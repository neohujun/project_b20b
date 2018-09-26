/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		Debug.h
**	Abstract:		head of debug
**	Creat By:		Zeno Liu
**	Creat Time:		2015.03.19
**	Version:		v1.0
**
******************************************************************************/

#ifndef	__DEBUG_H__
#define	__DEBUG_H__

#if	0
/* \brief
*/
#include "CanApp.h"

/* \brief
*/
extern CanMessage xDebugMessage;
extern BOOL isDebug;
extern WORD wDebugCycleCounter;

/* \brief
*/
//#ifdef	__DEBUG__
//can for debug
void vDebug(BYTE length);
void vDebugInit(void);
void vDebugCycleTimeTask(void);

//uart for printf
void vPrintf(const BYTE* pStr, BYTE* pData, BYTE DataLength);

//#endif
#endif

#endif

