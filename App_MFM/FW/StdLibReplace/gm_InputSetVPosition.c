/*
            $Workfile:   gm_InputSetVPosition.c  $
            $Revision: 1.7 $
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
// MODULE:      gm_InputSetVPosition.c
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "StdLibInc\gm_Global.h"
#include "StdLibInc\gm_HostReg.h"
#include "StdLibInc\gm_Register.h"
#include "StdLibInc\gm_StdLib.h"
#include "StdLibInc\gm_GlobalVariables.h"
#include "StdLibInc\gm_ChipFeature.h"
#include "StdLibInc\gm_Input.h"
#include "System\all.h"

//******************************************************************************
//  D E F I N E S
//******************************************************************************

#define VACTIVE_START 0x4

//******************************************************************************
//  C O D E
//******************************************************************************

//******************************************************************************
//
// FUNCTION       : gmt_RET_STAT gm_InputSetVPosition(gmt_PHY_CH B_Channel)
//
// USAGE          : Sets the Vertical Active Start Position for a channel
//                  + Validates Channel
//                  + Programs Delay
//                  + Programs VStart (Odd/Even)
//                  + Program MIFM
//
// INPUT          : B_Channel        -        CH_A or CH_B
//
// OUTPUT         : OK                -         Success
//                  ERR_CHANNEL    -             B_Channel is Invalid
//                  ERR_LIMIT        -         gmvsw_InputVSyncOffset[] is out of range
//
// GLOBALS        : gmvw_InputVStart         (R)
//                  gmvsw_InputVSyncOffset    (R)
//                  gmvw_InputSyncFlags        (R)
//
// USED_REGS      : IMP_VS_DELAY             (W)
//                  IMP_V_ACT_START_ODD     (W)
//                  IMP_V_ACT_START_EVEN     (W)
//                  IPP_VS_DELAY            (W)
//                  IPP_V_ACT_START_ODD        (W)
//                  IPP_V_ACT_START_EVEN    (W)
//                  MIFM_CTRL                (R)
//                  PIFM_CTRL                (R)
//
// PRE_CONDITION  : 
//
// POST_CONDITION : 
//
//******************************************************************************
gmt_RET_STAT gm_InputSetVPosition(gmt_PHY_CH B_Channel)
{
    WORD W_Vstart;
    WORD W_VstartDV;
    WORD W_Vdelay;
   
    if (gm_ValidatePhyCh(B_Channel) != OK)
        return ERR_CHANNEL;

    // Calculate V Start and V sync delay    
    W_Vdelay = 0;
    W_Vstart = stInput[B_Channel].Vstart;

#if 0//def UART_DEBUG_MODE
   gm_Print(" == gm_InputSetVPosition: stInput[B_Channel].Vstart = %d ",stInput[B_Channel].Vstart);
#endif
   if (gm_IsInputTripleADC(B_Channel))
    {
        #if (FEATURE_IMP_LOCKUP_ISSUE_FIX == REV1) //this is to get around an IMP hangup issue
            W_Vstart = VACTIVE_START;
            W_Vdelay = stInput[B_Channel].Vstart - W_Vstart + gmvsw_InputVSyncOffset[B_Channel];
            if ((SWORD)W_Vdelay < 0)
                return ERR_LIMIT;
        #else
            if (gmvsw_InputVSyncOffset[B_Channel] >= 0)
                W_Vdelay = gmvsw_InputVSyncOffset[B_Channel];
            else
            {
                W_Vdelay = (gm_ReadRegWord(ip_ibd_vtotal[B_Channel])&0xFFFE) + gmvsw_InputVSyncOffset[B_Channel];
                if ((SWORD)W_Vdelay < 0)
                    return ERR_LIMIT;
            }
        #endif
    }
    
    W_VstartDV = W_Vstart;
    #if (FEATURE_HDMI == ENABLE)
    // Switch On DV Mode
    if (gm_IsInputHdmiDvi(B_Channel))
	{
		if(!gm_IsCaptureCrop(B_Channel))
        	W_VstartDV = 0;
		else		//Neil120815+ align to DE, only need shift half of crop
        	W_VstartDV = (((stInput[B_Channel].Vactive*CROP_PERCENTAGE)/100)&0xfffc)/2;
    }
    #endif

     #if (FEATURE_DISPLAY_PORT == ENABLE)
      // Switch On DV Mode
      // 20090618 #1, For DP over Scan.
       if ( gm_IsInputDP(B_Channel))
	{
		if( !gm_IsCaptureCrop(B_Channel) ) 
       	      W_VstartDV = 0;
		else		//Neil120815+ align to DE, only need shift half of crop
        	W_VstartDV = (((stInput[B_Channel].Vactive*CROP_PERCENTAGE)/100)&0xfffc)/2;

       }
    #endif
    
    // Set Position
    gm_WriteRegWord(ip_vs_delay[B_Channel], W_Vdelay);
    gm_WriteRegWord(ip_v_act_start_odd[B_Channel], W_VstartDV);
    gm_WriteRegWord(ip_v_act_start_even[B_Channel], 
        (gmvw_InputSyncFlags[B_Channel] & gmd_SYNC_EVEN_OFFSET) ? (W_VstartDV + 1) : W_VstartDV);

	
	//Hyb130513+ for 1080i@30Hz interlace mode image abnormal;
	if ( gm_IsInputDP(B_Channel))
	{
		if((stInput[B_Channel].ModeFlags&gmd_MODE_INTERLACED)&&(gm_GetCurrentInputModeType_New(B_Channel) == gmd_1080I))
			gm_WriteRegWord(ip_v_act_start_even[B_Channel], W_VstartDV);
	}
	
    if (B_Channel == CH_A)
    {    
        if (gm_IsInputHdmiDvi(CH_A) || gm_IsInputDP(CH_A))
        {
            gm_WriteRegWord(IMP_FRAME_RESET_LINE, 4);
            gm_WriteRegWord(IMP_RFF_READY_LINE, 5);    
        }
        else
        {
            gm_WriteRegWord(IMP_FRAME_RESET_LINE, (W_Vstart > 2) ? (W_Vstart - 2) : W_Vstart);
            gm_WriteRegWord(IMP_RFF_READY_LINE, (W_Vstart > 1) ? (W_Vstart - 1) : W_Vstart);    // ? Script was 0x015
        }        
        gm_SetIFMCtrlBitField(MIFM, gmd_IFM_INVERT_ODD_EVEN_FIELD_ID, 
            (gmvw_InputSyncFlags[CH_A] & gmd_SYNC_ODD_INV) ? TRUE : FALSE);

        if((GetPortConnType(gmvb_InputConnector[CH_A]) == VGA) && gm_IsGraphics(CH_A))
            gm_SetIFMCtrlBitField(MIFM, gmd_IFM_OFFSET_HSYNC, gm_IsInterlaced(CH_A) ? TRUE : FALSE);
    }
    else
    {
        gm_WriteRegWord(IPP_FRAME_RESET_LINE, 
            (gm_IsInputHdmiDvi(CH_B) || gm_IsInputDP(CH_B)) ? 4 : (W_Vstart > 2) ? (W_Vstart - 2) : W_Vstart);
        gm_SetIFMCtrlBitField(PIFM, gmd_IFM_INVERT_ODD_EVEN_FIELD_ID, 
            (gmvw_InputSyncFlags[CH_B] & gmd_SYNC_ODD_INV) ? TRUE : FALSE);
    }

    gm_SyncUpdate(B_Channel, INPUT_CLOCK);
    return (OK);
}


