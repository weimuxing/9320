/*
	$Workfile:   WssHandler.c  $
	$Revision: 1.6 $
	$Date: 2012/01/16 06:29:31 $
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
// MODULE:      WssHandler.c
//
// USAGE:       This module contains application level of WSS handler
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include <math.h>
#include "System\all.h"


//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************
#if (FEATURE_WSS == ENABLE)


//#define _DEBUG

#ifdef _DEBUG
#define  msg(a)            		gm_Print(a,0)
#define  msg_i(a,b)        	 	gm_Print(a,b)
#else
#define  msg(a)
#define  msg_i(a,b)
#endif

//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************

//******************************************************************************
//  V A R I A B L E S
//******************************************************************************

//*****************************************************************************
//  S T A T I C    F U N C T I O N    P R O T O T Y P E S
//*****************************************************************************
static void WssAutoAdjust(gmt_PHY_CH B_Channel, BOOL isWssValid);

//*****************************************************************************
//  C O D E
//*****************************************************************************

//*****************************************************************************
//  C O D E    D R I V E R
//*****************************************************************************


//******************************************************************************
//
// FUNCTION       : BOOL IsWssStatusValid(gmt_PHY_CH B_Channel, Wss_Mode W_Mode)
//
// USAGE          : Checks if WSS block status is valid
//
// INPUT          : B_Channel - CH_A or CH_B
//                     Note : The value of CH_A or CH_B is stored in
//                     gmvb_MainChannel and gmvb_PipChannel
//                  W_Mode
//
// OUTPUT         : TRUE or FALSE
//
// GLOBALS        :
//
// USED_REGS      : IMP_WSS_STATUS		(R)
//                  IMP_WSS_DATA_lo		(R)
//                  IPP_WSS_STATUS		(R)
//                  IPP_WSS_DATA_lo		(R)
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BOOL IsWssStatusValid(gmt_PHY_CH B_Channel, Wss_Mode W_Mode)
{
   BOOL status, Available;
   gmt_DECODER_ID B_DecId = gmd_DEC_CORTEZ_ID;

   Available = TRUE;

   if (!((IsDecoderUsed(B_Channel) == TRUE) ||	   		// Composite signal
         (GetChannelConnType(B_Channel) == COMPONENT)))// Component signal
   {
      Available = FALSE;							// Status not avail
   }

   status = gm_WssCheckStatus(B_Channel, Available);

   if (W_Mode == Wss_PAL)
   {
      if ((IsDecoderTimingPAL(B_DecId) == TRUE) && (stInput[gmvb_MainChannel].ModeFlags & gmd_MODE_VALID))
         return TRUE;
      else
         return FALSE;
   }
   else if (W_Mode == Wss_NTSC)
   {
      if ((GetChannelConnType(B_Channel)!= TUNER) && status == TRUE &&
            (stInput[B_Channel].ModeFlags & gmd_MODE_VALID))
         return TRUE;
      else
         return FALSE;

   }
   else if (W_Mode == Wss_MULTI)
   {
   }

   return FALSE;


}

//******************************************************************************
//
// FUNCTION       : void WssDetectSetup(gmt_PHY_CH B_Channel)
//
// USAGE          : Sets up and enables WSS block based on input mode
//
// INPUT          : B_Channel - CH_A or CH_B
//                     Note : The value of CH_A or CH_B is stored in
//                     gmvb_MainChannel and gmvb_PipChannel
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      : IMP_WSS_LINE_ODD	(W)
//                  IMP_WSS_LINE_EVEN	(W)
//                  IMP_WSS_CTRL1		(W)
//                  IMP_WSS_CTRL2		(W)
//                  IPP_WSS_LINE_ODD	(W)
//                  IPP_WSS_LINE_EVEN	(W)
//                  IPP_WSS_CTRL1		(W)
//                  IPP_WSS_CTRL2		(W)
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void WssDetectSetup(gmt_PHY_CH B_Channel)
{
   gmt_DECODER_ID B_DecId = gmd_DEC_CORTEZ_ID;
   gmt_MODE_TYPE B_CurrentModeType;

   //
   // Setup for CVBS/S-video
   //
   if (IsDecoderUsed(B_Channel) == TRUE)		// Composite signal
   {
      if (IsDecoderTimingPAL(B_DecId))			// PAL timing
      {										// Yes
         if (B_Channel == CH_A)
         {
#ifdef PAL_TV_SYSTEM
            gm_WssSetup(B_Channel, Wss_PAL);
#endif
         }
      }
      else									// Otherwise NTSC
      {
         gm_WssSetup(B_Channel, Wss_NTSC);
      }
   }
   else if (GetChannelConnType(B_Channel) == COMPONENT)
   {
      // jason: changed input paramaters to B_Channel to reduce complexity
      // jason: should make a table like modetable and search instead of if statements
      // TODO: 1. Check for modes 546i and 576p

      B_CurrentModeType = gm_GetCurrentInputModeType(B_Channel);

      switch (B_CurrentModeType)
      {
         case gmd_NTSC_480I:								//480i
            gm_WssSetup(B_Channel, Wss_480i);
            break;

         case gmd_480P:										//480p
            gm_WssSetup(B_Channel, Wss_480p);
            break;

         case gmd_PAL_576I:									// 576i
            gm_WssSetup(B_Channel, Wss_576i);
            break;

         case gmd_576P:										// 576p
            gm_WssSetup(B_Channel, Wss_576p);
            break;

         case gmd_720P:										// 720p@60Hz
            gm_WssSetup(B_Channel, Wss_720p);
            break;

         case gmd_1080I:										// 1080i@60Hz
            gm_WssSetup(B_Channel, Wss_1080i);
            break;

         default:
            return;
      }
   }


   gm_SyncUpdateWait(B_Channel, INPUT_OUTPUT_CLOCK);
   gm_WssEnableMeasure(B_Channel);
}

//*****************************************************************************
//  C O D E    H A N D L E R
//*****************************************************************************

// jason: changed function with input paramater gmt_PHY_CH B_Channel to simplify complexity
//******************************************************************************
//
// FUNCTION       : void WssStatusHandler(gmt_PHY_CH B_Channel)
//
// USAGE          : Monitors the WSS status from WSS block on both
//                  MAIN and PIP channel
//
// INPUT          : B_Channel
//
// OUTPUT         : None
//
// GLOBALS        : W_WssCrcc		(RW)
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void WssStatusHandler(gmt_PHY_CH B_Channel)
{
   BOOL valid = FALSE;

   gmt_DECODER_ID B_DecId = gmd_DEC_CORTEZ_ID;

// jason: update for the multi-system
// TODO: implement case for multi system tv
#ifdef MULTI_TV_SYSTEM
   // Check if status is valid
   if (IsWssStatusValid(B_Channel, Wss_MULTI))
      valid = TRUE;

   // jason: this just tells you the status of the Wss mode by a print statement (ex. aspect ratio)
   // 		not yet implemented
   gm_MonitorStatus(B_Channel,Wss_MULTI,valid);

#endif

   //
   // Check PAL WSS status (Main channel only)
   //

#if (defined (PAL_TV_SYSTEM) || defined (PAL_PROJECT))
   B_DecId++;
   // Check if status is valid
   if (IsWssStatusValid(B_Channel,Wss_PAL))
      valid = TRUE;

   // jason: this just tells you the status of the Wss mode by a print statement (ex. aspect ratio)
   //		it calls the function GetWss325Data to get the data
   gm_MonitorStatus(B_Channel,Wss_PAL,valid);

#endif

   //
   // Check NTSC WSS status
   //
   // Make sure timing is not PAL
   if (IsDecoderTimingPAL(B_DecId) == TRUE) 			// PAL timing
      return;

   // Check if status is valid
   if (IsWssStatusValid(B_Channel, Wss_NTSC))
      valid = TRUE;

   // jason: this just tells you the status of the Wss mode by a print statement (ex. aspect ratio)
   //		it calls the function gm_WssGetStatus to get the data
   gm_MonitorStatus(B_Channel, Wss_NTSC, valid);

   WssAutoAdjust(B_Channel, valid);
}

//******************************************************************************
//
// FUNCTION:   :  void WssAutoAdjust(gmt_PHY_CH B_Channel, BOOL isWssValid)
//
// USAGE    :  Adjust the aspect ratio based on WSS information.
//
// INPUT    :  B_Channel
//             isWssValid
//
// OUTPUT      :  None
//
// GLOBALS     :
//
//******************************************************************************

#define WSS_REQ_DISPLAY_CHANGE	700 	// User configurable for time required before changing aspect ratio

void WssAutoAdjust(gmt_PHY_CH B_Channel, BOOL isWssValid)
{
   WORD W_Data = 0;
   BYTE B_DetectedWssAspectRatio = WSS_UNKNOWN_ASPECT_RATIO;

   // Disable auto adjuster for other channel for now.
   if (B_Channel == gmvb_PipChannel)
      return;

   //
   // Enable WSS feature only if User has turned it on and when System is turned on (i.e. Power On)
   //
   if (UserPrefAutoAspectRatio && UserPrefPowerState)
   {
      if (isWssValid)
      {
         W_Data = gm_WssGetStatus(B_Channel, WSS_WORD0);

         B_DetectedWssAspectRatio = gm_WssDecode( W_Data);

         switch ( B_DetectedWssAspectRatio)
         {
            case WSS_16_9:
               if ( UserPrefAspectRatioMain != AR_FULL_SCREEN)
               {
                  UserPrefAspectRatioMain = AR_FULL_SCREEN;
                  msg ("Attempt to change to Full screen for WSS_16_9 data");
                  ModeSetupRequest(gmvb_MainChannel, REQ_MS_WSS, PARAM_NONE, WSS_REQ_DISPLAY_CHANGE);
                  OSDRequestMsg(B_Channel, MTO_AspectRatioUpdate, REQUEST_NOW);
               }
               break;

            case WSS_4_3_LETTER:
               if ( UserPrefAspectRatioMain != AR_LETTER_BOX_EXPAND)
               {
                  UserPrefAspectRatioMain = AR_LETTER_BOX_EXPAND;
                  msg ("Attempt to change to letterbox expand for WSS_4_3_LETTER data");
                  ModeSetupRequest(gmvb_MainChannel, REQ_MS_WSS, PARAM_NONE, WSS_REQ_DISPLAY_CHANGE);
                  OSDRequestMsg(B_Channel, MTO_AspectRatioUpdate, REQUEST_NOW);
               }
               break;

            case WSS_4_3:
               if ( UserPrefAspectRatioMain != AR_PILLAR_BOX)
               {
                  UserPrefAspectRatioMain = AR_PILLAR_BOX;
                  msg ("Attempt to change to pillar box expand for AR_PILLAR_BOX data");
                  ModeSetupRequest(gmvb_MainChannel, REQ_MS_WSS, PARAM_NONE, WSS_REQ_DISPLAY_CHANGE);
                  OSDRequestMsg(B_Channel, MTO_AspectRatioUpdate, REQUEST_NOW);
               }
               break;

            default:
               msg ("do nothing...");
               return;
         }
      }
   }
}

#endif // FEATURE_WSS

/*********************************  END  **************************************/

