/*
   $Workfile:   LinearAccTbl.c  $
   $Revision: 1.1 $
   $Date: 2012/10/16 10:03:27 $
*/
#define __NATUREACCTBL_C__
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
// MODULE:      LinearAccTbl.c
//
// USAGE:      This file contains table for Automatic Contrast Control (ACC)
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "StdLibInc\gm_Global.h"
#include "StdLibReplace\Acc.h"


//***************************************************************************
//  C O D E
//***************************************************************************
#if (FEATURE_ACC3 == ENABLE)

VQ_ACC_Parameters_t ROM AccLinear =
{
   {
      sizeof(VQ_ACC_Parameters_t),
      0,//STLAYER_VIDEO_ACC,
      0
   },
   { // ACCSWParameters_high      
      0, // HistStretchEnable 0
      200,  // Cumulative_Black_Pixels 200
      50, // Cumulative_White_Pixels   50
      28, // Max_Black_Bin 28
      188,  // Min_White_Bin  188
      96, // ACC_Strength 96
      { // WF
         0, // WF00 0
         0, // WF01 0
         0, // WF02 0
         0, // WF03 0
         0, // WF04 0
         0, // WF05 0
         0, // WF06 0
         0, // WF07 0
         0, // WF08 1
         0, // WF09 2
         0, // WF10 3
         0, // WF11 3
         0, // WF12 3
         0, // WF13 3
         0, // WF14 3
         0, // WF15 3
         0, // WF16 3
         0, // WF17 3
         0, // WF18 3
         0, // WF19 3
         0, // WF20 3
         0, // WF21 3
         0, // WF22 2
         0, // WF23 1
         0, // WF24 0
         0, // WF25 0
         0, // WF26 0
         0, // WF27 0
         0, // WF28 0
         0, // WF29 0
         0, // WF30 0
         0, // WF31 0
         0, // WF32 0
         0, // WF33 0
         0, // WF34 0
         0, // WF35 0
         0, // WF36 0
         0, // WF37 0
         0, // WF38 0
         0, // WF39 0
         0, // WF40 0
         0, // WF41 0
         0, // WF42 0
         0, // WF43 0
         0, // WF44 0
         0, // WF45 0
         0, // WF46 0
         0, // WF47 0
         0, // WF48 0
         0, // WF49 0
         0, // WF50 0
         0, // WF51 0
         0, // WF52 0
         0, // WF53 0
         0, // WF54 0
         0, // WF55 0
         0, // WF56 0
         0, // WF57 0
         0, // WF58 0
         0, // WF59 0
         0, // WF60 0
         0, // WF61 0
         0, // WF62 0
         0  // WF63 0
      }, // WF
      { // BinLim
         255,  // BinLim00 324
         255,  // BinLim01 324
         255,  // BinLim02 324
         255,  // BinLim03 333
         255,  // BinLim04 342
         255,  // BinLim05 351
         255,  // BinLim06 358
         255,  // BinLim07 364
         255,  // BinLim08 370
         255,  // BinLim09 376
         255,  // BinLim10 381
         255,  // BinLim11 385
         255,  // BinLim12 390
         255,  // BinLim13 394
         255,  // BinLim14 397
         255,  // BinLim15 398
         255,  // BinLim16 399
         255,  // BinLim17 400
         255,  // BinLim18 401
         255,  // BinLim19 402
         255,  // BinLim20 403
         255,  // BinLim21 404
         255,  // BinLim22 400
         255,  // BinLim23 392
         255,  // BinLim24 383
         255,  // BinLim25 375
         255,  // BinLim26 372
         255,  // BinLim27 374
         255,  // BinLim28 376
         255,  // BinLim29 378
         255,  // BinLim30 378
         255,  // BinLim31 376
         255,  // BinLim32 374
         255,  // BinLim33 372
         255,  // BinLim34 370
         255,  // BinLim35 368
         255,  // BinLim36 366
         255,  // BinLim37 364
         255,  // BinLim38 364
         255,  // BinLim39 365
         255,  // BinLim40 366
         255,  // BinLim41 367
         255,  // BinLim42 366
         255,  // BinLim43 362
         255,  // BinLim44 358
         255,  // BinLim45 354
         255,  // BinLim46 350
         255,  // BinLim47 346
         255,  // BinLim48 342
         255,  // BinLim49 338
         255,  // BinLim50 333
         255,  // BinLim51 326
         255,  // BinLim52 320
         255,  // BinLim53 313
         255,  // BinLim54 312
         255,  // BinLim55 315
         255,  // BinLim56 318
         255,  // BinLim57 321
         255,  // BinLim58 321
         255,  // BinLim59 318
         255,  // BinLim60 315
         255,  // BinLim61 312
         255,  // BinLim62 312
         255   // BinLim63 312
      }  // BinLim
   }, // ACCSWParameters_high
   { // ACCStaticLut_high
      { // ACCStaticLut
         85,   // ACCStaticLut00 1
         85,   // ACCStaticLut01 1
         85,   // ACCStaticLut02 1
         85,   // ACCStaticLut03 1
         85,   // ACCStaticLut04 1
         85,   // ACCStaticLut05 1
         85,   // ACCStaticLut06 1
         85,   // ACCStaticLut07 1
         85,   // ACCStaticLut08 1
         85,   // ACCStaticLut09 1
         85,   // ACCStaticLut10 1
         85,   // ACCStaticLut11 1
         85,   // ACCStaticLut12 1
         85,   // ACCStaticLut13 1
         85,   // ACCStaticLut14 1
         85,   // ACCStaticLut15 1
         85,   // ACCStaticLut16 1
         85,   // ACCStaticLut17 1
         85,   // ACCStaticLut18 1
         85,   // ACCStaticLut19 1
         85,   // ACCStaticLut20 1
         85,   // ACCStaticLut21 1
         85,   // ACCStaticLut22 1
         85,   // ACCStaticLut23 1
         85,   // ACCStaticLut24 1
         85,   // ACCStaticLut25 1
         85,   // ACCStaticLut26 1
         85,   // ACCStaticLut27 1
         85,   // ACCStaticLut28 1
         85,   // ACCStaticLut29 1
         85,   // ACCStaticLut30 1
         85,   // ACCStaticLut31 1
         85,   // ACCStaticLut32 1
         85,   // ACCStaticLut33 1
         85,   // ACCStaticLut34 1
         85,   // ACCStaticLut35 1
         85,   // ACCStaticLut36 1
         85,   // ACCStaticLut37 1
         85,   // ACCStaticLut38 1
         85,   // ACCStaticLut39 1
         85,   // ACCStaticLut40 1
         85,   // ACCStaticLut41 1
         85,   // ACCStaticLut42 1
         85,   // ACCStaticLut43 1
         85,   // ACCStaticLut44 1
         85,   // ACCStaticLut45 1
         85,   // ACCStaticLut46 1
         85,   // ACCStaticLut47 1
         85,   // ACCStaticLut48 1
         85,   // ACCStaticLut49 1
         85,   // ACCStaticLut50 1
         85,   // ACCStaticLut51 1
         85,   // ACCStaticLut52 1
         85,   // ACCStaticLut53 1
         85,   // ACCStaticLut54 1
         85,   // ACCStaticLut55 1
         85,   // ACCStaticLut56 1
         85,   // ACCStaticLut57 1
         85,   // ACCStaticLut58 1
         85,   // ACCStaticLut59 1
         85,   // ACCStaticLut60 1
         85,   // ACCStaticLut61 1
         85,   // ACCStaticLut62 1
         85    // ACCStaticLut63 1
      }  // ACCStaticLut
   }, // ACCStaticLut_high
   { // ACCHWParameters_high
      0,	// IVP_IP_MEASURE_EN	0
      0,	// IVP_IP_MEAS_SC_CHK_EN	0
      0,	// IVP_IP_MEAS_SC_SOURCE	 External Detector
      0,	// IVP_IP_MEAS_SC_THRESH	0
      8,	// HpfAmpThreshY	8
      20, // HpfAmpThreshUV	20
      1,	// ClampBlack	1
      1,	// LutTempFilterEnable	1
      4,	// YclNlsatScale	4
      0,	// YclNlsatEn	0
      { // YclNlsatTable
         16, // YclNlsatTable00	16
         16, // YclNlsatTable01	16
         16, // YclNlsatTable02	16
         16, // YclNlsatTable03	16
         16, // YclNlsatTable04	16
         16, // YclNlsatTable05	16
         16, // YclNlsatTable06	16
         16	// YclNlsatTable07	16
      }, // YclNlsatTable
      1, // YclUseKY 1
      1, // CompLutLpfEn 1
      0, // CheckScEnable  1
      0, // SceneChangeResetEnable  1
      0, // TfFrameNum   000 = current frame only (no filtering)
      1, // YCLinkEnable 1
      1, // SceneChangeSource  ACC measured scene change
      5, // LeftSlope   5
      6, // RightSlope  6
      189,  // ScaleThresh 189
      4, // YCLinkGain  9
      200,  // SceneChangeThreshold 200
      0, // XvYccEnable 0
      { // YLpf
         0, // YLpf00   0.0000
         0, // YLpf01   0.0000
         0, // YLpf02   0.0000
         0, // YLpf03   0.0000
         0  // YLpf04   0.0000
      }, // YLpf
      { // CLpf
         0, // CLpf00   0.0000
         0, // CLpf01   0.0000
         0, // CLpf02   0.0000
         0, // CLpf03   0.0000
         0  // CLpf04   0.0000
      }, // CLpf
      3, // ACCYLpfFlt   9 taps (fixed)
      3, // ACCCLpfFlt   9 taps (fixed)
      1, // UseLpfY  1
      1, // StaticLUTEn 1
      1, // LACCEnable  1
      20, // LACCTextureThresh   20
      4, // LACCDiffShift  4
      4, // LACCTextureOffset 4
      255,  // LACCTextureGain   255
      100,  // LACCSmoothThresh 100
      0  // LACCSmoothness 0
   }  // ACCHWParameters_high
};

#endif