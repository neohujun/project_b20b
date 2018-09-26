/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		ApuWCmd.h
**	Abstract:		APUW_GID_CMD
**	Creat By:		Zeno Liu
**	Creat Time:		2015.03.10
**	Version:		v1.0
**
******************************************************************************/

#ifndef	__APU_W_CMD_H__
#define	__APU_W_CMD_H__

/* \brief
	APUW_GID_CMD sub id
*/
typedef enum
{
	APUW_CMD_COMMAND				=0x01,
	APUW_CMD_QUERY_STATUS
}enumApuwCmd;


/* \brief
	APUW_GID_CAN sub id
*/
typedef enum
{
	APUW_CAN_AC					=0x01,
	APUW_CAN_LIGHT,
	APUW_CAN_CENCTL,
	APUW_CAN_RADAR,
	APUW_CAN_AVM					=0x07,
	APUW_CAN_SUNROOF			=0x09,
	APUW_CAN_TRUNK				=0x0A,
	APUW_CAN_CONFIGURE			=0x0B,
	APUW_CAN_SPEED				=0x0C,
	APUW_CAN_NIGHT				=0x0D
}enumApuwCmd;

#endif


