/*
            $Workfile:   gm_InputSetHPosition.c  $
            $Revision: 1.6 $
            $Date    :   $
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
// MODULE:      gm_InputSetHPosition.c
//
//******************************************************************************
#include "StdLibInc\gm_Global.h"
#include "StdLibInc\gm_HostReg.h"
#include "StdLibInc\gm_Register.h"
#include "StdLibInc\gm_StdLib.h"
#include "StdLibInc\gm_GlobalVariables.h"
#include "StdLibInc\gm_ColorControl.h"
#include "StdLibInc\gm_ChipFeature.h"
#include "StdLibInc\gm_ModeSetup.h"
#include "System\all.h"

#define HACTIVE_START 0x8

//******************************************************************************
//
// FUNCTION       : gmt_RET_STAT gm_InputSetHPosition(gmt_PHY_CH B_Channel)
//
// USAGE          : Sets the Horizontal Active Start Position for a channel
//                  + Validates Channel
//                  + Programs HStart
//                  + Adjusts HStart for UV Alignment
//
// INPUT          : B_Channel    -            CH_A or CH_B
//
// OUTPUT         : OK                -        Success
//                  ERR_CHANNEL    -            B_Channel is Invalid
//                  ERR_LIMIT        -        gmvsw_InputHSyncOffset[] is out of range
//
// GLOBALS        : gmvw_InputHStart         (R)
//                  gmvsw_InputHSyncOffset    (R)
//                  gmvw_InputSyncFlags        (R)
//
// USED_REGS      : IMP_HS_DELAY             (W)
//                  IMP_H_ACT_START            (W)
//                  IPP_HS_DELAY             (W)
//                  IPP_H_ACT_START         (W)
//
// PRE_CONDITION  : 
//
// POST_CONDITION : 
//
//******************************************************************************
gmt_RET_STAT gm_InputSetHPosition(gmt_PHY_CH B_Channel)
{
    gmt_RET_STAT B_Result;
    WORD W_Hstart;
    WORD W_Hdelay;
    
    if ((B_Result = gm_ValidatePhyCh(B_Channel)) != OK)
        return(B_Result);

    // Calculate H Start and H sync delay
    #if (FEATURE_IMP_LOCKUP_ISSUE_FIX == REV1)//this is to get around an IMP hangup issue
        if (gm_IsInputTripleADC(B_Channel) && gm_IsGraphics(B_Channel))
        {
            SWORD  SW_Offset = stInput[B_Channel].Hstart - HACTIVE_START + gmvsw_InputHSyncOffset[B_Channel];
            if (SW_Offset < 0)
                return (ERR_LIMIT);

            W_Hstart = HACTIVE_START;
            W_Hdelay = (WORD)SW_Offset;                
        }
        else
    #endif
    {
        W_Hdelay = 0;
        W_Hstart = stInput[B_Channel].Hstart;

      if (gm_IsInputTripleADC(B_Channel))
        {   // For shifting picture left HS_Delay is modifiing
            // For shifting image right HS_Activestart is modifying
            // this helps to avoid white vertical bar issue for CortezPlus and 
            //changing vertical position issue for CortezAdvanced and CortezPlus
            if (gmvsw_InputHSyncOffset[B_Channel] >= 0)
                W_Hdelay = gmvsw_InputHSyncOffset[B_Channel];
            else
            {
                W_Hstart += gmvsw_InputHSyncOffset[B_Channel];        
                if (W_Hstart >= 0x8000) //if negative value
                    return (ERR_LIMIT);
            }
        }

        if ( gm_IsVideo(B_Channel) )
        {   // If the IBD measurement is done with the DE signal, we want to align Hstart with the DE signal
            if ( gmvw_InputSyncFlags[B_Channel] & gmd_SYNC_UV_ALIGN_TO_DE ) 
            {   // we only do the alignment if the signal is 4:2:2 (not if 4:4:4)
                if ( gm_GetChannelInputAttr(B_Channel, gmd_DATA_WIDTH) == gmd_422)
                {
			#ifdef DELL_U2410F
			//Neil120810+ gmvw_InputHStartOrg[B_Channel] can't tell it's ODD/EVEN after calculate by above code, just check IBD for odd/even
			//Pass all video mode so far, 1080i(odd),480i(even)
			if(gm_ReadRegWord(ip_control[B_Channel])&IMP_EXT_DV_EN)		//U2413 set IMP_EXT_DV_EN	//align to DE
			{
	                    if (gm_ReadRegWord(ip_ibd_hstart[B_Channel])& 0x01) //if original hstart odd
	                        W_Hstart &= ~1;    //Odd Make Even
	                    else
	                        W_Hstart &= ~1;    //Even Make Even
			}
			else
			{
	                    if (gm_ReadRegWord(ip_ibd_hstart[B_Channel])& 0x01) //if original hstart odd
	                        W_Hstart |= 1;    // Make Odd
	                    else
	                        W_Hstart &= ~1;    // Make Even
			}
			#else
		            if ( gmvw_InputHStartOrg[B_Channel] & 0x01) //if original hstart odd
		                W_Hstart |= 1;    // Make Odd
		            else
		                W_Hstart &= ~1;    // Make Even
			#endif
                }
            }
            else
            // for other modes, we follow the fixed UV align specific to that mode
            {
		//gm_Printf("%d gm_GetChannelInputAttr(B_Channel, gmd_DATA_WIDTH) = %d" , B_Channel, gm_GetChannelInputAttr(B_Channel, gmd_DATA_WIDTH));
		//gm_Printf("====%d stInput[B_Channel].Hstart = %d" , B_Channel, stInput[B_Channel].Hstart);
		
		#ifdef EXTERNAL_DECODER//(DEFAULT_DIP_DAUGHTER_BOARD_OUT == TVP5160_YCbCr422)
			if(IsDecoderInput((B_Channel == CH_A)?UserPrefCurrentInputMain : UserPrefCurrentInputPip))
			{
				if (gmvw_InputSyncFlags[B_Channel] & gmd_SYNC_UV_ALIGN)
					W_Hstart |= 1;    // Make Odd
				else
					W_Hstart &= ~1;    // Make Even
			}
			else
			{
				if (gmvw_InputSyncFlags[B_Channel] & gmd_SYNC_UV_ALIGN)
					W_Hstart |= 1;	  // Make Odd
				else
					W_Hstart &= ~1;    // Make Even
			}
		#else	
			if (gmvw_InputSyncFlags[B_Channel] & gmd_SYNC_UV_ALIGN)
			    W_Hstart |= 1;    // Make Odd
			else
			    W_Hstart &= ~1;    // Make Even
  		#endif
            }
        }
    }
   
    #if (FEATURE_HDMI == ENABLE)
    if (gm_IsInputHdmiDvi(B_Channel))
	{
		if(!gm_IsCaptureCrop(B_Channel))
            W_Hstart = 0; // Switch On DV Mode
        else 	//Neil120815+ align to DE, only shift half of crop
			W_Hstart = (((stInput[B_Channel].Hactive*CROP_PERCENTAGE)/100)&0xfffc)/2;

   	}
    #endif

     #if (FEATURE_DISPLAY_PORT == ENABLE)
	 // 20090618 #1, For DP over Scan.
       if (gm_IsInputDP(B_Channel) )
 	{
		if(!gm_IsCaptureCrop(B_Channel)) 
			W_Hstart = 0; // Switch On DV Mode
		else
			W_Hstart = (((stInput[B_Channel].Hactive*CROP_PERCENTAGE)/100)&0xfffc)/2;
       }

    #endif
	//Hyb140610* for adjust panel protect function
	W_SavedInputHStart[B_Channel] = W_Hstart;
	#if 0//def UART_DEBUG_MODE
	gm_Print("11111111111111111stInput[B_Channel].Hstart = %d", stInput[B_Channel].Hstart);
	gm_Printf("2222222222222222W_Hdelay= %d,  W_Hstart= %d", W_Hdelay, W_Hstart);
	#endif
    gm_WriteRegWord(ip_hs_delay[B_Channel], W_Hdelay);
    gm_WriteRegWord(ip_h_act_start[B_Channel], W_Hstart);

    gm_SyncUpdate(B_Channel, INPUT_CLOCK);
    return (OK);
}


