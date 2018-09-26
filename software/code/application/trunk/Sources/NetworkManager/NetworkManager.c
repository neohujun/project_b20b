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
	
	BYTE NMTxCount;					//���ʹ������
	BYTE NMRxCount;					//���մ������

	BOOL isNMActived;					// TRUE:�����������״̬   FALSE:�����������״̬Networkstatus.NMactive
	BOOL isNMLimpHome;					//��ǰ����״̬: TRUE:��ǰΪ����״̬��FALSE: ��ǰΪ������״̬Config.limphome
	BOOL isNMAppActived;					// ����Ӧ�ñ��ķ��ͽ���D_Online
	BOOL isNMSleepAllow;					// ��������˯��
	BOOL isNMNetworkBusSleep;			//�������нڵ㶼˯����Networkstatus.bussleep
	BOOL isNMSleep;						// ˯�߱�־
	BOOL isNMLimpHomeTxSuccess;		// �ɹ��������б���NMMerker.limphome
	BOOL isNMLimpHomeSleep;				// ����ģʽ˯�߱�־
	WORD wNMSleepDelayTimer;			//������NORMAL/LIMPHOMEģʽʱ��˯����С��ʱʱ��>5000ms,��ֹԶ�̻��Ѻ�Ƶ����˯�߻���

	BOOL isNMTypeTimerStart;				// Ttyp��ʱ������NM���Ĵ���֮��ĵ���ʱ��
	WORD wNMTypeTimer;

	BOOL isNMNormalMaxTimerStart;		// Tmax��ʱ������NM���Ĵ���֮������ʱ��
	WORD wNMNormalMaxTimer;

	BOOL isNMErrorTimerStart;				// Terror��ʱ���������б��Ĵ���֮���ʱ��
	WORD wNMErrorTimer;

	BOOL isNMSleepWaitTimerStart;		// Twbs��ʱ������˯������ȷ�ϵ����߽���˯��ģʽ֮���ʱ��
	WORD wNMSleepWaitTimer;

//	NMNode xNMNode[nmNODE_MAX];

	BOOL isBusoff;

	BYTE NMLimpHomeDTCTimer;
}NMInfo;

/* \brief
*/
NMInfo xNMInfo;
//BOOL isNMTxLimpHomeSuccess;		//���ڴ�busoff�ָ�ȷ�ϵ�һ֡���б����Ƿ��ͳɹ�
//BOOL isNMTxFirstFrameSuccess;		//�����ϵ��һ֡ȷ��can���Ϳ�ʼ�ټ�ʱ����ֹ��ʱʱ�䲻���ϱ�׼

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

				//�շ���˯��
				ioCAN_STBY_ON;
				ioCAN_EN_ON;

				xNMInfo.isNMSleep = TRUE;
				//��ʱ20ms����Ƿ�Զ�̻��ѣ���ֹ�շ�����˯��ʱ��INH��������ʱ���INH�ź�
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

	//���������������ģʽ
	xNMInfo.isNMActived = TRUE;

	//������Ӧ�ñ��ķ���
	xNMInfo.isNMAppActived = FALSE;

	//������δ���յ��κ���ϢʱĬ��������˯��״̬
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
			//���ڷ�����Զʧ��ʱϵͳ�ܹ���������
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

	//���������������ģʽ
	xNMInfo.isNMActived = TRUE;

	//������Ӧ�ñ��ķ���
	xNMInfo.isNMAppActived = FALSE;

	//������δ���յ��κ���ϢʱĬ��������˯��״̬
//	xNMInfo.isNMNetworkBusSleep = TRUE;

#if	0
	Can_SendMsgBufHead = Can_SendMsgBufTail;		//���buff
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

	//Limphome DTC���
	vDiagDTC(DIAG_DTC_NM_LIMPHOME, DIAG_DTC_CTL_SUCESS);
	
	vNMRxMessageConvert(pxCanRxMsg, &xNMRxMessage);

	//SourceIDΪBYTE�ͣ����д�������TRUE
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
								//����ģʽ��˯��ģʽ��ʼ��
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
							//����ģʽ��˯��ģʽ��ʼ��
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
				xNMInfo.NMRxCount = 0;		//���յ����Ĵ�������
				if((NM_OPTION_RING == xNMRxMessage.OptionCode.Bits.eNMOptionType) || (NM_OPTION_ALIVE == xNMRxMessage.OptionCode.Bits.eNMOptionType))
				{	//Ring/alive
					//����ȷ��Ŀ���ַ
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
									//����ALIVEʱ���ܷ���SLEEPACKΪTRUE��alvie
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
									
									//normalģʽʱ���ͳɹ�,pre_sleepģʽ������ɺ�Ҳ���л���normalģʽ����ʱTmax
									xNMInfo.NMTxCount = 0;		//���ʹ����������
								}
							}
						}
					}
				}
				else if(NM_OPTION_LIMPHOME == xNMRxMessage.OptionCode.Bits.eNMOptionType)
				{	//Limphome
					//�����ڵ������ģʽ��Ҫ��¼��ʲô��?
//						xNMInfo.xNMNode[xNMRxMessage.SourceID].isLimpHome = TRUE;
				}

				//˯���ж�
				if(NM_NORMAL == xNMInfo.eNMState)
				{
					if(xNMRxMessage.OptionCode.Bits.SleepAck)
					{
						if(xNMInfo.isNMNetworkBusSleep)
						{
							//����˯�߳�ʼ��
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
							//����˯�߳�ʼ��
							xNMInfo.isNMAppActived = FALSE;

							xNMInfo.wNMSleepWaitTimer = 0;
							xNMInfo.isNMSleepWaitTimerStart = TRUE;
							
							xNMInfo.eNMState = NM_SLEEP_WAIT;
						}
					}
					else
					{
						//������û�У���Ҫ�����ȥ��
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
								//����ģʽ��˯��ģʽ��ʼ��
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
						//����ģʽ��˯��ģʽ��ʼ��
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
		
		//��ʱ90ms����Ӧ�ñ���
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
				
				xNMInfo.NMTxCount++;	//�������ǰ�棬��Ϊ���ͺ������þ�Ĭ�ϳɹ��������ǵȴ���ȡ�ɹ�����ֵ
				vNMWriteMessage(TRUE);
//				isNMTxFirstFrameSuccess = FALSE;
//				xNMInfo.wNMDelayTimer = 100;
			}
			
			//��������ķ��ͺ��ٷ���Ӧ�ñ���
			xNMInfo.isNMAppActived = TRUE;
			
			if((xNMInfo.NMRxCount <= nmRX_COUNT_ERROR_LIMIT) && (xNMInfo.NMTxCount <= nmTX_COUNT_ERROR_LIMIT))
			{	//normal
				xNMInfo.isNMTypeTimerStart = TRUE;
				xNMInfo.wNMTypeTimer = 0;
				
				xNMInfo.eNMState = NM_NORMAL;

				//������NORMALģʽʱ��˯����С��ʱʱ��>5000ms,��ֹԶ�̻��Ѻ�Ƶ����˯�߻���
				xNMInfo.wNMSleepDelayTimer = 500;
			}
			else
			{	//LimpHome
				xNMInfo.isNMErrorTimerStart = TRUE;
				xNMInfo.wNMErrorTimer = 0;
				
				xNMInfo.eNMState = NM_LIMPHOME;
				xNMInfo.NMLimpHomeDTCTimer = nmLIMPHOME_DTC_TIMEOUT;

				//������LIMPHOMEģʽʱ��˯����С��ʱʱ��>5000ms,��ֹԶ�̻��Ѻ�Ƶ����˯�߻���
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
	//Ӧ�ñ��ĸ���һ֡���籨��Ҫ���90ms
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

	//RESETʱ������м�ʱ������ͼ��û�У�������ӵ�
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

	//����Can���ͽӿڼ����ͳɹ�
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
			xNMInfo.NMTxCount = 0;		//���ʹ����������
			if(NM_OPTION_RING == xNMInfo.xNMTxMessage.OptionCode.Bits.eNMOptionType)
			{
				//ȡ��Ttyp
				xNMInfo.isNMTypeTimerStart = FALSE;
				xNMInfo.wNMTypeTimer = 0;
				
				//���¶�ʱTmax
				xNMInfo.isNMNormalMaxTimerStart = TRUE;
				xNMInfo.wNMNormalMaxTimer = 0;

				if(xNMInfo.xNMTxMessage.OptionCode.Bits.SleepIndicate)
				{
					//HU����˯���������ͳɹ� ��ͽ���NM_PRE_SLEEP
					if(xNMInfo.isNMNetworkBusSleep)
					{
						xNMInfo.xNMTxMessage.OptionCode.Bits.SleepAck = TRUE;

						//����Ԥ˯��״̬
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

				//˯�ߵȴ�
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
		//�ڵ�����˯��
	if(xNMInfo.isNMSleepAllow && (0 == xNMInfo.wNMSleepDelayTimer))
	{
		if(!xNMInfo.isNMActived)
		{
			//����Ԥ˯��״̬
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
			
			xNMInfo.NMTxCount++;	//�������ǰ�棬��Ϊ���ͺ������þ�Ĭ�ϳɹ��������ǵȴ���ȡ�ɹ�����ֵ
			vNMWriteMessage(TRUE);
		}

		if(nmTX_COUNT_ERROR_LIMIT < xNMInfo.NMTxCount)
		{
			//����LimpHome
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

//				isNMTxLimpHomeSuccess = FALSE;		//���ڴ�busoff�ָ�
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

//				isNMTxLimpHomeSuccess = FALSE;		//���ڴ�busoff�ָ�
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
		//������˯������
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
			//˯�ߵȴ�
			xNMInfo.isNMNormalMaxTimerStart = FALSE;
			xNMInfo.wNMNormalMaxTimer = 0;
			
			xNMInfo.isNMAppActived = FALSE;

			xNMInfo.wNMSleepWaitTimer = 0;
			xNMInfo.isNMSleepWaitTimerStart = TRUE;
			
			xNMInfo.eNMState = NM_SLEEP_WAIT;
		}
	}

}

