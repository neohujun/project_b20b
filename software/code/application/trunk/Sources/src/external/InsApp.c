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
#include "InsApp.h"
#include "Ins.h"
#include "List.h"
#include "CanApp.h"
#include "system.h"
#include "common.h"
#include <MC9S08DZ60.h>

#include "Apu.h"
#include "Eeprom.h"


#include "system.h"


/* \brief
*/
#define	insappWRITE_BUFF_MAX						5

#define	insappPOSITIVE_RESPONSE_SID_BASE			0x40
#define	insappNEGATIVE_RESPONSE_SID					0x7f

#define	insappWRITE_DATA_TIMEOUT					10

#define	insappTYPE_NULL								0xff

#define MXA_PLAY_INFO_LEN		40
#define APU_HEAD_LEN			7



/* \brief
	这个用于发送缓冲，但是没有buff溢出检查，所以只能确保buff永远不会满就没有问题
*/
typedef struct
{
	InsMessage xInsWriteMsg[insappWRITE_BUFF_MAX];
	BYTE Head;
	BYTE Tail;
}InsAppWriteMsgBuff;

/* \brief
*/
typedef struct
{
	InsMessage* pxInsReadMsg;
	InsAppWriteMsgBuff xInsAppWriteMsgBuff;

	//通信控制
	BYTE CommControl[2];					//对常规应用报文和网络管理报文的控制

	//数据传输
	BOOL isWirteDataResponse;
	BYTE WriteDataCheckDelayTimer;
	BOOL isWirteOk;							//写入成功
}InsAppInfo;


/* \brief
*/
InsAppInfo xInsAppInfo;

/* \brief
*/
static void vInsAppWriteTask(void);

static void vInsAppWrite(void);


static void vInsAppReadDataByID(void);
static void vInsAppWriteDataByID(void);


/* \brief
*/
void vInsAppTask(void)
{
	vInsAppWriteTask();
	vInsTask();

	//超时退出非默认模式

}

/* \brief
*/
void vInsAppTaskInit(void)
{
	vInsTaskInit(&xInsAppInfo.xInsAppWriteMsgBuff.xInsWriteMsg[xInsAppInfo.xInsAppWriteMsgBuff.Tail]);

	xInsAppInfo.isWirteDataResponse = FALSE;
}

/* \brief
*/
void vInsAppTimer(void)
{
	vInsTimer();
}


/* \brief
*/
static void vInsAppWriteTask(void)
{
	if(isInsWriteBusy())
	{
		return;
	}
	
	if(xInsAppInfo.xInsAppWriteMsgBuff.Head != xInsAppInfo.xInsAppWriteMsgBuff.Tail)
	{	//buffer不为空
		//设置长度开始发送
		xInsAppInfo.xInsAppWriteMsgBuff.xInsWriteMsg[xInsAppInfo.xInsAppWriteMsgBuff.Tail].Dlc = xInsAppInfo.xInsAppWriteMsgBuff.xInsWriteMsg[xInsAppInfo.xInsAppWriteMsgBuff.Tail].DlcTemp;
		
		vInsWrite(&xInsAppInfo.xInsAppWriteMsgBuff.xInsWriteMsg[xInsAppInfo.xInsAppWriteMsgBuff.Tail]);

		if(++xInsAppInfo.xInsAppWriteMsgBuff.Tail >= insappWRITE_BUFF_MAX)
		{
			xInsAppInfo.xInsAppWriteMsgBuff.Tail = 0;
		}
	}
}

/* \brief
*/
static void vInsAppWrite(void)
{
	if(++xInsAppInfo.xInsAppWriteMsgBuff.Head >= insappWRITE_BUFF_MAX)
	{
		xInsAppInfo.xInsAppWriteMsgBuff.Head = 0;
	}
	return;
}

/* \brief
*/
void vInsApuPlayInfoSet(BYTE* pData)
{
	BYTE i = 0;
	xInsAppInfo.xInsAppWriteMsgBuff.xInsWriteMsg[xInsAppInfo.xInsAppWriteMsgBuff.Head].DlcTemp = pData[0] - APU_HEAD_LEN;

	
	switch(pData[2])
	{
		default:
			break;
		case APUW_SHOW_MUSIC:
			xInsAppInfo.xInsAppWriteMsgBuff.xInsWriteMsg[xInsAppInfo.xInsAppWriteMsgBuff.Head].eApuwShowMedia = 0x01;
			for(i = 0; i < (pData[0] - APU_HEAD_LEN); i++)
			{
				xInsAppInfo.xInsAppWriteMsgBuff.xInsWriteMsg[xInsAppInfo.xInsAppWriteMsgBuff.Head].Data[i] = pData[i + 3];
			}
			if(++xInsAppInfo.xInsAppWriteMsgBuff.Head >= insappWRITE_BUFF_MAX)
			{
				xInsAppInfo.xInsAppWriteMsgBuff.Head = 0;
			}
			break;
		case APUW_SHOW_RADIO:
			xInsAppInfo.xInsAppWriteMsgBuff.xInsWriteMsg[xInsAppInfo.xInsAppWriteMsgBuff.Head].eApuwShowMedia = 0x02;
			for(i = 0; i < (pData[0] - APU_HEAD_LEN); i++)
			{
				xInsAppInfo.xInsAppWriteMsgBuff.xInsWriteMsg[xInsAppInfo.xInsAppWriteMsgBuff.Head].Data[i] = pData[i + 3];
			}
			if(++xInsAppInfo.xInsAppWriteMsgBuff.Head >= insappWRITE_BUFF_MAX)
			{
				xInsAppInfo.xInsAppWriteMsgBuff.Head = 0;
			}
			break;
		case APUW_SHOW_PHONE:
			xInsAppInfo.xInsAppWriteMsgBuff.xInsWriteMsg[xInsAppInfo.xInsAppWriteMsgBuff.Head].eApuwShowMedia = 0x03;
			for(i = 0; i < (pData[0] - APU_HEAD_LEN); i++)
			{
				xInsAppInfo.xInsAppWriteMsgBuff.xInsWriteMsg[xInsAppInfo.xInsAppWriteMsgBuff.Head].Data[i] = pData[i + 3];
			}
			if(++xInsAppInfo.xInsAppWriteMsgBuff.Head >= insappWRITE_BUFF_MAX)
			{
				xInsAppInfo.xInsAppWriteMsgBuff.Head = 0;
			}
			break;
		case APUW_SHOW_CLEAR:
			xInsAppInfo.xInsAppWriteMsgBuff.xInsWriteMsg[xInsAppInfo.xInsAppWriteMsgBuff.Head].eApuwShowMedia = 0x00;
			for(i = 0; i < (pData[0] - APU_HEAD_LEN); i++)
			{
				xInsAppInfo.xInsAppWriteMsgBuff.xInsWriteMsg[xInsAppInfo.xInsAppWriteMsgBuff.Head].Data[i] = pData[i + 3];
			}
			if(++xInsAppInfo.xInsAppWriteMsgBuff.Head >= insappWRITE_BUFF_MAX)
			{
				xInsAppInfo.xInsAppWriteMsgBuff.Head = 0;
			}
			break;
	}
}

/* \brief
*/
void vInsApuNaviSet(BYTE* pData)
{
	BYTE i = 0;
//	xInsAppInfo.xInsAppWriteMsgBuff.xInsWriteMsg[xInsAppInfo.xInsAppWriteMsgBuff.Head].DlcTemp = pData[0] - APU_HEAD_LEN - 1;

	
	switch(pData[2])
	{
		case 0x03:		//navi info
			xInsAppInfo.xInsAppWriteMsgBuff.xInsWriteMsg[xInsAppInfo.xInsAppWriteMsgBuff.Head].eApuwShowMedia = APUW_SHOW_NAVI;
			xInsAppInfo.xInsAppWriteMsgBuff.xInsWriteMsg[xInsAppInfo.xInsAppWriteMsgBuff.Head].eApuwNaviState = pData[3];
			if(0 == pData[3])
			{//清除 0x02 0x01
				xInsAppInfo.xInsAppWriteMsgBuff.xInsWriteMsg[xInsAppInfo.xInsAppWriteMsgBuff.Head].DlcTemp = pData[0] - APU_HEAD_LEN;
			}
			else
			{
				xInsAppInfo.xInsAppWriteMsgBuff.xInsWriteMsg[xInsAppInfo.xInsAppWriteMsgBuff.Head].DlcTemp = pData[0] - APU_HEAD_LEN - 1;
			}
			for(i = 0; i < (pData[0] - APU_HEAD_LEN - 1); i++)
			{
				xInsAppInfo.xInsAppWriteMsgBuff.xInsWriteMsg[xInsAppInfo.xInsAppWriteMsgBuff.Head].Data[i] = pData[i + 4];
			}
			if(++xInsAppInfo.xInsAppWriteMsgBuff.Head >= insappWRITE_BUFF_MAX)
			{
				xInsAppInfo.xInsAppWriteMsgBuff.Head = 0;
			}
			break;
	}


}

