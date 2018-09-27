/******************************************************************************
**
**								Copyright (C) 2017  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		Ems.c
**	Abstract:			ENGINE_IDLE_AND_VEHICLE_SPEED
**	Creat By:		neo HU
**	Creat Time:		2018.1.25
**	Version:		v1.0
**
******************************************************************************/

/* \brief
	include
*/
#include "Ems.h"
#include "Apu.h"
#include <MC9S08DZ60.h>
#include "Pwm.h"
#include "ApuWCmd.h"
#include "system.h"
#include "DiagAppLayer.h"
#include "io.h"

/* \brief
*/
static BYTE wEmsCycleTime;
static BYTE wEmsSpeedCnt;

/* \brief
*/
void vEmsInit(void)
{
	wEmsCycleTime = 0x00;
	wEmsSpeedCnt = 0x00;
}

/* \brief
*/
void vEmsTimer(void)
{
	if(0 != wEmsCycleTime)
	{
		--wEmsCycleTime;
	}
}

/* \brief
*/
void vCanEms3A0ReadHandle(BYTE* pxCanMessage)
{
	WORD speed = 0;
	if((SYSTEM_ACC_IDLE != vSystemAccStatus())  || (!ioBAT_DET_IN))
	{//state of starting or shutdown
		return;
	}
	if((!isSystemInitCompleted) || (0 != wEmsCycleTime) || isDiagAppIOSpeed())
	{
		return;
	}
	if(0 != (pxCanMessage[0] >> 2) & 0x03)
	{
		return;
	}
	speed = ((U16)(pxCanMessage[3] << 8) | pxCanMessage[4]);

	if(0 == speed)
	{
		wEmsSpeedCnt = 0;
	}
	
	if(++wEmsSpeedCnt > 4)
	{
		wEmsSpeedCnt = 0;
		vApuWrite(APUW_GID_CAN_INFO, APUW_CAN_SPEED, &pxCanMessage[3], 2);
	}
	vPwmSpeedControl(TRUE, speed);
	wEmsCycleTime = 20;
}



/* \brief
*/
BOOL blCanEmsSpeed(WORD wSpeed)
{
	if(!isSystemInitCompleted)
	{
		return FALSE;
	}
	
	vApuWrite(APUW_GID_CAN_INFO, APUW_CAN_SPEED, (BYTE*)&wSpeed, 2);
	vPwmSpeedControl(TRUE, wSpeed);

	return TRUE;
}



