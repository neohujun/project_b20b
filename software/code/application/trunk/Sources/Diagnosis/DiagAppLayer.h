/******************************************************************************
**
**								Copyright (C) 2017  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		DiagAppLayer.h
**	Abstract:		head of diagnosis app layer
**	Creat By:		Zeno Liu
**	Creat Time:		2017.03.20
**	Version:		v1.0
**
******************************************************************************/

#ifndef	__DIAG_APP_LAYER_H__
#define	__DIAG_APP_LAYER_H__

/* \brief
*/
#include "type.h"

/* \brief
	function declaration
*/
void vDiagAppTask(void);
void vDiagAppTaskInit(void);
void vDiagAppTimer(void);

//通信控制
BOOL isDiagAppCanAppTx(void);
BOOL isDiagAppCanAppRx(void);
BOOL isDiagAppCanNMTx(void);
BOOL isDiagAppCanNmRx(void);

//DTC开关
BOOL isDiagAppDtcOn(void);

//config info handle
void vDiagAppConfigHandle(void);

BOOL isDiagAppIOIllume(void);
BOOL isDiagAppIOSpeed(void);
void vDiagAppTxVinTask(void);

#endif


