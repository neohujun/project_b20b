/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		eeprom.c
**	Abstract:		eeprom driver
**	Creat By:		Zeno Liu
**	Creat Time:		2015.03.19
**	Version:		v1.0
**
******************************************************************************/

/* \brief
	include files
*/
#include "eeprom.h"
#include "common.h"
#include "error.h"

#include <hidef.h>

#include <MC9S08DZ60.h>

#define	eepromADDR_START			0x1400U
#define	eepromADDR_END				0x17FFU
/* \brief
*/
static enumEepromStatus xEepromErase(WORD wAddr);
static enumEepromStatus xEepromWriteBurst(WORD wAddr, BYTE* pData, WORD length);

/* \brief
*/
void vEepromInit(void)
{
	FSTAT = 0x30;	// clear FPVIOL¡¢FACCER

	FCNFG = 0x11;
	
	FCDIV = 0x49;	//DIVLD=0,PRDIV8=0,DIV5=0,DIV4=0,DIV3=1,DIV2=0,DIV1=0,DIV0=1
					//EEPROM clock = 16M / 8*£¨DIV+1) = 16M / 8*10 = 200k
}

/* \brief
*/
enumEepromStatus xEepromWriteByte(WORD wAddr, BYTE data)
{
	BYTE temp[8];
	WORD sector_addr;
	BYTE index = wAddr & 0x07;

	if((wAddr< eepromADDR_START) || (wAddr> eepromADDR_END))
	{
		++xErrorRecord.EepromOutofRangeError;
		return EEPROM_OUTOF_RANGE;
	}

	sector_addr = wAddr & 0xfff8;

	*((DWORD*)temp) = *((DWORD*)sector_addr);
	*(((DWORD*)temp) + 1) = *(((DWORD*)sector_addr) + 1);
	temp[index] = data;

	if (EEPROM_OK == xEepromErase(sector_addr))
	{
		if (EEPROM_OK == xEepromWriteBurst(sector_addr, temp, 8))
		{
			return EEPROM_OK;
		}
		else
		{
			++xErrorRecord.EepromError;
			return EEPROM_ERROR;
		}
	}
	else
	{
		++xErrorRecord.EepromError;
		return EEPROM_ERROR;
	}
}

/* \brief
*/
enumEepromStatus xEepromWriteNByte(WORD wAddr, BYTE* pData, WORD length)
{
	BYTE temp[8];
	WORD start_sector_addr, end_sector_addr;
	WORD end_addr = 0;
	WORD index = 0;

	if((wAddr < eepromADDR_START) || ((wAddr+ length-1) > eepromADDR_END))
	{
		++xErrorRecord.EepromOutofRangeError;
		return EEPROM_OUTOF_RANGE;
	}

	if((NULL == pData) || (0 == length))
	{
		++xErrorRecord.EepromError;
		return EEPROM_ERROR;
	}

	end_addr = wAddr + length;

	//start sector write
	index = wAddr & 0x07;
	start_sector_addr = wAddr & 0xfff8;
	
	if(index != 0)
	{
		*((DWORD*)temp) = *((DWORD*)start_sector_addr);
		*(((DWORD*)temp) + 1) = *(((DWORD*)start_sector_addr) + 1);

		for(; (index != 8) && (length != 0); ++index, --length, ++pData)
		{
			temp[index] = *pData;
		}

		if (EEPROM_OK == xEepromErase(start_sector_addr))
		{
			if (EEPROM_OK == xEepromWriteBurst(start_sector_addr, temp, 8))
			{
				if(length == 0)
				{
					return EEPROM_OK;
				}
				start_sector_addr += 8;
			}
			else
			{
				++xErrorRecord.EepromError;
				return EEPROM_ERROR;
			}
		}     
		else
		{
			++xErrorRecord.EepromError;
			return EEPROM_ERROR;
		}
	}

	//end sector write
	index = end_addr & 0x07;

	end_sector_addr = end_addr & 0xfff8;

	if(index != 0)
	{
		*((DWORD*)temp) = *((DWORD*)end_sector_addr);
		*(((DWORD*)temp) + 1) = *(((DWORD*)end_sector_addr) + 1);

		for(; (index != 0) && (length != 0); --index, --length)
		{
			temp[index - 1] = *(pData+ length - 1);
		}

		if (EEPROM_OK == xEepromErase(end_sector_addr))
		{
			if (EEPROM_OK == xEepromWriteBurst(end_sector_addr, temp, 8))
			{
				if(length == 0)
				{
					return EEPROM_OK; 
				}
			}
			else
			{
				++xErrorRecord.EepromError;
				return EEPROM_ERROR;
			}
		}     
		else
		{
			++xErrorRecord.EepromError;
			return EEPROM_ERROR;
		}
	}

	//middle sector write
	for(index = 0; index != length;)
	{
		if(EEPROM_OK != xEepromErase(start_sector_addr + index))
		{
			++xErrorRecord.EepromError;
			return EEPROM_ERROR;
		}
		index += 8;
	}

	if(EEPROM_OK == xEepromWriteBurst(start_sector_addr, pData, length))
	{
		return EEPROM_OK;
	}
	else
	{
		++xErrorRecord.EepromError;
		return EEPROM_ERROR;
	}
}

/* \brief
*/
enumEepromStatus xEepromReadNByte(WORD wAddr, BYTE* pData, WORD length)
{
	WORD i = 0;

	if((wAddr < eepromADDR_START) || ((wAddr+ length) > eepromADDR_END))
	{
		++xErrorRecord.EepromOutofRangeError;
		return EEPROM_OUTOF_RANGE;
	}

	if((NULL == pData) || (0 == length))
	{
		++xErrorRecord.EepromError;
		return EEPROM_ERROR;
	}

	for (i=0; i!=length; i++)
	{
		*pData= *((BYTE*)wAddr);

		wAddr++;
		pData++;
	}
	
	return EEPROM_OK;
}

/* \brief
*/
static enumEepromStatus xEepromErase(WORD wAddr)
{
	if((wAddr < eepromADDR_START) || (wAddr > eepromADDR_END))
	{
		++xErrorRecord.EepromOutofRangeError;
		return EEPROM_OUTOF_RANGE;
	}
	
	if (FSTAT & 0xB0)
	{
		FSTAT = 0xF0;
	}

	*(BYTE *)(wAddr) = 0xff;

	FCMD = 0x40;
	FSTAT = 0x80; 

	asm "nop";
	asm "nop";
	asm "nop";
	asm "nop";
	asm "nop";

	if(blWaitSignalOk(&FSTAT, 6, TRUE, 5000))
	{
		return EEPROM_OK;
	}
	else
	{
		++xErrorRecord.EepromError;
		return EEPROM_ERROR;
	}
}

/* \brief
*/
static enumEepromStatus xEepromWriteBurst(WORD wAddr, BYTE* pData, WORD length)
{
	WORD i = 0;
	
	//check address and len
	if((wAddr < eepromADDR_START) || (wAddr > eepromADDR_END))
	{
		++xErrorRecord.EepromOutofRangeError;
		return EEPROM_OUTOF_RANGE;
	}

	if((NULL == pData) || (0 == length))
	{
		++xErrorRecord.EepromError;
		return EEPROM_ERROR;
	}

	if(FSTAT_FACCERR)
	{
		FSTAT_FACCERR = 1;
	}

	DisableInterrupts;

	for(; i != length; ++i)
	{
		if(!blWaitSignalOk(&FSTAT, 7, TRUE, 200))
		{
			if(isHardwareInitCompleted)
			{
				EnableInterrupts;
			}
			
			++xErrorRecord.EepromError;
			return EEPROM_ERROR;
		}

		*((BYTE*)(wAddr + i)) = *(pData+ i);
		FCMD = 0x25;
		FSTAT_FCBEF = 1;
		__asm
		{
			nop;
			nop;
			nop;
			nop;
		}

		if(FSTAT_FPVIOL || FSTAT_FACCERR)
		{
			if(isHardwareInitCompleted)
			{
				EnableInterrupts;
			}
			
			++xErrorRecord.EepromError;
			return EEPROM_ERROR;
		}
	}

	if(blWaitSignalOk(&FSTAT, 6, TRUE, 500))
	{
		if(isHardwareInitCompleted)
		{
			EnableInterrupts;
		}
		
		return EEPROM_OK;
	}
	else
	{
		if(isHardwareInitCompleted)
		{
			EnableInterrupts;
		}
		
		++xErrorRecord.EepromError;
		return EEPROM_ERROR;
	}
}

