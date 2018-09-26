/******************************************************************************
**
**								Copyright (C) 2017 
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		Ac.h
**	Abstract:			head of air condition_CAN_672
**	Creat By:		neo HU
**	Creat Time:		2017.7.1
**	Version:		v1.0
**
******************************************************************************/

#ifndef __AC_H__
#define __AC_H__

/* \brief
	include
*/
#include "type.h"
#include "AirCondition.h"


extern AirConditionInfo xAirConditionInfo;

/* \brief
	function declaration
*/
void vAcInit(void);
void vAcTimer(void);
void vAcTask(void);
void vAcApuSet(BYTE* pData);
void vCanAcReadHandle(BYTE* pxCanMessage);

#endif
