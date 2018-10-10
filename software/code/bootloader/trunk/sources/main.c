/**********************************************************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		main.c
**	Abstract:		
**	Creat By:		Zeno Liu
**	Creat Time:		2015.04.13
**	Version:		v1.0
**
**********************************************************************************************************************/

#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "System.h"
#include "SoftTimer.h"
#include "MCUinit.h"
#include <MC9S08DZ60.h>

/**********************************************************************************************************************
**	pass word define
**********************************************************************************************************************/
//const UINT08 pass_word1[8] @0x1900 = {0xf3, 0xf3, 0x23, 0x45, 0x65, 0x3c, 0xb1, 0x01};
//const UINT08 pass_word[8] @0xF3B8 = {0xf3, 0xf3, 0x23, 0x45, 0x65, 0x3c, 0xb1, 0x01};
//const UINT08 original_pass_word[8] = {0xf3, 0xf3, 0x23, 0x45, 0x65, 0x3c, 0xb1, 0x01};
const UINT08 ecu_id[2] = {0xb1, 0x01};
extern BOOL isMcuUpdate;
BOOL isForceWrite = FALSE;

/**********************************************************************************************************************
**	main
**********************************************************************************************************************/
void main(void)
{
//	MCU_init();
	
	SystemInit();
	
	//disabe interrupts for vector redirection
	DisableInterrupts; /* disable interrupts */
	/* include your code here */
	
	for(;;)
	{
		__RESET_WATCHDOG(); /* feeds the dog */
		
		SoftTimerTask();
		
		SystemTask();
	} /* loop forever */
	/* please make sure that you never leave main */
}

/**********************************************************************************************************************
**	define
**********************************************************************************************************************/
#define	SYSTEM_FLASH_ADDR_MIN		0x1900
//#ifdef	__DEBUG__
#define	SYSTEM_FLASH_ADDR_MAX		0xEDB7
/*#else
#define	SYSTEM_FLASH_ADDR_MAX		0xF3B7
#endif*/

typedef void (*VectFunc)(void);

//#ifdef	__DEBUG__
volatile VectFunc UserResetFunc @0xEDFE;
/*#else
volatile VectFunc UserResetFunc @0xF3FE;
#endif*/

void _Startup(void);
void PrepareStartup(void);

#pragma CODE_SEG  MY_SEGMENT
void FlashWriteStartup(void);
#pragma CODE_SEG  DEFAULT

/**********************************************************************************************************************
**	Func Name:		void PrepareStartup(void)
**	Parameters:		void
**	Return Value:	void
**	Abstract:		prepare startup for check power and user code
**********************************************************************************************************************/
void PrepareStartup(void)
{		//任何复位都进入烧写代码进行检测，如果3S内无任何操作并且有用户代码存在时再进入用户代码
/*	if(SRS_PIN)
	{	//force enter flash write
		isForceWrite = TRUE;
		main();
	}
	else
	{*/
		if((unsigned short)UserResetFunc < SYSTEM_FLASH_ADDR_MAX)
		{
			__RESET_WATCHDOG();
			
			UserResetFunc();
			FlashWriteStartup();
		}
		else
		{
			MCU_init();
			SystemHardwareInit();
			main();
		}
//	}
	
	return;
}

