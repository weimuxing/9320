#ifndef __ADJ_ACM3D_H__
#define __ADJ_ACM3D_H__

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
// MODULE:		adj_ACM3D.h
//
// USAGE:		This module contains adjusters for ACM3D functions.
//
//******************************************************************************
#if (FEATURE_ACM_3D == ENABLE)


//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

void ACM3D_Init(void);
void ACM3DSetColorScheme(BYTE B_ColorScheme);


#endif	// (FEATURE_ACM_3D == ENABLE)
#endif	// #ifndef __ADJ_ACM3D_H__
//*********************************  END  **************************************
