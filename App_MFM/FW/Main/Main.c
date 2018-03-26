/*
	$Workfile:   Main.c  $
	$Revision: 1.87 $
	$Date: 2013/04/02 02:13:44 $
*/
#define __EXT_MAIN_C__
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
// MODULE:      main.c
//
// USAGE:       This module contains main() function and main Loop related
//              functions.
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "System\All.h"

#if FEATURE_ACR == ENABLE
#include "system\ACR_Handler.h"
#endif

extern gmt_PwrStateType PwrState;

//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************
void far gm_MainLoop (void);

//******************************************************************************
//  E X T E R N A L     F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

extern void DebugHandler(void);

#if (FEATURE_CEC == ENABLE)
extern void CecHandler(void);
#endif

#ifdef USE_SMT
extern void SMT_MasterHandler(void);
#endif

#if (FEATURE_DP_HUB == ENABLE)
extern BOOL IsDpTxReady(void);
extern void DpRxLoadEdid2Buf(void);
#endif

#ifdef ST_4K2K_93xx_BOARD
extern void ChangeModeHandler(void);
#endif

#if (FEATURE_SOCIPC == ENABLE)
BOOL IsInitialFinished = FALSE;
#endif

#ifdef DELL_U2410F
extern void CustomHandler(void);
#endif

//******************************************************************************
//  S T A T I C    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

//******************************************************************************
//  C O D E
//******************************************************************************

#if defined(ST_4K2K_93xx_BOARD) && (DEBUG_DISPLAY_TIME_MEASUREMENT==ENABLE)
static BYTE b_ImgOnOff=OFF;
static DWORD dw_ImgOnOffTime;
void SetImgOnOffTime(BYTE b_OnOff, DWORD dw_Time)
{
   b_ImgOnOff=b_OnOff;
   dw_ImgOnOffTime=dw_Time;
}

static void PrintImgOnOffTime(void)
{
   static BYTE pre_ImgOnOff=OFF;
   if(pre_ImgOnOff!=b_ImgOnOff)
   {
      pre_ImgOnOff=b_ImgOnOff;
      if(b_ImgOnOff==OFF)
         gm_Print("DTM> IMG_OFF_T(%Ld)",dw_ImgOnOffTime);
      else
         gm_Print("DTM> IMG_ON_T(%Ld)",dw_ImgOnOffTime);
   }
}
#endif
//******************************************************************************
//
// FUNCTION       : void far main(void)
//
// USAGE          : Main loop that perform system and register initialization.
//                  Call each individual task handler in sequential order.
//
// INPUT          :
//
// OUTPUT         : None
//
// GLOBALS        : gmvb_DebugOnly		(R)
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void far main(void)
{
#if (FEATURE_SOCIPC == ENABLE)
	IsInitialFinished = FALSE;
#endif
   //
   // Initialize application level
   //
   ApplicationInit();
#if (FEATURE_SOCIPC == ENABLE)
	IsInitialFinished = TRUE;
#endif
   //
   // Indicate application start
   // Print version # and system information
   //
   PrintVersion();

#if (FEATURE_DP_HUB == ENABLE)
   UserPrefCurrentInputMain = 10;
   UserPrefCurrentInputPip  = 0;
   AdjustCurrentInputMain();

   if(IsDpTxReady())
   {
      DpRxLoadEdid2Buf(); // Temporary retry for load EDID from VegaTx
   }
#endif

#ifdef DISABLE_PRINT_MESSAGE
   gm_EnablePrints(FALSE);
#endif

   //
   // Application MAIN loop
   //
   while (TRUE)
   {
      gm_MainLoop ();
   }

}

//******************************************************************************
//
// FUNCTION       : void far gm_MainLoop(void)
//
// USAGE          : Main loop that perform system and register initialization.
//                  Call each individual task handler in sequential order.
//
//                  In case of RTEX usage, the main loop is organized as a low
//					priority task that always schedules itself to run again.
//					All other tasks should have ahigher priority and will be
//					scheduled by the timer interrupt at the desired time
//					intervals
//
// INPUT          :
//
// OUTPUT         : None
//
// GLOBALS        : gmvb_DebugOnly		(R)
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
#ifdef VIDEO_CAPTURE
extern void Capture_Video_Handler(void);
#endif

#ifdef USE_COLOR_FIDELITY
extern BYTE commonGamutDataBuf[];
extern BYTE far DoGamutMapping(BYTE* gamutDataBuf, BYTE mode);
#endif

#ifndef NO_PIP_SUPPORT
extern void PIP_Demo_Handler(void);
#endif

void far gm_MainLoop (void)
{

#if 0 // For test only
   static DWORD pre_Time;
   if(gm_ReadRegByte(IMP_PATGEN_BLUE)==1)
   {
      DWORD newTime=gm_GetSystemTime();
      pre_Time=newTime-pre_Time;
      if(pre_Time>gm_ReadRegByte(IMP_PATGEN_GRN))
         gm_Print("T(%d)",(WORD)pre_Time);
      
      pre_Time=newTime;
   }
#endif
   // Watchdog timer service: Main loop should not take more than WDT_EXP_TIME * 1 WDT unit
#ifdef WDT_SERVICE_ENABLE
   gm_WDT_Reset(WDT_EXP_TIME);
#endif

   // Process application level gProbe command from Serial and DDC2Bi
#ifdef USE_SMT
   gmvb_SerialStatus = SerialCmdHandler();
   /* SMT Master Handler for fowarding message stream */
   SMT_MasterHandler();
#else
   SerialCmdHandler();
#endif
   Ddc2biCmdHandler();
#if (FEATURE_DISPLAY_PORT == ENABLE) && defined(DDCCi_PROTOCOL_SUPPORT) 
   DpRxAuxHandler();
#endif

   if (!gmvb_DebugOnly)
      gm_HardwareUpdate();  // Update HW driver values

   // 20100802, Cable Status handler
   if (!gmvb_DebugOnly)
      CableStatusHandler();  // Update HW driver values

#if (FEATURE_3D == ENABLE)
   if (!gmvb_DebugOnly)
      gm_3D_Handler();
#endif

   if (!gmvb_DebugOnly)
      ModeHandler(); // Perform Mode detection and mode determination

/*ChengLin 20130221: For 4k2k platform Athena Right/Left Image Sync State Handler*/
#ifdef ST_4K2K_93xx_BOARD 
#if ENABLE_ATHENA_R_L_IMAGE_SYNC_HANDLER
	if (!gmvb_DebugOnly && (PwrState != Pwr_DeepSleep))
	  ImageSyncHandler();
#endif
#endif
	  
   if (!gmvb_DebugOnly)
      PowerHandler(); // Monitor signal condition and manage power modes

   if (!gmvb_DebugOnly && (PwrState != Pwr_DeepSleep))
      FilmModeHdlr(); // Monitor changes in Film Processing

   if (!gmvb_DebugOnly)
      ModeSetupHdlr(); // Check for State changes and Setup system for New Modes when detected

#if (FEATURE_HDMI==ENABLE)
   if (!gmvb_DebugOnly ) //&& (PwrState != Pwr_DeepSleep))
   {
   #ifdef ST_4K2K_93xx_BOARD
      if(GetChipID_4K2K()==CHIPID_4K2K_FE)
         HdmiDviHandler(); // HDMI state machine and monitoring function
   #else
         HdmiDviHandler(); // HDMI state machine and monitoring function
   #endif
   }
#endif

#if (FEATURE_CEC == ENABLE)
   if (!gmvb_DebugOnly && (PwrState != Pwr_DeepSleep))
   {
      CecHandler();
      RegMsgHandler();
   }
#endif

#if (FEATURE_DISPLAY_PORT == ENABLE)
   if (!gmvb_DebugOnly )   //&& (PwrState != Pwr_DeepSleep))//in "Deep-Sleep" mode clocks are too low.
   {
      DpAppHandler();
      #if (FEATURE_HDCP_REPEATER == ENABLE) && (!defined(ST_4K2K_93xx_BOARD))
         RunHdcpRepeater(HdcpDpRepeater);
      #endif
   }
#endif


#if (defined(SMART_ISP) && defined(SMART_ISP_AUTO_DETECTION))
   if (!gmvb_DebugOnly && (PwrState != Pwr_DeepSleep))
      SmartIspDetectHandler();
#endif


#if (defined(SMART_ISP) && defined(ST_4K2K_93xx_BOARD))
   if(GetChipID_4K2K()!=CHIPID_4K2K_R)
      SmartIspSlaveHandler();
#endif

#if 0//(FEATURE_SOCIPC == ENABLE)
#ifdef Athena_4K2K_Chip_R
	if (!gmvb_DebugOnly)
	{
	SocIPCSendKeypadEvent();
	SocIPCSendIREvent();
	}
#endif
#endif

#ifndef DISABLE_OSD_HANDLER
   if (!gmvb_DebugOnly)
      OsdEventHandler(); // OSD Action -> response to Keys and ModeHSetupHdlr
#endif

#ifdef AUTO_SCAN_INPUT
   if (!gmvb_DebugOnly)
      AutoScanInputHandler();
#endif

   if (!gmvb_DebugOnly && (PwrState != Pwr_DeepSleep))
      BlinkingHandler();

#ifdef DELL_U2410F // Mikhail for DELL OSD
	if (!gmvb_DebugOnly)	
		CustomHandler();
#endif

   #ifdef UART_DEBUG_MODE
   if (PwrState != Pwr_DeepSleep)
      DebugHandler(); // Debug handler for FW development only
	#endif
	
#ifdef SQA_AUTOMATION_DEBUGMODE
   if (PwrState != Pwr_DeepSleep)
   {   //for SQA to do automation test on Mode Determination
#ifndef SQA_AUTOMATION_PIP_ONLY
      SQA_DebugHandler(gmvb_MainChannel);
#endif
#ifndef SQA_AUTOMATION_MAIN_ONLY
      SQA_DebugHandler(gmvb_PipChannel);
#endif
   }
#endif

#ifdef VIDEO_CAPTURE
   Capture_Video_Handler();
#endif


#ifdef USE_COLOR_FIDELITY
 #if USE_DYNAMIC_GAMUTMAPPING_SUPPORT	// Color Fidelity
   DoGamutMapping(commonGamutDataBuf, 0);
 #endif
#endif

#if (FEATURE_VWD == ENABLE)
   if (!gmvb_DebugOnly && (gm_IsModeComplete(gmvb_MainChannel)))
      VWD_Handler();
#endif

#if (FEATURE_ACR == ENABLE)
   if (!gmvb_DebugOnly && (gm_IsModeComplete(gmvb_MainChannel)))
   	#ifdef DELL_U2410F
   	{
   	  if((B_SplashComplete == TRUE)&&(gm_IsModeComplete(CH_A))&&(UserPrefPipMode == NO_PIP)&&
	  	gm_BlenderIsOn(CH_A))//120903 Edward add this condition to avoid system hang up when DC / AC on at game / movie mode (VWD/DCR on both) 
		DCR_Handler();//20110815 Add by KevinChen.
   	}	
	#else
      ACR_Handler();
#endif
   #endif
   

#if (DDR_TEST_CODE == ENABLE)
   if (!gmvb_DebugOnly)
      DDR_RW_HANDLER();
#endif

#if (FEATURE_AUDIO == ENABLE)
   if (!gmvb_DebugOnly)
      Audio_Handler();
#endif

#ifndef NO_PIP_SUPPORT
   if (!gmvb_DebugOnly && (PwrState != Pwr_DeepSleep))
      PIP_Demo_Handler();
#endif

#ifdef ST_4K2K_93xx_BOARD
   if (!gmvb_DebugOnly)
   {
      if(GetChipID_4K2K()==CHIPID_4K2K_R)
      {
         if(PwrState != Pwr_DeepSleep)
         {
            ChangeModeHandler();
         }
         PanelControl_MasterHandler();
         #if (DEBUG_DISPLAY_TIME_MEASUREMENT==ENABLE)
         PrintImgOnOffTime ();
         #endif
      }
      else if(GetChipID_4K2K()==CHIPID_4K2K_L)
      {
         if(PwrState != Pwr_DeepSleep)
         {
            Image_Hstart_Patch();
         }
         PanelControl_SlaveHandler();
         #if (DEBUG_DISPLAY_TIME_MEASUREMENT==ENABLE)
         PrintImgOnOffTime ();
         #endif
      }
   }
#endif
}

/***********************************  END  ************************************/

