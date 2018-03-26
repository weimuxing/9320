/*
            $Workfile:   AppGlobalVariables.h  $
            $Revision: 1.17 $
            $Date    :   $
*/

#ifndef __APPGLOBALVARIABLES_H__
#define __APPGLOBALVARIABLES_H__
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
// MODULE:      AppGlobalVariables.h
//
// USAGE:       This module contains all the Global variables used across all
//				the modules in the Application. This is the header file
//				containing the externs declarations
//
//******************************************************************************

//******************************************************************************
//  G L O B A L    D E F I N E S
//******************************************************************************

//
// Mode Setup Blender Control
//
#define		BLENDER_NO_ACTION		0x00
#define		BLENDER_WAIT_MAIN		0x01
#define		BLENDER_READY_MAIN		0x02
#define		BLENDER_ON_MAIN			0x04
#define		BLENDER_MASK_MAIN		(BLENDER_WAIT_MAIN|BLENDER_READY_MAIN|BLENDER_ON_MAIN)
#define		BLENDER_WAIT_PIP		0x10
#define		BLENDER_READY_PIP		0x20
#define		BLENDER_ON_PIP			0x40
#define		BLENDER_MASK_PIP		(BLENDER_WAIT_PIP|BLENDER_READY_PIP|BLENDER_ON_PIP)
#define		BLENDER_BOTH_REQUIRED	0x80
#define		BLENDER_READY_BOTH		(BLENDER_READY_MAIN|BLENDER_READY_PIP)

enum
{
   STATUS_NONE,
   STATUS_SYNC_PRESENT,
   STATUS_NO_SYNC,
   STATUS_NO_CABLE,
   STATUS_INVALID_MODE
};

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************

enum				// Auto-adjust status
{
   AA_IDLE,		// Not doing anything
   AA_START,		// AA in progress
   AA_ABORT,		// AA abort by used
};

#define gmd_TRUE			 TRUE
#define gmd_FALSE			 FALSE

enum
{
   OFF = 0,
   ON  = 1
};

typedef union SystemFlagsUnion
{
   WORD all;
   struct
   {
      WORD DramCodeExecution:1;
      WORD SerialFlashInit:1;
      WORD PrintDebug:1;
      WORD IgnoreSleep:1;
      WORD MissionSleep:1; // 1: Keep Mission OCM power on for sleep mode
   }sys;
}SystemFlags_t;

//******************************************************************************
//  E X T E R N S
//******************************************************************************

//
// Application Init Status
//
extern BOOL		B_SplashComplete;			// Splash screen status

//
// Mode Detect
//
extern BYTE 	gB_CurrentInputConnector[NUM_CHANNELS];

//
// Mode Setup
//
extern BYTE		B_SetupInProgress;			// Mode Setup In Progress
extern BOOL		B_FreeRun[NUM_CHANNELS];	// System in Freerun
extern BYTE		B_DisplayLockMethod;		// display Lock Method (Closed Loop or DFL)
extern BYTE 	B_AutoAdjustStatus[NUM_CHANNELS];			// Auto-Adjust status
extern BYTE 	B_AutoAdjustStarted[NUM_CHANNELS];

//
// Overscan
//
extern BYTE    	W_OverscanEnabled[NUM_CHANNELS];		// 0 = Underscan, 1 = Overscan

//
// Aspect Ratios
//
extern gmt_ASPECT_RATIO 	B_OutputAspect;							// Panel Aspect Ratio (Determined in Init())
extern BYTE				 	B_AspectMethod[NUM_CHANNELS];			// Aspect Ratio Applied to Channel

//
// Dynamic Scaling
//
extern gmt_PHY_CH			B_FocusBorder;							// Channel with Focus Border

//
// Tuner Application (NTSC) - Parental Control
//
#if 0
extern gmt_CONTENT_ADVISORY 	TVMainProgramRating;				// Program rating for main input (used by OSD)
extern gmt_CONTENT_ADVISORY 	TVPipProgramRating;					// Program rating for pip input (used by OSD)
#endif
extern BOOL						B_MainProgramBlocked;				// Flag to indicate if program blocked (in main)
extern BOOL						B_PipProgramBlocked;				// Flag to indicate if program blocked (in pip)

//
// VBI
//
extern gmt_VBI_SERVICE 		VBI_ServiceType;            			// Teletex, CC, or none

//
// Buffer for caching Blocked Channels list reads
//
extern WORD W_BlockedChannelListBuffer[(125 + 8)/16];

extern BYTE	B_SignalStatus;

extern 	BYTE 	B_Overlap_Height;

#ifdef USE_SMT
	extern BYTE gmvb_SerialStatus;
#endif

#ifdef DELL_U2410F
#ifdef CHECK_PxP_STATES_AC_ON
extern BYTE B_StoredPipMode; //KevinL 20090219 for AC/DC on if main no signal, than turn off PIP when PIP was on.  
extern BOOL b_EnablePowerOnPIPModeFlag; //KevinL 20090219 for AC/DC on if main no signal, than turn off PIP when PIP was on.  
extern BYTE  B_MainNoSyncCheckState; //KevinL 20090325 for power on to determine PIP/PBP should be turned on or not.
#endif
//extern DWORD  DW_Time1,DW_Time2; 

extern BYTE CableStatus;

extern BOOL b_IsInuptOverScan[CH_PHY_NUM]; //KevinL 20090413
extern BOOL b_IsInputScanProcessingFlag; //KevinL 20090120 for power on input scan processing,  don't sent MTO to enter power saving 
extern BOOL b_IsPipModeHandlerAvaliable;
extern BYTE 	AudioPowerStatus;			//Neil 20090312+ for Audio power status check;
extern BOOL bRotateSensorFlag; //KevinL 20081111
extern BYTE  FunctionRunningFlag;	//Neil 20090110+
//extern BYTE KeyPressFlag;
extern BOOL bPipMessageAdmitFlag; //KevinL 20090212
extern SWORD W_SavePIPIconXoffset; //KevinL 20090417
extern SWORD W_SavePIPIconYoffset;	
extern SWORD W_SaveRotatedIconXoffset;
extern WORD   W_ACC2MaskStatus; //KevinL 20090417
//BOOL b_IsINLFactorySleepCommand;
extern BOOL IsEnterHiDCRStatus;	
extern WORD versionnumber;
extern BYTE versiontemp;//calo+
extern BYTE versiontemp2;//calo+

extern BOOL AudioUnsupport_Message;			//121002 Owen for audio message
extern BOOL DDCCIRunFlag;
extern BOOL DDCCI_ForceSleep;
extern BYTE B_UserSettingFlag;					//Neil120618+ for send flag from mission to LPM
extern BYTE B_NoMSGAfterSS ;					//Neil120618+ for send flag from mission to LPM
extern BYTE	AudioMuteFlag ;			//Neil 20090413+ for audio mute control	
extern BYTE B_NoMainIconFlag ;		//Neil120726+ for don't show port icon after factory reset
extern BYTE B_AudioTimerCtrl;		//Neil120814+ for audio pop noise 
extern BYTE SysChFlag[NUM_CHANNELS];		//Hyb130624+ 
extern BOOL	NonColorSpaceGammaPostLUT_Loaded;	//120817 Edward for preset mode default Post lut load from sRGB table 
extern BYTE CurrentColorPresetMode; 	//yvonne 20120828 for didn't load 2nd time
extern BYTE AudioUpdateNow ;			//Neil120907+ for test only
extern BYTE FirstACDCinPxP; //20120925 Add by KevinChen

#endif

extern WORD HighBrightness, LowBrightness, AVGBrightness;
extern BYTE DCR_State, AVGCount, TargetBright;
extern TUartData uartData;

#ifdef BUZZER_USED
extern BYTE B_BuzzerControlFlag;
extern BYTE KeyPressedInDeepSleep ;
extern BOOL MissionPWDown;//120420 Edward add for buzzer control
#endif

extern BYTE Input_RGB_Range;
extern BYTE Input_YUV_Range;

extern BYTE Input_Color_Domain;
extern BYTE Output_Color_type;

extern BYTE DCR_EnergyUse_Brightness;
#if 1//(defined(CMI_73XX_BOARD))
extern BYTE DCR_Energy_LowStep_Brightness;//120913 Edward for CMI fine step of Brightness < 20
#endif

extern SystemFlags_t SystemFlags;
extern gmt_AUDIO_INPORT_TYPE gB_AudioInput;

#if defined(Qisda_93XX_BOARD) //KevinL 20120906 (P3_TestDrive bug#1) DVI 2560x1440 PBP issue
extern BOOL b_ForceSharpnessFilterReload[CH_PHY_NUM]; 
#endif

#ifdef VWD_FULL_WINDOW_CHECK //KevinL 20120922 
extern BOOL b_IsVWDFullWindowChecking;
extern BOOL b_IsVWDFullWindowCheckRequired;
#endif //#ifdef VWD_FULL_WINDOW_CHECK
#ifdef PIP_BORDER_ENABLED    
extern BOOL PxpBorderFlag;
#endif

#ifdef EXTERNAL_DECODER
extern BYTE DecoderVideoMode;
#endif

extern BYTE B_NeedRedoDACGain;
extern BYTE DisplayFreeze ;
#ifdef PORT_CHANGE_SEAMLESS
extern BYTE PortChange_States ;
extern BYTE CurrentInputOnScreen;
#endif
extern BOOL CMD_PowerState;
extern BYTE PD_PortChange;
#if(defined(BOARD_93XX_MNT))
extern BOOL NO_CABLE_SLEEP;
#endif

#if (PROJECT_SELECT == PROJ_4K2K)
extern BYTE OutputModeStatus;
extern BYTE OutputModeAuto;
extern BYTE OutputPanelSel;
extern BYTE OutputModeSel;

#endif
extern WORD W_SavedInputHStart[NUM_CHANNELS];
extern BYTE LED_State;

#endif //__APPGLOBALVARIABLES_H__

