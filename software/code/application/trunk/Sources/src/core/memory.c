/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		memory.c
**	Abstract:		memory handle
**	Creat By:		Zeno Liu
**	Creat Time:		2015.04.07
**	Version:		v1.0
**
******************************************************************************/

/* \brief
	include files
*/
#include "memory.h"
#include "Apu.h"
#include "eeprom.h"
#include "common.h"

#include "Error.h"
#include "Key.h"

#include "iic.h"
#include "System.h"

/* \brief
*/
#define	memoryINVALID_VALUE			0xff
#define	memoryINIT_VALUE				0x00

/* \brief
*/
static void vMemoryVarialbeRangeCheck(void);
static void vMemorySystemInit(void);
static void vMemorySettingRangeCheck(void);
static void vMemoryDeviceStatusRangeCheck(void);

/* \brief
*/
void vMemoryAllRead(void)
{
	BYTE isMemoryValid = memoryINVALID;
	
	
	if(memoryVALID == isMemoryValid)
	{
		(void)xEepromReadNByte(memoryADDR_APU_SETTING, (BYTE*)&xApuSetting, sizeof(xApuSetting));
		(void)xEepromReadNByte(memoryADDR_APU_DEVICE_STATUS, (BYTE*)&xApuwDeviceStatus, sizeof(xApuwDeviceStatus));

		isMemoryValid = memoryINVALID;

	}

//	vMemoryVarialbeRangeCheck();
	
	vMemorySystemInit();
}

/* \brief
*/
static void vMemoryVarialbeRangeCheck(void)
{
	vMemorySettingRangeCheck();
	vMemoryDeviceStatusRangeCheck();

/*
	if(24 <= xSystemWatchTime.Hour)
	{
		xSystemWatchTime.Hour = 0;

		++xErrorRecord.MemoryError;
	}
	if(60 <= xSystemWatchTime.Minute)
	{
		xSystemWatchTime.Minute = 0;

		++xErrorRecord.MemoryError;
	}
	if(60 <= xSystemWatchTime.Second)
	{
		xSystemWatchTime.Second = 0;

		++xErrorRecord.MemoryError;
	}
*/
}

/* \brief
*/
static void vMemorySystemInit(void)
{
	
//	vAudioPowerOnVolumeReset();
	
	vApuWriteBuffClear();
}

/* \brief
*/
static void vMemorySettingRangeCheck(void)
{
	BYTE i=0;
	BYTE* pTemp=NULL;
	
	//xApuSetting.xApuGeneral
	pTemp = (BYTE*)&xApuSetting.xApuGeneral.xApuSysSettingFlag1;
	if(memoryINVALID_VALUE == (*pTemp))
	{
		(*pTemp) = memoryINIT_VALUE;
		++xErrorRecord.MemoryError;
	}


	pTemp = &xApuSetting.xApuGeneral.ClockHour;
	for(i=0; i!=8; ++i,++pTemp)
	{
		if(memoryINVALID_VALUE == (*pTemp))
		{
			(*pTemp) = memoryINIT_VALUE;

			++xErrorRecord.MemoryError;
		}
	}

	//xApuSetting.xAudio
	for(i=0; i!=4; ++i,++pTemp)
	{
		if(memoryINVALID_VALUE == (*pTemp))
		{
			(*pTemp) = memoryINIT_VALUE;

			++xErrorRecord.MemoryError;
		}
	}

	pTemp = &xApuSetting.xApuScreen.ScreenTilt;
	for(i=0; i!=34; ++i,++pTemp)
	{
		if(memoryINVALID_VALUE == (*pTemp))
		{
			(*pTemp) = memoryINIT_VALUE;

			++xErrorRecord.MemoryError;
		}
	}
}

/* \brief
*/
static void vMemoryDeviceStatusRangeCheck(void)
{
	BYTE i=0;
	BYTE* pTemp=NULL;
	
	pTemp = (BYTE*)&xApuwDeviceStatus;
	for(i=0; i!=sizeof(xApuwDeviceStatus); ++i,++pTemp)
	{
		if(memoryINVALID_VALUE == (*pTemp))
		{
			(*pTemp) = memoryINIT_VALUE;

			++xErrorRecord.MemoryError;
		}
	}
}


