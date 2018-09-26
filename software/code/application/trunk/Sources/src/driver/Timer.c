/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		timer.c
**	Abstract:		timer initialization
**	Creat By:		Zeno Liu
**	Creat Time:		2015.03.09
**	Version:		v1.0
**
******************************************************************************/


/* \brief
	include files
*/
#include "Timer.h"
#include "Debug.h"
#include	"System.h"
#include "io.h"
#include "Bcm.h"

#include <MC9S08DZ60.h>

/* \brief
	10ms timer
*/
BYTE Timer10msCounter = 0;
BYTE Timer1msCounter = 0;
BYTE TimerRevCounter = 0;

/* \brief
	10 ms
*/
void vTimerInit(void)
{
	TPM1SC   = 0x00;
	TPM1CNT  = 0x00;

	TPM1MOD = timeMOD;		//(16/16M)*(1000) = 0.001s
}

/* \brief
*/
void vTimerStart(void)
{
	TPM1SC   = 0x4c;
	TPM1CNT  = 0x00;
}

/* \brief
*/
__interrupt void vINTTimer(void)
{
	TPM1SC_TOF   = 0;
	vBcmTimerMS();
	if(++Timer1msCounter > 9)
	{
		Timer1msCounter = 0;
		++Timer10msCounter;
	}
	if(0 != TimerRevCounter)
	{
		if(--TimerRevCounter == 0)
		{
			ioAPU_REV_OFF;
		}
		else if(TimerRevCounter == 60)
		{
			ioAPU_REV_ON;
		}
		else if(TimerRevCounter == 40)
		{
			ioAPU_REV_OFF;
		}
		else if(TimerRevCounter == 20)
		{
			ioAPU_REV_ON;
		}
	}
	
}

void vTimerSetRevTimer(BYTE timer)
{
	TimerRevCounter = timer + 1;
}

