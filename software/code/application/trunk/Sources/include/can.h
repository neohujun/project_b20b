/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		can.h
**	Abstract:		head of can driver
**	Creat By:		Zeno Liu
**	Creat Time:		2015.03.11
**	Version:		v1.0
**
******************************************************************************/

#ifndef __CAN_H__
#define __CAN_H__

/* \brief
	include
*/
#include "type.h"

/*************NM ID*****************/
#define	canID_NM_BASE					0x400
#define	canID_NM_MAX					0x49f	//0x4ff,由于canID_RX_BCM_4A0已不是NM报文，所有NM报文最大到0x49f

/*************CAN TX/RX ID*****************/
#define	canID_RX_EPS_3A0_H			0x74
#define	canID_RX_EPS_3A0_L			0x00

#define	canID_RX_EPS_180_H			0x30
#define	canID_RX_EPS_180_L			0x00

#define	canID_RX_PAS_550_H			0xAA
#define	canID_RX_PAS_550_L			0x00

#define	canID_RX_AVM_427_H			0x84
#define	canID_RX_AVM_427_L			0xE0

#define	canID_RX_AVM_601_H			0xC0
#define	canID_RX_AVM_601_L			0x20

#define	canID_RX_AVM_620_H			0xC4
#define	canID_RX_AVM_620_L			0x00		//night panel

#define	canID_TX_AVM_671_H			0xCE		//360 Setting
#define	canID_TX_AVM_671_L			0x20		//360 Setting

#define	canID_TX_AVM_67A_H			0xCF		//360 Touch position
#define	canID_TX_AVM_67A_L			0x40		//360 Touch position

#define	canID_TX_AVM_510_H			0xA2		//GPS timer
#define	canID_TX_AVM_510_L			0x00		//GPS timer

#define	canID_TX_INS_673_H				0xCE		//instrument
#define	canID_TX_INS_673_L				0x60		//instrument

#define	canID_TX_INS_674_H				0xCE		//instrument
#define	canID_TX_INS_674_L				0x80		//instrument

#define	canID_RX_INS_678_H				0xCF		//instrument
#define	canID_RX_INS_678_L				0x00		//instrument

#define	canID_RX_BCM_4A0_H			0x94
#define	canID_RX_BCM_4A0_L			0x00

#define	canID_RX_BCM_5B0_H			0xB6
#define	canID_RX_BCM_5B0_L			0x00

#define	canID_RX_BCM_660_H			0xCC
#define	canID_RX_BCM_660_L			0x00

#define	canID_TX_BCM_587_H			0xB0		//Sunroof
#define	canID_TX_BCM_587_L			0xE0		//Sunroof

#define	canID_RX_AC_530_H				0xA6
#define	canID_RX_AC_530_L				0x00

#define	canID_TX_AC_672_H				0xCE		//AC Setting
#define	canID_TX_AC_672_L				0x40		//AC Setting

#define	canID_TX_AC_361_H				0x6C		//AC Setting
#define	canID_TX_AC_361_L				0x20		//AC Setting

#define	canID_TX_DIAG_7A0_H				0xf4
#define	canID_TX_DIAG_7A0_L				0x00

#define	canID_RX_DIAG_720_H				0xe4
#define	canID_RX_DIAG_720_L				0x00

#define	canID_RX_DIAG_FUNCID_7DF_H		0xfb
#define	canID_RX_DIAG_FUNCID_7DF_L		0xe0


#define	canID_DEBUG_INFO				0x0002


/* \brief
*/
typedef struct
{
	BYTE ID[2];
	BYTE Length;
	BYTE Data[8];
}CanFrame, *pCanFrame;

/* \brief
	function declaration
*/
void vCanTask(void);
void vCanTaskInit(void);
void vCanInit(void);
void vCanTimer(void);

void vCanWrite(pCanFrame pxCanFrame);
pCanFrame pxCanRead(void);
BOOL isCanBusOff(void);

void INTCanTx(void);
void INTCanRx(void);
void INTCanErr(void);
void INTCanWu(void);

#endif
