/******************************************************************************
**
**								Copyright (C) 2017  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		DiagNetLayer.h
**	Abstract:		head of diagnosis net layer
**	Creat By:		Zeno Liu
**	Creat Time:		2017.03.20
**	Version:		v1.0
**
******************************************************************************/

#ifndef	__DIAG_NET_LAYER_H__
#define	__DIAG_NET_LAYER_H__

/* \brief
*/
#include "type.h"
#include "can.h"

/* \brief
*/
#define	diagnetMESSAGE_MAX_LENGTH		103

//TX ID:0x7A0
#define	diagnetCAN_DIAG_TXID_H			canID_TX_DIAG_7A0_H
#define	diagnetCAN_DIAG_TXID_L			canID_TX_DIAG_7A0_L

/* \brief
*/
typedef enum
{
	DIAG_NET_RESULT_NULL,
	DIAG_NET_RESULT_OK,
	DIAG_NET_RESULT_STATUS_ERROR,
	DIAG_NET_RESULT_BS_TIMEOUT_ERROR,
	DIAG_NET_RESULT_CR_TIMEOUT_ERROR
}enumDiagNetWriteResultStatus;

/* \brief
*/
typedef enum
{
	DIAG_NET_READ_ERROR_NULL,
	DIAG_NET_READ_ERROR_SN,					//SERIAL NO ERROR
	DIAG_NET_READ_ERROR_WAIT_CF_TIMEOUT,	//等待连续帧超时
	DIAG_NET_READ_ERROR_PHY_FUNC,			//接收报文的寻址方式错乱
}enumDiagNetReadErrorStatus;

/* \brief
*/
typedef struct
{
	BYTE Data[diagnetMESSAGE_MAX_LENGTH];
	WORD Dlc;									//DLC赋值后就直接发送了，所以写入数据buff时暂存DlcTemp
	BOOL isFunc;									//用于接收消息时表示当前报文为功能寻址报文还是物理寻址报文
	WORD DlcTemp;								//用于暂存DLC长度
}DiagNetMessage;

/* \brief
	function declaration
*/
void vDiagNetTask(void);
void vDiagNetTaskInit(DiagNetMessage* pxDiagNetMessage);
void vDiagNetTimer(void);

void vDiagNetWrite(DiagNetMessage* pxDiagNetMessage);
DiagNetMessage* pxDiagNetRead(void);
enumDiagNetWriteResultStatus eGetDiagNetWriteResult(void);
void vDiagNetMessageHandle(CanFrame* pxCanMsg, BOOL isFunc);

#endif


