/*
	$Workfile:   gm_ModeSetup.h  $
	$Revision: 1.6 $
	$Date: 2011/12/14 03:44:47 $
*/
#ifndef __GM_MODESETUP_H__
#define __GM_MODESETUP_H__

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
// MODULE:      gm_ModeSetup.h
//
// USAGE:
//
//******************************************************************************

//******************************************************************************
//  D E F I N E S
//******************************************************************************

#define	FILM_MODE_NONE			0x00
#define	FILM_MODE_2_2			0x01
#define	FILM_MODE_3_3			0x02

typedef enum
{
   gmd_ComponentBypassDecoder,
   gmd_ComponentThroughDecoder,
}gmt_ComponentRoute;

//
// Channel Input Attributes
//
typedef enum
{
   gmd_DATA_WIDTH,
   gmd_COLOR_SPACE,
   gmd_RGB_TO_YUV,
   gmd_BLACK_OFFSET,
   gmd_RGB_RANGE
} gmt_CHANNEL_INPUT_ATTR;

//
// Channel Input Data Width Options
//
typedef enum
{
   gmd_422,
   gmd_444,
} gmt_DATA_WIDTH;

//
// Channel Input Color Space Options
//
typedef enum
{
   gmd_RGB,
   gmd_YUVSD,
   gmd_YUVHD,
} gmt_COLOR_SPACE;

//
// Channel Input RGB range.
//
typedef enum
{
   gmd_FULL_RGB_RANGE,
   gmd_HDMI_RGB_RANGE
} gmt_RGB_RANGE;


//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

void gm_SetChannelInputAttr(gmt_PHY_CH B_Channel, gmt_CHANNEL_INPUT_ATTR B_Option, WORD B_Value);
WORD gm_GetChannelInputAttr(gmt_PHY_CH B_Channel, gmt_CHANNEL_INPUT_ATTR B_Option);
void gm_CopyChannelInputAttr(gmt_PHY_CH B_Target, gmt_PHY_CH B_Destination);
void gm_SetupInputMain(void);
void gm_SetupInputPip(void);
void gm_AipSetClamping(gmt_PHY_CH B_Channel);
void gm_AfmSetup(BYTE B_OverscanEnabled);
void gm_SetFilmMode(BYTE B_Method);
void gm_ClockSelection(gmt_PHY_CH B_Channel);
//void gm_SetComponentRouteMode(gmt_ComponentRoute);
//gmt_ComponentRoute gm_GetComponentRouteMode(void);
#endif

//*********************************  END  **************************************


