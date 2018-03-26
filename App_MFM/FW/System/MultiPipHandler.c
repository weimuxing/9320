/*
	$Workfile:   MultiPipHandler.c  $
	$Revision:   1.72  $
	$Date:   May 11 2007 12:03:08  $
*/
#define __MULTIPIPHANDLER_C__

//******************************************************************************
//
//              Copyright (C) 2002.  GENESIS MICROCHIP INC.
//      All rights reserved.  No part of this program may be reproduced.
//
//     Genesis Microchip Inc., 165 Commerce Valley Dr. West
//     Thornhill, Ontario, Canada, L3T 7V8
//
//==============================================================================
//
// MODULE:      MultiPipHandler.c
//
// USAGE:       This module contains main() function and main Loop related
//              functions.
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "System\all.h"
#include "mem.h"

//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************

#define RESTORE_PIP_INPUT_AFTER_EXIT

#define MPIP_MAX_TIMEOUT_COUNT			2

//#define _DEBUG_MSG
#ifdef _DEBUG_MSG
	#define msg(x,y)		gm_Print(x,y)
#else
	#define msg(x,y)
#endif

typedef struct
{
	BYTE ScanOrder;
	BYTE InputSource;
	BYTE ChannelNumber;	
} MultiPipMapping;

typedef struct
{
	MultiPipStates CurrentState;
	BYTE CurrentWindow;
	BYTE AnchorWindow;
	BYTE AnchorChannelNumber;
	MultiPipScanMode ScanMode;
	MultiPipStyleInfo ROM *Stp_CurrentStyle;
	BYTE MaxWindows;
	PipModes PreviousPipMode;
	BYTE PreviousPipInput;
	gmt_SOURCE_TYPE PreviousPipChannelSource;
	MultiPipMapping Mapping[MULTIPIP_MAX_WINDOWS];
} MultiPipData;

//******************************************************************************
//  G L O B A L     V A R I A B L E
//******************************************************************************

//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
#if (FEATURE_MultiPxp == ENABLE)
static void MPipScanNextWindow(void);
static BOOL MPipTimeout(void);
static void MPipSetTimer(WORD W_Time);
static BYTE MPipGetMaxWindows(void);
static BYTE MPipGetWindowFromScanOrder(BYTE B_TargetScanOrder);
static BYTE MPipGetNextWindow(BYTE B_ReferenceWindow);
static BYTE MPipGetPrevWindow(BYTE B_ReferenceWindow);
static void MPipSetWindow(BYTE B_WindowNumber, BOOL DrawBorder, BOOL ReAnchor);
static PipModes MPipGetCurrentPipMode(void);
static void MPipSetCurrentPipMode(PipModes Mode);
static void MPipSetOsdWindowValues(WORD *XPos, WORD *YPos, WORD *Value, BYTE B_WindowNumber);
static void MPipUpdateOsd(BYTE B_WindowNumber);
static void MPipDeltaChannelChange(SBYTE SB_ChannelDelta);
static void MPipInit(MultiPipStyleInfo ROM *MPipStyle);
static void MPipChangePipStyle(MultiPipStyleInfo ROM *MPipStyle);
static void MPipStartScan(void);
static void MPipStartNavigate(void);
static BYTE MPipSearchChannel(TunerID_t TunerID, BYTE B_CurrentChannel, TunerChannelType ChannelType, SBYTE sB_Offset);
static void MPipSetPipOnTop(BOOL PipOnMain);
static WORD DivideRoundUp(WORD W_Dividend, WORD W_Divisor);
static void MPipGetPipActivePosition(WORD *HActive, WORD *VActive);
#endif	

//******************************************************************************
//  S T A T I C     V A R I A B L E
//******************************************************************************
static MultiPipData MPipPrivData;
static BOOL Bt_IsFreshStart;
static BOOL Bt_MemoryWriteFrozen;

//******************************************************************************
//  C O D E
//******************************************************************************

#if (FEATURE_MultiPxp == ENABLE)
//******************************************************************************
//
// FUNCTION       : static void MPipScanNextWindow(void)
//
// USAGE          : Retrieves the next window in the scan order and sets the window focus by calling
//                  MPipSetWindow.  Depending on ScanMode, the next windows input/channel can
//                  be changed.
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : MPipPrivData.CurrentWindow (R/W)
//                  MPipPrivData.ScanMode (R)
//                  gmvb_PipChannel (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  : 
//
// POST_CONDITION : 
//
//******************************************************************************
static void MPipScanNextWindow(void)
{
	static BYTE B_TempChannel;

	// Only save non-zero channel number
	if (MPipPrivData.Mapping[MPipPrivData.CurrentWindow].ChannelNumber != 0)
		B_TempChannel = MPipPrivData.Mapping[MPipPrivData.CurrentWindow].ChannelNumber;
	
	MPipPrivData.CurrentWindow = MPipGetNextWindow(MPipPrivData.CurrentWindow);

	switch(MPipPrivData.ScanMode)
	{
		case MPIP_PAGES:
			if (MPipPrivData.CurrentWindow == MPipPrivData.AnchorWindow)
			{
				break;
			}
			/* else fall through intentionally */
		case MPIP_SCAN:
		{
			if (MPipPrivData.Mapping[MPipPrivData.CurrentWindow].InputSource == RF1)
			{
				/*
				MPIP_SCAN mode.  Scan to the next channel and update mapping Table
				*/
				TunerID_t ActiveTuner = GetActiveTuner(gmvb_PipChannel);

				MPipPrivData.Mapping[MPipPrivData.CurrentWindow].ChannelNumber = 
					MPipSearchChannel(ActiveTuner, B_TempChannel, ACTIVE_CHANNEL, 1);
			}
			break;
		}	
		case MPIP_RELOAD:
		default:
			break;
	}
	MPipSetWindow(
			MPipPrivData.CurrentWindow, 
			MPipPrivData.Stp_CurrentStyle->MovingHighlight, 
			!MPipPrivData.Stp_CurrentStyle->AnchorPresent);
}


//******************************************************************************
//
// FUNCTION       : static BOOL MPipTimeout(void)
//
// USAGE          : Wrapper for multipip timer, returns TRUE when timeout
//
// INPUT          : None
//
// OUTPUT         : BOOL
//
// GLOBALS        : MULTIPIP_10ms_TMR timer	(R)		
//
// USED_REGS      : None
//
// PRE_CONDITION  : 
//
// POST_CONDITION : 
//
//******************************************************************************
static BOOL MPipTimeout(void)
{
	return (gm_TimerCheck(MULTIPIP_10ms_TMR) == TIMER_TMO);
}

//******************************************************************************
//
// FUNCTION       : static void MPipSetTimer(WORD W_Time)
//
// USAGE          : Wrapper for setting multipip timer
//
// INPUT          : W_Time for timeout amount
//
// OUTPUT         : None
//
// GLOBALS        : MULTIPIP_10ms_TMR timer	(W)		
//
// USED_REGS      : None
//
// PRE_CONDITION  : 
//
// POST_CONDITION : 
//
//******************************************************************************
static void MPipSetTimer(WORD W_Time)
{
	gm_TimerStart(MULTIPIP_10ms_TMR, W_Time);
}

//******************************************************************************
//
// FUNCTION       : static BYTE MPipGetMaxWindows(void)
//
// USAGE          : Scans MPipPrivData.Mapping array to determine the maximum number of
//                  MPip windows
//
// INPUT          : None
//
// OUTPUT         : Max
//
// GLOBALS        : MPipPrivData.Mapping array	(R)
//
// USED_REGS      : None
//
// PRE_CONDITION  : 
//
// POST_CONDITION : 
//
//******************************************************************************
static BYTE MPipGetMaxWindows(void)
{
	BYTE i, Max = 0;

	for (i = 0; i < MULTIPIP_MAX_WINDOWS; i++)
	{
		if (Max < MPipPrivData.Mapping[i].ScanOrder)
		{
			Max = MPipPrivData.Mapping[i].ScanOrder;
		}
	}
	return Max;
}

//******************************************************************************
//
// FUNCTION       : static BYTE MPipGetWindowFromScanOrder(BYTE B_TargetScanOrder)
//
// USAGE          : Determines the MPip window index from the scan oder
//
// INPUT          : B_TargetScanOrder
//
// OUTPUT         : i - MPip window index
//
// GLOBALS        : MPipPrivData.Mapping array (R)			
//
// USED_REGS      : None
//
// PRE_CONDITION  : 
//
// POST_CONDITION : 
//
//******************************************************************************
static BYTE MPipGetWindowFromScanOrder(BYTE B_TargetScanOrder)
{
	BYTE i;
	for (i = 0; i < MULTIPIP_MAX_WINDOWS; i++)
	{
		if (MPipPrivData.Mapping[i].ScanOrder == B_TargetScanOrder)
			break;
	}
	return i;
}

//******************************************************************************
//
// FUNCTION       : static BYTE MPipGetNextWindow(BYTE B_ReferenceWindow)
//
// USAGE          : Returns the next window index in the scanning order based on the input
//                  variable B_ReferenceWindow
//
// INPUT          : B_ReferenceWindow
//
// OUTPUT         : The next window index
//
// GLOBALS        : MPipPrivData.Mapping array (R)			
//
// USED_REGS      : None
//
// PRE_CONDITION  : 
//
// POST_CONDITION : 
//
//******************************************************************************
static BYTE MPipGetNextWindow(BYTE B_ReferenceWindow)
{
	BYTE TargetScanOrder, i;
	
	/*
	Is this the last window in scan sequence?
	*/
	if (MPipPrivData.Mapping[B_ReferenceWindow].ScanOrder == MPipPrivData.MaxWindows)
	{
		TargetScanOrder = 1;
	}
	else 
	{
		TargetScanOrder = MPipPrivData.Mapping[B_ReferenceWindow].ScanOrder + 1;
	}

	/*
	Found Target Scan Order, now lookup window index.
	*/
	return MPipGetWindowFromScanOrder(TargetScanOrder);
}

//******************************************************************************
//
// FUNCTION       : static BYTE MPipGetPrevWindow(BYTE B_ReferenceWindow)
//
// USAGE          : Returns the previous window index in the scanning order based on the input
//                  variable B_ReferenceWindow
//
// INPUT          : B_ReferenceWindow
//
// OUTPUT         : The previous window index
//
// GLOBALS        : MPipPrivData.Mapping array (R)			
//
// USED_REGS      : None
//
// PRE_CONDITION  : 
//
// POST_CONDITION : 
//
//******************************************************************************
static BYTE MPipGetPrevWindow(BYTE B_ReferenceWindow)
{
	BYTE TargetScanOrder, i;
	
	/*
	Is this the first window in scan sequence?
	*/
	if (MPipPrivData.Mapping[B_ReferenceWindow].ScanOrder == 1)
	{
		TargetScanOrder = MPipPrivData.MaxWindows;
	}
	else 
	{
		TargetScanOrder = MPipPrivData.Mapping[B_ReferenceWindow].ScanOrder - 1;
	}

	/*
	Found Target Scan Order, now lookup window index.
	*/
	return MPipGetWindowFromScanOrder(TargetScanOrder);
}

//******************************************************************************
//
// FUNCTION       : static void MPipSetWindow(BYTE B_WindowNumber, BOOL DrawBorder, BOOL ReAnchor)
//
// USAGE          : Sets the MPip window and window properties (border and anchor)
//
// INPUT          : B_WindowNumber
//                  DrawBorder
//                  ReAnchor
//
// OUTPUT         : None
//
// GLOBALS        : MPipPrivData.Stp_CurrentStyle->HighlightOrigin (R)	
//                  MPipPrivData.Mapping array (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  : 
//
// POST_CONDITION : 
//
//******************************************************************************
static void MPipSetWindow(BYTE B_WindowNumber, BOOL DrawBorder, BOOL ReAnchor)
{
	TunerID_t ActiveTuner = GetActiveTuner(gmvb_PipChannel);
	
	if(DrawBorder)
	{			
		gm_MultiPipSetHighlightWindow(B_WindowNumber,
      										MPipPrivData.Stp_CurrentStyle->HighlightOrigin);
	}
	
	gm_WaitForInputBlanking(gmvb_PipChannel);

	gm_InputCaptureFreeze(gmvb_PipChannel, TRUE);
	gm_MultiPipSetActiveWindow(B_WindowNumber);
	//without the following line highlight border may not show up correctly
	gm_SyncUpdateWait(gmvb_PipChannel, INPUT_OUTPUT_CLOCK);
	
	MPipPrivData.CurrentWindow = B_WindowNumber;
	if (ReAnchor)
	{
		MPipPrivData.AnchorWindow = B_WindowNumber;
	}

#if ((FEATURE_DECODER_TUNER_NUM == 2) && (MAX_TUNER_INPUTS == 2))
	//do not change the value of MPIP_PIP_INPUT_SOURCE if InputSource is RF1
	if (!((MPipPrivData.Mapping[B_WindowNumber].InputSource == ALL_BOARDS_RF1)
		&& ((MPIP_PIP_INPUT_SOURCE == ALL_BOARDS_RF1)||(MPIP_PIP_INPUT_SOURCE == ALL_BOARDS_RF2))))
		MPIP_PIP_INPUT_SOURCE = MPipPrivData.Mapping[B_WindowNumber].InputSource;
	
	if ((MPIP_PIP_INPUT_SOURCE == ALL_BOARDS_RF1)||(MPIP_PIP_INPUT_SOURCE == ALL_BOARDS_RF2))
#else
	MPIP_PIP_INPUT_SOURCE = MPipPrivData.Mapping[B_WindowNumber].InputSource;
	
	if (MPIP_PIP_INPUT_SOURCE == ALL_BOARDS_RF1)
#endif		
	{
		TunerSetChannel (ActiveTuner,
						MPipPrivData.Mapping[B_WindowNumber].ChannelNumber,
						BLANK_NONE);
		MPIP_CH_NUMBER_USERPREF = MPipPrivData.Mapping[B_WindowNumber].ChannelNumber;
		MPipUpdateOsd(B_WindowNumber);
	}
	AdjustCurrentInputPip();


	// AdjustCurrentInputPip() will set signal to unstable if current input is different from
	//   "MPipPrivData.Mapping[B_WindowNumber].InputSource"
	if (gm_IsValidSignal(gmvb_PipChannel))
	{
		if (Bt_MemoryWriteFrozen)
		{
			gm_MemoryWriteFreeze(gmvb_PipChannel, FALSE);	
			Bt_MemoryWriteFrozen = FALSE;
		}

		gm_WaitForInputBlanking(gmvb_PipChannel);
		gm_InputCaptureFreeze(gmvb_PipChannel, FALSE);
	}
	else
	{
		gm_WaitForInputBlanking (gmvb_PipChannel);
		gm_MemoryWriteFreeze(gmvb_PipChannel, TRUE);//do not freeze in the middle of a field
		Bt_MemoryWriteFrozen = TRUE;
	
//		gm_WriteRegWord(AFR_CONTROL, 0);
		gm_SetAutoMode(gmvb_PipChannel, AUTO_MUTE_DISABLE);
		gm_AfrRestart(gmvb_PipChannel);
		gm_DisplayForceBkgnd(gmvb_PipChannel, FALSE, 0);
	}
}

//******************************************************************************
//
// FUNCTION       : static PipModes MPipGetCurrentPipMode(void)
//
// USAGE          : Wrapper to return the current Pip mode
//
// INPUT          : None
//
// OUTPUT         : UserPrefPipMode or 0
//
// GLOBALS        : PipModes UserPrefPipMode (R)			
//
// USED_REGS      : None
//
// PRE_CONDITION  : 
//
// POST_CONDITION : 
//
//******************************************************************************
static PipModes MPipGetCurrentPipMode(void)
{
#ifdef UserPrefPipMode
	return UserPrefPipMode;
#else
	return 0;
#endif
}

//******************************************************************************
//
// FUNCTION       : static void MPipSetCurrentPipMode(PipModes Mode)
//
// USAGE          : Wrapper to set the current Pip mode
//
// INPUT          : Mode
//
// OUTPUT         : None
//
// GLOBALS        : PipModes UserPrefPipMode (W)		
//
// USED_REGS      : None
//
// PRE_CONDITION  : 
//
// POST_CONDITION : 
//
//******************************************************************************
static void MPipSetCurrentPipMode(PipModes Mode)
{
#ifdef UserPrefPipMode
	UserPrefPipMode = Mode;
#endif
}

//******************************************************************************
//
// FUNCTION       : static void MPipSetOsdWindowValues(
//                  	WORD *XPos, 
//                  	WORD *YPos, 
//                  	WORD *Value, 
//                  	BYTE B_WindowNumber)
//
// USAGE          : Sets the appropriate X and Y position and Actual channel value to the variables
//                  Mapped to OSD adjustors for displaying the correct channel number and position 
//                  on OSD
//
// INPUT          : XPos
//                  YPos
//                  Value
//                  B_WindowNumber
//
// OUTPUT         : None
//
// GLOBALS        : MPipPrivData.Stp_CurrentStyle->W_XPadding (R)
//                  MPipPrivData.Stp_CurrentStyle->W_YPadding (R)
//                  MPipPrivData.Mapping array
//
// USED_REGS      : None
//
// PRE_CONDITION  : 
//
// POST_CONDITION : 
//
//******************************************************************************
static void MPipSetOsdWindowValues(
	WORD *XPos, 
	WORD *YPos, 
	WORD *Value, 
	BYTE B_WindowNumber)
{
	WORD 
		W_PipHActive,
		W_PipVActive,
		W_PipWindowXOrgin,
		W_PipWindowYOrgin,
		W_NewXPos,
		W_NewYPos,
		W_XPadding,
		W_YPadding;


	MPipGetPipActivePosition(&W_PipHActive, &W_PipVActive);

	W_PipWindowXOrgin = (B_WindowNumber  % MULTIPIP_MAX_COLS) * 
						(W_PipHActive + MPipPrivData.Stp_CurrentStyle->W_BorderWidth) +
						MPipPrivData.Stp_CurrentStyle->W_MultiPipHStart;
	
	W_PipWindowYOrgin = (B_WindowNumber  / MULTIPIP_MAX_COLS) * 
						(W_PipVActive + MPipPrivData.Stp_CurrentStyle->W_BorderHeight) +
						MPipPrivData.Stp_CurrentStyle->W_MultiPipVStart;

	W_XPadding = MPipPrivData.Stp_CurrentStyle->W_XPadding;
	W_YPadding = MPipPrivData.Stp_CurrentStyle->W_YPadding;

	switch (MPipPrivData.Stp_CurrentStyle->B_ChPosition)
	{
		case TOP_LEFT:
			W_NewXPos = W_PipWindowXOrgin + W_XPadding;
			W_NewYPos = W_PipWindowYOrgin + W_YPadding;
			break;

		case TOP_RIGHT:
			W_NewXPos = W_PipWindowXOrgin + W_PipHActive - W_XPadding;
			W_NewYPos = W_PipWindowYOrgin + W_YPadding;
			break;

		case BOTTOM_LEFT:
			W_NewXPos = W_PipWindowXOrgin + W_XPadding;
			W_NewYPos = W_PipWindowYOrgin + W_PipVActive - W_YPadding;
			break;

		case BOTTOM_RIGHT:
			W_NewXPos = W_PipWindowXOrgin + W_PipHActive - W_XPadding;
			W_NewYPos = W_PipWindowYOrgin + W_PipVActive - W_YPadding;
			break;

		default:
			break;

	}

	*XPos = W_NewXPos;
	*YPos = W_NewYPos;
	*Value = MPipPrivData.Mapping[MPipPrivData.CurrentWindow].ChannelNumber;

}

//******************************************************************************
//
// FUNCTION       : static void MPipUpdateOsd(BYTE B_WindowNumber)
//
// USAGE          : Calls the update Osd values and sends message to OSD for update
//
// INPUT          : B_WindowNumber
//
// OUTPUT         : None
//
// GLOBALS        : gmvb_MainChannel (R)			
//
// USED_REGS      : None
//
// PRE_CONDITION  : 
//
// POST_CONDITION : 
//
//******************************************************************************
static void MPipUpdateOsd(BYTE B_WindowNumber)
{
	MPipSetOsdWindowValues(	
			(WORD *)&MPIP_CH_NUMBER_XOFFSET,
			(WORD *)&MPIP_CH_NUMBER_YOFFSET,
			(WORD *)&MPIP_CH_NUMBER_USERPREF,
			B_WindowNumber);
	OSDRequestMsg(gmvb_MainChannel, MTO_UpdateMultiPipChannel, REQUEST_NOW);

}

//******************************************************************************
//
// FUNCTION       : void MPipDeltaChannelChange(SBYTE SB_ChannelDelta)
//
// USAGE          : Changes the MPip channel by a given delta
//
// INPUT          : SB_ChannelDelta
//
// OUTPUT         : None
//
// GLOBALS        : gmvb_PipChannel (R)
//                  MPipPrivData.Mapping array (R)
//                  MPipPrivData.AnchorWindow (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  : 
//
// POST_CONDITION : 
//
//******************************************************************************
void MPipDeltaChannelChange(SBYTE SB_ChannelDelta)
{
	BYTE B_NewAnchorChannel, B_NewAnchorWindow;
	TunerID_t ActiveTuner;

	ActiveTuner = GetActiveTuner(gmvb_PipChannel);

	B_NewAnchorChannel = MPipSearchChannel (
							ActiveTuner,
							MPipPrivData.Mapping[MPipPrivData.AnchorWindow].ChannelNumber,
							ACTIVE_CHANNEL,
							SB_ChannelDelta);

	MPipPrivData.Mapping[MPipPrivData.AnchorWindow].ChannelNumber = B_NewAnchorChannel;
	MPipSetWindow(MPipPrivData.AnchorWindow, TRUE, FALSE);
	MPipStartScan();
}


//******************************************************************************
//
// FUNCTION       : void static MPipInit(MultiPipStyleInfo ROM *MPipStyle)
//
// USAGE          : Initializes MPip Style using MPipStyle pointer
//
// INPUT          : MPipStyle	
//
// OUTPUT         : None
//
// GLOBALS        : MPipPrivData.PreviousPipMode (W)
//                  MPipPrivData.CurrentWindow (R)
//                  gmvb_PipChannel (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  : 
//
// POST_CONDITION : 
//
//******************************************************************************
void static MPipInit(MultiPipStyleInfo ROM *MPipStyle)
{
	BYTE B_MainDisplayChangeSpeed = DISPLAY_CHANGE_NORMAL;
	MultiPipStartIdle();

	/*
	Save previous Non-mpip pip mode. Used when exiting MPip for restoring this Pip Mode
	*/
	MPipPrivData.PreviousPipMode = 
			(MPipGetCurrentPipMode()==MULTI_PIP ? NO_PIP : MPipGetCurrentPipMode());

	// if there is only one ADC and both channels are using the ADC yet configured to
	//   different sources, store previous pip input as the current main input; otherwise when
	//   exit from multipip mode, the pip input, which is in conflict with the main input, will
	//   force the main channel to pip input.
	if (((gmvb_ChannelSource[gmvb_MainChannel] == AIP_SD_VID)
		||(gmvb_ChannelSource[gmvb_MainChannel] == AIP_HD_GFX))
		&&
		((gmvb_ChannelSource[gmvb_PipChannel] == AIP_SD_VID)
		||(gmvb_ChannelSource[gmvb_PipChannel] == AIP_HD_GFX)))
	{
		MPipPrivData.PreviousPipInput = UserPrefCurrentInputMain;
		MPipPrivData.PreviousPipChannelSource = gmvb_ChannelSource[gmvb_MainChannel];
	}
 	else
 	{
		MPipPrivData.PreviousPipInput = UserPrefCurrentInputPip;
		MPipPrivData.PreviousPipChannelSource = gmvb_ChannelSource[gmvb_PipChannel];
	}
		
	MPipChangePipStyle(MPipStyle);
	UserPrefCurrentInputPip = MPipPrivData.Mapping[0].InputSource;
	
	//
	// Turn off any pip modes before setting up multipip
	//
	if (MPipGetCurrentPipMode() != NO_PIP)
	{
		MPipSetCurrentPipMode(NO_PIP);
		ModeSetupDisplayChange(gmvb_PipChannel, DISPLAY_CHANGE_QUICK);
	}
	else
	{
#if (FEATURE_TWO_STAGE_PROCESSING_MODE == ENABLE)
		if (gm_Is2StageEnabled())
		{
			ExitTwoStageProcessing();
			B_MainDisplayChangeSpeed = DISPLAY_CHANGE_QUICK;
		}
#endif
	}
#ifdef PIP_PORT_INDEPENDENCE
	// If there was no PIP previously, make sure that the input has been setup correctly by
	// either cloning or performing SelectInput()
	ChoosePipSetupMethod();
#else
	// Restart mode detection if UserPrefCurrentInputPip is not the same as the current input
	//   the hardware is configured to
	AdjustCurrentInputPip();
#endif

	MPipSetCurrentPipMode(MULTI_PIP);
	MPipSetPipOnTop(TRUE);   
	ModeSetupDisplayChange(gmvb_MainChannel, B_MainDisplayChangeSpeed);
	Bt_IsFreshStart = TRUE;
	Bt_MemoryWriteFrozen = FALSE;

	// if input source of first multipip window is the same as UserPrefCurrentInputPip, then valid
	//   flag remains set and call the following line to set up hardware for multipip; otherwise,
	//   mode detection state machine will go unstable and ModeSetupFullSetup() will be called
	//   when the state machine becomes stable again.
	if (gm_IsValidSignal(gmvb_PipChannel))
		ModeSetupDisplayChange(gmvb_PipChannel, DISPLAY_CHANGE_NORMAL);	
}

//******************************************************************************
//
// FUNCTION       : void static MPipChangePipStyle(MultiPipStyleInfo ROM *MPipStyle)
//
// USAGE          : Changes MPip Style using MPipStyle pointer
//
// INPUT          : MPipStyle
//
// OUTPUT         : None
//
// GLOBALS        : MPipPrivData.Stp_CurrentStyle (W)
//                  MPipPrivData.Mapping (R/W)
//                  MPipPrivData.Stp_CurrentStyle->Ba_ScanOrder (R)
//                  MPipPrivData.MaxWindows (W)
//                  MPipPrivData.AnchorWindow (W)
//
// USED_REGS      : None
//
// PRE_CONDITION  : 
//
// POST_CONDITION : 
//
//******************************************************************************
void static MPipChangePipStyle(MultiPipStyleInfo ROM *MPipStyle)
{
	BYTE 
		i,
		B_ChannelNumber;

	TunerID_t ActiveTuner = GetActiveTuner(gmvb_PipChannel);

	/*
	Is a defined styles?
	*/
	if (MPipStyle == NULL_PTR)
		return;

	/*
	Setup pointers and mapping table
	*/
	MPipPrivData.Stp_CurrentStyle = MPipStyle;

	memset(MPipPrivData.Mapping, 0, sizeof(MultiPipMapping) * MULTIPIP_MAX_WINDOWS);

	for (i = 0; i < MULTIPIP_MAX_WINDOWS; i++)
	{
		MPipPrivData.Mapping[i].ScanOrder = MPipPrivData.Stp_CurrentStyle->Ba_ScanOrder[i];
	}

	B_ChannelNumber = MPIP_START_CH_NUMBER;

#ifndef LCDTV_PAL
	{ // Blocked channel can't be visible in MPIP mode! SCR2660
	BYTE B_StartChannel = B_ChannelNumber;

		while (IsChannelBlocked(B_ChannelNumber))
		{

			B_ChannelNumber = TunerSearchChannel(ActiveTuner, B_ChannelNumber, ACTIVE_CHANNEL, 1);
			
			if (B_ChannelNumber == B_StartChannel)
				break;
		}
	};
#endif
	
	for (i = 0; i < MULTIPIP_MAX_WINDOWS; i++)
	{
		/*
		TODO: temporary.  this should be loaded from NVRAM

		Setup Source of each window, and setup the channel numbers to be initialized to an in-order
		scan of channel search.  The starting channel is based on MPIP_START_CH_NUMBER
		*/
		MPipPrivData.Mapping[i].InputSource = RF1;

		//
		// NOTE: For FLI8668 and FLI8538 only
		// Multipip now provides limited support for other inputs in addition to RF1. You may configure
		// the inputs as follows. Please note that plugging or unplugging cables of the input that the 
		// active multipip window is associated with may cause the whole multipip configuration to turn
		// black until the active window advances to another window.
		//
		/*if (i == 1)
			MPipPrivData.Mapping[i].InputSource = CVBS1;
		if (i == 2)
			MPipPrivData.Mapping[i].InputSource = COMP1;
		if (i == 3)
			MPipPrivData.Mapping[i].InputSource = HDMI1;
		*/
		if (MPipPrivData.Mapping[i].InputSource == RF1)
		{
			MPipPrivData.Mapping[MPipGetWindowFromScanOrder(i+1)].ChannelNumber = B_ChannelNumber;
			B_ChannelNumber = MPipSearchChannel(ActiveTuner, B_ChannelNumber, ACTIVE_CHANNEL, 1);
		}
		else
			MPipPrivData.Mapping[MPipGetWindowFromScanOrder(i+1)].ChannelNumber = 0;
	}
	
	MPipPrivData.MaxWindows = MPipGetMaxWindows();

	MPipPrivData.AnchorWindow = MPipPrivData.CurrentWindow = MPipGetWindowFromScanOrder(1);

	//MultiPipSetDisplay(gmvb_PipChannel);
}

//******************************************************************************
//
// FUNCTION       : void static MPipStartScan(void)
//
// USAGE          : Changes the multipip state machine state to MULTIPIP_SCANNING
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : MPipPrivData.Stp_CurrentStyle->ScanTimeout (R)			
//
// USED_REGS      : None
//
// PRE_CONDITION  : 
//
// POST_CONDITION : 
//
//******************************************************************************
void static MPipStartScan(void)
{
	MPipPrivData.CurrentState = MULTIPIP_SCANNING;
	MPipSetTimer(MPipPrivData.Stp_CurrentStyle->ScanTimeout);
	MuteControl(D_AUDIO_HEADPHONE, D_MUTE_ON);
}

//******************************************************************************
//
// FUNCTION       : void static MPipStartNavigate(void)
//
// USAGE          : Changes the multipip state machine state to MULTIPIP_NAVIGATING
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : MPipPrivData.Stp_CurrentStyle->NavigateTimeout (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  : 
//
// POST_CONDITION : 
//
//******************************************************************************
void static MPipStartNavigate(void)
{
	MPipPrivData.CurrentState = MULTIPIP_NAVIGATING;
	MPipSetTimer(MPipPrivData.Stp_CurrentStyle->NavigateTimeout);
	MuteControl(D_AUDIO_HEADPHONE, D_MUTE_OFF);
}

#pragma argsused
//******************************************************************************
//
// FUNCTION       : static BYTE MPipSearchChannel(
//                  	TunerID_t TunerID, 
//                  	BYTE B_CurrentChannel, 
//                  	TunerChannelType ChannelType, 
//                  	SBYTE sB_Offset)
//
// USAGE          : Wrapper for tuners search channel function given an offset
//
// INPUT          : TunerID
//                  B_CurrentChannel
//                  ChannelType
//                  sB_Offset
//
// OUTPUT         : B_CurrentChannel
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
static BYTE MPipSearchChannel(
	TunerID_t TunerID, 
	BYTE B_CurrentChannel, 
	TunerChannelType ChannelType, 
	SBYTE sB_Offset)
{
	BYTE B_StartChannel = B_CurrentChannel;		// Mark the current as start channel
	SBYTE Direction;

	if (sB_Offset > 0)
	{
		Direction = 1;
	}
	else
	{
		Direction = -1;
	}
	
	do {
		
#ifndef LCDTV_PAL
		B_CurrentChannel = TunerSearchChannel(TunerID, B_CurrentChannel, ChannelType, sB_Offset);
#else
		B_CurrentChannel = TunerSearchPreset(B_CurrentChannel, ChannelType, sB_Offset);
#endif
		sB_Offset = Direction;
	} while (IsChannelBlocked(B_CurrentChannel) && B_CurrentChannel != B_StartChannel);
	
	return B_CurrentChannel;
}

//******************************************************************************
//
// FUNCTION       : static void MPipSetPipOnTop(BOOL PipOnMain)
//
// USAGE          : Sets the Pip blending priority.  Pip on top, or Pip on bottom.
//
// INPUT          : PipOnMain
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
static void MPipSetPipOnTop(BOOL PipOnMain)
{
	if (PipOnMain == TRUE)
	{
		gm_BlenderSetPriority (PIP_ON_MAIN);
	} 
	else
	{
		gm_BlenderSetPriority (MAIN_ON_PIP);
	}
}
#endif 

//******************************************************************************
//
// FUNCTION       : BOOL IsMultiPipOn(void)
//
// USAGE          : Returns TRUE if MPip is on, and FALSE if MPip is off
//
// INPUT          : None
//
// OUTPUT         : TRUE or 0
//
// GLOBALS        : UserPrefPipMode (R)			
//
// USED_REGS      : None
//
// PRE_CONDITION  : 
//
// POST_CONDITION : 
//
//******************************************************************************
BOOL IsMultiPipOn(void)
{
#if (FEATURE_MultiPxp == ENABLE)
	#ifndef SMART_SWAP
	if (gmvb_MainChannel != CH_A) return FALSE;
	#endif

	return (UserPrefPipMode == MULTI_PIP);
#else
	return 0;
#endif	//#if (!(defined (NO_TUNER)))
}

//******************************************************************************
//
// FUNCTION       : void MultiPipStartIdle(void)
//
// USAGE          : Sets multipip state machine to MULTIPIP_IDLE
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : MULTIPIP_10ms_TMR timer			
//
// USED_REGS      : None
//
// PRE_CONDITION  : 
//
// POST_CONDITION : 
//
//******************************************************************************
void MultiPipStartIdle(void)
{
#if (FEATURE_MultiPxp == ENABLE)
	MPipPrivData.CurrentState = MULTIPIP_IDLE;
	gm_TimerStop(MULTIPIP_10ms_TMR);
#endif
}

//******************************************************************************
//
// FUNCTION       : void MultiPipHandler(void)
//
// USAGE          : Multipip state machine as called from mainloop
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : MPipPrivData.CurrentState (R)
//                  MPipPrivData.Stp_CurrentStyle->ScanTimeout (R)
//                  MPipPrivData.CurrentWindow (R)
//                  MPipPrivData.Stp_CurrentStyle->AnchorPresent (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  : 
//
// POST_CONDITION : 
//
//******************************************************************************
void MultiPipHandler(void)
{
#if (FEATURE_MultiPxp == ENABLE)
	static BYTE B_TimeoutCount = 0;
	
	if (!IsMultiPipOn())
   		return;

	//SCR1981 Switch Off Main Channel in 3x3Grig MPIP Mode
	//If detected new signal on Main. Then executed event REQ_MS_SETUP, and in ModeSetupFullSetup there is request for BLEND!
	if (MPipPrivData.Stp_CurrentStyle->W_MultiPipMask == MPIP_MASK_THREE_BY_THREE)
	{
		ModeSetupRequestCancel(CH_A, REQ_MS_BLENDER, PARAM_ANY);
	}
      
	switch (MPipPrivData.CurrentState)
    	{
		case MULTIPIP_IDLE:
			break; //Noop


		case MULTIPIP_SCANNING:
			/*
			Multipip SCAN (quick refresh of all pip windows) on timeout
			*/
			if (MPipTimeout())
			{
				/*
				Is Last Scan Window, and using an Anchor to stop?
				*/
				if (	MPipGetNextWindow(MPipPrivData.CurrentWindow) == MPipPrivData.AnchorWindow &&
					MPipPrivData.Stp_CurrentStyle->AnchorPresent)
				{
					MPipStartNavigate();
				}
				else
				{
					MPipSetTimer(MPipPrivData.Stp_CurrentStyle->ScanTimeout);
				}
				
				if (gm_IsModeComplete(gmvb_PipChannel) || (B_TimeoutCount >= MPIP_MAX_TIMEOUT_COUNT))
				{
					B_TimeoutCount = 0;
					MPipScanNextWindow();
				}
				else
					B_TimeoutCount++;
			}
			break;

		case MULTIPIP_NAVIGATING:
			/*
			Multipip NAVIGATING (when user changes focus of highlight window)
			*/
			if (MPipTimeout())
			{
				MPipStartScan();
			}
			break;

		default:
			msg("UNKNOWN STATE IN MULTIPIP HANDLER",0);
			break;
	}
#endif	
}
#if (FEATURE_MultiPxp == ENABLE)

//******************************************************************************
//
// FUNCTION       : static WORD DivideRoundUp(WORD W_Dividend, WORD W_Divisor)
//
// USAGE          : Divides the Dividend by the Divisor, then rounds up the result
//
// INPUT          : W_Dividend
//                  W_Divisor
//
// OUTPUT         : Quotient
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
static WORD DivideRoundUp(WORD W_Dividend, WORD W_Divisor)
{
	WORD W_Remainder;
	WORD W_Quotient;

	W_Quotient = W_Dividend / W_Divisor;
	W_Remainder = W_Dividend - (W_Quotient * W_Divisor);
	if ((W_Remainder * 2) >= W_Divisor)
		W_Quotient++;
	return W_Quotient;
}

//******************************************************************************
//
// FUNCTION       : static void MPipGetPipActivePosition(WORD *HActive, WORD *VActive)
//
// USAGE          : Calculates the configuration metrics for the mpip style.  These metrics include
//                  HActive and VActive
//
// INPUT          : HActive
//                  VActive
//
// OUTPUT         : None
//
// GLOBALS        : MPipPrivData.Stp_CurrentStyle->B_NumMultiPipCols (R)
//                  MPipPrivData.Stp_CurrentStyle->B_NumMultiPipRows (R)
//                  MPipPrivData.Stp_CurrentStyle->W_BorderWidth (R)
//                  MPipPrivData.Stp_CurrentStyle->W_BorderHeight (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  : 
//
// POST_CONDITION : 
//
//******************************************************************************
static void MPipGetPipActivePosition(WORD *HActive, WORD *VActive)
{
	WORD 
		W_TotalBorderWidths,
		W_TotalBorderHeights,
		W_ContributionHBorder,
		W_ContributionVBorder;
	
	/*
	Calculate the configuration border metrics
	*/
	W_TotalBorderWidths = (MPipPrivData.Stp_CurrentStyle->B_NumMultiPipCols + 1) * 
							MPipPrivData.Stp_CurrentStyle->W_BorderWidth;

	W_TotalBorderHeights = (MPipPrivData.Stp_CurrentStyle->B_NumMultiPipRows+ 1) * 
							MPipPrivData.Stp_CurrentStyle->W_BorderHeight;

	/*
	Subtract the averaged contribution width of border from each window HActive
	*/
	W_ContributionHBorder = DivideRoundUp(
								W_TotalBorderWidths, 
								MPipPrivData.Stp_CurrentStyle->B_NumMultiPipCols);

	W_ContributionVBorder = DivideRoundUp(
								W_TotalBorderHeights, 
								MPipPrivData.Stp_CurrentStyle->B_NumMultiPipRows);

	*HActive = MPipPrivData.Stp_CurrentStyle->W_PipWidth - W_ContributionHBorder;
	*VActive = MPipPrivData.Stp_CurrentStyle->W_PipHeight - W_ContributionVBorder;

	/*
	HActive cannot be odd in pixels
	*/
	*HActive &= ~1;
}
#endif	

//******************************************************************************
//
// FUNCTION       : void MultiPipSetDisplay(gmt_PHY_CH B_Channel)
//
// USAGE          : Sets the display configurations for output HActive, VActive, HStart, VStart for
//                  Pip or Main
//
// INPUT          : B_Channel - gmvb_MainChannel or gmvb_PipChannel
//
// OUTPUT         : None
//
// GLOBALS        : MPipPrivData.Stp_CurrentStyle->W_MainWidth (R)
//                  MPipPrivData.Stp_CurrentStyle->W_MainHeight (R)
//                  MPipPrivData.Stp_CurrentStyle->W_MultiPipHStart (R)
//                  MPipPrivData.Stp_CurrentStyle->W_MainVStart (R)
//                  gmvb_PipChannel (R)
//                  gmvb_MainChannel (R)
//                  gmvw_OutputHStart (W)
//                  gmvw_OutputStart (W)
//                  gmvw_OutputHActive (W)
//                  gmvw_OutputVActive (W)
//
// USED_REGS      : None
//
// PRE_CONDITION  : 
//
// POST_CONDITION : 
//
//******************************************************************************
void MultiPipSetDisplay(gmt_PHY_CH B_Channel)
{
#if (FEATURE_MultiPxp == ENABLE)
	WORD
		W_NewHActive,
		W_NewVActive,
		W_NewHStart,
		W_NewVStart;

	if (B_Channel==gmvb_MainChannel)
	{
		W_NewHActive = MPipPrivData.Stp_CurrentStyle->W_MainWidth;
		W_NewVActive = MPipPrivData.Stp_CurrentStyle->W_MainHeight;

		W_NewHStart = MPipPrivData.Stp_CurrentStyle->W_MainHStart;
		W_NewVStart = MPipPrivData.Stp_CurrentStyle->W_MainVStart;

		//HActive cannot be odd in pixels
		W_NewHActive &= ~1;
		
		msg("--MAIN-----------------------------",0);
		msg("gmvw_OutputHActive[gmvb_MainChannel] = %d", W_NewHActive);
		msg("gmvw_OutputVActive[gmvb_MainChannel] = %d", W_NewVActive);
		msg("gmvw_OutputHStart[gmvb_MainChannel] = %d", W_NewHStart);
		msg("gmvw_OutputVStart[gmvb_MainChannel] = %d", W_NewVStart);
		msg("---------------------------------------------------------------------------------",0);
	}
	else
	{
		MPipGetPipActivePosition(&W_NewHActive, &W_NewVActive);

		W_NewHStart = MPipPrivData.Stp_CurrentStyle->W_MultiPipHStart;
		W_NewVStart = MPipPrivData.Stp_CurrentStyle->W_MultiPipVStart;

		//HActive cannot be odd in pixels
		W_NewHActive &= ~1;

		msg("--PIP-----------------------------",0);
		msg("gmvw_OutputHActive[gmvb_PipChannel] = %d", W_NewHActive);
		msg("gmvw_OutputVActive[gmvb_PipChannel] = %d", W_NewVActive);
		msg("gmvw_OutputHStart[gmvb_PipChannel] = %d", W_NewHStart);
		msg("gmvw_OutputVStart[gmvb_PipChannel] = %d", W_NewVStart);
		msg("---------------------------------------------------------------------------------",0);
	}

	gmvw_OutputHActive[B_Channel] = W_NewHActive;
	gmvw_OutputVActive[B_Channel] = W_NewVActive;
	gmvw_OutputHStart[B_Channel] = W_NewHStart;
	gmvw_OutputVStart[B_Channel] = W_NewVStart;
#else
	B_Channel++;
#endif	
}

//******************************************************************************
//
// FUNCTION       : void MultiPipSetup(void)
//
// USAGE          : Function calls to setup multipip parameters
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : gmvb_PipChannel	(R)
//                  MPipPrivData.Stp_CurrentStyle (R)
//                  gmvw_OutputHActive (R)
//                  gmvw_OutputVActive (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  : 
//
// POST_CONDITION : 
//
//******************************************************************************
void MultiPipSetup(void)
{
#if (FEATURE_MultiPxp == ENABLE)

	if (Bt_IsFreshStart)
	{
		SetDisplayOff(gmvb_PipChannel, BACKGROUND_ONLY);
		gm_ScalerDynamicEnable(gmvb_PipChannel, FALSE);

		//
		// Enable multipip hardware
		// 
		MultiPipSetDisplay(gmvb_PipChannel);
		gm_PipSetMode(MODE_MULTI_PIP);
		gm_DisplaySetSize(gmvb_PipChannel);
		gm_DisplaySetPosition(gmvb_PipChannel);

	#if (FEATURE_BLENDER_FAILSAFE == REV1)
		//Blender failsafe
		//Patch up for rev AA
		if(gm_ReadProductRevReg() == 0)
		{
			gm_BlenderFailSafeOnOff(Off);
			gm_SyncUpdateWait(gmvb_PipChannel, OUTPUT_CLOCK);
		}
		//Patch for rev AB
		else if (gm_ReadProductRevReg() >= 1)
		{
			gm_BlenderFailSafeOnOff(Off);
			gm_SyncUpdateWait(gmvb_PipChannel, OUTPUT_CLOCK);
		}
	#elif (FEATURE_BLENDER_FAILSAFE == REV2)
		gm_BlenderFailSafeOnOff(Off);
		gm_SyncUpdateWait(gmvb_PipChannel, OUTPUT_CLOCK);
	#endif

		// Normally failsafe can handle the problem associated with the lack of data to
		//   blender on FLI8668 (disappearance of part of the screen); however, since
		//   failsafe is turned off above, the following logic is needed to handle the 
		//   blender bug. Second condition to fix SCR2670! It is nessesary to disable Main channel if MPIP is on full screen
		if ((!gm_IsValidSignal(CH_A)) || (MPipPrivData.Stp_CurrentStyle->W_MultiPipMask == MPIP_MASK_THREE_BY_THREE))
		{
			SetDisplayOff(CH_A, BLENDER_OFF);
			ModeSetupRequestCancel(CH_A, REQ_MS_BLENDER, PARAM_ANY);
		}

		// SCR2670 It is nessesary to switch of Detection for Main channel in 3x3 GRID MPIP mode
		// And to reduce bandwidth freez main channel		
                if (MPipPrivData.Stp_CurrentStyle->W_MultiPipMask == MPIP_MASK_THREE_BY_THREE)
		{
			gm_StateDetectionConfig(gmvb_MainChannel, DET_DISABLE);
			gm_MemoryWriteFreeze(gmvb_MainChannel, TRUE);
		}

		gm_ScalerSetup(gmvb_PipChannel);
		gm_MultiPipSetStyle(MPipPrivData.Stp_CurrentStyle->W_MultiPipMask);
		/*
		Is there a non-zero border?
		*/
		// gm_PipBorderEnable() must run after gm_MultiPipSetStyle()
		gm_PipBorderSetup(	MPipPrivData.Stp_CurrentStyle->W_BorderWidth, 
							MPipPrivData.Stp_CurrentStyle->W_BorderHeight,
							MPipPrivData.Stp_CurrentStyle->W_BorderColor);
		if (MPipPrivData.Stp_CurrentStyle->W_BorderWidth || MPipPrivData.Stp_CurrentStyle->W_BorderHeight)
			gm_PipBorderEnable(On);
		else
			gm_PipBorderEnable(Off);
		/*
		Force all the inactive pip windows in the multi pip configuration to BG color
		*/
		gm_MultiPipSetBkgnd(MPIP_BACKGROUND);

		// Set up highlight border
		gm_HighLightWindowSetup(	MPipPrivData.Stp_CurrentStyle->W_HighLightBorderWidth,
									MPipPrivData.Stp_CurrentStyle->W_HighLightBorderHeight,
									MPipPrivData.Stp_CurrentStyle->W_HighLightBorderColor);

		if (MPipPrivData.Stp_CurrentStyle->HighlightOrigin == ORIGIN_ON_BORDER)
		{
			gm_HighLightWindowSetSize(gmvw_OutputHActive[gmvb_PipChannel] + 
										2 * MPipPrivData.Stp_CurrentStyle->W_HighLightBorderWidth,
									  	gmvw_OutputVActive[gmvb_PipChannel] +
									  	2 * MPipPrivData.Stp_CurrentStyle->W_HighLightBorderHeight);
		}
		else
		{
			gm_HighLightWindowSetSize(gmvw_OutputHActive[gmvb_PipChannel],
									  	gmvw_OutputVActive[gmvb_PipChannel]);
		}
		gm_SetAutoMode(gmvb_PipChannel, AUTO_MUTE_DISABLE);//disable auto-background
		MPipSetWindow(MPipPrivData.CurrentWindow, TRUE, FALSE);
		Bt_IsFreshStart = FALSE;
	}
	else
	{
		gm_ScalerSetup(gmvb_PipChannel);
		MPipSetWindow(MPipPrivData.CurrentWindow, MPipPrivData.Stp_CurrentStyle->MovingHighlight, 
						!MPipPrivData.Stp_CurrentStyle->AnchorPresent);
	}

#endif	
}

//******************************************************************************
//
// FUNCTION       : void MultiPipIncWindow(void)
//
// USAGE          : Increments window to next one in scan order based on AnchorWindow
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : MPipPrivData.AnchorWindow			
//
// USED_REGS      : None
//
// PRE_CONDITION  : 
//
// POST_CONDITION : 
//
//******************************************************************************
void MultiPipIncWindow(void)
{
#if (FEATURE_MultiPxp == ENABLE)
	BYTE NextWindow = MPipGetNextWindow(MPipPrivData.AnchorWindow);
	MPipSetWindow(NextWindow, TRUE, TRUE);
	MPipStartNavigate();
#endif	
}

//******************************************************************************
//
// FUNCTION       : void MultiPipDecWindow(void)
//
// USAGE          : Decrements window to previous one in scan order based on AnchorWindow
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : MPipPrivData.AnchorWindow			
//
// USED_REGS      : None
//
// PRE_CONDITION  : 
//
// POST_CONDITION : 
//
//******************************************************************************
void MultiPipDecWindow(void)
{
#if (FEATURE_MultiPxp == ENABLE)
	BYTE PrevWindow = MPipGetPrevWindow(MPipPrivData.AnchorWindow);
	MPipSetWindow(PrevWindow, TRUE, TRUE);
	MPipStartNavigate();
#endif	
}

//******************************************************************************
//
// FUNCTION       : void MultiPipPageUp(void)
//
// USAGE          : Wrapper to change channel upwards by delta amount equal to number of mpip windows
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : MPipPrivData.MaxWindows			
//
// USED_REGS      : None
//
// PRE_CONDITION  : 
//
// POST_CONDITION : 
//
//******************************************************************************
void MultiPipPageUp(void)
{
#if (FEATURE_MultiPxp == ENABLE)
	MPipDeltaChannelChange(MPipPrivData.MaxWindows);
#endif	
}

//******************************************************************************
//
// FUNCTION       : void MultiPipPageDown(void)
//
// USAGE          : Wrapper to change channel downwards by delta amount equal to -ve number of mpip windows
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : MPipPrivData.MaxWindows			
//
// USED_REGS      : None	
//
// PRE_CONDITION  : 
//
// POST_CONDITION : 
//
//******************************************************************************
void MultiPipPageDown(void)
{
#if (FEATURE_MultiPxp == ENABLE)
	MPipDeltaChannelChange(-MPipPrivData.MaxWindows);
#endif	
}

//******************************************************************************
//
// FUNCTION       : void MultiPipChannelUp(void)
//
// USAGE          : Wrapper to change channel upwards by delta amount equal to 1
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
void MultiPipChannelUp(void)
{
#if (FEATURE_MultiPxp == ENABLE)
	MPipDeltaChannelChange(1);
#endif	
}
//******************************************************************************
//
// FUNCTION       : void MultiPipChannelDown(void)
//
// USAGE          : Wrapper to change channel downwards by delta amount equal to -1
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
void MultiPipChannelDown(void)
{
#if (FEATURE_MultiPxp == ENABLE)
	MPipDeltaChannelChange(-1);
#endif	
}

//******************************************************************************
//
// FUNCTION       : void MultiPipSetScanMode(MultiPipScanMode ScanMode)
//
// USAGE          : Accessor function to set mpip scan mode
//
// INPUT          : ScanMode
//
// OUTPUT         : None
//
// GLOBALS        : MPipPrivData.ScanMode	(W)		
//
// USED_REGS      : None
//
// PRE_CONDITION  : 
//
// POST_CONDITION : 
//
//******************************************************************************
void MultiPipSetScanMode(MultiPipScanMode ScanMode)
{
#if (FEATURE_MultiPxp == ENABLE)
	if (ScanMode > NUM_SCAN_MODES)
		return;

	MPipPrivData.ScanMode = ScanMode;
#else
	ScanMode++;
#endif	
}

//******************************************************************************
//
// FUNCTION       : void MultiPipTurnOn(MultiPipStyleInfo ROM *MPipStyle)
//
// USAGE          : Entry point for Application interface to turn on multipip
//
// INPUT          : MPipStyle
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
void MultiPipTurnOn(MultiPipStyleInfo ROM *MPipStyle)
{
#if (FEATURE_MultiPxp == ENABLE)
	MPipInit(MPipStyle);
	MPipStartScan();
#else
	MPipStyle++;
#endif	
}

//******************************************************************************
//
// FUNCTION       : void MultiPipTurnOff(void)
//
// USAGE          : Application interface to turn off multipip
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : gmvb_MainChannel (R)
//                  gmvb_PipChannel (R)
//                  MPipPrivData.PreviousPipMode (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void MultiPipTurnOff(void)
{
#if (FEATURE_MultiPxp == ENABLE)
	MultiPipStartIdle();

	/*
	Restore previous Pip Mode.
	*/
	MPipSetCurrentPipMode(MPipPrivData.PreviousPipMode);

	/*
	Trigger Mode Setup to change the display back
	*/
	PreModeChange(gmvb_MainChannel);
	PreModeChange(gmvb_PipChannel);

	gm_HighlightBorder(FALSE,0,0,0,0);
	MPipSetPipOnTop(FALSE);

	#if (FEATURE_TWO_STAGE_PROCESSING_MODE == ENABLE)
	if ((UserPrefPipMode == NO_PIP) && IsTwoStageAllowed())
	{
		gm_MemoryWriteFreeze(gmvb_PipChannel, FALSE);
		ModeSetupDisplayChange(gmvb_MainChannel, DISPLAY_CHANGE_QUICK);
	}
	else
	#endif
	{
//SCR 2671 Change sequence of scaling - first PIP then MAIN

		gm_MemorySetup(gmvb_PipChannel);	//reset the PFRC registers to prior-multipip configuation

	#ifdef RESTORE_PIP_INPUT_AFTER_EXIT
	if (((gmvb_ChannelSource[gmvb_MainChannel] == AIP_SD_VID)
		||(gmvb_ChannelSource[gmvb_MainChannel] == AIP_HD_GFX))
		&&
		((MPipPrivData.PreviousPipChannelSource == AIP_SD_VID)
		||(MPipPrivData.PreviousPipChannelSource == AIP_HD_GFX)))
	{
		// do not restore if there is a AIP conflict with the current main source and the
		// previous pip souce that we were going to change it back to
	}
 	else
 	{
		//restore previous pip input
		UserPrefCurrentInputPip = MPipPrivData.PreviousPipInput;

		gm_BlenderOnOff(gmvb_PipChannel, FALSE);//this fixes the mysterious whole-screen blinking
		AdjustCurrentInputPip();
 	}
	#endif
		gm_MemoryWriteFreeze(gmvb_PipChannel, FALSE);
		ModeSetupDisplayChange(gmvb_PipChannel, DISPLAY_CHANGE_QUICK);
		//ModeSetupFullSetup(gmvb_PipChannel);	// Must be a FULL setup in the PIP

		ModeSetupDisplayChange(gmvb_MainChannel, DISPLAY_CHANGE_NORMAL);//SCR 2671
	}
	#if (FEATURE_BLENDER_FAILSAFE == REV1)
	//Patch for rev AB
	if (gm_ReadProductRevReg() >= 1)
		gm_BlenderFailSafeOnOff(On);
	#elif (FEATURE_BLENDER_FAILSAFE == REV2)
	gm_BlenderFailSafeOnOff(On);
	#endif
	MPipSetPipOnTop(TRUE);

	/*
	Signal OSD that multipip has been turned off.
	*/
	OSDRequestMsg(gmvb_MainChannel, MTO_MultiPipOff, REQUEST_NOW);

	// SCR2670 It is nessesary to switch of Detection for Main channel in 3x3 GRID MPIP mode
	// And to reduce bandwidth freez main channel
        if (MPipPrivData.Stp_CurrentStyle->W_MultiPipMask == MPIP_MASK_THREE_BY_THREE)
	{
		gm_StateDetectionConfig(gmvb_MainChannel, DET_RESTART);
		gm_MemoryWriteFreeze(gmvb_MainChannel, FALSE);
	}

	#ifndef LCDTV_PAL
		//SCR2660 no need to continue show message 
		if (!((GetChannelConnType(gmvb_MainChannel) == TUNER) && (TunerChannelBlockedStatus(gmvb_MainChannel))))
	{
		UserPrefChannelBlockStateFlagPip = 0;	
			OSDRequestMsg(gmvb_PipChannel, MTO_TV_ChannelUnBlocked, REQUEST_NOW);
	}
	#endif //#ifndef LCDTV_PAL
#endif	
}

//******************************************************************************
//
// FUNCTION       : void MultiPipSetSourceInMain(void)
//
// USAGE          : Application interface function to set the mpip source/chanel into the main channel
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : MPipPrivData.Mapping array (R)			
//
// USED_REGS      : None
//
// PRE_CONDITION  : 
//
// POST_CONDITION : 
//
//******************************************************************************
void MultiPipSetSourceInMain(void)
{
#if (FEATURE_MultiPxp == ENABLE)	
	TunerID_t ActiveTuner = GetActiveTuner(gmvb_MainChannel);
	MultiPipTurnOff();

#if ((FEATURE_DECODER_TUNER_NUM == 2) && (MAX_TUNER_INPUTS == 2))
	//do not change the value of MPIP_MAIN_INPUT_SOURCE if InputSource is RF1
	//  this logic is put in because MPipChangePipStyle() defaults all InputSource to RF1 and
	//  MPIP_MAIN_INPUT_SOURCE can be already set to RF2 due to channel mapping
	if (!((MPipPrivData.Mapping[MPipPrivData.AnchorWindow].InputSource == ALL_BOARDS_RF1)
		&& ((MPIP_MAIN_INPUT_SOURCE == ALL_BOARDS_RF1)||(MPIP_MAIN_INPUT_SOURCE == ALL_BOARDS_RF2))))
		MPIP_MAIN_INPUT_SOURCE = MPipPrivData.Mapping[MPipPrivData.AnchorWindow].InputSource;

	if ((MPIP_MAIN_INPUT_SOURCE == ALL_BOARDS_RF1)||(MPIP_MAIN_INPUT_SOURCE == ALL_BOARDS_RF2))
	{
		if (UserPrefCurrentInputMain == UserPrefCurrentInputPip)
		{
			if (UserPrefCurrentInputPip == ALL_BOARDS_RF1)
				UserPrefCurrentInputMain = ALL_BOARDS_RF2;
			else if (UserPrefCurrentInputPip == ALL_BOARDS_RF2)
				UserPrefCurrentInputMain = ALL_BOARDS_RF1;
		}

		if (UserPrefCurrentInputMain == ALL_BOARDS_RF1)
			ActiveTuner = TUNER_INPUT_0;
		else if (UserPrefCurrentInputMain == ALL_BOARDS_RF2)
			ActiveTuner = TUNER_INPUT_1;
	}
	
	if ((MPIP_MAIN_INPUT_SOURCE == ALL_BOARDS_RF1)||(MPIP_MAIN_INPUT_SOURCE == ALL_BOARDS_RF2))
#else
	MPIP_MAIN_INPUT_SOURCE = MPipPrivData.Mapping[MPipPrivData.AnchorWindow].InputSource;

	if (MPIP_MAIN_INPUT_SOURCE == ALL_BOARDS_RF1)
#endif
	{
		TunerSetChannel (ActiveTuner,
						MPipPrivData.Mapping[MPipPrivData.AnchorWindow].ChannelNumber,
						BLANK_NONE);
	}
	AdjustCurrentInputMain();
#endif	
}



/***********************************  END  ************************************/

