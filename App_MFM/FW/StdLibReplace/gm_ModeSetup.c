/*
	$Workfile:   gm_ModeSetup.c  $
	$Revision: 1.29 $
	$Date: 2013/03/14 02:42:55 $
*/
#define __GM_MODESETUP_C__

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
// MODULE:      gm_ModeSetup.c
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#if 0
#include "LibInc\gm_Global.h"
#include "LibInc\gm_HostReg.h"
#include "LibInc\gm_Register.h"
#include "LibInc\gm_StdLib.h"
#include "LibInc\gm_ChipFeature.h"
#include "LibInc\gm_System.h"
#include "LibInc\gm_Clocks.h"
#include "LibInc\gm_InputConfigDefs.h"
#include "LibInc\gm_Input.h"
#include "LibInc\gm_GlobalVariables.h"

#include "LibInc\gm_ModeSetup.h"
#include "LibInc\gm_ModeDetect.h"
//#include "LibInc\gm_Dvi.h"
#include "LibInc\gm_HdmiInfo.h"
#else
#include "StdLibInc\gm_Global.h"
#include "StdLibInc\gm_HostReg.h"
#include "StdLibInc\gm_Register.h"
#include "StdLibInc\gm_StdLib.h"
#include "StdLibInc\gm_ChipFeature.h"
#include "StdLibApps\gm_System.h"
#include "StdLibInc\gm_Clocks.h"
#include "StdLibInc\gm_InputConfigDefs.h"
#include "StdLibInc\gm_Input.h"
#include "StdLibInc\gm_GlobalVariables.h"

#include "StdLibInc\gm_ModeSetup.h"
#include "StdLibInc\gm_ModeDetect.h"
//#include "LibInc\gm_Dvi.h"
#include "HdmiDvi\gm_HdmiInfo.h"

#endif

#include "System\all.h"


//******************************************************************************
//  D E F I N E S
//******************************************************************************

//#define _DEBUG

#ifdef _DEBUG
	#define msg(string)         gm_Print(string,0)
	#define msg_i(string,value) gm_Print(string,value)
#else
	#define msg(string)
	#define msg_i(string,value)
#endif

typedef enum
{
	gmd_CHANNEL_INPUT_DATA_WIDTH			= BIT0,
	gmd_CHANNEL_INPUT_COLOR_SPACE			= (BIT1 | BIT2),
	gmd_CHANNEL_INPUT_RGB_TO_YUV			= BIT3,
	gmd_CHANNEL_INPUT_BLACK_OFFSET			= BIT4,
	gmd_CHANNEL_INPUT_RGB_RANGE				= BIT5
} gmt_ChannelInputAttr;


//******************************************************************************
//  S T A T I C    V A R I A B L E S
//******************************************************************************

static gmt_ChannelInputAttr gB_ChannelInputAttr[CH_PHY_NUM];


//******************************************************************************
//  E X T E R N A L   V A R I A B L E S
//******************************************************************************
#ifdef DECODER_COMPONENT
extern gmt_ComponentRoute B_SDComponentMode;
#endif
//******************************************************************************
//  F U N C T I O N   P R O T O T Y P E S
//******************************************************************************
gmt_DPRX_DID            gm_GetDpDigitalID(gmt_UserDefConnNames B_InputConnector);

//*****************************************************************************
//  C O D E
//*****************************************************************************

//******************************************************************************
//
// FUNCTION       : void gm_SetChannelInputAttr(gmt_PHY_CH B_Channel, gmt_CHANNEL_INPUT_ATTR B_Option, WORD B_Value)
//
// USAGE          : Sets the channel input properties
//
// INPUT          : B_Channel - CH_A or CH_B for input channel
//                  B_Option - Property to set
//                  B_Value - Value
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
void gm_SetChannelInputAttr(gmt_PHY_CH B_Channel, gmt_CHANNEL_INPUT_ATTR B_Option, WORD B_Value)
{
	switch (B_Option)
	{
		case gmd_DATA_WIDTH:
			CLRBIT(gB_ChannelInputAttr[B_Channel], gmd_CHANNEL_INPUT_DATA_WIDTH);
			SETBIT(gB_ChannelInputAttr[B_Channel], ((B_Value << 0) & gmd_CHANNEL_INPUT_DATA_WIDTH));
			break;

		case gmd_COLOR_SPACE:
			CLRBIT(gB_ChannelInputAttr[B_Channel], gmd_CHANNEL_INPUT_COLOR_SPACE);
			SETBIT(gB_ChannelInputAttr[B_Channel], ((B_Value << 1) & gmd_CHANNEL_INPUT_COLOR_SPACE));
			break;

		case gmd_RGB_TO_YUV:
			CLRBIT(gB_ChannelInputAttr[B_Channel], gmd_CHANNEL_INPUT_RGB_TO_YUV);
			SETBIT(gB_ChannelInputAttr[B_Channel], ((B_Value << 3) & gmd_CHANNEL_INPUT_RGB_TO_YUV));
			break;

		case gmd_BLACK_OFFSET:
			CLRBIT(gB_ChannelInputAttr[B_Channel], gmd_CHANNEL_INPUT_BLACK_OFFSET);
			SETBIT(gB_ChannelInputAttr[B_Channel], ((B_Value << 4) & gmd_CHANNEL_INPUT_BLACK_OFFSET));
			break;

		case gmd_RGB_RANGE:			// CCIR861
			CLRBIT(gB_ChannelInputAttr[B_Channel], gmd_CHANNEL_INPUT_RGB_RANGE);
			SETBIT(gB_ChannelInputAttr[B_Channel], ((B_Value << 5) & gmd_CHANNEL_INPUT_RGB_RANGE));
			break;

		default:
			break;
	}
}


//******************************************************************************
//
// FUNCTION       : WORD gm_GetChannelInputAttr(gmt_PHY_CH B_Channel, gmt_CHANNEL_INPUT_ATTR B_Option)
//
// USAGE          : Gets the channel input properties
//
// INPUT          : B_Channel - CH_A or CH_B for input channel
//                  B_Option - Property to get
//
// OUTPUT         : Property value
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
WORD gm_GetChannelInputAttr(gmt_PHY_CH B_Channel, gmt_CHANNEL_INPUT_ATTR B_Option)
{
	WORD retval = 0;

	switch (B_Option)
	{
		case gmd_DATA_WIDTH:
			retval = (gB_ChannelInputAttr[B_Channel] & gmd_CHANNEL_INPUT_DATA_WIDTH) >> 0;
			break;

		case gmd_COLOR_SPACE:
			retval = (gB_ChannelInputAttr[B_Channel] & gmd_CHANNEL_INPUT_COLOR_SPACE) >> 1;
			break;

		case gmd_RGB_TO_YUV:
			retval = (gB_ChannelInputAttr[B_Channel] & gmd_CHANNEL_INPUT_RGB_TO_YUV) >> 3;
			break;

		case gmd_BLACK_OFFSET:
			retval = (gB_ChannelInputAttr[B_Channel] & gmd_CHANNEL_INPUT_BLACK_OFFSET) >> 4;
			break;

		case gmd_RGB_RANGE:			// CCIR861
			retval = (gB_ChannelInputAttr[B_Channel] & gmd_CHANNEL_INPUT_RGB_RANGE) >> 5;
			break;
			
		default:
			break;
	}

	return retval;
}

//******************************************************************************
//
// FUNCTION       : void gm_CopyChannelInputAttr(gmt_PHY_CH B_Target, gmt_PHY_CH B_Destination)
//
// USAGE          : Copy the channel input properties
//
// INPUT          : B_Target - Target
//                  B_Destination - Destination

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
void gm_CopyChannelInputAttr(gmt_PHY_CH B_Target, gmt_PHY_CH B_Destination)
{
	gB_ChannelInputAttr[B_Destination] = gB_ChannelInputAttr[B_Target];
}


//******************************************************************************
//
// FUNCTION       : void gm_SetupInputMain(void)
//
// USAGE          : Main()
//                  ModeSetupHdlr()
//                  ModeSetup()
//                  SetupChannel()
//                  SetCaptureMain()
//                  gm_SetupInputMain()
//                  Called by SetCaptureMain() to configure the IMP.
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : gmvw_InputHActive		(R/W)
//                  gmvw_InputModeAttr		(R)
//                  gmvb_MemoryBpp			(R)
//                  gmvb_CurrentSyncState	(R)
//
// USED_REGS      : IMP_CONTROL				(W)
//                  IMP_FLAGLINE			(W)
//                  IMP_VLOCK_EVENT			(W)
//                  IMP_V_ACT_START_ODD		(R)
//                  IMP_FRAME_RESET_LINE	(W)
//                  IMP_RFF_READY_LINE		(W)
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void gm_SetupInputMain(void)
{
	WORD			W_Control;
	WORD			W_Source;

	gm_ClearRegBitsWord(IMP_CONTROL, IMP_RUN_EN);

 	// Correct Width for YUV Input
	stInput[CH_A].Hactive = (stInput[CH_A].Hactive + 1) & ~1; // ceil at even

	// *** Control ****************

	// Preserve TNR setting
	#if (FEATURE_IMP_LOCKUP_ISSUE_FIX == REV2)
    	W_Control = gm_ReadRegWord(IMP_CONTROL) & (BIT6 |IMP_HS_DELAY_EN|IMP_HSVS_DEL_LIMIT_EN);
    #else
    	W_Control = gm_ReadRegWord(IMP_CONTROL) & (BIT6 |IMP_HS_DELAY_EN|IMP_CSC_OVERRIDE); // Enable IMP_CSC override via app 
    #endif
	
	W_Control |= IMP_RUN_EN; // Run Enable	

	if (gm_IsInterlaced(CH_A))
		W_Control |= IMP_INTLC_EN;

	// Set the input data width
	if (gm_GetChannelInputAttr(CH_A, gmd_DATA_WIDTH) == gmd_444)
		W_Control |= IMP_444_EN;

	// Set the color space
	if (gm_GetChannelInputAttr(CH_A, gmd_COLOR_SPACE) != gmd_RGB)
		{
		W_Control |= IMP_YUV_EN;

		// 20090210 #1, for DP YUV input ... Let MVF_Offset0 correct.
		#if 0 // (FEATURE_DISPLAY_PORT == ENABLE) // should not patch here
			if ( gm_IsInputDP(CH_A) )
				stInput[CH_A].ModeFlags |= gmd_MODE_HDVIDEO;
		#endif //(FEATURE_DISPLAY_PORT == ENABLE)
		}

	// Set color space conversion if required
	if (gm_GetChannelInputAttr(CH_A, gmd_RGB_TO_YUV))
		W_Control |= IMP_RGB_TO_YUV_EN;

	// Set black offset if required
	if (gm_GetChannelInputAttr(CH_A, gmd_BLACK_OFFSET))
		W_Control |= IMP_BLACK_OFFSET;

	// Set RGB signal range according CCIR861 stndard if required
	if (gm_GetChannelInputAttr(CH_A, gmd_RGB_RANGE) == gmd_HDMI_RGB_RANGE)
		W_Control |= IMP_CCIR861_EN;

#if (FEATURE_DISPLAY_PORT == ENABLE)
	if ( gm_IsInputDP(CH_A))// && (!gm_IsCaptureCrop(CH_A))) // Slave to DE
		W_Control |= IMP_EXT_DV_EN;
#endif //(FEATURE_DISPLAY_PORT == ENABLE)


#if (FEATURE_HDMI == ENABLE)
        if ( gm_IsInputHdmiDvi(CH_A) ) // Slave to DE
        {
            if (gm_IsHdmiTiming(gmvb_InputConnector[CH_A]) && (gm_HdmiGetAviPixelRepetition(gmvb_InputConnector[CH_A]) > HDMI_PIXEL_REPEAT_THRESHOLD))
                W_Control |= IMP_EXT_DV_EN;
	}
#endif

	// *** Source ****************
	W_Source = gm_ReadRegWord(IMP_SOURCE) & ~(IMP_VS_INV | IMP_HS_INV );

	// Invert HS and VS based on polarity only for DSS sync type
	if ((gm_IsInputTripleADC(CH_A)) && (gmvb_CurrentSyncState[CH_A] == DSS_STATE) ||  
		gm_IsInputHdmiDvi(CH_A)) 
	{
		if (stInput[CH_A].ModeFlags & gmd_MODE_NEG_HSYNC)
			W_Source |= IMP_HS_INV;
		if (stInput[CH_A].ModeFlags & gmd_MODE_NEG_VSYNC)
			W_Source |= IMP_VS_INV;
    }
	// 20090515 #1, Correct Polarity for DP.
	#if (FEATURE_DISPLAY_PORT == ENABLE) 
	if (gm_IsInputDP(CH_A))		
	{
		gmt_DPRX_DID b_DID=gm_GetDpDigitalID(gmvb_InputConnector[CH_A]);
		
		if ((gm_ReadRegWordDP12Rx(b_DID,DP12RX0_MS_HS_WIDTH) & DPRX_MS_HS_POLARITY))
			W_Source |= IMP_HS_INV;
	
		if ((gm_ReadRegWordDP12Rx(b_DID, DP12RX0_MS_VS_WIDTH) & DPRX_MS_VS_POLARITY))
			W_Source |= IMP_VS_INV; 
	}
	#endif
	// Invert Data Valid
#if (FEATURE_HDMI == ENABLE)
	if (gm_IsInputHdmiDvi(CH_A))
	{
		// If slaving to DE and VS is inverted, the IBD has to be read again without inverting VS.
		if (gm_IsHdmiTiming(gmvb_InputConnector[CH_A]) && (gm_HdmiGetAviPixelRepetition(gmvb_InputConnector[CH_A]) > HDMI_PIXEL_REPEAT_THRESHOLD) &&
			(W_Source & IMP_VS_INV))
		{
			W_Source &= ~IMP_VS_INV;
			gm_WriteRegWord(IMP_SOURCE, W_Source);
			gm_WriteRegWord(IMP_CONTROL, W_Control);
			W_Source |= IMP_VS_INV;
		}
		// if the previous mode had different VS/HS polarity, read the IBD again.
		else if (gm_ReadRegWord(IMP_SOURCE) != W_Source)
		{
			SWORD SW_HStart_diff;
			SWORD SW_VStart_diff;
			WORD W_HStart_before = gm_ReadRegWord(IMP_IBD_HSTART);     // Read H start;
			WORD W_VStart_before = gm_ReadRegWord(IMP_IBD_VSTART);     // Read V start;
            		gm_WriteRegWord(IMP_SOURCE, W_Source);
			gm_InputWaitVsync(CH_A, 2);

			//find the diff due to the change in VS/HS polarity
			SW_HStart_diff = gm_ReadRegWord(IMP_IBD_HSTART) - W_HStart_before;
			SW_VStart_diff = gm_ReadRegWord(IMP_IBD_VSTART) - W_VStart_before;
			//apply the diff to global
			stInput[CH_A].Hstart += SW_HStart_diff;
			stInput[CH_A].Vstart += SW_VStart_diff;
			gmvw_InputHStartOrg[CH_A] += SW_HStart_diff;
			gmvw_InputVStartOrg[CH_A] += SW_VStart_diff;
			//gm_WriteRegWord(IMP_SOURCE, W_Source);
		}
      
		// Not Force to DE when cropping ...
      //if(!gm_IsCaptureCrop(CH_A))		//Hyb130507- always align to DE even do crop;
      {
		W_Control |= IMP_EXT_DV_EN;
    	}     
   }        
#endif

	// Setup Window
	gm_InputSetCaptureWindow(CH_A);
	gm_SetLumaMeasureWindow();		// call it right after setting IMP capture window

	// VLock Event
        gm_WriteRegWord(IMP_VLOCK_EVENT, 
              (gm_ReadRegWord(IMP_V_ACT_START_ODD) + gm_ReadRegWord(IMP_V_ACT_LENGTH)  - 1));
//            (gm_ReadRegWord(IMP_V_ACT_START_ODD) + (gm_ReadRegWord(IMP_V_ACT_LENGTH) >> 4)));

#if (FEATURE_LUMA_CLAMP == ENABLE)
	// enable IMP luma calmping (64...943) when input signal is YUV
	if (gm_GetChannelInputAttr(CH_A, gmd_COLOR_SPACE) == gmd_RGB)
		gm_ClearRegBitsWord(IMP_MISC_CTRL, IMP_LUMA_CLAMP_EN);
	else
		gm_SetRegBitsWord(IMP_MISC_CTRL, IMP_LUMA_CLAMP_EN);
#endif
	// Program
	gm_WriteRegWord(IMP_SOURCE, W_Source);
	gm_WriteRegWord(IMP_CONTROL, W_Control);
}

//******************************************************************************
//
// FUNCTION       : void gm_SetupInputPip(void)
//
// USAGE          : Main()
//                  ModeSetupHdlr()
//                  ModeSetup()
//                  SetupPip()
//                  SetCapturePip()
//                  gm_SetupInputPip()
//                  Called by SetCapturePip() to configure the IPP.
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : gmvw_InputHActive		(R/W)
//                  gmvw_InputModeAttr		(R)
//                  gmvb_MemoryBpp			(R)
//                  gmvb_CurrentSyncState	(R)
//
// USED_REGS      : IPP_CONTROL				(W)
//                  IPP_VLOCK_EVENT			(W
//                  IPP_V_ACT_START_ODD		(R)
//                  IPP_FRAME_RESET_LINE	(W)
//                  IPP_SOURCE				(W)
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void gm_SetupInputPip(void)
{
	WORD			W_Control;
	WORD			W_Source;

	// Disable IPP
	gm_ClearRegBitsWord(IPP_CONTROL, IPP_RUN_EN);

 	// Correct Width for YUV Input
	stInput[CH_B].Hactive = (stInput[CH_B].Hactive + 1) & ~1; // ceil at even

	// *** Control ****************
#if (FEATURE_IMP_LOCKUP_ISSUE_FIX == REV2)
	W_Control = IPP_HSVS_DEL_LIMIT_EN;
#else
	W_Control = 0;
#endif

	// Run Enable
	W_Control |= IPP_RUN_EN;

	// Interlaced
	if (gm_IsInterlaced(CH_B) == TRUE)
	{
		W_Control |= IPP_INTLC_EN;
	}

	// Set the input data width
	if (gm_GetChannelInputAttr(CH_B, gmd_DATA_WIDTH) == gmd_444)
		W_Control |= IPP_444_EN;

	// Set the color space
	if (gm_GetChannelInputAttr(CH_B, gmd_COLOR_SPACE) != gmd_RGB)
		{
		W_Control |= IPP_YUV_EN;

		#if (FEATURE_DISPLAY_PORT == ENABLE)
			if ( gm_IsInputDP(CH_B) ) 
				stInput[CH_B].ModeFlags |= gmd_MODE_HDVIDEO;
		#endif //(FEATURE_DISPLAY_PORT == ENABLE)		
		}

	// Set color space conversion if required
	if (gm_GetChannelInputAttr(CH_B, gmd_RGB_TO_YUV))
		W_Control |= IPP_RGB_TO_YUV_EN;

	// Set black offset if required
	if (gm_GetChannelInputAttr(CH_B, gmd_BLACK_OFFSET))
		W_Control |= IPP_BLACK_OFFSET;

	// Set RGB signal range according CCIR861 stndard if required
	if (gm_GetChannelInputAttr(CH_B, gmd_RGB_RANGE) == gmd_HDMI_RGB_RANGE)
		W_Control |= IPP_CCIR861_EN;

#if (FEATURE_DISPLAY_PORT == ENABLE)
	if ( gm_IsInputDP(CH_B) ) // Slave to DE
		W_Control |= IPP_EXT_DV_EN;
#endif //(FEATURE_DISPLAY_PORT == ENABLE)
	
#if (FEATURE_HDMI == ENABLE)
	// Slave to DE
	if (gm_IsInputHdmiDvi(CH_B))
	{
		if (gm_IsHdmiTiming(gmvb_InputConnector[CH_B]) && (gm_HdmiGetAviPixelRepetition(gmvb_InputConnector[CH_B]) > HDMI_PIXEL_REPEAT_THRESHOLD))
			W_Control |= IPP_EXT_DV_EN;
	}
#endif

	// *** Source ****************

	W_Source = gm_ReadRegWord(IPP_SOURCE);

	// Invert HS and VS based on polarity only for DSS sync type
	if ((gm_IsInputTripleADC(CH_B)) && (gmvb_CurrentSyncState[CH_B] == DSS_STATE) ||  
		(gm_IsInputHdmiDvi(CH_B))) 
	{
		if (stInput[CH_B].ModeFlags & gmd_MODE_NEG_HSYNC)
		{
			W_Source |= IPP_HS_INV;
		}
		else
		{
			W_Source &= ~IPP_HS_INV;
		}

		// Invert VS
		if (stInput[CH_B].ModeFlags & gmd_MODE_NEG_VSYNC)
		{
			W_Source |= IPP_VS_INV;
		}
		else
		{
			W_Source &= ~IPP_VS_INV;
		}
	}
	else
	{
		W_Source &= ~(IPP_VS_INV | IPP_HS_INV );
	}


// 20090515 #1, Correct Polarity for DP.
#if (FEATURE_DISPLAY_PORT == ENABLE) 
	if (gm_IsInputDP(CH_B)) 	
	{
		gmt_DPRX_DID b_DID=gm_GetDpDigitalID(gmvb_InputConnector[CH_B]);
		
		if ((gm_ReadRegWordDP12Rx(b_DID,DP12RX0_MS_HS_WIDTH) & DPRX_MS_HS_POLARITY))
			W_Source |= IPP_HS_INV;

		if ((gm_ReadRegWordDP12Rx(b_DID, DP12RX0_MS_VS_WIDTH) & DPRX_MS_VS_POLARITY))
			W_Source |= IPP_VS_INV; 
	}
#endif

#if (FEATURE_HDMI == ENABLE)
	if (gm_IsInputHdmiDvi(CH_B))
	{
		// If slaving to DE and VS is inverted, the IBD has to be read again without inverting VS.
		if (gm_IsHdmiTiming(gmvb_InputConnector[CH_B]) && (gm_HdmiGetAviPixelRepetition(gmvb_InputConnector[CH_B]) > HDMI_PIXEL_REPEAT_THRESHOLD) &&
			(W_Source & IPP_VS_INV))
		{
			W_Source &= ~IPP_VS_INV;
			gm_WriteRegWord(IPP_SOURCE, W_Source);
			gm_WriteRegWord(IPP_CONTROL, W_Control);
			W_Source |= IPP_VS_INV;
		}
		// if the previous mode had different VS/HS polarity, read the IBD again.
		else if (gm_ReadRegWord(IPP_SOURCE) != W_Source)
		{
			SWORD SW_HStart_diff;
			SWORD SW_VStart_diff;
			WORD W_HStart_before = gm_ReadRegWord(IPP_IBD_HSTART);     // Read H start;
			WORD W_VStart_before = gm_ReadRegWord(IPP_IBD_VSTART);     // Read V start;
			gm_WriteRegWord(IPP_SOURCE, W_Source);
			gm_InputWaitVsync(CH_B, 2);

			//find the diff due to the change in VS/HS polarity
			SW_HStart_diff = gm_ReadRegWord(IPP_IBD_HSTART) - W_HStart_before;
			SW_VStart_diff = gm_ReadRegWord(IPP_IBD_VSTART) - W_VStart_before;
			//apply the diff to global
			stInput[CH_B].Hstart += SW_HStart_diff;
			stInput[CH_B].Vstart += SW_VStart_diff;
			gmvw_InputHStartOrg[CH_B] += SW_HStart_diff;
			gmvw_InputVStartOrg[CH_B] += SW_VStart_diff;
		}
	}
#endif

	// Invert Data Valid
	// TODO: Invert Data Valid Pip

	// Setup Window
	gm_InputSetCaptureWindow(CH_B);

	// VLock Event
	// For Sequoia - VLock Event = VStart (Odd) + VLength / 16
	gm_WriteRegWord(IPP_VLOCK_EVENT, (gm_ReadRegWord (IPP_V_ACT_START_ODD) + (gm_ReadRegWord (IPP_V_ACT_LENGTH) >> 4)));

#if (FEATURE_LUMA_CLAMP == ENABLE)
		// enable IPP luma calmping (64...943) when input signal is YUV
		if (gm_GetChannelInputAttr(CH_B, gmd_COLOR_SPACE) == gmd_RGB)
			gm_ClearRegBitsWord(IPP_MISC_CTRL, IPP_LUMA_CLAMP_EN);
		else
			gm_SetRegBitsWord(IPP_MISC_CTRL, IPP_LUMA_CLAMP_EN);
#endif

#if (FEATURE_HDMI == ENABLE)
// Switch On DV Mode
        if (gm_IsInputHdmiDvi(CH_B))
	{
            W_Control |= IPP_EXT_DV_EN;

            if (gm_IsInterlaced(CH_B) == TRUE)
                gmvw_InputSyncFlags[CH_B] |= gmd_SYNC_ODD_INV;
            else
                gmvw_InputSyncFlags[CH_B] &= ~gmd_SYNC_ODD_INV;
        }

#endif

	// Program
	gm_WriteRegWord(IPP_SOURCE, W_Source);
	gm_WriteRegWord(IPP_CONTROL, W_Control);
}

//******************************************************************************
//
// FUNCTION       : void gm_ClockSelection(gmt_PHY_CH B_Channel)
//
// USAGE          : Selects the IMP/IPP and SCLK clock sources
//
// INPUT          : B_Channel		-		CH_A or CH_B
//
// OUTPUT         : None
//
// GLOBALS        : gmvw_InputModeAttr		(RO)
//                  gmvw_InputOversample	(RO)
//                  B_SDComponentMode		(RO)
//
// USED_REGS      : None
//
// PRE_CONDITION  : gm_SourceClockSetup() has to prepare the gmvw_InputOversample() before this operation
//
// POST_CONDITION : None
//
//******************************************************************************
void gm_ClockSelection(gmt_PHY_CH B_Channel)
{
   gmt_CLK B_clk_hd = CLK_HD;

   if (gm_IsInputDigital(B_Channel))
      return;

   //
   // Input Mode Dependant Selects
   //
   switch (stInput[B_Channel].ModeFlags & gmd_MODE_SIGNAL_MASK)
   {
      case gmd_MODE_GRAPHICS:
         gm_ClkSelect(B_clk_hd, HD_CLK_SEL_SCLK);
         break;

      case gmd_MODE_HDVIDEO:
      case gmd_MODE_EDVIDEO:
         switch (gmvw_InputOversample[B_Channel])
         {
            case 2 :
               gm_ClkSelect(B_clk_hd, HD_CLK_SEL_SCLK_DIV2);
               break;
            
            case 4 :
               gm_ClkSelect(B_clk_hd, HD_CLK_SEL_SCLK_DIV4);
               break;
               
            default:
               gm_ClkSelect(B_clk_hd, HD_CLK_SEL_SCLK);
            #if FEATURE_AIP == ENABLE
               gm_AipHDGfxPathEnable(gmd_ADC_0, 0);
            #endif
               break;
         }
         break;

      case gmd_MODE_SDVIDEO:
         gm_ClkSelect(B_clk_hd, HD_CLK_SEL_SCLK_DIV4);
         break;
   }
}

//******************************************************************************
//
// FUNCTION       : void gm_SetDviEqPhase(gmt_PHY_CH B_Channel)
//
// USAGE          : Set the DVI eualization and phase.
//
// INPUT          : B_Channel	-	CH_A or CH_B
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  : None
//
// POST_CONDITION : None
//
//******************************************************************************
#if 0
//******************************************************************************
//
// FUNCTION       : void gm_SetComponentRouteMode(gmt_ComponentRoute B_Mode)
//
// USAGE          : Sets component routing configuration
//
// INPUT          : B_Mode			-		gmd_ComponentBypassDecoder
//                     gmd_ComponentThroughDecoder
//
// OUTPUT         : None
//
// GLOBALS        : B_SDComponentMode		(W)
//
// USED_REGS      : None
//
// PRE_CONDITION  : None
//
// POST_CONDITION : None
//
//******************************************************************************
void gm_SetComponentRouteMode(gmt_ComponentRoute B_Mode)
{
	B_SDComponentMode = B_Mode;
}

//******************************************************************************
//
// FUNCTION       : gmt_ComponentRoute gm_GetComponentRouteMode(void)
//
// USAGE          : Gets component routing configuration
//
// INPUT          : None
//
// OUTPUT         : gmt_ComponentRoute
//
// GLOBALS        : B_SDComponentMode		(W)
//
// USED_REGS      : None
//
// PRE_CONDITION  : None
//
// POST_CONDITION : None
//
//******************************************************************************
gmt_ComponentRoute gm_GetComponentRouteMode(void)
{
	return B_SDComponentMode;
}
#endif

/***********************************  END  ************************************/

