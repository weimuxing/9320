/*
	$Workfile:   Input.h  $
	$Revision: 1.4 $
	$Date: 2011/12/14 10:35:32 $
*/
#ifndef __INPUT_H__
#define __INPUT_H__
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
// MODULE:      Input.h
//
// USAGE:       Header file for all Input definitions.
//
//******************************************************************************

//******************************************************************************
//  E X T E R N S
//******************************************************************************

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************

void InitInput(void);
void SelectInput(gmt_PHY_CH B_Channel, gmt_UserDefConnNames B_InputConnector,
                 gmt_PortSelectMode B_SelectMode);
BOOL IsDecoderUsed		(gmt_PHY_CH B_Channel);
void ConfigureExtDevicesVideo(gmt_UserDefConnNames B_InputConnector);
void ConfigureExtDevicesAudio(gmt_UserDefConnNames B_InputConnector);
void ExternalDeviceInit (void);
BOOL IsDVI_Connected(void);
BOOL IsHDMI_Connected(void);
BOOL IsMainSourceValid(BYTE B_DesiredMain);
BOOL IsPipSourceValid(BYTE B_DesiredPip);
BOOL IsAdcCalibrationRequired(void);
BOOL IsCCValid(void);
BOOL IsTtxValid(void);
void ChangeCCFieldParity(gmt_PHY_CH B_Channel);
BOOL IsVideoEnhancementValid (gmt_VIDEO_PROCESSING D_Mode);
void AutoScan(gmt_PHY_CH B_Channel);
#endif //__INPUT_J
