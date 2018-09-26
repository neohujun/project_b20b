/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		Ins.h
**	Abstract:		head of instrument
**	Creat By:		neo Hu
**	Creat Time:		2011.11.27
**	Version:		v1.0
**
******************************************************************************/

#ifndef __INS_H__
#define __INS_H__

/* \brief
*/
#include "type.h"
#include "ApuWSysInfo.h"
#include "can.h"

/* \brief
*/
#define	insMESSAGE_MAX_LENGTH		42
//#define	insNAVI_STATUS_MASK		0x10

#define	insCAN_MEDIA_TXID_H			canID_TX_INS_673_H
#define	insCAN_MEDIA_TXID_L			canID_TX_INS_673_L

#define	insCAN_NAVI_TXID_H				canID_TX_INS_674_H
#define	insCAN_NAVI_TXID_L				canID_TX_INS_674_L

/* \brief
*/
typedef enum
{
	INS_WRITE_NULL,
	INS_WRITE_OK,
	INS_WRITING,
	INS_WRITE_T1ERROR,
}enumInsWriteResultStatus;

/* \brief
*/
typedef enum
{
	INS_READ_ERROR_NULL,
	INS_READ_ERROR_SN,					//SERIAL NO ERROR
	INS_READ_ERROR_WAIT_CF_TIMEOUT,	//�ȴ�����֡��ʱ
	INS_READ_ERROR_PHY_FUNC,			//���ձ��ĵ�Ѱַ��ʽ����
}enumInsReadErrorStatus;

/* \brief
*/
typedef struct
{
	BYTE Data[insMESSAGE_MAX_LENGTH];
	WORD Dlc;									//DLC��ֵ���ֱ�ӷ����ˣ�����д������buffʱ�ݴ�DlcTemp
	WORD DlcTemp;								//�����ݴ�DLC����
	enumApuwShowMedia eApuwShowMedia;
	enumApuwNaviState eApuwNaviState;
}InsMessage;



/* \brief
	function declaration
*/
void vInsTask(void);
void vInsTaskInit(InsMessage* pxInsMessage);
void vInsTimer(void);

void vInsWrite(InsMessage* pxInsMessage);
BOOL isInsWriteBusy(void);
void vInsMessageHandle(CanFrame* pxCanMsg);

#endif
