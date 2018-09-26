/******************************************************************************
**
**								Copyright (C) 2017  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		AirCondition.c
**	Abstract:			head of air condition_CAN_361
**	Creat By:		neo HU
**	Creat Time:		2017.7.1
**	Version:		v1.0
**
******************************************************************************/

#ifndef	__AIR_CONDITION_H__
#define	__AIR_CONDITION_H__

/* \brief
	include
*/
#include "type.h"


/* \brief
*/
typedef enum
{
	AC_CMD_SWITCH,						//0:close
	AC_CMD_AC_SWITCH,					//0:off
	AC_CMD_AC_CYCLE_CTL,					//0:inside, 1:out, 2:auto
	AC_CMD_DUAL_SWITCH,					//dual off
	AC_CMD_FRONT_DEFROST,				//0:close
	AC_CMD_REAR_DEFROST,					//0:close
	AC_CMD_AUTO_SWITCH,					//0:auto off, 1:auto small, 2:auto big
	AC_CMD_FL_TEMP_ADJUST,
	AC_CMD_FL_TEMP_DIRECT,				//0:temp down
	AC_CMD_FR_TEMP_ADJUST,
	AC_CMD_FR_TEMP_DIRECT,				//0:temp down, 1:temp up
	AC_CMD_REAR_TEMP_ADJUST,
	AC_CMD_SET_FRONT_WIND_MODE,
	AC_CMD_SET_REAR_WIND_MODE,
	AC_CMD_F_WIND_LEV_ADJUST,			//00~0E: 0~14level
	AC_CMD_F_WIND_LEV_DIRECT,			//0:wind down
	AC_CMD_R_WIND_LEV_ADJUST,
	AC_CMD_R_WIND_LEV_DIRECT,
	AC_CMD_LEFT_SEAT_HEAT,				//0:OFF, 1~14level
	AC_CMD_RIGHT_SEAT_HEAT,
	AC_CMD_AC_ECO_MODE_SWITCH,			//0:close
	AC_CMD_SENS_SET,						//0:close, 1:Low sensitivity, 2:High sensitivity,
	AC_CMD_AIR_QUALITY_SWITCH,			//0:off
	AC_CMD_FILTER_POLLEN_SWITCH,			//0:off
	AC_CMD_R_WIND_SHIELD_HEAT,			//0:close
	AC_CMD_WIND_AREA_CTL,
	AC_CMD_FORCE_DEFROST,
	AC_CMD_AQS,
	AC_CMD_ION,
	AC_CMD_LOW,
	AC_CMD_HOT,
	AC_CMD_FL_TEMP_LEVEL
	
}enumAcApuCmd;

/* \brief
*/
typedef union
{
//	BYTE flag;
	struct
	{
		unsigned F_AirOnOff			:2;
		unsigned F_ACOnOff			:2;
		unsigned F_CycleStatus		:2;
		unsigned F_DualStatus		:2;
	}Bits;
}unionAirConditionStatus;

/* \brief
*/
typedef union
{
//	BYTE flag;
	struct
	{
		unsigned F_FrontDefrost		:2;
		unsigned F_RearDefrost		:2;
		unsigned F_AutoFlag			:2;
		unsigned F_Reserved			:2;
	}Bits;
}unionDefrostAutoStatus;

/* \brief
*/
typedef union
{
//	BYTE flag;
	struct
	{
		unsigned ForceDefrost			:2;
		unsigned AQS					:2;
		unsigned ION					:2;
	}Bits;
}unionACExternInfo3;

/* \brief
*/
typedef struct
{
	unionAirConditionStatus uAirConditionStatus;
	unionDefrostAutoStatus uDefrostAutoStatus;
	BYTE FrontLeftTemp;
	BYTE FrontRightTemp;
	BYTE RearTemp;
	BYTE OutsideTemp;
	BYTE WindMode;
	BYTE WindLevel;
	BYTE SeatHeating;
	BYTE AirConditionExternInfo1;
	BYTE AirConditionExternInfo2;
	unionACExternInfo3 uACExternInfo3;
	BYTE FrontLeftTempLev;
	BYTE DisplayState;
}AirConditionInfo;


extern AirConditionInfo xAirConditionInfo;

/* \brief
	function declaration
*/

void vAirInit(void);
void vAirTimer(void);
void vAirTask(void);
void vAirApuSet(BYTE* pData);

#endif


