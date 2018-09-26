/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		uart.c
**	Abstract:		uart driver
**	Creat By:		Zeno Liu
**	Creat Time:		2015.03.05
**	Version:		v1.0
**
******************************************************************************/

/* \brief
	include files
*/
#include "Config.h"
#include "uart.h"
#include "Error.h"
#include "Debug.h"
#include "common.h"
#include <MC9S08DZ60.h>
#include <hidef.h>

/* \brief
	disable warning message
*/
//#pragma MESSAGE DISABLE C1420

/* \brief
*/
#define	uart1AUTO_TEST_STARTUP_COUNT		5
#define	uartTX_TIMEOUT							50

/* \brief
	variable define
*/
static List* pxUart1RxList = NULL;
static BYTE* pUart1Buff;
static BYTE Uart1Length =0;
static BYTE Uart1Counter = 0;
static BYTE Uart1AutoTestCounter = 0;
static BYTE Uart1TxTimeoutTimer;

static List* pxUart2RxList = NULL;
static BYTE* pUart2Buff;
static BYTE Uart2Length =0;
static BYTE Uart2Counter = 0;
static BYTE Uart2TxTimeoutTimer;

/* \brief
	UARTA1 initialization
*/
void vUart1Init(BOOL isHighBaudrate)
{
	SCI1C2 = 0x00;		//disable sci before initialization
	
	if(isHighBaudrate)
	{	//115200
		SCI1BDH = 0x00;
		SCI1BDL = 0x09;		//BAUD = BUS / (16 * SCI2BD) = 16M /16/9 = 115200

		SCI1C1 = 0x00;

		SCI1C2 = 0x20;		//enable rx interrupt
		SCI1C3 = 0x0f;		//enable error interrupt
	}
	else
	{
	//19200时发送数据反转，适应M40小屏发送
		SCI1BDH = 0x00;
		SCI1BDL = 0x34;		//BAUD = BUS / (16 * SCI2BD) = 16M /16/0x34 = 19200

		SCI1C1 = 0x00;

		SCI1C2 = 0x20;		//enable rx interrupt

		SCI1C3 = 0x0f;		//enable error interrupt


	}
}

/* \brief
	UARTA2 start
*/
void vUart1Start(List* pxRxList)
{
	pxUart1RxList = pxRxList;

	SCI1C2_TE = 1;		//start tx
	SCI1C2_RE = 1;		//start rx

	Uart1Counter =
	Uart1Length = 0;
}

/* \brief
	UARTA1 stop
*/
void vUart1Stop(void)
{
	SCI1C2_TE = 0;
	SCI1C2_RE = 0;

	Uart1Counter =
	Uart1Length = 0;
}

/* \brief
	start write
*/
BOOL blUart1Write(BYTE* pData, BYTE length)
{
	BYTE data=0;
	BYTE ListResult = 0;

	if(Uart1Counter != Uart1Length)
	{
		return FALSE;
	}

	DisableInterrupts;

	pUart1Buff = pData;
	Uart1Length = length;
	Uart1Counter = 0;
	Uart1TxTimeoutTimer = uartTX_TIMEOUT;

	if(SCI1S1_TDRE == 1)
	{
		SCI1C2_TIE = 1;				//start transmit, enable tx interrupt
		
		SCI1D = pUart1Buff[Uart1Counter++];
		
	}

	if(isHardwareInitCompleted)
	{
		EnableInterrupts;
	}

	return TRUE;
}

/* \brief
	is wirte busy
*/
BOOL isUart1WriteBusy(void)
{
	if(Uart1Counter != Uart1Length)
	{
		if(0 == Uart1TxTimeoutTimer)
		{
			++xErrorRecord.Uart1TxTimeoutError;
			
			Uart1Counter =
			Uart1Length = 0;
		}
		
		return TRUE;
	}
	else
	{
		Uart1TxTimeoutTimer = uartTX_TIMEOUT;
		
		return FALSE;
	}
}

/* \brief
	UARTA1 rx interrupt service
*/
__interrupt void INTUart1Tx(void)
{
	BYTE data = 0;
	
	(void)(SCI1S1 == 0);
	if(Uart1Counter != Uart1Length)
	{
		SCI1D = pUart1Buff[Uart1Counter++];
	}
	else
	{
		SCI1C2_TIE = 0;				//transmit end, disable tx interrupt
	}
}

/* \brief
	UARTA1 rx interrupt service
*/
__interrupt void INTUart1Rx(void)
{
	BYTE data = 0;
	
	(void)(SCI1S1 == 0);
	data = SCI1D;
	if(LIST_OVERFLOW_ERR == xListPushBackByte(pxUart1RxList, data))
	{
	}
}

/* \brief
	UARTA1 error interrupt service
*/
__interrupt void INTUart1Error(void)
{
	BYTE data = 0;
	
	(void)(SCI1S1 == 0);
	data = SCI1D;

	++xErrorRecord.Uart1Error;
}

/* \brief
	UARTA2 initialization
*/
void vUart2Init(void)
{
	SCI2C2 = 0x00;		//disable sci before initialization

	SCI2BDH = 0x00;
	SCI2BDL = 0x09;		//BAUD = BUS / (16 * SCI2BD) = 16M /16/9 = 115200
	
	SCI2C1 = 0x00;

	SCI2C2 = 0x20;		//enable rx interrupt
	SCI2C3 = 0x0f;		//enable error interrupt
}

/* \brief
	UARTA2 start
*/
void vUart2Start(List* pxRxList)
{
	pxUart2RxList = pxRxList;

	SCI2C2_TE = 1;		//start tx
	SCI2C2_RE = 1;		//start rx

	Uart2Counter =
	Uart2Length = 0;
}

/* \brief
	UARTA2 stop
*/
void vUart2Stop(void)
{
	SCI2C2_TE = 0;
	SCI2C2_RE = 0;

	Uart2Counter =
	Uart2Length = 0;
}

/* \brief
	start write
*/
BOOL blUart2Write(BYTE* pData, BYTE length)
{
	BYTE data=0;
	BYTE ListResult = 0;

	if(Uart2Counter != Uart2Length)
	{
		return FALSE;
	}

	DisableInterrupts;
	
	pUart2Buff = pData;
	Uart2Length = length;
	Uart2Counter = 0;
	Uart2TxTimeoutTimer = uartTX_TIMEOUT;

	if(SCI2S1_TDRE == 1)
	{
		SCI2C2_TIE = 1;				//start transmit, enable tx interrupt
		
		SCI2D = pUart2Buff[Uart2Counter++];
		
	}
	if(isHardwareInitCompleted)
	{
		EnableInterrupts;
	}

	return TRUE;
}

/* \brief
	is wirte busy
*/
BOOL isUart2WriteBusy(void)
{
	if(Uart2Counter != Uart2Length)
	{
		if(0 == Uart2TxTimeoutTimer)
		{
			++xErrorRecord.Uart2TxTimeoutError;
			
			Uart2Counter =
			Uart2Length = 0;
		}
		
		return TRUE;
	}
	else
	{
		Uart2TxTimeoutTimer = uartTX_TIMEOUT;
		
		return FALSE;
	}
}

/* \brief
	UARTA2 rx interrupt service
*/
__interrupt void INTUart2Tx(void)
{
	BYTE data = 0;
	
	(void)(SCI2S1 == 0);
	if(Uart2Counter != Uart2Length)
	{
		SCI2D = pUart2Buff[Uart2Counter++];
	}
	else
	{
		SCI2C2_TIE = 0;				//transmit end, disable tx interrupt
	}
}

/* \brief
	UARTA2 rx interrupt service
*/
__interrupt void INTUart2Rx(void)
{
	BYTE data = 0;
	
	(void)(SCI2S1 == 0);
	data = SCI2D;
	if(LIST_OVERFLOW_ERR == xListPushBackByte(pxUart2RxList, data))
	{
		++xErrorRecord.ApuDrvReadBuffOverflowError;
	}
}

/* \brief
	UARTA2 error interrupt service
*/
__interrupt void INTUart2Error(void)
{
	BYTE data = 0;
	
	(void)(SCI2S1 == 0);
	data = SCI2D;

	++xErrorRecord.Uart2Error;
}

/* \brief
*/
void vUartTimer(void)
{
	if(0 != Uart1TxTimeoutTimer)
	{
		--Uart1TxTimeoutTimer;
	}

	if(0 != Uart2TxTimeoutTimer)
	{
		--Uart2TxTimeoutTimer;
	}
}

