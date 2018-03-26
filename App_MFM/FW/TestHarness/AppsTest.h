/*
	$Workfile:   AppsTest.h  $
	$Revision: 1.11 $
	$Date: 2012/07/09 06:33:49 $
*/

#ifndef __APPSTEST_H__
#define __APPSTEST_H__

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
//=============================================================
//
// MODULE	:	AppsTest.h
//
// USAGE	:	This module contains all the user AppsTest commands
//
//******************************************************************************


//#define ISR_MONITOR_R_S_VALUES
//#define RD_WR_EXT_SDRAM
//#define PDEV_DEBUG
//#define EXT_OSD_TESTS
//#define FSB_OSD_VERIFICATION
//#define IROM_VALIDATION_TEST_ENABLE

#include "StdLibInc\gm_UpdateSharpness.h"

extern WORD ROM  MHF_LPF_Y_Zoom[];
extern WORD ROM  MHF_LPF_Y_0[];
extern WORD ROM  MHF_LPF_Y_1[];
extern WORD ROM  MHF_LPF_Y_2[];
extern WORD ROM  MHF_LPF_Y_3[];
extern WORD ROM  MHF_LPF_Y_4[];
extern WORD ROM  MHF_LPF_Y_5[];
extern WORD ROM  MHF_LPF_Y_6[];
extern WORD ROM  MHF_LPF_Y_7[];
extern WORD ROM  MHF_LPF_Y_8[];
extern WORD ROM  MHF_LPF_Y_9[];
extern WORD ROM  MHF_LPF_Y_10[];
extern WORD ROM  MHF_LPF_Y_11[];
extern WORD ROM  MHF_LPF_Y_12[];
extern WORD ROM  MHF_LPF_Y_13[];
extern WORD ROM  MHF_LPF_Y_14[];
extern WORD ROM  MHF_Zero[];
extern WORD ROM  MHF_LPF_Y_RGB_0[];
extern WORD ROM  MHF_HPF_Y_Zoom[];
extern WORD ROM  MHF_HPF_Y_0[];
extern WORD ROM  MHF_HPF_Y_1[];
extern WORD ROM  MHF_HPF_Y_2[];
extern WORD ROM  MHF_HPF_Y_3[];
extern WORD ROM  MHF_HPF_Y_4[];
extern WORD ROM  MHF_HPF_Y_5[];
extern WORD ROM  MHF_HPF_Y_6[];
extern WORD ROM  MHF_HPF_Y_7[];
extern WORD ROM  MHF_HPF_Y_RGB_0[];

extern BYTE ROM  MVF_LPF_Y_Zoom[];
extern BYTE ROM  MVF_LPF_Y_0[];
extern BYTE ROM  MVF_LPF_Y_1[];
extern BYTE ROM  MVF_LPF_Y_2[];
extern BYTE ROM  MVF_LPF_Y_3[];
extern BYTE ROM  MVF_LPF_Y_4[];
extern BYTE ROM  MVF_LPF_Y_5[];
extern BYTE ROM  MVF_LPF_Y_6[];
extern BYTE ROM  MVF_LPF_Y_7[];
extern BYTE ROM  MVF_LPF_Y_8[];
extern BYTE ROM  MVF_LPF_Y_9[];
extern BYTE ROM  MVF_LPF_Y_10[];
extern BYTE ROM  MVF_Zero[];
extern BYTE ROM  MVF_LPF_UV_Zoom[];
extern BYTE ROM  MVF_HPF_Y_Zoom[];
extern BYTE ROM  MVF_HPF_Y_0[];
extern BYTE ROM  MVF_HPF_Y_1[];
extern BYTE ROM  MVF_HPF_Y_2[];
extern BYTE ROM  MVF_HPF_Y_3[];
extern BYTE ROM  MVF_RGB_LPF_Y_Zoom[];
extern BYTE ROM  MVF_RGB_LPF_Y_0[];
extern BYTE ROM  MVF_RGB_LPF_Y_1[];
extern BYTE ROM  MVF_RGB_LPF_Y_2[];
extern BYTE ROM  MVF_RGB_LPF_Y_3[];
extern BYTE ROM  MVF_RGB_LPF_Y_4[];
extern BYTE ROM  MVF_RGB_LPF_Y_5[];
extern BYTE ROM  MVF_RGB_LPF_Y_6[];
extern BYTE ROM  MVF_RGB_LPF_Y_7[];
extern BYTE ROM  MVF_RGB_LPF_Y_8[];
extern BYTE ROM  MVF_RGB_LPF_Y_9[];
extern BYTE ROM  MVF_RGB_LPF_Y_10[];
extern BYTE ROM  MVF_RGB_HPF_Y_Zoom[];
extern BYTE ROM  MVF_RGB_HPF_Y_0[];
extern BYTE ROM  MVF_RGB_HPF_Y_1[];
extern BYTE ROM  MVF_RGB_HPF_Y_2[];
extern BYTE ROM  MVF_RGB_HPF_Y_3[];

extern BYTE ROM  PVF_LPF_Y_Zoom[];
extern BYTE ROM  PVF_LPF_Y_0[];
extern BYTE ROM  PVF_LPF_Y_1[];
extern BYTE ROM  PVF_LPF_Y_2[];
extern BYTE ROM  PVF_LPF_Y_3[];
extern BYTE ROM  PVF_LPF_Y_4[];
extern BYTE ROM  PVF_LPF_Y_5[];
extern BYTE ROM  PVF_LPF_Y_6[];
extern BYTE ROM  PVF_LPF_Y_7[];
extern BYTE ROM  PVF_LPF_Y_8[];
extern BYTE ROM  PVF_LPF_Y_9[];
extern BYTE ROM  PVF_LPF_Y_10[];
extern BYTE ROM  PVF_Zero[];
extern BYTE ROM  PVF_LPF_UV_Zoom[];
extern BYTE ROM  PVF_RGB_LPF_Y_Zoom[];
extern BYTE ROM  PVF_RGB_LPF_Y_0[];
extern BYTE ROM  PVF_RGB_LPF_Y_1[];
extern BYTE ROM  PVF_RGB_LPF_Y_2[];
extern BYTE ROM  PVF_RGB_LPF_Y_3[];
extern BYTE ROM  PVF_RGB_LPF_Y_4[];
extern BYTE ROM  PVF_RGB_LPF_Y_5[];
extern BYTE ROM  PVF_RGB_LPF_Y_6[];
extern BYTE ROM  PVF_RGB_LPF_Y_7[];
extern BYTE ROM  PVF_RGB_LPF_Y_8[];
extern BYTE ROM  PVF_RGB_LPF_Y_9[];
extern BYTE ROM  PVF_RGB_LPF_Y_10[];
extern BYTE ROM  PVF_HPF_Y_Zoom[];
extern BYTE ROM  PVF_HPF_Y_0[];
extern BYTE ROM  PVF_HPF_Y_1[];
extern BYTE ROM  PVF_HPF_Y_2[];
extern BYTE ROM  PVF_HPF_Y_3[];
extern BYTE ROM  PVF_HPF_UV_Zoom[];
extern BYTE ROM  PVF_RGB_HPF_Y_Zoom[];
extern BYTE ROM  PVF_RGB_HPF_Y_0[];
extern BYTE ROM  PVF_RGB_HPF_Y_1[];
extern BYTE ROM  PVF_RGB_HPF_Y_2[];
extern BYTE ROM  PVF_RGB_HPF_Y_3[];

extern WORD ROM  RGB_H_LPF_Y_Zoom[];
extern WORD ROM  RGB_H_LPF_Y_0[];
extern WORD ROM  RGB_H_LPF_Y_1[];
extern WORD ROM  RGB_H_LPF_Y_2[];
extern WORD ROM  RGB_H_LPF_Y_3[];
extern WORD ROM  RGB_H_LPF_Y_4[];
extern WORD ROM  RGB_H_LPF_Y_5[];
extern WORD ROM  RGB_H_LPF_Y_6[];
extern WORD ROM  RGB_H_LPF_Y_7[];
extern WORD ROM  RGB_H_LPF_Y_8[];
extern WORD ROM  RGB_H_LPF_Y_9[];
extern WORD ROM  RGB_H_LPF_Y_10[];

extern WORD ROM  RGB_H_HPF_Y_Zoom[];
extern WORD ROM  RGB_H_HPF_Y_0[];
extern WORD ROM  RGB_H_HPF_Y_1[];
extern WORD ROM  RGB_H_HPF_Y_2[];
extern WORD ROM  RGB_H_HPF_Y_3[];
extern WORD ROM  RGB_H_HPF_Y_4[];

extern WORD ROM  RGB_H_LPF_Y_1_0[];
extern WORD ROM  RGB_H_LPF_Y_1_1[];
extern WORD ROM  RGB_H_LPF_Y_1_2[];
extern WORD ROM  RGB_H_LPF_Y_1_3[];
extern WORD ROM  RGB_H_LPF_Y_1_4[];
extern WORD ROM  RGB_H_LPF_Y_1_5[];
extern WORD ROM  RGB_H_LPF_Y_1_6[];
extern WORD ROM  RGB_H_LPF_Y_1_7[];
extern WORD ROM  RGB_H_LPF_Y_1_8[];
extern WORD ROM  RGB_H_LPF_Y_1_9[];
extern WORD ROM  RGB_H_LPF_Y_2_0[];
extern WORD ROM  RGB_H_LPF_Y_2_1[];
extern WORD ROM  RGB_H_LPF_Y_2_2[];
extern WORD ROM  RGB_H_LPF_Y_2_3[];
extern WORD ROM  RGB_H_LPF_Y_2_4[];
extern WORD ROM  RGB_H_LPF_Y_2_5[];
extern WORD ROM  RGB_H_LPF_Y_2_6[];
extern WORD ROM  RGB_H_LPF_Y_2_7[];
extern WORD ROM  RGB_H_LPF_Y_2_8[];

extern WORD ROM  RGB_H_HPF_Y_1_0[];
extern WORD ROM  RGB_H_HPF_Y_1_1[];
extern WORD ROM  RGB_H_HPF_Y_1_2[];
extern WORD ROM  RGB_H_HPF_Y_1_3[];
extern WORD ROM  RGB_H_HPF_Y_1_4[];
extern WORD ROM  RGB_H_HPF_Y_1_5[];
extern WORD ROM  RGB_H_HPF_Y_1_6[];
extern WORD ROM  RGB_H_HPF_Y_1_7[];
extern WORD ROM  RGB_H_HPF_Y_1_8[];
extern WORD ROM  RGB_H_HPF_Y_1_9[];
extern WORD ROM  RGB_H_HPF_Y_2_0[];
extern WORD ROM  RGB_H_HPF_Y_2_1[];
extern WORD ROM  RGB_H_HPF_Y_2_2[];
extern WORD ROM  RGB_H_HPF_Y_2_3[];
extern WORD ROM  RGB_H_HPF_Y_2_4[];
extern WORD ROM  RGB_H_HPF_Y_2_5[];
extern WORD ROM  RGB_H_HPF_Y_2_6[];
extern WORD ROM  RGB_H_HPF_Y_2_7[];
extern WORD ROM  RGB_H_HPF_Y_2_8[];

extern BYTE ROM  MVF_RGB_LPF_Y_1_0[];
extern BYTE ROM  MVF_RGB_LPF_Y_1_1[];
extern BYTE ROM  MVF_RGB_LPF_Y_1_2[];
extern BYTE ROM  MVF_RGB_LPF_Y_1_3[];
extern BYTE ROM  MVF_RGB_LPF_Y_1_4[];
extern BYTE ROM  MVF_RGB_LPF_Y_1_5[];
extern BYTE ROM  MVF_RGB_LPF_Y_1_6[];
extern BYTE ROM  MVF_RGB_LPF_Y_1_7[];
extern BYTE ROM  MVF_RGB_LPF_Y_1_8[];
extern BYTE ROM  MVF_RGB_LPF_Y_1_9[];
extern BYTE ROM  MVF_RGB_LPF_Y_2_0[];
extern BYTE ROM  MVF_RGB_LPF_Y_2_1[];
extern BYTE ROM  MVF_RGB_LPF_Y_2_2[];
extern BYTE ROM  MVF_RGB_LPF_Y_2_3[];
extern BYTE ROM  MVF_RGB_LPF_Y_2_4[];
extern BYTE ROM  MVF_RGB_LPF_Y_2_5[];

extern BYTE ROM  MVF_RGB_HPF_Y_1_0[];
extern BYTE ROM  MVF_RGB_HPF_Y_1_1[];
extern BYTE ROM  MVF_RGB_HPF_Y_1_2[];
extern BYTE ROM  MVF_RGB_HPF_Y_1_3[];
extern BYTE ROM  MVF_RGB_HPF_Y_1_4[];
extern BYTE ROM  MVF_RGB_HPF_Y_1_5[];
extern BYTE ROM  MVF_RGB_HPF_Y_1_6[];
extern BYTE ROM  MVF_RGB_HPF_Y_1_7[];
extern BYTE ROM  MVF_RGB_HPF_Y_1_8[];
extern BYTE ROM  MVF_RGB_HPF_Y_1_9[];
extern BYTE ROM  MVF_RGB_HPF_Y_2_0[];
extern BYTE ROM  MVF_RGB_HPF_Y_2_1[];
extern BYTE ROM  MVF_RGB_HPF_Y_2_2[];
extern BYTE ROM  MVF_RGB_HPF_Y_2_3[];
extern BYTE ROM  MVF_RGB_HPF_Y_2_4[];
extern BYTE ROM  MVF_RGB_HPF_Y_2_5[];

extern BYTE ROM  PVF_RGB_LPF_Y_1_0[];
extern BYTE ROM  PVF_RGB_LPF_Y_1_1[];
extern BYTE ROM  PVF_RGB_LPF_Y_1_2[];
extern BYTE ROM  PVF_RGB_LPF_Y_1_3[];
extern BYTE ROM  PVF_RGB_LPF_Y_1_4[];
extern BYTE ROM  PVF_RGB_LPF_Y_1_5[];
extern BYTE ROM  PVF_RGB_LPF_Y_1_6[];
extern BYTE ROM  PVF_RGB_LPF_Y_1_7[];
extern BYTE ROM  PVF_RGB_LPF_Y_1_8[];
extern BYTE ROM  PVF_RGB_LPF_Y_1_9[];
extern BYTE ROM  PVF_RGB_LPF_Y_2_0[];
extern BYTE ROM  PVF_RGB_LPF_Y_2_1[];
extern BYTE ROM  PVF_RGB_LPF_Y_2_2[];
extern BYTE ROM  PVF_RGB_LPF_Y_2_3[];
extern BYTE ROM  PVF_RGB_LPF_Y_2_4[];
extern BYTE ROM  PVF_RGB_LPF_Y_2_5[];

extern BYTE ROM  PVF_RGB_HPF_Y_1_0[];
extern BYTE ROM  PVF_RGB_HPF_Y_1_1[];
extern BYTE ROM  PVF_RGB_HPF_Y_1_2[];
extern BYTE ROM  PVF_RGB_HPF_Y_1_3[];
extern BYTE ROM  PVF_RGB_HPF_Y_1_4[];
extern BYTE ROM  PVF_RGB_HPF_Y_1_5[];
extern BYTE ROM  PVF_RGB_HPF_Y_1_6[];
extern BYTE ROM  PVF_RGB_HPF_Y_1_7[];
extern BYTE ROM  PVF_RGB_HPF_Y_1_8[];
extern BYTE ROM  PVF_RGB_HPF_Y_1_9[];
extern BYTE ROM  PVF_RGB_HPF_Y_2_0[];
extern BYTE ROM  PVF_RGB_HPF_Y_2_1[];
extern BYTE ROM  PVF_RGB_HPF_Y_2_2[];
extern BYTE ROM  PVF_RGB_HPF_Y_2_3[];
extern BYTE ROM  PVF_RGB_HPF_Y_2_4[];
extern BYTE ROM  PVF_RGB_HPF_Y_2_5[];

extern WORD ROM  Malibu_RGB_H_LPF_Y_Zoom[];
extern WORD ROM  TwoTap_RGB_H_LPF_Y_Zoom[];
extern WORD ROM  TwoTap_RGB_H_HPF_Y_Zoom[];

void InitializeNvram(void);

#ifdef RD_WR_EXT_SDRAM
extern void ReadExtSdram (DWORD DW_StartAddr, DWORD DW_EndAddr, BYTE B_Check, WORD W_Data);
#endif

#if (FEATURE_ACC3 == ENABLE)
extern void gm_ACCDump(BYTE Mode, BYTE Count);
#endif
void ShowSdramMap(void);
#endif __APPSTEST_H__
//*********************************  END  **************************************

