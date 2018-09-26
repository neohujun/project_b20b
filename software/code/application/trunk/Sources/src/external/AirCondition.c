/******************************************************************************
**
**								Copyright (C) 2017  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		AirCondition.c
**	Abstract:			air condition_CAN_361
**	Creat By:		neo HU
**	Creat Time:		2017.7.1
**	Version:		v1.0
**
******************************************************************************/

/* \brief
	include files
*/
#include "AirCondition.h"
#include "List.h"
#include "system.h"
#include "CanApp.h"


/* \brief
*/
typedef enum
{
	AIR_STATE_WAIT,
	AIR_STATE_INIT,
	AIR_STATE_WORK,
	AIR_STATE_TX_CAN,
	AIR_STATE_TX_NONE,
	AIR_STATE_TX_APU,
	AIR_STATE_OFF
}enumAirState;

/* \brief
	variable
*/
CanFrame xAirMessage;
static enumAirState eAirState;

/* \brief
	variable
*/
static BYTE wAirCycleTime;
static BYTE wAirCycleCount;


/* \brief
*/
void vAirInit(void)
{
	xAirMessage.ID[0] = canID_TX_AC_361_H;
	xAirMessage.ID[1] = canID_TX_AC_361_L;
	xAirMessage.Length = 8;

	xAirMessage.Data[0] = 0x00;
	xAirMessage.Data[1] = 0x00;
	xAirMessage.Data[2] = 0x00;
	xAirMessage.Data[3] = 0x00;
	xAirMessage.Data[4] = 0x00;
	xAirMessage.Data[5] = 0x00;
	xAirMessage.Data[6] = 0x00;
	xAirMessage.Data[7] = 0x00;
	
	wAirCycleTime = 0;
	wAirCycleCount = 0;
	eAirState = AIR_STATE_WAIT;
}

/* \brief
*/
void vAirTimer(void)
{
	if(0 != wAirCycleTime)
	{
		--wAirCycleTime;
	}
}


/* \brief
*/
void vAirTask(void)
{
	switch(eAirState)
	{
		default:
			break;
		case AIR_STATE_WAIT:
			eAirState = AIR_STATE_WORK;
			break;
		case AIR_STATE_INIT:
			break;
		case AIR_STATE_WORK:
			if(0 == wAirCycleTime)
			{
				(void)blCanAppWrite(&xAirMessage);
				wAirCycleTime = 10;
			}
			break;
		case AIR_STATE_TX_CAN:
			if(0 == wAirCycleTime)
			{
				(void)blCanAppWrite(&xAirMessage);
				wAirCycleTime = 10;
				++wAirCycleCount;
			}
			if(wAirCycleCount > 2)
			{
				wAirCycleCount = 0;
				wAirCycleTime = 10;
				eAirState = AIR_STATE_TX_NONE;
			}
			break;

		case AIR_STATE_TX_NONE:
			if(0 == wAirCycleTime)
			{
				xAirMessage.Data[0] = 0x00;
				xAirMessage.Data[1] = 0x00;
				xAirMessage.Data[2] = 0x00;
				xAirMessage.Data[3] = 0x00;
				xAirMessage.Data[4] = 0x00;
				xAirMessage.Data[5] = 0x00;
				xAirMessage.Data[6] = 0x00;
				xAirMessage.Data[7] = 0x00;
				(void)blCanAppWrite(&xAirMessage);
				wAirCycleTime = 10;
				++wAirCycleCount;
			}
			if(wAirCycleCount > 2)
			{
				wAirCycleCount = 0;
				eAirState = AIR_STATE_WORK;
			}
			break;
			break;

		case AIR_STATE_TX_APU:
			break;
			
		case AIR_STATE_OFF:
			break;
	}
}

void vAirApuSet(BYTE* pData)
{
	BYTE i = 0;
	xAirMessage.Data[0] = 0x00;
	xAirMessage.Data[1] = 0x00;
	xAirMessage.Data[2] = 0x00;
	xAirMessage.Data[3] = 0x00;
	xAirMessage.Data[4] = 0x00;
	xAirMessage.Data[5] = 0x00;
	xAirMessage.Data[6] = 0x00;
	xAirMessage.Data[7] = 0x00;
	
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
					xAirMessage.Data[2] = 1 << 7;
					break;
				case 1:					//open
					xAirMessage.Data[2] = 1 << 7;
					break;
			}
			break;
			
		case AC_CMD_AC_SWITCH:
			switch(pData[1])
			{
				default:
					break;
				case 0:					//AC off
					xAirMessage.Data[0] = 1 << 0;
					break;
				case 1:					//on
					xAirMessage.Data[0] = 1 << 0;
					break;
			}
			break;
			
		case AC_CMD_AC_CYCLE_CTL:
			switch(pData[1])
			{
				default:
					break;
				case 0:					//inside
					xAirMessage.Data[0] = 1 << 5;
					break;
				case 1:					//out
					xAirMessage.Data[0] = 1 << 5;
					break;
				case 2:
					break;
			}
			break;
			
		case AC_CMD_DUAL_SWITCH:
			break;
			
		case AC_CMD_REAR_DEFROST:
			switch(pData[1])
			{
				default:
					break;
				case 0:					//off
					xAirMessage.Data[0] = 1 << 4;
					break;
				case 1:					//on
					xAirMessage.Data[0] = 1 << 4;
					break;
			}
			break;
			
		case AC_CMD_AUTO_SWITCH:
			switch(pData[1])
			{
				default:
					break;
				case 0:					//off
					xAirMessage.Data[2] = 1 << 4;
					break;
				case 1:					//on
				case 2:
					xAirMessage.Data[2] = 1 << 4;
					break;
			}
			break;

		case AC_CMD_FL_TEMP_DIRECT:	
			switch(pData[1])
			{
				default:
					break;
				case 0:					//Down
					xAirMessage.Data[2] = 1 << 1;
					break;
				case 1:					//up
					xAirMessage.Data[2] = 1 << 0;
					break;
			}
			break;

		case AC_CMD_SET_FRONT_WIND_MODE:
			switch(pData[1])
			{
				default:
					break;
				case 2:					//face
					xAirMessage.Data[1] = 1 << 3;
					break;
				case 4:					//foot
					xAirMessage.Data[1] = 1 << 5;
					break;
				case 6:					//B/L
					xAirMessage.Data[1] = 1 << 4;
					break;
				case 0x0C:		//foot + defrost
					xAirMessage.Data[1] = 1 << 6;
					break;
				case 8:					//defrost
					xAirMessage.Data[1] = 1 << 7;
					break;
			}
			break;
		case AC_CMD_F_WIND_LEV_ADJUST:	//00~0E: 0~14 lev
			if(0 == pData[1])
			{
				pData[1] = 1;
			}
			xAirMessage.Data[4] = pData[1] & 0x0F;
			break;

		case AC_CMD_FORCE_DEFROST:

			switch(pData[1])
			{
				default:
					break;
				case 0:					//off
					xAirMessage.Data[0] = 1 << 3;
					break;
				case 1:					//on
					xAirMessage.Data[0] = 1 << 3;
					break;
			}

			break;
		case AC_CMD_AQS:
			switch(pData[1])
			{
				default:
					break;
				case 0:					//off
					xAirMessage.Data[3] = 1 << 2;
					break;
				case 1:					//on
					xAirMessage.Data[3] = 1 << 2;
					break;
			}
			break;
		case AC_CMD_ION:
			switch(pData[1])
			{
				default:
					break;
				case 0:					//off
					xAirMessage.Data[3] = 1 << 3;
					break;
				case 1:					//on
					xAirMessage.Data[3] = 1 << 3;
					break;
			}
			break;
		case AC_CMD_LOW:
			switch(pData[1])
			{
				default:
					break;
				case 0:					//off
					xAirMessage.Data[2] = 1 << 5;
					break;
				case 1:					//on
					xAirMessage.Data[2] = 1 << 5;
					break;
			}
			break;
		case AC_CMD_HOT:
			switch(pData[1])
			{
				default:
					break;
				case 0:					//off
					xAirMessage.Data[2] = 1 << 6;
					break;
				case 1:					//on
					xAirMessage.Data[2] = 1 << 6;
					break;
			}
			break;
		case AC_CMD_FL_TEMP_LEVEL:
			xAirMessage.Data[5] = pData[1];
			break;
			
	}
	xAirMessage.Data[3] |= 1 << 4;
	wAirCycleTime = 0x00;
	eAirState = AIR_STATE_TX_CAN;
}


