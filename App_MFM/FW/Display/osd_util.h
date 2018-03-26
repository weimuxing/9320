/*
	$Workfile:   osd_util.h  $
	$Revision: 1.3 $
	$Date: 2011/12/15 03:12:03 $
*/
#ifndef __OSD_UTIL_H__
#define __OSD_UTIL_H__
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
// MODULE:      osd_util.h
//
// USAGE:
//
//******************************************************************************

//******************************************************************************
//  T Y P E    D E F I N I T I O N S
//******************************************************************************

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************

//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************

//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************

//******************************************************************************
//  E X T E R N A L    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
#ifdef DELL_U2410F
extern gmt_OSD_TILE_CTRL ROM DATA_FlyingMessagesMainTile;
extern gmt_OSD_TILE_CTRL ROM DATA_FlyingMessagesTile;
void InitOSDMenuPosition(void);
void ResetOSDMenuPosition(void);
void AdjustFlyingOsdPosition(void);
void AdjustOsdMenuLocation(void);
#endif

void ShowOsd(void);
void HideOsd(void);
void SetOsdTimeout(void);
void ResetTimer(void);
void AdjustOsdTimeout(void);
void AdjustOsdLanguage(void);
void AdjustOsdRotation(void);
void AdjustOsdHPosition(void);
void AdjustOsdVPosition(void);
void PostEvent(void);

#endif __OSD_UTIL_H__
//*********************************  END  **************************************

