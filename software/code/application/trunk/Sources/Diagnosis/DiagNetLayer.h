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
	DIAG_NET_READ_ERROR_WAIT_CF_TIMEOUT,	//�ȴ�����֡��ʱ
	DIAG_NET_READ_ERROR_PHY_FUNC,			//���ձ��ĵ�Ѱַ��ʽ����
}enumDiagNetReadErrorStatus;

/* \brief
*/
typedef struct
{
	BYTE Data[diagnetMESSAGE_MAX_LENGTH];
	WORD Dlc;									//DLC��ֵ���ֱ�ӷ����ˣ�����д������buffʱ�ݴ�DlcTemp
	BOOL isFunc;									//���ڽ�����Ϣʱ��ʾ��ǰ����Ϊ����Ѱַ���Ļ�������Ѱַ����
	WORD DlcTemp;								//�����ݴ�DLC����
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


