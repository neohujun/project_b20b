/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		Rtc.h
**	Abstract:		head of Rtc driver
**	Creat By:		Zeno Liu
**	Creat Time:		2015.08.05
**	Version:		v1.0
**
******************************************************************************/

#ifndef	__RTC_H__
#define	__RTC_H__
/* \brief
	include
*/

/* \brief
	function declaration
*/
void vRtcInit(void);
void vRtcStart(void);
void vRtcStop(void);
void vINTRtc(void);

#endif


