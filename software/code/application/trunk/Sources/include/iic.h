/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		iic.h
**	Abstract:		head of iic driver
**	Creat By:		Zeno Liu
**	Creat Time:		2015.03.10
**	Version:		v1.0
**
******************************************************************************/

#ifndef	__IIC_H__
#define	__IIC_H__
/* \brief
	include
*/
#include "list.h"
#include "Common.h"
#include "type.h"

/* \brief
*/
extern BYTE IICMcuResetCount;

/* \brief
	function declaration
*/
void vIICInit(BYTE SelfAddr);
void vIICAddrInit(BYTE writeAddr, BYTE readAddr);
void vIICTimer(void);
BOOL blIICRead (BYTE index, BYTE* value);
BOOL blIICWrite (BYTE index, BYTE value);
void blIICWriteByte (WORD index, BYTE value);
void blIICReadByte (WORD index, BYTE* value);
void tw8836I2CWrite(BYTE index, BYTE value);
BOOL blIICWriteNByte(BYTE addr, BYTE* pData, BYTE length);
U8 ReadI2C (U8 index);
void ReadTW88Page(U8 *ucPage);
void WriteTW88Page(U8 ucPage);

#define WriteTW88(a, b)		tw8836I2CWrite(  (U8)(a), (U8)(b) )
#define	ReadTW88(a)			ReadI2C((U8)(a))

#endif


