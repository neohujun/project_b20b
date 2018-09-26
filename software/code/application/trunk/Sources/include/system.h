/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		system.h
**	Abstract:		head of system
**	Creat By:		Zeno Liu
**	Creat Time:		2015.03.05
**	Version:		v1.0
**
******************************************************************************/

#ifndef	__SYSTEM_H__
#define	__SYSTEM_H__

/* \brief
*/
#include "type.h"
#include "Config.h"

/* \brief
*/
#include "MCU_APU_UICC_Def_V0.1.9.h"


/* \brief
*/
typedef enum
{
	SYSTEM_ACC_IDLE,
	SYSTEM_ACC_OFF_CHK,
	SYSTEM_ACC_OFF,
	SYSTEM_ACC_OFF_WAIT,
	SYSTEM_ACC_OFF_DONE,
	SYSTEM_ACC_ON,
	SYSTEM_ACC_ON_WAIT_POWER,
	SYSTEM_ACC_ON_WAIT,
	SYSTEM_ACC_ON_WAIT_MUTE_ON			//c7睡眠时因静待电流需要muteoff，所以在acc on时需要先mute on
}enumSystemAccState;


/* \brief
*/
typedef enum
{
	SYSTEM_STATUS_ACC_OFF,
	SYSTEM_STATUS_ACC_ON,
	SYSTEM_STATUS_CRK_ON						//汽车点火状态
}enumSystemAccStatus;

/* \brief
*/
typedef enum
{
	SYSTEM_START_NORMAL,
	SYSTEM_START_COLD
}enumSystemStartType;

/* \brief
*/
typedef union
{
	BYTE Byte;
	struct
	{
		unsigned AccStatus					:4;
		unsigned StartType					:4;
	}Bits;
}SystemPowerState;

/* \brief
*/
typedef enum
{
	SYSTEM_POWER_VOLTAGE_NORMAL,
	SYSTEM_POWER_VOLTAGE_LOW,
	SYSTEM_POWER_VOLTAGE_OVER
}enumSystemPowerVoltageState;


/* \brief
*/
extern BOOL isSystemInitCompleted;
extern SystemPowerState xSystemPowerState;
extern enumSystemPowerVoltageState eSystemPowerVoltageState;

/* \brief
	function declaration
*/
void vSystemInit(void);
void vSystemTask(void);
void vSystemTimer(void);
void vSystemQuickReverse(void);
void vSystemRestoreAllDefault(void);
void vSystemRestoreDefault(void);
void vSystemRestoreInit(void);
enumSystemAccState vSystemAccStatus(void);

#endif


