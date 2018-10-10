/**********************************************************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		System.h
**	Abstract:		
**	Creat By:		Zeno Liu
**	Creat Time:		2015.04.13
**	Version:		v1.0
**
**********************************************************************************************************************/

#ifndef	_SYSTEM_H_
#define _SYSTEM_H_

/**********************************************************************************************************************
**	Include Files
**********************************************************************************************************************/
#include "Public.h"

/**********************************************************************************************************************
**	Macdefine
**********************************************************************************************************************/
#ifdef	__CMM__
#define	systemVERSION_Y				15
#define	systemVERSION_M				10
#define	systemVERSION_D				14
#else
#define	systemVERSION_Y				18
#define	systemVERSION_M				10
#define	systemVERSION_D				9
#endif

#define	systemACC_IN_INIT				(PTBDD_PTBDD4 = 0)
#define	systemACC_IN					(PTBD_PTBD4 == 0)

#define	systemBAT_IN_INIT				(PTBDD_PTBDD2 = 0)
#define	systemBAT_IN					(PTBD_PTBD2 == 1)

#define	systemPOWER_INIT				{PTCDD_PTCDD3 = 1;\
										 systemPOWER_ON;}
#define	systemPOWER_ON				(PTCD_PTCD3 = 1)
#define	systemPOWER_OFF				(PTCD_PTCD3 = 0)

#ifdef	__CMM__
#define	systemTF_POWER_INIT			{PTDDD_PTDDD5 = 1;\
										 systemTF_POWER_ON;}
#define	systemTF_POWER_ON			(PTDD_PTDD5 = 1)
#define	systemTF_POWER_OFF			(PTDD_PTDD5 = 0)
#define	systemMONITOR_INIT			{PTGDD_PTGDD4 = 1;\
										 systemMONITOR_OFF;}
#define	systemMONITOR_ON				(PTGD_PTGD4 = 1)
#define	systemMONITOR_OFF				(PTGD_PTGD4 = 0)
#else
#define	systemMONITOR_POWER_INIT	{PTDDD_PTDDD1 = 1;\
										 PTGDD_PTGDD5 = 1;\
										 PTGDD_PTGDD4 = 1;\
										 systemMONITOR_POWER_ON;\
										 systemMONITOR_OFF;}
#define	systemMONITOR_POWER_ON		{PTDD_PTDD1 = 1;\
										 PTGD_PTGD5 = 1;\
										 PTGD_PTGD4 = 1;}
#define	systemMONITOR_POWER_OFF		{PTDD_PTDD1 = 0;\
										 PTGD_PTGD5 = 0;\
										 PTGD_PTGD4 = 0;}
#define	systemMONITOR_ON				(PTDD_PTDD1 = 1)
#define	systemMONITOR_OFF				(PTDD_PTDD1 = 0)
#endif

#define	systemMONITOR_VGHL_INIT		{PTADD_PTADD2 = 1;\
										 systemMONITOR_VGHL_ON;}
#define	systemMONITOR_VGHL_ON		(PTAD_PTAD2 = 1)
#define	systemMONITOR_VGHL_OFF		(PTAD_PTAD2 = 0)

#define	systemAPU_HW_RESET_INIT		{PTGDD_PTGDD2 = 1;\
										 systemAPU_HW_RESET_HIGH;}
#define	systemAPU_HW_RESET_HIGH		(PTGD_PTGD2 = 1)
#define	systemAPU_HW_RESET_LOW		(PTGD_PTGD2 = 0)
#define	systemAPU_POWER_INIT				{PTADD_PTADD3 = 1;\
										 systemAPU_POWER_ON;}
#define	systemAPU_POWER_ON					(PTAD_PTAD3 = 1)
#define	systemAPU_POWER_OFF					(PTAD_PTAD3 = 0)

#define	systemAPU_RESET_INIT				{PTADD_PTADD0 = 1;\
										 systemAPU_RESET_HIGH;}
#define	systemAPU_RESET_HIGH				(PTAD_PTAD0 = 1)
#define	systemAPU_RESET_LOW					(PTAD_PTAD0 = 0)

#define	systemAPU_AWAKE_SLEEP_INIT			{PTCDD_PTCDD2 = 1;\
										 systemAPU_AWAKE;}
#define	systemAPU_AWAKE						(PTCD_PTCD2 = 1)
#define	systemAPU_SLEEP						(PTCD_PTCD2 = 0)
//#define	systemUSB_DVD_POWER_INIT			{PTBDD_PTBDD3 = 1;\
//										 systemUSB_DVD_POWER_ON;}
//#define	systemUSB_DVD_POWER_ON				(PTBD_PTBD3 = 1)
//#define	systemUSB_DVD_POWER_OFF				(PTBD_PTBD3 = 0)

#define	systemAUDIO_MUTE_INIT				{PTEDD_PTEDD3 = 1;\
										 systemAUDIO_MUTE_ON;}
#define	systemAUDIO_MUTE_ON					(PTED_PTED3 = 0)
#define	systemAUDIO_MUTE_OFF				(PTED_PTED3 = 1)

#define	systemAUDIO_AMP_MUTE_INIT			{PTFDD_PTFDD4 = 1;\
										 systemAUDIO_AMP_MUTE_ON;}
#define	systemAUDIO_AMP_MUTE_ON				(PTFD_PTFD4 = 1)
#define	systemAUDIO_AMP_MUTE_OFF			(PTFD_PTFD4 = 0)
#define	systemAUDIO_AMP_STBY_INIT			{PTFDD_PTFDD5 = 1;\
										 systemAUDIO_AMP_STBY_OFF;}
#define	systemAUDIO_AMP_STBY_ON				(PTFD_PTFD5 = 1)
#define	systemAUDIO_AMP_STBY_OFF			(PTFD_PTFD5 = 0)

#ifdef	__HANG_SHENG__
#define	systemAPU_BACK_INIT					{PTCDD_PTCDD0 = 1;\
										 		PTCD_PTCD0 = 1;}
#else
#define	systemAPU_BACK_INIT					{PTCDD_PTCDD0 = 1;\
										 		PTCD_PTCD0 = 1;}
#endif
										 		
#define	systemFORCE_FLASH_WRITE_INIT		(PTADD_PTADD7 = 0)
#define	systemFORCE_FLASH_WRITE				(PTAD_PTAD7 == 0)

#ifdef	__CMM__
#define	systemRGB_POWER_INIT					{PTADD_PTADD2 = 1;\
												 systemRGB_POWER_ON;}
#define	systemRGB_POWER_ON					(PTAD_PTAD2 = 1)
#define	systemRGB_POWER_OFF					(PTAD_PTAD2 = 0)

#define	systemAUDIO_ENABLE_INIT			{PTBDD_PTBDD2 = 1;\
											 systemAUDIO_DISABLE;}
#define	systemAUDIO_ENABLE					(PTBD_PTBD2 = 1)
#define	systemAUDIO_DISABLE				(PTBD_PTBD2 = 0)

#define	sytemHDPOWER_INIT				{PTBDD_PTBDD3 = 1;\
											 sytemHDPOWER_ON;}
#define	sytemHDPOWER_ON					(PTBD_PTBD3 = 1)
#define	sytemHDPOWER_OFF					(PTBD_PTBD3 = 0)

#define	systemLOW_POWER_IN_INIT			(PTBDD_PTBDD1 = 0)
#define	systemLOW_POWER_IN				(PTBD_PTBD1 == 0)
#endif

//d50
#define	systemUSB_CONTROL_HOST1_INIT		{PTADD_PTADD1 = 1;\
												 systemUSB_CONTROL_HOST1_ON;}
#define	systemUSB_CONTROL_HOST1_ON			(PTAD_PTAD1 = 1)
#define	systemUSB_CONTROL_HOST1_OFF			(PTAD_PTAD1 = 0)

//XM1001
#define	systemUSB_CONTROL_HOST2_INIT		{PTFDD_PTFDD6 = 1;\
												 systemUSB_CONTROL_HOST2_ON;}
#define	systemUSB_CONTROL_HOST2_ON			(PTFD_PTFD6 = 1)
#define	systemUSB_CONTROL_HOST2_OFF			(PTFD_PTFD6 = 0)

#define	ioTw88xx_RESET_INIT			{PTBDD_PTBDD3 = 1;\
											ioTw88xx_RESET_L;}
#define	ioTw88xx_RESET_H				(PTBD_PTBD3 = 1)
#define	ioTw88xx_RESET_L				(PTBD_PTBD3 = 0)

/**********************************************************************************************************************
**	variable declaration
**********************************************************************************************************************/
extern UINT08 SystemReqState;

/**********************************************************************************************************************
**	function declaration
**********************************************************************************************************************/
void SystemTask(void);

void SystemInit(void);
void SystemHardwareInit(void);
void SystemFortyMSecTimer(void);

#endif

