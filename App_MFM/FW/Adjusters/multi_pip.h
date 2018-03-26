/*
	$Workfile:   multi_pip.h  $
	$Revision: 1.3 $
	$Date: 2011/12/15 03:12:00 $
*/
#ifndef __MULTI_PIP__
#define __MULTI_PIP__

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
// MODULE:      multi_pip.h
//
// USAGE:       This module contains user code
//
//******************************************************************************

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************
/*Mapping to WB User Preferences Interface*/
#define MPIP_CH_NUMBER_USERPREF	UserPrefOsdMPipChannel
#define MPIP_CH_NUMBER_XOFFSET	UserPrefOsdMPipHPos
#define MPIP_CH_NUMBER_YOFFSET	UserPrefOsdMPipVPos
#define MPIP_PIP_INPUT_SOURCE		UserPrefCurrentInputPip
#define MPIP_MAIN_INPUT_SOURCE	UserPrefCurrentInputMain
#define MPIP_START_CH_NUMBER		UserPrefTVChannelMain
#define MPIP_BACKGROUND			0x108080

typedef enum
{
   POP7,
   POP12,
   POP3,
   THREE_BY_THREE,
   NUM_MPIP_CONFIGS
}UserPrefConfigurations;

//1000_1000_1000_1111
#define MPIP_MASK_POP7				0x888F
//1111_1001_1001_1111
#define MPIP_MASK_POP12				0xF99F
//0000_0000_0100_1001
#define MPIP_MASK_POP3				0x0111
//0000_0111_0111_0111
#define MPIP_MASK_THREE_BY_THREE	0x0777

#define TIME_FOR_MPIP_SETUP			20
//******************************************************************************
//  G L O B A L    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

BYTE CanMultiPipBeOn(void);
void TurnOnMultiPipStyle(void);
void MPipConfigIncDec(void);
BOOL IsMultiPipControlLocked(void);
void LockMultiPipControl(void);
void UnlockMultiPipControl(void);

#endif //__MULTI_PIP__

//*********************************  END  **************************************
