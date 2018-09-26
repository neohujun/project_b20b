
#ifndef _UICC_DEF_H
#define _UICC_DEF_H
//=================================================================================
//APU_TX_CMD and MCU_TX_CMD Command ID define.

#include "Config.h"

typedef enum
{
	UICC_NONE				= 0x00,			//Unused.
		
	//=======Key command from panel key or Remote key=======
	UICC_MUTE				= 0x01,			//System audio mute or un-mute function.
											// 1. APU==>MCU when user press the Mute button on GUI. Param: 0x00(Mute on); 0x01(Mute off); 0x02(AMP Mute on); 0x03(AMP Mute off);
											// 2. MCU inside used when user press the Mute key on panel or SWC or Remote.
	
	UICC_PLAY_PAUSE		= 0x02,			//Play_Pause key. Param: Key_source(1: Rear remote, other: Front remote or panel)
											// 1. MCU inside used. (Seek start and seek stop when front source is SOURCE_TUNER)
											// 2. MCU==>APU. For media play(USB/SD/iPod/DVD) play or pause function.
											
	UICC_SKIPF				= 0x03,			//Skip Forward key. Param: Key_source 
											// 1. MCU inside used. Seek next station when front source is Tuner/HD Radio or TV.
											// 2. MCU==>APU. For media play(USB/SD/iPod/DVD) Next track function.
											
	 UICC_SKIPB				= 0x04,			//Skip Back key. Param: Key_source 
											// 1. MCU inside used. Seek previous station when front source is Tuner/HD Radio or TV.
											// 2. MCU==>APU. For media play(USB/SD/iPod/DVD) Previous track function.
											
	UICC_EJECT				= 0x05,			//Eject key on panel or Front remote or Eject button on GUI.
											// 1. MCU==>APU. Send to APU when front source is SOURCE_DVD for eject disc.
											// 2. APU==>MCU. APU send to MCU when user press the Eject button on GUI.
	
	UICC_SOURCE			= 0x06,			//MCU inside used only. For Map the SRC key on panel or remote.								
	UICC_ANGLE_DEC		= 0x07,			//MCU inside used only. For adjust TFT's Angle Decrease in 1Din machine.
	UICC_ANGLE_INC			= 0x08,			//MCU inside used only. For adjust TFT's Angle Increase in 1Din machine.
	UICC_OPEN				= 0x09,			//MCU inside used only. For Open or Close TFT screen when machine is 1Din or 2Din with slide screen.
	
	UICC_PICTURE			= 0x0A,			//MCU==>APU. APU enter into the setup menu and show the Video setting page.  

	UICC_DISP				= 0x0B,			//DISP key on panel or remote. Param: key_source
											// 1. MCU inside used to change LCD display mode when 1Din and the TFT is inside.
											// 2. MCU==>APU. Show or hide the info bar when current source is SOURCE_NAVI.
											// 3. MCU==>APU. Display switch between Text(ID3) and List of Disc media player when current source is SOURCE_DVD.
											// 4. MCU==>APU. Display switch between Text(ID3) and List of USB/SD media player when current source is SOURCE_USB or SOURCE_SD.
	
	
	UICC_AUDIO_SETUP		= 0x0C,			//MCU==>APU. For open or close EQ Setup Menu. Param: key_source. 
	
	UICC_AMFM				= 0x0D,			//Reversed for feature use. Change to UICC_RADIO.
											
	UICC_ASPS				= 0x0E,			//PS(Preset Scan) key.  Param: key_source
											// 1. MCU inside used to preset scan function when current front source is TUNER/HD_RADIO/TV
											// 2. MCU==>APU. For preset scan when current source is SiXM(SiXM is controlled by APU).
	
	UICC_INFO				= 0x0F,			//MCU==>APU. For video information display. Display the DVD Video information when current is DVD.

	//============Above keys have long press function=========

	UICC_UP				= 0x10,			//MCU==>APU. Up key function, Param: key_source.	
	UICC_DOWN				= 0x11,			//MCU==>APU. Down key function, Param: key_source.
	UICC_LEFT				= 0x12,			//MCU==>APU. Left key function, Param: key_source.										
	UICC_RIGHT				= 0x13,			//MCU==>APU. Right key function, Param: key_source.
	UICC_ENTER				= 0x14,			//MCU==>APU. Enter key function. Param: key_source.
											    
	UICC_0					= 0x15,			//MCU==>APU. (0 key), Param: key_source.
	UICC_1					= 0x16,			//MCU==>APU. (1 key), Param: key_source.
	UICC_2					= 0x17,			//MCU==>APU. (2 key), Param: key_source.
	UICC_3					= 0x18,			//MCU==>APU. (3 key), Param: key_source.
	UICC_4					= 0x19,			//MCU==>APU. (4 key), Param: key_source.
	UICC_5					= 0x1A,			//MCU==>APU. (5 key), Param: key_source.	
	UICC_6					= 0x1B,			//MCU==>APU. (6 key), Param: key_source.
	UICC_7					= 0x1C,			//MCU==>APU. (7 key), Param: key_source.
	UICC_8					= 0x1D,			//MCU==>APU. (8 key), Param: key_source.
	UICC_9					= 0x1E,			//MCU==>APU. (9 key), Param: key_source.
	UICC_10P				= 0x1F,			//MCU==>APU. (10+ key), Param: key_source.

	UICC_CLEAR				= 0x20,			//Unused. 
	UICC_SETUP				= 0x21,			//MCU==>APU. For enter into Setup menu. Param: key_source.

	UICC_STOP				= 0x22,			//Stop key. Param: key_source
											// 1. MCU inside used. For stop seek and change to current preset program 
											//    when current source is Tuner/HD_Radio or TV.
    										// 2. MCU==>APU. For stop media play(USB/SD/Disc/iPod.etc.).
    										
	UICC_SEARCH			= 0x23,			//MCU==>APU. Search key, for pop up the search menu when DVD mode. Param: key_source.
	UICC_ANGLE				= 0x24,			//MCU==>APU. Angle key, for change the DVD video disc's angle if this disc with angle function.	Param: key_source.
	UICC_TITLE				= 0x25,			//MCU==>APU. Title key, for enter into DVD video disc's title menu.	Param: key_source.
	UICC_AUDIO				= 0x26,			//MCU==>APU. Audio key,	for change the disc's audio track, include DVD Video and VCD disc. Param: key_source.  
	UICC_SUBTITLE			= 0x27,			//MCU==>APU. Subtitle key, for change the DVD video disc's subtitle. Param: key_source.
	UICC_MENU				= 0x28,			//MCU==>APU. Menu key, for open or close Main Menu.	Param: key_source.
	UICC_ZOOM				= 0x29,			//MCU==>APU. Zoom key, for zoom the current DVD video. Param: key_source.
	UICC_FASTR				= 0x2A,			//MCU==>APU. Fast Rewind key, for media player like disc/USB/SD/iPod.	Param: key_source.
	UICC_FASTF				= 0x2B,			//MCU==>APU. Fast Forward key, for media player like disc/USB/SD/iPod. Param: key_source.
	UICC_SCAN				= 0x2C,			//MCU==>APU. Scan play mode. for media player(USB/SD/Disc/iPod, etc).
	UICC_SHUFFLE			= 0x2D,			//MCU==>APU. Shuffle key. For change media player's shuffle on or off. Param: key_source.
	UICC_REPEAT			= 0x2E,			//MCU==>APU. Repeat key. For change repeat mode of media player. Param: key_source.
	UICC_REP_AB			= 0x2F,			//MCU==>APU. Repeat AB key. For DVD's repeat A-B function. Param: key_source.
	UICC_SLOWF			= 0x30,			//MCU==>APU. Slow forward key, for DVD use.	Param: key_source.
	UICC_SLOWB			= 0x31,			//MCU==>APU. Slow backward key, for DVD use. Param: key_source.
	UICC_PBC				= 0x32,			//MCU==>APU. Reversed for VCD disc's PBC function.
	
	UICC_SRC				= 0x33,			//MCU==>APU. For source display when user pressed the SRC key on panel or remote.
											//Param:  0x00: From Front Zone, 0x01: From Rear Zone.			 


	UICC_ESC				= 0x34,			//MCU==>APU. ESC key.			Param: key_source
											// 1. Exit the standby mode when current is standby.
											// 2. Close MainMenu/EQ Menu/Setup Menu to current source menu when current menu is MainMenu/EQ/Setup menu.
											// 3. Return previous menu or catalog for other applications.

	UICC_SEL				= UICC_AUDIO_SETUP, //MCU==>APU. For open or close EQ Setup Menu. Param: key_source 

	UICC_BACK				= 0x35,			//MCU==>APU. Back key, for Android Back function.
	UICC_OPTION			= 0x36,			//MCU==>APU. Menu key, for Android to open option menu in any Apps.
	UICC_HOMEPAGE			= 0x37,			//MCU==>APU. Home key for Android Home function.

	//=======Panel Encoder=======
	UICC_SMART_CW			= 0x39,			//MCU==>APU. Encoder ClockWise key, Param: key_source.
											//For moving the focus cursor clockwise when the top windows display is MainMenu/BT Menu/EQ Menu.
											//or send zoom in the map display.
											
	UICC_SMART_CCW		= 0x3A,			//MCU==>APU. Encoder Counter ClockWise key, Param: key_source.
											//For moving the focus cursor counter clockwise when the top windows display is MainMenu/BT Menu/EQ Menu.
											//or send zoom out the map display.
	
	UICC_SMART_CLICK		= 0x3B,			//MCU==>APU. Encoder Click key, Param: key_source.
											//It's Enter function when the top windows display is MainMenu/BT Menu/EQ Menu.
	UICC_SMART_HOLD		= 0x3C,			//MCU==>APU. Encoder Hold key, Param: key_source. For open EQ menu.

	//=======System Operation=======
	UICC_POWER			= 0x3D,			//MCU==>APU. MCU send system power state.
											//			 Param: 0 = Wait Sleep On (System will enter into wait sleep mode when ACC off or B+ abnormal);
											//					1 = Wait Sleep Off(System return to power on mode when ACC on or B+ recover from Wait sleep mode);
											//					2 = Sleep on      (Inform APP to enter into sleep mode)
											//					3 = Wake up.	  (System wake up from sleep mode)

	UICC_OPEN_MENU		= 0x3E,			//MCU==>APU. MCU inform APU to open menu. Param: Menu_Type; 
											//			 Menu_Type: 0x01=MainMenu;0x02=Menu Off; 
											//			 0x03=Anti-Theft password input Menu, APU judge PW by itself, if ok or error send UICC_ANTITHEFT_PW to MCU; 
											//			 0x04=Anti-Theft Locked Menu. 0x05=Warning Menu; 0x06=Return video state;
	
	UICC_TFT_POWER		= 0x3F,			//MCU==>APU. MCU inform APU enter into Monitor on or Off mode. System will power on/off TFT. Param: 0: TFT Power off, 1: TFT Power on.
	UICC_MEDIA				= 0x40,
	UICC_FRONT_SRC		= 0x47,			//MCU==>APU. MCU inform APU to change Front Source, Param: SOURCE_ID										
	UICC_REAR_SRC			= 0x48,			//MCU==>APU. MCU inform APU to change Rear Source,  Param: SOURCE_ID
	
	UICC_DISABLE_TS		= 0x49,			//MCU==>APU. When Camera,Touch Screen only top left corner enable.
	UICC_ENABLE_TS			= 0x4A,			//MCU==>APU. Touch Screen renew usefully.
	UICC_VR_ACTIVE			= 0x4B,			//MCU==>APU. MCU inform APU to active or deactive the Voice Recognition function when user press VR button on panel or SWC.
	UICC_INFOBAR			= 0x4C,			//MCU==>APU. Long press of Navi key, can use for Show or Hide the info play bar.
	UICC_ONE_KEY_NAVI		= 0x4D,			//MCU==>APU. OneKeyNavi, MCU send this key to APU for active the One Key Navi function.
	UICC_CLIMATE			= 0x4E,
	UICC_LINK				= 0x4F,			//MCU==>APU. Link key

	//=======For BlueTooth=======
	UICC_BT_VOICE			= 0x50,			//MCU==>APU. Voice dial key.	Param: key_source (2=Panel)
											//Press this key can run the voice dial function when phone have voice dial function, else invalid.
											
	UICC_BT_DIAL			= 0x51,			//MCU==>APU. Dial key. Param: key_source
											// 1. Press this key can open BT dial menu when BT menu isn't opened.
											// 2. Close BT menu when current menu is BT menu.
											// 3. Accept call when current have incoming call.
											
	UICC_BT_DONE			= 0x52,			//MCU==>APU. Call end key. Hang up the call and then close BT menu. Param: key_source
																				
	//=======For Navigation=======																				     
	UICC_RPT				= 0x53,			//MCU==>APU. For Navigation audio repeat play.								
	UICC_DEST				= 0x54,			//MCU==>APU. Destination key. MCU send this key to APU, and APU tell Map enter into the "Go to" menu or destination setting menu.
	UICC_FAV				= 0x55,			//MCU==>APU. Reversed for future used. (Favor source)
	UICC_MAP				= 0x56,			//MCU==>APU. Map key. MCU send this key to APU, and APU tell Navigation go to Map.
	UICC_HOME				= 0x57,			//MCU==>APU. Home key. MCU send this key to APU, and APU tell Navigation route to go home.
	
	//=======MCU inside used=======
	UICC_VOLUME_DOWN		= 0x60,			//MCU inside used for adjust the volume down.
	UICC_VOLUME_UP		= 0x61,			//MCU inside used for adjust the volume up. 
	UICC_POWER_ON			= 0x62,			//MCU inside used only(Power on key).
	UICC_POWER_OFF		= 0x63,			//MCU inside used only(Power off key).
	UICC_VOLUME_CW		= 0x64,			//MCU inside used only. Volume encoder for volume up,	Param: None
	UICC_VOLUME_CCW		= 0x65,			//MCU inside used only. Volume encoder for volume Down,	Param: None

	//=======Source key on panel or remote=======
	UICC_SD_CARD			= 0x66,			//MCU==>APU. SD key, request switch to SOURCE_SD_AUDIO(Default) when current source isn't SD_AUDIO.
	UICC_IPOD				= 0x67,			//MCU==>APU. iPod key, request switch to SOURCE_IPOD_AUDIO(Default) when current source isn't IPOD_AUDIO.
	UICC_DVD				= 0x68,			//MCU==>APU. DVD key, request switch to SOURCE_DVD_AUDIO(Default) when current source isn't DVD_AUDIO. If Current Disc type is only Video, need request SOURCE_DVD_VIDEO.
	UICC_NAVI				= 0x69,			//MCU==>APU. NAVI key, switch to Source_Navi when current source isn't Navigation, or exit Navigation mode
											//		     switch to AV source when current source is Navigation.
	UICC_RADIO				= 0x6A,			//MCU==>APU. FM/AM or Radio key, request switch to SOURCE_TUNER when current source isn't Tuner, or handled by MCU for switch Band when current source is Tuner.
	UICC_USB				= 0x6B,			//MCU==>APU. USB key, request switch to SOURCE_USB_AUDIO(Default) when current source isn't USB_AUDIO.
	UICC_PWR_STATE		= 0x6C,			//MCU-->APU Send vehicle power state (High 4 Bits : 0x01 Cold Start; 0x00 Normal Start ; Low 4 Bits  Acc off 0x00 acc on 0x01 crk on 0x02)
	UICC_PWR_VOLT			= 0x6D,			//MCU-->APU   send vehicle voltage state :normal voltage 0x00, low voltage 0x01  over voltage 0x02   


	//=======SWC state=======
	UICC_SWC_STATE		= 0x70,			//MCU==>APU. SD key, request switch to SOURCE_SD_AUDIO(Default) when current source isn't SD_AUDIO.
	UICC_SWC_CMD			= 0x71,			//MCU==>APU. iPod key, request switch to SOURCE_IPOD_AUDIO(Default) when current source isn't IPOD_AUDIO.
	UICC_SWC_KEY			= 0x72,			//MCU==>APU. DVD key, request switch to SOURCE_DVD_AUDIO(Default) when current source isn't DVD_AUDIO. If Current Disc type is only Video, need request SOURCE_DVD_VIDEO.

	//=========Above key cannot greater than 0x7f, because the highest bit for identifying the Long press==============

	UICC_MUTE_LONG 			= (UICC_MUTE|0x80),	 		//0x81	MCU inside used. Reversed.
	UICC_PAUSE_LONG			= (UICC_PLAY_PAUSE|0x80), 	//0x82	MCU inside used. Reversed.
	UICC_NEXT_LONG			= (UICC_SKIPF|0X80),		//0x83	1. MCU used for Tuner/HD Radio step up, 2. Fast Forward for USB/SD/DVD/iPod source.
	UICC_PREV_LONG			= (UICC_SKIPB|0x80),		//0x84	1. MCU used for Tuner/HD Radio step down, 2. Fast Back for USB/SD/DVD/iPod source.
	UICC_EJECT_LONG			= (UICC_EJECT|0x80),		//0x85	MCU inside used. Reversed for reset MPEG.
	UICC_SOURCE_LONG			= (UICC_SOURCE|0x80),		//0x86	Reversed.														
	UICC_ANGLE_DEC_LONG		= (UICC_ANGLE_DEC|0x80),	//0x87	MCU inside used(1Din). For start No Level TFT Angle Adjust Decrease.
	UICC_ANGLE_INC_LONG		= (UICC_ANGLE_INC|0x80),	//0x88	MCU inside used(1Din). For start No Level TFT Angle Adjust Increase.
	UICC_ANGLE_LONG_RELEASE	= 0x90,						//0x90  MCU inside used. Angle Dec/Inc long press key or Open long press release, for stop No Level TFT Angle Adjust.

	UICC_OPEN_LONG			= (UICC_OPEN|0x80),			//0x89	MCU inside used(2Din slide TFT mode). For start No Level TFT Angle Adjust .
	UICC_PICTURE_LONG			= (UICC_PICTURE|0x80),		//0x8A	Reversed.
	UICC_DISP_LONG				= (UICC_DISP|0x80),			//0x8B	MCU inside used. MCU send UICC_SETUP key to APU when long press DISP key.
															
	UICC_AUDIO_LONG 			= (UICC_AUDIO_SETUP|0x80), 	//0x8C	Reversed.
	
	UICC_AMFM_LONG			= (UICC_AMFM|0x80),			//0x8D	Reversed.
	UICC_ASPS_LONG			= (UICC_ASPS|0x80),			//0x8E	MCU inside used. MCU start Auto Store when user long press 
														//		the AS/PS key and current source is Tuner. Param: key_source.

	UICC_LEFT_LONG				= (UICC_LEFT|0x80),			//0x92	Reversed.
	UICC_RIGHT_LONG 			= (UICC_RIGHT|0x80),		//0x93	Reversed.
	
	//=============The command that APU send to MCU==============
	//For CDC function.
	UICC_CDC_CHG_MUSIC		= 0x9A,		//APU==>MCU. Inform MCU select CDC track, Param: Track_Number.
	UICC_CDC_CHG_DISC			= 0x9B,		//APU==>MCU. Inform MCU to change Disc. Param: Disc_Number.
	UICC_CDC_NORMAL_CMD		= 0x9C,		//APU==>MCU. CDC normal control command. See <CDC_NORMAL_CMD>

	UICC_IPOD_VIDEO_TS		= 0x9D,		//APU==>MCU. Inform MCU that iPod Video on/off status. Param: 0=Off, 1=On.

	//For Tuner/RDS/HD Radio/DAB function.
	UICC_FREQ_LIST_TS			= 0xA0,	 	//APU==>MCU. Load or Save Program when click or Long press the Tuner frequency list item.   
										//			 Param: high 4bit:	0: Load preset program 3: Save preset program;  low  4bit:	program number. 
	UICC_ASPS_LONG_TS			= 0xA1,		//APU==>MCU. Long press the AS/PS button on GUI for inform MCU to enter into AS state.
	UICC_BAND_TS				= 0xA2,		//APU==>MCU. Tuner Band switch(between FM1,FM2,FM3,AM1,AM2) when press Band button on GUI.  
										//			 Param: 1=FM1, 2=FM2, 3=FM3, 4=AM1, 5=AM2, 0=Switch band cycle (when BAND is one button)
	UICC_ASPS_TS				= 0xA3,		//APU==>MCU. Preset Scan start or stop when press PS button. Param: None
	UICC_PREV_TS				= 0xA4,		//APU==>MCU. Seek Previous program.	Param: None
	UICC_NEXT_TS				= 0xA5,		//APU==>MCU. Seek Next Program.	Param: None
	UICC_STEP_UP_TS			= 0xA6,		//APU==>MCU. Step to previous frequency. Param: None
	UICC_STEP_DOWN_TS		= 0xA7,		//APU==>MCU. Step to next frequency. Param: None
	UICC_SCAN_TS				= 0xA8,		//APU==>MCU. Start or Stop scan in all frequencies, play 10 seconds when scan a valid station. Param: none
	UICC_AF_TS					= 0xA9,		//APU==>MCU. Open or close Auto Frequency change function. Param: 0=Off, 1=On.
	UICC_TA_TS					= 0xAA,		//APU==>MCU. Open or close Set Traffic Annunciation function. Param: 0=Off, 1=On.
	UICC_REG_TS				= 0xAB,		//APU==>MCU. Open or close RDS REG function. Param: 0=Off, 1=On. 
	UICC_EON_TS				= 0xAC,		//APU==>MCU. Open or close RDS EON function. Param: 0=Off, 1=On.
	UICC_PTY_ENABLE_TS		= 0xAD,		//APU==>MCU. Reversed for PTY function on or off.
	UICC_SEL_PTY_TS			= 0xAE,		//APU==>MCU. Select PTY type. Param: PTY_Type.
	UICC_LOC_TS				= 0xAF,		//APU==>MCU. LOC/DX switch. Param: 1: Loc, 0: DX
	UICC_HD_TAG_TS			= 0xB0,		//APU==>MCU. Reversed for HD Radio Tag button function.
	UICC_PTY_SEEK_TS			= 0xB1,     //APU==>MCU. Start PTY seek. Param: None.

	//Reversed for TV function.
	UICC_TV_P1_TS				= 0xB3,		//APU==>MCU. (TV Seek target channel )	 Param: Preset channel
	UICC_TV_SEEK_UP_TS		= 0xB4,		//APU==>MCU. (TV Seek up )	Param: None
	UICC_TV_SEEK_DN_TS		= 0xB5,		//APU==>MCU. (TV Seek Down) Param: None
	UICC_TV_CH_UP_TS			= 0xB6,		//APU==>MCU. (TV channel UP) Param: None
	UICC_TV_CH_DN_TS			= 0xB7,		//APU==>MCU. (TV channel down) Param: None
	UICC_TV_AS_TS				= 0xB8,		//APU==>MCU. (TV Auto Scan) Param: None
	UICC_TV_SCAN_TS			= 0xB9,		//APU==>MCU. (TV Preset Scan) Param: None
	UICC_TV_JUMP_TS			= 0xBA,		//APU==>MCU. (TV Jump channel between old and current)	Param: None
	UICC_TV_ENTER_TS			= 0xBB,		//APU==>MCU. (No operation  but it can be the enter key of DTV) Param: None
	UICC_TV_EXIT_TS			= 0xBC,		//No used in MCU
	UICC_TV_SAVE_TS			= 0xBD,		//APU==>MCU. (Save Current Channel to Preset Channel) Param: Preset Channel 

	//System Operation.
	UICC_FRONT_ZONE_TS		= 0xC0,		//APU==>MCU. APU request Front Source change. Param: SOURCE_ID.
	UICC_REAR_ZONE_TS			= 0xC1,		//APU==>MCU. APU request Rear Source change.  Param: SOURCE_ID.
	UICC_BEEP_ONLY_TS			= 0xC2,		//APU==>MCU. Once Beep sound for button click.	Param: none.
	UICC_BEEP_FAIL_TS			= 0xC3,		//APU==>MCU. Twice Beep sound for fail button click. Param: none.
	UICC_MONITOR_OP			= 0xC4,   	//APU==>MCU. APU inform MCU enter into Monitor OPerate type.	Param: 0=Monitor off, 1=Monitor on.
											//MCU==>APU. Monitor key.	Param: 0=Monitor toggle, 1=Monitor on, 2=Monitor off
	UICC_CELEBRATION			= 0xC5,		//APU==>MCU. APU inform MCU that touch screen celebration state. Param: 0=Start, 1=End. 

	UICC_DIGIT_KEY_TS			= 0xC6,		//No Used, Reversed.

	UICC_MPEG_RESET			= 0xC7,		//APU==>MCU. APU request MCU to reset MPEG when MPEG occur abnormal or MPEG update completed.
	UICC_MPEG_MAIN_STATE		= 0xC8,		//APU==>MCU. APU transmit the main state of MPEG to MCU for eject disc operation. Param=MPEG_MAIN_SATE;                                                   

	UICC_BT_PHONE_AUDIO		= 0xC9,		//APU==>MCU. BT phone audio channel on or off. Inform MCU to switch audio channel to BT phone channel or not.
										//Param: 0=BT audio channel off, 1=BT phone audio channel on;

	UICC_APP_PROMPT_TONE		= 0xCA,		//APU==>MCU. Apps prompt tone(Like: VR/WeiChat/etc) audio channel on or off. Inform MCU to switch the audio channel to Apps prompt tone channel or not.
										//Param: 0 = Apps prompt tone end, 1 = Apps prompt tone begin; 3=call in

	UICC_3G_PHONE_AUDIO		= 0xCB,   	//APU==>MCU. Inform MCU switch audio channel to 3G audio channel. Param: 0=off, 1=3G phone on.

	UICC_BT_RESET				= 0xCC,   	//APU==>MCU. Inform MCU to reset BT module when BT reset is controlled by MCU. Param: None
	UICC_BT_UPDATE			= 0xCD,   	//APU==>MCU. Inform MCU to generate BT update timing sequence when BT need update(using Parrot BT module). Param: None
	
	UICC_NAVI_AUDIO			= 0xCE,		//APU==>MCU. Navi audio channel on or off. Inform MCU to switch the audio channel to Navi channel or not.
										//Param: 0 = Navi audio play end, 1 = Navi audio play start; 

	UICC_RESTORE_DEFAULT		= 0xD0,

	UICC_SYS_STATE				= 0xD1,	//APU-->MCU send SYStem state to MCU: AVN off 0x00, Normal 0x01, Timemode  0x02
	UICC_USB_VBUS				= 0xD2, // APU-->MCU send SYStem state to MCU: 0x01 Vbus voltage high, 0x02 Vbus voltage low,0x03 Vbus voltage low 500ms to high

	UICC_APU_ACTIVE			= 0xD3,		//APU==>MCU. Param: 0 = Off, 1 = On

}UICC_DEF_ENUM;


//=====Data struct define=====
typedef enum
{
	KS_F_REMOTE	= 0, //Key from Front Remote.
	KS_R_REMOTE	= 1, //Key from Rear Remote.
	KS_PANEL		= 2, //Key from panel.
	KS_TS			= 3, //Key from touch screen.
	KS_SWC			= 4, //Key from SWC.
}Key_Source;



typedef enum
{
	CDC_SCAN			= 0x76,
	CDC_SHUFFLE		= 0x77,
	CDC_DISC_DEC		= 0x7E,
	CDC_DISC_INC		= 0x7F,
	CDC_PLAY_PAUSE	= 0x02,
	CDC_SKIPF			= 0x03,
	CDC_SKIPB			= 0x04,
	CDC_FASTR			= 0x2C,
	CDC_FASTF			= 0x2D,
}CDC_NORMAL_CMD;

typedef enum
{
	MST_POWER_OFF		= 0x00, //Power Off
	MST_POWER_DOWN		= 0x01,	//Power down
	MST_NO_DISC			= 0x02,	//No disc
	MST_UNKNOWN_DISC		= 0x03,	//Unknown disc
	MST_TRAY_OPENDING		= 0x04,	//Tray opening
	MST_TRAY_CLOSING		= 0x05,	//Tray closing
	MST_TRAY_OPEN			= 0x06,	//Tray open
	MST_LOADING			= 0x07,	//Loading
	MST_DISC_READY		= 0x08,	//Disc ready
}MPEG_MAIN_STATE;



#endif
