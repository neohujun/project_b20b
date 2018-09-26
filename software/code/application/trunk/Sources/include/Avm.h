/******************************************************************************
**
**								Copyright (C) 2017  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		Avm.h
**	Abstract:			head of avm cmd
**	Creat By:		neo HU
**	Creat Time:		2017.7.1
**	Version:		v1.0
**
******************************************************************************/

#ifndef __AVM_H__
#define __AVM_H__

/* \brief
	include
*/
#include "type.h"

/* \brief
*/
typedef enum
{
	AVM_APU_REQ_NONE,
	AVM_APU_REQ_DISPLAY,
	AVM_APU_REQ_SET,
	AVM_APU_REQ_TOUCH
}enumAvmApuReqStatus;

/* \brief
*/
typedef enum
{
	AVM_APU_SET_NONE,
	AVM_APU_SET_LDW,
	AVM_APU_SET_BSD
}enumAvmApuSetStatus;

/* \brief
*/
typedef enum
{
	AVM_APU_SET_CMD_NONE,
	AVM_APU_SET_CMD_ENABLE,
	AVM_APU_SET_CMD_WARN_VOICE,
	AVM_APU_SET_CMD_WARN_LIGHT,
	AVM_APU_SET_CMD_WARN_SWC
}enumAvmApuSetCmd;

extern BOOL isAvmExistance;
extern BOOL isAvmOn;

/* \brief
	function declaration
*/
void vAvmInit(void);
void vAvmTimer(void);
void vAvmTask(void);
void vAvmApuSet(BYTE* pData);
void vCanAvmReadHandle(BYTE* pxCanMessage);

#endif
