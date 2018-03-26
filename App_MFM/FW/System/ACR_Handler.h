/*
	$Workfile:   ACR_Handler.h  $
	$Revision: 1.8 $
	$Date: 2013/02/05 07:12:46 $
*/

#ifndef __ACR_Handler_H__
#define __ACR_Handler_H__

//*****************************************************************************************
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
//
//	Adaptive Contrast Ratio
// Default: Get Luminance data from Pixel grab.
//	Second solution: Use ACC, if pixel grab used for VWD.
//	Limitation: If system can not use ACC and pixel grab already used for VWD, it will no ACR anymore.
//
//*****************************************************************************************

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************
#define INVALID_RESULT           0xFFFF

//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************
//120516 Edward disable , stub.h already defined
/*#define DEFAULT_VIDEO_BACKLIGHT_PWM	MAX_PWMPERIOD
#if defined(Qisda_93XX_BOARD)
#define DEFAULT_BACKLIGHT_PWM		225
#define MAX_PWMPERIOD					0x68C
#define MAX_BACKLIGHT_PWM				0x68C
#define MIN_BACKLIGHT_PWM				0x100
#else
#define DEFAULT_BACKLIGHT_PWM		225 // 155
#define MAX_PWMPERIOD					0x6A
#define MAX_BACKLIGHT_PWM				0x6A // 250		
#define MIN_BACKLIGHT_PWM				0x1A //0x20 
#endif*/

//120711 Edward add for ACC average brightness data issue at full white pattern 
#define ACCHighBinAddr		0x32EE8L
#define ACCHistogramLimit	0x22D580  //experience value


#if 0 //KevinL 20120606 disable it for DELL DCR, already defined in stub.h
#define DCR_READ_BRIGHTNESS_USE_ACC3		// 

#define DCR_NOISE_THRESHOLD			1	// 1: 0x37, 2: 0x6E, 3: 0xA6....

#define HIGH_DCR_ACTIVATION_TIME				10	// 10 seconds / 1s
#define HIGH_DCR_CONTINUOUS_LIMIT_TIME		60	// 1 minute / 1s
#define HIGH_DCR_PANEL_WARM_UP_TIME			600	// 10 minutes / 1s

#define DCR_NORMAL_PWM_PULSE_MAX	MAX_BACKLIGHT_PWM
#define DCR_NORMAL_PWM_PULSE_MIN	MIN_BACKLIGHT_PWM
#define DCR_HIGH_PWM_PULSE_MAX		DCR_NORMAL_PWM_PULSE_MAX
#define DCR_HIGH_PWM_PULSE_MIN		0x05//Calo 20090312* Org = 0x05//0x0C

#define DCR_PWM_PULSE_MAX		((HighDcrActivationProcedure == HighDCR_CONTINUOUS)? (DCR_HIGH_PWM_PULSE_MAX): (DCR_NORMAL_PWM_PULSE_MAX))
#define DCR_PWM_PULSE_MIN		((HighDcrActivationProcedure == HighDCR_CONTINUOUS)? (DCR_HIGH_PWM_PULSE_MIN): (DCR_NORMAL_PWM_PULSE_MIN))
#define DCR_PWM_PULSE_RANGE	(DCR_PWM_PULSE_MAX - DCR_PWM_PULSE_MIN)

#define IMP_BRIGHTNESS_MAX	241
#define IMP_BRIGHTNESS_MIN		32
#define IMP_BRIGHTNESS_RANGE	(IMP_BRIGHTNESS_MAX - IMP_BRIGHTNESS_MIN)

#define HIGH_DCR_GRAY_LEVEL_UNDER	(IMP_BRIGHTNESS_RANGE * 0x4 / 0xFF + IMP_BRIGHTNESS_MIN)

#define ConvertImpBrightnessToPwmPulse(W_Imp_Brightness) \
		((BYTE)((((DWORD)(W_Imp_Brightness - IMP_BRIGHTNESS_MIN) * DCR_PWM_PULSE_RANGE) / IMP_BRIGHTNESS_RANGE) + DCR_PWM_PULSE_MIN))
#endif
//110830 Edward disable already define at other file
/*#if defined(ALL_BOARDS_HDMI3)
	#define IsVideoInput(x) 				((x)==ALL_BOARDS_CVBS1 || (x)==ALL_BOARDS_SVIDEO1 || (x)==ALL_BOARDS_COMP1 || (x)==ALL_BOARDS_DIP1 || (x)==ALL_BOARDS_HDMI || (x)==ALL_BOARDS_HDMI2 || (x)==ALL_BOARDS_HDMI3 )
#elif defined(ALL_BOARDS_HDMI2)
	#define IsVideoInput(x) 				((x)==ALL_BOARDS_CVBS1 || (x)==ALL_BOARDS_SVIDEO1 || (x)==ALL_BOARDS_COMP1 || (x)==ALL_BOARDS_DIP1 || (x)==ALL_BOARDS_HDMI || (x)==ALL_BOARDS_HDMI2)
#elif defined(ALL_BOARDS_HDMI)
	#define	IsVideoInput(x)                 ((x)==ALL_BOARDS_CVBS1 || (x)==ALL_BOARDS_SVIDEO1 || (x)==ALL_BOARDS_COMP1 || (x)==ALL_BOARDS_DIP1 || (x)==ALL_BOARDS_HDMI)
#else
	#define IsVideoInput(x) 				((x)==ALL_BOARDS_CVBS1 || (x)==ALL_BOARDS_SVIDEO1 || (x)==ALL_BOARDS_COMP1 || (x)==ALL_BOARDS_DIP1)
#endif*/

//110830 Edward disable already define at other file
//#define IsPresetMode_DCR_ON()	(IsVideoInput(UserPrefCurrentInputMain)) //((IsPresetMode_VIDEO_MOVIE || IsPresetMode_VIDEO_GAME || IsPresetMode_VIDEO_NATURE || IsPresetMode_GRAPHICS_GAME))

//#define IsBrightnessAdjustBacklight()	(IsGraphicInput(UserPrefCurrentInputMain))

//#define IS_DCR_ON()				((UserPrefDCR == On) && IsPresetMode_DCR_ON() && (UserPrefPipMode == NO_PIP)) 
//#define IS_OSD_DCR_ENABLE()	(IsPresetMode_DCR_ON() && (UserPrefPipMode == NO_PIP))

#define IsBrightnessItemGreyout	FALSE 

//110830 Edward disable already define at other file
/*
#define BACKLIGHT_PWM_CONFIG			PWM3_CONFIG
#define BACKLIGHT_PWM_CTRL				PWM2_CTRL
#define ReadBacklightPWMPulse()			((BYTE)(gm_ReadRegWord(BACKLIGHT_PWM_CTRL) >> 8))
#define SetBacklightPWMPulse(pulse)		(gm_ClearAndSetBitsWord(BACKLIGHT_PWM_CTRL, PWM3_PULSE, (WORD)pulse << 8))


typedef	enum HighDcrActivationProcedureEnum
{
	HighDCR_STRANDBY,		// 0
	HighDCR_ACTIVATION,	// 1
	HighDCR_CONTINUOUS,	// 2
	HighDCR_LIMIT,			// 3
} HighDcrActivationProcedureType;
*/
typedef enum AcrHandlerStateEnum
{
   ACR_CALCULATE_PWM,
   ACR_ADJUST_PWM,
   ACR_NO_ACTION
} AcrHandlerState_t;

//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
WORD GetCurrentAcrResult(void);
void ACR_Init(void);
void ACR_Enable(void);
void ACR_Disable(void);
void ACR_Handler(void);

#endif

