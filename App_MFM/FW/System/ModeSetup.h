/*
	$Workfile:   ModeSetup.h  $
	$Revision: 1.17 $
	$Date: 2013/05/23 06:31:54 $
*/
#ifndef __MODESETUP_H__
#define __MODESETUP_H__

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
// MODULE:      ModeSetup.h
//
// USAGE:
//
//******************************************************************************

//******************************************************************************
//  D E F I N E S
//******************************************************************************

//
//
// Display Defering
//
// is used to allow the system to buffer up (process) a number frames before
// turning on the output, this ensures that all blocks are synch'd to the same
// imagery and timing.  It takes 5 Frames to flush/fill the system with
// completely new data.
//
//  1 field @ 60Hz 		= 16.6ms
//	5 fields 			= 16.6 * 5
//						= 83ms
//						~ 100ms

#define	DISPLAY_DEFER_TMO_NORMAL	100		// ms (5 fields for all ports except for Tuner)
#define	DISPLAY_DEFER_TMO_COMPONENT_NORMAL	500		// ms (5 fields and 120ms for clamping) - changed from 200 to 500 (SCR3715)
#define	DISPLAY_DEFER_TMO_RF		350		// ms (Tuner Enable)
#define	NOMINAL_AUDIO_DELAY_RF		1000	// ms (Additional Delay for RF audio)
#define	NOMINAL_AUDIO_DELAY			1900	// ms (Additional Delay for non-RF audio)
#define STARTUP_PIP_TMO				2000	// ms (startup PIP time)
#define MAX_BLENDER_WAIT_TIME		3000	// ms (Blenders hat mave been pedning this long get forced on)


//
// Parameters for different Mode Setup request
//

// parameters for REQ_MS_DISPLAY_CHANGE
#define DISPLAY_CHANGE_NORMAL		0
#define DISPLAY_CHANGE_QUICK		1

#ifdef DEBUG_MODE_SETUP
// parameters for REQ_DEBUG
#define	PARAM_DEBUG_ALL				0
#define	PARAM_DEBUG_VIDEO			2
#endif

// parameters for REQ_MS_DYNAMIC
#define	PARAM_DYN_START					0
#define	PARAM_DYN_STEP					1
#define	PARAM_DYN_DEFAULT				2

// parameters for REQ_MS_AUDIO
/*
enum
{
	TIMED_MUTESPEAKER_OFF,
	TIMED_MUTESPEAKER_ON,
	TIMED_MUTEHEADPHONE_OFF,
	TIMED_MUTEHEADPHONE_ON,
	TIMED_AUDIO_SPEAKER_S,
	TIMED_AUDIO_HEADPHONE_S,
};
*/

//*******************************************************
//  Macros for input switching time measurement
//   - time for input port change to stable picture at output
//   - time for new input mode change to stable picture at output
//   - time for new channel change to stable picture at output
//*******************************************************
//#define DEBUG_IST

#ifdef DEBUG_IST

extern WORD ISTstart[CH_PHY_NUM];
extern BYTE ISTflag;

#define debug_ist_start(channel) {\
	ISTflag |= (channel == CH_A) ? 1 : 2; \
	ISTstart[channel] = gm_GetSystemTimeW();\
	}

#define debug_ist_stop(channel) {\
	WORD w = gm_GetSystemTimeW();\
	if (ISTflag & ((channel == CH_A) ? 1 : 2))\
		gm_Printf("IST%d> %d", channel, w-ISTstart[channel]);\
	ISTflag &= (channel == CH_B) ? (~1) : (~2);\
	}

#else
#define debug_ist_start(channel)
#define debug_ist_stop(channel)
#endif

#ifdef ST_4K2K_93xx_BOARD
#if ENABLE_PBP_ATHENA_L_DEEP_SLEEP_CHECK               
#define PBP_SLEEP_WAIT_TIME_OUT  (NO_SIGNAL_DELAY-1000)
#endif
#endif

//******************************************************************************
//  T Y P E S
//******************************************************************************
typedef enum
{
   CAPTURE_MODE_FULL,
   CAPTURE_MODE_RIGHT,
   CAPTURE_MODE_LEFT,
   CAPTURE_MODE_HALF_HORIZONTAL
}gmt_CaptureMode;



//
// Blender Off Methods
//
typedef enum
{
   BACKGROUND_ONLY,
   BLENDER_OFF,
   BACKGROUND_ONLY_NO_SYNC_UPDATE
} t_DISP_OFF_MODE;




//******************************************************************************
typedef enum
{
   DISP_LOCK_NORMAL,
   DISP_LOCK_DFL,
   DISP_FREERUN,
} t_DISP_LOCKING;

#pragma pack(push)
#pragma pack(1)
typedef struct
{
  LLD_TuningData_t descriptor; // descriptor
  BYTE MSQNRFilterStrength;      // Mosquito noise reduction filter strength control
                               // interpreted to program the MSQNR filter strength
                               // registers
                               // range 0-10 with default of 5
} VQ_MNR_Parameters_t;
#pragma pack(pop)

//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

BOOL ModeSetupBusy			(gmt_PHY_CH B_Channel);
void ModeSetupInit			(void);
void ModeSetupStartup		(void);
void ModeSetupHdlr			(void);
void PreModeChange			(gmt_PHY_CH B_Channel);
void RequestBlender			(gmt_PHY_CH B_Channel, WORD W_Time);
BOOL BlenderIsPending		(gmt_PHY_CH B_Channel);
void SetDisplayOn			(gmt_PHY_CH B_Channel);
#ifdef ST_4K2K_93xx_BOARD
#if ENABLE_ATHENA_R_L_IMAGE_SYNC_HANDLER
void Set4K2K_ImageSyncReady(void); //Add by ChengLin 20130408
#endif
void PanelSetAsInputTiming (void);
#endif
void SetDisplayOff			(gmt_PHY_CH B_Channel, t_DISP_OFF_MODE B_Mode);
void 	UpdateFilters(gmt_PHY_CH B_Channel);


void ModeSetupSwap			(void);
void ModeSetupFullSetup		(gmt_PHY_CH B_Channel);
void ModeSetupFreerun		(gmt_PHY_CH B_Channel);
void ModeSetupInputChange	(gmt_PHY_CH B_Channel);
void ModeSetupDisplayChange	(gmt_PHY_CH B_Channel, BYTE B_Method);
void ModeSetupDisplayTiming	(void);
void ModeSetupUserPref		(gmt_PHY_CH B_Channel);
void ModeSetupBlender		(gmt_PHY_CH B_Channel, WORD W_Time);
void ModeSetupAutoAdjust	(gmt_PHY_CH B_Channel);

#if (FEATURE_CCS == ENABLE)
BOOL IsCCSforHDDisabled(void);
#endif

 #if (FEATURE_TWO_STAGE_PROCESSING_MODE == ENABLE)
BOOL IsTwoStageAllowed				(void);
void EnterTwoStageProcessing		(void);
void ExitTwoStageProcessing			(void);
#endif

#ifdef ST_4K2K_93xx_BOARD 
void Image_Force_OnOff(BOOL OnOff);
void Image_ReadyFlag(BOOL flag);
BOOL IS_Image_Ready(void);
void Image_output_Request(void);
BOOL MemoryCheck_4Kx2K(void);
#if ENABLE_ATHENA_R_L_IMAGE_SYNC_HANDLER
void ImageSyncHandleSetState(gmt_ImageSyncState ImageState);
gmt_ImageSyncState ImageSyncHandleGetState(void);
void ImageSyncHandleSetModeStableFlag(BOOL Stable);
BOOL ImageSyncHandleGetModeStableFlag(void);
void ImageSyncHandler(void);
#endif
#endif

/*ChengLin 20130221: AFR Interrupt Handler*/
void AFR_Interrupt(void);

//******************************************************************************
//  G L O B A L S
//******************************************************************************

extern BYTE B_ModeSetupInit;
extern BYTE B_ModeSetupStarted;
extern BYTE B_PrevPipMode;

#endif

//*********************************  END  **************************************


