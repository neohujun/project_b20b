/******************************************************************************
**
**								Copyright (C) 2017  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		Amp.h
**	Abstract:			head of Amp control
**	Creat By:		neo HU
**	Creat Time:		2017.7.1
**	Version:		v1.0
**
******************************************************************************/

#ifndef __AMP_H__
#define __AMP_H__

/* \brief
*/
#include "type.h"


/* \brief
	function declaration
*/
void vAmpInit(void);
void vAmpStbyEnable(BOOL enable);
void vAmpMuteHardware(BOOL mute);

#endif
