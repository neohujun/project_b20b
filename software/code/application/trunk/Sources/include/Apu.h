/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		Apu.h
**	Abstract:		APU info handle
**	Creat By:		Zeno Liu
**	Creat Time:		2015.03.09
**	Version:		v1.0
**
******************************************************************************/

#ifndef	__APU_H__
#define	__APU_H__

/* \brief
	include
*/
#include "type.h"
#include "pwm.h"

/* \brief
	APU RW Group id
*/
#include "ApuRWGroupID.h"

/* \brief
	APUW_GID_CMD
*/
#include "ApuWCmd.h"

/* \brief
	APUW_GID_SETTING
*/
#include "ApuWSetting.h"

/* \brief
	APUR_GID_SYS_INFO
*/
#include "ApuRSysInfo.h"

/* \brief
	APUR_GID_SETTING
*/
#include "ApuRSetting.h"

/* \brief
	APUR_GID_DEVICE_STATUS
*/
#include "ApuRDeviceStatus.h"

/* \brief
	APUW_GID_SYS_INFO
*/
#include "ApuWSysInfo.h"

/* \brief
	APUR_GID_CAN_CTL
*/
typedef enum
{
	APUR_CAN_CTL_AIR_CONDITION_CMD		=0x01,
	APUR_CAN_CTL_CENTER_CTL_CMD,
	APUR_CAN_CTL_RADAR_CMD,
	APUR_CAN_CTL_RESERVE,
	APUR_CAN_CTL_COORDINATION,
	APUR_CAN_CTL_NAVI_INFO,
	APUR_CAN_CTL_AVM_INFO				=0x07,
	APUR_CAN_CTL_BCM_INFO				=0x08,
	APUR_CAN_CTL_PLG_INFO				=0x09,
	APUR_CAN_CTL_CONFIG_INFO			=0x0b,
}enumApuRCanCtl;

/* \brief
	APUW_GID_SYS_INFO
*/
typedef enum
{
	APUW_MCU_OS_UPDATE_IDEL				=0x00,
	APUW_MCU_OS_UPDATE_START,
	APUW_MCU_OS_UPDATE_END
}enumApuwMcuOsUpdate;

/* \brief
	APUW_GID_MCU_UPDATE
*/
typedef enum
{
	APUW_MCU_UPDATE_REQ_CMD			=0x01,
	APUW_MCU_UPDATE_CONFIRM,
	APUW_MCU_UPDATE_REQ_KEY,
	APUW_MCU_UPDATE_REQ_DATA,
	APUW_MCU_UPDATE_REQ_RESET
}enumApuwMcuUpdate;

/* \brief
	APUW_GID_CAN_UPDATE
*/
typedef enum
{
	APUW_CAN_UPDATE_CONFIRM				=0x01,
	APUW_CAN_UPDATE_REQ_KEY,
	APUW_CAN_UPDATE_REQ_DATA,
	APUW_CAN_UPDATE_RESULT
}enumApuwCanUpdate;

/* \brief
	APUW_GID_RECORD
*/
typedef enum
{
	APUW_RECORD_CMD						=0x01,
	APUW_RECORD_DATE,
	APUW_RECORD_TIME,
	APUW_RECORD_VERSION,
	APUW_RECORD_INFO,
	APUW_RECORD_FILE,
	APUW_RECORD_VIDEO_TIME,
	APUW_RECORD_STATUS
}enumApuwMcuUpdate;

/* \brief
*/
#define	apuMONITOR_BRIGHTNESS			pwmMONITOR_BRIGHTNESS_MIDDLE
#define	apuWATCHDOG_TIMER				2000//20180912 OS soft wdog timeout, avoid restart twice because of framwork hang up

/* \brief
*/
extern BOOL isMonitorOn;
extern ApuSetting xApuSetting;
extern ApuwDeviceStatus xApuwDeviceStatus;
extern ApurDeviceStatus xApurDeviceStatus;
extern WORD wApuCanUpdatePackageNum;
extern BOOL isApuCanUpdate;

/* \brief
	function declaration
*/
void vApuInit(void);
void vApuTask(void);
void vApuTimer(void);
void vApuTimerSecond(void);
void vApuTimerMinute(void);
void vApuWrite(BYTE GroupId, BYTE SubId, BYTE* pData, BYTE length);		//group id+sub id+data
BOOL isApuInit(void);
enumApuwMcuOsUpdate isApuUpdate(void);
void vApuWriteBuffClear(void);
void vApuWatchDogFeed(void);
void vApuWatchDogEnable(BOOL enable);

#endif


