/**********************************************************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		LibCOMConfig.h
**	Abstract:		
**	Creat By:		Zeno Liu
**	Creat Time:		2015.04.13
**	Version:		v1.0
**
**********************************************************************************************************************/

#ifndef	_LIB_COM_CONFIG_H_
#define	_LIB_COM_CONFIG_H_

/**********************************************************************************************************************
**	library UART config
**********************************************************************************************************************/

/*************Switch UART BPS*****************/
#define DRV_COM_BUS_CLK				16000000			//unit:Hz

#define	DRV_COM1_BPS				115200				//unit:Hz
#define	DRV_COM2_BPS				38400				//unit:Hz

/*************Switch DEBUG PORT*****************/
#define	DRV_COM1_DEBUG				FALSE	//COM1
#define	DRV_COM2_DEBUG				TRUE	//COM2

//=========================COM1
#define	DRV_COM1_INIT_START			(SCI1C2 = 0x00)
// = DRV_COM_BUS_CLK/(16 * DRV_COM1_BPS)	115200kbps
#define	DRV_COM1_BPS_SEL			{SCI1BDH = 0x00;\
									 SCI1BDL = 0x09;}
#define DRV_COM1_DATA_MODE			{SCI1C1 = 0x00;\
									 SCI1C3 = 0x00;}
#define	DRV_COM1_ENABLE				(SCI1C2 = 0x0C)	//发送器接收器使能，接收器使能


#define	DRV_COM1GetRxBuff()			(SCI1D)
#define	DRV_COM1SetTxBuff(data)		(SCI1D = data)
#define	DRV_COM1IsTxBuffEmpty()		(SCI1S1_TDRE)
#define	DRV_COM1GetTxCompleteFlg()	(SCI1S1_TC)
#define	DRV_COM1IsRxErr()			(SCI1S1 & 0x0f)

//=========================COM1
#define	DRV_COM2_TX_PORT_MODE_SEL		
#define	DRV_COM2_RX_PORT_MODE_SEL

#define	DRV_COM2_INIT_START			(SCI2C2 = 0x00)
// = DRV_COM_BUS_CLK/(16 * DRV_COM1_BPS)	115200kbps
#define	DRV_COM2_BPS_SEL			{SCI2BDH = 0x00;\
									 SCI2BDL = 0x09;}
#define DRV_COM2_DATA_MODE			{SCI2C1 = 0x00;\
									 SCI2C3 = 0x00;}
#define	DRV_COM2_ENABLE				(SCI2C2 = 0x0C)	//发送器接收器使能，接收器使能


#define	DRV_COM2GetRxBuff()			(SCI2D)
#define	DRV_COM2SetTxBuff(data)		(SCI2D = data)
#define	DRV_COM2IsTxBuffEmpty()		(SCI2S1_TDRE)
#define	DRV_COM2GetTxCompleteFlg()	(SCI2S1_TC)
#define	DRV_COM2IsRxErr()			(SCI2S1 & 0x0f)
#define	DRV_COM2_IRQ_COMPLETE

//=========================COM2

#endif