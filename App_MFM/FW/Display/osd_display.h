/*
	$Workfile:   osd_display.h  $
	$Revision: 1.18 $
	$Date: 2013/04/08 02:01:38 $
*/
#ifndef __OSD_DISPLAY_H__
#define __OSD_DISPLAY_H__
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
// MODULE:      osd_display.h
//
// USAGE:
//
//******************************************************************************

typedef struct 		//Lvjie 20081028 moved from 2709
{
	SDWORD SDW_AdjValue;
	SDWORD SDW_SrcMin;
	SDWORD SDW_SrcMax;
	SWORD  SW_DstMaxValue;
	SWORD  SW_DstMinValue;
} ValueScaleConvertType;

typedef enum ImageSchemeEnum
{
	//120328 Edward for DELL input
	INPUT_DVI1=0, 	// 0
	INPUT_DP,		// 1
	#ifdef MUX_TEST_BOARD
	INPUT_MiniDP, 	// 2
	#endif
	INPUT_HDMI, 	// 3
	INPUT_VGA, 	// 4
	#ifdef EXTERNAL_DECODER
	//INPUT_DIP,
	INPUT_CVBS1,	//5	//Replace origin INPUT_DIP
	#endif
	#if(ON_BOARD_COMP == ENABLE)
	INPUT_COMP,	//6
	#endif
	#ifdef EXTERNAL_DECODER
	INPUT_COMP2,	//7
	INPUT_CVBS2,	//8
	#endif

	
	NUM_IMAGE_SCHEMES

	/*INPUT_VGA = 0,
	INPUT_DVI1,		// 1
	INPUT_DVI2,		// 2
	INPUT_DP,		// 3
	INPUT_HDMI,	// 4
	INPUT_COMP,	// 5
	INPUT_CVBS,	// 6
	NUM_IMAGE_SCHEMES*/
	
} ImageSchemes;

typedef enum ImageSchemeEnum_2
{
	USER=0,
	VIVID,
	CINEMA,
	GAME,
	SPORT,	
	NUM_IMAGE_SCHEMES_2
} ImageSchemes_2;

//#else

#define VWD_SCHEME     100      // Color scheme for VWD

//
// Color Space definition
//
typedef enum COLOR_SPACE
{
	ColorSpace_sRGB = 0,
	ColorSpace_Adobe98,
	ColorSpace_XvYcc,
	ColorSpace_DICOM,
	MAX_COLOR_SPACE_2
} COLOR_SPACE;


//
// Color Temperature definition
//
#ifdef DELL_U2410F
typedef enum DELL_COLOR_TEMP
{
	//DEFAULT_CTEMP = 0,
	CTEMP_5000K,			// 0
	CTEMP_5700K,		// 0	// 1
	CTEMP_6500K,		// 1	// 2
	CTEMP_7500K,		// 2	// 3  
	CTEMP_9300K,		// 3	// 4
	CTEMP_10000K,		// 4	// 5
	CTEMP_USER,				// 6
	MAX_DELL_COLOR_TEMP ,	// 7
	MAX_COLOR_TEMP = MAX_DELL_COLOR_TEMP,		//HYB121214*
	CTEMP_7200K,			// 8
} DELL_COLOR_TEMP;

typedef enum DELL_COLOR_SPACE
{
	CSPACE_AdobeRGB,		// 0
	CSPACE_sRGB,	// 1
	CSPACE_xvMode,	// 2
	//CSPACE_SMPTE,	      // 3  
	//CSPACE_REC709,      // 4
	//CSPACE_REC601,      // 5
	CSPACE_DCI_P3,      //6
//&*&*&*S 20120315:SWRD4001: Kordonm(0)  Modify for OSD Workbench
	CSPACE_CAL1,      //7
	CSPACE_CAL2,      //8
//&*&*&*E 20120315 : Kordonm(1)  Modify for OSD Workbench
	MAX_COLOR_SPACE
} DELL_COLOR_SPACE;

typedef enum DELL_PRINTER_PREVUE
{
	PRINTER_PREVUE1,    // 0
	PRINTER_PREVUE2,	// 1
	PRINTER_PREVUE3,	// 2
	MAX_PRINTER_PREVUE
} DELL_PRINTER_PREVUE;

typedef enum COLOR_UNIFORMITY
{
	UNIFORMITY_OFF,    // 0	
	UNIFORMITY_Table1,	// 1
	UNIFORMITY_Table2,	// 2
	MAX_UNIFORMITY
} COLOR_UNIFORMITY;


#else
typedef enum COLOR_TEMP
{
	DEFAULT_CTEMP = 0,
	CTEMP_4000K,
   	CTEMP_5000K,
   	CTEMP_6500K,
   	CTEMP_7500K,
	CTEMP_8200K,
   	CTEMP_9300K,
	CTEMP_10000K,
	CTEMP_11500K,
   	CTEMP_USER,
   	MAX_COLOR_TEMP
} COLOR_TEMP;
#endif

//
// Image scheme adjustment parameters
//
typedef struct
{
   SBYTE	sB_Brightness;
   WORD	W_Contrast;
   WORD	W_Saturation;
} gmt_ColorScheme;

//
// Flesh tones compensation levels
//
typedef enum FleshToneLevel
{
   FT_OFF = 0,
   FT_LOW,
   FT_MODERATE,
   FT_HIGH,
   NUM_FLESH_TONE
} FleshToneLevel;

//
// Simultaneous Actions in diferent thread
//
typedef enum SimultaneousAction
{
   ERASE_BLOCKED_CHANNEL,
   DISPLAY_BLOCKED_CHANNEL
} SimultaneousAction;

//
// CSC 3x3 matrix selection
//
typedef enum
{
   CSC_FULL_EXPANSION,
   CSC_LINEAR_EXPANSION,
   CSC_PARTIAL_EXPANSION,
   CSC_UNITY_MATRIX,
} CSC_MATRIX;


//
// UART Switch Selection
//
typedef enum
{
	UART_TO_ATHENA_R,
	UART_TO_ATHENA_L,
   UART_TO_ATHENA_FE,
   UART_TO_VEGA,
}UART_TO;

//
// PBP SETTING
//
typedef enum
{
	PBP_OFF,
	PBP_DP1_HDMI,
   PBP_DP2_HDMI,
   PBP_HDMI_DP1,
   PBP_HDMI_DP2,
}PBP_SETTING;

// OSD status
//

//******************************************************************************
//  E X T E R N A L    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
void AdjustPanelBacklight(BYTE B_Percent);
void AdjustBrightness(void);
void AdjustContrast(void);
void AdjustSaturation(void);
void AdjustHue(void);
void AdjustSharpness(void);
void AdjustSharpnessRGB(void);
void Adjust3DNR(void);
void AdjustMPEGNR(void);
void AdjustGNR(void);

void AdjustRedGain(void);
void AdjustGreenGain(void);
void AdjustBlueGain(void);

void AdjustRedOff(void);
void AdjustGreenOff(void);
void AdjustBlueOff(void);

void AdjustScartRedGain(void);
void AdjustScartGreenGain(void);
void AdjustScartBlueGain(void);

void AdjustScartRedOff(void);
void AdjustScartGreenOff(void);
void AdjustScartBlueOff(void);

BOOL IsColorTempAvailable(BYTE ColorTempRequest);
void AdjustColorTemp(void);
void AdjustColorSpace(void);
#ifdef USE_GAMMMA_TABLE_FROM_NVRAM
void AdjustColorSpace_NVRAM(void);//For DELL firmware
#endif
void AdjustScartRGBViaOverlay(void);

void AdjustImageScheme(void);
void AdjustFleshTone(void);
void FreezeDisplay(void);
void ToggleFreezeVar(void);

void AdjustDynamicContrast(void);
BYTE GetMaxAccTable (void);
void AdjustFleshToneLevel(void);
BOOL IsCCDefined(void);
BOOL IsTTXDefined(void);
BOOL IsNoTunerDefined(void);
void ToggleInfoDisplay(void);
BOOL IsHDMIDefined(void);

void SetOsdBitFlag(void);
void ClrOsdBitFlag(void);
#define OsdTtxOnBit		BIT0 
#define OsdTtxOnFlag		(UserPrefOsdBitFlags & OsdTtxOnBit)
BOOL IsAutoScanDefined(void);
void SimultaneousActionInDifferentThread(void);
void SetOsdReadyForPowerDown(void);
void SetOsdRequestForPowerUp(void);
void SelectCscMatrix(CSC_MATRIX Mode);


BOOL IsNoSync(void);
BOOL IsNoCable(void);
BOOL IsInvalidMode(void);
BOOL ToggleMenuStyle(void);
BOOL DisplayInfoCond(BOOL mainDisplay, int numnum);
BOOL IsPipFocusRight(void);
BOOL IsPipNoSync(void);
void ScheduleNoSignalMsg(void);
void CancelNoSignalMsg(void);
BOOL IsAUDIO_LS_HP_INDEPENDENTDefined(void);

BOOL IsGameModeApplicable(void);
void AdjustGameMode(void);
void AdjustXvYcc(void);
void AdjustDCR(void);
void DCR_Handler( void);

#ifdef DELL_U2410F

void ValueScaleConvert(ValueScaleConvertType* ptr);//Lvjie 20081028

extern ValueScaleConvertType vConverT;
#endif

#endif

