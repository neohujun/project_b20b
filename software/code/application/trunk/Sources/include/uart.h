/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		uart.h
**	Abstract:		head of uart driver
**	Creat By:		Zeno Liu
**	Creat Time:		2015.03.05
**	Version:		v1.0
**
******************************************************************************/

#ifndef	__UART_H__
#define	__UART_H__
/* \brief
	include
*/
#include "list.h"

/* \brief
	function declaration
*/
void vUart1Init(BOOL isHighBaudrate);
void vUart1Start(List* pxRxList);
void vUart1Stop(void);
BOOL blUart1Write(BYTE* pData, BYTE length);
BOOL isUart1WriteBusy(void);

void INTUart1Tx(void);
void INTUart1Rx(void);
void INTUart1Error(void);

void vUart2Init(void);
void vUart2Start(List* pxRxList);
void vUart2Stop(void);
BOOL blUart2Write(BYTE* pData, BYTE length);
BOOL isUart2WriteBusy(void);

void INTUart2Tx(void);
void INTUart2Rx(void);
void INTUart2Error(void);

void vUartTimer(void);

#endif


