/**********************************************************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		SoftTimer.c
**	Abstract:		
**	Creat By:		Zeno Liu
**	Creat Time:		2015.04.13
**	Version:		v1.0
**
**********************************************************************************************************************/

/**********************************************************************************************************************
**	Include Files
**********************************************************************************************************************/
#include "SoftTimer.h"
#include <MC9S08DZ60.h>

/**********************************************************************************************************************
**	variable define
**********************************************************************************************************************/
SoftTimerParaArea soft_timer_para;

/**********************************************************************************************************************
**	Func Name:		void SoftTimerTask(void)
**	Parameters:		void
**	Return Value:	void
**	Abstract:		common soft timer process
**********************************************************************************************************************/
void SoftTimerTask(void)
{
	UINT08 u08_timer_temp;
	
	TimerTask();
	
	u08_timer_temp = GetMilliSecondTimer();
	if(u08_timer_temp >= Timer_Unit_40_MS)	//40ms
	{
		SetMilliSecondTimer(u08_timer_temp - Timer_Unit_40_MS);
//		++soft_timer_para.u40ms_timer;
		
		SystemFortyMSecTimer();
		
//		if(soft_timer_para.u40ms_timer >= Timer_1000_MS)		//1s
//		{
//			soft_timer_para.u40ms_timer = soft_timer_para.u40ms_timer - Timer_1000_MS;
			
//		}
	}
}


