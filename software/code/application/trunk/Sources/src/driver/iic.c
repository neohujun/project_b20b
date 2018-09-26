/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		iic.c
**	Abstract:		iic driver
**	Creat By:		Zeno Liu
**	Creat Time:		2015.03.10
**	Version:		v1.0
**
******************************************************************************/

/* \brief
	include files
*/
#include "iic.h"
#include "Error.h"
#include "common.h"
#include "Debug.h"
#include "memory.h"
#include "Apu.h"
#include "eeprom.h"
#include "io.h"
#include "System.h"

#include <MC9S08DZ60.h>
#include <hidef.h>

/* \brief
*/
static BYTE iicADDR_W = 0xFF;
static BYTE iicADDR_R = 0xFF;


/* \brief
*/
static void vIICMonitor(void);
static void vIICAwake(void);
static BOOL vIICStart(void);
static BOOL vIICSend8Bit(BYTE data);
static BOOL vIICRead8Bit(BYTE* value);

/* \brief
	IIC initialization
*/
void vIICInit(BYTE SelfAddr)
{
	IICA = SelfAddr;
	
	IICC1_IICEN = 0;
	IICF_MULT =  0x00;
	IICF_ICR = 0x0B;			// 0x1E :bps = 16M/(1 * 192) = 83.333 kHz  // 0x0B: 40      400KHz
//	SOPT1_IICPS = 1;			//select PTE4,PTE5
	
//	IICC1_IICIE = 1;
	IICC1_IICEN = 1;
}

/* \brief
	IIC initialization
*/
void vIICAddrInit(BYTE writeAddr, BYTE readAddr)
{
	iicADDR_W = writeAddr;
	iicADDR_R = readAddr;
}

/* \brief
	IIC timer
*/
void vIICTimer(void)
{
	vIICMonitor();
}

/* \brief
	IIC error monitor
*/
static void vIICMonitor(void)
{
//	if((0 == ioIIC_DATA) && IICS_BUSY)
//	{
//		IICMonitorTimer++;
//	} 
//	else
}

/* \brief
*/
static void vIICAwake(void)
{
	UINT08 i = 0;
	UINT08 clk = 0;

	IICF =  0x00;     
	IICC1 = 0x00;      

//	ioIIC_CLK_DIR = 1;		//设置为IO输出

	for(i = 0; i < 18; i++)		//输出9个CLK，解除SDA
	{
		if(0 == clk)
		{
//			ioIIC_CLK = 1;     
			Delay(1);
			clk = 1;
		} 
		else
		{
//			ioIIC_CLK = 0;     
			Delay(1);
			clk = 0;
		}
	}
}

/* \brief
	IIC start
*/
static BOOL vIICStart(void)
{
	IICC1_IICEN = 0;
	IICC1_IICEN = 1;			//reset iic
 
	IICC1_TXAK = 0;			//产生应答信号
	
	IICS_ARBL = 1;			//Clear any pending interrupt
	IICS_IICIF = 1;
	IICS_TCF = 1;

	IICS_SRW = 0;
	IICC1_TX = 1;
	IICC1_MST = 0;
	Delay(1);
	if(!blWaitSignalOk(&IICS, 5, 0, 20000))
	{
		++xErrorRecord.IICError;
		IICC1_MST = 0;
		return FALSE;
	}
	IICC1_MST = 1;
	Delay(1);
	
	return TRUE;
	
}

/* \brief
	IIC start
*/
static BOOL vIICSend8Bit(BYTE data)
{
//	IIC_temp = data;
	
	IICD = data;
	if(!blWaitSignalOk(&IICS, 1, 1, 20000))
	{
		++xErrorRecord.IICError;
		IICC1_MST = 0;
		return FALSE;
	}
	IICS_IICIF = 1;
	
	if(!blWaitSignalOk(&IICS, 0, 0, 20000))
	{
		++xErrorRecord.IICError;
		IICC1_MST = 0;
		return FALSE;
	}
	
	return TRUE;
}

/* \brief
	IIC start
*/
static BOOL vIICRead8Bit(BYTE* value)
{
	BYTE temp = 0xFF;
	
	IICC1_TX = 0;		//读使能

	IICC1_TXAK = 1;
	temp = IICD;			//虚读
    
	if(!blWaitSignalOk(&IICS, 1, 1, 20000))
	{
		++xErrorRecord.IICError;
		IICC1_MST = 0;
		return FALSE;
	}
	IICS_IICIF = 1;
	
	IICC1_MST = 0;
	Delay(1);
	
	*value = IICD;	//读取最后一个字节
	
	return TRUE;
}

/* \brief
	IIC read
*/
U8 ReadI2C (U8 index)
{
	U8 val;
	if(!vIICStart())
	{
		return FALSE;
	}

	if(!vIICSend8Bit(iicADDR_W))
	{
		return FALSE;
	}

	if(!vIICSend8Bit(index))
	{
		return FALSE;
	}

	IICC1_MST = 0;
	Delay(1);
	
	if(!vIICStart())
	{
		return FALSE;
	}

	if(!vIICSend8Bit(iicADDR_R))
	{
		return FALSE;
	}

	if(!vIICRead8Bit(&val))
	{
		return FALSE;
	}
	
	
	return val;
}

/* \brief
	IIC read
*/
BOOL blIICRead (BYTE index, BYTE* value)
{

	if(!vIICStart())
	{
		return FALSE;
	}

	if(!vIICSend8Bit(iicADDR_W))
	{
		return FALSE;
	}

	if(!vIICSend8Bit(index))
	{
		return FALSE;
	}

	IICC1_MST = 0;
	Delay(1);
	
	if(!vIICStart())
	{
		return FALSE;
	}

	if(!vIICSend8Bit(iicADDR_R))
	{
		return FALSE;
	}

	if(!vIICRead8Bit(value))
	{
		return FALSE;
	}
	
	
	return TRUE;
}

/* \brief
	IIC read
*/
BOOL blIICWrite (BYTE index, BYTE value)
{

	if(!vIICStart())
	{
		return FALSE;
	}

	if(!vIICSend8Bit(iicADDR_W))
	{
		return FALSE;
	}

	if(!vIICSend8Bit(index))
	{
		return FALSE;
	}

	if(!vIICSend8Bit(value))
	{
		return FALSE;
	}
	
	IICC1_MST = 0;
	Delay(1);
	
	return TRUE;
}


/* \brief
	IIC read
*/
void blIICReadByte (WORD index, BYTE* value)
{

	BYTE reg = 0;
	BYTE page;
	BYTE falseTemp = 0;

	page = (index >> 8) & 0xFF;
	reg = index & 0xFF;

	
	if(!blIICWrite(0xFF, page))
	{
		++falseTemp;
	}
	else
	{
		falseTemp = 0;
	}

	if(!blIICRead(reg, value))
	{
		++falseTemp;
	}

}

/* \brief
	IIC read
*/
void blIICWriteByte (WORD index, BYTE value)
{

	BYTE reg = 0;
	BYTE page;
	BYTE falseTemp = 0;

	page = (index >> 8) & 0xFF;
	reg = index & 0xFF;

	if(!blIICWrite(0xFF, page))
	{
		++falseTemp;
	}
	else
	{
		falseTemp = 0;
	}

	if(!blIICWrite(reg, value))
	{
		++falseTemp;
	}

	
}

void tw8836I2CWrite(BYTE index, BYTE value)
{
	BYTE falseTemp = 0;
	if(!blIICWrite(index, value))
	{
		++falseTemp;
	}

}


/* \brief
	IIC Write
*/
BOOL blIICWriteNByte(BYTE addr, BYTE* pData, BYTE length)
{
	if((pData == NULL) || (0 == length) || (1 == IICC1_MST))
	{
		return FALSE;
	}

	if(!vIICStart())
	{
		return FALSE;
	}

	if(!vIICSend8Bit(addr))
	{
		return FALSE;
	}

	for(; 0!=length; --length,++pData)
	{
		IICD = (*pData);

		if(!blWaitSignalOk(&IICS, 1, 1, 20000))
		{
			++xErrorRecord.IICError;
			IICC1_MST = 0;
			return FALSE;
		}
		IICS_IICIF = 1;

		if(!blWaitSignalOk(&IICS, 0, 0, 20000))
		{
			++xErrorRecord.IICError;
			IICC1_MST = 0;
			return FALSE;
		}

		__RESET_WATCHDOG();
	}
	IICC1_MST = 0;
	Delay(1);			//总线保持时间
	
	return TRUE;
}

void WriteTW88Page(U8 ucPage)
{
	WriteTW88(0xFF, ucPage);
}


void ReadTW88Page(U8 *ucPage)
{
	*ucPage = ReadTW88(0xFF);
}


