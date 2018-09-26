/******************************************************************************
**
**								Copyright (C) 2017  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		DiagDTC.h
**	Abstract:		head of diagnosis DTC
**	Creat By:		Zeno Liu
**	Creat Time:		2017.03.24
**	Version:		v1.0
**
******************************************************************************/

#ifndef	__DIAG_DTC_H__
#define	__DIAG_DTC_H__

/* \brief
*/
#include "type.h"

/* \brief
*/
#define diagdtcSTATUS_AVAILABILITY_MASK		0x3B

/* \brief
*/
typedef enum
{
	DIAG_DTC_CAN_ERROR,			//can处于bus off状态超过5个周期
	DIAG_DTC_BT_COMM_ERROR,
	DIAG_DTC_NM_LIMPHOME,
	DIAG_DTC_LOST_BCM,
	DIAG_DTC_LOST_EMS,
//	DIAG_DTC_LOST_TCU,				//多媒体不接收TCU报文
	DIAG_DTC_LOST_ICM,
	DIAG_DTC_VOLTAGE_HIGH_EXCEPTION,
	DIAG_DTC_VOLTAGE_LOW_EXCEPTION,
	DIAG_DTC_NUM,
}enumDiagDTC;

/* \brief
*/
typedef enum
{
	DIAG_DTC_CTL_START,
	DIAG_DTC_CTL_FAIL,
	DIAG_DTC_CTL_SUCESS,
	DIAG_DTC_CTL_CONFIG,
	DIAG_DTC_CTL_STOP
}enumDiagDTCCtl;

/* \brief
*/
void vDiagDTCInit(void);
void vDiagDTCTimer(void);
void vDiagDTC(enumDiagDTC eDTC, enumDiagDTCCtl eDTCCtl);
BOOL blDiagDTCClear(BYTE DTC[3]);
WORD getDiagDTCNum(BYTE Mask);
BYTE getDiagDTC(BYTE Mask, BYTE DTCData[]);
BYTE getDiagAllDTC(BYTE DTCData[]);

#endif


