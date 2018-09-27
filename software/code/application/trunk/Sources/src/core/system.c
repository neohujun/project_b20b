/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		system.c
**	Abstract:		system initialization
**	Creat By:		Zeno Liu
**	Creat Time:		2015.03.04
**	Version:		v1.0
**
******************************************************************************/

/* \brief
	include files
*/
#include "Config.h"
#include "system.h"
#include "Apu.h"
#include "hdinit.h"
#include "SoftTimer.h"
#include "CanApp.h"
#include "common.h"
#include "memory.h"
#include "eeprom.h"
#include "Version.h"
#include "io.h"
#include "Key.h"
#include "derivative.h"
#include "Error.h"
#include "list.h"
#include "Timer.h"
#include "uart.h"
#include "Pwm.h"
#include "Radar.h"
#include "PortInterrupt.h"
#include "Tpm.h"
#include "iic.h"
#include "Display.h"
#include "TDF85xx.h"
#include "Avm.h"
#include "Amp.h"
#include "InsApp.h"
#include "DiagAppLayer.h"
#include "NetworkManager.h"
#include "DiagDTC.h"
#include <hidef.h>


/* \brief
	variable
*/
const BYTE* pVersion = verVERION;
static enumSystemAccState eSystemAccState = SYSTEM_ACC_IDLE;
static WORD wSystemAccDelayTimer = 0;

BOOL isSystemInitCompleted = FALSE;

SystemPowerState xSystemPowerState;
enumSystemPowerVoltageState eSystemPowerVoltageState;

BOOL isSystemBatError = FALSE;
BOOL SystemBatErrorTimer;

/* \brief
*/
static void vSystemAccTask(void);
static void vSystemBatTask(void);
static void vSystemExternSignalDetectTask(void);
static void vSystemApuStatusDetect(BOOL isApuSleep);

/* \brief
	system initialization
*/
void vSystemInit(void)
{
	DisableInterrupts;

	vCommonInit();
	vTimerInit();
	vHdInit();
	vSoftTimerInit();
	vKeyInit();
	vApuInit();
//APP donot use eeprom, rm it to accelerate for start up
//	vMemoryAllRead();
	
	vPwmStart();
	vCanAppInit();

	vDisplayInit();
	vInsAppTaskInit();
	xApuSetting.xApuScreen.ScreenBrightness = apuMONITOR_BRIGHTNESS;
	vPwmMonitorControl(TRUE, xApuSetting.xApuScreen.ScreenBrightness);

	vDiagAppTaskInit();
	vNMTaskInit();
	
//	vAmpMuteHardware(OFF);

	xSystemPowerState.Bits.StartType = SYSTEM_START_COLD;
	xSystemPowerState.Bits.AccStatus = SYSTEM_STATUS_ACC_ON;
	
	isHardwareInitCompleted = TRUE;
	isMonitorOn = FALSE;
	xApuwDeviceStatus.LCDDisp = TRUE;
	EnableInterrupts;
}

/* \brief
	system task
*/
void vSystemTask(void)
{
	vSoftTimerTask();

	vApuTask();

	vCanAppTask();

	vSystemAccTask();
	
	vKeyTask();

	vDisplayTask();

	vDiagAppTask();

	//wakeup of networking will not be supported until hardware version 4

	vNMTask();

	vInsAppTask();

	
	if(isSystemInitCompleted)
	{
		vSystemExternSignalDetectTask();
	}
	else if(isApuInit())
	{
		isSystemInitCompleted = TRUE;
	}

}

/* \brief
*/
void vSystemTimer(void)
{	
	if(0 != wSystemAccDelayTimer)
	{
		--wSystemAccDelayTimer;
	}

	if(0 != SystemBatErrorTimer)
	{
		--SystemBatErrorTimer;
	}
}


/* \brief
	speed up enter reversing after start up 
*/
void vSystemQuickReverse(void)
{
/*
	if(ioBACK_IN)
	{
		ioCAMERA_POWER_ON;
		ioAPU_BACK_ON;
	}
*/
}

/* \brief
	restart when restore factory setting complete
*/
void vSystemRestoreAllDefault(void)
{
//	vAmpMuteSoftware(TRUE);

	//delay restarting, avoid ueser feel too fast
}

/* \brief
	not restart when restore factory setting comlete
*/
void vSystemRestoreDefault(void)
{
	vAmpMuteHardware(ON);
	xApuSetting.xApuScreen.ScreenBrightness = apuMONITOR_BRIGHTNESS;
	xSystemPowerState.Bits.StartType = SYSTEM_START_NORMAL;
	xSystemPowerState.Bits.AccStatus = SYSTEM_STATUS_ACC_ON;
	//source

	//resynchronizate after reset
	isSystemInitCompleted = FALSE;

//	ioAUDIO_AMP_MUTE_OFF;
}

void vSystemRestoreInit(void)
{
	DisableInterrupts;

	vCommonInit();
	
	vHdInit();
	vSoftTimerInit();
	vKeyInit();
	vApuInit();
	vInsAppTaskInit();
//	vMemoryAllRead();
	
	vPwmStart();
	vCanAppInit();

	vDisplayInit();
	vPwmMonitorControl(TRUE, xApuSetting.xApuScreen.ScreenBrightness);

	vDiagAppTaskInit();
	vNMTaskInit();
	
//	vAmpMuteHardware(OFF);

	xSystemPowerState.Bits.StartType = SYSTEM_START_NORMAL;
	xSystemPowerState.Bits.AccStatus = SYSTEM_STATUS_ACC_ON;
	isHardwareInitCompleted = TRUE;
	isMonitorOn = FALSE;
	xApuwDeviceStatus.LCDDisp = TRUE;
	EnableInterrupts;
}

/* \brief
*/
static void vSystemBatTask(void)
{
	if(!isSystemBatError)
	{
		if(!ioBAT_DET_IN)
		{
			isSystemBatError = TRUE;
			SystemBatErrorTimer = 120;
		}
		else
		{
			vDiagDTC(DIAG_DTC_VOLTAGE_HIGH_EXCEPTION, DIAG_DTC_CTL_SUCESS);
		}
	}
	else
	{
		if(!ioBAT_DET_IN)
		{
			if(0 == SystemBatErrorTimer)
			{
				vAmpMuteHardware(ON);
				IOBLcdControl(OFF);
				isMonitorOn = FALSE;
				
				wSystemAccDelayTimer = 0;
				vPwmMonitorControl(FALSE, xApuSetting.xApuScreen.ScreenBrightness);
				ioAUDIO_AMP_STBY_L;
				eSystemAccState = SYSTEM_ACC_OFF_DONE;

				vDiagDTC(DIAG_DTC_VOLTAGE_HIGH_EXCEPTION, DIAG_DTC_CTL_FAIL);
			}
		}
		else
		{
			if(SystemBatErrorTimer >= 80)
			{
				vDisplayReset();
				wSystemAccDelayTimer = 0;
				isSystemBatError = FALSE;
			}
		}
	}
}


/* \brief
	system acc task
*/
static void vSystemAccTask(void)
{
	vSystemBatTask();
	
	switch(eSystemAccState)
	{
		case SYSTEM_ACC_IDLE:
			if(!ioACC_IN)
			{
				if(ioAPU_POWER_IS_OFF)
				{
					wSystemAccDelayTimer = 0;
					eSystemAccState = SYSTEM_ACC_OFF_WAIT;
					break;
				}
				else
				{
					wSystemAccDelayTimer = 120;
					eSystemAccState = SYSTEM_ACC_OFF_CHK;
				}
			}
			break;

		case SYSTEM_ACC_OFF_CHK:
			if((0 == wSystemAccDelayTimer) && (APUW_MCU_OS_UPDATE_START != isApuUpdate()))
			{
				BYTE data[2] ={0};
				data[0] = UICC_PWR_STATE;
				xSystemPowerState.Bits.AccStatus = SYSTEM_STATUS_ACC_OFF;
				data[1] = xSystemPowerState.Byte;
				
				vApuWrite(APUW_GID_CMD, APUW_CMD_COMMAND, data, 2);

				vAmpMuteHardware(ON);
				IOBLcdControl(OFF);
				isMonitorOn = FALSE;

				//allow sleep after timeout 1.2s
				vNMSleepAllow(TRUE);

				wSystemAccDelayTimer = 1000;
				eSystemAccState = SYSTEM_ACC_OFF;
			}
			else if(ioACC_IN)
			{
				wSystemAccDelayTimer = 0;
				eSystemAccState = SYSTEM_ACC_IDLE;
			}
			break;
			
		case SYSTEM_ACC_OFF:
			if((0 == wSystemAccDelayTimer) && isNMSleep())
			{
				BYTE data[2] ={0};
				data[0] = UICC_POWER_OFF;
				data[1] = 0x00;
				
				vApuWrite(APUW_GID_CMD, APUW_CMD_COMMAND, data, 2);
				ioAPU_SLEEP;

				wSystemAccDelayTimer = 400;
				eSystemAccState = SYSTEM_ACC_OFF_WAIT;
			}
			else if(ioACC_IN)
			{
				BYTE data[2] ={0};
				data[0] = UICC_PWR_STATE;
				xSystemPowerState.Bits.AccStatus = SYSTEM_STATUS_ACC_ON;
				data[1] = xSystemPowerState.Byte;

				if(isNMSleep())
				{
					vNMTaskAwake(TRUE);
				}
				else
				{
					vNMSleepAllow(FALSE);
				}
				
				vApuWrite(APUW_GID_CMD, APUW_CMD_COMMAND, data, 2);
				wSystemAccDelayTimer = 150;
				eSystemAccState = SYSTEM_ACC_ON_WAIT;
			}
			break;
		case SYSTEM_ACC_ON_WAIT:
			if(0 != wSystemAccDelayTimer)
			{
				break;
			}
			vAmpMuteHardware(OFF);
			IOBLcdControl(ON);
			vApuWatchDogFeed();
			isMonitorOn = TRUE;

			eSystemAccState = SYSTEM_ACC_IDLE;
			break;
			
		case SYSTEM_ACC_OFF_WAIT:
			if((!ioAPU_STATUS_SLEEP) && (0 != wSystemAccDelayTimer))
			{
				break;
			}
			vPwmMonitorControl(FALSE, xApuSetting.xApuScreen.ScreenBrightness);
			wSystemAccDelayTimer = 100;		//waitting for OS power off complete
			ioAUDIO_AMP_STBY_L;
			eSystemAccState = SYSTEM_ACC_OFF_DONE;
			break;
			
		case SYSTEM_ACC_OFF_DONE:
			if(0 != wSystemAccDelayTimer)
			{
				break;
			}
			DisableInterrupts;
			
			vUart1Stop();
			vUart2Stop();
			
			ioAPU_POWER_OFF;
//			ioAPU_RESET_LOW;
			ioAPU_SLEEP;

			//ioAPU_SLEEP;
			ioSYSTEM_POWER_OFF;

//			ioTW88xx_Reset_LOW;
			ioLCD_BL_OFF;
//			ioTFT_RESET_L;
//			ioAPU_HW_RESET_LOW;
//			ioLIGHT_KEY_POWER_OFF;
			ioLIGHT_KEY_POWER_OFF_V4;

			ioUSB_OTG_POWER_OFF;
			ioUSB_HOST_POWER_OFF;

			ioAUDIO_AMP_MUTE_L;

			ioCAN_EN_ON;
			ioCAN_STBY_ON;
			ioCAN_WAKE_OFF;

			ioAPU_REV_ON;
			ioAPU_REV_POW_OFF;
			ioTw88xx_RESET_L;

			ioODO_FWD_FW;
			
			//enter STOP3, enable extern interrupt wake up Mcu reset
			while(((!ioBAT_DET_IN) || (!ioACC_IN)) && isNMSleep())
			{

				PTBSC &= ~0x02;		//disalbe port A interrupt
				PTBSC |= 0x04;		//clear PTBIF
				PTBES_PTBES2 = ~PTBD_PTBD2;
				PTBES_PTBES4 = ~PTBD_PTBD4;
				PTBES_PTBES0 = ~PTBD_PTBD0;
				PTBPS |= 0x15;
				
				PTBSC |= 0x04;
				PTBSC |= 0x02;		//enable extern interrupt
				
				
				_Stop;
				PTBSC &= ~0x02;
				PTBSC |= 0x04;		//clear PTBIF

				//CAN wakeup break the while
				if(ioCAN_INH)
				{
					break;
				}
			}


			//network management need wakeup time <50ms, but powerup starting time is 60ms
			//use direct wake up, time is 36ms
//#if 0
			eSystemAccState = SYSTEM_ACC_ON;
//#endif
//			asm(BGND);		//illegal instruction caused mcu reset
			break;
			
		case SYSTEM_ACC_ON:
			vSystemRestoreInit();
			eSystemAccState = SYSTEM_ACC_IDLE;
			break;

		default:
			break;
	}
}


/* \brief
*/
enumSystemAccState vSystemAccStatus(void)
{
	return eSystemAccState;
}

/* \brief
*/
static void vSystemExternSignalDetectTask(void)
{
	vSystemApuStatusDetect(ioAPU_STATUS_SLEEP);
}

/* \brief
*/
static void vSystemApuStatusDetect(BOOL isApuSleep)
{
	if((SYSTEM_ACC_IDLE != vSystemAccStatus())  || (!ioBAT_DET_IN))
	{
		return;
	}

	if(isApuSleep)
	{
		if(xApuwDeviceStatus.LCDDisp)
		{
			vAmpMuteHardware(ON);
			IOBLcdControl(OFF);
			xApuwDeviceStatus.LCDDisp = FALSE;
			vApuWatchDogEnable(OFF);
		}
	}
	else
	{
		if(!xApuwDeviceStatus.LCDDisp)
		{
			IOBLcdControl(ON);
			xApuwDeviceStatus.LCDDisp = TRUE;
		}
	}
}


