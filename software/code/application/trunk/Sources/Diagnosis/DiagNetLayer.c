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

//������֡����
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
	DIAG_NET_FS_CONTINE_TO_SEND,						//�����ͷ�������������֡
	DIAG_NET_FS_WAIT,										//��ʾ���ͷ���Ҫ�ȴ���һ֡������֡
	DIAG_NET_FS_OVERFLOW									//��ʾ���շ����������
}enumDiagNetFlowStatus;

/* \brief
*/
typedef struct
{
	enumDiagNetWriteResultStatus eResultStatus;					//���ͽ���״̬
	BYTE TxSerialNo;										//����֡���
	WORD TxLengthCount;									//���ͳ��ȼ���
	
	BOOL isRxMsg;											//���յ����ı�־
	WORD RxLengthCount;
	BYTE RxBlockSizeCount;
	BYTE RxSerialNo;
	enumDiagNetReadErrorStatus eReadErrorStatus;

	//������>>
	BOOL isFlowControl;										//��������֡ʱ���յ���������֡
	enumDiagNetFlowStatus eFlowStatus;						//������״̬
	BOOL isNoWaitFlowControl;									//����Ҫ�ȴ�������ֱ֡�ӷ���
	BYTE BlockSize;											//��������������֡��Ŀ
	WORD STmin;											//��������֡�����С���
	WORD STTimer;											//����֡����ʱ���ʱ��
	WORD BSTimer;											//���ͷ����տ���֡�ĵȴ�ʱ��
	//������<<

	WORD CRTimer;											//���շ���������֡�ĵȴ�ʱ��
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
	����Ϊ����ָ�븳ֵ
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
		pxDiagNetMessage->Dlc = 0;							//��ʼ����ʱ��ֻ�ǳ�ʼ��ָ�룬������
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
		{	//�ȴ�����֡��ʱ�����ͽ���
			xDiagNetInfo.xDiagNetCommInfo.eResultStatus = DIAG_NET_RESULT_BS_TIMEOUT_ERROR;
			xDiagNetInfo.xDiagNetCommInfo.TxLengthCount = xDiagNetInfo.pxDiagNetTxMsg->Dlc;
			xDiagNetInfo.xDiagNetCommInfo.isFlowControl = FALSE;
			//������
			if(xDiagNetInfo.xDiagNetCommInfo.isFlowControl)
			{
				xDiagNetInfo.xDiagNetCommInfo.isFlowControl = FALSE;
			}
		}
	}

	if(0 != xDiagNetInfo.xDiagNetCommInfo.CRTimer)
	{
		if(0 == --xDiagNetInfo.xDiagNetCommInfo.CRTimer)
		{	//�ȴ�����֡��ʱ�����ս���
			xDiagNetInfo.xDiagNetCommInfo.eReadErrorStatus = DIAG_NET_READ_ERROR_WAIT_CF_TIMEOUT;
			xDiagNetInfo.xDiagNetCommInfo.RxLengthCount = xDiagNetInfo.xDiagNetRxMsg.Dlc;
		}
	}
	
	return;
}


/* \brief
	�ڷ�����֡ǰδ�жϵ�ǰ֡�Ƿ������?
	��ǰ�Ѿ����Թ���������ʱ�����
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
	{	//δ��������Ĭ�Ϸ���Ϊ���״̬��׼���÷�����֡
		return DIAG_NET_RESULT_OK;
	}

	return xDiagNetInfo.xDiagNetCommInfo.eResultStatus;
}

/* \brief
	BOOL isFunc: TRUE����Ѱַ��FALSE����Ѱַ
*/
void vDiagNetMessageHandle(CanFrame* pxCanMsg, BOOL isFunc)
{
	BYTE i=0;

	if(pxCanMsg->Length < 8)
	{	//���Ȳ���
		
		//���ͽ��ս���
		xDiagNetInfo.xDiagNetCommInfo.TxLengthCount = xDiagNetInfo.pxDiagNetTxMsg->Dlc;
		xDiagNetInfo.xDiagNetCommInfo.RxLengthCount = xDiagNetInfo.xDiagNetRxMsg.Dlc;
		xDiagNetInfo.xDiagNetCommInfo.isFlowControl = FALSE;
		return;
	}
	
	switch(pxCanMsg->Data[0]&0xf0)
	{
		case DIAG_NET_SINGLE_FRAME:
			if(xDiagNetInfo.xDiagNetCommInfo.TxLengthCount < xDiagNetInfo.pxDiagNetTxMsg->Dlc)
			{	//û������ɲ����յ�֡����
				break;
			}

			xDiagNetInfo.xDiagNetRxMsg.Dlc = pxCanMsg->Data[0]&0x0f;
			if((0 == xDiagNetInfo.xDiagNetRxMsg.Dlc) || (8 <= xDiagNetInfo.xDiagNetRxMsg.Dlc))
			{	//���Ȳ�����Э��ֱ���˳�
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
			{	//���Ȳ�����Э��ֱ���˳�
				break;
			}

			xDiagNetInfo.xDiagNetRxMsg.isFunc = isFunc;

			if(xDiagNetInfo.xDiagNetRxMsg.isFunc)
			{	//����Ѱַ��֧������֡
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
			{	//���ս�������ʱ�ٽ��յ�����֡�����б���
				break;
			}

			if(xDiagNetInfo.xDiagNetRxMsg.isFunc != isFunc)
			{	//���յ������������ǹ���Ѱַ������Ѱַ������
				xDiagNetInfo.xDiagNetCommInfo.eReadErrorStatus = DIAG_NET_READ_ERROR_PHY_FUNC;
				xDiagNetInfo.xDiagNetCommInfo.RxLengthCount = xDiagNetInfo.xDiagNetRxMsg.Dlc;
				xDiagNetInfo.xDiagNetCommInfo.isFlowControl = FALSE;
				break;
			}

			//����������Ƿ���ȷ
			if(xDiagNetInfo.xDiagNetCommInfo.RxSerialNo != (pxCanMsg->Data[0]&0x0f))
			{	//�������ս���
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
			{	//���յ���������֡�ǹ���Ѱַ������Ѱַ������
				xDiagNetInfo.xDiagNetCommInfo.eReadErrorStatus = DIAG_NET_READ_ERROR_PHY_FUNC;
				xDiagNetInfo.xDiagNetCommInfo.RxLengthCount = xDiagNetInfo.xDiagNetRxMsg.Dlc;
				xDiagNetInfo.xDiagNetCommInfo.isFlowControl = FALSE;
				break;
			}*/

			if(isFunc)
			{	//���յ���������֡����������Ѱַ
				xDiagNetInfo.xDiagNetCommInfo.eReadErrorStatus = DIAG_NET_READ_ERROR_PHY_FUNC;
				xDiagNetInfo.xDiagNetCommInfo.RxLengthCount = xDiagNetInfo.xDiagNetRxMsg.Dlc;
				xDiagNetInfo.xDiagNetCommInfo.isFlowControl = FALSE;
				break;
			}
			
			if(xDiagNetInfo.xDiagNetCommInfo.isFlowControl 
				&& ((DIAG_NET_FS_WAIT != xDiagNetInfo.xDiagNetCommInfo.eFlowStatus) && (!xDiagNetInfo.xDiagNetCommInfo.isNoWaitFlowControl) && (0 != xDiagNetInfo.xDiagNetCommInfo.BlockSize))
				&& (xDiagNetInfo.xDiagNetCommInfo.TxLengthCount < xDiagNetInfo.pxDiagNetTxMsg->Dlc))
			{	//���ѽ��յ�������֡���Ҳ���Ҫ�ȴ�������֡���ߵ�ǰBlockδ�������
				//��δ��ȫ�������ʱ������������֡
				break;
			}

			//�ѽ��յ�����֡
			xDiagNetInfo.xDiagNetCommInfo.BSTimer = 0;	//�����ȴ�����֡��ʱ
			
			xDiagNetInfo.xDiagNetCommInfo.eFlowStatus = pxCanMsg->Data[0]&0x0f;
			if(DIAG_NET_FS_WAIT == xDiagNetInfo.xDiagNetCommInfo.eFlowStatus)
			{
				xDiagNetInfo.xDiagNetCommInfo.BSTimer = diagnetBS_TIMEOUT;	//�����ȴ�����֡��ʱ
				break;
			}
			else if((DIAG_NET_FS_OVERFLOW < xDiagNetInfo.xDiagNetCommInfo.eFlowStatus)
				|| ((DIAG_NET_FS_OVERFLOW == xDiagNetInfo.xDiagNetCommInfo.eFlowStatus) && (!xDiagNetInfo.xDiagNetCommInfo.isFlowControl)))
			{	//���շ��������
				xDiagNetInfo.xDiagNetCommInfo.eResultStatus = DIAG_NET_RESULT_STATUS_ERROR;
				xDiagNetInfo.xDiagNetCommInfo.TxLengthCount = xDiagNetInfo.pxDiagNetTxMsg->Dlc;
				//������
				xDiagNetInfo.xDiagNetCommInfo.isFlowControl = FALSE;

				break;
			}

			xDiagNetInfo.xDiagNetCommInfo.isFlowControl = TRUE;
			//������֡��ҲҪST��ʱ
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
				//����֡�������10ms����֤STmin������СҪ��
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
	{	//�������ڽ���
		if(xDiagNetInfo.pxDiagNetTxMsg->Dlc < diagnetDATA_LENGTH_FRAME_MAX)
		{	//��֡
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
		{	//������֡����
			if(0 == xDiagNetInfo.xDiagNetCommInfo.TxLengthCount)
			{		//��֡
				xDiagNetInfo.xCanTxMsg.Data[0] = DIAG_NET_FIRST_FRAME|((xDiagNetInfo.pxDiagNetTxMsg->Dlc>>8)&0x0f);
				xDiagNetInfo.xCanTxMsg.Data[1] = xDiagNetInfo.pxDiagNetTxMsg->Dlc&0xff;

				for(i=2; i!= diagnetDATA_LENGTH_FRAME_MAX; ++i)
				{
					xDiagNetInfo.xCanTxMsg.Data[i] = xDiagNetInfo.pxDiagNetTxMsg->Data[i-2];
					++xDiagNetInfo.xDiagNetCommInfo.TxLengthCount;
				}
				
				vCanWrite(&xDiagNetInfo.xCanTxMsg);

				xDiagNetInfo.xDiagNetCommInfo.TxSerialNo = 1;		//��֡�����Ϊ0,��һ֡Ϊ1
				xDiagNetInfo.xDiagNetCommInfo.BSTimer = diagnetBS_TIMEOUT;	//�����ȴ�����֡��ʱ
			}
			else
			{		//����֡
				if(xDiagNetInfo.xDiagNetCommInfo.isFlowControl)
				{	//����������
					if((DIAG_NET_FS_WAIT == xDiagNetInfo.xDiagNetCommInfo.eFlowStatus)
						|| ((0 == xDiagNetInfo.xDiagNetCommInfo.BlockSize)&&(!xDiagNetInfo.xDiagNetCommInfo.isNoWaitFlowControl)))
					{	//�����ȴ��ָ�
						return;
					}

//					if(DIAG_NET_FS_OVERFLOW == xDiagNetInfo.xDiagNetCommInfo.eFlowStatus)
//					{	//���շ�����������
//						xDiagNetInfo.xDiagNetCommInfo.eResultStatus = DIAG_NET_RESULT_OVERFLOW_ERROR;
//						xDiagNetInfo.xDiagNetCommInfo.TxLengthCount = xDiagNetInfo.pxDiagNetTxMsg->Dlc;
						//������
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
						
						//������
						xDiagNetInfo.xDiagNetCommInfo.isFlowControl = FALSE;
					}
					else
					{
						if(xDiagNetInfo.xDiagNetCommInfo.TxSerialNo < diagnetTX_SERIAL_NO_MAX)
						{
							++xDiagNetInfo.xDiagNetCommInfo.TxSerialNo;
						}
						else
						{	//>15��0��ʼ��
							xDiagNetInfo.xDiagNetCommInfo.TxSerialNo = 0;
						}
						
						//�����Ƽ��ʱ��
						if(!xDiagNetInfo.xDiagNetCommInfo.isNoWaitFlowControl)
						{
							if(0 == --xDiagNetInfo.xDiagNetCommInfo.BlockSize)
							{
								xDiagNetInfo.xDiagNetCommInfo.BSTimer = diagnetBS_TIMEOUT;	//�����ȴ�����֡��ʱ
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
	{	//���Ϳ���δ���ʱ���ܷ��Ϳ���֡
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

