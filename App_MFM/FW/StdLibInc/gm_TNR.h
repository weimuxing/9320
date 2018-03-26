/*
   $Workfile:   gm_TNR.h  $
   $Revision: 1.2 $
   $Date: 2012/07/04 07:31:29 $
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
// MODULE:      gm_TNR.h
//
// USAGE:
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#include "System\all.h"
#include "StdLibInc\gm_Global.h"
#include "StdLibInc\gm_ChipFeature.h"
#include "StdLibInc\gm_StdLib.h"

//*************************************************************************
//
//   D E F I N I T I O N S
//
//*************************************************************************

//******************************************************************************
//  T Y P E    D E F I N I T I O N S
//******************************************************************************        
#if (FEATURE_TNR == REV3)
typedef struct
{
    BYTE EnableSNR;                // TNR3_BYPASS_SNR bit
    BYTE SNRLumaSensitivity;        // TNR3_Y_SNR_K_GRAD
    BYTE SNRLumaStrength;            // TNR3_Y_H_COEF_SEL
    BYTE SNRChromaSensitivity;    // TNR3_UV_SNR_K_GRAD
    BYTE SNRChromaStrength;        // TNR3_UV_H_COEF_SEL
    BYTE YKMin;                    // TNR3_Y_K_MIN
    BYTE UVKMin;                    // TNR3_UV_K_MIN
    BYTE YGrad;                    // TNR3_luma_grad
    BYTE FleshtoneGrad;            // TNR3_fleshtone_grad
    BYTE ChromaGrad;                // TNR3_chroma_grad
} VQ_TNR_Adaptive_Parameters_t;

#pragma pack(push)
#pragma pack(1)
typedef struct
{
    BYTE EnableGlobalNoiseAdaptive;   // 0 - TNR_STATIC; 1 - TNR_ADAPTIVE
    BYTE EnableGlobalMotionAdaptive;
    WORD GlobalMotionStdTh;        // motion threshold for standard
    WORD GlobalMotionHighTh;        // motion threshold for high
    BYTE GlobalMotionPanTh;        // motion threshold for fast vertical pan
    SBYTE GlobalMotionStd;            // noise subtractor for standard
    SBYTE GlobalMotionHigh;        // noise subtractor for high
    SBYTE GlobalMotionPan;            // noise subtractor for fast vertical pan
    VQ_TNR_Adaptive_Parameters_t AdaptiveData[12];    // 12 noise ranges
} VQ_TNR_Parameters_t;
#pragma pack(pop)

#endif // #if (FEATURE_TNR == REV3)

//******************************************************************************
//  G L O B A L    F U N C T I O N S
//******************************************************************************
gmt_RET_PASSFAIL gm_TNRSetup( BYTE B_TNRMode );
#if (FEATURE_TNR == REV1)
gmt_RET_PASSFAIL gmp_TNRSetAdaptiveLevel(WORD W_AdaptiveLevel, BYTE B_CVBS_Dec_Noise, BYTE B_SysPort);
#else // (FEATURE_TNR == REV3)
gmt_RET_PASSFAIL LLD_TNRSetAdaptivePar(VQ_TNR_Adaptive_Parameters_t far *Par_p);
gmt_RET_PASSFAIL gmp_TNRSetAdaptiveLevel_TNR3(WORD W_AdaptiveLevel);
void gm_TNR3SetTable(VQ_TNR_Parameters_t far * SetTable);
void gm_TNR3GetTable(VQ_TNR_Parameters_t far * GetTable);
#endif // (FEATURE_TNR == REV1)


