/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		Pwm.c
**	Abstract:		Pwm initialization
**	Creat By:		Zeno Liu
**	Creat Time:		2015.08.03
**	Version:		v1.0
**
******************************************************************************/


/* \brief
	include files
*/
#include "Pwm.h"
#include "Debug.h"
#include "System.h"
#include "Common.h"
#include "Apu.h"
#include "io.h"
#include "Timer.h"
#include <MC9S08DZ60.h>

/* \brief
 */
#define	pwmMONITOR_BRIGHTNESS_MAX			20

/* \brief
	monitor brightness table
*/

WORD xTimerMonitorBrightnessTab[] = {
	30,
	40,
	50,
	65,
	80,
	100,
	125,
	150,
	175,
	200,
	250,
	300,
	350,
	400,
	450,
	500,
	600,
	700,
	800,
	900,
	timeMOD
};

/* \brief
*/
void vPwmInit(void)
{
	TPM2SC   = 0x00;
	TPM2CNT  = 0x00;

	TPM2MOD = pwmMONITOR_BRIGHTNESS_CYCLE;//0xc8;	

	TPM2C0SC = 0x28;		// 1x10 high true PWM   //(16/16M)*50 = 0.00005
	TPM2C0VH = 0x00;
	TPM2C0VL = 0x00;

	TPM1C4SC = 0x28;		// 1x10 high true PWM   //(16/16M)*50 = 0.00005
	TPM1C4VH = 0x00;
	TPM1C4VL = 0x00;

	TPM1C3SC = 0x28;		// 1x10 high true PWM   //(16/16M)*50 = 0.00005
	TPM1C3VH = 0x00;
	TPM1C3VL = 0x00;
	
}

/* \brief
*/
void vPwmStart(void)
{

	TPM2SC   = 0x0C;

	TPM2CNT  = 0x00;
}

/* \brief
*/
void vPwmMonitorControl(BOOL isOpen, BYTE Brightness)
{
//	if(xApuwDeviceStatus.ReverseFlag)
//	{
//		Brightness = pwmMONITOR_BRIGHTNESS_MAX;
//	}
	if(pwmMONITOR_BRIGHTNESS_MAX < Brightness)
	{
		Brightness = pwmMONITOR_BRIGHTNESS_MAX;
	}


	if(isOpen)
	{
		TPM1C4V = xTimerMonitorBrightnessTab[Brightness];
	}
	else
	{
		TPM1C4VH = 0x00;
		TPM1C4VL = 0x00;	
	}

	

}


/* \brief
*/
void vPwmLightControl(BOOL isOpen, WORD Brightness)
{
//	if(xApuwDeviceStatus.ReverseFlag)
//	{
//		Brightness = pwmMONITOR_BRIGHTNESS_MAX;
//	}

	if(isOpen)
	{
		TPM1C3V = Brightness;
	}
	else
	{
		TPM1C3VH = 0x00;
		TPM1C3VL = 0x00;	
	}

}

/* \brief
*/
void vPwmSpeedControl(BOOL isOpen, WORD speed)
{
	if(isOpen)
	{
		TPM2MOD = (25000/speed)*100;
		TPM2C0V = TPM2MOD/2;
	}
	else
	{
		TPM2C0VH = 0x00;
		TPM2C0VL = 0x00;	
	}
}

/* \brief
*/
void IOBLcdControl(BOOL open)
{
	if(open)
	{
		ioLCD_BL_ON;
//		ioTFT_RESET_H;
	}
	else
	{
		ioLCD_BL_OFF;
//		ioTFT_RESET_L;
	}
}

