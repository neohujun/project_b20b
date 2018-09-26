/* Based on CPU DB MC9S08DZ60_64, version 3.00.021 (RegistersPrg V2.28) */
/*
** ###################################################################
**     Filename  : mc9s08dz60.h
**     Processor : MC9S08DZ60MLH
**     FileFormat: V2.28
**     DataSheet : MC9S08DZ60 Rev. 3 10/2007
**     Compiler  : CodeWarrior compiler
**     Date/Time : 27.5.2009, 10:26
**     Abstract  :
**         This header implements the mapping of I/O devices.
**
**     Copyright : 1997 - 2009 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
**
**     CPU Registers Revisions:
**      - 21.04.2006, V2.87.015:
**              - Removed bits MCGSC_IREFST, MCGSC_PLLST, FCNFG_ECCDIS, SDIDH_REV0..SDIDH_REV3.
**              - Added registers NVFTRIM, NVMCGTRM. Removed register NVECC.
**              - Renamed registers ADx ==> ADCx.
**              -   REASON: Changes in the data sheet (from rev 0.05 8/5/2005 to rev 1.0 Draft A 04/2006)
**      - 16.05.2006, V2.87.023:
**              - Renamed registers ADCVH ==> ADCCVH, ADCVL ==> ADCCVL, ADCV ==> ADCCV, ADCFG ==> ADCCFG,
**              - IICC1 ==> IICC, Added bit CANCTL1_BORM.
**              -   REASON: Changes in the data sheet (from rev 1.0 Draft A to rev 1.0 Draft B)
**      - 08.09.2006, V2.87.027:
**              - Added registers CANxIDR2, CANxIDR3, corrected CANxIDR0, CANxIDR1 bits (Standard ID ==> Extended ID).
**              -   REASON: Bug-fix (#3723 in Issue Manager)
**      - 28.06.2007, V2.87.128:
**              - Removed register FTSTMOD.
**              -   REASON: Bug-fix (#4466 in Issue Manager)
**      - 20.12.2007, V2.87.141:
**              - Added bits to the ADC registers to use the ADC 12-bit module.
**              -   REASON: Changes in the data sheet (from rev 1 6/2006 to rev 3 10/2007).
**      - 28.03.2008, V3.00.0:
**              - Added registers IICC, DBGCA, DBGCB.
**              -   REASON: Bug-fix (#5795 in Issue Manager)
**      - 22.08.2008, V3.00.1:
**              - Added definition of the 32-bit registers CANTIDR(0x000018B0), CANRIDR(0x000018A0).
**              -   REASON: Bug-fix (#6328 in Issue Manager). 
**
**     File-Format-Revisions:
**      - 08.03.2006, V2.04 :
**               - Support for bit(s) names duplicated with any register name in .h header files
**      - 24.03.2006, V2.05 :
**               - Changes have not affected this file (because they are related to another family)
**      - 26.04.2006, V2.06 :
**               - Absolute assembly supported (depreciated symbols are not defined)
**      - 27.04.2006, V2.07 :
**               - Fixed macro __RESET_WATCHDOG for HCS12, HCS12X ,HCS08 DZ and HCS08 EN derivatives (write 0x55,0xAA).
**      - 07.06.2006, V2.08 :
**               - For .inc files added constants "RAMStart" and "RAMEnd" even there is only Z_RAM.
**      - 03.07.2006, V2.09 :
**               - Flash commands constants supported
**      - 27.10.2006, V2.10 :
**               - __RESET_WATCHDOG improved formating and re-definition
**      - 23.11.2006, V2.11 :
**               - Changes have not affected this file (because they are related to another family)
**      - 22.01.2007, V2.12 :
**               - Changes have not affected this file (because they are related to another family)
**      - 01.03.2007, V2.13 :
**               - Flash commands constants values converted to HEX format
**      - 02.03.2007, V2.14 :
**               - Interrupt vector numbers added into .H, see VectorNumber_*
**      - 26.03.2007, V2.15 :
**               - Changes have not affected this file (because they are related to another family)
**      - 10.05.2007, V2.16 :
**               - Fixed flash commands definition for ColdFireV1 assembler (equ -> .equ)
**      - 05.06.2007, V2.17 :
**               - Changes have not affected this file (because they are related to another family)
**      - 19.07.2007, V2.18 :
**               - Improved number of blanked lines inside register structures
**      - 06.08.2007, V2.19 :
**               - CPUDB revisions generated ahead of the file-format revisions.
**      - 11.09.2007, V2.20 :
**               - Added comment about initialization of unbonded pins.
**      - 02.01.2008, V2.21 :
**               - Changes have not affected this file (because they are related to another family)
**      - 13.02.2008, V2.22 :
**               - Changes have not affected this file (because they are related to another family)
**      - 20.02.2008, V2.23 :
**               - Changes have not affected this file (because they are related to another family)
**      - 03.07.2008, V2.24 :
**               - Added support for bits with name starting with number (like "1HZ")
**      - 28.11.2008, V2.25 :
**               - StandBy RAM array declaration for ANSI-C added
**      - 1.12.2008, V2.26 :
**               - Duplication of bit (or bit-group) name with register name is not marked as a problem, is register is internal only and it is not displayed in I/O map.
**      - 17.3.2009, V2.27 :
**               - Merged bit-group is not generated, if the name matchs with anouther bit name in the register
**      - 6.4.2009, V2.28 :
**               - Fixed generation of merged bits for bit-groups with a digit at the end, if group-name is defined in CPUDB
**
**     Not all general-purpose I/O pins are available on all packages or on all mask sets of a specific
**     derivative device. To avoid extra current drain from floating input pins, the user’s reset
**     initialization routine in the application program must either enable on-chip pull-up devices
**     or change the direction of unconnected pins to outputs so the pins do not float.
** ###################################################################
*/

#ifndef _MC9S08DZ60_H
#define _MC9S08DZ60_H

/* Types definition */
typedef unsigned char byte;
typedef unsigned int word;
typedef unsigned long dword;
typedef unsigned long dlong[2];

/* Watchdog reset macro */
#ifndef __RESET_WATCHDOG
#ifdef _lint
  #define __RESET_WATCHDOG()  /* empty */
#else
  #define __RESET_WATCHDOG() (void)(SRS = 0x55, SRS = 0xAA)
#endif
#endif /* __RESET_WATCHDOG */

#define REG_BASE 0x0000                /* Base address for the I/O register block */


#pragma MESSAGE DISABLE C1106 /* WARNING C1106: Non-standard bitfield type */

/**************** interrupt vector numbers ****************/
#define VectorNumber_Vacmp2             31
#define VectorNumber_Vacmp1             30
#define VectorNumber_Vcantx             29
#define VectorNumber_Vcanrx             28
#define VectorNumber_Vcanerr            27
#define VectorNumber_Vcanwu             26
#define VectorNumber_Vrtc               25
#define VectorNumber_Viic               24
#define VectorNumber_Vadc               23
#define VectorNumber_Vport              22
#define VectorNumber_Vsci2tx            21
#define VectorNumber_Vsci2rx            20
#define VectorNumber_Vsci2err           19
#define VectorNumber_Vsci1tx            18
#define VectorNumber_Vsci1rx            17
#define VectorNumber_Vsci1err           16
#define VectorNumber_Vspi               15
#define VectorNumber_Vtpm2ovf           14
#define VectorNumber_Vtpm2ch1           13
#define VectorNumber_Vtpm2ch0           12
#define VectorNumber_Vtpm1ovf           11
#define VectorNumber_Vtpm1ch5           10
#define VectorNumber_Vtpm1ch4           9
#define VectorNumber_Vtpm1ch3           8
#define VectorNumber_Vtpm1ch2           7
#define VectorNumber_Vtpm1ch1           6
#define VectorNumber_Vtpm1ch0           5
#define VectorNumber_Vlol               4
#define VectorNumber_Vlvd               3
#define VectorNumber_Virq               2
#define VectorNumber_Vswi               1
#define VectorNumber_Vreset             0

/**************** interrupt vector table ****************/
#define Vacmp2                          0x0000FFC0
#define Vacmp1                          0x0000FFC2
#define Vcantx                          0x0000FFC4
#define Vcanrx                          0x0000FFC6
#define Vcanerr                         0x0000FFC8
#define Vcanwu                          0x0000FFCA
#define Vrtc                            0x0000FFCC
#define Viic                            0x0000FFCE
#define Vadc                            0x0000FFD0
#define Vport                           0x0000FFD2
#define Vsci2tx                         0x0000FFD4
#define Vsci2rx                         0x0000FFD6
#define Vsci2err                        0x0000FFD8
#define Vsci1tx                         0x0000FFDA
#define Vsci1rx                         0x0000FFDC
#define Vsci1err                        0x0000FFDE
#define Vspi                            0x0000FFE0
#define Vtpm2ovf                        0x0000FFE2
#define Vtpm2ch1                        0x0000FFE4
#define Vtpm2ch0                        0x0000FFE6
#define Vtpm1ovf                        0x0000FFE8
#define Vtpm1ch5                        0x0000FFEA
#define Vtpm1ch4                        0x0000FFEC
#define Vtpm1ch3                        0x0000FFEE
#define Vtpm1ch2                        0x0000FFF0
#define Vtpm1ch1                        0x0000FFF2
#define Vtpm1ch0                        0x0000FFF4
#define Vlol                            0x0000FFF6
#define Vlvd                            0x0000FFF8
#define Virq                            0x0000FFFA
#define Vswi                            0x0000FFFC
#define Vreset                          0x0000FFFE

/**************** registers I/O map ****************/

/*** PTAD - Port A Data Register; 0x00000000 ***/
typedef union {
  byte Byte;
  struct {
    byte PTAD0       :1;                                       /* Port A Data Register Bit 0 */
    byte PTAD1       :1;                                       /* Port A Data Register Bit 1 */
    byte PTAD2       :1;                                       /* Port A Data Register Bit 2 */
    byte PTAD3       :1;                                       /* Port A Data Register Bit 3 */
    byte PTAD4       :1;                                       /* Port A Data Register Bit 4 */
    byte PTAD5       :1;                                       /* Port A Data Register Bit 5 */
    byte PTAD6       :1;                                       /* Port A Data Register Bit 6 */
    byte PTAD7       :1;                                       /* Port A Data Register Bit 7 */
  } Bits;
} PTADSTR;
extern volatile PTADSTR _PTAD @0x00000000;
#define PTAD                            _PTAD.Byte
#define PTAD_PTAD0                      _PTAD.Bits.PTAD0
#define PTAD_PTAD1                      _PTAD.Bits.PTAD1
#define PTAD_PTAD2                      _PTAD.Bits.PTAD2
#define PTAD_PTAD3                      _PTAD.Bits.PTAD3
#define PTAD_PTAD4                      _PTAD.Bits.PTAD4
#define PTAD_PTAD5                      _PTAD.Bits.PTAD5
#define PTAD_PTAD6                      _PTAD.Bits.PTAD6
#define PTAD_PTAD7                      _PTAD.Bits.PTAD7

#define PTAD_PTAD0_MASK                 1
#define PTAD_PTAD1_MASK                 2
#define PTAD_PTAD2_MASK                 4
#define PTAD_PTAD3_MASK                 8
#define PTAD_PTAD4_MASK                 16
#define PTAD_PTAD5_MASK                 32
#define PTAD_PTAD6_MASK                 64
#define PTAD_PTAD7_MASK                 128


/*** PTADD - Port A Data Direction Register; 0x00000001 ***/
typedef union {
  byte Byte;
  struct {
    byte PTADD0      :1;                                       /* Data Direction for Port A Bit 0 */
    byte PTADD1      :1;                                       /* Data Direction for Port A Bit 1 */
    byte PTADD2      :1;                                       /* Data Direction for Port A Bit 2 */
    byte PTADD3      :1;                                       /* Data Direction for Port A Bit 3 */
    byte PTADD4      :1;                                       /* Data Direction for Port A Bit 4 */
    byte PTADD5      :1;                                       /* Data Direction for Port A Bit 5 */
    byte PTADD6      :1;                                       /* Data Direction for Port A Bit 6 */
    byte PTADD7      :1;                                       /* Data Direction for Port A Bit 7 */
  } Bits;
} PTADDSTR;
extern volatile PTADDSTR _PTADD @0x00000001;
#define PTADD                           _PTADD.Byte
#define PTADD_PTADD0                    _PTADD.Bits.PTADD0
#define PTADD_PTADD1                    _PTADD.Bits.PTADD1
#define PTADD_PTADD2                    _PTADD.Bits.PTADD2
#define PTADD_PTADD3                    _PTADD.Bits.PTADD3
#define PTADD_PTADD4                    _PTADD.Bits.PTADD4
#define PTADD_PTADD5                    _PTADD.Bits.PTADD5
#define PTADD_PTADD6                    _PTADD.Bits.PTADD6
#define PTADD_PTADD7                    _PTADD.Bits.PTADD7

#define PTADD_PTADD0_MASK               1
#define PTADD_PTADD1_MASK               2
#define PTADD_PTADD2_MASK               4
#define PTADD_PTADD3_MASK               8
#define PTADD_PTADD4_MASK               16
#define PTADD_PTADD5_MASK               32
#define PTADD_PTADD6_MASK               64
#define PTADD_PTADD7_MASK               128


/*** PTBD - Port B Data Register; 0x00000002 ***/
typedef union {
  byte Byte;
  struct {
    byte PTBD0       :1;                                       /* Port B Data Register Bit 0 */
    byte PTBD1       :1;                                       /* Port B Data Register Bit 1 */
    byte PTBD2       :1;                                       /* Port B Data Register Bit 2 */
    byte PTBD3       :1;                                       /* Port B Data Register Bit 3 */
    byte PTBD4       :1;                                       /* Port B Data Register Bit 4 */
    byte PTBD5       :1;                                       /* Port B Data Register Bit 5 */
    byte PTBD6       :1;                                       /* Port B Data Register Bit 6 */
    byte PTBD7       :1;                                       /* Port B Data Register Bit 7 */
  } Bits;
} PTBDSTR;
extern volatile PTBDSTR _PTBD @0x00000002;
#define PTBD                            _PTBD.Byte
#define PTBD_PTBD0                      _PTBD.Bits.PTBD0
#define PTBD_PTBD1                      _PTBD.Bits.PTBD1
#define PTBD_PTBD2                      _PTBD.Bits.PTBD2
#define PTBD_PTBD3                      _PTBD.Bits.PTBD3
#define PTBD_PTBD4                      _PTBD.Bits.PTBD4
#define PTBD_PTBD5                      _PTBD.Bits.PTBD5
#define PTBD_PTBD6                      _PTBD.Bits.PTBD6
#define PTBD_PTBD7                      _PTBD.Bits.PTBD7

#define PTBD_PTBD0_MASK                 1
#define PTBD_PTBD1_MASK                 2
#define PTBD_PTBD2_MASK                 4
#define PTBD_PTBD3_MASK                 8
#define PTBD_PTBD4_MASK                 16
#define PTBD_PTBD5_MASK                 32
#define PTBD_PTBD6_MASK                 64
#define PTBD_PTBD7_MASK                 128


/*** PTBDD - Port B Data Direction Register; 0x00000003 ***/
typedef union {
  byte Byte;
  struct {
    byte PTBDD0      :1;                                       /* Data Direction for Port B Bit 0 */
    byte PTBDD1      :1;                                       /* Data Direction for Port B Bit 1 */
    byte PTBDD2      :1;                                       /* Data Direction for Port B Bit 2 */
    byte PTBDD3      :1;                                       /* Data Direction for Port B Bit 3 */
    byte PTBDD4      :1;                                       /* Data Direction for Port B Bit 4 */
    byte PTBDD5      :1;                                       /* Data Direction for Port B Bit 5 */
    byte PTBDD6      :1;                                       /* Data Direction for Port B Bit 6 */
    byte PTBDD7      :1;                                       /* Data Direction for Port B Bit 7 */
  } Bits;
} PTBDDSTR;
extern volatile PTBDDSTR _PTBDD @0x00000003;
#define PTBDD                           _PTBDD.Byte
#define PTBDD_PTBDD0                    _PTBDD.Bits.PTBDD0
#define PTBDD_PTBDD1                    _PTBDD.Bits.PTBDD1
#define PTBDD_PTBDD2                    _PTBDD.Bits.PTBDD2
#define PTBDD_PTBDD3                    _PTBDD.Bits.PTBDD3
#define PTBDD_PTBDD4                    _PTBDD.Bits.PTBDD4
#define PTBDD_PTBDD5                    _PTBDD.Bits.PTBDD5
#define PTBDD_PTBDD6                    _PTBDD.Bits.PTBDD6
#define PTBDD_PTBDD7                    _PTBDD.Bits.PTBDD7

#define PTBDD_PTBDD0_MASK               1
#define PTBDD_PTBDD1_MASK               2
#define PTBDD_PTBDD2_MASK               4
#define PTBDD_PTBDD3_MASK               8
#define PTBDD_PTBDD4_MASK               16
#define PTBDD_PTBDD5_MASK               32
#define PTBDD_PTBDD6_MASK               64
#define PTBDD_PTBDD7_MASK               128


/*** PTCD - Port C Data Register; 0x00000004 ***/
typedef union {
  byte Byte;
  struct {
    byte PTCD0       :1;                                       /* Port C Data Register Bit 0 */
    byte PTCD1       :1;                                       /* Port C Data Register Bit 1 */
    byte PTCD2       :1;                                       /* Port C Data Register Bit 2 */
    byte PTCD3       :1;                                       /* Port C Data Register Bit 3 */
    byte PTCD4       :1;                                       /* Port C Data Register Bit 4 */
    byte PTCD5       :1;                                       /* Port C Data Register Bit 5 */
    byte PTCD6       :1;                                       /* Port C Data Register Bit 6 */
    byte PTCD7       :1;                                       /* Port C Data Register Bit 7 */
  } Bits;
} PTCDSTR;
extern volatile PTCDSTR _PTCD @0x00000004;
#define PTCD                            _PTCD.Byte
#define PTCD_PTCD0                      _PTCD.Bits.PTCD0
#define PTCD_PTCD1                      _PTCD.Bits.PTCD1
#define PTCD_PTCD2                      _PTCD.Bits.PTCD2
#define PTCD_PTCD3                      _PTCD.Bits.PTCD3
#define PTCD_PTCD4                      _PTCD.Bits.PTCD4
#define PTCD_PTCD5                      _PTCD.Bits.PTCD5
#define PTCD_PTCD6                      _PTCD.Bits.PTCD6
#define PTCD_PTCD7                      _PTCD.Bits.PTCD7

#define PTCD_PTCD0_MASK                 1
#define PTCD_PTCD1_MASK                 2
#define PTCD_PTCD2_MASK                 4
#define PTCD_PTCD3_MASK                 8
#define PTCD_PTCD4_MASK                 16
#define PTCD_PTCD5_MASK                 32
#define PTCD_PTCD6_MASK                 64
#define PTCD_PTCD7_MASK                 128


/*** PTCDD - Port C Data Direction Register; 0x00000005 ***/
typedef union {
  byte Byte;
  struct {
    byte PTCDD0      :1;                                       /* Data Direction for Port C Bit 0 */
    byte PTCDD1      :1;                                       /* Data Direction for Port C Bit 1 */
    byte PTCDD2      :1;                                       /* Data Direction for Port C Bit 2 */
    byte PTCDD3      :1;                                       /* Data Direction for Port C Bit 3 */
    byte PTCDD4      :1;                                       /* Data Direction for Port C Bit 4 */
    byte PTCDD5      :1;                                       /* Data Direction for Port C Bit 5 */
    byte PTCDD6      :1;                                       /* Data Direction for Port C Bit 6 */
    byte PTCDD7      :1;                                       /* Data Direction for Port C Bit 7 */
  } Bits;
} PTCDDSTR;
extern volatile PTCDDSTR _PTCDD @0x00000005;
#define PTCDD                           _PTCDD.Byte
#define PTCDD_PTCDD0                    _PTCDD.Bits.PTCDD0
#define PTCDD_PTCDD1                    _PTCDD.Bits.PTCDD1
#define PTCDD_PTCDD2                    _PTCDD.Bits.PTCDD2
#define PTCDD_PTCDD3                    _PTCDD.Bits.PTCDD3
#define PTCDD_PTCDD4                    _PTCDD.Bits.PTCDD4
#define PTCDD_PTCDD5                    _PTCDD.Bits.PTCDD5
#define PTCDD_PTCDD6                    _PTCDD.Bits.PTCDD6
#define PTCDD_PTCDD7                    _PTCDD.Bits.PTCDD7

#define PTCDD_PTCDD0_MASK               1
#define PTCDD_PTCDD1_MASK               2
#define PTCDD_PTCDD2_MASK               4
#define PTCDD_PTCDD3_MASK               8
#define PTCDD_PTCDD4_MASK               16
#define PTCDD_PTCDD5_MASK               32
#define PTCDD_PTCDD6_MASK               64
#define PTCDD_PTCDD7_MASK               128


/*** PTDD - Port D Data Register; 0x00000006 ***/
typedef union {
  byte Byte;
  struct {
    byte PTDD0       :1;                                       /* Port D Data Register Bit 0 */
    byte PTDD1       :1;                                       /* Port D Data Register Bit 1 */
    byte PTDD2       :1;                                       /* Port D Data Register Bit 2 */
    byte PTDD3       :1;                                       /* Port D Data Register Bit 3 */
    byte PTDD4       :1;                                       /* Port D Data Register Bit 4 */
    byte PTDD5       :1;                                       /* Port D Data Register Bit 5 */
    byte PTDD6       :1;                                       /* Port D Data Register Bit 6 */
    byte PTDD7       :1;                                       /* Port D Data Register Bit 7 */
  } Bits;
} PTDDSTR;
extern volatile PTDDSTR _PTDD @0x00000006;
#define PTDD                            _PTDD.Byte
#define PTDD_PTDD0                      _PTDD.Bits.PTDD0
#define PTDD_PTDD1                      _PTDD.Bits.PTDD1
#define PTDD_PTDD2                      _PTDD.Bits.PTDD2
#define PTDD_PTDD3                      _PTDD.Bits.PTDD3
#define PTDD_PTDD4                      _PTDD.Bits.PTDD4
#define PTDD_PTDD5                      _PTDD.Bits.PTDD5
#define PTDD_PTDD6                      _PTDD.Bits.PTDD6
#define PTDD_PTDD7                      _PTDD.Bits.PTDD7

#define PTDD_PTDD0_MASK                 1
#define PTDD_PTDD1_MASK                 2
#define PTDD_PTDD2_MASK                 4
#define PTDD_PTDD3_MASK                 8
#define PTDD_PTDD4_MASK                 16
#define PTDD_PTDD5_MASK                 32
#define PTDD_PTDD6_MASK                 64
#define PTDD_PTDD7_MASK                 128


/*** PTDDD - Port D Data Direction Register; 0x00000007 ***/
typedef union {
  byte Byte;
  struct {
    byte PTDDD0      :1;                                       /* Data Direction for Port D Bit 0 */
    byte PTDDD1      :1;                                       /* Data Direction for Port D Bit 1 */
    byte PTDDD2      :1;                                       /* Data Direction for Port D Bit 2 */
    byte PTDDD3      :1;                                       /* Data Direction for Port D Bit 3 */
    byte PTDDD4      :1;                                       /* Data Direction for Port D Bit 4 */
    byte PTDDD5      :1;                                       /* Data Direction for Port D Bit 5 */
    byte PTDDD6      :1;                                       /* Data Direction for Port D Bit 6 */
    byte PTDDD7      :1;                                       /* Data Direction for Port D Bit 7 */
  } Bits;
} PTDDDSTR;
extern volatile PTDDDSTR _PTDDD @0x00000007;
#define PTDDD                           _PTDDD.Byte
#define PTDDD_PTDDD0                    _PTDDD.Bits.PTDDD0
#define PTDDD_PTDDD1                    _PTDDD.Bits.PTDDD1
#define PTDDD_PTDDD2                    _PTDDD.Bits.PTDDD2
#define PTDDD_PTDDD3                    _PTDDD.Bits.PTDDD3
#define PTDDD_PTDDD4                    _PTDDD.Bits.PTDDD4
#define PTDDD_PTDDD5                    _PTDDD.Bits.PTDDD5
#define PTDDD_PTDDD6                    _PTDDD.Bits.PTDDD6
#define PTDDD_PTDDD7                    _PTDDD.Bits.PTDDD7

#define PTDDD_PTDDD0_MASK               1
#define PTDDD_PTDDD1_MASK               2
#define PTDDD_PTDDD2_MASK               4
#define PTDDD_PTDDD3_MASK               8
#define PTDDD_PTDDD4_MASK               16
#define PTDDD_PTDDD5_MASK               32
#define PTDDD_PTDDD6_MASK               64
#define PTDDD_PTDDD7_MASK               128


/*** PTED - Port E Data Register; 0x00000008 ***/
typedef union {
  byte Byte;
  struct {
    byte PTED0       :1;                                       /* Port E Data Register Bit 0 */
    byte PTED1       :1;                                       /* Port E Data Register Bit 1 */
    byte PTED2       :1;                                       /* Port E Data Register Bit 2 */
    byte PTED3       :1;                                       /* Port E Data Register Bit 3 */
    byte PTED4       :1;                                       /* Port E Data Register Bit 4 */
    byte PTED5       :1;                                       /* Port E Data Register Bit 5 */
    byte PTED6       :1;                                       /* Port E Data Register Bit 6 */
    byte PTED7       :1;                                       /* Port E Data Register Bit 7 */
  } Bits;
} PTEDSTR;
extern volatile PTEDSTR _PTED @0x00000008;
#define PTED                            _PTED.Byte
#define PTED_PTED0                      _PTED.Bits.PTED0
#define PTED_PTED1                      _PTED.Bits.PTED1
#define PTED_PTED2                      _PTED.Bits.PTED2
#define PTED_PTED3                      _PTED.Bits.PTED3
#define PTED_PTED4                      _PTED.Bits.PTED4
#define PTED_PTED5                      _PTED.Bits.PTED5
#define PTED_PTED6                      _PTED.Bits.PTED6
#define PTED_PTED7                      _PTED.Bits.PTED7

#define PTED_PTED0_MASK                 1
#define PTED_PTED1_MASK                 2
#define PTED_PTED2_MASK                 4
#define PTED_PTED3_MASK                 8
#define PTED_PTED4_MASK                 16
#define PTED_PTED5_MASK                 32
#define PTED_PTED6_MASK                 64
#define PTED_PTED7_MASK                 128


/*** PTEDD - Port E Data Direction Register; 0x00000009 ***/
typedef union {
  byte Byte;
  struct {
    byte PTEDD0      :1;                                       /* Data Direction for Port E Bit 0 */
    byte PTEDD1      :1;                                       /* Data Direction for Port E Bit 1 */
    byte PTEDD2      :1;                                       /* Data Direction for Port E Bit 2 */
    byte PTEDD3      :1;                                       /* Data Direction for Port E Bit 3 */
    byte PTEDD4      :1;                                       /* Data Direction for Port E Bit 4 */
    byte PTEDD5      :1;                                       /* Data Direction for Port E Bit 5 */
    byte PTEDD6      :1;                                       /* Data Direction for Port E Bit 6 */
    byte PTEDD7      :1;                                       /* Data Direction for Port E Bit 7 */
  } Bits;
} PTEDDSTR;
extern volatile PTEDDSTR _PTEDD @0x00000009;
#define PTEDD                           _PTEDD.Byte
#define PTEDD_PTEDD0                    _PTEDD.Bits.PTEDD0
#define PTEDD_PTEDD1                    _PTEDD.Bits.PTEDD1
#define PTEDD_PTEDD2                    _PTEDD.Bits.PTEDD2
#define PTEDD_PTEDD3                    _PTEDD.Bits.PTEDD3
#define PTEDD_PTEDD4                    _PTEDD.Bits.PTEDD4
#define PTEDD_PTEDD5                    _PTEDD.Bits.PTEDD5
#define PTEDD_PTEDD6                    _PTEDD.Bits.PTEDD6
#define PTEDD_PTEDD7                    _PTEDD.Bits.PTEDD7

#define PTEDD_PTEDD0_MASK               1
#define PTEDD_PTEDD1_MASK               2
#define PTEDD_PTEDD2_MASK               4
#define PTEDD_PTEDD3_MASK               8
#define PTEDD_PTEDD4_MASK               16
#define PTEDD_PTEDD5_MASK               32
#define PTEDD_PTEDD6_MASK               64
#define PTEDD_PTEDD7_MASK               128


/*** PTFD - Port F Data Register; 0x0000000A ***/
typedef union {
  byte Byte;
  struct {
    byte PTFD0       :1;                                       /* Port F Data Register Bit 0 */
    byte PTFD1       :1;                                       /* Port F Data Register Bit 1 */
    byte PTFD2       :1;                                       /* Port F Data Register Bit 2 */
    byte PTFD3       :1;                                       /* Port F Data Register Bit 3 */
    byte PTFD4       :1;                                       /* Port F Data Register Bit 4 */
    byte PTFD5       :1;                                       /* Port F Data Register Bit 5 */
    byte PTFD6       :1;                                       /* Port F Data Register Bit 6 */
    byte PTFD7       :1;                                       /* Port F Data Register Bit 7 */
  } Bits;
} PTFDSTR;
extern volatile PTFDSTR _PTFD @0x0000000A;
#define PTFD                            _PTFD.Byte
#define PTFD_PTFD0                      _PTFD.Bits.PTFD0
#define PTFD_PTFD1                      _PTFD.Bits.PTFD1
#define PTFD_PTFD2                      _PTFD.Bits.PTFD2
#define PTFD_PTFD3                      _PTFD.Bits.PTFD3
#define PTFD_PTFD4                      _PTFD.Bits.PTFD4
#define PTFD_PTFD5                      _PTFD.Bits.PTFD5
#define PTFD_PTFD6                      _PTFD.Bits.PTFD6
#define PTFD_PTFD7                      _PTFD.Bits.PTFD7

#define PTFD_PTFD0_MASK                 1
#define PTFD_PTFD1_MASK                 2
#define PTFD_PTFD2_MASK                 4
#define PTFD_PTFD3_MASK                 8
#define PTFD_PTFD4_MASK                 16
#define PTFD_PTFD5_MASK                 32
#define PTFD_PTFD6_MASK                 64
#define PTFD_PTFD7_MASK                 128


/*** PTFDD - Port F Data Direction Register; 0x0000000B ***/
typedef union {
  byte Byte;
  struct {
    byte PTFDD0      :1;                                       /* Data Direction for Port F Bit 0 */
    byte PTFDD1      :1;                                       /* Data Direction for Port F Bit 1 */
    byte PTFDD2      :1;                                       /* Data Direction for Port F Bit 2 */
    byte PTFDD3      :1;                                       /* Data Direction for Port F Bit 3 */
    byte PTFDD4      :1;                                       /* Data Direction for Port F Bit 4 */
    byte PTFDD5      :1;                                       /* Data Direction for Port F Bit 5 */
    byte PTFDD6      :1;                                       /* Data Direction for Port F Bit 6 */
    byte PTFDD7      :1;                                       /* Data Direction for Port F Bit 7 */
  } Bits;
} PTFDDSTR;
extern volatile PTFDDSTR _PTFDD @0x0000000B;
#define PTFDD                           _PTFDD.Byte
#define PTFDD_PTFDD0                    _PTFDD.Bits.PTFDD0
#define PTFDD_PTFDD1                    _PTFDD.Bits.PTFDD1
#define PTFDD_PTFDD2                    _PTFDD.Bits.PTFDD2
#define PTFDD_PTFDD3                    _PTFDD.Bits.PTFDD3
#define PTFDD_PTFDD4                    _PTFDD.Bits.PTFDD4
#define PTFDD_PTFDD5                    _PTFDD.Bits.PTFDD5
#define PTFDD_PTFDD6                    _PTFDD.Bits.PTFDD6
#define PTFDD_PTFDD7                    _PTFDD.Bits.PTFDD7

#define PTFDD_PTFDD0_MASK               1
#define PTFDD_PTFDD1_MASK               2
#define PTFDD_PTFDD2_MASK               4
#define PTFDD_PTFDD3_MASK               8
#define PTFDD_PTFDD4_MASK               16
#define PTFDD_PTFDD5_MASK               32
#define PTFDD_PTFDD6_MASK               64
#define PTFDD_PTFDD7_MASK               128


/*** PTGD - Port G Data Register; 0x0000000C ***/
typedef union {
  byte Byte;
  struct {
    byte PTGD0       :1;                                       /* Port G Data Register Bit 0 */
    byte PTGD1       :1;                                       /* Port G Data Register Bit 1 */
    byte PTGD2       :1;                                       /* Port G Data Register Bit 2 */
    byte PTGD3       :1;                                       /* Port G Data Register Bit 3 */
    byte PTGD4       :1;                                       /* Port G Data Register Bit 4 */
    byte PTGD5       :1;                                       /* Port G Data Register Bit 5 */
    byte             :1; 
    byte             :1; 
  } Bits;
  struct {
    byte grpPTGD :6;
    byte         :1;
    byte         :1;
  } MergedBits;
} PTGDSTR;
extern volatile PTGDSTR _PTGD @0x0000000C;
#define PTGD                            _PTGD.Byte
#define PTGD_PTGD0                      _PTGD.Bits.PTGD0
#define PTGD_PTGD1                      _PTGD.Bits.PTGD1
#define PTGD_PTGD2                      _PTGD.Bits.PTGD2
#define PTGD_PTGD3                      _PTGD.Bits.PTGD3
#define PTGD_PTGD4                      _PTGD.Bits.PTGD4
#define PTGD_PTGD5                      _PTGD.Bits.PTGD5
#define PTGD_PTGD                       _PTGD.MergedBits.grpPTGD

#define PTGD_PTGD0_MASK                 1
#define PTGD_PTGD1_MASK                 2
#define PTGD_PTGD2_MASK                 4
#define PTGD_PTGD3_MASK                 8
#define PTGD_PTGD4_MASK                 16
#define PTGD_PTGD5_MASK                 32
#define PTGD_PTGD_MASK                  63
#define PTGD_PTGD_BITNUM                0


/*** PTGDD - Port G Data Direction Register; 0x0000000D ***/
typedef union {
  byte Byte;
  struct {
    byte PTGDD0      :1;                                       /* Data Direction for Port G Bit 0 */
    byte PTGDD1      :1;                                       /* Data Direction for Port G Bit 1 */
    byte PTGDD2      :1;                                       /* Data Direction for Port G Bit 2 */
    byte PTGDD3      :1;                                       /* Data Direction for Port G Bit 3 */
    byte PTGDD4      :1;                                       /* Data Direction for Port G Bit 4 */
    byte PTGDD5      :1;                                       /* Data Direction for Port G Bit 5 */
    byte             :1; 
    byte             :1; 
  } Bits;
  struct {
    byte grpPTGDD :6;
    byte         :1;
    byte         :1;
  } MergedBits;
} PTGDDSTR;
extern volatile PTGDDSTR _PTGDD @0x0000000D;
#define PTGDD                           _PTGDD.Byte
#define PTGDD_PTGDD0                    _PTGDD.Bits.PTGDD0
#define PTGDD_PTGDD1                    _PTGDD.Bits.PTGDD1
#define PTGDD_PTGDD2                    _PTGDD.Bits.PTGDD2
#define PTGDD_PTGDD3                    _PTGDD.Bits.PTGDD3
#define PTGDD_PTGDD4                    _PTGDD.Bits.PTGDD4
#define PTGDD_PTGDD5                    _PTGDD.Bits.PTGDD5
#define PTGDD_PTGDD                     _PTGDD.MergedBits.grpPTGDD

#define PTGDD_PTGDD0_MASK               1
#define PTGDD_PTGDD1_MASK               2
#define PTGDD_PTGDD2_MASK               4
#define PTGDD_PTGDD3_MASK               8
#define PTGDD_PTGDD4_MASK               16
#define PTGDD_PTGDD5_MASK               32
#define PTGDD_PTGDD_MASK                63
#define PTGDD_PTGDD_BITNUM              0


/*** ACMP1SC - ACMP1 Status and Control Register; 0x0000000E ***/
typedef union {
  byte Byte;
  struct {
    byte ACMOD0      :1;                                       /* Analog Comparator Mode Bit 0 */
    byte ACMOD1      :1;                                       /* Analog Comparator Mode Bit 1 */
    byte ACOPE       :1;                                       /* Analog Comparator Output Pin Enable */
    byte ACO         :1;                                       /* Analog Comparator Output */
    byte ACIE        :1;                                       /* Analog Comparator Interrupt Enable */
    byte ACF         :1;                                       /* Analog Comparator Flag */
    byte ACBGS       :1;                                       /* Analog Comparator Bandgap Select */
    byte ACME        :1;                                       /* Analog Comparator Module Enable */
  } Bits;
  struct {
    byte grpACMOD :2;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} ACMP1SCSTR;
extern volatile ACMP1SCSTR _ACMP1SC @0x0000000E;
#define ACMP1SC                         _ACMP1SC.Byte
#define ACMP1SC_ACMOD0                  _ACMP1SC.Bits.ACMOD0
#define ACMP1SC_ACMOD1                  _ACMP1SC.Bits.ACMOD1
#define ACMP1SC_ACOPE                   _ACMP1SC.Bits.ACOPE
#define ACMP1SC_ACO                     _ACMP1SC.Bits.ACO
#define ACMP1SC_ACIE                    _ACMP1SC.Bits.ACIE
#define ACMP1SC_ACF                     _ACMP1SC.Bits.ACF
#define ACMP1SC_ACBGS                   _ACMP1SC.Bits.ACBGS
#define ACMP1SC_ACME                    _ACMP1SC.Bits.ACME
#define ACMP1SC_ACMOD                   _ACMP1SC.MergedBits.grpACMOD

#define ACMP1SC_ACMOD0_MASK             1
#define ACMP1SC_ACMOD1_MASK             2
#define ACMP1SC_ACOPE_MASK              4
#define ACMP1SC_ACO_MASK                8
#define ACMP1SC_ACIE_MASK               16
#define ACMP1SC_ACF_MASK                32
#define ACMP1SC_ACBGS_MASK              64
#define ACMP1SC_ACME_MASK               128
#define ACMP1SC_ACMOD_MASK              3
#define ACMP1SC_ACMOD_BITNUM            0


/*** ACMP2SC - ACMP2 Status and Control Register; 0x0000000F ***/
typedef union {
  byte Byte;
  struct {
    byte ACMOD0      :1;                                       /* Analog Comparator Mode Bit 0 */
    byte ACMOD1      :1;                                       /* Analog Comparator Mode Bit 1 */
    byte ACOPE       :1;                                       /* Analog Comparator Output Pin Enable */
    byte ACO         :1;                                       /* Analog Comparator Output */
    byte ACIE        :1;                                       /* Analog Comparator Interrupt Enable */
    byte ACF         :1;                                       /* Analog Comparator Flag */
    byte ACBGS       :1;                                       /* Analog Comparator Bandgap Select */
    byte ACME        :1;                                       /* Analog Comparator Module Enable */
  } Bits;
  struct {
    byte grpACMOD :2;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} ACMP2SCSTR;
extern volatile ACMP2SCSTR _ACMP2SC @0x0000000F;
#define ACMP2SC                         _ACMP2SC.Byte
#define ACMP2SC_ACMOD0                  _ACMP2SC.Bits.ACMOD0
#define ACMP2SC_ACMOD1                  _ACMP2SC.Bits.ACMOD1
#define ACMP2SC_ACOPE                   _ACMP2SC.Bits.ACOPE
#define ACMP2SC_ACO                     _ACMP2SC.Bits.ACO
#define ACMP2SC_ACIE                    _ACMP2SC.Bits.ACIE
#define ACMP2SC_ACF                     _ACMP2SC.Bits.ACF
#define ACMP2SC_ACBGS                   _ACMP2SC.Bits.ACBGS
#define ACMP2SC_ACME                    _ACMP2SC.Bits.ACME
#define ACMP2SC_ACMOD                   _ACMP2SC.MergedBits.grpACMOD

#define ACMP2SC_ACMOD0_MASK             1
#define ACMP2SC_ACMOD1_MASK             2
#define ACMP2SC_ACOPE_MASK              4
#define ACMP2SC_ACO_MASK                8
#define ACMP2SC_ACIE_MASK               16
#define ACMP2SC_ACF_MASK                32
#define ACMP2SC_ACBGS_MASK              64
#define ACMP2SC_ACME_MASK               128
#define ACMP2SC_ACMOD_MASK              3
#define ACMP2SC_ACMOD_BITNUM            0


/*** ADCSC1 - Status and Control Register 1; 0x00000010 ***/
typedef union {
  byte Byte;
  struct {
    byte ADCH0       :1;                                       /* Input Channel Select Bit 0 */
    byte ADCH1       :1;                                       /* Input Channel Select Bit 1 */
    byte ADCH2       :1;                                       /* Input Channel Select Bit 2 */
    byte ADCH3       :1;                                       /* Input Channel Select Bit 3 */
    byte ADCH4       :1;                                       /* Input Channel Select Bit 4 */
    byte ADCO        :1;                                       /* Continuous Conversion Enable - ADCO is used to enable continuous conversions */
    byte AIEN        :1;                                       /* Interrupt Enable - AIEN is used to enable conversion complete interrupts. When COCO becomes set while AIEN is high, an interrupt is asserted */
    byte COCO        :1;                                       /* Conversion Complete Flag */
  } Bits;
  struct {
    byte grpADCH :5;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} ADCSC1STR;
extern volatile ADCSC1STR _ADCSC1 @0x00000010;
#define ADCSC1                          _ADCSC1.Byte
#define ADCSC1_ADCH0                    _ADCSC1.Bits.ADCH0
#define ADCSC1_ADCH1                    _ADCSC1.Bits.ADCH1
#define ADCSC1_ADCH2                    _ADCSC1.Bits.ADCH2
#define ADCSC1_ADCH3                    _ADCSC1.Bits.ADCH3
#define ADCSC1_ADCH4                    _ADCSC1.Bits.ADCH4
#define ADCSC1_ADCO                     _ADCSC1.Bits.ADCO
#define ADCSC1_AIEN                     _ADCSC1.Bits.AIEN
#define ADCSC1_COCO                     _ADCSC1.Bits.COCO
#define ADCSC1_ADCH                     _ADCSC1.MergedBits.grpADCH

#define ADCSC1_ADCH0_MASK               1
#define ADCSC1_ADCH1_MASK               2
#define ADCSC1_ADCH2_MASK               4
#define ADCSC1_ADCH3_MASK               8
#define ADCSC1_ADCH4_MASK               16
#define ADCSC1_ADCO_MASK                32
#define ADCSC1_AIEN_MASK                64
#define ADCSC1_COCO_MASK                128
#define ADCSC1_ADCH_MASK                31
#define ADCSC1_ADCH_BITNUM              0


/*** ADCSC2 - Status and Control Register 2; 0x00000011 ***/
typedef union {
  byte Byte;
  struct {
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte ACFGT       :1;                                       /* Compare Function Greater Than Enable */
    byte ACFE        :1;                                       /* Compare Function Enable - ACFE is used to enable the compare function */
    byte ADTRG       :1;                                       /* Conversion Trigger Select-ADTRG is used to select the type of trigger to be used for initiating a conversion */
    byte ADACT       :1;                                       /* Conversion Active - ADACT indicates that a conversion is in progress. ADACT is set when a conversion is initiated and cleared when a conversion is completed or aborted */
  } Bits;
} ADCSC2STR;
extern volatile ADCSC2STR _ADCSC2 @0x00000011;
#define ADCSC2                          _ADCSC2.Byte
#define ADCSC2_ACFGT                    _ADCSC2.Bits.ACFGT
#define ADCSC2_ACFE                     _ADCSC2.Bits.ACFE
#define ADCSC2_ADTRG                    _ADCSC2.Bits.ADTRG
#define ADCSC2_ADACT                    _ADCSC2.Bits.ADACT

#define ADCSC2_ACFGT_MASK               16
#define ADCSC2_ACFE_MASK                32
#define ADCSC2_ADTRG_MASK               64
#define ADCSC2_ADACT_MASK               128


/*** ADCR - Data Result Register; 0x00000012 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** ADCRH - Data Result High Register; 0x00000012 ***/
    union {
      byte Byte;
      struct {
        byte ADR8        :1;                                       /* ADC Result Data Bit 8 */
        byte ADR9        :1;                                       /* ADC Result Data Bit 9 */
        byte ADR10       :1;                                       /* ADC Result Data Bit 10 */
        byte ADR11       :1;                                       /* ADC Result Data Bit 11 */
        byte             :1; 
        byte             :1; 
        byte             :1; 
        byte             :1; 
      } Bits;
      struct {
        byte grpADR_8 :4;
        byte     :1;
        byte     :1;
        byte     :1;
        byte     :1;
      } MergedBits;
    } ADCRHSTR;
    #define ADCRH                       _ADCR.Overlap_STR.ADCRHSTR.Byte
    #define ADCRH_ADR8                  _ADCR.Overlap_STR.ADCRHSTR.Bits.ADR8
    #define ADCRH_ADR9                  _ADCR.Overlap_STR.ADCRHSTR.Bits.ADR9
    #define ADCRH_ADR10                 _ADCR.Overlap_STR.ADCRHSTR.Bits.ADR10
    #define ADCRH_ADR11                 _ADCR.Overlap_STR.ADCRHSTR.Bits.ADR11
    #define ADCRH_ADR_8                 _ADCR.Overlap_STR.ADCRHSTR.MergedBits.grpADR_8
    #define ADCRH_ADR                   ADCRH_ADR_8
    
    #define ADCRH_ADR8_MASK             1
    #define ADCRH_ADR9_MASK             2
    #define ADCRH_ADR10_MASK            4
    #define ADCRH_ADR11_MASK            8
    #define ADCRH_ADR_8_MASK            15
    #define ADCRH_ADR_8_BITNUM          0
    

    /*** ADCRL - Data Result Low Register; 0x00000013 ***/
    union {
      byte Byte;
      struct {
        byte ADR0        :1;                                       /* ADC Result Data Bit 0 */
        byte ADR1        :1;                                       /* ADC Result Data Bit 1 */
        byte ADR2        :1;                                       /* ADC Result Data Bit 2 */
        byte ADR3        :1;                                       /* ADC Result Data Bit 3 */
        byte ADR4        :1;                                       /* ADC Result Data Bit 4 */
        byte ADR5        :1;                                       /* ADC Result Data Bit 5 */
        byte ADR6        :1;                                       /* ADC Result Data Bit 6 */
        byte ADR7        :1;                                       /* ADC Result Data Bit 7 */
      } Bits;
    } ADCRLSTR;
    #define ADCRL                       _ADCR.Overlap_STR.ADCRLSTR.Byte
    #define ADCRL_ADR0                  _ADCR.Overlap_STR.ADCRLSTR.Bits.ADR0
    #define ADCRL_ADR1                  _ADCR.Overlap_STR.ADCRLSTR.Bits.ADR1
    #define ADCRL_ADR2                  _ADCR.Overlap_STR.ADCRLSTR.Bits.ADR2
    #define ADCRL_ADR3                  _ADCR.Overlap_STR.ADCRLSTR.Bits.ADR3
    #define ADCRL_ADR4                  _ADCR.Overlap_STR.ADCRLSTR.Bits.ADR4
    #define ADCRL_ADR5                  _ADCR.Overlap_STR.ADCRLSTR.Bits.ADR5
    #define ADCRL_ADR6                  _ADCR.Overlap_STR.ADCRLSTR.Bits.ADR6
    #define ADCRL_ADR7                  _ADCR.Overlap_STR.ADCRLSTR.Bits.ADR7
    
    #define ADCRL_ADR0_MASK             1
    #define ADCRL_ADR1_MASK             2
    #define ADCRL_ADR2_MASK             4
    #define ADCRL_ADR3_MASK             8
    #define ADCRL_ADR4_MASK             16
    #define ADCRL_ADR5_MASK             32
    #define ADCRL_ADR6_MASK             64
    #define ADCRL_ADR7_MASK             128
    
  } Overlap_STR;

} ADCRSTR;
extern volatile ADCRSTR _ADCR @0x00000012;
#define ADCR                            _ADCR.Word


/*** ADCCV - Compare Value Register; 0x00000014 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** ADCCVH - Compare Value High Register; 0x00000014 ***/
    union {
      byte Byte;
      struct {
        byte ADCV8       :1;                                       /* Compare Function Value 8 */
        byte ADCV9       :1;                                       /* Compare Function Value 9 */
        byte ADCV10      :1;                                       /* Compare Function Value 10 */
        byte ADCV11      :1;                                       /* Compare Function Value 11 */
        byte             :1; 
        byte             :1; 
        byte             :1; 
        byte             :1; 
      } Bits;
      struct {
        byte grpADCV_8 :4;
        byte     :1;
        byte     :1;
        byte     :1;
        byte     :1;
      } MergedBits;
    } ADCCVHSTR;
    #define ADCCVH                      _ADCCV.Overlap_STR.ADCCVHSTR.Byte
    #define ADCCVH_ADCV8                _ADCCV.Overlap_STR.ADCCVHSTR.Bits.ADCV8
    #define ADCCVH_ADCV9                _ADCCV.Overlap_STR.ADCCVHSTR.Bits.ADCV9
    #define ADCCVH_ADCV10               _ADCCV.Overlap_STR.ADCCVHSTR.Bits.ADCV10
    #define ADCCVH_ADCV11               _ADCCV.Overlap_STR.ADCCVHSTR.Bits.ADCV11
    #define ADCCVH_ADCV_8               _ADCCV.Overlap_STR.ADCCVHSTR.MergedBits.grpADCV_8
    #define ADCCVH_ADCV                 ADCCVH_ADCV_8
    
    #define ADCCVH_ADCV8_MASK           1
    #define ADCCVH_ADCV9_MASK           2
    #define ADCCVH_ADCV10_MASK          4
    #define ADCCVH_ADCV11_MASK          8
    #define ADCCVH_ADCV_8_MASK          15
    #define ADCCVH_ADCV_8_BITNUM        0
    

    /*** ADCCVL - Compare Value Low Register; 0x00000015 ***/
    union {
      byte Byte;
      struct {
        byte ADCV0       :1;                                       /* Compare Function Value 0 */
        byte ADCV1       :1;                                       /* Compare Function Value 1 */
        byte ADCV2       :1;                                       /* Compare Function Value 2 */
        byte ADCV3       :1;                                       /* Compare Function Value 3 */
        byte ADCV4       :1;                                       /* Compare Function Value 4 */
        byte ADCV5       :1;                                       /* Compare Function Value 5 */
        byte ADCV6       :1;                                       /* Compare Function Value 6 */
        byte ADCV7       :1;                                       /* Compare Function Value 7 */
      } Bits;
    } ADCCVLSTR;
    #define ADCCVL                      _ADCCV.Overlap_STR.ADCCVLSTR.Byte
    #define ADCCVL_ADCV0                _ADCCV.Overlap_STR.ADCCVLSTR.Bits.ADCV0
    #define ADCCVL_ADCV1                _ADCCV.Overlap_STR.ADCCVLSTR.Bits.ADCV1
    #define ADCCVL_ADCV2                _ADCCV.Overlap_STR.ADCCVLSTR.Bits.ADCV2
    #define ADCCVL_ADCV3                _ADCCV.Overlap_STR.ADCCVLSTR.Bits.ADCV3
    #define ADCCVL_ADCV4                _ADCCV.Overlap_STR.ADCCVLSTR.Bits.ADCV4
    #define ADCCVL_ADCV5                _ADCCV.Overlap_STR.ADCCVLSTR.Bits.ADCV5
    #define ADCCVL_ADCV6                _ADCCV.Overlap_STR.ADCCVLSTR.Bits.ADCV6
    #define ADCCVL_ADCV7                _ADCCV.Overlap_STR.ADCCVLSTR.Bits.ADCV7
    
    #define ADCCVL_ADCV0_MASK           1
    #define ADCCVL_ADCV1_MASK           2
    #define ADCCVL_ADCV2_MASK           4
    #define ADCCVL_ADCV3_MASK           8
    #define ADCCVL_ADCV4_MASK           16
    #define ADCCVL_ADCV5_MASK           32
    #define ADCCVL_ADCV6_MASK           64
    #define ADCCVL_ADCV7_MASK           128
    
  } Overlap_STR;

} ADCCVSTR;
extern volatile ADCCVSTR _ADCCV @0x00000014;
#define ADCCV                           _ADCCV.Word


/*** ADCCFG - Configuration Register; 0x00000016 ***/
typedef union {
  byte Byte;
  struct {
    byte ADICLK0     :1;                                       /* Input Clock Select Bit 0 */
    byte ADICLK1     :1;                                       /* Input Clock Select Bit 1 */
    byte MODE0       :1;                                       /* Conversion Mode Selection Bit 0 */
    byte MODE1       :1;                                       /* Conversion Mode Selection Bit 1 */
    byte ADLSMP      :1;                                       /* Long Sample Time Configuration */
    byte ADIV0       :1;                                       /* Clock Divide Select Bit 0 */
    byte ADIV1       :1;                                       /* Clock Divide Select Bit 1 */
    byte ADLPC       :1;                                       /* Low Power Configuration */
  } Bits;
  struct {
    byte grpADICLK :2;
    byte grpMODE :2;
    byte         :1;
    byte grpADIV :2;
    byte         :1;
  } MergedBits;
} ADCCFGSTR;
extern volatile ADCCFGSTR _ADCCFG @0x00000016;
#define ADCCFG                          _ADCCFG.Byte
#define ADCCFG_ADICLK0                  _ADCCFG.Bits.ADICLK0
#define ADCCFG_ADICLK1                  _ADCCFG.Bits.ADICLK1
#define ADCCFG_MODE0                    _ADCCFG.Bits.MODE0
#define ADCCFG_MODE1                    _ADCCFG.Bits.MODE1
#define ADCCFG_ADLSMP                   _ADCCFG.Bits.ADLSMP
#define ADCCFG_ADIV0                    _ADCCFG.Bits.ADIV0
#define ADCCFG_ADIV1                    _ADCCFG.Bits.ADIV1
#define ADCCFG_ADLPC                    _ADCCFG.Bits.ADLPC
#define ADCCFG_ADICLK                   _ADCCFG.MergedBits.grpADICLK
#define ADCCFG_MODE                     _ADCCFG.MergedBits.grpMODE
#define ADCCFG_ADIV                     _ADCCFG.MergedBits.grpADIV

#define ADCCFG_ADICLK0_MASK             1
#define ADCCFG_ADICLK1_MASK             2
#define ADCCFG_MODE0_MASK               4
#define ADCCFG_MODE1_MASK               8
#define ADCCFG_ADLSMP_MASK              16
#define ADCCFG_ADIV0_MASK               32
#define ADCCFG_ADIV1_MASK               64
#define ADCCFG_ADLPC_MASK               128
#define ADCCFG_ADICLK_MASK              3
#define ADCCFG_ADICLK_BITNUM            0
#define ADCCFG_MODE_MASK                12
#define ADCCFG_MODE_BITNUM              2
#define ADCCFG_ADIV_MASK                96
#define ADCCFG_ADIV_BITNUM              5


/*** APCTL1 - Pin Control 1 Register; 0x00000017 ***/
typedef union {
  byte Byte;
  struct {
    byte ADPC0       :1;                                       /* ADC Pin Control 0 - ADPC0 is used to control the pin associated with channel AD0 */
    byte ADPC1       :1;                                       /* ADC Pin Control 1 - ADPC1 is used to control the pin associated with channel AD1 */
    byte ADPC2       :1;                                       /* ADC Pin Control 2 - ADPC2 is used to control the pin associated with channel AD2 */
    byte ADPC3       :1;                                       /* ADC Pin Control 3 - ADPC3 is used to control the pin associated with channel AD3 */
    byte ADPC4       :1;                                       /* ADC Pin Control 4 - ADPC4 is used to control the pin associated with channel AD4 */
    byte ADPC5       :1;                                       /* ADC Pin Control 5 - ADPC5 is used to control the pin associated with channel AD5 */
    byte ADPC6       :1;                                       /* ADC Pin Control 6 - ADPC6 is used to control the pin associated with channel AD6 */
    byte ADPC7       :1;                                       /* ADC Pin Control 7 - ADPC7 is used to control the pin associated with channel AD7 */
  } Bits;
} APCTL1STR;
extern volatile APCTL1STR _APCTL1 @0x00000017;
#define APCTL1                          _APCTL1.Byte
#define APCTL1_ADPC0                    _APCTL1.Bits.ADPC0
#define APCTL1_ADPC1                    _APCTL1.Bits.ADPC1
#define APCTL1_ADPC2                    _APCTL1.Bits.ADPC2
#define APCTL1_ADPC3                    _APCTL1.Bits.ADPC3
#define APCTL1_ADPC4                    _APCTL1.Bits.ADPC4
#define APCTL1_ADPC5                    _APCTL1.Bits.ADPC5
#define APCTL1_ADPC6                    _APCTL1.Bits.ADPC6
#define APCTL1_ADPC7                    _APCTL1.Bits.ADPC7

#define APCTL1_ADPC0_MASK               1
#define APCTL1_ADPC1_MASK               2
#define APCTL1_ADPC2_MASK               4
#define APCTL1_ADPC3_MASK               8
#define APCTL1_ADPC4_MASK               16
#define APCTL1_ADPC5_MASK               32
#define APCTL1_ADPC6_MASK               64
#define APCTL1_ADPC7_MASK               128


/*** APCTL2 - Pin Control 2 Register; 0x00000018 ***/
typedef union {
  byte Byte;
  struct {
    byte ADPC8       :1;                                       /* ADC Pin Control 8 - ADPC8 is used to control the pin associated with channel AD8 */
    byte ADPC9       :1;                                       /* ADC Pin Control 9 - ADPC9 is used to control the pin associated with channel AD9 */
    byte ADPC10      :1;                                       /* ADC Pin Control 10 - ADPC10 is used to control the pin associated with channel AD10 */
    byte ADPC11      :1;                                       /* ADC Pin Control 11 - ADPC11 is used to control the pin associated with channel AD11 */
    byte ADPC12      :1;                                       /* ADC Pin Control 12 - ADPC12 is used to control the pin associated with channel AD12 */
    byte ADPC13      :1;                                       /* ADC Pin Control 13 - ADPC13 is used to control the pin associated with channel AD13 */
    byte ADPC14      :1;                                       /* ADC Pin Control 14 - ADPC14 is used to control the pin associated with channel AD14 */
    byte ADPC15      :1;                                       /* ADC Pin Control 15 - ADPC15 is used to control the pin associated with channel AD15 */
  } Bits;
} APCTL2STR;
extern volatile APCTL2STR _APCTL2 @0x00000018;
#define APCTL2                          _APCTL2.Byte
#define APCTL2_ADPC8                    _APCTL2.Bits.ADPC8
#define APCTL2_ADPC9                    _APCTL2.Bits.ADPC9
#define APCTL2_ADPC10                   _APCTL2.Bits.ADPC10
#define APCTL2_ADPC11                   _APCTL2.Bits.ADPC11
#define APCTL2_ADPC12                   _APCTL2.Bits.ADPC12
#define APCTL2_ADPC13                   _APCTL2.Bits.ADPC13
#define APCTL2_ADPC14                   _APCTL2.Bits.ADPC14
#define APCTL2_ADPC15                   _APCTL2.Bits.ADPC15

#define APCTL2_ADPC8_MASK               1
#define APCTL2_ADPC9_MASK               2
#define APCTL2_ADPC10_MASK              4
#define APCTL2_ADPC11_MASK              8
#define APCTL2_ADPC12_MASK              16
#define APCTL2_ADPC13_MASK              32
#define APCTL2_ADPC14_MASK              64
#define APCTL2_ADPC15_MASK              128


/*** APCTL3 - Pin Control 3 Register; 0x00000019 ***/
typedef union {
  byte Byte;
  struct {
    byte ADPC16      :1;                                       /* ADC Pin Control 16 - ADPC16 is used to control the pin associated with channel AD16 */
    byte ADPC17      :1;                                       /* ADC Pin Control 17 - ADPC17 is used to control the pin associated with channel AD17 */
    byte ADPC18      :1;                                       /* ADC Pin Control 18 - ADPC18 is used to control the pin associated with channel AD18 */
    byte ADPC19      :1;                                       /* ADC Pin Control 19 - ADPC19 is used to control the pin associated with channel AD19 */
    byte ADPC20      :1;                                       /* ADC Pin Control 20 - ADPC20 is used to control the pin associated with channel AD20 */
    byte ADPC21      :1;                                       /* ADC Pin Control 21 - ADPC21 is used to control the pin associated with channel AD21 */
    byte ADPC22      :1;                                       /* ADC Pin Control 22 - ADPC22 is used to control the pin associated with channel AD22 */
    byte ADPC23      :1;                                       /* ADC Pin Control 23 - ADPC23 is used to control the pin associated with channel AD23 */
  } Bits;
} APCTL3STR;
extern volatile APCTL3STR _APCTL3 @0x00000019;
#define APCTL3                          _APCTL3.Byte
#define APCTL3_ADPC16                   _APCTL3.Bits.ADPC16
#define APCTL3_ADPC17                   _APCTL3.Bits.ADPC17
#define APCTL3_ADPC18                   _APCTL3.Bits.ADPC18
#define APCTL3_ADPC19                   _APCTL3.Bits.ADPC19
#define APCTL3_ADPC20                   _APCTL3.Bits.ADPC20
#define APCTL3_ADPC21                   _APCTL3.Bits.ADPC21
#define APCTL3_ADPC22                   _APCTL3.Bits.ADPC22
#define APCTL3_ADPC23                   _APCTL3.Bits.ADPC23

#define APCTL3_ADPC16_MASK              1
#define APCTL3_ADPC17_MASK              2
#define APCTL3_ADPC18_MASK              4
#define APCTL3_ADPC19_MASK              8
#define APCTL3_ADPC20_MASK              16
#define APCTL3_ADPC21_MASK              32
#define APCTL3_ADPC22_MASK              64
#define APCTL3_ADPC23_MASK              128


/*** IRQSC - Interrupt request status and control register; 0x0000001C ***/
typedef union {
  byte Byte;
  struct {
    byte IRQMOD      :1;                                       /* IRQ Detection Mode */
    byte IRQIE       :1;                                       /* IRQ Interrupt Enable */
    byte IRQACK      :1;                                       /* IRQ Acknowledge */
    byte IRQF        :1;                                       /* IRQ Flag */
    byte IRQPE       :1;                                       /* IRQ Pin Enable */
    byte IRQEDG      :1;                                       /* IRQ Edge Select */
    byte IRQPDD      :1;                                       /* IRQ Pull Device Disable */
    byte             :1; 
  } Bits;
} IRQSCSTR;
extern volatile IRQSCSTR _IRQSC @0x0000001C;
#define IRQSC                           _IRQSC.Byte
#define IRQSC_IRQMOD                    _IRQSC.Bits.IRQMOD
#define IRQSC_IRQIE                     _IRQSC.Bits.IRQIE
#define IRQSC_IRQACK                    _IRQSC.Bits.IRQACK
#define IRQSC_IRQF                      _IRQSC.Bits.IRQF
#define IRQSC_IRQPE                     _IRQSC.Bits.IRQPE
#define IRQSC_IRQEDG                    _IRQSC.Bits.IRQEDG
#define IRQSC_IRQPDD                    _IRQSC.Bits.IRQPDD

#define IRQSC_IRQMOD_MASK               1
#define IRQSC_IRQIE_MASK                2
#define IRQSC_IRQACK_MASK               4
#define IRQSC_IRQF_MASK                 8
#define IRQSC_IRQPE_MASK                16
#define IRQSC_IRQEDG_MASK               32
#define IRQSC_IRQPDD_MASK               64


/*** TPM1SC - TPM1 Status and Control Register; 0x00000020 ***/
typedef union {
  byte Byte;
  struct {
    byte PS0         :1;                                       /* Prescale Divisor Select Bit 0 */
    byte PS1         :1;                                       /* Prescale Divisor Select Bit 1 */
    byte PS2         :1;                                       /* Prescale Divisor Select Bit 2 */
    byte CLKSA       :1;                                       /* Clock Source Select A */
    byte CLKSB       :1;                                       /* Clock Source Select B */
    byte CPWMS       :1;                                       /* Center-Aligned PWM Select */
    byte TOIE        :1;                                       /* Timer Overflow Interrupt Enable */
    byte TOF         :1;                                       /* Timer Overflow Flag */
  } Bits;
  struct {
    byte grpPS   :3;
    byte grpCLKSx :2;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} TPM1SCSTR;
extern volatile TPM1SCSTR _TPM1SC @0x00000020;
#define TPM1SC                          _TPM1SC.Byte
#define TPM1SC_PS0                      _TPM1SC.Bits.PS0
#define TPM1SC_PS1                      _TPM1SC.Bits.PS1
#define TPM1SC_PS2                      _TPM1SC.Bits.PS2
#define TPM1SC_CLKSA                    _TPM1SC.Bits.CLKSA
#define TPM1SC_CLKSB                    _TPM1SC.Bits.CLKSB
#define TPM1SC_CPWMS                    _TPM1SC.Bits.CPWMS
#define TPM1SC_TOIE                     _TPM1SC.Bits.TOIE
#define TPM1SC_TOF                      _TPM1SC.Bits.TOF
#define TPM1SC_PS                       _TPM1SC.MergedBits.grpPS
#define TPM1SC_CLKSx                    _TPM1SC.MergedBits.grpCLKSx

#define TPM1SC_PS0_MASK                 1
#define TPM1SC_PS1_MASK                 2
#define TPM1SC_PS2_MASK                 4
#define TPM1SC_CLKSA_MASK               8
#define TPM1SC_CLKSB_MASK               16
#define TPM1SC_CPWMS_MASK               32
#define TPM1SC_TOIE_MASK                64
#define TPM1SC_TOF_MASK                 128
#define TPM1SC_PS_MASK                  7
#define TPM1SC_PS_BITNUM                0
#define TPM1SC_CLKSx_MASK               24
#define TPM1SC_CLKSx_BITNUM             3


/*** TPM1CNT - TPM1 Timer Counter Register; 0x00000021 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** TPM1CNTH - TPM1 Timer Counter Register High; 0x00000021 ***/
    union {
      byte Byte;
    } TPM1CNTHSTR;
    #define TPM1CNTH                    _TPM1CNT.Overlap_STR.TPM1CNTHSTR.Byte
    

    /*** TPM1CNTL - TPM1 Timer Counter Register Low; 0x00000022 ***/
    union {
      byte Byte;
    } TPM1CNTLSTR;
    #define TPM1CNTL                    _TPM1CNT.Overlap_STR.TPM1CNTLSTR.Byte
    
  } Overlap_STR;

} TPM1CNTSTR;
extern volatile TPM1CNTSTR _TPM1CNT @0x00000021;
#define TPM1CNT                         _TPM1CNT.Word


/*** TPM1MOD - TPM1 Timer Counter Modulo Register; 0x00000023 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** TPM1MODH - TPM1 Timer Counter Modulo Register High; 0x00000023 ***/
    union {
      byte Byte;
    } TPM1MODHSTR;
    #define TPM1MODH                    _TPM1MOD.Overlap_STR.TPM1MODHSTR.Byte
    

    /*** TPM1MODL - TPM1 Timer Counter Modulo Register Low; 0x00000024 ***/
    union {
      byte Byte;
    } TPM1MODLSTR;
    #define TPM1MODL                    _TPM1MOD.Overlap_STR.TPM1MODLSTR.Byte
    
  } Overlap_STR;

} TPM1MODSTR;
extern volatile TPM1MODSTR _TPM1MOD @0x00000023;
#define TPM1MOD                         _TPM1MOD.Word


/*** TPM1C0SC - TPM1 Timer Channel 0 Status and Control Register; 0x00000025 ***/
typedef union {
  byte Byte;
  struct {
    byte             :1; 
    byte             :1; 
    byte ELS0A       :1;                                       /* Edge/Level Select Bit A */
    byte ELS0B       :1;                                       /* Edge/Level Select Bit B */
    byte MS0A        :1;                                       /* Mode Select A for TPM Channel 0 */
    byte MS0B        :1;                                       /* Mode Select B for TPM Channel 0 */
    byte CH0IE       :1;                                       /* Channel 0 Interrupt Enable */
    byte CH0F        :1;                                       /* Channel 0 Flag */
  } Bits;
  struct {
    byte         :1;
    byte         :1;
    byte grpELS0x :2;
    byte grpMS0x :2;
    byte         :1;
    byte         :1;
  } MergedBits;
} TPM1C0SCSTR;
extern volatile TPM1C0SCSTR _TPM1C0SC @0x00000025;
#define TPM1C0SC                        _TPM1C0SC.Byte
#define TPM1C0SC_ELS0A                  _TPM1C0SC.Bits.ELS0A
#define TPM1C0SC_ELS0B                  _TPM1C0SC.Bits.ELS0B
#define TPM1C0SC_MS0A                   _TPM1C0SC.Bits.MS0A
#define TPM1C0SC_MS0B                   _TPM1C0SC.Bits.MS0B
#define TPM1C0SC_CH0IE                  _TPM1C0SC.Bits.CH0IE
#define TPM1C0SC_CH0F                   _TPM1C0SC.Bits.CH0F
#define TPM1C0SC_ELS0x                  _TPM1C0SC.MergedBits.grpELS0x
#define TPM1C0SC_MS0x                   _TPM1C0SC.MergedBits.grpMS0x

#define TPM1C0SC_ELS0A_MASK             4
#define TPM1C0SC_ELS0B_MASK             8
#define TPM1C0SC_MS0A_MASK              16
#define TPM1C0SC_MS0B_MASK              32
#define TPM1C0SC_CH0IE_MASK             64
#define TPM1C0SC_CH0F_MASK              128
#define TPM1C0SC_ELS0x_MASK             12
#define TPM1C0SC_ELS0x_BITNUM           2
#define TPM1C0SC_MS0x_MASK              48
#define TPM1C0SC_MS0x_BITNUM            4


/*** TPM1C0V - TPM1 Timer Channel 0 Value Register; 0x00000026 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** TPM1C0VH - TPM1 Timer Channel 0 Value Register High; 0x00000026 ***/
    union {
      byte Byte;
    } TPM1C0VHSTR;
    #define TPM1C0VH                    _TPM1C0V.Overlap_STR.TPM1C0VHSTR.Byte
    

    /*** TPM1C0VL - TPM1 Timer Channel 0 Value Register Low; 0x00000027 ***/
    union {
      byte Byte;
    } TPM1C0VLSTR;
    #define TPM1C0VL                    _TPM1C0V.Overlap_STR.TPM1C0VLSTR.Byte
    
  } Overlap_STR;

} TPM1C0VSTR;
extern volatile TPM1C0VSTR _TPM1C0V @0x00000026;
#define TPM1C0V                         _TPM1C0V.Word


/*** TPM1C1SC - TPM1 Timer Channel 1 Status and Control Register; 0x00000028 ***/
typedef union {
  byte Byte;
  struct {
    byte             :1; 
    byte             :1; 
    byte ELS1A       :1;                                       /* Edge/Level Select Bit A */
    byte ELS1B       :1;                                       /* Edge/Level Select Bit B */
    byte MS1A        :1;                                       /* Mode Select A for TPM Channel 1 */
    byte MS1B        :1;                                       /* Mode Select B for TPM Channel 1 */
    byte CH1IE       :1;                                       /* Channel 1 Interrupt Enable */
    byte CH1F        :1;                                       /* Channel 1 Flag */
  } Bits;
  struct {
    byte         :1;
    byte         :1;
    byte grpELS1x :2;
    byte grpMS1x :2;
    byte         :1;
    byte         :1;
  } MergedBits;
} TPM1C1SCSTR;
extern volatile TPM1C1SCSTR _TPM1C1SC @0x00000028;
#define TPM1C1SC                        _TPM1C1SC.Byte
#define TPM1C1SC_ELS1A                  _TPM1C1SC.Bits.ELS1A
#define TPM1C1SC_ELS1B                  _TPM1C1SC.Bits.ELS1B
#define TPM1C1SC_MS1A                   _TPM1C1SC.Bits.MS1A
#define TPM1C1SC_MS1B                   _TPM1C1SC.Bits.MS1B
#define TPM1C1SC_CH1IE                  _TPM1C1SC.Bits.CH1IE
#define TPM1C1SC_CH1F                   _TPM1C1SC.Bits.CH1F
#define TPM1C1SC_ELS1x                  _TPM1C1SC.MergedBits.grpELS1x
#define TPM1C1SC_MS1x                   _TPM1C1SC.MergedBits.grpMS1x

#define TPM1C1SC_ELS1A_MASK             4
#define TPM1C1SC_ELS1B_MASK             8
#define TPM1C1SC_MS1A_MASK              16
#define TPM1C1SC_MS1B_MASK              32
#define TPM1C1SC_CH1IE_MASK             64
#define TPM1C1SC_CH1F_MASK              128
#define TPM1C1SC_ELS1x_MASK             12
#define TPM1C1SC_ELS1x_BITNUM           2
#define TPM1C1SC_MS1x_MASK              48
#define TPM1C1SC_MS1x_BITNUM            4


/*** TPM1C1V - TPM1 Timer Channel 1 Value Register; 0x00000029 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** TPM1C1VH - TPM1 Timer Channel 1 Value Register High; 0x00000029 ***/
    union {
      byte Byte;
    } TPM1C1VHSTR;
    #define TPM1C1VH                    _TPM1C1V.Overlap_STR.TPM1C1VHSTR.Byte
    

    /*** TPM1C1VL - TPM1 Timer Channel 1 Value Register Low; 0x0000002A ***/
    union {
      byte Byte;
    } TPM1C1VLSTR;
    #define TPM1C1VL                    _TPM1C1V.Overlap_STR.TPM1C1VLSTR.Byte
    
  } Overlap_STR;

} TPM1C1VSTR;
extern volatile TPM1C1VSTR _TPM1C1V @0x00000029;
#define TPM1C1V                         _TPM1C1V.Word


/*** TPM1C2SC - TPM1 Timer Channel 2 Status and Control Register; 0x0000002B ***/
typedef union {
  byte Byte;
  struct {
    byte             :1; 
    byte             :1; 
    byte ELS2A       :1;                                       /* Edge/Level Select Bit A */
    byte ELS2B       :1;                                       /* Edge/Level Select Bit B */
    byte MS2A        :1;                                       /* Mode Select A for TPM Channel 2 */
    byte MS2B        :1;                                       /* Mode Select B for TPM Channel 2 */
    byte CH2IE       :1;                                       /* Channel 2 Interrupt Enable */
    byte CH2F        :1;                                       /* Channel 2 Flag */
  } Bits;
  struct {
    byte         :1;
    byte         :1;
    byte grpELS2x :2;
    byte grpMS2x :2;
    byte         :1;
    byte         :1;
  } MergedBits;
} TPM1C2SCSTR;
extern volatile TPM1C2SCSTR _TPM1C2SC @0x0000002B;
#define TPM1C2SC                        _TPM1C2SC.Byte
#define TPM1C2SC_ELS2A                  _TPM1C2SC.Bits.ELS2A
#define TPM1C2SC_ELS2B                  _TPM1C2SC.Bits.ELS2B
#define TPM1C2SC_MS2A                   _TPM1C2SC.Bits.MS2A
#define TPM1C2SC_MS2B                   _TPM1C2SC.Bits.MS2B
#define TPM1C2SC_CH2IE                  _TPM1C2SC.Bits.CH2IE
#define TPM1C2SC_CH2F                   _TPM1C2SC.Bits.CH2F
#define TPM1C2SC_ELS2x                  _TPM1C2SC.MergedBits.grpELS2x
#define TPM1C2SC_MS2x                   _TPM1C2SC.MergedBits.grpMS2x

#define TPM1C2SC_ELS2A_MASK             4
#define TPM1C2SC_ELS2B_MASK             8
#define TPM1C2SC_MS2A_MASK              16
#define TPM1C2SC_MS2B_MASK              32
#define TPM1C2SC_CH2IE_MASK             64
#define TPM1C2SC_CH2F_MASK              128
#define TPM1C2SC_ELS2x_MASK             12
#define TPM1C2SC_ELS2x_BITNUM           2
#define TPM1C2SC_MS2x_MASK              48
#define TPM1C2SC_MS2x_BITNUM            4


/*** TPM1C2V - TPM1 Timer Channel 2 Value Register; 0x0000002C ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** TPM1C2VH - TPM1 Timer Channel 2 Value Register High; 0x0000002C ***/
    union {
      byte Byte;
    } TPM1C2VHSTR;
    #define TPM1C2VH                    _TPM1C2V.Overlap_STR.TPM1C2VHSTR.Byte
    

    /*** TPM1C2VL - TPM1 Timer Channel 2 Value Register Low; 0x0000002D ***/
    union {
      byte Byte;
    } TPM1C2VLSTR;
    #define TPM1C2VL                    _TPM1C2V.Overlap_STR.TPM1C2VLSTR.Byte
    
  } Overlap_STR;

} TPM1C2VSTR;
extern volatile TPM1C2VSTR _TPM1C2V @0x0000002C;
#define TPM1C2V                         _TPM1C2V.Word


/*** TPM1C3SC - TPM1 Timer Channel 3 Status and Control Register; 0x0000002E ***/
typedef union {
  byte Byte;
  struct {
    byte             :1; 
    byte             :1; 
    byte ELS3A       :1;                                       /* Edge/Level Select Bit A */
    byte ELS3B       :1;                                       /* Edge/Level Select Bit B */
    byte MS3A        :1;                                       /* Mode Select A for TPM Channel 3 */
    byte MS3B        :1;                                       /* Mode Select B for TPM Channel 3 */
    byte CH3IE       :1;                                       /* Channel 3 Interrupt Enable */
    byte CH3F        :1;                                       /* Channel 3 Flag */
  } Bits;
  struct {
    byte         :1;
    byte         :1;
    byte grpELS3x :2;
    byte grpMS3x :2;
    byte         :1;
    byte         :1;
  } MergedBits;
} TPM1C3SCSTR;
extern volatile TPM1C3SCSTR _TPM1C3SC @0x0000002E;
#define TPM1C3SC                        _TPM1C3SC.Byte
#define TPM1C3SC_ELS3A                  _TPM1C3SC.Bits.ELS3A
#define TPM1C3SC_ELS3B                  _TPM1C3SC.Bits.ELS3B
#define TPM1C3SC_MS3A                   _TPM1C3SC.Bits.MS3A
#define TPM1C3SC_MS3B                   _TPM1C3SC.Bits.MS3B
#define TPM1C3SC_CH3IE                  _TPM1C3SC.Bits.CH3IE
#define TPM1C3SC_CH3F                   _TPM1C3SC.Bits.CH3F
#define TPM1C3SC_ELS3x                  _TPM1C3SC.MergedBits.grpELS3x
#define TPM1C3SC_MS3x                   _TPM1C3SC.MergedBits.grpMS3x

#define TPM1C3SC_ELS3A_MASK             4
#define TPM1C3SC_ELS3B_MASK             8
#define TPM1C3SC_MS3A_MASK              16
#define TPM1C3SC_MS3B_MASK              32
#define TPM1C3SC_CH3IE_MASK             64
#define TPM1C3SC_CH3F_MASK              128
#define TPM1C3SC_ELS3x_MASK             12
#define TPM1C3SC_ELS3x_BITNUM           2
#define TPM1C3SC_MS3x_MASK              48
#define TPM1C3SC_MS3x_BITNUM            4


/*** TPM1C3V - TPM1 Timer Channel 3 Value Register; 0x0000002F ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** TPM1C3VH - TPM1 Timer Channel 3 Value Register High; 0x0000002F ***/
    union {
      byte Byte;
    } TPM1C3VHSTR;
    #define TPM1C3VH                    _TPM1C3V.Overlap_STR.TPM1C3VHSTR.Byte
    

    /*** TPM1C3VL - TPM1 Timer Channel 3 Value Register Low; 0x00000030 ***/
    union {
      byte Byte;
    } TPM1C3VLSTR;
    #define TPM1C3VL                    _TPM1C3V.Overlap_STR.TPM1C3VLSTR.Byte
    
  } Overlap_STR;

} TPM1C3VSTR;
extern volatile TPM1C3VSTR _TPM1C3V @0x0000002F;
#define TPM1C3V                         _TPM1C3V.Word


/*** TPM1C4SC - TPM1 Timer Channel 4 Status and Control Register; 0x00000031 ***/
typedef union {
  byte Byte;
  struct {
    byte             :1; 
    byte             :1; 
    byte ELS4A       :1;                                       /* Edge/Level Select Bit A */
    byte ELS4B       :1;                                       /* Edge/Level Select Bit B */
    byte MS4A        :1;                                       /* Mode Select A for TPM Channel 4 */
    byte MS4B        :1;                                       /* Mode Select B for TPM Channel 4 */
    byte CH4IE       :1;                                       /* Channel 4 Interrupt Enable */
    byte CH4F        :1;                                       /* Channel 4 Flag */
  } Bits;
  struct {
    byte         :1;
    byte         :1;
    byte grpELS4x :2;
    byte grpMS4x :2;
    byte         :1;
    byte         :1;
  } MergedBits;
} TPM1C4SCSTR;
extern volatile TPM1C4SCSTR _TPM1C4SC @0x00000031;
#define TPM1C4SC                        _TPM1C4SC.Byte
#define TPM1C4SC_ELS4A                  _TPM1C4SC.Bits.ELS4A
#define TPM1C4SC_ELS4B                  _TPM1C4SC.Bits.ELS4B
#define TPM1C4SC_MS4A                   _TPM1C4SC.Bits.MS4A
#define TPM1C4SC_MS4B                   _TPM1C4SC.Bits.MS4B
#define TPM1C4SC_CH4IE                  _TPM1C4SC.Bits.CH4IE
#define TPM1C4SC_CH4F                   _TPM1C4SC.Bits.CH4F
#define TPM1C4SC_ELS4x                  _TPM1C4SC.MergedBits.grpELS4x
#define TPM1C4SC_MS4x                   _TPM1C4SC.MergedBits.grpMS4x

#define TPM1C4SC_ELS4A_MASK             4
#define TPM1C4SC_ELS4B_MASK             8
#define TPM1C4SC_MS4A_MASK              16
#define TPM1C4SC_MS4B_MASK              32
#define TPM1C4SC_CH4IE_MASK             64
#define TPM1C4SC_CH4F_MASK              128
#define TPM1C4SC_ELS4x_MASK             12
#define TPM1C4SC_ELS4x_BITNUM           2
#define TPM1C4SC_MS4x_MASK              48
#define TPM1C4SC_MS4x_BITNUM            4


/*** TPM1C4V - TPM1 Timer Channel 4 Value Register; 0x00000032 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** TPM1C4VH - TPM1 Timer Channel 4 Value Register High; 0x00000032 ***/
    union {
      byte Byte;
    } TPM1C4VHSTR;
    #define TPM1C4VH                    _TPM1C4V.Overlap_STR.TPM1C4VHSTR.Byte
    

    /*** TPM1C4VL - TPM1 Timer Channel 4 Value Register Low; 0x00000033 ***/
    union {
      byte Byte;
    } TPM1C4VLSTR;
    #define TPM1C4VL                    _TPM1C4V.Overlap_STR.TPM1C4VLSTR.Byte
    
  } Overlap_STR;

} TPM1C4VSTR;
extern volatile TPM1C4VSTR _TPM1C4V @0x00000032;
#define TPM1C4V                         _TPM1C4V.Word


/*** TPM1C5SC - TPM1 Timer Channel 5 Status and Control Register; 0x00000034 ***/
typedef union {
  byte Byte;
  struct {
    byte             :1; 
    byte             :1; 
    byte ELS5A       :1;                                       /* Edge/Level Select Bit A */
    byte ELS5B       :1;                                       /* Edge/Level Select Bit B */
    byte MS5A        :1;                                       /* Mode Select A for TPM Channel 5 */
    byte MS5B        :1;                                       /* Mode Select B for TPM Channel 5 */
    byte CH5IE       :1;                                       /* Channel 5 Interrupt Enable */
    byte CH5F        :1;                                       /* Channel 5 Flag */
  } Bits;
  struct {
    byte         :1;
    byte         :1;
    byte grpELS5x :2;
    byte grpMS5x :2;
    byte         :1;
    byte         :1;
  } MergedBits;
} TPM1C5SCSTR;
extern volatile TPM1C5SCSTR _TPM1C5SC @0x00000034;
#define TPM1C5SC                        _TPM1C5SC.Byte
#define TPM1C5SC_ELS5A                  _TPM1C5SC.Bits.ELS5A
#define TPM1C5SC_ELS5B                  _TPM1C5SC.Bits.ELS5B
#define TPM1C5SC_MS5A                   _TPM1C5SC.Bits.MS5A
#define TPM1C5SC_MS5B                   _TPM1C5SC.Bits.MS5B
#define TPM1C5SC_CH5IE                  _TPM1C5SC.Bits.CH5IE
#define TPM1C5SC_CH5F                   _TPM1C5SC.Bits.CH5F
#define TPM1C5SC_ELS5x                  _TPM1C5SC.MergedBits.grpELS5x
#define TPM1C5SC_MS5x                   _TPM1C5SC.MergedBits.grpMS5x

#define TPM1C5SC_ELS5A_MASK             4
#define TPM1C5SC_ELS5B_MASK             8
#define TPM1C5SC_MS5A_MASK              16
#define TPM1C5SC_MS5B_MASK              32
#define TPM1C5SC_CH5IE_MASK             64
#define TPM1C5SC_CH5F_MASK              128
#define TPM1C5SC_ELS5x_MASK             12
#define TPM1C5SC_ELS5x_BITNUM           2
#define TPM1C5SC_MS5x_MASK              48
#define TPM1C5SC_MS5x_BITNUM            4


/*** TPM1C5V - TPM1 Timer Channel 5 Value Register; 0x00000035 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** TPM1C5VH - TPM1 Timer Channel 5 Value Register High; 0x00000035 ***/
    union {
      byte Byte;
    } TPM1C5VHSTR;
    #define TPM1C5VH                    _TPM1C5V.Overlap_STR.TPM1C5VHSTR.Byte
    

    /*** TPM1C5VL - TPM1 Timer Channel 5 Value Register Low; 0x00000036 ***/
    union {
      byte Byte;
    } TPM1C5VLSTR;
    #define TPM1C5VL                    _TPM1C5V.Overlap_STR.TPM1C5VLSTR.Byte
    
  } Overlap_STR;

} TPM1C5VSTR;
extern volatile TPM1C5VSTR _TPM1C5V @0x00000035;
#define TPM1C5V                         _TPM1C5V.Word


/*** SCI1BD - SCI1 Baud Rate Register; 0x00000038 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** SCI1BDH - SCI1 Baud Rate Register High; 0x00000038 ***/
    union {
      byte Byte;
      struct {
        byte SBR8        :1;                                       /* Baud Rate Modulo Divisor Bit 8 */
        byte SBR9        :1;                                       /* Baud Rate Modulo Divisor Bit 9 */
        byte SBR10       :1;                                       /* Baud Rate Modulo Divisor Bit 10 */
        byte SBR11       :1;                                       /* Baud Rate Modulo Divisor Bit 11 */
        byte SBR12       :1;                                       /* Baud Rate Modulo Divisor Bit 12 */
        byte             :1; 
        byte RXEDGIE     :1;                                       /* RxD Input Active Edge Interrupt Enable (for RXEDGIF) */
        byte LBKDIE      :1;                                       /* LIN Break Detect Interrupt Enable (for LBKDIF) */
      } Bits;
      struct {
        byte grpSBR_8 :5;
        byte     :1;
        byte     :1;
        byte     :1;
      } MergedBits;
    } SCI1BDHSTR;
    #define SCI1BDH                     _SCI1BD.Overlap_STR.SCI1BDHSTR.Byte
    #define SCI1BDH_SBR8                _SCI1BD.Overlap_STR.SCI1BDHSTR.Bits.SBR8
    #define SCI1BDH_SBR9                _SCI1BD.Overlap_STR.SCI1BDHSTR.Bits.SBR9
    #define SCI1BDH_SBR10               _SCI1BD.Overlap_STR.SCI1BDHSTR.Bits.SBR10
    #define SCI1BDH_SBR11               _SCI1BD.Overlap_STR.SCI1BDHSTR.Bits.SBR11
    #define SCI1BDH_SBR12               _SCI1BD.Overlap_STR.SCI1BDHSTR.Bits.SBR12
    #define SCI1BDH_RXEDGIE             _SCI1BD.Overlap_STR.SCI1BDHSTR.Bits.RXEDGIE
    #define SCI1BDH_LBKDIE              _SCI1BD.Overlap_STR.SCI1BDHSTR.Bits.LBKDIE
    #define SCI1BDH_SBR_8               _SCI1BD.Overlap_STR.SCI1BDHSTR.MergedBits.grpSBR_8
    #define SCI1BDH_SBR                 SCI1BDH_SBR_8
    
    #define SCI1BDH_SBR8_MASK           1
    #define SCI1BDH_SBR9_MASK           2
    #define SCI1BDH_SBR10_MASK          4
    #define SCI1BDH_SBR11_MASK          8
    #define SCI1BDH_SBR12_MASK          16
    #define SCI1BDH_RXEDGIE_MASK        64
    #define SCI1BDH_LBKDIE_MASK         128
    #define SCI1BDH_SBR_8_MASK          31
    #define SCI1BDH_SBR_8_BITNUM        0
    

    /*** SCI1BDL - SCI1 Baud Rate Register Low; 0x00000039 ***/
    union {
      byte Byte;
      struct {
        byte SBR0        :1;                                       /* Baud Rate Modulo Divisor Bit 0 */
        byte SBR1        :1;                                       /* Baud Rate Modulo Divisor Bit 1 */
        byte SBR2        :1;                                       /* Baud Rate Modulo Divisor Bit 2 */
        byte SBR3        :1;                                       /* Baud Rate Modulo Divisor Bit 3 */
        byte SBR4        :1;                                       /* Baud Rate Modulo Divisor Bit 4 */
        byte SBR5        :1;                                       /* Baud Rate Modulo Divisor Bit 5 */
        byte SBR6        :1;                                       /* Baud Rate Modulo Divisor Bit 6 */
        byte SBR7        :1;                                       /* Baud Rate Modulo Divisor Bit 7 */
      } Bits;
    } SCI1BDLSTR;
    #define SCI1BDL                     _SCI1BD.Overlap_STR.SCI1BDLSTR.Byte
    #define SCI1BDL_SBR0                _SCI1BD.Overlap_STR.SCI1BDLSTR.Bits.SBR0
    #define SCI1BDL_SBR1                _SCI1BD.Overlap_STR.SCI1BDLSTR.Bits.SBR1
    #define SCI1BDL_SBR2                _SCI1BD.Overlap_STR.SCI1BDLSTR.Bits.SBR2
    #define SCI1BDL_SBR3                _SCI1BD.Overlap_STR.SCI1BDLSTR.Bits.SBR3
    #define SCI1BDL_SBR4                _SCI1BD.Overlap_STR.SCI1BDLSTR.Bits.SBR4
    #define SCI1BDL_SBR5                _SCI1BD.Overlap_STR.SCI1BDLSTR.Bits.SBR5
    #define SCI1BDL_SBR6                _SCI1BD.Overlap_STR.SCI1BDLSTR.Bits.SBR6
    #define SCI1BDL_SBR7                _SCI1BD.Overlap_STR.SCI1BDLSTR.Bits.SBR7
    
    #define SCI1BDL_SBR0_MASK           1
    #define SCI1BDL_SBR1_MASK           2
    #define SCI1BDL_SBR2_MASK           4
    #define SCI1BDL_SBR3_MASK           8
    #define SCI1BDL_SBR4_MASK           16
    #define SCI1BDL_SBR5_MASK           32
    #define SCI1BDL_SBR6_MASK           64
    #define SCI1BDL_SBR7_MASK           128
    
  } Overlap_STR;

} SCI1BDSTR;
extern volatile SCI1BDSTR _SCI1BD @0x00000038;
#define SCI1BD                          _SCI1BD.Word


/*** SCI1C1 - SCI1 Control Register 1; 0x0000003A ***/
typedef union {
  byte Byte;
  struct {
    byte PT          :1;                                       /* Parity Type */
    byte PE          :1;                                       /* Parity Enable */
    byte ILT         :1;                                       /* Idle Line Type Select */
    byte WAKE        :1;                                       /* Receiver Wakeup Method Select */
    byte M           :1;                                       /* 9-Bit or 8-Bit Mode Select */
    byte RSRC        :1;                                       /* Receiver Source Select */
    byte SCISWAI     :1;                                       /* SCI Stops in Wait Mode */
    byte LOOPS       :1;                                       /* Loop Mode Select */
  } Bits;
} SCI1C1STR;
extern volatile SCI1C1STR _SCI1C1 @0x0000003A;
#define SCI1C1                          _SCI1C1.Byte
#define SCI1C1_PT                       _SCI1C1.Bits.PT
#define SCI1C1_PE                       _SCI1C1.Bits.PE
#define SCI1C1_ILT                      _SCI1C1.Bits.ILT
#define SCI1C1_WAKE                     _SCI1C1.Bits.WAKE
#define SCI1C1_M                        _SCI1C1.Bits.M
#define SCI1C1_RSRC                     _SCI1C1.Bits.RSRC
#define SCI1C1_SCISWAI                  _SCI1C1.Bits.SCISWAI
#define SCI1C1_LOOPS                    _SCI1C1.Bits.LOOPS

#define SCI1C1_PT_MASK                  1
#define SCI1C1_PE_MASK                  2
#define SCI1C1_ILT_MASK                 4
#define SCI1C1_WAKE_MASK                8
#define SCI1C1_M_MASK                   16
#define SCI1C1_RSRC_MASK                32
#define SCI1C1_SCISWAI_MASK             64
#define SCI1C1_LOOPS_MASK               128


/*** SCI1C2 - SCI1 Control Register 2; 0x0000003B ***/
typedef union {
  byte Byte;
  struct {
    byte SBK         :1;                                       /* Send Break */
    byte RWU         :1;                                       /* Receiver Wakeup Control */
    byte RE          :1;                                       /* Receiver Enable */
    byte TE          :1;                                       /* Transmitter Enable */
    byte ILIE        :1;                                       /* Idle Line Interrupt Enable (for IDLE) */
    byte RIE         :1;                                       /* Receiver Interrupt Enable (for RDRF) */
    byte TCIE        :1;                                       /* Transmission Complete Interrupt Enable (for TC) */
    byte TIE         :1;                                       /* Transmit Interrupt Enable (for TDRE) */
  } Bits;
} SCI1C2STR;
extern volatile SCI1C2STR _SCI1C2 @0x0000003B;
#define SCI1C2                          _SCI1C2.Byte
#define SCI1C2_SBK                      _SCI1C2.Bits.SBK
#define SCI1C2_RWU                      _SCI1C2.Bits.RWU
#define SCI1C2_RE                       _SCI1C2.Bits.RE
#define SCI1C2_TE                       _SCI1C2.Bits.TE
#define SCI1C2_ILIE                     _SCI1C2.Bits.ILIE
#define SCI1C2_RIE                      _SCI1C2.Bits.RIE
#define SCI1C2_TCIE                     _SCI1C2.Bits.TCIE
#define SCI1C2_TIE                      _SCI1C2.Bits.TIE

#define SCI1C2_SBK_MASK                 1
#define SCI1C2_RWU_MASK                 2
#define SCI1C2_RE_MASK                  4
#define SCI1C2_TE_MASK                  8
#define SCI1C2_ILIE_MASK                16
#define SCI1C2_RIE_MASK                 32
#define SCI1C2_TCIE_MASK                64
#define SCI1C2_TIE_MASK                 128


/*** SCI1S1 - SCI1 Status Register 1; 0x0000003C ***/
typedef union {
  byte Byte;
  struct {
    byte PF          :1;                                       /* Parity Error Flag */
    byte FE          :1;                                       /* Framing Error Flag */
    byte NF          :1;                                       /* Noise Flag */
    byte OR          :1;                                       /* Receiver Overrun Flag */
    byte IDLE        :1;                                       /* Idle Line Flag */
    byte RDRF        :1;                                       /* Receive Data Register Full Flag */
    byte TC          :1;                                       /* Transmission Complete Flag */
    byte TDRE        :1;                                       /* Transmit Data Register Empty Flag */
  } Bits;
} SCI1S1STR;
extern volatile SCI1S1STR _SCI1S1 @0x0000003C;
#define SCI1S1                          _SCI1S1.Byte
#define SCI1S1_PF                       _SCI1S1.Bits.PF
#define SCI1S1_FE                       _SCI1S1.Bits.FE
#define SCI1S1_NF                       _SCI1S1.Bits.NF
#define SCI1S1_OR                       _SCI1S1.Bits.OR
#define SCI1S1_IDLE                     _SCI1S1.Bits.IDLE
#define SCI1S1_RDRF                     _SCI1S1.Bits.RDRF
#define SCI1S1_TC                       _SCI1S1.Bits.TC
#define SCI1S1_TDRE                     _SCI1S1.Bits.TDRE

#define SCI1S1_PF_MASK                  1
#define SCI1S1_FE_MASK                  2
#define SCI1S1_NF_MASK                  4
#define SCI1S1_OR_MASK                  8
#define SCI1S1_IDLE_MASK                16
#define SCI1S1_RDRF_MASK                32
#define SCI1S1_TC_MASK                  64
#define SCI1S1_TDRE_MASK                128


/*** SCI1S2 - SCI1 Status Register 2; 0x0000003D ***/
typedef union {
  byte Byte;
  struct {
    byte RAF         :1;                                       /* Receiver Active Flag */
    byte LBKDE       :1;                                       /* LIN Break Detection Enable */
    byte BRK13       :1;                                       /* Break Character Generation Length */
    byte RWUID       :1;                                       /* Receive Wake Up Idle Detect */
    byte RXINV       :1;                                       /* Receive Data Inversion */
    byte             :1; 
    byte RXEDGIF     :1;                                       /* RxD Pin Active Edge Interrupt Flag */
    byte LBKDIF      :1;                                       /* LIN Break Detect Interrupt Flag */
  } Bits;
} SCI1S2STR;
extern volatile SCI1S2STR _SCI1S2 @0x0000003D;
#define SCI1S2                          _SCI1S2.Byte
#define SCI1S2_RAF                      _SCI1S2.Bits.RAF
#define SCI1S2_LBKDE                    _SCI1S2.Bits.LBKDE
#define SCI1S2_BRK13                    _SCI1S2.Bits.BRK13
#define SCI1S2_RWUID                    _SCI1S2.Bits.RWUID
#define SCI1S2_RXINV                    _SCI1S2.Bits.RXINV
#define SCI1S2_RXEDGIF                  _SCI1S2.Bits.RXEDGIF
#define SCI1S2_LBKDIF                   _SCI1S2.Bits.LBKDIF

#define SCI1S2_RAF_MASK                 1
#define SCI1S2_LBKDE_MASK               2
#define SCI1S2_BRK13_MASK               4
#define SCI1S2_RWUID_MASK               8
#define SCI1S2_RXINV_MASK               16
#define SCI1S2_RXEDGIF_MASK             64
#define SCI1S2_LBKDIF_MASK              128


/*** SCI1C3 - SCI1 Control Register 3; 0x0000003E ***/
typedef union {
  byte Byte;
  struct {
    byte PEIE        :1;                                       /* Parity Error Interrupt Enable */
    byte FEIE        :1;                                       /* Framing Error Interrupt Enable */
    byte NEIE        :1;                                       /* Noise Error Interrupt Enable */
    byte ORIE        :1;                                       /* Overrun Interrupt Enable */
    byte TXINV       :1;                                       /* Transmit Data Inversion */
    byte TXDIR       :1;                                       /* TxD Pin Direction in Single-Wire Mode */
    byte T8          :1;                                       /* Ninth Data Bit for Transmitter */
    byte R8          :1;                                       /* Ninth Data Bit for Receiver */
  } Bits;
} SCI1C3STR;
extern volatile SCI1C3STR _SCI1C3 @0x0000003E;
#define SCI1C3                          _SCI1C3.Byte
#define SCI1C3_PEIE                     _SCI1C3.Bits.PEIE
#define SCI1C3_FEIE                     _SCI1C3.Bits.FEIE
#define SCI1C3_NEIE                     _SCI1C3.Bits.NEIE
#define SCI1C3_ORIE                     _SCI1C3.Bits.ORIE
#define SCI1C3_TXINV                    _SCI1C3.Bits.TXINV
#define SCI1C3_TXDIR                    _SCI1C3.Bits.TXDIR
#define SCI1C3_T8                       _SCI1C3.Bits.T8
#define SCI1C3_R8                       _SCI1C3.Bits.R8

#define SCI1C3_PEIE_MASK                1
#define SCI1C3_FEIE_MASK                2
#define SCI1C3_NEIE_MASK                4
#define SCI1C3_ORIE_MASK                8
#define SCI1C3_TXINV_MASK               16
#define SCI1C3_TXDIR_MASK               32
#define SCI1C3_T8_MASK                  64
#define SCI1C3_R8_MASK                  128


/*** SCI1D - SCI1 Data Register; 0x0000003F ***/
typedef union {
  byte Byte;
  struct {
    byte R0_T0       :1;                                       /* Receive/Transmit Data Bit 0 */
    byte R1_T1       :1;                                       /* Receive/Transmit Data Bit 1 */
    byte R2_T2       :1;                                       /* Receive/Transmit Data Bit 2 */
    byte R3_T3       :1;                                       /* Receive/Transmit Data Bit 3 */
    byte R4_T4       :1;                                       /* Receive/Transmit Data Bit 4 */
    byte R5_T5       :1;                                       /* Receive/Transmit Data Bit 5 */
    byte R6_T6       :1;                                       /* Receive/Transmit Data Bit 6 */
    byte R7_T7       :1;                                       /* Receive/Transmit Data Bit 7 */
  } Bits;
} SCI1DSTR;
extern volatile SCI1DSTR _SCI1D @0x0000003F;
#define SCI1D                           _SCI1D.Byte
#define SCI1D_R0_T0                     _SCI1D.Bits.R0_T0
#define SCI1D_R1_T1                     _SCI1D.Bits.R1_T1
#define SCI1D_R2_T2                     _SCI1D.Bits.R2_T2
#define SCI1D_R3_T3                     _SCI1D.Bits.R3_T3
#define SCI1D_R4_T4                     _SCI1D.Bits.R4_T4
#define SCI1D_R5_T5                     _SCI1D.Bits.R5_T5
#define SCI1D_R6_T6                     _SCI1D.Bits.R6_T6
#define SCI1D_R7_T7                     _SCI1D.Bits.R7_T7

#define SCI1D_R0_T0_MASK                1
#define SCI1D_R1_T1_MASK                2
#define SCI1D_R2_T2_MASK                4
#define SCI1D_R3_T3_MASK                8
#define SCI1D_R4_T4_MASK                16
#define SCI1D_R5_T5_MASK                32
#define SCI1D_R6_T6_MASK                64
#define SCI1D_R7_T7_MASK                128


/*** SCI2BD - SCI2 Baud Rate Register; 0x00000040 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** SCI2BDH - SCI2 Baud Rate Register High; 0x00000040 ***/
    union {
      byte Byte;
      struct {
        byte SBR8        :1;                                       /* Baud Rate Modulo Divisor Bit 8 */
        byte SBR9        :1;                                       /* Baud Rate Modulo Divisor Bit 9 */
        byte SBR10       :1;                                       /* Baud Rate Modulo Divisor Bit 10 */
        byte SBR11       :1;                                       /* Baud Rate Modulo Divisor Bit 11 */
        byte SBR12       :1;                                       /* Baud Rate Modulo Divisor Bit 12 */
        byte             :1; 
        byte RXEDGIE     :1;                                       /* RxD Input Active Edge Interrupt Enable (for RXEDGIF) */
        byte LBKDIE      :1;                                       /* LIN Break Detect Interrupt Enable (for LBKDIF) */
      } Bits;
      struct {
        byte grpSBR_8 :5;
        byte     :1;
        byte     :1;
        byte     :1;
      } MergedBits;
    } SCI2BDHSTR;
    #define SCI2BDH                     _SCI2BD.Overlap_STR.SCI2BDHSTR.Byte
    #define SCI2BDH_SBR8                _SCI2BD.Overlap_STR.SCI2BDHSTR.Bits.SBR8
    #define SCI2BDH_SBR9                _SCI2BD.Overlap_STR.SCI2BDHSTR.Bits.SBR9
    #define SCI2BDH_SBR10               _SCI2BD.Overlap_STR.SCI2BDHSTR.Bits.SBR10
    #define SCI2BDH_SBR11               _SCI2BD.Overlap_STR.SCI2BDHSTR.Bits.SBR11
    #define SCI2BDH_SBR12               _SCI2BD.Overlap_STR.SCI2BDHSTR.Bits.SBR12
    #define SCI2BDH_RXEDGIE             _SCI2BD.Overlap_STR.SCI2BDHSTR.Bits.RXEDGIE
    #define SCI2BDH_LBKDIE              _SCI2BD.Overlap_STR.SCI2BDHSTR.Bits.LBKDIE
    #define SCI2BDH_SBR_8               _SCI2BD.Overlap_STR.SCI2BDHSTR.MergedBits.grpSBR_8
    #define SCI2BDH_SBR                 SCI2BDH_SBR_8
    
    #define SCI2BDH_SBR8_MASK           1
    #define SCI2BDH_SBR9_MASK           2
    #define SCI2BDH_SBR10_MASK          4
    #define SCI2BDH_SBR11_MASK          8
    #define SCI2BDH_SBR12_MASK          16
    #define SCI2BDH_RXEDGIE_MASK        64
    #define SCI2BDH_LBKDIE_MASK         128
    #define SCI2BDH_SBR_8_MASK          31
    #define SCI2BDH_SBR_8_BITNUM        0
    

    /*** SCI2BDL - SCI2 Baud Rate Register Low; 0x00000041 ***/
    union {
      byte Byte;
      struct {
        byte SBR0        :1;                                       /* Baud Rate Modulo Divisor Bit 0 */
        byte SBR1        :1;                                       /* Baud Rate Modulo Divisor Bit 1 */
        byte SBR2        :1;                                       /* Baud Rate Modulo Divisor Bit 2 */
        byte SBR3        :1;                                       /* Baud Rate Modulo Divisor Bit 3 */
        byte SBR4        :1;                                       /* Baud Rate Modulo Divisor Bit 4 */
        byte SBR5        :1;                                       /* Baud Rate Modulo Divisor Bit 5 */
        byte SBR6        :1;                                       /* Baud Rate Modulo Divisor Bit 6 */
        byte SBR7        :1;                                       /* Baud Rate Modulo Divisor Bit 7 */
      } Bits;
    } SCI2BDLSTR;
    #define SCI2BDL                     _SCI2BD.Overlap_STR.SCI2BDLSTR.Byte
    #define SCI2BDL_SBR0                _SCI2BD.Overlap_STR.SCI2BDLSTR.Bits.SBR0
    #define SCI2BDL_SBR1                _SCI2BD.Overlap_STR.SCI2BDLSTR.Bits.SBR1
    #define SCI2BDL_SBR2                _SCI2BD.Overlap_STR.SCI2BDLSTR.Bits.SBR2
    #define SCI2BDL_SBR3                _SCI2BD.Overlap_STR.SCI2BDLSTR.Bits.SBR3
    #define SCI2BDL_SBR4                _SCI2BD.Overlap_STR.SCI2BDLSTR.Bits.SBR4
    #define SCI2BDL_SBR5                _SCI2BD.Overlap_STR.SCI2BDLSTR.Bits.SBR5
    #define SCI2BDL_SBR6                _SCI2BD.Overlap_STR.SCI2BDLSTR.Bits.SBR6
    #define SCI2BDL_SBR7                _SCI2BD.Overlap_STR.SCI2BDLSTR.Bits.SBR7
    
    #define SCI2BDL_SBR0_MASK           1
    #define SCI2BDL_SBR1_MASK           2
    #define SCI2BDL_SBR2_MASK           4
    #define SCI2BDL_SBR3_MASK           8
    #define SCI2BDL_SBR4_MASK           16
    #define SCI2BDL_SBR5_MASK           32
    #define SCI2BDL_SBR6_MASK           64
    #define SCI2BDL_SBR7_MASK           128
    
  } Overlap_STR;

} SCI2BDSTR;
extern volatile SCI2BDSTR _SCI2BD @0x00000040;
#define SCI2BD                          _SCI2BD.Word


/*** SCI2C1 - SCI2 Control Register 1; 0x00000042 ***/
typedef union {
  byte Byte;
  struct {
    byte PT          :1;                                       /* Parity Type */
    byte PE          :1;                                       /* Parity Enable */
    byte ILT         :1;                                       /* Idle Line Type Select */
    byte WAKE        :1;                                       /* Receiver Wakeup Method Select */
    byte M           :1;                                       /* 9-Bit or 8-Bit Mode Select */
    byte RSRC        :1;                                       /* Receiver Source Select */
    byte SCISWAI     :1;                                       /* SCI Stops in Wait Mode */
    byte LOOPS       :1;                                       /* Loop Mode Select */
  } Bits;
} SCI2C1STR;
extern volatile SCI2C1STR _SCI2C1 @0x00000042;
#define SCI2C1                          _SCI2C1.Byte
#define SCI2C1_PT                       _SCI2C1.Bits.PT
#define SCI2C1_PE                       _SCI2C1.Bits.PE
#define SCI2C1_ILT                      _SCI2C1.Bits.ILT
#define SCI2C1_WAKE                     _SCI2C1.Bits.WAKE
#define SCI2C1_M                        _SCI2C1.Bits.M
#define SCI2C1_RSRC                     _SCI2C1.Bits.RSRC
#define SCI2C1_SCISWAI                  _SCI2C1.Bits.SCISWAI
#define SCI2C1_LOOPS                    _SCI2C1.Bits.LOOPS

#define SCI2C1_PT_MASK                  1
#define SCI2C1_PE_MASK                  2
#define SCI2C1_ILT_MASK                 4
#define SCI2C1_WAKE_MASK                8
#define SCI2C1_M_MASK                   16
#define SCI2C1_RSRC_MASK                32
#define SCI2C1_SCISWAI_MASK             64
#define SCI2C1_LOOPS_MASK               128


/*** SCI2C2 - SCI2 Control Register 2; 0x00000043 ***/
typedef union {
  byte Byte;
  struct {
    byte SBK         :1;                                       /* Send Break */
    byte RWU         :1;                                       /* Receiver Wakeup Control */
    byte RE          :1;                                       /* Receiver Enable */
    byte TE          :1;                                       /* Transmitter Enable */
    byte ILIE        :1;                                       /* Idle Line Interrupt Enable (for IDLE) */
    byte RIE         :1;                                       /* Receiver Interrupt Enable (for RDRF) */
    byte TCIE        :1;                                       /* Transmission Complete Interrupt Enable (for TC) */
    byte TIE         :1;                                       /* Transmit Interrupt Enable (for TDRE) */
  } Bits;
} SCI2C2STR;
extern volatile SCI2C2STR _SCI2C2 @0x00000043;
#define SCI2C2                          _SCI2C2.Byte
#define SCI2C2_SBK                      _SCI2C2.Bits.SBK
#define SCI2C2_RWU                      _SCI2C2.Bits.RWU
#define SCI2C2_RE                       _SCI2C2.Bits.RE
#define SCI2C2_TE                       _SCI2C2.Bits.TE
#define SCI2C2_ILIE                     _SCI2C2.Bits.ILIE
#define SCI2C2_RIE                      _SCI2C2.Bits.RIE
#define SCI2C2_TCIE                     _SCI2C2.Bits.TCIE
#define SCI2C2_TIE                      _SCI2C2.Bits.TIE

#define SCI2C2_SBK_MASK                 1
#define SCI2C2_RWU_MASK                 2
#define SCI2C2_RE_MASK                  4
#define SCI2C2_TE_MASK                  8
#define SCI2C2_ILIE_MASK                16
#define SCI2C2_RIE_MASK                 32
#define SCI2C2_TCIE_MASK                64
#define SCI2C2_TIE_MASK                 128


/*** SCI2S1 - SCI2 Status Register 1; 0x00000044 ***/
typedef union {
  byte Byte;
  struct {
    byte PF          :1;                                       /* Parity Error Flag */
    byte FE          :1;                                       /* Framing Error Flag */
    byte NF          :1;                                       /* Noise Flag */
    byte OR          :1;                                       /* Receiver Overrun Flag */
    byte IDLE        :1;                                       /* Idle Line Flag */
    byte RDRF        :1;                                       /* Receive Data Register Full Flag */
    byte TC          :1;                                       /* Transmission Complete Flag */
    byte TDRE        :1;                                       /* Transmit Data Register Empty Flag */
  } Bits;
} SCI2S1STR;
extern volatile SCI2S1STR _SCI2S1 @0x00000044;
#define SCI2S1                          _SCI2S1.Byte
#define SCI2S1_PF                       _SCI2S1.Bits.PF
#define SCI2S1_FE                       _SCI2S1.Bits.FE
#define SCI2S1_NF                       _SCI2S1.Bits.NF
#define SCI2S1_OR                       _SCI2S1.Bits.OR
#define SCI2S1_IDLE                     _SCI2S1.Bits.IDLE
#define SCI2S1_RDRF                     _SCI2S1.Bits.RDRF
#define SCI2S1_TC                       _SCI2S1.Bits.TC
#define SCI2S1_TDRE                     _SCI2S1.Bits.TDRE

#define SCI2S1_PF_MASK                  1
#define SCI2S1_FE_MASK                  2
#define SCI2S1_NF_MASK                  4
#define SCI2S1_OR_MASK                  8
#define SCI2S1_IDLE_MASK                16
#define SCI2S1_RDRF_MASK                32
#define SCI2S1_TC_MASK                  64
#define SCI2S1_TDRE_MASK                128


/*** SCI2S2 - SCI2 Status Register 2; 0x00000045 ***/
typedef union {
  byte Byte;
  struct {
    byte RAF         :1;                                       /* Receiver Active Flag */
    byte LBKDE       :1;                                       /* LIN Break Detection Enable */
    byte BRK13       :1;                                       /* Break Character Generation Length */
    byte RWUID       :1;                                       /* Receive Wake Up Idle Detect */
    byte RXINV       :1;                                       /* Receive Data Inversion */
    byte             :1; 
    byte RXEDGIF     :1;                                       /* RxD Pin Active Edge Interrupt Flag */
    byte LBKDIF      :1;                                       /* LIN Break Detect Interrupt Flag */
  } Bits;
} SCI2S2STR;
extern volatile SCI2S2STR _SCI2S2 @0x00000045;
#define SCI2S2                          _SCI2S2.Byte
#define SCI2S2_RAF                      _SCI2S2.Bits.RAF
#define SCI2S2_LBKDE                    _SCI2S2.Bits.LBKDE
#define SCI2S2_BRK13                    _SCI2S2.Bits.BRK13
#define SCI2S2_RWUID                    _SCI2S2.Bits.RWUID
#define SCI2S2_RXINV                    _SCI2S2.Bits.RXINV
#define SCI2S2_RXEDGIF                  _SCI2S2.Bits.RXEDGIF
#define SCI2S2_LBKDIF                   _SCI2S2.Bits.LBKDIF

#define SCI2S2_RAF_MASK                 1
#define SCI2S2_LBKDE_MASK               2
#define SCI2S2_BRK13_MASK               4
#define SCI2S2_RWUID_MASK               8
#define SCI2S2_RXINV_MASK               16
#define SCI2S2_RXEDGIF_MASK             64
#define SCI2S2_LBKDIF_MASK              128


/*** SCI2C3 - SCI2 Control Register 3; 0x00000046 ***/
typedef union {
  byte Byte;
  struct {
    byte PEIE        :1;                                       /* Parity Error Interrupt Enable */
    byte FEIE        :1;                                       /* Framing Error Interrupt Enable */
    byte NEIE        :1;                                       /* Noise Error Interrupt Enable */
    byte ORIE        :1;                                       /* Overrun Interrupt Enable */
    byte TXINV       :1;                                       /* Transmit Data Inversion */
    byte TXDIR       :1;                                       /* TxD Pin Direction in Single-Wire Mode */
    byte T8          :1;                                       /* Ninth Data Bit for Transmitter */
    byte R8          :1;                                       /* Ninth Data Bit for Receiver */
  } Bits;
} SCI2C3STR;
extern volatile SCI2C3STR _SCI2C3 @0x00000046;
#define SCI2C3                          _SCI2C3.Byte
#define SCI2C3_PEIE                     _SCI2C3.Bits.PEIE
#define SCI2C3_FEIE                     _SCI2C3.Bits.FEIE
#define SCI2C3_NEIE                     _SCI2C3.Bits.NEIE
#define SCI2C3_ORIE                     _SCI2C3.Bits.ORIE
#define SCI2C3_TXINV                    _SCI2C3.Bits.TXINV
#define SCI2C3_TXDIR                    _SCI2C3.Bits.TXDIR
#define SCI2C3_T8                       _SCI2C3.Bits.T8
#define SCI2C3_R8                       _SCI2C3.Bits.R8

#define SCI2C3_PEIE_MASK                1
#define SCI2C3_FEIE_MASK                2
#define SCI2C3_NEIE_MASK                4
#define SCI2C3_ORIE_MASK                8
#define SCI2C3_TXINV_MASK               16
#define SCI2C3_TXDIR_MASK               32
#define SCI2C3_T8_MASK                  64
#define SCI2C3_R8_MASK                  128


/*** SCI2D - SCI2 Data Register; 0x00000047 ***/
typedef union {
  byte Byte;
  struct {
    byte R0_T0       :1;                                       /* Receive/Transmit Data Bit 0 */
    byte R1_T1       :1;                                       /* Receive/Transmit Data Bit 1 */
    byte R2_T2       :1;                                       /* Receive/Transmit Data Bit 2 */
    byte R3_T3       :1;                                       /* Receive/Transmit Data Bit 3 */
    byte R4_T4       :1;                                       /* Receive/Transmit Data Bit 4 */
    byte R5_T5       :1;                                       /* Receive/Transmit Data Bit 5 */
    byte R6_T6       :1;                                       /* Receive/Transmit Data Bit 6 */
    byte R7_T7       :1;                                       /* Receive/Transmit Data Bit 7 */
  } Bits;
} SCI2DSTR;
extern volatile SCI2DSTR _SCI2D @0x00000047;
#define SCI2D                           _SCI2D.Byte
#define SCI2D_R0_T0                     _SCI2D.Bits.R0_T0
#define SCI2D_R1_T1                     _SCI2D.Bits.R1_T1
#define SCI2D_R2_T2                     _SCI2D.Bits.R2_T2
#define SCI2D_R3_T3                     _SCI2D.Bits.R3_T3
#define SCI2D_R4_T4                     _SCI2D.Bits.R4_T4
#define SCI2D_R5_T5                     _SCI2D.Bits.R5_T5
#define SCI2D_R6_T6                     _SCI2D.Bits.R6_T6
#define SCI2D_R7_T7                     _SCI2D.Bits.R7_T7

#define SCI2D_R0_T0_MASK                1
#define SCI2D_R1_T1_MASK                2
#define SCI2D_R2_T2_MASK                4
#define SCI2D_R3_T3_MASK                8
#define SCI2D_R4_T4_MASK                16
#define SCI2D_R5_T5_MASK                32
#define SCI2D_R6_T6_MASK                64
#define SCI2D_R7_T7_MASK                128


/*** MCGC1 - MCG Control Register 1; 0x00000048 ***/
typedef union {
  byte Byte;
  struct {
    byte IREFSTEN    :1;                                       /* Internal Reference Stop Enable */
    byte IRCLKEN     :1;                                       /* Internal Reference Clock Enable */
    byte IREFS       :1;                                       /* Internal Reference Select */
    byte RDIV0       :1;                                       /* Reference Divider, bit 0 */
    byte RDIV1       :1;                                       /* Reference Divider, bit 1 */
    byte RDIV2       :1;                                       /* Reference Divider, bit 2 */
    byte CLKS0       :1;                                       /* Clock Source Select, bit 0 */
    byte CLKS1       :1;                                       /* Clock Source Select, bit 1 */
  } Bits;
  struct {
    byte         :1;
    byte         :1;
    byte         :1;
    byte grpRDIV :3;
    byte grpCLKS :2;
  } MergedBits;
} MCGC1STR;
extern volatile MCGC1STR _MCGC1 @0x00000048;
#define MCGC1                           _MCGC1.Byte
#define MCGC1_IREFSTEN                  _MCGC1.Bits.IREFSTEN
#define MCGC1_IRCLKEN                   _MCGC1.Bits.IRCLKEN
#define MCGC1_IREFS                     _MCGC1.Bits.IREFS
#define MCGC1_RDIV0                     _MCGC1.Bits.RDIV0
#define MCGC1_RDIV1                     _MCGC1.Bits.RDIV1
#define MCGC1_RDIV2                     _MCGC1.Bits.RDIV2
#define MCGC1_CLKS0                     _MCGC1.Bits.CLKS0
#define MCGC1_CLKS1                     _MCGC1.Bits.CLKS1
#define MCGC1_RDIV                      _MCGC1.MergedBits.grpRDIV
#define MCGC1_CLKS                      _MCGC1.MergedBits.grpCLKS

#define MCGC1_IREFSTEN_MASK             1
#define MCGC1_IRCLKEN_MASK              2
#define MCGC1_IREFS_MASK                4
#define MCGC1_RDIV0_MASK                8
#define MCGC1_RDIV1_MASK                16
#define MCGC1_RDIV2_MASK                32
#define MCGC1_CLKS0_MASK                64
#define MCGC1_CLKS1_MASK                128
#define MCGC1_RDIV_MASK                 56
#define MCGC1_RDIV_BITNUM               3
#define MCGC1_CLKS_MASK                 192
#define MCGC1_CLKS_BITNUM               6


/*** MCGC2 - MCG Control Register 2; 0x00000049 ***/
typedef union {
  byte Byte;
  struct {
    byte EREFSTEN    :1;                                       /* External Reference Stop Enable */
    byte ERCLKEN     :1;                                       /* External Reference Enable */
    byte EREFS       :1;                                       /* External Reference Select */
    byte LP          :1;                                       /* Low Power Select */
    byte HGO         :1;                                       /* High Gain Oscillator Select */
    byte RANGE       :1;                                       /* Frequency Range Select */
    byte BDIV0       :1;                                       /* Bus Frequency Divider, bit 0 */
    byte BDIV1       :1;                                       /* Bus Frequency Divider, bit 1 */
  } Bits;
  struct {
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte grpBDIV :2;
  } MergedBits;
} MCGC2STR;
extern volatile MCGC2STR _MCGC2 @0x00000049;
#define MCGC2                           _MCGC2.Byte
#define MCGC2_EREFSTEN                  _MCGC2.Bits.EREFSTEN
#define MCGC2_ERCLKEN                   _MCGC2.Bits.ERCLKEN
#define MCGC2_EREFS                     _MCGC2.Bits.EREFS
#define MCGC2_LP                        _MCGC2.Bits.LP
#define MCGC2_HGO                       _MCGC2.Bits.HGO
#define MCGC2_RANGE                     _MCGC2.Bits.RANGE
#define MCGC2_BDIV0                     _MCGC2.Bits.BDIV0
#define MCGC2_BDIV1                     _MCGC2.Bits.BDIV1
#define MCGC2_BDIV                      _MCGC2.MergedBits.grpBDIV

#define MCGC2_EREFSTEN_MASK             1
#define MCGC2_ERCLKEN_MASK              2
#define MCGC2_EREFS_MASK                4
#define MCGC2_LP_MASK                   8
#define MCGC2_HGO_MASK                  16
#define MCGC2_RANGE_MASK                32
#define MCGC2_BDIV0_MASK                64
#define MCGC2_BDIV1_MASK                128
#define MCGC2_BDIV_MASK                 192
#define MCGC2_BDIV_BITNUM               6


/*** MCGTRM - MCG Trim Register; 0x0000004A ***/
typedef union {
  byte Byte;
  struct {
    byte TRIM0       :1;                                       /* MCG Trim Setting, bit 0 */
    byte TRIM1       :1;                                       /* MCG Trim Setting, bit 1 */
    byte TRIM2       :1;                                       /* MCG Trim Setting, bit 2 */
    byte TRIM3       :1;                                       /* MCG Trim Setting, bit 3 */
    byte TRIM4       :1;                                       /* MCG Trim Setting, bit 4 */
    byte TRIM5       :1;                                       /* MCG Trim Setting, bit 5 */
    byte TRIM6       :1;                                       /* MCG Trim Setting, bit 6 */
    byte TRIM7       :1;                                       /* MCG Trim Setting, bit 7 */
  } Bits;
} MCGTRMSTR;
extern volatile MCGTRMSTR _MCGTRM @0x0000004A;
#define MCGTRM                          _MCGTRM.Byte
#define MCGTRM_TRIM0                    _MCGTRM.Bits.TRIM0
#define MCGTRM_TRIM1                    _MCGTRM.Bits.TRIM1
#define MCGTRM_TRIM2                    _MCGTRM.Bits.TRIM2
#define MCGTRM_TRIM3                    _MCGTRM.Bits.TRIM3
#define MCGTRM_TRIM4                    _MCGTRM.Bits.TRIM4
#define MCGTRM_TRIM5                    _MCGTRM.Bits.TRIM5
#define MCGTRM_TRIM6                    _MCGTRM.Bits.TRIM6
#define MCGTRM_TRIM7                    _MCGTRM.Bits.TRIM7

#define MCGTRM_TRIM0_MASK               1
#define MCGTRM_TRIM1_MASK               2
#define MCGTRM_TRIM2_MASK               4
#define MCGTRM_TRIM3_MASK               8
#define MCGTRM_TRIM4_MASK               16
#define MCGTRM_TRIM5_MASK               32
#define MCGTRM_TRIM6_MASK               64
#define MCGTRM_TRIM7_MASK               128


/*** MCGSC - MCG Status and Control Register; 0x0000004B ***/
typedef union {
  byte Byte;
  struct {
    byte FTRIM       :1;                                       /* MCG Fine Trim */
    byte OSCINIT     :1;                                       /* OSC Initialization */
    byte CLKST0      :1;                                       /* Clock Mode Status, bit 0 */
    byte CLKST1      :1;                                       /* Clock Mode Status, bit 1 */
    byte IREFST      :1;                                       /* Internal Reference Status */
    byte PLLST       :1;                                       /* PLL Select Status */
    byte LOCK        :1;                                       /* Lock Status */
    byte LOLS        :1;                                       /* Loss of Lock Status */
  } Bits;
  struct {
    byte         :1;
    byte         :1;
    byte grpCLKST :2;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} MCGSCSTR;
extern volatile MCGSCSTR _MCGSC @0x0000004B;
#define MCGSC                           _MCGSC.Byte
#define MCGSC_FTRIM                     _MCGSC.Bits.FTRIM
#define MCGSC_OSCINIT                   _MCGSC.Bits.OSCINIT
#define MCGSC_CLKST0                    _MCGSC.Bits.CLKST0
#define MCGSC_CLKST1                    _MCGSC.Bits.CLKST1
#define MCGSC_IREFST                    _MCGSC.Bits.IREFST
#define MCGSC_PLLST                     _MCGSC.Bits.PLLST
#define MCGSC_LOCK                      _MCGSC.Bits.LOCK
#define MCGSC_LOLS                      _MCGSC.Bits.LOLS
#define MCGSC_CLKST                     _MCGSC.MergedBits.grpCLKST

#define MCGSC_FTRIM_MASK                1
#define MCGSC_OSCINIT_MASK              2
#define MCGSC_CLKST0_MASK               4
#define MCGSC_CLKST1_MASK               8
#define MCGSC_IREFST_MASK               16
#define MCGSC_PLLST_MASK                32
#define MCGSC_LOCK_MASK                 64
#define MCGSC_LOLS_MASK                 128
#define MCGSC_CLKST_MASK                12
#define MCGSC_CLKST_BITNUM              2


/*** MCGC3 - MCG Control Register 3; 0x0000004C ***/
typedef union {
  byte Byte;
  struct {
    byte VDIV0       :1;                                       /* VCO Divider, bit 0 */
    byte VDIV1       :1;                                       /* VCO Divider, bit 1 */
    byte VDIV2       :1;                                       /* VCO Divider, bit 2 */
    byte VDIV3       :1;                                       /* VCO Divider, bit 3 */
    byte             :1; 
    byte CME         :1;                                       /* Clock Monitor Enable */
    byte PLLS        :1;                                       /* PLL Select */
    byte LOLIE       :1;                                       /* Loss of Lock Interrupt Enable */
  } Bits;
  struct {
    byte grpVDIV :4;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} MCGC3STR;
extern volatile MCGC3STR _MCGC3 @0x0000004C;
#define MCGC3                           _MCGC3.Byte
#define MCGC3_VDIV0                     _MCGC3.Bits.VDIV0
#define MCGC3_VDIV1                     _MCGC3.Bits.VDIV1
#define MCGC3_VDIV2                     _MCGC3.Bits.VDIV2
#define MCGC3_VDIV3                     _MCGC3.Bits.VDIV3
#define MCGC3_CME                       _MCGC3.Bits.CME
#define MCGC3_PLLS                      _MCGC3.Bits.PLLS
#define MCGC3_LOLIE                     _MCGC3.Bits.LOLIE
#define MCGC3_VDIV                      _MCGC3.MergedBits.grpVDIV

#define MCGC3_VDIV0_MASK                1
#define MCGC3_VDIV1_MASK                2
#define MCGC3_VDIV2_MASK                4
#define MCGC3_VDIV3_MASK                8
#define MCGC3_CME_MASK                  32
#define MCGC3_PLLS_MASK                 64
#define MCGC3_LOLIE_MASK                128
#define MCGC3_VDIV_MASK                 15
#define MCGC3_VDIV_BITNUM               0


/*** SPIC1 - SPI Control Register 1; 0x00000050 ***/
typedef union {
  byte Byte;
  struct {
    byte LSBFE       :1;                                       /* LSB First (Shifter Direction) */
    byte SSOE        :1;                                       /* Slave Select Output Enable */
    byte CPHA        :1;                                       /* Clock Phase */
    byte CPOL        :1;                                       /* Clock Polarity */
    byte MSTR        :1;                                       /* Master/Slave Mode Select */
    byte SPTIE       :1;                                       /* SPI Transmit Interrupt Enable */
    byte SPE         :1;                                       /* SPI System Enable */
    byte SPIE        :1;                                       /* SPI Interrupt Enable (for SPRF and MODF) */
  } Bits;
} SPIC1STR;
extern volatile SPIC1STR _SPIC1 @0x00000050;
#define SPIC1                           _SPIC1.Byte
#define SPIC1_LSBFE                     _SPIC1.Bits.LSBFE
#define SPIC1_SSOE                      _SPIC1.Bits.SSOE
#define SPIC1_CPHA                      _SPIC1.Bits.CPHA
#define SPIC1_CPOL                      _SPIC1.Bits.CPOL
#define SPIC1_MSTR                      _SPIC1.Bits.MSTR
#define SPIC1_SPTIE                     _SPIC1.Bits.SPTIE
#define SPIC1_SPE                       _SPIC1.Bits.SPE
#define SPIC1_SPIE                      _SPIC1.Bits.SPIE

#define SPIC1_LSBFE_MASK                1
#define SPIC1_SSOE_MASK                 2
#define SPIC1_CPHA_MASK                 4
#define SPIC1_CPOL_MASK                 8
#define SPIC1_MSTR_MASK                 16
#define SPIC1_SPTIE_MASK                32
#define SPIC1_SPE_MASK                  64
#define SPIC1_SPIE_MASK                 128


/*** SPIC2 - SPI Control Register 2; 0x00000051 ***/
typedef union {
  byte Byte;
  struct {
    byte SPC0        :1;                                       /* SPI Pin Control 0 */
    byte SPISWAI     :1;                                       /* SPI Stop in Wait Mode */
    byte             :1; 
    byte BIDIROE     :1;                                       /* Bidirectional Mode Output Enable */
    byte MODFEN      :1;                                       /* Master Mode-Fault Function Enable */
    byte             :1; 
    byte             :1; 
    byte             :1; 
  } Bits;
} SPIC2STR;
extern volatile SPIC2STR _SPIC2 @0x00000051;
#define SPIC2                           _SPIC2.Byte
#define SPIC2_SPC0                      _SPIC2.Bits.SPC0
#define SPIC2_SPISWAI                   _SPIC2.Bits.SPISWAI
#define SPIC2_BIDIROE                   _SPIC2.Bits.BIDIROE
#define SPIC2_MODFEN                    _SPIC2.Bits.MODFEN

#define SPIC2_SPC0_MASK                 1
#define SPIC2_SPISWAI_MASK              2
#define SPIC2_BIDIROE_MASK              8
#define SPIC2_MODFEN_MASK               16


/*** SPIBR - SPI Baud Rate Register; 0x00000052 ***/
typedef union {
  byte Byte;
  struct {
    byte SPR0        :1;                                       /* SPI Baud Rate Divisor Bit 0 */
    byte SPR1        :1;                                       /* SPI Baud Rate Divisor Bit 1 */
    byte SPR2        :1;                                       /* SPI Baud Rate Divisor Bit 2 */
    byte             :1; 
    byte SPPR0       :1;                                       /* SPI Baud Rate Prescale Divisor Bit 0 */
    byte SPPR1       :1;                                       /* SPI Baud Rate Prescale Divisor Bit 1 */
    byte SPPR2       :1;                                       /* SPI Baud Rate Prescale Divisor Bit 2 */
    byte             :1; 
  } Bits;
  struct {
    byte grpSPR  :3;
    byte         :1;
    byte grpSPPR :3;
    byte         :1;
  } MergedBits;
} SPIBRSTR;
extern volatile SPIBRSTR _SPIBR @0x00000052;
#define SPIBR                           _SPIBR.Byte
#define SPIBR_SPR0                      _SPIBR.Bits.SPR0
#define SPIBR_SPR1                      _SPIBR.Bits.SPR1
#define SPIBR_SPR2                      _SPIBR.Bits.SPR2
#define SPIBR_SPPR0                     _SPIBR.Bits.SPPR0
#define SPIBR_SPPR1                     _SPIBR.Bits.SPPR1
#define SPIBR_SPPR2                     _SPIBR.Bits.SPPR2
#define SPIBR_SPR                       _SPIBR.MergedBits.grpSPR
#define SPIBR_SPPR                      _SPIBR.MergedBits.grpSPPR

#define SPIBR_SPR0_MASK                 1
#define SPIBR_SPR1_MASK                 2
#define SPIBR_SPR2_MASK                 4
#define SPIBR_SPPR0_MASK                16
#define SPIBR_SPPR1_MASK                32
#define SPIBR_SPPR2_MASK                64
#define SPIBR_SPR_MASK                  7
#define SPIBR_SPR_BITNUM                0
#define SPIBR_SPPR_MASK                 112
#define SPIBR_SPPR_BITNUM               4


/*** SPIS - SPI Status Register; 0x00000053 ***/
typedef union {
  byte Byte;
  struct {
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte MODF        :1;                                       /* Master Mode Fault Flag */
    byte SPTEF       :1;                                       /* SPI Transmit Buffer Empty Flag */
    byte             :1; 
    byte SPRF        :1;                                       /* SPI Read Buffer Full Flag */
  } Bits;
} SPISSTR;
extern volatile SPISSTR _SPIS @0x00000053;
#define SPIS                            _SPIS.Byte
#define SPIS_MODF                       _SPIS.Bits.MODF
#define SPIS_SPTEF                      _SPIS.Bits.SPTEF
#define SPIS_SPRF                       _SPIS.Bits.SPRF

#define SPIS_MODF_MASK                  16
#define SPIS_SPTEF_MASK                 32
#define SPIS_SPRF_MASK                  128


/*** SPID - SPI Data Register; 0x00000055 ***/
typedef union {
  byte Byte;
} SPIDSTR;
extern volatile SPIDSTR _SPID @0x00000055;
#define SPID                            _SPID.Byte


/*** IICA - IIC Address Register; 0x00000058 ***/
typedef union {
  byte Byte;
  struct {
    byte             :1; 
    byte AD1         :1;                                       /* Slave Address Bit 1 */
    byte AD2         :1;                                       /* Slave Address Bit 2 */
    byte AD3         :1;                                       /* Slave Address Bit 3 */
    byte AD4         :1;                                       /* Slave Address Bit 4 */
    byte AD5         :1;                                       /* Slave Address Bit 5 */
    byte AD6         :1;                                       /* Slave Address Bit 6 */
    byte AD7         :1;                                       /* Slave Address Bit 7 */
  } Bits;
  struct {
    byte         :1;
    byte grpAD_1 :7;
  } MergedBits;
} IICASTR;
extern volatile IICASTR _IICA @0x00000058;
#define IICA                            _IICA.Byte
#define IICA_AD1                        _IICA.Bits.AD1
#define IICA_AD2                        _IICA.Bits.AD2
#define IICA_AD3                        _IICA.Bits.AD3
#define IICA_AD4                        _IICA.Bits.AD4
#define IICA_AD5                        _IICA.Bits.AD5
#define IICA_AD6                        _IICA.Bits.AD6
#define IICA_AD7                        _IICA.Bits.AD7
#define IICA_AD_1                       _IICA.MergedBits.grpAD_1
#define IICA_AD                         IICA_AD_1

#define IICA_AD1_MASK                   2
#define IICA_AD2_MASK                   4
#define IICA_AD3_MASK                   8
#define IICA_AD4_MASK                   16
#define IICA_AD5_MASK                   32
#define IICA_AD6_MASK                   64
#define IICA_AD7_MASK                   128
#define IICA_AD_1_MASK                  254
#define IICA_AD_1_BITNUM                1


/*** IICF - IIC Frequency Divider Register; 0x00000059 ***/
typedef union {
  byte Byte;
  struct {
    byte ICR0        :1;                                       /* IIC Clock Rate Bit 0 */
    byte ICR1        :1;                                       /* IIC Clock Rate Bit 1 */
    byte ICR2        :1;                                       /* IIC Clock Rate Bit 2 */
    byte ICR3        :1;                                       /* IIC Clock Rate Bit 3 */
    byte ICR4        :1;                                       /* IIC Clock Rate Bit 4 */
    byte ICR5        :1;                                       /* IIC Clock Rate Bit 5 */
    byte MULT0       :1;                                       /* Multiplier Factor Bit 0 */
    byte MULT1       :1;                                       /* Multiplier Factor Bit 1 */
  } Bits;
  struct {
    byte grpICR  :6;
    byte grpMULT :2;
  } MergedBits;
} IICFSTR;
extern volatile IICFSTR _IICF @0x00000059;
#define IICF                            _IICF.Byte
#define IICF_ICR0                       _IICF.Bits.ICR0
#define IICF_ICR1                       _IICF.Bits.ICR1
#define IICF_ICR2                       _IICF.Bits.ICR2
#define IICF_ICR3                       _IICF.Bits.ICR3
#define IICF_ICR4                       _IICF.Bits.ICR4
#define IICF_ICR5                       _IICF.Bits.ICR5
#define IICF_MULT0                      _IICF.Bits.MULT0
#define IICF_MULT1                      _IICF.Bits.MULT1
#define IICF_ICR                        _IICF.MergedBits.grpICR
#define IICF_MULT                       _IICF.MergedBits.grpMULT

#define IICF_ICR0_MASK                  1
#define IICF_ICR1_MASK                  2
#define IICF_ICR2_MASK                  4
#define IICF_ICR3_MASK                  8
#define IICF_ICR4_MASK                  16
#define IICF_ICR5_MASK                  32
#define IICF_MULT0_MASK                 64
#define IICF_MULT1_MASK                 128
#define IICF_ICR_MASK                   63
#define IICF_ICR_BITNUM                 0
#define IICF_MULT_MASK                  192
#define IICF_MULT_BITNUM                6


/*** IICC1 - IIC Control Register 1; 0x0000005A ***/
typedef union {
  byte Byte;
  union { /* Several registers at the same address */
    /*** IICC1 - IIC Control Register 1; Several registers at the same address ***/
    union {
      struct {
        byte             :1; 
        byte             :1; 
        byte RSTA        :1;                                       /* Repeat START */
        byte TXAK        :1;                                       /* Transmit Acknowledge Enable */
        byte TX          :1;                                       /* Transmit Mode Select */
        byte MST         :1;                                       /* Master Mode Select */
        byte IICIE       :1;                                       /* IIC Interrupt Enable */
        byte IICEN       :1;                                       /* IIC Enable */
      } Bits;
    } IICC1STR;
    #define IICC1                       _IICC1.Byte
    #define IICC1_RSTA                  _IICC1.SameAddr_STR.IICC1STR.Bits.RSTA
    #define IICC1_TXAK                  _IICC1.SameAddr_STR.IICC1STR.Bits.TXAK
    #define IICC1_TX                    _IICC1.SameAddr_STR.IICC1STR.Bits.TX
    #define IICC1_MST                   _IICC1.SameAddr_STR.IICC1STR.Bits.MST
    #define IICC1_IICIE                 _IICC1.SameAddr_STR.IICC1STR.Bits.IICIE
    #define IICC1_IICEN                 _IICC1.SameAddr_STR.IICC1STR.Bits.IICEN
    
    #define IICC1_RSTA_MASK             4
    #define IICC1_TXAK_MASK             8
    #define IICC1_TX_MASK               16
    #define IICC1_MST_MASK              32
    #define IICC1_IICIE_MASK            64
    #define IICC1_IICEN_MASK            128
    
    /*** IICC - IIC Control Register; Several registers at the same address ***/
    union {
      struct {
        byte             :1; 
        byte             :1; 
        byte RSTA        :1;                                       /* Repeat START */
        byte TXAK        :1;                                       /* Transmit Acknowledge Enable */
        byte TX          :1;                                       /* Transmit Mode Select */
        byte MST         :1;                                       /* Master Mode Select */
        byte IICIE       :1;                                       /* IIC Interrupt Enable */
        byte IICEN       :1;                                       /* IIC Enable */
      } Bits;
    } IICCSTR;
    #define IICC                        _IICC1.Byte
    #define IICC_RSTA                   _IICC1.SameAddr_STR.IICCSTR.Bits.RSTA
    #define IICC_TXAK                   _IICC1.SameAddr_STR.IICCSTR.Bits.TXAK
    #define IICC_TX                     _IICC1.SameAddr_STR.IICCSTR.Bits.TX
    #define IICC_MST                    _IICC1.SameAddr_STR.IICCSTR.Bits.MST
    #define IICC_IICIE                  _IICC1.SameAddr_STR.IICCSTR.Bits.IICIE
    #define IICC_IICEN                  _IICC1.SameAddr_STR.IICCSTR.Bits.IICEN
    
    #define IICC_RSTA_MASK              4
    #define IICC_TXAK_MASK              8
    #define IICC_TX_MASK                16
    #define IICC_MST_MASK               32
    #define IICC_IICIE_MASK             64
    #define IICC_IICEN_MASK             128
    
  } SameAddr_STR; /*Several registers at the same address */

} IICC1STR;
extern volatile IICC1STR _IICC1 @0x0000005A;


/*** IICS - IIC Status Register; 0x0000005B ***/
typedef union {
  byte Byte;
  struct {
    byte RXAK        :1;                                       /* Receive Acknowledge */
    byte IICIF       :1;                                       /* IIC Interrupt Flag */
    byte SRW         :1;                                       /* Slave Read/Write */
    byte             :1; 
    byte ARBL        :1;                                       /* Arbitration Lost */
    byte BUSY        :1;                                       /* Bus Busy */
    byte IAAS        :1;                                       /* Addressed as a Slave */
    byte TCF         :1;                                       /* Transfer Complete Flag */
  } Bits;
} IICSSTR;
extern volatile IICSSTR _IICS @0x0000005B;
#define IICS                            _IICS.Byte
#define IICS_RXAK                       _IICS.Bits.RXAK
#define IICS_IICIF                      _IICS.Bits.IICIF
#define IICS_SRW                        _IICS.Bits.SRW
#define IICS_ARBL                       _IICS.Bits.ARBL
#define IICS_BUSY                       _IICS.Bits.BUSY
#define IICS_IAAS                       _IICS.Bits.IAAS
#define IICS_TCF                        _IICS.Bits.TCF

#define IICS_RXAK_MASK                  1
#define IICS_IICIF_MASK                 2
#define IICS_SRW_MASK                   4
#define IICS_ARBL_MASK                  16
#define IICS_BUSY_MASK                  32
#define IICS_IAAS_MASK                  64
#define IICS_TCF_MASK                   128


/*** IICD - IIC Data I/O Register; 0x0000005C ***/
typedef union {
  byte Byte;
  struct {
    byte DATA0       :1;                                       /* IIC Data Bit 0 */
    byte DATA1       :1;                                       /* IIC Data Bit 1 */
    byte DATA2       :1;                                       /* IIC Data Bit 2 */
    byte DATA3       :1;                                       /* IIC Data Bit 3 */
    byte DATA4       :1;                                       /* IIC Data Bit 4 */
    byte DATA5       :1;                                       /* IIC Data Bit 5 */
    byte DATA6       :1;                                       /* IIC Data Bit 6 */
    byte DATA7       :1;                                       /* IIC Data Bit 7 */
  } Bits;
} IICDSTR;
extern volatile IICDSTR _IICD @0x0000005C;
#define IICD                            _IICD.Byte
#define IICD_DATA0                      _IICD.Bits.DATA0
#define IICD_DATA1                      _IICD.Bits.DATA1
#define IICD_DATA2                      _IICD.Bits.DATA2
#define IICD_DATA3                      _IICD.Bits.DATA3
#define IICD_DATA4                      _IICD.Bits.DATA4
#define IICD_DATA5                      _IICD.Bits.DATA5
#define IICD_DATA6                      _IICD.Bits.DATA6
#define IICD_DATA7                      _IICD.Bits.DATA7

#define IICD_DATA0_MASK                 1
#define IICD_DATA1_MASK                 2
#define IICD_DATA2_MASK                 4
#define IICD_DATA3_MASK                 8
#define IICD_DATA4_MASK                 16
#define IICD_DATA5_MASK                 32
#define IICD_DATA6_MASK                 64
#define IICD_DATA7_MASK                 128


/*** IICC2 - IIC Control Register 2; 0x0000005D ***/
typedef union {
  byte Byte;
  struct {
    byte AD8         :1;                                       /* Slave Address Bit 8 */
    byte AD9         :1;                                       /* Slave Address Bit 9 */
    byte AD10        :1;                                       /* Slave Address Bit 10 */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte ADEXT       :1;                                       /* Address Extension */
    byte GCAEN       :1;                                       /* General Call Address Enable */
  } Bits;
  struct {
    byte grpAD_8 :3;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} IICC2STR;
extern volatile IICC2STR _IICC2 @0x0000005D;
#define IICC2                           _IICC2.Byte
#define IICC2_AD8                       _IICC2.Bits.AD8
#define IICC2_AD9                       _IICC2.Bits.AD9
#define IICC2_AD10                      _IICC2.Bits.AD10
#define IICC2_ADEXT                     _IICC2.Bits.ADEXT
#define IICC2_GCAEN                     _IICC2.Bits.GCAEN
#define IICC2_AD_8                      _IICC2.MergedBits.grpAD_8
#define IICC2_AD                        IICC2_AD_8

#define IICC2_AD8_MASK                  1
#define IICC2_AD9_MASK                  2
#define IICC2_AD10_MASK                 4
#define IICC2_ADEXT_MASK                64
#define IICC2_GCAEN_MASK                128
#define IICC2_AD_8_MASK                 7
#define IICC2_AD_8_BITNUM               0


/*** TPM2SC - TPM2 Status and Control Register; 0x00000060 ***/
typedef union {
  byte Byte;
  struct {
    byte PS0         :1;                                       /* Prescale Divisor Select Bit 0 */
    byte PS1         :1;                                       /* Prescale Divisor Select Bit 1 */
    byte PS2         :1;                                       /* Prescale Divisor Select Bit 2 */
    byte CLKSA       :1;                                       /* Clock Source Select A */
    byte CLKSB       :1;                                       /* Clock Source Select B */
    byte CPWMS       :1;                                       /* Center-Aligned PWM Select */
    byte TOIE        :1;                                       /* Timer Overflow Interrupt Enable */
    byte TOF         :1;                                       /* Timer Overflow Flag */
  } Bits;
  struct {
    byte grpPS   :3;
    byte grpCLKSx :2;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} TPM2SCSTR;
extern volatile TPM2SCSTR _TPM2SC @0x00000060;
#define TPM2SC                          _TPM2SC.Byte
#define TPM2SC_PS0                      _TPM2SC.Bits.PS0
#define TPM2SC_PS1                      _TPM2SC.Bits.PS1
#define TPM2SC_PS2                      _TPM2SC.Bits.PS2
#define TPM2SC_CLKSA                    _TPM2SC.Bits.CLKSA
#define TPM2SC_CLKSB                    _TPM2SC.Bits.CLKSB
#define TPM2SC_CPWMS                    _TPM2SC.Bits.CPWMS
#define TPM2SC_TOIE                     _TPM2SC.Bits.TOIE
#define TPM2SC_TOF                      _TPM2SC.Bits.TOF
#define TPM2SC_PS                       _TPM2SC.MergedBits.grpPS
#define TPM2SC_CLKSx                    _TPM2SC.MergedBits.grpCLKSx

#define TPM2SC_PS0_MASK                 1
#define TPM2SC_PS1_MASK                 2
#define TPM2SC_PS2_MASK                 4
#define TPM2SC_CLKSA_MASK               8
#define TPM2SC_CLKSB_MASK               16
#define TPM2SC_CPWMS_MASK               32
#define TPM2SC_TOIE_MASK                64
#define TPM2SC_TOF_MASK                 128
#define TPM2SC_PS_MASK                  7
#define TPM2SC_PS_BITNUM                0
#define TPM2SC_CLKSx_MASK               24
#define TPM2SC_CLKSx_BITNUM             3


/*** TPM2CNT - TPM2 Timer Counter Register; 0x00000061 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** TPM2CNTH - TPM2 Timer Counter Register High; 0x00000061 ***/
    union {
      byte Byte;
    } TPM2CNTHSTR;
    #define TPM2CNTH                    _TPM2CNT.Overlap_STR.TPM2CNTHSTR.Byte
    

    /*** TPM2CNTL - TPM2 Timer Counter Register Low; 0x00000062 ***/
    union {
      byte Byte;
    } TPM2CNTLSTR;
    #define TPM2CNTL                    _TPM2CNT.Overlap_STR.TPM2CNTLSTR.Byte
    
  } Overlap_STR;

} TPM2CNTSTR;
extern volatile TPM2CNTSTR _TPM2CNT @0x00000061;
#define TPM2CNT                         _TPM2CNT.Word


/*** TPM2MOD - TPM2 Timer Counter Modulo Register; 0x00000063 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** TPM2MODH - TPM2 Timer Counter Modulo Register High; 0x00000063 ***/
    union {
      byte Byte;
    } TPM2MODHSTR;
    #define TPM2MODH                    _TPM2MOD.Overlap_STR.TPM2MODHSTR.Byte
    

    /*** TPM2MODL - TPM2 Timer Counter Modulo Register Low; 0x00000064 ***/
    union {
      byte Byte;
    } TPM2MODLSTR;
    #define TPM2MODL                    _TPM2MOD.Overlap_STR.TPM2MODLSTR.Byte
    
  } Overlap_STR;

} TPM2MODSTR;
extern volatile TPM2MODSTR _TPM2MOD @0x00000063;
#define TPM2MOD                         _TPM2MOD.Word


/*** TPM2C0SC - TPM2 Timer Channel 0 Status and Control Register; 0x00000065 ***/
typedef union {
  byte Byte;
  struct {
    byte             :1; 
    byte             :1; 
    byte ELS0A       :1;                                       /* Edge/Level Select Bit A */
    byte ELS0B       :1;                                       /* Edge/Level Select Bit B */
    byte MS0A        :1;                                       /* Mode Select A for TPM Channel 0 */
    byte MS0B        :1;                                       /* Mode Select B for TPM Channel 0 */
    byte CH0IE       :1;                                       /* Channel 0 Interrupt Enable */
    byte CH0F        :1;                                       /* Channel 0 Flag */
  } Bits;
  struct {
    byte         :1;
    byte         :1;
    byte grpELS0x :2;
    byte grpMS0x :2;
    byte         :1;
    byte         :1;
  } MergedBits;
} TPM2C0SCSTR;
extern volatile TPM2C0SCSTR _TPM2C0SC @0x00000065;
#define TPM2C0SC                        _TPM2C0SC.Byte
#define TPM2C0SC_ELS0A                  _TPM2C0SC.Bits.ELS0A
#define TPM2C0SC_ELS0B                  _TPM2C0SC.Bits.ELS0B
#define TPM2C0SC_MS0A                   _TPM2C0SC.Bits.MS0A
#define TPM2C0SC_MS0B                   _TPM2C0SC.Bits.MS0B
#define TPM2C0SC_CH0IE                  _TPM2C0SC.Bits.CH0IE
#define TPM2C0SC_CH0F                   _TPM2C0SC.Bits.CH0F
#define TPM2C0SC_ELS0x                  _TPM2C0SC.MergedBits.grpELS0x
#define TPM2C0SC_MS0x                   _TPM2C0SC.MergedBits.grpMS0x

#define TPM2C0SC_ELS0A_MASK             4
#define TPM2C0SC_ELS0B_MASK             8
#define TPM2C0SC_MS0A_MASK              16
#define TPM2C0SC_MS0B_MASK              32
#define TPM2C0SC_CH0IE_MASK             64
#define TPM2C0SC_CH0F_MASK              128
#define TPM2C0SC_ELS0x_MASK             12
#define TPM2C0SC_ELS0x_BITNUM           2
#define TPM2C0SC_MS0x_MASK              48
#define TPM2C0SC_MS0x_BITNUM            4


/*** TPM2C0V - TPM2 Timer Channel 0 Value Register; 0x00000066 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** TPM2C0VH - TPM2 Timer Channel 0 Value Register High; 0x00000066 ***/
    union {
      byte Byte;
    } TPM2C0VHSTR;
    #define TPM2C0VH                    _TPM2C0V.Overlap_STR.TPM2C0VHSTR.Byte
    

    /*** TPM2C0VL - TPM2 Timer Channel 0 Value Register Low; 0x00000067 ***/
    union {
      byte Byte;
    } TPM2C0VLSTR;
    #define TPM2C0VL                    _TPM2C0V.Overlap_STR.TPM2C0VLSTR.Byte
    
  } Overlap_STR;

} TPM2C0VSTR;
extern volatile TPM2C0VSTR _TPM2C0V @0x00000066;
#define TPM2C0V                         _TPM2C0V.Word


/*** TPM2C1SC - TPM2 Timer Channel 1 Status and Control Register; 0x00000068 ***/
typedef union {
  byte Byte;
  struct {
    byte             :1; 
    byte             :1; 
    byte ELS1A       :1;                                       /* Edge/Level Select Bit A */
    byte ELS1B       :1;                                       /* Edge/Level Select Bit B */
    byte MS1A        :1;                                       /* Mode Select A for TPM Channel 1 */
    byte MS1B        :1;                                       /* Mode Select B for TPM Channel 1 */
    byte CH1IE       :1;                                       /* Channel 1 Interrupt Enable */
    byte CH1F        :1;                                       /* Channel 1 Flag */
  } Bits;
  struct {
    byte         :1;
    byte         :1;
    byte grpELS1x :2;
    byte grpMS1x :2;
    byte         :1;
    byte         :1;
  } MergedBits;
} TPM2C1SCSTR;
extern volatile TPM2C1SCSTR _TPM2C1SC @0x00000068;
#define TPM2C1SC                        _TPM2C1SC.Byte
#define TPM2C1SC_ELS1A                  _TPM2C1SC.Bits.ELS1A
#define TPM2C1SC_ELS1B                  _TPM2C1SC.Bits.ELS1B
#define TPM2C1SC_MS1A                   _TPM2C1SC.Bits.MS1A
#define TPM2C1SC_MS1B                   _TPM2C1SC.Bits.MS1B
#define TPM2C1SC_CH1IE                  _TPM2C1SC.Bits.CH1IE
#define TPM2C1SC_CH1F                   _TPM2C1SC.Bits.CH1F
#define TPM2C1SC_ELS1x                  _TPM2C1SC.MergedBits.grpELS1x
#define TPM2C1SC_MS1x                   _TPM2C1SC.MergedBits.grpMS1x

#define TPM2C1SC_ELS1A_MASK             4
#define TPM2C1SC_ELS1B_MASK             8
#define TPM2C1SC_MS1A_MASK              16
#define TPM2C1SC_MS1B_MASK              32
#define TPM2C1SC_CH1IE_MASK             64
#define TPM2C1SC_CH1F_MASK              128
#define TPM2C1SC_ELS1x_MASK             12
#define TPM2C1SC_ELS1x_BITNUM           2
#define TPM2C1SC_MS1x_MASK              48
#define TPM2C1SC_MS1x_BITNUM            4


/*** TPM2C1V - TPM2 Timer Channel 1 Value Register; 0x00000069 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** TPM2C1VH - TPM2 Timer Channel 1 Value Register High; 0x00000069 ***/
    union {
      byte Byte;
    } TPM2C1VHSTR;
    #define TPM2C1VH                    _TPM2C1V.Overlap_STR.TPM2C1VHSTR.Byte
    

    /*** TPM2C1VL - TPM2 Timer Channel 1 Value Register Low; 0x0000006A ***/
    union {
      byte Byte;
    } TPM2C1VLSTR;
    #define TPM2C1VL                    _TPM2C1V.Overlap_STR.TPM2C1VLSTR.Byte
    
  } Overlap_STR;

} TPM2C1VSTR;
extern volatile TPM2C1VSTR _TPM2C1V @0x00000069;
#define TPM2C1V                         _TPM2C1V.Word


/*** RTCSC - RTC Status and Control Register; 0x0000006C ***/
typedef union {
  byte Byte;
  struct {
    byte RTCPS0      :1;                                       /* Real-Time Clock Prescaler Select, bit 0 */
    byte RTCPS1      :1;                                       /* Real-Time Clock Prescaler Select, bit 1 */
    byte RTCPS2      :1;                                       /* Real-Time Clock Prescaler Select, bit 2 */
    byte RTCPS3      :1;                                       /* Real-Time Clock Prescaler Select, bit 3 */
    byte RTIE        :1;                                       /* Real-Time Interrupt Enable */
    byte RTCLKS0     :1;                                       /* Real-Time Clock Source Select, bit 0 */
    byte RTCLKS1     :1;                                       /* Real-Time Clock Source Select, bit 1 */
    byte RTIF        :1;                                       /* Real-Time Interrupt Flag */
  } Bits;
  struct {
    byte grpRTCPS :4;
    byte         :1;
    byte grpRTCLKS :2;
    byte         :1;
  } MergedBits;
} RTCSCSTR;
extern volatile RTCSCSTR _RTCSC @0x0000006C;
#define RTCSC                           _RTCSC.Byte
#define RTCSC_RTCPS0                    _RTCSC.Bits.RTCPS0
#define RTCSC_RTCPS1                    _RTCSC.Bits.RTCPS1
#define RTCSC_RTCPS2                    _RTCSC.Bits.RTCPS2
#define RTCSC_RTCPS3                    _RTCSC.Bits.RTCPS3
#define RTCSC_RTIE                      _RTCSC.Bits.RTIE
#define RTCSC_RTCLKS0                   _RTCSC.Bits.RTCLKS0
#define RTCSC_RTCLKS1                   _RTCSC.Bits.RTCLKS1
#define RTCSC_RTIF                      _RTCSC.Bits.RTIF
#define RTCSC_RTCPS                     _RTCSC.MergedBits.grpRTCPS
#define RTCSC_RTCLKS                    _RTCSC.MergedBits.grpRTCLKS

#define RTCSC_RTCPS0_MASK               1
#define RTCSC_RTCPS1_MASK               2
#define RTCSC_RTCPS2_MASK               4
#define RTCSC_RTCPS3_MASK               8
#define RTCSC_RTIE_MASK                 16
#define RTCSC_RTCLKS0_MASK              32
#define RTCSC_RTCLKS1_MASK              64
#define RTCSC_RTIF_MASK                 128
#define RTCSC_RTCPS_MASK                15
#define RTCSC_RTCPS_BITNUM              0
#define RTCSC_RTCLKS_MASK               96
#define RTCSC_RTCLKS_BITNUM             5


/*** RTCCNT - RTC Counter Register; 0x0000006D ***/
typedef union {
  byte Byte;
} RTCCNTSTR;
extern volatile RTCCNTSTR _RTCCNT @0x0000006D;
#define RTCCNT                          _RTCCNT.Byte


/*** RTCMOD - RTC Modulo Register; 0x0000006E ***/
typedef union {
  byte Byte;
} RTCMODSTR;
extern volatile RTCMODSTR _RTCMOD @0x0000006E;
#define RTCMOD                          _RTCMOD.Byte


/*** SRS - System Reset Status Register; 0x00001800 ***/
typedef union {
  byte Byte;
  struct {
    byte             :1; 
    byte LVD         :1;                                       /* Low Voltage Detect */
    byte LOC         :1;                                       /* Loss-of-Clock Reset */
    byte ILAD        :1;                                       /* Illegal Address */
    byte ILOP        :1;                                       /* Illegal Opcode */
    byte COP         :1;                                       /* Computer Operating Properly (COP) Watchdog */
    byte PIN         :1;                                       /* External Reset Pin */
    byte POR         :1;                                       /* Power-On Reset */
  } Bits;
} SRSSTR;
extern volatile SRSSTR _SRS @0x00001800;
#define SRS                             _SRS.Byte
#define SRS_LVD                         _SRS.Bits.LVD
#define SRS_LOC                         _SRS.Bits.LOC
#define SRS_ILAD                        _SRS.Bits.ILAD
#define SRS_ILOP                        _SRS.Bits.ILOP
#define SRS_COP                         _SRS.Bits.COP
#define SRS_PIN                         _SRS.Bits.PIN
#define SRS_POR                         _SRS.Bits.POR

#define SRS_LVD_MASK                    2
#define SRS_LOC_MASK                    4
#define SRS_ILAD_MASK                   8
#define SRS_ILOP_MASK                   16
#define SRS_COP_MASK                    32
#define SRS_PIN_MASK                    64
#define SRS_POR_MASK                    128


/*** SBDFR - System Background Debug Force Reset Register; 0x00001801 ***/
typedef union {
  byte Byte;
  struct {
    byte BDFR        :1;                                       /* Background Debug Force Reset */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
  } Bits;
} SBDFRSTR;
extern volatile SBDFRSTR _SBDFR @0x00001801;
#define SBDFR                           _SBDFR.Byte
#define SBDFR_BDFR                      _SBDFR.Bits.BDFR

#define SBDFR_BDFR_MASK                 1


/*** SOPT1 - System Options Register 1; 0x00001802 ***/
typedef union {
  byte Byte;
  struct {
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte IICPS       :1;                                       /* IIC Pin Select */
    byte SCI2PS      :1;                                       /* SCI2 Pin Select */
    byte STOPE       :1;                                       /* Stop Mode Enable */
    byte COPT0       :1;                                       /* COP Watchdog Timeout, bit 0 */
    byte COPT1       :1;                                       /* COP Watchdog Timeout, bit 1 */
  } Bits;
  struct {
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte grpCOPT :2;
  } MergedBits;
} SOPT1STR;
extern volatile SOPT1STR _SOPT1 @0x00001802;
#define SOPT1                           _SOPT1.Byte
#define SOPT1_IICPS                     _SOPT1.Bits.IICPS
#define SOPT1_SCI2PS                    _SOPT1.Bits.SCI2PS
#define SOPT1_STOPE                     _SOPT1.Bits.STOPE
#define SOPT1_COPT0                     _SOPT1.Bits.COPT0
#define SOPT1_COPT1                     _SOPT1.Bits.COPT1
#define SOPT1_COPT                      _SOPT1.MergedBits.grpCOPT

#define SOPT1_IICPS_MASK                8
#define SOPT1_SCI2PS_MASK               16
#define SOPT1_STOPE_MASK                32
#define SOPT1_COPT0_MASK                64
#define SOPT1_COPT1_MASK                128
#define SOPT1_COPT_MASK                 192
#define SOPT1_COPT_BITNUM               6


/*** SOPT2 - System Options Register 2; 0x00001803 ***/
typedef union {
  byte Byte;
  struct {
    byte MCSEL0      :1;                                       /* MCLK Divide Select, bit 0 */
    byte MCSEL1      :1;                                       /* MCLK Divide Select, bit 1 */
    byte MCSEL2      :1;                                       /* MCLK Divide Select, bit 2 */
    byte             :1; 
    byte ADHTS       :1;                                       /* ADC Hardware Trigger Select */
    byte             :1; 
    byte COPW        :1;                                       /* COP Window */
    byte COPCLKS     :1;                                       /* COP Watchdog Clock Select */
  } Bits;
  struct {
    byte grpMCSEL :3;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} SOPT2STR;
extern volatile SOPT2STR _SOPT2 @0x00001803;
#define SOPT2                           _SOPT2.Byte
#define SOPT2_MCSEL0                    _SOPT2.Bits.MCSEL0
#define SOPT2_MCSEL1                    _SOPT2.Bits.MCSEL1
#define SOPT2_MCSEL2                    _SOPT2.Bits.MCSEL2
#define SOPT2_ADHTS                     _SOPT2.Bits.ADHTS
#define SOPT2_COPW                      _SOPT2.Bits.COPW
#define SOPT2_COPCLKS                   _SOPT2.Bits.COPCLKS
#define SOPT2_MCSEL                     _SOPT2.MergedBits.grpMCSEL

#define SOPT2_MCSEL0_MASK               1
#define SOPT2_MCSEL1_MASK               2
#define SOPT2_MCSEL2_MASK               4
#define SOPT2_ADHTS_MASK                16
#define SOPT2_COPW_MASK                 64
#define SOPT2_COPCLKS_MASK              128
#define SOPT2_MCSEL_MASK                7
#define SOPT2_MCSEL_BITNUM              0


/*** SDID - System Device Identification Register; 0x00001806 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** SDIDH - System Device Identification Register High; 0x00001806 ***/
    union {
      byte Byte;
      struct {
        byte ID8         :1;                                       /* Part Identification Number, bit 8 */
        byte ID9         :1;                                       /* Part Identification Number, bit 9 */
        byte ID10        :1;                                       /* Part Identification Number, bit 10 */
        byte ID11        :1;                                       /* Part Identification Number, bit 11 */
        byte             :1; 
        byte             :1; 
        byte             :1; 
        byte             :1; 
      } Bits;
      struct {
        byte grpID_8 :4;
        byte     :1;
        byte     :1;
        byte     :1;
        byte     :1;
      } MergedBits;
    } SDIDHSTR;
    #define SDIDH                       _SDID.Overlap_STR.SDIDHSTR.Byte
    #define SDIDH_ID8                   _SDID.Overlap_STR.SDIDHSTR.Bits.ID8
    #define SDIDH_ID9                   _SDID.Overlap_STR.SDIDHSTR.Bits.ID9
    #define SDIDH_ID10                  _SDID.Overlap_STR.SDIDHSTR.Bits.ID10
    #define SDIDH_ID11                  _SDID.Overlap_STR.SDIDHSTR.Bits.ID11
    #define SDIDH_ID_8                  _SDID.Overlap_STR.SDIDHSTR.MergedBits.grpID_8
    #define SDIDH_ID                    SDIDH_ID_8
    
    #define SDIDH_ID8_MASK              1
    #define SDIDH_ID9_MASK              2
    #define SDIDH_ID10_MASK             4
    #define SDIDH_ID11_MASK             8
    #define SDIDH_ID_8_MASK             15
    #define SDIDH_ID_8_BITNUM           0
    

    /*** SDIDL - System Device Identification Register Low; 0x00001807 ***/
    union {
      byte Byte;
      struct {
        byte ID0         :1;                                       /* Part Identification Number, bit 0 */
        byte ID1         :1;                                       /* Part Identification Number, bit 1 */
        byte ID2         :1;                                       /* Part Identification Number, bit 2 */
        byte ID3         :1;                                       /* Part Identification Number, bit 3 */
        byte ID4         :1;                                       /* Part Identification Number, bit 4 */
        byte ID5         :1;                                       /* Part Identification Number, bit 5 */
        byte ID6         :1;                                       /* Part Identification Number, bit 6 */
        byte ID7         :1;                                       /* Part Identification Number, bit 7 */
      } Bits;
    } SDIDLSTR;
    #define SDIDL                       _SDID.Overlap_STR.SDIDLSTR.Byte
    #define SDIDL_ID0                   _SDID.Overlap_STR.SDIDLSTR.Bits.ID0
    #define SDIDL_ID1                   _SDID.Overlap_STR.SDIDLSTR.Bits.ID1
    #define SDIDL_ID2                   _SDID.Overlap_STR.SDIDLSTR.Bits.ID2
    #define SDIDL_ID3                   _SDID.Overlap_STR.SDIDLSTR.Bits.ID3
    #define SDIDL_ID4                   _SDID.Overlap_STR.SDIDLSTR.Bits.ID4
    #define SDIDL_ID5                   _SDID.Overlap_STR.SDIDLSTR.Bits.ID5
    #define SDIDL_ID6                   _SDID.Overlap_STR.SDIDLSTR.Bits.ID6
    #define SDIDL_ID7                   _SDID.Overlap_STR.SDIDLSTR.Bits.ID7
    
    #define SDIDL_ID0_MASK              1
    #define SDIDL_ID1_MASK              2
    #define SDIDL_ID2_MASK              4
    #define SDIDL_ID3_MASK              8
    #define SDIDL_ID4_MASK              16
    #define SDIDL_ID5_MASK              32
    #define SDIDL_ID6_MASK              64
    #define SDIDL_ID7_MASK              128
    
  } Overlap_STR;

} SDIDSTR;
extern volatile SDIDSTR _SDID @0x00001806;
#define SDID                            _SDID.Word


/*** SPMSC1 - System Power Management Status and Control 1 Register; 0x00001809 ***/
typedef union {
  byte Byte;
  struct {
    byte BGBE        :1;                                       /* Bandgap Buffer Enable */
    byte             :1; 
    byte LVDE        :1;                                       /* Low-Voltage Detect Enable */
    byte LVDSE       :1;                                       /* Low-Voltage Detect Stop Enable */
    byte LVDRE       :1;                                       /* Low-Voltage Detect Reset Enable */
    byte LVWIE       :1;                                       /* Low-Voltage Warning Interrupt Enable */
    byte LVWACK      :1;                                       /* Low-Voltage Warning Acknowledge */
    byte LVWF        :1;                                       /* Low-Voltage Warning status */
  } Bits;
} SPMSC1STR;
extern volatile SPMSC1STR _SPMSC1 @0x00001809;
#define SPMSC1                          _SPMSC1.Byte
#define SPMSC1_BGBE                     _SPMSC1.Bits.BGBE
#define SPMSC1_LVDE                     _SPMSC1.Bits.LVDE
#define SPMSC1_LVDSE                    _SPMSC1.Bits.LVDSE
#define SPMSC1_LVDRE                    _SPMSC1.Bits.LVDRE
#define SPMSC1_LVWIE                    _SPMSC1.Bits.LVWIE
#define SPMSC1_LVWACK                   _SPMSC1.Bits.LVWACK
#define SPMSC1_LVWF                     _SPMSC1.Bits.LVWF

#define SPMSC1_BGBE_MASK                1
#define SPMSC1_LVDE_MASK                4
#define SPMSC1_LVDSE_MASK               8
#define SPMSC1_LVDRE_MASK               16
#define SPMSC1_LVWIE_MASK               32
#define SPMSC1_LVWACK_MASK              64
#define SPMSC1_LVWF_MASK                128


/*** SPMSC2 - System Power Management Status and Control 2 Register; 0x0000180A ***/
typedef union {
  byte Byte;
  struct {
    byte PPDC        :1;                                       /* Partial Power Down Control */
    byte             :1; 
    byte PPDACK      :1;                                       /* Partial Power Down Acknowledge */
    byte PPDF        :1;                                       /* Partial Power Down Flag */
    byte LVWV        :1;                                       /* Low-Voltage Warning Voltage Select */
    byte LVDV        :1;                                       /* Low-Voltage Detect Voltage Select */
    byte             :1; 
    byte             :1; 
  } Bits;
} SPMSC2STR;
extern volatile SPMSC2STR _SPMSC2 @0x0000180A;
#define SPMSC2                          _SPMSC2.Byte
#define SPMSC2_PPDC                     _SPMSC2.Bits.PPDC
#define SPMSC2_PPDACK                   _SPMSC2.Bits.PPDACK
#define SPMSC2_PPDF                     _SPMSC2.Bits.PPDF
#define SPMSC2_LVWV                     _SPMSC2.Bits.LVWV
#define SPMSC2_LVDV                     _SPMSC2.Bits.LVDV

#define SPMSC2_PPDC_MASK                1
#define SPMSC2_PPDACK_MASK              4
#define SPMSC2_PPDF_MASK                8
#define SPMSC2_LVWV_MASK                16
#define SPMSC2_LVDV_MASK                32


/*** DBGCA - Debug Comparator A Register; 0x00001810 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** DBGCAH - Debug Comparator A High Register; 0x00001810 ***/
    union {
      byte Byte;
      struct {
        byte Bit8        :1;                                       /* Comparator A High Compare Bit 8 */
        byte Bit9        :1;                                       /* Comparator A High Compare Bit 9 */
        byte Bit10       :1;                                       /* Comparator A High Compare Bit 10 */
        byte Bit11       :1;                                       /* Comparator A High Compare Bit 11 */
        byte Bit12       :1;                                       /* Comparator A High Compare Bit 12 */
        byte Bit13       :1;                                       /* Comparator A High Compare Bit 13 */
        byte Bit14       :1;                                       /* Comparator A High Compare Bit 14 */
        byte Bit15       :1;                                       /* Comparator A High Compare Bit 15 */
      } Bits;
    } DBGCAHSTR;
    #define DBGCAH                      _DBGCA.Overlap_STR.DBGCAHSTR.Byte
    #define DBGCAH_Bit8                 _DBGCA.Overlap_STR.DBGCAHSTR.Bits.Bit8
    #define DBGCAH_Bit9                 _DBGCA.Overlap_STR.DBGCAHSTR.Bits.Bit9
    #define DBGCAH_Bit10                _DBGCA.Overlap_STR.DBGCAHSTR.Bits.Bit10
    #define DBGCAH_Bit11                _DBGCA.Overlap_STR.DBGCAHSTR.Bits.Bit11
    #define DBGCAH_Bit12                _DBGCA.Overlap_STR.DBGCAHSTR.Bits.Bit12
    #define DBGCAH_Bit13                _DBGCA.Overlap_STR.DBGCAHSTR.Bits.Bit13
    #define DBGCAH_Bit14                _DBGCA.Overlap_STR.DBGCAHSTR.Bits.Bit14
    #define DBGCAH_Bit15                _DBGCA.Overlap_STR.DBGCAHSTR.Bits.Bit15
    
    #define DBGCAH_Bit8_MASK            1
    #define DBGCAH_Bit9_MASK            2
    #define DBGCAH_Bit10_MASK           4
    #define DBGCAH_Bit11_MASK           8
    #define DBGCAH_Bit12_MASK           16
    #define DBGCAH_Bit13_MASK           32
    #define DBGCAH_Bit14_MASK           64
    #define DBGCAH_Bit15_MASK           128
    

    /*** DBGCAL - Debug Comparator A Low Register; 0x00001811 ***/
    union {
      byte Byte;
      struct {
        byte Bit0        :1;                                       /* Comparator A Low Compare Bit 0 */
        byte Bit1        :1;                                       /* Comparator A Low Compare Bit 1 */
        byte Bit2        :1;                                       /* Comparator A Low Compare Bit 2 */
        byte Bit3        :1;                                       /* Comparator A Low Compare Bit 3 */
        byte Bit4        :1;                                       /* Comparator A Low Compare Bit 4 */
        byte Bit5        :1;                                       /* Comparator A Low Compare Bit 5 */
        byte Bit6        :1;                                       /* Comparator A Low Compare Bit 6 */
        byte Bit7        :1;                                       /* Comparator A Low Compare Bit 7 */
      } Bits;
    } DBGCALSTR;
    #define DBGCAL                      _DBGCA.Overlap_STR.DBGCALSTR.Byte
    #define DBGCAL_Bit0                 _DBGCA.Overlap_STR.DBGCALSTR.Bits.Bit0
    #define DBGCAL_Bit1                 _DBGCA.Overlap_STR.DBGCALSTR.Bits.Bit1
    #define DBGCAL_Bit2                 _DBGCA.Overlap_STR.DBGCALSTR.Bits.Bit2
    #define DBGCAL_Bit3                 _DBGCA.Overlap_STR.DBGCALSTR.Bits.Bit3
    #define DBGCAL_Bit4                 _DBGCA.Overlap_STR.DBGCALSTR.Bits.Bit4
    #define DBGCAL_Bit5                 _DBGCA.Overlap_STR.DBGCALSTR.Bits.Bit5
    #define DBGCAL_Bit6                 _DBGCA.Overlap_STR.DBGCALSTR.Bits.Bit6
    #define DBGCAL_Bit7                 _DBGCA.Overlap_STR.DBGCALSTR.Bits.Bit7
    
    #define DBGCAL_Bit0_MASK            1
    #define DBGCAL_Bit1_MASK            2
    #define DBGCAL_Bit2_MASK            4
    #define DBGCAL_Bit3_MASK            8
    #define DBGCAL_Bit4_MASK            16
    #define DBGCAL_Bit5_MASK            32
    #define DBGCAL_Bit6_MASK            64
    #define DBGCAL_Bit7_MASK            128
    
  } Overlap_STR;

} DBGCASTR;
extern volatile DBGCASTR _DBGCA @0x00001810;
#define DBGCA                           _DBGCA.Word


/*** DBGCB - Debug Comparator B Register; 0x00001812 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** DBGCBH - Debug Comparator B High Register; 0x00001812 ***/
    union {
      byte Byte;
      struct {
        byte Bit8        :1;                                       /* Comparator B High Compare Bit 8 */
        byte Bit9        :1;                                       /* Comparator B High Compare Bit 9 */
        byte Bit10       :1;                                       /* Comparator B High Compare Bit 10 */
        byte Bit11       :1;                                       /* Comparator B High Compare Bit 11 */
        byte Bit12       :1;                                       /* Comparator B High Compare Bit 12 */
        byte Bit13       :1;                                       /* Comparator B High Compare Bit 13 */
        byte Bit14       :1;                                       /* Comparator B High Compare Bit 14 */
        byte Bit15       :1;                                       /* Comparator B High Compare Bit 15 */
      } Bits;
    } DBGCBHSTR;
    #define DBGCBH                      _DBGCB.Overlap_STR.DBGCBHSTR.Byte
    #define DBGCBH_Bit8                 _DBGCB.Overlap_STR.DBGCBHSTR.Bits.Bit8
    #define DBGCBH_Bit9                 _DBGCB.Overlap_STR.DBGCBHSTR.Bits.Bit9
    #define DBGCBH_Bit10                _DBGCB.Overlap_STR.DBGCBHSTR.Bits.Bit10
    #define DBGCBH_Bit11                _DBGCB.Overlap_STR.DBGCBHSTR.Bits.Bit11
    #define DBGCBH_Bit12                _DBGCB.Overlap_STR.DBGCBHSTR.Bits.Bit12
    #define DBGCBH_Bit13                _DBGCB.Overlap_STR.DBGCBHSTR.Bits.Bit13
    #define DBGCBH_Bit14                _DBGCB.Overlap_STR.DBGCBHSTR.Bits.Bit14
    #define DBGCBH_Bit15                _DBGCB.Overlap_STR.DBGCBHSTR.Bits.Bit15
    
    #define DBGCBH_Bit8_MASK            1
    #define DBGCBH_Bit9_MASK            2
    #define DBGCBH_Bit10_MASK           4
    #define DBGCBH_Bit11_MASK           8
    #define DBGCBH_Bit12_MASK           16
    #define DBGCBH_Bit13_MASK           32
    #define DBGCBH_Bit14_MASK           64
    #define DBGCBH_Bit15_MASK           128
    

    /*** DBGCBL - Debug Comparator B Low Register; 0x00001813 ***/
    union {
      byte Byte;
      struct {
        byte Bit0        :1;                                       /* Comparator B Low Compare Bit 0 */
        byte Bit1        :1;                                       /* Comparator B Low Compare Bit 1 */
        byte Bit2        :1;                                       /* Comparator B Low Compare Bit 2 */
        byte Bit3        :1;                                       /* Comparator B Low Compare Bit 3 */
        byte Bit4        :1;                                       /* Comparator B Low Compare Bit 4 */
        byte Bit5        :1;                                       /* Comparator B Low Compare Bit 5 */
        byte Bit6        :1;                                       /* Comparator B Low Compare Bit 6 */
        byte Bit7        :1;                                       /* Comparator B Low Compare Bit 7 */
      } Bits;
    } DBGCBLSTR;
    #define DBGCBL                      _DBGCB.Overlap_STR.DBGCBLSTR.Byte
    #define DBGCBL_Bit0                 _DBGCB.Overlap_STR.DBGCBLSTR.Bits.Bit0
    #define DBGCBL_Bit1                 _DBGCB.Overlap_STR.DBGCBLSTR.Bits.Bit1
    #define DBGCBL_Bit2                 _DBGCB.Overlap_STR.DBGCBLSTR.Bits.Bit2
    #define DBGCBL_Bit3                 _DBGCB.Overlap_STR.DBGCBLSTR.Bits.Bit3
    #define DBGCBL_Bit4                 _DBGCB.Overlap_STR.DBGCBLSTR.Bits.Bit4
    #define DBGCBL_Bit5                 _DBGCB.Overlap_STR.DBGCBLSTR.Bits.Bit5
    #define DBGCBL_Bit6                 _DBGCB.Overlap_STR.DBGCBLSTR.Bits.Bit6
    #define DBGCBL_Bit7                 _DBGCB.Overlap_STR.DBGCBLSTR.Bits.Bit7
    
    #define DBGCBL_Bit0_MASK            1
    #define DBGCBL_Bit1_MASK            2
    #define DBGCBL_Bit2_MASK            4
    #define DBGCBL_Bit3_MASK            8
    #define DBGCBL_Bit4_MASK            16
    #define DBGCBL_Bit5_MASK            32
    #define DBGCBL_Bit6_MASK            64
    #define DBGCBL_Bit7_MASK            128
    
  } Overlap_STR;

} DBGCBSTR;
extern volatile DBGCBSTR _DBGCB @0x00001812;
#define DBGCB                           _DBGCB.Word


/*** DBGF - Debug FIFO Register; 0x00001814 ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** DBGFH - Debug FIFO High Register; 0x00001814 ***/
    union {
      byte Byte;
      struct {
        byte Bit8        :1;                                       /* FIFO High Data Bit 8 */
        byte Bit9        :1;                                       /* FIFO High Data Bit 9 */
        byte Bit10       :1;                                       /* FIFO High Data Bit 10 */
        byte Bit11       :1;                                       /* FIFO High Data Bit 11 */
        byte Bit12       :1;                                       /* FIFO High Data Bit 12 */
        byte Bit13       :1;                                       /* FIFO High Data Bit 13 */
        byte Bit14       :1;                                       /* FIFO High Data Bit 14 */
        byte Bit15       :1;                                       /* FIFO High Data Bit 15 */
      } Bits;
    } DBGFHSTR;
    #define DBGFH                       _DBGF.Overlap_STR.DBGFHSTR.Byte
    #define DBGFH_Bit8                  _DBGF.Overlap_STR.DBGFHSTR.Bits.Bit8
    #define DBGFH_Bit9                  _DBGF.Overlap_STR.DBGFHSTR.Bits.Bit9
    #define DBGFH_Bit10                 _DBGF.Overlap_STR.DBGFHSTR.Bits.Bit10
    #define DBGFH_Bit11                 _DBGF.Overlap_STR.DBGFHSTR.Bits.Bit11
    #define DBGFH_Bit12                 _DBGF.Overlap_STR.DBGFHSTR.Bits.Bit12
    #define DBGFH_Bit13                 _DBGF.Overlap_STR.DBGFHSTR.Bits.Bit13
    #define DBGFH_Bit14                 _DBGF.Overlap_STR.DBGFHSTR.Bits.Bit14
    #define DBGFH_Bit15                 _DBGF.Overlap_STR.DBGFHSTR.Bits.Bit15
    
    #define DBGFH_Bit8_MASK             1
    #define DBGFH_Bit9_MASK             2
    #define DBGFH_Bit10_MASK            4
    #define DBGFH_Bit11_MASK            8
    #define DBGFH_Bit12_MASK            16
    #define DBGFH_Bit13_MASK            32
    #define DBGFH_Bit14_MASK            64
    #define DBGFH_Bit15_MASK            128
    

    /*** DBGFL - Debug FIFO Low Register; 0x00001815 ***/
    union {
      byte Byte;
      struct {
        byte Bit0        :1;                                       /* FIFO Low Data Bit 0 */
        byte Bit1        :1;                                       /* FIFO Low Data Bit 1 */
        byte Bit2        :1;                                       /* FIFO Low Data Bit 2 */
        byte Bit3        :1;                                       /* FIFO Low Data Bit 3 */
        byte Bit4        :1;                                       /* FIFO Low Data Bit 4 */
        byte Bit5        :1;                                       /* FIFO Low Data Bit 5 */
        byte Bit6        :1;                                       /* FIFO Low Data Bit 6 */
        byte Bit7        :1;                                       /* FIFO Low Data Bit 7 */
      } Bits;
    } DBGFLSTR;
    #define DBGFL                       _DBGF.Overlap_STR.DBGFLSTR.Byte
    #define DBGFL_Bit0                  _DBGF.Overlap_STR.DBGFLSTR.Bits.Bit0
    #define DBGFL_Bit1                  _DBGF.Overlap_STR.DBGFLSTR.Bits.Bit1
    #define DBGFL_Bit2                  _DBGF.Overlap_STR.DBGFLSTR.Bits.Bit2
    #define DBGFL_Bit3                  _DBGF.Overlap_STR.DBGFLSTR.Bits.Bit3
    #define DBGFL_Bit4                  _DBGF.Overlap_STR.DBGFLSTR.Bits.Bit4
    #define DBGFL_Bit5                  _DBGF.Overlap_STR.DBGFLSTR.Bits.Bit5
    #define DBGFL_Bit6                  _DBGF.Overlap_STR.DBGFLSTR.Bits.Bit6
    #define DBGFL_Bit7                  _DBGF.Overlap_STR.DBGFLSTR.Bits.Bit7
    
    #define DBGFL_Bit0_MASK             1
    #define DBGFL_Bit1_MASK             2
    #define DBGFL_Bit2_MASK             4
    #define DBGFL_Bit3_MASK             8
    #define DBGFL_Bit4_MASK             16
    #define DBGFL_Bit5_MASK             32
    #define DBGFL_Bit6_MASK             64
    #define DBGFL_Bit7_MASK             128
    
  } Overlap_STR;

} DBGFSTR;
extern volatile DBGFSTR _DBGF @0x00001814;
#define DBGF                            _DBGF.Word


/*** DBGC - Debug Control Register; 0x00001816 ***/
typedef union {
  byte Byte;
  struct {
    byte RWBEN       :1;                                       /* Enable R/W for Comparator B */
    byte RWB         :1;                                       /* R/W Comparison Value for Comparator B */
    byte RWAEN       :1;                                       /* Enable R/W for Comparator A */
    byte RWA         :1;                                       /* R/W Comparison Value for Comparator A */
    byte BRKEN       :1;                                       /* Break Enable */
    byte TAG         :1;                                       /* Tag/Force Select */
    byte ARM         :1;                                       /* Arm Control */
    byte DBGEN       :1;                                       /* Debug Module Enable */
  } Bits;
} DBGCSTR;
extern volatile DBGCSTR _DBGC @0x00001816;
#define DBGC                            _DBGC.Byte
#define DBGC_RWBEN                      _DBGC.Bits.RWBEN
#define DBGC_RWB                        _DBGC.Bits.RWB
#define DBGC_RWAEN                      _DBGC.Bits.RWAEN
#define DBGC_RWA                        _DBGC.Bits.RWA
#define DBGC_BRKEN                      _DBGC.Bits.BRKEN
#define DBGC_TAG                        _DBGC.Bits.TAG
#define DBGC_ARM                        _DBGC.Bits.ARM
#define DBGC_DBGEN                      _DBGC.Bits.DBGEN

#define DBGC_RWBEN_MASK                 1
#define DBGC_RWB_MASK                   2
#define DBGC_RWAEN_MASK                 4
#define DBGC_RWA_MASK                   8
#define DBGC_BRKEN_MASK                 16
#define DBGC_TAG_MASK                   32
#define DBGC_ARM_MASK                   64
#define DBGC_DBGEN_MASK                 128


/*** DBGT - Debug Trigger Register; 0x00001817 ***/
typedef union {
  byte Byte;
  struct {
    byte TRG0        :1;                                       /* Select Trigger Mode Bit 0 */
    byte TRG1        :1;                                       /* Select Trigger Mode Bit 1 */
    byte TRG2        :1;                                       /* Select Trigger Mode Bit 2 */
    byte TRG3        :1;                                       /* Select Trigger Mode Bit 3 */
    byte             :1; 
    byte             :1; 
    byte BEGIN       :1;                                       /* Begin/End Trigger Select */
    byte TRGSEL      :1;                                       /* Trigger Type */
  } Bits;
  struct {
    byte grpTRG  :4;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} DBGTSTR;
extern volatile DBGTSTR _DBGT @0x00001817;
#define DBGT                            _DBGT.Byte
#define DBGT_TRG0                       _DBGT.Bits.TRG0
#define DBGT_TRG1                       _DBGT.Bits.TRG1
#define DBGT_TRG2                       _DBGT.Bits.TRG2
#define DBGT_TRG3                       _DBGT.Bits.TRG3
#define DBGT_BEGIN                      _DBGT.Bits.BEGIN
#define DBGT_TRGSEL                     _DBGT.Bits.TRGSEL
#define DBGT_TRG                        _DBGT.MergedBits.grpTRG

#define DBGT_TRG0_MASK                  1
#define DBGT_TRG1_MASK                  2
#define DBGT_TRG2_MASK                  4
#define DBGT_TRG3_MASK                  8
#define DBGT_BEGIN_MASK                 64
#define DBGT_TRGSEL_MASK                128
#define DBGT_TRG_MASK                   15
#define DBGT_TRG_BITNUM                 0


/*** DBGS - Debug Status Register; 0x00001818 ***/
typedef union {
  byte Byte;
  struct {
    byte CNT0        :1;                                       /* FIFO Valid Count Bit 0 */
    byte CNT1        :1;                                       /* FIFO Valid Count Bit 1 */
    byte CNT2        :1;                                       /* FIFO Valid Count Bit 2 */
    byte CNT3        :1;                                       /* FIFO Valid Count Bit 3 */
    byte             :1; 
    byte ARMF        :1;                                       /* Arm Flag */
    byte BF          :1;                                       /* Trigger Match B Flag */
    byte AF          :1;                                       /* Trigger Match A Flag */
  } Bits;
  struct {
    byte grpCNT  :4;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} DBGSSTR;
extern volatile DBGSSTR _DBGS @0x00001818;
#define DBGS                            _DBGS.Byte
#define DBGS_CNT0                       _DBGS.Bits.CNT0
#define DBGS_CNT1                       _DBGS.Bits.CNT1
#define DBGS_CNT2                       _DBGS.Bits.CNT2
#define DBGS_CNT3                       _DBGS.Bits.CNT3
#define DBGS_ARMF                       _DBGS.Bits.ARMF
#define DBGS_BF                         _DBGS.Bits.BF
#define DBGS_AF                         _DBGS.Bits.AF
#define DBGS_CNT                        _DBGS.MergedBits.grpCNT

#define DBGS_CNT0_MASK                  1
#define DBGS_CNT1_MASK                  2
#define DBGS_CNT2_MASK                  4
#define DBGS_CNT3_MASK                  8
#define DBGS_ARMF_MASK                  32
#define DBGS_BF_MASK                    64
#define DBGS_AF_MASK                    128
#define DBGS_CNT_MASK                   15
#define DBGS_CNT_BITNUM                 0


/*** FCDIV - EEPROM and FLASH Clock Divider Register; 0x00001820 ***/
typedef union {
  byte Byte;
  struct {
    byte DIV0        :1;                                       /* Clock Divider Bit 0 */
    byte DIV1        :1;                                       /* Clock Divider Bit 1 */
    byte DIV2        :1;                                       /* Clock Divider Bit 2 */
    byte DIV3        :1;                                       /* Clock Divider Bit 3 */
    byte DIV4        :1;                                       /* Clock Divider Bit 4 */
    byte DIV5        :1;                                       /* Clock Divider Bit 5 */
    byte PRDIV8      :1;                                       /* Enable Prescaler by 8 */
    byte DIVLD       :1;                                       /* Clock Divider Load Control */
  } Bits;
  struct {
    byte grpDIV  :6;
    byte grpPRDIV_8 :1;
    byte         :1;
  } MergedBits;
} FCDIVSTR;
extern volatile FCDIVSTR _FCDIV @0x00001820;
#define FCDIV                           _FCDIV.Byte
#define FCDIV_DIV0                      _FCDIV.Bits.DIV0
#define FCDIV_DIV1                      _FCDIV.Bits.DIV1
#define FCDIV_DIV2                      _FCDIV.Bits.DIV2
#define FCDIV_DIV3                      _FCDIV.Bits.DIV3
#define FCDIV_DIV4                      _FCDIV.Bits.DIV4
#define FCDIV_DIV5                      _FCDIV.Bits.DIV5
#define FCDIV_PRDIV8                    _FCDIV.Bits.PRDIV8
#define FCDIV_DIVLD                     _FCDIV.Bits.DIVLD
#define FCDIV_DIV                       _FCDIV.MergedBits.grpDIV

#define FCDIV_DIV0_MASK                 1
#define FCDIV_DIV1_MASK                 2
#define FCDIV_DIV2_MASK                 4
#define FCDIV_DIV3_MASK                 8
#define FCDIV_DIV4_MASK                 16
#define FCDIV_DIV5_MASK                 32
#define FCDIV_PRDIV8_MASK               64
#define FCDIV_DIVLD_MASK                128
#define FCDIV_DIV_MASK                  63
#define FCDIV_DIV_BITNUM                0


/*** FOPT - EEPROM and FLASH Options Register; 0x00001821 ***/
typedef union {
  byte Byte;
  struct {
    byte SEC0        :1;                                       /* Flash Security Bit 0 */
    byte SEC1        :1;                                       /* Flash Security Bit 1 */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte EPGMOD      :1;                                       /* EEPROM Sector Mode Bit */
    byte FNORED      :1;                                       /* Vector Redirection Disable Bit */
    byte KEYEN       :1;                                       /* Backdoor Key Security Enable Bit */
  } Bits;
  struct {
    byte grpSEC  :2;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} FOPTSTR;
extern volatile FOPTSTR _FOPT @0x00001821;
#define FOPT                            _FOPT.Byte
#define FOPT_SEC0                       _FOPT.Bits.SEC0
#define FOPT_SEC1                       _FOPT.Bits.SEC1
#define FOPT_EPGMOD                     _FOPT.Bits.EPGMOD
#define FOPT_FNORED                     _FOPT.Bits.FNORED
#define FOPT_KEYEN                      _FOPT.Bits.KEYEN
#define FOPT_SEC                        _FOPT.MergedBits.grpSEC

#define FOPT_SEC0_MASK                  1
#define FOPT_SEC1_MASK                  2
#define FOPT_EPGMOD_MASK                32
#define FOPT_FNORED_MASK                64
#define FOPT_KEYEN_MASK                 128
#define FOPT_SEC_MASK                   3
#define FOPT_SEC_BITNUM                 0


/*** FCNFG - EEPROM and FLASH Configuration Register; 0x00001823 ***/
typedef union {
  byte Byte;
  struct {
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte KEYACC      :1;                                       /* Enable Security Key Writing */
    byte EPGSEL      :1;                                       /* EEPROM Page Select Bit */
    byte             :1; 
  } Bits;
} FCNFGSTR;
extern volatile FCNFGSTR _FCNFG @0x00001823;
#define FCNFG                           _FCNFG.Byte
#define FCNFG_KEYACC                    _FCNFG.Bits.KEYACC
#define FCNFG_EPGSEL                    _FCNFG.Bits.EPGSEL

#define FCNFG_KEYACC_MASK               32
#define FCNFG_EPGSEL_MASK               64


/*** FPROT - EEPROM and FLASH Protection Register; 0x00001824 ***/
typedef union {
  byte Byte;
  struct {
    byte FPS0        :1;                                       /* Flash Protection Select Bits, bit 0 */
    byte FPS1        :1;                                       /* Flash Protection Select Bits, bit 1 */
    byte FPS2        :1;                                       /* Flash Protection Select Bits, bit 2 */
    byte FPS3        :1;                                       /* Flash Protection Select Bits, bit 3 */
    byte FPS4        :1;                                       /* Flash Protection Select Bits, bit 4 */
    byte FPS5        :1;                                       /* Flash Protection Select Bits, bit 5 */
    byte EPS0        :1;                                       /* EEPROM Protect Select Bits, bit 0 */
    byte EPS1        :1;                                       /* EEPROM Protect Select Bits, bit 1 */
  } Bits;
  struct {
    byte grpFPS  :6;
    byte grpEPS  :2;
  } MergedBits;
} FPROTSTR;
extern volatile FPROTSTR _FPROT @0x00001824;
#define FPROT                           _FPROT.Byte
#define FPROT_FPS0                      _FPROT.Bits.FPS0
#define FPROT_FPS1                      _FPROT.Bits.FPS1
#define FPROT_FPS2                      _FPROT.Bits.FPS2
#define FPROT_FPS3                      _FPROT.Bits.FPS3
#define FPROT_FPS4                      _FPROT.Bits.FPS4
#define FPROT_FPS5                      _FPROT.Bits.FPS5
#define FPROT_EPS0                      _FPROT.Bits.EPS0
#define FPROT_EPS1                      _FPROT.Bits.EPS1
#define FPROT_FPS                       _FPROT.MergedBits.grpFPS
#define FPROT_EPS                       _FPROT.MergedBits.grpEPS

#define FPROT_FPS0_MASK                 1
#define FPROT_FPS1_MASK                 2
#define FPROT_FPS2_MASK                 4
#define FPROT_FPS3_MASK                 8
#define FPROT_FPS4_MASK                 16
#define FPROT_FPS5_MASK                 32
#define FPROT_EPS0_MASK                 64
#define FPROT_EPS1_MASK                 128
#define FPROT_FPS_MASK                  63
#define FPROT_FPS_BITNUM                0
#define FPROT_EPS_MASK                  192
#define FPROT_EPS_BITNUM                6


/*** FSTAT - EEPROM and FLASH Status Register; 0x00001825 ***/
typedef union {
  byte Byte;
  struct {
    byte             :1; 
    byte             :1; 
    byte FBLANK      :1;                                       /* FLASH Flag Indicating the Erase Verify Operation Status */
    byte             :1; 
    byte FACCERR     :1;                                       /* FLASH Access Error Flag */
    byte FPVIOL      :1;                                       /* FLASH Protection Violation Flag */
    byte FCCF        :1;                                       /* FLASH Command Complete Interrupt Flag */
    byte FCBEF       :1;                                       /* FLASH Command Buffer Empty Flag */
  } Bits;
} FSTATSTR;
extern volatile FSTATSTR _FSTAT @0x00001825;
#define FSTAT                           _FSTAT.Byte
#define FSTAT_FBLANK                    _FSTAT.Bits.FBLANK
#define FSTAT_FACCERR                   _FSTAT.Bits.FACCERR
#define FSTAT_FPVIOL                    _FSTAT.Bits.FPVIOL
#define FSTAT_FCCF                      _FSTAT.Bits.FCCF
#define FSTAT_FCBEF                     _FSTAT.Bits.FCBEF

#define FSTAT_FBLANK_MASK               4
#define FSTAT_FACCERR_MASK              16
#define FSTAT_FPVIOL_MASK               32
#define FSTAT_FCCF_MASK                 64
#define FSTAT_FCBEF_MASK                128


/*** FCMD - EEPROM and FLASH Command Register; 0x00001826 ***/
typedef union {
  byte Byte;
  struct {
    byte FCMD0       :1;                                       /* Flash Command, bit 0 */
    byte FCMD1       :1;                                       /* Flash Command, bit 1 */
    byte FCMD2       :1;                                       /* Flash Command, bit 2 */
    byte FCMD3       :1;                                       /* Flash Command, bit 3 */
    byte FCMD4       :1;                                       /* Flash Command, bit 4 */
    byte FCMD5       :1;                                       /* Flash Command, bit 5 */
    byte FCMD6       :1;                                       /* Flash Command, bit 6 */
    byte FCMD7       :1;                                       /* Flash Command, bit 7 */
  } Bits;
} FCMDSTR;
extern volatile FCMDSTR _FCMD @0x00001826;
#define FCMD                            _FCMD.Byte
#define FCMD_FCMD0                      _FCMD.Bits.FCMD0
#define FCMD_FCMD1                      _FCMD.Bits.FCMD1
#define FCMD_FCMD2                      _FCMD.Bits.FCMD2
#define FCMD_FCMD3                      _FCMD.Bits.FCMD3
#define FCMD_FCMD4                      _FCMD.Bits.FCMD4
#define FCMD_FCMD5                      _FCMD.Bits.FCMD5
#define FCMD_FCMD6                      _FCMD.Bits.FCMD6
#define FCMD_FCMD7                      _FCMD.Bits.FCMD7

#define FCMD_FCMD0_MASK                 1
#define FCMD_FCMD1_MASK                 2
#define FCMD_FCMD2_MASK                 4
#define FCMD_FCMD3_MASK                 8
#define FCMD_FCMD4_MASK                 16
#define FCMD_FCMD5_MASK                 32
#define FCMD_FCMD6_MASK                 64
#define FCMD_FCMD7_MASK                 128


/*** PTAPE - Port A Pull Enable Register; 0x00001840 ***/
typedef union {
  byte Byte;
  struct {
    byte PTAPE0      :1;                                       /* Internal Pull Enable for Port A Bit 0 */
    byte PTAPE1      :1;                                       /* Internal Pull Enable for Port A Bit 1 */
    byte PTAPE2      :1;                                       /* Internal Pull Enable for Port A Bit 2 */
    byte PTAPE3      :1;                                       /* Internal Pull Enable for Port A Bit 3 */
    byte PTAPE4      :1;                                       /* Internal Pull Enable for Port A Bit 4 */
    byte PTAPE5      :1;                                       /* Internal Pull Enable for Port A Bit 5 */
    byte PTAPE6      :1;                                       /* Internal Pull Enable for Port A Bit 6 */
    byte PTAPE7      :1;                                       /* Internal Pull Enable for Port A Bit 7 */
  } Bits;
} PTAPESTR;
extern volatile PTAPESTR _PTAPE @0x00001840;
#define PTAPE                           _PTAPE.Byte
#define PTAPE_PTAPE0                    _PTAPE.Bits.PTAPE0
#define PTAPE_PTAPE1                    _PTAPE.Bits.PTAPE1
#define PTAPE_PTAPE2                    _PTAPE.Bits.PTAPE2
#define PTAPE_PTAPE3                    _PTAPE.Bits.PTAPE3
#define PTAPE_PTAPE4                    _PTAPE.Bits.PTAPE4
#define PTAPE_PTAPE5                    _PTAPE.Bits.PTAPE5
#define PTAPE_PTAPE6                    _PTAPE.Bits.PTAPE6
#define PTAPE_PTAPE7                    _PTAPE.Bits.PTAPE7

#define PTAPE_PTAPE0_MASK               1
#define PTAPE_PTAPE1_MASK               2
#define PTAPE_PTAPE2_MASK               4
#define PTAPE_PTAPE3_MASK               8
#define PTAPE_PTAPE4_MASK               16
#define PTAPE_PTAPE5_MASK               32
#define PTAPE_PTAPE6_MASK               64
#define PTAPE_PTAPE7_MASK               128


/*** PTASE - Port A Slew Rate Enable Register; 0x00001841 ***/
typedef union {
  byte Byte;
  struct {
    byte PTASE0      :1;                                       /* Output Slew Rate Enable for Port A Bit 0 */
    byte PTASE1      :1;                                       /* Output Slew Rate Enable for Port A Bit 1 */
    byte PTASE2      :1;                                       /* Output Slew Rate Enable for Port A Bit 2 */
    byte PTASE3      :1;                                       /* Output Slew Rate Enable for Port A Bit 3 */
    byte PTASE4      :1;                                       /* Output Slew Rate Enable for Port A Bit 4 */
    byte PTASE5      :1;                                       /* Output Slew Rate Enable for Port A Bit 5 */
    byte PTASE6      :1;                                       /* Output Slew Rate Enable for Port A Bit 6 */
    byte PTASE7      :1;                                       /* Output Slew Rate Enable for Port A Bit 7 */
  } Bits;
} PTASESTR;
extern volatile PTASESTR _PTASE @0x00001841;
#define PTASE                           _PTASE.Byte
#define PTASE_PTASE0                    _PTASE.Bits.PTASE0
#define PTASE_PTASE1                    _PTASE.Bits.PTASE1
#define PTASE_PTASE2                    _PTASE.Bits.PTASE2
#define PTASE_PTASE3                    _PTASE.Bits.PTASE3
#define PTASE_PTASE4                    _PTASE.Bits.PTASE4
#define PTASE_PTASE5                    _PTASE.Bits.PTASE5
#define PTASE_PTASE6                    _PTASE.Bits.PTASE6
#define PTASE_PTASE7                    _PTASE.Bits.PTASE7

#define PTASE_PTASE0_MASK               1
#define PTASE_PTASE1_MASK               2
#define PTASE_PTASE2_MASK               4
#define PTASE_PTASE3_MASK               8
#define PTASE_PTASE4_MASK               16
#define PTASE_PTASE5_MASK               32
#define PTASE_PTASE6_MASK               64
#define PTASE_PTASE7_MASK               128


/*** PTADS - Port A Drive Strength Selection Register; 0x00001842 ***/
typedef union {
  byte Byte;
  struct {
    byte PTADS0      :1;                                       /* Output Drive Strength Selection for Port A Bit 0 */
    byte PTADS1      :1;                                       /* Output Drive Strength Selection for Port A Bit 1 */
    byte PTADS2      :1;                                       /* Output Drive Strength Selection for Port A Bit 2 */
    byte PTADS3      :1;                                       /* Output Drive Strength Selection for Port A Bit 3 */
    byte PTADS4      :1;                                       /* Output Drive Strength Selection for Port A Bit 4 */
    byte PTADS5      :1;                                       /* Output Drive Strength Selection for Port A Bit 5 */
    byte PTADS6      :1;                                       /* Output Drive Strength Selection for Port A Bit 6 */
    byte PTADS7      :1;                                       /* Output Drive Strength Selection for Port A Bit 7 */
  } Bits;
} PTADSSTR;
extern volatile PTADSSTR _PTADS @0x00001842;
#define PTADS                           _PTADS.Byte
#define PTADS_PTADS0                    _PTADS.Bits.PTADS0
#define PTADS_PTADS1                    _PTADS.Bits.PTADS1
#define PTADS_PTADS2                    _PTADS.Bits.PTADS2
#define PTADS_PTADS3                    _PTADS.Bits.PTADS3
#define PTADS_PTADS4                    _PTADS.Bits.PTADS4
#define PTADS_PTADS5                    _PTADS.Bits.PTADS5
#define PTADS_PTADS6                    _PTADS.Bits.PTADS6
#define PTADS_PTADS7                    _PTADS.Bits.PTADS7

#define PTADS_PTADS0_MASK               1
#define PTADS_PTADS1_MASK               2
#define PTADS_PTADS2_MASK               4
#define PTADS_PTADS3_MASK               8
#define PTADS_PTADS4_MASK               16
#define PTADS_PTADS5_MASK               32
#define PTADS_PTADS6_MASK               64
#define PTADS_PTADS7_MASK               128


/*** PTASC - Port A Interrupt Status and Control Register; 0x00001844 ***/
typedef union {
  byte Byte;
  struct {
    byte PTAMOD      :1;                                       /* Port A Detection Mode */
    byte PTAIE       :1;                                       /* Port A Interrupt Enable */
    byte PTAACK      :1;                                       /* Port A Interrupt Acknowledge */
    byte PTAIF       :1;                                       /* Port A Interrupt Flag */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
  } Bits;
} PTASCSTR;
extern volatile PTASCSTR _PTASC @0x00001844;
#define PTASC                           _PTASC.Byte
#define PTASC_PTAMOD                    _PTASC.Bits.PTAMOD
#define PTASC_PTAIE                     _PTASC.Bits.PTAIE
#define PTASC_PTAACK                    _PTASC.Bits.PTAACK
#define PTASC_PTAIF                     _PTASC.Bits.PTAIF

#define PTASC_PTAMOD_MASK               1
#define PTASC_PTAIE_MASK                2
#define PTASC_PTAACK_MASK               4
#define PTASC_PTAIF_MASK                8


/*** PTAPS - Port A Interrupt Pin Select Register; 0x00001845 ***/
typedef union {
  byte Byte;
  struct {
    byte PTAPS0      :1;                                       /* Port A Interrupt Pin Select Bit 0 */
    byte PTAPS1      :1;                                       /* Port A Interrupt Pin Select Bit 1 */
    byte PTAPS2      :1;                                       /* Port A Interrupt Pin Select Bit 2 */
    byte PTAPS3      :1;                                       /* Port A Interrupt Pin Select Bit 3 */
    byte PTAPS4      :1;                                       /* Port A Interrupt Pin Select Bit 4 */
    byte PTAPS5      :1;                                       /* Port A Interrupt Pin Select Bit 5 */
    byte PTAPS6      :1;                                       /* Port A Interrupt Pin Select Bit 6 */
    byte PTAPS7      :1;                                       /* Port A Interrupt Pin Select Bit 7 */
  } Bits;
} PTAPSSTR;
extern volatile PTAPSSTR _PTAPS @0x00001845;
#define PTAPS                           _PTAPS.Byte
#define PTAPS_PTAPS0                    _PTAPS.Bits.PTAPS0
#define PTAPS_PTAPS1                    _PTAPS.Bits.PTAPS1
#define PTAPS_PTAPS2                    _PTAPS.Bits.PTAPS2
#define PTAPS_PTAPS3                    _PTAPS.Bits.PTAPS3
#define PTAPS_PTAPS4                    _PTAPS.Bits.PTAPS4
#define PTAPS_PTAPS5                    _PTAPS.Bits.PTAPS5
#define PTAPS_PTAPS6                    _PTAPS.Bits.PTAPS6
#define PTAPS_PTAPS7                    _PTAPS.Bits.PTAPS7

#define PTAPS_PTAPS0_MASK               1
#define PTAPS_PTAPS1_MASK               2
#define PTAPS_PTAPS2_MASK               4
#define PTAPS_PTAPS3_MASK               8
#define PTAPS_PTAPS4_MASK               16
#define PTAPS_PTAPS5_MASK               32
#define PTAPS_PTAPS6_MASK               64
#define PTAPS_PTAPS7_MASK               128


/*** PTAES - Port A Interrupt Edge Select Register; 0x00001846 ***/
typedef union {
  byte Byte;
  struct {
    byte PTAES0      :1;                                       /* Port A Edge Select Bit 0 */
    byte PTAES1      :1;                                       /* Port A Edge Select Bit 1 */
    byte PTAES2      :1;                                       /* Port A Edge Select Bit 2 */
    byte PTAES3      :1;                                       /* Port A Edge Select Bit 3 */
    byte PTAES4      :1;                                       /* Port A Edge Select Bit 4 */
    byte PTAES5      :1;                                       /* Port A Edge Select Bit 5 */
    byte PTAES6      :1;                                       /* Port A Edge Select Bit 6 */
    byte PTAES7      :1;                                       /* Port A Edge Select Bit 7 */
  } Bits;
} PTAESSTR;
extern volatile PTAESSTR _PTAES @0x00001846;
#define PTAES                           _PTAES.Byte
#define PTAES_PTAES0                    _PTAES.Bits.PTAES0
#define PTAES_PTAES1                    _PTAES.Bits.PTAES1
#define PTAES_PTAES2                    _PTAES.Bits.PTAES2
#define PTAES_PTAES3                    _PTAES.Bits.PTAES3
#define PTAES_PTAES4                    _PTAES.Bits.PTAES4
#define PTAES_PTAES5                    _PTAES.Bits.PTAES5
#define PTAES_PTAES6                    _PTAES.Bits.PTAES6
#define PTAES_PTAES7                    _PTAES.Bits.PTAES7

#define PTAES_PTAES0_MASK               1
#define PTAES_PTAES1_MASK               2
#define PTAES_PTAES2_MASK               4
#define PTAES_PTAES3_MASK               8
#define PTAES_PTAES4_MASK               16
#define PTAES_PTAES5_MASK               32
#define PTAES_PTAES6_MASK               64
#define PTAES_PTAES7_MASK               128


/*** PTBPE - Port B Pull Enable Register; 0x00001848 ***/
typedef union {
  byte Byte;
  struct {
    byte PTBPE0      :1;                                       /* Internal Pull Enable for Port B Bit 0 */
    byte PTBPE1      :1;                                       /* Internal Pull Enable for Port B Bit 1 */
    byte PTBPE2      :1;                                       /* Internal Pull Enable for Port B Bit 2 */
    byte PTBPE3      :1;                                       /* Internal Pull Enable for Port B Bit 3 */
    byte PTBPE4      :1;                                       /* Internal Pull Enable for Port B Bit 4 */
    byte PTBPE5      :1;                                       /* Internal Pull Enable for Port B Bit 5 */
    byte PTBPE6      :1;                                       /* Internal Pull Enable for Port B Bit 6 */
    byte PTBPE7      :1;                                       /* Internal Pull Enable for Port B Bit 7 */
  } Bits;
} PTBPESTR;
extern volatile PTBPESTR _PTBPE @0x00001848;
#define PTBPE                           _PTBPE.Byte
#define PTBPE_PTBPE0                    _PTBPE.Bits.PTBPE0
#define PTBPE_PTBPE1                    _PTBPE.Bits.PTBPE1
#define PTBPE_PTBPE2                    _PTBPE.Bits.PTBPE2
#define PTBPE_PTBPE3                    _PTBPE.Bits.PTBPE3
#define PTBPE_PTBPE4                    _PTBPE.Bits.PTBPE4
#define PTBPE_PTBPE5                    _PTBPE.Bits.PTBPE5
#define PTBPE_PTBPE6                    _PTBPE.Bits.PTBPE6
#define PTBPE_PTBPE7                    _PTBPE.Bits.PTBPE7

#define PTBPE_PTBPE0_MASK               1
#define PTBPE_PTBPE1_MASK               2
#define PTBPE_PTBPE2_MASK               4
#define PTBPE_PTBPE3_MASK               8
#define PTBPE_PTBPE4_MASK               16
#define PTBPE_PTBPE5_MASK               32
#define PTBPE_PTBPE6_MASK               64
#define PTBPE_PTBPE7_MASK               128


/*** PTBSE - Port B Slew Rate Enable Register; 0x00001849 ***/
typedef union {
  byte Byte;
  struct {
    byte PTBSE0      :1;                                       /* Output Slew Rate Enable for Port B Bit 0 */
    byte PTBSE1      :1;                                       /* Output Slew Rate Enable for Port B Bit 1 */
    byte PTBSE2      :1;                                       /* Output Slew Rate Enable for Port B Bit 2 */
    byte PTBSE3      :1;                                       /* Output Slew Rate Enable for Port B Bit 3 */
    byte PTBSE4      :1;                                       /* Output Slew Rate Enable for Port B Bit 4 */
    byte PTBSE5      :1;                                       /* Output Slew Rate Enable for Port B Bit 5 */
    byte PTBSE6      :1;                                       /* Output Slew Rate Enable for Port B Bit 6 */
    byte PTBSE7      :1;                                       /* Output Slew Rate Enable for Port B Bit 7 */
  } Bits;
} PTBSESTR;
extern volatile PTBSESTR _PTBSE @0x00001849;
#define PTBSE                           _PTBSE.Byte
#define PTBSE_PTBSE0                    _PTBSE.Bits.PTBSE0
#define PTBSE_PTBSE1                    _PTBSE.Bits.PTBSE1
#define PTBSE_PTBSE2                    _PTBSE.Bits.PTBSE2
#define PTBSE_PTBSE3                    _PTBSE.Bits.PTBSE3
#define PTBSE_PTBSE4                    _PTBSE.Bits.PTBSE4
#define PTBSE_PTBSE5                    _PTBSE.Bits.PTBSE5
#define PTBSE_PTBSE6                    _PTBSE.Bits.PTBSE6
#define PTBSE_PTBSE7                    _PTBSE.Bits.PTBSE7

#define PTBSE_PTBSE0_MASK               1
#define PTBSE_PTBSE1_MASK               2
#define PTBSE_PTBSE2_MASK               4
#define PTBSE_PTBSE3_MASK               8
#define PTBSE_PTBSE4_MASK               16
#define PTBSE_PTBSE5_MASK               32
#define PTBSE_PTBSE6_MASK               64
#define PTBSE_PTBSE7_MASK               128


/*** PTBDS - Port B Drive Strength Selection Register; 0x0000184A ***/
typedef union {
  byte Byte;
  struct {
    byte PTBDS0      :1;                                       /* Output Drive Strength Selection for Port B Bit 0 */
    byte PTBDS1      :1;                                       /* Output Drive Strength Selection for Port B Bit 1 */
    byte PTBDS2      :1;                                       /* Output Drive Strength Selection for Port B Bit 2 */
    byte PTBDS3      :1;                                       /* Output Drive Strength Selection for Port B Bit 3 */
    byte PTBDS4      :1;                                       /* Output Drive Strength Selection for Port B Bit 4 */
    byte PTBDS5      :1;                                       /* Output Drive Strength Selection for Port B Bit 5 */
    byte PTBDS6      :1;                                       /* Output Drive Strength Selection for Port B Bit 6 */
    byte PTBDS7      :1;                                       /* Output Drive Strength Selection for Port B Bit 7 */
  } Bits;
} PTBDSSTR;
extern volatile PTBDSSTR _PTBDS @0x0000184A;
#define PTBDS                           _PTBDS.Byte
#define PTBDS_PTBDS0                    _PTBDS.Bits.PTBDS0
#define PTBDS_PTBDS1                    _PTBDS.Bits.PTBDS1
#define PTBDS_PTBDS2                    _PTBDS.Bits.PTBDS2
#define PTBDS_PTBDS3                    _PTBDS.Bits.PTBDS3
#define PTBDS_PTBDS4                    _PTBDS.Bits.PTBDS4
#define PTBDS_PTBDS5                    _PTBDS.Bits.PTBDS5
#define PTBDS_PTBDS6                    _PTBDS.Bits.PTBDS6
#define PTBDS_PTBDS7                    _PTBDS.Bits.PTBDS7

#define PTBDS_PTBDS0_MASK               1
#define PTBDS_PTBDS1_MASK               2
#define PTBDS_PTBDS2_MASK               4
#define PTBDS_PTBDS3_MASK               8
#define PTBDS_PTBDS4_MASK               16
#define PTBDS_PTBDS5_MASK               32
#define PTBDS_PTBDS6_MASK               64
#define PTBDS_PTBDS7_MASK               128


/*** PTBSC - Port B Interrupt Status and Control Register; 0x0000184C ***/
typedef union {
  byte Byte;
  struct {
    byte PTBMOD      :1;                                       /* Port B Detection Mode */
    byte PTBIE       :1;                                       /* Port B Interrupt Enable */
    byte PTBACK      :1;                                       /* Port B Interrupt Acknowledge */
    byte PTBIF       :1;                                       /* Port B Interrupt Flag */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
  } Bits;
} PTBSCSTR;
extern volatile PTBSCSTR _PTBSC @0x0000184C;
#define PTBSC                           _PTBSC.Byte
#define PTBSC_PTBMOD                    _PTBSC.Bits.PTBMOD
#define PTBSC_PTBIE                     _PTBSC.Bits.PTBIE
#define PTBSC_PTBACK                    _PTBSC.Bits.PTBACK
#define PTBSC_PTBIF                     _PTBSC.Bits.PTBIF

#define PTBSC_PTBMOD_MASK               1
#define PTBSC_PTBIE_MASK                2
#define PTBSC_PTBACK_MASK               4
#define PTBSC_PTBIF_MASK                8


/*** PTBPS - Port B Interrupt Pin Select Register; 0x0000184D ***/
typedef union {
  byte Byte;
  struct {
    byte PTBPS0      :1;                                       /* Port B Interrupt Pin Select Bit 0 */
    byte PTBPS1      :1;                                       /* Port B Interrupt Pin Select Bit 1 */
    byte PTBPS2      :1;                                       /* Port B Interrupt Pin Select Bit 2 */
    byte PTBPS3      :1;                                       /* Port B Interrupt Pin Select Bit 3 */
    byte PTBPS4      :1;                                       /* Port B Interrupt Pin Select Bit 4 */
    byte PTBPS5      :1;                                       /* Port B Interrupt Pin Select Bit 5 */
    byte PTBPS6      :1;                                       /* Port B Interrupt Pin Select Bit 6 */
    byte PTBPS7      :1;                                       /* Port B Interrupt Pin Select Bit 7 */
  } Bits;
} PTBPSSTR;
extern volatile PTBPSSTR _PTBPS @0x0000184D;
#define PTBPS                           _PTBPS.Byte
#define PTBPS_PTBPS0                    _PTBPS.Bits.PTBPS0
#define PTBPS_PTBPS1                    _PTBPS.Bits.PTBPS1
#define PTBPS_PTBPS2                    _PTBPS.Bits.PTBPS2
#define PTBPS_PTBPS3                    _PTBPS.Bits.PTBPS3
#define PTBPS_PTBPS4                    _PTBPS.Bits.PTBPS4
#define PTBPS_PTBPS5                    _PTBPS.Bits.PTBPS5
#define PTBPS_PTBPS6                    _PTBPS.Bits.PTBPS6
#define PTBPS_PTBPS7                    _PTBPS.Bits.PTBPS7

#define PTBPS_PTBPS0_MASK               1
#define PTBPS_PTBPS1_MASK               2
#define PTBPS_PTBPS2_MASK               4
#define PTBPS_PTBPS3_MASK               8
#define PTBPS_PTBPS4_MASK               16
#define PTBPS_PTBPS5_MASK               32
#define PTBPS_PTBPS6_MASK               64
#define PTBPS_PTBPS7_MASK               128


/*** PTBES - Port B Interrupt Edge Select Register; 0x0000184E ***/
typedef union {
  byte Byte;
  struct {
    byte PTBES0      :1;                                       /* Port B Edge Select Bit 0 */
    byte PTBES1      :1;                                       /* Port B Edge Select Bit 1 */
    byte PTBES2      :1;                                       /* Port B Edge Select Bit 2 */
    byte PTBES3      :1;                                       /* Port B Edge Select Bit 3 */
    byte PTBES4      :1;                                       /* Port B Edge Select Bit 4 */
    byte PTBES5      :1;                                       /* Port B Edge Select Bit 5 */
    byte PTBES6      :1;                                       /* Port B Edge Select Bit 6 */
    byte PTBES7      :1;                                       /* Port B Edge Select Bit 7 */
  } Bits;
} PTBESSTR;
extern volatile PTBESSTR _PTBES @0x0000184E;
#define PTBES                           _PTBES.Byte
#define PTBES_PTBES0                    _PTBES.Bits.PTBES0
#define PTBES_PTBES1                    _PTBES.Bits.PTBES1
#define PTBES_PTBES2                    _PTBES.Bits.PTBES2
#define PTBES_PTBES3                    _PTBES.Bits.PTBES3
#define PTBES_PTBES4                    _PTBES.Bits.PTBES4
#define PTBES_PTBES5                    _PTBES.Bits.PTBES5
#define PTBES_PTBES6                    _PTBES.Bits.PTBES6
#define PTBES_PTBES7                    _PTBES.Bits.PTBES7

#define PTBES_PTBES0_MASK               1
#define PTBES_PTBES1_MASK               2
#define PTBES_PTBES2_MASK               4
#define PTBES_PTBES3_MASK               8
#define PTBES_PTBES4_MASK               16
#define PTBES_PTBES5_MASK               32
#define PTBES_PTBES6_MASK               64
#define PTBES_PTBES7_MASK               128


/*** PTCPE - Port C Pull Enable Register; 0x00001850 ***/
typedef union {
  byte Byte;
  struct {
    byte PTCPE0      :1;                                       /* Internal Pull Enable for Port C Bit 0 */
    byte PTCPE1      :1;                                       /* Internal Pull Enable for Port C Bit 1 */
    byte PTCPE2      :1;                                       /* Internal Pull Enable for Port C Bit 2 */
    byte PTCPE3      :1;                                       /* Internal Pull Enable for Port C Bit 3 */
    byte PTCPE4      :1;                                       /* Internal Pull Enable for Port C Bit 4 */
    byte PTCPE5      :1;                                       /* Internal Pull Enable for Port C Bit 5 */
    byte PTCPE6      :1;                                       /* Internal Pull Enable for Port C Bit 6 */
    byte PTCPE7      :1;                                       /* Internal Pull Enable for Port C Bit 7 */
  } Bits;
} PTCPESTR;
extern volatile PTCPESTR _PTCPE @0x00001850;
#define PTCPE                           _PTCPE.Byte
#define PTCPE_PTCPE0                    _PTCPE.Bits.PTCPE0
#define PTCPE_PTCPE1                    _PTCPE.Bits.PTCPE1
#define PTCPE_PTCPE2                    _PTCPE.Bits.PTCPE2
#define PTCPE_PTCPE3                    _PTCPE.Bits.PTCPE3
#define PTCPE_PTCPE4                    _PTCPE.Bits.PTCPE4
#define PTCPE_PTCPE5                    _PTCPE.Bits.PTCPE5
#define PTCPE_PTCPE6                    _PTCPE.Bits.PTCPE6
#define PTCPE_PTCPE7                    _PTCPE.Bits.PTCPE7

#define PTCPE_PTCPE0_MASK               1
#define PTCPE_PTCPE1_MASK               2
#define PTCPE_PTCPE2_MASK               4
#define PTCPE_PTCPE3_MASK               8
#define PTCPE_PTCPE4_MASK               16
#define PTCPE_PTCPE5_MASK               32
#define PTCPE_PTCPE6_MASK               64
#define PTCPE_PTCPE7_MASK               128


/*** PTCSE - Port C Slew Rate Enable Register; 0x00001851 ***/
typedef union {
  byte Byte;
  struct {
    byte PTCSE0      :1;                                       /* Output Slew Rate Enable for Port C Bit 0 */
    byte PTCSE1      :1;                                       /* Output Slew Rate Enable for Port C Bit 1 */
    byte PTCSE2      :1;                                       /* Output Slew Rate Enable for Port C Bit 2 */
    byte PTCSE3      :1;                                       /* Output Slew Rate Enable for Port C Bit 3 */
    byte PTCSE4      :1;                                       /* Output Slew Rate Enable for Port C Bit 4 */
    byte PTCSE5      :1;                                       /* Output Slew Rate Enable for Port C Bit 5 */
    byte PTCSE6      :1;                                       /* Output Slew Rate Enable for Port C Bit 6 */
    byte PTCSE7      :1;                                       /* Output Slew Rate Enable for Port C Bit 7 */
  } Bits;
} PTCSESTR;
extern volatile PTCSESTR _PTCSE @0x00001851;
#define PTCSE                           _PTCSE.Byte
#define PTCSE_PTCSE0                    _PTCSE.Bits.PTCSE0
#define PTCSE_PTCSE1                    _PTCSE.Bits.PTCSE1
#define PTCSE_PTCSE2                    _PTCSE.Bits.PTCSE2
#define PTCSE_PTCSE3                    _PTCSE.Bits.PTCSE3
#define PTCSE_PTCSE4                    _PTCSE.Bits.PTCSE4
#define PTCSE_PTCSE5                    _PTCSE.Bits.PTCSE5
#define PTCSE_PTCSE6                    _PTCSE.Bits.PTCSE6
#define PTCSE_PTCSE7                    _PTCSE.Bits.PTCSE7

#define PTCSE_PTCSE0_MASK               1
#define PTCSE_PTCSE1_MASK               2
#define PTCSE_PTCSE2_MASK               4
#define PTCSE_PTCSE3_MASK               8
#define PTCSE_PTCSE4_MASK               16
#define PTCSE_PTCSE5_MASK               32
#define PTCSE_PTCSE6_MASK               64
#define PTCSE_PTCSE7_MASK               128


/*** PTCDS - Port C Drive Strength Selection Register; 0x00001852 ***/
typedef union {
  byte Byte;
  struct {
    byte PTCDS0      :1;                                       /* Output Drive Strength Selection for Port C Bit 0 */
    byte PTCDS1      :1;                                       /* Output Drive Strength Selection for Port C Bit 1 */
    byte PTCDS2      :1;                                       /* Output Drive Strength Selection for Port C Bit 2 */
    byte PTCDS3      :1;                                       /* Output Drive Strength Selection for Port C Bit 3 */
    byte PTCDS4      :1;                                       /* Output Drive Strength Selection for Port C Bit 4 */
    byte PTCDS5      :1;                                       /* Output Drive Strength Selection for Port C Bit 5 */
    byte PTCDS6      :1;                                       /* Output Drive Strength Selection for Port C Bit 6 */
    byte PTCDS7      :1;                                       /* Output Drive Strength Selection for Port C Bit 7 */
  } Bits;
} PTCDSSTR;
extern volatile PTCDSSTR _PTCDS @0x00001852;
#define PTCDS                           _PTCDS.Byte
#define PTCDS_PTCDS0                    _PTCDS.Bits.PTCDS0
#define PTCDS_PTCDS1                    _PTCDS.Bits.PTCDS1
#define PTCDS_PTCDS2                    _PTCDS.Bits.PTCDS2
#define PTCDS_PTCDS3                    _PTCDS.Bits.PTCDS3
#define PTCDS_PTCDS4                    _PTCDS.Bits.PTCDS4
#define PTCDS_PTCDS5                    _PTCDS.Bits.PTCDS5
#define PTCDS_PTCDS6                    _PTCDS.Bits.PTCDS6
#define PTCDS_PTCDS7                    _PTCDS.Bits.PTCDS7

#define PTCDS_PTCDS0_MASK               1
#define PTCDS_PTCDS1_MASK               2
#define PTCDS_PTCDS2_MASK               4
#define PTCDS_PTCDS3_MASK               8
#define PTCDS_PTCDS4_MASK               16
#define PTCDS_PTCDS5_MASK               32
#define PTCDS_PTCDS6_MASK               64
#define PTCDS_PTCDS7_MASK               128


/*** PTDPE - Port D Pull Enable Register; 0x00001858 ***/
typedef union {
  byte Byte;
  struct {
    byte PTDPE0      :1;                                       /* Internal Pull Enable for Port D Bit 0 */
    byte PTDPE1      :1;                                       /* Internal Pull Enable for Port D Bit 1 */
    byte PTDPE2      :1;                                       /* Internal Pull Enable for Port D Bit 2 */
    byte PTDPE3      :1;                                       /* Internal Pull Enable for Port D Bit 3 */
    byte PTDPE4      :1;                                       /* Internal Pull Enable for Port D Bit 4 */
    byte PTDPE5      :1;                                       /* Internal Pull Enable for Port D Bit 5 */
    byte PTDPE6      :1;                                       /* Internal Pull Enable for Port D Bit 6 */
    byte PTDPE7      :1;                                       /* Internal Pull Enable for Port D Bit 7 */
  } Bits;
} PTDPESTR;
extern volatile PTDPESTR _PTDPE @0x00001858;
#define PTDPE                           _PTDPE.Byte
#define PTDPE_PTDPE0                    _PTDPE.Bits.PTDPE0
#define PTDPE_PTDPE1                    _PTDPE.Bits.PTDPE1
#define PTDPE_PTDPE2                    _PTDPE.Bits.PTDPE2
#define PTDPE_PTDPE3                    _PTDPE.Bits.PTDPE3
#define PTDPE_PTDPE4                    _PTDPE.Bits.PTDPE4
#define PTDPE_PTDPE5                    _PTDPE.Bits.PTDPE5
#define PTDPE_PTDPE6                    _PTDPE.Bits.PTDPE6
#define PTDPE_PTDPE7                    _PTDPE.Bits.PTDPE7

#define PTDPE_PTDPE0_MASK               1
#define PTDPE_PTDPE1_MASK               2
#define PTDPE_PTDPE2_MASK               4
#define PTDPE_PTDPE3_MASK               8
#define PTDPE_PTDPE4_MASK               16
#define PTDPE_PTDPE5_MASK               32
#define PTDPE_PTDPE6_MASK               64
#define PTDPE_PTDPE7_MASK               128


/*** PTDSE - Port D Slew Rate Enable Register; 0x00001859 ***/
typedef union {
  byte Byte;
  struct {
    byte PTDSE0      :1;                                       /* Output Slew Rate Enable for Port D Bit 0 */
    byte PTDSE1      :1;                                       /* Output Slew Rate Enable for Port D Bit 1 */
    byte PTDSE2      :1;                                       /* Output Slew Rate Enable for Port D Bit 2 */
    byte PTDSE3      :1;                                       /* Output Slew Rate Enable for Port D Bit 3 */
    byte PTDSE4      :1;                                       /* Output Slew Rate Enable for Port D Bit 4 */
    byte PTDSE5      :1;                                       /* Output Slew Rate Enable for Port D Bit 5 */
    byte PTDSE6      :1;                                       /* Output Slew Rate Enable for Port D Bit 6 */
    byte PTDSE7      :1;                                       /* Output Slew Rate Enable for Port D Bit 7 */
  } Bits;
} PTDSESTR;
extern volatile PTDSESTR _PTDSE @0x00001859;
#define PTDSE                           _PTDSE.Byte
#define PTDSE_PTDSE0                    _PTDSE.Bits.PTDSE0
#define PTDSE_PTDSE1                    _PTDSE.Bits.PTDSE1
#define PTDSE_PTDSE2                    _PTDSE.Bits.PTDSE2
#define PTDSE_PTDSE3                    _PTDSE.Bits.PTDSE3
#define PTDSE_PTDSE4                    _PTDSE.Bits.PTDSE4
#define PTDSE_PTDSE5                    _PTDSE.Bits.PTDSE5
#define PTDSE_PTDSE6                    _PTDSE.Bits.PTDSE6
#define PTDSE_PTDSE7                    _PTDSE.Bits.PTDSE7

#define PTDSE_PTDSE0_MASK               1
#define PTDSE_PTDSE1_MASK               2
#define PTDSE_PTDSE2_MASK               4
#define PTDSE_PTDSE3_MASK               8
#define PTDSE_PTDSE4_MASK               16
#define PTDSE_PTDSE5_MASK               32
#define PTDSE_PTDSE6_MASK               64
#define PTDSE_PTDSE7_MASK               128


/*** PTDDS - Port D Drive Strength Selection Register; 0x0000185A ***/
typedef union {
  byte Byte;
  struct {
    byte PTDDS0      :1;                                       /* Output Drive Strength Selection for Port D Bit 0 */
    byte PTDDS1      :1;                                       /* Output Drive Strength Selection for Port D Bit 1 */
    byte PTDDS2      :1;                                       /* Output Drive Strength Selection for Port D Bit 2 */
    byte PTDDS3      :1;                                       /* Output Drive Strength Selection for Port D Bit 3 */
    byte PTDDS4      :1;                                       /* Output Drive Strength Selection for Port D Bit 4 */
    byte PTDDS5      :1;                                       /* Output Drive Strength Selection for Port D Bit 5 */
    byte PTDDS6      :1;                                       /* Output Drive Strength Selection for Port D Bit 6 */
    byte PTDDS7      :1;                                       /* Output Drive Strength Selection for Port D Bit 7 */
  } Bits;
} PTDDSSTR;
extern volatile PTDDSSTR _PTDDS @0x0000185A;
#define PTDDS                           _PTDDS.Byte
#define PTDDS_PTDDS0                    _PTDDS.Bits.PTDDS0
#define PTDDS_PTDDS1                    _PTDDS.Bits.PTDDS1
#define PTDDS_PTDDS2                    _PTDDS.Bits.PTDDS2
#define PTDDS_PTDDS3                    _PTDDS.Bits.PTDDS3
#define PTDDS_PTDDS4                    _PTDDS.Bits.PTDDS4
#define PTDDS_PTDDS5                    _PTDDS.Bits.PTDDS5
#define PTDDS_PTDDS6                    _PTDDS.Bits.PTDDS6
#define PTDDS_PTDDS7                    _PTDDS.Bits.PTDDS7

#define PTDDS_PTDDS0_MASK               1
#define PTDDS_PTDDS1_MASK               2
#define PTDDS_PTDDS2_MASK               4
#define PTDDS_PTDDS3_MASK               8
#define PTDDS_PTDDS4_MASK               16
#define PTDDS_PTDDS5_MASK               32
#define PTDDS_PTDDS6_MASK               64
#define PTDDS_PTDDS7_MASK               128


/*** PTDSC - Port D Interrupt Status and Control Register; 0x0000185C ***/
typedef union {
  byte Byte;
  struct {
    byte PTDMOD      :1;                                       /* Port D Detection Mode */
    byte PTDIE       :1;                                       /* Port D Interrupt Enable */
    byte PTDACK      :1;                                       /* Port D Interrupt Acknowledge */
    byte PTDIF       :1;                                       /* Port D Interrupt Flag */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
  } Bits;
} PTDSCSTR;
extern volatile PTDSCSTR _PTDSC @0x0000185C;
#define PTDSC                           _PTDSC.Byte
#define PTDSC_PTDMOD                    _PTDSC.Bits.PTDMOD
#define PTDSC_PTDIE                     _PTDSC.Bits.PTDIE
#define PTDSC_PTDACK                    _PTDSC.Bits.PTDACK
#define PTDSC_PTDIF                     _PTDSC.Bits.PTDIF

#define PTDSC_PTDMOD_MASK               1
#define PTDSC_PTDIE_MASK                2
#define PTDSC_PTDACK_MASK               4
#define PTDSC_PTDIF_MASK                8


/*** PTDPS - Port D Interrupt Pin Select Register; 0x0000185D ***/
typedef union {
  byte Byte;
  struct {
    byte PTDPS0      :1;                                       /* Port D Interrupt Pin Select Bit 0 */
    byte PTDPS1      :1;                                       /* Port D Interrupt Pin Select Bit 1 */
    byte PTDPS2      :1;                                       /* Port D Interrupt Pin Select Bit 2 */
    byte PTDPS3      :1;                                       /* Port D Interrupt Pin Select Bit 3 */
    byte PTDPS4      :1;                                       /* Port D Interrupt Pin Select Bit 4 */
    byte PTDPS5      :1;                                       /* Port D Interrupt Pin Select Bit 5 */
    byte PTDPS6      :1;                                       /* Port D Interrupt Pin Select Bit 6 */
    byte PTDPS7      :1;                                       /* Port D Interrupt Pin Select Bit 7 */
  } Bits;
} PTDPSSTR;
extern volatile PTDPSSTR _PTDPS @0x0000185D;
#define PTDPS                           _PTDPS.Byte
#define PTDPS_PTDPS0                    _PTDPS.Bits.PTDPS0
#define PTDPS_PTDPS1                    _PTDPS.Bits.PTDPS1
#define PTDPS_PTDPS2                    _PTDPS.Bits.PTDPS2
#define PTDPS_PTDPS3                    _PTDPS.Bits.PTDPS3
#define PTDPS_PTDPS4                    _PTDPS.Bits.PTDPS4
#define PTDPS_PTDPS5                    _PTDPS.Bits.PTDPS5
#define PTDPS_PTDPS6                    _PTDPS.Bits.PTDPS6
#define PTDPS_PTDPS7                    _PTDPS.Bits.PTDPS7

#define PTDPS_PTDPS0_MASK               1
#define PTDPS_PTDPS1_MASK               2
#define PTDPS_PTDPS2_MASK               4
#define PTDPS_PTDPS3_MASK               8
#define PTDPS_PTDPS4_MASK               16
#define PTDPS_PTDPS5_MASK               32
#define PTDPS_PTDPS6_MASK               64
#define PTDPS_PTDPS7_MASK               128


/*** PTDES - Port D Interrupt Edge Select Register; 0x0000185E ***/
typedef union {
  byte Byte;
  struct {
    byte PTDES0      :1;                                       /* Port D Edge Select Bit 0 */
    byte PTDES1      :1;                                       /* Port D Edge Select Bit 1 */
    byte PTDES2      :1;                                       /* Port D Edge Select Bit 2 */
    byte PTDES3      :1;                                       /* Port D Edge Select Bit 3 */
    byte PTDES4      :1;                                       /* Port D Edge Select Bit 4 */
    byte PTDES5      :1;                                       /* Port D Edge Select Bit 5 */
    byte PTDES6      :1;                                       /* Port D Edge Select Bit 6 */
    byte PTDES7      :1;                                       /* Port D Edge Select Bit 7 */
  } Bits;
} PTDESSTR;
extern volatile PTDESSTR _PTDES @0x0000185E;
#define PTDES                           _PTDES.Byte
#define PTDES_PTDES0                    _PTDES.Bits.PTDES0
#define PTDES_PTDES1                    _PTDES.Bits.PTDES1
#define PTDES_PTDES2                    _PTDES.Bits.PTDES2
#define PTDES_PTDES3                    _PTDES.Bits.PTDES3
#define PTDES_PTDES4                    _PTDES.Bits.PTDES4
#define PTDES_PTDES5                    _PTDES.Bits.PTDES5
#define PTDES_PTDES6                    _PTDES.Bits.PTDES6
#define PTDES_PTDES7                    _PTDES.Bits.PTDES7

#define PTDES_PTDES0_MASK               1
#define PTDES_PTDES1_MASK               2
#define PTDES_PTDES2_MASK               4
#define PTDES_PTDES3_MASK               8
#define PTDES_PTDES4_MASK               16
#define PTDES_PTDES5_MASK               32
#define PTDES_PTDES6_MASK               64
#define PTDES_PTDES7_MASK               128


/*** PTEPE - Port E Pull Enable Register; 0x00001860 ***/
typedef union {
  byte Byte;
  struct {
    byte PTEPE0      :1;                                       /* Internal Pull Enable for Port E Bit 0 */
    byte PTEPE1      :1;                                       /* Internal Pull Enable for Port E Bit 1 */
    byte PTEPE2      :1;                                       /* Internal Pull Enable for Port E Bit 2 */
    byte PTEPE3      :1;                                       /* Internal Pull Enable for Port E Bit 3 */
    byte PTEPE4      :1;                                       /* Internal Pull Enable for Port E Bit 4 */
    byte PTEPE5      :1;                                       /* Internal Pull Enable for Port E Bit 5 */
    byte PTEPE6      :1;                                       /* Internal Pull Enable for Port E Bit 6 */
    byte PTEPE7      :1;                                       /* Internal Pull Enable for Port E Bit 7 */
  } Bits;
} PTEPESTR;
extern volatile PTEPESTR _PTEPE @0x00001860;
#define PTEPE                           _PTEPE.Byte
#define PTEPE_PTEPE0                    _PTEPE.Bits.PTEPE0
#define PTEPE_PTEPE1                    _PTEPE.Bits.PTEPE1
#define PTEPE_PTEPE2                    _PTEPE.Bits.PTEPE2
#define PTEPE_PTEPE3                    _PTEPE.Bits.PTEPE3
#define PTEPE_PTEPE4                    _PTEPE.Bits.PTEPE4
#define PTEPE_PTEPE5                    _PTEPE.Bits.PTEPE5
#define PTEPE_PTEPE6                    _PTEPE.Bits.PTEPE6
#define PTEPE_PTEPE7                    _PTEPE.Bits.PTEPE7

#define PTEPE_PTEPE0_MASK               1
#define PTEPE_PTEPE1_MASK               2
#define PTEPE_PTEPE2_MASK               4
#define PTEPE_PTEPE3_MASK               8
#define PTEPE_PTEPE4_MASK               16
#define PTEPE_PTEPE5_MASK               32
#define PTEPE_PTEPE6_MASK               64
#define PTEPE_PTEPE7_MASK               128


/*** PTESE - Port E Slew Rate Enable Register; 0x00001861 ***/
typedef union {
  byte Byte;
  struct {
    byte PTESE0      :1;                                       /* Output Slew Rate Enable for Port E Bit 0 */
    byte PTESE1      :1;                                       /* Output Slew Rate Enable for Port E Bit 1 */
    byte PTESE2      :1;                                       /* Output Slew Rate Enable for Port E Bit 2 */
    byte PTESE3      :1;                                       /* Output Slew Rate Enable for Port E Bit 3 */
    byte PTESE4      :1;                                       /* Output Slew Rate Enable for Port E Bit 4 */
    byte PTESE5      :1;                                       /* Output Slew Rate Enable for Port E Bit 5 */
    byte PTESE6      :1;                                       /* Output Slew Rate Enable for Port E Bit 6 */
    byte PTESE7      :1;                                       /* Output Slew Rate Enable for Port E Bit 7 */
  } Bits;
} PTESESTR;
extern volatile PTESESTR _PTESE @0x00001861;
#define PTESE                           _PTESE.Byte
#define PTESE_PTESE0                    _PTESE.Bits.PTESE0
#define PTESE_PTESE1                    _PTESE.Bits.PTESE1
#define PTESE_PTESE2                    _PTESE.Bits.PTESE2
#define PTESE_PTESE3                    _PTESE.Bits.PTESE3
#define PTESE_PTESE4                    _PTESE.Bits.PTESE4
#define PTESE_PTESE5                    _PTESE.Bits.PTESE5
#define PTESE_PTESE6                    _PTESE.Bits.PTESE6
#define PTESE_PTESE7                    _PTESE.Bits.PTESE7

#define PTESE_PTESE0_MASK               1
#define PTESE_PTESE1_MASK               2
#define PTESE_PTESE2_MASK               4
#define PTESE_PTESE3_MASK               8
#define PTESE_PTESE4_MASK               16
#define PTESE_PTESE5_MASK               32
#define PTESE_PTESE6_MASK               64
#define PTESE_PTESE7_MASK               128


/*** PTEDS - Port E Drive Strength Selection Register; 0x00001862 ***/
typedef union {
  byte Byte;
  struct {
    byte PTEDS0      :1;                                       /* Output Drive Strength Selection for Port E Bit 0 */
    byte PTEDS1      :1;                                       /* Output Drive Strength Selection for Port E Bit 1 */
    byte PTEDS2      :1;                                       /* Output Drive Strength Selection for Port E Bit 2 */
    byte PTEDS3      :1;                                       /* Output Drive Strength Selection for Port E Bit 3 */
    byte PTEDS4      :1;                                       /* Output Drive Strength Selection for Port E Bit 4 */
    byte PTEDS5      :1;                                       /* Output Drive Strength Selection for Port E Bit 5 */
    byte PTEDS6      :1;                                       /* Output Drive Strength Selection for Port E Bit 6 */
    byte PTEDS7      :1;                                       /* Output Drive Strength Selection for Port E Bit 7 */
  } Bits;
} PTEDSSTR;
extern volatile PTEDSSTR _PTEDS @0x00001862;
#define PTEDS                           _PTEDS.Byte
#define PTEDS_PTEDS0                    _PTEDS.Bits.PTEDS0
#define PTEDS_PTEDS1                    _PTEDS.Bits.PTEDS1
#define PTEDS_PTEDS2                    _PTEDS.Bits.PTEDS2
#define PTEDS_PTEDS3                    _PTEDS.Bits.PTEDS3
#define PTEDS_PTEDS4                    _PTEDS.Bits.PTEDS4
#define PTEDS_PTEDS5                    _PTEDS.Bits.PTEDS5
#define PTEDS_PTEDS6                    _PTEDS.Bits.PTEDS6
#define PTEDS_PTEDS7                    _PTEDS.Bits.PTEDS7

#define PTEDS_PTEDS0_MASK               1
#define PTEDS_PTEDS1_MASK               2
#define PTEDS_PTEDS2_MASK               4
#define PTEDS_PTEDS3_MASK               8
#define PTEDS_PTEDS4_MASK               16
#define PTEDS_PTEDS5_MASK               32
#define PTEDS_PTEDS6_MASK               64
#define PTEDS_PTEDS7_MASK               128


/*** PTFPE - Port F Pull Enable Register; 0x00001868 ***/
typedef union {
  byte Byte;
  struct {
    byte PTFPE0      :1;                                       /* Internal Pull Enable for Port F Bit 0 */
    byte PTFPE1      :1;                                       /* Internal Pull Enable for Port F Bit 1 */
    byte PTFPE2      :1;                                       /* Internal Pull Enable for Port F Bit 2 */
    byte PTFPE3      :1;                                       /* Internal Pull Enable for Port F Bit 3 */
    byte PTFPE4      :1;                                       /* Internal Pull Enable for Port F Bit 4 */
    byte PTFPE5      :1;                                       /* Internal Pull Enable for Port F Bit 5 */
    byte PTFPE6      :1;                                       /* Internal Pull Enable for Port F Bit 6 */
    byte PTFPE7      :1;                                       /* Internal Pull Enable for Port F Bit 7 */
  } Bits;
} PTFPESTR;
extern volatile PTFPESTR _PTFPE @0x00001868;
#define PTFPE                           _PTFPE.Byte
#define PTFPE_PTFPE0                    _PTFPE.Bits.PTFPE0
#define PTFPE_PTFPE1                    _PTFPE.Bits.PTFPE1
#define PTFPE_PTFPE2                    _PTFPE.Bits.PTFPE2
#define PTFPE_PTFPE3                    _PTFPE.Bits.PTFPE3
#define PTFPE_PTFPE4                    _PTFPE.Bits.PTFPE4
#define PTFPE_PTFPE5                    _PTFPE.Bits.PTFPE5
#define PTFPE_PTFPE6                    _PTFPE.Bits.PTFPE6
#define PTFPE_PTFPE7                    _PTFPE.Bits.PTFPE7

#define PTFPE_PTFPE0_MASK               1
#define PTFPE_PTFPE1_MASK               2
#define PTFPE_PTFPE2_MASK               4
#define PTFPE_PTFPE3_MASK               8
#define PTFPE_PTFPE4_MASK               16
#define PTFPE_PTFPE5_MASK               32
#define PTFPE_PTFPE6_MASK               64
#define PTFPE_PTFPE7_MASK               128


/*** PTFSE - Port F Slew Rate Enable Register; 0x00001869 ***/
typedef union {
  byte Byte;
  struct {
    byte PTFSE0      :1;                                       /* Output Slew Rate Enable for Port F Bit 0 */
    byte PTFSE1      :1;                                       /* Output Slew Rate Enable for Port F Bit 1 */
    byte PTFSE2      :1;                                       /* Output Slew Rate Enable for Port F Bit 2 */
    byte PTFSE3      :1;                                       /* Output Slew Rate Enable for Port F Bit 3 */
    byte PTFSE4      :1;                                       /* Output Slew Rate Enable for Port F Bit 4 */
    byte PTFSE5      :1;                                       /* Output Slew Rate Enable for Port F Bit 5 */
    byte PTFSE6      :1;                                       /* Output Slew Rate Enable for Port F Bit 6 */
    byte PTFSE7      :1;                                       /* Output Slew Rate Enable for Port F Bit 7 */
  } Bits;
} PTFSESTR;
extern volatile PTFSESTR _PTFSE @0x00001869;
#define PTFSE                           _PTFSE.Byte
#define PTFSE_PTFSE0                    _PTFSE.Bits.PTFSE0
#define PTFSE_PTFSE1                    _PTFSE.Bits.PTFSE1
#define PTFSE_PTFSE2                    _PTFSE.Bits.PTFSE2
#define PTFSE_PTFSE3                    _PTFSE.Bits.PTFSE3
#define PTFSE_PTFSE4                    _PTFSE.Bits.PTFSE4
#define PTFSE_PTFSE5                    _PTFSE.Bits.PTFSE5
#define PTFSE_PTFSE6                    _PTFSE.Bits.PTFSE6
#define PTFSE_PTFSE7                    _PTFSE.Bits.PTFSE7

#define PTFSE_PTFSE0_MASK               1
#define PTFSE_PTFSE1_MASK               2
#define PTFSE_PTFSE2_MASK               4
#define PTFSE_PTFSE3_MASK               8
#define PTFSE_PTFSE4_MASK               16
#define PTFSE_PTFSE5_MASK               32
#define PTFSE_PTFSE6_MASK               64
#define PTFSE_PTFSE7_MASK               128


/*** PTFDS - Port F Drive Strength Selection Register; 0x0000186A ***/
typedef union {
  byte Byte;
  struct {
    byte PTFDS0      :1;                                       /* Output Drive Strength Selection for Port F Bit 0 */
    byte PTFDS1      :1;                                       /* Output Drive Strength Selection for Port F Bit 1 */
    byte PTFDS2      :1;                                       /* Output Drive Strength Selection for Port F Bit 2 */
    byte PTFDS3      :1;                                       /* Output Drive Strength Selection for Port F Bit 3 */
    byte PTFDS4      :1;                                       /* Output Drive Strength Selection for Port F Bit 4 */
    byte PTFDS5      :1;                                       /* Output Drive Strength Selection for Port F Bit 5 */
    byte PTFDS6      :1;                                       /* Output Drive Strength Selection for Port F Bit 6 */
    byte PTFDS7      :1;                                       /* Output Drive Strength Selection for Port F Bit 7 */
  } Bits;
} PTFDSSTR;
extern volatile PTFDSSTR _PTFDS @0x0000186A;
#define PTFDS                           _PTFDS.Byte
#define PTFDS_PTFDS0                    _PTFDS.Bits.PTFDS0
#define PTFDS_PTFDS1                    _PTFDS.Bits.PTFDS1
#define PTFDS_PTFDS2                    _PTFDS.Bits.PTFDS2
#define PTFDS_PTFDS3                    _PTFDS.Bits.PTFDS3
#define PTFDS_PTFDS4                    _PTFDS.Bits.PTFDS4
#define PTFDS_PTFDS5                    _PTFDS.Bits.PTFDS5
#define PTFDS_PTFDS6                    _PTFDS.Bits.PTFDS6
#define PTFDS_PTFDS7                    _PTFDS.Bits.PTFDS7

#define PTFDS_PTFDS0_MASK               1
#define PTFDS_PTFDS1_MASK               2
#define PTFDS_PTFDS2_MASK               4
#define PTFDS_PTFDS3_MASK               8
#define PTFDS_PTFDS4_MASK               16
#define PTFDS_PTFDS5_MASK               32
#define PTFDS_PTFDS6_MASK               64
#define PTFDS_PTFDS7_MASK               128


/*** PTGPE - Port G Pull Enable Register; 0x00001870 ***/
typedef union {
  byte Byte;
  struct {
    byte PTGPE0      :1;                                       /* Internal Pull Enable for Port G Bit 0 */
    byte PTGPE1      :1;                                       /* Internal Pull Enable for Port G Bit 1 */
    byte PTGPE2      :1;                                       /* Internal Pull Enable for Port G Bit 2 */
    byte PTGPE3      :1;                                       /* Internal Pull Enable for Port G Bit 3 */
    byte PTGPE4      :1;                                       /* Internal Pull Enable for Port G Bit 4 */
    byte PTGPE5      :1;                                       /* Internal Pull Enable for Port G Bit 5 */
    byte             :1; 
    byte             :1; 
  } Bits;
  struct {
    byte grpPTGPE :6;
    byte         :1;
    byte         :1;
  } MergedBits;
} PTGPESTR;
extern volatile PTGPESTR _PTGPE @0x00001870;
#define PTGPE                           _PTGPE.Byte
#define PTGPE_PTGPE0                    _PTGPE.Bits.PTGPE0
#define PTGPE_PTGPE1                    _PTGPE.Bits.PTGPE1
#define PTGPE_PTGPE2                    _PTGPE.Bits.PTGPE2
#define PTGPE_PTGPE3                    _PTGPE.Bits.PTGPE3
#define PTGPE_PTGPE4                    _PTGPE.Bits.PTGPE4
#define PTGPE_PTGPE5                    _PTGPE.Bits.PTGPE5
#define PTGPE_PTGPE                     _PTGPE.MergedBits.grpPTGPE

#define PTGPE_PTGPE0_MASK               1
#define PTGPE_PTGPE1_MASK               2
#define PTGPE_PTGPE2_MASK               4
#define PTGPE_PTGPE3_MASK               8
#define PTGPE_PTGPE4_MASK               16
#define PTGPE_PTGPE5_MASK               32
#define PTGPE_PTGPE_MASK                63
#define PTGPE_PTGPE_BITNUM              0


/*** PTGSE - Port G Slew Rate Enable Register; 0x00001871 ***/
typedef union {
  byte Byte;
  struct {
    byte PTGSE0      :1;                                       /* Output Slew Rate Enable for Port G Bit 0 */
    byte PTGSE1      :1;                                       /* Output Slew Rate Enable for Port G Bit 1 */
    byte PTGSE2      :1;                                       /* Output Slew Rate Enable for Port G Bit 2 */
    byte PTGSE3      :1;                                       /* Output Slew Rate Enable for Port G Bit 3 */
    byte PTGSE4      :1;                                       /* Output Slew Rate Enable for Port G Bit 4 */
    byte PTGSE5      :1;                                       /* Output Slew Rate Enable for Port G Bit 5 */
    byte             :1; 
    byte             :1; 
  } Bits;
  struct {
    byte grpPTGSE :6;
    byte         :1;
    byte         :1;
  } MergedBits;
} PTGSESTR;
extern volatile PTGSESTR _PTGSE @0x00001871;
#define PTGSE                           _PTGSE.Byte
#define PTGSE_PTGSE0                    _PTGSE.Bits.PTGSE0
#define PTGSE_PTGSE1                    _PTGSE.Bits.PTGSE1
#define PTGSE_PTGSE2                    _PTGSE.Bits.PTGSE2
#define PTGSE_PTGSE3                    _PTGSE.Bits.PTGSE3
#define PTGSE_PTGSE4                    _PTGSE.Bits.PTGSE4
#define PTGSE_PTGSE5                    _PTGSE.Bits.PTGSE5
#define PTGSE_PTGSE                     _PTGSE.MergedBits.grpPTGSE

#define PTGSE_PTGSE0_MASK               1
#define PTGSE_PTGSE1_MASK               2
#define PTGSE_PTGSE2_MASK               4
#define PTGSE_PTGSE3_MASK               8
#define PTGSE_PTGSE4_MASK               16
#define PTGSE_PTGSE5_MASK               32
#define PTGSE_PTGSE_MASK                63
#define PTGSE_PTGSE_BITNUM              0


/*** PTGDS - Port G Drive Strength Selection Register; 0x00001872 ***/
typedef union {
  byte Byte;
  struct {
    byte PTGDS0      :1;                                       /* Output Drive Strength Selection for Port G Bit 0 */
    byte PTGDS1      :1;                                       /* Output Drive Strength Selection for Port G Bit 1 */
    byte PTGDS2      :1;                                       /* Output Drive Strength Selection for Port G Bit 2 */
    byte PTGDS3      :1;                                       /* Output Drive Strength Selection for Port G Bit 3 */
    byte PTGDS4      :1;                                       /* Output Drive Strength Selection for Port G Bit 4 */
    byte PTGDS5      :1;                                       /* Output Drive Strength Selection for Port G Bit 5 */
    byte             :1; 
    byte             :1; 
  } Bits;
  struct {
    byte grpPTGDS :6;
    byte         :1;
    byte         :1;
  } MergedBits;
} PTGDSSTR;
extern volatile PTGDSSTR _PTGDS @0x00001872;
#define PTGDS                           _PTGDS.Byte
#define PTGDS_PTGDS0                    _PTGDS.Bits.PTGDS0
#define PTGDS_PTGDS1                    _PTGDS.Bits.PTGDS1
#define PTGDS_PTGDS2                    _PTGDS.Bits.PTGDS2
#define PTGDS_PTGDS3                    _PTGDS.Bits.PTGDS3
#define PTGDS_PTGDS4                    _PTGDS.Bits.PTGDS4
#define PTGDS_PTGDS5                    _PTGDS.Bits.PTGDS5
#define PTGDS_PTGDS                     _PTGDS.MergedBits.grpPTGDS

#define PTGDS_PTGDS0_MASK               1
#define PTGDS_PTGDS1_MASK               2
#define PTGDS_PTGDS2_MASK               4
#define PTGDS_PTGDS3_MASK               8
#define PTGDS_PTGDS4_MASK               16
#define PTGDS_PTGDS5_MASK               32
#define PTGDS_PTGDS_MASK                63
#define PTGDS_PTGDS_BITNUM              0


/*** CANCTL0 - MSCAN Control 0 Register; 0x00001880 ***/
typedef union {
  byte Byte;
  struct {
    byte INITRQ      :1;                                       /* Initialization Mode Request */
    byte SLPRQ       :1;                                       /* Sleep Mode Request */
    byte WUPE        :1;                                       /* Wake-Up Enable */
    byte TIME        :1;                                       /* Timer Enable */
    byte SYNCH       :1;                                       /* Synchronized Status */
    byte CSWAI       :1;                                       /* CAN Stops in Wait Mode */
    byte RXACT       :1;                                       /* Receiver Active Status */
    byte RXFRM       :1;                                       /* Received Frame Flag */
  } Bits;
} CANCTL0STR;
extern volatile CANCTL0STR _CANCTL0 @0x00001880;
#define CANCTL0                         _CANCTL0.Byte
#define CANCTL0_INITRQ                  _CANCTL0.Bits.INITRQ
#define CANCTL0_SLPRQ                   _CANCTL0.Bits.SLPRQ
#define CANCTL0_WUPE                    _CANCTL0.Bits.WUPE
#define CANCTL0_TIME                    _CANCTL0.Bits.TIME
#define CANCTL0_SYNCH                   _CANCTL0.Bits.SYNCH
#define CANCTL0_CSWAI                   _CANCTL0.Bits.CSWAI
#define CANCTL0_RXACT                   _CANCTL0.Bits.RXACT
#define CANCTL0_RXFRM                   _CANCTL0.Bits.RXFRM
/* CANCTL_ARR: Access 2 CANCTLx registers in an array */
#define CANCTL_ARR                      ((volatile byte * __far) &CANCTL0)

#define CANCTL0_INITRQ_MASK             1
#define CANCTL0_SLPRQ_MASK              2
#define CANCTL0_WUPE_MASK               4
#define CANCTL0_TIME_MASK               8
#define CANCTL0_SYNCH_MASK              16
#define CANCTL0_CSWAI_MASK              32
#define CANCTL0_RXACT_MASK              64
#define CANCTL0_RXFRM_MASK              128


/*** CANCTL1 - MSCAN Control 1 Register; 0x00001881 ***/
typedef union {
  byte Byte;
  struct {
    byte INITAK      :1;                                       /* Initialization Mode Acknowledge */
    byte SLPAK       :1;                                       /* Sleep Mode Acknowledge */
    byte WUPM        :1;                                       /* Wake-Up Mode */
    byte BORM        :1;                                       /* Bus-Off Recovery Mode */
    byte LISTEN      :1;                                       /* Listen Only Mode */
    byte LOOPB       :1;                                       /* Loop Back Self Test Mode */
    byte CLKSRC      :1;                                       /* MSCAN Clock Source */
    byte CANE        :1;                                       /* MSCAN Enable */
  } Bits;
} CANCTL1STR;
extern volatile CANCTL1STR _CANCTL1 @0x00001881;
#define CANCTL1                         _CANCTL1.Byte
#define CANCTL1_INITAK                  _CANCTL1.Bits.INITAK
#define CANCTL1_SLPAK                   _CANCTL1.Bits.SLPAK
#define CANCTL1_WUPM                    _CANCTL1.Bits.WUPM
#define CANCTL1_BORM                    _CANCTL1.Bits.BORM
#define CANCTL1_LISTEN                  _CANCTL1.Bits.LISTEN
#define CANCTL1_LOOPB                   _CANCTL1.Bits.LOOPB
#define CANCTL1_CLKSRC                  _CANCTL1.Bits.CLKSRC
#define CANCTL1_CANE                    _CANCTL1.Bits.CANE

#define CANCTL1_INITAK_MASK             1
#define CANCTL1_SLPAK_MASK              2
#define CANCTL1_WUPM_MASK               4
#define CANCTL1_BORM_MASK               8
#define CANCTL1_LISTEN_MASK             16
#define CANCTL1_LOOPB_MASK              32
#define CANCTL1_CLKSRC_MASK             64
#define CANCTL1_CANE_MASK               128


/*** CANBTR0 - MSCAN Bus Timing Register 0; 0x00001882 ***/
typedef union {
  byte Byte;
  struct {
    byte BRP0        :1;                                       /* Baud Rate Prescaler 0 */
    byte BRP1        :1;                                       /* Baud Rate Prescaler 1 */
    byte BRP2        :1;                                       /* Baud Rate Prescaler 2 */
    byte BRP3        :1;                                       /* Baud Rate Prescaler 3 */
    byte BRP4        :1;                                       /* Baud Rate Prescaler 4 */
    byte BRP5        :1;                                       /* Baud Rate Prescaler 5 */
    byte SJW0        :1;                                       /* Synchronization Jump Width 0 */
    byte SJW1        :1;                                       /* Synchronization Jump Width 1 */
  } Bits;
  struct {
    byte grpBRP  :6;
    byte grpSJW  :2;
  } MergedBits;
} CANBTR0STR;
extern volatile CANBTR0STR _CANBTR0 @0x00001882;
#define CANBTR0                         _CANBTR0.Byte
#define CANBTR0_BRP0                    _CANBTR0.Bits.BRP0
#define CANBTR0_BRP1                    _CANBTR0.Bits.BRP1
#define CANBTR0_BRP2                    _CANBTR0.Bits.BRP2
#define CANBTR0_BRP3                    _CANBTR0.Bits.BRP3
#define CANBTR0_BRP4                    _CANBTR0.Bits.BRP4
#define CANBTR0_BRP5                    _CANBTR0.Bits.BRP5
#define CANBTR0_SJW0                    _CANBTR0.Bits.SJW0
#define CANBTR0_SJW1                    _CANBTR0.Bits.SJW1
/* CANBTR_ARR: Access 2 CANBTRx registers in an array */
#define CANBTR_ARR                      ((volatile byte * __far) &CANBTR0)
#define CANBTR0_BRP                     _CANBTR0.MergedBits.grpBRP
#define CANBTR0_SJW                     _CANBTR0.MergedBits.grpSJW

#define CANBTR0_BRP0_MASK               1
#define CANBTR0_BRP1_MASK               2
#define CANBTR0_BRP2_MASK               4
#define CANBTR0_BRP3_MASK               8
#define CANBTR0_BRP4_MASK               16
#define CANBTR0_BRP5_MASK               32
#define CANBTR0_SJW0_MASK               64
#define CANBTR0_SJW1_MASK               128
#define CANBTR0_BRP_MASK                63
#define CANBTR0_BRP_BITNUM              0
#define CANBTR0_SJW_MASK                192
#define CANBTR0_SJW_BITNUM              6


/*** CANBTR1 - MSCAN Bus Timing Register 1; 0x00001883 ***/
typedef union {
  byte Byte;
  struct {
    byte TSEG10      :1;                                       /* Time Segment 10 */
    byte TSEG11      :1;                                       /* Time Segment 11 */
    byte TSEG12      :1;                                       /* Time Segment 12 */
    byte TSEG13      :1;                                       /* Time Segment 13 */
    byte TSEG20      :1;                                       /* Time Segment 20 */
    byte TSEG21      :1;                                       /* Time Segment 21 */
    byte TSEG22      :1;                                       /* Time Segment 22 */
    byte SAMP        :1;                                       /* Sampling */
  } Bits;
  struct {
    byte grpTSEG_10 :4;
    byte grpTSEG_20 :3;
    byte         :1;
  } MergedBits;
} CANBTR1STR;
extern volatile CANBTR1STR _CANBTR1 @0x00001883;
#define CANBTR1                         _CANBTR1.Byte
#define CANBTR1_TSEG10                  _CANBTR1.Bits.TSEG10
#define CANBTR1_TSEG11                  _CANBTR1.Bits.TSEG11
#define CANBTR1_TSEG12                  _CANBTR1.Bits.TSEG12
#define CANBTR1_TSEG13                  _CANBTR1.Bits.TSEG13
#define CANBTR1_TSEG20                  _CANBTR1.Bits.TSEG20
#define CANBTR1_TSEG21                  _CANBTR1.Bits.TSEG21
#define CANBTR1_TSEG22                  _CANBTR1.Bits.TSEG22
#define CANBTR1_SAMP                    _CANBTR1.Bits.SAMP
#define CANBTR1_TSEG_10                 _CANBTR1.MergedBits.grpTSEG_10
#define CANBTR1_TSEG_20                 _CANBTR1.MergedBits.grpTSEG_20
#define CANBTR1_TSEG                    CANBTR1_TSEG_10

#define CANBTR1_TSEG10_MASK             1
#define CANBTR1_TSEG11_MASK             2
#define CANBTR1_TSEG12_MASK             4
#define CANBTR1_TSEG13_MASK             8
#define CANBTR1_TSEG20_MASK             16
#define CANBTR1_TSEG21_MASK             32
#define CANBTR1_TSEG22_MASK             64
#define CANBTR1_SAMP_MASK               128
#define CANBTR1_TSEG_10_MASK            15
#define CANBTR1_TSEG_10_BITNUM          0
#define CANBTR1_TSEG_20_MASK            112
#define CANBTR1_TSEG_20_BITNUM          4


/*** CANRFLG - MSCAN Receiver Flag Register; 0x00001884 ***/
typedef union {
  byte Byte;
  struct {
    byte RXF         :1;                                       /* Receive Buffer Full */
    byte OVRIF       :1;                                       /* Overrun Interrupt Flag */
    byte TSTAT0      :1;                                       /* Transmitter Status Bit 0 */
    byte TSTAT1      :1;                                       /* Transmitter Status Bit 1 */
    byte RSTAT0      :1;                                       /* Receiver Status Bit 0 */
    byte RSTAT1      :1;                                       /* Receiver Status Bit 1 */
    byte CSCIF       :1;                                       /* CAN Status Change Interrupt Flag */
    byte WUPIF       :1;                                       /* Wake-up Interrupt Flag */
  } Bits;
  struct {
    byte         :1;
    byte         :1;
    byte grpTSTAT :2;
    byte grpRSTAT :2;
    byte         :1;
    byte         :1;
  } MergedBits;
} CANRFLGSTR;
extern volatile CANRFLGSTR _CANRFLG @0x00001884;
#define CANRFLG                         _CANRFLG.Byte
#define CANRFLG_RXF                     _CANRFLG.Bits.RXF
#define CANRFLG_OVRIF                   _CANRFLG.Bits.OVRIF
#define CANRFLG_TSTAT0                  _CANRFLG.Bits.TSTAT0
#define CANRFLG_TSTAT1                  _CANRFLG.Bits.TSTAT1
#define CANRFLG_RSTAT0                  _CANRFLG.Bits.RSTAT0
#define CANRFLG_RSTAT1                  _CANRFLG.Bits.RSTAT1
#define CANRFLG_CSCIF                   _CANRFLG.Bits.CSCIF
#define CANRFLG_WUPIF                   _CANRFLG.Bits.WUPIF
#define CANRFLG_TSTAT                   _CANRFLG.MergedBits.grpTSTAT
#define CANRFLG_RSTAT                   _CANRFLG.MergedBits.grpRSTAT

#define CANRFLG_RXF_MASK                1
#define CANRFLG_OVRIF_MASK              2
#define CANRFLG_TSTAT0_MASK             4
#define CANRFLG_TSTAT1_MASK             8
#define CANRFLG_RSTAT0_MASK             16
#define CANRFLG_RSTAT1_MASK             32
#define CANRFLG_CSCIF_MASK              64
#define CANRFLG_WUPIF_MASK              128
#define CANRFLG_TSTAT_MASK              12
#define CANRFLG_TSTAT_BITNUM            2
#define CANRFLG_RSTAT_MASK              48
#define CANRFLG_RSTAT_BITNUM            4


/*** CANRIER - MSCAN Receiver Interrupt Enable Register; 0x00001885 ***/
typedef union {
  byte Byte;
  struct {
    byte RXFIE       :1;                                       /* Receiver Full Interrupt Enable */
    byte OVRIE       :1;                                       /* Overrun Interrupt Enable */
    byte TSTATE0     :1;                                       /* Transmitter Status Change Enable 0 */
    byte TSTATE1     :1;                                       /* Transmitter Status Change Enable 1 */
    byte RSTATE0     :1;                                       /* Receiver Status Change Enable 0 */
    byte RSTATE1     :1;                                       /* Receiver Status Change Enable 1 */
    byte CSCIE       :1;                                       /* CAN Status Change Interrupt Enable */
    byte WUPIE       :1;                                       /* Wake-up Interrupt Enable */
  } Bits;
  struct {
    byte         :1;
    byte         :1;
    byte grpTSTATE :2;
    byte grpRSTATE :2;
    byte         :1;
    byte         :1;
  } MergedBits;
} CANRIERSTR;
extern volatile CANRIERSTR _CANRIER @0x00001885;
#define CANRIER                         _CANRIER.Byte
#define CANRIER_RXFIE                   _CANRIER.Bits.RXFIE
#define CANRIER_OVRIE                   _CANRIER.Bits.OVRIE
#define CANRIER_TSTATE0                 _CANRIER.Bits.TSTATE0
#define CANRIER_TSTATE1                 _CANRIER.Bits.TSTATE1
#define CANRIER_RSTATE0                 _CANRIER.Bits.RSTATE0
#define CANRIER_RSTATE1                 _CANRIER.Bits.RSTATE1
#define CANRIER_CSCIE                   _CANRIER.Bits.CSCIE
#define CANRIER_WUPIE                   _CANRIER.Bits.WUPIE
#define CANRIER_TSTATE                  _CANRIER.MergedBits.grpTSTATE
#define CANRIER_RSTATE                  _CANRIER.MergedBits.grpRSTATE

#define CANRIER_RXFIE_MASK              1
#define CANRIER_OVRIE_MASK              2
#define CANRIER_TSTATE0_MASK            4
#define CANRIER_TSTATE1_MASK            8
#define CANRIER_RSTATE0_MASK            16
#define CANRIER_RSTATE1_MASK            32
#define CANRIER_CSCIE_MASK              64
#define CANRIER_WUPIE_MASK              128
#define CANRIER_TSTATE_MASK             12
#define CANRIER_TSTATE_BITNUM           2
#define CANRIER_RSTATE_MASK             48
#define CANRIER_RSTATE_BITNUM           4


/*** CANTFLG - MSCAN Transmitter Flag Register; 0x00001886 ***/
typedef union {
  byte Byte;
  struct {
    byte TXE0        :1;                                       /* Transmitter Buffer Empty 0 */
    byte TXE1        :1;                                       /* Transmitter Buffer Empty 1 */
    byte TXE2        :1;                                       /* Transmitter Buffer Empty 2 */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
  } Bits;
  struct {
    byte grpTXE  :3;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} CANTFLGSTR;
extern volatile CANTFLGSTR _CANTFLG @0x00001886;
#define CANTFLG                         _CANTFLG.Byte
#define CANTFLG_TXE0                    _CANTFLG.Bits.TXE0
#define CANTFLG_TXE1                    _CANTFLG.Bits.TXE1
#define CANTFLG_TXE2                    _CANTFLG.Bits.TXE2
#define CANTFLG_TXE                     _CANTFLG.MergedBits.grpTXE

#define CANTFLG_TXE0_MASK               1
#define CANTFLG_TXE1_MASK               2
#define CANTFLG_TXE2_MASK               4
#define CANTFLG_TXE_MASK                7
#define CANTFLG_TXE_BITNUM              0


/*** CANTIER - MSCAN Transmitter Interrupt Enable Register; 0x00001887 ***/
typedef union {
  byte Byte;
  struct {
    byte TXEIE0      :1;                                       /* Transmitter Empty Interrupt Enable 0 */
    byte TXEIE1      :1;                                       /* Transmitter Empty Interrupt Enable 1 */
    byte TXEIE2      :1;                                       /* Transmitter Empty Interrupt Enable 2 */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
  } Bits;
  struct {
    byte grpTXEIE :3;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} CANTIERSTR;
extern volatile CANTIERSTR _CANTIER @0x00001887;
#define CANTIER                         _CANTIER.Byte
#define CANTIER_TXEIE0                  _CANTIER.Bits.TXEIE0
#define CANTIER_TXEIE1                  _CANTIER.Bits.TXEIE1
#define CANTIER_TXEIE2                  _CANTIER.Bits.TXEIE2
#define CANTIER_TXEIE                   _CANTIER.MergedBits.grpTXEIE

#define CANTIER_TXEIE0_MASK             1
#define CANTIER_TXEIE1_MASK             2
#define CANTIER_TXEIE2_MASK             4
#define CANTIER_TXEIE_MASK              7
#define CANTIER_TXEIE_BITNUM            0


/*** CANTARQ - MSCAN Transmitter Message Abort Request; 0x00001888 ***/
typedef union {
  byte Byte;
  struct {
    byte ABTRQ0      :1;                                       /* Abort Request 0 */
    byte ABTRQ1      :1;                                       /* Abort Request 1 */
    byte ABTRQ2      :1;                                       /* Abort Request 2 */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
  } Bits;
  struct {
    byte grpABTRQ :3;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} CANTARQSTR;
extern volatile CANTARQSTR _CANTARQ @0x00001888;
#define CANTARQ                         _CANTARQ.Byte
#define CANTARQ_ABTRQ0                  _CANTARQ.Bits.ABTRQ0
#define CANTARQ_ABTRQ1                  _CANTARQ.Bits.ABTRQ1
#define CANTARQ_ABTRQ2                  _CANTARQ.Bits.ABTRQ2
#define CANTARQ_ABTRQ                   _CANTARQ.MergedBits.grpABTRQ

#define CANTARQ_ABTRQ0_MASK             1
#define CANTARQ_ABTRQ1_MASK             2
#define CANTARQ_ABTRQ2_MASK             4
#define CANTARQ_ABTRQ_MASK              7
#define CANTARQ_ABTRQ_BITNUM            0


/*** CANTAAK - MSCAN Transmitter Message Abort Acknowledge; 0x00001889 ***/
typedef union {
  byte Byte;
  struct {
    byte ABTAK0      :1;                                       /* Abort Acknowledge 0 */
    byte ABTAK1      :1;                                       /* Abort Acknowledge 1 */
    byte ABTAK2      :1;                                       /* Abort Acknowledge 2 */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
  } Bits;
  struct {
    byte grpABTAK :3;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} CANTAAKSTR;
extern volatile CANTAAKSTR _CANTAAK @0x00001889;
#define CANTAAK                         _CANTAAK.Byte
#define CANTAAK_ABTAK0                  _CANTAAK.Bits.ABTAK0
#define CANTAAK_ABTAK1                  _CANTAAK.Bits.ABTAK1
#define CANTAAK_ABTAK2                  _CANTAAK.Bits.ABTAK2
#define CANTAAK_ABTAK                   _CANTAAK.MergedBits.grpABTAK

#define CANTAAK_ABTAK0_MASK             1
#define CANTAAK_ABTAK1_MASK             2
#define CANTAAK_ABTAK2_MASK             4
#define CANTAAK_ABTAK_MASK              7
#define CANTAAK_ABTAK_BITNUM            0


/*** CANTBSEL - MSCAN Transmit Buffer Selection; 0x0000188A ***/
typedef union {
  byte Byte;
  struct {
    byte TX0         :1;                                       /* Transmit Buffer Select 0 */
    byte TX1         :1;                                       /* Transmit Buffer Select 1 */
    byte TX2         :1;                                       /* Transmit Buffer Select 2 */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
  } Bits;
  struct {
    byte grpTX   :3;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} CANTBSELSTR;
extern volatile CANTBSELSTR _CANTBSEL @0x0000188A;
#define CANTBSEL                        _CANTBSEL.Byte
#define CANTBSEL_TX0                    _CANTBSEL.Bits.TX0
#define CANTBSEL_TX1                    _CANTBSEL.Bits.TX1
#define CANTBSEL_TX2                    _CANTBSEL.Bits.TX2
#define CANTBSEL_TX                     _CANTBSEL.MergedBits.grpTX

#define CANTBSEL_TX0_MASK               1
#define CANTBSEL_TX1_MASK               2
#define CANTBSEL_TX2_MASK               4
#define CANTBSEL_TX_MASK                7
#define CANTBSEL_TX_BITNUM              0


/*** CANIDAC - MSCAN Identifier Acceptance Control Register; 0x0000188B ***/
typedef union {
  byte Byte;
  struct {
    byte IDHIT0      :1;                                       /* Identifier Acceptance Hit Indicator 0 */
    byte IDHIT1      :1;                                       /* Identifier Acceptance Hit Indicator 1 */
    byte IDHIT2      :1;                                       /* Identifier Acceptance Hit Indicator 2 */
    byte             :1; 
    byte IDAM0       :1;                                       /* Identifier Acceptance Mode 0 */
    byte IDAM1       :1;                                       /* Identifier Acceptance Mode 1 */
    byte             :1; 
    byte             :1; 
  } Bits;
  struct {
    byte grpIDHIT :3;
    byte         :1;
    byte grpIDAM :2;
    byte         :1;
    byte         :1;
  } MergedBits;
} CANIDACSTR;
extern volatile CANIDACSTR _CANIDAC @0x0000188B;
#define CANIDAC                         _CANIDAC.Byte
#define CANIDAC_IDHIT0                  _CANIDAC.Bits.IDHIT0
#define CANIDAC_IDHIT1                  _CANIDAC.Bits.IDHIT1
#define CANIDAC_IDHIT2                  _CANIDAC.Bits.IDHIT2
#define CANIDAC_IDAM0                   _CANIDAC.Bits.IDAM0
#define CANIDAC_IDAM1                   _CANIDAC.Bits.IDAM1
#define CANIDAC_IDHIT                   _CANIDAC.MergedBits.grpIDHIT
#define CANIDAC_IDAM                    _CANIDAC.MergedBits.grpIDAM

#define CANIDAC_IDHIT0_MASK             1
#define CANIDAC_IDHIT1_MASK             2
#define CANIDAC_IDHIT2_MASK             4
#define CANIDAC_IDAM0_MASK              16
#define CANIDAC_IDAM1_MASK              32
#define CANIDAC_IDHIT_MASK              7
#define CANIDAC_IDHIT_BITNUM            0
#define CANIDAC_IDAM_MASK               48
#define CANIDAC_IDAM_BITNUM             4


/*** CANMISC - MSCAN Miscellaneous Register; 0x0000188D ***/
typedef union {
  byte Byte;
  struct {
    byte BOHOLD      :1;                                       /* Bus-off State Hold Until User Request - If BORM is set */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
  } Bits;
} CANMISCSTR;
extern volatile CANMISCSTR _CANMISC @0x0000188D;
#define CANMISC                         _CANMISC.Byte
#define CANMISC_BOHOLD                  _CANMISC.Bits.BOHOLD

#define CANMISC_BOHOLD_MASK             1


/*** CANRXERR - MSCAN Receive Error Counter Register; 0x0000188E ***/
typedef union {
  byte Byte;
  struct {
    byte RXERR0      :1;                                       /* Bit 0 */
    byte RXERR1      :1;                                       /* Bit 1 */
    byte RXERR2      :1;                                       /* Bit 2 */
    byte RXERR3      :1;                                       /* Bit 3 */
    byte RXERR4      :1;                                       /* Bit 4 */
    byte RXERR5      :1;                                       /* Bit 5 */
    byte RXERR6      :1;                                       /* Bit 6 */
    byte RXERR7      :1;                                       /* Bit 7 */
  } Bits;
} CANRXERRSTR;
extern volatile CANRXERRSTR _CANRXERR @0x0000188E;
#define CANRXERR                        _CANRXERR.Byte
#define CANRXERR_RXERR0                 _CANRXERR.Bits.RXERR0
#define CANRXERR_RXERR1                 _CANRXERR.Bits.RXERR1
#define CANRXERR_RXERR2                 _CANRXERR.Bits.RXERR2
#define CANRXERR_RXERR3                 _CANRXERR.Bits.RXERR3
#define CANRXERR_RXERR4                 _CANRXERR.Bits.RXERR4
#define CANRXERR_RXERR5                 _CANRXERR.Bits.RXERR5
#define CANRXERR_RXERR6                 _CANRXERR.Bits.RXERR6
#define CANRXERR_RXERR7                 _CANRXERR.Bits.RXERR7

#define CANRXERR_RXERR0_MASK            1
#define CANRXERR_RXERR1_MASK            2
#define CANRXERR_RXERR2_MASK            4
#define CANRXERR_RXERR3_MASK            8
#define CANRXERR_RXERR4_MASK            16
#define CANRXERR_RXERR5_MASK            32
#define CANRXERR_RXERR6_MASK            64
#define CANRXERR_RXERR7_MASK            128


/*** CANTXERR - MSCAN Transmit Error Counter Register; 0x0000188F ***/
typedef union {
  byte Byte;
  struct {
    byte TXERR0      :1;                                       /* Bit 0 */
    byte TXERR1      :1;                                       /* Bit 1 */
    byte TXERR2      :1;                                       /* Bit 2 */
    byte TXERR3      :1;                                       /* Bit 3 */
    byte TXERR4      :1;                                       /* Bit 4 */
    byte TXERR5      :1;                                       /* Bit 5 */
    byte TXERR6      :1;                                       /* Bit 6 */
    byte TXERR7      :1;                                       /* Bit 7 */
  } Bits;
} CANTXERRSTR;
extern volatile CANTXERRSTR _CANTXERR @0x0000188F;
#define CANTXERR                        _CANTXERR.Byte
#define CANTXERR_TXERR0                 _CANTXERR.Bits.TXERR0
#define CANTXERR_TXERR1                 _CANTXERR.Bits.TXERR1
#define CANTXERR_TXERR2                 _CANTXERR.Bits.TXERR2
#define CANTXERR_TXERR3                 _CANTXERR.Bits.TXERR3
#define CANTXERR_TXERR4                 _CANTXERR.Bits.TXERR4
#define CANTXERR_TXERR5                 _CANTXERR.Bits.TXERR5
#define CANTXERR_TXERR6                 _CANTXERR.Bits.TXERR6
#define CANTXERR_TXERR7                 _CANTXERR.Bits.TXERR7

#define CANTXERR_TXERR0_MASK            1
#define CANTXERR_TXERR1_MASK            2
#define CANTXERR_TXERR2_MASK            4
#define CANTXERR_TXERR3_MASK            8
#define CANTXERR_TXERR4_MASK            16
#define CANTXERR_TXERR5_MASK            32
#define CANTXERR_TXERR6_MASK            64
#define CANTXERR_TXERR7_MASK            128


/*** CANIDAR0 - MSCAN Identifier Acceptance Register 0; 0x00001890 ***/
typedef union {
  byte Byte;
  struct {
    byte AC0         :1;                                       /* Acceptance Code Bit 0 */
    byte AC1         :1;                                       /* Acceptance Code Bit 1 */
    byte AC2         :1;                                       /* Acceptance Code Bit 2 */
    byte AC3         :1;                                       /* Acceptance Code Bit 3 */
    byte AC4         :1;                                       /* Acceptance Code Bit 4 */
    byte AC5         :1;                                       /* Acceptance Code Bit 5 */
    byte AC6         :1;                                       /* Acceptance Code Bit 6 */
    byte AC7         :1;                                       /* Acceptance Code Bit 7 */
  } Bits;
} CANIDAR0STR;
extern volatile CANIDAR0STR _CANIDAR0 @0x00001890;
#define CANIDAR0                        _CANIDAR0.Byte
#define CANIDAR0_AC0                    _CANIDAR0.Bits.AC0
#define CANIDAR0_AC1                    _CANIDAR0.Bits.AC1
#define CANIDAR0_AC2                    _CANIDAR0.Bits.AC2
#define CANIDAR0_AC3                    _CANIDAR0.Bits.AC3
#define CANIDAR0_AC4                    _CANIDAR0.Bits.AC4
#define CANIDAR0_AC5                    _CANIDAR0.Bits.AC5
#define CANIDAR0_AC6                    _CANIDAR0.Bits.AC6
#define CANIDAR0_AC7                    _CANIDAR0.Bits.AC7
/* CANIDAR_ARR: Access 4 CANIDARx registers in an array */
#define CANIDAR_ARR                     ((volatile byte * __far) &CANIDAR0)

#define CANIDAR0_AC0_MASK               1
#define CANIDAR0_AC1_MASK               2
#define CANIDAR0_AC2_MASK               4
#define CANIDAR0_AC3_MASK               8
#define CANIDAR0_AC4_MASK               16
#define CANIDAR0_AC5_MASK               32
#define CANIDAR0_AC6_MASK               64
#define CANIDAR0_AC7_MASK               128


/*** CANIDAR1 - MSCAN Identifier Acceptance Register 1; 0x00001891 ***/
typedef union {
  byte Byte;
  struct {
    byte AC0         :1;                                       /* Acceptance Code Bit 0 */
    byte AC1         :1;                                       /* Acceptance Code Bit 1 */
    byte AC2         :1;                                       /* Acceptance Code Bit 2 */
    byte AC3         :1;                                       /* Acceptance Code Bit 3 */
    byte AC4         :1;                                       /* Acceptance Code Bit 4 */
    byte AC5         :1;                                       /* Acceptance Code Bit 5 */
    byte AC6         :1;                                       /* Acceptance Code Bit 6 */
    byte AC7         :1;                                       /* Acceptance Code Bit 7 */
  } Bits;
} CANIDAR1STR;
extern volatile CANIDAR1STR _CANIDAR1 @0x00001891;
#define CANIDAR1                        _CANIDAR1.Byte
#define CANIDAR1_AC0                    _CANIDAR1.Bits.AC0
#define CANIDAR1_AC1                    _CANIDAR1.Bits.AC1
#define CANIDAR1_AC2                    _CANIDAR1.Bits.AC2
#define CANIDAR1_AC3                    _CANIDAR1.Bits.AC3
#define CANIDAR1_AC4                    _CANIDAR1.Bits.AC4
#define CANIDAR1_AC5                    _CANIDAR1.Bits.AC5
#define CANIDAR1_AC6                    _CANIDAR1.Bits.AC6
#define CANIDAR1_AC7                    _CANIDAR1.Bits.AC7

#define CANIDAR1_AC0_MASK               1
#define CANIDAR1_AC1_MASK               2
#define CANIDAR1_AC2_MASK               4
#define CANIDAR1_AC3_MASK               8
#define CANIDAR1_AC4_MASK               16
#define CANIDAR1_AC5_MASK               32
#define CANIDAR1_AC6_MASK               64
#define CANIDAR1_AC7_MASK               128


/*** CANIDAR2 - MSCAN Identifier Acceptance Register 2; 0x00001892 ***/
typedef union {
  byte Byte;
  struct {
    byte AC0         :1;                                       /* Acceptance Code Bit 0 */
    byte AC1         :1;                                       /* Acceptance Code Bit 1 */
    byte AC2         :1;                                       /* Acceptance Code Bit 2 */
    byte AC3         :1;                                       /* Acceptance Code Bit 3 */
    byte AC4         :1;                                       /* Acceptance Code Bit 4 */
    byte AC5         :1;                                       /* Acceptance Code Bit 5 */
    byte AC6         :1;                                       /* Acceptance Code Bit 6 */
    byte AC7         :1;                                       /* Acceptance Code Bit 7 */
  } Bits;
} CANIDAR2STR;
extern volatile CANIDAR2STR _CANIDAR2 @0x00001892;
#define CANIDAR2                        _CANIDAR2.Byte
#define CANIDAR2_AC0                    _CANIDAR2.Bits.AC0
#define CANIDAR2_AC1                    _CANIDAR2.Bits.AC1
#define CANIDAR2_AC2                    _CANIDAR2.Bits.AC2
#define CANIDAR2_AC3                    _CANIDAR2.Bits.AC3
#define CANIDAR2_AC4                    _CANIDAR2.Bits.AC4
#define CANIDAR2_AC5                    _CANIDAR2.Bits.AC5
#define CANIDAR2_AC6                    _CANIDAR2.Bits.AC6
#define CANIDAR2_AC7                    _CANIDAR2.Bits.AC7

#define CANIDAR2_AC0_MASK               1
#define CANIDAR2_AC1_MASK               2
#define CANIDAR2_AC2_MASK               4
#define CANIDAR2_AC3_MASK               8
#define CANIDAR2_AC4_MASK               16
#define CANIDAR2_AC5_MASK               32
#define CANIDAR2_AC6_MASK               64
#define CANIDAR2_AC7_MASK               128


/*** CANIDAR3 - MSCAN Identifier Acceptance Register 3; 0x00001893 ***/
typedef union {
  byte Byte;
  struct {
    byte AC0         :1;                                       /* Acceptance Code Bit 0 */
    byte AC1         :1;                                       /* Acceptance Code Bit 1 */
    byte AC2         :1;                                       /* Acceptance Code Bit 2 */
    byte AC3         :1;                                       /* Acceptance Code Bit 3 */
    byte AC4         :1;                                       /* Acceptance Code Bit 4 */
    byte AC5         :1;                                       /* Acceptance Code Bit 5 */
    byte AC6         :1;                                       /* Acceptance Code Bit 6 */
    byte AC7         :1;                                       /* Acceptance Code Bit 7 */
  } Bits;
} CANIDAR3STR;
extern volatile CANIDAR3STR _CANIDAR3 @0x00001893;
#define CANIDAR3                        _CANIDAR3.Byte
#define CANIDAR3_AC0                    _CANIDAR3.Bits.AC0
#define CANIDAR3_AC1                    _CANIDAR3.Bits.AC1
#define CANIDAR3_AC2                    _CANIDAR3.Bits.AC2
#define CANIDAR3_AC3                    _CANIDAR3.Bits.AC3
#define CANIDAR3_AC4                    _CANIDAR3.Bits.AC4
#define CANIDAR3_AC5                    _CANIDAR3.Bits.AC5
#define CANIDAR3_AC6                    _CANIDAR3.Bits.AC6
#define CANIDAR3_AC7                    _CANIDAR3.Bits.AC7

#define CANIDAR3_AC0_MASK               1
#define CANIDAR3_AC1_MASK               2
#define CANIDAR3_AC2_MASK               4
#define CANIDAR3_AC3_MASK               8
#define CANIDAR3_AC4_MASK               16
#define CANIDAR3_AC5_MASK               32
#define CANIDAR3_AC6_MASK               64
#define CANIDAR3_AC7_MASK               128


/*** CANIDMR0 - MSCAN Identifier Mask Register 0; 0x00001894 ***/
typedef union {
  byte Byte;
  struct {
    byte AM0         :1;                                       /* Acceptance Mask Bit 0 */
    byte AM1         :1;                                       /* Acceptance Mask Bit 1 */
    byte AM2         :1;                                       /* Acceptance Mask Bit 2 */
    byte AM3         :1;                                       /* Acceptance Mask Bit 3 */
    byte AM4         :1;                                       /* Acceptance Mask Bit 4 */
    byte AM5         :1;                                       /* Acceptance Mask Bit 5 */
    byte AM6         :1;                                       /* Acceptance Mask Bit 6 */
    byte AM7         :1;                                       /* Acceptance Mask Bit 7 */
  } Bits;
} CANIDMR0STR;
extern volatile CANIDMR0STR _CANIDMR0 @0x00001894;
#define CANIDMR0                        _CANIDMR0.Byte
#define CANIDMR0_AM0                    _CANIDMR0.Bits.AM0
#define CANIDMR0_AM1                    _CANIDMR0.Bits.AM1
#define CANIDMR0_AM2                    _CANIDMR0.Bits.AM2
#define CANIDMR0_AM3                    _CANIDMR0.Bits.AM3
#define CANIDMR0_AM4                    _CANIDMR0.Bits.AM4
#define CANIDMR0_AM5                    _CANIDMR0.Bits.AM5
#define CANIDMR0_AM6                    _CANIDMR0.Bits.AM6
#define CANIDMR0_AM7                    _CANIDMR0.Bits.AM7
/* CANIDMR_ARR: Access 4 CANIDMRx registers in an array */
#define CANIDMR_ARR                     ((volatile byte * __far) &CANIDMR0)

#define CANIDMR0_AM0_MASK               1
#define CANIDMR0_AM1_MASK               2
#define CANIDMR0_AM2_MASK               4
#define CANIDMR0_AM3_MASK               8
#define CANIDMR0_AM4_MASK               16
#define CANIDMR0_AM5_MASK               32
#define CANIDMR0_AM6_MASK               64
#define CANIDMR0_AM7_MASK               128


/*** CANIDMR1 - MSCAN Identifier Mask Register 1; 0x00001895 ***/
typedef union {
  byte Byte;
  struct {
    byte AM0         :1;                                       /* Acceptance Mask Bit 0 */
    byte AM1         :1;                                       /* Acceptance Mask Bit 1 */
    byte AM2         :1;                                       /* Acceptance Mask Bit 2 */
    byte AM3         :1;                                       /* Acceptance Mask Bit 3 */
    byte AM4         :1;                                       /* Acceptance Mask Bit 4 */
    byte AM5         :1;                                       /* Acceptance Mask Bit 5 */
    byte AM6         :1;                                       /* Acceptance Mask Bit 6 */
    byte AM7         :1;                                       /* Acceptance Mask Bit 7 */
  } Bits;
} CANIDMR1STR;
extern volatile CANIDMR1STR _CANIDMR1 @0x00001895;
#define CANIDMR1                        _CANIDMR1.Byte
#define CANIDMR1_AM0                    _CANIDMR1.Bits.AM0
#define CANIDMR1_AM1                    _CANIDMR1.Bits.AM1
#define CANIDMR1_AM2                    _CANIDMR1.Bits.AM2
#define CANIDMR1_AM3                    _CANIDMR1.Bits.AM3
#define CANIDMR1_AM4                    _CANIDMR1.Bits.AM4
#define CANIDMR1_AM5                    _CANIDMR1.Bits.AM5
#define CANIDMR1_AM6                    _CANIDMR1.Bits.AM6
#define CANIDMR1_AM7                    _CANIDMR1.Bits.AM7

#define CANIDMR1_AM0_MASK               1
#define CANIDMR1_AM1_MASK               2
#define CANIDMR1_AM2_MASK               4
#define CANIDMR1_AM3_MASK               8
#define CANIDMR1_AM4_MASK               16
#define CANIDMR1_AM5_MASK               32
#define CANIDMR1_AM6_MASK               64
#define CANIDMR1_AM7_MASK               128


/*** CANIDMR2 - MSCAN Identifier Mask Register 2; 0x00001896 ***/
typedef union {
  byte Byte;
  struct {
    byte AM0         :1;                                       /* Acceptance Mask Bit 0 */
    byte AM1         :1;                                       /* Acceptance Mask Bit 1 */
    byte AM2         :1;                                       /* Acceptance Mask Bit 2 */
    byte AM3         :1;                                       /* Acceptance Mask Bit 3 */
    byte AM4         :1;                                       /* Acceptance Mask Bit 4 */
    byte AM5         :1;                                       /* Acceptance Mask Bit 5 */
    byte AM6         :1;                                       /* Acceptance Mask Bit 6 */
    byte AM7         :1;                                       /* Acceptance Mask Bit 7 */
  } Bits;
} CANIDMR2STR;
extern volatile CANIDMR2STR _CANIDMR2 @0x00001896;
#define CANIDMR2                        _CANIDMR2.Byte
#define CANIDMR2_AM0                    _CANIDMR2.Bits.AM0
#define CANIDMR2_AM1                    _CANIDMR2.Bits.AM1
#define CANIDMR2_AM2                    _CANIDMR2.Bits.AM2
#define CANIDMR2_AM3                    _CANIDMR2.Bits.AM3
#define CANIDMR2_AM4                    _CANIDMR2.Bits.AM4
#define CANIDMR2_AM5                    _CANIDMR2.Bits.AM5
#define CANIDMR2_AM6                    _CANIDMR2.Bits.AM6
#define CANIDMR2_AM7                    _CANIDMR2.Bits.AM7

#define CANIDMR2_AM0_MASK               1
#define CANIDMR2_AM1_MASK               2
#define CANIDMR2_AM2_MASK               4
#define CANIDMR2_AM3_MASK               8
#define CANIDMR2_AM4_MASK               16
#define CANIDMR2_AM5_MASK               32
#define CANIDMR2_AM6_MASK               64
#define CANIDMR2_AM7_MASK               128


/*** CANIDMR3 - MSCAN Identifier Mask Register 3; 0x00001897 ***/
typedef union {
  byte Byte;
  struct {
    byte AM0         :1;                                       /* Acceptance Mask Bit 0 */
    byte AM1         :1;                                       /* Acceptance Mask Bit 1 */
    byte AM2         :1;                                       /* Acceptance Mask Bit 2 */
    byte AM3         :1;                                       /* Acceptance Mask Bit 3 */
    byte AM4         :1;                                       /* Acceptance Mask Bit 4 */
    byte AM5         :1;                                       /* Acceptance Mask Bit 5 */
    byte AM6         :1;                                       /* Acceptance Mask Bit 6 */
    byte AM7         :1;                                       /* Acceptance Mask Bit 7 */
  } Bits;
} CANIDMR3STR;
extern volatile CANIDMR3STR _CANIDMR3 @0x00001897;
#define CANIDMR3                        _CANIDMR3.Byte
#define CANIDMR3_AM0                    _CANIDMR3.Bits.AM0
#define CANIDMR3_AM1                    _CANIDMR3.Bits.AM1
#define CANIDMR3_AM2                    _CANIDMR3.Bits.AM2
#define CANIDMR3_AM3                    _CANIDMR3.Bits.AM3
#define CANIDMR3_AM4                    _CANIDMR3.Bits.AM4
#define CANIDMR3_AM5                    _CANIDMR3.Bits.AM5
#define CANIDMR3_AM6                    _CANIDMR3.Bits.AM6
#define CANIDMR3_AM7                    _CANIDMR3.Bits.AM7

#define CANIDMR3_AM0_MASK               1
#define CANIDMR3_AM1_MASK               2
#define CANIDMR3_AM2_MASK               4
#define CANIDMR3_AM3_MASK               8
#define CANIDMR3_AM4_MASK               16
#define CANIDMR3_AM5_MASK               32
#define CANIDMR3_AM6_MASK               64
#define CANIDMR3_AM7_MASK               128


/*** CANIDAR4 - MSCAN Identifier Acceptance Register 4; 0x00001898 ***/
typedef union {
  byte Byte;
  struct {
    byte AC0         :1;                                       /* Acceptance Code Bit 0 */
    byte AC1         :1;                                       /* Acceptance Code Bit 1 */
    byte AC2         :1;                                       /* Acceptance Code Bit 2 */
    byte AC3         :1;                                       /* Acceptance Code Bit 3 */
    byte AC4         :1;                                       /* Acceptance Code Bit 4 */
    byte AC5         :1;                                       /* Acceptance Code Bit 5 */
    byte AC6         :1;                                       /* Acceptance Code Bit 6 */
    byte AC7         :1;                                       /* Acceptance Code Bit 7 */
  } Bits;
} CANIDAR4STR;
extern volatile CANIDAR4STR _CANIDAR4 @0x00001898;
#define CANIDAR4                        _CANIDAR4.Byte
#define CANIDAR4_AC0                    _CANIDAR4.Bits.AC0
#define CANIDAR4_AC1                    _CANIDAR4.Bits.AC1
#define CANIDAR4_AC2                    _CANIDAR4.Bits.AC2
#define CANIDAR4_AC3                    _CANIDAR4.Bits.AC3
#define CANIDAR4_AC4                    _CANIDAR4.Bits.AC4
#define CANIDAR4_AC5                    _CANIDAR4.Bits.AC5
#define CANIDAR4_AC6                    _CANIDAR4.Bits.AC6
#define CANIDAR4_AC7                    _CANIDAR4.Bits.AC7

#define CANIDAR4_AC0_MASK               1
#define CANIDAR4_AC1_MASK               2
#define CANIDAR4_AC2_MASK               4
#define CANIDAR4_AC3_MASK               8
#define CANIDAR4_AC4_MASK               16
#define CANIDAR4_AC5_MASK               32
#define CANIDAR4_AC6_MASK               64
#define CANIDAR4_AC7_MASK               128


/*** CANIDAR5 - MSCAN Identifier Acceptance Register 5; 0x00001899 ***/
typedef union {
  byte Byte;
  struct {
    byte AC0         :1;                                       /* Acceptance Code Bit 0 */
    byte AC1         :1;                                       /* Acceptance Code Bit 1 */
    byte AC2         :1;                                       /* Acceptance Code Bit 2 */
    byte AC3         :1;                                       /* Acceptance Code Bit 3 */
    byte AC4         :1;                                       /* Acceptance Code Bit 4 */
    byte AC5         :1;                                       /* Acceptance Code Bit 5 */
    byte AC6         :1;                                       /* Acceptance Code Bit 6 */
    byte AC7         :1;                                       /* Acceptance Code Bit 7 */
  } Bits;
} CANIDAR5STR;
extern volatile CANIDAR5STR _CANIDAR5 @0x00001899;
#define CANIDAR5                        _CANIDAR5.Byte
#define CANIDAR5_AC0                    _CANIDAR5.Bits.AC0
#define CANIDAR5_AC1                    _CANIDAR5.Bits.AC1
#define CANIDAR5_AC2                    _CANIDAR5.Bits.AC2
#define CANIDAR5_AC3                    _CANIDAR5.Bits.AC3
#define CANIDAR5_AC4                    _CANIDAR5.Bits.AC4
#define CANIDAR5_AC5                    _CANIDAR5.Bits.AC5
#define CANIDAR5_AC6                    _CANIDAR5.Bits.AC6
#define CANIDAR5_AC7                    _CANIDAR5.Bits.AC7

#define CANIDAR5_AC0_MASK               1
#define CANIDAR5_AC1_MASK               2
#define CANIDAR5_AC2_MASK               4
#define CANIDAR5_AC3_MASK               8
#define CANIDAR5_AC4_MASK               16
#define CANIDAR5_AC5_MASK               32
#define CANIDAR5_AC6_MASK               64
#define CANIDAR5_AC7_MASK               128


/*** CANIDAR6 - MSCAN Identifier Acceptance Register 6; 0x0000189A ***/
typedef union {
  byte Byte;
  struct {
    byte AC0         :1;                                       /* Acceptance Code Bit 0 */
    byte AC1         :1;                                       /* Acceptance Code Bit 1 */
    byte AC2         :1;                                       /* Acceptance Code Bit 2 */
    byte AC3         :1;                                       /* Acceptance Code Bit 3 */
    byte AC4         :1;                                       /* Acceptance Code Bit 4 */
    byte AC5         :1;                                       /* Acceptance Code Bit 5 */
    byte AC6         :1;                                       /* Acceptance Code Bit 6 */
    byte AC7         :1;                                       /* Acceptance Code Bit 7 */
  } Bits;
} CANIDAR6STR;
extern volatile CANIDAR6STR _CANIDAR6 @0x0000189A;
#define CANIDAR6                        _CANIDAR6.Byte
#define CANIDAR6_AC0                    _CANIDAR6.Bits.AC0
#define CANIDAR6_AC1                    _CANIDAR6.Bits.AC1
#define CANIDAR6_AC2                    _CANIDAR6.Bits.AC2
#define CANIDAR6_AC3                    _CANIDAR6.Bits.AC3
#define CANIDAR6_AC4                    _CANIDAR6.Bits.AC4
#define CANIDAR6_AC5                    _CANIDAR6.Bits.AC5
#define CANIDAR6_AC6                    _CANIDAR6.Bits.AC6
#define CANIDAR6_AC7                    _CANIDAR6.Bits.AC7

#define CANIDAR6_AC0_MASK               1
#define CANIDAR6_AC1_MASK               2
#define CANIDAR6_AC2_MASK               4
#define CANIDAR6_AC3_MASK               8
#define CANIDAR6_AC4_MASK               16
#define CANIDAR6_AC5_MASK               32
#define CANIDAR6_AC6_MASK               64
#define CANIDAR6_AC7_MASK               128


/*** CANIDAR7 - MSCAN Identifier Acceptance Register 7; 0x0000189B ***/
typedef union {
  byte Byte;
  struct {
    byte AC0         :1;                                       /* Acceptance Code Bit 0 */
    byte AC1         :1;                                       /* Acceptance Code Bit 1 */
    byte AC2         :1;                                       /* Acceptance Code Bit 2 */
    byte AC3         :1;                                       /* Acceptance Code Bit 3 */
    byte AC4         :1;                                       /* Acceptance Code Bit 4 */
    byte AC5         :1;                                       /* Acceptance Code Bit 5 */
    byte AC6         :1;                                       /* Acceptance Code Bit 6 */
    byte AC7         :1;                                       /* Acceptance Code Bit 7 */
  } Bits;
} CANIDAR7STR;
extern volatile CANIDAR7STR _CANIDAR7 @0x0000189B;
#define CANIDAR7                        _CANIDAR7.Byte
#define CANIDAR7_AC0                    _CANIDAR7.Bits.AC0
#define CANIDAR7_AC1                    _CANIDAR7.Bits.AC1
#define CANIDAR7_AC2                    _CANIDAR7.Bits.AC2
#define CANIDAR7_AC3                    _CANIDAR7.Bits.AC3
#define CANIDAR7_AC4                    _CANIDAR7.Bits.AC4
#define CANIDAR7_AC5                    _CANIDAR7.Bits.AC5
#define CANIDAR7_AC6                    _CANIDAR7.Bits.AC6
#define CANIDAR7_AC7                    _CANIDAR7.Bits.AC7

#define CANIDAR7_AC0_MASK               1
#define CANIDAR7_AC1_MASK               2
#define CANIDAR7_AC2_MASK               4
#define CANIDAR7_AC3_MASK               8
#define CANIDAR7_AC4_MASK               16
#define CANIDAR7_AC5_MASK               32
#define CANIDAR7_AC6_MASK               64
#define CANIDAR7_AC7_MASK               128


/*** CANIDMR4 - MSCAN Identifier Mask Register 4; 0x0000189C ***/
typedef union {
  byte Byte;
  struct {
    byte AM0         :1;                                       /* Acceptance Mask Bit 0 */
    byte AM1         :1;                                       /* Acceptance Mask Bit 1 */
    byte AM2         :1;                                       /* Acceptance Mask Bit 2 */
    byte AM3         :1;                                       /* Acceptance Mask Bit 3 */
    byte AM4         :1;                                       /* Acceptance Mask Bit 4 */
    byte AM5         :1;                                       /* Acceptance Mask Bit 5 */
    byte AM6         :1;                                       /* Acceptance Mask Bit 6 */
    byte AM7         :1;                                       /* Acceptance Mask Bit 7 */
  } Bits;
} CANIDMR4STR;
extern volatile CANIDMR4STR _CANIDMR4 @0x0000189C;
#define CANIDMR4                        _CANIDMR4.Byte
#define CANIDMR4_AM0                    _CANIDMR4.Bits.AM0
#define CANIDMR4_AM1                    _CANIDMR4.Bits.AM1
#define CANIDMR4_AM2                    _CANIDMR4.Bits.AM2
#define CANIDMR4_AM3                    _CANIDMR4.Bits.AM3
#define CANIDMR4_AM4                    _CANIDMR4.Bits.AM4
#define CANIDMR4_AM5                    _CANIDMR4.Bits.AM5
#define CANIDMR4_AM6                    _CANIDMR4.Bits.AM6
#define CANIDMR4_AM7                    _CANIDMR4.Bits.AM7

#define CANIDMR4_AM0_MASK               1
#define CANIDMR4_AM1_MASK               2
#define CANIDMR4_AM2_MASK               4
#define CANIDMR4_AM3_MASK               8
#define CANIDMR4_AM4_MASK               16
#define CANIDMR4_AM5_MASK               32
#define CANIDMR4_AM6_MASK               64
#define CANIDMR4_AM7_MASK               128


/*** CANIDMR5 - MSCAN Identifier Mask Register 5; 0x0000189D ***/
typedef union {
  byte Byte;
  struct {
    byte AM0         :1;                                       /* Acceptance Mask Bit 0 */
    byte AM1         :1;                                       /* Acceptance Mask Bit 1 */
    byte AM2         :1;                                       /* Acceptance Mask Bit 2 */
    byte AM3         :1;                                       /* Acceptance Mask Bit 3 */
    byte AM4         :1;                                       /* Acceptance Mask Bit 4 */
    byte AM5         :1;                                       /* Acceptance Mask Bit 5 */
    byte AM6         :1;                                       /* Acceptance Mask Bit 6 */
    byte AM7         :1;                                       /* Acceptance Mask Bit 7 */
  } Bits;
} CANIDMR5STR;
extern volatile CANIDMR5STR _CANIDMR5 @0x0000189D;
#define CANIDMR5                        _CANIDMR5.Byte
#define CANIDMR5_AM0                    _CANIDMR5.Bits.AM0
#define CANIDMR5_AM1                    _CANIDMR5.Bits.AM1
#define CANIDMR5_AM2                    _CANIDMR5.Bits.AM2
#define CANIDMR5_AM3                    _CANIDMR5.Bits.AM3
#define CANIDMR5_AM4                    _CANIDMR5.Bits.AM4
#define CANIDMR5_AM5                    _CANIDMR5.Bits.AM5
#define CANIDMR5_AM6                    _CANIDMR5.Bits.AM6
#define CANIDMR5_AM7                    _CANIDMR5.Bits.AM7

#define CANIDMR5_AM0_MASK               1
#define CANIDMR5_AM1_MASK               2
#define CANIDMR5_AM2_MASK               4
#define CANIDMR5_AM3_MASK               8
#define CANIDMR5_AM4_MASK               16
#define CANIDMR5_AM5_MASK               32
#define CANIDMR5_AM6_MASK               64
#define CANIDMR5_AM7_MASK               128


/*** CANIDMR6 - MSCAN Identifier Mask Register 6; 0x0000189E ***/
typedef union {
  byte Byte;
  struct {
    byte AM0         :1;                                       /* Acceptance Mask Bit 0 */
    byte AM1         :1;                                       /* Acceptance Mask Bit 1 */
    byte AM2         :1;                                       /* Acceptance Mask Bit 2 */
    byte AM3         :1;                                       /* Acceptance Mask Bit 3 */
    byte AM4         :1;                                       /* Acceptance Mask Bit 4 */
    byte AM5         :1;                                       /* Acceptance Mask Bit 5 */
    byte AM6         :1;                                       /* Acceptance Mask Bit 6 */
    byte AM7         :1;                                       /* Acceptance Mask Bit 7 */
  } Bits;
} CANIDMR6STR;
extern volatile CANIDMR6STR _CANIDMR6 @0x0000189E;
#define CANIDMR6                        _CANIDMR6.Byte
#define CANIDMR6_AM0                    _CANIDMR6.Bits.AM0
#define CANIDMR6_AM1                    _CANIDMR6.Bits.AM1
#define CANIDMR6_AM2                    _CANIDMR6.Bits.AM2
#define CANIDMR6_AM3                    _CANIDMR6.Bits.AM3
#define CANIDMR6_AM4                    _CANIDMR6.Bits.AM4
#define CANIDMR6_AM5                    _CANIDMR6.Bits.AM5
#define CANIDMR6_AM6                    _CANIDMR6.Bits.AM6
#define CANIDMR6_AM7                    _CANIDMR6.Bits.AM7

#define CANIDMR6_AM0_MASK               1
#define CANIDMR6_AM1_MASK               2
#define CANIDMR6_AM2_MASK               4
#define CANIDMR6_AM3_MASK               8
#define CANIDMR6_AM4_MASK               16
#define CANIDMR6_AM5_MASK               32
#define CANIDMR6_AM6_MASK               64
#define CANIDMR6_AM7_MASK               128


/*** CANIDMR7 - MSCAN Identifier Mask Register 7; 0x0000189F ***/
typedef union {
  byte Byte;
  struct {
    byte AM0         :1;                                       /* Acceptance Mask Bit 0 */
    byte AM1         :1;                                       /* Acceptance Mask Bit 1 */
    byte AM2         :1;                                       /* Acceptance Mask Bit 2 */
    byte AM3         :1;                                       /* Acceptance Mask Bit 3 */
    byte AM4         :1;                                       /* Acceptance Mask Bit 4 */
    byte AM5         :1;                                       /* Acceptance Mask Bit 5 */
    byte AM6         :1;                                       /* Acceptance Mask Bit 6 */
    byte AM7         :1;                                       /* Acceptance Mask Bit 7 */
  } Bits;
} CANIDMR7STR;
extern volatile CANIDMR7STR _CANIDMR7 @0x0000189F;
#define CANIDMR7                        _CANIDMR7.Byte
#define CANIDMR7_AM0                    _CANIDMR7.Bits.AM0
#define CANIDMR7_AM1                    _CANIDMR7.Bits.AM1
#define CANIDMR7_AM2                    _CANIDMR7.Bits.AM2
#define CANIDMR7_AM3                    _CANIDMR7.Bits.AM3
#define CANIDMR7_AM4                    _CANIDMR7.Bits.AM4
#define CANIDMR7_AM5                    _CANIDMR7.Bits.AM5
#define CANIDMR7_AM6                    _CANIDMR7.Bits.AM6
#define CANIDMR7_AM7                    _CANIDMR7.Bits.AM7

#define CANIDMR7_AM0_MASK               1
#define CANIDMR7_AM1_MASK               2
#define CANIDMR7_AM2_MASK               4
#define CANIDMR7_AM3_MASK               8
#define CANIDMR7_AM4_MASK               16
#define CANIDMR7_AM5_MASK               32
#define CANIDMR7_AM6_MASK               64
#define CANIDMR7_AM7_MASK               128


/*** CANRIDR - MSCAN 0 Receive Identifier Register; 0x000018A0 ***/
typedef union {
  dword Dword;
   /* Overlapped registers: */
  struct {
    /*** CANRIDR0 - MSCAN 0 Receive Identifier Register 0; 0x000018A0 ***/
    union {
      byte Byte;
      struct {
        byte ID21        :1;                                       /* Extended format identifier Bit 21 */
        byte ID22        :1;                                       /* Extended format identifier Bit 22 */
        byte ID23        :1;                                       /* Extended format identifier Bit 23 */
        byte ID24        :1;                                       /* Extended format identifier Bit 24 */
        byte ID25        :1;                                       /* Extended format identifier Bit 25 */
        byte ID26        :1;                                       /* Extended format identifier Bit 26 */
        byte ID27        :1;                                       /* Extended format identifier Bit 27 */
        byte ID28        :1;                                       /* Extended format identifier Bit 28 */
      } Bits;
    } CANRIDR0STR;
    #define CANRIDR0                    _CANRIDR.Overlap_STR.CANRIDR0STR.Byte
    #define CANRIDR0_ID21               _CANRIDR.Overlap_STR.CANRIDR0STR.Bits.ID21
    #define CANRIDR0_ID22               _CANRIDR.Overlap_STR.CANRIDR0STR.Bits.ID22
    #define CANRIDR0_ID23               _CANRIDR.Overlap_STR.CANRIDR0STR.Bits.ID23
    #define CANRIDR0_ID24               _CANRIDR.Overlap_STR.CANRIDR0STR.Bits.ID24
    #define CANRIDR0_ID25               _CANRIDR.Overlap_STR.CANRIDR0STR.Bits.ID25
    #define CANRIDR0_ID26               _CANRIDR.Overlap_STR.CANRIDR0STR.Bits.ID26
    #define CANRIDR0_ID27               _CANRIDR.Overlap_STR.CANRIDR0STR.Bits.ID27
    #define CANRIDR0_ID28               _CANRIDR.Overlap_STR.CANRIDR0STR.Bits.ID28
    /* CANRIDR_ARR: Access 4 CANRIDRx registers in an array */
    #define CANRIDR_ARR                 ((volatile byte * __far) &CANRIDR0)
    
    #define CANRIDR0_ID21_MASK          1
    #define CANRIDR0_ID22_MASK          2
    #define CANRIDR0_ID23_MASK          4
    #define CANRIDR0_ID24_MASK          8
    #define CANRIDR0_ID25_MASK          16
    #define CANRIDR0_ID26_MASK          32
    #define CANRIDR0_ID27_MASK          64
    #define CANRIDR0_ID28_MASK          128
    

    /*** CANRIDR1 - MSCAN 0 Receive Identifier Register 1; 0x000018A1 ***/
    union {
      byte Byte;
      struct {
        byte ID15        :1;                                       /* Extended format identifier Bit 15 */
        byte ID16        :1;                                       /* Extended format identifier Bit 16 */
        byte ID17        :1;                                       /* Extended format identifier Bit 17 */
        byte IDE         :1;                                       /* ID Extended */
        byte SRR         :1;                                       /* Substitute Remote Request */
        byte ID18        :1;                                       /* Extended format identifier Bit 18 */
        byte ID19        :1;                                       /* Extended format identifier Bit 19 */
        byte ID20        :1;                                       /* Extended format identifier Bit 20 */
      } Bits;
      struct {
        byte grpID_15 :3;
        byte     :1;
        byte     :1;
        byte grpID_18 :3;
      } MergedBits;
    } CANRIDR1STR;
    #define CANRIDR1                    _CANRIDR.Overlap_STR.CANRIDR1STR.Byte
    #define CANRIDR1_ID15               _CANRIDR.Overlap_STR.CANRIDR1STR.Bits.ID15
    #define CANRIDR1_ID16               _CANRIDR.Overlap_STR.CANRIDR1STR.Bits.ID16
    #define CANRIDR1_ID17               _CANRIDR.Overlap_STR.CANRIDR1STR.Bits.ID17
    #define CANRIDR1_IDE                _CANRIDR.Overlap_STR.CANRIDR1STR.Bits.IDE
    #define CANRIDR1_SRR                _CANRIDR.Overlap_STR.CANRIDR1STR.Bits.SRR
    #define CANRIDR1_ID18               _CANRIDR.Overlap_STR.CANRIDR1STR.Bits.ID18
    #define CANRIDR1_ID19               _CANRIDR.Overlap_STR.CANRIDR1STR.Bits.ID19
    #define CANRIDR1_ID20               _CANRIDR.Overlap_STR.CANRIDR1STR.Bits.ID20
    #define CANRIDR1_ID_15              _CANRIDR.Overlap_STR.CANRIDR1STR.MergedBits.grpID_15
    #define CANRIDR1_ID_18              _CANRIDR.Overlap_STR.CANRIDR1STR.MergedBits.grpID_18
    #define CANRIDR1_ID                 CANRIDR1_ID_15
    
    #define CANRIDR1_ID15_MASK          1
    #define CANRIDR1_ID16_MASK          2
    #define CANRIDR1_ID17_MASK          4
    #define CANRIDR1_IDE_MASK           8
    #define CANRIDR1_SRR_MASK           16
    #define CANRIDR1_ID18_MASK          32
    #define CANRIDR1_ID19_MASK          64
    #define CANRIDR1_ID20_MASK          128
    #define CANRIDR1_ID_15_MASK         7
    #define CANRIDR1_ID_15_BITNUM       0
    #define CANRIDR1_ID_18_MASK         224
    #define CANRIDR1_ID_18_BITNUM       5
    

    /*** CANRIDR2 - MSCAN 0 Receive Identifier Register 2; 0x000018A2 ***/
    union {
      byte Byte;
      struct {
        byte ID7         :1;                                       /* Extended format identifier Bit 7 */
        byte ID8         :1;                                       /* Extended format identifier Bit 8 */
        byte ID9         :1;                                       /* Extended format identifier Bit 9 */
        byte ID10        :1;                                       /* Extended format identifier Bit 10 */
        byte ID11        :1;                                       /* Extended format identifier Bit 11 */
        byte ID12        :1;                                       /* Extended format identifier Bit 12 */
        byte ID13        :1;                                       /* Extended format identifier Bit 13 */
        byte ID14        :1;                                       /* Extended format identifier Bit 14 */
      } Bits;
    } CANRIDR2STR;
    #define CANRIDR2                    _CANRIDR.Overlap_STR.CANRIDR2STR.Byte
    #define CANRIDR2_ID7                _CANRIDR.Overlap_STR.CANRIDR2STR.Bits.ID7
    #define CANRIDR2_ID8                _CANRIDR.Overlap_STR.CANRIDR2STR.Bits.ID8
    #define CANRIDR2_ID9                _CANRIDR.Overlap_STR.CANRIDR2STR.Bits.ID9
    #define CANRIDR2_ID10               _CANRIDR.Overlap_STR.CANRIDR2STR.Bits.ID10
    #define CANRIDR2_ID11               _CANRIDR.Overlap_STR.CANRIDR2STR.Bits.ID11
    #define CANRIDR2_ID12               _CANRIDR.Overlap_STR.CANRIDR2STR.Bits.ID12
    #define CANRIDR2_ID13               _CANRIDR.Overlap_STR.CANRIDR2STR.Bits.ID13
    #define CANRIDR2_ID14               _CANRIDR.Overlap_STR.CANRIDR2STR.Bits.ID14
    
    #define CANRIDR2_ID7_MASK           1
    #define CANRIDR2_ID8_MASK           2
    #define CANRIDR2_ID9_MASK           4
    #define CANRIDR2_ID10_MASK          8
    #define CANRIDR2_ID11_MASK          16
    #define CANRIDR2_ID12_MASK          32
    #define CANRIDR2_ID13_MASK          64
    #define CANRIDR2_ID14_MASK          128
    

    /*** CANRIDR3 - MSCAN 0 Receive Identifier Register 3; 0x000018A3 ***/
    union {
      byte Byte;
      struct {
        byte RTR         :1;                                       /* Remote Transmission Request */
        byte ID0         :1;                                       /* Extended format identifier Bit 0 */
        byte ID1         :1;                                       /* Extended format identifier Bit 1 */
        byte ID2         :1;                                       /* Extended format identifier Bit 2 */
        byte ID3         :1;                                       /* Extended format identifier Bit 3 */
        byte ID4         :1;                                       /* Extended format identifier Bit 4 */
        byte ID5         :1;                                       /* Extended format identifier Bit 5 */
        byte ID6         :1;                                       /* Extended format identifier Bit 6 */
      } Bits;
      struct {
        byte     :1;
        byte grpID :7;
      } MergedBits;
    } CANRIDR3STR;
    #define CANRIDR3                    _CANRIDR.Overlap_STR.CANRIDR3STR.Byte
    #define CANRIDR3_RTR                _CANRIDR.Overlap_STR.CANRIDR3STR.Bits.RTR
    #define CANRIDR3_ID0                _CANRIDR.Overlap_STR.CANRIDR3STR.Bits.ID0
    #define CANRIDR3_ID1                _CANRIDR.Overlap_STR.CANRIDR3STR.Bits.ID1
    #define CANRIDR3_ID2                _CANRIDR.Overlap_STR.CANRIDR3STR.Bits.ID2
    #define CANRIDR3_ID3                _CANRIDR.Overlap_STR.CANRIDR3STR.Bits.ID3
    #define CANRIDR3_ID4                _CANRIDR.Overlap_STR.CANRIDR3STR.Bits.ID4
    #define CANRIDR3_ID5                _CANRIDR.Overlap_STR.CANRIDR3STR.Bits.ID5
    #define CANRIDR3_ID6                _CANRIDR.Overlap_STR.CANRIDR3STR.Bits.ID6
    #define CANRIDR3_ID                 _CANRIDR.Overlap_STR.CANRIDR3STR.MergedBits.grpID
    
    #define CANRIDR3_RTR_MASK           1
    #define CANRIDR3_ID0_MASK           2
    #define CANRIDR3_ID1_MASK           4
    #define CANRIDR3_ID2_MASK           8
    #define CANRIDR3_ID3_MASK           16
    #define CANRIDR3_ID4_MASK           32
    #define CANRIDR3_ID5_MASK           64
    #define CANRIDR3_ID6_MASK           128
    #define CANRIDR3_ID_MASK            254
    #define CANRIDR3_ID_BITNUM          1
    
  } Overlap_STR;

} CANRIDRSTR;
extern volatile CANRIDRSTR _CANRIDR @0x000018A0;
#define CANRIDR                         _CANRIDR.Dword


/*** CANRDSR0 - MSCAN Receive Data Segment Register 0; 0x000018A4 ***/
typedef union {
  byte Byte;
  struct {
    byte DB0         :1;                                       /* Data Bit 0 */
    byte DB1         :1;                                       /* Data Bit 1 */
    byte DB2         :1;                                       /* Data Bit 2 */
    byte DB3         :1;                                       /* Data Bit 3 */
    byte DB4         :1;                                       /* Data Bit 4 */
    byte DB5         :1;                                       /* Data Bit 5 */
    byte DB6         :1;                                       /* Data Bit 6 */
    byte DB7         :1;                                       /* Data Bit 7 */
  } Bits;
} CANRDSR0STR;
extern volatile CANRDSR0STR _CANRDSR0 @0x000018A4;
#define CANRDSR0                        _CANRDSR0.Byte
#define CANRDSR0_DB0                    _CANRDSR0.Bits.DB0
#define CANRDSR0_DB1                    _CANRDSR0.Bits.DB1
#define CANRDSR0_DB2                    _CANRDSR0.Bits.DB2
#define CANRDSR0_DB3                    _CANRDSR0.Bits.DB3
#define CANRDSR0_DB4                    _CANRDSR0.Bits.DB4
#define CANRDSR0_DB5                    _CANRDSR0.Bits.DB5
#define CANRDSR0_DB6                    _CANRDSR0.Bits.DB6
#define CANRDSR0_DB7                    _CANRDSR0.Bits.DB7
/* CANRDSR_ARR: Access 8 CANRDSRx registers in an array */
#define CANRDSR_ARR                     ((volatile byte * __far) &CANRDSR0)

#define CANRDSR0_DB0_MASK               1
#define CANRDSR0_DB1_MASK               2
#define CANRDSR0_DB2_MASK               4
#define CANRDSR0_DB3_MASK               8
#define CANRDSR0_DB4_MASK               16
#define CANRDSR0_DB5_MASK               32
#define CANRDSR0_DB6_MASK               64
#define CANRDSR0_DB7_MASK               128


/*** CANRDSR1 - MSCAN Receive Data Segment Register 1; 0x000018A5 ***/
typedef union {
  byte Byte;
  struct {
    byte DB0         :1;                                       /* Data Bit 0 */
    byte DB1         :1;                                       /* Data Bit 1 */
    byte DB2         :1;                                       /* Data Bit 2 */
    byte DB3         :1;                                       /* Data Bit 3 */
    byte DB4         :1;                                       /* Data Bit 4 */
    byte DB5         :1;                                       /* Data Bit 5 */
    byte DB6         :1;                                       /* Data Bit 6 */
    byte DB7         :1;                                       /* Data Bit 7 */
  } Bits;
} CANRDSR1STR;
extern volatile CANRDSR1STR _CANRDSR1 @0x000018A5;
#define CANRDSR1                        _CANRDSR1.Byte
#define CANRDSR1_DB0                    _CANRDSR1.Bits.DB0
#define CANRDSR1_DB1                    _CANRDSR1.Bits.DB1
#define CANRDSR1_DB2                    _CANRDSR1.Bits.DB2
#define CANRDSR1_DB3                    _CANRDSR1.Bits.DB3
#define CANRDSR1_DB4                    _CANRDSR1.Bits.DB4
#define CANRDSR1_DB5                    _CANRDSR1.Bits.DB5
#define CANRDSR1_DB6                    _CANRDSR1.Bits.DB6
#define CANRDSR1_DB7                    _CANRDSR1.Bits.DB7

#define CANRDSR1_DB0_MASK               1
#define CANRDSR1_DB1_MASK               2
#define CANRDSR1_DB2_MASK               4
#define CANRDSR1_DB3_MASK               8
#define CANRDSR1_DB4_MASK               16
#define CANRDSR1_DB5_MASK               32
#define CANRDSR1_DB6_MASK               64
#define CANRDSR1_DB7_MASK               128


/*** CANRDSR2 - MSCAN Receive Data Segment Register 2; 0x000018A6 ***/
typedef union {
  byte Byte;
  struct {
    byte DB0         :1;                                       /* Data Bit 0 */
    byte DB1         :1;                                       /* Data Bit 1 */
    byte DB2         :1;                                       /* Data Bit 2 */
    byte DB3         :1;                                       /* Data Bit 3 */
    byte DB4         :1;                                       /* Data Bit 4 */
    byte DB5         :1;                                       /* Data Bit 5 */
    byte DB6         :1;                                       /* Data Bit 6 */
    byte DB7         :1;                                       /* Data Bit 7 */
  } Bits;
} CANRDSR2STR;
extern volatile CANRDSR2STR _CANRDSR2 @0x000018A6;
#define CANRDSR2                        _CANRDSR2.Byte
#define CANRDSR2_DB0                    _CANRDSR2.Bits.DB0
#define CANRDSR2_DB1                    _CANRDSR2.Bits.DB1
#define CANRDSR2_DB2                    _CANRDSR2.Bits.DB2
#define CANRDSR2_DB3                    _CANRDSR2.Bits.DB3
#define CANRDSR2_DB4                    _CANRDSR2.Bits.DB4
#define CANRDSR2_DB5                    _CANRDSR2.Bits.DB5
#define CANRDSR2_DB6                    _CANRDSR2.Bits.DB6
#define CANRDSR2_DB7                    _CANRDSR2.Bits.DB7

#define CANRDSR2_DB0_MASK               1
#define CANRDSR2_DB1_MASK               2
#define CANRDSR2_DB2_MASK               4
#define CANRDSR2_DB3_MASK               8
#define CANRDSR2_DB4_MASK               16
#define CANRDSR2_DB5_MASK               32
#define CANRDSR2_DB6_MASK               64
#define CANRDSR2_DB7_MASK               128


/*** CANRDSR3 - MSCAN Receive Data Segment Register 3; 0x000018A7 ***/
typedef union {
  byte Byte;
  struct {
    byte DB0         :1;                                       /* Data Bit 0 */
    byte DB1         :1;                                       /* Data Bit 1 */
    byte DB2         :1;                                       /* Data Bit 2 */
    byte DB3         :1;                                       /* Data Bit 3 */
    byte DB4         :1;                                       /* Data Bit 4 */
    byte DB5         :1;                                       /* Data Bit 5 */
    byte DB6         :1;                                       /* Data Bit 6 */
    byte DB7         :1;                                       /* Data Bit 7 */
  } Bits;
} CANRDSR3STR;
extern volatile CANRDSR3STR _CANRDSR3 @0x000018A7;
#define CANRDSR3                        _CANRDSR3.Byte
#define CANRDSR3_DB0                    _CANRDSR3.Bits.DB0
#define CANRDSR3_DB1                    _CANRDSR3.Bits.DB1
#define CANRDSR3_DB2                    _CANRDSR3.Bits.DB2
#define CANRDSR3_DB3                    _CANRDSR3.Bits.DB3
#define CANRDSR3_DB4                    _CANRDSR3.Bits.DB4
#define CANRDSR3_DB5                    _CANRDSR3.Bits.DB5
#define CANRDSR3_DB6                    _CANRDSR3.Bits.DB6
#define CANRDSR3_DB7                    _CANRDSR3.Bits.DB7

#define CANRDSR3_DB0_MASK               1
#define CANRDSR3_DB1_MASK               2
#define CANRDSR3_DB2_MASK               4
#define CANRDSR3_DB3_MASK               8
#define CANRDSR3_DB4_MASK               16
#define CANRDSR3_DB5_MASK               32
#define CANRDSR3_DB6_MASK               64
#define CANRDSR3_DB7_MASK               128


/*** CANRDSR4 - MSCAN Receive Data Segment Register 4; 0x000018A8 ***/
typedef union {
  byte Byte;
  struct {
    byte DB0         :1;                                       /* Data Bit 0 */
    byte DB1         :1;                                       /* Data Bit 1 */
    byte DB2         :1;                                       /* Data Bit 2 */
    byte DB3         :1;                                       /* Data Bit 3 */
    byte DB4         :1;                                       /* Data Bit 4 */
    byte DB5         :1;                                       /* Data Bit 5 */
    byte DB6         :1;                                       /* Data Bit 6 */
    byte DB7         :1;                                       /* Data Bit 7 */
  } Bits;
} CANRDSR4STR;
extern volatile CANRDSR4STR _CANRDSR4 @0x000018A8;
#define CANRDSR4                        _CANRDSR4.Byte
#define CANRDSR4_DB0                    _CANRDSR4.Bits.DB0
#define CANRDSR4_DB1                    _CANRDSR4.Bits.DB1
#define CANRDSR4_DB2                    _CANRDSR4.Bits.DB2
#define CANRDSR4_DB3                    _CANRDSR4.Bits.DB3
#define CANRDSR4_DB4                    _CANRDSR4.Bits.DB4
#define CANRDSR4_DB5                    _CANRDSR4.Bits.DB5
#define CANRDSR4_DB6                    _CANRDSR4.Bits.DB6
#define CANRDSR4_DB7                    _CANRDSR4.Bits.DB7

#define CANRDSR4_DB0_MASK               1
#define CANRDSR4_DB1_MASK               2
#define CANRDSR4_DB2_MASK               4
#define CANRDSR4_DB3_MASK               8
#define CANRDSR4_DB4_MASK               16
#define CANRDSR4_DB5_MASK               32
#define CANRDSR4_DB6_MASK               64
#define CANRDSR4_DB7_MASK               128


/*** CANRDSR5 - MSCAN Receive Data Segment Register 5; 0x000018A9 ***/
typedef union {
  byte Byte;
  struct {
    byte DB0         :1;                                       /* Data Bit 0 */
    byte DB1         :1;                                       /* Data Bit 1 */
    byte DB2         :1;                                       /* Data Bit 2 */
    byte DB3         :1;                                       /* Data Bit 3 */
    byte DB4         :1;                                       /* Data Bit 4 */
    byte DB5         :1;                                       /* Data Bit 5 */
    byte DB6         :1;                                       /* Data Bit 6 */
    byte DB7         :1;                                       /* Data Bit 7 */
  } Bits;
} CANRDSR5STR;
extern volatile CANRDSR5STR _CANRDSR5 @0x000018A9;
#define CANRDSR5                        _CANRDSR5.Byte
#define CANRDSR5_DB0                    _CANRDSR5.Bits.DB0
#define CANRDSR5_DB1                    _CANRDSR5.Bits.DB1
#define CANRDSR5_DB2                    _CANRDSR5.Bits.DB2
#define CANRDSR5_DB3                    _CANRDSR5.Bits.DB3
#define CANRDSR5_DB4                    _CANRDSR5.Bits.DB4
#define CANRDSR5_DB5                    _CANRDSR5.Bits.DB5
#define CANRDSR5_DB6                    _CANRDSR5.Bits.DB6
#define CANRDSR5_DB7                    _CANRDSR5.Bits.DB7

#define CANRDSR5_DB0_MASK               1
#define CANRDSR5_DB1_MASK               2
#define CANRDSR5_DB2_MASK               4
#define CANRDSR5_DB3_MASK               8
#define CANRDSR5_DB4_MASK               16
#define CANRDSR5_DB5_MASK               32
#define CANRDSR5_DB6_MASK               64
#define CANRDSR5_DB7_MASK               128


/*** CANRDSR6 - MSCAN Receive Data Segment Register 6; 0x000018AA ***/
typedef union {
  byte Byte;
  struct {
    byte DB0         :1;                                       /* Data Bit 0 */
    byte DB1         :1;                                       /* Data Bit 1 */
    byte DB2         :1;                                       /* Data Bit 2 */
    byte DB3         :1;                                       /* Data Bit 3 */
    byte DB4         :1;                                       /* Data Bit 4 */
    byte DB5         :1;                                       /* Data Bit 5 */
    byte DB6         :1;                                       /* Data Bit 6 */
    byte DB7         :1;                                       /* Data Bit 7 */
  } Bits;
} CANRDSR6STR;
extern volatile CANRDSR6STR _CANRDSR6 @0x000018AA;
#define CANRDSR6                        _CANRDSR6.Byte
#define CANRDSR6_DB0                    _CANRDSR6.Bits.DB0
#define CANRDSR6_DB1                    _CANRDSR6.Bits.DB1
#define CANRDSR6_DB2                    _CANRDSR6.Bits.DB2
#define CANRDSR6_DB3                    _CANRDSR6.Bits.DB3
#define CANRDSR6_DB4                    _CANRDSR6.Bits.DB4
#define CANRDSR6_DB5                    _CANRDSR6.Bits.DB5
#define CANRDSR6_DB6                    _CANRDSR6.Bits.DB6
#define CANRDSR6_DB7                    _CANRDSR6.Bits.DB7

#define CANRDSR6_DB0_MASK               1
#define CANRDSR6_DB1_MASK               2
#define CANRDSR6_DB2_MASK               4
#define CANRDSR6_DB3_MASK               8
#define CANRDSR6_DB4_MASK               16
#define CANRDSR6_DB5_MASK               32
#define CANRDSR6_DB6_MASK               64
#define CANRDSR6_DB7_MASK               128


/*** CANRDSR7 - MSCAN Receive Data Segment Register 7; 0x000018AB ***/
typedef union {
  byte Byte;
  struct {
    byte DB0         :1;                                       /* Data Bit 0 */
    byte DB1         :1;                                       /* Data Bit 1 */
    byte DB2         :1;                                       /* Data Bit 2 */
    byte DB3         :1;                                       /* Data Bit 3 */
    byte DB4         :1;                                       /* Data Bit 4 */
    byte DB5         :1;                                       /* Data Bit 5 */
    byte DB6         :1;                                       /* Data Bit 6 */
    byte DB7         :1;                                       /* Data Bit 7 */
  } Bits;
} CANRDSR7STR;
extern volatile CANRDSR7STR _CANRDSR7 @0x000018AB;
#define CANRDSR7                        _CANRDSR7.Byte
#define CANRDSR7_DB0                    _CANRDSR7.Bits.DB0
#define CANRDSR7_DB1                    _CANRDSR7.Bits.DB1
#define CANRDSR7_DB2                    _CANRDSR7.Bits.DB2
#define CANRDSR7_DB3                    _CANRDSR7.Bits.DB3
#define CANRDSR7_DB4                    _CANRDSR7.Bits.DB4
#define CANRDSR7_DB5                    _CANRDSR7.Bits.DB5
#define CANRDSR7_DB6                    _CANRDSR7.Bits.DB6
#define CANRDSR7_DB7                    _CANRDSR7.Bits.DB7

#define CANRDSR7_DB0_MASK               1
#define CANRDSR7_DB1_MASK               2
#define CANRDSR7_DB2_MASK               4
#define CANRDSR7_DB3_MASK               8
#define CANRDSR7_DB4_MASK               16
#define CANRDSR7_DB5_MASK               32
#define CANRDSR7_DB6_MASK               64
#define CANRDSR7_DB7_MASK               128


/*** CANRDLR - MSCAN Receive Data Length Register; 0x000018AC ***/
typedef union {
  byte Byte;
  struct {
    byte DLC0        :1;                                       /* Data Length Code Bit 0 */
    byte DLC1        :1;                                       /* Data Length Code Bit 1 */
    byte DLC2        :1;                                       /* Data Length Code Bit 2 */
    byte DLC3        :1;                                       /* Data Length Code Bit 3 */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
  } Bits;
  struct {
    byte grpDLC  :4;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} CANRDLRSTR;
extern volatile CANRDLRSTR _CANRDLR @0x000018AC;
#define CANRDLR                         _CANRDLR.Byte
#define CANRDLR_DLC0                    _CANRDLR.Bits.DLC0
#define CANRDLR_DLC1                    _CANRDLR.Bits.DLC1
#define CANRDLR_DLC2                    _CANRDLR.Bits.DLC2
#define CANRDLR_DLC3                    _CANRDLR.Bits.DLC3
#define CANRDLR_DLC                     _CANRDLR.MergedBits.grpDLC

#define CANRDLR_DLC0_MASK               1
#define CANRDLR_DLC1_MASK               2
#define CANRDLR_DLC2_MASK               4
#define CANRDLR_DLC3_MASK               8
#define CANRDLR_DLC_MASK                15
#define CANRDLR_DLC_BITNUM              0


/*** CANRTSR - MSCAN Receive Time Stamp Register; 0x000018AE ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** CANRTSRH - MSCAN Receive Time Stamp Register High; 0x000018AE ***/
    union {
      byte Byte;
      struct {
        byte TSR8        :1;                                       /* Time Stamp Bit 8 */
        byte TSR9        :1;                                       /* Time Stamp Bit 9 */
        byte TSR10       :1;                                       /* Time Stamp Bit 10 */
        byte TSR11       :1;                                       /* Time Stamp Bit 11 */
        byte TSR12       :1;                                       /* Time Stamp Bit 12 */
        byte TSR13       :1;                                       /* Time Stamp Bit 13 */
        byte TSR14       :1;                                       /* Time Stamp Bit 14 */
        byte TSR15       :1;                                       /* Time Stamp Bit 15 */
      } Bits;
    } CANRTSRHSTR;
    #define CANRTSRH                    _CANRTSR.Overlap_STR.CANRTSRHSTR.Byte
    #define CANRTSRH_TSR8               _CANRTSR.Overlap_STR.CANRTSRHSTR.Bits.TSR8
    #define CANRTSRH_TSR9               _CANRTSR.Overlap_STR.CANRTSRHSTR.Bits.TSR9
    #define CANRTSRH_TSR10              _CANRTSR.Overlap_STR.CANRTSRHSTR.Bits.TSR10
    #define CANRTSRH_TSR11              _CANRTSR.Overlap_STR.CANRTSRHSTR.Bits.TSR11
    #define CANRTSRH_TSR12              _CANRTSR.Overlap_STR.CANRTSRHSTR.Bits.TSR12
    #define CANRTSRH_TSR13              _CANRTSR.Overlap_STR.CANRTSRHSTR.Bits.TSR13
    #define CANRTSRH_TSR14              _CANRTSR.Overlap_STR.CANRTSRHSTR.Bits.TSR14
    #define CANRTSRH_TSR15              _CANRTSR.Overlap_STR.CANRTSRHSTR.Bits.TSR15
    
    #define CANRTSRH_TSR8_MASK          1
    #define CANRTSRH_TSR9_MASK          2
    #define CANRTSRH_TSR10_MASK         4
    #define CANRTSRH_TSR11_MASK         8
    #define CANRTSRH_TSR12_MASK         16
    #define CANRTSRH_TSR13_MASK         32
    #define CANRTSRH_TSR14_MASK         64
    #define CANRTSRH_TSR15_MASK         128
    

    /*** CANRTSRL - MSCAN Receive Time Stamp Register Low; 0x000018AF ***/
    union {
      byte Byte;
      struct {
        byte TSR0        :1;                                       /* Time Stamp Bit 0 */
        byte TSR1        :1;                                       /* Time Stamp Bit 1 */
        byte TSR2        :1;                                       /* Time Stamp Bit 2 */
        byte TSR3        :1;                                       /* Time Stamp Bit 3 */
        byte TSR4        :1;                                       /* Time Stamp Bit 4 */
        byte TSR5        :1;                                       /* Time Stamp Bit 5 */
        byte TSR6        :1;                                       /* Time Stamp Bit 6 */
        byte TSR7        :1;                                       /* Time Stamp Bit 7 */
      } Bits;
    } CANRTSRLSTR;
    #define CANRTSRL                    _CANRTSR.Overlap_STR.CANRTSRLSTR.Byte
    #define CANRTSRL_TSR0               _CANRTSR.Overlap_STR.CANRTSRLSTR.Bits.TSR0
    #define CANRTSRL_TSR1               _CANRTSR.Overlap_STR.CANRTSRLSTR.Bits.TSR1
    #define CANRTSRL_TSR2               _CANRTSR.Overlap_STR.CANRTSRLSTR.Bits.TSR2
    #define CANRTSRL_TSR3               _CANRTSR.Overlap_STR.CANRTSRLSTR.Bits.TSR3
    #define CANRTSRL_TSR4               _CANRTSR.Overlap_STR.CANRTSRLSTR.Bits.TSR4
    #define CANRTSRL_TSR5               _CANRTSR.Overlap_STR.CANRTSRLSTR.Bits.TSR5
    #define CANRTSRL_TSR6               _CANRTSR.Overlap_STR.CANRTSRLSTR.Bits.TSR6
    #define CANRTSRL_TSR7               _CANRTSR.Overlap_STR.CANRTSRLSTR.Bits.TSR7
    
    #define CANRTSRL_TSR0_MASK          1
    #define CANRTSRL_TSR1_MASK          2
    #define CANRTSRL_TSR2_MASK          4
    #define CANRTSRL_TSR3_MASK          8
    #define CANRTSRL_TSR4_MASK          16
    #define CANRTSRL_TSR5_MASK          32
    #define CANRTSRL_TSR6_MASK          64
    #define CANRTSRL_TSR7_MASK          128
    
  } Overlap_STR;

} CANRTSRSTR;
extern volatile CANRTSRSTR _CANRTSR @0x000018AE;
#define CANRTSR                         _CANRTSR.Word


/*** CANTIDR - MSCAN 0 Transmit Identifier Register; 0x000018B0 ***/
typedef union {
  dword Dword;
   /* Overlapped registers: */
  struct {
    /*** CANTIDR0 - MSCAN 0 Transmit Identifier Register 0; 0x000018B0 ***/
    union {
      byte Byte;
      struct {
        byte ID21        :1;                                       /* Extended format identifier Bit 21 */
        byte ID22        :1;                                       /* Extended format identifier Bit 22 */
        byte ID23        :1;                                       /* Extended format identifier Bit 23 */
        byte ID24        :1;                                       /* Extended format identifier Bit 24 */
        byte ID25        :1;                                       /* Extended format identifier Bit 25 */
        byte ID26        :1;                                       /* Extended format identifier Bit 26 */
        byte ID27        :1;                                       /* Extended format identifier Bit 27 */
        byte ID28        :1;                                       /* Extended format identifier Bit 28 */
      } Bits;
    } CANTIDR0STR;
    #define CANTIDR0                    _CANTIDR.Overlap_STR.CANTIDR0STR.Byte
    #define CANTIDR0_ID21               _CANTIDR.Overlap_STR.CANTIDR0STR.Bits.ID21
    #define CANTIDR0_ID22               _CANTIDR.Overlap_STR.CANTIDR0STR.Bits.ID22
    #define CANTIDR0_ID23               _CANTIDR.Overlap_STR.CANTIDR0STR.Bits.ID23
    #define CANTIDR0_ID24               _CANTIDR.Overlap_STR.CANTIDR0STR.Bits.ID24
    #define CANTIDR0_ID25               _CANTIDR.Overlap_STR.CANTIDR0STR.Bits.ID25
    #define CANTIDR0_ID26               _CANTIDR.Overlap_STR.CANTIDR0STR.Bits.ID26
    #define CANTIDR0_ID27               _CANTIDR.Overlap_STR.CANTIDR0STR.Bits.ID27
    #define CANTIDR0_ID28               _CANTIDR.Overlap_STR.CANTIDR0STR.Bits.ID28
    /* CANTIDR_ARR: Access 4 CANTIDRx registers in an array */
    #define CANTIDR_ARR                 ((volatile byte * __far) &CANTIDR0)
    
    #define CANTIDR0_ID21_MASK          1
    #define CANTIDR0_ID22_MASK          2
    #define CANTIDR0_ID23_MASK          4
    #define CANTIDR0_ID24_MASK          8
    #define CANTIDR0_ID25_MASK          16
    #define CANTIDR0_ID26_MASK          32
    #define CANTIDR0_ID27_MASK          64
    #define CANTIDR0_ID28_MASK          128
    

    /*** CANTIDR1 - MSCAN 0 Transmit Identifier Register 1; 0x000018B1 ***/
    union {
      byte Byte;
      struct {
        byte ID15        :1;                                       /* Extended format identifier Bit 15 */
        byte ID16        :1;                                       /* Extended format identifier Bit 16 */
        byte ID17        :1;                                       /* Extended format identifier Bit 17 */
        byte IDE         :1;                                       /* ID Extended */
        byte SRR         :1;                                       /* Substitute Remote Request */
        byte ID18        :1;                                       /* Extended format identifier Bit 18 */
        byte ID19        :1;                                       /* Extended format identifier Bit 19 */
        byte ID20        :1;                                       /* Extended format identifier Bit 20 */
      } Bits;
      struct {
        byte grpID_15 :3;
        byte     :1;
        byte     :1;
        byte grpID_18 :3;
      } MergedBits;
    } CANTIDR1STR;
    #define CANTIDR1                    _CANTIDR.Overlap_STR.CANTIDR1STR.Byte
    #define CANTIDR1_ID15               _CANTIDR.Overlap_STR.CANTIDR1STR.Bits.ID15
    #define CANTIDR1_ID16               _CANTIDR.Overlap_STR.CANTIDR1STR.Bits.ID16
    #define CANTIDR1_ID17               _CANTIDR.Overlap_STR.CANTIDR1STR.Bits.ID17
    #define CANTIDR1_IDE                _CANTIDR.Overlap_STR.CANTIDR1STR.Bits.IDE
    #define CANTIDR1_SRR                _CANTIDR.Overlap_STR.CANTIDR1STR.Bits.SRR
    #define CANTIDR1_ID18               _CANTIDR.Overlap_STR.CANTIDR1STR.Bits.ID18
    #define CANTIDR1_ID19               _CANTIDR.Overlap_STR.CANTIDR1STR.Bits.ID19
    #define CANTIDR1_ID20               _CANTIDR.Overlap_STR.CANTIDR1STR.Bits.ID20
    #define CANTIDR1_ID_15              _CANTIDR.Overlap_STR.CANTIDR1STR.MergedBits.grpID_15
    #define CANTIDR1_ID_18              _CANTIDR.Overlap_STR.CANTIDR1STR.MergedBits.grpID_18
    #define CANTIDR1_ID                 CANTIDR1_ID_15
    
    #define CANTIDR1_ID15_MASK          1
    #define CANTIDR1_ID16_MASK          2
    #define CANTIDR1_ID17_MASK          4
    #define CANTIDR1_IDE_MASK           8
    #define CANTIDR1_SRR_MASK           16
    #define CANTIDR1_ID18_MASK          32
    #define CANTIDR1_ID19_MASK          64
    #define CANTIDR1_ID20_MASK          128
    #define CANTIDR1_ID_15_MASK         7
    #define CANTIDR1_ID_15_BITNUM       0
    #define CANTIDR1_ID_18_MASK         224
    #define CANTIDR1_ID_18_BITNUM       5
    

    /*** CANTIDR2 - MSCAN 0 Transmit Identifier Register 2; 0x000018B2 ***/
    union {
      byte Byte;
      struct {
        byte ID7         :1;                                       /* Extended format identifier Bit 7 */
        byte ID8         :1;                                       /* Extended format identifier Bit 8 */
        byte ID9         :1;                                       /* Extended format identifier Bit 9 */
        byte ID10        :1;                                       /* Extended format identifier Bit 10 */
        byte ID11        :1;                                       /* Extended format identifier Bit 11 */
        byte ID12        :1;                                       /* Extended format identifier Bit 12 */
        byte ID13        :1;                                       /* Extended format identifier Bit 13 */
        byte ID14        :1;                                       /* Extended format identifier Bit 14 */
      } Bits;
    } CANTIDR2STR;
    #define CANTIDR2                    _CANTIDR.Overlap_STR.CANTIDR2STR.Byte
    #define CANTIDR2_ID7                _CANTIDR.Overlap_STR.CANTIDR2STR.Bits.ID7
    #define CANTIDR2_ID8                _CANTIDR.Overlap_STR.CANTIDR2STR.Bits.ID8
    #define CANTIDR2_ID9                _CANTIDR.Overlap_STR.CANTIDR2STR.Bits.ID9
    #define CANTIDR2_ID10               _CANTIDR.Overlap_STR.CANTIDR2STR.Bits.ID10
    #define CANTIDR2_ID11               _CANTIDR.Overlap_STR.CANTIDR2STR.Bits.ID11
    #define CANTIDR2_ID12               _CANTIDR.Overlap_STR.CANTIDR2STR.Bits.ID12
    #define CANTIDR2_ID13               _CANTIDR.Overlap_STR.CANTIDR2STR.Bits.ID13
    #define CANTIDR2_ID14               _CANTIDR.Overlap_STR.CANTIDR2STR.Bits.ID14
    
    #define CANTIDR2_ID7_MASK           1
    #define CANTIDR2_ID8_MASK           2
    #define CANTIDR2_ID9_MASK           4
    #define CANTIDR2_ID10_MASK          8
    #define CANTIDR2_ID11_MASK          16
    #define CANTIDR2_ID12_MASK          32
    #define CANTIDR2_ID13_MASK          64
    #define CANTIDR2_ID14_MASK          128
    

    /*** CANTIDR3 - MSCAN 0 Transmit Identifier Register 3; 0x000018B3 ***/
    union {
      byte Byte;
      struct {
        byte RTR         :1;                                       /* Remote Transmission Request */
        byte ID0         :1;                                       /* Extended format identifier Bit 0 */
        byte ID1         :1;                                       /* Extended format identifier Bit 1 */
        byte ID2         :1;                                       /* Extended format identifier Bit 2 */
        byte ID3         :1;                                       /* Extended format identifier Bit 3 */
        byte ID4         :1;                                       /* Extended format identifier Bit 4 */
        byte ID5         :1;                                       /* Extended format identifier Bit 5 */
        byte ID6         :1;                                       /* Extended format identifier Bit 6 */
      } Bits;
      struct {
        byte     :1;
        byte grpID :7;
      } MergedBits;
    } CANTIDR3STR;
    #define CANTIDR3                    _CANTIDR.Overlap_STR.CANTIDR3STR.Byte
    #define CANTIDR3_RTR                _CANTIDR.Overlap_STR.CANTIDR3STR.Bits.RTR
    #define CANTIDR3_ID0                _CANTIDR.Overlap_STR.CANTIDR3STR.Bits.ID0
    #define CANTIDR3_ID1                _CANTIDR.Overlap_STR.CANTIDR3STR.Bits.ID1
    #define CANTIDR3_ID2                _CANTIDR.Overlap_STR.CANTIDR3STR.Bits.ID2
    #define CANTIDR3_ID3                _CANTIDR.Overlap_STR.CANTIDR3STR.Bits.ID3
    #define CANTIDR3_ID4                _CANTIDR.Overlap_STR.CANTIDR3STR.Bits.ID4
    #define CANTIDR3_ID5                _CANTIDR.Overlap_STR.CANTIDR3STR.Bits.ID5
    #define CANTIDR3_ID6                _CANTIDR.Overlap_STR.CANTIDR3STR.Bits.ID6
    #define CANTIDR3_ID                 _CANTIDR.Overlap_STR.CANTIDR3STR.MergedBits.grpID
    
    #define CANTIDR3_RTR_MASK           1
    #define CANTIDR3_ID0_MASK           2
    #define CANTIDR3_ID1_MASK           4
    #define CANTIDR3_ID2_MASK           8
    #define CANTIDR3_ID3_MASK           16
    #define CANTIDR3_ID4_MASK           32
    #define CANTIDR3_ID5_MASK           64
    #define CANTIDR3_ID6_MASK           128
    #define CANTIDR3_ID_MASK            254
    #define CANTIDR3_ID_BITNUM          1
    
  } Overlap_STR;

} CANTIDRSTR;
extern volatile CANTIDRSTR _CANTIDR @0x000018B0;
#define CANTIDR                         _CANTIDR.Dword


/*** CANTDSR0 - MSCAN Transmit Data Segment Register 0; 0x000018B4 ***/
typedef union {
  byte Byte;
  struct {
    byte DB0         :1;                                       /* Data Bit 0 */
    byte DB1         :1;                                       /* Data Bit 1 */
    byte DB2         :1;                                       /* Data Bit 2 */
    byte DB3         :1;                                       /* Data Bit 3 */
    byte DB4         :1;                                       /* Data Bit 4 */
    byte DB5         :1;                                       /* Data Bit 5 */
    byte DB6         :1;                                       /* Data Bit 6 */
    byte DB7         :1;                                       /* Data Bit 7 */
  } Bits;
} CANTDSR0STR;
extern volatile CANTDSR0STR _CANTDSR0 @0x000018B4;
#define CANTDSR0                        _CANTDSR0.Byte
#define CANTDSR0_DB0                    _CANTDSR0.Bits.DB0
#define CANTDSR0_DB1                    _CANTDSR0.Bits.DB1
#define CANTDSR0_DB2                    _CANTDSR0.Bits.DB2
#define CANTDSR0_DB3                    _CANTDSR0.Bits.DB3
#define CANTDSR0_DB4                    _CANTDSR0.Bits.DB4
#define CANTDSR0_DB5                    _CANTDSR0.Bits.DB5
#define CANTDSR0_DB6                    _CANTDSR0.Bits.DB6
#define CANTDSR0_DB7                    _CANTDSR0.Bits.DB7
/* CANTDSR_ARR: Access 8 CANTDSRx registers in an array */
#define CANTDSR_ARR                     ((volatile byte * __far) &CANTDSR0)

#define CANTDSR0_DB0_MASK               1
#define CANTDSR0_DB1_MASK               2
#define CANTDSR0_DB2_MASK               4
#define CANTDSR0_DB3_MASK               8
#define CANTDSR0_DB4_MASK               16
#define CANTDSR0_DB5_MASK               32
#define CANTDSR0_DB6_MASK               64
#define CANTDSR0_DB7_MASK               128


/*** CANTDSR1 - MSCAN Transmit Data Segment Register 1; 0x000018B5 ***/
typedef union {
  byte Byte;
  struct {
    byte DB0         :1;                                       /* Data Bit 0 */
    byte DB1         :1;                                       /* Data Bit 1 */
    byte DB2         :1;                                       /* Data Bit 2 */
    byte DB3         :1;                                       /* Data Bit 3 */
    byte DB4         :1;                                       /* Data Bit 4 */
    byte DB5         :1;                                       /* Data Bit 5 */
    byte DB6         :1;                                       /* Data Bit 6 */
    byte DB7         :1;                                       /* Data Bit 7 */
  } Bits;
} CANTDSR1STR;
extern volatile CANTDSR1STR _CANTDSR1 @0x000018B5;
#define CANTDSR1                        _CANTDSR1.Byte
#define CANTDSR1_DB0                    _CANTDSR1.Bits.DB0
#define CANTDSR1_DB1                    _CANTDSR1.Bits.DB1
#define CANTDSR1_DB2                    _CANTDSR1.Bits.DB2
#define CANTDSR1_DB3                    _CANTDSR1.Bits.DB3
#define CANTDSR1_DB4                    _CANTDSR1.Bits.DB4
#define CANTDSR1_DB5                    _CANTDSR1.Bits.DB5
#define CANTDSR1_DB6                    _CANTDSR1.Bits.DB6
#define CANTDSR1_DB7                    _CANTDSR1.Bits.DB7

#define CANTDSR1_DB0_MASK               1
#define CANTDSR1_DB1_MASK               2
#define CANTDSR1_DB2_MASK               4
#define CANTDSR1_DB3_MASK               8
#define CANTDSR1_DB4_MASK               16
#define CANTDSR1_DB5_MASK               32
#define CANTDSR1_DB6_MASK               64
#define CANTDSR1_DB7_MASK               128


/*** CANTDSR2 - MSCAN Transmit Data Segment Register 2; 0x000018B6 ***/
typedef union {
  byte Byte;
  struct {
    byte DB0         :1;                                       /* Data Bit 0 */
    byte DB1         :1;                                       /* Data Bit 1 */
    byte DB2         :1;                                       /* Data Bit 2 */
    byte DB3         :1;                                       /* Data Bit 3 */
    byte DB4         :1;                                       /* Data Bit 4 */
    byte DB5         :1;                                       /* Data Bit 5 */
    byte DB6         :1;                                       /* Data Bit 6 */
    byte DB7         :1;                                       /* Data Bit 7 */
  } Bits;
} CANTDSR2STR;
extern volatile CANTDSR2STR _CANTDSR2 @0x000018B6;
#define CANTDSR2                        _CANTDSR2.Byte
#define CANTDSR2_DB0                    _CANTDSR2.Bits.DB0
#define CANTDSR2_DB1                    _CANTDSR2.Bits.DB1
#define CANTDSR2_DB2                    _CANTDSR2.Bits.DB2
#define CANTDSR2_DB3                    _CANTDSR2.Bits.DB3
#define CANTDSR2_DB4                    _CANTDSR2.Bits.DB4
#define CANTDSR2_DB5                    _CANTDSR2.Bits.DB5
#define CANTDSR2_DB6                    _CANTDSR2.Bits.DB6
#define CANTDSR2_DB7                    _CANTDSR2.Bits.DB7

#define CANTDSR2_DB0_MASK               1
#define CANTDSR2_DB1_MASK               2
#define CANTDSR2_DB2_MASK               4
#define CANTDSR2_DB3_MASK               8
#define CANTDSR2_DB4_MASK               16
#define CANTDSR2_DB5_MASK               32
#define CANTDSR2_DB6_MASK               64
#define CANTDSR2_DB7_MASK               128


/*** CANTDSR3 - MSCAN Transmit Data Segment Register 3; 0x000018B7 ***/
typedef union {
  byte Byte;
  struct {
    byte DB0         :1;                                       /* Data Bit 0 */
    byte DB1         :1;                                       /* Data Bit 1 */
    byte DB2         :1;                                       /* Data Bit 2 */
    byte DB3         :1;                                       /* Data Bit 3 */
    byte DB4         :1;                                       /* Data Bit 4 */
    byte DB5         :1;                                       /* Data Bit 5 */
    byte DB6         :1;                                       /* Data Bit 6 */
    byte DB7         :1;                                       /* Data Bit 7 */
  } Bits;
} CANTDSR3STR;
extern volatile CANTDSR3STR _CANTDSR3 @0x000018B7;
#define CANTDSR3                        _CANTDSR3.Byte
#define CANTDSR3_DB0                    _CANTDSR3.Bits.DB0
#define CANTDSR3_DB1                    _CANTDSR3.Bits.DB1
#define CANTDSR3_DB2                    _CANTDSR3.Bits.DB2
#define CANTDSR3_DB3                    _CANTDSR3.Bits.DB3
#define CANTDSR3_DB4                    _CANTDSR3.Bits.DB4
#define CANTDSR3_DB5                    _CANTDSR3.Bits.DB5
#define CANTDSR3_DB6                    _CANTDSR3.Bits.DB6
#define CANTDSR3_DB7                    _CANTDSR3.Bits.DB7

#define CANTDSR3_DB0_MASK               1
#define CANTDSR3_DB1_MASK               2
#define CANTDSR3_DB2_MASK               4
#define CANTDSR3_DB3_MASK               8
#define CANTDSR3_DB4_MASK               16
#define CANTDSR3_DB5_MASK               32
#define CANTDSR3_DB6_MASK               64
#define CANTDSR3_DB7_MASK               128


/*** CANTDSR4 - MSCAN Transmit Data Segment Register 4; 0x000018B8 ***/
typedef union {
  byte Byte;
  struct {
    byte DB0         :1;                                       /* Data Bit 0 */
    byte DB1         :1;                                       /* Data Bit 1 */
    byte DB2         :1;                                       /* Data Bit 2 */
    byte DB3         :1;                                       /* Data Bit 3 */
    byte DB4         :1;                                       /* Data Bit 4 */
    byte DB5         :1;                                       /* Data Bit 5 */
    byte DB6         :1;                                       /* Data Bit 6 */
    byte DB7         :1;                                       /* Data Bit 7 */
  } Bits;
} CANTDSR4STR;
extern volatile CANTDSR4STR _CANTDSR4 @0x000018B8;
#define CANTDSR4                        _CANTDSR4.Byte
#define CANTDSR4_DB0                    _CANTDSR4.Bits.DB0
#define CANTDSR4_DB1                    _CANTDSR4.Bits.DB1
#define CANTDSR4_DB2                    _CANTDSR4.Bits.DB2
#define CANTDSR4_DB3                    _CANTDSR4.Bits.DB3
#define CANTDSR4_DB4                    _CANTDSR4.Bits.DB4
#define CANTDSR4_DB5                    _CANTDSR4.Bits.DB5
#define CANTDSR4_DB6                    _CANTDSR4.Bits.DB6
#define CANTDSR4_DB7                    _CANTDSR4.Bits.DB7

#define CANTDSR4_DB0_MASK               1
#define CANTDSR4_DB1_MASK               2
#define CANTDSR4_DB2_MASK               4
#define CANTDSR4_DB3_MASK               8
#define CANTDSR4_DB4_MASK               16
#define CANTDSR4_DB5_MASK               32
#define CANTDSR4_DB6_MASK               64
#define CANTDSR4_DB7_MASK               128


/*** CANTDSR5 - MSCAN Transmit Data Segment Register 5; 0x000018B9 ***/
typedef union {
  byte Byte;
  struct {
    byte DB0         :1;                                       /* Data Bit 0 */
    byte DB1         :1;                                       /* Data Bit 1 */
    byte DB2         :1;                                       /* Data Bit 2 */
    byte DB3         :1;                                       /* Data Bit 3 */
    byte DB4         :1;                                       /* Data Bit 4 */
    byte DB5         :1;                                       /* Data Bit 5 */
    byte DB6         :1;                                       /* Data Bit 6 */
    byte DB7         :1;                                       /* Data Bit 7 */
  } Bits;
} CANTDSR5STR;
extern volatile CANTDSR5STR _CANTDSR5 @0x000018B9;
#define CANTDSR5                        _CANTDSR5.Byte
#define CANTDSR5_DB0                    _CANTDSR5.Bits.DB0
#define CANTDSR5_DB1                    _CANTDSR5.Bits.DB1
#define CANTDSR5_DB2                    _CANTDSR5.Bits.DB2
#define CANTDSR5_DB3                    _CANTDSR5.Bits.DB3
#define CANTDSR5_DB4                    _CANTDSR5.Bits.DB4
#define CANTDSR5_DB5                    _CANTDSR5.Bits.DB5
#define CANTDSR5_DB6                    _CANTDSR5.Bits.DB6
#define CANTDSR5_DB7                    _CANTDSR5.Bits.DB7

#define CANTDSR5_DB0_MASK               1
#define CANTDSR5_DB1_MASK               2
#define CANTDSR5_DB2_MASK               4
#define CANTDSR5_DB3_MASK               8
#define CANTDSR5_DB4_MASK               16
#define CANTDSR5_DB5_MASK               32
#define CANTDSR5_DB6_MASK               64
#define CANTDSR5_DB7_MASK               128


/*** CANTDSR6 - MSCAN Transmit Data Segment Register 6; 0x000018BA ***/
typedef union {
  byte Byte;
  struct {
    byte DB0         :1;                                       /* Data Bit 0 */
    byte DB1         :1;                                       /* Data Bit 1 */
    byte DB2         :1;                                       /* Data Bit 2 */
    byte DB3         :1;                                       /* Data Bit 3 */
    byte DB4         :1;                                       /* Data Bit 4 */
    byte DB5         :1;                                       /* Data Bit 5 */
    byte DB6         :1;                                       /* Data Bit 6 */
    byte DB7         :1;                                       /* Data Bit 7 */
  } Bits;
} CANTDSR6STR;
extern volatile CANTDSR6STR _CANTDSR6 @0x000018BA;
#define CANTDSR6                        _CANTDSR6.Byte
#define CANTDSR6_DB0                    _CANTDSR6.Bits.DB0
#define CANTDSR6_DB1                    _CANTDSR6.Bits.DB1
#define CANTDSR6_DB2                    _CANTDSR6.Bits.DB2
#define CANTDSR6_DB3                    _CANTDSR6.Bits.DB3
#define CANTDSR6_DB4                    _CANTDSR6.Bits.DB4
#define CANTDSR6_DB5                    _CANTDSR6.Bits.DB5
#define CANTDSR6_DB6                    _CANTDSR6.Bits.DB6
#define CANTDSR6_DB7                    _CANTDSR6.Bits.DB7

#define CANTDSR6_DB0_MASK               1
#define CANTDSR6_DB1_MASK               2
#define CANTDSR6_DB2_MASK               4
#define CANTDSR6_DB3_MASK               8
#define CANTDSR6_DB4_MASK               16
#define CANTDSR6_DB5_MASK               32
#define CANTDSR6_DB6_MASK               64
#define CANTDSR6_DB7_MASK               128


/*** CANTDSR7 - MSCAN Transmit Data Segment Register 7; 0x000018BB ***/
typedef union {
  byte Byte;
  struct {
    byte DB0         :1;                                       /* Data Bit 0 */
    byte DB1         :1;                                       /* Data Bit 1 */
    byte DB2         :1;                                       /* Data Bit 2 */
    byte DB3         :1;                                       /* Data Bit 3 */
    byte DB4         :1;                                       /* Data Bit 4 */
    byte DB5         :1;                                       /* Data Bit 5 */
    byte DB6         :1;                                       /* Data Bit 6 */
    byte DB7         :1;                                       /* Data Bit 7 */
  } Bits;
} CANTDSR7STR;
extern volatile CANTDSR7STR _CANTDSR7 @0x000018BB;
#define CANTDSR7                        _CANTDSR7.Byte
#define CANTDSR7_DB0                    _CANTDSR7.Bits.DB0
#define CANTDSR7_DB1                    _CANTDSR7.Bits.DB1
#define CANTDSR7_DB2                    _CANTDSR7.Bits.DB2
#define CANTDSR7_DB3                    _CANTDSR7.Bits.DB3
#define CANTDSR7_DB4                    _CANTDSR7.Bits.DB4
#define CANTDSR7_DB5                    _CANTDSR7.Bits.DB5
#define CANTDSR7_DB6                    _CANTDSR7.Bits.DB6
#define CANTDSR7_DB7                    _CANTDSR7.Bits.DB7

#define CANTDSR7_DB0_MASK               1
#define CANTDSR7_DB1_MASK               2
#define CANTDSR7_DB2_MASK               4
#define CANTDSR7_DB3_MASK               8
#define CANTDSR7_DB4_MASK               16
#define CANTDSR7_DB5_MASK               32
#define CANTDSR7_DB6_MASK               64
#define CANTDSR7_DB7_MASK               128


/*** CANTDLR - MSCAN Transmit Data Length Register; 0x000018BC ***/
typedef union {
  byte Byte;
  struct {
    byte DLC0        :1;                                       /* Data Length Code Bit 0 */
    byte DLC1        :1;                                       /* Data Length Code Bit 1 */
    byte DLC2        :1;                                       /* Data Length Code Bit 2 */
    byte DLC3        :1;                                       /* Data Length Code Bit 3 */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
  } Bits;
  struct {
    byte grpDLC  :4;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} CANTDLRSTR;
extern volatile CANTDLRSTR _CANTDLR @0x000018BC;
#define CANTDLR                         _CANTDLR.Byte
#define CANTDLR_DLC0                    _CANTDLR.Bits.DLC0
#define CANTDLR_DLC1                    _CANTDLR.Bits.DLC1
#define CANTDLR_DLC2                    _CANTDLR.Bits.DLC2
#define CANTDLR_DLC3                    _CANTDLR.Bits.DLC3
#define CANTDLR_DLC                     _CANTDLR.MergedBits.grpDLC

#define CANTDLR_DLC0_MASK               1
#define CANTDLR_DLC1_MASK               2
#define CANTDLR_DLC2_MASK               4
#define CANTDLR_DLC3_MASK               8
#define CANTDLR_DLC_MASK                15
#define CANTDLR_DLC_BITNUM              0


/*** CANTTBPR - MSCAN Transmit Buffer Priority; 0x000018BD ***/
typedef union {
  byte Byte;
  struct {
    byte PRIO0       :1;                                       /* Transmit Buffer Priority Bit 0 */
    byte PRIO1       :1;                                       /* Transmit Buffer Priority Bit 1 */
    byte PRIO2       :1;                                       /* Transmit Buffer Priority Bit 2 */
    byte PRIO3       :1;                                       /* Transmit Buffer Priority Bit 3 */
    byte PRIO4       :1;                                       /* Transmit Buffer Priority Bit 4 */
    byte PRIO5       :1;                                       /* Transmit Buffer Priority Bit 5 */
    byte PRIO6       :1;                                       /* Transmit Buffer Priority Bit 6 */
    byte PRIO7       :1;                                       /* Transmit Buffer Priority Bit 7 */
  } Bits;
} CANTTBPRSTR;
extern volatile CANTTBPRSTR _CANTTBPR @0x000018BD;
#define CANTTBPR                        _CANTTBPR.Byte
#define CANTTBPR_PRIO0                  _CANTTBPR.Bits.PRIO0
#define CANTTBPR_PRIO1                  _CANTTBPR.Bits.PRIO1
#define CANTTBPR_PRIO2                  _CANTTBPR.Bits.PRIO2
#define CANTTBPR_PRIO3                  _CANTTBPR.Bits.PRIO3
#define CANTTBPR_PRIO4                  _CANTTBPR.Bits.PRIO4
#define CANTTBPR_PRIO5                  _CANTTBPR.Bits.PRIO5
#define CANTTBPR_PRIO6                  _CANTTBPR.Bits.PRIO6
#define CANTTBPR_PRIO7                  _CANTTBPR.Bits.PRIO7

#define CANTTBPR_PRIO0_MASK             1
#define CANTTBPR_PRIO1_MASK             2
#define CANTTBPR_PRIO2_MASK             4
#define CANTTBPR_PRIO3_MASK             8
#define CANTTBPR_PRIO4_MASK             16
#define CANTTBPR_PRIO5_MASK             32
#define CANTTBPR_PRIO6_MASK             64
#define CANTTBPR_PRIO7_MASK             128


/*** CANTTSR - MSCAN Transmit Time Stamp Register; 0x000018BE ***/
typedef union {
  word Word;
   /* Overlapped registers: */
  struct {
    /*** CANTTSRH - MSCAN Transmit Time Stamp Register High; 0x000018BE ***/
    union {
      byte Byte;
      struct {
        byte TSR8        :1;                                       /* Time Stamp Bit 8 */
        byte TSR9        :1;                                       /* Time Stamp Bit 9 */
        byte TSR10       :1;                                       /* Time Stamp Bit 10 */
        byte TSR11       :1;                                       /* Time Stamp Bit 11 */
        byte TSR12       :1;                                       /* Time Stamp Bit 12 */
        byte TSR13       :1;                                       /* Time Stamp Bit 13 */
        byte TSR14       :1;                                       /* Time Stamp Bit 14 */
        byte TSR15       :1;                                       /* Time Stamp Bit 15 */
      } Bits;
    } CANTTSRHSTR;
    #define CANTTSRH                    _CANTTSR.Overlap_STR.CANTTSRHSTR.Byte
    #define CANTTSRH_TSR8               _CANTTSR.Overlap_STR.CANTTSRHSTR.Bits.TSR8
    #define CANTTSRH_TSR9               _CANTTSR.Overlap_STR.CANTTSRHSTR.Bits.TSR9
    #define CANTTSRH_TSR10              _CANTTSR.Overlap_STR.CANTTSRHSTR.Bits.TSR10
    #define CANTTSRH_TSR11              _CANTTSR.Overlap_STR.CANTTSRHSTR.Bits.TSR11
    #define CANTTSRH_TSR12              _CANTTSR.Overlap_STR.CANTTSRHSTR.Bits.TSR12
    #define CANTTSRH_TSR13              _CANTTSR.Overlap_STR.CANTTSRHSTR.Bits.TSR13
    #define CANTTSRH_TSR14              _CANTTSR.Overlap_STR.CANTTSRHSTR.Bits.TSR14
    #define CANTTSRH_TSR15              _CANTTSR.Overlap_STR.CANTTSRHSTR.Bits.TSR15
    
    #define CANTTSRH_TSR8_MASK          1
    #define CANTTSRH_TSR9_MASK          2
    #define CANTTSRH_TSR10_MASK         4
    #define CANTTSRH_TSR11_MASK         8
    #define CANTTSRH_TSR12_MASK         16
    #define CANTTSRH_TSR13_MASK         32
    #define CANTTSRH_TSR14_MASK         64
    #define CANTTSRH_TSR15_MASK         128
    

    /*** CANTTSRL - MSCAN Transmit Time Stamp Register Low; 0x000018BF ***/
    union {
      byte Byte;
      struct {
        byte TSR0        :1;                                       /* Time Stamp Bit 0 */
        byte TSR1        :1;                                       /* Time Stamp Bit 1 */
        byte TSR2        :1;                                       /* Time Stamp Bit 2 */
        byte TSR3        :1;                                       /* Time Stamp Bit 3 */
        byte TSR4        :1;                                       /* Time Stamp Bit 4 */
        byte TSR5        :1;                                       /* Time Stamp Bit 5 */
        byte TSR6        :1;                                       /* Time Stamp Bit 6 */
        byte TSR7        :1;                                       /* Time Stamp Bit 7 */
      } Bits;
    } CANTTSRLSTR;
    #define CANTTSRL                    _CANTTSR.Overlap_STR.CANTTSRLSTR.Byte
    #define CANTTSRL_TSR0               _CANTTSR.Overlap_STR.CANTTSRLSTR.Bits.TSR0
    #define CANTTSRL_TSR1               _CANTTSR.Overlap_STR.CANTTSRLSTR.Bits.TSR1
    #define CANTTSRL_TSR2               _CANTTSR.Overlap_STR.CANTTSRLSTR.Bits.TSR2
    #define CANTTSRL_TSR3               _CANTTSR.Overlap_STR.CANTTSRLSTR.Bits.TSR3
    #define CANTTSRL_TSR4               _CANTTSR.Overlap_STR.CANTTSRLSTR.Bits.TSR4
    #define CANTTSRL_TSR5               _CANTTSR.Overlap_STR.CANTTSRLSTR.Bits.TSR5
    #define CANTTSRL_TSR6               _CANTTSR.Overlap_STR.CANTTSRLSTR.Bits.TSR6
    #define CANTTSRL_TSR7               _CANTTSR.Overlap_STR.CANTTSRLSTR.Bits.TSR7
    
    #define CANTTSRL_TSR0_MASK          1
    #define CANTTSRL_TSR1_MASK          2
    #define CANTTSRL_TSR2_MASK          4
    #define CANTTSRL_TSR3_MASK          8
    #define CANTTSRL_TSR4_MASK          16
    #define CANTTSRL_TSR5_MASK          32
    #define CANTTSRL_TSR6_MASK          64
    #define CANTTSRL_TSR7_MASK          128
    
  } Overlap_STR;

} CANTTSRSTR;
extern volatile CANTTSRSTR _CANTTSR @0x000018BE;
#define CANTTSR                         _CANTTSR.Word


/*** NVFTRIM - Nonvolatile MCG Fine Trim; 0x0000FFAE ***/
typedef union {
  byte Byte;
  struct {
    byte FTRIM       :1;                                       /* MCG Fine Trim */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte             :1; 
  } Bits;
} NVFTRIMSTR;
/* Tip for register initialization in the user code:  const byte NVFTRIM_INIT @0x0000FFAE = <NVFTRIM_INITVAL>; */
#define _NVFTRIM (*(const NVFTRIMSTR * __far)0x0000FFAE)
#define NVFTRIM                         _NVFTRIM.Byte
#define NVFTRIM_FTRIM                   _NVFTRIM.Bits.FTRIM

#define NVFTRIM_FTRIM_MASK              1


/*** NVMCGTRM - Nonvolatile MCG Trim Register; 0x0000FFAF ***/
typedef union {
  byte Byte;
  struct {
    byte TRIM0       :1;                                       /* MCG Trim Setting, bit 0 */
    byte TRIM1       :1;                                       /* MCG Trim Setting, bit 1 */
    byte TRIM2       :1;                                       /* MCG Trim Setting, bit 2 */
    byte TRIM3       :1;                                       /* MCG Trim Setting, bit 3 */
    byte TRIM4       :1;                                       /* MCG Trim Setting, bit 4 */
    byte TRIM5       :1;                                       /* MCG Trim Setting, bit 5 */
    byte TRIM6       :1;                                       /* MCG Trim Setting, bit 6 */
    byte TRIM7       :1;                                       /* MCG Trim Setting, bit 7 */
  } Bits;
} NVMCGTRMSTR;
/* Tip for register initialization in the user code:  const byte NVMCGTRM_INIT @0x0000FFAF = <NVMCGTRM_INITVAL>; */
#define _NVMCGTRM (*(const NVMCGTRMSTR * __far)0x0000FFAF)
#define NVMCGTRM                        _NVMCGTRM.Byte
#define NVMCGTRM_TRIM0                  _NVMCGTRM.Bits.TRIM0
#define NVMCGTRM_TRIM1                  _NVMCGTRM.Bits.TRIM1
#define NVMCGTRM_TRIM2                  _NVMCGTRM.Bits.TRIM2
#define NVMCGTRM_TRIM3                  _NVMCGTRM.Bits.TRIM3
#define NVMCGTRM_TRIM4                  _NVMCGTRM.Bits.TRIM4
#define NVMCGTRM_TRIM5                  _NVMCGTRM.Bits.TRIM5
#define NVMCGTRM_TRIM6                  _NVMCGTRM.Bits.TRIM6
#define NVMCGTRM_TRIM7                  _NVMCGTRM.Bits.TRIM7

#define NVMCGTRM_TRIM0_MASK             1
#define NVMCGTRM_TRIM1_MASK             2
#define NVMCGTRM_TRIM2_MASK             4
#define NVMCGTRM_TRIM3_MASK             8
#define NVMCGTRM_TRIM4_MASK             16
#define NVMCGTRM_TRIM5_MASK             32
#define NVMCGTRM_TRIM6_MASK             64
#define NVMCGTRM_TRIM7_MASK             128


/*** NVBACKKEY0 - Backdoor Comparison Key 0; 0x0000FFB0 ***/
typedef union {
  byte Byte;
  struct {
    byte KEY0        :1;                                       /* Backdoor Comparison Key 0 Bits, bit 0 */
    byte KEY1        :1;                                       /* Backdoor Comparison Key 0 Bits, bit 1 */
    byte KEY2        :1;                                       /* Backdoor Comparison Key 0 Bits, bit 2 */
    byte KEY3        :1;                                       /* Backdoor Comparison Key 0 Bits, bit 3 */
    byte KEY4        :1;                                       /* Backdoor Comparison Key 0 Bits, bit 4 */
    byte KEY5        :1;                                       /* Backdoor Comparison Key 0 Bits, bit 5 */
    byte KEY6        :1;                                       /* Backdoor Comparison Key 0 Bits, bit 6 */
    byte KEY7        :1;                                       /* Backdoor Comparison Key 0 Bits, bit 7 */
  } Bits;
} NVBACKKEY0STR;
/* Tip for register initialization in the user code:  const byte NVBACKKEY0_INIT @0x0000FFB0 = <NVBACKKEY0_INITVAL>; */
#define _NVBACKKEY0 (*(const NVBACKKEY0STR * __far)0x0000FFB0)
#define NVBACKKEY0                      _NVBACKKEY0.Byte
#define NVBACKKEY0_KEY0                 _NVBACKKEY0.Bits.KEY0
#define NVBACKKEY0_KEY1                 _NVBACKKEY0.Bits.KEY1
#define NVBACKKEY0_KEY2                 _NVBACKKEY0.Bits.KEY2
#define NVBACKKEY0_KEY3                 _NVBACKKEY0.Bits.KEY3
#define NVBACKKEY0_KEY4                 _NVBACKKEY0.Bits.KEY4
#define NVBACKKEY0_KEY5                 _NVBACKKEY0.Bits.KEY5
#define NVBACKKEY0_KEY6                 _NVBACKKEY0.Bits.KEY6
#define NVBACKKEY0_KEY7                 _NVBACKKEY0.Bits.KEY7
/* NVBACKKEY_ARR: Access 8 NVBACKKEYx registers in an array */
#define NVBACKKEY_ARR                   ((volatile byte * __far) &NVBACKKEY0)

#define NVBACKKEY0_KEY0_MASK            1
#define NVBACKKEY0_KEY1_MASK            2
#define NVBACKKEY0_KEY2_MASK            4
#define NVBACKKEY0_KEY3_MASK            8
#define NVBACKKEY0_KEY4_MASK            16
#define NVBACKKEY0_KEY5_MASK            32
#define NVBACKKEY0_KEY6_MASK            64
#define NVBACKKEY0_KEY7_MASK            128


/*** NVBACKKEY1 - Backdoor Comparison Key 1; 0x0000FFB1 ***/
typedef union {
  byte Byte;
  struct {
    byte KEY0        :1;                                       /* Backdoor Comparison Key 1 Bits, bit 0 */
    byte KEY1        :1;                                       /* Backdoor Comparison Key 1 Bits, bit 1 */
    byte KEY2        :1;                                       /* Backdoor Comparison Key 1 Bits, bit 2 */
    byte KEY3        :1;                                       /* Backdoor Comparison Key 1 Bits, bit 3 */
    byte KEY4        :1;                                       /* Backdoor Comparison Key 1 Bits, bit 4 */
    byte KEY5        :1;                                       /* Backdoor Comparison Key 1 Bits, bit 5 */
    byte KEY6        :1;                                       /* Backdoor Comparison Key 1 Bits, bit 6 */
    byte KEY7        :1;                                       /* Backdoor Comparison Key 1 Bits, bit 7 */
  } Bits;
} NVBACKKEY1STR;
/* Tip for register initialization in the user code:  const byte NVBACKKEY1_INIT @0x0000FFB1 = <NVBACKKEY1_INITVAL>; */
#define _NVBACKKEY1 (*(const NVBACKKEY1STR * __far)0x0000FFB1)
#define NVBACKKEY1                      _NVBACKKEY1.Byte
#define NVBACKKEY1_KEY0                 _NVBACKKEY1.Bits.KEY0
#define NVBACKKEY1_KEY1                 _NVBACKKEY1.Bits.KEY1
#define NVBACKKEY1_KEY2                 _NVBACKKEY1.Bits.KEY2
#define NVBACKKEY1_KEY3                 _NVBACKKEY1.Bits.KEY3
#define NVBACKKEY1_KEY4                 _NVBACKKEY1.Bits.KEY4
#define NVBACKKEY1_KEY5                 _NVBACKKEY1.Bits.KEY5
#define NVBACKKEY1_KEY6                 _NVBACKKEY1.Bits.KEY6
#define NVBACKKEY1_KEY7                 _NVBACKKEY1.Bits.KEY7

#define NVBACKKEY1_KEY0_MASK            1
#define NVBACKKEY1_KEY1_MASK            2
#define NVBACKKEY1_KEY2_MASK            4
#define NVBACKKEY1_KEY3_MASK            8
#define NVBACKKEY1_KEY4_MASK            16
#define NVBACKKEY1_KEY5_MASK            32
#define NVBACKKEY1_KEY6_MASK            64
#define NVBACKKEY1_KEY7_MASK            128


/*** NVBACKKEY2 - Backdoor Comparison Key 2; 0x0000FFB2 ***/
typedef union {
  byte Byte;
  struct {
    byte KEY0        :1;                                       /* Backdoor Comparison Key 2 Bits, bit 0 */
    byte KEY1        :1;                                       /* Backdoor Comparison Key 2 Bits, bit 1 */
    byte KEY2        :1;                                       /* Backdoor Comparison Key 2 Bits, bit 2 */
    byte KEY3        :1;                                       /* Backdoor Comparison Key 2 Bits, bit 3 */
    byte KEY4        :1;                                       /* Backdoor Comparison Key 2 Bits, bit 4 */
    byte KEY5        :1;                                       /* Backdoor Comparison Key 2 Bits, bit 5 */
    byte KEY6        :1;                                       /* Backdoor Comparison Key 2 Bits, bit 6 */
    byte KEY7        :1;                                       /* Backdoor Comparison Key 2 Bits, bit 7 */
  } Bits;
} NVBACKKEY2STR;
/* Tip for register initialization in the user code:  const byte NVBACKKEY2_INIT @0x0000FFB2 = <NVBACKKEY2_INITVAL>; */
#define _NVBACKKEY2 (*(const NVBACKKEY2STR * __far)0x0000FFB2)
#define NVBACKKEY2                      _NVBACKKEY2.Byte
#define NVBACKKEY2_KEY0                 _NVBACKKEY2.Bits.KEY0
#define NVBACKKEY2_KEY1                 _NVBACKKEY2.Bits.KEY1
#define NVBACKKEY2_KEY2                 _NVBACKKEY2.Bits.KEY2
#define NVBACKKEY2_KEY3                 _NVBACKKEY2.Bits.KEY3
#define NVBACKKEY2_KEY4                 _NVBACKKEY2.Bits.KEY4
#define NVBACKKEY2_KEY5                 _NVBACKKEY2.Bits.KEY5
#define NVBACKKEY2_KEY6                 _NVBACKKEY2.Bits.KEY6
#define NVBACKKEY2_KEY7                 _NVBACKKEY2.Bits.KEY7

#define NVBACKKEY2_KEY0_MASK            1
#define NVBACKKEY2_KEY1_MASK            2
#define NVBACKKEY2_KEY2_MASK            4
#define NVBACKKEY2_KEY3_MASK            8
#define NVBACKKEY2_KEY4_MASK            16
#define NVBACKKEY2_KEY5_MASK            32
#define NVBACKKEY2_KEY6_MASK            64
#define NVBACKKEY2_KEY7_MASK            128


/*** NVBACKKEY3 - Backdoor Comparison Key 3; 0x0000FFB3 ***/
typedef union {
  byte Byte;
  struct {
    byte KEY0        :1;                                       /* Backdoor Comparison Key 3 Bits, bit 0 */
    byte KEY1        :1;                                       /* Backdoor Comparison Key 3 Bits, bit 1 */
    byte KEY2        :1;                                       /* Backdoor Comparison Key 3 Bits, bit 2 */
    byte KEY3        :1;                                       /* Backdoor Comparison Key 3 Bits, bit 3 */
    byte KEY4        :1;                                       /* Backdoor Comparison Key 3 Bits, bit 4 */
    byte KEY5        :1;                                       /* Backdoor Comparison Key 3 Bits, bit 5 */
    byte KEY6        :1;                                       /* Backdoor Comparison Key 3 Bits, bit 6 */
    byte KEY7        :1;                                       /* Backdoor Comparison Key 3 Bits, bit 7 */
  } Bits;
} NVBACKKEY3STR;
/* Tip for register initialization in the user code:  const byte NVBACKKEY3_INIT @0x0000FFB3 = <NVBACKKEY3_INITVAL>; */
#define _NVBACKKEY3 (*(const NVBACKKEY3STR * __far)0x0000FFB3)
#define NVBACKKEY3                      _NVBACKKEY3.Byte
#define NVBACKKEY3_KEY0                 _NVBACKKEY3.Bits.KEY0
#define NVBACKKEY3_KEY1                 _NVBACKKEY3.Bits.KEY1
#define NVBACKKEY3_KEY2                 _NVBACKKEY3.Bits.KEY2
#define NVBACKKEY3_KEY3                 _NVBACKKEY3.Bits.KEY3
#define NVBACKKEY3_KEY4                 _NVBACKKEY3.Bits.KEY4
#define NVBACKKEY3_KEY5                 _NVBACKKEY3.Bits.KEY5
#define NVBACKKEY3_KEY6                 _NVBACKKEY3.Bits.KEY6
#define NVBACKKEY3_KEY7                 _NVBACKKEY3.Bits.KEY7

#define NVBACKKEY3_KEY0_MASK            1
#define NVBACKKEY3_KEY1_MASK            2
#define NVBACKKEY3_KEY2_MASK            4
#define NVBACKKEY3_KEY3_MASK            8
#define NVBACKKEY3_KEY4_MASK            16
#define NVBACKKEY3_KEY5_MASK            32
#define NVBACKKEY3_KEY6_MASK            64
#define NVBACKKEY3_KEY7_MASK            128


/*** NVBACKKEY4 - Backdoor Comparison Key 4; 0x0000FFB4 ***/
typedef union {
  byte Byte;
  struct {
    byte KEY0        :1;                                       /* Backdoor Comparison Key 4 Bits, bit 0 */
    byte KEY1        :1;                                       /* Backdoor Comparison Key 4 Bits, bit 1 */
    byte KEY2        :1;                                       /* Backdoor Comparison Key 4 Bits, bit 2 */
    byte KEY3        :1;                                       /* Backdoor Comparison Key 4 Bits, bit 3 */
    byte KEY4        :1;                                       /* Backdoor Comparison Key 4 Bits, bit 4 */
    byte KEY5        :1;                                       /* Backdoor Comparison Key 4 Bits, bit 5 */
    byte KEY6        :1;                                       /* Backdoor Comparison Key 4 Bits, bit 6 */
    byte KEY7        :1;                                       /* Backdoor Comparison Key 4 Bits, bit 7 */
  } Bits;
} NVBACKKEY4STR;
/* Tip for register initialization in the user code:  const byte NVBACKKEY4_INIT @0x0000FFB4 = <NVBACKKEY4_INITVAL>; */
#define _NVBACKKEY4 (*(const NVBACKKEY4STR * __far)0x0000FFB4)
#define NVBACKKEY4                      _NVBACKKEY4.Byte
#define NVBACKKEY4_KEY0                 _NVBACKKEY4.Bits.KEY0
#define NVBACKKEY4_KEY1                 _NVBACKKEY4.Bits.KEY1
#define NVBACKKEY4_KEY2                 _NVBACKKEY4.Bits.KEY2
#define NVBACKKEY4_KEY3                 _NVBACKKEY4.Bits.KEY3
#define NVBACKKEY4_KEY4                 _NVBACKKEY4.Bits.KEY4
#define NVBACKKEY4_KEY5                 _NVBACKKEY4.Bits.KEY5
#define NVBACKKEY4_KEY6                 _NVBACKKEY4.Bits.KEY6
#define NVBACKKEY4_KEY7                 _NVBACKKEY4.Bits.KEY7

#define NVBACKKEY4_KEY0_MASK            1
#define NVBACKKEY4_KEY1_MASK            2
#define NVBACKKEY4_KEY2_MASK            4
#define NVBACKKEY4_KEY3_MASK            8
#define NVBACKKEY4_KEY4_MASK            16
#define NVBACKKEY4_KEY5_MASK            32
#define NVBACKKEY4_KEY6_MASK            64
#define NVBACKKEY4_KEY7_MASK            128


/*** NVBACKKEY5 - Backdoor Comparison Key 5; 0x0000FFB5 ***/
typedef union {
  byte Byte;
  struct {
    byte KEY0        :1;                                       /* Backdoor Comparison Key 5 Bits, bit 0 */
    byte KEY1        :1;                                       /* Backdoor Comparison Key 5 Bits, bit 1 */
    byte KEY2        :1;                                       /* Backdoor Comparison Key 5 Bits, bit 2 */
    byte KEY3        :1;                                       /* Backdoor Comparison Key 5 Bits, bit 3 */
    byte KEY4        :1;                                       /* Backdoor Comparison Key 5 Bits, bit 4 */
    byte KEY5        :1;                                       /* Backdoor Comparison Key 5 Bits, bit 5 */
    byte KEY6        :1;                                       /* Backdoor Comparison Key 5 Bits, bit 6 */
    byte KEY7        :1;                                       /* Backdoor Comparison Key 5 Bits, bit 7 */
  } Bits;
} NVBACKKEY5STR;
/* Tip for register initialization in the user code:  const byte NVBACKKEY5_INIT @0x0000FFB5 = <NVBACKKEY5_INITVAL>; */
#define _NVBACKKEY5 (*(const NVBACKKEY5STR * __far)0x0000FFB5)
#define NVBACKKEY5                      _NVBACKKEY5.Byte
#define NVBACKKEY5_KEY0                 _NVBACKKEY5.Bits.KEY0
#define NVBACKKEY5_KEY1                 _NVBACKKEY5.Bits.KEY1
#define NVBACKKEY5_KEY2                 _NVBACKKEY5.Bits.KEY2
#define NVBACKKEY5_KEY3                 _NVBACKKEY5.Bits.KEY3
#define NVBACKKEY5_KEY4                 _NVBACKKEY5.Bits.KEY4
#define NVBACKKEY5_KEY5                 _NVBACKKEY5.Bits.KEY5
#define NVBACKKEY5_KEY6                 _NVBACKKEY5.Bits.KEY6
#define NVBACKKEY5_KEY7                 _NVBACKKEY5.Bits.KEY7

#define NVBACKKEY5_KEY0_MASK            1
#define NVBACKKEY5_KEY1_MASK            2
#define NVBACKKEY5_KEY2_MASK            4
#define NVBACKKEY5_KEY3_MASK            8
#define NVBACKKEY5_KEY4_MASK            16
#define NVBACKKEY5_KEY5_MASK            32
#define NVBACKKEY5_KEY6_MASK            64
#define NVBACKKEY5_KEY7_MASK            128


/*** NVBACKKEY6 - Backdoor Comparison Key 6; 0x0000FFB6 ***/
typedef union {
  byte Byte;
  struct {
    byte KEY0        :1;                                       /* Backdoor Comparison Key 6 Bits, bit 0 */
    byte KEY1        :1;                                       /* Backdoor Comparison Key 6 Bits, bit 1 */
    byte KEY2        :1;                                       /* Backdoor Comparison Key 6 Bits, bit 2 */
    byte KEY3        :1;                                       /* Backdoor Comparison Key 6 Bits, bit 3 */
    byte KEY4        :1;                                       /* Backdoor Comparison Key 6 Bits, bit 4 */
    byte KEY5        :1;                                       /* Backdoor Comparison Key 6 Bits, bit 5 */
    byte KEY6        :1;                                       /* Backdoor Comparison Key 6 Bits, bit 6 */
    byte KEY7        :1;                                       /* Backdoor Comparison Key 6 Bits, bit 7 */
  } Bits;
} NVBACKKEY6STR;
/* Tip for register initialization in the user code:  const byte NVBACKKEY6_INIT @0x0000FFB6 = <NVBACKKEY6_INITVAL>; */
#define _NVBACKKEY6 (*(const NVBACKKEY6STR * __far)0x0000FFB6)
#define NVBACKKEY6                      _NVBACKKEY6.Byte
#define NVBACKKEY6_KEY0                 _NVBACKKEY6.Bits.KEY0
#define NVBACKKEY6_KEY1                 _NVBACKKEY6.Bits.KEY1
#define NVBACKKEY6_KEY2                 _NVBACKKEY6.Bits.KEY2
#define NVBACKKEY6_KEY3                 _NVBACKKEY6.Bits.KEY3
#define NVBACKKEY6_KEY4                 _NVBACKKEY6.Bits.KEY4
#define NVBACKKEY6_KEY5                 _NVBACKKEY6.Bits.KEY5
#define NVBACKKEY6_KEY6                 _NVBACKKEY6.Bits.KEY6
#define NVBACKKEY6_KEY7                 _NVBACKKEY6.Bits.KEY7

#define NVBACKKEY6_KEY0_MASK            1
#define NVBACKKEY6_KEY1_MASK            2
#define NVBACKKEY6_KEY2_MASK            4
#define NVBACKKEY6_KEY3_MASK            8
#define NVBACKKEY6_KEY4_MASK            16
#define NVBACKKEY6_KEY5_MASK            32
#define NVBACKKEY6_KEY6_MASK            64
#define NVBACKKEY6_KEY7_MASK            128


/*** NVBACKKEY7 - Backdoor Comparison Key 7; 0x0000FFB7 ***/
typedef union {
  byte Byte;
  struct {
    byte KEY0        :1;                                       /* Backdoor Comparison Key 7 Bits, bit 0 */
    byte KEY1        :1;                                       /* Backdoor Comparison Key 7 Bits, bit 1 */
    byte KEY2        :1;                                       /* Backdoor Comparison Key 7 Bits, bit 2 */
    byte KEY3        :1;                                       /* Backdoor Comparison Key 7 Bits, bit 3 */
    byte KEY4        :1;                                       /* Backdoor Comparison Key 7 Bits, bit 4 */
    byte KEY5        :1;                                       /* Backdoor Comparison Key 7 Bits, bit 5 */
    byte KEY6        :1;                                       /* Backdoor Comparison Key 7 Bits, bit 6 */
    byte KEY7        :1;                                       /* Backdoor Comparison Key 7 Bits, bit 7 */
  } Bits;
} NVBACKKEY7STR;
/* Tip for register initialization in the user code:  const byte NVBACKKEY7_INIT @0x0000FFB7 = <NVBACKKEY7_INITVAL>; */
#define _NVBACKKEY7 (*(const NVBACKKEY7STR * __far)0x0000FFB7)
#define NVBACKKEY7                      _NVBACKKEY7.Byte
#define NVBACKKEY7_KEY0                 _NVBACKKEY7.Bits.KEY0
#define NVBACKKEY7_KEY1                 _NVBACKKEY7.Bits.KEY1
#define NVBACKKEY7_KEY2                 _NVBACKKEY7.Bits.KEY2
#define NVBACKKEY7_KEY3                 _NVBACKKEY7.Bits.KEY3
#define NVBACKKEY7_KEY4                 _NVBACKKEY7.Bits.KEY4
#define NVBACKKEY7_KEY5                 _NVBACKKEY7.Bits.KEY5
#define NVBACKKEY7_KEY6                 _NVBACKKEY7.Bits.KEY6
#define NVBACKKEY7_KEY7                 _NVBACKKEY7.Bits.KEY7

#define NVBACKKEY7_KEY0_MASK            1
#define NVBACKKEY7_KEY1_MASK            2
#define NVBACKKEY7_KEY2_MASK            4
#define NVBACKKEY7_KEY3_MASK            8
#define NVBACKKEY7_KEY4_MASK            16
#define NVBACKKEY7_KEY5_MASK            32
#define NVBACKKEY7_KEY6_MASK            64
#define NVBACKKEY7_KEY7_MASK            128


/*** NVPROT - Nonvolatile FLASH Protection Register; 0x0000FFBD ***/
typedef union {
  byte Byte;
  struct {
    byte FPS0        :1;                                       /* FLASH Protect Select Bits, bit 0 */
    byte FPS1        :1;                                       /* FLASH Protect Select Bits, bit 1 */
    byte FPS2        :1;                                       /* FLASH Protect Select Bits, bit 2 */
    byte FPS3        :1;                                       /* FLASH Protect Select Bits, bit 3 */
    byte FPS4        :1;                                       /* FLASH Protect Select Bits, bit 4 */
    byte FPS5        :1;                                       /* FLASH Protect Select Bits, bit 5 */
    byte EPS0        :1;                                       /* EEPROM Protect Select Bits, bit 0 */
    byte EPS1        :1;                                       /* EEPROM Protect Select Bits, bit 1 */
  } Bits;
  struct {
    byte grpFPS  :6;
    byte grpEPS  :2;
  } MergedBits;
} NVPROTSTR;
/* Tip for register initialization in the user code:  const byte NVPROT_INIT @0x0000FFBD = <NVPROT_INITVAL>; */
#define _NVPROT (*(const NVPROTSTR * __far)0x0000FFBD)
#define NVPROT                          _NVPROT.Byte
#define NVPROT_FPS0                     _NVPROT.Bits.FPS0
#define NVPROT_FPS1                     _NVPROT.Bits.FPS1
#define NVPROT_FPS2                     _NVPROT.Bits.FPS2
#define NVPROT_FPS3                     _NVPROT.Bits.FPS3
#define NVPROT_FPS4                     _NVPROT.Bits.FPS4
#define NVPROT_FPS5                     _NVPROT.Bits.FPS5
#define NVPROT_EPS0                     _NVPROT.Bits.EPS0
#define NVPROT_EPS1                     _NVPROT.Bits.EPS1
#define NVPROT_FPS                      _NVPROT.MergedBits.grpFPS
#define NVPROT_EPS                      _NVPROT.MergedBits.grpEPS

#define NVPROT_FPS0_MASK                1
#define NVPROT_FPS1_MASK                2
#define NVPROT_FPS2_MASK                4
#define NVPROT_FPS3_MASK                8
#define NVPROT_FPS4_MASK                16
#define NVPROT_FPS5_MASK                32
#define NVPROT_EPS0_MASK                64
#define NVPROT_EPS1_MASK                128
#define NVPROT_FPS_MASK                 63
#define NVPROT_FPS_BITNUM               0
#define NVPROT_EPS_MASK                 192
#define NVPROT_EPS_BITNUM               6


/*** NVOPT - Nonvolatile FLASH Options Register; 0x0000FFBF ***/
typedef union {
  byte Byte;
  struct {
    byte SEC0        :1;                                       /* Security State Code, bit 0 */
    byte SEC1        :1;                                       /* Security State Code, bit 1 */
    byte             :1; 
    byte             :1; 
    byte             :1; 
    byte EPGMOD      :1;                                       /* EEPROM Sector Mode Bit */
    byte FNORED      :1;                                       /* Vector Redirection Disable Bit */
    byte KEYEN       :1;                                       /* Backdoor Key Security Enable Bit */
  } Bits;
  struct {
    byte grpSEC  :2;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
    byte         :1;
  } MergedBits;
} NVOPTSTR;
/* Tip for register initialization in the user code:  const byte NVOPT_INIT @0x0000FFBF = <NVOPT_INITVAL>; */
#define _NVOPT (*(const NVOPTSTR * __far)0x0000FFBF)
#define NVOPT                           _NVOPT.Byte
#define NVOPT_SEC0                      _NVOPT.Bits.SEC0
#define NVOPT_SEC1                      _NVOPT.Bits.SEC1
#define NVOPT_EPGMOD                    _NVOPT.Bits.EPGMOD
#define NVOPT_FNORED                    _NVOPT.Bits.FNORED
#define NVOPT_KEYEN                     _NVOPT.Bits.KEYEN
#define NVOPT_SEC                       _NVOPT.MergedBits.grpSEC

#define NVOPT_SEC0_MASK                 1
#define NVOPT_SEC1_MASK                 2
#define NVOPT_EPGMOD_MASK               32
#define NVOPT_FNORED_MASK               64
#define NVOPT_KEYEN_MASK                128
#define NVOPT_SEC_MASK                  3
#define NVOPT_SEC_BITNUM                0



/* Flash commands */
#define mBlank                          0x05
#define mBurstProg                      0x25
#define mByteProg                       0x20
#define mMassErase                      0x41
#define mSectorErase                    0x40
#define mEraseAbort                     0x47


/***********************************************/
/**   D E P R E C I A T E D   S Y M B O L S   **/
/***********************************************/
/* --------------------------------------------------------------------------- */
/* The following symbols were removed, because they were invalid or irrelevant */
/* --------------------------------------------------------------------------- */

/* **** 25.7.2007 11:02:33 */

#define FTSTMOD                          This_symb_has_been_depreciated
#define FTSTMOD_MRDS0                    This_symb_has_been_depreciated
#define FTSTMOD_MRDS1                    This_symb_has_been_depreciated
#define FTSTMOD_MRDS                     This_symb_has_been_depreciated
#define FTSTMOD_MRDS0_MASK               This_symb_has_been_depreciated
#define FTSTMOD_MRDS1_MASK               This_symb_has_been_depreciated
#define FTSTMOD_MRDS_MASK                This_symb_has_been_depreciated
#define FTSTMOD_MRDS_BITNUM              This_symb_has_been_depreciated
#endif
