/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		SoftTimer.c
**	Abstract:		soft timer
**	Creat By:		Zeno Liu
**	Creat Time:		2015.03.09
**	Version:		v1.0
**
******************************************************************************/

/* \brief
	include files
*/
#include "SoftTimer.h"
#include "Timer.h"
#include "Apu.h"
#include "Key.h"
#include "common.h"
#include "iic.h"
#include "Debug.h"
#include "system.h"
#include "Radar.h"
#include "CanApp.h"
#include "Avm.h"
#include "Bcm.h"
#include "Ac.h"
#include "Amp.h"
#include "Radar.h"
#include "Gps.h"
#include "Tw88xx_Drv.h"
#include "AirCondition.h"
#include "DiagAppLayer.h"
#include "NetworkManager.h"
#include "Uart.h"
#include "InsApp.h"

/* \brief
*/
//#define	softtimerSECOND_COUNT			100
#define	softtimerTIMER_MAX_DECREASE			10

/* \brief
*/
static BYTE SoftTimerSecCount;
static BYTE SoftTimerMinCount;

/* \brief
*/
void vSoftTimerInit(void)
{
	vTimerStart();
}

/* \brief
*/
//#include "DTV.h"
//BYTE timer;
void vSoftTimerTask(void)
{
	BYTE i = 0;
	
	for(i=0;(softtimerTEN_MILLISECOND_COUNT <= Timer10msCounter)&&(softtimerTIMER_MAX_DECREASE!=i);++i)
	{
		--Timer10msCounter;
		
		vApuTimer();
//		vAudioTimer();

		vKeyTimer();

		vSystemTimer();
		vIICTimer();
		vAvmTimer();
		vBcmTimer();
		vAcTimer();
		vAirTimer();
		vRadarTimer();
		vGpsTimer();
		vTw88xx_DrvDelay();
		vDiagAppTimer();
		vNMTimer();
		vCanAppTimer();
		vUartTimer();
		vInsAppTimer();
		
		if(softtimerSECOND_COUNT <= ++SoftTimerSecCount)
		{
			SoftTimerSecCount -= softtimerSECOND_COUNT;
			vApuTimerSecond();

			if(softtimerMINUTE_COUNT <= ++SoftTimerMinCount)
			{
				SoftTimerMinCount -= softtimerMINUTE_COUNT;
				vApuTimerMinute();
			}
		}
	}
}



