/*
	$Workfile:   gm_VBI.h  $
	$Revision: 1.4 $
	$Date: 2011/12/14 10:35:31 $
*/
#ifndef __GM_VBI_H__
#define __GM_VBI_H__
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
// MODULE:      gm_VBIInit.c
//
// USAGE:       This file contains standard library function for the
//				Cortez VBI Buffer Handler Initialization
//
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

//******************************************************************************
//  D E F I N I T I O N S
//******************************************************************************
//
// The following definition defines the FIFO depth of the slicer.
// Each time the slicer write to the VBI buffer, it writes this size.
//
#define SZ_SLICER_FIFO              128   // FIFO depth in bytes

//
// The following definitions define the header bytes from the
// slicer output: 0x00, 0xff, 0xff, status, len, data ...
//
#define VBI_HEADER_0		0x00
#define VBI_HEADER_1		0xFF
#define VBI_HEADER_2		0xFF

//
// VBI Driver stage
//
typedef enum
{
   VBI_DRV_NOT_INIT,
   VBI_DRV_INIT,
   VBI_DRV_CONFIG,
   VBI_DRV_ENABLE,
   VBI_DRV_DISABLE,
} gmt_VBI_DRV_STAT;

//
// structure to maintians the VBI driver status
//
typedef struct
{
   gmt_VBI_DRV_STAT     Stat;          // VBI driver status
   gmt_VBI_SERVICE      Service;       // VBI driver service configuration
} gmt_VBI_DRV;

// Vps Acquistion call back
typedef void (far * FPT_VpsAcqCallBack)(void);

//
// This variable controls which slicer is to used.
//
typedef enum
{
   gmd_VBI_SLICER_0,    // this slicer exists on all Cortez chip
} gmt_VBI_SLICER_ID;

//
// The definition control which type of slicer to be used
// The New slicer is only available on FLI8668 for evaluation
//
typedef enum
{
   gmd_VBI_SLICER_TYPE_OLD,   	// Cortez original slicer
} gmt_VBI_SLICER_TYPE;

//******************************************************************************
//  V A R I A B L E S
//******************************************************************************
#ifdef __GM_VBIINIT_C__
#define EXTERN
#else
#define EXTERN extern
#endif

EXTERN gmt_VBI_DRV VBI_Drv;      // VBI driver status

extern DWORD Gd_TeletextDataSdramStartAddress;

extern FPT_VpsAcqCallBack gFp_VpsAcqCallBack;

//******************************************************************************
//  F U N C T I O N S
//******************************************************************************
void			 gm_VbiInit				(void);
gmt_RET_STAT 	 gm_VBI_ServiceConfig   (gmt_VBI_SERVICE ServiceType);
gmt_RET_STAT 	 gm_VBI_ServiceEnable   (void);
gmt_RET_STAT 	 gm_VBI_ServiceDisable  (void);
gmt_RET_STAT 	 gm_VBI_TTXReset        (void);
gmt_RET_PASSFAIL gm_GetFirstPacket      (BYTE *Bp_Dest, WORD *Wp_Len, WORD maxLen);
gmt_RET_PASSFAIL gm_GetNextPacket       (BYTE *Bp_Dest, WORD *Wp_Len, WORD maxLen);

//IROM
gmt_RET_STAT far gm_StartDrawRow(WORD W_TileId, WORD W_Ypos);
void far gm_FinishDrawRow(void);
gmt_RET_STAT far gm_DrawCharacter(WORD W_Xpos, WORD W_BgColour, WORD W_FgColour,
                                  WORD W_WidthInPixels, WORD W_HeightInLines,
                                  WORD W_Flag, WORD far *Wp_PixelData,
                                  BYTE far *Bp_ColourMap);

void Packet830t1(BYTE *aPacket );
void Packet830t2(BYTE *aPacket );
void ResetPacket830(void);
void gm_CBHeaderPacket( BYTE *aPacket );
BOOL gm_TtxGetHeaderTimerFlag( void );

gmt_VBI_SLICER_ID gm_GetSlicerId(void);
gmt_RET_STAT gm_SetSlicerId(gmt_VBI_SLICER_ID thisSlicer);
gmt_VBI_SLICER_TYPE gm_GetSlicerType(void);
gmt_RET_STAT gm_SetSlicerType(gmt_VBI_SLICER_TYPE thisSlicer);

#endif  /* #ifndef __GM_VBI_H__ */
