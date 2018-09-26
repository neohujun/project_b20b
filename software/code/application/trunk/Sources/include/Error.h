/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		Error.h
**	Abstract:		head of error record
**	Creat By:		Zeno Liu
**	Creat Time:		2015.03.09
**	Version:		v1.0
**
******************************************************************************/

#ifndef	__ERROR_H__
#define	__ERROR_H__

/* \brief
	include
*/
#include "Config.h"
#include "type.h"

/* \brief
	error struct define
*/
typedef struct
{
	BYTE Uart1Error;
	BYTE Uart2Error;

	BYTE ApuDrvParityError;
	BYTE ApuDrvWaitTimeoutError;
	BYTE ApuDrvMemCopyOverflowError;
	BYTE ApuDrvWriteBuffFrameError;
	BYTE ApuDrvReadBuffOverflowError;
	BYTE ApuDrvWriteBuffOverflowError;

	BYTE ApuReadGroupIdUnknown;
	BYTE ApuResetFailError;
	BYTE ApuAwakeFailError;

	BYTE IICError;

	BYTE CanReadBuffOverflowError;
	BYTE CanWriteBuffOverflowError;
	BYTE CanWritePriorityOverflowError;
	BYTE CanError;

	BYTE EepromOutofRangeError;
	BYTE EepromError;
	BYTE MemoryError;

	BYTE ADConvertError;

	BYTE CopDrvParityError;
	BYTE CopDrvWaitTimeoutError;
	BYTE CopDrvMemCopyOverflowError;
	BYTE CopDrvWriteBuffFrameError;
	BYTE CopDrvReadBuffOverflowError;
	BYTE CopDrvWriteBuffOverflowError;

	BYTE Uart1TxTimeoutError;
	BYTE Uart2TxTimeoutError;
}ErrorRecord;

/* \brief
	variable declaration
*/
extern ErrorRecord xErrorRecord;

#endif


