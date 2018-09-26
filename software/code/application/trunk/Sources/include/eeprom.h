/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		eeprom.h
**	Abstract:		head of eeprom driver
**	Creat By:		Zeno Liu
**	Creat Time:		2015.03.19
**	Version:		v1.0
**
******************************************************************************/

#ifndef	__EEPROM_H__
#define	__EEPROM_H__

/* \brief
*/
#include "type.h"

/* \brief
*/
typedef enum
{
	EEPROM_OK,
	EEPROM_ERROR,
	EEPROM_OUTOF_RANGE
}enumEepromStatus;

/* \brief
*/
void vEepromInit(void);
enumEepromStatus xEepromWriteByte(WORD wAddr, BYTE data);
enumEepromStatus xEepromWriteNByte(WORD wAddr, BYTE* pData, WORD length);
enumEepromStatus xEepromReadNByte(WORD wAddr, BYTE* pData, WORD length);

#endif


