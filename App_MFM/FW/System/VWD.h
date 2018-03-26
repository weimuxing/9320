#ifndef __VWD_H__
#define __VWD_H__

#include "stdlibinc\PIXCRMain.h"
//******************************************************************
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
//================================================================
//
// MODULE:      VWD.h
//
//******************************************************************

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************
enum
{
#ifdef DELL_U2410F
   VWD_DEMO_OFF = 0,
   VWD_DEMO_FULL   = 1,	
   VWD_DEMO_RIGHT  = 2,
   VWD_DEMO_LEFT   = 3,
   VWD_DEMO_HIGHLIGHT = 4,
   VWD_DEMO_ADVANCE = 5,//20120618 Add by KevinChen
#else
   VWD_DEMO_FULL   = 0,
   VWD_DEMO_RIGHT  = 1,
   VWD_DEMO_LEFT   = 2,
   VWD_DEMO_HIGHLIGHT = 3,
#endif
};

#define VWD_HIGHLIGHT_DEMO_COUNT    300
#define VWD_HIGHLIGHT_DEMO_TARGET   0x6500

//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
#if 1//(FEATURE_VWD == ENABLE)
BOOL IsVWDEnabled(void);
void VWD_Handler(void);
void VWD_Initial(void);
void VWD_Start(void);
void VWD_Running(void);
void VWD_Stop(void);
void VWD_ClearHighlighWindow(void);
BOOL IsVwdDetected(void);
//void CscUpdateColorsVWD();

#endif

void VWD_Switch(void);
void VWD_ApplyHighLightWindow(void);
void AdjustAdvanceVWD(void);//20120618 Add by KevinChen

#endif
