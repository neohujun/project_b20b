/******************************************************************************
**
**								Copyright (C) 2017  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		Ac.c
**	Abstract:			air condition_CAN_672
**	Creat By:		neo HU
**	Creat Time:		2017.7.1
**	Version:		v1.0
**
******************************************************************************/

/* \brief
	include
*/
#include "Ac.h"
#include "List.h"
#include "CanApp.h"
#include "io.h"
#include "Apu.h"
#include "system.h"
#include "io.h"
#include <MC9S08DZ60.h>


/* \brief
*/
typedef enum
{
	AC_STATE_WAIT,
	AC_STATE_INIT,
	AC_STATE_WORK,
	AC_STATE_TX_CAN,
	AC_STATE_TX_NONE,
	AC_STATE_TX_APU,
	AC_STATE_OFF
}enumAcState;

/* \brief
*/
typedef enum
{
	AC_VALID_OP_VALUE_OK,
	AC_VALID_OP_VALUE_BELOW,
	AC_VALID_OP_VALUE_ABOVE,
	AC_VALID_OP_VALUE_UNRELIABLE
}enumAcValidOp;

/* \brief
*/
typedef enum
{
	AC_AUTO_MODE_OFF,
	AC_AUTO_MODE_ON
}enumAcAutoModeSts;

/* \brief
*/
typedef enum
{
	AC_CLM_MODE_OFF,
	AC_CLM_MODE_VENT,
	AC_CLM_MODE_BL,
	AC_CLM_MODE_FOOT,
	AC_CLM_MODE_MIX,
	AC_CLM_MODE_DEFROST
}enumAcClmModeSts;

/* \brief
*/
typedef enum
{
	AC_FRONT_DEFROST_OFF,
	AC_FRONT_DEFROST_ON
}enumAcFrontDefrost;

/* \brief
*/
typedef enum
{
	AC_INTAKE_WIND_OFF,
	AC_INTAKE_WIND_FRESH,
	AC_INTAKE_WIND_RECYCLE
}enumAcIntakeWindSts;

/* \brief
*/
typedef enum
{
	AC_ON_REQ_OFF,
	AC_ON_REQ_ON
}enumAcOnReq;

/* \brief
*/
typedef enum
{
	AC_OFF_MODE_OFF,
	AC_OFF_MODE_ON
}enumAcOffModeSts;

/* \brief
*/
typedef enum
{
	AC_TEMP_SET_ERR_OFF,
	AC_TEMP_SET_ERR_ON
}enumAcTempSetValueErrorAc;

/* \brief
*/
typedef enum
{
	AC_AQS_OFF,
	AC_AQS_ON
}enumAcAqs;

/* \brief
*/
typedef enum
{
	AC_ANION_WORK_OFF,
	AC_ANION_WORK_ON_CLEAN,
	AC_ANION_WORK_ON_ION,
	AC_ANION_WORK_FAULT
}enumAcAnionWorkStatus;


/* \brief
*/
typedef enum
{
	AC_DIS_INACTIVE,
	AC_DIS_ACTIVE
}enumAcDisplayActive;

/* \brief
*/
typedef enum
{
	AC_REQ_LIGHT_OFF,
	AC_REQ_LIGHT_ON
}enumAcReqLight;

/* \brief
*/
typedef enum
{
	AC_CFG_MANUAL,
	AC_CFG_ELECTRICAL,
	AC_CFG_AUTO,
	AC_CFG_AUTO_WITH
}enumAcConfigure;


/* \brief
	variable
*/
CanFrame xAcMessage;
static enumAcState eAcState;

/* \brief
	variable
*/
static BYTE wAcCycleTime;
static BYTE wAcCycleCount;
static BYTE wAcInitDelayTime;
AirConditionInfo xAirConditionInfo;
BYTE rxDataCheck[8] = {0x00};


/* \brief
*/
void vAcInit(void)
{
	xAcMessage.ID[0] = canID_TX_AC_672_H;
	xAcMessage.ID[1] = canID_TX_AC_672_L;
	
	xAcMessage.Length = 8;

	xAcMessage.Data[0] = 0x00;
	xAcMessage.Data[1] = 0x00;
	xAcMessage.Data[2] = 0x00;
	xAcMessage.Data[3] = 0x00;
	xAcMessage.Data[4] = 0x00;
	xAcMessage.Data[5] = 0x00;
	xAcMessage.Data[6] = 0x00;
	xAcMessage.Data[7] = 0x00;
	
	wAcCycleTime = 0;
	wAcCycleCount = 0;
	wAcInitDelayTime = 0;
	eAcState = AC_STATE_WAIT;
}

/* \brief
*/
void vAcTimer(void)
{
	if(0 != wAcCycleTime)
	{
		--wAcCycleTime;
	}
	if(0 != wAcInitDelayTime)
	{
		--wAcInitDelayTime;
	}
}

/* \brief
*/
void vAcTask(void)
{
	switch(eAcState)
	{
		default:
			break;
		case AC_STATE_WAIT:
			if(!isSystemInitCompleted)
			{
				break;
			}
			wAcInitDelayTime = 100;
			eAcState = AC_STATE_INIT;
			break;
		case AC_STATE_INIT:
			if(0 != wAcInitDelayTime)
			{
				break;
			}
			else
			{
//				vApuWrite(APUW_GID_CAN_INFO, APUW_CAN_AC, (BYTE*)&xAirConditionInfo, sizeof(xAirConditionInfo));
				eAcState = AC_STATE_WORK;
			}
			
			break;
		case AC_STATE_WORK:
			break;
		case AC_STATE_TX_CAN:
			if(0 == wAcCycleTime)
			{
				(void)blCanAppWrite(&xAcMessage);
				wAcCycleTime = 10;
				++wAcCycleCount;
			}
			if(wAcCycleCount > 2)
			{
				wAcCycleCount = 0;
				wAcCycleTime = 10;
				eAcState = AC_STATE_TX_NONE;
			}
			break;

		case AC_STATE_TX_NONE:
			if(0 == wAcCycleTime)
			{
				xAcMessage.Data[0] = 0x00;
				xAcMessage.Data[1] = 0x00;
				xAcMessage.Data[2] = 0x00;
				xAcMessage.Data[3] = 0x00;
				xAcMessage.Data[4] = 0x00;
				xAcMessage.Data[5] = 0x00;
				xAcMessage.Data[6] = 0x00;
				xAcMessage.Data[7] = 0x00;
				(void)blCanAppWrite(&xAcMessage);
				wAcCycleTime = 10;
				++wAcCycleCount;
			}
			if(wAcCycleCount > 2)
			{
				wAcCycleCount = 0;
				eAcState = AC_STATE_WORK;
			}
			break;

		case AC_STATE_TX_APU:
			break;
			
		case AC_STATE_OFF:
			break;
	}
}

/* \brief
*/
void vAcApuSet(BYTE* pData)
{
	BYTE i = 0;
	xAcMessage.Data[0] = 0x00;
	xAcMessage.Data[1] = 0x00;
	xAcMessage.Data[2] = 0x00;
	xAcMessage.Data[3] = 0x00;
	xAcMessage.Data[4] = 0x00;
	xAcMessage.Data[5] = 0x00;
	xAcMessage.Data[6] = 0x00;
	
	switch(pData[0])
	{
		default:
			break;
			
		case AC_CMD_SWITCH:					//req off
			switch(pData[1])
			{
				default:
					break;
				case 0:					//close
					xAcMessage.Data[0] = 1 << 4;
					break;
				case 1:					//open
					xAcMessage.Data[0] = 2 << 4;
					break;
			}
			break;
			
		case AC_CMD_AC_SWITCH:
			switch(pData[1])
			{
				default:
					break;
				case 0:					//AC off
					xAcMessage.Data[0] = 1 << 0;
					break;
				case 1:					//on
					xAcMessage.Data[0] = 2 << 0;
					break;
			}
			break;
			
		case AC_CMD_AC_CYCLE_CTL:
			switch(pData[1])
			{
				default:
					break;
				case 0:					//inside
					xAcMessage.Data[0] = 2 << 6;
					break;
				case 1:					//out
					xAcMessage.Data[0] = 1 << 6;
					break;
				case 2:
					break;
			}
			break;
			
		case AC_CMD_DUAL_SWITCH:
			switch(pData[1])
			{
				default:
					break;
				case 0:					//off
					xAcMessage.Data[3] = 2 << 0;
					break;
				case 1:					//on
					xAcMessage.Data[3] = 1 << 0;
					break;
			}
			break;
			
		case AC_CMD_FRONT_DEFROST:
			break;
			
		case AC_CMD_REAR_DEFROST:
			switch(pData[1])
			{
				default:
					break;
				case 0:					//off
					xAcMessage.Data[0] = 1 << 2;
					break;
				case 1:					//on
					xAcMessage.Data[0] = 2 << 2;
					break;
			}
			break;
			
		case AC_CMD_AUTO_SWITCH:
			switch(pData[1])
			{
				default:
					break;
				case 0:					//off
					xAcMessage.Data[2] = 1 << 0;
					break;
				case 1:					//on
				case 2:
					xAcMessage.Data[2] = 2 << 0;
					break;
			}
			break;
			
		case AC_CMD_FL_TEMP_ADJUST:	//adjust value
			xAcMessage.Data[4] = pData[1] << 1;
			break;
		case AC_CMD_FL_TEMP_DIRECT:	
			switch(pData[1])
			{
				default:
					break;
				case 0:					//Down
					xAcMessage.Data[2] = 2 << 2;
					break;
				case 1:					//up
					xAcMessage.Data[2] = 1 << 2;
					break;
			}
			break;
			
		case AC_CMD_FR_TEMP_ADJUST:	//adjust value
			
			break;
		case AC_CMD_FR_TEMP_DIRECT:
			switch(pData[1])
			{
				default:
					break;
				case 0:					//temp down
					break;
				case 1:					//temp up 1 degree
					break;
			}
			break;
		case AC_CMD_REAR_TEMP_ADJUST:	//adjust value
			
			break;
		case AC_CMD_SET_FRONT_WIND_MODE:
			switch(pData[1])
			{
				default:
					break;
				case 2:					//face
					xAcMessage.Data[2] = 1 << 4;
					break;
				case 4:					//foot
					xAcMessage.Data[2] = 3 << 4;
					break;
				case 6:					//B/L
					xAcMessage.Data[2] = 2 << 4;
					break;
				case 0x0C:		//foot + defrost
					xAcMessage.Data[2] = 4 << 4;
					break;
				case 8:					//defrost
					xAcMessage.Data[2] = 5 << 4;
					break;
			}
			break;
			
		case AC_CMD_SET_REAR_WIND_MODE:
			
			break;
		case AC_CMD_F_WIND_LEV_ADJUST:	//00~0E: 0~14 lev
			if(0 == pData[1])
			{
				pData[1] = 1;
			}
			xAcMessage.Data[3] = pData[1] << 2;
			break;
		case AC_CMD_F_WIND_LEV_DIRECT:
			switch(pData[1])
			{
				default:
					break;
				case 0:					//wind down
					xAcMessage.Data[1] = 2 << 2;
					break;
				case 1:					//wind up 1 lev
					xAcMessage.Data[1] = 1 << 2;
					break;
			}
			break;
		case AC_CMD_R_WIND_LEV_ADJUST:
			
			break;
		case AC_CMD_R_WIND_LEV_DIRECT:
			switch(pData[1])
			{
				default:
					break;
				case 0:					//wind down
					break;
				case 1:					//wind up 1 lev
					break;
			}
			break;
		case AC_CMD_LEFT_SEAT_HEAT:	//0:off, 0~14 lev
			
			break;
		case AC_CMD_RIGHT_SEAT_HEAT:	//0:off, 0~14 lev
			
			break;
		case AC_CMD_AC_ECO_MODE_SWITCH:
			switch(pData[1])
			{
				default:
					break;
				case 0:					//close
					break;
				case 1:					//open
					break;
			}
			break;
		case AC_CMD_SENS_SET:
			switch(pData[1])
			{
				default:
					break;
				case 0:					//close
					break;
				case 1:					//low sensitivity
					break;
				case 2:					//high sensitivity
					break;
			}
			break;
		case AC_CMD_AIR_QUALITY_SWITCH:
			switch(pData[1])
			{
				default:
					break;
				case 0:					//off
					break;
				case 1:					//on
					break;
			}
			break;
		case AC_CMD_FILTER_POLLEN_SWITCH:
			switch(pData[1])
			{
				default:
					break;
				case 0:					//off
					break;
				case 1:					//on
					break;
			}
			break;
		case AC_CMD_R_WIND_SHIELD_HEAT:
			switch(pData[1])
			{
				default:
					break;
				case 0:					//close
					break;
				case 1:					//open
					break;
			}
			break;
		case AC_CMD_FORCE_DEFROST:
			switch(pData[1])
			{
				default:
					break;
				case 0:					//off
					xAcMessage.Data[5] = 1 << 1;
					break;
				case 1:					//on
					xAcMessage.Data[5] = 2 << 1;
					break;
			}
			break;
		case AC_CMD_LOW:
			switch(pData[1])
			{
				default:
					break;
				case 0:					//off
					xAcMessage.Data[1] = 1 << 4;
					break;
				case 1:					//on
					xAcMessage.Data[1] = 2 << 4;
					break;
			}
			break;
		case AC_CMD_HOT:
			switch(pData[1])
			{
				default:
					break;
				case 0:					//off
					xAcMessage.Data[1] = 1 << 6;
					break;
				case 1:					//on
					xAcMessage.Data[1] = 2 << 6;
					break;
			}
			break;
			
	}
	for(i = 0; i < 7; i++)
	{
		xAcMessage.Data[7] += xAcMessage.Data[i];
	}
	eAcState = AC_STATE_TX_CAN;
}

void vCanAcReadHandle(BYTE* pxCanMessage)
{
	BYTE count = 0;
	BYTE i = 0;
	if((SYSTEM_ACC_IDLE != vSystemAccStatus())  || (!ioBAT_DET_IN))
	{//开机，关机状态，
		return;
	}

	for(i = 0; i < 8; i++)
	{
		rxDataCheck[i] != pxCanMessage[i] ? (rxDataCheck[i] = pxCanMessage[i]) : ++count;
		if(8 == count)
		{
			return;
		}
	}
	switch(pxCanMessage[0] & 0x03)
	{
		default:
			break;
		case AC_VALID_OP_VALUE_OK:
			break;
		case AC_VALID_OP_VALUE_BELOW:
			break;
		case AC_VALID_OP_VALUE_ABOVE:
			break;
		case AC_VALID_OP_VALUE_UNRELIABLE:
			break;
	}
	switch((pxCanMessage[0] >> 2) & 0x01)
	{
		default:
			break;
		case AC_AUTO_MODE_OFF:
			xAirConditionInfo.uDefrostAutoStatus.Bits.F_AutoFlag = 0;
			break;
		case AC_AUTO_MODE_ON:
			xAirConditionInfo.uDefrostAutoStatus.Bits.F_AutoFlag = 1;
			break;
	}
	switch((pxCanMessage[0] >> 3) & 0x07)
	{
		default:
			break;
		case AC_CLM_MODE_OFF:
			break;
		case AC_CLM_MODE_VENT:
			xAirConditionInfo.WindMode = 0x02;
			break;
		case AC_CLM_MODE_BL:
			xAirConditionInfo.WindMode = 0x06;
			break;
		case AC_CLM_MODE_FOOT:
			xAirConditionInfo.WindMode = 0x04;
			break;
		case AC_CLM_MODE_MIX:
			xAirConditionInfo.WindMode = 0x0C;
			break;
		case AC_CLM_MODE_DEFROST:
			xAirConditionInfo.WindMode = 0x08;
			break;
	}
	switch((pxCanMessage[0] >> 6) & 0x01)
	{
		default:
			break;
		case AC_FRONT_DEFROST_OFF:
			xAirConditionInfo.uACExternInfo3.Bits.ForceDefrost = 0;
			break;
		case AC_FRONT_DEFROST_ON:
			xAirConditionInfo.uACExternInfo3.Bits.ForceDefrost = 1;
			break;
	}
	
	switch(pxCanMessage[1] & 0x03)
	{
		default:
			break;
		case AC_INTAKE_WIND_OFF:
			break;
		case AC_INTAKE_WIND_FRESH:
			xAirConditionInfo.uAirConditionStatus.Bits.F_CycleStatus = 1;
			break;
		case AC_INTAKE_WIND_RECYCLE:
			xAirConditionInfo.uAirConditionStatus.Bits.F_CycleStatus = 0;
			break;
	}
	switch((pxCanMessage[5] >> 1) & 0x01)		//空调协议变更
	{
		default:
			break;
		case AC_ON_REQ_OFF:
			xAirConditionInfo.uAirConditionStatus.Bits.F_ACOnOff = 0;
			break;
		case AC_ON_REQ_ON:
			xAirConditionInfo.uAirConditionStatus.Bits.F_ACOnOff = 1;
			break;
	}
	switch((pxCanMessage[1] >> 4) & 0x01)
	{
		default:
			break;
		case AC_OFF_MODE_OFF:
			xAirConditionInfo.uAirConditionStatus.Bits.F_AirOnOff = 0;
			break;
		case AC_OFF_MODE_ON:
			xAirConditionInfo.uAirConditionStatus.Bits.F_AirOnOff = 1;
			break;
	}
	switch((pxCanMessage[1] >> 5) & 0x01)
	{
		default:
			break;
		case AC_TEMP_SET_ERR_OFF:
			break;
		case AC_TEMP_SET_ERR_ON:
			break;
	}
	
	xAirConditionInfo.OutsideTemp = pxCanMessage[2];
	
	xAirConditionInfo.FrontLeftTemp = (pxCanMessage[3] >> 1) & 0x7F;
	
	switch(pxCanMessage[4] & 0x03)
	{
		default:
			break;
		case AC_AQS_OFF:
			xAirConditionInfo.uACExternInfo3.Bits.AQS = 0;
			break;
		case AC_AQS_ON:
			xAirConditionInfo.uACExternInfo3.Bits.AQS = 1;
			break;
	}
	switch((pxCanMessage[4] >> 2) & 0x03)
	{
		default:
			break;
		case AC_ANION_WORK_OFF:
			xAirConditionInfo.uACExternInfo3.Bits.ION = 0;
			break;
		case AC_ANION_WORK_ON_CLEAN:
			xAirConditionInfo.uACExternInfo3.Bits.ION = 2;
			break;
		case AC_ANION_WORK_ON_ION:
			xAirConditionInfo.uACExternInfo3.Bits.ION = 1;
			break;
		case AC_ANION_WORK_FAULT:
			break;
	}
	
	xAirConditionInfo.WindLevel = (pxCanMessage[4] >> 4) & 0x0F;
	xAirConditionInfo.DisplayState = pxCanMessage[5];
/*	switch(pxCanMessage[5] & 0x01)
	{
		default:
			break;
		case AC_DIS_INACTIVE:
			xAirConditionInfo.DisplayState ^= (1 << 0);
			break;
		case AC_DIS_ACTIVE:
			xAirConditionInfo.DisplayState |= (1 << 0);
			break;
	}

	switch((pxCanMessage[5] >> 1) & 0x01)
	{
		default:
			break;
		case AC_REQ_LIGHT_OFF:
			break;
		case AC_REQ_LIGHT_ON:
			break;
	}

	switch((pxCanMessage[5] >> 2) & 0x03)
	{
		default:
			break;
		case AC_CFG_MANUAL:
			break;
		case AC_CFG_ELECTRICAL:
			break;
		case AC_CFG_AUTO:
			break;
		case AC_CFG_AUTO_WITH:
			break;
	}
*/	
	xAirConditionInfo.FrontLeftTempLev = pxCanMessage[7] & 0x7F;

//	if(AC_STATE_WORK != eAcState)
//	{
//		return;
//	}
	vApuWrite(APUW_GID_CAN_INFO, APUW_CAN_AC, (BYTE*)&xAirConditionInfo, sizeof(xAirConditionInfo));

}

