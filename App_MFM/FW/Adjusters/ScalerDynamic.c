/*
   $Workfile:   ScalerDynamic.c  $
   $Revision: 1.13 $
   $Date: 2012/09/14 07:08:41 $
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
// MODULE:      ScalerDynamic.c
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "System\all.h"
#include "StdLibInc\gm_ACC3.h"

//******************************************************************************
//  E X T E R N S
//******************************************************************************

extern void CcDataSourceSwap(void);
//  D E F I N E S
//******************************************************************************

#define BITMASK_CHANNEL_A      1
#define BITMASK_CHANNEL_B      2

#define DYNAMIC_SCALE_ENABLE    1
#define DYNAMIC_SCALE_DISABLE    0

// Dynamic scaler steps in side by side (2 equals to one iteration)
// # of steps in one dynamic scaling sequence; this only applies to side-by-side mode
#define DYNAMIC_STEP         2

//#define NONLINEAR_DYNAMIC_SCALING_STEP
#ifdef NONLINEAR_DYNAMIC_SCALING_STEP
#define MAX_DYNAMIC_SCALING_INCREMENT 4 //max increment per iteration
#endif

// Limit of window sizes for side by side
#define SIDEBYSIDE_WIDTH_LIMIT      ((PanelWidth * 9) /10)
#define SIDEBYSIDE_HEIGHT_LIMIT      (PanelHeight - (2 * DYN_MAX_PIXELS_PER_SEQ))

//#define MAX(a,b)            ((a>b)?a:b)
#define MAX(a,b) (((a)>(b))?(a):(b))
#define ABS_DIFF(a,b)         ((a>b)?(a-b):(b-a))

//*****************************************************************************
//  S T R U C T S
//*****************************************************************************

typedef struct
{
   WORD W_NumberOfSteps;
   WORD W_BeginSizeH;
   WORD W_BeginSizeV;
   WORD W_EndSizeH;
   WORD W_EndSizeV;
   WORD W_BeginPosH;
   WORD W_BeginPosV;
   WORD W_EndPosH;
   WORD W_EndPosV;
} DYNAMIC_STRUCT;

//*****************************************************************************
//  G L O B A L S
//*****************************************************************************

BYTE   B_DynScalingDirection = DYN_IDLE;   // Dynamic scaling direction
BYTE   B_DynScalingCoast;               //
BYTE   B_DynPixelsPerSeq;               //
SWORD  SW_DynScalePosition = 0;               //

//******************************************************************************
//  L O C A L S    V A R I A B L E S
//******************************************************************************

static BYTE   B_DynScalRun[NUM_CHANNELS];
static DYNAMIC_STRUCT    St_DynScale[NUM_CHANNELS];

//******************************************************************************
//  L O C A L S    F U N C T I O N S
//******************************************************************************

static void DynamicScalerSetup      (gmt_PHY_CH B_Channel);
static BYTE SetDynamicScalerOutput   (gmt_PHY_CH B_Channel, WORD W_Index,
                                      WORD W_NumberOfSteps);
static BOOL CanDynamicScalingBeSkipped(DYNAMIC_STRUCT *S_DynScaleCh);
static WORD GetStartPosition(WORD W_PrevStart, WORD W_PrevActive, WORD W_CurrStart,
                             WORD W_CurrActive, WORD W_DesiredActive);
static WORD GetProportionalVActive(WORD W_PrevHActive, WORD W_CurrHActive,
                                   WORD W_PrevVActive,   WORD W_CurrVActive, WORD W_DesiredHActive);
static void WB_DependentFixes(gmt_PHY_CH B_Channel, BOOL Bt_CorrectStartOnly);
static void SetDynamicScalerToBypass(gmt_PHY_CH B_Channel,
                                     BOOL Bt_InputFixed, gmt_WindowStruct* WinPrevDim);
static WORD GetNextDynamicScalingIndex(BOOL Bt_IsNext, WORD W_CurrentIndex, WORD W_MaxStepNum);
static void DynamicScalerChangeFocusWindow(gmt_PHY_CH B_FocusWin);

//******************************************************************************
//  C O D E
//******************************************************************************

//******************************************************************************
//
// FUNCTION       : void DynamicScalerSideBySide(BYTE B_Command)
//
// USAGE          : Handle Dynamic scale on side-by-side pip from OSD control
//
// INPUT          : B_Command
//
// OUTPUT         : None
//
// GLOBALS        : B_DynScalingDirection
//                  B_DynPixelsPerSeq
//                  gmvb_MainChannel
//                  B_DynScalingCoast
//                  B_FocusBorder
//
// USED_REGS      : None
//
// PRE_CONDITION  : None
//
// POST_CONDITION : None
//
//******************************************************************************
void DynamicScalerSideBySide(BYTE B_Command)
{
   WORD W_Param;

   // Validate main and pip size
   // Stop increasing or pip when main or pip is maximum size
   if (((B_FocusBorder==gmvb_PipChannel) && (DOWN == B_Command))
         || ((B_FocusBorder==gmvb_MainChannel) && (UP == B_Command)))
   {
      if ((gmvw_OutputHActive[gmvb_MainChannel] >= SIDEBYSIDE_WIDTH_LIMIT) ||
            (gmvw_OutputVActive[gmvb_MainChannel] >= SIDEBYSIDE_HEIGHT_LIMIT))
      {
         return;
      }
   }
   else if (((B_FocusBorder==gmvb_MainChannel) && (DOWN == B_Command))
            || ((B_FocusBorder==gmvb_PipChannel) && (UP == B_Command)))
   {
      if ((gmvw_OutputHActive[gmvb_PipChannel] >= SIDEBYSIDE_WIDTH_LIMIT) ||
            (gmvw_OutputVActive[gmvb_PipChannel] >= SIDEBYSIDE_HEIGHT_LIMIT))
      {
         return;
      }
   }

   switch (B_Command)
   {
         // Up
      case UP:

         // Unfreeze main and pip for dynamic scale in side-by-side
         if ((BlenderIsPending(gmvb_MainChannel) == FALSE))
         {
            Freeze(gmvb_MainChannel, FALSE);
         }

         if ((BlenderIsPending(gmvb_PipChannel) == FALSE))
         {
            Freeze(gmvb_PipChannel, FALSE);
         }

         UserPrefFreeze = 0;
         OSDRequestMsg(gmvb_MainChannel, MTO_UnFreeze, REQUEST_NOW);


         // Start in new Move
         if (B_DynScalingDirection != DYN_DIR_INCREASE)
         {
            ModeSetupRequestCancel(gmvb_MainChannel, REQ_MS_DYNAMIC, PARAM_ANY);//remove all residual requests
            B_DynScalingDirection = DYN_DIR_INCREASE;
            B_DynPixelsPerSeq = DYN_MIN_PIXELS_PER_SEQ;
            W_Param = PARAM_DYN_START | (DYN_DIR_INCREASE << 8);
            ModeSetupRequest(gmvb_MainChannel, REQ_MS_DYNAMIC, W_Param, REQUEST_NOW);
         }

         // Extend current coast
         else if (B_DynScalingDirection == DYN_DIR_INCREASE)
         {
            ++B_DynPixelsPerSeq;
            if (B_DynPixelsPerSeq > DYN_MAX_PIXELS_PER_SEQ)
               B_DynPixelsPerSeq = DYN_MAX_PIXELS_PER_SEQ;
            B_DynScalingCoast = DYN_COAST;
         }

         break;

         // Down
      case DOWN:

         // Unfreeze main and pip for dynamic scale in side-by-side
         if ((BlenderIsPending(gmvb_MainChannel) == FALSE))
         {
            Freeze(gmvb_MainChannel, FALSE);
         }

         if ((BlenderIsPending(gmvb_PipChannel) == FALSE))
         {
            Freeze(gmvb_PipChannel, FALSE);
         }

         UserPrefFreeze = 0;
         OSDRequestMsg(gmvb_MainChannel, MTO_UnFreeze, REQUEST_NOW);


         // Start in new Move
         if (B_DynScalingDirection != DYN_DIR_DECREASE)
         {
            ModeSetupRequestCancel(gmvb_MainChannel, REQ_MS_DYNAMIC, PARAM_ANY);//remove all residual requests
            B_DynScalingDirection = DYN_DIR_DECREASE;
            B_DynPixelsPerSeq = DYN_MIN_PIXELS_PER_SEQ;
            W_Param = PARAM_DYN_START | (DYN_DIR_DECREASE << 8);
            ModeSetupRequest(gmvb_MainChannel, REQ_MS_DYNAMIC, W_Param, REQUEST_NOW);
         }

         // Extend current coast
         else if (B_DynScalingDirection == DYN_DIR_DECREASE)
         {
            ++B_DynPixelsPerSeq;
            if (B_DynPixelsPerSeq > DYN_MAX_PIXELS_PER_SEQ)
               B_DynPixelsPerSeq = DYN_MAX_PIXELS_PER_SEQ;
            B_DynScalingCoast = DYN_COAST;
         }

         break;

         // Right
      case RIGHT:
         B_FocusBorder = gmvb_PipChannel;
         DynamicScalerChangeFocusWindow(B_FocusBorder);
         break;

         // Left
      case LEFT:
         B_FocusBorder = gmvb_MainChannel;
         DynamicScalerChangeFocusWindow(B_FocusBorder);
         break;

      case SBS_DEFAULT:
         // Remove all residual requests
         ModeSetupRequestCancel(gmvb_MainChannel, REQ_MS_DYNAMIC, PARAM_ANY);

         // Do not perform scaling if already in default state
         if (SW_DynScalePosition < 0)
         {
            SW_DynScalePosition = 0;
            W_Param = PARAM_DYN_DEFAULT | (gmvb_PipChannel << 8);
            ModeSetupRequest(gmvb_MainChannel, REQ_MS_DYNAMIC, W_Param, REQUEST_NOW);
            W_Param = PARAM_DYN_DEFAULT | (gmvb_MainChannel << 8);
            ModeSetupRequest(gmvb_MainChannel, REQ_MS_DYNAMIC, W_Param, REQUEST_NOW);
         }
         else if (SW_DynScalePosition > 0)
         {
            SW_DynScalePosition = 0;
            W_Param = PARAM_DYN_DEFAULT | (gmvb_MainChannel << 8);
            ModeSetupRequest(gmvb_MainChannel, REQ_MS_DYNAMIC, W_Param, REQUEST_NOW);
            W_Param = PARAM_DYN_DEFAULT | (gmvb_PipChannel << 8);
            ModeSetupRequest(gmvb_MainChannel, REQ_MS_DYNAMIC, W_Param, REQUEST_NOW);
         }
         break;
   }
}

//******************************************************************************
//
// FUNCTION       : static void DynamicScalerChangeFocusWindow(gmt_PHY_CH B_FocusWin)
//
// USAGE          : Change focus window. This function will change the audio accordingly.
//
// INPUT          : B_FocusWin - CH_A or CH_B
//
// OUTPUT         : None
//
// GLOBALS        : B_MainProgramBlocked (R)
//                  B_PipProgramBlocked (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  : None
//
// POST_CONDITION : None
//
//******************************************************************************
static void DynamicScalerChangeFocusWindow(gmt_PHY_CH B_FocusWin)
{
//#ifdef INTERNAL_HDMI_PORT
//	BOOL B_AvsSourceChange = FALSE;
//	if ((IsInternalHdmiDviPort(gmvb_PipChannel) && !(IsInternalHdmiDviPort(gmvb_MainChannel))) ||
//		(IsInternalHdmiDviPort(gmvb_MainChannel) && !(IsInternalHdmiDviPort(gmvb_PipChannel))) )
//	{
//		B_AvsSourceChange = TRUE;
//	}
//#endif
#ifndef NO_PIP_SUPPORT
   SetPipBorder(B_FocusWin, TRUE);
   gm_SyncUpdate(B_FocusWin, OUTPUT_CLOCK);
#else
   UNUSED_PARAMETER(B_FocusWin);
#endif
}

//******************************************************************************
//
// FUNCTION       : void ShowDynamicScaler(BYTE B_Direction)
//
// USAGE          : In side by side mode it presents dynamic scaling feature,
//                  by shrinking and expanding main and pip
//
// INPUT          : B_Direction
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : MVF_CTRL
//                  MFRC_CTRL
//                  HOST_CONTROL
//
// PRE_CONDITION  : system is set to side by side main and pip
//
// POST_CONDITION : None
//
//******************************************************************************
void ShowDynamicScaler(BYTE B_Direction)
{
   // Make sure system is in dynamic scaling mode
   DynamicScalerStart(gmvb_MainChannel);
   DynamicScalerStart(gmvb_PipChannel);

   //
   // Setup MAIN & PIP display and start size
   //
   SetDynamicScalerSizeBegin(gmvb_MainChannel);
   SetDynamicScalerSizeBegin(gmvb_PipChannel);

   if (B_Direction == DYN_DIR_DECREASE)
   {
      if (gmvw_OutputVActive[gmvb_PipChannel] <
            (PanelHeight - (2 * B_DynPixelsPerSeq)))
      {
         SW_DynScalePosition -= B_DynPixelsPerSeq;
      }
   }
   else if (B_Direction == DYN_DIR_INCREASE)
   {
      if (gmvw_OutputVActive[gmvb_MainChannel] <
            (PanelHeight - (2 * B_DynPixelsPerSeq)))
      {
         SW_DynScalePosition += B_DynPixelsPerSeq;
      }
   }
   else if (B_Direction == DYN_DIR_RESET)
      SW_DynScalePosition = 0;
   else
   {
      gm_Printf("Invalid Dynamic option",0);
      return;                  // Error input don't do anything
   }

   //
   // Setup MAIN & PIP display and end size
   //
   SetDisplayMain();
#ifndef NO_PIP_SUPPORT   
   SetDisplayPip();
#endif
//   SetPipBorder(gmvb_MainChannel, TRUE);
//   SetPipBorder(gmvb_PipChannel, TRUE);
   SetDynamicScalerSizeEnd(gmvb_MainChannel);
   SetDynamicScalerSizeEnd(gmvb_PipChannel);

   //
   // Setup Main channel
   // Setup Pip channel
   //
   DynamicScalerSetup(gmvb_MainChannel);
   DynamicScalerSetup(gmvb_PipChannel);
   DynamicScalerProcess(TRUE);
}

//******************************************************************************
//
// FUNCTION       : static WORD GetNextDynamicScalingIndex(BOOL Bt_IsNext,
//                           WORD W_CurrentIndex, WORD W_MaxStepNum)
//
// USAGE          : Generate the next index for dynamic scaling sequence.
//
// INPUT          :   Bt_IsNext - TRUE means requesting the next index; FALSE means
//                        retrieving the previous index
//               W_CurrentIndex - current index; this is needed for calculating the
//                           next index; for retrieving the previous index
//                           a static variable is used to keep track of it.
//               W_MaxStepNum - max index in the sequence. This is used to
//                           calculate the next index.
//
// OUTPUT         : Return the new index
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
static WORD GetNextDynamicScalingIndex(BOOL Bt_IsNext, WORD W_CurrentIndex,
                                       WORD W_MaxStepNum)
{
#ifndef NONLINEAR_DYNAMIC_SCALING_STEP

//
// Linear Trajectory
//
// Step         |
// Incremented   |-------------------
// Per         |
// Iteration   |
//            ================
//                     time

   W_MaxStepNum = W_MaxStepNum;

   if (Bt_IsNext)
      W_CurrentIndex++;
   else if (W_CurrentIndex > 0)
      W_CurrentIndex--;
#else
//
// Ramp Trajectory
//
// Step         |
// Incremented   |  --------------
// Per         | /              \                                       .
// Iteration   |/                \                                      .
//            ================
//                     time
//
// The following code implemented a trajectory similar to the above trajectory in that
// there is a plateau region (limited by #define MAX_DYNAMIC_SCALING_INCREMENT).
// However, instead of the above straight ramp-up/ramp-down region, an exponential curve is
// implemented for those regions in the following code. Visually there should be minimal
// differences between the linear and the exponential ramp regions due to the relatively
// low plateau region. Please note that to see good effect the number of dynamic scaling steps
// should be at least 10.

   static WORD W_PreviousIndex = 0;

   //reset W_PreviousIndex to zero for new dynamic scaling sequence
   if ((W_CurrentIndex <= W_PreviousIndex)&&(W_CurrentIndex == 1))
      W_PreviousIndex = 0;

   if (Bt_IsNext)
   {
      if (W_PreviousIndex < W_CurrentIndex)
      {
         W_PreviousIndex = W_CurrentIndex;
      }

      if (W_CurrentIndex >= W_MaxStepNum)
      {
         W_CurrentIndex = W_MaxStepNum + 1;
      }
      else if (W_CurrentIndex < W_MaxStepNum/2)
      {
         if (W_CurrentIndex > MAX_DYNAMIC_SCALING_INCREMENT)
            W_CurrentIndex += MAX_DYNAMIC_SCALING_INCREMENT;
         else if (W_CurrentIndex == 0)
            W_CurrentIndex++;
         else
            W_CurrentIndex *= 2;
      }
      else
      {
         WORD W_Diff = W_MaxStepNum - W_CurrentIndex;

         if (W_Diff/2 > 0)
         {
            if (W_Diff/2 > MAX_DYNAMIC_SCALING_INCREMENT)
               W_CurrentIndex += MAX_DYNAMIC_SCALING_INCREMENT;
            else
               W_CurrentIndex += W_Diff/2;
         }
         else
            W_CurrentIndex++;
      }

   }
   else
   {
      W_CurrentIndex = W_PreviousIndex;
   }
#endif

   return W_CurrentIndex;
}

//******************************************************************************
//
// FUNCTION       : void DynamicScalerProcess(BOOL Bt_SyncBothWinSize)
//
// USAGE          : After populating St_DynScaleChx structure this function will
//                  perform complete dynamic scaling operation including setting
//                  up input output memory and scaler for number of steps
//                  (given within the structure).
//
// INPUT          : Bt_SyncBothWinSize - TRUE when syncing both window sizes is
//                              desired; FALSE for no syncing
//
// OUTPUT         : None
//
// GLOBALS        : St_DynScale
//                  gmvw_OutputHActive
//                  gmvw_OutputVActive
//                  gmvw_OutputHStart
//                  gmvw_OutputVStart
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DynamicScalerProcess(BOOL Bt_SyncBothWinSize)
{
   DYNAMIC_STRUCT *S_DynScaleMain, *S_DynScalePip;
   BYTE B_ChBitMask;
   BOOL Bt_IndexCorrectionMain, Bt_IndexCorrectionPip;
   BOOL Bt_CanFinishEarly;
   BOOL Bt_Acc3En;
   WORD W_Index;
   WORD W_NumberOfSteps;
   WORD W_NumberOfStepsMain = 0;
   WORD W_NumberOfStepsPip = 0;

   //
   // Setup Dynamic Scale structure and steps
   //
   S_DynScaleMain = &St_DynScale[gmvb_MainChannel];
   S_DynScalePip = &St_DynScale[gmvb_PipChannel];

   //Disable ACC3 to avoid picture flashing during dinamic scaling.
#if (FEATURE_ACC3 == ENABLE)
   {
      Bt_Acc3En = gm_ACC3_IsProcessingEnabled(TRUE);
      gm_ACC3StopISR();
   }
#endif

   if (S_DynScalePip != 0)
   {
      W_NumberOfStepsPip= S_DynScalePip->W_NumberOfSteps;
      if (W_NumberOfStepsPip == 0)
         S_DynScalePip = 0;
   }

   if (S_DynScaleMain != 0)
   {
      W_NumberOfStepsMain = S_DynScaleMain->W_NumberOfSteps;
      if (W_NumberOfStepsMain == 0)
         S_DynScaleMain = 0;
   }

   // Determine if scaling can be skipped
   Bt_CanFinishEarly = CanDynamicScalingBeSkipped(S_DynScalePip);
   if (Bt_CanFinishEarly)   //not to overwrite result from above
      Bt_CanFinishEarly = CanDynamicScalingBeSkipped(S_DynScaleMain);

   if ((!gm_IsValidSignal(gmvb_MainChannel))&&(!gm_IsValidSignal(gmvb_PipChannel)))
   {
      Bt_CanFinishEarly = TRUE;
      W_NumberOfSteps = 1;
   }
   else
      W_NumberOfSteps = MAX(W_NumberOfStepsMain,W_NumberOfStepsPip);

   //
   // Process each dynamic scale steps
   //
   for (W_Index = 1; W_Index <= W_NumberOfSteps;
         W_Index = GetNextDynamicScalingIndex(TRUE, W_Index, W_NumberOfSteps))
   {
      B_ChBitMask = 0;
      Bt_IndexCorrectionMain = FALSE;
      Bt_IndexCorrectionPip = FALSE;

      // to skip over unneccessary steps; skip only if neither channel needs to scale
      if (Bt_CanFinishEarly)
         W_Index = W_NumberOfSteps;

      if (S_DynScaleMain||Bt_SyncBothWinSize)
      {
         Bt_IndexCorrectionMain = SetDynamicScalerOutput(gmvb_MainChannel, W_Index, W_NumberOfSteps);
      }

      if (S_DynScalePip||Bt_SyncBothWinSize)
      {
         Bt_IndexCorrectionPip = SetDynamicScalerOutput(gmvb_PipChannel, W_Index, W_NumberOfSteps);
      }

      //Modify window size to compensate for 1:1 scaling step of the other window
      if (Bt_SyncBothWinSize)
      {
         if (Bt_IndexCorrectionMain && Bt_IndexCorrectionPip)
         {
            WB_DependentFixes(gmvb_MainChannel, TRUE);
            WB_DependentFixes(gmvb_PipChannel, TRUE);
         }
         else if (Bt_IndexCorrectionMain)
         {
            WB_DependentFixes(gmvb_PipChannel, FALSE);
         }
         else if (Bt_IndexCorrectionPip)
         {
            WB_DependentFixes(gmvb_MainChannel, FALSE);
         }
         else
         {
            WB_DependentFixes(gmvb_PipChannel, FALSE);
         }
      }

      //
      // Process MAIN window
      //
      if (S_DynScaleMain||Bt_SyncBothWinSize)
      {
         //change memory only if signal is valid
         if (gm_IsValidSignal(gmvb_MainChannel))
         {
            B_ChBitMask = (gmvb_MainChannel == CH_A)?BITMASK_CHANNEL_A:BITMASK_CHANNEL_B;
         }
         gm_DisplaySetSize(gmvb_MainChannel);
         gm_DisplaySetPosition(gmvb_MainChannel);
#ifndef NO_PIP_SUPPORT         
         SetPipBorder(gmvb_MainChannel, TRUE);
#endif         
      }
      //
      // Process PIP window
      //
#ifndef NO_PIP_SUPPORT
      if (S_DynScalePip||Bt_SyncBothWinSize)
      {
         //change memory only if signal is valid
         if (gm_IsValidSignal(gmvb_PipChannel))
         {
            B_ChBitMask |= (gmvb_MainChannel == CH_A)?BITMASK_CHANNEL_B:BITMASK_CHANNEL_A;
         }
         gm_DisplaySetSize(gmvb_PipChannel);
         gm_DisplaySetPosition(gmvb_PipChannel);
         SetPipBorder(gmvb_PipChannel, TRUE);
      }
#endif

      if (B_ChBitMask)
         gm_ScalerDynamic(B_ChBitMask);

      if ((Bt_IndexCorrectionMain || Bt_IndexCorrectionPip)&&(!Bt_CanFinishEarly))
      {
         W_Index = GetNextDynamicScalingIndex(FALSE, W_Index, W_NumberOfSteps);
      }

      if (W_NumberOfSteps > W_Index)
      {
	  if(gm_IsModeComplete(CH_A)&&(S_DynScaleMain != 0))
      		gm_ScalerDynamicWait(NUM_VSYNCS);
	  if(gm_IsModeComplete(CH_B)&&(S_DynScalePip != 0))
      		gm_ScalerDynamicWait(NUM_VSYNCS);	  
      }
   }

//restore ACC3 functioning after dinamic scaling completion.
#if (FEATURE_ACC3 == ENABLE)
   if (Bt_Acc3En)
   {
	if(gm_IsModeComplete(CH_A))
      gm_WaitForDisplaySync(2);
      gm_ACC3StartISR();
   }
#endif

}

//******************************************************************************
//
// FUNCTION       : static BOOL CanDynamicScalingBeSkipped(DYNAMIC_STRUCT *S_DynScaleCh)
//
// USAGE          : Return if dynamic scaling is done early.
//
// INPUT          : S_DynScaleCh
//
// OUTPUT         : TRUE if finished early; otherwise, FALSE
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
static BOOL CanDynamicScalingBeSkipped(DYNAMIC_STRUCT *S_DynScaleCh)
{
   if (S_DynScaleCh == NULL_PTR)
      return TRUE;
   else if (S_DynScaleCh->W_NumberOfSteps == 0)
      return TRUE;
   else if ((S_DynScaleCh->W_BeginPosH == S_DynScaleCh->W_EndPosH ) &&
            (S_DynScaleCh->W_BeginPosV == S_DynScaleCh->W_EndPosV ) &&
            (S_DynScaleCh->W_BeginSizeH == S_DynScaleCh->W_EndSizeH) &&
            (S_DynScaleCh->W_BeginSizeV == S_DynScaleCh->W_EndSizeV))
      return TRUE;
   else
      return FALSE;
}

//******************************************************************************
//
// FUNCTION       : static WORD GetStartPosition(WORD W_PrevStart, WORD W_PrevActive,
//                     WORD W_CurrStart, WORD W_CurrActive, WORD W_DesiredActive)
//
// USAGE          : Return the [H|V]Start value corresponding to the desired active by
//                 interpolation on values from the current and previous frame.
//
// INPUT          : W_PrevStart - previous [H|V]Start location
//               W_PrevActive - previous [H|V]Active
//               W_CurrStart - new [H|V]Start location
//               W_CurrActive - new [H|V]Active
//               W_DesiredActive - desired [H|V]Active
//
// OUTPUT         : WORD
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
static WORD GetStartPosition(WORD W_PrevStart, WORD W_PrevActive, WORD W_CurrStart,
                             WORD W_CurrActive, WORD W_DesiredActive)
{
   WORD W_PrevEnd = W_PrevStart+W_PrevActive;
   WORD W_CurrEnd = W_CurrStart+W_CurrActive;
   SDWORD SDW_TrickyTerm1;
   SDWORD SDW_TrickyTerm2;
   SDWORD SDW_Result;

   SDW_TrickyTerm1 = (SDWORD)W_PrevEnd-W_DesiredActive;
   SDW_TrickyTerm2 = ABS_DIFF(W_CurrStart,W_PrevStart);

   if ((W_CurrEnd<W_PrevEnd)&&(W_CurrStart<W_PrevStart))
   {
      SDW_TrickyTerm1 = -SDW_TrickyTerm1;
      SDW_TrickyTerm2 = -SDW_TrickyTerm2;
   }
   else if ((W_CurrEnd>W_PrevEnd)&&(W_CurrStart>W_PrevStart))
   {
      SDW_TrickyTerm1 = -SDW_TrickyTerm1;
      SDW_TrickyTerm2 = -SDW_TrickyTerm2;
   }
   //prevent division by zero
   if ((ABS_DIFF(W_PrevEnd,W_CurrEnd)+SDW_TrickyTerm2) == 0)
   {
      return (W_CurrStart+W_PrevStart)/2;//this is the best guess
   }
   else
   {
      SDW_Result = (ABS_DIFF(W_PrevEnd,W_CurrEnd)*(SDWORD)W_PrevStart+ABS_DIFF(W_CurrStart,W_PrevStart)*SDW_TrickyTerm1)
                   /(ABS_DIFF(W_PrevEnd,W_CurrEnd)+SDW_TrickyTerm2);

      if (SDW_Result < 0)
         return 0;
      else
         return (WORD)SDW_Result;
   }
}

//******************************************************************************
//
// FUNCTION       : static WORD GetProportionalVActive(WORD W_PrevHActive, WORD W_CurrHActive,
//                  WORD W_PrevVActive, WORD W_CurrVActive, WORD W_DesiredHActive)
//
// USAGE          : Return the [H|V]Start value corresponding to the desired active by
//                 interpolation on values from the current and previous frame.
//
// INPUT          : W_PrevHActive - previous [H|V]Start location
//               W_CurrHActive - previous [H|V]Active
//               W_PrevVActive - new [H|V]Start location
//               W_CurrVActive - new [H|V]Active
//               W_DesiredHActive - desired [H|V]Active
//
// OUTPUT         : WORD
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
static WORD GetProportionalVActive(WORD W_PrevHActive, WORD W_CurrHActive, WORD W_PrevVActive,
                                   WORD W_CurrVActive, WORD W_DesiredHActive)
{
   WORD W_Term1;

   //prevent division by zero
   if (W_CurrHActive == W_PrevHActive)
      return W_CurrVActive;

   W_Term1 = (WORD)((DWORD)ABS_DIFF(W_PrevVActive,W_CurrVActive)
                    * ABS_DIFF(W_DesiredHActive, W_PrevHActive)
                    /(DWORD)ABS_DIFF(W_CurrHActive, W_PrevHActive));

   if (W_CurrVActive > W_PrevVActive)
      return W_PrevVActive + W_Term1;
   else
      return (W_PrevVActive > W_Term1)?W_PrevVActive - W_Term1:0;
}

//******************************************************************************
//
// FUNCTION       : BYTE SetDynamicScalerOutput(gmt_PHY_CH B_Channel, WORD W_Index,
//                                       WORD W_NumberOfSteps)
//
// USAGE          : Setup the output of dynamic scaling
//
// INPUT          : B_Channel
//                  W_Index
//                  W_NumberOfSteps
//
// OUTPUT         : Correction index
//
// GLOBALS        : B_DynScalRun      (R)
//                  gmvw_OutputHActive   (W)
//                  gmvw_OutputVActive   (W)
//                  gmvw_OutputHStart   (W)
//                  gmvw_OutputVStart   (W)
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BYTE SetDynamicScalerOutput(gmt_PHY_CH B_Channel, WORD W_Index,
                            WORD W_NumberOfSteps)
{
   DYNAMIC_STRUCT *S_DynScale;
   gmt_WindowStruct WinPrevDim;
   BOOL Bt_BypassStepInserted;
   static BOOL Bt_IsFirstIteration[NUM_CHANNELS] = {TRUE, TRUE};

   S_DynScale = &St_DynScale[B_Channel];

   if (S_DynScale == NULL_PTR) return 0;

   //if run for the first time, set W_PrevHStart, W_PrevVStart, W_PrevHActive and W_PrevVActive to
   //  the current size of the window
   if ((W_Index == 1)&&(Bt_IsFirstIteration[B_Channel]))
   {
      WinPrevDim.W_Hstart = S_DynScale->W_BeginPosH;
      WinPrevDim.W_Vstart = S_DynScale->W_BeginPosV;
      WinPrevDim.W_Width = S_DynScale->W_BeginSizeH;
      WinPrevDim.W_Height = S_DynScale->W_BeginSizeV;
      Bt_IsFirstIteration[B_Channel] = FALSE;

      //if input == output and scaler is not in bypass, put scaler in bypass first
      if (((gm_GetScalerVMode(B_Channel) != SCALER_BYPASS)&&
            (stInput[B_Channel].Vactive == S_DynScale->W_BeginSizeV))
            ||((gm_GetScalerHMode(B_Channel) != SCALER_BYPASS)&&
               (stInput[B_Channel].Hactive == S_DynScale->W_BeginSizeH)))
      {
         SetDynamicScalerToBypass(B_Channel, TRUE, &WinPrevDim);
      }
   }
   //otherwise, use the values from the previous iteration
   else
   {
      WinPrevDim.W_Hstart = gmvw_OutputHStart[B_Channel];
      WinPrevDim.W_Vstart = gmvw_OutputVStart[B_Channel];
      WinPrevDim.W_Width = gmvw_OutputHActive[B_Channel];
      WinPrevDim.W_Height = gmvw_OutputVActive[B_Channel];
   }

   if (W_Index == W_NumberOfSteps)
   {
      gmvw_OutputHActive[B_Channel] = S_DynScale->W_EndSizeH;
      gmvw_OutputVActive[B_Channel] = S_DynScale->W_EndSizeV;
      gmvw_OutputHStart[B_Channel]  = S_DynScale->W_EndPosH;
      gmvw_OutputVStart[B_Channel]  = S_DynScale->W_EndPosV;
   }
   else
   {
      gmvw_OutputHActive[B_Channel] =
         (WORD)((SDWORD)S_DynScale->W_BeginSizeH -
                (((S_DynScale->W_BeginSizeH - (SDWORD)S_DynScale->W_EndSizeH) * (SDWORD)W_Index)
                 / (SWORD)W_NumberOfSteps));

      gmvw_OutputVActive[B_Channel] =
         (WORD)((SDWORD)S_DynScale->W_BeginSizeV -
                (((S_DynScale->W_BeginSizeV - (SDWORD)S_DynScale->W_EndSizeV) * (SDWORD)W_Index)
                 / (SWORD)W_NumberOfSteps));

      gmvw_OutputHStart[B_Channel] =
         (WORD)((SDWORD)S_DynScale->W_BeginPosH -
                (((S_DynScale->W_BeginPosH - (SDWORD)S_DynScale->W_EndPosH) * (SDWORD)W_Index)
                 / (SWORD)W_NumberOfSteps));

      gmvw_OutputVStart[B_Channel] =
         (WORD)((SDWORD)S_DynScale->W_BeginPosV -
                (((S_DynScale->W_BeginPosV - (SDWORD)S_DynScale->W_EndPosV) * (SDWORD)W_Index)
                 / (SWORD)W_NumberOfSteps));
   }
   //
   // Validate Input Capture and Display Windows (ceil at even)
   //
   stInput[B_Channel].Hactive  = (stInput[B_Channel].Hactive + 1)  & (~1);
   gmvw_OutputHActive[B_Channel] = (gmvw_OutputHActive[B_Channel] + 1) & (~1);

   Bt_BypassStepInserted = IsDynamicScalerBypassStepInserted(B_Channel, TRUE, &WinPrevDim);

   //For the following transitions, setting scaler to 1:1 scaling factor before/after entering scaler bypass is critical; otherwise, buffer underrun
   //  will happen on higher resolution panels
   //This extra logic applies to the following transitions:
   //  1. BYPASS to ZOOM
   //The transitions that are NOT included due to speed concern (too many steps will make choppy sequence) and implementation difficulties are:
   //   1. BYPASS to SHRINK
   //   2. SHRINK to BYPASS
   //   3. ZOOM to BYPASS
   if (((gm_GetScalerHMode(B_Channel) == SCALER_BYPASS) && (gmvw_OutputHActive[B_Channel] > stInput[B_Channel].Hactive))
         || ((gm_GetScalerVMode(B_Channel) == SCALER_BYPASS) && (gmvw_OutputVActive[B_Channel] > stInput[B_Channel].Vactive)))
   {
      WORD W_TempOutputHActive = gmvw_OutputHActive[B_Channel];
      WORD W_TempOutputVActive = gmvw_OutputVActive[B_Channel];

      gm_ScalerDynamicEnable(B_Channel, FALSE);
      gmvw_OutputHActive[B_Channel] = WinPrevDim.W_Width;
      gmvw_OutputVActive[B_Channel] = WinPrevDim.W_Height;
      gm_ScalerSetup(B_Channel);
         gm_WaitForDisplaySync(1); // if absent it causes flash on screen
      gm_SyncUpdateWait(B_Channel, INPUT_OUTPUT_CLOCK);
      gm_ScalerDynamicEnable(B_Channel, TRUE);
      //restore original desired output HActive
      gmvw_OutputHActive[B_Channel] = W_TempOutputHActive;
      gmvw_OutputVActive[B_Channel] = W_TempOutputVActive;
   }

   if ((gmvw_OutputHActive[B_Channel] == S_DynScale->W_EndSizeH)
         &&(gmvw_OutputVActive[B_Channel] == S_DynScale->W_EndSizeV)
         &&(gmvw_OutputHStart[B_Channel] == S_DynScale->W_EndPosH)
         &&(gmvw_OutputVStart[B_Channel] == S_DynScale->W_EndPosV))
   {
      Bt_IsFirstIteration[B_Channel] = TRUE;//prepare for another new dynamic scaling sequence
   }

   // Make sure (HStart+HActive) <= PanelWidth and (VStart+VActive) <= PanelHeight
   if (gmvw_OutputHStart[B_Channel] + gmvw_OutputHActive[B_Channel] > PanelWidth)
   {
      if ((gmvw_OutputHStart[B_Channel] > 0) && (PanelWidth >= gmvw_OutputHActive[B_Channel]))
         gmvw_OutputHStart[B_Channel] = PanelWidth - gmvw_OutputHActive[B_Channel];
      else
      {
         gmvw_OutputHStart[B_Channel] = 0;
         gmvw_OutputHActive[B_Channel] = PanelWidth;
      }
   }
   if (gmvw_OutputVStart[B_Channel] + gmvw_OutputVActive[B_Channel] > PanelHeight)
   {
      if ((gmvw_OutputVStart[B_Channel] > 0) && (PanelHeight >= gmvw_OutputVActive[B_Channel]))
         gmvw_OutputVStart[B_Channel] = PanelHeight - gmvw_OutputVActive[B_Channel];
      else
      {
         gmvw_OutputVStart[B_Channel] = 0;
         gmvw_OutputVActive[B_Channel] = PanelHeight;
      }
   }
   return Bt_BypassStepInserted;
}

//******************************************************************************
//
// FUNCTION       : static void WB_DependentFixes(gmt_PHY_CH B_Channel,
//                        BOOL Bt_CorrectStartOnly)
//
// USAGE          : Fix variables with the knowledge of the side-by-side PIP layout
//               in the reference design.
//
// INPUT          : B_Channel - CH_A or CH_B
//               Bt_CorrectStartOnly - TRUE if only [H|V]Start can be modified
//                              for syncing the sizes for both windows;
//                              FALSE if all [H|V]Start and [H|V]Active
//                              can be modified for syncing;
//
// OUTPUT         : None
//
// GLOBALS        : gmvw_OutputHActive[]   (R/W)
//                  gmvw_OutputVActive[]   (R/W)
//                  gmvw_OutputHStart[]      (R/W)
//                  gmvw_OutputVStart[]      (R/W)
//               B_AspectMethod[]      (W)
//
// USED_REGS      :
//
// PRE_CONDITION  : This function should be run at the very end of the code that
//                  generates the location and size of the output window.
//
// POST_CONDITION :
//
//******************************************************************************
static void WB_DependentFixes(gmt_PHY_CH B_Channel, BOOL Bt_CorrectStartOnly)
{
   WORD W_DesiredOutputHActive;

   if (IsSideBySidePipMode(UserPrefPipMode))
   {
      //Perform these fixes only if both Main and Pip horizontally are full
      if (((B_AspectMethod[gmvb_MainChannel] == AR_FULL_SCREEN)
            || (B_AspectMethod[gmvb_MainChannel] == AR_LETTER_BOX)
            || (B_AspectMethod[gmvb_MainChannel] == AR_LETTER_BOX_EXPAND))
            && ((B_AspectMethod[gmvb_PipChannel] == AR_FULL_SCREEN)
                || (B_AspectMethod[gmvb_PipChannel] == AR_LETTER_BOX)
                || (B_AspectMethod[gmvb_PipChannel] == AR_LETTER_BOX_EXPAND)))
      {
         if (Bt_CorrectStartOnly)
         {
            if (B_Channel == gmvb_MainChannel)
               gmvw_OutputHStart[gmvb_MainChannel] = PIP_BORDER_WIDTH;
            else
            {
               gmvw_OutputHStart[gmvb_PipChannel] = PanelWidth - PIP_BORDER_WIDTH - gmvw_OutputHActive[gmvb_PipChannel];
               gmvw_OutputHStart[gmvb_PipChannel] &= ~1;//pip always start at even to prevent shaky highlight border
            }
            gmvw_OutputVStart[B_Channel] = (PanelHeight - gmvw_OutputVActive[B_Channel])/2;
         }
         else
         {
            if (B_Channel == gmvb_PipChannel)
            {
               W_DesiredOutputHActive = PanelWidth - PIP_BORDER_WIDTH*3 - gmvw_OutputHActive[gmvb_MainChannel];
               W_DesiredOutputHActive &= ~1;
               gmvw_OutputHStart[gmvb_PipChannel] = PanelWidth - PIP_BORDER_WIDTH - W_DesiredOutputHActive;
               gmvw_OutputHStart[gmvb_PipChannel] &= ~1;//pip always start at even to prevent shaky highlight border
               gmvw_OutputVActive[gmvb_PipChannel]
               = (WORD)(gmvw_OutputVActive[gmvb_PipChannel] * (DWORD)W_DesiredOutputHActive
                        /gmvw_OutputHActive[gmvb_PipChannel]);
               gmvw_OutputHActive[gmvb_PipChannel] = W_DesiredOutputHActive;
               gmvw_OutputVStart[gmvb_PipChannel] = (PanelHeight - gmvw_OutputVActive[gmvb_PipChannel])/2;
            }
            else
            {
               gmvw_OutputHStart[gmvb_PipChannel] &= ~1;//pip always start at even to prevent shaky highlight border

//               W_DesiredOutputHActive = PanelWidth - PIP_BORDER_WIDTH*3 - gmvw_OutputHActive[gmvb_PipChannel];
               W_DesiredOutputHActive = gmvw_OutputHStart[gmvb_PipChannel] - PIP_BORDER_WIDTH*2;
               W_DesiredOutputHActive &= ~1;
               gmvw_OutputHStart[gmvb_MainChannel] = PIP_BORDER_WIDTH;
               gmvw_OutputVActive[gmvb_MainChannel]
               = (WORD)(gmvw_OutputVActive[gmvb_MainChannel]   * (DWORD)W_DesiredOutputHActive
                        /gmvw_OutputHActive[gmvb_MainChannel]);
               gmvw_OutputHActive[gmvb_MainChannel] = W_DesiredOutputHActive;
               gmvw_OutputVStart[gmvb_MainChannel] = (PanelHeight - gmvw_OutputVActive[gmvb_MainChannel])/2;
            }
         }
      }
   }
}

//******************************************************************************
//
// FUNCTION       : BOOL IsDynamicScalerBypassStepInserted(gmt_PHY_CH B_Channel,
//                 BOOL Bt_InputFixed, gmt_WindowStruct* pWinPrevDim)
//
// USAGE          : Return if bypass step needs to be inserted; if TRUE, the varying
//                 dimension (either input or output, determined by Bt_InputFixed)
//                 will be changed as well.
//
// INPUT          : B_Channel - CH_A or CH_B
//               Bt_InputFixed - if TRUE, means that input dimension is the reference
//                           (fixed) and output dimension can be changed;
//                           otherwise, output dimension is fixed and the input
//                           dimension can be changed
//               pWinPrevDim - previous window dimension; used to generate the
//                          dimension for the bypass step
//
// OUTPUT         : TRUE if bypass step needs to be inserted and the varying dimension is
//                 modified; otherwise, FALSE meaning the varying dimension is not modified
//                 and bypass step is not needed
//
// GLOBALS        : gmvw_OutputHActive   (W)
//                  gmvw_OutputVActive   (W)
//                  gmvw_OutputHStart   (W)
//                  gmvw_OutputVStart   (W)
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BOOL IsDynamicScalerBypassStepInserted(gmt_PHY_CH B_Channel, BOOL Bt_InputFixed,
                                       gmt_WindowStruct* pWinPrevDim)
{
   BOOL Bt_CorrectionH = FALSE;
   BOOL Bt_CorrectionV = FALSE;
   BYTE B_IndexCorrection = 0;
   WORD * Wp_FixedHActive;
   WORD * Wp_FixedVActive;
   WORD * Wp_VaryingHStart;
   WORD * Wp_VaryingHActive;
   WORD * Wp_VaryingVStart;
   WORD * Wp_VaryingVActive;

   if (!gm_IsValidSignal(B_Channel)) return 0;

   if (Bt_InputFixed)
   {
      Wp_FixedHActive = &stInput[B_Channel].Hactive;
      Wp_FixedVActive = &stInput[B_Channel].Vactive;
      Wp_VaryingHStart = &gmvw_OutputHStart[B_Channel];
      Wp_VaryingHActive = &gmvw_OutputHActive[B_Channel];
      Wp_VaryingVStart = &gmvw_OutputVStart[B_Channel];
      Wp_VaryingVActive = &gmvw_OutputVActive[B_Channel];
   }
   else
   {
      Wp_FixedHActive = &gmvw_OutputHActive[B_Channel];
      Wp_FixedVActive = &gmvw_OutputVActive[B_Channel];
      Wp_VaryingHStart = &stInput[B_Channel].Hstart;
      Wp_VaryingHActive = &stInput[B_Channel].Hactive;
      Wp_VaryingVStart = &stInput[B_Channel].Vstart;
      Wp_VaryingVActive = &stInput[B_Channel].Vactive;
   }

   //
   // Check if last was zoom and new is shrink then put system
   // in bypass in extra step
   //
   if (((gm_GetScalerHMode(B_Channel) == SCALER_ZOOM) && (gmvw_OutputHActive[B_Channel] < stInput[B_Channel].Hactive))
         ||((gm_GetScalerHMode(B_Channel) == SCALER_SHRINK) && (gmvw_OutputHActive[B_Channel] > stInput[B_Channel].Hactive)))
   {
      //GetStartPosition() and GetProportionalVActive() will fail if either value is zero
      if ((gmvw_OutputHActive[B_Channel] > 0)&&(stInput[B_Channel].Hactive > 0))
         Bt_CorrectionH = TRUE;
   }

   if (((gm_GetScalerVMode(B_Channel) == SCALER_ZOOM) && (gmvw_OutputVActive[B_Channel] < stInput[B_Channel].Vactive))
         ||((gm_GetScalerVMode(B_Channel) == SCALER_SHRINK) && (gmvw_OutputVActive[B_Channel] > stInput[B_Channel].Vactive)))
   {
      //GetStartPosition() and GetProportionalVActive() will fail if either value is zero
      if ((gmvw_OutputVActive[B_Channel] > 0)&&(stInput[B_Channel].Vactive > 0))
         Bt_CorrectionV = TRUE;
   }

   if (Bt_CorrectionH && Bt_CorrectionV)
   {
      WORD W_DesiredVActive;
      W_DesiredVActive = GetProportionalVActive(pWinPrevDim->W_Width, *Wp_VaryingHActive, pWinPrevDim->W_Height, *Wp_VaryingVActive, *Wp_FixedHActive);

      //Find out which 1:1 scaler step comes earlier and leave the other 1:1 scaler step for next iteration
      if (((pWinPrevDim->W_Height >= W_DesiredVActive)&&(W_DesiredVActive >= *Wp_FixedVActive))
            ||((pWinPrevDim->W_Height < W_DesiredVActive)&&(W_DesiredVActive < *Wp_FixedVActive)))
      {
         Bt_CorrectionV = FALSE;
      }
      else
      {
         Bt_CorrectionH = FALSE;
      }

      //both H and V need 1:1 scaler frame in the same iteration, calculate the correct [H|V]Start
      /**Wp_VaryingHStart = GetStartPosition(pWinPrevDim->W_Hstart, pWinPrevDim->W_Width, *Wp_VaryingHStart, *Wp_VaryingHActive, *Wp_FixedHActive);
      *Wp_VaryingHActive = *Wp_FixedHActive;
      *Wp_VaryingVStart = GetStartPosition(pWinPrevDim->W_Vstart, pWinPrevDim->W_Height, *Wp_VaryingVStart, *Wp_VaryingVActive, *Wp_FixedVActive);
      *Wp_VaryingVActive = *Wp_FixedVActive;
      B_IndexCorrection = 1;*/
   }
   if (Bt_CorrectionH)
   {
      WORD W_DesiredVActive;

      //only H need the 1:1 scaler bypass frame, calculate the correct HStart
      *Wp_VaryingHStart = GetStartPosition(pWinPrevDim->W_Hstart, pWinPrevDim->W_Width, *Wp_VaryingHStart, *Wp_VaryingHActive, *Wp_FixedHActive);

      //proportionally size V to match the modified H
      W_DesiredVActive = GetProportionalVActive(pWinPrevDim->W_Width, *Wp_VaryingHActive, pWinPrevDim->W_Height, *Wp_VaryingVActive, *Wp_FixedHActive);

      *Wp_VaryingHActive = *Wp_FixedHActive;
      *Wp_VaryingVStart = GetStartPosition(pWinPrevDim->W_Vstart, pWinPrevDim->W_Height, *Wp_VaryingVStart, *Wp_VaryingVActive, W_DesiredVActive);
      *Wp_VaryingVActive = W_DesiredVActive;
      B_IndexCorrection = 1;
   }
   if (Bt_CorrectionV)
   {
      WORD W_DesiredHActive;

      //only V need the 1:1 scaler bypass frame, calculate the correct VStart
      *Wp_VaryingVStart = GetStartPosition(pWinPrevDim->W_Vstart, pWinPrevDim->W_Height, *Wp_VaryingVStart, *Wp_VaryingVActive, *Wp_FixedVActive);

      //proportionally size H to match the modified V; can use the same function but need to reverse the H's and V's
      W_DesiredHActive = GetProportionalVActive(pWinPrevDim->W_Height, *Wp_VaryingVActive, pWinPrevDim->W_Width, *Wp_VaryingHActive, *Wp_FixedVActive);

      *Wp_VaryingVActive = *Wp_FixedVActive;
      *Wp_VaryingHStart = GetStartPosition(pWinPrevDim->W_Hstart, pWinPrevDim->W_Width, *Wp_VaryingHStart, *Wp_VaryingHActive, W_DesiredHActive);
      *Wp_VaryingHActive = W_DesiredHActive;
      B_IndexCorrection = 1;
   }

   //make sure values are even
   *Wp_VaryingHActive = (*Wp_VaryingHActive + 1) & ~1;

   return (B_IndexCorrection);
}

//******************************************************************************
//
// FUNCTION       : static void SetDynamicScalerToBypass(gmt_PHY_CH B_Channel,
//                 BOOL Bt_InputFixed, gmt_WindowStruct* WinPrevDim)
//
// USAGE          : Put the scaler to bypass mode while preserving the desired window
//                  dimension, given that the current input and output sizes are
//                  identical.
//
// INPUT          : B_Channel - CH_A or CH_B
//               Bt_InputFixed - if TRUE, means that input dimension is the reference
//                           (fixed) and output dimension can change;
//                           otherwise, output dimension is fixed and the input
//                           dimension can change
//               WinPrevDim - previous window dimension; used to generate the
//                          dimension for the bypass step
//
// OUTPUT         : None
//
// GLOBALS        : gmvw_InputHActive   (R/W)
//                  gmvw_InputVActive   (R/W)
//                  gmvw_InputHStart   (R/W)
//                  gmvw_InputVStart   (R/W)
//               gmvw_OutputHActive   (R/W)
//                  gmvw_OutputVActive   (R/W)
//                  gmvw_OutputHStart   (R/W)
//                  gmvw_OutputVStart   (R/W)
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION : Since MAIN_DYNAMIC_SCALE_EN is set, there needs to be a way
//                  to turn it off, either by calling gm_ScalerDynamicEnable(FALSE)
//                  or calling DynamicScalerStop()
//
//******************************************************************************
static void SetDynamicScalerToBypass(gmt_PHY_CH B_Channel, BOOL Bt_InputFixed,
                                     gmt_WindowStruct* WinPrevDim)
{
   gmt_WindowStruct TempWin;

   if (!gm_IsValidSignal(B_Channel)) return;

   if (Bt_InputFixed)
   {
      //save the desired output dimension
      TempWin.W_Hstart = gmvw_OutputHStart[B_Channel];
      TempWin.W_Width = gmvw_OutputHActive[B_Channel];
      TempWin.W_Vstart = gmvw_OutputVStart[B_Channel];
      TempWin.W_Height = gmvw_OutputVActive[B_Channel];

      //restore original size to get scaler to bypass mode
      gmvw_OutputHStart[B_Channel] = WinPrevDim->W_Hstart;
      gmvw_OutputHActive[B_Channel] = WinPrevDim->W_Width;
      gmvw_OutputVStart[B_Channel] = WinPrevDim->W_Vstart;
      gmvw_OutputVActive[B_Channel] = WinPrevDim->W_Height;
   }
   else
   {
      //save the desired input dimension
      TempWin.W_Hstart = stInput[B_Channel].Hstart;
      TempWin.W_Width = stInput[B_Channel].Hactive;
      TempWin.W_Vstart = stInput[B_Channel].Vstart;
      TempWin.W_Height = stInput[B_Channel].Vactive;

      //restore original size to get scaler to bypass mode
      stInput[B_Channel].Hstart = WinPrevDim->W_Hstart;
      stInput[B_Channel].Hactive = WinPrevDim->W_Width;
      stInput[B_Channel].Vstart = WinPrevDim->W_Vstart;
      stInput[B_Channel].Vactive = WinPrevDim->W_Height;
   }

   gm_ScalerDynamicEnable(B_Channel, TRUE);
   gm_ScalerSetup(B_Channel);
   gm_SyncUpdateWait(B_Channel, INPUT_OUTPUT_CLOCK);

   if (Bt_InputFixed)
   {
      //restore the desired dimension
      gmvw_OutputHStart[B_Channel] = TempWin.W_Hstart;
      gmvw_OutputHActive[B_Channel] = TempWin.W_Width;
      gmvw_OutputVStart[B_Channel] = TempWin.W_Vstart;
      gmvw_OutputVActive[B_Channel] = TempWin.W_Height;
   }
   else
   {
      //restore the desired dimension
      stInput[B_Channel].Hstart = TempWin.W_Hstart;
      stInput[B_Channel].Hactive = TempWin.W_Width;
      stInput[B_Channel].Vstart = TempWin.W_Vstart;
      stInput[B_Channel].Vactive = TempWin.W_Height;
   }
}

//******************************************************************************
//
// FUNCTION       : void SetDynamicScalerSizeBegin(gmt_PHY_CH B_Channel)
//
// USAGE          : Setup dynamic scaling beginning size
//
// INPUT          : B_Channel
//
// OUTPUT         : None
//
// GLOBALS        : St_DynScale            (W)
//                  gmvw_OutputHActive      (R)
//                  gmvw_OutputVActive      (R)
//                  gmvw_OutputHStart      (R)
//                  gmvw_OutputVStart      (R)
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void SetDynamicScalerSizeBegin(gmt_PHY_CH B_Channel)
{
   St_DynScale[B_Channel].W_BeginSizeH = gmvw_OutputHActive[B_Channel];
   St_DynScale[B_Channel].W_BeginSizeV = gmvw_OutputVActive[B_Channel];
   St_DynScale[B_Channel].W_BeginPosH  = gmvw_OutputHStart[B_Channel];
   St_DynScale[B_Channel].W_BeginPosV  = gmvw_OutputVStart[B_Channel];
}

//******************************************************************************
//
// FUNCTION       : void SetDynamicScalerSizeEnd(gmt_PHY_CH B_Channel)
//
// USAGE          : Setup dynamic scaling Final size
//
// INPUT          : B_Channel
//
// OUTPUT         : None
//
// GLOBALS        : St_DynScale            (W)
//                  gmvw_OutputHActive      (R)
//                  gmvw_OutputVActive      (R)
//                  gmvw_OutputHStart      (R)
//                  gmvw_OutputVStart      (R)
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void SetDynamicScalerSizeEnd(gmt_PHY_CH B_Channel)
{
   St_DynScale[B_Channel].W_EndPosH    = gmvw_OutputHStart[B_Channel];
   St_DynScale[B_Channel].W_EndPosV    = gmvw_OutputVStart[B_Channel];
   St_DynScale[B_Channel].W_EndSizeH   = gmvw_OutputHActive[B_Channel];
   St_DynScale[B_Channel].W_EndSizeV   = gmvw_OutputVActive[B_Channel];
}

//******************************************************************************
//
// FUNCTION       : void SetDynamicScalerStep(gmt_PHY_CH B_Channel, BYTE B_Step)
//
// USAGE          : Setup dynamic scaling step size
//
// INPUT          : B_Channel
//                  B_Step
//
// OUTPUT         : None
//
// GLOBALS        : St_DynScale            (W)
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void SetDynamicScalerStep(gmt_PHY_CH B_Channel, BYTE B_Step)
{
   St_DynScale[B_Channel].W_NumberOfSteps = B_Step;
}

//******************************************************************************
//
// FUNCTION       : void DynamicScalerSetup(gmt_PHY_CH B_Channel)
//
// USAGE          : Setup dynamic scaling step size
//
// INPUT          : B_Channel
//
// OUTPUT         : None
//
// GLOBALS        : St_DynScale            (W)
//
// USED_REGS      :
//
// PRE_CONDITION  : SetDynamicScalerSizeBegin()
//
// POST_CONDITION :
//
//******************************************************************************
void DynamicScalerSetup(gmt_PHY_CH B_Channel)
{
   if (gm_IsValidSignal(B_Channel) == TRUE)
   {
      St_DynScale[B_Channel].W_NumberOfSteps = DYNAMIC_STEP;
   }
   else
   {
      //turn off PIP at blender when no signal to PIP; this is a software workaround
      //  for disappearing MAIN window when no signal to PIP at blender
      //FLI8668 should not have this problem due to FAILSAFE feature
#if (FEATURE_BLENDER_FAILSAFE == REV_NONE)
      if ((B_Channel == gmvb_PipChannel) && (IsSideBySidePipMode(UserPrefPipMode) == TRUE))
      {
         if (gm_IsValidSignal(CH_A))
         {
            gmvw_OutputHActive[CH_A] = St_DynScale[CH_A].W_BeginSizeH;
            gmvw_OutputVActive[CH_A] = St_DynScale[CH_A].W_BeginSizeV;
            gmvw_OutputHStart[CH_A] = St_DynScale[CH_A].W_BeginPosH;
            gmvw_OutputVStart[CH_A] = St_DynScale[CH_A].W_BeginPosV;

            //This code is added here because MOTION_PROC is set to spatial in DynamicScalerStart()
            //  but the Main scaler co-eff is not changed accordingly and the call to gm_BlenderOnOff()
            //  will cause sync update and this will cause Main blender to underflow.
            gm_ScalerSetup(CH_A);
            gm_MemorySizeAdjust(CH_A);
            gm_SyncUpdate(CH_A, INPUT_OUTPUT_CLOCK);
         }
         gm_BlenderOnOff(gmvb_PipChannel, FALSE);
      }
#endif
      St_DynScale[B_Channel].W_NumberOfSteps = 0;
      /*   gm_DisplaySetSize(B_Channel);
         gm_DisplaySetPosition(B_Channel);
         gm_ScalerSetup(B_Channel);
         gm_MemorySizeAdjust(B_Channel);
         gm_SyncUpdate(B_Channel, INPUT_OUTPUT_CLOCK);*/
   }
}

//******************************************************************************
//
// FUNCTION       : void DynamicScalerStart(gmt_PHY_CH B_Channel)
//
// USAGE          : Changes system setup to start dynamic scaling
//
// INPUT          :
//
// OUTPUT         : None
//
// GLOBALS        : B_DynScalRun      (RW)
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DynamicScalerStart(gmt_PHY_CH B_Channel)
{
   if (B_DynScalRun[B_Channel] == FALSE)
   {
      if (gm_IsValidSignal(B_Channel) == FALSE)
         return;

      B_DynScalRun[B_Channel] = TRUE;            // Indicate dynamic started

      //
      // Indicate dynamic scale start, disable necessary video process
      //
      if (B_Channel == CH_A)
      {
         //Exit VCR mode first before entering dynamic scaling
         if (gm_IsVCRTrickModeEnabled())
         {
            gm_VideoProcessingDisable(VIDEO_VCR);
            gm_SyncUpdateWait(CH_A, OUTPUT_CLOCK);
         }
         gm_VideoProcessingEnable(VIDEO_DYN_SCAL);
      }
#if (FEATURE_PIP_DCDI == ENABLE)
      else
         gm_VideoProcessingDisable(VIDEO_DCDI_PIP);
#endif
   }
}

//******************************************************************************
//
// FUNCTION       : void DynamicScalerStop(void)
//
// USAGE          : Changes system setup to end dynamic scaling
//
// INPUT          :
//
// OUTPUT         : None
//
// GLOBALS        : B_DynScalRun      (RW)
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DynamicScalerStop(void)
{
   BOOL Bt_UpdateFilter[] = {FALSE, FALSE};
   {
      //
      // Finish dynamic scaling
      //
      if (B_DynScalRun[CH_A])
      {
	if(gm_IsModeComplete(CH_A))
         gm_ScalerDynamicWait(NUM_VSYNCS); // if absent it causes flash on screen

         Bt_UpdateFilter[CH_A] = TRUE;
         B_DynScalRun[CH_A] = FALSE;
         gm_VideoProcessingDisable(VIDEO_DYN_SCAL|VIDEO_DCDI_MAIN);
         gm_ScalerDynamicEnable(CH_A,DYNAMIC_SCALE_DISABLE);

         //
         // Disable spatial processing
         //
         gm_VideoProcessingDisable(VIDEO_SPATIAL);

         gm_ScalerSetup(CH_A);
         gm_AfmSetup(W_OverscanEnabled[CH_A]);
         // This sync update wait cause the latest line display late.
         //gm_SyncUpdateWait(CH_A, INPUT_OUTPUT_CLOCK);//this line is critical since CH_A is getting out of SPATIAL         
         
#if (FEATURE_PIP_DCDI == ENABLE)
         if (gmvb_MainChannel == CH_A)
         {
            if (UserPrefDCDiMain == ENABLE)
               gm_VideoProcessingEnable(VIDEO_DCDI_MAIN);
         }
         else
         {
            if (UserPrefDCDiPip== ENABLE)
               gm_VideoProcessingEnable(VIDEO_DCDI_MAIN);
         }
#endif

      }
      
      if (B_DynScalRun[CH_B])
      {
         if (!Bt_UpdateFilter[CH_A]&&gm_IsModeComplete(CH_B)) gm_ScalerDynamicWait(NUM_VSYNCS); // if absent it causes flash on screen
         Bt_UpdateFilter[CH_B] = TRUE;
         B_DynScalRun[CH_B] = FALSE;
#if (FEATURE_PIP_DCDI == ENABLE)
         gm_VideoProcessingDisable(VIDEO_DCDI_PIP);
#endif
         gm_ScalerDynamicEnable(CH_B,DYNAMIC_SCALE_DISABLE);

         //this line is not necessary since, unlike CH_A, CH_B is always in SPATIAL mode, but
         //  this line can be necessary in the future if MADI is ported to CH_B
         //gm_ScalerSetup(CH_B);
         //gm_SyncUpdateWait(CH_B, INPUT_OUTPUT_CLOCK);
#if (FEATURE_PIP_DCDI == ENABLE)
         if (gmvb_MainChannel == CH_B)
         {
            if (UserPrefDCDiMain == ENABLE)
               gm_VideoProcessingEnable(VIDEO_DCDI_PIP);
         }
         else
         {
            if (UserPrefDCDiPip== ENABLE)
               gm_VideoProcessingEnable(VIDEO_DCDI_PIP);
         }
#endif
      }

      //
      // We need to restore system default parameters
      //
#if (FEATURE_PIP_DCDI == DISABLE)
      if ((gmvb_MainChannel == CH_A)&&(UserPrefDCDiMain == ENABLE))
         gm_VideoProcessingEnable(VIDEO_DCDI_MAIN);
#endif
   }
   if (Bt_UpdateFilter[CH_A] || Bt_UpdateFilter[CH_B])
   {
	  if(Bt_UpdateFilter[CH_A] == gmd_TRUE)
      		UpdateFilters(CH_A);//update filters immediately so that there is no observable transient effect
	  if(Bt_UpdateFilter[CH_B] == gmd_TRUE)
      		UpdateFilters(CH_B);//update filters immediately so that there is no observable transient effect
      #if 0 // Already update in UpdateFilters()
      if (Bt_UpdateFilter[CH_A] && Bt_UpdateFilter[CH_B])
      {
         gm_SyncUpdateWait(CH_B, INPUT_CLOCK);
      }
      else if (Bt_UpdateFilter[CH_A])
      {
         gm_SyncUpdateWait(CH_A, INPUT_OUTPUT_CLOCK);
      }
      else if (Bt_UpdateFilter[CH_B])
      {
         gm_SyncUpdateWait(CH_B, INPUT_OUTPUT_CLOCK);
   }
      #endif
   }
}
/***********************************  END  ************************************/

