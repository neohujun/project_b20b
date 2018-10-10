/**********************************************************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		Timer.c
**	Abstract:		
**	Creat By:		Zeno Liu
**	Creat Time:		2015.04.13
**	Version:		v1.0
**
**********************************************************************************************************************/

/**********************************************************************************************************************
**	Include Files
**********************************************************************************************************************/
#include "Timer.h"
#include <MC9S08DZ60.h>

#include "Debug.h"

/**********************************************************************************************************************
**	variable define
**********************************************************************************************************************/
UINT08 gu08_ms_timer;

/**********************************************************************************************************************
**	Func Name:		void TimerTask(void)
**	Parameters:		void
**	Return Value:	void
**	Abstract:		timer task
**********************************************************************************************************************/
void TimerTask(void)
{
	if(TM_OVERFLOW_FLAG)
	{
		TM_OVERFLOW_FLAG_CLR;
		
		++gu08_ms_timer;

		vAppTimer(10);
	}
	
	return;
}

/**********************************************************************************************************************
**	Func Name:		UINT08 GetMilliSecondTimer(void)
**	Parameters:		void
**	Return Value:	UINT08 ms
**	Abstract:		return timer(millisecond)
**********************************************************************************************************************/
UINT08 GetMilliSecondTimer(void)
{
	return gu08_ms_timer;
}

/**********************************************************************************************************************
**	Func Name:		void SetMilliSecondTimer(UINT08 timer)
**	Parameters:		void
**	Return Value:	void
**	Abstract:		set ms timer
**********************************************************************************************************************/
void SetMilliSecondTimer(UINT08 timer)
{
	gu08_ms_timer = timer;
	
	return;
}

/**********************************************************************************************************************
**	Func Name:		void TimerWait(UINT16 time)
**	Parameters:		void
**	Return Value:	void
**	Abstract:		timer wait
**********************************************************************************************************************/
void TimerWait(UINT16 time)
{
	volatile UINT16 timer=0;

	for(timer=0; timer!=time; ++timer)
	{
		__RESET_WATCHDOG();
	}
}

/**********************************************************************************************************************
**	Func Name:		__interrupt void INTTMH0Process(void)
**	Parameters:		void
**	Return Value:	void
**	Abstract:		User interrupt service routine, timer interrupt process 1 ms once
**********************************************************************************************************************/
/*__interrupt void isrVtpm1ovf(void)
{
    TM_OVERFLOW_FLAG_CLR;
    
    ++gu08_ms_timer;
}*/


 
