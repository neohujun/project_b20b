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
	INS_FS_CONTINE_TO_SEND,						//允许发送方继续发送连续帧
	INS_FS_WAIT,										//表示发送方需要等待下一帧流控制帧
	INS_FS_OVERFLOW									//表示接收方缓冲区溢出
}enumInsFlowStatus;

/* \brief
*/
typedef struct
{
	BYTE TxSerialNo;										//发送帧序号
	WORD TxLengthCount;									//发送长度计数
	
	BYTE T1Timer;											//等待应答超时时间
	BYTE TxRetryCount;										//发送尝试次数
	BOOL isTxCompleted;										//当前帧发送完成标志
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
	参数为接收指针赋值
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
		pxInsMessage->Dlc = 0;							//初始化的时候只是初始化指针，不发送

		xInsInfo.xInsCommInfo.TxSerialNo = 1;				//首帧为1，
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
			{	//发送3次未接收到应答停止发送
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
	BOOL isFunc: TRUE功能寻址，FALSE物理寻址
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
				{	//大于最大帧的数据
					vInsWriteAbort();
				}
				xInsInfo.xInsCommInfo.T1Timer = 0;
			}
			else
			{	//应答的帧号不对重新发送当前帧
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
	{	//发送正在进行
		if(xInsInfo.pxInsTxMsg->Dlc + 2 <= insDATA_LENGTH_FRAME_MAX)
		{	//单帧
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
				{//清除 0x02 0x01
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
		{	//启动多帧发送
			if(0 == xInsInfo.xInsCommInfo.TxLengthCount)
			{		//首帧
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
			{		//连续帧
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

		xInsInfo.xInsCommInfo.T1Timer = insT1_TIMEOUT;	//启动等待应答超时计时
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


