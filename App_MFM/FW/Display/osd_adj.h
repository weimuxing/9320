/*
	$Workfile:   osd_adj.h  $
	$Revision: 1.21 $
	$Date: 2013/03/14 05:53:18 $
*/
#ifndef __OSD_ADJ_H__
#define __OSD_ADJ_H__
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
// MODULE:      osd_adj.h
//
// USAGE:
//
//******************************************************************************

//******************************************************************************
//  T Y P E    D E F I N I T I O N S
//******************************************************************************

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************

// Ballastic Acceleration Speed
#define BALLASTIC_ACCELERATION_SPEED		10	//FAST (10ms)
//#define BALLASTIC_ACCELERATION_SPEED		25	//MEDIUM (25ms)
//#define BALLASTIC_ACCELERATION_SPEED		50	//SLOW (50ms)

//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************

enum
{
   DOWN,
   UP,
   LEFT,
   RIGHT,
   TVMain,
   TVPip,
};

#define SBS_DEFAULT  5		// for calling DynamicScale() and telling it to bring size back to 50/50

//All frequencies are defined in Hz
#define START_SPECTRUM_FREQ_HZ		47000000UL	//47 MHz
#define END_SPECTRUM_FREQ_HZ		855500000UL	//855.5 MHz

#define MAX_POSSIBLE_ITERATIONS	4

//
// PAL Tuner Application Definitions
//
#define MINCHANNELFREQSLIDER       (BYTE)(((START_SPECTRUM_FREQ_HZ/1000000UL)/4)/MAX_POSSIBLE_ITERATIONS)
#define MAXCHANNELFREQSLIDER      (BYTE)((END_SPECTRUM_FREQ_HZ/1000000UL)/4)

#define MINCHANNELFREQVALUE		(WORD)(START_SPECTRUM_FREQ_HZ/1000000UL)
#define MAXCHANNELFREQVALUE		(WORD)(END_SPECTRUM_FREQ_HZ/1000000UL)


typedef enum
{
   TTX_WESTERN_EUROPE,
   TTX_EASTERN_EUROPE,
   TTX_RUSSIAN,
   TTX_ARABIC,
}Ttx_Regions;

typedef enum
{
   TTX_BLEND_100,
   TTX_BLEND_50,
   MAX_TTX_BLEND_LEVELS,
}Ttx_Blend_Levels;

typedef enum
{
   TTX_LM_TOGGLE_LIST_MODE,
   TTX_LM_RED_KEY,
   TTX_LM_GREEN_KEY,
   TTX_LM_YELLOW_KEY,
   TTX_LM_BLUE_KEY
}Ttx_ListMode_Keys;

typedef enum
{
   CHIP_FLI8532,
   CHIP_FLI8668,
   CHIP_FLI8538,
   CHIP_FLI30436,
   CHIP_FLI326xx,
   CHIP_FLI326xxBx
}Chips;

typedef enum
{
   NATIVE_MODE_OFF,
   NATIVE_MODE_NO_FILTERS,
   NATIVE_MODE_WITH_FILTERS
} NativeModeSettings;
//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************

//******************************************************************************
//  E X T E R N A L    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

WORD BallisticAcceleration(void);
BYTE GetChipID(void);

void SaveGrModeDependentSettings(void);
void RequestSoftPowerOff(void);
void RequestSoftPowerOn(void);
void FactoryReset(void);


void DynamicScale(void);
void DynamicScaling_DTV (gmt_PHY_CH B_Channel,
                         WORD Width,
                         WORD Height,
                         WORD Xpos,
                         WORD Ypos) ;

void TurnCcOnOff	(void);
void AdjustCCConfig	(void);
BOOL IsCC_On		(gmt_PHY_CH B_Channel);
BYTE GetCC_Config(	void);
void AdjustVBIService(void);

//
//  HELPER FUNCTIONS
//
void ToggleScrollMenuItem(void);


void TurnOffMainBlender(void);
void TurnOnMainBlender(void);
void ListModeHandler(void);
void SubPageModeHandler(void);

void CcDataSourceSwap();
void CcDataSourceSwitch(void);

void ScaleOsdToFullScreen(void);


//void ToggleMainRouteModeComponent(void);
//void ToggleMainRouteModeScartRGB(void);
//void TogglePipRouteModeComponent(void);
//void TogglePipRouteModeScartRGB(void);
//void SetMainRouteModeComponent( BYTE B_UserPrefMainRouteModeComponent );
//BYTE GetMainRouteModeComponent( void );
//void SetMainRouteModeScartRGB( BYTE B_UserPrefMainRouteModeScartRGB );
//BYTE GetMainRouteModeScartRGB( void );
//void SetPipRouteModeComponent( BYTE B_UserPrefPipRouteModeComponent );
//BYTE GetPipRouteModeComponent( void );
//void SetPipRouteModeScartRGB( BYTE B_UserPrefPipRouteModeScartRGB );
//BYTE GetPipRouteModeScartRGB( void );

void AdjustNativeMode(void);
BOOL IsOutputBlocked(void);
BOOL ForceSameAudioSource(void);

BOOL IsAnyTTXDataAvailable(void);
void ResetParentalSettings(void);
BOOL IsTtxSubtitlePage(void);
BOOL IsTtxSubtitlePageReEnableMode(void);
void ImageSchemeReset(void); // PSD
void gm_SetCcSourceOnFocus(gmt_PHY_CH channel);
void AdjustSub(void);
BOOL IsGotoSleep(void);
void AdjustColorDomain(void);
BOOL IsAVInfoSource(void);
void PIP_Demo(void);
void AdjustDUMSwitch(void); // OSD used ...
void ApplyDUMTable(void);//120813 Edward for OSD apply after "tick" press
#if (FEATURE_DUM == ENABLE)
void DisplayUniformityModule(BOOL B_EnableDUM, BYTE B_Is3Ch, BYTE B_XGRIDS, BYTE B_YGRIDS, WORD W_PanelWidth, WORD W_PanelHeight);
#if defined(SAVING_DATA_INTO_FLASH) // Flash data saving 	 
   void DUM_FlashCheck(void);
#elif defined(SAVING_DATA_INTO_NVRAM)
   void DUM_NVRAMCheck(void);
	void DUM_SAVEtoNVRAM(void);
void InitializeGammaNvram(BYTE C_Selected_Profile); // Clear Gamma NVRAM
#endif
#endif
void AdjustOSDBlending(void);
void AdjustOSDBlending1(void);
void AdjustOSDBlending2(void);
void AdjustCaptureMain(void);
void AdjustImageRotation(void);
void AdjustDPSwitch(void);
#ifdef ST_4K2K_93xx_BOARD
void AdjustUARTSwitch(void);
void AdjustPBP(void);
void External_UART_Switch(BYTE number);
void DrawSTimingInfo(void);
WORD CheckVideoTimingResolution(gmt_PHY_CH B_Channel, WORD W_VActive);
#endif
//void AdjustDPSwitch_Init(void);
void gm_ImageHFlipControl(BOOL Bt_State);
void gm_ImageVFlipControl(BOOL Bt_State);

void EditChannelList(void);
#endif __OSD_ADJ_H__
//*********************************  END  **************************************

