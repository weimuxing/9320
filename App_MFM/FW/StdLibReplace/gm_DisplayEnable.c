/*
	$Workfile:   gm_DisplayEnable.c  $
	$Revision: 1.10 $
	$Date: 2012/04/09 06:37:10 $
*/

#define __gm_DisplayEnable_C__
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
// MODULE	: gm_DisplayEnable.c
//
// USAGE	: Enables/Disables the display engine
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#include "system\all.h"

#include "StdLibInc\gm_Global.h"
#include "StdLibInc\gm_HostReg.h"
#include "StdLibInc\gm_Register.h"
#include "StdLibInc\gm_StdLib.h"
#include "StdLibInc\gm_Clocks.h"
#include "StdLibInc\gm_GlobalVariables.h"
//#include "SysLib\Driver\3D\3D.h"

//******************************************************************************
//  C O D E
//******************************************************************************

//******************************************************************************
//
// FUNCTION       : void gm_DisplayOnOff (BOOL Bt_OnOff)
//
// USAGE          : This function is used to either enable or disable the display
//                  engine. This will be used in the case of panel power up and power
//                  down
//
// INPUT          : Bt_OnOff -
//                     Off(0) = Disable
//                     On(1) = Enable
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : DISPLAY_CONTROL		(W)
//
// PRE_CONDITION  : 
//
// POST_CONDITION : 
//
//******************************************************************************
void gm_DisplayOnOff (BOOL Bt_OnOff)
{
	//
      // Control the display engine, display control and display data
     //
	if (Bt_OnOff == Off)
   {
      gm_ClkSelect(CLK_ODP, ODP_SEL_DCLK);
		gm_ClearRegBitsWord(DISPLAY_CONTROL, DTG_RUN_EN | DCNTL_EN | DDATA_EN);


      //if(IsCUT2_1() == FALSE) // Enable Pad CTRL for GPIO change LVDS issue.
      {
         // SW workaround for LVDS QUAL unable control C and D channel
         WORD B_DelayCount = 0;
         
         //while(gm_ReadRegByte(PANEL_POWER_STATUS) != 0x01)
         while(1)
         {            
            if(gm_ReadRegByte(PANEL_POWER_STATUS) <= 1)
            {

               break;
            }
            
            gm_Delay1ms(1);
            B_DelayCount++;
            
      		if (B_DelayCount >= 1000)
            {      
               //gm_Printf("PANEL_POWER_STATUS = 0x%x", gm_ReadRegByte(PANEL_POWER_STATUS));
      			break;               
            }
         }
            
         gm_ClearRegBitsDWord(LVDS_BUS_AB_PAD_CTRL, BUS_A_CH0_ENABLE|BUS_A_CH1_ENABLE|BUS_A_CH2_ENABLE|BUS_A_CH3_ENABLE|BUS_A_CH4_ENABLE|BUS_A_CH5_ENABLE|BUS_A_CLK_ENABLE|
                                                  BUS_B_CH0_ENABLE|BUS_B_CH1_ENABLE|BUS_B_CH2_ENABLE|BUS_B_CH3_ENABLE|BUS_B_CH4_ENABLE|BUS_B_CH5_ENABLE|BUS_B_CLK_ENABLE );
                     
         gm_ClearRegBitsDWord(LVDS_BUS_CD_PAD_CTRL, BUS_C_CH0_ENABLE|BUS_C_CH1_ENABLE|BUS_C_CH2_ENABLE|BUS_C_CH3_ENABLE|BUS_C_CH4_ENABLE|BUS_C_CH5_ENABLE|BUS_C_CLK_ENABLE|
                                                  BUS_D_CH0_ENABLE|BUS_D_CH1_ENABLE|BUS_D_CH2_ENABLE|BUS_D_CH3_ENABLE|BUS_D_CH4_ENABLE|BUS_D_CH5_ENABLE|BUS_D_CLK_ENABLE );

	  }         
   }
	else
   {
		//if(IsCUT2_1() == FALSE) // Enable Pad CTRL for GPIO change LVDS issue.
		{
				 // SW workaround for LVDS QUAL unable control C and D channel

				 //if((gm_ReadRegWord(LVDS_DIGITAL_CTRL)&WIDTH_SEL) == WIDTH_SEL) // QUAD_BUS
				 // Workaround for LVDS
				 {
            #if 1 // BUS_x_CLK_ENABLE is not supported with Athena.
					gm_SetRegBitsDWord(LVDS_BUS_AB_PAD_CTRL, BUS_A_CH0_ENABLE|BUS_A_CH1_ENABLE|BUS_A_CH2_ENABLE|BUS_A_CH3_ENABLE|BUS_A_CH4_ENABLE|BUS_A_CH5_ENABLE|
															 BUS_B_CH0_ENABLE|BUS_B_CH1_ENABLE|BUS_B_CH2_ENABLE|BUS_B_CH3_ENABLE|BUS_B_CH4_ENABLE|BUS_B_CH5_ENABLE);
				 
					gm_SetRegBitsDWord(LVDS_BUS_CD_PAD_CTRL, BUS_C_CH0_ENABLE|BUS_C_CH1_ENABLE|BUS_C_CH2_ENABLE|BUS_C_CH3_ENABLE|BUS_C_CH4_ENABLE|BUS_C_CH5_ENABLE|
															 BUS_D_CH0_ENABLE|BUS_D_CH1_ENABLE|BUS_D_CH2_ENABLE|BUS_D_CH3_ENABLE|BUS_D_CH4_ENABLE|BUS_D_CH5_ENABLE);
					
            #else         
					gm_SetRegBitsDWord(LVDS_BUS_AB_PAD_CTRL, BUS_A_CH0_ENABLE|BUS_A_CH1_ENABLE|BUS_A_CH2_ENABLE|BUS_A_CH3_ENABLE|BUS_A_CH4_ENABLE|BUS_A_CH5_ENABLE|BUS_A_CLK_ENABLE|
															 BUS_B_CH0_ENABLE|BUS_B_CH1_ENABLE|BUS_B_CH2_ENABLE|BUS_B_CH3_ENABLE|BUS_B_CH4_ENABLE|BUS_B_CH5_ENABLE|BUS_B_CLK_ENABLE );
				 
					gm_SetRegBitsDWord(LVDS_BUS_CD_PAD_CTRL, BUS_C_CH0_ENABLE|BUS_C_CH1_ENABLE|BUS_C_CH2_ENABLE|BUS_C_CH3_ENABLE|BUS_C_CH4_ENABLE|BUS_C_CH5_ENABLE|BUS_C_CLK_ENABLE|
															 BUS_D_CH0_ENABLE|BUS_D_CH1_ENABLE|BUS_D_CH2_ENABLE|BUS_D_CH3_ENABLE|BUS_D_CH4_ENABLE|BUS_D_CH5_ENABLE|BUS_D_CLK_ENABLE );
            #endif
				 }													   
			  }
	
	
	#if (FEATURE_3D == ENABLE)
		if (gm_3D_Is3DModeEnabled() && gm_3D_IsODPin400MHz())
		{
		   gm_ClkSelect(CLK_ODP, ODP_SEL_FS432_SYNTH_CLK2);
		}
		else
	#endif // (FEATURE_3D == ENABLE)
		{
		   gm_ClearAndSetBitsWord(LVDS_CLOCK_CONFIG, DCLK_SEL, BIT2); // Set back to PLL
		   gm_ClkSelect(CLK_ODP, ODP_SEL_ODP_PLL_CLK);	 
		}

		
		#if(DEFAULT_PANEL == LG_WQHD_LM270WQ1)
		gm_SetRegBitsWord(DISPLAY_CONTROL, DTG_RUN_EN | DCNTL_EN);
		#else
		#ifdef MODE_CHANGE_BACKLIGHT_OFF
		gm_SetRegBitsWord(DISPLAY_CONTROL, DCNTL_EN | DDATA_EN);
		#else
		gm_SetRegBitsWord(DISPLAY_CONTROL, DTG_RUN_EN | DCNTL_EN | DDATA_EN);
		#endif
		#endif

		{
			WORD B_DelayCount = 0;
			
			BYTE B_PanelInit = gmd_FALSE;
			while(1)
			{			 
				#ifndef MODE_CHANGE_BACKLIGHT_OFF
				#if((DEFAULT_PANEL == ZDT_4K2K_CMI)||(DEFAULT_PANEL == ZDT_4K2K_CMI_39)\
				||(DEFAULT_PANEL == ZDT_4K2K_CMI_65)||(DEFAULT_PANEL == ZDT_4K2K_CMI_50))
				if((gm_ReadRegByte(PANEL_POWER_STATUS) >= 1)&&(B_PanelInit == gmd_FALSE))
				{
					gm_Delay10ms(20);  //20  wei 140814 inital delay Vcc to Backlight
					ZDT_4K2K_CMI_Panel_Init(UserPrefPanelSelect);
					B_PanelInit = gmd_TRUE;
				}
				#endif
				#endif
			   
			   if(gm_ReadRegByte(PANEL_POWER_STATUS) >= 2)
			   {
				  break;
			   }
			   
			   gm_Delay1ms(1);
			   B_DelayCount++;
			   
			   if (B_DelayCount >= 1000)
			   {	  
				  //gm_Printf("PANEL_POWER_STATUS = 0x%x", gm_ReadRegByte(PANEL_POWER_STATUS));
				   break;				
			   }
			}
			B_PanelInit = B_PanelInit;
		}
	
		#if (DEFAULT_PANEL == LG_WQHD_LM270WQ1)
		gm_SetRegBitsWord(DISPLAY_CONTROL,  DDATA_EN);
		#endif
   }
}

//*********************************  END  **************************************
