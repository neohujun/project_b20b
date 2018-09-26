/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		Radar.h
**	Abstract:		head of radar
**	Creat By:		Zeno Liu
**	Creat Time:		2015.10.27
**	Version:		v1.0
**
******************************************************************************/

#ifndef __RADAR_H__
#define __RADAR_H__

/* \brief
	include
*/
#include "type.h"
/* \brief
	function declaration
*/
void vRadarInit(void);
void vRadarTimer(void);
void vRadarTask(void);
void vCanEpsReadHandle(BYTE* pxCanMessage);
void vCanPasReadHandle(BYTE* pxCanMessage);

#endif
