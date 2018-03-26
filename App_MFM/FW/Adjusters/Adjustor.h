/*
	$Workfile:   Adjustor.h  $
	$Revision: 1.5 $
	$Date: 2012/07/09 06:40:44 $
*/
#ifndef __ADJUSTOR_H__
#define __ADJUSTOR_H__

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
// MODULE:      Adjustor.h
//
// USAGE:
//
//******************************************************************************

//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

void SetupHTotal(WORD W_Htotal);
void MadiSetup		  	(gmt_PHY_CH B_Channel);

#if 1 // not support CC
#define CC_ClearDisplay(x)
#define CC_BlockDisplay(x)
#define CC_EnableDisplay(x)
#endif

#endif
//*********************************  END  **************************************
