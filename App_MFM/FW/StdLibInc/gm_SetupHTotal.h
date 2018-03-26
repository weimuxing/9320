/*
	$Workfile:   gm_SetupHTotal.h  $
	$Revision: 1.6 $
	$Date: 2011/12/14 10:35:31 $
*/
#ifndef __GM_SETUPHTOTAL_H__
#define __GM_SETUPHTOTAL_H__

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
// MODULE:      gm_SetupHTotal.h
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

//******************************************************************************
//  D E F I N E S
//******************************************************************************

#define SDDS_K_MAIN_MIN			1
#define SDDS_K_MAIN_SHIFT		1
#define SDDS_K_DIFF_MIN			1
#define SDDS_K_DIFF_SHIFT		4

void gm_SetupHTotal(WORD W_Htotal, gmt_PHY_CH B_Channel);

#endif __GM_SETUPHTOTAL_H__
