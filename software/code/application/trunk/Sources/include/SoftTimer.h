/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		SoftTimer.h
**	Abstract:		head of soft timer
**	Creat By:		Zeno Liu
**	Creat Time:		2015.03.09
**	Version:		v1.0
**
******************************************************************************/

#ifndef	__SOFTTIMER_H__
#define	__SOFTTIMER_H__

/* \brief
	define
*/
#define	softtimerTEN_MILLISECOND_COUNT		1
#define	softtimerTIMER_UNIT					10
#define	softtimerSECOND_COUNT				100							// 1S count value
#define	softtimerMILLISECOND(timer)			(timer/softtimerTIMER_UNIT)	//count/10 because timer unit is 10ms

/* \brief
	function declaration
*/
void vSoftTimerInit(void);
void vSoftTimerTask(void);

#endif


