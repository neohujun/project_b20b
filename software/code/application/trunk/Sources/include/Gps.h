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

#ifndef __GPS_H__
#define __GPS_H__

/* \brief
*/
#include "type.h"
#include "ApuWSysInfo.h"


/* \brief
	function declaration
*/
void vGpsInit(void);
void vGpsTimer(void);
void vGpsTask(void);
void vGpsApuSet(enumApuwSetTime eApuwSetTime, BYTE* pData);

#endif
