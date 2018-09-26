/**********************************************************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		LibConfig.h
**	Abstract:		
**	Creat By:		Zeno Liu
**	Creat Time:		2015.04.13
**	Version:		v1.0
**
**********************************************************************************************************************/

#ifndef	_LIB_CONFIG_H_
#define	_LIB_CONFIG_H_

/**********************************************************************************************************************
**	library config
**********************************************************************************************************************/

/**********************************************************************************************************************
**	system config
**********************************************************************************************************************/
#include "derivative.h"

/**********************************************************************************************************************
**	MacroDefine
**********************************************************************************************************************/
#ifndef	TURE
	#define	TRUE					1
#endif	//TRUE
#ifndef	FALSE
	#define	FALSE					0
#endif	//FALSE
#ifndef	NULL
	#define NULL					((void*)0)
#endif

/**********************************************************************************************************************
**	Typedef
**********************************************************************************************************************/
typedef	unsigned char				UINT08,BOOL,BYTE;
typedef unsigned short				UINT16,WORD;
typedef signed short				INT16;
typedef unsigned long				UINT32,DWORD;
typedef unsigned int				UINT;
typedef signed char					INT08;
typedef signed int					INT;

/****************Switch Debug/Release******************/
#define DEBUG_POWER_ON				FALSE

/*******************Switch Compiler********************/
#define	NEC_PM_PLUS					FALSE
#define	ARM7_ADS					FALSE
#define	FREESCALE_CODEWARRIOR		TRUE

#if NEC_PM_PLUS
	#define	MY_NOP					NOP()
#else	//NEC_PM_PLUS
//#if ARM7_ADS
	#define MY_NOP	    	        __asm "nop";
										//__asm{NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;
//										  NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;
//										  NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;}
//#endif	//ARM7_ADS
#endif	//NEC_PM_PLUS

/**********************************************************************************************************************
**	Switch Library Module
**********************************************************************************************************************/
//#define	LIB_SYS_AUDIO
//#define	LIB_SYS_RADIO
//#define	LIB_SYS_USB_SD
//#define	LIB_SYS_DISC
//#define	LIB_SYS_AUX
#define	LIB_SYS_COM
//#define	LIB_SYS_DISPLAY
//#define	LIB_SYS_IIC
//#define	LIB_SYS_CAN

/**********************************************************************************************************************
**	system config
**********************************************************************************************************************/
#define	WDT_RESET					//(WDTE = 0xAC)

//=========================system==============================
#define	POWER_OUTPUT				//(PTDDD_PTDDD2 = 1)

#define POWER_ON					//(PTDD_PTDD2 = 1)
#define POWER_OFF					//(PTDD_PTDD2 = 0)

#define	PIN_ACC_INPUT				{ClrDataBit(PINSEL1, (1 << 8) | (1 << 9));\
									 ClrDataBit(IO0DIR, 1 << 20);}
#define PIN_ACC_IN					(!((IO0PIN >> 20) & 0x00000001))

//=========================AUDIO==============================
#ifdef LIB_SYS_AUDIO
#include "LibAudioConfig.h"
#endif

//=========================RADIO==============================
#ifdef LIB_SYS_RADIO
#include "LibRadioConfig.h"
#endif

//=========================USB/SD=============================
#ifdef LIB_SYS_USB_SD
#include "LibUsbSdConfig.h"
#endif

//==========================CD================================
#ifdef LIB_SYS_DISC
#include "LibDISCConfig.h"
#endif

//==========================AUX===============================
#ifdef LIB_SYS_AUX
#include "LibAuxConfig.h"
#endif

//==========================UART==============================
#ifdef LIB_SYS_COM
#include "LibCOMConfig.h"
#endif

//=========================Display============================
#ifdef LIB_SYS_DISPLAY
#include "LibDisplayConfig.h"
#endif

//===========================IIC==============================
#ifdef LIB_SYS_IIC
#include "LibIICConfig.h"
#endif

//===========================CAN==============================
#ifdef LIB_SYS_CAN
#include "LibCANConfig.h"
#endif

/**********************************************************************************************************************
**	Disable warning Message
*********************************************************************************************************************/
#pragma	MESSAGE DISABLE	C1420				//Result of function-call is ignored
#pragma	MESSAGE DISABLE	C1855				//Recursive function call

#endif

