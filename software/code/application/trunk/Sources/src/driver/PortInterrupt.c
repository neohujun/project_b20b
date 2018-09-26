/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		PortInterrupt.c
**	Abstract:		PortInterrupt
**	Creat By:		Zeno Liu
**	Creat Time:		2015.06.08
**	Version:		v1.0
**
******************************************************************************/

/* \brief
	include files
*/
#include "PortInterrupt.h"
#include "Config.h"
#include "Radar.h"

#include <MC9S08DZ60.h>

/* \brief
*/
static void vPortInterruptAccInit(void);
static void vPortInterruptBatInit(void);
static void vPortInterruptCmmInit(void);

/* \brief
*/
void vPortInterruptInit(void)
{

//	vPortInterruptAccInit();

	vPortInterruptBatInit();

}

/* \brief
*/
void vPortInterruptUartInit(void)
{
	PTDSC_PTDIE = 0;		//disalbe port A interrupt
	PTDES_PTDES2 = 0;	//falling edge
//	PTDSC &= ~0x01;		//edge only
	PTDPS_PTDPS2 = 1;	//PTDD2 select
//	PTDSC_PTDACK = 1;	//clear PTBIF
//	PTDSC_PTDIE = 1;
}

/* \brief
*/
void vPortInterruptUartStart(void)
{
	PTDPS_PTDPS2 = 1;
	PTDSC_PTDACK = 1;	//clear PTBIF
	PTDSC_PTDIE = 1;
}

/* \brief
*/
void vPortInterruptUartStop(void)
{
	PTDPS_PTDPS2 = 0;
	PTDSC_PTDACK = 1;	//clear PTBIF
	PTDSC_PTDIE = 0;
}

/* \brief
*/
__interrupt void INTPort(void)
{
	PTBSC |= 0x04;		//clear PTBIF
	PTBSC &= ~0x02;		//disable interrupt
}

/* \brief
*/
static void vPortInterruptAccInit(void)
{
	PTBSC_PTBIE = 0;			//disalbe port B interrupt
	PTBES_PTBES4 = 0;		//falling edge
	PTBPS_PTBPS4 = 0;		//PTDD5 select
	PTBSC_PTBACK = 1;		//clear PTBIF
}

/* \brief
*/
static void vPortInterruptBatInit(void)
{
	//PTBD2 PTBD4
	PTBSC &= ~0x02;		//disalbe port A interrupt
	PTBSC |= 0x04;		//clear PTBIF
	PTBES_PTBES2 = 1;
	PTBES_PTBES4 = 0;
	PTBES_PTBES0 = 0;
	PTBPS |= 0x15;		//
	PTBSC |= 0x04;		//clear PTBIF
}




