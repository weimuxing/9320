/*
    $Workfile:   osd_ports.c  $
    $Revision: 1.41 $
    $Date: 2013/04/18 06:56:40 $
*/
#define __OSD_PORTS_C__
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
// MODULE:         osd_ports.c
//
// USAGE:        This module contains all adjustors that deal with input ports
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

extern void TeletextStop(void);

//******************************************************************************
//  D E F I N E S
//******************************************************************************

//#define _DEBUG

#ifdef _DEBUG
#define  msg(a)                    gm_Print(a,0)
#define  msg_i(a,b)                 gm_Print(a,b)
#else
#define  msg(a)
#define  msg_i(a,b)
#endif

//******************************************************************************
//  E X T E R N    V A R I A B L E S
//******************************************************************************

extern BYTE gB_CurrentInputConnector[];
#if 0 // ndef RD1_8028_BOARD // Not Mars board
extern void GpioHdmiSetDDC_Line(gmt_UserDefConnNames Connection);
#endif	//RD1_8028_BOARD
extern gmt_UserDefConnNames MovePortFromConflictPort(gmt_UserDefConnNames Port);

//******************************************************************************
//  S T A T I C   V A R I A B L E S
//******************************************************************************

static gmt_PHY_CH InputPortChannel;

//******************************************************************************
// S T A T I C   F U N C T I O N S
//******************************************************************************
static BOOL IsPortSCART( gmt_UserDefConnNames Port );
static gmt_UserDefConnNames GetNextSCARTSignal(gmt_UserDefConnNames CurrPort);

#ifdef CHANNEL_CLONE
static void ChannelClone(gmt_PHY_CH B_Channel);
static void CopyUserPrefs(gmt_PHY_CH B_Channel);
static void CopyGlobalData(gmt_PHY_CH B_DestChannel, gmt_PHY_CH B_SrcChannel);
static void UpdateSyncPolarity(gmt_PHY_CH B_Channel);
#endif

static void ResourceResolver(void);

#if 0 //(FEATURE_DISPLAY_PORT == ENABLE)
// 20090411 #1, Added flag for change port wake up for DP.
extern BOOL NonDPPortSleep;
#endif
//******************************************************************************
//  C O D E
//******************************************************************************
//Returns if the specified port is a SCART port
#define IsPortSCART(Port) ((Port >= ALL_BOARDS_SIG1_SCART1) && (Port <= ALL_BOARDS_SIG4_SCART3))

#ifdef ADJUSTER_CURRENTINPUTMAIN_DECLARED

#if 0 //(FEATURE_DISPLAY_PORT == ENABLE)
// 20090411 #1, Added flag for change port wake up for DP.
extern void DpRxSendHpdLongPulse(void);
extern BOOL DpRxLinkStatusDoPulse(void);
#endif
//******************************************************************************
//
// FUNCTION       : void AdjustCurrentInputMain(void)
//
// USAGE          : Source input in Main display is set to value of user preference variable
//                  UserPrefCurrentInputMain. Adjust source input for main display
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : UserPrefCurrentInputMain    (R) - new setting for Main's source input
//                  UserPrefCurrentInputPip    (R/W)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AdjustCurrentInputMain(void)
{
#ifndef NO_PIP_SUPPORT
   gmt_UserDefConnNames inputPipOrig = UserPrefCurrentInputPip;
#endif

   if (gB_CurrentInputConnector[gmvb_MainChannel] == UserPrefCurrentInputMain)
      return;

	//Neil120615+ clear WB store state if change port
	if(UserPrefStateTempFlag != OSD_SCAN_SOURCE)//owen 20120810 
	UserPrefStateTempFlag = DEFAULT; 
	stInput[gmvb_MainChannel].ModeFlags &= ~gmd_MODE_OUT_RANGE;  
	
#ifdef DELL_U2410F//120517 Edward test
	if(UserPrefAudioMainInput == gmd_AUDIO_SRC_AUTO_Main)//120716 Edward for mute audio if audio in the main and would like to change main input
	  AUDIO_MUTE();
#endif
#ifdef PORT_CHANGE_SEAMLESS
	if(IsPortChangeSeamlessOK())
	{
		SetPortChangeStates(PCS_INIT);
		return;
	}
#endif

#ifndef NO_PIP_SUPPORT
   // Change Pip source to be the same as Main if there is a conflict
   if (!IsMainSourceValid(UserPrefCurrentInputMain))
   {
	  #ifdef CHANNEL_CLONE		//Hyb130826* for channel clone mode, set pxp same as main if conflict
	  	UserPrefCurrentInputPip = UserPrefCurrentInputMain;//Neil120616* don't chage to same port
	  #else
		  #ifdef DELL_U2410F
		  	UserPrefCurrentInputPip = MovePortFromConflictPort(UserPrefCurrentInputMain);//Neil120616+
		  #else
	      	UserPrefCurrentInputPip = UserPrefCurrentInputMain;//Neil120616* don't chage to same port
	      #endif
	  #endif
   }
   else
   {
#ifdef CHANNEL_CLONE
      if ((UserPrefCurrentInputMain == UserPrefCurrentInputPip) && (gm_IsModeComplete(gmvb_PipChannel)))
      {
		 if(!IsCloneAvariable(gmvb_PipChannel))
		 	return;
         ModeSetupFreerun(gmvb_MainChannel);
         ChannelClone(gmvb_MainChannel);
          return;
      }
#endif // CHANNEL_CLONE    

   }
#endif //NO_PIP_SUPPORT

   //ResourceResolver();

#ifndef NO_PIP_SUPPORT
   //if PIP input has been changed due to any reason
   if ((inputPipOrig != UserPrefCurrentInputPip)
#ifdef PIP_PORT_INDEPENDENCE
         ||((UserPrefCurrentInputMain != UserPrefCurrentInputPip) && !gm_IsModeComplete(gmvb_PipChannel))
#endif
      )
   {
      msg("SelectInput for PIP");

      // Only AIP need to be config again ...
      if (GetPortProc(UserPrefCurrentInputPip) == AIP)
         SelectInput(gmvb_PipChannel, UserPrefCurrentInputPip, NORMAL_MODE);
   }
#endif //NO_PIP_SUPPORT

#ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()==CHIPID_4K2K_R)
   {
     if(UserPrefCaptureM != CAPTURE_MODE_HALF_HORIZONTAL)
	 {
	   BYTE TempPort=UserPrefCurrentInputMain;
		SocIPCGetSlavePortStatus(SOCIPC_DEVICEL_DST_ADDRESS);
		gm_Delay10ms(1);
/*ChengLin 20130222: Athena Right and Left Must black screen at the same time*/		
#if ENABLE_ATHENA_R_L_IMAGE_SYNC_HANDLER
		SOCIPC_SetAthena_Right_Left_PB_Ctrl_Status(FALSE);
		gm_Delay10ms(1); //Added by ChengLin 20130306 for avoid below I2C warite command failure
#endif
		gm_ClearRegBitsWord(PB_CTRL, PB_MAIN_ENABLE);
		gm_SetRegBitsWord(PB_CTRL, PB_FORCE_MAIN_BKGND);
		gm_ForceUpdate(CH_A , INPUT_OUTPUT_CLOCK);

		if(UserPrefCurrentInputMain == ALL_BOARDS_DP3)
		   UserPrefCurrentInputMain = ALL_BOARDS_DP1;

      /*ChengLin 20130418: Athena L source change timeout check for avoid Athena R hang up for standalone ISP*/
      gm_TimerStart(AthenaL_SourceChange_Timeout_10ms_TMR, 300); //Wait 3 seconds for source change timeout

		while(gAthena_LEFT_FE_Slave_Parameters.PortStatus != UserPrefCurrentInputMain)
		{
         if(gm_TimerCheck(AthenaL_SourceChange_Timeout_10ms_TMR) == TIMER_TMO)
         {
	         gm_TimerStop(AthenaL_SourceChange_Timeout_10ms_TMR);	         
            gm_Print("\n<TimeOut> Athena R wait Athena L source change timeout.\n",0);
            break;
         }
         
			SOCIPC_SyncOSDAdjustHandler( SI_OSD_SYNC_SEL_INPUT_MAIN );
			//gm_Print("L port is %d", gAthena_LEFT_FE_Slave_Parameters.PortStatus);
			gm_Delay10ms(1);
			SocIPCGetSlavePortStatus(SOCIPC_DEVICEL_DST_ADDRESS);			
		}
      gm_TimerStop(AthenaL_SourceChange_Timeout_10ms_TMR);
      UserPrefCurrentInputMain = TempPort;
  	  }
   }
#endif

   SelectInput(gmvb_MainChannel, UserPrefCurrentInputMain, NORMAL_MODE);
	//if(UserPrefPipMode == NO_PIP)
	//Neil 20140819* for OSD abnormal when change port
	if((gB_CurrentInputConnector[gmvb_PipChannel] != UserPrefCurrentInputPip)||(UserPrefPipMode == NO_PIP))
		SelectInput(gmvb_PipChannel, UserPrefCurrentInputPip, NORMAL_MODE);	

#if 0 //FEATURE_DISPLAY_PORT == ENABLE)
   // // 20090411 #1, Added flag for change port wake up for DP, hardcode for sleep from other port, can not wake up in some TX. (If no long pulse).
   //Some DP equipment can't wake after change port from a Nosync port To DP ,bug list 372
#if 1
   if (UserPrefCurrentInputMain == ALL_BOARDS_DP1)
   {
      if (NonDPPortSleep)
      {
         DpRxSendHpdLongPulse();
         NonDPPortSleep = 0;
      }
      else
         DpRxLinkStatusDoPulse();
   }
#endif
#endif

	if (UserPrefCurrentInputMain == ALL_BOARDS_VGA1)
	{
			SetCapabilitiesString(VGAvcpstr, GetCapabilitiesStringSize());
		gm_WriteRegWord(NOISE_CORING_THRESHOLD, 0x20);	//20121127 Kordonm+ for VGA Port, Over Driver Screen Show Phase Noise and Left Size Vertical Line Lighting
	}		
	else
	{
			SetCapabilitiesString(strCapabilitiesRequest, GetCapabilitiesStringSize());
		gm_WriteRegWord(NOISE_CORING_THRESHOLD, 0x0C);	//Origianl Value
	}
}
#endif

#ifdef ADJUSTER_CURRENTINPUTPIP_DECLARED
//******************************************************************************
//
// FUNCTION       : void AdjustCurrentInputPip(void)
//
// USAGE          : Source input in Pip display is set to value of user preference variable
//                  UserPrefCurrentInputPip.  Adjust source input for pip display
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : gmvb_InputConnector    (R)
//                  gmvb_PipChannel        (R)
//                  UserPrefCurrentInputPip (R)    -  new setting for PIP's source input
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AdjustCurrentInputPip(void)
{
#if 0 //(FEATURE_DECODER_TUNER_NUM == 2)        
   if (UserPrefCurrentInputMain == UserPrefCurrentInputPip)
   {   // Prevent both inputs set to the same RF source
      if (UserPrefCurrentInputMain == ALL_BOARDS_RF1)
         UserPrefCurrentInputPip = ALL_BOARDS_RF2;
      else if (UserPrefCurrentInputMain == ALL_BOARDS_RF2)
         UserPrefCurrentInputPip = ALL_BOARDS_RF1;
   }
#endif    //(FEATURE_DECODER_TUNER_NUM == 2)

   
   if (gB_CurrentInputConnector[gmvb_PipChannel] == UserPrefCurrentInputPip)
      return;

//Neil120721+ 
#ifdef DELL_U2410F//120517 Edward test
	if(UserPrefAudioMainInput == gmd_AUDIO_SRC_AUTO_Pxp)//120716 Edward for mute audio if audio in the main and would like to change main input
	  AUDIO_MUTE();
#endif

#ifdef CHANNEL_CLONE
   if ((UserPrefCurrentInputMain == UserPrefCurrentInputPip) &&
         gm_IsModeComplete(gmvb_MainChannel))
   {
	  if(!IsCloneAvariable(gmvb_MainChannel))
		 return;	  
      ModeSetupFreerun(gmvb_PipChannel);
      ChannelClone(gmvb_PipChannel);
   }
   else
#endif // CHANNEL_CLONE
   {
      SelectInput(gmvb_PipChannel, UserPrefCurrentInputPip, NORMAL_MODE);
   }

}
#endif

//******************************************************************************
//
// FUNCTION       : void ChoosePipSetupMethod(void)
//
// USAGE          : Purpose of this function is to determine whether to setup
//                  PIP channel using cloning or normal SelectInput().
//                  If there is a port collision and the Main is stable, or if
//                  both channels are the same source, perform clone for PIP.
//                  If PIP is already setup and complete, don't do anything.
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : gmvb_MainChannel (R)
//                  gmvb_PipChannel (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
#ifdef PIP_PORT_INDEPENDENCE
void ChoosePipSetupMethod(void)
{
   // Check for port collision
   if (!IsMainSourceValid(UserPrefCurrentInputMain))
   {
      UserPrefCurrentInputPip = UserPrefCurrentInputMain;
      if (gm_IsModeComplete(gmvb_MainChannel))
      {
         msg("Show PIP using clone...");
         ChannelClone(gmvb_PipChannel);
      }
      else
      {
         //SelectInput(gmvb_PipChannel, UserPrefCurrentInputPip, NORMAL_MODE);
         //This was creating a transition in the statemachine.This is now commented for testing.
      }
   }
   else if ((UserPrefCurrentInputPip == UserPrefCurrentInputMain) &&
            (!gm_IsValidSignal(gmvb_PipChannel)))
   {
      msg("Show PIP using clone (no port collision)...");
      if (gm_IsValidSignal(gmvb_MainChannel))
         ChannelClone(gmvb_PipChannel);
   }
   else if (!gm_IsValidSignal(gmvb_PipChannel))
   {
      //SelectInput(gmvb_PipChannel, UserPrefCurrentInputPip, NORMAL_MODE);
      //This was creating a transition in the statemachine.This is now commented for testing.
   }
}
#endif

//******************************************************************************
//
// FUNCTION       : void DisablePipModeDetection(void)
//
// USAGE          : Disable mode detection on the PIP channel by forcing
//                  detection state to DISABLE. Also, the COMPLETE and VALID
//                  mode bits are cleared.
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : gmvb_PipChannel (R)
//                  gmvw_InputModeAttr (W)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
#ifdef PIP_PORT_INDEPENDENCE
void DisablePipModeDetection(void)
{
   CLRBIT(stInput[gmvb_PipChannel].ModeFlags, gmd_MODE_COMPLETE);
   CLRBIT(stInput[gmvb_PipChannel].ModeFlags, gmd_MODE_VALID);
   gm_StateDetectionConfig(gmvb_PipChannel, DET_DISABLE);
}
#endif

//******************************************************************************
//
// FUNCTION       : static void ChannelClone(gmt_PHY_CH B_Channel)
//
// USAGE          : Perform setup of specified channel to have same input as
//                  the other channel. Mode detection is skipped.
//
// INPUT          : B_Channel - target of the clone procedure
//                  (i.e. channel that will be the same as the other channel)
//
// OUTPUT         : None
//
// GLOBALS        : gmvw_InputHTotal (R)
//
// USED_REGS      : IMP_SOURCE (R/W)
//                  IPP_SOURCE (R/W)
//                  AFR_CONTROL (W)
//                  AUTO_BKGND_CONTROL (W)
//
// PRE_CONDITION  : The channel to be cloned must be valid and complete.
//
// POST_CONDITION : Both channels will be valid and complete to same input.
//
//******************************************************************************
#ifdef CHANNEL_CLONE
static void ChannelClone(gmt_PHY_CH B_Channel)
{
   msg_i("Cloning channel: %d", gm_OtherChannel(B_Channel));

   // Clone OSD UserPrefs and save settings
   CopyUserPrefs(B_Channel);
   SaveModeIndependentSettings();

   // Copy global data for channel
   CopyGlobalData(B_Channel, gm_OtherChannel(B_Channel));

   // Remove SDDS AFR display event if the DDDS is locked to a channel that
   // is not an AIP source. It is causing Main to detect the event when it
   // occurs for PIP channel, since AFR is checked if channel source is HDMI_DVIP.
   if (gm_IsInputHdmiDvi(B_Channel) || gm_IsInputDP(B_Channel))
   {
      gm_SetAfrControl(AFR_DISP_SDDS_ERR_EN, FALSE);
   }

   // Select input without touching the frontend
   SelectInput(B_Channel,
               (B_Channel == gmvb_MainChannel) ? UserPrefCurrentInputMain : UserPrefCurrentInputPip, CLONE_CH_MODE);

   gmvb_SyncDetectClamp[gmd_ADC_0] = TRUE;
   ModeSetPrefs(B_Channel);

   // Clock selection and color space setup
   //gm_ClockSelection(B_Channel);
   ModeSetupUserPref(B_Channel);
   SetupColorSpace(B_Channel);

   //Update the sync polarity same as the source channel.This function should be strictly
   //called before updating the Capture values. DO NOT CHANGE THE SEQUENCE
   UpdateSyncPolarity( B_Channel);

   // Run simplified mode setup
   ModeSetupInputChange(B_Channel);
   ModeSetupRequestCancel(B_Channel, REQ_MS_BLENDER, PARAM_NONE);
   if (B_Channel ==  gmvb_MainChannel)
      B_SignalStatus = STATUS_SYNC_PRESENT;

   // Reset status registers
   if (B_Channel ==  gmvb_MainChannel)
   {
      gm_WriteRegWord(MAIN_INPUT_STATUS, 0xFFFF);
   }
   else
   {
      gm_WriteRegWord(PIP_INPUT_STATUS, 0xFFFF);
   }

   // Run memory and scalar setup
   gm_MemorySetup(B_Channel);
   gm_ScalerSetup(B_Channel);

   // Clear previous AFR events that were triggered during input selection
   gm_AfrRestart(B_Channel);

   OSDRequestCancelMsg(B_Channel, MTO_NoSync);
   OSDRequestCancelMsg(B_Channel, MTO_NoSignal);
#ifdef CHECK_IS_CABLE_CONNECT
   OSDRequestCancelMsg(B_Channel, MTO_NoCable); // SCR2456
#endif
   OSDRequestCancelMsg(B_Channel, MTO_InvalidMode);
   OSDRequestMsg(B_Channel, MTO_ValidMode, REQUEST_NOW);

   // Turn on blender request
   ModeSetupBlender(B_Channel, DISPLAY_DEFER_TMO_NORMAL * 2);

   //Adjust the Display according to changed Main input timing.
   DisplayLockSource();

   //
   // Display Timing
   //
   // On Cortez-Plus display clock seems to need to change gradually when switching between 50Hz and 60Hz;
   // otherwise, panel will flicker.

   DisplayClockLock(B_Channel, 5);

}
#endif    //#ifdef CHANNEL_CLONE

//******************************************************************************
//
// FUNCTION       : static void CopyGlobalData(gmt_PHY_CH B_DestChannel,
//                     gmt_PHY_CH B_SrcChannel)
//
// USAGE          : Copies global data from source channel to destination
//                  channel.
//
// INPUT          : B_DestChannel - destination channel
//                  B_SrcChannel - source channel
//
// OUTPUT         : None
//
// GLOBALS        : gmvb_InputConnector (R/W)
//                  gB_CurrentInputConnector (R/W)
//                  st_InputMeasurement (R/W)
//                  gmvw_InputVFreq (R/W)
//                  gmvw_InputHFreq (R/W)
//                  gmvw_InputModeAttr (R/W)
//                  gmvw_InputVTotalOrg (R/W)
//                  gmvw_InputHTotalOrg (R/W)
//                  gmvw_InputVStartOrg (R/W)
//                  gmvw_InputVActiveOrg (R/W)
//                  gmvw_InputHStartOrg (R/W)
//                  gmvw_InputHActiveOrg (R/W)
//                  gmvw_InputVTotal (R/W)
//                  gmvw_InputHTotal (R/W)
//                  gmvw_InputVStart (R/W)
//                  gmvw_InputVActive (R/W)
//                  gmvw_InputHStart (R/W)
//                  gmvw_InputHActive (R/W)
//                  gmvw_InputVTotal (R/W)
//                  gmvw_InputHTotal (R/W)
//                  gmvb_ChannelSource (R/W)
//                  gmvw_InputSyncFlags (R/W)
//                  gmvw_InputInterlace (R/W)
//                  B_ModeIndex (R/W)
//                  gmvw_InputOversample (R/W)
//                  W_OverscanEnabled (R/W)
//                  gmvb_VCRMode (R/W)
//                  B_FreeRun (R/W)
//                  B_AutoAdjustStatus (R/W)
//                  B_AutoAdjustStarted (R/W)
//                  gmvb_CurrentSyncState (R/W)
//                  gmvb_EnableClamp (R/W)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
#ifdef CHANNEL_CLONE
static void CopyGlobalData(gmt_PHY_CH B_DestChannel, gmt_PHY_CH B_SrcChannel)
{
   gmvb_InputConnector[B_DestChannel] = gmvb_InputConnector[B_SrcChannel];
   gB_CurrentInputConnector[B_DestChannel] = gB_CurrentInputConnector[B_SrcChannel];

   stInput[B_DestChannel] = stInput[B_SrcChannel];
   stInput[B_DestChannel].ModeFlags = stInput[B_SrcChannel].ModeFlags;

   gmvw_InputVTotalOrg[B_DestChannel] = gmvw_InputVTotalOrg[B_SrcChannel];
   gmvw_InputHTotalOrg[B_DestChannel] = gmvw_InputHTotalOrg[B_SrcChannel];
   gmvw_InputVStartOrg[B_DestChannel] = gmvw_InputVStartOrg[B_SrcChannel];
   gmvw_InputVActiveOrg[B_DestChannel] = gmvw_InputVActiveOrg[B_SrcChannel];
   gmvw_InputHStartOrg[B_DestChannel] = gmvw_InputHStartOrg[B_SrcChannel];
   gmvw_InputHActiveOrg[B_DestChannel] = gmvw_InputHActiveOrg[B_SrcChannel];

   // Copy the original input values so as not to copy any cropped ones
   stInput[B_DestChannel].Vtotal = gmvw_InputVTotalOrg[B_SrcChannel];
   stInput[B_DestChannel].Htotal = gmvw_InputHTotalOrg[B_SrcChannel];
   stInput[B_DestChannel].Vstart = gmvw_InputVStartOrg[B_SrcChannel];
   stInput[B_DestChannel].Vactive = gmvw_InputVActiveOrg[B_SrcChannel];
   stInput[B_DestChannel].Hstart = gmvw_InputHStartOrg[B_SrcChannel];
   stInput[B_DestChannel].Hactive = gmvw_InputHActiveOrg[B_SrcChannel];

   gmvb_ChannelSource[B_DestChannel] = gmvb_ChannelSource[B_SrcChannel];
   gmvw_InputSyncFlags[B_DestChannel] = gmvw_InputSyncFlags[B_SrcChannel];
   gmvw_InputOversample[B_DestChannel] = gmvw_InputOversample[B_SrcChannel];
   W_OverscanEnabled[B_DestChannel] = W_OverscanEnabled[B_SrcChannel];
   gmvb_VCRMode[B_DestChannel] = gmvb_VCRMode[B_SrcChannel];

   B_FreeRun[B_DestChannel] = B_FreeRun[B_SrcChannel];
   B_AutoAdjustStatus[B_DestChannel] = B_AutoAdjustStatus[B_SrcChannel];
   B_AutoAdjustCount[B_DestChannel] = B_AutoAdjustCount[B_SrcChannel];
   B_AutoAdjustStarted[B_DestChannel] = B_AutoAdjustStarted[B_SrcChannel];
   gmvb_InputSamplingPhase[B_DestChannel] = gmvb_InputSamplingPhase[B_SrcChannel];
   gmvsw_InputHSyncOffset[B_DestChannel] = gmvsw_InputHSyncOffset[B_SrcChannel];
   gmvsw_InputVSyncOffset[B_DestChannel] = gmvsw_InputVSyncOffset[B_SrcChannel];

   B_AspectMethod[B_DestChannel] = B_AspectMethod[B_SrcChannel];
   gmvb_STABLE_COUNT[B_DestChannel] = gmvb_STABLE_COUNT[B_SrcChannel];
   gmvb_UNSTABLE_COUNT[B_DestChannel] = gmvb_UNSTABLE_COUNT[B_SrcChannel];
}
#endif    //#ifdef CHANNEL_CLONE

//******************************************************************************
//
// FUNCTION       : static void CopyUserPrefs(gmt_PHY_CH B_Channel)
//
// USAGE          : Overwrites UserPrefs for specified channel with UserPrefs
//                  from the other channel.
//
// INPUT          : B_Channel - UserPrefs to overwrite
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
#ifdef CHANNEL_CLONE
static void CopyUserPrefs(gmt_PHY_CH B_Channel)
{
   if (B_Channel == gmvb_MainChannel)
   {
      UserPrefSharpness = UserPrefSharpnessPip;
      UserPrefSharpnessRGB = UserPrefSharpnessRGBpip;
      UserPrefHSyncDelay = UserPrefHSyncDelay_PIP;
      UserPrefVSyncDelay = UserPrefVSyncDelay_PIP;
   }
   else
   {
      UserPrefSharpnessPip = UserPrefSharpness;
      UserPrefSharpnessRGBpip = UserPrefSharpnessRGB;
      UserPrefHSyncDelay_PIP = UserPrefHSyncDelay;
      UserPrefVSyncDelay_PIP = UserPrefVSyncDelay;
   }
}
#endif    //#ifdef CHANNEL_CLONE

//******************************************************************************
//
// FUNCTION       : BOOL IsInputVGA(gmt_PHY_CH B_Channel)
//
// USAGE          : Returns if current input is a VGA input
//
// INPUT          : Channel
//
// OUTPUT         : TRUE or FALSE
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
BOOL IsInputVGA(gmt_PHY_CH B_Channel)
{
   if (B_Channel == gmvb_MainChannel)
      return (GetPortSignalType(UserPrefCurrentInputMain) == SIG_VGA) ? TRUE :FALSE;
   else if (B_Channel == gmvb_PipChannel)
      return (GetPortSignalType(UserPrefCurrentInputPip) == SIG_VGA) ? TRUE :FALSE;
   else   
      return FALSE;
}

//******************************************************************************
//
// FUNCTION       : BOOL IsVgaOnScreen(void)
//
// USAGE          : Returns if VGA displayed on screen
//
// INPUT          : None
//
// OUTPUT         : TRUE or FALSE
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
BOOL IsVgaOnScreen(void)
{
   if ((GetPortSignalType(UserPrefCurrentInputMain) == SIG_VGA) && (gm_IsValidSignal(gmvb_MainChannel)))
      return TRUE;
   else if ((GetPortSignalType(UserPrefCurrentInputPip) == SIG_VGA) && (gm_IsValidSignal(gmvb_PipChannel)) && (UserPrefPipMode != NO_PIP))
      return TRUE;
   else   
      return FALSE;
}

//******************************************************************************
//
// FUNCTION       : BOOL IsInputDigital(BYTE B_Channel)
//
// USAGE          : Returns if current input is a digital input
//
// INPUT          : Channel
//
// OUTPUT         : TRUE or FALSE
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
BOOL IsInputDigital(BYTE B_Channel)
{
   BYTE B_InputPort;
   
   if (B_Channel == gmvb_MainChannel)
      B_InputPort = UserPrefCurrentInputMain;
   else
      B_InputPort = UserPrefCurrentInputPip;
      
   if ((GetPortSignalType(B_InputPort) == SIG_DVI) || (GetPortSignalType(B_InputPort) == SIG_DP))
      return TRUE;

   return FALSE;
}

//******************************************************************************
//
// FUNCTION       : gmt_UserDefConnNames GetNextSCARTPort(gmt_UserDefConnNames CurrPort)
//
// USAGE          : Returns the next SCART port
//
// INPUT          : CurrPort - current SCART port
//
// OUTPUT         : Next valid port for SCART
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
gmt_UserDefConnNames GetNextSCARTPort(gmt_UserDefConnNames CurrPort)
{
#if 0
   return
      (CurrPort <= ALL_BOARDS_SIG1_SCART1) ? // non-SCART -> SCART1
      ((UserPrefScart1Config == 0) ?
       ALL_BOARDS_SIG1_SCART1 :
       (UserPrefScartRgbViaOverlay && (UserPrefScart1Config == 4)) ?
       (ALL_BOARDS_SIG1_SCART1 + UserPrefScart1Config - 2) :
       (ALL_BOARDS_SIG1_SCART1 + UserPrefScart1Config - 1)) :
            (CurrPort <= ALL_BOARDS_SIG1_SCART2) ? // SCART1 -> SCART2
            ((UserPrefScart2Config == 0) ?
             ALL_BOARDS_SIG1_SCART2 :
             (ALL_BOARDS_SIG1_SCART2 + UserPrefScart2Config - 1)) :
            (CurrPort <= ALL_BOARDS_SIG1_SCART3) ? // SCART2 -> SCART3
            ALL_BOARDS_SIG1_SCART3 :
            (ALL_BOARDS_SIG4_SCART3 + 1); // SCART3 -> non-SCART
#else
   CurrPort = CurrPort;
   return 0;
#endif
}

//******************************************************************************
//
// FUNCTION       : gmt_UserDefConnNames GetPrevSCARTPort(gmt_UserDefConnNames CurrPort)
//
// USAGE          : Returns the previous SCART port
//
// INPUT          : CurrPort - current SCART port
//
// OUTPUT         : Previous valid port for SCART
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
gmt_UserDefConnNames GetPrevSCARTPort(gmt_UserDefConnNames CurrPort)
{
#if 0
   gmt_UserDefConnNames PrevScartPort = CurrPort;

   // non-SCART -> SCART3
   if ( CurrPort > ALL_BOARDS_SIG1_SCART3)
   {
      PrevScartPort = ALL_BOARDS_SIG1_SCART3;
   }
   // SCART3 -> SCART2
   else if ( (CurrPort <=ALL_BOARDS_SIG1_SCART3) && (CurrPort > ALL_BOARDS_SIG1_SCART2))
   {
      PrevScartPort = ALL_BOARDS_SIG1_SCART2;

      if (UserPrefScart2Config > 0)
         PrevScartPort += UserPrefScart2Config - 1;
   }
   // SCART2 -> SCART1
   else if ( (CurrPort <=ALL_BOARDS_SIG1_SCART2) && (CurrPort > ALL_BOARDS_SIG1_SCART1))
   {
      PrevScartPort = ALL_BOARDS_SIG1_SCART1;

      if (UserPrefScart1Config > 0)
      {
         if ((UserPrefScartRgbViaOverlay) && (UserPrefScart1Config == 4))
            PrevScartPort += UserPrefScart1Config - 2; // select overlay channel for rgb
         else
            PrevScartPort += UserPrefScart1Config - 1;
      }
   }
   // SCART1 -> non- SCART
   else if ( CurrPort <= ALL_BOARDS_SIG1_SCART1 )
   {
      PrevScartPort = ALL_BOARDS_SIG1_SCART1 - 1;
   }

   return PrevScartPort;
#else
   CurrPort = CurrPort;
   return CurrPort;
#endif
}


//******************************************************************************
//
// FUNCTION       : gmt_UserDefConnNames GetNextSCARTSignal(gmt_UserDefConnNames CurrPort)
//
// USAGE          : Returns the next SCART signal of the same SCART port
//
// INPUT          : CurrPort - the SCART port
//
// OUTPUT         : Next SCART signal
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
gmt_UserDefConnNames GetNextSCARTSignal(gmt_UserDefConnNames CurrPort)
{
#if 0
   gmt_UserDefConnNames NextScartPort = CurrPort;

   // SCART1
   if ((CurrPort >= ALL_BOARDS_SIG1_SCART1) &&  (CurrPort <= ALL_BOARDS_SIG4_SCART1))
   {
      NextScartPort = ALL_BOARDS_SIG1_SCART1;
      if (UserPrefScart1Config > 0)
      {
         if ((UserPrefScartRgbViaOverlay) && (UserPrefScart1Config == 4))
            NextScartPort += UserPrefScart1Config - 2; // select overlay channel for rgb
         else
            NextScartPort += UserPrefScart1Config - 1;
      }
   }
   // SCART2
   else if ((CurrPort >= ALL_BOARDS_SIG1_SCART2) &&  (CurrPort <= ALL_BOARDS_SIG4_SCART2))
   {
      NextScartPort = ALL_BOARDS_SIG1_SCART2;
      if (UserPrefScart2Config > 0)
         NextScartPort += UserPrefScart2Config - 1;
   }

   return NextScartPort;
#else
   CurrPort = CurrPort;
   return CurrPort;
#endif
}

#if 0
//******************************************************************************
static BOOL isPortNonSelectable(gmt_UserDefConnNames Port)
{
   return (GetPortConnName(Port) == NOT_USED);
}

//******************************************************************************
static BOOL isMainNonSelectable(gmt_UserDefConnNames Port)
{
   return /*(isPortNonSelectable(Port) ||*/
      (!IsMainSourceValid(Port));
}
#endif
//******************************************************************************
//
// FUNCTION       : gmt_UserDefConnNames GetNextMainInputPort(gmt_UserDefConnNames CurrentPort)
//
// USAGE          : Returns the next input port for the main channel when user changes port
//
// INPUT          : CurrentPort
//
// OUTPUT         : Next valid port for Main
//
// GLOBALS        : gmvs_InputConfigTbl
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
gmt_UserDefConnNames GetNextMainInputPort(gmt_UserDefConnNames Port)
{
#ifdef ST_4K2K_93xx_BOARD
   Port= UserPrefCurrentInputMain;
   if(GetChipID_4K2K()==CHIPID_4K2K_R)
   {
      if(Port==ALL_BOARDS_DP1)
         Port=ALL_BOARDS_DP3;
      else if(Port==ALL_BOARDS_DP3)
         Port=ALL_BOARDS_DP2;
      else if(Port==ALL_BOARDS_DP2)
         Port=ALL_BOARDS_DP1;
   }
   //gm_Print("*****GetNextMainPort=%d",Port);
   return Port;
#else
   do
   {
      Port = (Port+1) % NUM_OF_CONNECTORS;
   } while (!IsMainSourceValid(Port));
   return(Port);
#endif   
}

//******************************************************************************
//
// FUNCTION       : gmt_UserDefConnNames GetPrevMainInputPort(gmt_UserDefConnNames CurrentPort)
//
// USAGE          : Returns the previous input port for the main channel when user changes port
//
// INPUT          : CurrentPort
//
// OUTPUT         : Previous valid port for Main
//
// GLOBALS        : gmvs_InputConfigTbl
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
gmt_UserDefConnNames GetPrevMainInputPort(gmt_UserDefConnNames Port)
{
   do
   {
      Port = (Port) ? (Port-1) : (NUM_OF_CONNECTORS - 1);
   } while (!IsMainSourceValid(Port));
   return(Port);
}

//******************************************************************************
//
// FUNCTION       : void InputPortSelectionChannelDeterm(void)
//
// USAGE          : Function used to set static variable as to whether the input port selection will be
//                for Main or Pip
//
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
void InputPortSelectionChannelDeterm (BYTE InputPortSelection)
{
   //BYTE InputPortSelection = gm_OsdGetActionByteParameter();

   if (InputPortSelection == TVMain)
      InputPortChannel = gmvb_MainChannel;
   else if (InputPortSelection == TVPip)
      InputPortChannel = gmvb_PipChannel;
}



//******************************************************************************
//
// FUNCTION       : void InputPortListScroll(void)
//
// USAGE          : Function used to scroll input port list up or down.
//
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

void InputPortListScroll(void)
{
   //**IMPORTANT: In OSD Workbench, the values of UserPrefInputPortList are
   //ALL assigned initially to a dummy value of 99

   // Get action from OSD
   BYTE OSD_Action = gm_OsdGetActionByteParameter();
   BYTE i = 0;
   BYTE j = 0;
   SBYTE SB_Offset = 0;
   gmt_UserDefConnNames tempInputPort; //dummy value initialization

   // Set up offset to either increment or decrement port input type
   if (OSD_Action == DOWN)
   {
      SB_Offset = 1;
   }
   else if (OSD_Action == UP)
   {
      SB_Offset = 2;
   }
   else
      //determines input port channel
   {
      SB_Offset = 0;
      InputPortSelectionChannelDeterm(OSD_Action);
   }

   if (InputPortChannel == gmvb_MainChannel)
   {
      //Initial Population of input port list
      if (SB_Offset == 0)
      {
         //Resetting the Input Port List to default value
         for (j = 0; j < 5; j++)
         {
            InputPortList = j;
            UserPrefInputPortList = 0x63;
         }

         //Count how many elements in the input port list. Stops counting after 5 times.
         //Note: OSD Menu set up to only hold a maximum of 5 entries
         UserPrefInputPortCount = 0;

         //Check to see if there are 5 valid ports, if not, store the number in UserPrefInputPortCount
         tempInputPort = UserPrefCurrentInputMain;
         do
         {
            tempInputPort = GetNextMainInputPort(tempInputPort);
            UserPrefInputPortCount += 1;
         } while ( ( tempInputPort != UserPrefCurrentInputMain) && ( UserPrefInputPortCount < 5));

         //Current input port assignment
         InputPortList = 0;
         UserPrefInputPortList = UserPrefCurrentInputMain;

         for (i = 1; i < UserPrefInputPortCount; i++)
         {
            //Takes a look at the previous entry, stores it into temp input define.
            //The current location on the input port list is then assigned to this temp variable
            InputPortList = i-1;
            tempInputPort = GetNextMainInputPort(UserPrefInputPortList);
            InputPortList = i;
            UserPrefInputPortList = tempInputPort;
         }
      }
      //Increment port input in the list
      else if (SB_Offset == 1)
      {
         for (i = 0; i < UserPrefInputPortCount; i++)
         {
            InputPortList = i;
            UserPrefInputPortList = GetNextMainInputPort(UserPrefInputPortList);
         }
      }
      //Decrement port input in the list
      else
      {
         for (i = 0; i < UserPrefInputPortCount; i++)
         {
            InputPortList = i;
            UserPrefInputPortList = GetPrevMainInputPort(UserPrefInputPortList);
         }
      }
   }
   else if (InputPortChannel == gmvb_PipChannel)
   {
      //Initial Population of input port list
      if (SB_Offset == 0)
      {
         //Resetting the Input Port List to default value
         for (i = 0 ; j < 5; j++)
         {
            InputPortList = j;
            UserPrefInputPortList = 0x63;
         }

         //Count how many elements in the input port list. Stops counting after 5 times.
         //Note: OSD Menu set up to only hold a maximum of 5 entries
         UserPrefInputPortCount = 0;

         //Check to see if there are 5 valid ports, if not, store the number in UserPrefInputPortCount
         tempInputPort = UserPrefCurrentInputPip;
         do
         {
            tempInputPort = GetNextPipInputPort(tempInputPort);
            UserPrefInputPortCount += 1;
         } while ((tempInputPort != UserPrefCurrentInputPip) && (UserPrefInputPortCount < 5));

         //Current input port assignment
         InputPortList = i;
         UserPrefInputPortList = UserPrefCurrentInputPip;

         for (i = 1; i < UserPrefInputPortCount; i++)
         {
            //Takes a look at the previous entry, stores it into temp input define.
            //The current location on the input port list is then assigned to this temp variable
            InputPortList = i-1;
            tempInputPort = GetNextPipInputPort(UserPrefInputPortList);
            InputPortList = i;
            UserPrefInputPortList = tempInputPort;
         }
      }
      //Increment port input in the list
      else if (SB_Offset == 1)
      {
         for (i = 0; i < UserPrefInputPortCount; i++)
         {
            InputPortList = i;
            UserPrefInputPortList = GetNextPipInputPort(UserPrefInputPortList);
         }
      }
      //Decrement port input in the list
      else
      {
         for (i = 0; i < UserPrefInputPortCount; i++)
         {
            InputPortList = i;
            UserPrefInputPortList = GetPrevPipInputPort(UserPrefInputPortList);
         }
      }
   }

}

//******************************************************************************
//
// FUNCTION       : gmt_UserDefConnNames GetNextPipInputPort(gmt_UserDefConnNames CurrentPort)
//
// USAGE          : Returns the next input port for the pip channel when user changes port
//
// INPUT          : CurrentPort
//
// OUTPUT         : Next valid port for Main
//
// GLOBALS        : gmvs_InputConfigTbl
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
gmt_UserDefConnNames GetNextPipInputPort(gmt_UserDefConnNames Port)
{
   do
   {
      Port = (Port+1) % NUM_OF_CONNECTORS;
   } while (!IsPipSourceValid(Port));
   return(Port);
}

//******************************************************************************
//
// FUNCTION       : gmt_UserDefConnNames GetPrevPipInputPort(gmt_UserDefConnNames CurrentPort)
//
// USAGE          : Returns the previous input port for the pip channel when user changes port
//
// INPUT          : CurrentPort
//
// OUTPUT         : Previous valid port for Pip
//
// GLOBALS        : gmvs_InputConfigTbl
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
gmt_UserDefConnNames GetPrevPipInputPort(gmt_UserDefConnNames Port)
{
   do
   {
      Port = (Port) ? (Port-1) : (NUM_OF_CONNECTORS - 1);
   } while (!IsPipSourceValid(Port));
   return(Port);
}


//******************************************************************************
//
// FUNCTION       : BOOL IsSystemPal()
//
// USAGE          : Checks if the system supports PAL
//
// INPUT          :
//
// OUTPUT         : TRUE or FALSE
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
BOOL IsSystemPal()
{
#ifdef LCDTV_PAL    // PAL tuner
   return TRUE;
   /*
       //
       // Multi-System Tuner
       //
       #elif defined (TUNER_MULTI_SYSTEM_SUPPORT)
           #ifdef UserPrefTunerType
               if (UserPrefTunerType == TUNER_PAL)
                   return TRUE;
               else
                   return FALSE;
           #endif
   */
#else
   return FALSE;    // NTSC tuner only
#endif
}

//******************************************************************************
//
// FUNCTION       : BOOL IsMultiSystem()
//
// USAGE          : Checks if multisystem support is available
//
// INPUT          :
//
// OUTPUT         : TRUE or FALSE
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
BOOL IsMultiSystem()
{
#ifdef TUNER_MULTI_SYSTEM_SUPPORT
   return TRUE;
#else
   return FALSE;
#endif
}

//******************************************************************************
//
// FUNCTION       : gmt_UserDefConnNames AutoScanSvideoDefault(gmt_UserDefConnNames CurrentPort)
//
// USAGE          : If AUTOSCAN_SVIDEO_BY_DEFAULT returns
//                  ALL_BOARDS_SVIDEOx instead of ALL_BOARDS_CVBSx.
//                  Force CVBSx to SVideo input port.
//
// INPUT          : CurrentPort
//
// OUTPUT         : NextPort
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
gmt_UserDefConnNames AutoScanSvideoDefault(gmt_UserDefConnNames CurrentPort)
{
   gmt_UserDefConnNames NextPort;

   NextPort = CurrentPort;

#ifdef AUTOSCAN_SVIDEO_BY_DEFAULT

   if (IsAutoScanDefined())
   {
      // assume if AutoScan is defined every CVBS has an S-VIDEO pair
      switch ( NextPort )
      {
         case ALL_BOARDS_CVBS1:
            NextPort = ALL_BOARDS_SVIDEO1;
            gm_TimerStart(AUTOSCAN_CHA_10ms_TMR, AUTOSCAN_CYCLE);
            gm_TimerStart(AUTOSCAN_CHB_10ms_TMR, AUTOSCAN_CYCLE);
            break;
         case ALL_BOARDS_CVBS2:
            NextPort = ALL_BOARDS_SVIDEO2;
            gm_TimerStart(AUTOSCAN_CHA_10ms_TMR, AUTOSCAN_CYCLE);
            gm_TimerStart(AUTOSCAN_CHB_10ms_TMR, AUTOSCAN_CYCLE);
            break;
         case ALL_BOARDS_CVBS3:
            NextPort = ALL_BOARDS_SVIDEO3;
            gm_TimerStart(AUTOSCAN_CHA_10ms_TMR, AUTOSCAN_CYCLE);
            gm_TimerStart(AUTOSCAN_CHB_10ms_TMR, AUTOSCAN_CYCLE);
            break;
         default:
            break;
      }
   }

#endif

   return NextPort;
}


//******************************************************************************
//
// FUNCTION       : static void UpdateSyncPolarity(gmt_PHY_CH B_Channel)
//
// USAGE          : Programes the HS_INV and VS_INV during cloning.
//
// INPUT          : B_Channel
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
static void UpdateSyncPolarity(gmt_PHY_CH B_Channel)
{
   if ( B_Channel == CH_A )
   {
      WORD W_HSVS_Status = (gm_ReadRegWord(IPP_SOURCE) & ( IPP_HS_INV | IPP_VS_INV )) ;
      WORD W_MaskHSVS_Status =      (gm_ReadRegWord(IMP_SOURCE) & (~( IMP_HS_INV | IMP_VS_INV ))) ;


      gm_WriteRegWord(IMP_SOURCE, W_HSVS_Status| W_MaskHSVS_Status);

   }
   else
   {
      WORD W_HSVS_Status = (gm_ReadRegWord(IMP_SOURCE) & ( IMP_HS_INV | IMP_VS_INV )) ;
      WORD W_MaskHSVS_Status =      (gm_ReadRegWord(IPP_SOURCE) & (~( IPP_HS_INV | IPP_VS_INV ))) ;


      gm_WriteRegWord(IPP_SOURCE, W_HSVS_Status|W_MaskHSVS_Status);
   }
}

#if 1//defined(Qisda_93XX_BOARD) //KevinL 20120430 DP Switch Control
//******************************************************************************
//
// FUNCTION       : void Input_DP_Switch_Select(DP_Port_Types B_DP_Type)
//
// USAGE          : LPM GPIO 0 output for DP switch 0: Mini DP 1 : DP
//
// INPUT          : B_Channel
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
void Input_DP_Switch_Select(DP_Port_Types B_DP_Type)
{
   WORD msg_data[3];

   msg_data[1] = LPM_GPOUTPUT_0;
   msg_data[2] = LPM_GPIO0_OUT;

   if (B_DP_Type == DP_NORMAL)
   {
      gm_Print("LPM GPIO0 : 1 => DP_NORMAL",0);

      msg_data[0] = Set_bit;
      
     if(gm_MessageSend(gmd_MESSAGE_LPM_GPIO_CTRL, msg_data, sizeof(msg_data)) == gmd_FALSE)
      {
			static DWORD DW_BreakTime ;
			DW_BreakTime = gm_GetSystemTime();
			while(1)
			{
				if (abs((WORD)(gm_GetSystemTime() - DW_BreakTime)) > 500)
					break;
				if(gm_MessageSend(gmd_MESSAGE_LPM_GPIO_CTRL, msg_data, sizeof(msg_data)))
					break;
			}
	  }
   }
   else // Mini DP
   {
      gm_Print("LPM GPIO0 : 0 => DP_MINI",0);
		
	  msg_data[0] = Clear_bit;
         
	  if(gm_MessageSend(gmd_MESSAGE_LPM_GPIO_CTRL, msg_data, sizeof(msg_data)) == gmd_FALSE)
	  {
			static DWORD DW_BreakTime ;
			DW_BreakTime = gm_GetSystemTime();
			while(1)
			{
				if (abs((WORD)(gm_GetSystemTime() - DW_BreakTime)) > 500)
					break;
				if(gm_MessageSend(gmd_MESSAGE_LPM_GPIO_CTRL, msg_data, sizeof(msg_data)))
					break;
			}
	  }
   }
//   gm_Delay10ms(1);
}
#endif //defined(Qisda_93XX_BOARD)
//*********************************  END  **************************************
