/*
   $Workfile:   NatureAccTbl.c  $
   $Revision: 1.7 $
   $Date: 2012/10/12 06:15:39 $
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
// MODULE:      NatureAccTbl.c
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
/*
LLD_TuningData_t ROM ACC3TuningData =
{
	sizeof(VQ_ACC_Parameters_t),
	0, // STLAYER_VIDEO_ACC, // Useless in Athena
	0
}

// Algorithm paramters
static gmt_ACC3Params ROM Acc3NatureColorChemeParams =
{
    310,10,2,208,
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {256,256,256,256,256,256,256,256,256,256,256,256,256,256,256,256},
    96,
    {6120, 6120, 6120, 6120 ,6120 ,6120 ,6120 ,6120 ,6120 ,6120 ,6120 ,6120 ,6120 ,6120 ,6120 ,6120},
    {1, 2, 4, 2, 1},
    {{16,29},{30,43},{44,57},{58,70},{71,84},{85,98},{99,111},{112,125},{126,139},{140,153},
    {154,166},{167,180},{181,194},{195,208},{209,221},{222,235}},   //  BYTE    Histogram_Bin_Position[NUMBER_OF_BINS][2];
};

gmt_ACC3Options ROM Acc3NatureOptionsTbl =
{
   8,
   1,0,1,0,0,0,0,1,
   1,//Scene Change source
   3,//Left Slope (YCL_SCALE_MIN)
   5,//Right Slope (YCL_SCALE_MAX)
   40,//Scale Thresh (YCL_SCALE_THRESH)
   12,//YC Link Gain (YCL_SCALE_FACTOR)
   3,//Bin overlap distance = 2^ACC_HIST_OVERLAP_DIST
   20//Scene Change Treshold (ACC_SC_THRESH)
};

VIP_ACCStaticLut_t ROM Acc3NatureStatic =
{
	0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
	0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
	0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
	0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
	0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
	0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
	0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
	0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55
}

VQ_ACC_Parameters_t ROM AccNature =
{
	(LLD_TuningData_t *)&ACC3TuningData,
	(VIP_ACCSWParameters_t *)&Acc3NatureColorChemeParams,
	(VIP_ACCStaticLut_t *)&Acc3NatureStatic,
	(VIP_ACCHWParameters_t *)&Acc3NatureOptionsTbl
};
*/
VQ_ACC_Parameters_t ROM AccNature =
{
   {
      sizeof(VQ_ACC_Parameters_t),
      0, // STLAYER_VIDEO_ACC,
      0
   },
   { // ACCSWParameters_high
      0,	// HistStretchEnable	0
      200,	// Cumulative_Black_Pixels	200
      50, // Cumulative_White_Pixels	50
      28, // Max_Black_Bin	28
      188,	// Min_White_Bin	188
      96, // ACC_Strength 96
      { // WF
         0,	// WF00 0
         0,	// WF01 0
         0,	// WF02 0
         0,	// WF03 0
         0,	// WF04 0
         0,	// WF05 0
         0,	// WF06 0
         0,	// WF07 0
         1,	// WF08 1
         2,	// WF09 2
         3,	// WF10 3
         3,	// WF11 3
         3,	// WF12 3
         3,	// WF13 3
         3,	// WF14 3
         3,	// WF15 3
         3,	// WF16 3
         3,	// WF17 3
         3,	// WF18 3
         3,	// WF19 3
         3,	// WF20 3
         3,	// WF21 3
         2,	// WF22 2
         1,	// WF23 1
         0,	// WF24 0
         0,	// WF25 0
         0,	// WF26 0
         0,	// WF27 0
         0,	// WF28 0
         0,	// WF29 0
         0,	// WF30 0
         0,	// WF31 0
         0,	// WF32 0
         0,	// WF33 0
         0,	// WF34 0
         0,	// WF35 0
         0,	// WF36 0
         0,	// WF37 0
         0,	// WF38 0
         0,	// WF39 0
         0,	// WF40 0
         0,	// WF41 0
         0,	// WF42 0
         0,	// WF43 0
         0,	// WF44 0
         0,	// WF45 0
         0,	// WF46 0
         0,	// WF47 0
         0,	// WF48 0
         0,	// WF49 0
         0,	// WF50 0
         0,	// WF51 0
         0,	// WF52 0
         0,	// WF53 0
         0,	// WF54 0
         0,	// WF55 0
         0,	// WF56 0
         0,	// WF57 0
         0,	// WF58 0
         0,	// WF59 0
         0,	// WF60 0
         0,	// WF61 0
         0,	// WF62 0
         0	// WF63 0
      },	// WF
      { // BinLim
         324,	// BinLim00 324
         324,	// BinLim01 324
         324,	// BinLim02 324
         333,	// BinLim03 333
         342,	// BinLim04 342
         351,	// BinLim05 351
         358,	// BinLim06 358
         364,	// BinLim07 364
         370,	// BinLim08 370
         376,	// BinLim09 376
         381,	// BinLim10 381
         385,	// BinLim11 385
         390,	// BinLim12 390
         394,	// BinLim13 394
         397,	// BinLim14 397
         398,	// BinLim15 398
         399,	// BinLim16 399
         400,	// BinLim17 400
         401,	// BinLim18 401
         402,	// BinLim19 402
         403,	// BinLim20 403
         404,	// BinLim21 404
         400,	// BinLim22 400
         392,	// BinLim23 392
         383,	// BinLim24 383
         375,	// BinLim25 375
         372,	// BinLim26 372
         374,	// BinLim27 374
         376,	// BinLim28 376
         378,	// BinLim29 378
         378,	// BinLim30 378
         376,	// BinLim31 376
         374,	// BinLim32 374
         372,	// BinLim33 372
         370,	// BinLim34 370
         368,	// BinLim35 368
         366,	// BinLim36 366
         364,	// BinLim37 364
         364,	// BinLim38 364
         365,	// BinLim39 365
         366,	// BinLim40 366
         367,	// BinLim41 367
         366,	// BinLim42 366
         362,	// BinLim43 362
         358,	// BinLim44 358
         354,	// BinLim45 354
         350,	// BinLim46 350
         346,	// BinLim47 346
         342,	// BinLim48 342
         338,	// BinLim49 338
         333,	// BinLim50 333
         326,	// BinLim51 326
         320,	// BinLim52 320
         313,	// BinLim53 313
         312,	// BinLim54 312
         315,	// BinLim55 315
         318,	// BinLim56 318
         321,	// BinLim57 321
         321,	// BinLim58 321
         318,	// BinLim59 318
         315,	// BinLim60 315
         312,	// BinLim61 312
         312,	// BinLim62 312
         312 // BinLim63 312
      }	// BinLim
   },	// ACCSWParameters_high
   { // ACCStaticLut_high
      { // ACCStaticLut
			// ACCStaticLut00
			85,
			// ACCStaticLut01
			85,
			// ACCStaticLut02
			85,
			// ACCStaticLut03
			85,
			// ACCStaticLut04
			85,
			// ACCStaticLut05
			85,
			// ACCStaticLut06
			85,
			// ACCStaticLut07
			85,
			// ACCStaticLut08
			85,
			// ACCStaticLut09
			85,
			// ACCStaticLut10
			85,
			// ACCStaticLut11
			85,
			// ACCStaticLut12
			85,
			// ACCStaticLut13
			85,
			// ACCStaticLut14
			85,
			// ACCStaticLut15
			85,
			// ACCStaticLut16
			85,
			// ACCStaticLut17
			85,
			// ACCStaticLut18
			85,
			// ACCStaticLut19
			85,
			// ACCStaticLut20
			85,
			// ACCStaticLut21
			85,
			// ACCStaticLut22
			85,
			// ACCStaticLut23
			85,
			// ACCStaticLut24
			85,
			// ACCStaticLut25
			85,
			// ACCStaticLut26
			85,
			// ACCStaticLut27
			85,
			// ACCStaticLut28
			85,
			// ACCStaticLut29
			85,
			// ACCStaticLut30
			85,
			// ACCStaticLut31
			85,
			// ACCStaticLut32
			85,
			// ACCStaticLut33
			85,
			// ACCStaticLut34
			85,
			// ACCStaticLut35
			85,
			// ACCStaticLut36
			85,
			// ACCStaticLut37
			85,
			// ACCStaticLut38
			85,
			// ACCStaticLut39
			85,
			// ACCStaticLut40
			85,
			// ACCStaticLut41
			85,
			// ACCStaticLut42
			85,
			// ACCStaticLut43
			85,
			// ACCStaticLut44
			85,
			// ACCStaticLut45
			85,
			// ACCStaticLut46
			85,
			// ACCStaticLut47
			85,
			// ACCStaticLut48
			85,
			// ACCStaticLut49
			85,
			// ACCStaticLut50
			85,
			// ACCStaticLut51
			85,
			// ACCStaticLut52
			85,
			// ACCStaticLut53
			85,
			// ACCStaticLut54
			85,
			// ACCStaticLut55
			85,
			// ACCStaticLut56
			85,
			// ACCStaticLut57
			85,
			// ACCStaticLut58
			85,
			// ACCStaticLut59
			85,
			// ACCStaticLut60
			85,
			// ACCStaticLut61
			85,
			// ACCStaticLut62
			85,
			// ACCStaticLut63
			85
      }	// ACCStaticLut
   },	// ACCStaticLut_high
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
      },	// YclNlsatTable
      1,	// YclUseKY 1
      1,	// CompLutLpfEn 1
      1,	// CheckScEnable	1
      1,	// SceneChangeResetEnable	1
      0,	// TfFrameNum	 000 = current frame only (no filtering)
      1,	// YCLinkEnable 1
      1,	// SceneChangeSource	 ACC measured scene change
      5,	// LeftSlope	5
      6,	// RightSlope	6
      189,	// ScaleThresh	189
      9,	// YCLinkGain	9
      200,	// SceneChangeThreshold 200
      0,	// XvYccEnable	0
      { // YLpf
         0,	// YLpf00	0.0000
         0,	// YLpf01	0.0000
         0,	// YLpf02	0.0000
         0,	// YLpf03	0.0000
         0	// YLpf04	0.0000
      },	// YLpf
      { // CLpf
         0,	// CLpf00	0.0000
         0,	// CLpf01	0.0000
         0,	// CLpf02	0.0000
         0,	// CLpf03	0.0000
         0	// CLpf04	0.0000
      },	// CLpf
      3,	// ACCYLpfFlt	 9 taps (fixed)
      3,	// ACCCLpfFlt	 9 taps (fixed)
      1,	// UseLpfY	1
      1,	// StaticLUTEn	1
      1,	// LACCEnable	1
      20, // LACCTextureThresh	20
      4,	// LACCDiffShift	4
      4,	// LACCTextureOffset	4
      255,	// LACCTextureGain	255
      100,	// LACCSmoothThresh 100
      0	// LACCSmoothness	0
   }	// ACCHWParameters_high
};

#endif
//******************************** END *********************************//


