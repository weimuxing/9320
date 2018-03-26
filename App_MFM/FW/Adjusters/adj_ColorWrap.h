/*
	$Workfile:   adj_ColorWrap.h  $
	$Revision: 1.6 $
	$Date: 2012/09/13 05:44:50 $
*/
#ifndef __COLORWARP_H__
#define __COLORWARP_H__
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
// MODULE:      adj_ColorWrap.h
//
// USAGE:
//
//******************************************************************************


//
// Six-Axis Color Control color axis type definition
//
typedef enum SixAxisEnum
{
   RED_AXIS = 0,
   GREEN_AXIS,
   BLUE_AXIS,
   CYAN_AXIS,
   YELLOW_AXIS,
   MAGENTA_AXIS,
	#if (FEATURE_COLORWARP == REV2)
	BLACK_AXIS,
	WHITE_AXIS,
	#endif
   MAX_AXIS_NUM
}SixAxisType;


//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
#if (FEATURE_COLORWARP == REV1)
void AdjustSixAxisColor(BYTE AxisId);
#else // (FEATURE_COLORWARP == REV2)
void AdjustColorWarp(BYTE AxisId);
#endif // (FEATURE_COLORWARP == REV1)

BOOL IsColorWarpApply(void);

#ifdef DELL_U2410F//120523 Edward for SixAxis color restore default by scheme mode
void AdjustColorWarpDefault(BYTE AxisId);
#endif

#endif // __COLORWARP_H__
