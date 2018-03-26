/*
	$Workfile:   AppGlobalVariables.c  $
	$Revision: 1.11 $
	$Date: 2012/06/07 08:01:16 $
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
// MODULE:      AppGlobalVariables.c
//
// USAGE:       This module contains all the Global variables used across all
//				the modules in the Application.
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "system\all.h"
//
// Application Init Status
//
BOOL 	B_SplashComplete = FALSE;	// Splash screen status

//
// Mode Detect
//
BYTE 	gB_CurrentInputConnector[NUM_CHANNELS] = {NOT_USED, NOT_USED};

//
// Mode Setup
//
BYTE 	B_SetupInProgress;			// Mode Setup In Progress
BOOL	B_FreeRun[NUM_CHANNELS];	// System in Freerun
BYTE	B_DisplayLockMethod;	  	// display Lock Method
// (Closed Loop or DFL)
BYTE 	B_AutoAdjustStatus[NUM_CHANNELS] = {AA_IDLE, AA_IDLE};	// Auto-Adjust status
BYTE 	B_AutoAdjustStarted[NUM_CHANNELS] = {FALSE,FALSE};

//
// Overscan
//
BYTE 	W_OverscanEnabled[NUM_CHANNELS]; 			// 0 = Underscan,
// 1 = Overscan
//
// Aspect Ratios
//
gmt_ASPECT_RATIO 		B_OutputAspect;					// Panel Aspect Ratio (Determined in Init())
BYTE				 	B_AspectMethod[NUM_CHANNELS];	// Aspect Ratio Applied to Channel

//
// Tuner Application (NTSC) - Parental Control
//
#if 0
gmt_CONTENT_ADVISORY 	TVMainProgramRating;		   	// Program rating for main input (used by OSD)
gmt_CONTENT_ADVISORY 	TVPipProgramRating;			   	// Program rating for pip input (used by OSD)
#endif
BOOL					B_MainProgramBlocked;			// Flag to indicate if program blocked (in main)
BOOL					B_PipProgramBlocked;			// Flag to indicate if program blocked (in pip)

//
// Dynamic Scaling
//
gmt_PHY_CH				B_FocusBorder;				   	// Channel with Focus Border

//
// The following parameter indicates which VBI service is support on the system.
// The parameter will be used to configure the VBI driver during initialization
// time.
//

gmt_VBI_SERVICE VBI_ServiceType;
gmt_VBI_SERVICE VBI_ServicePreferred;

// Signal Status (Present, Nosync, NoCable, None)
BYTE	B_SignalStatus = STATUS_NONE;

// Height condition for SOG signal
BYTE 	B_Overlap_Height = OVERLAP_GR350;

#ifdef USE_SMT
BYTE gmvb_SerialStatus;
#endif

#ifdef DELL_U2410F
#ifdef CHECK_PxP_STATES_AC_ON
BYTE B_StoredPipMode = NO_PIP; //KevinL 20090219 for AC/DC on if main no signal, than turn off PIP when PIP was on.  
BOOL b_EnablePowerOnPIPModeFlag = FALSE; //KevinL 20090219 for AC/DC on if main no signal, than turn off PIP when PIP was on.  
BYTE  B_MainNoSyncCheckState = STOP_CHECK; //KevinL 20090325 for power on to determine PIP/PBP should be turned on or not.	
#endif
//DWORD  DW_Time1=0,DW_Time2=0; 
BYTE CableStatus = 0;

BOOL b_IsInuptOverScan[CH_PHY_NUM] = {FALSE,FALSE}; //KevinL 20090413
BOOL b_IsInputScanProcessingFlag = FALSE; //KevinL 20090120 for power on input scan processing,  don't sent MTO to enter power saving 
BOOL b_IsPipModeHandlerAvaliable = FALSE; //KevinL 20090108 for AC on DP/VGA in PIP mode abnormal display, main modesetup first than Pip.  
BYTE  AudioPowerStatus = ON;			//Neil 20090312+ for Audio power status check;
BOOL bRotateSensorFlag = OSD_LANDSCAPE; //KevinL 20081111
BYTE  FunctionRunningFlag = FALSE;		//Neil 20080110+ 
//BYTE KeyPressFlag = TouchKey_None;
BOOL bPipMessageAdmitFlag = TRUE; //KevinL 20090212

SWORD W_SavePIPIconXoffset = 0; //KevinL 20090417
SWORD W_SavePIPIconYoffset = 0;	
SWORD W_SaveRotatedIconXoffset = 0;
WORD W_ACC2MaskStatus = 0; //KevinL 20090417
//BOOL b_IsINLFactorySleepCommand = FALSE;
BOOL IsEnterHiDCRStatus = FALSE;
WORD versionnumber = 0;
BYTE versiontemp = 0;//calo+
BYTE versiontemp2 = 0;//calo+

BOOL AudioUnsupport_Message = TRUE;			//121002 Owen for audio message
BOOL DDCCIRunFlag = FALSE;
BOOL DDCCI_ForceSleep=gmd_FALSE;
BYTE B_UserSettingFlag = 0;					//Neil120618+ for send flag from mission to LPM
BYTE B_NoMSGAfterSS = 0;					//Neil120618+ for send flag from mission to LPM
BYTE AudioMuteFlag = 0xFF;			//Neil 20090413+ for audio mute control	ON ==> Mute; OFF ==> UnMute;
BYTE B_NoMainIconFlag = 0;		//Neil120726+ for don't show port icon after factory reset
BYTE B_AudioTimerCtrl = Aud_Tmr_None;		//Neil120814+ for audio pop noise 
BYTE SysChFlag[NUM_CHANNELS] = {SysCh_None, SysCh_None};		//Hyb130624+ 
BOOL NonColorSpaceGammaPostLUT_Loaded = gmd_FALSE;//120817 Edward for preset mode default Post lut load from sRGB table 
BYTE CurrentColorPresetMode=0xFF; 	//yvonne 20120828 for didn't load 2nd time

BYTE AudioUpdateNow = gmd_TRUE;			//Neil120907+ for test only

BYTE FirstACDCinPxP = FALSE; //20120925 Add by KevinChen
#endif

WORD HighBrightness = 0x3FF, LowBrightness = 0x100, AVGBrightness = 0;
BYTE DCR_State = 0, AVGCount =0, TargetBright = 0xFF;

TUartData uartData;


#ifdef BUZZER_USED// Raymond[(2011-08-04):02], add buzzer function
BYTE B_BuzzerControlFlag=BuzzerState_IDLE;
BYTE KeyPressedInDeepSleep = FALSE;
BOOL MissionPWDown = FALSE;
#endif


BYTE Input_RGB_Range = 0;
BYTE Input_YUV_Range = 0;

BYTE Input_Color_Domain = 0;
BYTE Output_Color_type = 0;

BYTE DCR_EnergyUse_Brightness=0;
#if 1//(defined(CMI_73XX_BOARD))
BYTE DCR_Energy_LowStep_Brightness=0;//120913 Edward for CMI fine step of Brightness < 20
#endif

#if IGNORE_SLEEP
SystemFlags_t SystemFlags = {0x0008};
#else
SystemFlags_t SystemFlags = {0x0000};
#endif

gmt_AUDIO_INPORT_TYPE gB_AudioInput = gmd_AUDIO_IN_DPRx1_I2S;

#if defined(Qisda_93XX_BOARD) //KevinL 20120906 (P3_TestDrive bug#1) DVI 2560x1440 PBP issue
BOOL b_ForceSharpnessFilterReload[CH_PHY_NUM] = {FALSE,FALSE}; 
#endif

#ifdef VWD_FULL_WINDOW_CHECK //KevinL 20120922 
BOOL b_IsVWDFullWindowChecking = FALSE; //Checking is processing or not
BOOL b_IsVWDFullWindowCheckRequired = TRUE; //Checking is required or not
#endif //#ifdef VWD_FULL_WINDOW_CHECK
#ifdef PIP_BORDER_ENABLED    
BOOL PxpBorderFlag = gmd_TRUE;
#endif


#ifdef TVP5160_DEV_ID
BYTE DecoderVideoMode = 0;
#endif
BYTE B_NeedRedoDACGain = gmd_TRUE;
BYTE DisplayFreeze = gmd_FALSE;
#ifdef PORT_CHANGE_SEAMLESS
BYTE PortChange_States  = PCS_IDLE;
BYTE CurrentInputOnScreen = ALL_BOARDS_VGA1;
#endif
BOOL CMD_PowerState = ON;
BYTE PD_PortChange = 0;;
#if(defined(BOARD_93XX_MNT))
BOOL NO_CABLE_SLEEP = gmd_FALSE;
#endif

#if (PROJECT_SELECT == PROJ_4K2K)
BYTE OutputModeStatus = OPS_Idle;
BYTE OutputModeAuto = gmd_TRUE;
BYTE OutputPanelSel = DEFAULT_PANEL;
BYTE OutputModeSel = OPM_Normal;


#endif
WORD W_SavedInputHStart[NUM_CHANNELS] = {0, 0};
BYTE LED_State = LED_IDLE;


/*********************************  END  ************************************/
