/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		ApuDriver.c
**	Abstract:		apu driver
**	Creat By:		Zeno Liu
**	Creat Time:		2015.03.05
**	Version:		v1.0
**
******************************************************************************/

/* \brief
	include files
*/
#include "ApuDriver.h"
#include "list.h"
#include "uart.h"
#include "Error.h"
#include "io.h"
#include "Debug.h"
#include "common.h"
#include <hidef.h>
#include <string.h>


/* \brief
*/
#define	apudrvWRITE_NOCONTINUOUS_FRAME					0xff
#define	apudrvWRITE_END_CONTINUOUS_FRAME				0xfe

#define	apudrvWRITE_RESPONSE_LENGTH						4

#define	apudrvREWRITE_COUNT								3

/* \brief
	enum define
*/
typedef	enum
{
	APUDRV_ACK						= 0xf0,
	APUDRV_NACK_PARITY_NG,
	APUDRV_NACK_NO_DATA
}enumApuDrvResponse;

/* \brief
	response flag
*/
typedef struct
{
	BOOL Flag;
	BYTE SeqNo;
}ApuDrvReadResponseFlag;

/* \brief
	inter variable define
*/
static List xApuDrvWriteList, xApuDrvReadList;
static ApuDrvReadResponseFlag xApuDrvReadResponseFlag;

static BYTE ApuDrvWriteBuff[apudrvWRITE_LIST_SIZE*apudrvWRITE_LIST_ELEM_LENGTH];
static BYTE ApuDrvReadBuff[apudrvREAD_LIST_SIZE];

static BYTE ApuDrvWriteMessage[apudrvWRITE_FRAME_MAX_LENGTH];
static BYTE ApuDrvReadMessage[apudrvREAD_FRAME_MAX_LENGTH];
static BYTE ApuDrvWriteResponse[apudrvWRITE_RESPONSE_LENGTH];

static BOOL isApuDrvReadMeassageOk;
static BYTE ApuDrvWriteResponseCounter;
static BYTE ApuDrvReadTimeoutTimer;
static BYTE ApuDrvReWriteDelayTimer;
static BYTE ApuDrvReWriteCounter;

static BYTE ApuDrvSequenceNo;

/* \brief
	function declaration
*/
static void vApuDrvWriteTask(void);
static void vApuDrvReadTask(void);
static BOOL blApuDrvFindFrame(void);
static BOOL blApuDrvCheckParity(BYTE length);
static void vApuDrvResponseAck(BYTE SeqNo, BYTE eAck);

/* \brief
	APU control port initialization
*/
void vApuDrvPortInit(void)
{
	ioAPU_POWER_INIT;

	ioAPU_HW_RESET_INIT;

	ioAPU_STATUS_IN_INIT;
	
	ioAPU_AWAKE_SLEEP_INIT;

}

/* \brief
*/
void vApuDrvInit(void)
{
	ApuDrvWriteMessage[0] =
	ApuDrvWriteResponse[0] = apudrvMcuAddr;
	ApuDrvWriteMessage[1] =
	ApuDrvWriteResponse[1] = apudrvApuAddr;
	
	xApuDrvWriteList.elem_length = apudrvWRITE_LIST_ELEM_LENGTH;
	xApuDrvWriteList.size = apudrvWRITE_LIST_SIZE;
	xApuDrvWriteList.base = ApuDrvWriteBuff;
	(void)xListCreate(&xApuDrvWriteList);

	xApuDrvReadList.elem_length = 1;
	xApuDrvReadList.size = apudrvREAD_LIST_SIZE;
	xApuDrvReadList.base = ApuDrvReadBuff;
	(void)xListCreate(&xApuDrvReadList);
	
	vUart2Start(&xApuDrvReadList);

	isApuDrvReadMeassageOk = FALSE;
	ApuDrvWriteResponseCounter = 0;
	ApuDrvReadTimeoutTimer = 0;
	ApuDrvReWriteDelayTimer = 0;
	ApuDrvReWriteCounter = 0;

	ApuDrvSequenceNo = 0;
}

/* \brief
*/
void vApuDrvTask(void)
{
	vApuDrvWriteTask();
	
	vApuDrvReadTask();
}

/* \brief
*/
void vApuDrvTimer(void)
{
	//apudrvREAD_TIMEOUT_TIME frame read complete,otherwise clear buffer
	if(!isListEmpty(xApuDrvReadList))
	{
		if(0 != ApuDrvReadTimeoutTimer)
		{
			if(0 == --ApuDrvReadTimeoutTimer)
			{
				++xErrorRecord.ApuDrvWaitTimeoutError;
				
				(void)xListClear(&xApuDrvReadList);
			}
		}
	}
	else
	{
		//reset timeout when buffer is empty
		ApuDrvReadTimeoutTimer = apudrvREAD_TIMEOUT_TIME;
	}

	if(0 != ApuDrvReWriteDelayTimer)
	{
		--ApuDrvReWriteDelayTimer;
	}

}

/* \brief
*/
BYTE* pApuDrvRead(void)
{
	BYTE* pData;
	
	if(isApuDrvReadMeassageOk)
	{
		pData = ApuDrvReadMessage;
		isApuDrvReadMeassageOk = FALSE;
	}
	else
	{
		pData = NULL;
	}

	return pData;
}

/* \brief
	pData:group id+sub id+data
*/
void vApuDrvWrite(BYTE GroupId, BYTE SubId, BYTE* pData, BYTE length)
{
	BYTE i = 0;
	BYTE data = 0;
	BYTE ListResult = 0;
	BYTE WriteMessage[apudrvWRITE_LIST_ELEM_LENGTH] = {0};
	BYTE ContinousIndex = 0;

	//continous frame flag
	if(apudrvFRAME_LENGTH < (length+7))
	{
		WriteMessage[0] = ContinousIndex++;

		//elem length
		WriteMessage[1] = apudrvFRAME_LENGTH-2;
	}
	else
	{
		WriteMessage[0] = apudrvWRITE_NOCONTINUOUS_FRAME;

		//elem length
		WriteMessage[1] = length+5;
	}

	//group id
	WriteMessage[4] = 
	WriteMessage[2] = GroupId;
	

	//frame length
	WriteMessage[4] ^=
	WriteMessage[3] = length + 9;

	//parity
//	WriteMessage[4] = 0x00;

	//data length
	WriteMessage[4] ^=
	WriteMessage[5] = length + 1;

	//sub id
	WriteMessage[4] ^=
	WriteMessage[6] = SubId;

	//direct write data, avoid need more RAM for ApuDrvWriteMessage;
	for(i=0; (i != length) && ((apudrvFRAME_LENGTH-7) != i); ++i)
	{
		WriteMessage[4] ^=
		WriteMessage[7+i] = *pData++;
	}

//	DisableInterrupts;
	ListResult = xListPushBack(&xApuDrvWriteList, WriteMessage);
//	if(isHardwareInitCompleted)
//	{
//		EnableInterrupts;
//	}
	
	if(LIST_OVERFLOW_ERR == ListResult)
	{
		++xErrorRecord.ApuDrvWriteBuffOverflowError;
		return;
	}
	
	if(apudrvWRITE_NOCONTINUOUS_FRAME != WriteMessage[0])
	{
		
		for(; i != length;)
		{
			int j=0;

			//adjust whether end frame or not
			if((length-i) > (apudrvWRITE_LIST_ELEM_LENGTH-3))
			{
				WriteMessage[0] = ContinousIndex++;

				//elem length
				WriteMessage[2] = apudrvWRITE_LIST_ELEM_LENGTH-3;
			}
			else
			{
				WriteMessage[0] = apudrvWRITE_END_CONTINUOUS_FRAME;

				//elem length
				WriteMessage[2] = length-i;
			}

			WriteMessage[1] = 0;
			
			for(j=0; (j!=(apudrvWRITE_LIST_ELEM_LENGTH-3)) && (i != length); ++j,++i)
			{
				WriteMessage[1] ^=
				WriteMessage[3+j] = *pData++;
			}

//			DisableInterrupts;
			ListResult = xListPushBack(&xApuDrvWriteList, WriteMessage);
//			if(isHardwareInitCompleted)
//			{
//				EnableInterrupts;
//			}
			
			if(LIST_OVERFLOW_ERR == ListResult)
			{
				++xErrorRecord.ApuDrvWriteBuffOverflowError;
				return;
			}
		}
	}

	return;
}

/* \brief
*/
void vApuDrvWriteBuffClear(void)
{
	(void)xListClear(&xApuDrvWriteList);
}

/* \brief
	
*/
static void vApuDrvWriteTask(void)
{
	if(isUart2WriteBusy())
	{		//iic busy,can not write message
		return;
	}

	if(0 != ApuDrvWriteResponseCounter)
	{		//have response write
		if(blUart2Write(ApuDrvWriteResponse, 4))//apudrvWRITE_RESPONSE_LENGTH))
		{
			--ApuDrvWriteResponseCounter;		//write response done, clear response flag
		}
	}
	else
	{
		if((0 == ApuDrvReWriteCounter)																					//not wait ack
			|| (xApuDrvReadResponseFlag.Flag && (xApuDrvReadResponseFlag.SeqNo == ApuDrvWriteMessage[2]))
			|| (apudrvREWRITE_COUNT == ApuDrvReWriteCounter))
		{
			BYTE Elem[apudrvWRITE_LIST_ELEM_LENGTH] = {0};
			BYTE Index = 0;
			
			if(LIST_OK == xListPopFront(&xApuDrvWriteList, Elem))
			{
				if(Elem[1] <= (apudrvWRITE_FRAME_MAX_LENGTH-3))
				{
					(void)memcpy(&ApuDrvWriteMessage[3], &Elem[2], Elem[1]);
				}
				else
				{
					++xErrorRecord.ApuDrvMemCopyOverflowError;
					
					ApuDrvReWriteCounter = 0;
					return;
				}

				ApuDrvWriteMessage[5] ^= (ApuDrvWriteMessage[0]^ApuDrvWriteMessage[1]);		//parity

				if(apudrvWRITE_NOCONTINUOUS_FRAME == Elem[0])
				{
					ApuDrvWriteMessage[2] = ApuDrvSequenceNo++;					//sequency No
					ApuDrvWriteMessage[5] ^= ApuDrvWriteMessage[2];
					
					ApuDrvWriteMessage[ApuDrvWriteMessage[4]-1] = 0xAA;			//oxAA for frame end
					ApuDrvWriteMessage[5] ^= 0xAA;								//parity
					ApuDrvWriteMessage[5] = ~ApuDrvWriteMessage[5];

					{
						BYTE Length = (ApuDrvWriteMessage[4]/4+1)*4;

						if(Length != ApuDrvWriteMessage[4])
						{
							BYTE i;
							for(i=ApuDrvWriteMessage[4]; i!=Length; ++i)
							{
								ApuDrvWriteMessage[i] = 0x00;
							}
						}
						
						(void)blUart2Write(ApuDrvWriteMessage, Length);
					}

					//clear response flag
					xApuDrvReadResponseFlag.Flag = TRUE;						//FALSE;
					xApuDrvReadResponseFlag.SeqNo = ApuDrvWriteMessage[2];		//0x00

					ApuDrvReWriteDelayTimer = 200;
					ApuDrvReWriteCounter = 1;
				}
				else if(0 == Elem[0])
				{	//first frame in continuous frame
					BYTE FrameNo = 1;
					
					Index += (apudrvWRITE_LIST_ELEM_LENGTH+1);

					while(LIST_OK == xListPopFront(&xApuDrvWriteList, Elem))
					{
						switch(Elem[0])
						{
							case apudrvWRITE_NOCONTINUOUS_FRAME:
								if(Elem[1] <= (apudrvWRITE_FRAME_MAX_LENGTH-3))
								{
									(void)memcpy(&ApuDrvWriteMessage[3], &Elem[2], Elem[1]);
								}
								else
								{
									++xErrorRecord.ApuDrvMemCopyOverflowError;
									
									--ApuDrvSequenceNo;
									ApuDrvReWriteCounter = 0;
									return;
								}
								
								ApuDrvWriteMessage[5] ^= (ApuDrvWriteMessage[0]^ApuDrvWriteMessage[1]);		//parity

								ApuDrvWriteMessage[2] = ApuDrvSequenceNo++;					//sequency No
								ApuDrvWriteMessage[5] ^= ApuDrvWriteMessage[2];
								
								ApuDrvWriteMessage[ApuDrvWriteMessage[4]-1] = 0xAA;			//oxAA for frame end
								ApuDrvWriteMessage[5] ^= 0xAA;								//parity
								ApuDrvWriteMessage[5] = ~ApuDrvWriteMessage[5];
								
								{
									BYTE Length = (ApuDrvWriteMessage[4]/4+1)*4;

									if(Length != ApuDrvWriteMessage[4])
									{
										BYTE i;
										for(i=ApuDrvWriteMessage[4]; i!=Length; ++i)
										{
											ApuDrvWriteMessage[i] = 0x00;
										}
									}
									
									(void)blUart2Write(ApuDrvWriteMessage, Length);
								}

								//clear response flag
								xApuDrvReadResponseFlag.Flag = TRUE;						//FALSE;
								xApuDrvReadResponseFlag.SeqNo = ApuDrvWriteMessage[2];		//0x00

								ApuDrvReWriteDelayTimer = 200;
								ApuDrvReWriteCounter = 1;

								return;

							default:
								if(Elem[0] != FrameNo)
								{		//continuous frame fail
									++xErrorRecord.ApuDrvWriteBuffFrameError;
									
									ApuDrvReWriteCounter = 0;
									return;
								}
								//continuous frame ok
								
							case apudrvWRITE_END_CONTINUOUS_FRAME:
								if(Elem[2] <= (apudrvWRITE_FRAME_MAX_LENGTH-Index))
								{
									(void)memcpy(&ApuDrvWriteMessage[Index], &Elem[3], Elem[2]);
								}
								else
								{
									++xErrorRecord.ApuDrvMemCopyOverflowError;
									
									ApuDrvReWriteCounter = 0;
									return;
								}
								ApuDrvWriteMessage[5] ^= Elem[1];

								if(apudrvWRITE_END_CONTINUOUS_FRAME == Elem[0])
								{
									ApuDrvWriteMessage[2] = ApuDrvSequenceNo++;					//sequency No
									ApuDrvWriteMessage[5] ^= ApuDrvWriteMessage[2];
									
									ApuDrvWriteMessage[ApuDrvWriteMessage[4]-1] = 0xAA;			//oxAA for frame end
									ApuDrvWriteMessage[5] ^= 0xAA;								//parity
									ApuDrvWriteMessage[5] = ~ApuDrvWriteMessage[5];
									
									{
										BYTE Length = (ApuDrvWriteMessage[4]/4+1)*4;

										if(Length != ApuDrvWriteMessage[4])
										{
											BYTE i;
											for(i=ApuDrvWriteMessage[4]; i!=Length; ++i)
											{
												ApuDrvWriteMessage[i] = 0x00;
											}
										}
										
										(void)blUart2Write(ApuDrvWriteMessage, Length);
									}

									//clear response flag
									xApuDrvReadResponseFlag.Flag = TRUE;						//FALSE;
									xApuDrvReadResponseFlag.SeqNo = ApuDrvWriteMessage[2];		//0x00

									ApuDrvReWriteDelayTimer = 200;
									ApuDrvReWriteCounter = 1;

									return;
								}
								else
								{
									++FrameNo;
									Index += (apudrvWRITE_LIST_ELEM_LENGTH-3);
									if(apudrvWRITE_FRAME_MAX_LENGTH <= Index)
									{
										ApuDrvReWriteCounter = 0;
										return;
									}
								}
								break;
						}
						// not normal end
						ApuDrvReWriteCounter = 0;
					}

					//
				}
				else
				{	//first frame is end frame or not 1, not write return
					++xErrorRecord.ApuDrvWriteBuffFrameError;
					
					ApuDrvReWriteCounter = 0;
				}
			}
			else
			{
				ApuDrvReWriteCounter = 0;
			}
		}
 		else if((0 == ApuDrvReWriteDelayTimer))
		{	//rewrite message
			{
				BYTE Length = (ApuDrvWriteMessage[4]/4+1)*4;

				if(Length != ApuDrvWriteMessage[4])
				{
					BYTE i;
					for(i=ApuDrvWriteMessage[4]; i!=Length; ++i)
					{
						ApuDrvWriteMessage[i] = 0x00;
					}
				}
				
				(void)blUart2Write(ApuDrvWriteMessage, Length);
			}

			//clear response flag
			xApuDrvReadResponseFlag.Flag = TRUE;						//FALSE;
			xApuDrvReadResponseFlag.SeqNo = ApuDrvWriteMessage[2];		//0x00

			++ApuDrvReWriteCounter;
			ApuDrvReWriteDelayTimer = 200;
		}
	}
}

/* \brief
	
*/
static void vApuDrvReadTask(void)
{
	if(!isApuDrvReadMeassageOk)
	{	//have not untreated data
		if(blApuDrvFindFrame())
		{
			isApuDrvReadMeassageOk = TRUE;

			//========debug
/*			{
			BYTE* pReadMessage = &ApuDrvReadMessage[3];
			xDebugMessage.wID = canID_DEBUG_INFO;
			xDebugMessage.data[0] = *pReadMessage;
			pReadMessage += 3;
			xDebugMessage.data[1] = *pReadMessage++;
			xDebugMessage.data[2] = *pReadMessage++;
			xDebugMessage.data[3] = *pReadMessage++;
			xDebugMessage.data[4] = *pReadMessage++;
			xDebugMessage.data[5] = *pReadMessage++;
			xDebugMessage.data[6] = *pReadMessage++;
			xDebugMessage.data[7] = *pReadMessage++;
			vDebug(8);
			}*/
			//========debug
		}
	}
	else
	{
		//reset read timeout when have untreated data
		ApuDrvReadTimeoutTimer = apudrvREAD_TIMEOUT_TIME;
	}
	
	return;
}

/* \brief
	
*/
static BOOL blApuDrvFindFrame(void)
{
	BYTE data = 0;

	if(apudrvWRITE_RESPONSE_LENGTH > wListLength(xApuDrvReadList))
	{
		return FALSE;
	}
	


	for(xListSetIterator(&xApuDrvReadList, wListBegin(xApuDrvReadList)); apudrvWRITE_RESPONSE_LENGTH <= wListLength(xApuDrvReadList); xListSetIterator(&xApuDrvReadList, wListBegin(xApuDrvReadList)))
	{
		//apuaddr
		if(LIST_OK == xListFind(xApuDrvReadList, xApuDrvReadList.itor, &data))
		{
			if(apudrvApuAddr == data)
			{
				//mcuaddr
				(void)xListIteratorIncrement(&xApuDrvReadList, 1);
				if((LIST_OK == xListFind(xApuDrvReadList, xApuDrvReadList.itor, &data)) && (apudrvMcuAddr == data))
				{
					//frame length
					(void)xListIteratorIncrement(&xApuDrvReadList, 3);
					if((LIST_OK == xListFind(xApuDrvReadList, xApuDrvReadList.itor, &data)) && (apudrvWRITE_RESPONSE_LENGTH <= data))
					{
						if(wListLength(xApuDrvReadList) >= data)
						{	//frame length ok, then check parity

							//reset read timeout when buffer have enough data
							ApuDrvReadTimeoutTimer = apudrvREAD_TIMEOUT_TIME;
							
							if(TRUE == blApuDrvCheckParity(data))
							{
	/*							if(apudrvWRITE_RESPONSE_LENGTH == data)
								{	//APU response
									xApuDrvReadResponseFlag.Flag = TRUE;
									xApuDrvReadResponseFlag.SeqNo = ApuDrvReadMessage[2];
								}
								else
								{*/
									vApuDrvResponseAck(ApuDrvReadMessage[2], APUDRV_ACK);

									return TRUE;
	//							}
							}
							else
							{
								vApuDrvResponseAck(ApuDrvReadMessage[2], APUDRV_NACK_PARITY_NG);
							}
						}
						else
						{
//							vApuDrvResponseAck(ApuDrvReadMessage[2], APUDRV_NACK_PARITY_NG+1);
							//frame length not enough, wait...
							//使用返回而不是break，防止长度不够时一直在此循环中等待而不退出此循环导致假死机
							return FALSE;
						}
					}
					else
					{
						(void)xListErase(&xApuDrvReadList, 2);			//长度不够，删除头
						vApuDrvResponseAck(ApuDrvReadMessage[2], APUDRV_NACK_PARITY_NG+2);
						//frame length not enough, wait...
						//使用返回而不是break，防止长度不够时一直在此循环中等待而不退出此循环导致假死机
						return FALSE;
					}
				}
				else
				{
					DisableInterrupts;
//					vApuDrvResponseAck(ApuDrvReadMessage[2], APUDRV_NACK_PARITY_NG+3);
					(void)xListPopFront(&xApuDrvReadList, &data);
					if(isHardwareInitCompleted)
					{
						EnableInterrupts;
					}
				}
			}
			else if(0x00 == data)
			{
				(void)xListIteratorIncrement(&xApuDrvReadList, 1);
				if(LIST_OK == xListFind(xApuDrvReadList, xApuDrvReadList.itor, &data))
				{
					if(0x00 == data)
					{
						(void)xListIteratorIncrement(&xApuDrvReadList, 1);
						if(LIST_OK == xListFind(xApuDrvReadList, xApuDrvReadList.itor, &data))
						{
							if(0xf0 == data)
							{
								(void)xListIteratorIncrement(&xApuDrvReadList, 1);
								if(LIST_OK == xListFind(xApuDrvReadList, xApuDrvReadList.itor, &data))
								{
									if(0xf0 == data)
									{
										xApuDrvReadResponseFlag.Flag = TRUE;
										DisableInterrupts;
										(void)xListErase(&xApuDrvReadList, apudrvWRITE_RESPONSE_LENGTH);
										if(isHardwareInitCompleted)
										{
											EnableInterrupts;
										}
									}
									else
									{
										DisableInterrupts;
										(void)xListErase(&xApuDrvReadList, 1);
										if(isHardwareInitCompleted)
										{
											EnableInterrupts;
										}
									}
								}
								else
								{
									DisableInterrupts;
									(void)xListErase(&xApuDrvReadList, 1);
									if(isHardwareInitCompleted)
									{
										EnableInterrupts;
									}
								}
							}
							else
							{
								DisableInterrupts;
								(void)xListErase(&xApuDrvReadList, 1);
								if(isHardwareInitCompleted)
								{
									EnableInterrupts;
								}
							}
						}
						else
						{
							DisableInterrupts;
							(void)xListErase(&xApuDrvReadList, 1);
							if(isHardwareInitCompleted)
							{
								EnableInterrupts;
							}
						}
						
					}
					else
					{
						DisableInterrupts;
						(void)xListErase(&xApuDrvReadList, 1);
						if(isHardwareInitCompleted)
						{
							EnableInterrupts;
						}
					}
				}
				else
				{
					DisableInterrupts;
					(void)xListErase(&xApuDrvReadList, 1);
					if(isHardwareInitCompleted)
					{
						EnableInterrupts;
					}
				}
			}
			else
			{
				DisableInterrupts;
	//			vApuDrvResponseAck(ApuDrvReadMessage[2], APUDRV_NACK_PARITY_NG+4);
				(void)xListPopFront(&xApuDrvReadList, &data);
				if(isHardwareInitCompleted)
				{
					EnableInterrupts;
				}
			}
		}
		else
		{
			break;
		}
	}
	
	return FALSE;
}

/* \brief
	
*/
static BOOL blApuDrvCheckParity(BYTE length)
{
	BYTE i = 0, data = 0, parity = 0;

	(void)xListSetIterator(&xApuDrvReadList, wListBegin(xApuDrvReadList));
	for(i=0; (i!=length)&&((apudrvREAD_FRAME_MAX_LENGTH)!=i); ++i,xListIteratorIncrement(&xApuDrvReadList, 1))
	{
		if(LIST_OK == xListFind(xApuDrvReadList, xApuDrvReadList.itor, &data))
		{
			ApuDrvReadMessage[i] = data;
			if(5 != i)
			{
				parity ^= ApuDrvReadMessage[i];
			}
		}
	}
	parity = ~parity;
	
	if(parity == ApuDrvReadMessage[5])
	{
		//parity ok, erase current frame
		
		(void)xListErase(&xApuDrvReadList, length);
		
		return TRUE;
	}
	else
	{	//parity fail, erase apuaddr and mcuaddr
		
		(void)xListErase(&xApuDrvReadList, 2);

		++xErrorRecord.ApuDrvParityError;
		
		return FALSE;
	}
}

/* \brief
	response ack, diff from vApuDrvWrite because sequence
*/
static void vApuDrvResponseAck(BYTE SeqNo, BYTE eAck)
{
/*	BYTE data = 0;
	BYTE ListResult = 0;

	ApuDrvWriteResponse[2] = SeqNo;
	ApuDrvWriteResponse[3] = eAck;
	ApuDrvWriteResponse[4] = apudrvWRITE_RESPONSE_LENGTH;
	ApuDrvWriteResponse[5] = ApuDrvWriteResponse[0]^ApuDrvWriteResponse[1]^ApuDrvWriteResponse[2]^ApuDrvWriteResponse[3]^ApuDrvWriteResponse[4];
	ApuDrvWriteResponse[5] = ~ApuDrvWriteResponse[5];*/
	
	(void)(SeqNo == 0);
//	if(APUDRV_ACK == eAck)
	{
		ApuDrvWriteResponse[0] = 0x00;
		ApuDrvWriteResponse[1] = 0x00;
		ApuDrvWriteResponse[2] = 0xf0;
		ApuDrvWriteResponse[3] = eAck;

		++ApuDrvWriteResponseCounter;
	}
	
	return;
}


