/*
   $Workfile:   ModeSetup.c  $
   $Revision: 1.260 $
   $Date: 2013/06/10 10:04:02 $
*/
#define __MODESETUP_C__

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
// MODULE:      ModeSetup.c
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include <string.h>
#include "System\All.h"
#include "StdLibInc\gm_Register.h"

#if (FEATURE_ACC3 == ENABLE)
#include "StdLibInc\gm_Acc3.h"
#endif

#if (FEATURE_ACR == ENABLE)
#include "system\ACR_Handler.h"
#endif
//******************************************************************************
//  D E F I N E S
//******************************************************************************

#define   NUM_OF_STEPS      1            // Num of transitional steps when changing display clock

//#define   DEBUG_SCHEDULER      CH_A
//#define   DEBUG_SCHEDULER      CH_B
//#define   DEBUG_BLENDER
//#define   DEBUG_REQUEST        REQ_ANY

#ifdef DEBUG_REQUEST
#define debugRequest(string, value)      \
      if ((B_Request==DEBUG_REQUEST) || (DEBUG_REQUEST==REQ_ANY)) \
         gm_Print(string, value)
#else
#define debugRequest(string, value)
#endif

#ifdef DEBUG_BLENDER
#define debugBlender(string, value)      gm_Print(string, value)
#else
#define debugBlender(string, value)
#endif

//---input switch timer---
//   time for input port change to stable picture at output
//   time for new input mode change to stable picture at output
//   time for new channel change to stable picture at output
#ifdef DEBUG_IST
WORD ISTstart[CH_PHY_NUM];
BYTE ISTflag=0;
#endif

#define SCART_RGB_NOT_OVERLAY   2
#define SCART_RGB_YES_OVERLAY 1

#define MAX_MNR_ARRAY_SIZE 11
// 23:18 GAMMA 17:12 THRESH2 11:6 THRESH1 5:0 THRESH0
// Sequoai Default 20 28 F 1C ==> 0x8283DC
const DWORD MSNQR_Ctrl3[MAX_MNR_ARRAY_SIZE] =
{
    0x0,       /*0*/
    0xC81A0A,  /*1*/
    0xE02C12,  /*2*/
    0xE02D1E,  /*3*/
    0xE03E2A,  /*4*/
    0xE03F36,  /*5*/
    0xE04FFF,  /*6*/
    0xE84FFF,  /*7*/
    0xF04FFF,  /*8*/
    0xF84FFF,  /*9*/
    0xFC4FFF   /*10*/
};

VQ_MNR_Parameters_t ROM MNRVQTable = 
{
	{
		sizeof(VQ_MNR_Parameters_t),
		0, // STLAYER_VIDEO_MNR,
		0
	},
	// Filter_Strength
	5
};

//******************************************************************************
//  G L O B A L   V A R I A B L E S
//******************************************************************************

BYTE B_PrevPipMode = 0xff;            // PIP Mode Tracking
BYTE B_ModeSetupInit = FALSE;
BYTE B_ModeSetupStarted = FALSE;

BYTE DP_EQStore = 4;	// Default
BYTE DP_SWINGStore = 0;	// 400mv
BYTE DP_EMPStore = 1;	// 3.5db
BYTE Store_Lock_Mode = 0;
WORD Store_HTotal = 0;
WORD Store_VTotal = 0;
extern WORD Real_HTotal;
extern WORD Real_VTotal;

#ifdef ST_4K2K_93xx_BOARD //Added by ChengLin 20130221
#if ENABLE_ATHENA_R_L_IMAGE_SYNC_HANDLER
BOOL ImageSyncModeRightStable = FALSE;
BOOL ImageSyncModeLeftFeStable = FALSE;
BOOL ImageSyncINT1_Flag = FALSE;
#define IMAGE_SYNC_HANDLER_WAIT_TIME_OUT 600 //6 seconds

#if ENABLE_DP_MST_TIMEOUT_CHECK_FOR_OWN_READY_BIT
BOOL ImageSyncDP_MST_TimeOutCheck = FALSE;
BOOL ImageSync_ForceCheckReadySetting = FALSE;
#define IMAGE_SYNC_DP_MST_WAIT_TIME_OUT 200 // 2seconds
#endif //ENABLE_DP_MST_TIMEOUT_CHECK_FOR_OWN_READY_BIT

#endif
#if ENABLE_PBP_ATHENA_L_DEEP_SLEEP_CHECK               
BOOL AthenaRIgnoreSleep_PBP = FALSE;
BOOL PBP_AthenaR_ReCheck_AthenaLStatus = FALSE;
extern BOOL PBP_StartPowerCheck_AthenaR;
#endif

#if ENABLE_TWO_LOGICAL_PORTS_SLEEP_CHECK  
extern BOOL DP_TWO_LogicalPorts_StartPowerCheck_AthenaR;
#endif

#endif

//******************************************************************************
//  E X T E R N S
//******************************************************************************

extern gmt_RET_STAT   gm_ScalerDynamic(BYTE B_ChanAB);
extern void         gm_ScalerDynamicEnable(gmt_PHY_CH B_Channel, BYTE B_Enable);
extern void         TeletextStart(void);
extern void         TeletextStop(void);
extern BOOL          ParentalControlAvailable (gmt_PHY_CH B_Channel);
extern far WORD      gW_DetectionMode;

#if (FEATURE_CEC == ENABLE)
extern WORD UserExecutionTimeoutMain;
extern WORD UserExecutionTimeoutPip;
extern BYTE B_UserCommandExecution;
#endif

#ifdef ST_4K2K_93xx_BOARD
#if ENABLE_ATHENA_R_L_IMAGE_SYNC_HANDLER
extern gmt_ImageSyncState gCurrentAthenaRIGHT_SyncState;
extern gmt_ImageSyncState gCurrentAthenaLEFT_FE_SyncState;
#endif
#endif

#if 1//(NON_SCALING_OUTPUT == 1) && (DEINTERLACED_OUTPUT == 0)
void BypassMemory(void);
#endif
#if defined(DRAM_CODE_EXECUTION) && (DRAM_CODE_EXECUTION_MODE == 1)
void DramExecute(void);
#endif

#if(FEATURE_SOURCE_SCAN == ENABLE)
extern BYTE SourceScanStatus;
#endif
//******************************************************************************
//  L O C A L   V A R I A B L E S
//******************************************************************************

static BYTE         B_BlenderBothRequired;
static BYTE         B_BlenderBothReady;
static DWORD      D_BlenderPending[NUM_CHANNELS];
static BYTE       B_DisableDynamicScale = FALSE;
static BYTE       B_AutoAdjustStarted[NUM_CHANNELS] = {FALSE,FALSE};

//******************************************************************************
//  L O C A L    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
void            RetrieveAdcCalibrationValues(gmt_PHY_CH B_Channel);
static void       PreBlenderEnable(gmt_PHY_CH B_Channel);
static void       UpdateAOC(gmt_PHY_CH B_Channel);
static void       ModeSetupEnd(gmt_PHY_CH B_Channel);
static void       ModeSetupDynamicScaling(WORD);
static void       MonitorBlenders(gmt_PHY_CH B_Channel, DWORD Time);
static void       ModeSetupRequestExecute(gmt_PHY_CH B_Channel, BYTE B_Request, WORD W_Param);

void GameModeControl(void);
void gm_4K2K_MainCapture(void);
//******************************************************************************
//  L O C A L    T Y P E S
//******************************************************************************

struct DynamicScale
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
};

//*****************************************************************************
//  C O D E
//*****************************************************************************

//******************************************************************************
//
// FUNCTION       : void ModeSetupInit(void)
//
// USAGE          : Main()
//                  ApplicationInit()
//                  ModeSetupInit()
//                  Mode Setup Initialization - called once at the
//                  beginning Application Init. Prepares Queues, etc.
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : gmvw_VideoProcessingEnabled      (WO)
//                  gmvb_MainChannel            (RO)
//                  gmvb_PipChannel               (RO)
//                  B_FreeRun                  (WO)
//                  B_BlenderBothRequired         (WO)
//                  B_FocusBorder               (WO)
//                  B_DisableDynamicScale         (WO)
//                  Stp_CurrentMultiPipStyle      (WO)
//                  B_ModeSetupInit               (WO)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void ModeSetupInit(void)
{
   gmt_WindowStruct winDim;

   winDim.W_Hstart = 0;
   winDim.W_Vstart = 0;
   winDim.W_Width = PanelWidth;
   winDim.W_Height = PanelHeight;

   //
   // Main
   //
   B_FreeRun[gmvb_MainChannel] = FALSE;
   D_BlenderPending[gmvb_MainChannel] = 0;

   //
   // PIP
   //
   B_FreeRun[gmvb_PipChannel] = FALSE;
   D_BlenderPending[gmvb_PipChannel] = 0;

   //
   // Variable Init
   //
   B_BlenderBothRequired = FALSE;
   B_BlenderBothReady = FALSE;
   B_DisableDynamicScale = TRUE;

//Kordonm add for test 2011 0926
if (Is_PIP_ON() && IsSideBySidePipMode(UserPrefPipMode))
	B_FocusBorder = gmvb_PipChannel;
else
	B_FocusBorder = gmvb_MainChannel;	//ST Org

   gmvd_VideoProcessingEnabled = 0;

   //
   // Remove Borders
   //
   gm_HighlightBorder(FALSE,0,0,0,0);

   //
   // Blank Blenders
   //
   SetDisplayOff(gmvb_MainChannel, BLENDER_OFF);
   SetDisplayOff(gmvb_PipChannel, BLENDER_OFF);

   //
   // Color Window (Auto Blanking) // TODO : Remove
   //
   gm_DisplayColorWindowSetup(&winDim, RGB_565(MAIN_BACKGROUND_COLOR));

   B_ModeSetupInit = TRUE;
   B_ModeSetupStarted = FALSE;
   stInput[gmvb_MainChannel].Flags= 0; // so far only xvYCC used and clear the flag
   stInput[gmvb_PipChannel].Flags= 0; // so far only xvYCC used and clear the flag
}


//******************************************************************************
//
// FUNCTION       : void ModeSetupStartup(void)
//
// USAGE          : Main()
//                  ApplicationInit()
//                  ModeSetupInit()
//                  Mode Setup Startup - called once at the end of
//                  Application Init.  Displays PIP, etc.
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : gmvw_VideoProcessingEnabled      (WO)
//                  gmvb_MainChannel            (RO)
//                  gmvb_PipChannel               (RO)
//                  B_FreeRun                  (WO)
//                  B_BlenderBothRequired         (WO)
//                  B_FocusBorder               (WO)
//                  B_DisableDynamicScale         (WO)
//                  Stp_CurrentMultiPipStyle      (WO)
//                  B_ModeSetupInit               (WO)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void ModeSetupStartup(void)
{
   B_ModeSetupStarted = TRUE;

   // Bring PIP up Based on NVRAM
   B_PrevPipMode = 0xff;
   if (IsSideBySidePipMode(UserPrefPipMode) == TRUE) // || (UserPrefPipMode == NO_PIP))
   {
      // Need to indicate to change main on future pip change,
      // only happens if last mode was side by side
      B_PrevPipMode = SIDE_BY_SIDE;
   }
   if (UserPrefPipMode != NO_PIP)
   {
      ModeSetupDisplayChange(gmvb_PipChannel, DISPLAY_CHANGE_NORMAL);
      ModeSetupBlender(gmvb_PipChannel, STARTUP_PIP_TMO);
      B_DisableDynamicScale = TRUE;
   }
   else
      B_PrevPipMode = UserPrefPipMode;

}

//******************************************************************************
//
// FUNCTION       : void PreModeChange(gmt_PHY_CH B_Channel)
//
// USAGE          : Called by OSD and PortSelection when a change is being
//                  made that will affect the Mode.  Cancels any Scheduled
//                  events for the existing Mode.
//
// INPUT          : B_Channel - CH_A or CH_B
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
void PreModeChange(gmt_PHY_CH B_Channel)
{
   ModeSetupRequestCancel(B_Channel, REQ_ANY, PARAM_ANY);

   if (AutoScanCondition(gmvb_InputConnector[gmvb_MainChannel])==FALSE)
   {
      OSDRequestCancelMsg(B_Channel, MTO_InvalidMode);
	  
  #ifdef DELL_U2410F	  
	OSDRequestCancelMsg(B_Channel, MTO_OutOfRange); //Neil120615+
	OSDRequestCancelMsg(B_Channel, MTO_NoSync);
  #else 
      OSDRequestCancelMsg(B_Channel, MTO_NoSignal);
  #endif
#ifdef CHECK_IS_CABLE_CONNECT
      OSDRequestCancelMsg(B_Channel, MTO_NoCable); // SCR2456
#endif
   }

   if (B_AutoAdjustStarted[B_Channel] == TRUE)
   {
      OSDRequestCancelMsg(B_Channel, REQ_ANY);
      OSDRequestMsg(B_Channel, MTO_AutoAdjustCompleted, REQUEST_NOW);
   }
}

//******************************************************************************
//
// FUNCTION       : void PreBlenderEnable(gmt_PHY_CH B_Channel)
//
// USAGE          : Called just before Blenders are turned ON.  Any "last
//                  minute" filter loading can be done here after mode is
//                  setup and before blenders come on.
//
// INPUT          : B_Channel - CH_A or CH_B
//
// OUTPUT         : None
//
// GLOBALS        : gmvb_MainChannel            (RO)
//                  D_BlenderPending            (WO)
//                  B_BlenderBothRequired         (WO)
//                  UserPrefImageScheme            (RO)
//                  UserPrefSplitDirection         (RO)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void PreBlenderEnable(gmt_PHY_CH B_Channel)
{

   //
   // Load Appropriate H/V Filters before the blender is enabled (in Force mode)
   //
   UpdateFilters(B_Channel);
   //AdjustSharpness();//120530 Edward add here due to Athena without setup sharpness when system boot up

   //
   // Main
   //
   if (B_Channel == gmvb_MainChannel)
   {
      if (gm_IsVideo(gmvb_MainChannel) == TRUE)
      {
         if (gm_TimingIsStable(gmvb_MainChannel) == TRUE)
         {
            if (UserPrefImageScheme)
            {
#if defined(UserPrefSplitDirection) && defined(UserPrefSplitAccAcm)
               if (UserPrefSplitDirection)
               {
                  gm_CscSetVideoWindow(UserPrefSplitDirection); // Setup video window and program Set B of coefficients+offsets
                  if (UserPrefSplitAccAcm)
                  {
                     //gm_UpdateImageScheme(UserPrefImageScheme, ACC_ACM_SPLIT); // Turn on ACC/ACM
                  }
               }
               else
               {
                  //gm_UpdateImageScheme(IS_ON, 0); // Turn on ACC/ACM
               }
#else
               //gm_UpdateImageScheme(IS_ON, 0); // Turn on ACC/ACM
#endif
            }
         }
      }
      if (gm_IsVideo(gmvb_MainChannel) == FALSE)
      {
         //gm_UpdateImageScheme(IS_OFF, 0); // Turn off ACC/ACM
      }
   }

   //
   // PIP
   //
   else
   {
      // TODO: Currently no PIP Requirements
   }

   // Blender has been serviced
   D_BlenderPending[B_Channel] = 0;

   // One Blender is ON (No need to wait on other)
   B_BlenderBothRequired = FALSE;

}

//******************************************************************************
//
// FUNCTION       : BOOL BlenderIsPending(gmt_PHY_CH B_Channel)
//
// USAGE          : Determines if the Blender is OFF but waiting to come
//                  ON (pending)
//
// INPUT          : B_Channel - CH_A or CH_B
//
// OUTPUT         : TRUE - Pending
//                  FALSE
//
// GLOBALS        : D_BlenderPending            (RO)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BOOL BlenderIsPending(gmt_PHY_CH B_Channel)
{
   return(D_BlenderPending[B_Channel] > 0);
}

//******************************************************************************
//
// FUNCTION       : BOOL ModeSetupBusy(gmt_PHY_CH B_Channel)
//
// USAGE          : Used to determine if ModeSetup is processing an
//                  Input or Display change such as PIP Position
//
// INPUT          : B_Channel - CH_A or CH_B
//
// OUTPUT         : TRUE  - Busy
//                  FALSE - Idle
//
// GLOBALS        : B_ModeSetupQSize            (RO)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BOOL ModeSetupBusy(gmt_PHY_CH B_Channel)
{
   return ( (ModeSetupRequestCheck(B_Channel, REQ_MS_INPUT_CHANGE) != NO_INDEX)
            ||  (ModeSetupRequestCheck(B_Channel, REQ_MS_SETUP) != NO_INDEX)
            ||  (ModeSetupRequestCheck(B_Channel, REQ_MS_FREERUN) != NO_INDEX)
            ||  (ModeSetupRequestCheck(B_Channel, REQ_MS_SWAP) != NO_INDEX)
            ||  (ModeSetupRequestCheck(B_Channel, REQ_MS_DISPLAY_CHANGE) != NO_INDEX)
            ||  (ModeSetupRequestCheck(B_Channel, REQ_MS_AUTO_ADJUST) != NO_INDEX)
            //||  (ModeSetupRequestCheck(B_Channel, REQ_MS_WSS) != NO_INDEX)
            //||  (ModeSetupRequestCheck(B_Channel, REQ_MS_DISPLAY_TIMING) != NO_INDEX)
            //||  (ModeSetupRequestCheck(B_Channel, REQ_MS_USER_PREF) != NO_INDEX)
            //||  (ModeSetupRequestCheck(B_Channel, REQ_MS_TUNER) != NO_INDEX)
            ||  (ModeSetupRequestCheck(B_Channel, REQ_MS_BLENDER) != NO_INDEX)
            //||  (ModeSetupRequestCheck(B_Channel, REQ_MS_DYNAMIC) != NO_INDEX)
            //||  (ModeSetupRequestCheck(B_Channel, REQ_MS_AUDIO) != NO_INDEX)
            //||  (ModeSetupRequestCheck(B_Channel, REQ_MS_TUNER) != NO_INDEX)
          );
}

//******************************************************************************
//
// FUNCTION       : void ModeSetupHdlr(void)
//
// USAGE          : Main()
//                  ModeSetupHdlr()
//                  Mode Setup Handler functions for Main and PIP
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : gmvb_MainChannel            (RO)
//                  gmvb_PipChannel               (RO)
//                  B_ModeSetupQSize            (RW)
//                  ST_ModeSetupQ               (RW)
//                  B_PrevPipMode               (RW)
//                  B_BlenderBothRequired         (RW)
//                  B_DisableDynamicScale         (RW)
//                  W_OverscanEnabled            (RW)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
static t_REQUEST      ST_Request;
void ModeSetupHdlr(void)
{
   WORD          W_Data;
   WORD         W_Mask;
   BYTE         B_Channel;
   BYTE         B_Index;
   WORD          W_TimerRead;
   DWORD         D_Now = gm_GetSystemTime();

   //
   // If system is in low power mode return;
   //
   if (UserPrefPowerState == POWER_STATE_OFF)
      return;
   
#if (CUSTOMER_INFO == ENABLE)
	  if((B_SplashComplete == FALSE)&&(UserPrefLOGO_Ctrl == ON)) //Neil 20120606+ 
		  return;	
#endif

   //
   // Startup (Wait for Splash Screen to Finish)
   //
   if (B_ModeSetupStarted == FALSE)
   {
      if (B_SplashComplete == TRUE)
      {
         //
         // Enable speaker now, right before the image is shown
         //
         ModeSetupStartup();

      }
   }

   //
   // Started
   //
   else
   {
      //
      // For each Channel
      //
      for (B_Channel = CH_A; B_Channel <= CH_B; ++B_Channel)
      {
#if (FEATURE_3D == ENABLE)
         if (gm_3D_Is3DModeEnabled())
            if (gm_3D_IsWaitToDisplay() && (B_Channel == CH_A))
            {
               continue;
            }
#endif

         //
         // Process all Matured Events
         //
         ModeSetupRequestGet(B_Channel, REQ_ANY, &ST_Request);
         while (ST_Request.B_Request != REQ_NONE)
         {
            // Debug Scheduler
#ifdef DEBUG_SCHEDULER
            if (B_Channel == DEBUG_SCHEDULER)
               gm_Printf("MODULE_MODE_SETUP::[CH%d] Serviced:%d(%d)", B_Channel, ST_Request.B_Event, ST_Request.W_Param);
#endif

            //
            // Handle Event
            //
            ModeSetupRequestExecute(B_Channel, ST_Request.B_Request, ST_Request.W_Param );

            // More Events?
            ModeSetupRequestGet(B_Channel, REQ_ANY, &ST_Request);

         } // while Events

         //
         // Monitor Blenders
         //
         MonitorBlenders(B_Channel, D_Now);
         B_DisableDynamicScale = FALSE;

      } // for Channel


#ifdef DEBUG_VIDEO_PROCESSING
      if (gmvw_VideoProcessingEnabled != W_LastVideoProcessing)
      {
         DebugProcessing();
         W_LastVideoProcessing = gmvw_VideoProcessingEnabled;
      }
#endif

   } // Startup
}

#if (HDMI_DVI_TIMING_LOSS_PATCH==1)
//static WORD TestFlag=1;
//static WORD TestIndex;
BOOL HdmiLoss(gmt_PHY_CH B_Channel)
{
#if 1
   if (!((GetPortConnType(gB_CurrentInputConnector[B_Channel])==HDMI)||(GetPortConnType(gB_CurrentInputConnector[B_Channel])==DVI)))
      return FALSE;

     return (IsHdmiDviTimingChangeFromStable2Unstable(gB_CurrentInputConnector[B_Channel]));
#else
  if (TestFlag && TestIndex)
   {
   gm_Print("--- HdmiLoss TRUE (%d).",TestIndex);
   return(TRUE);
   }
  else
   {
   return(FALSE);
   }
  #endif
}
void MS_Abort(gmt_PHY_CH B_Channel)
{

if(B_Channel == CH_A)
   B_SignalStatus = STATUS_NO_SYNC;
ModeSetupRequestCancel(B_Channel, REQ_MS_SETUP, PARAM_NONE);
ModeSetupRequest(B_Channel, REQ_MS_RESTART, PARAM_NONE, 0);
ModeSetupRequestCancel(B_Channel, REQ_MS_BLENDER, PARAM_ANY);
ModeSetupRequestCancel(B_Channel, REQ_DEBUG, PARAM_NONE);

#if (defined(SMART_ISP) && defined(SMART_ISP_AUTO_DETECTION))
   if ((GetChannelConnType(B_Channel) == HDMI) || (GetChannelConnType(B_Channel) == DVI) || (GetChannelConnType(B_Channel) == CT_DISPLAY_PORT))
   {
      if (!gm_IsInterlaced(B_Channel))
      {
         EnableSmartIspDetect(FALSE);
      }
   }
#endif

#if (FEATURE_VWD == ENABLE)
   if ((UserPrefVwdFlag == ON) && (B_Channel == CH_A))
      VWD_Stop();
#endif

#if (FEATURE_ACR == ENABLE)
   if ((UserPrefDCR == ON) && (B_Channel == CH_A))
      ACR_Disable();
#endif

  gm_ForceModeChanged(B_Channel);
  //gm_Print("--- MS_Abort: %d",B_Channel);
}
#endif
//******************************************************************************
//
// FUNCTION       : static void ModeSetupRequestExecute(gmt_PHY_CH B_Channel, BYTE B_Request, WORD W_Param)
//
// USAGE          : Executes a Mode Setup function that corresponds to B_Request
//
// INPUT          : B_Channel
//                  B_Request
//                  W_Param
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
static void ModeSetupRequestExecute(gmt_PHY_CH B_Channel, BYTE B_Request, WORD W_Param)
{
   switch (B_Request)
   {
         //
         // Init
         //
      case REQ_INIT:
         debugRequest("MODULE_MODE_SETUP::REQ_INIT(%d)", B_Channel);
         break;

         //
         // Swap
         //
      case REQ_MS_SWAP:
         debugRequest("MODULE_MODE_SETUP::REQ_MS_SWAP(%d)", B_Channel);
         if (B_Channel == gmvb_MainChannel)
         {
            ModeSetupSwap();
         }
         break;

         //
         // Freerun
         //
      case REQ_MS_FREERUN:
         debugRequest("MODULE_MODE_SETUP::REQ_MS_FREERUN(%d)", B_Channel);
#ifdef DEBUG_MODE_SETUP
         D_Elapsed[0] = gm_GetSystemTime();
#endif
         ModeSetupFreerun(B_Channel);
         break;

      case REQ_MS_RESTART:
         gm_StateDetectionConfig(B_Channel, DET_RESTART);      // Start detection
         break;

         //
         // ModeSetup
         //
      case REQ_MS_SETUP:
         debugRequest("MODULE_MODE_SETUP::REQ_MS_SETUP(%d)", B_Channel);
#ifdef DEBUG_MODE_SETUP
         D_Elapsed[1] = gm_GetSystemTime();
#endif
         ModeSetupFullSetup(B_Channel);
	  b_IsPipModeHandlerAvaliable = gmd_TRUE; //KevinL 20090108 for AC on DP/VGA in PIP mode abnormal display, main modesetup first than Pip. 
#ifdef DEBUG_MODE_SETUP
         D_Elapsed[2] = gm_GetSystemTime();
         /*
          * Ref. SCR #2143
          * In case blender was not to change, initialze the debug timing to
          * to aviod negative number shows up in Profiling
          */
         D_Elapsed[3] = D_Elapsed[2];
#endif
         break;

         //
         // Input Change
         //
      case REQ_MS_INPUT_CHANGE:
         debugRequest("MODULE_MODE_SETUP::REQ_MS_INPUT_CHANGE(%d)", B_Channel);
         ModeSetupInputChange(B_Channel);
         break;

         //
         // Display Change
         //
      case REQ_MS_DISPLAY_CHANGE:
         debugRequest("MODULE_MODE_SETUP::REQ_MS_DISPLAY_CHANGE(%d)", B_Channel);
         ModeSetupDisplayChange(B_Channel, (BYTE)(W_Param));
         break;

         //
         // WSS
         //
      case REQ_MS_WSS:
         AdjustAspectRatioMain();
         debugRequest("MODULE_MODE_SETUP::REQ_MS_WSS(%d)", B_Channel);
         break;

         //
         // Display Timing Change (Film Mode)
         //
      case REQ_MS_DISPLAY_TIMING:
         debugRequest("MODULE_MODE_SETUP::REQ_MS_DISPLAY_TIMING(%d)", B_Channel);
         //if (B_Channel == gmvb_MainChannel)
         //{
         //ModeSetupDisplayTiming();
         //}
         break;

         //
         // Display Auto Adjust
         //
      case REQ_MS_AUTO_ADJUST:
#if FEATURE_AIP == ENABLE
         debugRequest("MODULE_MODE_SETUP::REQ_MS_AUTO_ADJUST(%d)", B_Channel);
         ModeSetupAutoAdjust(B_Channel);
#endif
         break;

         //
         // User Pref Change
         //
      case REQ_MS_USER_PREF:
         debugRequest("MODULE_MODE_SETUP::REQ_MS_USER_PREF(%d)", B_Channel);
         ModeSetupUserPref(B_Channel);
         break;

         //
         // Blender
         //
      case REQ_MS_BLENDER:
         debugRequest("MODULE_MODE_SETUP::REQ_MS_BLENDER(%d)", B_Channel);
         ModeSetupEnd(B_Channel);
         break;

         //
         // Dynamic Scaling
         //
      case REQ_MS_DYNAMIC:
         debugRequest("MODULE_MODE_SETUP::REQ_MS_DYNAMIC(%d)", B_Channel);
         ModeSetupDynamicScaling(W_Param);
         break;

         //
         // Audio
         //
      case REQ_MS_AUDIO:
         debugRequest("MODULE_MODE_SETUP::REQ_MS_AUDIO(%d)", B_Channel);
         break;

         //
         // Tuner
         //
      case REQ_MS_TUNER:
         debugRequest("MODULE_MODE_SETUP::REQ_MS_TUNER(%d)", B_Channel);
         break;

         //
         // DRAM_EXECUTE
         //
      case REQ_DRAM_EXECUTE:
         debugRequest("MODULE_MODE_SETUP::REQ_DRAM_EXECUTE(%d)", B_Channel);
         #if defined(DRAM_CODE_EXECUTION) && (DRAM_CODE_EXECUTION_MODE == 1)
         DramExecute();
         #endif
         break;
         
         //
         // Debug
         //
      case REQ_DEBUG:
         debugRequest("MODULE_MODE_SETUP::REQ_DEBUG(%d)", B_Channel);
#ifdef DEBUG_MODE_SETUP
         switch (W_Param)
         {
            case PARAM_DEBUG_VIDEO:
               DebugProcessing();
               break;
            default:
               DebugMode(B_Channel);
               break;
         }
#endif
         break;

         //
         // Error
         //
      default:
         debugRequest("MODULE_MODE_SETUP::Unsupported Request(%d)", B_Request);
         break;

   } // switch
}

//******************************************************************************
//
// FUNCTION       : static void MonitorBlenders(gmt_PHY_CH B_Channel, DWORD Time)
//
// USAGE          :
//
// INPUT          : B_Channel
//                  Time
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
static void MonitorBlenders(gmt_PHY_CH B_Channel, DWORD Time)
{
   BOOL B_TurnOnMain = FALSE;
   BOOL B_TurnOnPip = FALSE;

#ifdef DEBUG_MODE_SETUP
   if (D_BlenderPending[B_Channel])
      D_Elapsed[3] = gm_GetSystemTime();
#endif
   if ((D_BlenderPending[B_Channel] > 0)
         && (Time > D_BlenderPending[B_Channel])
	#if(SPEEDUP_MODE_CHANGE == ENABLE)
         && ((Time - D_BlenderPending[B_Channel]) > MAX_BLENDER_WAIT_TIME/SPEEDUP_FACT))
         #else
         && ((Time - D_BlenderPending[B_Channel]) > MAX_BLENDER_WAIT_TIME))
	#endif
   {
      D_BlenderPending[B_Channel] = 0;

      if (B_Channel == gmvb_MainChannel)
         B_TurnOnMain = TRUE;
      else
         B_TurnOnPip = TRUE;

      //if both blenders are supposed to turn on, force them on
      if (B_BlenderBothRequired == TRUE)
      {
         B_BlenderBothRequired = FALSE;

         if (B_Channel == gmvb_MainChannel)
         {
            //if Pip blender is deferred but both are supposed to turn on,
            //  force it on
            if (D_BlenderPending[gmvb_PipChannel] == 0)
               B_TurnOnPip = TRUE;
         }
         else
         {
            //if Main blender is deferred but both are supposed to turn on,
            //  force it on
            if (D_BlenderPending[gmvb_MainChannel] == 0)
               B_TurnOnMain = TRUE;
         }
      }

      if (B_TurnOnMain == TRUE)
      {
         // Force Main
         ModeSetupDisplayChange(gmvb_MainChannel, DISPLAY_CHANGE_NORMAL);
         debugBlender("** Blender Main Forced ON.", 0);
      }

      if (B_TurnOnPip == TRUE)
      {
         // Force Pip
         if ((UserPrefPipMode != MULTI_PIP) && (UserPrefPipMode != NO_PIP))
         {
            ModeSetupDisplayChange(gmvb_PipChannel, DISPLAY_CHANGE_NORMAL);
            debugBlender("** Blender PIP Forced ON.", 0);
         }
      }
   }
}


//******************************************************************************
//
// FUNCTION       : static void ModeSetupDynamicScaling(WORD Param)
//
// USAGE          :
//
// INPUT          : Param
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
static void ModeSetupDynamicScaling(WORD Param)
{
   //
   // Start
   //

   //Make sure the screen is not in freeze state when doing DynamicScaling
   if (UserPrefFreeze != 0)
   {
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
   }


   if ((Param& 0xff) == PARAM_DYN_START)
   {
      //gm_Printf("DynStart",0);
      if (B_DynScalingCoast > 0)//This is TRUE when changing of direction or repeated requests
         gm_ScalerDynamicWait(NUM_VSYNCS);//make sure previous dynamic scaling is finished

      B_DynScalingCoast = DYN_COAST;
      ModeSetupRequest(gmvb_MainChannel, REQ_MS_DYNAMIC, (Param & ~0xff) | PARAM_DYN_STEP, REQUEST_NOW);
   }
   else if ((Param& 0xff) == PARAM_DYN_DEFAULT)
   {
      BYTE B_Channel;

      B_Channel = (Param>>8)& 0xff;

      //
      // Setup Dynamic scaling size Start
      //
      SetDynamicScalerSizeBegin(B_Channel);

      // Setup Display
      if (B_Channel == gmvb_MainChannel)
      {
         SetDynamicScalerStep(gmvb_MainChannel, 4);
         SetDynamicScalerStep(gmvb_PipChannel, 0);   // turn off pip scaling
         SetDisplayMain();
      }
      else
      {
         SetDynamicScalerStep(gmvb_MainChannel, 0);   // turn off Main scaling
         SetDynamicScalerStep(gmvb_PipChannel, 5);
         SetDisplayPip();
      }

      //
      // Setup Dynamic scaling size end
      //
      SetDynamicScalerSizeEnd(B_Channel);

      //
      // Program Dynamic Scaling
      //
      DynamicScalerStart(B_Channel);
      DynamicScalerProcess(FALSE);
      DynamicScalerStop();

      //
      // Reset variables in case this is called during a zoom/shrink in SBS.
      //
      B_DynScalingDirection = DYN_IDLE;
   }
   //
   // Step
   //
   else
   {
      //gm_Printf("DynStep",0);
      if (((Param>>8)& 0xff) == DYN_DIR_INCREASE)
      {
         if (B_FocusBorder == gmvb_PipChannel)
            ShowDynamicScaler(DYN_DIR_DECREASE);
         else
            ShowDynamicScaler(DYN_DIR_INCREASE);
      }
      else if (((Param>>8)& 0xff) == DYN_DIR_DECREASE)
      {
         if (B_FocusBorder == gmvb_PipChannel)
            ShowDynamicScaler(DYN_DIR_INCREASE);
         else
            ShowDynamicScaler(DYN_DIR_DECREASE);
      }
      else // Idle
      {
         // Should never happen
      }
      if (B_DynScalingCoast > 0)
         --B_DynScalingCoast;
      if (B_DynScalingCoast == 0)
      {
         if ((SW_DynScalePosition < (B_DynPixelsPerSeq * 3))//This logic is for aligning left and right windows when the sizes are almost the same
               && (SW_DynScalePosition >  -(SBYTE)(B_DynPixelsPerSeq* 3)))
         {
            ShowDynamicScaler(DYN_DIR_RESET);
         }
         B_DynScalingDirection = DYN_IDLE;
         DynamicScalerStop();
      }
      else
      {
         ModeSetupRequest(gmvb_MainChannel, REQ_MS_DYNAMIC, (Param & ~0xff) | PARAM_DYN_STEP, DYN_COAST_INTERVAL);
      }
   }
}


//******************************************************************************
//
// FUNCTION       : static void ModeSetupEnd(gmt_PHY_CH B_Channel)
//
// USAGE          :
//
// INPUT          : B_Channel
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
#ifdef ST_4K2K_93xx_BOARD
WORD FE_HWidth[2];
WORD FE_VLength[2];
WORD FE_VFreq[2];
WORD FE_InputType[2];
#endif
static void ModeSetupEnd(gmt_PHY_CH B_Channel)
{
#if (defined(RD2_93xx_BOARD)) && (PATCH_OF_RD2_LVDS_LOCKUP==1)
   BYTE tmp_WIDTH_SEL;
#endif
dtm_msg("== 07 == ModeSetupEnd:	 %d", gm_GetSystemTime());

#if (DEBUG_HDMI_PRINT_TIMING_DISPLAY_TIME!=0) || (DEBUG_DP_PRINT_TIMING_DISPLAY_TIME!=0)
    gm_Print(" --> MSetEndB: T(%d)",(WORD)(gm_GetSystemTime()-GetDebugDisplayTime()));
#endif    

#if (DEBUG_DISPLAY_TIME_MEASUREMENT == ENABLE)
   dtm_msg("DTM> MSetEnd_B_T(%Ld)",gm_GetSystemTime());
#endif


#if 0 // (FEATURE_HDMI == ENABLE)
   if (GetChannelConnType(B_Channel) == HDMI)
   {
#if (CHIP_ID == STDP93xx)
      //if player support HDCP function, AUTHENTICATION bit should be set after ModeSetupFullSetup()
      if (((gm_ReadRegByte(HDRX_HDCP_STATUS) & (HDRX_HDCP_DEC_STATUS|HDRX_HDCP_AUTHENTICATION)) == HDRX_HDCP_AUTHENTICATION)
            &&(gm_ReadRegByte(HDRX_IRQ_STATUS) & HDRX_LINK_IRQ_STS))
#endif
      {
         ModeSetupBlender(B_Channel, DISPLAY_DEFER_TMO_NORMAL*10);////SCR 3622 wait HDCP authorization for 1 sec
         return;
      }
   }
#endif//(FEATURE_HDMI == ENABLE)
   //CheckInputAlignment(B_Channel);
   CheckScanlingFilterCtrl(B_Channel);

   // re-trigger the AFR source
   gm_AfrRestart(B_Channel);

#if 0 // (HDMI_DVI_TIMING_LOSS_PATCH==1) // Pip channel will call ModeSetupEnd() without stable signal when adjust pip mode.
   if(HdmiLoss(B_Channel)) // 8
   {
      MS_Abort(B_Channel);
      return;
   }
#endif

   #ifdef DELL_U2410F//Neil120906* don't set mode complete if pxp off
   if (gm_IsValidSignal(B_Channel))
   {
	   if(B_Channel == gmvb_MainChannel)
	   {
		 OSDRequestCancelMsg(gmvb_MainChannel, MTO_IconShow);			 
		 //OSDSendMessage(MTO_IconShow);
		 OSDRequestMsg (CH_A, MTO_IconShow, REQUEST_NOW);
		 stInput[B_Channel].ModeFlags |= gmd_MODE_COMPLETE;   // Mode setup is complete
	   }
	   else if(Is_PIP_ON())
	   {
		 //OSDRequestCancelMsg(gmvb_PipChannel, MTO_PIPIconShow);			   
		 PIPSendMessage(MTO_PIPIconShow);
		 //OSDRequestMsg_Pxp(B_Channel,MTO_PIPIconShow,20);
		 stInput[B_Channel].ModeFlags |= gmd_MODE_COMPLETE;   // Mode setup is complete
	   }

//20121106 for DP AFR enable, display gabage issue
      if (B_Channel == gmvb_MainChannel)
         gm_SetRegBitsWord(MC_FIFO_EN_lo, MC_FIFO_MAIN_WR);

	  /*
      if (B_Channel == gmvb_PipChannel)
            if(UserPrefOsdRotation != OSD_NON_ROTATED)	//when rotation, disable pip memory
                  gm_ClearRegBitsWord(MC_FIFO_EN_lo, MC_FIFO_PIP_WR);
            else										//when non-rotation, enable pip memory
                  gm_SetRegBitsWord(MC_FIFO_EN_lo, MC_FIFO_PIP_WR);
                  */
   }
   #else
   if (gm_IsValidSignal(B_Channel))
      stInput[B_Channel].ModeFlags |= gmd_MODE_COMPLETE;   // Mode setup is complete
   #endif   
   B_FreeRun[B_Channel] = FALSE;               // Clear flag

   // Determine which one is on top
   gm_BlenderSetPriority((gmvb_MainChannel == CH_A)?PIP_ON_MAIN:MAIN_ON_PIP);

   //if((UserPrefColorTemp > USER_TEMP) && ((gm_ReadRegWord(IMP_LUT_CONTROL) & IMP_LUT_EN) == 0))
   HighBrightness = 0;
   LowBrightness = 0;
   AVGBrightness = 0;

	//Hyb130920- Disable since DP in Pxp image error//Hyb130825+ for pxp DP input position error;
	/*
	if(gm_IsModeComplete(gmvb_PipChannel)&&gm_IsInputDP(gmvb_PipChannel))
	{
		if(gm_ReadRegWord(ip_control[gmvb_PipChannel]&IPP_EXT_DV_EN))
			gm_ClearRegBitsWord(IPP_CONTROL,IPP_EXT_DV_EN);
	}
	*/
   // Color temp follow main channel...
   if(B_Channel == gmvb_MainChannel) 
   {
   	  #if  (defined(CMI_73XX_BOARD) )//owen 121017-1//120613 Edward for DCR / Energy use
	   if(IS_DCR_ON() || IS_ENERGY_SMART_ON())
	   {
	  	  DCR_EnergyUse_Brightness = UserPrefBrightness;
		  
		  #if PWM_LowLevel_Smooth
		  if(IS_ENERGY_SMART_ON()) //20120913 Edward
		  {
		  	if(DCR_EnergyUse_Brightness <= WB_Brigntness_OSD_DCR_Start)
				DCR_Energy_LowStep_Brightness = DCR_EnergyUse_Brightness*FineStep_DCR_PWM;
			
			//gm_Print("##DCR_EnergyUse_Brightness  %d",DCR_EnergyUse_Brightness);
		  }
		  #endif
		  AdjustBackLight();//120913 for DCR/Energy enable should be load again orignal BL setting
	   }
	  #endif 	
	  
#if (defined(RD2_93xx_BOARD)) && (PATCH_OF_RD2_LVDS_LOCKUP==1)
      tmp_WIDTH_SEL=gm_ReadRegByte(LVDS_DIGITAL_CTRL) & WIDTH_SEL;
      gm_ClearRegBitsByte(LVDS_DIGITAL_CTRL,WIDTH_SEL);
#endif

	  dtm_msg("== 07 1== ModeSetupEnd:	   %d", gm_GetSystemTime());

   }

	#if(SPEEDUP_MODE_CHANGE == ENABLE)
	if(SysChFlag[B_Channel]&(SysCh_ACOn|SysCh_DCOn|SysCh_InputChange|SysCh_ChannelChange))
	{
	 	  //if first time setup from AC or port change, setup this before display on, 
	  	//otherwise image will have garbage
		 if(B_Channel == gmvb_MainChannel)
		{
		   AdjustSharpness();	//Neil120707+ 
		   AdjustColorTemp();  //Chunbao120920+ setup main color even current setup pxp channel. VWD will make main color abnormal when setup pxp channel
		 }
	
		 // Force xvYCC when video input is updated in AdjustColorSpace so
	 	// SetupColorSpace must after this function.
		SetupColorSpace(B_Channel);
		#if (FEATURE_OVERDRIVE == ENABLE) // OD Pre-frame workaround
		AdjustLCDOverdrive();
		#endif
	}
	#else
		 if(B_Channel == gmvb_MainChannel)
		{
		   AdjustSharpness();	//Neil120707+ 
		   AdjustColorTemp();  //Chunbao120920+ setup main color even current setup pxp channel. VWD will make main color abnormal when setup pxp channel
		 }
		
		 // Force xvYCC when video input is updated in AdjustColorSpace so
		// SetupColorSpace must after this function.
		SetupColorSpace(B_Channel);
		#if (FEATURE_OVERDRIVE == ENABLE) // OD Pre-frame workaround
		AdjustLCDOverdrive();
		#endif
	#endif
	
	 dtm_msg("== 07 2 == ModeSetupEnd Before SetDisplayOn:	  %d", gm_GetSystemTime());


	#ifdef DELL_U2410F
		#ifdef USE_TIMER_CTRL_AUD_PWR
		//if(AudioDetect())			//Neil120924- Disable this condition, turn it on any way
			AudioPowerStatus = ON;			//Neil 20090325+ for audio power control
		#endif
		B_NoMSGAfterSS = FALSE;
      #endif

#if 0 // (HDMI_DVI_TIMING_LOSS_PATCH==1)
   if(HdmiLoss(B_Channel)) // 9
   {
      MS_Abort(B_Channel);
      return;
   }
#endif
#if(defined(BOARD_93XX_4K2K))
		#if (DEFAULT_PANEL == ZDT_4K2K_QFHD)		//For PA168
			   //if(stInput[gmvb_MainChannel].Htotal > 4096)
			   {
				   gm_TimerStop(ZDT_4K2K_Reset168_100ms_TMR);
				   gm_TimerStart(ZDT_4K2K_Reset168_100ms_TMR, 20);
				   gm_DisplayOnOff(OFF);
			   }
		#endif
#endif

   if ((B_Channel == gmvb_PipChannel) && (UserPrefPipMode == NO_PIP))
   {
      D_BlenderPending[B_Channel] = 0;
      B_BlenderBothRequired = FALSE;
   }
   else
   {
      if (B_BlenderBothRequired == TRUE)
      {
         // Are both channels ready to come on simultaneously?
         if (B_BlenderBothReady == TRUE)
         {
            // Re-trigger the other AFR source
            gm_AfrRestart(gm_OtherChannel(B_Channel));

		if(B_Channel == CH_B)
		{
			PreBlenderEnable(CH_B);
			#if (SPEEDUP_MODE_CHANGE == DISABLE)
			gm_SyncUpdateWait(CH_B, INPUT_CLOCK);
			#endif
		}
		if(B_Channel == CH_A)
            	{
	            	PreBlenderEnable(CH_A);
			#if (SPEEDUP_MODE_CHANGE == DISABLE)
	            	gm_SyncUpdateWait(CH_A, INPUT_OUTPUT_CLOCK);
			#endif
		}
		//Neil 20081212+ for LCD condition runing
		if (UserPrefGlobalLCDOnOffFlag == FALSE)
		{	 
			#if (SPEEDUP_MODE_CHANGE == ENABLE)
			if(Is_PIP_ON())
				SetDisplayOn(CH_B);
			SetDisplayOn(CH_A); 
			#else
			#if 1//(DEFAULT_PANEL != ZDT_4K2K_QFHD)		//For PA168
			if(B_Channel == gmvb_MainChannel)
			{
				if(gm_IsValidSignal(B_Channel))
					OSDRequestMsg(gmvb_MainChannel, MTO_ValidMode, REQUEST_NOW);
				gm_TimerStart(Display_Enable_Main_10ms_TMR,  MainDisplayEnableDelay);
				
				#ifdef MODE_CHANGE_BACKLIGHT_OFF
					SetDisplayOn(CH_A); 
				#endif
			}
			else if(Is_PIP_ON())
				gm_TimerStart(Display_Enable_Pxp_10ms_TMR,  PxpDisplayEnableDelay);
			#endif
			#endif
	 	}
            B_BlenderBothReady = FALSE;
            debugBlender("** Both Blenders Enabled.", B_Channel);
         }
         // Other Blender Pending (ie ready to come on)?
         else if ((D_BlenderPending[gm_OtherChannel(B_Channel)] > 0)
            && (B_FreeRun[gm_OtherChannel(B_Channel)] == FALSE))
         {
            B_BlenderBothReady = TRUE;
            D_BlenderPending[B_Channel] = 0;
            ModeSetupRequestCancel(B_Channel, REQ_MS_BLENDER, PARAM_ANY);
            debugBlender("Blender(%d) Request Deferred.", B_Channel);
         }
         // Wait Again (Max Blender wait below will force on)
         else
         {
            if (B_FreeRun[gm_OtherChannel(B_Channel)] == TRUE)
               B_BlenderBothRequired = FALSE;

            ModeSetupRequest(B_Channel, REQ_MS_BLENDER, PARAM_NONE, DISPLAY_DEFER_TMO_NORMAL);
            debugBlender("Blender(%d) Request deferred.", B_Channel);
         }
      }
      else
      {
         PreBlenderEnable(B_Channel);
#if (SPEEDUP_MODE_CHANGE == DISABLE)
         gm_SyncUpdateWait(B_Channel, INPUT_OUTPUT_CLOCK);
#endif
		 //Neil 20081212+ for LCD condition runing
		 if (UserPrefGlobalLCDOnOffFlag == FALSE)
		 {	 
			#if (SPEEDUP_MODE_CHANGE == ENABLE)
			if(B_Channel == gmvb_MainChannel)
				SetDisplayOn(B_Channel);
			else if(Is_PIP_ON())
				SetDisplayOn(B_Channel);
			
			if(IsDPInput(B_Channel))
                       	 	Reset_DP_Audio();//JC_SZ_0924		
			#else
			#if 1//(DEFAULT_PANEL != ZDT_4K2K_QFHD)		//For PA168
			if(B_Channel == gmvb_MainChannel)
			{
				if(gm_IsValidSignal(B_Channel))
					OSDRequestMsg(gmvb_MainChannel, MTO_ValidMode, REQUEST_NOW);
				gm_TimerStart(Display_Enable_Main_10ms_TMR,  MainDisplayEnableDelay);
				
				#ifdef MODE_CHANGE_BACKLIGHT_OFF
					SetDisplayOn(CH_A); 
				#endif
			}
			else if(Is_PIP_ON())
			gm_TimerStart(Display_Enable_Pxp_10ms_TMR,  PxpDisplayEnableDelay);
			#endif
			#endif
		 }

         debugBlender("** Blender(%d) Enabled.", B_Channel);
      }
   } // !NO_PIP
   dtm_msg("== 07 3 == ModeSetupEnd SetDisplayOn:	%d", gm_GetSystemTime());

	#if(SPEEDUP_MODE_CHANGE == ENABLE)
	//Not from AC/DC on, not from port change;
	if((SysChFlag[B_Channel]&(SysCh_ACOn|SysCh_DCOn|SysCh_InputChange|SysCh_ChannelChange)) == 0)
	{
		if(B_Channel == gmvb_MainChannel) 
		{
			AdjustPresetModes();
			ApplyDUMTable();	//20120920 yvonne load DUM while modesetup
			
			AdjustSharpness();	//Neil120707+ 
			AdjustColorTemp();	 //Chunbao120920+ setup main color even current setup pxp channel. VWD will make main color abnormal when setup pxp channel
		}
		// Force xvYCC when video input is updated in AdjustColorSpace so
		// SetupColorSpace must after this function.
		SetupColorSpace(B_Channel);

		#if (FEATURE_OVERDRIVE == ENABLE) // OD Pre-frame workaround
			AdjustLCDOverdrive();
		#endif
	}
	#endif
	//clear all flag for next time reuse;
	SysChFlag[B_Channel] &= ~SysCh_All;
#ifdef NO_PIP_SUPPORT
   // For EV6_8541_BOARD and EV5_8531_BOARD disable the PIP, OD read/write FIFOs.
   // Increase priority for Motion Vectors WR FIFO 1 -> 2
   // Disable CCS FIFO clients for higher than 480i video on COMPONENT
   if ((B_Channel == CH_A))
   {
      if (gm_IsAnyHDVideo(CH_A)==TRUE)
      {
         // Turn off CC.
         // Instead of turning off CC decoder, call block display.
         // Turning off CC decoder will not turn CC back on after a source change is done.
         // CC will be re-enabled after a source change by SetDisplayOn() calling CC_EnableDisplay()
         // which will unblock the display.
      }

      // Turn off PIP capture and blender
      gm_ClearRegBitsWord(IPP_CONTROL, IPP_RUN_EN);
      gm_ClearRegBitsWord(PB_CTRL, PB_PIP_ENABLE);
      gm_SyncUpdateWait(CH_B, INPUT_OUTPUT_CLOCK);

      // disable unwanted MC clients (PIP and OD)
      gm_FifoControl(DISABLE, (FIFO_PIP_WR | FIFO_PIP_RD | FIFO_OD_WR | FIFO_OD_RD));

      // change MV WR FIFO priority to avoid DDR BW on 1080i
      gm_SetRegBitsWord(MC_FIFO1_CONFIG, BIT9);
      gm_ClearRegBitsWord(MC_FIFO1_CONFIG, BIT8);
   }
#endif

#if 0 // (HDMI_DVI_TIMING_LOSS_PATCH==1)
   if(HdmiLoss(B_Channel)) // 10
   {
      MS_Abort(B_Channel);
      return;
   }
#endif

   //SWENG1200201 Modified for power on 1st time DpTX no display in Passthru mode 


#ifdef NO_CCS_ON_HD_SUPPORT
   if ((B_Channel == CH_A))
   {
      if (gm_IsAnyHDVideo(CH_A)==TRUE)
      {
         gm_FifoControl(DISABLE, (FIFO_CCS_WR | FIFO_CCS_RD));   // disable CCS FIFO clients
      }
      else
      {
         gm_FifoControl(ENABLE, (FIFO_CCS_WR | FIFO_CCS_RD));   // enable CCS FIFO clients
      }
   }
#endif
#ifdef DELL_U2410F		//Neil120922+
#ifndef NO_CCS_ON_HD_SUPPORT
     gm_FifoControl(DISABLE, (FIFO_CCS_WR | FIFO_CCS_RD));   // disable CCS FIFO clients
#endif
#if (DEFAULT_PANEL == LG_WHD_LM290WW1_SSA1)//Michael for 29" 20121011
	if((gm_ReadRegWord(OVERDRIVE_CTRL0)&OVERDRIVE_EN) == 0)	//disable OD FIFO clients if not enable OD functions
		gm_FifoControl(DISABLE, (FIFO_OD_WR | FIFO_OD_RD));
#endif
#endif

#if (FEATURE_CEC == ENABLE)
   if (B_Channel == gmvb_MainChannel)
   {
      UserExecutionTimeoutMain = 0;
      B_UserCommandExecution &=(BYTE)(~BIT0);//Main
   }
   else
   {
      UserExecutionTimeoutPip = 0;
      B_UserCommandExecution &=(BYTE)(~BIT1); //Pip
   }
#endif

#if defined(DRAM_CODE_EXECUTION) && (DRAM_CODE_EXECUTION_MODE == 1)
   if(SystemFlags.sys.DramCodeExecution != TRUE)
   {
      ModeSetupRequest(B_Channel, REQ_DRAM_EXECUTE, PARAM_NONE, 500);
   }
#endif

#ifndef DELL_U2410F		//Neil120814 move to full setup
//Neil120814* move before VWD so VWD can stop it if VWD start
#if (FEATURE_ACR == ENABLE)
   if ((UserPrefDCR == ON) && (B_Channel == CH_A))
      ACR_Enable();
#endif

#if 0//(FEATURE_VWD == ENABLE) //20120925 yvonne move after display on enable VWD
   if ((UserPrefVwdFlag == ON) && (B_Channel == CH_A))
      VWD_Start();
#endif
#endif

//=========Audio control===================
#if 1//defined(CMI_73XX_BOARD)//120517 Edward all source //Neil120727* check channel, otherwise will start timer when pxp off
	if(((B_Channel == gmvb_MainChannel)&&(IsDPInput(CH_A)|| IsMainHDMIInput())) ||
		((B_Channel == gmvb_PipChannel)&&(Is_PIP_ON() && (IsDPInput(CH_B)|| IsPIPHDMIInput()))))
	{
		//if(IsAudioPacketPresent(UserPrefCurrentInputMain))
			//AUDIO_UNMUTE();
		AudioUpdateNow = gmd_FALSE;
		B_NeedRedoDACGain = gmd_TRUE;
		gm_TimerStart(AUDIO_WAIT_10ms_TMR, GetAudioTimerValue());	//Neil120901* 300ms later than image output
		gm_Printf("## %d ##Audio timer start %d",B_Channel,gm_GetSystemTime());
	}	
#endif
//=================================

	b_IsPipModeHandlerAvaliable = TRUE; //KevinL 20090108 for AC on DP/VGA in PIP mode abnormal display, main modesetup first than Pip. 
	
#if (FEATURE_HDMI == ENABLE) && (USE_HDMIDVI_AFR_DETECT_INTERRUPT!=0)
   if ((GetPortConnType(gB_CurrentInputConnector[B_Channel])==HDMI)||(GetPortConnType(gB_CurrentInputConnector[B_Channel])==DVI))
     HdmiDviEnableAfrInterrupt(gB_CurrentInputConnector[B_Channel], TRUE);
#endif


dtm_msg("== 08 == ModeSetupEnd:	 %d", gm_GetSystemTime());
	SaveModeIndependentSettings();
	SavePortSchemeDependentSettings();
	SavePipPortSchemeDependent();
	gm_Printf("== 08 B_Channel = %d == ModeSetupEnd:	 %d", B_Channel,gm_GetSystemTime());
	
#if (DEBUG_HDMI_PRINT_TIMING_DISPLAY_TIME==1) 
    gm_Print("PortInit --> ModeSetupEnd: T(%d)",(WORD)(gm_GetSystemTime()-GetDebugDisplayTime()));
#elif (DEBUG_HDMI_PRINT_TIMING_DISPLAY_TIME==2) || (DEBUG_DP_PRINT_TIMING_DISPLAY_TIME!=0)
    gm_Print("CablePlugged -->ModeSetupEnd: T(%d)",(WORD)(gm_GetSystemTime()-GetDebugDisplayTime()));
#endif    

#if (DEBUG_DISPLAY_TIME_MEASUREMENT == ENABLE)
   dtm_msg("DTM> MSetEnd_T(%Ld)",gm_GetSystemTime());
#endif

#if 0//def ST_4K2K_93xx_BOARD
	if(GetChipID_4K2K()==CHIPID_4K2K_R)
	{
		if(UserPrefCurrentInputMain == ALL_BOARDS_DP2)
		{
			if(SocIPCGetSlaveTimingInfo(SOCIPC_DEVICEFE_DST_ADDRESS))
	      	{
				FE_HWidth[B_Channel] = gAthena_LEFT_FE_Slave_Parameters.HWidth;
				FE_VLength[B_Channel] = gAthena_LEFT_FE_Slave_Parameters.VLength;
				FE_VFreq[B_Channel] = gAthena_LEFT_FE_Slave_Parameters.VFreq;
				FE_InputType[B_Channel] = gAthena_LEFT_FE_Slave_Parameters.ModeFlags;
			}

			#if 1
			gm_TimerStart(AthenaR_GetFEInfo_Timeout_10ms_TMR, 100);
			
			while(FE_HWidth[B_Channel] == 0)
			{
				if(gm_TimerCheck(AthenaR_GetFEInfo_Timeout_10ms_TMR) == TIMER_TMO)
		        {
			        gm_TimerStop(AthenaR_GetFEInfo_Timeout_10ms_TMR);	         
		            break;
		        }
				
				if(SocIPCGetSlaveTimingInfo(SOCIPC_DEVICEFE_DST_ADDRESS))
	      		{
	      			//gm_Print("Read FE info error, read again", 0);
					FE_HWidth[B_Channel] = gAthena_LEFT_FE_Slave_Parameters.HWidth;
					FE_VLength[B_Channel] = gAthena_LEFT_FE_Slave_Parameters.VLength;
					FE_VFreq[B_Channel] = gAthena_LEFT_FE_Slave_Parameters.VFreq;
					FE_InputType[B_Channel] = gAthena_LEFT_FE_Slave_Parameters.ModeFlags;
				}
			}
			#endif

			if(SocIPCGetSlaveModeInfo(SOCIPC_DEVICEFE_DST_ADDRESS))
			{
				FE_InputType[B_Channel] = gAthena_LEFT_FE_Slave_Parameters.ModeFlags;
			}
		}
	}
   #if ENABLE_ATHENA_R_L_IMAGE_SYNC_HANDLER
      Set4K2K_ImageSyncReady(); //Add by ChengLin 20130408
   #endif   
#endif
}

//******************************************************************************
//
// FUNCTION       : void ModeSetupSwap(void)
//
// USAGE          : Request Handler to Manage Swap.  This handler can be
//                  called directly from the Application, or via the
//                  scheduler.
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : gmvb_MainChannel            (RO)
//                  gmvb_PipChannel               (RO)
//                  B_BlenderBothRequired         (WO)
//                  B_DisableDynamicScale         (WO)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void ModeSetupSwap(void)
{
   //gm_ModeSwap();
   Freeze(gmvb_MainChannel, FALSE);
   Freeze(gmvb_PipChannel, FALSE);
   DynamicScalerStop();
   B_BlenderBothRequired = TRUE;
   B_DisableDynamicScale = TRUE;
   UserPrefFreeze = 0;
}

//******************************************************************************
//
// FUNCTION       : void ModeSetupFreerun(gmt_PHY_CH B_Channel)
//
// USAGE          : Request Handler for Freerun.  This handler can be called
//                  directly from the Application, or via the scheduler.
//               This function disables ISR of and all video processing on B_Channel. The
//               output of B_Channel is painted black by either force background or
//               turning off blender. Scaler is set to bypass and memory adjusted. This
//               function also searches for a valid source for display clock lock. If no
//               such source exists, display clock is set to freerun.
//
// INPUT          : B_Channel - CH_A or CH_B
//
// OUTPUT         : None
//
// GLOBALS        : B_DisableDynamicScale         (WO)
//                  gmvb_MainChannel            (RO)
//                  gmvw_InputHStart            (WO)
//                  gmvw_InputHActive            (WO)
//                  gmvw_InputVStart            (WO)
//                  gmvw_InputVActive            (WO)
//                  gmvw_OutputHActive            (RO)
//                  gmvw_OutputVActive            (RO)
//                  gmvb_DDDSSourceChannel         (RW)
//               gmvb_SyncDetectClamp         (W)
//               B_FreeRun                  (RW)
//               gmvb_MemoryNumBuffers         (W)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void ModeSetupFreerun(gmt_PHY_CH B_Channel)
{
	#ifdef EXTERNAL_DECODER
	BYTE B_InputConnector = (B_Channel == CH_A) ? UserPrefCurrentInputMain : UserPrefCurrentInputPip;
   #endif
   // reset gmvb_SyncDetectClamp so that when clamping registers need to be updated
   //     by the next mode change, the system does not think that these clamping
   //    registers are already set up correctly due to channel clone.
   gmvb_SyncDetectClamp[gmd_ADC_0] = TRUE;

   //
   // Disable ISR as no valid signal
   //
   if ( B_Channel == CH_A )     // Disable AdaptISR for CH_A
   {
#if (FEATURE_INTERRUPT_SERVICE == REV2)
      INTERRUPT_DISABLE(MAIN_INPUT_IRQ3MASK, IMP_VS_MASK);
#endif

/*ChengLin 20130227: Disable AFR Interrupt*/
#ifdef ST_4K2K_93xx_BOARD 
#if ENABLE_ATHENA_R_L_IMAGE_SYNC_HANDLER
	//if(GetChipID_4K2K() != CHIPID_4K2K_FE)
		INTERRUPT_DISABLE(DISPLAY_DEC_IRQ5MASK, IMP_AFR_DETECT_MASK);
#endif
#endif
   }
   else                  // disable PIP VSync ISR (Odd Field Correction)
   {                           // in SysLib
#if (FEATURE_INTERRUPT_SERVICE == REV2)
      INTERRUPT_DISABLE(PIP_INPUT_IRQ4MASK, IPP_VS_MASK);
#endif
   }
   //
   // Reset the IFM source to default
   //
   if ( B_Channel == CH_A )
   {
		#ifdef EXTERNAL_DECODER
		if(GetPortConnType(B_InputConnector) != DIPX)
		#endif
      gm_ClearRegBitsWord( MIFM_CTRL , MIFM_SOURCE_SEL );
   }
   else
   {
      {
		#ifdef EXTERNAL_DECODER
		if(GetPortConnType(B_InputConnector) != DIPX)
		#endif
         gm_ClearRegBitsWord( PIFM_CTRL , PIFM_SOURCE_SEL );
      }
   }

#if (FEATURE_CCS == ENABLE)
   //
   // CCS - disable when timing unstable
   //
   if (B_Channel == CH_A)
      gm_VideoProcessingDisable(VIDEO_CCS);
#endif

   DynamicScalerStop();

   //
   // Not Already in Freerun
   //
   if (B_FreeRun[B_Channel] == FALSE)
   {
      // Indicate in Freerun
      B_FreeRun[B_Channel] = TRUE;

      // End Dynamic Scaling
      //gm_ScalerDynamicEnable(B_Channel, FALSE);

      //
      // Display off
      //
      if (B_Channel == gmvb_MainChannel)
      {
#if (FEATURE_BLENDER_FAILSAFE == REV_NONE)
         // Disable main channel mixing at the PIP blender (clearing PB_MAIN_ENABLE) instead
         // of just forcing it to background (setting PB_FORCE_MAIN_BKGND).  This is to
         // workaround an issue with the blender stopping for lack of data from the memory
         // when changing from YUV (16 bpp) to RGB (24 bpp).
         SetDisplayOff(gmvb_MainChannel, BLENDER_OFF);
#else
         SetDisplayOff(gmvb_MainChannel, BACKGROUND_ONLY);
#endif

         gm_ClearRegBitsWord(AUTO_BKGND_CONTROL,
                             IMP_BKGND_SDDS_ERR_EN|IMP_BKGND_DDDS_ERR_EN|IMP_BKGND_VTD_ERR_EN|
                             IMP_BKGND_DEC_ERR_EN|IMP_BKGND_SDDS2_ERR_EN );
         gm_ClearRegBitsWord(AFR_CONTROL, AFR_DISP_SDDS2_ERR_EN | AFR_DISP_SDDS_ERR_EN);
      }
      else
      {
#if (FEATURE_BLENDER_FAILSAFE == REV_NONE)
         // Part of the highlight border will go missing if Pip blender is not turned off and in
         //   tall side-by-side mode
         if (IsSideBySidePipMode(UserPrefPipMode))
         {
            SetDisplayOff(gmvb_PipChannel, BLENDER_OFF);
         }
         else
#endif
            // This is to prevent blanking out all the multipip windows if input is unplugged while
            // the corresponding window is active
            if (UserPrefPipMode != MULTI_PIP)
            {
               SetDisplayOff(gmvb_PipChannel, BACKGROUND_ONLY);
            }

         gm_ClearRegBitsWord(AUTO_BKGND_CONTROL, IPP_BKGND_SDDS_ERR_EN|IPP_BKGND_DDDS_ERR_EN|IPP_BKGND_VTD_ERR_EN|IPP_BKGND_DEC_ERR_EN|IPP_BKGND_SDDS2_ERR_EN );
         gm_ClearRegBitsWord(AFR_CONTROL, AFR_DISP_SDDS2_ERR_EN | AFR_DISP_SDDS_ERR_EN);
      }

      if (B_Channel == CH_A)
      {
#if (FEATURE_PIP_DCDI == ENABLE)
         gm_VideoProcessingDisable(VIDEO_PROCESS_ALL & (~VIDEO_DCDI_PIP));
#else
         gm_VideoProcessingDisable(VIDEO_PROCESS_ALL);
#endif
      }
      else
      {
#if (FEATURE_PIP_DCDI == ENABLE)
         gm_VideoProcessingDisable(VIDEO_DCDI_PIP);
#endif
      }

      //
      // Ensure data path is setup for Blender (Only required when not MultiPIP Multi PIP)
      //
      if (gm_IsValidSignal(B_Channel) == FALSE)
      {
         if (B_Channel == CH_A)
         {
            stInput[B_Channel].Hstart = 50;
            stInput[B_Channel].Vstart = 50;
            stInput[B_Channel].Hactive = gmvw_OutputHActive[B_Channel];
            stInput[B_Channel].Vactive = gmvw_OutputVActive[B_Channel];
            gm_ScalerBypass(B_Channel);
            // Set the number of MAIN memory buffers to 1
            gmvb_MemoryNumBuffers[B_Channel] = 1;
            gm_MemorySetup(B_Channel);
         }
         else
         {
            if (UserPrefPipMode != MULTI_PIP)
            {
               stInput[B_Channel].Hstart = 50;
               stInput[B_Channel].Vstart = 50;
               stInput[B_Channel].Hactive = gmvw_OutputHActive[B_Channel];
               stInput[B_Channel].Vactive = gmvw_OutputVActive[B_Channel];
               gm_ScalerBypass(B_Channel);
               gmvb_MemoryNumBuffers[B_Channel] = 1;
               gm_MemorySetup(B_Channel);
            }
         }
      }

      //
      // Attempt to find Lock Source
      // Lock Source still Invalid?
      //
      DisplayLockSource();
      if (gm_IsValidSignal(gmvb_DDDSSourceChannel) == FALSE)
      {      
         // Force Lock to Main and Freerun
         gmvb_DDDSSourceChannel = gmvb_MainChannel;
			if((gm_ReadRegWord(SYNC_CONTROL)&FRAME_LOCK_MODE))
			{
				if(gmv_OUTPUT_VTOTAL != PanelTypVTotal)
				{
					gmv_OUTPUT_VTOTAL = PanelTypVTotal;
				}      
            
         	gm_SetDisplayFreeRun();
			}
      }
      else
      {
         DisplayClockLock(B_Channel, NUM_OF_STEPS);
      }

      //
      // Update the Output and wait till completed
      //
      gm_SyncUpdateWait(B_Channel, OUTPUT_CLOCK);
   }

   //B_DisableDynamicScale = TRUE;
#ifdef AUTO_SCAN_INPUT
   AutoScanInit(B_Channel);
#endif
} 

//******************************************************************************
//
// FUNCTION       : void ModeSetupFullSetup(gmt_PHY_CH B_Channel)
//
// USAGE          : Request Handler for a complete channel setup.  This
//                  handler can be called directly from the Application, or
//                  via the scheduler.
//
// INPUT          : B_Channel - CH_A or CH_B
//
// OUTPUT         : None
//
// GLOBALS        : B_DisableDynamicScale         (WO)
//                  gmvw_InputHTotal            (WO)
//                  gmvw_InputHTotalOrg            (RO)
//                  gmvw_InputVTotalOrg            (RO)
//                  gmvb_MainChannel            (RO)
//                  gmvb_PipChannel               (RO)
//                  gmvw_InputModeAttr            (WO)
//                  B_AspectMethod               (WO)
//                  W_OverscanEnabled            (RO)
//                  B_FreeRun                  (WO)
//                  B_AutoAdjustCount            (RO)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
extern WORD STD_HPorch;

#if 0
void gm_DddsCal(void)
{
   DWORD D_DisplayTotal;
   DWORD D_InputTotal;
   DWORD D_Remainder;
   DWORD D_Numerator;
   DWORD D_Denominator;
   DWORD D_Fraction;
   DWORD D_CompareRatio;
   DWORD D_Test;
   WORD W_RefLine;
   WORD W_InputVTotal;
   BYTE B_Shift;
   BYTE B_ScaleDown;
   DWORD DW_TempHTol = stInput[gmvb_DDDSSourceChannel].Vtotal; // gm_ReadRegWord(DPRX_MS_HTOTAL);

   //gm_Print(" stInput[gmvb_DDDSSourceChannel].Vtotal = %d",  stInput[gmvb_DDDSSourceChannel].Vtotal);
   //gm_Print(" stInput[gmvb_DDDSSourceChannel].Htotal = %d",  stInput[gmvb_DDDSSourceChannel].Htotal);

//    W_InputVTotal = gm_ReadRegWord(DPRX_MS_VTOTAL); // stInput[gmvb_DDDSSourceChannel].Vtotal;
   W_InputVTotal = stInput[gmvb_DDDSSourceChannel].Vtotal;

   D_InputTotal = DW_TempHTol * W_InputVTotal; // stInput[gmvb_DDDSSourceChannel].Htotal * W_InputVTotal;
#if 0 // No interlace supported
   if (gm_IsInterlaced(gmvb_DDDSSourceChannel))
   {
      W_InputVTotal = (W_InputVTotal & ~1) * 2 + 1;
//SCR4035 re-calculate D_InputTotal
      D_InputTotal = (DWORD)stInput[gmvb_DDDSSourceChannel].Htotal * W_InputVTotal;

      D_InputTotal /= 2;
   }
#endif

   D_DisplayTotal = (DWORD)gmv_OUTPUT_HTOTAL * gmv_OUTPUT_VTOTAL;
   W_RefLine = stInput[gmvb_DDDSSourceChannel].Htotal;

   // Check for DWORD overflow situation.  If detected, increment the ScaleDown divider to 2.
   B_ScaleDown = (D_DisplayTotal >= ((DWORD)0xFFFFFFFFL /W_RefLine)) ? 2 : 1;
   //gm_Print("B_ScaleDown = %d", B_ScaleDown);
   D_Numerator = (DWORD)W_RefLine * (D_DisplayTotal / B_ScaleDown);
   //gm_Print("D_Numerator = %d", D_Numerator);
   D_Denominator = D_InputTotal / B_ScaleDown;
   //gm_Print("D_Denominator = %d", D_Denominator);
   D_CompareRatio = D_Numerator / D_Denominator;
   //gm_Print("~ 1 D_CompareRatio = %d", D_CompareRatio);

   // Ratio (Based on Vertical)
   if (D_CompareRatio > 4095)    // Use Vertical Ratio
   {
      W_RefLine = W_InputVTotal;
      // Check for DWORD overflow situation.  If detected, increment the ScaleDown divider to 2.
      B_ScaleDown = (D_DisplayTotal >= ((DWORD)0xFFFFFFFFL /W_RefLine)) ? 2 : 1;
      D_Numerator = (DWORD)W_RefLine * (D_DisplayTotal / B_ScaleDown);
      D_Denominator = D_InputTotal / B_ScaleDown;
      D_CompareRatio = D_Numerator / D_Denominator;

      //gm_Print("~ 2 D_CompareRatio = %d", D_CompareRatio);

   }

//SCR4035 re-calculate in case D_CompareRatio > 4095, e.g., 480i to WUXGA
   // Ratio (Based on half of Vertical)
   if (D_CompareRatio > 4095)    // Use half of Vertical Ratio
   {
      W_RefLine = W_InputVTotal/2;
      // Check for DWORD overflow situation.  If detected, increment the ScaleDown divider to 2.
      B_ScaleDown = (D_DisplayTotal >= ((DWORD)0xFFFFFFFFL /W_RefLine)) ? 2 : 1;
      D_Numerator = (DWORD)W_RefLine * (D_DisplayTotal / B_ScaleDown);
      D_Denominator = D_InputTotal / B_ScaleDown;
      D_CompareRatio = D_Numerator / D_Denominator;

      //gm_Print("~ 3 D_CompareRatio = %d", D_CompareRatio);
   }
//======================================================
   // Calculate Fractional of ((RLine * DTotal) / ITotal) * 2^24
   //    = Integer     = (int)(Numerator / Denominator)
   //  = Remainder = (Numerator - (Integer * Denominator))
   //    = Fraction    = (Remainder / Denominator) * 2^24
   //    = Fraction    = ((Remainder * 2^(x)) / Denominator) * 2^(24-x)
   D_Remainder = D_Numerator - (D_CompareRatio * D_Denominator);
   B_Shift = 0;
   D_Test = D_Remainder;
   while (((D_Test & 0x80000000L) == 0) && (B_Shift < 24))
   {
      ++B_Shift;
      D_Test = D_Test << 1;
   }
   D_Fraction = ((D_Remainder << B_Shift) / D_Denominator) << (24 - B_Shift);

   // Program Compare & Ref Line
   gm_WriteRegWord(DDDS1_REF_LINE_VAL, W_RefLine);
   gm_WriteRegWord(DDDS1_COMP_INTEGER, (WORD)(D_CompareRatio & 0xffff));
   gm_WriteRegDWord_LE(DDDS1_COMP_FRACTION, D_Fraction);
   //gm_WriteRegWord(DDDS_COMP_FRACTION_lo, (WORD)(D_Fraction & 0xffff));
   // +0x50 is request by Tommy due to avoid down spread clock issue with display flashing data in the right side of panel
   //gm_WriteRegWord(DDDS_COMP_FRACTION_hi, (WORD)(D_Fraction >> 16)+0x50);

#if 0 // Set in other code ...
   gm_DdsForceCloseLoop (CLK_DDDS);

   // Update Register
   gm_WriteRegWord(SYNC_CONTROL,
                   ((gmvb_DDDSSourceChannel == CH_B) ? LOCK_SOURCE : 0) |
                   (gm_IsInterlaced(gmvb_DDDSSourceChannel) ? (INTERLACE_EN|ODD_EN) : 0));
   gm_SyncUpdate(gmvb_MainChannel, OUTPUT_CLOCK);

   return gm_DdsWaitStable(CLK_DDDS); // Tracking Error
#endif
}

#endif

#ifdef ST_4K2K_93xx_BOARD
void PanelSetAsInputTiming (void)
{
   if(gmvw_InputHActiveOrg[gmvb_MainChannel]%2)
   {
      gm_Printf("~~~~ Input is not correct force re-detect %d", gmvw_InputHActiveOrg[gmvb_MainChannel]);
      gm_ForceModeChanged(gmvb_MainChannel);
      return;
   }

   if(gmvw_InputHStartOrg[gmvb_MainChannel] == 0)
   {      
      gm_ForceModeChanged(gmvb_MainChannel);
      return;
   }   

   _fmemcpy((BYTE far *)&CurPanelData, (BYTE far *)CurrentPanelData, sizeof(gmt_PanelData));
   CurrentPanelData = (gmt_PanelData far *) &CurPanelData;

   if(gm_IsInterlaced(gmvb_MainChannel))
   {
      if(stInput[gmvb_MainChannel].Vtotal % 2)
      {
         stInput[gmvb_MainChannel].Vtotal -= 1;
      } 
   }

   PanelWidth = stInput[gmvb_MainChannel].Hactive;
   PanelHeight = stInput[gmvb_MainChannel].Vactive;
   PanelMaxVFreq = (stInput[gmvb_MainChannel].Vfrq + 5) / 10;
   PanelMinVFreq = (stInput[gmvb_MainChannel].Vfrq + 5) / 10;
   PanelMaxPClk = ((DWORD)stInput[gmvb_MainChannel].Htotal * stInput[gmvb_MainChannel].Vtotal/10) * stInput[gmvb_MainChannel].Vfrq/1000;
   PanelMinHTotal = stInput[gmvb_MainChannel].Htotal;
   PanelTypHTotal = stInput[gmvb_MainChannel].Htotal;
   Panel22HTotal = stInput[gmvb_MainChannel].Htotal;
   Panel33HTotal = stInput[gmvb_MainChannel].Htotal;

   {
      WORD W_Hperiod;
      WORD W_Hpulse;
      W_Hperiod =gm_ReadRegWord(MIFM_HS_PERIOD);
      W_Hpulse = gm_ReadRegWord(MIFM_HS_PULSE);

      if (W_Hpulse > (W_Hperiod / 2))
         PanelMinHSyncWidth   = W_Hperiod - W_Hpulse;
      else
         PanelMinHSyncWidth   = W_Hpulse;

      PanelMinHSyncWidth= (BYTE)((DWORD)(PanelTypHTotal*PanelMinHSyncWidth) / W_Hperiod);
      PanelMinHSyncWidth &= 0xF8;
      
      if (PanelMinHSyncWidth <8)
         PanelMinHSyncWidth = 8;
   }
   PanelMinHSyncBackPorch = stInput[gmvb_MainChannel].Hstart - PanelMinHSyncWidth;
   PanelMinHSyncBackPorch &= 0xFFF8;

   Panel22VTotal=stInput[gmvb_MainChannel].Vtotal;
   Panel33VTotal=stInput[gmvb_MainChannel].Vtotal;
   PanelMaxVTotal=stInput[gmvb_MainChannel].Vtotal;
   PanelTypVTotal=stInput[gmvb_MainChannel].Vtotal;

   {
      WORD W_Vperiod;
      WORD W_Vpulse;
      
      W_Vperiod = gm_ReadRegWord(MIFM_VS_PERIOD);
      W_Vpulse = gm_ReadRegWord(MIFM_VS_PULSE);
      
      if (W_Vpulse > (W_Vperiod / 2))
         PanelMinVSyncWidth   =  W_Vperiod - W_Vpulse;
      else
         PanelMinVSyncWidth   = W_Vpulse;
   }

   PanelMinVSyncBackPorch = stInput[gmvb_MainChannel].Vstart - PanelMinVSyncWidth;

   if(gm_IsInterlaced(gmvb_MainChannel))
   {
      PanelHeight = stInput[gmvb_MainChannel].Vactive * 2;
      PanelMaxPClk = ((DWORD)stInput[gmvb_MainChannel].Htotal * (stInput[gmvb_MainChannel].Vtotal*2 + 1)/10) * stInput[gmvb_MainChannel].Vfrq/1000;
      Panel22VTotal = stInput[gmvb_MainChannel].Vtotal * 2 + 1;
      Panel33VTotal = stInput[gmvb_MainChannel].Vtotal * 2 + 1;
      PanelMaxVTotal = stInput[gmvb_MainChannel].Vtotal * 2 + 1;
      PanelTypVTotal = stInput[gmvb_MainChannel].Vtotal * 2 + 1;
   }

   gmv_OUTPUT_HTOTAL = PanelTypHTotal; // Typical
   gmv_OUTPUT_VTOTAL = PanelTypVTotal; // Typical
   gmv_OUTPUT_HTOTAL_22 = Panel22HTotal; // HTotal for 2:2 (60Hz to 48Hz conversion)
   gmv_OUTPUT_VTOTAL_22 = Panel22VTotal; // VTotal for 2:2 (60Hz to 48Hz conversion)
   gmv_OUTPUT_HTOTAL_33 = Panel33HTotal; // HTotal for 3:3 (60Hz to 72Hz conversion)
   gmv_OUTPUT_VTOTAL_33 = Panel33VTotal; // VTotal for 3:3 (60Hz to 72Hz conversion)
   gmv_OUTPUT_HACTIVE = PanelWidth;
   gmv_OUTPUT_VACTIVE = PanelHeight;      
}
#endif


void ModeSetupFullSetup(gmt_PHY_CH B_Channel)
{
   BYTE B_TVChannel;

#if (DEBUG_HDMI_PRINT_TIMING_DISPLAY_TIME!=0) || (DEBUG_DP_PRINT_TIMING_DISPLAY_TIME!=0)
   gm_Print(" -->MSetFulB: T(%d)",(WORD)(gm_GetSystemTime()-GetDebugDisplayTime()));
#endif

#if (DEBUG_DISPLAY_TIME_MEASUREMENT == ENABLE)
   dtm_msg("DTM> MSetFul_B_T(%Ld)",gm_GetSystemTime());
#endif

dtm_msg("== 05 == ModeSetupFullSetup:   %d", gm_GetSystemTime());

#if PATCH_OF_DVI_AFB_DIABLE == 1       //Patch for AFB always be triggered if input from DVI front-end.
   if (GetChannelConnType(B_Channel) == DVI)
   {
      if (B_Channel==CH_A)
         gm_ClearRegBitsWord(OVP_CTRL,MAIN_AFB_EN);
      else
         gm_ClearRegBitsWord(OVP_CTRL,PIP_AFB_EN);
   }
#endif

#if 1 // patch for PIP noise issue
   if (B_Channel == gmvb_MainChannel)
   {
      gm_WriteRegWord(HARD_RESETS_2, IMP_ISM_FIFO_RESET);
      gm_Delay1us(5);  // 1us is enough
      gm_WriteRegWord(HARD_RESETS_2, 0);
   }
   else
   {
      gm_WriteRegWord(HARD_RESETS_2, IPP_ISM_FIFO_RESET);
      gm_Delay1us(5);  // 1us is enough
      gm_WriteRegWord(HARD_RESETS_2, 0);
   }
#endif

#if (FEATURE_TWO_STAGE_PROCESSING_MODE == ENABLE)
   if (IsTwoStageAllowed()&& (UserPrefPipMode == NO_PIP) && (B_Channel == CH_B))
      return;
#endif

 
   Freeze(B_Channel, FALSE);

   // Set Userpref Freeze to 0 only for Main Channel
   if (B_Channel == gmvb_MainChannel)
      UserPrefFreeze = 0;

   OSDRequestMsg(gmvb_MainChannel, MTO_UnFreeze, REQUEST_NOW);

   // Stop dynamic scaling
   DynamicScalerStop();
   B_DisableDynamicScale = TRUE;

   if (!gm_IsValidSignal(B_Channel))
      return;

   // Restore Original Data
   //stInput[B_Channel].Htotal = gmvw_InputHTotalOrg[B_Channel];
   //stInput[B_Channel].Vtotal = gmvw_InputVTotalOrg[B_Channel];

   DisplayLockSource(); //this function has been moved to here because gm_DdsCloseLoop(),
   //which may be called by gm_SourceClockSetup(), relies on gmvb_DDDSSourceChannel[]
   
#if (FEATURE_AIP == ENABLE)
   if (GetPortProc(gmvb_InputConnector[B_Channel]) == AIP)
   {
      RetrieveAdcCalibrationValues(B_Channel);
      
      gm_AipSetClamping(B_Channel);
      gm_SourceClockSetup(B_Channel, stInput[B_Channel].Htotal);
      gm_SetAdcPhase(0, UserPrefHSyncPhase);
      gm_ClockSelection(B_Channel);
      gm_AipDecimationFilterSetup(B_Channel);
 #if (FEATURE_GLITCH_FILTER == ENABLE)
      gm_GlitchFilterSetup(B_Channel);
 #endif
   }
#endif

   // Remove SDDS AFR display event if the DDDS is locked to a channel that
   // is not an AIP source. It is causing Main to detect the event when it
   // occurs for PIP channel, since AFR is checked if channel source is HDMI_DVIP.
   if (gm_IsInputHdmiDvi(gmvb_DDDSSourceChannel) || gm_IsInputDP(gmvb_DDDSSourceChannel))
   {
      gm_SetAfrControl(AFR_DISP_SDDS_ERR_EN, FALSE);
   }

   //gm_SetAfrControl(AFR_DISP_IFM_ERR_EN | AFR_DDDS_IFM_ERR_EN,(gm_IsInputDP(gmvb_DDDSSourceChannel)) ? FALSE : TRUE);
   gm_SetAfrControl(AFR_DISP_IFM_ERR_EN | AFR_DDDS_IFM_ERR_EN, TRUE);

#if (FEATURE_DISPLAY_PORT == ENABLE)
   if (gm_IsInputDP(B_Channel))
      DpRxAfrEnable(gmvb_InputConnector[B_Channel],TRUE);
#endif

   // Input Capture
   if (B_Channel == CH_A)
      gm_SetupInputMain();
 #ifndef NO_PIP_SUPPORT
   else
      gm_SetupInputPip();
 #endif


#if 0 //(FEATURE_DISPLAY_PORT == ENABLE)
   if (gm_IsInputDP(B_Channel))
   {
      #if (NON_SCALING_OUTPUT==1) && (PASS_THRU_MODE==1)
         gm_ClearRegBitsWord(ip_source[B_Channel], IMP_HS_INV |IMP_VS_INV);
         
      #else
         if (stInput[B_Channel].ModeFlags & gmd_MODE_NEG_HSYNC)
            gm_SetRegBitsWord(ip_source[B_Channel], IMP_HS_INV);

         if (stInput[B_Channel].ModeFlags & gmd_MODE_NEG_VSYNC)
            gm_SetRegBitsWord(ip_source[B_Channel], IMP_VS_INV);
      #endif         
   }
#endif

#ifdef ST_4K2K_93xx_BOARD
	if(GetChipID_4K2K()==CHIPID_4K2K_FE)
	{
		if(gm_ReadRegWord(DISPLAY_CONTROL) != 0x07)
			gm_WriteRegWord(DISPLAY_CONTROL,0x07); 
	}
#endif	
   // 20100330 #2, Correct Even / Odd for DP interlance
   // Test 062309, seperate IMP and IPP later.
   {
      WORD	W_IFM_CTRL = ((B_Channel == CH_A) ? MIFM_CTRL : PIFM_CTRL);
      gm_ClearRegBitsWord(W_IFM_CTRL, (MIFM_FREERUN_ODD));
      if (((GetChannelConnType(B_Channel)) == CT_DISPLAY_PORT) && gm_IsInterlaced(B_Channel))
      {
         gm_ClearRegBitsWord(W_IFM_CTRL, MIFM_INT_ODD_EN);
      }
   }

   if (GetChannelConnType(B_Channel)!= HDMI)
   {
      gm_ClearRegBitsWord(ip_source[B_Channel], IMP_SOURCE_CLK_INV);
   }

   if (B_Channel == gmvb_MainChannel)
   {   
	#if(defined(BOARD_93XX_4K2K))
	#if(DEFAULT_PANEL == ZDT_4K2K_CMI)	//Hyb140807*  For CMI M version tcon still need this
	PanelSetAsInputTiming(); // must before gm_MemorySetup 
	MemoryAllocate(B_Channel); // For max capture memory upadate
	#endif
	DisplaySetTiming();
	ReInitOSD();
	SetOSDByPanelParameters();
	AdjustOsdRotation();

	#endif
   
      #ifdef ST_4K2K_93xx_BOARD
      if(GetChipID_4K2K() != CHIPID_4K2K_FE)
      #endif
      {
      	SetDisplayMain();
      }
      #if 0//def ST_4K2K_93xx_BOARD
      else // CHIPID_4K2K_FE
      {
         gmvw_OutputHStart[gmvb_MainChannel] = 0;
         gmvw_OutputVStart[gmvb_MainChannel] = 0;
         gmvw_OutputHActive[gmvb_MainChannel] = stInput[gmvb_MainChannel].Hactive;
         if(gm_IsInterlaced(gmvb_MainChannel))
            gmvw_OutputVActive[gmvb_MainChannel] = stInput[gmvb_MainChannel].Vactive * 2;               
         else
            gmvw_OutputVActive[gmvb_MainChannel] = stInput[gmvb_MainChannel].Vactive;         

         if(GetChipID_4K2K()==CHIPID_4K2K_FE)
         {
            PanelSetAsInputTiming(); // must before gm_MemorySetup 
            MemoryAllocate(B_Channel); // For max capture memory upadate
            DisplaySetTiming();
         }             
      }
      #endif      
   }   
#ifndef NO_PIP_SUPPORT
   else
   {   
   		SetDisplayPip();
		#ifdef PIP_BORDER_ENABLED
	  	SetPipBorder(B_Channel, TRUE);
		#endif
   }
#endif

#if (HDMI_DVI_TIMING_LOSS_PATCH==1)
   if(HdmiLoss(B_Channel)) // 1  // 2 ms
   {
      MS_Abort(B_Channel);
      return;
   }
#endif

   if (B_Channel == CH_A)
      gm_AfmSetup(W_OverscanEnabled[CH_A]);

   gm_DisplaySetSize(B_Channel);
   gm_DisplaySetPosition(B_Channel);

   // Scaler
#ifdef SDRAM_BANDWIDTH_1TO1_VF
   if ((gm_IsHDVideo(B_Channel) && gm_IsInterlaced(B_Channel))// 1080i input
         && (PanelHeight == 1080)) // panel has 1080 lines
      gm_ScalerSetUseMVFBeforeMemory_1to1(B_Channel, TRUE); // to set Vert Filter before SDRAM
   else
      gm_ScalerSetUseMVFBeforeMemory_1to1(B_Channel, FALSE); // to set Vert Filter after SDRAM
#endif
   gm_ScalerSetup(B_Channel);

   gm_MemorySetup(B_Channel); // Video Memory
   
	#if 0//ndef ST_4K2K_93xx_BOARD
	if(B_Channel == CH_A)
	{
   		AdjustImageRotation();
	}
	#endif
   
#if (HDMI_DVI_TIMING_LOSS_PATCH==1)
   if(HdmiLoss(B_Channel)) // 2 // 3 ms
   {
      MS_Abort(B_Channel);
      return;
   }
#endif
   
/////...Hyb130715- move up before scaler setup

/*ChengLin 20120217: Add for QFHD Panel*/
#ifdef ST_4K2K_93xx_BOARD

 /*	
	if(GetChipID_4K2K()== CHIPID_4K2K_FE)
   {
		gm_WriteRegWord(HDRX_VID_REGEN_CTRL, 0x00);
	}
 */
	if(GetChipID_4K2K()== CHIPID_4K2K_L)
		ImageSync_HStart();
	
//	gm_SyncUpdate(gmvb_MainChannel, INPUT_OUTPUT_CLOCK);

#endif	

	dtm_msg("== 06 1== ModeSetupFullSetup:before AdjustPresetModes()	 %d", gm_GetSystemTime());

	#ifdef DELL_U2410F//120629 Edward add for port change color setting not loaded correctly 
		#if(SPEEDUP_MODE_CHANGE == DISABLE)
		if(B_Channel == gmvb_MainChannel)
		{
		   	AdjustPresetModes();
			ApplyDUMTable();	//20120920 yvonne load DUM while modesetup
		}
		//else		//Hyb130624- 
		#endif
	#endif
	dtm_msg("== 06 2== ModeSetupFullSetup:after AdjustPresetModes()	 %d", gm_GetSystemTime());

   // Move to ModeSetupEnd
   //SetupColorSpace(B_Channel);

	//gmv_OUTPUT_HTOTAL = PanelTypHTotal;
	//gmv_OUTPUT_VTOTAL = PanelTypVTotal;

   #if (ENABLE_DFL == 0)
	if((B_Channel == CH_A) && (GetChannelConnType (B_Channel) == VGA) && (IsGraphics(B_Channel) == TRUE)
		&& (stInput[B_Channel].Vactive == PanelHeight))
	{
		Store_Lock_Mode = B_DisplayLockMethod;
		if((gmvb_OutputFrameRate <= PanelMaxVFreq))
		{
			B_DisplayLockMethod = DISP_LOCK_NORMAL;
			#if 1
			Store_HTotal = stInput[B_Channel].Htotal;
			Store_VTotal = stInput[B_Channel].Vtotal;
			if(Real_HTotal <= PanelTypHTotal)
				stInput[B_Channel].Htotal = Real_HTotal;
			else
				stInput[B_Channel].Htotal = PanelTypHTotal;
			
		// 20120917 Torisa, for 1080P water wave issue
		if((Real_VTotal <= PanelTypVTotal)||(stInput[B_Channel].Hactive == 1920))
			stInput[B_Channel].Vtotal = Real_VTotal;
		else			
			stInput[B_Channel].Vtotal = PanelTypVTotal;
			
			#endif
		}
		else
			B_DisplayLockMethod = Store_Lock_Mode;
	}
	else
		   B_DisplayLockMethod = DISP_FREERUN;
   #endif

	#if 0	//Hyb1307Move to after clock setup
	// 20120207, Game Mode
   if (B_Channel == CH_A)
   {
      GameModeControl();

      #if (ENABLE_DFL == 1) // VGA Noise     
      if((GetChannelConnType (B_Channel) == VGA) && (IsGraphics(B_Channel) == TRUE))
      {
         if(stInput[B_Channel].Vactive == PanelHeight)
         {            
            gmv_OUTPUT_VTOTAL = stInput[B_Channel].Vtotal;
   			gm_WriteRegWord(DV_TOTAL, stInput[B_Channel].Vtotal);
         }
      }
      #endif  
   }
	#endif
	
   if(B_Channel == gmvb_DDDSSourceChannel)
   	DisplayClockLock(B_Channel, NUM_OF_STEPS);

#if (FEATURE_AIP == ENABLE) && (ENABLE_DFL == 0)
	if((B_Channel == CH_A) && (GetChannelConnType (B_Channel) == VGA) && (IsGraphics(B_Channel) == TRUE))
	{
		if((stInput[B_Channel].Vactive == PanelHeight) && (B_DisplayLockMethod == DISP_LOCK_NORMAL)
			&& (gmvb_OutputFrameRate <= PanelMaxVFreq))
			// && (stInput[B_Channel].Hactive == PanelWidth) && (stInput[B_Channel].Htotal > 2100))
		{
			gm_WriteRegWord(DV_TOTAL, stInput[B_Channel].Vtotal);
			gm_WriteRegWord(DH_LOCK_LOAD, 0x01);
			gm_WriteRegWord(DV_LOCK_LOAD, gm_ReadRegWord(DV_DE_START) - 1); // stInput[B_Channel].Vstart - 4);
			gm_WriteRegWord(IMP_VLOCK_EVENT, 1); // gm_ReadRegWord(DV_TOTAL));
			
			gm_ClearAndSetBitsWord(SYNC_CONTROL, FRAME_LOCK_MODE, BIT2);

			stInput[B_Channel].Htotal = Store_HTotal;
			stInput[B_Channel].Vtotal = Store_VTotal;
			SetupHTotal(stInput[B_Channel].Htotal);

			gm_ClearRegBitsWord(DDDS1_CONTROL, DDDS1_FORCE_OPLOOP);
			gm_SetRegBitsWord(DDDS1_INIT, 0x01);
		}
	}
#endif

//Hyb130720+ move here	otherwise DP output image distortion	 // 20120207, Game Mode
if (B_Channel == CH_A)
{
   GameModeControl();

   #if (ENABLE_DFL == 1) // VGA Noise	  
   if((GetChannelConnType (B_Channel) == VGA) && (IsGraphics(B_Channel) == TRUE))
   {
	  if(stInput[B_Channel].Vactive == PanelHeight)
	  { 		   
		 gmv_OUTPUT_VTOTAL = stInput[B_Channel].Vtotal;
		 gm_WriteRegWord(DV_TOTAL, stInput[B_Channel].Vtotal);
	  }
   }
   #endif  
}

#if (HDMI_DVI_TIMING_LOSS_PATCH==1)
   if(HdmiLoss(B_Channel)) // 3 // 7 ms
   {
      MS_Abort(B_Channel);
      return;
   }
#endif

   // User Preferences - Video Enhancement Features
   ModeSetupUserPref(B_Channel);

//#if (NON_SCALING_OUTPUT == 0)
   if ((B_Channel == CH_A)
	#ifdef OUTPUT_MODE_CHANGE
   	&&(UserPrefOutputMode == OPM_Normal)
	#endif
	)
      gm_ScalerSetup(B_Channel);      // (CH_A only; Changes in Spatial Processing Require Scaler Reconfig)

//#endif

#ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()!=CHIPID_4K2K_FE) 
   {
		if((B_Channel == CH_A)) // Modify later ... Disable it when DP input MST 
		{
			if ((UserPrefCurrentInputMain == ALL_BOARDS_DP1) ||(UserPrefCurrentInputMain == ALL_BOARDS_DP3))
			{
            if(DpRxIsMST_Stream(UserPrefCurrentInputMain)==FALSE)
				//if(UserPrefDPSWitch == DP_Input_11)
				{
               if(GetChipID_4K2K()==CHIPID_4K2K_R)
               {
                  if(UserPrefCaptureM != CAPTURE_MODE_HALF_HORIZONTAL) // Modify to contril by UserPrefPBPmpde later
                  UserPrefCaptureM = CAPTURE_MODE_RIGHT; // Right
               }
               else
               {
                  if(UserPrefCaptureM != CAPTURE_MODE_HALF_HORIZONTAL) // Modify to contril by UserPrefPBPmpde later
                  UserPrefCaptureM = CAPTURE_MODE_LEFT; // Left
               }
               //gm_Print("SST_Stream in DPRX0", 0);
               gm_4K2K_MainCapture();
				}
				else
				{
               if(GetChipID_4K2K()==CHIPID_4K2K_R)
               {
                  if(UserPrefCaptureM != CAPTURE_MODE_HALF_HORIZONTAL) // Modify to contril by UserPrefPBPmpde later
                  UserPrefCaptureM = CAPTURE_MODE_FULL; // Right
               }
               else
               {
                  if(UserPrefCaptureM != CAPTURE_MODE_HALF_HORIZONTAL) // Modify to contril by UserPrefPBPmpde later
                  UserPrefCaptureM = CAPTURE_MODE_FULL; // Left
               }
               //gm_Print("MST_Stream in DPRX0", 0);
               //gm_4K2K_MainCapture();
				}
			}
			else //if (UserPrefCurrentInputMain == ALL_BOARDS_DP2)
			{// HDMI
			   if(GetChipID_4K2K()==CHIPID_4K2K_R)
			   {
			   		if(UserPrefCaptureM != CAPTURE_MODE_HALF_HORIZONTAL) // Modify to contril by UserPrefPBPmpde later
						UserPrefCaptureM = CAPTURE_MODE_RIGHT; // Right
				}
				else
				{
					if(UserPrefCaptureM != CAPTURE_MODE_HALF_HORIZONTAL) // Modify to contril by UserPrefPBPmpde later
						UserPrefCaptureM = CAPTURE_MODE_LEFT; // Left
				}
				
				//gm_Print("HDMI in DPRX1.......... %d", 0);
				gm_4K2K_MainCapture();
			}
		}
	}
#elif defined(RD2_4K2K_DEMO)
      //if(DpRxIsMST_Stream(UserPrefCurrentInputMain)==FALSE)
      if(GetHdmiInputNumber()!=2)   
		{
         if(GetChipID_4K2K()==CHIPID_4K2K_R)
         {
           // if(UserPrefCaptureM != CAPTURE_MODE_HALF_HORIZONTAL) // Modify to contril by UserPrefPBPmpde later
            UserPrefCaptureM = CAPTURE_MODE_RIGHT; // Right
         }
         else
         {
           // if(UserPrefCaptureM != CAPTURE_MODE_HALF_HORIZONTAL) // Modify to contril by UserPrefPBPmpde later
            UserPrefCaptureM = CAPTURE_MODE_LEFT; // Left
         }
         //gm_Print("SST_Stream in DPRX0", 0);
         gm_4K2K_MainCapture();
		}
		else
		{

         UserPrefCaptureM = CAPTURE_MODE_FULL;
         /*
         if(1) //GetChipID_4K2K()==CHIPID_4K2K_R)
         {
            if(UserPrefCaptureM != CAPTURE_MODE_HALF_HORIZONTAL) // Modify to contril by UserPrefPBPmpde later
            UserPrefCaptureM = CAPTURE_MODE_FULL; // Right
         }
         else
         {
            if(UserPrefCaptureM != CAPTURE_MODE_HALF_HORIZONTAL) // Modify to contril by UserPrefPBPmpde later
            UserPrefCaptureM = CAPTURE_MODE_FULL; // Left
         }
         */
         //gm_Print("MST_Stream in DPRX0", 0);
         //gm_4K2K_MainCapture();
		}
#endif

   //
   // Enable ISR when input signal is stable and setup
   //
   if (B_Channel == CH_A)   // Enable AdaptISR for CH_A
   {
#if (FEATURE_INTERRUPT_SERVICE == REV2)
      INTERRUPT_ENABLE(MAIN_INPUT_IRQ3MASK, IMP_VS_MASK);
#endif

/*ChengLin 20130227: Enable AFR Interrupt*/
#ifdef ST_4K2K_93xx_BOARD 
#if ENABLE_ATHENA_R_L_IMAGE_SYNC_HANDLER
	//if(GetChipID_4K2K() != CHIPID_4K2K_FE)
		INTERRUPT_ENABLE(DISPLAY_DEC_IRQ5MASK, IMP_AFR_DETECT_MASK);
#endif
#endif
   }
   else  // enable PIP VSync ISR (Odd Field Correction)
   {     // in SysLib
#if (FEATURE_INTERRUPT_SERVICE == REV2)
      INTERRUPT_ENABLE(PIP_INPUT_IRQ4MASK, IPP_VS_MASK);
#endif
   }

#if (HDMI_DVI_TIMING_LOSS_PATCH==1)
   if(HdmiLoss(B_Channel)) // 4 // 3 ms
   {
      MS_Abort(B_Channel);
      return;
   }
#endif

	  //Hyb130625+ for input v start error sometimes because setup too fast to capture right data;
	  if(ReCheckInput(B_Channel)== gmd_FALSE)
	  	return;
	if(B_Channel == gmvb_MainChannel)
		AdjustImageRotation();
	
   if (GetChannelConnType(B_Channel) == CT_DISPLAY_PORT)
   {
      // Store DO EQ swing PreEmp for LPM mode.
      // DP_EQStore = 4;
      // DP_SWINGStore = 0;
      // DP_EMPStore = 1;
   }
   //
   // Auto AutoAdjust
   //
#if (FEATURE_AIP == ENABLE) && (defined(AUTO_AUTOADJUST))
   //if (gm_IsInputTripleADC(B_Channel))
   if (gm_IsInputTripleADC(B_Channel) && (GetChannelConnType(B_Channel) != COMPONENT)
   	&& IsGraphics(B_Channel)
	&& (B_AutoAdjustCount[B_Channel] < MAX_AUTO_ADJUST
	|| MAX_AUTO_ADJUST == 0)
	&&!gm_IsInterlaced(B_Channel))// && !UserPrefFactoryFlag)	//Hyb131106* factory mode still do auto
   {
	#ifdef ADC_BANDGAP_CALIBRATION		//Hyb130503+ for ADC auto color first time;
		  if((gm_ReadRegWord(ADC_BG_STATUS)&BG_CAL_DONE) != BG_CAL_DONE)
			  PerformAdcCalibration();
	#endif
	  
      //make sure autoadjust is not performed when pip cannot be seen on the screen
     //if (!((B_Channel == gmvb_PipChannel))&& (UserPrefPipMode == NO_PIP))		//Hyb131106- Pxp do auto now
	if ((B_Channel != gmvb_PipChannel))
	{
		//if (UserPrefCurrentInputMain == ALL_BOARDS_VGA1) 		//Neil 20081125+ only VGA perform auto,for component do auto issue
		if ((UserPrefCurrentInputMain == ALL_BOARDS_VGA1) 
			#if(FEATURE_SOURCE_SCAN == ENABLE)
			&& (SourceScanStatus == StopScanSource)
			#endif
			) //KevinL 20090320 
		{
			if(B_Channel == gmvb_MainChannel)	//Only main channel show OSD
			OSDRequestMsg(B_Channel, MTO_AutoAdjustStart, DISPLAY_DEFER_TMO_NORMAL + 20);

			// To have enough time for ModeSetupEnd to be finished before the request for AA executed
       		// The delay in ModeSetupEnd is caused by gm_GetScalerSharpness() in UpdateFilters() in ModeSetupEnd()
       		// AutoAdjust need to be excuted after some times.
			//ModeSetupRequest(B_Channel, REQ_MS_AUTO_ADJUST, PARAM_NONE, DISPLAY_DEFER_TMO_NORMAL + 250);
			B_AutoAdjustStarted[B_Channel] = TRUE;

            // Track Auto AutoAdjusts
            ++B_AutoAdjustCount[B_Channel];
         }
     }
	else		//Pxp channel
	{
		if (UserPrefCurrentInputPip == ALL_BOARDS_VGA1) 
		{
			//Do auto but don't show OSD
			OSDRequestCancelMsg(B_Channel, MTO_AutoAdjustStart);
			ModeSetupRequest(B_Channel, REQ_MS_AUTO_ADJUST, PARAM_NONE, DISPLAY_DEFER_TMO_NORMAL + 250);

			// Track Auto AutoAdjusts
			++B_AutoAdjustCount[B_Channel];
		}
	}
   }
#endif

   {
      if (GetChannelConnType(B_Channel) == TUNER)
         ModeSetupBlender(B_Channel, DISPLAY_DEFER_TMO_RF);
      else
      {
#if (FEATURE_AIP == ENABLE)
         if (gmvb_CurrentSyncState[B_Channel] == SOG_STATE)
            // component input requires extra time for clamping to stabilize
            ModeSetupBlender(B_Channel, DISPLAY_DEFER_TMO_COMPONENT_NORMAL);
         else
#endif
         {
#if (FEATURE_HDMI == ENABLE) || (FEATURE_DISPLAY_PORT== ENABLE)
            if ((GetChannelConnType(B_Channel) == HDMI) ||(GetChannelConnType(B_Channel) == DVI) ||(GetChannelConnType(B_Channel) == CT_DISPLAY_PORT))
            {
#ifdef ST_4K2K_93xx_BOARD
               /*ChengLin 20130314: Delay to do end of mode setup due  to inter-chip commands  for adjust color space and DUM will be failed*/
               if(GetChipID_4K2K() == CHIPID_4K2K_R)
                  ModeSetupBlender(B_Channel, 500);
               else
                  ModeSetupBlender(B_Channel, 0);                  
#else
               ModeSetupBlender(B_Channel, 0);
#endif
            }
            else
#endif//(FEATURE_HDMI == ENABLE)
	#if(SPEEDUP_MODE_CHANGE == ENABLE)
               ModeSetupBlender(B_Channel, REQUEST_NOW);
	#else
		ModeSetupBlender(B_Channel, DISPLAY_DEFER_TMO_NORMAL);
	#endif
         }
      }
   }
#if (HDMI_DVI_TIMING_LOSS_PATCH==1)
   if(HdmiLoss(B_Channel)) // 5
   {
      MS_Abort(B_Channel);
      return;
   }
#endif

   //
   // Sync Updates
   //
   gm_SyncUpdateWait( B_Channel, INPUT_OUTPUT_CLOCK);

   if (gmc_MemoryAllocationMethod == gmd_MEMORY_ALLOCATE_DYNAMIC)
   {
      //gmt_DECODER_ID B_DecoderID = gmd_DEC_CORTEZ_ID;
      //gmt_BUFF_TYPE W_BufferType = BUF_DECODER;
   }
#if (FEATURE_AIP == ENABLE) && (FEATURE_ADC_AOC == ENABLE)
   // Enable AOC (automatic offset compensation)
   UpdateAOC(B_Channel);
#endif

#if (FEATURE_TWO_STAGE_PROCESSING_MODE == ENABLE)
   if ((UserPrefTwoStageProcessing == TRUE))
   {
      if ((UserPrefPipMode ==  NO_PIP) && IsTwoStageAllowed() && (B_Channel == CH_A))
      {
         //need to set IPP_FRAME_RESET_LINE to make it work somehow
         gm_InputSetVPosition(CH_B);
         EnterTwoStageProcessing();
      }
      else
      {
         gm_VideoProcessingDisable(VIDEO_2STAGE);

         gm_WriteRegWord(DBL_FILT_CTRL, 0);
         /*
         if (gm_ReadRegWord(DBL_FILT_CTRL) & DBL_FILT_EN)
         {
            ExitTwoStageProcessing();
         }
         */
      }
   }
#endif

#if FEATURE_AIP == ENABLE
   if (gm_IsInputTripleADC(B_Channel) && gm_IsGraphics(B_Channel))
   {
      gm_InputSetVPosition(B_Channel);
   }
#endif
#if (HDMI_DVI_TIMING_LOSS_PATCH==1)
   if(HdmiLoss(B_Channel)) // 6 //  ms
   {
      MS_Abort(B_Channel);
      return;
   }
#endif


   // Debug
   #ifdef UART_DEBUG_MODE
   ModeSetupRequest(B_Channel, REQ_DEBUG, PARAM_NONE, 1000);
   #endif

#if (defined(SMART_ISP) && defined(SMART_ISP_AUTO_DETECTION))
   if ((GetChannelConnType(B_Channel) == HDMI) || (GetChannelConnType(B_Channel) == DVI) || (GetChannelConnType(B_Channel) == CT_DISPLAY_PORT))
   {
      if (!gm_IsInterlaced(B_Channel))
      {
         EnableSmartIspDetect(TRUE);
      }
   }
#endif
#if(IS_PROJ_4K2K())
		CheckScanlingFilterCtrl(B_Channel);

	if(gm_ReadRegWord(SYNC_CONTROL)&FRAME_LOCK_MODE)		//Free run ?
		gm_ClearRegBitsWord(SYNC_CONTROL,FRAME_LOCK_MODE);	//set to Free run.
	gm_ForceUpdate(CH_A,INPUT_OUTPUT_CLOCK);
		
#endif

   /*
      gm_ClearRegBitsWord(LVDS_DIGITAL_CTRL,WIDTH_SEL);
      gm_Delay1ms(10);
      gm_SetRegBitsWord(LVDS_DIGITAL_CTRL,WIDTH_SEL);

   */
#if (HDMI_DVI_TIMING_LOSS_PATCH==1)
   if(HdmiLoss(B_Channel)) // 7 //  ms
   {
      MS_Abort(B_Channel);
      return;
   }
#endif
//Neil120814* move here buy our 1 loop only...
#ifdef DELL_U2410F
	//#ifdef AUDIO_NOISE_REDUCE
	//AudioSourceSelect();	//Neil130425- 
	//#endif
	
	#if 0			//Hyb130429- audio PCM message.
	if(IsMainHDMIInput())//owen 120925
	{
		//gm_Print("##GetHDMIAudioPCMWarningMessage %d",GetHDMIAudioPCMWarningMessage());
		if(GetHDMIAudioPCMWarningMessage())	 
		{
			//gm_Print("##mute audio at HDMI",0);
			AUDIO_MUTE();
		}
		gm_TimerStart(AudioPCM_10ms_TMR, 100); 
	}
	#endif
//Neil120814* move before VWD so VWD can stop it if VWD start
#if (FEATURE_ACR == ENABLE)
   if ((UserPrefDCR == ON) && (B_Channel == CH_A))
      ACR_Enable();
#endif

#if 0//(FEATURE_VWD == ENABLE)	//20120925 yvonne move after display on enable VWD
   if ((UserPrefVwdFlag == ON) && (B_Channel == CH_A))
      VWD_Start();
#endif
#endif
   
//Hyb130715- move up before setup memory and scaling;
//ohterwise dptx image will wrong, DDS will setup as panel define
#if 0//(NON_SCALING_OUTPUT == 1)
      UpdateCurrentPanelData(gmvb_MainChannel);
      DisplaySetTiming();
 #if (PASS_THRU_MODE == 1) || (DEINTERLACED_OUTPUT == 0)
      gm_ClkSelect(CLK_ODP, ODP_SEL_IMP_CLK);
      gm_ClkSelect(CLK_LVTX_IN, LVTX_IN_SEL_IMP_CLK);
      gm_SetRegBitsWord(LVDS_CLOCK_CONFIG, DCLK_SEL);
      gm_ClkSelect(CLK_ODP_CAPTURE, ODP_CAPTURE_SEL_IMP_CLK);
 #endif

 #if (PASS_THRU_MODE == 0) && (DEINTERLACED_OUTPUT == 0)
   {
      HDMI_AVI_Y B_HdmiAviY = gm_HdmiGetAviColorSpace(gB_CurrentInputConnector[B_Channel]);
      gmt_CEA861_AVI_Y BDpAviY = DpRxGetAviColorSpace(gB_CurrentInputConnector[B_Channel]);

      if (((GetChannelConnType(gmvb_MainChannel) == HDMI) && (B_HdmiAviY == HDMI_AVI_Y_YUV422))
         || ((GetChannelConnType(gmvb_MainChannel) == CT_DISPLAY_PORT) && (BDpAviY == gmd_CEA861_AVI_Y_YUV422)))
      {
         gm_ClearRegBitsWord(OVP_CTRL, MAIN_INTERPOLATOR_BYPASS);
      }
      
      if (stInput[B_Channel].ModeFlags & gmd_MODE_INTERLACED)
      {
         gm_WriteRegWord(DV_TOTAL, gm_ReadRegWord(DV_TOTAL) | BIT0);
         INTERRUPT_ENABLE(DISPLAY_DEC_IRQ5MASK, D_VS_MASK);
      }
      else    
         INTERRUPT_DISABLE(DISPLAY_DEC_IRQ5MASK, D_VS_MASK);
   }
 #endif // (PASS_THRU_MODE == 0)
#endif // (NON_SCALING_OUTPUT==1)

#if (DEBUG_HDMI_PRINT_TIMING_DISPLAY_TIME!=0) || (DEBUG_DP_PRINT_TIMING_DISPLAY_TIME!=0)
      gm_Print(" -->MSetFulE: T(%d)",(WORD)(gm_GetSystemTime()-GetDebugDisplayTime()));
#endif    

#if (DEBUG_DISPLAY_TIME_MEASUREMENT == ENABLE)
   dtm_msg("DTM> MSetFul_T(%Ld)",gm_GetSystemTime());
#endif

#if (FEATURE_HDMI==ENABLE) && (DEBUG_HDMI_CPHY_CAPTURE !=0)
   if (GetChannelConnType(B_Channel) == HDMI)
      StopCaptureCphyRegs();
#endif    
}

//******************************************************************************
//
// FUNCTION       : void ModeSetupInputChange(gmt_PHY_CH B_Channel)
//
// USAGE          : Request Handler for a change to the Input Capture.  This
//                  handler can be called directly from the Application, or
//                  via the scheduler.
//
// INPUT          : B_Channel - CH_A or CH_B
//
// OUTPUT         : None
//
// GLOBALS        : B_DisableDynamicScale         (WO)
//                  gmvb_MainChannel            (RO)
//                  W_OverscanEnabled            (RO)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void ModeSetupInputChange(gmt_PHY_CH B_Channel)
{
   SetDisplayOff(B_Channel, BACKGROUND_ONLY);
   DynamicScalerStop();
   //B_DisableDynamicScale = TRUE;
#if (FEATURE_AIP == ENABLE)   
   RetrieveAdcCalibrationValues(B_Channel);
#endif

   if (B_Channel == gmvb_MainChannel)
      SelectAspectMain();
   else
      SelectAspectPip();
   SetCapture(B_Channel);
   // Do not change the hardware config unless the mode is determined
   if (gm_IsValidSignal(B_Channel))
   {
      if (B_Channel == CH_A)
      {
         gm_SetupInputMain();
         gm_AfmSetup(W_OverscanEnabled[B_Channel]);
      }
   #ifndef NO_PIP_SUPPORT
      else
      {
         gm_SetupInputPip();
      }
   #endif
   }

#if(SPEEDUP_MODE_CHANGE == ENABLE)
   ModeSetupBlender(B_Channel, DISPLAY_DEFER_TMO_NORMAL/SPEEDUP_FACT);
#else
ModeSetupBlender(B_Channel, DISPLAY_DEFER_TMO_NORMAL);
#endif
   gm_SyncUpdate(B_Channel, INPUT_CLOCK);
}

//******************************************************************************
//
// FUNCTION       : void ModeSetupDisplayChange(gmt_PHY_CH B_Channel, BYTE B_Method)
//
// USAGE          : Request Handler for a change to the Output.  This
//                  handler can be called directly from the Application, or
//                  via the scheduler.
//
// INPUT          : B_Channel - CH_A or CH_B
//                  B_Method - DISPLAY_CHANGE_NORMAL
//                     DISPLAY_CHANGE_QUICK
//
// OUTPUT         : None
//
// GLOBALS        : B_DisableDynamicScale         (WO)
//                  gmvb_MainChannel            (RO)
//                  gmvb_PipChannel               (RO)
//                  W_OverscanEnabled            (RO)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void ModeSetupDisplayChange(gmt_PHY_CH B_Channel, BYTE B_Method)
{
   BYTE B_TVChannel;

#if (FEATURE_TWO_STAGE_PROCESSING_MODE == ENABLE)
   if (gm_Is2StageEnabled() && (B_Channel == CH_B))
      return;
#endif

   Freeze(B_Channel, FALSE);
   // Set Userpref Freeze to 0 only for Main Channel
   if (B_Channel == gmvb_MainChannel)
   {
      UserPrefFreeze = 0;
      OSDRequestMsg(gmvb_MainChannel, MTO_UnFreeze, REQUEST_NOW);
   }
	if(B_Method == DISPLAY_CHANGE_QUICK )
		SysChFlag[B_Channel] |= SysCh_ModeChange_Quick;
	else
		SysChFlag[B_Channel] |= SysCh_ModeChange_Normal;
   	

   {
      //
      // Setup Dynamic scaling size Start
      //
      SetDynamicScalerSizeBegin(B_Channel);

      // Setup Display
      if (B_Channel == gmvb_MainChannel)
      {
         SetDynamicScalerStep(gmvb_MainChannel, 1);		//Neil120725* org = 4
         SetDynamicScalerStep(gmvb_PipChannel, 0);   // turn off pip scaling
         SetDisplayMain();
      }
      else
      {
         SetDynamicScalerStep(gmvb_MainChannel, 0);   // turn off Main scaling
         if(UserPrefPipMode == FLEXIBLE_PIP)
            SetDynamicScalerStep(gmvb_PipChannel, 1); // temporary for demo
         else
         SetDynamicScalerStep(gmvb_PipChannel, 1);
         SetDisplayPip();
      }
      if (B_Channel == CH_A)
         gm_AfmSetup(W_OverscanEnabled[CH_A]);
      //
      // Setup Dynamic scaling size end
      //
      SetDynamicScalerSizeEnd(B_Channel);

      //
      // Program Normal
      //
      if ((gm_IsValidSignal(B_Channel) == FALSE)
            || (B_DisableDynamicScale == TRUE)
#if (FEATURE_TWO_STAGE_PROCESSING_MODE == ENABLE)
            || gm_Is2StageEnabled()
#endif
            || (B_Method == DISPLAY_CHANGE_QUICK))
      {
#ifndef NO_PIP_SUPPORT      
         if(B_Channel == gmvb_PipChannel) //KevinL 20090318 Border for PIP only
         SetPipBorder(B_Channel, TRUE);
#endif
         SetDisplayOff(B_Channel, BACKGROUND_ONLY_NO_SYNC_UPDATE);
         gm_ScalerDynamicEnable(B_Channel, FALSE);
         gm_DisplaySetSize(B_Channel);
         gm_DisplaySetPosition(B_Channel);

#if (FEATURE_TWO_STAGE_PROCESSING_MODE == ENABLE)
         if ((UserPrefPipMode == NO_PIP) && (B_Channel == CH_A)
               && IsTwoStageAllowed())
         {
            if (!gm_Is2StageEnabled())
            {
               EnterTwoStageProcessing();
            }
         }
         else
#endif
         {
            if (gm_IsValidSignal(B_Channel) == FALSE)
            {
               //If output window size is zero (this should not happen but anways), set to
               //  some arbitrary number to avoid memory and output size to be set to zero
               if (gmvw_OutputHActive[B_Channel] == 0)
                  gmvw_OutputHActive[B_Channel] = 50;
               if (gmvw_OutputVActive[B_Channel] == 0)
                  gmvw_OutputVActive[B_Channel] = 50;

               stInput[B_Channel].Hactive = gmvw_OutputHActive[B_Channel];
               stInput[B_Channel].Vactive = gmvw_OutputVActive[B_Channel];
               gm_ScalerBypass(B_Channel);
            }
            else
            {
               gm_ScalerSetup(B_Channel);
            }

            gm_MemorySizeAdjust(B_Channel);
            gm_SyncUpdateWait(B_Channel, INPUT_OUTPUT_CLOCK);
         }

      }
      //
      // Program Dynamic Scaling
      //
      else
      {
         DynamicScalerStart(B_Channel);
         DynamicScalerProcess(FALSE);
         DynamicScalerStop();
      }
   }
	#if(IS_PROJ_4K2K())
	CheckScanlingFilterCtrl(B_Channel);
	#endif

   
   //
   // Disable TNR (already include TNR_PROGRESSIVE) bit
   // in case of Vertical Shrink
   // when mode is stable but changing active display
   //
   if (B_Channel == CH_A)
   {
      if (stInput[CH_A].Vactive > gmvw_OutputVActive[CH_A])
      {
         gm_VideoProcessingDisable(VIDEO_TNR);
      }
   }

   // Request Blenders ON and User Prefs
#if(SPEEDUP_MODE_CHANGE == ENABLE)
   		ModeSetupBlender(B_Channel, REQUEST_NOW);
#else
ModeSetupBlender(B_Channel, DISPLAY_DEFER_TMO_NORMAL);
#endif
}

//******************************************************************************
//
// FUNCTION       : void ModeSetupDisplayTiming(void)
//
// USAGE          : Request Handler for a change to the Display Timing.
//                  This handler can be called directly from the
//                  Application, or via the scheduler.
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : gmvb_MainChannel            (RO)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void ModeSetupDisplayTiming(void)
{
}

//******************************************************************************
//
// FUNCTION       : void ModeSetupUserPref(gmt_PHY_CH B_Channel)
//
// USAGE          :
//
// INPUT          : B_Channel - CH_A or CH_B
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
void ModeSetupUserPref(gmt_PHY_CH B_Channel)
{
   gmt_PHY_CH B_ColorChannel;

   // This line is needed to update gmvd_VideoProcessingDesired and gmvd_VideoProcessingEnabled
   //   when none of the following features are enabled.
   gm_VideoProcessingEnable(gmvd_VideoProcessingDesired);

   //
   // Main Channel Adjustments
   //
   if (B_Channel == CH_A)
   {
      //
      // Video User Prefs
      //
      if ((gm_IsVideo(CH_A) == TRUE) ||(gm_IsGraphics(CH_A) && gm_IsInterlaced(CH_A) ) )
      {
         // Disable whole videoprocessing before
#if (FEATURE_PIP_DCDI == ENABLE)
         gm_VideoProcessingDisable(VIDEO_PROCESS_ALL & (~VIDEO_DCDI_PIP));
#else
         gm_VideoProcessingDisable(VIDEO_PROCESS_ALL);
#endif

         //
         // Film Mode Detection
         //
         if (UserPrefFilmMode == FM_DISABLED)
         {
            gm_VideoProcessingDisable(VIDEO_FILMMODE); // 8668: also AFM_DETECT_EN = 0
            gW_DetectionMode = 5; // to force MADI processing
            gm_ClearRegBitsWord(MOTION_PROC_CTRL, AFM_DETECT_EN);
            gm_SyncUpdate(CH_A, INPUT_OUTPUT_CLOCK);
         }
         else
         {
            if ( gm_VideoProcessingEnable(VIDEO_FILMMODE) == OK ) // also AFM_DETECT_EN = 1
            {
               switch ( UserPrefFilmMode )
               {
                  case FM_VIDEO_32_22:
                     gW_DetectionMode = 0; // to force MADI processing
                     break;
                  case FM_VIDEO_32:
                     gW_DetectionMode = 3; // disable 2:2 video processing
                     break;
                  case FM_VIDEO_22:
                     gW_DetectionMode = 2; // disable 3:2 video processing
                     break;
                  default:
                     break;
               }
               //since we are forcing filmmode here, we need to update
               //  gmvd_VideoProcessingEnabled; gm_VideoProcessingEnable() does not
               //  guarantee the enabling of filmmode
               gmvd_VideoProcessingEnabled |= VIDEO_FILMMODE;
               gm_SetRegBitsWord(MOTION_PROC_CTRL, AFM_DETECT_EN);
               gm_SyncUpdate(CH_A, INPUT_OUTPUT_CLOCK);
            }
         }
         //
         // Film Display Mode
         //
#ifdef Action_AdjustThreeTwoFilmDisplayMode_WB_Used
         AdjustThreeTwoFilmDisplayMode();
#endif

         //
         // MADI
         //
         gm_VideoProcessingAdjustment(VIDEO_MADI, UserPrefMADI);
         //
         // DCDi
         //
         if ((gmvb_MainChannel == CH_A))
         {
            if (UserPrefDCDiMain == ENABLE)
               gm_VideoProcessingEnable(VIDEO_DCDI_MAIN);
            else
               gm_VideoProcessingDisable(VIDEO_DCDI_MAIN);
         }
         else
         {
            if (UserPrefDCDiPip == ENABLE)
               gm_VideoProcessingEnable(VIDEO_DCDI_MAIN);
            else
               gm_VideoProcessingDisable(VIDEO_DCDI_MAIN);
         }

#if (FEATURE_CCS == ENABLE)
         //
         // CCS
         //
         gm_VideoProcessingAdjustment(VIDEO_CCS, UserPrefCCS);
         if (IsCCSforHDDisabled())
            gm_VideoProcessingAdjustment(VIDEO_CCS, CCS_OFF);
#endif


         //
         // Temporal Noise Reduction
         //
         if ((UserPrefMADI == MADI_OFF) || (stInput[CH_A].Hactive > (1920 + 10)) || (stInput[CH_A].Vactive > (1080+10)))
            gm_VideoProcessingAdjustment(VIDEO_TNR, TNR_OFF);
         else
	{
		#ifdef EXTERNAL_DECODER
		if(IsVideoInput(UserPrefCurrentInputMain))
			gm_VideoProcessingAdjustment(VIDEO_TNR, UserPrefTNR);  
		else
		#endif
		{
			//Neil120727+ for color banding issue when switch 32 grays to 256 grays when TNR on.
			//if(IsPresetMode_DCR_ON())		//Hyb130429- 
				gm_VideoProcessingAdjustment(VIDEO_TNR, UserPrefTNR);  
		}
	}

         //
         // MPEG Noise Reduction
         //
         //if (UserPrefMPEGNRMode == MPEG_NR_OFF)
			if ((UserPrefMPEGNRMode == MPEG_NR_OFF) || (stInput[CH_A].Hactive > 1920) || (stInput[CH_A].Vactive > 1080))
         {
            UserPrefMPEGNRMode = MPEG_NR_OFF;
            gm_VideoProcessingDisable(VIDEO_MNR | VIDEO_GNR);
         }
         else
         {
            #if 1                                            
            gm_ClearAndSetBitsWord(MPEG_NR_CTRL0, MPEG_NR_GAMMA,(((WORD)(MSNQR_Ctrl3[MNRVQTable.MSQNRFilterStrength] >> 18)&0x3F) << 2));
            gm_WriteRegWord(MPEG_NR_CTRL1, ((WORD)MSNQR_Ctrl3[MNRVQTable.MSQNRFilterStrength]&0xFFF));
            gm_WriteRegWord(MPEG_NR_CTRL2, ((WORD)(MSNQR_Ctrl3[MNRVQTable.MSQNRFilterStrength] >> 12)&0x3F));
            #else
            gm_UpdateMpegNrThreshold(UserPrefMPEGNRThresh0,
                                            UserPrefMPEGNRThresh1,
                                            UserPrefMPEGNRThresh2);
            
            #endif

            if (UserPrefMPEGNRMode == MPEG_NR_MOSQ_GAUSS)
            {
               gm_VideoProcessingEnable(VIDEO_MNR | VIDEO_GNR);
            }
            else if (UserPrefMPEGNRMode == MPEG_NR_MOSQ)
            {
               gm_VideoProcessingEnable(VIDEO_MNR);
               gm_VideoProcessingDisable(VIDEO_GNR);
            }
            else
            {
               gm_VideoProcessingEnable(VIDEO_GNR);
               gm_VideoProcessingDisable(VIDEO_MNR);
            }
         }

#if (FEATURE_VIDEO_BLUESTRETCH == ENABLE)
         //
         // Blue stretch
         //
         if (UserPrefBlueStretch != BLUE_STRETCH_OFF)
         {
            gm_VideoProcessingEnable(VIDEO_BLUESTRETCH);
         }
         else
         {
            gm_VideoProcessingDisable(VIDEO_BLUESTRETCH);
         }
#endif
#if (FEATURE_VIDEO_FLESHTONE == ENABLE)
         //
         // Fleshtone
         //
#ifdef Action_AdjustNonACMFleshtoneCorrection_WB_Used
         if ((UserPrefNonACMFleshtoneCorrection == ENABLE) ||
               (UserPrefFleshToneComp != FT_OFF))
#else
         if (UserPrefFleshToneComp != FT_OFF)
#endif
         {
            gm_VideoProcessingEnable(VIDEO_FLESHTONE);
         }
         else
         {
            gm_VideoProcessingDisable(VIDEO_FLESHTONE);
         }
#endif
#if (FEATURE_VIDEO_SHARPNOISE == REV2)
         //
         // Sharpness Noise Coring
         //
         gm_VideoProcessingAdjustment(VIDEO_SHARPNOISE, UserPrefSharpNoise);
#endif
      }
      //
      // Non - Video
      //
      else
      {
         //
         // Turn OFF
         //
#if (FEATURE_PIP_DCDI == ENABLE)
         gm_VideoProcessingDisable(VIDEO_PROCESS_ALL & (~VIDEO_DCDI_PIP));
#else
         gm_VideoProcessingDisable(VIDEO_PROCESS_ALL);
#endif
      }
   }
   //
   // PIP Channel Adjustments
   //
   else
   {
#if (FEATURE_PIP_DCDI == ENABLE)
#if defined (UserPrefDCDiPip)
      //
      // PIP DCDi
      //
      if (gm_IsVideo(CH_B) == TRUE)
      {
         if ((CH_B == gmvb_PipChannel))
         {
            if (UserPrefDCDiPip == ENABLE)
               gm_VideoProcessingEnable(VIDEO_DCDI_PIP);
            else
               gm_VideoProcessingDisable(VIDEO_DCDI_PIP);
         }
         else
         {
            if (UserPrefDCDiMain == ENABLE)
               gm_VideoProcessingEnable(VIDEO_DCDI_PIP);
            else
               gm_VideoProcessingDisable(VIDEO_DCDI_PIP);
         }

      }
      else
      {
         gm_VideoProcessingDisable(VIDEO_DCDI_PIP);
      }
#endif
#endif
   }

   // if processing channel is not connected to ACC/ACM, exit
#if (FEATURE_CROSS_POINT_SWITCH == ENABLE)
#ifdef UserPrefColorResourceRouting
   if (!(((B_Channel == CH_A) && (UserPrefColorResourceRouting == 0))
         ||
         ((B_Channel == CH_B) && (UserPrefColorResourceRouting == 1))))
      return;
#endif
#else
   if (B_Channel != CH_A)
      return;
#endif

   // 20081118 #1, allow all input support ACC ACM
   {
      //
      // Dynamic Contrast feature (ACC)
      //
#if (!defined(ACC_LUT_METHOD))      
	#ifdef DELL_U2410F//120208 Edward ACC/ACM depend on OSD setting copy from 2410
	  if (IsAcmEnablePresetMode())
	#else  	
      if (UserPrefImageScheme)
	#endif  	
      {
		#if (FEATURE_ACC3 == ENABLE)    
			#ifdef DELL_U2410F//120208 Edward ACC/ACM depend on OSD setting copy from 2410
				AdjustDELLACCScheme();
			#else
            	gm_UpdateAcc(UserPrefImageScheme);
			#endif
            gm_VideoProcessingEnable(VIDEO_ACC);
            gm_Printf(" Enable ACC in Modesetupuserpref %d", UserPrefImageScheme);
		#endif
      }
      else
      {
         gm_VideoProcessingDisable(VIDEO_ACC);
      }
#endif         
      //
      // ACM - based on fleshtone and color adjustments
      //
      if (IsAcmUsed())
      {
         gm_VideoProcessingEnable(VIDEO_ACM);
      }
      else
      {
         gm_VideoProcessingDisable(VIDEO_ACM);
      }
   }
}

//******************************************************************************
//
// FUNCTION       : void ModeSetupBlender(gmt_PHY_CH B_Channel, WORD W_Time)
//
// USAGE          : Wrapper for Blender request, because BlenderPending
//                  must be updated whenever a Blender request is made.
//                  Use this function instead of Posting a REQ_MS_BLENDER
//                  directly.
//
// INPUT          : B_Channel - CH_A or CH_B
//                  W_Time - ms til Blender ON
//
// OUTPUT         : None
//
// GLOBALS        : D_BlenderPending            (WO)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void ModeSetupBlender(gmt_PHY_CH B_Channel, WORD W_Time)
{
   // Prevent multiple REQ_MS_BLENDER requests by always using only the latest request;
   //   otherwise, blender may prematurely open
   ModeSetupRequestCancel(B_Channel, REQ_MS_BLENDER, PARAM_ANY);
   ModeSetupRequest(B_Channel, REQ_MS_BLENDER, PARAM_NONE, W_Time);

   D_BlenderPending[B_Channel] = gm_GetSystemTime();
}

//******************************************************************************
//
// FUNCTION       : void ModeSetupAutoAdjust(gmt_PHY_CH B_Channel)
//
// USAGE          : Request Handler for an Auto Adjust. This handler can be
//                  called directly from the Application, or via the
//                  scheduler.
//
// INPUT          : B_Channel - CH_A or CH_B
//
// OUTPUT         : None
//
// GLOBALS        : gmvb_MainChannel      (RO)
//                  gmvw_InputHTotal            (RO)
//                  gmvw_InputHStart            (RO)
//                  gmvw_InputVStart            (RO)
//                  gmvb_InputSamplingPhase      (RO)
//         UserPrefCurrentInputMain      (RO)
//         UserPrefCurrentInputPip      (RO)
//         gmvsw_InputHSyncOffset      (RW)
//         gmvsw_InputVSyncOffset      (RW)
//         gmvb_InputSamplingPhase      (RW)
//                  B_AutoAdjustCount         (WO)
//                  UserPrefHTotal            (WO)
//                  UserPrefHStart            (WO)
//                  UserPrefVStart            (WO)
//                  UserPrefHSyncPhase         (WO)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
#if FEATURE_AIP == ENABLE

#if (FEATURE_AA_ENHANCED == ENABLE)
extern void gm_AutoPhase(gmt_PHY_CH B_Channel);
#endif //(FEATURE_AA_ENHANCED == ENABLE)
void ModeSetupAutoAdjust(gmt_PHY_CH B_Channel)
{
   gmt_IA_RET_STAT IaRetStatus;
   ModeDependentLookUpType LookUpType;

   if (AfeGetChannelPortProc(B_Channel) != AIP)
   {
      OSDRequestMsg(B_Channel, MTO_AutoAdjustFailed, REQUEST_NOW);
      return;
   }

   if (gm_IsGraphics(B_Channel) == FALSE)
   {
      if (stInput[B_Channel].Htotal != gmvw_InputHTotalOrg[B_Channel])
      {
         gm_SetupHTotal(gmvw_InputHTotalOrg[B_Channel], B_Channel);
         stInput[B_Channel].Htotal = gmvw_InputHTotalOrg[B_Channel];
         UserPrefHTotal = gmvw_InputHTotalOrg[B_Channel];
      }
      gm_AutoPhase(B_Channel);
      gmvb_InputSamplingPhase[B_Channel] = (gm_ReadRegByte((SDDS1_CONTROL + 2)) & 0x3F);
      UserPrefHSyncPhase = gmvb_InputSamplingPhase[B_Channel];
      // Save Updated Values to NVRAM
      SaveModeDependentSettings(B_Channel, &LookUpType);
      OSDRequestMsg(B_Channel, MTO_AutoAdjustCompleted, REQUEST_NOW);
      return;
   }
   
#if (FEATURE_ADC_AOC == ENABLE)
   // disable AOC during autoadjust
   gm_AOCEnable(gmd_ADC_0, FALSE);
#endif

   // Reset Manual Corrections
   gmvsw_InputHSyncOffset[B_Channel] = 0;
   gmvsw_InputVSyncOffset[B_Channel] = 0;
   UserPrefHSyncDelay = gmvsw_InputHSyncOffset[B_Channel];
   UserPrefVSyncDelay = gmvsw_InputVSyncOffset[B_Channel];
   UserPrefHStart = gmvw_InputHStartOrg[B_Channel]+9;
   UserPrefVStart = gmvw_InputVStartOrg[B_Channel];
   UserPrefHSyncPhase = gmvb_InputSamplingPhase[B_Channel];
   gm_Print("*****************************************",0);
   gm_Print("Reset Manual Corrections UserPrefHSyncDelay %d",UserPrefHSyncDelay);
   gm_Print("Reset Manual Corrections UserPrefVSyncDelay %d",UserPrefVSyncDelay);
   gm_Print("Reset Manual Corrections UserPrefHStart %d",UserPrefHStart);
   gm_Print("Reset Manual Corrections UserPrefVStart %d",UserPrefVStart);
   gm_Print("Reset Manual Corrections UserPrefHSyncPhase %d",UserPrefHSyncPhase);
   gm_Print("*****************************************",0);
   gm_InputSetHPosition(B_Channel);
   gm_InputSetVPosition(B_Channel);

   // Perform Adjustment
   B_AutoAdjustStatus[B_Channel] = AA_START;            // AA started

#if (FEATURE_AA_ENHANCED == ENABLE)
   if (IsEstimatedMode(B_Channel))
   {
      BOOL RetAutoPos;
      
      gm_Print("Estimated Mode .... AutoPosition & AutoPhase Only!",0);
      RetAutoPos = gm_AutoPosition(B_Channel);
      gm_AutoPhase(B_Channel);
      //gm_AutoGeometry( B_Channel, gmd_FORCE_WIDTH);
      
      if (RetAutoPos == TRUE)
      IaRetStatus = IA_RET_OK;
      else
         IaRetStatus = IA_RET_FAIL;
   }
   else
#endif //(FEATURE_AA_ENHANCED == ENABLE)
      IaRetStatus = gm_AutoGeometry( B_Channel, gmd_DEFAULT_AUTO);

   if ((B_AutoAdjustStatus[B_Channel] == AA_ABORT) || (IaRetStatus != IA_RET_OK))
   {
      // Tell OSD AutoAdjust is failed
      OSDRequestMsg(B_Channel, MTO_AutoAdjustFailed, REQUEST_NOW);

      //SQA Automation Gprobe message -> related to SQA_DebugHandler.c
#ifdef SQA_AUTOMATION_DEBUGMODE
      SQA_ModeDetectVGA_AutoAdjust(FALSE);
#endif
   }
   else
   {
      // Read Results back into Globals
      stInput[B_Channel].Htotal = gm_ReadRegWord(SDDS1_HTOTAL);
      gmvb_InputSamplingPhase[B_Channel] = (gm_ReadRegByte((SDDS1_CONTROL + 2)) & 0x3F);

      if (B_Channel == CH_A)
      {
         stInput[B_Channel].Hstart = gm_ReadRegWord(IMP_HS_DELAY) + gm_ReadRegWord(IMP_H_ACT_START);
         stInput[B_Channel].Vstart = gm_ReadRegWord(IMP_VS_DELAY) + gm_ReadRegWord(IMP_V_ACT_START_EVEN);
      }
      else if (B_Channel == CH_B)
      {
         stInput[B_Channel].Hstart = gm_ReadRegWord(IPP_HS_DELAY) + gm_ReadRegWord(IPP_H_ACT_START);
         stInput[B_Channel].Vstart = gm_ReadRegWord(IPP_VS_DELAY) + gm_ReadRegWord(IPP_V_ACT_START_EVEN);
      }

      if (gm_IsInputTripleADC(B_Channel) && gm_IsGraphics(B_Channel))
      {
         if (gmvsw_InputVSyncOffset[B_Channel] < 0)
         {
            if (B_Channel == CH_A)
               stInput[B_Channel].Vstart -= (gm_ReadRegWord(IMP_VS_DELAY) & 0xFFFE);
            else if (B_Channel == CH_B)
               stInput[B_Channel].Vstart -= (gm_ReadRegWord(IPP_VS_DELAY) & 0xFFFE);
         }
      }
      // Write results back to Original - overwriting table setting
      //SaveOriginalValue(B_Channel);

      // Copy Globals into UserPrefs
      UserPrefHStart = stInput[B_Channel].Hstart;
      UserPrefVStart = stInput[B_Channel].Vstart;
      UserPrefHTotal = stInput[B_Channel].Htotal;
      UserPrefHSyncPhase = gmvb_InputSamplingPhase[B_Channel];
      UserPrefHSyncDelay = gmvsw_InputHSyncOffset[B_Channel];
      UserPrefVSyncDelay = gmvsw_InputVSyncOffset[B_Channel];
	  gm_Print("*****************************************",0);
	  gm_Print("Copy Globals into UserPrefs UserPrefHStart %d",UserPrefHStart);
	  gm_Print("Copy Globals into UserPrefs UserPrefVStart %d",UserPrefVStart);
	  gm_Print("Copy Globals into UserPrefs UserPrefHTotal %d",UserPrefHTotal);
	  gm_Print("Copy Globals into UserPrefs UserPrefHSyncPhase %d",UserPrefHSyncPhase);
	  gm_Print("Copy Globals into UserPrefs UserPrefHSyncDelay %d",UserPrefHSyncDelay);
	  gm_Print("Copy Globals into UserPrefs UserPrefVSyncDelay %d",UserPrefVSyncDelay);
	  gm_Print("*****************************************",0);

      LookUpType.W_Hfreq = stInput[B_Channel].Hfrq;
      LookUpType.W_Vfreq = stInput[B_Channel].Vfrq;
      LookUpType.W_Vtotal = stInput[B_Channel].Vtotal;

      // Save Updated Values to NVRAM
      SaveModeDependentSettings(B_Channel, &LookUpType);

      // Tell OSD AutoAdjust is Complete
      OSDRequestMsg(B_Channel, MTO_AutoAdjustCompleted, REQUEST_NOW);

      //SQA Automation Gprobe message -> related to SQA_DebugHandler.c
#ifdef SQA_AUTOMATION_DEBUGMODE
      SQA_ModeDetectVGA_AutoAdjust(TRUE);
#endif
   }
   B_AutoAdjustStatus[B_Channel] = AA_IDLE;            // AA end
	if(gm_ReadRegWord(ADC_CONTROL0)&INST_AUTO_CH_SEL )		//Hyb130503+ for auto finish need clear this bits;
		gm_ClearRegBitsWord(ADC_CONTROL0,INST_AUTO_CH_SEL);
   //
   // Since AA takes long time and need to
   // clear OSD key queue after AutoAdjust
   //
   FlushOsdKeyQueue();
   B_AutoAdjustStarted[B_Channel] = FALSE;

#if (FEATURE_ADC_AOC == ENABLE)
   // Enable AOC
   if ((AfeGetChannelPortProc(B_Channel) == AIP) && (gmvb_CurrentSyncState[B_Channel] == DSS_STATE))
   {
      RetrieveAdcCalibrationValues(B_Channel);
      gm_AOCEnable(gmd_ADC_0, TRUE);
   }
#endif

}

#endif // FEATURE_AIP == ENABLE

//******************************************************************************
//
// FUNCTION       : void SetDisplayOn(gmt_PHY_CH B_Channel)
//
// USAGE          : Call to enable the display of MAIN or PIP channel by
//                  enabling blender and disable the force background method.
//
// INPUT          : B_Channel - CH_A or CH_B
//
// OUTPUT         : None
//
// GLOBALS        : gmvb_MainChannel         (RO)
//                  gmvb_PipChannel            (RO)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
extern BOOL Into_Pwr_Transition;
void SetDisplayOn(gmt_PHY_CH B_Channel)
{
#ifdef ST_4K2K_93xx_BOARD
	BOOL B_UpdateComplete = FALSE;	
	#if ENABLE_ATHENA_R_L_IMAGE_SYNC_HANDLER
		UNUSED_PARAMETER(B_Channel);
	#endif
#else
	BOOL B_UpdateComplete = FALSE;
#endif

	 //
	// Enable blender to turn on display
	//
#ifdef ST_4K2K_93xx_BOARD
#if (!ENABLE_ATHENA_R_L_IMAGE_SYNC_HANDLER)
	if(GetChipID_4K2K()!=CHIPID_4K2K_R)  
	{  // For STD / Athena_FE / L
   	if (!B_UpdateComplete)
   	{
	  gm_DisplayForceBkgnd(B_Channel, !gm_IsValidSignal(B_Channel), (B_Channel == gmvb_MainChannel)?
						   RGB_565(MAIN_BACKGROUND_COLOR):RGB_565(PIP_BACKGROUND_COLOR));
	  gm_BlenderOnOff(B_Channel, On);
	  gm_SyncUpdateWait(B_Channel,INPUT_OUTPUT_CLOCK);
	  D_BlenderPending[B_Channel] = 0;
   	}
   	debug_ist_stop(B_Channel);
	}
#else
	if(GetChipID_4K2K() == CHIPID_4K2K_FE)
	{
   	if (!B_UpdateComplete)
   	{
	  gm_DisplayForceBkgnd(B_Channel, !gm_IsValidSignal(B_Channel), (B_Channel == gmvb_MainChannel)?
						   RGB_565(MAIN_BACKGROUND_COLOR):RGB_565(PIP_BACKGROUND_COLOR));
	  gm_BlenderOnOff(B_Channel, On);
	  gm_SyncUpdateWait(B_Channel,INPUT_OUTPUT_CLOCK);
	  D_BlenderPending[B_Channel] = 0;
   	}
   	debug_ist_stop(B_Channel);
	}

	// test 20130411
	if(GetChipID_4K2K()==CHIPID_4K2K_R)
	{
		if(Into_Pwr_Transition == TRUE)
		{
         KeypadLightControl(KEYPAD_LED_GREEN, LED_NORMAL);
			Into_Pwr_Transition = FALSE;
			gm_Print("Panel power up from Pwr_Transition", 0);
			PanelPowerOn4k2k();
		}
	}


#endif
#else //Original
{  // For STD / Athena_FE / L
   if (!B_UpdateComplete)
   {
	  gm_DisplayForceBkgnd(B_Channel, !gm_IsValidSignal(B_Channel), (B_Channel == gmvb_MainChannel)?
						   RGB_565(MAIN_BACKGROUND_COLOR):RGB_565(PIP_BACKGROUND_COLOR));
	  gm_BlenderOnOff(B_Channel, On);
	  gm_SyncUpdateWait(B_Channel,INPUT_OUTPUT_CLOCK);
	  D_BlenderPending[B_Channel] = 0;
   }
   debug_ist_stop(B_Channel);
}
#endif
}

//20120608 Copy from library by KevinChen. Replace MAIN_AUTO_BKGND_COLOR,PIP_AUTO_BKGND_COLOR by PB_BKGND
gmt_RET_STAT gm_DisplayForceBkgnd_Customer (gmt_PHY_CH B_Channel, BYTE B_Enable, WORD W_BkgndColor)
{
   //
   // 1) Select the channel for which force of background color is to be
   //      enabled/disabled
   // 2) Select whether to enable or disable the forcing of background color
   // 3) Select the color (RGB565 format) of the backgound to which it has to
   // 	    be forced
   //
   if (B_Channel == CH_A)
   {
      if (B_Enable == TRUE)
      {
         // Force back ground seems must disable PB enable first.
         gm_SetRegBitsWord(PB_CTRL, PB_FORCE_MAIN_BKGND);
         //gm_ClearAndSetBitsWord(PB_CTRL, PB_MAIN_ENABLE, PB_FORCE_MAIN_BKGND);
         gm_WriteRegWord(PB_BKGND, W_BkgndColor);
      }
      else
      {
         gm_ClearRegBitsWord(PB_CTRL, PB_FORCE_MAIN_BKGND);
      }
   }
   else if (B_Channel == CH_B)
   {
      if (B_Enable == TRUE)
      {
         gm_SetRegBitsWord(PB_CTRL, PB_FORCE_PIP_BKGND);
         //gm_ClearAndSetBitsWord(PB_CTRL, PB_PIP_ENABLE, PB_FORCE_PIP_BKGND);
         gm_WriteRegWord(PB_BKGND, W_BkgndColor);
      }
      else
      {
         gm_ClearRegBitsWord(PB_CTRL, PB_FORCE_PIP_BKGND);
      }
   }
   else
      return (ERR_CHANNEL);

   gm_SyncUpdate(B_Channel, OUTPUT_CLOCK);	// Panoramic only applies to MAIN channel
   return (OK);
}
#ifdef ST_4K2K_93xx_BOARD //Add by ChengLin 20130408
#if ENABLE_ATHENA_R_L_IMAGE_SYNC_HANDLER
//******************************************************************************
// FUNCTION:   Set4K2K_ImageSyncReady
// USAGE: Set Athena Left/Right Image Ready GPIO pin to "High" level
//
// INPUT:  None
// OUTPUT: None
// GLOBALS:
//******************************************************************************
void Set4K2K_ImageSyncReady(void)
{
#if ENABLE_ATHENA_R_L_IMAGE_SYNC_HANDLER
	if(GetChipID_4K2K() != CHIPID_4K2K_FE)
	{
	/*ChengLin 20130221: New Image Sync Handler*/
	if(ImageSyncHandleGetState() != IMAGE_LVDS_ON_STATE)
	{
		if(ImageSyncHandleGetModeStableFlag() == TRUE) //ChengLin 20130225: Before set Ready bit, double check stable Flag first for AC power on garbage issue 
			Image_ReadyFlag(TRUE);				
	}
	ImageSyncHandler_msg("Set Athena Right and Left to ready state.", 0);
	}
#else //Original  
	Image_ReadyFlag(TRUE);
	if(GetChipID_4K2K()==CHIPID_4K2K_R)
	{
		return;
	}
	else if(GetChipID_4K2K()==CHIPID_4K2K_L)
	{
		gm_Delay10ms(50);
		Image_output_Request();
	}
#endif   
}
#endif
#endif

//******************************************************************************
//
// FUNCTION       : void SetDisplayOff(gmt_PHY_CH B_Channel, t_DISP_OFF_MODE B_Mode)
//
// USAGE          : Call to disable the display of MAIN or PIP channel by
//                  disabling blender or enable the force background method.
//
// INPUT          : B_Channel - CH_A or CH_B
//                  B_Mode - BACKGROUND_ONLY
//                     BLENDER_OFF
//
// OUTPUT         : None
//
// GLOBALS        : gmvb_MainChannel         (RO)
//                  gmvb_PipChannel            (RO)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void SetDisplayOff(gmt_PHY_CH B_Channel, t_DISP_OFF_MODE B_Mode)
{
#ifdef BLANK_DISPLAY_MUTE_ON
   if ((B_Mode == BACKGROUND_ONLY)||(B_Mode == BACKGROUND_ONLY_NO_SYNC_UPDATE))
   {
	  WORD BKG_Color;
	  if(IsSplashScreenCompleted())	  	
	 	 BKG_Color = (B_Channel == gmvb_MainChannel) ? RGB_565(MAIN_BACKGROUND_COLOR): RGB_565(PIP_BACKGROUND_COLOR);
	  else
	 	 BKG_Color = (B_Channel == gmvb_MainChannel) ? RGB_565(SPLASHSCREEN_BKG_COLOR): RGB_565(PIP_BACKGROUND_COLOR);
	  	
      gm_DisplayForceBkgnd_Customer(B_Channel, TRUE, BKG_Color);

	  //Hyb131106+ for force background to color full when no sync
	  if(B_Channel == CH_A)
	  	gm_BlenderOnOff(B_Channel, Off);	 
   }
   else
	{
	   WORD BKG_Color;
	   if(IsSplashScreenCompleted())	 
		  BKG_Color = (B_Channel == gmvb_MainChannel) ? RGB_565(MAIN_BACKGROUND_COLOR): RGB_565(PIP_BACKGROUND_COLOR);
	   else
		  BKG_Color = (B_Channel == gmvb_MainChannel) ? RGB_565(SPLASHSCREEN_BKG_COLOR): RGB_565(PIP_BACKGROUND_COLOR);
		 
	   gm_DisplayForceBkgnd_Customer(B_Channel, TRUE, BKG_Color);
	   gm_BlenderOnOff(B_Channel, Off);   
   	}
   
   if (B_Mode != BACKGROUND_ONLY_NO_SYNC_UPDATE)
      gm_SyncUpdateWait(B_Channel,INPUT_OUTPUT_CLOCK);
#else
   B_Mode++;                        // Dummy to aviod warning
#endif
   D_BlenderPending[B_Channel] = 0;
}

//******************************************************************************
//
// FUNCTION       : void UpdateFilters(gmt_PHY_CH B_Channel)
//
// USAGE          : This is a static function called by ModeSetup to load
//                  the proper horizontal and vertical coefficients whenever
//                  a new mode is setup.  It applies only to the main
//                  channel when a video input is active (FLI8532) or for both Main and Pip
//                  Video or Graphics input is active (FLI8668).
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : gmvb_MainChannel            (RO)
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void UpdateFilters(gmt_PHY_CH B_Channel)
#if 1
{
#if (FEATURE_LOAD_RGB_COEFF == ENABLE)
   BYTE B_Sharpness_CH_A, B_Sharpness_CH_B;

   if (gm_TimingIsStable(gmvb_MainChannel) == TRUE)
  	B_Sharpness_CH_A= UserPrefPortSharpness;

   if ((gm_TimingIsStable(gmvb_PipChannel) == TRUE) && (UserPrefPipMode != NO_PIP))
   {
      if (gm_IsVideo(gmvb_PipChannel))
      {
         if (gmvb_PipChannel == CH_A)
            B_Sharpness_CH_A = UserPrefSharpnessPip;
         else
            B_Sharpness_CH_B = UserPrefSharpnessPip;
      }
      else
      {
         if (gmvb_PipChannel == CH_A)
            B_Sharpness_CH_A = UserPrefSharpnessRGBpip;
         else
            B_Sharpness_CH_B = UserPrefSharpnessRGBpip;
      }
   }

   //
   // Channel A
   //
   //if (B_Update_CH_A == TRUE)
   if(B_Channel == CH_A)	//Hyb130429* 
   {
      gm_SetScalerSharpness(CH_A, B_Sharpness_CH_A);
#if (FEATURE_SHARPNESS == REV6)
      if (gm_IsVideo(CH_A))
      {
         gm_SetRegBitsWord(MVF_CTRL, MVF_SHARP_DIR);
         gm_ClearRegBitsWord(MVF_CTRL, (MVF_INT_COEF_EN | MVF_DITHER_EN)); // External coeff for MVF
      }
      else
      {
         gm_SetRegBitsWord(MVF_CTRL, MVF_DITHER_EN);
         gm_ClearRegBitsWord(MVF_CTRL, (MVF_INT_COEF_EN | MVF_SHARP_DIR)); // External coeff for MVF
      }
      gm_ClearRegBitsWord(MHF_CTRL, MHF_INT_COEF_EN ); // External coeff for MHF
      
      if ((gm_IsVideo(CH_A))&& IsColorSpace_GRAPHICS_SRGB)  // 20121026 Torisa 
	  	gm_SetRegBitsWord(MVF_CTRL, MVF_INT_COEF_EN);   // Use internal coefficients	   
   #else
      gm_SetRegBitsWord(MHF_CTRL, MHF_INT_COEF_EN | MHF_DITHER_EN);
      gm_SetRegBitsWord(MVF_CTRL, MVF_INT_COEF_EN | MVF_DITHER_EN);
#endif

      gm_SyncUpdate(CH_A, INPUT_OUTPUT_CLOCK);
   }

   //
   // Channel B
   //
   //if (B_Update_CH_B == TRUE)
   if((B_Channel == CH_B)&&(UserPrefPipMode != NO_PIP))
   {
      gm_SetScalerSharpness(CH_B, B_Sharpness_CH_B);
#if (FEATURE_SHARPNESS == REV6)
      if (gm_IsVideo(CH_B))
      {
         gm_SetRegBitsWord(PVF_CTRL, PVF_SHARP_DIR);
         gm_ClearRegBitsWord(PVF_CTRL, (PVF_INT_COEF_EN | PVF_DITHER_EN)); // External coeff for PVF
      }
      else
      {
         gm_SetRegBitsWord(PVF_CTRL, PVF_DITHER_EN);
         gm_ClearRegBitsWord(PVF_CTRL, (PVF_INT_COEF_EN | PVF_SHARP_DIR)); // External coeff for PVF
      }
      gm_ClearRegBitsWord(PHF_CTRL, PHF_INT_COEF_EN ); // External coeff for PHF
#else
      gm_SetRegBitsWord(PHF_CTRL, PHF_INT_COEF_EN );  // Internal coeff for PHF
      gm_SetRegBitsWord(PVF_CTRL, PVF_INT_COEF_EN );  // Internal coeff for PVF
#endif
      gm_SyncUpdate(CH_B, INPUT_OUTPUT_CLOCK);
   }

#endif
}

#else
void UpdateFilters(void)
{
#if (FEATURE_LOAD_RGB_COEFF == ENABLE)
   BOOL B_Update_CH_A, B_Update_CH_B;
   BYTE B_Sharpness_CH_A, B_Sharpness_CH_B;

   //
   // Decide which filters need updating and with what values
   //
   B_Update_CH_A = FALSE;
   B_Update_CH_B = FALSE;

   if (gm_TimingIsStable(gmvb_MainChannel) == TRUE)
   {
      if (gmvb_MainChannel == CH_A)
         B_Update_CH_A = TRUE;
      else
         B_Update_CH_B = TRUE;
      if (gm_IsVideo(gmvb_MainChannel))
      {
         if (gmvb_MainChannel == CH_A)
            B_Sharpness_CH_A = UserPrefSharpness;
         else
            B_Sharpness_CH_B = UserPrefSharpness;
      }
      else
      {
         if (gmvb_MainChannel == CH_A)
            B_Sharpness_CH_A = UserPrefSharpnessRGB;
         else
            B_Sharpness_CH_B = UserPrefSharpnessRGB;
      }
   }

   if ((gm_TimingIsStable(gmvb_PipChannel) == TRUE) && (UserPrefPipMode != NO_PIP))
   {
      if (gmvb_PipChannel == CH_A)
         B_Update_CH_A = TRUE;
      else
         B_Update_CH_B = TRUE;
      if (gm_IsVideo(gmvb_PipChannel))
      {
         if (gmvb_PipChannel == CH_A)
            B_Sharpness_CH_A = UserPrefSharpnessPip;
         else
            B_Sharpness_CH_B = UserPrefSharpnessPip;
      }
      else
      {
         if (gmvb_PipChannel == CH_A)
            B_Sharpness_CH_A = UserPrefSharpnessRGBpip;
         else
            B_Sharpness_CH_B = UserPrefSharpnessRGBpip;
      }
   }

   //
   // Channel A
   //
   //if (B_Update_CH_A == TRUE)
   if(B_Channel == CH_A)	//Hyb130429* 
   {
      gm_SetScalerSharpness(CH_A, B_Sharpness_CH_A);
#if (FEATURE_SHARPNESS == REV6)
      if (gm_IsVideo(CH_A))
      {
         gm_SetRegBitsWord(MVF_CTRL, MVF_SHARP_DIR);
         gm_ClearRegBitsWord(MVF_CTRL, (MVF_INT_COEF_EN | MVF_DITHER_EN)); // External coeff for MVF
      }
      else
      {
         gm_SetRegBitsWord(MVF_CTRL, MVF_DITHER_EN);
         gm_ClearRegBitsWord(MVF_CTRL, (MVF_INT_COEF_EN | MVF_SHARP_DIR)); // External coeff for MVF
      }
      gm_ClearRegBitsWord(MHF_CTRL, MHF_INT_COEF_EN ); // External coeff for MHF
      
      if ((gm_IsVideo(CH_A))&& IsColorSpace_GRAPHICS_SRGB)  // 20121026 Torisa 
	  	gm_SetRegBitsWord(MVF_CTRL, MVF_INT_COEF_EN);   // Use internal coefficients	   
   #else
      gm_SetRegBitsWord(MHF_CTRL, MHF_INT_COEF_EN | MHF_DITHER_EN);
      gm_SetRegBitsWord(MVF_CTRL, MVF_INT_COEF_EN | MVF_DITHER_EN);
#endif

      gm_SyncUpdate(CH_A, INPUT_OUTPUT_CLOCK);
   }

   //
   // Channel B
   //
   if (B_Update_CH_B == TRUE)
   {
      gm_SetScalerSharpness(CH_B, B_Sharpness_CH_B);
#if (FEATURE_SHARPNESS == REV6)
      if (gm_IsVideo(CH_B))
      {
         gm_SetRegBitsWord(PVF_CTRL, PVF_SHARP_DIR);
         gm_ClearRegBitsWord(PVF_CTRL, (PVF_INT_COEF_EN | PVF_DITHER_EN)); // External coeff for PVF
      }
      else
      {
         gm_SetRegBitsWord(PVF_CTRL, PVF_DITHER_EN);
         gm_ClearRegBitsWord(PVF_CTRL, (PVF_INT_COEF_EN | PVF_SHARP_DIR)); // External coeff for PVF
      }
      gm_ClearRegBitsWord(PHF_CTRL, PHF_INT_COEF_EN ); // External coeff for PHF
#else
      gm_SetRegBitsWord(PHF_CTRL, PHF_INT_COEF_EN );  // Internal coeff for PHF
      gm_SetRegBitsWord(PVF_CTRL, PVF_INT_COEF_EN );  // Internal coeff for PVF
#endif
      gm_SyncUpdate(CH_B, INPUT_OUTPUT_CLOCK);
   }

#endif
}
#endif
//******************************************************************************
//
// FUNCTION       : static void   RetrieveAdcCalibrationValues(gmt_PHY_CH B_Channel)
//
// USAGE          : Writes adc calibration values (reads them from
//                  NVRAM) for PAL    composite input or svideo,
//                  System should be previously calibrated.
//
// INPUT          : B_Channel - CH_A or CH_B
//
// OUTPUT         : None
//
// GLOBALS        : gmvb_MainChannel            (RO)
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
#if FEATURE_AIP == ENABLE
void RetrieveAdcCalibrationValues(gmt_PHY_CH B_Channel)
{
   BOOL status = FALSE;
   gmt_ADC_SETTING AdcSettings;
   BYTE B_InputConnector;

   //
   // Check which channel we are workin on
   //
   if (B_Channel == gmvb_MainChannel)
   {
      B_InputConnector = UserPrefCurrentInputMain;
   }
   else
   {
      B_InputConnector = UserPrefCurrentInputPip;
   }
   //
   // Check if we use external, or decoder not used then exit
   //
   if ((GetPortProcType(B_InputConnector) == EXTERNAL))
      return;

#ifdef CHANNEL_CLONE
   // In cloning, ADC values were already set previously by other channel.
   if ((UserPrefCurrentInputMain == UserPrefCurrentInputPip) &&
         (gm_IsModeComplete(gm_OtherChannel(B_Channel))))
   {
      return;
   }
#endif

   //
   // Determine which input type we have
   // Read appropriate NVRAM location
   //
   switch (GetPortConnType(B_InputConnector))
   {
      case COMPOSITE:
      case TUNER:
      case SVIDEO:
         break;

      case COMPONENT:
         status = ReadAdcSettings(gmd_ADC_0, (BYTE*)&AdcSettings, ADC_SETTING_HD);
         break;
      case VGA:
         if (gm_IsGraphics(B_Channel))
            //
            // Reload adc calibration registers
            // (This prerequisite for Enable AOC in mode setup)
            //
            status = ReadAdcSettings(gmd_ADC_0, (BYTE*)&AdcSettings, ADC_SETTING_GRAPHIC);
         else
#ifdef ALLOW_COMP_CALIB_ON_VGA
            status = ReadAdcSettings(gmd_ADC_0, (BYTE*)&AdcSettings, ADC_SETTING_HD2);
#else
            status = ReadAdcSettings(gmd_ADC_0, (BYTE*)&AdcSettings, ADC_SETTING_HD);
#endif
         break;
   }

   //
   // Update ADC settings
   //
   if (status == TRUE)
      gm_SetAdcGainOffset(gmd_ADC_0, &AdcSettings);

   //
   // if we have scart overlay2 we need to program adc C same as graphics.
   //
   if ((((GetPortSignalType(gmvb_InputConnector[B_Channel]) == SIG_OVL_TYPE2)
         || (GetPortSignalType(gmvb_InputConnector[B_Channel] + 2) == SIG_OVL_TYPE2))
         || (GetPortSignalType(gmvb_InputConnector[B_Channel] + 3) == SIG_OVL_TYPE3)))
   {
      //
      // 1 Recall vga calibration from nvram
      // 2 Program adc with vga calibration
      // 3 Read adc_c data to local array
      // 4 Write composite calibration setting back to the registers
      // 5 Write local copy of adc_c for vga
      //
      gmt_ADC_SETTING   AdcVgaSettings;
      BYTE B_RegAdc[4];
      BYTE B_Counter;

      status = ReadAdcSettings(gmd_ADC_0, (BYTE*)&AdcVgaSettings, ADC_SETTING_GRAPHIC);

      if (status == TRUE)
         gm_SetAdcGainOffset(gmd_ADC_0, &AdcVgaSettings);

      for (B_Counter = 0; B_Counter < 4; B_Counter++)
      {
         B_RegAdc[B_Counter] = gm_ReadRegByte(ADC_C_OFFSET1 + B_Counter);
      }

      gm_SetAdcGainOffset(gmd_ADC_0, &AdcSettings);
      for (B_Counter = 0; B_Counter < 4; B_Counter++)
      {
         gm_WriteRegByte(ADC_C_OFFSET1 + B_Counter, B_RegAdc[B_Counter]);
      }
   }
}

 #if (FEATURE_ADC_AOC == ENABLE)
//******************************************************************************
//
// FUNCTION       : void UpdateAOC(void)
//
// USAGE          : It should be called once per mode setup, as it tracks changes
//                from Aoc on - off. When changes happend on second channel we need to
//               refresh Aoc.
// INPUT          : B_Channel Channel number Main or pip
//
// OUTPUT         :
//
// GLOBALS        : None
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void UpdateAOC(gmt_PHY_CH B_Channel)
{
   static BOOL B_AocEnabled[2];
   BYTE B_OtherChannel = B_Channel == 1 ? 0 : 1;
   BOOL B_LastAoc = B_AocEnabled[gmd_ADC_0];
   //
   // Adc calibration registers already reloaded
   // Enable AOC (automatic offset compensation)
   //

   if (gm_IsGraphics(B_Channel)
         && (AfeGetChannelPortProc(B_Channel) == AIP)
         && (gmvb_CurrentSyncState[B_Channel] == DSS_STATE))
   {
      B_AocEnabled[gmd_ADC_0] = TRUE;
      gm_AOCEnable(gmd_ADC_0,TRUE);
      B_LastAoc = TRUE;
   }
   else
   {
      B_AocEnabled[gmd_ADC_0] = FALSE;
      gm_AOCEnable(gmd_ADC_0,FALSE);
   }
   if (B_AocEnabled[gmd_ADC_0]
         && B_LastAoc
         && gm_IsGraphics(B_OtherChannel)
         && (AfeGetChannelPortProc(B_OtherChannel) == AIP)
         && (gmvb_CurrentSyncState[B_OtherChannel] == DSS_STATE))
   {
      // reset AOC
      gm_AOCEnable(gmd_ADC_0, FALSE);
      B_AocEnabled[gmd_ADC_0] = FALSE;
      RetrieveAdcCalibrationValues(B_OtherChannel);
      gm_AOCEnable(gmd_ADC_0,TRUE);
      B_AocEnabled[gmd_ADC_0] = TRUE;
   }
}
 #endif // FEATURE_ADC_AOC
#endif // FEATURE_AIP == ENABLE

#if (FEATURE_CCS == ENABLE)
//******************************************************************************
//
// FUNCTION       : BOOL IsCCSforHDDisabled(void)
//
// USAGE          : Returns boolean based on CH_A input and UserPrefCCSforHD.
//
// INPUT          : None
//
// OUTPUT         : FALSE - HD video on CH_A and UerPrefCCSforHD is off
//
// GLOBALS        : None
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BOOL IsCCSforHDDisabled(void)
{
#ifdef Action_AdjustCCSforHD_WB_Used
   if ((gm_IsHDVideo(CH_A)) && (UserPrefCCSforHD==Off))
      return TRUE;
   else
#endif
      return FALSE;
}
#endif

#if (FEATURE_TWO_STAGE_PROCESSING_MODE == ENABLE)
//******************************************************************************
//
// FUNCTION       : BOOL IsTwoStageAllowed(void)
//
// USAGE          : Return if two-stage processing is allowed
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :   None
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BOOL IsTwoStageAllowed(void)
{
   // Add the limitation for Athena which doesn't have SDDS2.
   WORD W_InputVActive = stInput[CH_A].Vactive;
   WORD W_OutputVActive = gmvw_OutputVActive[CH_A];
   DWORD DW_DisplayClockFreq
   = (DWORD)gmv_OUTPUT_HTOTAL * gmv_OUTPUT_VTOTAL * gmvb_OutputFrameRate;
   DWORD DW_SourceClockFreq;
   BYTE B_Result = TRUE;

   gm_WriteRegWord(M_CLKMEAS_CONTROL, 256L<<3);
   gm_Delay1ms(1);//only 12.8us delay is needed

   DW_SourceClockFreq = (DWORD)gm_ReadRegWord(M_CLKMEAS_RESULT);
   DW_SourceClockFreq = DW_SourceClockFreq * ((DWORD)(gmc_TCLK_FREQ) / 256L);

   if ((W_InputVActive*2 > W_OutputVActive)
         &&(DW_DisplayClockFreq <= 2*DW_SourceClockFreq))
      B_Result = FALSE;

   return ((UserPrefTwoStageProcessing == TRUE) && gm_IsInterlaced(CH_A)
           && (gm_GetChannelInputAttr(CH_A, gmd_COLOR_SPACE) != gmd_RGB)
           && (gmvb_MainChannel == CH_A)
           && B_Result
          );
}

//******************************************************************************
//
// FUNCTION       : void EnterTwoStageProcessing(void)
//
// USAGE          : Reconfigure both channels to enter two-stage processing
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :   W_OverscanEnabled[]  (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void EnterTwoStageProcessing(void)
{
   SetDisplayOff(CH_A, BLENDER_OFF);
   SetDisplayOff(CH_B, BLENDER_OFF);

   gm_VideoProcessingEnable(VIDEO_2STAGE);

   //restore the normal mapping
   gmvb_MainChannel = CH_A;
   gmvb_PipChannel = CH_B;

   //must execute the following lines otherwise green and pink lines will appear
   gm_SetRegBitsWord(PHF_CTRL, PHF_INT_COEF_EN);
   gm_SetRegBitsWord(PVF_CTRL, PVF_INT_COEF_EN);

   gm_StateDetectionConfig(CH_B, DET_DISABLE);

   gm_SetFlagLineProcess(CH_B, DISABLE);
   //#if (FEATURE_INTERRUPT_SERVICE == REV2)
   //    INTERRUPT_DISABLE(PIP_INPUT_IRQ4MASK, IPP_LINEFLAG_MASK);
   //#endif

   //gm_DecoderMemory(CH_B, gmd_DISABLE);

   SetDisplayMain();
   gm_AfmSetup(W_OverscanEnabled[CH_A]);
   gm_DisplaySetSize(CH_A);
   gm_DisplaySetPosition(CH_A);
#ifndef NO_PIP_SUPPORT
   SetPipBorder(CH_A, FALSE);
   SetPipBorder(CH_B, FALSE);
#endif
   gm_ScalerSetupTwoStage();
   gm_ForceUpdate(CH_B, INPUT_CLOCK);//must run after gm_ScalerSetupTwoStage()
}

//******************************************************************************
//
// FUNCTION       : void ExitTwoStageProcessing(void)
//
// USAGE          : Reconfigure both channels to exit two-stage processing
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :   gmvw_InputHActive[]      (W)
//               gmvw_InputVActive[]      (W)
//               gmvw_InputHTotal[]      (W)
//               gmvw_InputHStart[]      (W)
//               gmvw_InputVStart[]      (W)
//               gmvw_InputHActiveOrg[]   (R)
//               gmvw_InputVActiveOrg[]   (R)
//               gmvw_InputHTotalOrg[]   (R)
//               gmvw_InputVTotalOrg[]   (R)
//               gmvw_InputHStartOrg[]   (R)
//               gmvw_InputVStartOrg[]   (R)
//               gmvb_MemoryNumBuffers[]   (W)
//               W_OverscanEnabled[]      (R)
//
// USED_REGS      :      DBL_FILT_CTRL (W)
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void ExitTwoStageProcessing(void)
{
   SetDisplayOff(CH_A, BLENDER_OFF);
   SetDisplayOff(CH_B, BLENDER_OFF);

   gm_VideoProcessingDisable(VIDEO_2STAGE);
   gm_WriteRegWord(DBL_FILT_CTRL, 0);

   //gm_DecoderMemory(CH_B, gmd_ENABLE);
   gm_SetFlagLineProcess(CH_B, ENABLE);
   //#if (FEATURE_INTERRUPT_SERVICE == REV2)
   //    INTERRUPT_ENABLE(PIP_INPUT_IRQ4MASK, IPP_LINEFLAG_MASK);
   //#endif

   gm_StateDetectionConfig(CH_B, DET_RESTART);

   gmvb_MemoryBufferTypes[CH_A] = 0;
   gmvb_MemoryBufferTypes[CH_B] = 0;
   stInput[gmvb_MainChannel].Vtotal = gmvw_InputVTotalOrg[gmvb_MainChannel];
   stInput[gmvb_MainChannel].Htotal = gmvw_InputHTotalOrg[gmvb_MainChannel];
   stInput[gmvb_MainChannel].Vstart = gmvw_InputVStartOrg[gmvb_MainChannel];
   stInput[gmvb_MainChannel].Vactive = gmvw_InputVActiveOrg[gmvb_MainChannel];
   stInput[gmvb_MainChannel].Hstart = gmvw_InputHStartOrg[gmvb_MainChannel];
   stInput[gmvb_MainChannel].Hactive = gmvw_InputHActiveOrg[gmvb_MainChannel];
   
 #ifndef NO_PIP_SUPPORT
   stInput[gmvb_PipChannel].Vtotal = gmvw_InputVTotalOrg[gmvb_PipChannel];
   stInput[gmvb_PipChannel].Htotal = gmvw_InputHTotalOrg[gmvb_PipChannel];
   stInput[gmvb_PipChannel].Vstart = gmvw_InputVStartOrg[gmvb_PipChannel];
   stInput[gmvb_PipChannel].Vactive = gmvw_InputVActiveOrg[gmvb_PipChannel];
   stInput[gmvb_PipChannel].Hstart = gmvw_InputHStartOrg[gmvb_PipChannel];
   stInput[gmvb_PipChannel].Hactive = gmvw_InputHActiveOrg[gmvb_PipChannel];
 #endif

   ModeSetPrefs(CH_A);
   ModeSetPrefs(CH_B);
   ModeSetupUserPref(gmvb_MainChannel);
 #ifndef NO_PIP_SUPPORT
   ModeSetupUserPref(gmvb_PipChannel);
 #endif
   SetupColorSpace(gmvb_MainChannel);
 #ifndef NO_PIP_SUPPORT
   SetupColorSpace(gmvb_PipChannel);
 #endif

//   ModeSetupInputChange(CH_A);
//   ModeSetupInputChange(CH_B);
   SelectAspectMain();
   SetCapture(gmvb_MainChannel);
   gm_SetupInputMain();
   gm_AfmSetup(W_OverscanEnabled[CH_A]);

#ifndef NO_PIP_SUPPORT
   SelectAspectPip();
   SetCapture(gmvb_PipChannel);
   gm_SetupInputPip();
#endif

   gm_ScalerSetup(CH_A);
   gm_ScalerSetup(CH_B);
   gm_MemorySetup(CH_A);
   gm_MemorySetup(CH_B);

   gm_SyncUpdate(CH_A, INPUT_OUTPUT_CLOCK);
   gm_SyncUpdate(CH_B, INPUT_OUTPUT_CLOCK);
}
#endif

//******************************************************************************
//
// FUNCTION       : void BypassMemory(void)
//
// USAGE          : Bypass memory for non scaling output and debug usage
//
// INPUT          : None
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
void BypassMemory(void)
{
   // Update Register
   if (gm_ReadRegWord(IMP_CONTROL) & IMP_EXT_DV_EN)
   {
      #if 0
      gm_WriteRegWord(IMP_H_ACT_START, gm_ReadRegWord(IMP_IBD_HSTART));
      gm_WriteRegWord(IMP_V_ACT_START_ODD, gm_ReadRegWord(IMP_IBD_VSTART));
      gm_WriteRegWord(IMP_V_ACT_START_EVEN, gm_ReadRegWord(IMP_IBD_VSTART));
      gm_ClearRegBitsWord(IMP_CONTROL,IMP_EXT_DV_EN);
      #else
         gm_WriteRegWord(IMP_H_ACT_START, 0);
         gm_WriteRegWord(IMP_V_ACT_START_ODD, 0);
         gm_WriteRegWord(IMP_V_ACT_START_EVEN, 0);
      #endif
   }
   else
   {
      gm_WriteRegWord(IMP_H_ACT_START, gm_ReadRegWord(IMP_IBD_HSTART));
      gm_WriteRegWord(IMP_V_ACT_START_ODD, gm_ReadRegWord(IMP_IBD_VSTART));
      gm_WriteRegWord(IMP_V_ACT_START_EVEN, gm_ReadRegWord(IMP_IBD_VSTART));
   }

   //gm_WriteRegWord(SYNC_CONTROL, BIT2);
   gm_ClearAndSetBitsWord(SYNC_CONTROL, FRAME_LOCK_MODE, BIT2);
   gm_SyncUpdate(gmvb_MainChannel, INPUT_OUTPUT_CLOCK);

   gm_ClkSelect(CLK_ODP, ODP_SEL_IMP_CLK);
   gm_ClkSelect(CLK_LVTX_IN, LVTX_IN_SEL_IMP_CLK);
   gm_WaitForInputBlanking(CH_A);

   if (gm_ReadRegWord(IMP_CONTROL) & IMP_EXT_DV_EN)
         gm_WriteRegWord(IMP_VLOCK_EVENT, 0x00);
   else
         gm_WriteRegWord(IMP_VLOCK_EVENT, 0x01);

   gm_WriteRegWord(DH_LOCK_LOAD, 0);
   gm_WriteRegWord(DV_LOCK_LOAD, gm_ReadRegWord(DV_ACTIVE_START) - 1);
 
   gm_SyncUpdate(gmvb_MainChannel, OUTPUT_CLOCK);

   //gm_ClearRegBitsWord(MC_MEMORY_CONFIG, MAIN_MC_BYPASS);
   gm_WriteRegWord(0xA872, 0x100);  // Bypass memory. Only for Athena cut2.

   gm_ForceUpdate(gmvb_MainChannel, INPUT_OUTPUT_CLOCK);
}

void GameModeControl(void)
{
#ifdef DELL_U2410F//121005 Edward for game mode will become disable if change to non 60 and change back to 60
		if(IsGameModeEnable())
			UserPrefGameMode = On;
		else
			UserPrefGameMode = OFF;
#else
		//if((abs(stInput[gmvb_MainChannel].Vfrq) - 600) > 10)
	if(IsGameModeApplicable() == FALSE)
	{
	  if (B_DisplayLockMethod == DISP_LOCK_NORMAL)
		{
     #if (ENABLE_DFL == 1) 
		 B_DisplayLockMethod = DISP_LOCK_DFL;
		 gm_Printf("~~GameModeControl~~ Change Back to DFL ~~~~", 0);
     #else
		 B_DisplayLockMethod = DISP_FREERUN;
		 gm_Printf("~~GameModeControl~~ Change Back to Free Run ~~~~", 0);		   
     #endif
	  }
		//gm_Print("V Freq. not 60Hz ... Disable Game Mode ...", 0);
	}
#endif
		
		//120925 Edward add for OSD change disable VWD when Preset mode is "Game"
		if(IsPresetMode_GRAPHICS_GAME)
		{
			if(UserPrefVwdFlag == ON)
			{
				UserPrefVwdFlag = OFF;
				VWD_Stop();
			}
		}
		else
		{
			if (UserPrefVwdDemo != VWD_DEMO_OFF)
			{
				UserPrefVwdFlag = ON;
			}
		}
	
	#if(FEATURE_AIP == ENABLE)
		if (IsAnalogInput(gmvb_MainChannel))
		{
			if(UserPrefVwdFlag == ON)
			{
				UserPrefVwdFlag = OFF;
				VWD_Stop();
			}
		}
	#endif
		
		//Neil120827+
		if (IsGameModeEnable() == gmd_FALSE)
	{
		if(GetChannelConnType(CH_A) == VGA)
		{
			if((stInput[CH_A].Vactive != PanelHeight))
			{
				if(gm_ReadRegWord(DV_TOTAL) != gmv_OUTPUT_VTOTAL)
					gm_WriteRegWord(DV_TOTAL, gmv_OUTPUT_VTOTAL);
			}
		}
		else
		{
			if(gm_ReadRegWord(DV_TOTAL) != gmv_OUTPUT_VTOTAL)
				gm_WriteRegWord(DV_TOTAL, gmv_OUTPUT_VTOTAL);
		}
		//Hyb130720+ disable game mode, set to DFL, don't set to DFL, not program value
		gm_WriteRegWord(SYNC_CONTROL, 2);							  
		#if (ENABLE_DFL == 1) 
		B_DisplayLockMethod = DISP_LOCK_DFL;
		gm_Printf("~~GameModeControl~~ Change Back to DFL ~~~~", 0);
		#else
		B_DisplayLockMethod = DISP_FREERUN;
		gm_Printf("~~GameModeControl~~ Change Back to Free Run ~~~~", 0);		  
		#endif
	
		if (IsMainVGAInput()&& (stInput[CH_A].Vactive == PanelHeight))
			gm_ClearRegBitsWord(DDDS1_CONTROL, DDDS1_FORCE_OPLOOP);
		else
			gm_SetRegBitsWord(DDDS1_CONTROL, DDDS1_FORCE_OPLOOP);	
		return;
	}
   else
   {
  #if (ENABLE_DFL == 1)
	  if (B_DisplayLockMethod == DISP_LOCK_DFL)
  #else
	  if (B_DisplayLockMethod == DISP_FREERUN)
  #endif
	  {
		 B_DisplayLockMethod = DISP_LOCK_NORMAL;
		 //gm_Printf("~~~~ Game Mode Enable change to DISP_LOCK_NORMAL ~~~~", 0);
	  }
   }
	   //121005 Edward change by AE comment some 60Hz timing image distortion	
#if (ENABLE_DFL == 0)
   gm_Printf("~=========GameModeControl====================Set to Program Lock 01", 0);
   gm_WriteRegWord(SYNC_CONTROL, BIT0);//BIT2
   
   gm_SyncUpdate(gmvb_MainChannel, INPUT_OUTPUT_CLOCK);


	if (IsMainVGAInput() && (stInput[CH_A].Vactive == PanelHeight) )
		gm_WriteRegWord(DV_TOTAL, Real_VTotal);
	else
	gm_WriteRegWord(DV_TOTAL, gmv_OUTPUT_VTOTAL + 4);

	   gm_WaitForInputBlanking(CH_A);
	
		if (IsMainVGAInput() && (stInput[CH_A].Vactive == PanelHeight) )
			gm_ClearRegBitsWord(DDDS1_CONTROL, DDDS1_FORCE_OPLOOP);
		else
	gm_SetRegBitsWord(DDDS1_CONTROL,DDDS1_FORCE_OPLOOP);
	gm_WriteRegWord(DDDS1_INIT, 0x01);


	gm_WriteRegWord(IMP_VLOCK_EVENT, 0x10); // 0);

	gm_WriteRegWord(DH_LOCK_LOAD, 1);
   gm_WriteRegWord(DV_LOCK_LOAD, gm_ReadRegWord(DV_ACTIVE_START) - 1);
 
   gm_SyncUpdate(gmvb_MainChannel, OUTPUT_CLOCK);
#endif
}


#if defined(ST_4K2K_93xx_BOARD) || defined(RD2_4K2K_DEMO)
extern WORD Real_Input_H;
extern WORD Real_Input_V;
void gm_4K2K_MainCapture(void)
{
			gmt_WindowStruct InputWinDim, OutputWinDim;
			gmt_WindowStruct PrevInputDim, PrevOutputDim;
			WORD W_OutputLeftBorderWidth;
			WORD W_OutputRightBorderWidth;
			WORD W_OutputTopBorderHeight;
			WORD W_OutputBottomBorderHeight;
			WORD W_PanelWidth = PanelWidth;
			WORD W_PanelHeight = PanelHeight;
			WORD W_PanelBezelWidth = 0;
			WORD W_PanelBezelHeight = 0;

			DWORD DW_TotalOutputW;
			DWORD DW_TotalOutputH;

			//these values are restored at the end of the routine
			ASPECT_RATIOS_MODE TempAspectRatio = UserPrefAspectRatioMain;
			//BYTE TempPCWideMode = UserPrefPCWideMode;

			//if((GetParam(0) == 0) || (GetParam(1) == 0) || (GetParam(4) == 0) || (GetParam(5) == 0))
			//{
			//	gm_Print("Wariing ............... Input or Output Width and Height can not set to %d", 0);
			//	gm_Print("Format: Appstest 8 InputWidth InputHStart InputHeight InputVStart OutputWidth OutputHStart OutputHeight OutputVStart ", 0);
			//	return;
			//}

			#if 0
			if(GetParam(1) < stInput[gmvb_MainChannel].Hstart)
			{
				gm_Print("Wariing ............... H Start should great then %d", stInput[gmvb_MainChannel].Hstart);
				return;
			}

			if(GetParam(3) < stInput[gmvb_MainChannel].Vstart)
			{
				gm_Print("Wariing ............... V Start should great then %d", stInput[gmvb_MainChannel].Vstart);
				return;
			}
			#endif

			OutputWinDim.W_Width = PanelWidth;
			OutputWinDim.W_Hstart = 0;
			OutputWinDim.W_Height = PanelHeight;
			OutputWinDim.W_Vstart = 0;
	#if (FEATURE_PANORAMIC == ENABLE)
			//exit panoramic mode
			if (UserPrefAspectRatioMain == AR_PANORAMIC)
			{

				// RC: Disable blender such that OSD does not disappear when panoramic
				// is disabled.
				gm_BlenderOnOff(gmvb_MainChannel, FALSE);
				gm_ForceUpdate(gmvb_MainChannel, OUTPUT_CLOCK);
				gm_ScalerSetPanoramic(PanelWidth, gmd_PANORAMIC_OFF);
			}
   #endif
			if (!gm_IsValidSignal(gmvb_MainChannel))//bbao July 24, 2006: if no signal ignore the command
				return;

			PrevInputDim.W_Hstart = stInput[gmvb_MainChannel].Hstart;
			PrevInputDim.W_Width = stInput[gmvb_MainChannel].Hactive;
			PrevInputDim.W_Vstart = stInput[gmvb_MainChannel].Vstart;
			PrevInputDim.W_Height = stInput[gmvb_MainChannel].Vactive;
			PrevOutputDim.W_Hstart = gmvw_OutputHStart[gmvb_MainChannel];
			PrevOutputDim.W_Width = gmvw_OutputHActive[gmvb_MainChannel];
			PrevOutputDim.W_Vstart = gmvw_OutputVStart[gmvb_MainChannel];
			PrevOutputDim.W_Height = gmvw_OutputVActive[gmvb_MainChannel];
			
			if (gm_IsVideo(gmvb_MainChannel))
			{
				gmt_WindowStruct LocalOutputWin;
				LocalOutputWin.W_Width = PanelWidth;
				LocalOutputWin.W_Height = PanelHeight;

				//following two lines taken from SelectAspectMain()
				B_AspectMethod[gmvb_MainChannel] = UserPrefAspectRatioMain;
				//B_InputAspect[gmvb_MainChannel]  = GetAspectRatio(gmvb_MainChannel);
				SetCapture(gmvb_MainChannel);

				//assuming B_AspectMethod is correct
				//ApplyOutputAspect(&LocalOutputWin, B_AspectMethod[gmvb_MainChannel]);
				ApplyOutputAspect(gmvb_MainChannel, B_AspectMethod[gmvb_MainChannel]);

				gmvw_OutputHActive[gmvb_MainChannel] = LocalOutputWin.W_Width;
				gmvw_OutputVActive[gmvb_MainChannel] = LocalOutputWin.W_Height;
			}
			else
			{
				//UserPrefAspectRatioMain = WMODE_NORMAL;
				//UserPrefPCWideMode = PCMODE_FULL2;//fill the whole screen
				//AdjustPCWideModeInputAspect();
				//AdjustPCWideModeOutputAspect();
				//following two lines taken from SelectAspectMain()
				B_AspectMethod[gmvb_MainChannel] = UserPrefAspectRatioMain;
				//B_InputAspect[gmvb_MainChannel]  = GetAspectRatio(gmvb_MainChannel);
				SetCapture(gmvb_MainChannel);
			}
			InputWinDim.W_Height = stInput[gmvb_MainChannel].Vactive;
			InputWinDim.W_Width = stInput[gmvb_MainChannel].Hactive;
			InputWinDim.W_Hstart = stInput[gmvb_MainChannel].Hstart;
			InputWinDim.W_Vstart = stInput[gmvb_MainChannel].Vstart;

			#if 0
			 if(UserPrefCaptureM == CAPTURE_MODE_LEFT)
         {
            SETBIT(stInput[gmvb_MainChannel].ModeFlags, gmd_MODE_CAPTURE_CROP);        
         }
			#endif
			
			// calculate output size, assuming output window size is determined already
			DW_TotalOutputW = W_PanelWidth + ((DWORD)W_PanelWidth + W_PanelBezelWidth) * (UserPrefPD_MainXNumber - 1);
			DW_TotalOutputH = W_PanelHeight + ((DWORD)W_PanelHeight + W_PanelBezelHeight) * (UserPrefPD_MainYNumber - 1);

			//if ((IsCurrentInputPC() && (UserPrefPCWideMode == PCMODE_FULL2))
			//	||(IsCurrentInputVideo() && (UserPrefAspectRatioMain != AR_PILLAR_BOX)))//bbao Aug 19, 2006: only 4:3 widemode displays blank area
			if((gm_IsVideo(gmvb_MainChannel) && (UserPrefAspectRatioMain != AR_PILLAR_BOX)) ||
				(gm_IsGraphics(gmvb_MainChannel) && (UserPrefAspectRatioMain == AR_FULL_SCREEN)))
			{
				// do not care about the aspect ratio and just fill the whole screen
				W_OutputTopBorderHeight = 0;
				W_OutputBottomBorderHeight = 0;
				W_OutputLeftBorderWidth = 0;
				W_OutputRightBorderWidth = 0;
			}
			else if ((DW_TotalOutputW * gmvw_OutputVActive[gmvb_MainChannel]/gmvw_OutputHActive[gmvb_MainChannel])
				> DW_TotalOutputH)
			{	//height is the limiting factor
				W_OutputTopBorderHeight = 0;
				W_OutputBottomBorderHeight = 0;
				W_OutputLeftBorderWidth = (WORD)((DW_TotalOutputW - DW_TotalOutputH * gmvw_OutputHActive[gmvb_MainChannel] / gmvw_OutputVActive[gmvb_MainChannel])/2);
				W_OutputRightBorderWidth = W_OutputLeftBorderWidth;
			}
			else
			{	//width is the limiting factor
				W_OutputLeftBorderWidth = 0;
				W_OutputRightBorderWidth = 0;
				W_OutputTopBorderHeight = (WORD)((DW_TotalOutputH - DW_TotalOutputW * gmvw_OutputVActive[gmvb_MainChannel] / gmvw_OutputHActive[gmvb_MainChannel])/2);
				W_OutputBottomBorderHeight = W_OutputTopBorderHeight;
			}

			#if 0
			MultiDisplaySetup(W_OutputLeftBorderWidth, W_OutputRightBorderWidth, W_OutputTopBorderHeight,
				W_OutputBottomBorderHeight, W_PanelWidth, W_PanelHeight, W_PanelBezelWidth, W_PanelBezelHeight,
				UserPrefXNumber, UserPrefYNumber);
			GetMultiDisplayWinAttrib((UserPrefPublicDisplayNumber-1), &InputWinDim, &OutputWinDim);
			InputWinDim.W_Width = (InputWinDim.W_Width + 1) & ~1;//make sure it is even
			OutputWinDim.W_Hstart &= ~1;//make sure it is even
			OutputWinDim.W_Width = (OutputWinDim.W_Width + 1) & ~1;//make sure it is even
			#endif

         #if 1
         //gm_Print("Capture Main 1 = .............. %d", UserPrefCaptureM);
         
			InputWinDim.W_Vstart += 0;         
         InputWinDim.W_Height = Real_Input_V;         
         
         if(UserPrefCaptureM == CAPTURE_MODE_FULL)		// FE
         {
            //gm_Print("gm_4K2K_MainCapture-Full",0);
         	InputWinDim.W_Width = Real_Input_H / 2;
            OutputWinDim.W_Width = PanelWidth; //gmvw_OutputHActive[gmvb_MainChannel];
            InputWinDim.W_Hstart = 0;
				
         	//InputWinDim.W_Width = (Real_Input_H / 2);
            //OutputWinDim.W_Width = PanelWidth; // / 2;
            //InputWinDim.W_Hstart += ((PanelWidth / 2));
				//gm_WriteRegWord(DV_TOTAL,gmv_OUTPUT_VTOTAL + 4);
         }
         else if(UserPrefCaptureM == CAPTURE_MODE_RIGHT)		// R
         {
            //gm_Print("gm_4K2K_MainCapture-Right",0);         
         	//InputWinDim.W_Width = Real_Input_H / 2;
            //OutputWinDim.W_Width = PanelWidth; //gmvw_OutputHActive[gmvb_MainChannel];
            //InputWinDim.W_Hstart = 0;

				InputWinDim.W_Width = (Real_Input_H / 2);
            #if 1
            InputWinDim.W_Hstart += ((gmvw_InputHActiveOrg[gmvb_MainChannel] / 2));
            if(InputWinDim.W_Hstart & BIT0) // for middle missing 1 line : 1366 x 768
            {
               InputWinDim.W_Hstart += 1;
            }
            #else
            InputWinDim.W_Hstart += ((PanelWidth / 2));
            #endif
            OutputWinDim.W_Width = PanelWidth; // / 2;
				gm_WriteRegWord(DV_TOTAL,gmv_OUTPUT_VTOTAL); // + 4);
         }
         else if(UserPrefCaptureM == CAPTURE_MODE_LEFT)	// L
         {
            //gm_Print("gm_4K2K_MainCapture-Left",0);         
            InputWinDim.W_Width = (Real_Input_H / 2);
            InputWinDim.W_Hstart += 0;            
            OutputWinDim.W_Width = PanelWidth; // / 2;
				gm_WriteRegWord(DV_TOTAL,gmv_OUTPUT_VTOTAL); // + 2);
         }
			else
			{
				InputWinDim.W_Width = Real_Input_H;
            OutputWinDim.W_Width = PanelWidth; //gmvw_OutputHActive[gmvb_MainChannel];
            #if 1
            InputWinDim.W_Hstart = gm_ReadRegWord(IMP_IBD_HSTART);
            #else
            InputWinDim.W_Hstart = 0;
         	#endif
			}
         #endif
        
			OutputWinDim.W_Height = PanelHeight;
			OutputWinDim.W_Hstart = 0;
			OutputWinDim.W_Vstart = 0;

			#if 0
			if(UserPrefXNumber > 1)
				{
			InputWinDim.W_Width += 2;
			OutputWinDim.W_Width +=2;
				}
			if(UserPrefYNumber > 1)
				{
			InputWinDim.W_Height+= 1;
			OutputWinDim.W_Height+=2;			
				}
			#endif
			
			// after this line the new input and output dimension is stored in InputWinDim and OutputWinDim
				
/****** ZOOM STARTS ******/
			// This means no image should be shown in this panel
			if ((InputWinDim.W_Width == 0)
				||(InputWinDim.W_Height == 0))
			{
				SetDisplayOff(gmvb_MainChannel, BACKGROUND_ONLY);
				return;
			}

			#if 0
			gm_Print("Input W_Width ........... = %d", InputWinDim.W_Width);
			gm_Print("Input W_Height ........... = %d", InputWinDim.W_Height);
         gm_Print("Input W_Hstart ........... = %d", InputWinDim.W_Hstart);
			gm_Print("Input W_Vstart ........... = %d", InputWinDim.W_Vstart);
			
			gm_Print("Output W_Width ........... = %d", OutputWinDim.W_Width);
			gm_Print("Output W_Height ........... = %d", OutputWinDim.W_Height);
         gm_Print("Output W_Hstart ........... = %d", OutputWinDim.W_Hstart);
			gm_Print("Output W_Vstart ........... = %d", OutputWinDim.W_Vstart);
			
			gm_Print("Prev W_Width ........... = %d", PrevOutputDim.W_Width);
			gm_Print("Prev W_Height ........... = %d", PrevOutputDim.W_Height);
         gm_Print("Prev W_Hstart ........... = %d", PrevOutputDim.W_Hstart);
			gm_Print("Prev W_Vstart ........... = %d", PrevOutputDim.W_Vstart);
			#endif

			// Restore original output size; this is needed for RelocateInputCaptureWindow()
			gmvw_OutputHStart[gmvb_MainChannel] = PrevOutputDim.W_Hstart;
			gmvw_OutputHActive[gmvb_MainChannel] = PrevOutputDim.W_Width;
			gmvw_OutputVStart[gmvb_MainChannel] = PrevOutputDim.W_Vstart;
			gmvw_OutputVActive[gmvb_MainChannel] = PrevOutputDim.W_Height;
			//RelocateInputCaptureWindow(gmvb_MainChannel, &PrevInputDim,
			//				 		&InputWinDim, FALSE);
			RelocateInputCaptureWindow(gmvb_MainChannel, &PrevInputDim,
							 		&InputWinDim);
			
/****** ZOOM ENDS ******/
/****** DYNAMIC SCALING STARTS ******/
			// these 4 lines are needed for programming the scalers
			stInput[gmvb_MainChannel].Vactive = InputWinDim.W_Height;
			stInput[gmvb_MainChannel].Hactive = InputWinDim.W_Width;
			stInput[gmvb_MainChannel].Hstart= InputWinDim.W_Hstart;
			stInput[gmvb_MainChannel].Vstart= InputWinDim.W_Vstart;
			
			// store current output dimension
			SetDynamicScalerSizeBegin(gmvb_MainChannel);

			gmvw_OutputVActive[gmvb_MainChannel] = OutputWinDim.W_Height;
			gmvw_OutputHActive[gmvb_MainChannel] = OutputWinDim.W_Width;
			gmvw_OutputHStart[gmvb_MainChannel] = OutputWinDim.W_Hstart;
			gmvw_OutputVStart[gmvb_MainChannel] = OutputWinDim.W_Vstart;

			//if no change to input-output ratio, skip the second dynamic scaling sequence
			if (((gmvw_OutputHActive[gmvb_MainChannel] * PrevInputDim.W_Width)
				== (stInput[gmvb_MainChannel].Hactive  * PrevOutputDim.W_Width))
				&&
				((gmvw_OutputVActive[gmvb_MainChannel] * PrevInputDim.W_Height)
				== (stInput[gmvb_MainChannel].Vactive  * PrevOutputDim.W_Height))
				&& (gmvw_OutputHStart[gmvb_MainChannel] == PrevOutputDim.W_Hstart)
				&& (gmvw_OutputVStart[gmvb_MainChannel] == PrevOutputDim.W_Vstart)
			)
			{
				DynamicScalerStop();
			}
			else
			{
				// Setup Display
				//using only one step is probably too abrupt for the hardware; artifact is
				//  part of the image go black temporarily
				SetDynamicScalerStep(gmvb_MainChannel, 2);
				SetDynamicScalerStep(gmvb_PipChannel, 0);	// turn off pip scaling
			
				SetDynamicScalerSizeEnd(gmvb_MainChannel);

				DynamicScalerStart(gmvb_MainChannel);
				DynamicScalerProcess(FALSE);		
				DynamicScalerStop();
				
				if (gmvb_MainChannel == CH_A)
				{
				//
				// to disable TNR (TNR_PROGRESSIVE) bits 
				// in case of Vertical Shrink
				//
					if (stInput[gmvb_MainChannel].Vactive > gmvw_OutputVActive[CH_A])	
					{
						gm_VideoProcessingDisable(VIDEO_TNR);
					}
				}
			}
			
			//restore original aspect ratio
			UserPrefAspectRatioMain = TempAspectRatio;
			//UserPrefPCWideMode = TempPCWideMode;

         #if 0
			//gm_Print("Input W_Hstart 1 ........... = %d", InputWinDim.W_Hstart);
         if(UserPrefCaptureM == CAPTURE_MODE_RIGHT)
			   gm_WriteRegWord(IMP_H_ACT_START, (Real_Input_H / 2));
         else
            gm_WriteRegWord(IMP_H_ACT_START, 0);
         
			gm_WriteRegWord(IMP_V_ACT_START_ODD, 0);
			gm_WriteRegWord(IMP_V_ACT_START_EVEN, 1);
         #endif
			
			// Request Blenders ON and User Prefs
			ModeSetupBlender(gmvb_MainChannel, DISPLAY_DEFER_TMO_NORMAL);

         W_OutputLeftBorderWidth = W_OutputLeftBorderWidth;
			W_OutputRightBorderWidth = W_OutputRightBorderWidth;
			W_OutputTopBorderHeight = W_OutputTopBorderHeight;
			W_OutputBottomBorderHeight = W_OutputBottomBorderHeight;
}

#endif

#ifdef ST_4K2K_93xx_BOARD
// For Image synchronization
#if (DEBUG_DISPLAY_TIME_MEASUREMENT==ENABLE)
extern void SetImgOnOffTime(BYTE b_OnOff, DWORD dw_Time);
#endif
void Image_Force_OnOff(BOOL OnOff)
{
	if(OnOff) // On
	{
		gm_SetRegBitsWord(PB_CTRL, PB_MAIN_ENABLE);
   	gm_ClearRegBitsWord(PB_CTRL, PB_FORCE_MAIN_BKGND);
   	gm_ForceUpdate(CH_A , INPUT_OUTPUT_CLOCK);
   	#if DEBUG_DISPLAY_TIME_MEASUREMENT
      SetImgOnOffTime(On,gm_GetSystemTime());
   	#endif
   	
	}
	else // Off
	{
		gm_ClearRegBitsWord(PB_CTRL, PB_MAIN_ENABLE);
   	gm_SetRegBitsWord(PB_CTRL, PB_FORCE_MAIN_BKGND);
   	gm_ForceUpdate(CH_A , INPUT_OUTPUT_CLOCK);
      #if DEBUG_DISPLAY_TIME_MEASUREMENT
      SetImgOnOffTime(Off,gm_GetSystemTime());
      #endif
   	
	}
}

void Image_ReadyFlag(BOOL flag)
{
	if(GetChipID_4K2K()==CHIPID_4K2K_R)
	{
		if(flag) // Ready
			gm_SetRegBitsWord(GPOUTPUT5, GPIO82_OUT);
		else	// Not Ready
			gm_ClearRegBitsWord(GPOUTPUT5, GPIO82_OUT);
	}
	else if(GetChipID_4K2K()==CHIPID_4K2K_L)
	{
		if(flag) // Ready
			gm_SetRegBitsWord(GPOUTPUT5, GPIO84_OUT);
		else	// Not Ready
			gm_ClearRegBitsWord(GPOUTPUT5, GPIO84_OUT);	
	}
}

BOOL IS_Image_Ready(void)
{
	if(GetChipID_4K2K()==CHIPID_4K2K_R)
	{
#if ENABLE_DP_MST_TIMEOUT_CHECK_FOR_OWN_READY_BIT	
      if(DpRxIsMST_Stream(UserPrefCurrentInputMain) == TRUE) //MST Mode
      {
         if(gm_TimerCheck(IMAGE_SYNC_DP_MST_TIMEOUT_10ms_TMR) != TIMER_TMO)
         {       
            /*ChengLin 20130415: If timer not equal timeout, system will keep SST method to check ready status*/
            if(UserPrefCaptureM != CAPTURE_MODE_HALF_HORIZONTAL)
            {
               if(ImageSync_ForceCheckReadySetting == FALSE)
               {
                  if(gm_ReadRegWord(GPINPUT5) & GPIO82_OUT) 
                     return TRUE;
               }
               else //DP MST Time out then force check Ready Bit output setting status
               {         
                  if(gm_ReadRegWord(GPOUTPUT5) & GPIO82_OUT)   
                     return TRUE;
               }
            }
            else//PBP
            {        
               if(gm_ReadRegWord(GPOUTPUT5) & GPIO82_OUT)   
                  return TRUE;
            }
         }
         else
         {   
            /*ChengLin 20130415: If timer equal timeout, system will check own ready bit setting status*/  
            if(gm_ReadRegWord(GPOUTPUT5) & GPIO82_OUT)   
               return TRUE;
         }
      }
      else //SST Mode
#endif      
      {
		   /*ChengLin 20130306: Separate Ready bit status check by PBP mode status*/
		   if(UserPrefCaptureM != CAPTURE_MODE_HALF_HORIZONTAL)
		   {
			   if(gm_ReadRegWord(GPINPUT5) & GPIO82_OUT)	
				   return TRUE;
		   }
		   else //PBP
		   {
			   if(gm_ReadRegWord(GPOUTPUT5) & GPIO82_OUT)	
				   return TRUE;
		   }
      }
	}
	else if(GetChipID_4K2K()==CHIPID_4K2K_L)
	{
#if ENABLE_DP_MST_TIMEOUT_CHECK_FOR_OWN_READY_BIT	
      if(DpRxIsMST_Stream(UserPrefCurrentInputMain) == TRUE) //MST Mode
      {
         /*ChengLin 20130415: If timer not equal timeout, system will keep SST method to check ready status*/      
         if(gm_TimerCheck(IMAGE_SYNC_DP_MST_TIMEOUT_10ms_TMR) != TIMER_TMO)
         {       
            if(UserPrefCaptureM != CAPTURE_MODE_HALF_HORIZONTAL)
            {
               if(ImageSync_ForceCheckReadySetting == FALSE)
               {
                  if(gm_ReadRegWord(GPINPUT5) & GPIO84_OUT)    
                     return TRUE;
               }
               else //DP MST Time out then force check Ready Bit output setting status
               {
                  if(gm_ReadRegWord(GPOUTPUT5) & GPIO84_OUT)      
                     return TRUE;
               }
            }
            else//PBP
            {
               if(gm_ReadRegWord(GPOUTPUT5) & GPIO84_OUT)      
                  return TRUE;
            }
         }
         else
         {       
            /*ChengLin 20130415: If timer equal timeout, system will check own ready bit setting status*/           
            if(gm_ReadRegWord(GPOUTPUT5) & GPIO84_OUT)      
               return TRUE;
         }
      }
      else //SST Mode
#endif      
      {    
		   /*ChengLin 20130306: Separate Ready bit status check by PBP mode status*/	
		   if(UserPrefCaptureM != CAPTURE_MODE_HALF_HORIZONTAL)
		   {
			   if(gm_ReadRegWord(GPINPUT5) & GPIO84_OUT)		
				   return TRUE;
		   }
		   else //PBP
		   {
			   if(gm_ReadRegWord(GPOUTPUT5) & GPIO84_OUT)		
				   return TRUE;
		   }
      }
	}

	return FALSE;
}

void Image_output_Request(void)
{
		gm_SetRegBitsWord(GPOUTPUT5, GPIO85_OUT);
#if !ENABLE_ATHENA_R_L_IMAGE_SYNC_HANDLER
/*ChengLin 20130222: Don't to delay in AFR Interrupt*/
		gm_Delay1ms(5);
#endif		
		gm_ClearRegBitsWord(GPOUTPUT5, GPIO85_OUT);
		//	gm_Print("Send Ready signal to R ..... %d", 0);
}
#endif // ST_4K2K_93xx_BOARD


BOOL MemoryCheck_4Kx2K(void)
{
	BOOL result = FALSE;
	
	#ifdef ST_4K2K_93xx_BOARD 
	if(GetChipID_4K2K()==CHIPID_4K2K_L)
	{
      if(DpRxIsMST_Stream(UserPrefCurrentInputMain) == TRUE)
         result = FALSE;
      else if(UserPrefCaptureM == CAPTURE_MODE_HALF_HORIZONTAL) // PBP Mode
         result = FALSE;
      else
      {
         /*ChengLin 20130318: Special for DP SST/FULL Screen Mode & Aspect Ratio is 4:3, Athena Left will show bad image issue*/
         if(((UserPrefOutputAspectRatio == OUTPUT_AR_4_3) && (UserPrefCurrentInputMain == ALL_BOARDS_DP1)) || /*&& (UserPrefDPSWitch == DP_Input_11)*/
             ((UserPrefOutputAspectRatio == OUTPUT_AR_4_3) && (UserPrefCurrentInputMain == ALL_BOARDS_DP2) && (gmvw_InputHActiveOrg[gmvb_MainChannel] > 2880)) )         
           result = FALSE;
         else
            if(gmvw_InputHActiveOrg[gmvb_MainChannel] > gmvw_OutputHActive[gmvb_MainChannel])
               result = FALSE;
            else
		 	   	result = TRUE;
      }
	}
	#endif

	return result;
}

#ifdef ST_4K2K_93xx_BOARD 
#if ENABLE_ATHENA_R_L_IMAGE_SYNC_HANDLER
//******************************************************************************
// FUNCTION:   ImageSyncHandleSetState
// USAGE: Set Current State for Image Sync Handler
//
// INPUT:  ImageState
// OUTPUT: None
// GLOBALS:
//******************************************************************************
void ImageSyncHandleSetState(gmt_ImageSyncState ImageState)
{
	if(GetChipID_4K2K() == CHIPID_4K2K_R)
		gCurrentAthenaRIGHT_SyncState = ImageState;		
	else if(GetChipID_4K2K() == CHIPID_4K2K_L)
		gCurrentAthenaLEFT_FE_SyncState = ImageState;
}

//******************************************************************************
// FUNCTION:   ImageSyncHandleGetState
// USAGE: Get current state for Image Sync Handler
//
// INPUT:  None
// OUTPUT: ImageState
// GLOBALS:
//******************************************************************************
gmt_ImageSyncState ImageSyncHandleGetState(void)
{
	gmt_ImageSyncState ImageState;

	if(GetChipID_4K2K() == CHIPID_4K2K_R)
		ImageState = gCurrentAthenaRIGHT_SyncState;
	else if(GetChipID_4K2K() == CHIPID_4K2K_L)
		ImageState = gCurrentAthenaLEFT_FE_SyncState;

	return ImageState;
}

//******************************************************************************
// FUNCTION:   ImageSyncHandleSetModeStableFlag
// USAGE: Set Signal Stable Status for Image Sync Handler
//
// INPUT:  Stable
// OUTPUT: None
// GLOBALS:
//******************************************************************************
void ImageSyncHandleSetModeStableFlag(BOOL Stable)
{
	if(GetChipID_4K2K() == CHIPID_4K2K_R)
		ImageSyncModeRightStable = Stable;
	else if(GetChipID_4K2K() == CHIPID_4K2K_L)
		ImageSyncModeLeftFeStable = Stable;	
}

//******************************************************************************
// FUNCTION:   ImageSyncHandleGetModeStableFlag
// USAGE: Get Signal Stable Status for Image Sync Handler
//
// INPUT:  None
// OUTPUT: Stable
// GLOBALS:
//******************************************************************************
BOOL ImageSyncHandleGetModeStableFlag(void)
{
	BOOL Stable;

	if(GetChipID_4K2K() == CHIPID_4K2K_R)
		Stable = ImageSyncModeRightStable;
	else if(GetChipID_4K2K() == CHIPID_4K2K_L)
		Stable = ImageSyncModeLeftFeStable;	

	return Stable;
}

//******************************************************************************
// FUNCTION:   ImageSyncHandler
// USAGE: State Machine for Athena Right and Left Image Sync Engine
//
// INPUT:  None
// OUTPUT: None
// GLOBALS:
//******************************************************************************
void ImageSyncHandler(void)
{
	if(GetChipID_4K2K() != CHIPID_4K2K_FE)
	{
		switch(ImageSyncHandleGetState())
		{
   			case IMAGE_LVDS_ON_STATE:
   			case IMAGE_LVDS_OFF_STATE:
				break;
   			case IMAGE_OFF_STATE:                  
				/*ChengLin 20130307: Clear all Flag & Timers first*/			
				gm_TimerStop(IMAGE_SYNC_HANDLER_TIMEOUT_10ms_TMR);         
           #if ENABLE_DP_MST_TIMEOUT_CHECK_FOR_OWN_READY_BIT            
				gm_TimerStop(IMAGE_SYNC_DP_MST_TIMEOUT_10ms_TMR);
            ImageSyncDP_MST_TimeOutCheck = FALSE;
            ImageSync_ForceCheckReadySetting = FALSE;
           #endif
				
				if(ImageSyncHandleGetModeStableFlag() == TRUE) //Stable
				{		
					ImageSyncHandleSetState(IMAGE_RDY_WAIT_STATE);
					gm_TimerStart(IMAGE_SYNC_HANDLER_TIMEOUT_10ms_TMR, IMAGE_SYNC_HANDLER_WAIT_TIME_OUT); //Enable Timer here!!					
				}
				if(GetChipID_4K2K() == CHIPID_4K2K_R)
				{
					/*INT1_Flag==1  && IMG_RDY bit==L*/
					if(IS_Image_Ready() == FALSE && ImageSyncINT1_Flag == TRUE)
					{
						ImageSyncINT1_Flag = FALSE;
						ImageSyncHandleSetState(IMAGE_FAIL_STATE);					
					}
				}
				break;
   			case IMAGE_RDY_WAIT_STATE:           
				if(ImageSyncHandleGetModeStableFlag() == FALSE) //Unstable
				{
					Image_ReadyFlag(FALSE); //Clear Ready Bit			
					gm_TimerStop(IMAGE_SYNC_HANDLER_TIMEOUT_10ms_TMR);        
              #if ENABLE_DP_MST_TIMEOUT_CHECK_FOR_OWN_READY_BIT                
				   gm_TimerStop(IMAGE_SYNC_DP_MST_TIMEOUT_10ms_TMR);
               ImageSyncDP_MST_TimeOutCheck = FALSE;
               ImageSync_ForceCheckReadySetting = FALSE;
              #endif
					ImageSyncHandleSetState(IMAGE_OFF_STATE);
				}

           #if ENABLE_DP_MST_TIMEOUT_CHECK_FOR_OWN_READY_BIT 
            /*ChengLin 20130415: Enanle DP MST Timeout checking timer one time*/
            if((ImageSyncDP_MST_TimeOutCheck == FALSE) && (DpRxIsMST_Stream(UserPrefCurrentInputMain) == TRUE))
            {
               /*ChengLin 20130415: Enable MST ready bit checking timer*/
					gm_TimerStart(IMAGE_SYNC_DP_MST_TIMEOUT_10ms_TMR, IMAGE_SYNC_DP_MST_WAIT_TIME_OUT);
               ImageSyncDP_MST_TimeOutCheck = TRUE;
            }
           #endif
           
				if(IS_Image_Ready() == TRUE)
				{
#if ENABLE_DP_MST_TIMEOUT_CHECK_FOR_OWN_READY_BIT 				
               if(DpRxIsMST_Stream(UserPrefCurrentInputMain) == TRUE) //MST Mode
               {            
                  /*ChengLin 20130415: If timer not equal timeout, system will keep SST method to check ready status*/
                  if(gm_TimerCheck(IMAGE_SYNC_DP_MST_TIMEOUT_10ms_TMR) != TIMER_TMO)
                  {                 
					      /*ChengLin 20130306: Separate Ready bit status check by PBP mode status*/					
					      if(UserPrefCaptureM != CAPTURE_MODE_HALF_HORIZONTAL)
					      {
						      if(GetChipID_4K2K() == CHIPID_4K2K_L)
						      {
							      Image_output_Request(); //Trigger Athena Right INT1
							      Image_Force_OnOff(TRUE);
							      gm_TimerStop(IMAGE_SYNC_HANDLER_TIMEOUT_10ms_TMR);
				               gm_TimerStop(IMAGE_SYNC_DP_MST_TIMEOUT_10ms_TMR);
                           ImageSyncDP_MST_TimeOutCheck = FALSE;
                           ImageSync_ForceCheckReadySetting = FALSE;
							      ImageSyncHandleSetState(IMAGE_ON_STATE);									
						      }
                        else if(GetChipID_4K2K() == CHIPID_4K2K_R)
                        {
                           /*ChengLin 20130320: Athena Right just to check "IMG_RDY" status then change state*/
						         gm_TimerStop(IMAGE_SYNC_HANDLER_TIMEOUT_10ms_TMR);
				               gm_TimerStop(IMAGE_SYNC_DP_MST_TIMEOUT_10ms_TMR);
                           ImageSyncDP_MST_TimeOutCheck = FALSE;
                           ImageSync_ForceCheckReadySetting = FALSE;
						         ImageSyncHandleSetState(IMAGE_ON_STATE);
                        }
					      }
					      else //PBP
					      {
					         #if ENABLE_PBP_ATHENA_L_DEEP_SLEEP_CHECK  
                        if(GetChipID_4K2K() == CHIPID_4K2K_L)
                           gm_TimerStart(PBP_SLEEP_CHECK_TIMEOUT_1ms_TMR, PBP_SLEEP_WAIT_TIME_OUT);
                        #endif
						      Image_Force_OnOff(TRUE);						
						      gm_TimerStop(IMAGE_SYNC_HANDLER_TIMEOUT_10ms_TMR);
				            gm_TimerStop(IMAGE_SYNC_DP_MST_TIMEOUT_10ms_TMR);
                        ImageSyncDP_MST_TimeOutCheck = FALSE;
                        ImageSync_ForceCheckReadySetting = FALSE;                        
						      ImageSyncHandleSetState(IMAGE_ON_STATE);
					      }		
                  }
                  else
                  {                  
                     /*ChengLin 20130415: If timer equal timeout, system will check own ready bit setting status*/
						   Image_Force_OnOff(TRUE);						
						   gm_TimerStop(IMAGE_SYNC_HANDLER_TIMEOUT_10ms_TMR);
				         gm_TimerStop(IMAGE_SYNC_DP_MST_TIMEOUT_10ms_TMR);
                     ImageSyncDP_MST_TimeOutCheck = FALSE;
                     ImageSync_ForceCheckReadySetting = TRUE;                       
						   ImageSyncHandleSetState(IMAGE_ON_STATE);
                  }
               }
               else //SST Mode
#endif               
               {               
                  /*ChengLin 20130306: Separate Ready bit status check by PBP mode status*/              
                  if(UserPrefCaptureM != CAPTURE_MODE_HALF_HORIZONTAL)
                  {
                     if(GetChipID_4K2K() == CHIPID_4K2K_L)
                     {
                        Image_output_Request(); //Trigger Athena Right INT1
                        Image_Force_OnOff(TRUE);
                        gm_TimerStop(IMAGE_SYNC_HANDLER_TIMEOUT_10ms_TMR);
                       #if ENABLE_DP_MST_TIMEOUT_CHECK_FOR_OWN_READY_BIT 	                        
                        gm_TimerStop(IMAGE_SYNC_DP_MST_TIMEOUT_10ms_TMR);
                        ImageSyncDP_MST_TimeOutCheck = FALSE;
                        ImageSync_ForceCheckReadySetting = FALSE;
                       #endif
                        ImageSyncHandleSetState(IMAGE_ON_STATE);                          
                     }
                     else if(GetChipID_4K2K() == CHIPID_4K2K_R)
                     {
                        /*ChengLin 20130320: Athena Right just to check "IMG_RDY" status then change state*/
                        gm_TimerStop(IMAGE_SYNC_HANDLER_TIMEOUT_10ms_TMR);
                       #if ENABLE_DP_MST_TIMEOUT_CHECK_FOR_OWN_READY_BIT                         
                        gm_TimerStop(IMAGE_SYNC_DP_MST_TIMEOUT_10ms_TMR);
                        ImageSyncDP_MST_TimeOutCheck = FALSE;
                        ImageSync_ForceCheckReadySetting = FALSE;
                       #endif
                        ImageSyncHandleSetState(IMAGE_ON_STATE);
                     }
                  }
                  else //PBP
                  {
                     #if ENABLE_PBP_ATHENA_L_DEEP_SLEEP_CHECK  
                     if(GetChipID_4K2K() == CHIPID_4K2K_L)
                        gm_TimerStart(PBP_SLEEP_CHECK_TIMEOUT_1ms_TMR, PBP_SLEEP_WAIT_TIME_OUT);
                     #endif
                  
                     Image_Force_OnOff(TRUE);                  
                     gm_TimerStop(IMAGE_SYNC_HANDLER_TIMEOUT_10ms_TMR);
                    #if ENABLE_DP_MST_TIMEOUT_CHECK_FOR_OWN_READY_BIT                      
                     gm_TimerStop(IMAGE_SYNC_DP_MST_TIMEOUT_10ms_TMR);
                     ImageSyncDP_MST_TimeOutCheck = FALSE;
                     ImageSync_ForceCheckReadySetting = FALSE;
                    #endif 
                     ImageSyncHandleSetState(IMAGE_ON_STATE);
                  }
               }
				}

				if(GetChipID_4K2K() == CHIPID_4K2K_R)
				{
					if( ((IS_Image_Ready() == FALSE) && (ImageSyncHandleGetModeStableFlag() == TRUE)) && \
						(ImageSyncINT1_Flag == TRUE || gm_TimerCheck(IMAGE_SYNC_HANDLER_TIMEOUT_10ms_TMR) == TIMER_TMO))
					{
						/*
						(Wait Time Out || INT1_Flag==True) && Stable  && When IMG_RDY ==L
						*/
						ImageSyncINT1_Flag = FALSE;						
						gm_TimerStop(IMAGE_SYNC_HANDLER_TIMEOUT_10ms_TMR);
                 #if ENABLE_DP_MST_TIMEOUT_CHECK_FOR_OWN_READY_BIT                   
				      gm_TimerStop(IMAGE_SYNC_DP_MST_TIMEOUT_10ms_TMR);
                  ImageSyncDP_MST_TimeOutCheck = FALSE;
                  ImageSync_ForceCheckReadySetting = FALSE;
                 #endif
						ImageSyncHandleSetState(IMAGE_FAIL_STATE);
					}
				}
				else if(GetChipID_4K2K() == CHIPID_4K2K_L)
				{
				//Wait Time Out  && IMG_RDY ==L
					if(IS_Image_Ready() == FALSE && gm_TimerCheck(IMAGE_SYNC_HANDLER_TIMEOUT_10ms_TMR) == TIMER_TMO)
					{
						/*ChengLin 20130307: Only Full Scrren Mode to Black Athena Right*/
                  #if !ENABLE_PBP_ATHENA_L_DEEP_SLEEP_CHECK                    
						if(UserPrefCaptureM != CAPTURE_MODE_HALF_HORIZONTAL)
							Image_output_Request(); //Trigger Athena Right INT1	
						#else
                     if(UserPrefCaptureM != CAPTURE_MODE_HALF_HORIZONTAL)
							   Image_output_Request(); //Trigger Athena Right INT1	
							else              
                        gm_TimerStart(PBP_SLEEP_CHECK_TIMEOUT_1ms_TMR, PBP_SLEEP_WAIT_TIME_OUT);
                  #endif   
				
						gm_TimerStop(IMAGE_SYNC_HANDLER_TIMEOUT_10ms_TMR);
                 #if ENABLE_DP_MST_TIMEOUT_CHECK_FOR_OWN_READY_BIT                    
				      gm_TimerStop(IMAGE_SYNC_DP_MST_TIMEOUT_10ms_TMR);
                  ImageSyncDP_MST_TimeOutCheck = FALSE;
                  ImageSync_ForceCheckReadySetting = FALSE;
                 #endif 
						ImageSyncHandleSetState(IMAGE_FAIL_STATE);						
					}
				}
				break;
   			case IMAGE_ON_STATE:
				if(IS_Image_Ready() == FALSE)
				{
					/*ChengLin 20130307: Clear all Flag & Timers first*/
					gm_TimerStop(IMAGE_SYNC_HANDLER_TIMEOUT_10ms_TMR);
              #if ENABLE_DP_MST_TIMEOUT_CHECK_FOR_OWN_READY_BIT                
				   gm_TimerStop(IMAGE_SYNC_DP_MST_TIMEOUT_10ms_TMR);
               ImageSyncDP_MST_TimeOutCheck = FALSE;
               ImageSync_ForceCheckReadySetting = FALSE; 
              #endif
					ImageSyncHandleSetState(IMAGE_LOST_WAIT_STATE);				
				}	
				break;
   			case IMAGE_LOST_WAIT_STATE:
				if(ImageSyncHandleGetModeStableFlag() == FALSE) //Unstable
				{
					ImageSyncHandleSetState(IMAGE_OFF_STATE);
				}
				else //Stable
				{				
					ImageSyncHandleSetState(IMAGE_RDY_WAIT_STATE);
					gm_TimerStart(IMAGE_SYNC_HANDLER_TIMEOUT_10ms_TMR, IMAGE_SYNC_HANDLER_WAIT_TIME_OUT); //Enable Timer here!!					
				}
				break;
			case IMAGE_FAIL_STATE:           
				if(GetChipID_4K2K() == CHIPID_4K2K_R)
				{
					ImageSyncHandleSetState(IMAGE_OFF_STATE);
				}
				else if(GetChipID_4K2K() == CHIPID_4K2K_L)
				{
					if(ImageSyncHandleGetModeStableFlag() == TRUE && IS_Image_Ready())
					{
						/*ChengLin 20130307: Clear all Flag & Timers first*/						
						gm_TimerStop(IMAGE_SYNC_HANDLER_TIMEOUT_10ms_TMR);
                 #if ENABLE_DP_MST_TIMEOUT_CHECK_FOR_OWN_READY_BIT                  
				      gm_TimerStop(IMAGE_SYNC_DP_MST_TIMEOUT_10ms_TMR);
                  ImageSyncDP_MST_TimeOutCheck = FALSE;
                  ImageSync_ForceCheckReadySetting = FALSE;
                 #endif
						ImageSyncHandleSetState(IMAGE_RDY_WAIT_STATE);
						gm_TimerStart(IMAGE_SYNC_HANDLER_TIMEOUT_10ms_TMR, IMAGE_SYNC_HANDLER_WAIT_TIME_OUT); //Enable Timer here!!				
					}
		
					if(ImageSyncHandleGetModeStableFlag() == FALSE)
					{
						Image_ReadyFlag(FALSE); //Clear Ready Bit
						ImageSyncHandleSetState(IMAGE_OFF_STATE);
					}
				}
				break;
		}
	}
}
#endif
#endif
//******************************************************************************
// FUNCTION:   AFR_Interrupt
// USAGE: Handle AFR Interrupt and process Athena Right/Left Ready Flag
//
// INPUT:  None
// OUTPUT: None
// GLOBALS:
//******************************************************************************
/*ChengLin 20130221: AFR Interrupt Handler*/
void AFR_Interrupt(void)
{
#ifdef ST_4K2K_93xx_BOARD 
#if ENABLE_ATHENA_R_L_IMAGE_SYNC_HANDLER
	if(GetChipID_4K2K() != CHIPID_4K2K_FE)
	{
		if( (ImageSyncHandleGetState() != IMAGE_LVDS_ON_STATE) && (IS_Image_Ready() == TRUE))
		{
			//if(GetChipID_4K2K() == CHIPID_4K2K_R) //Debug Only
			//	KeypadLightControl(KEYPAD_LED_RED, LED_NORMAL);
     
			Image_ReadyFlag(FALSE);		

			/*ChengLin 20130306: PBP Mode control PB_Ctrl by itself, Athena Left don't control Athena Right PB_Ctrl status*/
         #if !ENABLE_PBP_ATHENA_L_DEEP_SLEEP_CHECK                    
            if((GetChipID_4K2K() == CHIPID_4K2K_L) && (UserPrefCaptureM != CAPTURE_MODE_HALF_HORIZONTAL))
               Image_output_Request(); //SET Athena Right INT1 for handle PB_Ctrl status
         #else
            if((GetChipID_4K2K() == CHIPID_4K2K_L))
            {
               if(UserPrefCaptureM != CAPTURE_MODE_HALF_HORIZONTAL)
                  Image_output_Request();
               else
                  gm_TimerStart(PBP_SLEEP_CHECK_TIMEOUT_1ms_TMR, PBP_SLEEP_WAIT_TIME_OUT);  
            }
            else
            {
               if(UserPrefCaptureM == CAPTURE_MODE_HALF_HORIZONTAL)
               {
                  gm_TimerStart(PBP_SLEEP_CHECK_TIMEOUT_1ms_TMR, PBP_SLEEP_WAIT_TIME_OUT);
                  PBP_StartPowerCheck_AthenaR = FALSE;
               }
               #if ENABLE_TWO_LOGICAL_PORTS_SLEEP_CHECK //Add by ChengLin 20130509               
               else
               {               
                  gm_TimerStart(DP_TWO_LOGICAL_PORTS_SLEEP_CHECK_TIMEOUT_1ms_TMR, (NO_SIGNAL_DELAY-1000));
                  DP_TWO_LogicalPorts_StartPowerCheck_AthenaR = FALSE;                
               }
               #endif
            }
         #endif 
		}	
	}
#endif
#endif
}

//******************************************************************************


