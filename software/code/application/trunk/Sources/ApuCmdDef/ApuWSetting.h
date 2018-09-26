/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		ApuWSetting.h
**	Abstract:		APUW_GID_SETTING
**	Creat By:		Zeno Liu
**	Creat Time:		2015.03.10
**	Version:		v1.0
**
******************************************************************************/

#ifndef	__APU_W_SETTING_H__
#define	__APU_W_SETTING_H__

/* \brief
	include
*/

/* \brief
	APUW_GID_SETTING sub id
*/
typedef enum
{
	APUW_SETTING_SYS				=0x01,
	APUW_SETTING_GENERAL,
	APUW_SETTING_AUDIO,
	APUW_SETTING_ASP,
	APUW_SETTING_GEQ,
	APUW_SETTING_PEQ,
	APUW_SETTING_SCREEN,
	APUW_SETTING_6CH_VOL,
	APUW_SETTING_CH_GAIN
}enumApuwSetting;

/* \brief
	APUW_SETTING_GENERAL data
*/
typedef struct
{
	unsigned AutoOpenEnFlag			:1;
	unsigned ReverseMuteOn			:1;
	unsigned CameraMirrorFlag		:1;
	unsigned AuxInFormatFlag		:2;
	unsigned DownmixFlag			:1;
	unsigned RTC24HourFlag			:1;
	unsigned MotorExhibitFlag		:1;
}ApuSysSettingFlag1;

/* \brief
	APUW_SETTING_GENERAL data
*/
typedef struct
{
	unsigned RdsCtSet				:1;
	unsigned CanBrightSet			:1;
	unsigned BeepFlag				:2;
	unsigned ExtTelOnOffSet			:2;
	unsigned ExtTelChSet			:1;
	unsigned ActiveSoundSet			:1;
}ApuSysSettingFlag2;

/* \brief
	APUW_SETTING_GENERAL data
*/
typedef struct
{
	unsigned AuxInSwSet				:2;
	unsigned AccLedSet				:1;
	unsigned NaviGuidanceMode		:2;
	unsigned AutoDimmerSet			:1;
	unsigned AuxInSourceSet			:1;
	unsigned CameraOff				:1;
}ApuSysSettingFlag3;

/* \brief
	APUW_SETTING_GENERAL data
*/
typedef struct
{
	unsigned KeyColor				:3;
	unsigned AntennaSupply			:1;
	unsigned ParkingDetMode			:2;
	unsigned AntitheftSet			:1;
	unsigned Reserved				:1;
}ApuSysSettingFlag4;

/* \brief
	APUW_SETTING_GENERAL data
*/
typedef struct
{
	unsigned Reserved				:8;
}ApuSysSettingFlag5;

/* \brief
	APUW_SETTING_GENERAL data
*/
typedef struct
{
	ApuSysSettingFlag1 xApuSysSettingFlag1;
	BYTE RadioField;
	BYTE ClockHour;
	BYTE ClockMinute;
	ApuSysSettingFlag2 xApuSysSettingFlag2;
	ApuSysSettingFlag3 xApuSysSettingFlag3;
	BYTE CanBacklight;
	ApuSysSettingFlag4 xApuSysSettingFlag4;
	ApuSysSettingFlag5 xApuSysSettingFlag5;
	BYTE AntennaGain;
}ApuGeneralSetting;

/* \brief
	APUW_SETTING_GENERAL data
*/
typedef struct
{
	BYTE ScreenBrightness;
	BYTE ScreenTilt;
	BYTE ScreenNightBrightness;
}ApuScreenSetting;

/* \brief
	Apu Setting data
*/
typedef struct
{
	ApuGeneralSetting xApuGeneral;		// 10bytes
	ApuScreenSetting xApuScreen;		// 3bytes
}ApuSetting;

#endif


