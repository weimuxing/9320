/*
	$Workfile:   LetterBoxHandler.c  $
	$Revision: 1.5 $
	$Date: 2012/01/16 06:25:31 $
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
// MODULE:      LetterBoxHandler.c
//
// USAGE:       This module contains Letter Box detection handler to determine
//				whether input is full screen or letterbox.
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include <math.h>
#include "system\all.h"

//******************************************************************************
//  L O C A L    D E F I N I T I O N
//******************************************************************************

#ifdef USE_LETTERBOX_DETECTION

#define LETTER_BOX_TMO			100					  	// ms unit
#define DET_TIMEOUT				(3000/LETTER_BOX_TMO) 	// 3 seconds
#define NUM_LINES_DELTA			2					  	// line move +/-
#define LB_LINES_DELTA			20						// LB boundary
#define LB_DETECT_COUNT			60						// consistency count
#define TOP_WINDOW_LIMIT		60						// Boundary detect
#define BOTTOM_WINDOW_LIMIT		220						// Boundary detect

//
// LetterBox detection state
//
enum
{
   STATE_LB_IDLE,  			//
   STATE_LB_CHECKING,			//
   STATE_FULL_SCREEN,			//
   STATE_LETTER_BOX 			//
};

enum
{
   MODE_1_37,					// Film 		1:1.37
   MODE_1_78,					// WSTV 		1:1.78
   MODE_1_85,					// Acadamy 		1:1.85
   MODE_2_35,					// Anamorphic 	1:2.35
};

//******************************************************************************
//  S T A T I C    V A R I A B L E S
//******************************************************************************

static WORD W_State[CH_PHY_NUM] =
   {STATE_LB_IDLE, STATE_LB_IDLE};		// Current state
static WORD W_LetterBoxState[CH_PHY_NUM] =
   {STATE_LB_IDLE, STATE_LB_IDLE};		// Current state
static WORD W_PrevTime[CH_PHY_NUM];				// Prev system time
static WORD W_DetectTmo[CH_PHY_NUM] =
   {DET_TIMEOUT, DET_TIMEOUT};			// LB detection timeout (100ms)
static WORD W_DetectCounter[CH_PHY_NUM];		// Detected counter
static WORD W_DetectFirstLine[CH_PHY_NUM];		// Detected 1st line
static WORD W_DetectLastLine[CH_PHY_NUM]; 		// Detected last line
static WORD W_DetectRegion[CH_PHY_NUM]; 		// Detected region

//*****************************************************************************
//  S T A T I C    F U N C T I O N    P R O T O T Y P E S
//*****************************************************************************

static gmt_RET_STAT LetterBoxDetectConfig	(gmt_PHY_CH B_Channel);
static BYTE IsLetterBoxRegion				(gmt_PHY_CH B_Channel);

//*****************************************************************************
//  C O D E
//*****************************************************************************

//******************************************************************************
//
// FUNCTION       : gmt_RET_STAT LetterBoxDetectConfig(gmt_PHY_CH B_Channel)
//
// USAGE          : Configures the parameters for letterbox detection
//                  handler
//
// INPUT          : B_Channel - CH_A or CH_B
//
// OUTPUT         : OK
//                  ERR_CHANNEL
//
// GLOBALS        :
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
gmt_RET_STAT LetterBoxDetectConfig(gmt_PHY_CH B_Channel)
{
   if (B_Channel < CH_PHY_NUM)
   {
      W_State[B_Channel] 	   = STATE_LB_IDLE;		  	// Reset state
      W_LetterBoxState[B_Channel] = STATE_LB_IDLE;	//
      W_PrevTime[B_Channel]  = gm_GetSystemTimeW();	// Current system time
      W_DetectCounter[B_Channel] = 0;					//
      W_DetectFirstLine[B_Channel]=0;					// Detected 1st line
      W_DetectLastLine[B_Channel]=0;					// Detected last line
      return (OK);
   }
   else
      return (ERR_CHANNEL);
}

//******************************************************************************
//
// FUNCTION       : BOOL LetterBoxDetectStatus(gmt_PHY_CH B_Channel, WORD *W_Mode)
//
// USAGE          : Get current input (CH_A) letterbox detection status
//
// INPUT          : B_Channel - CH_A or CH_B
//                  W_Mode - letterbox ratio in multiple of 100
//							 e.g. 235 -> 1:2.35
//
// OUTPUT         : TRUE  - input is letterbox
//                  FALSE - input is fullscreen or unknown
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
BOOL LetterBoxDetectStatus(gmt_PHY_CH B_Channel, WORD *W_Mode)
{
   WORD W_ActiveRegion;
   DWORD D_Ratio;

   if (W_Mode == 0)						// NULL pointer
      return (FALSE);						// error

   if (B_Channel < CH_PHY_NUM)
   {
      if (W_State[B_Channel] == STATE_LETTER_BOX)	// In LetterBox state
      {
         if (gm_IsSDVideo(B_Channel) == TRUE)
            W_ActiveRegion = W_DetectRegion[B_Channel]*2;
         else
            W_ActiveRegion = W_DetectRegion[B_Channel];
         D_Ratio = (DWORD)(gmvw_InputHActiveOrg[B_Channel])*100;
         D_Ratio = (D_Ratio*9/W_ActiveRegion)/8;
         *W_Mode = (WORD)D_Ratio;
         return (TRUE);				  	// Yes
      }
      else
         return (FALSE);				   	// No
   }
   else
      return (FALSE);					  	// Wrong channel-default status
}

//******************************************************************************
//
// FUNCTION       : void LetterBoxSetup(gmt_PHY_CH B_Channel)
//
// USAGE          : Sets Letterbox detection window parameters
//
// INPUT          : B_Channel - CH_A or CH_B
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
void LetterBoxSetup(gmt_PHY_CH B_Channel)
{
   gmt_LB_WINDOW St_Param;

   if (gm_IsVideo(B_Channel)==TRUE)
   {
      if (gm_IsSDVideo(B_Channel) == TRUE)
      {
         if (gmvw_InputVTotalOrg[B_Channel] <= 280)			// NTSC
            gm_LetterBoxDetectInit(B_Channel, LB_480i);
         else 												// PAL
            gm_LetterBoxDetectInit(B_Channel, LB_576i);
      }
      else if (gm_IsEDVideo(B_Channel) == TRUE)
      {
         if (gmvw_InputVTotalOrg[B_Channel] <= 550)			// NTSC
            gm_LetterBoxDetectInit(B_Channel, LB_480p);
         else 												// PAL
            gm_LetterBoxDetectInit(B_Channel, LB_576p);
      }
      else
      {
         gm_LetterBoxDetectInit(B_Channel, LB_NO_SUPPORT);
         return;
      }
   }
   else
   {
      gm_LetterBoxDetectInit(B_Channel, LB_NO_SUPPORT);
      return;
   }
   LetterBoxDetectConfig(B_Channel);

   //
   // Setup capture/detection window
   //
   St_Param.W_TopHstart = gmvw_InputHStartOrg[B_Channel] +
                          (gmvw_InputHActiveOrg[B_Channel]/6);
   St_Param.W_TopHend 	 = gmvw_InputHStartOrg[B_Channel] +
                          (2*(gmvw_InputHActiveOrg[B_Channel]/3));
   St_Param.W_TopVstart = gmvw_InputVStartOrg[B_Channel];
   St_Param.W_TopVend   = gmvw_InputVStartOrg[B_Channel] + 80;
   St_Param.W_BottomHstart = gmvw_InputHStartOrg[B_Channel] +
                             (gmvw_InputHActiveOrg[B_Channel]/6);
   St_Param.W_BottomHend   = gmvw_InputHStartOrg[B_Channel] +
                             (2*(gmvw_InputHActiveOrg[B_Channel]/3));
   St_Param.W_BottomVstart = gmvw_InputVStartOrg[B_Channel] +
                             (gmvw_InputVActiveOrg[B_Channel] - 80);
   St_Param.W_BottomVend   = gmvw_InputVStartOrg[B_Channel] +
                             gmvw_InputVActiveOrg[B_Channel];

   gm_LetterBoxDetectSetup (B_Channel, &St_Param);
}

//******************************************************************************
//
// FUNCTION       : void LetterBoxDetectHandler(gmt_PHY_CH B_Channel)
//
// USAGE          : Gets current input Letter Box detection status
//
// INPUT          : B_Channel - CH_A or CH_B
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
void LetterBoxDetectHandler(gmt_PHY_CH B_Channel)
{
   BYTE B_Status;
   WORD W_CurrentTime;
   WORD W_Ratio;

   if (B_Channel >= CH_PHY_NUM)				// incorrect channel
      return;									// exit

   //
   // Check timeout for letterbox status
   //
   W_CurrentTime = gm_GetSystemTimeW();  		// Current system time
   if (W_PrevTime[B_Channel] > W_CurrentTime)	// Overflow ?
      W_CurrentTime = (0xFFFF - W_PrevTime[B_Channel]) + W_CurrentTime + 1;
   else
      W_CurrentTime -= W_PrevTime[B_Channel];	// Time elasped
   if (W_CurrentTime < LETTER_BOX_TMO)			// Over limit ?
   {											// Not yet
      return;									// exit, don't do anything
   }

   //
   // Save current system time
   // Get letterbox line number
   //
   W_PrevTime[B_Channel] = gm_GetSystemTimeW();			// Save current time
   //
   // Validate paremeters
   //

   switch (W_State[B_Channel])
   {
      case STATE_LB_IDLE:
         W_State[B_Channel] = STATE_LB_CHECKING;
         break;

      case STATE_LB_CHECKING:
         B_Status = IsLetterBoxRegion(B_Channel);
         if (B_Status == STATE_LB_IDLE)
            return;

         if (W_LetterBoxState[B_Channel] == B_Status)	// Same state
         {
            W_DetectCounter[B_Channel]++;
         }
         else
         {
            W_LetterBoxState[B_Channel] = B_Status;
            W_DetectCounter[B_Channel]  = 0;
         }
         if (W_DetectCounter[B_Channel] > LB_DETECT_COUNT)
         {
            W_State[B_Channel] = W_LetterBoxState[B_Channel];
            if (W_State[B_Channel] == STATE_LETTER_BOX)
            {
               LetterBoxDetectStatus(B_Channel,&W_Ratio);
               //gm_Print("LetterBox - %d",W_Ratio);
            }
            else
               ;//gm_Print("Full Screen",0);
         }
         break;

      case STATE_FULL_SCREEN:
         B_Status = IsLetterBoxRegion(B_Channel);
         if (B_Status == STATE_LB_IDLE)
            return;
         else if (B_Status == STATE_LETTER_BOX)
            W_DetectCounter[B_Channel]++;
         else
            W_DetectCounter[B_Channel]  = 0;

         if (W_DetectCounter[B_Channel] > LB_DETECT_COUNT)
         {
            W_State[B_Channel] = STATE_LETTER_BOX;
            LetterBoxDetectStatus(B_Channel,&W_Ratio);
            //gm_Print("LetterBox - %d",W_Ratio);
         }
         break;

      case STATE_LETTER_BOX:
         B_Status = IsLetterBoxRegion(B_Channel);
         if (B_Status == STATE_LB_IDLE)
            return;
         else if (B_Status == STATE_FULL_SCREEN)
            W_DetectCounter[B_Channel]++;
         else
            W_DetectCounter[B_Channel]  = 0;

         if (W_DetectCounter[B_Channel] > LB_DETECT_COUNT)
         {
            W_State[B_Channel] = STATE_FULL_SCREEN;
            //gm_Print("Full Screen",0);
         }
         break;

      default:
         break;
   }
}

//******************************************************************************
//
// FUNCTION       : BYTE IsLetterBoxRegion(gmt_PHY_CH B_Channel)
//
// USAGE          : Gets current input Letter Box detection state
//
// INPUT          : B_Channel - CH_A or CH_B
//
// OUTPUT         : STATE_LB_IDLE
//                  STATE_LETTER_BOX
//                  STATE_FULL_SCREEN
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
BYTE IsLetterBoxRegion(gmt_PHY_CH B_Channel)
{
   WORD W_Status, W_Data;
   WORD W_FirstActiveLine;
   WORD W_LastActiveLine;
   WORD W_ActiveRegion;

   W_Status = gm_LetterBoxDetectLine(B_Channel, &W_FirstActiveLine,
                                     &W_LastActiveLine);

   if ((W_Status != OK) ||
         (W_FirstActiveLine == 0x7FF || W_LastActiveLine == 0x7FF) ||
         (W_FirstActiveLine > TOP_WINDOW_LIMIT) ||
         (W_LastActiveLine < BOTTOM_WINDOW_LIMIT))
   {											// Not valid data region
      return (STATE_LB_IDLE);					// No valid data from HW
   }
   //
   // Detected 1st line
   //
   if (abs(W_FirstActiveLine - W_DetectFirstLine[B_Channel]) > NUM_LINES_DELTA)
   {
      W_DetectFirstLine[B_Channel] = W_FirstActiveLine;
//		gm_Print("1st line - %d",W_FirstActiveLine);
   }
   //
   // Detected last line
   //
   if (abs(W_LastActiveLine - W_DetectLastLine[B_Channel]) > NUM_LINES_DELTA)
   {
      W_DetectLastLine[B_Channel] = W_LastActiveLine;
//		gm_Print("last line - %d",W_LastActiveLine);
   }
   //
   // Determine region size
   //
   W_ActiveRegion = W_DetectLastLine[B_Channel] - W_DetectFirstLine[B_Channel];
   if (abs(W_ActiveRegion - W_DetectRegion[B_Channel]) > NUM_LINES_DELTA*2)
   {
      W_DetectRegion[B_Channel] = W_ActiveRegion;
//		gm_Print("Region - %d",W_ActiveRegion);
   }
   //
   // Determine LB or not
   //
   if (gm_IsSDVideo(B_Channel) == TRUE)
   {
      W_Data = gmvw_InputVActiveOrg[B_Channel]*2;
      W_ActiveRegion = W_DetectRegion[B_Channel]*2;
   }
   else
   {
      W_Data = gmvw_InputVActiveOrg[B_Channel];
      W_ActiveRegion = W_DetectRegion[B_Channel];
   }
   if (W_ActiveRegion < (W_Data - LB_LINES_DELTA))
      return (STATE_LETTER_BOX);
   else
      return (STATE_FULL_SCREEN);
}

#endif // USE_LETTERBOX_DETECTION

/*********************************  END  **************************************/

