/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto（China） Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		all.h
**	Abstract:		all include files
**	Creat By:		Zeno Liu
**	Creat Time:		2015.03.04
**	Version:		v1.0
**
******************************************************************************/



/* \brief
	include files
*/
#include "all.h"

/* \brief
	disable condition always true warning
*/
#pragma MESSAGE DISABLE C4000

/*
**-----------------------------------------------------------------------------
**
**	Abstract:
**		This function implements main function.
**
**	Parameters:
**		None
**
**	Returns:
**		None
**
**-----------------------------------------------------------------------------
*/
/* \brief
		This function implements main function.
*/
void main(void)
{
	//从上电到进入main需要60ms
	vSystemInit();
	
	while(TRUE)
	{
		__RESET_WATCHDOG();

		vSystemTask();
	}
}

