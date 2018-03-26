/*
	$Workfile:   ModeDeterm.h  $
	$Revision: 1.4 $
	$Date: 2013/03/19 06:11:57 $
*/
#ifndef __MODEDETERM_H__
#define __MODEDETERM_H__

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
// MODULE:      ModeDeterm.h
//
//******************************************************************************

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************

#define IsPresetMode(B_channel) ((stInput[B_Channel].ModeFlags & (gmd_MODE_UNKNOWN | gmd_MODE_ESTIMATED)) == 0)

//To check whether the mode is CVT mode
#ifdef CVT_MODES_SUPPORT
#define IsCvtMode(B_Channel)		\
				((stInput[B_Channel].ModeFlags & gmd_MODE_CVT) == gmd_MODE_CVT)
#endif

#ifdef ESTIMATED_MODES_SUPPORT
#define IsEstimatedMode(B_Channel)		\
				((stInput[B_Channel].ModeFlags & gmd_MODE_ESTIMATED) == gmd_MODE_ESTIMATED)
#endif

typedef enum
{
   PICK_FIRST = 1,
   PICK_SECOND,
   PICK_THIRD,
   PICK_FOURTH,
}OVERLAP_MODE_POLICY;

typedef enum
{
   INTERNAL_OVERLAP_HANDLING,
   EXTERNAL_OVERLAP_HANDLING,
}OVERLAP_MODE;

//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

BOOL ModeDetermine				(gmt_PHY_CH B_Channel);
BOOL IsMode1080i				(gmt_PHY_CH B_Channel);
BOOL IsModeNTSC					(gmt_PHY_CH B_Channel);
BOOL IsInputPal					(void);
BOOL IsDecoderTimingPAL			(gmt_DECODER_ID B_DecoderID);
BOOL IsGraphics					(gmt_PHY_CH B_Channel);

#ifdef DELL_Docking_PR02X_DVI_Patch
// 20121006
void Reback_DVI_Setting(void);
#endif
#endif	//#ifndef __MODEDETERM_H__
//*********************************  END  **************************************


