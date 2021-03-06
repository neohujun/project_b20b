/*
** ###################################################################
**     This code is generated by the Device Initialization Tool.
**     It is overwritten during code generation.
**     USER MODIFICATION ARE PRESERVED ONLY INSIDE INTERRUPT SERVICE ROUTINES
**     OR EXPLICITLY MARKED SECTIONS
**
**     Project   : Platform
**     Processor : MC9S08DZ60CLH
**     Version   : Component 01.075, Driver 01.06, CPU db: 3.00.021
**     Datasheet : MC9S08DZ60 Rev. 3 10/2007
**     Date/Time : 2015/5/12, 10:31
**     Abstract  :
**         This module contains device initialization code 
**         for selected on-chip peripherals.
**     Contents  :
**         Function "MCU_init" initializes selected peripherals
**
**     Copyright : 1997 - 2009 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################
*/

/* MODULE MCUinit */

#include <MC9S08DZ60.h>                /* I/O map for MC9S08DZ60CLH */
#include "MCUinit.h"

#include "uart.h"
#include "Timer.h"
#include "iic.h"
#include "can.h"
#include "PortInterrupt.h"
#include "Rtc.h"
#include "Config.h"
#include "Tpm.h"
#include "Pwm.h"

/* User declarations and definitions */
/*   Code, declarations and definitions here will be preserved during code generation */
/* End of user declarations and definitions */


/*
** ===================================================================
**     Method      :  MCU_init (component MC9S08DZ60_64)
**
**     Description :
**         Device initialization code for selected peripherals.
** ===================================================================
*/
void MCU_init(void)
{
  /* ### MC9S08DZ60_64 "Cpu" init code ... */
  /*  PE initialization code after reset */

  /* Common initialization of the write once registers */
  /* SOPT1: COPT=3,STOPE=1,SCI2PS=0,IICPS=0 */
#ifdef	__DEBUG__
  SOPT1 = 0x20;
#else
#ifdef	__M40_FINAL__
  SOPT1 = 0xE0;
#else
  SOPT1 = 0x20;
#endif
#endif
  /* SOPT2: COPCLKS=1,COPW=0,ADHTS=0,MCSEL=0 */
  SOPT2 = 0x80;                                      
  /* SPMSC1: LVWF=0,LVWACK=0,LVWIE=0,LVDRE=1,LVDSE=1,LVDE=1,BGBE=0 */
  SPMSC1 = 0x1C;                                      
  /* SPMSC2: LVDV=0,LVWV=0,PPDF=0,PPDACK=0,PPDC=0 */
  SPMSC2 = 0x00;                                      
  /*  System clock initialization */
  if (*(unsigned char*far)0xFFAF != 0xFF) { /* Test if the device trim value is stored on the specified address */
    MCGTRM = *(unsigned char*far)0xFFAF; /* Initialize MCGTRM register from a non volatile memory */
    MCGSC = *(unsigned char*far)0xFFAE; /* Initialize MCGSC register from a non volatile memory */
  }
  /* MCGC2: BDIV=0,RANGE=1,HGO=1,LP=0,EREFS=1,ERCLKEN=1,EREFSTEN=0 */
  MCGC2 = 0x36;                        /* Set MCGC2 register */
  /* MCGC1: CLKS=2,RDIV=7,IREFS=0,IRCLKEN=0,IREFSTEN=0 */
  MCGC1 = 0xB8;                        /* Set MCGC1 register */
  while(!MCGSC_OSCINIT) {              /* Wait until external reference is stable */
  }
  while(MCGSC_IREFST) {                /* Wait until external reference is selected */
  }
  while((MCGSC & 0x0C) != 0x08) {      /* Wait until external clock is selected as a bus clock reference */
  }
  /* MCGC2: BDIV=0,RANGE=1,HGO=1,LP=1,EREFS=1,ERCLKEN=1,EREFSTEN=0 */
  MCGC2 = 0x3E;                        /* Set MCGC2 register */
  /* MCGC1: CLKS=2,RDIV=1,IREFS=0,IRCLKEN=0,IREFSTEN=0 */
  MCGC1 = 0x88;                        /* Set MCGC1 register */
  /* MCGC3: LOLIE=0,PLLS=1,CME=0,VDIV=4 */
  MCGC3 = 0x44;                        /* Set MCGC3 register */
  /* MCGC2: LP=0 */
  MCGC2 &= (unsigned char)~0x08;                     
  while(!MCGSC_PLLST) {                /* Wait until PLL is selected */
  }
  while(!MCGSC_LOCK) {                 /* Wait until PLL is locked */
  }
  /* MCGC1: CLKS=0,RDIV=1,IREFS=0,IRCLKEN=0,IREFSTEN=0 */
  MCGC1 = 0x08;                        /* Set MCGC1 register */
  while((MCGSC & 0x0C) != 0x0C) {      /* Wait until PLL clock is selected as a bus clock reference */
  }
  
  /* Common initialization of the CPU registers */
  /* PTASE: PTASE7=0,PTASE6=0,PTASE5=0,PTASE4=0,PTASE3=0,PTASE2=0,PTASE1=0,PTASE0=0 */
  PTASE = 0x00;                                      
  /* PTBSE: PTBSE7=0,PTBSE6=0,PTBSE5=0,PTBSE4=0,PTBSE3=0,PTBSE2=0,PTBSE1=0,PTBSE0=0 */
  PTBSE = 0x00;                                      
  /* PTCSE: PTCSE7=0,PTCSE6=0,PTCSE5=0,PTCSE4=0,PTCSE3=0,PTCSE2=0,PTCSE1=0,PTCSE0=0 */
  PTCSE = 0x00;                                      
  /* PTDSE: PTDSE7=0,PTDSE6=0,PTDSE5=0,PTDSE4=0,PTDSE3=0,PTDSE2=0,PTDSE1=0,PTDSE0=0 */
  PTDSE = 0x00;                                      
  /* PTESE: PTESE7=0,PTESE6=0,PTESE5=0,PTESE4=0,PTESE3=0,PTESE2=0,PTESE0=0 */
  PTESE &= (unsigned char)~0xFD;                     
  /* PTFSE: PTFSE7=0,PTFSE6=0,PTFSE5=0,PTFSE4=0,PTFSE3=0,PTFSE2=0,PTFSE1=0,PTFSE0=0 */
  PTFSE = 0x00;                                      
  /* PTGSE: PTGSE5=0,PTGSE4=0,PTGSE3=0,PTGSE2=0,PTGSE1=0,PTGSE0=0 */
  PTGSE &= (unsigned char)~0x3F;                     
  /* PTADS: PTADS7=0,PTADS6=0,PTADS5=0,PTADS4=0,PTADS3=0,PTADS2=0,PTADS1=0,PTADS0=0 */
  PTADS = 0x00;                                      
  /* PTBDS: PTBDS7=0,PTBDS6=0,PTBDS5=0,PTBDS4=0,PTBDS3=0,PTBDS2=0,PTBDS1=0,PTBDS0=0 */
  PTBDS = 0x00;                                      
  /* PTCDS: PTCDS7=0,PTCDS6=0,PTCDS5=0,PTCDS4=0,PTCDS3=0,PTCDS2=0,PTCDS1=0,PTCDS0=0 */
  PTCDS = 0x00;                                      
  /* PTDDS: PTDDS7=0,PTDDS6=0,PTDDS5=0,PTDDS4=0,PTDDS3=0,PTDDS2=0,PTDDS1=0,PTDDS0=0 */
  PTDDS = 0x00;                                      
  /* PTEDS: PTEDS7=0,PTEDS6=0,PTEDS5=0,PTEDS4=0,PTEDS3=0,PTEDS2=0,PTEDS1=0,PTEDS0=0 */
  PTEDS = 0x00;                                      
  /* PTFDS: PTFDS7=0,PTFDS6=0,PTFDS5=0,PTFDS4=0,PTFDS3=0,PTFDS2=0,PTFDS1=0,PTFDS0=0 */
  PTFDS = 0x00;                                      
  /* PTGDS: PTGDS5=0,PTGDS4=0,PTGDS3=0,PTGDS2=0,PTGDS1=0,PTGDS0=0 */
  PTGDS = 0x00;                                      
  /* ### */
//  asm CLI;                             /* Enable interrupts */
} /*MCU_init*/



/* Initialization of the CPU registers in FLASH */

/* NVPROT: EPS=3,FPS=0x3F */
//const unsigned char NVPROT_INIT @0x0000FFBD = 0x3E;

/* NVOPT: KEYEN=0,FNORED=1,EPGMOD=1,SEC1=1,SEC0=0 */
//const unsigned char NVOPT_INIT @0x0000FFBF = 0x62;



extern near void _Startup(void);

/* Interrupt vector table */
#ifndef UNASSIGNED_ISR
  #define UNASSIGNED_ISR ((void(*near const)(void)) 0xFFFF) /* unassigned interrupt service routine */
#endif

void (* near const _vect[])(void) @0xFFC0 = { /* Interrupt vector table */
	UNASSIGNED_ISR,               /* Int.no. 31 Vacmp2 (at FFC0)                Unassigned */
	UNASSIGNED_ISR,               /* Int.no. 30 Vacmp1 (at FFC2)                Unassigned */
	INTCanTx,                          /* Int.no. 29 Vcantx (at FFC4)                Unassigned */
	INTCanRx,                          /* Int.no. 28 Vcanrx (at FFC6)                Unassigned */
	INTCanErr,                         /* Int.no. 27 Vcanerr (at FFC8)               Unassigned */
	INTCanWu,                         /* Int.no. 26 Vcanwu (at FFCA)                Unassigned */
	UNASSIGNED_ISR,				/* Int.no. 25 Vrtc (at FFCC)                  Unassigned */
	UNASSIGNED_ISR,                               /* Int.no. 24 Viic (at FFCE)                  Unassigned */
	UNASSIGNED_ISR,               /* Int.no. 23 Vadc (at FFD0)                  Unassigned */
	INTPort,              	 /* Int.no. 22 Vport (at FFD2)                 Unassigned */
	INTUart2Tx,                       /* Int.no. 21 Vsci2tx (at FFD4)               Unassigned */
	INTUart2Rx,                       /* Int.no. 20 Vsci2rx (at FFD6)               Unassigned */
	INTUart2Error,                  /* Int.no. 19 Vsci2err (at FFD8)              Unassigned */
	INTUart1Tx,             		  /* Int.no. 18 Vsci1tx (at FFDA)               Unassigned */
	INTUart1Rx,           	    /* Int.no. 17 Vsci1rx (at FFDC)               Unassigned */
	INTUart1Error,         	      /* Int.no. 16 Vsci1err (at FFDE)              Unassigned */
	UNASSIGNED_ISR,               /* Int.no. 15 Vspi (at FFE0)                  Unassigned */
	UNASSIGNED_ISR,                        /* Int.no. 14 Vtpm2ovf (at FFE2)              Unassigned */
	UNASSIGNED_ISR,               /* Int.no. 13 Vtpm2ch1 (at FFE4)              Unassigned */
	UNASSIGNED_ISR,               /* Int.no. 12 Vtpm2ch0 (at FFE6)              Unassigned */
	vINTTimer,               /* Int.no. 11 Vtpm1ovf (at FFE8)              Unassigned */
	UNASSIGNED_ISR,               /* Int.no. 10 Vtpm1ch5 (at FFEA)              Unassigned */
	UNASSIGNED_ISR,               /* Int.no.  9 Vtpm1ch4 (at FFEC)              Unassigned */
	UNASSIGNED_ISR,               /* Int.no.  8 Vtpm1ch3 (at FFEE)              Unassigned */
	UNASSIGNED_ISR,               /* Int.no.  7 Vtpm1ch2 (at FFF0)              Unassigned */
	UNASSIGNED_ISR,               /* Int.no.  6 Vtpm1ch1 (at FFF2)              Unassigned */
	UNASSIGNED_ISR,               /* Int.no.  5 Vtpm1ch0 (at FFF4)              Unassigned */
	UNASSIGNED_ISR,               /* Int.no.  4 Vlol (at FFF6)                  Unassigned */
	UNASSIGNED_ISR,               /* Int.no.  3 Vlvd (at FFF8)                  Unassigned */
	UNASSIGNED_ISR,               /* Int.no.  2 Virq (at FFFA)                  Unassigned */
	UNASSIGNED_ISR,               /* Int.no.  1 Vswi (at FFFC)                  Unassigned */
	_Startup                      /* Int.no.  0 Vreset (at FFFE)                Reset vector */
};




/* END MCUinit */

/*
** ###################################################################
**
**     This file was created by Processor Expert 3.07 [04.34]
**     for the Freescale HCS08 series of microcontrollers.
**
** ###################################################################
*/
