/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		Tw88xx_Drv.c
**	Abstract:		Tw88xx drver
**	Creat By:		neo Hu
**	Creat Time:		2017.03.14
**	Version:		v1.0
**
******************************************************************************/
/* \brief
	include files
*/
#include "TDF85xx.h"
#include "iic.h"
#include "type.h"
#include "io.h"
#include "Config.h"


/* \brief
*/
void vTDF85xxStbyEnable(BOOL en)
{
	if(en)
	{
		ioAUDIO_AMP_STBY_ON;
	}
	else
	{
		ioAUDIO_AMP_STBY_OFF;
	}
}


/* \brief
*/
void vTDF85xxMuteHardware(BOOL mute)
{
	if(mute)
	{
		ioAUDIO_AMP_MUTE_ON;
	}
	else
	{
		ioAUDIO_AMP_MUTE_OFF;
	}
}




