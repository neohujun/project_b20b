/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		Display.c
**	Abstract:		display application
**	Creat By:		neo Hu
**	Creat Time:		2017.03.14
**	Version:		v1.0
**
******************************************************************************/

/* \brief
	include files
*/
#include "Display.h"
#include "Tw88xx_Drv.h"

/* \brief
*/

/* \brief
*/
void vDisplayInit(void)
{
	vTw88xx_DrvInit();
}


void vDisplayTask(void)
{
	vTw88xx_DrvTask();
}

void vDisplayReset(void)
{
	Tw8836_Reset();
}



