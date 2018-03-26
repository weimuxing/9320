#define __SQA_DEBUGHANDLER_C__
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
// MODULE:      SQA_DebugHandler.c
//
// USAGE:       This module contains debug handler for application and library
//				debug messages used by SQA
//
//******************************************************************************



//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include <string.h>
#include "system\all.h"
#include "StdLibInc\gm_Register.h"


#define  msg(a)             gm_Printf(a,0)
#define  msg_i(a,b)         gm_Printf(a,b)

static BOOL SQA_IsDecoderUsed(ALL_BOARDS_INPUT_PORT B_Connector)
{
   if ((B_Connector == ALL_BOARDS_RF1) || (B_Connector == ALL_BOARDS_RF2) || (B_Connector == ALL_BOARDS_CVBS1)
         || (B_Connector == ALL_BOARDS_CVBS2) || (B_Connector == ALL_BOARDS_CVBS3) || (B_Connector == ALL_BOARDS_SVIDEO1)
         || (B_Connector == ALL_BOARDS_SVIDEO2) || (B_Connector == ALL_BOARDS_SVIDEO3) || (B_Connector == ALL_BOARDS_SIG1_SCART1)
         || (B_Connector == ALL_BOARDS_SIG2_SCART1) || (B_Connector == ALL_BOARDS_SIG3_SCART1)
         || (B_Connector == ALL_BOARDS_SIG4_SCART1) || (B_Connector == ALL_BOARDS_SIG1_SCART2)
         || (B_Connector == ALL_BOARDS_SIG2_SCART2) || (B_Connector == ALL_BOARDS_SIG3_SCART2)
         || (B_Connector == ALL_BOARDS_SIG4_SCART2) || (B_Connector == ALL_BOARDS_SIG1_SCART3)
         || (B_Connector == ALL_BOARDS_SIG2_SCART3) || (B_Connector == ALL_BOARDS_SIG3_SCART3)
         || (B_Connector == ALL_BOARDS_SIG4_SCART3))
      return TRUE;
   else
      return FALSE;
}

void SQA_DebugHandler(gmt_PHY_CH B_Channel)
{
   ALL_BOARDS_INPUT_PORT B_Connector = (B_Channel == gmvb_MainChannel)?UserPrefCurrentInputMain:UserPrefCurrentInputPip;
   static ALL_BOARDS_INPUT_PORT B_SavedConnector[2] = {0xFF, 0xFF};
   static gmt_DECODER_VIDEO_MODE B_SavedDecVideoMode[2] = {0xFF, 0xFF};
   gmt_DECODER_ID DecoderId = gmd_DEC_CORTEZ_ID;

   // Report only if channel is set up completely
   if (!gm_IsModeComplete(B_Channel))
      return;

#if 0
   if (SQA_IsDecoderUsed(B_Connector))
   {
      if ((B_SavedConnector[B_Channel] == B_Connector)
            && (B_SavedDecVideoMode[DecoderId] == gm_DecoderGetVideoMode(DecoderId)))
      {
         return;
      }
      else
      {
         B_SavedConnector[B_Channel] = B_Connector;
         B_SavedDecVideoMode[DecoderId] = gm_DecoderGetVideoMode(DecoderId);
      }
   }
   else
#endif
   {
      B_SavedDecVideoMode[DecoderId] = 0xFF;//reset to default

      if (B_SavedConnector[B_Channel] == B_Connector)
      {
         return;
      }
      else
      {
         B_SavedConnector[B_Channel] = B_Connector;
      }
   }

   if (B_Channel == gmvb_MainChannel)
   {
      msg("Main messages");
   }
   else if (B_Channel == gmvb_PipChannel)
   {
      msg("PIP messages");
   }
   else
   {
      msg("SQA_Debughander.c error: cannot identify main or pip");
      return;
   }

   if (SQA_IsDecoderUsed(B_Connector))
   {
      //  CVBS
      SQA_ModeDetectCVBS(B_Channel);
   }
   else if ((B_Connector == ALL_BOARDS_COMP1) || (B_Connector == ALL_BOARDS_COMP2) || (B_Connector == ALL_BOARDS_COMP3))
   {
      //   Component
      SQA_ModeDetectComponent(B_Channel);
   }
   else if (IsInternalHdmiDviPort(B_Channel))
      //else if ((B_Connector == ALL_BOARDS_DVI1) || (B_Connector == ALL_BOARDS_HDMI))
   {
      //   HDMI
      SQA_ModeDetectHDMI(B_Channel);
   }
   else if ((B_Connector == ALL_BOARDS_VGA1) || (B_Connector ==  ALL_BOARDS_VGA2))
   {
      //   VGA
      SQA_ModeDetectVGA(B_Channel);
   }
   else
   {
      msg("SQA_Debughander.c error:  Mode was NOT detected - NO port information available");
   }
}

void SQA_ModeDetectCVBS(gmt_PHY_CH B_Channel)
{
#if 0
   BYTE B_DecoderMode;
   gmt_DECODER_ID DecoderId = gmd_DEC_CORTEZ_ID;

   B_DecoderMode = gm_DecoderGetVideoMode(DecoderId);

   //Display Mode
   switch (B_DecoderMode)
   {
      case gmd_DEC_MODE_NTSC:
         msg("NTSC");
         break;

      case gmd_DEC_MODE_PAL:
         msg("PAL");
         break;

      case gmd_DEC_MODE_PAL_N:
         msg("PAL-N");
         break;

      case gmd_DEC_MODE_PAL_M:
         msg("PAL-M");
         break;

      case gmd_DEC_MODE_SECAM:
         msg("SECAM");
         break;

      case gmd_DEC_MODE_NTSC_443:
         msg("NTSC-443");
         break;

      case gmd_DEC_MODE_PAL_60:
         msg("PAL-60");
         break;

      default:
         msg("MODE UNKNOWN");
         break;
   }

   //Line Number
   if (gm_DecoderGetStatus(DecoderId, gmd_DEC_LINE525_625_MODE) == TRUE)
   {
      msg("525 Line");
   }
   else
   {
      msg("625 Line");
   }

   //3D or 2D Adaptive
   if (gm_DecoderGetStatus(DecoderId, gmd_DEC_3D_COMB_STATUS) == TRUE)
   {
      msg("3D Adaptive");
   }
   else
   {
      msg("2D Adaptive");
   }

   //Color Burst
   if (gm_DecoderGetStatus(DecoderId, gmd_DEC_COLOR_BURST_PRESENT) == TRUE)
   {
      msg("Color Burst Present");
   }
   else
   {
      msg("Color Burst Absent");
   }
#endif
}

void SQA_ModeDetectComponent(gmt_PHY_CH B_Channel)
{
   WORD W_Data =	 stInput[B_Channel].ModeFlags;

   // displaying Vertical Active
   msg_i("V Active %d", gmvw_InputVActiveOrg[B_Channel]);

   // Progressive/ Interlace
   if (gm_IsInterlaced(B_Channel))
   {
      msg("Interlace");
   }
   else
   {
      msg("Progressive");
   }

   // Vertical Freuency
   W_Data = stInput[B_Channel].Vfrq;
   msg_i("Vertical Frequency %d Hz", (int)(W_Data / 10));

}

void SQA_ModeDetectHDMI(gmt_PHY_CH B_Channel)
{
   WORD W_Data =	 stInput[B_Channel].ModeFlags;
   BYTE B_Connector;
#if (FEATURE_HDMI==ENABLE)
   HDMI_AVI_Y B_HdmiAviY;
#endif

#ifdef ANX9021_DEV_ID
   gmt_HDMI_SIG_FORMAT B_Format;

   if (ANX9021_IsSignalHDMI() && ANX9021_IsSignalStable())
      B_Format = ANX9021_GetSignalFormat();
   else
      B_Format = gmd_HDMI_RGB;
#endif

#if (FEATURE_HDMI==ENABLE)
   if (gm_IsHdmiTiming(gB_CurrentInputConnector[B_Channel]) && gm_HdmiIsStableState(gB_CurrentInputConnector[B_Channel]))
      B_HdmiAviY = gm_HdmiGetAviColorSpace(gB_CurrentInputConnector[B_Channel]);
   else
      B_HdmiAviY = HDMI_AVI_Y_RGB;
#endif

   if (B_Channel == gmvb_MainChannel)
      B_Connector = UserPrefCurrentInputMain;
   else
      B_Connector = UserPrefCurrentInputPip;

   // displaying Vertical Active
   msg_i("V Active %d", gmvw_InputVActiveOrg[B_Channel]);

   msg("Mode: Digital");


   // HDMI/DVI
   if (B_Connector == ALL_BOARDS_DVI1)
   {
      msg("Type: DVI");
   }
   else if (B_Connector == ALL_BOARDS_HDMI)
   {
      if (GetChannelConnType(B_Channel) == DVI)
      {
         msg("Type: DVI");
      }
      else
      {
         msg("Type: HDMI");
      }
   }

#if (FEATURE_HDMI==ENABLE)
   if (B_HdmiAviY == HDMI_AVI_Y_RGB)
   {
      msg("Video Type: RGB");
   }
   else if (B_HdmiAviY == HDMI_AVI_Y_YUV422)
   {
      msg("Video Type: YUV422");
   }
   else if (B_HdmiAviY == HDMI_AVI_Y_YUV444)
   {
      msg("Video Type: YUV444");
   }
   else
   {
      msg("Video Type: Unknown");
   }
#endif

   // Progressive/ Interlace
   if (gm_IsInterlaced(B_Channel))
   {
      msg("Interlace");
   }
   else
   {
      msg("Progressive");
   }

   // Vertical Frequency
   W_Data = stInput[B_Channel].Vfrq;
   msg_i("Vertical Frequency %d Hz", (int)(W_Data / 10));

}

void SQA_ModeDetectVGA(gmt_PHY_CH B_Channel)
{
   WORD W_Data =	 stInput[B_Channel].ModeFlags;

   // displaying Horizontal Active
   msg_i("H Active %d", gmvw_InputHActiveOrg[B_Channel]);

   // displaying Vertical Active
   msg_i("V Active %d", gmvw_InputVActiveOrg[B_Channel]);

   // Vertical Freuency
   W_Data = stInput[B_Channel].Vfrq;
   msg_i("Vertical Frequency %d Hz", (int)(W_Data / 10));

   // Sync
   if (gmvb_CurrentSyncState[B_Channel] == DSS_STATE) 	// Normal mode
   {
      msg("Sync: DSS");
   }
   else if (gmvb_CurrentSyncState[B_Channel] == NO_SYNC_STATE)          // No SYnc
   {
      msg("Sync: NO SYNC");
   }
   else if (gmvb_CurrentSyncState[B_Channel] == DCS_STATE)             // Composite Sync
   {
      msg("Sync: DCS");
   }
   else if (gmvb_CurrentSyncState[B_Channel] == SOG_STATE)             // SOG State
   {
      msg("Sync: SOG");
   }
   else if (gmvb_CurrentSyncState[B_Channel] == MAX_SYNC_TYPE)
   {
      msg("Sync: MAX sync");
   }
   else
   {
      msg("Sync: Not present!");
   }

   // RBG/YUV
   if (gm_GetChannelInputAttr(B_Channel, gmd_COLOR_SPACE) == gmd_RGB)
      msg("Video Type: RGB");
   else if (gm_GetChannelInputAttr(B_Channel, gmd_COLOR_SPACE) == gmd_YUVSD)
      msg("Video Type: YUVSD");
   else if (gm_GetChannelInputAttr(B_Channel, gmd_COLOR_SPACE) == gmd_YUVHD)
      msg("Video Type: gmd_YUVHD");
   else
      msg("Video Type: Unknown");

   // Polarity VSYNC
   if (stInput[B_Channel].ModeFlags & gmd_MODE_NEG_VSYNC)
   {
      msg("Polarity of VSync: Negative");
   }
   else
   {
      msg("Polarity of VSync: Positive");
   }
   // Polarity HSYNC
   if (stInput[B_Channel].ModeFlags & gmd_MODE_NEG_HSYNC)
   {
      msg("Polarity of HSync: Negative");
   }
   else
   {
      msg("Polarity of HSync: Positive");
   }

   // Auto Adjust Status is printed by SQA_ModeDetectVGA_AutoAdjust called by ModeSetup

}

void SQA_ModeDetectVGA_AutoAdjust(BOOL passed)
{
   //Auto Adjust Status
   if (passed)
      msg("Auto Adjust: Pass");
   else
      msg("Auto Adjust: Failed");
}

/***********************************  END  ************************************/

