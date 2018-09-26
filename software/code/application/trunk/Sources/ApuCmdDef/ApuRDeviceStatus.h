/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		ApuRDeviceStatus.h
**	Abstract:		APUR_GID_DEVICE_STATUS
**	Creat By:		Zeno Liu
**	Creat Time:		2015.05.11
**	Version:		v1.0
**
******************************************************************************/

#ifndef	__APU_R_DEVICE_STATUS_H__
#define	__APU_R_DEVICE_STATUS_H__

/* \brief
	APUR_GID_DEVICE_STATUS sub id
*/
typedef enum
{
	APUR_DEVICE_STATUS				=0x01,
	APUR_DEVICE_TYPE,
	APUR_DEVICE_INFO,
	APUR_DEVICE_STATUS_REQ
}enumApurDeviceStatus;

/* \brief
	APUR_DEVICE_STATUS
*/
typedef struct
{
	union{
		BYTE Byte;
		struct
		{
			unsigned isIPodConnected					:1;
			unsigned isUsbConnected						:1;
			unsigned isSdConnected						:1;
			unsigned Reserved							:1;
			unsigned isNaviConnected					:1;
			unsigned isInternetConnected				:1;
			unsigned isDTVConnected						:1;
			unsigned isHDRadio							:1;
		}Bits;
	}Status1;

	union{
		BYTE Byte;
		struct
		{
			unsigned BTState							:2;
			unsigned isWifiOpened						:1;
			unsigned isSiriusConnected					:1;
			unsigned isXmConnected						:1;
			unsigned isMhlConnected						:1;
			unsigned isMirrorLinkConnected				:1;
			unsigned Reserved							:1;
		}Bits;
	}Status2;
}ApurDeviceStatus;

#endif


