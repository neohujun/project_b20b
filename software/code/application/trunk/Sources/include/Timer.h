/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		timer.h
**	Abstract:		head of timer initialization
**	Creat By:		Zeno Liu
**	Creat Time:		2015.03.09
**	Version:		v1.0
**
******************************************************************************/

#ifndef	__TIMER_H__
#define	__TIMER_H__

/* \brief
	include
*/
#include "type.h"

/* \brief
*/
#define	timeMOD			1000//0x2710

/* \brief
	function declaration
*/
void vTimerInit(void);
void vTimerStart(void);
void vINTTimer(void);

/* \brief
	variable declaration
*/
extern BYTE Timer10msCounter;
void vPwmODOControl(BOOL isOpen, BYTE Brightness);
void vTimerSetRevTimer(BYTE timer);

#endif


