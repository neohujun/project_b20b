/******************************************************************************
**
**								Copyright (C) 2017  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		NetworkManager.c
**	Abstract:		network manager
**	Creat By:		Zeno Liu
**	Creat Time:		2017.03.13
**	Version:		v1.0
**
******************************************************************************/


/* \brief
	include files
*/
#include "NetworkManager.h"

#include "DiagAppLayer.h"
#include "io.h"
#include "CanApp.h"
#include "DiagDTC.h"

/* \brief
	Marco define
*/
#define	nmBASE_ID						0x04

#define	nmID_HU						0x20
#define	nmID_START					0x00
#define	nmID_END						0xff

#define	nmTX_COUNT_ERROR_LIMIT		8
#define	nmRX_COUNT_ERROR_LIMIT		4

#define	nmNODE_MAX					256

#define	nmTIMEOUT_TYPE_TIMER			10
#define	nmTIMEOUT_MAX_TIMER			26
#define	nmTIMEOUT_ERROR_TIMER		100
#define	nmTIMEOUT_SLEEP_WAIT_TIMER	150

#define	nmLIMPHOME_DTC_TIMEOUT		200

/* \brief
*/
typedef enum
{
	NM_IDLE,
	NM_INIT,
	NM_NORMAL,
	NM_LIMPHOME,
	NM_PRE_SLEEP,
	NM_SLEEP_WAIT,
	NM_SLEEP
}enumNMState;

/* \brief
*/
typedef enum
{
	NM_INIT_CONFIG,
	NM_INIT_LIMPHOME_RESET,
	NM_INIT_RESET,
	NM_INIT_ALIVE,
	NM_INIT_DONE
}enumNMInitState;

/* \brief
*/
typedef enum
{
	NM_OPTION_ALIVE			=0x01,
	NM_OPTION_RING			=0x02,
	NM_OPTION_LIMPHOME		=0x04
}enumNMOptionType;

/* \brief
	NM messate struct
*/
typedef struct
{
	BYTE BaseID;
	BYTE SourceID;
	BYTE DestID;
	union{
		BYTE Byte;
		struct{
			unsigned eNMOptionType			:3;		//enumNMOptionTyep
			unsigned Reserve1				:1;
			
			unsigned SleepIndicate			:1;
			unsigned SleepAck				:1;
			unsigned Reseve2					:2;
		}Bits;
	}OptionCode;
}NMMessage;

/* \brief
*/
typedef struct
{
	BOOL isActived;
	
	BOOL isLimpHome;
	BOOL isSleep;
}NMNode;

/* \brief
*/
typedef struct
{
	enumNMState eNMState;
	enumNMInitState eNMInitState;
	NMMessage xNMTxMessage;
	WORD wNMDelayTimer;

//	NMLimpHomeNode xNMLimpHomeNode;
	
	BYTE NMTxCount;					//发送错误计数
	BYTE NMRxCount;					//接收错误计数

	BOOL isNMActived;					// TRUE:主动网络管理状态   FALSE:被动网络管理状态Networkstatus.NMactive
	BOOL isNMLimpHome;					//当前网络状态: TRUE:当前为跛行状态；FALSE: 当前为非跛行状态Config.limphome
	BOOL isNMAppActived;					// 允许应用报文发送接收D_Online
	BOOL isNMSleepAllow;					// 允许网络睡眠
	BOOL isNMNetworkBusSleep;			//网络所有节点都睡眠了Networkstatus.bussleep
	BOOL isNMSleep;						// 睡眠标志
	BOOL isNMLimpHomeTxSuccess;		// 成功发送跛行报文NMMerker.limphome
	BOOL isNMLimpHomeSleep;				// 跛行模式睡眠标志
	WORD wNMSleepDelayTimer;			//当进入NORMAL/LIMPHOME模式时的睡眠最小延时时间>5000ms,防止远程唤醒后频繁的睡眠唤醒

	BOOL isNMTypeTimerStart;				// Ttyp计时器两个NM报文传输之间的典型时间
	WORD wNMTypeTimer;

	BOOL isNMNormalMaxTimerStart;		// Tmax计时器两个NM报文传输之间的最大时间
	WORD wNMNormalMaxTimer;

	BOOL isNMErrorTimerStart;				// Terror计时器两个跛行报文传输之间的时间
	WORD wNMErrorTimer;

	BOOL isNMSleepWaitTimerStart;		// Twbs计时器总线睡眠请求被确认到总线进入睡眠模式之间的时间
	WORD wNMSleepWaitTimer;

//	NMNode xNMNode[nmNODE_MAX];

	BOOL isBusoff;

	BYTE NMLimpHomeDTCTimer;
}NMInfo;

/* \brief
*/
NMInfo xNMInfo;
//BOOL isNMTxLimpHomeSuccess;		//用于从busoff恢复确认第一帧跛行报文是否发送成功
//BOOL isNMTxFirstFrameSuccess;		//用于上电第一帧确认can发送开始再计时，防止延时时间不符合标准

/* \brief
*/
static void vNMInit(void);
static void vNMInitReset(void);

static void vNMInitConfig(void);
static void vNMWriteMessage(BOOL isCheckResult);
static void vNMRxMessageConvert(CanFrame* pxSourceCanMsg, NMMessage* pxDestNMMsg);

static void vNMWriteOkHandle(void);
static void vNMNormalHandle(void);
static void vNMLimpHomeHandle(void);
static void vNMPreSleepHandle(void);
static void vNMCheckBusSleep(NMMessage* pxNMRxMsg);
static void vNMClearBusSleep(void);

/* \brief
*/
void vNMTask(void)
{
	if(xNMInfo.isNMSleepAllow != xNMInfo.isNMNetworkBusSleep)
	{
		xNMInfo.isNMNetworkBusSleep = xNMInfo.isNMSleepAllow;
	}

	switch(xNMInfo.eNMState)
	{
		case NM_INIT:
			vNMInit();
			break;
			
		case NM_NORMAL:
			vNMNormalHandle();
			break;
			
		case NM_LIMPHOME:
			vNMLimpHomeHandle();
			break;
			
		case NM_PRE_SLEEP:
			vNMPreSleepHandle();
			break;
			
		case NM_SLEEP_WAIT:
			if(xNMInfo.wNMSleepWaitTimer >= nmTIMEOUT_SLEEP_WAIT_TIMER)
			{
				xNMInfo.eNMState = NM_SLEEP;

				//收发器睡眠
				ioCAN_STBY_ON;
				ioCAN_EN_ON;

				xNMInfo.isNMSleep = TRUE;
				//延时20ms检测是否远程唤醒，防止收发器刚睡眠时，INH还不正常时检测INH信号
				xNMInfo.wNMDelayTimer = 2;
			}
			else if(!xNMInfo.isNMSleepAllow)
			{
				xNMInfo.wNMSleepWaitTimer = 0;
				xNMInfo.isNMSleepWaitTimerStart = FALSE;

				if(xNMInfo.isNMLimpHomeSleep)
				{
					xNMInfo.wNMErrorTimer = 0;
					xNMInfo.isNMErrorTimerStart = TRUE;
					
					xNMInfo.isNMAppActived = TRUE;

					xNMInfo.eNMState = NM_LIMPHOME;
					xNMInfo.NMLimpHomeDTCTimer = nmLIMPHOME_DTC_TIMEOUT;
				}
				else
				{
					//RESET
					xNMInfo.eNMState = NM_INIT;
					xNMInfo.eNMInitState = NM_INIT_RESET;
				}
			}
			break;
			
		case NM_SLEEP:
			if(0 != xNMInfo.wNMDelayTimer)
			{
				break;
			}
			
			if(ioCAN_INH)
			{
				vNMTaskAwake(FALSE);
			}
			break;

		case NM_IDLE:
			break;
	}
	
	return;
}

/* \brief
*/
void vNMTaskInit(void)
{
	xNMInfo.eNMState = NM_INIT;
	xNMInfo.eNMInitState = NM_INIT_CONFIG;
	
	xNMInfo.xNMTxMessage.BaseID = nmBASE_ID;
	xNMInfo.xNMTxMessage.SourceID = nmID_HU;

	xNMInfo.isNMTypeTimerStart = FALSE;
	xNMInfo.isNMNormalMaxTimerStart = FALSE;
	xNMInfo.isNMErrorTimerStart = FALSE;
	xNMInfo.isNMSleepWaitTimerStart = FALSE;
	xNMInfo.wNMTypeTimer = 0;
	xNMInfo.wNMNormalMaxTimer = 0;
	xNMInfo.wNMErrorTimer = 0;
	xNMInfo.wNMSleepWaitTimer = 0;
	
	xNMInfo.isNMSleep = FALSE;
	xNMInfo.isNMSleepAllow = FALSE;
	xNMInfo.isNMNetworkBusSleep = xNMInfo.isNMSleepAllow;

	//进入主动网络管理模式
	xNMInfo.isNMActived = TRUE;

	//不允许应用报文发送
	xNMInfo.isNMAppActived = FALSE;

	//启动后未接收到任何消息时默认网络是睡眠状态
//	xNMInfo.isNMNetworkBusSleep = TRUE;

	xNMInfo.isBusoff = FALSE;
	
	return;
}

/* \brief
*/
void vNMTimer(void)
{
	if(xNMInfo.isNMTypeTimerStart)
	{
		++xNMInfo.wNMTypeTimer;
	}

	if(xNMInfo.isNMNormalMaxTimerStart)
	{
		++xNMInfo.wNMNormalMaxTimer;
	}

	if(xNMInfo.isNMErrorTimerStart && (!xNMInfo.isBusoff))
	{
		++xNMInfo.wNMErrorTimer;
	}

	if(xNMInfo.isNMSleepWaitTimerStart)
	{
		++xNMInfo.wNMSleepWaitTimer;
	}

	if(0 != xNMInfo.wNMDelayTimer)
	{
		if(0 == --xNMInfo.wNMDelayTimer)
		{
			//用于发送永远失败时系统能够正常启动
//			isNMTxFirstFrameSuccess = TRUE;
		}
	}

	if(0 != xNMInfo.wNMSleepDelayTimer)
	{
		--xNMInfo.wNMSleepDelayTimer;
	}

	if((0 != xNMInfo.NMLimpHomeDTCTimer) && (!xNMInfo.isBusoff))
	{
		--xNMInfo.NMLimpHomeDTCTimer;
	}
	
	return;
}

/* \brief
*/
void vNMTaskAwake(BOOL isHardwareAwke)
{
	if(!xNMInfo.isNMSleep)
	{
		return;
	}
	
	xNMInfo.eNMState = NM_INIT;
	xNMInfo.eNMInitState = NM_INIT_CONFIG;
	
	xNMInfo.xNMTxMessage.BaseID = nmBASE_ID;
	xNMInfo.xNMTxMessage.SourceID = nmID_HU;

	xNMInfo.isNMTypeTimerStart = FALSE;
	xNMInfo.isNMNormalMaxTimerStart = FALSE;
	xNMInfo.isNMErrorTimerStart = FALSE;
	xNMInfo.isNMSleepWaitTimerStart = FALSE;
	xNMInfo.wNMTypeTimer = 0;
	xNMInfo.wNMNormalMaxTimer = 0;
	xNMInfo.wNMErrorTimer = 0;
	xNMInfo.wNMSleepWaitTimer = 0;

	if(isHardwareAwke)
	{
		xNMInfo.isNMSleepAllow = FALSE;
		xNMInfo.isNMNetworkBusSleep = xNMInfo.isNMSleepAllow;
	}
	xNMInfo.isNMSleep = FALSE;

	ioCAN_STBY_OFF;
	ioCAN_EN_ON;

	//进入主动网络管理模式
	xNMInfo.isNMActived = TRUE;

	//不允许应用报文发送
	xNMInfo.isNMAppActived = FALSE;

	//启动后未接收到任何消息时默认网络是睡眠状态
//	xNMInfo.isNMNetworkBusSleep = TRUE;

#if	0
	Can_SendMsgBufHead = Can_SendMsgBufTail;		//清空buff
#endif
	
	return;
}

/* \brief
*/
BOOL isNMAppActived(void)
{
	return xNMInfo.isNMAppActived;
}

/* \brief
*/
void vNMMessageHandle(CanFrame* pxCanRxMsg)
{
	NMMessage xNMRxMessage;

	if(!isDiagAppCanNmRx())
	{
		return;
	}

	//Limphome DTC解除
	vDiagDTC(DIAG_DTC_NM_LIMPHOME, DIAG_DTC_CTL_SUCESS);
	
	vNMRxMessageConvert(pxCanRxMsg, &xNMRxMessage);

	//SourceID为BYTE型，所有此条件恒TRUE
//	if((xNMRxMessage.SourceID >= nmID_START) && (xNMRxMessage.SourceID <= nmID_END))
	{//NM
		switch(xNMInfo.eNMState)
		{
			case NM_PRE_SLEEP:
				if(xNMInfo.isNMLimpHomeSleep)
				{
					if(xNMInfo.isNMActived && (!xNMInfo.isNMLimpHomeTxSuccess))
					{
						if(xNMInfo.isNMNetworkBusSleep&& xNMRxMessage.OptionCode.Bits.SleepIndicate)
						{
							if(xNMRxMessage.OptionCode.Bits.SleepAck)
							{
								//跛行模式下睡眠模式初始化
								xNMInfo.isNMAppActived = FALSE;

								xNMInfo.wNMSleepWaitTimer = 0;
								xNMInfo.isNMSleepWaitTimerStart = TRUE;
								
								xNMInfo.eNMState = NM_SLEEP_WAIT;
							}
						}
						else
						{
							xNMInfo.eNMState = NM_LIMPHOME;
							xNMInfo.NMLimpHomeDTCTimer = nmLIMPHOME_DTC_TIMEOUT;
						}
					}
					else
					{
						if(xNMInfo.isNMNetworkBusSleep&& xNMRxMessage.OptionCode.Bits.SleepAck)
						{
							//跛行模式下睡眠模式初始化
							xNMInfo.isNMAppActived = FALSE;

							xNMInfo.wNMSleepWaitTimer = 0;
							xNMInfo.isNMSleepWaitTimerStart = TRUE;
							
							xNMInfo.eNMState = NM_SLEEP_WAIT;
						}
						else
						{
							//RESET
							xNMInfo.wNMErrorTimer = 0;
							xNMInfo.isNMErrorTimerStart = FALSE;
							xNMInfo.isNMLimpHomeTxSuccess = 0;
							
							xNMInfo.eNMState = NM_INIT;
							xNMInfo.eNMInitState = NM_INIT_LIMPHOME_RESET;
						}
					}
					break;
				}
			case NM_NORMAL:
				xNMInfo.NMRxCount = 0;		//接收到报文错误清零
				if((NM_OPTION_RING == xNMRxMessage.OptionCode.Bits.eNMOptionType) || (NM_OPTION_ALIVE == xNMRxMessage.OptionCode.Bits.eNMOptionType))
				{	//Ring/alive
					//重新确定目标地址
					if((nmID_HU == xNMInfo.xNMTxMessage.DestID)
						|| ((nmID_HU < xNMRxMessage.SourceID) && (xNMRxMessage.SourceID < xNMInfo.xNMTxMessage.DestID))
						|| ((xNMRxMessage.SourceID < xNMInfo.xNMTxMessage.DestID) && (xNMInfo.xNMTxMessage.DestID < nmID_HU))
						|| ((xNMInfo.xNMTxMessage.DestID < nmID_HU) && (nmID_HU < xNMRxMessage.SourceID)))
					{
						xNMInfo.xNMTxMessage.DestID = xNMRxMessage.SourceID;
					}
					
					if(NM_OPTION_RING == xNMRxMessage.OptionCode.Bits.eNMOptionType)
					{	//Ring
						xNMInfo.isNMTypeTimerStart = FALSE;
						xNMInfo.wNMTypeTimer = 0;
						xNMInfo.isNMNormalMaxTimerStart = FALSE;
						xNMInfo.wNMNormalMaxTimer = 0;

						if((nmID_HU == xNMRxMessage.DestID) || (xNMRxMessage.SourceID == xNMRxMessage.DestID))
						{
							xNMInfo.isNMTypeTimerStart = TRUE;
							xNMInfo.wNMTypeTimer = 0;
						}
						else
						{
							xNMInfo.isNMNormalMaxTimerStart = TRUE;
							xNMInfo.wNMNormalMaxTimer = 0;

							if(((xNMRxMessage.DestID < xNMRxMessage.SourceID) && (xNMRxMessage.SourceID < nmID_HU))
								|| ((nmID_HU < xNMRxMessage.DestID) && (xNMRxMessage.DestID < xNMRxMessage.SourceID))
								|| ((xNMRxMessage.SourceID < nmID_HU) && (nmID_HU < xNMRxMessage.DestID)))
							{
								if(xNMInfo.isNMActived)
								{
									//发送ALIVE时不能发送SLEEPACK为TRUE的alvie
									xNMInfo.xNMTxMessage.OptionCode.Bits.SleepAck = FALSE;
									if(xNMInfo.isNMSleepAllow)
									{
										xNMInfo.xNMTxMessage.OptionCode.Bits.SleepIndicate = TRUE;
									}
									else
									{
										xNMInfo.xNMTxMessage.OptionCode.Bits.SleepIndicate = FALSE;
									}
									xNMInfo.xNMTxMessage.OptionCode.Bits.eNMOptionType = NM_OPTION_ALIVE;
									vNMWriteMessage(FALSE);
									
									//normal模式时发送成功,pre_sleep模式接收完成后也会切换到normal模式，计时Tmax
									xNMInfo.NMTxCount = 0;		//发送错误计数清零
								}
							}
						}
					}
				}
				else if(NM_OPTION_LIMPHOME == xNMRxMessage.OptionCode.Bits.eNMOptionType)
				{	//Limphome
					//其他节点的跛行模式我要记录有什么用?
//						xNMInfo.xNMNode[xNMRxMessage.SourceID].isLimpHome = TRUE;
				}

				//睡眠判断
				if(NM_NORMAL == xNMInfo.eNMState)
				{
					if(xNMRxMessage.OptionCode.Bits.SleepAck)
					{
						if(xNMInfo.isNMNetworkBusSleep)
						{
							//进入睡眠初始化
							xNMInfo.isNMAppActived = FALSE;

							xNMInfo.wNMSleepWaitTimer = 0;
							xNMInfo.isNMSleepWaitTimerStart = TRUE;
							
							xNMInfo.eNMState = NM_SLEEP_WAIT;
						}
					}
				}
				else if(NM_PRE_SLEEP == xNMInfo.eNMState)
				{
					if(xNMRxMessage.OptionCode.Bits.SleepIndicate)
					{
						if(xNMRxMessage.OptionCode.Bits.SleepAck)
						{
							//进入睡眠初始化
							xNMInfo.isNMAppActived = FALSE;

							xNMInfo.wNMSleepWaitTimer = 0;
							xNMInfo.isNMSleepWaitTimerStart = TRUE;
							
							xNMInfo.eNMState = NM_SLEEP_WAIT;
						}
					}
					else
					{
						//流程里没有，需要添加上去的
						xNMInfo.xNMTxMessage.OptionCode.Bits.SleepAck = FALSE;
						
						xNMInfo.eNMState = NM_NORMAL;
					}
				}
				break;

			case NM_SLEEP_WAIT:
				if(!xNMRxMessage.OptionCode.Bits.SleepIndicate)
				{
					xNMInfo.wNMSleepWaitTimer = 0;
					xNMInfo.isNMSleepWaitTimerStart = FALSE;

					if(xNMInfo.isNMLimpHomeSleep)
					{
						xNMInfo.wNMErrorTimer = 0;
						xNMInfo.isNMErrorTimerStart = TRUE;
						
						xNMInfo.isNMAppActived = TRUE;

						xNMInfo.eNMState = NM_LIMPHOME;
						xNMInfo.NMLimpHomeDTCTimer = nmLIMPHOME_DTC_TIMEOUT;
					}
					else
					{
						//RESET
						xNMInfo.eNMState = NM_INIT;
						xNMInfo.eNMInitState = NM_INIT_RESET;
					}
				}
				break;

			case NM_LIMPHOME:
				if(xNMInfo.isNMActived && (!xNMInfo.isNMLimpHomeTxSuccess))
				{
					if(xNMInfo.isNMNetworkBusSleep)
					{
						if(xNMRxMessage.OptionCode.Bits.SleepIndicate)
						{
							if(xNMRxMessage.OptionCode.Bits.SleepAck)
							{
								//跛行模式下睡眠模式初始化
								xNMInfo.isNMAppActived = FALSE;

								xNMInfo.wNMSleepWaitTimer = 0;
								xNMInfo.isNMSleepWaitTimerStart = TRUE;

								xNMInfo.isNMLimpHomeSleep = TRUE;
								
								xNMInfo.eNMState = NM_SLEEP_WAIT;
								
							}
							else
							{
								xNMInfo.isNMLimpHomeSleep = TRUE;
								xNMInfo.eNMState = NM_PRE_SLEEP;
							}
						}
					}
				}
				else
				{
					if(xNMInfo.isNMNetworkBusSleep && xNMRxMessage.OptionCode.Bits.SleepAck)
					{
						//跛行模式下睡眠模式初始化
						xNMInfo.isNMAppActived = FALSE;

						xNMInfo.wNMSleepWaitTimer = 0;
						xNMInfo.isNMSleepWaitTimerStart = TRUE;

						xNMInfo.isNMLimpHomeSleep = TRUE;
						
						xNMInfo.eNMState = NM_SLEEP_WAIT;
					}
					else
					{
						//reset
						xNMInfo.wNMErrorTimer = 0;
						xNMInfo.isNMErrorTimerStart = FALSE;
						xNMInfo.isNMLimpHomeTxSuccess = 0;
						
						xNMInfo.eNMState = NM_INIT;
						xNMInfo.eNMInitState = NM_INIT_LIMPHOME_RESET;
					}
				}
				break;
		}
	}
}

/* \brief
*/
void vNMSleepAllow(BOOL isNMSleepAllow)
{
	xNMInfo.isNMSleepAllow = isNMSleepAllow;
	xNMInfo.isNMNetworkBusSleep = xNMInfo.isNMSleepAllow;
}

/* \brief
*/
void vNMActived(BOOL isActived)
{
	xNMInfo.isNMActived = isActived;
}

/* \brief
*/
BOOL isNMSleep(void)
{
	return xNMInfo.isNMSleep;
}

/* \brief
*/
BOOL isNMSleepAllow(void)
{
	return xNMInfo.isNMSleepAllow;
}

/* \brief
*/
void vNMBusoff(BOOL isBusoff)
{
	xNMInfo.isBusoff = isBusoff;
	
	if(xNMInfo.isBusoff)
	{
		xNMInfo.NMLimpHomeDTCTimer = nmLIMPHOME_DTC_TIMEOUT;
	}
	else
	{
		xNMInfo.isNMErrorTimerStart = TRUE;
		xNMInfo.wNMErrorTimer = 0;
		
		xNMInfo.eNMState = NM_LIMPHOME;
		xNMInfo.NMLimpHomeDTCTimer = 200;
		
		//延时90ms发送应用报文
		SetCanAppDelayTimer(9);
	}
}

/* \brief
*/
void vNMBusoffRecovery(void)
{
	if(xNMInfo.isNMNetworkBusSleep && (0 == xNMInfo.wNMSleepDelayTimer))
	{
		xNMInfo.xNMTxMessage.OptionCode.Bits.SleepIndicate = TRUE;
	}
	else
	{
		xNMInfo.xNMTxMessage.OptionCode.Bits.SleepIndicate = FALSE;
	}

	xNMInfo.xNMTxMessage.OptionCode.Bits.eNMOptionType = NM_OPTION_LIMPHOME;
	vNMWriteMessage(TRUE);
	
	xNMInfo.wNMErrorTimer = 0;
}

/* \brief
*/
static void vNMInit(void)
{
	switch(xNMInfo.eNMInitState)
	{
		case NM_INIT_CONFIG:
			vNMInitConfig();

		case NM_INIT_LIMPHOME_RESET:
			xNMInfo.NMRxCount = 0;
			xNMInfo.NMTxCount = 0;	

		case NM_INIT_RESET:
			vNMInitReset();

			if(xNMInfo.isNMActived)
			{
				xNMInfo.xNMTxMessage.OptionCode.Bits.eNMOptionType = NM_OPTION_ALIVE;
				
				xNMInfo.NMTxCount++;	//必须放在前面，因为发送函数调用就默认成功，而不是等待读取成功返回值
				vNMWriteMessage(TRUE);
//				isNMTxFirstFrameSuccess = FALSE;
//				xNMInfo.wNMDelayTimer = 100;
			}
			
			//网络管理报文发送后再发送应用报文
			xNMInfo.isNMAppActived = TRUE;
			
			if((xNMInfo.NMRxCount <= nmRX_COUNT_ERROR_LIMIT) && (xNMInfo.NMTxCount <= nmTX_COUNT_ERROR_LIMIT))
			{	//normal
				xNMInfo.isNMTypeTimerStart = TRUE;
				xNMInfo.wNMTypeTimer = 0;
				
				xNMInfo.eNMState = NM_NORMAL;

				//当进入NORMAL模式时的睡眠最小延时时间>5000ms,防止远程唤醒后频繁的睡眠唤醒
				xNMInfo.wNMSleepDelayTimer = 500;
			}
			else
			{	//LimpHome
				xNMInfo.isNMErrorTimerStart = TRUE;
				xNMInfo.wNMErrorTimer = 0;
				
				xNMInfo.eNMState = NM_LIMPHOME;
				xNMInfo.NMLimpHomeDTCTimer = nmLIMPHOME_DTC_TIMEOUT;

				//当进入LIMPHOME模式时的睡眠最小延时时间>5000ms,防止远程唤醒后频繁的睡眠唤醒
				xNMInfo.wNMSleepDelayTimer = 500;
			}
			break;
	}
	
	return;
}

/* \brief
*/
static void vNMInitConfig(void)
{
	//应用报文跟第一帧网络报文要间隔90ms
	SetCanAppDelayTimer(9);
	
	//awake from
	xNMInfo.isNMLimpHome = FALSE;
}

/* \brief
*/
static void vNMInitReset(void)
{
	xNMInfo.xNMTxMessage.OptionCode.Bits.SleepIndicate = FALSE;
	xNMInfo.xNMTxMessage.OptionCode.Bits.SleepAck = FALSE;
	
	//exit normal from,reset
	xNMInfo.xNMTxMessage.DestID = nmID_HU;

	xNMInfo.NMRxCount++;
	
	xNMInfo.isNMLimpHomeTxSuccess = FALSE;
	xNMInfo.isNMLimpHomeSleep = FALSE;

	//RESET时清空所有计时，流程图里没有，另外添加的
	xNMInfo.isNMTypeTimerStart = FALSE;
	xNMInfo.isNMNormalMaxTimerStart = FALSE;
	xNMInfo.isNMErrorTimerStart = FALSE;
	xNMInfo.isNMSleepWaitTimerStart = FALSE;
	xNMInfo.wNMTypeTimer = 0;
	xNMInfo.wNMNormalMaxTimer = 0;
	xNMInfo.wNMErrorTimer = 0;
	xNMInfo.wNMSleepWaitTimer = 0;
}

/* \brief
*/
static void vNMWriteMessage(BOOL isCheckResult)
{
	CanFrame xTxMsg;
	WORD wCanID;

	if(!isDiagAppCanNMTx())
	{
		return;
	}

	xTxMsg.Length = 8;
	wCanID = xNMInfo.xNMTxMessage.BaseID<<8|xNMInfo.xNMTxMessage.SourceID;
	xTxMsg.ID[0] = (BYTE)(wCanID>>3);
	xTxMsg.ID[1] = (BYTE)(wCanID<<5);

	xTxMsg.Data[0] = xNMInfo.xNMTxMessage.DestID;
	xTxMsg.Data[1] = xNMInfo.xNMTxMessage.OptionCode.Byte;
	xTxMsg.Data[2] = 0x00;
	xTxMsg.Data[3] = 0x00;
	xTxMsg.Data[4] = 0x00;
	xTxMsg.Data[5] = 0x00;
	xTxMsg.Data[6] = 0x00;
	xTxMsg.Data[7] = 0x00;
	
	vCanWrite(&xTxMsg);

	//调用Can发送接口即发送成功
	if(isCheckResult)
	{
		vNMWriteOkHandle();
	}
		
	return;
}

/* \brief
*/
static void vNMRxMessageConvert(CanFrame* pxSourceCanMsg, NMMessage* pxDestNMMsg)
{
	WORD wCanID = pxSourceCanMsg->ID[1]>>5;
	wCanID |= ((WORD)pxSourceCanMsg->ID[0]<<3);

	pxDestNMMsg->SourceID = (BYTE)wCanID;
	pxDestNMMsg->DestID = pxSourceCanMsg->Data[0];
	pxDestNMMsg->OptionCode.Byte = pxSourceCanMsg->Data[1];
}

/* \brief
*/
static void vNMWriteOkHandle(void)
{
	switch(xNMInfo.eNMState)
	{
		case NM_NORMAL:
			xNMInfo.NMTxCount = 0;		//发送错误计数清零
			if(NM_OPTION_RING == xNMInfo.xNMTxMessage.OptionCode.Bits.eNMOptionType)
			{
				//取消Ttyp
				xNMInfo.isNMTypeTimerStart = FALSE;
				xNMInfo.wNMTypeTimer = 0;
				
				//重新定时Tmax
				xNMInfo.isNMNormalMaxTimerStart = TRUE;
				xNMInfo.wNMNormalMaxTimer = 0;

				if(xNMInfo.xNMTxMessage.OptionCode.Bits.SleepIndicate)
				{
					//HU满足睡眠条件发送成功 后就进入NM_PRE_SLEEP
					if(xNMInfo.isNMNetworkBusSleep)
					{
						xNMInfo.xNMTxMessage.OptionCode.Bits.SleepAck = TRUE;

						//进入预睡眠状态
						xNMInfo.eNMState = NM_PRE_SLEEP;
					}
				}
			}
			break;

		case NM_PRE_SLEEP:
			if(xNMInfo.isNMLimpHomeSleep)
			{
				if(NM_OPTION_LIMPHOME == xNMInfo.xNMTxMessage.OptionCode.Bits.eNMOptionType)
				{
					xNMInfo.isNMLimpHomeTxSuccess = TRUE;
				}
			}
			else
			{
				xNMInfo.isNMNormalMaxTimerStart = FALSE;
				xNMInfo.wNMNormalMaxTimer = 0;

				//睡眠等待
				xNMInfo.isNMAppActived = FALSE;

				xNMInfo.wNMSleepWaitTimer = 0;
				xNMInfo.isNMSleepWaitTimerStart = TRUE;
				
				xNMInfo.eNMState = NM_SLEEP_WAIT;
			}
			break;

		case NM_LIMPHOME:
			if(NM_OPTION_LIMPHOME == xNMInfo.xNMTxMessage.OptionCode.Bits.eNMOptionType)
			{
				xNMInfo.isNMLimpHomeTxSuccess = TRUE;
			}
			break;
	}
}

static void vNMNormalHandle(void)
{
		//节点满足睡眠
	if(xNMInfo.isNMSleepAllow && (0 == xNMInfo.wNMSleepDelayTimer))
	{
		if(!xNMInfo.isNMActived)
		{
			//进入预睡眠状态
			xNMInfo.eNMState = NM_PRE_SLEEP;
			return;
		}
	}
	if(xNMInfo.wNMTypeTimer >= nmTIMEOUT_TYPE_TIMER)
	{
		xNMInfo.isNMTypeTimerStart = FALSE;
		xNMInfo.wNMTypeTimer = 0;
		
		xNMInfo.isNMNormalMaxTimerStart = TRUE;
		xNMInfo.wNMNormalMaxTimer = 0;

		if(xNMInfo.isNMActived)
		{
			if(xNMInfo.isNMSleepAllow && (0 == xNMInfo.wNMSleepDelayTimer))
			{
				xNMInfo.xNMTxMessage.OptionCode.Bits.SleepIndicate = TRUE;
			}
			else
			{
				xNMInfo.xNMTxMessage.OptionCode.Bits.SleepIndicate = FALSE;
			}

			xNMInfo.xNMTxMessage.OptionCode.Bits.eNMOptionType = NM_OPTION_RING;
			
			xNMInfo.NMTxCount++;	//必须放在前面，因为发送函数调用就默认成功，而不是等待读取成功返回值
			vNMWriteMessage(TRUE);
		}

		if(nmTX_COUNT_ERROR_LIMIT < xNMInfo.NMTxCount)
		{
			//进入LimpHome
			xNMInfo.isNMLimpHome = TRUE;
			xNMInfo.eNMState = NM_LIMPHOME;
			xNMInfo.NMLimpHomeDTCTimer = nmLIMPHOME_DTC_TIMEOUT;
			return;
		}
	}
	if(xNMInfo.wNMNormalMaxTimer >= nmTIMEOUT_MAX_TIMER)
	{		//reset	
		xNMInfo.isNMNormalMaxTimerStart = FALSE;
		xNMInfo.wNMNormalMaxTimer = 0;
		
		xNMInfo.eNMState = NM_INIT;
		xNMInfo.eNMInitState = NM_INIT_RESET;
	}
}

/* \brief
*/
static void vNMLimpHomeHandle(void)
{
	if(0 == xNMInfo.NMLimpHomeDTCTimer)
	{
		vDiagDTC(DIAG_DTC_NM_LIMPHOME, DIAG_DTC_CTL_FAIL);

		xNMInfo.NMLimpHomeDTCTimer = 200;
	}
	
	if(xNMInfo.wNMErrorTimer >= nmTIMEOUT_ERROR_TIMER)
	{
		xNMInfo.isNMAppActived = TRUE;
			
		if(xNMInfo.isNMNetworkBusSleep && (0 == xNMInfo.wNMSleepDelayTimer))
		{
			xNMInfo.isNMNormalMaxTimerStart = TRUE;
			xNMInfo.wNMNormalMaxTimer = 0;

			if(xNMInfo.isNMActived)
			{
				xNMInfo.xNMTxMessage.OptionCode.Bits.SleepIndicate = TRUE;
				xNMInfo.xNMTxMessage.OptionCode.Bits.eNMOptionType = NM_OPTION_LIMPHOME;

//				isNMTxLimpHomeSuccess = FALSE;		//用于从busoff恢复
				vNMWriteMessage(TRUE);
			}

			xNMInfo.eNMState = NM_PRE_SLEEP;
			xNMInfo.isNMLimpHomeSleep = TRUE;
		}
		else
		{
			xNMInfo.isNMErrorTimerStart = TRUE;
			xNMInfo.wNMErrorTimer = 0;

			if(xNMInfo.isNMActived)
			{
				xNMInfo.xNMTxMessage.OptionCode.Bits.SleepIndicate = FALSE;
				xNMInfo.xNMTxMessage.OptionCode.Bits.eNMOptionType = NM_OPTION_LIMPHOME;

//				isNMTxLimpHomeSuccess = FALSE;		//用于从busoff恢复
				vNMWriteMessage(TRUE);
			}
		}
	}
}

/* \brief
*/
static void vNMPreSleepHandle(void)
{
	if(!xNMInfo.isNMLimpHomeSleep)
	{
		//不满足睡眠条件
		if(!xNMInfo.isNMSleepAllow)
		{
			xNMInfo.xNMTxMessage.OptionCode.Bits.SleepAck = FALSE;
			
			xNMInfo.eNMState = NM_NORMAL;
			return;
		}
		if(xNMInfo.wNMTypeTimer >= nmTIMEOUT_TYPE_TIMER)
		{
			xNMInfo.isNMTypeTimerStart = FALSE;
			xNMInfo.wNMTypeTimer = 0;
			
			if(xNMInfo.isNMActived)
			{
				xNMInfo.xNMTxMessage.OptionCode.Bits.SleepAck = TRUE;
				xNMInfo.xNMTxMessage.OptionCode.Bits.eNMOptionType = NM_OPTION_RING;
				vNMWriteMessage(TRUE);

				xNMInfo.isNMNormalMaxTimerStart = TRUE;
				xNMInfo.wNMNormalMaxTimer = 0;
			}
		}
		if(xNMInfo.wNMNormalMaxTimer >= nmTIMEOUT_MAX_TIMER)
		{		//reset
			xNMInfo.isNMNormalMaxTimerStart = FALSE;
			xNMInfo.wNMNormalMaxTimer = 0;
			
			xNMInfo.eNMState = NM_INIT;
			xNMInfo.eNMInitState = NM_INIT_RESET;
		}
	}
	else
	{
		if(!xNMInfo.isNMSleepAllow)
		{
			xNMInfo.isNMNormalMaxTimerStart = FALSE;
			xNMInfo.wNMNormalMaxTimer = 0;

			xNMInfo.isNMErrorTimerStart = TRUE;
			xNMInfo.wNMErrorTimer = 0;

			xNMInfo.eNMState = NM_LIMPHOME;
			xNMInfo.NMLimpHomeDTCTimer = nmLIMPHOME_DTC_TIMEOUT;
		}
		else if(xNMInfo.wNMNormalMaxTimer >= nmTIMEOUT_MAX_TIMER)
		{
			//睡眠等待
			xNMInfo.isNMNormalMaxTimerStart = FALSE;
			xNMInfo.wNMNormalMaxTimer = 0;
			
			xNMInfo.isNMAppActived = FALSE;

			xNMInfo.wNMSleepWaitTimer = 0;
			xNMInfo.isNMSleepWaitTimerStart = TRUE;
			
			xNMInfo.eNMState = NM_SLEEP_WAIT;
		}
	}

}

