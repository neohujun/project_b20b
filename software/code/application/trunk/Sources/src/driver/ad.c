/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		ad.c
**	Abstract:		ad driver
**	Creat By:		Zeno Liu
**	Creat Time:		2015.04.09
**	Version:		v1.0
**
******************************************************************************/

/* \brief
	include
*/
#include "ad.h"
#include "common.h"
#include "Error.h"
#include <MC9S08DZ60.h>

/* \brief
*/
static const BYTE xADChannelTab[] =
{
	14,
	21,
	7,
	22,
	15,
	23
};

/* \brief
*/
static void vADStart(BYTE ADChannel);
static void vADConvertStart(BYTE ADChannel);

/* \brief
*/
void vADInit(void)
{
	SOPT2 &= (unsigned char) ~0x10;
	
	APCTL2 = 0x40;		//ADP14
	APCTL3 = 0x60;		//ADP21, AD22
	APCTL1 = 0x80;		//ADP7


	ADCCFG = 0x00;
	ADCSC2 = 0x00;
	ADCCV = 0x00;
	ADCSC1 = 0x1F;
}

/* \brief
*/
BYTE ADGetValue(BYTE channel)
{
	vADConvertStart(xADChannelTab[channel]);
	
	return (ADCRL);
}

/* \brief
*/
static void vADConvertStart(BYTE ADChannel)
{
	ADCSC1 = ADChannel& 0x1f;

	if(!blWaitSignalOk(&ADCSC1, 7, TRUE, 1000))
	{
		++xErrorRecord.ADConvertError;
	}
}

