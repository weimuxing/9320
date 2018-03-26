
#define __STUBS_C__
//******************************************************************************
//
//              Copyright (C) 2002, 2003.  GENESIS MICROCHIP INC.
//      All rights reserved.  No part of this program may be reproduced.
//
//     Genesis Microchip Inc., 165 Commerce Valley Dr. West
//     Thornhill, Ontario, Canada, L3T 7V8
//
//==============================================================================
//
// MODULE:     	Stubs.c
//
// USAGE:		This module contains miscelaneous osd stuff
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#include "system\all.h"
#include "StdLibInc\gm_Register.h"
#include <math.h>
#include <embedded.h>
#include "Board\PortAttrib.h"
#include <string.h>
#if (FEATURE_ACR == ENABLE)
 #include "system\ACR_Handler.h"
#endif
#include <mem.h>

//******************************************************************************
//  E X T E R N S
//******************************************************************************

extern PortSchemeDependentUserPrefType ROM SchemeFactoryDefaultInROM[NUM_IMAGE_SCHEMES]; 
extern BYTE DpRxGetPortId(gmt_UserDefConnNames b_InputConnector);
#ifdef USER_IR_SUPPORT
extern USER_IRType ROM  USER_IR_Data[];
extern BYTE ROM IRKey_Factory[]; 
#endif

//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************

//#define _DEBUG
#ifdef _DEBUG
	#define msg(a)            gm_Print(a,0)
	#define msg_i(a,b)      gm_Print(a,b)
#else
	#define msg(a)              
	#define msg_i(a,b)
#endif

#ifdef DELL_U2410F
BYTE MenuKeyState = ZERO_KEYSTATE;

#define glyph_N	17
#define glyph_T	18
#define glyph_S	19
#define glyph_C	20
#define glyph_P	21
#define glyph_A	22
#define glyph_L	23
#define dTimingInfoSecamOffset		26
#define glyph_i_lower	31
#define glyph_p_lower	32
#define dTimingInfoNoSignalOffset		34

#define MainIconDialogId 		143 //146
#define PIPIconDialogId		144 //147
#define PIPMessageDialogId    145 //148

#define ROTATED_FLY_YPOSITION  0xFFFF; //KevinL 20090310  Vstart 360

#define MAX_BRIGHTNESS			255 
#define MID_BRIGHTNESS			128 
#define MIN_BRIGHTNESS			0
#define DEFAULT_BRIGHTNESS		128 

#ifdef INPUTSCAN
#define NUM_OF_SCAN_SOURCE	3
#define	PERIOD_OF_START_INPUT_SCAN_DC	25 //KevinL 20090410
#define	PERIOD_OF_START_INPUT_SCAN_AC	60 //KevinL 20090422 Add 3.5 Sec. for DELL Logo display time
#define	PERIOD_OF_INPUT_SCAN	15
#endif
#if 1 // flying window
#define UP				2
#define LEFT				1
#define MAX_VER			600 //wei 140627  1000 // 960 // 1280 // 1000UL //(1000UL - DIALOG_SIZE_V)
#define MAX_HOR			600 //wei 140627  1000 // 690 // 862 // 1000UL //(1000UL - DIALOG_SIZE_H)
#define MIN_VER                   400 //wei 140718
#define MIN_HOR                  400 //wei 140718
#define MAX_VER_PORTRAIT  600 //wei 140627  1000//KevinL 20090311
#define MAX_HOR_PORTRAIT 300 //wei 140718
#define MIN_VER_PORTRAIT  400 //wei 140627  0
#define MIN_HOR_PORTRAIT 100 //wei 140627  
#define ABS_DIFF(a,b)			((a>b)?(a-b):(b-a))

BYTE	Flying = 0;
WORD StoreOsdHPosition = 500, StoreOsdVPosition = 500;
DWORD prevOnTime = 0UL;
//extern BYTE EnableDDC2BiAddress0x7C;//20120726 Add by KevinChen
extern gmt_UserDefConnNames MovePortFromConflictPort(gmt_UserDefConnNames Port);
// Louis 041309
//extern BOOL UnSupportAudio;
#endif

typedef struct
{
	PresetModesType st; //structure PSD
	BYTE cs; //check sum
} PresetModesTypeCS; // PSD


//******************************************************************************
//  S T A T I C    V A R I A B L E S
//******************************************************************************
#ifdef INPUTSCAN
BYTE B_INPUTSCAN_LOOP = 0;
BYTE OriginalInput = ALL_BOARDS_VGA1; //KevinL 20090117
#endif

#ifdef DELL_U2410F
BOOL gmvb_OsdReadyForPowerDown = FALSE;	// Osd status for power down
#endif

#if 0//(FEATURE_DPRX == ENABLE)
extern gmt_DPRX_AUDIO_SF GetDpRxAudioSampleRate(void);
#endif//#if (FEATURE_DPRX == ENABLE)

BYTE B_Force_ColorFormat = FALSE;
BYTE B_Force_ColorMode = FALSE;
#define OSD_Is_Force_ColorMode		B_Force_ColorMode
#define OSD_Is_Force_ColorFormat		B_Force_ColorFormat

PresetModesType PresetModeUserPreferences;
PresetModesType PipPresetModeUserPreferences;
PresetModesType ROM PresetModeFactoryDefaultInROM[NUM_IMAGE_SCHEMES] =
{
	//120328 Edward for DELL input
	// 0 INPUT_DVI1	
	{
		// 0 VideoPresetModes
		{
			// 0 Movie
			{
				0,			// Hue			
				50			// Saturation
			},
			// 1 Game
			{
				0,			// Hue			
				50			// Saturation
			},
			// 2 Nature
			{
				0,			// Hue			
				50			// Saturation
			},
			// 3 xvMode
			{
				0,			// Hue			
				50			// Saturation
			}				
		}
	},
	// 1 INPUT_DP
	{
		// 0 VideoPresetModes
		{
			// 0 Movie
			{
				0,			// Hue			
				50			// Saturation
			},
			// 1 Game
			{
				0,			// Hue			
				50			// Saturation
			},
			// 2 Nature
			{
				0,			// Hue			
				50			// Saturation
			},
			// 3 xvMode
			{
				0,			// Hue			
				50			// Saturation
			}				
		}
	},
	#ifdef MUX_TEST_BOARD
	// 2 INPUT_MiniDP
	{
		// 0 VideoPresetModes
		{
			// 0 Movie
			{
				0,			// Hue			
				50			// Saturation
			},
			// 1 Game
			{
				0,			// Hue			
				50			// Saturation
			},
			// 2 Nature
			{
				0,			// Hue			
				50			// Saturation
			},
			// 3 xvMode
			{
				0,			// Hue			
				50			// Saturation
			}				
		}
	},
	#endif
	 // 3 INPUT_HDMI
	{
		// 0 VideoPresetModes
		{
			// 0 Movie
			{
				0,			// Hue			
				50			// Saturation
			},
			// 1 Game
			{
				0,			// Hue			
				50			// Saturation
			},
			// 2 Nature
			{
				0,			// Hue			
				50			// Saturation
			},
			// 3 xvMode
			{
				0,			// Hue			
				50			// Saturation
			}				
		}
	},

	// 4 INPUT_VGA	
	{
		// 0 VideoPresetModes
		{
			// 0 Movie
			{
				0,			// Hue			
				50			// Saturation
			},
			// 1 Game
			{
				0,			// Hue			
				50			// Saturation
			},
			// 2 Nature
			{
				0,			// Hue			
				50			// Saturation
			},
			// 3 xvMode
			{
				0,			// Hue			
				50			// Saturation
			}				
		}
	},

	#ifdef EXTERNAL_DECODER
	// 5 INPUT_CVBS1	
	{
		// 0 VideoPresetModes
		{
			// 0 Movie
			{
				0,			// Hue			
				50			// Saturation
			},
			// 1 Game
			{
				0,			// Hue			
				50			// Saturation
			},
			// 2 Nature
			{
				0,			// Hue			
				50			// Saturation
			},
			// 3 xvMode
			{
				0,			// Hue			
				50			// Saturation
			}				
		}
	},
	#endif
	#if(ON_BOARD_COMP == ENABLE)
	// 6 INPUT_COMP	on board YPbPr
	{
		// 0 VideoPresetModes
		{
			// 0 Movie
			{
				0,			// Hue			
				50			// Saturation
			},
			// 1 Game
			{
				0,			// Hue			
				50			// Saturation
			},
			// 2 Nature
			{
				0,			// Hue			
				50			// Saturation
			},
			// 3 xvMode
			{
				0,			// Hue			
				50			// Saturation
			}				
		}
	},
	#endif

#ifdef EXTERNAL_DECODER
	// 7 INPUT_COMP2 on TVP5160	
	{
		// 0 VideoPresetModes
		{
			// 0 Movie
			{
				0,			// Hue			
				50			// Saturation
			},
			// 1 Game
			{
				0,			// Hue			
				50			// Saturation
			},
			// 2 Nature
			{
				0,			// Hue			
				50			// Saturation
			},
			// 3 xvMode
			{
				0,			// Hue			
				50			// Saturation
			}				
		}
	},

	// 8 INPUT_CVBS2	
	{
		// 0 VideoPresetModes
		{
			// 0 Movie
			{
				0,			// Hue			
				50			// Saturation
			},
			// 1 Game
			{
				0,			// Hue			
				50			// Saturation
			},
			// 2 Nature
			{
				0,			// Hue			
				50			// Saturation
			},
			// 3 xvMode
			{
				0,			// Hue			
				50			// Saturation
			}				
		}
	},
#endif
	
	
};

static BYTE B_PresetModesStore = 0xFF;						// Increased by ST_hansPENG 090205
static BYTE B_PresetColorSpace = 0xFF;						// Increased by ST_hansPENG 090205

//KevinL 20090320	
WORD ROM GraphicContrastBoundary[MAX_GRAPHICS_MODES][3] = 
{
	//order of 3 elements as follow:
	//min ,default  Max
	{ADJ_MIN_CONTRAST,  ADJ_DEFAULT_CONTRAST,  		ADJ_MAX_CONTRAST},     //Graphics_STANDARD 
	{ADJ_MIN_CONTRAST,  ADJ_DEFAULT_CONTRAST,  		ADJ_MAX_CONTRAST},     //Graphics_MULTIMEDIA
	{ADJ_MIN_CONTRAST,  ADJ_DEFAULT_CONTRAST,  		ADJ_MAX_CONTRAST},     //Graphics_GAME
	{ADJ_MIN_CONTRAST,  ADJ_DEFAULT_CONTRAST,  		ADJ_MAX_CONTRAST},     //Graphics_MOVIE	
	{ADJ_MIN_CONTRAST,  ADJ_TEXT_DEFAULT_CONTRAST,  ADJ_MAX_CONTRAST},     //Graphics_TEXT		
	{ADJ_MIN_CONTRAST,  ADJ_DEFAULT_CONTRAST,  		ADJ_MAX_CONTRAST},     //Graphics_COLOR_TEMP
	{ADJ_MIN_CONTRAST,  ADJ_DEFAULT_CONTRAST,  		ADJ_MAX_CONTRAST},     //Graphics_COLOR_SPACE
	//{0,  266,  306},     //Graphics_PRINTER
	{ADJ_MIN_CONTRAST,  ADJ_CUSTOM_DEFAULT_CONTRAST,ADJ_MAX_CONTRAST},//{0,  282,  WB_CONTRAST_MAX}      //Graphics_CUSTOM_COLOR	
};

WORD ROM YUVContrastBoundary[MAX_GRAPHICS_MODES][3] = 
{
	//order of 3 elements as follow:
	//min ,default  Max
	{ADJ_MIN_CONTRAST,  ADJ_DEFAULT_CONTRAST,  		ADJ_MAX_CONTRAST_YUV},     //Graphics_STANDARD 
	{ADJ_MIN_CONTRAST,  ADJ_DEFAULT_CONTRAST,  		ADJ_MAX_CONTRAST_YUV},     //Graphics_MULTIMEDIA
	{ADJ_MIN_CONTRAST,  ADJ_DEFAULT_CONTRAST,  		ADJ_MAX_CONTRAST_YUV},     //Graphics_GAME
	{ADJ_MIN_CONTRAST,  ADJ_DEFAULT_CONTRAST,  		ADJ_MAX_CONTRAST_YUV},     //Graphics_MOVIE	
	{ADJ_MIN_CONTRAST,  ADJ_TEXT_DEFAULT_CONTRAST,  ADJ_MAX_CONTRAST_YUV},     //Graphics_TEXT		
	{ADJ_MIN_CONTRAST,  ADJ_DEFAULT_CONTRAST,  		ADJ_MAX_CONTRAST_YUV},     //Graphics_COLOR_TEMP
	{ADJ_MIN_CONTRAST,  ADJ_DEFAULT_CONTRAST,  		ADJ_MAX_CONTRAST_YUV},     //Graphics_COLOR_SPACE
	//{0,  266,  306},     //Graphics_PRINTER
	{ADJ_MIN_CONTRAST,  ADJ_CUSTOM_DEFAULT_CONTRAST,ADJ_MAX_CONTRAST_YUV},//{0,  282,  WB_CONTRAST_MAX}      //Graphics_CUSTOM_COLOR	
};


WORD ROM VideoContrastBoundary[MAX_VIDEO_MODES][3] = 
{
	//order of 3 elements as follow:
	//min ,default  Max
	{0,  256,  306},     //Video_MOVIE
	{0,  256,  306},     //Video_GAME
	{0,  256,  306},     //Video_NATURE
	{0,  256,  306}      //Video_xvMode	
};

//******************************************************************************
//  S T A T I C    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

extern void gmp_HwDelay1ms(WORD W_Delay);

void SavePortDependentSettings () {};
void ClrForceBackground() {};
void RestorePipOnscreenState () {};
void ClrBitFlag () {};
void SetWbMainChannel () {};
void Osd_Hide () {};
void SetForceBackground () {};
void SavePipOnScreenState () {};
//void AdjustColorMode () {};
void AutoConfig() {};
void UpdateCustomRGB () {};
void UpdateSourceLeds() {};
void RestoreLedState() {};
void TurnSoundOff() {};
void TurnSoundOn() {};
void AdjustAudioPreset() {};
void ClearUserArea () {};
void FAdjustWriteReg () {};
void FUpdateGm5766AdcSettings() {};
//void ResetAudioSetting () {};
void SaveUserTimeInNVR () {};
void SetMessageHVPos() {};
//void StartSourceScan() {};
void UpdateColorTemp () {};
void AdjustAngleInterpolProp (){};
void MaxBlackLevel() {};
void AdjustAudioConfig() {};
void AdjustAudioInput() {};

void AdjustBlackLevel() {};
void AdjustBlue() {};
void MinBlackLevel() {};
void BallisticAccelerationForUserBlue () {};
void BallisticAccelerationForUserGreen() {};
//void AdjustColorFormat() {};
void BallisticAccelerationForUserRed() {};
void AdjustFilmModeProp () {};
//void AdjustForceBackGroundColor() {};
void AdjustGrBlacklevel() {};
void AdjustGreen() {};
void AdjustMadiProp() {};


void AdjustPanelResponseTime() {};
void AdjustPipBlendProp() {};
void AdjustPipHPosProp () {};
void AdjustPipSizeProp() {};
void AdjustPipVPosProp() {};
void AdjustPipValue () {};
void AdjustRed() {};
void AdjustTnrProp () {};
void AdjustUSBLink () {};
void FAdjustAmplitudeValue2() {};
void FAdjustGain_B() {};
void FAdjustGain_G() {};
void FAdjustGain_R() {};
void FAdjustIspMode() {};
void FAdjustNormalPresetGamma18Common() {};

void FAdjustAmplitudeValue() //120503 Kevin Chen update
{
	UserPrefSpreadAmpValue&=0x07;
	UserPrefSpreadPeriodValue&=0x3F;
	gm_WriteRegWord(DDDS1_ESM_CTRL,DDDS1_SPREAD_SP_EN | UserPrefSpreadAmpValue<<1 | (UserPrefSpreadPeriodValue <<4));//20120503 Modify By KevinChen
}

void FAdjustPeriodValue() //120503 Kevin Chen update
{
	UserPrefSpreadAmpValue&=0x07;
	UserPrefSpreadPeriodValue&=0x3F;
	gm_WriteRegWord(DDDS1_ESM_CTRL,DDDS1_SPREAD_SP_EN | UserPrefSpreadAmpValue<<1 | (UserPrefSpreadPeriodValue <<4));//20120503 Modify By KevinChen
}

void FAdjustOffset1_B ()
{
	FactoryAdjustADCValues();
}
void FAdjustOffset1_G () 
{
	FactoryAdjustADCValues();
}
void FAdjustOffset1_R ()
{
	FactoryAdjustADCValues();
}
void FAdjustOffset2_B ()
{
	FactoryAdjustADCValues();
}
void FAdjustOffset2_G ()
{
	FactoryAdjustADCValues();
}
void FAdjustOffset2_R ()
{
	FactoryAdjustADCValues();
}

//120406 Edward replace all in the factory adjust
void FAdjustColorTempCommon()
{
#if defined(WISTRON_93XX_BOARD_30)	//20121012
	BYTE B_ColorTemp;
	B_ColorTemp = UserPrefPipColorTemp;
	switch (B_ColorTemp)
	{
		case CTEMP_5000K:	// Graphics_WARM, Video_MOVIE	//5000K
			if(UserPrefColorT5000_R > 255) UserPrefColorT5000_R =255;
			if(UserPrefColorT5000_G > 255) UserPrefColorT5000_G =255;
			if(UserPrefColorT5000_B > 255) UserPrefColorT5000_B =255;
			break;			

		case CTEMP_5700K:	// YPbPr_GameVideo, YPbPr_Nature  //zuguang 20090318				
			if(UserPrefColorT5700_R > 255) UserPrefColorT5700_R =255;
			if(UserPrefColorT5700_G > 255) UserPrefColorT5700_G =255;
			if(UserPrefColorT5700_B > 255) UserPrefColorT5700_B =255;
			break;
			
		case CTEMP_6500K:	// Graphics_STANDARD
			if(UserPrefColorT6500_R > 255) UserPrefColorT6500_R =255;
			if(UserPrefColorT6500_G > 255) UserPrefColorT6500_G =255;
			if(UserPrefColorT6500_B > 255) UserPrefColorT6500_B =255;
			break;		

		case CTEMP_7200K:	//  for Multimedia
			if(UserPrefColorT7200_R > 255) UserPrefColorT7200_R =255;
			if(UserPrefColorT7200_G > 255) UserPrefColorT7200_G =255;
			if(UserPrefColorT7200_B > 255) UserPrefColorT7200_B =255;
			break;	
			
		case CTEMP_7500K:	// Graphics_STANDARD
			if(UserPrefColorT7500_R > 255) UserPrefColorT7500_R =255;
			if(UserPrefColorT7500_G > 255) UserPrefColorT7500_G =255;
			if(UserPrefColorT7500_B > 255) UserPrefColorT7500_B =255;
			break;					
			
		case CTEMP_9300K:	// Graphics_COOL
			if(UserPrefColorT9300_R > 255) UserPrefColorT9300_R =255;
			if(UserPrefColorT9300_G > 255) UserPrefColorT9300_G =255;
			if(UserPrefColorT9300_B > 255) UserPrefColorT9300_B =255;
			break;			

		case CTEMP_10000K:	// YPbPr Movie				
			if(UserPrefColorT10000_R > 255) UserPrefColorT10000_R =255;
			if(UserPrefColorT10000_G > 255) UserPrefColorT10000_G =255;
			if(UserPrefColorT10000_B > 255) UserPrefColorT10000_B =255;
			break;	
		case CTEMP_USER:	// User ctemp			
			if(UserPrefUserRedGain > 255) UserPrefUserRedGain =255;
			if(UserPrefUserGreenGain > 255) UserPrefUserGreenGain =255;
			if(UserPrefUserBlueGain > 255) UserPrefUserBlueGain=255;
			break;				
	}
#endif
	CscUpdateColors(gmvb_MainChannel);
}
/*void FAdjustNormalPresetCommon() 
{
	CscUpdateColors(gmvb_MainChannel);
}

void FAdjustBluePresetCommon() 
{
	CscUpdateColors(gmvb_MainChannel);
}
void FAdjustRedPresetCommon() 
{
	CscUpdateColors(gmvb_MainChannel);
}*/

void FAdjustPeriodValue2() {};
void FAdjustReadReg() {};
void FAdjustRegisterNumber() {};
void FAdjustSpreadEnable() {};
void FAdjustTimeUsedHour () {};
void FAdjustTimeUsedMin () {};
void FAdjustUserPresetCommon() {};
#if(FEATURE_SOURCE_SCAN == ENABLE)
void far SourceScanHandler(void);
#endif
void LED_Handler(void);
void FlyingWindowHandler(void);
void BurnInHandler(void);
void HardLockTimerHandler(void);
void SplashScreenTimerHandler(void);
void DiagnosticTestPatternTimerHandler(void);
void MonitorOnTimeHandler(void);
void NoneKeyHandler(void);


//******************************************************************************
//  C O D E
//******************************************************************************
#if 0//GPIO2PWM
void AdjustBackLight(void)
{
	DWORD B_Pulse;
	DWORD G_Pulse;
	BYTE B_TempBrightness = UserPrefBrightness + 128;
	BYTE B_TempDefaultBrightness;
	BYTE MaxMin;
	WORD BriBaseTemp = 0;
	DWORD B_BriBase;
	DWORD G_BriBase;
	MaxMin = MAX_BACKLIGHT_PWM - MIN_BACKLIGHT_PWM; 
	if (UserPrefColorTemp == CTEMP_9300K)
		BriBaseTemp = UserPrefBriBase_6500K;
	else if ((UserPrefImageScheme == Graphics_COLOR_SPACE)&&(UserPrefPresetColorSpace == CSPACE_sRGB))
		BriBaseTemp = UserPrefBriBase_sRGB;
	else if ((UserPrefImageScheme == Graphics_COLOR_SPACE)&&(UserPrefPresetColorSpace == CSPACE_AdobeRGB))
		BriBaseTemp = UserPrefBriBase_AdobeRGB;
	else
		BriBaseTemp = UserPrefBriBase_6500K;
	if(IsPresetMode_GRAPHICS_TEXT) //Default 150 nits   
	{	    
	    B_TempBrightness = B_TempBrightness - 30;
	    if(B_TempBrightness<78)
		B_TempBrightness = 78;	
	}
	B_BriBase = ((WORD)(BriBaseTemp * MaxMin)) / 100 + MIN_BACKLIGHT_PWM;
	G_BriBase = ((WORD)(BriBaseTemp * MaxMin)) / 100 + MIN_BACKLIGHT_PWM;
	if (IsBrightnessAdjustBacklight())	// Modified by ST_hansPENG 090218
	{
		if (B_TempBrightness <= 128)
			{
			B_Pulse = ((DWORD)(B_BriBase - MIN_BACKLIGHT_PWM) * (DWORD)(B_TempBrightness - 78)) / 50 + MIN_BACKLIGHT_PWM;
			G_Pulse = ((DWORD)(G_BriBase - MIN_BACKLIGHT_PWM) * (DWORD)(B_TempBrightness - 78)) / 50 + MIN_BACKLIGHT_PWM;
			}
		else
			{
			B_Pulse = ((DWORD)(MAX_BACKLIGHT_PWM - B_BriBase)*(DWORD)(B_TempBrightness - 128)) / 50 + B_BriBase;
			G_Pulse = ((DWORD)(MAX_BACKLIGHT_PWM - G_BriBase)*(DWORD)(B_TempBrightness - 128)) / 50 + G_BriBase;
			}	
	}
	else
	{
		B_Pulse = DEFAULT_VIDEO_BACKLIGHT_PWM;
		G_Pulse = DEFAULT_VIDEO_BACKLIGHT_PWM;
	}
	if ((B_Pulse <= MIN_BACKLIGHT_PWM) || (G_Pulse <= MIN_BACKLIGHT_PWM))
		{
		B_Pulse = MIN_BACKLIGHT_PWM;
		G_Pulse = MIN_BACKLIGHT_PWM;
		}
	if ((B_Pulse >= MAX_BACKLIGHT_PWM) || (G_Pulse >= MAX_BACKLIGHT_PWM))
		{
		B_Pulse = MAX_BACKLIGHT_PWM;
	    G_Pulse = MAX_BACKLIGHT_PWM;
		}
	gm_WriteRegWord(BACKLIGHT_B_PWM_CTRL, MAX_PWMPERIOD |(B_Pulse << 8));
	gm_WriteRegWord(BACKLIGHT_G_PWM_CTRL, MAX_PWMPERIOD |(G_Pulse << 8));
	gm_WriteRegWord(BACKLIGHT_DCR_PWM_CTRL, 0xFFFF); //DCR
}
#else

void AdjustBackLight(void)
{
#if ((DEFAULT_PANEL == LG_WUXGA_LM240WU9_SLA1))
    BYTE B_def_brightness_position = 0;
    DWORD D_Temp_DCR_PWM = 0;
    WORD W_DCR_PWM_HI = MAX_DCR_BACKLIGHT_PWM & PWM0_PULSE_MSB;
    WORD W_DCR_PWM_LOW = (WORD)(MAX_DCR_BACKLIGHT_PWM & PWM0_PERIOD) << 8;
    WORD W_B_TempBrightness_HI = 0;
    WORD W_B_TempBrightness_LOW = 0;
    WORD W_G_TempBrightness_HI = 0;
    WORD W_G_TempBrightness_LOW = 0;
	BYTE B_Brightness;

    WORD W_B_PWM_Table[WB_Brightness_TAB_LEN] = {
        0x181, 0x18F, 0x19D, 0x1AA, 0x1B8, //
        0x1C4, 0x1D3, 0x1DF, 0x1ED, 0x1F9, 0x206, 0x213, 0x220, 0x22D, 0x23A, //      150cd/m2
        0x248, 0x254, 0x261, 0x26E, 0x27B, 0x286, 0x293, 0x29F, 0x2AC, 0x2B9, //50.5% 200cd/m2
        0x2C5, 0x2D1, 0x2DD, 0x2EA, 0x2F5, 0x301, 0x30E, 0x31A, 0x326, 0x331, //55.5% 215cd/m2
        0x33B, 0x348, 0x354, 0x360, 0x36C, 0x376, 0x382, 0x38E, 0x399, 0x3A6, //64.7% 250cd/m2
        0x3B0, 0x3BC, 0x3C8, 0x3D3, 0x3DF, 0x3E9, 0x3F5, 0x400, 0x40C, 0x416, //73.8% 280cd/m2
        0x421, 0x42D, 0x438, 0x442, 0x44E, 0x458, 0x464, 0x46F, 0x479, 0x484, //82.6% 310cd/m2
        0x490, 0x49A, 0x4A5, 0x4AF, 0x4BA, 0x4C4, 0x4CE, 0x4D9, 0x4E6, 0x4F3, //91.3% 340cd/m2
        0x4FF                                                                 //100%  380cd/m2
    };

    WORD W_G_PWM_Table[WB_Brightness_TAB_LEN] = {
        0x13F, 0x14C, 0x159, 0x166, 0x173, //25% 120cd/m2
        0x17F, 0x18C, 0x199, 0x1A6, 0x1B2, 0x1BF, 0x1CC, 0x1D9, 0x1E6, 0x1F3, //30%  150cd/m2
        0x1FF, 0x20C, 0x219, 0x226, 0x233, 0x23F, 0x24C, 0x259, 0x266, 0x273, //40%  200cd/m2
        0x27F, 0x28C, 0x299, 0x2A6, 0x2B2, 0x2BF, 0x2CC, 0x2D9, 0x2E6, 0x2F2, //50%  215cd/m2
        0x2FF, 0x30C, 0x319, 0x326, 0x333, 0x33F, 0x34C, 0x359, 0x365, 0x373, //60%  250cd/m2
        0x37F, 0x38C, 0x399, 0x3A6, 0x3B3, 0x3BF, 0x3CC, 0x3D9, 0x3E6, 0x3F2, //70%  280cd/m2
        0x3FF, 0x40C, 0x419, 0x425, 0x433, 0x43F, 0x44C, 0x459, 0x465, 0x472, //80%  310cd/m2
        0x47F, 0x48C, 0x499, 0x4A5, 0x4B3, 0x4BF, 0x4CC, 0x4D9, 0x4E6, 0x4F3, //90%  340cd/m2
        0x4FF                                                                 //100% 380cd/m2
    };

	if((IS_DCR_ON() || IS_ENERGY_SMART_ON()) && gm_IsValidSignal(CH_A))//120713 Edward for DCR / Energy use, OOR /no signal will restore original BL
    {
		B_Brightness = DCR_EnergyUse_Brightness;		
    }
	else
    {
		B_Brightness = UserPrefBrightness;
    }

	if(B_Brightness > WB_Brigntness_OSD_Max)
		B_Brightness = WB_Brigntness_OSD_Max;

	if(IS_UNIFORMITY_Off())//120731 Edward add for Foxconn UC On /Off
    {	
    	if (IsColorSpace_GRAPHICS_SRGB)
    	{
            B_def_brightness_position = (BYTE)(((DWORD)(UserPrefBriBase_sRGB)) * (WB_Brightness_TAB_LEN - 1) / WB_PWM_Factory_Max);
        }
		else if (IsColorSpace_GRAPHICS_ADOBE_RGB)
    	{
            B_def_brightness_position = (BYTE)(((DWORD)(UserPrefBriBase_AdobeRGB)) * (WB_Brightness_TAB_LEN - 1) / WB_PWM_Factory_Max);
        } 
		else if (IsPresetMode_GRAPHICS_TEXT) //120727 Edward for color tuning request set at OSD 35/100
    	{    	
            B_def_brightness_position = (BYTE)(((DWORD)(UserPrefBriBase_Text)) * (WB_Brightness_TAB_LEN - 1) / WB_PWM_Factory_Max);
        } 
		else
    	{
            B_def_brightness_position = (BYTE)(((DWORD)(UserPrefBriBase_6500K)) * (WB_Brightness_TAB_LEN - 1) / WB_PWM_Factory_Max);
		}
    }
	else
	{
		if(UserPrefDUMSwitch==UNIFORMITY_Table1)
		{			
            B_def_brightness_position = (BYTE)(((DWORD)(UserPrefBriBase_Table1)) * (WB_Brightness_TAB_LEN - 1) / WB_PWM_Factory_Max);
        } 
		else
		{			
            B_def_brightness_position = (BYTE)(((DWORD)(UserPrefBriBase_Table2)) * (WB_Brightness_TAB_LEN - 1) / WB_PWM_Factory_Max);
		}	
	}

  if(IS_UNIFORMITY_Off())
   {
    if (B_Brightness <= WB_Brigntness_OSD_DCR_Start) //Brightness 0 ~ 20
    {   
    	//20120808 Modifiy for contrl DCR_PWM by TigerSong, OSD_Brightness < 20. 120913 Edward add fine step    	
		#if PWM_LowLevel_Smooth
    	if(IS_ENERGY_SMART_ON())			
			D_Temp_DCR_PWM = MAX_DCR_BACKLIGHT_PWM - ((DWORD)((WB_Brigntness_OSD_DCR_Start*FineStep_DCR_PWM) - DCR_Energy_LowStep_Brightness)) * (MAX_DCR_BACKLIGHT_PWM - MIN_DCR_BACKLIGHT_PWM) / (WB_Brigntness_OSD_DCR_Start*FineStep_DCR_PWM);
		else
		#endif	
        	D_Temp_DCR_PWM = MAX_DCR_BACKLIGHT_PWM - ((DWORD)(WB_Brigntness_OSD_DCR_Start - B_Brightness)) * (MAX_DCR_BACKLIGHT_PWM - MIN_DCR_BACKLIGHT_PWM) / WB_Brigntness_OSD_DCR_Start;

		W_DCR_PWM_HI = (WORD) (D_Temp_DCR_PWM & PWM0_PULSE_MSB);
        W_DCR_PWM_LOW = (WORD) (D_Temp_DCR_PWM & PWM0_PERIOD) << 8;

        W_B_TempBrightness_HI = W_B_PWM_Table[0] & PWM0_PULSE_MSB;
        W_B_TempBrightness_LOW = (W_B_PWM_Table[0] & (PWM0_PULSE >> 8)) << 8;

        W_G_TempBrightness_HI = W_G_PWM_Table[0] & PWM1_PULSE_MSB;
        W_G_TempBrightness_LOW = (W_G_PWM_Table[0] & (PWM1_PULSE >> 8)) << 8;
    } 
	else if ((B_Brightness > WB_Brigntness_OSD_DCR_Start) && (B_Brightness <= WB_Brigntness_OSD_Default)) //Brightness 21 ~ 50
    {
        W_B_TempBrightness_HI = (WORD)((MIN_B_BACKLIGHT_PWM + (DWORD)(W_B_PWM_Table[B_def_brightness_position] - MIN_B_BACKLIGHT_PWM) * (B_Brightness - WB_Brigntness_OSD_DCR_Start) *10 / (WB_Brigntness_OSD_Default - WB_Brigntness_OSD_DCR_Start) /10) & 0x0F00);
        W_B_TempBrightness_LOW = (WORD)(((MIN_B_BACKLIGHT_PWM + (DWORD)(W_B_PWM_Table[B_def_brightness_position] - MIN_B_BACKLIGHT_PWM) * (B_Brightness - WB_Brigntness_OSD_DCR_Start) *10 / (WB_Brigntness_OSD_Default - WB_Brigntness_OSD_DCR_Start) /10) & 0xFF) << 8);

        W_G_TempBrightness_HI = (WORD)((MIN_G_BACKLIGHT_PWM + (DWORD)(W_G_PWM_Table[B_def_brightness_position] - MIN_G_BACKLIGHT_PWM) * (B_Brightness - WB_Brigntness_OSD_DCR_Start) *10 / (WB_Brigntness_OSD_Default - WB_Brigntness_OSD_DCR_Start) /10) & 0x0F00);
        W_G_TempBrightness_LOW = (WORD)(((MIN_G_BACKLIGHT_PWM + (DWORD)(W_G_PWM_Table[B_def_brightness_position] - MIN_G_BACKLIGHT_PWM) * (B_Brightness - WB_Brigntness_OSD_DCR_Start) *10 / (WB_Brigntness_OSD_Default - WB_Brigntness_OSD_DCR_Start) /10) & 0xFF) << 8);

        //gm_Print("W_B_TempBrightness_HI = 0x%x", W_B_TempBrightness_HI);
        //gm_Print("W_B_TempBrightness_LOW = 0x%x", W_B_TempBrightness_LOW);
        //gm_Print("W_G_TempBrightness_HI = 0x%x", W_G_TempBrightness_HI);
        //gm_Print("W_G_TempBrightness_LOW = 0x%x", W_G_TempBrightness_LOW);

    } 
	else if ((B_Brightness > WB_Brigntness_OSD_Default) && (B_Brightness < WB_Brigntness_OSD_Max)) //Brightness 51 ~ 99
    {
        W_B_TempBrightness_HI = (WORD)((W_B_PWM_Table[B_def_brightness_position] + (DWORD)(MAX_B_BACKLIGHT_PWM - W_B_PWM_Table[B_def_brightness_position]) * (B_Brightness - WB_Brigntness_OSD_Default) *10/ (WB_Brigntness_OSD_Max - WB_Brigntness_OSD_Default) /10) & 0x0F00);
        W_B_TempBrightness_LOW = (WORD)(((W_B_PWM_Table[B_def_brightness_position] + (DWORD)(MAX_B_BACKLIGHT_PWM - W_B_PWM_Table[B_def_brightness_position]) * (B_Brightness - WB_Brigntness_OSD_Default) *10 / (WB_Brigntness_OSD_Max - WB_Brigntness_OSD_Default) /10) & 0xFF) << 8);

        W_G_TempBrightness_HI = (WORD)((W_G_PWM_Table[B_def_brightness_position] + (DWORD)(MAX_G_BACKLIGHT_PWM - W_G_PWM_Table[B_def_brightness_position]) * (B_Brightness - WB_Brigntness_OSD_Default) *10 / (WB_Brigntness_OSD_Max - WB_Brigntness_OSD_Default) /10) & 0x0F00);
        W_G_TempBrightness_LOW = (WORD)(((W_G_PWM_Table[B_def_brightness_position] + (DWORD)(MAX_G_BACKLIGHT_PWM - W_G_PWM_Table[B_def_brightness_position]) * (B_Brightness - WB_Brigntness_OSD_Default) *10 / (WB_Brigntness_OSD_Max - WB_Brigntness_OSD_Default) /10) & 0xFF) << 8);

        //gm_Print("W_B_TempBrightness_HI = 0x%x", W_B_TempBrightness_HI);
        //gm_Print("W_B_TempBrightness_LOW = 0x%x", W_B_TempBrightness_LOW);
        //gm_Print("W_G_TempBrightness_HI = 0x%x", W_G_TempBrightness_HI);
        //gm_Print("W_G_TempBrightness_LOW = 0x%x", W_G_TempBrightness_LOW);

    } 
	else // Brightness=100 or Others
    {
        W_B_TempBrightness_HI = W_B_PWM_Table[WB_Brightness_TAB_LEN - 1] & PWM0_PULSE_MSB;
        W_B_TempBrightness_LOW = (W_B_PWM_Table[WB_Brightness_TAB_LEN - 1] & (PWM0_PULSE >> 8)) << 8;

        W_G_TempBrightness_HI = W_G_PWM_Table[WB_Brightness_TAB_LEN - 1] & PWM1_PULSE_MSB;
        W_G_TempBrightness_LOW = (W_G_PWM_Table[WB_Brightness_TAB_LEN - 1] & (PWM1_PULSE >> 8)) << 8;
    }
   }
  else
   { //chunbao for UC on brightness do not dependon osd value, use osd 50 uc_pwm
	 W_B_TempBrightness_HI = (WORD)((MIN_B_BACKLIGHT_PWM + (DWORD)(W_B_PWM_Table[B_def_brightness_position] - MIN_B_BACKLIGHT_PWM) * (WB_Brigntness_OSD_Default - WB_Brigntness_OSD_DCR_Start) *10 / (WB_Brigntness_OSD_Default - WB_Brigntness_OSD_DCR_Start) /10) & 0x0F00);
	 W_B_TempBrightness_LOW = (WORD)(((MIN_B_BACKLIGHT_PWM + (DWORD)(W_B_PWM_Table[B_def_brightness_position] - MIN_B_BACKLIGHT_PWM) * (WB_Brigntness_OSD_Default - WB_Brigntness_OSD_DCR_Start) *10 / (WB_Brigntness_OSD_Default - WB_Brigntness_OSD_DCR_Start) /10) & 0xFF) << 8);
	 W_G_TempBrightness_HI = (WORD)((MIN_G_BACKLIGHT_PWM + (DWORD)(W_G_PWM_Table[B_def_brightness_position] - MIN_G_BACKLIGHT_PWM) * (WB_Brigntness_OSD_Default - WB_Brigntness_OSD_DCR_Start) *10 / (WB_Brigntness_OSD_Default - WB_Brigntness_OSD_DCR_Start) /10) & 0x0F00);
	 W_G_TempBrightness_LOW = (WORD)(((MIN_G_BACKLIGHT_PWM + (DWORD)(W_G_PWM_Table[B_def_brightness_position] - MIN_G_BACKLIGHT_PWM) * (WB_Brigntness_OSD_Default - WB_Brigntness_OSD_DCR_Start) *10 / (WB_Brigntness_OSD_Default - WB_Brigntness_OSD_DCR_Start) /10) & 0xFF) << 8);
    }

    if(IS_DCR_ON() && (B_Brightness==DCR_HIGH_PWM_PULSE_MIN))//high DCR
    {
        gm_WriteRegWord(BACKLIGHT_DCR_PWM_CTRL_MSB, (WORD)(HIGHT_DCR_BACKLIGHT_PWM & PWM0_PERIOD) << 8 | ((MAX_DCR_BACKLIGHT_PWM >> 8) & PWM0_PERIOD_MS)); // DCR control
		gm_WriteRegWord(BACKLIGHT_DCR_PWM_CTRL, (WORD)(HIGHT_DCR_BACKLIGHT_PWM&PWM0_PERIOD)<<8 |(MAX_DCR_BACKLIGHT_PWM&PWM0_PERIOD));
        //gm_Print("###High DCR", 0);
    } 
	else
    {
        gm_WriteRegWord(BACKLIGHT_DCR_PWM_CTRL_MSB, W_DCR_PWM_HI | ((MAX_DCR_BACKLIGHT_PWM >> 8) & PWM0_PERIOD_MS)); // DCR control
        gm_WriteRegWord(BACKLIGHT_DCR_PWM_CTRL, W_DCR_PWM_LOW | (MAX_DCR_BACKLIGHT_PWM & PWM0_PERIOD));
    }

    gm_WriteRegWord(BACKLIGHT_B_PWM_CTRL_MSB, W_B_TempBrightness_HI | ((MAX_B_BACKLIGHT_PWM>>8)&PWM0_PERIOD_MS)); // B_led control
    gm_WriteRegWord(BACKLIGHT_B_PWM_CTRL, W_B_TempBrightness_LOW | (MAX_B_BACKLIGHT_PWM&PWM0_PERIOD));

    gm_WriteRegWord(BACKLIGHT_G_PWM_CTRL_MSB, W_G_TempBrightness_HI | ((MAX_G_BACKLIGHT_PWM >> 8) & PWM1_PERIOD_MS)); // G_led control
    gm_WriteRegWord(BACKLIGHT_G_PWM_CTRL, W_G_TempBrightness_LOW | (MAX_G_BACKLIGHT_PWM & PWM1_PERIOD));

#elif(DEFAULT_PANEL == LPL_WUXGA_LM240WU5)		//This panel use I2C adjust backlight, Range 0~0xFF;
	{
		#define BLAddress	0x6A
		BYTE Buffer[2];
		BYTE Brightness;

		Brightness = ((WORD)UserPrefBrightness * (MAX_BACKLIGHT_PWM - MIN_BACKLIGHT_PWM))/(WB_BRIGHTNESS_MAX-WB_BRIGHTNESS_MIN) + MIN_BACKLIGHT_PWM ;
			
		Buffer[0] = 0xC4;
		Buffer[1] = Brightness;

		if (gm_Write2WireBlock(0, BLAddress, (BYTE*)Buffer, 2, TRUE) == gmd_PASS)
		{
		   gm_Printf("Write to 0x%x at offset 0x%x to value %x succeeded",BLAddress, Buffer[0], Buffer[1]);
		}
		else
		{
		   gm_Printf("Write to 0x%x at offset 0x%x failed",BLAddress, Buffer[0]);
		}
	 }
#else

	DWORD W_Pulse;
    //BYTE B_TempBrightness = UserPrefBrightness + 128;
    BYTE B_TempDefaultBrightness;
    //BYTE MaxMin;
    //WORD BriBaseTemp = 0;
    DWORD BriBase;
    //20120605 Modify by KevinChen for input range 0-100
    //gm_Print("UserPrefBrightnes-%d",UserPrefBrightness);
    //MaxMin = MAX_BACKLIGHT_PWM - MIN_BACKLIGHT_PWM; 

	if(IS_UNIFORMITY_Off())//120731 Edward add for Foxconn UC On /Off
	{
		//20120711 Add sRGB,AdobeRGB,Text,6500K BriBase by KevinChen.
	    	if (IsColorSpace_GRAPHICS_SRGB)
	    	{//gm_Print("UserPrefBriBase_sRGB-%d",UserPrefBriBase_sRGB);
	        	BriBase=UserPrefBriBase_sRGB;
	    	}
	    	else if (IsColorSpace_GRAPHICS_ADOBE_RGB)
	    	{//gm_Print("IsColorSpace_GRAPHICS_ADOBE_RGB-%d",UserPrefBriBase_AdobeRGB);
	        	BriBase=UserPrefBriBase_AdobeRGB;
	    	}
			else if(IsPresetMode_GRAPHICS_TEXT)
			{//gm_Print("IsPresetMode_GRAPHICS_TEXT-%d",UserPrefBriBase_Text);
	        	BriBase=UserPrefBriBase_Text;
			}
	    	else
	    	{
	        	BriBase=UserPrefBriBase_6500K;//0x5B;//20120706 Follow Qisda mail request by KevinChen//20120530 KevinChen
	        	//gm_Print("other - %d",BriBase);
	    	}
	}
	else
	{
		if(UserPrefDUMSwitch==UNIFORMITY_Table1)	
			BriBase=UserPrefBriBase_Table1;
		else
			BriBase=UserPrefBriBase_Table2;
	}

	if(BriBase>=MAX_BACKLIGHT_PWM) BriBase=MAX_BACKLIGHT_PWM;//20120719 Add by KevinChen
	if(BriBase<=MIN_BACKLIGHT_PWM) BriBase=MIN_BACKLIGHT_PWM;//20120719 Add by KevinChen

	//W_Pulse = ((DWORD)(MAX_BACKLIGHT_PWM - MIN_BACKLIGHT_PWM) * (UserPrefBrightness)) /(WB_BRIGHTNESS_MAX - WB_BRIGHTNESS_MIN)+ MIN_BACKLIGHT_PWM;
	if(UserPrefBrightness <= WB_BRIGHTNESS_DEFAULT)
		W_Pulse = ((DWORD)(DEF_BACKLIGHT_PWM - MIN_BACKLIGHT_PWM) * (UserPrefBrightness)) /(WB_BRIGHTNESS_DEFAULT - WB_BRIGHTNESS_MIN)+ MIN_BACKLIGHT_PWM;
	else
	{
		#if (WB_BRIGHTNESS_MAX == WB_BRIGHTNESS_DEFAULT)
			W_Pulse = MAX_BACKLIGHT_PWM;
		#else
			W_Pulse = ((DWORD)(MAX_BACKLIGHT_PWM - DEF_BACKLIGHT_PWM + 1) * (UserPrefBrightness - WB_BRIGHTNESS_DEFAULT))/(WB_BRIGHTNESS_MAX - WB_BRIGHTNESS_DEFAULT)+ DEF_BACKLIGHT_PWM;
		#endif
	}

	 if (W_Pulse <= MIN_BACKLIGHT_PWM)
		   W_Pulse = MIN_BACKLIGHT_PWM;
	 if (W_Pulse >= MAX_BACKLIGHT_PWM)
		   W_Pulse = MAX_BACKLIGHT_PWM;


#if BACKLIGHT_PWM_INVERT
			   W_Pulse = MAX_PWMPERIOD - W_Pulse;//Invert W_Pulse Value
		//W_Pulse = MAX_BACKLIGHT_PWM - W_Pulse + MIN_BACKLIGHT_PWM;	//Invert W_Pulse Value
#endif
	 //20120507 Set 1Khz for PWM1,3 by KevinChen
	 //gm_WriteRegWord(BACKLIGHT_PWM_CTRL, MAX_PWMPERIOD |(W_Pulse << 8));
	gm_WriteRegWord(BACKLIGHT_B_PWM_CTRL, (BYTE)MAX_PWMPERIOD |(W_Pulse << 8));
	gm_WriteRegWord(BACKLIGHT_G_PWM_CTRL, (BYTE)MAX_PWMPERIOD |(W_Pulse << 8));

	gm_WriteRegWord(BACKLIGHT_B_PWM_CTRL_MSB, (MAX_PWMPERIOD >> 8) | (W_Pulse & 0x0F00));
	gm_WriteRegWord(BACKLIGHT_G_PWM_CTRL_MSB, (MAX_PWMPERIOD >> 8) | (W_Pulse & 0x0F00));	

#endif
};
#endif


void AdjustZoom(void)	//Neil 20081218* this function will be called by mode setup to calculate the relative registers;
{
	WORD W_Percent;
	WORD W_Dx;
	WORD W_Dy;	

	W_Percent = UserPrefMainImageZoom;

	// Wait for VSync
	#ifdef DELL_U2410F		//Neil 20090527+ for interlace mode adjust zoom image distrotion
		/*if(gm_IsInterlaced(gmvb_MainChannel))
			DynamicScalerStart_ForZoom(gmvb_MainChannel);
		else*/
			DynamicScalerStart(gmvb_MainChannel);
	#else
		DynamicScalerStart(gmvb_MainChannel);
	#endif
	// Restore Input Capture
	//Neil 20081218- just calculate and setup it.
	/*if (W_Percent == 0)
	{
		SetCapture(gmvb_MainChannel);
		SetDisplayMain();
		#ifndef DELL_U2410F
			SetPipBorder(gmvb_MainChannel, TRUE);			//Neil 20081211- border always on Pip
		#endif	
	}
	else		// Zoom
	*/
	{
		WORD W_InHAct, W_InVAct;
		//if(gmvw_InputHActiveOrg[gmvb_MainChannel]<700) //KevinL for HActive 640 zoom 95~100% failed		//Neil 20081111- for not only HAct < 700 image failed;
		{
			W_Percent = (W_Percent * ZOOM_FACTOR)/100;	//Neil 20081218* 480i no image when UserPrefImageZoom = 100
		}
		W_InHAct = gmvw_InputHActiveOrg[gmvb_MainChannel];
		W_InVAct = gmvw_InputVActiveOrg[gmvb_MainChannel];
		
		stInput[gmvb_MainChannel].Hstart = gmvw_InputHStartOrg[gmvb_MainChannel];
		stInput[gmvb_MainChannel].Vstart = gmvw_InputVStartOrg[gmvb_MainChannel];

		//Neil 20081213* if Overscan on, crop it first, otherwise image will zoom out then zoom in
		 if(b_IsInuptOverScan[CH_A] == TRUE)	
		{
			stInput[gmvb_MainChannel].Hactive = (WORD)(W_InHAct - W_InHAct *3/100);		//overscan first
			stInput[gmvb_MainChannel].Vactive = (WORD)(W_InVAct - W_InVAct*3 /100);		//overscan first
			stInput[gmvb_MainChannel].Hstart += W_InHAct *3/200;
			stInput[gmvb_MainChannel].Vstart += W_InVAct *3/200;
                     InputOverScanCrop(gmvb_MainChannel);//KevinL 20081231 for finetune position			
		}
		else
		{
			stInput[gmvb_MainChannel].Hactive = W_InHAct;
			stInput[gmvb_MainChannel].Vactive = W_InVAct;
		}

		W_Dx = (WORD)(((DWORD)W_Percent * W_InHAct) /200);	//Neil 20081111+ for force type convert avoid pricision lost, Should be(DWORD) Bug list 142
		W_Dy = (WORD)(((DWORD)W_Percent * W_InVAct)/200);	//Neil 20081111+ for force type convert avoid pricision lost, Should be(DWORD) Bug list 142

		stInput[gmvb_MainChannel].Hstart += W_Dx;
		stInput[gmvb_MainChannel].Hactive -= (W_Dx * 2);
		stInput[gmvb_MainChannel].Vstart += W_Dy;
		stInput[gmvb_MainChannel].Vactive -= (W_Dy * 2);

	}

	// Reconfig
	if (gmvb_MainChannel == CH_A)
		gm_SetupInputMain();
	else
		gm_SetupInputPip();
	gm_ClearRegBitsWord(IMP_CONTROL, IMP_RUN_EN); //KevinL 20090102	
	gm_ScalerDynamic(gmvb_MainChannel==CH_A?1:2);
	gm_SetRegBitsWord(IMP_CONTROL, IMP_RUN_EN); //KevinL 20090102		
	gm_ScalerDynamicWait(4);	
	DynamicScalerStop();		
}

void CustomHandler(void)
{
	DisplayEnable();
	//SourceScanHandler();			
	LED_Handler();
	FlyingWindowHandler();
	//BurnInHandler();	
	#if(OSD_HARD_LOCK == ENABLE)
	HardLockTimerHandler();		//Neil 20081119- we did in touchpad.c, otherwise will send MTO_HardLock twice so OSD can't lock,Buglist 127
	#endif
	//SplashScreenTimerHandler();
	DiagnosticTestPatternTimerHandler();
//	AudioRequestHandler();
	MonitorOnTimeHandler();	

	#if(PROJECT_SELECT == PROJ_4K2K)
	#ifdef OUTPUT_MODE_CHANGE
	OutputModeHandler_4K2K(gmvb_MainChannel);		//Hyb130730+ 
	#endif
	#endif

#if 1//def INNOLUX_MARS_BOARD 
	#ifdef USB_POWER_CONTROL
	//UsbPowerHandler();
	#endif
	//CableDetectionHandler();
	//PIPMessageHandler();//KevinL 20090506
	#ifdef PIVOT_SENSOR_ENABLE
	RotationSensorHandler();
	#endif
	//FactoryModeAdmitHandler();	
	PipMessageAdmitHandler();		//Neil120712* enable it for PIP message control
	#ifdef CHECK_PxP_STATES_AC_ON
       PowerOnPIPModeHandler();	
	#endif
	#ifdef INPUTSCAN
	//if((B_StoredPipMode == NO_PIP) && (B_MainNoSyncCheckState == STOP_CHECK)) //KevinL 20090401
		//InputScan();
	#endif
	//#ifdef AUDIO_NOISE_REDUCE
	//AudioSourceAutoSelect();	//Hyb130425- may modified base on new library later;
	//#endif
	//CheckACCUpdateStatusWhenOSDShow();		//Neil 20090402+ for OSD broken when PBP press key
	//CVBSForceModeSetupHandler();  //KevinL 20090415	
	#ifdef PIP_POS_OSD_ACC_PATCH //KevinL 20090513
 	//AccUpdatedCheckHandler(); 
	#endif
#endif		
	NoCablePowerDownHandler();	     

	//EnergySmartHandler();			//Hyb130303-
	//EnergyUsedLevelHandler();		//Hyb130303-
	#ifdef PORT_CHANGE_SEAMLESS
	PortChangeSeamlessHandler();
	#endif
        AdjustPanelHStart(CH_A);  //wei 140530
        
	//Hyb140304+ for BKG color check;
	CheckNoSyncBKGColor(gmvb_MainChannel);
	
#ifdef USE_TIMER_CTRL_AUD_PWR	//Neil120804* CMI follow U2410 routine//def Qisda_93XX_BOARD//20120518 Add by KevinChen for audio power control.
	AudioSpeakerPowerCheck();	
#endif
}

void UsbDetectControl(void)
{
	//static WORD UsbLoopCount = 0;
	WORD msg_data[3];

    	//if ((UsbLoopCount++) > 200)
	{
		if(gm_MessageReceived(gmd_MESSAGE_LPM_GPIO_CTRL, &msg_data, sizeof(msg_data)))
		{
			if((msg_data[1] == GPOUTPUT1) && (msg_data[2] == (GPIO31_OUT|GPIO32_OUT)))
			{
				if(msg_data[0] == Set_bit)			//USB Insert
				{
					//gm_Print("$$ USB Insert",0);
					gm_SetRegBitsWord(GPOUTPUT1,GPIO31_OUT);	//DET_CTRL
					gm_SetRegBitsWord(GPOUTPUT2,GPIO32_OUT);	//GPO1			
				}
				else if(msg_data[0] == Clear_bit)	//USB non-Insert
				{
					//gm_Print("$$ USB non-Insert");
					gm_ClearRegBitsWord(GPOUTPUT1,GPIO31_OUT);	//DET_CTRL
					gm_ClearRegBitsWord(GPOUTPUT2,GPIO32_OUT);	//GPO1
				}
				else
				{
					gm_SetRegBitsWord(GPOUTPUT1,GPIO31_OUT);	//DET_CTRL
					gm_SetRegBitsWord(GPOUTPUT2,GPIO32_OUT);	//GPO1
				}
			}
		//UsbLoopCount = 0;
		}
	}
}

#if ((DEFAULT_PANEL == LG_WHD_LM290WW1_SSA1)||(DEFAULT_PANEL == LG_WQXGA_LM300WQ5_SLA1))
void AdjustBackLightBase(void)
{
	WORD BriBase;

	if (IsColorSpace_GRAPHICS_SRGB)
	{//gm_Print("UserPrefBriBase_sRGB-0x%x",UserPrefBriBase_sRGB);
		BriBase=UserPrefBriBase_sRGB;
	}
	else if (IsColorSpace_GRAPHICS_ADOBE_RGB)
	{//gm_Print("IsColorSpace_GRAPHICS_ADOBE_RGB-0x%x",UserPrefBriBase_AdobeRGB);
		BriBase=UserPrefBriBase_AdobeRGB;
	}
	else if(IsPresetMode_GRAPHICS_TEXT)
	{//gm_Print("IsPresetMode_GRAPHICS_TEXT-0x%x",UserPrefBriBase_Text);
 		BriBase=UserPrefBriBase_Text;
	}
	else
	{//gm_Print("Other - 0x%x",UserPrefBriBase_6500K);
		BriBase=UserPrefBriBase_6500K;//0x5B;//20120706 Follow Qisda mail request by KevinChen//20120530 KevinChen
	}

	if(BriBase>= MAX_BACKLIGHT_PWM) BriBase= MAX_BACKLIGHT_PWM;//20120719 Add by KevinChen
	if(BriBase<= MIN_BACKLIGHT_PWM) BriBase= MIN_BACKLIGHT_PWM;//20120719 Add by KevinChen



	 #if BACKLIGHT_PWM_INVERT
          BriBase  =0xFF-BriBase;//Invert W_Pulse Value
	//BriBase = MAX_BACKLIGHT_PWM - BriBase;	//Invert W_Pulse Value
	#endif
	///////////////////////////////////////////////////////////
	//gm_Print("BriBase = 0x%x",BriBase);
	
     //20120507 Set 1Khz for PWM1,3 by KevinChen
     //gm_WriteRegWord(BACKLIGHT_PWM_CTRL, MAX_PWMPERIOD |(BriBase << 8));
     gm_WriteRegWord(BACKLIGHT_B_PWM_CTRL, (BYTE)MAX_PWMPERIOD |(BriBase << 8));
     gm_WriteRegWord(BACKLIGHT_G_PWM_CTRL, (BYTE)MAX_PWMPERIOD |(BriBase << 8));
     gm_WriteRegWord(BACKLIGHT_B_PWM_CTRL_MSB, (MAX_PWMPERIOD >> 8) | (BriBase & 0xFF00));
     gm_WriteRegWord(BACKLIGHT_G_PWM_CTRL_MSB, (MAX_PWMPERIOD >> 8) | (BriBase & 0xFF00)); 
}
#endif

BYTE IsLockOSD(void)
{
	#if (OSD_HARD_LOCK == ENABLE)
	return (UserPrefOSDLock);
	#else
	return gmd_FALSE;
	#endif
}

void LockOSD(void)
{
#if (OSD_HARD_LOCK == ENABLE)
	UserPrefOSDLock = OSD_LOCK;
#endif
};

void UnLockOSD(void)
{
#if (OSD_HARD_LOCK == ENABLE)
	UserPrefOSDLock = OSD_UNLOCK;
#endif
};

BYTE IsOSDHardLock(void)
{
	#if (OSD_HARD_LOCK == ENABLE)
	return (UserPrefOSDHardLock);
	#else
	return gmd_FALSE;
	#endif
}

void OSDHardLock(void)
{
	#if (OSD_HARD_LOCK == ENABLE)
	UserPrefOSDHardLock = OSD_LOCK;
	SaveModeIndependentSettings();
	#endif
}

void OSDHardUnLock(void)
{
	#if (OSD_HARD_LOCK == ENABLE)
	UserPrefOSDHardLock = OSD_UNLOCK;
	SaveModeIndependentSettings();
	#endif
}

#define FlyingMessagesMainTile_ID 13
#define FlyingMessagesTile_ID 14 
#if (FLY_WIN_RANDOM == ENABLE)
#define STEP_X_Max			6
#define STEP_Y_Max			5
#endif
void FlyingWindowHandler(void)
{
	if (!Flying)
		return;
	else
	{
		static BYTE direction = UP|LEFT;
		static BYTE speedX    = 1;
		static BYTE speedY    = 2;
		#if (FLY_WIN_RANDOM == ENABLE)
		static BYTE X_Random = 1;
		static BYTE Y_Random = 2;	
		#endif
		
		DWORD CurrentOnTime = gm_GetSystemTime();		/// The time started from System ON (ms)
		BYTE decTimer ;

			//gm_Print ("-----BEGIN FLYING----",1);

		decTimer = (BYTE)((DWORD)labs(CurrentOnTime - prevOnTime)/(DWORD)(640L));//(64L));	// Checking for 1 min
		if (!decTimer)
			return;		
		prevOnTime = CurrentOnTime;

		if(UserPrefOsdRotation == OSD_ROTATED_180) //wei 140718  OSD_ROTATED_90   KevinL 20090205 
		{
			#if (FLY_WIN_RANDOM == ENABLE)
			speedX = X_Random;	
			speedY = Y_Random;	
			#else
			speedX = 1;	
			speedY = 2;	
			#endif
		}
		else	
		{
			#if (FLY_WIN_RANDOM == ENABLE)
			speedX = X_Random;	
			speedY = Y_Random;
			#else
			speedX = 1;	
			speedY = 2;	
			#endif
		}
		if (direction & UP)
		{
			if(UserPrefOsdRotation == OSD_ROTATED_180) //KevinL 20090205 
			{
			    if (UserPrefOsdVPosFloat>MIN_VER_PORTRAIT)
			    {
				if (UserPrefOsdVPosFloat >= (speedY + MIN_VER_PORTRAIT))
					UserPrefOsdVPosFloat -= speedY;
				else
					UserPrefOsdVPosFloat = MIN_VER_PORTRAIT;  // wei 140627   0;	
			    }
			    else
			    {
					#if (FLY_WIN_RANDOM == ENABLE)
					X_Random = Random_BYTE(STEP_X_Max); 
					Y_Random = Random_BYTE(STEP_Y_Max); 		
					if((X_Random == 0)&&(Y_Random == 0))
					{
						X_Random = 1;
						Y_Random = 1;
					}
					#endif
				
					direction &= ~UP;
			    }
			}
			else
			{
			    if (UserPrefOsdVPosFloat > MIN_VER)   //wei 140630
			    {
				if (UserPrefOsdVPosFloat >= (MIN_VER + speedY))
					UserPrefOsdVPosFloat -= speedY;
				else
					UserPrefOsdVPosFloat = MIN_VER;  // wei 140627   0;	
			    }
			    else
			    {
				#if (FLY_WIN_RANDOM == ENABLE)
				X_Random = Random_BYTE(STEP_X_Max); 
				Y_Random = Random_BYTE(STEP_Y_Max); 	
				if((X_Random == 0)&&(Y_Random == 0))
				{
					X_Random = 1;
					Y_Random = 1;
				}
				#endif
				
				direction &= ~UP;
			    }
			}
		}
		else
		{		
			if (UserPrefOsdRotation == OSD_ROTATED_180) //KevinL 20090205
			{
				if (UserPrefOsdVPosFloat < MAX_VER_PORTRAIT)
				{
					if (UserPrefOsdVPosFloat <= (MAX_VER_PORTRAIT - speedY))
						UserPrefOsdVPosFloat += speedY;
					else  
						UserPrefOsdVPosFloat = MAX_VER_PORTRAIT;
				}
				else
				{

					#if (FLY_WIN_RANDOM == ENABLE)
					X_Random = Random_BYTE(STEP_X_Max); 
					Y_Random = Random_BYTE(STEP_Y_Max); 	
					if((X_Random == 0)&&(Y_Random == 0))
					{
						X_Random = 1;
						Y_Random = 1;
					}
					#endif
					
					direction |= UP;
				}
			}
			else
			{
				if (UserPrefOsdVPosFloat < MAX_VER)
				{
					if (UserPrefOsdVPosFloat <= (MAX_VER - speedY))
						UserPrefOsdVPosFloat += speedY;
					else  
						UserPrefOsdVPosFloat = MAX_VER;
				}
				else
				{
					#if (FLY_WIN_RANDOM == ENABLE)
					X_Random = Random_BYTE(STEP_X_Max); 
					Y_Random = Random_BYTE(STEP_Y_Max); 	
					if((X_Random == 0)&&(Y_Random == 0))
					{
						X_Random = 1;
						Y_Random = 1;
					}
					#endif
				
					direction |= UP;
				}
			}
		}
		if (direction & LEFT)
		{
			if (UserPrefOsdRotation == OSD_ROTATED_180) //KevinL 20090311
			{
				if (UserPrefOsdHPosFloat > (MIN_HOR_PORTRAIT))  ///wei 140627   2)
				{
					if (UserPrefOsdHPosFloat >= (MIN_HOR_PORTRAIT + speedX))
						UserPrefOsdHPosFloat -= speedX;
					else
						UserPrefOsdHPosFloat = MIN_HOR_PORTRAIT;  //wei 140627   0;	
				}
				else
				{
					#if (FLY_WIN_RANDOM == ENABLE)
					X_Random = Random_BYTE(STEP_X_Max); 
					Y_Random = Random_BYTE(STEP_Y_Max); 	
					if((X_Random == 0)&&(Y_Random == 0))
					{
						X_Random = 1;
						Y_Random = 1;
					}
					#endif
					
					direction &= ~LEFT;
				}
			}
			else
			{
				if (UserPrefOsdHPosFloat>MIN_HOR)
				{
					if (UserPrefOsdHPosFloat >= (MIN_HOR+speedX))  //wei 140627
						UserPrefOsdHPosFloat -= speedX;
					else
						UserPrefOsdHPosFloat = MIN_HOR;  //0;	
				}
				else
				{
					#if (FLY_WIN_RANDOM == ENABLE)
					X_Random = Random_BYTE(STEP_X_Max); 
					Y_Random = Random_BYTE(STEP_Y_Max); 	
					if((X_Random == 0)&&(Y_Random == 0))
					{
						X_Random = 1;
						Y_Random = 1;
					}
					#endif
					
					direction &= ~LEFT;
				}
			}				
		}
		else
		{
		        if (UserPrefOsdRotation == OSD_ROTATED_180)  //wei 140718
		        {
				if (UserPrefOsdHPosFloat < (MAX_HOR_PORTRAIT))  ///wei 140627   2)
				{
					if (UserPrefOsdHPosFloat <= (MAX_HOR_PORTRAIT -speedX))
						UserPrefOsdHPosFloat += speedX;
					else
						UserPrefOsdHPosFloat = MAX_HOR_PORTRAIT;  //wei 140627   0;	
				}
				else
				{
					#if (FLY_WIN_RANDOM == ENABLE)
					X_Random = Random_BYTE(STEP_X_Max); 
					Y_Random = Random_BYTE(STEP_Y_Max); 	
					if((X_Random == 0)&&(Y_Random == 0))
					{
						X_Random = 1;
						Y_Random = 1;
					}
					#endif
					
					direction |= LEFT;
				}
			}
			else
			{
			       if (UserPrefOsdHPosFloat < MAX_HOR)
			       {
				        if (UserPrefOsdHPosFloat <= (MAX_HOR - speedX))
					        UserPrefOsdHPosFloat += speedX;
				        else
					        UserPrefOsdHPosFloat = MAX_HOR;
			       }
			       else
			       {
				        #if (FLY_WIN_RANDOM == ENABLE)
				         X_Random = Random_BYTE(STEP_X_Max); 
				         Y_Random = Random_BYTE(STEP_Y_Max); 	
				         if((X_Random == 0)&&(Y_Random == 0))
				         {
					          X_Random = 1;
					          Y_Random = 1;
				          }
				         #endif
				
				        direction |= LEFT;
			         }
			}
		}
                #if 0
		if (UserPrefOsdRotation == OSD_ROTATED_180) //KevinL 20090205 
		{
			gmv_OsdTilePosition[FlyingMessagesMainTile_ID].W_YPos = ROTATED_FLY_YPOSITION;
			gmv_OsdTilePosition[FlyingMessagesTile_ID].W_YPos = ROTATED_FLY_YPOSITION;
		}
		#endif
		//AdjustOsdHPosition();
		AdjustFlyingOsdPosition();
		if ((gm_ReadRegWord(OSD_CONTROL)& OSD_ENABLE) == 0) // JP 20070702 patch for OSD disappear after AC on in DVI mode
			gm_SetRegBitsWord(OSD_CONTROL, OSD_ENABLE);
	}
}

void StartFlyingWindow(void)
{
	#if(FLYING_WINDOW == DISABLE)
	#ifdef UART_DEBUG_MODE
	gm_Print(" =======>StartFlyingWindow: No Flying ======",0);
	#endif
	//StoreOsdHPosition = UserPrefOsdHPosition;
	//StoreOsdVPosition = UserPrefOsdVPosition;
	
	UserPrefOsdHPosition = 500; // 345; // 500; 
	UserPrefOsdVPosition = 460; // 480; // 500;
	AdjustOsdHPosition();	
	Flying = 0;
	if ((gm_ReadRegWord(OSD_CONTROL)& OSD_ENABLE) == 0) // JP 20070702 patch for OSD disappear after AC on in DVI mode
		gm_SetRegBitsWord(OSD_CONTROL, OSD_ENABLE);
	
	return;
	#else
	Flying = 1;
	//StoreOsdHPosition = UserPrefOsdHPosition;
	//StoreOsdVPosition = UserPrefOsdVPosition;
	prevOnTime = gm_GetSystemTime();
	if(UserPrefOsdRotation == OSD_ROTATED_90) //KevinL 20090205 
	{
		gmv_OsdTilePosition[FlyingMessagesMainTile_ID].W_YPos = ROTATED_FLY_YPOSITION;
		gmv_OsdTilePosition[FlyingMessagesTile_ID].W_YPos = ROTATED_FLY_YPOSITION;
	}

	if(UserPrefOsdRotation == OSD_ROTATED_180) //KevinL 20090205 
	{
		UserPrefOsdHPosFloat = 240; // 345; // 500; 
		UserPrefOsdVPosFloat = 540; // 480; // 500;
	}
	else
	{
		UserPrefOsdHPosFloat = 500; // 345; // 500; 
		UserPrefOsdVPosFloat = 500; // 480; // 500;
	}
	AdjustFlyingOsdHPosition();
	#ifdef UART_DEBUG_MODE
	gm_Print("========StartFlyingWindow: Flying==========",0);
	#endif
	#endif
} 

BOOL IsOSDFlying(void)
{
	return Flying;	
}

void StopFlyingWindow(void)
{
	Flying = 0;

	UserPrefOsdHPosFloat = 500;
	UserPrefOsdVPosFloat = 500;

	/*
	if(UserPrefOsdRotation == OSD_ROTATED_180) //KevinL 20090205 
	{
		UserPrefOsdHPosition = 500; // 345; // 500; 
		UserPrefOsdVPosition = 500; // 480; // 500;
	}
	else
	{
		UserPrefOsdHPosition = StoreOsdHPosition;
		UserPrefOsdVPosition = StoreOsdVPosition;
	}
	*/
	AdjustFlyingOsdHPosition();
	#ifdef UART_DEBUG_MODE
	gm_Print("StopFlyingWindow=========",0);
	#endif
}

BYTE IsMacMode(void)
{
	return (UserPrefGammaMode == Gamma_MAC);
}

BYTE IsPCMode(void)
{
	return (UserPrefGammaMode == Gamma_PC);
}


//BYTE DynamicText[7] ={0};   
//&*&*&*S 20120328:SWRD4002: Kordonm(0)  DDC2Bi Dynamic String Test
#if 0
void DrawDynamicText(void)
{	// GM5766 Version GNSS
	// 2GQ001T
	// 2: proto 2
	// 1: GNSS
	// B: BenQ
	// 0: Code Base
	// 01: FW version
	// T: Scaler
	DynamicText[0] = 'Q';
	DynamicText[1] = '1';
	DynamicText[2] = 'B';
	DynamicText[3] = '1';
	DynamicText[4] = '0';
	DynamicText[5] = '4';
	DynamicText[6] = 'T';

//	DrawDynamicTextControl(TEXT_COLOR_STATE);
}
#endif
//&*&*&*E 20120328 : Kordonm(1)  DDC2Bi Dynamic String Test

BYTE IsMainVideoInput(void)
{
	//gm_Print("UserPrefCurrentInputMain=%d",UserPrefCurrentInputMain);

	return ((UserPrefCurrentInputMain == ALL_BOARDS_CVBS1) || (UserPrefCurrentInputMain == ALL_BOARDS_CVBS2) ||
		   (UserPrefCurrentInputMain == ALL_BOARDS_COMP1) || (UserPrefCurrentInputMain == ALL_BOARDS_COMP2));
	
}

BOOL IsMain1920x1200(void)
{
	if((gmvw_InputVActiveOrg[gmvb_MainChannel] == 1200) && (gmvw_InputHActiveOrg[gmvb_MainChannel] == 1920))
		return TRUE;
	
	return FALSE;
}

//110818 for OSD Optimize Resolution check
BOOL IsMain2560x1600(void)
{
	if((gmvw_InputVActiveOrg[gmvb_MainChannel] == 1600) && (gmvw_InputHActiveOrg[gmvb_MainChannel] == 2560))
		return TRUE;
	
	return FALSE;
}


BOOL IsMain2560x1440(void)//owen 20120614 for OSD Optimize Resolution check
{
	if((gmvw_InputVActiveOrg[gmvb_MainChannel] == 1440) && (gmvw_InputHActiveOrg[gmvb_MainChannel] == 2560))
		return TRUE;
	
	return FALSE;
}
BOOL IsMain2560x1080(void)//owen 20120614 for OSD Optimize Resolution check
{
	if((gmvw_InputVActiveOrg[gmvb_MainChannel] == 1080) && (gmvw_InputHActiveOrg[gmvb_MainChannel] == 2560))
		return TRUE;
	
	return FALSE;
}


BYTE IsLcdOn(void)
{
	return (gm_ReadRegWord(DISPLAY_CONTROL)&DTG_RUN_EN);
//	return TRUE;
//	return (gm_ReadRegWord(OP_ENABLE) & POWER_SEQ_EN);
}

BOOL IsCableConnected_Main (void)
{
#ifdef CHECK_IS_CABLE_CONNECT	
       if(b_IsInputScanProcessingFlag == TRUE) //KevinL 20090413 keep original input setting for OSD when input scan is precessing
		return isCableConnected(OriginalInput); 	   	
	else   	
		return isCableConnected(UserPrefCurrentInputMain); 
#else
	return TRUE;
#endif	
}

BYTE IsVGA(void)
{
	return IsMainVGAInput();
}

BYTE IsDVI(void)
{
	BYTE B_Result = FALSE;

	if ((UserPrefCurrentInputMain == ALL_BOARDS_DVI1) ||
		(UserPrefCurrentInputMain == ALL_BOARDS_DVI2))
		B_Result = TRUE;

	return B_Result;
}

BOOL IsOSD_DCR_Or_NR_On(void)
{
    if((!Is_OSD_NR_ON()&& IS_OSD_DCR_ENABLE()) ||(Is_OSD_NR_ON() && !IS_OSD_DCR_ENABLE()))
	return TRUE;
    else
	return FALSE;	
}


BOOL IsAnalogInput(gmt_PHY_CH B_Channel)
{
	if (B_Channel == CH_A)
		return (UserPrefCurrentInputMain== ALL_BOARDS_VGA1);
	else
		return (UserPrefCurrentInputPip== ALL_BOARDS_VGA1);		
}

//&*&*&*S 20120315:SWRD4001: Kordonm(0)  Modify for OSD Workbench
BOOL IsDigitalInput(gmt_PHY_CH B_Channel)
{
       if (B_Channel == CH_A)
       {
       	if ((UserPrefCurrentInputMain == ALL_BOARDS_HDMI) || (UserPrefCurrentInputMain == ALL_BOARDS_DVI1) ||
			(UserPrefCurrentInputMain == ALL_BOARDS_DVI2) || (UserPrefCurrentInputMain == ALL_BOARDS_DP1) || (UserPrefCurrentInputMain == ALL_BOARDS_DP3))
      			return TRUE;
		else
      			return FALSE;			
       }
	else
	{
       	if ((UserPrefCurrentInputPip == ALL_BOARDS_HDMI) || (UserPrefCurrentInputPip == ALL_BOARDS_DVI1) || 
			(UserPrefCurrentInputPip == ALL_BOARDS_DVI2)|| (UserPrefCurrentInputPip == ALL_BOARDS_DP1) || (UserPrefCurrentInputPip == ALL_BOARDS_DP3))
      			return TRUE;
		else
      			return FALSE;		
	}
}

BOOL IsDPInput(gmt_PHY_CH B_Channel) 
{
       if(B_Channel == gmvb_MainChannel)
		return ((UserPrefCurrentInputMain == ALL_BOARDS_DP1)||(UserPrefCurrentInputMain == ALL_BOARDS_DP3));
	else
		return ((UserPrefCurrentInputPip == ALL_BOARDS_DP1)||(UserPrefCurrentInputPip == ALL_BOARDS_DP3));		
}

BOOL IsDIPInput(gmt_PHY_CH B_Channel) 
{
	
	#ifdef EXTERNAL_DECODER
	if(B_Channel == gmvb_MainChannel)
		return (UserPrefCurrentInputMain == ALL_BOARDS_DIP1);
	else
		return (UserPrefCurrentInputPip == ALL_BOARDS_DIP1);	
	#else
		UNUSED_VAR(B_Channel);
		return gmd_FALSE;
	#endif
}


BOOL IsMenuLandscape(void)
{
     return (UserPrefOsdRotation == OSD_NON_ROTATED);
}
BOOL IsMainVGAInput(void)
{
	return (UserPrefCurrentInputMain == ALL_BOARDS_VGA1);
}

BOOL IsPIPVGAInput(void)
{
	return (UserPrefCurrentInputPip== ALL_BOARDS_VGA1);
}

BOOL IsMainDVID1Input(void)
{
	return (UserPrefCurrentInputMain == ALL_BOARDS_DVI1);
}
BOOL IsPIPDVID1Input(void)
{
      return (UserPrefCurrentInputPip == ALL_BOARDS_DVI1);		
}

BOOL IsMainDVID2Input(void)
{
	return (UserPrefCurrentInputMain == ALL_BOARDS_DVI2);		
}
BOOL IsPIPDVID2Input(void)
{
      return (UserPrefCurrentInputPip == ALL_BOARDS_DVI2);		
}

BOOL IsMainHDMIInput(void)
{
	return (UserPrefCurrentInputMain == ALL_BOARDS_HDMI);		
}
BOOL IsPIPHDMIInput(void)
{
      return (UserPrefCurrentInputPip == ALL_BOARDS_HDMI);		
}

BOOL IsMainDP1Input(void)
{
	return (UserPrefCurrentInputMain == ALL_BOARDS_DP1);		
}
BOOL IsPIPDP1Input(void)
{
      return (UserPrefCurrentInputPip == ALL_BOARDS_DP1);		
}

BOOL IsMainDP3Input(void)
{
	return (UserPrefCurrentInputMain == ALL_BOARDS_DP3);		
}
BOOL IsPIPDP3Input(void)
{
      return (UserPrefCurrentInputPip == ALL_BOARDS_DP3);		
}
BOOL IsMainCOMPInput(void)
{
      return ((UserPrefCurrentInputMain == ALL_BOARDS_COMP1)||(UserPrefCurrentInputMain == ALL_BOARDS_COMP2));		
}
BOOL IsPIPCOMPInput(void)
{
      return ((UserPrefCurrentInputPip == ALL_BOARDS_COMP1)||(UserPrefCurrentInputPip == ALL_BOARDS_COMP2));		
}

BOOL IsMainCOMP1Input(void)
{
      return (UserPrefCurrentInputMain == ALL_BOARDS_COMP1);		
}

BOOL IsPIPCOMP1Input(void)
{
      return (UserPrefCurrentInputPip == ALL_BOARDS_COMP1);		
}

BOOL IsMainCOMP2Input(void)
{
      return (UserPrefCurrentInputMain == ALL_BOARDS_COMP2);		
}

BOOL IsPIPCOMP2Input(void)
{
      return (UserPrefCurrentInputPip == ALL_BOARDS_COMP2);		
}

BOOL IsMainCVBSInput(void)
{
      return ((UserPrefCurrentInputMain == ALL_BOARDS_CVBS1)||(UserPrefCurrentInputMain == ALL_BOARDS_CVBS2));		
}

BOOL IsPIPCVBSInput(void)
{
      return ((UserPrefCurrentInputPip == ALL_BOARDS_CVBS1)||(UserPrefCurrentInputPip == ALL_BOARDS_CVBS2));		
}

BOOL IsMainCVBS1Input(void)
{
      return (UserPrefCurrentInputMain == ALL_BOARDS_CVBS1);		
}

BOOL IsPIPCVBS1Input(void)
{
      return (UserPrefCurrentInputPip == ALL_BOARDS_CVBS1);		
}

BOOL IsMainCVBS2Input(void)
{
      return (UserPrefCurrentInputMain == ALL_BOARDS_CVBS2);		
}

BOOL IsPIPCVBS2Input(void)
{
      return (UserPrefCurrentInputPip == ALL_BOARDS_CVBS2);		
}


BOOL IsMainDIPInput(void)
{
      return (UserPrefCurrentInputMain == ALL_BOARDS_DIP1);		
}

BOOL IsPIPDIPInput(void)
{
      return (UserPrefCurrentInputPip == ALL_BOARDS_DIP1);		
}

BOOL IsMainDIPInput_D1(void)
{
      return (UserPrefCurrentInputMainDIP == DEC_1Ch_Sel_D1);		
}

BOOL IsMainDIPInput_D2(void)
{
      return (UserPrefCurrentInputMainDIP == DEC_1Ch_Sel_D2);		
}

BOOL IsMainDIPInput_D3(void)
{
      return (UserPrefCurrentInputMainDIP == DEC_1Ch_Sel_D3);		
}

BOOL IsMainDIPInput_D4(void)
{
      return (UserPrefCurrentInputMainDIP == DEC_1Ch_Sel_D4);		
}

BOOL IsPxpDIPInput_D1(void)
{
      return (UserPrefCurrentInputPxpDIP == DEC_1Ch_Sel_D1);		
}

BOOL IsPxpDIPInput_D2(void)
{
      return (UserPrefCurrentInputPxpDIP == DEC_1Ch_Sel_D2);		
}

BOOL IsPxpDIPInput_D3(void)
{
      return (UserPrefCurrentInputPxpDIP == DEC_1Ch_Sel_D3);		
}

BOOL IsPxpDIPInput_D4(void)
{
      return (UserPrefCurrentInputPxpDIP == DEC_1Ch_Sel_D4);		
}

BOOL IsMainDigitalInput(void)
{
	return ((UserPrefCurrentInputMain == ALL_BOARDS_DVI1)||(UserPrefCurrentInputMain == ALL_BOARDS_HDMI)
		||(UserPrefCurrentInputMain == ALL_BOARDS_DP1));		
}


BOOL IsDPVideoMode(gmt_PHY_CH B_Channel)
{
    if (((stInput[B_Channel].ModeFlags & gmd_MODE_SIGNAL_MASK) > gmd_MODE_GRAPHICS) ||((stInput[B_Channel].ModeFlags & gmd_MODE_INTERLACED) == gmd_MODE_INTERLACED))       
    {
        //gm_Print("DP SD/ED/HD Video mode",0);
	 return TRUE;
    }
    else
    {
        //gm_Print("DP non-Video mode",0);    
	 return FALSE;	
    }
}

BOOL IsDPColorSpaceRGB(void)
{
	BYTE B_Current_ColorSpace;
	B_Current_ColorSpace = (gm_ReadRegByte(DP12RX0_MS_FORMAT) & DPRX_MS_COLOR_SPACE) >> 1;

	if (B_Current_ColorSpace == ColorSpace_RGB)
	{
		msg_i("DP ColorSpace -- RGB",0);   
		return TRUE;	   
	}
	else if (B_Current_ColorSpace == ColorSpace_YCbCr422)
	{
		msg_i("DP ColorSpace -- YCbCr422",0);   
		return FALSE;		
	}
	else if (B_Current_ColorSpace == ColorSpace_YCbCr444)
	{
		msg_i("DP ColorSpace -- YCbCr444",0);   
		return FALSE;		
	}
	else	
	{
		msg_i("DP ColorSpace -- UNKNOW",0);   
		return FALSE;			  
	}
}

BOOL IsHDMIColorSpaceRGB(gmt_PHY_CH B_Channel)
{
	HDMI_AVI_Y B_HdmiAviY = gm_HdmiGetAviColorSpace(gB_CurrentInputConnector[B_Channel]);

	if (B_HdmiAviY == HDMI_AVI_Y_RGB)	
	{
		msg_i("HDMI AVIInfo -- RGB", 0);
		return TRUE;
	}
	else if (B_HdmiAviY == HDMI_AVI_Y_YUV422)
	{
		msg_i("HDMI AVIInfo -- YUV422", 0);
		return FALSE;	   	
	}	
	else if (B_HdmiAviY == HDMI_AVI_Y_YUV444)
	{
		msg_i("HDMI AVIInfo -- YUV444", 0);
		return FALSE;
	}	
	else	   
	{
		msg_i("HDMI AVIInfo -- UNKNOW", 0);
		return FALSE;
	}		
}


BOOL IsCableConnected_Pip (void)
{
	return isCableConnected(UserPrefCurrentInputPip); 
}

BOOL IsCableConnected_Audio(void)
{
	return TRUE;
}



BOOL IsSplashScreenCompleted(void)
{
	return B_SplashComplete;
}


BOOL IsDisplayPortBoard(void)
{
	return TRUE;
}


BOOL IsInputColorFormatDisable (void)//KevinL 20081114 updated Color format do not need to disable
{
 if ((UserPrefColorMode == InputColorFormat_YUV)||(B_Force_ColorFormat == TRUE)|| (UserPrefCurrentInputMain == ALL_BOARDS_CVBS1)
 	||(UserPrefCurrentInputMain == ALL_BOARDS_CVBS2))
    return TRUE;
 return FALSE;
}

BOOL IsGammaDisable (void)
{
#ifdef DELL_U2410F
	if ((UserPrefCurrentInputMain == ALL_BOARDS_COMP1)||(UserPrefCurrentInputMain == ALL_BOARDS_COMP2)
		||(UserPrefCurrentInputMain == ALL_BOARDS_CVBS1)||(UserPrefCurrentInputMain == ALL_BOARDS_CVBS2))
#else
	if (((UserPrefCurrentInputMain == ALL_BOARDS_CVBS1) || (UserPrefCurrentInputMain == ALL_BOARDS_COMP1))	 ||
	     (!OSD_IsGraphics))	
#endif	     
		return 1;
	return 0;
}

BOOL IsModeSelectDisable (void)
{
#ifdef DELL_U2410F
       if(IsVideoInput(UserPrefCurrentInputMain)) //KevinL 20081223
		return TRUE;
	else   
       	return FALSE;
#else
	if(UserPrefCurrentInputMain == ALL_BOARDS_COMP1)
		return 1;
	if ((B_Force_ColorMode== TRUE) || (UserPrefCurrentInputMain == ALL_BOARDS_CVBS1))
		return 1;
	return 0;
#endif	
}

BOOL IsWideMode_1_1_Disable(void) //KevinL 20090115
{	  
	return ((gmvw_InputHActiveOrg[gmvb_MainChannel] > PanelWidth) || (gmvw_InputVActiveOrg[gmvb_MainChannel] > PanelHeight)) ;	
}

/*BOOL IsHardLockTimerRunning (void)
{
	return FALSE;
}*/

void AdjustPipHStart (void) {};
void AdjustPipVStart (void) {};
BOOL AudioDetect(void)
{
	#if (BOARD_USED == DELL_93xx_BOARD)
	if (IsAUDIO_Detected())
	{
		//Neil120620+ for audio power control
		if(UserPrefPwrAudio == OFF)
			B_UserSettingFlag |= Flag_AudioDetect;		//Don't clear PSV if 
		else
			B_UserSettingFlag &= ~Flag_AudioDetect;		//clear PSV if Pwr audio on
			
		SendMissionFlagToLPM();

               if(UserPrefSundBox_OSD_False == 1)//owen 120919
               	{
               	        OSDRequestMsg(gmvb_MainChannel, MTO_SundBox_OSD_Eanble, REQUEST_NOW);
			UserPrefSundBox_OSD_False = 0;			
               	}   
            	UserPrefSundBox_OSD_True=1;
		return TRUE;	
	}
	else
	{
		B_UserSettingFlag &= ~Flag_AudioDetect;
		SendMissionFlagToLPM();
		 if(UserPrefSundBox_OSD_True==1)//owen 120919 
		 {
			OSDRequestMsg(gmvb_MainChannel, MTO_SundBox_OSD_Disable, REQUEST_NOW);
			UserPrefSundBox_OSD_True = 0;	
		 }
		UserPrefSundBox_OSD_False = 1;
			
		return FALSE;
	}
	#else
	return gmd_TRUE;
	#endif
}

extern gmt_PwrStateType PwrState;
/*
void SetOsdReadyForPowerDown (void)
{
//	gm_Print("~~~~ SetOsdReadyForPowerDown ~~~~", 0);
	OSDRequestCancelMsg(gmvb_MainChannel, REQ_ANY);
	OSDRequestCancelMsg(gmvb_PipChannel, REQ_ANY);	

#ifdef DELL_U2410F	
	gmvb_OsdReadyForPowerDown = TRUE;
#endif
	SetAudioDACOutput(OFF);		//Neil 20090312+
	if ((UserPrefPwrAudio == ON) && AudioDetect()) //KevinL 20090225 for Power Save Audio function	
	{
		//gm_Print("~~~~ SetOsdReadyForPowerDown ~~AudioDetect() = %d~~", AudioDetect());
		PowerDownAudioSpeaker();		//Neil 20090217+
		gm_Delay100ms(6);			//if enter power saving, need more delay for audio pop noise
	}

	if (PwrState != Pwr_Sleep)
	{
		PwrState = Pwr_Sleep;
	}
}

void SetOsdRequestForPowerUp(void) 
{
   if(PwrState == Pwr_DeepSleep) //KevinL 20090505
      PowerDeepSleepOutRequest();
}*/

//&*&*&*S 20120315:SWRD4001: Kordonm(0)  Modify for OSD Workbench
BYTE PxPSourceIsGraphic (void) 
{
	if ((UserPrefPipMode != NO_PIP) &&  ((UserPrefCurrentInputPip  == ALL_BOARDS_VGA1) || (UserPrefCurrentInputPip  == ALL_BOARDS_HDMI) || 
		(UserPrefCurrentInputPip  == ALL_BOARDS_DVI1) || (UserPrefCurrentInputPip  == ALL_BOARDS_DVI2) || (UserPrefCurrentInputPip  == ALL_BOARDS_DP1) || (UserPrefCurrentInputPip  == ALL_BOARDS_DP3)))
		return 1;
	return 0;
}
//&*&*&*E 20120315 : Kordonm(1)  Modify for OSD Workbench

BYTE PxPSourceIsVideo (void) 
{
	if ((UserPrefPipMode != NO_PIP) &&  ((UserPrefCurrentInputPip  == ALL_BOARDS_CVBS1) || (UserPrefCurrentInputPip  == ALL_BOARDS_COMP1)
		||(UserPrefCurrentInputPip  == ALL_BOARDS_CVBS2)||(UserPrefCurrentInputPip  == ALL_BOARDS_COMP2)))
		return 1;
	return 0;
}


BOOL PxPSourceIsDVIHDMI (void) 
{
	if ((UserPrefPipMode != NO_PIP) &&  ((UserPrefCurrentInputPip  == ALL_BOARDS_HDMI) || (UserPrefCurrentInputPip  == ALL_BOARDS_DVI1) || (UserPrefCurrentInputPip  == ALL_BOARDS_DVI2)))
		return TRUE;
	return FALSE;
}

//20120917 Add by KevinChen
BOOL PxPSourceIsDPMiniDP (void) 
{
	if ((UserPrefPipMode != NO_PIP) &&  ((UserPrefCurrentInputPip  == ALL_BOARDS_DP1) || (UserPrefCurrentInputPip  == ALL_BOARDS_DP3)))
		return TRUE;
	return FALSE;
}

//&*&*&*S 20120425 :SWRD4004: Kordonm(0)  Modify for OSD Workbench
BYTE PxPSourceIsDVI (void)
{
	if ((UserPrefPipMode != NO_PIP) && ((UserPrefCurrentInputPip == ALL_BOARDS_DVI1) || (UserPrefCurrentInputPip  == ALL_BOARDS_DVI2)))
		return TRUE;
	return FALSE;
}

BYTE PxPSourceIsHDMI (void)
{
	if ((UserPrefPipMode != NO_PIP) && (UserPrefCurrentInputPip == ALL_BOARDS_HDMI))
		return TRUE;
	return FALSE;
}
//&*&*&*E 20120425 : Kordonm(1)  Modify for OSD Workbench

BYTE PxPSourceIsSVideoComposite (void)
{
	if ((UserPrefPipMode != NO_PIP) &&  ((UserPrefCurrentInputPip  == ALL_BOARDS_SVIDEO1) || 
		(UserPrefCurrentInputPip  == ALL_BOARDS_CVBS1)||(UserPrefCurrentInputPip  == ALL_BOARDS_CVBS2)))
		return 1;
	return 0;
}

void OSDSendMessage(BYTE MessageId)
{
	msg_i("=============>>OSDSendMessage = %d", MessageId);
		
	//Neil120802+ for Icon splash twice 
	if((MessageId == MTO_IconShow)&&(!IsOSDMainMenu()))
	{
		static DWORD DW_MessageTimer = 0;
		if(abs((WORD)(gm_GetSystemTime() - DW_MessageTimer))< 3000)		// don't handler Iconshow message within 3s, icon will show at least 3s
		{
			OSDRequestCancelMsg(CH_A, MTO_IconShow);
			return;
		}
		DW_MessageTimer = gm_GetSystemTime();
	}
	
	//Move down here
	if((B_NoMainIconFlag == gmd_TRUE)&&(MessageId == MTO_IconShow))// don't show port icon after factory reset
	{
		B_NoMainIconFlag = FALSE;
		OSDRequestCancelMsg(CH_A, MTO_IconShow);
		return;
	}	
	
	if(UserPrefOSDMessageCtrl == 1)	
	{
		UserPrefOSDMessageCtrl = 0;
		return;
	}
	OSDRequestMsg (CH_A, MessageId, REQUEST_NOW);
}

void PIPSendMessage(BYTE MessageId)
{
//	extern gmt_OSD_TILE_CTRL ROM DATA_PIPMessagesTile;  
	//if(UserPrefPipMode != NO_PIP)
		if((!Is_PIP_ON())&&((MessageId == MTO_PIPIconShow)||(MessageId == MTO_PIPMShow)))
		{
			OSDRequestCancelMsg(CH_A,MessageId);
			return;
		}
	
	gm_Print(" PIPSendMessage: %d", MessageId);
		#if(defined(BOARD_93XX_4K2K))
		if(gm_IsValidSignal(CH_A))
		#endif
		OSDRequestMsg_Pxp (CH_A,MessageId,REQUEST_NOW);
}

void AdjustPIPMessageOnOff(void)
{
	if (UserPrefPIPMessageOnOffFlag)
	{
		OSDRequestMsg(gmvb_PipChannel, MTO_PIPMShow, REQUEST_NOW); // Show Pip message
	}
	else
	{
		OSDRequestMsg(CH_B, MTO_PIPMHide, REQUEST_NOW); // Clear Pip message	
	}
}

void ChangeOSDRotation ()
{
	AdjustOsdRotation();
	gmvb_OsdRotation = UserPrefOsdRotation;
}

#ifdef CONTROLTYPE_LONGVALUEHEX_USED
void DrawLongValueHexControlWB(BYTE B_ColorState, BYTE B_DrawFlag)
{
	BYTE TestChar[100];
   
	gmt_OSD_LONGVALUEHEX_CTRL ROM *Sp_LongValueCtrl;
	BYTE ROM *Bp_ColorMap;
	SDWORD SDW_AdjValue, SDW_AdjMin, SDW_AdjMax;
	SWORD SW_LVMaxValue, SW_LVMinValue;
	BYTE B_NumOfGlyphs = 0;
	BYTE Ba_TextString[20] = {0};  //that's displaying 20 digits
	WORD W_Interval;
	WORD W_XOffset, W_YOffset;
	BYTE ROM *Bp_Text;

	// Variable Initialization
	gm_OsdGetActionOperand();
	Sp_LongValueCtrl = (gmt_OSD_LONGVALUEHEX_CTRL ROM *)gmv_ActionOpPtr;
	Bp_Text = Sp_LongValueCtrl->UsedGlyphs;

	// Returns true if we need to draw the image and false if it's already
	// been highlighted.
	if(!ParseDrawProperties( B_ColorState, Sp_LongValueCtrl->Colors))
	{
		return;
	}
	
	SW_LVMaxValue = Sp_LongValueCtrl->MaxValue;
	SW_LVMinValue = Sp_LongValueCtrl->MinValue;

	//calls adujster functions from adjustingEngine.c
	SDW_AdjMax = gm_GetAdjusterMax(Sp_LongValueCtrl->Associate);
	SDW_AdjMin = gm_GetAdjusterMin(Sp_LongValueCtrl->Associate);

	// get current value
	SDW_AdjValue = gm_GetAdjuster(Sp_LongValueCtrl->Associate);

	if(SW_LVMaxValue | SW_LVMinValue)
	{
		// We want to map the associate value to a percentage of the way through
		// our current range.
		W_Interval = (WORD)(SDW_AdjMax - SDW_AdjMin);
	
		if( W_Interval != 0 ) 
		{
			SDW_AdjValue =  (( SDW_AdjValue - SDW_AdjMin ) * 
							 ( SW_LVMaxValue - SW_LVMinValue )) / W_Interval
							 + SW_LVMinValue;
		}
		else
		{
			SDW_AdjValue = SW_LVMinValue;
		}
	}

	// if value has been changed in case of updating, do nothing
	if((!B_DrawFlag) && (SDW_AdjValue == gSDW_PreviousLongValue) && 
		(gmv_ActionOpPtr == gBp_PreviousLongValueActionOpPtr))
	{
		return;
	}

	//	save new values
	gSDW_PreviousLongValue = SDW_AdjValue;
	gBp_PreviousLongValueActionOpPtr = gmv_ActionOpPtr;

	Bp_ColorMap = gmv_ColorMap;
	
	// We uncompress the text string if Gb_Osd_Compr_Txt is set.
	if(gmc_OsdComprTxt) 
	{
		gm_GmcaCycleDecompress((BYTE far *)Bp_Text, gmv_OsdTextStringDecompBuffer, DecompressWrapper);
		Bp_Text = (BYTE ROM *)gmv_OsdTextStringDecompBuffer;
	}

	//init font id with the specified font
	gm_OsdSetGlobalFontId(*Bp_Text);

//   B_NumOfGlyphs = 0;
 	Ba_TextString[B_NumOfGlyphs++] = *(Bp_Text + 2+((((WORD)SDW_AdjValue & 0xF000) >> 12) & 0x000F));
	Ba_TextString[B_NumOfGlyphs++] = *(Bp_Text + 2+((((WORD)SDW_AdjValue & 0x0F00) >>  8) & 0x000F));
	Ba_TextString[B_NumOfGlyphs++] = *(Bp_Text + 2+((((WORD)SDW_AdjValue & 0x00F0) >>  4) & 0x000F));
	Ba_TextString[B_NumOfGlyphs++] = *(Bp_Text + 2+  ((WORD)SDW_AdjValue & 0x000F));
	
	// If the value we want to display is negative, add the negative glyph.
//	if(SDW_AdjValue < 0)
// /	{
//		Ba_TextString[B_NumOfGlyphs++] = Bp_Text[glyph_negative + 2];
//		Ba_TextString[B_NumOfGlyphs++] = Bp_Text[glyph_space + 2];
//		SDW_AdjValue = -SDW_AdjValue;
//	}

	// convert to glyphs
//	B_NumOfGlyphs += NumberToGlyphs((DWORD) SDW_AdjValue, 
//								Ba_TextString + B_NumOfGlyphs,  
//								(BYTE far*)Bp_Text + 2);
//  	B_NumOfGlyphs += NumberToGlyphs((DWORD) SDW_AdjValue, 
//								Ba_TextString+B_NumOfGlyphs,  
//								(BYTE far*)Bp_Text + 2);
//	
	W_XOffset = CalculateXAlignmentOffset(B_NumOfGlyphs,
			Sp_LongValueCtrl->Horizontal, Ba_TextString,
			*(Bp_Text), Sp_LongValueCtrl->XSize );

	W_YOffset = CalculateYAlignmentOffset(Sp_LongValueCtrl->Vertical,
			*(Bp_Text), Sp_LongValueCtrl->YSize);

	// Now that we've collected all the information, draw the text with the
	// appropriate rotation.
	DrawRotatedText((BYTE ROM *)Sp_LongValueCtrl, Bp_ColorMap, W_XOffset,
   W_YOffset,	*(Bp_Text), Ba_TextString, B_NumOfGlyphs );

}
#endif

#ifdef CONTROLTYPE_LONGVALUEHEX_USED
void DrawLongValueHexWB(void)
{
	DrawLongValueHexControlWB(LONGVALUE_COLOR_STATE, BIT0);
}

void DrawSelectedLongValueHexWB(void)
{
	DrawLongValueHexControlWB(SELECT_LONGVALUE_COLOR_STATE, TRUE);
}
#endif

void TurnLcdOff(void)
{
	msg_i("TurnLcdOff",0);
	//PanelPowerOffSequence();
	#ifdef USE_TIMER_CTRL_AUD_PWR	
	AudioPowerStatus = OFF;		//Neil 20090313+
	gm_TimerStart(AudioSpeakerPowerUp_100ms_TMR, 15);	//Neil 20090311+ for audio pop noise 
	#endif	

	//gm_ClearRegBitsWord(DISPLAY_CONTROL, DTG_RUN_EN);
	gm_DisplayOnOff(Off);		//Hyb130715- 
}

void TurnLcdOn(void)
{
	msg_i("TurnLcdOn",0);
	//PanelPowerOnSequence();
	#ifdef USE_TIMER_CTRL_AUD_PWR	
	AudioPowerStatus = ON;
	gm_TimerStart(AudioSpeakerPowerUp_100ms_TMR, 15);		//Neil 20090313+
	#endif
	//gm_SetRegBitsWord(DISPLAY_CONTROL, DTG_RUN_EN); 	
#if(IS_PROJ_PD())
	 if(CMD_PowerState != OFF)
#endif
	gm_DisplayOnOff(On);

}

void SaveModeDep_gm57xx(void)
{
	SaveModeIndependentSettings();
}

void DrawDTimingInfo(void)
{
	gmt_OSD_TIMINGINFO_CTRL ROM *Sp_TimingInfoCtrl;
	BYTE ROM *Bp_ColorMap;
	BYTE B_NumOfGlyphs = 0;
	BYTE Ba_TextString[20] = {0};  //that's displaying 20 digits
	WORD W_XOffset, W_YOffset;
//    WORD Wa_CtrlProp[4];
	BYTE i = 0;
	//BYTE TestChar[500];
	BYTE ROM *Bp_Text;
	//WORD w_vfreq;
	BYTE B_Input;	
	BYTE B_Channel;
	
	// Get pointer
	gm_OsdGetActionOperand();
	Sp_TimingInfoCtrl = (gmt_OSD_TIMINGINFO_CTRL ROM *)gmv_ActionOpPtr;
	Bp_Text = Sp_TimingInfoCtrl->UsedGlyphs;
       //gm_Print("DrawDTimingInfo",0);
	// We uncompress the text string if Gb_Osd_Compr_Txt is set.
	if(gmc_OsdComprTxt)
	{
		gm_GmcaCycleDecompress((BYTE far *)Bp_Text, gmv_OsdTextStringDecompBuffer, DecompressWrapper);
		Bp_Text = (BYTE ROM *)gmv_OsdTextStringDecompBuffer;
	}

	if(Sp_TimingInfoCtrl->Channel == 0)
	{
		B_Input = UserPrefCurrentInputMain;
		B_Channel = gmvb_MainChannel;
	}
	else
	{
		B_Input = UserPrefCurrentInputPip;
		B_Channel = gmvb_PipChannel;
	}	

	if(!gm_IsValidSignal(B_Channel))
	{

		for (i = 0; i < 9; i++)
		{
			Ba_TextString[i] = *(Bp_Text + 2 + dTimingInfoNoSignalOffset + i);
		}
		B_NumOfGlyphs = i;	
		
		//Neil 20081202+ for no signal input draw timing info, Buglist 202;
		// Calculate offsets for alignment
		W_XOffset = CalculateXAlignmentOffset(B_NumOfGlyphs, Sp_TimingInfoCtrl->Horizontal, Ba_TextString, *(Bp_Text), Sp_TimingInfoCtrl->XSize);

		W_YOffset = CalculateYAlignmentOffset(Sp_TimingInfoCtrl->Vertical, *(Bp_Text), Sp_TimingInfoCtrl->YSize);

		Bp_ColorMap = Sp_TimingInfoCtrl->Colors;

		// Check to see if a color mapping exists.
		if(Bp_ColorMap != NULL_PTR)
		{
			Bp_ColorMap += 2;
		}

		#ifndef ROTATION_ON_THE_FLY
			DrawRotatedText((BYTE ROM *)Sp_TimingInfoCtrl, Bp_ColorMap, W_XOffset, W_YOffset, (BYTE)*(Bp_Text), (BYTE ROM*)Ba_TextString, B_NumOfGlyphs);
		#else
			DrawRotatedText((BYTE ROM *)Sp_TimingInfoCtrl, Bp_ColorMap, W_XOffset, W_YOffset, *Bp_Text, Ba_TextString, B_NumOfGlyphs );
		#endif	//ROTATION_ON_THE_FLY
		return;
	}

#ifdef DELL_U2410F
       if(OSDDrawVideoTimingInfo(B_Channel))
#else
	if((gm_IsVideo(B_Channel)) && (((IsGraphicInput(B_Input) && (!IsTMDSInput(B_Input))) && (!OSD_IsGraphics)) || 
										((IsTMDSInput(B_Input)) && (!UserPrefHDMIColorMode)) ||
										IsVideoInput(B_Input) || gm_IsInterlaced(B_Channel))||
										((stInput[B_Channel].Vfrq< 310) && (gm_GetCurrentInputModeType(B_Channel) == gmd_1080P)))
#endif										
	{
              gm_Print("Draw Video timing",0);    
		/*
		if(B_Input == ALL_BOARDS_CVBS1)
		{
			// ***  0: NTSC/480I/525I -  720 x  240 x 60I ***
			#ifdef DELL_U2410F
			if(((gm_ReadRegByte(HVTIMER_STATUS1) & STD_DETECTED) == NTSC443) ||
				((gm_ReadRegByte(HVTIMER_STATUS1) & STD_DETECTED) == NTSC))
			#else
			if(stInput[B_Channel].Vfrq == 599 && gmvw_InputVActiveOrg[B_Channel] == 240)
			#endif
			{
				// PAL-M and NTSC both are detected at 60 Hz. field rate
				// from the mode handler code.  Use video decoder mode
				// indicator to differentiate between PAL and NTSC.  The
				// video decoder returns PAL mode with a field rate of
				// 60 Hz. for PAL-M.
				
				// NTSC mode -- display as NTSC@60Hz.
            			gm_Print("--------NTSC/480I/525I",0);
				for (i = 0; i < 4; i++)
				{
					Ba_TextString[i] = *(Bp_Text + 2 + glyph_N + i);
				}
				B_NumOfGlyphs = i;
			}
         		// *** 1: PAL/576I/625I  -  720 x  288 x 50I ***
         		#ifdef DELL_U2410F //071219 merge
			else if(((gm_ReadRegByte(HVTIMER_STATUS1) & STD_DETECTED) != NTSC443) &&
				((gm_ReadRegByte(HVTIMER_STATUS1) & STD_DETECTED) != NTSC))
			#else
         		else if(stInput[B_Channel].Vfrq == 500 && gmvw_InputVActiveOrg[B_Channel] == 288)
			#endif
         		{
            			// PAL and SECAM both are detected at 50 Hz. field rate
            			// from the mode handler code.  Use video decoder mode
            			// indicator to differentiate between PAL and SECAM.
				if (gm_DecoderGetVideoMode(gm_GetAiptoChannel(CH_A) ==
        						gmd_ADC_0 ? gmd_DEC_CORTEZ_ID1 : gmd_DEC_CORTEZ_ID2) == gmd_DEC_MODE_SECAM)
				{
					// SECAM mode -- display as SECAM@50Hz.
					gm_Print("--------SECAM/576I/625I ",0);
					for (i = 0; i < 5; i++)
					{
						Ba_TextString[i] = *(Bp_Text + 2 + dTimingInfoSecamOffset + i);
					}
					B_NumOfGlyphs = i;
				}
				else
				{
					// PAL mode -- display as PAL@50Hz.
					gm_Print("--------PAL/576I/625I ",0);
					for (i = 0; i < 3; i++)
					{
						Ba_TextString[i] = *(Bp_Text + 2 + glyph_P + i);
					}
					B_NumOfGlyphs = i;
				}
         		}
		}	
		else*/
		if((B_Input == ALL_BOARDS_COMP1) || (B_Input == ALL_BOARDS_DP1) || (B_Input == ALL_BOARDS_DP3) ||
			(B_Input == ALL_BOARDS_HDMI) || (B_Input == ALL_BOARDS_DVI1) || 
			(B_Input == ALL_BOARDS_DVI2) || (B_Input == ALL_BOARDS_VGA1)||
			(B_Input == ALL_BOARDS_CVBS1) || (B_Input == ALL_BOARDS_CVBS2)||(B_Input == ALL_BOARDS_COMP2))
      		{
			#ifdef DELL_U2410F
                     if((B_Input == ALL_BOARDS_DP1)||(B_Input == ALL_BOARDS_DP3))
			 	gmvw_InputVActiveOrg[B_Channel] = gm_ReadRegWord(DP12RX0_MS_VACT_WIDTH);
			#endif
			if (gm_IsInterlaced(B_Channel))
			{
				WORD W_VActive;

				//gm_Print("--------%di ",gmvw_InputVActiveOrg[B_Channel]<< 1);			
				// convert height from number to glyphs
				//Neil120811+ for video timing sometimes show 574i, 1152i,etc
				W_VActive = CheckVideoTimingResolution(B_Channel, gmvw_InputVActiveOrg[B_Channel]);
				if(W_VActive == 0)
					W_VActive = gmvw_InputVActiveOrg[B_Channel];
				B_NumOfGlyphs += NumberToGlyphs((DWORD)(W_VActive<< 1), Ba_TextString + B_NumOfGlyphs, (BYTE far *)(Bp_Text + 2));

				Ba_TextString[B_NumOfGlyphs++] = *(Bp_Text + 2 + glyph_i_lower);
			}
			else// Progressive mode  // only show input Height + p
			{
				WORD W_VActive;
				//gm_Print("--------%dP ",gmvw_InputVActiveOrg[B_Channel]);			

				W_VActive = CheckVideoTimingResolution(B_Channel, gmvw_InputVActiveOrg[B_Channel]);
				if(W_VActive == 0)
					W_VActive = gmvw_InputVActiveOrg[B_Channel];
				
				// convert height from number to glyphs
				B_NumOfGlyphs += NumberToGlyphs((DWORD)W_VActive, Ba_TextString + B_NumOfGlyphs, (BYTE far *)(Bp_Text + 2));

				Ba_TextString[B_NumOfGlyphs++] = *(Bp_Text + 2 + glyph_p_lower);
			}
		}
	}
	else 
	{	
//              gm_Print("Draw Graphic timing",0);     
	#ifdef DELL_U2410F //KevinL 20081209	
		if(B_Channel == gmvb_MainChannel)
		{
			if((UserPrefCurrentInputMain == ALL_BOARDS_DP1)||(UserPrefCurrentInputMain == ALL_BOARDS_DP3))
			{			
			   	gmvw_InputHActiveOrg[B_Channel] = gm_ReadRegWord(DP12RX0_MS_HACT_WIDTH);
 		  	      	gmvw_InputVActiveOrg[B_Channel] = gm_ReadRegWord(DP12RX0_MS_VACT_WIDTH);
			}
		}
	 	else
	 	{
			if((UserPrefCurrentInputPip == ALL_BOARDS_DP1)||(UserPrefCurrentInputPip == ALL_BOARDS_DP3))
			{			
			   	gmvw_InputHActiveOrg[B_Channel] = gm_ReadRegWord(DP12RX0_MS_HACT_WIDTH);
 		  	      	gmvw_InputVActiveOrg[B_Channel] = gm_ReadRegWord(DP12RX0_MS_VACT_WIDTH);
			}		 	
	 	}		
	#endif	
		
		gmvw_InputHActiveOrg[B_Channel] &= 0xFFFE;		//Neil 20090416+ force to even
		
	 	if(UserPrefOsdRotation == OSD_NON_ROTATED) 
		{	
			// Convert width from number to glyphs
			B_NumOfGlyphs += NumberToGlyphs((DWORD)gmvw_InputHActiveOrg[B_Channel], Ba_TextString + B_NumOfGlyphs, (BYTE far *)(Bp_Text + 2));

			Ba_TextString[B_NumOfGlyphs++] = *(Bp_Text + 2 + glyph_x);
			if( gmvw_InputVActiveOrg[CHANNEL_A]==722) //force to show 1280x720  timing info for CVT mode
				B_NumOfGlyphs += NumberToGlyphs((DWORD)720, Ba_TextString + B_NumOfGlyphs, (BYTE far *)(Bp_Text + 2));
			else  // convert height from number to glyphs
				B_NumOfGlyphs += NumberToGlyphs((DWORD)gmvw_InputVActiveOrg[B_Channel], Ba_TextString + B_NumOfGlyphs, (BYTE far *)(Bp_Text + 2));
		}
		else
		{
			// Convert width from number to glyphs
			B_NumOfGlyphs += NumberToGlyphs((DWORD)gmvw_InputHActiveOrg[B_Channel], Ba_TextString + B_NumOfGlyphs, (BYTE far *)(Bp_Text + 2));

			Ba_TextString[B_NumOfGlyphs++] = *(Bp_Text + 2 + glyph_x);

			// convert height from number to glyphs
			B_NumOfGlyphs += NumberToGlyphs((DWORD)gmvw_InputVActiveOrg[B_Channel], Ba_TextString + B_NumOfGlyphs, (BYTE far *)(Bp_Text + 2));
		}
		//Debugmessage_i("--------B_NumOfGlyphs=%d ",B_NumOfGlyphs);

		Ba_TextString[B_NumOfGlyphs++] = *(Bp_Text + 2 + glyph_sp);
		Ba_TextString[B_NumOfGlyphs++] = *(Bp_Text + 2 + 33);
		Ba_TextString[B_NumOfGlyphs++] = *(Bp_Text + 2 + glyph_sp);

		//gm_Print("~~~~ stInput[B_Channel].Vfrq = %d", stInput[B_Channel].Vfrq);
		{
			DWORD DW_TempVHz = (DWORD)stInput[B_Channel].Vfrq/10;
			if((stInput[B_Channel].Vfrq%10) >= 4)
				DW_TempVHz += 1;
			if(DW_TempVHz == 61)
				DW_TempVHz = 60;
			B_NumOfGlyphs += NumberToGlyphs(DW_TempVHz, Ba_TextString + B_NumOfGlyphs, (BYTE far *)(Bp_Text + 2));
		}

		Ba_TextString[B_NumOfGlyphs++] = *(Bp_Text + 2 + glyph_H);
		Ba_TextString[B_NumOfGlyphs++] = *(Bp_Text + 2 + glyph_z);

	}

	// Calculate offsets for alignment
	W_XOffset = CalculateXAlignmentOffset(B_NumOfGlyphs, Sp_TimingInfoCtrl->Horizontal, Ba_TextString, *(Bp_Text), Sp_TimingInfoCtrl->XSize);
	W_YOffset = CalculateYAlignmentOffset(Sp_TimingInfoCtrl->Vertical, *(Bp_Text), Sp_TimingInfoCtrl->YSize);

	Bp_ColorMap = Sp_TimingInfoCtrl->Colors;

	// Check to see if a color mapping exists.
	if(Bp_ColorMap != NULL_PTR)
	{
		Bp_ColorMap += 2;
	}

#ifndef ROTATION_ON_THE_FLY
	DrawRotatedText((BYTE ROM *)Sp_TimingInfoCtrl, Bp_ColorMap, W_XOffset, W_YOffset, (BYTE)*(Bp_Text), (BYTE ROM*)Ba_TextString, B_NumOfGlyphs);
#else
	DrawRotatedText((BYTE ROM *)Sp_TimingInfoCtrl, Bp_ColorMap, W_XOffset, W_YOffset, *Bp_Text, Ba_TextString, B_NumOfGlyphs );
#endif	//ROTATION_ON_THE_FLY
}   	


void PanelPowerOffSequence (void)
{
	BYTE B_DelayCount = 0;

	gm_ClearRegBitsWord(DISPLAY_CONTROL, DTG_RUN_EN);
	//Back light(PBIAS) off  
	while (((gm_ReadRegByte(PANEL_POWER_STATUS) & 0x03) == 0x03)||((gm_ReadRegByte(PANEL_POWER_STATUS) & 0x03) == 0x02))
	{
		//gmp_HwDelay1ms(1);
		B_DelayCount++;

		if (B_DelayCount == 150)
		{
			msg ("PowerOff:!");
			break;
		}
	};
	gm_Delay100ms(2);				//StKam090115		Panel specification T4>200ms	
	//LVDS Clock off
	CustomPanelOnOffControl(Off);	
	gm_Delay10ms(3);				//StKam090115		Panel specification 50ms>T5>0.1ms	
	//Panel Power off 
	if(!((UserPrefPwrAudio == Off) && AudioDetect())) //KevinL 20090225 for Power Save Audio function
	#ifdef CMI_73XX_BOARD
	
	#else
		//gm_ClearRegBitsWord(GPOUTPUT5, PIN_PWR24V_ON);      //AC12-GPIO88-PWR24_ON-Cn402 pin 16 - Power board control
	#endif

	//Remove for M1F192A	KevinL 20100224 
	//gm_SetRegBitsWord(GPIO_4_ENABLE, GPIO77_EN); //KevinL 20081027 for Power Saving GPIO77(22mA), GPIO79(34mA)
	//gm_SetRegBitsWord(GPIO_4_DIRCTRL, GPIO77_EN);	
	
	gm_ClearRegBitsWord(DISPLAY_CONTROL, DTG_RUN_EN);	
}

void PanelPowerOnSequence (void)
{
	BYTE B_DelayCount = 0;

	// set display freerun on startup so that output is not tied to input
	gm_SetDisplayFreeRun();
	gm_ForceUpdate(CH_A, OUTPUT_CLOCK);

	CustomPanelOnOffControl(On);

	gm_DisplayOnOff(On);
	//--- delete it //LVDSChannelCBus0Patch(On);	//20120823 Kordonm+ for Panel Power Sequece (CH_C.0) Patch	//20120921 Kordonm modify
      
#if (CHIP_ID != STDP93xx)    
	// Power up LVDS block
	//gm_PowerUp(POWER_LVDS); //Mikhail
	if((gm_ReadRegByte(LVDS_POWER)&0x3F) != 0x3F)
		gm_WriteRegByte(LVDS_POWER, 0x3F);	
#endif

	// verify that panel control and data are active
	while ((gm_ReadRegByte(PANEL_POWER_STATUS) & 0x02) != 0x02)
	{
		//gmp_HwDelay1ms(1);
		B_DelayCount++;

		if (B_DelayCount == 230)
		{
			msg ("PowerOn:!");
			break;
		}
	};	
}

void AdjustOsdBlend(void)
{
#if 1 // Mikhail 20070628
	WORD blendvalue;

	//blendvalue=((15*UserPrefOsdBlend)/100);
          blendvalue=((12*UserPrefOsdBlend)/100);//owen 120828

	gm_WriteRegWord(OSD_BLEND_TABLE0, blendvalue);	
	//gm_WriteRegWord(OSD_BLEND_TABLE0, 0);//demo temp	
#else
	gm_WriteRegWord(OSD_BLEND_TABLE0, UserPrefOsdBlend<<4);
#endif     
	gm_WriteRegWord(HOST_CONTROL, 0x0008);
}

void HideMain ()
{
	BYTE i;

	//Debugmessage_i("~~~~ HideMain ... ###", 0);
	
	for (i = 2; i <= 15; i++)
	{
		gm_OsdDisableTile(i);
		ReverseTileSet();
		gm_OsdDisableTile(i);
	}
}

void PipBorderControl(void)
{
	//Debugmessage_i("~~~~ PipBorderControl ... ###", 0);

	//if (OSD_IsMainSync || OSD_IsPipSync)
	if (OSD_IsMainSync || OSD_IsPipSync ||OSD_IsPipOutOfRange||!OSD_IsCableConnectedMain) //KevinL 20081017 conditions of do not turn LCD off
	{
		if(Is_PIP_Pip()) //KevinL 20081023 only PIP On need to set boarder and display PIP message
		{
			SetPipBorder(gmvb_PipChannel, TRUE);
			OSDRequestMsg (CH_B, MTO_PIPMShow, REQUEST_NOW); // Show Pip message
		}
		//if ((Is_Pxp_Off()) && (!OSD_IsMainSync) && (!IsOSDFlying()))
		//	TurnLcdOff();
		//else 
		//if(!IsLcdOn())
		//	TurnLcdOn();
	}
	else
	{
		#ifndef DELL_U2410F
			SetPipBorder(gmvb_PipChannel, FALSE); // when both channel is no signal clear border ... //20081211- don't clear border even pip no signal
		#endif	
		OSDRequestMsg (CH_B, MTO_PIPMHide, REQUEST_NOW); // Clear Pip message
		//if (!IsOSDFlying())
		//	TurnLcdOff();
	}
}

//====================Splash Screen====================================

void AdjustCustomSplashScreen(void)
{

/*#ifdef DELL_U2410F//081029 Edward for DELL logo display no blending
	gm_WriteRegWord( OSD_BLEND_TABLE1, 0); 
	gm_WriteRegWord( OSD_BLEND_TABLE0, 0);
	gm_SetRegBitsWord(HOST_CONTROL, ODP_SYNC_UPDATE);
#else	 
	if (UserPrefCustomSplashScreen > 60)
	{
		WORD tmp = ((UserPrefCustomSplashScreen - 60)<<8)|0xf0ff; // Begin value == 75
			
		gm_WriteRegWord( OSD_BLEND_TABLE1, tmp);  // Blend 7
		gm_WriteRegWord( OSD_BLEND_TABLE0, 0xfff0);
	}
	else if (UserPrefCustomSplashScreen > 45)
	{
		WORD tmp = ((UserPrefCustomSplashScreen-45)<<4)|0xf00f;

		gm_WriteRegWord( OSD_BLEND_TABLE1, tmp);  // Blend 6
		gm_WriteRegWord( OSD_BLEND_TABLE0, 0xfff0);
	}
	else if (UserPrefCustomSplashScreen > 30)
	{
		WORD tmp = ((UserPrefCustomSplashScreen-30))|0xf000;
		gm_WriteRegWord( OSD_BLEND_TABLE1, tmp);  // Blend 5
		gm_WriteRegWord( OSD_BLEND_TABLE0, 0xfff0);
	}
  	else if (UserPrefCustomSplashScreen > 15)
	{
		WORD tmp = ((UserPrefCustomSplashScreen-15)<<12)|0x0ff0;
		gm_WriteRegWord( OSD_BLEND_TABLE1, 0);
		gm_WriteRegWord( OSD_BLEND_TABLE0, tmp); // Blend 4
	}
	else
	{
		//WORD tmp = ((UserPrefCustomSplashScreen) << 4) | 0x000f;
		gm_WriteRegWord( OSD_BLEND_TABLE1, 0);
		gm_WriteRegWord( OSD_BLEND_TABLE0, 0);
	}

	if (UserPrefCustomSplashScreen > 14)
		gm_SetRegBitsWord(HOST_CONTROL, ODP_SYNC_UPDATE);
#endif	
*/
}

void SplashScreenTimerHandler(void)
{
	/*		//Hyb130102- 
	if (gm_TimerCheck(SPLASHSCREEN_1ms_TMR) == TIMER_TMO)
	{
		gm_TimerStop(SPLASHSCREEN_1ms_TMR);
		OSDRequestMsg(gmvb_MainChannel, MTO_SplashScreenChangeState, REQUEST_NOW);	
		//if (UserPrefCustomSplashScreen == 0)
		//{
		//	Debugmessage_i("~~~~ Splash Screen Complete 1 !%d", gm_GetSystemTime());
		//	B_SplashComplete = TRUE;
		//}
	}
	*/
}

void SetSplashScreenTimer(void)
{
	BYTE B_AppendValue = gm_OsdGetActionByteParameter();

	//gm_Print("~~~~ SetSplashScreenTimer = %d ", B_AppendValue);
	//gm_Print("~~~~ UserPrefCustomSplashScreen = %d ", UserPrefCustomSplashScreen);
	if (B_AppendValue == 255) // Enable ModeSetup ...
	{
		//gm_Print("~~~~ Splash Screen Complete !%d", gm_GetSystemTime());
		B_SplashComplete = TRUE;
		OSDRequestMsg (CH_A, MTO_PIPMHide, REQUEST_NOW); // Clear Pip message
		if (gm_IsValidSignal(gmvb_MainChannel))
		{
			ModeSetupRequestCancel(gmvb_MainChannel, REQ_MS_SETUP, PARAM_NONE);
			ModeSetupRequest(gmvb_MainChannel, REQ_MS_SETUP, PARAM_NONE, REQUEST_NOW);
		}
	}
	else
	{
		gm_TimerStart(SPLASHSCREEN_1ms_TMR, B_AppendValue);
	}
}

void HardLockTimerHandler(void)
{
extern BOOL CheckMenuKey;

	if (gm_TimerCheck(HardLock_1s_TMR) == TIMER_TMO)
	{
		//Debugmessage_i("Send MTO_HardLock",0); 
		gm_TimerStop(HardLock_1s_TMR);
		OSDRequestMsg(gmvb_MainChannel, MTO_HardLock, REQUEST_NOW);	
		CheckMenuKey =	FALSE;
	}
}

BOOL IsHardLockTimerRunning(void)
{
	BYTE B_Result = FALSE;

	if (gm_TimerCheck(HardLock_1s_TMR) == TIMER_OK)
	{
		B_Result = TRUE;
	}

	return B_Result;
}

void StartHardLockTimer(void)
{
	gm_TimerStart(HardLock_1s_TMR, 10);		//Neil 20120525*
	if (UserPrefOsdTimeout < 16) // extend the OSD timeout for Hardlock ...
	{
		gB_OsdTimerStart[B_OSD_TimeoutTid] = OSD_TIMER_START;
			
		//
		// OSD menu timeout in second base unit
		//
		gW_OsdTimers[B_OSD_TimeoutTid] =
			(WORD)(((DWORD)(15+UserPrefOsdTimeout) * 100) / W_OsdTimerResolution);		
	}
}
void AdjustPanelProtectCount(void)   // 140530
{
       if(UserPrefPanelProtect)
       gm_TimerStart(PanelProtect_1s_TMR,10);
	   W_SavedInputHStart[CH_A] = gm_ReadRegWord(ip_h_act_start[CH_A]);
}
void AdjustPanelHStart(gmt_PHY_CH B_Channel)    //wei 140530
{
        if(UserPrefPanelProtect)
        {
	        if(gm_TimerCheck(PanelProtect_1s_TMR)==TIMER_TMO)
	        {
	            UserPrefPanelProtectAdjustHStartFlag = !UserPrefPanelProtectAdjustHStartFlag;
				if(UserPrefPanelProtectAdjustHStartFlag)
				{
		            gm_WriteRegWord(ip_h_act_start[B_Channel], W_SavedInputHStart[B_Channel]+1);//(MaxHStart()-2+MinHStart())
					//if(IsDigitalInput(B_Channel))
					//	gm_ClearRegBitsWord(IMP_CONTROL, IMP_EXT_DV_EN);
					gm_SyncUpdate(B_Channel, INPUT_OUTPUT_CLOCK);
				        gm_TimerStart(PanelProtect_1s_TMR,6);
				}
				else
				{
					gm_WriteRegWord(ip_h_act_start[B_Channel], W_SavedInputHStart[B_Channel]);	
					//if(IsDigitalInput(B_Channel))
					//	gm_SetRegBitsWord(IMP_CONTROL, IMP_EXT_DV_EN);
					gm_SyncUpdate(B_Channel, INPUT_OUTPUT_CLOCK);
					gm_TimerStart(PanelProtect_1s_TMR,6);
				}
	        }
		}
		else
		{
			if(gm_ReadRegWord(ip_h_act_start[B_Channel]) != W_SavedInputHStart[B_Channel])
			{	
				gm_WriteRegWord(ip_h_act_start[B_Channel], W_SavedInputHStart[B_Channel]);	
				gm_SyncUpdate(B_Channel, INPUT_OUTPUT_CLOCK);
				gm_TimerStop(PanelProtect_1s_TMR);
			}
		}
}

//extern void SetTouchPadLedStatus(BYTE LedStatus);
void SetOSDMenuState(BYTE MenuState)
{
#ifdef TOUCH_KEY
//#ifdef TouchPadLed //Lvjie 081013
    static BYTE PrevState = 0xFF;
    BYTE Buff_Osd[5];
	MenuKeyState = MenuState;
	if (MenuKeyState == CLOSE_OSD)
	{
		#ifdef TOUCHKEY_INVERSE
		MenuKeyState = FIVE_KEYSTATE;
		#else
		MenuKeyState = ZERO_KEYSTATE;
		#endif
		//SetTouchPadLedStatus(0);//turn off LED when OSD timeout
	} 
	#ifdef TOUCHKEY_INVERSE
	if(MenuKeyState == ZERO_KEYSTATE)
		MenuKeyState = CLOSE_OSD;
	#endif

    if ( PrevState != MenuKeyState )
    {
        PrevState = MenuKeyState;

        Buff_Osd[0] = 0xED;
        Buff_Osd[1] = B_SplashComplete;
        Buff_Osd[2] = UserPrefFactoryFlag;
        Buff_Osd[3] = PwrState;
        Buff_Osd[4] = MenuKeyState;
        if(gm_MessageSend(gmd_MESSAGE_USER, &Buff_Osd, sizeof(Buff_Osd)))
        {
			#ifdef UART_DEBUG_MODE
            msg_i("Buff_Osd[0] = %d",Buff_Osd[0]);
            msg_i("Buff_Osd[1] = %d",Buff_Osd[1]);
            msg_i("Buff_Osd[2] = %d",Buff_Osd[2]);
            msg_i("Buff_Osd[3] = %d",Buff_Osd[3]);
            msg_i("Buff_Osd[4] = %d",Buff_Osd[4]);
            msg_i("MenuKeyState send as %d",MenuKeyState);
			#endif
        }
        else
        {
            msg_i("==> MenuKeyState send as %d failed",MenuKeyState);
        }
    }
#endif
MenuState = MenuState;

}

BOOL IsOSDMainMenu(void)
{
      return MenuKeyState == FOUR_KEYSTATE;
}
BOOL IsOSDSideMenu(void)
{
      return MenuKeyState == FIVE_KEYSTATE;
}

BOOL ISZeroKeyState(void)
{
      return MenuKeyState == ZERO_KEYSTATE;
}

BOOL IsThreeKeyState(void)
{
	return MenuKeyState == THREE_KEYSTATE;
}

BOOL IsBurnInState(void)
{
      return MenuKeyState == BURNIN_KEYSTATE;
}

BOOL IsDiagnosticState(void)
{
      return MenuKeyState == DIAGNOSTIC_KEYSTATE;
}

BOOL IsFactoryKeyState(void)
{
      return MenuKeyState == FIVE_KEYSTATE;
}

/*
void DrawDynamicText2(void)
{	// Fli8668 Version Dell
	
	DynamicText[0] = 'T';
	DynamicText[1] = 'O';
	DynamicText[2] = '0';
	DynamicText[3] = '8';
	DynamicText[4] = '3';
	DynamicText[5] = '0';   
	DynamicText[6] = 'A';

	DrawDynamicTextControl(TEXT_COLOR_STATE);
}

void DrawDynamicText3(void)
{	// Fli8668 Version GNSS

	DynamicText[0] = 'S';
	DynamicText[1] = 'L';
	DynamicText[2] = '0';
	DynamicText[3] = '8';
	DynamicText[4] = '3';
	DynamicText[5] = '0';   
	DynamicText[6] = 'A' ;

	DrawDynamicTextControl(TEXT_COLOR_STATE);
}
*/
void SetSavingBitFlag()
{
   BYTE temp = gm_OsdGetCurrentSATOffsetByte();
   UserPrefSavingBitFlags |= temp;
   SaveModeIndependentSettings();
}

//&*&*&*S 20120328:SWRD4002: Kordonm(0)  DDC2Bi Dynamic String Test
#if 0
void DrawDynamicTextControl(WORD W_ColorState)
{
	gmt_OSD_LONGVALUE_CTRL ROM *Sp_LongValueCtrl;
	BYTE ROM *Bp_ColorMap;
	SDWORD SDW_AdjValue, SDW_AdjMin, SDW_AdjMax;
	SWORD SW_LVMaxValue, SW_LVMinValue;
	BYTE B_NumOfGlyphs = 0;
	BYTE Ba_TextString[63] = {0};  //that's displaying 20 digits
	WORD W_Interval;
	WORD W_XOffset, W_YOffset;
	BYTE ROM *Bp_Text;
	#if 1
	BYTE B_DrawFlag=0;
	BYTE i;
	#endif

	// Variable Initialization
	gm_OsdGetActionOperand();
	Sp_LongValueCtrl = (gmt_OSD_LONGVALUE_CTRL ROM *)gmv_ActionOpPtr;
	Bp_Text = Sp_LongValueCtrl->UsedGlyphs;

	// Returns true if we need to draw the image and false if it's already
	// been highlighted.
	if(!ParseDrawProperties( W_ColorState, Sp_LongValueCtrl->Colors))
	{
		return;
	}

	// if value has been changed in case of updating, do nothing
	if((!B_DrawFlag) && (SDW_AdjValue == gSDW_PreviousLongValue) && 
		(gmv_ActionOpPtr == gBp_PreviousLongValueActionOpPtr))
	{
		return;
	}

	Bp_ColorMap = gmv_ColorMap;
	
	// We uncompress the text string if Gb_Osd_Compr_Txt is set.
	if(gmc_OsdComprTxt) 
	{
		gm_GmcaCycleDecompress((BYTE far *)Bp_Text, gmv_OsdTextStringDecompBuffer, DecompressWrapper);
		Bp_Text = (BYTE ROM *)gmv_OsdTextStringDecompBuffer;
	}

	//init font id with the specified font
	gm_OsdSetGlobalFontId(*Bp_Text);

	#if 1
	#ifndef VERSION_PLUS
	for (i = 0; i < 6; i++)	//KevinL 20081022
	#else
	for (i = 0; i < 7; i++)	//calo update 20081022
	#endif
	{
		//The structure of uncompressed buffer is as follows:
		//Byte 0: glyph table index
		//Byte 1: number of glyphs
		//Byte 2 .. byte n: glyph indices
		if(DynamicText[i] >= 48 && DynamicText[i]<= 57)
			{
			// 0-9
			Ba_TextString[B_NumOfGlyphs++] =Bp_Text[DynamicText[i]-48 + 2];
			}
		else  if(DynamicText[i] >= 97 && DynamicText[i]<= 122)
			{
			// a-z
			Ba_TextString[B_NumOfGlyphs++] =Bp_Text[DynamicText[i]-60 + 2];
			}
		else  if(DynamicText[i] == 32)
			{
			// ' '
			Ba_TextString[B_NumOfGlyphs++] =Bp_Text[DynamicText[i]+32 + 2];
			}
		else
			{
			// A-Z
			Ba_TextString[B_NumOfGlyphs++] =Bp_Text[DynamicText[i]-55 + 2];
			}
	}	
	#endif


	W_XOffset = CalculateXAlignmentOffset(B_NumOfGlyphs,
			Sp_LongValueCtrl->Horizontal, Ba_TextString,
			*(Bp_Text), Sp_LongValueCtrl->XSize );

	W_YOffset = CalculateYAlignmentOffset(Sp_LongValueCtrl->Vertical,
			*(Bp_Text), Sp_LongValueCtrl->YSize);

	// Now that we've collected all the information, draw the text with the
	// appropriate rotation.
	DrawRotatedText((BYTE ROM *)Sp_LongValueCtrl, Bp_ColorMap, W_XOffset,
					 W_YOffset,	*(Bp_Text), Ba_TextString, B_NumOfGlyphs );
}
#endif
//&*&*&*E 20120328 : Kordonm(1)  DDC2Bi Dynamic String Test


void AdjustColorFormat(void)
{
	SavePortSchemeDependentSettings();
	AdjustColorDomain();
	//ModeSetupRequest(gmvb_MainChannel, REQ_MS_RESTART, PARAM_NONE, REQUEST_NOW);	// Modified by ST_hansPENG 081127
}

//&*&*&*S 20120315:SWRD4001: Kordonm(0)  Modify for OSD Workbench
BYTE IsMainGraphicsMode(void)
{
	return ((UserPrefCurrentInputMain == ALL_BOARDS_HDMI) || (UserPrefCurrentInputMain == ALL_BOARDS_DVI1) ||
			(UserPrefCurrentInputMain == ALL_BOARDS_DVI2) || (UserPrefCurrentInputMain == ALL_BOARDS_VGA1) || 
			(UserPrefCurrentInputMain == ALL_BOARDS_DP1)||(UserPrefCurrentInputMain == ALL_BOARDS_DP3));
}

BYTE IsPipGraphicsMode(void)
{
	return ((UserPrefCurrentInputPip == ALL_BOARDS_HDMI) || (UserPrefCurrentInputPip == ALL_BOARDS_DVI1) ||
			(UserPrefCurrentInputPip == ALL_BOARDS_DVI2) ||(UserPrefCurrentInputPip == ALL_BOARDS_VGA1) ||
			(UserPrefCurrentInputPip == ALL_BOARDS_DP1)||(UserPrefCurrentInputPip == ALL_BOARDS_DP3));
}
//&*&*&*E 20120315 : Kordonm(1)  Modify for OSD Workbench

BYTE IsMainTMDSMode(void)
{
	return ((UserPrefCurrentInputMain == ALL_BOARDS_HDMI) || (UserPrefCurrentInputMain == ALL_BOARDS_DVI1) ||
			(UserPrefCurrentInputMain == ALL_BOARDS_DVI2));
}

BYTE IsPipTMDSMode(void)
{
	return ((UserPrefCurrentInputPip == ALL_BOARDS_HDMI) || (UserPrefCurrentInputPip == ALL_BOARDS_DVI1) ||
			(UserPrefCurrentInputPip == ALL_BOARDS_DVI2));
}

BYTE InputConvertScheme(BYTE B_Input)  
{
#if 1//120328 Edward for DELL input
	if (B_Input == ALL_BOARDS_DVI1)
		return INPUT_DVI1;	
	else if (B_Input == ALL_BOARDS_DP1)
		return INPUT_DP;	
	#ifdef MUX_TEST_BOARD
	else if (B_Input == ALL_BOARDS_DP3)
		return INPUT_MiniDP;	
	#endif
	else if (B_Input == ALL_BOARDS_HDMI)
		return INPUT_HDMI;	
	else if (B_Input == ALL_BOARDS_VGA1)
		return INPUT_VGA;
	#ifdef EXTERNAL_DECODER
	else if (B_Input == ALL_BOARDS_COMP2)
		return INPUT_COMP2;
	else if (B_Input == ALL_BOARDS_CVBS1)
		return INPUT_CVBS1;
	else if (B_Input == ALL_BOARDS_CVBS2)
		return INPUT_CVBS2;	
	#endif
	#if(ON_BOARD_COMP == ENABLE)
	else if (B_Input == ALL_BOARDS_COMP1)
		return INPUT_COMP;
	#endif
	
	else
	{
		return INPUT_DVI1;
	}
#else	
	if (B_Input == ALL_BOARDS_VGA1)
		return INPUT_VGA;
	else if (B_Input == ALL_BOARDS_DVI1)
		return INPUT_DVI1;
	else if (B_Input == ALL_BOARDS_DVI2)
		return INPUT_DVI2;	
	else if (B_Input == ALL_BOARDS_HDMI)
		return INPUT_HDMI;	
	else if (B_Input == ALL_BOARDS_DP1)
		return INPUT_DP;	
	else if (B_Input == ALL_BOARDS_CVBS1)
		return INPUT_CVBS;	
	else if (B_Input == ALL_BOARDS_COMP1)
		return INPUT_COMP;	
	else
	{
		Debugmessage_i("~~~~ Not Correct B_Input = %d", B_Input);
		return INPUT_VGA;
	}
#endif	
}

//120417 Edward add for NVRAM save local parameter in each Preset mode
BYTE ConvertPresetMode(BYTE ImageScheme)  
{
	if (ImageScheme < Graphics_COLOR_SPACE)
		return ImageScheme;	
	else if (ImageScheme == Graphics_COLOR_SPACE)
		{
			if(IsColorSpace_GRAPHICS_ADOBE_RGB)
				return WB_AdobeRGB;
			else if(IsColorSpace_GRAPHICS_SRGB)
				return WB_sRGB;
			else if(IsColorSpace_GRAPHICS_CAL1)
				return WB_CAL1;
			else if(IsColorSpace_GRAPHICS_CAL2)
				return WB_CAL2;
		}			
	else if (ImageScheme > Graphics_COLOR_SPACE)
		return WB_CustomColor;	

	
	return Graphics_STANDARD;
	
}

void Slave_SetMiscControl(void)
{
}

void EnableGameMode(void) //KevinL 20090415
{
//111004 Edward return for temp , it's need to posting code of VGA_GAME_MODE_PATCH
//#pragma message "!!!!Need to implement game mode function"
//return;

#ifdef VGA_GAME_MODE_PATCH //KevinL 20090420
	if (!IsVGADisplayFrameLock())	
#endif		
	{
		
		//120925 Edward add for OSD change disable VWD when Preset mode is "Game"
		if(UserPrefVwdFlag == ON)
		{
			UserPrefVwdFlag = OFF;
			VWD_Stop();
			VWD_ClearHighlighWindow();//120926 Edward for contrast can not adjust issue in the "Game" mode
		}
		
  		if (IsAnalogInput(gmvb_MainChannel))
		{
			if(UserPrefVwdFlag == ON)
			{
				UserPrefVwdFlag = OFF;
				VWD_Stop();
			}
		}
		
		if (IsGameModeEnable())
		{
			if (UserPrefGameMode == Off)
			{
				UserPrefGameMode = On;
				B_DisplayLockMethod = DISP_LOCK_NORMAL;	
				if (IsMainVGAInput() && (stInput[CH_A].Vactive == PanelHeight) )
					gm_ClearRegBitsWord(DDDS1_CONTROL, DDDS1_FORCE_OPLOOP);
				else
					gm_SetRegBitsWord(DDDS1_CONTROL, DDDS1_FORCE_OPLOOP);		     

				UserPrefStateTempFlag = INPUT_COLOR_FORMAT_STATE;
				ModeSetupRequest(gmvb_MainChannel, REQ_MS_RESTART, PARAM_NONE, 0);//20121115 yvonne to reduce time 300);		
				UserPrefAdjustInputColorFormatByOSD = TRUE;	//20120908 yvonne for not change end user select color format issue.
				//GameModeStatusChanged();
				
			}
		}
	}
}

void DisableGameMode(void)//KevinL 20090415
{
//111004 Edward return for temp , it's need to posting code of VGA_GAME_MODE_PATCH
//#pragma message "!!!!Need to implement game mode function"
//return;

#ifdef VGA_GAME_MODE_PATCH //KevinL 20090418
	if (!IsVGADisplayFrameLock())	
#endif	
	{		
		/*//120925 Edward add for OSD change disable VWD when Preset mode is "Game"
		if (UserPrefVwdDemo != VWD_DEMO_OFF)
		{
			UserPrefVwdFlag = ON;
		}
		*/
		
		if (IsAnalogInput(gmvb_MainChannel))
		{
			if(UserPrefVwdFlag == ON)
			{
				UserPrefVwdFlag = OFF;
				VWD_Stop();
			}
		}
		
		if ((UserPrefGameMode == On) && (UserPrefPipMode ==NO_PIP))
		{
			UserPrefGameMode = Off;	
			gm_WriteRegWord(SYNC_CONTROL, 2); 							  
			B_DisplayLockMethod = DISP_LOCK_DFL;
			if (IsMainVGAInput()&& (stInput[CH_A].Vactive == PanelHeight))
				gm_ClearRegBitsWord(DDDS1_CONTROL, DDDS1_FORCE_OPLOOP);
			else
				gm_SetRegBitsWord(DDDS1_CONTROL, DDDS1_FORCE_OPLOOP);		    
    
			UserPrefStateTempFlag = INPUT_COLOR_FORMAT_STATE;		
			ModeSetupRequest(gmvb_MainChannel, REQ_MS_RESTART, PARAM_NONE, 0);//20121115 yvonne to reduce time 300);			
			UserPrefAdjustInputColorFormatByOSD = TRUE; //20120908 yvonne for not change end user select color format issue.
			//GameModeStatusChanged();	

		}
	}
}

void AdjustPresetModes(void)	// Increased by ST_hansPENG 090216
{
	//if(CurrentColorPresetMode != UserPrefGraphicsImageMode)
	{	
	//120530 Edward save the Brightness / contrast / Sharpness by scheme mode
	if(IsPresetMode_GRAPHICS_TEXT)
	{
		UserPrefBrightness = UserPrefBrightness_Text;
		UserPrefContrast = UserPrefContrast_Text;
		UserPrefPortSharpness = UserPrefPortSharpness_Text;
	}
	else if(IsColorSpace_GRAPHICS_CAL1)//120806 Edward add for CAL1/CAL2 save seperate
	{
		UserPrefBrightness = UserPrefBrightness_CAL1;
		UserPrefContrast = UserPrefContrast_CAL1;
		UserPrefPortSharpness = UserPrefPortSharpness_All;
	}
	else if(IsColorSpace_GRAPHICS_CAL2)
	{
		UserPrefBrightness = UserPrefBrightness_CAL2;
		UserPrefContrast = UserPrefContrast_CAL2;
		UserPrefPortSharpness = UserPrefPortSharpness_All;
	}
	else
	{
		UserPrefBrightness = UserPrefBrightness_All;
		UserPrefContrast = UserPrefContrast_All;
		UserPrefPortSharpness = UserPrefPortSharpness_All;
	}	

	//120608 Edward save the Hue / Saturation 
	if(IsPresetMode_GRAPHICS_MOVIE)
	{
			UserPrefHue = UserPrefHue_Movie;
			UserPrefSaturation = UserPrefSaturation_Movie;
	}
	else if(IsPresetMode_GRAPHICS_GAME)
	{
			UserPrefHue = UserPrefHue_Game;
			UserPrefSaturation = UserPrefSaturation_Game;
	}
	else
	{
			UserPrefHue = 0;
			UserPrefSaturation = WB_SATURATION_DEFAULT;
	}	
	
			
 if (IsPresetMode_GRAPHICS_COLOR_SPACE)//120523 Edward
 {
 	//120822 Edward for save UserPrefDUMSwitch by preset mode
	WB_ImageScheme = ConvertPresetMode(UserPrefGraphicsImageMode);
	//gm_Print("###color space WB_ImageScheme  %d",WB_ImageScheme);
	//gm_Print("###UserPrefDCR  %d",UserPrefDCR);
	//gm_Print("###color space UserPrefDUMSwitch  %d",UserPrefDUMSwitch);
	if( !IsVwdDetected())	//20121024 Kordonm+ for VWD enable, color preset mode set to sRGB, VWD window color error
	{
		#if (FEATURE_ACM_3D == ENABLE)
			AdjustACM3DColorScheme();
		#endif
		#ifdef DELL_U2410F//120209 Edward copy from 2410
			AdjustDELLACCScheme();
		#endif
	}
	
 	CscUpdateColors(gmvb_MainChannel);//reload default gain / offset
 	#ifdef USE_GAMMMA_TABLE_FROM_NVRAM
		AdjustColorSpace_NVRAM();
	#else
 		AdjustColorSpace();
	#endif

	//if(gm_IsVideo(CH_A))
	gm_SetRegBitsWord(MVF_CTRL, MVF_INT_COEF_EN);	// Use internal coefficients          
	
	NonColorSpaceGammaPostLUT_Loaded = gmd_FALSE;
 }	
 else
 {
	#if (FEATURE_ACM_3D == ENABLE)
		AdjustACM3DColorScheme();
		//#elif (FEATURE_ACM2 == ENABLE)
		//gm_UpdateColorScheme(UserPrefImageScheme);
	#endif

	#ifdef DELL_U2410F//120209 Edward copy from 2410
		AdjustDELLACCScheme();
	#endif

	//if (IsPresetMode_GRAPHICS_ADOBE_RGB || IsPresetMode_GRAPHICS_SRGB || IsPresetMode_VIDEO_xvMode) //KevinL 20090312 for Vincent's update 
	//if(IsPresetMode_GRAPHICS_COLOR_SPACE)
		//gm_SetRegBitsWord(MVF_CTRL, MVF_INT_COEF_EN);	// Use internal coefficients            
	//else
		gm_ClearRegBitsWord(MVF_CTRL, MVF_INT_COEF_EN);	// Use external coefficients

		ReadPresetModesSettings();
	
		SetupColorSpace(gmvb_MainChannel);		
		#ifdef USE_GAMMMA_TABLE_FROM_NVRAM
			AdjustColorSpace();
		#endif
		
		//AdjustColorTemp();
	
	if (IsPreLutEnable()) //KevinL 20090304 set IMP_DITH_10TO8 if PreLut used
		gm_SetRegBitsWord(IMP_CONTROL, IMP_DITH_10TO8);
	else
		gm_ClearRegBitsWord(IMP_CONTROL, IMP_DITH_10TO8);	

	NonColorSpaceGammaPostLUT_Loaded = gmd_TRUE;
 }

	
	#ifdef CMI_73XX_BOARD//120613 Edward for DCR / Energy use
	 if(IS_DCR_ON() || IS_ENERGY_SMART_ON())//120630 Edward for DCR / Energy on initial backlight value
	 {
	 	//if((B_PresetModesStore != Graphics_GAME) && )
	  	DCR_EnergyUse_Brightness = UserPrefBrightness;	
		
		#if (defined(CMI_73XX_BOARD)&&(PWM_LowLevel_Smooth))
		if(IS_ENERGY_SMART_ON()) //20120913 Edward
		  {
		  	if(DCR_EnergyUse_Brightness <= WB_Brigntness_OSD_DCR_Start)
				DCR_Energy_LowStep_Brightness = DCR_EnergyUse_Brightness * FineStep_DCR_PWM;
			
			//gm_Print("##DCR_EnergyUse_Brightness  %d",DCR_EnergyUse_Brightness);
		  }
		#endif
	 }		
	#endif


	
	AdjustSharpness();//120529 Edward configure sharpness due to Text mode have different setting	
	AdjustBackLight();//120928 Edward configure Brightness due to Text mode have different setting
	//CurrentColorPresetMode = UserPrefGraphicsImageMode;
	}
}

void AdjustColorMode(void)
{
	if (UserPrefAspectRatioMain != AR_FULL_SCREEN)
		AdjustAspectRatioMain();
	AdjustPresetModes();

#ifdef VGA_GAME_MODE_PATCH //KevinL 20090420
	if (!IsVGADisplayFrameLock())//Michael
#endif
	{
       	if(IsGameModeEnable())
	    	EnableGameMode();
		else
	    	DisableGameMode();
	}
}

//&*&*&*S 20120315:SWRD4001: Kordonm(0)  Modify for OSD Workbench
void AdjustPrinterPrevue(void)
{

}
//&*&*&*E 20120315 : Kordonm(1)  Modify for OSD Workbench
#if (FEATURE_SOURCE_SCAN == ENABLE)
//==========Source scan=================





#define SS_DEBUG
#ifdef SS_DEBUG
	#define	msg_SS(a,b)            gm_Print(a,b)
#else
	#define	msg_SS(a,b)
#endif

BYTE SourceScanStatus =0;
BYTE ScanSourceResult = 0;
BYTE ScanPort = 0;
BYTE SaveMainPortBeforeScan=0;
BYTE SavePipPortBeforeScan=0;
SBYTE SavePipMode=NO_PIP;	
BYTE StoreOsdState=0; //KevinL 20090119
BYTE SaveVWDStatus = 0;			

#define VGA_WAIT		160
#define CVBS_WAIT		150
#define COMPENT_WAIT	150
#define HDMI_WAIT		200

void far StartSourceScan(void)		//StKam081014		WB will check main port must has sync before scan port  
{
	SetAudioDACOutput(OFF);
	SaveVWDStatus = UserPrefVwdFlag;
	if(UserPrefVwdFlag != OFF)
	{
		UserPrefVwdFlag = OFF;
		VWD_Stop();
	}
	ScanSourceResult = 0;	//Clean All
	SourceScanStatus = BeginScanSource;
	SetDisplayOff(gmvb_PipChannel, BACKGROUND_ONLY_NO_SYNC_UPDATE);
	SetDisplayOff(gmvb_MainChannel, BACKGROUND_ONLY_NO_SYNC_UPDATE);
	SetDisplayOff(CH_B, BACKGROUND_ONLY_NO_SYNC_UPDATE);
	gm_TimerStart(ScanSource_10ms_TMR, 10);
	SaveMainPortBeforeScan = UserPrefCurrentInputMain;
	SavePipPortBeforeScan = UserPrefCurrentInputPip;	
	SavePipMode = UserPrefPipMode;

        if(stInput[gmvb_MainChannel].ModeFlags&gmd_MODE_COMPLETE)		//Current port valid 
		B_NoMSGAfterSS = gmd_TRUE;
	else
		B_NoMSGAfterSS = gmd_FALSE;

	ScanPort = 1;		
#ifdef TOUCHPAD_SO381000
	TouchPadOff = TRUE;
#endif
	msg_SS("~~~~ Source scan  start %d",gm_GetSystemTime());
}

void far EndSourceScan(void)		
{
	UserPrefScanResult = ScanSourceResult;

#ifdef TOUCHPAD_SO381000		
	TouchPadOff = FALSE;					//enable touch pad use
#endif

	SourceScanStatus = FinishScanSource; //Neil120811* move up, change port function will check this condition//KevinL 20090320
	UserPrefCurrentInputMain = SaveMainPortBeforeScan;
	AdjustCurrentInputMain();
   	//gm_AUDIO_ReStartDetection();
	ModeSetupRequest(gmvb_MainChannel, REQ_MS_RESTART, PARAM_NONE, REQUEST_NOW);			
	if(SaveVWDStatus == ON)
		UserPrefVwdFlag = SaveVWDStatus;
	//VWD_Start(); 	will start in ModeSetupEnd();
	B_NoMainIconFlag = gmd_TRUE;
	

	if (SavePipMode > NO_PIP)
	{
		UserPrefCurrentInputPip = SavePipPortBeforeScan;		//whatever ResourceResolver() did, just change to saved port since we doing port scan
		//AdjustCurrentInputPip();	//Neil 20120606- AdjustPipMode() will setup input
		UserPrefPipMode =	SavePipMode;		
		AdjustPipMode();	
		SelectInput(gmvb_PipChannel, UserPrefCurrentInputPip, NORMAL_MODE);
		if (UserPrefPipMode != NO_PIP) //Neil120810* 
		{
			gm_TimerStart(Display_Enable_Pxp_10ms_TMR, 2);
			//SetDisplayOn(gmvb_PipChannel);
		}
	}

	//StKam081014		source scan has a conflict on port rule , some port will reject by select port check and HDMI mux does work well
	/*
	if (UserPrefCurrentInputMain==ALL_BOARDS_HDMI)
		gm_HdmiChannelSwitch(0);
	if ((UserPrefCurrentInputMain == ALL_BOARDS_HDMI2) || (UserPrefCurrentInputMain == ALL_BOARDS_HDMI3))
		gm_HdmiChannelSwitch(1);
	*/
	//SetDisplayOn(gmvb_MainChannel);

	SetOsdTimeout();		//Neil120808+
	OSDRequestCancelMsg(gmvb_MainChannel, REQ_ANY);
	OSDRequestCancelMsg(gmvb_PipChannel, REQ_ANY);	

	if (UserPrefStateTempFlag == ENTER_POWER_SAVING)
		gm_BlenderOnOff(gmvb_MainChannel, Off);

	UserPrefGlobalLCDOnOffFlag = FALSE;
	gm_TimerStart(Display_Enable_Main_10ms_TMR,  DisplayEnableDelay);

	if (UserPrefAutoAdjustJump == 1) //KevinL 20090406 from Input Hot Key Menu only
	{
		if ((gm_ReadRegWord(PB_CTRL) & PB_MAIN_ENABLE)) //KevinL 20090221 for no cable/Sync in all source than apply source scan ,OSD can't display issue.
			gm_ClearRegBitsWord(PB_CTRL, PB_MAIN_ENABLE);	   
	}
	msg_SS("~~~~ Source scan  end time %d",gm_GetSystemTime());
}

void far SourceScanHandler(void)
{	
	BYTE RetryCount;

	if (SourceScanStatus && (gm_TimerCheck(ScanSource_10ms_TMR) == TIMER_TMO))
	{
		switch (SourceScanStatus)
		{
			case BeginScanSource:
			{
				if ( ScanPort == 1)		
				{
					UserPrefCurrentInputMain = ALL_BOARDS_VGA1;
					gm_TimerStart(ScanSource_10ms_TMR, VGA_WAIT);
				}
				/*
				if ( ScanPort == 2)
				{
					UserPrefCurrentInputMain = ALL_BOARDS_CVBS1;	
					gm_TimerStart(ScanSource_10ms_TMR, CVBS_WAIT);					
				}
				if ( ScanPort == 3)		
				{
					UserPrefCurrentInputMain = ALL_BOARDS_COMP1;					
					gm_TimerStart(ScanSource_10ms_TMR, COMPENT_WAIT);					
				}
				*/
				if ( ScanPort == 4)		
				{	
				#if 0
					gm_HdmiChannelSwitch(0);
					gm_Delay1ms(500);

					if (gm_ReadRegByte(DVI_STATUS) & DVI_LINK_CLK_DETECTED) 
					{
						ScanSourceResult |=  0x08;						
						msg_SS(" HDMI OK",0);
					}
					else
					{
						msg_SS("HDMI FAIL , try another way",0);
						UserPrefCurrentInputMain=ALL_BOARDS_HDMI;	
						gm_TimerStart(ScanSource_10ms_TMR, HDMI_WAIT);					
					}
				#else
					//if (IsDPCableConnected() == TRUE)
					/* //Neil 20120530- Start from current input, don't need force to change to DP, buy our some times
					if(isCableConnected(DP1))
					{
						UserPrefCurrentInputMain = ALL_BOARDS_DP1;
						gm_TimerStart(ScanSource_10ms_TMR, 60);	
					}
					*/
				#endif
				}				
				// if port <5 that meaning scan finish 
				if (ScanPort < 5)
				{
					//if (ScanPort!=4)	//HDMI not need to change port 

						if (ScanPort==1)
							UserPrefGlobalLCDOnOffFlag = TRUE;

						AdjustCurrentInputMain();

					gm_TimerStart(ScanSource_10ms_TMR, 60);	
					SourceScanStatus = CheckScanSource;
				}
				else
				{
					if (UserPrefAutoAdjustJump == 0) //Source Scan from Main Menu
						gm_TimerStart(ScanSource_10ms_TMR, HDMI_WAIT); //KevinL 20090320 wait ori-input timing stable than display Main Menu
					EndSourceScan();
				}
				break;
			}

			case CheckScanSource:
			{
				if (ScanPort == 1)	//VGA
				{
					for (RetryCount = 0; RetryCount < 4; RetryCount++)
					{
						if (gm_ReadRegWord(MAIN_INPUT_IRQ3MASK) & IMP_VS_MASK)
						{
							ScanSourceResult |= VGABit;
							//msg_SS("~~~~ VGA OK",0);
							break;
						}
						else
						{
							gm_Delay100ms(1);
							//msg_SS("~~~~ VGA FAIL -Retry %d ",RetryCount);
						}
					}	
				#if 0
					if ((ScanSourceResult & BIT0) == 0)
					{
						WORD IFM1 = 0;
						WORD IFM2 = 0;
						WORD IFM3 = 0;						
						IFM1 = gm_ReadRegWord(MIFM_HS_PERIOD);
						gm_Delay10ms(10);								
						IFM2 = gm_ReadRegWord(MIFM_HS_PERIOD);
						gm_Delay10ms(10);								
						IFM3 = gm_ReadRegWord(MIFM_HS_PERIOD);

						if ((abs(IFM1-IFM2) < 2) && (abs(IFM2-IFM3)<2))
						{
							ScanSourceResult |= 0x01;
							gm_Print("~~~~ VGA1 OK",0);
						}
					}
				#endif
					ScanPort = 4;	
					SourceScanStatus = BeginScanSource;					
					break;
				}					

				/*
				if (ScanPort == 2)	//CVBS
				{
					for (RetryCount = 0; RetryCount < 4; RetryCount++)
					{
						if (gm_ReadRegWord(MAIN_INPUT_IRQ3MASK) & IMP_VS_MASK)
						{
							ScanSourceResult |= 0x40;						
							msg_SS("~~~~ CVBS OK",0);
							break;							
						}
						else
						{
							gm_Delay100ms(1);							
							//msg_SS("~~~~ CVBS FAIL -Retry %d",RetryCount); 		
						}
					}
					ScanPort = 3;		
					SourceScanStatus = BeginScanSource;					
					break;
				}	


				if (ScanPort == 3)	//COMP
				{
					for (RetryCount = 0; RetryCount < 4; RetryCount++)
					{
						if (gm_ReadRegWord(MAIN_INPUT_IRQ3MASK) & IMP_VS_MASK)
						{
							ScanSourceResult |= 0x80;						
							msg_SS("~~~~ COMP OK",0); 
							break;							
						}
						else
						{
							gm_Delay100ms(1);							
							//msg_SS("~~~~ COMP FAIL -Retry %d",RetryCount); 		
						}
					}
					ScanPort = 4;		
					SourceScanStatus = BeginScanSource;					
					break;					
				}
				*/
				if (ScanPort == 4)	
				{
				   BYTE OrgInputPortMain=UserPrefCurrentInputMain;
				   BYTE OrgInputPortPip=UserPrefCurrentInputPip;
				   BYTE i;
               //gm_Print("***ORG @(%d,%d)",UserPrefCurrentInputMain,UserPrefCurrentInputPip);


				   for(i=0;i<NUM_CHANNELS;i++)
				   {
                  switch(gB_CurrentInputConnector[i])
                  {
                     case VGA1:
                        if (gm_ReadRegWord(MAIN_INPUT_IRQ3MASK) & IMP_VS_MASK)
                        {
                           ScanSourceResult |= VGABit;
                           //msg_SS("~~~ VGA OK",0);
                        };
                        break;
                  
                     case HDMI1:
			#if (LINK_CLOCK_CHECK_SUPPORT ==1) 
                        if(HdmiDviIsLinkClockPresent(HDMI1))       
			#endif							
                        {
                           ScanSourceResult |= HDMIBit;  
                           //gm_Print("~~~ HDMI OK",0);                
                        }
                        break;
                  
                  
                     case DVI1:
			#if (LINK_CLOCK_CHECK_SUPPORT ==1) 
                        if(HdmiDviIsLinkClockPresent(DVI1))                
			#endif
                        {
                           ScanSourceResult |= DVIBit;   
                           //msg_SS("~~~ DVI 1 OK",0);               
                        }
                        break;                     
                  
                  
                     case DP1:
                        if (isSourcePowerConnected(DP1))
                        {                       
                           //msg_SS("~~~ DP Ok ",0);                
                           ScanSourceResult |=  DPBit;                                             
                        }
                        break;
                  //#pragma message "!!!!Need to complete source scan??"
                  /*
                     case DP3:
                        if (isSourcePowerConnected(DP3))
                        {                       
                           msg_SS("~~~ Mini DP Ok ",0);                
                           ScanSourceResult |= DP3Bit;                                             
                        }
                        break;
                        */
                  }

				   }

				
					if((ScanSourceResult & VGABit) == 0)	//If first time check vga source fail, check it here again to avoid missing vga
					{
						if (gm_ReadRegWord(MAIN_INPUT_IRQ3MASK) & IMP_VS_MASK)
						{
							ScanSourceResult |= VGABit;
							msg_SS("~~~~ VGA OK 2",0);
							//break;
						}
						else
							msg_SS("~~~~ VGA FAIL 2",0);
					}


					//HDMI
					if((isCableConnected(HDMI1)) && (OrgInputPortMain != HDMI1) && (OrgInputPortPip != HDMI1))
					{
						//HdmiDviSetSkipHpdPulseFlag(HDMI1,TRUE);
   					//	SelectInput(gmvb_MainChannel, HDMI1, NORMAL_MODE);//Neil120618* don't disable it. somehow HDMI need call this function!!
						//HdmiDviSetSkipHpdPulseFlag(HDMI1,FALSE);
						UserPrefCurrentInputMain=HDMI1;
						UserPrefCurrentInputPip = MovePortFromConflictPort(UserPrefCurrentInputMain);
						AdjustCurrentInputPip();
						AdjustCurrentInputMain();		
						//gm_Print("***Hdmi@(%d,%d)",UserPrefCurrentInputMain,UserPrefCurrentInputPip);
						#if 1
   						#if (defined(CMI_73XX_BOARD))
						gm_TimerStart(WaitDelay_100ms_TMR,30);
						while(gm_TimerCheck(WaitDelay_100ms_TMR) != TIMER_TMO)
						{
							#if (LINK_CLOCK_CHECK_SUPPORT ==1) 
							if(HdmiDviIsLinkClockPresent(HDMI1))						
							{
								ScanSourceResult |= HDMIBit;	
								//msg_SS("~~~~ HDMI OK",0); 					
								break;							
							}
							else
							{
								HdmiDviHandler();
								//gm_Delay1ms(10);	
								//gm_Delay1ms(100);//120920 Kevin Chen for Chroma in China							
								//gm_Print("~~~~ HDMI FAIL -Retry %d", RetryCount); 		
							}
							#endif
						}
						#else
      						gm_TimerStart(WaitDelay_100ms_TMR,30);
      						while(gm_TimerCheck(WaitDelay_100ms_TMR) != TIMER_TMO)
      						{
							#if (LINK_CLOCK_CHECK_SUPPORT ==1) 
      							if(HdmiDviIsLinkClockPresent(HDMI1))		
							#endif
      							{
      								ScanSourceResult |= HDMIBit;	
      								//msg_SS("~~~~ HDMI OK",0); 					
      							}
     								HdmiDviHandler();
     								gm_Delay1ms(1);	
   						}
						#endif
						#else
						for (RetryCount = 0; RetryCount < 15; RetryCount++)		//Neil120618* org = 30x10x15 = 4500ms = 4.5s,too long
						{						
							if(HdmiDviIsLinkClockPresent(HDMI1))						
							{
								ScanSourceResult |= HDMIBit;	
								gm_Print("~~~~ HDMI OK",0); 					
								break;							
							}
							else
							{
								HdmiDviHandler();
								//gm_Delay1ms(5);	
								gm_Delay1ms(100);//120920 Kevin Chen for Chroma in China							
								gm_Print("~~~~ HDMI FAIL -Retry %d", RetryCount); 		
							}
						}
						#endif
					}

					//DVI1
               if((isCableConnected(DVI1)) && (OrgInputPortMain != DVI1) && (OrgInputPortPip != DVI1))
					{
                  //HdmiDviSetSkipHpdPulseFlag(DVI1,TRUE);
                  // SelectInput(gmvb_MainChannel, DVI1, NORMAL_MODE);     //Neil120618* don't disable it. somehow DVI need call this function!!
                  //HdmiDviSetSkipHpdPulseFlag(DVI1,FALSE);
                  UserPrefCurrentInputMain=DVI1;
                  UserPrefCurrentInputPip = MovePortFromConflictPort(UserPrefCurrentInputMain);
                  AdjustCurrentInputPip();
                  AdjustCurrentInputMain();
                  //gm_Print("***DVI1@(%d,%d)",UserPrefCurrentInputMain,UserPrefCurrentInputPip);
						#if 1
                  gm_TimerStart(WaitDelay_100ms_TMR,30);
						while(gm_TimerCheck(WaitDelay_100ms_TMR) != TIMER_TMO)
						{
							#if (LINK_CLOCK_CHECK_SUPPORT ==1) 
							if(HdmiDviIsLinkClockPresent(DVI1))						
							{
								ScanSourceResult |= DVIBit;	
                        //msg_SS("~~~~ DVI 1 OK",0);                
								break;							
							}
							else
							{
								HdmiDviHandler();
                        //gm_Delay1ms(10);   
                        //gm_Print("~~~~ DVI 1 FAIL -Retry %d",RetryCount);      
							}
							#endif
						}
						#else
						for (RetryCount = 0; RetryCount < 5; RetryCount++)
						{
							if(HdmiDviIsLinkClockPresent(DVI1))// Neil120618* Org = DVI0_LINK_CLK_DETECTED)DVI0_LINK_CLK_DETECTED will be set even no signal 
							{
								ScanSourceResult |=  DVIBit;	
								gm_Print("~~~~ DVI 1 OK",0); 					
								break;							
							}
							else
							{								
								HdmiDviHandler();
								gm_Delay1ms(5);
								gm_Print("~~~~ DVI 1 FAIL -Retry %d",RetryCount); 		
							}
						}	
						#endif
					}


					#if 0	//Neil 20120526- no DVI2 for U2413
					//DVI2
					if (IsDVI2CableConnected() == 1)
					{
						//gm_HdmiChannelSwitch(1);					
						SwitchDviInput(ALL_BOARDS_DVI2);	
						gm_Delay100ms(10);	//for separate DVI1 signal					
						//gm_HdmiReset();				

						for (RetryCount = 0; RetryCount < 20; RetryCount++)
						{
							if (gm_ReadRegByte(HDRX_MAIN_LINK_STATUS) & HDRX_LINK_CLK_DETECTED) 
							{
								ScanSourceResult |= 0x04;	
								msg_SS("~~~~ DVI 2 OK",0); 					
								break;							
							}
							else
							{
								gm_Delay10ms(15);
								msg_SS("~~~~ DVI 2 FAIL -Retry %d",RetryCount); 		
							}
						}
					}
					#endif
					//DP
					//if (IsDPCableConnected() == 1)
					if((isCableConnected(DP1)) && (OrgInputPortMain != DP1) && (OrgInputPortPip != DP1))  
					{
						WORD i;
						//msg_SS("~~~~ DP cable connected !!",0);
   						//SelectInput(gmvb_MainChannel, DP1, NORMAL_MODE);
						//ResetDPRXInit();		//Neil120711+ reset otherwise can't detect twice.//120720 Edward disable temp
						//Input_DP_Switch_Select(DP_NORMAL);
						
						#if 0//def Qisda_93XX_BOARD //20120810 Add by KevinChen for Scan DP source test.
							gm_WriteRegWord(DP12RX0_DPCD_STATUS,0xffff);
							for(i=0;i<2250;i++)
							{
								if((DpRxIsLT_Success(DP1)==1)||((gm_ReadRegWord(DP12RX0_DPCD_STATUS)&0x2222)!=0))
								{
									break;
								}
								DpAppHandler();
							}
						gm_Print("%d",i);
						#endif	 
						if (isSourcePowerConnected(DP1))
							{								
								//msg_SS("~~~~ DP Ok ",0); 					
								ScanSourceResult |=  DPBit;															
							}
							else
							{				
								//msg_SS("~~~~ DP FAIL ",0); 		
							}					
						/*for (RetryCount = 0; RetryCount < 5; RetryCount++)
						{
							#if 1
							    gm_WriteRegWord(DP12RX0_DPCD_STATUS,0xffff);//20120810 Add by KevinChen for Scan DP source test.
								if(DpRxIsLT_Success(DP1)||(gm_ReadRegWord(DP12RX0_DPCD_STATUS)&0x2222))
								{
									msg_SS("~~~~ DP Ok 0x%x",gm_ReadRegWord(DP12RX0_DPCD_STATUS)); 					
									ScanSourceResult |=  DPBit;
									break;
								}
								else
								{
									msg_SS("~~~~ DP FAIL 0x%x",gm_ReadRegWord(DP12RX0_DPCD_STATUS));
								}
								msg_SS("~~~~ DP status %d",DpRxIsStableState(DP1));
							#else
							if ((gm_ReadRegWord(DP12RX0_DPCD_STATUS) == 0x2222) && (gm_ReadRegWord(DP12RX0_MS_HTOTAL) != 0))
							{
								if ((gm_ReadRegWord(DP12RX0_BE_IRQ_STATUS)&DPRX_NO_BS_STS)==0) 
								{
									msg_SS("~~~~ DP Ok 0x%x",gm_ReadRegWord(DP12RX0_DPCD_STATUS)); 					
									ScanSourceResult |=  DPBit;
									break;
								}
							}
							else
							{
								gm_Delay100ms(1);							
								msg_SS("~~~~ DP FAIL 0x%x",gm_ReadRegWord(DP12RX0_DPCD_STATUS)); 					
								msg_SS("~~~~ DP FAIL -Retry %d",RetryCount); 		
							}
							#endif
						}*/
					}
					else
						//msg_SS("~~~~ DP cable not connected %d!!",(isCableConnected(DP1)));
					#if 0
					if((isCableConnected(DP3)) && (OrgInputPortMain != DP3) && (OrgInputPortPip != DP3))
					{
					    WORD j;
						msg_SS("~~~~ mini DP cable connected !!",0);
						
   						//SelectInput(gmvb_MainChannel, DP3, NORMAL_MODE);
						//ResetDPRXInit();		//Neil120711+ reset otherwise can't detect twice.//120720 Edward disable temp
         				//Input_DP_Switch_Select(DP_MINI);
         				
						#if 0//def Qisda_93XX_BOARD //20120810 Add by KevinChen for Scan DP source test.
							gm_WriteRegWord(DP12RX0_DPCD_STATUS,0xffff);
							for(j=0;j<2250;j++)
							{
								if((DpRxIsLT_Success(DP3)==1)||((gm_ReadRegWord(DP12RX0_DPCD_STATUS)&0x2222)!=0))
								{
									break;
								}
								DpAppHandler();
							}
						gm_Print("%d",j);
						
						if (isSourcePowerConnected(DP3))
							{								
								msg_SS("~~~~ Mini DP Ok ",0); 					
								ScanSourceResult |= DP3Bit;															
							}
							else
							{				
								msg_SS("~~~~ Mini DP FAIL ",0); 		
							}
							#endif	
						/*for (RetryCount = 0; RetryCount < 5; RetryCount++)
						{
							#if 1								
								gm_WriteRegWord(DP12RX0_DPCD_STATUS,0xffff);//20120810 Add by KevinChen for Scan DP source test.
								if(DpRxIsLT_Success(DP3)||(gm_ReadRegWord(DP12RX0_DPCD_STATUS)&0x2222))
								{
									msg_SS("~~~~ DP3 Ok 0x%x",gm_ReadRegWord(DP12RX0_DPCD_STATUS)); 					
									ScanSourceResult |=  DP3Bit;
									break;
								}
								else
								{
									msg_SS("~~~~ DP3 FAIL 0x%x",gm_ReadRegWord(DP12RX0_DPCD_STATUS)); 					
								}
								msg_SS("~~~~ mini DP status %d",DpRxIsStableState(DP3));
							#else
							if ((gm_ReadRegWord(DP12RX0_DPCD_STATUS) == 0x2222) && (gm_ReadRegWord(DP12RX0_MS_HTOTAL) != 0))
							{
								if ((gm_ReadRegWord(DP12RX0_BE_IRQ_STATUS)&DPRX_NO_BS_STS)==0) 
								{
									msg_SS("~~~~ Mini DP Ok ",0); 					
									ScanSourceResult |=  DP3Bit;
									break;
								}
							}
							else
							{
								gm_Delay100ms(1);							
								msg_SS("~~~~ Mini DP FAIL -Retry %d",RetryCount); 		
							}
							#endif
						}*/
					}
					#endif
					//EndSourceScan();	
					ScanPort = 5;	//KevinL 20090320	
					SourceScanStatus = BeginScanSource;	
					UserPrefCurrentInputMain=OrgInputPortMain;
					UserPrefCurrentInputPip =OrgInputPortPip; //MovePortFromConflictPort(UserPrefCurrentInputMain);
					AdjustCurrentInputPip();
					AdjustCurrentInputMain();								
					break;
				}				
			}

			case FinishScanSource:
				OSDRequestMsg(gmvb_MainChannel, MTO_ScanFinished, AUTOSCAN_DELAY); //KevinL 20090320 
				SourceScanStatus = StopScanSource;
				break;

			default: //KevinL for StopScanSource status
				EndSourceScan();			
				//gm_Print("---SourceScan Fail---",0);
				break;
		}
	}
}
#endif
void LED_Handler(void)
{
#ifdef TOUCH_KEY//Lvjie 081013
	//SourceLedControl();
	//LedLightUpHandler();
	//Neil120923+ for factory mode LED off sometimes
	if((GetPowerState()== Pwr_On)||(GetPowerState()== Pwr_Sleep))
	{
		if((UserPrefFactoryFlag == gmd_TRUE)&&!IsFactoryKeyState())
			SetOSDMenuState(FIVE_KEYSTATE);
	}
#endif
{
	static BYTE LED_OnOff = OFF;
	
	switch(LED_State)
	{
		default:
		case LED_IDLE:
		{
			if(LED_OnOff == OFF)
			{
				if(gm_TimerCheck(LED_BLINKING_100ms_TMR) == TIMER_TMO)
				{
					LED_OnOff = ON;
					gm_TimerStart(LED_BLINKING_100ms_TMR,20);
					gm_SetRegBitsWord(LED_BLINK_PORT,LED_BLINK_PIN);
				}

			}
			else
			{
				if(gm_TimerCheck(LED_BLINKING_100ms_TMR) == TIMER_TMO)
				{
					LED_OnOff = OFF;
					gm_TimerStart(LED_BLINKING_100ms_TMR,15);
					gm_ClearRegBitsWord(LED_BLINK_PORT,LED_BLINK_PIN);
				}
			}
		}
		break;
		
		case LED_BUSY:
		{
			if(LED_OnOff == OFF)
			{
				if(gm_TimerCheck(LED_BLINKING_100ms_TMR) == TIMER_TMO)
				{
					LED_OnOff = ON;
					gm_TimerStart(LED_BLINKING_100ms_TMR,5);
					gm_SetRegBitsWord(LED_BLINK_PORT,LED_BLINK_PIN);
				}
			}
			else
			{
				if(gm_TimerCheck(LED_BLINKING_100ms_TMR) == TIMER_TMO)
				{
					LED_OnOff = OFF;
					gm_TimerStart(LED_BLINKING_100ms_TMR,5);
					gm_ClearRegBitsWord(LED_BLINK_PORT,LED_BLINK_PIN);
				}
			}

		}
		break;
	}

}


}

#ifdef TOUCHPAD_SO381000
DWORD NoneKeyTime;

void NoneKeyHandler(void)
{
#if 0
	if (PwrState != Pwr_On)
		return;
#endif
	if (IsKeyPress() == FALSE)
	{
		if (NoneKeyTime == 0)
			NoneKeyTime = gm_GetSystemTime();	
		else if ((gm_GetSystemTime() - NoneKeyTime) > 500)
		{
			OSDRequestMsg(gmvb_MainChannel, MTO_NoneKey, REQUEST_NOW);	
			NoneKeyTime = gm_GetSystemTime();	
		}
	}
	else
		NoneKeyTime = gm_GetSystemTime();	
}

 BOOL IsKeyPress(void)
{
	BYTE k, B_Temp;
	BOOL l = FALSE;

	for (k = 0; k < 3; k++)
	{
		B_Temp = TouchPadGetValue(k);
		if (KeyDataPresent(B_Temp))
			l = TRUE;
	}
	if (l)
		return TRUE;
	else
 		return FALSE;
}
#endif

void SourceLedControl(void)
{
};


void BurnInHandler(void)
{
	WORD W_Reg;
	static BYTE BurnInState;

	//calo add for when exit factory mode ,set the burn in off 
	// if (UserPrefFactoryFlag == FALSE)		//Neil 20081119- for burn in control
	//	UserPrefRunBurnInModeFlag = FALSE;
	#if(FEATURE_SOURCE_SCAN == ENABLE)
	if((SourceScanStatus == BeginScanSource)||(SourceScanStatus == CheckScanSource))	//Neil 20090506* for burn in stop after scan source
		return;
	#endif
	
	if (BurnInState != 0 &&UserPrefBurnInModeFun == 0)
	{
		gm_WriteRegWord(PB_BKGND, RGB_565(MAIN_BACKGROUND_COLOR));		//Neil 20120604*  Use PB_BKGND instead of MAIN_AUTO_BKGND_COLOR now, otherwise BKG may show colors after exit
		//gm_SetRegBitsWord(PB_CTRL, PB_MAIN_ENABLE);
		gm_WriteRegWord(PB_MAIN_HWIDTH, PanelWidth);
		gm_WriteRegWord(PB_MAIN_VHEIGHT, PanelHeight);			
		gm_WriteRegWord(PB_MAIN_HSTART, 0);
		gm_WriteRegWord(PB_MAIN_VSTART, 0);	

		gm_SyncUpdateWait(gmvb_MainChannel, OUTPUT_CLOCK);
		BurnInState = 0;
	}

	if ((UserPrefRunBurnInModeFlag == 0) || (UserPrefPipMode != NO_PIP) || gm_IsValidSignal(gmvb_MainChannel))	//Neil 20081119+ for valid signal just return; for AC off/on no image output if burnin flag =1
		return;

   
    if (UserPrefRunBurnInModeFlag == TRUE) 	   
       UserPrefFactoryFlag = TRUE;
	
	if (!IsGraphicInput(UserPrefCurrentInputMain))
	{
		if (UserPrefBurnInModeFun != 0)
		{
			UserPrefBurnInModeFun = 0;
			gm_WriteRegWord(PB_BKGND, RGB_565(MAIN_BACKGROUND_COLOR));
			//gm_SetRegBitsWord(PB_CTRL, PB_MAIN_ENABLE);
			gm_WriteRegWord(PB_MAIN_HWIDTH, PanelWidth);
			gm_WriteRegWord(PB_MAIN_VHEIGHT, PanelHeight);			
			gm_WriteRegWord(PB_MAIN_HSTART, 0);
			gm_WriteRegWord(PB_MAIN_VSTART, 0);	

			gm_SyncUpdateWait(gmvb_MainChannel, OUTPUT_CLOCK);
		}
		return;
	}

	if (UserPrefBurnInModeFun != 0)
		BurnInState = UserPrefBurnInModeFun;
	
	if (gmvb_MainChannel == CH_A)
		W_Reg = PB_BKGND;//MAIN_AUTO_BKGND_COLOR; //120514 Edward for burn in reg change
	else
		W_Reg = PIP_AUTO_BKGND_COLOR;

	if ((gm_TimerCheck(BURNIN_100ms_TMR) == TIMER_TMO) && (UserPrefBurnInModeFun != 0))
	{
		//   SetBacklightPWMPulse(DCR_NORMAL_PWM_PULSE_MAX); //calo update for max PWM when in burn in frame 090323

		gm_TimerStart(BURNIN_100ms_TMR, 30); 

		if (UserPrefBurnInModeFun == 1) // R ==> G
		{
			UserPrefBurnInModeFun = 2;
			gm_WriteRegWord(W_Reg, 0x07E0);
		}
		else if (UserPrefBurnInModeFun == 2) // G ==> B
		{
			UserPrefBurnInModeFun = 3;
			gm_WriteRegWord(W_Reg, 0x001F);
		}
		else if (UserPrefBurnInModeFun == 3) // B ==> Black
		{
			UserPrefBurnInModeFun = 4;
			gm_WriteRegWord(W_Reg, 0x0);
		}			
		else if (UserPrefBurnInModeFun == 4) // Black ==> W
		{
			UserPrefBurnInModeFun = 5;
			gm_WriteRegWord(W_Reg, 0xFFFF);
		}
		else // if(UserPrefBurnInModeFun == 5) // W ==> R
		{
			UserPrefBurnInModeFun = 1;
			gm_WriteRegWord(W_Reg, 0xF800);
		}			

		if (W_Reg == PB_BKGND)
		{
			//gm_SetRegBitsWord(PB_CTRL, PB_MAIN_ENABLE);			
			gm_ClearRegBitsWord(PB_CTRL, PB_MAIN_ENABLE);//120514 Edward for force bkg
			
			gm_WriteRegWord(PB_MAIN_HWIDTH, PanelWidth);
			gm_WriteRegWord(PB_MAIN_VHEIGHT, PanelHeight);			
			gm_WriteRegWord(PB_MAIN_HSTART, 0);
			gm_WriteRegWord(PB_MAIN_VSTART, 0);					
		}
		else
		{
			gm_SetRegBitsWord(PB_CTRL, PB_PIP_ENABLE|PB_SINGLE_PIP);
			gm_WriteRegWord(PB_PIP_HWIDTH, PanelWidth);
			gm_WriteRegWord(PB_PIP_VHEIGHT, PanelHeight);
			gm_WriteRegWord(PB_PIP_HSTART, 0);
			gm_WriteRegWord(PB_PIP_VSTART, 0);								
		}

		gm_SyncUpdateWait(gmvb_MainChannel, OUTPUT_CLOCK);
	#if 0
		if (gm_ReadRegWord(PB_CTRL) && PB_FORCE_MAIN_BKGND)
		{
			//gm_ClearRegBitsWord(PB_CTRL, PB_FORCE_MAIN_BKGND);		
			gm_SetRegBitsWord(PB_CTRL, PB_FORCE_MAIN_BKGND);
			gm_SyncUpdateWait(gmvb_MainChannel, OUTPUT_CLOCK);
		}
	#endif
	}
}

#ifdef  DELL_U2410F
void DiagnosticTestPatternTimerHandler(void)
{
	if (gm_TimerCheck(DiagnosticTestPattern_1s_TMR) == TIMER_TMO)
	{
		//gm_Print("~~~~~ DiagnosticTestPattern !!", 0);
		gm_TimerStop(DiagnosticTestPattern_1s_TMR);
		OSDRequestMsg(gmvb_MainChannel, MTO_DiagnosticTestPattern, REQUEST_NOW);	
	}
}
#endif

void MonitorOnTimeHandler(void)
{
	if(gm_TimerCheck(Monitor_OnTime_1s_TMR) == TIMER_TMO)
		{
			gm_TimerStart(Monitor_OnTime_1s_TMR, 60);

			//Calo 20090507+ for hours and minutes be 0 sometimes
			if((UserPrefTimeUsedMin == 0)&&(UserPrefTimeUsedHour == 0))	
			{
				NVRam_ReadBuffer(FactorySettingStart+TimeUsedMin_offset,(BYTE *)&UserPrefTimeUsedMin,1);
				NVRam_ReadBuffer(FactorySettingStart+TimeUsedHour_offset,(BYTE *)&UserPrefTimeUsedHour,2);
			}

			if((gm_ReadRegWord(PANEL_POWER_STATUS) & PANEL_STATUS) == 0x3)
				UserPrefTimeUsedMin+=1;
			
			if(UserPrefTimeUsedMin > 59)
				{
				UserPrefTimeUsedMin= 0;
				UserPrefTimeUsedHour+=1;
				}			
			
			#if (defined(Qisda_93XX_BOARD))//20120830 Add by KevinChen for Qisda request every 5 min save value when BurnIn mode on.
			 	if(UserPrefRunBurnInModeFlag && (UserPrefTimeUsedMin % 5 == 0))
            #else
				//calo add for save,one minute save one time when in burn in on ,others save one time every 20mins
				if(UserPrefRunBurnInModeFlag)
            #endif			
				SaveFactorySettingsInNVR();
			else if((!UserPrefRunBurnInModeFlag) && (UserPrefTimeUsedMin % 20 == 0))
				SaveFactorySettingsInNVR();
		}
}

#ifdef UART_DEBUG_MODE
void	DebugOSD(BYTE state)
{
	#if 1//def WorkbenchPrintMessage
	switch (state)
	{
		case 0:		gm_Print("~~~~Enter DefaultState~~~~",0); break;
		case 1:		gm_Print("~~~~Exit DefaultState~~~~",0); break;
		case 2:		gm_Print("~~~~Enter ValidMode_StateState~~~~",0); break;
		case 3:		gm_Print("~~~~Exit ValidMode_StateState~~~~",0); break;
		case 4:		gm_Print("~~~~Enter BurnInModeState~~~~",0); break;
		case 5:		gm_Print("~~~~Exit BurnInModeState~~~~",0); break;
		case 6:		gm_Print("~~~~Enter PowerOffState~~~~",0); break;
		case 7:		gm_Print("~~~~Exit PowerOffState~~~~",0); break;	
		case 8:		gm_Print("~~~~Enter PowerSavingState~~~~",0); break;
		case 9:		gm_Print("~~~~Exit PowerSavingState~~~~",0); break;	
		case 10:		gm_Print("~~~~Enter AutoSelInvState~~~~",0);
		case 11:		gm_Print("~~~~Exit AutoSelInvState~~~~",0); break;
		case 12:		gm_Print("~~~~Enter PIPOnScreenState~~~~",0); break;
		case 13:		gm_Print("~~~~Exit PIPOnScreenState~~~~",0); break;
		case 14:		gm_Print("~~~~Enter VideoOnScreenState~~~~",0); break;
		case 15:		gm_Print("~~~~Exit VideoOnScreenState~~~~",0); break;
		case 16:		gm_Print("~~~~Enter LanguageUpdateState~~~~",0); break;
		case 17:		gm_Print("~~~~Exit LanguageUpdateState~~~~",0); break;	
		case 18:		gm_Print("~~~~Enter NoSignalInvisibleState~~~~",0); break;
		case 19:		gm_Print("~~~~Exit NoSignalInvisibleState~~~~",0); break;		
		case 20:		gm_Print("~~~~Enter Message-NoSignalInvisibleState~~~~",0); break;
		case 21:		gm_Print("~~~~Exit Message-NoSignalInvisibleState~~~~",0); break;
		case 22:		gm_Print("~~~~Enter ScanningInvisibleState~~~~",0); break;
		case 23:		gm_Print("~~~~Exit ScanningInvisibleState~~~~",0); break;
		case 24:		gm_Print("~~~~Enter MesInvState(Flying)~~~~",0); break;
		case 25:		gm_Print("~~~~Exit MesInvState(Fly)~~~~",0); break;	
		case 26:		gm_Print("~~~~Enter TransferOsdState~~~~",0); break;
		case 27:		gm_Print("~~~~Exit TransferOsdState~~~~",0); break;	
		case 28:		gm_Print("~~~~MTO_NoSync  Received~~~~",0);	break;	
		case 29:		gm_Print("~~~~MTO_NoCable  Received~~~~",0); break;	
		case 30:		gm_Print("~~~~MTO_NoOutOfRange  Received~~~~",0); break;		
		case 31:		gm_Print("~~~~Enter Pip DefaultState~~~~",0); break;
		case 32:		gm_Print("~~~~Exit Pip DefaultState~~~~",0); break;
		case 33:		gm_Print("~~~~Enter Pip HideInvisibleState~~~~",0); break;
		case 34:		gm_Print("~~~~Exit Pip HideInvisibleState~~~~",0); break;
		case 35:		gm_Print("~~~~MTO_NoSync_Pip  Received~~~~",0); break;	
		case 36:		gm_Print("~~~~MTO_NoCable_pip  Received~~~~",0); break;	
		case 37:		gm_Print("~~~~MTO_NoOutOfRange_Pip  Received~~~~",0); break;		
		case 38:		gm_Print("~~~~OE_PIPMShow  Received~~~~",0); break;	
		case 39:		gm_Print("~~~~OE_PIPMHide  Received~~~~",0); break;	
		case 40:		gm_Print("~~~~Enter PIP Message~~~~",0); break;
		case 41:		gm_Print("~~~~Exit PIP Message~~~~",0); break;		
		case 42:		gm_Print("~~~~Enter PBP Message~~~~",0); break;
		case 43:		gm_Print("~~~~Exit PBP Message~~~~",0); break;
		case 44:		gm_Print("~~~~Enter No Signal Video Message~~~~",0); break;
		case 45:		gm_Print("~~~~Exit No Signal Video Message~~~~",0); break;		
		case 46:		gm_Print("~~~~Enter Component  OSD_IsMainSync is %d~~~~",OSD_IsMainSync); break;	
		case 47:		gm_Print("~~~~Exit No Signal Video Message~~~~",0); break;	
		case 48:		gm_Print("~~~~Enter OOR Message~~~~",0); 
	                            gm_Print("~~~~(OOR flag == 11) UserPrefStateTempFlag : %d",UserPrefStateTempFlag); 	  
	                            gm_Print("~~~~stInput[CH_A].ModeFlags & gmd_MODE_OUT_RANGE : %d",((stInput[CH_A].ModeFlags & gmd_MODE_OUT_RANGE) == gmd_MODE_OUT_RANGE)); 	  								
		break;	
		case 49:		gm_Print("~~~~MTO_ValidMode  Received~~~~",0); 
	                            gm_Print("~~~~UserPrefStateTempFlag : %d",UserPrefStateTempFlag); 	  								
		break;
		case 50:		gm_Print("~~~~Enter PIP OOR Message~~~~",0); 
	                            gm_Print("~~~~(OOR flag == 11) UserPrefStateTempFlag : %d",UserPrefStateTempFlag); 	  
	                            gm_Print("~~~~stInput[CH_B].ModeFlags & gmd_MODE_OUT_RANGE : %d",((stInput[CH_B].ModeFlags & gmd_MODE_OUT_RANGE) == gmd_MODE_OUT_RANGE)); 	  								
		break;			
		case 51:		gm_Print("~~~~Enter Main Input Icon State~~~~",0); 
	                            gm_Printf("~~~~ Icon X: %d   Y: %d",UserPrefPIPIconsXPos,UserPrefPIPIconsYPos); 	  
			 	       gm_Printf("Pos Offset X, Y : %d, %d",UserPrefGlobalXPosOffset,UserPrefGlobalYPosOffset);		   	
 	       			gm_Printf("OSD Position X, Y : %d, %d",UserPrefOsdHPosition,UserPrefOsdVPosition);		                            									
		break;		
		case 52:		gm_Print("~~~~ Enter OSD Splash Screen~~~~",0); break;	
		case 53:		gm_Print("~~~~ Exit OSD Splash Screen~~~~",0); break;
		case 61:		gm_Print("~~~~ Enter Choose ICON~~~~",0); break;
		case 62:		gm_Print("~~~~ Enter DP ICON~~~~",0); break;
		case 71:		gm_Print("~~~~ Enter Main OSD~~~~",0); break;
		case 72:		gm_Print("~~~~ Enter Splash Screen~~~~",0); break;
		case 73:		gm_Print("~~~~ Exit Splash Screen~~~~",0); break;
		case 74:		gm_Print("~~~~ Enter LOGO Fly ~~~~",0); break;
		case 75:		gm_Print("~~~~ Exit LOGO Fly~~~~",0); break;
		case 76:		gm_Print("~~~~ Enter Audio Voluem~~~~",0); break;
		case 77:		gm_Print("~~~~ Exit Audio Voluem~~~",0); break;
		case 78:		gm_Print("~~~~ Enter Pip Icon ~~~~",0); break;
		case 79:		gm_Print("~~~~ Exit Pip Icon~~~~",0); break;
		default:		gm_Print("~~~~No State error~~~~%d",state); break;
	}
	#else
	   state = state;
	#endif
}
#else
void	DebugOSD(BYTE state)
{UNUSED_VAR(state);}
#endif
SBYTE ConvertACM3RGBCMY(SBYTE B_ACM3_6Axis) //KevinL 20090331 OSD range 50~-50 to range 127 ~-127
{
	SBYTE B_ReturnValue;  
		
	if (B_ACM3_6Axis < 0)
		B_ReturnValue = (SBYTE)((SWORD)(B_ACM3_6Axis * ACM3_6AXIS_MIN)/WB_6AXIS_MIN);
	else	
		B_ReturnValue = (SBYTE)((SWORD)(B_ACM3_6Axis * ACM3_6AXIS_MAX)/WB_6AXIS_MAX);	

	return B_ReturnValue;
}

SBYTE ConvertBrightness(SBYTE B_Brightness)
{
       SBYTE B_ReturnValue;   

#ifdef DELL_U2410F  //KevinL 20081211 remapping OSD range 100~0 to range 255 ~0;
       B_ReturnValue = (SBYTE)(((SWORD)B_Brightness*(127)) / (WB_BRIGHTNESS_MAX - 0));
#else
	BYTE B_TempBrightness = B_Brightness + 128;
	BYTE B_TempDefaultBrightness = 128-4;
	
	if (B_TempBrightness >= MID_BRIGHTNESS)
	{
		B_ReturnValue = B_TempDefaultBrightness + (BYTE) (((WORD)(B_TempBrightness -MID_BRIGHTNESS)*(MAX_BRIGHTNESS - B_TempDefaultBrightness))/(MAX_BRIGHTNESS -MID_BRIGHTNESS)) - 128;
	}
	else
	{
		B_ReturnValue =((BYTE)((((WORD)B_TempBrightness *(B_TempDefaultBrightness -MIN_BRIGHTNESS))/MID_BRIGHTNESS) +MIN_BRIGHTNESS)-128); 
	}  
#endif

	return B_ReturnValue;
}

WORD ConvertContrast(WORD W_Contrast, gmt_PHY_CH Channel) //KevinL 20090320 updated
{
	WORD W_ReturnValue;
	WORD W_ContrastMin, W_ContrastCenter, W_ContrastMax;
	
	//gm_Printf("%d stInput[Channel].ColorDomain = %d", Channel, stInput[Channel].ColorDomain);
	//gm_Printf("UserPrefGraphicsImageMode = %d",  UserPrefGraphicsImageMode);
	//gm_Printf("UserPrefDUMSwitch = %d",  UserPrefDUMSwitch);
	
	if(Channel == gmvb_MainChannel)
	{
		//if(UserPrefColorMode == ColorMode_GRAPHIC)
		if(stInput[Channel].ColorDomain == Input_RGB)
		{
	  		W_ContrastMin = GraphicContrastBoundary[UserPrefGraphicsImageMode][0]; 
	  		W_ContrastCenter = GraphicContrastBoundary[UserPrefGraphicsImageMode][1]; 
	  		W_ContrastMax = GraphicContrastBoundary[UserPrefGraphicsImageMode][2]; 	  
		}
		else
		{
			//120731 Edward for YUV domain use
	  		W_ContrastMin = YUVContrastBoundary[UserPrefGraphicsImageMode][0]; 
	  		W_ContrastCenter = YUVContrastBoundary[UserPrefGraphicsImageMode][1]; 
	  		W_ContrastMax = YUVContrastBoundary[UserPrefGraphicsImageMode][2]; 		
		}
		/*if(stInput[Channel].ColorDomain != Input_RGB)
		{
			gm_Print("YUV",0);
			W_ContrastMin = 0; 
	  		W_ContrastCenter = WB_CONTRAST_DEFAULT; 
			if (W_Contrast >= WB_CONTRAST_DEFAULT)
				W_Contrast = (WORD)(((DWORD)(W_Contrast-WB_CONTRAST_DEFAULT)*(WB_CONTRAST_MAX_YUV-WB_CONTRAST_DEFAULT))/(WB_CONTRAST_MAX-WB_CONTRAST_DEFAULT) + WB_CONTRAST_DEFAULT);
	  		W_ContrastMax = WB_CONTRAST_MAX_YUV; 
		}*/
	}
	else
	{
		if(UserPrefPipColorMode == ColorMode_GRAPHIC)
		{
	  		W_ContrastMin = GraphicContrastBoundary[UserPrefGraphicsImageMode][0]; 
	  		W_ContrastCenter = GraphicContrastBoundary[UserPrefGraphicsImageMode][1]; 
	  		W_ContrastMax = GraphicContrastBoundary[UserPrefGraphicsImageMode][2]; 	  
		}
		else
		{
	  		W_ContrastMin = GraphicContrastBoundary[UserPrefVideoImageMode][0]; 
	  		W_ContrastCenter = GraphicContrastBoundary[UserPrefVideoImageMode][1]; 
	  		W_ContrastMax = GraphicContrastBoundary[UserPrefVideoImageMode][2]; 		
		}	
	}
	
	//gm_Print("ConvertContrast input : %d",W_Contrast - WB_MIN_CONTRAST);
#ifdef DELL_U2410F//KevinL 20081121 remapping 306 ~ 206 to User define range 306 ~0;
	#if 0
	if (W_Contrast >= WB_CONTRAST_DEFAULT) //306 ~256 map into 306 ~256 
		W_ReturnValue = W_ContrastCenter + (WORD)(((DWORD)(W_Contrast - WB_CONTRAST_DEFAULT) * (W_ContrastMax - W_ContrastCenter)) / (WB_CONTRAST_MAX - WB_CONTRAST_DEFAULT));		  
	else //255~206 map into 255~0 		
		W_ReturnValue =   W_ContrastMin + (WORD)(((DWORD)(W_Contrast - WB_CONTRAST_MIN) * (W_ContrastCenter - W_ContrastMin)) / (DEFAULT_CONTRAST-WB_CONTRAST_MIN));	
	#else
	/*if((stInput[Channel].ColorDomain != Input_RGB))
	{
		if (W_Contrast >= WB_CONTRAST_DEFAULT) //306 ~256 map into 306 ~256 
			W_ReturnValue = W_ContrastCenter + (WORD)(((DWORD)(W_Contrast - WB_CONTRAST_DEFAULT) * (W_ContrastMax - W_ContrastCenter)) / (WB_CONTRAST_MAX_YUV - WB_CONTRAST_DEFAULT));		  
		else //255~206 map into 255~0 		
			W_ReturnValue =   W_ContrastMin + (WORD)(((DWORD)(W_Contrast - WB_CONTRAST_MIN) * (W_ContrastCenter - W_ContrastMin)) / (DEFAULT_CONTRAST-WB_CONTRAST_MIN));	
	}
	else*/
	#if 1//defined(CMI_73XX_BOARD)
	  if(IS_UNIFORMITY_Off())
	  	{
		 if (W_Contrast >= WB_CONTRAST_DEFAULT) //306 ~256 map into 306 ~256 
			W_ReturnValue = W_ContrastCenter + (WORD)(((DWORD)(W_Contrast - WB_CONTRAST_DEFAULT) * (W_ContrastMax - W_ContrastCenter)) / (WB_CONTRAST_MAX - WB_CONTRAST_DEFAULT));		  
		 else //255~206 map into 255~0 		
			W_ReturnValue = W_ContrastMin + (WORD)(((DWORD)(W_Contrast - WB_CONTRAST_MIN) * (W_ContrastCenter - W_ContrastMin)) / (WB_CONTRAST_DEFAULT - WB_CONTRAST_MIN));	
	    }
	  else
	  	{ // chunbao do not adjust contrast when UC on, set value to OSD default 50 
	  	    W_ReturnValue = W_ContrastCenter + (WORD)(((DWORD)(WB_CONTRAST_DEFAULT - WB_CONTRAST_DEFAULT) * (W_ContrastMax - W_ContrastCenter)) / (WB_CONTRAST_MAX - WB_CONTRAST_DEFAULT));
	  	}
	#else
	{
		if (W_Contrast >= WB_CONTRAST_DEFAULT) //306 ~256 map into 306 ~256 
			W_ReturnValue = W_ContrastCenter + (WORD)(((DWORD)(W_Contrast - WB_CONTRAST_DEFAULT) * (W_ContrastMax - W_ContrastCenter)) / (WB_CONTRAST_MAX - WB_CONTRAST_DEFAULT));		  
		else //255~206 map into 255~0 		
			W_ReturnValue = W_ContrastMin + (WORD)(((DWORD)(W_Contrast - WB_CONTRAST_MIN) * (W_ContrastCenter - W_ContrastMin)) / (WB_CONTRAST_DEFAULT - WB_CONTRAST_MIN));	
	}
	#endif
	#endif
		
#else 
	if (W_Contrast >= MID_CONTRAST)
	{
		W_ReturnValue = DEFAULT_CONTRAST + (WORD) (((DWORD)(W_Contrast - MID_CONTRAST) * (MAX_CONTRAST - DEFAULT_CONTRAST)) / (MAX_CONTRAST -MID_CONTRAST));
	}
	else
	{
		W_ReturnValue = (WORD)((((DWORD)W_Contrast * (DEFAULT_CONTRAST -MIN_CONTRAST)) / MID_CONTRAST) + MIN_CONTRAST); 
	}  
#endif    
	//gm_Print("ConvertContrast output : %d",W_ReturnValue);
	return W_ReturnValue;
}

BYTE ConvertSharpnessVWD(BYTE B_SharpnessVWD) //20120618 Add by KevinChen OSD range 100~0 to range 255 ~0
{
	BYTE B_ReturnValue;   

#if 1 // Limit range -63 ~ + 63
	   B_ReturnValue = (BYTE)((((SWORD)B_SharpnessVWD - 50)*127)/100);
#else
	   B_ReturnValue = ((WORD)(B_SharpnessVWD*255))/100;
#endif

	return B_ReturnValue;
}

WORD ConvertSaturation(WORD W_Saturation) //KevinL 20081211 remapping OSD range 100~0 to range 512 ~0
{
	WORD W_ReturnValue;   

	if(W_Saturation <= WB_SATURATION_DEFAULT)
		W_ReturnValue = (WORD)(W_Saturation*(SATURATION_DEFAULT - SATURATION_MIN))/(WB_SATURATION_DEFAULT -WB_SATURATION_MIN);
	else
		W_ReturnValue = (WORD)(W_Saturation*(SATURATION_MAX - SATURATION_DEFAULT))/(WB_SATURATION_MAX-WB_SATURATION_DEFAULT);
		

	return W_ReturnValue;
}

SWORD ConvertHue(SWORD W_Hue) //KevinL 20081211 remapping OSD range 100~0 to range 180 ~ -180
{
	SWORD W_ReturnValue;
	
	//Neil120720*
	 W_ReturnValue = (SWORD)((W_Hue*(360))/(HUE_MAX-0));	

	//W_ReturnValue = (W_Hue*(180))/(HUE_MAX-0);	

	return W_ReturnValue;
}

WORD ConvertRGBGain(WORD W_RGBGain) //KevinL 20081212 remapping OSD range 100~0 to range 256 ~0
{
	WORD W_ReturnValue;   
 	W_ReturnValue =   MIN_RGB_GAIN+(WORD)(((DWORD)(W_RGBGain - USER_GAIN_MIN)*(MAX_RGB_GAIN-MIN_RGB_GAIN))/(USER_GAIN_MAX-USER_GAIN_MIN));

   	 return W_ReturnValue;
}

SBYTE ConvertRGBOffset(SBYTE B_RGBOffset) //KevinL 20090312 remapping OSD range 100~0 to range 100 ~ -100  ==> reduce effect range to 65~43 (30 ~-14)
{
	SBYTE B_ReturnValue;  

	if (B_RGBOffset < 0)
	{
		B_ReturnValue = RGB_OFFSET_DEFAULT - (SBYTE)((SWORD)(-B_RGBOffset * (RGB_OFFSET_DEFAULT - RGB_OFFSET_MIN))/USER_RGB_OFFSET_MAX);
	}
	else
	{
		B_ReturnValue = RGB_OFFSET_DEFAULT + (SBYTE)((SWORD)(B_RGBOffset * (RGB_OFFSET_MAX - RGB_OFFSET_DEFAULT))/USER_RGB_OFFSET_MAX);	
	}

	return B_ReturnValue;
}

BYTE ConvertVGAPhase(BYTE HSyncPhase) //KevinL 20090116 convert VGA phase from OSD 0~100 to setting 0~63
{
   BYTE B_ReturnValue; 
   B_ReturnValue = ((WORD)HSyncPhase * MAX_VGA_PHASE)/WB_MAX_VGA_PHASE;
   return B_ReturnValue;
}

BYTE InvertVGAPhase(BYTE HSyncPhase) //KevinL 20090116 convert VGA phase from setting 0~63 to 0~100 OSD
{
   BYTE B_ReturnValue; 
   B_ReturnValue = (((WORD)HSyncPhase * WB_MAX_VGA_PHASE/MAX_VGA_PHASE*10)+5)/10; //KevinL 20090316 
   return B_ReturnValue;
}

void InitPresetModesDescr(void)
{
	NVRam_WriteByte(PresetModeSettingIndexStart, NUM_OF_IMAGE_SCHEMES_ENTRIES);
}

void InitPresetModesDependent(void)
{
	WORD W_Address;
	BYTE i;
	PresetModesTypeCS Stp_tmp;

	W_Address  = PresetModeSettingStart;

	for (i = 0; i < NUM_OF_IMAGE_SCHEMES_ENTRIES; i++)
	{
  	    W_Address  = PresetModeSettingStart +(WORD)i*(PresetModesUserPrefSize + ChecksumSize); //KevinL 20090316 update address for correct input	
		PresetModeUserPreferences = PresetModeFactoryDefaultInROM[i];
		NVRam_WriteBuffer(W_Address, (BYTE *)(&PresetModeUserPreferences), PresetModesUserPrefSize);
		NVRam_WriteByte(W_Address + PresetModesUserPrefSize, CalculateChecksum((&PresetModeUserPreferences), PresetModesUserPrefSize));
	}
}

BOOL ReadPresetModesDependentEntry(BYTE B_Index)
{
	WORD W_Address;					// holds the computed NVRAM address
	BYTE B_Checksum1, B_Checksum2;	// holds the computed checksum values

	// compute the start address
	W_Address = PresetModeSettingStart + (WORD)B_Index * (PresetModesUserPrefSize + ChecksumSize);

	// read the PortDependentUserPreferences record and save it
	NVRam_ReadBuffer(W_Address, (BYTE *)(&PresetModeUserPreferences), PresetModesUserPrefSize);

	// read the checksum byte
	B_Checksum1 = NVRam_ReadByte(W_Address + PresetModesUserPrefSize);

	// calculate checksum.
	B_Checksum2 = CalculateChecksum((&PresetModeUserPreferences), PresetModesUserPrefSize);

	if (B_Checksum1 != B_Checksum2)
	{
		PresetModeUserPreferences = PresetModeFactoryDefaultInROM[B_Index];
		NVRam_WriteBuffer(W_Address, (BYTE *)(&PresetModeUserPreferences), PresetModesUserPrefSize);
		NVRam_WriteByte(W_Address + PresetModesUserPrefSize, CalculateChecksum((&PresetModeUserPreferences), PresetModesUserPrefSize));

		msg_i("PMD: NVRAM entry %d", B_Index);
		msg_i("PMD: checksum different: %x", B_Checksum1);
		return FALSE;
	}

	msg_i("PMD: NVRAM entry %d", B_Index);
	msg_i("PMD: NVRAM found matched, address 0x%x", W_Address);

	return TRUE;
}

BOOL CheckPresetModesDependentEntry(BYTE B_Index)
{
	PresetModesTypeCS st_PD;

	if (NVRam_ReadBuffer(PresetModeSettingStart + B_Index * (PresetModesUserPrefSize + ChecksumSize), (BYTE*)&st_PD, sizeof(st_PD)) != OK)
		return FALSE;

	// calculate mode dependent checksum.
	B_Index = CalculateChecksum(&st_PD.st, PresetModesUserPrefSize);

	// We need to make sure the data the we retrieved is valid otherwise return FALSE
	if (B_Index != st_PD.cs)
	{
		msg_i("PMD: PD entry Checksum failed: 0x%x", B_Index);
		msg_i("PMD: PD entry Checksum failed: 0x%x", st_PD.cs);
		return FALSE;
	}
	return TRUE;
}

BOOL CheckPresetModesDependentDescriptor(void)
{
	BYTE B_NumEntries = NVRam_ReadByte(PresetModeSettingIndexStart);

	if (B_NumEntries != NUM_OF_IMAGE_SCHEMES_ENTRIES)
	{
		msg_i("PMD: NUM_OF_CONNECTORS defined in FW: %d", NUM_OF_IMAGE_SCHEMES_ENTRIES);
		msg_i("PMD: Descriptor value: %d", B_NumEntries);
		return FALSE;
	}
	return TRUE;
}

void SavePresetModesDependentSettings(void)
{
	BYTE B_Index = InputConvertScheme(UserPrefCurrentInputMain);
	WORD W_Address = PresetModeSettingStart + (WORD)B_Index * (PresetModesUserPrefSize + ChecksumSize);

	NVRam_WriteBuffer(W_Address, (BYTE *)(&PresetModeUserPreferences), PresetModesUserPrefSize);
	NVRam_WriteByte(W_Address + PresetModesUserPrefSize, CalculateChecksum(&PresetModeUserPreferences, PresetModesUserPrefSize));

	msg_i("PMD: Address of: 0x%x", W_Address );
	msg_i("PMD: saved NVRAM mode entry %d", B_Index);
}

void ReadPipPortSchemeDependent(void)	// Increased by ST_hansPENG 090212
{
	PortSchemeDependentUserPrefType PortSchemeDependentUserPreferencesBackup;
	WORD W_Address;					// holds the computed NVRAM address
	BYTE B_Checksum1, B_Checksum2;	// holds the computed checksum values

	// Backup PortSchemeDependent of Main Chanel
	PortSchemeDependentUserPreferencesBackup = PortSchemeDependentUserPreferences;

	// compute the start address
	W_Address = PortSchemeDependentEntryStart + (WORD)UserPrefPipImageScheme * (PortSchemeDependentUserPrefSize + ChecksumSize);

	// read the PortDependentUserPreferences record and save it
	NVRam_ReadBuffer(W_Address, (BYTE *)(&PortSchemeDependentUserPreferences), PortSchemeDependentUserPrefSize);

	UserPrefPipBrightness = PortSchemeDependentUserPreferences.Brightness;
	UserPrefPipContrast = PortSchemeDependentUserPreferences.Contrast;
	UserPrefPipSaturation = PortSchemeDependentUserPreferences.Saturation;
	UserPrefPipHue = PortSchemeDependentUserPreferences.Hue;

	UserPrefPipUserRedGain = PortSchemeDependentUserPreferences.UserRedGain;
	UserPrefPipUserGreenGain = PortSchemeDependentUserPreferences.UserGreenGain;
	UserPrefPipUserBlueGain = PortSchemeDependentUserPreferences.UserBlueGain;

	UserPrefPipUserRedOff = PortSchemeDependentUserPreferences.UserRedOff;
	UserPrefPipUserGreenOff = PortSchemeDependentUserPreferences.UserGreenOff;
	UserPrefPipUserBlueOff = PortSchemeDependentUserPreferences.UserBlueOff;

	UserPrefPipColorTemp = PortSchemeDependentUserPreferences.ColorTemp;
	UserPrefPipGammaTable = PortSchemeDependentUserPreferences.GammaTable;
	UserPrefPipColorFormat = PortSchemeDependentUserPreferences.ColorFormat;
	UserPrefPipColorMode = PortSchemeDependentUserPreferences.ColorMode;
	UserPrefPipVideoImageMode = PortSchemeDependentUserPreferences.VideoImageMode;
	UserPrefPipGraphicsImageMode = PortSchemeDependentUserPreferences.GraphicsImageMode;

	SaveModeIndependentSettings();

	// Restore PortSchemeDependent of Main Chanel
	PortSchemeDependentUserPreferences = PortSchemeDependentUserPreferencesBackup;
}

void SavePipPortSchemeDependent(void)	// Increased by ST_hansPENG 090212
{
	PortSchemeDependentUserPrefType PortSchemeDependentUserPreferencesBackup;
	WORD W_Address;
	
	// Backup PortSchemeDependent of Main Chanel
	PortSchemeDependentUserPreferencesBackup = PortSchemeDependentUserPreferences;

	// compute the start address
	W_Address = PortSchemeDependentEntryStart + (WORD)UserPrefPipImageScheme * (PortSchemeDependentUserPrefSize + ChecksumSize);

	// read the PortDependentUserPreferences record and save it
	NVRam_ReadBuffer(W_Address, (BYTE *)(&PortSchemeDependentUserPreferences), PortSchemeDependentUserPrefSize);

	PortSchemeDependentUserPreferences.Brightness = UserPrefPipBrightness;
	PortSchemeDependentUserPreferences.Contrast = UserPrefPipContrast;
	PortSchemeDependentUserPreferences.Contrast_All = UserPrefPipContrast;   //Neil 140816 for pipcontrast issue
	PortSchemeDependentUserPreferences.Saturation = UserPrefPipSaturation;
	PortSchemeDependentUserPreferences.Hue = UserPrefPipHue;
 
	PortSchemeDependentUserPreferences.UserRedGain = UserPrefPipUserRedGain;
	PortSchemeDependentUserPreferences.UserGreenGain = UserPrefPipUserGreenGain;
	PortSchemeDependentUserPreferences.UserBlueGain = UserPrefPipUserBlueGain;

	PortSchemeDependentUserPreferences.UserRedOff = UserPrefPipUserRedOff;
	PortSchemeDependentUserPreferences.UserGreenOff = UserPrefPipUserGreenOff;
	PortSchemeDependentUserPreferences.UserBlueOff = UserPrefPipUserBlueOff;
 
	PortSchemeDependentUserPreferences.ColorTemp = UserPrefPipColorTemp;
	PortSchemeDependentUserPreferences.GammaTable = UserPrefPipGammaTable;
	PortSchemeDependentUserPreferences.ColorFormat = UserPrefPipColorFormat;
	PortSchemeDependentUserPreferences.ColorMode = UserPrefPipColorMode;
	PortSchemeDependentUserPreferences.VideoImageMode = UserPrefPipVideoImageMode;
	PortSchemeDependentUserPreferences.GraphicsImageMode = UserPrefPipGraphicsImageMode;

	NVRam_WriteBuffer(W_Address, (BYTE *)(&PortSchemeDependentUserPreferences), PortSchemeDependentUserPrefSize);
	NVRam_WriteByte(W_Address + PortSchemeDependentUserPrefSize, CalculateChecksum(&PortSchemeDependentUserPreferences, PortSchemeDependentUserPrefSize));
	SavePresetModesSettings();

	// Restore PortSchemeDependent of Main Chanel
	PortSchemeDependentUserPreferences = PortSchemeDependentUserPreferencesBackup;
}

void PipImageSchemeReset(void)
{
	UserPrefPipBrightness = FactoryDefaultInROM.PipColorGroup.PipBrightness;
	UserPrefPipContrast = FactoryDefaultInROM.PipColorGroup.PipContrast;
	UserPrefPipSaturation = FactoryDefaultInROM.PipColorGroup.PipSaturation;
	UserPrefPipHue = FactoryDefaultInROM.PipColorGroup.PipHue;

	UserPrefPipUserRedGain = FactoryDefaultInROM.PipColorGroup.PipUserRedGain;
	UserPrefPipUserGreenGain = FactoryDefaultInROM.PipColorGroup.PipUserGreenGain;
	UserPrefPipUserBlueGain = FactoryDefaultInROM.PipColorGroup.PipUserGreenGain;

	UserPrefPipUserRedOff = FactoryDefaultInROM.PipColorGroup.PipUserRedOff;
	UserPrefPipUserGreenOff = FactoryDefaultInROM.PipColorGroup.PipUserGreenOff;
	UserPrefPipUserBlueOff = FactoryDefaultInROM.PipColorGroup.PipUserGreenOff;

	UserPrefPipColorTemp = FactoryDefaultInROM.PipColorGroup.PipColorTemp;
	UserPrefPipGammaTable = FactoryDefaultInROM.PipColorGroup.PipGammaTable;
	UserPrefPipColorFormat = FactoryDefaultInROM.PipColorGroup.PipColorFormat;
	UserPrefPipColorMode = FactoryDefaultInROM.PipColorGroup.PipColorMode;
	UserPrefPipVideoImageMode = FactoryDefaultInROM.PipColorGroup.PipVideoImageMode;
	UserPrefPipGraphicsImageMode = FactoryDefaultInROM.PipColorGroup.PipGraphicsImageMode;

	SaveModeIndependentSettings();
}

void AllocateTile(void)	
{
	gmt_OSD_TILE_CTRL ROM *Sp_TileCtrl;
	gmt_OSD_DIALOG_CTRL ROM *Sp_DialogCtrl;
	ST_OSD_TILE St_Tile;
	WORD W_DialogId;
	gmt_OSD_TILE_POSITION* Stp_TilePos;
	BYTE B_AllocScheme = 0;

	// Get a pointer to the TILE_CTRL struture
	gm_OsdGetActionOperand();
	Sp_TileCtrl = (gmt_OSD_TILE_CTRL ROM *)gmv_ActionOpPtr;

	// Initialize the fields in St_Tile from the TILE_CTRL structure
	//gm_Print("Sp_TileCtrl->DialogId %d",Sp_TileCtrl->DialogId);
	//gm_Print("Sp_TileCtrl->TileId %d",Sp_TileCtrl->TileId);	
	//gm_Print("Sp_TileCtrl->XSize %d",Sp_TileCtrl->XSize);	
	//gm_Print("Sp_TileCtrl->YSize %d",Sp_TileCtrl->YSize);		
	//gm_Print("Sp_TileCtrl->ColorAttr %d",Sp_TileCtrl->ColorAttr);		
	St_Tile.B_Attr = Sp_TileCtrl->Attr;
	
	//KevinL 20081016 PIP Icon/Message Y position incorrect, DialogId 146/147 Main/Pip Input Icon, DialogId 148 PIP Message 
	if((UserPrefOsdRotation==OSD_NON_ROTATED)&&(/*(Sp_TileCtrl->DialogId == MainIconDialogId)||*/(Sp_TileCtrl->DialogId == PIPIconDialogId)||(Sp_TileCtrl->DialogId == PIPMessageDialogId)))//owen 120828 disable MainIconDialogId
	{
	   St_Tile.W_Xpos = Sp_TileCtrl->XPos;
	   St_Tile.W_Ypos = Sp_TileCtrl->YPos;		
	}
	else
	{
	   St_Tile.W_Xpos = Sp_TileCtrl->XPos+UserPrefGlobalXPosOffset;
  	   St_Tile.W_Ypos = Sp_TileCtrl->YPos+UserPrefGlobalYPosOffset;	
	}
		
	St_Tile.W_Width = Sp_TileCtrl->XSize;
	St_Tile.W_Height = Sp_TileCtrl->YSize;
	St_Tile.W_Color = Sp_TileCtrl->ColorAttr;

	ScaleXParam(&St_Tile.W_Width);
	ScaleYParam(&St_Tile.W_Height);
	ScaleXParam(&St_Tile.W_Xpos);
	ScaleYParam(&St_Tile.W_Ypos);

	// probably we have to exchange blink and blend attributes here
	// because they are defined differently in WB output and in IntRom f/w meantime
	St_Tile.B_Attr &= (~(TILE_ATTR_BLINK_BIT | TILE_ATTR_BLEND_BIT));
	if (Sp_TileCtrl->Attr & TILE_ATTR_BLINK_BIT)
	{
		St_Tile.B_Attr |= TILE_ATTR_BLEND_BIT;
	}
	if (Sp_TileCtrl->Attr & TILE_ATTR_BLEND_BIT)
	{
		St_Tile.B_Attr |= TILE_ATTR_BLINK_BIT;
	}

	SetRotatedTileSizePosition(&St_Tile);

	//msg("AllocateTile: structure parameters set.");

	// Determine the DialogId and set the XPos and YPos for the dialog
	W_DialogId = Sp_TileCtrl->DialogId & TILE_DIALOG_ID_BITS;
	Stp_TilePos = &gmv_OsdTilePosition[Sp_TileCtrl->TileId];

	// if BIT7 is set then Dialog information exists
	if (Sp_TileCtrl->DialogId & TILE_DIALOG_INFO_BIT)
	{
		//msg("AllocateTile: Dialog setup entered");

		Stp_TilePos->W_XPos = St_Tile.W_Xpos;
		Stp_TilePos->W_YPos = St_Tile.W_Ypos;

		// Get a pointer to the Dialog control
		Sp_DialogCtrl = (gmt_OSD_DIALOG_CTRL ROM *)(gmv_OsdDialogs[W_DialogId]);

		// If the horizontal reference point is not 0 then set the Tile Xpos to
		// the horizontal reference point for the dialog
		SetTileOffset(&St_Tile, Sp_DialogCtrl);

		//msg("AllocateTile: Dialog information set.");
	}
	else
	{
		Stp_TilePos->W_XPos = Stp_TilePos->W_YPos = UNKNOWN_TILE_DIALOG_POSITION;
	}
	
	St_Tile.D_BmpAddr = Sp_TileCtrl->Address;

	//bandwidh efficient memory allocation
	if (Sp_TileCtrl->AllocScheme)
	{
		B_AllocScheme = BIT2;
	}

	//msg("AllocateTile: Allocating tile space with gm_OsdSetTile.");

	// Call gm_OsdSetTile to Allocate and enable new tile in SRAM
	gm_OsdSetTile(Sp_TileCtrl->TileId, (ST_OSD_TILE ROM *)(&St_Tile), (B_AllocScheme | 1));
	//msg("AllocateTile: Reversing TileSet.");
	ReverseTileSet();
	gm_OsdSetTile(Sp_TileCtrl->TileId, (ST_OSD_TILE ROM *)(&St_Tile), (B_AllocScheme | 0));
	//msg("AllocateTile: Finished function.");
}

//******************************************************************************
// FUNCTION:	static void SetRotatedTileSizePosition(ST_OSD_TILE *Stp_Tile)
//
// USAGE:		If necessary recalculate x and y start positions of a tile
//				as well as its width and height based on an OSD rotation angle,
//				and then updates Stp_Tile fields
//
// INPUT:		Stp_Tile - ponter to a tile data structure which contains
//							original tile x and y start positions,
//							width and height
//
// OUTPUT:		None
//
// GLOBALS:		gmvb_OsdRotation
//				
// USED_REGS:	OSD_V_ACTIVE
//
//******************************************************************************
void SetRotatedTileSizePosition(ST_OSD_TILE *Stp_Tile)	
{
	WORD W_temp;
	WORD W_PanelHeight;
	gmt_OSD_TILE_CTRL ROM *Sp_TileCtrl;//KevinL 20090211	
	// Get a pointer to the TILE_CTRL struture
	gm_OsdGetActionOperand();
	Sp_TileCtrl = (gmt_OSD_TILE_CTRL ROM *)gmv_ActionOpPtr;	

	if (TEST_OSD_PARAM_ROTATION)
	{
		W_PanelHeight = gm_OsdGetVerticalSize();

		//save X position
		W_temp  = Stp_Tile->W_Xpos;

		//set X position
		Stp_Tile->W_Xpos = Stp_Tile->W_Ypos;

		//set Y position
		if (W_PanelHeight < (W_temp + Stp_Tile->W_Width))
		{
			Stp_Tile->W_Ypos = 0;
		}
		else
		{
			if (Sp_TileCtrl->DialogId == MainIconDialogId) //KevinL 20090210		       
				Stp_Tile->W_Ypos = W_PanelHeight - W_temp - Stp_Tile->W_Width;
			else
				Stp_Tile->W_Ypos = W_PanelHeight - W_temp - Stp_Tile->W_Width + (W_PanelHeight-1000); // 1000 is OSD dialog height		
		}

		//save tile height
		W_temp  = Stp_Tile->W_Height;

		//set tile height
		Stp_Tile->W_Height = Stp_Tile->W_Width;

		//set tile width
		Stp_Tile->W_Width = W_temp;
	}
}

//******************************************************************************
//
// FUNCTION: 	static void SetTileOffset(ST_OSD_TILE *Sp_Tile,
//								gmt_OSD_DIALOG_CTRL ROM *Sp_DialogCtrl)
//
// USAGE:		Adds appropriate offsets to tile x and y start positions
//				based on an OSD rotation angle and position information
//				for the current dialog, and then updates Stp_Tile fields
// 				
// INPUT:       Sp_Tile - ponter to a tile data structure which contains
//							tile x and y start positions, width and height
// 				Sp_DialogCtrl - pointer to the current dialog data structure
//
// OUTPUT:		None
//
// GLOBALS:		None
//
// USED_REGS:	OSD_H_ACTIVE, OSD_V_ACTIVE
//
//******************************************************************************
void SetTileOffset(ST_OSD_TILE *Sp_Tile,	gmt_OSD_DIALOG_CTRL ROM *Sp_DialogCtrl)	
{
	WORD W_PanelWidth = gm_OsdGetHorizontalSize() * 2;
	WORD W_PanelHeight = gm_OsdGetVerticalSize();
	WORD W_XInterval = W_PanelWidth;
	WORD W_YInterval = W_PanelHeight;
	WORD W_HposRef;
	WORD W_VposRef;

	if (TEST_OSD_PARAM_ROTATION)
	{
		W_XInterval -= Sp_DialogCtrl->MaxYpos;
		W_YInterval -= Sp_DialogCtrl->MaxXpos;
		W_HposRef = Sp_DialogCtrl->VPositionRef;
		W_VposRef = Sp_DialogCtrl->HPositionRef;

		Sp_Tile->W_Ypos -= (WORD)CalculateTileLocation(W_VposRef, 0, W_YInterval);
	}
	else
	{
		W_XInterval -= Sp_DialogCtrl->MaxXpos;
		W_YInterval -= Sp_DialogCtrl->MaxYpos;
		W_HposRef = Sp_DialogCtrl->HPositionRef;
		W_VposRef = Sp_DialogCtrl->VPositionRef;

		Sp_Tile->W_Ypos += (WORD)CalculateTileLocation(W_VposRef, 0, W_YInterval);
	}

	Sp_Tile->W_Xpos += (WORD)CalculateTileLocation(W_HposRef, 0, W_XInterval);
}

//******************************************************************************
//
// FUNCTION:	static SDWORD CalculateTileLocation(WORD W_PosRef,
//												SDWORD SDW_Min, SDWORD SDW_Max)
//
// USAGE:		Calculates actual tile x or y offsets in pixels
//				based on position information for the current dialog
//			
// INPUT:		W_PosRef - reference to dialog position adjuster
//				SDW_Min  - minimum position value in pixels
//				SDW_Max  - maximum position value in pixels
//
// OUTPUT:		Tile x or y offset in pixels
//
// GLOBALS:		None
//
// USED_REGS:	None
//
//******************************************************************************
static SDWORD CalculateTileLocation(WORD W_PosRef, SDWORD SDW_Min, SDWORD SDW_Max)
{
	WORD W_Interval;
	SDWORD SDW_AdjMax, SDW_AdjMin, SDW_Pos = 0;

	// Set the interval width
	W_Interval = (WORD)(SDW_Max - SDW_Min);

	// If the reference is valid cacluate the new value
	if (!(W_PosRef & UNADJUSTABLE_BIT))
	{
		SDW_AdjMin = gm_GetAdjusterMin(W_PosRef);
		SDW_AdjMax = gm_GetAdjusterMax(W_PosRef);

		if (W_Interval > 0)
		{
			SDW_Pos = gm_GetAdjuster(W_PosRef);
			// Rescale the adjustor to the full screen size.
			SDW_Pos = ((SDW_Pos - SDW_AdjMin) * W_Interval) / (SDW_AdjMax - SDW_AdjMin);		
		}
	}
	else
	{
		SDW_Pos = (W_PosRef & (~UNADJUSTABLE_BIT));
	}

	return SDW_Min + SDW_Pos;
}

void CustomRotateAdjust(void)	
{
	gmvb_OsdRotation = UserPrefOsdRotation;
};

void ReadPresetModesSettings(void)
{
	ReadPresetModesDependentEntry(UserPrefPipImageScheme);
	if (IsPipColorMode_GRAPHIC)
	{
		UserPrefPipHue = SchemeFactoryDefaultInROM[UserPrefPipImageScheme].Hue;
		UserPrefPipSaturation = SchemeFactoryDefaultInROM[UserPrefPipImageScheme].Saturation;
	}
	else		// if (IsColorMode_VODEOO)
	{
		switch (UserPrefPipVideoImageMode)
		{
			case Video_MOVIE:
				UserPrefPipHue = PresetModeUserPreferences.VideoPresetMode.Movie.Hue;
				UserPrefPipSaturation = PresetModeUserPreferences.VideoPresetMode.Movie.Saturation;
				break;

			case Video_GAME:
				UserPrefPipHue = PresetModeUserPreferences.VideoPresetMode.Game.Hue;
				UserPrefPipSaturation = PresetModeUserPreferences.VideoPresetMode.Game.Saturation;
				break;

			case Video_NATURE:
				UserPrefPipHue = PresetModeUserPreferences.VideoPresetMode.Nature.Hue;
				UserPrefPipSaturation = PresetModeUserPreferences.VideoPresetMode.Nature.Saturation;
				break;
				
			case Video_xvMode:
				UserPrefPipHue = PresetModeUserPreferences.VideoPresetMode.xvMode.Hue;
				UserPrefPipSaturation = PresetModeUserPreferences.VideoPresetMode.xvMode.Saturation;
				break;				
		};
	}

	ReadPresetModesDependentEntry(UserPrefImageScheme);
	if (IsColorMode_GRAPHIC)
	{
		//UserPrefImageScheme by port save
		//UserPrefGraphicsImageMode by different image scheme
		WB_ImageScheme = ConvertPresetMode(UserPrefGraphicsImageMode);
		//gm_Print("###WB_ImageScheme  %d",WB_ImageScheme);
		//gm_Print("###UserPrefDCR  %d",UserPrefDCR);
		//gm_Print("###UserPrefDUMSwitch  %d",UserPrefDUMSwitch);
		
		//120608 Edward disable for movie / game
		//UserPrefHue = SchemeFactoryDefaultInROM[UserPrefImageScheme].Hue;
		//UserPrefSaturation = SchemeFactoryDefaultInROM[UserPrefImageScheme].Saturation;		
	}
	else		// if (IsColorMode_VODEOO)
	{
		switch (UserPrefVideoImageMode)
		{
			case Video_MOVIE:
				UserPrefHue = PresetModeUserPreferences.VideoPresetMode.Movie.Hue;
				UserPrefSaturation = PresetModeUserPreferences.VideoPresetMode.Movie.Saturation;
				break;

			case Video_GAME:
				UserPrefHue = PresetModeUserPreferences.VideoPresetMode.Game.Hue;
				UserPrefSaturation = PresetModeUserPreferences.VideoPresetMode.Game.Saturation;
				break;

			case Video_NATURE:
				UserPrefHue = PresetModeUserPreferences.VideoPresetMode.Nature.Hue;
				UserPrefSaturation = PresetModeUserPreferences.VideoPresetMode.Nature.Saturation;				
				break;
				
			case Video_xvMode:
				UserPrefHue = PresetModeUserPreferences.VideoPresetMode.xvMode.Hue;
				UserPrefSaturation = PresetModeUserPreferences.VideoPresetMode.xvMode.Saturation;
				break;				
		};
	}
}

void  SavePresetModesSettings(void)
{
	if (IsColorMode_VODEO)
	{
		switch (UserPrefVideoImageMode)
		{
			case Video_MOVIE:
				PresetModeUserPreferences.VideoPresetMode.Movie.Hue = UserPrefHue;
				PresetModeUserPreferences.VideoPresetMode.Movie.Saturation = UserPrefSaturation;
				break;

			case Video_GAME:
				PresetModeUserPreferences.VideoPresetMode.Game.Hue = UserPrefHue;
				PresetModeUserPreferences.VideoPresetMode.Game.Saturation = UserPrefSaturation;
				break;

			case Video_NATURE:
				PresetModeUserPreferences.VideoPresetMode.Nature.Hue = UserPrefHue;
				PresetModeUserPreferences.VideoPresetMode.Nature.Saturation = UserPrefSaturation;
				break;
				
			case Video_xvMode:
				PresetModeUserPreferences.VideoPresetMode.xvMode.Hue = UserPrefHue;
				PresetModeUserPreferences.VideoPresetMode.xvMode.Saturation = UserPrefSaturation;
				break;				
		}
	}

	SavePresetModesDependentSettings();
}

void StartNOFlyingWindow(void) 
{
	StoreOsdHPosition = UserPrefOsdHPosition;
	StoreOsdVPosition = UserPrefOsdVPosition;
	
	if(UserPrefPipMode != SIDE_BY_SIDE)
	{
	   UserPrefOsdHPosition = 500; 
	   UserPrefOsdVPosition = 500; 
	}
	else
	{
	if(PanelH_1440)
	   UserPrefOsdHPosition = 194; //owen 20120912
	else
	   UserPrefOsdHPosition = 164; 
	
	   UserPrefOsdVPosition = 500; 

	   //120601 Edward for PBP main no cable msg overlap sub channel
	   W_SavePIPIconXoffset= UserPrefGlobalXPosOffset;
	   UserPrefGlobalXPosOffset =0; 
	}
	PipBorderControl();
}
void StopNOFlyingWindow(void)
{
	UserPrefOsdHPosition = StoreOsdHPosition;
	UserPrefOsdVPosition = StoreOsdVPosition;

	if(Is_SIDE_BY_SIDE())//120601 Edward for PBP main no cable msg overlap sub channel
		UserPrefGlobalXPosOffset = W_SavePIPIconXoffset;
}

void StoreACC2Mask(void) //KevinL 20090417 for PBP OSD noise
{
 	if(UserPrefPipMode == SIDE_BY_SIDE)
 	{
		if (gm_ReadRegWord(MISC_OCMMASK) & ACC_UPDATE_MASK)
		{
			W_ACC2MaskStatus = gm_ReadRegWord(MISC_OCMMASK)&ACC_UPDATE_MASK;
			gm_ClearRegBitsWord(MISC_OCMMASK, ACC_UPDATE_MASK);
		}
 	}
}

void RetoreACC2Mask(void)
{
 	if(UserPrefPipMode == SIDE_BY_SIDE)
 	{
		if (W_ACC2MaskStatus)
		{
			gm_SetRegBitsWord(MISC_OCMMASK, ACC_UPDATE_MASK);
			W_ACC2MaskStatus = 0;
		}
 	}
}

void SetRotatedIconXOffset(void) //KevinL 20090417 Save rotated Main Menu X Offset
{
   if(UserPrefOsdRotation == OSD_ROTATED_90)
   {
   

     	W_SaveRotatedIconXoffset = UserPrefGlobalXPosOffset;
		
		if(PanelHeight == 1200)//owen 20120620
      	UserPrefGlobalXPosOffset = 200;
		else if(PanelHeight == 1440)
      		UserPrefGlobalXPosOffset = 436;
      	       else
		UserPrefGlobalXPosOffset = 2;//200;
		
     
   }
}

void RestoreRotatedXOffsetAfterIconDraw(void) //KevinL 20090417 Update reoated X offset for Main Menu
{
   if(UserPrefOsdRotation == OSD_ROTATED_90)
   {   
   	UserPrefGlobalXPosOffset = W_SaveRotatedIconXoffset;	
       W_SaveRotatedIconXoffset = 0;	   
   } 
}

void SetPIPIconXYOffset(void) //KevinL 20090417 Save Main Menu XY Offset
{
   //if(UserPrefMenuOnOffFlag == On)//120605 Edward disable for PBP main icon position error
   {
      if(UserPrefGlobalXPosOffset !=0)
      {
	      W_SavePIPIconXoffset = UserPrefGlobalXPosOffset;
		UserPrefGlobalXPosOffset = 0;
      }

      if(UserPrefGlobalYPosOffset !=0)  	  
      {
	      W_SavePIPIconYoffset = UserPrefGlobalYPosOffset;
		UserPrefGlobalYPosOffset = 0;      
      }	 
   }  
}

void RestoreXYOffsetAfterPIPIconDraw(void) //KevinL 20090417 Update XY offset for Main Menu
{
   //if(UserPrefMenuOnOffFlag == On)//120605 Edward disable for PBP main icon position error
   {
       UserPrefGlobalXPosOffset = W_SavePIPIconXoffset;
    	UserPrefGlobalYPosOffset = W_SavePIPIconYoffset;
		W_SavePIPIconXoffset = 0;
		W_SavePIPIconYoffset = 0;
   }
}


void InitPIPIconPosition(void) //KevinL 20090120
{
    if((UserPrefPipMode == SIDE_BY_SIDE))
    {
        UserPrefPIPIconsXPos = 1003;
	   	UserPrefPIPIconsYPos = 260;		 	
		
		if (PanelH_1080)
	   	UserPrefPIPIconsYPos = 84;	
		
             if( PanelH_1440) //owen 120830)
             	{
             	UserPrefPIPIconsXPos = 996;
		UserPrefPIPIconsYPos = 220;	
             	}
		
		
    }
	else if(UserPrefPipMode == SIDE_BY_SIDE_TALL)
	{
        UserPrefPIPIconsXPos = 1003;
	   	UserPrefPIPIconsYPos = 30;		 	
		
		if (PanelH_1080)
	   	UserPrefPIPIconsYPos = 84;	
		
             if( PanelH_1440) //owen 120830)
             	{
             	UserPrefPIPIconsXPos = 996;
		UserPrefPIPIconsYPos = 220;	
             	}
		
		
    }	
    else if(UserPrefPipMode == SMALL_PIP)//owen 121004
	{

		switch (UserPrefPipPosition)
		{
			case TOP_LEFT:
				UserPrefPIPIconsXPos = 30;
				UserPrefPIPIconsYPos = 30;			   
			break;

			case BOTTOM_LEFT:
				UserPrefPIPIconsXPos = 30;
				if(UserPrefPipSize == SMALL_PIP)
				{
					if((PanelHeight == 1200)||(PanelHeight == 1080)||(PanelHeight == 2160))	
						UserPrefPIPIconsYPos = 1004;	
					else if(PanelHeight == 1440)
						UserPrefPIPIconsYPos = 980;
					else
						UserPrefPIPIconsYPos = 1003;					
				}
				else 	  			
					UserPrefPIPIconsYPos = 779;			
			break;

			case TOP_RIGHT:

			if(UserPrefPipSize == SMALL_PIP)
			{
				if((PanelHeight == 1200)||(PanelHeight == 1080)||(PanelHeight == 2160))	
					UserPrefPIPIconsXPos = 1602;	
				else if(PanelHeight == 1440)
					UserPrefPIPIconsXPos = 1599;//owen 20120710
				else if(PanelHeight == 1600)
					UserPrefPIPIconsXPos = 1599;
			}
			else 	  			
				UserPrefPIPIconsXPos = 1243;      	       	

			UserPrefPIPIconsYPos = 30;				   
			break;

			case BOTTOM_RIGHT:

				if(UserPrefPipSize == SMALL_PIP)
				{
					UserPrefPIPIconsXPos = 1599;					

					if((PanelHeight == 1200)||(PanelHeight == 1080)||(PanelHeight == 2160))
					{
						UserPrefPIPIconsYPos = 1004;
						UserPrefPIPIconsXPos = 1602;
					}
					else if(PanelHeight == 1440)
						UserPrefPIPIconsYPos = 980;	
					else if(PanelHeight == 1600)
						UserPrefPIPIconsYPos = 1003;
				}
				else 	  			
					UserPrefPIPIconsXPos = 1243;            

			break;
		}	
	}
	else if(UserPrefPipMode == LARGE_PIP)
	{
		switch (UserPrefPipPosition)
	   	{
			case TOP_LEFT:
			   	UserPrefPIPIconsXPos = 30;
			     	UserPrefPIPIconsYPos = 30;			   
			break;
			
			case BOTTOM_LEFT:
		      	   	UserPrefPIPIconsXPos = 30;
				
				if(UserPrefPipSize == LARGE_PIP)	  
				{
					if((PanelHeight == 1200)||(PanelHeight == 1080)||(PanelHeight == 2160))
			   			UserPrefPIPIconsYPos = 646;//wei 140902  //846;				
					else if(PanelHeight == 1440)
						UserPrefPIPIconsYPos = 862;
					else if(PanelHeight == 1600)
						UserPrefPIPIconsYPos = 896;	
				}
				else 	  			
			   		UserPrefPIPIconsYPos = 779;			
			break;
			
			case TOP_RIGHT:
			
				if(UserPrefPipSize == LARGE_PIP)	  
				{
					if((PanelHeight == 1200)||(PanelHeight == 1080)||(PanelHeight == 2160))
		    				UserPrefPIPIconsXPos = 1030;//wei 140902  //1354;	
					else if(PanelHeight == 1440)
						UserPrefPIPIconsXPos = 1428;	
					else if(PanelHeight == 1600)
		    				UserPrefPIPIconsXPos = 1428;				
				}
				else 	  			
			     		UserPrefPIPIconsXPos = 1243;      	       	
				
		     		UserPrefPIPIconsYPos = 30;				   
			
			break;
			
			case BOTTOM_RIGHT:

		       	if(UserPrefPipSize == LARGE_PIP)	  
			    	{
					if((PanelHeight == 1200)||(PanelHeight == 1080)||(PanelHeight == 2160))
					{
		    				UserPrefPIPIconsXPos = 1030;//wei 140902    //1354;	
						UserPrefPIPIconsYPos = 646; //wei 140902     //846;//owen 20120719	
					}
					else if(PanelHeight == 1440)//owen 20120625
					{
						UserPrefPIPIconsXPos = 1428;	
				      		UserPrefPIPIconsYPos = 862;
					}
					else if(PanelHeight == 1600)
					{
		    	 			UserPrefPIPIconsXPos = 1428;
						UserPrefPIPIconsYPos = 896;
					}
				}			
				else 	  			
			    		UserPrefPIPIconsXPos = 1243;            

			break;
		}	
	}
	else if(UserPrefPipMode == MEDIUM_PIP)
	{

		switch (UserPrefPipPosition)
		{
			case TOP_LEFT:
				UserPrefPIPIconsXPos = 30;
				UserPrefPIPIconsYPos = 30;			   
			break;

			case BOTTOM_LEFT:
			UserPrefPIPIconsXPos = 30;

			if(UserPrefPipSize == MEDIUM_PIP)	  
			{
				if((PanelHeight == 1200)||(PanelHeight == 1080)||(PanelHeight == 2160))	
					UserPrefPIPIconsYPos = 923;			
				else if(PanelHeight == 1440)
					UserPrefPIPIconsYPos = 900;
				else
					UserPrefPIPIconsYPos = 923;					
			}			
			else 	  			
				UserPrefPIPIconsYPos = 779;			
			break;


			case TOP_RIGHT:

			if(UserPrefPipSize == MEDIUM_PIP)	  
			{

				if((PanelHeight == 1200)||(PanelHeight == 1080)||(PanelHeight == 2160))	
					UserPrefPIPIconsXPos = 1475;			
				else if(PanelHeight == 1440)
					UserPrefPIPIconsXPos = 1475;//owen 20120710
				else if(PanelHeight == 1600)
					UserPrefPIPIconsXPos = 1475;

			}			
			else 	  			
				UserPrefPIPIconsXPos = 1243;      	       	

			UserPrefPIPIconsYPos = 30;				   

			break;


			case BOTTOM_RIGHT:

			UserPrefPIPIconsXPos = 1475;			
			if(UserPrefPipSize == MEDIUM_PIP)	  
			{

				if((PanelHeight == 1200)||(PanelHeight == 1080)||(PanelHeight == 2160))
					UserPrefPIPIconsYPos = 950;
				else if(PanelHeight == 1440)
					UserPrefPIPIconsYPos = 980;	
				else if(PanelHeight == 1600)
					UserPrefPIPIconsYPos = 1003;
			}
			else 	  			
				UserPrefPIPIconsYPos = 980;	

			break;
		}	
	}	
   }


void InitPIPMessagePosition(void) 
{
   if(UserPrefOsdRotation == OSD_NON_ROTATED)
   {
       	if((UserPrefPipMode == SIDE_BY_SIDE)||(UserPrefPipMode == SIDE_BY_SIDE_TALL))
      	{
		   if((PanelHeight == 1200)||((PanelHeight == 1080)&&(PanelWidth != 2560)))
		   {
			   UserPrefPIPMessagesXPos = 1168;
			   UserPrefPIPMessagesYPos = 600;	   
		   }				   
		   else if(PanelHeight == 1440)//owen 20120625
		   {
			   UserPrefPIPMessagesXPos = 1348;	// 1648
			   UserPrefPIPMessagesYPos = 600;   
		   }
		   else if((PanelHeight == 1600)||((PanelHeight == 1080)&&(PanelWidth == 2560)))
		   {
			   UserPrefPIPMessagesXPos = 1348;
			   UserPrefPIPMessagesYPos = 600;			  
		   }
		   else if(PanelHeight == 2160)
		   {
			   UserPrefPIPMessagesXPos = 1200;
			   UserPrefPIPMessagesYPos = 600;   
		   }		   
		   else //if(PanelHeight == 1050)
		   {
			   UserPrefPIPMessagesXPos = 968;
			   UserPrefPIPMessagesYPos = 600;	
		   }
      	}
		else if(UserPrefPipMode == SMALL_PIP)
		{
			if(UserPrefPipPosition == TOP_LEFT) 
		   	{
				if((PanelHeight == 1200)||(PanelHeight == 1080))
				{
					UserPrefPIPMessagesXPos = 32;
					UserPrefPIPMessagesYPos = 48;		
				}					
				else if(PanelHeight == 1440)//owen 20120625
				{
					UserPrefPIPMessagesXPos = 76;
					UserPrefPIPMessagesYPos = 74;	
				}
				else if(PanelHeight == 1600)
				{
					UserPrefPIPMessagesXPos = 76;
					UserPrefPIPMessagesYPos = 68;			   
				}
				else if(PanelHeight == 2160)
				{
					UserPrefPIPMessagesXPos = 76;
					UserPrefPIPMessagesYPos = 68;			   
				}				
				else
				{
					UserPrefPIPMessagesXPos = 32;
					UserPrefPIPMessagesYPos = 48;	 
				}
		 	}
		   	else if(UserPrefPipPosition == BOTTOM_LEFT)
		   	{
				if((PanelHeight == 1200)||(PanelHeight == 1080))
				{
  	           		UserPrefPIPMessagesXPos = 32;
     				UserPrefPIPMessagesYPos = 1170;	
				}
				else if(PanelHeight == 1440)//owen 20120625	
				{
			       	UserPrefPIPMessagesXPos = 76;
     				UserPrefPIPMessagesYPos = 1140;	
				}
				else if(PanelHeight == 1600)
				{
           			UserPrefPIPMessagesXPos = 76;
     				UserPrefPIPMessagesYPos = 1146;				   	
				}
				else if(PanelHeight == 2160)
				{
           			UserPrefPIPMessagesXPos = 76;
     				UserPrefPIPMessagesYPos = 1146;				   	
				}				
				else
				{
  	           		UserPrefPIPMessagesXPos = 32;
     				UserPrefPIPMessagesYPos = 1170;	
				}					
		  	 }
		   	else if(UserPrefPipPosition == TOP_RIGHT)
		   	{
		   		if((PanelHeight == 1200)||(PanelHeight == 1080))
				{
      	           	UserPrefPIPMessagesXPos = 1906;
	     			UserPrefPIPMessagesYPos = 48;	
				}
				else if(PanelHeight == 1440)//owen 20120625
				{
					UserPrefPIPMessagesXPos = 1858;
	     			UserPrefPIPMessagesYPos = 74;	
				}
				else if(PanelHeight == 1600)
				{
      	           	UserPrefPIPMessagesXPos = 1858;
	     			UserPrefPIPMessagesYPos = 68;				   
		  		}
				else if(PanelHeight == 2160)
				{
      	           	UserPrefPIPMessagesXPos = 1906;
	     			UserPrefPIPMessagesYPos = 68;				   
		  		}				
				else
				{
      	           	UserPrefPIPMessagesXPos = 1906;
	     			UserPrefPIPMessagesYPos = 48;	
				}					
		  	}
		   	else if(UserPrefPipPosition == BOTTOM_RIGHT)
		   	{
		   		if((PanelHeight == 1200)||(PanelHeight == 1080))
				{
           			UserPrefPIPMessagesXPos = 1906;
     				UserPrefPIPMessagesYPos = 1170;	
		   		}
				else if(PanelHeight == 1440)//owen 20120625
				{
					UserPrefPIPMessagesXPos = 1858;
     				UserPrefPIPMessagesYPos = 1140;	
				}
				else if(PanelHeight == 1600)
				{
           			UserPrefPIPMessagesXPos = 1858;
     				UserPrefPIPMessagesYPos = 1146;				   
		   		}
				else if(PanelHeight == 1600)
				{
           			UserPrefPIPMessagesXPos = 1906;
     				UserPrefPIPMessagesYPos = 1170;				   
		   		}				
				else
				{
  	           		UserPrefPIPMessagesXPos = 1906;
     				UserPrefPIPMessagesYPos = 1170;	
		   		}					
	       	}
		
	}	
	else if(UserPrefPipMode == LARGE_PIP)
	{
			if(UserPrefPipPosition == TOP_LEFT) 
		   	{
		   		if((PanelHeight == 1200)||(PanelHeight == 1080)||(PanelHeight == 2160))
				{
           			UserPrefPIPMessagesXPos = 180;
     				UserPrefPIPMessagesYPos = 136;		
		   		}
				else if(PanelHeight == 1440)//owen 20120625
				{
           			UserPrefPIPMessagesXPos = 174;
     				UserPrefPIPMessagesYPos = 140;					
				}
				else
		       {
  	           		UserPrefPIPMessagesXPos = 175;
	     			UserPrefPIPMessagesYPos = 134;			   
		 		}					
		 	}
		   	else if(UserPrefPipPosition == BOTTOM_LEFT)
		   	{
		   		if((PanelHeight == 1200)||(PanelHeight == 1080)||(PanelHeight == 2160))
				{
  	           			UserPrefPIPMessagesXPos = 180;
	     				UserPrefPIPMessagesYPos = 1080;		
		   		}	
				else if(PanelHeight == 1440)//owen 20120625
				{
					UserPrefPIPMessagesXPos = 174;
     				UserPrefPIPMessagesYPos = 1074;	
				}
				else
				{
  	           		UserPrefPIPMessagesXPos = 175;
	     			UserPrefPIPMessagesYPos = 1076;				   
		  	 	}					
		  	 }
		   	else if(UserPrefPipPosition == TOP_RIGHT)
		   	{
		   		if((PanelHeight == 1200)||(PanelHeight == 1080)||(PanelHeight == 2160))
				{
           			UserPrefPIPMessagesXPos = 1756;
     				UserPrefPIPMessagesYPos = 136;			
		   		}
				else if(PanelHeight == 1440)//owen 20120625
				{
			      	UserPrefPIPMessagesXPos = 1760;
     				UserPrefPIPMessagesYPos = 140;		
				}
				else
				{
           			UserPrefPIPMessagesXPos = 1754;
     				UserPrefPIPMessagesYPos = 134;				   
		  		}				
		  	}
		   	else if(UserPrefPipPosition == BOTTOM_RIGHT)
		   	{
		   		if((PanelHeight == 1200)||(PanelHeight == 1080)||(PanelHeight == 2160))
				{
           			UserPrefPIPMessagesXPos = 1756;
     				UserPrefPIPMessagesYPos = 1080;		
		   		}
				else if(PanelHeight == 1440)//owen 20120625
				{
			      	UserPrefPIPMessagesXPos = 1760;
     				UserPrefPIPMessagesYPos = 1074;	
				}
				else
				{
           			UserPrefPIPMessagesXPos = 1754;
     				UserPrefPIPMessagesYPos = 1076;				   
		   		}					
			}	
		
	}
	else if(UserPrefPipMode == MEDIUM_PIP)
	{
			if(UserPrefPipPosition == TOP_LEFT) 
		   	{
				if((PanelHeight == 1200)||(PanelHeight == 1080)||(PanelHeight == 2160))
				{
					UserPrefPIPMessagesXPos = 160;
					UserPrefPIPMessagesYPos = 116;		
				}
				else if(PanelHeight == 1440)//owen 20120625
				{
					UserPrefPIPMessagesXPos = 154;
					UserPrefPIPMessagesYPos = 120;					
				}
				else
				{
					UserPrefPIPMessagesXPos = 155;
					UserPrefPIPMessagesYPos = 114;			   
				}					
		 	}
		   	else if(UserPrefPipPosition == BOTTOM_LEFT)
		   	{
				if((PanelHeight == 1200)||(PanelHeight == 1080)||(PanelHeight == 2160))
				{
					UserPrefPIPMessagesXPos = 170;
					UserPrefPIPMessagesYPos = 1060;		
				}	
				else if(PanelHeight == 1440)//owen 20120625
				{
					UserPrefPIPMessagesXPos = 164;
					UserPrefPIPMessagesYPos = 1054;	
				}
				else
				{
					UserPrefPIPMessagesXPos = 165;
					UserPrefPIPMessagesYPos = 1056;				   
				}	
		  	 }
		   	else if(UserPrefPipPosition == TOP_RIGHT)
		   	{
				if((PanelHeight == 1200)||(PanelHeight == 1080)||(PanelHeight == 2160))
				{
					UserPrefPIPMessagesXPos = 1736;
					UserPrefPIPMessagesYPos = 126;			
				}
				else if(PanelHeight == 1440)//owen 20120625
				{
					UserPrefPIPMessagesXPos = 1740;
					UserPrefPIPMessagesYPos = 130;		
				}
				else
				{
					UserPrefPIPMessagesXPos = 1734;
					UserPrefPIPMessagesYPos = 124;				   
				}	
		  	}
		   	else if(UserPrefPipPosition == BOTTOM_RIGHT)
		   	{
				if((PanelHeight == 1200)||(PanelHeight == 1080)||(PanelHeight == 2160))
				{
					UserPrefPIPMessagesXPos = 1736;
					UserPrefPIPMessagesYPos = 1060;		
				}
				else if(PanelHeight == 1440)//owen 20120625
				{
					UserPrefPIPMessagesXPos = 1740;
					UserPrefPIPMessagesYPos = 1054;	
				}
				else
				{
					UserPrefPIPMessagesXPos = 1734;
					UserPrefPIPMessagesYPos = 1056;				   
				}		
			}	
		}
		else if(UserPrefPipMode == HUGE_PIP)
		{
			if(UserPrefPipPosition == TOP_LEFT) 
			{
				UserPrefPIPMessagesXPos = 245;
				UserPrefPIPMessagesYPos = 168;			   
			}
			else if(UserPrefPipPosition == BOTTOM_LEFT)
			{
				UserPrefPIPMessagesXPos = 245;
				UserPrefPIPMessagesYPos = 1038;				   
			}
			else if(UserPrefPipPosition == TOP_RIGHT)
			{
				UserPrefPIPMessagesXPos = 1690;
				UserPrefPIPMessagesYPos = 168;				   
			}
			else if(UserPrefPipPosition == BOTTOM_RIGHT)
			{
				UserPrefPIPMessagesXPos = 1690;
				UserPrefPIPMessagesYPos = 1038;				   
			}		
		}
    }
   else if(UserPrefOsdRotation == OSD_ROTATED_90)
   {
       if((UserPrefPipMode == SIDE_BY_SIDE)||(UserPrefPipMode == SIDE_BY_SIDE_TALL))
      	{
      	     UserPrefPIPMessagesXPos = 1600;
	     UserPrefPIPMessagesYPos = 600;		 	
      	}
	else if(UserPrefPipMode == SMALL_PIP)
	{
	       if(UserPrefPipSize == SMALL_PIP)
		{
			if(UserPrefPipPosition == TOP_LEFT) 
		   	{
      	           		UserPrefPIPMessagesXPos = 30;
	     			UserPrefPIPMessagesYPos = 40;			   
		 	}
		   	else if(UserPrefPipPosition == BOTTOM_LEFT)
		   	{
      	           		UserPrefPIPMessagesXPos = 30;
	     			UserPrefPIPMessagesYPos = 1166;				   
		  	 }
		   	else if(UserPrefPipPosition == TOP_RIGHT)
		   	{
      	           		UserPrefPIPMessagesXPos = 1902;
	     			UserPrefPIPMessagesYPos = 40;				   
		  	}
		   	else if(UserPrefPipPosition == BOTTOM_RIGHT)
		   	{
      	           		UserPrefPIPMessagesXPos = 1902;
	     			UserPrefPIPMessagesYPos = 1166;				   
		   	}
	       }
		else if(UserPrefPipSize == MEDIUM_PIP)
		{
			if(UserPrefPipPosition == TOP_LEFT) 
		   	{
      	           		UserPrefPIPMessagesXPos = 100;
	     			UserPrefPIPMessagesYPos = 90;			   
		 	}
		   	else if(UserPrefPipPosition == BOTTOM_LEFT)
		   	{
      	           		UserPrefPIPMessagesXPos = 100;
	     			UserPrefPIPMessagesYPos = 1122;				   
		  	 }
		   	else if(UserPrefPipPosition == TOP_RIGHT)
		   	{
      	           		UserPrefPIPMessagesXPos = 1827;
	     			UserPrefPIPMessagesYPos = 90;				   
		  	}
		   	else if(UserPrefPipPosition == BOTTOM_RIGHT)
		   	{
      	           		UserPrefPIPMessagesXPos = 1827;
	     			UserPrefPIPMessagesYPos = 1122;				   
		   	}		
		}
		else if(UserPrefPipSize == LARGE_PIP)
		{
			if(UserPrefPipPosition == TOP_LEFT) 
		   	{
      	           		UserPrefPIPMessagesXPos = 175;
	     			UserPrefPIPMessagesYPos = 134;			   
		 	}
		   	else if(UserPrefPipPosition == BOTTOM_LEFT)
		   	{
      	           		UserPrefPIPMessagesXPos = 175;
	     			UserPrefPIPMessagesYPos = 1076;				   
		  	 }
		   	else if(UserPrefPipPosition == TOP_RIGHT)
		   	{
      	           		UserPrefPIPMessagesXPos = 1754;
	     			UserPrefPIPMessagesYPos = 134;				   
		  	}
		   	else if(UserPrefPipPosition == BOTTOM_RIGHT)
		   	{
      	           		UserPrefPIPMessagesXPos = 1754;
	     			UserPrefPIPMessagesYPos = 1706;				   
		   	}		
		}	
		else //HUGE_PIP
		{
			if(UserPrefPipPosition == TOP_LEFT) 
		   	{
      	           		UserPrefPIPMessagesXPos = 245;
	     			UserPrefPIPMessagesYPos = 168;			   
		 	}
		   	else if(UserPrefPipPosition == BOTTOM_LEFT)
		   	{
      	           		UserPrefPIPMessagesXPos = 245;
	     			UserPrefPIPMessagesYPos = 1038;				   
		  	 }
		   	else if(UserPrefPipPosition == TOP_RIGHT)
		   	{
      	           		UserPrefPIPMessagesXPos = 1690;
	     			UserPrefPIPMessagesYPos = 168;				   
		  	}
		   	else if(UserPrefPipPosition == BOTTOM_RIGHT)
		   	{
      	           		UserPrefPIPMessagesXPos = 1690;
	     			UserPrefPIPMessagesYPos = 1038;				   
		   	}		
		}
	}	
   }
   else
   {
 	UserPrefPIPMessagesXPos = 0;
	UserPrefPIPMessagesYPos = 0;
   	gm_Print("OSD 180 and 270 degree rotation do not support!!!",0);
   }
}

void StorePresetModes(void)
{
	//if (IsColorMode_GRAPHIC)
		B_PresetModesStore = UserPrefGraphicsImageMode;

	//Neil120724+
	if(IsPresetMode_GRAPHICS_COLOR_SPACE)
		B_PresetColorSpace = UserPrefPresetColorSpace;
	//else
	//	B_PresetModesStore = UserPrefVideoImageMode;
}

void RestorePresetModes(void)
{	
	//if (IsColorMode_GRAPHIC)
	{
		if (B_PresetModesStore == UserPrefGraphicsImageMode)
			return;

		UserPrefGraphicsImageMode = B_PresetModesStore;
	}
	/*
	else
	{
		if (B_PresetModesStore == UserPrefVideoImageMode)
			return;

		UserPrefVideoImageMode = B_PresetModesStore;
	}
	*/
	//Neil120724+
	if(IsPresetMode_GRAPHICS_COLOR_SPACE)
		UserPrefPresetColorSpace = B_PresetColorSpace;
	
	AdjustPresetModes();
	SavePortSchemeDependentSettings();
}

void ResetColorSettings(void)
{
	//Input Color Format
	//Gamma
	//Mode Selection
	//Preset Mode
	BYTE B_Index; 
	BYTE B_ColorSpace;   

	msg_i("Reset Color Setting",0);   
	
	//yvonne 20120906 set black screen before colorsettings reset
	gm_BlenderOnOff(gmvb_MainChannel,OFF);
	gm_SyncUpdateWait(gmvb_MainChannel,INPUT_OUTPUT_CLOCK);
	
#ifdef Gamma_DUM_Initial_Check
	GAMMA_DUM_NVRAM_CHECK(0);	//check sRGB
	GAMMA_DUM_NVRAM_CHECK(1);	//check adobeRGB
	GAMMA_DUM_NVRAM_CHECK(2);	//check DUM
#endif

	B_Index = InputConvertScheme(UserPrefCurrentInputMain);
	PresetModeUserPreferences = PresetModeFactoryDefaultInROM[B_Index]; //Reset current input	
	UserPrefColorFormat =  SchemeFactoryDefaultInROM[B_Index].ColorFormat;
	UserPrefColorMode = SchemeFactoryDefaultInROM[B_Index].ColorMode;
	UserPrefGraphicsImageMode = SchemeFactoryDefaultInROM[B_Index].GraphicsImageMode;
	UserPrefVideoImageMode = SchemeFactoryDefaultInROM[B_Index].VideoImageMode;	
	UserPrefGammaMode = SchemeFactoryDefaultInROM[B_Index].GammaMode;
	UserPrefPresetColorTemp = SchemeFactoryDefaultInROM[B_Index].PresetColorTemp;
	UserPrefPresetColorSpace = SchemeFactoryDefaultInROM[B_Index].PresetColorSpace;
	UserPrefPresetPrinterPrevue= SchemeFactoryDefaultInROM[B_Index].PresetPrinterPrevue;
	UserPrefPortTNR = SchemeFactoryDefaultInROM[B_Index].PortTNR;
	UserPrefTNR = UserPrefPortTNR;
	//120608 Edward save the Hue / Saturation 
	{
		UserPrefHue_Movie= SchemeFactoryDefaultInROM[B_Index].Hue_Movie;
		UserPrefSaturation_Movie= SchemeFactoryDefaultInROM[B_Index].Saturation_Movie;
		
		UserPrefHue_Game= SchemeFactoryDefaultInROM[B_Index].Hue_Game;
		UserPrefSaturation_Game= SchemeFactoryDefaultInROM[B_Index].Saturation_Game;
		
		UserPrefHue = SchemeFactoryDefaultInROM[B_Index].Hue;
		UserPrefSaturation = SchemeFactoryDefaultInROM[B_Index].Saturation;

		//120907 Edward reset all seperate setting as well
		/*          		
        Brightness/Contrast will only be reset by Factory Reset.
        Reset Color Settings will only reset those settings in the "Color Settings" menu.
		*/
		#if 0 //KevinL 20120920
		UserPrefBrightness = SchemeFactoryDefaultInROM[B_Index].Brightness;
		UserPrefContrast = SchemeFactoryDefaultInROM[B_Index].Contrast;

		UserPrefBrightness_All = SchemeFactoryDefaultInROM[B_Index].Brightness_All;
		UserPrefContrast_All = SchemeFactoryDefaultInROM[B_Index].Contrast_All;
		
		UserPrefBrightness_Text = SchemeFactoryDefaultInROM[B_Index].Brightness_Text; 
		UserPrefContrast_Text = SchemeFactoryDefaultInROM[B_Index].Contrast_Text; 
        #endif
        
		UserPrefPortSharpness = SchemeFactoryDefaultInROM[B_Index].Sharpness;		
		UserPrefPortSharpness_All = SchemeFactoryDefaultInROM[B_Index].Sharpness_All;		
		UserPrefPortSharpness_Text = SchemeFactoryDefaultInROM[B_Index].Sharpness_Text; 
	}	
	
	UserPrefSaturation_YPbPr = SchemeFactoryDefaultInROM[B_Index].Saturation_YPbPr;	
	UserPrefUserRedGain = SchemeFactoryDefaultInROM[B_Index].UserRedGain; 	
	UserPrefUserGreenGain = SchemeFactoryDefaultInROM[B_Index].UserGreenGain; 	
	UserPrefUserBlueGain = SchemeFactoryDefaultInROM[B_Index].UserBlueGain; 	
	UserPrefUserRedOff = SchemeFactoryDefaultInROM[B_Index].UserRedOff; 	
	UserPrefUserGreenOff = SchemeFactoryDefaultInROM[B_Index].UserGreenOff; 	
	UserPrefUserBlueOff = SchemeFactoryDefaultInROM[B_Index].UserBlueOff; 		

	UserPrefCustomColorSelection = FactoryDefaultInROM.CustomColorSelection;

	
	/*if(IsPresetMode_GRAPHICS_TEXT)//20120730 Add by KevinChen for ResetColorSetting need to reload some values.
	{
		UserPrefBrightness = UserPrefBrightness_Text;
		UserPrefContrast = UserPrefContrast_Text;
		UserPrefPortSharpness = UserPrefPortSharpness_Text;
	}	
	else if(IsColorSpace_GRAPHICS_CAL1)//120806 Edward add for CAL1/CAL2 save seperate
	{
		UserPrefBrightness = UserPrefBrightness_CAL1;
		UserPrefContrast = UserPrefContrast_CAL1;
		UserPrefPortSharpness = UserPrefPortSharpness_All;
	}
	else if(IsColorSpace_GRAPHICS_CAL2)
	{
		UserPrefBrightness = UserPrefBrightness_CAL2;
		UserPrefContrast = UserPrefContrast_CAL2;
		UserPrefPortSharpness = UserPrefPortSharpness_All;
	}
	else
	{
		UserPrefBrightness = UserPrefBrightness_All;
		UserPrefContrast = UserPrefContrast_All;
		UserPrefPortSharpness = UserPrefPortSharpness_All;
	}*/
	
	ResetACM6AxisSettings();	
	SaveModeIndependentSettings();	
	SavePresetModesDependentSettings(); 
	
	if(UserPrefGameMode == On)	//yvonne 20120922 solve color reset will osd disappear issue.
	{
		 UserPrefGameMode = Off; 
		 gm_WriteRegWord(SYNC_CONTROL, 2);							   
		 B_DisplayLockMethod = DISP_LOCK_DFL;
		 gm_SetRegBitsWord(DDDS1_CONTROL, DDDS1_FORCE_OPLOOP);			 
		 UserPrefStateTempFlag = INPUT_COLOR_FORMAT_STATE;
//		 gm_Print("@@#Reset game mode",0);
	}	 
	AdjustColorMode();
	AdjustHue();
	AdjustSaturation();
	AdjustColorFormat();
	UserPrefAdjustInputColorFormatByOSD = FALSE; 	//yvonne 20120915 for color format won't follow the infoframe
//&*&*&*S 20120315:SWRD4001: Kordonm(0)  Modify for OSD Workbench
	AdjustPrinterPrevue();
//&*&*&*E 20120315 : Kordonm(1)  Modify for OSD Workbench
	StorePresetModes();
	AdjustTNR();

}

void ResetAudioSetting(void) //KevinL 20110809+
{
	UserPrefButtonSoundOff = FactoryDefaultInROM.ButtonSoundOff;
	UserPrefPwrAudio = FactoryDefaultInROM.PwrAudio;
	SaveModeIndependentSettings();
}

void ResetDisplaySettings (void) 
{
	//WideMode
	//HP
	//VP
	//Sharpness
	//Zoom
	//PCLK
	//Phase
	//DCR
	BYTE B_Index; 

	B_Index = InputConvertScheme(UserPrefCurrentInputMain);	   
	//UserPrefAspectRatioMain = SchemeFactoryDefaultInROM[B_Index].AspectRatio;//calo add for the display reset with wide mode 090207
	if(UserPrefPortAspectRatio != SchemeFactoryDefaultInROM[B_Index].AspectRatio)		//Neil 20090413+
	{
		UserPrefPortAspectRatio = SchemeFactoryDefaultInROM[B_Index].AspectRatio;
		AdjustAspectRatioMain();
	}
	
	if(UserPrefPortImageZoom != SchemeFactoryDefaultInROM[B_Index].ImageZoom)		//Neil 20090413+
	{
		UserPrefPortImageZoom = SchemeFactoryDefaultInROM[B_Index].ImageZoom;
		UserPrefMainImageZoom = UserPrefPortImageZoom;
		AdjustZoom();
	}
	UserPrefDCR = SchemeFactoryDefaultInROM[B_Index].PresetModes[WB_ImageScheme].DCR;
//owen 20120621	UserPrefDUMSwitch = SchemeFactoryDefaultInROM[B_Index].PresetModes[WB_ImageScheme].DUMSwitch;
	UserPrefDUMSwitch = FactoryDefaultInROM.DUMSwitch;//owen 20120621

	
	//UserPrefSharpness = SchemeFactoryDefaultInROM[B_Index].Sharpness; 
	if(IsPresetMode_GRAPHICS_TEXT)//120426 Edward add for DELL request save for seperate in text mode
		UserPrefPortSharpness= SchemeFactoryDefaultInROM[B_Index].Sharpness_Text;
	else
		UserPrefPortSharpness = SchemeFactoryDefaultInROM[B_Index].Sharpness_All; //calo add for the display reset with sharpness 090207

	UserPrefSharpnessPip = SchemeFactoryDefaultInROM[B_Index].Sharpness_All; 
	AdjustSharpness();

	AdjustTNR();
		
	if (IsMainVGAInput()) 
	{
		UserPrefHStart = stInput[CH_A].Hstart;
		UserPrefVStart = stInput[CH_A].Vstart;
		UserPrefHTotal = stInput[CH_A].Htotal;
              UserPrefHSyncDelay = 0;
              UserPrefVSyncDelay = 0;			  
		AdjustHStart();
		SaveGrModeDependentSettings();
	}

//&*&*&*S 20120315:SWRD4001: Kordonm(0)  Modify for OSD Workbench
	//UserPrefDUMSwitch = UNIFORMITY_Table1;
	//Color_Uniformity_Test(UNIFORMITY_OFF);//test
	AdjustDUMSwitch();

	UserPrefVwdFlag = VWD_DEMO_OFF;	//NVRam_ReadByte(ModeIndependentStart + VwdDemo_offset);
	VWD_ApplyHighLightWindow();
//&*&*&*E 20120315 : Kordonm(1)  Modify for OSD Workbench

//&*&*&*S 20120517 :SWRD4007: Kordonm(0)  Modify for OSD Workbench				
	UserPrefContrast_Vwd= FactoryDefaultInROM.Contrast_Vwd;
	UserPrefSharpness_Vwd= FactoryDefaultInROM.Sharpness_Vwd;
	UserPrefOffset_Vwd= FactoryDefaultInROM.Offset_Vwd;
	UserPrefHue_Vwd= FactoryDefaultInROM.Hue_Vwd;
	UserPrefSaturation_Vwd= FactoryDefaultInROM.Saturation_Vwd;	
//&*&*&*E 20120515 : Kordonm(1)  Modify for OSD Workbench

  	UserPrefVwdDemo = FactoryDefaultInROM.VwdDemo; //owen 20120607 
	//VWD_ApplyHighLightWindow();	//20120919 for Display reset, VWD window didn't apply to default setting
	VWD_ClearHighlighWindow();// 20121212 yvonne do not change color settings

	UserPrefDPSWitch = FactoryDefaultInROM.DPSWitch; //20121211 yvonne for DP MST reset
	   AdjustDPSwitch();//120808 Edward restore DP setting
	   
	SaveModeIndependentSettings();
	SavePortSchemeDependentSettings();			
}

void ResetACM6AxisSettings(void)//KevinL 20081203
{
	BYTE B_Index; 

	B_Index = InputConvertScheme(UserPrefCurrentInputMain);
	UserPrefAcm3RedSat = SchemeFactoryDefaultInROM[B_Index].Acm3RedSat; 	
	UserPrefAcm3GreenSat = SchemeFactoryDefaultInROM[B_Index].Acm3GreenSat; 	
	UserPrefAcm3BlueSat = SchemeFactoryDefaultInROM[B_Index].Acm3BlueSat; 	
	UserPrefAcm3CyanSat = SchemeFactoryDefaultInROM[B_Index].Acm3CyanSat; 	
	UserPrefAcm3YellowSat = SchemeFactoryDefaultInROM[B_Index].Acm3YellowSat; 	
	UserPrefAcm3MagentaSat = SchemeFactoryDefaultInROM[B_Index].Acm3MagentaSat; 	
	
	UserPrefAcm3RedHue = SchemeFactoryDefaultInROM[B_Index].Acm3RedHue; 	
	UserPrefAcm3GreenHue = SchemeFactoryDefaultInROM[B_Index].Acm3GreenHue; 	
	UserPrefAcm3BlueHue = SchemeFactoryDefaultInROM[B_Index].Acm3BlueHue; 	
	UserPrefAcm3CyanHue = SchemeFactoryDefaultInROM[B_Index].Acm3CyanHue; 	
	UserPrefAcm3YellowHue = SchemeFactoryDefaultInROM[B_Index].Acm3YellowHue; 	
	UserPrefAcm3MagentaHue = SchemeFactoryDefaultInROM[B_Index].Acm3MagentaHue; 		
	SavePortSchemeDependentSettings();	

	//120523 Edward reset 6 AXIS color setting
	{// ColorWarp
      BYTE i;

      #if (FEATURE_COLORWARP == REV1)
      for(i=0; i<6; i++)
      #else
      for(i=0; i<8; i++)
      #endif
      {
         #if (FEATURE_COLORWARP == REV1)
         AdjustSixAxisColor(i);
         #else // (FEATURE_COLORWARP == REV2)
         AdjustColorWarp(i);
         #endif // (FEATURE_COLORWARP == REV1)
      }
   }
}

void NullFunction(void)
{
	;
}

#ifdef INPUTSCAN

BOOL IsInputScanCompleted(void) //KevinL 20090407
{
   return (b_IsInputScanProcessingFlag==FALSE);
}

BOOL IsInputOriInputVGA(void)
{
       return (OriginalInput == ALL_BOARDS_VGA1);
}

BOOL IsInputOriInputDVI1(void)
{
       return (OriginalInput == ALL_BOARDS_DVI1);
}

BOOL IsInputOriInputDVI2(void)
{
       return (OriginalInput == ALL_BOARDS_DVI2);
}

void InputScan(void)
{
	if (gm_TimerCheck(INPUT_SCAN_TMR) != TIMER_TMO)
		return;

	if(b_IsInputScanProcessingFlag == FALSE) //KevinL 20090413
	   	return;
	   
	//gm_Printf("InputScan %d times",B_INPUTSCAN_LOOP);
	gm_WaitForDisplaySync(2);	//wait for 2 frames or 100ms



	if(gm_TimingIsStable(gmvb_MainChannel) == TRUE)
	{	
		gm_TimerStop(INPUT_SCAN_TMR);
		b_IsInputScanProcessingFlag = FALSE; 
              UserPrefHideIconFlag = FALSE; 		
		return;
	}

	if(B_INPUTSCAN_LOOP >= NUM_OF_SCAN_SOURCE) //KevinL 20090413  
	{
		//gm_Print("InputScan restore Input : %d",OriginalInput);
		UserPrefCurrentInputMain = OriginalInput;
		AdjustCurrentInputMain();
		gm_TimerStop(INPUT_SCAN_TMR);
		b_IsInputScanProcessingFlag = FALSE;
		#ifdef CHECK_PxP_STATES_AC_ON
		B_MainNoSyncCheckState = STOP_CHECK_FOR_ICON; //KevinL 20090314
		#endif
              UserPrefHideIconFlag = FALSE; 		
		return;
	}
	
	//Do Inputscan only at the following main/pip selection
	#ifdef CHECK_PxP_STATES_AC_ON
	if (((UserPrefCurrentInputMain== ALL_BOARDS_VGA1)||(UserPrefCurrentInputMain == ALL_BOARDS_DVI1)||(UserPrefCurrentInputMain == ALL_BOARDS_DVI2)) && (B_StoredPipMode == NO_PIP)) //KevinL 20090314
	#else
	if ((UserPrefCurrentInputMain== ALL_BOARDS_VGA1)||(UserPrefCurrentInputMain == ALL_BOARDS_DVI1)||(UserPrefCurrentInputMain == ALL_BOARDS_DVI2))//KevinL 20090314
	#endif
	{
	
		switch (UserPrefCurrentInputMain) //KevinL 20090318 remove cable detect
		{
			case ALL_BOARDS_VGA1:				
				   UserPrefCurrentInputMain = ALL_BOARDS_DVI1;
				   AdjustCurrentInputMain();					
				break;
			
			case ALL_BOARDS_DVI1:			
				  UserPrefCurrentInputMain = ALL_BOARDS_DVI2;			
	 			  AdjustCurrentInputMain();						
				break;
				
			case ALL_BOARDS_DVI2:					
				  UserPrefCurrentInputMain = ALL_BOARDS_VGA1;
				  AdjustCurrentInputMain();					
				break;
			default:				
				    B_INPUTSCAN_LOOP += 2; //Exit						
				break;
		}
		gm_TimerStart(INPUT_SCAN_TMR, PERIOD_OF_INPUT_SCAN);	
		B_INPUTSCAN_LOOP++;
	}
	else
	{
		b_IsInputScanProcessingFlag = FALSE; 		
		gm_TimerStop(INPUT_SCAN_TMR);	
	}
}

void StartInputScan(void)
{
       msg_i("StartInputScan",0);
	OriginalInput = UserPrefCurrentInputMain;
       UserPrefHideIconFlag = TRUE; 	
	B_INPUTSCAN_LOOP = 0;
	b_IsInputScanProcessingFlag = TRUE; 	
	if(B_SplashComplete == TRUE) //KevinL 20090410
		gm_TimerStart(INPUT_SCAN_TMR, PERIOD_OF_START_INPUT_SCAN_DC);
	else
		gm_TimerStart(INPUT_SCAN_TMR, PERIOD_OF_START_INPUT_SCAN_AC);		
}
#endif


void AdjustForceBackGroundColor(void)//081028 Edward copy from 2709
{	 
	WORD W_Reg_Color;
	WORD W_Reg_Force_BKGND;
	WORD W_Reg_PB_HSTART;
	WORD W_Reg_PB_VSTART;
	WORD W_Reg_PB_HWIDTH;
	WORD W_Reg_PB_VHEIGHT;
	BYTE B_PB_ENABLE;
	static WORD W_HStart = 0;
	static WORD W_VStart = 0; 
	static WORD W_HWidth = 0;
	static WORD W_VHeight = 0;
	BYTE B_Index; 
	//LLD_SetAutoForcedBackground(0,0,0,0,0);

	B_Index = InputConvertScheme(UserPrefCurrentInputMain);		

	if ((UserPrefRunBurnInModeFlag == 1) && IsMainSourceValid(UserPrefCurrentInputMain))
	{
		return;
	}

	if (gmvb_MainChannel == CH_A)
	{
		W_Reg_Color = PB_BKGND;//MAIN_AUTO_BKGND_COLOR;
		W_Reg_Force_BKGND = PB_FORCE_MAIN_BKGND;//|PB_MAIN_ENABLE;
		W_Reg_PB_HSTART = PB_MAIN_HSTART;
		W_Reg_PB_VSTART = PB_MAIN_VSTART;
		W_Reg_PB_HWIDTH = PB_MAIN_HWIDTH;
		W_Reg_PB_VHEIGHT = PB_MAIN_VHEIGHT;
		B_PB_ENABLE = PB_MAIN_ENABLE; // PIP to be cleared ...
	}
	else
	{
		W_Reg_Color = PIP_AUTO_BKGND_COLOR;
		W_Reg_Force_BKGND = PB_FORCE_PIP_BKGND|PB_PIP_ENABLE;
		W_Reg_PB_HSTART = PB_PIP_HSTART;
		W_Reg_PB_VSTART = PB_PIP_VSTART;
		W_Reg_PB_HWIDTH = PB_PIP_HWIDTH;
		W_Reg_PB_VHEIGHT = PB_PIP_VHEIGHT;  
		B_PB_ENABLE = PB_PIP_ENABLE;  // PIP to be cleared ...
	}

#ifdef DIAGNOSTIC_BLACK_DISABLE //KevinL 20090615 
	if(UserPrefDiagnosticTestState == 1) 
	{
	     if(UserPrefForceBGColor == FORCE_BACKGROUND_BLACK)
		   UserPrefForceBGColor = FORCE_BACKGROUND_WHITE;
	}
#endif	
		   
	if (UserPrefForceBGColor== FORCE_BACKGROUND_GRAY) // 40% Gray ...
	{
		gm_WriteRegWord(W_Reg_Color, 0x632C);
	}
	else if (UserPrefForceBGColor == FORCE_BACKGROUND_RED)
	{
		gm_WriteRegWord(W_Reg_Color, 0xF800);
	}
	else if (UserPrefForceBGColor == FORCE_BACKGROUND_GREEN)
	{
		gm_WriteRegWord(W_Reg_Color, 0x07E0);
	}
	else if (UserPrefForceBGColor == FORCE_BACKGROUND_BLUE)
	{
		gm_WriteRegWord(W_Reg_Color, 0x001F);
	}
	//080812 M1B115T Edward delete the black image for Qisda request
	else if (UserPrefForceBGColor == FORCE_BACKGROUND_BLACK)//calo enable for the IR pattern 090313
	{
		gm_WriteRegWord(W_Reg_Color, 0x0000);
	}
	else if (UserPrefForceBGColor == FORCE_BACKGROUND_WHITE)
	{
		gm_WriteRegWord(W_Reg_Color, 0xFFFF);
	}
	else if (UserPrefForceBGColor == FORCE_BACKGROUND_YELLOW)	//for Dell BID Test Pattern - FY13 Text Pattern
	{
		if(UserPrefDiagnosticTestState == 1)
			gm_WriteRegWord(W_Reg_Color, 0x0000);
		else
			gm_WriteRegWord(W_Reg_Color, 0xFFE0);
	}
	else if(UserPrefForceBGColor == FORCE_BACKGROUND_MAGENTA)
	{
	
	    if(UserPrefDiagnosticTestState == 1)
	    	{
	    	;//chunbao avoid exit Diagnostic test patten show magenta patten
	    	}
		else
			{
		gm_WriteRegWord(W_Reg_Color, 0xF81F);
	}
	}
	else if(UserPrefForceBGColor == FORCE_BACKGROUND_CYAN)
	{
		gm_WriteRegWord(W_Reg_Color, 0x07FF);
	}

	//if (gm_ReadRegWord(PB_CTRL) && PB_FORCE_MAIN_BKGND)
	if (UserPrefForceBGColor < 250)		//Neil120620* org: != 255
	{
		if (UserPrefPipMode != NO_PIP)
		{
			gm_ClearRegBitsWord(PB_CTRL, B_PB_ENABLE | PB_HL_BORDER_EN);
		}

		//gm_SetRegBitsWord(PB_CTRL, W_Reg_Force_BKGND);
		gm_ClearAndSetBitsWord(PB_CTRL, PB_MAIN_ENABLE, W_Reg_Force_BKGND);

		if (gm_ReadRegWord(W_Reg_PB_HSTART) != 0)
		{
			W_HStart = gm_ReadRegWord(W_Reg_PB_HSTART);
			gm_WriteRegWord(W_Reg_PB_HSTART, 0);
		}

		if (gm_ReadRegWord(W_Reg_PB_VSTART) != 0)
		{
			W_VStart = gm_ReadRegWord(W_Reg_PB_VSTART);
			gm_WriteRegWord(W_Reg_PB_VSTART, 0);
		}

		if (gm_ReadRegWord(W_Reg_PB_HWIDTH) != PanelWidth)	// Raymond[(2011-08-10):01], fix for corrupt issue
		{
			W_HWidth = gm_ReadRegWord(W_Reg_PB_HWIDTH);
			gm_WriteRegWord(W_Reg_PB_HWIDTH, PanelWidth);	// Raymond[(2011-08-10):01], fix for corrupt issue
		}

		if (gm_ReadRegWord(W_Reg_PB_VHEIGHT) != PanelHeight)	// Raymond[(2011-08-10):01], fix for corrupt issue
		{
			W_VHeight = gm_ReadRegWord(W_Reg_PB_VHEIGHT);
			gm_WriteRegWord(W_Reg_PB_VHEIGHT, PanelHeight);	// Raymond[(2011-08-10):01], fix for corrupt issue
		}

		//gm_SyncUpdateWait(gmvb_MainChannel, OUTPUT_CLOCK);
	}
	else
	{
		gm_WriteRegWord(W_Reg_Color, RGB_565(MAIN_BACKGROUND_COLOR)); // Set to Black

		if (UserPrefPipMode != NO_PIP)
			gm_SetRegBitsWord(PB_CTRL, B_PB_ENABLE|PB_HL_BORDER_EN);


		if (gm_IsValidSignal(gmvb_MainChannel))
		{		
                     if ((UserPrefPipMode == NO_PIP) && ((UserPrefPortAspectRatio == SchemeFactoryDefaultInROM[B_Index].AspectRatio))) //KevinL20090408 //calo update for when set Aspect wide mode ,run IR and exit it has wrong display
			{
				W_HStart = 0;
				W_VStart = 0;
				W_HWidth = PanelWidth;	// Raymond[(2011-08-10):01], fix for corrupt issue
				W_VHeight = PanelHeight;	// Raymond[(2011-08-10):01], fix for corrupt issue
			}				
			if (W_HStart)
			{
				gm_WriteRegWord(W_Reg_PB_HSTART, W_HStart);
				W_HStart=0;		//20120918 yvonne clear the value
			}
			if (W_VStart)
			{
				gm_WriteRegWord(W_Reg_PB_VSTART, W_VStart);
				W_VStart =0;	//20120918 yvonne clear the value
			}
			if (W_HWidth)
			{
				gm_WriteRegWord(W_Reg_PB_HWIDTH, W_HWidth);
				W_HWidth=0;		//20120918 yvonne clear the value
			}
			if (W_VHeight)
			{
				gm_WriteRegWord(W_Reg_PB_VHEIGHT, W_VHeight);
				W_VHeight=0;	//20120918 yvonne clear the value
			}

			if(UserPrefForceBGColor != 254)	//Neil120620+ powerkey pressed, don't clear BKGND if power down request;
			{
			gm_ClearRegBitsWord(PB_CTRL, W_Reg_Force_BKGND);

			if (B_PB_ENABLE == PB_MAIN_ENABLE)
				gm_SetRegBitsWord(PB_CTRL, PB_MAIN_ENABLE);
			else
				gm_SetRegBitsWord(PB_CTRL, PB_PIP_ENABLE);
		}
		}
		//gm_SyncUpdateWait(gmvb_MainChannel, OUTPUT_CLOCK);			
	}

	gm_SyncUpdateWait(gmvb_MainChannel, OUTPUT_CLOCK);	
}

#endif // DELL_U2410F

#ifdef DELL_U2410F
/*BOOL IsVGACableConnected(void)
{
	return TRUE;
}*/

void TurnLEDOff(void)
{
    #ifdef TOUCH_KEY//Lvjie 081013
    BYTE Buff_Osd = 0xFF;	// Raymond[(2011-07-29):01], LED turn off by OSD directly
    if(gm_MessageSend(gmd_MESSAGE_USER, &Buff_Osd, sizeof(Buff_Osd)))
    {    	
		//hgh modify 20120906
        #ifdef DP_NOSYNC_LED_FADING //120808 Kordonm add
		{
			BYTE command = 0;   // 0x5A: 0x600 Normal mode, fake sleep led fading			   0: Exit fake sleep led fading
			if (gm_MessageSend(gmd_MESSAGE_CableStatus, &command, sizeof(command)) == FALSE)
				msg("##### Send quit fading Status Fail !!");
		}
        #endif
		//hgh modify 20120906
		
        msg_i("WB TurnLEDOff requested", 0);
    }
    else
    {
        msg_i("==> TurnLEDOff requested failed", 0);
    }
    #endif	
}

/*BOOL IsDVI1CableConnected(void)
{
	return TRUE;	
}

BOOL IsDVI2CableConnected(void)
{
	return TRUE;	
}*/
#endif

//====================================================
//	Code for Innolux Boar
//====================================================

#ifdef DELL_U2410F//#ifdef INNOLUX_MARS_BOARD
void SwitchDviInput(BYTE input)
{
#if defined(INNOLUX_MARS_BOARD_V2) 
    if(input==ALL_BOARDS_DVI1)
    {
	gm_SetRegBitsWord(GPOUTPUT1, DIGITAL_SW);       	
    }
    if(input==ALL_BOARDS_DVI2)
    {
	gm_ClearRegBitsWord(GPOUTPUT1, DIGITAL_SW); 	
    }
#else
    if(input==ALL_BOARDS_DVI2)
    {
	gm_SetRegBitsWord(GPOUTPUT1, DIGITAL_SW);       	
    }
    if(input==ALL_BOARDS_DVI1)
    {
	gm_ClearRegBitsWord(GPOUTPUT1, DIGITAL_SW); 	
    }
#endif	
}

void FactoryModeAdmitHandler(void)
{
	#ifdef	DELL_U2410F
    	if (gm_TimerCheck(FactoryModeAdmit_1s_TMR) != TIMER_TMO)
		return;
	gm_TimerStop(FactoryModeAdmit_1s_TMR);
	//bFactoryModeAdmitFlag = FALSE;
	#endif
}

void PipMessageAdmitHandler(void)
{
	#ifdef	DELL_U2410F
    	if (gm_TimerCheck(SystemPowerOn_TMR) != TIMER_TMO)
		return;
	if((B_SplashComplete == FALSE) && (UserPrefPipMode != NO_PIP))
	{
		gm_TimerStart(SystemPowerOn_TMR, 30); 	
		bPipMessageAdmitFlag = FALSE;		
		return;
	}
	else
		gm_TimerStop(SystemPowerOn_TMR);
	
	bPipMessageAdmitFlag = TRUE;
	#endif
}

#ifdef PIP_POS_OSD_ACC_PATCH //KevinL 20090513		
void AccUpdatedCheckHandler(void) 
{
	if(b_ACCDisableFlag == FALSE)
		return;
	
    	if (gm_TimerCheck(ACCUpdateCheck_1s_TMR) != TIMER_TMO)
		return;

       if(b_ACCDisableFlag == TRUE)
       {
		gm_TimerStop(ACCUpdateCheck_1s_TMR);
		b_ACCDisableFlag = FALSE;
		gm_SetRegBitsWord(MISC_OCMMASK, ACC_UPDATE_MASK);
       }
}
#endif

#ifdef CHECK_PxP_STATES_AC_ON
void PowerOnPIPModeHandler(void)
{
      	if (gm_TimerCheck(MainNoSyncCheck_100ms_TMR) == TIMER_TMO)    //KevinL 20090325 for power on to determine PIP/PBP should be turned on or not.	   
      	{
			//DW_Time2 = gm_GetSystemTime();	      	   
			gm_TimerStop(MainNoSyncCheck_100ms_TMR);
			B_MainNoSyncCheckState = STOP_CHECK;
          	if(B_StoredPipMode != NO_PIP) //Turn off PIP and start input scan
           {
      	       //gm_Print("FINISH_CHECK Time : %d, turn off pip", (DW_Time2-DW_Time1));		              
               B_StoredPipMode = NO_PIP;
		       UserPrefStoredPipMode = NO_PIP;	
	           SaveModeIndependentSettings();				
		    	//if (IsDVI() || IsMainVGAInput())
					//StartInputScan();
           }		
      	}
	   
    	if ((b_EnablePowerOnPIPModeFlag == FALSE) ||(B_SplashComplete == FALSE) ||(b_IsInputScanProcessingFlag == TRUE))
		return;
		
	if(gm_IsValidSignal(gmvb_MainChannel)) 
	{	   
	   if(B_StoredPipMode != NO_PIP)
	   {   
      	   gm_Print("AC ON  turn on pip", 0);		       
	       UserPrefPipMode = B_StoredPipMode;
		   AdjustPipMode();
	       SaveModeIndependentSettings();	
		   gm_BlenderOnOff(gmvb_PipChannel, On);			
 	       //owen 120830 disable    OSDRequestMsg (CH_A, MTO_IconShow, REQUEST_NOW);
		//owen 120830 disable    OSDRequestMsg (CH_B, MTO_PIPIconShow, REQUEST_NOW);		
	   }
	}
	else
	{
      	   gm_Print("InValid mode AC ON  turn off pip", 0);
           B_StoredPipMode = NO_PIP;
	       UserPrefStoredPipMode = NO_PIP;				   		   
	}
       b_EnablePowerOnPIPModeFlag = FALSE;	
}
#endif

void PIPMessageHandler(void) //KevinL 20090515 updated
{
	static BYTE PrePxpMes = 0xFF;

	if (gm_TimerCheck(PipMessage_1s_TMR) != TIMER_TMO)
		return;
	
	gm_TimerStart(PipMessage_1s_TMR, 2);
	
	if((UserPrefPipMode !=NO_PIP) && (UserPrefPIPMessageOnFlag == On) && ((PwrState == Pwr_DeepSleep)
	#if(FEATURE_SOURCE_SCAN == ENABLE)
		||(SourceScanStatus != StopScanSource)
	#endif
		))
	{
		OSDRequestMsg (CH_B, MTO_PIPMHide, REQUEST_NOW);
	}	

	if(IsPowerInDeepSleep())
		return;

	if(UserPrefMenuAdjust == On)
	{
		if(PrePxpMes != MTO_PIPMHide)
		{
			PrePxpMes = MTO_PIPMHide;
			OSDRequestMsg (CH_B, MTO_PIPMHide, REQUEST_NOW);
		}
		return;
	}
	PrePxpMes = MTO_PIPMShow;
	
	if((UserPrefPipMode !=NO_PIP) && (IsPipNoSync() || OSD_IsPipOutOfRange) && (UserPrefPIPMessageOnFlag== Off) && (UserPrefMenuAdjust== Off) && 
		(B_SplashComplete == TRUE) && (UserPrefGlobalLCDOnOffFlag == Off) && (PwrState != Pwr_DeepSleep) 
	#if(FEATURE_SOURCE_SCAN == ENABLE)
		&& (SourceScanStatus == StopScanSource)
	#endif
		) 
	{
		//gm_Print("PIP Message no displayed, send	MTO_PIPMShow",0);		
		if(IsDigitalInput(gmvb_PipChannel)) //KevinL 20090223
			OSDRequestMsg(gmvb_PipChannel, MTO_PIPMShow, PIP_MESSAGE_DELAY_DIGITAL); // Show Pip message
		else
			OSDRequestMsg(gmvb_PipChannel, MTO_PIPMShow, PIP_MESSAGE_DELAY); // Show Pip message
	}		

}


//LBADC_INT1 : VGA : 2.63V ; DVI2 : 2.27V     ADC value : 149(VGA+DVI2) ; 202(VGA) ; 174(DVI2) ; ?(no cable) 
//LBADC_INT2 : HDMI : 2.63V ; DVI1 : 2.27V    ADC value : 148(HDMI+DVI1) ; 201(HDMI) ; 174(DVI1) ; 252(no cable) 
//LBADC_INT3 : DP_DAP1 (Cable)                   ADC value : 23(DP) ; 249(no cable) 
//LBADC_INT4 : DP_DAP2 (PWR TX)                ADC value : 0(no PWR TX) ; 239(PWR TX) ; 0(no cable) 
#define ADC0_VGA_VALUE 		202
#define ADC0_VGA_DVI2_VALUE 	149
#define ADC0_DVI2_VALUE 		174
#define ADC1_HDMI_DVI1_VALUE	148
#define ADC1_DVI1_VALUE 		174
#define ADC1_HDMI_VALUE 		201
#define ADC2_DP_VALUE 			150 //100 //22
#define ADC3_DP_PWR_VALUE 	239

void CableDetectionHandler(void)	
{
	BYTE ADCValue;
	BYTE ADCChannel;
   #ifdef	DELL_U2410F
	if (gm_TimerCheck(CableDection_100ms_TMR) != TIMER_TMO)
		return;
	
	gm_TimerStart(CableDection_100ms_TMR, 1);
	
	#ifdef DDCCI_FORCE_SLEEP
	if (DDCCI_ForceSleep == gmd_TRUE)
	{
		SETBIT(CableStatus, VGA_DAP);
		SETBIT(CableStatus, DVI2_DAP);	
		SETBIT(CableStatus, HDMI_DAP);
		SETBIT(CableStatus, DVI1_DAP);		
		SETBIT(CableStatus, DP_DAP1);
		return;
	}
	#endif
#endif

	//Check VGA/DVI2
	ADCChannel = 0;
	ADCValue = gm_ScanAdcKey((ADCChannel << BIT1) & (BIT4 | BIT2 | BIT3));  
	if (abs(ADCValue - ADC0_VGA_VALUE) < DAP_DELTA) //VGA
	{
		SETBIT(CableStatus, VGA_DAP);
		CLRBIT(CableStatus, DVI2_DAP);
	}
	else if (abs(ADCValue - ADC0_VGA_DVI2_VALUE) < DAP_DELTA) //VGA+DVI2
	{
		SETBIT(CableStatus, VGA_DAP);
		SETBIT(CableStatus, DVI2_DAP);	   
	}
	else if (abs(ADCValue - ADC0_DVI2_VALUE) < DAP_DELTA) //DVI2
	{
		CLRBIT(CableStatus, VGA_DAP);
		SETBIT(CableStatus, DVI2_DAP);	   
	}	
	else //NO VGA, NO DVI2
	{
		CLRBIT(CableStatus, VGA_DAP);
		CLRBIT(CableStatus, DVI2_DAP);
	}

	//Check HDMI/DVI1
	ADCChannel = 1;
	ADCValue = gm_ScanAdcKey ((ADCChannel << BIT1) & (BIT4 | BIT2 | BIT3));  
	if (abs(ADCValue - ADC1_HDMI_DVI1_VALUE) < DAP_DELTA) //HDMI+DVI1
	{
		SETBIT(CableStatus, HDMI_DAP);
		SETBIT(CableStatus, DVI1_DAP);

	#ifdef HdmiPacketCondition
		if (HdmiNoCableState==0)
		{
			HdmiNoCableState=1;
			RunCount=0;			
		}
	#endif
	}
	else if (abs(ADCValue - ADC1_DVI1_VALUE) < DAP_DELTA) //DVI1
	{
		CLRBIT(CableStatus, HDMI_DAP);
		SETBIT(CableStatus, DVI1_DAP);

	#ifdef HdmiPacketCondition
		HdmiNoCableState=0;	
	#endif
	}
	else if (abs(ADCValue - ADC1_HDMI_VALUE) < DAP_DELTA) //HDMI
	{
		SETBIT(CableStatus, HDMI_DAP);
		CLRBIT(CableStatus, DVI1_DAP);

	#ifdef HdmiPacketCondition
		if (HdmiNoCableState==0)
		{
			HdmiNoCableState=1;
			RunCount=0;			
		}
	#endif
	}
	else // No HDMI, No DVI1
	{
		CLRBIT(CableStatus, HDMI_DAP);
		CLRBIT(CableStatus, DVI1_DAP);

	#ifdef HdmiPacketCondition
		HdmiNoCableState=0;
	#endif
	}

	//Check DP_DAP1 (Cable detection)
	ADCChannel = 2;
	ADCValue = gm_ScanAdcKey ((ADCChannel << BIT1) & (BIT4 | BIT2 | BIT3));
	//if (abs(ADCValue-ADC2_DP_VALUE)<DAP_DELTA) //DP
	if(ADCValue<ADC2_DP_VALUE) //KevinL 20090108 for capability for some DP TX device (ADC range 20 ~ 43) 
	{
		SETBIT(CableStatus, DP_DAP1);
	}
	else // No DP cable
	{
		CLRBIT(CableStatus, DP_DAP1);
		// Louis 050509
		//gm_DpRxDpcdWriteByte(0x600, 0x01);
	}

	//Check DP_DAP2 (Powered TX detection)
	ADCChannel = 3;
	ADCValue = gm_ScanAdcKey ((ADCChannel << BIT1) & (BIT4 | BIT2 | BIT3));
	if (abs(ADCValue - ADC3_DP_PWR_VALUE) < DAP_DELTA) //DP
	{
		SETBIT(CableStatus, DP_DAP2);
	}
	else // No Powered TX cable
	{
		CLRBIT(CableStatus, DP_DAP2);
	}   
}

void CableDetectionInit(void) 
{
	BYTE ADCValue;
	BYTE ADCChannel;
       CableStatus = 0;	
	//Check VGA/DVI2
	ADCChannel = 0;
	ADCValue = gm_ScanAdcKey((ADCChannel << BIT1) & (BIT4|BIT2|BIT3)); 
	if (abs(ADCValue - ADC0_VGA_VALUE) < DAP_DELTA) //VGA
	{
		SETBIT(CableStatus, VGA_DAP);
		CLRBIT(CableStatus, DVI2_DAP);
	}
	else if (abs(ADCValue - ADC0_VGA_DVI2_VALUE) < DAP_DELTA) //VGA+DVI2
	{
		SETBIT(CableStatus, VGA_DAP);
		SETBIT(CableStatus, DVI2_DAP);	   
	}
	else if (abs(ADCValue - ADC0_DVI2_VALUE) < DAP_DELTA) //DVI2
	{
		CLRBIT(CableStatus, VGA_DAP);
		SETBIT(CableStatus, DVI2_DAP);	   			
	}	
	else //NO VGA, NO DVI2
	{
		CLRBIT(CableStatus, VGA_DAP);
		CLRBIT(CableStatus, DVI2_DAP);		
	}

	//Check HDMI/DVI1
	ADCChannel = 1;
	ADCValue = gm_ScanAdcKey ((ADCChannel << BIT1) & (BIT4 | BIT2 | BIT3));  
	if (abs(ADCValue - ADC1_HDMI_DVI1_VALUE) < DAP_DELTA) //HDMI+DVI1
	{
		SETBIT(CableStatus, HDMI_DAP);
		SETBIT(CableStatus, DVI1_DAP);
	}
	else if (abs(ADCValue - ADC1_DVI1_VALUE) < DAP_DELTA) //DVI1
	{
		CLRBIT(CableStatus, HDMI_DAP);
		SETBIT(CableStatus, DVI1_DAP);
	}
	else if (abs(ADCValue - ADC1_HDMI_VALUE) < DAP_DELTA) //HDMI
	{
		SETBIT(CableStatus, HDMI_DAP);
		CLRBIT(CableStatus, DVI1_DAP);
	}
	else // No HDMI, No DVI1
	{
		CLRBIT(CableStatus, HDMI_DAP);
		CLRBIT(CableStatus, DVI1_DAP);
	}

	//Check DP_DAP1 (Cable detection)
	ADCChannel = 2;
	ADCValue = gm_ScanAdcKey ((ADCChannel << BIT1) & (BIT4 | BIT2 | BIT3));
	//if (abs(ADCValue-ADC2_DP_VALUE)<DAP_DELTA) //DP
	if(ADCValue<ADC2_DP_VALUE) //KevinL 20090108 for capability for some DP TX device (ADC range 20 ~ 43) 
	{
		SETBIT(CableStatus, DP_DAP1);
	}
	else // No DP cable
	{
		CLRBIT(CableStatus, DP_DAP1);
	}

	//Check DP_DAP2 (Powered TX detection)
	ADCChannel = 3;
	ADCValue = gm_ScanAdcKey ((ADCChannel << BIT1) & (BIT4 | BIT2 | BIT3));
	if (abs(ADCValue - ADC3_DP_PWR_VALUE) < DAP_DELTA) //DP
	{
		SETBIT(CableStatus, DP_DAP2);
	}
	else // No Powered TX cable
	{
		CLRBIT(CableStatus, DP_DAP2);
	}   
}

BOOL IsVGACableConnected(void)
{
	return ((CableStatus & VGA_DAP) != 0);
}

BOOL IsHDMICableConnected(void)
{
	return ((CableStatus & HDMI_DAP) != 0);	
}

BOOL IsDVI1CableConnected(void)
{
	return ((CableStatus & DVI1_DAP) != 0);	
}

BOOL IsDVI2CableConnected(void)
{
	return ((CableStatus & DVI2_DAP) != 0);	
}

BOOL IsDPCableConnected(void)
{
	return ((CableStatus & DP_DAP1) != 0);	
}

BOOL IsDP_PWRTX_Connected(void)
{
	return ((CableStatus & DP_DAP2) != 0);	
}

#ifdef PIVOT_SENSOR_ENABLE
#define RotationCnt  3
#define RotationDelay	200
void RotationSensorHandler(void)	
{
	if (gm_TimerCheck(RotationSensor_100ms_TMR) != TIMER_TMO)
		return;
	gm_TimerStart(RotationSensor_100ms_TMR, 3);

	if(UserPrefOsdRotation_Auto == ON)
	{
		static BYTE B_OSDRotateCnt = 0;

		//Neil120717+
		if (Is_SENSOR_ROTATE90() && (IsSplashScreenCompleted()))			
		{
			B_OSDRotateCnt++;
			if((B_OSDRotateCnt >= RotationCnt))//20120803 Add by kevinChen for reset counter avoid HideOSD.
				B_OSDRotateCnt = RotationCnt;
		}			
		else if(Is_SENSOR_ROTATE90() && !IsSplashScreenCompleted())//chunbao for rotation has no splashscreen
			B_OSDRotateCnt = 3;
		else
			B_OSDRotateCnt = 0;
		
		if((B_OSDRotateCnt >= RotationCnt) && (Is_SENSOR_ROTATE90()))	// 1.5 s
		{
	       if (UserPrefOsdRotation == OSD_LANDSCAPE)
			{
			
				HideOsd();
				OSDRequestMsg(gmvb_MainChannel, MTO_RemoveOSD, REQUEST_NOW);
				if(UserPrefPipMode != NO_PIP)
				{
					UserPrefStoredPipMode = UserPrefPipMode = NO_PIP;
					OSDRequestMsg(gmvb_PipChannel, MTO_PIPMHide, REQUEST_NOW);		//Neil120719+
					AdjustPipMode();
				}	
				
		    	UserPrefOsdRotation = OSD_ROTATED_90;
		    	AdjustOsdRotation();
				SaveModeIndependentSettings();
				//Neil120719+ for re-send message to OSD show main OSD again
				if(!gm_IsValidSignal(gmvb_MainChannel))
				{
					if(!isCableConnected(UserPrefCurrentInputMain))				
						OSDRequestMsg(gmvb_MainChannel, MTO_NoCable, RotationDelay);
					else
						OSDRequestMsg(gmvb_MainChannel, MTO_NoSync, RotationDelay);
				}
				if(gm_IsModeOOR(gmvb_MainChannel))
					OSDRequestMsg(gmvb_MainChannel, MTO_OutOfRange, RotationDelay);
	    	}
			//bRotateSensorFlag = OSD_PORTRAIT;   
		}
		else if((B_OSDRotateCnt < RotationCnt) && !Is_SENSOR_ROTATE90())
		{
	       if (UserPrefOsdRotation == OSD_PORTRAIT)	
	       {
	       		HideOsd();
				OSDRequestMsg(gmvb_MainChannel, MTO_RemoveOSD, REQUEST_NOW);
		     	UserPrefOsdRotation = OSD_NON_ROTATED;		
		     	AdjustOsdRotation();		
				SaveModeIndependentSettings();
				//Neil120719+ for re-send message to OSD show OSD again
				if(!gm_IsValidSignal(gmvb_MainChannel))
				{
					if(!isCableConnected(UserPrefCurrentInputMain))				
						OSDRequestMsg(gmvb_MainChannel, MTO_NoCable, RotationDelay);
					else
						OSDRequestMsg(gmvb_MainChannel, MTO_NoSync, RotationDelay);
				}
				if(gm_IsModeOOR(gmvb_MainChannel))
					OSDRequestMsg(gmvb_MainChannel, MTO_OutOfRange, RotationDelay);				
	       }
			//bRotateSensorFlag = OSD_LANDSCAPE; 
		}
	}	
}
#endif

#endif

#ifdef DELL_U2410F
void AdjustINLFactoryInfo(void)
{
    if(UserPrefInfoFlag == 0)
		UserPrefInfoFlag= 1;
    else
		UserPrefInfoFlag= 0;		
}

BYTE B_BackupPresetMode;

void BackupPresetMode(void)
{
	//B_BackupPresetMode = (IsColorMode_VODEO)? (UserPrefVideoImageMode): (UserPrefGraphicsImageMode);
	B_BackupPresetMode = UserPrefGraphicsImageMode;		//Neil 20120522* No color mode now, only graphic mode
//       gm_Print("BackupPresetMode  B_BackupPresetMode : %d",B_BackupPresetMode);
	//Neil120724+
	if(IsPresetMode_GRAPHICS_COLOR_SPACE)
		B_PresetColorSpace = UserPrefPresetColorSpace;
}

//120322 Edward disable unused
/*void SetColorFormat(void) 
{
	//
	// RGB
	//
	if (UserPrefMiscCtlSL == InputColorFormat_RGB)
	{
		gm_Print("SetColorFormat : RGB",0);
		//gm_WriteRegWord(RGB2YUV_CTRL, 0x00);
		gm_ClearRegBitsWord(OVP_CTRL , RGB2YUV_WIN_CTRL);
		
		//gm_ClearRegBitsWord(MAIN_MULT_CTRL, MAIN_MULT_IN_FORMAT);
	}
	//
	// YUV to RGB
	//
	else
	{
		gm_Print("SetColorFormat : YUV",0);
		//gm_WriteRegWord(RGB2YUV_CTRL, RGB2YUV_WIN_CTRL);
		gm_SetRegBitsWord(OVP_CTRL, RGB2YUV_WIN_CTRL);
		
		//gm_SetRegBitsWord(MAIN_MULT_CTRL, MAIN_MULT_IN_FORMAT);
	}
	CscUpdateColors(gmvb_MainChannel);
	CscUpdateColors(gmvb_PipChannel);
}*/

void SaveFactorySettingsInNVR(void)
{
	BYTE B_Checksum;

	NVRam_WriteBuffer(FactorySettingStart, (BYTE *)&FactoryUserPreferences, FactoryUserPrefSize);

	//
	// Calculate mode independent checksum.
	//
	B_Checksum = CalculateChecksum( &FactoryUserPreferences, FactoryUserPrefSize);

	//
	// Update mode independent checksum.
	//
	NVRam_WriteByte(FactorySettingChecksumStart, B_Checksum);
}

void ResetBurninTime(void) 
{
	UserPrefTimeUsedMin = 0;
	UserPrefTimeUsedHour = 0;
	gm_Print("........service block reset........reset timer.............",0);
}

void ResetBID(void) 
{
	//UserPrefBID = 0;
	msg_i("........service block reset........reset BID.............",0);
}
BOOL IsPIPSetupCompleted(void)
{
    return (BlenderIsPending(gmvb_PipChannel) == FALSE);
}

#ifdef VGA_GAME_MODE_PATCH //KevinL 20090427
BOOL IsVGADisplayFrameLock(void)
{
	if (((UserPrefCurrentInputMain == ALL_BOARDS_VGA1) && (gmvw_InputVActiveOrg[gmvb_MainChannel] == PanelHeight) &&
		(abs(stInput[gmvb_MainChannel].Vfrq - 600) <= 10)) && (UserPrefPipMode == NO_PIP))
		return TRUE;
	else
		return FALSE;
}
#endif	
		

BOOL IsGameModeEnable(void) 
{
	if(abs(stInput[gmvb_MainChannel].Vfrq - 600) > 10)
		return FALSE;
	
	 if(gm_ReadRegWord(IMP_CONTROL)&IMP_YUV_EN)
		 return FALSE;
	
	 if(UserPrefPipMode != NO_PIP)
		 return FALSE;

	//20120922 Add check OutputVActive by KevinChen for disable game mode when V-Scaling.
	if (IsGraphicInput(UserPrefCurrentInputMain) && gm_IsGraphics(gmvb_MainChannel) && (IsPresetMode_GRAPHICS_GAME ||IsPresetMode_VIDEO_GAME)&& (UserPrefPipMode == NO_PIP)&& gmvw_OutputVActive[gmvb_MainChannel] == PanelHeight) //KevinL 20090414	
	{
		//Neil 20090304+ 848X480 don't enable game mode
		if ((abs(stInput[gmvb_MainChannel].Vfrq - 600) <= 10) && (abs(stInput[gmvb_MainChannel].Hfrq - 298) < 10) && 
			(abs(stInput[gmvb_MainChannel].Vtotal - 500) < 4))
			return FALSE;

		//Haiwen  20090401+ VGA_640X480 <=59.8HZ  don't enable game mode
		if ((UserPrefCurrentInputMain == ALL_BOARDS_VGA1) && (abs(stInput[gmvb_MainChannel].Vfrq - 600) <= 10) && 
			(abs(stInput[gmvb_MainChannel].Hfrq - 315) < 10) && (abs(stInput[gmvb_MainChannel].Vtotal - 525) < 4) )
		{//dell FX160 VGA 640X480 59.6   low bandwidth , unlock 
			if (stInput[gmvb_MainChannel].Vfrq <= 598)
				return FALSE;
		}	

		if (abs(stInput[gmvb_MainChannel].Vfrq - 600) <= 15)
			return TRUE;
		else
			return FALSE;	
	}
	else
		return FALSE;
}

BOOL DPOverScan(gmt_PHY_CH B_Channel)	//Neil 20090312* for PIP need do overscan 
{
	gmt_MODE_TYPE B_CurrentModeType;
	B_CurrentModeType = gm_GetCurrentInputModeType_New(B_Channel);		//Neil 20090312* change to APP function

	if (IsDPInput(B_Channel))		//Neil 20090312* for PIP need do overscan 
	{
		/*	DP 1080p YUV do overscan, too
		if (B_CurrentModeType == gmd_1080P)
			return FALSE;	
		else
		*/
		{
			if (IsDPColorSpaceRGB())
			{
				if ((IsDPVideoMode(B_Channel)) &&(gm_IsInterlaced(B_Channel)))
					return TRUE;
			}
			else	
			{
				if (IsDPVideoMode(B_Channel)||(gm_IsInterlaced(B_Channel))||(B_CurrentModeType == gmd_480P)||(B_CurrentModeType == gmd_1080P))
					return TRUE;
			}
		}
		return FALSE;	   	    	
	}
	else
		return TRUE;
}

//Neil 20090312* for PIP need do overscan, too
BOOL HDMIOverScan(gmt_PHY_CH B_Channel)
{
	gmt_MODE_TYPE B_CurrentModeType;
	BYTE B_CurrentPort = (B_Channel == CH_A)?UserPrefCurrentInputMain:UserPrefCurrentInputPip;
	B_CurrentModeType = gm_GetCurrentInputModeType_New(B_Channel);		//Neil 20090312* change to APP function

	if (B_CurrentPort == ALL_BOARDS_HDMI)
	{
		/*	// HDMI 1080p do over scan, too
		if (B_CurrentModeType == gmd_1080P)
			return FALSE;
		else
		*/
		{
			if (IsHDMIColorSpaceRGB(B_Channel))
			{
				//SPEC: RGB only interlace mode do overscan
				if ((B_CurrentModeType > gmd_GRAPHICS_MODE) && (gm_IsInterlaced(B_Channel)) && HdmiGetITContentDoOverScan(B_CurrentPort)) //HDMI RGB progressive no over scan
					return TRUE;
				else
					return FALSE;	
			}
			else //YCbCr
			{				
				if((B_CurrentModeType > gmd_GRAPHICS_MODE) && HdmiGetITContentDoOverScan(B_CurrentPort)) //HDMI YUV color space over scan
					return TRUE;
				else
					return FALSE;					
			}
		}
	}
	else
		return TRUE;	
}

BOOL DVIOverScan(gmt_PHY_CH B_Channel)//calo update for overscan of PIP 090309	
{
	#if 1
	BYTE B_Input = (B_Channel == CH_A)?UserPrefCurrentInputMain: UserPrefCurrentInputPip;	//Neil120811* only check current channel input
		if((B_Input == ALL_BOARDS_DVI1)||(B_Input == ALL_BOARDS_DVI2))
	 	{
	 	    if(gm_IsInterlaced(B_Channel))
			 return TRUE;
		    else
			return FALSE;	
		}
		else
		     return TRUE;	
	#else
	if((UserPrefCurrentInputMain == ALL_BOARDS_DVI1)||(UserPrefCurrentInputMain == ALL_BOARDS_DVI2)
		|| (UserPrefCurrentInputPip == ALL_BOARDS_DVI1) || (UserPrefCurrentInputPip == ALL_BOARDS_DVI2))
 	{
 	    if(gm_IsInterlaced(B_Channel))
		 return TRUE;
	    else
		return FALSE;	
	}
	else
	     return TRUE;	
	#endif
}


//Neil120626+ for VGA over scan rules.
BOOL VGAOverScan(gmt_PHY_CH B_Channel)	
{
	gmt_MODE_TYPE B_CurrentModeType;
	B_CurrentModeType = gm_GetCurrentInputModeType_New(B_Channel);		//Neil 20090312* change to APP function

	if (GetChannelConnType(B_Channel) == VGA)		//Neil 20090312* for PIP need do overscan 
	{
		if (B_CurrentModeType == gmd_1080P)
			return FALSE;	
		else
		{
			if(gm_IsInterlaced(B_Channel))
				return TRUE;
			
			if (UserPrefColorFormat == Input_RGB)
				return FALSE;
			else if(B_CurrentModeType > gmd_GRAPHICS_MODE)	
				return TRUE;
		}
		return FALSE;	   	    	
	}
	else
		return TRUE;
}

/*BOOL IsSubCarrier443(void) //KevinL 20090413 for NTSC443
{
	return (gm_ReadRegByte(HVTIMER_STATUS0) & SUBCARR443);
}

BOOL IsCVBS_SECAM(void) //KevinL 20090418 for CVBS SECAM 
{
    if((gm_ReadRegByte(HVTIMER_STATUS1) & STD_DETECTED) == SECAM)
	return TRUE;
    else
	return FALSE;	
}
*/

void InputOverScanCrop(gmt_PHY_CH B_Channel)
{
	BYTE B_InputConnector;
	BYTE B_IsVideo;
	gmt_MODE_TYPE B_CurrentModeType;
	WORD W_HOverScan, W_VOverScan;

	B_CurrentModeType = gm_GetCurrentInputModeType_New(B_Channel);  
	if (B_Channel == gmvb_MainChannel)
		B_InputConnector = UserPrefCurrentInputMain;
	else
		B_InputConnector = UserPrefCurrentInputPip;

	//
	// NTSC/480i (input to Cortez)
	//
	if (B_CurrentModeType == gmd_NTSC_480I)
	{
		switch (B_InputConnector)
		{
			// TODO: Add more correct cropping values
			/*case ALL_BOARDS_CVBS1:		
				if(IsSubCarrier443()) //KevinL 20090413 for NTSC 443
				{
					stInput[B_Channel].Hstart -= 12;
					stInput[B_Channel].Hactive -= 12;
					stInput[B_Channel].Vstart -= 2;
					stInput[B_Channel].Vactive += 3;
					gm_Print("CVBS NTSC-443 Overscan",0);									
				}
				else
				{
					stInput[B_Channel].Hstart -= 2;
					stInput[B_Channel].Hactive -= 9;
					stInput[B_Channel].Vstart -= 2;
					stInput[B_Channel].Vactive += 3;
					gm_Print("CVBS NTSC-M Overscan",0);					
				}
				break;*/

			case ALL_BOARDS_COMP1:
			case ALL_BOARDS_COMP2:
				stInput[B_Channel].Hstart -= 2;
				stInput[B_Channel].Hactive -= 0;
				stInput[B_Channel].Vstart += 3;
				stInput[B_Channel].Vactive += 1;
				gm_Print("COMP 480I Overscan",0);
				break;

			case ALL_BOARDS_VGA1:
				stInput[B_Channel].Hstart -= 69;
				stInput[B_Channel].Hactive -= 0;
				stInput[B_Channel].Vstart += 0;
				stInput[B_Channel].Vactive += 3;
				gm_Print("VGA 480I Overscan",0);
				break;

			case ALL_BOARDS_HDMI:
			case ALL_BOARDS_DVI1:
			case ALL_BOARDS_DVI2:
				stInput[B_Channel].Hstart += 0;
				stInput[B_Channel].Hactive -= 0;
				stInput[B_Channel].Vstart += 0;
				stInput[B_Channel].Vactive -= 0;
				gm_Print("DVI 480I Overscan",0);
				break;

			case ALL_BOARDS_DP1:
//&*&*&*S 20120315:SWRD4001: Kordonm(0)  Modify for OSD Workbench
			case ALL_BOARDS_DP3:
//&*&*&*E 20120315 : Kordonm(1)  Modify for OSD Workbench
				stInput[B_Channel].Hstart -= 129;
				stInput[B_Channel].Hactive -= 0;
				stInput[B_Channel].Vstart -= 5;
				stInput[B_Channel].Vactive += 1;
				gm_Print("DP 480i Overscan",0);
				break;
		}
	}
	//
	// PAL/576i (input to Cortez)
	//
	else if (B_CurrentModeType == gmd_PAL_576I)
	{
		switch (B_InputConnector)
		{
			// TODO: Add more correct cropping values
			/*case ALL_BOARDS_CVBS1:
				if(IsCVBS_SECAM()) //KevinL 20090418
				{
					stInput[B_Channel].Hstart -= 1;
					stInput[B_Channel].Hactive -= 19;
					stInput[B_Channel].Vstart -= 1;
					stInput[B_Channel].Vactive -= 0;
					gm_Print("CVBS SECAM Overscan",0);
				}
				else
				{
					stInput[B_Channel].Hstart += 4;
					stInput[B_Channel].Hactive -= 19;
					stInput[B_Channel].Vstart -= 1;
					stInput[B_Channel].Vactive -= 0;
					gm_Print("CVBS PAL Overscan",0);
				}
				break;*/

			case ALL_BOARDS_COMP1:
			case ALL_BOARDS_COMP2:
				stInput[B_Channel].Hstart -= 8;
				stInput[B_Channel].Hactive -= 2;
				stInput[B_Channel].Vstart -= 2;
				stInput[B_Channel].Vactive -= 0;
				gm_Print("COMP 576i Overscan",0);
				break;

			case ALL_BOARDS_VGA1:
				stInput[B_Channel].Hstart -= 76;
				stInput[B_Channel].Hactive -= 2;
				stInput[B_Channel].Vstart -= 2;
				stInput[B_Channel].Vactive -= 0;
				break;

			case ALL_BOARDS_HDMI:
			case ALL_BOARDS_DVI1:
			case ALL_BOARDS_DVI2:
				stInput[B_Channel].Hstart += 0;
				stInput[B_Channel].Hactive -= 0;
				stInput[B_Channel].Vstart += 0;
				stInput[B_Channel].Vactive -= 0;
				break;

			case ALL_BOARDS_DP1:
//&*&*&*S 20120315:SWRD4001: Kordonm(0)  Modify for OSD Workbench
			case ALL_BOARDS_DP3:
//&*&*&*E 20120315 : Kordonm(1)  Modify for OSD Workbench				
				stInput[B_Channel].Hstart -= 130;
				stInput[B_Channel].Hactive -= 0;
				stInput[B_Channel].Vstart -= 5;
				stInput[B_Channel].Vactive -= 0;
				gm_Print("DP 576i Overscan",0);
				break;
		}
	}
	//
	// 480p (input to Cortez)
	//
	else if (B_CurrentModeType == gmd_480P)
	{
		switch (B_InputConnector)
		{
			case ALL_BOARDS_COMP1:
			case ALL_BOARDS_COMP2:
			case ALL_BOARDS_VGA1:
				stInput[B_Channel].Hstart -= 23;
				stInput[B_Channel].Hactive -= 0;
				stInput[B_Channel].Vstart -= 3;
				stInput[B_Channel].Vactive += 3;
				gm_Print("COMP/VGA 480P Overscan",0);	
				break;

			case ALL_BOARDS_HDMI:
			case ALL_BOARDS_DVI1:
			case ALL_BOARDS_DVI2:
				stInput[B_Channel].Hstart += 0;
				stInput[B_Channel].Hactive -= 0;
				stInput[B_Channel].Vstart += 0;
				stInput[B_Channel].Vactive -= 0;
				gm_Print("DVI 480P Overscan",0);
				break;

			case ALL_BOARDS_DP1:
//&*&*&*S 20120315:SWRD4001: Kordonm(0)  Modify for OSD Workbench
			case ALL_BOARDS_DP3:
//&*&*&*E 20120315 : Kordonm(1)  Modify for OSD Workbench				
				stInput[B_Channel].Hstart -= 4;
				stInput[B_Channel].Hactive -= 0;
				stInput[B_Channel].Vstart += 43;
				stInput[B_Channel].Vactive -= 0;
				gm_Print("DP 480P YUV Overscan",0);
				break;
		}
	}
	//
	// 576p (input to Cortez)
	//
	else if (B_CurrentModeType == gmd_576P)
	{
		switch (B_InputConnector)
		{
			case ALL_BOARDS_COMP1:
			case ALL_BOARDS_COMP2:
			case ALL_BOARDS_VGA1:
				stInput[B_Channel].Hstart -= 47;
				stInput[B_Channel].Hactive -= 1;
				stInput[B_Channel].Vstart -= 5;
				stInput[B_Channel].Vactive += 2;
				break;

			case ALL_BOARDS_HDMI:
			case ALL_BOARDS_DVI1:
			case ALL_BOARDS_DVI2:
				stInput[B_Channel].Hstart += 0;
				stInput[B_Channel].Hactive -= 0;
				stInput[B_Channel].Vstart += 0;
				stInput[B_Channel].Vactive -= 0;
				break;

			case ALL_BOARDS_DP1:
//&*&*&*S 20120315:SWRD4001: Kordonm(0)  Modify for OSD Workbench
			case ALL_BOARDS_DP3:
//&*&*&*E 20120315 : Kordonm(1)  Modify for OSD Workbench				
				stInput[B_Channel].Hstart -= 68;
				stInput[B_Channel].Hactive -= 0;
				stInput[B_Channel].Vstart -= 5;
				stInput[B_Channel].Vactive -= 0;
				gm_Print("DP 576P YUV Overscan",0);
				break;
		}
	}
	//
	// 720p (input to Cortez)
	//
	else if (B_CurrentModeType == gmd_720P)
	{
		switch (B_InputConnector)
		{
			case ALL_BOARDS_COMP1:
			case ALL_BOARDS_COMP2:
			case ALL_BOARDS_VGA1:
				if (stInput[B_Channel].ModeFlags & gmd_MODE_SOG)//YUV
				{
					if (stInput[B_Channel].Vfrq < 550)//720P@50Hz
					{
						stInput[B_Channel].Hstart += 6;
						stInput[B_Channel].Hactive += 2;
						stInput[B_Channel].Vstart += 1;
						stInput[B_Channel].Vactive += 0;
						gm_Print("COMP/VGA 720P@50Hz Overscan",0);
					}
					else //720P@60Hz
					{
						stInput[B_Channel].Hstart += 6;
						stInput[B_Channel].Hactive -= 0;
						stInput[B_Channel].Vstart += 1;
						stInput[B_Channel].Vactive += 3;
						gm_Print("COMP/VGA 720P@60Hz Overscan",0);
					}
				}
				break;

			case ALL_BOARDS_HDMI:
			case ALL_BOARDS_DVI1:
			case ALL_BOARDS_DVI2:
				stInput[B_Channel].Hstart += 0;
				stInput[B_Channel].Hactive -= 0;
				stInput[B_Channel].Vstart += 0;
				stInput[B_Channel].Vactive -= 0;
				gm_Print("DVI 720P Overscan",0);
				break;

			case ALL_BOARDS_DP1:
//&*&*&*S 20120315:SWRD4001: Kordonm(0)  Modify for OSD Workbench
			case ALL_BOARDS_DP3:
//&*&*&*E 20120315 : Kordonm(1)  Modify for OSD Workbench				
				stInput[B_Channel].Hstart -= 4;
				stInput[B_Channel].Hactive -= 0;
				stInput[B_Channel].Vstart += 0;
				stInput[B_Channel].Vactive -= 0;
				gm_Print("DP 720P YUV Overscan",0);
				break;
		}
	}
	//
	// 1080i (input to Cortez)
	//
	else if (B_CurrentModeType == gmd_1080I)
	{
		switch (B_InputConnector)
		{
			case ALL_BOARDS_COMP1:
			case ALL_BOARDS_COMP2:
			case ALL_BOARDS_VGA1:
				if (stInput[B_Channel].ModeFlags & gmd_MODE_SOG)//YUV
				{
					if (stInput[B_Channel].Vfrq < 550) //1080i@50Hz
					{
						stInput[B_Channel].Hstart += 0;
						stInput[B_Channel].Hactive += 2;
						stInput[B_Channel].Vstart += 3;
						stInput[B_Channel].Vactive += 2;
					}
					else//1080i@60Hz
					{
						stInput[B_Channel].Hstart += 8;
						stInput[B_Channel].Hactive -= 2;
						stInput[B_Channel].Vstart += 4;
						stInput[B_Channel].Vactive += 2;
					}
					gm_Print("COMP/VGA YUV  1080i Overscan",0);
				}
				else//RGB
				{
					if (stInput[B_Channel].Vfrq < 550) //1080i@50Hz
					{
						stInput[B_Channel].Hstart -= 4;
						stInput[B_Channel].Hactive -= 0;
						stInput[B_Channel].Vstart += 6;
						stInput[B_Channel].Vactive += 3;
						gm_Print("COMP/VGA RGB 1080i Overscan",0);
					}
					else
					{
						stInput[B_Channel].Hstart += 2;
						stInput[B_Channel].Hactive -= 0;
						stInput[B_Channel].Vstart += 6;
						stInput[B_Channel].Vactive += 3;
					}
				}
				break;

			case ALL_BOARDS_DP1:
//&*&*&*S 20120315:SWRD4001: Kordonm(0)  Modify for OSD Workbench
			case ALL_BOARDS_DP3:
//&*&*&*E 20120315 : Kordonm(1)  Modify for OSD Workbench				
				stInput[B_Channel].Hstart -= 4;
				stInput[B_Channel].Hactive -= 0;
				stInput[B_Channel].Vstart -= 1;
				stInput[B_Channel].Vactive -= 0;
				gm_Print("DP 1080i Overscan",0);
				break;
		}
	}
	//
	// 1152iLA (input to Cortez)
	//
	else if ((B_CurrentModeType == gmd_576P) && (OVERLAP_1152I == 1188))
	{
		switch (B_InputConnector)
		{
			case ALL_BOARDS_COMP1:
			case ALL_BOARDS_COMP2:
			case ALL_BOARDS_VGA1:
				if (stInput[B_Channel].ModeFlags & gmd_MODE_SOG)//YUV
				{
					stInput[B_Channel].Hstart += 0;
					stInput[B_Channel].Hactive -= 0;
					stInput[B_Channel].Vstart += 0;
					stInput[B_Channel].Vactive -= 0;
				}
				else//RGB
				{
					stInput[B_Channel].Hstart += 0;
					stInput[B_Channel].Hactive -= 0;
					stInput[B_Channel].Vstart += 0;
					stInput[B_Channel].Vactive -= 0;
				}
				break;

			case ALL_BOARDS_HDMI:
			case ALL_BOARDS_DVI1:
			case ALL_BOARDS_DVI2:
				stInput[B_Channel].Hstart += 0;
				stInput[B_Channel].Hactive -= 0;
				stInput[B_Channel].Vstart += 0;
				stInput[B_Channel].Vactive -= 0;
				break;
		}
	}
	//
	// 1152iSH (input to Cortez)
	//
	else if ((B_CurrentModeType == gmd_576P) && (OVERLAP_1152I == 1584))
	{
		switch (B_InputConnector)
		{
			case ALL_BOARDS_COMP1:
			case ALL_BOARDS_COMP2:
			case ALL_BOARDS_VGA1:
				if (stInput[B_Channel].ModeFlags & gmd_MODE_SOG)//YUV
				{
					stInput[B_Channel].Hstart += 0;
					stInput[B_Channel].Hactive -= 0;
					stInput[B_Channel].Vstart += 0;
					stInput[B_Channel].Vactive -= 0;
				}
				else//RGB
				{
					stInput[B_Channel].Hstart += 0;
					stInput[B_Channel].Hactive -= 0;
					stInput[B_Channel].Vstart += 0;
					stInput[B_Channel].Vactive -= 0;
				}
				break;

			case ALL_BOARDS_HDMI:
			case ALL_BOARDS_DVI1:
			case ALL_BOARDS_DVI2:
				stInput[B_Channel].Hstart += 0;
				stInput[B_Channel].Hactive -= 0;
				stInput[B_Channel].Vstart += 0;
				stInput[B_Channel].Vactive -= 0;
				break;
		}
	}
	//
	// 1152iLH (input to Cortez)
	//
	else if ((B_CurrentModeType == gmd_576P) && (OVERLAP_1152I == 2376))
	{
		switch (B_InputConnector)
		{
			case ALL_BOARDS_COMP1:
			case ALL_BOARDS_COMP2:
			case ALL_BOARDS_VGA1:
				if (stInput[B_Channel].ModeFlags & gmd_MODE_SOG)//YUV
				{
					stInput[B_Channel].Hstart += 0;
					stInput[B_Channel].Hactive -= 0;
					stInput[B_Channel].Vstart += 0;
					stInput[B_Channel].Vactive -= 0;
				}
				else//RGB
				{
					stInput[B_Channel].Hstart += 0;
					stInput[B_Channel].Hactive -= 0;
					stInput[B_Channel].Vstart += 0;
					stInput[B_Channel].Vactive -= 0;
				}
				break;

			case ALL_BOARDS_HDMI:
			case ALL_BOARDS_DVI1:
			case ALL_BOARDS_DVI2:
				stInput[B_Channel].Hstart += 0;
				stInput[B_Channel].Hactive -= 0;
				stInput[B_Channel].Vstart += 0;
				stInput[B_Channel].Vactive -= 0;
				break;
		}
	}
	else if (B_CurrentModeType == gmd_1080P)
	{
		switch (B_InputConnector)
		{
			case ALL_BOARDS_COMP1:
			case ALL_BOARDS_COMP2:		
				if (stInput[B_Channel].Vfrq < 245) //1080P@24Hz	
				{
					stInput[B_Channel].Hstart += 0;
					stInput[B_Channel].Hactive += 0;
					stInput[B_Channel].Vstart -= 8;
					stInput[B_Channel].Vactive += 4;	
					gm_Print("COMP 1080P 24Hz Overscan",0);
				}					
				else if(stInput[B_Channel].Vfrq < 255) //1080P@25Hz	
				{
					stInput[B_Channel].Hstart += 10;
					stInput[B_Channel].Hactive += 76;
					stInput[B_Channel].Vstart -= 6;
					stInput[B_Channel].Vactive += 4;	
					gm_Print("COMP 1080P 25Hz Overscan",0);
				}
				else if(stInput[B_Channel].Vfrq < 310) //1080P@30Hz	
				{
					stInput[B_Channel].Hstart -= 3;
					stInput[B_Channel].Hactive -= 0;
					stInput[B_Channel].Vstart += 17;
					stInput[B_Channel].Vactive += 3;	
					gm_Print("COMP 1080P 30Hz Overscan",0);
				}	
				else if(stInput[B_Channel].Vfrq < 510)//1080P@50Hz
				{
					stInput[B_Channel].Hstart += 0;
					stInput[B_Channel].Hactive -= 2;
					stInput[B_Channel].Vstart += 16;
					stInput[B_Channel].Vactive += 4;	
					gm_Print("COMP 1080P 50Hz Overscan",0);
				}
				else//1080P@60Hz
				{
					stInput[B_Channel].Hstart -= 2;
					stInput[B_Channel].Hactive -= 0;
					stInput[B_Channel].Vstart += 18;
					stInput[B_Channel].Vactive += 2;	
					gm_Print("COMP 1080P 60Hz Overscan",0);
				}
				break;
		}
	}
	else
	{
		gm_Print("No OverScan",0);
	}
}

BOOL OSDDrawVideoTimingInfo(gmt_PHY_CH B_Channel)
{
	BYTE B_Input;	
	gmt_MODE_TYPE B_CurrentModeType;

	//B_CurrentModeType = gm_GetCurrentInputModeType(B_Channel);	//Neil 20081222-
	B_CurrentModeType = gm_GetCurrentInputModeType_New(B_Channel);	//Neil 20081222+
	if (B_Channel == gmvb_MainChannel)
		B_Input = UserPrefCurrentInputMain;
	else
		B_Input = UserPrefCurrentInputPip;	

	if (IsVideoInput(B_Input) || gm_IsInterlaced(B_Channel))
		return TRUE;
	else if ((B_Input == ALL_BOARDS_DVI1) || (B_Input == ALL_BOARDS_DVI2)) 
	{
		if (IsDVI_HDCP_ON(B_Channel))
		{
			if ((B_CurrentModeType == gmd_576P) || (B_CurrentModeType == gmd_720P) || (B_CurrentModeType == gmd_1080P) ||
				(B_CurrentModeType == gmd_480P))//20120702 Disable condition Hactive>650 by KevinChen. DVI show 480P when HDCP on.
				//((B_CurrentModeType == gmd_480P) && (stInput[B_Channel].Hactive > 650)))
				return TRUE;
			else 
				return FALSE; 	
		}
		else 		
			return FALSE;  
	}
	else if (B_Input == ALL_BOARDS_HDMI) 
	{
		if ((B_CurrentModeType > gmd_GRAPHICS_MODE))// && (!IsHDMIColorSpaceRGB()))//KevinL 20090117 updated
		{	
			if(IsHDMIColorSpaceRGB(B_Channel))		//Neil120626+ for video timing showing
				return FALSE;
			else
			return TRUE;                            //calo remove the color space checking for video timing 090207
		}
		else
			return FALSE;		
	}
//&*&*&*S 20120315:SWRD4001: Kordonm(0)  Modify for OSD Workbench
	else if((B_Input == ALL_BOARDS_DP1)||(B_Input == ALL_BOARDS_DP3))
//&*&*&*E 20120315 : Kordonm(1)  Modify for OSD Workbench
	{
		if ((B_CurrentModeType > gmd_GRAPHICS_MODE))// && (!IsDPColorSpaceRGB()))//KevinL 20090117 updated		
		{	
			if(IsDPColorSpaceRGB())		//Neil120626+ for video timing showing
				return FALSE;
			else
			return TRUE;                            //calo remove the color space checking for video timing 090207
		}	
		else		
			return FALSE;	
	}
	else if (B_Input == ALL_BOARDS_VGA1)
	{
		if ((B_CurrentModeType > gmd_GRAPHICS_MODE) && ((stInput[B_Channel].ModeFlags&gmd_MODE_SOG) == gmd_MODE_SOG))
		{
			if(UserPrefColorFormat == Input_RGB)	//Neil120626+ for Draw timing ifor for video timing
				return FALSE;
			else
			return TRUE;
		}
		else
			return FALSE;	
	}
	else	
		return FALSE;	
}

BOOL IsDVI_HDCP_ON(gmt_PHY_CH B_Channel) //KevinL 20081118
{
    if(IsDVIInput(B_Channel))
    {
		//Neil120612* CMI use DVI0 HDCP register as DVI input HDCP
        if(gm_ReadRegByte(DVI0_HDCP_STATUS_EXT) & DVI0_HDCP_RE_AUTHENTICATION)
        {
              gm_Print("DVI HDCP ON",0);
        	return TRUE;
        }
	 else
	 {
              gm_Print("DVI HDCP OFF",0);	 
        	return FALSE;	 
	 }
    }
    else
	 return FALSE;
}

BOOL IsDVIInput(gmt_PHY_CH B_Channel) //KevinL 20090422
{
   if(B_Channel == gmvb_MainChannel)
   {
    	if((UserPrefCurrentInputMain == ALL_BOARDS_DVI1)||(UserPrefCurrentInputMain == ALL_BOARDS_DVI2))
		return TRUE;
	else
		return FALSE;
   }
   else
   {
    	if((UserPrefCurrentInputPip == ALL_BOARDS_DVI1)||(UserPrefCurrentInputPip == ALL_BOARDS_DVI2))
		return TRUE;
	else
		return FALSE;   
   }
}

BYTE ReadDPBandWidth(void) //KevinL 20081202
{
    BYTE B_Value;
    B_Value = gm_ReadRegWord(DP12RX0_LT_STATUS)&DPRX_FINAL_LINK_BW;//Neil120613* Org = DPRX_FINAL_LINK_BW
    if((B_Value == 0x06) || (B_Value == 0x0A) || (B_Value == 0x14))	
    	return B_Value;
    else
	return 0;	
}

BYTE ReadDPLaneCount(void) //KevinL 20081202
{
	return (gm_ReadRegWord(DP12RX0_LT_STATUS)&DPRX_FINAL_LANE_COUNT)>>8;//Neil120613* Org = DPRX_FINAL_LANE_COUNT
}

OSDMainMessageType MainMessageTypeReturn(void) 
{
	#if 0
	return OSD_NULL;	   		 	

	#else
        //CableInfo CableUpdate;
	#ifdef CHECK_PxP_STATES_AC_ON
       if((B_SplashComplete == TRUE) && (B_MainNoSyncCheckState == STOP_CHECK))	   
       {
              if(UserPrefStoredPipMode != UserPrefPipMode) 
              {
                     gm_Print("Restore PIP mode : %d",UserPrefStoredPipMode);
          		UserPrefPipMode = UserPrefStoredPipMode;
              }
       }
	#endif

       //if(UserPrefRunBurnInModeFlag != Off)//KevinL 20090506 for Burn In mode On
       //    return OSD_NULL;
	   
	if (UserPrefPipMode == NO_PIP) //Main
	{
		if (OSD_IsMainOutOfRange)
		{
			if ((stInput[gmvb_MainChannel].ModeFlags & gmd_MODE_COMPLETE) != 0) //KevinL 20090316	
				return OSD_MAIN_OUT_OF_RANGE;
		}

		if (UserPrefStateTempFlag == OUT_OF_RANGE_STATE)         
			return OSD_MAIN_OUT_OF_RANGE;		          

		if (!OSD_IsMainSync)	  
		{
			//if (!OSD_IsCableConnectedMain) // No Cable
			if(UserPrefStateTempFlag == NO_CABLE_STATE)			
				return OSD_MAIN_NO_CABLE;

			if (IsGraphicInput(UserPrefCurrentInputMain)) //Graphic input
			{
			#if 0//120922 Edward disable for enter power msg alway display when port change
			       Get_CableStatus(&CableUpdate);
			   if((UserPrefCurrentInputMain==ALL_BOARDS_DVI1)&&(CableUpdate.PHY2_CableStatus==1)&&(CableUpdate.PHY2_SourcePowerStatus==0))
			   	return OSD_MAIN_ENTER_POWER_SAVING_GRAPHIC;
			   if((UserPrefCurrentInputMain==ALL_BOARDS_HDMI)&&(CableUpdate.PHY3_CableStatus==1)&&(CableUpdate.PHY3_SourcePowerStatus==0))
			   	return OSD_MAIN_ENTER_POWER_SAVING_GRAPHIC; // chunbao add HDMI
			   if((UserPrefCurrentInputMain==ALL_BOARDS_DP1)&&(CableUpdate.PHY1_CableStatus==1)&&(CableUpdate.PHY1_SourcePowerStatus==0))
				 return OSD_MAIN_ENTER_POWER_SAVING_GRAPHIC;
		          if((UserPrefCurrentInputMain==ALL_BOARDS_DP3)&&(CableUpdate.MiniDP_CableStatus==1)&&(CableUpdate.MiniDP_SourcePowerStatus==0))
				 return OSD_MAIN_ENTER_POWER_SAVING_GRAPHIC;
			#endif	  
				if (UserPrefStateTempFlag == ENTER_POWER_SAVING) 
					return OSD_MAIN_ENTER_POWER_SAVING_GRAPHIC;
				else if (UserPrefStateTempFlag == NO_SIGNAL_STATE) 
					return OSD_MAIN_NO_SIGNAL_GAPHIC;					 
				else 
					return OSD_NULL;
			}
			else if (IsVideoInput(UserPrefCurrentInputMain)) //Video Input
			{
				if (UserPrefStateTempFlag == ENTER_POWER_SAVING) 
					return OSD_MAIN_ENTER_POWER_SAVING_VIDEO;
				else if (UserPrefStateTempFlag == NO_SIGNAL_STATE) 
					return OSD_MAIN_NO_SIGNAL_VIDEO;
				else   
					return OSD_NULL;	   
			}
		}	   	   	  
	}
	else if (IS_PBP()) // PBP Main
	{
		if (OSD_IsMainOutOfRange)
			return OSD_PBP_MAIN_OUT_OF_RANGE;

		if (UserPrefStateTempFlag == OUT_OF_RANGE_STATE) //PBP OOR
			return OSD_PBP_MAIN_OUT_OF_RANGE;

		if (!OSD_IsMainSync) // PBP Main No Signal
		{
			//if (!OSD_IsCableConnectedMain) // No Cable
			if(UserPrefStateTempFlag == NO_CABLE_STATE)
				return OSD_PBP_MAIN_NO_CABLE;

			if (IsGraphicInput(UserPrefCurrentInputMain))
			{
				if (UserPrefStateTempFlag == NO_SIGNAL_STATE)
					return OSD_PBP_MAIN_NO_SIGNAL_GRAPHIC;	

				if (UserPrefStateTempFlag == ENTER_POWER_SAVING) 
					return OSD_PBP_MAIN_ENTER_POWER_SAVING;		

				//if (UserPrefStateTempFlag == PIP_ON_SCREEN_STATE) 
				//	return OSD_PIP_ON_SCREEN;					 
			}
			else if(IsVideoInput(UserPrefCurrentInputMain))	
			{
				if (UserPrefStateTempFlag == NO_SIGNAL_STATE)  
					return OSD_PBP_MAIN_NO_SIGNAL_VIDEO;				 	

				//if (UserPrefStateTempFlag == PIP_ON_SCREEN_STATE) 
				//	return OSD_PIP_ON_SCREEN;			 
			}			  
		}	     
	}
	else if (Is_PIP_Pip()) // PIP Main
	{
		if ((OSD_IsMainOutOfRange))// PIP Main OOR
			return OSD_MAIN_OUT_OF_RANGE;	

		if (UserPrefStateTempFlag == OUT_OF_RANGE_STATE) //PIP OOR
			return OSD_MAIN_OUT_OF_RANGE;		  

		else if (!OSD_IsMainSync) // PIP Main No Signal
		{
			//if (!OSD_IsCableConnectedMain) // No Cable
			if(UserPrefStateTempFlag == NO_CABLE_STATE)
				return OSD_PBP_MAIN_NO_CABLE;

			if (IsGraphicInput(UserPrefCurrentInputMain))
			{
				if (UserPrefStateTempFlag == NO_SIGNAL_STATE)
					return OSD_MAIN_NO_SIGNAL_GAPHIC;			 	

				//if (UserPrefStateTempFlag == PIP_ON_SCREEN_STATE) 
				//	return OSD_PIP_ON_SCREEN;					   
			}
			else if (IsVideoInput(UserPrefCurrentInputMain))	
			{
				if (UserPrefStateTempFlag == NO_SIGNAL_STATE)  
					return OSD_MAIN_NO_SIGNAL_VIDEO;				 	

				//if (UserPrefStateTempFlag == PIP_ON_SCREEN_STATE) 
				//	return OSD_PIP_ON_SCREEN;				 
			}
		}		    
	}
	return OSD_NULL;	   		 	
	#endif
}

#endif


#ifdef  DELL_HIGH_DCR //KevinL 20120718 updated
static HighDcrActivationProcedureType HighDcrActivationProcedure = HighDCR_STRANDBY;
static WORD W_InputBrightnessLevel = 255;
//******************************************************************************
//
// FUNCTION       : void Get_ACC_Average_Brightness(void)
//
// USAGE          : Get ACC Average Brightness from the memory of ACC BIN 
//
// INPUT          : None
//
// OUTPUT         : None
//
//******************************************************************************
#define ACC3_ACCMEAS_HISTOGRAM_0_MEM_START 0x32E00L
WORD Get_ACC_Average_Brightness(void)
{
#if 1
	{
    	DWORD DW_AvgBrightness = 0; // (gm_ReadRegWord(ACC_AVG_BRIGHTNESS) & 0xFFFF) / 2;
    	DWORD DW_TotalPixel = 0;
    	union
    	{
        	DWORD  dw;
        	BYTE  Ba_Bin[4];
    	} DWordVal;
    	BYTE i;

		
    	for(i = 0; i < 64; i++) //64 BINs
    	{
        	DWORD DW_TempAddr = (DWORD)ACC3_ACCMEAS_HISTOGRAM_0_MEM_START + i * 4;         
   
        	DWordVal.Ba_Bin[0] = *(LINEAR_TO_FPTR_B(DW_TempAddr));
        	DW_TempAddr++;
        	DWordVal.Ba_Bin[1] = *(LINEAR_TO_FPTR_B(DW_TempAddr));
        	DW_TempAddr++;
        	DWordVal.Ba_Bin[2] = *(LINEAR_TO_FPTR_B(DW_TempAddr));
        	DW_TempAddr++;
        	DWordVal.Ba_Bin[3] = *(LINEAR_TO_FPTR_B(DW_TempAddr));

        	DW_TotalPixel += DWordVal.dw;
        	DW_AvgBrightness += DWordVal.dw * i; // total brightness
    	}
		
		
    	DW_AvgBrightness = DW_AvgBrightness/DW_TotalPixel; // avg brightness
    	//Remapping to YUV 4~58 to RGB 0~255
    	DW_AvgBrightness = (((DW_AvgBrightness-4)*255)+27)/54;
	
		return (WORD)DW_AvgBrightness;
	}	
#else
	{
		WORD W_AvgBrightness;
	 	DWORD DWa_HistBuff;	
	 
	 	DWa_HistBuff =  (DWORD)(*(LINEAR_TO_FPTR_B(ACCHighBinAddr)));	
	 	DWa_HistBuff |= ((DWORD)(*(LINEAR_TO_FPTR_B(ACCHighBinAddr+1)))) << 8;  
	 	DWa_HistBuff |= ((DWORD)(*(LINEAR_TO_FPTR_B(ACCHighBinAddr+2)))) << 16;   

	 	if(DWa_HistBuff > ACCHistogramLimit)
	 	{
	   		//gm_Print("ACC over limit",0);
	   		return (WORD)(0x8000);  
	 	} 
	 	else
	   	{
		   W_AvgBrightness = (gm_ReadRegWord(ACC_AVG_BRIGHTNESS) & 0xFFFF) / 2;
		   
		   if (W_AvgBrightness > 2000)
			   W_AvgBrightness -= 2000;
		   else
			   W_AvgBrightness = 0;

		   return W_AvgBrightness;
	   }
	}
#endif
}
//#endif

//100804 Edward for the cursor on the screen
BOOL IsBlackScreen(void)
{
	WORD temp_reg;
	WORD Store_Hi , Store_Lo;
		
	temp_reg = gm_ReadRegWord(IMP_SUMDIFF_MINMAX_CONFIG);
	//gm_Print("T1  = ....................... %d", gm_GetSystemTime());	

	if(gm_GetChannelInputAttr(CH_A, gmd_COLOR_SPACE) != gmd_RGB)
	{
		gm_WriteRegWord(IMP_SUMDIFF_MINMAX_CONFIG, IMP_SUMDIFF_EN | 0x02 | 0x200);	// enable sumdiff on G only for YUV domain
	}
	else
		gm_WriteRegWord(IMP_SUMDIFF_MINMAX_CONFIG, IMP_SUMDIFF_EN | IMP_SUMDIFF_RGB_SEL | 0x200);	// enable sumdiff on all colors
			
		gm_InputWaitVsync(gmvb_MainChannel, 2);
		
        Store_Hi = gm_ReadRegWord(IMP_SUMDIFF_RESULT_hi);
        Store_Lo = gm_ReadRegWord(IMP_SUMDIFF_RESULT_lo);

		//gm_Print("Store Hi = ....................... %d", Store_Hi);		
		//gm_Print("Store Lo = ....................... %d", Store_Lo);
		
		gm_WriteRegWord(IMP_SUMDIFF_MINMAX_CONFIG, temp_reg);	// enable sumdiff on all colors
		//gm_Print("T2  = ....................... %d", gm_GetSystemTime());	

		if ((Store_Hi == 0)&& (Store_Lo < 0xFF))
			return TRUE;
		else
			return FALSE;
 }	  	

//******************************************************************************
//
// FUNCTION       : static WORD ReadInputAverageBrigttness(void)
//
// USAGE          : return the average brightness level (0~255) of input signal
//
// INPUT          : None
//
// OUTPUT         : W_InputBrightness
//
//******************************************************************************
static WORD ReadInputAverageBrigttness(void)
{
#if (CHIP_ID == STDP93xx)
	return (WORD)Get_ACC_Average_Brightness();
#endif //(CHIP_ID == STDP93xx)
}

//
// U2410 High DCR - Activation Criteria
//
// DCR for Gaming and Movie Modes only
// DCR On/Off from OSD menu (default is on)
//	When DCR is On:
//	2 step DCR Activation Procedure
//		Step 1: Normal DCR turn on immediately 
//			1. DCR level(CR=tbd) at panel specified min Lamp Current
//		Step 2: High DCR - Activation Criteria
//			1. Activated when gray level < L4 for 10 seconds
//			2. High DCR continuous activation limit to 1 minute
//			3. High DCR activation only after the panel warm up for 10 minutes
//

static void ExitHighDCR_RestoreBackLight(void)
{
	#if((DEFAULT_PANEL == LG_WHD_LM290WW1_SSA1))
		SetBlueBacklightPWMPulse(0xFF -DCR_PWM_PULSE_MIN);	//DCR_NORMAL_PWM_PULSE_MIN);	
		SetGreenBacklightPWMPulse(0xFF -DCR_PWM_PULSE_MIN);	//DCR_NORMAL_PWM_PULSE_MIN);	
	#else
		DCR_EnergyUse_Brightness = DCR_NORMAL_PWM_PULSE_MIN;
		AdjustBackLight();	
	#endif
		DCR_BACKLIGHT_ON(); 	

	
	gm_Print("Exit HIGH DCR Restore Backlight", 0);
 }
static void HighDcrActivationCheck(void)
{
	if (W_InputBrightnessLevel < HIGH_DCR_GRAY_LEVEL_UNDER)
	{
		switch (HighDcrActivationProcedure)
		{
			case HighDCR_STRANDBY:
				gm_TimerStart(HIGH_DCR_ACTIVATION_1s_TMR, HIGH_DCR_ACTIVATION_TIME);
				HighDcrActivationProcedure = HighDCR_ACTIVATION;
				break;
				
			case HighDCR_ACTIVATION:	// 1. Activated when gray level < L4 for 10 seconds
				if (IS_DCR_ON() && (gm_TimerCheck(HIGH_DCR_PANEL_WARM_UP_1s_TMR) == TIMER_TMO) && 
					(gm_TimerCheck(HIGH_DCR_ACTIVATION_1s_TMR) == TIMER_TMO))
				{
					//120919 add flag"UserPrefMenuAdjust" for yes/no dialog do not turn off B.L  
					if(UserPrefMenuOnOffFlag || UserPrefMenuAdjust || (!IsBlackScreen()))
					{
						HighDcrActivationProcedure = HighDCR_STRANDBY;
						gm_Print("Reset HIGH DCR state", 0);
					}
					else
					{
						gm_TimerStart(HIGH_DCR_ACTIVATION_1s_TMR, HIGH_DCR_CONTINUOUS_LIMIT_TIME);
						gm_TimerStart(HIGH_DCR_BlackScreen_1s_TMR, HIGH_DCR_BlackScreen_TIME);
						HighDcrActivationProcedure = HighDCR_CONTINUOUS;
						gm_Print("Enter HIGH DCR state", 0);

				#if BACKLIGHT_PWM_INVERT
					SetBlueBacklightPWMPulse(0xFF);
					SetGreenBacklightPWMPulse(0xFF);
				#endif

				DCR_BACKLIGHT_OFF();	//20121030 Kordonm+ for DCR backlight gpio control

					}
				}
				break;

			case HighDCR_CONTINUOUS:	// 2. High DCR continuous activation limit to 1 minute
				if (gm_TimerCheck(HIGH_DCR_ACTIVATION_1s_TMR) == TIMER_TMO)
				{
					gm_TimerStop(HIGH_DCR_ACTIVATION_1s_TMR);
					#if 1
					HighDcrActivationProcedure = HighDCR_STRANDBY;
					ExitHighDCR_RestoreBackLight();
					
					gm_Print("Exit HIGH DCR state 1min Limit and reset H-DCR", 0);
					#else
					HighDcrActivationProcedure = HighDCR_LIMIT;					
					gm_Print("Exit HIGH DCR state for continuous 1 minut limit", 0);
					#endif
				}	
				else//120924 check is not black screen but ACC data is return 0 need to turn on panel 
				{
					if(gm_TimerCheck(HIGH_DCR_BlackScreen_1s_TMR) == TIMER_TMO)
					{					
						gm_TimerStop(HIGH_DCR_BlackScreen_1s_TMR);
						if(IsBlackScreen())
						{
							gm_TimerStart(HIGH_DCR_BlackScreen_1s_TMR, HIGH_DCR_BlackScreen_TIME);
							gm_Print("HGH DCR Black screen Timer restart", 0);
						}
						else
						{
							HighDcrActivationProcedure = HighDCR_STRANDBY;
							ExitHighDCR_RestoreBackLight();
							gm_Print("Exit HIGH DCR state not Black screen", 0);
						}
					}
				}
				break;
		}
	}
	else
	{
		gm_TimerStop(HIGH_DCR_ACTIVATION_1s_TMR);
		HighDcrActivationProcedure = HighDCR_STRANDBY;		
	}
}

void HighDCR_QuitCheck(void)//120704 Edward test for OSD display should need to adjust backlight
{
	if(IS_DCR_ON()&&(B_SplashComplete == TRUE)) 
	{		
  	  	#if (defined(CMI_73XX_BOARD)) 
			if((DCR_EnergyUse_Brightness == DCR_HIGH_PWM_PULSE_MIN))	 		
			{
				gm_TimerStop(HIGH_DCR_ACTIVATION_1s_TMR);
				if(gm_TimingIsStable(gmvb_MainChannel))
				{
					//120914 Edward reset H-DCR fucntion
					HighDcrActivationProcedure = HighDCR_STRANDBY;//HighDCR_LIMIT;//is key press to stop high DCR
					//DCR_EnergyUse_Brightness = UserPrefBrightness;
				}	
				else
					HighDcrActivationProcedure = HighDCR_STRANDBY;//is modechange to stop high DCR , so need to restart high DCR
			
				ExitHighDCR_RestoreBackLight();
			
				gm_Print("Exit HIGH DCR state for OSD Display", 0);
			}
		#elif (defined(Athena_U)) 
			WORD W_PWMPulse = 0;

			W_PWMPulse = ReadBlueBacklightPWMPulse();//121009 Edward modify for Wistron need to convert PWM again
			//gm_Print("PWM value W_PWMPulse 0x%x", W_PWMPulse);
			if((W_PWMPulse >= (0xFF-DCR_NORMAL_PWM_PULSE_MIN)))	 		
			{
				gm_TimerStop(HIGH_DCR_ACTIVATION_1s_TMR);
				if(gm_TimingIsStable(gmvb_MainChannel))
				{
					//120914 Edward reset H-DCR fucntion
					HighDcrActivationProcedure = HighDCR_STRANDBY;//HighDCR_LIMIT;//is key press to stop high DCR
				}
				else
					HighDcrActivationProcedure = HighDCR_STRANDBY;//is modechange to stop high DCR , so need to restart high DCR
			
				ExitHighDCR_RestoreBackLight();

				gm_Print("Exit HIGH DCR state for OSD Display", 0);
			}
	  	#else
	    	if((ReadBlueBacklightPWMPulse() <= DCR_HIGH_PWM_PULSE_MIN))	 		
			{
				gm_TimerStop(HIGH_DCR_ACTIVATION_1s_TMR);
				if(gm_TimingIsStable(gmvb_MainChannel))
				{
					//120914 Edward reset H-DCR fucntion
					HighDcrActivationProcedure = HighDCR_STRANDBY;//HighDCR_LIMIT;//is key press to stop high DCR
				}
				else
					HighDcrActivationProcedure = HighDCR_STRANDBY;//is modechange to stop high DCR , so need to restart high DCR
			
				ExitHighDCR_RestoreBackLight();
				
				gm_Print("Exit HIGH DCR state for OSD Display", 0);
		    }
	  #endif
	}	
}



//#define DCR_CHECK_TIME_FAST			18 //121008 KimChye	// 180ms / 10ms
//#define DCR_CHECK_TIME_MID_FAST		240 //121008 KimChye	// 180ms / 10ms

#if (defined(CMI_73XX_BOARD))//owen 121017 add 30


#define DCR_CHECK_TIME_FAST			18 //121008 KimChye	// 180ms / 10ms

#if defined(CMI_73XX_BOARD)
#define DCR_CHECK_TIME	800//18 //121005 KimChye	// 8000ms / 10ms
 #endif
#if defined(WISTRON_93XX_BOARD_30)
 #define DCR_CHECK_TIME	800 //121101 KimChye
#endif
 

void DCR_Handler(void)//For CMI_73XX_BOARD
{
	WORD curAvgINTXY;
	//static WORD curAvgINTXY_tmp;
	BYTE W_ConvertedPWMPulse = 0;
	
      if((UserPrefEnergySmartOnOffFlag == On) ||(UserPrefIsEnergySmartToggledFlag == TRUE)) //KevinL 20101116 FY12 OSD feature      	     
	   return;     
	  #if 0
	if (IsEnterHiDCRStatus)//calo add for the factory command to test high DCR status
	{
		if ((gm_TimerCheck(HIGH_DCR_TEST_1s_TMR) == TIMER_TMO) || (gm_TimerCheck(HIGH_DCR_TEST_1s_TMR) == TIMER_STOP))
		{ 
			IsEnterHiDCRStatus = FALSE;
			gm_TimerStop(HIGH_DCR_TEST_1s_TMR);
			SetBacklightPWMPulse(CurrentBrightness);
		}
		else
			SetBacklightPWMPulse(DCR_HIGH_PWM_PULSE_MIN);
	}
	else
		#endif
	{

       	// ********* High DCR state 
       	// HighDcrActivationProcedure 
		// 0 : HighDCR_STRANDBY  --- System is not in High DCR mode, input level > L4
		// 1 : HighDCR_ACTIVATION --- High DCR activate when gray level < L4 for 10 seconds
		// 2 : HighDCR_CONTINUOUS ---  High DCR is activating
		// 3 : HighDCR_LIMIT ---  High DCR active 1 Mins time out, change to normal DCR			
		if(IS_DCR_ON())
			HighDcrActivationCheck();

		if (IS_DCR_ON() && (gm_TimerCheck(DCR_CHECK_10ms_TMR) != TIMER_OK))//20110815 Modify condition TIMER_OK by KevinChen.
		{
			WORD W_Imp_Brightness;
			
			//W_InputBrightnessLevel = ReadInputAverageBrigttness();
			W_InputBrightnessLevel = Get_ACC_Average_Brightness();
			//gm_Print("W_Imp_Brightness : %d",W_InputBrightnessLevel);

			//B_PwmPulse = DCR_EnergyUse_Brightness;
			//gm_Print("DCR_EnergyUse_Brightness-%d",DCR_EnergyUse_Brightness);
			//gm_Print("DCR_PWM_PULSE_MAX-%d",DCR_PWM_PULSE_MAX);
			//gm_Print("DCR_PWM_PULSE_MIN-%d",DCR_PWM_PULSE_MIN);
		    //gm_Print("HighDcrActivationProcedure-%d",HighDcrActivationProcedure);			   
			   
			if( W_InputBrightnessLevel > 248) // 121009 KimChye
				gm_TimerStart(DCR_CHECK_10ms_TMR,DCR_CHECK_TIME_FAST);
			else
				gm_TimerStart(DCR_CHECK_10ms_TMR,DCR_CHECK_TIME);

			if (DCR_EnergyUse_Brightness > DCR_PWM_PULSE_MAX)
			{
				DCR_EnergyUse_Brightness = DCR_PWM_PULSE_MAX;				
				AdjustBackLight();
			}
			else if (DCR_EnergyUse_Brightness < DCR_PWM_PULSE_MIN)
			{
				DCR_EnergyUse_Brightness = DCR_PWM_PULSE_MIN;
				AdjustBackLight();				
			}
			
			W_ConvertedPWMPulse = ConvertImpBrightnessToPwmPulse(W_InputBrightnessLevel);			
			//gm_Printf("ConvertImpBrightnessToPwmPulse(%d)-%d",W_InputBrightnessLevel, W_ConvertedPWMPulse);			
			#if(defined(WISTRON_93XX_BOARD_30))
			if((HighDcrActivationProcedure != HighDCR_CONTINUOUS))//owen 121031 
			{
				DCR_BACKLIGHT_ON();
			}
			#endif


			if ((abs(W_ConvertedPWMPulse - DCR_EnergyUse_Brightness) > DCR_NOISE_THRESHOLD) || 
				((W_ConvertedPWMPulse != DCR_EnergyUse_Brightness) && 
				((W_ConvertedPWMPulse > (DCR_PWM_PULSE_MAX - DCR_NOISE_THRESHOLD)) || 
				(W_ConvertedPWMPulse < (DCR_PWM_PULSE_MIN + DCR_NOISE_THRESHOLD)))))
			{
				gm_TimerStart(DELAY_1ms_TMR, 100);
				// clear VBLANK edge from previous frame
				gm_WriteRegWord(MAIN_INPUT_STATUS, IMP_VACTIVE);
				while ((gm_ReadRegWord(MAIN_INPUT_STATUS) & IMP_VACTIVE) == 0)
				{
					if (gm_TimerCheck(DELAY_1ms_TMR) == TIMER_TMO)
						break;	// timeout - return with error
				}

				if (DCR_EnergyUse_Brightness < W_ConvertedPWMPulse)
				{
					DCR_EnergyUse_Brightness += 1;
					//gm_Print("PWM Pulse ++ : %d",DCR_EnergyUse_Brightness);
				
					AdjustBackLight();
				}
				else if (DCR_EnergyUse_Brightness > W_ConvertedPWMPulse)
				{
					DCR_EnergyUse_Brightness -= 1;
					//gm_Print("PWM Pulse --: %d",DCR_EnergyUse_Brightness);
					
					AdjustBackLight();
				}
				
		}
	}
}
}

#else

#define DCR_CHECK_TIME	300//18 //121005 KimChye	// 5000ms / 10ms
#define DCR_CHECK_TIME_MID_FAST		240 //121008 KimChye	
#define DCR_CHECK_TIME_FAST			18 //121008 KimChye	// 180ms / 10ms
#define DCR_LEVEL_HIGH 248
#define DCR_LEVEL_MID_START 220
#define DCR_LOW_STEP	2

void DCR_Handler(void)
{
	WORD curAvgINTXY;
	//static WORD curAvgINTXY_tmp;
	WORD W_ConvertedPWMPulse = 0;
	
      if((UserPrefEnergySmartOnOffFlag == On) ||(UserPrefIsEnergySmartToggledFlag == TRUE)) //KevinL 20101116 FY12 OSD feature      	     
	   return;     
	  #if 0
	if (IsEnterHiDCRStatus)//calo add for the factory command to test high DCR status
	{
		if ((gm_TimerCheck(HIGH_DCR_TEST_1s_TMR) == TIMER_TMO) || (gm_TimerCheck(HIGH_DCR_TEST_1s_TMR) == TIMER_STOP))
		{ 
			IsEnterHiDCRStatus = FALSE;
			gm_TimerStop(HIGH_DCR_TEST_1s_TMR);
			SetBacklightPWMPulse(CurrentBrightness);
		}
		else
			SetBacklightPWMPulse(DCR_HIGH_PWM_PULSE_MIN);
	}
	else
		#endif
	{

       	// ********* High DCR state 
       	// HighDcrActivationProcedure 
		// 0 : HighDCR_STRANDBY  --- System is not in High DCR mode, input level > L4
		// 1 : HighDCR_ACTIVATION --- High DCR activate when gray level < L4 for 10 seconds
		// 2 : HighDCR_CONTINUOUS ---  High DCR is activating
		// 3 : HighDCR_LIMIT ---  High DCR active 1 Mins time out, change to normal DCR			
		if(IS_DCR_ON())
			HighDcrActivationCheck();

		if (IS_DCR_ON() && (gm_TimerCheck(DCR_CHECK_10ms_TMR) != TIMER_OK))//20110815 Modify condition TIMER_OK by KevinChen.
		{
			BYTE B_PwmPulse;
			//gm_Print("Stime-%d",gm_GetSystemTime());
			//gm_TimerStart(DCR_CHECK_10ms_TMR, DCR_CHECK_TIME);
			//InputBrightnessLevel = ReadInputAverageBrigttness();
			W_InputBrightnessLevel = Get_ACC_Average_Brightness();
			//gm_Print("W_InputBrightnessLevel A : %d",W_InputBrightnessLevel);
			B_PwmPulse = ReadBlueBacklightPWMPulse();
			//gm_Print("B_PwmPulse-%d",B_PwmPulse);
			//gm_Print("DCR_PWM_PULSE_MAX-%d",DCR_PWM_PULSE_MAX);
			//gm_Print("DCR_PWM_PULSE_MIN-%d",DCR_PWM_PULSE_MIN);
		    //gm_Print("HighDcrActivationProcedure-%d",HighDcrActivationProcedure);			   

			if( W_InputBrightnessLevel > DCR_LEVEL_HIGH ) // 121009 KimChye
				gm_TimerStart(DCR_CHECK_10ms_TMR,DCR_CHECK_TIME_FAST);
			else if((W_InputBrightnessLevel > DCR_LEVEL_MID_START) && (W_InputBrightnessLevel < DCR_LEVEL_HIGH))
				gm_TimerStart(DCR_CHECK_10ms_TMR,DCR_CHECK_TIME_MID_FAST);
			else
				gm_TimerStart(DCR_CHECK_10ms_TMR,DCR_CHECK_TIME);


  		#if 0
			if (B_PwmPulse > DCR_PWM_PULSE_MAX)
			{
				SetBlueBacklightPWMPulse(DCR_PWM_PULSE_MAX);
				SetGreenBacklightPWMPulse(DCR_PWM_PULSE_MAX);
				B_PwmPulse = DCR_PWM_PULSE_MAX;
			}
			else if (B_PwmPulse < DCR_PWM_PULSE_MIN)
			{
				SetBlueBacklightPWMPulse(DCR_PWM_PULSE_MIN);
				SetGreenBacklightPWMPulse(DCR_PWM_PULSE_MIN);
				B_PwmPulse = DCR_PWM_PULSE_MIN;
			}
			W_ConvertedPWMPulse = ConvertImpBrightnessToPwmPulse(W_InputBrightnessLevel);

		#elif (defined(Athena_U) )
			#if BACKLIGHT_PWM_INVERT
			if (B_PwmPulse > (0xFF - DCR_PWM_PULSE_MIN))	
			{
				B_PwmPulse = (0xFF - DCR_PWM_PULSE_MIN);
				SetBlueBacklightPWMPulse(B_PwmPulse);
				SetGreenBacklightPWMPulse(B_PwmPulse);
			}
			else if (B_PwmPulse < (0xFF - DCR_PWM_PULSE_MAX))
			{
				B_PwmPulse = (0xFF - DCR_PWM_PULSE_MAX);
				SetBlueBacklightPWMPulse(B_PwmPulse);
				SetGreenBacklightPWMPulse(B_PwmPulse);
			}
			W_ConvertedPWMPulse = ConvertImpBrightnessToPwmPulse(W_InputBrightnessLevel);
			//gm_Printf("Convert(%d)-0x%x",W_InputBrightnessLevel, ConvertImpBrightnessToPwmPulse(W_InputBrightnessLevel));

			W_ConvertedPWMPulse = (0xFF - W_ConvertedPWMPulse);	// + DCR_PWM_PULSE_MIN);	//Invert DCR Target PWM Pulse Value
			//W_ConvertedPWMPulse = (DCR_PWM_PULSE_MAX - W_ConvertedPWMPulse);	// + DCR_PWM_PULSE_MIN);	//Invert DCR Target PWM Pulse Value
			//gm_Print("W_ConvertedPWMPulse-%d",W_ConvertedPWMP ulse);

            		#else
			if (B_PwmPulse < DCR_PWM_PULSE_MIN)
			{
				B_PwmPulse = DCR_PWM_PULSE_MIN;
				SetBlueBacklightPWMPulse(B_PwmPulse);
				SetGreenBacklightPWMPulse(B_PwmPulse);
			}
			else if (B_PwmPulse > DCR_PWM_PULSE_MAX)
			{
				B_PwmPulse = DCR_PWM_PULSE_MAX;
				SetBlueBacklightPWMPulse(B_PwmPulse);
				SetGreenBacklightPWMPulse(B_PwmPulse);
			}
			W_ConvertedPWMPulse = ConvertImpBrightnessToPwmPulse(W_InputBrightnessLevel);

			#endif

		if((HighDcrActivationProcedure!=HighDCR_CONTINUOUS))		//20121030 Kordonm+ for DCR backlight gpio control, sometimes when in HDCR state and recovery data, DCR BACKLIGHT does't turn on
		{
			DCR_BACKLIGHT_ON();
		}
			#endif

			if ((abs(W_ConvertedPWMPulse - B_PwmPulse) > DCR_NOISE_THRESHOLD) || 
				((W_ConvertedPWMPulse != B_PwmPulse) && 
				((W_ConvertedPWMPulse > (DCR_PWM_PULSE_MAX - DCR_NOISE_THRESHOLD)) || 
				(W_ConvertedPWMPulse < (DCR_PWM_PULSE_MIN + DCR_NOISE_THRESHOLD)))))
			{
				gm_TimerStart(DELAY_1ms_TMR, 100);
				// clear VBLANK edge from previous frame
				gm_WriteRegWord(MAIN_INPUT_STATUS, IMP_VACTIVE);
				while ((gm_ReadRegWord(MAIN_INPUT_STATUS) & IMP_VACTIVE) == 0)
				{
					if (gm_TimerCheck(DELAY_1ms_TMR) == TIMER_TMO)
						break;	// timeout - return with error
				}

				if (B_PwmPulse < W_ConvertedPWMPulse)
				{
					//gm_Print("PWM Pulse ++ : %d",B_PwmPulse + 1);
					SetBlueBacklightPWMPulse(B_PwmPulse + 1);
					SetGreenBacklightPWMPulse(B_PwmPulse + 1);
				}
				else if (B_PwmPulse > W_ConvertedPWMPulse)
				{
					//gm_Print("PWM Pulse --: %d",B_PwmPulse - 1);
					if(W_InputBrightnessLevel < DCR_LEVEL_MID_START )
					{
					SetBlueBacklightPWMPulse(B_PwmPulse - DCR_LOW_STEP );  //131212 KimChye
					SetGreenBacklightPWMPulse(B_PwmPulse - DCR_LOW_STEP); //131212 KimChye
					}
					else
					{
					SetBlueBacklightPWMPulse(B_PwmPulse - 1);
					SetGreenBacklightPWMPulse(B_PwmPulse - 1);
					}
				}
				
				#ifdef Qisda_93XX_BOARD//20120531 For Qisda test
				{
				static BYTE DCR_TurnOffBacklight=0;
				//gm_Print("W_Imp_Brightness-%d",W_Imp_Brightness);
				if(ReadBlueBacklightPWMPulse()<=MIN_BACKLIGHT_PWM&&DCR_TurnOffBacklight!=0)//20120905 Modify DCR backlight-off condition.
				{

					if(UserPrefMenuOnOffFlag)	//yvonne 20120920 for enter highDCR press Menu key, can't see the menu issue
					{
						DCR_TurnOffBacklight=1;
						gm_TimerStop(HIGH_DCR_ACTIVATION_1s_TMR);
						HighDcrActivationProcedure = HighDCR_STRANDBY;
						ExitHighDCR_RestoreBackLight();
						gm_Print("Reset HIGH DCR state", 0);
					}
					else
					{
					DCR_TurnOffBacklight=0;
					//gm_DisplayOnOff(Off);
					gm_ClearRegBitsWord(GPOUTPUT1,GPIO29_OUT);//20120809 Add by KevinChen. Set low. Panel off by GPIO29.
					#if 0//def Qisda_93XX_BOARD
					gm_Delay1ms(Bit_Select_Off_time);
					gm_ClearRegBitsWord(GPOUTPUT1,GPIO20_OUT);
					#endif
					gm_Print("OFF",0);
				}
				}
				else if (ReadBlueBacklightPWMPulse()>MIN_BACKLIGHT_PWM&&DCR_TurnOffBacklight==0)//20120905 Modify DCR backlight-on condition.
				{
					DCR_TurnOffBacklight=1;
					//gm_DisplayOnOff(On);
					gm_SetRegBitsWord(GPOUTPUT1,GPIO29_OUT);////20120809 Add by KevinChen. Set high. Panel on/off control by PBIAS.
					#if 0//def Qisda_93XX_BOARD
					gm_Delay1ms(Bit_Select_On_time);
					gm_SetRegBitsWord(GPOUTPUT1,GPIO20_OUT);
					#endif
					gm_Print("ON",0);
				}
			}
				#endif
		}
	}
}
}
		#endif
static void HighDcrProcedureForPanelOnOff(BOOL On_Off)
{
	if (On_Off == On)
		gm_TimerStart(HIGH_DCR_PANEL_WARM_UP_1s_TMR, HIGH_DCR_PANEL_WARM_UP_TIME);
	else
	{
		gm_TimerStop(HIGH_DCR_ACTIVATION_1s_TMR);
		gm_TimerStop(HIGH_DCR_PANEL_WARM_UP_1s_TMR);
	}
	HighDcrActivationProcedure = HighDCR_STRANDBY;
}

void DCR_Test_Handler(void) //KevinL 20090421 High DCR always on
{
	if (UserPrefDCRTestOnOffFlag == On)
	{
		WORD W_Imp_Brightness;
		BYTE B_PwmPulse;    

		HighDcrActivationProcedure = HighDCR_CONTINUOUS;	
		W_Imp_Brightness = ReadInputAverageBrigttness();
		B_PwmPulse = ReadBlueBacklightPWMPulse();
		if (B_PwmPulse > DCR_PWM_PULSE_MAX)
		{
			SetBlueBacklightPWMPulse(DCR_PWM_PULSE_MAX);
			SetGreenBacklightPWMPulse(DCR_PWM_PULSE_MAX);
			B_PwmPulse = DCR_PWM_PULSE_MAX;
		}
		else if (B_PwmPulse < DCR_PWM_PULSE_MIN)
		{
			SetBlueBacklightPWMPulse(DCR_PWM_PULSE_MIN);
			SetGreenBacklightPWMPulse(DCR_PWM_PULSE_MIN);
			B_PwmPulse = DCR_PWM_PULSE_MIN;
		}

		if ((abs(ConvertImpBrightnessToPwmPulse(W_Imp_Brightness) - B_PwmPulse) > DCR_NOISE_THRESHOLD) || 
			((ConvertImpBrightnessToPwmPulse(W_Imp_Brightness) != B_PwmPulse) && 
			((ConvertImpBrightnessToPwmPulse(W_Imp_Brightness) > (DCR_PWM_PULSE_MAX - DCR_NOISE_THRESHOLD)) || 
			(ConvertImpBrightnessToPwmPulse(W_Imp_Brightness) < (DCR_PWM_PULSE_MIN + DCR_NOISE_THRESHOLD)))))
		{
			gm_TimerStart(DELAY_1ms_TMR, 100);
			// clear VBLANK edge from previous frame
			gm_WriteRegWord(MAIN_INPUT_STATUS, IMP_VACTIVE);
			while ((gm_ReadRegWord(MAIN_INPUT_STATUS) & IMP_VACTIVE) == 0)
			{
				if (gm_TimerCheck(DELAY_1ms_TMR) == TIMER_TMO)
				break;	// timeout - return with error
			}

			if (B_PwmPulse < ConvertImpBrightnessToPwmPulse(W_Imp_Brightness))
			{
				SetBlueBacklightPWMPulse(B_PwmPulse + 1);
				SetGreenBacklightPWMPulse(B_PwmPulse + 1);
			}	
			else if (B_PwmPulse > ConvertImpBrightnessToPwmPulse(W_Imp_Brightness))
			{
				SetBlueBacklightPWMPulse(B_PwmPulse - 1);
				SetGreenBacklightPWMPulse(B_PwmPulse - 1);
			}		
		}				   
	}
}
#endif//endif DELL_HIGH_DCR

#ifdef USB_POWER_CONTROL
void UsbPowerHandler(void)		//Neil 20090402 modify for 5V to 3.7 control base on USB power and audio sound bar power status
{
#if 1//ADD BY LION 20090417
	static BOOL bUsbJustConnected = FALSE;
	 static BYTE UsbConnectStatus = 0;
	 static BOOL bSpeakerPowerJustChangeTo5v=FALSE;
	 static BYTE SpeakerPowerStatus=0;
	if(UsbConnectStatus!=IsUsbConnect())
	{
		UsbConnectStatus= IsUsbConnect();
		if(IsUsbConnect()) 
			bUsbJustConnected= TRUE;

	}
	if(SpeakerPowerStatus!= (AudioDetect()&&(UserPrefPwrAudio == OFF)))
	{
			SpeakerPowerStatus = (AudioDetect()&&UserPrefPwrAudio == OFF);
			if(AudioDetect()&&UserPrefPwrAudio == OFF)bSpeakerPowerJustChangeTo5v=TRUE;
	}
#endif
	#if 1  // For USB ISP  simon add 090305
		if(UserPrefCurrentInputMain == ALL_BOARDS_HDMI)
		{
			 if(IsUsbConnect() && (gm_ReadRegWord(GPINPUT1)&BIT1) == 0   )
					gm_ClearRegBitsWord(DDC2B_CTRL, DDC2B_SRC_SEL);
		}
	#endif
#ifdef DELL_U2410F
	if (PwrState == Pwr_Off) //|| !IsUsbConnect()||(UserPrefPwrAudio == ON))
	{
		#if 0//def PowerSavePinUsed  	
			if (PwrState == Pwr_DeepSleep)	
			{
				//if (((gm_ReadRegWord(GPOUTPUT5) & PIN_PWR_SAVE) == PIN_PWR_SAVE)&&(UserPrefPwrAudio == ON))	
				//	gm_ClearRegBitsWord(GPOUTPUT5, PIN_PWR_SAVE); 		              
			}
		#endif				  
		SetUsbPower_OFF();
	}
	else if(PwrState == Pwr_DeepSleep) //&&((AudioDetect()&&(UserPrefPwrAudio == OFF))||(IsUsbConnect())))	//Neil 20090402*
	{
	#ifdef PowerSavePinUsed  
		//Neil 20090402+ for power save pin control when USB connect and speaker bar power on	
		if((AudioDetect()&&(UserPrefPwrAudio == OFF))||(IsUsbConnect()))
		{				
			gm_SetRegBitsWord(GPOUTPUT5, PIN_PWR_SAVE); 		
		}
		else
		{
		    if(AudioDetect()) //KevinL 20090529 enable PIN_PWR_SAVE when Speak power connected
		    	gm_SetRegBitsWord(GPOUTPUT5, PIN_PWR_SAVE); 
		    else	
			gm_ClearRegBitsWord(GPOUTPUT5, PIN_PWR_SAVE); 
		}
		
		if(AudioDetect()&&(UserPrefPwrAudio == OFF))
		{
			#ifdef (CMI_73XX_BOARD)
				//work like this? need check later	Neil 
				gm_SetRegBitsWord(LPM_GPOUTPUT_0, LPM_GPIO9_OUT); 	//default set as high, power saving set low by condition
			#else
			//gm_SetRegBitsWord(GPOUTPUT5, PIN_PWR24V_ON); //KevinL 20090529 connect Speak power should enable 24V
			#endif

		
			//gm_SetRegBitsWord(GPIO_5_ENABLE, AUDIO_EN);   //simon 20090529 for AudioSpeakerPowerCheck function AudioCableNotDetectCnt > 10 then gm_ClearRegBitsWord(GPOUTPUT5,AUDIO_EN); 
			gm_SetRegBitsWord(GPOUTPUT5, AUDIO_EN);         //bertonchange 2009/05/30
		}
				
	#endif	
		if(IsUsbConnect())		//Neil 20090415*  for power saving power comsumption
			SetUsbPower_ON();
		else			
			SetUsbPower_OFF();
		#if 1//add by lion 20090417
			if(bUsbJustConnected||bSpeakerPowerJustChangeTo5v)
			{
						gm_Print("USB CABLE  PLUG IN",0);
						gm_Delay100ms(1);
						SynapitcsIICWriteRegister(0x0010, 0);//(SENSOR1_SENS*128)|SENSOR0_SENS);
						SynapitcsIICWriteRegister(0x0011, 0);//(SENSOR3_SENS*128)|SENSOR2_SENS);
						SynapitcsIICWriteRegister(0x0012, 0);//(SENSOR5_SENS*128)|SENSOR4_SENS);
						SynapitcsIICWriteRegister(0x0013, 0);//(SENSOR7_SENS*128)|SENSOR6_SENS);
						gm_Delay100ms(1);
						SynapitcsDriverInit();
			}
		#endif
	}
	else
	{
		SetUsbPower_ON();
	#if 0// Simon 090610 //120521 Edward Disable no video port
		if (((UserPrefCurrentInputMain == ALL_BOARDS_CVBS1)||(UserPrefCurrentInputMain == ALL_BOARDS_COMP1))&&(PwrState == Pwr_On)) //simon add for when no signal on Composite&component, powersaving audio off, unplug and pulg speaker, speaker no power 
		{
			if (AudioDetect()&&(UserPrefPwrAudio == OFF))
			{
				gm_SetRegBitsWord(GPOUTPUT5, AUDIO_EN); 
			}
		}
	#endif
	}
#endif    
#if 1//add by lion  20090417  PLEASE KEEP THIS  AT THE BOOTOM OF THIS FUNCTION
	bUsbJustConnected=FALSE;
	bSpeakerPowerJustChangeTo5v=FALSE;
#endif
	}
#endif

void CustomPanelOnOffControl(BOOL BL_PanelOnOff)
{
	if (BL_PanelOnOff == On)
	{
	#ifdef PIVOT_SENSOR_POWER_CONTROL
		SetPivotSensorPower_ON();
	#endif
	#ifdef DELL_HIGH_DCR
		HighDcrProcedureForPanelOnOff(On);
	#endif //#ifdef DELL_HIGH_DCR
	}
	else
	{
	#ifdef DELL_HIGH_DCR
		HighDcrProcedureForPanelOnOff(Off);
	#endif //#ifdef DELL_HIGH_DCR
	#ifdef PIVOT_SENSOR_POWER_CONTROL
		SetPivotSensorPower_OFF();
	#endif
	}
}

void gm_ForceModeChange(gmt_PHY_CH Channel)
{
	ModeSetupRequest(Channel, REQ_MS_RESTART, PARAM_NONE, REQUEST_NOW);
}

BYTE IsNativeModeInput(gmt_PHY_CH Channel)
{
	return ((abs(gmvw_InputHActiveOrg[Channel] - PanelWidth)<3) &&(abs(gmvw_InputVActiveOrg[Channel] - PanelHeight)<3)) ;	
}

//Force decoder to free run instead of force mode to decoder
/*void SetDecoderFreeRun(gmt_PHY_CH Channel)
{
		gmt_DECODER_ID B_DecId = gmd_DEC_CORTEZ_ID;

		#if (FEATURE_ADC_NUM == 2)
			if( gm_GetAiptoChannel(Channel) == gmd_ADC_1 )
				B_DecId = gmd_DEC_CORTEZ_ID2;
		#endif

		gm_DecoderFreeRunModeEnable(B_DecId, TRUE);
}*/

//******************************************************************************
//
// FUNCTION       : gmt_MODE_TYPE gm_GetCurrentInputModeType (gmt_PHY_CH B_Channel)
//
// USAGE          : Selects and returns the mode type of the current channel based on gmvw_InputVTotal
//
// INPUT          : B_Channel
//
// OUTPUT         : gmt_MODE_TYPE	
//
// GLOBALS        : gmvb_InputVTotal[]
//
// USED_REGS      : None
//
// PRE_CONDITION  : None
//
// POST_CONDITION : None
//
//******************************************************************************
//Neil20081222+ There are some mode Vtotal overlap when determine the mode type
gmt_MODE_TYPE gm_GetCurrentInputModeType_New(gmt_PHY_CH B_Channel)	
{
	switch (stInput[B_Channel].Vtotal)
	{
		//
		// NTSC/480i (input to Cortez)
		//
		case 262:
		case 263:
			return gmd_NTSC_480I;

		//
		// PAL/576i (input to Cortez)
		//
		case 312:
		case 313:
			return gmd_PAL_576I;
	
		//
		// 480p (input to Cortez)
		//
		case 524: 
		case 525:
			return gmd_480P;

		//
		// 576p (input to Cortez) or 1152iLA, 1152iSH or 1152iLH
		//
		case 624:
		case 625:
		case 626:
		case 627:
			if ((abs(stInput[B_Channel].Hfrq - 312) < 10)&&(abs(stInput[B_Channel].Htotal- 864) < 20))	//576P Hfrq = 312, 800X600@56/75Hz Vtotal = 625
				return gmd_576P;	
			else if((abs(stInput[B_Channel].Htotal- 2304) < 4)&&(abs(stInput[B_Channel].Hactive - 1920) < 2)
				&&(abs(stInput[B_Channel].Vactive - 540) < 4))		//Neil120719+ for fake 1080i(VG870 1075) do over scan
				return gmd_1080I;
			else
				return gmd_UNKNOWN_MODE;

		//
		// 720p (input to Cortez)
		//
		case 750:
			return gmd_720P;

		//
		// 1080i (input to Cortez)
		//
		case 232:		
		case 561:		
		case 562:
		case 563:
			return gmd_1080I;

		//
		// 1080p (input to Cortez)
		//
		case 1125:
				return gmd_1080P;

		//
		// Other Modes
		//
		default:
			return gmd_UNKNOWN_MODE;
	}	
}

BOOL IsSDVideo(gmt_PHY_CH B_Channel)//KevinL 20100309
{
   switch(stInput[B_Channel].Vtotal)
	{
		//
		// NTSC/480i (input to Cortez)
		//
		case 262:
		case 263:
		//
		// PAL/576i (input to Cortez)
		//
		case 312:
		case 313:
		//
		// 480p (input to Cortez)
		//
		case 524: 
		case 525:
		//
		// 576p (input to Cortez) or 1152iLA, 1152iSH or 1152iLH
		//
		   return TRUE;
		case 624:
		case 625:
		case 626:
		case 627:
			if ((abs(stInput[B_Channel].Hfrq - 312) < 10)&&(abs(stInput[B_Channel].Htotal- 864) < 20))	//576P Hfrq = 312, 800X600@56/75Hz Vtotal = 625
				return TRUE;	
			else
				return FALSE;
		//
		// Other Modes
		//
		default:
			return FALSE;
   }
}

void ResetIFMWatchDog(gmt_PHY_CH B_Channel)
{
	WORD RegAdd;
	//WORD RegVal = gm_ReadRegWord(RegAdd);
	RegAdd = (B_Channel == CH_A)?MIFM_WATCHDOG:PIFM_WATCHDOG;

	gm_WriteRegWord(RegAdd, 0);
	gm_Print(">>>>>>>>>>>>>>>>>ResetIFMWatchDog", 0);
}

/*void SetFunctionRunningFlag(CheckFunctionState Adj_Flag,CheckKeyState KeyPressed)
{
	if(Adj_Flag == Adj_Zoom ||Adj_Flag == Adj_ImageScheme)
	{
		FunctionRunningFlag = Adj_Flag;
		KeyPressFlag = KeyPressed;	
	}
	else
	{
		FunctionRunningFlag = Adj_None;
		KeyPressFlag = KeyPressed;	
	}
}*/

void SetOSDPosition(void)		//Neil 20090206+ for CVBS & Componet no sync input ,AC on message overlap
{
	//H max = 600;V max = 800;
	if(UserPrefPipMode == SIDE_BY_SIDE)
	{
		UserPrefOsdHPosCenter = 90;
		UserPrefOsdVPosCenter = 410;
		//gm_Print("SetOSDPosition H = %d", UserPrefOsdHPosCenter);
	}
	else
	{
		UserPrefOsdHPosCenter = 300;
		UserPrefOsdVPosCenter = 400;
		//gm_Print("SetOSDPosition H = %d", UserPrefOsdHPosCenter);
	}
}

void AudioControl(gmt_PHY_CH B_Channel)
{
	BYTE CurrentInput = (B_Channel == CH_A)? UserPrefCurrentInputMain: UserPrefCurrentInputPip;

	//Neil 20090311+ for none HDMI or DP input, Mute audio;
//&*&*&*S 20120315:SWRD4001: Kordonm(0)  Modify for OSD Workbench
	if ((UserPrefCurrentInputMain != ALL_BOARDS_DP1) && (UserPrefCurrentInputMain != ALL_BOARDS_DP3) && (UserPrefCurrentInputMain != ALL_BOARDS_HDMI)
		&& (UserPrefCurrentInputPip != ALL_BOARDS_DP1) && (UserPrefCurrentInputPip != ALL_BOARDS_DP3) && (UserPrefCurrentInputPip != ALL_BOARDS_HDMI))
		SetAudioDACOutput(OFF);		//Neil 20090312+
//&*&*&*E 20120315 : Kordonm(1)  Modify for OSD Workbench	
		
	#ifdef AUDIO_NOISE_REDUCE
	AudioSourceSelect();		//Neil 20090218+
	#endif
	if(AudioDetect())			//Neil 20090421+ for audio 12v control 
		AudioPowerStatus = ON;

	if ((B_Channel == gmvb_MainChannel)&&(UserPrefSpeakerAudioSource == AUDIO_MAIN))
	{
	#ifdef Action_AdjustAVSDelayTime_WB_Used
		AdjustAVSDelayTime();
	#endif
		//SetAudioVolume(D_AUDIO_LOUDSPEAKER, UserPrefAudioVolume);
		//SetAudioBass(D_AUDIO_LOUDSPEAKER, UserPrefAudioBass);
		//SetAudioTreble(D_AUDIO_LOUDSPEAKER, UserPrefAudioTreble);
		//SetAudioBalance(D_AUDIO_LOUDSPEAKER, UserPrefAudioBalance);
	}
	else if ((B_Channel == gmvb_PipChannel)&&(UserPrefSpeakerAudioSource == AUDIO_PIP))
	{
		//SetAudioVolume(D_AUDIO_HEADPHONE, UserPrefAudioVolume);
		//SetAudioBass(D_AUDIO_HEADPHONE, UserPrefAudioBass);
		//SetAudioTreble(D_AUDIO_HEADPHONE, UserPrefAudioTreble);
		//SetAudioBalance(D_AUDIO_HEADPHONE, UserPrefAudioBalance);
	}

	//Neil 20090422 Move here, setup it anyway. otherwise will no audio if HDMI is PIP
	if((CurrentInput == ALL_BOARDS_HDMI)&& (stInput[B_Channel].ModeFlags & gmd_MODE_VALID))
		gm_SetRegBitsByte(HDRX_AU_CONV_CTRL, HDRX_AUDIO_CONV_EN);

	//Neil 20090211* move here for mute off after image appear, buglist 410-3;
	if(B_Channel == gmvb_MainChannel)
	{
		if(UserPrefSpeakerAudioSource == AUDIO_MAIN)
			AdjustSpeakerAudioSource(); 	//Neil 20090218+

		gm_TimerStart(AUDIO_WAIT_10ms_TMR, 30);
			
	#ifdef HdmiSoundPacketConfirm
		if ( UserPrefCurrentInputMain ==ALL_BOARDS_HDMI)		//StKam081108		Select basic audio modes processing during audio packet decoding, audio mode is programmed based on the AVI information packet decoding
		{
			if (gm_ReadRegWord(HDMI_PKT_STATUS)&(HDMI_AVI_INFO_AVLBL|HDMI_AUDIO_INFO))		//20090212 From Edward 
			{
				//gm_HdmiAudioMute(FALSE);		
		   		//AudioMuteFlag =  OFF;		//Neil 20090413+
			}
			else if(IsAudioOnCurrentPort(ALL_BOARDS_HDMI))
				SetAudioDACOutput(OFF);		//Neil 20090312+
		}

//&*&*&*S 20120315:SWRD4001: Kordonm(0)  Modify for OSD Workbench
		if ((UserPrefCurrentInputMain==ALL_BOARDS_DP1)||(UserPrefCurrentInputMain==ALL_BOARDS_DP3))	
//&*&*&*E 20120315 : Kordonm(1)  Modify for OSD Workbench
		{
			//if(IsAudioPacketPresent(ALL_BOARDS_DP1))
		   		//AudioMuteFlag =  OFF;		//Neil 20090413+
				
		}					   
	#endif
	}
	else
	{
		if(UserPrefSpeakerAudioSource == AUDIO_PIP)
			AdjustSpeakerAudioSource(); 	//Neil 20090218+
		gm_TimerStart(AUDIO_WAIT_10ms_TMR, 30);
			
//&*&*&*S 20120315:SWRD4001: Kordonm(0)  Modify for OSD Workbench			
		if (((UserPrefCurrentInputMain != ALL_BOARDS_DP1) && (UserPrefCurrentInputMain != ALL_BOARDS_DP3) && (UserPrefCurrentInputMain != ALL_BOARDS_HDMI))
			&&((UserPrefPipMode != NO_PIP) && ((UserPrefCurrentInputPip == ALL_BOARDS_DP1) || (UserPrefCurrentInputPip == ALL_BOARDS_DP3) || (UserPrefCurrentInputPip == ALL_BOARDS_HDMI))))
//&*&*&*E 20120315 : Kordonm(1)  Modify for OSD Workbench
		{
			if ( UserPrefCurrentInputPip == ALL_BOARDS_HDMI)		//StKam081108		Select basic audio modes processing during audio packet decoding, audio mode is programmed based on the AVI information packet decoding
			{
				if (gm_ReadRegWord(HDRX_PKT_STATUS) & (HDRX_AVI_INFO_AVLBL | HDRX_AUDIO_INFO_AVLBL))		//20090212 From Edward 
				{
					//gm_HdmiAudioMute(FALSE);
		   			//AudioMuteFlag =  OFF;		//Neil 20090413+
				}
				else if(IsAudioOnCurrentPort(ALL_BOARDS_HDMI))
					SetAudioDACOutput(OFF);		//Neil 20090312+
			}
//&*&*&*S 20120315:SWRD4001: Kordonm(0)  Modify for OSD Workbench
			if ((UserPrefCurrentInputPip==ALL_BOARDS_DP1)||(UserPrefCurrentInputPip==ALL_BOARDS_DP3))	
//&*&*&*E 20120315 : Kordonm(1)  Modify for OSD Workbench
			{
				//if(IsAudioPacketPresent(ALL_BOARDS_DP1))
		   			//AudioMuteFlag =  OFF;		//Neil 20090413+
			}
		}
	}
}

//Neil 20090217+ for audio pop noise when DC off
void PowerDownAudioSpeaker(void)
{
	//if(gm_ReadRegWord(GPOUTPUT5)&AUDIO_EN)
#if (BOARD_USED == DELL_93xx_BOARD)
	if(IsAUDIO_Detected())
	{
		//gm_Print(">>>>>>>>>>>>>>>>>>PowerDownAudio", 0);
		//gm_ClearRegBitsWord(GPOUTPUT5,AUDIO_EN); 
		AUDIO_POWER_DISABLE();
		gm_Delay10ms(40);		//Neil 20090217+ wait for DC drop avoid audio pop noise, at least > 250ms
	}
#endif	
}

#ifdef USE_TIMER_CTRL_AUD_PWR
void AudioSpeakerPowerCheck(void)		//for wake up from power saving audio pop noise
{
	static BYTE AudioNotDetCnt ;	//This count for led flashing once when plug in audio cable 

	if (AudioDetect())
	{
		//gm_Print("AudioSpeakerPowerCheck :PwrState = %d Audio Power up %d Timer = %d", PwrState,(gm_ReadRegWord(GPOUTPUT5) & AUDIO_EN),(gm_TimerCheck(AudioSpeakerPowerUp_100ms_TMR)));
		if((GetPowerState() == Pwr_On)||(GetPowerState() == Pwr_Sleep))
		{
			if((!(gm_ReadRegWord(AUDIO_POWER_PORT) & AUDIO_POWER_PIN)) && (gm_TimerCheck(AudioSpeakerPowerUp_100ms_TMR) != TIMER_OK))//modified by lion 20090423  ==  TIMER_TMO))	//Neil 20090309* for audio will power up again when enter power saving
			{
				if(AudioPowerStatus != OFF)
			{
					gm_Print("AudioSpeakerPowerCheck : Audio Power up %d", gm_ReadRegWord(AUDIO_POWER_PORT) & AUDIO_POWER_PIN);
					AUDIO_POWER_ENABLE();
					gm_TimerStop(AudioSpeakerPowerUp_100ms_TMR);
				}
			}
		}
		else if(GetPowerState() == Pwr_DeepSleep)
		{
			if((UserPrefPwrAudio == ON)&&(gm_ReadRegWord(AUDIO_POWER_PORT) & AUDIO_POWER_PIN))
			{
				PowerDownAudioSpeaker();
			}
		}
		AudioNotDetCnt = 0;
	}
	else		// sound bar not been detected
		{
		//if(gm_TimerCheck(AudioSpeakerPowerUp_100ms_TMR) !=  TIMER_OK)
		{	
			//gm_Print("AudioSpeakerPowerCheck :not detect Audio Power up %d", gm_ReadRegWord(GPOUTPUT5) & AUDIO_EN);
			//gm_TimerStart(AudioSpeakerPowerUp_100ms_TMR, 20);
			if(!AudioDetect())
				AudioNotDetCnt++;
			if(AudioNotDetCnt > 10)
				AUDIO_POWER_DISABLE();
		}
		}
}
#endif

#ifdef AUDIO_NOISE_REDUCE
//Neil 20090218+ for audio source select 
static BYTE B_PreAudioSource = 0xff ;	//Initial it to not main nor pip;
static BYTE B_PreAudioInput = gmd_AUDIO_IN_UNKNOWN;
void RestartAudioRoutine(void)
{
	B_PreAudioSource = NUM_AUDIO_SOURCES;
	B_PreAudioInput = gmd_AUDIO_IN_UNKNOWN;
}
void AudioSourceSelect(void)
{
	if(!Is_PIP_ON())		//No Pxp, just set to Main
   	{
	   if (((UserPrefCurrentInputMain == ALL_BOARDS_DP1)
	     || (UserPrefCurrentInputMain == ALL_BOARDS_DP3)
	     || (UserPrefCurrentInputMain == ALL_BOARDS_HDMI)) &&
	        /*IsAudioPacketPresent(UserPrefCurrentInputMain) &&*/		//Neil120915- ignore packet check when modesetup
	        IsInputSignalPresent(gmvb_MainChannel, UserPrefCurrentInputMain))
		{
			UserPrefSpeakerAudioSource = AUDIO_MAIN;
			UserPrefHeadphoneAudioSource = AUDIO_MAIN;

			if (((B_PreAudioSource != AUDIO_MAIN) || (B_PreAudioInput != GetAudioSource(gmvb_MainChannel)))
	       && (gm_IsValidSignal(gmvb_MainChannel)))
			{
				if(B_PreAudioSource != AUDIO_MAIN)
					B_AudioTimerCtrl |= Aud_Tmr_ChannelChange;
				if(B_PreAudioInput != gB_AudioInput)
					B_AudioTimerCtrl |= Aud_Tmr_InputChange;
				
				B_AudioTimerCtrl &= ~Aud_Tmr_AudSampleRateChange;	
				gm_Print("AudioSourceSelect : AUDIO_MAIN %d ", gB_AudioInput);

				AudioMuteFlag = ON;
				B_PreAudioSource = AUDIO_MAIN;
				B_PreAudioInput = GetAudioSource(gmvb_MainChannel);

				//gm_AUDIO_DisableVolume();
				//Audio_RedoDACGain();
			}
		}
   	}
   	else
   	{
	   //Main has audio packet, set to main
   if (((UserPrefCurrentInputMain == ALL_BOARDS_DP1)
     || (UserPrefCurrentInputMain == ALL_BOARDS_DP3)
     || (UserPrefCurrentInputMain == ALL_BOARDS_HDMI)) &&
        IsAudioPacketPresent(UserPrefCurrentInputMain) &&
        IsInputSignalPresent(gmvb_MainChannel, UserPrefCurrentInputMain))
	{
		UserPrefSpeakerAudioSource = AUDIO_MAIN;
		UserPrefHeadphoneAudioSource = AUDIO_MAIN;

		if (((B_PreAudioSource != AUDIO_MAIN) || (B_PreAudioInput != GetAudioSource(gmvb_MainChannel)))
       && (gm_IsValidSignal(gmvb_MainChannel)))
		{
			if(B_PreAudioSource != AUDIO_MAIN)
				B_AudioTimerCtrl |= Aud_Tmr_ChannelChange;
				if(B_PreAudioInput != gB_AudioInput)
				B_AudioTimerCtrl |= Aud_Tmr_InputChange;
			B_AudioTimerCtrl &= ~Aud_Tmr_AudSampleRateChange;	
			
			gm_Print("AudioSourceSelect : AUDIO_MAIN %d ", gB_AudioInput);

			AudioMuteFlag = ON;
				B_PreAudioSource = AUDIO_MAIN;
			B_PreAudioInput = GetAudioSource(gmvb_MainChannel);

				//gm_AUDIO_DisableVolume();
				//Audio_RedoDACGain();
		}
	}
	   else if(((UserPrefCurrentInputPip == ALL_BOARDS_DP1)	//Main no audio packet, set to Pxp
          || (UserPrefCurrentInputPip == ALL_BOARDS_DP3)
          || (UserPrefCurrentInputPip == ALL_BOARDS_HDMI)) &&
            IsAudioPacketPresent(UserPrefCurrentInputPip) &&
            IsInputSignalPresent(gmvb_PipChannel, UserPrefCurrentInputPip))
	{
		UserPrefSpeakerAudioSource = AUDIO_PIP;
		UserPrefHeadphoneAudioSource = AUDIO_PIP;

		if (((B_PreAudioSource != AUDIO_PIP) || (B_PreAudioInput != GetAudioSource(gmvb_PipChannel)))
       && (gm_IsValidSignal(gmvb_PipChannel)))
		{
			if(B_PreAudioSource != AUDIO_PIP)
				B_AudioTimerCtrl |= Aud_Tmr_ChannelChange;
				if(B_PreAudioInput != gB_AudioInput)
				B_AudioTimerCtrl |= Aud_Tmr_InputChange;

			B_AudioTimerCtrl &= ~Aud_Tmr_AudSampleRateChange;	
			gm_Print("AudioSourceSelect : AUDIO_PIP  ", gB_AudioInput);

			AudioMuteFlag = ON;							
				B_PreAudioSource = AUDIO_PIP;
			B_PreAudioInput = GetAudioSource(gmvb_PipChannel);

				//gm_AUDIO_DisableVolume();
				//Audio_RedoDACGain();
		}
	}
	   else if(((UserPrefCurrentInputMain == ALL_BOARDS_DP1)	//Main and Pxp no packet, set to main
	     || (UserPrefCurrentInputMain == ALL_BOARDS_DP3)
	     || (UserPrefCurrentInputMain == ALL_BOARDS_HDMI)) &&
	        /*IsAudioPacketPresent(UserPrefCurrentInputMain) &&	*/
	        IsInputSignalPresent(gmvb_MainChannel, UserPrefCurrentInputMain))
	{
			UserPrefSpeakerAudioSource = AUDIO_MAIN;
			UserPrefHeadphoneAudioSource = AUDIO_MAIN;

			if (((B_PreAudioSource != AUDIO_MAIN) || (B_PreAudioInput != GetAudioSource(gmvb_MainChannel)))
	       && (gm_IsValidSignal(gmvb_MainChannel)))
			{
				if(B_PreAudioSource != AUDIO_MAIN)
					B_AudioTimerCtrl |= Aud_Tmr_ChannelChange;
				if(B_PreAudioInput != gB_AudioInput)
					B_AudioTimerCtrl |= Aud_Tmr_InputChange;
				
				B_AudioTimerCtrl &= ~Aud_Tmr_AudSampleRateChange;	
				
				gm_Print("AudioSourceSelect : 2AUDIO_MAIN %d ", gB_AudioInput);

				AudioMuteFlag = ON;
				B_PreAudioSource = AUDIO_MAIN;
				B_PreAudioInput = GetAudioSource(gmvb_MainChannel);

				//gm_AUDIO_DisableVolume();
				//Audio_RedoDACGain();
			}
		}
	}

	B_AudioTimerCtrl |= Aud_Tmr_ModeChange;
}
#endif

BYTE GetAudioTimerValue(void)
{
	BYTE B_AudioTimerValue = Aud_SampleRateChange_Time;

	if(B_AudioTimerCtrl&(Aud_Tmr_ACOn|Aud_Tmr_DCOn))	//from AC on, need more time to setup, 500
	{
		if(B_AudioTimerValue < Aud_ACDC_Time)
		{
			B_AudioTimerValue = Aud_ACDC_Time;
			B_AudioTimerCtrl &= ~(Aud_Tmr_ACOn|Aud_Tmr_DCOn);
		}
	}
	if(B_AudioTimerCtrl&Aud_Tmr_ModeChange)//mode change only, 100ms
	{
		if(B_AudioTimerValue < Aud_ModeChange_Time)
		{
			B_AudioTimerValue = Aud_ModeChange_Time;
			B_AudioTimerCtrl &= ~Aud_Tmr_ModeChange;
		}
	}
	if(B_AudioTimerCtrl&Aud_Tmr_ChannelChange)//mode change only, 100ms
	{
		if(B_AudioTimerValue < Aud_ChannelChange_Time)
		{
			B_AudioTimerValue = Aud_ChannelChange_Time;
			B_AudioTimerCtrl &= ~Aud_Tmr_ChannelChange;
		}
	}
	if(B_AudioTimerCtrl&Aud_Tmr_InputChange)//mode change only, 100ms
	{
		if(B_AudioTimerValue < Aud_Portchange_Time)
		{
			B_AudioTimerValue = Aud_Portchange_Time;
			B_AudioTimerCtrl &= ~Aud_Tmr_InputChange;
		}
	}
	if((B_AudioTimerValue <= MainDisplayEnableDelay)
		&&(IsCurChannelDisplayOFF((UserPrefSpeakerAudioSource == AUDIO_PIP)? CH_B : CH_A)))		//audio output later than display output
		B_AudioTimerValue = MainDisplayEnableDelay + 5;
	
	//gm_Printf(">>>>0x%x>>>>>>>B_AudioTimerValue = %d<<<<<<<<<<%d<<<<<<<<<<", B_AudioTimerCtrl,B_AudioTimerValue, gm_GetSystemTime());
	return B_AudioTimerValue;
}

#ifdef DP_MUTE_FLAG_RECHECK //KevinL 20100309
BOOL IsDPAudioPacketMuted(gmt_PHY_CH B_Channel) //KevinL 20100224 on M1F192A for BITS Item DF362429 no Audio issue
{
	if(IsDPInput(B_Channel))
	{
		if(gm_ReadRegByte(DPRX_SDP_AUD_STS_0) & (DPRX_SDP_AUD_MUTE_STS|DPRX_SDP_AUD_MUTE_GLBL_STS))	
         return TRUE;
      else
         return FALSE;
	}
	return FALSE;
}
#endif

#ifdef AUDIO_NOISE_REDUCE
void AudioSourceAutoSelect(void)
{
	if((IsDPInput(CH_A)|| IsMainHDMIInput()) ||
	((Is_PIP_ON() && (IsDPInput(CH_B)|| IsPIPHDMIInput()))))
	{
		static BYTE B_AudioNoPKTCNT = 0;
		#ifdef AUDIO_DP_M_CHECK
		BYTE B_AudioStable = gmd_FALSE;
		#endif
		if(Is_PIP_ON())
		{
			//if(!gm_IsValidSignal(gmvb_MainChannel)&&!gm_IsValidSignal(gmvb_PipChannel))
			//	return;
			
				if(!IsAudioPacketPresent(UserPrefCurrentInputMain)&&!IsAudioPacketPresent(UserPrefCurrentInputPip))//All ports don't have packet
				{
				if(B_AudioNoPKTCNT < 10)
					B_AudioNoPKTCNT++;
					if(B_AudioNoPKTCNT >= 3)
					AUDIO_MUTE();
				}
				if(IsAudioPacketPresent(UserPrefCurrentInputMain)||IsAudioPacketPresent(UserPrefCurrentInputPip))//one of them have packet
				{
				//From no packet to have packet, need largest time to setup whole process
					if((B_AudioNoPKTCNT != 0)&&(gm_TimerCheck(AUDIO_WAIT_10ms_TMR) != TIMER_OK))
					{
						//AudioUpdateNow = gmd_FALSE;
						B_AudioTimerCtrl |= Aud_Tmr_ModeChange;
					gm_TimerStart(AUDIO_WAIT_10ms_TMR, Aud_ModeChange_Time);		
					}
					B_AudioNoPKTCNT = 0;
				}

			if(gm_IsModeComplete(gmvb_MainChannel)||gm_IsModeComplete(gmvb_PipChannel))
			{
				static BYTE B_AudioStableCNT = 0;
				if(IsAudioPacketPresent(UserPrefCurrentInputMain)&&(UserPrefSpeakerAudioSource == AUDIO_PIP)
					&&(!gm_IsModeOOR(gmvb_MainChannel)))
					{	
						if((B_AudioStableCNT == 3)&&(gm_TimerCheck(AUDIO_WAIT_10ms_TMR) != TIMER_OK))
						{
							gm_Print("00 IsAudioPacketPresent %d", IsAudioPacketPresent(UserPrefCurrentInputMain));
							//AudioUpdateNow = gmd_FALSE;
							AUDIO_MUTE();
							B_AudioTimerCtrl |= Aud_Tmr_ChannelChange;
							gm_TimerStart(AUDIO_WAIT_10ms_TMR, Aud_ChannelChange_Time);		
						}
						if(B_AudioStableCNT < 6)
							B_AudioStableCNT++;
					}
					else if(!IsAudioPacketPresent(UserPrefCurrentInputMain)&&IsAudioPacketPresent(UserPrefCurrentInputPip)
						&&(UserPrefSpeakerAudioSource == AUDIO_MAIN)&&(!gm_IsModeOOR(gmvb_PipChannel)))
					{
						if((B_AudioStableCNT == 3)&&(gm_TimerCheck(AUDIO_WAIT_10ms_TMR) != TIMER_OK))
						{	
							gm_Print("01 IsAudioPacketPresent %d", IsAudioPacketPresent(UserPrefCurrentInputMain));
							//AudioUpdateNow = gmd_FALSE;
							AUDIO_MUTE();
							B_AudioTimerCtrl |= Aud_Tmr_ChannelChange;
							gm_TimerStart(AUDIO_WAIT_10ms_TMR, Aud_ChannelChange_Time);		
						}
						if(B_AudioStableCNT < 6)
							B_AudioStableCNT++;
					}
					else if(IsAudioPacketPresent(UserPrefCurrentInputMain)&&IsAudioPacketPresent(UserPrefCurrentInputPip)
						&&(UserPrefSpeakerAudioSource == AUDIO_MAIN)&&(gm_IsModeOOR(gmvb_MainChannel)))
					{	
						if((B_AudioStableCNT == 3)&&(gm_TimerCheck(AUDIO_WAIT_10ms_TMR) != TIMER_OK))
						{
							gm_Print("02 IsAudioPacketPresent %d", IsAudioPacketPresent(UserPrefCurrentInputMain));
							//AudioUpdateNow = gmd_FALSE;
							B_AudioTimerCtrl |= Aud_Tmr_ChannelChange;
							gm_TimerStart(AUDIO_WAIT_10ms_TMR, Aud_ChannelChange_Time);		
						}
						if(B_AudioStableCNT < 6)
							B_AudioStableCNT++;
					}
					else
						B_AudioStableCNT = 0;
				}
		}
		else
		{
			//if(!gm_IsValidSignal(gmvb_MainChannel))
			//	return;
			if(gm_IsModeComplete(gmvb_MainChannel))
			{
				if(!IsAudioPacketPresent(UserPrefCurrentInputMain))
				{
					if(B_AudioNoPKTCNT < 10)
					B_AudioNoPKTCNT++;
					//if(B_AudioNoPKTCNT >= 3)
					AUDIO_MUTE();
				}
				if(IsAudioPacketPresent(UserPrefCurrentInputMain))
				{
					//From no packet to have packet, need largest time to setup whole process
					if((B_AudioNoPKTCNT != 0)&&(gm_TimerCheck(AUDIO_WAIT_10ms_TMR) != TIMER_OK))
					{
						//AudioUpdateNow = gmd_FALSE;
						gm_Print("Main IsAudioPacketPresent %d", IsAudioPacketPresent(UserPrefCurrentInputMain));
						B_AudioTimerCtrl |= Aud_Tmr_ModeChange;
						gm_TimerStart(AUDIO_WAIT_10ms_TMR, Aud_ModeChange_Time);	//
					}
					B_AudioNoPKTCNT = 0;
				}
			}
		}
			
	if((gm_TimerCheck(AUDIO_WAIT_10ms_TMR) == TIMER_OK)&&(AudioUpdateNow != gmd_TRUE))
		return;

	AudioUpdateNow = gmd_FALSE;
	
	if((IsDPInput(gmvb_MainChannel)||(IsMainHDMIInput()))&&(!gm_IsModeOOR(gmvb_MainChannel))
		&&IsInputSignalPresent(gmvb_MainChannel, UserPrefCurrentInputMain)&&IsAudioPacketPresent(UserPrefCurrentInputMain))
	{
		UserPrefSpeakerAudioSource = AUDIO_MAIN;
		UserPrefHeadphoneAudioSource = AUDIO_MAIN;

		//if(gm_IsValidSignal(gmvb_MainChannel))
		{
			if((B_PreAudioSource != AUDIO_MAIN)||(B_PreAudioInput != GetAudioSource(gmvb_MainChannel)))
			{
				gm_Print("AudioSourceAutoSelect : AUDIO_MAIN %d ", GetAudioSource(gmvb_MainChannel));
				AUDIO_MUTE();
					//AdjustSpeakerAudioSource();
				B_PreAudioSource = AUDIO_MAIN;
				B_PreAudioInput = GetAudioSource(gmvb_MainChannel);
					AudioMuteFlag = ON;
				B_AudioTimerCtrl &= ~Aud_Tmr_AudSampleRateChange;	
                                     
					//gm_AUDIO_ReStartDetection();
				//Audio_RedoDACGain();				
			   	gm_TimerStart(AUDIO_WAIT_10ms_TMR, Aud_ChannelChange_Time);		//Neil120720 Need this when change audio channel from PIP to main
			}
			else if((B_PreAudioSource == AUDIO_MAIN)&&(B_PreAudioInput == GetAudioSource(gmvb_MainChannel)))
			{
				//if(gm_AUDIO_Is_VolumeEnabled()&&(AudioMuteFlag == ON))
				//	AudioMuteFlag = OFF;
			}
		}
		///else
		//	SetAudioDACOutput(OFF);
		if(gm_IsModeComplete(gmvb_MainChannel))
		{
				//gm_Print("Set AUDIO Main: %d", IsInputSignalPresent(gmvb_MainChannel, UserPrefCurrentInputMain));
			if(IS_AUDIO_MUTE()&&(AudioMuteFlag == OFF)&&(gm_TimerCheck(AUDIO_WAIT_10ms_TMR) != TIMER_OK))//Neil120721* Add timer check, don't unmute when timer active
			{	
				if(IsDPInput(gmvb_MainChannel))
				{
					//Neil120730+ for DP DPRX_SDP_AUD_MUTE_AUTO_EN enable but DPRX_SDP_AUD_MUTE_SOFT will be set not accroding to 
					//VBID mute flag, it will cause audio mute when switch between DP and MiniDP
					if((!(GetDPVBIDStatus()&VBID_AudioMute_Flag))&&(gm_ReadRegWord(DP12RX0_SDP_AUD_CTRL)&DPRX_SDP_AUD_MUTE_SOFT))
						gm_ClearRegBitsWord(DP12RX0_SDP_AUD_CTRL, DPRX_SDP_AUD_MUTE_SOFT);		//Clear soft mute if no VBID mute flag 
				}
				#ifdef AUDIO_DP_M_CHECK
				if(B_AudioStable)
				#endif
				{
					if(IsMainHDMIInput()&&GetHDMIAudioPCMWarningMessage())//121002 Owen mute for unsupport audio format
						AUDIO_MUTE();
					else
						AUDIO_UNMUTE();
					
					gm_TimerStop(AUDIO_WAIT_10ms_TMR);	
				}
			}
			}
		else
			AUDIO_MUTE();
			
	}	
	else if((Is_PIP_ON())&&(IsDPInput(gmvb_PipChannel)||IsPIPHDMIInput())&&(!gm_IsModeOOR(gmvb_PipChannel))
		&&IsInputSignalPresent(gmvb_PipChannel, UserPrefCurrentInputPip)&&IsAudioPacketPresent(UserPrefCurrentInputPip))
	{
		UserPrefSpeakerAudioSource = AUDIO_PIP;
		UserPrefHeadphoneAudioSource = AUDIO_PIP;
			//gm_Print("================================Set AUDIO PIP: %d", IsInputSignalPresent(gmvb_PipChannel, UserPrefCurrentInputPip));
		
		//if(gm_IsValidSignal(gmvb_PipChannel))
		{
			if((B_PreAudioSource != AUDIO_PIP)||(B_PreAudioInput != GetAudioSource(gmvb_PipChannel)))
			{
				gm_Print("AudioSourceAutoSelect : AUDIO_PIP %d ", GetAudioSource(gmvb_PipChannel));
				AUDIO_MUTE();
				//AdjustSpeakerAudioSource();
				B_PreAudioSource = AUDIO_PIP;
				B_PreAudioInput = GetAudioSource(gmvb_PipChannel);
					AudioMuteFlag = ON;						
				B_AudioTimerCtrl &= ~Aud_Tmr_AudSampleRateChange;	

					//gm_AUDIO_ReStartDetection();
				//Audio_RedoDACGain();				
			    gm_TimerStart(AUDIO_WAIT_10ms_TMR, Aud_ChannelChange_Time);	//Neil120720 Need this when change audio channel from Main to Pip
			}
			else if((B_PreAudioSource == AUDIO_PIP)&&(B_PreAudioInput == GetAudioSource(gmvb_PipChannel)))
			{
				//if(gm_AUDIO_Is_VolumeEnabled()&&(AudioMuteFlag == ON))
				//	AudioMuteFlag = OFF;
			}
		}
		//else
		//	SetAudioDACOutput(OFF);

		if(gm_IsModeComplete(gmvb_PipChannel))
		{
				//gm_Print("Set AUDIO PIP: %d", IsInputSignalPresent(gmvb_PipChannel, UserPrefCurrentInputPip));
			if(IS_AUDIO_MUTE()&&(AudioMuteFlag == OFF)&&(gm_TimerCheck(AUDIO_WAIT_10ms_TMR) != TIMER_OK))//Neil120721* Add timer check, don't unmute when timer active
			{	
				if(IsDPInput(gmvb_PipChannel))
				{
					//Neil120730+ for DP DPRX_SDP_AUD_MUTE_AUTO_EN enable but DPRX_SDP_AUD_MUTE_SOFT will be set not accroding to 
					//VBID mute flag, it will cause audio mute when switch between DP and MiniDP
					if((!(GetDPVBIDStatus()&VBID_AudioMute_Flag))&&(gm_ReadRegWord(DP12RX0_SDP_AUD_CTRL)&DPRX_SDP_AUD_MUTE_SOFT))
						gm_ClearRegBitsWord(DP12RX0_SDP_AUD_CTRL, DPRX_SDP_AUD_MUTE_SOFT);		//Clear soft mute if no VBID mute flag 

				}	
				#ifdef AUDIO_DP_M_CHECK
				if(B_AudioStable)
				#endif
				{
					if(IsMainHDMIInput()&&GetHDMIAudioPCMWarningMessage())//121002 Owen mute for unsupport audio format
						AUDIO_MUTE();
					else
						AUDIO_UNMUTE();
					
					gm_TimerStop(AUDIO_WAIT_10ms_TMR);	
				}
			}	
		}
		else
			AUDIO_MUTE();
		}
		else
			AUDIO_MUTE();
	}
	else
	{
		//if change none audio port, reset variable and set time to setup whole audio process(DAC)
		//since audio setup first than mode;
		RestartAudioRoutine();
		B_AudioTimerCtrl &= ~Aud_Tmr_AudSampleRateChange;	
		B_AudioTimerCtrl |= (Aud_Tmr_ModeChange|Aud_Tmr_ChannelChange|Aud_Tmr_InputChange);
		AUDIO_MUTE();
}
}
#endif

void SetAudioDACOutput(BYTE OnOrOff)
{
	//Neil120718* enable it for audio pop noise
	if (OnOrOff == ON)		//Neil120721* On is unmute
	{
		if(IS_AUDIO_MUTE())
		{
			ToggleAudioDACAttn();			
			#ifdef UART_DEBUG_MODE
			gm_Print("ON %d", gm_GetSystemTime());
			#endif
			//Audio_AdjustVolume(3);		//for pop noise when change mode if PC mute
			#if((BOARD_USED == BOARD_93xx_CJ2))
			HEADPHONE_UNMUTE();
			#endif

			//gm_ClearRegBitsWord(AUDIO_DAC23_CTRL_0, (AUDIO_DAC2_LEFT_MUTE | AUDIO_DAC3_RIGHT_MUTE));
			//Audio_AdjustVolume(DEFAULT_VOLUME_VALUE);		//for pop noise when change mode if PC mute
			#if(FEATURE_TDA7491 == ENABLE)
			TDA7491_ENABLE();
			#endif
			
			gm_Delay10ms(10);
			gm_SetRegBitsWord(AUDIO_MUTE_PORT, AUDIO_MUTE_PIN);
			gm_ClearRegBitsWord(AUDIO_DAC_CTRL2, MUTE_DAC_EN);		
			B_AudioTimerCtrl = Aud_Tmr_None;
			// for library setup DAC use.
			B_AudioTimerCtrl |= Aud_Tmr_AudSampleRateChange;
		}
	}
	else
	{
		if(!IS_AUDIO_MUTE())		//Off is mute
		{
			#if(FEATURE_TDA7491 == ENABLE)
			TDA7491_DISABLE();
			#endif
			#if((BOARD_USED == BOARD_93xx_CJ2))
			HEADPHONE_MUTE();
			#endif
			#ifdef UART_DEBUG_MODE
			gm_Print("OFF %d", gm_GetSystemTime());
			#endif
			//Audio_AdjustVolume(0);		//for pop noise when change mode if PC mute
			gm_ClearRegBitsWord(AUDIO_MUTE_PORT, AUDIO_MUTE_PIN);
			gm_SetRegBitsWord(AUDIO_DAC_CTRL2, MUTE_DAC_EN);
        	 //AUDIO_MUTE();
			//gm_SetRegBitsWord(AUDIO_DAC23_CTRL_0, (AUDIO_DAC2_LEFT_MUTE | AUDIO_DAC3_RIGHT_MUTE));
		}
	}
}

//Neil 20090416+ for DP&HDMI signal detect
BYTE IsInputSignalPresent(gmt_PHY_CH B_Channel,ALL_BOARDS_INPUT_PORT B_CurrentPort)
{
	BYTE B_SignalDetect = gmd_FALSE;
	WORD W_InputStatusReg = ((B_Channel == CH_A) ? MAIN_INPUT_STATUS : PIP_INPUT_STATUS);
	
	if(B_CurrentPort == ALL_BOARDS_HDMI)
		B_SignalDetect = (gm_ReadRegWord(HDRX_MAIN_LINK_STATUS)&HDRX_HDMI_SIGN_DETECTED);	//(gm_ReadRegWord(HDMI_STATUS) & HDMI_SGNL_DTCT)
//&*&*&*S 20120315:SWRD4001: Kordonm(0)  Modify for OSD Workbench
	else if((B_CurrentPort == ALL_BOARDS_DP1)||(B_CurrentPort == ALL_BOARDS_DP3))
//&*&*&*E 20120315 : Kordonm(1)  Modify for OSD Workbench
		B_SignalDetect = !((gm_ReadRegWord(W_InputStatusReg)) & ((IMP_NO_HS|IMP_NO_VS)|(IMP_HS_PERIOD_ERR|IMP_VS_PERIOD_ERR)));//(IMP_SYNC_STATUS_MASK | IPP_TIMING_STATUS_MASK));

	return B_SignalDetect;
}

//Neil 20090416+ for DP&HDMI audio packets detect
BYTE IsAudioPacketPresent(ALL_BOARDS_INPUT_PORT B_CurrentPort)
{
	BYTE B_PKTStatus = gmd_FALSE;
	//WORD W_InputStatusReg = ((B_Channel == CH_A) ? MAIN_INPUT_STATUS : PIP_INPUT_STATUS);
	
	if(B_CurrentPort == ALL_BOARDS_HDMI)
		B_PKTStatus = (gm_ReadRegWord(HDRX_PKT_STATUS)&(HDRX_AVI_INFO_AVLBL|HDRX_AUDIO_INFO_AVLBL));	//(gm_ReadRegWord(HDMI_STATUS) & HDMI_SGNL_DTCT)
	else if((B_CurrentPort == ALL_BOARDS_DP1)||(B_CurrentPort == ALL_BOARDS_DP3))
		B_PKTStatus = (gm_ReadRegWord(DP12RX0_SDP_AUD_STS) & DPRX_SDP_AUD_PKT_PRSNT);//(IMP_SYNC_STATUS_MASK | IPP_TIMING_STATUS_MASK));
	
	return B_PKTStatus;
}

BYTE IsAudioOnCurrentPort(ALL_BOARDS_INPUT_PORT B_CurrentPort)	//Neil 20090514* add sync present check, DP audio PKT preset after wake from power saving
{
	#if 1
	BYTE B_CurInput = (UserPrefAudioMainInput == gmd_AUDIO_SRC_AUTO_Pxp)? UserPrefCurrentInputPip : UserPrefCurrentInputMain;

	return (B_CurInput == B_CurrentPort);
	#else
	BYTE B_IsAudioPort = gmd_FALSE;
	if(B_CurrentPort == ALL_BOARDS_HDMI)
	{
		if (((UserPrefCurrentInputMain == ALL_BOARDS_HDMI)&&IsAudioPacketPresent(ALL_BOARDS_HDMI)&&IsInputSignalPresent(CH_A, ALL_BOARDS_HDMI))
			||((UserPrefCurrentInputPip == ALL_BOARDS_HDMI)&&IsInputSignalPresent(CH_B, ALL_BOARDS_HDMI)
			&&((UserPrefCurrentInputMain != ALL_BOARDS_DP1)||(UserPrefCurrentInputMain == ALL_BOARDS_DP1)
			&&((!IsAudioPacketPresent(ALL_BOARDS_DP1))||(!IsInputSignalPresent(CH_A, ALL_BOARDS_DP1))))
			&&((UserPrefCurrentInputMain != ALL_BOARDS_DP3)||(UserPrefCurrentInputMain == ALL_BOARDS_DP3)
			&&((!IsAudioPacketPresent(ALL_BOARDS_DP3))||(!IsInputSignalPresent(CH_A, ALL_BOARDS_DP1))))))
			B_IsAudioPort = TRUE;
	}
	else if(B_CurrentPort == ALL_BOARDS_DP1)
	{
		if (((UserPrefCurrentInputMain == ALL_BOARDS_DP1)&&IsAudioPacketPresent(ALL_BOARDS_DP1)&&IsInputSignalPresent(CH_A, ALL_BOARDS_DP1))
			||((UserPrefCurrentInputPip == ALL_BOARDS_DP1)&&IsInputSignalPresent(CH_B, ALL_BOARDS_DP1)
			&&((UserPrefCurrentInputMain != ALL_BOARDS_HDMI)||(UserPrefCurrentInputMain == ALL_BOARDS_HDMI)
			&&((!IsAudioPacketPresent(ALL_BOARDS_HDMI))||(!IsInputSignalPresent(CH_A, ALL_BOARDS_HDMI))))))
			B_IsAudioPort = TRUE;
	}
	else if(B_CurrentPort == ALL_BOARDS_DP3)
	{
		if (((UserPrefCurrentInputMain == ALL_BOARDS_DP3)&&IsAudioPacketPresent(ALL_BOARDS_DP3)&&IsInputSignalPresent(CH_A, ALL_BOARDS_DP3))
			||((UserPrefCurrentInputPip == ALL_BOARDS_DP3)&&IsInputSignalPresent(CH_B, ALL_BOARDS_DP3)
			&&((UserPrefCurrentInputMain != ALL_BOARDS_HDMI)||(UserPrefCurrentInputMain == ALL_BOARDS_HDMI)
			&&((!IsAudioPacketPresent(ALL_BOARDS_HDMI))||(!IsInputSignalPresent(CH_A, ALL_BOARDS_HDMI))))))
			B_IsAudioPort = TRUE;
	}

	return B_IsAudioPort;
	#endif
}

//Neil 20090303*
WORD GetDPHSartFromMS(void)
{
	return (WORD)(gm_ReadRegWord(DP12RX0_MS_HACT_START) -(gm_ReadRegWord(DP12RX0_MS_HS_WIDTH)&0xFF) -4);
}

WORD GetDPVSartFromMS(void)
{
	return (WORD)(gm_ReadRegWord(DP12RX0_MS_VACT_START) - (gm_ReadRegWord(DP12RX0_MS_VS_WIDTH)&0xFF) -1);
}

BOOL IsPowerInDeepSleep(void) //Kevinl 20090505
{
	return (PwrState == Pwr_DeepSleep);	
}
BOOL IsPreLutEnable(void) //KevinL 20090304
{
	if (IsInputColorFormat_RGB())
	{
	#if 1//def RGB_xvMode_Patch //KevinL 20090609 for RGB xvMode	
		//if (IsPresetMode_GRAPHICS_ADOBE_RGB || IsPresetMode_GRAPHICS_SRGB || IsPresetMode_GRAPHICS_CUSTOM_COLOR)	
		if(IsPresetMode_GRAPHICS_COLOR_SPACE || IsPresetMode_GRAPHICS_CUSTOM_COLOR)
	#else
		if (IsPresetMode_GRAPHICS_ADOBE_RGB || IsPresetMode_GRAPHICS_SRGB || IsPresetMode_GRAPHICS_CUSTOM_COLOR || IsPresetMode_VIDEO_xvMode)
	#endif		
			return TRUE;
		else
			return FALSE;			
	}
	else
	{
		if (IsPresetMode_GRAPHICS_CUSTOM_COLOR)
			return TRUE;
		else 
			return FALSE;
	}
}

BOOL IsPostLutEnable(void) //KevinL 20090312
{
	if (UserPrefGammaMode == Gamma_MAC)
		return TRUE;

	if (IsInputColorFormat_RGB())
	{
		//if (IsPresetMode_GRAPHICS_MULTIMEDIA || IsPresetMode_GRAPHICS_GAME || IsPresetMode_GRAPHICS_ADOBE_RGB || 
			//IsPresetMode_GRAPHICS_SRGB || IsPresetMode_GRAPHICS_CUSTOM_COLOR || IsColorMode_VODEO)
		if (IsPresetMode_GRAPHICS_MULTIMEDIA || IsPresetMode_GRAPHICS_GAME || IsPresetMode_GRAPHICS_COLOR_SPACE 
			|| IsPresetMode_GRAPHICS_CUSTOM_COLOR || IsColorMode_VODEO)	
			return TRUE;
		else
			return FALSE;			
	}
	else
	{
		//if (IsPresetMode_GRAPHICS_STANDARD ||IsPresetMode_GRAPHICS_TEXT || IsPresetMode_GRAPHICS_WARM || IsPresetMode_GRAPHICS_COOL)
		if (IsPresetMode_GRAPHICS_STANDARD ||IsPresetMode_GRAPHICS_TEXT || IsPresetMode_GRAPHICS_COLOR_TEMP)
			return FALSE;
		else 
			return TRUE;
	}
}

BOOL IsHDMIInfoFrameExist(void)           //calo updat for the factory menu 090404
{
	if (gm_ReadRegByte(HDRX_MAIN_LINK_STATUS) & HDRX_HDMI_SIGN_DETECTED) //HDMI infoframe exist 
		return TRUE;
	else 
		return FALSE;
}

BYTE  ColorSpaceJudgement(void)        //calo updat for the factory menu 090404
{
	BYTE TempColorValue;

	if (UserPrefCurrentInputMain == ALL_BOARDS_HDMI)
	{
		TempColorValue = ((gm_ReadRegByte(HDRX_AVI_AUTO_FIELDS) & HDRX_AVI_YUV_CTRL) >> 4) ;

		if (TempColorValue == RGBColorSpace)
			return RGBColorSpace;
		else if (TempColorValue == YUV_422)
			return YUV_422;
		else if (TempColorValue == YUV_444)
			return YUV_444;
		else 
			return UnKnown;
	}
//&*&*&*S 20120315:SWRD4001: Kordonm(0)  Modify for OSD Workbench
	else if((UserPrefCurrentInputMain == ALL_BOARDS_DP1)||(UserPrefCurrentInputMain == ALL_BOARDS_DP3))
//&*&*&*E 20120315 : Kordonm(1)  Modify for OSD Workbench
	{
		TempColorValue = ((gm_ReadRegByte(DP12RX0_MS_FORMAT) & DPRX_MS_COLOR_SPACE) >> 1);

		if (TempColorValue == RGBColorSpace)
			return RGBColorSpace;
		else if (TempColorValue == YUV_422)
			return YUV_422;
		else if (TempColorValue == YUV_444)
			return YUV_444;
		else 
			return UnKnown;
	}
	else 
		return UnKnown;
}

BOOL IsHDMIAudioPacketExist(void)
{
	if (gm_ReadRegWord(HDRX_PKT_STATUS) & HDRX_AUDIO_INFO_AVLBL) //HDMI audio packe preset
		return TRUE;
	else 
		return FALSE;
}

BOOL IsDPAudioExist(void)
{
	BYTE AudioSampleValue;
	//AudioSampleValue = GetDpRxAudioSampleRate();

	if (AudioSampleValue != 0)
		return TRUE;
	else 
		return FALSE;
}

void CheckACCUpdateStatusWhenOSDShow(void) //KevinL 20090511 updated
{
	static BYTE ACCStatus = 0;

	//if(IsOSDMainMenu()&&)
	if((((gm_ReadRegWord(PB_MAIN_VHEIGHT)) < PanelHeight)&&(stInput[gmvb_MainChannel].ModeFlags & gmd_MODE_COMPLETE))
	 &&((stInput[gmvb_MainChannel].ModeFlags & gmd_MODE_COMPLETE)&&(!ISZeroKeyState())))
	{
		if (gm_ReadRegWord(MISC_OCMMASK) & ACC_UPDATE_MASK)
		{
			ACCStatus = gm_ReadRegWord(MISC_OCMMASK)&ACC_UPDATE_MASK;
			gm_ClearRegBitsWord(MISC_OCMMASK, ACC_UPDATE_MASK);
		}
	}
	else if (ACCStatus)
	{
		gm_SetRegBitsWord(MISC_OCMMASK, ACCStatus);
		ACCStatus = 0;
	}
}

BYTE GraphicModeEnhGain[4][3] = 
{
	//MHF_ENH_Y_FINALSGAIN		MHF_ENH_Y_FINALLGAIN	MHF_ENH_Y_FINALGAIN	
	{0x00, 						0x00,					0x00},

	//MHF_ENH_UV_FINALSGAIN	MHF_ENH_UV_FINALLGAIN	MHF_ENH_UV_FINALGAIN	
	{0x00, 						0x00,					0x00},

	//MVF_ENH_Y_FINALSGAIN		MVF_ENH_Y_FINALLGAIN	MVF_ENH_Y_FINALGAIN
	{0x00, 						0x00,					0x00},
		
	//MVF_ENH_UV_FINALSGAIN	MVF_ENH_UV_FINALLGAIN	MVF_ENH_UV_FINALGAIN	
	{0x00, 						0x00,					0x00},
};

BYTE VideoModeEnhGain[4][3] = 
{
	//MHF_ENH_Y_FINALSGAIN		MHF_ENH_Y_FINALLGAIN	MHF_ENH_Y_FINALGAIN	
	{0x40, 						0x20,					0x40},

	//MHF_ENH_UV_FINALSGAIN	MHF_ENH_UV_FINALLGAIN	MHF_ENH_UV_FINALGAIN	
	{0x40, 						0x40,					0x80},

	//MVF_ENH_Y_FINALSGAIN		MVF_ENH_Y_FINALLGAIN	MVF_ENH_Y_FINALGAIN
	{0x40, 						0x20,					0x20},
		
	//MVF_ENH_UV_FINALSGAIN	MVF_ENH_UV_FINALLGAIN	MVF_ENH_UV_FINALGAIN	
	{0x40, 						0x40,					0x80},
};

//Neil 20090317+ for ENH still working even ENH_ENABLE not set to enable
void AdjustEnhancer(gmt_PHY_CH B_Channel)
{
	//WORD RegBass = MHF_ENH_Y_FINALSGAIN;
	BYTE i;
	if (gm_IsVideo(gmvb_MainChannel))
	{
		for (i = 0; i < 3; i++)
		{
			gm_WriteRegWord(MHF_ENH_Y_FINALSGAIN +i*2, VideoModeEnhGain[0][i]);
			gm_WriteRegWord(MVF_ENH_Y_FINALSGAIN +i*2, VideoModeEnhGain[2][i]);
		}
	}
	else
	{
		for (i = 0; i < 3; i++)
		{
			gm_WriteRegWord(MHF_ENH_Y_FINALSGAIN +i*2, GraphicModeEnhGain[0][i]);
			gm_WriteRegWord(MVF_ENH_Y_FINALSGAIN +i*2, GraphicModeEnhGain[2][i]);
		}
	}
	gm_ForceUpdate(B_Channel, INPUT_OUTPUT_CLOCK);
}

BOOL CheckUserPrefSettingRange(void)	// Increased by ST_hansPENG 090319
{
	BOOL BL_Result = TRUE, BL_ResultRegion = TRUE;

	//
	// PortSchemeDependentUserPref
	//
	{
		BYTE B_ImageSchemeCount,B_PresetModeCount;
		PortSchemeDependentUserPrefType  CheckPortSchemeDependentUserPreferences;

		for (B_ImageSchemeCount = 0; B_ImageSchemeCount < NUM_IMAGE_SCHEMES; B_ImageSchemeCount++)
		{
			WORD W_AddressPSD;

			//
			// PresetModeUserPreferences
			//
			// Preset Mode
			{
				WORD W_AddressPreMode;
				PresetModesType CheckPresetModeUserPreferences;
				
				ReadPresetModesDependentEntry(B_ImageSchemeCount);
				// compute the start address
				W_AddressPreMode = PresetModeSettingStart + (WORD)B_ImageSchemeCount * (PresetModesUserPrefSize + ChecksumSize);
				
				// read the PortDependentUserPreferences record and save it
				NVRam_ReadBuffer(W_AddressPreMode, (BYTE *)(&CheckPresetModeUserPreferences), PresetModesUserPrefSize);
			
				// Video Mode Select
				// Movie - Hue
				if (IsUserPrefOutOfRange(CheckPresetModeUserPreferences.VideoPresetMode.Movie.Hue, ADJV_Hue))
				{
					CheckPresetModeUserPreferences.VideoPresetMode.Movie.Hue = SchemeFactoryDefaultInROM[B_ImageSchemeCount].Hue;
					BL_ResultRegion = FALSE;
				}
				// Movie - Saturation
				if (IsUserPrefOutOfRange(CheckPresetModeUserPreferences.VideoPresetMode.Movie.Saturation, ADJV_Saturation))
				{
					CheckPresetModeUserPreferences.VideoPresetMode.Movie.Saturation = SchemeFactoryDefaultInROM[B_ImageSchemeCount].Saturation;
					BL_ResultRegion = FALSE;
				}
				// Game - Hue
				if (IsUserPrefOutOfRange(CheckPresetModeUserPreferences.VideoPresetMode.Game.Hue, ADJV_Hue))
				{
					CheckPresetModeUserPreferences.VideoPresetMode.Game.Hue = SchemeFactoryDefaultInROM[B_ImageSchemeCount].Hue;
					BL_ResultRegion = FALSE;
				}
				// Game - Saturation
				if (IsUserPrefOutOfRange(CheckPresetModeUserPreferences.VideoPresetMode.Game.Saturation, ADJV_Saturation))
				{
					CheckPresetModeUserPreferences.VideoPresetMode.Game.Saturation = SchemeFactoryDefaultInROM[B_ImageSchemeCount].Saturation;
					BL_ResultRegion = FALSE;
				}
				// Nature - Hue
				if (IsUserPrefOutOfRange(CheckPresetModeUserPreferences.VideoPresetMode.Nature.Hue, ADJV_Hue))
				{
					CheckPresetModeUserPreferences.VideoPresetMode.Nature.Hue = SchemeFactoryDefaultInROM[B_ImageSchemeCount].Hue;
					BL_ResultRegion = FALSE;
				}
				// Nature - Saturation
				if (IsUserPrefOutOfRange(CheckPresetModeUserPreferences.VideoPresetMode.Nature.Saturation, ADJV_Saturation))
				{
					CheckPresetModeUserPreferences.VideoPresetMode.Nature.Saturation = SchemeFactoryDefaultInROM[B_ImageSchemeCount].Saturation;
					BL_ResultRegion = FALSE;
				}
				// xvMode - Hue
				if (IsUserPrefOutOfRange(CheckPresetModeUserPreferences.VideoPresetMode.xvMode.Hue, ADJV_Hue))
				{
					CheckPresetModeUserPreferences.VideoPresetMode.xvMode.Hue = SchemeFactoryDefaultInROM[B_ImageSchemeCount].Hue;
					BL_ResultRegion = FALSE;
				}
				// xvMode - Saturation
				if (IsUserPrefOutOfRange(CheckPresetModeUserPreferences.VideoPresetMode.xvMode.Saturation, ADJV_Saturation))
				{
					CheckPresetModeUserPreferences.VideoPresetMode.xvMode.Saturation = SchemeFactoryDefaultInROM[B_ImageSchemeCount].Saturation;
					BL_ResultRegion = FALSE;
				}
			
				if (BL_ResultRegion == FALSE)
				{
					NVRam_WriteBuffer(W_AddressPreMode, (BYTE *)(&CheckPresetModeUserPreferences), PresetModesUserPrefSize);
					NVRam_WriteByte(W_AddressPreMode + PresetModesUserPrefSize, CalculateChecksum(&CheckPresetModeUserPreferences, PresetModesUserPrefSize));
					BL_Result = FALSE;
					BL_ResultRegion = TRUE;
					msg_i("Check Preset Mode UserPref in NVRam error!", 0);
				}
			}

			// compute the start address
			W_AddressPSD = PortSchemeDependentEntryStart + (WORD)B_ImageSchemeCount * (PortSchemeDependentUserPrefSize + ChecksumSize);
			
			// read the PortDependentUserPreferences record and save it
			NVRam_ReadBuffer(W_AddressPSD, (BYTE *)(&CheckPortSchemeDependentUserPreferences), PortSchemeDependentUserPrefSize);

			// Brightness/Contrast
			// Brightness
			if (IsUserPrefOutOfRange(CheckPortSchemeDependentUserPreferences.Brightness, ADJV_Brightness))
			{
				CheckPortSchemeDependentUserPreferences.Brightness = SchemeFactoryDefaultInROM[B_ImageSchemeCount].Brightness;
				BL_ResultRegion = FALSE;
			}
			// Contrast
			if (IsUserPrefOutOfRange(CheckPortSchemeDependentUserPreferences.Contrast, ADJV_Contrast))
			{
				CheckPortSchemeDependentUserPreferences.Contrast = SchemeFactoryDefaultInROM[B_ImageSchemeCount].Contrast;
				BL_ResultRegion = FALSE;
			}

			// Color Settings
			// Input Color Format
			if (IsUserPrefOutOfRange(CheckPortSchemeDependentUserPreferences.ColorFormat, ADJV_ColorFormat))
			{
				CheckPortSchemeDependentUserPreferences.ColorFormat = SchemeFactoryDefaultInROM[B_ImageSchemeCount].ColorFormat;
				BL_ResultRegion = FALSE;
			}
			// Gamma
			if (IsUserPrefOutOfRange(CheckPortSchemeDependentUserPreferences.GammaMode, ADJV_GammaMode))
			{
				CheckPortSchemeDependentUserPreferences.GammaMode = SchemeFactoryDefaultInROM[B_ImageSchemeCount].GammaMode;
				BL_ResultRegion = FALSE;
			}
			// Mode Select
			if (IsUserPrefOutOfRange(CheckPortSchemeDependentUserPreferences.ColorMode, ADJV_ColorMode))
			{
				CheckPortSchemeDependentUserPreferences.ColorMode = SchemeFactoryDefaultInROM[B_ImageSchemeCount].ColorMode;
				BL_ResultRegion = FALSE;
			}
			// Preset Mode
			if (IsUserPrefOutOfRange(CheckPortSchemeDependentUserPreferences.GraphicsImageMode, ADJV_GraphicsImageMode))
			{
				CheckPortSchemeDependentUserPreferences.GraphicsImageMode = SchemeFactoryDefaultInROM[B_ImageSchemeCount].GraphicsImageMode;
				BL_ResultRegion = FALSE;
			}
			if (IsUserPrefOutOfRange(CheckPortSchemeDependentUserPreferences.VideoImageMode, ADJV_VideoImageMode))
			{
				CheckPortSchemeDependentUserPreferences.VideoImageMode = SchemeFactoryDefaultInROM[B_ImageSchemeCount].VideoImageMode;
				BL_ResultRegion = FALSE;
			}
			// Custom Color - Gain R
			if (IsUserPrefOutOfRange(CheckPortSchemeDependentUserPreferences.UserRedGain, ADJV_UserRedGain))
			{
				CheckPortSchemeDependentUserPreferences.UserRedGain = SchemeFactoryDefaultInROM[B_ImageSchemeCount].UserRedGain;
				BL_ResultRegion = FALSE;
			}
			// Custom Color - Gain G
			if (IsUserPrefOutOfRange(CheckPortSchemeDependentUserPreferences.UserGreenGain,  ADJV_UserGreenGain))
			{
				CheckPortSchemeDependentUserPreferences.UserGreenGain = SchemeFactoryDefaultInROM[B_ImageSchemeCount].UserGreenGain;
				BL_ResultRegion = FALSE;
			}
			// Custom Color - Gain B
			if (IsUserPrefOutOfRange(CheckPortSchemeDependentUserPreferences.UserBlueGain, ADJV_UserBlueGain))
			{
				CheckPortSchemeDependentUserPreferences.UserBlueGain = SchemeFactoryDefaultInROM[B_ImageSchemeCount].UserBlueGain;
				BL_ResultRegion = FALSE;
			}
			// Custom Color - Offset R
			if (IsUserPrefOutOfRange(CheckPortSchemeDependentUserPreferences.UserRedOff, ADJV_UserRedOff))
			{
				CheckPortSchemeDependentUserPreferences.UserRedOff = SchemeFactoryDefaultInROM[B_ImageSchemeCount].UserRedOff;
				BL_ResultRegion = FALSE;
			}
			// Custom Color - Offset G
			if (IsUserPrefOutOfRange(CheckPortSchemeDependentUserPreferences.UserGreenOff, ADJV_UserGreenOff))
			{
				CheckPortSchemeDependentUserPreferences.UserGreenOff = SchemeFactoryDefaultInROM[B_ImageSchemeCount].UserGreenOff;
				BL_ResultRegion = FALSE;
			}
			// Custom Color - Offset B
			if (IsUserPrefOutOfRange(CheckPortSchemeDependentUserPreferences.UserBlueOff, ADJV_UserBlueOff))
			{
				CheckPortSchemeDependentUserPreferences.UserBlueOff = SchemeFactoryDefaultInROM[B_ImageSchemeCount].UserBlueOff;
				BL_ResultRegion = FALSE;
			}
			// Custom Color - Hue R
			if (IsUserPrefOutOfRange(CheckPortSchemeDependentUserPreferences.Acm3RedHue, ADJV_Acm3RedHue))
			{
				CheckPortSchemeDependentUserPreferences.Acm3RedHue = SchemeFactoryDefaultInROM[B_ImageSchemeCount].Acm3RedHue;
				BL_ResultRegion = FALSE;
			}
			// Custom Color - Hue G
			if (IsUserPrefOutOfRange(CheckPortSchemeDependentUserPreferences.Acm3GreenHue, ADJV_Acm3GreenHue))
			{
				CheckPortSchemeDependentUserPreferences.Acm3GreenHue = SchemeFactoryDefaultInROM[B_ImageSchemeCount].Acm3GreenHue;
				BL_ResultRegion = FALSE;
			}
			// Custom Color - Hue B
			if (IsUserPrefOutOfRange(CheckPortSchemeDependentUserPreferences.Acm3BlueHue, ADJV_Acm3BlueHue))
			{
				CheckPortSchemeDependentUserPreferences.Acm3BlueHue = SchemeFactoryDefaultInROM[B_ImageSchemeCount].Acm3BlueHue;
				BL_ResultRegion = FALSE;
			}
			// Custom Color - Hue C
			if (IsUserPrefOutOfRange(CheckPortSchemeDependentUserPreferences.Acm3CyanHue, ADJV_Acm3CyanHue))
			{
				CheckPortSchemeDependentUserPreferences.Acm3CyanHue = SchemeFactoryDefaultInROM[B_ImageSchemeCount].Acm3CyanHue;
				BL_ResultRegion = FALSE;
			}
			// Custom Color - Hue M
			if (IsUserPrefOutOfRange(CheckPortSchemeDependentUserPreferences.Acm3MagentaHue, ADJV_Acm3MagentaHue))
			{
				CheckPortSchemeDependentUserPreferences.Acm3MagentaHue = SchemeFactoryDefaultInROM[B_ImageSchemeCount].Acm3MagentaHue;
				BL_ResultRegion = FALSE;
			}
			// Custom Color - Hue Y
			if (IsUserPrefOutOfRange(CheckPortSchemeDependentUserPreferences.Acm3YellowHue, ADJV_Acm3YellowHue))
			{
				CheckPortSchemeDependentUserPreferences.Acm3YellowHue = SchemeFactoryDefaultInROM[B_ImageSchemeCount].Acm3YellowHue;
				BL_ResultRegion = FALSE;
			}
			// Custom Color - Saturation R
			if (IsUserPrefOutOfRange(CheckPortSchemeDependentUserPreferences.Acm3RedSat, ADJV_Acm3RedSat))
			{
				CheckPortSchemeDependentUserPreferences.Acm3RedSat = SchemeFactoryDefaultInROM[B_ImageSchemeCount].Acm3RedSat;
				BL_ResultRegion = FALSE;
			}
			// Custom Color - Saturation G
			if (IsUserPrefOutOfRange(CheckPortSchemeDependentUserPreferences.Acm3GreenSat, ADJV_Acm3GreenSat))
			{
				CheckPortSchemeDependentUserPreferences.Acm3GreenSat = SchemeFactoryDefaultInROM[B_ImageSchemeCount].Acm3GreenSat;
				BL_ResultRegion = FALSE;
			}
			// Custom Color - Saturation B
			if (IsUserPrefOutOfRange(CheckPortSchemeDependentUserPreferences.Acm3BlueSat, ADJV_Acm3BlueSat))
			{
				CheckPortSchemeDependentUserPreferences.Acm3BlueSat = SchemeFactoryDefaultInROM[B_ImageSchemeCount].Acm3BlueSat;
				BL_ResultRegion = FALSE;
			}
			// Custom Color - Saturation C
			if (IsUserPrefOutOfRange(CheckPortSchemeDependentUserPreferences.Acm3CyanSat, ADJV_Acm3CyanSat))
			{
				CheckPortSchemeDependentUserPreferences.Acm3CyanSat = SchemeFactoryDefaultInROM[B_ImageSchemeCount].Acm3CyanSat;
				BL_ResultRegion = FALSE;
			}
			// Custom Color - Saturation M
			if (IsUserPrefOutOfRange(CheckPortSchemeDependentUserPreferences.Acm3MagentaSat, ADJV_Acm3MagentaSat))
			{
				CheckPortSchemeDependentUserPreferences.Acm3MagentaSat = SchemeFactoryDefaultInROM[B_ImageSchemeCount].Acm3MagentaSat;
				BL_ResultRegion = FALSE;
			}
			// Custom Color - Saturation Y
			if (IsUserPrefOutOfRange(CheckPortSchemeDependentUserPreferences.Acm3YellowSat, ADJV_Acm3YellowSat))
			{
				CheckPortSchemeDependentUserPreferences.Acm3YellowSat = SchemeFactoryDefaultInROM[B_ImageSchemeCount].Acm3YellowSat;
				BL_ResultRegion = FALSE;
			}
			// Hue
			if (IsUserPrefOutOfRange(CheckPortSchemeDependentUserPreferences.Hue, ADJV_Hue))
			{
				CheckPortSchemeDependentUserPreferences.Hue = SchemeFactoryDefaultInROM[B_ImageSchemeCount].Hue;
				BL_ResultRegion = FALSE;
			}
			// Saturation
			if (IsUserPrefOutOfRange(CheckPortSchemeDependentUserPreferences.Saturation, ADJV_Saturation))
			{
				CheckPortSchemeDependentUserPreferences.Saturation = SchemeFactoryDefaultInROM[B_ImageSchemeCount].Saturation;
				BL_ResultRegion = FALSE;
			}

			// Display Settings
			// Wide Mode
			if (IsUserPrefOutOfRange(CheckPortSchemeDependentUserPreferences.AspectRatio, ADJV_PortAspectRatio))
			{
				CheckPortSchemeDependentUserPreferences.AspectRatio = SchemeFactoryDefaultInROM[B_ImageSchemeCount].AspectRatio;
				BL_ResultRegion = FALSE;
			}
			// Sharpness
			if (IsUserPrefOutOfRange(CheckPortSchemeDependentUserPreferences.Sharpness, ADJV_Sharpness))
			{
				CheckPortSchemeDependentUserPreferences.Sharpness = SchemeFactoryDefaultInROM[B_ImageSchemeCount].Sharpness;
				BL_ResultRegion = FALSE;
			}
			// Dynamic Contrast
			{
				for (B_PresetModeCount=0; B_PresetModeCount<=WB_CustomColor ; B_PresetModeCount++)
				{
					if(IsUserPrefOutOfRange(CheckPortSchemeDependentUserPreferences.PresetModes[B_PresetModeCount].DCR, ADJV_DCR))
					{
						CheckPortSchemeDependentUserPreferences.PresetModes[B_PresetModeCount].DCR = 
							SchemeFactoryDefaultInROM[B_ImageSchemeCount].PresetModes[B_PresetModeCount].DCR;
						BL_ResultRegion = FALSE;
					}

				}	
			}	

			if (BL_ResultRegion == FALSE)
			{
				NVRam_WriteBuffer(W_AddressPSD, (BYTE *)(&CheckPortSchemeDependentUserPreferences), PortSchemeDependentUserPrefSize);
				NVRam_WriteByte(W_AddressPSD + PortSchemeDependentUserPrefSize, CalculateChecksum(&CheckPortSchemeDependentUserPreferences, PortSchemeDependentUserPrefSize));
				BL_Result = FALSE;
				BL_ResultRegion = TRUE;
				msg_i("Check Port Scheme Dependent UserPref in NVRam error!", 0);
			}
		}
	}

	//
	// ModeIndependentUserPreferences
	//
	// Input Sourde
	if (IsUserPrefOutOfRange(UserPrefCurrentInputMain, ADJV_CurrentInputMain))
	{
		UserPrefCurrentInputMain = FactoryDefaultInROM.CurrentInputMain;
		BL_ResultRegion = FALSE;
	}

	// Display Settings
	// Noise Reduction
	if (IsUserPrefOutOfRange(UserPrefTNR, ADJV_TNR))
	{
		UserPrefTNR = FactoryDefaultInROM.TNR;
		BL_ResultRegion = FALSE;
	}

	// PIP Settings
	// PIP Mode
	if (IsUserPrefOutOfRange(UserPrefPipMode, ADJV_PipMode))
	{
		UserPrefPipMode = FactoryDefaultInROM.PipMode;
		BL_ResultRegion = FALSE;
	}
	// PIP Source
	if (IsUserPrefOutOfRange(UserPrefCurrentInputPip, ADJV_CurrentInputPip))
	{
		UserPrefCurrentInputPip = FactoryDefaultInROM.CurrentInputPip;
		BL_ResultRegion = FALSE;
	}
	// PIP Size
	if (IsUserPrefOutOfRange(UserPrefPipSize, ADJV_PipSize))
	{
		UserPrefPipSize = FactoryDefaultInROM.PipSize;
		BL_ResultRegion = FALSE;
	}
	// PIP Position
	if (IsUserPrefOutOfRange(UserPrefPipPosition, ADJV_PipPosition))
	{
		UserPrefPipPosition = FactoryDefaultInROM.PipPosition;
		BL_ResultRegion = FALSE;
	}
	// PIP Contrast
	if (IsUserPrefOutOfRange(UserPrefPipContrast, ADJV_PipContrast))
	{
		UserPrefPipContrast = FactoryDefaultInROM.PipColorGroup.PipContrast;
		BL_ResultRegion = FALSE;
	}

	// Other Settings
	// Language
	if (IsUserPrefOutOfRange(UserPrefOsdLanguage, ADJV_OsdLanguage)||UserPrefOsdLanguage >= NUM_LANGUAGES)
	{
		UserPrefOsdLanguage = FactoryDefaultInROM.OsdLanguage;
		BL_ResultRegion = FALSE;
	}
	// Menu Transparency
	if (IsUserPrefOutOfRange(UserPrefOsdBlend, ADJV_OsdBlend))
	{
		UserPrefOsdBlend = FactoryDefaultInROM.OsdBlend;
		BL_ResultRegion = FALSE;
	}
	// Menu Timer
	if (IsUserPrefOutOfRange(UserPrefOsdTimeout, ADJV_OsdTimeout))
	{
		UserPrefOsdTimeout = FactoryDefaultInROM.OsdTimeout;
		BL_ResultRegion = FALSE;
	}
	// Menu Lock
	if (IsUserPrefOutOfRange(UserPrefOSDLock, ADJV_OSDLock))
	{
		UserPrefOSDLock = FactoryDefaultInROM.OSDLock;
		BL_ResultRegion = FALSE;
	}
	// Menu Rotation
	if (IsUserPrefOutOfRange(UserPrefOsdRotation, ADJV_OsdRotation))
	{
		UserPrefOsdRotation = FactoryDefaultInROM.OsdRotation;
		BL_ResultRegion = FALSE;
	}
	// Button Sound
	if (IsUserPrefOutOfRange(UserPrefButtonSoundOff, ADJV_ButtonSoundOff))
	{
		UserPrefButtonSoundOff = FactoryDefaultInROM.ButtonSoundOff;
		BL_ResultRegion = FALSE;
	}
	// Power Save Audio
	if (IsUserPrefOutOfRange(UserPrefPwrAudio, ADJV_PwrAudio))
	{
		UserPrefPwrAudio = FactoryDefaultInROM.PwrAudio;
		BL_ResultRegion = FALSE;
	}
	// DDC/CI
	if (IsUserPrefOutOfRange(UserPrefDdcciOnOffFlag, ADJV_DdcciOnOffFlag))
	{
		UserPrefDdcciOnOffFlag = FactoryDefaultInROM.DdcciOnOffFlag;
		BL_ResultRegion = FALSE;
	}

	// Personalize
	// Shotcut Key 1
	if (IsUserPrefOutOfRange(UserPrefPersonalKey1, ADJV_PersonalKey1))
	{
		UserPrefPersonalKey1 = FactoryDefaultInROM.PersonalKey1;
		BL_ResultRegion = FALSE;
	}
	// Shotcut Key 2
	if (IsUserPrefOutOfRange(UserPrefPersonalKey2, ADJV_PersonalKey2))
	{
		UserPrefPersonalKey2 = FactoryDefaultInROM.PersonalKey2;
		BL_ResultRegion = FALSE;
	}
	// Shotcut Key 3
	if (IsUserPrefOutOfRange(UserPrefPersonalKey3, ADJV_PersonalKey3))
	{
		UserPrefPersonalKey3 = FactoryDefaultInROM.PersonalKey3;
		BL_ResultRegion = FALSE;
	}

	if (BL_ResultRegion == FALSE)
	{
		SaveModeIndependentSettings();
		BL_Result = FALSE;
		BL_ResultRegion = TRUE;
		msg_i("Check Mode Independent UserPref in NVRam error!", 0);
	}

	//
	// ModeDependentUserPreferences
	//
	// Display Settings
	#if 1//modified by lion 20090410 //commente:  only initial the bad value is reasonable !  we met a issue: VGA 1920x1080@50p,it's htotal is 2460,so every time power on,this mode always do autoadjust every time!!
	{
		BYTE B_ModeDependentCount;
		ModeDependentUserPrefType CheckModeDependentUserPreferences;
		WORD W_AddressMD;
		for (B_ModeDependentCount = 0; B_ModeDependentCount < MAX_USER_MODES_SAVED; B_ModeDependentCount++)
		{
			

			W_AddressMD = ModeDependentUserEntryStart + (WORD)B_ModeDependentCount * (ModeDependentUserPrefSize + sizeof(BYTE) + ChecksumSize);
			NVRam_ReadBuffer(W_AddressMD, (BYTE *)(&CheckModeDependentUserPreferences), ModeDependentUserPrefSize);

			// Horizontal Position
			//if (IsUserPrefOutOfRange(CheckModeDependentUserPreferences.HSyncDelay, ADJV_HSyncDelay))
			if ((CheckModeDependentUserPreferences.HSyncDelay > 50) || (CheckModeDependentUserPreferences.HSyncDelay < -50))
				{
				CheckModeDependentUserPreferences.HSyncDelay=0;
  				BL_ResultRegion = FALSE;
				}
 			// Vertical Position
			//if (IsUserPrefOutOfRange(CheckModeDependentUserPreferences.VSyncDelay, ADJV_VSyncDelay))
			if ((CheckModeDependentUserPreferences.VSyncDelay > 50) || (CheckModeDependentUserPreferences.VSyncDelay < -50))
				{
				CheckModeDependentUserPreferences.VSyncDelay=0;
 				BL_ResultRegion = FALSE;
				}
 			// Pixel Clock
 			// (IsUserPrefOutOfRange(CheckModeDependentUserPreferences.HTotal, ADJV_HTotal))
			if (CheckModeDependentUserPreferences.HTotal > (2640+300))// modified by lion 20090410 1920x1080@50P htotal is 2640!!  the old value cause this mode always do autoadjust after power on  (2208 + 100))	// Max support mode ==> 2408x1400@60R HTotal:2208
				{
				CheckModeDependentUserPreferences.HTotal=0;	
 				BL_ResultRegion = FALSE;
				}
 			// Phase
 			if (IsUserPrefOutOfRange(CheckModeDependentUserPreferences.HSyncPhase, ADJV_HSyncPhase))
 				{
 				CheckModeDependentUserPreferences.HSyncPhase=0;
 				BL_ResultRegion = FALSE;
 				}

		if (BL_ResultRegion == FALSE)
		{
			//extern void SaveModeDependentEntry(WORD W_Address, ModeDependentUserPrefType * UserPrefTypePtr, BYTE B_AutoAdjustCount);
			//InitModeDependentLookUp(gmvb_PipChannel);
			msg_i("Check Mode Dependent UserPref in NVRam error!", 0);
			//SaveModeDependentEntry(W_AddressMD,&CheckModeDependentUserPreferences,0);//since some value error,we need do autoadjust again,so write "B_AutoAdjustCount" as 0

			BL_Result = TRUE;//lion comment: since only the bad value was initialized and the checksum was calculated again,so just return TRUE is reasonable           FALSE;
		}
	}
}
#else
		{
		BYTE B_ModeDependentCount;
		ModeDependentUserPrefType CheckModeDependentUserPreferences;

		for (B_ModeDependentCount = 0; B_ModeDependentCount < MAX_USER_MODES_SAVED; B_ModeDependentCount++)
		{
			WORD W_AddressMD;

			W_AddressMD = ModeDependentUserEntryStart + (WORD)B_ModeDependentCount * (ModeDependentUserPrefSize + sizeof(BYTE) + ChecksumSize);
			NVRam_ReadBuffer(W_AddressMD, (BYTE *)(&CheckModeDependentUserPreferences), ModeDependentUserPrefSize);

			// Horizontal Position
			//if (IsUserPrefOutOfRange(CheckModeDependentUserPreferences.HSyncDelay, ADJV_HSyncDelay))
			if ((CheckModeDependentUserPreferences.HSyncDelay > 50) || (CheckModeDependentUserPreferences.HSyncDelay < -50))
  				BL_ResultRegion = FALSE;
 			// Vertical Position
			//if (IsUserPrefOutOfRange(CheckModeDependentUserPreferences.VSyncDelay, ADJV_VSyncDelay))
			if ((CheckModeDependentUserPreferences.VSyncDelay > 50) || (CheckModeDependentUserPreferences.VSyncDelay < -50))
 				BL_ResultRegion = FALSE;
 			// Pixel Clock
 			// (IsUserPrefOutOfRange(CheckModeDependentUserPreferences.HTotal, ADJV_HTotal))
			if (CheckModeDependentUserPreferences.HTotal >  (2208 + 100))	// Max support mode ==> 2408x1400@60R HTotal:2208
 				BL_ResultRegion = FALSE;
 			// Phase
 			if (IsUserPrefOutOfRange(CheckModeDependentUserPreferences.HSyncPhase, ADJV_HSyncPhase))
 				BL_ResultRegion = FALSE;
 		}
	}

	if (BL_ResultRegion == FALSE)
	{
		// Re-initialize mode dependent descriptor and lookup table.
		InitModeDependentDescr(gmvb_MainChannel);
		//InitModeDependentDescr(gmvb_PipChannel);
		InitModeDependentLookUp(gmvb_MainChannel);
		//InitModeDependentLookUp(gmvb_PipChannel);
		gm_Print("Check Mode Dependent UserPref in NVRam error!", 0);

		BL_Result = FALSE;
	}

#endif
	return BL_Result;
}


void CVBSForceModeSetupHandler(void) //KevinL 20090420 for CVBS NTSC-M <->NTSC443 , PAL <-> SECAM need modesetup for correct setting
{
  /* static BYTE B_CVBS_System_New = 0, B_CVBS_System_Old = 0; //0: NTSC-M  5:NTSC443  ; 1: PAL-I  4 : SECAM, 

  	if(UserPrefCurrentInputMain == ALL_BOARDS_CVBS1)
   	{
	     B_CVBS_System_New= (gm_ReadRegByte(HVTIMER_STATUS1) & STD_DETECTED);   	
	     if((((B_CVBS_System_New == NTSC) && (B_CVBS_System_Old == NTSC443)) ||((B_CVBS_System_New == NTSC443) && (B_CVBS_System_Old == NTSC)))
		 	||(((B_CVBS_System_New == PAL) && (B_CVBS_System_Old == SECAM)) ||((B_CVBS_System_New == SECAM) && (B_CVBS_System_Old == PAL))))
	     {
		 B_CVBS_System_Old = B_CVBS_System_New;	
		 ModeSetupRequest(gmvb_MainChannel, REQ_MS_INPUT_CHANGE, PARAM_NONE, 50);
	     }
	     else
		 B_CVBS_System_Old = B_CVBS_System_New;	
 
   	}
	else if((UserPrefCurrentInputPip == ALL_BOARDS_CVBS1) && (UserPrefPipMode != NO_PIP))
	{
	     B_CVBS_System_New= (gm_ReadRegByte(HVTIMER_STATUS1) & STD_DETECTED);   	
	     if((((B_CVBS_System_New == NTSC) && (B_CVBS_System_Old == NTSC443)) ||((B_CVBS_System_New == NTSC443) && (B_CVBS_System_Old == NTSC)))
		 		||(((B_CVBS_System_New == PAL) && (B_CVBS_System_Old == SECAM)) ||((B_CVBS_System_New == SECAM) && (B_CVBS_System_Old == PAL))))
	     {
		 B_CVBS_System_Old = B_CVBS_System_New;	
		 ModeSetupRequest(gmvb_PipChannel, REQ_MS_INPUT_CHANGE, PARAM_NONE, 50);
	     }
	     else
		 B_CVBS_System_Old = B_CVBS_System_New;		
	}
	*/
}

#define B_DataThreshold 	3
BYTE CheckIBDThreshold(gmt_PHY_CH B_Channel)
{
	BYTE IBD_Threshold_Inc, IBD_Threshold_Dec;
	//DWORD DW_Time = gm_GetSystemTime();

	//Read the original setting for return
	IBD_Threshold_Inc =  (gm_ReadRegWord(ip_ibd_control[B_Channel])&0xF0)>>4;
	//gm_Print("AutoIBD original Threshold IBD_Threshold_Inc = %d ", IBD_Threshold_Inc);

	if((abs(gm_ReadRegWord(ip_ibd_hwidth[B_Channel]) - stInput[B_Channel].Hactive)> 10)
		||(abs(gm_ReadRegWord(ip_ibd_vlength[B_Channel]) - stInput[B_Channel].Vactive)> 10))
	{
		BYTE IBD_W =0, IBD_H =0;
		WORD W_Width[14] = {0}, W_Height[14] = {0};
		//BYTE B_DataThreshold;
		//for(B_DataThreshold = 1; B_DataThreshold < 4; B_DataThreshold++)
		{
			for(IBD_Threshold_Inc = 2; IBD_Threshold_Inc < 14; IBD_Threshold_Inc++)
			{
		        	gm_WriteRegByte(ip_ibd_control[B_Channel], IBD_Threshold_Inc << 4);		
				gm_InputWaitVsync(B_Channel, 1);
				W_Width[IBD_W] = gm_ReadRegWord(ip_ibd_hwidth[B_Channel]);
				W_Height[IBD_H] = gm_ReadRegWord(ip_ibd_vlength[B_Channel]);

			 	//gm_Print("AutoIBD IBD_Threshold_Inc = %d, B_Width[%d] =  %d, B_Height[%d] = %d ",IBD_Threshold_Inc, IBD_W,W_Width[IBD_W], IBD_H, W_Height[IBD_H]);

				if((IBD_Threshold_Inc > 3)&&(abs(W_Width[IBD_W] - stInput[B_Channel].Hactive)< 5)&&(abs(W_Height[IBD_H] - stInput[B_Channel].Vactive)< 5))
				{
			 		//gm_Print("AutoIBD W_Width[IBD_W] = %d, W_Width[IBD_W -1] =  %d, W_Width[IBD_W -2] = %d ",W_Width[IBD_W], W_Width[IBD_W-1],W_Width[IBD_W-2]);
			 		//gm_Print("AutoIBD W_Height[IBD_H] = %d, W_Height[IBD_H -1] =  %d, W_Height[IBD_H -2] = %d ",W_Height[IBD_H], W_Height[IBD_H-1],W_Height[IBD_H-2]);
					if((abs(W_Width[IBD_W] -W_Width[IBD_W-1])< B_DataThreshold) &&(abs(W_Width[IBD_W-1] -W_Width[IBD_W-2])< B_DataThreshold)
						&&(abs(W_Height[IBD_H] -W_Height[IBD_H-1])< B_DataThreshold) &&(abs(W_Height[IBD_H-1] -W_Height[IBD_H-2])< B_DataThreshold))
					{
						//gm_Print("AutoIBD find Threshold IBD_Threshold_Inc = %d ", IBD_Threshold_Inc);
						break;
					}
				}
				IBD_W++;
				IBD_H++;
			}
			
			IBD_W = 0;
			IBD_H = 0;
			for(IBD_Threshold_Dec = 14; IBD_Threshold_Dec > 2; IBD_Threshold_Dec--)
			{
		        	gm_WriteRegByte(ip_ibd_control[B_Channel], IBD_Threshold_Dec << 4);		
				gm_InputWaitVsync(B_Channel, 1);
				W_Width[IBD_W] = gm_ReadRegWord(ip_ibd_hwidth[B_Channel]);
				W_Height[IBD_H] = gm_ReadRegWord(ip_ibd_vlength[B_Channel]);

			 	//gm_Print("AutoIBD IBD_Threshold_Dec = %d, B_Width[%d] =  %d, B_Height[%d] = %d ",IBD_Threshold_Dec, IBD_W,W_Width[IBD_W], IBD_H, W_Height[IBD_H]);

				if((IBD_Threshold_Dec < 13)&&(abs(W_Width[IBD_W] - stInput[B_Channel].Hactive)< 5)&&(abs(W_Height[IBD_H] - stInput[B_Channel].Vactive)< 5))
				{

			 	//gm_Print("AutoIBD W_Width[IBD_W] = %d, W_Width[IBD_W -1] =  %d, W_Width[IBD_W -2] = %d ",W_Width[IBD_W], W_Width[IBD_W-1],W_Width[IBD_W-2]);
			 	//gm_Print("AutoIBD W_Height[IBD_H] = %d, W_Height[IBD_H -1] =  %d, W_Height[IBD_H -2] = %d ",W_Height[IBD_H], W_Height[IBD_H-1],W_Height[IBD_H-2]);
					if((abs(W_Width[IBD_W] -W_Width[IBD_W-1])< B_DataThreshold) &&(abs(W_Width[IBD_W-1] -W_Width[IBD_W-2])< B_DataThreshold)
						&&(abs(W_Height[IBD_H] -W_Height[IBD_H-1])< B_DataThreshold) &&(abs(W_Height[IBD_H-1] -W_Height[IBD_H-2])< B_DataThreshold))
					{
						//gm_Print("AutoIBD find Threshold IBD_Threshold_Dec = %d ", IBD_Threshold_Dec);
						break;
					}
				}
				IBD_W++;
				IBD_H++;
			}
			
			//gm_Print("AutoIBD Threshold IBD_Threshold_Inc = %d ", IBD_Threshold_Inc);
			//gm_Print("AutoIBD Threshold IBD_Threshold_Dec = %d ", IBD_Threshold_Dec);
			if((IBD_Threshold_Inc != 14)&&(IBD_Threshold_Dec != 2))
				IBD_Threshold_Inc = (IBD_Threshold_Inc +IBD_Threshold_Dec)/2;
			else	 if((IBD_Threshold_Inc == 14)&&(IBD_Threshold_Dec != 2))
				IBD_Threshold_Inc = IBD_Threshold_Dec;
			else if((IBD_Threshold_Inc != 14)&&(IBD_Threshold_Dec == 2))
				IBD_Threshold_Inc = IBD_Threshold_Inc;
			else 		//Not found
			{
				BYTE B_LowIBD;
				IBD_Threshold_Inc = 4;			//Set default
				for(B_LowIBD = 4; B_LowIBD < 6; B_LowIBD++)
				{
		        		gm_WriteRegByte(ip_ibd_control[B_Channel], B_LowIBD << 4);		
					if((abs(gm_ReadRegWord(ip_ibd_hwidth[B_Channel]) - stInput[B_Channel].Hactive)< 2)
						||(abs(gm_ReadRegWord(ip_ibd_vlength[B_Channel]) - stInput[B_Channel].Vactive) < 2))
					{
						IBD_Threshold_Inc = B_LowIBD;
						break;
					}
				}
				//IBD_Threshold_Inc = 5;			//Set default
			}
			//gm_Print("AutoIBD Final Threshold IBD_Threshold_Dec = %d ", IBD_Threshold_Inc);
		}
		gm_WriteRegByte(ip_ibd_control[B_Channel], IBD_Threshold_Inc<< 4);
	}

	//gm_Print("AutoIBD Threshold take time = %d ", (gm_GetSystemTime() - DW_Time));

	return IBD_Threshold_Inc;
}



#ifdef RESET_AUDIO_MPE
extern void gm_MpePowerDn(void);
void NoSounddetect(void)		
{
	 static BYTE ucNoSoundCtrl;

//&*&*&*S 20120315:SWRD4001: Kordonm(0)  Modify for OSD Workbench
	if(!((UserPrefCurrentInputMain == ALL_BOARDS_DP1) ||(UserPrefCurrentInputMain == ALL_BOARDS_DP3)|| (UserPrefCurrentInputMain == ALL_BOARDS_HDMI) || 
		(((UserPrefCurrentInputPip == ALL_BOARDS_DP1) || (UserPrefCurrentInputPip == ALL_BOARDS_DP3) || (UserPrefCurrentInputPip == ALL_BOARDS_HDMI)) && (UserPrefPipMode != NO_PIP))) ) //KevinL 20090525 for VGA DVI hang on MPE DMA when PIP on factory reset apply adjust FIFO(AVS)
		return;
//&*&*&*E 20120315 : Kordonm(1)  Modify for OSD Workbench
	
	if(((UserPrefSpeakerAudioSource == AUDIO_MAIN)&&(!gm_IsModeComplete(CH_A)))
		||((UserPrefSpeakerAudioSource == AUDIO_PIP)&&((!gm_IsModeComplete(CH_B))||(UserPrefPipMode == NO_PIP))))
	return;
	if (gm_ReadRegWord(X86_COMMCTRL_LOWER)& TX_STATUS_FAILED)
		ucNoSoundCtrl++;
	else
		ucNoSoundCtrl=0;
 
   	if(ucNoSoundCtrl==5)		//after 5 Loops
   	{
		ucNoSoundCtrl = 0;
		SetAudioDACOutput(OFF);		//Neil 20090312+

		gm_ClearRegBitsWord(SOFT_PD, AUDDS1_EN);
		gm_ClearRegBitsWord(SOFT_PD, AUDDS3_EN);
		
		FreeInstance( AUD_INST_TYPE_1 );
		#if( FEATURE_AUDIO_ARCHITECTURE == AUDIO_MAD8 )
			FreeInstance( AUD_INST_TYPE_2 );
			gm_MpePowerDn();
		#endif
		#ifdef USE_RTEX
			StopAudioTask(); 
		#endif
		AudioInit(); 
		#ifdef USE_RTEX
		#if (FEATURE_AUDIO == ENABLE)
			StartAudioTask();
		#endif
		#endif //#ifdef USE_RTEX
		
		gm_SetRegBitsWord(SOFT_PD, AUDDS1_EN);		//Neil 20090507+ 
		gm_SetRegBitsWord(SOFT_PD, AUDDS3_EN);		//Neil 20090507+ 

		//Neil 20090427* Re-setup audio parameter after reset MPE
		//SetAudioVolume(D_AUDIO_LOUDSPEAKER, AUDIO_VOLUME);
		//SetAudioBass(D_AUDIO_LOUDSPEAKER, UserPrefAudioBass);
		//SetAudioTreble(D_AUDIO_LOUDSPEAKER, UserPrefAudioTreble);
		//SetAudioBalance(D_AUDIO_LOUDSPEAKER, UserPrefAudioBalance);
		msg("000000000000000000000000000000000000000", 0);
		AudioMuteFlag = OFF;
 	}
   	else
	{
            return;
	}
}
#endif 

#ifdef ADJUST_FIFO_ENABLE
void AdjustFIFO(void)
{
	if (UserPrefPipMode == NO_PIP )
	{
	#ifdef VGA_193MHZ_FIFO_ADJUST	//Kam 20090716 #01 , Main-VGA 1920x1200@60 ,193Mhz  , Pip-CVBS ,Open Maximum OSD , Bandwidth cause screen flash.
		if (UserPrefCurrentInputMain == ALL_BOARDS_VGA1)// && UserPrefCurrentInputPip == ALL_BOARDS_CVBS1)
		{
			gm_ClearRegBitsWord(MC_FIFO_EN_lo, ( MC_FIFO_OD_RD |MC_FIFO_OD_WR | MC_FIFO_VBI_WR));
			gm_ClearRegBitsWord(MC_FIFO_EN_hi, ( MC_FIFO_AVS_RD|MC_FIFO_AVS_WR));
		}
		else
	#endif
		{
			if (gm_ReadRegWord(MC_FIFO_EN_lo) != 0xFFFF)
				gm_WriteRegWord(MC_FIFO_EN_lo, 0xFFFF);	

			if ((UserPrefCurrentInputMain == ALL_BOARDS_HDMI) || (UserPrefCurrentInputMain == DP1))
			{
				msg("HDMI DP reduce bandwidth",0);		
				if (!(gm_IsInterlaced(CH_A)))
				{
					gm_ClearRegBitsWord(MC_FIFO_EN_lo, (MC_FIFO_MV_WR | MC_FIFO_MADI_PREV_RD | MC_FIFO_MADI_PM1_RD | MC_FIFO_MV_RD |MC_FIFO_MV_PREV_RD));
				}

				gm_ClearRegBitsWord(MC_FIFO_EN_lo, ( MC_FIFO_OD_WR | MC_FIFO_VBI_WR | MC_FIFO_DEC1_WR|MC_FIFO_DEC2_WR | MC_FIFO_OD_RD | MC_FIFO_DEC1_RD1));
				gm_ClearRegBitsWord(MC_FIFO_EN_hi, (MC_FIFO_DEC1_RD2 | MC_FIFO_DEC2_RD1 |MC_FIFO_DEC2_RD2 |  MC_FIFO_TNR_RD | MC_FIFO_TNR_WR));			

				gm_SetRegBitsWord(MC_FIFO_EN_hi, (MC_FIFO_AVS_WR | MC_FIFO_AVS_RD));
			#ifdef NoPipByPassMemory
				gm_SetRegBitsWord(MC_MEMORY_CONFIG, PIP_MC_BYPASS);			
			#endif
			}
			else
			{
				gm_SetRegBitsWord(MC_FIFO_EN_hi, (MC_FIFO_DEC1_RD2 | MC_FIFO_DEC2_RD1|MC_FIFO_DEC2_RD2)); //KevinL 20090528 for CVBS 		
			}
		}
	}
	else
	{
		gm_WriteRegWord(MC_FIFO_EN_hi, 0xFFF);
		gm_WriteRegWord(MC_FIFO_EN_lo, 0xFFFF);	
	}
}
#endif

void FactoryModeDisableRotation(void)  //calo add for enter into factory menu disable rotation function 090618
{
      gmvb_OsdRotation = 0;
}

BYTE RGBSharpnessRemapping(void) //121010 Edward for DELL 2913
{
   BYTE B_ReturnValue = 0;
   
   //const BYTE Ba_Sharpeness_RGB[25] = {0,0,4,4,6,6,8,8,10,10,12,12,14,14,16,16,18,18,20,20,22,22,24,24,24};
  
   if(UserPrefSharpness>24)
   	B_ReturnValue = 12;
   else
   {   	  
   	 #if(DEFAULT_PANEL == LG_WHD_LM290WW1_SSA1)
	  if(!IsColorSpace_GRAPHICS_SRGB)
   	 	B_ReturnValue = UserPrefSharpness+2;   
	  else
	 #endif
	 	B_ReturnValue = UserPrefSharpness;
   }
  
   return B_ReturnValue;
}
BYTE YUVSharpnessRemapping(void)
{
   BYTE B_ReturnValue = 0;
   #ifdef DELL_U2410F//120922 Edward remapping from Vicnent test result   
   const BYTE Ba_SharpenessYUV_HD[25] = {0,0,4,4,6,6,8,8,10,10,12,12,14,14,16,16,18,18,20,20,22,22,24,24,24};
   #else
   const BYTE Ba_SharpenessYUV_HD[25] = {0,0,0,0,0,0,0,0,0,1,1,2,2,4,4,6,6,8,8,10,10,12,12,12,12};
   #endif   
   const BYTE Ba_SharpenessYUV_SD[25] = {2,2,2,2,2,3,3,3,3,4,4,4,4,5,5,6,6,8,8,10,10,12,12,12,12};
   //const BYTE Ba_SharpenessYUV_SD[25] = {0,0,4,4,6,6,8,8,10,10,12,12,14,14,16,16,18,18,20,20,22,22,24,24,24};

   gmt_MODE_TYPE B_VideoType = gm_GetCurrentInputModeType_New(gmvb_MainChannel);
   

   gm_Print(" UserPrefSharpness = %d", UserPrefSharpness);
   if(UserPrefSharpness>24)
   	B_ReturnValue = 12;
   else
   {
   	//gm_Print("##YUV sharpness %d",UserPrefSharpness);
      switch(B_VideoType)
      {
         case gmd_NTSC_480I:
         case gmd_PAL_576I:     
         case gmd_480P:
         case gmd_576P:                   
   	      B_ReturnValue = Ba_SharpenessYUV_SD[UserPrefSharpness];
            break;
         case gmd_720P:
         case gmd_1080I:            
         case gmd_1080P:
     	      B_ReturnValue = Ba_SharpenessYUV_HD[UserPrefSharpness];
            break;
         default:
            if(stInput[gmvb_MainChannel].Vtotal<700)
       	      B_ReturnValue = Ba_SharpenessYUV_SD[UserPrefSharpness];
            else
        	      B_ReturnValue = Ba_SharpenessYUV_HD[UserPrefSharpness];              
            break;                  
      }
   }
  
  gm_Print("##YUV sharpness table %d",B_ReturnValue);
   return B_ReturnValue;
}

WORD PixelGrab(gmt_PHY_CH B_Channel, WORD W_Xcord, WORD W_Ycord, gmt_RGB_COLOR B_Color )
{
   WORD W_Retval;
   WORD W_FlaglineOrg;
   
   if (B_Channel == CH_A)
   {
      W_FlaglineOrg = gm_ReadRegWord(IMP_FLAGLINE);         // save the current flagline value
      gm_WriteRegWord(IMP_PIXGRAB_V, W_Ycord);            // co-ords of the pixel we want
      gm_WriteRegWord(IMP_PIXGRAB_H, W_Xcord);
      gm_WriteRegWord(IMP_FLAGLINE, (W_Ycord + 1));         // wait for the correct line
      gm_SetRegBitsWord(IMP_CONTROL, IMP_PIXGRAB_EN);         // enable pixel grab

      gm_TimerStart(DELAY_10ms_TMR, 3);      // setup a timeout
      gm_SetRegBitsWord(MAIN_INPUT_STATUS, IMP_LINEFLAG);      // clear IP_LINEFLAG

      if (gm_IsInterlaced(B_Channel))               // the return statuses are all messed up here
      {
         // wait for this row done & an even field
         while (((gm_ReadRegWord(MAIN_INPUT_STATUS) & IMP_LINEFLAG) == 0) || (gm_ReadRegWord(MISC_STATUS) & IMP_ODD_STATUS))
         {
            if (gm_TimerCheck( DELAY_10ms_TMR) == TIMER_TMO)
            {
               break;
            }
         }
      }
      else
      {
         // wait for this row done
         while ((gm_ReadRegWord(MAIN_INPUT_STATUS) & IMP_LINEFLAG) == 0)
         {
            if (gm_TimerCheck(DELAY_10ms_TMR) == TIMER_TMO)
            {
               break;
            }
         }
      }

      if (B_Color == gmd_RED_COMPONENT)
         W_Retval = gm_ReadRegWord(IMP_PIXGRAB_RED);
      else if (B_Color == gmd_GREEN_COMPONENT)
         W_Retval = gm_ReadRegWord(IMP_PIXGRAB_GRN);
      else
         W_Retval = gm_ReadRegWord(IMP_PIXGRAB_BLU);

      // Do not disable pixel grab so we can get other color values after this function returns
      // Note: For FLI8668 disabling pixel grab causes the raw ADC data to be writen into the pixel grab result registers.
//      gm_ClearRegBitsWord(IMP_CONTROL, IMP_PIXGRAB_EN);
      gm_WriteRegWord(IMP_FLAGLINE, W_FlaglineOrg);         // restore the original flagline value
   }
   else
   {
      W_FlaglineOrg = gm_ReadRegWord(IPP_FLAGLINE);         // save the current flagline value
      gm_WriteRegWord(IPP_PIXGRAB_V, W_Ycord);            // co-ords of the pixel we want
      gm_WriteRegWord(IPP_PIXGRAB_H, W_Xcord);            // enable pixel grab
      gm_WriteRegWord(IPP_FLAGLINE, (W_Ycord + 1));         // wait for the correct line
      gm_SetRegBitsWord(IPP_CONTROL, IPP_PIXGRAB_EN);         // enable pixel grab

      gm_TimerStart(DELAY_10ms_TMR, 3);      // setup a timeout
      gm_SetRegBitsWord(PIP_INPUT_STATUS, IPP_LINEFLAG);      // clear IP_LINEFLAG

      if (gm_IsInterlaced(B_Channel))
      {
         // wait for this row done & an even field
         while (((gm_ReadRegWord(PIP_INPUT_STATUS) & IPP_LINEFLAG) == 0) || (gm_ReadRegWord(MISC_STATUS) & IPP_ODD_STATUS))
         {
            if (gm_TimerCheck(DELAY_10ms_TMR) == TIMER_TMO)
            {
               break;
            }
         }
      }
      else
      {
         // wait for this row done
         while ((gm_ReadRegWord(PIP_INPUT_STATUS) & IPP_LINEFLAG) == 0)
         {
            if (gm_TimerCheck(DELAY_10ms_TMR) == TIMER_TMO)
            {
               break;
            }
         }
      }

      if (B_Color == gmd_RED_COMPONENT)
         W_Retval = gm_ReadRegWord(IPP_PIXGRAB_RED);
      else if (B_Color == gmd_GREEN_COMPONENT)
         W_Retval = gm_ReadRegWord(IPP_PIXGRAB_GRN);
      else
         W_Retval = gm_ReadRegWord(IPP_PIXGRAB_BLU);

      // Do not disable pixel grab so we can get other color values after this function returns
      // Note: For FLI8668 disabling pixel grab causes the raw ADC data to be writen into the pixel grab result registers.
//      gm_ClearRegBitsWord(IPP_CONTROL, IPP_PIXGRAB_EN);
      gm_WriteRegWord(IPP_FLAGLINE, W_FlaglineOrg);         // restore the original flagline value
   }

   return W_Retval;
}

#define DEBUG_ENERGY_SMART 0
#if 1//120425 Edward test
void GetInputAverageLevel(void)
{  
  switch (PixelGrap_State)
   {
      case PixelGrap_START:
	  	{  		
	  		SAMPLE_Index = 0;
			DW_StoreR = DW_StoreG = DW_StoreB = 0;	  	
			PixelGrap_State++;
			#if 0//DEBUG_ENERGY_SMART  
			gm_Print("@1 %d",gm_GetSystemTime());
			#endif
      	}
	  	break;
      case PixelGrap_CONFIG:	  	
	  		{	
				//add SAMPLE_Index for get random pixel of x 
	  			X_Position = stInput[CH_A].Hstart + (SAMPLE_Index%X_Block)*(Block_OFFSET_X) + Block_OFFSET_X/2 + SAMPLE_Index;
				Y_Position = stInput[CH_A].Vstart + (SAMPLE_Index%Y_Block)*(Block_OFFSET_Y) + Block_OFFSET_Y/2;

				gm_WriteRegWord(IMP_PIXGRAB_V, Y_Position);            // co-ords of the pixel we want
         		gm_WriteRegWord(IMP_PIXGRAB_H, X_Position);
         		gm_WriteRegWord(IMP_FLAGLINE, (Y_Position + 1));         // wait for the correct line
         		gm_SetRegBitsWord(IMP_CONTROL, IMP_PIXGRAB_EN);         // enable pixel grab
         		gm_SetRegBitsWord(MAIN_INPUT_STATUS, IMP_LINEFLAG);      // clear IP_LINEFLAG        
         		PixelGrap_State++;
				//gm_Print("@2 X-->%d",X_Position);
				//gm_Print("@2 Y-->%d",Y_Position);
	  		}
			break;
	  case PixelGrap_WAIT:
	  	{				
	  		if (gm_ReadRegWord(MAIN_INPUT_STATUS) & IMP_LINEFLAG)
	   		{
		 		DW_StoreR += gm_ReadRegWord(IMP_PIXGRAB_RED);
         		DW_StoreG += gm_ReadRegWord(IMP_PIXGRAB_GRN);
         		DW_StoreB += gm_ReadRegWord(IMP_PIXGRAB_BLU);
				
				SAMPLE_Index++;
				if(SAMPLE_Index < Total_Block)					 			
					PixelGrap_State--;
				else
					PixelGrap_State++;
	   		}
	   		else
		  		return;		
	  	}
	     break;
		 
	   case PixelGrap_CALCULATE:	 
	   	{
			DW_StoreR = (DW_StoreR/((X_Block)*Y_Block))>>2;
  			DW_StoreG = (DW_StoreG/((X_Block)*Y_Block))>>2;
  			DW_StoreB = (DW_StoreB/((X_Block)*Y_Block))>>2;  
		#if 0 //(DEBUG_ENERGY_SMART == 1)  
			  gm_Print("R : %d",DW_StoreR);
			  gm_Print("G : %d",DW_StoreG);
			  gm_Print("B : %d",DW_StoreB);
		#endif  
			
			// Y709 = 0.183R + 0.614G + 0.062B + 16   (RGB 0 ~ 255 ==> Y 16-> 235)
  			B_YLevel = (BYTE)((DW_StoreR*183+DW_StoreG*614+DW_StoreB*62)/1000)+16;
		
			//temp for single color input
			if((DW_StoreR > 240 || (DW_StoreB > 240)) && (DW_StoreG < 20))
				B_YLevel = 	180;

			#if DEBUG_ENERGY_SMART 
  			gm_Print("B_YLevel : %d",B_YLevel);
			gm_Print("RLevel : %d",DW_StoreR);
			gm_Print("GLevel : %d",DW_StoreG);
			gm_Print("BLevel : %d",DW_StoreB);
			//gm_Print("@2 %d",gm_GetSystemTime());
			#endif
			
			PixelGrap_State++;
	   	}
	   break;

	  case PixelGrap_FINISHED:
	  	{
			//wait for next grap
	  	}
	  	break;
	  default:
	  	gm_Print("Pixel Grap Wrong state",0);
	  	break;
  	}	 
}

#else
#define X_SAMPLES 4 // 4 blocks sampling 2 pixels
#define Y_SAMPLES 4
#define X_STEP PanelWidth/X_SAMPLES
#define Y_STEP PanelHeight/Y_SAMPLES

BYTE GetInputAverageLevel(void)
{
  WORD X_Position = X_STEP/2, Y_Position = Y_STEP/2;
  DWORD DW_StoreR = 0, DW_StoreG = 0, DW_StoreB = 0;
  WORD W_XIndex,W_YIndex;
  BYTE B_YLevel;

  gm_InputWaitVsync(CH_A, 1);
  for(W_YIndex = 0; W_YIndex <Y_SAMPLES; W_YIndex++) //Line
  {
     if((W_YIndex%2) == 0) 
        Y_Position = Y_STEP/2; 
     else
        Y_Position = Y_STEP/2 + 9; 

     Y_Position += W_YIndex*Y_STEP;
     for(W_XIndex = 0; W_XIndex <(X_SAMPLES/2); W_XIndex++) //Pixel
     {
        X_Position = X_STEP/2;        
            if((W_YIndex%2) == 0) //Left
	            X_Position += W_XIndex*(X_STEP*2);
		else
	            X_Position += W_XIndex*(X_STEP*2)+X_STEP/2;			
	      //gm_Printf("X : %d, Y : %d",X_Position,Y_Position);		
            DW_StoreR += PixelGrab(CH_A, X_Position, Y_Position , gmd_RED_COMPONENT);
            DW_StoreG += gm_ReadRegWord(IMP_PIXGRAB_GRN);
            DW_StoreB += gm_ReadRegWord(IMP_PIXGRAB_BLU);		
            DW_StoreR += PixelGrab(CH_A, X_Position+9, Y_Position , gmd_RED_COMPONENT);
            DW_StoreG += gm_ReadRegWord(IMP_PIXGRAB_GRN);
            DW_StoreB += gm_ReadRegWord(IMP_PIXGRAB_BLU);				
	      //gm_Print("DW_StoreR : %d",DW_StoreR);
            //gm_Print("DW_StoreG : %d",DW_StoreG);
            //gm_Print("DW_StoreB : %d",DW_StoreB);  			
     }    	 
  }
  DW_StoreR = (DW_StoreR/((X_SAMPLES)*Y_SAMPLES))>>2;
  DW_StoreG = (DW_StoreG/((X_SAMPLES)*Y_SAMPLES))>>2;
  DW_StoreB = (DW_StoreB/((X_SAMPLES)*Y_SAMPLES))>>2;  
#if 0 //(DEBUG_ENERGY_SMART == 1)  
  gm_Print("R : %d",DW_StoreR);
  gm_Print("G : %d",DW_StoreG);
  gm_Print("B : %d",DW_StoreB);
#endif  

  // Y709 = 0.183R + 0.614G + 0.062B + 16   (RGB 0 ~ 255 ==> Y 16-> 235)
  B_YLevel = (BYTE)((DW_StoreR*183+DW_StoreG*614+DW_StoreB*62)/1000)+16;
  //gm_Print("B_YLevel : %d",B_YLevel);
  return B_YLevel;
}
#endif
//******************************************************************************/
//
// FUNCTION :   EnergySmartHandler
//
// USAGE   :   Adjust PWM depended on input signal level for power consumption purpose  
//
// INPUT   : None
//            
// OUTPUT   :   None
//
// GLOBALS   :   None
//
// USED_REGS:   None
//
// PRE_CONDITION:   None
//
// POST_CONDITION:   DCR will be turn off when EnergySmart turn on
// Input signal level range : 0 ~ 255
// level >= 200  : 15% power saving 
// level 200 ~ 100 : linear power saving between Min./Max.   
// level <= 100 :  40% power saving
//******************************************************************************
#if 1//owen 121017-1 add 30
void EnergySmartHandler(void)//CMI_73XX_BOARD
{
    WORD W_Imp_Brightness;
    //const BYTE Ba_EnergySmartLevel[6] = {0x4E,0x49,0x43,0x3D,0x36,0x30}; //Power saving 15%, 20%, 25%, 30%, 35%, 40%

    BYTE W_PWMTargetValue = 0;
	 
    if(UserPrefEnergySmartOnOffFlag == Off)
        return;

    //if (gm_TimerCheck(EnergySmart_100ms_TMR) != TIMER_TMO)
        //return;	
	
    if(IsNoSync() ||!gm_IsValidSignal(gmvb_MainChannel) || (B_SplashComplete == FALSE))
		return;	
	
	if(UserPrefPipMode != NO_PIP) //20120907 yvonne for PIP/PBP on EnergySmart must turn off
		return;

    //if(GetOSDQueueLen()) //OSD event is queued.
		//return;	

#if (DEBUG_ENERGY_SMART == 1)
    DW_Time1 = gm_GetSystemTime();
#endif

	GetInputAverageLevel();
	
	if (PixelGrap_State != PixelGrap_FINISHED)
		return;
	
	W_Imp_Brightness = B_YLevel;
	
    //if(IsAccEnablePresetMode())	 
       //W_Imp_Brightness = ReadInputAverageBrigttness();
    //else
       //W_Imp_Brightness = GetInputAverageLevel();

	
#if (DEBUG_ENERGY_SMART == 1)    
    //DW_Time2 = gm_GetSystemTime();    			
#endif
	
    if(W_Imp_Brightness>=MAX_ENERGY_Y) // 15%
    {
#if (DEBUG_ENERGY_SMART == 1)    
      gm_Print("EnergySmart  Level 0 : %d",W_Imp_Brightness);
#endif
      W_PWMTargetValue = ENERGY_SAVE_15;
    }
    else if((W_Imp_Brightness<MAX_ENERGY_Y) && (W_Imp_Brightness>MIN_ENERGY_Y))
    {
      W_PWMTargetValue = (WORD)((W_Imp_Brightness - MIN_ENERGY_Y)*(ENERGY_SAVE_15 - ENERGY_SAVE_40))/(MAX_ENERGY_Y-MIN_ENERGY_Y) + ENERGY_SAVE_40;
#if (DEBUG_ENERGY_SMART == 1)    
	  gm_Print("EnergySmart  Level 1 : %d",W_Imp_Brightness);
	  gm_Print("W_PWMTargetValue  : %d",W_PWMTargetValue);
#endif
    }    
    else //<= 100 40%	
    {
#if (DEBUG_ENERGY_SMART == 1)    
      gm_Print("EnergySmart  Level 6 : %d",W_Imp_Brightness);   
#endif
      W_PWMTargetValue = ENERGY_SAVE_40;
    }

    if ((DCR_EnergyUse_Brightness != W_PWMTargetValue))
    {
    
	#if 0//120726 Edward disable for speed up backlight adjust
	   gm_TimerStart(DELAY_1ms_TMR, 100);
		// clear VBLANK edge from previous frame
	   gm_WriteRegWord(MAIN_INPUT_STATUS, IMP_VACTIVE);
		while ((gm_ReadRegWord(MAIN_INPUT_STATUS) & IMP_VACTIVE) == 0)
		{
			if (gm_TimerCheck(DELAY_1ms_TMR) == TIMER_TMO)
			break;	// timeout - return with error
		}
	#endif	
	
	if(abs(W_PWMTargetValue-DCR_EnergyUse_Brightness)>0x06) //Speed Up
	{
	   if(W_PWMTargetValue>DCR_EnergyUse_Brightness)
	   	{
	   	   	#if (defined(CMI_73XX_BOARD)&&(PWM_LowLevel_Smooth))
	   		if(IS_ENERGY_SMART_ON())
	   		{			
				if(DCR_EnergyUse_Brightness < W_PWMTargetValue)  //hgh modify 20120919
				{	   			
	   				if(DCR_Energy_LowStep_Brightness < (DCR_EnergyUse_Brightness*FineStep_DCR_PWM))
						DCR_Energy_LowStep_Brightness++;
					else if(DCR_Energy_LowStep_Brightness > (DCR_EnergyUse_Brightness*FineStep_DCR_PWM))	
						DCR_Energy_LowStep_Brightness--;
					else
						{
							DCR_EnergyUse_Brightness += 1;	
							DCR_Energy_LowStep_Brightness++;
						}
				}
				else
					DCR_EnergyUse_Brightness += 1;
	   		}	
			else
			#endif
	   			DCR_EnergyUse_Brightness += 2;
			
            AdjustBackLight();			
			#if (DEBUG_ENERGY_SMART == 1)				
				gm_Print("EnergySmart LowStep_Brightness++++ : %d",DCR_Energy_LowStep_Brightness); 
				gm_Print("EnergySmart Brightness+2 : %d",DCR_EnergyUse_Brightness);	
			#endif
	   	}	
	   else
	   	{
	   		#if (defined(CMI_73XX_BOARD)&&(PWM_LowLevel_Smooth))
	   		if(IS_ENERGY_SMART_ON())
	   		{
				if(DCR_EnergyUse_Brightness < WB_Brigntness_OSD_DCR_Start)
				{	   			
	   				if(DCR_Energy_LowStep_Brightness < (DCR_EnergyUse_Brightness*FineStep_DCR_PWM))
						DCR_Energy_LowStep_Brightness++;
					else if(DCR_Energy_LowStep_Brightness > (DCR_EnergyUse_Brightness*FineStep_DCR_PWM))	
						DCR_Energy_LowStep_Brightness--;
					else
					{
						DCR_EnergyUse_Brightness -= 1;
						DCR_Energy_LowStep_Brightness--;
					}
				}
				else
					DCR_EnergyUse_Brightness -= 1;

				if(DCR_EnergyUse_Brightness == WB_Brigntness_OSD_DCR_Start)
				  DCR_Energy_LowStep_Brightness = (DCR_EnergyUse_Brightness*FineStep_DCR_PWM);
	   		}	
			else
			#endif	
            	DCR_EnergyUse_Brightness -= 2;
					
            AdjustBackLight();
			#if (DEBUG_ENERGY_SMART == 1)			
			gm_Print("EnergySmart LowStep_Brightness---- : %d",DCR_Energy_LowStep_Brightness); 
			gm_Print("EnergySmart Brightness-2 : %d",DCR_EnergyUse_Brightness);	
			#endif
	   	}	
	}
	else
	{
	   if(W_PWMTargetValue>DCR_EnergyUse_Brightness)
	   	{
	   	   	#if (defined(CMI_73XX_BOARD)&&(PWM_LowLevel_Smooth))
	   		if(IS_ENERGY_SMART_ON())
	   		{
				if(DCR_EnergyUse_Brightness < W_PWMTargetValue)  //hgh modify 20120919
				{	   			
	   				if(DCR_Energy_LowStep_Brightness < (DCR_EnergyUse_Brightness*FineStep_DCR_PWM))
						DCR_Energy_LowStep_Brightness++;
					else if(DCR_Energy_LowStep_Brightness > (DCR_EnergyUse_Brightness*FineStep_DCR_PWM))	
						DCR_Energy_LowStep_Brightness--;
					else
					{
						DCR_EnergyUse_Brightness += 1;
						DCR_Energy_LowStep_Brightness++;
					}
				}
				else
					DCR_EnergyUse_Brightness += 1;
	   		}	
			else
			#endif	
				   DCR_EnergyUse_Brightness += 1;
			
            AdjustBackLight();  
			#if (DEBUG_ENERGY_SMART == 1)			
			gm_Print("EnergySmart LowStep_Brightness++ : %d",DCR_Energy_LowStep_Brightness); 
			gm_Print("EnergySmart Brightness+1 : %d",DCR_EnergyUse_Brightness);	
			#endif
	   	}	
	   else
	   	{
	   	   	#if (defined(CMI_73XX_BOARD)&&(PWM_LowLevel_Smooth))
	   		if(IS_ENERGY_SMART_ON())
	   		{
				if(DCR_EnergyUse_Brightness < WB_Brigntness_OSD_DCR_Start)
				{	   			
	   				if(DCR_Energy_LowStep_Brightness < (DCR_EnergyUse_Brightness*FineStep_DCR_PWM))
						DCR_Energy_LowStep_Brightness++;
					else if(DCR_Energy_LowStep_Brightness > (DCR_EnergyUse_Brightness*FineStep_DCR_PWM))	
						DCR_Energy_LowStep_Brightness--;
					else
					{
						DCR_EnergyUse_Brightness -= 1;
						DCR_Energy_LowStep_Brightness--;
					}
				}
				else
					DCR_EnergyUse_Brightness -= 1;

				if(DCR_EnergyUse_Brightness == WB_Brigntness_OSD_DCR_Start)
				 DCR_Energy_LowStep_Brightness = (DCR_EnergyUse_Brightness*FineStep_DCR_PWM);
	   		}	
			else
			#endif	
            		DCR_EnergyUse_Brightness -= 1;
			
            AdjustBackLight();  
			#if (DEBUG_ENERGY_SMART == 1)			
			gm_Print("EnergySmart LowStep_Brightness-- : %d",DCR_Energy_LowStep_Brightness); 
			gm_Print("EnergySmart Brightness-1 : %d",DCR_EnergyUse_Brightness);	
			#endif
	   	}
	}		
		
   }	
   //gm_TimerStart(EnergySmart_100ms_TMR, 2);	
   
   PixelGrap_State = PixelGrap_START;//restart capture
}

#else
#if (defined(Qisda_93XX_BOARD))
#define EnergySmartSmooth //20121017 Add by KevinChen
#define InputDataDelta 50
#define NextDataStepCount 200
#endif
void EnergySmartHandler(void)//Qisda + Wistron U29
{
    WORD W_Imp_Brightness;
    BYTE B_PwmPulse;   
#ifdef EnergySmartSmooth
	static BYTE CheckCounter=0;
	static WORD W_Imp_Brightness_Last=0;
#endif
	#if (DEFAULT_PANEL == LG_WHD_LM290WW1_SSA1)
     const BYTE Ba_EnergySmartLevel[6] = {123,112,101,91,80,68};	//<- default brightness value 75	//{212,198,182,168,152,135}; <- max brightness value 100

    #elif (DEFAULT_PANEL == LG_WQXGA_LM300WQ5_SLA1)
     const BYTE Ba_EnergySmartLevel[6] = {75, 66, 59, 52, 45, 38};//{112, 100, 88, 76, 64, 52};
    #elif ((DEFAULT_PANEL == LG_WQHD_LM270WQ1)||(DEFAULT_PANEL == LG_WUXGA_LM240WU9_SLA1))
	    #ifdef LGD_HBar    //modified for energy smart new spec with LGDHBar;20120922;Barbara Lin
	        const BYTE Ba_EnergySmartLevel[6] = {0x2B,0x28,0x22,0x1E,0x19,0x15}; //Power saving 15%, 20%, 25%, 30%, 35%, 40%
	    #else
	    const BYTE Ba_EnergySmartLevel[6] = {0x60,0x58,0x50,0x47,0x3F,0x37}; //Power saving 15%, 20%, 25%, 30%, 35%, 40%
	    #endif

	#else
	const BYTE Ba_EnergySmartLevel[6] = {0x60,0x58,0x50,0x47,0x3F,0x37}; //Power saving 15%, 20%, 25%, 30%, 35%, 40%
	
    #endif
    WORD W_PWMTargetValue = 0;
	 
    if(UserPrefEnergySmartOnOffFlag == Off)
        return;

    //if (gm_TimerCheck(EnergySmart_100ms_TMR) != TIMER_TMO)
        //return;	
	
    if(IsNoSync() ||!gm_IsValidSignal(gmvb_MainChannel) || (B_SplashComplete == FALSE))
		return;	
	
	if(UserPrefPipMode != NO_PIP) //20120907 yvonne for PIP/PBP on EnergySmart must turn off
		return;
	

    //if(GetOSDQueueLen()) //OSD event is queued.
		//return;	

#if (DEBUG_ENERGY_SMART == 1)
    DW_Time1 = gm_GetSystemTime();
#endif

	GetInputAverageLevel();
	
	if (PixelGrap_State != PixelGrap_FINISHED)
		return;

	W_Imp_Brightness = B_YLevel;
	
#ifdef EnergySmartSmooth
	if(abs(W_Imp_Brightness-W_Imp_Brightness_Last)<InputDataDelta)
	{
		if(CheckCounter<NextDataStepCount)
		{
			CheckCounter++;
			return;
		}
		else
		{
			//gm_Printf("new 0x%x last 0x%x < 10",W_Imp_Brightness,W_Imp_Brightness_Last);
			//gm_Print("counter - %d >10",CheckCounter);
			CheckCounter=0;
			if(W_Imp_Brightness<W_Imp_Brightness_Last)
			{
				W_Imp_Brightness_Last--;
				W_Imp_Brightness=W_Imp_Brightness_Last;
			}
			else if(W_Imp_Brightness>W_Imp_Brightness_Last)
			{
				W_Imp_Brightness_Last++;
				W_Imp_Brightness=W_Imp_Brightness_Last;
			}
			else
			{
			
			}
		}
	}
	else
	{//gm_Printf("new 0x%x last 0x%x > 10",W_Imp_Brightness,W_Imp_Brightness_Last);
		CheckCounter=0;
		W_Imp_Brightness_Last=W_Imp_Brightness;
	}
#endif
	
    //if(IsAccEnablePresetMode())	 
       //W_Imp_Brightness = ReadInputAverageBrigttness();
    //else
       //W_Imp_Brightness = GetInputAverageLevel();

	
#if (DEBUG_ENERGY_SMART == 1)    
    //DW_Time2 = gm_GetSystemTime();    			
#endif

#if BACKLIGHT_PWM_INVERT
   B_PwmPulse = 0xFF-ReadBlueBacklightPWMPulse();	
#else
    B_PwmPulse = ReadBlueBacklightPWMPulse();	
#endif

    if(W_Imp_Brightness>=200) // 15%
    {
#if (DEBUG_ENERGY_SMART == 1)    
      gm_Print("EnergySmart  Level 0 : %d",W_Imp_Brightness);
#endif
      W_PWMTargetValue = Ba_EnergySmartLevel[0];
    }
    else if(W_Imp_Brightness>=180)
    {
#if (DEBUG_ENERGY_SMART == 1)    
      gm_Print("EnergySmart  Level 1 : %d",W_Imp_Brightness);
#endif
      W_PWMTargetValue = (WORD)((W_Imp_Brightness - 180)*(Ba_EnergySmartLevel[0] - Ba_EnergySmartLevel[1]))/20 + Ba_EnergySmartLevel[1];
    }
    else if(W_Imp_Brightness>=160)
    {
#if (DEBUG_ENERGY_SMART == 1)    
      gm_Print("EnergySmart  Level 2 : %d",W_Imp_Brightness); 
#endif
      W_PWMTargetValue = (WORD)((W_Imp_Brightness - 160)*(Ba_EnergySmartLevel[1] - Ba_EnergySmartLevel[2]))/20 + Ba_EnergySmartLevel[2];    
    }
    else if(W_Imp_Brightness>=140)
    {
#if (DEBUG_ENERGY_SMART == 1)    
      gm_Print("EnergySmart  Level 3 : %d",W_Imp_Brightness);   
#endif
      W_PWMTargetValue = (WORD)((W_Imp_Brightness - 140)*(Ba_EnergySmartLevel[2] - Ba_EnergySmartLevel[3]))/20 + Ba_EnergySmartLevel[3];    
    }
    else if(W_Imp_Brightness>=120)
    {
#if (DEBUG_ENERGY_SMART == 1)    
      gm_Print("EnergySmart  Level 4 : %d",W_Imp_Brightness);  
#endif
      W_PWMTargetValue = (WORD)((W_Imp_Brightness - 120)*(Ba_EnergySmartLevel[3] - Ba_EnergySmartLevel[4]))/20 + Ba_EnergySmartLevel[4];       
    }
    else if(W_Imp_Brightness>100)
    {
#if (DEBUG_ENERGY_SMART == 1)    
      gm_Print("EnergySmart  Level 5 : %d",W_Imp_Brightness);
#endif
      W_PWMTargetValue = (WORD)((W_Imp_Brightness - 100)*(Ba_EnergySmartLevel[4] - Ba_EnergySmartLevel[5]))/20 + Ba_EnergySmartLevel[5];       
    }
    else //<= 100 40%	
    {
#if (DEBUG_ENERGY_SMART == 1)    
      gm_Print("EnergySmart  Level 6 : %d",W_Imp_Brightness);   
#endif
      W_PWMTargetValue = Ba_EnergySmartLevel[5];
    }

    if ((B_PwmPulse != W_PWMTargetValue))
    {
#if (DEBUG_ENERGY_SMART == 1)    
       //gm_Print("EnergySmart  W_PWMTargetValue : 0x%x",W_PWMTargetValue);  	
       //gm_Print("Get Input Level time : %d ms",(DW_Time2-DW_Time1));		
#endif
       gm_TimerStart(DELAY_1ms_TMR, 100);
	// clear VBLANK edge from previous frame
	gm_WriteRegWord(MAIN_INPUT_STATUS, IMP_VACTIVE);
	while ((gm_ReadRegWord(MAIN_INPUT_STATUS) & IMP_VACTIVE) == 0)
	{
		if (gm_TimerCheck(DELAY_1ms_TMR) == TIMER_TMO)
		break;	// timeout - return with error
	}
	
	if(abs(W_PWMTargetValue-B_PwmPulse)>0x0A) //Speed Up
	{
	   if(W_PWMTargetValue>B_PwmPulse)
	   	{
	          #if BACKLIGHT_PWM_INVERT
			  SetBlueBacklightPWMPulse(0xFF-(B_PwmPulse+2));	
		         SetGreenBacklightPWMPulse(0xFF-(B_PwmPulse+2));
		    #else
			SetBlueBacklightPWMPulse(B_PwmPulse+2);	
		        SetGreenBacklightPWMPulse(B_PwmPulse+2);	
		    #endif	
	   	}	
	   else
	   	{
	               #if BACKLIGHT_PWM_INVERT
				 SetBlueBacklightPWMPulse(0xFF-(B_PwmPulse-2));
			        SetGreenBacklightPWMPulse(0xFF-(B_PwmPulse-2));
		        #else
				SetBlueBacklightPWMPulse(B_PwmPulse-2);
			        SetGreenBacklightPWMPulse(B_PwmPulse-2);
		        #endif
	   	}	
	}
	else
	{
	   if(W_PWMTargetValue>B_PwmPulse)
	   	{
			#if BACKLIGHT_PWM_INVERT
			SetBlueBacklightPWMPulse(0xFF-(B_PwmPulse+1));	  	
			SetGreenBacklightPWMPulse(0xFF-(B_PwmPulse+1));
			#else
			SetBlueBacklightPWMPulse(B_PwmPulse+1);	  	
			SetGreenBacklightPWMPulse(B_PwmPulse+1);
			#endif	
	   	}	
	   else
	   	{
			#if BACKLIGHT_PWM_INVERT
			SetBlueBacklightPWMPulse(0xFF-(B_PwmPulse-1));	  
			SetGreenBacklightPWMPulse(0xFF-(B_PwmPulse-1));	  	
			#else
			SetBlueBacklightPWMPulse(B_PwmPulse-1);	  
			SetGreenBacklightPWMPulse(B_PwmPulse-1);	  
			#endif
	   	}
	}
   }	
   //gm_TimerStart(EnergySmart_100ms_TMR, 2);	
   
   PixelGrap_State = PixelGrap_START;//restart capture
}
#endif//end of different board

#define ENERGY_LEVEL 20
void EnergyUsedLevelHandler(void)
{
    BYTE B_EnergyUsedLevel = 0;
    WORD W_PWMValue = 0; 	
    WORD W_TargetEULevel = 0;	

	WORD MAX_BRIGHTNESS_VALUE , MIN_BRIGHTNESS_VALUE; 

    //if(!gm_IsValidSignal(gmvb_MainChannel))
    if(IsNoSync())
		return;	

	#if (defined(Athena_C))
		MAX_BRIGHTNESS_VALUE = WB_BRIGHTNESS_MAX;
		MIN_BRIGHTNESS_VALUE = WB_BRIGHTNESS_MIN;
		W_PWMValue = ((IS_DCR_ON()|| IS_ENERGY_SMART_ON())? DCR_EnergyUse_Brightness : UserPrefBrightness);
	#else
	    	MAX_BRIGHTNESS_VALUE = MAX_BACKLIGHT_PWM;
		MIN_BRIGHTNESS_VALUE = MIN_BACKLIGHT_PWM;
    		W_PWMValue = ReadBlueBacklightPWMPulse();

		#if BACKLIGHT_PWM_INVERT
			W_PWMValue = 0xFF - W_PWMValue;	//Invert Energy Smart Level Value
		#endif
	#endif
	
    if(UserPrefEnergySmartOnOffFlag == On) //Energy Smart
    {
    	if(W_PWMValue <= MAX_ENERGY_USE_PWM)	
    	{
      	   if(W_PWMValue <= MIN_ENERGY_USE_PWM)
	      	B_EnergyUsedLevel = 0;
	   	   else
	      {
	      	W_TargetEULevel = (WORD)((W_PWMValue - MIN_ENERGY_USE_PWM) *ENERGY_LEVEL);
	      	B_EnergyUsedLevel = (BYTE)(W_TargetEULevel/(MAX_ENERGY_USE_PWM - MIN_ENERGY_USE_PWM));
	   	  }
       }
       else
          B_EnergyUsedLevel = ENERGY_LEVEL;
    }
    else //DCR/Brightness
    {
       if(W_PWMValue <= MAX_BRIGHTNESS_VALUE)	
       {
          if(W_PWMValue <= MIN_BRIGHTNESS_VALUE)
	       B_EnergyUsedLevel = 0;
	   else
	   {
	      W_TargetEULevel = (WORD)((W_PWMValue - MIN_BRIGHTNESS_VALUE) *ENERGY_LEVEL);
	      B_EnergyUsedLevel = (BYTE)(W_TargetEULevel/(MAX_BRIGHTNESS_VALUE - MIN_BRIGHTNESS_VALUE));	
		  if(B_EnergyUsedLevel==0) B_EnergyUsedLevel=1;//20120820 Add by KevinChen for at least one level show PWM still using.
	   }
       }
       else
          B_EnergyUsedLevel = ENERGY_LEVEL;
    }	
	
    if(UserPrefEnergyUsedLevel != B_EnergyUsedLevel)
    {
       //gm_Print("B_EnergyUsedLevel : %d",B_EnergyUsedLevel);
       UserPrefEnergyUsedLevel = B_EnergyUsedLevel;
	 if(UserPrefMenuOnOffFlag == On)
	    OSDSendMessage(MTO_UpdateEnergyStar);	   	   
    }
}

void AdjustEnergySmart(void)
{
   if(UserPrefEnergySmartOnOffFlag == Off)
   {
       if(!IS_DCR_ON())
   	   	AdjustBackLight();
   }
   else//120425 Edward test
   	{
		PixelGrap_State = PixelGrap_START;
		
		#if defined(CMI_73XX_BOARD)//120628 Edward for energy smart adjust for smooth backlight change
			DCR_EnergyUse_Brightness = UserPrefBrightness;	 

			#if PWM_LowLevel_Smooth
			if(IS_ENERGY_SMART_ON()) //20120913 Edward
		  	{
		  		if(DCR_EnergyUse_Brightness <= WB_Brigntness_OSD_DCR_Start)
					DCR_Energy_LowStep_Brightness = DCR_EnergyUse_Brightness*FineStep_DCR_PWM;
			
				//gm_Print("##DCR_EnergyUse_Brightness  %d",DCR_EnergyUse_Brightness);
		  	}
			#endif
		#endif

		#if (defined(WISTRON_93XX_BOARD_30) && WST_P3_BAORD_30 == 0)//owen 121114 
		DCR_EnergyUse_Brightness = UserPrefBrightness;
		#endif

		
   	}
   	
}
void NoCablePowerDownHandler(void)
{
#if(NO_CABLE_COUNT_DOWN == ENABLE)
      if(UserPrefNoCablePowerDownFlag == Off)      	      	    	
	  	return;      	
	  
	if (gm_TimerCheck(NoCablePowerDown_100ms_TMR) != TIMER_TMO)
		return;

        if(UserPrefNoCablePowerDownTime >= NoCableCNT)
	    return; 		
	  else		
           UserPrefNoCablePowerDownTime++;
	gm_TimerStart(NoCablePowerDown_100ms_TMR, 10);		//Neil 20120607* Org = 9*100*300 = 270000ms = 4.5min,Spec need 5minute
	//gm_Print("Power Down Time up to 300 : %d",UserPrefNoCablePowerDownTime);
      OSDSendMessage(MTO_NoCableTimerUpdate);//Updated OSD
#endif      

}
void StartNoCablePowerDownCounter(void)
{	
#if(defined(BOARD_93XX_MNT))
	NO_CABLE_SLEEP = gmd_FALSE;
#endif
	#if(NO_CABLE_COUNT_DOWN == ENABLE)
    UserPrefNoCablePowerDownFlag = On;
	UserPrefNoCablePowerDownMins = 5;
	UserPrefNoCablePowerDownSecs = 0;	 
	UserPrefNoCablePowerDownTime = 0;
	gm_TimerStart(NoCablePowerDown_100ms_TMR, 10);
	#endif
}

void StopNoCablePowerDownCounter(void)
{
	#if(NO_CABLE_COUNT_DOWN == ENABLE)
	UserPrefNoCablePowerDownMins = 5;
	UserPrefNoCablePowerDownSecs = 0;
	UserPrefNoCablePowerDownTime = 0;
	gm_TimerStop(NoCablePowerDown_100ms_TMR);
	UserPrefNoCablePowerDownFlag = Off;
	#endif
}

void UpdateOSDMenuRotationPosition(void) //Use in WB
{
    if(UserPrefOsdRotation == OSD_NON_ROTATED)
    {
          UserPrefOSDMenuLocation = BOTTOM_RIGHT;
    }
    else if(UserPrefOsdRotation == OSD_ROTATED_90)	
    {
          UserPrefOSDMenuLocation = BOTTOM_LEFT;
    }
}

void AdjustColorUniformity(void)
{
	 Color_Uniformity_Test(UserPrefDUMSwitch);
}

void ToggleMenuRotation(void)
{
	  //UpdateOSDMenuRotationPosition();    

	  if((UserPrefOsdRotation == OSD_NON_ROTATED) && (UserPrefGlobalRotationEnabled== FALSE)) //ShortCut
	  {
   	      UserPrefGlobalRotationEnabled = TRUE;
	      UserPrefOsdRotation = OSD_ROTATED_90;		  
	  }
	  else
	  {
   	      UserPrefGlobalRotationEnabled = FALSE;	  
	  	UserPrefOsdRotation = OSD_NON_ROTATED;
	  }
	  UpdateOSDMenuRotationPosition();  //Neil 20110808* move down here,UserPrefOsdRotation changed everywhere will impact this functions 
         SaveModeIndependentSettings();
	  AdjustOsdRotation();  
}


//Keep this on buttom
void DrawDynamicText1(void) //Neil 20090204 DELL Version
{	
	/*
	DynamicText[0] = 'M';
	DynamicText[1] = '1';
	DynamicText[2] = 'F';
	DynamicText[3] = '1';
	DynamicText[4] = '9';
	DynamicText[5] = '4';
#ifdef VERSION_PLUS
	DynamicText[6] = 'C';
#endif
	versiontemp = DynamicText[4] - 0x30;
	if ((DynamicText[5] >= 'A') && (DynamicText[5] <= 'Z'))
		  versiontemp2 = DynamicText[5] - 0x37;
	else if ((DynamicText[5] >= '0') && (DynamicText[5] <= '9'))
		  versiontemp2 = DynamicText[5] - 0x30;

#ifdef VERSION_PLUS	// Increased by ST_hansPENG 090805
	if ((DynamicText[6] >= 'a') && (DynamicText[6] <= 'z'))
		DynamicText[6] -= 1;
#endif

	versionnumber = ((versiontemp << 8) | versiontemp2);
	*/

	
	if (!DDCCIRunFlag)
		DrawDynamicTextControl(TEXT_COLOR_STATE);
}

//#ifdef DELL_U2410F 
void ReadFactorySettings(void)
{
	NVRam_ReadBuffer(FactorySettingStart, (BYTE *)&FactoryUserPreferences, FactoryUserPrefSize);
}
//#endif


#if COLOR_UNIFORMITY_DEMO//110831 Edward for demo
void Color_Uniformity_Test(BOOL On_Off)
{

	WORD DUM_XGRID_SIZE1, DUM_YGRID_SIZE1, DUM_NUM_XGRIDS1, DUM_NUM_YGRIDS1, DUM_RSTART_XGRID1;
	//DWORD DUM_XGRID_IINC1, DUM_YGRID_IINC1;

	if (On_Off == On)
	{
		//internal pattern with white screen
		gm_WriteRegWord(IMP_PATGEN_BLUE , 0xFF);
		gm_WriteRegWord(IMP_PATGEN_GRN, 0xFF);
		gm_WriteRegWord(IMP_PATGEN_RED, 0xFF);
		gm_WriteRegWord(IMP_PATGEN_CONTROL, 0xE0);
					
		//data from 0 ~ 0x1FFF			
		//_fmemset(LINEAR_TO_FPTR(0x2E000UL) ,0x00,8192);
		*(WORD far *)LINEAR_TO_FPTR(0x2E00EUL) = 0x1FFF;	
		*(WORD far *)LINEAR_TO_FPTR(0x2E000UL) = 0x1EFF;	
		*(WORD far *)LINEAR_TO_FPTR(0x2E002UL) = 0x1DFF;	
		*(WORD far *)LINEAR_TO_FPTR(0x2E004UL) = 0x1CFF;	
		*(WORD far *)LINEAR_TO_FPTR(0x2F00EUL) = 0x1BFF;	
		*(WORD far *)LINEAR_TO_FPTR(0x2F000UL) = 0x1AFF;	
		*(WORD far *)LINEAR_TO_FPTR(0x2F002UL) = 0x19FF;	
		*(WORD far *)LINEAR_TO_FPTR(0x2F004UL) = 0x18FF;	
		*(WORD far *)LINEAR_TO_FPTR(0x2E016UL) = 0x17FF;	
		*(WORD far *)LINEAR_TO_FPTR(0x2E008UL) = 0x16FF;	
		*(WORD far *)LINEAR_TO_FPTR(0x2E00AUL) = 0x15FF;	
		*(WORD far *)LINEAR_TO_FPTR(0x2E00CUL) = 0x14FF;	
		*(WORD far *)LINEAR_TO_FPTR(0x2F016UL) = 0x13FF;	
		*(WORD far *)LINEAR_TO_FPTR(0x2F008UL) = 0x12FF;	
		*(WORD far *)LINEAR_TO_FPTR(0x2F00AUL) = 0x11FF;	
		*(WORD far *)LINEAR_TO_FPTR(0x2F00CUL) = 0x10FF;	

		//default 4x4 for demo			
		DUM_NUM_XGRIDS1 = 4;// (WORD)GetParam(0); //16;
		DUM_NUM_YGRIDS1 = 4;//(WORD)GetParam(1); //16;
	
		DUM_XGRID_SIZE1 = (PanelWidth / DUM_NUM_XGRIDS1)+1;
		DUM_YGRID_SIZE1 = (PanelHeight / DUM_NUM_YGRIDS1)+1;		
			
		//DUM_XGRID_IINC1 = (DWORD)(524288 / DUM_XGRID_SIZE1);
		//DUM_YGRID_IINC1 = (DWORD)(524288 / DUM_YGRID_SIZE1);
	
		DUM_RSTART_XGRID1 = (DUM_NUM_XGRIDS1 / 2);
					
		gm_WriteRegWord(C9VODP_DUM_CONTROL_0, (DUM_ENABLE|DUM_GRID_DOTS)); 
							
		gm_WriteRegDWord_LE(C9VODP_DUM_NUM_GRIDS, (DWORD)(DUM_NUM_YGRIDS1 << 9)|DUM_NUM_XGRIDS1);
		gm_WriteRegDWord_LE(C9VODP_DUM_GRID_SZ, ((DWORD)DUM_YGRID_SIZE1 << 12)|(DUM_XGRID_SIZE1));
		gm_WriteRegWord(C9VODP_DUM_GLOBAL0, 0xFFFF);
		gm_WriteRegWord(C9VODP_DUM_GLOBAL0 + 2, 0x03FF);		   
		gm_WriteRegWord(C9VODP_DUM_GLOBAL1, 0x1FFF);
		gm_WriteRegDWord_LE(C9VODP_DUM_XGRID_IINC, 0);
		gm_WriteRegDWord_LE(C9VODP_DUM_YGRID_IINC, 0);
		gm_WriteRegWord(C9VODP_DUM_RSTART, DUM_RSTART_XGRID1);
	}
	else
	{
		gm_WriteRegWord(IMP_PATGEN_BLUE , 0);
		gm_WriteRegWord(IMP_PATGEN_GRN, 0);
		gm_WriteRegWord(IMP_PATGEN_RED, 0);
		gm_WriteRegWord(IMP_PATGEN_CONTROL, 0);
		gm_WriteRegWord(C9VODP_DUM_CONTROL_0, 0); 
		
	}

}
#endif

#if (BOARD_USED == DELL_93xx_BOARD)
BOOL IsUSBConnected(void)	
{
	WORD Msg[3];

	Msg[1] = LPM_GPADC_GPIO_CONTROL;
	Msg[2] = GPADC_ADC0IN_DIS;
	
	//Neil 20120503+ don't know why need toggle this bit, otherwise can't get input correctly
	Msg[0] = Set_bit;
	gm_MessageSend(gmd_MESSAGE_LPM_GPIO_CTRL, Msg, sizeof(Msg));
	
	gm_Delay10ms(1);
	Msg[0] = Clear_bit;	
	gm_MessageSend(gmd_MESSAGE_LPM_GPIO_CTRL, Msg, sizeof(Msg));
	
	if((gm_ReadRegWord(LPM_GPADC_GPINPUT) & USB_DET_PIN) == USB_DET_PIN)
		return gmd_TRUE;
	else
		return gmd_FALSE;

}



void USB_Enable(void)	
{
	if(!IsUSBConnected())
		gm_ClearRegBitsWord(GPOUTPUT4,USB_EN_PIN);
	else
		gm_SetRegBitsWord(GPOUTPUT4,USB_EN_PIN);
}

void PSV_CTRL(void)
{
	WORD msg_data[3];

	msg_data[1] = LPM_GPOUTPUT_0;
	msg_data[2] = LPM_GPIO9_OUT;

	if(PwrState == Pwr_DeepSleep)
	{
		if(!IsUSBConnected()&&!IsAUDIO_Detected())
		{
			msg_data[0] = Clear_bit;
			gm_ClearRegBitsWord(LPM_GPOUTPUT_0,PSV_PIN);
		}
		else
		{
			msg_data[0] = Set_bit;
			gm_SetRegBitsWord(LPM_GPOUTPUT_0,PSV_PIN);
		}
	}
	else if(PwrState == Pwr_Off)
	{
		msg_data[0] = Clear_bit;
		gm_ClearRegBitsWord(LPM_GPOUTPUT_0,PSV_PIN);
	}
	else
	{
		msg_data[0] = Set_bit;
		gm_SetRegBitsWord(LPM_GPOUTPUT_0,PSV_PIN);
	}

	gm_MessageSend(gmd_MESSAGE_LPM_GPIO_CTRL, msg_data, sizeof(msg_data));
}

void AudioPowerCtrlHandler(void)
{
	if(AudioDetect())
	{
		//120824 Edward for fake sleep need to control audio power by OSD setting 
		if((GetPowerState() == Pwr_DeepSleep) && UserPrefPwrAudio == ON)
		 	AUDIO_POWER_DISABLE();
		else
			AUDIO_POWER_ENABLE();
	}	
	else
		AUDIO_POWER_DISABLE();

}
#endif	//(BOARD_USED == DELL_93xx_BOARD)

void ConvertDynamicString(char *c_DString)
{
	BYTE B_StringLen,i;

	B_StringLen = strlen(c_DString);

/*
	for(B_StringLen=0;B_StringLen<D_STRING_LEN;B_StringLen++)
	{
		if((c_DString[B_StringLen]) == '\0')
			break;
	}
*/
		//gm_Print("## B_StringLen = %d",B_StringLen);

	for(;B_StringLen<D_STRING_LEN;B_StringLen++)
	{
		c_DString[B_StringLen] = ' ';
	}

	c_DString[D_STRING_LEN] = '\0';

}

//120517 Edward for version display
//	char C_Firmware_Date[] 		= __DATE__;
char CustomApp_DateTime[] 		= CustomApp_FW_Date;//"20130113";    
char CustomApp_FWVersion[] 		= CustomApp_FW_Version;


//#define PANEL_NAME				c_Panel_Name
char Mode_Name[] 				= ModeName;
#if(defined(BOARD_93XX_PD))
#define Mode_Name_NoBland			"PDisplay"
#elif(defined(BOARD_93XX_OV))
#define Mode_Name_NoBland			"LoopView"
#elif(defined(BOARD_93XX_SUV))
#define Mode_Name_NoBland			"SUV"
#elif(defined(BOARD_93XX_4K2K))
#define Mode_Name_NoBland			"4K2K"
#else
#define Mode_Name_NoBland			"MNT"
#endif

char Mode_Name_NB[]				= Mode_Name_NoBland;

extern char ApplicationName[];
static char C_DStringpPtrTemp[D_STRING_LEN] = "";
extern BYTE code DpEdidExample[];
void SetDynamicStringPtr(OSD_DynamicDisplayType Ptr_Type)
{	
	switch(Ptr_Type)
	{
		case Ptr_Date:
			strcpy(C_DStringpPtrTemp,CustomApp_DateTime);
		break;
		
		case Ptr_FWVersion:
			strcpy(C_DStringpPtrTemp,CustomApp_FWVersion);	
		break;
		
		case Ptr_PanelName:
	     	_fmemset((void far*)(C_DStringpPtrTemp), ' ',sizeof(C_DStringpPtrTemp));
    		_fmemcpy((void far*)(C_DStringpPtrTemp),  PanelName, sizeof(C_DStringpPtrTemp));
		break;
		
		case Ptr_ModelName:
			#if(CUSTOMER_INFO == ENABLE)
			if(UserPrefLOGO_Ctrl == ON)
				strcpy(C_DStringpPtrTemp,Mode_Name);
			else
			{
				strcpy(C_DStringpPtrTemp,Mode_Name_NB);
			}
			#endif
		break;
		
		case Ptr_BoardID:
			strcpy(C_DStringpPtrTemp,ApplicationName);
		break;
		
		case Ptr_SerialNo:
			#if(DP_EDID_LOCATION == DP_EDID_FROM_NVRAM)
			NVRam_ReadBuffer(DpEdidTableStart+0x4D, (BYTE*)C_DStringpPtrTemp, 13); //Read from DP EDID serial nubmer
			#else
    		_fmemcpy((void far*)(C_DStringpPtrTemp),  DpEdidExample+0x4D, 13);
			#endif
			   //ReadMonitorSerialNumberfromNVRAM((BYTE *)C_DStringpPtrTemp);
			   C_DStringpPtrTemp[13] = '\0';
		break;
	
		default:
		break;

	}

	ConvertDynamicString(C_DStringpPtrTemp);
	UserPrefPtrSegmentDynamicString	= FP_SEG(C_DStringpPtrTemp);
	UserPrefPtrOffsetDynamicString	= FP_OFF(C_DStringpPtrTemp);

}

//******************************************************************************
//
// FUNCTION       : static BOOL IsTimingOutOfRange(Void)
//
// USAGE          : Determine timing is out of range or not
//
// INPUT          : B_Channel
//                   
//
// OUTPUT         : gmd_TRUE/gmd_FALSE
//
// GLOBALS        : 
//
// USED_REGS      : 
//
// PRE_CONDITION  : None
//
// POST_CONDITION : None
//
//******************************************************************************
BOOL IsTimingOutOfRange(gmt_PHY_CH B_Channel)//Neil120615* update for DELL new timing support list
{
	#define MaxVGAClock 	  205000UL	//20120822 Modify by KordonmChen for VGA max clock is 205MHz
	//#define Max_V_Freq	  860L		//120830 Edward limit range 85Hz	
#if (defined(Athena_U) )
	#if (PROJECT_SELECT == PROJ_4K2K)
    	#define MaxRxClock       305000UL//20120622 Modify by KevinChen from 255000UL to 297000UL fro support timing 1920X1440/75
	#define MaxDCLk 		 305000UL//20120622 Modify by KevinChen from 255000UL to 300000UL fro support timing 1920X1440/75
    	#else
	#define MaxRxClock		 305000UL//20120622 Modify by KevinChen from 255000UL to 297000UL fro support timing 1920X1440/75
	#define MaxDCLk 		 305000UL//20120622 Modify by KevinChen from 255000UL to 300000UL fro support timing 1920X1440/75
	#endif
    #define MaxTMDSRxClock   165000UL
#else
	#define MaxRxClock		255000UL			//Neil120730* Org = 205M for DMT1975A(1920x1200@75H) output 245M 
	#define MaxTMDSRxClock	165000UL
	#define MaxDCLk			300000UL
#endif
	#define MaxHWidth		PanelWidth + 10		//Add 10 for tolerrence
	#define MaxVHeight		PanelHeight + 10		//Add 10 for tolerrence

	DWORD dclk;
	WORD Max_V_Freq;//		  860L	
	WORD W_HWidth, W_VLength;
	dclk = (((DWORD) stInput[B_Channel].Vfrq) * stInput[B_Channel].Vtotal* stInput[B_Channel].Htotal) / 10000;
	W_HWidth = stInput[B_Channel].Hactive;
	W_VLength = stInput[B_Channel].Vactive;

	if((PanelMinVFreq >= 49)&&(PanelMaxVFreq <= 77))
		Max_V_Freq = 860;
	else if((PanelMinVFreq >= 49)&&(PanelMaxVFreq <= 120))
		Max_V_Freq = 1300;
	else
		Max_V_Freq = 860;
	
	gm_Printf("stInput[%d].Vfrq : %d", B_Channel, stInput[B_Channel].Vfrq);
	gm_Printf("stInput[%d].Vtotal : %d", B_Channel, stInput[B_Channel].Vtotal);
	gm_Printf("stInput[%d].Htotal : %d", B_Channel, stInput[B_Channel].Htotal);
	gm_Printf("stInput[%d].Hactive : %d", B_Channel, stInput[B_Channel].Hactive);
	gm_Printf("stInput[%d].Vactive : %d", B_Channel, stInput[B_Channel].Vactive);
	gm_Print("dclk : %d MHz", dclk / 1000);   



	if (dclk > MaxDCLk)
		return gmd_TRUE;

	#if ((IS_PROJ_PD())||IS_PROJ_4K2K())
	UNUSED_VAR(W_HWidth);
	UNUSED_VAR(W_VLength);
	#else
		#if defined(Athena_C)
			if((W_HWidth>1920)||(W_VLength >1200))	
				return gmd_TRUE;
		#else
			if((W_HWidth>2560)||(W_VLength >1600))	
				return gmd_TRUE;
		#endif
	#endif

	if(stInput[B_Channel].Vfrq > Max_V_Freq)
		return gmd_TRUE;

	#ifdef Athena_C
		//follow DELL Tommas sugguestion, 1920x1200@85Hz show OOR for U2413
		if((stInput[B_Channel].Hactive == 1920)&&(stInput[B_Channel].Vactive == 1200)
			&&(stInput[B_Channel].Vfrq > 840))		// -10 for tolerance
		return gmd_TRUE;
	#endif
	
	if (IsAnalogInput(B_Channel))	
	{
		if (dclk > MaxVGAClock)	
			return gmd_TRUE;	   
	}
	else if (IsDigitalInput(B_Channel))		
	{
		#ifdef Athena_U
		if (IsDPInput(B_Channel)||IsDVIInput(B_Channel))
		{
			if (dclk > MaxRxClock)	//DP1.2 still follow this condition? 
				return gmd_TRUE;	       
		}		
		else	//HDMI
		{
			if (dclk > MaxDCLk)//20120622 Add by KevinChen for HDMI support up to 300M.
				return gmd_TRUE;			 
		}
		#else		//CMI 
		if (IsDPInput(B_Channel)) //Neil 20120522* . DP > 205Mhz ==> OOR
		{
			if (dclk > MaxRxClock)	//same as U2410
				return gmd_TRUE;	       
		}
		else if(IsDVIInput(B_Channel))	//Athena-c sigle DVI
		{
			if (dclk > MaxTMDSRxClock)
				return gmd_TRUE;	   
		}
		else		//HDMI
		{
			if (dclk > MaxDCLk)	//HDMI1.2 support up to 300M	
				return gmd_TRUE;
		}
		#endif
	
	}

	return gmd_FALSE;
}

//Neil 20120531+ for color space change from RGB to YUV4:4:4 not trige the mode change;
//But color depth will triger mode change,
void ColorSpaceHandler(gmt_PHY_CH B_Channel)
{
	static BYTE SB_ColorSpace[CH_PHY_NUM] = {0xFF,0xFF};
	static BYTE SB_PortChanged[CH_PHY_NUM] = {0xFF,0xFF};
	BYTE B_CurrentPort = (B_Channel == gmvb_MainChannel)? UserPrefCurrentInputMain : UserPrefCurrentInputPip;
	
	if ((gm_TimingIsStable(B_Channel) != TRUE)&&(!gm_IsModeComplete(B_Channel)))
		return;
	if(UserPrefAdjustInputColorFormatByOSD == TRUE)
		return;
	if((B_Channel == gmvb_PipChannel)&&(UserPrefPipMode == NO_PIP))
		return;
	
	switch (GetChannelConnType(B_Channel))
	{
		case HDMI: //=====HDMI color space===============================================
		{
			HDMI_AVI_Y B_HdmiAviY = gm_HdmiGetAviColorSpace(gB_CurrentInputConnector[B_Channel]);

			if(SB_PortChanged[B_Channel]!= B_CurrentPort)
			{
				SB_PortChanged[B_Channel] = B_CurrentPort;
				SB_ColorSpace[B_Channel] = 0xFF;	//0xFF meaning nothing ,just for below condition	
			}
			
			if(SB_ColorSpace[B_Channel] != B_HdmiAviY)
			{
		 		SetDisplayOff(B_Channel, BACKGROUND_ONLY);
				msg_i("ColorSpaceHandler: HDMI Color Space = %d", B_HdmiAviY);
				if(SB_ColorSpace[B_Channel] != 0xFF)	//if == 0xFF ,first time enter, don't need force to change
	    		{
					gm_TimerStart((B_Channel == CH_A)? Display_Enable_Main_10ms_TMR : Display_Enable_Pxp_10ms_TMR, 5);
	    			ModeSetupRequest(B_Channel, REQ_MS_RESTART, PARAM_NONE, REQUEST_NOW);			
				}
				SB_ColorSpace[B_Channel] = B_HdmiAviY ;
			}

		break;
		}
		case DVI: //=====DVI color space===============================================
		{
			if(SB_PortChanged[B_Channel] != B_CurrentPort)
			{
				SB_PortChanged[B_Channel] = B_CurrentPort;
				SB_ColorSpace[B_Channel] = 0xFF;	//0xFF meaning nothing ,just for below condition	
			}

		break;
		}
		case CT_DISPLAY_PORT: //=====DP color space===============================================
		{
			static BYTE SB_DPColorDepth[CH_PHY_NUM]  = {0xFF,0xFF};
            BYTE B_DPRXColorimetry = DpRxGetColorimetry(gB_CurrentInputConnector[B_Channel]);
			BYTE B_DPRXColorDepth = gm_DpRxGetColorDepth(gB_CurrentInputConnector[B_Channel]);

			if(SB_PortChanged[B_Channel] != B_CurrentPort)
			{
				SB_PortChanged[B_Channel] = B_CurrentPort;
				SB_ColorSpace[B_Channel] = 0xFF;	//0xFF meaning nothing ,just for below condition	
			}
			
			if((SB_ColorSpace[B_Channel] !=B_DPRXColorimetry)||(SB_DPColorDepth[B_Channel] != B_DPRXColorDepth))
			{
		 		SetDisplayOff(B_Channel, BACKGROUND_ONLY);
				gm_Print("ColorSpaceHandler: DP Color Depth = %d", B_DPRXColorDepth);
				if(SB_ColorSpace[B_Channel] != 0xFF)//if == 0xFF ,first time enter, don't need force to change
	    		{
					gm_TimerStart((B_Channel == CH_A)? Display_Enable_Main_10ms_TMR : Display_Enable_Pxp_10ms_TMR, 5);
	    			ModeSetupRequest(B_Channel, REQ_MS_RESTART, PARAM_NONE, REQUEST_NOW);			
				}
				SB_ColorSpace[B_Channel] = B_DPRXColorimetry ;
				SB_DPColorDepth[B_Channel] = B_DPRXColorDepth;
			}	
		break;
		}

		default: //=====other port color space===============================================
		{
			if(SB_PortChanged[B_Channel] != B_CurrentPort)
			{
				SB_PortChanged[B_Channel] = B_CurrentPort;
				SB_ColorSpace[B_Channel] = 0xFF;	
			}
		break;
		}

	}

}


//******************************************************************************
//
// FUNCTION       : void HdmiCropInput(gmt_PHY_CH B_Channel)
//
// USAGE          : This function determines if the HDMI image should be cropped 
//
// INPUT          : B_Channel - CH_A or CH_B
//
// OUTPUT         : True if image should be cropped, else FALSE.
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  : 
//
// POST_CONDITION : 
//
//******************************************************************************
BOOL HdmiCropInput(gmt_PHY_CH B_Channel)
{
    if (gm_HdmiIsStableState(gB_CurrentInputConnector[B_Channel]))
    {
        // if AVI packet specified overscan, do cropping.
        if (gm_HdmiGetAviScanInfo(gB_CurrentInputConnector[B_Channel]) == HDMI_AVI_S_OVERSCAN_TV)
        {
            if (gm_HdmiGetAviPixelRepetition(gB_CurrentInputConnector[B_Channel]) > HDMI_PIXEL_REPEAT_THRESHOLD)
            {
                // cannot crop for pixel repeated formats since slave to DE is enabled
                return FALSE;
            }
            else
            {
                return TRUE;
            }
        }
    }
    // if an AVI packet is not available, only crop video.
    else if ( gm_IsVideo(B_Channel) || gm_IsEmbeddedVideo(B_Channel) )
    {
        return TRUE;
    }

    return FALSE;
}

//Neil120616+ need reset DP after power up.otherwise DP12RX0_MS_M might be wrong. don't know why
void ResetDPRXInit(void)
{
	gm_WriteRegByte(DP12RX0_RESET_CTRL, DPRX_RESET_AUX|DPRX_RESET_LINK|DPRX_RESET_UART);
	gm_Delay10ms(2);
}

//Neil120616+ for 1920x1080@50i switch between 8&10bits dp will lost M value 
#define DP_LinkCheckCNT  		25
void DPRX_LinkCheck(gmt_PHY_CH B_Channel)
	{
	static BYTE SB_LinkCheckFailCnt[CH_PHY_NUM] = {0,0};
	BYTE InputConnector = (B_Channel == gmvb_MainChannel)?UserPrefCurrentInputMain : UserPrefCurrentInputPip;

	if(gm_TimingIsStable(B_Channel)||(GetPortConnType(InputConnector)!=CT_DISPLAY_PORT)||(gm_ReadRegWord(DP12RX0_DPCD_STATUS)!= 0x2222))
		{
		SB_LinkCheckFailCnt[B_Channel] = 0;
		return;
		}
	
	if(DpRxIsLT_Success(InputConnector))
	{
		WORD W_InputStatus = (B_Channel == gmvb_MainChannel)?MAIN_INPUT_STATUS : PIP_INPUT_STATUS ;
		
		if(gm_ReadRegWord(W_InputStatus)& (IMP_NO_VS|IMP_HS_PERIOD_ERR|IMP_VS_PERIOD_ERR))
		{
    		PortAttrib ROM *pPort = &AppPortAttrib[InputConnector];  
			static WORD DPMValue;
			
			//gm_Printf("==========%d=DPRX_LinkCheck:DPMValue = 0x%x ", SB_LinkCheckFailCnt[B_Channel],DPMValue);
			if(abs(gm_ReadRegWord(DP12RX0_MS_M)- DPMValue)> 0x1FF)
			{
				SB_LinkCheckFailCnt[B_Channel]++;
				if(SB_LinkCheckFailCnt[B_Channel] > 5)
			    {
					OSDRequestCancelMsg(gmvb_MainChannel, MTO_NoSync);		//Neil120808+ cancel message 
			    	gmvb_UNSTABLE_COUNT[B_Channel] = pPort->B_UnstableCount + DP_LinkCheckCNT;
	}
	}

			DPMValue = gm_ReadRegWord(DP12RX0_MS_M)&0xFFFF;		//although this 24bit register ,but check low 16bits is enough
			if(SB_LinkCheckFailCnt[B_Channel] > DP_LinkCheckCNT)
			{
            	//gmt_DPRX_DID b_DID=gm_GetDpDigitalID(InputConnector);
				
				//OSDRequestCancelMsg(gmvb_MainChannel, MTO_NoSync);
				SB_LinkCheckFailCnt[B_Channel] = 0;
				ResetDPRXInit();
				#if 0//ENABLE_HPD_OVERRIDED
				SetDpRxHpdPulse(InputConnector, gmd_DPRX_HPD_PULSE_LONG);
				#endif

				gm_Print("==================DPRX_LinkCheck:ResetDPRXInit() ", 0);
			    gm_ModeDetectionConfig(B_Channel, DET_STANDARD); // Enable standard mode detection
			    gm_SetSyncTypeOrder(B_Channel, pPort->Ba_Sync[0], pPort->Ba_Sync[1], pPort->Ba_Sync[2], pPort->Ba_Sync[3]);
			    gmvb_STABLE_COUNT[B_Channel] = pPort->B_StableCount; 
			    gmvb_UNSTABLE_COUNT[B_Channel] = pPort->B_UnstableCount;
			}
		}
	}
}



void SendMissionFlagToLPM(void)
{
    static BYTE PrevState = 0xFF;
    BYTE Buff_Osd[2];

    if ( PrevState != B_UserSettingFlag )
    {
        PrevState = B_UserSettingFlag;

        Buff_Osd[0] = 0xDD;

        Buff_Osd[1] = B_UserSettingFlag;
        if ( gm_MessageSend(gmd_MESSAGE_USER, &Buff_Osd, sizeof(Buff_Osd)) )
        {
            //gm_Print("Buff_Osd[0] = %d",Buff_Osd[0]);
            //gm_Print("Buff_Osd[1] = %d",Buff_Osd[1]);

           // gm_Print("B_UserSettingFlag send as %d",B_UserSettingFlag);
        }
        else
        {
            gm_Print("==> B_UserSettingFlag send as %d failed",B_UserSettingFlag);
        }
    }

}

void OSDRequestMsg_Pxp(gmt_PHY_CH B_Channel,WORD W_Param, DWORD DW_Time)
{
	if(UserPrefPipMode != NO_PIP)
    	SchedulerPut(SchedulerOsdPack[B_Channel], REQ_OSD_MSG, W_Param, DW_Time);
}

BYTE GetDPVBIDStatus(void)
{
	return ((gm_ReadRegWord(DP12RX0_SDP_STATUS)&0xF0)>>8);
}

void AdjustBurnInMode(void) 
{
	SaveModeIndependentSettings();
}

void BurninOffDisablrAddress0x7C(void)
{
	#if defined(Qisda_93XX_BOARD)//20120726 Add by KevinChen
	UserPrefEnableDDC2BiAddress0x7C = 0;//20120726 Add by KevinChen
	#endif
}

//Neil120730+ for mode change but library not report mode change, no image output, 
//DMT1175 change to DMT1175A, M value 0x199A0 to 0x17B40;
BOOL far CheckDP_Mvid_ForSyncChange(BYTE InputConnector)
{
	static BYTE B_SyncChangeCNT = 0;
	static BYTE B_UnStableCNT = 0;
	static WORD W_PreM=0;
	WORD   W_Mvid =0;
    PortAttrib ROM *pPort = &AppPortAttrib[InputConnector];                 
	gmt_PHY_CH B_Channel = (InputConnector == UserPrefCurrentInputMain)? CH_A : CH_B;
	BYTE B_Ret = gmd_FALSE;
	
	if(GetPortConnType(InputConnector)!=CT_DISPLAY_PORT)
		return gmd_FALSE;	
	//Only check it when mode stale 
	if(!gm_TimingIsStable(B_Channel))
	{
		W_PreM = gm_ReadRegWordDP12Rx(gm_GetDpDigitalID(InputConnector),DP12RX0_MS_M)&0xFFFF ;		//24 bits value, but WORD should enough
		B_UnStableCNT = 0;
		return gmd_FALSE;
	}
	
	if(gm_ReadRegWord(input_status[B_Channel])&(IMP_NO_HS|IMP_NO_VS))// == (IMP_NO_HS|IMP_NO_VS))
		B_UnStableCNT++;
	else
		B_UnStableCNT = 0;

	if(B_UnStableCNT > (pPort->B_UnstableCount + 2)) // 4 more loops later this function will work
	{
		gm_Print("============%d=====CheckDP_Mvid_ForSyncChange true  ", B_UnStableCNT);
		B_UnStableCNT = 0;
		B_Ret = gmd_TRUE;
	}
	
	//Also check DP12RX0_MS_HTOTAL ?
	W_Mvid = gm_ReadRegWordDP12Rx(gm_GetDpDigitalID(InputConnector),DP12RX0_MS_M)&0xFFFF ;		//24 bits value, but WORD should enough
	if (abs(W_PreM - W_Mvid)> 0x200)	// 0x200 ?
		B_SyncChangeCNT++;

	if(B_SyncChangeCNT >= (pPort->B_UnstableCount + 2))	// if unstable still not triger, 2 more loop later this function will work
	{
		W_PreM = W_Mvid;
		B_SyncChangeCNT = 0;
		gm_Print("=================CheckDP_Mvid_ForSyncChange true 0x%x ", W_Mvid);
		B_Ret = gmd_TRUE;
	}

	if(B_Ret == gmd_TRUE)
	{
		gm_Print("=========REQ_MS_RESTART========CheckDP_Mvid_ForSyncChange true 0x%x ", 0);
		gm_ForceModeChange(B_Channel);
	}
	return B_Ret;

	}

//Neil120801+ for enable all channel after several frames avoid gargbage
void DisplayEnable(void)
{
	BYTE i;
	gmt_PHY_CH B_Channel;
	
	for(i = 0; i < CH_PHY_NUM; i++)
	{
		B_Channel = i;
		if(B_Channel == gmvb_MainChannel)
		{
			if((gm_TimerCheck(Display_Enable_Main_10ms_TMR) == TIMER_TMO)&&gm_IsModeComplete(gmvb_MainChannel))
			{
				gm_Print(" Main On %d ", gm_GetSystemTime());
			#if (DEFAULT_PANEL == LG_WHD_LM290WW1_SSA1)  //Michael for 29" 20121011
				{
					gm_SetOverdriveCompressRatio(On, 1, PanelHeight, PanelWidth, On);		  
					gm_UpdateOverDrive(OD_FULL_SCREEN, NULL_PTR,NULL_PTR); // Enable OD for entire display
					gm_FifoControl(ENABLE, (FIFO_OD_WR | FIFO_OD_RD));
				}
			#endif 
					
				if (UserPrefVwdFlag == ON)	//20120925 yvonne move to here after display on enable VWD
				VWD_Start();
			#if(defined(BOARD_93XX_4K2K))
				#if(DEFAULT_PANEL == ZDT_4K2K_QFHD)
				if((gm_TimerCheck(ZDT_4K2K_Reset168_100ms_TMR) == TIMER_TMO)&&((gm_ReadRegWord(DISPLAY_CONTROL)&DTG_RUN_EN) != DTG_RUN_EN))
				{
					if(UserPrefGlobalLCDOnOffFlag == FALSE)//120910 Torisa add for wake up from DPMS LCD condition will overlap with image 
						SetDisplayOn(B_Channel);
					
					gm_TimerStop(ZDT_4K2K_Reset168_100ms_TMR);
					gm_TimerStop(Display_Enable_Main_10ms_TMR);
					gm_Delay10ms(10);
					gm_DisplayOnOff(ON);
					if(IsDPInput(gmvb_MainChannel))
	                    Reset_DP_Audio();//JC_SZ_0924	
		
				}
				#else
					if(UserPrefGlobalLCDOnOffFlag == FALSE)//120910 Torisa add for wake up from DPMS LCD condition will overlap with image 
						SetDisplayOn(B_Channel);	
					
					#if(DEFAULT_PANEL == ZDT_4K2K_CMI)
					ZDT_4K2K_CMI_Panel_Blank(UserPrefPanelSelect,FALSE);
					#endif
				
					if(IsDPInput(gmvb_MainChannel))
	                    Reset_DP_Audio();//JC_SZ_0924			
					gm_TimerStop(Display_Enable_Main_10ms_TMR);
					//gm_DisplayOnOff(ON);
				#endif
				
				#ifdef MODE_CHANGE_BACKLIGHT_OFF
					gm_SetRegBitsWord(DISPLAY_CONTROL,DTG_RUN_EN);
				#endif

			#else
			if(UserPrefGlobalLCDOnOffFlag == FALSE)//120910 Torisa add for wake up from DPMS LCD condition will overlap with image 
				SetDisplayOn(B_Channel);	
			
			if(IsDPInput(gmvb_MainChannel))
				Reset_DP_Audio();//JC_SZ_0924	
		
				gm_TimerStop(Display_Enable_Main_10ms_TMR);		
			#endif
				if(AAPI_Audio_Get_SourceChannel() == AUDIO_MAIN)
				{
					AUDIO_UNMUTE();
				}	
			}
		}
		else if(B_Channel == gmvb_PipChannel)
		{
			if(Is_PIP_ON()&&(gm_TimerCheck(Display_Enable_Pxp_10ms_TMR) == TIMER_TMO))//Neil120816- if no signal,still need setup PB_PIP_ENABLE &&gm_IsModeComplete(gmvb_PipChannel))
			{
				if(Is_PIP_ON()&&(!gm_ReadRegWord(PB_CTRL)&PB_PIP_ENABLE))
					gm_BlenderOnOff(gmvb_PipChannel, On);			
				if(gm_IsModeComplete(gmvb_PipChannel))
				{
					if(IsDPInput(gmvb_PipChannel))
						Reset_DP_Audio();//Neil120927+also set to Pxp,too.....//JC_SZ_0924	
					SetDisplayOn(B_Channel);
					gm_Print(" Pxp On %d ", gm_GetSystemTime());

				if(AAPI_Audio_Get_SourceChannel() == AUDIO_PIP)
				{
					AUDIO_UNMUTE();					
				}
				gm_TimerStop(Display_Enable_Pxp_10ms_TMR);
			}
		}
}
}

#if(defined(BOARD_93XX_4K2K))
	#if(DEFAULT_PANEL == ZDT_4K2K_QFHD) 	//For PA168
	if((gm_TimerCheck(ZDT_4K2K_Reset168_100ms_TMR) == TIMER_TMO)/*&&(gm_TimerCheck(Display_Enable_Main_10ms_TMR) == TIMER_TMO)*/
		&&((gm_ReadRegWord(DISPLAY_CONTROL)&DTG_RUN_EN) != DTG_RUN_EN))
	{
		//if((gm_TimerCheck(Display_Enable_Main_10ms_TMR) == TIMER_TMO))
		{
			gm_TimerStop(ZDT_4K2K_Reset168_100ms_TMR);
			gm_DisplayOnOff(ON);
		}
	}
	#endif
#endif
}

#if (FEATURE_CEC == DISABLE)
BOOL IsHDMIInput(gmt_UserDefConnNames Port)
{
    if ((Port == ALL_BOARDS_HDMI) || (Port == ALL_BOARDS_HDMI2))
    {
        return gmd_TRUE;
}
    else
    {
        return gmd_FALSE;
    }
}
#endif
 void far DVI_Reset(BYTE InputConnector)		//for 480i, 576i issue
{
	if(GetPortConnType(InputConnector)==DVI)
	{
		gm_SetRegBitsWord(DVI0_RESET_CTRL, DVI0_RESET_LINK);	
		//gm_SetRegBitsWord(DVI0_RESET_CTRL, DVI0_RESET_ALL);
		//gm_Delay10ms(2);
		//gm_ClearRegBitsWord(DVI0_RESET_CTRL, DVI0_RESET_ALL);
	}
}


void far SetDpRxHpdPulse(gmt_UserDefConnNames B_InputConnector,gmt_DPRX_HPD_PULSE_DURATION W_DurationUs)
{
	if (GetPortConnType(B_InputConnector)==CT_DISPLAY_PORT)
	{
		gmt_DPRX_DID b_DID = gm_GetDpDigitalID(B_InputConnector);

		if(W_DurationUs == gmd_DPRX_HPD_PULSE_SHORT)		//short pulse
		{
			gm_Print("Set RX%d HPD SHORT PULSE!",b_DID);
			gm_ClearRegBitsWord(DP12RX0_HPD_PULSE_GEN_CONTROL, DPRX_HPD_LEVEL_CONTROL);	//set HPD to low
			gm_Delay1ms(1);		//short pulse < 2ms
			gm_SetRegBitsWord(DP12RX0_HPD_PULSE_GEN_CONTROL, DPRX_HPD_LEVEL_CONTROL);	//set HPD to low
		}
		else		//Long pulse
		{
			gm_Print("Set RX%d HPD LONG PULSE!",b_DID);
			ForceDpHpd2Low(b_DID,TRUE);
			SetForceHpdLowFlag(b_DID, TRUE);
			if(b_DID==DID_DPRX0)
			gm_TimerStart(RX0_HPD_OVERRIDED_10ms_TMR,HPD_OVERRIDED_TIMEx10ms);
			else if(b_DID==DID_DPRX1)
			gm_TimerStart(RX1_HPD_OVERRIDED_10ms_TMR,HPD_OVERRIDED_TIMEx10ms);

		}
	}
}

void far InitPanelSpectrum(void)
{
	BYTE B_SpreadEnable = Panel_Spread_Spect_En&DDDS1_SPREAD_SP_EN;
	
	UserPrefSpreadAmpValue = (PanelSpreadSpectrumCtrl>>1)&0x07;
	UserPrefSpreadPeriodValue =(PanelSpreadSpectrumCtrl>>4)&0x3F;
	gm_WriteRegWord(DDDS1_ESM_CTRL,B_SpreadEnable | UserPrefSpreadAmpValue<<1 | (UserPrefSpreadPeriodValue <<4));
}

#define DVI_PHS_SCR_THOLD		0xA0		// Threshold for DVI phase scroe, 0xA0 is working for 480i&576i, 1080i
BOOL far CheckDVIInputMode(gmt_PHY_CH B_Channel)
{

	if(IsDVIInput(B_Channel))
	{
		if(stInput[B_Channel].ModeFlags&gmd_MODE_INTERLACED)
		{
			BYTE i,B_LoopCnt, B_IntLaceCheckCNT = 0;
			BYTE B_InputConnector = (B_Channel == CH_A)?UserPrefCurrentInputMain : UserPrefCurrentInputPip;
			WORD W_IBD_VLEGTH = (B_Channel == CH_A)? IMP_IBD_VLENGTH : IPP_IBD_VLENGTH;
			if(gm_ReadRegWord(W_IBD_VLEGTH) >= 540)		//1080i need check more than 10 times for stable
				B_LoopCnt = 15;		
			else
				B_LoopCnt = 10;	//480i&576i check 10 times is enough	
				
			for(i = 0; i < B_LoopCnt; i++)		// 10 loop
			{
				if((gm_ReadRegByte(DVI0_RX_PHY_PHS_A_SCR + 2) > DVI_PHS_SCR_THOLD)||
					(gm_ReadRegByte(DVI0_RX_PHY_PHS_B_SCR + 2) > DVI_PHS_SCR_THOLD)||
					(gm_ReadRegByte(DVI0_RX_PHY_PHS_C_SCR + 2) > DVI_PHS_SCR_THOLD))	
					B_IntLaceCheckCNT++;

				gm_Printf(" DVI Interlace :%d Error:A = 0x%x--B=0x%x--C = 0x%x",B_IntLaceCheckCNT, gm_ReadRegByte(DVI0_RX_PHY_PHS_A_SCR + 2),gm_ReadRegByte(DVI0_RX_PHY_PHS_B_SCR + 2),gm_ReadRegByte(DVI0_RX_PHY_PHS_C_SCR + 2));
				if(B_IntLaceCheckCNT >= 3)
				{
					gm_Printf(" DVI Interlace :%d Error:A = 0x%x--B=0x%x--C = 0x%x",B_IntLaceCheckCNT, gm_ReadRegByte(DVI0_RX_PHY_PHS_A_SCR + 2),gm_ReadRegByte(DVI0_RX_PHY_PHS_B_SCR + 2),gm_ReadRegByte(DVI0_RX_PHY_PHS_C_SCR + 2));
					DVI_Reset(B_InputConnector);		//Reset DVI then re-do mode setup
					gm_ForceModeChange(B_Channel);
					return gmd_FALSE;
				}
				gm_WaitForDisplaySync(2);
			}

		}
	}
	return gmd_TRUE;
}
	

#ifdef Gamma_DUM_Initial_Check
extern WORD ROM COLOR_MAPPING_GAMUT_MATRIX_AdobeRGB[9];
extern WORD ROM Gamma_RED_PostLUT_Steep_AdobeRGB[128];
extern WORD ROM Gamma_GREEN_PostLUT_Steep_AdobeRGB[128];
extern WORD ROM Gamma_BLUE_PostLUT_Steep_AdobeRGB[128];
extern BYTE ROM Gamma_RED_PostLUT_AdobeRGB[1024];
extern BYTE ROM Gamma_GREEN_PostLUT_AdobeRGB[1024];
extern BYTE ROM Gamma_BLUE_PostLUT_AdobeRGB[1024];
extern BYTE ROM Gamma_RED_PreLUT_AdobeRGB[1024];
extern BYTE ROM Gamma_GREEN_PreLUT_AdobeRGB[1024];
extern BYTE ROM Gamma_BLUE_PreLUT_AdobeRGB[1024];
extern WORD ROM PostLUT_Register_AdobeRGB[13];
extern WORD ROM PreLUT_Register_AdobeRGB[13];
extern WORD ROM COLOR_MAPPING_GAMUT_MATRIX_sRGB[9];
extern WORD ROM Gamma_RED_PostLUT_Steep_sRGB[128];
extern WORD ROM Gamma_GREEN_PostLUT_Steep_sRGB[128];
extern WORD ROM Gamma_BLUE_PostLUT_Steep_sRGB[128];
extern BYTE ROM Gamma_RED_PostLUT_sRGB[1024];
extern BYTE ROM Gamma_GREEN_PostLUT_sRGB[1024];
extern BYTE ROM Gamma_BLUE_PostLUT_sRGB[1024];
extern BYTE ROM Gamma_RED_PreLUT_sRGB[1024];
extern BYTE ROM Gamma_GREEN_PreLUT_sRGB[1024];
extern BYTE ROM Gamma_BLUE_PreLUT_sRGB[1024];
extern WORD ROM PostLUT_Register_sRGB[13];
extern WORD ROM PreLUT_Register_sRGB[13];

extern BYTE ROM DUM_1_ODD[4096];
extern BYTE ROM DUM_1_EVEN[4096];
extern BYTE ROM DUM_1_Register[100];

//#define Checksum_DEBUG
#ifdef Checksum_DEBUG
	#define msg_checksum(a)        gm_Print(a,0)
	#define msg_checksum_i(a,b)      gm_Print(a,b)
#else
	#define msg_checksum(a)              
	#define msg_checksum_i(a,b)
#endif
// Gamma check Size is 6994 byte
// 30(3x3 Matrix)+26(PreLUT registers)+1024*3(PreLUT)+26(PostLUT registers)+1024*3(PostLUT)+256*3(PostLUT Steep)
// Total is 6994 byte
//sRGB is 0x2000, AdobeRGB is 0x4000
void GAMMA_DUM_NVRAM_CHECK(BYTE Check)
{
	BYTE B_CheckSum,S_CheckSum;
	WORD DW_NvramAddr,W_Size;
	BYTE NVRAM_Check_buf[4096];

//	if (!(gm_ReadRegWord(LPM_MISSION_STATUS) & LP_SRAM_LOADED))
	{
		msg_checksum_i("@@@@@@ GAMMA_DUM_NVRAM_CHECK %d@@@@@@",Check);
		msg_checksum_i("0 Start check Time =%d", gm_GetSystemTime());
		if(Check ==0)
		{
		//sRGB
		W_Size= 4096;
		DW_NvramAddr=C_GAMMA_REGISTER_START_OFFSET;
		NVRam_ReadBuffer(DW_NvramAddr,NVRAM_Check_buf,W_Size);
		B_CheckSum=CalculateChecksum(NVRAM_Check_buf, W_Size);
		W_Size= 2898;
		DW_NvramAddr=C_GAMMA_REGISTER_START_OFFSET+4096;
		NVRam_ReadBuffer(DW_NvramAddr,NVRAM_Check_buf,W_Size);
		B_CheckSum+=CalculateChecksum(NVRAM_Check_buf, W_Size);
		DW_NvramAddr = C_GAMMA_COMPONENT_SAVED_START_OFFSET +3;
		S_CheckSum=NVRam_ReadByte(DW_NvramAddr);

		//if checksum is the same, check next
		//else load table from firmware to NVRAM
		if(B_CheckSum != S_CheckSum)
			Load_GAMMA_From_Table_To_NVRAM(0);
		}
		else if(Check==1)
		{
		//AdobeRGB
		W_Size= 4096;
		DW_NvramAddr=C_GAMMA_REGISTER_START_OFFSET+0x2000;
		NVRam_ReadBuffer(DW_NvramAddr,NVRAM_Check_buf,W_Size);
		B_CheckSum=CalculateChecksum(NVRAM_Check_buf, W_Size);
		W_Size= 2898;
		DW_NvramAddr=C_GAMMA_REGISTER_START_OFFSET+0x2000+4096;
		NVRam_ReadBuffer(DW_NvramAddr,NVRAM_Check_buf,W_Size);
		B_CheckSum+=CalculateChecksum(NVRAM_Check_buf, W_Size);
		
		DW_NvramAddr = C_GAMMA_COMPONENT_SAVED_START_OFFSET +0x2000 +3;
		S_CheckSum=NVRam_ReadByte(DW_NvramAddr);
		
		//if checksum is the same, check next
		//else load table from firmware to NVRAM
		if(B_CheckSum != S_CheckSum)
			Load_GAMMA_From_Table_To_NVRAM(1);
		}
		else if(Check==2)
		{
		//DUM
		DW_NvramAddr=UniformityRegisterSettingStart;
		NVRam_ReadBuffer(DW_NvramAddr,NVRAM_Check_buf,28);
		B_CheckSum=CalculateChecksum(NVRAM_Check_buf, 28);
		DW_NvramAddr = UniformityIndexODDStart;
		NVRam_ReadBuffer(DW_NvramAddr,NVRAM_Check_buf,4096);
		B_CheckSum+=CalculateChecksum(NVRAM_Check_buf, 4096);
		DW_NvramAddr = UniformityIndexEvenStart;
		NVRam_ReadBuffer(DW_NvramAddr,NVRAM_Check_buf,4096);
		B_CheckSum+=CalculateChecksum(NVRAM_Check_buf, 4096);
		DW_NvramAddr = UniformityIndexODDStart-1;
		S_CheckSum=NVRam_ReadByte(DW_NvramAddr);
//		gm_Print("DUM S_CheckSum=%x",S_CheckSum);
		
		//if checksum is the same, check next
		//else load table from firmware to NVRAM
		if(B_CheckSum != S_CheckSum)
			Load_DUM_From_Table_To_NVRAM();
		}
		msg_checksum_i("END Start check Time =%d", gm_GetSystemTime());

	}
}

WORD ROM COLOR_MAPPING_GAMUT_MATRIX_InOutOffset[6]=
{
	0, 0, 0, 
	0, 0, 0, 
}; //Size = 6


// 30(3x3 Matrix)+26(PreLUT registers)+1024*3(PreLUT)+26(PostLUT registers)+1024*3(PostLUT)+256*3(PostLUT Steep)

void Load_GAMMA_From_Table_To_NVRAM(BOOL B_Selection)
{
	WORD DW_NvramAddr,DW_NvramAddr1;
	BYTE CheckSum=0;
	//sRGB is 0x2000, AdobeRGB is 0x4000
	if(B_Selection==1)
	{
		msg_checksum_i("!!!! Reload AdobeRGB Table !!!!",0);
		//Pre LUT
		DW_NvramAddr1 =C_PRE_LUT_START_OFFSET+0x2000*B_Selection;
		NVRam_WriteBuffer_DUM_Gamma(DW_NvramAddr1,(BYTE*)Gamma_RED_PreLUT_AdobeRGB,1024);
		DW_NvramAddr =DW_NvramAddr1 + C_ONE_CHANNEL_GAMMA_SIZE;
		NVRam_WriteBuffer_DUM_Gamma(DW_NvramAddr,(BYTE*)Gamma_GREEN_PreLUT_AdobeRGB,1024);
		DW_NvramAddr =DW_NvramAddr1 + C_ONE_CHANNEL_GAMMA_SIZE*2;
		NVRam_WriteBuffer_DUM_Gamma(DW_NvramAddr,(BYTE*)Gamma_BLUE_PreLUT_AdobeRGB,1024);
		//POST LUT
		DW_NvramAddr1 =C_POST_LUT_START_OFFSET+0x2000*B_Selection;
		NVRam_WriteBuffer_DUM_Gamma(DW_NvramAddr1,(BYTE*)Gamma_RED_PostLUT_AdobeRGB,1024);
		DW_NvramAddr =DW_NvramAddr1+ C_ONE_CHANNEL_GAMMA_SIZE;
		NVRam_WriteBuffer_DUM_Gamma(DW_NvramAddr,(BYTE*)Gamma_GREEN_PostLUT_AdobeRGB,1024);
		DW_NvramAddr =DW_NvramAddr1+ C_ONE_CHANNEL_GAMMA_SIZE*2;
		NVRam_WriteBuffer_DUM_Gamma(DW_NvramAddr,(BYTE*)Gamma_BLUE_PostLUT_AdobeRGB,1024);
		//POST STEEP
		DW_NvramAddr1 =C_POST_STEEP_START_OFFSET+0x2000*B_Selection;;
		NVRam_WriteBuffer_DUM_Gamma(DW_NvramAddr1,(BYTE*)Gamma_RED_PostLUT_Steep_AdobeRGB,256);
		DW_NvramAddr =DW_NvramAddr1+ C_ONE_CHANNEL_GAMMA_STEEP_SIZE;
		NVRam_WriteBuffer_DUM_Gamma(DW_NvramAddr,(BYTE*)Gamma_GREEN_PostLUT_Steep_AdobeRGB,256);
		DW_NvramAddr =DW_NvramAddr1+ C_ONE_CHANNEL_GAMMA_STEEP_SIZE*2;
		NVRam_WriteBuffer_DUM_Gamma(DW_NvramAddr,(BYTE*)Gamma_BLUE_PostLUT_Steep_AdobeRGB,256);	

		//3x3 Matrix
		DW_NvramAddr1 =C_GAMMA_REGISTER_START_OFFSET+0x2000*B_Selection;;
		NVRam_WriteBuffer_DUM_Gamma(DW_NvramAddr1,(BYTE*)COLOR_MAPPING_GAMUT_MATRIX_AdobeRGB,18);
		NVRam_WriteBuffer_DUM_Gamma(DW_NvramAddr1+18,(BYTE*)COLOR_MAPPING_GAMUT_MATRIX_InOutOffset,12);
		
		//POSTLUT Register
		DW_NvramAddr1 =C_POST_LUT_CONFIG_REG_START_OFFSET+0x2000*B_Selection;
		NVRam_WriteBuffer_DUM_Gamma(DW_NvramAddr1,(BYTE*)PostLUT_Register_AdobeRGB,26);
		//PreLUT Register
		DW_NvramAddr1 =C_PRE_LUT_CONFIG_REG_START_OFFSET+0x2000*B_Selection;
		NVRam_WriteBuffer_DUM_Gamma(DW_NvramAddr1,(BYTE*)PreLUT_Register_AdobeRGB,26);
		
		DW_NvramAddr1 =C_GAMMA_COMPONENT_SAVED_START_OFFSET+0x2000*B_Selection;
		NVRam_WriteByte(DW_NvramAddr1,0x1F);

		CheckSum = CalculateChecksum_far((BYTE far*)Gamma_RED_PreLUT_AdobeRGB,1024);
		CheckSum+= CalculateChecksum_far((BYTE far*)Gamma_GREEN_PreLUT_AdobeRGB,1024);
		CheckSum+= CalculateChecksum_far((BYTE far*)Gamma_BLUE_PreLUT_AdobeRGB,1024);
		CheckSum+= CalculateChecksum_far((BYTE far*)Gamma_RED_PostLUT_AdobeRGB,1024);
		CheckSum+= CalculateChecksum_far((BYTE far*)Gamma_GREEN_PostLUT_AdobeRGB,1024);
		CheckSum+= CalculateChecksum_far((BYTE far*)Gamma_BLUE_PostLUT_AdobeRGB,1024);
		CheckSum+= CalculateChecksum_Word(Gamma_RED_PostLUT_Steep_AdobeRGB,128);
		CheckSum+= CalculateChecksum_Word(Gamma_GREEN_PostLUT_Steep_AdobeRGB,128);
		CheckSum+= CalculateChecksum_Word(Gamma_BLUE_PostLUT_Steep_AdobeRGB,128);
		CheckSum+= CalculateChecksum_Word(COLOR_MAPPING_GAMUT_MATRIX_AdobeRGB,9);
		CheckSum+= CalculateChecksum_Word(COLOR_MAPPING_GAMUT_MATRIX_InOutOffset,6);
		CheckSum+= CalculateChecksum_Word(PostLUT_Register_AdobeRGB,13);
		CheckSum+= CalculateChecksum_Word(PreLUT_Register_AdobeRGB,13);
		//save checksum to NVRAM
		DW_NvramAddr =C_GAMMA_COMPONENT_SAVED_START_OFFSET+0x2000*B_Selection+3;
		NVRam_WriteByte(DW_NvramAddr,CheckSum);
		//set 01 to display at Factory menu to know it's default settings
		DW_NvramAddr =C_GAMMA_COMPONENT_SAVED_START_OFFSET+0x2000*B_Selection+2;
		NVRam_WriteByte(DW_NvramAddr,0x01);
	}
	else
	{
		msg_checksum_i("!!!! Reload sRGB Table !!!!",0);
		//Pre LUT
		DW_NvramAddr1 =C_PRE_LUT_START_OFFSET+0x2000*B_Selection;
		NVRam_WriteBuffer_DUM_Gamma(DW_NvramAddr1,(BYTE*)Gamma_RED_PreLUT_sRGB,1024);
		DW_NvramAddr =DW_NvramAddr1 + C_ONE_CHANNEL_GAMMA_SIZE;
		NVRam_WriteBuffer_DUM_Gamma(DW_NvramAddr,(BYTE*)Gamma_GREEN_PreLUT_sRGB,1024);
		DW_NvramAddr =DW_NvramAddr1 + C_ONE_CHANNEL_GAMMA_SIZE*2;
		NVRam_WriteBuffer_DUM_Gamma(DW_NvramAddr,(BYTE*)Gamma_BLUE_PreLUT_sRGB,1024);
		//POST LUT
		DW_NvramAddr1 =C_POST_LUT_START_OFFSET+0x2000*B_Selection;
		NVRam_WriteBuffer_DUM_Gamma(DW_NvramAddr1,(BYTE*)Gamma_RED_PostLUT_sRGB,1024);
		DW_NvramAddr =DW_NvramAddr1+ C_ONE_CHANNEL_GAMMA_SIZE;
		NVRam_WriteBuffer_DUM_Gamma(DW_NvramAddr,(BYTE*)Gamma_GREEN_PostLUT_sRGB,1024);
		DW_NvramAddr =DW_NvramAddr1+ C_ONE_CHANNEL_GAMMA_SIZE*2;
		NVRam_WriteBuffer_DUM_Gamma(DW_NvramAddr,(BYTE*)Gamma_BLUE_PostLUT_sRGB,1024);
		//POST STEEP
		DW_NvramAddr1 =C_POST_STEEP_START_OFFSET+0x2000*B_Selection;
		NVRam_WriteBuffer_DUM_Gamma(DW_NvramAddr1,(BYTE*)Gamma_RED_PostLUT_Steep_sRGB,256);
		DW_NvramAddr =DW_NvramAddr1+ C_ONE_CHANNEL_GAMMA_STEEP_SIZE;
		NVRam_WriteBuffer_DUM_Gamma(DW_NvramAddr,(BYTE*)Gamma_GREEN_PostLUT_Steep_sRGB,256);
		DW_NvramAddr =DW_NvramAddr1+ C_ONE_CHANNEL_GAMMA_STEEP_SIZE*2;
		NVRam_WriteBuffer_DUM_Gamma(DW_NvramAddr,(BYTE*)Gamma_BLUE_PostLUT_Steep_sRGB,256);	
		
		//3x3 Matrix
		DW_NvramAddr1 =C_GAMMA_REGISTER_START_OFFSET+0x2000*B_Selection;;
		NVRam_WriteBuffer_DUM_Gamma(DW_NvramAddr1,(BYTE*)COLOR_MAPPING_GAMUT_MATRIX_sRGB,18);
		NVRam_WriteBuffer_DUM_Gamma(DW_NvramAddr1+18,(BYTE*)COLOR_MAPPING_GAMUT_MATRIX_InOutOffset,12);
			
		//POSTLUT Register
		DW_NvramAddr1 =C_POST_LUT_CONFIG_REG_START_OFFSET+0x2000*B_Selection;
		NVRam_WriteBuffer_DUM_Gamma(DW_NvramAddr1,(BYTE*)PostLUT_Register_sRGB,26);
		//PreLUT Register
		DW_NvramAddr1 =C_PRE_LUT_CONFIG_REG_START_OFFSET+0x2000*B_Selection;
		NVRam_WriteBuffer_DUM_Gamma(DW_NvramAddr1,(BYTE*)PreLUT_Register_sRGB,26);

		DW_NvramAddr1 =C_GAMMA_COMPONENT_SAVED_START_OFFSET+0x2000*B_Selection;
		NVRam_WriteByte(DW_NvramAddr1,0x1F);
		
		CheckSum = CalculateChecksum_far((BYTE far*)Gamma_RED_PreLUT_sRGB,1024);
		CheckSum+= CalculateChecksum_far((BYTE far*)Gamma_GREEN_PreLUT_sRGB,1024);
		CheckSum+= CalculateChecksum_far((BYTE far*)Gamma_BLUE_PreLUT_sRGB,1024);
		CheckSum+= CalculateChecksum_far((BYTE far*)Gamma_RED_PostLUT_sRGB,1024);
		CheckSum+= CalculateChecksum_far((BYTE far*)Gamma_GREEN_PostLUT_sRGB,1024);
		CheckSum+= CalculateChecksum_far((BYTE far*)Gamma_BLUE_PostLUT_sRGB,1024);
		CheckSum+= CalculateChecksum_Word(Gamma_RED_PostLUT_Steep_sRGB,128);
		CheckSum+= CalculateChecksum_Word(Gamma_GREEN_PostLUT_Steep_sRGB,128);
		CheckSum+= CalculateChecksum_Word(Gamma_BLUE_PostLUT_Steep_sRGB,128);
		CheckSum+= CalculateChecksum_Word(COLOR_MAPPING_GAMUT_MATRIX_sRGB,9);
		CheckSum+= CalculateChecksum_Word(COLOR_MAPPING_GAMUT_MATRIX_InOutOffset,6);
		CheckSum+= CalculateChecksum_Word(PostLUT_Register_sRGB,13);
		CheckSum+= CalculateChecksum_Word(PreLUT_Register_sRGB,13);
		DW_NvramAddr =C_GAMMA_COMPONENT_SAVED_START_OFFSET+0x2000*B_Selection+3;
		NVRam_WriteByte(DW_NvramAddr,CheckSum);
//		msg_checksum("SRGB CheckSum=%x",CheckSum);
		//set 01 to display at Factory menu to know it's default settings
		DW_NvramAddr =C_GAMMA_COMPONENT_SAVED_START_OFFSET+0x2000*B_Selection+2;
		NVRam_WriteByte(DW_NvramAddr,0x01);
	}
		
	
}

void Load_DUM_From_Table_To_NVRAM(void)
{
	WORD DW_NvramAddr,DW_NvramAddr1;
	BYTE CheckSum=0;

	
	msg_checksum_i("!!!! Reload DUM Table !!!!",0);
	DW_NvramAddr1 =UniformityIndexODDStart;
	NVRam_WriteBuffer_DUM_Gamma(DW_NvramAddr1,(BYTE*)DUM_1_ODD,4096);
	DW_NvramAddr1 =UniformityIndexEvenStart;
	NVRam_WriteBuffer_DUM_Gamma(DW_NvramAddr1,(BYTE*)DUM_1_EVEN,4096);
	DW_NvramAddr1 =UniformityRegisterSettingStart;
	NVRam_WriteBuffer_DUM_Gamma(DW_NvramAddr1,(BYTE*)DUM_1_Register,28);

	CheckSum = CalculateChecksum_far((BYTE far*)DUM_1_ODD,4096);
	CheckSum += CalculateChecksum_far((BYTE far*)DUM_1_EVEN,4096);
	CheckSum += CalculateChecksum_far((BYTE far*)DUM_1_Register,28);
	DW_NvramAddr =UniformityIndexODDStart-1;
	NVRam_WriteByte(DW_NvramAddr,CheckSum);
	msg_checksum_i("DUM CheckSum=%x",CheckSum);
	//set 01 to display at Factory menu to know it's default settings
	DW_NvramAddr =UniformityIndexODDStart-2;
	NVRam_WriteByte(DW_NvramAddr,0x01);

}

BOOL CheckSumResultAtFactoryMenu(BYTE B_Selection)
{
	WORD DW_NvramAddr=0;
    BYTE B_Value=0xFF;	
	BOOL result=0;
	if((B_Selection==0)||(B_Selection==1))//sRGB=0;AdobeRGB=1
	{
		DW_NvramAddr = (C_GAMMA_COMPONENT_SAVED_START_OFFSET+0x2000*B_Selection)+2;
		B_Value = NVRam_ReadByte(DW_NvramAddr);
		if(B_Value == 0xFF)
			result = 0;
		else
			result = 1;
	}
	else if(B_Selection==2)
	{
			DW_NvramAddr =UniformityIndexODDStart-2;
			B_Value = NVRam_ReadByte(DW_NvramAddr);
			if(B_Value == 0xFF)
				result = 0;
			else
				result = 1;
	}
	return result;	
}
#else
BOOL CheckSumResultAtFactoryMenu(BYTE B_Selection){ B_Selection= B_Selection;return gmd_FALSE;}

#endif

WORD CheckVideoTimingResolution(gmt_PHY_CH B_Channel, WORD W_VActive)
{
	#define TimingTHold			8
	WORD W_VLength = 0;

	if(gm_IsVideo(B_Channel))
	{
		if(gm_IsInterlaced(B_Channel))		// interlace 480i, 576i, 1080i
		{
			if(abs(W_VActive - 240)< TimingTHold)
				W_VLength = 240;
			else if(abs(W_VActive - 288)< TimingTHold)
				W_VLength = 288;
			else if(abs(W_VActive - 540)< TimingTHold)
				W_VLength = 540;
			else
				W_VLength = 0;
		}
		else
		{
			if(abs(W_VActive - 480)< TimingTHold)
				W_VLength = 480;
			else if(abs(W_VActive - 576)< TimingTHold)
				W_VLength = 576;
			else if(abs(W_VActive - 720)< TimingTHold)
				W_VLength = 720;
			else if(abs(W_VActive - 1080)< TimingTHold)
				W_VLength = 1080;
			else
				W_VLength = 0;			
		}
	}
	return 	W_VLength;
}

#ifdef AUDIO_DIF_CHECK
void SetAUD_BUF_DELAY(BYTE B_Delay)
{
	BYTE B_CurrentAudioInput = (UserPrefSpeakerAudioSource == AUDIO_MAIN)? UserPrefCurrentInputMain : UserPrefCurrentInputPip;

	//gm_Printf("B_CurrentAudioInput %d = %d ", B_CurrentAudioInput,B_Delay);
	if((B_CurrentAudioInput == ALL_BOARDS_DP1)||(B_CurrentAudioInput == ALL_BOARDS_DP3))
		gm_WriteRegWord(DP12RX0_SDP_AUD_BUF_CNTRL, ((B_Delay<<8)|(gm_ReadRegWord(DP12RX0_SDP_AUD_BUF_CNTRL)&0xFF)));
	else if(B_CurrentAudioInput == ALL_BOARDS_HDMI)
		gm_WriteRegWord(HDRX_SDP_AUD_BUF_DELAY, B_Delay);
}

BYTE ReadAUD_BUF_DELAY(gmt_UserDefConnNames B_InputConnector)
{
	if((B_InputConnector == ALL_BOARDS_DP1)||(B_InputConnector == ALL_BOARDS_DP3))
		return (gm_ReadRegWord(DP12RX0_SDP_AUD_BUF_CNTRL)>>8);
	else if(B_InputConnector == ALL_BOARDS_HDMI)
		return gm_ReadRegWord(HDRX_SDP_AUD_BUF_DELAY)&0xFF;
	else 
		return 0;
}

WORD ReadAUD_BUFPTR_DIF(gmt_UserDefConnNames B_InputConnector)
{
	if((B_InputConnector == ALL_BOARDS_DP1)||(B_InputConnector == ALL_BOARDS_DP3))
		return gm_ReadRegWord(DP12RX0_SDP_AUD_BUFPTR_DIF);
	else if(B_InputConnector == ALL_BOARDS_HDMI)
		return gm_ReadRegWord(HDRX_SDP_AUD_BUFPTR_DIF);
	else 
		return 0;
}

WORD AUD_DIF_MAX(gmt_UserDefConnNames B_InputConnector)
{
	if((B_InputConnector == ALL_BOARDS_DP1)||(B_InputConnector == ALL_BOARDS_DP3))
		return 0x3F0;	// actually 0x3FF
	else if(B_InputConnector == ALL_BOARDS_HDMI)
		return 0xFFF0; //actually 0xFFFF
	else 
		return 0x3FF;	
}	

BYTE AUD_DIF_States = AUD_DIF_INIT;
#define HDMI_AUD_BUF_DELAY_Min  8
#define HDMI_AUD_BUF_DELAY_MAX  0x2A
#define DP_AUD_BUF_DELAY_Min  0x0E
#define DP_AUD_BUF_DELAY_Max  0x26
#define AUD_STABLE_THOLD 	5
#define AUD_UNSTABLE_THOLD 	4
#define AUD_CHECK_LOOP 		8
#define CHECK_CNT			4
BYTE AUD_Delay_Min(gmt_UserDefConnNames B_InputConnector)
{
	if((B_InputConnector == ALL_BOARDS_DP1)||(B_InputConnector == ALL_BOARDS_DP3))
		return DP_AUD_BUF_DELAY_Min;
	else if(B_InputConnector == ALL_BOARDS_HDMI)
		return HDMI_AUD_BUF_DELAY_Min;
	else
		return 0;
}

BYTE AUD_Delay_Max(gmt_UserDefConnNames B_InputConnector)
{
	if((B_InputConnector == ALL_BOARDS_DP1)||(B_InputConnector == ALL_BOARDS_DP3))
		return DP_AUD_BUF_DELAY_Max;
	else if(B_InputConnector == ALL_BOARDS_HDMI)
		return HDMI_AUD_BUF_DELAY_MAX;
	else
		return 0;
}

BYTE AUD_DIF_THOLD(gmt_UserDefConnNames B_InputConnector)
{
	if((B_InputConnector == ALL_BOARDS_DP1)||(B_InputConnector == ALL_BOARDS_DP3))
		return 1;
	else if(B_InputConnector == ALL_BOARDS_HDMI)
		return 3;
	else
		return 2;
}
void AudioDIFCheck(void)
{
	BYTE B_CurrentAudioInput = (UserPrefSpeakerAudioSource == AUDIO_MAIN)? UserPrefCurrentInputMain : UserPrefCurrentInputPip;
	//BYTE B_Channel = (UserPrefSpeakerAudioSource == AUDIO_MAIN)? CH_A : CH_B;
	BYTE B_BufferDef = (B_CurrentAudioInput == ALL_BOARDS_HDMI)? 0x20 : 0x16;	//these value are FW default;
	static BYTE B_BuffDelay;
	static BYTE B_LoopCNT = 0, B_CheckCNT = CHECK_CNT;
	static BYTE B_AudStable, B_AudUnStable;

	//if(!gm_IsValidSignal(B_Channel))
	//	return;
	
	switch(AUD_DIF_States)
	{
		case AUD_DIF_INIT:
			B_LoopCNT = 0;
			B_CheckCNT	= CHECK_CNT;		
			B_AudStable = 0;
			B_AudUnStable = 0;
			if(ReadAUD_BUFPTR_DIF(B_CurrentAudioInput) > AUD_DIF_THOLD(B_CurrentAudioInput))
			{
				gm_Print("Start fine tune B_BuffDelay = %d", B_BuffDelay);
				AUD_DIF_States = AUD_DIF_UP;
			}

			if(B_BuffDelay != B_BufferDef )
			{
				B_BuffDelay = B_BufferDef;
				SetAUD_BUF_DELAY(B_BuffDelay);
			}
		break;
		case AUD_DIF_ReCheck:
			if(ReadAUD_BUFPTR_DIF(B_CurrentAudioInput) > (AUD_DIF_THOLD(B_CurrentAudioInput)+ B_CheckCNT))
				AUD_DIF_States = AUD_DIF_UP;
			//B_BuffDelay = B_BufferDef;
			B_LoopCNT = 0;
			B_AudStable = 0;
			B_AudUnStable = 0;
			//SetAUD_BUF_DELAY(B_BuffDelay);
		break;		
		case AUD_DIF_UP:
			if(ReadAUD_BUFPTR_DIF(B_CurrentAudioInput) <= (AUD_DIF_THOLD(B_CurrentAudioInput)+ B_CheckCNT))	//stable//check it after 5 loops
				B_AudStable++;
			else
				B_AudUnStable++;
		
			if((B_AudUnStable > AUD_UNSTABLE_THOLD)&&(B_LoopCNT >= AUD_CHECK_LOOP))//failed 3 times in 8 loops
			{	
				if(B_BuffDelay < AUD_Delay_Max(B_CurrentAudioInput))
				B_BuffDelay++;
				B_LoopCNT = 0;
				B_AudStable = 0;
				B_AudUnStable = 0;
				SetAUD_BUF_DELAY(B_BuffDelay);
				//gm_Print("Up : B_BuffDelay = %d", B_BuffDelay);
			}
				
			B_LoopCNT++;

			if(B_AudStable > AUD_STABLE_THOLD)
			{
				B_LoopCNT = 0;
				B_AudStable = 0;
				B_AudUnStable = 0;				
				SetAUD_BUF_DELAY(B_BuffDelay);
				AUD_DIF_States = AUD_DIF_FINISH;
			}

			if(B_BuffDelay >= AUD_Delay_Max(B_CurrentAudioInput))
			{
				//B_BuffDelay = B_BufferDef;
				B_LoopCNT = 0;
				B_AudStable = 0;
				B_AudUnStable = 0;
				AUD_DIF_States = AUD_DIF_DOWN;
				SetAUD_BUF_DELAY(B_BuffDelay);
			}
		break;
		case AUD_DIF_DOWN:
			if(ReadAUD_BUFPTR_DIF(B_CurrentAudioInput) <= (AUD_DIF_THOLD(B_CurrentAudioInput)+ B_CheckCNT))	//stable//check it after 5 loops
				B_AudStable++;
			else
				B_AudUnStable++;
			
			if((B_AudUnStable > AUD_UNSTABLE_THOLD)&&(B_LoopCNT >= AUD_CHECK_LOOP))//failed 3 times in 8 loops
			{	
				if(B_BuffDelay > AUD_Delay_Min(B_CurrentAudioInput))
				B_BuffDelay--;
				B_LoopCNT = 0;
				B_AudStable = 0;
				B_AudUnStable = 0;
				SetAUD_BUF_DELAY(B_BuffDelay);
				//gm_Print(" DOWN: B_BuffDelay = %d", B_BuffDelay);
			}
				
			B_LoopCNT++;

			if(B_AudStable > AUD_STABLE_THOLD)
			{
				B_LoopCNT = 0;
				B_AudStable = 0;
				B_AudUnStable = 0;
				SetAUD_BUF_DELAY(B_BuffDelay);
				AUD_DIF_States = AUD_DIF_FINISH;
			}

			if(B_BuffDelay <= AUD_Delay_Min(B_CurrentAudioInput))
			{
				B_BuffDelay = B_BufferDef;
				B_LoopCNT = 0;
				B_AudStable = 0;
				B_AudUnStable = 0;
				AUD_DIF_States = AUD_DIF_FINISH;
				SetAUD_BUF_DELAY(B_BuffDelay);
			}
		break;
		case AUD_DIF_FINISH:
			B_LoopCNT++;
			B_CheckCNT--;
			if(ReadAUD_BUFPTR_DIF(B_CurrentAudioInput) > (AUD_DIF_THOLD(B_CurrentAudioInput)+ B_CheckCNT))//check it after 5 loops
			{	
				B_AudUnStable++;		
				if(B_LoopCNT >25)		// 25 loops
				{		
					if(B_AudUnStable >= 15)	// got 12 times in 25 loops, need fine tune again
					{
						//gm_Printf("%d B_AudUnStable %d", B_CheckCNT,B_AudUnStable);
						if(B_CheckCNT == 0)
							AUD_DIF_States = AUD_DIF_Stop;
						else
							AUD_DIF_States = AUD_DIF_ReCheck;
					}
					else
					{
						B_LoopCNT = 0;
						B_AudStable = 0;
						B_AudUnStable = 0;
					}
				}
				if((ReadAUD_BUFPTR_DIF(B_CurrentAudioInput) > 0xFFF0)&&(B_BuffDelay > AUD_Delay_Min(B_CurrentAudioInput)))
					SetAUD_BUF_DELAY(--B_BuffDelay);

			}	
			if(ReadAUD_BUFPTR_DIF(B_CurrentAudioInput) <= (AUD_DIF_THOLD(B_CurrentAudioInput)+ B_CheckCNT))
			{
				B_AudStable++;
				if(B_LoopCNT >25)		// 25 loops
				{		
					if(B_AudStable <= 8)	// got 8 times in 25 loops, need fine tune again
					{
						//gm_Printf("%d B_AudStable %d", B_CheckCNT,B_AudStable);
						if(B_CheckCNT == 0)
							AUD_DIF_States = AUD_DIF_Stop;
						else
							AUD_DIF_States = AUD_DIF_ReCheck;
					}					
					else
					{
						B_LoopCNT = 0;
						B_AudStable = 0;
						B_AudUnStable = 0;
					}
				}
			}
		break;

		case AUD_DIF_Stop:
			if(!gm_IsValidSignal((UserPrefSpeakerAudioSource == AUDIO_MAIN)? CH_A : CH_B))
			{
				//B_BuffDelay = B_BufferDef;
				B_LoopCNT = 0;
				B_CheckCNT = CHECK_CNT;
				B_AudStable = 0;
				B_AudUnStable = 0;
				SetAUD_BUF_DELAY(B_BuffDelay);
				AUD_DIF_States = AUD_DIF_ReCheck;
			}			
		break;
		default:
		break;		
	}
}
#endif


//Neil120908+ for 6850 graphic card audio Maud always change after AC off/on ;
#ifdef AUDIO_DP_M_CHECK
BOOL far CheckDP_Maud_ForAudioStable(void)
{
	#if 1
	#define AudioCheckLoop			15
	#define AudioUnStableTHOLD 		1
	gmt_UserDefConnNames B_InputConnector = (UserPrefSpeakerAudioSource == AUDIO_MAIN)? UserPrefCurrentInputMain : UserPrefCurrentInputPip;
	if(GetPortConnType(B_InputConnector)== CT_DISPLAY_PORT)
	{
		static BYTE B_AudUnStableCNT = 0, B_LoopCNT = 0;
		static WORD W_PreMaud=0;
		WORD   W_Maud =0;
		gmt_PHY_CH B_Channel = (B_InputConnector == UserPrefCurrentInputMain)? CH_A : CH_B;
		BYTE B_Ret = gmd_FALSE;
		static BYTE B_AudioReady = gmd_FALSE;
		//Only check it when mode stable 
		if(!gm_IsValidSignal(B_Channel))
		{
			W_PreMaud = gm_ReadRegWordDP12Rx(gm_GetDpDigitalID(B_InputConnector),DP12RX0_SDP_AUD_M)&0xFFFF ;		//24 bits value, but WORD should enough
			return gmd_FALSE;
		}

		W_Maud = gm_ReadRegWordDP12Rx(gm_GetDpDigitalID(B_InputConnector),DP12RX0_SDP_AUD_M)&0xFFFF ;		//24 bits value, but WORD should enough
		if (abs(W_PreMaud - W_Maud) > 0x200)	// 0x20 ?
		{
			if(!IS_AUDIO_MUTE())//&&(AudioMuteFlag == OFF))
			{
				gm_Print("==OFF==%d", gm_GetSystemTime());
				B_AudioReady = gmd_FALSE;
				SetAudioDACOutput(OFF);			
			}
			B_AudUnStableCNT++;
			B_Ret = gmd_FALSE;
		}
		
		if(B_LoopCNT >= AudioCheckLoop)	//count 5 stable in 7 loops
		{
			static BYTE B_StableCNT = 0;
			B_LoopCNT = 0;
			W_PreMaud = W_Maud;
			if(B_AudUnStableCNT >= AudioUnStableTHOLD)
				{
				gm_Printf("====B_AudUnStaleCNT====%d===0x%x==== ", B_AudUnStableCNT,W_Maud);
				B_StableCNT = 0;
				B_AudUnStableCNT = 0;
				if(!IS_AUDIO_MUTE())//&&(AudioMuteFlag == OFF))
					SetAudioDACOutput(OFF);
				B_Ret = gmd_FALSE;
			}
			else
			{
				B_AudUnStableCNT = 0;

				if(IS_AUDIO_MUTE()&&(AudioMuteFlag == OFF)&&(B_StableCNT> 5)&&(gm_TimerCheck(AUDIO_WAIT_10ms_TMR)!= TIMER_OK))//&&(!gm_ReadRegWord(DP12RX0_SDP_AUD_STS)&DPRX_SDP_AUD_MUTE_STS))
				{
					B_StableCNT = 0;
					if(IsAudioPacketPresent(B_InputConnector)&&(B_AudioReady == gmd_FALSE))
					{
						B_AudioReady = gmd_TRUE;
						gm_Print("==ON==%d", gm_GetSystemTime());
						if(gm_TimerCheck(AUDIO_WAIT_10ms_TMR)== TIMER_STOP)
						gm_TimerStart(AUDIO_WAIT_10ms_TMR, Aud_default_Time);
						//SetAudioDACOutput(ON);
				}
					B_Ret = gmd_TRUE;
				}
				B_StableCNT++;
			}

		}	
		
		B_LoopCNT++;	
		return B_Ret;
	}
	#endif
	return gmd_TRUE;
}
#endif

BOOL Is_HDMIInput(gmt_PHY_CH B_Channel)
{
	BYTE B_Input = (B_Channel == gmvb_MainChannel)?UserPrefCurrentInputMain : UserPrefCurrentInputPip;
	return (B_Input == ALL_BOARDS_HDMI); 
}
gmt_AUDIO_INPORT_TYPE GetAudioSource(gmt_PHY_CH B_Channel)//,gmt_UserDefConnNames B_InputConnector)
{
	return AAPI_Audio_Get_CurrentInput(B_Channel);
}

ALL_BOARDS_INPUT_PORT GetCurrentAudioPort(void)
{
	if(UserPrefAudioMainInput == gmd_AUDIO_SRC_AUTO_Pxp)
		return UserPrefCurrentInputPip;
	else
		return UserPrefCurrentInputMain;
}
BOOL IsCurChannelDisplayOFF(gmt_PHY_CH B_Channel)
{
	if(B_Channel == gmvb_PipChannel)
		return ((gm_ReadRegWord(PB_CTRL)&PB_FORCE_PIP_BKGND) == PB_FORCE_PIP_BKGND);
	else //if(B_Channel == gmvb_MainChannel)
		return ((gm_ReadRegWord(PB_CTRL)&PB_FORCE_MAIN_BKGND) == PB_FORCE_MAIN_BKGND);
}


BOOL GetHDMIAudioPCMWarningMessage(void)//owen 120925
{
 	if(IsMainHDMIInput() && (OSD_IsMainSync))
 	{
		if ( SYS_API_AUDIO_GetCurrentCodingType(gmvb_MainChannel)==0)// owen 120920 detected =0 not show PCMWarningMessageB
	       	return gmd_FALSE;
		else
		{
			if(SYS_API_AUDIO_GetCurrentCodingType(gmvb_MainChannel) == gmd_AUDIO_CODING_PCM)
			{
				AudioUnsupport_Message = TRUE;//reset message display flag
				return gmd_FALSE;
			}
	     	else
				return gmd_TRUE;
		}

 	}
  return gmd_FALSE;
}

void ResetDP_SDP_Data_Path(void)
{
	gm_Print("4444444444444444444444444444ResetDP_SDP_Data_Path %d",gm_GetSystemTime());
	gm_SetRegBitsWord(DP12TX1_RL_CONFIG, BIT28);		// BIT28 = DP12TX1_SOFT_RESET_DP11_SDP
	gm_Delay1ms(5);
	gm_ClearRegBitsWord(DP12TX1_RL_CONFIG, BIT28);		// BIT28 = DP12TX1_SOFT_RESET_DP11_SDP
}

U8 B_audio_patch=1;
void Reset_DP_Audio(void)
{


#define DP12RX_MAX_AUD_SMP_PER_LINE 0x8F2C
if( (B_audio_patch == 1)&&(gm_ReadRegDWord(DP12RX_MAX_AUD_SMP_PER_LINE) != 0) &&
	(gm_ReadRegDWord(DP12RX0_SDP_AUD_M) != 0)  && (gm_ReadRegDWord(DP12RX0_SDP_AUD_N) == 0x80000) &&
	((gm_ReadRegDWord(DP12RX0_SDP_AUD_STS) & DPRX_SDP_AUD_PKT_PRSNT) == 0)  )   	
	{
   		B_audio_patch = 0;

	gm_Print("===========Reset_DP_Audio: Done  %d",gm_GetSystemTime());		
/*		
		CLEAR_REG_BITS_U32(DP12TX_RL_CONFIG, DP_BIT0);
		SET_REG_BITS_U32(DP12TX_RL_CONFIG, DP_BIT0);
		gm_Print("+Aud sts%x",READ_REG0_U16(DP12RX_SDP_AUD_STS));

		CLEAR_REG_BITS_U32(DP12TX_RL_CONFIG, DP_BIT28);
		SET_REG_BITS_U32(DP12TX_RL_CONFIG, DP_BIT28);
		gm_Print("++AUD Sts %x",READ_REG0_U16(DP12RX_SDP_AUD_STS));


		SET_REG_BITS_U32(DP12RX_RL_CONFIG, DP_BIT0);
		CLEAR_REG_BITS_U32(DP12TX_RL_CONFIG, DP_BIT0);
		gm_Print("+Aud sts%x",READ_REG0_U16(DP12RX_SDP_AUD_STS));
*/
	     gm_SetRegBitsDWord(DP12RX_RL_CONFIG, BIT28);		
	     gm_SetRegBitsDWord(DP12TX1_RL_CONFIG, BIT28);			     
	     gm_Delay1ms(10);
	      gm_ClearRegBitsDWord(DP12RX_RL_CONFIG, BIT28);			
	      gm_ClearRegBitsDWord(DP12TX1_RL_CONFIG, BIT28);			  
	      gm_Print("++++++++++Audio Stas  0x%x",gm_ReadRegDWord(DP12RX0_SDP_AUD_STS));				
	}
     else
		gm_Print("-------------No need : Reset_DP_Audio  %d",gm_GetSystemTime());	
}


//VWD need wait audio ready then start VWD processing
BOOL VWD_IsAudioReady(gmt_PHY_CH B_Channel)
 	{
	if((IsDPInput(B_Channel)||Is_HDMIInput(B_Channel)))
	{
		static DWORD DW_CheckTime = 0;
		
		if(SYS_API_AUDIO_Is_DAC_VolumeSetupDone() == gmd_FALSE)
			DW_CheckTime = gm_GetSystemTime();
		if(!IS_AUDIO_MUTE())
	       return gmd_TRUE;
		//if audio already finish setup less than 3.5seconds, return true
		if(abs((WORD)(gm_GetSystemTime() - DW_CheckTime)) > 8000)		
			return gmd_TRUE;
		
		if(IsAudioPacketPresent(UserPrefCurrentInputMain)&&(abs((WORD)(gm_GetSystemTime() - DW_CheckTime)) > 6000))
			return gmd_TRUE;
		


  return gmd_FALSE;
}
	else
		return gmd_TRUE;


}

//Neil120920+ for audio volume became larger after DC on compare to AC on;
void ToggleAudioDACAttn(void)
{
	gm_SetRegBitsWord(AUDIO_DAC_CTRL2, ATTN_LATCH_DAC);
	gm_Delay1ms(1);
	gm_ClearRegBitsWord(AUDIO_DAC_CTRL2, ATTN_LATCH_DAC);
}

extern BOOL gm_AUDIO_DAC_GAIN_APPLY_T1_INIT(WORD data);
extern void gm_AUDIO_DAC_GAIN_APPLY_T2_LATCH_ENABLE(void);
extern void gm_AUDIO_DAC_GAIN_APPLY_T3_RIGHT_T1(void);
extern void gm_AUDIO_DAC_GAIN_APPLY_T4_RIGHT_T2(void);
extern void gm_AUDIO_DAC_GAIN_APPLY_T5_LEFT_T1(void);
extern void gm_AUDIO_DAC_GAIN_APPLY_T6_LEFT_T2(void);
extern void gm_AUDIO_DAC_GAIN_APPLY_T7_LATCH_DISABLE(void);


void Audio_RedoDACGain(void)
{
	DWORD DW_TimerCheck = 0;
	BYTE AUDIO_DAC_GAIN_States = 1;
	BYTE time_zone = 30;	 //Set to 30ms and give it a try
	static DWORD checkpoint;
	
	B_AudioTimerCtrl &= ~Aud_Tmr_AudSampleRateChange;
	//AUDIO_DAC_GAIN = 1;	
	//AUDIO_DAC_GAIN_LEVEL = DEFAULT_VOLUME_VALUE;
  	//UserPrefAudioVolume = DEFAULT_VOLUME_VALUE;
	DW_TimerCheck = gm_GetSystemTime();
	if(B_NeedRedoDACGain == gmd_FALSE)
		return;
    	//gm_Print("===============Audio_RedoDACGain============= %d ", (WORD)gm_GetSystemTime());
	while(1)
	{
		if(gm_GetSystemTime() - DW_TimerCheck > 2000)
			break;
		
		switch (AUDIO_DAC_GAIN_States)
		{
			case 1:

				// if (gm_AUDIO_DAC_GAIN_APPLY_T1_INIT(AUDIO_DAC_GAIN_LEVEL*ATTN_DAC_GAIN_MULTIPLY) == FALSE)
				/*
				if (gm_AUDIO_DAC_GAIN_APPLY_T1_INIT(20*10) == FALSE)
				{
				AUDIO_DAC_GAIN_States = 0;
				}
				else
				*/
				{
				//gm_AUDIO_Print("AUDIO - Configuration of DAC gain 0x%X.", AUDIO_DAC_GAIN_LEVEL*ATTN_DAC_GAIN_MULTIPLY);

				AUDIO_DAC_GAIN_States = 2;

				//gm_Printf( "%d AUDIO_DAC_GAIN_States = %d",AUDIO_DAC_GAIN_States, gm_GetSystemTime());
				checkpoint = (DWORD) gm_GetSystemTime();

				// return FALSE;
				}
			break;
			case 2:

				if ((gm_GetSystemTime() - checkpoint) > time_zone)
				{

				gm_AUDIO_DAC_GAIN_APPLY_T2_LATCH_ENABLE();

				AUDIO_DAC_GAIN_States = 3;

				checkpoint = (DWORD) gm_GetSystemTime();

				//gm_Printf( "%d AUDIO_DAC_GAIN_States = %d",AUDIO_DAC_GAIN_States, gm_GetSystemTime());
				// return FALSE;

				}
			break;
			case 3:

				if ((gm_GetSystemTime() - checkpoint) > time_zone)
				{

				gm_AUDIO_DAC_GAIN_APPLY_T3_RIGHT_T1();

				AUDIO_DAC_GAIN_States = 4;

				checkpoint = (DWORD) gm_GetSystemTime();

				//gm_Printf( "%d AUDIO_DAC_GAIN_States = %d",AUDIO_DAC_GAIN_States, gm_GetSystemTime());
				}
			break;
			case 4:

				if ((gm_GetSystemTime() - checkpoint) > time_zone)
				{

				gm_AUDIO_DAC_GAIN_APPLY_T4_RIGHT_T2();

				AUDIO_DAC_GAIN_States = 5;

				checkpoint = (DWORD) gm_GetSystemTime();

				//gm_Printf( "%d AUDIO_DAC_GAIN_States = %d",AUDIO_DAC_GAIN_States, gm_GetSystemTime());
				}
			break;
			case 5:

				if ((gm_GetSystemTime() - checkpoint) > time_zone)
				{

				gm_AUDIO_DAC_GAIN_APPLY_T5_LEFT_T1();

				AUDIO_DAC_GAIN_States = 6;

				checkpoint = (DWORD) gm_GetSystemTime();

				// return FALSE;

				//gm_Printf( "%d AUDIO_DAC_GAIN_States = %d",AUDIO_DAC_GAIN_States, gm_GetSystemTime());
				}
			break;

			case 6:

				if ((gm_GetSystemTime() - checkpoint) > time_zone)
				{

				gm_AUDIO_DAC_GAIN_APPLY_T6_LEFT_T2();

				AUDIO_DAC_GAIN_States = 7;

				checkpoint = (DWORD) gm_GetSystemTime();

				//gm_Printf( "%d AUDIO_DAC_GAIN_States = %d",AUDIO_DAC_GAIN_States, gm_GetSystemTime());
				// return FALSE;

				}
			break;
			case 7:

				if ((gm_GetSystemTime() - checkpoint) > time_zone)
				{

				gm_AUDIO_DAC_GAIN_APPLY_T7_LATCH_DISABLE();

				AUDIO_DAC_GAIN_States = 8;

				checkpoint = (DWORD) gm_GetSystemTime();

				// return FALSE;

				//gm_Printf( "%d AUDIO_DAC_GAIN_States = %d",AUDIO_DAC_GAIN_States, gm_GetSystemTime());
				}
			break;
			case 8:

				if ((gm_GetSystemTime() - checkpoint) > 2)
				{

				AUDIO_DAC_GAIN_States = 0;

				//gm_AUDIO_Print("AUDIO - Configuration of DAC gain finished.");

				//gm_Printf( "%d AUDIO_DAC_GAIN_States = %d",AUDIO_DAC_GAIN_States, gm_GetSystemTime());
				}
			break;
			case 0:
				B_NeedRedoDACGain = gmd_FALSE;
			return;
			
			default:
			break; ;
		}
		
	}
	B_NeedRedoDACGain = gmd_FALSE;
    	//gm_Print("===============Audio_RedoDACGain============= %d ", (WORD)gm_GetSystemTime());
}


#ifndef AUDIO_DP_USE_REGEN
void AUDIO_DP_ClearRegen(void)
{
	gm_ClearRegBitsWord(DP12TX1_AUD_REGEN_CTRL, (DP12TX1_SDP_AUD_TS_REGEN_EN|DP12TX1_SDP_AUD_TS_REGEN_AUTO_EN));
}
void AUDIO_DP_SetRegen(void)
{
	gm_SetRegBitsWord(DP12TX1_AUD_REGEN_CTRL, (DP12TX1_SDP_AUD_TS_REGEN_EN|DP12TX1_SDP_AUD_TS_REGEN_AUTO_EN));
}
#endif

//Neil120925+ Main is DVI, DP on Pxp, AC off/on, no audio, need reset regen,
//but no such issue if DP on main......
void Audio_ResetRegen(void)
{
	if(gm_ReadRegWord(DP12TX1_AUD_REGEN_CTRL)&(DP12TX1_SDP_AUD_TS_REGEN_EN|DP12TX1_SDP_AUD_TS_REGEN_AUTO_EN))
	{
	gm_ClearRegBitsWord(DP12TX1_AUD_REGEN_CTRL, (DP12TX1_SDP_AUD_TS_REGEN_EN|DP12TX1_SDP_AUD_TS_REGEN_AUTO_EN));
	gm_Delay1ms(20);
	gm_SetRegBitsWord(DP12TX1_AUD_REGEN_CTRL, (DP12TX1_SDP_AUD_TS_REGEN_EN|DP12TX1_SDP_AUD_TS_REGEN_AUTO_EN));
}
}

//Neil120927+ only rest RX SDP so far avoid MST audio been reset, 
//if still found no audio issue, then may need reset TX SDP
void DP_AudioResetSDP(void)
{
	gm_SetRegBitsDWord(DP12RX_RL_CONFIG, DPRX_SOFT_RESET_DP11_SDP);		
	//gm_SetRegBitsDWord(DP12TX1_RL_CONFIG, DP12TX1_SOFT_RESET_DP11_SDP);			     
	gm_Delay1ms(10);
	gm_ClearRegBitsDWord(DP12RX_RL_CONFIG, DPRX_SOFT_RESET_DP11_SDP);			
	//gm_ClearRegBitsDWord(DP12TX1_RL_CONFIG, DP12TX1_SOFT_RESET_DP11_SDP);	
}
void HDRX_ResetAudioOutput(void)
{
	 gm_ClearRegBitsWord(HDRX_SDP_AUD_OUT_CNTRL, HDRX_SDP_AUD_OUT_EN);
	 gm_Delay10ms(3);
	 gm_SetRegBitsWord(HDRX_SDP_AUD_OUT_CNTRL, HDRX_SDP_AUD_OUT_EN); 
}
//Neil120925+ just replace before toggle audio output in gm_Audio_Patch();
void Audio_ResetAudioOutput(void)
{
	gmt_UserDefConnNames B_InputConnector = (UserPrefSpeakerAudioSource == AUDIO_MAIN)? UserPrefCurrentInputMain : UserPrefCurrentInputPip;

	//gm_Print("==5555555555555===========Audio_ResetAudioOutput %d", B_InputConnector);

	if(B_InputConnector == ALL_BOARDS_HDMI)
			HDRX_ResetAudioOutput();

	if((B_InputConnector == ALL_BOARDS_DP1)||(B_InputConnector == ALL_BOARDS_DP3))
	{
		//Main is DVI no audio, DP on Pxp, no audio output after DC off/on;
		//but if DP on Main, always has audio, Why???
		DP_AudioResetSDP();	//change to soft reset instead of reset Regen
		
		#ifndef AUDIO_DP_USE_REGEN
		//AUDIO_DP_ClearRegen();	
		#endif		
		DPRX_ResetAudio(DpRxGetPortId(B_InputConnector));//Neil120927 hear noise once, enable it again//120925 Edward temp disable for AE suggest after DDS correct
	}

}

//Neil120925+ adjust volume, will use in the future
extern void gm_AUDIO_VOLUME(gmt_PHY_CH B_Channel);
void Audio_AdjustVolume(BYTE B_AdjustValue)
{
	//DWORD DW_TimerCheck = 0;
  	gm_AUDIO_VOLUME(GetUserPrefAudioVolume());

	if(B_AdjustValue  > GetMaximumVolumeOSDStep())
		B_AdjustValue = GetMaximumVolumeOSDStep();
	if(B_AdjustValue  == MINIMAL_VOLUME_VALUE)	//if = 0, this function won't work
		B_AdjustValue = MINIMAL_VOLUME_VALUE + 1;
	
	//AUDIO_DAC_GAIN = 1;	
	//AUDIO_DAC_GAIN_LEVEL = B_AdjustValue;
	B_AudioTimerCtrl &= ~Aud_Tmr_AudSampleRateChange;
	//DW_TimerCheck = gm_GetSystemTime();
	UserPrefAudioVolume = B_AdjustValue;
	//gm_Print("===============Audio_RedoDACGain============= %d ", (WORD)gm_GetSystemTime());
	/*
	while(1)
	{
		if(AUDIO_DAC_GAIN_IS_DONE())
			break;
		if(gm_GetSystemTime() - DW_TimerCheck > 2000)
			break;
	} 
	*/
	//gm_Print("===============Audio_RedoDACGain============= %d ", (WORD)gm_GetSystemTime());
	B_AudioTimerCtrl |= Aud_Tmr_AudSampleRateChange;
}

void OSD_AdjustBrigntnessContrast(void)
{

	  if(IsPresetMode_GRAPHICS_TEXT)
	  {
		UserPrefBrightness_Text = UserPrefBrightness;	
		UserPrefContrast_Text = UserPrefContrast;	
	  }
	  else if(IsColorSpace_GRAPHICS_CAL1)
	  {
	       UserPrefBrightness_CAL1= UserPrefBrightness;	
		UserPrefContrast_CAL1= UserPrefContrast;					
	   }
	  else if(IsColorSpace_GRAPHICS_CAL2)
	  {
	       UserPrefBrightness_CAL2= UserPrefBrightness;	
		UserPrefContrast_CAL2 = UserPrefContrast;
	   }
	  else
	  {
	       UserPrefBrightness_All = UserPrefBrightness;	
		UserPrefContrast_All = UserPrefContrast;
	  }
	  //gm_Print("=========UserPrefBrightness = %d " , UserPrefBrightness);
	  //gm_Print("=========UserPrefContrast = %d " , UserPrefContrast);
}

//20121001 yvonne for SoftMccs can support issue
BYTE EmptystrCapabilitiesRequest[]="";
void DDC_CI_CapabilitiesString(void)
{
	if(UserPrefDdcciOnOffFlag == ENABLE)
	{
		if (UserPrefCurrentInputMain == ALL_BOARDS_VGA1)
			SetCapabilitiesString(VGAvcpstr, GetCapabilitiesStringSize());
		else
		SetCapabilitiesString(strCapabilitiesRequest, GetCapabilitiesStringSize());
	}
	else
		SetCapabilitiesString(EmptystrCapabilitiesRequest, sizeof(EmptystrCapabilitiesRequest));
}

void RestorePresetModes_OSD(void)//owen 20121002
{	

	{
		if (B_PresetModesStore == UserPrefGraphicsImageMode)
			return;

		UserPrefGraphicsImageMode = B_PresetModesStore;

		//121009 Edward for DCR value save to game / movie mismatch 
		WB_ImageScheme = ConvertPresetMode(UserPrefGraphicsImageMode);
		
	}

	if(IsPresetMode_GRAPHICS_COLOR_SPACE)
		UserPrefPresetColorSpace = B_PresetColorSpace;
	
//20121022 Kordonm modify for change preset modes user data load error
if(IsPresetMode_GRAPHICS_TEXT)
	{
		UserPrefBrightness = UserPrefBrightness_Text;
		UserPrefContrast = UserPrefContrast_Text;
		UserPrefPortSharpness = UserPrefPortSharpness_Text;
	}
	else if(IsColorSpace_GRAPHICS_CAL1)//120806 Edward add for CAL1/CAL2 save seperate
	{
		UserPrefBrightness = UserPrefBrightness_CAL1;
		UserPrefContrast = UserPrefContrast_CAL1;
		UserPrefPortSharpness = UserPrefPortSharpness_All;
	}
	else if(IsColorSpace_GRAPHICS_CAL2)
	{
		UserPrefBrightness = UserPrefBrightness_CAL2;
		UserPrefContrast = UserPrefContrast_CAL2;
		UserPrefPortSharpness = UserPrefPortSharpness_All;
	}
	else
	{
		UserPrefBrightness = UserPrefBrightness_All;
		UserPrefContrast = UserPrefContrast_All;
		UserPrefPortSharpness = UserPrefPortSharpness_All;
	}

	if(IsPresetMode_GRAPHICS_MOVIE)
	{
			UserPrefHue = UserPrefHue_Movie;
			UserPrefSaturation = UserPrefSaturation_Movie;
	}
	else if(IsPresetMode_GRAPHICS_GAME)
	{
			UserPrefHue = UserPrefHue_Game;
			UserPrefSaturation = UserPrefSaturation_Game;
	}
	else
	{
			UserPrefHue = 0;
			UserPrefSaturation = WB_SATURATION_DEFAULT;
	}
	
	//AdjustPresetModes();
	SavePortSchemeDependentSettings();
}
#if (!defined(BOARD_93XX_PD)&&(!defined(BOARD_93XX_OV)))
WORD MaxMainDisplayNumber(void){return 1;};
WORD MaxPxpDisplayNumber(void){return 1;};
void PublicDisplaySetup(gmt_PHY_CH B_Channel, BOOL PD_Enable){UNUSED_VAR(B_Channel);UNUSED_VAR(PD_Enable);};
void AdjustPD_AutoBlank(void){};
void AdjustPublicDisplayCtrl(void){};
void AdjustPD_XNumber(void){};
BYTE GetXNumber(void){return 1;};
void AdjustPD_YNumber(void){};
BYTE GetYNumber(void){return 1;};
BYTE GetMaxBoardID(void){return 1;};
BYTE GetBoardID(void){return 1;};
void AdjustPD_DisplayNumber(void){};
BYTE GetPDNumber(void){return 1;};
BYTE GetPDChannel(void){return 0;};
BYTE GetAutoBlankCtrl(void){return 0;};
void Adj_Free_Zoom(BYTE part,WORD W_Percent)
{UNUSED_VAR(part);UNUSED_VAR(W_Percent);};
void Adj_Pixcel_Zoom(BYTE X_Number,BYTE Y_Number,BYTE DisplayNumber,BYTE Pixcel_NUMBER)
{UNUSED_VAR(X_Number);UNUSED_VAR(Y_Number);UNUSED_VAR(DisplayNumber);UNUSED_VAR(Pixcel_NUMBER);};

#endif	//defined(BOARD_93XX_PD)


void AdjustOSDBlending(void)
{
   WORD blendvalue;

   blendvalue = (gm_ReadRegWord(OSD_BLEND_TABLE0) & 0xFFF0) | UserPrefOSDG1Blend;

   gm_WriteRegWord(OSD_BLEND_TABLE0, blendvalue);
   gm_WriteRegWord(HOST_CONTROL, 0x0008);
}

void AdjustOSDBlending1(void)
{
   WORD blendvalue;

   blendvalue = (gm_ReadRegWord(OSD_BLEND_TABLE0) & 0xFF0F) | UserPrefOSDG2Blend << 4;

   gm_WriteRegWord(OSD_BLEND_TABLE0, blendvalue);
   gm_WriteRegWord(HOST_CONTROL, 0x0008);
}

void AdjustOSDBlending2(void)
{
   WORD blendvalue;

   blendvalue = (gm_ReadRegWord(OSD_BLEND_TABLE0) & 0xF0FF) | UserPrefOSDG3Blend << 8;

   gm_WriteRegWord(OSD_BLEND_TABLE0, blendvalue);
   gm_WriteRegWord(HOST_CONTROL, 0x0008);
}


extern void DrawResultDStaticText(BYTE B_TextIndex, BYTE B_ColorState);
#ifdef CONTROLTYPE_D2STATICTEXT_USED
#define DEFAULT_COLOR_STATE		0		// DStaticText
//******************************************************************************
//
// FUNCTION       : void DrawOption1D2StaticText(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption1D2StaticText(void)
{
   DrawResultDStaticText(0,DEFAULT_COLOR_STATE);
}

void DrawOption2D2StaticText(void)
{
   DrawResultDStaticText(1,DEFAULT_COLOR_STATE);
}
#endif

#ifdef CONTROLTYPE_D3STATICTEXT_USED
//******************************************************************************
//
// FUNCTION       : void DrawOption1DStaticText(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption1D3StaticText(void)
{
   DrawResultDStaticText(0,DEFAULT_COLOR_STATE);
}

void DrawOption2D3StaticText(void)
{
   DrawResultDStaticText(1,DEFAULT_COLOR_STATE);
}
#endif


#ifdef CONTROLTYPE_D2STATICTEXTBUTTON_USED
//******************************************************************************
//
// FUNCTION       : void DrawOption1DStaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption1D2StaticTextButton(void)
{
   DrawResultDStaticText(0,DEFAULT_COLOR_STATE);
}
//******************************************************************************
//
// FUNCTION       : void DrawOption1D2StaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption2D2StaticTextButton(void)
{
   DrawResultDStaticText(1,DEFAULT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawSelectedOption1DStaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawSelectedOption1D2StaticTextButton(void)
{
   DrawResultDStaticText(0,SELECT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawSelectedOption2D2StaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawSelectedOption2D2StaticTextButton(void)
{
   DrawResultDStaticText(1,SELECT_COLOR_STATE);
}
#endif

#ifdef CONTROLTYPE_D3STATICTEXTBUTTON_USED
//******************************************************************************
//
// FUNCTION       : void DrawOption1D3StaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption1D3StaticTextButton(void)
{
   DrawResultDStaticText(0,DEFAULT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawOption2D3StaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption2D3StaticTextButton(void)
{
   DrawResultDStaticText(1,DEFAULT_COLOR_STATE);
}
//******************************************************************************
//
// FUNCTION       : void DrawOption3D3StaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption3D3StaticTextButton(void)
{
   DrawResultDStaticText(2,DEFAULT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawSelectedOption1D3StaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawSelectedOption1D3StaticTextButton(void)
{
   DrawResultDStaticText(0,SELECT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawSelectedOption2D3StaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawSelectedOption2D3StaticTextButton(void)
{
   DrawResultDStaticText(1,SELECT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawSelectedOption3D3StaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawSelectedOption3D3StaticTextButton(void)
{
   DrawResultDStaticText(2,SELECT_COLOR_STATE);
}

#endif


#ifdef CONTROLTYPE_D4STATICTEXTBUTTON_USED
//******************************************************************************
//
// FUNCTION       : void DrawOption1D4StaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption1D4StaticTextButton(void)
{
   DrawResultDStaticText(0,DEFAULT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawOption2D4StaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption2D4StaticTextButton(void)
{
   DrawResultDStaticText(1,DEFAULT_COLOR_STATE);
}
//******************************************************************************
//
// FUNCTION       : void DrawOption3D4StaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption3D4StaticTextButton(void)
{
   DrawResultDStaticText(2,DEFAULT_COLOR_STATE);
}
//******************************************************************************
//
// FUNCTION       : void DrawOption4D4StaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption4D4StaticTextButton(void)
{
   DrawResultDStaticText(3,DEFAULT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawSelectedOption1D4StaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawSelectedOption1D4StaticTextButton(void)
{
   DrawResultDStaticText(0,SELECT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawSelectedOption2D4StaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawSelectedOption2D4StaticTextButton(void)
{
   DrawResultDStaticText(1,SELECT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawSelectedOption3D4StaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawSelectedOption3D4StaticTextButton(void)
{
   DrawResultDStaticText(2,SELECT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawSelectedOption4D4StaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawSelectedOption4D4StaticTextButton(void)
{
   DrawResultDStaticText(3,SELECT_COLOR_STATE);
}

#endif

void AdjustCurrentMainInputDIP(void)
{
	//if(UserPrefCurrentInputMain != ALL_BOARDS_DIP1)
	#ifdef TVP5158_DEV_ID
	UserPrefCurrentInputMain = ALL_BOARDS_DIP1;
	TVP5158_1CH_ChangeToChannel(UserPrefCurrentInputMainDIP);
	#endif
	AdjustCurrentInputMain();
	//gm_ForceModeChanged(gmvb_MainChannel);
}

void AdjustCurrentPxpInputDIP(void)
{
	#ifdef TVP5158_DEV_ID
	UserPrefCurrentInputPip = ALL_BOARDS_DIP1;
	TVP5158_1CH_ChangeToChannel(UserPrefCurrentInputPxpDIP);
	#endif
	AdjustCurrentInputPip();
	//gm_ForceModeChanged(gmvb_PipChannel);
}
//Neil121128+
void ReInitHDMIVDDS(gmt_PHY_CH B_Channel)
{
	BYTE B_InputConnector = (B_Channel == CH_A) ?  UserPrefCurrentInputMain : UserPrefCurrentInputPip;
	
      	if ((GetPortConnType(B_InputConnector) == HDMI))// || (GetPortConnType(B_InputConnector) == DVI))
      	{
		gm_WriteRegWord(HDMI_CLK_CFG,(0x2B0|HDMI_VCLK_SEL_VDDS1_CLK));	//Neil121128+ default set to AVDDS1
		if(B_Channel == CH_A)
		{
			gm_WriteRegWord(HDMI_CLK_CFG,0x2B2);
			//gm_WriteRegWord(DVI_CLK_CFG,0);
			gm_WriteRegWord(AVDDS1_CONTROL0,0x3048);
			gm_WriteRegWord(AVDDS1_CONTROL1,(0xC3|AVDDS1_TRK_ERR_FILT_EN));
	      		gm_WriteRegWord(AVDDS1_FREQ_DELTA,0);
      			gm_WriteRegByte(AVDDS1_INIT,0x11);		
		}
		else
		{
			gm_WriteRegWord(HDMI_CLK_CFG,(0x3B0|HDMI_VCLK_SEL_VDDS2_CLK));//Neil121128+ pxp set to AVDDS2
			gm_WriteRegWord(AVDDS2_CONTROL0,0x3048);
			gm_WriteRegWord(AVDDS2_CONTROL1,(0xC3|AVDDS1_TRK_ERR_FILT_EN));
	      		gm_WriteRegWord(AVDDS2_FREQ_DELTA,0);
      			gm_WriteRegByte(AVDDS2_INIT,0x1);		
		}
	}

}



void CheckSystemMessageState(gmt_PHY_CH B_Channel)
{
	MessageToOsdType MTO_Message = MTO_Empty; 
	#ifdef CHECK_IS_CABLE_CONNECT
	if(gm_TimingIsStable(B_Channel))
	{
               OSDRequestCancelMsg(gmvb_MainChannel, MTO_NoCable);
               OSDRequestCancelMsg(gmvb_MainChannel, MTO_NoSync);
		if((stInput[(B_Channel)].ModeFlags & gmd_MODE_OUT_RANGE) == gmd_MODE_OUT_RANGE)
			MTO_Message = MTO_OutOfRange;
		else if((stInput[(B_Channel)].ModeFlags & gmd_MODE_VALID) == gmd_MODE_VALID)
			MTO_Message = MTO_ValidMode;
		else
			MTO_Message = MTO_InvalidMode;
	}
	else
	{
		CableInfo CableUpdate;
		
		Get_CableStatus(&CableUpdate);
		gm_Delay10ms(3);
		if(!isCableConnected((B_Channel == CH_A)? UserPrefCurrentInputMain : UserPrefCurrentInputPip)) // SCR2456
			MTO_Message = MTO_NoCable;
		else
			MTO_Message = MTO_NoSync;		
	}
	#else
	MTO_Message = MTO_NoSync;		
	#endif
	//gm_Printf("11======>>>B_Channel = %d; MTO_Message = %d ", B_Channel, MTO_Message);
	if(MTO_Message != MTO_Empty)
	{
		if(B_Channel == gmvb_MainChannel)
			OSDRequestMsg(B_Channel, MTO_Message, NO_SIGNAL_DELAY);
		else
			OSDRequestMsg_Pxp(B_Channel, MTO_Message, NO_SIGNAL_DELAY);
	}
}


#ifdef USER_IR_SUPPORT
BYTE IR_SearchKey( WORD W_Cmd, WORD W_Data)
{
	BYTE IR_Value = 0;
	USER_IRType ROM* IR_Ptr = USER_IR_Data; 
	
	//gm_Printf(" SearchIRKey:  W_Cmd = 0x%x;  W_Data = 0x%x ",W_Cmd, W_Data);
	#if  defined( IR_REMOTE_RC5_EH)
	if((W_Cmd < 0x1F)&&(W_Cmd > 0x1b))
	#elif defined( IR_REMOTE_RC5_KTC)
	if(W_Cmd < 0x1F)
	#elif (defined( IR_REMOTE_GENERIC_RECS80_KTC))
	if(W_Cmd < 0x1F)		
	#elif (defined( IR_REMOTE_GENERIC_RECS80_NH))
	if(W_Cmd == 0x7D)	
	#endif
	{
		for(; ; IR_Ptr++)
		{
			#ifdef IR_REMOTE_GENERIC_RECS80_SKYWORTH
			if(IR_Ptr->IR_Value == 0xFF)	//End of table, found nothing, 
				return 0xFF;
			#else
			if(IR_Ptr->IR_CMD == 0xFF)	//End of table, found nothing, 
				return 0xFF;
			#endif
			
			if((W_Cmd == IR_Ptr->IR_CMD)&&(W_Data == IR_Ptr->IR_Value))//USER_IR_Data[i].IR_CMD)
			{
				IR_Value = IR_Ptr->WBKey;
				break;
			}
		}
		//gm_Print("SearchIRKey: IR_Value = 0x%x", IR_Value);
		//return IR_Value; 
	}
	return IR_Value; 
	//return 0xFF;
}

#define IR_Max 	4//10
void IR_EnterFactory(BYTE IR_Value)
{
	static BYTE IR_Key = 0;
	static BYTE IR_EnterFac_Processing = gmd_FALSE;
	BYTE IR_CheckTime;

	#ifdef IR_REMOTE_RC5_KTC
		IR_CheckTime = 10;
	#else
		IR_CheckTime = 5; //15;
	#endif
	if((IR_Value == 0x00)||(IR_Value == 0xFF))
		return;
	
	if((IR_Value == IRKey_Factory[IR_Key])&&(IR_EnterFac_Processing == gmd_FALSE))
	{
		//gm_Print("11111 IR_EnterFactory: Start ===> %d ",IR_Key);
		IR_EnterFac_Processing = gmd_TRUE;
		IR_Key = 0;
		gm_TimerStart(Key_Check_1s_TMR,IR_CheckTime);
	}
	else if((gm_TimerCheck(Key_Check_1s_TMR) == TIMER_OK)&&(IR_EnterFac_Processing == gmd_TRUE))
	{
		//gm_Print("22222 IR_EnterFactory: Processing IR_Key ===> %d ",IR_Key);
		//gm_Print("???????????IR_EnterFactory: Processing IR_Key ===> 0x%x ",IRKey_Factory[IR_Key]);
		//IR_EnterFactoryCount +=1;
		if(IR_Value == IRKey_Factory[++IR_Key])
			IR_EnterFac_Processing = gmd_TRUE;
		else
			IR_EnterFac_Processing = gmd_FALSE;
	
		if((IR_Value == IRKey_Factory[IR_Max -1])&&(IR_EnterFac_Processing == gmd_TRUE)&&(IR_Key == (IR_Max-1)))  //wei 140520 for enter factory
		{
			//gm_Print("33333 IR_EnterFactory: Done ===> %d ",IR_Key);
			OSDRequestMsg(gmvb_MainChannel, MTO_RemoveOSD, REQUEST_NOW);
			UserPrefFactoryFlag= gmd_TRUE;
			UserPrefRunBurnInModeFlag = gmd_TRUE;
		}
	}
	else
	{
		IR_Key = 0;
		IR_EnterFac_Processing = gmd_FALSE;
		gm_TimerStop(Key_Check_1s_TMR);
	}

}
#endif

//Keypad enter factory sequence : Right==>Up==>Up==>Down==>Down==>Left==>Right==>Left==>Right;
ROM BYTE B_Factory_Key_Map[][2] = {{1,57},{1,85},{1,85},{1,193},{1,193},{1,112},{1,57},{1,112},{1,57}};
//Keypad enter factory sequence : Source==>Down==>Down==>Up==>Up==>Left==>Source==>Left==>Source;
ROM BYTE RestorePanel_Key_Map[][2] = {{0,147},{1,193},{1,193},{1,85},{1,85},{1,112},{0,147},{1,112},{0,147}};
BOOL Keypad_SpecialKey(BYTE KeyCh, BYTE KeyVal, BYTE far* SpecialKey, BYTE SK_Len)
{
	BYTE i;
	BYTE CheckTimer = 15;
	BOOL B_Return = gmd_FALSE;
	static BYTE B_F_Counter = 0;
	static BYTE B_F_Buffer[10][2];
	static BYTE SpecialKey_Flag = 0;
	static BYTE B_F_Counter_Flag = 0;

	

	//if(((gm_ReadRegWord(OSD_CONTROL) & BIT1) == 0) && ((KeyVal == B_Factory_Key_Map[0])&&(KeyCh==1)))
	
	if((B_F_Counter_Flag == 0) && ((KeyCh==SpecialKey[0])&&(KeyVal == SpecialKey[1])))
	{
		B_F_Counter = 0;
		B_F_Counter_Flag = 1;
		B_F_Buffer[B_F_Counter][0] = KeyCh;
		B_F_Buffer[B_F_Counter][1] = KeyVal;		
		gm_TimerStart(Key_Check_1s_TMR,CheckTimer);
	}

	if(gm_TimerCheck(Key_Check_1s_TMR) == TIMER_OK)
	{
		if(KeyVal)
		{
			if((B_F_Counter_Flag == 1)&&(B_F_Buffer[0][1] == SpecialKey[1]))
			{
				B_F_Buffer[B_F_Counter][0] = KeyCh;
				B_F_Buffer[B_F_Counter][1] = KeyVal;
				B_F_Counter++;
			}
		}
			
		if(B_F_Counter == SK_Len)
		{
			SpecialKey_Flag = 1;
			B_F_Counter_Flag = 0;
			B_F_Counter = 0;
			for(i = 0;i< SK_Len;i++)
			{
				//gm_Printf("==%d== Key = %d; SK = %d",i, B_F_Buffer[i][1], SpecialKey[2*i+1]);
				if((B_F_Buffer[i][0] != SpecialKey[2*i])||(B_F_Buffer[i][1] != SpecialKey[2*i+1]))
					SpecialKey_Flag = 0;
			}
		}
	}
	else
	{
		B_F_Counter = 0;
		B_F_Counter_Flag = 0;
		SpecialKey_Flag = 0;
		gm_TimerStop(Key_Check_1s_TMR);
	}
	
	if(SpecialKey_Flag == 1)
	{
		B_Return = gmd_TRUE;
		//gm_Print("=====================>SpecialKey_Flag is true!!",0);
		SpecialKey_Flag = 0;
		gm_TimerStop(Key_Check_1s_TMR);
	}
	return B_Return;
}
void Keypad_EnterFactory(BYTE KeyCh, BYTE KeyVal)// BYTE * SpecialKey, BYTE SK_Len)
{
	{
		if(Keypad_SpecialKey(KeyCh,KeyVal,(BYTE far*)B_Factory_Key_Map,sizeof(B_Factory_Key_Map)/2))
		{
			UserPrefFactoryFlag = 1;
			UserPrefRunBurnInModeFlag = 1;
		}
	}
}


void AdjustPxpBlendLevel(void)
{

	WORD W_Temp;

	W_Temp = gm_ReadRegWord(PB_CTRL);

    //
    // Consider only the 4 LSBs of B_BlendLevel as the level of blending is a 4
    // bit value
    //
    if (UserPrefPxpBlendLevel> 15)
    	UserPrefPxpBlendLevel = 15;
    //
    // Clear the previous value of blend level and load the new value
    //
	W_Temp = (W_Temp & (~(WORD)(PB_BLEND_PIP_MAIN))) | (UserPrefPxpBlendLevel << 4);
	gm_WriteRegWord(PB_CTRL, W_Temp);
	gm_SyncUpdate(gmvb_PipChannel, OUTPUT_CLOCK);
}


BYTE GetProjIDInfo(void)
{
	#if (PROJECT_SELECT == PROJ_SUV)
	   return PROJ_SUV;
	#elif (PROJECT_SELECT == PROJ_MNT)
	   	return PROJ_MNT;	
	#elif (PROJECT_SELECT == PROJ_4K2K)
	   return PROJ_4K2K;	
	#else
	   return PROJ_MNT

	
	#endif
}

BYTE Random_BYTE(BYTE B_Max)
{
	static DWORD Rand = 0xFF;
	DWORD DW_Time = gm_GetSystemTime() + Rand;

	Rand = (DWORD)Rand * DW_Time + 123;
	
	return (BYTE)(Rand%B_Max);
}


WORD Random_WORD(WORD B_Max)
{
	static DWORD W_Rand;
	DWORD DW_Time = gm_GetSystemTime();
	BYTE i;
	
	for(i = 0; i< Random_BYTE(7)+3; i++)
		W_Rand = (DWORD)W_Rand * DW_Time +678;
	
	return (WORD)(W_Rand%B_Max);
}


BOOL IsFactoryMode(void)
{
	return (UserPrefFactoryFlag == gmd_TRUE);
}

void AdjustOsdZoom(void) 
{
         BYTE i;
         WORD xpos, ypos;

         //turn osd zoom on
         if(UserPrefOSDZoom == ON)
         {
            gm_SetRegBitsWord(OSD_CONTROL, OSD_ZOOM);
            gm_WriteRegWord(OSD_H_ACTIVE, (gm_ReadRegWord(OSD_H_ACTIVE) >> 1));
            gm_WriteRegWord(OSD_V_ACTIVE, (gm_ReadRegWord(OSD_V_ACTIVE) >> 1));
            gm_SetRegBitsByte(HOST_CONTROL, ODP_FORCE_UPDATE);

            for(i = 0 ; i < MAX_NUMBER_OF_TILES_USED ; i++)
            {
               if( OK == gm_OsdGetTilePosition(i, &xpos, &ypos))
               {
                  gm_OsdSetTilePosition(i, xpos >> 1, ypos >> 1);
               }
            }
            ReverseTileSet();
            gm_SetRegBitsByte(HOST_CONTROL, ODP_FORCE_UPDATE);
            for(i = 0 ; i < MAX_NUMBER_OF_TILES_USED ; i++)
            {
               if( OK == gm_OsdGetTilePosition(i, &xpos, &ypos))
               {
                  gm_OsdSetTilePosition(i, xpos >> 1, ypos >> 1);
               }
            }
         }
         //turn osd zoom off
         else
         {
            gm_ClearRegBitsWord(OSD_CONTROL, OSD_ZOOM);
            gm_WriteRegWord(OSD_H_ACTIVE, (gm_ReadRegWord(OSD_H_ACTIVE) << 1));
            gm_WriteRegWord(OSD_V_ACTIVE, (gm_ReadRegWord(OSD_V_ACTIVE) << 1));
            gm_SetRegBitsByte(HOST_CONTROL, ODP_FORCE_UPDATE);

            for(i = 0 ; i < MAX_NUMBER_OF_TILES_USED ; i++)
            {
               if( OK == gm_OsdGetTilePosition(i, &xpos, &ypos))
               {
                  gm_OsdSetTilePosition(i, xpos << 1, ypos << 1);
               }
            }
            ReverseTileSet();
            gm_SetRegBitsByte(HOST_CONTROL, ODP_FORCE_UPDATE);
            for(i = 0 ; i < MAX_NUMBER_OF_TILES_USED ; i++)
            {
               if( OK == gm_OsdGetTilePosition(i, &xpos, &ypos))
               {
                  gm_OsdSetTilePosition(i, xpos << 1, ypos << 1);
               }
            }
         }
}

void EraseUnicodeRangeText(void)
{
	EraseControl();
}
extern gmt_RET_STAT gm_DisplayForceBkgnd_Customer (gmt_PHY_CH B_Channel, BYTE B_Enable, WORD W_BkgndColor);
void SetBackGroundColor(FORCE_BACKGROUND Color)
{
	gm_ClearAndSetBitsWord(PB_CTRL, PB_MAIN_ENABLE,PB_FORCE_MAIN_BKGND);
	//gm_WriteRegWord(PB_MAIN_HWIDTH, PanelWidth);
	//gm_WriteRegWord(PB_MAIN_VHEIGHT, PanelHeight);			
	//gm_WriteRegWord(PB_MAIN_HSTART, 0);
	//gm_WriteRegWord(PB_MAIN_VSTART, 0); 	
	switch(Color)
	{
		case FORCE_BACKGROUND_BLACK:
			gm_WriteRegWord(PB_BKGND, 0x0000); //Black
		break;
		case FORCE_BACKGROUND_WHITE:
			gm_WriteRegWord(PB_BKGND, 0xFFFF); //blue
		break;
		case FORCE_BACKGROUND_RED:
			gm_WriteRegWord(PB_BKGND, 0xF800); //blue
		break;
		case FORCE_BACKGROUND_GREEN:
			gm_WriteRegWord(PB_BKGND, 0x3E0); //Green
		break;
		case FORCE_BACKGROUND_BLUE:
			gm_WriteRegWord(PB_BKGND, 0x001F); //blue
		break;
		case FORCE_BACKGROUND_GRAY:
			gm_WriteRegWord(PB_BKGND, 0x632C); //blue
		break;
		case FORCE_BACKGROUND_YELLOW:
			gm_WriteRegWord(PB_BKGND, 0xFFE0); //blue
		break;
		case FORCE_BACKGROUND_MAGENTA:
			gm_WriteRegWord(PB_BKGND, 0xF81F); //blue
		break;
		case FORCE_BACKGROUND_CYAN:
			gm_WriteRegWord(PB_BKGND, 0x07FF); //blue
		break;		
		default:
			gm_DisplayForceBkgnd_Customer(CH_A, !gm_IsValidSignal(CH_A),  RGB_565(MAIN_BACKGROUND_COLOR));
			break;
	}
	gm_SyncUpdateWait(CH_A, OUTPUT_CLOCK);
}

void ClearBackGroundColor(void)
{
	gm_ClearAndSetBitsWord(PB_CTRL,PB_FORCE_MAIN_BKGND, PB_MAIN_ENABLE);
	//gm_WriteRegWord(PB_MAIN_HWIDTH, PanelWidth);
	//gm_WriteRegWord(PB_MAIN_VHEIGHT, PanelHeight);			
	//gm_WriteRegWord(PB_MAIN_HSTART, 0);
	//gm_WriteRegWord(PB_MAIN_VSTART, 0); 	
	gm_WriteRegWord(PB_BKGND, RGB_565(MAIN_BACKGROUND_COLOR)); //Black
	gm_SyncUpdateWait(CH_A, OUTPUT_CLOCK);
}


BYTE GetNextPxpMode(void)
{
	BYTE NextPxpMode = NO_PIP;
	
	switch(UserPrefPipMode)
	{
		case NO_PIP:
			NextPxpMode = SMALL_PIP;
		break;
		case SMALL_PIP:
			NextPxpMode = LARGE_PIP;
		break;
		case LARGE_PIP:
			NextPxpMode = NO_PIP;   //Wei 140819  SIDE_BY_SIDE;
		break;
		case SIDE_BY_SIDE:
			NextPxpMode = NO_PIP;
		break;
		case SIDE_BY_SIDE_TALL:
			NextPxpMode = NO_PIP;
		break;
		default:
		break;
	}
	return NextPxpMode;
}

BYTE GetPrevPxpMode(void)
{
	BYTE PrevPxpMode = NO_PIP;
	
	switch(UserPrefPipMode)
	{
		case NO_PIP:
			PrevPxpMode = LARGE_PIP; //Wei 140819   SIDE_BY_SIDE;
		break;
		case SMALL_PIP:
			PrevPxpMode = NO_PIP;
		break;
		case LARGE_PIP:
			PrevPxpMode = SMALL_PIP;
		break;
		case SIDE_BY_SIDE:
			PrevPxpMode = LARGE_PIP;
		break;
		case SIDE_BY_SIDE_TALL:
			PrevPxpMode = SIDE_BY_SIDE;
		break;
		default:
		break;
	}
	return PrevPxpMode;
}

BYTE GetNextPresetMode(void)
{
	BYTE NextPresetMode = Graphics_STANDARD;
	
	gm_Print(" UserPrefGraphicsImageMode  = %d ",UserPrefGraphicsImageMode);
	switch(UserPrefGraphicsImageMode)
	{
		default:
		case Graphics_STANDARD:
			NextPresetMode = Graphics_MULTIMEDIA;
		break;
		case Graphics_MULTIMEDIA:
			NextPresetMode = Graphics_GAME;
		break;
		case Graphics_GAME:
			NextPresetMode = Graphics_MOVIE;
		break;
		#if 1//(!IS_PROJ_PD())
		case Graphics_MOVIE:
			NextPresetMode = Graphics_STANDARD;
		break;
		#else
		case Graphics_MOVIE:
			NextPresetMode = Graphics_TEXT;
		break;
		case Graphics_TEXT:
			NextPresetMode = Graphics_COLOR_TEMP;
		break;
		case Graphics_COLOR_TEMP:
			NextPresetMode = Graphics_COLOR_SPACE;
		break;
		case Graphics_COLOR_SPACE:
			NextPresetMode = Graphics_CUSTOM_COLOR;
		break;
		case Graphics_CUSTOM_COLOR:
			NextPresetMode = Graphics_STANDARD;
		break;

		#endif

	}
	gm_Print(" NextPresetMode  = %d ",NextPresetMode);
	return NextPresetMode;
}

BYTE GetPrevPresetMode(void)
{
	BYTE PrevPresetMode = Graphics_STANDARD;
	
	switch(UserPrefGraphicsImageMode)
	{
		default:
		case Graphics_STANDARD:
			PrevPresetMode = Graphics_MOVIE;
		break;
		case Graphics_MULTIMEDIA:
			PrevPresetMode = Graphics_STANDARD;
		break;
		case Graphics_GAME:
			PrevPresetMode = Graphics_MULTIMEDIA;
		break;
		case Graphics_MOVIE:
			PrevPresetMode = Graphics_GAME;
		break;
	}
	return PrevPresetMode;
}

BYTE GetNextGammaTable(void)
{
#if USE_NEW_GAMMA
		BYTE NextGamma = Gamma_PC;
	
		switch(UserPrefGammaMode)
		{
			default:
			case Gamma_PC:
				NextGamma = Gamma_MAC;
			break;
			case Gamma_MAC:
				NextGamma = Gamma_PC;
			break;
		}
		return NextGamma;
		
#else
	
	BYTE NextGamma = GAMMA_TABLE_OFF;
	
	//gm_Print(" UserPrefGammaTable  = %d ",UserPrefGammaTable);
	switch(UserPrefGammaTable)
	{
		default:
		case GAMMA_TABLE_OFF:
			NextGamma = GAMMA_TABLE_LINEAR;
		break;
		case GAMMA_TABLE_LINEAR:
			NextGamma = GAMMA_TABLE_22;
		break;
		case GAMMA_TABLE_22:
			NextGamma = GAMMA_TABLE_24;
		break;
		case GAMMA_TABLE_24:
			NextGamma = GAMMA_TABLE_SCV;
		break;
		case GAMMA_TABLE_SCV:
			NextGamma = GAMMA_TABLE_OFF;
		break;
	}
	//gm_Print(" NextGamma  = %d ",NextGamma);
	return NextGamma;
#endif	
}

BYTE GetPrevGammaTable(void)
{
  	#if USE_NEW_GAMMA
		BYTE PrevGamma = Gamma_PC;
	
		switch(UserPrefGammaMode)
		{
			default:
			case Gamma_PC:
				PrevGamma = Gamma_MAC;
			break;
			case Gamma_MAC:
				PrevGamma = Gamma_PC;
			break;

		}
		return PrevGamma;
	#else
		BYTE PrevGamma = GAMMA_TABLE_OFF;
		
		switch(UserPrefGammaTable)
		{
		default:
		case GAMMA_TABLE_OFF:
			PrevGamma = GAMMA_TABLE_SCV;
		break;
		case GAMMA_TABLE_LINEAR:
			PrevGamma = GAMMA_TABLE_OFF;
		break;
		case GAMMA_TABLE_22:
			PrevGamma = GAMMA_TABLE_LINEAR;
		break;
		case GAMMA_TABLE_24:
			PrevGamma = GAMMA_TABLE_22;
		break;
		case GAMMA_TABLE_SCV:
			PrevGamma = GAMMA_TABLE_24;
		break;
	}
		return PrevGamma;

	#endif
}



BYTE GetNextPxpPostion(void)
{
	BYTE NextPxpPos = 0;
	
	switch(UserPrefPipPosition)
	{
		case TOP_LEFT:
			NextPxpPos = TOP_RIGHT;
		break;
		case TOP_RIGHT:
			NextPxpPos = BOTTOM_RIGHT;
		break;
		case BOTTOM_LEFT:
			NextPxpPos = TOP_LEFT;
		break;
		case BOTTOM_RIGHT:
			NextPxpPos = BOTTOM_LEFT;
		break;
		//case SIDE_BY_SIDE_TALL:
		//	NextPxpMode = SMALL_PIP;
		//break;
		default:
		break;
	}
	return NextPxpPos;
}

BYTE GetPrevPxpPosition(void)
{
	BYTE PrevPxpPos = NO_PIP;
	
	switch(UserPrefPipPosition)
	{
		case TOP_LEFT:
			PrevPxpPos = BOTTOM_LEFT;
		break;
		case TOP_RIGHT:
			PrevPxpPos = TOP_LEFT;
		break;
		case BOTTOM_LEFT:
			PrevPxpPos = BOTTOM_RIGHT;
		break;
		case BOTTOM_RIGHT:
			PrevPxpPos = TOP_RIGHT;
		break;
		//case SIDE_BY_SIDE_TALL:
		//	NextPxpMode = SMALL_PIP;
		//break;
		default:
		break;
	}
	return PrevPxpPos;
}
void SetPxpModePosition(void)
{
	WORD	W_HActiveMain = ((PanelWidth - (3 * PIP_BORDER_WIDTH)) / 2) + SW_DynScalePosition;
		W_HActiveMain &= ~1;
		if(UserPrefPIPScalerSW)
			{
			switch (UserPrefPipPosition)
				{
				case BOTTOM_LEFT:
				case BOTTOM_RIGHT:
				case TOP_LEFT:
				case TOP_RIGHT:				
				gmvw_OutputHStart[gmvb_PipChannel] = PIP_WINDOW_HORIZONTAL_OFFSET;
				gmvw_OutputVStart[gmvb_PipChannel] = PIP_WINDOW_VERTICAL_OFFSET;
				break;
				default:
					break;
				}
			}
		else if(UserPrefPipAutoScalerSW)
			{
			switch (UserPrefPipPosition)
			{
			// by redy for show pip all screen 20140828 
			//add - PIP_BORDER_WIDTH*2 ane PIP_BORDER_HEIGHT*2
				case BOTTOM_RIGHT:
					gmvw_OutputHStart[gmvb_PipChannel] = PanelWidth - gmvw_OutputHActive[gmvb_PipChannel] - PIP_WINDOW_HORIZONTAL_OFFSET-PIP_BORDER_WIDTH*2; 
					gmvw_OutputVStart[gmvb_PipChannel] = PanelHeight - gmvw_OutputVActive[gmvb_PipChannel] - PIP_WINDOW_VERTICAL_OFFSET-PIP_BORDER_HEIGHT*2;
					break;
				case TOP_RIGHT:
					gmvw_OutputHStart[gmvb_PipChannel] = PanelWidth - gmvw_OutputHActive[gmvb_PipChannel] - PIP_WINDOW_HORIZONTAL_OFFSET-PIP_BORDER_WIDTH*2;
					gmvw_OutputVStart[gmvb_PipChannel] = PIP_WINDOW_VERTICAL_OFFSET;
					break;
				case TOP_LEFT:
					gmvw_OutputHStart[gmvb_PipChannel] = PIP_WINDOW_HORIZONTAL_OFFSET;
					gmvw_OutputVStart[gmvb_PipChannel] = PIP_WINDOW_VERTICAL_OFFSET;
					break;
				case BOTTOM_LEFT:
					gmvw_OutputHStart[gmvb_PipChannel] = PIP_WINDOW_HORIZONTAL_OFFSET;
					gmvw_OutputVStart[gmvb_PipChannel] = PanelHeight - gmvw_OutputVActive[gmvb_PipChannel] - PIP_WINDOW_VERTICAL_OFFSET-PIP_WINDOW_VERTICAL_OFFSET-PIP_BORDER_HEIGHT*2;
					break;
				default:
					break;
			}
					}
		else{
			switch (UserPrefPipPosition)
				{
				case BOTTOM_LEFT:
				case BOTTOM_RIGHT:
				case TOP_LEFT:
				case TOP_RIGHT:
				gmvw_OutputHStart[gmvb_PipChannel] = UserPrefSerlCmdPanelHStart;//PanelWidth - gmvw_OutputHActive[gmvb_PipChannel] - PIP_WINDOW_HORIZONTAL_OFFSET-PIP_BORDER_WIDTH*2; 
				gmvw_OutputVStart[gmvb_PipChannel] = UserPrefSerlCmdPanelVStart;//PanelHeight - gmvw_OutputVActive[gmvb_PipChannel] - PIP_WINDOW_VERTICAL_OFFSET-PIP_BORDER_HEIGHT*2;
				break;
				default:
					break;
				}
			}
}


#if(FEATURE_LAMP == ENABLE)
static WORD W_PBReg;
void AdjustBKG_Lamp(void)
{
	gm_ClearAndSetBitsByte(PB_CTRL, PB_MAIN_ENABLE,PB_FORCE_MAIN_BKGND);
	gm_WriteRegWord(PB_MAIN_HWIDTH, PanelWidth);
	gm_WriteRegWord(PB_MAIN_VHEIGHT, PanelHeight);			
	gm_WriteRegWord(PB_MAIN_HSTART, 0);
	gm_WriteRegWord(PB_MAIN_VSTART, 0); 
	
	switch(UserPrefLampBrightness)
	{
		case 0x00:
			gm_WriteRegWord(PB_BKGND, 0x632C); 	
		break;
		case 0x01:
			gm_WriteRegWord(PB_BKGND, 0x8C51); 
		break;
		case 0x02:
			gm_WriteRegWord(PB_BKGND, 0xAD95);
		break;
		case 0x03:
			gm_WriteRegWord(PB_BKGND, 0xD6FA); 
		break;
		case 0x04:
			gm_WriteRegWord(PB_BKGND, 0xE77C); 
		break;
		case 0x05:
			gm_WriteRegWord(PB_BKGND, 0xFFFF);
		break;
		default:
			gm_WriteRegWord(PB_BKGND, 0xFFFF); 
		break;

			
	}
	gm_SyncUpdateWait(CH_A, OUTPUT_CLOCK);
}
void EnterLampState(void)
{
	W_PBReg = gm_ReadRegWord(PB_CTRL);
	AdjustBKG_Lamp();
}
void ExitLampState(void)
{
	gm_WriteRegWord(PB_CTRL, W_PBReg);
	gm_SyncUpdateWait(CH_A, OUTPUT_CLOCK);
}
#else
void AdjustBKG_Lamp(void){}
void EnterLampState(void){}
void ExitLampState(void){}
#endif	//FEATURE_LAMP




//******************************************************************************
// FUNCTION       : gmt_DPRX_DID GetDpDigitalID(gmt_UserDefConnNames InputConnector)
// USAGE          : Called for Get Digital PHY ID of a DP input port
// INPUT          : gmt_UserDefConnNames InputConnector
// OUTPUT         : gmt_DPRX_DID (DP Digital Phy ID)
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  : 
// POST_CONDITION : 
//******************************************************************************
gmt_DipPins GetDIPPins(gmt_UserDefConnNames B_InputConnector)
{
	if(GetPortConnType(B_InputConnector)==DIPX)
		return ((gmt_DipProperties far *)(GetPortPropertyTable(B_InputConnector)))->B_DipPins[0];
	return DIP_NONE;
}

//Neil130103+ This function working after Auto success, HTotal and VTotal should be correct,
//Try to find the Best V position for HP8200 Elite Ultra-slim desktop, this set have noise about > 128
// in V blanking area;
#define IBD_DET_THRESH_SHIFT 4
void CheckVideoParaForActive(gmt_PHY_CH B_Channel)
{
	//if(((abs(pS_IcpParam->W_InputHActWidth -  1680 < 3)) &&(abs(pS_IcpParam->W_InputVActWidth -  1050 < 3)))
	//	||((abs(pS_IcpParam->W_InputHActWidth -  1440 < 3)) &&(abs(pS_IcpParam->W_InputVActWidth -  900 < 3))))
	{
		BYTE B_Color = 0;
		BYTE B_IBD_THOLD; 
		WORD W_IBD_VStart, W_IBD_VActWidth_1, W_IBD_VActWidth_2;
		gm_Print(">CheckVideoParaAfterAuto  == Native Mode",0);


		B_IBD_THOLD = (gm_ReadRegByte(ip_ibd_control[B_Channel])&IMP_DET_THOLD_SEL) >> 4;
		
		gm_Print(">CheckVideoParaAfterAuto  =B_IBD_THOLD = %d",B_IBD_THOLD);
		if(B_IBD_THOLD < 5)
			B_IBD_THOLD += 4;
		for(; B_IBD_THOLD> 3; B_IBD_THOLD--)
		{
			gm_SetRegBitsWord(ip_ibd_control[B_Channel], (B_IBD_THOLD << IBD_DET_THRESH_SHIFT));
			//W_IBD_VStart = gm_ReadRegWord(ip_ibd_vstart[B_Channel]) & 0x3FF;
			W_IBD_VActWidth_1 = abs(gm_ReadRegWord(ip_ibd_vlength[B_Channel]) - stInput[B_Channel].Vactive);
			for(B_Color  =  1; B_Color < 4; B_Color++)
			{
				gm_SetRegBitsWord(ip_ibd_control[B_Channel], (B_Color <<2)|(B_IBD_THOLD << IBD_DET_THRESH_SHIFT));
				gm_Delay1ms(16);
				W_IBD_VActWidth_2 =  abs(gm_ReadRegWord(ip_ibd_vlength[B_Channel]) - stInput[B_Channel].Vactive);
				if(W_IBD_VActWidth_2 <= W_IBD_VActWidth_1)
				{
					W_IBD_VActWidth_1 = W_IBD_VActWidth_2;
					//W_IBD_VStart = gm_ReadRegWord(ip_ibd_vstart[B_Channel]) & 0x3FF;
				}
			}

			//stInput[B_Channel].Vstart = W_IBD_VStart;
			gm_Print(">stInput[B_Channel].Vstart = %d",stInput[B_Channel].Vstart);
		}
	}
}



#if 1//def Action_AdjustSpeakers_WB_Used
void AdjustSpeakers(void)
{
   gm_Print("UserPrefSpeakers %d",UserPrefSpeakers);
}
#endif

#if 1//1def Action_AdjustSpeakerAudioSource_WB_Used 
void AdjustSpeakerAudioSource(void)
{
   gm_Print ("UserPrefSpeakerAudioSource %d",UserPrefSpeakerAudioSource);
}
#endif

#if 1//def Action_AdjustHeadphones_WB_Used
void AdjustHeadphones(void)
{
   gm_Print("UserPrefHeadphones %d",UserPrefHeadphones);
}
#endif

#if 1//def Action_AdjustHeadphoneAudioSource_WB_Used
void AdjustHeadphoneAudioSource(void)
{
   gm_Print("UserPrefHeadphoneAudioSource %d",UserPrefHeadphoneAudioSource);
}
#endif


void ToggleFreeze(void)
{
	UserPrefFreeze ^= 1;
	Freeze(gmvb_MainChannel, UserPrefFreeze);
}

void EDID_WP_ENABLE(void)		
{
	gm_SetRegBitsWord(EDID_WP_PORT, EDID_WP_PIN);
}
void EDID_WP_DISABLE(void)
{
	gm_ClearRegBitsWord(EDID_WP_PORT, EDID_WP_PIN);
}

#define ADC_Offset1_Thold		0x10 	//((gm_ReadRegWord(ADC_A_OFFSET1)&0x3F) /4)					// quater of normal value;
#define ADC_Offset2_Thold		0x20		// quater of normal value;
#define ADC_Gain_Thold			0x30			// quater of normal value;
BOOL CheckADCCaliData(gmt_ADC_SETTING * ADCSettingTable)
{
	if((ADCSettingTable->AdcRedOffset1 < ADC_Offset1_Thold) ||(ADCSettingTable->AdcGreenOffset1< ADC_Offset1_Thold)||(ADCSettingTable->AdcBlueOffset1 < ADC_Offset1_Thold))
		return gmd_FALSE;
	if((ADCSettingTable->AdcRedOffset2 < ADC_Offset2_Thold) ||(ADCSettingTable->AdcGreenOffset2 < ADC_Offset2_Thold)||(ADCSettingTable->AdcBlueOffset2 < ADC_Offset2_Thold))
		return gmd_FALSE;
	if((ADCSettingTable->AdcRedGain < ADC_Gain_Thold) ||(ADCSettingTable->AdcGreenGain < ADC_Gain_Thold)||(ADCSettingTable->AdcBlueGain < ADC_Gain_Thold))
		return gmd_FALSE;

	return gmd_TRUE;
}

BYTE GetPrevMainAudInput(void)
{
	BYTE PrevAudInput = gmd_AUDIO_SRC_AUTO_Main;
	
	//gm_Print("GetPrevMainAudInput :UserPrefAudioMainInput = %d ",UserPrefAudioMainInput);
	switch(UserPrefAudioMainInput)
	{
		default:
		case gmd_AUDIO_SRC_AUTO_Main:
			if(Is_PIP_ON())
				PrevAudInput = gmd_AUDIO_SRC_AUTO_Pxp;
			else
				PrevAudInput = gmd_AUDIO_SRC_LINE_IN;
		break;
		case gmd_AUDIO_SRC_LINE_IN:
			PrevAudInput = gmd_AUDIO_SRC_AUTO_Main;
		break;
		case gmd_AUDIO_SRC_I2S_IN:
			PrevAudInput = gmd_AUDIO_SRC_AUTO_Main;
		break;
		case gmd_AUDIO_SRC_SPDIF_IN:
			PrevAudInput = gmd_AUDIO_SRC_AUTO_Main;
		break;

		case gmd_AUDIO_SRC_AUTO_Pxp:
			PrevAudInput = gmd_AUDIO_SRC_LINE_IN;	
	}
	return PrevAudInput;
}

BYTE GetNextMainAudInput(void)
{
	BYTE NextAudInput = gmd_AUDIO_SRC_AUTO_Main;
	
	//gm_Print("GetNextMainAudInput :UserPrefAudioMainInput = %d ",UserPrefAudioMainInput);
	switch(UserPrefAudioMainInput)
	{
		default:
		case gmd_AUDIO_SRC_AUTO_Main:
			NextAudInput = gmd_AUDIO_SRC_LINE_IN;
		break;
		case gmd_AUDIO_SRC_LINE_IN:
			//NextAudInput = gmd_AUDIO_SRC_AUTO;
			if(Is_PIP_ON())
				NextAudInput = gmd_AUDIO_SRC_AUTO_Pxp;
			else
				NextAudInput = gmd_AUDIO_SRC_AUTO_Main;
		break;
		//Not support I2S and Spdif, all router to auto
		case gmd_AUDIO_SRC_I2S_IN:
			NextAudInput = gmd_AUDIO_SRC_AUTO_Main;
		break;
		case gmd_AUDIO_SRC_SPDIF_IN:
			NextAudInput = gmd_AUDIO_SRC_AUTO_Main;
		break;
		case gmd_AUDIO_SRC_AUTO_Pxp:
			NextAudInput = gmd_AUDIO_SRC_AUTO_Main;
		break;		
	}
	//gm_Print(" NextPresetMode  = %d ",NextAudInput);
	return NextAudInput;
}

BYTE GetPrevMainAudOutput(void)
{
	BYTE PrevAudOutput = gmd_AUDIO_TAR_HeadPhone_OUT;
	
	switch(UserPrefAudioMainOutput)
	{
		default:
		case gmd_AUDIO_TAR_HeadPhone_OUT:
			#if (FEATURE_TDA7491 == ENABLE)
			PrevAudOutput = gmd_AUDIO_TAR_LINE_OUT;
			#else
			PrevAudOutput = gmd_AUDIO_TAR_HeadPhone_OUT;
			#endif
		break;

			
		case gmd_AUDIO_TAR_LINE_OUT:
			PrevAudOutput = gmd_AUDIO_TAR_HeadPhone_OUT;
		break;

		//Not support I2S and Spdif, all router to Line out
		case gmd_AUDIO_TAR_SPDIF_OUT:
			PrevAudOutput = gmd_AUDIO_TAR_HeadPhone_OUT;
		break;
		case gmd_AUDIO_TAR_I2S_OUT:
			PrevAudOutput = gmd_AUDIO_TAR_HeadPhone_OUT;
		break;
		case gmd_AUDIO_TAR_DPTx_OUT:
			PrevAudOutput = gmd_AUDIO_TAR_HeadPhone_OUT;
		break;
	}
	return PrevAudOutput;
}

BYTE GetNextMainAudOutput(void)
{
	BYTE NextAudOutput = gmd_AUDIO_TAR_HeadPhone_OUT;
	
	switch(UserPrefAudioMainOutput)
	{
		default:
		case gmd_AUDIO_TAR_HeadPhone_OUT:
			#if (FEATURE_TDA7491 == ENABLE)
			NextAudOutput = gmd_AUDIO_TAR_LINE_OUT;
			#else
			NextAudOutput = gmd_AUDIO_TAR_HeadPhone_OUT;
			#endif
		break;

		case gmd_AUDIO_TAR_LINE_OUT:
			NextAudOutput = gmd_AUDIO_TAR_HeadPhone_OUT;
		break;

		//Not support I2S and Spdif, all router to HP out
		case gmd_AUDIO_TAR_SPDIF_OUT:
			NextAudOutput = gmd_AUDIO_TAR_HeadPhone_OUT;
		break;
		case gmd_AUDIO_TAR_I2S_OUT:
			NextAudOutput = gmd_AUDIO_TAR_HeadPhone_OUT;
		break;
		case gmd_AUDIO_TAR_DPTx_OUT:
			NextAudOutput = gmd_AUDIO_TAR_HeadPhone_OUT;
		break;
	}
	//gm_Print(" NextPresetMode  = %d ",NextAudInput);
	return NextAudOutput;
}

BYTE GetPrevPxpAudInput(void)
{
	BYTE PrevAudInput = gmd_AUDIO_SRC_AUTO_Pxp;
	
	switch(UserPrefAudioPipInput)
	{
		default:
		case gmd_AUDIO_SRC_AUTO_Pxp:
			PrevAudInput = gmd_AUDIO_SRC_LINE_IN;
		break;
		case gmd_AUDIO_SRC_LINE_IN:
			PrevAudInput = gmd_AUDIO_SRC_AUTO_Pxp;
		break;
		//Not support I2S and Spdif, all router to auto
		case gmd_AUDIO_SRC_I2S_IN:
			PrevAudInput = gmd_AUDIO_SRC_AUTO_Pxp;
		break;
		case gmd_AUDIO_SRC_SPDIF_IN:
			PrevAudInput = gmd_AUDIO_SRC_AUTO_Pxp;
		break;
	}
	return PrevAudInput;
}

BYTE GetNextPxpAudInput(void)
{
	BYTE NextAudInput = gmd_AUDIO_SRC_AUTO_Pxp;
	
	switch(UserPrefAudioPipInput)
	{
		default:
		case gmd_AUDIO_SRC_AUTO_Pxp:
			NextAudInput = gmd_AUDIO_SRC_LINE_IN;
		break;
		case gmd_AUDIO_SRC_LINE_IN:
			NextAudInput = gmd_AUDIO_SRC_AUTO_Pxp;
		break;
		case gmd_AUDIO_SRC_I2S_IN:
			NextAudInput = gmd_AUDIO_SRC_AUTO_Pxp;
		break;
		case gmd_AUDIO_SRC_SPDIF_IN:
			NextAudInput = gmd_AUDIO_SRC_AUTO_Pxp;
		break;
	}
	//gm_Print(" NextPresetMode  = %d ",NextAudInput);
	return NextAudInput;
}

BYTE GetPrevPxpAudOutput(void)
{
	BYTE PrevAudOutput = gmd_AUDIO_TAR_HeadPhone_OUT;
	
	switch(UserPrefAudioPipOutput)
	{
		default:
		case gmd_AUDIO_TAR_HeadPhone_OUT:
			#if (FEATURE_TDA7491 == ENABLE)
			PrevAudOutput = gmd_AUDIO_TAR_LINE_OUT;
			#else
			PrevAudOutput = gmd_AUDIO_TAR_HeadPhone_OUT;
			#endif
		break;	
		
		case gmd_AUDIO_TAR_LINE_OUT:
			PrevAudOutput = gmd_AUDIO_TAR_HeadPhone_OUT;
		break;

		//Not support I2S and Spdif, all router to HP out
		case gmd_AUDIO_TAR_SPDIF_OUT:
			PrevAudOutput = gmd_AUDIO_TAR_HeadPhone_OUT;
		break;
		case gmd_AUDIO_TAR_I2S_OUT:
			PrevAudOutput = gmd_AUDIO_TAR_HeadPhone_OUT;
		break;
		case gmd_AUDIO_TAR_DPTx_OUT:
			PrevAudOutput = gmd_AUDIO_TAR_HeadPhone_OUT;
		break;
	}
	return PrevAudOutput;
}

BYTE GetNextPxpAudOutput(void)
{
	BYTE NextAudOutput = gmd_AUDIO_TAR_HeadPhone_OUT;
	
	switch(UserPrefAudioPipOutput)
	{
		default:
		case gmd_AUDIO_TAR_HeadPhone_OUT:
			#if (FEATURE_TDA7491 == ENABLE)
			NextAudOutput = gmd_AUDIO_TAR_LINE_OUT;
			#else
			NextAudOutput = gmd_AUDIO_TAR_LINE_OUT;
			#endif
		break;
			
		case gmd_AUDIO_TAR_LINE_OUT:
			NextAudOutput = gmd_AUDIO_TAR_HeadPhone_OUT;
		break;

		//Not support I2S and Spdif, all router to Headphone out
		case gmd_AUDIO_TAR_SPDIF_OUT:
			NextAudOutput = gmd_AUDIO_TAR_HeadPhone_OUT;
		break;
		case gmd_AUDIO_TAR_I2S_OUT:
			NextAudOutput = gmd_AUDIO_TAR_HeadPhone_OUT;
		break;
		case gmd_AUDIO_TAR_DPTx_OUT:
			NextAudOutput = gmd_AUDIO_TAR_HeadPhone_OUT;
		break;
	}
	//gm_Print(" NextPresetMode  = %d ",NextAudInput);
	return NextAudOutput;
}

gmt_UserDefConnNames GetNextPxpPort(void)
{
	BYTE B_NextPort = ALL_BOARDS_VGA1;
	//VGA==>DP1==>DVI/HDMI==No YPbPr/AV==>VGA;
	//						  ==Has YPbPr	==>YPbPr ==Has AV==>AV ==No AV==>VGA;
	//						  ==NoYPbPr Has AV==>AV; ==>No AV ==>VGA;

	switch(UserPrefCurrentInputPip)
	{
		case ALL_BOARDS_VGA1:
		#ifdef CHANNEL_CLONE
			B_NextPort = ALL_BOARDS_DP1;
		#else
			if(!IsMainDP1Input())
				B_NextPort = ALL_BOARDS_DP1;
			else
				B_NextPort = ALL_BOARDS_DVI1;
		#endif
		break;
		
		case ALL_BOARDS_DP1:
		#ifdef CHANNEL_CLONE
				B_NextPort = ALL_BOARDS_DVI1;
		#else
			#ifdef DVI_SINGLE_LINK
				if(!IsMainDVID1Input())
			#else
				if((!IsMainDVID1Input())&&(!IsMainHDMIInput()))
			#endif
					B_NextPort = ALL_BOARDS_DVI1;
				else
				{
				#ifdef DVI_SINGLE_LINK
					B_NextPort = ALL_BOARDS_HDMI;
				#else
					#if(IS_PROJ_HAS_YPbPr())		
					#if(ON_BOARD_COMP == ENABLE)
							B_NextPort = ALL_BOARDS_COMP1;
					#else
							B_NextPort = ALL_BOARDS_COMP2;
					#endif
					#elif(IS_PROJ_HAS_AV())
							B_NextPort = ALL_BOARDS_CVBS1;
					#else
							B_NextPort = ALL_BOARDS_VGA1;
					#endif
				#endif
				}
		#endif
		break;
			
		case ALL_BOARDS_DVI1:
	#ifdef CHANNEL_CLONE
			B_NextPort = ALL_BOARDS_HDMI;
	#else
		#ifdef DVI_SINGLE_LINK
			if(!IsMainHDMIInput())
		#else
			if((!IsMainDVID1Input())&&(!IsMainHDMIInput()))
		#endif
				B_NextPort = ALL_BOARDS_HDMI;
			else
			{
			#if(IS_PROJ_HAS_YPbPr())		
				#if(ON_BOARD_COMP == ENABLE)
						B_NextPort = ALL_BOARDS_COMP1;
				#else
						B_NextPort = ALL_BOARDS_COMP2;
				#endif
			#elif(IS_PROJ_HAS_AV())
					B_NextPort = ALL_BOARDS_CVBS1;
			#else
					B_NextPort = ALL_BOARDS_VGA1;
			#endif
			}
	#endif
		break;

		case ALL_BOARDS_HDMI:
	#ifdef CHANNEL_CLONE
		#if(IS_PROJ_HAS_YPbPr())		
			#if(ON_BOARD_COMP == ENABLE)
					if(!IsMainVGAInput())
						B_NextPort = ALL_BOARDS_COMP1;
					else
					{
					#if(IS_PROJ_HAS_AV())
						B_NextPort = ALL_BOARDS_CVBS1;
					#else
						B_NextPort = ALL_BOARDS_DP1;
					#endif
					}
			#else
				if(GetPortConnType(UserPrefCurrentInputMain) != DIPX)
					B_NextPort = ALL_BOARDS_COMP2;
				else
					B_NextPort = UserPrefCurrentInputMain;
			#endif
		#elif(IS_PROJ_HAS_AV())
				if(!IsMainCVBSInput())
				B_NextPort = ALL_BOARDS_CVBS1;
				else if(IsMainCVBS1Input())
					B_NextPort = ALL_BOARDS_CVBS1;
				else
					B_NextPort = ALL_BOARDS_VGA1;
		#else
				B_NextPort = ALL_BOARDS_VGA1;
		#endif
	#else
		#if(IS_PROJ_HAS_YPbPr())		
				if(!IsMainCOMPInput())
				{
				#if(ON_BOARD_COMP == ENABLE)
						B_NextPort = ALL_BOARDS_COMP1;
				#else
					if(GetPortConnType(UserPrefCurrentInputMain) != DIPX)
						B_NextPort = ALL_BOARDS_COMP2;
					else
						B_NextPort = ALL_BOARDS_VGA1;
				#endif
				}
				else if(GetPortConnType(UserPrefCurrentInputMain) != DIPX)
				{
				#if(IS_PROJ_HAS_AV())
						B_NextPort = ALL_BOARDS_CVBS1;
				#else
						B_NextPort = ALL_BOARDS_VGA1;
				#endif
				}
				else
					B_NextPort = ALL_BOARDS_VGA1;
		#elif(IS_PROJ_HAS_AV())
				if(!IsMainCVBSInput())
					B_NextPort = ALL_BOARDS_CVBS1;
				else
					B_NextPort = ALL_BOARDS_VGA1;
		#else
				B_NextPort = ALL_BOARDS_VGA1;
		#endif
	#endif
		break;

	#if(IS_PROJ_HAS_YPbPr())		
	#if(ON_BOARD_COMP == ENABLE)
		case ALL_BOARDS_COMP1:
	#else
		case ALL_BOARDS_COMP2:
	#endif
			
		#ifdef CHANNEL_CLONE
			#if(IS_PROJ_HAS_AV())
					B_NextPort = ALL_BOARDS_CVBS1;
			#else
					B_NextPort = ALL_BOARDS_VGA1;
			#endif
		#else
			#if(IS_PROJ_HAS_AV())
				{
					if(!IsMainCVBSInput())
						B_NextPort = ALL_BOARDS_CVBS1;
					else
						B_NextPort = ALL_BOARDS_VGA1;
				}
			#else
					if(!IsMainVGAInput())
						B_NextPort = ALL_BOARDS_VGA1;
					else
						B_NextPort = ALL_BOARDS_DP1;
			#endif
		#endif
		break;
	#endif	//(IS_PROJ_HAS_YPbPr())
#ifdef MUX_TEST_BOARD
		case ALL_BOARDS_DP3:
			if(!IsMainVGAInput())
				B_NextPort = ALL_BOARDS_VGA1;
			else
				B_NextPort = ALL_BOARDS_HDMI;
		break;
#endif
		
#if(IS_PROJ_HAS_AV())
		case ALL_BOARDS_CVBS1:
			if(GetPortConnType(UserPrefCurrentInputMain) != DIPX)
				B_NextPort = ALL_BOARDS_CVBS2;
			else
				B_NextPort = ALL_BOARDS_VGA1;				
			/*
		#ifdef CHANNEL_CLONE
				B_NextPort = ALL_BOARDS_CVBS2;
		#else
				if(!IsMainVGAInput())
					B_NextPort = ALL_BOARDS_VGA1;
				else
					B_NextPort = ALL_BOARDS_DP1;
		#endif
			*/
		break;
#endif
	
#if(IS_PROJ_HAS_AV())
		case ALL_BOARDS_CVBS2:
	#ifdef CHANNEL_CLONE
			B_NextPort = ALL_BOARDS_VGA1;
	#else
			if(!IsMainVGAInput())
				B_NextPort = ALL_BOARDS_VGA1;
			else
				B_NextPort = ALL_BOARDS_DP1;
	#endif
		break;
#endif
		default:
		break;
	}
	return (B_NextPort);

}


gmt_UserDefConnNames GetPrevPxpPort(void)
{
	BYTE B_PrevPort = ALL_BOARDS_VGA1;
	//VGA==>DP1==>DVI/HDMI==No YPbPr/AV==>VGA;
	//						  ==Has YPbPr	==>YPbPr ==Has AV==>AV ==No AV==>VGA;
	//						  ==NoYPbPr Has AV==>AV; ==>No AV ==>VGA;

	switch(UserPrefCurrentInputPip)
	{
		case ALL_BOARDS_VGA1:
	#ifdef CHANNEL_CLONE
		#if(IS_PROJ_HAS_AV())	
				if(GetPortConnType(UserPrefCurrentInputMain) != DIPX)
					B_PrevPort = ALL_BOARDS_CVBS2;
				else
					B_PrevPort = UserPrefCurrentInputMain;
		#elif(IS_PROJ_HAS_YPbPr())
			#if(ON_BOARD_COMP == ENABLE)
					if(GetPortConnType(UserPrefCurrentInputMain) != VGA)
						B_PrevPort = ALL_BOARDS_COMP1;
					else
						B_PrevPort = ALL_BOARDS_HDMI;
			#else
					if(GetPortConnType(UserPrefCurrentInputMain) != DIPX)
						B_PrevPort = ALL_BOARDS_COMP2;
					else
						B_PrevPort = UserPrefCurrentInputMain;
			#endif
		#else
				B_PrevPort = ALL_BOARDS_HDMI;
		#endif
	#else	//CHANNEL_CLONE
		#if(IS_PROJ_HAS_AV())	
				if(GetPortConnType(UserPrefCurrentInputMain) != DIPX)
					B_PrevPort = ALL_BOARDS_CVBS2;
				else
				{
				#if(IS_PROJ_HAS_YPbPr())
					#if(ON_BOARD_COMP == ENABLE)
							B_PrevPort = ALL_BOARDS_COMP1;
					#else
							B_PrevPort = ALL_BOARDS_HDMI;
					#endif
				#else
					B_PrevPort = ALL_BOARDS_HDMI;
				#endif
				}
		#elif(IS_PROJ_HAS_YPbPr())
				if(!IsMainCOMPInput())
				{
				#if(ON_BOARD_COMP == ENABLE)
						B_PrevPort = ALL_BOARDS_COMP1;
				#else
					if(GetPortConnType(UserPrefCurrentInputMain) != DIPX)
						B_PrevPort = ALL_BOARDS_COMP2;
					else
						B_PrevPort = ALL_BOARDS_HDMI;
				#endif
				}
				else
					B_PrevPort = ALL_BOARDS_HDMI;
		#else
			#ifdef DVI_SINGLE_LINK
				if(!IsMainHDMIInput())
					B_PrevPort = ALL_BOARDS_HDMI;
				else
			#else
				if((!IsMainDVID1Input())&&(!IsMainHDMIInput()))
					B_PrevPort = ALL_BOARDS_HDMI;
				else
			#endif
				{
				#ifdef DVI_SINGLE_LINK
						B_PrevPort = ALL_BOARDS_DVI1;
				#else
						B_PrevPort = ALL_BOARDS_DP1;
				#endif
				}
		#endif
	#endif
		break;
		
		case ALL_BOARDS_DP1:
	#ifdef CHANNEL_CLONE
				B_PrevPort = ALL_BOARDS_VGA1;
	#else
				if(!IsMainVGAInput())
					B_PrevPort = ALL_BOARDS_VGA1;
				else
				{
			#if(IS_PROJ_HAS_AV())		
					B_PrevPort = ALL_BOARDS_CVBS2;
			#elif(IS_PROJ_HAS_YPbPr())
				#if(ON_BOARD_COMP == ENABLE)
						B_PrevPort = ALL_BOARDS_COMP1;
				#else
						B_PrevPort = ALL_BOARDS_COMP2;
				#endif
			#else
					B_PrevPort = ALL_BOARDS_HDMI;
			#endif
				}
	#endif
		break;
			
		case ALL_BOARDS_DVI1:
#ifdef CHANNEL_CLONE
		B_PrevPort = ALL_BOARDS_DP1;
#else
		if(!IsMainDP1Input())
			B_PrevPort = ALL_BOARDS_DP1;
		else
			B_PrevPort = ALL_BOARDS_VGA1;
#endif
		break;

		case ALL_BOARDS_HDMI:
		#ifdef CHANNEL_CLONE
			#ifdef DVI_SINGLE_LINK
				B_PrevPort = ALL_BOARDS_DVI1;
			#else
				B_PrevPort = ALL_BOARDS_DP1;
			#endif
		#else
			#ifdef DVI_SINGLE_LINK
				if(!IsMainDVID1Input())
					B_PrevPort = ALL_BOARDS_DVI1;
				else
					B_PrevPort = ALL_BOARDS_DP1;
			#else
					B_PrevPort = ALL_BOARDS_DVI1;
			#endif
		#endif
		break;

#if(IS_PROJ_HAS_YPbPr())		
#if(ON_BOARD_COMP == ENABLE)
		case ALL_BOARDS_COMP1:
#else
		case ALL_BOARDS_COMP2:
#endif
			
	#ifdef CHANNEL_CLONE
			B_PrevPort = ALL_BOARDS_HDMI;
	#else
		#ifdef DVI_SINGLE_LINK
			if(!IsMainHDMIInput())
		#else
			if((!IsMainDVID1Input())&&(!IsMainHDMIInput()))
		#endif
				B_PrevPort = ALL_BOARDS_HDMI;
			else
			{
			#ifdef DVI_SINGLE_LINK
				B_PrevPort = ALL_BOARDS_DVI1;
			#else
				B_PrevPort = ALL_BOARDS_DP1;
			#endif
			}
	#endif
		break;
#endif	//(IS_PROJ_HAS_YPbPr())
	
#ifdef MUX_TEST_BOARD
		case ALL_BOARDS_DP3:
			if(!IsMainVGAInput())
				B_PrevPort = ALL_BOARDS_VGA1;
			else
				B_PrevPort = ALL_BOARDS_HDMI;
		break;
#endif
		
#if(IS_PROJ_HAS_AV())
		case ALL_BOARDS_CVBS1:
	#ifdef CHANNEL_CLONE
		#if(IS_PROJ_HAS_YPbPr())
			#if(ON_BOARD_COMP == ENABLE)
				if(GetPortConnType(UserPrefCurrentInputMain) != VGA)
					B_PrevPort = ALL_BOARDS_COMP1;
				else
					B_PrevPort = ALL_BOARDS_HDMI;
			#else
				if(GetPortConnType(UserPrefCurrentInputMain) != DIPX)
					B_PrevPort = ALL_BOARDS_COMP2;
				else
					B_PrevPort = ALL_BOARDS_HDMI;					
			#endif
		#else
				B_PrevPort = ALL_BOARDS_HDMI;
		#endif
	#else
		#if(IS_PROJ_HAS_YPbPr())
				if(!IsMainCOMPInput())
				{
				#if(ON_BOARD_COMP == ENABLE)
						B_PrevPort = ALL_BOARDS_COMP1;
				#else
						B_PrevPort = ALL_BOARDS_COMP2;
				#endif
				}
				else
					B_PrevPort = ALL_BOARDS_HDMI;
		#else
			#ifdef DVI_SINGLE_LINK
				if(!IsMainHDMIInput())
			#else
				if((!IsMainDVID1Input())&&(!IsMainHDMIInput()))
			#endif
					B_PrevPort = ALL_BOARDS_HDMI;
				else
				{
				#ifdef DVI_SINGLE_LINK
					B_PrevPort = ALL_BOARDS_DVI1;
				#else
					B_PrevPort = ALL_BOARDS_DP1;
				#endif
				}
		#endif

	#endif
		break;
#endif
	
#if(IS_PROJ_HAS_AV())
		case ALL_BOARDS_CVBS2:
		#ifdef CHANNEL_CLONE
				B_PrevPort = ALL_BOARDS_CVBS1;
		#else	//CHANNEL_CLONE
				B_PrevPort = ALL_BOARDS_CVBS1;
				/*
			#if(IS_PROJ_HAS_YPbPr())
					if(!IsMainCOMPInput())
					{
					#if(ON_BOARD_COMP == ENABLE)
							B_PrevPort = ALL_BOARDS_COMP1;
					#else
							B_PrevPort = ALL_BOARDS_COMP2;
					#endif
					}
					else
						B_PrevPort = ALL_BOARDS_HDMI;
						
			#else
					if(!IsMainHDMIInput())
						B_PrevPort = ALL_BOARDS_HDMI;
					else
						B_PrevPort = ALL_BOARDS_DP1;
			#endif
				*/
		#endif
		break;
#endif
	
		default:
		break;
	}
	return (B_PrevPort);

}




gmt_UserDefConnNames GetNextMainPort_4K2K(void)
{
	BYTE B_NextPort = ALL_BOARDS_VGA1;
	//VGA==>DP1==>DVI/HDMI==No YPbPr/AV==>VGA;
	//					      ==Has YPbPr	==>YPbPr ==Has AV==>AV ==No AV==>VGA;
	//					      ==NoYPbPr Has AV==>AV; ==>No AV ==>VGA;

	switch(UserPrefCurrentInputMain)
	{
		case ALL_BOARDS_VGA1:
			#ifdef CHANNEL_CLONE
			B_NextPort = ALL_BOARDS_DP1;
			#else
				B_NextPort = ALL_BOARDS_DP1;
			#endif
		break;
		case ALL_BOARDS_DP1:
			#ifdef CHANNEL_CLONE
				B_NextPort = ALL_BOARDS_DVI1;
			#else
				B_NextPort = ALL_BOARDS_DVI1;
			#endif
		break;
			
		case ALL_BOARDS_DVI1:
		#ifdef CHANNEL_CLONE
			B_NextPort = ALL_BOARDS_HDMI;
		#else
				B_NextPort = ALL_BOARDS_HDMI;
		#endif
		break;

		case ALL_BOARDS_HDMI:
		#ifdef CHANNEL_CLONE
			#if(IS_PROJ_HAS_YPbPr())		
				#if(ON_BOARD_COMP == ENABLE)
					B_NextPort = ALL_BOARDS_COMP1;
				#else
					B_NextPort = ALL_BOARDS_COMP2;
				#endif
			#elif(IS_PROJ_HAS_AV())
				if(!IsMainCVBSInput())
				B_NextPort = ALL_BOARDS_CVBS1;
				else if(IsMainCVBS1Input())
					B_NextPort = ALL_BOARDS_CVBS1;
				else
					B_NextPort = ALL_BOARDS_VGA1;
			#else
				B_NextPort = ALL_BOARDS_VGA1;
			#endif
		#else
			#if(IS_PROJ_HAS_YPbPr())		
				#if(ON_BOARD_COMP == ENABLE)
					B_NextPort = ALL_BOARDS_COMP1;
				#else
					B_NextPort = ALL_BOARDS_COMP2;
				#endif
			#elif(IS_PROJ_HAS_AV())
					B_NextPort = ALL_BOARDS_CVBS1;
			#else
				B_NextPort = ALL_BOARDS_VGA1;
			#endif
		#endif
		break;

		#if(IS_PROJ_HAS_YPbPr())		
		#if(ON_BOARD_COMP == ENABLE)
		case ALL_BOARDS_COMP1:
		#else
		case ALL_BOARDS_COMP2:
		#endif
			
			#ifdef CHANNEL_CLONE
				#if(IS_PROJ_HAS_AV())
					B_NextPort = ALL_BOARDS_CVBS1;
				#else
					B_NextPort = ALL_BOARDS_VGA1;
				#endif
			#else
				#if(IS_PROJ_HAS_AV())
					B_NextPort = ALL_BOARDS_CVBS1;
				#else
					B_NextPort = ALL_BOARDS_VGA1;
				#endif
			#endif
		break;
		#endif	//(IS_PROJ_HAS_YPbPr())
	#ifdef MUX_TEST_BOARD
		case ALL_BOARDS_DP3:
				B_NextPort = ALL_BOARDS_HDMI;
		break;
	#endif
		
	#if(IS_PROJ_HAS_AV())
		case ALL_BOARDS_CVBS1:
			B_NextPort = ALL_BOARDS_CVBS2;
			/*
			#ifdef CHANNEL_CLONE
				B_NextPort = ALL_BOARDS_CVBS2;
			#else
				if(!IsMainVGAInput())
					B_NextPort = ALL_BOARDS_VGA1;
				else
					B_NextPort = ALL_BOARDS_DP1;
			#endif
			*/
		break;
	#endif
	
	#if(IS_PROJ_HAS_AV())
		case ALL_BOARDS_CVBS2:
		#ifdef CHANNEL_CLONE
			B_NextPort = ALL_BOARDS_VGA1;
		#else
			B_NextPort = ALL_BOARDS_VGA1;
		#endif
		break;
	#endif
		default:
		break;
	}

	gm_Print("GetNextMainPort_4K2K:  UserPrefCurrentInputMain = %d",UserPrefCurrentInputMain);
	gm_Print("GetNextMainPort_4K2K: B_NextPort= %d",B_NextPort);
	return (B_NextPort);

}


gmt_UserDefConnNames GetPrevMainPort_4K2K(void)
{
	BYTE B_PrevPort = ALL_BOARDS_VGA1;
	//VGA==>DP1==>DVI/HDMI==No YPbPr/AV==>VGA;
	//						  ==Has YPbPr	==>YPbPr ==Has AV==>AV ==No AV==>VGA;
	//						  ==NoYPbPr Has AV==>AV; ==>No AV ==>VGA;

	switch(UserPrefCurrentInputMain)
	{
		case ALL_BOARDS_VGA1:
		#ifdef CHANNEL_CLONE
			#if(IS_PROJ_HAS_AV())	
				B_PrevPort = ALL_BOARDS_CVBS2;
			#elif(IS_PROJ_HAS_YPbPr())
				#if(ON_BOARD_COMP == ENABLE)
					B_PrevPort = ALL_BOARDS_COMP1;
				#else
					B_PrevPort = ALL_BOARDS_COMP2;
				#endif
			#else
				B_PrevPort = ALL_BOARDS_HDMI;
			#endif
		#else	//CHANNEL_CLONE
			#if(IS_PROJ_HAS_AV())	
				if(!IsMainCVBSInput())
					B_PrevPort = ALL_BOARDS_CVBS2;
				else
				{
					#if(IS_PROJ_HAS_YPbPr())
						#if(ON_BOARD_COMP == ENABLE)
							B_PrevPort = ALL_BOARDS_COMP1;
						#else
							B_PrevPort = ALL_BOARDS_COMP2;
						#endif
					#else
					B_PrevPort = ALL_BOARDS_HDMI;
					#endif
				}
			#elif(IS_PROJ_HAS_YPbPr())
				if(!IsMainCOMPInput())
				{
					#if(ON_BOARD_COMP == ENABLE)
						B_PrevPort = ALL_BOARDS_COMP1;
					#else
						B_PrevPort = ALL_BOARDS_COMP2;
					#endif
				}
				else
					B_PrevPort = ALL_BOARDS_HDMI;
			#else
				if(!IsMainHDMIInput())
					B_PrevPort = ALL_BOARDS_HDMI;
				else
					B_PrevPort = ALL_BOARDS_DP1;
			#endif
		#endif
		break;
		
		case ALL_BOARDS_DP1:
		#ifdef CHANNEL_CLONE
				B_PrevPort = ALL_BOARDS_VGA1;
		#else
				if(!IsMainVGAInput())
					B_PrevPort = ALL_BOARDS_VGA1;
				else
				{
				#if(IS_PROJ_HAS_AV())		
					B_PrevPort = ALL_BOARDS_CVBS2;
				#elif(IS_PROJ_HAS_YPbPr())
					#if(ON_BOARD_COMP == ENABLE)
						B_PrevPort = ALL_BOARDS_COMP1;
					#else
						B_PrevPort = ALL_BOARDS_COMP2;
					#endif
				#else
					B_PrevPort = ALL_BOARDS_HDMI;
				#endif
				}
		#endif
		break;
			
		case ALL_BOARDS_DVI1:
	#ifdef CHANNEL_CLONE
		B_PrevPort = ALL_BOARDS_DP1;
	#else
		if(!IsMainDP1Input())
			B_PrevPort = ALL_BOARDS_DP1;
		else
			B_PrevPort = ALL_BOARDS_VGA1;
	#endif
		break;

		case ALL_BOARDS_HDMI:
			#ifdef CHANNEL_CLONE
				B_PrevPort = ALL_BOARDS_DVI1;
			#else
				if(!IsMainDVID1Input())
					B_PrevPort = ALL_BOARDS_DVI1;
				else
					B_PrevPort = ALL_BOARDS_DP1;
			#endif
		break;

	#if(IS_PROJ_HAS_YPbPr())		
	#if(ON_BOARD_COMP == ENABLE)
		case ALL_BOARDS_COMP1:
	#else
		case ALL_BOARDS_COMP2:
	#endif
			
		#ifdef CHANNEL_CLONE
			B_PrevPort = ALL_BOARDS_HDMI;
		#else
			if(!IsMainHDMIInput())
				B_PrevPort = ALL_BOARDS_HDMI;
			else
				B_PrevPort = ALL_BOARDS_DP1;
		#endif
		break;
	#endif	//(IS_PROJ_HAS_YPbPr())
	
#ifdef MUX_TEST_BOARD
		case ALL_BOARDS_DP3:
			if(!IsMainVGAInput())
				B_PrevPort = ALL_BOARDS_VGA1;
			else
				B_PrevPort = ALL_BOARDS_HDMI;
		break;
#endif
		
	#if(IS_PROJ_HAS_AV())
		case ALL_BOARDS_CVBS1:
		#ifdef CHANNEL_CLONE
			#if(IS_PROJ_HAS_YPbPr())
				#if(ON_BOARD_COMP == ENABLE)
					B_PrevPort = ALL_BOARDS_COMP1;
				#else
					B_PrevPort = ALL_BOARDS_COMP2;
				#endif
			#else
				B_PrevPort = ALL_BOARDS_HDMI;
			#endif
		#else
			#if(IS_PROJ_HAS_YPbPr())
				if(!IsMainCOMPInput())
				{
					#if(ON_BOARD_COMP == ENABLE)
						B_PrevPort = ALL_BOARDS_COMP1;
					#else
						B_PrevPort = ALL_BOARDS_COMP2;
					#endif
				}
				else
					B_PrevPort = ALL_BOARDS_HDMI;
			#else
				if(!IsMainHDMIInput())
					B_PrevPort = ALL_BOARDS_HDMI;
				else
					B_PrevPort = ALL_BOARDS_DP1;
			#endif

		#endif
		break;
	#endif
	
	#if(IS_PROJ_HAS_AV())
		case ALL_BOARDS_CVBS2:
			#ifdef CHANNEL_CLONE
				B_PrevPort = ALL_BOARDS_CVBS1;
			#else	//CHANNEL_CLONE
				B_PrevPort = ALL_BOARDS_CVBS1;
				/*
				#if(IS_PROJ_HAS_YPbPr())
					if(!IsMainCOMPInput())
					{
						#if(ON_BOARD_COMP == ENABLE)
							B_PrevPort = ALL_BOARDS_COMP1;
						#else
							B_PrevPort = ALL_BOARDS_COMP2;
						#endif
					}
					else
						B_PrevPort = ALL_BOARDS_HDMI;
						
				#else
					if(!IsMainHDMIInput())
						B_PrevPort = ALL_BOARDS_HDMI;
					else
						B_PrevPort = ALL_BOARDS_DP1;
				#endif
				*/
			#endif
		break;
	#endif
	
		default:
		break;
	}
	return (B_PrevPort);

}

void SetupDPPanel(void)
{
#if(SELECT_DPTX_TYPE==USE_IDP_DPTX_OUTPUT)
	DP12_ActivateSstTxPort(iDpTxPort);
	DPTxPortSwitch (MstTxPort, iDpTxPort);
	gm_ClearRegBitsByte(HOST_DVI_DP_SETUP,DPTX_PHY_SRC_SELECT);
#endif
}

void UARTInit(void)
{
	#ifdef UART_DEBUG_MODE
		#ifdef USE_SECOND_UART
		gm_UartInit(UART1, gmc_OCLK_FREQ, 115200UL, &uartData);
		gm_SetPrintPort(SERIAL_USE_UART1);		
		#else
		gm_UartInit(UART0, gmc_OCLK_FREQ, 115200UL, &uartData);
		gm_SetPrintPort(SERIAL_USE_UART0);	
		#endif
	#else
		DWORD DW_BaudRate = BAUDRATE_SELECT;
	
		DW_BaudRate = GetUartBaudRate();
	
		#if(defined(BOARD_93XX_PD)||defined(BOARD_93XX_SUV)||defined(BOARD_93XX_OV))
			#if(OSD_UART_DEF == UART1)
			//gm_UartInit(UART1, gmc_OCLK_FREQ, BAUDRATE_SELECT, &uartData);
			gm_UartInit(UART1, gmc_OCLK_FREQ, DW_BaudRate, &uartData);
			gm_SetPrintPort(SERIAL_USE_UART1);			
			gm_ClearRegBitsWord(GPIO_4_ENABLE, GPIO79_EN);
			gm_ClearRegBitsWord(GPIO_5_ENABLE, GPIO80_EN);
			#else
			//gm_UartInit(UART0, gmc_OCLK_FREQ, BAUDRATE_SELECT, &uartData);
			gm_UartInit(UART0, gmc_OCLK_FREQ, DW_BaudRate, &uartData);
			gm_SetPrintPort(SERIAL_USE_UART0);		
			#endif
		#else
			//gm_UartInit(UART0, gmc_OCLK_FREQ, BAUDRATE_SELECT, &uartData);
			gm_UartInit(UART0, gmc_OCLK_FREQ, DW_BaudRate, &uartData);
			gm_SetPrintPort(SERIAL_USE_UART0);		  
		#endif
	#endif
}

void AdjustUartSelect(void)
{
	DWORD DW_BaudRate;

	
	DW_BaudRate = GetUartBaudRate();

	
	if(UserPrefUartSelect == UART0)
	{
		//gm_UartInit(UART0, gmc_OCLK_FREQ, 9600L, &uartData);
		gm_UartInit(UART0, gmc_OCLK_FREQ, DW_BaudRate, &uartData);
		gm_SetPrintPort(UserPrefUartSelect);		  
		gm_Print("UserPrefUartSelect =UART0 ", 0);
	}
	else
	{

    		gm_UartInit(UART1, gmc_OCLK_FREQ, DW_BaudRate, &uartData);
		gm_SetPrintPort(UserPrefUartSelect);		
		{
			gm_ClearRegBitsWord(GPIO_4_ENABLE, GPIO79_EN);
			gm_ClearRegBitsWord(GPIO_5_ENABLE, GPIO80_EN);
		}
		gm_Print("UserPrefUartSelect =UART1 ", 0);
	}
}
    


void DCR_BACKLIGHT_ON(void)
{
	#if(DEFAULT_PANEL == LG_WHD_LM290WW1_SSA1)
	gm_SetRegBitsWord(DCR_BACKLIGHT_OUTPUT, DCR_BACKLIGHT_OUT);

	//#elif(DEFAULT_PANEL == LG_WQXGA_LM300WQ5_SLA1)
	//gm_SetRegBitsWord(DCR_BACKLIGHT_OUTPUT, DCR_BACKLIGHT_OUT);

	#endif


}
void DCR_BACKLIGHT_OFF(void)           	
{
	#if(DEFAULT_PANEL == LG_WHD_LM290WW1_SSA1)
	gm_ClearRegBitsWord(DCR_BACKLIGHT_OUTPUT, DCR_BACKLIGHT_OUT);

	//#elif(DEFAULT_PANEL == LG_WQXGA_LM300WQ5_SLA1)
	//gm_ClearRegBitsWord(DCR_BACKLIGHT_OUTPUT, DCR_BACKLIGHT_OUT);

	#endif


}

void BacklightInit(void)
{
	#if(DEFAULT_PANEL == LPL_WUXGA_LM240WU5)
		BYTE Bp_Buffer[2];
	
		Bp_Buffer[0] = 0xC4;
		Bp_Buffer[1] = MAX_BACKLIGHT_PWM;	// Set to max for Logo showing;
		gm_Write2WireBlock(0, 0x6A, (BYTE*)Bp_Buffer, 2, TRUE);
		gm_Delay1ms(1);
	#else

		gm_WriteRegWord(BACKLIGHT_DCR_PWM_CONFIG, BACKLIGHT_PWM_CONFIG); //20120602 Modifiy to 8.5kHz by TigerSong for DCR_PWM. brightness < 30.
		#if(DEFAULT_PANEL == SHARP_1080P_LD900D3LE1)
		gm_WriteRegWord(BACKLIGHT_B_PWM_CONFIG, DIM_PWM_CONFIG); 
		#else
		gm_WriteRegWord(BACKLIGHT_B_PWM_CONFIG, BACKLIGHT_PWM_CONFIG); //20120602 Modifiy to 10kHz by TigerSong for G/B backlight. brightness >=30.
		#endif
		gm_WriteRegWord(BACKLIGHT_G_PWM_CONFIG, BACKLIGHT_PWM_CONFIG);
		
		#if(DEFAULT_PANEL == SHARP_1080P_LD900D3LE1)
		gm_WriteRegWord(BACKLIGHT_B_PWM_CTRL_MSB, ((MAX_B_BACKLIGHT_PWM&PWM0_PULSE_MSB))|((MAX_B_PWMPERIOD>>8)&PWM0_PERIOD_MS)); // B_led control
		gm_WriteRegWord(BACKLIGHT_B_PWM_CTRL, ((WORD)(MAX_B_PWMPERIOD&PWM0_PERIOD) |((MAX_B_BACKLIGHT_PWM&0xFF)<<8)));
		#endif
		
	#endif
}


void AdjustBacklightForLogo(void)
{
	WORD MAX_PULSE;

	MAX_PULSE = (MAX_BACKLIGHT_PWM&0xFF);
	#if BACKLIGHT_PWM_INVERT
		MAX_PULSE = MAX_PWMPERIOD - MAX_PULSE;
	#endif
	
	gm_WriteRegWord(BACKLIGHT_DCR_PWM_CTRL_MSB, ((MAX_BACKLIGHT_PWM&PWM0_PULSE_MSB))|((MAX_PWMPERIOD>>8)&PWM0_PERIOD_MS)); // DCR control
	gm_WriteRegWord(BACKLIGHT_DCR_PWM_CTRL, ((WORD)(MAX_PWMPERIOD&PWM0_PERIOD) |(MAX_PULSE<<8)));
	
	#if(DEFAULT_PANEL != SHARP_1080P_LD900D3LE1)
	gm_WriteRegWord(BACKLIGHT_B_PWM_CTRL_MSB, ((MAX_BACKLIGHT_PWM&PWM0_PULSE_MSB))|((MAX_PWMPERIOD>>8)&PWM0_PERIOD_MS)); // B_led control
	gm_WriteRegWord(BACKLIGHT_B_PWM_CTRL, ((WORD)(MAX_PWMPERIOD&PWM0_PERIOD) |(MAX_PULSE<<8)));
	#endif
	gm_WriteRegWord(BACKLIGHT_G_PWM_CTRL_MSB, ((MAX_BACKLIGHT_PWM&PWM0_PULSE_MSB))|((MAX_PWMPERIOD>>8)&PWM0_PERIOD_MS)); // G_led control
	gm_WriteRegWord(BACKLIGHT_G_PWM_CTRL, ((WORD)(MAX_PWMPERIOD&PWM0_PERIOD) |(MAX_PULSE<<8)));

}

BOOL ReCheckInput(gmt_PHY_CH B_Channel)
{
	static BYTE ReCheckCNT = 0;
	//gm_Print("=======ReCheckInput=======>>>>>>>>>>>>>>>>> ",0);
	if(((abs(gm_ReadRegWord(ip_ibd_vstart[B_Channel]) - stInput[B_Channel].Vstart)>3)
		||(abs(gm_ReadRegWord(ip_ibd_vlength[B_Channel]) - stInput[B_Channel].Vactive) > 3)
	)&&gm_IsInputHdmiDvi(B_Channel)/*&&(!gm_IsInterlaced(B_Channel))*/&&ReCheckCNT < 3)
	{
		gmt_IbdStruct InIBD;
		gm_IbdRead(B_Channel,&InIBD);
	
		ReCheckCNT++;
		#ifdef UART_DEBUG_MODE
		gm_Print("=======ReCheckInput=======>>>>>>>>>>>>>>>>> : InIBD.VStart 0x%x",InIBD.VStart);
		gm_Print("=======ReCheckInput=======>>>>>>>>>>>>>>>>> : InIBD.VActive 0x%x",InIBD.VActive);
		#endif
		stInput[B_Channel].Htotal = InIBD.HTotal;
		stInput[B_Channel].Hstart = InIBD.HStart;
		stInput[B_Channel].Hactive = InIBD.HActive;
		stInput[B_Channel].Vtotal = InIBD.VTotal;
		stInput[B_Channel].Vstart = InIBD.VStart;
		stInput[B_Channel].Vactive = InIBD.VActive;
		//if(stInput[B_Channel].Vactive < 480)
		
		//SaveOriginalValue(B_Channel);
		#if(IS_PROJ_PD())
		//SavePDOriginalValue(B_Channel);
		#endif
		if(B_Channel == CH_A)
		   B_SignalStatus = STATUS_NO_SYNC;
		ModeSetupRequestCancel(B_Channel, REQ_MS_SETUP, PARAM_NONE);
		//ModeSetupRequest(B_Channel, REQ_MS_RESTART, PARAM_NONE, 0);
		ModeSetupRequestCancel(B_Channel, REQ_MS_BLENDER, PARAM_ANY);
		ModeSetupRequestCancel(B_Channel, REQ_DEBUG, PARAM_NONE);

		gm_ForceModeChange(B_Channel);
		return gmd_FALSE;

	}
	ReCheckCNT = 0;
	return gmd_TRUE;
}

void AdjustOutputMode(void){}

void ModeSetupDisplayScaling(gmt_PHY_CH B_Channel, BYTE B_Step)
{

   Freeze(B_Channel, TRUE);

   {
      //
      // Setup Dynamic scaling size Start
      //
      SetDynamicScalerSizeBegin(B_Channel);

      // Setup Display
      if (B_Channel == gmvb_MainChannel)
      {
         SetDynamicScalerStep(gmvb_MainChannel, B_Step);		//Neil120725* org = 4
         SetDynamicScalerStep(gmvb_PipChannel, 0);   // turn off pip scaling
         //SetDisplayMain();
      }
      else
      {
         SetDynamicScalerStep(gmvb_MainChannel, 0);   // turn off Main scaling
         SetDynamicScalerStep(gmvb_PipChannel, B_Step);
         //SetDisplayPip();
      }
      if (B_Channel == CH_A)
         gm_AfmSetup(W_OverscanEnabled[CH_A]);
      //
      // Setup Dynamic scaling size end
      //
      SetDynamicScalerSizeEnd(B_Channel);

      //
      // Program Normal
      //
	  /*
      if ((gm_IsValidSignal(B_Channel) == FALSE)
            || (B_DisableDynamicScale == TRUE)
#if (FEATURE_TWO_STAGE_PROCESSING_MODE == ENABLE)
            || gm_Is2StageEnabled()
#endif
            || (B_Method == DISPLAY_CHANGE_QUICK))
            */
      {
#ifndef NO_PIP_SUPPORT      
         //if(B_Channel == gmvb_PipChannel) //KevinL 20090318 Border for PIP only
         //SetPipBorder(B_Channel, TRUE);
#endif
         //SetDisplayOff(B_Channel, BACKGROUND_ONLY_NO_SYNC_UPDATE);
         gm_ScalerDynamicEnable(B_Channel, FALSE);
         gm_DisplaySetSize(B_Channel);
         gm_DisplaySetPosition(B_Channel);

/*
#if (FEATURE_TWO_STAGE_PROCESSING_MODE == ENABLE)
         if ((UserPrefPipMode == NO_PIP) && (B_Channel == CH_A)
               && IsTwoStageAllowed())
         {
            if (!gm_Is2StageEnabled())
            {
               EnterTwoStageProcessing();
            }
         }
         else
#endif
*/
         {
            if (gm_IsValidSignal(B_Channel) == FALSE)
            {
               //If output window size is zero (this should not happen but anways), set to
               //  some arbitrary number to avoid memory and output size to be set to zero
               if (gmvw_OutputHActive[B_Channel] == 0)
                  gmvw_OutputHActive[B_Channel] = 50;
               if (gmvw_OutputVActive[B_Channel] == 0)
                  gmvw_OutputVActive[B_Channel] = 50;

               stInput[B_Channel].Hactive = gmvw_OutputHActive[B_Channel];
               stInput[B_Channel].Vactive = gmvw_OutputVActive[B_Channel];
               gm_ScalerBypass(B_Channel);
            }
            else
            {
               gm_ScalerSetup(B_Channel);
            }

            gm_MemorySizeAdjust(B_Channel);
            gm_SyncUpdateWait(B_Channel, INPUT_OUTPUT_CLOCK);
         }

      }
	  /*
      //
      // Program Dynamic Scaling
      //
      else
      {
         DynamicScalerStart(B_Channel);
         DynamicScalerProcess(FALSE);
         DynamicScalerStop();
      }
      */
   }
   gm_SyncUpdateWait(gmvb_PipChannel, OUTPUT_CLOCK);

   Freeze(B_Channel, FALSE);

   // Request Blenders ON and User Prefs
   //ModeSetupBlender(B_Channel, DISPLAY_DEFER_TMO_NORMAL);
}

void AdjustMainPosition(void)
{
	//AdjustMainMode();
	switch(UserPrefMainPosition)
	{
		case BOTTOM_RIGHT:
			gmvw_OutputHStart[gmvb_MainChannel] = PanelWidth - gmvw_OutputHActive[gmvb_MainChannel] ;//- PIP_WINDOW_HORIZONTAL_OFFSET;
			gmvw_OutputVStart[gmvb_MainChannel] = PanelHeight - gmvw_OutputVActive[gmvb_MainChannel] ;//- PIP_WINDOW_VERTICAL_OFFSET;
			break;
		case TOP_RIGHT:
			gmvw_OutputHStart[gmvb_MainChannel] = PanelWidth - gmvw_OutputHActive[gmvb_MainChannel];// - PIP_WINDOW_HORIZONTAL_OFFSET;
			gmvw_OutputVStart[gmvb_MainChannel] = 0;//PIP_WINDOW_VERTICAL_OFFSET;
			break;
		case TOP_LEFT:
			gmvw_OutputHStart[gmvb_MainChannel] = 0;//PIP_WINDOW_HORIZONTAL_OFFSET;
			gmvw_OutputVStart[gmvb_MainChannel] = 0;//PIP_WINDOW_VERTICAL_OFFSET;
			break;
		case BOTTOM_LEFT:
			gmvw_OutputHStart[gmvb_MainChannel] = 0;//PIP_WINDOW_HORIZONTAL_OFFSET;
			gmvw_OutputVStart[gmvb_MainChannel] = PanelHeight - gmvw_OutputVActive[gmvb_MainChannel];//- PIP_WINDOW_VERTICAL_OFFSET;
			break;
		default:
			break;
	}
}

void AdjustMainMode(void)
{
		gmvw_OutputHActive[gmvb_MainChannel]  = PanelWidth;
		gmvw_OutputVActive[gmvb_MainChannel] = PanelHeight;
		gmvw_OutputHStart[gmvb_MainChannel]  = 0;
		gmvw_OutputVStart[gmvb_MainChannel] = 0;
		
		switch(UserPrefMainMode)
		{
			default:
			case Main_FULL_SCREEN:
				gmvw_OutputHActive[gmvb_MainChannel]  = PanelWidth;
				gmvw_OutputVActive[gmvb_MainChannel] = PanelHeight;
				AdjustMainPosition();
				break;
			case Main_SMALL:
				gmvw_OutputHActive[gmvb_MainChannel]  = PanelWidth /6;
				gmvw_OutputVActive[gmvb_MainChannel] = PanelHeight/6;
				AdjustMainPosition();
				break;
			case Main_MEDIUM:
				gmvw_OutputHActive[gmvb_MainChannel]  = PanelWidth/5;
				gmvw_OutputVActive[gmvb_MainChannel] = PanelHeight/5;
				AdjustMainPosition();
				break;
			case Main_LARGE:
				gmvw_OutputHActive[gmvb_MainChannel]  = PanelWidth/4;
				gmvw_OutputVActive[gmvb_MainChannel] = PanelHeight/4;
				AdjustMainPosition();
				break;
			case Main_HUGE:
				gmvw_OutputHActive[gmvb_MainChannel]  = PanelWidth/3;
				gmvw_OutputVActive[gmvb_MainChannel] = PanelHeight/3;
				AdjustMainPosition();
				break;
				
			case Main_SIDE_BY_SIDE:
				gmvw_OutputHActive[gmvb_MainChannel] = ((PanelWidth - 3 * PIP_BORDER_WIDTH) / 2 + SW_DynScalePosition);		
				#if (DEFAULT_PANEL == LG_WHD_LM290WW1_SSA1)
					gmvw_OutputVActive[gmvb_MainChannel] = (PanelHeight - (2 * PIP_BORDER_HEIGHT));
				#else
					gmvw_OutputVActive[gmvb_MainChannel] = (WORD)(((DWORD)gmvw_OutputHActive[gmvb_MainChannel] * 3) / 4);
				#endif
				switch(UserPrefMainPosition)
				{
					case BOTTOM_RIGHT:
					case TOP_RIGHT:
						gmvw_OutputHStart[gmvb_MainChannel] = PanelWidth/2;
						gmvw_OutputVStart[gmvb_MainChannel] = ((PanelHeight - gmvw_OutputVActive[gmvb_MainChannel]) / 2);		
					break;
					//Below are align to left start for SBS;
					#if(IS_PROJ_PD())//||(PROJECT_SELECT == PROJ_EH_MNT))
					case POS_FULL_SCREEN:
					#endif
					case BOTTOM_LEFT:
					case TOP_LEFT:
					default:
						gmvw_OutputHStart[gmvb_MainChannel] = 0;
						gmvw_OutputVStart[gmvb_MainChannel] = ((PanelHeight - gmvw_OutputVActive[gmvb_MainChannel]) / 2);			
					break;
				}
			break;
			
			case Main_SIDE_BY_SIDE_TALL:
				gmvw_OutputHActive[gmvb_MainChannel] = PanelWidth/2;		
				gmvw_OutputVActive[gmvb_MainChannel] = PanelHeight;

				switch(UserPrefMainPosition)
				{
					case BOTTOM_RIGHT:
					case TOP_RIGHT:
						gmvw_OutputHStart[gmvb_MainChannel] = PanelWidth/2;
						gmvw_OutputVStart[gmvb_MainChannel] = 0;		
					break;
					//Below are align to left start for SBS;
					#if(IS_PROJ_PD())
					case POS_FULL_SCREEN:
					#endif
					case BOTTOM_LEFT:
					case TOP_LEFT:
					default:
						gmvw_OutputHStart[gmvb_MainChannel] = 0;
						gmvw_OutputVStart[gmvb_MainChannel] = 0;			
					break;
				}	
			break;
			#if(IS_PROJ_PD())//||(PROJECT_SELECT == PROJ_EH_MNT))
			case Main_FLEXIBLE:				//8
				if((GetFlexiblePxpWindow(gmvb_MainChannel)->W_Width != 0)&&(GetFlexiblePxpWindow(gmvb_MainChannel)->W_Height != 0))
				{
					gmvw_OutputHActive[gmvb_MainChannel]  = GetFlexiblePxpWindow(gmvb_MainChannel)->W_Width;
					gmvw_OutputVActive[gmvb_MainChannel] = GetFlexiblePxpWindow(gmvb_MainChannel)->W_Height;
					gmvw_OutputHStart[gmvb_MainChannel] = GetFlexiblePxpWindowLimit(gmvb_MainChannel)->W_Hstart +GetFlexiblePxpWindow(gmvb_MainChannel)->W_Hstart;
					gmvw_OutputVStart[gmvb_MainChannel] = GetFlexiblePxpWindowLimit(gmvb_MainChannel)->W_Vstart + GetFlexiblePxpWindow(gmvb_MainChannel)->W_Vstart;
					gm_Print("GetFlexiblePxpWindow(gmvb_MainChannel)->W_Width = %d ", GetFlexiblePxpWindow(gmvb_MainChannel)->W_Width);
					gm_Print("GetFlexiblePxpWindow(gmvb_MainChannel)->W_Height = %d ", GetFlexiblePxpWindow(gmvb_MainChannel)->W_Height);
					gm_Print("GetFlexiblePxpWindow(gmvb_MainChannel)->W_Hstart = %d ", GetFlexiblePxpWindow(gmvb_MainChannel)->W_Hstart);
					gm_Print("GetFlexiblePxpWindow(gmvb_MainChannel)->W_Vstart = %d ", GetFlexiblePxpWindow(gmvb_MainChannel)->W_Vstart);
				
				}
			break;
			case Main_SIDE_BY_SIDE_PUP:
				gmvw_OutputHActive[gmvb_MainChannel] = PanelWidth;		
				gmvw_OutputVActive[gmvb_MainChannel] = PanelHeight/2;
				switch(UserPrefMainPosition)
				{
					case BOTTOM_RIGHT:
					case BOTTOM_LEFT:
						gmvw_OutputHStart[gmvb_MainChannel] = 0;
						gmvw_OutputVStart[gmvb_MainChannel] = PanelHeight/2;		
					break;
					//Below are align to left start for SBS;
					case POS_FULL_SCREEN:
					case TOP_RIGHT:
					case TOP_LEFT:
					default:
						gmvw_OutputHStart[gmvb_MainChannel] = 0;
						gmvw_OutputVStart[gmvb_MainChannel] = 0;			
					break;
				}

			break;
	
			case Main_QUARTER_PXP:
				gmvw_OutputHActive[gmvb_MainChannel] = PanelWidth/2;		
				gmvw_OutputVActive[gmvb_MainChannel] = PanelHeight/2;
				switch(UserPrefMainPosition)
				{
					case POS_QUARTER_PXP_BR:
						gmvw_OutputHStart[gmvb_MainChannel] = PanelWidth/2;
						gmvw_OutputVStart[gmvb_MainChannel] = PanelHeight/2;	
					break;
					
					case POS_QUARTER_PXP_BL:
						gmvw_OutputHStart[gmvb_MainChannel] = 0;
						gmvw_OutputVStart[gmvb_MainChannel] = PanelHeight/2;		
					break;
					case POS_QUARTER_PXP_TR:
						gmvw_OutputHStart[gmvb_MainChannel] = PanelWidth/2;
						gmvw_OutputVStart[gmvb_MainChannel] = 0;							
					break;
					
					case POS_QUARTER_PXP_TL:
						gmvw_OutputHStart[gmvb_MainChannel] = 0;
						gmvw_OutputVStart[gmvb_MainChannel] = 0;							
					break;
					
					case POS_FULL_SCREEN:
					default:
						gmvw_OutputHStart[gmvb_MainChannel] = 0;
						gmvw_OutputVStart[gmvb_MainChannel] = 0;			
					break;
				}
			break;
		#endif
		}

	//gm_Print("gmvw_OutputHStart[gmvb_MainChannel] = %d ",  gmvw_OutputHStart[gmvb_MainChannel]);
	//gm_Print("gmvw_OutputVStart[gmvb_MainChannel] = %d ",  gmvw_OutputVStart[gmvb_MainChannel]);
	//gm_Print("gmvw_OutputHActive[gmvb_MainChannel] = %d ",  gmvw_OutputHActive[gmvb_MainChannel]);
	//gm_Print("gmvw_OutputVActive[gmvb_MainChannel] = %d ",  gmvw_OutputVActive[gmvb_MainChannel]);
	
	ModeSetupDisplayScaling(gmvb_MainChannel, 1);

	//gm_Print("11gmvw_OutputHStart[gmvb_MainChannel] = %d ",  gmvw_OutputHStart[gmvb_MainChannel]);
	//gm_Print("11gmvw_OutputVStart[gmvb_MainChannel] = %d ",  gmvw_OutputVStart[gmvb_MainChannel]);
	//gm_Print("11gmvw_OutputHActive[gmvb_MainChannel] = %d ",  gmvw_OutputHActive[gmvb_MainChannel]);
	//gm_Print("11gmvw_OutputVActive[gmvb_MainChannel] = %d ",  gmvw_OutputVActive[gmvb_MainChannel]);
}
void AdjustPxpMode(void){}


void AdjustMainBlendLevel(void){}

#if ((BOARD_USED == BOARD_93xx_CJ2))
BOOL Board_External_Detected(void)
{
	return ((gm_ReadRegWord(GPINPUT5)&GPIO88_IN) == gmd_FALSE);
}
#endif




void AdjustUartBaudRate(void)
{
	DWORD DW_BaudRate;

	DW_BaudRate = GetUartBaudRate();
	gm_UartInit(UserPrefUartSelect, gmc_OCLK_FREQ, DW_BaudRate, &uartData);

}

DWORD GetUartBaudRate(void)
{
	DWORD DW_BaudRate;
	
 	switch(UserPrefUartBaudRate)
 	{
 		case BR_9600:
 			DW_BaudRate = 9600UL;
 		break;
 		
 		case BR_19200:
 			DW_BaudRate = 19200UL;
 		break;
 		
 		case BR_38400:
 			DW_BaudRate = 38400UL;
 		break;
 		
 		case BR_57600:
 			DW_BaudRate = 57600UL;
 		break;
 		
 		case BR_115200:
 			DW_BaudRate = 115200UL;
 		break;
 		
 		default:
 			#ifdef UART_DEBUG_MODE
 				DW_BaudRate = 115200UL;
 			#else
 				DW_BaudRate = BAUDRATE_SELECT;
 			#endif
 		break;
 	}

	return DW_BaudRate;
}

void CheckInputAlignment(gmt_PHY_CH B_Channel)
{
	if(IsDigitalInput(B_Channel))
	{
		//BYTE B_ImageZoom = (B_Channel == CH_A)? UserPrefMainImageZoom : UserPrefPxpImageZoom;
		//BYTE PD_CTRL = (B_Channel == CH_A)? UserPrefPD_MainCtrl : UserPrefPD_PxpCtrl;
		{
			WORD W_IBDWidth, W_IBDHeight;
			
			W_IBDWidth = gm_ReadRegWord(ip_ibd_hwidth[B_Channel]);
			W_IBDHeight = gm_ReadRegWord(ip_ibd_vlength[B_Channel]);
			#ifdef UART_DEBUG_MODE
			gm_Print(" CheckInputAlignment W_IBDWidth	= 0x%x ",W_IBDWidth);
			gm_Print(" CheckInputAlignment stInput[B_Channel].Hactive	= 0x%x ",stInput[B_Channel].Hactive);
			gm_Print(" CheckInputAlignment stInput[B_Channel].Hstart	= 0x%x ",stInput[B_Channel].Hstart);
			gm_Print(" CheckInputAlignment W_IBDHeight	= 0x%x ",W_IBDHeight);
			gm_Print(" CheckInputAlignment stInput[B_Channel].Vactive	= 0x%x ",stInput[B_Channel].Vactive);
			gm_Print(" CheckInputAlignment stInput[B_Channel].Vstart	= 0x%x ",stInput[B_Channel].Vstart);
			#endif

			/*
			if ((abs(stInput[B_Channel].Hactive - W_IBDWidth) > 3)||(abs(stInput[B_Channel].Vactive - W_IBDHeight)> 3)
				||((gm_ReadRegWord(ip_h_act_start[B_Channel]) >= 4)&&(gm_ReadRegWord(ip_v_act_start_odd[B_Channel]) >= 4))
				||((gm_ReadRegWord(ip_h_act_start[B_Channel]) != 0)&&(gm_ReadRegWord(ip_v_act_start_odd[B_Channel]) > 4))
				||(B_ImageZoom != 0)||gm_IsCaptureCrop(B_Channel)
				#if(IS_PROJ_PD())
				||Is_PD_ON(B_Channel)||(PD_CTRL == ON)
				#endif
			)
			*/
			if((abs(stInput[B_Channel].Hactive - W_IBDWidth) < 3)&&(abs(stInput[B_Channel].Vactive - W_IBDHeight) < 3))			
			{
				if((gm_ReadRegWord(ip_h_act_start[B_Channel]) < 4)||(gm_ReadRegWord(ip_v_act_start_odd[B_Channel]) < 4))
				{
					gm_WriteRegWord(ip_h_act_start[B_Channel],0);
					gm_WriteRegWord(ip_v_act_start_odd[B_Channel],0);
					gm_WriteRegWord(ip_v_act_start_even[B_Channel],1);
					gm_SetRegBitsWord(ip_control[B_Channel],IMP_EXT_DV_EN);
				}
				else
				{
					gm_WriteRegWord(ip_h_act_start[B_Channel],stInput[B_Channel].Hstart);
					gm_WriteRegWord(ip_v_act_start_odd[B_Channel],stInput[B_Channel].Vstart);
					gm_WriteRegWord(ip_v_act_start_even[B_Channel],stInput[B_Channel].Vstart + 1);
					gm_ClearRegBitsWord(ip_control[B_Channel],IMP_EXT_DV_EN);
				}
			}
			else
				gm_ClearRegBitsWord(ip_control[B_Channel],IMP_EXT_DV_EN);
			
				
		}

	}
	else
	{
		gm_ClearRegBitsWord(ip_control[B_Channel],IMP_EXT_DV_EN);
	}
}


void SetOSDByPanelParameters(void)
{
	if((stInput[gmvb_MainChannel].Hactive > 2560)&&(PanelWidth > 2560)&&(gm_IsValidSignal(gmvb_MainChannel)))
	{
		gm_WriteRegWord(OSD_H_ACTIVE,PanelWidth/2);
		//gm_WriteRegWord(OSD_H_DELAY,2560/4);
		gm_WriteRegWord(OSD_H_DELAY,0);
	}
	else
	{
		gm_WriteRegWord(OSD_H_ACTIVE,PanelWidth/2);
		gm_WriteRegWord(OSD_H_DELAY,0);
	}
	
	gm_WriteRegWord(OSD_V_ACTIVE,PanelHeight);
	gm_ForceUpdate(CH_A,INPUT_OUTPUT_CLOCK);
}

#define MuteDelay		10
void AdjustAudioMute(void)
{
	UserPrefAudioMute ^= 1;
	if(UserPrefAudioMute == ON)
	{
		AUDIO_MUTE();
		gm_Delay10ms(MuteDelay);
	}

	
	AdjustAudio();
	
	if(UserPrefAudioMute == OFF)   // Neil 140816 for audio noise when change pip source&mute on off
	{
		gm_Delay10ms(MuteDelay);
		AUDIO_UNMUTE();
	}

}

void AdjustAudioInputSource(void)
{
	AUDIO_MUTE();
	//B_NeedRedoDACGain = gmd_TRUE;

	gm_Delay10ms(MuteDelay);
	
	AdjustAudio();
	//Audio_RedoDACGain();
	ToggleAudioDACAttn();
	AUDIO_UNMUTE();
}

void StartNoCableSleep(void)
{
#if(defined(BOARD_93XX_MNT))
	NO_CABLE_SLEEP = gmd_TRUE;
#endif
}

void CheckScanlingFilterCtrl(gmt_PHY_CH B_Channel)
{
	//if(UserPrefPanelSelect == ZDT_4K2K_QFHD)
	if(stInput[B_Channel].Hactive > 2560)
	{
		if(gmvw_OutputHActive[B_Channel] > 2560)
		{
			if(B_Channel == gmvb_MainChannel)
			{
			   gm_WriteRegWord(MAIN_SCALING_FILTER_CTRL,0);
			   gm_ForceUpdate(B_Channel,INPUT_OUTPUT_CLOCK);
			}
			else
			{
			   gm_WriteRegWord(PIP_SCALING,0);
			   gm_ForceUpdate(B_Channel,INPUT_OUTPUT_CLOCK);
			}
		}
	}
}

BYTE GetPrevImageRotation(void)
{
	BYTE PrevIR = OSD_NON_ROTATED;
	
	switch(UserPrefImageRotation)
	{
		default:
		case OSD_NON_ROTATED:
			#if((DEFAULT_PANEL == ZDT_4K2K_CMI_39)||(DEFAULT_PANEL == ZDT_4K2K_CMI_65)||(DEFAULT_PANEL == ZDT_4K2K_CMI_50))
			PrevIR = OSD_ROTATED_180;
			#else
			PrevIR = OSD_ROTATED_270;
			#endif
		break;
		case OSD_ROTATED_90:
			PrevIR = OSD_NON_ROTATED;
		break;
		//Not support I2S and Spdif, all router to auto
		case OSD_ROTATED_180:
			#if((DEFAULT_PANEL == ZDT_4K2K_CMI_39)||(DEFAULT_PANEL == ZDT_4K2K_CMI_65)||(DEFAULT_PANEL == ZDT_4K2K_CMI_50))
			PrevIR = OSD_NON_ROTATED;
			#else
			PrevIR = OSD_ROTATED_90;
			#endif
		break;
		case OSD_ROTATED_270:
			PrevIR = OSD_ROTATED_180;
		break;
	}
	return PrevIR;
}

BYTE GetNextImageRotation(void)
{
	BYTE NextIR = OSD_NON_ROTATED;
	
	switch(UserPrefImageRotation)
	{
		default:
		case OSD_NON_ROTATED:

			#if((DEFAULT_PANEL == ZDT_4K2K_CMI_39)||(DEFAULT_PANEL == ZDT_4K2K_CMI_65)||(DEFAULT_PANEL == ZDT_4K2K_CMI_50))
			NextIR = OSD_ROTATED_180;
			#else
			NextIR = OSD_ROTATED_90;
			#endif
		break;
		case OSD_ROTATED_90:
			NextIR = OSD_ROTATED_180;
		break;
		case OSD_ROTATED_180:
			#if((DEFAULT_PANEL == ZDT_4K2K_CMI_39)||(DEFAULT_PANEL == ZDT_4K2K_CMI_65)||(DEFAULT_PANEL == ZDT_4K2K_CMI_50))
			NextIR = OSD_NON_ROTATED;
			#else
			NextIR = OSD_ROTATED_270;
			#endif
		break;
		case OSD_ROTATED_270:
			NextIR = OSD_NON_ROTATED;
		break;
	}
	//gm_Print(" GetNextImageRotation  = %d ",NextIR);
	return NextIR;
}


BYTE GetPrevOSDRotation(void)
{
	BYTE PrevOR = OSD_NON_ROTATED;
	
	switch(UserPrefOsdRotation)
	{
		default:
		case OSD_NON_ROTATED:
			#if(defined(BOARD_93XX_4K2K))
			PrevOR = OSD_ROTATED_180;
			#else
			PrevOR = OSD_ROTATED_270;
			#endif
		break;
		case OSD_ROTATED_90:
			PrevOR = OSD_NON_ROTATED;
		break;
		//Not support I2S and Spdif, all router to auto
		case OSD_ROTATED_180:
			#if(defined(BOARD_93XX_4K2K))
			PrevOR = OSD_NON_ROTATED;
			#else
			PrevOR = OSD_ROTATED_90;
			#endif
		break;
		case OSD_ROTATED_270:
			PrevOR = OSD_ROTATED_180;
		break;
	}
	return PrevOR;
}

BYTE GetNextOSDRotation(void)
{
	BYTE NextOR = OSD_NON_ROTATED;
	
	switch(UserPrefOsdRotation)
	{
		default:
		case OSD_NON_ROTATED:

			#if(defined(BOARD_93XX_4K2K))
			NextOR = OSD_ROTATED_180;
			#else
			NextOR = OSD_ROTATED_90;
			#endif
		break;
		case OSD_ROTATED_90:
			NextOR = OSD_ROTATED_180;
		break;
		case OSD_ROTATED_180:
			#if(defined(BOARD_93XX_4K2K))
			NextOR = OSD_NON_ROTATED;
			#else
			NextOR = OSD_ROTATED_270;
			#endif
		break;
		case OSD_ROTATED_270:
			NextOR = OSD_NON_ROTATED;
		break;
	}
	//gm_Print(" GetNextOSDRotation  = %d ",NextOR);
	return NextOR;
}


void CheckNoSyncBKGColor(gmt_PHY_CH B_Channel)
{
	if(B_SplashComplete == gmd_FALSE)
		return;
	if((B_Channel == CH_B)&&(!Is_PIP_ON()))
		return;
	
	if(!gm_IsValidSignal(B_Channel)||(gm_IsModeOOR(B_Channel)&&(B_Channel == CH_A)))
	{
		BYTE PB_Enable = (B_Channel == CH_A)? PB_MAIN_ENABLE : PB_PIP_ENABLE;
		if(((gm_ReadRegWord(PB_CTRL)&PB_Enable) == PB_Enable)
			||((MAIN_BACKGROUND_COLOR != BLACK)&&(gm_ReadRegWord(PB_BKGND) == 0)))
			//||((PIP_BACKGROUND_COLOR != BLACK)&&(gm_ReadRegWord(PB_BKGND) == 0)))//Neil 140701  for background issue
		{
			#ifdef UART_DEBUG_MODE
			gm_Printf("=======%d==CheckNoSyncBKGColor == %d= ",B_Channel, (gm_ReadRegWord(PB_CTRL)&PB_Enable));
			#endif
			SetDisplayOff(B_Channel,BLENDER_OFF);
		}
	}

}

BOOL CheckDPLinkError(gmt_PHY_CH B_Channel)
{
	static BYTE DPResetCNT = 0;
	BYTE InputConnector = (B_Channel == CH_A)? UserPrefCurrentInputMain : UserPrefCurrentInputPip ;
	
	if(GetPortConnType(InputConnector)==CT_DISPLAY_PORT)
	{	
		if((gm_ReadRegWord(DP12RX0_MS_IRQ_STATUS)&DPRX_MS_VID_ERR_IRQ_STS == DPRX_MS_VID_ERR_IRQ_STS)
			&&(gm_ReadRegDWord(DP12RX0_LINK_CONTROL)&(DPRX_8B10B_DI_SYMBOLS_SWP|DPRX_ALGN_BLCK_MSK))
			&&(((gm_ReadRegWord(DP12RX0_LINK_IRQ_STS)&&DPRX_CR_LOST_IRQ_STS) != DPRX_CR_LOST_IRQ_STS)
			||(gm_ReadRegWord(DP12RX0_ERROR_CNT) >= 0xF000)
			||(gm_ReadRegWord(DP12RX0_DPCD_STATUS)&0x2222 != 0x00)
			))
		{
			#ifdef UART_DEBUG_MODE
			//gm_Print("=============================>>>>CheckDPLink : Need Reset %d",0);
			#endif
			//if((gm_ReadRegDWord(DP12RX0_LINK_CONTROL) != 0x300F030)&&(gm_TimerCheck(DP_Recover_100ms_TMR) != TIMER_OK))//&&(DPResetCNT < 6))
			//if()
			if(gm_TimerCheck(DP_Recover_100ms_TMR) != TIMER_OK)
			{
				//if(gm_ReadRegWord(DP12RX0_LINK_IRQ_STS)&&(DPRX_ALIGN_LOST_IRQ_STS|DPRX_DEC_IRQ_STS|DPRX_SYM_LOCK_LOST_IRQ_STS))
				//((gm_ReadRegWord(DP12RX0_LINK_IRQ_STS)&&DPRX_CR_LOST_IRQ_STS) != DPRX_CR_LOST_IRQ_STS)
					

				
				DPResetCNT++;
				gm_TimerStart(DP_Recover_100ms_TMR, 2);
				#ifdef UART_DEBUG_MODE
				gm_Print("====>>>>CheckDPLinkError : Need Reset %d",gm_GetSystemTime());
				#endif
				#if 1
				gm_WriteRegDWord(DP12RX0_BE_RESET_CTRL,0x07);
				gm_WriteRegDWord(DP12RX0_LINK_CONTROL,0x300F030);
				//if(DPResetCNT >3)
					AdjustDPSwitch();
				#endif
				return gmd_FALSE;
			}
		}
		DPResetCNT = 0;
	}
	return gmd_TRUE;
}

void AdjustDimEffect(void)
{
#if(DEFAULT_PANEL == SHARP_1080P_LD900D3LE1)
	DWORD W_Pulse;
    //BYTE B_TempBrightness = UserPrefBrightness + 128;
    BYTE B_TempDefaultBrightness;
    //BYTE MaxMin;
    //WORD BriBaseTemp = 0;
    DWORD BriBase;
    //20120605 Modify by KevinChen for input range 0-100
    //gm_Print("UserPrefBrightnes-%d",UserPrefBrightness);
    //MaxMin = MAX_BACKLIGHT_PWM - MIN_BACKLIGHT_PWM; 

	BriBase=UserPrefBriBase_6500K;//0x5B;//20120706 Follow Qisda mail request by KevinChen//20120530 KevinChen
	gm_Print("AdjustBackLightZoneEffect BriBase- %d",BriBase);
		
	if(BriBase>=MAX_B_BACKLIGHT_PWM) BriBase=MAX_B_BACKLIGHT_PWM;//20120719 Add by KevinChen
	if(BriBase<=MIN_B_BACKLIGHT_PWM) BriBase=MIN_B_BACKLIGHT_PWM;//20120719 Add by KevinChen
	
	W_Pulse = ((DWORD)(MAX_B_BACKLIGHT_PWM - MIN_B_BACKLIGHT_PWM) * (UserPrefDimEffect)) /(WB_BRIGHTNESS_MAX - WB_BRIGHTNESS_MIN)+ MIN_B_BACKLIGHT_PWM;

	 if (W_Pulse <= MIN_B_BACKLIGHT_PWM)
	       W_Pulse = MIN_B_BACKLIGHT_PWM;
	 if (W_Pulse >= MAX_B_BACKLIGHT_PWM)
	       W_Pulse = MAX_B_BACKLIGHT_PWM;

	
	#if BACKLIGHT_PWM_INVERT
	           W_Pulse = MAX_B_PWMPERIOD - W_Pulse;//Invert W_Pulse Value
		//W_Pulse = MAX_BACKLIGHT_PWM - W_Pulse + MIN_BACKLIGHT_PWM;	//Invert W_Pulse Value
	#endif
     //20120507 Set 1Khz for PWM1,3 by KevinChen
	gm_WriteRegWord(BACKLIGHT_B_PWM_CTRL, (BYTE)MAX_B_PWMPERIOD |(W_Pulse << 8));
	gm_WriteRegWord(BACKLIGHT_B_PWM_CTRL_MSB, (MAX_B_PWMPERIOD >> 8) | (W_Pulse & 0x0F00));

	//gm_WriteRegWord(BACKLIGHT_B_PWM_CTRL_MSB, ((MAX_B_BACKLIGHT_PWM&PWM0_PULSE_MSB))|((MAX_B_PWMPERIOD>>8)&PWM0_PERIOD_MS)); // B_led control
	//gm_WriteRegWord(BACKLIGHT_B_PWM_CTRL, ((WORD)(MAX_B_PWMPERIOD&PWM0_PERIOD) |(MAX_B_BACKLIGHT_PWM<<8)));	
#endif
};

#ifdef OSD_MAX_TEXT_BUFFER_SIZE//20131206 Mark Liao for 1080p OSD	
#if (OSD_MAX_TEXT_BUFFER_SIZE != 0)
extern WORD PixelExpansionTable[(OSD_MAX_TEXT_BUFFER_SIZE + 1) / 2];
#endif


void ReInitOSD(void)
{
#if 1 //20131213 Mark Liao for 1080p OSD		 
	gm_OsdInit(MAX_NUMBER_OF_TILES_USED, TILESET_0_ADDRESS, TILESET_1_ADDRESS);
	//pixel expansion table init
#ifdef OSD_MAX_TEXT_BUFFER_SIZE
#if (OSD_MAX_TEXT_BUFFER_SIZE != 0)
	gmv_OsdTextStringDecompBuffer = (BYTE *)PixelExpansionTable;
	gm_OsdPixelExpansionBufferInit((BYTE near *)PixelExpansionTable,
									sizeof(PixelExpansionTable) / 2);
#endif //#if (OSD_MAX_TEXT_BUFFER_SIZE != 0)
#endif //#ifdef OSD_MAX_TEXT_BUFFER_SIZE

//
// OSD SM/library initialization
//
#ifdef OSD_WB_USED
#ifdef RENDER_ENABLED
	gm_OsdSetFontVector(gmv_GlyphTblSet, NUMBER_OF_GLYPH_TABLES);
#endif //#ifdef RENDER_ENABLED

	gm_OsdSmInitThreadsContext();
#endif
#endif


}
#endif


#ifdef DYNAMIC_OUTPUT
void PanelSetAsInputTiming (void)
{
   #ifdef DYNAMIC_OUTPUT
   BYTE AthenaOutputTiming = T3840x2160_30Hz;
   #endif
   if(gmvw_InputHActiveOrg[gmvb_MainChannel]%2)
   {
      gm_Printf("~~~~ Input is not correct force re-detect %d", gmvw_InputHActiveOrg[gmvb_MainChannel]);
      gm_ForceModeChanged(gmvb_MainChannel);
      return;
   }

   if(gmvw_InputHStartOrg[gmvb_MainChannel] == 0)
   {      
      gm_ForceModeChanged(gmvb_MainChannel);
      return;
   }   

   _fmemcpy((BYTE far *)&CurPanelData, (BYTE far *)CurrentPanelData, sizeof(gmt_PanelData));
   CurrentPanelData = (gmt_PanelData far *) &CurPanelData;

#ifdef DYNAMIC_OUTPUT
#ifdef UART_DEBUG_MODE
//gm_Print("@@@UserPrefImagePresetMode@@@ - %d",UserPrefImagePresetMode);
gm_Print("@@@ H - %d",stInput[gmvb_MainChannel].Hactive);
gm_Print("@@@ V - %d",stInput[gmvb_MainChannel].Vactive);
gm_Print("@@@ Vf - %d",stInput[gmvb_MainChannel].Vfrq);
gm_Print("@@@ ori H - %d",gmvw_InputHActiveOrg[gmvb_MainChannel]);
gm_Print("@@@ ori V - %d",gmvw_InputVActiveOrg[gmvb_MainChannel]);
gm_Print("@@@ Interlace - %d",(gm_IsInterlaced(gmvb_MainChannel) ));
#endif

   if(IsPresetMode_VIDEO_MOVIE&&//PA168 video mode enable
      ((stInput[gmvb_MainChannel].Hactive == 1920)&&(stInput[gmvb_MainChannel].Vactive == 1080)&&abs(stInput[gmvb_MainChannel].Vfrq-240) <= 5)//1920x1080@24
      )
      {
#ifdef UART_DEBUG_MODE
      gm_Print("A",0);
#endif
         AthenaOutputTiming = T1920x1080_48Hz;
      }
   else if(/*IsPresetMode_VIDEO_MOVIE &&//PA168 video mode enable*/
      ((((stInput[gmvb_MainChannel].Hactive == 1920)&&(stInput[gmvb_MainChannel].Vactive == 1080)&&abs(stInput[gmvb_MainChannel].Vfrq-250) <5) ||//1920x1080@25
      ((stInput[gmvb_MainChannel].Hactive == 1920)&&(stInput[gmvb_MainChannel].Vactive == 1080)&&abs(stInput[gmvb_MainChannel].Vfrq-500) <10)) ||//1920x1080@50
      ((gmvw_InputHActiveOrg[gmvb_MainChannel] == 720)&&(gmvw_InputVActiveOrg[gmvb_MainChannel] == 576)&&abs(stInput[gmvb_MainChannel].Vfrq-500) <10) ||//720x576P50
      ((gmvw_InputHActiveOrg[gmvb_MainChannel] == 1280)&&(gmvw_InputVActiveOrg[gmvb_MainChannel] == 720)&&abs(stInput[gmvb_MainChannel].Vfrq-500) <10))//1280x720P50
      )
      {
#ifdef UART_DEBUG_MODE
      gm_Print("B",0);
#endif
         AthenaOutputTiming = T1920x1080_50Hz;
      }
   else if(/*IsPresetMode_VIDEO_MOVIE &&//PA168 video mode enable*/
      (((stInput[gmvb_MainChannel].Hactive == 1920)&&(stInput[gmvb_MainChannel].Vactive == 1080)&&abs(stInput[gmvb_MainChannel].Vfrq-600) <10) ||//1920x1080@60
      ((gmvw_InputHActiveOrg[gmvb_MainChannel] == 720)&&(gmvw_InputVActiveOrg[gmvb_MainChannel] == 480)&&abs(stInput[gmvb_MainChannel].Vfrq-600) <10) ||//720x480P60
      ((gmvw_InputHActiveOrg[gmvb_MainChannel] == 1280)&&(gmvw_InputVActiveOrg[gmvb_MainChannel] == 720)&&abs(stInput[gmvb_MainChannel].Vfrq-600) <10))//1280x720P60
      )
      {
#ifdef UART_DEBUG_MODE
      gm_Print("C",0);
#endif
         AthenaOutputTiming = T1920x1080_60Hz;
      }
   else if(IsPresetMode_VIDEO_MOVIE &&//PA168 video mode enable
      ((stInput[gmvb_MainChannel].Hactive == 3840)&&(stInput[gmvb_MainChannel].Vactive == 2160)&&abs(stInput[gmvb_MainChannel].Vfrq-240) <10)//3840x2160@24
      )
      {
#ifdef UART_DEBUG_MODE
      gm_Print("D",0);
#endif
         AthenaOutputTiming = T3840x2160_24Hz;
      }
   else if((gm_IsInterlaced(gmvb_MainChannel)) &&//interlace mode
      ((gmvw_InputHActiveOrg[gmvb_MainChannel] == 1920)&&(gmvw_InputVActiveOrg[gmvb_MainChannel] == 540)&&abs(stInput[gmvb_MainChannel].Vfrq-600) <10) ||//1920x1080i60
      (((gmvw_InputHActiveOrg[gmvb_MainChannel] == 720)||(gmvw_InputHActiveOrg[gmvb_MainChannel] == 1440))&&(gmvw_InputVActiveOrg[gmvb_MainChannel] == 240)&&abs(stInput[gmvb_MainChannel].Vfrq-600) <10)//720x480i60
	  ||((gmvw_InputHActiveOrg[gmvb_MainChannel] == 1920)&&(gmvw_InputVActiveOrg[gmvb_MainChannel] == 540)&&abs(stInput[gmvb_MainChannel].Vfrq-500) <10)
	  )
      {
#ifdef UART_DEBUG_MODE
      gm_Print("E",0);
#endif
         AthenaOutputTiming = T1920x1080_60Hz;		//Hyb140610- image abnormal when testing in skyworth. 
		  //AthenaOutputTiming = T3840x2160_30Hz;  	////Hyb140730- NG in Asianda with GIEC DVD
      }
   else if((gm_IsInterlaced(gmvb_MainChannel)) &&//interlace mode
      (((gmvw_InputHActiveOrg[gmvb_MainChannel] == 720)||(gmvw_InputHActiveOrg[gmvb_MainChannel] == 1440))&&(gmvw_InputVActiveOrg[gmvb_MainChannel] == 288)&&abs(stInput[gmvb_MainChannel].Vfrq-500) <10)//720x288i50
      )
      {
#ifdef UART_DEBUG_MODE
      gm_Print("F",0);
#endif
         AthenaOutputTiming = T1920x1080_50Hz;		//Hyb140610- no image if output as 1080p@50Hz, just set to 4K2K@30Hz
         //AthenaOutputTiming = T3840x2160_30Hz;			//Hyb140730- NG in Asianda with GIEC DVD
      }
   else if(IsGameModeApplicable()&& abs(stInput[gmvb_MainChannel].Vfrq-600) <10)
      {
#ifdef UART_DEBUG_MODE
      gm_Print("G",0);
#endif
         AthenaOutputTiming = T1920x1080_60Hz;
      }
   else
      {
#ifdef UART_DEBUG_MODE
      		gm_Print("H",0);
#endif
			//if(!Is_PD_ON(gmvb_MainChannel))
				AthenaOutputTiming = T3840x2160_30Hz;
			//else
			//	AthenaOutputTiming = T1920x1080_60Hz;
      }

   switch(AthenaOutputTiming)
   {
      case T1920x1080_48Hz:
#ifdef UART_DEBUG_MODE
         gm_Print("xxxxxxxxxxxxxx 1920x1080@48 xxxxxxxx",0);
#endif
   		PanelWidth = 1920;
   		PanelHeight = 1080;
   		PanelMaxVFreq = 48;
   		PanelMinVFreq = 48;
   		PanelMaxPClk = 148500;
   		PanelMinHTotal = 2200;
   		PanelTypHTotal = 2200;
   		Panel22HTotal = 2200;
   		Panel33HTotal = 2200;
   		PanelMinHSyncWidth = 16;
   		PanelMinHSyncBackPorch = 256;

   		Panel22VTotal=1125;
   		Panel33VTotal=1125;
   		PanelMaxVTotal=1125;
   		PanelTypVTotal=1125;

   		PanelMinVSyncWidth  = 3;

   		PanelMinVSyncBackPorch =39;
   		gmvw_OutputHStart[gmvb_MainChannel] = 0;
   		gmvw_OutputVStart[gmvb_MainChannel] = 0;
   		gmvw_OutputHActive[gmvb_MainChannel] = 1920;
   		gmvw_OutputVActive[gmvb_MainChannel] = 1080;
   		//UserPrefIsDyanmicOutput4K2K_Flag=FALSE;
      break;
      case T1920x1080_50Hz:
#ifdef UART_DEBUG_MODE
         gm_Print("xxxxxxxxxxxxxx 1920x1080@50 xxxxxxxx",0);
#endif
   		PanelWidth = 1920;
   		PanelHeight = 1080;
   		PanelMaxVFreq = 50;
   		PanelMinVFreq = 50;
   		PanelMaxPClk = 148500;
   		PanelMinHTotal = 2200;
   		PanelTypHTotal = 2200;
   		Panel22HTotal = 2200;
   		Panel33HTotal = 2200;
   		PanelMinHSyncWidth = 16;
   		PanelMinHSyncBackPorch = 256;

   		Panel22VTotal=1125;
   		Panel33VTotal=1125;
   		PanelMaxVTotal=1125;
   		PanelTypVTotal=1125;

   		PanelMinVSyncWidth  = 3;

   		PanelMinVSyncBackPorch =39;
   		gmvw_OutputHStart[gmvb_MainChannel] = 0;
   		gmvw_OutputVStart[gmvb_MainChannel] = 0;
   		gmvw_OutputHActive[gmvb_MainChannel] = 1920;
   		gmvw_OutputVActive[gmvb_MainChannel] = 1080;
   		//UserPrefIsDyanmicOutput4K2K_Flag=FALSE;
      break;
      case T1920x1080_60Hz:
#ifdef UART_DEBUG_MODE
         gm_Print("xxxxxxxxxxxxxx 1920x1080@60 xxxxxxxx",0);
#endif
   		PanelWidth = 1920;
   		PanelHeight = 1080;
   		PanelMaxVFreq = 60;
   		PanelMinVFreq = 60;
   		PanelMaxPClk = 148500;
   		PanelMinHTotal = 2200;
   		PanelTypHTotal = 2200;
   		Panel22HTotal = 2200;
   		Panel33HTotal = 2200;
   		PanelMinHSyncWidth = 16;
   		PanelMinHSyncBackPorch = 256;

   		Panel22VTotal=1125;
   		Panel33VTotal=1125;
   		PanelMaxVTotal=1125;
   		PanelTypVTotal=1125;

   		PanelMinVSyncWidth  = 3;

   		PanelMinVSyncBackPorch =39;
   		gmvw_OutputHStart[gmvb_MainChannel] = 0;
   		gmvw_OutputVStart[gmvb_MainChannel] = 0;
   		gmvw_OutputHActive[gmvb_MainChannel] = 1920;
   		gmvw_OutputVActive[gmvb_MainChannel] = 1080;
   		//UserPrefIsDyanmicOutput4K2K_Flag=FALSE;
      break;
      case T3840x2160_24Hz:
#ifdef UART_DEBUG_MODE
         gm_Print("xxxxxxxxxxxxxx 3840x2160@24 xxxxxxxx",0);
#endif
		   PanelWidth = 3840;
   		PanelHeight = 2160;
   		PanelMaxVFreq = 24;
   		PanelMinVFreq = 24;
   		PanelMaxPClk = 316000;
   		PanelMinHTotal = 3968;
   		PanelTypHTotal = 3968;
   		Panel22HTotal = 4000;
   		Panel33HTotal = 4000;
   		PanelMinHSyncWidth = 32;
   		PanelMinHSyncBackPorch = 16;

   		Panel22VTotal= 2250;
   		Panel33VTotal= 2250;
   		PanelMaxVTotal= 2208;
   		PanelTypVTotal= 2208;

   		PanelMinVSyncWidth  = 8;

   		PanelMinVSyncBackPorch = 16;
   		gmvw_OutputHStart[gmvb_MainChannel] = 0;
   		gmvw_OutputVStart[gmvb_MainChannel] = 0;
   		gmvw_OutputHActive[gmvb_MainChannel] = 3840;
   		gmvw_OutputVActive[gmvb_MainChannel] = 2160;	
   		//UserPrefIsDyanmicOutput4K2K_Flag=TRUE;
      break;
      case T3840x2160_30Hz:
      default:
#ifdef UART_DEBUG_MODE
         gm_Print("xxxxxxxxxxxxxx 3840x2160@30 xxxxxxxx",0);
#endif
		   PanelWidth = 3840;
   		PanelHeight = 2160;
   		PanelMaxVFreq = 30;
   		PanelMinVFreq = 29;
   		PanelMaxPClk = 316000;
   		PanelMinHTotal = 3968;
   		PanelTypHTotal = 3968;
   		Panel22HTotal = 4000;
   		Panel33HTotal = 4000;
   		PanelMinHSyncWidth = 32;
   		PanelMinHSyncBackPorch = 16;

   		Panel22VTotal= 2250;
   		Panel33VTotal= 2250;
   		PanelMaxVTotal= 2208;
   		PanelTypVTotal= 2208;

   		PanelMinVSyncWidth  = 8;

   		PanelMinVSyncBackPorch = 16;
   		gmvw_OutputHStart[gmvb_MainChannel] = 0;
   		gmvw_OutputVStart[gmvb_MainChannel] = 0;
   		gmvw_OutputHActive[gmvb_MainChannel] = 3840;
   		gmvw_OutputVActive[gmvb_MainChannel] = 2160;	

      break;
   }
#else
   if(gm_IsInterlaced(gmvb_MainChannel))
   {
      if(stInput[gmvb_MainChannel].Vtotal % 2)
      {
         stInput[gmvb_MainChannel].Vtotal -= 1;
      } 
   }

   PanelWidth = stInput[gmvb_MainChannel].Hactive;
   PanelHeight = stInput[gmvb_MainChannel].Vactive;
   PanelMaxVFreq = (stInput[gmvb_MainChannel].Vfrq + 5) / 10;
   PanelMinVFreq = (stInput[gmvb_MainChannel].Vfrq + 5) / 10;
   PanelMaxPClk = ((DWORD)stInput[gmvb_MainChannel].Htotal * stInput[gmvb_MainChannel].Vtotal/10) * stInput[gmvb_MainChannel].Vfrq/1000;
   PanelMinHTotal = stInput[gmvb_MainChannel].Htotal;
   PanelTypHTotal = stInput[gmvb_MainChannel].Htotal;
   Panel22HTotal = stInput[gmvb_MainChannel].Htotal;
   Panel33HTotal = stInput[gmvb_MainChannel].Htotal;

   {
      WORD W_Hperiod;
      WORD W_Hpulse;
      W_Hperiod =gm_ReadRegWord(MIFM_HS_PERIOD);
      W_Hpulse = gm_ReadRegWord(MIFM_HS_PULSE);

      if (W_Hpulse > (W_Hperiod / 2))
         PanelMinHSyncWidth   = W_Hperiod - W_Hpulse;
      else
         PanelMinHSyncWidth   = W_Hpulse;

      PanelMinHSyncWidth= (BYTE)((DWORD)(PanelTypHTotal*PanelMinHSyncWidth) / W_Hperiod);
      PanelMinHSyncWidth &= 0xF8;
      
      if (PanelMinHSyncWidth <8)
         PanelMinHSyncWidth = 8;
   }
   PanelMinHSyncBackPorch = stInput[gmvb_MainChannel].Hstart - PanelMinHSyncWidth;
   PanelMinHSyncBackPorch &= 0xFFF8;

   Panel22VTotal=stInput[gmvb_MainChannel].Vtotal;
   Panel33VTotal=stInput[gmvb_MainChannel].Vtotal;
   PanelMaxVTotal=stInput[gmvb_MainChannel].Vtotal;
   PanelTypVTotal=stInput[gmvb_MainChannel].Vtotal;

   {
      WORD W_Vperiod;
      WORD W_Vpulse;
      
      W_Vperiod = gm_ReadRegWord(MIFM_VS_PERIOD);
      W_Vpulse = gm_ReadRegWord(MIFM_VS_PULSE);
      
      if (W_Vpulse > (W_Vperiod / 2))
         PanelMinVSyncWidth   =  W_Vperiod - W_Vpulse;
      else
         PanelMinVSyncWidth   = W_Vpulse;
   }

   PanelMinVSyncBackPorch = stInput[gmvb_MainChannel].Vstart - PanelMinVSyncWidth;

   if(gm_IsInterlaced(gmvb_MainChannel))
   {
      PanelHeight = stInput[gmvb_MainChannel].Vactive * 2;
      PanelMaxPClk = ((DWORD)stInput[gmvb_MainChannel].Htotal * (stInput[gmvb_MainChannel].Vtotal*2 + 1)/10) * stInput[gmvb_MainChannel].Vfrq/1000;
      Panel22VTotal = stInput[gmvb_MainChannel].Vtotal * 2 + 1;
      Panel33VTotal = stInput[gmvb_MainChannel].Vtotal * 2 + 1;
      PanelMaxVTotal = stInput[gmvb_MainChannel].Vtotal * 2 + 1;
      PanelTypVTotal = stInput[gmvb_MainChannel].Vtotal * 2 + 1;
   }
#endif
	ZDT_4K2K_OutputConfig();
#if ((DEFAULT_PANEL == ZDT_4K2K_QFHD))
	gm_SetRegBitsWord(LVDS_DIGITAL_CTRL,WIDTH_SEL);
#endif
   gmv_OUTPUT_HTOTAL = PanelTypHTotal; // Typical
   gmv_OUTPUT_VTOTAL = PanelTypVTotal; // Typical
   gmv_OUTPUT_HTOTAL_22 = Panel22HTotal; // HTotal for 2:2 (60Hz to 48Hz conversion)
   gmv_OUTPUT_VTOTAL_22 = Panel22VTotal; // VTotal for 2:2 (60Hz to 48Hz conversion)
   gmv_OUTPUT_HTOTAL_33 = Panel33HTotal; // HTotal for 3:3 (60Hz to 72Hz conversion)
   gmv_OUTPUT_VTOTAL_33 = Panel33VTotal; // VTotal for 3:3 (60Hz to 72Hz conversion)
   gmv_OUTPUT_HACTIVE = PanelWidth;
   gmv_OUTPUT_VACTIVE = PanelHeight;      
   //ZDT_4K2K_CMI_Panel_Init();
}
#endif		//DYNAMIC_OUTPUT

#define DecoderCheckTime	 150	//check decoder status every 50ms

BOOL IsDecoderInputStable(gmt_PHY_CH B_Channel)
{
#ifdef EXTERNAL_DECODER
	static BYTE B_DecoderStatus = gmd_FALSE; 
	static DWORD DW_CheckTime; 

	if(B_SplashComplete == gmd_FALSE)
		return gmd_FALSE;

	if((gm_GetSystemTime() -DW_CheckTime) > DecoderCheckTime)
	{
		//static BYTE B_PreStableStates; 
		BYTE B_Input;
		
		DW_CheckTime = gm_GetSystemTime();
		B_Input = ((B_Channel == CH_A)? UserPrefCurrentInputMain : UserPrefCurrentInputPip);
		if((B_Channel == CH_B)&&(!Is_PIP_ON()))
		//if((IsPIPCVBSInput()||IsPIPCOMP2Input())&&(!Is_PIP_ON())&&(B_Channel == CH_B))
			return gmd_FALSE;
		
		if(!IsDecoderInput(B_Input))
			return gmd_FALSE;
		
		//if(Mission_TVP5160_IsSignalPresent(B_Channel))
		{	
			static BYTE B_StableCnt, B_UnStableCnt;
			PortAttrib ROM *pPort = &AppPortAttrib[B_Input];			  
			
			#if DECODER_IN_LPM
			B_DecoderStatus =  Mission_TVP5160_GetVideoStatus(B_Channel);
			#else
			B_DecoderStatus =  MIS_TVP5160GetVideoStatus(B_Channel);
			#endif
			if(B_DecoderStatus  == 0x02)
			{
				gm_ForceModeChange(B_Channel);
				B_DecoderStatus= gmd_FALSE;
			}
			else if(B_DecoderStatus  == D_LOCKED)
			{
				B_StableCnt++;
				B_UnStableCnt = 0;
				if(B_StableCnt >= pPort->B_StableCount)
					B_DecoderStatus = gmd_TRUE;
			}
			else
			{
				B_StableCnt = 0;
				B_UnStableCnt++;
				if(B_UnStableCnt >= pPort->B_UnstableCount)
					B_DecoderStatus= gmd_FALSE;

			}
		}

	}

	return B_DecoderStatus;
#else
UNUSED_VAR(B_Channel);
return gmd_FALSE;

#endif
}

void QFHD_LVDSConfig(void)
{
#if ((DEFAULT_PANEL == ZDT_4K2K_CMI)||(DEFAULT_PANEL == ZDT_4K2K_CMI_39)\
	||(DEFAULT_PANEL == ZDT_4K2K_CMI_65)||(DEFAULT_PANEL == ZDT_4K2K_CMI_50))
	{
		//static BYTE PanelMode = 00;	//Default is 3840x2160
		if((PanelWidth == 3840))//&&(PanelMode !=  0x00))
		{
			//PanelMode = 0x00;
		#ifdef UART_DEBUG_MODE
			gm_Print("xxxxxxxxxxxxxx 4K2K@30 xxxxxxxx",0);
		#endif
		#if((DEFAULT_PANEL == ZDT_4K2K_CMI)||(DEFAULT_PANEL == ZDT_4K2K_CMI_39))
				ReConfigLVDSSetting(PanelWidth);
		#endif
			ZDT_4K2K_CMI_Panel_QFHD30(UserPrefPanelSelect,0);
		}
		else if((PanelWidth == 1920))//&&(PanelMode !=	0x01))
		{
			//PanelMode = 0x01;
		#ifdef UART_DEBUG_MODE
			gm_Print("xxxxxxxxxxxxxx 1080p@30 xxxxxxxx",0);
		#endif
		#if((DEFAULT_PANEL == ZDT_4K2K_CMI)||(DEFAULT_PANEL == ZDT_4K2K_CMI_39))
				ReConfigLVDSSetting(PanelWidth);
		#endif
			ZDT_4K2K_CMI_Panel_FHD60(UserPrefPanelSelect,0);
		}
	}
#endif

}


void AdjustOSDAudioVolume(void)     //Neil 140728 for volume range 0-100
{
	if(UserPrefOSDAudioVolume >= 50)
		UserPrefAudioVolume = (BYTE)(((WORD)UserPrefOSDAudioVolume * (MAXIMAL_VOLUME_VALUE - DEFAULT_VOLUME_VALUE))/50 + DEFAULT_VOLUME_VALUE);
	else
		UserPrefAudioVolume = (BYTE)(((WORD)UserPrefOSDAudioVolume * (DEFAULT_VOLUME_VALUE - MINIMAL_VOLUME_VALUE))/50 + MINIMAL_VOLUME_VALUE);
	
	AdjustAudioVolume();
}


//******************************************************************************
//
// FUNCTION       : void SetOsdTilePosition_Flying(WORD W_TileId, WORD W_TileHpos, WORD W_TileVpos)
//
// USAGE          : Sets OSD tile position taking into account OSD rotation
//
// INPUT          : W_TileId - tile id of a tile
//                  W_TileHpos - new X position
//                  W_TileVpos - new Y position
//
// OUTPUT         : None
//
// GLOBALS        : gmv_OsdDialogs		(R)
//                  gmvb_OsdActiveDialog	(R)
//                  ModeIndependentUserPreferences.OsdRotation		(R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void SetOsdTilePosition_Flying(WORD W_TileId, WORD W_TileHpos, WORD W_TileVpos)
{
   gmt_OSD_DIALOG_CTRL ROM *Sp_DialogCtrl = (gmt_OSD_DIALOG_CTRL ROM *)(gmv_OsdDialogs[gmvb_OsdActiveDialog]);
   WORD W_XPos, W_YPos, W_Interval, W_ADJV_OsdPosition;
   SDWORD SDW_AdjMin, SDW_AdjMax, SDW_Pos;

   // Set the interval width
   if ((UserPrefOsdRotation == OSD_NON_ROTATED)
         ||	(UserPrefOsdRotation == OSD_ROTATED_180))
   {
      W_ADJV_OsdPosition = ADJV_OsdHPosFloat;
      W_Interval = PanelWidth - Sp_DialogCtrl->MaxXpos;
   }
   else
   {
      W_ADJV_OsdPosition = ADJV_OsdVPosFloat;
      W_Interval = PanelWidth - Sp_DialogCtrl->MaxYpos;
   }

   if (W_Interval > PanelWidth) //checks for overflow **caused if screen size is too small
   {
      W_Interval = 0;
   }

   SDW_AdjMin = gm_GetAdjusterMin(W_ADJV_OsdPosition);
   SDW_AdjMax = gm_GetAdjusterMax(W_ADJV_OsdPosition);
   SDW_Pos = gm_GetAdjuster(W_ADJV_OsdPosition);

   // Rescale the adjustor to the full screen size.
   W_XPos = (WORD)(((SDW_Pos - SDW_AdjMin ) * W_Interval) / ( SDW_AdjMax - SDW_AdjMin ));

   if ((UserPrefOsdRotation == OSD_NON_ROTATED)
         ||	(UserPrefOsdRotation == OSD_ROTATED_180))
   {
      W_ADJV_OsdPosition = ADJV_OsdVPosFloat;
      W_Interval = PanelHeight - Sp_DialogCtrl->MaxYpos;
   }
   else
   {
      W_ADJV_OsdPosition = ADJV_OsdHPosFloat;
      W_Interval = PanelHeight - Sp_DialogCtrl->MaxXpos;

   }

   if (W_Interval > PanelHeight) //checks for overflow **caused if screen size is too small
   {
      W_Interval = 0;
   }

   SDW_AdjMin = gm_GetAdjusterMin(W_ADJV_OsdPosition);
   SDW_AdjMax = gm_GetAdjusterMax(W_ADJV_OsdPosition);
   SDW_Pos = gm_GetAdjuster(W_ADJV_OsdPosition);

   // Rescale the adjustor to the full screen size.
   W_YPos = (WORD)((( SDW_Pos - SDW_AdjMin ) * W_Interval) / ( SDW_AdjMax - SDW_AdjMin ));

   if ((UserPrefOsdRotation == OSD_NON_ROTATED)
         ||	(UserPrefOsdRotation == OSD_ROTATED_180))
   {
      gm_OsdSetTilePosition(W_TileId, W_TileHpos + W_XPos, W_TileVpos + W_YPos);
   }
   else
   {
		//gm_OsdSetTilePosition(W_TileId, W_TileHpos + W_XPos,  W_TileVpos + W_YPos); // KevinL 20090310 W_TileHpos - W_XPos will get negative incorrect H position		   
		//gm_Print(" W_TileHpos = %d",W_TileHpos);
		//gm_Print(" W_TileVpos = %d",W_TileVpos);
		//gm_Print(" W_XPos = %d",W_XPos);
		//gm_Print(" W_YPos = %d",W_YPos);
		gm_OsdSetTilePosition(W_TileId, W_TileHpos + W_XPos,  W_TileVpos - W_YPos); // KevinL 20090310 W_TileHpos - W_XPos will get negative incorrect H position		   
		//gm_OsdSetTilePosition(W_TileId, PanelHeight/2 ,  PanelWidth/2); // KevinL 20090310 W_TileHpos - W_XPos will get negative incorrect H position		   
	}
}


//******************************************************************************
//
// FUNCTION       : void AdjustFlyingOsdHPosition(void)
//
// USAGE          : Adjuster wrapper for OSD horizontal position
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : gmv_OsdTilePosition	(R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AdjustFlyingOsdHPosition(void)
{
   WORD i;
   WORD W_flag = 1;

   while (1)
   {
      for (i = 0 ; i < MAX_NUMBER_OF_TILES ; i++)
		{
			if(gmv_OsdTilePosition[i].W_XPos != UNKNOWN_TILE_DIALOG_POSITION)
			{
				SetOsdTilePosition_Flying(i, gmv_OsdTilePosition[i].W_XPos, gmv_OsdTilePosition[i].W_YPos);
			}
		}

		if(W_flag)
		{
			ReverseTileSet();
			W_flag = 0;
		}
		else
		{
			break;
		}
	}
}
//******************************************************************************
//
// FUNCTION       : void AdjustFlyingOsdVPosition(void)
//
// USAGE          : Adjuster wrapper for OSD vertical position
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : gmv_OsdTilePosition		(R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AdjustFlyingOsdVPosition(void)
{
   WORD i;
   WORD W_flag = 1;

   while (1)
   {
      for (i = 0 ; i < MAX_NUMBER_OF_TILES ; i++)
      {
         if (gmv_OsdTilePosition[i].W_YPos != UNKNOWN_TILE_DIALOG_POSITION)
         {
            SetOsdTilePosition_Flying(i, gmv_OsdTilePosition[i].W_XPos,
                               gmv_OsdTilePosition[i].W_YPos);
         }
      }

      if (W_flag)
      {
         ReverseTileSet();
         W_flag = 0;
      }
      else
      {
         break;
      }
   }

}


void ZDT_4K2K_OutputConfig(void)
{
#if ((DEFAULT_PANEL == ZDT_4K2K_CMI)||(DEFAULT_PANEL == ZDT_4K2K_CMI_39)||(DEFAULT_PANEL == ZDT_4K2K_CMI_65)||(DEFAULT_PANEL == ZDT_4K2K_CMI_50))
	{
		//static BYTE PanelMode = 00;	//Default is 3840x2160
		if((PanelWidth == 3840))//&&(PanelMode !=  0x00))
		{
			//PanelMode = 0x00;
		#ifdef UART_DEBUG_MODE
			gm_Print("xxxxxxxxxxxxxx 4K2K@30 xxxxxxxx",0);
		#endif
			#if((DEFAULT_PANEL == ZDT_4K2K_CMI)||(DEFAULT_PANEL == ZDT_4K2K_CMI_39))//||(DEFAULT_PANEL == ZDT_4K2K_CMI_65))
				ReConfigLVDSSetting(UserPrefPanelSelect,PanelWidth);
			#endif
			ZDT_4K2K_CMI_Panel_QFHD30(UserPrefPanelSelect,0);
		}
		else if((PanelWidth == 1920))//&&(PanelMode !=	0x01))
		{
			//PanelMode = 0x01;
		#ifdef UART_DEBUG_MODE
			gm_Print("xxxxxxxxxxxxxx 1080p@30 xxxxxxxx",0);
		#endif
		#if((DEFAULT_PANEL == ZDT_4K2K_CMI)||(DEFAULT_PANEL == ZDT_4K2K_CMI_39))//||(DEFAULT_PANEL == ZDT_4K2K_CMI_65))
				ReConfigLVDSSetting(UserPrefPanelSelect,PanelWidth);
		#endif
			ZDT_4K2K_CMI_Panel_FHD60(UserPrefPanelSelect,0);
		}
	}
#endif

}


void TurnBacklightOn(void)
{

#ifdef MODE_CHANGE_BACKLIGHT_OFF
	if((gm_ReadRegWord(DISPLAY_CONTROL)&DTG_RUN_EN) == 0)
		gm_SetRegBitsWord(DISPLAY_CONTROL,DTG_RUN_EN);
#endif

}
