/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		Gps.c
**	Abstract:			radar function
**	Creat By:		Zeno Liu
**	Creat Time:		2015.10.27
**	Version:		v1.0
**
******************************************************************************/

/* \brief
	include
*/
#include "Gps.h"
#include "List.h"
#include "CanApp.h"
#include "system.h"
#include "common.h"
#include <MC9S08DZ60.h>

/* \brief
*/
typedef enum
{
	GPS_STATE_WAIT,
	GPS_STATE_INIT,
	GPS_STATE_WORK,
	GPS_STATE_TX_DATA,
	GPS_STATE_OFF
}enumGpsState;

/* \brief
*/
CanFrame xGpsMessage;
static enumGpsState eGpsState;

/* \brief
*/
static BYTE wGpsCycleTime;

/* \brief
*/
void vGpsInit(void)
{
	xGpsMessage.ID[0] = canID_TX_AVM_510_H;
	xGpsMessage.ID[1] = canID_TX_AVM_510_L;
	xGpsMessage.Length = 8;

	xGpsMessage.Data[0] = 0x00;
	xGpsMessage.Data[1] = 0x00;
	xGpsMessage.Data[2] = 0x00;
	xGpsMessage.Data[3] = 0x00;
	xGpsMessage.Data[4] = 0x00;
	xGpsMessage.Data[5] = 0x00;
	xGpsMessage.Data[6] = 0x00;
	xGpsMessage.Data[7] = 0x00;

	eGpsState = GPS_STATE_WAIT;
	wGpsCycleTime = 0;
	
}

/* \brief
*/
void vGpsTimer(void)
{
	if(0 != wGpsCycleTime)
	{
		--wGpsCycleTime;
	}
}

/* \brief
*/
void vGpsTask(void)
{
//	vCanAvmReadHandle();
	switch(eGpsState)
	{
		default:
			break;
		case GPS_STATE_WAIT:
			wGpsCycleTime = 0;//100;
			eGpsState = GPS_STATE_WORK;
			break;
		case GPS_STATE_INIT:
			eGpsState = GPS_STATE_WORK;
			break;
		case GPS_STATE_WORK:
			if(0 != wGpsCycleTime)
			{
				break;
			}
			xGpsMessage.Data[0] = 0x00;
			xGpsMessage.Data[1] = 0x00;
			xGpsMessage.Data[2] = 0x00;
			xGpsMessage.Data[3] = 0x00;
			xGpsMessage.Data[4] = 0x00;
			xGpsMessage.Data[5] = 0x00;
			xGpsMessage.Data[6] = 0x00;
			xGpsMessage.Data[7] = 0x00;
			if(blCanAppWrite(&xGpsMessage))
			{
				wGpsCycleTime = 100;
			}
			break;
		case GPS_STATE_TX_DATA:
			xGpsMessage.Data[0] = 0x80;
			xGpsMessage.Data[1] = xSystemWatchTime.Hour;
			xGpsMessage.Data[2] = xSystemWatchTime.Minute;
			xGpsMessage.Data[3] = xSystemWatchTime.Second;
			xGpsMessage.Data[4] = xSystemDate.Day;
			xGpsMessage.Data[5] = (xSystemDate.Year - 14);
			xGpsMessage.Data[6] = xSystemDate.Month;
			(void)blCanAppWrite(&xGpsMessage);
			wGpsCycleTime = 100;
			eGpsState = GPS_STATE_WORK;
			break;
		case GPS_STATE_OFF:
			break;
	}
}

void vGpsApuSet(enumApuwSetTime eApuwSetTime, BYTE* pData)
{
	if(APUW_SET_DATE == eApuwSetTime)
	{
		WORD wYear = ((WORD)pData[0]<<1)|pData[1];
		xSystemDate.Year = wYear%100;
		xSystemDate.Month = pData[2];
		xSystemDate.Day = pData[3];
		if(xSystemDate.Year < 14)
		{
			xSystemDate.Year = 14;
		}
	}
	else
	{
		xSystemWatchTime.Hour = pData[0];
		xSystemWatchTime.Minute = pData[1];
		xSystemWatchTime.Second = pData[2];
		eGpsState = GPS_STATE_TX_DATA;
	}
	
}

