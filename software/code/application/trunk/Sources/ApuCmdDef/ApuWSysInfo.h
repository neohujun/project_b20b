/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		ApuWSysInfo.h
**	Abstract:		APUW_GID_SYS_INFO
**	Creat By:		Zeno Liu
**	Creat Time:		2015.03.10
**	Version:		v1.0
**
******************************************************************************/

#ifndef	__APU_W_SYSINFO_H__
#define	__APU_W_SYSINFO_H__

/* \brief
	APUW_GID_SYS_INFO sub id
*/
typedef enum
{
	APUW_SYSINFO_VOLUME				=0x01,
	APUW_SYSINFO_SYSCLK,
	APUW_SYSINFO_VERSION,
	APUW_SYSINFO_DEVICE_STATUS,
	APUW_SYSINFO_CAN_ADAP_VERSION,
	APUW_SYSINFO_SYS_DEVICES_TYPE,
	APUW_SYSINFO_SYS_DEVICES_INFO,
	APUW_SYSINFO_VIN_CODE
}enumApuwSysInfo;

/* \brief
	APUW_SYSINFO_SYSCLK
*/
typedef enum
{
	APUW_SET_TIME					=0x00,
	APUW_SET_DATE					=0x01
}enumApuwSetTime;

/* \brief
	APUW_SYSINFO_SYSCLK
*/
typedef enum
{
	APUW_SHOW_NONE					=0x00,
	APUW_SHOW_MUSIC					=0x01,
	APUW_SHOW_MEIDA_TIME			=0x02,
	APUW_SHOW_RADIO					=0x03,
	APUW_SHOW_PHONE					=0x04,
	APUW_SHOW_CLEAR					=0x05,
	APUW_SHOW_NAVI					=0x10
}enumApuwShowMedia;

/* \brief
	APUW_SYSINFO_SYSCLK
*/
typedef enum
{
	APUW_NAVI_CLEAR					=0x00,
	APUW_NAVI_WORK					=0x01,
	APUW_NAVI_CLOSE					=0x02,
	APUW_NAVI_TEST					=0x03
}enumApuwNaviState;


/* \brief
	APUW_SYSINFO_DEVICE_STATUS
*/
typedef struct
{
	unsigned DTVConnected			:1;
	unsigned ATVConnected			:1;
	unsigned DiscStatus				:2;
	unsigned HDRadioConnected		:1;
	unsigned DABConnected			:1;
	unsigned TPMSConnected			:1;
	unsigned RadarConnected			:1;
	unsigned IllumiOn					:1;
	unsigned ReverseFlag				:1;
	unsigned CarParking				:1;
	unsigned JBLConnected			:1;
	unsigned VideoExist				:1;
	unsigned DownmixEn				:1;
	unsigned MuteOn					:1;
	unsigned LCDDisp					:1;
	unsigned AuxConnected			:1;
	unsigned Reserve1					:7;
	unsigned Reserve2					:8;
}ApuwDeviceStatus;

#endif


