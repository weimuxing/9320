#ifndef __GM_UPDATESHARPNESS_H__
#define __GM_UPDATESHARPNESS_H__
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
// MODULE:      gm_UpdateSharpness.h
//
// USAGE:		Defines FLI8668 HW dependable constants for Sharpness filters
//
//******************************************************************************

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************
#if (FEATURE_SHARPNESS == REV6)
extern BOOL gmp_EnhancerEnable;
#endif

#define MAX_SHARPNESS_SLIDER	24

#if (FEATURE_SHARPNESS == REV6)

 #ifdef SINGLE_FIELD_DEINT_FIXED
   #define NUM_VERT_SCALE_RANGES			17
 #else
#define NUM_VERT_SCALE_RANGES			13
 #endif
#define NUM_HOR_SCALE_RANGES			17
#define NUM_RGB_HOR_SCALE_RANGES		21
#define NUM_RGB_VER_SCALE_RANGES		18

// Note: the order of sections is UV – Y – Ypk – Uvpk
// Offset for LUT - MAIN Horizontal Filters
#define UV_MHF_START_ADDR   	0x0000
#define Y_MHF_START_ADDR    	0x0400
#define YPK_MHF_START_ADDR  	0x0800
#define UVPK_MHF_START_ADDR 	0x0C00

// Offset for LUT - PIP Horizontal Filters
#define UV_PHF_START_ADDR   	UV_MHF_START_ADDR
#define Y_PHF_START_ADDR    	Y_MHF_START_ADDR
#define YPK_PHF_START_ADDR  	YPK_MHF_START_ADDR
#define UVPK_PHF_START_ADDR 	UVPK_MHF_START_ADDR

// Offset for LUT - MAIN Vertical Filters
#define UV_MVF_START_ADDR   	0x0000
#define Y_MVF_START_ADDR   	0x0800
#define YPK_MVF_START_ADDR  	0x1000
#define UVPK_MVF_START_ADDR 	0x1800

// Offset for LUT - PIP Vertical Filters
#define UV_PVF_START_ADDR   	0x0000
#define Y_PVF_START_ADDR   	0x0100
#define YPK_PVF_START_ADDR  	0x0200
#define UVPK_PVF_START_ADDR 	0x0300

#define MHF_LUT_SIZE 			0x240	// 576 bytes, MHF, PHF
#define PHF_LUT_SIZE 			MHF_LUT_SIZE
#define MVF_LUT_SIZE 			0x420	// 1056 bytes, MVF
#define PVF_LUT_SIZE 			0x88	// 136 bytes, PVF

typedef struct
{
   WORD ROM * Wp_UV;
   WORD ROM * Wp_Y;
   WORD ROM * Wp_UVpk;
   WORD ROM * Wp_Ypk;
} stWpSharpTbls;

typedef struct
{
   BYTE ROM * Bp_UV;
   BYTE ROM * Bp_Y;
   BYTE ROM * Bp_UVpk;
   BYTE ROM * Bp_Ypk;
} stBpSharpTbls;
#endif

#endif

