/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		Config.h
**	Abstract:		
**	Creat By:		Zeno Liu
**	Creat Time:		2015.10.14
**	Version:		v1.0
**
******************************************************************************/

#ifndef __CONFIG_H__
#define __CONFIG_H__

/* \brief
*/

#define	__SMARTA_B20B__					0
#define	__SMARTA_B20B_newPlant__			1


/*
 *  config.h - System Configurations 
 *
 *  Copyright (C) 2011~2014 Intersil Corporation
 *
 */
/*
Disclaimer: THIS INFORMATION IS PROVIDED 'AS-IS' FOR EVALUATION PURPOSES ONLY.  
INTERSIL CORPORATION AND ITS SUBSIDIARIES ('INTERSIL') DISCLAIM ALL WARRANTIES, 
INCLUDING WITHOUT LIMITATION FITNESS FOR A PARTICULAR PURPOSE AND MERCHANTABILITY.  
Intersil provides evaluation platforms to help our customers to develop products. 
However, factors beyond Intersil's control could significantly affect Intersil 
product performance. 
It remains the customers' responsibility to verify the actual system performance.
*/
/*****************************************************************************/
/*																			 */
/*                           System Configurations                           */
/*																			 */
/*****************************************************************************/		   

//-----------------------------------------------------------------------------
// MCU
//-----------------------------------------------------------------------------
// DP80390XP - TW8836A, TW8836B.

//-----------------------------------------------------------------------------
// MODEL
//-----------------------------------------------------------------------------
#define MODEL_TW8836B2
//#define MODEL_TW8836DEMO
/*
	DEMO set use
	#define PANEL_1024X600
	#define SUPPORT_HDMI_24BIT
	#undef SUPPORT_BT656_LOOP		

*/

//-----------------------------------------------------------------------------
// Firmware Version
//-----------------------------------------------------------------------------
#define	FWVER			0x044		//REV 0.44 160205 TW8836B2 for Watchdog test

//-----------------------------------------------------------------------------
// PANEL
// select only one
//-----------------------------------------------------------------------------
#define PANEL_800X480
//#define PANEL_1024X600
//#define PANEL_1280X800


//-----------------------------------------------------------------------------
// Hardware
//-----------------------------------------------------------------------------
#define SUPPORT_I2C_MASTER		//some customer ask to disable.
#define USE_SFLASH_EEPROM		//E3PROM(FlashEepromEmulator)
			
#define SUPPORT_SPIOSD



#define SUPPORT_TOUCH


#define SUPPORT_WATCHDOG
#define SUPPORT_SPIFLASH_4BYTES_ADDRESS


//-----------------------------------------------------------------------------
// Software	MODELs
//
//-----------------------------------------------------------------------------
#define SUPPORT_I2CCMD_SERVER
//#define SUPPORT_RCD					//RearCameraDisplay. use EE[0x0F]
#undef SUPPORT_FAST_INPUT_TOGGLE	//only for CVBS & DTV
#undef SUPPORT_FOSD_MENU			//FontOSD MENU


//-----------------------------------------------------------------------------
//		IR Remote Controller Type
//-----------------------------------------------------------------------------
#define REMO_RC5					// RC5 style
#define TECHWELL_REMOCON
//#define REMO_NEC					// NEC style
//#define PHILIPS_REMOCON 			// New remocon 


//-----------------------------------------------------------------------------
//		Options for Possible Inputs
//-----------------------------------------------------------------------------

#define SUPPORT_DEC
#ifdef SUPPORT_DEC
	#define SUPPORT_CVBS
	#define SUPPORT_SVIDEO
	#undef SUPPORT_DCVBS
#endif

#define SUPPORT_ARGB
#ifdef SUPPORT_ARGB
	#define SUPPORT_COMPONENT			// support component analog
	#define SUPPORT_PC  				// support PC
#endif
	#define ANALOG_OVERSCAN

#define SUPPORT_DTV
#ifdef SUPPORT_DTV
	#define SUPPORT_HDMI
	#ifdef SUPPORT_I2C_MASTER
		#define SUPPORT_HDMI_EP907M
	#endif
	#define SUPPORT_HDMI_24BIT		/* conflict with BT656_LOOP */
	#undef SUPPORT_DEONLY_DTV

	#define SUPPORT_DTV656		//BT656 Decoder at DTV i656 input. Not related with BT656 module.
#endif
#if defined(MODEL_TW8836DEMO)
	#define SUPPORT_HDMI_24BIT		/* conflict with BT656_LOOP */
#endif


#define SUPPORT_LVDSRX
#define SUPPORT_BT656		//BT656 Encoder

#if defined(SUPPORT_CVBS) && defined(SUPPORT_BT656) && defined(SUPPORT_DTV656)
	//CVBS=>BT656Enc=>BT656Dec=>Scaler=>Panel.
    //aRGB(low resolution)=>BT656Enc=>BT656Dec=>Scaler=>Panel.
	//LVDS-RX(low resolution)=>BT656Enc=>BT656Dec=>Scaler=>Panel.
	#define SUPPORT_BT656_LOOP		
#endif

#if defined(MODEL_TW8836DEMO)
	#undef SUPPORT_BT656_LOOP		
#endif



#define SUPPORT_SCALER_OVERWRITE_TABLE


//-----------------------------------------------------------------------------
//		Options for Possible Standards
//		Default:NTSC
//-----------------------------------------------------------------------------
#ifdef SUPPORT_FOSD_MENU
	#define SUPPORT_PAL			
	#define SUPPORT_SECAM
	#define SUPPORT_NTSC4		
	#define SUPPORT_PALM	
	#define SUPPORT_PALN		
	#define SUPPORT_PAL60		
#endif

//-----------------------------------------------------------------------------
//		Options for Debugging/Release
//-----------------------------------------------------------------------------
#define DEBUG		// include debug information
#ifdef DEBUG
	#undef DEBUG_MAIN
	#undef DEBUG_TIME
	#undef DEBUG_ISR
	#undef DEBUG_TW88
	#undef DEBUG_UART
	#undef DEBUG_I2C
	#undef DEBUG_SPI
	#undef DEBUG_EEP
	#undef DEBUG_SFLASH_EEPROM
	#define DEBUG_SPIFLASH_TEST
	#undef DEBUG_OSD
		#undef DEBUG_FOSD
		#undef DEBUG_SOSD
	#undef DEBUG_AUDIO
	#undef DEBUG_SETPANEL
	#undef DEBUG_DECODER
	#define DEBUG_PC
	#undef DEBUG_DTV
	#undef DEBUG_BT656
	#undef DEBUG_BANK
	#undef DEBUG_PAUSE
	#undef DEBUG_MENU
	#undef DEBUG_KEYREMO
	#undef DEBUG_TOUCH_HW
	#undef DEBUG_TOUCH_SW
	#undef DEBUG_REMO
	#undef DEBUG_REMO_NEC
	#ifdef SUPPORT_WATCHDOG
		#undef DEBUG_WATCHDOG
	#endif
	#undef DEBUG_SCALER
	#undef DEBUG_SCALER_OVERWRITE_TABLE
#endif


//-----------------------------------------------------------------------------
//		Panel Vendor Specific
// select only ONE !!
//-----------------------------------------------------------------------------
#ifdef PANEL_800X480
	//default. for PANEL_INNOLUX_AT080TN03 & PANEL_INNILUX_AT070TN84
	#define PANEL_TCON
	#undef  PANEL_SRGB
	#undef	PANEL_FP_LSB
	#define PANEL_FORMAT_666

	#define PANEL_H			820
	#define PANEL_V			246
	#define PANEL_H_MIN		848		//unknown
	#define PANEL_H_TYP		1056	//1088
	#define PANEL_H_MAX		2000	//unknown
	#define PANEL_PCLK_MIN	27		//unknown
	#define PANEL_PCLK_TYP	40
	#define PANEL_PCLK_MAX	45

	#define PANEL_SSPLL		108
	#define PANEL_PCLK_DIV	1
	#define PANEL_PCLKO_DIV	3		
#endif


//-----------------------------------------------------------------------------
//	SPIOSD_IMAGE SIZE
//-----------------------------------------------------------------------------
#if (PANEL_H==1280 && PANEL_V==800)
	#define SPIOSD_USE_1024X600_IMG
#elif (PANEL_H==1024 && PANEL_V==600)
	#define SPIOSD_USE_1024X600_IMG
#else
	#define SPIOSD_USE_800X480_IMG
#endif


//-----------------------------------------------------------------------------
// default FREERUN value.
// It is depend on PANEL size & PCLK.
//-----------------------------------------------------------------------------
#if (PANEL_H==1280 && PANEL_V==800)
//	#define FREERUN_DEFAULT_HTOTAL	1400
//	#define FREERUN_DEFAULT_VTOTAL	875
	//SSPLL:120MHz, PCLKO:80MHz
	#define FREERUN_DEFAULT_HTOTAL	1768
	#define FREERUN_DEFAULT_VTOTAL	875
#elif (PANEL_H==1024 && PANEL_V==600)
//@90MHz div2
//	#define FREERUN_DEFAULT_HTOTAL	1144
//	#define FREERUN_DEFAULT_VTOTAL	656
//@90MHz div1.5
//	#define FREERUN_DEFAULT_HTOTAL	1450
//	#define FREERUN_DEFAULT_VTOTAL	692
//108M div2
//	#define FREERUN_DEFAULT_HTOTAL	1304
//	#define FREERUN_DEFAULT_VTOTAL	689
	#define FREERUN_DEFAULT_HTOTAL	1376
	#define FREERUN_DEFAULT_VTOTAL	656
#else
	#define FREERUN_DEFAULT_HTOTAL	1085
	#define FREERUN_DEFAULT_VTOTAL	553
#endif



#endif
