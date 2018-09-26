/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		can.c
**	Abstract:		can driver
**	Creat By:		Zeno Liu
**	Creat Time:		2015.03.11
**	Version:		v1.0
**
******************************************************************************/

/* \brief
	include
*/
#include "can.h"
#include "list.h"
#include "Error.h"
#include "common.h"
#include "Debug.h"
#include "io.h"
#include "Apu.h"
#include "DiagAppLayer.h"
#include "DiagDTC.h"
#include "NetworkManager.h"
#include <MC9S08DZ60.h>
#include <hidef.h>

/* \brief
	disable message
*/
//#pragma MESSAGE DISABLE C1420

/* \brief
	marcodefine
*/
#define	canBUFF_SIZE				20
#define	canBUSOFF_DTC_COUNT		2

/* \brief
*/
typedef enum
{
	CAN_BUS_OFF_IDLE,
	CAN_BUS_OFF_REINIT,
	CAN_BUS_OFF_RESEND,
	CAN_BUS_OFF_RECOVERY_CHECK
}CanBusOffState;

/* \brief
*/
typedef struct
{
	CanFrame xCanFrame[canBUFF_SIZE];
	BYTE Front;
	BYTE Rear;
}CanMessageBuff;

/* \brief
*/
static CanMessageBuff xCanReadBuff;
static CanMessageBuff xCanWriteBuff;

static CanBusOffState xCanBusOffState;
static BYTE CanBusoffRecoveryTimer;
static BYTE CanBusoffRecoveryCounter;
static BOOL isCanBusoffRecovery;
static BYTE WritePriority = 0;

/* \brief
*/
static void vCanWriteTask(void);
static void vCanBusoffTask(void);

/* \brief
*/
void vCanTask(void)
{
	vCanBusoffTask();
//	vCanWriteTask();
}

/* \brief
*/
void vCanTaskInit(void)
{
	xCanBusOffState = CAN_BUS_OFF_IDLE;

	CanBusoffRecoveryTimer = 0;
	CanBusoffRecoveryCounter = 0;
	isCanBusoffRecovery = FALSE;
	WritePriority = 0;
}

/* \brief
*/
void vCanTimer(void)
{
	if(0 != CanBusoffRecoveryTimer)
	{
		--CanBusoffRecoveryTimer;
	}
}

/* \brief
*/
void vCanInit(void)
{
	CANCTL0_INITRQ = 1;							//enter initialization status
	if(!blWaitSignalOk(&CANCTL1, 0, TRUE, 500))
	{
		CANCTL0_INITRQ = 1;						//enter initialization status
		(void)blWaitSignalOk(&CANCTL1, 0, TRUE, 500);
	}
	

	CANBTR0 = 0x81;
	CANBTR1 = 0x49;								//125khz

	CANIDAC = 0x10;								// 4 16bit rx filter

	CANIDAR0 = 0x00;
	CANIDAR1 = 0x00;
	CANIDAR2 = 0x00;
	CANIDAR3 = 0x00;
	CANIDAR4 = 0x00;
	CANIDAR5 = 0x00;
	CANIDAR6 = 0x00;
	CANIDAR7 = 0x00;

	CANIDMR0 = 0xff;
	CANIDMR1 = 0xff;
	CANIDMR2 = 0xff;
	CANIDMR3 = 0xff;
	CANIDMR4 = 0xff;
	CANIDMR5 = 0xff;
	CANIDMR6 = 0xff;
	CANIDMR7 = 0xff;

	CANCTL1 = 0x80;
	CANCTL1_BORM = 1;

	CANCTL0_INITRQ = 0;							//exit initialization status
	if(!blWaitSignalOk(&CANCTL1, 0, FALSE, 500))
	{
		CANCTL0_INITRQ = 0;						//exit initialization status
		(void)blWaitSignalOk(&CANCTL1, 0, FALSE, 500);
	}

	(void)blWaitSignalOk(&CANCTL0, 4, TRUE, 1000);		//wait synch

	CANCTL0 |= 0x04;							//can wake up
	CANRIER = 0xc1;//0xc3;
}


/* \brief
*/

void vCanWrite(pCanFrame pxCanMessage)
{
	UINT16 count = 0;

	//bus off时不发送
	if((!CANTFLG) || (0 != CanBusoffRecoveryTimer))
	{       //buffer full
		return;
	}
	
	CANTBSEL = CANTFLG;
	

	CANTIDR0 = pxCanMessage->ID[0];
	CANTIDR1 = pxCanMessage->ID[1];
	
	CANTDSR0 = pxCanMessage->Data[0];
	CANTDSR1 = pxCanMessage->Data[1];
	CANTDSR2 = pxCanMessage->Data[2];
	CANTDSR3 = pxCanMessage->Data[3];
	CANTDSR4 = pxCanMessage->Data[4];
	CANTDSR5 = pxCanMessage->Data[5];
	CANTDSR6 = pxCanMessage->Data[6];
	CANTDSR7 = pxCanMessage->Data[7];
	
	CANTDLR = pxCanMessage->Length;
	
	CANTFLG = CANTBSEL;
	
	while(!(CANTFLG & CANTBSEL))
	{
		if(count++ > 5000)
		{
			break;
		}
	}

}



/* \brief
*/
#if 0
void vCanWrite(pCanFrame pxCanFrame)
{
	if(!isDiagAppCanAppTx())
	{
		xCanWriteBuff.Rear = 
		xCanWriteBuff.Front = 0;
		return;
	}
	
	xCanWriteBuff.xCanFrame[xCanWriteBuff.Rear].ID[0] = pxCanFrame->ID[0];
	xCanWriteBuff.xCanFrame[xCanWriteBuff.Rear].ID[1] = pxCanFrame->ID[1];

	xCanWriteBuff.xCanFrame[xCanWriteBuff.Rear].Length = pxCanFrame->Length;

	xCanWriteBuff.xCanFrame[xCanWriteBuff.Rear].Data[0] = pxCanFrame->Data[0];
	xCanWriteBuff.xCanFrame[xCanWriteBuff.Rear].Data[1] = pxCanFrame->Data[1];
	xCanWriteBuff.xCanFrame[xCanWriteBuff.Rear].Data[2] = pxCanFrame->Data[2];
	xCanWriteBuff.xCanFrame[xCanWriteBuff.Rear].Data[3] = pxCanFrame->Data[3];
	xCanWriteBuff.xCanFrame[xCanWriteBuff.Rear].Data[4] = pxCanFrame->Data[4];
	xCanWriteBuff.xCanFrame[xCanWriteBuff.Rear].Data[5] = pxCanFrame->Data[5];
	xCanWriteBuff.xCanFrame[xCanWriteBuff.Rear].Data[6] = pxCanFrame->Data[6];
	xCanWriteBuff.xCanFrame[xCanWriteBuff.Rear].Data[7] = pxCanFrame->Data[7];

	if(canBUFF_SIZE <= ++xCanWriteBuff.Rear)
	{
		xCanWriteBuff.Rear = 0;
	}

	return;
}

#endif

/* \brief
*/
pCanFrame pxCanRead(void)
{
	if(xCanReadBuff.Front != xCanReadBuff.Rear)
	{
		pCanFrame pxCanReadFrame = &xCanReadBuff.xCanFrame[xCanReadBuff.Front++];

		if(canBUFF_SIZE <= xCanReadBuff.Front)
		{
			xCanReadBuff.Front = 0;
		}
		return pxCanReadFrame;
	}
	else
	{
		return NULL;
	}
}

/* \brief
*/
BOOL isCanBusOff(void)
{
	if((CAN_BUS_OFF_IDLE == xCanBusOffState) && (!CANMISC_BOHOLD))
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

/* \brief
*/
__interrupt void INTCanTx(void)
{
	CANTIER = 0x00;		//发送完成进入中断，用于bus off处理
	isCanBusoffRecovery = TRUE;
}

/* \brief
*/
__interrupt void INTCanRx(void)
{
	BYTE CanRxFlag = FALSE;

	WORD wCanID = (WORD)(CANRIDR0<<3) | (BYTE)(CANRIDR1>>5);

	if((wCanID >= canID_NM_BASE) && (wCanID <= canID_NM_MAX))
	{	//nm报文
		CanRxFlag = TRUE;
	}
	else
	{
		switch(CANRIDR0)
		{
			case canID_RX_AVM_601_H:
				if(isDiagAppCanAppRx() && (canID_RX_AVM_601_L == CANRIDR1))
				{
					CanRxFlag = TRUE;
				}
				break;

			case canID_RX_AVM_620_H:
				if(isDiagAppCanAppRx() && (canID_RX_AVM_620_L == CANRIDR1))
				{
					CanRxFlag = TRUE;
				}
				break;
				
			case canID_RX_EPS_180_H:
				if(isDiagAppCanAppRx() && (canID_RX_EPS_180_L == CANRIDR1))
				{
					CanRxFlag = TRUE;
				}
				break;
				
			case canID_RX_PAS_550_H:
				if(isDiagAppCanAppRx() && (canID_RX_PAS_550_L == CANRIDR1))
				{
					CanRxFlag = TRUE;
				}
				break;
				
			case canID_RX_BCM_660_H:
				if(isDiagAppCanAppRx() && (canID_RX_BCM_660_L == CANRIDR1))
				{
					CanRxFlag = TRUE;
				}
				break;
				
			case canID_RX_AC_530_H:
				if(isDiagAppCanAppRx() && (canID_RX_AC_530_L == CANRIDR1))
				{
					CanRxFlag = TRUE;
				}
				break;
				
			case canID_RX_BCM_4A0_H:
				if(isDiagAppCanAppRx() && (canID_RX_BCM_4A0_L == CANRIDR1))
				{
					CanRxFlag = TRUE;
				}
				break;
				
			case canID_RX_BCM_5B0_H:
				if(isDiagAppCanAppRx() && (canID_RX_BCM_5B0_L == CANRIDR1))
				{
					CanRxFlag = TRUE;
				}
				break;

			case canID_RX_EPS_3A0_H:
				if(isDiagAppCanAppRx() && (canID_RX_EPS_3A0_L== CANRIDR1))
				{
					CanRxFlag = TRUE;
				}
				break;

			case canID_RX_INS_678_H:
				if(isDiagAppCanAppRx() && (canID_RX_INS_678_L == CANRIDR1))
				{
					CanRxFlag = TRUE;
				}
				break;
				
			case canID_RX_DIAG_720_H:
				if(canID_RX_DIAG_720_L == CANRIDR1)
				{
					CanRxFlag = TRUE;
				}
				break;

			case canID_RX_DIAG_FUNCID_7DF_H:
				if(canID_RX_DIAG_FUNCID_7DF_L == CANRIDR1)
				{
					CanRxFlag = TRUE;
				}
				break;

			
			default:
				break;
		}
	}
	
	if(CanRxFlag)
	{	//0x7e8
		xCanReadBuff.xCanFrame[xCanReadBuff.Rear].ID[0] = CANRIDR0;
		xCanReadBuff.xCanFrame[xCanReadBuff.Rear].ID[1] = CANRIDR1&0xe0;

		xCanReadBuff.xCanFrame[xCanReadBuff.Rear].Length = CANRDLR;

		xCanReadBuff.xCanFrame[xCanReadBuff.Rear].Data[0] = CANRDSR0;
		xCanReadBuff.xCanFrame[xCanReadBuff.Rear].Data[1] = CANRDSR1;
		xCanReadBuff.xCanFrame[xCanReadBuff.Rear].Data[2] = CANRDSR2;
		xCanReadBuff.xCanFrame[xCanReadBuff.Rear].Data[3] = CANRDSR3;
		xCanReadBuff.xCanFrame[xCanReadBuff.Rear].Data[4] = CANRDSR4;
		xCanReadBuff.xCanFrame[xCanReadBuff.Rear].Data[5] = CANRDSR5;
		xCanReadBuff.xCanFrame[xCanReadBuff.Rear].Data[6] = CANRDSR6;
		xCanReadBuff.xCanFrame[xCanReadBuff.Rear].Data[7] = CANRDSR7;

		if(canBUFF_SIZE <= ++xCanReadBuff.Rear)
		{
			xCanReadBuff.Rear = 0;
		}
	}
	
	CANRFLG = 0x01;			//clear flag
}

/* \brief
*/
__interrupt void INTCanErr(void)
{
	++xErrorRecord.CanError;
}

/* \brief
*/
__interrupt void INTCanWu(void)
{
}

/* \brief
*/
static void vCanWriteTask(void)
{
	if(xCanWriteBuff.Rear == xCanWriteBuff.Front)
	{
		return;
	}
	
	//check sync
	if(!(CANCTL0&0x10))
	{
		return;
	}

	if(CANTFLG == 0x07)
	{	// clear priority when T BUFF is empty
		WritePriority = 0;
	}
	
	//select BUFF
	CANTBSEL = CANTFLG;

	CANTIDR0 = xCanWriteBuff.xCanFrame[xCanWriteBuff.Front].ID[0];
	CANTIDR1 = xCanWriteBuff.xCanFrame[xCanWriteBuff.Front].ID[1];
	
	CANTDLR  = xCanWriteBuff.xCanFrame[xCanWriteBuff.Front].Length;

	CANTDSR0 = xCanWriteBuff.xCanFrame[xCanWriteBuff.Front].Data[0];
	CANTDSR1 = xCanWriteBuff.xCanFrame[xCanWriteBuff.Front].Data[1];
	CANTDSR2 = xCanWriteBuff.xCanFrame[xCanWriteBuff.Front].Data[2];
	CANTDSR3 = xCanWriteBuff.xCanFrame[xCanWriteBuff.Front].Data[3];
	CANTDSR4 = xCanWriteBuff.xCanFrame[xCanWriteBuff.Front].Data[4];
	CANTDSR5 = xCanWriteBuff.xCanFrame[xCanWriteBuff.Front].Data[5];
	CANTDSR6 = xCanWriteBuff.xCanFrame[xCanWriteBuff.Front].Data[6];
	CANTDSR7 = xCanWriteBuff.xCanFrame[xCanWriteBuff.Front].Data[7];

	if(canBUFF_SIZE <= ++xCanWriteBuff.Front)
	{
		xCanWriteBuff.Front = 0;
	}

	CANTIDR1 &= ~0x10;
	
	CANTTBPR = WritePriority;
	
	if(WritePriority == 8)
	{
		++xErrorRecord.CanWritePriorityOverflowError;
		
		WritePriority = 0;
	}
	else
	{
		++WritePriority;
	}
	
	CANTFLG  = CANTBSEL;
	CANTIER = CANTBSEL;
	return;
}

/* \brief
*/
static void vCanBusoffTask(void)
{
	switch(xCanBusOffState)
	{
		case CAN_BUS_OFF_IDLE:
			if(CANMISC_BOHOLD)
			{
				vNMBusoff(TRUE);
				CanBusoffRecoveryCounter = 0;
				isCanBusoffRecovery = FALSE;
				xCanBusOffState = CAN_BUS_OFF_REINIT;
			}
			else
			{
				vDiagDTC(DIAG_DTC_CAN_ERROR, DIAG_DTC_CTL_SUCESS);
			}
			break;
			
		case CAN_BUS_OFF_REINIT:
			//终止发送缓冲区
			CANTARQ= 0x07;
			CANTIER = 0x07;		//发送完成进入中断，用于bus off处理
			CANMISC_BOHOLD = 1;
			CanBusoffRecoveryTimer = 100;
			xCanBusOffState = CAN_BUS_OFF_RESEND;
			break;
			
		case CAN_BUS_OFF_RESEND:
			if(0 != CanBusoffRecoveryTimer)
			{
				break;
			}
			vNMBusoffRecovery();
			xCanBusOffState = CAN_BUS_OFF_RECOVERY_CHECK;
			break;
			
		case CAN_BUS_OFF_RECOVERY_CHECK:
			if((!CANMISC_BOHOLD) && isCanBusoffRecovery)
			{
				vNMBusoff(FALSE);
				xCanBusOffState = CAN_BUS_OFF_IDLE;
				break;
			}

			if(canBUSOFF_DTC_COUNT <= ++CanBusoffRecoveryCounter)
			{
				vDiagDTC(DIAG_DTC_CAN_ERROR, DIAG_DTC_CTL_FAIL);
			}
			isCanBusoffRecovery = FALSE;
			xCanBusOffState = CAN_BUS_OFF_REINIT;
			break;

		default:
			xCanBusOffState = CAN_BUS_OFF_IDLE;
			break;
	}
}

