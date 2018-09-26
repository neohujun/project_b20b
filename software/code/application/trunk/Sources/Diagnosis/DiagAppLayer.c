/******************************************************************************
**
**								Copyright (C) 2017  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		DiagAppLayer.c
**	Abstract:		diagnosis app layer
**	Creat By:		Zeno Liu
**	Creat Time:		2017.03.20
**	Version:		v1.0
**
******************************************************************************/


/* \brief
	include files
*/
#include "DiagAppLayer.h"
#include "DiagNetLayer.h"
#include "DiagDTC.h"
#include "memory.h"
#include "Apu.h"
#include "Eeprom.h"
#include "io.h"
#include "Amp.h"
#include "system.h"
#include "Ems.h"

/* \brief
*/
#define	diagappWRITE_BUFF_MAX						5

#define	diagappPOSITIVE_RESPONSE_SID_BASE			0x40
#define	diagappNEGATIVE_RESPONSE_SID					0x7f

#define	diagappWRITE_DATA_TIMEOUT					10

#define	diagappTYPE_NULL								0xff

#define	diagappDTC_GROUP_HU							0xc00000
#define	diagappDTC_GROUP_ALL							0xffffff

#define	diagappSECURITY_ACCESS_KEY_PARA				0x71F96C1D
#define	diagappSECURITY_ACCESS_RETRY					3
#define	diagappSECURITY_ACCESS_FORBID_TIMEOUT		1000

#define	diagappACTIVE_DIAG_INFO_LENGTH				3
#define	diagappPART_NUMBER_LENGTH					9
#define	diagappSYSTEM_SUPPLIER_ID_LENGTH			10
#define	diagappECU_SERIAL_NUMBER_LENGTH				16
#define	diagappECU_VIN_LENGTH							17
#define	diagappHW_VERSION_LENGTH						4
#define	diagappSW_VERSION_LENGTH						4
#define	diagappSYSTEM_NAME_LENGTH					9
#define	diagappPROGRAMMING_DATE_LENGTH				6
#define	diagappCONFIG_LENGTH							4

#define	diagappP2_CAN_SERVER_MAX						5			//服务器接收到请求消息后发出响应消息时间的性能要求
#define	diagappP2_X_CAN_SERVER_MAX					500			//当服务器在发送否定响应为0x78时，到服务器发出响应消息的时间要求
#define	diagappS3_SERVER								500			//用于非默认会话模式，如果服务器未接收到任何诊断报文，回到默认模式

/* \brief
	DID enum,enum是8bit,所有改为define
*/
#define	DIAG_APP_DID_ACTIVE_DIAGNOSTIC_INFO		0xf100		//
#define	DIAG_APP_DID_PART_NUMBER					0xf187		//零件编号
#define	DIAG_APP_DID_SYSTEM_SUPPLIER_ID				0xf18a		//供应商标识符
#define	DIAG_APP_DID_ECU_SERIAL_NUMBER				0xf18c		//ECU序列号
#define	DIAG_APP_DID_VIN								0xf190		//VIN整车识别码
#define	DIAG_APP_DID_HW_VERSION						0xf193		//ECU硬件版本
#define	DIAG_APP_DID_SW_VERSION						0xf195		//ECU软件版本
#define	DIAG_APP_DID_SYSTEM_NAME					0xf197		//
#define	DIAG_APP_DID_PROGRAMMING_DATE				0xf199		//编程日期
#define	DIAG_APP_DID_CONFIG							0x53b0		//下线配置
#define	DIAG_APP_DID_ILLUME							0x53a7		//I/O DID 背光
#define	DIAG_APP_DID_VEHICLE_SPEED					0x53a8		//I/O DID 车速

/* \brief
*/
typedef enum
{
	//诊断和通信的管理
	DIAG_APP_SID_SESSION_CONTROL			=0x10,		//诊断模式控制
	DIAG_APP_SID_ECU_RESET,								//电控单元复位
	DIAG_APP_SID_SECURITY_ACCESS			=0x27,		//安全访问
	DIAG_APP_SID_COMM_CONTROL,							//通信控制
	DIAG_APP_SID_TESTER_PRESENT				=0x3E,		//诊断设备在线
	DIAG_APP_SID_CONTROL_DTC_SETTING		=0x85,		//控制DTC设置

	//数据传输
	DIAG_APP_SID_READ_DATA_BY_ID			=0x22,		//读取数据
	DIAG_APP_SID_READ_MEMORY_BY_ADDR,					//读取内存
	DIAG_APP_SID_READ_DATA_BY_PERIODIC_ID	=0x2a,		//周期读取数据
	DIAG_APP_SID_WRITE_DATA_BY_ID			=0x2e,		//写入数据
	DIAG_APP_SID_WRITE_MEMORY_BY_ADDR		=0x3d,		//写入内存

	//已存储数据传输
	DIAG_APP_SID_CLEAR_DIAG_INFO			=0x14,		//清除诊断信息
	DIAG_APP_SID_READ_DTC_INFO				=0x19,		//读取诊断信息

	//输入输出控制
	DIAG_APP_SID_INPUT_OUTPUT_CONTROL		=0x2f,		//输入输出控制

	//上传下载
	DIAG_APP_SID_REQUEST_DOWNLOAD			=0x34,		//请求下载
	DIAG_APP_SID_TRANSFER_DATA				=0x36,		//数据传输
	DIAG_APP_SID_REQUEST_TRANSFER_EXIT,					//请求退出传输

	//例程控制
	DIAG_APP_SID_ROUTINE_CONTROL			=0x31
}enumDiagAppServiceID;

/* \brief
	诊断模式
*/
typedef enum
{
	DIAG_APP_TYPE_NONE,
	DIAG_APP_TYPE_DEFAULT,			//默认模式
	DIAG_APP_TYPE_PROGRAMMING,		//编程模式
	DIAG_APP_TYPE_EXTENDED			//扩展模式
}enumDiagAppSessionType;

/* \brief
	否定响应码NRC
*/
typedef enum
{
	DIAG_APP_NRC_UNDEFINE									=0x10,		//未定义NRC
	DIAG_APP_NRC_SERVICE_NOT_SUPPORTED,								//服务不支持
	DIAG_APP_NRC_SUBFUNC_NOT_SUPPORTED					=0x12,		//子功能不支持
	DIAG_APP_NRC_INCORRECT_MSG_LENGTH_OR_INVALIORMAT	=0x13,		//报文长度错误或者格式非法
	DIAG_APP_NRC_CONDITIONS_NOT_CORRECT					=0x22,		//条件未满足:1.服务器正处于编程模式
	DIAG_APP_NRC_REQUEST_SEQUENCE_ERROR					=0x24,		//请求次序错误，即在未收到请求种子报文时收到发送密钥报文
	DIAG_APP_NRC_REQUEST_OUT_OF_RANGE						=0x31,		//请求超出范围
	DIAG_APP_NRC_SECURITY_ACCESS_DENIED					=0x33,		//安全访问拒绝
	DIAG_APP_NRC_INVALID_KEY									=0x35,		//密钥非法
	DIAG_APP_NRC_EXCEEDED_NUMBER_OF_ATTEMPTS,						//超出安全访问尝试次数限制
	DIAG_APP_NRC_REQUIRED_TIME_DELAY_NOT_EXPIRED,						//延时时间未到
	DIAG_APP_NRC_GENERAL_PROGRAMMING_FAILURE				=0x72,		//写入失败
	DIAG_APP_NRC_SUBFUNC_NOT_SUPPORTED_IN_ACTIVE_SESSION=0x7e,		//当前诊断模式下子功能不支持
	DIAG_APP_NRC_SERVICE_NOT_SUPPORTED_IN_ACTIVE_SESSION=0x7f,		//当前诊断模式下服务不支持
}enumDiagAppNegativeResponseCode;

/* \brief
	ECU复位类型
*/
typedef enum
{
	DIAG_APP_ECU_NONE,
	DIAG_APP_ECU_HARD_RESET,					//硬件复位
	DIAG_APP_ECU_KEY_OFF_ON_RESET,			//点火钥匙复位
	DIAG_APP_ECU_SOFT_RESET					//软件复位
}enumDiagAppEcuResetType;

/* \brief
	安全访问子功能类型
*/
typedef enum
{
	DIAG_APP_SECURITY_ACCESS_NONE,
	DIAG_APP_SECURITY_ACCESS_REQUEST_SEED,					//请求种子
	DIAG_APP_SECURITY_ACCESS_SEND_KEY,						//发送密钥KEY
}enumDiagAppSecurityAccessType;

/* \brief
	通信控制的控制类型
*/
typedef enum
{
	DIAG_APP_COMM_CTL_ENABLE_RX_TX,			//允许接收和发送
	DIAG_APP_COMM_CTL_ENABLE_RX,			//允许接收禁止发送
	DIAG_APP_COMM_CTL_ENABLE_TX,			//禁止接收允许发送
	DIAG_APP_COMM_CTL_DISABLE				//禁止接收和发送
}enumDiagAppCommControlType;

/* \brief
	通信控制的通信类型
*/
typedef enum
{
	DIAG_APP_COMM_APP			=0x01,		//常规应用报文
	DIAG_APP_COMM_NM,						//网络管理报文
	DIAG_APP_COMM_APP_NM					//常规应用报文和网络管理报文
}enumDiagAppCommType;

/* \brief
	DID enum
*/
typedef enum
{
	DIAG_APP_DTC_REPORT_NUM					=0x01,		//获取与客户端定义的DTC状态屏蔽码相匹配的DTC数目
	DIAG_APP_DTC_REPORT,									//获取与客户端定义的DTC状态屏蔽码相匹配的所有DTC列表
	DIAG_APP_DTC_SNAPSHOT_ID,							//获取与客户端定义的DTC和状态屏蔽码相关的DTCSnapshot 快照数据 ID
	DIAG_APP_DTC_SNAPSHOT_RECOORD,						//获取与客户端定义的DTC和状态屏蔽码相关的DTCSnapshot快照数据记录信息
	DIAG_APP_DTC_EXTENDED_DATA				=0x06,		//获取与客户端定义的DTC相关的扩展数据
	DIAG_APP_DTC_REPORT_SUPPORTED_DTC		=0x0a,		//获取服务器支持的所有DTC的状态
}enumDiagAppDTCSubFunc;

/* \brief
	DID enum
*/
typedef enum
{
	DIAG_APP_IO_CTL_RETURN_CONTROL_TO_ECU	=0x00,		//客户端(诊断仪)不再控制输入信号，由MCU自动采集
	DIAG_APP_IO_CTL_SHORT_TERM_ADJUSTMENT	=0x03,		//输入信号由客户端控制
}enumDiagAppInputOutputControlParameter;

/* \brief
*/
typedef union
{
	DWORD Byte;
	struct
	{
		unsigned B0		:1;
		unsigned B1		:1;
		unsigned B2		:1;
		unsigned B3		:1;
		unsigned B4		:1;
		unsigned B5		:1;
		unsigned B6		:1;
		unsigned B7		:1;

		unsigned B8		:1;
		unsigned B9		:1;
		unsigned B10		:1;
		unsigned B11		:1;
		unsigned B12		:1;
		unsigned B13		:1;
		unsigned B14		:1;
		unsigned B15		:1;

		unsigned B16		:1;
		unsigned B17		:1;
		unsigned B18		:1;
		unsigned B19		:1;
		unsigned B20		:1;
		unsigned B21		:1;
		unsigned B22		:1;
		unsigned B23		:1;

		unsigned B24		:1;
		unsigned B25		:1;
		unsigned B26		:1;
		unsigned B27		:1;
		unsigned B28		:1;
		unsigned B29		:1;
		unsigned B30		:1;
		unsigned B31		:1;
	}Bits;
}unionUint32Bits;

/* \brief
	这个用于发送缓冲，但是没有buff溢出检查，所以只能确保buff永远不会满就没有问题
*/
typedef struct
{
	DiagNetMessage xDiagNetWriteMsg[diagappWRITE_BUFF_MAX];
	BYTE Head;
	BYTE Tail;
}DiagAppWriteMsgBuff;

/* \brief
*/
typedef struct
{
	DiagNetMessage* pxDiagNetReadMsg;
	DiagAppWriteMsgBuff xDiagAppWriteMsgBuff;
	enumDiagAppSessionType eSessionType;		//诊断模式

	BOOL isSPRMIB;							//是否禁止肯定响应指示
	
	BYTE EcuResetDelayTimer;				//用于接收到ECU RESET指令后的等待肯定响应发送时间

	//通信控制
	BYTE CommControl[2];					//对常规应用报文和网络管理报文的控制

	//数据传输
	WORD eDID;
	BOOL isWirteDataResponse;
	BYTE WriteDataCheckDelayTimer;
	BOOL isWirteOk;							//写入成功

	//安全访问
	BOOL isSecurityAccess;					//是否有安全访问权限
	DWORD SecurityAccessSeed;				//安全访问种子
	BOOL isSecurityAccessSeedSend;			//种子是否发送
	DWORD SecurityAccessKey;				//根据当前发送的种子得到的KEY
	BYTE SecurityAccessRetryCount;			//安全访问认证失败重试次数
	WORD SecurityAccessForbidTimer;		//安全访问认证失败禁止操作时间

	BOOL isDTCOn;

	BOOL isIODIDIllume;						//IO DID控制背光
	BOOL isIODIDSpeed;						//IO DID控制车速

	BYTE IODIDControlStateRecord[2];			//IO DID 存储控制状态记录

	WORD S3ServerTimer;
}DiagAppInfo;

/* \brief
	DID
*/
static const BYTE DiagAppActiveInfo[diagappACTIVE_DIAG_INFO_LENGTH];
static const BYTE DiagAppPartNumber[diagappPART_NUMBER_LENGTH];					//询问车厂
static const BYTE DiagAppSystemSupplierID[diagappSYSTEM_SUPPLIER_ID_LENGTH];		//询问车厂
static const BYTE DiagAppECUSerialNumber[diagappECU_SERIAL_NUMBER_LENGTH];
static const BYTE DiagAppHWVersion[] = "V1.0";
static const BYTE DiagAppSWVersion[] = "V1.0";
static const BYTE DiagAppSystemName[diagappSYSTEM_NAME_LENGTH];
static const BYTE DiagAppProgrammingDate[] = "170908";

BYTE DiagAppVIN[diagappECU_VIN_LENGTH] = {0x00};
BYTE DiagAppConfig[diagappCONFIG_LENGTH] = {0x00};

/* \brief
*/
DiagAppInfo xDiagAppInfo;

/* \brief
*/
static void vDiagAppWriteTask(void);
static void vDiagAppReadTask(void);
static void vDiagAppSubTask(void);

static void vDiagAppPositiveResponse(enumDiagAppServiceID eSID, BYTE eType, BYTE* ResponseData, BYTE ResponseDataLength);
static void vDiagAppNegativeResponse(enumDiagAppServiceID eSID, enumDiagAppNegativeResponseCode eResponseCode);
static void vDiagAppWrite(void);

static void vDiagAppSessionEnter(enumDiagAppSessionType eSessionType);
static void vDiagAppEcuRest(void);
static DWORD DiagAppSecurityAccessCalc(DWORD Seed);

static void vDiagAppReadDataByID(void);
static void vDiagAppWriteDataByID(void);
static void vDiagAppDTCReport(void);
static void vDiagAppInputOutputControl(void);

/* \brief
*/
void vDiagAppTask(void)
{
	vDiagNetTask();
	
	vDiagAppReadTask();
	vDiagAppSubTask();
	vDiagAppWriteTask();

	//超时退出非默认模式
	if(0 == xDiagAppInfo.S3ServerTimer)
	{
		if(DIAG_APP_TYPE_DEFAULT != xDiagAppInfo.eSessionType)
		{
			vDiagAppSessionEnter(DIAG_APP_TYPE_DEFAULT);
		}
	}
}

/* \brief
*/
void vDiagAppTaskInit(void)
{
	vDiagNetTaskInit(&xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Tail]);

	xDiagAppInfo.eSessionType = DIAG_APP_TYPE_DEFAULT;

	xDiagAppInfo.CommControl[DIAG_APP_COMM_APP-1] = DIAG_APP_COMM_CTL_ENABLE_RX_TX;
	xDiagAppInfo.CommControl[DIAG_APP_COMM_NM-1] = DIAG_APP_COMM_CTL_ENABLE_RX_TX;

	xDiagAppInfo.isWirteDataResponse = FALSE;
				
	xDiagAppInfo.isSecurityAccess = FALSE;
	xDiagAppInfo.isSecurityAccessSeedSend = FALSE;
	xDiagAppInfo.SecurityAccessRetryCount = 0;
	xDiagAppInfo.SecurityAccessForbidTimer = 0;

	xDiagAppInfo.isIODIDIllume = FALSE;
	xDiagAppInfo.isIODIDSpeed = FALSE;

	//默认开
	xDiagAppInfo.isDTCOn = TRUE;

	xDiagAppInfo.SecurityAccessForbidTimer = diagappSECURITY_ACCESS_FORBID_TIMEOUT;

	vDiagDTCInit();
}

/* \brief
*/
void vDiagAppTxVinTask(void)
{
	(void)xEepromReadNByte(memoryADDR_VIN, DiagAppVIN, diagappECU_VIN_LENGTH);
	vApuWrite(APUW_GID_SYS_INFO, APUW_SYSINFO_VIN_CODE, DiagAppVIN, diagappECU_VIN_LENGTH);
}

/* \brief
*/
void vDiagAppTimer(void)
{
	if(0 != xDiagAppInfo.EcuResetDelayTimer)
	{
		if(0 == --xDiagAppInfo.EcuResetDelayTimer)
		{
			vDiagAppEcuRest();
		}
	}

	if(0 != xDiagAppInfo.SecurityAccessForbidTimer)
	{
		--xDiagAppInfo.SecurityAccessForbidTimer;
	}

	++xDiagAppInfo.SecurityAccessSeed;

	if(0 != xDiagAppInfo.S3ServerTimer)
	{
		--xDiagAppInfo.S3ServerTimer;
	}

	vDiagNetTimer();
	vDiagDTCTimer();
}

/* \brief
*/
BOOL isDiagAppCanAppTx(void)
{
	if((DIAG_APP_COMM_CTL_ENABLE_RX_TX == xDiagAppInfo.CommControl[DIAG_APP_COMM_APP-1]) || (DIAG_APP_COMM_CTL_ENABLE_TX == xDiagAppInfo.CommControl[DIAG_APP_COMM_APP-1]))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/* \brief
*/
BOOL isDiagAppCanAppRx(void)
{
	if((DIAG_APP_COMM_CTL_ENABLE_RX_TX == xDiagAppInfo.CommControl[DIAG_APP_COMM_APP-1]) || (DIAG_APP_COMM_CTL_ENABLE_RX == xDiagAppInfo.CommControl[DIAG_APP_COMM_APP-1]))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/* \brief
*/
BOOL isDiagAppCanNMTx(void)
{
	if((DIAG_APP_COMM_CTL_ENABLE_RX_TX == xDiagAppInfo.CommControl[DIAG_APP_COMM_NM-1]) || (DIAG_APP_COMM_CTL_ENABLE_TX == xDiagAppInfo.CommControl[DIAG_APP_COMM_NM-1]))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/* \brief
*/
BOOL isDiagAppCanNmRx(void)
{
	if((DIAG_APP_COMM_CTL_ENABLE_RX_TX == xDiagAppInfo.CommControl[DIAG_APP_COMM_NM-1]) || (DIAG_APP_COMM_CTL_ENABLE_RX == xDiagAppInfo.CommControl[DIAG_APP_COMM_NM-1]))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/* \brief
*/
BOOL isDiagAppDtcOn(void)
{
	return xDiagAppInfo.isDTCOn;
}

/* \brief
	config info handle
*/
void vDiagAppConfigHandle(void)
{
	if(xDiagAppInfo.isWirteDataResponse)
	{
		xDiagAppInfo.isWirteOk = TRUE;
	}
	
	return;
}

/* \brief
	config info handle
*/
BOOL isDiagAppIOIllume(void)
{
	return xDiagAppInfo.isIODIDIllume;
}

/* \brief
	config info handle
*/
BOOL isDiagAppIOSpeed(void)
{
	return xDiagAppInfo.isIODIDSpeed;
}

/* \brief
*/
static void vDiagAppWriteTask(void)
{
	if(xDiagAppInfo.xDiagAppWriteMsgBuff.Head != xDiagAppInfo.xDiagAppWriteMsgBuff.Tail)
	{	//buffer不为空
		//设置长度开始发送
		xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Tail].Dlc = xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Tail].DlcTemp;
		
		vDiagNetWrite(&xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Tail]);

		if(++xDiagAppInfo.xDiagAppWriteMsgBuff.Tail >= diagappWRITE_BUFF_MAX)
		{
			xDiagAppInfo.xDiagAppWriteMsgBuff.Tail = 0;
		}
	}
}

/* \brief
*/
static void vDiagAppReadTask(void)
{
	xDiagAppInfo.pxDiagNetReadMsg = pxDiagNetRead();

	if(NULL == xDiagAppInfo.pxDiagNetReadMsg)
	{
		return;
	}

	xDiagAppInfo.S3ServerTimer = diagappS3_SERVER;

//	xDiagAppInfo.isSPRMIB = (xDiagAppInfo.pxDiagNetReadMsg->Data[1]>>7)&0x01;
//	xDiagAppInfo.pxDiagNetReadMsg->Data[1] &= 0x7f;

	switch(xDiagAppInfo.pxDiagNetReadMsg->Data[0])
	{
		case DIAG_APP_SID_SESSION_CONTROL:
			if(xDiagAppInfo.pxDiagNetReadMsg->Dlc == 1)
			{	//只有SID
				//否定响应
				vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_INCORRECT_MSG_LENGTH_OR_INVALIORMAT);
				break;
			}
			xDiagAppInfo.isSPRMIB = (xDiagAppInfo.pxDiagNetReadMsg->Data[1]>>7)&0x01;
			xDiagAppInfo.pxDiagNetReadMsg->Data[1] &= 0x7f;
			switch(xDiagAppInfo.pxDiagNetReadMsg->Data[1])
			{
				case DIAG_APP_TYPE_DEFAULT:
					if(xDiagAppInfo.pxDiagNetReadMsg->Dlc != 2)
					{	//长度非法
						//否定响应
						vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_INCORRECT_MSG_LENGTH_OR_INVALIORMAT);
						break;
					}
					vDiagAppSessionEnter(xDiagAppInfo.pxDiagNetReadMsg->Data[1]);

					if(!xDiagAppInfo.isSPRMIB)
					{
						//长度为4的NULL是因为要发送4个字节时间信息
						BYTE SessionParaRecord[4] = {0};

						SessionParaRecord[0] = (diagappP2_CAN_SERVER_MAX>>8)&0xff;
						SessionParaRecord[1] = diagappP2_CAN_SERVER_MAX&0xff;
						SessionParaRecord[2] = (diagappP2_X_CAN_SERVER_MAX>>8)&0xff;
						SessionParaRecord[3] = diagappP2_X_CAN_SERVER_MAX&0xff;
						
						vDiagAppPositiveResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], xDiagAppInfo.pxDiagNetReadMsg->Data[1], SessionParaRecord, 4);
					}
					break;

				case DIAG_APP_TYPE_EXTENDED:
					if(xDiagAppInfo.pxDiagNetReadMsg->Dlc != 2)
					{	//长度非法
						//否定响应
						vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_INCORRECT_MSG_LENGTH_OR_INVALIORMAT);
						break;
					}
					if(DIAG_APP_TYPE_PROGRAMMING != xDiagAppInfo.eSessionType)
					{
						vDiagAppSessionEnter(xDiagAppInfo.pxDiagNetReadMsg->Data[1]);

						if(!xDiagAppInfo.isSPRMIB)
						{
							//长度为4的NULL是因为要发送4个字节时间信息
							BYTE SessionParaRecord[4] = {0};

							SessionParaRecord[0] = (diagappP2_CAN_SERVER_MAX>>8)&0xff;
							SessionParaRecord[1] = diagappP2_CAN_SERVER_MAX&0xff;
							SessionParaRecord[2] = (diagappP2_X_CAN_SERVER_MAX>>8)&0xff;
							SessionParaRecord[3] = diagappP2_X_CAN_SERVER_MAX&0xff;
							vDiagAppPositiveResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], xDiagAppInfo.pxDiagNetReadMsg->Data[1], SessionParaRecord, 4);
						}
					}
					else
					{
						vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_SUBFUNC_NOT_SUPPORTED_IN_ACTIVE_SESSION);
					}
					break;

				case DIAG_APP_TYPE_PROGRAMMING:
				default:
					//子功能不支持
					if(!xDiagAppInfo.pxDiagNetReadMsg->isFunc)
					{
						vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_SUBFUNC_NOT_SUPPORTED);
					}
					break;
			}
			break;

		case DIAG_APP_SID_ECU_RESET:
			if(xDiagAppInfo.pxDiagNetReadMsg->Dlc == 1)
			{	//只有SID
				//否定响应
				vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_INCORRECT_MSG_LENGTH_OR_INVALIORMAT);
				break;
			}
			xDiagAppInfo.isSPRMIB = (xDiagAppInfo.pxDiagNetReadMsg->Data[1]>>7)&0x01;
			xDiagAppInfo.pxDiagNetReadMsg->Data[1] &= 0x7f;
			switch(xDiagAppInfo.pxDiagNetReadMsg->Data[1])
			{
				case DIAG_APP_ECU_HARD_RESET:
				case DIAG_APP_ECU_SOFT_RESET:
					if(xDiagAppInfo.pxDiagNetReadMsg->Dlc != 2)
					{	//长度非法
						//否定响应
						vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_INCORRECT_MSG_LENGTH_OR_INVALIORMAT);
						break;
					}
					if(!xDiagAppInfo.isSPRMIB)
					{
						vDiagAppPositiveResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], xDiagAppInfo.pxDiagNetReadMsg->Data[1], NULL, 0);
					}
					xDiagAppInfo.EcuResetDelayTimer = 30;//5;		//静音需要200ms,所以延时用300ms

					//关屏关背光，延时等待静音完成
					ioLCD_BL_OFF;
					vAmpMuteHardware(ON);
					break;
					
				case DIAG_APP_ECU_KEY_OFF_ON_RESET:
					//此功能不支持
					vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_SUBFUNC_NOT_SUPPORTED);
					break;
					
				default:
					//子功能不支持
					if(!xDiagAppInfo.pxDiagNetReadMsg->isFunc)
					{
						vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_SUBFUNC_NOT_SUPPORTED);
					}
					break;
			}
			break;

		case DIAG_APP_SID_SECURITY_ACCESS:
			if(xDiagAppInfo.pxDiagNetReadMsg->isFunc)
			{	//功能寻址不支持此服务
				vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_SERVICE_NOT_SUPPORTED);
				break;
			}
			if(DIAG_APP_TYPE_EXTENDED != xDiagAppInfo.eSessionType)
			{
				vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_SERVICE_NOT_SUPPORTED_IN_ACTIVE_SESSION);
				
				break;
			}
			if(xDiagAppInfo.pxDiagNetReadMsg->Dlc == 1)
			{	//只有SID
				//否定响应
				vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_INCORRECT_MSG_LENGTH_OR_INVALIORMAT);
				break;
			}
			xDiagAppInfo.isSPRMIB = (xDiagAppInfo.pxDiagNetReadMsg->Data[1]>>7)&0x01;
			xDiagAppInfo.pxDiagNetReadMsg->Data[1] &= 0x7f;
			switch(xDiagAppInfo.pxDiagNetReadMsg->Data[1])
			{
				case DIAG_APP_SECURITY_ACCESS_REQUEST_SEED:
					if(xDiagAppInfo.pxDiagNetReadMsg->Dlc != 2)
					{	//长度非法
						//否定响应
						vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_INCORRECT_MSG_LENGTH_OR_INVALIORMAT);
					}
					else if(0 != xDiagAppInfo.SecurityAccessForbidTimer)
					{
						vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_REQUIRED_TIME_DELAY_NOT_EXPIRED);
					}
					else
					{
						BYTE Seed[4]={0x00,0x00,0x00,0x00};

						if(!xDiagAppInfo.isSecurityAccess)
						{
							xDiagAppInfo.SecurityAccessKey = DiagAppSecurityAccessCalc(xDiagAppInfo.SecurityAccessSeed);
							xDiagAppInfo.isSecurityAccessSeedSend = TRUE;

							Seed[0] = (BYTE)(xDiagAppInfo.SecurityAccessSeed>>24);
							Seed[1] = (BYTE)(xDiagAppInfo.SecurityAccessSeed>>16);
							Seed[2] = (BYTE)(xDiagAppInfo.SecurityAccessSeed>>8);
							Seed[3] = (BYTE)(xDiagAppInfo.SecurityAccessSeed);
						}
//						if(!xDiagAppInfo.isSPRMIB)
						{
							vDiagAppPositiveResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], xDiagAppInfo.pxDiagNetReadMsg->Data[1], &Seed[0], 4);
						}
					}
					break;

				case DIAG_APP_SECURITY_ACCESS_SEND_KEY:
					if(xDiagAppInfo.pxDiagNetReadMsg->Dlc != 6)
					{	//长度非法
						//否定响应
						vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_INCORRECT_MSG_LENGTH_OR_INVALIORMAT);
					}
					else if(!xDiagAppInfo.isSecurityAccessSeedSend)
					{
						vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_REQUEST_SEQUENCE_ERROR);
					}
					else if(0 != xDiagAppInfo.SecurityAccessForbidTimer)
					{
						vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_REQUIRED_TIME_DELAY_NOT_EXPIRED);
					}
					else
					{
						DWORD Key = ((DWORD)xDiagAppInfo.pxDiagNetReadMsg->Data[2]<<24)|((DWORD)xDiagAppInfo.pxDiagNetReadMsg->Data[3]<<16)
							|((DWORD)xDiagAppInfo.pxDiagNetReadMsg->Data[4]<<8)|xDiagAppInfo.pxDiagNetReadMsg->Data[5];
						//此处还要验证密钥，密钥验证成功后才能发送肯定响应
						if(Key == xDiagAppInfo.SecurityAccessKey)
						{
							xDiagAppInfo.SecurityAccessRetryCount = 0;
							xDiagAppInfo.isSecurityAccess = TRUE;
//							if(!xDiagAppInfo.isSPRMIB)
							{
								vDiagAppPositiveResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], xDiagAppInfo.pxDiagNetReadMsg->Data[1], NULL, 0);
							}
						}
						else
						{
							xDiagAppInfo.isSecurityAccess = FALSE;
							if(++xDiagAppInfo.SecurityAccessRetryCount >= diagappSECURITY_ACCESS_RETRY)
							{
								xDiagAppInfo.SecurityAccessRetryCount = 0;
								xDiagAppInfo.SecurityAccessForbidTimer = diagappSECURITY_ACCESS_FORBID_TIMEOUT;
								vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_EXCEEDED_NUMBER_OF_ATTEMPTS);
							}
							else
							{
								vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_INVALID_KEY);
							}
						}
					}
					break;

				default:
					//子功能不支持
					vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_SUBFUNC_NOT_SUPPORTED);
					break;
			}
			break;

		case DIAG_APP_SID_COMM_CONTROL:
			if(DIAG_APP_TYPE_EXTENDED != xDiagAppInfo.eSessionType)
			{
				vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_SERVICE_NOT_SUPPORTED_IN_ACTIVE_SESSION);
				
				break;
			}
			if(xDiagAppInfo.pxDiagNetReadMsg->Dlc == 1)
			{	//只有SID
				//否定响应
				vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_INCORRECT_MSG_LENGTH_OR_INVALIORMAT);
				break;
			}
			
			xDiagAppInfo.isSPRMIB = (xDiagAppInfo.pxDiagNetReadMsg->Data[1]>>7)&0x01;
			xDiagAppInfo.pxDiagNetReadMsg->Data[1] &= 0x7f;
			switch(xDiagAppInfo.pxDiagNetReadMsg->Data[2])
			{
				case DIAG_APP_COMM_APP:
				case DIAG_APP_COMM_NM:
					if((DIAG_APP_COMM_CTL_DISABLE == xDiagAppInfo.pxDiagNetReadMsg->Data[1]) || (DIAG_APP_COMM_CTL_ENABLE_RX_TX == xDiagAppInfo.pxDiagNetReadMsg->Data[1]))
					{
						if(xDiagAppInfo.pxDiagNetReadMsg->Dlc != 3)
						{	//长度非法
							//否定响应
							vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_INCORRECT_MSG_LENGTH_OR_INVALIORMAT);
							break;
						}
						xDiagAppInfo.CommControl[xDiagAppInfo.pxDiagNetReadMsg->Data[2]-1] = xDiagAppInfo.pxDiagNetReadMsg->Data[1];
						if(!xDiagAppInfo.isSPRMIB)
						{
							vDiagAppPositiveResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], xDiagAppInfo.pxDiagNetReadMsg->Data[1], NULL, 0);
						}
					}
					else
					{
						if(!xDiagAppInfo.pxDiagNetReadMsg->isFunc)
						{
							vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_SUBFUNC_NOT_SUPPORTED);
						}
					}
					break;

				case DIAG_APP_COMM_APP_NM:
					if((DIAG_APP_COMM_CTL_DISABLE == xDiagAppInfo.pxDiagNetReadMsg->Data[1]) || (DIAG_APP_COMM_CTL_ENABLE_RX_TX == xDiagAppInfo.pxDiagNetReadMsg->Data[1]))
					{
						if(xDiagAppInfo.pxDiagNetReadMsg->Dlc != 3)
						{	//长度非法
							//否定响应
							vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_INCORRECT_MSG_LENGTH_OR_INVALIORMAT);
							break;
						}
						xDiagAppInfo.CommControl[DIAG_APP_COMM_APP-1] = xDiagAppInfo.pxDiagNetReadMsg->Data[1];
						xDiagAppInfo.CommControl[DIAG_APP_COMM_NM-1] = xDiagAppInfo.pxDiagNetReadMsg->Data[1];
						if(!xDiagAppInfo.isSPRMIB)
						{
							vDiagAppPositiveResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], xDiagAppInfo.pxDiagNetReadMsg->Data[1], NULL, 0);
						}
					}
					else
					{
						if(!xDiagAppInfo.pxDiagNetReadMsg->isFunc)
						{
							vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_SUBFUNC_NOT_SUPPORTED);
						}
					}
					break;

				default:
					if((DIAG_APP_COMM_CTL_DISABLE == xDiagAppInfo.pxDiagNetReadMsg->Data[1]) || (DIAG_APP_COMM_CTL_ENABLE_RX_TX == xDiagAppInfo.pxDiagNetReadMsg->Data[1]))
					{
						if(xDiagAppInfo.pxDiagNetReadMsg->Dlc != 3)
						{	//长度非法
							//否定响应
							vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_INCORRECT_MSG_LENGTH_OR_INVALIORMAT);
							break;
						}
						vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_REQUEST_OUT_OF_RANGE);
					}
					else
					{
						if(!xDiagAppInfo.pxDiagNetReadMsg->isFunc)
						{
							vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_SUBFUNC_NOT_SUPPORTED);
						}
					}
					break;
			}
			break;
			
		case DIAG_APP_SID_TESTER_PRESENT:
			if(xDiagAppInfo.pxDiagNetReadMsg->Dlc == 1)
			{	//只有SID
				//否定响应
				vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_INCORRECT_MSG_LENGTH_OR_INVALIORMAT);
				break;
			}
			xDiagAppInfo.isSPRMIB = (xDiagAppInfo.pxDiagNetReadMsg->Data[1]>>7)&0x01;
			xDiagAppInfo.pxDiagNetReadMsg->Data[1] &= 0x7f;
			if(xDiagAppInfo.pxDiagNetReadMsg->Data[1] == 0x00)
			{
				if(xDiagAppInfo.pxDiagNetReadMsg->Dlc != 2)
				{	//长度非法
					//否定响应
					vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_INCORRECT_MSG_LENGTH_OR_INVALIORMAT);
					break;
				}
				if(!xDiagAppInfo.isSPRMIB)
				{	//没有禁止发送肯定应答
					vDiagAppPositiveResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], xDiagAppInfo.pxDiagNetReadMsg->Data[1], NULL, 0);
				}
			}
			else
			{
				if(!xDiagAppInfo.pxDiagNetReadMsg->isFunc)
				{
					vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_SUBFUNC_NOT_SUPPORTED);
				}
			}
			break;
			
		case DIAG_APP_SID_CONTROL_DTC_SETTING:
			if(DIAG_APP_TYPE_EXTENDED != xDiagAppInfo.eSessionType)
			{
				vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_SERVICE_NOT_SUPPORTED_IN_ACTIVE_SESSION);
				
				break;
			}
			if(xDiagAppInfo.pxDiagNetReadMsg->Dlc == 1)
			{	//只有SID
				//否定响应
				vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_INCORRECT_MSG_LENGTH_OR_INVALIORMAT);
				break;
			}
			
			xDiagAppInfo.isSPRMIB = (xDiagAppInfo.pxDiagNetReadMsg->Data[1]>>7)&0x01;
			xDiagAppInfo.pxDiagNetReadMsg->Data[1] &= 0x7f;
			switch(xDiagAppInfo.pxDiagNetReadMsg->Data[1])
			{
				case 0x01:
					if(xDiagAppInfo.pxDiagNetReadMsg->Dlc != 2)
					{	//长度非法
						//否定响应
						vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_INCORRECT_MSG_LENGTH_OR_INVALIORMAT);
						break;
					}
					xDiagAppInfo.isDTCOn = TRUE;
					if(!xDiagAppInfo.isSPRMIB)
					{
						vDiagAppPositiveResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], xDiagAppInfo.pxDiagNetReadMsg->Data[1], NULL, 0);
					}
					break;

				case 0x02:
					if(xDiagAppInfo.pxDiagNetReadMsg->Dlc != 2)
					{	//长度非法
						//否定响应
						vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_INCORRECT_MSG_LENGTH_OR_INVALIORMAT);
						break;
					}
					xDiagAppInfo.isDTCOn = FALSE;
					if(!xDiagAppInfo.isSPRMIB)
					{
						vDiagAppPositiveResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], xDiagAppInfo.pxDiagNetReadMsg->Data[1], NULL, 0);
					}
					break;

				default:
					if(!xDiagAppInfo.pxDiagNetReadMsg->isFunc)
					{
						vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_SUBFUNC_NOT_SUPPORTED);
					}
					break;
			}
			break;
			
		case DIAG_APP_SID_READ_DATA_BY_ID:
			vDiagAppReadDataByID();
			break;
			
		case DIAG_APP_SID_READ_MEMORY_BY_ADDR:
		case DIAG_APP_SID_READ_DATA_BY_PERIODIC_ID:
			if(xDiagAppInfo.pxDiagNetReadMsg->isFunc)
			{	//功能寻址不响应
				break;
			}
			//发送子功能不支持?
			vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_SERVICE_NOT_SUPPORTED);
			break;
			
		case DIAG_APP_SID_WRITE_DATA_BY_ID:
			vDiagAppWriteDataByID();
			break;
			
		case DIAG_APP_SID_WRITE_MEMORY_BY_ADDR:
			if(xDiagAppInfo.pxDiagNetReadMsg->isFunc)
			{	//功能寻址不响应
				break;
			}
			//发送子功能不支持?
			vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_SERVICE_NOT_SUPPORTED);
			break;
			
		case DIAG_APP_SID_CLEAR_DIAG_INFO:
			if(xDiagAppInfo.pxDiagNetReadMsg->Dlc != 4)
			{	//长度非法
				//否定响应
				vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_INCORRECT_MSG_LENGTH_OR_INVALIORMAT);
				break;
			}
			if(blDiagDTCClear(&xDiagAppInfo.pxDiagNetReadMsg->Data[1]))
			{
				vDiagAppPositiveResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], diagappTYPE_NULL, NULL, 0);
			}
			else
			{
				if(!xDiagAppInfo.pxDiagNetReadMsg->isFunc)
				{
					vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_REQUEST_OUT_OF_RANGE);
				}
			}
			break;
			
		case DIAG_APP_SID_READ_DTC_INFO:
			if(xDiagAppInfo.pxDiagNetReadMsg->Dlc == 1)
			{	//只有SID
				//否定响应
				vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_INCORRECT_MSG_LENGTH_OR_INVALIORMAT);
				break;
			}
			xDiagAppInfo.isSPRMIB = (xDiagAppInfo.pxDiagNetReadMsg->Data[1]>>7)&0x01;
			xDiagAppInfo.pxDiagNetReadMsg->Data[1] &= 0x7f;
			switch(xDiagAppInfo.pxDiagNetReadMsg->Data[1])
			{
				case DIAG_APP_DTC_REPORT_NUM:
					if(xDiagAppInfo.pxDiagNetReadMsg->Dlc != 3)
					{	//长度非法
						//否定响应
						vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_INCORRECT_MSG_LENGTH_OR_INVALIORMAT);
						break;
					}
					{
						WORD DTCNum = getDiagDTCNum(xDiagAppInfo.pxDiagNetReadMsg->Data[2]&diagdtcSTATUS_AVAILABILITY_MASK);
						BYTE Data[4] = {0};

						Data[0] = diagdtcSTATUS_AVAILABILITY_MASK;
						Data[1] = 0x00;			//ISO15031
						Data[2] = (BYTE)(DTCNum>>8);
						Data[3] = (BYTE)DTCNum;

//						if(!xDiagAppInfo.isSPRMIB)
						{
							vDiagAppPositiveResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], xDiagAppInfo.pxDiagNetReadMsg->Data[1], Data, 4);
						}
					}
					break;

				case DIAG_APP_DTC_REPORT:
					vDiagAppDTCReport();
					break;

				case DIAG_APP_DTC_SNAPSHOT_ID:
				case DIAG_APP_DTC_SNAPSHOT_RECOORD:
				case DIAG_APP_DTC_EXTENDED_DATA:
				default:
					if(!xDiagAppInfo.pxDiagNetReadMsg->isFunc)
					{
						vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_SUBFUNC_NOT_SUPPORTED);
					}
					break;

				case DIAG_APP_DTC_REPORT_SUPPORTED_DTC:
					if(xDiagAppInfo.pxDiagNetReadMsg->Dlc != 2)
					{	//长度非法
						//否定响应
						vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_INCORRECT_MSG_LENGTH_OR_INVALIORMAT);
						break;
					}
					{
						WORD DTCNum = getDiagAllDTC(&xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].Data[3]);

						xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].Data[2] = diagdtcSTATUS_AVAILABILITY_MASK;			//无掩码
//						if(!xDiagAppInfo.isSPRMIB)
						{
							vDiagAppPositiveResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], xDiagAppInfo.pxDiagNetReadMsg->Data[1], &xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].Data[2], DTCNum+1);
						}
					}
					break;
			}
			break;
			
		case DIAG_APP_SID_INPUT_OUTPUT_CONTROL:
			vDiagAppInputOutputControl();
			break;
			
		case DIAG_APP_SID_REQUEST_DOWNLOAD:
		case DIAG_APP_SID_TRANSFER_DATA:
		case DIAG_APP_SID_REQUEST_TRANSFER_EXIT:
		case DIAG_APP_SID_ROUTINE_CONTROL:
		default:
			if(xDiagAppInfo.pxDiagNetReadMsg->isFunc)
			{	//功能寻址不响应
				break;
			}
			//发送子功能不支持?
			vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_SERVICE_NOT_SUPPORTED);
			break;
	}
}

/* \brief
	用于一些延时应答的消息处理，但此任务必须放在vDiagAppWriteTask前，要不会可能导致发送2次
*/
static void vDiagAppSubTask(void)
{
	//写DID操作
	if(xDiagAppInfo.isWirteDataResponse)
	{
		if(xDiagAppInfo.isWirteOk)
		{
			BYTE DID[2];
			
			xDiagAppInfo.isWirteDataResponse = FALSE;

			DID[0] = xDiagAppInfo.eDID>>8;
			DID[1] = (BYTE)xDiagAppInfo.eDID;

			vDiagAppPositiveResponse(DIAG_APP_SID_WRITE_DATA_BY_ID, DID[0], &DID[1], 1);
		}
		else if(0 == xDiagAppInfo.WriteDataCheckDelayTimer)
		{		//写入数据超时失败
			xDiagAppInfo.isWirteDataResponse = FALSE;
			
			vDiagAppNegativeResponse(DIAG_APP_SID_WRITE_DATA_BY_ID, DIAG_APP_NRC_GENERAL_PROGRAMMING_FAILURE);
		}
	}
	else
	{
		xDiagAppInfo.isWirteOk = FALSE;
	}
}

/* \brief
*/
static void vDiagAppPositiveResponse(enumDiagAppServiceID eSID, BYTE eType, BYTE* ResponseData, BYTE ResponseDataLength)
{
	BYTE i=0;

	if(diagappTYPE_NULL == eType)
	{
		xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].DlcTemp = 1+ResponseDataLength;
	}
	else
	{
		xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].DlcTemp = 2+ResponseDataLength;
	}
	xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].Data[0] = eSID+diagappPOSITIVE_RESPONSE_SID_BASE;
	if(diagappTYPE_NULL != eType)
	{
		xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].Data[1] = eType;
	}
	else
	{
		xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].Data[1] = 0;
	}

	if(NULL != ResponseData)
	{
		for(i=0; i!=ResponseDataLength; ++i)
		{
			xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].Data[2+i] = ResponseData[i];
		}
	}
	else
	{
		for(i=0; i!=ResponseDataLength; ++i)
		{
			xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].Data[2+i] = 0x00;
		}
	}
	
	vDiagAppWrite();
}

/* \brief
*/
static void vDiagAppNegativeResponse(enumDiagAppServiceID eSID, enumDiagAppNegativeResponseCode eResponseCode)
{
	xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].DlcTemp = 3;
	xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].Data[0] = diagappNEGATIVE_RESPONSE_SID;
	xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].Data[1] = eSID;
	xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].Data[2] = eResponseCode;
	vDiagAppWrite();
}

/* \brief
*/
static void vDiagAppWrite(void)
{
	if(++xDiagAppInfo.xDiagAppWriteMsgBuff.Head >= diagappWRITE_BUFF_MAX)
	{
		xDiagAppInfo.xDiagAppWriteMsgBuff.Head = 0;
	}
	return;
}

/* \brief
*/
static void vDiagAppSessionEnter(enumDiagAppSessionType eSessionType)
{
	switch(xDiagAppInfo.eSessionType)
	{
		case DIAG_APP_TYPE_DEFAULT:
			if(DIAG_APP_TYPE_DEFAULT == eSessionType)
			{
				xDiagAppInfo.CommControl[DIAG_APP_COMM_APP-1] = DIAG_APP_COMM_CTL_ENABLE_RX_TX;
				xDiagAppInfo.CommControl[DIAG_APP_COMM_NM-1] = DIAG_APP_COMM_CTL_ENABLE_RX_TX;

				xDiagAppInfo.isWirteDataResponse = FALSE;
				
				xDiagAppInfo.isSecurityAccess = FALSE;
				xDiagAppInfo.isSecurityAccessSeedSend = FALSE;
				xDiagAppInfo.SecurityAccessRetryCount = 0;
				xDiagAppInfo.SecurityAccessForbidTimer = 0;
			}
			xDiagAppInfo.eSessionType = eSessionType;
			break;
			
		case DIAG_APP_TYPE_PROGRAMMING:
		case DIAG_APP_TYPE_EXTENDED:
			if(DIAG_APP_TYPE_DEFAULT == eSessionType)
			{
				xDiagAppInfo.isSecurityAccess = FALSE;
				xDiagAppInfo.isSecurityAccessSeedSend = FALSE;
				xDiagAppInfo.SecurityAccessRetryCount = 0;
				xDiagAppInfo.SecurityAccessForbidTimer = 0;

				xDiagAppInfo.CommControl[DIAG_APP_COMM_APP-1] = DIAG_APP_COMM_CTL_ENABLE_RX_TX;
				xDiagAppInfo.CommControl[DIAG_APP_COMM_NM-1] = DIAG_APP_COMM_CTL_ENABLE_RX_TX;

				//0x2a配置的周期调度被禁止
				//0x85设置复位
				xDiagAppInfo.isDTCOn = TRUE;
			}
			else
			{
				xDiagAppInfo.isSecurityAccess = FALSE;
				xDiagAppInfo.isSecurityAccessSeedSend = FALSE;
				xDiagAppInfo.SecurityAccessRetryCount = 0;
				xDiagAppInfo.SecurityAccessForbidTimer = 0;
			}
			xDiagAppInfo.eSessionType = eSessionType;
			break;

		default:
			break;
	}
}

/* \brief
*/
static void vDiagAppEcuRest(void)
{
	asm(BGND);		//illegal instruction caused mcu reset
	
	return;
}

/* \brief
*/
static DWORD DiagAppSecurityAccessCalc(DWORD Seed)
{
	#define	diagappSECURITY_MASK	0xA926E3F6
	
	DWORD Key=0;
	BYTE i=0;
	
	if(0 != Seed)
	{
		for (i=0; i<35; i++)
		{
			if (Seed & 0x80000000)
			{
				Seed <<= 1;
				Seed ^= diagappSECURITY_MASK;
			}
			else
			{
				Seed <<= 1;	
			}
		}
		Key = Seed;
	}
	return Key;
#if	0
	//江铃3J1算法，跟长安算法相同
	unionUint32Bits uUint32Bits;
	DWORD Key[2];
	BYTE BitTemp=0;

	Key[0] = Seed^diagappSECURITY_ACCESS_KEY_PARA;
	uUint32Bits.Byte = Seed;

	//bit0~15
	BitTemp = uUint32Bits.Bits.B0;
	uUint32Bits.Bits.B0 = uUint32Bits.Bits.B31;
	uUint32Bits.Bits.B31 = BitTemp;

	BitTemp = uUint32Bits.Bits.B1;
	uUint32Bits.Bits.B1 = uUint32Bits.Bits.B30;
	uUint32Bits.Bits.B30 = BitTemp;

	BitTemp = uUint32Bits.Bits.B2;
	uUint32Bits.Bits.B2 = uUint32Bits.Bits.B29;
	uUint32Bits.Bits.B29 = BitTemp;

	BitTemp = uUint32Bits.Bits.B3;
	uUint32Bits.Bits.B3 = uUint32Bits.Bits.B28;
	uUint32Bits.Bits.B28 = BitTemp;

	BitTemp = uUint32Bits.Bits.B4;
	uUint32Bits.Bits.B4 = uUint32Bits.Bits.B27;
	uUint32Bits.Bits.B27 = BitTemp;

	BitTemp = uUint32Bits.Bits.B5;
	uUint32Bits.Bits.B5 = uUint32Bits.Bits.B26;
	uUint32Bits.Bits.B26 = BitTemp;

	BitTemp = uUint32Bits.Bits.B6;
	uUint32Bits.Bits.B6 = uUint32Bits.Bits.B25;
	uUint32Bits.Bits.B25 = BitTemp;

	BitTemp = uUint32Bits.Bits.B7;
	uUint32Bits.Bits.B7 = uUint32Bits.Bits.B24;
	uUint32Bits.Bits.B24 = BitTemp;

	BitTemp = uUint32Bits.Bits.B8;
	uUint32Bits.Bits.B8 = uUint32Bits.Bits.B23;
	uUint32Bits.Bits.B23 = BitTemp;

	BitTemp = uUint32Bits.Bits.B9;
	uUint32Bits.Bits.B9 = uUint32Bits.Bits.B22;
	uUint32Bits.Bits.B22 = BitTemp;

	BitTemp = uUint32Bits.Bits.B10;
	uUint32Bits.Bits.B10 = uUint32Bits.Bits.B21;
	uUint32Bits.Bits.B21 = BitTemp;

	BitTemp = uUint32Bits.Bits.B11;
	uUint32Bits.Bits.B11 = uUint32Bits.Bits.B20;
	uUint32Bits.Bits.B20 = BitTemp;

	BitTemp = uUint32Bits.Bits.B12;
	uUint32Bits.Bits.B12 = uUint32Bits.Bits.B19;
	uUint32Bits.Bits.B19 = BitTemp;

	BitTemp = uUint32Bits.Bits.B13;
	uUint32Bits.Bits.B13 = uUint32Bits.Bits.B18;
	uUint32Bits.Bits.B18 = BitTemp;

	BitTemp = uUint32Bits.Bits.B14;
	uUint32Bits.Bits.B14 = uUint32Bits.Bits.B17;
	uUint32Bits.Bits.B17 = BitTemp;

	BitTemp = uUint32Bits.Bits.B15;
	uUint32Bits.Bits.B15 = uUint32Bits.Bits.B16;
	uUint32Bits.Bits.B16 = BitTemp;

	Key[1] = uUint32Bits.Byte^diagappSECURITY_ACCESS_KEY_PARA;
	
	return (Key[0]+Key[1]);
#endif
}

/* \brief
*/
static void vDiagAppReadDataByID(void)
{
	if(xDiagAppInfo.pxDiagNetReadMsg->Dlc < 3)
	{	//长度非法
		//否定响应
		vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_INCORRECT_MSG_LENGTH_OR_INVALIORMAT);
		return;
	}
	else if((xDiagAppInfo.pxDiagNetReadMsg->Dlc%2) == 0)
	{	//长度非法
		//否定响应
		vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_INCORRECT_MSG_LENGTH_OR_INVALIORMAT);
		return;
	}
	xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].DlcTemp = 1;
	xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].Data[0] = xDiagAppInfo.pxDiagNetReadMsg->Data[0]+diagappPOSITIVE_RESPONSE_SID_BASE;
	{
		BYTE i=0,j=1,t=0;
		for(i=0; i!=((xDiagAppInfo.pxDiagNetReadMsg->Dlc-1)/2); ++i)
		{
			WORD DID = xDiagAppInfo.pxDiagNetReadMsg->Data[1+2*i]<<8|xDiagAppInfo.pxDiagNetReadMsg->Data[2+2*i];
			switch(DID)
			{
				case DIAG_APP_DID_ACTIVE_DIAGNOSTIC_INFO:
					xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].DlcTemp += (diagappACTIVE_DIAG_INFO_LENGTH+2);;
					xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].Data[j++] = xDiagAppInfo.pxDiagNetReadMsg->Data[1+2*i];
					xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].Data[j++] = xDiagAppInfo.pxDiagNetReadMsg->Data[2+2*i];

					for(t=0; t!=diagappACTIVE_DIAG_INFO_LENGTH; ++t)
					{
						xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].Data[j++] = DiagAppActiveInfo[t];
					}
					break;
					
				case DIAG_APP_DID_SYSTEM_NAME:
					xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].DlcTemp += (diagappSYSTEM_NAME_LENGTH+2);
					xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].Data[j++] = xDiagAppInfo.pxDiagNetReadMsg->Data[1+2*i];
					xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].Data[j++] = xDiagAppInfo.pxDiagNetReadMsg->Data[2+2*i];

					for(t=0; t!=diagappSYSTEM_NAME_LENGTH; ++t)
					{
						xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].Data[j++] = DiagAppSystemName[t];
					}
					break;
					
				case DIAG_APP_DID_PROGRAMMING_DATE:
					xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].DlcTemp += (diagappPROGRAMMING_DATE_LENGTH+2);
					xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].Data[j++] = xDiagAppInfo.pxDiagNetReadMsg->Data[1+2*i];
					xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].Data[j++] = xDiagAppInfo.pxDiagNetReadMsg->Data[2+2*i];
					
					for(t=0; t!=diagappPROGRAMMING_DATE_LENGTH; ++t)
					{
						xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].Data[j++] = DiagAppProgrammingDate[t];
					}
					break;
					
				case DIAG_APP_DID_SW_VERSION:
					xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].DlcTemp += (diagappSW_VERSION_LENGTH+2);
					xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].Data[j++] = xDiagAppInfo.pxDiagNetReadMsg->Data[1+2*i];
					xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].Data[j++] = xDiagAppInfo.pxDiagNetReadMsg->Data[2+2*i];

					for(t=0; t!=diagappSW_VERSION_LENGTH; ++t)
					{
						xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].Data[j++] = DiagAppSWVersion[t];
					}
					break;
					
				case DIAG_APP_DID_HW_VERSION:
					xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].DlcTemp += (diagappHW_VERSION_LENGTH+2);
					xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].Data[j++] = xDiagAppInfo.pxDiagNetReadMsg->Data[1+2*i];
					xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].Data[j++] = xDiagAppInfo.pxDiagNetReadMsg->Data[2+2*i];

					for(t=0; t!=diagappHW_VERSION_LENGTH; ++t)
					{
						xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].Data[j++] = DiagAppHWVersion[t];
					}
					break;
					
				case DIAG_APP_DID_PART_NUMBER:
					xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].DlcTemp += (diagappPART_NUMBER_LENGTH+2);
					xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].Data[j++] = xDiagAppInfo.pxDiagNetReadMsg->Data[1+2*i];
					xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].Data[j++] = xDiagAppInfo.pxDiagNetReadMsg->Data[2+2*i];
					
					for(t=0; t!=diagappPART_NUMBER_LENGTH; ++t)
					{
						xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].Data[j++] = DiagAppPartNumber[t];
					}
					break;
					
				case DIAG_APP_DID_SYSTEM_SUPPLIER_ID:
					xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].DlcTemp += (diagappSYSTEM_SUPPLIER_ID_LENGTH+2);
					xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].Data[j++] = xDiagAppInfo.pxDiagNetReadMsg->Data[1+2*i];
					xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].Data[j++] = xDiagAppInfo.pxDiagNetReadMsg->Data[2+2*i];
					
					for(t=0; t!=diagappSYSTEM_SUPPLIER_ID_LENGTH; ++t)
					{
						xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].Data[j++] = DiagAppSystemSupplierID[t];
					}
					break;
					
				case DIAG_APP_DID_ECU_SERIAL_NUMBER:
					xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].DlcTemp += (diagappECU_SERIAL_NUMBER_LENGTH+2);
					xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].Data[j++] = xDiagAppInfo.pxDiagNetReadMsg->Data[1+2*i];
					xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].Data[j++] = xDiagAppInfo.pxDiagNetReadMsg->Data[2+2*i];

					for(t=0; t!=diagappECU_SERIAL_NUMBER_LENGTH; ++t)
					{
						xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].Data[j++] = DiagAppECUSerialNumber[t];
					}
					break;
					
				case DIAG_APP_DID_VIN:
					xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].DlcTemp += (diagappECU_VIN_LENGTH+2);
					xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].Data[j++] = xDiagAppInfo.pxDiagNetReadMsg->Data[1+2*i];
					xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].Data[j++] = xDiagAppInfo.pxDiagNetReadMsg->Data[2+2*i];

					(void)xEepromReadNByte(memoryADDR_VIN, DiagAppVIN, diagappECU_VIN_LENGTH);

					for(t=0; t!=diagappECU_VIN_LENGTH; ++t)
					{
						xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].Data[j++] = DiagAppVIN[t];
					}
					break;
				
				case DIAG_APP_DID_CONFIG:
					xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].DlcTemp += (diagappCONFIG_LENGTH+2);
					xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].Data[j++] = xDiagAppInfo.pxDiagNetReadMsg->Data[1+2*i];
					xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].Data[j++] = xDiagAppInfo.pxDiagNetReadMsg->Data[2+2*i];

					(void)xEepromReadNByte(memoryADDR_CONFIG, DiagAppConfig, diagappCONFIG_LENGTH);
					
					for(t=0; t!=diagappCONFIG_LENGTH; ++t)
					{
						xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].Data[j++] = DiagAppConfig[t];
					}
					break;

				default:
					if(!xDiagAppInfo.pxDiagNetReadMsg->isFunc)
					{
						vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_REQUEST_OUT_OF_RANGE);
					}
					return;
			}
		}
	}
	vDiagAppWrite();
}

/* \brief
*/
static void vDiagAppWriteDataByID(void)
{
	if(xDiagAppInfo.pxDiagNetReadMsg->isFunc)
	{	//功能寻址不支持此服务
		vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_SERVICE_NOT_SUPPORTED);
		return;
	}
	if(DIAG_APP_TYPE_EXTENDED != xDiagAppInfo.eSessionType)
	{
		vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_SERVICE_NOT_SUPPORTED_IN_ACTIVE_SESSION);
		return;
	}
	if(xDiagAppInfo.pxDiagNetReadMsg->Dlc == 1)
	{	//只有SID
		//否定响应
		vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_INCORRECT_MSG_LENGTH_OR_INVALIORMAT);
		return;
	}
	if(!xDiagAppInfo.isSecurityAccess)
	{
		vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_SECURITY_ACCESS_DENIED);
		return;
	}
	
	{
		xDiagAppInfo.eDID = xDiagAppInfo.pxDiagNetReadMsg->Data[1]<<8|xDiagAppInfo.pxDiagNetReadMsg->Data[2];
		switch(xDiagAppInfo.eDID)
		{
			case DIAG_APP_DID_VIN:
				if((diagappECU_VIN_LENGTH+3) != xDiagAppInfo.pxDiagNetReadMsg->Dlc)
				{	//长度非法
					//否定响应
					vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_INCORRECT_MSG_LENGTH_OR_INVALIORMAT);
					break;
				}
				if(EEPROM_OK == xEepromWriteNByte(memoryADDR_VIN, &xDiagAppInfo.pxDiagNetReadMsg->Data[3], diagappECU_VIN_LENGTH))
				{
					vApuWrite(APUW_GID_SYS_INFO, APUW_SYSINFO_VIN_CODE, &xDiagAppInfo.pxDiagNetReadMsg->Data[3], diagappECU_VIN_LENGTH);
					vDiagAppPositiveResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], xDiagAppInfo.pxDiagNetReadMsg->Data[1], &xDiagAppInfo.pxDiagNetReadMsg->Data[2], 1);
				}
				else if(0 == xDiagAppInfo.WriteDataCheckDelayTimer)
				{		//写入数据失败
					vDiagAppNegativeResponse(DIAG_APP_SID_WRITE_DATA_BY_ID, DIAG_APP_NRC_GENERAL_PROGRAMMING_FAILURE);
				}
				break;
				
			case DIAG_APP_DID_CONFIG:
				if((diagappCONFIG_LENGTH+3) != xDiagAppInfo.pxDiagNetReadMsg->Dlc)
				{	//长度非法
					//否定响应
					vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_INCORRECT_MSG_LENGTH_OR_INVALIORMAT);
					break;
				}
				if(EEPROM_OK == xEepromWriteNByte(memoryADDR_CONFIG, &xDiagAppInfo.pxDiagNetReadMsg->Data[3], diagappCONFIG_LENGTH))
				{
					BYTE Data[diagappCONFIG_LENGTH+1] ={0};
					BYTE i=0;

					Data[0] = 0x01;
					for(i=0; i!=diagappCONFIG_LENGTH; ++i)
					{
						Data[1+i] = xDiagAppInfo.pxDiagNetReadMsg->Data[3+i];
					}
					vApuWrite(APUW_GID_CAN_INFO, 0x0b, Data, diagappCONFIG_LENGTH+1);
					
					vDiagAppPositiveResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], xDiagAppInfo.pxDiagNetReadMsg->Data[1], &xDiagAppInfo.pxDiagNetReadMsg->Data[2], 1);
				}
				else if(0 == xDiagAppInfo.WriteDataCheckDelayTimer)
				{		//写入数据失败
					vDiagAppNegativeResponse(DIAG_APP_SID_WRITE_DATA_BY_ID, DIAG_APP_NRC_GENERAL_PROGRAMMING_FAILURE);
				}
				break;

			default:
				vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_REQUEST_OUT_OF_RANGE);
				break;
		}
	}

}

/* \brief
*/
static void vDiagAppDTCReport(void)
{
	WORD DTCNum;
	
	if(xDiagAppInfo.pxDiagNetReadMsg->Dlc != 3)
	{	//长度非法
		//否定响应
		vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_INCORRECT_MSG_LENGTH_OR_INVALIORMAT);
		return;
	}

	if(!xDiagAppInfo.isSPRMIB)
	{
		DTCNum = getDiagDTC(xDiagAppInfo.pxDiagNetReadMsg->Data[2]&diagdtcSTATUS_AVAILABILITY_MASK, &xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].Data[3]);

		xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].DlcTemp = 3+DTCNum;
		xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].Data[0] = xDiagAppInfo.pxDiagNetReadMsg->Data[0]+diagappPOSITIVE_RESPONSE_SID_BASE;
		xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].Data[1] = xDiagAppInfo.pxDiagNetReadMsg->Data[1];
		xDiagAppInfo.xDiagAppWriteMsgBuff.xDiagNetWriteMsg[xDiagAppInfo.xDiagAppWriteMsgBuff.Head].Data[2] = diagdtcSTATUS_AVAILABILITY_MASK;
		
		vDiagAppWrite();
	}
}

/* \brief
*/
static void vDiagAppInputOutputControl(void)
{
	if(xDiagAppInfo.pxDiagNetReadMsg->isFunc)
	{	//功能寻址不支持此服务
		vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_SERVICE_NOT_SUPPORTED);
		return;
	}
	if(DIAG_APP_TYPE_EXTENDED != xDiagAppInfo.eSessionType)
	{
		vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_SERVICE_NOT_SUPPORTED_IN_ACTIVE_SESSION);
		return;
	}
	if(xDiagAppInfo.pxDiagNetReadMsg->Dlc == 1)
	{	//只有SID
		//否定响应
		vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_INCORRECT_MSG_LENGTH_OR_INVALIORMAT);
		return;
	}
	if(!xDiagAppInfo.isSecurityAccess)
	{
		vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_SECURITY_ACCESS_DENIED);
		return;
	}

	xDiagAppInfo.eDID = xDiagAppInfo.pxDiagNetReadMsg->Data[1]<<8|xDiagAppInfo.pxDiagNetReadMsg->Data[2];
	switch(xDiagAppInfo.eDID)
	{
		case DIAG_APP_DID_ILLUME:
			switch(xDiagAppInfo.pxDiagNetReadMsg->Data[3])
			{
				case DIAG_APP_IO_CTL_RETURN_CONTROL_TO_ECU:
					if(5 != xDiagAppInfo.pxDiagNetReadMsg->Dlc)
					{	//长度非法
						//否定响应
						vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_INCORRECT_MSG_LENGTH_OR_INVALIORMAT);
						break;
					}
					if(isMonitorOn)
					{
						vPwmMonitorControl(TRUE, xApuSetting.xApuScreen.ScreenBrightness);
					}
					else
					{
						vPwmMonitorControl(FALSE, xApuSetting.xApuScreen.ScreenBrightness);
					}
					xDiagAppInfo.isIODIDIllume = FALSE;
					xDiagAppInfo.pxDiagNetReadMsg->Data[3] = xDiagAppInfo.IODIDControlStateRecord[0];
					vDiagAppPositiveResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], xDiagAppInfo.pxDiagNetReadMsg->Data[1], &xDiagAppInfo.pxDiagNetReadMsg->Data[2], 3);
					break;

				case DIAG_APP_IO_CTL_SHORT_TERM_ADJUSTMENT:
					if(6 != xDiagAppInfo.pxDiagNetReadMsg->Dlc)
					{	//长度非法
						//否定响应
						vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_INCORRECT_MSG_LENGTH_OR_INVALIORMAT);
						break;
					}
					if((!isMonitorOn) || (SYSTEM_ACC_IDLE != vSystemAccStatus()))
					{
						vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_CONDITIONS_NOT_CORRECT);
						break;
					}
					xDiagAppInfo.IODIDControlStateRecord[0] = xDiagAppInfo.pxDiagNetReadMsg->Data[4];
					if(0 == xDiagAppInfo.pxDiagNetReadMsg->Data[4])
					{
						vPwmMonitorControl(FALSE, xApuSetting.xApuScreen.ScreenBrightness);
					}
					else
					{
						vPwmMonitorControl(TRUE, xDiagAppInfo.pxDiagNetReadMsg->Data[4]*2);
					}
					xDiagAppInfo.isIODIDIllume = TRUE;
					vDiagAppPositiveResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], xDiagAppInfo.pxDiagNetReadMsg->Data[1], &xDiagAppInfo.pxDiagNetReadMsg->Data[2], 3);
					break;

				default:
					vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_REQUEST_OUT_OF_RANGE);
					break;
			}
			break;
			
		case DIAG_APP_DID_VEHICLE_SPEED:
			switch(xDiagAppInfo.pxDiagNetReadMsg->Data[3])
			{
				case DIAG_APP_IO_CTL_RETURN_CONTROL_TO_ECU:
					if(5 != xDiagAppInfo.pxDiagNetReadMsg->Dlc)
					{	//长度非法
						//否定响应
						vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_INCORRECT_MSG_LENGTH_OR_INVALIORMAT);
						break;
					}
					
					xDiagAppInfo.isIODIDSpeed = FALSE;

					xDiagAppInfo.pxDiagNetReadMsg->Data[3] = xDiagAppInfo.IODIDControlStateRecord[1];
					//恢复时自动根据can的数据进行恢复
					vDiagAppPositiveResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], xDiagAppInfo.pxDiagNetReadMsg->Data[1], &xDiagAppInfo.pxDiagNetReadMsg->Data[2], 3);
					break;

				case DIAG_APP_IO_CTL_SHORT_TERM_ADJUSTMENT:
					if(6 != xDiagAppInfo.pxDiagNetReadMsg->Dlc)
					{	//长度非法
						//否定响应
						vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_INCORRECT_MSG_LENGTH_OR_INVALIORMAT);
						break;
					}
					if(SYSTEM_ACC_IDLE != vSystemAccStatus())
					{
						vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_CONDITIONS_NOT_CORRECT);
						break;
					}
					xDiagAppInfo.IODIDControlStateRecord[1] = xDiagAppInfo.pxDiagNetReadMsg->Data[4];
					if(blCanEmsSpeed(xDiagAppInfo.pxDiagNetReadMsg->Data[4]*10))
					{
						xDiagAppInfo.isIODIDSpeed = TRUE;
						vDiagAppPositiveResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], xDiagAppInfo.pxDiagNetReadMsg->Data[1], &xDiagAppInfo.pxDiagNetReadMsg->Data[2], 3);
					}
					else
					{
						vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_CONDITIONS_NOT_CORRECT);
					}
					break;

				default:
					vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_REQUEST_OUT_OF_RANGE);
					break;
			}
			break;
			
		default:
			vDiagAppNegativeResponse(xDiagAppInfo.pxDiagNetReadMsg->Data[0], DIAG_APP_NRC_REQUEST_OUT_OF_RANGE);
			break;
	}
}

