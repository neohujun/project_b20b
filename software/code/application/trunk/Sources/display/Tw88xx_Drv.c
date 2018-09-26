/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		Tw88xx_Drv.c
**	Abstract:		Tw88xx drver
**	Creat By:		neo Hu
**	Creat Time:		2017.03.14
**	Version:		v1.0
**
******************************************************************************/
/* \brief
	include files
*/
#include "Tw88xx_Drv.h"
#include "iic.h"
#include "Tw88xx_Para.h"
#include "io.h"
#include "system.h"
#include "Pwm.h"
#include "logo.h"
#include "Apu.h"
#include "Avm.h"

/* \brief
*/
#define	tw88xxIIC_ADDR_W			0x8A
#define	tw88xxIIC_ADDR_R			0x8B


/* \brief
	variable
*/
tw8836_register_step_enum VideoSubCtl;
static WORD wTw88xxDelayTimer;
static WORD wBLOpenDelayTimer;
static BOOL isDispInitComplete = FALSE;


/* \brief
*/
static void vTw88xxInitTask(void);
static void vTw88xxCommTask(void);
static void Tw8836_ChannelCtrlLvds(void);
static void Tw8836_ChannelCtrl360(void);
static void Tw8836_ChannelReCtrlLvds(void);


/* \brief
*/
void vTw88xx_DrvInit(void)
{
	isDispInitComplete = FALSE;
	VideoSubCtl = TW8836_INIT_STATE_WAIT;
	wTw88xxDelayTimer = 0;
	wBLOpenDelayTimer = 0;
	vIICAddrInit(tw88xxIIC_ADDR_W, tw88xxIIC_ADDR_R);
	vTwOsdInit();
}

/* \brief
*/
void vTw88xx_DrvTask(void)
{
	vTw88xxCommTask();
	vTw88xxInitTask();
}

/* \brief
*/
void vTw88xx_DrvDelay(void)
{
	if(0 != wTw88xxDelayTimer)
	{
		--wTw88xxDelayTimer;
	}

	vTwOsdDelay();
}

/* \brief
*/
static void vTw88xxCommTask(void)
{
	vTwOsdTask();
}

/* \brief
*/
static void vTw88xxInitTask(void)
{
	if((SYSTEM_ACC_IDLE != vSystemAccStatus()) || (!ioBAT_DET_IN))
	{
		return;
	}
	
	switch(VideoSubCtl)
	{
		case TW8836_INIT_STATE_WAIT:
			ioTw88xx_RESET_L;
			wTw88xxDelayTimer = 50;
			VideoSubCtl = TW8836_INIT_STATE_HOLD;
			break;

		case TW8836_INIT_STATE_HOLD:
			if(0 != wTw88xxDelayTimer)
			{
				break;
			}
			ioTw88xx_RESET_H;
			wTw88xxDelayTimer = 5;
			VideoSubCtl = TW8836_INIT_STATE_INIT;
			break;

		case TW8836_INIT_STATE_INIT:
			if(0 != wTw88xxDelayTimer)
			{
				break;
			}
			Tw8836_ChannelCtrlLvds();
			Delay(2);
			InitCore();
			InitSystem(LOGO_IMG_LVDS);
			isDispInitComplete = TRUE;
			VideoSubCtl = TW8836_INIT_STATE_WORK;
			break;

		case TW8836_INIT_STATE_BL:
			
			vPwmMonitorControl(TRUE, xApuSetting.xApuScreen.ScreenBrightness);
			VideoSubCtl = TW8836_INIT_STATE_WORK;
			break;
			
		case TW8836_INIT_STATE_WORK:
			break;

		case TW8836_INIT_STATE_LVDS:
			if(isAvmExistance)
			{
				Tw8836_ChannelCtrlLvds();
				Delay(2);
			}
			InitCore();
			InitSystem(LOGO_IMG_LVDS);
			
			wTw88xxDelayTimer = 300;
			VideoSubCtl = TW8836_INIT_STATE_LVDS_HOLD;
			break;

		case TW8836_INIT_STATE_LVDS_HOLD:

			if(ioAPU_HW_RESET_HIGH || (0 == wTw88xxDelayTimer))
			{
				VideoSubCtl = TW8836_INIT_STATE_BL;
			}
			break;

		case TW8836_INIT_STATE_360:
			Tw8836_ChannelCtrl360();
			Delay(2);
			SpiOsdEnable(OFF);
			vTwOsdStateSet(OSD_STATE_WORK);
			
			wTw88xxDelayTimer = 200;
			VideoSubCtl = TW8836_INIT_STATE_REVERSE_HOLD;
			break;

		case TW8836_INIT_STATE_REVERSE:
			InitSystem(LOGO_IMG_REV);

			wTw88xxDelayTimer = 800;
			VideoSubCtl = TW8836_INIT_STATE_REVERSE_HOLD;
			break;
			
		case TW8836_INIT_STATE_REVERSE_HOLD:
			if(ioAPU_HW_RESET_LOW || (0 == wTw88xxDelayTimer))
			{
				VideoSubCtl = TW8836_INIT_STATE_BL;
			}

			break;
			
		default:
			VideoSubCtl = TW8836_INIT_STATE_WORK;
			break;
	}
}

void Tw8836_SwitchChannel(tw8836_channel_enum Vch)
{
	WORD i;
	i = 0;

	switch(Vch)
	{
		case VCH_LVDS:
			VideoSubCtl = TW8836_INIT_STATE_LVDS;
			break;

		case VCH_360:
			VideoSubCtl = TW8836_INIT_STATE_360;
			break;
			
		case VCH_REVERSE:
			VideoSubCtl = TW8836_INIT_STATE_REVERSE;
			break;
			
		case VCH_DISPLAY_EN:
			break;	

		case VCH_ORIGINAL:
		default:
			break;
	}
}


static void Tw8836_ChannelCtrlLvds(void)
{
	WORD i;
	i = 0;

  	while(TRUE)
	{
		if((0xFF == Tw8836_LvdsIn_Para[i]) && (0xFE == Tw8836_LvdsIn_Para[i + 1]))
		{
			break;
		}
		tw8836I2CWrite(Tw8836_LvdsIn_Para[i],Tw8836_LvdsIn_Para[i + 1]);
		i += 2;
	}
}

void Tw8836_Reset(void)
{
	VideoSubCtl = TW8836_INIT_STATE_WAIT;
}

static void Tw8836_ChannelCtrl360(void)
{
	WORD i;
	i = 0;
	while(TRUE)
	{
		if((0xFF == Tw8836_360In_Para[i]) && (0xFE == Tw8836_360In_Para[i + 1]))
		{
			break;
		}
		tw8836I2CWrite(Tw8836_360In_Para[i],Tw8836_360In_Para[i + 1]);
		i += 2;
	}
}

/* \brief
*/
BOOL getDispInitComplete(void)
{
	return isDispInitComplete;
}

