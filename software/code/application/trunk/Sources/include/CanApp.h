/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		CanApp.h
**	Abstract:		head of can application
**	Creat By:		Zeno Liu
**	Creat Time:		2015.03.19
**	Version:		v1.0
**
******************************************************************************/

#ifndef	__CAN_APP_H__
#define	__CAN_APP_H__

/* \brief
*/
#include "can.h"

/* \brief
*/
void vCanAppTask(void);
void vCanAppInit(void);
void vCanAppTimer(void);
BOOL blCanAppWrite(pCanFrame pxCanFrame);
void SetCanAppDelayTimer(UINT08 DelayTimer);

#endif


