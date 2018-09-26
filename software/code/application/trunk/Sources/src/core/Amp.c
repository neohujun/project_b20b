/******************************************************************************
**
**								Copyright (C) 2017  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		Amp.c
**	Abstract:			Amp control
**	Creat By:		neo HU
**	Creat Time:		2017.7.1
**	Version:		v1.0
**
******************************************************************************/

/* \brief
	include
*/
#include "Amp.h"
#include "TDF85xx.h"
#include "common.h"


/* \brief
*/
void vAmpStbyEnable(BOOL enable)
{
	vTDF85xxStbyEnable(enable);
}


/* \brief
*/
void vAmpMuteHardware(BOOL mute)
{
	vTDF85xxMuteHardware(mute);
}



