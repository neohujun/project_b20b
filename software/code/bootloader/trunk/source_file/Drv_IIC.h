/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		Drv_IIC.h
**	Abstract:		head of iic driver
**	Creat By:		Neo Hu
**	Creat Time:		2015.03.10
**	Version:		v1.0
**
******************************************************************************/

#ifndef	_DRV_IIC_H_
#define _DRV_IIC_H_

/**********************************************************************************************************************
**	Include Files
**********************************************************************************************************************/
#include "Public.h"


void vIICInit(BYTE SelfAddr);
void vIICWriteDrvConfig(void);

#endif


