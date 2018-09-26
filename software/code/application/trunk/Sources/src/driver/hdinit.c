/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		hdinit.h
**	Abstract:		hardware initialization
**	Creat By:		Zeno Liu
**	Creat Time:		2015.03.09
**	Version:		v1.0
**
******************************************************************************/

/* \brief
	include files
*/
#include "Config.h"
#include "hdinit.h"
#include "Timer.h"
#include "uart.h"
#include "ApuDriver.h"
#include "MCUinit.h"
#include "can.h"
#include "eeprom.h"
#include "System.h"
#include "io.h"
#include "ad.h"
#include "iic.h"
#include "common.h"
#include "PortInterrupt.h"
#include "Pwm.h"
#include "Rtc.h"
#include "Radar.h"
#include "Tpm.h"

/* \brief
*/
static void HdinitApuPowerOn(void);
static void HardwareVersionDetect(void);

/* \brief
	system initialization
*/
void vHdInit(void)
{
	MCU_init();

//	==HW version
	ioHW_DET_VER_INIT;
//	==power
	ioACC_IN_INIT;
	ioBAT_DET_IN_INIT;
//	==BL
	ioLCD_BL_INIT;
//	ioTFT_RESET_INIT;
	
	ioTw88xx_RESET_INIT;
//	==REVERSE
	ioAPU_REV_IN_INIT;
	ioAPU_REV_POW_INIT;
//	==KEY
	ioKEY_TUNE_IN_INIT;
//	==CAN
	ioCAN_EN_INIT;
	ioCAN_STBY_INIT;
	ioCAN_INH_INIT;
//	==APU
	vApuDrvPortInit();
//	==USB
	ioUSB_OTG_POWER_INIT;
	ioUSB_HOST_POWER_INIT;

	ioODO_FWD_INIT;

	vEepromInit();
	
//	vTimerInit();

	vUart1Init(TRUE);
	vUart2Init();
	vADInit();
	vIICInit(0x00);
	vPortInterruptInit();

	Delay(5000);		//wait power on
	
	HdinitApuPowerOn();
//	HardwareVersionDetect();
//	if(HARDWARE_VER_4 == eHardwareVer)
//	{
		ioLIGHT_KEY_POWER_INIT_V4;
//	}
//	else
//	{
//		ioLIGHT_KEY_POWER_INIT;
//	}
	vPwmInit();
	vCanInit();
	
//	==AMP
	ioAUDIO_AMP_STBY_INIT;
	ioAUDIO_AMP_MUTE_INIT;

}
/* \brief
*/
static void HdinitApuPowerOn(void)
{

//	if((!ioACC_IN) || (!ioBAT_DET_IN))
//	{
//		return;
//	}


	//核心板上电要等待电源稳定，然后再上SYSTEM5V即GPS5V,防止GPS5V影响核心板上电时序
	ioAPU_POWER_ON;
	

	ioAPU_AWAKE;
	Delay(5000);			//10ms
//	ioAPU_RESET_HIGH;

	Delay(500);

	ioSYSTEM_POWER_INIT;

	Delay(5000);		//10ms

	//system 3.3v给倒车检测脚上拉
	vSystemQuickReverse();			//从上常电到倒车检测有40ms
}


