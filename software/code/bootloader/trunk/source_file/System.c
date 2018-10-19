/**********************************************************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		System.c
**	Abstract:		
**	Creat By:		Zeno Liu
**	Creat Time:		2015.04.13
**	Version:		v1.0
**
**********************************************************************************************************************/

/**********************************************************************************************************************
**	Include Files
**********************************************************************************************************************/
#include "System.h"
#include "DRV_COM.h"
#include "Flash.h"
#include "Timer.h"
#include "SoftTimer.h"
#include "Drv_IIC.h"
#include <MC9S08DZ60.h>

#pragma MESSAGE DISABLE C4001

/**********************************************************************************************************************
**
**	DECLARATION
**
**********************************************************************************************************************/

/**********************************************************************************************************************
**	MacDefine
**********************************************************************************************************************/
#define	SYSTEM_FLASH_ADDR_MIN				0x1900
#define	SYSTEM_FLASH_ADDR_MAX				0xEDB7
//#define	SYSTEM_FLASH_ADDR_MAX				0xf3b7

#define	SYSTEM_FLASH_SECTOR_MIN				0x19

#define	SYSTEM_FLASH_SECTOR_MAX				0xfa
#define	SYSTEM_FLASH_SECTOR_USE_MAX			0xEE		//使用了3k的保护区域 0xF4
#define	SYSTEM_FLASH_VECTOR_DUMMY			0xf9
#define	SYSTEM_FLASH_VECTOR_ADDR			0xf300
#define	SYSTEM_FLASH_SECTOR_SIZE			0x3

#define	SYSTEM_RX_TIMEOUT_TIMER				Timer_120_MS
#define	SYSTEM_OPR_RETRY_NUM				3

#define	SYSTEM_RESPONSE_ERR					TRUE
#define	SYSTEM_RESPONSE_OK					FALSE

#define	SYSTEM_RX_SENDER					0xA0
#define	SYSTEM_RX_RECEIVER					0x55
#define	SYSTEM_RX_GROUP_ID					0xB9

/**********************************************************************************************************************
**	enum
**********************************************************************************************************************/
typedef enum
{
	SYSTEM_RXMSG_IDLE,
	SYSTEM_RXMSG_SENDER,
	SYSTEM_RXMSG_RECEIVER,
	SYSTEM_RXMSG_SERIAL_NO,
	SYSTEM_RXMSG_GROUP_ID,
	SYSTEM_RXMSG_LENGTH,
	SYSTEM_RXMSG_CHECKSUM,
	SYSTEM_RXMSG_SUB_ID,
	SYSTEM_RXMSG_DATA,
	SYSTEM_RXMSG_RESPONSE
}SystemRxMsgState;

/**********************************************************************************************************************
**	enum
**********************************************************************************************************************/
typedef enum
{
	SYSTEM_FLASH_WRITE_IDLE,
	SYSTEM_FLASH_WRITE_ERASE,
	SYSTEM_FLASH_WRITE_PROGRAM
}SystemFlashWriteState;

/**********************************************************************************************************************
**	enum
**********************************************************************************************************************/
typedef enum
{
	SYSTEM_COMMAND_PROGRAM			= 0xa8,
	SYSTEM_COMMAND_ERASE			= 0xaa
}SystemCommand;

/**********************************************************************************************************************
**	enum
**********************************************************************************************************************/
typedef enum
{
	SYSTEM_ERRNO_NONE,
	SYSTEM_ERRNO_ERASE,
	SYSTEM_ERRNO_PROGRAM,
	SYSTEM_ERRNO_ERASE_START_ADDR,
	SYSTEM_ERRNO_ERASE_END_ADDR,
	SYSTEM_ERRNO_PROGRAM_ADDR,
}SystemErrNo;

/**********************************************************************************************************************
**	enum
**********************************************************************************************************************/
typedef enum
{
	SYSTEM_RESPONSE_TYPE_NORMAL,
	SYSTEM_RESPONSE_TYPE_PASSWORD,
	SYSTEM_RESPONSE_TYPE_RECEIVE,
	SYSTEM_RESPONSE_TYPE_ERROR,
	SYSTEM_RESPONSE_TYPE_ERASE,
	SYSTEM_RESPONSE_TYPE_PROGRAM,
	SYSTEM_RESPONSE_TYPE_SYNCHRONOUS
}SystemResponseType;

/**********************************************************************************************************************
**	enum
**********************************************************************************************************************/
typedef enum
{
	SYSTEM_TYPE_CMD,
	SYSTEM_TYPE_DATA,
	SYSTEM_TYPE_SYNCHRONOUS,
	SYSTEM_TYPE_PASSWORD
}SystemMsgType;

/**********************************************************************************************************************
**	struct
**********************************************************************************************************************/
typedef struct
{
	UINT08 length;
	UINT16 addr;
	UINT08 data[253];
}FlashWriteMsg;

/**********************************************************************************************************************
**	struct
**********************************************************************************************************************/
typedef struct
{
	UINT08 sender;
	UINT08 receiver;
	UINT08 SerialNo;
	UINT08 GroupID;
	UINT08 length;
	UINT08 Checksum;
	UINT08 SubID;
	UINT08 data[256];

	UINT08 length_i;
	UINT08 ChecksumTemp;
	BOOL data_flag;
	UINT08 read_cnt;
	UINT08 read_i;
}SystemRxMsg;

/**********************************************************************************************************************
**	struct
**********************************************************************************************************************/
typedef struct
{
	UINT08 sender;
	UINT08 receiver;
	UINT08 SerialNo;
	UINT08 GroupID;
	UINT08 length;
	UINT08 Checksum;
	UINT08 PayloadLength;
	UINT08 SubID;
	UINT08 data[256];
}SystemTxMsg;

/**********************************************************************************************************************
**	struct
**********************************************************************************************************************/
typedef struct
{
	SystemRxMsgState rx_state;
	SystemTxMsg tx_msg;
	SystemRxMsg rx_msg;
	SystemFlashWriteState flash_w_state;
	SystemErrNo err_no;
	
	UINT08 erase_start_sector;
	UINT08 erase_end_sector;
	UINT08 erase_sector_i;
	
	UINT16 program_addr;
	
	UINT08 rx_timeout_ms_timer;
	WORD flash_w_timeout_ms_timer;
	UINT08 opr_retry_cnt;
	BOOL isMcuUpdatePrepare;
}SystemComParaArea;

/**********************************************************************************************************************
**	struct
**********************************************************************************************************************/
typedef struct
{
	UINT16 addr;
	UINT08 length;
	UINT08 data[32];
	UINT08 crc;
	BOOL isOk;
}SystemWriteData;

/**********************************************************************************************************************
**	function declaration
**********************************************************************************************************************/
static void SystemRxTask(void);
static void SystemFlashWriteTask(void);
static void SystemDecCommand(void);
static void SystemDecMcuUpdate(void);
//static void SystemErrTask(void);
static void SystemChkFlashWCompTask(void);
static void SystemWrite(SystemTxMsg* pTxMsg);
static void SystemResponse(UINT08 SerialNo);
static void SystemReqTask(void);
static void SystemDecData(void);
static UINT08 SystemAsciiToOneByte(UINT08 para1, UINT08 para2);

typedef void (*VectFunc)(void);

extern volatile VectFunc UserResetFunc;
extern UINT08 pass_word[8];
extern UINT08 original_pass_word[8];
extern BOOL isForceWrite;

/**********************************************************************************************************************
**
**	DEFINE
**
**********************************************************************************************************************/

/**********************************************************************************************************************
**	variable define
**********************************************************************************************************************/
static SystemComParaArea system_para;
BOOL isMcuUpdate = FALSE;
UINT16 SystemUpdatePacketID = 0;
UINT08 SystemUpdateReqTimeoutTimer = 0;
BOOL isUpdateReqDataPause = FALSE;
UINT08 SystemReqState = 1;
SystemWriteData xSystemWriteData;

/**********************************************************************************************************************
**	Func Name:		void SystemTask(void)
**	Parameters:		void
**	Return Value:	void
**	Abstract:		system task
**********************************************************************************************************************/
void SystemTask(void)
{
	SystemChkFlashWCompTask();
	SystemRxTask();
	SystemDecCommand();
	SystemDecData();
	SystemFlashWriteTask();
	SystemReqTask();

	if(!systemBAT_IN)
	{
		asm(BGND);
	}
	
	return;
}

/**********************************************************************************************************************
**	Func Name:		void SystemInit(void)
**	Parameters:		void
**	Return Value:	void
**	Abstract:		system init
**********************************************************************************************************************/
void SystemInit(void)
{
#ifdef	__CMM__
	systemPOWER_INIT;
	systemMONITOR_INIT;
	systemTF_POWER_INIT
	systemAUDIO_ENABLE_INIT;
	systemLOW_POWER_IN_INIT;
	systemAPU_BACK_INIT;
	TimerWait(5000);
	systemAPU_AWAKE_SLEEP_INIT;
	systemAPU_POWER_INIT;
	TimerWait(5000);
	systemAPU_RESET_INIT;
	TimerWait(5000);
	sytemHDPOWER_INIT;
	systemRGB_POWER_INIT;
#else
/*
	systemMONITOR_VGHL_INIT;
	systemMONITOR_POWER_INIT;
//	systemAPU_HW_RESET_INIT;
//	systemUSB_DVD_POWER_INIT;
	systemAUDIO_AMP_MUTE_INIT;
	systemAUDIO_AMP_STBY_INIT;
	systemAUDIO_MUTE_INIT;
	systemACC_IN_INIT;
	systemAPU_BACK_INIT;
	systemUSB_CONTROL_HOST1_INIT;
	systemUSB_CONTROL_HOST2_INIT;
//	systemFORCE_FLASH_WRITE_INIT;
	TimerWait(5000);
	systemAPU_AWAKE_SLEEP_INIT;
	systemAPU_POWER_INIT;
	TimerWait(5000);
	systemAPU_RESET_INIT;
	TimerWait(5000);
	systemPOWER_INIT;
*/
#endif
	
	DRV_COM2PortInit();
	DRV_COM2Init();
	FlashInit();
	
	system_para.rx_state = SYSTEM_RXMSG_SENDER;
//	system_para.err_no = SYSTEM_ERRNO_NONE;

	system_para.tx_msg.sender = SYSTEM_RX_RECEIVER;
	system_para.tx_msg.receiver = SYSTEM_RX_SENDER;
	system_para.flash_w_timeout_ms_timer = Timer_20_S;
//	system_para.flash_w_state = SYSTEM_FLASH_WRITE_IDLE;
	system_para.isMcuUpdatePrepare = FALSE;
	system_para.opr_retry_cnt = 0;

//	isMcuUpdate = FALSE;
	SystemUpdatePacketID = 0;
	SystemUpdateReqTimeoutTimer = 0;
	isUpdateReqDataPause = FALSE;
//	SystemReqState = 1;

	gu08_ms_timer = 0;
	soft_timer_para.u40ms_timer = 0;

	
	//等待核心板启动
	TimerWait(50000);
	systemMONITOR_ON;
	
	return;
}

/**********************************************************************************************************************
**	Func Name:		void SystemHardwareInit(void)
**	Parameters:		void
**	Return Value:	void
**	Abstract:		system init
**********************************************************************************************************************/
void SystemHardwareInit(void)
{
	systemACC_IN_INIT;
	systemBAT_IN_INIT;
	TimerWait(5000);
	//wait for ACC ok, prevent loop of restart
	while((!systemACC_IN) || (!systemBAT_IN))
	{
		__RESET_WATCHDOG();
	}
	ioTw88xx_RESET_INIT;

	systemMONITOR_VGHL_INIT;
	systemMONITOR_POWER_INIT;
//	systemAPU_HW_RESET_INIT;
//	systemUSB_DVD_POWER_INIT;
	systemAUDIO_AMP_MUTE_INIT;
	systemAUDIO_AMP_STBY_INIT;
	systemAUDIO_MUTE_INIT;
	systemAPU_BACK_INIT;
	systemUSB_CONTROL_HOST1_INIT;
	systemUSB_CONTROL_HOST2_INIT;
//	systemFORCE_FLASH_WRITE_INIT;
	TimerWait(5000);
	systemAPU_AWAKE_SLEEP_INIT;
	systemAPU_POWER_INIT;
	TimerWait(5000);
	systemAPU_RESET_INIT;
	TimerWait(5000);
	systemPOWER_INIT;

	vIICInit(0x00);

	TimerWait(50000);
	ioTw88xx_RESET_H;
	TimerWait(50000);
	TimerWait(50000);
	TimerWait(50000);
	TimerWait(50000);
	TimerWait(50000);
	TimerWait(50000);
	TimerWait(50000);
	vIICWriteDrvConfig();
}

/**********************************************************************************************************************
**	Func Name:		void SystemInit(void)
**	Parameters:		void
**	Return Value:	void
**	Abstract:		system init
**********************************************************************************************************************/
void SystemFortyMSecTimer(void)
{
	if(system_para.rx_timeout_ms_timer != 0)
	{
		--system_para.rx_timeout_ms_timer;
	}
	
	if(system_para.flash_w_timeout_ms_timer != 0)
	{
		--system_para.flash_w_timeout_ms_timer;
	}

	if(SystemUpdateReqTimeoutTimer != 0)
	{
		--SystemUpdateReqTimeoutTimer;
	}
	
	return;
}

/**********************************************************************************************************************
**	Func Name:		static void SystemRxTask(void)
**	Parameters:		void
**	Return Value:	void
**	Abstract:		system rx from UART task
**********************************************************************************************************************/
static void SystemRxTask(void)
{
	if(system_para.rx_timeout_ms_timer == 0)
	{
		system_para.rx_state = SYSTEM_RXMSG_SENDER;
	}
	
	switch(system_para.rx_state)
	{
		case SYSTEM_RXMSG_SENDER:
			if(DRV_COM2ReadByte(&system_para.rx_msg.sender))
			{
				if(SYSTEM_RX_SENDER == system_para.rx_msg.sender)
				{
					system_para.rx_msg.ChecksumTemp = system_para.rx_msg.sender;
					system_para.rx_state = SYSTEM_RXMSG_RECEIVER;
					
					system_para.rx_timeout_ms_timer = SYSTEM_RX_TIMEOUT_TIMER;
				}
			}
			break;
			
		case SYSTEM_RXMSG_RECEIVER:
			if(DRV_COM2ReadByte(&system_para.rx_msg.receiver))
			{
				if(SYSTEM_RX_RECEIVER == system_para.rx_msg.receiver)
				{
					system_para.rx_msg.ChecksumTemp ^= system_para.rx_msg.receiver;
					system_para.rx_state = SYSTEM_RXMSG_SERIAL_NO;
				}
				else if(SYSTEM_RX_SENDER == system_para.rx_msg.receiver)
				{
					system_para.rx_msg.sender = system_para.rx_msg.receiver;
				}
				else
				{
					system_para.rx_state = SYSTEM_RXMSG_SENDER;
				}
				system_para.rx_timeout_ms_timer = SYSTEM_RX_TIMEOUT_TIMER;
			}
			break;
			
		case SYSTEM_RXMSG_SERIAL_NO:
			if(DRV_COM2ReadByte(&system_para.rx_msg.SerialNo))
			{
				system_para.rx_msg.ChecksumTemp ^= system_para.rx_msg.SerialNo;
				system_para.rx_state = SYSTEM_RXMSG_GROUP_ID;
				
				system_para.rx_timeout_ms_timer = SYSTEM_RX_TIMEOUT_TIMER;
			}
			break;
			
		case SYSTEM_RXMSG_GROUP_ID:
			if(DRV_COM2ReadByte(&system_para.rx_msg.GroupID))
			{
				system_para.rx_msg.ChecksumTemp ^= system_para.rx_msg.GroupID;
				system_para.rx_state = SYSTEM_RXMSG_LENGTH;
				system_para.rx_timeout_ms_timer = SYSTEM_RX_TIMEOUT_TIMER;
			}
			break;
			
		case SYSTEM_RXMSG_LENGTH:
			if(DRV_COM2ReadByte(&system_para.rx_msg.length))
			{
				system_para.rx_msg.ChecksumTemp ^= system_para.rx_msg.length;
				system_para.rx_state = SYSTEM_RXMSG_CHECKSUM;
				
				system_para.rx_timeout_ms_timer = SYSTEM_RX_TIMEOUT_TIMER;
			}
			break;
			
		case SYSTEM_RXMSG_CHECKSUM:
			if(DRV_COM2ReadByte(&system_para.rx_msg.Checksum))
			{
				system_para.rx_state = SYSTEM_RXMSG_SUB_ID;
				
				system_para.rx_timeout_ms_timer = SYSTEM_RX_TIMEOUT_TIMER;
			}
			break;
			
		case SYSTEM_RXMSG_SUB_ID:
			if(DRV_COM2ReadByte(&system_para.rx_msg.SubID))
			{
				system_para.rx_msg.ChecksumTemp ^= system_para.rx_msg.SubID;
				system_para.rx_state = SYSTEM_RXMSG_DATA;
				system_para.rx_msg.length_i = 0;
				
				system_para.rx_timeout_ms_timer = SYSTEM_RX_TIMEOUT_TIMER;
			}
			break;
			
		case SYSTEM_RXMSG_DATA:
			if(system_para.rx_msg.length_i == (system_para.rx_msg.length-7))
			{			//接收完毕
				system_para.rx_msg.ChecksumTemp = ~system_para.rx_msg.ChecksumTemp;
				
				if(system_para.rx_msg.ChecksumTemp == system_para.rx_msg.Checksum)
				{		//校验成功
					Printfpc("Mcu Rx:	", &system_para.rx_msg.sender, system_para.rx_msg.length);
					
					SystemResponse(system_para.rx_msg.SerialNo);
					
					system_para.rx_msg.data_flag = TRUE;
				}
				system_para.rx_state = SYSTEM_RXMSG_SENDER;
				
				break;
			}
			
			if(DRV_COM2ReadByte(&system_para.rx_msg.data[system_para.rx_msg.length_i]))
			{
				if(system_para.rx_msg.length_i < (system_para.rx_msg.length - 7))
				{
					system_para.rx_msg.ChecksumTemp ^= system_para.rx_msg.data[system_para.rx_msg.length_i];
				}
				++system_para.rx_msg.length_i;
				
				system_para.rx_timeout_ms_timer = SYSTEM_RX_TIMEOUT_TIMER;
			}
			break;
			
		default:
			break;
	}
	
	return;
}

/**********************************************************************************************************************
**	Func Name:		static void SystemFlashWriteTask(void)
**	Parameters:		void
**	Return Value:	void
**	Abstract:		system flash write task
**********************************************************************************************************************/
static void SystemFlashWriteTask(void)
{
	switch(system_para.flash_w_state)
	{
		case SYSTEM_FLASH_WRITE_IDLE:
		default:
			break;
			
		case SYSTEM_FLASH_WRITE_ERASE:
			if((system_para.erase_start_sector) <= (system_para.erase_end_sector - system_para.erase_sector_i))
			{
				if(FlashSectorErase(((UINT16)(system_para.erase_end_sector - system_para.erase_sector_i)) << 8))
				{
//					SystemResponse(SYSTEM_RESPONSE_TYPE_ERASE, system_para.erase_start_sector + system_para.erase_sector_i);
					
					system_para.erase_sector_i += 3;
					
					system_para.opr_retry_cnt = 0;
				}
				else
				{
					if(system_para.opr_retry_cnt == SYSTEM_OPR_RETRY_NUM)
					{
//						system_para.err_no = SYSTEM_ERRNO_ERASE;
						
						system_para.flash_w_state = SYSTEM_FLASH_WRITE_IDLE;
					}
					++system_para.opr_retry_cnt;
				}
			}
			else
			{
				system_para.flash_w_state = SYSTEM_FLASH_WRITE_IDLE;

			}
			
			system_para.flash_w_timeout_ms_timer = Timer_20_S;
			break;
			
		case SYSTEM_FLASH_WRITE_PROGRAM:
			if(FlashBurstProgram(xSystemWriteData.addr, &xSystemWriteData.data[0], xSystemWriteData.length))
			{
				system_para.flash_w_state = SYSTEM_FLASH_WRITE_IDLE;
			}
			else
			{
				if(system_para.opr_retry_cnt == SYSTEM_OPR_RETRY_NUM)
				{
//					system_para.err_no = SYSTEM_ERRNO_PROGRAM;
					
					system_para.flash_w_state = SYSTEM_FLASH_WRITE_IDLE;
				}
				++system_para.opr_retry_cnt;
			}
			
			system_para.flash_w_timeout_ms_timer = Timer_50_S;
			break;
	}
	
	return;
}

/**********************************************************************************************************************
**	Func Name:		void SystemDecCommand(void)
**	Parameters:		void
**	Return Value:	void
**	Abstract:		system decode command
**********************************************************************************************************************/
static void SystemDecCommand(void)
{
	if(system_para.rx_msg.data_flag)
	{
		system_para.rx_msg.data_flag = FALSE;

		switch(system_para.rx_msg.GroupID)
		{
			case 0xB9:		//MCU UPDATE
				SystemDecMcuUpdate();
				break;
				
			default:
				break;
		}
	}
	
	return;
}

/**********************************************************************************************************************
**	Func Name:		static void SystemDecMcuUpdate(void)
**	Parameters:		void
**	Return Value:	void
**	Abstract:		system decode mcu update
**********************************************************************************************************************/
static void SystemDecMcuUpdate(void)
{
	switch(system_para.rx_msg.SubID)
	{
		case 0x01:		//cmd
			if(system_para.rx_msg.data[0] == 0x02)
			{
				//confirm
				system_para.tx_msg.GroupID = 0x19;
				system_para.tx_msg.length = 0x0a;
				system_para.tx_msg.PayloadLength = 0x02;
				system_para.tx_msg.SubID = 0x02;
				system_para.tx_msg.data[0] = 0x02;
				SystemWrite(&system_para.tx_msg);

				//request key
				system_para.tx_msg.GroupID = 0x19;
				system_para.tx_msg.length = 0x09;
				system_para.tx_msg.PayloadLength = 0x01;
				system_para.tx_msg.SubID = 0x03;
				SystemWrite(&system_para.tx_msg);

				system_para.isMcuUpdatePrepare = TRUE;
				SystemReqState = 0;
			}
			break;

		case 0x02:		//key
			if(!system_para.isMcuUpdatePrepare)
			{
				break;
			}
			if((system_para.rx_msg.data[0] == 0x56) && (system_para.rx_msg.data[1] == 0xAE))
			{
				//start write
				isMcuUpdate = TRUE;
				SystemUpdatePacketID = 0;

				//start erase
				SystemReqState = 2;
			}
			break;

		case 0x03:		//data
			if((!isMcuUpdate) || (system_para.rx_msg.read_cnt != 0))
			{
				break;
			}

			if(system_para.rx_msg.data[0] == 0x02)
			{		//app
				UINT16 PacketID = system_para.rx_msg.data[2];
				PacketID <<= 8;
				PacketID |= system_para.rx_msg.data[1];
				if(SystemUpdatePacketID == PacketID)
				{
					isUpdateReqDataPause = TRUE;
					system_para.rx_msg.read_cnt = system_para.rx_msg.length-10;
					system_para.rx_msg.read_i = 3;
					
				}
			}
			break;

		case 0x04:		//checksum
			if(!isMcuUpdate)
			{
				break;
			}
			//end,reset
//			asm(BGND);
			break;

		case 0x05:		//reset ready
			if(!isMcuUpdate)
			{
				break;
			}
			break;

		case 0x06:		//config data
			if(!isMcuUpdate)
			{
				break;
			}
			break;

		case 0x07:		//request version
			system_para.tx_msg.GroupID = 0x19;
			system_para.tx_msg.length = 0x0c;
			system_para.tx_msg.PayloadLength = 0x04;
			system_para.tx_msg.SubID = 0x07;
			system_para.tx_msg.data[0] = systemVERSION_Y;
			system_para.tx_msg.data[1] = systemVERSION_M;
			system_para.tx_msg.data[2] = systemVERSION_D;
			SystemWrite(&system_para.tx_msg);
			break;

		default:
			break;
	}
}

/**********************************************************************************************************************
**	Func Name:		void SystemErrTask(void)
**	Parameters:		void
**	Return Value:	void
**	Abstract:		
**********************************************************************************************************************/
/*static void SystemErrTask(void)
{
	if(system_para.err_no != SYSTEM_ERRNO_NONE)
	{
		//error process for debug
//		SystemResponse(SYSTEM_RESPONSE_TYPE_ERROR, system_para.err_no);
		
		
		system_para.err_no = SYSTEM_ERRNO_NONE;
	}
}*/

/**********************************************************************************************************************
**	Func Name:		void SystemChkFlashWCompTask(void)
**	Parameters:		void
**	Return Value:	void
**	Abstract:		
**********************************************************************************************************************/
static void SystemChkFlashWCompTask(void)
{
	if((system_para.flash_w_timeout_ms_timer != 0))// || isForceWrite)
	{
		return;
	}
	
	if((unsigned short)UserResetFunc < SYSTEM_FLASH_ADDR_MAX)
	{
		__RESET_WATCHDOG();
		
		UserResetFunc();
	}
	else if(!systemACC_IN)
	{
		asm(BGND);
	}
	
	return;
}

/**********************************************************************************************************************
**	Func Name:		static void SystemWrite(SystemTxMsg* pTxMsg)
**	Parameters:		SystemTxMsg* pTxMsg
**	Return Value:	void
**	Abstract:		system write
**********************************************************************************************************************/
static void SystemWrite(SystemTxMsg* pTxMsg)
{
	UINT08 Checksum = 0;
	UINT08 i = 0;

	//Checksum
	Checksum = pTxMsg->sender;
	Checksum ^= pTxMsg->receiver;
	Checksum ^= pTxMsg->SerialNo;
	Checksum ^= pTxMsg->GroupID;
	Checksum ^= pTxMsg->length;
	Checksum ^= pTxMsg->PayloadLength;
	Checksum ^= pTxMsg->SubID;
	for(i=0; i!=(pTxMsg->PayloadLength-1); ++i)
	{
		Checksum ^= pTxMsg->data[i];
	}
	Checksum ^= 0xAA;
	pTxMsg->Checksum = ~Checksum;
	
	Printfpc("Mcu Tx:	", &pTxMsg->sender, pTxMsg->length-1);
	
	DRV_COM2WriteByte(pTxMsg->sender);
	DRV_COM2WriteByte(pTxMsg->receiver);
	DRV_COM2WriteByte(pTxMsg->SerialNo++);
	DRV_COM2WriteByte(pTxMsg->GroupID);
	DRV_COM2WriteByte(pTxMsg->length);
	DRV_COM2WriteByte(pTxMsg->Checksum);
	DRV_COM2WriteByte(pTxMsg->PayloadLength);
	DRV_COM2WriteByte(pTxMsg->SubID);

	for(i=0; i!=(pTxMsg->PayloadLength-1); ++i)
	{
		DRV_COM2WriteByte(pTxMsg->data[i]);
	}
	DRV_COM2WriteByte(0xAA);
		
	return;
}

/**********************************************************************************************************************
**	Func Name:		static void SystemResponse(UINT08 SerialNo)
**	Parameters:		UINT08 SerialNo
**	Return Value:	void
**	Abstract:		system response for UART
**********************************************************************************************************************/
static void SystemResponse(UINT08 SerialNo)
{
	(void)(SerialNo == 0);
	
	Printfpc("Mcu Tx:	ACK", NULL, 0);
	
	DRV_COM2WriteByte(0x00);
	DRV_COM2WriteByte(0x00);
	DRV_COM2WriteByte(0xf0);
	DRV_COM2WriteByte(0xf0);
//	DRV_COM2WriteByte(0x00);
//	DRV_COM2WriteByte(0x00);
//	DRV_COM2WriteByte(0x00);
//	DRV_COM2WriteByte(0x00);
		
	return;
}

/**********************************************************************************************************************
**	Func Name:		static void SystemReqTask(void)
**	Parameters:		void
**	Return Value:	void
**	Abstract:		request data for write
**********************************************************************************************************************/
static void SystemReqTask(void)
{
	if(0 != SystemUpdateReqTimeoutTimer)
	{
		return;
	}
	
	switch(SystemReqState)
	{
		case 1:		//request update cmd
			system_para.tx_msg.GroupID = 0x19;
			system_para.tx_msg.length = 0x0a;
			system_para.tx_msg.PayloadLength = 0x02;
			system_para.tx_msg.SubID = 0x01;
			system_para.tx_msg.data[0] = 0x02;		//app
			SystemWrite(&system_para.tx_msg);
			
			SystemUpdateReqTimeoutTimer = Timer_1000_MS;
			break;

		case 2:
			system_para.erase_start_sector = SYSTEM_FLASH_SECTOR_MIN;
	
			system_para.erase_end_sector = SYSTEM_FLASH_SECTOR_USE_MAX - 3;
			
			system_para.erase_sector_i = 0;
			system_para.opr_retry_cnt = 0;
			
			system_para.flash_w_state = SYSTEM_FLASH_WRITE_ERASE;
			SystemReqState = 3;
			break;
			
		case 3://request data
			if((!isMcuUpdate) || isUpdateReqDataPause || (system_para.flash_w_state != SYSTEM_FLASH_WRITE_IDLE))
			{
				return;
			}
			//request data
			system_para.tx_msg.GroupID = 0x19;
			system_para.tx_msg.length = 0x0c;
			system_para.tx_msg.PayloadLength = 0x04;
			system_para.tx_msg.SubID = 0x04;
			system_para.tx_msg.data[0] = 0x02;		//app
			system_para.tx_msg.data[1] = SystemUpdatePacketID&0xff;
			system_para.tx_msg.data[2] = (SystemUpdatePacketID>>8)&0xff;
			SystemWrite(&system_para.tx_msg);

			SystemUpdateReqTimeoutTimer = Timer_3000_MS;
			break;

		default:
			break;
	}
}

/**********************************************************************************************************************
**	Func Name:		static void SystemDecData(void)
**	Parameters:		void
**	Return Value:	void
**	Abstract:		decode data
**********************************************************************************************************************/
static void SystemDecData(void)
{
	static UINT08 DataIndex=0;
	static UINT08 crc=0;
	static UINT08 DecState;
	static UINT08 para1=0,para2=0;

	if(system_para.rx_msg.read_cnt == 0)
	{
		return;
	}

	for(;system_para.rx_msg.read_cnt!=0;--system_para.rx_msg.read_cnt)
	{
		__RESET_WATCHDOG();
		
		if((system_para.rx_msg.data[system_para.rx_msg.read_i] == 0x0d) || (system_para.rx_msg.data[system_para.rx_msg.read_i] == 0x0a))
		{
			if(--system_para.rx_msg.read_cnt == 0)
			{
				break;
			}
			++system_para.rx_msg.read_i;
		}
		switch(DecState)
		{
			case 0:			//'S'
				if('S' == system_para.rx_msg.data[system_para.rx_msg.read_i++])
				{
					DecState = 1;
				}
				break;

			case 1:			//'1'
				if('1' == system_para.rx_msg.data[system_para.rx_msg.read_i])
				{
					DecState = 2;
				}
				else if('9' == system_para.rx_msg.data[system_para.rx_msg.read_i])
				{
					//end.....,reset
					asm(BGND);
				}
				else
				{
					DecState = 0;
				}
				system_para.rx_msg.read_i++;
				break;

			case 2:			//length high
				para1 = system_para.rx_msg.data[system_para.rx_msg.read_i++];
				DecState = 3;
				break;

			case 3:			//length low
				para2 = system_para.rx_msg.data[system_para.rx_msg.read_i++];
				xSystemWriteData.length = SystemAsciiToOneByte(para1, para2);
				crc = xSystemWriteData.length;
				xSystemWriteData.length -= 3;
				DecState = 4;
				break;

			case 4:			//addr
				para1 = system_para.rx_msg.data[system_para.rx_msg.read_i++];
				DecState = 5;
				break;

			case 5:			//addr
				para2 = system_para.rx_msg.data[system_para.rx_msg.read_i++];
				xSystemWriteData.addr = SystemAsciiToOneByte(para1, para2);
				crc += (xSystemWriteData.addr&0xff);
				xSystemWriteData.addr <<= 8;
				DecState = 6;
				break;

			case 6:			//addr
				para1 = system_para.rx_msg.data[system_para.rx_msg.read_i++];
				DecState = 7;
				break;

			case 7:			//addr
				para2 = system_para.rx_msg.data[system_para.rx_msg.read_i++];
				xSystemWriteData.addr |= SystemAsciiToOneByte(para1, para2);
				crc += (xSystemWriteData.addr&0xff);
				DataIndex = 0;
				DecState = 8;
				break;

			case 8:
				if(DataIndex != xSystemWriteData.length)
				{
					para1 = system_para.rx_msg.data[system_para.rx_msg.read_i++];
					DecState = 9;
				}
				else
				{
					DecState = 10;
				}
				break;

			case 9:
				para2 = system_para.rx_msg.data[system_para.rx_msg.read_i++];
				xSystemWriteData.data[DataIndex] = SystemAsciiToOneByte(para1, para2);
				crc += xSystemWriteData.data[DataIndex];
				DataIndex++;
				if(DataIndex == xSystemWriteData.length)
				{
					DecState = 10;
				}
				else
				{
					DecState = 8;
				}
				break;

			case 10:
				para1 = system_para.rx_msg.data[system_para.rx_msg.read_i++];
				DecState = 11;
				break;

			case 11:
				para2 = system_para.rx_msg.data[system_para.rx_msg.read_i++];
				xSystemWriteData.crc = SystemAsciiToOneByte(para1, para2);
				crc = ~crc;

				if(crc == xSystemWriteData.crc)
				{
					system_para.flash_w_state = SYSTEM_FLASH_WRITE_PROGRAM;
//#ifdef	__DEBUG__
					if(xSystemWriteData.addr >= 0xEE00)
					{
						xSystemWriteData.addr -= 0x1200;
					}
/*#else
					if(xSystemWriteData.addr >= 0xF400)
					{
						xSystemWriteData.addr -= 0xC00;
					}
#endif*/
					if(system_para.rx_msg.read_cnt == 1)
					{
						++SystemUpdatePacketID;
						isUpdateReqDataPause = FALSE;
						SystemUpdateReqTimeoutTimer = Timer_80_MS;
					}
				}
				else
				{
					isUpdateReqDataPause = FALSE;
					SystemUpdateReqTimeoutTimer = Timer_80_MS;
				}
				DecState = 0;
				--system_para.rx_msg.read_cnt;
				return;
			

			default:
				break;
		}
	}
	++SystemUpdatePacketID;
	isUpdateReqDataPause = FALSE;
	SystemUpdateReqTimeoutTimer = Timer_80_MS;
}

/**********************************************************************************************************************
**	Func Name:		static UINT08 SystemAsciiToOneByte(UINT08 para1, UINT08 para2)
**	Parameters:		void
**	Return Value:	void
**	Abstract:		
**********************************************************************************************************************/
static UINT08 SystemAsciiToOneByte(UINT08 para1, UINT08 para2)
{
	UINT08 data=0;
	
	if((para1 <= '9') && (para1 >= '0'))
	{
		data = (para1 - '0')<<4;
	}
	else if((para1 <= 'f') && (para1 >= 'a'))
	{
		data = (para1 - 'a' + 10)<<4;
	}
	else if((para1 <= 'F') && (para1 >= 'A'))
	{
		data = (para1 - 'A' + 10)<<4;
	}

	if((para2 <= '9') && (para2 >= '0'))
	{
		data |= (para2 - '0');
	}
	else if((para2 <= 'f') && (para2 >= 'a'))
	{
		data |= (para2 - 'a' + 10);
	}
	else if((para2 <= 'F') && (para2 >= 'A'))
	{
		data |= (para2 - 'A' + 10);
	}

	return data;
}

