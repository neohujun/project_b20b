/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		ApuRWGroupID.h
**	Abstract:		APU rw group id
**	Creat By:		Zeno Liu
**	Creat Time:		2015.03.10
**	Version:		v1.0
**
******************************************************************************/

#ifndef	__APU_RW_GROUP_ID_H__
#define	__APU_RW_GROUP_ID_H__

/* \brief
	MCU to APU Group id
*/
typedef enum
{
	APUW_GID_CMD					=0x10,
	APUW_GID_SETTING,
	APUW_GID_SYS_INFO,
	APUW_GID_RADIO_DATA,
	APUW_GID_TV_DATA,
	APUW_GID_CDC_DATA,
	APUW_GID_SWC_DATA,
	APUW_GID_CAN_INFO,
	APUW_GID_TPMS_DATA,
	APUW_GID_MCU_UPDATE,
	APUW_GID_CAN_UPDATE,
	APUW_GID_FCT_TEST,
	APUW_GID_CAN_BUS_DATA,
	APUW_GID_RECORD				=0x1f,
	APUW_GID_ELECTRIC_INFO		=0x20,
	APUW_GID_ERROR_RECORD		=0xE0,
	APUW_GID_BACK_KEY
}enumApuwGroupId;

/* \brief
	APU to MCU Group id
*/
typedef enum
{
	APUR_GID_CMD					=0xB0,
	APUR_GID_SYS_INFO,
	APUR_GID_SETTING,
	APUR_GID_TIME_DATA,
	APUR_GID_MEDIA_INFO,
	APUR_GID_CAN_CTL,
	APUR_GID_DEVICE_STATUS,
	APUR_GID_SWC_CTL,
	APUR_GID_TPMS_CTL,
	APUR_GID_MCU_UPDATE,
	APUR_GID_CAN_UPDATE,
	APUR_GID_FCT_TEST,
	APUR_GID_CAN_BUS_DATA,
	APUR_GID_RECORD				=0xbf,
	APUR_GID_DTV_CONTROL		=0xd0,
	APUR_GID_SWC_LEARN			=0xd1,
	APUR_GID_DVR_DATA			= 0xd2,
	APUR_GID_ERROR_RECORD		=0xF0,
	APUR_GID_BACK_KEY
}enumApurGroupId;

#endif


