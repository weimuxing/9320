/*
	$Workfile:   debug_measure.c  $
	$Revision: 1.3 $
	$Date: 2011/12/15 06:09:16 $
*/
#define __DEBUG_MEASURE_C__
//******************************************************************************
// COPYRIGHT (C) STMicroelectronics 2011.
//
// All rights reserved. This document contains proprietary and
// confidential information of the STMicroelectronics Group. This
// document is not to be copied in whole or part. STMicroelectronics
// assumes no responsibility for the consequences of use of such
// information nor for any infringement of patents or other rights of
// third parties which may result from its use. No license is granted by
// implication or otherwise under any patent or patent rights of
// STMicroelectronics.
// STMicroelectronics
// products are not authorized for use as critical components in life
// support devices or systems without express written approval of
// STMicroelectronics.
//==============================================================================
//
// MODULE:      debug_measure.c
//
// USAGE:       This file contains functions for time measurement analysis
//				It was first developed for measuring the VBI driver and
//				TTX decoder
//
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#include "system\all.h"
#include "TestHarness\TestServices\debug_measure.h"
#include "StdLibInc\gm_Register.h"

//******************************************************************************
//  G L O B A L    F U N C T I O N S
//******************************************************************************

//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************
WORD TickBufIdx;// = 0;
ST_MEASURE_STAMP debug_TickBuf[SZ_TICK_BUF];

//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************
#define ENABLE_TMR0_INT		SET_PCB_REG_BITS(T0CON, INT)
#define DISABLE_TMR0_INT	CLEAR_PCB_REG_BITS(T0CON, INT)

//******************************************************************************
//  L O C A L    F U N C T I O N S
//******************************************************************************
#ifdef __MEASURE_IN_USEC__
static void ApplicationTimer0Init (void);
static void interrupt ApplicationTimer0Isr(void);
#endif

//******************************************************************************
//  L O C A L    V A R I A B L E S
//******************************************************************************
static DWORD DW_TickCount;// = 0;

//******************************************************************************
//  M A C R O   D E F I N I T I O N
//******************************************************************************

//******************************************************************************
//  C O D E
//******************************************************************************

//******************************************************************************
//
// FUNCTION       : void debug_Measure_Init (void)
//
// USAGE          : The function initiates the measurement macro
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : none
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void debug_Measure_Init (void)
{
#ifdef __MEASURE_IN_USEC__
   ApplicationTimer0Init();
#endif

#ifdef __MEASURE_BY_PROBE__
   SetGPIO_Enable( FLT_IND_LED_GPIO );		// enable gpio pin
   SetGPIO_DirToOut( FLT_IND_LED_GPIO );	// .. as output
   SetGPIO_OpToPullUp( FLT_IND_LED_GPIO );	// .. pull up (i.e. not open drain)
   SetGPIO_OpHi( FLT_IND_LED_GPIO );		// pin goes hi now

   // output two pulses for reference
   SetGPIO_OpLo( FLT_IND_LED_GPIO );		// pin goes low now
   SetGPIO_OpHi( FLT_IND_LED_GPIO );		// pin goes hi now
   SetGPIO_OpLo( FLT_IND_LED_GPIO );		// pin goes low now
   SetGPIO_OpHi( FLT_IND_LED_GPIO );		// pin goes hi now
#endif

   return;
}

//******************************************************************************
//
// FUNCTION       : DWORD debug_GetTickCount(void)
//
// USAGE          : return tick count
//
// INPUT          : None
//
// OUTPUT         : current system tick count.  The default resolution 1ms,
//                  unless __MEASURE_IN_USEC__ is defined.
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
DWORD debug_GetTickCount(void)
{
#ifdef __MEASURE_IN_USEC__
   register DWORD Dw_Temp;

   DISABLE_TMR0_INT;
   Dw_Temp = DW_TickCount;
   ENABLE_TMR0_INT;

   return Dw_Temp;
#else
   return gm_GetSystemTime();
#endif
}

#ifdef __MEASURE_IN_USEC__
//******************************************************************************
//
// FUNCTION       : static void ApplicationTimer0Init (void)
//
// USAGE          : Initialization of system timer Control Registers and setup	Timer0 interrupt
//                  routine.  The timer is configure with an interrupt interval of
//                  TICK_COUNT_RESOLUTION.
//                  (Note: the function assumes OCM clock = 120MHz)
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : gmc_OCLK_FREQ		(RO)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
#define TICK_COUNT_RESOULTION		100L	// 100 us
static void ApplicationTimer0Init (void)
{
   DWORD D_Tmo;

   /*
   ** Setup timer 0, time-out interval = 100us (assume running @ 120MHz)
   */
   D_Tmo = ((gmc_OCLK_FREQ/1000000L)/4) * TICK_COUNT_RESOULTION;
   WRITE_PCB_REG(T0CMPA, (WORD)D_Tmo);		    // Configure timer0 value
   WRITE_PCB_REG(T0CNT, 0);                    // Clear counter
   //
   // Enable Timer0 with interrupt in continue mode
   // Setup Timer0 Interrupt vector
   //
   SET_VECTOR(TIMER0_VECTOR, ApplicationTimer0Isr);
   WRITE_PCB_REG(T0CON, (EN|INH|INT|CONT));
}
#endif

#ifdef __MEASURE_IN_USEC__
//******************************************************************************
//
// FUNCTION       : static void interrupt ApplicationTimer0Isr (void)
//
// USAGE          : Timer0 interrupt service routine.  The serviceroutine is called every msec.  A
//                  prescale of 20 is used before the applicaiton function is called.
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
static void interrupt ApplicationTimer0Isr (void)
{
   asm sti; //allow other high or equal priority ISR
   DW_TickCount++;
   //
   // Clear MaxCount flag and interrupt exit
   //
   CLEAR_PCB_REG_BITS(T0CON, MC);
   asm cli; //to avoid recurrent interrupt between following line and iret
   WRITE_PCB_REG(EOI, TIMER0_VTYPE);
}
#endif

//******************************************************************************
//
// FUNCTION       : DWORD debug_Measure_GetAverage( WORD Id )
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : debug_TickBuf	(R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
DWORD debug_Measure_GetAverage( WORD Id )
{
   WORD i;
   DWORD average = 0;
   WORD  averagecount = 0;

   for ( i = 0; i < SZ_TICK_BUF; i++ )
   {
      if ( debug_TickBuf[i].id == Id )
      {
         average += debug_TickBuf[i].end - debug_TickBuf[i].begin;
         averagecount++;
      }
   }
   if ( averagecount )
      average = average / averagecount;

   return average;
}

//******************************************************************************
//
// FUNCTION       : DWORD debug_Measure_GetMaximum( WORD Id )
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : debug_TickBuf	(R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
DWORD debug_Measure_GetMaximum( WORD Id )
{
   WORD i;
   DWORD value;
   DWORD max = 0;

   for ( i = 0; i < SZ_TICK_BUF; i++ )
   {
      if ( debug_TickBuf[i].id == Id )
      {
         value = debug_TickBuf[i].end - debug_TickBuf[i].begin;
         if ( value != 0 )
         {
            if ( value > max )
               max = value;
         }
      }
   }

   return max;
}


