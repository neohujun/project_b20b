/**********************************************************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		Drv_COM.c
**	Abstract:		
**	Creat By:		Zeno Liu
**	Creat Time:		2015.04.13
**	Version:		v1.0
**
**********************************************************************************************************************/

/**********************************************************************************************************************
**	pragma
**********************************************************************************************************************/
//#pragma interrupt INTSR0 DRV_UARTINTSR0Handle
//#pragma interrupt INTST0 DRV_UARTINTST0Handle

/**********************************************************************************************************************
**	Include Files
**********************************************************************************************************************/
#include "Drv_COM.h"
#include <MC9S08DZ60.h>


/**********************************************************************************************************************
**	Func Name:		void DRV_COM1Init(void)
**	Parameters:		void
**	Return Value:	void
**	Abstract:		COM1 initialization
**********************************************************************************************************************/
void DRV_COM1Init(void)
{
	DRV_COM1_INIT_START;
	DRV_COM1_BPS_SEL;
	DRV_COM1_DATA_MODE;
	DRV_COM1_ENABLE;
		
	return;
}

/**********************************************************************************************************************
**	Func Name:		BOOL DRV_COM1WriteByte(UINT08 data)
**	Parameters:		UINT08 data
**	Return Value:	1: success		0:fail
**	Abstract:		COM1 write 1 byte data
**********************************************************************************************************************/
BOOL DRV_COM1WriteByte(UINT08 data)
{
	UINT16 u16_time_out_counter;
	
	for(u16_time_out_counter = 0; !DRV_COM1IsTxBuffEmpty(); ++u16_time_out_counter)
	{
		__RESET_WATCHDOG();
		if(u16_time_out_counter >= TIME_OUT_COUNTER)
		{
			return FALSE;
		}
	}
	
	DRV_COM1SetTxBuff(data);
	
	for(u16_time_out_counter = 0; !DRV_COM1GetTxCompleteFlg(); ++u16_time_out_counter)
	{
		__RESET_WATCHDOG();
		if(u16_time_out_counter >= TIME_OUT_COUNTER)
		{
			return FALSE;
		}
	}
	
	return TRUE;
}

/**********************************************************************************************************************
**	Func Name:		void DRV_COM2PortInit(void)
**	Parameters:		void
**	Return Value:	void
**	Abstract:		COM2 port initialization
**********************************************************************************************************************/
void DRV_COM2PortInit(void)
{
	DRV_COM2_TX_PORT_MODE_SEL;
	DRV_COM2_RX_PORT_MODE_SEL;
	
	return;
}

/**********************************************************************************************************************
**	Func Name:		void DRV_COM2Init(void)
**	Parameters:		void
**	Return Value:	void
**	Abstract:		COM2 initialization
**********************************************************************************************************************/
void DRV_COM2Init(void)
{
	DRV_COM2_INIT_START;
	DRV_COM2_BPS_SEL;
	DRV_COM2_DATA_MODE;
	DRV_COM2_ENABLE;
		
	return;
}

/**********************************************************************************************************************
**	Func Name:		BOOL DRV_COM2WriteByte(UINT08 data)
**	Parameters:		UINT08 data
**	Return Value:	1: success		0:fail
**	Abstract:		COM2 write 1 byte data
**********************************************************************************************************************/
BOOL DRV_COM2WriteByte(UINT08 data)
{
	UINT16 u16_time_out_counter;
	
	for(u16_time_out_counter = 0; !DRV_COM2IsTxBuffEmpty(); ++u16_time_out_counter)
	{
		__RESET_WATCHDOG();
		if(u16_time_out_counter >= TIME_OUT_COUNTER)
		{
			return FALSE;
		}
	}
	
	DRV_COM2SetTxBuff(data);
	
	for(u16_time_out_counter = 0; !DRV_COM2GetTxCompleteFlg(); ++u16_time_out_counter)
	{
		__RESET_WATCHDOG();
		if(u16_time_out_counter >= TIME_OUT_COUNTER)
		{
			return FALSE;
		}
	}
	
	return TRUE;
}

/**********************************************************************************************************************
**	Func Name:		BOOL DRV_COM2ReadByte(UINT08 *p_data)
**	Parameters:		UINT08 *p_data
**	Return Value:	read data result
**	Abstract:		read 1 byte data
**********************************************************************************************************************/
BOOL DRV_COM2ReadByte(UINT08 *p_data)
{
	if(!SCI2S1_RDRF)
	{
		return FALSE;
	}
	
	(*p_data) = SCI2D;
	
	return TRUE;
}
