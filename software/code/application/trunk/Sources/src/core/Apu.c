/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		Apu.c
**	Abstract:		APU info handle
**	Creat By:		Zeno Liu
**	Creat Time:		2015.03.09
**	Version:		v1.0
**
******************************************************************************/

/* \brief
	include files
*/
#include "Config.h"
#include "Apu.h"
#include "ApuDriver.h"
#include "Error.h"
#include "SoftTimer.h"
#include "io.h"
#include "Version.h"
#include "MCU_APU_UICC_Def_V0.1.9.h"
#include "system.h"
#include "common.h"
#include "Timer.h"
#include "memory.h"
#include "eeprom.h"
#include "Key.h"
#include "Pwm.h"
#include "Avm.h"
#include "Bcm.h"
#include "Ac.h"
#include "Amp.h"
#include "Gps.h"
#include "InsApp.h"
#include "AirCondition.h"
#include "PortInterrupt.h"
#include "system.h"
#include "DiagAppLayer.h"
#include <MC9S08DZ60.h>
#include <hidef.h>

/* \brief
	marco define
*/
#define	apuINIT_MAX_RETRY_NUM			3

/* \brief
	update cmd
*/
typedef enum
{
	APUR_UPDATE_CMD						=0x01,
	APUR_UPDATE_KEY,
	APUR_UPDATE_DATA,
	APUR_UPDATE_CHECKSUM,
	APUR_UPDATE_RET_READY,
	APUR_UPDATE_CONFIG_DATA
}enumApurUpdate;


/* \brief
	apu initialization state
*/
typedef enum
{
	APU_INIT_IDLE,
	APU_INIT_POWER_ON,
	APU_INIT_RESET,
	APU_INIT_IS_AWAKE,
	APU_INIT_QUERY_APU_STATUS,
	APU_INIT_IS_READY,
	APU_INIT_SETTING1,
	APU_INIT_SETTING2,
	APU_INIT_SETTING3,
	APU_INIT_SETTING4,
	APU_INIT_SETTING5,
	APU_INIT_SETTING6,
	APU_INIT_SYS_INFO,
	APU_INIT_RADIO_DATA,
	APU_INIT_SOURCE,
	APU_INIT_DONE
}enumApuInitState;

/* \brief
	variable declaration
*/
BOOL isMonitorOn = FALSE;
ApuSetting xApuSetting;
ApuwDeviceStatus xApuwDeviceStatus;
ApurDeviceStatus xApurDeviceStatus;
enumApuwMcuOsUpdate eApuwMcuOsUpdate;

static enumApuInitState eApuInitState = APU_INIT_POWER_ON;
static BOOL isApuInitComplete = FALSE;
static BOOL isApuUpdateStart = FALSE;
static BYTE ApuInitResetRetry = 0;
static BYTE ApuInitAwakeRetry = 0;
static BYTE ApuDelayMsTimer = 0;
static BYTE ApuDelaySecTimer = 0;
static BYTE ApuDelaySecCount = 0;
static BYTE ApuDelayMonitorOnTimer = 0;
static WORD ApuWatchDogTimer = apuWATCHDOG_TIMER;

static BOOL isApuOsReady = FALSE;
static BOOL isApuInitTwice = FALSE;
//static BOOL isApuRestart = FALSE;

static BOOL isApuMcuUpdate = FALSE;
static BYTE ApuMcuUpdateTimeoutTimer = 0;
static BYTE ApuOsUpdateRestartTimer = 0;
static BYTE ApuOsUpdateTimeoutTimer = 0;


static BYTE ApuUsbCtlDelayTimer = 0;
static BYTE txPowerCount = 0;

static BOOL isApuWatchDogEnable = OFF;



static BYTE NVBACKKEY[8] @0x0000FFB0;

//static ApuTime xApuTime;

/* \brief
	function declaration
*/
static void vApuInfoHandle(void);
static void vApuInitTask(void);
static void vApuCommandHandle(BYTE* pData);
static void vApuSysInfoHandle(BYTE* pData);
static void vApuSettingHandle(BYTE* pData);
static void vApuTimeDataHandle(BYTE* pData);
static void vApuMediaInfoHandle(BYTE* pData);
static void vApuCanControlHandle(BYTE* pData);
static void vApuDeviceStatusHandle(BYTE* pData);
static void vApuMcuUpdateHandle(BYTE* pData);
static void vApuCoordinationHandle(BYTE* pData);
static void vApuMcuUpdateCmd(BYTE* pData);
static void vApuAirConditonCmdHandle(BYTE* pData);

/* \brief
	init MCU module
*/
void vApuInit(void)
{
	vApuDrvInit();

	//xApuSetting init
//	IOBLcdControl(ON);
	
	//xApuwDeviceStatus init
	
	eApuInitState = APU_INIT_POWER_ON;
	isApuInitComplete = FALSE;
	isApuUpdateStart = FALSE;
	eApuwMcuOsUpdate = APUW_MCU_OS_UPDATE_IDEL;
	ApuInitResetRetry = 0;
	ApuInitAwakeRetry = 0;
	ApuOsUpdateTimeoutTimer = 0;
	ApuOsUpdateRestartTimer = 0;

	isApuOsReady = FALSE;
	isMonitorOn = FALSE;

	vApuWatchDogEnable(OFF);
	vApuWatchDogFeed();
}

/* \brief
*/
void vApuTask(void)
{
	vApuDrvTask();
	
	vApuInfoHandle();

	vApuInitTask();
}

/* \brief
*/
void vApuTimer(void)
{
	vApuDrvTimer();

	if(0 != ApuWatchDogTimer)
	{
		if(isApuWatchDogEnable && (SYSTEM_ACC_IDLE == vSystemAccStatus()) && (ioBAT_DET_IN))
		{
			if(--ApuWatchDogTimer == 0)
			{
				vResetMcuJumpBgnd();
			}
		}
	}

	if(0 != ApuDelayMsTimer)
	{
		--ApuDelayMsTimer;
	}

	if((0 != ApuDelaySecTimer) && (softtimerSECOND_COUNT <= ApuDelaySecCount++))
	{
		ApuDelaySecCount -= softtimerSECOND_COUNT;
		
		--ApuDelaySecTimer;
	}

	if(0 != ApuMcuUpdateTimeoutTimer)
	{
		if(--ApuMcuUpdateTimeoutTimer == 0)
		{
			isApuMcuUpdate = FALSE;
		}
		else if(0 == (ApuMcuUpdateTimeoutTimer%20))
		{
			BYTE data = 0x02;
			vApuWrite(APUW_GID_MCU_UPDATE, APUW_MCU_UPDATE_CONFIRM, &data, 1);
			vApuWrite(APUW_GID_MCU_UPDATE, APUW_MCU_UPDATE_REQ_KEY, NULL, 0);
		}
	}

	if(0 != ApuUsbCtlDelayTimer)
	{
		if(--ApuUsbCtlDelayTimer == 0)
		{
			ioUSB_OTG_POWER_ON;
//			ioUSB_HOST_POWER_ON;
		}
	}
}

/* \brief
*/
void vApuTimerSecond(void)
{
	if(0 != ApuOsUpdateRestartTimer)
	{
		if(isApuUpdateStart && (SYSTEM_ACC_IDLE == vSystemAccStatus()) && (ioBAT_DET_IN))
		{
			if(--ApuOsUpdateRestartTimer == 0)
			{
				isApuUpdateStart = FALSE;
				vResetMcuJumpBgnd();
			}
		}
	}
}

/* \brief
*/
void vApuTimerMinute(void)
{
	if(0 != ApuOsUpdateTimeoutTimer)
	{
		if(--ApuOsUpdateTimeoutTimer == 0)
		{
			isApuUpdateStart = FALSE;
			eApuwMcuOsUpdate = APUW_MCU_OS_UPDATE_IDEL;
		}
	}
}

/* \brief
	pData:group id+sub id+data
*/
void vApuWrite(BYTE GroupId, BYTE SubId, BYTE* pData, BYTE length)
{
	vApuDrvWrite(GroupId, SubId, pData, length);
	
	return;
}

/* \brief
*/
BOOL isApuInit(void)
{
	return isApuInitComplete;
}

/* \brief
*/
enumApuwMcuOsUpdate isApuUpdate(void)
{
	return eApuwMcuOsUpdate;
}

/* \brief
*/
void vApuWriteBuffClear(void)
{
	vApuDrvWriteBuffClear();
}

/* \brief
*/
static void vApuInfoHandle(void)
{
	BYTE* pInfo = pApuDrvRead();

	if(NULL == pInfo)
	{
		return;
	}

	switch(pInfo[3])
	{		//Group id
		case APUR_GID_CMD:
			vApuCommandHandle(pInfo);
			break;
			
		case APUR_GID_SYS_INFO:
			vApuSysInfoHandle(pInfo);
			break;
			
		case APUR_GID_SETTING:
			vApuSettingHandle(pInfo);
			break;
			
		case APUR_GID_TIME_DATA:
			vApuTimeDataHandle(pInfo);
			break;
			
		case APUR_GID_MEDIA_INFO:
			vApuMediaInfoHandle(pInfo);
			break;
			
		case APUR_GID_CAN_CTL:
			vApuCanControlHandle(pInfo);
			break;
			
		case APUR_GID_DEVICE_STATUS:
			vApuDeviceStatusHandle(pInfo);
			break;
			
		case APUR_GID_SWC_CTL:
			break;
			
		case APUR_GID_TPMS_CTL:
			break;
			
		case APUR_GID_MCU_UPDATE:
			vApuMcuUpdateHandle(pInfo);
			break;
			
		case APUR_GID_CAN_UPDATE:
			break;
			
		case APUR_GID_FCT_TEST:
			break;
			
		case APUR_GID_CAN_BUS_DATA:
			break;

		case APUR_GID_RECORD:
			break;
			
		case APUR_GID_ERROR_RECORD:		//uart return error record
			if(0x01 == pInfo[6])
			{
				vApuWrite(APUW_GID_ERROR_RECORD, 0x01, (BYTE*)&xErrorRecord, sizeof(xErrorRecord));
			}
			break;

		case APUR_GID_BACK_KEY:
			if(0x01 == pInfo[6])
			{
				FCNFG_KEYACC = 1;
				
				NVBACKKEY[0] = pInfo[7];
				NVBACKKEY[1] = pInfo[8];
				NVBACKKEY[2] = pInfo[9];
				NVBACKKEY[3] = pInfo[10];
				NVBACKKEY[4] = pInfo[11];
				NVBACKKEY[5] = pInfo[12];
				NVBACKKEY[6] = pInfo[13];
				NVBACKKEY[7] = pInfo[14];

				FCNFG_KEYACC = 0;
			}
			break;

		default:
			++xErrorRecord.ApuReadGroupIdUnknown;
			
			break;
	}
}

/* \brief
	INIT APU system
*/
static void vApuInitTask(void)
{
	switch(eApuInitState)
	{
		case APU_INIT_IDLE:
		default:
			break;
			
		case APU_INIT_POWER_ON:
			eApuInitState = APU_INIT_RESET;
			break;
			
		case APU_INIT_RESET:
			if(0 != ApuDelayMsTimer)
			{
				break;
			}


			++ApuInitResetRetry;
			
			ApuDelaySecTimer = 250;		//add timer, prevent power later cause awake donot setting
			ApuDelaySecCount = 0;		//clear second count
			
			eApuInitState = APU_INIT_IS_AWAKE;
			break;
			
		case APU_INIT_IS_AWAKE:
			if(ioAPU_STATUS_AWAKE)
			{
				vAmpStbyEnable(ON);
				if((SYSTEM_ACC_IDLE == vSystemAccStatus())  && (ioBAT_DET_IN))
				{
					IOBLcdControl(ON);
					isMonitorOn = TRUE;
				}
				
				ApuDelayMonitorOnTimer = 200;

				ApuDelaySecTimer = 250;//80;//30;	//80 for first power on,150 for D50 Restore or first power on
				ApuDelaySecCount = 0;		//clear second count

				ApuDelayMsTimer = 200;		//for rewrite query status

				++ApuInitAwakeRetry;
				ApuInitResetRetry = 0;
				
				eApuInitState = APU_INIT_IS_READY;


			}
			else if(0 == ApuDelaySecTimer)
			{
				if(apuINIT_MAX_RETRY_NUM > ApuInitResetRetry)
				{
					eApuInitState = APU_INIT_POWER_ON;
//					ioAPU_HW_RESET_HIGH;
					ApuDelayMsTimer = softtimerMILLISECOND(500);
				}
				else
				{	//reset fail
					eApuInitState = APU_INIT_IDLE;
					++xErrorRecord.ApuResetFailError;
				}
			}
			break;

		case APU_INIT_QUERY_APU_STATUS:
			if(0 == ApuDelaySecTimer)
			{
				BYTE data = 0x00;
				vApuWrite(APUW_GID_CMD, APUW_CMD_QUERY_STATUS, &data, 1);

				ApuDelaySecTimer = 150;//80;//30;	//80 for first power on,150 for D50 Restore or first power on
				ApuDelaySecCount = 0;		//clear second count

				ApuDelayMsTimer = 200;		//for rewrite query status

				++ApuInitAwakeRetry;
				ApuInitResetRetry = 0;
				
				eApuInitState = APU_INIT_IS_READY;
			}
			break;
			
		case APU_INIT_IS_READY:
			if(isApuOsReady)
			{
				eApuInitState = APU_INIT_SETTING1;

				ApuDelaySecTimer = 0;
			}
			else if(0 == ApuDelaySecTimer)
			{
				if(apuINIT_MAX_RETRY_NUM > ApuInitAwakeRetry)
				{
					eApuInitState = APU_INIT_POWER_ON;
					ApuDelayMsTimer = softtimerMILLISECOND(500);
				}
				else
				{	//reset fail
					eApuInitState = APU_INIT_IDLE;
					++xErrorRecord.ApuAwakeFailError;
				}
			}
			break;
			
		case APU_INIT_SETTING1:
			if(0 != ApuDelayMsTimer)
			{
				break;
			}
			vApuWrite(APUW_GID_SYS_INFO, APUW_SYSINFO_VERSION, (BYTE*)pVersion, verVERION_MAX_NUM);
			ApuDelayMsTimer = softtimerMILLISECOND(100);
//			vAmpStbyEnable(ON);
			eApuInitState = APU_INIT_SETTING2;
			break;

		case APU_INIT_SETTING2:
			if(0 != ApuDelayMsTimer)
			{
				break;
			}
			else
			{
				
				BYTE data[2] ={0};
				data[0] = UICC_PWR_STATE;
				data[1] = xSystemPowerState.Byte;
				vApuWrite(APUW_GID_CMD, APUW_CMD_COMMAND, data, 2);
			}
			ApuDelayMsTimer = softtimerMILLISECOND(100);
			if(++txPowerCount > 2)
			{
				txPowerCount = 0;
				eApuInitState = APU_INIT_SETTING3;
			}
			
			break;
		case APU_INIT_SETTING3:
			if(0 != ApuDelayMsTimer)
			{
				break;
			}
			else
			{
				BYTE data[2] ={0};
				data[0] = UICC_PWR_VOLT;
				data[1] = (BYTE)eSystemPowerVoltageState;
				vApuWrite(APUW_GID_CMD, APUW_CMD_COMMAND, data, 2);
			}
			ApuDelayMsTimer = softtimerMILLISECOND(100);
			
			eApuInitState = APU_INIT_SETTING4;
			break;
		case APU_INIT_SETTING4:
			if(0 != ApuDelayMsTimer)
			{
				break;
			}
			vApuWrite(APUW_GID_CAN_INFO, APUW_CAN_AC, (BYTE*)&xAirConditionInfo, sizeof(xAirConditionInfo));
			
			ApuDelayMsTimer = softtimerMILLISECOND(100);
			eApuInitState = APU_INIT_SETTING5;
			break;
			
		case APU_INIT_SETTING5:
			if(0 != ApuDelayMsTimer)
			{
				break;
			}
			vBcmTxApuTrunkState();
			ApuDelayMsTimer = softtimerMILLISECOND(100);
			eApuInitState = APU_INIT_SETTING6;
			break;
			
		case APU_INIT_SETTING6:
			if(0 != ApuDelayMsTimer)
			{
				break;
			}
			vBcmTxApuSurState();
			ApuDelayMsTimer = softtimerMILLISECOND(100);
			eApuInitState = APU_INIT_SYS_INFO;
			break;
			
		case APU_INIT_SYS_INFO:
			if(0 != ApuDelayMsTimer)
			{
				break;
			}
			vDiagAppTxVinTask();
			ApuDelayMsTimer = softtimerMILLISECOND(50);
			if(!isMonitorOn)
			{
				IOBLcdControl(ON);
				isMonitorOn = TRUE;
			}
			eApuInitState = APU_INIT_DONE;
			break;
			
		case APU_INIT_SOURCE:

			break;
			
		case APU_INIT_DONE:
			if(0 != ApuDelayMsTimer)
			{
				break;
			}
			isApuInitComplete = TRUE;
			eApuInitState = APU_INIT_IDLE;

			break;
	}
}

/* \brief
	CMD handle
*/
static void vApuCommandHandle(BYTE* pData)
{
	if(NULL == pData)
	{
		return;
	}

	switch(pData[7])
	{
		case UICC_MUTE:
			switch(pData[8])
			{
				case 0x00:
					if(!xApuwDeviceStatus.MuteOn)
					{
						xApuwDeviceStatus.MuteOn = TRUE;
					}
					vApuWrite(APUW_GID_SYS_INFO, APUW_SYSINFO_DEVICE_STATUS, (BYTE*)&xApuwDeviceStatus, sizeof(xApuwDeviceStatus));
					break;

				case 0x01:
					if(xApuwDeviceStatus.MuteOn)
					{
						xApuwDeviceStatus.MuteOn = FALSE;
						
					}
					vApuWrite(APUW_GID_SYS_INFO, APUW_SYSINFO_DEVICE_STATUS, (BYTE*)&xApuwDeviceStatus, sizeof(xApuwDeviceStatus));
					break;
				case 0x02:
					if(vSystemAccStatus() == SYSTEM_ACC_IDLE)
					{
						vAmpMuteHardware(ON);
					}
					
					break;

				case 0x03:
					if(vSystemAccStatus() == SYSTEM_ACC_IDLE)
					{
						vAmpMuteHardware(OFF);
					}
					break;
			}
			break;
			
		case UICC_BAND_TS:
		case UICC_ASPS_LONG_TS:
		case UICC_PREV_TS:
		case UICC_NEXT_TS:
		case UICC_STEP_UP_TS:
		case UICC_STEP_DOWN_TS:
		case UICC_SCAN_TS:
		case UICC_STOP:

			break;
			
		case UICC_FRONT_ZONE_TS:
			break;

		case UICC_BT_PHONE_AUDIO:
		case UICC_3G_PHONE_AUDIO:
			break;

		case UICC_MONITOR_OP:

			//donot allow adjust backlight at reverse state
			if(xApuwDeviceStatus.ReverseFlag)
			{
				break;
			}
			break;

		case UICC_APP_PROMPT_TONE:
			switch(pData[8])
			{
				case 0:
					break;
					
				case 1:
					break;

				case 3:			//call in
					break;
			}
			break;

		case UICC_NAVI_AUDIO:
			break;

		case UICC_RESTORE_DEFAULT:
			vSystemRestoreDefault();
			break;

		case UICC_BEEP_ONLY_TS:			//none beep noise
			break;

		case UICC_SYS_STATE:
			break;

		case UICC_USB_VBUS:
			switch(pData[8])
			{
				case 0x00:
					ioUSB_OTG_POWER_OFF;
//					ioUSB_HOST_POWER_OFF;
					ApuUsbCtlDelayTimer = 0;
					break;

				case 0x01:
					ioUSB_OTG_POWER_ON;
//					ioUSB_HOST_POWER_ON;
					ApuUsbCtlDelayTimer = 0;
					break;

				case 0x02:
					ioUSB_OTG_POWER_OFF;
//					ioUSB_HOST_POWER_OFF;
					ApuUsbCtlDelayTimer = 50;
					break;

				default:
					break;
			}
			break;
		case UICC_APU_ACTIVE:
			switch(pData[8])
			{
				default:
					break;
				case 0:
					vBcmHomeLightCtrl(OFF);
					break;
				case 1:		//active
					vBcmHomeLightCtrl(ON);
					break;
			}
			break;
		default:
			break;
	}
}

/* \brief
	system info handle
*/
static void vApuSysInfoHandle(BYTE* pData)
{
	if(NULL == pData)
	{
		return;
	}
	
	switch(pData[6])
	{
		case APUR_SYSINFO_STATUS:
			switch(pData[7])
			{
				case APUR_STATUS_STARTING:
					vApuWatchDogFeed();
					if(!isApuWatchDogEnable)
					{
						vApuWatchDogEnable(ON);
					}
					break;
				case APUR_STATUS_MENU_ON:
					break;
					
				case APUR_STATUS_OS_READY:
					if(isSystemInitCompleted && (APU_INIT_IDLE == eApuInitState))
					{
						isApuInitComplete = FALSE;
						isSystemInitCompleted = FALSE;
						ApuDelayMsTimer = 5;
						eApuInitState = APU_INIT_SETTING1;
						
					}
					isApuOsReady = TRUE;
					break;

				case APUR_STATUS_MAIN_MENU_ON:
				case APUR_STATUS_MAIN_MENU_OFF:
				case APUR_STATUS_SETUP_ON:
				case APUR_STATUS_EQ_ON:
				case APUR_STATUS_IPOD_VIDEO_ON:
				case APUR_STATUS_POWER_OFF_READY:
				case APUR_STATUS_IPOD_AUDIO_ANALOG:
				case APUR_STATUS_IPOD_AUDIO_DIGITAL:
				case APUR_STATUS_SETUP_PASS_OK:
				case APUR_STATUS_APU_REQ_UPDATE:
					break;

				case APUR_STATUS_APU_UPDATE_START:
					eApuwMcuOsUpdate = APUW_MCU_OS_UPDATE_START;
					isApuUpdateStart = TRUE;
					ApuOsUpdateTimeoutTimer = 5;
					break;

				case APUR_STATUS_APU_UPDATE_END:
					eApuwMcuOsUpdate = APUW_MCU_OS_UPDATE_END;
					ApuOsUpdateRestartTimer = 2;
					break;

				case APUR_STATUS_APU_SYS_UNLOCKED:
				case APUR_STATUS_APU_SYS_LOCKED:
				default:
					break;
			}
			break;
			
		case APUR_SYSINFO_OS_TYPE:
			break;
			
		default:
			break;
	}
}

/* \brief
	setting handle
*/
static void vApuSettingHandle(BYTE* pData)
{
	if(NULL == pData)
	{
		return;
	}
	
	switch(pData[6])
	{
		case APUR_SET_GENERAL:
			switch(pData[7])
			{
				case APUR_SET_GENERAL_RADIO_AREA:

					break;

				case APUR_SET_GENERAL_AUTO_DIMMER:
					break;
					
				default:
					break;
			}
			break;

		case APUR_SET_AUDIO:
			break;
			
		case APUR_SET_GEQ:
			switch(pData[7])
			{
				case 0:
					break;

				default:
					break;
			}
			break;

		case APUR_SET_SCREEN:
			if((SYSTEM_ACC_IDLE != vSystemAccStatus())  || (!ioBAT_DET_IN))
			{//starting or shutting down donot control backlight to avoid flicking screen
				return;
			}
			switch(pData[7])
			{

				case 0:
					xApuSetting.xApuScreen.ScreenBrightness = pData[8];
					if(!isDiagAppIOIllume())
					{
						vPwmMonitorControl(TRUE, xApuSetting.xApuScreen.ScreenBrightness);
					}
					
					break;

				case 1:
					
					if(0 == pData[8])
					{
						if(!isDiagAppIOIllume())
						{
							IOBLcdControl(OFF);
							isMonitorOn = FALSE;
						}
					}
					else
					{
						if(!isDiagAppIOIllume())
						{
							IOBLcdControl(ON);
							isMonitorOn = TRUE;
						}
					}
					break;
					
				default:
					break;
			}
//			vApuWrite(APUW_GID_SETTING, APUW_SETTING_SCREEN, (BYTE*)&xApuSetting.xApuScreen, sizeof(xApuSetting.xApuScreen));
			break;

		case APUR_SET_TUNER_FREQ:
			break;
			
		default:
			break;
	}

}

/* \brief
	obtain GPS time
*/
static void vApuTimeDataHandle(BYTE* pData)
{
	if(NULL == pData)
	{
		return;
	}
	
	switch(pData[6])
	{
		case 0x01:		//time
			vGpsApuSet(APUW_SET_TIME, &pData[7]);

			break;

		case 0x02:		//date
			vGpsApuSet(APUW_SET_DATE, &pData[7]);
			break;

		case 0x03:
		case 0x04:
			vInsApuNaviSet(&pData[4]);
			break;

		default:
			break;
	}
}

/* \brief
*/
static void vApuMediaInfoHandle(BYTE* pData)
{
	if(NULL == pData)
	{
		return;
	}
	vInsApuPlayInfoSet(&pData[4]);
}

/* \brief
	can control handle
*/
static void vApuCanControlHandle(BYTE* pData)
{
	if(NULL == pData)
	{
		return;
	}

	switch(pData[6])
	{
		case APUR_CAN_CTL_AIR_CONDITION_CMD:
			if(0x01 == pData[9])
			{
				vAcApuSet(&pData[7]);
			}
			else
			{
				vAirApuSet(&pData[7]);
			}
			break;
			
		case APUR_CAN_CTL_CENTER_CTL_CMD:
			break;
			
		case APUR_CAN_CTL_RADAR_CMD:
			break;

		case APUR_CAN_CTL_COORDINATION:
			break;

		case APUR_CAN_CTL_NAVI_INFO:
			break;
		case APUR_CAN_CTL_AVM_INFO:
//			vApuAvmCmdHandle(&pData[7]);
			vAvmApuSet(&pData[7]);
			break;
		case APUR_CAN_CTL_BCM_INFO:
//			vApuBcmCmdHandle(&pData[7]);
			vBcmApuSurSet(&pData[7]);
			break;
		case APUR_CAN_CTL_PLG_INFO:
			vBcmApuTrunkSet(&pData[7]);
			break;

		case APUR_CAN_CTL_CONFIG_INFO:
			break;
			
		default:
			break;
	}
}

/* \brief
	device status handle
*/
static void vApuDeviceStatusHandle(BYTE* pData)
{
	if(NULL == pData)
	{
		return;
	}
	
	switch(pData[6])
	{
		case APUR_DEVICE_STATUS:
			break;
			
		case APUR_DEVICE_TYPE:
			break;
			
		case APUR_DEVICE_INFO:
			break;
			
		case APUR_DEVICE_STATUS_REQ:
			break;

		default:
			break;
	}

}

/* \brief
	update handle
*/
static void vApuMcuUpdateHandle(BYTE* pData)
{
	if(NULL == pData)
	{
		return;
	}
	
	switch(pData[6])
	{
		case APUR_UPDATE_CMD:
			vApuMcuUpdateCmd(&pData[7]);
			vAmpMuteHardware(ON);
			break;
			
		case APUR_UPDATE_KEY:
			if(!isApuMcuUpdate)
			{
				break;
			}
			if((0x56 == pData[7]) && (0xAE == pData[8]))
			{
				vResetMcuJumpBoot();
			}
			break;
			
		case APUR_UPDATE_DATA:
			break;
			
		case APUR_UPDATE_CHECKSUM:
			break;
			
		case APUR_UPDATE_RET_READY:
			break;
			
		case APUR_UPDATE_CONFIG_DATA:
			break;

		default:
			break;
	}
}


/* \brief
*/
static void vApuCoordinationHandle(BYTE* pData)
{
	if(NULL == pData)
	{
		return;
	}

	switch(pData[0])
	{
		case 0x00:		//DVR
			break;

		case 0x01:		//AVM
//			vCoprocessorAvmCoordination(&pData[1]);
			break;

		default:
			break;
	}
}

/* \brief
	update cmd
*/
static void vApuMcuUpdateCmd(BYTE* pData)
{
	switch(pData[0])
	{
		case 0x02:
			{
				isApuMcuUpdate = TRUE;
				ApuMcuUpdateTimeoutTimer = 200;
			}
			break;

		default:
		case 0x01:
		case 0x03:
			break;
	}
}


/* \brief
*/
static void vApuAirConditonCmdHandle(BYTE* pData)
{

	(void)(pData == NULL);

}

/* \brief
	apu watchDog reset
*/
void vApuWatchDogFeed(void)
{
	ApuWatchDogTimer = apuWATCHDOG_TIMER;
}

/* \brief
	apu watchDog reset
*/
void vApuWatchDogEnable(BOOL enable)
{
	isApuWatchDogEnable = enable;
}



