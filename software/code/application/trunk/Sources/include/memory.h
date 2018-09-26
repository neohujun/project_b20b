/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		memory.h
**	Abstract:		head of memory handle
**	Creat By:		Zeno Liu
**	Creat Time:		2015.04.07
**	Version:		v1.0
**
******************************************************************************/

#ifndef	__MEMORY_H__
#define	__MEMORY_H__

/* \brief
*/
#include "type.h"
#include "Config.h"

/* \brief
*/
#define	memoryVALID					0x55
#define	memoryINVALID				0xff

//====DIAG MEMORY
#define	memoryADDR_DIAG									0x1400
#define	memoryADDR_VIN									memoryADDR_DIAG
#define	memoryADDR_CONFIG								memoryADDR_DIAG+17//diagappECU_VIN_LENGTH
#define	memoryADDR_DTC_CONFIRM							memoryADDR_CONFIG+4//diagappCONFIG_LENGTH

//====APU
#define	memoryADDR_APU_SETTING							0x1500
#define	memoryADDR_APU_DEVICE_STATUS					(memoryADDR_APU_SETTING+sizeof(xApuSetting))


//====memory valid flag
#define	memoryADDR_VALID								(memoryADDR_SYSTEM_LANGUAGE+1)//sizeof(eAppSource))


/* \brief
*/
void vMemoryAllRead(void);

#endif

