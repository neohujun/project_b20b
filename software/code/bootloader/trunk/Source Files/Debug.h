/***************************************************Copyright (c)******************************************************
**										2008-2015,Shenzhen BYD Auto Co.,Ltd.
**		All rights reserved by SoftWare Team,Multimedia Electronics Factory,Division XV,Shenzhen BYD Auto Co.,Ltd.
**
**----------------------------------------------------File Info--------------------------------------------------------
**
**	Filename:			Debug.h
**	Abstract:			Head file of UART DEBUG
**	Creat By:			Liu Can
**	Creat Time:			2010/10/26
**	Version:			V1.00
**
**	Modify By:
**	Modify Time:
**	Version:
**	Description:
**
**********************************************************************************************************************/

#ifndef	_DEBUG_H_
#define _DEBUG_H_

/**********************************************************************************************************************
**	Include Files
**********************************************************************************************************************/
#ifdef __DEBUG__
#include "LibConfig.h"

/**********************************************************************************************************************
**	function declaration
**********************************************************************************************************************/
void Printfpc(const UINT08* p_str, UINT08* p_data, UINT08 data_num);
void vAppTimer(BYTE TimerUnit);

#endif
#endif

