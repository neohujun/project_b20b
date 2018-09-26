/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		Tpm.c
**	Abstract:		tpm initialization
**	Creat By:		Zeno Liu
**	Creat Time:		2015.12.30
**	Version:		v1.0
**
******************************************************************************/


/* \brief
	include files
*/
#include "Tpm.h"
#include "Timer.h"
#include "Common.h"
#include "io.h"

#include <MC9S08DZ60.h>

/* \brief
*/
#define	tpmPWM_FREQUENCY					5000
#define	tpmPWM_FREQUENCY_OFFSET			250		//tpmPWM_FREQUENCY*5%
#define	tpmPWM_FREQUENCY_HIGH			5500	//tpmPWM_FREQUENCY+tpmPWM_FREQUENCY_OFFSET
#define	tpmPWM_FREQUENCY_LOW			4500	//tpmPWM_FREQUENCY-tpmPWM_FREQUENCY_OFFSET

/* \brief
*/
BYTE TpmLampLevel = tpmBRIGHTNESS_INIT;			//n%*100

static WORD wTpmTimerBak;
static WORD wTpmHighWidth;
static WORD wTpmLowWidth;

static SignalFilter xTpmDutyCycleFilter;
static BYTE TpmCheckDelayTimer;			//检测的间隔时间
static BYTE TpmCheckTimeoutTimer;		//检测没有pwm的超时时间
static BOOL isTpmChanged;

/* \brief
*/
static void vTpmStart(void);
static void vTpmStop(void);

/* \brief
*/
void vTpmInit(void)
{
	TPM1C5SC = 0;
	vTpmStart();

	xTpmDutyCycleFilter.FilterCount = 3;
}

/* \brief
*/
void vTpmTimer(void)
{
	if(0 != TpmCheckDelayTimer)
	{
		if(0 == --TpmCheckDelayTimer)
		{
			vTpmStart();
		}
	}
	else if(0 != TpmCheckTimeoutTimer)
	{
		if(0 == --TpmCheckTimeoutTimer)
		{
			vTpmStop();
		}
	}
	else
	{
		if(isTpmChanged)
		{
			TpmCheckDelayTimer = 10;
			isTpmChanged = FALSE;
			xTpmDutyCycleFilter.counter = xTpmDutyCycleFilter.FilterCount;			//复位滤波器计数，重新滤波
		}
		else
		{
			xTpmDutyCycleFilter.OldSignal = xTpmDutyCycleFilter.NewSignal;

//			if(ioLAMP_IN)
//			{
//				xTpmDutyCycleFilter.NewSignal = 10;
//			}
//			else
//			{
//				xTpmDutyCycleFilter.NewSignal = 11;
//			}

			xTpmDutyCycleFilter.OkSignal = xTpmDutyCycleFilter.NewSignal;

			if(SIGNAL_ON == GetFilterSignal(&xTpmDutyCycleFilter))
			{
				TpmLampLevel = (xTpmDutyCycleFilter.OkSignal-1)*2;

				TpmCheckDelayTimer = 10;
			}
		}
	}
}

/* \brief
*/
__interrupt void vINTTpm1Ch5(void)
{
	TPM1C5SC_CH5F = 0;

	if(0 != TpmCheckDelayTimer)
	{
		return;
	}

	isTpmChanged = TRUE;

	if(TPM1C5SC_ELS5A == 1)
	{
		TPM1C5SC_ELS5A = 0;
		TPM1C5SC_ELS5B = 1;
		
		if(TPM1C5V > wTpmTimerBak)
		{
			wTpmHighWidth = TPM1C5V - wTpmTimerBak;
		}
		else
		{
			wTpmHighWidth = TPM1C5V + timeMOD - wTpmTimerBak;
		}
	}
	else
	{
		WORD wTpmCycle;
		
		TPM1C5SC_ELS5A = 1;
		TPM1C5SC_ELS5B = 0;
		
		if(TPM1C5V > wTpmTimerBak)
		{
			wTpmLowWidth = TPM1C5V - wTpmTimerBak;
		}
		else
		{
			wTpmLowWidth = TPM1C5V + timeMOD - wTpmTimerBak;
		}

		wTpmCycle = wTpmHighWidth+wTpmLowWidth;

		if((wTpmCycle >= tpmPWM_FREQUENCY_LOW) && (wTpmCycle <= tpmPWM_FREQUENCY_HIGH))
		{
			xTpmDutyCycleFilter.OldSignal = xTpmDutyCycleFilter.NewSignal;
			xTpmDutyCycleFilter.NewSignal = (BYTE)((wTpmHighWidth+tpmPWM_FREQUENCY_OFFSET)*10/(wTpmCycle));

			xTpmDutyCycleFilter.OkSignal = xTpmDutyCycleFilter.NewSignal;

			if(SIGNAL_ON == GetFilterSignal(&xTpmDutyCycleFilter))
			{
				if((xTpmDutyCycleFilter.NewSignal >= 1) && (xTpmDutyCycleFilter.NewSignal <= 9))
				{
					TpmLampLevel = (xTpmDutyCycleFilter.OkSignal - 1)*2;
				}

				TpmCheckDelayTimer = 10;
				vTpmStop();
			}
		}
	}

	wTpmTimerBak = TPM1C5V;
}

/* \brief
*/
static void vTpmStart(void)
{
	TPM1C5SC = 0x48;			//下降沿捕捉并使能

	TpmCheckTimeoutTimer = 10;
	isTpmChanged = FALSE;

	xTpmDutyCycleFilter.counter = xTpmDutyCycleFilter.FilterCount;			//复位滤波器计数，重新滤波
}

/* \brief
*/
static void vTpmStop(void)
{
	TPM1C5SC = 0;
	
	xTpmDutyCycleFilter.counter = xTpmDutyCycleFilter.FilterCount;			//复位滤波器计数，重新滤波
}

