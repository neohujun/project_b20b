/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		Ems.h
**	Abstract:		head of ENGINE_IDLE_AND_VEHICLE_SPEED
**	Creat By:		Neo hu
**	Creat Time:		2018.1.25
**	Version:		v1.0
**
******************************************************************************/

#ifndef __EMS_H__
#define __EMS_H__

/* \brief
*/
#include "type.h"

/* \brief
	function declaration
*/
void vEmsInit(void);
void vEmsTimer(void);
void vCanEms3A0ReadHandle(BYTE* pxCanMessage);
BOOL blCanEmsSpeed(WORD wSpeed);


#endif
