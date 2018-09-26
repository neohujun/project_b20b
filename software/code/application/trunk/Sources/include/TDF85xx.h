/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		Display.h
**	Abstract:		head of display application
**	Creat By:		neo Hu
**	Creat Time:		2017.03.14
**	Version:		v1.0
**
******************************************************************************/

#ifndef	__TDF85xx_H__
#define	__TDF85xx_H__
/* \brief
	include
*/
#include "type.h"

/* \brief
	function declaration
*/
void vTDF85xxInit(void);
void vTDF85xxStbyEnable(BOOL en);
void vTDF85xxMuteHardware(BOOL mute);
void vTDF85xxMuteSoftware(BOOL mute);
void vTDF85xxTask(void);

#endif