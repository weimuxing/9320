#ifndef __AUTOSCANINPUT_H__
#define __AUTOSCANINPUT_H__

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
// MODULE:      AutoScanInput.h
//
// USAGE:       This module contains function that is used for Automatic
//              Selection of physical Input Port for logical VIDEO Input
//
//******************************************************************************
#ifdef AUTO_SCAN_INPUT
void AutoScanInit(gmt_PHY_CH B_Channel);
BOOL AutoScanCondition(gmt_UserDefConnNames Connection);
void AutoScanInputHandler(void);
#else
#define AutoScanInit(channel)
//please, do not change this define
//it's implemented in this way to avoif compilation warnings
//but to make sure this macro always returns FALSE
#define AutoScanCondition(Connection) ((Connection) != (Connection))
#define AutoScanInputHandler()
#endif

#if (defined (AUTO_SCAN_INPUT)&&defined (AUTO_SCAN_INPUT_NVRAM))
void AutoScanInput_InitDefault( void );
void AutoScanInput_SetDefault(gmt_PHY_CH B_Channel, gmt_UserDefConnNames Connection);
void AutoScanInput_GetNext(gmt_PHY_CH B_Channel, gmt_UserDefConnNames *const pConnection);
#else
#define AutoScanInput_InitDefault()
#define AutoScanInput_SetDefault(channel, connection)
#define AutoScanInput_GetNext(channel, connection)
#endif


#endif __AUTOSCANINPUT_H__
//*********************************  END  **************************************

