/*
   $Workfile:   gm_DisplayIsr.c  $
   $Revision: 1.19 $
   $Date: 2013/05/16 09:59:43 $
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
// MODULE:      gm_DisplayIsr.c
//
// USAGE:       This file contains standard library function for the
//            display ISR function setup.
//
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#include <mem.h>

#include "System\All.h"
#include "stdlibinc\gm_Mcu186.h"
#include "StdLibInc\gm_Register.h"
#include "StdLibInc\gm_UpdateSharpness.h"
#include "StdLibInc\gm_ACC3.h"

//******************************************************************************
//  S T A T I C   V A R I A B L E S
//******************************************************************************

//******************************************************************************
//  F U N C T I O N     D E C L A R A T I O N
//******************************************************************************
extern void gm_AUDIO_INTERRUPT(void);

//******************************************************************************
//  C O D E
//******************************************************************************

//******************************************************************************
//
// FUNCTION       : void gm_DptxInterlaceIsr (void)
//
// USAGE          : ISR function for DPTx interlace output
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
#if 1//(NON_SCALING_OUTPUT == 1) && (PASS_THRU_MODE == 0) && (DEINTERLACED_OUTPUT == 0) && (FEATURE_DISPLAY_PORT == ENABLE)
void gm_DptxInterlaceIsr(void)
{
   static BOOL OutputPolarity;
   static BYTE B_ErrCount = 0;
   static WORD W_ImpVTotal = 0;

   if (OutputPolarity)
   {
      gm_SetRegBitsWord(DPTX_MS_CTRL0, DPTX_MS_FIELD_ID_INV);     
      gm_WriteRegWord(DPTX_MS_VTOTAL, W_ImpVTotal | BIT0);
      OutputPolarity = 0;
   }
   else
   {
      gm_ClearRegBitsWord(DPTX_MS_CTRL0, DPTX_MS_FIELD_ID_INV);
      gm_WriteRegWord(DPTX_MS_VTOTAL, W_ImpVTotal & 0xFFFE);
      OutputPolarity = 1;
   }

   gm_WriteRegWord(DPTX_MS_UPDATE, DPTX_MS_UPDATE_CTRL);

   W_ImpVTotal = gm_ReadRegWord(IMP_IBD_VTOTAL);

   if (OutputPolarity == (BOOL)(W_ImpVTotal & BIT0))
   {  
      B_ErrCount++;
      
      if (B_ErrCount >= 8)
      {
         OutputPolarity = (~OutputPolarity) & BIT0;
         B_ErrCount = 0;
      }
   }
}
#endif

//******************************************************************************
//
// FUNCTION       : void far gm_DisplayIsrProcess (WORD W_DisplayStatus)
//
// USAGE          : Setup the DISPLAY ISR function call.  This will re-direct the
//                  call to other ISR process.
//
// INPUT          : W_DisplayStatus - DISPLAY_DEC_STATUS
//
// OUTPUT         : none
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void far interrupt gm_DisplayIsrProcess (void)
{
   WORD W_DisplayStatus = gm_ReadRegWord(DISPLAY_DEC_STATUS);

   if (W_DisplayStatus & D_ACTIVE)
   {
#if (FEATURE_3D == ENABLE)
      gm_3D_Routine_In_DV_Active();
#endif
      gm_WriteRegWord(DISPLAY_DEC_STATUS, D_ACTIVE);
   }

   if (W_DisplayStatus & D_BLANK)
   {
#if (FEATURE_3D == ENABLE)
      gm_3D_Routine_In_DV_Blank();
#endif
      gm_WriteRegWord(DISPLAY_DEC_STATUS, D_BLANK);
   }

   if (W_DisplayStatus & D_VS_EDGE)
   {
//#if (NON_SCALING_OUTPUT == 1) && (PASS_THRU_MODE == 0) && (DEINTERLACED_OUTPUT == 0) && (FEATURE_DISPLAY_PORT == ENABLE)
	  #ifdef OUTPUT_MODE_CHANGE
	  if(UserPrefOutputMode == OPM_NoScaling)
      		gm_DptxInterlaceIsr();
	  #endif
//#endif

#if (FEATURE_3D == ENABLE)
      gm_3D_Routine_In_DV_Sync();
#endif
      gm_WriteRegWord(DISPLAY_DEC_STATUS, D_VS_EDGE);
   }

   if ((gm_ReadRegWord(DISPLAY_DEC_IRQ5MASK) & D_LINEFLAG_MASK) && (W_DisplayStatus & D_LINEFLAG))
   {
#if (FEATURE_3D == ENABLE)
      gm_3D_Routine_In_DV_LineFlag();
#endif

#if (FEATURE_ACC3 == ENABLE)
      // flip ACC3 LUT every dispaly frame in one line before display data is enabled
      gm_ACC3SwitchLUT();
#endif

      gm_WriteRegWord(DISPLAY_DEC_STATUS, D_LINEFLAG);
   }

   if (W_DisplayStatus & LPM_DDC_IRQ)
   {
   	SLAVE_CH I2cSlaveCh = I2C_SLAVE4_CH;
   	WORD LpmMisCsmIStatus = gm_ReadRegWord(LPM_MIS_CSM_ISTATUS);

//   gm_Print("#### LPM_DDC_IRQ ####",0);
      
   	if ((W_DisplayStatus & LPM_DDC4_IRQ_STS)||(LpmMisCsmIStatus & LPM_MIS_DDC4_IRQ_STATUS))
   		I2cSlaveCh = I2C_SLAVE4_CH;
   	else if((W_DisplayStatus & LPM_DDC2_IRQ_STS)||(LpmMisCsmIStatus & LPM_MIS_DDC2_IRQ_STATUS))
   		I2cSlaveCh = I2C_SLAVE2_CH;
   	else if((W_DisplayStatus & LPM_DDC3_IRQ_STS)||(LpmMisCsmIStatus & LPM_MIS_DDC3_IRQ_STATUS)) 
   		I2cSlaveCh = I2C_SLAVE3_CH;
   	else if((W_DisplayStatus & LPM_DDC1_IRQ_STS)||(LpmMisCsmIStatus & LPM_MIS_DDC1_IRQ_STATUS))
   		I2cSlaveCh = I2C_SLAVE1_CH;

      
   	gm_I2CSlaveIsr(I2cSlaveCh);


      if(I2cSlaveCh == I2C_SLAVE4_CH)
      {
      	gm_WriteRegWord(LPM_MIS_CSM_ISTATUS, LPM_MIS_DDC4_IRQ_STATUS);	
      	gm_WriteRegWord(DISPLAY_DEC_STATUS, LPM_DDC4_IRQ_STS|LPM_DDC_IRQ);
      }
      else if(I2cSlaveCh == I2C_SLAVE2_CH)
      {
      	gm_WriteRegWord(LPM_MIS_CSM_ISTATUS, LPM_MIS_DDC2_IRQ_STATUS);	
      	gm_WriteRegWord(DISPLAY_DEC_STATUS, LPM_DDC2_IRQ_STS|LPM_DDC_IRQ);
      }      
      else if(I2cSlaveCh == I2C_SLAVE3_CH)
      {
      	gm_WriteRegWord(LPM_MIS_CSM_ISTATUS, LPM_MIS_DDC3_IRQ_STATUS);	
      	gm_WriteRegWord(DISPLAY_DEC_STATUS, LPM_DDC3_IRQ_STS|LPM_DDC_IRQ);
      }  
      else if(I2cSlaveCh == I2C_SLAVE1_CH)
      {
      	gm_WriteRegWord(LPM_MIS_CSM_ISTATUS, LPM_MIS_DDC1_IRQ_STATUS);	
      	gm_WriteRegWord(DISPLAY_DEC_STATUS, LPM_DDC1_IRQ_STS|LPM_DDC_IRQ);
      }    
   }

/*ChengLin 20130221: AFR Interrupt Handler, Don't move it to the front of gm_DisplayIsrProcess*/
   if ( (gm_ReadRegWord(DISPLAY_DEC_IRQ5MASK) & IMP_AFR_DETECT_MASK) && (W_DisplayStatus & IMP_AFR_DETECT))
   {
#ifdef ST_4K2K_93xx_BOARD 
#if ENABLE_ATHENA_R_L_IMAGE_SYNC_HANDLER   
	Image_Force_OnOff(FALSE); //ChengLin 20130301: Add for fast black screen for avoid Athena Right to observe garbage images
#endif

      #ifdef ST_4K2K_93xx_BOARD
      if((gm_ReadRegWord(CLOCK_CONFIG1)&LVTX_IN_CLK_SEL) == (BIT11|BIT12)) // LVTX_IN_SEL_IMP_CLK
      {
         #if 0
         WORD W_VEGA_LVDS_MODE = SOCIPC_VEGA_I2C_ReadWord_Via_FE(0x0C);

         if((W_VEGA_LVDS_MODE&0xFFFC) != BIT1)
         {
            W_VEGA_LVDS_MODE = (W_VEGA_LVDS_MODE & 0xFFFC)|BIT1;
            SOCIPC_VEGA_I2C_WriteWord_Via_FE(0x0C, W_VEGA_LVDS_MODE); // Quad bus            
            gm_ClearAndSetBitsDWord(LVDS_DIGITAL_CTRL, WIDTH_SEL, LvdsBusType);
            gm_ClearRegBitsWord(LVDS_DIGITAL_CTRL, A_B_SWAP|C_D_SWAP);
            gm_WriteRegDWord_LE(LVDS_DLL_CTRL_FREQ, 0x78);
         }
         
         AssignPanelData(0);
         #endif
         
         gm_ClkSelect(CLK_LVTX_IN, LVTX_IN_SEL_DDDS);
         gm_ClkSelect(CLK_ODP_CAPTURE, ODP_CAPTURE_SEL_DCLK);
         gm_ClkSelect(CLK_ODP, ODP_SEL_ODP_PLL_CLK); // ODP_CLK must before DCLK_SEL
      }
      #endif
#endif

   #if (FEATURE_AUDIO == ENABLE)
      gm_AUDIO_INTERRUPT();
   #endif
   
     AFR_Interrupt();
      gm_WriteRegWord(DISPLAY_DEC_STATUS, IMP_AFR_DETECT); //Clear Main AFR Interrupt			 
   }

#if (FEATURE_AUDIO == ENABLE)
   gm_AUDIO_INTERRUPT();
#endif

   asm cli; //to avoid interrupt between next command and iret

   WRITE_PCB_REG(EOI, INT5_VTYPE);  // Interrupt exit
}


//*********************************  END  **************************************
