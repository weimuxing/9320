/*
	$Workfile:   init.h  $
	$Revision: 1.11 $
	$Date: 2013/05/16 09:59:38 $
*/

#ifndef __INIT_H__
#define __INIT_H__
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
// MODULE:      init.h
//
// USAGE:       Header file for init.c application initialization.
//
//******************************************************************************

#include <stdlib.h>
#include "Board\panel.h"

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************

#define OSD_TILESET_SIZE (sizeof(ST_SRAM_TILE) * (MAX_NUMBER_OF_TILES_USED))
	
#define INSTANT_AUTO_BUFFER_SIZE	0

#define TEMP_BUFFER_SIZE	max(OSD_MAX_TEXT_BUFFER_SIZE, OSD_BMP_DECOMPR_BUFFER_SIZE)

typedef enum
{
   BUS_MISSION,
   BUS_LPM
}gmt_BusOwn;

#pragma pack(push)
#pragma pack(1)
typedef struct
{
    LLD_TuningData_t descriptor; // descriptor
    BYTE Mode;        // 0-enable all, 2-disable 3:2, 3-disable 2:2, 5-force MADI, 6-force 3:2, 8-force 2:2
    BYTE RTh;
    BYTE STh;
    BYTE VMTh1;
    BYTE VMTh2;
    BYTE VMTh3;
    BYTE EnableHWThNoiseAdaptive;
    BYTE ThEnter32;   // Unsigned 8 bit integer
    BYTE ThLeave32;   // Unsigned 8 bit integer
    BYTE ThEnter22;   // Unsigned 8 bit integer
    BYTE ThLeave22;   // Unsigned 8 bit integer
    BYTE Enable32Persis;
    BYTE AFM_32Persis;
    BYTE Enable22Persis;
    BYTE AFM_22Persis;
    BYTE LowSignalThreshold;      // 0-2.55, step 0.01
    BYTE SceneChangeThreshold;    // 0-255
    BYTE Rej32ShortSeq;
    BYTE Rej32LowMotion;
    BYTE WinLeft;         // 0…100
    BYTE WinRight;        // 0…100
    BYTE WinTop;          // 0…100
    BYTE WinBottom;       // 0…100
} VQ_AFM_Parameters_t;
#pragma pack(pop)

//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************
#if FEATURE_AIP == ENABLE
//
// Default ADC setting
//
extern ROM gmt_ADC_SETTING AdcDefaultSetting[];
#endif

//
//global multipurpose buffer for OSD, the table loader, etc.
//
extern BYTE TempBuffer[TEMP_BUFFER_SIZE];


//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

void ApplicationInit(void);
void AssignPanelData(gmd_MasterPanelList PanelIndex);
void BoardInit(void);
void LPM_CalibrateRingOSC(void);
void LPM_SendCalibratedTrim(void);

#endif
//*********************************  END  **************************************
