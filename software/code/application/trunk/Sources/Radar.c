/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		Radar.c
**	Abstract:			radar function
**	Creat By:		Zeno Liu
**	Creat Time:		2015.10.27
**	Version:		v1.0
**
******************************************************************************/

/* \brief
	include
*/
#include "Radar.h"
#include "Tw88xx_Drv.h"
#include "Apu.h"
#include "logo.h"
#include "Avm.h"
#include "system.h"
#include "io.h"

#define EPS_ANGLE_BOUNDARY		7800
#define EPS_ANGLE_DIVISOR		135

static BYTE RadarEpsDelayTimer = 0;
static BYTE RadarPasDelayTimer = 0;
/* \brief
*/
void vRadarInit(void)
{
	RadarEpsDelayTimer = 20;
	RadarPasDelayTimer = 20;
}

/* \brief
*/
void vRadarTimer(void)
{
	if(0 != RadarEpsDelayTimer)
	{
		--RadarEpsDelayTimer;
	}
	if(0 != RadarPasDelayTimer)
	{
		--RadarPasDelayTimer;
	}
}

/* \brief
*/
void vRadarTask(void)
{

}

/* \brief
*/
void vCanEpsReadHandle(BYTE* pxCanMessage)
{
	BYTE epsOsdPosition = 0;
	WORD wPesAngle = 0;
	if((SYSTEM_ACC_IDLE != vSystemAccStatus())  || (!ioBAT_DET_IN))
	{//开机，关机状态，
		return;
	}
	if(xApuwDeviceStatus.ReverseFlag == FALSE)
	{
		return;
	}
	if(0 != RadarEpsDelayTimer || (TW8836_INIT_STATE_WORK != VideoSubCtl) || isAvmOn)
	{
		return;
	}
	wPesAngle = pxCanMessage[3] << 8;
	wPesAngle |= pxCanMessage[4];
	
	if(wPesAngle >= EPS_ANGLE_BOUNDARY)
	{
		wPesAngle -= EPS_ANGLE_BOUNDARY;
		epsOsdPosition = (BYTE)(wPesAngle / EPS_ANGLE_DIVISOR);
		ShowLogEpsLeft(epsOsdPosition);
	}
	else
	{
		wPesAngle = EPS_ANGLE_BOUNDARY - wPesAngle;
		epsOsdPosition = (BYTE)(wPesAngle / EPS_ANGLE_DIVISOR);
		ShowLogEpsRight(epsOsdPosition);
	}
	
	RadarEpsDelayTimer = 10;
}

/* \brief
*/
void vCanPasReadHandle(BYTE* pxCanMessage)
{
	BYTE i = 0;
	BYTE xRadarPosition[osd_RADAR_MAX_LENGTH] = {0};
	if((SYSTEM_ACC_IDLE != vSystemAccStatus())  || (!ioBAT_DET_IN))
	{//开机，关机状态，
		return;
	}
	if(0 != RadarPasDelayTimer || (TW8836_INIT_STATE_WORK != VideoSubCtl) || isAvmOn)
	{
		return;
	}

	xRadarPosition[0] = pxCanMessage[2] & 0x0F;
	xRadarPosition[1] = (pxCanMessage[2] >> 4) & 0x0F;
	xRadarPosition[2] = pxCanMessage[0] & 0x0F;
	xRadarPosition[3] = (pxCanMessage[0] >> 4) & 0x0F;
	xRadarPosition[4] = pxCanMessage[1] & 0x0F;
	xRadarPosition[5] = (pxCanMessage[1] >> 4) & 0x0F;
	
	for(i = 0; i < osd_RADAR_MAX_LENGTH; i++)
	{
		if(xRadarPosition[i] > 4)
		{
			xRadarPosition[i] = 4;
		}
	}
	
	ShowLogPas(xRadarPosition);
	RadarPasDelayTimer = 20;
}

