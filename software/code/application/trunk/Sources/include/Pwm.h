/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		Pwm.h
**	Abstract:		head of Pwm initialization
**	Creat By:		Zeno Liu
**	Creat Time:		2015.08.03
**	Version:		v1.0
**
******************************************************************************/

#ifndef	__PWM_H__
#define	__PWM_H__

/* \brief
	include
*/
#include "type.h"
#include "Config.h"

/* \brief
*/
#define	pwmMONITOR_BRIGHTNESS_MAX				20
#define	pwmMONITOR_BRIGHTNESS_MIDDLE			10
#define	pwmMONITOR_BRIGHTNESS_MIN				0

#define	pwmMONITOR_BRIGHTNESS_CYCLE			300


/* \brief
	function declaration
*/
void vPwmInit(void);
void vPwmStart(void);
void vPwmMonitorControl(BOOL isOpen, BYTE Brightness);
void vPwmLightControl(BOOL isOpen, WORD Brightness);
void vPwmSpeedControl(BOOL isOpen, WORD speed);
void IOBLcdControl(BOOL open);

#endif


