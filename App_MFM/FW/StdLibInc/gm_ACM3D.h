/*
	$Workfile:   gm_ACM3D.h  $
	$Revision: 1.13 $
	$Date: 2012/04/11 02:56:18 $
*/
#ifndef __ACM3D_H__
#define __ACM3D_H__
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
// MODULE:      gm_ACM3D.h
//
// USAGE:
//
//******************************************************************************

#ifndef max
#define max(a, b)    ( ((a) > (b)) ? (a) : (b) )
#define min(a, b)    ( ((a) < (b)) ? (a) : (b) )
#endif

//*************************************************************************
//
//	A C M     R E G I O N       D E F I N I T I O N
//
//*************************************************************************

#define ACM_NUM_OF_HW_REGIONS 8

//******************************************************************************
//  T Y P E    D E F I N I T I O N S
//******************************************************************************

// There are 8 HW ACM 3D regions at all in FLI8538 (Cortez Plus)
enum
{
   REGION_MAGENTA = 0, // must start from 0 as used as an array index
   REGION_RED,
   REGION_YELLOW,
   REGION_GREEN,
   REGION_CYAN,
   REGION_BLUE,
   REGION_COLOR6,
   REGION_COLOR7,
   ACM3D_REGION_NUM
};

typedef struct
{
   BYTE ThetaFadeLoRGainEnable;
   BYTE ThetaFadeLoRValue;
   BYTE ThetaFadeLoRCutoff;
} VIP_ACMParameters_t;

typedef struct
{
   WORD W_HueCenter;
   WORD W_HueAperture;
   WORD W_R1;
   WORD W_R2;
   WORD W_Y1;
   WORD W_Y2;
   WORD W_HueFade;
   WORD W_SatFade;
   WORD W_LumiFade;
} VIP_ACMRegionDefinitions_t;

typedef struct
{
   SWORD SW_HueOffs;
   SWORD SW_HueGain;
   SWORD SW_SatOffs;
   WORD W_SatGain;
   SWORD SW_LumiOffs;
   WORD W_LumiGain;
   SWORD SW_U_Vect;
   SWORD SW_V_Vect;
   WORD W_Alpha;
} VIP_ACMRegionCorrectionParameters_t;

typedef struct
{
   LLD_TuningData_t descriptor; // descriptor
   VIP_ACMParameters_t ACMParams;
   BYTE RegionsNum;                //num of active regions
   VIP_ACMRegionDefinitions_t ACMRegionDefs[ACM_NUM_OF_HW_REGIONS];
   VIP_ACMRegionCorrectionParameters_t ACMRegionCorrectionParams[ACM_NUM_OF_HW_REGIONS];
} VQ_ACM_Parameters_t;

//*************************************************************************
//
//	A C M     C O R R E C T I O N     P A R A M E T E R S
//
//*************************************************************************

//******************************************************************************
//  T Y P E    D E F I N I T I O N S
//******************************************************************************

//
// Run-time Calculated Correction parameters definition struct, 5 params for each Region
//
typedef struct gmt_ACM3DCalcCorrParamsStruct
{
   WORD	W_1_RLo;		// 1/5...1/63, 0.10u, ACM_Zx_SATR1_SOFT
   WORD	W_1_RUp;		// 1/5...1/63, 0.10u, ACM_Zx_SATR2_SOFT
   WORD	W_1_YLo;		// 1/5...1/63, 0.10u, ACM_Zx_LUMI1_SOFT
   WORD	W_1_YUp;		// 1/5...1/63, 0.10u, ACM_Zx_LUMI2_SOFT
} gmt_ACM3DCalcCorrParams;

//******************************************************************************
//  G L O B A L    F U N C T I O N S
//******************************************************************************

extern VIP_ACMRegionDefinitions_t far * gPa_ACM3DRegionDefs;
extern VIP_ACMRegionCorrectionParameters_t far * gPa_ACM3DCorrectionParamDefs;

extern VQ_ACM_Parameters_t ROM ACM_Params_Default;
extern VQ_ACM_Parameters_t ROM ACM_Params_Sport;
extern VQ_ACM_Parameters_t ROM ACM_Params_Game;
extern VQ_ACM_Parameters_t ROM ACM_Params_Cinema;
extern VQ_ACM_Parameters_t ROM ACM_Params_Vivid;
extern VQ_ACM_Parameters_t ROM ACM_Params_VWD;

#ifdef DELL_U2410F//120717 Edward add for separate table by input color format
extern VQ_ACM_Parameters_t ROM ACM_Params_XvYcc;
extern VQ_ACM_Parameters_t ROM ACM_Params_XvYcc_SD;
extern VQ_ACM_Parameters_t ROM ACM_Params_XvYcc_HD;
extern VQ_ACM_Parameters_t ROM ACM_Params_RGB_XvYcc_SD;
extern VQ_ACM_Parameters_t ROM ACM_Params_RGB_XvYcc_HD;
extern VQ_ACM_Parameters_t ROM ACM_Params_Multimedia_Graphic;
extern VQ_ACM_Parameters_t ROM ACM_Params_Multimedia_Video;
extern VQ_ACM_Parameters_t ROM ACM_Params_Game_Graphic;
extern VQ_ACM_Parameters_t ROM ACM_Params_Game_Video;
extern VQ_ACM_Parameters_t ROM ACM_Params_Movie_Graphic;
extern VQ_ACM_Parameters_t ROM ACM_Params_Movie_Video;
extern VQ_ACM_Parameters_t ROM ACM_Params_Nature;
extern VQ_ACM_Parameters_t ROM ACM_Params_Game_Video_Comp;
extern VQ_ACM_Parameters_t ROM ACM_Params_Movie_Comp;
extern VQ_ACM_Parameters_t ROM ACM_Params_Nature_Comp;
extern VQ_ACM_Parameters_t ROM ACM_Params_sRGB_YUV;
extern VQ_ACM_Parameters_t ROM ACM_Params_sRGB_YUV_SD;
extern VQ_ACM_Parameters_t ROM ACM_Params_Adobe_YUV;
extern VQ_ACM_Parameters_t ROM ACM_Params_sRGB_YUV_DP;
extern VQ_ACM_Parameters_t ROM ACM_Params_sRGB_YUV_SD_DP;
extern VQ_ACM_Parameters_t ROM ACM_Params_6Axis;

/*extern VIP_ACMRegionDefinitions_t ROM gma_ACM3DRegionDefs_XvYcc[ACM3D_REGION_NUM]; //KevinL 20090206
extern VIP_ACMRegionDefinitions_t ROM gma_ACM3DRegionDefs_XvYcc_SD[ACM3D_REGION_NUM]; //zuguang 20090217
extern VIP_ACMRegionDefinitions_t ROM gma_ACM3DRegionDefs_XvYcc_HD[ACM3D_REGION_NUM]; //zuguang 20090217
extern VIP_ACMRegionDefinitions_t ROM gma_ACM3DRegionDefs_RGB_XvYcc_SD[ACM3D_REGION_NUM]; //KevinL 20090609
extern VIP_ACMRegionDefinitions_t ROM gma_ACM3DRegionDefs_RGB_XvYcc_HD[ACM3D_REGION_NUM]; //KevinL 20090609

extern VIP_ACMRegionDefinitions_t ROM gma_ACM3DRegionDefs_Multimedia[ACM3D_REGION_NUM]; //zuguang 20090220
extern VIP_ACMRegionDefinitions_t ROM gma_ACM3DRegionDefs_Game_Graphic[ACM3D_REGION_NUM]; //zuguang 20090220
extern VIP_ACMRegionDefinitions_t ROM gma_ACM3DRegionDefs_Game_Video[ACM3D_REGION_NUM]; //zuguang 20090220
extern VIP_ACMRegionDefinitions_t ROM gma_ACM3DRegionDefs_Movie[ACM3D_REGION_NUM]; //zuguang 20090220
extern VIP_ACMRegionDefinitions_t ROM gma_ACM3DRegionDefs_Nature[ACM3D_REGION_NUM]; //zuguang 20090220
extern VIP_ACMRegionDefinitions_t ROM gma_ACM3DRegionDefs_Game_Video_Comp[ACM3D_REGION_NUM]; //zuguang 20090220
extern VIP_ACMRegionDefinitions_t ROM gma_ACM3DRegionDefs_Movie_Comp[ACM3D_REGION_NUM]; //zuguang 20090220
extern VIP_ACMRegionDefinitions_t ROM gma_ACM3DRegionDefs_Nature_Comp[ACM3D_REGION_NUM]; //zuguang 20090220
extern VIP_ACMRegionDefinitions_t ROM gma_ACM3DRegionDefs_sRGB_YUV[ACM3D_REGION_NUM]; //zuguang 20090220
extern VIP_ACMRegionDefinitions_t ROM gma_ACM3DRegionDefs_sRGB_YUV_SD[ACM3D_REGION_NUM]; //zuguang 20090319
extern VIP_ACMRegionDefinitions_t ROM gma_ACM3DRegionDefs_Adobe_YUV[ACM3D_REGION_NUM]; //zuguang 20090220
extern VIP_ACMRegionDefinitions_t ROM gma_ACM3DRegionDefs_sRGB_YUV_DP[ACM3D_REGION_NUM]; //KevinL 20090513 for DP HD
extern VIP_ACMRegionDefinitions_t ROM gma_ACM3DRegionDefs_sRGB_YUV_SD_DP[ACM3D_REGION_NUM]; //KevinL 20090513 for DP SD

extern VIP_ACMRegionCorrectionParameters_t ROM gma_ACM3DCorrectionParamDefs_XvYcc[ACM3D_REGION_NUM];
extern VIP_ACMRegionCorrectionParameters_t ROM gma_ACM3DCorrectionParamDefs_XvYcc_SD[ACM3D_REGION_NUM];//zuguang 20090217
extern VIP_ACMRegionCorrectionParameters_t ROM gma_ACM3DCorrectionParamDefs_XvYcc_HD[ACM3D_REGION_NUM];//zuguang 20090217
extern VIP_ACMRegionCorrectionParameters_t ROM gma_ACM3DCorrectionParamDefs_RGB_XvYcc_SD[ACM3D_REGION_NUM];//KevinL 20090609
extern VIP_ACMRegionCorrectionParameters_t ROM gma_ACM3DCorrectionParamDefs_RGB_XvYcc_HD[ACM3D_REGION_NUM];//KevinL 20090609

extern VIP_ACMRegionCorrectionParameters_t ROM gma_ACM3DCorrectionParamDefs_Multimedia[ACM3D_REGION_NUM];//zuguang 20090220
extern VIP_ACMRegionCorrectionParameters_t ROM gma_ACM3DCorrectionParamDefs_Game_Graphic[ACM3D_REGION_NUM];//zuguang 20090220
extern VIP_ACMRegionCorrectionParameters_t ROM gma_ACM3DCorrectionParamDefs_Game_Video[ACM3D_REGION_NUM];//zuguang 20090220
extern VIP_ACMRegionCorrectionParameters_t ROM gma_ACM3DCorrectionParamDefs_Movie[ACM3D_REGION_NUM];//zuguang 20090220
extern VIP_ACMRegionCorrectionParameters_t ROM gma_ACM3DCorrectionParamDefs_Nature[ACM3D_REGION_NUM];//zuguang 20090220
extern VIP_ACMRegionCorrectionParameters_t ROM gma_ACM3DCorrectionParamDefs_Game_Video_Comp[ACM3D_REGION_NUM];//zuguang 20090220
extern VIP_ACMRegionCorrectionParameters_t ROM gma_ACM3DCorrectionParamDefs_Movie_Comp[ACM3D_REGION_NUM];//zuguang 20090220
extern VIP_ACMRegionCorrectionParameters_t ROM gma_ACM3DCorrectionParamDefs_Nature_Comp[ACM3D_REGION_NUM];//zuguang 20090220
extern VIP_ACMRegionCorrectionParameters_t ROM gma_ACM3DCorrectionParamDefs_sRGB_YUV[ACM3D_REGION_NUM];//zuguang 20090220
extern VIP_ACMRegionCorrectionParameters_t ROM gma_ACM3DCorrectionParamDefs_sRGB_YUV_SD[ACM3D_REGION_NUM];//zuguang 20090319
extern VIP_ACMRegionCorrectionParameters_t ROM gma_ACM3DCorrectionParamDefs_Adobe_YUV[ACM3D_REGION_NUM];//zuguang 20090220
extern VIP_ACMRegionCorrectionParameters_t ROM gma_ACM3DCorrectionParamDefs_sRGB_YUV_DP[ACM3D_REGION_NUM];//KevinL 20090513 for DP HD
extern VIP_ACMRegionCorrectionParameters_t ROM gma_ACM3DCorrectionParamDefs_sRGB_YUV_SD_DP[ACM3D_REGION_NUM];//KevinL 20090513 for DP SD

extern VIP_ACMRegionDefinitions_t ROM gma_ACM3DRegionDefs_6Axis[ACM3D_REGION_NUM];
extern VIP_ACMRegionCorrectionParameters_t ROM gma_ACM3DCorrectionParamDefs_6Axis[ACM3D_REGION_NUM];
*/
#endif

void gm_ACM3DInit( void );
void gm_ACM3DReadAll( void );
void gm_ACM3DUpdateCtrl(BYTE B_Enable, BYTE B_WindowCtrl);
void LLD_ACMSetParameters(VIP_ACMParameters_t far * Params_p);
void gm_ACM3DSetRegionDefinitionTablePtr(VIP_ACMRegionDefinitions_t far * pt_stRegionDef);
void gm_ACM3DSetCorrectionParamsTablePtr(VIP_ACMRegionCorrectionParameters_t far * pt_stCorrParams);
/*
void gm_ACM3DUpdateSaturation(BYTE B_Region, WORD W_SaturationAdjuster);
void gm_ACM3DUpdateHue(BYTE B_Region, SWORD SW_HueAdjuster);
void gm_ACM3DUpdateLumaGain(BYTE B_Region, WORD W_LumaAdjuster);
void gm_ACM3DUpdateLumaOffs(BYTE B_Region, BYTE B_LumaAdjuster);
*/
#endif


