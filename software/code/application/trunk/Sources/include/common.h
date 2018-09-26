/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		common.h
**	Abstract:		head of common
**	Creat By:		Zeno Liu
**	Creat Time:		2015.03.10
**	Version:		v1.0
**
******************************************************************************/

#ifndef __COMMON_H__
#define __COMMON_H__

/* \brief
	include
*/
#include "type.h"
#include "MCU_APU_UICC_Def_V0.1.9.h"

/* \brief
*/
#define keySWC_VALUE_SECTION					7
#define keySWC_MAX_KEY_NUM					14


/* \brief
*/
typedef enum
{
	SIGNAL_OFF,
	SIGNAL_ON,
	SIGNAL_CHATTER
}enumSignalStatus;

/* \brief
*/
typedef enum
{
	HARDWARE_VER_1,
	HARDWARE_VER_2,
	HARDWARE_VER_3,
	HARDWARE_VER_4
}enumHardwareVer;

/* \brief
*/
typedef struct
{
	BYTE counter;				//no use

	BYTE OldSignal;			//user assignment
	BYTE NewSignal;			//user assignment

	BYTE OkSignal;			//user assignment or not
	BYTE FilterCount;			//user assignment or not
}SignalFilter;

/* \brief
*/
typedef struct
{
	BYTE counter;			//no use

	BYTE BaseValue;			//user assignment
	BYTE OldValue;			//user assignment
	BYTE NewValue;			//user assignment

	BYTE OkValue;			//user assignment or not
	BYTE FilterCount;			//user assignment or not
}KeyValueFilter;

/* \brief
*/
typedef struct
{
	BYTE Year;
	BYTE Month;
	BYTE Day;
}SystemDate;

/* \brief
*/
typedef struct
{
	BYTE Hour;
	BYTE Minute;
	BYTE Second;
}SystemWatchTime;

/* \brief
*/
extern enumHardwareVer eHardwareVer;
extern SystemDate xSystemDate;
extern SystemWatchTime xSystemWatchTime;

extern BOOL isHardwareInitCompleted;

/* \brief
	function declaration
*/
void vCommonInit(void);
void Delay(WORD wCounter);
BOOL blWaitSignalOk(BYTE* pSignal, BYTE SignalBit, BOOL OkSignal, WORD wCounter);
enumSignalStatus GetFilterSignal(SignalFilter* pxSignalFilter);
enumSignalStatus GetFilterValue(KeyValueFilter* pxSignalFilter);
void vResetMcuJumpBoot(void);
void vResetMcuJumpBgnd(void);

#endif
