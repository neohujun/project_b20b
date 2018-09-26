/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		Debug.c
**	Abstract:		Debug
**	Creat By:		Zeno Liu
**	Creat Time:		2015.03.19
**	Version:		v1.0
**
******************************************************************************/

/* \brief
	include files
*/
#if	0
#include "Debug.h"

/* \brief
*/
CanMessage xDebugMessage;
BOOL isDebug = TRUE;
WORD wDebugCycleTime = 0;
WORD wDebugCycleCounter = 0;

//#ifdef	__DEBUG__

/* \brief
*/
void vDebug(BYTE length)
{
	if(isDebug)
	{
		xDebugMessage.length = length;
		
//		vCanAppWrite(&xDebugMessage);
	}
}

/* \brief
*/
void vDebugInit(void)
{
	xDebugMessage.wID = canID_DEBUG_INFO;
	xDebugMessage.length = 8;
	xDebugMessage.RTR = 0;

	wDebugCycleTime = 0;
	wDebugCycleCounter = 0;
}

/* \brief
*/
void vDebugCycleTimeTask(void)
{
	if(wDebugCycleCounter > wDebugCycleTime)
	{
		wDebugCycleTime = wDebugCycleCounter;
	}
	wDebugCycleCounter = 0;
}

/* \brief
*/
void vPrintf(const BYTE* pStr, BYTE* pData, BYTE DataLength)
{
	(void)(pStr == NULL);
	(void)(pData == NULL);
	(void)(DataLength == 0);
}
#endif

//#endif

