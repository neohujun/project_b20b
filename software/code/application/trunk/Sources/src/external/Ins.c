/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		Ins.c
**	Abstract:			instrument function
**	Creat By:		neo Hu
**	Creat Time:		2017.11.27
**	Version:		v1.0
**
******************************************************************************/

/* \brief
	include
*/
#include "Ins.h"
#include "List.h"
#include "CanApp.h"
#include "system.h"
#include "common.h"
#include <MC9S08DZ60.h>

#define MXA_PLAY_INFO_LEN		40
/* \brief
*/
#define	insFILL_BYTE							0x00
#define	insDATA_LENGTH_FRAME_MAX				8

#define	insTX_SERIAL_NO_MAX				7

#define	insTX_RETRY_COUNT					3
#define	insT1_TIMEOUT						10

/* \brief
*/
/*
typedef enum
{
	INS_SINGLE_FRAME,								//SF
	INS_FIRST_FRAME						=0x10,		//FF
	INS_CONSECUTIVE_FRAME				=0x20,		//CF
	INS_FLOW_CONTROL_FRAME				=0x30,		//FC
}enumInsFrameType;
*/
/* \brief
*/
typedef enum
{
	INS_RESP_N_MEDIA,							//media negative
	INS_RESP_P_MUSIC				=0x01,		//music positive
	INS_RESP_P_RADIO				=0x02,		//radio
	INS_RESP_P_PHONE				=0x03,		//phone
	INS_RESP_P_NAVI				=0X04,
	INS_RESP_N_NAVI
}enumInsFrameType;

/* \brief
*/
typedef enum
{
	INS_FS_CONTINE_TO_SEND,						//�����ͷ�������������֡
	INS_FS_WAIT,										//��ʾ���ͷ���Ҫ�ȴ���һ֡������֡
	INS_FS_OVERFLOW									//��ʾ���շ����������
}enumInsFlowStatus;

/* \brief
*/
typedef struct
{
	BYTE TxSerialNo;										//����֡���
	WORD TxLengthCount;									//���ͳ��ȼ���
	
	BYTE T1Timer;											//�ȴ�Ӧ��ʱʱ��
	BYTE TxRetryCount;										//���ͳ��Դ���
	BOOL isTxCompleted;										//��ǰ֡������ɱ�־
}InsCommInfo;

/* \brief
*/
typedef struct
{
	InsMessage* pxInsTxMsg;
	InsMessage xInsRxMsg;
	InsCommInfo xInsCommInfo;

	CanFrame xCanTxMsg;
	CanFrame xNaviCanTxMsg;
}InsInfo;

/* \brief
*/
InsInfo xInsInfo;


/* \brief
*/
static void vInsWriteTask(void);
static void vInsWriteAbort(void);

/* \brief
*/
void vInsTask(void)
{
	vInsWriteTask();
}

/* \brief
	����Ϊ����ָ�븳ֵ
*/
void vInsTaskInit(InsMessage* pxInsMessage)
{
	xInsInfo.xCanTxMsg.Length  = insDATA_LENGTH_FRAME_MAX;
	xInsInfo.xCanTxMsg.ID[0]  = insCAN_MEDIA_TXID_H;
	xInsInfo.xCanTxMsg.ID[1]  = insCAN_MEDIA_TXID_L;

	xInsInfo.xNaviCanTxMsg.Length  = insDATA_LENGTH_FRAME_MAX;
	xInsInfo.xNaviCanTxMsg.ID[0]  = insCAN_NAVI_TXID_H;
	xInsInfo.xNaviCanTxMsg.ID[1]  = insCAN_NAVI_TXID_L;

	if(NULL != pxInsMessage)
	{
		xInsInfo.pxInsTxMsg = pxInsMessage;
		xInsInfo.xInsCommInfo.TxLengthCount = 0;
		pxInsMessage->Dlc = 0;							//��ʼ����ʱ��ֻ�ǳ�ʼ��ָ�룬������

		xInsInfo.xInsCommInfo.TxSerialNo = 1;				//��֡Ϊ1��
	}
}

/* \brief
*/
void vInsTimer(void)
{
	if(0 != xInsInfo.xInsCommInfo.T1Timer)
	{
		if(0 == --xInsInfo.xInsCommInfo.T1Timer)
		{
			if(insTX_RETRY_COUNT <= xInsInfo.xInsCommInfo.TxRetryCount)
			{	//����3��δ���յ�Ӧ��ֹͣ����
				vInsWriteAbort();
			}
		}
	}
	
	return;
}


/* \brief
*/
void vInsWrite(InsMessage* pxInsMessage)
{
	if(NULL == pxInsMessage)
	{
		return;
	}

	xInsInfo.pxInsTxMsg = pxInsMessage;
	xInsInfo.xInsCommInfo.TxLengthCount = 0;
	xInsInfo.xInsCommInfo.TxRetryCount = 0;

	xInsInfo.xInsCommInfo.TxSerialNo = 1;
	
	return;
}

/* \brief
*/
BOOL isInsWriteBusy(void)
{
	if((NULL == xInsInfo.pxInsTxMsg) || (xInsInfo.xInsCommInfo.TxLengthCount >= xInsInfo.pxInsTxMsg->Dlc))
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

/* \brief
	BOOL isFunc: TRUE����Ѱַ��FALSE����Ѱַ
*/
void vInsMessageHandle(CanFrame* pxCanMsg)
{
	switch(pxCanMsg->Data[0])
	{
		default:
		case INS_RESP_N_MEDIA:
		case INS_RESP_N_NAVI:
			vInsWriteAbort();
			xInsInfo.xInsCommInfo.T1Timer = 0;
			break;
			
		case INS_RESP_P_MUSIC:
		case INS_RESP_P_RADIO:
		case INS_RESP_P_PHONE:
		case INS_RESP_P_NAVI:
			if(pxCanMsg->Data[1] == xInsInfo.xInsCommInfo.TxSerialNo)
			{
				xInsInfo.xInsCommInfo.TxLengthCount += 6;
				if(++xInsInfo.xInsCommInfo.TxSerialNo > insTX_SERIAL_NO_MAX)
				{	//�������֡������
					vInsWriteAbort();
				}
				xInsInfo.xInsCommInfo.T1Timer = 0;
			}
			else
			{	//Ӧ���֡�Ų������·��͵�ǰ֡
			}
			break;
	}
}

/* \brief
*/
static void vInsWriteTask(void)
{
	BYTE i=0;
	
	if((NULL == xInsInfo.pxInsTxMsg) || (0 != xInsInfo.xInsCommInfo.T1Timer))
	{
		return;
	}
	
	if(xInsInfo.xInsCommInfo.TxLengthCount < xInsInfo.pxInsTxMsg->Dlc)
	{	//�������ڽ���
		if(xInsInfo.pxInsTxMsg->Dlc + 2 <= insDATA_LENGTH_FRAME_MAX)
		{	//��֡
			//play info
			if(xInsInfo.pxInsTxMsg ->eApuwShowMedia < APUW_SHOW_NAVI)
			{
				xInsInfo.xCanTxMsg.Data[0] = (BYTE)(xInsInfo.pxInsTxMsg ->Dlc) + 2;
				xInsInfo.xCanTxMsg.Data[1] = xInsInfo.pxInsTxMsg ->eApuwShowMedia << 3 | xInsInfo.xInsCommInfo.TxSerialNo;
				for(i=2; i!= insDATA_LENGTH_FRAME_MAX; ++i)
				{
					if((i-2) < xInsInfo.pxInsTxMsg->Dlc)
					{
						xInsInfo.xCanTxMsg.Data[i] = xInsInfo.pxInsTxMsg->Data[i-2];
					}
					else
					{
						xInsInfo.xCanTxMsg.Data[i] = insFILL_BYTE;
					}
				}
				(void)blCanAppWrite(&xInsInfo.xCanTxMsg);
			}
			else
			{
				//navi
				if(0 == xInsInfo.pxInsTxMsg ->eApuwNaviState)
				{//��� 0x02 0x01
					xInsInfo.pxInsTxMsg ->Dlc -= 1;
				}
				xInsInfo.xNaviCanTxMsg.Data[0] = (BYTE)(xInsInfo.pxInsTxMsg ->Dlc) + 2;
				xInsInfo.xNaviCanTxMsg.Data[1] = xInsInfo.pxInsTxMsg ->eApuwNaviState << 3 | xInsInfo.xInsCommInfo.TxSerialNo;

				for(i=2; i!= insDATA_LENGTH_FRAME_MAX; ++i)
				{
					if((i-2) < xInsInfo.pxInsTxMsg->Dlc)
					{
						xInsInfo.xNaviCanTxMsg.Data[i] = xInsInfo.pxInsTxMsg->Data[i-2];
					}
					else
					{
						xInsInfo.xNaviCanTxMsg.Data[i] = insFILL_BYTE;
					}
				}
				
				(void)blCanAppWrite(&xInsInfo.xNaviCanTxMsg);
			}
			
			
		}
		else
		{	//������֡����
			if(0 == xInsInfo.xInsCommInfo.TxLengthCount)
			{		//��֡
				if(xInsInfo.pxInsTxMsg ->eApuwShowMedia < APUW_SHOW_NAVI)
				{	//play info
					xInsInfo.xCanTxMsg.Data[0] = (BYTE)(xInsInfo.pxInsTxMsg ->Dlc) + ((xInsInfo.pxInsTxMsg ->Dlc-1)/6) * 2 + 2;
					xInsInfo.xCanTxMsg.Data[1] = xInsInfo.pxInsTxMsg ->eApuwShowMedia << 3 | xInsInfo.xInsCommInfo.TxSerialNo;

					for(i=2; i!= insDATA_LENGTH_FRAME_MAX; ++i)
					{
						xInsInfo.xCanTxMsg.Data[i] = xInsInfo.pxInsTxMsg->Data[i-2];
					}
					
					(void)blCanAppWrite(&xInsInfo.xCanTxMsg);
				}
				else
				{

					xInsInfo.xNaviCanTxMsg.Data[0] = (BYTE)(xInsInfo.pxInsTxMsg ->Dlc) + ((xInsInfo.pxInsTxMsg ->Dlc-1)/6) * 2 + 2;
					xInsInfo.xNaviCanTxMsg.Data[1] = xInsInfo.pxInsTxMsg ->eApuwNaviState << 3 | xInsInfo.xInsCommInfo.TxSerialNo;

					for(i=2; i!= insDATA_LENGTH_FRAME_MAX; ++i)
					{
						xInsInfo.xNaviCanTxMsg.Data[i] = xInsInfo.pxInsTxMsg->Data[i-2];
					}
					
					(void)blCanAppWrite(&xInsInfo.xNaviCanTxMsg);
				}
			}
			else
			{		//����֡
				if(xInsInfo.pxInsTxMsg ->eApuwShowMedia < APUW_SHOW_NAVI)
				{	//play info
					xInsInfo.xCanTxMsg.Data[0] = (BYTE)(xInsInfo.pxInsTxMsg ->Dlc) + ((xInsInfo.pxInsTxMsg ->Dlc-1)/6) * 2 + 2;
					xInsInfo.xCanTxMsg.Data[1] = xInsInfo.pxInsTxMsg ->eApuwShowMedia << 3 | xInsInfo.xInsCommInfo.TxSerialNo;

					for(i=2; i!= insDATA_LENGTH_FRAME_MAX; ++i)
					{
						if((i-2) < (xInsInfo.pxInsTxMsg->Dlc-xInsInfo.xInsCommInfo.TxLengthCount))
						{
							xInsInfo.xCanTxMsg.Data[i] = xInsInfo.pxInsTxMsg->Data[xInsInfo.xInsCommInfo.TxLengthCount + i - 2];
						}
						else
						{
							xInsInfo.xCanTxMsg.Data[i] = insFILL_BYTE;
						}
					}
					
					(void)blCanAppWrite(&xInsInfo.xCanTxMsg);
				}
				else
				{

					xInsInfo.xNaviCanTxMsg.Data[0] = (BYTE)(xInsInfo.pxInsTxMsg ->Dlc) + ((xInsInfo.pxInsTxMsg ->Dlc-1)/6) * 2 + 2;
					xInsInfo.xNaviCanTxMsg.Data[1] = xInsInfo.pxInsTxMsg ->eApuwNaviState << 3 | xInsInfo.xInsCommInfo.TxSerialNo;

					for(i=2; i!= insDATA_LENGTH_FRAME_MAX; ++i)
					{
						if((i-2) < (xInsInfo.pxInsTxMsg->Dlc-xInsInfo.xInsCommInfo.TxLengthCount))
						{
							xInsInfo.xNaviCanTxMsg.Data[i] = xInsInfo.pxInsTxMsg->Data[xInsInfo.xInsCommInfo.TxLengthCount + i - 2];
						}
						else
						{
							xInsInfo.xNaviCanTxMsg.Data[i] = insFILL_BYTE;
						}
					}
					
					(void)blCanAppWrite(&xInsInfo.xNaviCanTxMsg);
				}
			}
		}

		xInsInfo.xInsCommInfo.T1Timer = insT1_TIMEOUT;	//�����ȴ�Ӧ��ʱ��ʱ
		++xInsInfo.xInsCommInfo.TxRetryCount;
	}
}

/* \brief
*/
static void vInsWriteAbort(void)
{
	xInsInfo.xInsCommInfo.TxLengthCount = xInsInfo.pxInsTxMsg->Dlc;
	xInsInfo.xInsCommInfo.TxRetryCount = 0;

	xInsInfo.xInsCommInfo.TxSerialNo = 1;
}


