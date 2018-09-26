/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		Display.h
**	Abstract:		head of display application
**	Creat By:		neo Hu
**	Creat Time:		2017.03.14
**	Version:		v1.0
**
******************************************************************************/

#ifndef	__Tw88xx_Drv_H__
#define	__Tw88xx_Drv_H__
/* \brief
	include
*/
#include "type.h"

/* \brief
*/
typedef enum
{
	 VCH_ORIGINAL = 0x00,
	 VCH_CVBS_PIP,
	 VCH_ARGB_PIP,
	 VCH_LVDS,
	 VCH_REVERSE,
	 VCH_360,
	 VCH_DISPLAY_EN,
}tw8836_channel_enum;

/* \brief
*/
typedef enum TW8836_REGISTER_STEP
{
	 TW8836_INIT_STATE_WAIT = 0x00,
	 TW8836_INIT_STATE_HOLD,
	 TW8836_INIT_STATE_INIT,
	 TW8836_INIT_STATE_BL,
	 TW8836_INIT_STATE_WORK,
	 TW8836_INIT_STATE_LVDS,
	 TW8836_INIT_STATE_LVDS_HOLD,
	 TW8836_INIT_STATE_REVERSE_HOLD,
	 TW8836_INIT_STATE_360,
	 TW8836_INIT_STATE_REVERSE
}tw8836_register_step_enum;

/* \brief
	function declaration
*/
extern tw8836_register_step_enum VideoSubCtl;
void vTw88xx_DrvInit(void);
void vTw88xx_DrvTask(void);
void vTw88xx_DrvDelay(void);
void Tw8836_SwitchChannel(tw8836_channel_enum Vch);
BOOL getDispInitComplete(void);
void Tw8836_Reset(void);
#endif

