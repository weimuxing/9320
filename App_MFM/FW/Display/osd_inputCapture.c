/*
   $Workfile:   osd_inputCapture.c  $
   $Revision: 1.42 $
   $Date: 2013/06/10 10:03:58 $
*/
#define __OSD_INPUTCAPTURE_C__
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
// MODULE:        osd_inputCapture.c
//
// USAGE:      This module contains all adjustors associated with the input
//              capture such as HTotal, HStart, VStart, etc. and Aspect ratio
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "system\all.h"
#include "StdLibInc\gm_Register.h"
#include "StdLibInc\gm_ACC3.h"

//******************************************************************************
//  E X T E R N S
//******************************************************************************

//******************************************************************************
//  D E F I N E S
//******************************************************************************

#define _DEBUG

#ifdef _DEBUG
#define  msg(a)                  gm_Print(a,0)
#define  msg_i(a,b)               gm_Print(a,b)
#else
#define  msg(a)
#define  msg_i(a,b)
#endif

#define BITMASK_CHANNEL_A      1
#define BITMASK_CHANNEL_B      2

//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************

//******************************************************************************
// S T A T I C   F U N C T I O N S
//******************************************************************************

static void PerformRelocateInputCaptureWindow(gmt_PHY_CH B_Channel, BOOL Bt_IsNewVStartLarger);

//******************************************************************************
//  C O D E
//******************************************************************************

gmt_PHY_CH OtherThanFocus (void)
{
   if (B_FocusBorder == CH_A)
      return (CH_B);
   else
      return (CH_A);
}

#if FEATURE_AIP == ENABLE

#ifdef Action_AdjustHTotal_WB_Used
//******************************************************************************
//
// FUNCTION       : WORD MinHTotal(void)
//
// USAGE          : This function calulates the lower range value for the HTotal
//                  (pixelclock frequency). Minimum HTotal is calculated as
//                  HStart + HActive.  This approximation leaves us with zero
//                  front porch.
//
// INPUT          : None
//
// OUTPUT         : Minimum usable value for HTotal
//
// GLOBALS        : gmvw_InputHStart      (RO)
//                  gmvw_InputHTotal       (RO)
//                  gmvb_MainChannel      (RO)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
WORD MinHTotal(void)
{
   WORD W_Limit = 0;
   if (gm_IsValidSignal(gmvb_MainChannel/*B_FocusBorder*/) == TRUE)
   {
    #if 1
      W_Limit = gmvw_InputHTotalOrg[gmvb_MainChannel/*B_FocusBorder*/] - 60;

    #else
      W_Limit = gmvw_InputHStartOrg[B_FocusBorder]
         + gmvw_InputHActiveOrg[B_FocusBorder];
    #endif
   }
   return(W_Limit);
}

//******************************************************************************
//
// FUNCTION       : WORD MaxHTotal(void)
//
// USAGE          : This function calulates the higher range value for the
//                  HTotal (pixelclock frequency). If the backporch is less
//                  then 10% then min = HTotal +4% if the backporch is greater
//                  then 10% then min = HTotal +10%Calculates the maximum Htotal
//                  value.
//
// INPUT          : None
//
// OUTPUT         : Maximum usable value for HTotal
//
// GLOBALS        : gmvw_InputHStart      (RO)
//                  gmvw_InputHTotal      (RO)
//                  gmvb_MainChannel      (RO)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
WORD MaxHTotal(void)
{
   WORD W_Limit = 0;
   if (gm_IsValidSignal(gmvb_MainChannel/*B_FocusBorder*/) == TRUE)
   {
  
    #if 1
      W_Limit = gmvw_InputHTotalOrg[gmvb_MainChannel/*B_FocusBorder*/] + 60;
  
    #else
      // if back porch less than 10% then MaxHTotal = StdHTotal + 4%
      if ((gmvw_InputHTotalOrg[B_FocusBorder] > 0) && (((DWORD)gmvw_InputHStartOrg[B_FocusBorder] * 100 / gmvw_InputHTotalOrg[B_FocusBorder]) < 10))
         W_Limit = ((gmvw_InputHTotalOrg[B_FocusBorder] * 26 ) / 25);

      // if back porch greater than 10% then MaxHTotal = StdHTotal + 10%
      else
         W_Limit = ((gmvw_InputHTotalOrg[B_FocusBorder] * 11 ) / 10);
    #endif
   }
   return(W_Limit);
}

//******************************************************************************
//
// FUNCTION       : void AdjustHTotal(void)
//
// USAGE          : This function adjusts the input pixel clock frequency if the
//                  value provided by user is in rangeAdjusts the input pixelclock
//                  frequency.
//
// INPUT         : None
//
// OUTPUT      : None
//
// GLOBALS      : UserPrefHTotal, UserPrefCurrentInputMain, UserPrefCurrentInputPip,   (R)
//            : B_FocusBorder   (R)
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AdjustHTotal(void)
{
   //
   // If HTotal is not in Range Abort
   //
   if ((UserPrefHTotal <= MaxHTotal()) && (UserPrefHTotal >= MinHTotal()))
   {      
      SetupHTotal(UserPrefHTotal);
   }
}

//******************************************************************************
//
// FUNCTION       : void AdjustHTotalPip(void)
//
// USAGE          : This function adjusts the input pixel clock frequency if the
//                  value provided by user is in rangeAdjusts the input pixelclock
//                  frequency.
//
// INPUT         : None
//
// OUTPUT      : None
//
// GLOBALS      : UserPrefHTotal_PIP, UserPrefCurrentInputMain,   (R)
//            : UserPrefCurrentInputPip, B_FocusBorder         (R)
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AdjustHTotalPip(void)
{
   //
   // If HTotal is not in Range Abort
   //
   #if 0//120711 Edward merge standard code disable
   if ((UserPrefHTotal_PIP <= MaxHTotal()) && (UserPrefHTotal_PIP >= MinHTotal()))
   {
      
	  SetupHTotal(UserPrefHTotal);
      
      if (UserPrefCurrentInputMain == UserPrefCurrentInputPip)
      {    
         SetupHTotal(OtherThanFocus(), UserPrefHTotal_PIP);
         UserPrefHTotal = UserPrefHTotal_PIP;
      }
   }
   #endif
}
#endif

#ifdef Action_AdjustHSyncPhase_WB_Used
//******************************************************************************
//
// FUNCTION       : void AdjustHSyncPhase(void)
//
// USAGE          : This function adjusts the input clock phase in increments of 1/64.
//                  Adjusts the input clock phase
//
// INPUT      : None
//
// OUTPUT   : None
//
// GLOBALS   : UserPrefHSyncPhase, UserPrefCurrentInputMain, UserPrefCurrentInputPip, (R)
//         : B_FocusBorder,   (R)
//         : gmvb_InputSamplingPhase   (W)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AdjustHSyncPhase(void)
{
   //msg_i("UserPrefHSyncPhase = %d", UserPrefHSyncPhase);
   gmvb_InputSamplingPhase[gmvb_MainChannel/*B_FocusBorder*/] = UserPrefHSyncPhase;
   gm_SetAdcPhase(gmvb_MainChannel/*B_FocusBorder*/, UserPrefHSyncPhase);   //the 2 zeros are arbitrary
}
#endif

#ifdef Action_AdjustHSyncPhasePip_WB_Used
//******************************************************************************
//
// FUNCTION       : void AdjustHSyncPhasePip(void)
//
// USAGE          : This function adjusts the input clock phase in increments of 1/64.
//                  Adjusts the input clock phase
//
// INPUT          : None
//
// OUTPUT         : None
//
// INPUT      : None
//
// OUTPUT   : None
//
// GLOBALS   : UserPrefHSyncPhase_PIP, UserPrefCurrentInputMain,  (R)
//         : UserPrefCurrentInputPip, B_FocusBorder,   (R)
//         : gmvb_InputSamplingPhase   (W)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AdjustHSyncPhasePip(void)
{
 #if 0//120711 Edward merge standard code disable
   //msg_i("UserPrefHSyncPhase = %d", UserPrefHSyncPhase);
   gmvb_InputSamplingPhase[B_FocusBorder] = UserPrefHSyncPhase_PIP;
   gm_SetAdcPhase(B_FocusBorder, UserPrefHSyncPhase_PIP);   //the 2 zeros are arbitrary

   if (UserPrefCurrentInputMain == UserPrefCurrentInputPip)
   {
      gmvb_InputSamplingPhase[OtherThanFocus()] = UserPrefHSyncPhase_PIP;
      gm_SetAdcPhase(OtherThanFocus(), UserPrefHSyncPhase_PIP);
      UserPrefHSyncPhase = UserPrefHSyncPhase_PIP;
   }
 #endif
}
#endif


#ifdef ADJUSTER_HSTART_DECLARED
//******************************************************************************
//
// FUNCTION       : void AdjustHStart(void)
//
// USAGE          : Adjust HStart of IGP channel. User preference variable
//                  UserPrefGrHStart is passed to system function gm_SetHPosition to
//                  set HStart of graphics channel value of IGP_FORCE_UPDATE is written
//                  to register HOST_CONTROL
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS      : UserPrefHSyncDelay, B_FocusBorder      (R)
//              UserPrefCurrentInputMain, UserPrefCurrentInputPip   (R)
//              gmvsw_InputHSyncOffset   (W)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AdjustHStart(void)
{
   //UserPrefHSyncDelay = MaxHStart() - UserPrefHSyncDelay;
   gmvsw_InputHSyncOffset[gmvb_MainChannel/*B_FocusBorder*/] =UserPrefHSyncDelay; //(MaxHStart()-UserPrefHSyncDelay+MinHStart());  //wei 140723   
   gm_InputSetHPosition(gmvb_MainChannel/*B_FocusBorder*/);
}
#endif //#ifdef ADJUSTER_HSTART_DECLARED

#ifdef ADJUSTER_HSTART_PIP_DECLARED
//******************************************************************************
//
// FUNCTION       : void AdjustHStartPip(void)
//
// USAGE          : Adjust HStart of IGP channel. User preference variable
//                  UserPrefGrHStart is passed to system function gm_SetHPosition to
//                  set HStart of graphics channel value of IGP_FORCE_UPDATE is written
//                  to register HOST_CONTROL
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS      : UserPrefHSyncDelay_PIP, B_FocusBorder,      (R) 
//              UserPrefCurrentInputMain, UserPrefCurrentInputPip   (R)
//              gmvsw_InputHSyncOffset   (W)
//
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AdjustHStartPip(void)
{
    #if 0//120711 Edward merge standard code disable
   gm_Print("AdjustHStartPip: %d", UserPrefHSyncDelay_PIP);
   gmvsw_InputHSyncOffset[B_FocusBorder] = UserPrefHSyncDelay_PIP;
   gm_InputSetHPosition(B_FocusBorder);

   if (UserPrefCurrentInputMain == UserPrefCurrentInputPip)
   {
      gmvsw_InputHSyncOffset[OtherThanFocus()] = UserPrefHSyncDelay_PIP;   
      gm_InputSetHPosition(OtherThanFocus());
      UserPrefHSyncDelay = UserPrefHSyncDelay_PIP;
   }
   #endif        
}
#endif //#ifdef ADJUSTER_HSTART_PIP_DECLARED

#ifdef ADJUSTER_HSTART_DECLARED
//******************************************************************************
//
// FUNCTION       : SWORD MinHStart(void)
//
// USAGE          : Returns the minimum allowed horizontal start value for the
//                  current (graphics) mode.
//
// INPUT          : None
//
// OUTPUT         : Minimum allowed horizontal start value for the current (graphics) mode.
//
// GLOBALS        : gmvw_InputHStart   (R)
//                  gmvb_MainChannel   (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
SWORD MinHStart(void)
{
   if (((SWORD)stInput[gmvb_MainChannel/*B_FocusBorder*/].Hstart - 8) > 100)
      return -100;
   else
      return (8 - (SWORD)stInput[gmvb_MainChannel/*B_FocusBorder*/].Hstart);
}
#endif //#ifdef ADJUSTER_HSTART_DECLARED

#ifdef ADJUSTER_HSTART_DECLARED
//******************************************************************************
//
// FUNCTION       : SWORD MaxHStart(void)
//
// USAGE          : Returns the maximum allowed horizontal start value for the
//                  current (graphics) mode.
//
// INPUT          : None
//
// OUTPUT         : Maximum allowed horizontal start value for the current (graphics) mode.
//
// GLOBALS        : gmvw_InputHStart   (R)
//                  gmvb_MainChannel   (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
SWORD MaxHStart(void)
{
   if (((SWORD)stInput[gmvb_MainChannel/*B_FocusBorder*/].Hstart - 8) > 100)
      return 100;
   else
      return ((SWORD)stInput[gmvb_MainChannel/*B_FocusBorder*/].Hstart - 8);
}
#endif //#ifdef ADJUSTER_HSTART_DECLARED

#ifdef ADJUSTER_VSTART_DECLARED
//******************************************************************************
//
// FUNCTION       : void AdjustVStart(void)
//
// USAGE          : Adjust VStart of IGP channel. User preference variable
//                  UserPrefGrVStart is passed to system function gm_SetVPosition to
//                  set new VPosition value of graphics channel value of
//                  IGP_FORCE_UPDATE is copied to register HOST_CONTROL
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS      : UserPrefVSyncDelay   (R) - new setting for VStart of graphics channel
//              UserPrefCurrentInputMain, UserPrefCurrentInputPip   (R)
//              gmvsw_InputVSyncOffset      (W)
//              B_FocusBorder   (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AdjustVStart(void)
{
   gmvsw_InputVSyncOffset[gmvb_MainChannel/*B_FocusBorder*/] = UserPrefVSyncDelay; // (MaxVStart()-UserPrefVSyncDelay+MinVStart()); //wei 140723   
   gm_InputSetVPosition(gmvb_MainChannel/*B_FocusBorder*/);
}
#endif //#ifdef ADJUSTER_VSTART_DECLARED

#ifdef ADJUSTER_VSTART_PIP_DECLARED
//******************************************************************************
//
// FUNCTION       : void AdjustVStartPip(void)
//
// USAGE          : Adjust VStart of IGP channel. User preference variable
//                  UserPrefGrVStart is passed to system function gm_SetVPosition to
//                  set new VPosition value of graphics channel value of
//                  IGP_FORCE_UPDATE is copied to register HOST_CONTROL
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS      : UserPrefVSyncDelay_PIP   (R) - new setting for VStart of graphics channel
//              UserPrefCurrentInputMain, UserPrefCurrentInputPip   (R)
//              gmvsw_InputVSyncOffset      (W)
//              B_FocusBorder   (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AdjustVStartPip(void)
{
 #if 0//120711 Edward merge standard code disable
   gm_Print("AdjustVStartPip: %d", UserPrefVSyncDelay_PIP);
   gmvsw_InputVSyncOffset[B_FocusBorder] = UserPrefVSyncDelay_PIP;
   gm_InputSetVPosition(B_FocusBorder);

   if (UserPrefCurrentInputMain == UserPrefCurrentInputPip)
   {
      gmvsw_InputVSyncOffset[OtherThanFocus()] = UserPrefVSyncDelay_PIP;
      gm_InputSetVPosition(OtherThanFocus());
      UserPrefVSyncDelay = UserPrefVSyncDelay_PIP;
   }    
#endif           
}
#endif //#ifdef ADJUSTER_VSTART_PIP_DECLARED

#ifdef ADJUSTER_VSTART_DECLARED
//******************************************************************************
//
// FUNCTION       : SWORD MinVStart(void)
//
// USAGE          : Returns the minimum allowed vertical start value for the
//                  current (graphics) mode.
//
// INPUT          : None
//
// OUTPUT         : Minimum allowed vertical start value for the current (graphics) mode.
//
// GLOBALS        : gmvw_InputVStart   (R)
//                  gmvb_MainChannel   (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
SWORD MinVStart(void)
{
   if (((SWORD)stInput[gmvb_MainChannel/*B_FocusBorder*/].Vstart - 4) > 100)
      return -100;
   else
      return (4 - (SWORD)stInput[gmvb_MainChannel/*B_FocusBorder*/].Vstart);
}
#endif //#ifdef ADJUSTER_VSTART_DECLARED

#ifdef ADJUSTER_VSTART_DECLARED
//******************************************************************************
//
// FUNCTION       : SWORD MaxVStart(void)
//
// USAGE          : Returns the maximum allowed vertical start value for the
//                  current (graphics) mode.
//
// INPUT          : None
//
// OUTPUT         : Maximum allowed vertical start value for the current (graphics) mode.
//
// GLOBALS        : gmvw_InputVStart   (R)
//                  gmvb_MainChannel   (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
SWORD MaxVStart(void)
{
   if (((SWORD)stInput[gmvb_MainChannel/*B_FocusBorder*/].Vstart - 4) > 100)
      return 100;
   else
      return ((SWORD)stInput[gmvb_MainChannel/*B_FocusBorder*/].Vstart - 4);
}
#endif //#ifdef ADJUSTER_VSTART_DECLARED

#else // FEATURE_AIP
void AdjustHTotal(void){}
void AdjustHTotalPip(void){}
void AdjustHSyncPhase(void){}
void AdjustHSyncPhasePip(void){}
void AdjustHStart(void){}
void AdjustHStartPip(void){}
void AdjustVStart(void){}
void AdjustVStartPip(void){}
WORD MinHTotal(void){return 0;}
WORD MaxHTotal(void){return 0;}
SWORD MinHStart(void){return 0;}
SWORD MaxHStart(void){return 0;}
SWORD MinVStart(void){return 0;}
SWORD MaxVStart(void){return 0;}

#endif // FEATURE_AIP == ENABLE

#ifdef ADJUSTER_PIPHSTART_DECLARED
//******************************************************************************
//
// FUNCTION       : void AdjustPipHStart(void)
//
// USAGE          : Adjust Pip HStart
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : B_ModeSetupReq      (R/W)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AdjustPipHStart(void)
{
   ModeSetupInputChange(gmvb_PipChannel);
}
#endif //#ifdef ADJUSTER_HSTART_DECLARED

#ifdef ADJUSTER_PIPHSTART_DECLARED
//******************************************************************************
//
// FUNCTION       : WORD MaxPipHStart(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         :
//
// GLOBALS        : gmvw_OutputHStart      (R)
//                  gmvb_PipChannel         (R)
//                  gmvw_OutputHActive      (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
WORD MaxPipHStart(void)
{
   return (gmvw_OutputHStart[gmvb_PipChannel] + UserPrefPipHStart -(PanelWidth - gmvw_OutputHActive[gmvb_PipChannel]));
}
#endif //#ifdef ADJUSTER_VSTART_DECLARED

#ifdef ADJUSTER_PIPHSTART_DECLARED
//******************************************************************************
//
// FUNCTION       : WORD MinPipHStart(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         :
//
// GLOBALS        : gmvw_OutputHStart      (R)
//                  gmvb_PipChannel         (R)
//                  UserPrefPipHStart      (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
WORD MinPipHStart(void)
{
   return (0 - gmvw_OutputHStart[gmvb_PipChannel] + UserPrefPipHStart);
}
#endif //#ifdef ADJUSTER_VSTART_DECLARED

#ifdef ADJUSTER_PIPVSTART_DECLARED
//******************************************************************************
//
// FUNCTION       : void AdjustPipVStart(void)
//
// USAGE          : Adjust Pip VStart
//
// INPUT          : None
//
// OUTPUT         :
//
// GLOBALS        : B_ModeSetupReq      (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AdjustPipVStart(void)
{
   ModeSetupInputChange(gmvb_PipChannel);
   //ModeSetupDisplayChange(gmvb_PipChannel, 0);
   /*
      gmvw_OutputVStart[gmvb_PipChannel] = gmvw_OutputVStart[gmvb_PipChannel] + UserPrefPipVStart;

      gm_SetupInputPip();

      gm_SyncUpdate(gmvb_PipChannel, OUTPUT_CLOCK);
   */
}
#endif //#ifdef ADJUSTER_HSTART_DECLARED

#ifdef ADJUSTER_PIPVSTART_DECLARED
//******************************************************************************
//
// FUNCTION       : WORD MaxPipVStart(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         :
//
// GLOBALS        : gmbw_OutputVStart      (R)
//                  gmvb_PipChannel         (R)
//                  gmvw_OutputVActive      (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
WORD MaxPipVStart(void)
{
   return (gmvw_OutputVStart[gmvb_PipChannel] + UserPrefPipVStart -(PanelHeight - gmvw_OutputVActive[gmvb_PipChannel]));
}
#endif //#ifdef ADJUSTER_VSTART_DECLARED

#ifdef ADJUSTER_PIPVSTART_DECLARED
//******************************************************************************
//
// FUNCTION       : WORD MinPipVStart(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         :
//
// GLOBALS        : gmvw_OutputVSTart      (R)
//                  gmvb_PipChannel         (R)
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
WORD MinPipVStart(void)
{
   return (0 - gmvw_OutputVStart[gmvb_PipChannel] + UserPrefPipVStart);
}
#endif //#ifdef ADJUSTER_VSTART_DECLARED


//******************************************************************************
//
// FUNCTION       : void AutoAdjust(void)
//
// USAGE          : Performs Auto Adjustment
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
void AutoAdjust(void)
{
#if FEATURE_AIP == ENABLE

   gmt_PHY_CH B_Channel;
   BOOL B_PerformAutoAdjust = FALSE;

   //check to see if Graphics mode/zoom in
   if (!IsZoom())
   {
      if (IsInputVGA(gmvb_MainChannel))
      {
         B_Channel = gmvb_MainChannel;
          if (gm_IsValidSignal(gmvb_MainChannel))
         {
	  #ifdef DELL_U2410F
	 	//Neil 20081124+ only VGA input do AUTO function
		if ((((B_Channel == gmvb_MainChannel) && (UserPrefCurrentInputMain == ALL_BOARDS_VGA1)) || 
			((B_Channel == gmvb_PipChannel) && (UserPrefCurrentInputPip == ALL_BOARDS_VGA1)))
			&&!gm_IsInterlaced(B_Channel))	//Neil 20081201+	 
	 #endif		
        	 	B_PerformAutoAdjust = TRUE;
         }
      }     
      else if (IsInputVGA(gmvb_PipChannel))
      {
          B_Channel = gmvb_PipChannel;

         if (gm_IsValidSignal(gmvb_PipChannel) && (UserPrefPipMode != NO_PIP))
         {
	 #ifdef DELL_U2410F	 
		 //Neil 20081201* only VGA input do AUTO function
		if (UserPrefCurrentInputPip == ALL_BOARDS_VGA1&&!gm_IsInterlaced(B_Channel))
	 #endif		
         		B_PerformAutoAdjust = TRUE;
         }
      }
   }

   if (B_PerformAutoAdjust)
   {
      //OSDRequestMsg(B_Channel, MTO_AutoAdjustStart, REQUEST_NOW);
      ModeSetupRequest(B_Channel, REQ_MS_AUTO_ADJUST, PARAM_NONE, 40);	
   }
   
#endif
}



#ifdef Action_AdjustAspectRatioMain_WB_Used
//******************************************************************************
//
// FUNCTION       : void AdjustAspectRatioMain(void)
//
// USAGE          : Adjusts main window's aspect ratio conversion
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : B_ModeSetupReq      (R/W)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
#ifdef ST_4K2K_93xx_BOARD
BOOL CheckInputAspectRatio(void)
{
   DWORD temp1, temp2;

   temp1 = (DWORD)PanelWidth*100 / PanelHeight;

   if(gm_IsInterlaced(gmvb_MainChannel))
   {
      if(UserPrefCaptureM == CAPTURE_MODE_HALF_HORIZONTAL)
         temp2 = (DWORD)gmvw_InputHActiveOrg[gmvb_MainChannel]*50 / gmvw_InputVActiveOrg[gmvb_MainChannel];
      else
         temp2 = (DWORD)gmvw_InputHActiveOrg[gmvb_MainChannel]*25 / gmvw_InputVActiveOrg[gmvb_MainChannel];   
   }
   else
   {
      if(UserPrefCaptureM == CAPTURE_MODE_HALF_HORIZONTAL)
         temp2 = (DWORD)gmvw_InputHActiveOrg[gmvb_MainChannel]*100 / gmvw_InputVActiveOrg[gmvb_MainChannel];
      else
         temp2 = (DWORD)gmvw_InputHActiveOrg[gmvb_MainChannel]*50 / gmvw_InputVActiveOrg[gmvb_MainChannel];
   }


   if(temp1 >= temp2)
   {
      return TRUE; // Panel Aspect is bigger.
   }
   else
   {
      return FALSE;
   }
}

void LVDS_PLL_STABLE_WAIT(void)
{
   BYTE B_StableCount = 0;
   
   while(gm_ReadRegWord(LVDS_DLL_LOCK_STATUS) == 0)
   {
      if(B_StableCount > 50)
      {
         gm_Printf("LVDS PLL Unable to Get Stable", 0);
         break;
      }
      B_StableCount++;
      gm_Delay1ms(1);
   }

   gm_Printf("LVDS PLL Stable Count %d", B_StableCount);
}

void OutputAspectOverride(void)
{
   WORD W_HStart_Check = 0;
   WORD W_VStart_Check = 0;
   WORD W_Width_Check = 0;
   WORD W_Length_Check = 0;

   W_Width_Check = gmvw_OutputHActive[gmvb_MainChannel];
   W_Length_Check = gmvw_OutputVActive[gmvb_MainChannel];
   W_HStart_Check  = gmvw_OutputHStart[gmvb_MainChannel];
   W_VStart_Check  = gmvw_OutputVStart[gmvb_MainChannel];
   
   //
   // Setup Dynamic scaling size Start
   //
   SetDynamicScalerSizeBegin(gmvb_MainChannel);   
   
   #if 1 // FE always 1:1
   if(GetChipID_4K2K() == CHIPID_4K2K_FE)
   {
      gmvw_OutputHActive[gmvb_MainChannel] = gmvw_InputHActiveOrg[gmvb_MainChannel];      
      if(gm_IsInterlaced(gmvb_MainChannel))
         gmvw_OutputVActive[gmvb_MainChannel] = gmvw_InputVActiveOrg[gmvb_MainChannel]*2;
      else
         gmvw_OutputVActive[gmvb_MainChannel] = gmvw_InputVActiveOrg[gmvb_MainChannel];
      gmvw_OutputVStart[gmvb_MainChannel] = 0;
      gmvw_OutputHStart[gmvb_MainChannel] = 0;

      if((gm_ReadRegWord(BYPASS) & PASS_THROUGH) == 0) //Add by ChengLin 20130327 for solve 4k2k timing can't show out during AC ON 
      {
         gm_WriteRegWord(DH_DE_END, gm_ReadRegWord(DH_DE_START)+gmvw_OutputHActive[gmvb_MainChannel]);
         gm_WriteRegWord(DV_DE_END, gm_ReadRegWord(DV_DE_START)+gmvw_OutputVActive[gmvb_MainChannel]);
      }

   	#if ENABLE_VEGA_CONTROL_VIA_ATHENA_FE
      {
         WORD W_VEGA_LVDS_MODE = SOCIPC_VEGA_I2C_ReadWord_Via_FE(0x0C);
               
         gm_ClearRegBitsDWord(LVDS_DIGITAL_CTRL, LVTX_HS_DEL_ADJ);
         
         if((PanelMaxPClk < 90000) && ((gm_ReadRegWord(BYPASS) & PASS_THROUGH) == 0)) // 86000)
         {                     
            gm_SetRegBitsWord(LVDS_DLL_CTRL_FREQ, LVDS_DLL_PD);
            
            if(PanelMaxPClk < 45000) // 43000)
            {
               if((gm_IsInterlaced(gmvb_MainChannel)) || (PanelMaxPClk < 23000))
               {
                  if(PanelMaxPClk < 12000)
                  {
                     gm_WriteRegDWord_LE(LVDS_DLL_CTRL_FREQ, 0x2704);
                  }
                  else
               	{
                  	gm_WriteRegDWord_LE(LVDS_DLL_CTRL_FREQ, 0x2384);              
               	}
               }
               else               
               {
                  gm_WriteRegDWord_LE(LVDS_DLL_CTRL_FREQ, 0x21C4);
               }
            }
            else
            {
               if(gm_IsInterlaced(gmvb_MainChannel))
               {
                  gm_WriteRegDWord_LE(LVDS_DLL_CTRL_FREQ, 0x11C4);              
               }
               else            
               {
                  gm_WriteRegDWord_LE(LVDS_DLL_CTRL_FREQ, 0x10E4);
               }
            }

				gm_ClearAndSetBitsDWord(LVDS_DIGITAL_CTRL, WIDTH_SEL, 0); // Single bus
            gm_WriteRegDWord_LE(LVDS_BUS_AB_PAD_CTRL, 0xAAA0AAA);
            gm_WriteRegDWord_LE(LVDS_BUS_CD_PAD_CTRL, 0); // shut down unused LVDS channel
				gm_ClearRegBitsWord(LVDS_DIGITAL_CTRL, A_B_SWAP|C_D_SWAP);
            gm_WriteRegDWord_LE(LVDS_CLOCK_CONFIG, 0xC);
            gm_ClearRegBitsWord(LVDS_DLL_CTRL_FREQ, LVDS_DLL_PD);

            LVDS_PLL_STABLE_WAIT();
            
            //if((W_VEGA_LVDS_MODE&0xFFFC) != 0)//ML
            {
               W_VEGA_LVDS_MODE = W_VEGA_LVDS_MODE & 0xFFFC;
               SOCIPC_VEGA_I2C_WriteWord_Via_FE(0x0C, W_VEGA_LVDS_MODE); // Single Bus               
            }
         }
         else
         {
            if((PanelMaxPClk < 180000) && ((gm_ReadRegWord(BYPASS) & PASS_THROUGH) == 0)) // 172000)
            {
               gm_SetRegBitsWord(LVDS_DLL_CTRL_FREQ, LVDS_DLL_PD);
               
               if(gm_IsInterlaced(gmvb_MainChannel))
               {
                  gm_WriteRegDWord_LE(LVDS_DLL_CTRL_FREQ, 0x11C8);              
               }
               else
               {
                  gm_WriteRegDWord_LE(LVDS_DLL_CTRL_FREQ, 0x10E8);
               }
               
               #if 1 // adjust LVDS HS delay can cover 2 channel LVDS shift 1 pixel issue.
               gm_SetRegBitsDWord(LVDS_DIGITAL_CTRL, BIT24);                           
               #else
               gm_WriteRegWord(DH_ACTIVE_START, gm_ReadRegWord(DH_ACTIVE_START) + 1);
               gm_WriteRegWord(DH_DE_START,     gm_ReadRegWord(DH_DE_START) + 1);
               gm_WriteRegWord(DH_DE_END,       gm_ReadRegWord(DH_DE_END) + 1);               
               #endif

					gm_ClearAndSetBitsDWord(LVDS_DIGITAL_CTRL, WIDTH_SEL, 1);
               gm_WriteRegDWord_LE(LVDS_BUS_AB_PAD_CTRL, 0xAAA0000); // shut down unused LVDS channel
               gm_WriteRegDWord_LE(LVDS_BUS_CD_PAD_CTRL, 0xAAA); // shut down unused LVDS channel
					gm_SetRegBitsWord(LVDS_DIGITAL_CTRL, A_B_SWAP|C_D_SWAP);
               gm_WriteRegDWord_LE(LVDS_CLOCK_CONFIG, 0x14);
               gm_ClearRegBitsWord(LVDS_DLL_CTRL_FREQ, LVDS_DLL_PD);
               LVDS_PLL_STABLE_WAIT();
               
               // if((W_VEGA_LVDS_MODE&0xFFFC) != BIT0)//ML
               {
                  W_VEGA_LVDS_MODE = (W_VEGA_LVDS_MODE & 0xFFFC)|BIT0;
                  SOCIPC_VEGA_I2C_WriteWord_Via_FE(0x0C, W_VEGA_LVDS_MODE); // Dual bus                              
               }            
            }
            else
            {
               #if 0
               if(PanelMaxPClk < 172000)
               {
                  if(gm_IsInterlaced(gmvb_MainChannel))
                  {
                     gm_WriteRegDWord_LE(LVDS_DLL_CTRL_FREQ, 0x11C8);              
						}
                  else
                  {
                     gm_WriteRegDWord_LE(LVDS_DLL_CTRL_FREQ, 0x10E8);
						}
               }
               else
               #endif
					{
                  gm_SetRegBitsWord(LVDS_DLL_CTRL_FREQ, LVDS_DLL_PD);
                     
                  if(gm_IsInterlaced(gmvb_MainChannel))
						{
                     gm_WriteRegDWord_LE(LVDS_DLL_CTRL_FREQ, 0xE8);              
                  }
                  else
                  {
                     gm_WriteRegDWord_LE(LVDS_DLL_CTRL_FREQ, 0x78);                  
                  }
               }

               gm_ClearAndSetBitsDWord(LVDS_DIGITAL_CTRL, WIDTH_SEL, LvdsBusType);
               gm_WriteRegDWord_LE(LVDS_BUS_AB_PAD_CTRL, 0xAAA0AAA);
               gm_WriteRegDWord_LE(LVDS_BUS_CD_PAD_CTRL, 0xAAA0AAA);
               gm_ClearRegBitsWord(LVDS_DIGITAL_CTRL, A_B_SWAP|C_D_SWAP);
               if((gm_ReadRegWord(BYPASS) & PASS_THROUGH) == 0)
               {
               gm_WriteRegDWord_LE(LVDS_CLOCK_CONFIG, 0x14);
               }
               else
               {
                  gm_WriteRegDWord_LE(LVDS_CLOCK_CONFIG, 0x16);
               }
               gm_ClearRegBitsWord(LVDS_DLL_CTRL_FREQ, LVDS_DLL_PD);
               LVDS_PLL_STABLE_WAIT();
              
               // if((W_VEGA_LVDS_MODE&0xFFFC) != BIT1)//ML
               {
                  W_VEGA_LVDS_MODE = (W_VEGA_LVDS_MODE & 0xFFFC)|BIT1;
                  SOCIPC_VEGA_I2C_WriteWord_Via_FE(0x0C, W_VEGA_LVDS_MODE); // Quad bus            
               }
            }            
         }
       }
   #endif  
      gm_ForceUpdate(gmvb_MainChannel,OUTPUT_CLOCK); 
   }
   else
   #endif
   {
   if(UserPrefOutputAspectRatio == OUTPUT_AR_1_1)
   {                        
         if(stInput[gmvb_MainChannel].Hactive < PanelWidth) // for HDMI 4096x2160
      		gmvw_OutputHActive[gmvb_MainChannel] = stInput[gmvb_MainChannel].Hactive;
         else
            gmvw_OutputHActive[gmvb_MainChannel] = PanelWidth;

         if(gm_IsInterlaced(gmvb_MainChannel))
         {
            gmvw_OutputVActive[gmvb_MainChannel] = stInput[gmvb_MainChannel].Vactive * 2;
         }
         else
      		gmvw_OutputVActive[gmvb_MainChannel] = stInput[gmvb_MainChannel].Vactive;
         
         gmvw_OutputVStart[gmvb_MainChannel] = (PanelHeight - gmvw_OutputVActive[gmvb_MainChannel])/2;
   }
   else if(UserPrefOutputAspectRatio == OUTPUT_AR_4_3)
   {
		#if 0 // FE always 1:1
      if(GetChipID_4K2K() == CHIPID_4K2K_FE)
      {
         gmvw_OutputVStart[gmvb_MainChannel] = 0;
         gmvw_OutputHActive[gmvb_MainChannel] = PanelWidth;
         gmvw_OutputVActive[gmvb_MainChannel] = PanelHeight;         
      }
      else
		#endif
		{
         if(UserPrefCaptureM == CAPTURE_MODE_HALF_HORIZONTAL)
         {
            if((PanelHeight/3) > (PanelWidth/4))
            {
               gmvw_OutputHActive[gmvb_MainChannel] = PanelWidth;
               gmvw_OutputVActive[gmvb_MainChannel] = PanelWidth*3/4;         
            }
            else
            {
               gmvw_OutputVActive[gmvb_MainChannel] = PanelHeight;
               gmvw_OutputHActive[gmvb_MainChannel] = PanelHeight*4/3;         
            }         
         }
         else
         {
            if((PanelHeight/3) > (PanelWidth/2))
            {
               gmvw_OutputHActive[gmvb_MainChannel] = PanelWidth;
               gmvw_OutputVActive[gmvb_MainChannel] = PanelWidth*3/2;         
            }
            else
            {
               gmvw_OutputVActive[gmvb_MainChannel] = PanelHeight;
               gmvw_OutputHActive[gmvb_MainChannel] = PanelHeight*2/3;         
            }
         }

         gmvw_OutputVStart[gmvb_MainChannel] = (PanelHeight - gmvw_OutputVActive[gmvb_MainChannel]) / 2;
      }
   }
   else if(UserPrefOutputAspectRatio == OUTPUT_AR_AUTO)
   {
      if(CheckInputAspectRatio())
      {
         gmvw_OutputVActive[gmvb_MainChannel] = PanelHeight;
         //gm_Printf("OutputAspectOverride PanelHeight", 0);
         #if 0 // FE always 1:1
         if(GetChipID_4K2K() == CHIPID_4K2K_FE)
         {
            gmvw_OutputHActive[gmvb_MainChannel] = (WORD)(((DWORD)gmvw_InputHActiveOrg[gmvb_MainChannel])*PanelHeight/gmvw_InputVActiveOrg[gmvb_MainChannel]);
         }
         else
         #endif
         {
            if((DpRxIsMST_Stream(UserPrefCurrentInputMain) == TRUE) || (UserPrefCaptureM == CAPTURE_MODE_HALF_HORIZONTAL))
				{
					if(gm_IsInterlaced(gmvb_MainChannel))
						gmvw_OutputHActive[gmvb_MainChannel] = (WORD)((((DWORD)gmvw_InputHActiveOrg[gmvb_MainChannel])*PanelHeight/(gmvw_InputVActiveOrg[gmvb_MainChannel]*2)));
					else
               	gmvw_OutputHActive[gmvb_MainChannel] = (WORD)((((DWORD)gmvw_InputHActiveOrg[gmvb_MainChannel])*PanelHeight/gmvw_InputVActiveOrg[gmvb_MainChannel]));
				}
				else
				{
					if(gm_IsInterlaced(gmvb_MainChannel))
						gmvw_OutputHActive[gmvb_MainChannel] = (WORD)((((DWORD)gmvw_InputHActiveOrg[gmvb_MainChannel])*PanelHeight/(gmvw_InputVActiveOrg[gmvb_MainChannel]*2))/2);
            	else
						gmvw_OutputHActive[gmvb_MainChannel] = (WORD)((((DWORD)gmvw_InputHActiveOrg[gmvb_MainChannel])*PanelHeight/gmvw_InputVActiveOrg[gmvb_MainChannel])/2);
				}
      	}
      }
      else
      {
         gmvw_OutputHActive[gmvb_MainChannel] = PanelWidth;
         gm_Printf("OutputAspectOverride PanelWidth", 0);
         #if 0 // FE always 1:1
         if(GetChipID_4K2K() == CHIPID_4K2K_FE)
         {
            gmvw_OutputVActive[gmvb_MainChannel] = (WORD)(((DWORD)gmvw_InputVActiveOrg[gmvb_MainChannel])*PanelWidth/gmvw_InputHActiveOrg[gmvb_MainChannel]);      
         }
         else
         #endif
         {
				if(gm_IsInterlaced(gmvb_MainChannel))
				{
					if((DpRxIsMST_Stream(UserPrefCurrentInputMain) == TRUE) || (UserPrefCaptureM == CAPTURE_MODE_HALF_HORIZONTAL))
						gmvw_OutputVActive[gmvb_MainChannel] = (WORD)(((DWORD)gmvw_InputVActiveOrg[gmvb_MainChannel])*2*PanelWidth/gmvw_InputHActiveOrg[gmvb_MainChannel]);
					else
						gmvw_OutputVActive[gmvb_MainChannel] = (WORD)(((DWORD)gmvw_InputVActiveOrg[gmvb_MainChannel])*2*PanelWidth*2/gmvw_InputHActiveOrg[gmvb_MainChannel]);               
				}
				else
				{
            	if((DpRxIsMST_Stream(UserPrefCurrentInputMain) == TRUE) || (UserPrefCaptureM == CAPTURE_MODE_HALF_HORIZONTAL))
               	gmvw_OutputVActive[gmvb_MainChannel] = (WORD)(((DWORD)gmvw_InputVActiveOrg[gmvb_MainChannel])*PanelWidth/gmvw_InputHActiveOrg[gmvb_MainChannel]);
            	else
            		gmvw_OutputVActive[gmvb_MainChannel] = (WORD)(((DWORD)gmvw_InputVActiveOrg[gmvb_MainChannel])*PanelWidth*2/gmvw_InputHActiveOrg[gmvb_MainChannel]);
      		}
      	}
      }
      gmvw_OutputVStart[gmvb_MainChannel] = (PanelHeight - gmvw_OutputVActive[gmvb_MainChannel]) / 2;
   }
   else
   {
      gmvw_OutputVStart[gmvb_MainChannel] = 0;
      gmvw_OutputHActive[gmvb_MainChannel] = PanelWidth;
      gmvw_OutputVActive[gmvb_MainChannel] = PanelHeight;         
   }   

      if(GetChipID_4K2K() == CHIPID_4K2K_L)
      {
         if(UserPrefCaptureM == CAPTURE_MODE_HALF_HORIZONTAL)
            gmvw_OutputHStart[gmvb_MainChannel] = (PanelWidth - gmvw_OutputHActive[gmvb_MainChannel])/2;
         else            
            gmvw_OutputHStart[gmvb_MainChannel] = PanelWidth - gmvw_OutputHActive[gmvb_MainChannel];
      }
      else
      {
         if(UserPrefCaptureM == CAPTURE_MODE_HALF_HORIZONTAL)
            gmvw_OutputHStart[gmvb_MainChannel] = (PanelWidth - gmvw_OutputHActive[gmvb_MainChannel])/2;
         else
            gmvw_OutputHStart[gmvb_MainChannel] = 0;
      }      
   }

   #if 0 // FE always 1:1
   if(GetChipID_4K2K() == CHIPID_4K2K_FE)
   {
      gmvw_OutputHStart[gmvb_MainChannel] = (PanelWidth - gmvw_OutputHActive[gmvb_MainChannel]) / 2;               
   }
   else if(GetChipID_4K2K() == CHIPID_4K2K_L)
   {
      if(UserPrefCaptureM == CAPTURE_MODE_HALF_HORIZONTAL)
         gmvw_OutputHStart[gmvb_MainChannel] = (PanelWidth - gmvw_OutputHActive[gmvb_MainChannel])/2;
      else
      	gmvw_OutputHStart[gmvb_MainChannel] = PanelWidth - gmvw_OutputHActive[gmvb_MainChannel];
   }
   else
   {
      if(UserPrefCaptureM == CAPTURE_MODE_HALF_HORIZONTAL)
         gmvw_OutputHStart[gmvb_MainChannel] = (PanelWidth - gmvw_OutputHActive[gmvb_MainChannel])/2;
      else
      	gmvw_OutputHStart[gmvb_MainChannel] = 0;
   }
   #endif

   //gm_Printf("OutputAspectOverride UserPrefCaptureM %d", UserPrefCaptureM);
   //gm_Printf("OutputAspectOverride UserPrefOutputAspectRatio %d", UserPrefOutputAspectRatio);
   //gm_Printf("OutputAspectOverride gmvw_OutputHStart[gmvb_MainChannel] %d", gmvw_OutputHStart[gmvb_MainChannel]);
   //gm_Printf("OutputAspectOverride gmvw_OutputVStart[gmvb_MainChannel] %d", gmvw_OutputVStart[gmvb_MainChannel]);
   //gm_Printf("OutputAspectOverride gmvw_OutputHActive[gmvb_MainChannel] %d", gmvw_OutputHActive[gmvb_MainChannel]);
   //gm_Printf("OutputAspectOverride gmvw_OutputVActive[gmvb_MainChannel] %d", gmvw_OutputVActive[gmvb_MainChannel]);

   if(
      (W_Width_Check == gmvw_OutputHActive[gmvb_MainChannel]) &&
      (W_Length_Check == gmvw_OutputVActive[gmvb_MainChannel]) &&
      (W_HStart_Check  == gmvw_OutputHStart[gmvb_MainChannel]) &&
      (W_VStart_Check  == gmvw_OutputVStart[gmvb_MainChannel])
   )
   {
      //gm_Printf("\n", 0);
      //gm_Printf("No Output Override Again", 0);
      //gm_Printf("\n", 0);
      return; // no change no need to do again
   }

   SetDynamicScalerStep(gmvb_MainChannel, 2);
   SetDynamicScalerStep(gmvb_PipChannel, 0);   // turn off pip scaling

   //
   // Setup Dynamic scaling size end
   //
   SetDynamicScalerSizeEnd(gmvb_MainChannel);		 
  
   DynamicScalerStart(gmvb_MainChannel);
   DynamicScalerProcess(FALSE);
   DynamicScalerStop();
}

void AdjustOutputAspect(void)
{      
   SetDisplayOff(gmvb_MainChannel, BACKGROUND_ONLY);
   Image_ReadyFlag(FALSE);

   OutputAspectOverride();
   
   // Request Blenders ON and User Prefs
   if(GetChipID_4K2K() == CHIPID_4K2K_R)
   {
	   ModeSetupBlender(gmvb_MainChannel, DISPLAY_DEFER_TMO_NORMAL);
   }
   else if(GetChipID_4K2K() == CHIPID_4K2K_FE)
   {
      ModeSetupBlender(gmvb_MainChannel, DISPLAY_DEFER_TMO_NORMAL+60); // FE need to slower then R/L
   }
   else
   {
      gm_SyncUpdateWait(gmvb_MainChannel, OUTPUT_CLOCK);
		#if(ENABLE_ATHENA_R_L_IMAGE_SYNC_HANDLER == 1)
      ImageSyncHandleSetState(IMAGE_OFF_STATE); //Add by ChengLin 20130314 for fix Athena Left can't disbale black screen in PBP mode
      Set4K2K_ImageSyncReady(); 
      #endif
      SetDisplayOn(gmvb_MainChannel);      
      #if 0 // ENABLE_ATHENA_R_L_IMAGE_SYNC_HANDLER
		Image_output_Request(); //Trigger Athena Right INT1
		Image_Force_OnOff(TRUE);
      #endif
   }
}
#endif

#ifdef DELL_U2410F	//Neil 20081219* don't change input setting since mode alread setup, otherwise will conflict with zoom setting
void AdjustAspectRatioMain(void)
{
#ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()==CHIPID_4K2K_R)
   {
	#if ENABLE_SOC_IPC_BUSY_CHECK
	/*ChengLin 20130124: New 4k2k board BUSY status check*/
	if(!SocIPCIsSlaveLeftReady())
		return;
	#endif  
	   AdjustOutputAspect();
   	SOCIPC_SyncOSDAdjustHandler( SI_OSD_SYNC_DISPLAY_SETUP );	
#else

	if (!BlenderIsPending(gmvb_MainChannel)
	#ifndef NO_TUNER
		&& !IsProgramBlocked(gmvb_MainChannel)
	#endif
		)
	#if (FEATURE_VWD == ENABLE) //20120924 Move to here by KevinChen for Change AspecRatio VWD enhance position not renew.
	VWD_Stop();
	VWD_Start();
	#endif
	{	//Neil 20081219 just change output to do aspect change, No WSS
		gm_InputWaitVsync(gmvb_MainChannel, 1);
		SelectAspectMain();
		ModeSetupDisplayChange(gmvb_MainChannel, DISPLAY_CHANGE_NORMAL);
	}
	#if 1//20120924 Modify by KevinChen for disable game mode when V-Scaling.
	{
		if(IsGameModeEnable())
			EnableGameMode();
		else
		{
			if ((UserPrefGameMode == On))
			{
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
#endif	
}
#else
void AdjustAspectRatioMain(void)
{
    if (!BlenderIsPending(gmvb_MainChannel)
        )
   {
      gmt_WindowStruct PrevInputDim;
      gmt_WindowStruct NewInputDim;
      PrevInputDim.W_Hstart = stInput[gmvb_MainChannel].Hstart;
      PrevInputDim.W_Width = stInput[gmvb_MainChannel].Hactive;
      PrevInputDim.W_Vstart = stInput[gmvb_MainChannel].Vstart;
      PrevInputDim.W_Height = stInput[gmvb_MainChannel].Vactive;

      gm_InputWaitVsync(gmvb_MainChannel, 1);
      SelectAspectMain();
      SetCapture(gmvb_MainChannel);

      NewInputDim.W_Hstart = stInput[gmvb_MainChannel].Hstart;
      NewInputDim.W_Width = stInput[gmvb_MainChannel].Hactive;
      NewInputDim.W_Vstart = stInput[gmvb_MainChannel].Vstart;
      NewInputDim.W_Height = stInput[gmvb_MainChannel].Vactive;

      RelocateInputCaptureWindow(gmvb_MainChannel, &PrevInputDim, &NewInputDim);
      ModeSetupDisplayChange(gmvb_MainChannel, DISPLAY_CHANGE_NORMAL);
   }
}
#endif
#endif

#ifdef Action_AdjustAspectRatioPip_WB_Used
//******************************************************************************
//
// FUNCTION       : void AdjustAspectRatioPip(void)
//
// USAGE          : Adjusts pip window's aspect ratio conversion
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : B_ModeSetupReq      (R/W)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AdjustAspectRatioPip(void)
{
#ifndef NO_PIP_SUPPORT
   if (BlenderIsPending(gmvb_PipChannel) == FALSE)
   {
      gmt_WindowStruct PrevInputDim;
      gmt_WindowStruct NewInputDim;

      PrevInputDim.W_Hstart = stInput[gmvb_PipChannel].Hstart;
      PrevInputDim.W_Width = stInput[gmvb_PipChannel].Hactive;
      PrevInputDim.W_Vstart = stInput[gmvb_PipChannel].Vstart;
      PrevInputDim.W_Height = stInput[gmvb_PipChannel].Vactive;

      gm_InputWaitVsync(gmvb_PipChannel, 1);

      SelectAspectPip();
      SetCapture(gmvb_PipChannel);
      ModeSetupRequestCancel(gmvb_PipChannel, REQ_MS_BLENDER, PARAM_ANY);
      SetDisplayPip();
      SetPipBorder(gmvb_PipChannel, TRUE);
      gm_DisplaySetSize(gmvb_PipChannel);
      gm_DisplaySetPosition(gmvb_PipChannel);


      NewInputDim.W_Hstart = stInput[gmvb_PipChannel].Hstart;
      NewInputDim.W_Width = stInput[gmvb_PipChannel].Hactive;
      NewInputDim.W_Vstart = stInput[gmvb_PipChannel].Vstart;
      NewInputDim.W_Height = stInput[gmvb_PipChannel].Vactive;

      RelocateInputCaptureWindow(gmvb_PipChannel, &PrevInputDim, &NewInputDim);
      ModeSetupDisplayChange(gmvb_PipChannel, DISPLAY_CHANGE_NORMAL);
#if(SPEEDUP_MODE_CHANGE == ENABLE)	 
      ModeSetupBlender(gmvb_PipChannel, DISPLAY_DEFER_TMO_NORMAL/SPEEDUP_FACT);
#else
ModeSetupBlender(gmvb_PipChannel, DISPLAY_DEFER_TMO_NORMAL);
#endif
   }
#endif
}
#endif //Action_AdjustAspectRatioPip_WB_Used

//******************************************************************************
//
// FUNCTION       : void AdjustAutoAspectRatio(void)
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
#ifdef Action_AdjustAutoAspectRatio_WB_Used
void AdjustAutoAspectRatio()
{
   //gm_Print("UserPrefAutoAspectRatio: %d", UserPrefAutoAspectRatio);
   return;
}
#endif

#ifdef Action_Zoom_WB_Used

#define   ZOOM_PERCENT   10   // % of Full
#define   MAX_STEPS      6

//******************************************************************************
//
// FUNCTION       : void Zoom(void)
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
void Zoom(void)
{
   WORD W_Dx;
   WORD W_Dy;
   BYTE B_Direction = gm_OsdGetActionByteParameter();
   BOOL Bt_Dirty = FALSE;
   gmt_WindowStruct WinNewDim;
   gmt_WindowStruct WinPrevDim;
   
   WinPrevDim.W_Hstart = stInput[gmvb_MainChannel].Hstart;
   WinPrevDim.W_Width = stInput[gmvb_MainChannel].Hactive;
   WinPrevDim.W_Vstart = stInput[gmvb_MainChannel].Vstart;
   WinPrevDim.W_Height = stInput[gmvb_MainChannel].Vactive;

   W_Dx = (ZOOM_PERCENT * gmvw_InputHActiveOrg[gmvb_MainChannel]) / 200;
   W_Dy = (ZOOM_PERCENT * gmvw_InputVActiveOrg[gmvb_MainChannel]) / 200;

   // Zoom In
   if (B_Direction == UP)
   {
      if ((stInput[gmvb_MainChannel].Hactive - (W_Dx * 2)
            > gmvw_InputHActiveOrg[gmvb_MainChannel] - (W_Dx * MAX_STEPS * 2))
            && (stInput[gmvb_MainChannel].Vactive - (W_Dy * 2)
                > gmvw_InputVActiveOrg[gmvb_MainChannel] - (W_Dy * MAX_STEPS * 2)))
      {
         stInput[gmvb_MainChannel].Hstart += W_Dx;
         stInput[gmvb_MainChannel].Hactive -= (W_Dx * 2);
         stInput[gmvb_MainChannel].Vstart += W_Dy;
         stInput[gmvb_MainChannel].Vactive -= (W_Dy * 2);
         Bt_Dirty = TRUE;
      }
   }

   // Zoom Out
   else
   {
      if ((stInput[gmvb_MainChannel].Hactive + (W_Dx * 2) <= gmvw_InputHActiveOrg[gmvb_MainChannel])
            && (stInput[gmvb_MainChannel].Vactive + (W_Dy * 2) <= gmvw_InputVActiveOrg[gmvb_MainChannel]))
      {
         stInput[gmvb_MainChannel].Hstart -= W_Dx;
         stInput[gmvb_MainChannel].Hactive += (W_Dx * 2);
         stInput[gmvb_MainChannel].Vstart -= W_Dy;
         stInput[gmvb_MainChannel].Vactive += (W_Dy * 2);
         Bt_Dirty = TRUE;
      }
   }

   // Reconfig Chip
   if (Bt_Dirty == TRUE)
   {   
      gm_ClearRegBitsWord(PB_CTRL, PB_MAIN_ENABLE);
   	gm_SetRegBitsWord(PB_CTRL, PB_FORCE_MAIN_BKGND);
   	gm_ForceUpdate(CH_A , INPUT_OUTPUT_CLOCK);
      
      //save the desired input dimension
      WinNewDim.W_Hstart = stInput[gmvb_MainChannel].Hstart;
      WinNewDim.W_Width = stInput[gmvb_MainChannel].Hactive;
      WinNewDim.W_Vstart = stInput[gmvb_MainChannel].Vstart;
      WinNewDim.W_Height = stInput[gmvb_MainChannel].Vactive;

      // 20090618 #1, For DP over Scan.
	   #if ((FEATURE_HDMI == ENABLE) || (FEATURE_DISPLAY_PORT == ENABLE))
      if ((gm_IsInputHdmiDvi(gmvb_MainChannel)) ||(gm_IsInputDP(gmvb_MainChannel)))
      {
			stInput[gmvb_MainChannel].ModeFlags |= gmd_MODE_CAPTURE_CROP;
         
			if ((WinNewDim.W_Width ==  gmvw_InputHActiveOrg[gmvb_MainChannel])&&(WinNewDim.W_Height== gmvw_InputVActiveOrg[gmvb_MainChannel]))
			{
				stInput[gmvb_MainChannel].Hstart = gmvw_InputHStartOrg[gmvb_MainChannel];
            stInput[gmvb_MainChannel].Vstart = gmvw_InputVStartOrg[gmvb_MainChannel];
            stInput[gmvb_MainChannel].ModeFlags &= ~gmd_MODE_CAPTURE_CROP;
         }
      }
	   #endif
   
	   //following line is already done by enabling VIDEO_DYN_SCAL
	   //gm_VideoProcessingDisable(VIDEO_DCDI_MAIN);
	   {  
        DWORD V_StartAddr = gm_ReadRegDWord_BE(MFRC_WBUF_START_hi);

		//gm_Print("V_StartAddr Hi 0x%x", (V_StartAddr >> 16));
         //following line is already done by enabling VIDEO_DYN_SCAL
         //gm_VideoProcessingDisable(VIDEO_DCDI_MAIN);
         gm_MemoryAdjust_VFlip(gmvb_MainChannel, V_StartAddr, WinNewDim.W_Width, WinNewDim.W_Height);
		 
      }
   
	   Freeze(gmvb_MainChannel, FALSE);
	   UserPrefFreeze = 0;                  // System is unfrozen so WB must be notified.

      //Disable ACC3 before zooming to avoid picture flashing
		#if (FEATURE_ACC3 == ENABLE)
      {
         BOOL Bt_Acc3En;
         Bt_Acc3En = gm_ACC3_IsProcessingEnabled(TRUE);
         gm_ACC3StopISR();
		#endif
         RelocateInputCaptureWindow(gmvb_MainChannel, &WinPrevDim, &WinNewDim);

         gm_SetRegBitsWord(PB_CTRL, PB_MAIN_ENABLE);
   	   gm_ClearRegBitsWord(PB_CTRL, PB_FORCE_MAIN_BKGND);
   	   gm_ForceUpdate(CH_A , INPUT_OUTPUT_CLOCK);
		#if (FEATURE_ACC3 == ENABLE)
         if (Bt_Acc3En)
            gm_ACC3StartISR();
      }
		#endif
   }
}
#endif


//******************************************************************************
//
// FUNCTION       : BOOL IsZoom(void)
//
// USAGE          : Returns if picture is zoomed in
//
// INPUT          : None
//
// OUTPUT         : True or False
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
BOOL IsZoom(void)
{
   gmt_PHY_CH B_Channel = gmvb_MainChannel;
   if ((gm_IsGraphics(B_Channel)) && ((gmvw_InputHActiveOrg[B_Channel] != stInput[B_Channel].Hactive) || (gmvw_InputVActiveOrg[B_Channel] != stInput[B_Channel].Vactive)))
      return TRUE;
   return FALSE;
}




//******************************************************************************
//
// FUNCTION       : void Pan(void)
//
// USAGE          : Relocate input capture window
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :   gmvw_InputHStart[]   (R)
//               gmvw_InputHActive[]   (R)
//               gmvw_InputVStart[]   (R)
//               gmvw_InputVActive[]   (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void Pan(void)
{
   gmt_WindowStruct PrevInputDim;
   gmt_WindowStruct NewInputDim;

   PrevInputDim.W_Hstart = stInput[gmvb_MainChannel].Hstart;
   PrevInputDim.W_Width = stInput[gmvb_MainChannel].Hactive;
   PrevInputDim.W_Vstart = stInput[gmvb_MainChannel].Vstart;
   PrevInputDim.W_Height = stInput[gmvb_MainChannel].Vactive;

   // Please enter new locations
   // Note: must maintain input HActive and VActive if you are not changing memory size
   //      HStart must shift by multiples of 2; otherwise, for YUV inputs colors, will swap
   //NewInputDim.W_Hstart = gmvw_InputHStart[gmvb_MainChannel];
   //NewInputDim.W_Width = gmvw_InputHActive[gmvb_MainChannel];
   //NewInputDim.W_Vstart = gmvw_InputVStart[gmvb_MainChannel];
   //NewInputDim.W_Height = gmvw_InputVActive[gmvb_MainChannel];

   RelocateInputCaptureWindow(gmvb_MainChannel, &PrevInputDim,
                              &NewInputDim);
}

//******************************************************************************
//
// FUNCTION       : void RelocateInputCaptureWindow(gmt_PHY_CH B_Channel,
//                  gmt_WindowStruct* pPrevInputDim, gmt_WindowStruct* pNewInputDim)
//
// USAGE          :   Relocate input capture window. This is a wrapper function for
//               PerformRelocateInputCaptureWindow().
//
// INPUT          :   B_Channel - CH_A or CH_B
//               pPrevInputDim -   pointer to a struct that stores the current input
//                           window attributes
//               pNewInputDim -   pointer to a struct that stores the desired input
//                           window attributes
//
// OUTPUT         : None
//
// GLOBALS        :   gmvw_InputHStart[]   (R/W)
//               gmvw_InputHActive[]   (R/W)
//               gmvw_InputVStart[]   (R/W)
//               gmvw_InputVActive[]   (R/W)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void RelocateInputCaptureWindow(gmt_PHY_CH B_Channel,
                                gmt_WindowStruct* pPrevInputDim, gmt_WindowStruct* pNewInputDim)
{
   WORD W_TNR_LUT_UV;
   BYTE B_NumOfIterations = 0;

   //Need to track whether the new VStart is larger than the current VStart because
   //  IMP_FRAME_RESET_LINE (read/write) is set to 2 lines before VStart (pending active).
   //  Upon calling gm_InputSetVPosition(), even before the new VStart becomes in effect
   //  (through dynamic scaling), the value of IMP_FRAME_RESET_LINE has already become
   //  effective and this causes temporary image corruption. Delaying programming
   //  IMP_FRAME_RESET_LINE seems to fix the issue.
   BOOL Bt_IsNewVStartLarger = (pNewInputDim->W_Vstart > pPrevInputDim->W_Vstart);

   //return if new input capture window is the same as the current one
   if ((pNewInputDim->W_Hstart == pPrevInputDim->W_Hstart)
         && (pNewInputDim->W_Width == pPrevInputDim->W_Width)
         && (pNewInputDim->W_Vstart == pPrevInputDim->W_Vstart)
         && (pNewInputDim->W_Height == pPrevInputDim->W_Height))
      return;

   stInput[B_Channel].Hstart = pNewInputDim->W_Hstart;
   stInput[B_Channel].Hactive = pNewInputDim->W_Width;
   stInput[B_Channel].Vstart = pNewInputDim->W_Vstart;
   stInput[B_Channel].Vactive = pNewInputDim->W_Height;

   // To prevent pink bar on white color 16 gray bar (color ghosting)
   // when do zoom in-out for Progressive input with Progressive TNR on
   W_TNR_LUT_UV = gm_ReadRegWord( TNR_LUT_UV );
   gm_WriteRegWord(TNR_LUT_UV, 0);

   DynamicScalerStart(B_Channel);

   //inserted bypass step which modified input dimension
   while (IsDynamicScalerBypassStepInserted(B_Channel, FALSE, pPrevInputDim))
   {
      //if HStart difference is odd, make it even so that no colour swap is observed
      if ((stInput[B_Channel].Hstart ^ pNewInputDim->W_Hstart) & BIT0)
         stInput[B_Channel].Hstart += 1;

      PerformRelocateInputCaptureWindow(B_Channel, Bt_IsNewVStartLarger);
      //if(gm_IsModeComplete(B_Channel))
	//gm_ScalerDynamicWait(NUM_VSYNCS);

      if (((gm_GetScalerHMode(B_Channel) == SCALER_BYPASS) && (gmvw_OutputHActive[B_Channel] > pNewInputDim->W_Width))
            ||((gm_GetScalerVMode(B_Channel) == SCALER_BYPASS) && (gmvw_OutputVActive[B_Channel] > pNewInputDim->W_Height)))
      {
         gm_ScalerDynamicEnable(B_Channel, FALSE);
         gm_ScalerSetup(B_Channel);
	 //if(gm_IsModeComplete(B_Channel))
         	//gm_SyncUpdateWait(B_Channel, OUTPUT_CLOCK);
         gm_ScalerDynamicEnable(B_Channel, TRUE);
      }
      //update previous input dimension with current dimension
      pPrevInputDim->W_Hstart = stInput[B_Channel].Hstart;
      pPrevInputDim->W_Width = stInput[B_Channel].Hactive;
      pPrevInputDim->W_Vstart = stInput[B_Channel].Vstart;
      pPrevInputDim->W_Height = stInput[B_Channel].Vactive;

      //restore the desired dimension
      stInput[B_Channel].Hstart = pNewInputDim->W_Hstart;
      stInput[B_Channel].Hactive = pNewInputDim->W_Width;
      stInput[B_Channel].Vstart = pNewInputDim->W_Vstart;
      stInput[B_Channel].Vactive = pNewInputDim->W_Height;

      //this is to prevent infinite loop; set to 2 because at most horizontally and vertically each inserted one step
      B_NumOfIterations++;
      if (B_NumOfIterations > 2)
         break;
   }
   PerformRelocateInputCaptureWindow(B_Channel, Bt_IsNewVStartLarger);
   if (Bt_IsNewVStartLarger)
   {
	  if(gm_IsModeComplete(B_Channel))
      gm_WaitForInputBlanking(B_Channel);
      gm_InputSetVPosition(B_Channel);
   }
   DynamicScalerStop();
   gm_WriteRegWord(TNR_LUT_UV, W_TNR_LUT_UV);
   gm_SyncUpdateWait(B_Channel, INPUT_OUTPUT_CLOCK);
}

//******************************************************************************
//
// FUNCTION       : static void PerformRelocateInputCaptureWindow(gmt_PHY_CH B_Channel,
//                                             BOOL Bt_IsNewVStartLarger)
//
// USAGE          : Relocate input capture window
//
// INPUT          : B_Channel - CH_A or CH_B
//               Bt_IsNewVStartLarger - TRUE or FALSE
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  :   DynamicScalerStart() should have been called
//
// POST_CONDITION : DynamicScalerStop() should be called
//
//******************************************************************************
static void PerformRelocateInputCaptureWindow(gmt_PHY_CH B_Channel, BOOL Bt_IsNewVStartLarger)
{
   WORD W_Temp1, W_Temp2;

   //Save original values of R/W registers
   if (Bt_IsNewVStartLarger)
   {
      W_Temp1 = gm_ReadRegWord((B_Channel == CH_A)?IMP_FRAME_RESET_LINE:IPP_FRAME_RESET_LINE);
      if (B_Channel == CH_A)
         W_Temp2 = gm_ReadRegWord(IMP_RFF_READY_LINE);
   }

   if ((gm_IsInputTripleADC(B_Channel)) && (IsGraphics(B_Channel) == TRUE))
   {
      //the following 4 lines are mainly for FLI8532, but work as well on other chips
	if(gm_IsModeComplete(B_Channel))
      	gm_WaitForInputBlanking(B_Channel);
      gm_InputSetHPosition(B_Channel);
      gm_InputSetVPosition(B_Channel);
      //Restore original values of R/W registers; if they are not restored, R/W registers and VStart can mismatch
      if (Bt_IsNewVStartLarger)
      {
         gm_WriteRegWord((B_Channel == CH_A)?IMP_FRAME_RESET_LINE:IPP_FRAME_RESET_LINE, W_Temp1);
         if (B_Channel == CH_A)
            gm_WriteRegWord(IMP_RFF_READY_LINE, W_Temp2);
      }
      gm_ForceUpdate(B_Channel, INPUT_CLOCK);

      if ((B_Channel == CH_A))
         gm_SetupInputMain();
   #ifndef NO_PIP_SUPPORT
      else
         gm_SetupInputPip();
   #endif
   }
   else
   {
      gm_InputSetCaptureWindow(B_Channel);
   }
   if (B_Channel == CH_A)
   {
      gm_AfmSetup(W_OverscanEnabled[CH_A]);
      gm_SetLumaMeasureWindow();
   }

   //Restore original values of R/W registers; if they are not restored, R/W registers and VStart can mismatch
   if (Bt_IsNewVStartLarger)
   {
      gm_WriteRegWord((B_Channel == CH_A)?IMP_FRAME_RESET_LINE:IPP_FRAME_RESET_LINE, W_Temp1);
      if (B_Channel == CH_A)
         gm_WriteRegWord(IMP_RFF_READY_LINE, W_Temp2);
   }
   gm_ScalerDynamic((B_Channel == CH_A)?BITMASK_CHANNEL_A:BITMASK_CHANNEL_B);
}


//******************************************************************************
//
// FUNCTION       : BOOL IsPanoramicAvailable (void)
//
// USAGE          : Indicates if panoramic adjustment is possible or not
//
// INPUT          : None
//
// OUTPUT         : TRUE or FALSE
//
// GLOBALS        : gmvw_InputModeAttr (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BOOL IsPanoramicAvailable (void)
{
   gmt_MODE_FLAGS   W_Input = stInput[gmvb_MainChannel].ModeFlags & gmd_MODE_SIGNAL_MASK;

   //
   // Panoramic cannot be applied if the input is graphics or if in side-by-side mode
   //
   if ((IsSideBySidePipMode(UserPrefPipMode) == FALSE) && (W_Input != gmd_MODE_GRAPHICS))
   {
      if ( ((W_Input == gmd_MODE_SDVIDEO) || (W_Input == gmd_MODE_EDVIDEO))
            && ((B_OutputAspect == gmd_ASPECT_16x9) || (B_OutputAspect == gmd_ASPECT_15x9)) )
         return TRUE;

      if ( (W_Input == gmd_MODE_HDVIDEO) &&
            ((B_OutputAspect == gmd_ASPECT_4x3) || (B_OutputAspect == gmd_ASPECT_5x4)) )
         //return TRUE;
         return FALSE;   // Return FALSE, since no tables available for HD inputs

      else
         return FALSE;
   }
   else
   {
      return FALSE;      // Cannot perform panoramic scaling in SBS mode
   }
}

//******************************************************************************
//
// FUNCTION       : gmt_ASPECT_RATIO GetDisplayAspectRatio(void)
//
// USAGE          : Returns what current panel aspect ratio is (16x9 or 4:3)
//
// INPUT          : None
//
// OUTPUT         : gmt_ASPECT_RATIO
//
// GLOBALS        : B_OutputAspect      (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
gmt_ASPECT_RATIO GetDisplayAspectRatio(void)
{
   if (B_OutputAspect == gmd_ASPECT_16x9 || B_OutputAspect == gmd_ASPECT_15x9)
   {
      return gmd_ASPECT_16x9;
   }
   else
   {
      return gmd_ASPECT_4x3;
   }
}

//******************************************************************************
//
// FUNCTION       : ASPECT_RATIOS_MODE GetNextMainAspectRatio(ASPECT_RATIOS_MODE W_CurrentAspect)
//
// USAGE          : Returns the next aspect for the main channel when user
//                  try to change aspect ratio setting
//
// INPUT          : W_CurrentAspect - Currect OSD selection
//
// OUTPUT         : Next valid aspect ratio for Main
//
// GLOBALS        : gmvb_MainChannel   (R)
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
ASPECT_RATIOS_MODE GetNextMainAspectRatio(ASPECT_RATIOS_MODE W_CurrentAspect)
{
   return (GetNextAspectRatio(gmvb_MainChannel, W_CurrentAspect));
}

//******************************************************************************
//
// FUNCTION       : ASPECT_RATIOS_MODE GetNextPipAspectRatio(ASPECT_RATIOS_MODE W_CurrentAspect)
//
// USAGE          : Returns the next aspect for the pip channel when user
//                  try to change aspect ratio setting
//
// INPUT          : W_CurrentAspect - Currect OSD selection
//
// OUTPUT         : Next valid aspect ratio for PIP
//
// GLOBALS        : gmvb_PipChannel      (R)
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
ASPECT_RATIOS_MODE GetNextPipAspectRatio(ASPECT_RATIOS_MODE W_CurrentAspect)
{
   return (GetNextAspectRatio(gmvb_PipChannel, W_CurrentAspect));
}

//******************************************************************************
//
// FUNCTION       : ASPECT_RATIOS_MODE GetNextAspectRatio(gmt_PHY_CH B_Channel,
//                                               ASPECT_RATIOS_MODE W_CurrentAspect)
//
// USAGE          : Returns the next aspect for the channel when user
//                  try to change aspect ratio setting
//
// INPUT          : B_Channel - gmvb_MainChannel or gmvb_PipChannel
//                  W_CurrentAspect - Currect OSD selection
//
// OUTPUT         : Next valid aspect ratio for B_Channel
//
// GLOBALS        : gmvb_MainChannel   (R)
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
ASPECT_RATIOS_MODE GetNextAspectRatio(gmt_PHY_CH B_Channel,
                                      ASPECT_RATIOS_MODE W_CurrentAspect)
{
   ASPECT_RATIOS_MODE W_NextAspect;
   gmt_MODE_FLAGS W_Input;

   W_Input = stInput[B_Channel].ModeFlags & gmd_MODE_SIGNAL_MASK;

   //
   // Base on display aspect ratio to determine what is allow
   // for next selection
   //
   if (GetDisplayAspectRatio() == gmd_ASPECT_16x9)
   {                                 // 16:9 display screen
	  //HYB130126* 
      switch (W_CurrentAspect)
      {
         case AR_FULL_SCREEN:
            W_NextAspect = AR_LETTER_BOX;
            break;
         case AR_LETTER_BOX:
            W_NextAspect = AR_FULL_SCREEN;  //AR_PILLAR_BOX;  wei 140529
            break;

         case AR_PILLAR_BOX:
		#if 0 //HYB130126* 
            //
            // Graphic mode do not allow input crop mode
            //
            if (IsGraphics(B_Channel) == TRUE)
               W_NextAspect = AR_FULL_SCREEN;
            else
               W_NextAspect = AR_LETTER_BOX_EXPAND;
		#else
            W_NextAspect = AR_PILLAR_BOX_EXPAND;

		#endif
            break;
         case AR_PILLAR_BOX_EXPAND:
            W_NextAspect = AR_LETTER_BOX_EXPAND; 
            break;         
	  case AR_LETTER_BOX_EXPAND:
		W_Input = W_Input;
		#if 0 //HYB130126* 
            //
            // If MAIN channel, SD or ED video mode and
            // not side-by-side mode
            //
            if ((B_Channel == gmvb_MainChannel) &&
                  (IsSideBySidePipMode(UserPrefPipMode) == FALSE) &&
                  ((W_Input==gmd_MODE_SDVIDEO)||(W_Input==gmd_MODE_EDVIDEO)))
            {
               W_NextAspect = AR_PANORAMIC;   // panoramic allow
            }
            else
	#endif
               W_NextAspect = AR_FULL_SCREEN;   // otherwise fullscreen
            break;

         case AR_PANORAMIC:
            W_NextAspect = AR_FULL_SCREEN;
            break;

         default:
            W_NextAspect = W_CurrentAspect;
            break;
      }
   }
   else                               // 4:3 display screen
   {
      switch (W_CurrentAspect)
      {
         case AR_FULL_SCREEN:
            W_NextAspect = AR_LETTER_BOX;
            break;

         case AR_LETTER_BOX:
            //
            // Graphic mode do not allow input crop mode
            //
            if (IsGraphics(B_Channel) == TRUE)
               W_NextAspect = AR_FULL_SCREEN;
            else
               W_NextAspect = AR_PILLAR_BOX_EXPAND;
            break;

         case AR_PILLAR_BOX_EXPAND:
            W_NextAspect = AR_FULL_SCREEN;
            break;

//         case AR_WATER_GLASS:            // No implemented
//            break;

         default:
            W_NextAspect = W_CurrentAspect;
            break;
      }
   }
   return (W_NextAspect);
}

//******************************************************************************
//
// FUNCTION       : ASPECT_RATIOS_MODE GetPrevMainAspectRatio(ASPECT_RATIOS_MODE W_CurrentAspect)
//
// USAGE          : Returns the previous aspect for the main channel when user
//                  try to change aspect ratio setting
//
// INPUT          : W_CurrentAspect - Currect OSD selection
//
// OUTPUT         : Previous valid aspect ratio for Main
//
// GLOBALS        : gmvb_MainChannel      (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
ASPECT_RATIOS_MODE GetPrevMainAspectRatio(ASPECT_RATIOS_MODE W_CurrentAspect)
{
   return (GetPrevAspectRatio(gmvb_MainChannel, W_CurrentAspect));
}

//******************************************************************************
//
// FUNCTION       : ASPECT_RATIOS_MODE GetPrevPipAspectRatio(ASPECT_RATIOS_MODE W_CurrentAspect)
//
// USAGE          : Returns the previous aspect for the pip channel when user
//                  try to change aspect ratio setting
//
// INPUT          : W_CurrentAspect - Currect OSD selection
//
// OUTPUT         : Previous valid aspect ratio for PIP
//
// GLOBALS        : gmvb_PipChannel      (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
ASPECT_RATIOS_MODE GetPrevPipAspectRatio(ASPECT_RATIOS_MODE W_CurrentAspect)
{
   return (GetPrevAspectRatio(gmvb_PipChannel, W_CurrentAspect));
}

//******************************************************************************
//
// FUNCTION       : ASPECT_RATIOS_MODE GetPrevAspectRatio(gmt_PHY_CH B_Channel,
//                                               ASPECT_RATIOS_MODE W_CurrentAspect)
//
// USAGE          : Returns the previous aspect ratio for the channel
//
// INPUT          : B_Channel - gmvb_MainChannel or gmvb_PipChannel
//                  W_CurrentAspect - Currect OSD selection
//
// OUTPUT         : Previous valid aspect ratio for B_Channel
//
// GLOBALS        : gmvb_MainChannel   (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
ASPECT_RATIOS_MODE GetPrevAspectRatio(gmt_PHY_CH B_Channel,
                                      ASPECT_RATIOS_MODE W_CurrentAspect)
{
   ASPECT_RATIOS_MODE W_PrevAspect;

   //
   // Based on current aspect ratio, determine previous allowed aspect ratio
   //
   if (GetDisplayAspectRatio() == gmd_ASPECT_16x9)
   {                                 // 16:9 display screen
      switch (W_CurrentAspect)
      {
         case AR_FULL_SCREEN:
		#if 1		//HYB130126*
                  W_PrevAspect = AR_LETTER_BOX;  //  wei 140529  AR_LETTER_BOX_EXPAND;   // otherwise skip it
		#else
            //
            // If channel is in graphics mode
            //
            if (IsGraphics(B_Channel))
            {
               W_PrevAspect = AR_PILLAR_BOX;   // skip letterbox expanded and panoramic
            }
            else
            {
               //
               // If MAIN channel, SD or ED video mode and not side-by-side mode
               //
               if ((B_Channel == gmvb_MainChannel) &&
                     (!IsSideBySidePipMode(UserPrefPipMode)) &&
                     (gm_IsSDVideo(B_Channel) || gm_IsEDVideo(B_Channel)))
               {
                  W_PrevAspect = AR_PANORAMIC;   // panoramic allowed
               }
               else
               {
                  W_PrevAspect = AR_LETTER_BOX_EXPAND;   // otherwise skip it
               }
            }
		#endif
            break;
         case AR_LETTER_BOX:
            W_PrevAspect = AR_FULL_SCREEN;
            break;
         case AR_PILLAR_BOX:
            W_PrevAspect = AR_LETTER_BOX;
            break;
         case AR_PILLAR_BOX_EXPAND:
            W_PrevAspect = AR_PILLAR_BOX;  
            break;
         case AR_LETTER_BOX_EXPAND:
            W_PrevAspect = AR_PILLAR_BOX_EXPAND;
            break;

         case AR_PANORAMIC:
            W_PrevAspect = AR_LETTER_BOX_EXPAND;
            break;

         default:
            W_PrevAspect = W_CurrentAspect;
            break;
      }
   }
   else                               // 4:3 display screen
   {
      switch (W_CurrentAspect)
      {
         case AR_FULL_SCREEN:
            //
            // If channel is in graphics mode
            //
            if (IsGraphics(B_Channel))
            {
               W_PrevAspect = AR_LETTER_BOX;   // skip pillarbox
            }
            else
            {
               W_PrevAspect = AR_PILLAR_BOX_EXPAND;   //pillarbox allowed
            }
            break;

         case AR_LETTER_BOX:
            W_PrevAspect = AR_FULL_SCREEN;
            break;

         case AR_PILLAR_BOX_EXPAND:
            W_PrevAspect = AR_LETTER_BOX;
            break;

//         case AR_WATER_GLASS:      // not implemented
//            break;

         default:
            W_PrevAspect = W_CurrentAspect;
            break;
      }
   }
   return (W_PrevAspect);
}

/*DWORD MaxDisplayNumber(void)
{
	return (UserPrefXNumber*UserPrefYNumber);
} */

//*********************************  END  **************************************
