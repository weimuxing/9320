/*
	$Workfile:   adj_Display.h  $
	$Revision: 1.9 $
	$Date: 2013/05/14 02:14:50 $
*/
#ifndef __ADJ_DISPLAY_H__
#define __ADJ_DISPLAY_H__

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
// MODULE:      adj_display.h
//
// USAGE:
//
//******************************************************************************
//******************************************************************************
//  D E F I N E S
//******************************************************************************
typedef enum
{
   SEC_IN_OUT_WIN,
   PRI_IN_OUT_WIN,
   SEC_IN_PRI_OUT_WIN,
   PRI_IN_SEC_OUT_WIN
};

//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
BOOL SetBrightnessPWM(SBYTE Brightness);
void CscUpdateColors (gmt_PHY_CH Channel);
void SetupColorSpace	(gmt_PHY_CH B_Channel);
void Freeze				(gmt_PHY_CH B_Channel, BOOL B_State);
void SetPipBorder(gmt_PHY_CH B_Channel, BOOL B_State);
void DisplayLockSource		(void);
void DisplayClockLock(gmt_PHY_CH B_Channel, BYTE B_NumOfSteps);
void SetWindowSharpness(WORD FilterVal, WORD W_HorStart, WORD W_HorEnd, WORD W_VerStart, WORD W_VerEnd, BYTE B_NeedUpdated);

#endif

