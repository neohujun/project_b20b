/******************************************************************************
**
**								Copyright (C) 2017  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		NetworkManager.h
**	Abstract:		head of network
**	Creat By:		Zeno Liu
**	Creat Time:		2017.03.13
**	Version:		v1.0
**
******************************************************************************/

#ifndef	__NETWORK_MANAGER_H__
#define	__NETWORK_MANAGER_H__

/* \brief
*/
#include "type.h"
#include "can.h"

/* \brief
*/
//extern BOOL isNMTxLimpHomeSuccess;
//extern BOOL isNMTxFirstFrameSuccess;

/* \brief
	function declaration
*/
void vNMTask(void);
void vNMTaskInit(void);
void vNMTimer(void);
void vNMTaskAwake(BOOL isHardwareAwke);

BOOL isNMAppActived(void);
void vNMMessageHandle(CanFrame* pxCanRxMsg);
void vNMSleepAllow(BOOL isSleepAllow);
void vNMActived(BOOL isActived);
BOOL isNMSleep(void);
BOOL isNMSleepAllow(void);
void vNMBusoff(BOOL isBusoff);
void vNMBusoffRecovery(void);

#endif


