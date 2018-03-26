/*
	$Workfile:   osd_factory.h  $
	$Revision: 1.12 $
	$Date: 2013/04/08 02:01:38 $
*/
#ifndef __OSD_FACTORY_H__
#define __OSD_FACTORY_H__
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
// MODULE:      osd_factory.h
//
// USAGE:
//
//******************************************************************************

#include "Board\Panel.h"

//******************************************************************************
//  T Y P E    D E F I N I T I O N S
//******************************************************************************

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************
#define MPEG_NR_THRESHOLD_0_MAX	0x3F
#define MPEG_NR_THRESHOLD_1_MAX	0x3F
#define MPEG_NR_THRESHOLD_2_MAX	0x3F
#define MPEG_NR_THRESHOLD_0_DEF	0x1C
#define MPEG_NR_THRESHOLD_1_DEF	0xF
#define MPEG_NR_THRESHOLD_2_DEF	0x28

//******************************************************************************
//  G L O B A L    S T R U C T U R E
//******************************************************************************

#define NUM_OF_ACC_TABLES		4

#if 1 // (FEATURE_OVERDRIVE == ENABLE) // Need to change WB file ...
typedef enum LCDOverdriveEnum
{
   LCD_OD_OFF=0,
   LCD_OD_MED,
   LCD_OD_LOW,
   LCD_OD_HI,
   NUM_LCD_OD_MODES
} OverdriveModes;
#endif

typedef enum FilmModeEnum
{
   FM_DISABLED,
   FM_VIDEO_32_22,
   FM_VIDEO_32,
   FM_VIDEO_22
} FilmModes;

typedef enum ThreeTwoFilmDisplayEnum
{
   NORMAL_32_DISPLAY_MODE,
   FORCE_33_DISPLAY_MODE,
   FORCE_22_DISPLAY_MODE
} ThreeTwoFilmDisplaySettings;

typedef enum ThreeDNREnum
{
   OFF_3DNR,
   LOW,
   MEDIUM,
   HIGH,
   NUM_3DNR_SETTINGS
} ThreeDNRSettings;

typedef enum GammaTableEnum
{
   GAMMA_TABLE_OFF,
   GAMMA_TABLE_LINEAR ,		//Hyb
   GAMMA_TABLE_22,
   GAMMA_TABLE_24,
   GAMMA_TABLE_SCV,	   
   GAMMA_TABLE_20 , 	
   GAMMA_TABLE_26,			//Hyb S cuve gamma
   GAMMA_TABLE_28
} GammaTable;

typedef enum ColorSchemesACM3DEnum
{
#ifdef DELL_U2410F
	OFF_ACM3D_SCHEME,
	CUSTOM_ACM3D_SCHEME,//GAME_ACM3D_SCHEME,	
	XVYCC_ACM3D_SCHEME, //KevinL 20090206
	MULTIMEDIA_ACM3D_SCHEME, //zuguang 20090220
	MOVIE_ACM3D_SCHEME, //zuguang 20090220
	NATURE_ACM3D_SCHEME, //zuguang 20090220 
	GAME_ACM3D_SCHEME, //zuguang 20090220
	SRGB_YUV_ACM3D_SCHEME, //zuguang 20090220
	ADOBE_YUV_ACM3D_SCHEME, //zuguang 20090220
	#ifdef VWD_ACM_TEST
	VWD_ACM3D_SCHEME,
	#endif
#else
	OFF_ACM3D_SCHEME,
   VIVID_ACM3D_SCHEME,
   CINEMA_ACM3D_SCHEME,
   GAME_ACM3D_SCHEME,
   SPORTS_ACM3D_SCHEME,
   CUSTOM_ACM3D_SCHEME,
#endif	
	NUM_ACM3D_COLOR_SCHEMES
} ColorSchemesACM3D;

#ifndef DELL_U2410F
#define VWD_ACM3D_SCHEME   100
#endif

typedef enum BlueStretchEnum
{
   BLUE_STRETCH_OFF,
   BLUE_STRETCH_LOW,
   BLUE_STRETCH_MED,
   BLUE_STRETCH_HIGH
} BlueStretchSettings;

typedef enum MpegNrEnum
{
   MPEG_NR_OFF,
   MPEG_NR_MOSQ_GAUSS,
   MPEG_NR_MOSQ,
   MPEG_NR_GAUSS
} MpegNrSettings;

typedef enum YUV2RGBExpansionEnum
{
   YUV2RGB_EXPANSION_LINEAR,
   YUV2RGB_EXPANSION_PARTIAL,
   YUV2RGB_EXPANSION_FULL
} YUV2RGBExpansionSettings;

typedef enum MPipTileSetEnum
{
   Column_One,
   Column_Two,
   Column_Three,
   Column_Four
} MPipTileSet;

#ifdef GammaData_To_DDR
#define Gamma_RAM far

typedef struct
{
   BYTE B_PreLUTLoaded;			//
   BYTE GammaPreLUTSteep_R[512];
   BYTE GammaPreLUTSteep_G[512];
   BYTE GammaPreLUTSteep_B[512];   
   BYTE GammaPreLUT_R[1024];
   BYTE GammaPreLUT_G[1024];
   BYTE GammaPreLUT_B[1024];

   BYTE B_PostLUTLoaded;
   BYTE GammaPostLUTSteep_R[512];
   BYTE GammaPostLUTSteep_G[512];
   BYTE GammaPostLUTSteep_B[512];   
   BYTE GammaPostLUT_R[1024];
   BYTE GammaPostLUT_G[1024];
   BYTE GammaPostLUT_B[1024];
}GammaData_Structure;
#endif
//******************************************************************************
//  E X T E R N A L    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

void AdjustAccAcmSplit(void);
void AdjustLcdOverdriveSplit(void);
void AdjustSplitDirection(void);
void AdjustSplitScreen(void);

void FactoryFunction(void);
void SetWBRegAddr(void);
void WBWriteReg(void);
BYTE WBReadReg(BYTE B_Nible);
void FactoryGetADCValues(void);
void FactorySaveADCValues(void);
void FactoryAdjustADCValues(void);
void CC_Process(void);

void ADCCalibration(void);
void AdjustDCDiMain(void);
void AdjustDCDiPip(void);
void AdjustFilmMode(void);
BOOL IsOverdriveAvailable(void);
void OverDrive_Setup(void);
void AdjustLCDOverdrive(void);
void AdjustLCDOverdriveNoiseCoring(void);
#ifdef SAVING_DATA_INTO_NVRAM
BYTE Get_NVRAMTableIndex(void);//120815 Edward for Gamma table check move to this function
BOOL NVRAM_TableCheck(BYTE 	Table_Type ,BYTE Table_Index);//120815 Edward for Gamma table check move to this function
void AdjustGammaCorrection_NVRAM(void);
#endif
void AdjustGammaCorrection(void);
void AdjustGammaMode(void);
#ifdef DELL_U2410F
void AdjustGammaCorrection_ColorSpace(void);
gmt_GammaDescr ROM * ROM * GetGammaTable_ColorSpce(void);
#endif
void AdjustGammaCorrection_Ext(void); //Added by ChengLin 20130226
void AdjustImpVlut(void);
gmt_GammaDescr ROM * ROM * GetGammaTable(void);
void AdjustFactoryCustom(void);
//BYTE GetMaxAcmTable (void);
//Adjust ACM Color Saturation
BOOL IsAcmUsed (void);
void AdjustAcmRedSat(void);
void AdjustAcmGreenSat(void);
void AdjustAcmBlueSat(void);
void AdjustAcmYellowSat(void);
void AdjustAcmMagentaSat(void);
void AdjustAcmCyanSat(void);
void AdjustAcmBlackSat(void);
void AdjustAcmWhiteSat(void);

//Adjust ACM Color Hue
void AdjustAcmRedHue(void);
void AdjustAcmGreenHue(void);
void AdjustAcmBlueHue(void);
void AdjustAcmYellowHue(void);
void AdjustAcmMagentaHue(void);
void AdjustAcmCyanHue(void);
void AdjustAcmBlackHue(void);
void AdjustAcmWhiteHue(void);

// ACM3D Luma adjusters
void AdjustAcmRedLuma(void);
void AdjustAcmGreenLuma(void);
void AdjustAcmBlueLuma(void);
void AdjustAcmYellowLuma(void);
void AdjustAcmMagentaLuma(void);
void AdjustAcmCyanLuma(void);
void AdjustAcmBlackLuma(void);
void AdjustAcmWhiteLuma(void);

//void AdjustColorScheme(void);
void AdjustACM3DColorScheme(void);
//Adjust AGC
void AdjustDecoderAGCMethod(void);
//Adjust TTX
void AdjustTTXLanguageGroup(void);
//AdjustAVS
void AdjustAVSDelayOnOff(void);
void AdjustAVSDelayTime(void);
void AdjustComponentRouteMode(void);

//Adaptive MADI, TNR, CCS
void AdjustMADI(void);
void AdjustTNR(void);
void AdjustCCS(void);
void AdjustSharpNoise(void);

void AdjustBlueStretch(void);

//
// Engineering menu
//

// Main Advanced Sharpness
void AdjustMVF_SHARPNESS_LUMA(void);
void AdjustMVF_SHARPNESS_CHROMA(void);
void AdjustMVF_NOISE_CORING_THRESH(void);
void AdjustMVF_NONLINEAR_THRESH1(void);
void AdjustMVF_NONLINEAR_GAIN1(void);
void AdjustMVF_NONLINEAR_THRESH2(void);
void AdjustMVF_NONLINEAR_GAIN2(void);
void AdjustMHF_SHARPNESS_LUMA(void);
void AdjustMHF_SHARPNESS_CHROMA(void);
void AdjustMHF_NOISE_CORING_THRESH(void);
void AdjustMHF_NONLINEAR_THRESH1(void);
void AdjustMHF_NONLINEAR_GAIN1(void);
void AdjustMHF_NONLINEAR_THRESH2(void);
void AdjustMHF_NONLINEAR_GAIN2(void);
// PIP Advanced Sharpness
void AdjustPVF_SHARPNESS_LUMA(void);
void AdjustPVF_SHARPNESS_CHROMA(void);
void AdjustPVF_NOISE_CORING_THRESH(void);
void AdjustPVF_NONLINEAR_THRESH1(void);
void AdjustPVF_NONLINEAR_GAIN1(void);
void AdjustPVF_NONLINEAR_THRESH2(void);
void AdjustPVF_NONLINEAR_GAIN2(void);
void AdjustPHF_SHARPNESS_LUMA(void);
void AdjustPHF_SHARPNESS_CHROMA(void);
void AdjustPHF_NOISE_CORING_THRESH(void);
void AdjustPHF_NONLINEAR_THRESH1(void);
void AdjustPHF_NONLINEAR_GAIN1(void);
void AdjustPHF_NONLINEAR_THRESH2(void);
void AdjustPHF_NONLINEAR_GAIN2(void);
// Enhancer
void AdjustHEM_HLE_THR(void);
void AdjustHEM_HLE_GAIN(void);
void AdjustHEM_HDP_THR(void);
void AdjustHEM_HDP_GAIN(void);
void AdjustHEM_CE_THR(void);
void AdjustHEM_CE_GAIN(void);
void AdjustVDP_THRESHOLD(void);
void AdjustVDP_GAIN(void);

void AdjustMPEGNRMode(void);
void AdjustMPEGNRThresh(void);

void AdjustACCScheme(void);
void AdjustDELLACCScheme(void);//120209 Edward add from 2410
//void AdjustACMScheme(void);
//void AdjustACCACMScheme(void);

void AdjustThreeTwoFilmDisplayMode(void);
// Keep the define to maintain backward compatibility
#define IsPanel_LG_WXGA_LM171W01			IsFilmDisplayModeSupport
BOOL IsFilmDisplayModeSupport(void);
void AdjustYUV2RGBExpansionMode(void);

void AdjustNonACMFleshtoneCorrectionLevel(void);
void AdjustNonACMFleshtoneBalance(void);
void AdjustNonACMFleshtoneCorrection(void);

void AdjustCCSforHD(void);
void AdjustTwoStageProcessing(void);

void SetSharpnessNoiseCoringToTNR(void);

// Sequoia Enhancers
void AdjustReg_MHF_ENH_Y_LSHOOT_TOL(void);
void AdjustReg_MHF_ENH_Y_SSHOOT_TOL(void);
void AdjustReg_MHF_ENH_Y_LGAIN(void);
void AdjustReg_MHF_ENH_Y_SGAIN(void);
void AdjustReg_MHF_ENH_Y_FINALSGAIN(void);
void AdjustReg_MHF_ENH_Y_FINALLGAIN(void);
void AdjustReg_MHF_ENH_Y_FINALGAIN(void);
void AdjustReg_MHF_ENH_Y_DELTA(void);
void AdjustReg_MHF_ENH_Y_SLOPE(void);
void AdjustReg_MHF_ENH_Y_THRESH(void);
void AdjustReg_MHF_ENH_Y_HIGH_SLOPE_AGC(void);
void AdjustReg_MHF_ENH_Y_LOW_SLOPE_AGC(void);
void AdjustReg_MHF_ENH_Y_HIGH_THRESH_AGC(void);
void AdjustReg_MHF_ENH_Y_LOW_THRESH_AGC(void);
void AdjustReg_MHF_ENH_UV_LSHOOT_TOL(void);
void AdjustReg_MHF_ENH_UV_SSHOOT_TOL(void);
void AdjustReg_MHF_ENH_UV_LGAIN(void);
void AdjustReg_MHF_ENH_UV_SGAIN(void);
void AdjustReg_MHF_ENH_UV_FINALSGAIN(void);
void AdjustReg_MHF_ENH_UV_FINALLGAIN(void);
void AdjustReg_MHF_ENH_UV_FINALGAIN(void);
void AdjustReg_MHF_ENH_UV_DELTA(void);
void AdjustReg_MHF_ENH_UV_SLOPE(void);
void AdjustReg_MHF_ENH_UV_THRESH(void);
void AdjustReg_MHF_ENH_UV_HIGH_SLOPE_AGC(void);
void AdjustReg_MHF_ENH_UV_LOW_SLOPE_AGC(void);
void AdjustReg_MHF_ENH_UV_HIGH_THRESH_AGC(void);
void AdjustReg_MHF_ENH_UV_LOW_THRESH_AGC(void);
void AdjustReg_MVF_ENH_Y_LSHOOT_TOL(void);
void AdjustReg_MVF_ENH_Y_SSHOOT_TOL(void);
void AdjustReg_MVF_ENH_Y_LGAIN(void);
void AdjustReg_MVF_ENH_Y_SGAIN(void);
void AdjustReg_MVF_ENH_Y_FINALSGAIN(void);
void AdjustReg_MVF_ENH_Y_FINALLGAIN(void);
void AdjustReg_MVF_ENH_Y_FINALGAIN(void);
void AdjustReg_MVF_ENH_Y_DELTA(void);
void AdjustReg_MVF_ENH_Y_SLOPE(void);
void AdjustReg_MVF_ENH_Y_THRESH(void);
void AdjustReg_MVF_ENH_Y_HIGH_SLOPE_AGC(void);
void AdjustReg_MVF_ENH_Y_LOW_SLOPE_AGC(void);
void AdjustReg_MVF_ENH_Y_HIGH_THRESH_AGC(void);
void AdjustReg_MVF_ENH_Y_LOW_THRESH_AGC(void);
void AdjustReg_MVF_ENH_UV_LSHOOT_TOL(void);
void AdjustReg_MVF_ENH_UV_SSHOOT_TOL(void);
void AdjustReg_MVF_ENH_UV_LGAIN(void);
void AdjustReg_MVF_ENH_UV_SGAIN(void);
void AdjustReg_MVF_ENH_UV_FINALSGAIN(void);
void AdjustReg_MVF_ENH_UV_FINALLGAIN(void);
void AdjustReg_MVF_ENH_UV_FINALGAIN(void);
void AdjustReg_MVF_ENH_UV_DELTA(void);
void AdjustReg_MVF_ENH_UV_SLOPE(void);
void AdjustReg_MVF_ENH_UV_THRESH(void);
void AdjustReg_MVF_ENH_UV_HIGH_SLOPE_AGC(void);
void AdjustReg_MVF_ENH_UV_LOW_SLOPE_AGC(void);
void AdjustReg_MVF_ENH_UV_HIGH_THRESH_AGC(void);
void AdjustReg_MVF_ENH_UV_LOW_THRESH_AGC(void);


void AdjustEnhancerEnable(void);
void AdjustOldEnhancerEnable(void);
void AdjustOldEnhancerRouting(void);
void AdjustColorResourceRouting(void);
BOOL AllowNewEnhancers(void);
BOOL OldEnhancerEnableChangedByUser(BYTE inValue);
#endif


