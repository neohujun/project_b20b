/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		Ins.h
**	Abstract:		head of instrument
**	Creat By:		neo Hu
**	Creat Time:		2011.11.27
**	Version:		v1.0
**
******************************************************************************/

#ifndef	__INS_APP_H__
#define	__INS_APP_H__

/* \brief
*/
#include "type.h"

/* \brief
	function declaration
*/
void vInsAppTask(void);
void vInsAppTaskInit(void);
void vInsAppTimer(void);

//communication control
BOOL isInsAppCanNMTx(void);
BOOL isInsAppCanNmRx(void);


//config info handle
void vInsAppConfigHandle(void);

BOOL isInsAppIOIllume(void);
BOOL isInsAppIOSpeed(void);
void vInsApuPlayInfoSet(BYTE* pData);
void vInsApuNaviSet(BYTE* pData);

#endif
