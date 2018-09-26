/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		io.h
**	Abstract:		
**	Creat By:		Zeno Liu
**	Creat Time:		2015.03.10
**	Version:		v1.0
**
******************************************************************************/

#ifndef __IO_H__
#define __IO_H__

/* \brief
	include
*/
#include "Config.h"

#include <MC9S08DZ60.h>


/* \brief
	SYSTEM PORT
*/
#define	ioSYSTEM_POWER_INIT			{PTCDD_PTCDD3 = 1;\
											ioSYSTEM_POWER_ON;}
#define	ioSYSTEM_POWER_ON				(PTCD_PTCD3 = 1)
#define	ioSYSTEM_POWER_OFF			(PTCD_PTCD3 = 0)


#define	ioACC_IN_INIT					(PTBDD_PTBDD4 = 0)
#define	ioACC_IN						(PTBD_PTBD4 == 0)


#define	ioBAT_DET_IN_INIT				(PTBDD_PTBDD2 = 0)
#define	ioBAT_DET_IN					(PTBD_PTBD2 == 1)



/* \brief
	APU PORT
*/

#define	ioAPU_POWER_INIT				{PTADD_PTADD3 = 1;\
											ioAPU_POWER_OFF;}
#define	ioAPU_POWER_ON				(PTAD_PTAD3 = 1)
#define	ioAPU_POWER_OFF				(PTAD_PTAD3 = 0)
#define	ioAPU_POWER_IS_OFF			(PTAD_PTAD3 == 0)

#define	ioAPU_HW_RESET_INIT			(PTADD_PTADD0 = 0)
#define	ioAPU_HW_RESET_HIGH			(PTAD_PTAD0 == 1)
#define	ioAPU_HW_RESET_LOW			(PTAD_PTAD0 == 0)

#define	ioAPU_STATUS_IN_INIT			(PTCDD_PTCDD1 = 0)
#define	ioAPU_STATUS_AWAKE			(PTCD_PTCD1 == 1)
#define	ioAPU_STATUS_SLEEP				(PTCD_PTCD1 == 0)



#define	ioAPU_AWAKE_SLEEP_INIT		{PTCDD_PTCDD2 = 1;\
											ioAPU_AWAKE;}
#define	ioAPU_AWAKE					(PTCD_PTCD2 = 1)
#define	ioAPU_SLEEP						(PTCD_PTCD2 = 0)



/* \brief
	AUDIO PORT
*/

#define	ioAUDIO_AMP_STBY_INIT			{PTFDD_PTFDD5 = 1;\
											ioAUDIO_AMP_STBY_L;}
#define	ioAUDIO_AMP_STBY_H			(PTFD_PTFD5 = 1)
#define	ioAUDIO_AMP_STBY_L			(PTFD_PTFD5 = 0)

#define	ioAUDIO_AMP_STBY_ON			ioAUDIO_AMP_STBY_H
#define	ioAUDIO_AMP_STBY_OFF			ioAUDIO_AMP_STBY_L



#define	ioAUDIO_AMP_MUTE_INIT			{PTFDD_PTFDD4 = 1;\
											ioAUDIO_AMP_MUTE_L;}
#define	ioAUDIO_AMP_MUTE_H			(PTFD_PTFD4 = 1)
#define	ioAUDIO_AMP_MUTE_L			(PTFD_PTFD4 = 0)

#define	ioAUDIO_AMP_MUTE_ON			ioAUDIO_AMP_MUTE_L
#define	ioAUDIO_AMP_MUTE_OFF			ioAUDIO_AMP_MUTE_H


/* \brief
	display
*/
#define ioLCD_BL_INIT					{PTGDD_PTGDD4 = 1;\
											ioLCD_BL_OFF;}
#define ioLCD_BL_ON						PTGD_PTGD4 = 1;
#define ioLCD_BL_OFF						PTGD_PTGD4 = 0;

#define	ioTw88xx_RESET_INIT			{PTBDD_PTBDD3 = 1;\
											ioTw88xx_RESET_L;}
#define	ioTw88xx_RESET_H				(PTBD_PTBD3 = 1)
#define	ioTw88xx_RESET_L				(PTBD_PTBD3 = 0)

//#define	ioTFT_RESET_INIT				{PTBDD_PTBDD7 = 1;\
//											ioTFT_RESET_L;}
//#define	ioTFT_RESET_H					(PTBD_PTBD7 = 1)
//#define	ioTFT_RESET_L					(PTBD_PTBD7 = 0)

/* \brief
	USB Control
*/
#define	ioUSB_OTG_POWER_INIT			{PTGDD_PTGDD3 = 1;\
											 ioUSB_OTG_POWER_ON;}
#define	ioUSB_OTG_POWER_ON			(PTGD_PTGD3 = 1)
#define	ioUSB_OTG_POWER_OFF			(PTGD_PTGD3 = 0)

#define	ioUSB_HOST_POWER_INIT			{PTFDD_PTFDD6 = 1;\
											 ioUSB_HOST_POWER_ON;}
#define	ioUSB_HOST_POWER_ON			(PTFD_PTFD6 = 1)
#define	ioUSB_HOST_POWER_OFF			(PTFD_PTFD6 = 0)


#define	ioUSB_OTG_FAULT_INIT			(PTBDD_PTBDD1 = 0)
#define	ioUSB_OTG_FAULT				(PTBD_PTBD1 == 1)

#define	ioUSB_HOST_FAULT_INIT			(PTADD_PTADD1 = 0)
#define	ioUSB_HOST_FAULT				(PTAD_PTAD1 == 1)

/* \brief
	CAMERA PORT
*/

#define	ioAPU_REV_IN_INIT				{PTCDD_PTCDD0 = 1;\
											 ioAPU_REV_OFF;}
#define	ioAPU_REV_ON					(PTCD_PTCD0 = 0)
#define	ioAPU_REV_OFF					(PTCD_PTCD0 = 1)

#define	ioAPU_REV_POW_INIT			{PTGDD_PTGDD2 = 1;\
											 ioAPU_REV_POW_ON;}
#define	ioAPU_REV_POW_ON				(PTGD_PTGD2 = 1)
#define	ioAPU_REV_POW_OFF				(PTGD_PTGD2 = 0)


/* \brief
	KEY PORT
*/
#define	ioKEY_TUNE_IN_INIT				{PTDDD_PTDDD4 = 0;\
										 	PTDDD_PTDDD3 = 0;\
											PTDDD_PTDDD2 = 0;}
#define	ioKEY_TUNE_A_HIGH				(PTDD_PTDD4 == 1)
#define	ioKEY_TUNE_A_LOW				(PTDD_PTDD4 == 0)
#define	ioKEY_TUNE_A					(PTDD_PTDD4)
#define	ioKEY_TUNE_B_HIGH				(PTDD_PTDD3 == 1)
#define	ioKEY_TUNE_B_LOW				(PTDD_PTDD3 == 0)
#define	ioKEY_TUNE_B					(PTDD_PTDD3)
#define	ioKEY_TUNE_C_HIGH				(PTDD_PTDD2 == 1)
#define	ioKEY_TUNE_C_LOW				(PTDD_PTDD2 == 0)
#define	ioKEY_TUNE_C					(PTDD_PTDD2)

/* \brief
	TFT light
*/
#define	ioLIGHT_KEY_POWER_INIT_V4		{PTDDD_PTDDD1 = 1;\
											ioLIGHT_KEY_POWER_OFF_V4;}
#define	ioLIGHT_KEY_POWER_ON_V4			(PTDD_PTDD1 = 1)
#define	ioLIGHT_KEY_POWER_OFF_V4			(PTDD_PTDD1 = 0)


//#define	ioLIGHT_KEY_POWER_INIT		{PTDDD_PTDDD5 = 1;\
//											ioLIGHT_KEY_POWER_OFF;}
//#define	ioLIGHT_KEY_POWER_ON			(PTDD_PTDD5 = 1)
//#define	ioLIGHT_KEY_POWER_OFF			(PTDD_PTDD5 = 0)

//#if (eHardwareVer == HARDWARE_VER_2)
//#define	ioLIGHT_KEY_POWER_INIT		{PTBDD_PTBDD2 = 1;\
//											ioLIGHT_KEY_POWER_OFF;}
//#define	ioLIGHT_KEY_POWER_ON			(PTBD_PTBD2 = 1)
//#define	ioLIGHT_KEY_POWER_OFF			(PTBD_PTBD2 = 0)


/* \brief
	CAN enable
*/
#define	ioCAN_EN_INIT					{PTADD_PTADD5 = 1;\
											ioCAN_EN_ON;}
#define	ioCAN_EN_ON					(PTAD_PTAD5 = 1)
#define	ioCAN_EN_OFF					(PTAD_PTAD5 = 0)

#define	ioCAN_STBY_INIT				{PTBDD_PTBDD5 = 1;\
											ioCAN_STBY_OFF;}
#define	ioCAN_STBY_ON					(PTBD_PTBD5 = 0)
#define	ioCAN_STBY_OFF					(PTBD_PTBD5 = 1)

#define	ioCAN_WAKE_INIT				{PTADD_PTADD4 = 1;\
											ioCAN_WAKE_OFF;}
#define	ioCAN_WAKE_ON					(PTAD_PTAD4 = 1)
#define	ioCAN_WAKE_OFF				(PTAD_PTAD4 = 0)

#define	ioCAN_INH_INIT					{PTBDD_PTBDD0 = 0;\
											PTBPE_PTBPE0 = 1;}
#define	ioCAN_INH						(PTBD_PTBD0 == 0)

/* \brief
	Hardware version
*/
#define	ioHW_DET_VER_INIT				{PTEDD_PTEDD2 = 0;\
											PTEDD_PTEDD3 = 0;}
#define	ioHW_DET_VER_H				(PTED_PTED2 == 1)
#define	ioHW_DET_VER_L					(PTED_PTED3 == 1)


#define	ioODO_FWD_INIT				{PTADD_PTADD2 = 1;\
											ioODO_FWD_FW;}
#define	ioODO_FWD_BK				(PTAD_PTAD2 = 1)
#define	ioODO_FWD_FW				(PTAD_PTAD2 = 0)

#endif
