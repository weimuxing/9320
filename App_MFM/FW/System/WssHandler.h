#ifndef __WssHandler_H__
#define __WssHandler_H__

//*******************************************************************************
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
// MODULE:	WssHandler.H
//
// USAGE:	Header file for Wss Handler
//
//*******************************************************************************

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************

extern WORD gmvw_ModeTableVTotal[NUM_CHANNELS];

//******************************************************************************
//	G L O B A L    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

BOOL IsWssStatusValid(gmt_PHY_CH B_Channel, Wss_Mode W_Mode);
void WssDetectSetup(gmt_PHY_CH B_Channel);
void WssStatusHandler(gmt_PHY_CH B_Channel);

#endif
//*********************************  END  **************************************

