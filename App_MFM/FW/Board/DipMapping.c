/*
	$Workfile:   DipMapping.c  $
	$Revision: 1.6 $
	$Date: 2013/01/08 04:07:21 $
*/

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
// MODULE:      DipMapping.c
//
// USAGE:       This module contains the board specific Dip mapping info
//
//******************************************************************************

// Add 3 boards
#if defined(EV1_93xx_BOARD)
#include	"Board\EV1-93xx\DipMapping.c"
#elif	defined(EV2_93xx_BOARD)
#include	"Board\EV2-93xx\DipMapping.c"
#elif	defined(RD1_93xx_BOARD)
#include	"Board\RD1-93xx\DipMapping.c"
#elif	defined(ST_4K2K_93xx_BOARD)
#include	"Board\ST4K2K\DipMapping.c"
#else
// Dummy function
void gm_InitDipMapping(void)
{
}
#endif
