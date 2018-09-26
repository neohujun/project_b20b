/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		ApuDriver.h
**	Abstract:		head of apu driver
**	Creat By:		Zeno Liu
**	Creat Time:		2015.03.05
**	Version:		v1.0
**
******************************************************************************/

#ifndef	__APU_DRIVER_H__
#define	__APU_DRIVER_H__

/* \brief
	include
*/
#include "type.h"

/* \brief
	marco define
*/
#define	apudrvFRAME_LENGTH					30
#define	apudrvCAN_UPDATE_MAX_LENGTH		128

#define	apudrvWRITE_LIST_SIZE				10
#define	apudrvWRITE_LIST_ELEM_LENGTH		apudrvFRAME_LENGTH				// frist element :1 continous frame id + 1 elem length +1 group id+ length of frame+ parity+...+not include "0xaa"
																			// not frist element:1 continous frame id+1parity+1 elem length
#define	apudrvREAD_LIST_SIZE				255

#define	apudrvWRITE_FRAME_MAX_LENGTH		255
#define	apudrvREAD_FRAME_MAX_LENGTH			apudrvCAN_UPDATE_MAX_LENGTH+9//138//烧写data最大值	57//apudrvFRAME_LENGTH		play info 的最大长度是57

#define	apudrvREAD_TIMEOUT_TIME				150				//100ms(unit:10ms)

#define	apudrvMcuAddr						0x55
#define	apudrvApuAddr						0xa0

/* \brief
	function declaration
*/
void vApuDrvPortInit(void);
void vApuDrvInit(void);
void vApuDrvTask(void);
void vApuDrvTimer(void);
BYTE* pApuDrvRead(void);
void vApuDrvWrite(BYTE GroupId, BYTE SubId, BYTE* pData, BYTE length);		//group id+sub id+data
void vApuDrvWriteBuffClear(void);

#endif


