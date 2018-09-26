/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		rtc.c
**	Abstract:		rtc driver
**	Creat By:		Zeno Liu
**	Creat Time:		2015.08.05
**	Version:		v1.0
**
******************************************************************************/

/* \brief
	include files
*/
#include "Rtc.h"
#include "Error.h"
#include "common.h"
#include "Debug.h"

#include <MC9S08DZ60.h>
#include <hidef.h>

/* \brief
*/
#define	rtcMOD			100

/* \brief
	variable define
*/

/* \brief
*/
void vRtcInit(void)
{
	RTCMOD = rtcMOD;
	RTCSC = 0x1B;
}

/* \brief
*/
void vRtcStart(void)
{
	RTCSC = 0x18;
}

/* \brief
*/
void vRtcStop(void)
{
	RTCSC = 0x00;
}

/* \brief
*/
__interrupt void vINTRtc(void)
{
	RTCSC_RTIF = 1;
/*	
	if(++xSystemWatchTime.Second == 60)
	{
		xSystemWatchTime.Second = 0;

		if(++xSystemWatchTime.Minute == 60)
		{
			xSystemWatchTime.Minute = 0;

			if(++xSystemWatchTime.Hour == 24)
			{
				xSystemWatchTime.Hour = 0;
			}
		}
	}
*/
}

