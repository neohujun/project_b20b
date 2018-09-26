/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		Bcm.h
**	Abstract:		head of radar
**	Creat By:		Zeno Liu
**	Creat Time:		2015.10.27
**	Version:		v1.0
**
******************************************************************************/

#ifndef __BCM_H__
#define __BCM_H__

/* \brief
*/
#include "type.h"

/* \brief
	function declaration
*/
void vBcmInit(void);
void vBcmTimer(void);
void vBcmTimerMS(void);
void vBcmTask(void);
void vBcmApuSurSet(BYTE* pData);
void vBcmApuTrunkSet(BYTE* pData);
void vCanBcm660ReadHandle(BYTE* pxCanMessage);
void vCanBcm4A0ReadHandle(BYTE* pxCanMessage);
void vCanBcm5B0ReadHandle(BYTE* pxCanMessage);
void vBcmHomeLightCtrl(BOOL isLightUp);
void vCanBcm620ReadHandle(BYTE* pxCanMessage);
void vBcmTxApuSurState(void);
void vBcmTxApuTrunkState(void);
void vReverseButton(void);



#endif
