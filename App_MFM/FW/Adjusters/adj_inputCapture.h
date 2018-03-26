/*
	$Workfile:   adj_inputCapture.h  $
	$Revision: 1.4 $
	$Date: 2011/12/13 10:32:34 $
*/
#ifndef __ADJ_INPUTCAPTURE_H__
#define __ADJ_INPUTCAPTURE_H__

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
// MODULE:      adj_inputcapture.h
//
// USAGE:
//
//******************************************************************************

//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

void InputCaptureCrop(gmt_PHY_CH B_Channel);

gmt_ASPECT_RATIO GetAspectRatio(gmt_PHY_CH B_Channel);

void SelectAspectMain(void);
void SetDisplayMain(void);

void SelectAspectPip(void);
void SetDisplayPip(void);
void SetCapture(gmt_PHY_CH B_Channel);
void ApplyOutputAspect(gmt_PHY_CH B_Channel, BYTE B_Method);
BOOL SetFlexiblePipWindowLimit(WORD Hstart, WORD Vstart, WORD Width, WORD Height);
BOOL SetFlexiblePipWindow(WORD Hstart, WORD Vstart, WORD Width, WORD Height);
gmt_WindowStruct GetFlexiblePipWindow(void);


#endif
