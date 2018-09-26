/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		Key.h
**	Abstract:		head of key handle
**	Creat By:		Zeno Liu
**	Creat Time:		2015.04.09
**	Version:		v1.0
**
******************************************************************************/

#ifndef __KEY_H__
#define __KEY_H__

/* \brief
*/
#include "Config.h"
#include "type.h"

/* \brief
*/
#define	keyLONG_PRESSED								100
#define	keyLONG_L_PRESSED								200

/* \brief
*/
typedef enum
{
	KEY_EMPTY				= 0,
	KEY_MUTE,

	KEY_DISP				= 11,
	KEY_BACK,
	KEY_MEDIA,
	KEY_RADIO,
	
	KEY_TEL					= 21,
	KEY_DOWN,
	KEY_UP,
	KEY_VOLDN,
	KEY_VOLUP,
	KEY_VR,
	KEY_SRC,
	
	KEY_HOME				= 31,
	
	KEY_POWER				=KEY_MUTE+keyLONG_PRESSED,
	KEY_POWER_RST			=KEY_DISP+keyLONG_PRESSED,
	KEY_DOWN_L			=KEY_DOWN+keyLONG_PRESSED,
	KEY_UP_L				=KEY_UP+keyLONG_PRESSED
	
}enumKey;

/* \brief
*/

/* \brief
*/
void vKeyTask(void);
void vKeyInit(void);
void vKeyTimer(void);

#endif