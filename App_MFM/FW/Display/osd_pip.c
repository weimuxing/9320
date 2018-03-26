/*
	$Workfile:   osd_pip.c  $
	$Revision: 1.29 $
	$Date: 2013/05/17 02:55:05 $
*/
#define __OSD_PIP_C__
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
// MODULE:     	osd_pip.c
//
// USAGE:		This module contains standard function for PIP control
//				(e.g. single PIP on/off, multi PIP, SWAP)
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "system\all.h"
#include "StdLibInc\gm_Register.h"

//******************************************************************************
//  E X T E R N S
//******************************************************************************

extern void CcDataSourceSwap(void);

//******************************************************************************
//  D E F I N E S
//******************************************************************************

//
// Debug print message
//
//#define _DEBUG
#ifdef _DEBUG
#define  msg(a)            		gm_Print(a,0)
#define  msg_i(a,b)        	 	gm_Print(a,b)
#else
#define  msg(a)
#define  msg_i(a,b)
#endif

enum
{
   BOTH_RF,
   MAIN_RF,
   PIP_RF
};


//******************************************************************************
//  S T A T I C    V A R I A B L E S
//******************************************************************************

static BYTE B_PreviousPipMode;

//******************************************************************************
// S T A T I C   F U N C T I O N S
//******************************************************************************
static void SwapMainPip_OutputSwap(void);
static void SwapTuner(BYTE B_SwapType);
static void SwapArray(void * ArrayPtr, WORD W_ArraySize);
static void SwapGlobalData(void);

//******************************************************************************
//  G L O B A L   V A R I A B L E S
//******************************************************************************
extern gmt_UserDefConnNames MovePortFromConflictPort(gmt_UserDefConnNames Port);
extern void VWD_ClearWindowsInfo(void);//120813 Edward for reset VWD detection

//******************************************************************************
//  C O D E
//******************************************************************************

#ifdef SMART_SWAP
//******************************************************************************
//
// FUNCTION       : static void SwapArray(void * ArrayPtr, WORD W_ArraySize)
//
// USAGE          : Given a pointer to a two-element array and the total array size,
//					swap the content of the first and the second element
//
// INPUT          :	ArrayPtr - pointer to an array
//					W_ArraySize - total array size
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
static void SwapArray(void * ArrayPtr, WORD W_ArraySize)
{
   WORD W_Temp;
   DWORD DW_Temp;
   BYTE B_Temp;

   switch (W_ArraySize)
   {
      case sizeof(WORD)*2:
         W_Temp = *(WORD*)ArrayPtr;
         *(WORD*)ArrayPtr = *((WORD*)ArrayPtr+1);
         *((WORD*)ArrayPtr+1) = W_Temp;
         break;
      case sizeof(BYTE)*2:
         B_Temp = *(BYTE*)ArrayPtr;
         *(BYTE*)ArrayPtr = *((BYTE*)ArrayPtr+1);
         *((BYTE*)ArrayPtr+1) = B_Temp;
         break;
      case sizeof(DWORD)*2:
      default:
         DW_Temp = *(DWORD*)ArrayPtr;
         *(DWORD*)ArrayPtr = *((DWORD*)ArrayPtr+1);
         *((DWORD*)ArrayPtr+1) = DW_Temp;
   }
}

//******************************************************************************
//
// FUNCTION       : static void SwapGlobalData(void)
//
// USAGE          : Swaps main and pip global variables for input swap feature
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : gmvb_InputConnector (R/W)
//					gB_CurrentInputConnector (R/W)
//					st_InputMeasurement (R/W)
//					gmvw_InputVFreq (R/W)
//					gmvw_InputHFreq (R/W)
//					gmvw_InputModeAttr (R/W)
//					gmvw_InputVTotalOrg (R/W)
//					gmvw_InputHTotalOrg (R/W)
//					gmvw_InputVStartOrg (R/W)
//					gmvw_InputVActiveOrg (R/W)
//					gmvw_InputHStartOrg (R/W)
//					gmvw_InputHActiveOrg (R/W)
//					gmvw_InputVTotal (R/W)
//					gmvw_InputHTotal (R/W)
//					gmvw_InputVStart (R/W)
//					gmvw_InputVActive (R/W)
//					gmvw_InputHStart (R/W)
//					gmvw_InputHActive (R/W)
//					gmvw_InputVTotal (R/W)
//					gmvw_InputHTotal (R/W)
//					gmvb_ChannelSource (R/W)
//					gmvw_InputSyncFlags (R/W)
//					gmvw_InputInterlace (R/W)
//					B_ModeIndex (R/W)
//					W_OverscanEnabled (R/W)
//					gmvb_VCRMode (R/W)
//					B_FreeRun (R/W)
//					B_AutoAdjustStatus (R/W)
//					B_AutoAdjustStarted (R/W)
//					gmvb_CurrentSyncState (R/W)
//					gmvb_EnableClamp (R/W)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
static void SwapGlobalData(void)
{
   gmt_InputStruct StructTempBuffer;
   WORD W_Temp;
   BYTE B_Temp;

   // for master-slave the pip allocation is fixed

   SwapArray(gmvb_InputConnector, sizeof(gmvb_InputConnector));
   SwapArray(gB_CurrentInputConnector, sizeof(gB_CurrentInputConnector));

   StructTempBuffer = stInput[gmvb_MainChannel];
   stInput[gmvb_MainChannel] = stInput[gmvb_PipChannel];
   stInput[gmvb_PipChannel] = StructTempBuffer;

   SwapArray(gmvw_InputVTotalOrg, sizeof(gmvw_InputVTotalOrg));
   SwapArray(gmvw_InputHTotalOrg, sizeof(gmvw_InputHTotalOrg));
   SwapArray(gmvw_InputVStartOrg, sizeof(gmvw_InputVStartOrg));
   SwapArray(gmvw_InputVActiveOrg, sizeof(gmvw_InputVActiveOrg));
   SwapArray(gmvw_InputHStartOrg, sizeof(gmvw_InputHStartOrg));
   SwapArray(gmvw_InputHActiveOrg, sizeof(gmvw_InputHActiveOrg));

   SwapArray(gmvw_MemoryMaxHCapture, sizeof(gmvw_MemoryMaxHCapture));

   //need to swap the Org ones because the input values can have been cropped
   stInput[gmvb_MainChannel].Vtotal = gmvw_InputVTotalOrg[gmvb_MainChannel];
   stInput[gmvb_MainChannel].Htotal = gmvw_InputHTotalOrg[gmvb_MainChannel];
   stInput[gmvb_MainChannel].Vstart = gmvw_InputVStartOrg[gmvb_MainChannel];
   stInput[gmvb_MainChannel].Vactive = gmvw_InputVActiveOrg[gmvb_MainChannel];
   stInput[gmvb_MainChannel].Hstart = gmvw_InputHStartOrg[gmvb_MainChannel];
   stInput[gmvb_MainChannel].Hactive = gmvw_InputHActiveOrg[gmvb_MainChannel];
   stInput[gmvb_PipChannel].Vtotal = gmvw_InputVTotalOrg[gmvb_PipChannel];
   stInput[gmvb_PipChannel].Htotal = gmvw_InputHTotalOrg[gmvb_PipChannel];
   stInput[gmvb_PipChannel].Vstart = gmvw_InputVStartOrg[gmvb_PipChannel];
   stInput[gmvb_PipChannel].Vactive = gmvw_InputVActiveOrg[gmvb_PipChannel];
   stInput[gmvb_PipChannel].Hstart = gmvw_InputHStartOrg[gmvb_PipChannel];
   stInput[gmvb_PipChannel].Hactive = gmvw_InputHActiveOrg[gmvb_PipChannel];

   SwapArray(gmvb_ChannelSource, sizeof(gmvb_ChannelSource));
   SwapArray(gmvw_InputSyncFlags, sizeof(gmvw_InputSyncFlags));
   SwapArray(gmvw_InputOversample, sizeof(gmvw_InputOversample));
   SwapArray(W_OverscanEnabled, sizeof(W_OverscanEnabled));
   SwapArray(gmvb_VCRMode, sizeof(gmvb_VCRMode));

   SwapArray(B_FreeRun, sizeof(B_FreeRun));
   SwapArray(B_AutoAdjustStatus, sizeof(B_AutoAdjustStatus));
   SwapArray(B_AutoAdjustCount, sizeof(B_AutoAdjustCount));
   SwapArray(B_AutoAdjustStarted, sizeof(B_AutoAdjustStarted));
   SwapArray(gmvb_InputSamplingPhase, sizeof(gmvb_InputSamplingPhase));
   SwapArray(gmvsw_InputHSyncOffset, sizeof(gmvsw_InputHSyncOffset));
   SwapArray(gmvsw_InputVSyncOffset, sizeof(gmvsw_InputVSyncOffset));

   SwapArray(B_AspectMethod,sizeof(B_AspectMethod));
   SwapArray(gmvb_STABLE_COUNT, sizeof(gmvb_STABLE_COUNT));
   SwapArray(gmvb_UNSTABLE_COUNT, sizeof(gmvb_UNSTABLE_COUNT));
}

//******************************************************************************
//
// FUNCTION       : static void SwapMainPip_OutputSwap(void)
//
// USAGE          :	Perform a swap at the blender with mode detection skipped
//
// INPUT          : None
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
static void SwapMainPip_OutputSwap(void)
{
   gmt_PHY_CH B_Channel;
   gmt_WindowStruct TempWin;

   if (UserPrefPipMode != NO_PIP)
   {
      //need to turn off pip border and highlight border because in the new mapping
      //  highlight border can be used in place of pip border
//		gm_PipBorderEnable(Off);
//		gm_HighlightBorder(FALSE,0,0,0,0);

      B_Channel = gmvb_MainChannel;
      gmvb_MainChannel = gmvb_PipChannel;
      gmvb_PipChannel = B_Channel;

      B_FocusBorder = gm_OtherChannel(B_FocusBorder);

      SaveModeIndependentSettings();

      //Save the output dimension of CH_A since it will be changed by calling
      //  SetDisplayMain()/SetDisplayPip()
      TempWin.W_Hstart = gmvw_OutputHStart[CH_A];
      TempWin.W_Vstart = gmvw_OutputVStart[CH_A];
      TempWin.W_Width = gmvw_OutputHActive[CH_A];
      TempWin.W_Height = gmvw_OutputVActive[CH_A];

      //VIDEO_SPATIAL directly affects MVF. Run these lines to ensure that
      //  VIDEO_SPATIAL is set/cleared accordingly.
      //  Without MAIN/PIP display size we cannot disable VIDEO_SPATIAL if previously
      //  CH_A scaler is set to shrink mode.
      if (gmvb_MainChannel == CH_A)
         SetDisplayMain();
      else
         SetDisplayPip();
      gm_VideoProcessingDisable(VIDEO_SPATIAL);

      //Restore original output dimension
      gmvw_OutputHStart[CH_A] = TempWin.W_Hstart;
      gmvw_OutputVStart[CH_A] = TempWin.W_Vstart;
      gmvw_OutputHActive[CH_A] = TempWin.W_Width;
      gmvw_OutputVActive[CH_A] = TempWin.W_Height;

      // Determine which one is on top
      gm_BlenderSetPriority((gmvb_MainChannel == CH_A)?PIP_ON_MAIN:MAIN_ON_PIP);

      ModeSetupDisplayChange(gmvb_MainChannel, DISPLAY_CHANGE_QUICK);
      ModeSetupDisplayChange(gmvb_PipChannel, DISPLAY_CHANGE_QUICK);
   }
}

//******************************************************************************
//
// FUNCTION       : void SwapMainPip_InputSwap(void)
//
// USAGE          :	Perform swap which triggers mode detection (Cortez & Plus)
//					Perform swap at the Input Select Mux with mode detection
//					skipped (Advanced)
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :	gmvb_MemoryBufferTypes	(W)
//					gmvw_InputModeAttr		(W)
//					gmvw_InputHTotal		(R)
//
// USED_REGS      :	IMP_SOURCE 			(R/W)
//					IPP_SOURCE 			(R/W)
//					AFR_CONTROL			(W)
//					AUTO_BKGND_CONTROL	(W)
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void SwapMainPip_InputSwap(void)
{
   // retrieve previous channel sources for both main and pip
   gmt_SOURCE_TYPE Old_IMP_SOURCE = gm_GetChannelSource(CH_A);
   gmt_SOURCE_TYPE Old_IPP_SOURCE = gm_GetChannelSource(CH_B);
   WORD W_OldAutoBkgndControlPip;
   WORD W_OldAutoBkgndControlMain;
   WORD W_NewAutoBkgndControl;

   SwapGlobalData();

   //For the AUTO_BKGND_CONTROL register, swap the pip and main bits
   W_OldAutoBkgndControlPip = gm_ReadRegWord(AUTO_BKGND_CONTROL) >> 8;
   W_OldAutoBkgndControlMain = gm_ReadRegWord(AUTO_BKGND_CONTROL) << 8;
   W_NewAutoBkgndControl = ( W_OldAutoBkgndControlMain | W_OldAutoBkgndControlPip ) ;

   //The following two lines must be executed one immediately after the other. Please refer to
   //   comments in SetupInternalDevice() for details.

   if (GetPortSignalType(UserPrefCurrentInputMain) == SIG_COMPONENT)
   {
      SelectInput(gmvb_MainChannel, UserPrefCurrentInputMain, SMART_SWAP_MODE);
      SelectInput(gmvb_PipChannel, UserPrefCurrentInputPip, SMART_SWAP_MODE);
   }
   else
   {
      SelectInput(gmvb_PipChannel, UserPrefCurrentInputPip, SMART_SWAP_MODE);
      SelectInput(gmvb_MainChannel, UserPrefCurrentInputMain, SMART_SWAP_MODE);
   }

   // write swapped AUTO_BKGND_CONTROL register
   gm_WriteRegWord(AUTO_BKGND_CONTROL, W_NewAutoBkgndControl);

//	gm_DecoderModeDetectionEnable(gmd_DEC_CORTEZ_ID1, FALSE);
//	gm_DecoderModeDetectionEnable(gmd_DEC_CORTEZ_ID2, FALSE);
//	gm_StateDetectionConfig(gmvb_MainChannel, DET_FORCED_STABLE);// This is already done in SetupInternalDevice()
//	gm_StateDetectionConfig(gmvb_PipChannel, DET_FORCED_STABLE);
//	gmvb_MemoryBufferTypes[gmvb_MainChannel] = 0;
//	gmvb_MemoryBufferTypes[gmvb_PipChannel] = 0;

   // NOTE: for dynamic memory allocation, ModeSetPrefs(gmvb_PipChannel) MUST run before
   //       ModeSetPrefs(gmvb_MainChannel), since Main can disable Pip if there is not enough memory

//	SETBIT(gmvw_InputModeAttr[gmvb_PipChannel], gmd_MODE_VALID);
   ModeSetPrefs(gmvb_PipChannel);
//	SETBIT(gmvw_InputModeAttr[gmvb_MainChannel], gmd_MODE_VALID);
   ModeSetPrefs(gmvb_MainChannel);

   // set current main source as previous pip source and current pip source as previous main source
   gm_SetChannelSource(CH_A, Old_IPP_SOURCE);
   gm_SetChannelSource(CH_B, Old_IMP_SOURCE);

   //
   // Clamping
   // SDDS
   // Clock Selection
   //
//	gm_AipSetClamping(gmvb_MainChannel);
//	gm_SourceClockSetup( gmvb_MainChannel, gmvw_InputHTotal[gmvb_MainChannel]);
   gm_ClockSelection(gmvb_MainChannel);
//	gm_AipDecimationFilterSetup(gmvb_MainChannel);

//	gm_AipSetClamping(gmvb_PipChannel);
//	gm_SourceClockSetup( gmvb_PipChannel, gmvw_InputHTotal[gmvb_PipChannel]);
   gm_ClockSelection(gmvb_PipChannel);
//	gm_AipDecimationFilterSetup(gmvb_PipChannel);

   ModeSetupUserPref(gmvb_MainChannel);
   ModeSetupUserPref(gmvb_PipChannel);
   SetupColorSpace(gmvb_MainChannel);
   SetupColorSpace(gmvb_PipChannel);

   ModeSetupInputChange(gmvb_MainChannel);
   ModeSetupInputChange(gmvb_PipChannel);

   gmvb_SyncDetectClamp[gmd_ADC_0] = TRUE;//this is set to FALSE during input swap and channel clone

   //the above two lines change the input dimension, so need to run memory and
   //  scaler setup
   gm_MemorySetup(gmvb_MainChannel);
   gm_MemorySetup(gmvb_PipChannel);
   gm_ScalerSetup(gmvb_MainChannel);
   gm_ScalerSetup(gmvb_PipChannel);

   //Adjust source for dynamic frame lock
   DisplayLockSource();
   if (gmvb_DDDSSourceChannel == gmvb_MainChannel)
      DisplayClockLock(gmvb_MainChannel, 1);
   else
      DisplayClockLock(gmvb_PipChannel, 1);

   //Blender delay in ModeSetupInputChange() is not long enough
   ModeSetupRequestCancel(gmvb_MainChannel, REQ_MS_BLENDER, PARAM_ANY);
   ModeSetupRequestCancel(gmvb_PipChannel, REQ_MS_BLENDER, PARAM_ANY);

   //if any channel is not valid, set to free run
   if (gm_IsValidSignal(gmvb_MainChannel))
      ModeSetupRequest(gmvb_MainChannel, REQ_MS_BLENDER, PARAM_NONE, DISPLAY_DEFER_TMO_NORMAL+50);
   else
      SelectInput(gmvb_MainChannel, UserPrefCurrentInputMain, NORMAL_MODE);

   if (gm_IsValidSignal(gmvb_PipChannel))
      ModeSetupRequest(gmvb_PipChannel, REQ_MS_BLENDER, PARAM_NONE, DISPLAY_DEFER_TMO_NORMAL+50);
   else
      SelectInput(gmvb_PipChannel, UserPrefCurrentInputPip, NORMAL_MODE);

#if 0
   if (gmc_MemoryAllocationMethod == gmd_MEMORY_ALLOCATE_DYNAMIC)
   {
      if ((gm_IsValidSignal(gmvb_MainChannel) && (gmvb_MemoryBufferTypes[gmvb_MainChannel] & BUF_DECODER))
            || (gm_IsValidSignal(gmvb_PipChannel) && (gmvb_MemoryBufferTypes[gmvb_PipChannel] & BUF_DECODER)))
      {
         gm_DecoderMemory(gmd_DEC_CORTEZ_ID, gmd_RESET);
      }
   }
#endif
}
#endif


#if 1//def Action_SwapMainPip_WB_Used
//******************************************************************************
//
// FUNCTION       : void SwapMainPip(void)
//
// USAGE          : Swaps main and pip window input sources
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : B_ModeSetupReq		(R/W)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void SwapMainPip(void)
{
   BOOL B_SwapComplete = FALSE;	// Supports quicker swapping for special cases

   if ((UserPrefPipMode != NO_PIP)
         && (BlenderIsPending(CH_A) == FALSE))
   {
      //
      // Remaining swap cases
      //
      if (B_SwapComplete == FALSE)
      {
			BYTE B_Temp, B_InputTemp;
         BYTE ImageScheme; 	// 20081118 #5, keep Scheme mode for new channel to match OSD

         SetDisplayOff(CH_A, BACKGROUND_ONLY);
         SetDisplayOff(CH_B, BACKGROUND_ONLY);
		 
		 SysChFlag[CH_A] |= SysCh_ChannelChange;
		 SysChFlag[CH_B] |= SysCh_ChannelChange;


         // clear mode flags since swap will not change status ...
         if (!gm_IsValidSignal(gmvb_PipChannel))
         {
            stInput[gmvb_MainChannel].Flags= 0; // so far only xvYCC used and clear the flag
            stInput[gmvb_MainChannel].ModeFlags= 0; // Clear mode flag ...
         }

         // 20081118 #5, keep Scheme mode for new channel to match OSD
         ImageScheme = UserPrefImageScheme;

			// Swap current input UserPrefs and set previous input UserPref for Main
			B_InputTemp = UserPrefCurrentInputMain;
			UserPrefCurrentInputMain = UserPrefCurrentInputPip;
			UserPrefCurrentInputPip = B_InputTemp;

         B_Temp = UserPrefAspectRatioMain;
         UserPrefAspectRatioMain = UserPrefAspectRatioPip;
         UserPrefAspectRatioPip = B_Temp;

#ifdef SMART_SWAP

         /*        Commented out in order to appear a swapped image

                     //If there is any pending REQ_MS_SETUP, do it now; otherwise, corrupted image
                     //  may appear since SwapMainPip_OutputSwap() and SwapMainPip_InputSwap()
                     //  assume that if the input port has signal, then the channel must been set up
                     //  already.

                 	if (ModeSetupRequestCancel(gmvb_MainChannel, REQ_MS_SETUP, PARAM_ANY) == OK)
                     {
                         ModeSetupFullSetup(gmvb_MainChannel);
                	    }
                	    if (ModeSetupRequestCancel(gmvb_PipChannel, REQ_MS_SETUP, PARAM_ANY) == OK)
                	    {
                	        ModeSetupFullSetup(gmvb_PipChannel);
                     }

         */
         #if 0 // not implement
         SwapUserpref();
         #endif

         //remove all NoSignal msg boxes since we are changing both channels
         OSDRequestCancelMsg(gmvb_MainChannel, MTO_NoSignal);
         OSDRequestCancelMsg(gmvb_PipChannel, MTO_NoSignal);
#ifdef CHECK_IS_CABLE_CONNECT
         OSDRequestCancelMsg(gmvb_MainChannel, MTO_NoCable); // SCR2456
         OSDRequestCancelMsg(gmvb_PipChannel, MTO_NoCable); // SCR2456
#endif
         OSDRequestMsg(gmvb_MainChannel, MTO_ValidMode, REQUEST_NOW);
         OSDRequestMsg(gmvb_PipChannel, MTO_ValidMode, REQUEST_NOW);

#ifdef DYNAMIC_CHANNEL_MAPPING
         //output swap when the current modes are optimal in the current config;
         //  otherwise, perform input swap to make it optimal
         if (gm_IsValidSignal(CH_B) && gm_IsGraphics(CH_B) && !gm_IsInterlaced(CH_B))
            SwapMainPip_OutputSwap();
         else
#endif
         {
            /*				//make sure that RF1 is always associated with MAIN and RF2 with PIP
            				if (UserPrefCurrentInputMain == ALL_BOARDS_RF2)
            					UserPrefCurrentInputMain = ALL_BOARDS_RF1;
            				if (UserPrefCurrentInputPip == ALL_BOARDS_RF1)
            					UserPrefCurrentInputPip = ALL_BOARDS_RF2;*/

            SaveModeIndependentSettings();
            SwapMainPip_InputSwap();
         }

         //
         // need to swap the CC source to synchronize the image
         // SCR #2296, #2297
         //
         #if 0 // not support
         CcDataSourceSwap();
         #endif

         if (gm_IsValidSignal(gmvb_MainChannel))
         {
            B_SignalStatus = STATUS_SYNC_PRESENT;
            OSDRequestCancelMsg(gmvb_MainChannel, MTO_NoSignal);
#ifdef CHECK_IS_CABLE_CONNECT
            OSDRequestCancelMsg(gmvb_MainChannel, MTO_NoCable);
#endif
         }
         else
         {
#ifdef CHECK_IS_CABLE_CONNECT
            if (!isCableConnected(UserPrefCurrentInputMain)) // SCR2456
               OSDRequestMsg(gmvb_MainChannel, MTO_NoCable, 2 * NO_SIGNAL_DELAY);
            else
#endif
               OSDRequestMsg(gmvb_MainChannel, MTO_NoSignal, 2 * NO_SIGNAL_DELAY);
         }
#else

         SaveModeIndependentSettings();
#if (FEATURE_HDMI==ENABLE)
         if ((GetPortProc(UserPrefCurrentInputMain)==DVI_PORT) && (GetPortProc(UserPrefCurrentInputPip)==DVI_PORT))
            HdmiDviCloseInputPort(UserPrefCurrentInputPip);
#endif
         SelectInput(gmvb_MainChannel, UserPrefCurrentInputMain,  NORMAL_MODE);
         SelectInput(gmvb_PipChannel, UserPrefCurrentInputPip, NORMAL_MODE);
#endif
         // 20081118 #5, keep Scheme mode for new channel to match OSD
         UserPrefImageScheme = ImageScheme;
         if (gm_IsValidSignal(gmvb_MainChannel) && (gm_IsValidSignal(gmvb_PipChannel)))
            ModeSetupSwap();
         B_SwapComplete = TRUE;
      }
      else
      {
         return;
      }
   }
}
#endif	//#ifdef Action_SwapMainPip_WB_Used


//******************************************************************************
//
// FUNCTION       : void AdjustPipMode(void)
//
// USAGE          : Changes Pip mode to be on/large/small/side-by-side/off
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : B_ModeSetupReq		(R/W)
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AdjustPipMode(void)
{

#if (FEATURE_BLENDER_FAILSAFE == REV1)
   //Blender failsafe
   //Patch up for rev AA

   // 20090610 #1, Improved dither noise by shift pixel.
#if 0 // Input_Dither
   if (UserPrefPipMode == NO_PIP)
   {
      if (gm_ReadRegWord(GAMMA_LUT1_CTRL) & GAMMA_LUT1_CTL)
      {
         gm_WriteRegWord (OCM_SCRATCHPAD, 0x3);
         INTERRUPT_ENABLE(MAIN_INPUT_IRQ3MASK, IMP_BLANK_MASK);
      }
      else
      {
         gm_WriteRegWord (OCM_SCRATCHPAD, 0x0);
         INTERRUPT_DISABLE(MAIN_INPUT_IRQ3MASK, IMP_BLANK_MASK);
      }
   }
   else
   {
      if (UserPrefPipMode<SIDE_BY_SIDE && (gm_ReadRegWord(GAMMA_LUT1_CTRL) & GAMMA_LUT1_CTL) )  		//Large & Small PIP
      {
         gm_WriteRegWord(OCM_SCRATCHPAD,0);
         gm_WriteRegWord(IMP_H_ACT_WIDTH,stInput[CH_A].Hactive);
         gmp_MemorySizeAdjustMain(stInput[CH_A].Hactive,stInput[CH_A].Vactive,30);

         gm_SetRegBitsWord(HOST_CONTROL, IMP_SYNC_UPDATE);
      }
   }
#endif
   if (gm_ReadProductRevReg() == 0)
   {
      gm_BlenderFailSafeOnOff(Off);
      gm_SyncUpdateWait(gmvb_PipChannel, OUTPUT_CLOCK);
   }
#endif

   if (BlenderIsPending(gmvb_PipChannel) == FALSE)	// Wait for blender
   {												// No
      //
      // Dynamic Scaling - Reset
      //
      SW_DynScalePosition = 0;					// Default position
		#ifndef DELL_U2410F		//Neil120818* border always on pxp for U2413
		if (IsSideBySidePipMode(UserPrefPipMode) == FALSE)
			B_FocusBorder = gmvb_MainChannel;
        else		//Kordonm add for test 2011 0926
        #endif
			B_FocusBorder = gmvb_PipChannel;
      // If in side-by-side dynamic scaling sequence, remove all queued requests
	  PIPSendMessage(MTO_PIPMHide);
      ModeSetupRequestCancel(CH_A, REQ_MS_DYNAMIC, PARAM_ANY);
      ModeSetupRequestCancel(CH_B, REQ_MS_DYNAMIC, PARAM_ANY);
                 FirstACDCinPxP = FALSE;

      //
      // PIP Change
      // Will pass parameter to indicate not to run dynamic scaling,
      // in order to show pip quickly
      //
      if (B_PrevPipMode == NO_PIP)
      {
#if (FEATURE_TWO_STAGE_PROCESSING_MODE == ENABLE)
         if (gm_Is2StageEnabled())
         {
            ExitTwoStageProcessing();

            //need to turn on blender since
            //  ModeSetupDisplayChange(gmvb_MainChannel,...) will not execute
            //  in single pip mode
            if (IsSinglePipMode(UserPrefPipMode))
               ModeSetupBlender(CH_A, DISPLAY_DEFER_TMO_NORMAL);
         }
#endif

#ifdef PIP_PORT_INDEPENDENCE
         // Clone if there is port collision or if PIP is same as Main channel
         // Otherwise, perform a normal SelectInput() if PIP is not already setup
         ChoosePipSetupMethod();
#endif
			#ifdef DELL_U2410F
			//if(UserPrefSpeakerAudioSource == AUDIO_PIP)
			if(UserPrefAudioMainInput == gmd_AUDIO_SRC_AUTO_Pxp)
			{
				AudioUpdateNow = gmd_FALSE;
				gm_TimerStart(AUDIO_WAIT_10ms_TMR, Aud_ChannelChange_Time);
				//gm_AUDIO_ReStartDetection();
			}
			#endif

         ModeSetupDisplayChange(gmvb_PipChannel, DISPLAY_CHANGE_QUICK);
			//yvonne 20120908 for PIP/PBP On, DCR/EnergySmart should set OFF and brightness should back to original setting.
			if(((UserPrefDCR == On) && IsPresetMode_DCR_ON())|| (UserPrefEnergySmartOnOffFlag == On))
				AdjustBackLight();
      }
      else
      {
         //when highlight border is used in place of pip border at the transition from
         //  single-pip to side-by-side, remove highlight border; this is a temp patch
         if (IsSinglePipMode(B_PrevPipMode)&&IsSideBySidePipMode(UserPrefPipMode)
               &&(gmvb_MainChannel!=CH_A))
            gm_HighlightBorder(FALSE,0,0,0,0);

#if (FEATURE_TWO_STAGE_PROCESSING_MODE == ENABLE)
         if (!gm_Is2StageEnabled() && (UserPrefPipMode == NO_PIP)
               && IsTwoStageAllowed())
         {
            EnterTwoStageProcessing();
            //need to turn on blender since
            //  ModeSetupDisplayChange(gmvb_MainChannel,...) will not execute
            //  in single pip mode
            ModeSetupBlender(CH_A, DISPLAY_DEFER_TMO_NORMAL);
         }
         else
#endif
            ModeSetupDisplayChange(gmvb_PipChannel, DISPLAY_CHANGE_NORMAL);
      }

#if (FEATURE_BLENDER_FAILSAFE == REV_NONE)
      // Do not turn on blender if in side-by-side and PIP is invalid; otherwise, the bottom
      //   border of highlight border will disappear. Not a problem on FLI8668 due to failsafe.
      if ((IsSideBySidePipMode(UserPrefPipMode)) && !gm_IsValidSignal(gmvb_PipChannel))
      {
         ModeSetupRequestCancel(gmvb_PipChannel, REQ_MS_BLENDER, PARAM_ANY);
         SetDisplayOff(gmvb_PipChannel, BLENDER_OFF);
      }
#endif
      B_FreeRun[gmvb_PipChannel] = FALSE;	// Force Freerun if signal missing

      //
      // Main Change Required (Coming or going from Side-by-Side)
      // Will use main on top of pip when in side be side mode,
      // entering or leaving
      //
      if ((IsSideBySidePipMode(B_PrevPipMode) == TRUE) ||
            (IsSideBySidePipMode(UserPrefPipMode) == TRUE))
      {
		 gm_ClearRegBitsWord(PB_CTRL,PB_MAIN_ENABLE);
		 gm_SyncUpdate(gmvb_MainChannel,OUTPUT_CLOCK);
         ModeSetupDisplayChange(gmvb_MainChannel, DISPLAY_CHANGE_NORMAL);
		 gm_SetRegBitsWord(PB_CTRL,PB_MAIN_ENABLE);
		 gm_SyncUpdate(gmvb_MainChannel,OUTPUT_CLOCK);

			//120813 Edward for reset VWD detection for window size change to PBP will remain the original window at main win 
			#if(FEATURE_VWD == ENABLE)
			if(IsVWDEnabled())
			{
				//VWD_ClearWindowsInfo();	//20120928 Disable by KeivnChen. Use VWD_Stop start function		
				VWD_Stop();
				VWD_Start();
			}
			#endif
		}

//		gm_BlenderSetPriority((gmvb_MainChannel == CH_A)?PIP_ON_MAIN:MAIN_ON_PIP);

		//120622 Edward test , for audio source in PIP and turn off PIP should turn off audio
		if ((UserPrefAudioMainInput == gmd_AUDIO_SRC_AUTO_Pxp) && 
			(B_PrevPipMode != NO_PIP)&&(!Is_PIP_ON()))
			AUDIO_MUTE();

      B_PrevPipMode = UserPrefPipMode;

#ifndef PIP_PORT_INDEPENDENCE
      //
      // Change Pip source to be the same as Main if there is a conflict
      //
      if (!IsMainSourceValid(UserPrefCurrentInputMain))
      {
			#ifdef DELL_U2410F	//Don't chage to same port as main for U2413
			if(UserPrefCurrentInputPip == UserPrefCurrentInputMain)
	   			UserPrefCurrentInputPip = MovePortFromConflictPort(UserPrefCurrentInputMain);
			#else
			UserPrefCurrentInputPip = UserPrefCurrentInputMain;
			#endif
			SelectInput(gmvb_PipChannel, UserPrefCurrentInputPip, NORMAL_MODE);
		}
#endif
	}
	 //120912 Edward temp solution for no PIP window when sub channel no signal or cable and adjust Pip on
	if(IsSinglePipMode(UserPrefPipMode)&& (!gm_BlenderIsOn(gmvb_PipChannel)))
	{
		gm_BlenderOnOff(gmvb_PipChannel,On);
		//gm_Print("## Enable Pip window",0);
	}	
	#if 0
	//gm_SetRegBitsWord(DISPLAY_CONTROL, DTG_RUN_EN);	// 20120912 yvonne for adjust pip garbage
	//20120926 yvonne change to blender screen
	gm_BlenderOnOff(gmvb_MainChannel, ON);
	gm_SyncUpdateWait(gmvb_MainChannel,INPUT_OUTPUT_CLOCK);
	#endif
#if 1//20120924 Modify by KevinChen for disable game mode when V-Scaling.
	{
	if(IsGameModeEnable())
		EnableGameMode();
	else
		{
			if ((UserPrefGameMode == On))
			{//gm_Print("Disable game mode",0);
				UserPrefGameMode = Off;	
				gm_WriteRegWord(SYNC_CONTROL, 2); 							  
				B_DisplayLockMethod = DISP_LOCK_DFL;
				gm_SetRegBitsWord(DDDS1_CONTROL, DDDS1_FORCE_OPLOOP);		    
				UserPrefStateTempFlag = INPUT_COLOR_FORMAT_STATE;		
				ModeSetupRequest(gmvb_MainChannel, REQ_MS_RESTART, PARAM_NONE, 300);				
				UserPrefAdjustInputColorFormatByOSD = TRUE; //20120908 yvonne for not change end user select color format issue.
				//GameModeStatusChanged();	
			}
		}
	}
#endif

#if (FEATURE_BLENDER_FAILSAFE == REV1)
   //Blender failsafe
   //Patch up for rev AA
   if (gm_ReadProductRevReg() == 0)
   {
      if ((UserPrefPipMode == SIDE_BY_SIDE) || (UserPrefPipMode == SIDE_BY_SIDE_TALL))
      {
         gm_BlenderFailSafeOnOff(On);
         gm_SyncUpdateWait(gmvb_PipChannel, OUTPUT_CLOCK);
      }
   }
#endif

} 

//******************************************************************************
//
// FUNCTION       : BOOL IsPipModeAllowed(PipModes PendingPipMode)
//
// USAGE          : Checks if the pip mode system is trying to change to is valid.
//
// INPUT          : PipModes enum (LARGE_PIP, SMALL_PIP, SIDE_BY_SIDE, etc.)
//
// OUTPUT         : TRUE/FALSE
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
BOOL IsPipModeAllowed(PipModes PendingPipMode)
{
#ifdef NO_PIP_SUPPORT
   PendingPipMode = PendingPipMode;	// dummy line to eliminate compiler warning
   return FALSE;
#else
   switch (PendingPipMode)
   {
      case LARGE_PIP:
       case SMALL_PIP:
       case SIDE_BY_SIDE:
      case SIDE_BY_SIDE_TALL:
         return TRUE;
   }
   return FALSE;
#endif	// ifndef NO_PIP_SUPPORT
}


#ifdef Action_AdjustPipPosition_WB_Used
//******************************************************************************
//
// FUNCTION       : void AdjustPipPosition(void)
//
// USAGE          : Changes pip window position to be either one of the four corners of the display
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : B_ModeSetupReq		(R/W)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AdjustPipPosition(void)
{
   if (BlenderIsPending(gmvb_PipChannel) == FALSE)
   {
	  PIPSendMessage(MTO_PIPMHide);
      ModeSetupDisplayChange(gmvb_PipChannel, DISPLAY_CHANGE_NORMAL);
   }
}
#endif

//******************************************************************************
//
// FUNCTION       : PIP Demo function
//
// USAGE          : 
//
// INPUT          : None
//
// OUTPUT         : None
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
#ifndef NO_PIP_SUPPORT
BYTE PIP_Index = 0xFF;
BYTE PIP_sub = 0;
BYTE PIP_Oder = 0;
BYTE PIP_Blend_Level = 0;
BYTE PIP_Demo_Order[] = {1,2,3,4,1,2,5,6,7,8,3,10,11,2,14,1,8,8,1,9,0xFF};

void PIP_Basic_Adjust(void)
{
   if (gm_TimerCheck(BLINKING_LED_TMR) == TIMER_TMO)
   {
      gm_TimerStop(BLINKING_LED_TMR);
      switch (PIP_Index)
      {
         case 0:
            //PIP_Oder = 0;
            PIP_Index = 0;
            PIP_Blend_Level = 0;
            UserPrefPipMode = NO_PIP;
            break;
         case 1:
            UserPrefPipMode = SMALL_PIP;
            break;
         case 2:
            UserPrefPipMode = LARGE_PIP;
            break;
         case 3:
            UserPrefPipMode = SIDE_BY_SIDE;
            break;
         case 4:
            UserPrefPipMode = SIDE_BY_SIDE_TALL;
            break;
         case 5:
            UserPrefPipPosition = TOP_RIGHT;
            break;
         case 6:
            UserPrefPipPosition = TOP_LEFT;
            break;
         case 7:
            UserPrefPipPosition = BOTTOM_LEFT;
            break;
         case 8:
            UserPrefPipPosition = BOTTOM_RIGHT;
            break;
            //case 9:
            //	SwapMainPip();
            //gm_TimerStart(BLINKING_LED_TMR, 20);
            //	break;
      }

      //gm_Delay100ms(30);

      if (PIP_Index == 9)
      {
         SwapMainPip();
         gm_TimerStart(BLINKING_LED_TMR, 20);
      }
      else
      {
         AdjustPipMode();
         gm_TimerStart(BLINKING_LED_TMR, 5);
      }


      PIP_Oder++;
      PIP_Index = PIP_Demo_Order[PIP_Oder];


   }
   //PIP_Index = 0xFF;
}


// Limit of window sizes for side by side
#define SIDEBYSIDE_WIDTH_LIMIT      ((PanelWidth * 9) /10)
#define SIDEBYSIDE_HEIGHT_LIMIT      (PanelHeight - (2 * DYN_MAX_PIXELS_PER_SEQ))
void PIP_Scaling_Adjust(void)
{
   // 10 Up; 11 Doen; 12 Right; 13: Left
   BYTE S_Direction;

   //if(gm_TimerCheck(BLINKING_LED_TMR) == TIMER_TMO)
   {
      gm_TimerStop(BLINKING_LED_TMR);
      S_Direction = PIP_Index - 10;

      if (((B_FocusBorder==gmvb_PipChannel) && (DOWN == S_Direction))
            || ((B_FocusBorder==gmvb_MainChannel) && (UP == S_Direction)))
      {
         if ((gmvw_OutputHActive[gmvb_MainChannel] >= SIDEBYSIDE_WIDTH_LIMIT) ||
               (gmvw_OutputVActive[gmvb_MainChannel] >= SIDEBYSIDE_HEIGHT_LIMIT))
         {
            PIP_sub ++;

            PIP_Oder++;
            PIP_Index = PIP_Demo_Order[PIP_Oder];
            gm_TimerStart(BLINKING_LED_TMR, 5);
         }
      }
      else if (((B_FocusBorder==gmvb_MainChannel) && (DOWN == S_Direction))
               || ((B_FocusBorder==gmvb_PipChannel) && (UP == S_Direction)))
      {
         if ((gmvw_OutputHActive[gmvb_PipChannel] >= SIDEBYSIDE_WIDTH_LIMIT) ||
               (gmvw_OutputVActive[gmvb_PipChannel] >= SIDEBYSIDE_HEIGHT_LIMIT))
         {
            PIP_sub ++;

            PIP_Oder++;
            PIP_Index = PIP_Demo_Order[PIP_Oder];
            gm_TimerStart(BLINKING_LED_TMR, 5);
         }
      }

      if (PIP_sub < 2)
      {
         DynamicScalerSideBySide(S_Direction);

         if (S_Direction >=2)
            PIP_Index = 0xFF;
      }
      else
      {
         //PIP_Index = 0xFF;
         //PIP_Oder++;
         //PIP_Index = PIP_Demo_Order[PIP_Oder];
         PIP_sub = 0;
      }

      //gm_TimerStart(BLINKING_LED_TMR, 5);
   }
}

void PIP_Blend_Adjust(void)
{
   if (gm_TimerCheck(BLINKING_LED_TMR) == TIMER_TMO)
   {
      gm_TimerStop(BLINKING_LED_TMR);
      if (PIP_sub == 0)
      {
         PIP_Blend_Level ++;
         //gm_Print("PIP_Blend_Level = %d", PIP_Blend_Level);
         if (PIP_Blend_Level <= 15)
         {
            gm_BlenderSetLevel(PIP_Blend_Level);
            gm_SyncUpdate(gmvb_MainChannel, OUTPUT_CLOCK);
         }

         if (PIP_Blend_Level == 15)
            PIP_sub = 1;
      }
      else if (PIP_sub == 1)
      {
         PIP_Blend_Level --;
         //gm_Print("PIP_Blend_Level = %d", PIP_Blend_Level);
         if (PIP_Blend_Level !=0)
         {
            gm_BlenderSetLevel(PIP_Blend_Level);
            gm_SyncUpdate(gmvb_MainChannel, OUTPUT_CLOCK);
         }

         if (PIP_Blend_Level == 0)
         {
            PIP_sub = 2;
            gm_BlenderSetLevel(0);
            gm_SyncUpdate(gmvb_MainChannel, OUTPUT_CLOCK);
         }
      }
      gm_TimerStart(BLINKING_LED_TMR, 5);
   }

   if (PIP_sub == 2)
   {
      //PIP_Index = 0xFF;
      PIP_sub = 0;
      PIP_Blend_Level = 0;

      PIP_Oder++;
      PIP_Index = PIP_Demo_Order[PIP_Oder];
      gm_TimerStart(BLINKING_LED_TMR, 5);
   }
}

BOOL PipWinAdjust(BYTE index)
{
#define H_STEP 80
#define V_STEP 60
#define SW_STEP 80
#define SH_STEP 60

   gmt_WindowStruct PipWindow;
   WORD W_Hstart;
   WORD W_Vstart;
   WORD W_Width;
   WORD W_Height;
   BOOL Status = TRUE;
   PipWindow = GetFlexiblePipWindow();
   W_Hstart = PipWindow.W_Hstart;
   W_Vstart = PipWindow.W_Vstart;
   W_Width = PipWindow.W_Width;
   W_Height = PipWindow.W_Height;
   switch (index)
   {
      case 0: // Init
         SetFlexiblePipWindowLimit(PIP_WINDOW_HORIZONTAL_OFFSET,PIP_WINDOW_VERTICAL_OFFSET,
                                   PanelWidth - PIP_WINDOW_HORIZONTAL_OFFSET,PanelHeight - PIP_WINDOW_VERTICAL_OFFSET);
         SetFlexiblePipWindow(PanelWidth-2*PIP_WINDOW_HORIZONTAL_OFFSET-2*PIP_BORDER_WIDTH-800,
                              PanelHeight-2*PIP_WINDOW_VERTICAL_OFFSET-2*PIP_BORDER_HEIGHT-600,800,600);
         UserPrefPipMode = FLEXIBLE_PIP;
         AdjustPipMode();
         Status = FALSE;
         break;
// Pip Relocating
      case 1: // up
         if (W_Vstart >= V_STEP)
            W_Vstart -= V_STEP;
         else
            Status = FALSE;
         break;
      case 2: // down
         W_Vstart += V_STEP;
         break;
      case 3: // left
         if (W_Hstart >= H_STEP)
            W_Hstart -= H_STEP;
         else
            Status = FALSE;
         break;
      case 4: // right
         W_Hstart += H_STEP;
         break;
// Pip Resizing
      case 5: // size up BOTTOM_RIGHT
         W_Width += SW_STEP;
         W_Height += SH_STEP;
         break;
      case 6: // size up TOP_LEFT
         if (W_Hstart >= SW_STEP && W_Vstart >= SH_STEP)
         {
            W_Hstart -= SW_STEP;
            W_Vstart -= SH_STEP;
            W_Width += SW_STEP;
            W_Height += SH_STEP;
         }
         else
            Status = FALSE;
         break;
      case 7: // size up TOP_RIGHT
         if (W_Vstart >= SH_STEP)
         {
            W_Vstart -= SH_STEP;
            W_Width += SW_STEP;
            W_Height += SH_STEP;
         }
         else
            Status = FALSE;
         break;
      case 8: // size up BOTTOM_LEFT
         if (W_Hstart >= SW_STEP)
         {
            W_Hstart -= SW_STEP;
            W_Width += SW_STEP;
            W_Height += SH_STEP;
         }
         else
            Status = FALSE;
         break;
      case 9: // size down BOTTOM_RIGHT
         if (W_Width >= SW_STEP && W_Height >= SH_STEP)
         {
            W_Width -= SW_STEP;
            W_Height -= SH_STEP;
         }
         else
            Status = FALSE;
         break;
      case 10: // size down TOP_LEFT
         if (W_Width >= SW_STEP && W_Height >= SH_STEP)
         {
            W_Vstart += SH_STEP;
            W_Hstart += SW_STEP;
            W_Width -= SW_STEP;
            W_Height -= SH_STEP;
         }
         else
            Status = FALSE;
         break;
      case 11: // size down TOP_RIGHT
         if (W_Width >= SW_STEP && W_Height >= SH_STEP)
         {
            W_Vstart += SH_STEP;
            W_Width -= SW_STEP;
            W_Height -= SH_STEP;
         }
         else
            Status = FALSE;
         break;
      case 12: // size down BOTTOM_LEFT
         if (W_Width >= SW_STEP && W_Height >= SH_STEP)
         {
            W_Hstart += SW_STEP;
            W_Width -= SW_STEP;
            W_Height -= SH_STEP;
         }
         else
            Status = FALSE;
         break;
      default:
         Status = FALSE;
         break;
   }
   if (Status == TRUE)
      Status = SetFlexiblePipWindow(W_Hstart,W_Vstart,W_Width,W_Height);
   return Status;
}

void PipRelocatingResizing(void)
{
   if (gm_TimerCheck(BLINKING_LED_TMR) == TIMER_TMO)
   {
      gm_TimerStop(BLINKING_LED_TMR);
      if (PipWinAdjust(PIP_sub) == FALSE)
         PIP_Index = 0xFF;
      else
      {
         UserPrefPipMode = FLEXIBLE_PIP;
         AdjustPipMode();
      }
      gm_TimerStart(BLINKING_LED_TMR, 5);
   }
}

void PIP_Demo_Handler(void)
{
   if (PIP_Index == 0xFF)
      return;

   if (PIP_Index <= 9)
   {
      PIP_Basic_Adjust();
   }
   else if ((PIP_Index >= 10) && (PIP_Index <= 13))
   {
      PIP_Scaling_Adjust();
   }
   else if (PIP_Index == 14)
   {
      //gm_TimerStart(BLINKING_LED_TMR, 5);
      PIP_Blend_Adjust();
   }
   else if (PIP_Index == 15)
   {
      PipRelocatingResizing();
   }
   else
   {
      PIP_Index = 0xFF;
      PIP_sub = 0;
      PIP_Blend_Level = 0;
      PIP_Oder = 0;
      gm_Print("Invalid command %d", PIP_Index);
   }

}

#endif // NO_PIP_SUPPORT

//*********************************  END  **************************************
