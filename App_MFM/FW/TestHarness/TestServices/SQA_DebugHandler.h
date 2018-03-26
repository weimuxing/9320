#ifndef __SQA_DEBUG_HANDLER_H__
#define __SQA_DEBUG_HANDLER_H__
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
// MODULE:      SQA_DebugHandler.h
//
// USAGE:       Header file for application sqa debug handler definition
//
//******************************************************************************

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************

//******************************************************************************
//  G L O B A L    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************


void SQA_DebugHandler(gmt_PHY_CH B_Channel);
void SQA_ModeDetectCVBS(gmt_PHY_CH B_Channel);
void SQA_ModeDetectComponent(gmt_PHY_CH B_Channel);
void SQA_ModeDetectHDMI(gmt_PHY_CH B_Channel);
void SQA_ModeDetectVGA(gmt_PHY_CH B_Channel);
void SQA_ModeDetectVGA_AutoAdjust(BOOL passed);

#endif // __SQA_DEBUG_HANDLER_H__
//*********************************  END  **************************************
