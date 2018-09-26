/******************************************************************************
**
**								Copyright (C) 2017  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		DiagNetLayer.c
**	Abstract:		diagnosis net layer
**	Creat By:		Zeno Liu
**	Creat Time:		2017.03.20
**	Version:		v1.0
**
******************************************************************************/


/* \brief
	include files
*/
#include "DiagNetLayer.h"

#include "can.h"

/* \brief
*/
#define	diagnetFILL_BYTE						0xaa
#define	diagnetDATA_LENGTH_FRAME_MAX		8

#define	diagnetTX_SERIAL_NO_MAX				15

//流控制帧参数
#define	diagnetBLOCK_SIZE_MAX					8
#define	diagnetSTMIN							2
#define	diagnetBS_TIMEOUT						15
#define	diagnetCR_TIMEOUT						15

/* \brief
*/
typedef enum
{
	DIAG_NET_SINGLE_FRAME,								//SF
	DIAG_NET_FIRST_FRAME						=0x10,		//FF
	DIAG_NET_CONSECUTIVE_FRAME				=0x20,		//CF
	DIAG_NET_FLOW_CONTROL_FRAME			=0x30,		//FC
}enumDiagNetFrameType;

/* \brief
*/
typedef enum
{
	DIAG_NET_FS_CONTINE_TO_SEND,						//允许发送方继续发送连续帧
	DIAG_NET_FS_WAIT,										//表示发送方需要等待下一帧流控制帧
	DIAG_NET_FS_OVERFLOW									//表示接收方缓冲区溢出
}enumDiagNetFlowStatus;

/* \brief
*/
typedef struct
{
	enumDiagNetWriteResultStatus eResultStatus;					//发送结束状态
	BYTE TxSerialNo;										//发送帧序号
	WORD TxLengthCount;									//发送长度计数
	
	BOOL isRxMsg;											//接收到报文标志
	WORD RxLengthCount;
	BYTE RxBlockSizeCount;
	BYTE RxSerialNo;
	enumDiagNetReadErrorStatus eReadErrorStatus;

	//流控制>>
	BOOL isFlowControl;										//发送连续帧时接收到了流控制帧
	enumDiagNetFlowStatus eFlowStatus;						//流控制状态
	BOOL isNoWaitFlowControl;									//不需要等待流控制帧直接发送
	BYTE BlockSize;											//流控制连续发送帧数目
	WORD STmin;											//两个连续帧间的最小间隔
	WORD STTimer;											//连续帧间间隔时间计时器
	WORD BSTimer;											//发送方接收控制帧的等待时间
	//流控制<<

	WORD CRTimer;											//接收方接收连续帧的等待时间
}DiagNetCommInfo;

/* \brief
*/
typedef struct
{
	DiagNetMessage* pxDiagNetTxMsg;
	DiagNetMessage xDiagNetRxMsg;
	DiagNetCommInfo xDiagNetCommInfo;

	CanFrame xCanTxMsg;
}DiagNetInfo;

/* \brief
*/
DiagNetInfo xDiagNetInfo;

/* \brief
*/
static void vDiagNetWriteTask(void);
static void vDiagNetReadTask(void);
static void vDiagNetWriteFlowControl(enumDiagNetFlowStatus eDiagNetFlowStatus, BYTE BlockSize, BYTE STmin);

/* \brief
*/
void vDiagNetTask(void)
{
	vDiagNetWriteTask();
	vDiagNetReadTask();
}

/* \brief
	参数为接收指针赋值
*/
void vDiagNetTaskInit(DiagNetMessage* pxDiagNetMessage)
{
	xDiagNetInfo.xCanTxMsg.Length  = diagnetDATA_LENGTH_FRAME_MAX;
	xDiagNetInfo.xCanTxMsg.ID[0]  = diagnetCAN_DIAG_TXID_H;
	xDiagNetInfo.xCanTxMsg.ID[1]  = diagnetCAN_DIAG_TXID_L;

	if(NULL != pxDiagNetMessage)
	{
		xDiagNetInfo.pxDiagNetTxMsg = pxDiagNetMessage;
		xDiagNetInfo.xDiagNetCommInfo.TxLengthCount = 0;
		pxDiagNetMessage->Dlc = 0;							//初始化的时候只是初始化指针，不发送
		xDiagNetInfo.xDiagNetCommInfo.eResultStatus = DIAG_NET_RESULT_NULL;

		xDiagNetInfo.xDiagNetCommInfo.isFlowControl = FALSE;
		xDiagNetInfo.xDiagNetCommInfo.isNoWaitFlowControl = FALSE;
		xDiagNetInfo.xDiagNetCommInfo.STTimer = 0;
	}
}

/* \brief
*/
void vDiagNetTimer(void)
{
	if(0 != xDiagNetInfo.xDiagNetCommInfo.STTimer)
	{
		--xDiagNetInfo.xDiagNetCommInfo.STTimer;
	}

	if((!xDiagNetInfo.xDiagNetCommInfo.isNoWaitFlowControl) && (0 != xDiagNetInfo.xDiagNetCommInfo.BSTimer))
	{
		if(0 == --xDiagNetInfo.xDiagNetCommInfo.BSTimer)
		{	//等待控制帧超时，发送结束
			xDiagNetInfo.xDiagNetCommInfo.eResultStatus = DIAG_NET_RESULT_BS_TIMEOUT_ERROR;
			xDiagNetInfo.xDiagNetCommInfo.TxLengthCount = xDiagNetInfo.pxDiagNetTxMsg->Dlc;
			xDiagNetInfo.xDiagNetCommInfo.isFlowControl = FALSE;
			//流控制
			if(xDiagNetInfo.xDiagNetCommInfo.isFlowControl)
			{
				xDiagNetInfo.xDiagNetCommInfo.isFlowControl = FALSE;
			}
		}
	}

	if(0 != xDiagNetInfo.xDiagNetCommInfo.CRTimer)
	{
		if(0 == --xDiagNetInfo.xDiagNetCommInfo.CRTimer)
		{	//等待连续帧超时，接收结束
			xDiagNetInfo.xDiagNetCommInfo.eReadErrorStatus = DIAG_NET_READ_ERROR_WAIT_CF_TIMEOUT;
			xDiagNetInfo.xDiagNetCommInfo.RxLengthCount = xDiagNetInfo.xDiagNetRxMsg.Dlc;
		}
	}
	
	return;
}


/* \brief
	在发送新帧前未判断当前帧是否发送完毕?
	当前已经测试过，所以暂时不添加
*/
void vDiagNetWrite(DiagNetMessage* pxDiagNetMessage)
{
	if(NULL == pxDiagNetMessage)
	{
		return;
	}

	xDiagNetInfo.pxDiagNetTxMsg = pxDiagNetMessage;
	xDiagNetInfo.xDiagNetCommInfo.TxLengthCount = 0;
	xDiagNetInfo.xDiagNetCommInfo.eResultStatus = DIAG_NET_RESULT_NULL;

	xDiagNetInfo.xDiagNetCommInfo.isFlowControl = FALSE;
	xDiagNetInfo.xDiagNetCommInfo.isNoWaitFlowControl = FALSE;
	xDiagNetInfo.xDiagNetCommInfo.STTimer = 0;

	return;
}

/* \brief
*/
DiagNetMessage* pxDiagNetRead(void)
{
	if(xDiagNetInfo.xDiagNetCommInfo.isRxMsg)
	{
		xDiagNetInfo.xDiagNetCommInfo.isRxMsg = FALSE;

		return &xDiagNetInfo.xDiagNetRxMsg;
	}
	return NULL;
}

/* \brief
*/
enumDiagNetWriteResultStatus eGetDiagNetWriteResult(void)
{
	if(NULL == xDiagNetInfo.pxDiagNetTxMsg)
	{	//未启动发送默认发送为完成状态，准备好发送下帧
		return DIAG_NET_RESULT_OK;
	}

	return xDiagNetInfo.xDiagNetCommInfo.eResultStatus;
}

/* \brief
	BOOL isFunc: TRUE功能寻址，FALSE物理寻址
*/
void vDiagNetMessageHandle(CanFrame* pxCanMsg, BOOL isFunc)
{
	BYTE i=0;

	if(pxCanMsg->Length < 8)
	{	//长度不够
		
		//发送接收结束
		xDiagNetInfo.xDiagNetCommInfo.TxLengthCount = xDiagNetInfo.pxDiagNetTxMsg->Dlc;
		xDiagNetInfo.xDiagNetCommInfo.RxLengthCount = xDiagNetInfo.xDiagNetRxMsg.Dlc;
		xDiagNetInfo.xDiagNetCommInfo.isFlowControl = FALSE;
		return;
	}
	
	switch(pxCanMsg->Data[0]&0xf0)
	{
		case DIAG_NET_SINGLE_FRAME:
			if(xDiagNetInfo.xDiagNetCommInfo.TxLengthCount < xDiagNetInfo.pxDiagNetTxMsg->Dlc)
			{	//没发送完成不接收单帧数据
				break;
			}

			xDiagNetInfo.xDiagNetRxMsg.Dlc = pxCanMsg->Data[0]&0x0f;
			if((0 == xDiagNetInfo.xDiagNetRxMsg.Dlc) || (8 <= xDiagNetInfo.xDiagNetRxMsg.Dlc))
			{	//长度不满足协议直接退出
				break;
			}
			
			xDiagNetInfo.xDiagNetRxMsg.isFunc = isFunc;
			for(i=0; (i!=xDiagNetInfo.xDiagNetRxMsg.Dlc)&&(i!=(diagnetDATA_LENGTH_FRAME_MAX-1)); ++i)
			{
				xDiagNetInfo.xDiagNetRxMsg.Data[i] = pxCanMsg->Data[1+i];
			}

			xDiagNetInfo.xDiagNetCommInfo.isRxMsg = TRUE;
			break;
			
		case DIAG_NET_FIRST_FRAME:
			xDiagNetInfo.xDiagNetRxMsg.Dlc = pxCanMsg->Data[0]&0x0f;
			xDiagNetInfo.xDiagNetRxMsg.Dlc <<= 8;
			xDiagNetInfo.xDiagNetRxMsg.Dlc |= pxCanMsg->Data[1];

			if(8 > xDiagNetInfo.xDiagNetRxMsg.Dlc)
			{	//长度不满足协议直接退出
				break;
			}

			xDiagNetInfo.xDiagNetRxMsg.isFunc = isFunc;

			if(xDiagNetInfo.xDiagNetRxMsg.isFunc)
			{	//功能寻址不支持连续帧
				xDiagNetInfo.xDiagNetRxMsg.isFunc = FALSE;
				break;
			}

			xDiagNetInfo.xDiagNetCommInfo.RxLengthCount = 0;
			xDiagNetInfo.xDiagNetCommInfo.RxSerialNo = 1;
			xDiagNetInfo.xDiagNetCommInfo.eReadErrorStatus = DIAG_NET_READ_ERROR_NULL;

			for(i=0; i!=(diagnetDATA_LENGTH_FRAME_MAX-2); ++i)
			{
				xDiagNetInfo.xDiagNetRxMsg.Data[xDiagNetInfo.xDiagNetCommInfo.RxLengthCount++] = pxCanMsg->Data[2+i];
			}
			vDiagNetWriteFlowControl(DIAG_NET_FS_CONTINE_TO_SEND, 0, 0);//diagnetBLOCK_SIZE_MAX, diagnetSTMIN);
			xDiagNetInfo.xDiagNetCommInfo.RxBlockSizeCount = 0;
			xDiagNetInfo.xDiagNetCommInfo.CRTimer = diagnetCR_TIMEOUT;
			break;
			
		case DIAG_NET_CONSECUTIVE_FRAME:
			if(xDiagNetInfo.xDiagNetCommInfo.RxLengthCount >= xDiagNetInfo.xDiagNetRxMsg.Dlc)
			{	//接收结束，此时再接收到连续帧不进行保存
				break;
			}

			if(xDiagNetInfo.xDiagNetRxMsg.isFunc != isFunc)
			{	//接收到的连续报文是功能寻址跟物理寻址错乱了
				xDiagNetInfo.xDiagNetCommInfo.eReadErrorStatus = DIAG_NET_READ_ERROR_PHY_FUNC;
				xDiagNetInfo.xDiagNetCommInfo.RxLengthCount = xDiagNetInfo.xDiagNetRxMsg.Dlc;
				xDiagNetInfo.xDiagNetCommInfo.isFlowControl = FALSE;
				break;
			}

			//检测接收序号是否正确
			if(xDiagNetInfo.xDiagNetCommInfo.RxSerialNo != (pxCanMsg->Data[0]&0x0f))
			{	//报错，接收结束
				xDiagNetInfo.xDiagNetCommInfo.eReadErrorStatus = DIAG_NET_READ_ERROR_SN;
				xDiagNetInfo.xDiagNetCommInfo.RxLengthCount = xDiagNetInfo.xDiagNetRxMsg.Dlc;
				break;
			}
			if(xDiagNetInfo.xDiagNetCommInfo.RxSerialNo < diagnetTX_SERIAL_NO_MAX)
			{
				xDiagNetInfo.xDiagNetCommInfo.RxSerialNo++;
			}
			else
			{
				xDiagNetInfo.xDiagNetCommInfo.RxSerialNo = 0;
			}
			
			for(i=0; (i!=(diagnetDATA_LENGTH_FRAME_MAX-1))&&(xDiagNetInfo.xDiagNetCommInfo.RxLengthCount != xDiagNetInfo.xDiagNetRxMsg.Dlc); ++i)
			{
				xDiagNetInfo.xDiagNetRxMsg.Data[xDiagNetInfo.xDiagNetCommInfo.RxLengthCount++] = pxCanMsg->Data[1+i];
			}
			
			if(xDiagNetInfo.xDiagNetCommInfo.RxLengthCount == xDiagNetInfo.xDiagNetRxMsg.Dlc)
			{
				xDiagNetInfo.xDiagNetCommInfo.isRxMsg = TRUE;
			}
			else
			{
				++xDiagNetInfo.xDiagNetCommInfo.RxBlockSizeCount;
				xDiagNetInfo.xDiagNetCommInfo.CRTimer = diagnetCR_TIMEOUT;
//				if(++xDiagNetInfo.xDiagNetCommInfo.RxBlockSizeCount == diagnetBLOCK_SIZE_MAX)
//				{
//					vDiagNetWriteFlowControl(DIAG_NET_FS_CONTINE_TO_SEND, diagnetBLOCK_SIZE_MAX, diagnetSTMIN);
//					xDiagNetInfo.xDiagNetCommInfo.RxBlockSizeCount = 0;
//				}
			}
			break;
			
		case DIAG_NET_FLOW_CONTROL_FRAME:
/*			if(xDiagNetInfo.xDiagNetRxMsg.isFunc != isFunc)
			{	//接收到的流控制帧是功能寻址跟物理寻址错乱了
				xDiagNetInfo.xDiagNetCommInfo.eReadErrorStatus = DIAG_NET_READ_ERROR_PHY_FUNC;
				xDiagNetInfo.xDiagNetCommInfo.RxLengthCount = xDiagNetInfo.xDiagNetRxMsg.Dlc;
				xDiagNetInfo.xDiagNetCommInfo.isFlowControl = FALSE;
				break;
			}*/

			if(isFunc)
			{	//接收到的流控制帧必须是物理寻址
				xDiagNetInfo.xDiagNetCommInfo.eReadErrorStatus = DIAG_NET_READ_ERROR_PHY_FUNC;
				xDiagNetInfo.xDiagNetCommInfo.RxLengthCount = xDiagNetInfo.xDiagNetRxMsg.Dlc;
				xDiagNetInfo.xDiagNetCommInfo.isFlowControl = FALSE;
				break;
			}
			
			if(xDiagNetInfo.xDiagNetCommInfo.isFlowControl 
				&& ((DIAG_NET_FS_WAIT != xDiagNetInfo.xDiagNetCommInfo.eFlowStatus) && (!xDiagNetInfo.xDiagNetCommInfo.isNoWaitFlowControl) && (0 != xDiagNetInfo.xDiagNetCommInfo.BlockSize))
				&& (xDiagNetInfo.xDiagNetCommInfo.TxLengthCount < xDiagNetInfo.pxDiagNetTxMsg->Dlc))
			{	//在已接收到流控制帧并且不需要等待流控制帧或者当前Block未发送完成
				//在未完全发送完成时不接收流控制帧
				break;
			}

			//已接收到控制帧
			xDiagNetInfo.xDiagNetCommInfo.BSTimer = 0;	//启动等待控制帧计时
			
			xDiagNetInfo.xDiagNetCommInfo.eFlowStatus = pxCanMsg->Data[0]&0x0f;
			if(DIAG_NET_FS_WAIT == xDiagNetInfo.xDiagNetCommInfo.eFlowStatus)
			{
				xDiagNetInfo.xDiagNetCommInfo.BSTimer = diagnetBS_TIMEOUT;	//启动等待控制帧计时
				break;
			}
			else if((DIAG_NET_FS_OVERFLOW < xDiagNetInfo.xDiagNetCommInfo.eFlowStatus)
				|| ((DIAG_NET_FS_OVERFLOW == xDiagNetInfo.xDiagNetCommInfo.eFlowStatus) && (!xDiagNetInfo.xDiagNetCommInfo.isFlowControl)))
			{	//接收方出错结束
				xDiagNetInfo.xDiagNetCommInfo.eResultStatus = DIAG_NET_RESULT_STATUS_ERROR;
				xDiagNetInfo.xDiagNetCommInfo.TxLengthCount = xDiagNetInfo.pxDiagNetTxMsg->Dlc;
				//流控制
				xDiagNetInfo.xDiagNetCommInfo.isFlowControl = FALSE;

				break;
			}

			xDiagNetInfo.xDiagNetCommInfo.isFlowControl = TRUE;
			//流控制帧后也要ST延时
			xDiagNetInfo.xDiagNetCommInfo.STTimer = xDiagNetInfo.xDiagNetCommInfo.STmin;

			if(0 == pxCanMsg->Data[1])
			{
				xDiagNetInfo.xDiagNetCommInfo.isNoWaitFlowControl = TRUE;
			}
			else if(diagnetBLOCK_SIZE_MAX >= pxCanMsg->Data[1])
			{
				xDiagNetInfo.xDiagNetCommInfo.isNoWaitFlowControl = FALSE;
				xDiagNetInfo.xDiagNetCommInfo.BlockSize = pxCanMsg->Data[1];
			}
			else
			{
				xDiagNetInfo.xDiagNetCommInfo.isNoWaitFlowControl = FALSE;
				xDiagNetInfo.xDiagNetCommInfo.BlockSize = diagnetBLOCK_SIZE_MAX;
			}
			if(2 <= pxCanMsg->Data[2])
			{
				//连续帧间隔增加10ms，保证STmin满足最小要求
				xDiagNetInfo.xDiagNetCommInfo.STmin = pxCanMsg->Data[2]/10+1;
			}
			else
			{
				xDiagNetInfo.xDiagNetCommInfo.STmin = 0;
			}
			break;

		default:
			break;
	}
}

/* \brief
*/
static void vDiagNetWriteTask(void)
{
	BYTE i=0;
	
	if((NULL == xDiagNetInfo.pxDiagNetTxMsg) || (0 != xDiagNetInfo.xDiagNetCommInfo.STTimer))
	{
		return;
	}
	
	if(xDiagNetInfo.xDiagNetCommInfo.TxLengthCount < xDiagNetInfo.pxDiagNetTxMsg->Dlc)
	{	//发送正在进行
		if(xDiagNetInfo.pxDiagNetTxMsg->Dlc < diagnetDATA_LENGTH_FRAME_MAX)
		{	//单帧
			xDiagNetInfo.xCanTxMsg.Data[0] = DIAG_NET_SINGLE_FRAME|(BYTE)xDiagNetInfo.pxDiagNetTxMsg->Dlc;

			for(i=1; i!= diagnetDATA_LENGTH_FRAME_MAX; ++i)
			{
				if(xDiagNetInfo.xDiagNetCommInfo.TxLengthCount < xDiagNetInfo.pxDiagNetTxMsg->Dlc)
				{
					xDiagNetInfo.xCanTxMsg.Data[i] = xDiagNetInfo.pxDiagNetTxMsg->Data[i-1];
					++xDiagNetInfo.xDiagNetCommInfo.TxLengthCount;
				}
				else
				{
					xDiagNetInfo.xCanTxMsg.Data[i] = diagnetFILL_BYTE;
				}
			}
			
			vCanWrite(&xDiagNetInfo.xCanTxMsg);
		}
		else
		{	//启动多帧发送
			if(0 == xDiagNetInfo.xDiagNetCommInfo.TxLengthCount)
			{		//首帧
				xDiagNetInfo.xCanTxMsg.Data[0] = DIAG_NET_FIRST_FRAME|((xDiagNetInfo.pxDiagNetTxMsg->Dlc>>8)&0x0f);
				xDiagNetInfo.xCanTxMsg.Data[1] = xDiagNetInfo.pxDiagNetTxMsg->Dlc&0xff;

				for(i=2; i!= diagnetDATA_LENGTH_FRAME_MAX; ++i)
				{
					xDiagNetInfo.xCanTxMsg.Data[i] = xDiagNetInfo.pxDiagNetTxMsg->Data[i-2];
					++xDiagNetInfo.xDiagNetCommInfo.TxLengthCount;
				}
				
				vCanWrite(&xDiagNetInfo.xCanTxMsg);

				xDiagNetInfo.xDiagNetCommInfo.TxSerialNo = 1;		//首帧的序号为0,下一帧为1
				xDiagNetInfo.xDiagNetCommInfo.BSTimer = diagnetBS_TIMEOUT;	//启动等待控制帧计时
			}
			else
			{		//连续帧
				if(xDiagNetInfo.xDiagNetCommInfo.isFlowControl)
				{	//启动流控制
					if((DIAG_NET_FS_WAIT == xDiagNetInfo.xDiagNetCommInfo.eFlowStatus)
						|| ((0 == xDiagNetInfo.xDiagNetCommInfo.BlockSize)&&(!xDiagNetInfo.xDiagNetCommInfo.isNoWaitFlowControl)))
					{	//跳出等待恢复
						return;
					}

//					if(DIAG_NET_FS_OVERFLOW == xDiagNetInfo.xDiagNetCommInfo.eFlowStatus)
//					{	//接收方溢出出错结束
//						xDiagNetInfo.xDiagNetCommInfo.eResultStatus = DIAG_NET_RESULT_OVERFLOW_ERROR;
//						xDiagNetInfo.xDiagNetCommInfo.TxLengthCount = xDiagNetInfo.pxDiagNetTxMsg->Dlc;
						//流控制
//						xDiagNetInfo.xDiagNetCommInfo.isFlowControl = FALSE;

//						return;
//					}
					xDiagNetInfo.xCanTxMsg.Data[0] = DIAG_NET_CONSECUTIVE_FRAME|xDiagNetInfo.xDiagNetCommInfo.TxSerialNo;

					for(i=1; i!= diagnetDATA_LENGTH_FRAME_MAX; ++i)
					{
						if(xDiagNetInfo.xDiagNetCommInfo.TxLengthCount < xDiagNetInfo.pxDiagNetTxMsg->Dlc)
						{
							xDiagNetInfo.xCanTxMsg.Data[i] = xDiagNetInfo.pxDiagNetTxMsg->Data[xDiagNetInfo.xDiagNetCommInfo.TxLengthCount];
							++xDiagNetInfo.xDiagNetCommInfo.TxLengthCount;
						}
						else
						{
							xDiagNetInfo.xCanTxMsg.Data[i] = diagnetFILL_BYTE;
						}
					}
					
					vCanWrite(&xDiagNetInfo.xCanTxMsg);

					if(xDiagNetInfo.xDiagNetCommInfo.TxLengthCount == xDiagNetInfo.pxDiagNetTxMsg->Dlc)
					{
						xDiagNetInfo.xDiagNetCommInfo.eResultStatus = DIAG_NET_RESULT_OK;
						
						//流控制
						xDiagNetInfo.xDiagNetCommInfo.isFlowControl = FALSE;
					}
					else
					{
						if(xDiagNetInfo.xDiagNetCommInfo.TxSerialNo < diagnetTX_SERIAL_NO_MAX)
						{
							++xDiagNetInfo.xDiagNetCommInfo.TxSerialNo;
						}
						else
						{	//>15从0开始计
							xDiagNetInfo.xDiagNetCommInfo.TxSerialNo = 0;
						}
						
						//流控制间隔时间
						if(!xDiagNetInfo.xDiagNetCommInfo.isNoWaitFlowControl)
						{
							if(0 == --xDiagNetInfo.xDiagNetCommInfo.BlockSize)
							{
								xDiagNetInfo.xDiagNetCommInfo.BSTimer = diagnetBS_TIMEOUT;	//启动等待控制帧计时
							}
						}
						xDiagNetInfo.xDiagNetCommInfo.STTimer = xDiagNetInfo.xDiagNetCommInfo.STmin;
					}
				}
			}
		}
	}
}

/* \brief
*/
static void vDiagNetReadTask(void)
{
}

/* \brief
*/
static void vDiagNetWriteFlowControl(enumDiagNetFlowStatus eDiagNetFlowStatus, BYTE BlockSize, BYTE STmin)
{
	BYTE i=0;

	if(xDiagNetInfo.xDiagNetCommInfo.TxLengthCount < xDiagNetInfo.pxDiagNetTxMsg->Dlc)
	{	//发送控制未完成时不能发送控制帧
		return;
	}
	
	xDiagNetInfo.xCanTxMsg.Data[0] = DIAG_NET_FLOW_CONTROL_FRAME|(BYTE)eDiagNetFlowStatus;
	xDiagNetInfo.xCanTxMsg.Data[1] = BlockSize;
	xDiagNetInfo.xCanTxMsg.Data[2] = STmin;

	for(i=3; i!= diagnetDATA_LENGTH_FRAME_MAX; ++i)
	{
		xDiagNetInfo.xCanTxMsg.Data[i] = diagnetFILL_BYTE;
	}
	
	vCanWrite(&xDiagNetInfo.xCanTxMsg);
}

