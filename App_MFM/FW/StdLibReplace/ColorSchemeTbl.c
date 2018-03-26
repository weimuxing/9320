/*
   $Workfile:   ColorSchemeTbl.c  $
   $Revision: 1.14 $
   $Date: 2012/02/14 07:54:03 $
*/
#define __COLORSCHEMETBL_C__
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
// MODULE:        ColorSchemeTbl.c
//
// USAGE:   This file contains arrays for summarizing the available Adaptive
//            Color Management (ACM) tables and associated data.
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "StdLibInc\gm_Global.h"
#include "StdLibInc\gm_ChipFeature.h"
#include "System\All.h"		//Hyb130503+ for elimite compiler warning;

#if (FEATURE_ACM_3D == ENABLE)
#include "StdLibInc\gm_ACM3D.h"
#endif
#if (DEFAULT_PANEL == LG_WUXGA_LM240WU9_SLA1)||(DEFAULT_PANEL == LPL_WUXGA_LM240WU5)
#include "..\..\Panel\WUxga\LG\LM240WU9_SLA1\LM240WU9_ACMTbl.h"
#elif (DEFAULT_PANEL == LG_WQHD_LM270WQ1)
#include "..\..\Panel\WQHD\LG\LM270WQ1\LM270WQ1_ACMTbl.h"
#elif (DEFAULT_PANEL == LG_WHD_LM290WW1_SSA1)
#include "..\..\Panel\WHD\LG\LM290WW1\LM290WW1_ACMTbl.h"
#elif ((DEFAULT_PANEL == LG_WQXGA_LM300WQ5_SLA1)||(DEFAULT_PANEL == LG_WQXGA_LM300WQ6_SLA1))
#include "..\..\Panel\WQXGA\LG\LM300WQ5_SLA1\LM300WQ5_ACMTbl.h"
#else

//******************************************************************************
//  D E F I N E S
//******************************************************************************

//******************************************************************************
//  E X T E R N S
//******************************************************************************

//***************************************************************************
//  C O D E
//***************************************************************************


#if (FEATURE_ACM_3D == ENABLE)

VQ_ACM_Parameters_t ROM ACM_Params_Default =
{
   {
      sizeof(VQ_ACM_Parameters_t),
      0, // STLAYER_VIDEO_ACM, // Useless in Athena
      0
   },
   {
      1,	// ThFadeLorGainEn
      8,	// ThFadeLorVal
      112,	// ThFadeLorCutOff
   },
   8,    // Number of active regions
   {
      // W_HueCenter, W_HueAperture, B_R1, B_R2, B_Y1, B_Y2, B_HueFade, B_SatFade, B_LumiFade
      {200,   5, 400, 2240, 480, 3440,  20, 400, 400},
      {255,  10, 288, 2240, 320, 3600,  20, 288, 288},
      {315,  10, 560, 2240, 800, 3520,  20, 400, 400},
      {  9,   1, 560, 2288, 800, 3520,  25, 400, 400},
      {123,   4, 400, 2000, 1760, 2960,  30, 320, 400},
      {123,   4, 400, 2400, 320, 1600,  25, 320, 400},
      {164,   1, 400, 2240, 400, 3200,  16, 320, 240},
      {  0,   0,   0,   0,   0,   0,   5,  80,  80},
   },
   {
      // hue_offset, hue_gain, sat_offset, sat_gain, lum_offset, lum_gain, u_vect, v_vect, alpha
      {  8,  -2,   0,  67, -106,  64,   0,   0,  26},
      {-10,   2, -32,  67, -68,  64,   0,   0,  20},
      { -9,  -3,   0,  65, -68,  60,   0,   0,  14},
      {-15,   3, -32,  67, -80,  64,   0,   0,   0},
      { -2,  -2,   0,  67,   0,  64,   0,   0,  11},
      { -5,  -2,   0,  67,   0,  65,   0,   0,  11},
      {  2,   2,   0,  65, -20,  64,   0,   0,  16},
      {  0,   0,   0,  64,   0,  64,   0,   0,   0},
   }
} ;

VQ_ACM_Parameters_t ROM ACM_Params_Vivid =
{
	{
		sizeof(VQ_ACM_Parameters_t), // Size
		0, // STLAYER_VIDEO_ACM, // Signature
		768 // Version
	},
   {    
   	0,	// ThFadeLorGainEn
   	5,	// ThFadeLorVal
   	128,	// ThFadeLorCutOff
   },
	8,	// Number of active regions
	{
		// W_HueCenter, B_HueAperture, B_R1, B_R2, B_Y1, B_Y2, B_HueFade, B_SatFade, B_LumiFade
		{180,   4, 192, 496, 480, 3440,  10,  80, 400},
		{315,  10, 320, 2240, 736, 3520,  20, 256, 256},
		{110,  12, 1152, 2016, 640, 3520,  16, 256, 400},
		{172,   4, 832, 2368, 400, 3840,   8, 288, 240},
		{232,  31, 320, 1792, 384, 3840,  16, 256, 256},
		{120,  20, 384, 912, 1280, 3200,  22, 320, 640},
		{  0,   0,   0,   0,   0,   0,   5,  80,  80},
		{  0,   0,   0,   0,   0,   0,   5,  80,  80},
	},
	{
		// hue_offset, hue_gain, sat_offset, sat_gain, lum_offset, lum_gain, u_vect, v_vect, alpha
		{  4,   4,  32,  67,   0,  64,   0,   0,  16},
		{  0,  -4,  64,  68, -32,  60,   0,   0,  16},
		{ -2,   0,   0,  66,   0,  64,   0,   0,  16},
		{ -4,  -2,  64,  67,   0,  64,   0,   0,  16},
		{  0,  -2,  96,  67,   0,  64,   0,   0,  16},
		{ -1,  -2,   0,  64,  16,  64,   0,   0,   8},
		{  0,   0,   0,  64,   0,  64,   0,   0,   0},
		{  0,   0,   0,  64,   0,  64,   0,   0,   0},
	}
} ;

VQ_ACM_Parameters_t ROM ACM_Params_Cinema =
{
	{
		sizeof(VQ_ACM_Parameters_t),
		0, // STLAYER_VIDEO_ACM,
		768
	},
   {    
   	0,	// ThFadeLorGainEn
   	5,	// ThFadeLorVal
   	128,	// ThFadeLorCutOff
   },
	8,	// Number of active regions
	{
		// W_HueCenter, B_HueAperture, B_R1, B_R2, B_Y1, B_Y2, B_HueFade, B_SatFade, B_LumiFade
      {200,   5, 400, 2240, 480, 3440,  20, 400, 400},
      {255,  10, 288, 2240, 320, 3600,  20, 288, 288},
      {315,  10, 560, 2240, 800, 3520,  20, 400, 400},
      {  9,   1, 560, 2288, 800, 3520,  25, 400, 400},
      {123,   4, 400, 2000, 1760, 2960,  30, 320, 400},
      {123,   4, 400, 2400, 320, 1600,  25, 320, 400},
      {164,   1, 400, 2240, 400, 3200,  16, 320, 240},
      {  0,   0,   0,   0,   0,   0,   5,  80,  80},
	},
{
		// hue_offset, hue_gain, sat_offset, sat_gain, lum_offset, lum_gain, u_vect, v_vect, alpha
		{ 10,  -3,   0,  70, -128,  64,   0,   0,  16},
		{-10,   3, -32,  70, -80,  64,   0,   0,  16},
		{ -9,  -4,   0,  68, -128,  60,   0,   0,  16},
		{-15,   4, -32,  68, -96,  64,   0,   0,  10},
		{ -2,  -2,   0,  68,   0,  64,   0,   0,  10},
		{ -5,  -2,   0,  68,   0,  66,   0,   0,  10},
		{  2,   2,   0,  68, -80,  64,   0,   0,  16},
      {  0,   0,   0,  64,   0,  64,   0,   0,   0},
	}
};

VQ_ACM_Parameters_t ROM ACM_Params_Game =
{
{
		sizeof(VQ_ACM_Parameters_t),
		0, // STLAYER_VIDEO_ACM,
		768
	},
{
   	0,	// ThFadeLorGainEn
   	5,	// ThFadeLorVal
   	128,	// ThFadeLorCutOff
   },
	8,	// Number of active regions
{
		// W_HueCenter, B_HueAperture, B_R1, B_R2, B_Y1, B_Y2, B_HueFade, B_SatFade, B_LumiFade
		{180,   4, 192, 496, 480, 3440,  10,  80, 400},
		{315,  10, 320, 2240, 736, 3520,  20, 256, 256},
		{110,  12, 1152, 2016, 640, 3520,  16, 256, 400},
		{172,   4, 832, 2368, 400, 3840,   8, 288, 240},
		{232,  31, 320, 1792, 384, 3840,  16, 256, 256},
		{120,  20, 384, 912, 1280, 3200,  22, 320, 640},
		{  0,   0,   0,   0,   0,   0,   5,  80,  80},
		{  0,   0,   0,   0,   0,   0,   5,  80,  80},
	},
{
		// hue_offset, hue_gain, sat_offset, sat_gain, lum_offset, lum_gain, u_vect, v_vect, alpha
		{  4,   4,  32,  67,   0,  64,   0,   0,  16},
		{  0,  -4,  64,  68, -32,  60,   0,   0,  16},
		{ -2,   0,   0,  66,   0,  64,   0,   0,  16},
		{ -4,  -2,  64,  67,   0,  64,   0,   0,  16},
		{  0,  -2,  96,  67,   0,  64,   0,   0,  16},
		{ -1,  -2,   0,  64,  16,  64,   0,   0,   8},
   {  0,   0,   0,  64,   0,  64,   0,   0,   0},
   {  0,   0,   0,  64,   0,  64,   0,   0,   0},
	}
};

VQ_ACM_Parameters_t ROM ACM_Params_Sport =
{
{
		sizeof(VQ_ACM_Parameters_t),
		0, // STLAYER_VIDEO_ACM,
		768
	},
{
   	0,	// ThFadeLorGainEn
   	5,	// ThFadeLorVal
   	128,	// ThFadeLorCutOff
   },
	8,	// Number of active regions
	{
		// W_HueCenter, B_HueAperture, B_R1, B_R2, B_Y1, B_Y2, B_HueFade, B_SatFade, B_LumiFade
		{171,  10, 160, 2880, 2080, 2112,   5,  80,  80},
		{289,  10, 160, 2880,   0, 4080,   5, 160,  80},
		{208,  10, 160, 2880, 320, 2720,   5, 160,  80},
		{ 51,  10, 160, 2880, 1600, 1920,   5, 160,  80},
		{108,  10, 160, 2880,   0, 4080,   5, 160,  80},
		{323,  10, 288, 960, 2240, 3840,   5, 160,  80},
		{  0, 180, 160, 2880, 480, 640,   5, 160,  80},
		{  0,   0,   0,   0,   0,   0,   5,  80,  80},
	},
{
		// hue_offset, hue_gain, sat_offset, sat_gain, lum_offset, lum_gain, u_vect, v_vect, alpha
   {0,   0,   0,   64,   0,   64,   0,   0,   0},
   {0,   0,   0,   64,   0,   64,   0,   0,   0},
		{  0,   0, 320,  64,   0,  64,   0,   0,   0},
   {0,   0,   0,   64,   0,   64,   0,   0,   0},
   {0,   0,   0,   64,   0,   64,   0,   0,   0},
		{  0,   0, 240,  70,   0,  64,  96, -64,   0},
   {0,   0,   0,   64,   0,   64,   0,   0,   0},
   {0,   0,   0,   64,   0,   64,   0,   0,   0},
	}
};



//***************************************************************************
//   NATURE   M O D E
//***************************************************************************
VQ_ACM_Parameters_t ROM ACM_Params_Nature =
{
	{
		sizeof(VQ_ACM_Parameters_t),
		0, // STLAYER_VIDEO_ACM,
		768
	},
	{
   		0,	// ThFadeLorGainEn
   		5,	// ThFadeLorVal
   		128,	// ThFadeLorCutOff
   	},
		8,	// Number of active regions
	{
		{305,   7,  640, 1600,   320, 2400,  19,  480,  288},
		{229,   9,  704, 2560,   352, 3520,  16,  240,  320},
		{  4,  11,  608, 2560,   320, 3200,  14,  320,  288},
		{193,   8,  880, 2560,   352, 3520,  22,  288,  320},
		{166,   8,  528, 1600,   352, 3616,  14,  224,  448},
		{107,   6,  704, 2560,   320, 3200,  15,  352,  240},
		{130,   6,  640, 1600,  1536, 3840,  12,   96,  240},
		{ 52,   4,  528, 2560,   480, 3200,  12,  240,  320},
	},
	{
		{  2,   2,    0,  64,   80,  64,   0,    0,   0},
		{-22,   0,    0,  60,  112,  64,   0,    0,   0},
		{ -5,   0,    0,  60,  -32,  64,   0,    0,   0},
		{-11,   0,  -48,  60,    0,  64,   0,    0,   0},
		{ -2,   0,   80,  64,    0,  62,   0,    0,   0},
		{ -6,   0,    0,  57,  -32,  60,   0,   16,   0},
		{ -2,   0,  -80,  62,   64,  65,   0,    0,   0},
		{  0,   0,   0,  58,   0,  64,   0,   0,   0},
	}
};


VQ_ACM_Parameters_t ROM ACM_Params_VWD =
{
{
		sizeof(VQ_ACM_Parameters_t),
		0, // STLAYER_VIDEO_ACM,
		768
	},
{
   	0,	// ThFadeLorGainEn
   	5,	// ThFadeLorVal
   	128,	// ThFadeLorCutOff
   },
	8,	// Number of active regions
{
		// W_HueCenter, B_HueAperture, B_R1, B_R2, B_Y1, B_Y2, B_HueFade, B_SatFade, B_LumiFade
      {123,   4, 608,  880, 960, 2300, 272, 640, 640},
      {315,  10,  35,  1280,   50, 1536,  20,  80, 512},   // Z2: Blue region
      {223,  32, 512,  1280, 1280, 2304, 320, 480, 512},   // Z3: Green region
      {  0,   0,   0,    0,   0,    0,   0,   0,   0},
      {  0,   0,   0,    0,   0,    0,   0,   0,   0},
      {  0,   0,   0,    0,   0,    0,   0,   0,   0},
      {  0,   0,   0,    0,   0,    0,   0,   0,   0},
      {  0,   0,   0,    0,   0,    0,   0,   0,   0},
	},
{
		// hue_offset, hue_gain, sat_offset, sat_gain, lum_offset, lum_gain, u_vect, v_vect, alpha
      {  0,   0,   0,  50,   0,  64,   0,   0,   0},
      {  0,   0,  20,  48,   0,  60,  21,  15,  15},
      {  0,   0,   0, 122,   0,  64,   0,   0,   0},
      {  0,   0,   0,  64,   0,  64,   0,   0,   0},
      {  0,   0,   0,  64,   0,  64,   0,   0,   0},
      {  0,   0,   0,  64,   0,  64,   0,   0,   0},
      {  0,   0,   0,  64,   0,  64,   0,   0,   0},
      {  0,   0,   0,  64,   0,  64,   0,   0,   0},
	}
};

#ifdef DELL_U2410F	
//KevinL 20090206 add XvYcc ACM table
//***************************************************************************
//   XvYcc   M O D E
//***************************************************************************
//programmed as per CPlus_ACC_ACM_Programming_Model.doc V1.4

//   W_HueCenter, B_HueAperture,   B_R1, B_R2, B_Y1, B_Y2, B_HueFade, B_SatFade, B_LumiFade
// h_centre   th_ap   R1   R2   Y1   Y2   th_fade   R_fade   Y_fade   hue_offset   hue_gain   sat_offset   sat_gain   lum_offset   lum_gain   u_vect   v_vect   alpha
VQ_ACM_Parameters_t ROM ACM_Params_XvYcc = 
{
	{
			sizeof(VQ_ACM_Parameters_t), // Size
			0, // STLAYER_VIDEO_ACM, // Signature
			768 // Version
	},
	{	
			0,	// ThFadeLorGainEn
			5,	// ThFadeLorVal
			128,	// ThFadeLorCutOff
	},
			8,	// Number of active regions
	{
		{ 52,  14,  90*16, 180*16,  16*16, 220*16,  14,  35*16,   5*16},
		{109,  14,  75*16, 180*16,  16*16, 180*16,  14,  35*16,   5*16},
		{171,  14,  90*16, 180*16,  16*16, 220*16,  14,  35*16,   5*16},
		{232,  14,  75*16, 180*16,  16*16, 180*16,  14,  35*16,   5*16},
		{289,  14, 100*16, 180*16,  16*16, 220*16,  14,  35*16,   5*16},
		{351,  14,  75*16, 180*16,  16*16, 180*16,  14,  35*16,   5*16},
		{  0,   0,   0,   0,   0,   0,   0,   0,   0},
		{  0,   0,   0,   0,   0,   0,   0,   0,   0},
	},
	{
		{  5,   0,  -8*16,  55,  29*16,  72,   0,   0,   0},
		{  6,   0,  -6*16,  62,  30*16,  70,   0,   0,   0},
		{  1,   0, -15*16,  60,   4*16,  64,   0,   0,   0},
		{-16,   0, -11*16,  56,  15*16,  67,   0,   0,   0},
		{  0,   0, -30*16,  58,  18*16,  64,   0,   0,   0},
		{  8,   0,  10*16,  68, -20*16,  55,   0,   0,   0},
		{  0,   0,   0,  64,   0,  64,   0,   0,   0},
		{  0,   0,   0,  64,   0,  64,   0,   0,   0},
	},			
};


//zuguang 20090217
VQ_ACM_Parameters_t ROM ACM_Params_XvYcc_SD = 
{
	{
		sizeof(VQ_ACM_Parameters_t), // Size
		0, // STLAYER_VIDEO_ACM, // Signature
		768 // Version
	},
	{	
		0,	// ThFadeLorGainEn
		5,	// ThFadeLorVal
		128,	// ThFadeLorCutOff
	},
		8,	// Number of active regions
	{
		{ 52,  14,  90*16, 180*16,  16*16, 220*16,  15,  35*16,   5*16},
		{109,  14,  75*16, 180*16,  16*16, 180*16,  16,  35*16,   5*16},
		{171,  14,  90*16, 180*16,  16*16, 220*16,  18,  35*16,   5*16},
		{232,  14,  88*16, 160*16,  16*16, 180*16,  16,  35*16,   5*16},
		{289,  14, 100*16, 180*16,  16*16, 220*16,  18,  35*16,   5*16},
		{351,  14,  75*16, 180*16,   0, 164*16,  16,  35*16,   5*16},
		{  0,   0,   0,   0,   0,   0,   0,   0,   0},
		{  0,   0,   0,   0,   0,   0,   0,   0,   0},
	},

	{
		{  6,   0,  -3*16,  56,  29*16,  70,   0,   0,   0},
		{  5,   0,  -2*16,  64,  30*16,  68,   0,   0,   0},
		{  1,   0,  -9*16,  58,   5*16,  64,   0,   0,   0},
		{-16,   0,  -8*16,  54,  22*16,  69,   0,   0,   0},
		{  0,   0, -29*16,  56,  24*16,  65,   0,   0,   0},
		{ -4,   0,  -3*16,  59,  24*16,  72,   0,   0,   0},
		{  0,   0,   0,  64,   0,  64,   0,   0,   0},
		{  0,   0,   0,  64,   0,  64,   0,   0,   0},
	}
};


//zuguang 20090217
VQ_ACM_Parameters_t ROM ACM_Params_XvYcc_HD = 
{
	{
		sizeof(VQ_ACM_Parameters_t), // Size
		0, // STLAYER_VIDEO_ACM, // Signature
		768 // Version
	},
	{	
		0,	// ThFadeLorGainEn
		5,	// ThFadeLorVal
		128,	// ThFadeLorCutOff
	},
		8,	// Number of active regions
	{
		{ 52,  14,  90*16, 180*16,  16*16, 220*16,  14,  35*16,   5*16},
		{109,  14,  75*16, 180*16,  16*16, 180*16,  14,  35*16,   5*16},
		{171,  14,  90*16, 180*16,  16*16, 220*16,  14,  35*16,   5*16},
		{232,  14,  75*16, 180*16,  16*16, 180*16,  14,  35*16,   5*16},
		{289,  14, 100*16, 180*16,  16*16, 220*16,  14,  35*16,   5*16},
		{351,  14,  75*16, 180*16,  16*16, 180*16,  14,  35*16,   5*16},
		{  0,   0,   0,   0,   0,   0,   0,   0,   0},
		{  0,   0,   0,   0,   0,   0,   0,   0,   0},
	},
	{
		{  5,   0, -10*16,  58,  30*16,  75,   0,   0,   0},
		{  8,   0,   1*16,  61,  33*16,  72,   0,   0,   0},
		{  1,   0, -12*16,  60,   4*16,  64,   0,   0,   0},
		{-13,   0,  -9*16,  53,  14*16,  68,   0,   0,   0},
		{  0,   0, -30*16,  58,  16*16,  65,   0,   0,   0},
		{-17,   0,   0,  64,  20*16,  67,   0,   0,   0},
		{  0,   0,   0,  64,   0,  64,   0,   0,   0},
		{  0,   0,   0,  64,   0,  64,   0,   0,   0},
	}
};

//KevinL 20090609 RGB xvMode
VQ_ACM_Parameters_t ROM ACM_Params_RGB_XvYcc_SD = 
{
	{
		sizeof(VQ_ACM_Parameters_t), // Size
		0, // STLAYER_VIDEO_ACM, // Signature
		768 // Version
	},
	{	
		0,	// ThFadeLorGainEn
		5,	// ThFadeLorVal
		128,	// ThFadeLorCutOff
	},
		8,	// Number of active regions
	{
		{ 52,  14,  90*16, 180*16,  16*16, 220*16,  14,  35*16,   5*16},
		{109,  14,  75*16, 180*16,  16*16, 180*16,  14,  35*16,   5*16},
		{171,  14,  90*16, 180*16,  16*16, 220*16,  14,  35*16,   5*16},
		{232,  14,  75*16, 180*16,  16*16, 180*16,  14,  35*16,   5*16},
		{289,  14, 100*16, 180*16,  16*16, 220*16,  14,  35*16,   5*16},
		{351,  14,  75*16, 180*16,  16*16, 180*16,  14,  35*16,   5*16},
		{  0,   0,   0,   0,   0,   0,   0,   5,   5},
		{  0,   0,   0,   0,   0,   0,   0,   5,   5},
	},
	{
		{  0,   0, -13*16,  55,   0,  64,  20*16, -10*16,   0},
		{ 10,   0,   1*16,  64,  25*16,  66,   0,   0,   0},
		{  1,   0, -12*16,  60,   4*16,  64,   0,   0,   0},
		{-25,   0, -11*16,  45,  14*16,  68,   0,   0,   0},
		{  0,   0, -17*16,  53,   0,  64,  -6*16,  33*16,   0},
		{  0,   0,   0,  64,  11*16,  67,   0,   0,   0},
		{  0,   0,   0,  64,   0,  64,   0,   0,   0},
		{  0,   0,   0,  64,   0,  64,   0,   0,   0},
	}
};


//KevinL 20090609 RGB xvMode
VQ_ACM_Parameters_t ROM ACM_Params_RGB_XvYcc_HD = 
{
	{
		sizeof(VQ_ACM_Parameters_t), // Size
		0, // STLAYER_VIDEO_ACM, // Signature
		768 // Version
	},
	{	
		0,	// ThFadeLorGainEn
		5,	// ThFadeLorVal
		128,	// ThFadeLorCutOff
	},
		8,	// Number of active regions
	{
		{ 52,  14,  90*16, 180*16,  16*16, 220*16,  14,  35*16,   5*16},
		{109,  14,  75*16, 180*16,  16*16, 180*16,  14,  35*16,   5*16},
		{171,  14,  90*16, 180*16,  16*16, 220*16,  14,  35*16,   5*16},
		{232,  14,  75*16, 180*16,  16*16, 180*16,  14,  35*16,   5*16},
		{289,  14, 100*16, 180*16,  16*16, 220*16,  14,  35*16,   5*16},
		{351,  14,  75*16, 180*16,  16*16, 180*16,  14,  35*16,   5*16},
		{  0,   0,   0,   0,   0,   0,   0,   5,   5},
		{  0,   0,   0,   0,   0,   0,   0,   5,   5},
	},
	{
		{  0,   0, -13*16,  55,   0,  64,  20*16, -10*16,   0},
		{ 10,   0,   1*16,  64,  25*16,  66,   0,   0,   0},
		{  1,   0, -12*16,  60,   4*16,  64,   0,   0,   0},
		{-25,   0, -11*16,  45,  14*16,  68,   0,   0,   0},
		{  0,   0, -17*16,  53,   0,  64,  -6*16,  33*16,   0},
		{  0,   0,   0,  64,  11*16,  67,   0,   0,   0},
		{  0,   0,   0,  64,   0,  64,   0,   0,   0},
		{  0,   0,   0,  64,   0,  64,   0,   0,   0},
	}
};

//zuguang 20090220 >
VQ_ACM_Parameters_t ROM ACM_Params_Multimedia_Graphic = 
{

	//20120917 KimChye

	{
		sizeof(VQ_ACM_Parameters_t),
		0, //STLAYER_VIDEO_ACM,
		768
	},
	{
		0,	// ThFadeLorGainEn
		0,	// ThFadeLorVal
		0,	// ThFadeLorCutOff
	},
	8,	// Number of active regions
	{
		// W_HueCenter, B_HueAperture, B_R1, B_R2, B_Y1, B_Y2, B_HueFade, B_SatFade, B_LumiFade
		{298,  14, 578, 2312, 273, 3083,   6, 357, 288},
		{216,	9, 578, 2360, 289, 3003,  16,	0,	33},
		{  0,  11, 578, 2312, 321, 3099,  14, 211, 288},
		{174,	8, 578, 2601, 273, 3003,   4,	0,	33},
		{147,	8, 786, 2601, 273, 3003,   5,	0,	33},
		{ 55,	6, 289, 2328, 433, 3003,   6,	0,	33},
		{104,	7, 578, 2039, 273, 3822,  12,	0,	33},
		{145,	7, 289, 1332, 273, 1911,  11,  48,	33},
	},
	{
		// hue_offset, hue_gain, sat_offset, sat_gain, lum_offset, lum_gain, u_vect, v_vect, alpha
		{ -6,	0,	 0,  64,   0,  64,	 0,   0,  16},
		{  0,	0,	55,  64,   0,  64,	 0,   0,  16},
		{  0,	0,	22,  56,   0,  58,	 0,   0,  16},
		{  0,	0,	 0,  64,   0,  64,	 0,   0,  16},
		{ -3,	0,	 0,  64,   0,  64,	 0,   0,  16},
		{  0,	0,	22,  62,   0,  58,	 0,   0,  16},
		{ -3,	0,	22,  58,   0,  58,	 0,   0,  16},
		{ -2,	0,	33,  64,   0,  64,	 0,   0,  16},
	}

		
};

VQ_ACM_Parameters_t ROM ACM_Params_Multimedia_Video = 
{
	
	//20120917 KimChye

	{
		sizeof(VQ_ACM_Parameters_t),
		0, //STLAYER_VIDEO_ACM,
		768
	},
	{
		0,	// ThFadeLorGainEn
		0,	// ThFadeLorVal
		0,	// ThFadeLorCutOff
	},
	8,	// Number of active regions
	{
		// W_HueCenter, B_HueAperture, B_R1, B_R2, B_Y1, B_Y2, B_HueFade, B_SatFade, B_LumiFade
		{298,  14, 578, 2312, 273, 3083,   6, 357, 288},
		{216,	9, 578, 2360, 289, 3003,  16,	0,	33},
		{  0,  11, 578, 2312, 321, 3099,  14, 211, 288},
		{174,	8, 578, 2601, 273, 3003,   4,	0,	33},
		{147,	8, 786, 2601, 273, 3003,   5,	0,	33},
		{ 55,	6, 289, 2328, 433, 3003,   6,	0,	33},
		{104,	7, 578, 2039, 273, 3822,  12,	0,	33},
		{145,	7, 289, 1332, 273, 1911,  11,  48,	33},
	},
	{
		// hue_offset, hue_gain, sat_offset, sat_gain, lum_offset, lum_gain, u_vect, v_vect, alpha
		{ -6,	0,	 0,  64,   0,  64,	 0,   0,  16},
		{  0,	0,	55,  64,   0,  64,	 0,   0,  16},
		{  0,	0,	22,  56,   0,  58,	 0,   0,  16},
		{  0,	0,	 0,  64,   0,  64,	 0,   0,  16},
		{ -3,	0,	 0,  64,   0,  64,	 0,   0,  16},
		{  0,	0,	22,  62,   0,  58,	 0,   0,  16},
		{ -3,	0,	22,  58,   0,  58,	 0,   0,  16},
		{ -2,	0,	33,  64,   0,  64,	 0,   0,  16},
	}

		
};



VQ_ACM_Parameters_t ROM ACM_Params_Game_Graphic = 
{

		//20120917 KimChye
		{
			sizeof(VQ_ACM_Parameters_t),
			0, //STLAYER_VIDEO_ACM,
			768
		},
		{
			0,	// ThFadeLorGainEn
			0,	// ThFadeLorVal
			0,	// ThFadeLorCutOff
		},
		8,	// Number of active regions
		{
			// W_HueCenter, B_HueAperture, B_R1, B_R2, B_Y1, B_Y2, B_HueFade, B_SatFade, B_LumiFade
			{296,  14, 578, 2312, 273, 3051,   6, 341, 288},
			{215,	9, 578, 2344, 273, 3003,  16,	0,	33},
			{  0,	6, 578, 2312, 273, 3019,  14, 129, 288},
			{167,	8, 497, 2601, 273, 3003,   4,	0,	33},
			{133,	8, 578, 2601, 273, 3003,   5,	0,	33},
			{ 53,	6, 289, 2312, 401, 3003,   6,	0,	33},
			{108,	7, 465, 2023, 273, 3822,  12,	0,	33},
			{142,	8, 500, 1188, 128, 1911,  13,  32,	40},
		},
		{
			// hue_offset, hue_gain, sat_offset, sat_gain, lum_offset, lum_gain, u_vect, v_vect, alpha
			{-10,	0,	 0,  64,   0,  64,	 0,   0,  16},
			{-11,	0,	55,  64,   0,  64,	 0,   0,  16},
			{ 10,	0,	16,  62,   0,  58,	 0,   0,  16},
			{ -3,	0,	 0,  64,   0,  64,	 0,   0,  16},
			{  0,	0,	 0,  64,   0,  64,	 0,   0,  16},
			{  0,	0,	 0,  60,   0,  60,	 0,   0,  16},
			{ -3,	0,	33,  57,   0,  58,	 0,   0,  16},
			{  0,	0,	 0,  64,   0,  64,	 0,   0,  16},
		}
		
	
};

//RGB: Game Video
VQ_ACM_Parameters_t ROM ACM_Params_Game_Video = 
{

	//20120917 KimChye
	{
		sizeof(VQ_ACM_Parameters_t),
		0, //STLAYER_VIDEO_ACM,
		768
	},
	{
		0,	// ThFadeLorGainEn
		0,	// ThFadeLorVal
		0,	// ThFadeLorCutOff
	},
	8,	// Number of active regions
	{
		// W_HueCenter, B_HueAperture, B_R1, B_R2, B_Y1, B_Y2, B_HueFade, B_SatFade, B_LumiFade
		{296,  14, 578, 2312, 273, 3051,   6, 341, 288},
		{215,	9, 578, 2344, 273, 3003,  16,	0,	33},
		{  0,	6, 578, 2312, 273, 3019,  14, 129, 288},
		{167,	8, 497, 2601, 273, 3003,   4,	0,	33},
		{133,	8, 578, 2601, 273, 3003,   5,	0,	33},
		{ 53,	6, 289, 2312, 401, 3003,   6,	0,	33},
		{108,	7, 465, 2023, 273, 3822,  12,	0,	33},
		{142,	8, 500, 1188, 128, 1911,  13,  32,	40},
	},
	{
		// hue_offset, hue_gain, sat_offset, sat_gain, lum_offset, lum_gain, u_vect, v_vect, alpha
		{-10,	0,	 0,  64,   0,  64,	 0,   0,  16},
		{-11,	0,	55,  64,   0,  64,	 0,   0,  16},
		{ 10,	0,	16,  62,   0,  58,	 0,   0,  16},
		{ -3,	0,	 0,  64,   0,  64,	 0,   0,  16},
		{  0,	0,	 0,  64,   0,  64,	 0,   0,  16},
		{  0,	0,	 0,  60,   0,  60,	 0,   0,  16},
		{ -3,	0,	33,  57,   0,  58,	 0,   0,  16},
		{  0,	0,	 0,  64,   0,  64,	 0,   0,  16},
	}
};

//YPbPr: Game Video
VQ_ACM_Parameters_t ROM ACM_Params_Game_Video_Comp = 
{
	{
		sizeof(VQ_ACM_Parameters_t), // Size
		0, // STLAYER_VIDEO_ACM, // Signature
		768 // Version
	},
	{	
		0,	// ThFadeLorGainEn
		5,	// ThFadeLorVal
		128,	// ThFadeLorCutOff
	},
		8,	// Number of active regions
	{
		{319,   7,  44*16, 150*16,  20*16, 200*16,  22,  44*16,  18*16},
		{229,   9,  40*16, 160*16,  22*16, 200*16,  16,  15*16,  20*16},
		{354,  11,  22*16, 150*16,  20*16, 200*16,  14,  20*16,  18*16},
		{193,   8,  44*16, 171*16,  22*16, 200*16,  22,  18*16,  20*16},
		{166,   8,  33*16, 130*16,  22*16, 200*16,  14,  14*16,  28*16},
		{107,   5,  44*16, 160*16,  20*16, 200*16,  22,  22*16,  15*16},
		{121,   6,  22*16, 100*16,  22*16, 240*16,  12,  11*16,  15*16},
		{131,   4,  33*16, 160*16,  30*16, 120*16,  11,  22*16,  20*16},
	},
	{
		{  6,   2,  11*16,  64,   3*16,  60,   0,   0,  16},
		{-22,   0,   0,  64,   2*16,  77,   0,   0,   0},
		{  0,   0,  11*16,  71,  -2*16,  60,   0,   0,  22},
		{  5,   0,   8*16,  60,   0,  60,   0,   0,   0},
		{  1,   0,  11*16,  60,   0,  67,  -2*16,   2*16,   0},
		{  0,   0,   0,  64,   0,  60,   0,   0,  16},
		{ -2,   0,   2*16,  60,   2*16,  65,   0,   0,   8},
		{  0,   0,   0,  64,  -2*16,  64,   0,   0,  16},
	}
};

VQ_ACM_Parameters_t ROM ACM_Params_Movie_Graphic = 
{
	//20120917 KimChye

	{
		sizeof(VQ_ACM_Parameters_t),
		0, //STLAYER_VIDEO_ACM,
		768
	},
	{
		0,	// ThFadeLorGainEn
		5,	// ThFadeLorVal
		128,	// ThFadeLorCutOff
	},
		8,	// Number of active regions
	{
		// W_HueCenter, B_HueAperture, B_R1, B_R2, B_Y1, B_Y2, B_HueFade, B_SatFade, B_LumiFade
		{304,  14, 626, 2392, 305, 3195,  23, 470, 288},
		{229,	9, 416, 2560, 352, 3200,  16, 240, 320},
		{  3,  11, 594, 2392, 305, 3195,  14, 308, 288},
		{193,	8, 416, 2736, 352, 3200,  22, 288, 320},
		{165,	8, 433, 2713, 337, 3195,  14, 211, 448},
		{107,	6, 528, 2560, 320, 3200,  15, 352, 240},
		{130,	6, 416, 1600, 352, 3840,  12, 352, 240},
		{153,	4, 880, 1600, 480, 1920,  12, 240, 320},
	},
	{
		// hue_offset, hue_gain, sat_offset, sat_gain, lum_offset, lum_gain, u_vect, v_vect, alpha
		{  6,	2,	96,  64,  48,  64,	 0,   0,  16},
		{  0,	0,	 0,  60,  80,  67,	 0,   0,  16},
		{ -5,	0,	80,  58, -32,  59,	 0,   0,  16},
		{-15,	0,	 0,  60, 176,  64,	 0,   0,  16},
		{ -3,	0,	33,  60,   0,  64,	 0,   0,  16},
		{ -3,	0, 176,  58,   0,  58,	 0,   0,  16},
		{ -2,	0,	 0,  62,   0,  64,	 0,   0,  16},
		{  5,	0,	32,  64,   0,  63,	 0,   0,  16},
	}
} ;
		

VQ_ACM_Parameters_t ROM ACM_Params_Movie_Video = 
{
	//20120917 KimChye

	{
		sizeof(VQ_ACM_Parameters_t),
		0, //STLAYER_VIDEO_ACM,
		768
	},
	{
		0,	// ThFadeLorGainEn
		5,	// ThFadeLorVal
		128,	// ThFadeLorCutOff
	},
		8,	// Number of active regions
	{
		// W_HueCenter, B_HueAperture, B_R1, B_R2, B_Y1, B_Y2, B_HueFade, B_SatFade, B_LumiFade
		{304,  14, 626, 2392, 305, 3195,  23, 470, 288},
		{229,	9, 416, 2560, 352, 3200,  16, 240, 320},
		{  3,  11, 594, 2392, 305, 3195,  14, 308, 288},
		{193,	8, 416, 2736, 352, 3200,  22, 288, 320},
		{165,	8, 433, 2713, 337, 3195,  14, 211, 448},
		{107,	6, 528, 2560, 320, 3200,  15, 352, 240},
		{130,	6, 416, 1600, 352, 3840,  12, 352, 240},
		{153,	4, 880, 1600, 480, 1920,  12, 240, 320},
	},
	{
		// hue_offset, hue_gain, sat_offset, sat_gain, lum_offset, lum_gain, u_vect, v_vect, alpha
		{  6,	2,	96,  64,  48,  64,	 0,   0,  16},
		{  0,	0,	 0,  60,  80,  67,	 0,   0,  16},
		{ -5,	0,	80,  58, -32,  59,	 0,   0,  16},
		{-15,	0,	 0,  60, 176,  64,	 0,   0,  16},
		{ -3,	0,	33,  60,   0,  64,	 0,   0,  16},
		{ -3,	0, 176,  58,   0,  58,	 0,   0,  16},
		{ -2,	0,	 0,  62,   0,  64,	 0,   0,  16},
		{  5,	0,	32,  64,   0,  63,	 0,   0,  16},
	}

};


VQ_ACM_Parameters_t ROM ACM_Params_Movie_Comp = 
{
	{
		sizeof(VQ_ACM_Parameters_t), // Size
		0, // STLAYER_VIDEO_ACM, // Signature
		768 // Version
	},
	{	
		0,	// ThFadeLorGainEn
		5,	// ThFadeLorVal
		128,	// ThFadeLorCutOff
	},
		8,	// Number of active regions
	{
		{319,   7,  44*16, 150*16,  20*16, 200*16,  22,  44*16,  18*16},
		{229,   9,  22*16, 160*16,  22*16, 200*16,  16,  15*16,  20*16},
		{354,  11,  22*16, 150*16,  20*16, 200*16,  14,  20*16,  18*16},
		{199,   8,  44*16, 160*16,  22*16, 200*16,  22,  18*16,  20*16},
		{167,   8,  33*16, 160*16,  22*16, 200*16,  14,  14*16,  28*16},
		{107,   5,  44*16, 160*16,  20*16, 200*16,  11,  22*16,  15*16},
		{121,   6,  55*16, 100*16,  22*16, 240*16,  12,  11*16,  15*16},
		{131,   4,  33*16, 160*16,  30*16, 120*16,  11,  22*16,  20*16},
	},
	{
		{  6,   2,  11*16,  64,   3*16,  60,   0,   0,  16},
		{-22,   0,   0,  64,   2*16,  75,   0,   0,   0},
		{  5,   0,  11*16,  71,  -2*16,  60,   0,   0,  22},
		{  5,   0,   8*16,  60,   0,  60,   0,   0,   0},
		{ -2,   0,  11*16,  64,   0,  67,  -2*16,   2*16,  13},
		{  2,   0,   5*16,  64,   0,  70,   0,   0,   6},
		{ -4,   0,   1*16,  60,   0,  64,   0,   0,  16},
		{  0,   0,   8*16,  64,  -2*16,  65,   0,   0,  15},
	}
};


//YPbPr: Nature
VQ_ACM_Parameters_t ROM ACM_Params_Nature_Comp = 
{
	{
		sizeof(VQ_ACM_Parameters_t), // Size
		0, // STLAYER_VIDEO_ACM, // Signature
		768 // Version
	},
	{	
		0,	// ThFadeLorGainEn
		5,	// ThFadeLorVal
		128,	// ThFadeLorCutOff
	},
			8,	// Number of active regions
	{
		{277,   7,  44*16, 170*16,  20*16, 200*16,  22,  22*16,  18*16},
		{237,   9,  22*16, 160*16,  22*16, 200*16,  16,  15*16,  20*16},
		{312,   8,  44*16, 150*16,  20*16, 200*16,  11,  20*16,  18*16},
		{199,   8,  44*16, 171*16,  22*16, 200*16,  22,  18*16,  20*16},
		{167,   8,  44*16, 170*16,  22*16, 200*16,  16,  14*16,  15*16},
		{107,   4,  44*16, 160*16,  20*16, 200*16,  22,  22*16,  15*16},
		{121,   6,  55*16, 160*16,  22*16, 200*16,  12,  11*16,  15*16},
		{131,   4,  66*16, 160*16,  30*16, 120*16,  22,  22*16,  20*16},
	},
	{
		{  6,   0,  -5*16,  64,   3*16,  64,   0,   0,  22},
		{-11,   0,   0,  64,   2*16,  70,   0,   0,  11},
		{  8,   0,   5*16,  71,   0,  64,   0,   0,  11},
		{ -5,   0,   0,  64,   0,  64,   0,   0,  16},
		{  1,   0,   0,  60,   0,  64,   0,   0,  11},
		{ -2,   0,   0,  64,   0,  60,   0,   0,  16},
		{  5,   0,  -8*16,  60,  -2*16,  62,   0,   0,  22},
		{  0,   0,   5*16,  60,  -2*16,  62,   0,   0,  22},
	}
};


//YPbPr: sRGB for DP HD
VQ_ACM_Parameters_t ROM ACM_Params_sRGB_YUV_DP = 
{
	{
		sizeof(VQ_ACM_Parameters_t), // Size
		0, // STLAYER_VIDEO_ACM, // Signature
		768 // Version
	},
	{	
		0,	// ThFadeLorGainEn
		5,	// ThFadeLorVal
		128,	// ThFadeLorCutOff
	},
		8,	// Number of active regions
	{
		{ 52,   3,  77*16, 140*16,  22*16, 225*16,  14,  33*16,   5*16},
		{100,   3,  88*16, 140*16,  22*16, 225*16,  17,  35*16,   5*16},
		{169,   3,  77*16, 140*16,  22*16, 225*16,  14,  33*16,   5*16},
		{232,   4,  80*16, 150*16,  22*16, 225*16,  14,  50*16,   5*16},
		{280,   3,  85*16, 150*16,  22*16, 225*16,   5,  33*16,   5*16},
		{351,   3,  55*16, 140*16,  22*16, 240*16,  14,  33*16,   5*16},
		{  0,   0,   0,   0,   0,   0,   0,   5,   5},
		{  0,   0,   0,   0,   0,   0,   0,   5,   5},
	},
	{
		{-27,   7,   9*16,  65,  15*16,  80,   0,   0,  16},
		{ 42,   0,   8*16,  68,  22*16,  77,   0,   0,  16},
		{ -5,   0, -11*16,  55,   0,  61,   0,   0,  16},
		{-26,   7, -22*16,  62,  11*16,  75,   0,   0,  16},
		{ 40,   7, -11*16,  60,  13*16,  67,   0,   0,  16},
		{-16,   7,  22*16,  80,   0,  88,   0,   0,  16},
		{  0,   0,   0,  64,   0,  64,   0,   0,   0},
		{  0,   0,   0,  64,   0,  64,   0,   0,   0},
	}
};


//YPbPr: sRGB for DP SD
VQ_ACM_Parameters_t ROM ACM_Params_sRGB_YUV_SD_DP = 
{
	{
		sizeof(VQ_ACM_Parameters_t), // Size
		0, // STLAYER_VIDEO_ACM, // Signature
		768 // Version
	},
	{	
		0,	// ThFadeLorGainEn
		5,	// ThFadeLorVal
		128,	// ThFadeLorCutOff
	},
		8,	// Number of active regions
	{
		{ 52,   3,  77*16, 140*16,  22*16, 225*16,  14,  33*16,   5*16},
		{104,   3,  88*16, 140*16,  22*16, 225*16,   9,  35*16,   5*16},
		{172,   3,  77*16, 140*16,  22*16, 225*16,  14,  33*16,   5*16},
		{232,   4,  55*16, 140*16,  22*16, 225*16,  14,  33*16,   5*16},
		{289,   3,  77*16, 140*16,  22*16, 225*16,  14,  33*16,   5*16},
		{351,   3,  55*16, 140*16,  22*16, 225*16,  14,  33*16,   5*16},
		{  0,   0,   0,   0,   0,   0,   0,   5,   5},
		{  0,   0,   0,   0,   0,   0,   0,   5,   5},
	},
	{
		{-29,   7,   3*16,  72,  11*16,  75,   0,   0,  16},
		{ 40,   0,   8*16,  68,  22*16,  77,   0,   0,  16},
		{ -2,   0, -11*16,  55,   0,  61,   0,   0,  16},
		{-32,   7, -24*16,  50,  11*16,  70,   0,   0,  16},
		{ 36,  -7,   0,  60,  11*16,  78,   0,   0,  16},
		{-18,   0,   0,  64,  14*16,  70,   0,   0,  16},
		{  0,   0,   0,  64,   0,  64,   0,   0,   0},
		{  0,   0,   0,  64,   0,  64,   0,   0,   0},
	}
};



//YPbPr: sRGB for non-DP HD
VQ_ACM_Parameters_t ROM ACM_Params_sRGB_YUV = 
{
	{
		sizeof(VQ_ACM_Parameters_t), // Size
		0, // STLAYER_VIDEO_ACM, // Signature
		768 // Version
	},
	{	
		0,	// ThFadeLorGainEn
		5,	// ThFadeLorVal
		128,	// ThFadeLorCutOff
	},
		8,	// Number of active regions
	{
		{ 52,   3,  77*16, 120*16,  22*16, 225*16,  14,  33*16,   5*16},
		{100,   3,  88*16, 135*16,  22*16, 225*16,  17,  35*16,   5*16},
		{172,   3,  77*16, 120*16,  22*16, 225*16,  14,  33*16,   5*16},
		{232,   4,  80*16, 150*16,  22*16, 225*16,  14,  50*16,   5*16},
		{280,   3,  85*16, 150*16,  22*16, 225*16,   5,  33*16,   5*16},
		{351,   3,  55*16, 120*16,  22*16, 225*16,  14,  33*16,   5*16},
		{  0,   0,   0,   0,   0,   0,   0,   5,   5},
		{  0,   0,   0,   0,   0,   0,   0,   5,   5},
	},
	{
		{-20,   7,   3*16,  70,  11*16,  75,   0,   0,   0},
		{ 37,   0,   8*16,  68,  22*16,  77,   0,   0,  16},
		{ -2,   0, -11*16,  55,   0,  61,   0,   0,   0},
		{-26,   7, -22*16,  62,  11*16,  75,   0,   0,   0},
		{ 40,  -7, -13*16,  50,  13*16,  67,   0,   0,   0},
		{-11,   0,   0,  64,  14*16,  70,   0,   0,   0},
		{  0,   0,   0,  64,   0,  64,   0,   0,   0},
		{  0,   0,   0,  64,   0,  64,   0,   0,   0},
	}
};


//YPbPr: sRGB for non-DP SD
VQ_ACM_Parameters_t ROM ACM_Params_sRGB_YUV_SD = 
{
	{
		sizeof(VQ_ACM_Parameters_t), // Size
		0, // STLAYER_VIDEO_ACM, // Signature
		768 // Version
	},
	{	
		0,	// ThFadeLorGainEn
		5,	// ThFadeLorVal
		128,	// ThFadeLorCutOff
	},
		8,	// Number of active regions
	{
		{ 52,   3,  77*16, 120*16,  22*16, 225*16,  14,  33*16,   5*16},
		{107,   3,  88*16, 120*16,  22*16, 225*16,  14,  33*16,   5*16},
		{171,   3,  77*16, 120*16,  22*16, 225*16,  14,  33*16,   5*16},
		{232,   4,  55*16, 120*16,  22*16, 225*16,  14,  33*16,   5*16},
		{289,   3,  77*16, 120*16,  22*16, 225*16,  14,  33*16,   5*16},
		{351,   3,  55*16, 120*16,  22*16, 225*16,  14,  33*16,   5*16},
		{  0,   0,   0,   0,   0,   0,   0,   0,   0},
		{  0,   0,   0,   0,   0,   0,   0,   0,   0},
	},
	{
		{-22,   7,   0,  70,  11*16,  75,   0,   0,   0},
		{ 35,   0,  10*16,  70,   2*16,  66,   0,   0,   0},
		{ -2,   0, -11*16,  55,   0,  64,   0,   0,   0},
		{-43,  -7,  -3*16,  60,   5*16,  68,   0,   0,   0},
		{ 25,  -7, -11*16,  48,  11*16,  76,   0,   0,   0},
		{-11,   0,   0,  64,  14*16,  70,   0,   0,   0},
		{  0,   0,   0,  64,   0,  64,   0,   0,   0},
		{  0,   0,   0,  64,   0,  64,   0,   0,   0},
	}
};

VQ_ACM_Parameters_t ROM ACM_Params_Adobe_YUV = 
{
	{
		sizeof(VQ_ACM_Parameters_t), // Size
		0, // STLAYER_VIDEO_ACM, // Signature
		768 // Version
	},
	{	
		0,	// ThFadeLorGainEn
		5,	// ThFadeLorVal
		128,	// ThFadeLorCutOff
	},
		8,	// Number of active regions
	{
		{ 52,   3,  55*16, 140*16,  22*16, 225*16,  14,  35*16,   5*16},
		{100,   5,  88*16, 140*16,  22*16, 225*16,  17,  35*16,   5*16},
		{171,   3,  55*16, 140*16,  22*16, 225*16,  14,  35*16,   5*16},
		{232,   3,  55*16, 140*16,  22*16, 225*16,  14,  35*16,   5*16},
		{289,   3,  55*16, 140*16,  22*16, 225*16,  14,  35*16,   5*16},
		{351,   3,  55*16, 140*16,  22*16, 225*16,  14,  35*16,   5*16},
		{  0,   0,   0,   0,   0,   0,   0,   5,   5},
		{  0,   0,   0,   0,   0,   0,   0,   5,   5},
	},
	{
		{  1,   0,  -9*16,  52,  11*16,  77,   0,   0,   0},
		{ 37,   0,   8*16,  68,  22*16,  77,   0,   0,   0},
		{ -3,   0,   0,  64,   0,  60,   0,   0,   0},
		{-21,  -7,   0,  72,  -3*16,  58,   0,   0,   0},
		{  2,   0,   0,  64,   0,  58,   0,   0,   0},
		{-11,   0,   0,  64,  14*16,  70,   0,   0,   0},
		{  0,   0,   0,  64,   0,  64,   0,   0,   0},
		{  0,   0,   0,  64,   0,  64,   0,   0,   0},
	}
};


VQ_ACM_Parameters_t ROM ACM_Params_6Axis = 
{
	{
		sizeof(VQ_ACM_Parameters_t), // Size
		0, // STLAYER_VIDEO_ACM, // Signature
		768 // Version
	},
	{	
		0,	// ThFadeLorGainEn
		5,	// ThFadeLorVal
		128,	// ThFadeLorCutOff
	},
		8,	// Number of active regions
	{

   //zone1,  Magenta Axis
   {
         HUE_CENTER_MAGENTA,     //hue center, 50 degree
         APERTURE_6AXIS,      //hue aperture
         SAT_R1_LOW,      //R1
         SAT_R2_HIGH,      //R2
         LUM_L1_LOW,       //L1
         LUM_L2_HIGH,      //L2
         HUE_FADE_6AXIS,   //hue fade
         SAT_R_FADE,      //saturation fade
         LUM_L_FADE,      //luminance fade
      },

   //zone2,  Red Axis
   {
         HUE_CENTER_RED,     //hue center, 110 degree
         APERTURE_6AXIS,      //hue aperture
         SAT_R1_LOW,      //R1
         SAT_R2_HIGH,      //R2
         LUM_L1_LOW,       //L1
         LUM_L2_HIGH,      //L2
         HUE_FADE_6AXIS,   //hue fade
         SAT_R_FADE,      //saturation fade
         LUM_L_FADE,      //luminance fade
     },
     
   //zone3,  Yellow Axis
   {
         HUE_CENTER_YELLOW,     //hue center, 170 degree
         APERTURE_6AXIS,      //hue aperture
         SAT_R1_LOW+4,      //R1
         SAT_R2_HIGH,      //R2
         LUM_L1_LOW,       //L1
         LUM_L2_HIGH,      //L2
         HUE_FADE_6AXIS-6,   //hue fade
         SAT_R_FADE+4,      //saturation fade
         LUM_L_FADE,      //luminance fade
      },

   //zone4,  Green Axis
   {
         HUE_CENTER_GREEN,     //hue center, 230 degree
         APERTURE_6AXIS,      //hue aperture
         SAT_R1_LOW,      //R1
         SAT_R2_HIGH,      //R2
         LUM_L1_LOW,       //L1
         LUM_L2_HIGH,      //L2
         HUE_FADE_6AXIS,   //hue fade
         SAT_R_FADE,      //saturation fade
         LUM_L_FADE,      //luminance fade
      },

   //zone5,  Cyan Axis
   {
         HUE_CENTER_CYAN,     //hue center, 290 degree
         APERTURE_6AXIS,      //hue aperture
         SAT_R1_LOW,      //R1
         SAT_R2_HIGH,      //R2
         LUM_L1_LOW,       //L1
         LUM_L2_HIGH,      //L2
         HUE_FADE_6AXIS-14,   //hue fade
         SAT_R_FADE,      //saturation fade
         LUM_L_FADE,      //luminance fade
      },

   //zone6,  Blue Axis
   {
         HUE_CENTER_BLUE,     //hue center, 350 degree
         APERTURE_6AXIS,      //hue aperture
         SAT_R1_LOW,      //R1
         SAT_R2_HIGH,      //R2
         LUM_L1_LOW,       //L1
         LUM_L2_HIGH,      //L2
         HUE_FADE_6AXIS+3,   //hue fade
         SAT_R_FADE,      //saturation fade
         LUM_L_FADE,      //luminance fade
      },
     
   //zone7,  not used region
   {
      0,     //hue center
      0,      //hue aperture
      0,      //R1
      0,      //R2
      0,       //L1
      0,      //L2
      0,      //hue fade
      0,         //saturation fade
      0,         //luminance fade
   },

   //zone8,  not used region
   {
      0,     //hue center
      0,      //hue aperture
      0,      //R1
      0,      //R2
      0,       //L1
      0,      //L2
      0,      //hue fade
      0,         //saturation fade
      0,         //luminance fade
   }
	},
	{
		{ 0, 0, 0, 64, 0, 64, 0, 0, 0x0},
		{ 0, 0, 0, 64, 0, 64, 0, 0, 0x0},
		{ 0, 0, 0, 64, 0, 64, 0, 0, 0x0},
		{ 0, 0, 0, 64, 0, 64, 0, 0, 0x0},
		{ 0, 0, 0, 64, 0, 64, 0, 0, 0x0},
		{ 0, 0, 0, 64, 0, 64, 0, 0, 0x0},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0}, // not used Region
	}
};
#endif//end if DELL_U2410F

#endif //(FEATURE_ACM_3D == ENABLE)
#endif

