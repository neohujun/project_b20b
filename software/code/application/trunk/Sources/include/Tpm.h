/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		Tpm.h
**	Abstract:		head of Tpm initialization
**	Creat By:		Zeno Liu
**	Creat Time:		2015.12.30
**	Version:		v1.0
**
******************************************************************************/

#ifndef	__TPM_H__
#define	__TPM_H__

/* \brief
	include
*/
#include "type.h"

/* \brief
*/
#define	tpmBRIGHTNESS_INIT			20

/* \brief
*/
extern BYTE TpmLampLevel;

/* \brief
	function declaration
*/
void vTpmInit(void);
void vTpmTimer(void);
void vINTTpm1Ch5(void);

#endif


