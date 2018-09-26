/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		type.h
**	Abstract:		
**	Creat By:		Zeno Liu
**	Creat Time:		2015.03.06
**	Version:		v1.0
**
******************************************************************************/

#ifndef __TYPE_H__
#define __TYPE_H__

/* \brief
	
*/
#define CODE_P

//#define CODE_P		const
typedef	unsigned char				UINT08,U8,BOOL,BYTE;
typedef signed char				INT08,SBYTE;
typedef unsigned short				UINT16,U16,WORD;
typedef signed short				INT16,SWORD;
typedef unsigned long				UINT32,U32,DWORD; 
typedef signed long				INT32,LONG,S32,SDWORD;
typedef unsigned int				UINT;
typedef signed int					INT;


typedef void (*Func) (void);

/* \brief
	
*/
#ifndef	TRUE
	#define	TRUE					1
#endif	//TRUE

#ifndef	FALSE
	#define	FALSE					0
#endif	//FALSE

#ifndef	ON
	#define	ON					1
#endif	//TRUE

#ifndef	OFF
	#define	OFF					0
#endif	


#ifndef	NULL
	#define NULL					((void*)0)
#endif

typedef struct image_info_s
{
    BYTE  lut_type;     /*  1: luts, byte pointer method ; 0: lut , address method  */
    BYTE  rle;          /*  upper : bpp ; bottom : rle counter  */
    WORD  dx, dy;       /*  width , height  */
} image_info_t;


typedef struct image_item_info_s
{
//    BYTE  type;         /*  0: LUT + Image , 1: Header + LUT + Image , 2: Image Only , 3: LUT Only , 4: Header Only  */
	DWORD loc;          /*  Absolute location on SpiFlash  */
	DWORD size;
//    image_info_t *info;
//    BYTE  alpha;        /*  Alpha Index . 0xFF : ignore.  */
} image_item_info_t;

struct RLE2_HEADER{
    BYTE  id[2];        /*  IT : Intersil Techwell  */

    BYTE  op_bmpbits;
    BYTE  rledata_cnt;

    WORD  w;
    WORD  h;

    DWORD size;

    BYTE  LUT_format;   /*  Bit0- 0:RGBa, 1:BGRa, bit1- 0:LUTS, 1:LUT;  */
    BYTE  LUT_colors;   /*  0xff:256 colors,  0x7F:128 colors,...  */

    BYTE  dummy0;
    BYTE  dummy1;
};

typedef struct menu_image_header_s {
	DWORD lut_loc;
	DWORD image_loc;

	BYTE lut_type;	//
	BYTE bpp;
	BYTE rle;		//upper:bpp bottom:rle_counter.
	WORD dx,dy;		//width, height
	WORD lut_size;	//we don't need it. we can assume LUT size & image size.
} menu_image_header_t;

struct SOsdLut_s {
	BYTE type;		//use flag (0x80) + LUTTYPE (BYTE:1, ADDR 0)	
	WORD offset;	//lut offset. if LUTTYPE_BYTE, MSB:(LutOffset >> 6),LSB:(LutOffset << 2)
	WORD size;		//lut size
	DWORD addr;		//address on SpiFlash.
	BYTE alpha;		//alpha_index. 0~0xFE. 0xFF measn the no-alpha.
};

struct SOsdRle_s {
	BYTE win;
	BYTE bpp;
	BYTE count;
};

#endif
