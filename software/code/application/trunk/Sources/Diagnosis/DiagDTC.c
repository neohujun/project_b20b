/******************************************************************************
**
**								Copyright (C) 2017  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		DiagDTC.c
**	Abstract:		diagnosis DTC
**	Creat By:		Zeno Liu
**	Creat Time:		2017.03.24
**	Version:		v1.0
**
******************************************************************************/


/* \brief
	include files
*/
#include "DiagDTC.h"

#include "DiagNetLayer.h"
#include "Memory.h"
#include "Eeprom.h"
#include "DiagAppLayer.h"

/* \brief
*/
#define	diagdtcSTATUS_INIT			0x10
#define	diagdtcSTATUS_CYCLE_CLEAR	0x39		//驾驶循环开始需要清空的位(为0的位被清空)
#define	diagdtcPOWER_ON_DELAY	500			//开机5s后再检查DTC

/* \brief
*/
typedef union
{
	BYTE Byte;
	struct
	{
		unsigned TestFail									:1;		//测试失效
		unsigned TestFailThisMonitoringCycle				:1;		//本次驾驶周期内失败
		unsigned PendingDTC								:1;		//等待DTC
		unsigned ConfirmedDTC							:1;		//确认DTC
		
		unsigned TestNotCompletedSinceLastClear			:1;		//上次清零后测试未完成
		unsigned TestFailedSinceLastClear					:1;		//上次清零后测试失效
		unsigned TestNotCompletedThisMonitoringCycle		:1;		//本次驾驶周期中测试未完成
		unsigned WarningIndicatorRequested				:1;		//警告指示位请求
	}Bits;
}DiagDTCStatus;

/* \brief
*/
typedef struct
{
	BYTE DTC[DIAG_DTC_NUM][3];
	DiagDTCStatus uStatus[DIAG_DTC_NUM];

	BYTE Chesum;
}DiagDTC;

/* \brief
*/
static const BYTE DiagDTCInit[DIAG_DTC_NUM][3]=
{
	0xC0,0x73,0x00,
	0xD7,0x01,0x87,
	0xE2,0x0D,0x00,
	0xC1,0x40,0x00,
	0xC1,0x00,0x00,
//	0xC1,0x01,0x00,
	0xC1,0x55,0x00,
	0x91,0x17,0x17,
	0x91,0x17,0x16,
};

/* \brief
*/
typedef struct
{
	DiagDTC xDiagDTC;

	BOOL isDtcClear;
	
	WORD wDiagDtcDelayTimer;
}DiagDTCInfo;

/* \brief
*/
static DiagDTCInfo xDiagDTCInfo;

/* \brief
*/
static void vDiagDTCConfirmWrite(void);
static void vDiagDTCStatusReadAll(void);

/* \brief
*/
void vDiagDTCInit(void)
{
	BYTE i=0;

	vDiagDTCStatusReadAll();

	for(i=0; i!=DIAG_DTC_NUM;++i)
	{
		xDiagDTCInfo.xDiagDTC.DTC[i][0] = DiagDTCInit[i][0];
		xDiagDTCInfo.xDiagDTC.DTC[i][1] = DiagDTCInit[i][1];
		xDiagDTCInfo.xDiagDTC.DTC[i][2] = DiagDTCInit[i][2];
		
		xDiagDTCInfo.xDiagDTC.uStatus[i].Byte &= diagdtcSTATUS_CYCLE_CLEAR;
	}

	xDiagDTCInfo.wDiagDtcDelayTimer = diagdtcPOWER_ON_DELAY;
}

/* \brief
*/
void vDiagDTCTimer(void)
{
	if(0 != xDiagDTCInfo.wDiagDtcDelayTimer)
	{
		--xDiagDTCInfo.wDiagDtcDelayTimer;
	}

	if(xDiagDTCInfo.isDtcClear)
	{
		vDiagDTCConfirmWrite();
		
		xDiagDTCInfo.isDtcClear = FALSE;
	}
}

/* \brief
*/
void vDiagDTC(enumDiagDTC eDTC, enumDiagDTCCtl eDTCCtl)
{
	BOOL isWriteStatus = FALSE;
	if(!isDiagAppDtcOn() || (0 != xDiagDTCInfo.wDiagDtcDelayTimer))
	{
		return;
	}
	
	switch(eDTCCtl)
	{
		case DIAG_DTC_CTL_START:
			break;
			
		case DIAG_DTC_CTL_FAIL:
			xDiagDTCInfo.xDiagDTC.uStatus[eDTC].Bits.TestFail = 1;
			xDiagDTCInfo.xDiagDTC.uStatus[eDTC].Bits.TestFailThisMonitoringCycle = 1;

			if(xDiagDTCInfo.xDiagDTC.uStatus[eDTC].Bits.TestNotCompletedSinceLastClear)
			{
				xDiagDTCInfo.xDiagDTC.uStatus[eDTC].Bits.TestNotCompletedSinceLastClear = 0;
				isWriteStatus = TRUE;
			}
			if(!xDiagDTCInfo.xDiagDTC.uStatus[eDTC].Bits.TestFailedSinceLastClear)
			{
				xDiagDTCInfo.xDiagDTC.uStatus[eDTC].Bits.TestFailedSinceLastClear = 1;
				isWriteStatus = TRUE;
			}
			if(!xDiagDTCInfo.xDiagDTC.uStatus[eDTC].Bits.ConfirmedDTC)
			{
				xDiagDTCInfo.xDiagDTC.uStatus[eDTC].Bits.ConfirmedDTC = 1;
				isWriteStatus = TRUE;
			}
			break;
			
		case DIAG_DTC_CTL_SUCESS:
			xDiagDTCInfo.xDiagDTC.uStatus[eDTC].Bits.TestFail = 0;
			if(xDiagDTCInfo.xDiagDTC.uStatus[eDTC].Bits.TestNotCompletedSinceLastClear)
			{
				xDiagDTCInfo.xDiagDTC.uStatus[eDTC].Bits.TestNotCompletedSinceLastClear = 0;
				isWriteStatus = TRUE;
			}
			break;
			
		case DIAG_DTC_CTL_CONFIG:
			break;

		case DIAG_DTC_CTL_STOP:
			break;

		default:
			break;
	}

	if(isWriteStatus)
	{
		vDiagDTCConfirmWrite();
	}
}

/* \brief
*/
BOOL blDiagDTCClear(BYTE DTC[3])
{
	BOOL re = FALSE;

	BYTE i=0;

	if((0xff == DTC[0]) && (0xff == DTC[1]) && (0xff == DTC[2]))
	{	//清除所有群组的DTC
		for(i=0; i!=DIAG_DTC_NUM;++i)
		{
			xDiagDTCInfo.xDiagDTC.uStatus[i].Byte = diagdtcSTATUS_INIT;
		}
		//清空DTC超时导致发送肯定应答太慢，所有放到主循环清空
		xDiagDTCInfo.isDtcClear = TRUE;
		
		re = TRUE;
	}
#if	0
	if((0x80 == DTC[0]) && (0x00 == DTC[1]) && (0x00 == DTC[2]))
	{	//清除所有群组的DTC
		for(i=DIAG_DTC_VOLTAGE_HIGH_EXCEPTION; i!=DIAG_DTC_NUM;++i)
		{
			xDiagDTCInfo.xDiagDTC.uStatus[i].Byte = diagdtcSTATUS_INIT;
		}

		DiagDtcConfirm &= ~0x3f800;
		_SendMsgToEEP(MS_EEP_WRITE_DTC_CONFIRM, 0, (BYTE*)&DiagDtcConfirm);
		
		re = TRUE;
	}
	if((0xC0 == DTC[0]) && (0x00 == DTC[1]) && (0x00 == DTC[2]))
	{	//清除所有群组的DTC
		for(i=DIAG_DTC_CAN_ERROR; i!=DIAG_DTC_VOLTAGE_HIGH_EXCEPTION;++i)
		{
			xDiagDTCInfo.xDiagDTC.uStatus[i].Byte = diagdtcSTATUS_INIT;
		}

		DiagDtcConfirm &= ~0x7ff;
		_SendMsgToEEP(MS_EEP_WRITE_DTC_CONFIRM, 0, (BYTE*)&DiagDtcConfirm);
		
		re = TRUE;
	}
#endif
	
	return re;
}

/* \brief
*/
WORD getDiagDTCNum(BYTE Mask)
{
	BYTE i=0;
	WORD Num=0;

	for(i=0; i!=DIAG_DTC_NUM;++i)
	{
		if(xDiagDTCInfo.xDiagDTC.uStatus[i].Byte&Mask)
		{
			++Num;
		}
	}

	return Num;
}

/* \brief
*/
BYTE getDiagDTC(BYTE Mask, BYTE DTCData[])
{
	BYTE i=0;
	BYTE Num=0;

	for(i=0; (i!=DIAG_DTC_NUM)&&(i!=((diagnetMESSAGE_MAX_LENGTH-3)/4));++i)
	{
		if(xDiagDTCInfo.xDiagDTC.uStatus[i].Byte&Mask)
		{
			DTCData[Num] = xDiagDTCInfo.xDiagDTC.DTC[i][0];
			DTCData[Num+1] = xDiagDTCInfo.xDiagDTC.DTC[i][1];
			DTCData[Num+2] = xDiagDTCInfo.xDiagDTC.DTC[i][2];
			DTCData[Num+3] = xDiagDTCInfo.xDiagDTC.uStatus[i].Byte;
			Num += 4;
		}
	}

	return Num;
}

/* \brief
*/
BYTE getDiagAllDTC(BYTE DTCData[])
{
	BYTE i=0;
	BYTE Num=0;

	for(i=0; (i!=DIAG_DTC_NUM)&&(i!=((diagnetMESSAGE_MAX_LENGTH-3)/4));++i)
	{
		DTCData[4*i] = xDiagDTCInfo.xDiagDTC.DTC[i][0];
		DTCData[4*i+1] = xDiagDTCInfo.xDiagDTC.DTC[i][1];
		DTCData[4*i+2] = xDiagDTCInfo.xDiagDTC.DTC[i][2];
		DTCData[4*i+3] = xDiagDTCInfo.xDiagDTC.uStatus[i].Byte;
		Num += 4;
	}

	return Num;
}

/* \brief
*/
static void vDiagDTCConfirmWrite(void)
{
	BYTE i;
	
	xDiagDTCInfo.xDiagDTC.Chesum = 0;
	for(i=0; i!=DIAG_DTC_NUM; ++i)
	{
		xDiagDTCInfo.xDiagDTC.Chesum += xDiagDTCInfo.xDiagDTC.uStatus[i].Byte;
	}
	xDiagDTCInfo.xDiagDTC.Chesum = ~xDiagDTCInfo.xDiagDTC.Chesum;
	
	(void)xEepromWriteNByte(memoryADDR_DTC_CONFIRM, (BYTE*)&xDiagDTCInfo.xDiagDTC.uStatus[0].Byte, DIAG_DTC_NUM+1);
}

/* \brief
*/
static void vDiagDTCStatusReadAll(void)
{
	BYTE i;
	BYTE Chksum=0;
	
	(void)xEepromReadNByte(memoryADDR_DTC_CONFIRM, (BYTE*)&xDiagDTCInfo.xDiagDTC.uStatus[0].Byte, DIAG_DTC_NUM+1);

	for(i=0; i!=DIAG_DTC_NUM; ++i)
	{
		Chksum += xDiagDTCInfo.xDiagDTC.uStatus[i].Byte;
	}
	Chksum = ~Chksum;

	if(Chksum != xDiagDTCInfo.xDiagDTC.Chesum)
	{
		for(i=0; i!=DIAG_DTC_NUM; ++i)
		{
			xDiagDTCInfo.xDiagDTC.uStatus[i].Byte = diagdtcSTATUS_INIT;
		}
	}
}

