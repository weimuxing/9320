/*
	$Workfile:   TNRTbl.c  $
	$Revision: 1.1 $
	$Date: 2012/03/15 08:08:41 $
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
// MODULE:	TNRTbl.c
//
// USAGE:	This file contains table for TNR
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#include "StdLibInc\gm_ChipFeature.h"
#if (FEATURE_TNR == REV3)
#include "StdLibInc\gm_TNR.h"
//******************************************************************************
//  E X T E R N S
//******************************************************************************

//***************************************************************************
//  C O D E
//***************************************************************************
VQ_TNR_Parameters_t ROM TNRParams_High = 
{
	/*
	{
		0, // sizeof(VQ_TNR_Parameters_t),
		0, // STLAYER_VIDEO_TNR,
		0
	},
   */
	0,		// EnableGlobalNoiseAdaptive
	0,		// EnableGlobalMotionAdaptive
	3,		// GlobalMotionStdTh
	4800,		// GlobalMotionHighTh
	17,		// GlobalMotionPanTh
	-8,		// GlobalMotionStd
	-2,		// GlobalMotionHigh
	-3,		// GlobalMotionPan
	// EnableSNR, SNRLumaSensitivity, SNRLumaStrength, SNRChromaSensitivity, SNRChromaStrength,
	// YKMin, UVKMin, YGrad, FleshtoneGrad, ChromaGrad
	{
		{ // 0
			1, 32, 0, 32, 0, 2, 2, 28, 48, 32,
		},
		{ // 1
			0, 1, 2, 63, 2, 31, 31, 255, 255, 255,
		},
		{ // 2
			0, 2, 2, 63, 2, 31, 31, 255, 255, 255,
		},
		{ // 3
			0, 3, 2, 63, 2, 31, 31, 255, 255, 255,
		},
		{ // 4
			0, 4, 2, 63, 2, 31, 31, 255, 255, 255,
		},
		{ // 5
			0, 5, 2, 63, 2, 31, 31, 255, 255, 255,
		},
		{ // 6
			0, 6, 2, 63, 2, 31, 31, 255, 255, 255,
		},
		{ // 7
			0, 7, 2, 63, 2, 31, 31, 255, 255, 255,
		},
		{ // 8
			0, 8, 2, 63, 2, 31, 31, 255, 255, 255,
		},
		{ // 9
			0, 9, 2, 63, 2, 31, 31, 255, 255, 255,
		},
		{ // 10
			0, 10, 2, 63, 2, 31, 31, 255, 255, 255,
		},
		{ // 11
			0, 11, 2, 63, 2, 31, 31, 255, 255, 255,
		},
	}
};

VQ_TNR_Parameters_t ROM TNRParams_Medium =
{
	/*
	{
		0, // sizeof(VQ_TNR_Parameters_t),
		0, // STLAYER_VIDEO_TNR,
		0
	},
   */
	0,		// EnableGlobalNoiseAdaptive
	0,		// EnableGlobalMotionAdaptive
	3,		// GlobalMotionStdTh
	4800,		// GlobalMotionHighTh
	17,		// GlobalMotionPanTh
	-8,		// GlobalMotionStd
	-2,		// GlobalMotionHigh
	-3,		// GlobalMotionPan
	// EnableSNR, SNRLumaSensitivity, SNRLumaStrength, SNRChromaSensitivity, SNRChromaStrength,
	// YKMin, UVKMin, YGrad, FleshtoneGrad, ChromaGrad
	{
		{ // 0
			1, 63, 0, 63, 0, 3, 3, 36, 54, 36,
		},
		{ // 1
			0, 1, 2, 63, 2, 31, 31, 255, 255, 255,
		},
		{ // 2
			0, 2, 2, 63, 2, 31, 31, 255, 255, 255,
		},
		{ // 3
			0, 3, 2, 63, 2, 31, 31, 255, 255, 255,
		},
		{ // 4
			0, 4, 2, 63, 2, 31, 31, 255, 255, 255,
		},
		{ // 5
			0, 5, 2, 63, 2, 31, 31, 255, 255, 255,
		},
		{ // 6
			0, 6, 2, 63, 2, 31, 31, 255, 255, 255,
		},
		{ // 7
			0, 7, 2, 63, 2, 31, 31, 255, 255, 255,
		},
		{ // 8
			0, 8, 2, 63, 2, 31, 31, 255, 255, 255,
		},
		{ // 9
			0, 9, 2, 63, 2, 31, 31, 255, 255, 255,
		},
		{ // 10
			0, 10, 2, 63, 2, 31, 31, 255, 255, 255,
		},
		{ // 11
			0, 11, 2, 63, 2, 31, 31, 255, 255, 255,
		},
	}
};

VQ_TNR_Parameters_t ROM TNRParams_Low =
{
	/*
	{
		0, // sizeof(VQ_TNR_Parameters_t),
		0, // STLAYER_VIDEO_TNR,
		0
	},
   */
	0,		// EnableGlobalNoiseAdaptive
	0,		// EnableGlobalMotionAdaptive
	3,		// GlobalMotionStdTh
	4800,		// GlobalMotionHighTh
	17,		// GlobalMotionPanTh
	-8,		// GlobalMotionStd
	-2,		// GlobalMotionHigh
	-3,		// GlobalMotionPan
	// EnableSNR, SNRLumaSensitivity, SNRLumaStrength, SNRChromaSensitivity, SNRChromaStrength,
	// YKMin, UVKMin, YGrad, FleshtoneGrad, ChromaGrad
	{
		{ // 0
			0, 63, 0, 63, 0, 4, 4, 50, 72, 50, 
		},
		{ // 1
			0, 1, 2, 63, 2, 31, 31, 255, 255, 255, 
		},
		{ // 2
			0, 2, 2, 63, 2, 31, 31, 255, 255, 255, 
		},
		{ // 3
			0, 3, 2, 63, 2, 31, 31, 255, 255, 255, 
		},
		{ // 4
			0, 4, 2, 63, 2, 31, 31, 255, 255, 255, 
		},
		{ // 5
			0, 5, 2, 63, 2, 31, 31, 255, 255, 255, 
		},
		{ // 6
			0, 6, 2, 63, 2, 31, 31, 255, 255, 255, 
		},
		{ // 7
			0, 7, 2, 63, 2, 31, 31, 255, 255, 255, 
		},
		{ // 8
			0, 8, 2, 63, 2, 31, 31, 255, 255, 255, 
		},
		{ // 9
			0, 9, 2, 63, 2, 31, 31, 255, 255, 255, 
		},
		{ // 10
			0, 10, 2, 63, 2, 31, 31, 255, 255, 255, 
		},
		{ // 11
			0, 11, 2, 63, 2, 31, 31, 255, 255, 255, 
		},
	}
};
#endif