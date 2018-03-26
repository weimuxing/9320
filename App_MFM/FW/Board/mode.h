/*
	$Workfile:   mode.h  $
	$Revision: 1.9 $
	$Date: 2012/07/13 07:55:55 $
*/

#ifndef __MODE_H__
#define __MODE_H__
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
//  MODULE: 	mode.h
//
//  USAGE: 	This module contains the supported modes look up tables
//			information
//
//******************************************************************************

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************

#if FEATURE_AIP == ENABLE
//
// Standard mode table definition (MUST be updated with mode.c)
//
extern ROM BYTE NumOfModeTableHD;   // Std=17
extern ROM BYTE NumOfModeTable60;   // Std=39
extern ROM BYTE NumOfModeTable70;   // Std=39
extern ROM BYTE NumOfModeTable85;   // Std=14

#define NumOfGraphicsPresetMode     NumOfModeTable60 + NumOfModeTable70 + NumOfModeTable85 + NumOfModeTableHD
#define NUM_GRAPHICS_PRESETMODE     120      // Reserve 120 preset modes for NVRAM space

#else
#define NumOfGraphicsPresetMode     0
#define NUM_GRAPHICS_PRESETMODE     0

#endif

//
// Overlap Flags used in Mode Determine
//
typedef enum
{
   gmd_MODE_OVERLAPPED_GR640_GR720  = BIT0,	// Non-resolved graphics 640x350, 640x400, 720x350, and 720x400 modes.
   gmd_MODE_OVERLAPPED_480P         = BIT1,	// Non-resolved 480P and graphics 640x480@60Hz modes
   gmd_MODE_OVERLAPPED_1080_576     = BIT2,	// Non-resolved 1080i and 576p modes
   gmd_MODE_OVERLAPPED_240p_480i    = BIT3,	// Non-resolved 240p and 480i modes

   gmd_MODE_OVERLAPPED_HWIDTH       = BIT4,
   gmd_MODE_OVERLAPPED_VWIDTH       = BIT5,
   gmd_MODE_OVERLAPPED_SYNCTYPE     = BIT6,
} gmt_OVERLAP_FLAGS;

// Height condition checked for SOG signal instead polarity
enum
{
   OVERLAP_GR350,       // Height of graphics overlapped modes is 350
   OVERLAP_GR400        // Height of graphics overlapped modes is 400
};

//
// Standard Input timing data
//
typedef struct
{
   WORD W_StdHFreq;				// Standard H. frequency in 100 Hz unit
   BYTE B_DeltaHFreq;				// H. frequency tolerance in 100 Hz unit
   WORD W_StdVFreq;				// Standard V. frequency in 1/10 Hz unit
   BYTE B_DeltaVFreq;				// V. frequency tolerance in 1/10 Hz unit
   WORD W_StdVTotal;				// Standard V. total in line unit
   BYTE B_DeltaVTotal; 				// V. total tolerance in line unit
   WORD W_StdHTotal;				// Standard H. total in pixel unit
   WORD W_StdWidth;				// Standard width in pixel unit
   WORD W_StdHeight;				// Standard height in line unit
   WORD W_StdHStart;				// Standard H. start in pixel unit
   WORD W_StdVStart;				// Standard V. start in line unit
   BYTE B_StdHpulse;				// Standard HS pulse width in TCLK + delta (MAX)
   // Use for VideoOverlapped mode only
   WORD W_ModeType;				// Mode type
   WORD W_OverlapType;			// Overlap type
} StandardModeStruct;

//******************************************************************************
//  E X T E R N A L    D E F I N I T I O N S
//******************************************************************************

#if FEATURE_AIP == ENABLE
extern ROM StandardModeStruct  CompHD_ModeTable[];
extern ROM StandardModeStruct  StandardModeTable_60[];
extern ROM StandardModeStruct  StandardModeTable_70[];
extern ROM StandardModeStruct  StandardModeTable_85[];
extern ROM StandardModeStruct  StandardModeTable_OOR[];
#endif

#ifdef EXTERNAL_DECODER
extern ROM StandardModeStruct  IntDecoder_ModeTable[];
extern ROM StandardModeStruct  ExtDecoder_ModeTable[];
#endif

extern ROM StandardModeStruct  HDMI_ModeTable[];


#endif //__MODE_H__
//*********************************  END  **************************************
