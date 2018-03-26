#ifndef __MODEPREFS_H__
#define __MODEPREFS_H__

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
// MODULE:		ModePrefs.h
//
//******************************************************************************

//******************************************************************************
//  G L O B A L    S T R U C T U R E
//******************************************************************************
//
// The pack pragma specifies packing alignment for structure and union members
//
#ifdef __PARADIGM__
#pragma pack(push)
#pragma pack(1)
#endif

//
// Lookup table for saved modes' timing tags.
//
typedef struct
{
   WORD W_Hfreq;				// Input H. frequency in 100Hz unit
   WORD W_Vfreq;				// Input V. frequency in 1/10Hz unit
   WORD W_Vtotal;				// Input V. total in line unit
} ModeDependentLookUpType;

#ifdef __PARADIGM__
#pragma pack(pop)
#endif

//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

void SaveOriginalValue(gmt_PHY_CH B_Channel);
void ModeSetPrefs(gmt_PHY_CH B_Channel);


//*********************************  END  **************************************
#endif	// __MODEPREFS_H__

