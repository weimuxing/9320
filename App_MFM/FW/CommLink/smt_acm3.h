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
// MODULE:      smt_acm3.h
//
//******************************************************************
#ifndef SMT_ACM_3D_INCLUDED
#define SMT_ACM_3D_INCLUDED

#ifdef USE_SMT
#if (FEATURE_ACM_3D == ENABLE)

//*************************************************************************
// ACM3 specific commands (for SMTPORT_ACM3)
//*************************************************************************
typedef enum ACM3_SMTCommandEnum
{
   ACM3_WRITEANDRUN = 1,
   ACM3_READALL = 2,
   ACM3_SETWINDOW = 3,
   ACM3_GETMAXREGIONSPERTRANS = 0,
   ACM3_GETMAXREGION = 4,
   ACM3_ENABLE = 5,
   ACM3_DISABLE = 6,
   ACM3_UNUSEDCMD = 0xFF
} ACM3_SMTCommandType;

typedef enum ACM3_SMTErrorEnum
{
   ACM3_OK = 0,
   ACM3_ERROR = 1,
   ACM3_INVALIDPARAM = 2,
} ACM3_SMTErrorType;

#define ACM3_REGION_NUM 8

#define ACM3_CONTEXT_SIZE 38 // 38 bytes for a region

#define ACM3_FULL_CONTEXT_SIZE ACM3_CONTEXT_SIZE+3 // Data bufer size for full context

#define CONTEXT_REGION_OFFS 2
/**************************************************************************
 Definition for SMT transactions handler to determine buffersize enough for this prot handler
**************************************************************************/
#if 1
#define ACM3_FULL_SIZE (304 + 2 + 2 + 1) // ACM3 Data structure is 304 and 2 for SMT command and 2 for data size counter and the other 1 for regiion

#if !defined(SMT_MAX_BUF_SIZE)
#define SMT_MAX_BUF_SIZE ACM3_FULL_SIZE
#elif SMT_MAX_BUF_SIZE < ACM3_FULL_SIZE
#undef SMT_MAX_BUF_SIZE
#define SMT_MAX_BUF_SIZE ACM3_FULL_SIZE
#endif

#else
#if !defined(SMT_MAX_BUF_SIZE)
#define SMT_MAX_BUF_SIZE ACM3_FULL_CONTEXT_SIZE
#elif SMT_MAX_BUF_SIZE < ACM3_FULL_CONTEXT_SIZE
#undef SMT_MAX_BUF_SIZE
#define SMT_MAX_BUF_SIZE ACM3_FULL_CONTEXT_SIZE
#endif
#endif
/**************************************************************************/

//
// Offset in SMT context buffer
//
#define REGION_B 	(1 + CONTEXT_REGION_OFFS) // << Region number Byte > 
#define HUE_CENT_LB 	(2 + CONTEXT_REGION_OFFS) // << Low byte of Hue Center word Zone 1>
#define HUE_CENT_HB 	(3 + CONTEXT_REGION_OFFS) // << High byte of Hue Center word Zone 1>, 
#define HUE_APERT_LB 	(4 + CONTEXT_REGION_OFFS) // << Low Byte of Hue Aperture Zone 1>
#define HUE_APERT_HB 	(5 + CONTEXT_REGION_OFFS) // << High Byte of Hue Aperture Zone 1>,
#define SAT_R1_LB 	(6 + CONTEXT_REGION_OFFS) // << Low Byte of Saturation R1 Zone 1>
#define SAT_R1_HB 	(7 + CONTEXT_REGION_OFFS) // << High Byte of Saturation R1 Zone 1>, 
#define SAT_R2_LB 	(8 + CONTEXT_REGION_OFFS) // << Low Byte of Saturation R2 Zone 1>
#define SAT_R2_HB 	(9 + CONTEXT_REGION_OFFS) // << High Byte of Saturation R2 Zone 1>,
#define LUMI_Y1_LB 	(10 + CONTEXT_REGION_OFFS) // << Low Byte of Luminance Y1 Zone 1>
#define LUMI_Y1_HB 	(11 + CONTEXT_REGION_OFFS) // << High Byte of Luminance Y1 Zone 1>, 
#define LUMI_Y2_LB 	(12 + CONTEXT_REGION_OFFS) // << Low Byte of Luminance Y2 Zone 1>
#define LUMI_Y2_HB 	(13 + CONTEXT_REGION_OFFS) // << High Byte of Luminance Y2 Zone 1>,
#define HUE_FADE_LB (14 + CONTEXT_REGION_OFFS) // << Low Byte of Hue Fade Zone 1>
#define HUE_FADE_HB 	(15 + CONTEXT_REGION_OFFS) // << High Byte of Hue Fade Zone 1>, 
#define SAT_FADE_LB 	(16 + CONTEXT_REGION_OFFS) // << Low Byte of Saturation Fade Zone 1>
#define SAT_FADE_HB 	(17 + CONTEXT_REGION_OFFS) // << High Byte of Saturation Fade Zone 1>, 
#define LUMI_FADE_LB 	(18 + CONTEXT_REGION_OFFS) // << Low Byte of Luminance Fade Zone 1>
#define LUMI_FADE_HB 	(19 + CONTEXT_REGION_OFFS) // << High Byte of Luminance Fade Zone 1>,
#define HUE_OFFS_LB 	(20 + CONTEXT_REGION_OFFS) // < Low Byte of Hue Offset Zone 1>
#define HUE_OFFS_HB 	(21 + CONTEXT_REGION_OFFS) // < High Byte of Hue Offset Zone 1>
#define HUE_GAIN_LB 	(22 + CONTEXT_REGION_OFFS) // < Low Byte of Hue Gain Zone 1>
#define HUE_GAIN_HB 	(23 + CONTEXT_REGION_OFFS) // < High Byte of Hue Gain Zone 1>
#define SAT_OFFS_LB 	(24 + CONTEXT_REGION_OFFS) // < Low Byte of Saturation Offset Zone 1>
#define SAT_OFFS_HB 	(25 + CONTEXT_REGION_OFFS) // < High Byte of Saturation Offset Zone 1>
#define SAT_GAIN_LB 	(26 + CONTEXT_REGION_OFFS)// < Low Byte of Saturation Gain Zone 1>
#define SAT_GAIN_HB 	(27 + CONTEXT_REGION_OFFS) // < High Byte of Saturation Gain Zone 1>
#define LUMI_OFFS_LB 	(28 + CONTEXT_REGION_OFFS) // < Low Byte of Luminance Offset Zone 1>
#define LUMI_OFFS_HB 	(29 + CONTEXT_REGION_OFFS) // < High Byte of Luminance Offset Zone 1>
#define LUMI_GAIN_LB 	(30 + CONTEXT_REGION_OFFS) // < Low Byte of Luminance Gain Zone 1>
#define LUMI_GAIN_HB 	(31 + CONTEXT_REGION_OFFS) // < High Byte of Luminance Gain Zone 1>
#define U_VECT_LB 	(32 + CONTEXT_REGION_OFFS) // < Low Byte of U vector Zone 1>
#define U_VECT_HB 	(33 + CONTEXT_REGION_OFFS) // < High Byte of U vector Zone 1>
#define V_VECT_LB 	(34 + CONTEXT_REGION_OFFS) // < Low Byte of V vector Zone 1>
#define V_VECT_HB 	(35 + CONTEXT_REGION_OFFS) // < High Byte of V vector Zone 1>
#define ALPHA_LB 	(36 + CONTEXT_REGION_OFFS) // < Low Byte of Alpha Zone 1>
#define ALPHA_HB 	(37 + CONTEXT_REGION_OFFS) // < High Byte of Alpha Zone 1>
#define SMT_REGION_DATA_SIZE 	(ALPHA_HB - HUE_CENT_LB + 1)

#define WIN_CTRL 	4 // << Window control Byte > 

//******************************************************************************
//  G L O B A L    F U N C T I O N S
//******************************************************************************
BYTE ACM3_SMT_MsgHandler(MsgPacket_t *message, BYTE comm_use);
#endif
#endif
#endif
