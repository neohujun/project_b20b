/******************************************************************************
**
**								Copyright (C) 2017  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		Bcm.c
**	Abstract:			Can BCM function
**	Creat By:		neo HU
**	Creat Time:		2017.7.1
**	Version:		v1.0
**
******************************************************************************/

/* \brief
	include
*/
#include "Bcm.h"
#include "List.h"
#include "CanApp.h"
#include "io.h"
#include "Apu.h"
#include <MC9S08DZ60.h>
#include "AirCondition.h"
#include "Pwm.h"
#include "type.h"
#include "Tw88xx_Drv.h"
#include "Amp.h"
#include "Timer.h"
#include "ApuWCmd.h"
#include "system.h"
#include "Avm.h"
#include "common.h"
#include "logo.h"
#include "DiagAppLayer.h"

/* \brief
*/
typedef enum
{
	BCM_STATE_WAIT,
	BCM_STATE_WORK,
	BCM_STATE_TX_CAN,
	BCM_STATE_TX_NONE,
	BCM_STATE_OFF
}enumBcmState;

/* \brief
*/
typedef enum
{
	REV_STATE_WAIT,
	REV_STATE_HOLD,
	REV_STATE_WORK,
	REV_STATE_ON,
	REV_STATE_OFF
}enumRevState;


/* \brief
*/
typedef enum
{
	LIGHT_STATE_WAIT,
	LIGHT_STATE_INIT,
	LIGHT_STATE_WORK,
	LIGHT_STATE_UP,
	LIGHT_STATE_DOWN,
	LIGHT_STATE_OFF
}enumHomeLightState;

/* \brief
*/
typedef enum
{
	BCM_SR_STATUS_NORMAL,
	BCM_SR_STATUS_FAIL
}enumBcmSurStatus;

/* \brief
*/
typedef enum
{
	BCM_SR_MOV_STOPPED,
	BCM_SR_MOV_START,
	BCM_SR_MOV_STOP
}enumBcmSurMov;

/* \brief
*/
typedef enum
{
	BCM_SR_POSITION_NONE,
	BCM_SR_POSITION_CLOSE,
	BCM_SR_POSITION_TILT_AREA,
	BCM_SR_POSITION_TILT,
	BCM_SR_POSITION_SLIDE_AREA,
	BCM_SR_POSITION_OPEN
}enumBcmSurPos;

/* \brief
*/
typedef enum
{
	BCM_SR_STATE_CLOSED,
	BCM_SR_STATE_OPENED,
	BCM_SR_STATE_START,
	BCM_SR_STATE_STOP
}enumBcmTxApuSurState;

/* \brief
*/
typedef struct
{
	WORD CurValue;
	WORD MaxValue;
	WORD MixValue;
	BYTE LevelValue;
}BcmLightValue;


/* \brief
*/
typedef struct
{
	BcmLightValue xLightValue;

	enumHomeLightState eLightState;
	
	BYTE parkLightOn;
	
}LightCommInfo;

/* \brief
*/
typedef struct
{
	enumBcmState eBcmState;
	CanFrame xCanTxMsg;

	enumBcmSurPos eBcmSurPos;
	enumBcmSurMov eBcmSurMov;
	
	LightCommInfo xLightInfo;

	enumRevState eRevState;

	BYTE txApuTrunkState;
	BYTE txApuSurState;
	
}BcmInfo;


/* \brief
*/
BcmInfo xBcmInfo;

/* \brief
*/
static BYTE wReverseDelayTime;
static BYTE wReverseRxDelayTime;
static BYTE wBcmCycleTime;
static BYTE wBcmCycleCount;
static BYTE wBcmLightCycleTime;


static BOOL rearRefrost = FALSE;


static BOOL trunkSwitch = FALSE;
static BOOL trunkMovement = FALSE;

/* \brief
*/
static void vHomeLightTask(void);
static void vReverseTask(void);

/* \brief
*/
void vBcmInit(void)
{
	xBcmInfo.xCanTxMsg.ID[0] = canID_TX_BCM_587_H;
	xBcmInfo.xCanTxMsg.ID[1] = canID_TX_BCM_587_L;
	xBcmInfo.xCanTxMsg.Length = 8;

	xBcmInfo.xCanTxMsg.Data[0] = 0x00;
	xBcmInfo.xCanTxMsg.Data[1] = 0x00;
	xBcmInfo.xCanTxMsg.Data[2] = 0x00;
	xBcmInfo.xCanTxMsg.Data[3] = 0x00;
	xBcmInfo.xCanTxMsg.Data[4] = 0x00;
	xBcmInfo.xCanTxMsg.Data[5] = 0x00;
	xBcmInfo.xCanTxMsg.Data[6] = 0x00;
	xBcmInfo.xCanTxMsg.Data[7] = 0x00;

	wReverseDelayTime = 0;
	wReverseRxDelayTime = 0;
	wBcmCycleTime = 0;
	wBcmCycleCount = 0;
	rearRefrost = FALSE;
	xBcmInfo.xLightInfo.parkLightOn = 0;
	wBcmLightCycleTime = 0;
	xApuwDeviceStatus.IllumiOn = FALSE;
	trunkSwitch = FALSE;
	
	trunkMovement = FALSE;

	xBcmInfo.xLightInfo.xLightValue.CurValue = 0;
	xBcmInfo.xLightInfo.xLightValue.MaxValue = timeMOD;
	xBcmInfo.xLightInfo.xLightValue.MixValue = 0;
	xBcmInfo.xLightInfo.xLightValue.LevelValue = 0;
	xApuwDeviceStatus.ReverseFlag = FALSE;


	xBcmInfo.txApuTrunkState = 0;
	xBcmInfo.txApuSurState = 0;
}

/* \brief
*/
void vBcmTimer(void)
{
	if(0 != wBcmCycleTime)
	{
		--wBcmCycleTime;
	}
	if(0 != wReverseDelayTime)
	{
		--wReverseDelayTime;
	}
	if(0 != wReverseRxDelayTime)
	{
		--wReverseRxDelayTime;
	}
}

void vBcmTimerMS(void)
{
	if(0 != wBcmLightCycleTime)
	{
		--wBcmLightCycleTime;
	}
}

/* \brief
*/
void vBcmTask(void)
{
	vReverseTask();
	vHomeLightTask();
	switch(xBcmInfo.eBcmState)
	{
		default:
			break;
		case BCM_STATE_WAIT:
			if(!isSystemInitCompleted)
			{
				break;
			}
			wBcmCycleTime = 120;
			xBcmInfo.eBcmState = BCM_STATE_WORK;
			break;

		case BCM_STATE_WORK:
			break;
		case BCM_STATE_TX_CAN:
			if(0 == wBcmCycleTime)
			{
				(void)blCanAppWrite(&xBcmInfo.xCanTxMsg);
				wBcmCycleTime = 10;
				++wBcmCycleCount;
			}
			if(wBcmCycleCount > 2)
			{
				wBcmCycleCount = 0;
				xBcmInfo.eBcmState = BCM_STATE_TX_NONE;
			}
			break;

		case BCM_STATE_TX_NONE:
			if(0 == wBcmCycleTime)
			{
				xBcmInfo.xCanTxMsg.Data[0] = 0x00;
				(void)blCanAppWrite(&xBcmInfo.xCanTxMsg);
				wBcmCycleTime = 10;
				++wBcmCycleCount;
			}
			if(wBcmCycleCount > 2)
			{
				wBcmCycleCount = 0;
				xBcmInfo.eBcmState = BCM_STATE_WORK;
			}
			break;
			
		case BCM_STATE_OFF:
			break;
	}
}

/* \brief
*/
static void vReverseTask(void)
{
	switch(xBcmInfo.eRevState)
	{
		default:
			break;
		case REV_STATE_WAIT:
			xBcmInfo.eRevState = REV_STATE_WORK;
			break;
		case REV_STATE_HOLD:
			if(0 != wReverseDelayTime)
			{
				break;
			}
			xBcmInfo.eRevState = REV_STATE_WORK;
			break;
		case REV_STATE_WORK:
			break;
		case REV_STATE_ON:
			if(0 != wReverseDelayTime)
			{
				break;
			}
			if(xApuwDeviceStatus.ReverseFlag)
			{
				Tw8836_SwitchChannel(VCH_REVERSE);
			}
			else
			{
				Tw8836_SwitchChannel(VCH_LVDS);
			}
			xBcmInfo.eRevState = REV_STATE_WORK;
			break;
		case REV_STATE_OFF:
			break;
		}
}

/* \brief
*/
static void vHomeLightTask(void)
{
	switch(xBcmInfo.xLightInfo.eLightState)
	{
		default:
			break;
		case LIGHT_STATE_WAIT:
			xBcmInfo.xLightInfo.eLightState = LIGHT_STATE_WORK;
			break;
		case LIGHT_STATE_INIT:
			break;
		case LIGHT_STATE_WORK:
			break;
		case LIGHT_STATE_UP:
			if(0 != wBcmLightCycleTime)
			{
				break;
			}
			
			vPwmLightControl(TRUE, xBcmInfo.xLightInfo.xLightValue.CurValue);

			wBcmLightCycleTime = 20;
//			wBcmLightCycleTime = 10;
			if(xBcmInfo.xLightInfo.xLightValue.CurValue >= xBcmInfo.xLightInfo.xLightValue.MaxValue)
			{
				xBcmInfo.xLightInfo.eLightState = LIGHT_STATE_WORK;
//				xBcmInfo.xLightInfo.eLightState = LIGHT_STATE_DOWN;
			}
			else if(xBcmInfo.xLightInfo.xLightValue.CurValue > 700)
			{
				xBcmInfo.xLightInfo.xLightValue.CurValue += 3;
			}
			else if(xBcmInfo.xLightInfo.xLightValue.CurValue > 250)
			{
				xBcmInfo.xLightInfo.xLightValue.CurValue += 2;
			}
			else
			{
				++xBcmInfo.xLightInfo.xLightValue.CurValue;
			}
			break;
		case LIGHT_STATE_DOWN:
			if(0 != wBcmLightCycleTime)
			{
				break;
			}
			
			vPwmLightControl(TRUE, xBcmInfo.xLightInfo.xLightValue.CurValue);

			wBcmLightCycleTime = 20;
			if(xBcmInfo.xLightInfo.xLightValue.CurValue <= xBcmInfo.xLightInfo.xLightValue.MixValue)
			{
				if(!xApuwDeviceStatus.IllumiOn)
				{
					ioLIGHT_KEY_POWER_OFF_V4;
					xBcmInfo.xLightInfo.eLightState = LIGHT_STATE_WORK;
				}
// 闪烁
/*
				else
				{
					wBcmLightCycleTime = 10;
					xBcmInfo.xLightInfo.eLightState = LIGHT_STATE_UP;
				}
*/				
			}
			else if(xBcmInfo.xLightInfo.xLightValue.CurValue > 700)
			{
				xBcmInfo.xLightInfo.xLightValue.CurValue -= 3;
			}
			else if(xBcmInfo.xLightInfo.xLightValue.CurValue > 250)
			{
				xBcmInfo.xLightInfo.xLightValue.CurValue -= 2;
			}
			else
			{
				--xBcmInfo.xLightInfo.xLightValue.CurValue;
			}
			
			break;
		case LIGHT_STATE_OFF:
			break;
	}
}

/* \brief
*/
void vCanBcm4A0ReadHandle(BYTE* pxCanMessage)
{
	if((SYSTEM_ACC_IDLE != vSystemAccStatus())  || (!ioBAT_DET_IN))
	{//开机，关机状态，
		return;
	}
	if(rearRefrost != ((pxCanMessage[1] >> 7) & 0x01))
	{
		rearRefrost = (pxCanMessage[1] >> 7) & 0x01;
		xAirConditionInfo.uDefrostAutoStatus.Bits.F_RearDefrost = rearRefrost;
		vApuWrite(APUW_GID_CAN_INFO, APUW_CAN_AC, (BYTE*)&xAirConditionInfo, sizeof(xAirConditionInfo));
	}
	
	if(xBcmInfo.xLightInfo.parkLightOn != pxCanMessage[4] & 0x03)
	{
		xBcmInfo.xLightInfo.parkLightOn = pxCanMessage[4] & 0x03;
		if(0 == xBcmInfo.xLightInfo.parkLightOn)
		{
			xApuwDeviceStatus.IllumiOn = FALSE;
			vApuWrite(APUW_GID_SYS_INFO, APUW_SYSINFO_DEVICE_STATUS, (BYTE*)&xApuwDeviceStatus, sizeof(xApuwDeviceStatus));
			xBcmInfo.xLightInfo.xLightValue.MixValue = 0;
			xBcmInfo.xLightInfo.eLightState = LIGHT_STATE_DOWN;
		}
		else
		{
			xApuwDeviceStatus.IllumiOn = TRUE;
			vApuWrite(APUW_GID_SYS_INFO, APUW_SYSINFO_DEVICE_STATUS, (BYTE*)&xApuwDeviceStatus, sizeof(xApuwDeviceStatus));
			ioLIGHT_KEY_POWER_ON_V4;
			xBcmInfo.xLightInfo.eLightState = LIGHT_STATE_UP;
		}
	}
	
}


/* \brief
*/
void vBcmHomeLightCtrl(BOOL isLightUp)
{
	if(xApuwDeviceStatus.IllumiOn)
	{
		if(isLightUp)
		{
			xBcmInfo.xLightInfo.eLightState = LIGHT_STATE_UP;
		}
		else
		{
			xBcmInfo.xLightInfo.xLightValue.MixValue = 0;
			xBcmInfo.xLightInfo.eLightState = LIGHT_STATE_DOWN;
		}
	}
}


/* \brief
*/
void vCanBcm5B0ReadHandle(BYTE* pxCanMessage)
{
	if((SYSTEM_ACC_IDLE != vSystemAccStatus())  || (!ioBAT_DET_IN))
	{//开机，关机状态，
		return;
	}
	if(trunkSwitch != (pxCanMessage[0]  & 0x01))
	{
		trunkSwitch = pxCanMessage[0]  & 0x01;
		if(trunkSwitch)
		{
			xBcmInfo.txApuTrunkState = 0x01;
		}
		else
		{
			xBcmInfo.txApuTrunkState = 0x00;
		}
		vApuWrite(APUW_GID_CAN_INFO, APUW_CAN_TRUNK, &xBcmInfo.txApuTrunkState, 1);
	}
	
	if(trunkMovement != ((pxCanMessage[0] >> 7) & 0x01))
	{
		trunkMovement = (pxCanMessage[0] >> 7) & 0x01;
		if(trunkMovement)
		{
			xBcmInfo.txApuTrunkState = 0x02;
		}
		else
		{
			xBcmInfo.txApuTrunkState = 0x03;
		}
		vApuWrite(APUW_GID_CAN_INFO, APUW_CAN_TRUNK, &xBcmInfo.txApuTrunkState, 1);
	}
	
}


/* \brief
*/
void vCanBcm620ReadHandle(BYTE* pxCanMessage)
{
	if((SYSTEM_ACC_IDLE != vSystemAccStatus())  || (!ioBAT_DET_IN))
	{//开机，关机状态，
		return;
	}
	if(((pxCanMessage[0] >> 4) & 0x03) != 0)
	{
		return;
	}
	
	if(xBcmInfo.xLightInfo.xLightValue.LevelValue!= pxCanMessage[1])
	{
		xBcmInfo.xLightInfo.xLightValue.LevelValue = pxCanMessage[1];
		if(!xApuwDeviceStatus.IllumiOn)
		{
			return;
		}
		if((0x10 < xBcmInfo.xLightInfo.xLightValue.LevelValue) && (0x7F > xBcmInfo.xLightInfo.xLightValue.LevelValue))
		{
			xBcmInfo.xLightInfo.xLightValue.LevelValue *= 2;
			if(xBcmInfo.xLightInfo.xLightValue.LevelValue > 0x4E)
			{
				xBcmInfo.xLightInfo.xLightValue.LevelValue *= 2;
			}

//			xBcmInfo.xLightInfo.xLightValue.MaxValue = xBcmInfo.xLightInfo.xLightValue.LevelValue;
			
			if(xBcmInfo.xLightInfo.xLightValue.CurValue <= xBcmInfo.xLightInfo.xLightValue.LevelValue)
			{
				xBcmInfo.xLightInfo.xLightValue.MaxValue = xBcmInfo.xLightInfo.xLightValue.LevelValue;
				xBcmInfo.xLightInfo.eLightState = LIGHT_STATE_UP;
			}
			else
			{
				xBcmInfo.xLightInfo.xLightValue.MaxValue = xBcmInfo.xLightInfo.xLightValue.MixValue = xBcmInfo.xLightInfo.xLightValue.LevelValue;
				xBcmInfo.xLightInfo.eLightState = LIGHT_STATE_DOWN;
			}
		}
		
	}
	
}


/* \brief
*/
void vCanBcm660ReadHandle(BYTE* pxCanMessage)
{
	enumBcmSurMov tempMov = (pxCanMessage[6] >> 2) & 0x03;
	BOOL reverseValueDisable = pxCanMessage[7] & 0x01;
	BOOL reverseValue = ((pxCanMessage[7]) >> 1) & 0x01;
	enumBcmSurPos tempPos = pxCanMessage[7] >> 5;
	if((SYSTEM_ACC_IDLE != vSystemAccStatus())  || (!ioBAT_DET_IN))
	{//开机，关机状态，
		return;
	}
	if(tempMov != xBcmInfo.eBcmSurMov)
	{
		xBcmInfo.eBcmSurMov = tempMov;
		switch(xBcmInfo.eBcmSurMov)
		{
			default:
				break;
			case BCM_SR_MOV_START:
				xBcmInfo.txApuSurState = 0x02;
				break;
			case BCM_SR_MOV_STOP:
				xBcmInfo.txApuSurState = 0x03;
				break;
		}
		vApuWrite(APUW_GID_CAN_INFO, APUW_CAN_SUNROOF, (BYTE*)&xBcmInfo.txApuSurState, 1);
	}
	if(tempPos != xBcmInfo.eBcmSurPos)
	{
		xBcmInfo.eBcmSurPos = tempPos;
		switch(xBcmInfo.eBcmSurPos)
		{
			default:
				break;
			case BCM_SR_POSITION_CLOSE:
				xBcmInfo.txApuSurState = 0x00;
				break;
			case BCM_SR_POSITION_OPEN:
				xBcmInfo.txApuSurState = 0x01;
				break;
		}
		vApuWrite(APUW_GID_CAN_INFO, APUW_CAN_SUNROOF, (BYTE*)&xBcmInfo.txApuSurState, 1);
	}

	
	if(isAvmExistance ||  (0 != wReverseRxDelayTime) || (!getDispInitComplete()))
	{
		return;
	}
	if(!reverseValueDisable)
	{
		if(xApuwDeviceStatus.ReverseFlag != reverseValue)
		{
			xApuwDeviceStatus.ReverseFlag = reverseValue;
			vPwmMonitorControl(FALSE, xApuSetting.xApuScreen.ScreenBrightness);
			if(xApuwDeviceStatus.ReverseFlag)
			{
				ioAPU_REV_ON;
				ioODO_FWD_BK;
			}
			else
			{
				ioAPU_REV_OFF;
				ioODO_FWD_FW;
			}
			wReverseDelayTime = 0;
			wReverseRxDelayTime = 80;
			xBcmInfo.eRevState = REV_STATE_ON;
		}
	}
	
}

/* \brief
*/
void vReverseButton(void)
{
	vPwmMonitorControl(FALSE, xApuSetting.xApuScreen.ScreenBrightness);
	if(!xApuwDeviceStatus.ReverseFlag)
	{
		ioAPU_REV_ON;
		ioODO_FWD_BK;
		xApuwDeviceStatus.ReverseFlag = TRUE;
	}
	else
	{
		ioAPU_REV_OFF;
		ioODO_FWD_FW;
		xApuwDeviceStatus.ReverseFlag = FALSE;
	}
	wReverseDelayTime = 0;
	xBcmInfo.eRevState = REV_STATE_ON;
}

/* \brief
*/
void vBcmApuSurSet(BYTE* pData)
{
	switch(pData[0])
	{
		default:
			break;
		case 1:
			switch(pData[1])
			{

				default:
				case 0:
					break;
				case 1:
				case 2:
				case 3:
					xBcmInfo.xCanTxMsg.Data[0] = (pData[1] & 0x03) << 5;
					xBcmInfo.eBcmState = BCM_STATE_TX_CAN;
					break;
			}
			break;
	}
	
}

/* \brief
*/
void vBcmApuTrunkSet(BYTE* pData)
{
	switch(pData[0])
	{
		default:
			break;
		case 1:
			switch(pData[1])
			{

				default:
				case 0:
					break;
				case 1:
				case 2:
				case 3:
					xBcmInfo.xCanTxMsg.Data[0] = (pData[1] & 0x03) << 1;
					xBcmInfo.eBcmState = BCM_STATE_TX_CAN;
					break;
			}
			break;
	}
	
}

/* \brief
*/
void vBcmTxApuSurState(void)
{
	vApuWrite(APUW_GID_CAN_INFO, APUW_CAN_SUNROOF, &xBcmInfo.txApuSurState, 1);
}

/* \brief
*/
void vBcmTxApuTrunkState(void)
{
	vApuWrite(APUW_GID_CAN_INFO, APUW_CAN_TRUNK, &xBcmInfo.txApuTrunkState, 1);
}


