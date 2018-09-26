/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		ApuRSysInfo.h
**	Abstract:		APUR_GID_SYS_INFO
**	Creat By:		Zeno Liu
**	Creat Time:		2015.03.10
**	Version:		v1.0
**
******************************************************************************/

#ifndef	__APU_R_SYSINFO_H__
#define	__APU_R_SYSINFO_H__

/* \brief
	APUR_GID_SYS_INFO sub id
*/
typedef enum
{
	APUR_SYSINFO_STATUS				=0x01,
	APUR_SYSINFO_OS_TYPE
}enumApurSysInfo;

/* \brief
	APUR_SYSINFO_STATUS
*/
typedef enum
{
	APUR_STATUS_STARTING			=0x00,
	APUR_STATUS_MENU_ON,
	APUR_STATUS_OS_READY,
	APUR_STATUS_MAIN_MENU_ON,
	APUR_STATUS_MAIN_MENU_OFF,
	APUR_STATUS_SETUP_ON,
	APUR_STATUS_EQ_ON,
	APUR_STATUS_IPOD_VIDEO_ON,
	APUR_STATUS_POWER_OFF_READY,
	APUR_STATUS_IPOD_AUDIO_ANALOG,
	APUR_STATUS_IPOD_AUDIO_DIGITAL,
	APUR_STATUS_SETUP_PASS_OK
}enumApurSysStatus;

#endif


