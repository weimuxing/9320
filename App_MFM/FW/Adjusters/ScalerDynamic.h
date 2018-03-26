/*
	$Workfile:   ScalerDynamic.h  $
	$Revision: 1.3 $
	$Date: 2011/12/15 03:52:08 $
*/
#ifndef __SCALER_DYNAMIC_H__
#define __SCALER_DYNAMIC_H__
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
// MODULE:      ScalerDynamic.h
//
// USAGE:
//
//******************************************************************************

//******************************************************************************
//  D E F I N E S
//******************************************************************************

#define DYN_COAST						4	// Coast Iterations; # of dynamic scaling
// sequences per request (per depression
// of remote button).

#define DYN_COAST_INTERVAL				64	// ms; must be close to 4 VSyncs time otherwise dynamic scaling will not finish completely

#define	DYN_IDLE						0	// Dynamic Scaling
#define	DYN_DIR_INCREASE				1
#define	DYN_DIR_DECREASE				2
#define	DYN_DIR_RESET					3

#define NUM_VSYNCS						4	// # of VSyncs between any two steps

//Overall window size change (in pixels) in one dynamic scaling sequence
#define DYN_MIN_PIXELS_PER_SEQ 			4
#define DYN_MAX_PIXELS_PER_SEQ	 		4		// Change to 30 or so to have a 
// non-linear ramp (step size)
// Increases when key held


//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************

extern	WORD	W_MvfCtrlSave;
extern	WORD	W_MfrcCtrlSave;
extern	SWORD	SW_DynScalePosition;
extern	BYTE	B_DynScalingDirection;
extern  BYTE	B_DynScalingCoast;
extern  BYTE	B_DynPixelsPerSeq;

//******************************************************************************
//  E X T E R N A L    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

void SetDynamicScalerSizeBegin	(gmt_PHY_CH B_Channel);
void SetDynamicScalerSizeEnd	(gmt_PHY_CH B_Channel);
void SetDynamicScalerStep		(gmt_PHY_CH B_Channel, BYTE B_Step);
void DynamicScalerStart			(gmt_PHY_CH B_Channel);
void DynamicScalerStop			(void);
void DynamicScalerProcess		(BOOL Bt_SyncBothWinSize);
void DynamicScalerSideBySide	(BYTE B_Direction);
void ShowDynamicScaler			(BYTE B_Direction);
BOOL IsDynamicScalerBypassStepInserted(gmt_PHY_CH B_Channel,
                                       BOOL Bt_InputFixed, gmt_WindowStruct* WinPrevDim);
WORD GetNextDynamicScalingIndex(BOOL Bt_IsNext,
                                WORD W_CurrentIndex, WORD W_MaxStepNum);

#endif __SCALER_DYNAMIC_H__
//*********************************  END  **************************************

