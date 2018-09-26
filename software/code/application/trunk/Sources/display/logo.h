#ifndef __LOGO_H__
#define __LOGO_H__

#undef EXTERN

#ifdef __LOGO_C__
#define EXTERN
#else
#define EXTERN extern
#endif


#include "type.h"

/* \brief
*/
typedef enum
{
	LOGO_IMG_NONE,
	LOGO_IMG_REV,
	LOGO_IMG_LVDS
}enumLogoImageStatus;


#define SOSD_WIN0	0x00
#define SOSD_WIN1	0x01
#define SOSD_WIN2	0x02
#define SOSD_WIN3	0x03
#define SOSD_WIN4	0x04
#define SOSD_WIN5	0x05
#define SOSD_WIN6	0x06
#define SOSD_WIN7	0x07
#define SOSD_WIN8	0x08

#define SFLASH_UNKNOWN				0
#define SFLASH_VENDOR_MX			1	//MX25L12845(C2 20 18) -- defaut
#define SFLASH_VENDOR_EON			2	//EN25Q128 ID(1C 30 18)
#define SFLASH_VENDOR_WB			3
#define SFLASH_VENDOR_MICRON		4	//STM(20 BA 18) Now Micron
#define SFLASH_VENDOR_EON_256		5	//EN25Q256 ID(1C 70 19)			
#define SFLASH_VENDOR_MX_256		6	//MX25L25635F ID(C2 20 19)
#define SFLASH_VENDOR_SPANSION		7	//S25FL256 ID(01 02 19)
#define SFLASH_VENDOR_MICRON_256	8	//N25Q256A13ESF40F ID(20 BA 19)


#define DMA_DEST_FONTRAM			0
#define DMA_DEST_CHIPREG			1
#define DMA_DEST_SOSD_LUT			2
#define DMA_DEST_MCU_XMEM			3
//void SpiFlashDmaDestType(BYTE dest, BYTE access_mode);
//void SpiFlashSetCmdLength(BYTE len);
#define	SPIDMA_READ					0
#define SPIDMA_WRITE				1
#define SPIDMA_BUSYCHECK			1


#define DMA_BUFF_REG_ADDR_PAGE		0x04
#define DMA_BUFF_REG_ADDR_INDEX		0xD0
#define DMA_BUFF_REG_ADDR			0x04D0


#define bv(a)          (1<<(a))

//-----------------------------------------------------------------------------
// SPI FLASH Command
//-----------------------------------------------------------------------------
#define SPICMD_WREN				0x06	//write enable
#define SPICMD_WRDI				0x04	//write disable
#define SPICMD_RDID				0x9F	//read identification.
#define SPICMD_RDSR				0x05	//read status register
#define SPICMD_RDSR2			0x35	//read status2 register(WB). dat[1]:QE
#define SPICMD_RDINFO			0x2B	//read information register. S[2]=1:4byte mode 
#define SPICMD_WRSR				0x01	//write status register
#define SPICMD_EN4B				0xB7	//enter 4Byte mode
#define SPICMD_EX4B				0xE9	//exit 4Byte mode
#define SPICMD_ENHBL			0x67	//enter high bank latch mode
#define SPICMD_EXHBL			0x98	//exit high bank latch mode
#define SPICMD_FASTDTRD			0x0D	//fast DT read
#define SPICMD_2DTRD			0xBD	//dual I/O DT Read
#define SPICMD_4DTRD			0xED	//Quad I/O DT Read
#define SPICMD_READ				0x03	//Read data
#define SPICMD_FASTREAD			0x0B	//fast read data
#define SPICMD_2READ			0xBB	//2x I/O read command
#define SPICMD_4READ			0xEB	//4x I/O read command
#define SPICMD_4PP				0x38	//quad page program
#define SPICMD_SE				0x20	//sector erase
#define SPICMD_BE				0xD8	//block erase 64KB
#define SPICMD_BE32K			0x52	//block erase 32KB
#define SPICMD_CE				0x60	//chip erase. 0x60 or 0xC7
#define SPICMD_PP				0x02	//Page program
#define SPICMD_CP				0xAD	//continusly program mode
#define SPICMD_DP				0xB9	//Deep power down
#define SPICMD_RDP				0xAB	//Release from deep power down
#define SPICMD_RES				0xAB	//read electronic ID
#define SPICMD_REMS				0x90	//read electronic manufacturer & device ID
#define SPICMD_REMS2			0xEF	//read ID for 2x I/O mode
#define SPICMD_REMS4			0xDF	//read ID for 4x I/O mode
#define SPICMD_REMS4D			0xCF	//read ID for 4x I/O DT mode
#define SPICMD_ENSO				0xB1	//enter secured OTP
#define SPICMD_EXSO				0xC1	//exit secured OTP
#define SPICMD_RDSCUR			0x2B	//read security register
#define SPICMD_WRSCUR			0x2F	//write security register
#define SPICMD_ESRY				0x70	//enable SO to output RY/BY#
#define SPICMD_DSRY				0x80	//disable SO to output RY/BY#
#define SPICMD_ENPLM			0x55	//enter parallel mode
#define SPICMD_EXPLM			0x45	//exit parallel mode
#define SPICMD_CLSR				0x30	//clear SR fail flags
#define SPICMD_HPM				0xA3	//high performance enable mode
#define SPICMD_WPSEL			0x68	//write protection selection
#define SPICMD_SBLK				0x36	//single block lock
#define SPICMD_SBULK			0x39	//single block unlock
#define SPICMD_RDBLOCK			0x3C	//block protect read
#define SPICMD_GBLK				0x7E	//gang block lock
#define SPICMD_GBULK			0x98	//gang block unlock

#define SPICMD_RDVEREG			0x65	//read volatile enhanced register(micron)
#define SPICMD_WDVEREG			0x61	//write volatile enhanced register(micron)
#define SPICMD_RDVREG			0x85	//read volatile register(micron)
#define SPICMD_WDVREG			0x81	//write volatile register(micron)
#define SPICMD_RDNVREG			0xB5	//read non-volatile register(micron)
#define SPICMD_WDNVREG			0xB1	//write non-volatile register(micron)

#define SPICMD_RDCR				0x15	//read configuration register(Macronix)

#define SYS_MODE_NORMAL		0
#define SYS_MODE_NOINIT		1
#define SYS_MODE_RCD		2

#define	SPI_READ_SLOW		0
#define SPI_READ_FAST 		1
#define SPI_READ_DUAL	 	2
#define SPI_READ_QUAD  		3
#define SPI_READ_DUALIO		4
#define SPI_READ_QUADIO		5
#define SPI_READ_DEDGE		6	//QUADIO with DTR(Double Transfer Rate) mode for micron

//default read mode
#define SPI_READ_MODE		SPI_READ_QUADIO

#define PCLK_DIV_1			0
#define PCLKO_DIV_1			0
#define PCLKO_DIV_2			1
#define PCLKO_DIV_1DOT5		6


#define PCLKO_DIV1			0
#define PCLKO_DIV2			1
#define PCLKO_DIV3			2
#define PCLKO_DIV15			6
//void TW8836_PCLK_set(DWORD sspll_reg, BYTE pclk_div, BYTE pclko_div);
//BYTE TW8836_MCLK_set(BYTE mcuclk_sel, BYTE pllclk_sel, BYTE pllclk_div);

//---PllClk------------------------------
#define PLLCLK_SSPLL		0
#define PLLCLK_PLL108		1

#define PLLCLK_DIV_1P0		0
#define PLLCLK_DIV_1P5		1
#define PLLCLK_DIV_2P0		2
#define PLLCLK_DIV_2P5		3
#define PLLCLK_DIV_3P0		4
#define PLLCLK_DIV_3P5		5
#define PLLCLK_DIV_4P0		6
#define PLLCLK_DIV_5P0		7



#define	SPI_OSDWIN_ENABLE		0x00
#define SPI_OSDWIN_HZOOM		0x00

#define	SPI_OSD_ST				REG400
#define SPI_WIN0_ST				REG420
#define SPI_WIN1_ST				REG440
#define SPI_WIN2_ST				REG450
#define SPI_WIN3_ST				REG460
#define SPI_WIN4_ST				REG470
#define SPI_WIN5_ST				REG480
#define SPI_WIN6_ST				REG490
#define SPI_WIN7_ST				REG4A0
#define SPI_WIN8_ST				REG4B0

//REG410 SOSD_LUT control
#define SOSD_LUT_WEN		0x80
#define SOSD_LUT_INC_ADDR	0x40
#define SOSD_LUT_INC_COLM	0x20
#define SOSD_LUT_INC_NO		0x00
#define SOSD_LUT_HIGH256	0x08
#define SOSD_LUT_BGRP		0x04
#define SOSD_LUT_ATTR		0x03


#define SPI_OSDWIN_SCREEN				0x01
#define	SPI_OSDWIN_BUFFERSTART			0x07
#define SPI_OSDWIN_BUFFERSTART_BIT		0x0A
#define SPI_OSDWIN_DISPSIZE				0x0A
#define SPI_OSDWIN_DISPOFFSET			0x0D
#define SPI_OSDWIN_ALPHA				0x0C	//430(42C+4), 44C
#define SPI_OSDWIN_LUT_PTR				0x0D	//431(42D+4), 44D
#define SPI_OSDWIN_FILLCOLOR			0x0E	//436(42E+8), 44E
#define	SPI_OSDWIN_ANIMATION			0x12    //432
//---------------------------------------------
#define LUTTYPE_BYTE	1	//LUTS
#define LUTTYPE_ADDR	0
#define LUTTYPE_MASK	0x01


#define IMAGE_ITEM_TYPE_0		0	// LUT+IMAGE and use image_info_s
#define IMAGE_ITEM_TYPE_1		1	// RLE2_HEADER+LUT+IMAGE.
#define IMAGE_ITEM_TYPE_2		2	// RLE2_HEADER+LUT+IMAGE but, use image_info_s
#define IMAGE_ITEM_TYPE_3		3	// N/A	IMAGE only
#define IMAGE_ITEM_TYPE_4		4	// N/A  LUT only
#define IMAGE_ITEM_TYPE_5		5	// N/A
#define IMAGE_ITEM_TYPE_6		6	// N/A

#define IMG_TYPE_LUTIMG		0	//LUT+IMG, no size info
#define IMG_TYPE_RLE2		1	//REL2
#define IMG_TYPE_RLE2INFO	2	//RLE2, but use image_info_s
#define IMG_TYPE_IMG		3	//IMAGE ONLY. no size ifno
#define IMG_TYPE_LUT		4	//LUT ONLY.	no size info

#define TWBASE_SPI			0xC0	//note:Not a page. it is a offset.


#define osd_RADAR_MAX_LENGTH	6

/* \brief
*/
typedef enum
{
	OSD_STATE_IDLE,
	OSD_STATE_INIT,
	OSD_STATE_WORK,
	OSD_STATE_ON,
	OSD_STATE_LVDS_ON,
	OSD_STATE_OFF
}enumOsdState;

void InitCore(void);
void InitSystem(enumLogoImageStatus eLogoImageStatus);
void vTwOsdInit(void);
void vTwOsdDelay(void);
void vTwOsdTask(void);
void ShowLogEpsLeft( BYTE number);
void ShowLogEpsRight( BYTE number);
void ShowLogPas(BYTE* pData);
void SpiOsdEnable(BYTE en);
void vTwOsdStateSet(enumOsdState exOsdState);

#endif

