/*
   $Workfile:   gm_InputConfig.h  $
   $Revision: 1.9 $
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
// MODULE :gm_InputConfig.h
//
// USAGE  :This module contains Tables and defines used for Configuration of
//         Front-End (AFE+DFE).
//
//*******************************************************************************

#ifndef __INPUTCONFIG_H__
#define __INPUTCONFIG_H__

#ifndef __SYSTEM_H__
#include "System\System.h"      // System specific - top of system files
#endif

#if (defined EV1_93xx_BOARD)
#define SERIAL_USE SERIAL_USE_UART0
#define DDC2BI_USE I2C_SLAVE1_USE
#include    	"Board\EV1-93xx\NO_TUNER\gm_InputConfig.h"
#include    	"Board\EV1-93xx\NO_TUNER\gm_ExtInputConfig.h"
#elif (defined EV2_93xx_BOARD)
#define SERIAL_USE SERIAL_USE_UART0
#define DDC2BI_USE I2C_SLAVE1_USE
#include    	"Board\EV2-93xx\NO_TUNER\gm_InputConfig.h"
#include    	"Board\EV2-93xx\NO_TUNER\gm_ExtInputConfig.h"
#elif (defined RD1_93xx_BOARD)
#ifdef USE_SECOND_UART
#define SERIAL_USE SERIAL_USE_UART1
#else
#define SERIAL_USE OSD_UART_DEF
#endif
//#define SERIAL_USE 	UserPrefUartSelect
#define DDC2BI_USE I2C_SLAVE1_USE
#include    	"Board\RD1-93xx\NO_TUNER\gm_InputConfig.h"
#include    	"Board\RD1-93xx\NO_TUNER\gm_ExtInputConfig.h"
#elif (defined ST_4K2K_93xx_BOARD)
#define SERIAL_USE SERIAL_USE_UART0
#define DDC2BI_USE I2C_SLAVE1_USE
#include    	"Board\ST4K2K\NO_TUNER\gm_InputConfig.h"
#include    	"Board\ST4K2K\NO_TUNER\gm_ExtInputConfig.h"
#endif

//******************************************************************************
//  G L O B A L    F U N C T I O N
//******************************************************************************
BYTE GetChannelProcType(gmt_PHY_CH B_Channel);
BYTE GetChannelConnName(gmt_PHY_CH B_Channel);
BYTE GetChannelConnType(gmt_PHY_CH B_Channel);
BOOL IsInternalHdmiDviPort(gmt_PHY_CH B_Channel);
BOOL IsInternalHdmiPort(gmt_PHY_CH B_Channel);
BOOL IsInternalDviPort(gmt_PHY_CH B_Channel);


#endif // __GM_INPUTCONFIG_H__
//*********************************  END  **************************************
