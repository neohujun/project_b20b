/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		Key.c
**	Abstract:		key handle
**	Creat By:		Zeno Liu
**	Creat Time:		2015.04.09
**	Version:		v1.0
**
******************************************************************************/

/* \brief
	include
*/
#include "Config.h"
#include "Key.h"
#include "common.h"
#include "ad.h"
#include "Apu.h"
#include "system.h"
#include "io.h"
#include "Timer.h"
#include "memory.h"
#include "error.h"
#include "eeprom.h"
#include "Pwm.h"
#include "Bcm.h"
#include "AirCondition.h"

/* \brief
*/
#define	keyMAX_CHANNEL_NUM							6
#define	keyMAX_KEY_NUM_EACH_CHANNEL					10

/* \brief
*/
typedef enum
{
	KEY_SCAN_IDLE,
	KEY_SCAN_PRESS,
	KEY_SCAN_PRESSED,
	KEY_SCAN_SUSTAINED_PRESSED,
	KEY_SCAN_WAIT_FULL_RELEASE
}enumKeyScanState;

/* \brief
*/

BYTE* KeyCh4Tab = NULL;
BYTE* KeyCh5Tab = NULL;

static BOOL is360Status = FALSE;

BYTE KeyCh0Tab[] =
{
	0xFF, 0xF0,
 	0x0A, 0x00			//mute
};

BYTE KeyCh1Tab[] =
{
	0xFF, 0xF0,
	0x92, 0x7E,		//KEY_DISP ad: 88
	0x70, 0x5C,		//KEY_BACK ad: 66
	0x45, 0x31,		//key_media ad: 3B
 	0x0A, 0x00		//key_radio ad: 0
};

BYTE KeyCh2Tab[] =
{
	0xFF, 0xF0,		//NONE
	0xC4, 0xB0,		//phone	ad: 0xBA 186 2.41
	0xA3, 0x8F,		//down	ad: Ox99 153 1.98
	0x85, 0x71,		//up		ad: 0x7B 123 1.59
	0x67, 0x53,		//VolDn	ad: 0x5D 93  1.2
	0x47, 0x33,		//VolUp	ad: 0x3D 61 0.79
	0x29, 0x15,		//VR	ad: 0x1F 31 0.4
	0x0A, 0x00		//mode	ad: 0x00
};

BYTE KeyCh3Tab[] =
{
	0xFF, 0xF0,
 	0x10, 0x00			//Home
};



/* \brief
*/
BYTE* pKeyChTab[] =
{
	KeyCh0Tab,
	KeyCh1Tab,
	KeyCh2Tab,
	KeyCh3Tab,
	KeyCh4Tab,
	KeyCh5Tab
};

/* \brief
*/
static enumKeyScanState eKeyScanState;

static SignalFilter xKeyFilter[keyMAX_CHANNEL_NUM];
static enumKey eKey;
static WORD wKeyDelayTimer;
static BYTE KeyScanTimer;

/* \brief
*/
static void vKeyScan(void);
static void vKeyEncodeScan(void);
static BYTE KeyGetNumber(BYTE channel);

/* \brief
*/
void vKeyInit(void)
{
	eKeyScanState = KEY_SCAN_PRESS;
	eKey = KEY_EMPTY;
	wKeyDelayTimer = 0;
	KeyScanTimer = 0;
}

/* \brief
*/
void vKeyTask(void)
{
	if((SYSTEM_ACC_IDLE != vSystemAccStatus()) || (!ioBAT_DET_IN))
	{
		eKey = KEY_EMPTY;
		
		return;
	}
	
	vKeyEncodeScan();

	vKeyScan();
	
	if((!isSystemInitCompleted) && (KEY_POWER_RST != eKey))
	{
		eKey = KEY_EMPTY;
		return;
	}
	
	if(KEY_EMPTY != eKey)
	{

		switch(eKey)
		{
			case KEY_POWER_RST:
				{
					vResetMcuJumpBgnd();
				}
				break;

			case KEY_POWER:			//powerkey long
				{
					BYTE data[2] ={0};
					data[0] = UICC_POWER;
					data[1] = 0x00;
					vApuWrite(APUW_GID_CMD, APUW_CMD_COMMAND, data, 2);
				}
				break;
				
			case KEY_MUTE:			//powerkey short
				{
					BYTE data[2] ={0};
					data[0] = UICC_MUTE;
					data[1] = 0x00;
					vApuWrite(APUW_GID_CMD, APUW_CMD_COMMAND, data, 2);
				}
				break;
				
			case KEY_RADIO:
				{
					BYTE data[2] ={0};
					data[0] = UICC_RADIO;
					data[1] = 0x00;
					vApuWrite(APUW_GID_CMD, APUW_CMD_COMMAND, data, 2);
				}
				break;

			case KEY_BACK:
				{
					BYTE data[2] ={0};
					data[0] = UICC_BACK;
					data[1] = 0x00;
					vApuWrite(APUW_GID_CMD, APUW_CMD_COMMAND, data, 2);
				}
				break;
				
			case KEY_DISP:
				{
					BYTE data[2] ={0};
					data[0] = UICC_DISP;
					data[1] = 0x00;
					vApuWrite(APUW_GID_CMD, APUW_CMD_COMMAND, data, 2);
				}
				break;
				
			case KEY_MEDIA:
				{
					BYTE data[2] ={0};
					data[0] = UICC_MEDIA;
					data[1] = 0x00;
					vApuWrite(APUW_GID_CMD, APUW_CMD_COMMAND, data, 2);
				}
				break;
				
			case KEY_TEL:
				{
					BYTE data[2] ={0};
					data[0] = UICC_BT_DIAL;
					data[1] = 0x00;
					vApuWrite(APUW_GID_CMD, APUW_CMD_COMMAND, data, 2);
				}
				break;

			case KEY_VOLUP:
				{
					BYTE data[2] ={0};
					data[0] = UICC_VOLUME_UP;
					data[1] = 0x00;
					vApuWrite(APUW_GID_CMD, APUW_CMD_COMMAND, data, 2);
				}

				break;
				
			case KEY_VOLDN:
				{
					BYTE data[2] ={0};
					data[0] = UICC_VOLUME_DOWN;
					data[1] = 0x00;
					vApuWrite(APUW_GID_CMD, APUW_CMD_COMMAND, data, 2);
				}
				break;
				
			case KEY_UP:
				{
					BYTE data[2] ={0};
					data[0] = UICC_UP;
					data[1] = 0x00;
					vApuWrite(APUW_GID_CMD, APUW_CMD_COMMAND, data, 2);
				}

				break;

			case KEY_DOWN:
				{
					BYTE data[2] ={0};
					data[0] = UICC_DOWN;
					data[1] = 0x00;
					vApuWrite(APUW_GID_CMD, APUW_CMD_COMMAND, data, 2);
				}

				break;
				
			case KEY_VR:
				{
					BYTE data[2] ={0};
					data[0] = UICC_VR_ACTIVE;
					data[1] = 0x00;
					vApuWrite(APUW_GID_CMD, APUW_CMD_COMMAND, data, 2);
				}
				break;
				
			case KEY_SRC:
				{
					BYTE data[2] ={0};
					data[0] = UICC_SRC;
					data[1] = 0x00;
					vApuWrite(APUW_GID_CMD, APUW_CMD_COMMAND, data, 2);
				}
				break;
				
			case KEY_HOME:
				//for test===============
//				vReverseButton();
				//for test===============

				{
					BYTE data[2] ={0};
					data[0] = UICC_MENU;
					data[1] = 0x00;
					vApuWrite(APUW_GID_CMD, APUW_CMD_COMMAND, data, 2);
				}

				break;
				
			case KEY_UP_L:
				{
					BYTE data[2] ={0};
					data[0] = UICC_UP;
					data[1] = 0x00;
					vApuWrite(APUW_GID_CMD, APUW_CMD_COMMAND, data, 2);
				}

				break;

			case KEY_DOWN_L:
				{
					BYTE data[2] ={0};
					data[0] = UICC_DOWN;
					data[1] = 0x00;
					vApuWrite(APUW_GID_CMD, APUW_CMD_COMMAND, data, 2);
				}

				break;

			default:
				break;
		}
		
		eKey = KEY_EMPTY;
	}
}

/* \brief
*/
void vKeyTimer(void)
{
	if(0 != wKeyDelayTimer)
	{
		--wKeyDelayTimer;
	}
	if(0 != KeyScanTimer)
	{
		--KeyScanTimer;
	}
}


/* \brief
*/
static void vKeyScan(void)
{
	static BYTE PressedChannel = 0;
	static enumKey eKeyTemp = KEY_EMPTY;

	if(0 != KeyScanTimer)
	{
		return;
	}
	KeyScanTimer = 2;
	
	xKeyFilter[0].OldSignal = xKeyFilter[0].NewSignal;
	xKeyFilter[1].OldSignal = xKeyFilter[1].NewSignal;
	xKeyFilter[2].OldSignal = xKeyFilter[2].NewSignal;
	xKeyFilter[3].OldSignal = xKeyFilter[3].NewSignal;
	xKeyFilter[4].OldSignal = xKeyFilter[4].NewSignal;
	xKeyFilter[5].OldSignal = xKeyFilter[5].NewSignal;

//	xKeyFilter[3].NewSignal = 0x00;//KeyGetNumber(3);
	xKeyFilter[4].NewSignal = 0x00;//KeyGetNumber(4);
	xKeyFilter[5].NewSignal = 0x00;//KeyGetNumber(5);

	xKeyFilter[0].NewSignal = KeyGetNumber(0);
	xKeyFilter[1].NewSignal = KeyGetNumber(1);
	xKeyFilter[2].NewSignal = KeyGetNumber(2);
	xKeyFilter[3].NewSignal = KeyGetNumber(3);


	xKeyFilter[0].FilterCount = 3;
	xKeyFilter[1].FilterCount = 3;
	xKeyFilter[2].FilterCount = 3;
	xKeyFilter[3].FilterCount = 3;

	switch(eKeyScanState)
	{
		case KEY_SCAN_PRESS:
			if ((0 != xKeyFilter[0].NewSignal)
				|| (0 != xKeyFilter[1].NewSignal)
				|| (0 != xKeyFilter[2].NewSignal)
				|| (0 != xKeyFilter[3].NewSignal))
			{
	  			for (PressedChannel = 0; keyMAX_CHANNEL_NUM != PressedChannel; ++PressedChannel)
				{
					/* set signal value */
					xKeyFilter[PressedChannel].OkSignal = xKeyFilter[PressedChannel].NewSignal;

					if (SIGNAL_ON == GetFilterSignal(&xKeyFilter[PressedChannel]))
					{
						/* save key */
						eKeyTemp = (BYTE)((keyMAX_KEY_NUM_EACH_CHANNEL*PressedChannel) + xKeyFilter[PressedChannel].NewSignal);
						if(KEY_MUTE == eKeyTemp)
						{
							wKeyDelayTimer = 75;
						}
						else if(KEY_DISP== eKeyTemp)
						{
							wKeyDelayTimer = 750;
						}
						else
						{
							wKeyDelayTimer = 170;
						}
						eKeyScanState = KEY_SCAN_PRESSED;

						break;
					}
				}
			}
			break;
			
		case KEY_SCAN_PRESSED:
			switch (GetFilterSignal(&xKeyFilter[PressedChannel]))
			{
				case SIGNAL_ON:
					if (0 == wKeyDelayTimer)
					{
						if((KEY_UP == eKeyTemp) || (KEY_DOWN == eKeyTemp))
						{
							eKey = (BYTE)(eKeyTemp + keyLONG_PRESSED);
							wKeyDelayTimer = 20;
							eKeyScanState = KEY_SCAN_SUSTAINED_PRESSED;
						}
						else
						{
							eKey = (BYTE)(eKeyTemp + keyLONG_PRESSED);
							eKeyScanState = KEY_SCAN_WAIT_FULL_RELEASE;
						}
					}
					else
					{
						switch (eKeyTemp)
						{
							default:
								eKey = eKeyTemp;
								eKeyScanState = KEY_SCAN_WAIT_FULL_RELEASE;
								break;


							case KEY_MUTE:
							case KEY_DISP:
								break;
								
							case KEY_VOLUP:
							case KEY_VOLDN:
								eKey = eKeyTemp;
								wKeyDelayTimer = 20;
								eKeyScanState = KEY_SCAN_SUSTAINED_PRESSED;
								break;
						}
					}
					break;

				case SIGNAL_CHATTER:
				case SIGNAL_OFF:
				default:
					eKey = eKeyTemp;
					eKeyScanState = KEY_SCAN_WAIT_FULL_RELEASE;
					if(KEY_POWER == eKey)
					{
						KeyScanTimer = 30;
					}
					break;
			}
			break;
			
		case KEY_SCAN_SUSTAINED_PRESSED:
			switch (GetFilterSignal(&xKeyFilter[PressedChannel]))
			{
				case SIGNAL_ON:
					if(0 != wKeyDelayTimer)
					{
						break;
					}
					
					switch (eKeyTemp)
						{
							default:
								eKeyScanState = KEY_SCAN_WAIT_FULL_RELEASE;
								break;

							case KEY_MUTE:
							case KEY_DISP:
							case KEY_UP:
							case KEY_DOWN:
								eKey = (BYTE)(eKeyTemp + keyLONG_PRESSED);
								wKeyDelayTimer = 20;
								break;

							//for test===============
//							case KEY_HOME:
//							case KEY_BACK:
							//for test===============
							
							case KEY_VOLUP:
							case KEY_VOLDN:
								eKey = eKeyTemp;
								wKeyDelayTimer = 20;
								break;
						}
					break;
					
				case SIGNAL_CHATTER:
				case SIGNAL_OFF:
				default:
					eKeyScanState = KEY_SCAN_WAIT_FULL_RELEASE;
					break;
			}

			break;
			
		case KEY_SCAN_WAIT_FULL_RELEASE:
			if ((0 == xKeyFilter[0].NewSignal)
				&& (0 == xKeyFilter[1].NewSignal)
				&& (0 == xKeyFilter[2].NewSignal)
				&& (0 == xKeyFilter[3].NewSignal)
				&& (0 == xKeyFilter[4].NewSignal)
				&& (0 == xKeyFilter[5].NewSignal))
			{
				eKeyScanState = KEY_SCAN_PRESS;
			}
			break;
			
		case KEY_SCAN_IDLE:
			eKeyScanState = KEY_SCAN_PRESS;
			break;
	}
}
/* \brief
*/
static void vKeyEncodeScan(void)
{
	static SignalFilter xTuneEncodeFilter;
	static BYTE VolumeSignal=0,VolumeSignalBak=0;
	static BYTE TuneSignal=0,TuneSignalBak=0;

	//tune key
	xTuneEncodeFilter.OldSignal = xTuneEncodeFilter.NewSignal;
	xTuneEncodeFilter.NewSignal = (ioKEY_TUNE_A<<2) | (ioKEY_TUNE_B<<1) | (ioKEY_TUNE_C);

	xTuneEncodeFilter.FilterCount = 2;
	switch(GetFilterSignal(&xTuneEncodeFilter))
	{
		case SIGNAL_ON:
		case SIGNAL_OFF:
			TuneSignal = xTuneEncodeFilter.NewSignal;
			break;

		default:
			break;
	}

	TuneSignal = xTuneEncodeFilter.NewSignal;
	if(0x07 != TuneSignal)
	{
		if(TuneSignalBak != TuneSignal)
		{
			if(0x06 == TuneSignalBak)
			{
				if(0x05 == TuneSignal)
				{
					eKey = KEY_VOLUP;
				}
				else if(0x03 == TuneSignal)
				{
					eKey = KEY_VOLDN;
				}
			}
			else if(0x05 == TuneSignalBak)
			{
				if(0x03 == TuneSignal)
				{
					eKey = KEY_VOLUP;
				}
				else if(0x06 == TuneSignal)
				{
					eKey = KEY_VOLDN;
				}
			}
			else if(0x03 == TuneSignalBak)
			{
				if(0x06 == TuneSignal)
				{
					eKey = KEY_VOLUP;
				}
				else if(0x05 == TuneSignal)
				{
					eKey = KEY_VOLDN;
				}
			}
			TuneSignalBak = TuneSignal;
		}
	}

}


/* \brief
*/
static BYTE KeyGetNumber(BYTE channel)
{
	BYTE i = 0;
	BYTE KeyValue = ADGetValue(channel);


	for(i=0; (keyMAX_KEY_NUM_EACH_CHANNEL*2)!=i; i+=2)
	{
		if((pKeyChTab[channel][i] >= KeyValue) && (pKeyChTab[channel][i+1] <= KeyValue))
		{
			break;
		}
	}
	return i/2;

}

