/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		ad.h
**	Abstract:		head of ad driver
**	Creat By:		Zeno Liu
**	Creat Time:		2015.04.09
**	Version:		v1.0
**
******************************************************************************/

#ifndef __AD_H__
#define __AD_H__

/* \brief
*/
#include "type.h"

/* \brief
*/
void vADInit(void);
BYTE ADGetValue(BYTE channel);

#endif