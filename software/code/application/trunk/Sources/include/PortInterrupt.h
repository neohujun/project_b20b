/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		PortInterrupt.h
**	Abstract:		head of PortInterrupt
**	Creat By:		Zeno Liu
**	Creat Time:		2015.06.08
**	Version:		v1.0
**
******************************************************************************/

#ifndef	__PORT_INTERRUPT_H__
#define	__PORT_INTERRUPT_H__

/* \brief
*/
void vPortInterruptInit(void);
void vPortInterruptUartInit(void);
void vPortInterruptUartStart(void);
void vPortInterruptUartStop(void);
void INTPort(void);

#endif

