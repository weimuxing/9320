/*
	$Workfile:   osd_inputCapture.h  $
	$Revision: 1.7 $
	$Date: 2013/04/08 02:01:38 $
*/
#ifndef __OSD_INPUTCAPTURE_H__
#define __OSD_INPUTCAPTURE_H__
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
// MODULE:      osd_inputCapture.h
//
// USAGE:
//
//******************************************************************************

//******************************************************************************
//  T Y P E    D E F I N I T I O N S
//******************************************************************************

typedef enum
{
   AR_FULL_SCREEN,
   AR_LETTER_BOX,
   AR_PILLAR_BOX,
   AR_PILLAR_BOX_EXPAND,
   AR_LETTER_BOX_EXPAND,
   AR_WATER_GLASS,
   AR_PANORAMIC,
	AR_16V10,
   NUM_ASPECT_RATIOS,							// Max number selection
} ASPECT_RATIOS_MODE;

typedef enum
{
   OUTPUT_AR_FULL,
   OUTPUT_AR_16_9 = OUTPUT_AR_FULL,
   OUTPUT_AR_AUTO,
   OUTPUT_AR_4_3,
   OUTPUT_AR_1_1,
   NUM_OUTPUT_ASPECT_RATIOS = OUTPUT_AR_1_1,							// Max number selection
} OUTPUT_ASPECT_RATIOS_MODE;

#define AR_AUTO 	AR_FULL_SCREEN				// Use by WB only

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************

//******************************************************************************
//  E X T E R N A L    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

void AdjustHTotal			(void);
void AdjustHTotalPip		(void);
void AdjustHSyncPhase		(void);
void AdjustHSyncPhasePip	(void);
void AdjustHStart			(void);
void AdjustHStartPip		(void);
void AdjustVStart			(void);
void AdjustVStartPip		(void);
void AdjustPipHStart		(void);
void AdjustPipVStart		(void);
void AutoAdjust				(void);
void AdjustAspectRatioMain	(void);
void AdjustAspectRatioPip	(void);
void AdjustAutoAspectRatio  (void);
BOOL IsPanoramicAvailable 	(void);
void Zoom					(void);
BOOL IsZoom(void);
void Pan					(void);
void RelocateInputCaptureWindow(gmt_PHY_CH B_Channel, gmt_WindowStruct * pPrevInputDim,
                                gmt_WindowStruct * pNewInputDim);
gmt_ASPECT_RATIO GetDisplayAspectRatio(void);
ASPECT_RATIOS_MODE GetNextMainAspectRatio(ASPECT_RATIOS_MODE W_CurrentAspect);
ASPECT_RATIOS_MODE GetNextPipAspectRatio(ASPECT_RATIOS_MODE W_CurrentAspect);
ASPECT_RATIOS_MODE GetNextAspectRatio	(gmt_PHY_CH B_Channel,
                                       ASPECT_RATIOS_MODE W_CurrentAspect);
ASPECT_RATIOS_MODE GetPrevMainAspectRatio(ASPECT_RATIOS_MODE W_CurrentAspect);
ASPECT_RATIOS_MODE GetPrevPipAspectRatio(ASPECT_RATIOS_MODE W_CurrentAspect);
ASPECT_RATIOS_MODE GetPrevAspectRatio	(gmt_PHY_CH B_Channel,
                                       ASPECT_RATIOS_MODE W_CurrentAspect);

//
// MIN and MAX FUNCTIONS
//
WORD MinHTotal				(void);
WORD MaxHTotal				(void);
SWORD MinHStart				(void);
SWORD MaxHStart				(void);
SWORD MinVStart				(void);
SWORD MaxVStart				(void);
WORD MinPipHStart			(void);
WORD MaxPipHStart			(void);
WORD MinPipVStart			(void);
WORD MaxPipVStart			(void);


#endif __OSD_INPUTCAPTURE_H__
//*********************************  END  **************************************

