/*
	$Workfile:   wdt.c  $
	$Revision: 1.11 $
	$Date: 2012/10/03 08:39:44 $
*/

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
// MODULE:      wdt.c
//
// USAGE:       WDT interface implementetion
//
//******************************************************************************
//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "system\all.h"
#include "StdLibInc\gm_Mcu186.h"
#include "StdLibInc\gm_Register.h"

#ifdef WDT_SERVICE_ENABLE

//******************************************************************************

//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************

//******************************************************************************
//  F U N C T I O N   P R O T O T Y P E S
//******************************************************************************
void LoopDelay1ms(WORD ms);

//******************************************************************************
//  C O D E
//******************************************************************************

//******************************************************************************
//
// FUNCTION       : void WdtResetSystem(void)
//
// USAGE          : Turn off panel than reset system thru GPIO routed to reset
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : GPIO 9.5
//
//******************************************************************************
#pragma codeseg _DATA "DATA" DGROUP //WDT code is allocated in RAM for stronger reliability

void WdtResetSystem(void)
{
   WORD w;
   //gm_PowerDownAll(); could be used instead of following lines
   //Panel power off
#if 1 // Fix gm_DisplayOnOff() function call gm_Delay1ms() in ISR cause hang up issue.
   gm_ClkSelect(CLK_ODP, ODP_SEL_DCLK);
   gm_ClearRegBitsWord(DISPLAY_CONTROL, DTG_RUN_EN);
   
   //if(IsCUT2_1() == FALSE) // Enable Pad CTRL for GPIO change LVDS issue.
   {
      // SW workaround for LVDS QUAL unable control C and D channel
      WORD B_DelayCount = 0;
      while(gm_ReadRegByte(PANEL_POWER_STATUS) != 0x01)
      {            
         LoopDelay1ms(1);
         B_DelayCount++;
         
         if (B_DelayCount == 600)
         {      
            gm_Printf("PANEL_POWER_STATUS = 0x%x", gm_ReadRegByte(PANEL_POWER_STATUS));
            break;               
         }
      }
         
      gm_ClearRegBitsDWord(LVDS_BUS_AB_PAD_CTRL, BUS_A_CH0_ENABLE|BUS_A_CH1_ENABLE|BUS_A_CH2_ENABLE|BUS_A_CH3_ENABLE|BUS_A_CH4_ENABLE|BUS_A_CH5_ENABLE|BUS_A_CLK_ENABLE|
                                                 BUS_B_CH0_ENABLE|BUS_B_CH1_ENABLE|BUS_B_CH2_ENABLE|BUS_B_CH3_ENABLE|BUS_B_CH4_ENABLE|BUS_B_CH5_ENABLE|BUS_B_CLK_ENABLE );   
         
      gm_ClearRegBitsDWord(LVDS_BUS_CD_PAD_CTRL, BUS_C_CH0_ENABLE|BUS_C_CH1_ENABLE|BUS_C_CH2_ENABLE|BUS_C_CH3_ENABLE|BUS_C_CH4_ENABLE|BUS_C_CH5_ENABLE|BUS_C_CLK_ENABLE|
                                               BUS_D_CH0_ENABLE|BUS_D_CH1_ENABLE|BUS_D_CH2_ENABLE|BUS_D_CH3_ENABLE|BUS_D_CH4_ENABLE|BUS_D_CH5_ENABLE|BUS_D_CLK_ENABLE );
   }         
   gm_Print("Turn off panel");
   gm_SetRegBitsWord(PBUS_MSTR_CTRL_1,PBUS_TOC_ENABLE|PBUS_TOC_SEL);
#else
   gm_DisplayOnOff(Off);
#endif

   gm_Print("WDT_Reset",0);

   //Wait for panel power down sequence to complete
   w = (WORD)((DWORD)(((PanelPwrDnTiming >> 8) & 0xFF) + (PanelPwrDnTiming & 0xFF))*511*32*1000/gmc_TCLK_FREQ);
   if (w>0xFF) w=0xFF;

   //Delay "w" ms
   for (;w;w--)
   {
      WRITE_PCB_REG(T2CMPA, gmc_OCLK_FREQ/4000);		//Setup Timer2 to 1ms timeout
      WRITE_PCB_REG(T2CNT, 0); 								//Clear counter
      WRITE_PCB_REG(T2CON,  EN|INH);						//Enable Timer2
      while ( (READ_PCB_REG(T2CON) & MC) == 0);
      //WdtResetHW();
   }

   //reset chip
#ifdef WDT_RESET_BY_HARD_RESETS_REG
   gm_WriteRegWord(OCM_NR_SCRATCHPAD,0);
   gm_ClearRegBitsWord(CLOCK_CONFIG1,OCM_CLK_SEL);
   gm_WriteRegWord(HARD_RESETS_2, 0xFFFF);
   gm_WriteRegWord(AUDIO_HARD_RESETS, 0xFFFF);
   //Reset LVDS to avoid DCLK from PLL is gated off inside LVDS loop that kill ODP source clock.
   gm_WriteRegWord(LVDS_RESET_CTRL, LVDSTX_HARD_RESET);
   gm_WriteRegWord(HARD_RESETS  , 0xFFFF);
#else
   GPIO_SELF_RESET();

   //followed line should not be called
   gm_Print("ERROR: Board is not equipped with Self reset circuitry", 0);
#endif
}

//******************************************************************************
//
// FUNCTION       : void LoopDelay1ms(WORD ms)
//
// USAGE          : Delay 1ms by while loop 
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : None
//
//******************************************************************************
#define LOOP_DELAY1MS_COUNT 4500 // 4500 for OCM clcok run in 100MHz.
void LoopDelay1ms(WORD ms)
{
   WORD count;
   while(ms--)
   {
      for(count = 0; count < LOOP_DELAY1MS_COUNT; count++)
         { asm nop; }
   }
}

//******************************************************************************
#endif //WDT_SERVICE_ENABLE


