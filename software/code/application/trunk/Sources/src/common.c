/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		common.c
**	Abstract:		define some common func
**	Creat By:		Zeno Liu
**	Creat Time:		2015.03.10
**	Version:		v1.0
**
******************************************************************************/

/* \brief
	include
*/
#include "common.h"
#include <MC9S08DZ60.h>
#include <hidef.h>
#include "Pwm.h"
#include "Amp.h"

/* \brief
*/
enumHardwareVer eHardwareVer = HARDWARE_VER_4;
SystemDate xSystemDate;
SystemWatchTime xSystemWatchTime;
BOOL isHardwareInitCompleted = FALSE;


/* \brief
*/
void vCommonInit(void)
{
	isHardwareInitCompleted = FALSE;
}

/* \brief
*/
void Delay(WORD wCounter)
{
	for(; 0!=wCounter; --wCounter)
	{
		__RESET_WATCHDOG();
	}
}

/* \brief
*/
BOOL blWaitSignalOk(BYTE* pSignal, BYTE SignalBit, BOOL OkSignal, WORD wCounter)
{
	for(; 0!=wCounter; --wCounter)
	{
		if(((*pSignal>>SignalBit)&0x01) == OkSignal)
		{
			return TRUE;
		}

		__RESET_WATCHDOG();
	}
	
	return FALSE;
}

/* \brief
*/
enumSignalStatus GetFilterSignal(SignalFilter* pxSignalFilter)
{
	if(pxSignalFilter->OkSignal == 0)
	{//initialization signal value for not set
		pxSignalFilter->OkSignal = 1;
	}
	if(pxSignalFilter->FilterCount >= 2)
	{
		pxSignalFilter->FilterCount -= 2;
	}
	else
	{	//default count is 3,FilterCount = 1;
		pxSignalFilter->FilterCount = 1;
	}
	
	if(pxSignalFilter->NewSignal == pxSignalFilter->OldSignal)
	{
		if(pxSignalFilter->counter == 0)
		{
			return ((pxSignalFilter->NewSignal == pxSignalFilter->OkSignal) && (pxSignalFilter->OkSignal != 0))\
					? (SIGNAL_ON) : (SIGNAL_OFF);
		}
		else
		{
			--pxSignalFilter->counter;
			return SIGNAL_CHATTER;
		}
	}
	else
	{
		(pxSignalFilter->counter) = pxSignalFilter->FilterCount;
		return SIGNAL_CHATTER;
	}
}

/* \brief
*/
enumSignalStatus GetFilterValue(KeyValueFilter* pxValueFilter)
{
	if(pxValueFilter->OkValue == 0)
	{//initialization signal value for not set
		pxValueFilter->OkValue = 1;
	}
	if(pxValueFilter->FilterCount >= 2)
	{
		pxValueFilter->FilterCount -= 2;
	}
	else
	{	//default count is 3,FilterCount = 1;
		pxValueFilter->FilterCount = 1;
	}
	
	if((pxValueFilter->OldValue) == (pxValueFilter->NewValue))
	{
		if((pxValueFilter->counter == 0)
			&& ((pxValueFilter->BaseValue) == (pxValueFilter->NewValue)))
		{
			return (pxValueFilter->OkValue) == (pxValueFilter->NewValue)\
					? (SIGNAL_ON) : (SIGNAL_OFF);
		}
		else
		{
			pxValueFilter->BaseValue = pxValueFilter->OldValue;
			--pxValueFilter->counter;
			return SIGNAL_CHATTER;
		}
	}
	else
	{
		(pxValueFilter->counter) = pxValueFilter->FilterCount;
		return SIGNAL_CHATTER;
	}
}

/* \brief
*/
void vResetMcuJumpBoot(void)
{
	DisableInterrupts;

	asm(jmp $ff00);
}

/* \brief
*/
void vResetMcuJumpBgnd(void)
{
	vAmpMuteHardware(ON);
	IOBLcdControl(OFF);
	DisableInterrupts;

	Delay(5000);
	Delay(5000);
	asm(BGND);		//illegal instruction caused mcu reset
}

