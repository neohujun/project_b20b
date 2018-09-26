/******************************************************************************
**
**								Copyright (C) 2017  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		Avm.c
**	Abstract:			avm cmd
**	Creat By:		neo HU
**	Creat Time:		2017.7.1
**	Version:		v1.0
**
******************************************************************************/

/* \brief
	include
*/
#include "Avm.h"
#include "List.h"
#include "CanApp.h"
#include "Tw88xx_Drv.h"
#include "io.h"
#include <MC9S08DZ60.h>
#include "Apu.h"
#include "system.h"
#include "io.h"

/* \brief
*/

typedef enum
{
	AVM_STATE_WAIT,
	AVM_STATE_INIT,
	AVM_STATE_HOLD,
	AVM_STATE_WORK,
	AVM_STATE_CH_SWITCH,
	AVM_STATE_OFF
}enumAvmState;


/* \brief
*/
typedef enum
{
	AVM_NO_REQUEST,		//quit AVM
	AVM_VIEW_NORMAL,		//
	AVM_VIEW_ERROR		//error OSD
}enumAvmDisplayReq;


/* \brief
*/
CanFrame xAvmMessage;
static enumAvmState eAvmState;
static enumAvmDisplayReq eAvmDisplayReq;

/* \brief
*/
static BYTE wAvmRxCycleTime;
static BYTE wAvmCycleTime;
BOOL isAvmExistance = FALSE;
BOOL isAvmOn = FALSE;


/* \brief
*/

/* \brief
*/
void vAvmInit(void)
{
	xAvmMessage.ID[0] = canID_TX_AVM_67A_H;
	xAvmMessage.ID[1] = canID_TX_AVM_67A_L;
	
	xAvmMessage.Length = 8;

	xAvmMessage.Data[0] = 0x00;
	xAvmMessage.Data[1] = 0x00;
	xAvmMessage.Data[2] = 0x00;
	xAvmMessage.Data[3] = 0x00;
	xAvmMessage.Data[4] = 0x00;
	xAvmMessage.Data[5] = 0x00;
	xAvmMessage.Data[6] = 0x00;
	xAvmMessage.Data[7] = 0x00;
	

	wAvmCycleTime = 0;
	wAvmRxCycleTime = 0;
	eAvmState = AVM_STATE_WAIT;
	eAvmDisplayReq = AVM_NO_REQUEST;
	isAvmOn = FALSE;
}

/* \brief
*/
void vAvmTimer(void)
{
	if(0 != wAvmCycleTime)
	{
		--wAvmCycleTime;
	}
	if(0 != wAvmRxCycleTime)
	{
		--wAvmRxCycleTime;
	}
}

/* \brief
*/
void vAvmTask(void)
{
	switch(eAvmState)
	{
		default:
			break;
		case AVM_STATE_WAIT:
			eAvmState = AVM_STATE_WORK;
			break;
		case AVM_STATE_INIT:
			break;
		case AVM_STATE_WORK:
			break;
		case AVM_STATE_CH_SWITCH:
			if(0 != wAvmCycleTime)
			{
				break;
			}
			if(xApuwDeviceStatus.ReverseFlag)
			{
				Tw8836_SwitchChannel(VCH_360);
				isAvmOn = TRUE;
			}
			else
			{
				Tw8836_SwitchChannel(VCH_LVDS);
				isAvmOn = FALSE;
			}
			eAvmState = AVM_STATE_WORK;
			break;
		case AVM_STATE_HOLD:
			if(0 != wAvmCycleTime)
			{
				break;
			}
			eAvmState = AVM_STATE_WORK;
			break;
		case AVM_STATE_OFF:
			break;
	}
}

/* \brief
*/
void vCanAvmReadHandle(BYTE* pxCanMessage)
{
	BYTE i = 0;
	BYTE count = 0;
	enumAvmDisplayReq tempReq = ((pxCanMessage[1] >> 4) & 0x03);
	
	if((0 != wAvmRxCycleTime) || (!getDispInitComplete()) || (SYSTEM_ACC_IDLE != vSystemAccStatus())  || (!ioBAT_DET_IN))
	{//开机，关机状态，
		return;
	}
	for(i = 0; i < 8; i++)
	{
		if(pxCanMessage[i] == 0)
		{
			if(++count == 8)
			{
				return;
			}
		}
	}
	
	isAvmExistance = TRUE;

	if(eAvmDisplayReq != tempReq)
	{
		vPwmMonitorControl(FALSE, xApuSetting.xApuScreen.ScreenBrightness);
		eAvmDisplayReq = tempReq;
		switch(eAvmDisplayReq)
		{
			default:
				ioAPU_REV_OFF;
				ioODO_FWD_FW;
				xApuwDeviceStatus.ReverseFlag = FALSE;
				break;
			case AVM_VIEW_NORMAL:
				ioAPU_REV_ON;
				ioODO_FWD_BK;
				xApuwDeviceStatus.ReverseFlag = TRUE;
				break;
			case AVM_VIEW_ERROR:
				break;
		}
		wAvmCycleTime = 0;
		wAvmRxCycleTime = 80;
		eAvmState = AVM_STATE_CH_SWITCH;
	}

}


/* \brief
*/
void vAvmApuSet(BYTE* pData)
{
	switch(pData[0])
	{
		default:
		case AVM_APU_REQ_NONE:
		case AVM_APU_REQ_DISPLAY:
			break;
		case AVM_APU_REQ_SET:
			xAvmMessage.ID[0] = canID_TX_AVM_671_H;
			xAvmMessage.ID[1] = canID_TX_AVM_671_L;
			switch(pData[1])
			{
				default:
				case AVM_APU_SET_NONE:
					break;
				case AVM_APU_SET_LDW:
					switch(pData[2])
					{
						default:
						case AVM_APU_SET_CMD_NONE:
							break;
						case AVM_APU_SET_CMD_ENABLE:
							xAvmMessage.Data[0] = (xAvmMessage.Data[0] & 0x0C) | pData[3];
							break;
						case AVM_APU_SET_CMD_WARN_VOICE:
							break;
						case AVM_APU_SET_CMD_WARN_LIGHT:
							break;
						case AVM_APU_SET_CMD_WARN_SWC:
							break;
					}
					break;
				case AVM_APU_SET_BSD:
					switch(pData[2])
					{
						default:
						case AVM_APU_SET_CMD_NONE:
							break;
						case AVM_APU_SET_CMD_ENABLE:
							xAvmMessage.Data[0] = (xAvmMessage.Data[0] & 0x03) | (pData[3] << 2);
							break;
						case AVM_APU_SET_CMD_WARN_VOICE:
							break;
						case AVM_APU_SET_CMD_WARN_LIGHT:
							break;
						case AVM_APU_SET_CMD_WARN_SWC:
							break;
					}
					break;
			}
			break;
		case AVM_APU_REQ_TOUCH:
			
			xAvmMessage.ID[0] = canID_TX_AVM_67A_H;
			xAvmMessage.ID[1] = canID_TX_AVM_67A_L;
			
			xAvmMessage.Data[0] = 0x10;

			xAvmMessage.Data[1] = pData[1] << 5;
			xAvmMessage.Data[1] |= pData[2] >> 3 ;
			xAvmMessage.Data[2] = pData[2] << 5;
			xAvmMessage.Data[3] = pData[3] << 5;
			xAvmMessage.Data[3] |= pData[4] >> 3 ;
			xAvmMessage.Data[4] = pData[4] << 5;
			(void)blCanAppWrite(&xAvmMessage);
			break;
	}
	
}

