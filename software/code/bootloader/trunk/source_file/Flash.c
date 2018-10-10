/**********************************************************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		Flash.c
**	Abstract:		
**	Creat By:		Zeno Liu
**	Creat Time:		2015.04.13
**	Version:		v1.0
**
**********************************************************************************************************************/

/**********************************************************************************************************************
**	Include Files
**********************************************************************************************************************/
#include "Flash.h"
#include <MC9S08DZ60.h>

#pragma MESSAGE DISABLE C1805

/**********************************************************************************************************************
**
**	DECLARATION
**
**********************************************************************************************************************/

/**********************************************************************************************************************
**	MacDefine
**********************************************************************************************************************/
#define	FLASH_FUNC_RAM_SIZE				200

/**********************************************************************************************************************
**	enum
**********************************************************************************************************************/
typedef enum
{
	FFCMD_IDLE,
	FFCMD_BLANK_CHECK					= 0x05,
	FFCMD_BYTE_PROGRAM					= 0x20,
	FFCMD_BURST_PROGRAM					= 0x25,
	FFCMD_SECTOR_ERASE					= 0x40,
	FFCMD_MASS_ERASE					= 0x41,
	FFCMD_SECTOR_ERASE_ABORT			= 0x47,
}FlashFCMD;

/**********************************************************************************************************************
**	struct
**********************************************************************************************************************/
typedef struct
{
	FlashFCMD f_fcmd;
	UINT08 ram[FLASH_FUNC_RAM_SIZE];
}FlashFuncRam;

/**********************************************************************************************************************
**	struct
**********************************************************************************************************************/
typedef struct
{
	FlashFuncRam func_ram;
}FlashComParaArea;

/**********************************************************************************************************************
**	function declaration
**********************************************************************************************************************/
typedef	BOOL (*FlashBurstProgramInRamType)(UINT16, UINT08*, UINT08);
typedef	BOOL (*FlashEraseProgramInRamType)(UINT16, UINT08, UINT08);

#define	FlashBurstProgramInRam(addr, pdata, length)	((FlashBurstProgramInRamType)flash_para.func_ram.ram)(addr, pdata, length)
#define	FlashEraseProgramInRam(addr, data, fcmd)	((FlashEraseProgramInRamType)flash_para.func_ram.ram)(addr, data, fcmd)

static void FlashFuncCopyToRam(UINT08* p_dst, UINT08* p_func);
static BOOL FlashEraseProgramFunc(UINT16 addr, UINT08 data, UINT08 fcmd);
static BOOL FlashBurstProgramFunc(UINT16 addr, UINT08 *data, UINT08 length);


/**********************************************************************************************************************
**
**	DEFINE
**
**********************************************************************************************************************/

/**********************************************************************************************************************
**	variable define
**********************************************************************************************************************/
static FlashComParaArea flash_para;

/**********************************************************************************************************************
**	Func Name:		void FlashInit(void)
**	Parameters:		void
**	Return Value:	void
**	Abstract:		flash init
**********************************************************************************************************************/
void FlashInit(void)
{
	FSTAT = 0x30;                        //Clear error flags
	FCDIV = 0x49;                        //Set clock divider,200KHz
	
	flash_para.func_ram.f_fcmd = FFCMD_IDLE;
	
	return;
}

/**********************************************************************************************************************
**	Func Name:		BOOL FlashSectorErase(UINT16 addr)
**	Parameters:		UINT16 addr
**	Return Value:	BOOL
**	Abstract:		flash sector erase
**********************************************************************************************************************/
BOOL FlashSectorErase(UINT16 addr)
{
	switch(flash_para.func_ram.f_fcmd)
	{
		default:
			FlashFuncCopyToRam(flash_para.func_ram.ram, (UINT08*)FlashEraseProgramFunc);
			break;
			
		case FFCMD_SECTOR_ERASE:
			break;
	}
	
	flash_para.func_ram.f_fcmd = FFCMD_SECTOR_ERASE;
	
	return FlashEraseProgramInRam(addr, 0xff, FFCMD_SECTOR_ERASE);
}

/**********************************************************************************************************************
**	Func Name:		static BOOL FlashBurstProgram(UINT16 addr, UINT08 *data, UINT08 length)
**	Parameters:		UINT16 addr, UINT08 *data, UINT08 length
**	Return Value:	BOOL
**	Abstract:		flash burst program in RAM
**********************************************************************************************************************/
BOOL FlashBurstProgram(UINT16 addr, UINT08 *data, UINT08 length)
{
	switch(flash_para.func_ram.f_fcmd)
	{
		case FFCMD_BURST_PROGRAM:
			break;
			
		default:
			FlashFuncCopyToRam(flash_para.func_ram.ram, (UINT08*)FlashBurstProgramFunc);
			break;
	}
	
	flash_para.func_ram.f_fcmd = FFCMD_BURST_PROGRAM;
	
	return FlashBurstProgramInRam(addr, data, length);
}

/**********************************************************************************************************************
**	Func Name:		static void FlashFuncCopyToRam(UINT08* p_dst, UINT08* p_func)
**	Parameters:		UINT08* p_dst, UINT08* p_func
**	Return Value:	void
**	Abstract:		copy func to RAM
**********************************************************************************************************************/
static void FlashFuncCopyToRam(UINT08* p_dst, UINT08* p_func)
{
	UINT08 i = 0;
	
	for(; i != FLASH_FUNC_RAM_SIZE; ++i)
	{
		*p_dst++ = *p_func++;

		__RESET_WATCHDOG();
	}
	
	return;
}

/**********************************************************************************************************************
**	Func Name:		static BOOL FlashEraseProgramFunc(UINT16 addr, UINT08 data, UINT08 fcmd)
**	Parameters:		UINT16 addr, UINT08 data, UINT08 fcmd
**	Return Value:	BOOL
**	Abstract:		flash erase or program
**********************************************************************************************************************/
static BOOL FlashEraseProgramFunc(UINT16 addr, UINT08 data, UINT08 fcmd)
{
	if(FSTAT_FACCERR)
	{
		FSTAT_FACCERR = 1;
	}
	
	(*(volatile UINT08*)addr) = data;
	
	FCMD = fcmd;
	FSTAT_FCBEF = 1;
	
	__asm
	{
		nop;
		nop;
		nop;
		nop;
	};
	
	while(!FSTAT_FCCF)
	{
		__RESET_WATCHDOG();
	}
	
	if(FSTAT & 0x30)			//Check FSTAT_FPVIOL and FSTAT_FACCERR
	{
		return FALSE;
	}
	
	return TRUE;
}

/**********************************************************************************************************************
**	Func Name:		static BOOL FlashBurstProgramFunc(UINT16 addr, UINT08 *data, UINT08 length)
**	Parameters:		UINT16 addr, UINT08 *data, UINT08 length
**	Return Value:	BOOL
**	Abstract:		flash burst program
**********************************************************************************************************************/
static BOOL FlashBurstProgramFunc(UINT16 addr, UINT08 *data, UINT08 length)
{
	UINT08 i = 0;
	//check address and len
	
	if(FSTAT_FACCERR)
	{
		FSTAT_FACCERR = 1;
	}
	
	for(; i != length; ++i)
	{
		while(!FSTAT_FCBEF)
		{
			__RESET_WATCHDOG();
		}
				
		*((byte*)addr) = *(data + i);
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
			return FALSE;
		}
	}
	
	while(!FSTAT_FCCF)
	{
		__RESET_WATCHDOG();
	}
	
	return TRUE;
}
