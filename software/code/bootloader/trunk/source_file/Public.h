/**********************************************************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		Public.h
**	Abstract:		
**	Creat By:		Zeno Liu
**	Creat Time:		2015.04.13
**	Version:		v1.0
**
**********************************************************************************************************************/

#ifndef	_PUBLIC_H_
#define _PUBLIC_H_

/**********************************************************************************************************************
**	Include Files
**********************************************************************************************************************/
#include "LibConfig.h"

/**********************************************************************************************************************
**
**	IMPLEMENT
**
**********************************************************************************************************************/

/**********************************************************************************************************************
**	MacroDefine
**********************************************************************************************************************/
#ifndef	HIGH
	#define	HIGH					TRUE
#endif
#ifndef	LOW
	#define	LOW						FALSE
#endif

#define	NO_EXP						0
#define DAT_NUL						-1

#ifndef	Timer_40_MS
	#define Timer_40_MS				1
#endif
#ifndef	Timer_80_MS
	#define Timer_80_MS				2
#endif
#ifndef	Timer_120_MS
	#define Timer_120_MS			3
#endif
#ifndef	Timer_160_MS
	#define	Timer_160_MS			4
#endif
#ifndef Timer_200_MS
	#define Timer_200_MS			5
#endif
#ifndef	Timer_240_MS
	#define	Timer_240_MS			6
#endif
#ifndef	Timer_280_MS
	#define	Timer_280_MS			7
#endif
#ifndef	Timer_320_MS
	#define	Timer_320_MS			8
#endif
#ifndef	Timer_360_MS
	#define	Timer_360_MS			9
#endif
#ifndef Timer_400_MS
	#define	Timer_400_MS			10
#endif
#ifndef Timer_480_MS
	#define	Timer_480_MS			12
#endif
#ifndef	Timer_800_MS
	#define Timer_800_MS			20
#endif
#ifndef Timer_1000_MS
	#define Timer_1000_MS			25
#endif
#ifndef	Timer_1200_MS
	#define	Timer_1200_MS			30
#endif
#ifndef	Timer_1600_MS
	#define	Timer_1600_MS			40
#endif
#ifndef	Timer_2000_MS
	#define	Timer_2000_MS			50
#endif
#ifndef	Timer_3000_MS
	#define	Timer_3000_MS			75
#endif
#ifndef Timer_5000_MS
	#define Timer_5000_MS			125
#endif
#ifndef Timer_6000_MS
	#define Timer_6000_MS			150
#endif
#ifndef	Timer_10000_MS
	#define	Timer_10000_MS			250
#endif
#ifndef Timer_1_S
	#define Timer_1_S				1
#endif
#ifndef Timer_2_S
	#define Timer_2_S				2
#endif
#ifndef Timer_3_S
	#define Timer_3_S				3
#endif
#ifndef	Timer_4_S
	#define Timer_4_S				4
#endif
#ifndef Timer_5_S
	#define Timer_5_S				5
#endif
#ifndef	Timer_8_S
	#define	Timer_8_S				8
#endif
#ifndef	Timer_10_S
	#define	Timer_10_S				10
#endif
#ifndef	Timer_15_S
	#define	Timer_15_S				15
#endif
#ifndef	Timer_20_S
	#define	Timer_20_S				500
#endif
#ifndef	Timer_30_S
	#define	Timer_30_S				30
#endif
#ifndef	Timer_40_S
	#define	Timer_40_S				40
#endif
#ifndef	Timer_50_S
	#define	Timer_50_S				1250
#endif
#ifndef	Timer_60_S
	#define	Timer_60_S				60
#endif
#ifndef	Timer_80_S
	#define	Timer_80_S				80
#endif
#ifndef	Timer_100_S
	#define	Timer_100_S				100
#endif

#define	TIME_OUT_COUNTER			5000

/**********************************************************************************************************************
**
**	IMPLEMENT
**
**********************************************************************************************************************/

#ifdef	__DEBUG__
#include "Debug.h"
#else
#define	Printfpc(p_str,p_data,data_num)
#define	vAppTimer(TimerUnit)
#endif

#endif
