#define __LOGO_C__

#include "iic.h"
#include "logo.h"
#include "TW8836.h"
#include "system.h"
#include "Apu.h"
#include "Timer.h"

#define VBLANK_WAIT_VALUE	0xFFFE
#define offsetradar 0x100000
#define sizeof_LogEps	40


enumOsdState eOsdState = OSD_STATE_IDLE;
static enumLogoImageStatus eLogoImageStatus;
static BYTE SPI_CmdBuffer[8];
static BYTE SPICMD_x_READ      		= 0x03;
static BYTE SPICMD_x_BYTES			= 5;
static BYTE SpiFlashVendor;
static BYTE SpiFlash4ByteAddr;
static BYTE CurrSystemMode;
static BOOL isAccessOpen = TRUE;

static WORD wOsdDelayTimer;

/* \brief
*/
const image_item_info_t LogEpsRight[] = { 
	{ offsetradar+0x000000, 0x001A58 },    // eps_0 
	{ offsetradar+0x056F52, 0x001A79 },    // eps_right (1) 
	{ offsetradar+0x0589CB, 0x001C35 },    // eps_right (2) 
	{ offsetradar+0x05A600, 0x001CF4 },    // eps_right (3) 
	{ offsetradar+0x05C2F4, 0x001DE4 },    // eps_right (4) 
	{ offsetradar+0x05E0D8, 0x001EDA },    // eps_right (5) 
	{ offsetradar+0x05FFB2, 0x001F5F },    // eps_right (6) 
	{ offsetradar+0x061F11, 0x001FF1 },    // eps_right (7) 
	{ offsetradar+0x063F02, 0x002041 },    // eps_right (8) 
	{ offsetradar+0x065F43, 0x002096 },    // eps_right (9) 
	{ offsetradar+0x067FD9, 0x0020AC },    // eps_right (10) 
	{ offsetradar+0x06A085, 0x0020CF },    // eps_right (11) 
	{ offsetradar+0x06C154, 0x0020FA },    // eps_right (12) 
	{ offsetradar+0x06E24E, 0x00216F },    // eps_right (13) 
	{ offsetradar+0x0703BD, 0x002146 },    // eps_right (14) 
	{ offsetradar+0x072503, 0x0021A1 },    // eps_right (15) 
	{ offsetradar+0x0746A4, 0x0021F4 },    // eps_right (16) 
	{ offsetradar+0x076898, 0x002221 },    // eps_right (17) 
	{ offsetradar+0x078AB9, 0x00222B },    // eps_right (18) 
	{ offsetradar+0x07ACE4, 0x00229C },    // eps_right (19) 
	{ offsetradar+0x07CF80, 0x0022E0 },    // eps_right (20) 
	{ offsetradar+0x07F260, 0x00230C },    // eps_right (21) 
	{ offsetradar+0x08156C, 0x00236C },    // eps_right (22) 
	{ offsetradar+0x0838D8, 0x00238F },    // eps_right (23) 
	{ offsetradar+0x085C67, 0x0023F2 },    // eps_right (24) 
	{ offsetradar+0x088059, 0x0023F5 },    // eps_right (25) 
	{ offsetradar+0x08A44E, 0x002418 },    // eps_right (26) 
	{ offsetradar+0x08C866, 0x0023F1 },    // eps_right (27) 
	{ offsetradar+0x08EC57, 0x002428 },    // eps_right (28) 
	{ offsetradar+0x09107F, 0x00243A },    // eps_right (29) 
	{ offsetradar+0x0934B9, 0x002459 },    // eps_right (30) 
	{ offsetradar+0x095912, 0x002445 },    // eps_right (31) 
	{ offsetradar+0x097D57, 0x00244A },    // eps_right (32) 
	{ offsetradar+0x09A1A1, 0x002475 },    // eps_right (33) 
	{ offsetradar+0x09C616, 0x00246B },    // eps_right (34) 
	{ offsetradar+0x09EA81, 0x00247C },    // eps_right (35) 
	{ offsetradar+0x0A0EFD, 0x002478 },    // eps_right (36) 
	{ offsetradar+0x0A3375, 0x00244E },    // eps_right (37) 
	{ offsetradar+0x0A57C3, 0x00244A },    // eps_right (38) 
	{ offsetradar+0x0A7C0D, 0x002455 },    // eps_right (39) 
	{ offsetradar+0x0AA062, 0x0023C8 },    // eps_right (40) 
};
const image_item_info_t LogEpsLeft[] = { 
	{ offsetradar+0x000000, 0x001A58 },    // eps_0 
	{ offsetradar+0x001A58, 0x001A7B },    // eps_left (1) 
	{ offsetradar+0x0034D3, 0x001C36 },    // eps_left (2) 
	{ offsetradar+0x005109, 0x001CF6 },    // eps_left (3) 
	{ offsetradar+0x006DFF, 0x001DE5 },    // eps_left (4) 
	{ offsetradar+0x008BE4, 0x001EDB },    // eps_left (5) 
	{ offsetradar+0x00AABF, 0x001F60 },    // eps_left (6) 
	{ offsetradar+0x00CA1F, 0x001FF2 },    // eps_left (7) 
	{ offsetradar+0x00EA11, 0x002043 },    // eps_left (8) 
	{ offsetradar+0x010A54, 0x002099 },    // eps_left (9) 
	{ offsetradar+0x012AED, 0x0020AD },    // eps_left (10) 
	{ offsetradar+0x014B9A, 0x0020D1 },    // eps_left (11) 
	{ offsetradar+0x016C6B, 0x0020FD },    // eps_left (12) 
	{ offsetradar+0x018D68, 0x002172 },    // eps_left (13) 
	{ offsetradar+0x01AEDA, 0x002148 },    // eps_left (14) 
	{ offsetradar+0x01D022, 0x0021A2 },    // eps_left (15) 
	{ offsetradar+0x01F1C4, 0x0021F4 },    // eps_left (16) 
	{ offsetradar+0x0213B8, 0x002221 },    // eps_left (17) 
	{ offsetradar+0x0235D9, 0x00222B },    // eps_left (18) 
	{ offsetradar+0x025804, 0x00229C },    // eps_left (19) 
	{ offsetradar+0x027AA0, 0x0022E0 },    // eps_left (20) 
	{ offsetradar+0x029D80, 0x00230C },    // eps_left (21) 
	{ offsetradar+0x02C08C, 0x00236C },    // eps_left (22) 
	{ offsetradar+0x02E3F8, 0x002390 },    // eps_left (23) 
	{ offsetradar+0x030788, 0x0023F2 },    // eps_left (24) 
	{ offsetradar+0x032B7A, 0x0023F5 },    // eps_left (25) 
	{ offsetradar+0x034F6F, 0x002418 },    // eps_left (26) 
	{ offsetradar+0x037387, 0x0023F2 },    // eps_left (27) 
	{ offsetradar+0x039779, 0x002428 },    // eps_left (28) 
	{ offsetradar+0x03BBA1, 0x00243A },    // eps_left (29) 
	{ offsetradar+0x03DFDB, 0x002459 },    // eps_left (30) 
	{ offsetradar+0x040434, 0x002445 },    // eps_left (31) 
	{ offsetradar+0x042879, 0x00244A },    // eps_left (32) 
	{ offsetradar+0x044CC3, 0x002475 },    // eps_left (33) 
	{ offsetradar+0x047138, 0x00246C },    // eps_left (34) 
	{ offsetradar+0x0495A4, 0x00247C },    // eps_left (35) 
	{ offsetradar+0x04BA20, 0x002478 },    // eps_left (36) 
	{ offsetradar+0x04DE98, 0x00244E },    // eps_left (37) 
	{ offsetradar+0x0502E6, 0x00244A },    // eps_left (38) 
	{ offsetradar+0x052730, 0x002456 },    // eps_left (39) 
	{ offsetradar+0x054B86, 0x0023CC },    // eps_left (40) 
	
}; 

/* \brief
*/
const image_item_info_t LogSolid[] = { 
	{ offsetradar+0x0AC42A, 0x005A00 },    // 0_top (1) 
	{ offsetradar+0x0B1E2A, 0x015D01 },    // 1_top 
	{ offsetradar+0x0C7B2B, 0x005A00 },    // 2_top 
	{ offsetradar+0x0CD52B, 0x016BB3 },    // center_car 
	{ offsetradar+0x0E40DE, 0x008D54 },    // center_x_car 

}; 

/* \brief
*/
const image_item_info_t LogLut[] = {
	{ offsetradar+0x239C40, 0x000400 },    // 1_top 
}; 

/* \brief
*/
const image_item_info_t LogRadarFL[] ={
	{ offsetradar+0x0ECE32, 0x0030D0 },    // F_L (0) 
	{ offsetradar+0x0EFF02, 0x0030D0 },    // F_L (1) 
	{ offsetradar+0x0F2FD2, 0x0030D0 },    // F_L (2) 
	{ offsetradar+0x0F60A2, 0x0030D0 },    // F_L (3) 
	{ offsetradar+0x0F9172, 0x0030D0 },    // F_L (4) 
};


/* \brief
*/
const image_item_info_t LogRadarFR[] ={
	{ offsetradar+0x0FC242, 0x003078 },    // F_R (0) 
	{ offsetradar+0x0FF2BA, 0x003078 },    // F_R (1) 
	{ offsetradar+0x102332, 0x003078 },    // F_R (2) 
	{ offsetradar+0x1053AA, 0x003078 },    // F_R (3) 
	{ offsetradar+0x108422, 0x003078 },    // F_R (4) 
};

/* \brief
*/
const image_item_info_t LogRadarRL[] ={
	{ offsetradar+0x10B49A, 0x0040FE },    // R_L_0 (0) 
	{ offsetradar+0x10F598, 0x0040FE },    // R_L_0 (1) 
	{ offsetradar+0x113696, 0x0040FE },    // R_L_0 (2) 
	{ offsetradar+0x117794, 0x0040FE },    // R_L_0 (3) 
	{ offsetradar+0x11B892, 0x0040FE },    // R_L_0 (4) 
	{ offsetradar+0x11F990, 0x0040FE },    // R_L_1 (0) 
	{ offsetradar+0x123A8E, 0x0040FE },    // R_L_1 (1) 
	{ offsetradar+0x127B8C, 0x0040FE },    // R_L_1 (2) 
	{ offsetradar+0x12BC8A, 0x0040FE },    // R_L_1 (3) 
	{ offsetradar+0x12FD88, 0x0040FE },    // R_L_1 (4) 
	{ offsetradar+0x133E86, 0x0040FE },    // R_L_2 (0) 
	{ offsetradar+0x137F84, 0x0040FE },    // R_L_2 (1) 
	{ offsetradar+0x13C082, 0x0040FE },    // R_L_2 (2) 
	{ offsetradar+0x140180, 0x0040FE },    // R_L_2 (3) 
	{ offsetradar+0x14427E, 0x0040FE },    // R_L_2 (4) 
	{ offsetradar+0x14837C, 0x0040FE },    // R_L_3 (0) 
	{ offsetradar+0x14C47A, 0x0040FE },    // R_L_3 (1) 
	{ offsetradar+0x150578, 0x0040FE },    // R_L_3 (2) 
	{ offsetradar+0x154676, 0x0040FE },    // R_L_3 (3) 
	{ offsetradar+0x158774, 0x0040FE },    // R_L_3 (4) 
	{ offsetradar+0x15C872, 0x0040FE },    // R_L_4 (0) 
	{ offsetradar+0x160970, 0x0040FE },    // R_L_4 (1) 
	{ offsetradar+0x164A6E, 0x0040FE },    // R_L_4 (2) 
	{ offsetradar+0x168B6C, 0x0040FE },    // R_L_4 (3) 
	{ offsetradar+0x16CC6A, 0x0040FE },    // R_L_4 (4) 
};


/* \brief
*/
const image_item_info_t LogRadarRR[] ={
	{ offsetradar+0x170D68, 0x004174 },    // R_R_0 (0) 
	{ offsetradar+0x174EDC, 0x004174 },    // R_R_0 (1) 
	{ offsetradar+0x179050, 0x004174 },    // R_R_0 (2) 
	{ offsetradar+0x17D1C4, 0x004174 },    // R_R_0 (3) 
	{ offsetradar+0x181338, 0x004174 },    // R_R_0 (4) 
	{ offsetradar+0x1854AC, 0x004174 },    // R_R_1 (0) 
	{ offsetradar+0x189620, 0x004174 },    // R_R_1 (1) 
	{ offsetradar+0x18D794, 0x004174 },    // R_R_1 (2) 
	{ offsetradar+0x191908, 0x004174 },    // R_R_1 (3) 
	{ offsetradar+0x195A7C, 0x004174 },    // R_R_1 (4) 
	{ offsetradar+0x199BF0, 0x004174 },    // R_R_2 (0) 
	{ offsetradar+0x19DD64, 0x004174 },    // R_R_2 (1) 
	{ offsetradar+0x1A1ED8, 0x004174 },    // R_R_2 (2) 
	{ offsetradar+0x1A604C, 0x004174 },    // R_R_2 (3) 
	{ offsetradar+0x1AA1C0, 0x004174 },    // R_R_2 (4) 
	{ offsetradar+0x1AE334, 0x004174 },    // R_R_3 (0) 
	{ offsetradar+0x1B24A8, 0x004174 },    // R_R_3 (1) 
	{ offsetradar+0x1B661C, 0x004174 },    // R_R_3 (2) 
	{ offsetradar+0x1BA790, 0x004174 },    // R_R_3 (3) 
	{ offsetradar+0x1BE904, 0x004174 },    // R_R_3 (4) 
	{ offsetradar+0x1C2A78, 0x004174 },    // R_R_4 (0) 
	{ offsetradar+0x1C6BEC, 0x004174 },    // R_R_4 (1) 
	{ offsetradar+0x1CAD60, 0x004174 },    // R_R_4 (2) 
	{ offsetradar+0x1CEED4, 0x004174 },    // R_R_4 (3) 
	{ offsetradar+0x1D3048, 0x004174 },    // R_R_4 (4) 
}; 

/* \brief
*/
const image_item_info_t LogRadarXFL[] ={
	{ offsetradar+0x1D71BC, 0x0014BE },    // x_car_F_L (0) 
	{ offsetradar+0x1D867A, 0x0014BE },    // x_car_F_L (1) 
	{ offsetradar+0x1D9B38, 0x0014BE },    // x_car_F_L (2) 
	{ offsetradar+0x1DAFF6, 0x0014BE },    // x_car_F_L (3) 
	{ offsetradar+0x1DC4B4, 0x0014BE },    // x_car_F_L (4) 
};

/* \brief
*/
const image_item_info_t LogRadarXFR[] ={
	{ offsetradar+0x1DD972, 0x0014BE },    // x_car_F_R (0) 
	{ offsetradar+0x1DEE30, 0x0014BE },    // x_car_F_R (1) 
	{ offsetradar+0x1E02EE, 0x0014BE },    // x_car_F_R (2) 
	{ offsetradar+0x1E17AC, 0x0014BE },    // x_car_F_R (3) 
	{ offsetradar+0x1E2C6A, 0x0014BE },    // x_car_F_R (4) 
};


/* \brief
*/
const image_item_info_t LogRadarXRL[] ={
	{ offsetradar+0x1E4128, 0x001B6C },    // x_car_R_L_0 (0) 
	{ offsetradar+0x1E5C94, 0x001B6C },    // x_car_R_L_0 (1) 
	{ offsetradar+0x1E7800, 0x001B6C },    // x_car_R_L_0 (2) 
	{ offsetradar+0x1E936C, 0x001B6C },    // x_car_R_L_0 (3) 
	{ offsetradar+0x1EAED8, 0x001B6C },    // x_car_R_L_0 (4) 
	{ offsetradar+0x1ECA44, 0x001B6C },    // x_car_R_L_1 (0) 
	{ offsetradar+0x1EE5B0, 0x001B6C },    // x_car_R_L_1 (1) 
	{ offsetradar+0x1F011C, 0x001B6C },    // x_car_R_L_1 (2) 
	{ offsetradar+0x1F1C88, 0x001B6C },    // x_car_R_L_1 (3) 
	{ offsetradar+0x1F37F4, 0x001B6C },    // x_car_R_L_1 (4) 
	{ offsetradar+0x1F5360, 0x001B6C },    // x_car_R_L_2 (0) 
	{ offsetradar+0x1F6ECC, 0x001B6C },    // x_car_R_L_2 (1) 
	{ offsetradar+0x1F8A38, 0x001B6C },    // x_car_R_L_2 (2) 
	{ offsetradar+0x1FA5A4, 0x001B6C },    // x_car_R_L_2 (3) 
	{ offsetradar+0x1FC110, 0x001B6C },    // x_car_R_L_2 (4) 
	{ offsetradar+0x1FDC7C, 0x001B6C },    // x_car_R_L_3 (0) 
	{ offsetradar+0x1FF7E8, 0x001B6C },    // x_car_R_L_3 (1) 
	{ offsetradar+0x201354, 0x001B6C },    // x_car_R_L_3 (2) 
	{ offsetradar+0x202EC0, 0x001B6C },    // x_car_R_L_3 (3) 
	{ offsetradar+0x204A2C, 0x001B6C },    // x_car_R_L_3 (4) 
	{ offsetradar+0x206598, 0x001B6C },    // x_car_R_L_4 (0) 
	{ offsetradar+0x208104, 0x001B6C },    // x_car_R_L_4 (1) 
	{ offsetradar+0x209C70, 0x001B6C },    // x_car_R_L_4 (2) 
	{ offsetradar+0x20B7DC, 0x001B6C },    // x_car_R_L_4 (3) 
    { offsetradar+0x20D348, 0x001B6C },    // x_car_R_L_4 (4) 
};

/* \brief
*/
const image_item_info_t LogRadarXRR[] ={
	{ offsetradar+0x20EEB4, 0x001B6C },    // x_car_R_R_0 (0) 
	{ offsetradar+0x210A20, 0x001B6C },    // x_car_R_R_0 (1) 
	{ offsetradar+0x21258C, 0x001B6C },    // x_car_R_R_0 (2) 
	{ offsetradar+0x2140F8, 0x001B6C },    // x_car_R_R_0 (3) 
	{ offsetradar+0x215C64, 0x001B6C },    // x_car_R_R_0 (4) 
	{ offsetradar+0x2177D0, 0x001B6C },    // x_car_R_R_1 (0) 
	{ offsetradar+0x21933C, 0x001B6C },    // x_car_R_R_1 (1) 
	{ offsetradar+0x21AEA8, 0x001B6C },    // x_car_R_R_1 (2) 
	{ offsetradar+0x21CA14, 0x001B6C },    // x_car_R_R_1 (3) 
	{ offsetradar+0x21E580, 0x001B6C },    // x_car_R_R_1 (4) 
	{ offsetradar+0x2200EC, 0x001B6C },    // x_car_R_R_2 (0) 
	{ offsetradar+0x221C58, 0x001B6C },    // x_car_R_R_2 (1) 
	{ offsetradar+0x2237C4, 0x001B6C },    // x_car_R_R_2 (2) 
	{ offsetradar+0x225330, 0x001B6C },    // x_car_R_R_2 (3) 
	{ offsetradar+0x226E9C, 0x001B6C },    // x_car_R_R_2 (4) 
	{ offsetradar+0x228A08, 0x001B6C },    // x_car_R_R_3 (0) 
	{ offsetradar+0x22A574, 0x001B6C },    // x_car_R_R_3 (1) 
	{ offsetradar+0x22C0E0, 0x001B6C },    // x_car_R_R_3 (2) 
	{ offsetradar+0x22DC4C, 0x001B6C },    // x_car_R_R_3 (3) 
	{ offsetradar+0x22F7B8, 0x001B6C },    // x_car_R_R_3 (4) 
	{ offsetradar+0x231324, 0x001B6C },    // x_car_R_R_4 (0) 
	{ offsetradar+0x232E90, 0x001B6C },    // x_car_R_R_4 (1) 
	{ offsetradar+0x2349FC, 0x001B6C },    // x_car_R_R_4 (2) 
	{ offsetradar+0x236568, 0x001B6C },    // x_car_R_R_4 (3) 
	{ offsetradar+0x2380D4, 0x001B6C },    // x_car_R_R_4 (4) 
}; 

static void SpiOsdWinBufferMem_Nobuff(BYTE winno, DWORD start);




void WaitVBlank(BYTE cnt)
{
	BYTE i;
	WORD loop;
	U8 page = 0;
	
	ReadTW88Page(&page);
	WriteTW88Page(PAGE0_GENERAL );

	for ( i=0; i<cnt; i++ )
	{
		WriteTW88(REG002, 0xff );
		loop = 0;
		while (!( ReadTW88(REG002 ) & 0x40 ) )
		{
			// wait VBlank
			loop++;
			if(loop > VBLANK_WAIT_VALUE  )
			{
				break;
			}
		}		
	}
	WriteTW88Page(page);
}


BYTE SpiFlashChipRegCmd(BYTE cmd, BYTE w_len, BYTE r_len, BYTE vblank)
{
	BYTE dma_option;
	BYTE i;
	volatile BYTE vdata;

	WriteTW88Page(PAGE4_SPI);
	WriteTW88(REG4C3,(DMA_DEST_CHIPREG << 6) | (1+w_len)); //cmd+cmd_buff_len
	WriteTW88(REG4C6, DMA_BUFF_REG_ADDR_PAGE);
	WriteTW88(REG4C7, DMA_BUFF_REG_ADDR_INDEX);
	WriteTW88(REG4C8, 0x00 );							// data Buff count middle
	WriteTW88(REG4C9, r_len );							// data Buff count Lo
	WriteTW88(REG4CA, cmd );
	for(i=0; i < w_len; i++)
		WriteTW88(REG4CB+i, SPI_CmdBuffer[i]);
	WriteTW88(REG4DA, 0x00 );							// data Buff count high
	//vblank wait
	if(vblank)
		WaitVBlank(vblank);

	//dma option
	dma_option = 0x00;
	if(cmd==SPICMD_PP
	|| cmd==SPICMD_SE
	|| cmd==SPICMD_BE
	|| cmd==SPICMD_BE32K
	|| cmd==SPICMD_CE
	)
		dma_option |= 0x02;	//DMA Buff Write Mode

	if(cmd==SPICMD_WDVEREG 
	|| cmd==SPICMD_WDVREG 
	|| cmd==SPICMD_WDNVREG

	|| cmd==SPICMD_READ
	|| cmd==SPICMD_FASTREAD
	|| cmd==SPICMD_4READ

	|| cmd==SPICMD_WRSR

	|| cmd==SPICMD_SE
	|| cmd==SPICMD_BE
	|| cmd==SPICMD_BE32K
	|| cmd==SPICMD_CE
	)
		dma_option |= 0x04;	//BUSY CHECK

	// DMA-start
	WriteTW88(REG4C4, 0x01 | dma_option);	

	if(cmd==SPICMD_EX4B) SpiFlash4ByteAddr = 0;
	if(cmd==SPICMD_EN4B) SpiFlash4ByteAddr = 1;


	//wait done
	for(i=0; i < 200; i++)
	{
		vdata = ReadTW88(REG4C4);
		if((vdata & 0x01)==0)
			break;
		Delay(10);
	}
	if(i==200)
	{
		return 2;	//fail:busy
	}
	//read
	if(cmd==SPICMD_PP)
		r_len = 0;
	for(i=0; i < r_len; i++) 
		SPI_CmdBuffer[i] = ReadTW88(REG4D0+i);		

	return 0;	//success
}

void SPI_SetReadModeByRegister( BYTE mode )
{
	WriteTW88Page(PAGE4_SPI);
	WriteTW88(REG4C0, (ReadTW88(REG4C0) & ~0x07) | mode);

	switch( mode )
	{
		case 0:	//--- Slow
			//max speed is 50MHz.
			//but, normally, 54MHz is working.
			SPICMD_x_READ	= 0x03;	
			SPICMD_x_BYTES	= 4;	//(8+24)/8
			break;
		case 1:	//--- Fast
			SPICMD_x_READ	= 0x0b;	
			SPICMD_x_BYTES	= 5;   //(8+24+8)/8. 8 dummy
			break;
		case 2:	//--- Dual
			SPICMD_x_READ	= 0x3b;
			SPICMD_x_BYTES	= 5;
			break;
		case 3:	//--- Quad
			SPICMD_x_READ	= 0x6b;	
			SPICMD_x_BYTES	= 5;
			break;
		case 4:	//--- Dual-IO
			SPICMD_x_READ	= 0xbb;	
			SPICMD_x_BYTES	= 5;	//(8+12*2+4*2)/8. Note:*2 means 2 lines.
			break;
		case 5:	//--- Quad-IO
			SPICMD_x_READ	= 0xeb;	 
			SPICMD_x_BYTES	= 7;   //(8+6*4+2*4+4*4)/8. Note:*4 means 4 lines.
			break;
		case 6:	//--- DEdge. DTR(Double Transfer Rate)
			SPICMD_x_READ	= 0xed;	 
			SPICMD_x_BYTES	= 12;   //.
			break;
		default:
			//fast read mode can support Extender/Qual/Quad.
			SPICMD_x_READ	= 0x0b;	
			SPICMD_x_BYTES	= 5;   //(8+24+8)/8. 8 dummy
			break;
			
 	}
}
void SPI_Set4BytesAddress(BYTE fOn)
{
	BYTE cmd;
	
	if(SpiFlashVendor == SFLASH_VENDOR_MICRON_256)	 //BK130703
		SpiFlashChipRegCmd(SPICMD_WREN, 0, 0, 0);

	cmd = 0;	
	if(fOn)
	{
		if(SpiFlash4ByteAddr==0)
		{
			SpiFlash4ByteAddr = 1;
			cmd = SPICMD_EN4B;
		}	
	}
	else
	{
		if(SpiFlash4ByteAddr)
		{
			SpiFlash4ByteAddr = 0;
			cmd = SPICMD_EX4B;
		}	
	}
	if(cmd)
	{	
		//BKTODO: skip the done flag check.
		(void)SpiFlashChipRegCmd(cmd,0,0,0);
	}
}

BYTE SPI_QUADInit(void)
{
	BYTE dat0;
	BYTE vid;
	BYTE cid;
	BYTE ret;
	BYTE temp;
							 
	ret = SpiFlashChipRegCmd(SPICMD_RDID,0,3,0);
	if(ret)
	{
		return 0;
	}
	vid  = SPI_CmdBuffer[0];
	dat0 = SPI_CmdBuffer[1];
	cid  = SPI_CmdBuffer[2];


	if(vid == 0x1C)
	{
		SpiFlashVendor = SFLASH_VENDOR_EON;
		if(dat0==0x70 && cid==0x19)
			SpiFlashVendor = SFLASH_VENDOR_EON_256;
	}
	else if(vid == 0xC2)
	{
	 	SpiFlashVendor = SFLASH_VENDOR_MX;
		if(dat0==0x20 && cid==0x19)
			SpiFlashVendor = SFLASH_VENDOR_MX_256;
	} 
	else if(vid == 0xEF)
		SpiFlashVendor = SFLASH_VENDOR_WB;
	else if(vid == 0x20)
	{
		if(cid == 0x18)
			SpiFlashVendor = SFLASH_VENDOR_MICRON; //numonyx, micron
		else if(cid == 0x19)
			SpiFlashVendor = SFLASH_VENDOR_MICRON_256;
		else
		{		
			return 0;
		}
	}
	else if(vid == 0x01)
	{
		SpiFlashVendor = SFLASH_VENDOR_SPANSION;	
	}
	else
	{
		return 0;
	}

	//----------------------------
	//read status register
	//----------------------------

	if (vid == 0xC2 || vid == 0x1C)
	{ 							//C2:MX 1C:EON
		ret=SpiFlashChipRegCmd(SPICMD_RDSR, 0, 1, 0);
		temp = SPI_CmdBuffer[0] & 0x40;							
		//if 0, need to enable quad
	}
	else if (vid == 0xEF)
	{					// WB
		//if(cid == 0x18) {				//Q128 case, different status read command
			ret=SpiFlashChipRegCmd(SPICMD_RDSR2,0, 1, 0);
			temp = SPI_CmdBuffer[0];							//dat0[1]:QE
			//if 0, need to enable quad

	}
	else if(vid == 0x20 )
	{//SFLASH_VENDOR_MICRON
	
#if 1 //BK131119
		ret=SpiFlashChipRegCmd(SPICMD_RDNVREG,0, 2, 0);	//cmd, read NonVolatile register
		if(SPI_CmdBuffer[0] & 0x10)
			temp = 0; //need an enable routine
		else if((SPI_CmdBuffer[1] & 0xF0) != 0x60)
			temp = 0; //need an enable routine
		else
			temp = 1;  //ok. we have QuadIO and 6 dummy.
#else
		ret=SpiFlashChipRegCmd(SPICMD_RDVREG,0, 1, 0);	//cmd, read Volatile register
		temp = SPI_CmdBuffer[0];
		Printf("\n\rVolatile Register: %02bx", temp );
		if(temp != 0x6B)
			temp = 0; //need an enable routine
#endif
	}
	
	else if(vid == 0x01 )
	{	  //SFLASH_VENDOR_SPANSION
		ret=SpiFlashChipRegCmd(SPICMD_RDSR2,0, 1, 0);	//cmd, read Volatile register
		temp = SPI_CmdBuffer[0];
		temp = SPI_CmdBuffer[0] & 0x02;	//if 0, need an enable routine
	}
	if(temp)
		//HW is ready for QuadIO.
		return SpiFlashVendor;

	//----------------------------
	// enable QuadIO
	//----------------------------
	if (vid == 0xC2 || vid == 0x1c)
	{
		(void)SpiFlashChipRegCmd(SPICMD_WREN, 0, 0, 0);

		SPI_CmdBuffer[0] = 0x40;	//en QAUD mode
		(void)SpiFlashChipRegCmd(SPICMD_WRSR, 1, 0, 0);
	

		(void)SpiFlashChipRegCmd(SPICMD_WRDI, 0, 0, 0);
	}
	else if(vid == 0xEF)
	{
		(void)SpiFlashChipRegCmd(SPICMD_WREN, 0, 0, 0);
		SPI_CmdBuffer[0] = 0x00;	//cmd, en QAUD mode
		SPI_CmdBuffer[1] = 0x02;	
		(void)SpiFlashChipRegCmd(SPICMD_WRSR, 2, 0, 0);
		(void)SpiFlashChipRegCmd(SPICMD_WRDI, 0, 0, 0);
	}
	else if(vid == 0x20 )
	{	 //SFLASH_VENDOR_MICRON

		(void)SpiFlashChipRegCmd(SPICMD_WREN, 0, 0, 0);
		SPI_CmdBuffer[0] = 0xEF;		//LSB. [4] is a QuadIO, not [3]. DO not write 0xF7. It will be corruptted.
		SPI_CmdBuffer[1] = 0x6F;		//MSB. [7:4] is dummy cycle
		(void)SpiFlashChipRegCmd(SPICMD_WDNVREG, 2, 0, 0);	// cmd, write Non-Volatile.

		SpiFlashVendor = 0xFF;

	}
	else if(vid == 0x01)
	{	//SPANSION
		(void)SpiFlashChipRegCmd(SPICMD_WREN, 0, 0, 0);
		SPI_CmdBuffer[0] = 0x02;	//en QAUD mode
		(void)SpiFlashChipRegCmd(SPICMD_WRSR, 1, 0, 0);
		(void)SpiFlashChipRegCmd(SPICMD_WRDI, 0, 0, 0);
	}
	return SpiFlashVendor;
}

void InitCore(void)
{

	//----- Search SPIFlash & enable QuadIO.
	SpiFlashVendor = SPI_QUADInit();

	if(SpiFlashVendor==0xFF)
	{
		//we need a reboot.
		CurrSystemMode = SYS_MODE_NOINIT;
		isAccessOpen = FALSE;
		return;
	}

	//---- Match DMA READ mode with SPI-read
	if(SpiFlashVendor==0)
	{
		//---------------------
		//try FAST mode.
		SPI_SetReadModeByRegister(SPI_READ_FAST);
	}
	else
	{
		//---------------------
		// now TW8836HW using SPI_READ_MODE (QuadIO mode).
		SPI_SetReadModeByRegister(SPI_READ_QUAD);		
	}

	if(SpiFlashVendor==SFLASH_VENDOR_MX_256)
		SPI_Set4BytesAddress(ON);
	if(SpiFlashVendor==SFLASH_VENDOR_MICRON_256)
		SPI_Set4BytesAddress(ON);

}


BYTE SpiWinBuff[10*0x10];

struct SOSD_CTRL_s
	{
		BYTE *reg[9];
		struct SOsdLut_s lut[9];
		struct SOsdRle_s rlc[2];
	} SOsdCtrl;
//TW8836 has 9 windows
const BYTE SpiOsdWinBase[9] =
	{
		SPI_WIN0_ST, SPI_WIN1_ST, SPI_WIN2_ST,
		SPI_WIN3_ST, SPI_WIN4_ST, SPI_WIN5_ST,
		SPI_WIN6_ST, SPI_WIN7_ST, SPI_WIN8_ST
	};

void SOsd_init(void)
{
	BYTE win;
	BYTE offset;

	for(win=0; win <= 8; win++)
	{
		if(win) offset = win+1;
		else	offset = 0;
		SOsdCtrl.reg[win] = &SpiWinBuff[offset << 4];
	}
}

void SOsd_CleanReg(void)
{
	BYTE i;
	for(i=0; i < 0xA0; i++)
		SpiWinBuff[i] =0;	
}

void SOsd_CleanLut(void)
{
	struct SOsdLut_s *pLut;
	BYTE win;

	for(win=0; win <= 8; win++)
	{
		pLut = &SOsdCtrl.lut[win];
		pLut->type = 0;
		pLut->offset = 0;
		pLut->size = 0;
		pLut->addr = 0;
		pLut->alpha = 0xFF;	//disable alpha
	}
}
void SOsd_CleanRlc(void)
{
	SOsdCtrl.rlc[0].win = 0;  SOsdCtrl.rlc[0].bpp = 0; SOsdCtrl.rlc[0].count = 0;
	SOsdCtrl.rlc[1].win = 0;  SOsdCtrl.rlc[1].bpp = 0; SOsdCtrl.rlc[1].count = 0;
}
void SOsd_UpdateReg(BYTE s_win, BYTE e_win)
{
	BYTE win;
	BYTE *data_p;
	WORD reg_i;



	for(win=s_win; win <= e_win; win++)
	{
		data_p = SOsdCtrl.reg[win];	
		reg_i = SpiOsdWinBase[win];

		WriteTW88(reg_i++, *data_p++);	//0
		WriteTW88(reg_i++, *data_p++);	//1
		WriteTW88(reg_i++, *data_p++);	//2
		WriteTW88(reg_i++, *data_p++);	//3
		WriteTW88(reg_i++, *data_p++);	//4
		WriteTW88(reg_i++, *data_p++);	//5
		WriteTW88(reg_i++, *data_p++);	//6
		WriteTW88(reg_i++, *data_p++);	//7
		WriteTW88(reg_i++, *data_p++);	//8
		WriteTW88(reg_i++, *data_p++);	//9
		WriteTW88(reg_i++, *data_p++);	//A
		WriteTW88(reg_i++, *data_p++);	//B
		WriteTW88(reg_i++, *data_p++);	//C
		WriteTW88(reg_i++, *data_p++);	//D
		WriteTW88(reg_i++, *data_p++);	//E
		WriteTW88(reg_i++, *data_p++);	//F
		if(win==0)
		{
			WriteTW88(reg_i++, *data_p++);	//10  REG430
			WriteTW88(reg_i++, *data_p++);	//11
			WriteTW88(reg_i++, *data_p++);	//12
			WriteTW88(reg_i++, *data_p++);	//13
			WriteTW88(reg_i++, *data_p++);	//14
			WriteTW88(reg_i++, *data_p++);	//15
			WriteTW88(reg_i++, *data_p++);	//16 REG436
			WriteTW88(reg_i++, *data_p++);	//17 REG437
		}
	}
}
void SOsd_UpdateRlc(void)
{
	BYTE i;
	BYTE bTemp;
	struct SOsdRle_s *pRlc;

	WriteTW88Page(PAGE4_SOSD);
	for(i=0; i < 2; i++)
	{
		pRlc = &SOsdCtrl.rlc[i];
		WriteTW88(REG404 + 2 - i*2, pRlc->win << 4);
		bTemp = pRlc->bpp;
		if(bTemp==7)	bTemp++;
		bTemp <<= 4;
		bTemp |= pRlc->count;
		WriteTW88(REG405 + 2 - i*2, bTemp);
	}
}

void SpiOsdEnable(BYTE en)
{
	BYTE dat;
	WriteTW88Page(PAGE4_SOSD );
	dat = ReadTW88(REG400);
	if( en )
	{
		WriteTW88(REG400, dat | 0x04);						//enable SpiOSD
	}
	else
	{
		WriteTW88(REG400, dat & ~0x04);						//disable SpiOSD
	}
}


void SpiOsdWinBufferSizeXY (BYTE winno, WORD x, WORD y)
{
	BYTE index,value;

	index = SpiOsdWinBase[winno] + SPI_OSDWIN_DISPSIZE;
	WriteTW88Page(PAGE4_SOSD);

	if(winno)
	{
		WriteTW88(index++, x>>8);
		WriteTW88(index++, x);
	}
	else
	{
		//now only for WIN0
		value = (BYTE)(x >> 8);
		value <<= 4;
		value |= (BYTE)( y>>8 );
		WriteTW88(index++,value);  // 42a
		WriteTW88(index++,(BYTE)x);//42b
		WriteTW88(index++,(BYTE)y);//42c

	}
}

void SpiOsdWinScreen_Nobuff(BYTE winno, WORD x, WORD y, WORD w, WORD h)
{
	BYTE index;

	index = SpiOsdWinBase[winno] + SPI_OSDWIN_SCREEN;
	WriteTW88Page(PAGE4_SOSD);//WriteTW88( 0xff, OSD_PAGE );
	WriteTW88(index++, (BYTE)((x>>8)|((BYTE)(y>>8))<<4));
	WriteTW88(index++, (BYTE)x);
	WriteTW88(index++, (BYTE)y);
	WriteTW88(index++, (BYTE)((w>>8)|((BYTE)(h>>8))<<4));
	WriteTW88(index++, (BYTE)w);
	WriteTW88(index,   (BYTE)h);
}

void SpiOsdWinLutOffset_DMA( BYTE winno, U16 table_offset )
{
	BYTE index;

	index = SpiOsdWinBase[winno] + SPI_OSDWIN_LUT_PTR;

	WriteTW88( index, (BYTE)(table_offset>>4));
}

void SpiOsdLoadLUT_AB(BYTE winno,BYTE type, WORD LutOffset, WORD size, U32 address)
{
	BYTE reg;

	if(type==0)
	{
		// IO方式加载色表一般不用这种方式
		return;
	}	

//	McuSpiClkToPclk(0x02);	//with divider 1=1.5(72MHz)	try 2


	WriteTW88Page( PAGE4_SOSD );

	//--- SPI-OSD config
	reg = 0;										//ReadTW88(0x10) ;
	if(LutOffset & 0x100)
		reg = 0x08;
	if(type==0)
		WriteTW88( 0x10, reg | 0xc0 );    	// LUT Write Mode, En & address ptr inc.
	else
		WriteTW88( 0x10, reg | 0xa0 );    	// LUT Write Mode, En & byte ptr inc.
		
	WriteTW88( 0x11, (BYTE)LutOffset ); 			// LUT addr. set 0 on DMA

if(winno==1||winno==2)
	WriteTW88( 0x10, ReadTW88(0x10)|0x04 );	       // LUT B
else
	WriteTW88( 0x10, ReadTW88(0x10)&~0x04 );	   // LUTA


	//Spi Flash DMA
	WriteTW88( TWBASE_SPI+0x04, 0x00 );		// DMA stop	
	WriteTW88( TWBASE_SPI+0x03, 0x80|SPICMD_x_BYTES); 	//LUT,Increase, 0x0B with 5 commands, 0xeb with 7 commands  SPICMD_BYTES
	           
	WriteTW88( TWBASE_SPI+0x0a,  SPICMD_x_READ);			// SPI Command=R  SPICMD_READ
	WriteTW88( TWBASE_SPI+0x0b, (BYTE)(address>>16) ); 	// SPI Addr
	WriteTW88( TWBASE_SPI+0x0c, (BYTE)(address>>8) );
	WriteTW88( TWBASE_SPI+0x0d, (BYTE)(address) ); 			

	//if use byte ptr inc.
	WriteTW88( TWBASE_SPI+0x06, (BYTE)(LutOffset >> 6) ); 	//LutOffset[8:6] -> R4C6[2:0]
	WriteTW88( TWBASE_SPI+0x07, (BYTE)(LutOffset << 2) );	//LutOffset[5:0] -> R4C7[7:2] 
		                                                        //R4C7[1:0]  start of byteptr
	WriteTW88( TWBASE_SPI+0x1a, 0x00 ); // DMA size
	WriteTW88( TWBASE_SPI+0x08, (BYTE)(size >> 8) );
	WriteTW88( TWBASE_SPI+0x09, (BYTE)size );
	
	WriteTW88( TWBASE_SPI+0x04, 0x01 ); // DMA Start

//	McuSpiClkRestore();
}

void SpiOsdWin_Rle(BYTE winno,BYTE dcnt,BYTE ccnt)
{
	BYTE index;

	index = SpiOsdWinBase[winno] + SPI_OSDWIN_DISPSIZE;
	WriteTW88Page(PAGE4_SOSD);//WriteTW88( 0xff, OSD_PAGE );

	if(winno==1||winno==2)
	{
		//7 means 8BPP with 128 color.
		if(dcnt==7)
		dcnt++;
		WriteTW88(0x07, (dcnt << 4) | (ccnt));
		WriteTW88(0x06, (winno<<4));
	}
	else
	{
		//7 means 8BPP with 128 color.
		if(dcnt==7)
		dcnt++;
		WriteTW88(0x05, (dcnt << 4) | (ccnt));
		WriteTW88(0x04, (winno<<4));

	}


}
void SpiOsdWinEnable_DMA(BYTE winno, BYTE en,BYTE bpp)
{
	BYTE index;

	index = SpiOsdWinBase[winno] + SPI_OSDWIN_ENABLE;
	
	WriteTW88Page(PAGE4_SOSD);//WriteTW88( 0xff, OSD_PAGE );
	if( en )
		WriteTW88(index, ReadTW88(index) | 0x01);
	else
		WriteTW88(index, ReadTW88(index) & 0xfe);


	switch(bpp)
	{
		case 4: WriteTW88(index, ReadTW88(index)&0x3f);break;
		case 6: WriteTW88(index, ReadTW88(index)|0x40);break;
		case 8: WriteTW88(index, ReadTW88(index)|0x80);break;
		default: WriteTW88(index, ReadTW88(index)|0x80);break;
	}

	
}

void	SpiOsdWinPixelAlpha_Nobuff( BYTE winno, BYTE alpha )
{
	BYTE index;

	index = SpiOsdWinBase[winno] + 0;
	if(alpha)
		WriteTW88( index, ReadTW88(index)|bv(5)|bv(4));
	else
		WriteTW88( index, ReadTW88(index)&~bv(5)&~bv(4));

}

void SpiOsdPixelAlphaAttr_Nobuff(BYTE winno, WORD lutloc)
{
	WriteTW88Page( PAGE4_SOSD );

	WriteTW88( 0x10, 0xc3 );    		// LUT Write Mode, En & byte ptr inc.

	if(lutloc >> 8)	
		WriteTW88( 0x10, ReadTW88(0x10) | 0x08);	//support 512 palette
	else
		WriteTW88( 0x10, ReadTW88(0x10) & 0xF7);



	if(winno==1||winno==2)
		WriteTW88( 0x10, ReadTW88(0x10)|0x04 ); 	   // LUT B
	else
		WriteTW88( 0x10, ReadTW88(0x10)&~0x04 );	   // LUTA

	
	WriteTW88( 0x11, (BYTE)lutloc ); // LUT addr
	WriteTW88( 0x12, 0x7f ); // LUT addr

}



/*
*************************************************************************
设计		liwei
文件		CarDemo
功能		显示一张800x480的图片
			用不压缩不用buff的方式
			主要功能是打开SPIOSD开关配置窗口属性，告诉
			窗口flash地址和加载图片色表

*************************************************************************
*/

void MovingGrid(BYTE WIN_NO, DWORD addr)
{
	
	WaitVBlank(1);

	SpiOsdWinBufferMem_Nobuff(WIN_NO, addr);
	
}

void ImageRevInit( BYTE SPI_WIN, WORD loc_x, WORD loc_y, WORD dx, WORD dy, DWORD addr)
{
	SpiOsdWinBufferMem_Nobuff( SPI_WIN, addr);
	SpiOsdWinBufferSizeXY( SPI_WIN, dx, dy );
	SpiOsdWinScreen_Nobuff( SPI_WIN, loc_x, loc_y, dx, dy);
	SpiOsdWinLutOffset_DMA(SPI_WIN,0);

	WaitVBlank(1);
	SpiOsdLoadLUT_AB(SPI_WIN,1, 0, 0x400, LogLut[0].loc);
	if(1 == SPI_WIN)
	{
		SpiOsdWin_Rle(SPI_WIN, 0x88, 0x88);
	}
	SpiOsdWinEnable_DMA(SPI_WIN,1,8);

	SpiOsdWinPixelAlpha_Nobuff(SPI_WIN, 1);
	SpiOsdPixelAlphaAttr_Nobuff(SPI_WIN, 0);

}

void ImageLvdsInit( BYTE SPI_WIN, WORD loc_x, WORD loc_y, WORD dx, WORD dy, DWORD addr)
{
	SpiOsdWinBufferMem_Nobuff( SPI_WIN, addr);
	SpiOsdWinBufferSizeXY( SPI_WIN, dx, dy );
	SpiOsdWinScreen_Nobuff( SPI_WIN, loc_x, loc_y, dx, dy);
	SpiOsdWinLutOffset_DMA(SPI_WIN,0);

	WaitVBlank(1);
	SpiOsdLoadLUT_AB(SPI_WIN,1, 0, 0x400, LogLut[0].loc);
	if(1 == SPI_WIN)
	{
		SpiOsdWin_Rle(SPI_WIN, 0x88, 0x88);
	}
//	SpiOsdWinEnable_DMA(SPI_WIN,1,8);


}


void InitLogoRev(void)
{
	SpiOsdEnable(ON);
	
	ImageRevInit(SOSD_WIN1, 35, 400, 820, 246, LogEpsLeft[0].loc);

	ImageRevInit(SOSD_WIN0, 350, 31, 192, 120, LogSolid[0].loc);
	ImageRevInit(SOSD_WIN3, 212, 760, 535, 167, LogSolid[1].loc);
	
	ImageRevInit(SOSD_WIN4, 301, 928, 329, 283, LogSolid[3].loc);
	ImageRevInit(SOSD_WIN5, 213, 1069, 88, 142, LogRadarFL[0].loc);
	ImageRevInit(SOSD_WIN6, 213, 928, 88, 141, LogRadarFR[0].loc);
	ImageRevInit(SOSD_WIN7, 630, 1069, 118, 141, LogRadarRL[0].loc);
	ImageRevInit(SOSD_WIN8, 630, 928, 118, 142, LogRadarRR[0].loc);
	
}

void InitLogoLvds(void)
{

	SpiOsdEnable(ON);
	
	ImageLvdsInit(SOSD_WIN4, 745, 500, 180, 201, LogSolid[4].loc);
	
	ImageLvdsInit(SOSD_WIN5, 745, 441, 90, 59, LogRadarXFL[0].loc);
	ImageLvdsInit(SOSD_WIN6, 835, 441, 90, 59, LogRadarXFR[0].loc);
	ImageLvdsInit(SOSD_WIN7, 745, 701, 90, 78, LogRadarXRL[0].loc);
	ImageLvdsInit(SOSD_WIN8, 835, 701, 90, 78, LogRadarXRR[0].loc);
	SpiOsdEnable(OFF);
	SpiOsdWinEnable_DMA(SOSD_WIN4,1,8);
	SpiOsdWinEnable_DMA(SOSD_WIN5,1,8);
	SpiOsdWinEnable_DMA(SOSD_WIN6,1,8);
	SpiOsdWinEnable_DMA(SOSD_WIN7,1,8);
	SpiOsdWinEnable_DMA(SOSD_WIN8,1,8);

}

/*
*************************************************************************
设计		liwei
文件		SpiOsdWinBufferMem
功能		告诉窗口图片在flash里的起始地址

注意		这跟TW8825有差异:TW8836把地址的低4位移到了
			每组的4*F寄存器
*************************************************************************
*/	

static void SpiOsdWinBufferMem_Nobuff(BYTE winno, DWORD start)
{
	BYTE index;
	
	index = SpiOsdWinBase[winno] + SPI_OSDWIN_BUFFERSTART;

	WriteTW88Page(PAGE4_SOSD);

	WriteTW88(index++, (BYTE)(start>>20));
	WriteTW88(index++, (BYTE)(start>>12));
	WriteTW88(index, (BYTE)(start>>4));
	if(winno)
	{
		index=index+6;
		WriteTW88(index, (BYTE)(start>>0));
	}
	else
	{
		index=index+6;
		WriteTW88(index, 0x01);
	}

}

void InitSystem(enumLogoImageStatus eImageStatus)
{
	eLogoImageStatus = eImageStatus;
	if(!isAccessOpen)
		//do nothing.
		return;

	//---------------
	//Init SpiOSD data structire.
	SOsd_init();

	//
	//draw Logo
	//
	
	SOsd_CleanReg();
	SOsd_CleanLut();
	SOsd_CleanRlc();
	SOsd_UpdateReg(SOSD_WIN0, SOSD_WIN8);
//	SOsd_UpdateRlc();
	switch(eLogoImageStatus)
	{
		default:
		case LOGO_IMG_NONE:
			break;
		case LOGO_IMG_REV:
			vTwOsdStateSet(OSD_STATE_WORK);
			InitLogoRev();
			break;
		case LOGO_IMG_LVDS:
			InitLogoLvds();			
			break;
	}
	return;
}

/* \brief
*/
void vTwOsdInit(void)
{
	wOsdDelayTimer = 0;
	eOsdState = OSD_STATE_IDLE;
	eLogoImageStatus = LOGO_IMG_NONE;
}


/* \brief
*/
void vTwOsdDelay(void)
{
	if(0 != wOsdDelayTimer)
	{
		--wOsdDelayTimer;
	}
	
}

/* \brief
*/
void vTwOsdTask(void)
{
	switch(eOsdState)
	{
		case OSD_STATE_IDLE:
			eOsdState = OSD_STATE_WORK;
			break;
		case OSD_STATE_INIT:
			break;
		case OSD_STATE_WORK:
			break;
		case OSD_STATE_ON:
			break;
		case OSD_STATE_OFF:
			break;
	}
}


/* \brief
*/
void ShowLogEpsLeft( BYTE number)
{
	if(number > sizeof_LogEps)
	{
		number = sizeof_LogEps;
	}
	MovingGrid(SOSD_WIN1, LogEpsLeft[number].loc);
}

/* \brief
*/
void ShowLogEpsRight( BYTE number)
{
	if(number > sizeof_LogEps)
	{
		number = sizeof_LogEps;
	}
	MovingGrid(SOSD_WIN1, LogEpsRight[number].loc);
}

/* \brief
*/
void ShowLogPas(BYTE* pData)
{
	BYTE i = 0;

	switch(eLogoImageStatus)
	{
		default:
		case LOGO_IMG_NONE:
			break;
		case LOGO_IMG_REV:
			MovingGrid(SOSD_WIN5, LogRadarFL[pData[0]].loc);
			MovingGrid(SOSD_WIN6, LogRadarFR[pData[1]].loc);
			MovingGrid(SOSD_WIN7, LogRadarRL[pData[2]* 5 + pData[3]].loc);
			MovingGrid(SOSD_WIN8, LogRadarRR[pData[4]* 5 + pData[5]].loc);
			break;
		case LOGO_IMG_LVDS:
			if(!isSystemInitCompleted)
			{
				return;
			}
			for(i = 0; i < osd_RADAR_MAX_LENGTH; i++)
			{
				if(0 == pData[i])
				{
					if((osd_RADAR_MAX_LENGTH - 1) == i)
					{
						if(OSD_STATE_OFF != eOsdState)
						{
							SpiOsdEnable(OFF);
//							vTimerSetRevTimer(20);
							eOsdState = OSD_STATE_OFF;
						}
						return;
					}
				}
				else
				{
					break;
				}
			}
			if(OSD_STATE_ON != eOsdState)
			{
				SpiOsdEnable(ON);
//				vTimerSetRevTimer(60);
				eOsdState = OSD_STATE_ON;
			}
			MovingGrid(SOSD_WIN5, LogRadarXFL[pData[0]].loc);
			MovingGrid(SOSD_WIN6, LogRadarXFR[pData[1]].loc);
			MovingGrid(SOSD_WIN7, LogRadarXRL[pData[2]* 5 + pData[3]].loc);
			MovingGrid(SOSD_WIN8, LogRadarXRR[pData[4]* 5 + pData[5]].loc);	
			break;
	}
	
}


/* \brief
*/
void vTwOsdStateSet(enumOsdState exOsdState)
{
	eOsdState = exOsdState;
}


