/***************************************************Copyright (c)******************************************************
**										2008-2015,Shenzhen BYD Auto Co.,Ltd.
**		All rights reserved by SoftWare Team,Multimedia Electronics Factory,Division XV,Shenzhen BYD Auto Co.,Ltd.
**
**----------------------------------------------------File Info--------------------------------------------------------
**
**	Filename:			Debug.c
**	Abstract:			This file implements UART Debug
**	Creat By:			Liu Can
**	Creat Time:			2010/10/26
**	Version:			V1.00
**
**	Modify By:
**	Modify Time:
**	Version:
**	Description:
**
**********************************************************************************************************************/

/**********************************************************************************************************************
**	Include Files
**********************************************************************************************************************/
#ifdef __DEBUG__
#include "Debug.h"
#include "Drv_COM.h"

/**********************************************************************************************************************
**	const Define
**********************************************************************************************************************/
static const UINT08 char_table[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

/**********************************************************************************************************************
**
**********************************************************************************************************************/
typedef struct
{
	BYTE Hour;
	BYTE Minute;
	BYTE Second;
	WORD wMilliSecond;
}AppTimer;

/**********************************************************************************************************************
**
**********************************************************************************************************************/
static AppTimer xAppTimer;

/**********************************************************************************************************************
**	function declaration
**********************************************************************************************************************/
static void DebugTransmitArrayBuf(const UINT08* p_string, UINT08* p_data, UINT08 data_num);
static UINT08 *itoa(INT num, UINT08 *str, INT radix);

/**********************************************************************************************************************
**
**	DEFINE
**
**********************************************************************************************************************/

/**********************************************************************************************************************
**	Func Name:		void Printfpc(const UINT08* p_str, UINT08* p_data, UINT08 data_num)
**	Parameters:
**	Return Value:	void
**	Abstract:	
**********************************************************************************************************************/
void Printfpc(const UINT08* p_str, UINT08* p_data, UINT08 data_num)
{
	static BOOL debug_init_comp_flag = FALSE;
	
	if(!debug_init_comp_flag)
	{
		DRV_COM1Init();
		
		debug_init_comp_flag = TRUE;
	}
	
	DebugTransmitArrayBuf(p_str, p_data, data_num);
	
	return;
}

/**********************************************************************************************************************
**	Func Name:		void vAppTimer(BYTE TimerUnit)
**	Parameters:
**	Return Value:
**	Abstract:	
**********************************************************************************************************************/
void vAppTimer(BYTE TimerUnit)
{
	xAppTimer.wMilliSecond += TimerUnit;

	if(xAppTimer.wMilliSecond == 1000)
	{
		xAppTimer.wMilliSecond = 0;
		if(++xAppTimer.Second == 60)
		{
			xAppTimer.Second = 0;
			if(++xAppTimer.Minute == 60)
			{
				xAppTimer.Minute = 0;
				xAppTimer.Hour++;
			}
		}
	}
}

/**********************************************************************************************************************
**	Func Name:		void DebugTransmitArrayBuf(const UINT08* p_string, UINT08* p_data, UINT08 data_num, BOOL new_line)
**	Parameters:		const UINT08* p_string, const UINT08* p_data, UINT08 data_num, BOOL new_line
**	Return Value:	void
**	Abstract:		transmit debug array buffer(hex)
**********************************************************************************************************************/
static void DebugTransmitArrayBuf(const UINT08* p_string, UINT08* p_data, UINT08 data_num)
{
	const UINT08 MAX_STR_LENGTH = 50;
	volatile UINT08 i;
	UINT08 str_temp[10];
	const UINT08* p_str = NULL;

	itoa(xAppTimer.Hour, str_temp, 10);
	p_str = (const UINT08*)str_temp;
	while((*p_str) != '\0')
	{
		DRV_COM1WriteByte(*p_str++);
	}
	DRV_COM1WriteByte(':');
	itoa(xAppTimer.Minute, str_temp, 10);
	p_str = (const UINT08*)str_temp;
	while((*p_str) != '\0')
	{
		DRV_COM1WriteByte(*p_str++);
	}
	DRV_COM1WriteByte(':');
	itoa(xAppTimer.Second, str_temp, 10);
	p_str = (const UINT08*)str_temp;
	while((*p_str) != '\0')
	{
		DRV_COM1WriteByte(*p_str++);
	}
	DRV_COM1WriteByte('.');
	itoa(xAppTimer.wMilliSecond, str_temp, 10);
	p_str = (const UINT08*)str_temp;
	while((*p_str) != '\0')
	{
		DRV_COM1WriteByte(*p_str++);
	}

	DRV_COM1WriteByte(':');
	DRV_COM1WriteByte('	');
	
	if(p_string != NULL)
	{
		for(i = 0; (*p_string) && (i != MAX_STR_LENGTH); ++p_string, ++i)
		{
			DRV_COM1WriteByte(*p_string);
		}
	}
	
	if((p_data != NULL) && (data_num != 0))
	{
		for(; data_num != 0; --data_num, ++p_data)
		{
			itoa(*p_data, str_temp, 16);
			p_string = (const UINT08*)str_temp;
			if(p_string != NULL)
			{
				if(!(*(p_string + 1)))
				{
					DRV_COM1WriteByte('0');
				}
				else
				{
					DRV_COM1WriteByte(*p_string++);
				}
				DRV_COM1WriteByte(*p_string);
			}
			DRV_COM1WriteByte(' ');
		}
	}
	
	DRV_COM1WriteByte('\r');
	DRV_COM1WriteByte('\n');
	
	return;
}

/**********************************************************************************************************************
**	Func Name:		static UINT08 *itoa(INT value, UINT08 *string, INT radix)
**	Parameters:		INT value, UINT08 *string, INT radix
**	Return Value:	void
**	Abstract:		int to char
**********************************************************************************************************************/
static UINT08 *itoa(INT num, UINT08 *str, INT radix)
{	
	unsigned unum;		//中间变量
	
	INT i = 0,j,k;
	
	//确定unum的值
	if((radix == 10) && (num < 0))
	{
		unum = (unsigned)-num;
		str[i++] = '-';
	}
	else
	{
		unum = (unsigned)num;
	}
	
	do
	{
		str[i++] = char_table[unum%(unsigned)radix];
		
		unum /= radix;
	}while(unum);
	
	str[i] = '\0';
	
	//转换
	if(str[0] == '-')
	{
		k = 1;	//十进制负数
	}
	else
	{
		k = 0;
	}
	
	//将原来的'/2'改为'/2.0',保证当num在16~255之间，radix等于16时，也能得到正确结果
//	for(j = k; j < ((i - 1) / 2.0 + k); j++)
	for(j = k; j < (i / 2 + k); j++)		//只能用于10进制，当前配置无法进行浮点运算
	{
		num = str[j];
		str[j] = str[i - j - 1 + k];
		str[i - j - 1 + k] = (UINT08)num;
	}
	
	return str;
}

#endif
