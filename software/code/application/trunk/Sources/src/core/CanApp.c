/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		CanApp.c
**	Abstract:		Can application
**	Creat By:		Zeno Liu
**	Creat Time:		2015.03.19
**	Version:		v1.0
**
******************************************************************************/

/* \brief
	include files
*/
#include "CanApp.h"
#include "Avm.h"
#include "Bcm.h"
#include "Ac.h"
#include "Gps.h"
#include "Debug.h"
#include "AirCondition.h"
#include "Radar.h"
#include "Config.h"
#include "Apu.h"
#include "DiagAppLayer.h"
#include "DiagNetLayer.h"
#include "NetworkManager.h"
#include "Common.h"
#include "DiagDTC.h"
#include "system.h"
#include "Ins.h"
#include "Ems.h"

/* \brief
*/
#define	canappDTC_NUM					3

#define	canappDTC_4A0_CYCLE_TIME		10//5+5		//周期是50ms
#define	canappDTC_3A0_CYCLE_TIME		4// 2+2		//周期是20ms
#define	canappDTC_620_CYCLE_TIME		20//10+10		//周期是100ms

#define	canappDTC_FAIL_COUNT			10
#define	canappDTC_SUCCESS_COUNT		5

/* \brief
*/
typedef struct
{
	BYTE DtcTimer;
	BYTE DtcLostCounter;
	BYTE DtcRevCounter;
	BOOL isDtcLost;
}CanAppDTCInfo;

/* \brief
*/
static UINT08 CanAppDelayTimer;
static CanAppDTCInfo xCanAppDTCInfo[canappDTC_NUM];
static BYTE CanAppDTCCycle[canappDTC_NUM] =
{
	canappDTC_4A0_CYCLE_TIME,
	canappDTC_3A0_CYCLE_TIME,
	canappDTC_620_CYCLE_TIME,
};
/* \brief
*/
static void vCanAppReadHandle(void);
static void vCanDtcTask(void);
static void vCanDtcStop(void);

/* \brief
*/
void vCanAppTask(void)
{
	vCanTask();
	vCanAppReadHandle();
	
	
	vAvmTask();
	vBcmTask();
	vAcTask();
	vAirTask();
	vGpsTask();
	vCanDtcTask();
}

/* \brief
*/
void vCanAppInit(void)
{

	vAvmInit();
	vBcmInit();
	vAcInit();
//	vDebugInit();
	vAirInit();
	vGpsInit();
	vRadarInit();
	vCanTaskInit();
	vEmsInit();
}

/* \brief
*/
void vCanAppTimer(void)
{
	BYTE i;
	
	if(0 != CanAppDelayTimer)
	{
		--CanAppDelayTimer;
	}

	for(i=0; i!=canappDTC_NUM; ++i)
	{
		if(0 != xCanAppDTCInfo[i].DtcTimer)
		{
			--xCanAppDTCInfo[i].DtcTimer;
		}
	}
	
	vCanTimer();
	vEmsTimer();
}

/* \brief
*/
BOOL blCanAppWrite(pCanFrame pxCanFrame)
{
	if((!isDiagAppCanAppTx()) || (!isNMAppActived()) || (0 != CanAppDelayTimer) || isCanBusOff())
	{
		return FALSE;
	}
	
	vCanWrite(pxCanFrame);

	return TRUE;
}

/* \brief
*/
void SetCanAppDelayTimer(UINT08 DelayTimer)
{
	CanAppDelayTimer = DelayTimer;
}

/* \brief
*/
static void vCanAppReadHandle(void)
{
	pCanFrame pxCanFrame;
	WORD wCanID;

	pxCanFrame = pxCanRead();

	if(NULL == pxCanFrame)
	{
		return;
	}

	wCanID = (WORD)(pxCanFrame->ID[0]<<3) | (BYTE)(pxCanFrame->ID[1]>>5);

	if((wCanID >= canID_NM_BASE) && (wCanID <= canID_NM_MAX))
	{	//nm报文
		vNMMessageHandle(pxCanFrame);
	}
	else
	{
		switch(pxCanFrame->ID[0])
		{
			default:
				break;
			case canID_RX_EPS_3A0_H:
				if(canID_RX_EPS_3A0_L == pxCanFrame->ID[1])
				{
					xCanAppDTCInfo[1].isDtcLost = FALSE;
					vCanEms3A0ReadHandle(pxCanFrame->Data);
				}
				break;
			case canID_RX_AVM_601_H:
				if(canID_RX_AVM_601_L == pxCanFrame->ID[1])
				{
					vCanAvmReadHandle(pxCanFrame->Data);
				}
				break;
			case canID_RX_AVM_620_H:
				if(canID_RX_AVM_620_L == pxCanFrame->ID[1])
				{
					xCanAppDTCInfo[2].isDtcLost = FALSE;
					vCanBcm620ReadHandle(pxCanFrame->Data);
				}
				break;
			case canID_RX_AVM_427_H:
				if(canID_RX_AVM_427_L == pxCanFrame->ID[1])
				{
				}
				break;
					
			case canID_RX_AC_530_H:
				if(canID_RX_AC_530_L == pxCanFrame->ID[1])
				{
					vCanAcReadHandle(pxCanFrame->Data);
				}
				break;
			case canID_RX_BCM_4A0_H:
				if(canID_RX_BCM_4A0_L == pxCanFrame->ID[1])
				{
					xCanAppDTCInfo[0].isDtcLost = FALSE;
					vCanBcm4A0ReadHandle(pxCanFrame->Data);
				}
				break;
			case canID_RX_BCM_5B0_H:
				if(canID_RX_BCM_5B0_L == pxCanFrame->ID[1])
				{
					vCanBcm5B0ReadHandle(pxCanFrame->Data);
				}
				break;
			case canID_RX_BCM_660_H:
				if(canID_RX_BCM_660_L == pxCanFrame->ID[1])
				{
					vCanBcm660ReadHandle(pxCanFrame->Data);
				}
				break;
			case canID_RX_EPS_180_H:
				if(canID_RX_EPS_180_L == pxCanFrame->ID[1])
				{
					vCanEpsReadHandle(pxCanFrame->Data);
				}
				break;
			case canID_RX_PAS_550_H:
				if(canID_RX_PAS_550_L == pxCanFrame->ID[1])
				{
					vCanPasReadHandle(pxCanFrame->Data);
				}
				break;
			case canID_RX_DIAG_720_H:
				if(canID_RX_DIAG_720_L == pxCanFrame->ID[1])
				{
					vDiagNetMessageHandle(pxCanFrame, FALSE);
				}
				break;

			case canID_RX_DIAG_FUNCID_7DF_H:
				if(canID_RX_DIAG_FUNCID_7DF_L == pxCanFrame->ID[1])
				{
					vDiagNetMessageHandle(pxCanFrame, TRUE);
				}
				break;
			case canID_RX_INS_678_H:
				if(canID_RX_INS_678_L == pxCanFrame->ID[1])
				{
					vInsMessageHandle(pxCanFrame);
				}
				break;
		}
	}
}

/* \brief
*/
static void vCanDtcTask(void)
{
	BYTE i=0;

	if((SYSTEM_ACC_IDLE != vSystemAccStatus()) || isCanBusOff())
	{
		//acc或者BAT正常时才检测can lost，防止断acc 或者bat时由于其他节点关闭时间不一致导致出现can lost DTC
		//busoff时不检测节点丢失
		vCanDtcStop();
		return;
	}

	for(i=0; i!=canappDTC_NUM; ++i)
	{
		if(0 == xCanAppDTCInfo[i].DtcTimer)
		{
			if(xCanAppDTCInfo[i].isDtcLost)
			{
				if(0 != xCanAppDTCInfo[i].DtcRevCounter)
				{
					xCanAppDTCInfo[i].DtcLostCounter = 0;
					xCanAppDTCInfo[i].DtcRevCounter = 0;
				}
				if(++xCanAppDTCInfo[i].DtcLostCounter >= canappDTC_FAIL_COUNT)
				{
					vDiagDTC((enumDiagDTC)(DIAG_DTC_LOST_BCM+i), DIAG_DTC_CTL_FAIL);
				}
			}
			else
			{
				if(0 != xCanAppDTCInfo[i].DtcLostCounter)
				{
					xCanAppDTCInfo[i].DtcLostCounter = 0;
					xCanAppDTCInfo[i].DtcRevCounter = 0;
				}
				if(++xCanAppDTCInfo[i].DtcRevCounter >= canappDTC_SUCCESS_COUNT)
				{
					vDiagDTC((enumDiagDTC)(DIAG_DTC_LOST_BCM+i), DIAG_DTC_CTL_SUCESS);
				}
			}
			xCanAppDTCInfo[i].DtcTimer = CanAppDTCCycle[i];
			xCanAppDTCInfo[i].isDtcLost = TRUE;
		}
	}
}

/* \brief
*/
static void vCanDtcStop(void)
{
	BYTE i;
	
	for(i=0; i!=canappDTC_NUM; ++i)
	{
		xCanAppDTCInfo[i].DtcTimer = CanAppDTCCycle[i];
		xCanAppDTCInfo[i].isDtcLost = FALSE;
		xCanAppDTCInfo[i].DtcLostCounter = 0;
		xCanAppDTCInfo[i].DtcRevCounter = 0;
	}
}

