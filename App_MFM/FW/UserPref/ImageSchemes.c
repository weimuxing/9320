/*
	$Workfile:   ImageSchemes.c  $
	$Revision: 1.10 $
	$Date: 2012/04/19 03:18:38 $
*/
#define __IMAGE_SCHEMES_C__

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
// MODULE:      adj_display.c
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "System\all.h"

//*****************************************************************************
// G L O B A L S
//*****************************************************************************

#ifdef DELL_U2410F
PortSchemeDependentUserPrefType ROM SchemeFactoryDefaultInROM[NUM_IMAGE_SCHEMES] =
{
   {	// 0 Normal
      0,	// Acm3BlueLuma
      0,	// Acm3CyanLuma
      0,	// Acm3GreenLuma
      0,	// Acm3MagentaLuma
      0,	// Acm3RedLuma
      0,	// Acm3YellowLuma
      0,	// Acm3BlackLuma
      0,	// Acm3WhiteLuma
      
      0,	// Acm3BlueHue
      0,	// Acm3CyanHue
      0,	// Acm3GreenHue
      0,	// Acm3MagentaHue
      0,	// Acm3RedHue
      0,	// Acm3YellowHue
      0, // Acm3BlackHue
      0, // Acm3WhiteHue
 	50,	// Acm3BlueSat
 	50,	// Acm3CyanSat
 	50,	// Acm3GreenSat
 	50,	// Acm3MagentaSat
 	50,	// Acm3RedSat
 	50,	// Acm3YellowSat
 	0,	// Acm3BlackSat
 	0,	// Acm3WhiteSat
 	WB_BRIGHTNESS_DEFAULT,	// Brightness
 	WB_BRIGHTNESS_DEFAULT,	// Brightness_All
 	WB_BRIGHTNESS_TEXT_DEFAULT,	// Brightness_Text
 	WB_BRIGHTNESS_CAL1_DEFAULT,	// Brightness_CAL1
 	WB_BRIGHTNESS_CAL2_DEFAULT,	// Brightness_CAL2
 	WB_CONTRAST_DEFAULT,	// Contrast
 	WB_CONTRAST_DEFAULT,	// Contrast_All
 	WB_CONTRAST_TEXT_DEFAULT,	// Contrast_Text
 	WB_CONTRAST_CAL1_DEFAULT,	// Contrast_CAL1
 	WB_CONTRAST_CAL2_DEFAULT,	// Contrast_CAL2
 	WB_SATURATION_DEFAULT,	// Saturation
 	WB_SATURATION_DEFAULT,	// Saturation_Movie
 	WB_SATURATION_DEFAULT,	// Saturation_Game
 	0,	// Hue
 	0,	// Hue_Movie
 	0,	// Hue_Game
 	USER_GAIN_DEF,	// UserRedGain
 	USER_GAIN_DEF,	// UserGreenGain
 	USER_GAIN_DEF,	// UserBlueGain
 	0,	// UserRedOff
 	0,	// UserGreenOff
 	0,	// UserBlueOff
 	CTEMP_6500K,	// ColorTemp
 	GAMMA_TABLE_OFF,	// GammaTable 'Desc' :Varaible to choose Gamma Table
 	255,	// UserRed
 	255,	// UserGreen
 	255,	// UserBlue
 	Input_RGB,	// ColorFormat
 	Video_MOVIE,	// VideoImageMode
 	Graphics_STANDARD,	// GraphicsImageMode
 	CTEMP_6500K,	// PresetColorTemp
 	WB_CSPACE_DEFAULT,	//CSPACE_AdobeRGB,	// PresetColorSpace	//20121023 Kordonm+ for VGA port image scheme data save
 	ColorMode_GRAPHIC,	// ColorMode
 	WB_SHARPNESS_DEFAULT,	// Sharpness
 	WB_SHARPNESS_DEFAULT,	// Sharpness_All
 	WB_SHARPNESS_DEFAULT,	// Sharpness_Text
 	0,	// ImageZoom
 	0,	// AspectRatio

	#if(BOARD_USED == DELL_93xx_BOARD)
 	{
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ On, UNIFORMITY_OFF },
		{ On, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF }
	},
	#else
	{
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF }
	},	
	#endif
	0,	// ResponseTime
 	Gamma_PC,	// GammaMode
 	WB_SATURATION_YPbPr_DEFAULT,	// Saturation_YPbPr
 	PRINTER_PREVUE1,	// PresetPrinterPrevue
 	TNR_OFF,	// PresetPrinterPrevue
 	0,	//AcmRedHue
 	0,	//AcmYellowHue
 	0,	//AcmBlueSat
 	0,	//AcmCyanSat
 	0,	//AcmGreenSat
 	0,	//AcmMagentSat
 	0,	//AcmRedSat
 	0,	//AcmYellowSat
 	VIVID,	//MFMImageScheme
 	0,	//InputColorDomain
 	0,	//OutputColorType
 	MPEG_NR_OFF,
 },
		
 { // 1 INPUT_DP
	0,	// Acm3BlueLuma
 	0,	// Acm3CyanLuma
 	0,	// Acm3GreenLuma
 	0,	// Acm3MagentaLuma
 	0,	// Acm3RedLuma
 	0,	// Acm3YellowLuma
 	0,	// Acm3BlackLuma
 	0,	// Acm3WhiteLuma
 	0,	// Acm3BlueHue
 	0,	// Acm3CyanHue
 	0,	// Acm3GreenHue
 	0,	// Acm3MagentaHue
 	0,	// Acm3RedHue
 	0,	// Acm3YellowHue
 	0,	// Acm3BlackHue
 	0,	// Acm3WhiteHue
 	50,	// Acm3BlueSat
 	50,	// Acm3CyanSat
 	50,	// Acm3GreenSat
 	50,	// Acm3MagentaSat
 	50,	// Acm3RedSat
 	50,	// Acm3YellowSat
 	0,	// Acm3BlackSat
 	0,	// Acm3WhiteSat
 	WB_BRIGHTNESS_DEFAULT,	// Brightness
 	WB_BRIGHTNESS_DEFAULT,	// Brightness_All
 	WB_BRIGHTNESS_TEXT_DEFAULT,	// Brightness_Text
 	WB_BRIGHTNESS_CAL1_DEFAULT,	// Brightness_CAL1
 	WB_BRIGHTNESS_CAL2_DEFAULT,	// Brightness_CAL2
 	WB_CONTRAST_DEFAULT,	// Contrast
 	WB_CONTRAST_DEFAULT,	// Contrast_All
 	WB_CONTRAST_TEXT_DEFAULT,	// Contrast_Text
 	WB_CONTRAST_CAL1_DEFAULT,	// Contrast_CAL1
 	WB_CONTRAST_CAL2_DEFAULT,	// Contrast_CAL2
 	WB_SATURATION_DEFAULT,	// Saturation
 	WB_SATURATION_DEFAULT,	// Saturation_Movie
 	WB_SATURATION_DEFAULT,	// Saturation_Game
 	0,	// Hue
 	0,	// Hue_Movie
 	0,	// Hue_Game
 	USER_GAIN_DEF,	// UserRedGain
 	USER_GAIN_DEF,	// UserGreenGain
 	USER_GAIN_DEF,	// UserBlueGain
 	0,	// UserRedOff
 	0,	// UserGreenOff
 	0,	// UserBlueOff
 	CTEMP_6500K,	// ColorTemp
 	GAMMA_TABLE_OFF,	// GammaTable 'Desc' :Varaible to choose Gamma Table
 	255,	// UserRed
 	255,	// UserGreen
 	255,	// UserBlue
 	Input_RGB,	// ColorFormat
 	Video_MOVIE,	// VideoImageMode
 	Graphics_STANDARD,	// GraphicsImageMode
 	CTEMP_6500K,	// PresetColorTemp
 	WB_CSPACE_DEFAULT,	//CSPACE_AdobeRGB,	// PresetColorSpace	//20121023 Kordonm+ for VGA port image scheme data save
 	ColorMode_GRAPHIC,	// ColorMode
 	WB_SHARPNESS_DEFAULT,	// Sharpness
 	WB_SHARPNESS_DEFAULT,	// Sharpness_All
 	WB_SHARPNESS_DEFAULT,	// Sharpness_Text
 	0,	// ImageZoom
 	0,	// AspectRatio

	#if(BOARD_USED == DELL_93xx_BOARD)
 	{
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ On, UNIFORMITY_OFF },
		{ On, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF }
	},
	#else
	{
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF }
	},	
	#endif

	0,	// ResponseTime
 	Gamma_PC,	// GammaMode
 	WB_SATURATION_YPbPr_DEFAULT,	// Saturation_YPbPr
 	PRINTER_PREVUE1,	// PresetPrinterPrevue
 	TNR_OFF,	// PresetPrinterPrevue
 	0,	//AcmRedHue
 	0,	//AcmYellowHue
 	0,	//AcmBlueSat
 	0,	//AcmCyanSat
 	0,	//AcmGreenSat
 	0,	//AcmMagentSat
 	0,	//AcmRedSat
 	0,	//AcmYellowSat
 	VIVID,	//MFMImageScheme
 	0,	//InputColorDomain
 	0,	//OutputColorType
	MPEG_NR_OFF,
 },
		
#ifdef MUX_TEST_BOARD
  { // 2 INPUT_DP3
	0,	// Acm3BlueLuma
 	0,	// Acm3CyanLuma
 	0,	// Acm3GreenLuma
 	0,	// Acm3MagentaLuma
 	0,	// Acm3RedLuma
 	0,	// Acm3YellowLuma
 	0,	// Acm3BlackLuma
 	0,	// Acm3WhiteLuma
 	0,	// Acm3BlueHue
 	0,	// Acm3CyanHue
 	0,	// Acm3GreenHue
 	0,	// Acm3MagentaHue
 	0,	// Acm3RedHue
 	0,	// Acm3YellowHue
 	0,	// Acm3BlackHue
 	0,	// Acm3WhiteHue
 	50,	// Acm3BlueSat
 	50,	// Acm3CyanSat
 	50,	// Acm3GreenSat
 	50,	// Acm3MagentaSat
 	50,	// Acm3RedSat
 	50,	// Acm3YellowSat
 	0,	// Acm3BlackSat
 	0,	// Acm3WhiteSat
 	WB_BRIGHTNESS_DEFAULT,	// Brightness
 	WB_BRIGHTNESS_DEFAULT,	// Brightness_All
 	WB_BRIGHTNESS_TEXT_DEFAULT,	// Brightness_Text
 	WB_BRIGHTNESS_CAL1_DEFAULT,	// Brightness_CAL1
 	WB_BRIGHTNESS_CAL2_DEFAULT,	// Brightness_CAL2
 	WB_CONTRAST_DEFAULT,	// Contrast
 	WB_CONTRAST_DEFAULT,	// Contrast_All
 	WB_CONTRAST_TEXT_DEFAULT,	// Contrast_Text
 	WB_CONTRAST_CAL1_DEFAULT,	// Contrast_CAL1
 	WB_CONTRAST_CAL2_DEFAULT,	// Contrast_CAL2
 	WB_SATURATION_DEFAULT,	// Saturation
 	WB_SATURATION_DEFAULT,	// Saturation_Movie
 	WB_SATURATION_DEFAULT,	// Saturation_Game
 	0,	// Hue
 	0,	// Hue_Movie
 	0,	// Hue_Game
 	USER_GAIN_DEF,	// UserRedGain
 	USER_GAIN_DEF,	// UserGreenGain
 	USER_GAIN_DEF,	// UserBlueGain
 	0,	// UserRedOff
 	0,	// UserGreenOff
 	0,	// UserBlueOff
 	CTEMP_6500K,	// ColorTemp
 	GAMMA_TABLE_OFF,	// GammaTable 'Desc' :Varaible to choose Gamma Table
 	255,	// UserRed
 	255,	// UserGreen
 	255,	// UserBlue
 	Input_RGB,	// ColorFormat
 	Video_MOVIE,	// VideoImageMode
 	Graphics_STANDARD,	// GraphicsImageMode
 	CTEMP_6500K,	// PresetColorTemp
 	WB_CSPACE_DEFAULT,	//CSPACE_AdobeRGB,	// PresetColorSpace	//20121023 Kordonm+ for VGA port image scheme data save
 	ColorMode_GRAPHIC,	// ColorMode
 	WB_SHARPNESS_DEFAULT,	// Sharpness
 	WB_SHARPNESS_DEFAULT,	// Sharpness_All
 	WB_SHARPNESS_DEFAULT,	// Sharpness_Text
 	0,	// ImageZoom
 	0,	// AspectRatio

	#if(BOARD_USED == DELL_93xx_BOARD)
 	{
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ On, UNIFORMITY_OFF },
		{ On, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF }
	},
	#else
	{
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF }
	},	
	#endif
	0,	// ResponseTime
 	Gamma_PC,	// GammaMode
 	WB_SATURATION_YPbPr_DEFAULT,	// Saturation_YPbPr
 	PRINTER_PREVUE1,	// PresetPrinterPrevue
 	TNR_OFF,	// PresetPrinterPrevue
 	0,	//AcmRedHue
 	0,	//AcmYellowHue
 	0,	//AcmBlueSat
 	0,	//AcmCyanSat
 	0,	//AcmGreenSat
 	0,	//AcmMagentSat
 	0,	//AcmRedSat
 	0,	//AcmYellowSat
 	VIVID,	//MFMImageScheme
 	0,	//InputColorDomain
 	0,	//OutputColorType
	MPEG_NR_OFF,
  },
#endif	
  {// 3 INPUT_HDMI
	0,	// Acm3BlueLuma
 	0,	// Acm3CyanLuma
 	0,	// Acm3GreenLuma
 	0,	// Acm3MagentaLuma
 	0,	// Acm3RedLuma
 	0,	// Acm3YellowLuma
 	0,	// Acm3BlackLuma
 	0,	// Acm3WhiteLuma
 	0,	// Acm3BlueHue
 	0,	// Acm3CyanHue
 	0,	// Acm3GreenHue
 	0,	// Acm3MagentaHue
 	0,	// Acm3RedHue
 	0,	// Acm3YellowHue
 	0,	// Acm3BlackHue
 	0,	// Acm3WhiteHue
 	50,	// Acm3BlueSat
 	50,	// Acm3CyanSat
 	50,	// Acm3GreenSat
 	50,	// Acm3MagentaSat
 	50,	// Acm3RedSat
 	50,	// Acm3YellowSat
 	0,	// Acm3BlackSat
 	0,	// Acm3WhiteSat
 	WB_BRIGHTNESS_DEFAULT,	// Brightness
 	WB_BRIGHTNESS_DEFAULT,	// Brightness_All
 	WB_BRIGHTNESS_TEXT_DEFAULT,	// Brightness_Text
 	WB_BRIGHTNESS_CAL1_DEFAULT,	// Brightness_CAL1
 	WB_BRIGHTNESS_CAL2_DEFAULT,	// Brightness_CAL2
 	WB_CONTRAST_DEFAULT,	// Contrast
 	WB_CONTRAST_DEFAULT,	// Contrast_All
 	WB_CONTRAST_TEXT_DEFAULT,	// Contrast_Text
 	WB_CONTRAST_CAL1_DEFAULT,	// Contrast_CAL1
 	WB_CONTRAST_CAL2_DEFAULT,	// Contrast_CAL2
 	WB_SATURATION_DEFAULT,	// Saturation
 	WB_SATURATION_DEFAULT,	// Saturation_Movie
 	WB_SATURATION_DEFAULT,	// Saturation_Game
 	0,	// Hue
 	0,	// Hue_Movie
 	0,	// Hue_Game
 	USER_GAIN_DEF,	// UserRedGain
 	USER_GAIN_DEF,	// UserGreenGain
 	USER_GAIN_DEF,	// UserBlueGain
 	0,	// UserRedOff
 	0,	// UserGreenOff
 	0,	// UserBlueOff
 	CTEMP_6500K,	// ColorTemp
 	GAMMA_TABLE_OFF,	// GammaTable 'Desc' :Varaible to choose Gamma Table
 	255,	// UserRed
 	255,	// UserGreen
 	255,	// UserBlue
 	Input_RGB,	// ColorFormat
 	Video_MOVIE,	// VideoImageMode
 	Graphics_STANDARD,	// GraphicsImageMode
 	CTEMP_6500K,	// PresetColorTemp
 	WB_CSPACE_DEFAULT,	//CSPACE_AdobeRGB,	// PresetColorSpace	//20121023 Kordonm+ for VGA port image scheme data save
 	ColorMode_GRAPHIC,	// ColorMode
 	WB_SHARPNESS_DEFAULT,	// Sharpness
 	WB_SHARPNESS_DEFAULT,	// Sharpness_All
 	WB_SHARPNESS_DEFAULT,	// Sharpness_Text
 	0,	// ImageZoom
 	0,	// AspectRatio

	#if(BOARD_USED == DELL_93xx_BOARD)
 	{
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ On, UNIFORMITY_OFF },
		{ On, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF }
	},
	#else
	{
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF }
	},	
	#endif
	0,	// ResponseTime
 	Gamma_PC,	// GammaMode
 	WB_SATURATION_YPbPr_DEFAULT,	// Saturation_YPbPr
 	PRINTER_PREVUE1,	// PresetPrinterPrevue
 	TNR_OFF,	// PresetPrinterPrevue
 	0,	//AcmRedHue
 	0,	//AcmYellowHue
 	0,	//AcmBlueSat
 	0,	//AcmCyanSat
 	0,	//AcmGreenSat
 	0,	//AcmMagentSat
 	0,	//AcmRedSat
 	0,	//AcmYellowSat
 	VIVID,	//MFMImageScheme
 	0,	//InputColorDomain
 	0,	//OutputColorType
	MPEG_NR_OFF,
 },	

{// 4 INPUT_VGA1
	0,	// Acm3BlueLuma
 	0,	// Acm3CyanLuma
 	0,	// Acm3GreenLuma
 	0,	// Acm3MagentaLuma
 	0,	// Acm3RedLuma
 	0,	// Acm3YellowLuma
 	0,	// Acm3BlackLuma
 	0,	// Acm3WhiteLuma
 	0,	// Acm3BlueHue
 	0,	// Acm3CyanHue
 	0,	// Acm3GreenHue
 	0,	// Acm3MagentaHue
 	0,	// Acm3RedHue
 	0,	// Acm3YellowHue
 	0,	// Acm3BlackHue
 	0,	// Acm3WhiteHue
 	50,	// Acm3BlueSat
 	50,	// Acm3CyanSat
 	50,	// Acm3GreenSat
 	50,	// Acm3MagentaSat
 	50,	// Acm3RedSat
 	50,	// Acm3YellowSat
 	0,	// Acm3BlackSat
 	0,	// Acm3WhiteSat
 	WB_BRIGHTNESS_DEFAULT,	// Brightness
 	WB_BRIGHTNESS_DEFAULT,	// Brightness_All
 	WB_BRIGHTNESS_TEXT_DEFAULT,	// Brightness_Text
 	WB_BRIGHTNESS_CAL1_DEFAULT,	// Brightness_CAL1
 	WB_BRIGHTNESS_CAL2_DEFAULT,	// Brightness_CAL2
 	WB_CONTRAST_DEFAULT,	// Contrast
 	WB_CONTRAST_DEFAULT,	// Contrast_All
 	WB_CONTRAST_TEXT_DEFAULT,	// Contrast_Text
 	WB_CONTRAST_CAL1_DEFAULT,	// Contrast_CAL1
 	WB_CONTRAST_CAL2_DEFAULT,	// Contrast_CAL2
 	WB_SATURATION_DEFAULT,	// Saturation
 	WB_SATURATION_DEFAULT,	// Saturation_Movie
 	WB_SATURATION_DEFAULT,	// Saturation_Game
 	0,	// Hue
 	0,	// Hue_Movie
 	0,	// Hue_Game
 	USER_GAIN_DEF,	// UserRedGain
 	USER_GAIN_DEF,	// UserGreenGain
 	USER_GAIN_DEF,	// UserBlueGain
 	0,	// UserRedOff
 	0,	// UserGreenOff
 	0,	// UserBlueOff
 	CTEMP_6500K,	// ColorTemp
 	GAMMA_TABLE_OFF,	// GammaTable 'Desc' :Varaible to choose Gamma Table
 	255,	// UserRed
 	255,	// UserGreen
 	255,	// UserBlue
 	Input_RGB,	// ColorFormat
 	Video_MOVIE,	// VideoImageMode
 	Graphics_STANDARD,	// GraphicsImageMode
 	CTEMP_6500K,	// PresetColorTemp
 	WB_CSPACE_DEFAULT,	// PresetColorSpace
 	ColorMode_GRAPHIC,	// ColorMode
 	WB_SHARPNESS_DEFAULT,	// Sharpness
 	WB_SHARPNESS_DEFAULT,	// Sharpness_All
 	WB_SHARPNESS_DEFAULT,	// Sharpness_Text
 	0,	// ImageZoom
 	0,	// AspectRatio

	#if(BOARD_USED == DELL_93xx_BOARD)
 	{
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ On, UNIFORMITY_OFF },
		{ On, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF }
	},
	#else
	{
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF }
	},	
	#endif
	0,	// ResponseTime
 	Gamma_PC,	// GammaMode
 	WB_SATURATION_YPbPr_DEFAULT,	// Saturation_YPbPr
 	PRINTER_PREVUE1,	// PresetPrinterPrevue
 	TNR_LOW,	// PresetPrinterPrevue
 	0,	//AcmRedHue
 	0,	//AcmYellowHue
 	0,	//AcmBlueSat
 	0,	//AcmCyanSat
 	0,	//AcmGreenSat
 	0,	//AcmMagentSat
 	0,	//AcmRedSat
 	0,	//AcmYellowSat
 	VIVID,	//MFMImageScheme
 	0,	//InputColorDomain
 	0,	//OutputColorType
	MPEG_NR_OFF,
 },

#ifdef EXTERNAL_DECODER

{// 5 INPUT_CVBS1
	0,	// Acm3BlueLuma
 	0,	// Acm3CyanLuma
 	0,	// Acm3GreenLuma
 	0,	// Acm3MagentaLuma
 	0,	// Acm3RedLuma
 	0,	// Acm3YellowLuma
 	0,	// Acm3BlackLuma
 	0,	// Acm3WhiteLuma
 	0,	// Acm3BlueHue
 	0,	// Acm3CyanHue
 	0,	// Acm3GreenHue
 	0,	// Acm3MagentaHue
 	0,	// Acm3RedHue
 	0,	// Acm3YellowHue
 	0,	// Acm3BlackHue
 	0,	// Acm3WhiteHue
 	50,	// Acm3BlueSat
 	50,	// Acm3CyanSat
 	50,	// Acm3GreenSat
 	50,	// Acm3MagentaSat
 	50,	// Acm3RedSat
 	50,	// Acm3YellowSat
 	0,	// Acm3BlackSat
 	0,	// Acm3WhiteSat
 	WB_BRIGHTNESS_DEFAULT,	// Brightness
 	WB_BRIGHTNESS_DEFAULT,	// Brightness_All
 	WB_BRIGHTNESS_TEXT_DEFAULT,	// Brightness_Text
 	WB_BRIGHTNESS_CAL1_DEFAULT,	// Brightness_CAL1
 	WB_BRIGHTNESS_CAL2_DEFAULT,	// Brightness_CAL2
 	WB_CONTRAST_DEFAULT,	// Contrast
 	WB_CONTRAST_DEFAULT,	// Contrast_All
 	WB_CONTRAST_TEXT_DEFAULT,	// Contrast_Text
 	WB_CONTRAST_CAL1_DEFAULT,	// Contrast_CAL1
 	WB_CONTRAST_CAL2_DEFAULT,	// Contrast_CAL2
 	WB_SATURATION_DEFAULT,	// Saturation
 	WB_SATURATION_DEFAULT,	// Saturation_Movie
 	WB_SATURATION_DEFAULT,	// Saturation_Game
 	0,	// Hue
 	0,	// Hue_Movie
 	0,	// Hue_Game
 	USER_GAIN_DEF,	// UserRedGain
 	USER_GAIN_DEF,	// UserGreenGain
 	USER_GAIN_DEF,	// UserBlueGain
 	0,	// UserRedOff
 	0,	// UserGreenOff
 	0,	// UserBlueOff
 	CTEMP_6500K,	// ColorTemp
 	GAMMA_TABLE_OFF,	// GammaTable 'Desc' :Varaible to choose Gamma Table
 	255,	// UserRed
 	255,	// UserGreen
 	255,	// UserBlue
 	InputColorFormat_YUV,	// ColorFormat
 	Video_MOVIE,	// VideoImageMode
 	Graphics_STANDARD,	// GraphicsImageMode
 	CTEMP_6500K,	// PresetColorTemp
 	WB_CSPACE_DEFAULT,	// PresetColorSpace
 	ColorMode_GRAPHIC,	// ColorMode
 	WB_SHARPNESS_DEFAULT,	// Sharpness
 	WB_SHARPNESS_DEFAULT,	// Sharpness_All
 	WB_SHARPNESS_DEFAULT,	// Sharpness_Text
 	0,	// ImageZoom
 	0,	// AspectRatio

	#if(BOARD_USED == DELL_93xx_BOARD)
 	{
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ On, UNIFORMITY_OFF },
		{ On, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF }
	},
	#else
	{
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF }
	},	
	#endif
	0,	// ResponseTime
 	Gamma_PC,	// GammaMode
 	WB_SATURATION_YPbPr_DEFAULT,	// Saturation_YPbPr
 	PRINTER_PREVUE1,	// PresetPrinterPrevue
 	TNR_HIGH,	// PresetPrinterPrevue
 	0,	//AcmRedHue
 	0,	//AcmYellowHue
 	0,	//AcmBlueSat
 	0,	//AcmCyanSat
 	0,	//AcmGreenSat
 	0,	//AcmMagentSat
 	0,	//AcmRedSat
 	0,	//AcmYellowSat
 	VIVID,	//MFMImageScheme
 	0,	//InputColorDomain
 	0,	//OutputColorType
	MPEG_NR_GAUSS,
 },
#endif	
#if(ON_BOARD_COMP == ENABLE)
{// 6 INPUT_COMP		on board YPbPr
	0,	// Acm3BlueLuma
 	0,	// Acm3CyanLuma
 	0,	// Acm3GreenLuma
 	0,	// Acm3MagentaLuma
 	0,	// Acm3RedLuma
 	0,	// Acm3YellowLuma
 	0,	// Acm3BlackLuma
 	0,	// Acm3WhiteLuma
 	0,	// Acm3BlueHue
 	0,	// Acm3CyanHue
 	0,	// Acm3GreenHue
 	0,	// Acm3MagentaHue
 	0,	// Acm3RedHue
 	0,	// Acm3YellowHue
 	0,	// Acm3BlackHue
 	0,	// Acm3WhiteHue
 	50,	// Acm3BlueSat
 	50,	// Acm3CyanSat
 	50,	// Acm3GreenSat
 	50,	// Acm3MagentaSat
 	50,	// Acm3RedSat
 	50,	// Acm3YellowSat
 	0,	// Acm3BlackSat
 	0,	// Acm3WhiteSat
 	WB_BRIGHTNESS_DEFAULT,	// Brightness
 	WB_BRIGHTNESS_DEFAULT,	// Brightness_All
 	WB_BRIGHTNESS_TEXT_DEFAULT,	// Brightness_Text
 	WB_BRIGHTNESS_CAL1_DEFAULT,	// Brightness_CAL1
 	WB_BRIGHTNESS_CAL2_DEFAULT,	// Brightness_CAL2
 	WB_CONTRAST_DEFAULT,	// Contrast
 	WB_CONTRAST_DEFAULT,	// Contrast_All
 	WB_CONTRAST_TEXT_DEFAULT,	// Contrast_Text
 	WB_CONTRAST_CAL1_DEFAULT,	// Contrast_CAL1
 	WB_CONTRAST_CAL2_DEFAULT,	// Contrast_CAL2
 	WB_SATURATION_DEFAULT,	// Saturation
 	WB_SATURATION_DEFAULT,	// Saturation_Movie
 	WB_SATURATION_DEFAULT,	// Saturation_Game
 	0,	// Hue
 	0,	// Hue_Movie
 	0,	// Hue_Game
 	USER_GAIN_DEF,	// UserRedGain
 	USER_GAIN_DEF,	// UserGreenGain
 	USER_GAIN_DEF,	// UserBlueGain
 	0,	// UserRedOff
 	0,	// UserGreenOff
 	0,	// UserBlueOff
 	CTEMP_6500K,	// ColorTemp
 	GAMMA_TABLE_OFF,	// GammaTable 'Desc' :Varaible to choose Gamma Table
 	255,	// UserRed
 	255,	// UserGreen
 	255,	// UserBlue
 	InputColorFormat_YUV,	// ColorFormat
 	Video_MOVIE,	// VideoImageMode
 	Graphics_STANDARD,	// GraphicsImageMode
 	CTEMP_6500K,	// PresetColorTemp
 	WB_CSPACE_DEFAULT,	// PresetColorSpace
 	ColorMode_GRAPHIC,	// ColorMode
 	WB_SHARPNESS_DEFAULT,	// Sharpness
 	WB_SHARPNESS_DEFAULT,	// Sharpness_All
 	WB_SHARPNESS_DEFAULT,	// Sharpness_Text
 	0,	// ImageZoom
 	0,	// AspectRatio

	#if(BOARD_USED == DELL_93xx_BOARD)
 	{
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ On, UNIFORMITY_OFF },
		{ On, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF }
	},
	#else
	{
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF }
	},	
	#endif
	0,	// ResponseTime
 	Gamma_PC,	// GammaMode
 	WB_SATURATION_YPbPr_DEFAULT,	// Saturation_YPbPr
 	PRINTER_PREVUE1,	// PresetPrinterPrevue
 	TNR_ADAPTIVE,	// PresetPrinterPrevue
 	0,	//AcmRedHue
 	0,	//AcmYellowHue
 	0,	//AcmBlueSat
 	0,	//AcmCyanSat
 	0,	//AcmGreenSat
 	0,	//AcmMagentSat
 	0,	//AcmRedSat
 	0,	//AcmYellowSat
 	VIVID,	//MFMImageScheme
 	0,	//InputColorDomain
 	0,	//OutputColorType
	MPEG_NR_GAUSS,
 },
#endif	
#ifdef EXTERNAL_DECODER
{// 7 INPUT_COMP2 on Decoder
	0,	// Acm3BlueLuma
 	0,	// Acm3CyanLuma
 	0,	// Acm3GreenLuma
 	0,	// Acm3MagentaLuma
 	0,	// Acm3RedLuma
 	0,	// Acm3YellowLuma
 	0,	// Acm3BlackLuma
 	0,	// Acm3WhiteLuma
 	0,	// Acm3BlueHue
 	0,	// Acm3CyanHue
 	0,	// Acm3GreenHue
 	0,	// Acm3MagentaHue
 	0,	// Acm3RedHue
 	0,	// Acm3YellowHue
 	0,	// Acm3BlackHue
 	0,	// Acm3WhiteHue
 	50,	// Acm3BlueSat
 	50,	// Acm3CyanSat
 	50,	// Acm3GreenSat
 	50,	// Acm3MagentaSat
 	50,	// Acm3RedSat
 	50,	// Acm3YellowSat
 	0,	// Acm3BlackSat
 	0,	// Acm3WhiteSat
 	WB_BRIGHTNESS_DEFAULT,	// Brightness
 	WB_BRIGHTNESS_DEFAULT,	// Brightness_All
 	WB_BRIGHTNESS_TEXT_DEFAULT,	// Brightness_Text
 	WB_BRIGHTNESS_CAL1_DEFAULT,	// Brightness_CAL1
 	WB_BRIGHTNESS_CAL2_DEFAULT,	// Brightness_CAL2
 	WB_CONTRAST_DEFAULT,	// Contrast
 	WB_CONTRAST_DEFAULT,	// Contrast_All
 	WB_CONTRAST_TEXT_DEFAULT,	// Contrast_Text
 	WB_CONTRAST_CAL1_DEFAULT,	// Contrast_CAL1
 	WB_CONTRAST_CAL2_DEFAULT,	// Contrast_CAL2
 	WB_SATURATION_DEFAULT,	// Saturation
 	WB_SATURATION_DEFAULT,	// Saturation_Movie
 	WB_SATURATION_DEFAULT,	// Saturation_Game
 	0,	// Hue
 	0,	// Hue_Movie
 	0,	// Hue_Game
 	USER_GAIN_DEF,	// UserRedGain
 	USER_GAIN_DEF,	// UserGreenGain
 	USER_GAIN_DEF,	// UserBlueGain
 	0,	// UserRedOff
 	0,	// UserGreenOff
 	0,	// UserBlueOff
 	CTEMP_6500K,	// ColorTemp
 	GAMMA_TABLE_OFF,	// GammaTable 'Desc' :Varaible to choose Gamma Table
 	255,	// UserRed
 	255,	// UserGreen
 	255,	// UserBlue
 	InputColorFormat_YUV,	// ColorFormat
 	Video_MOVIE,	// VideoImageMode
 	Graphics_STANDARD,	// GraphicsImageMode
 	CTEMP_6500K,	// PresetColorTemp
 	WB_CSPACE_DEFAULT,	// PresetColorSpace
 	ColorMode_GRAPHIC,	// ColorMode
 	WB_SHARPNESS_DEFAULT,	// Sharpness
 	WB_SHARPNESS_DEFAULT,	// Sharpness_All
 	WB_SHARPNESS_DEFAULT,	// Sharpness_Text
 	0,	// ImageZoom
 	0,	// AspectRatio

	#if(BOARD_USED == DELL_93xx_BOARD)
 	{
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ On, UNIFORMITY_OFF },
		{ On, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF }
	},
	#else
	{
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF }
	},	
	#endif
	0,	// ResponseTime
 	Gamma_PC,	// GammaMode
 	WB_SATURATION_YPbPr_DEFAULT,	// Saturation_YPbPr
 	PRINTER_PREVUE1,	// PresetPrinterPrevue
 	TNR_HIGH,	// PresetPrinterPrevue
 	0,	//AcmRedHue
 	0,	//AcmYellowHue
 	0,	//AcmBlueSat
 	0,	//AcmCyanSat
 	0,	//AcmGreenSat
 	0,	//AcmMagentSat
 	0,	//AcmRedSat
 	0,	//AcmYellowSat
 	VIVID,	//MFMImageScheme
 	0,	//InputColorDomain
 	0,	//OutputColorType
	MPEG_NR_GAUSS,
 },

{// 8 INPUT_CVBS2
	0,	// Acm3BlueLuma
 	0,	// Acm3CyanLuma
 	0,	// Acm3GreenLuma
 	0,	// Acm3MagentaLuma
 	0,	// Acm3RedLuma
 	0,	// Acm3YellowLuma
 	0,	// Acm3BlackLuma
 	0,	// Acm3WhiteLuma
 	0,	// Acm3BlueHue
 	0,	// Acm3CyanHue
 	0,	// Acm3GreenHue
 	0,	// Acm3MagentaHue
 	0,	// Acm3RedHue
 	0,	// Acm3YellowHue
 	0,	// Acm3BlackHue
 	0,	// Acm3WhiteHue
 	50,	// Acm3BlueSat
 	50,	// Acm3CyanSat
 	50,	// Acm3GreenSat
 	50,	// Acm3MagentaSat
 	50,	// Acm3RedSat
 	50,	// Acm3YellowSat
 	0,	// Acm3BlackSat
 	0,	// Acm3WhiteSat
 	WB_BRIGHTNESS_DEFAULT,	// Brightness
 	WB_BRIGHTNESS_DEFAULT,	// Brightness_All
 	WB_BRIGHTNESS_TEXT_DEFAULT,	// Brightness_Text
 	WB_BRIGHTNESS_CAL1_DEFAULT,	// Brightness_CAL1
 	WB_BRIGHTNESS_CAL2_DEFAULT,	// Brightness_CAL2
 	WB_CONTRAST_DEFAULT,	// Contrast
 	WB_CONTRAST_DEFAULT,	// Contrast_All
 	WB_CONTRAST_TEXT_DEFAULT,	// Contrast_Text
 	WB_CONTRAST_CAL1_DEFAULT,	// Contrast_CAL1
 	WB_CONTRAST_CAL2_DEFAULT,	// Contrast_CAL2
 	WB_SATURATION_DEFAULT,	// Saturation
 	WB_SATURATION_DEFAULT,	// Saturation_Movie
 	WB_SATURATION_DEFAULT,	// Saturation_Game
 	0,	// Hue
 	0,	// Hue_Movie
 	0,	// Hue_Game
 	USER_GAIN_DEF,	// UserRedGain
 	USER_GAIN_DEF,	// UserGreenGain
 	USER_GAIN_DEF,	// UserBlueGain
 	0,	// UserRedOff
 	0,	// UserGreenOff
 	0,	// UserBlueOff
 	CTEMP_6500K,	// ColorTemp
 	GAMMA_TABLE_OFF,	// GammaTable 'Desc' :Varaible to choose Gamma Table
 	255,	// UserRed
 	255,	// UserGreen
 	255,	// UserBlue
 	InputColorFormat_YUV,	// ColorFormat
 	Video_MOVIE,	// VideoImageMode
 	Graphics_STANDARD,	// GraphicsImageMode
 	CTEMP_6500K,	// PresetColorTemp
 	WB_CSPACE_DEFAULT,	// PresetColorSpace
 	ColorMode_GRAPHIC,	// ColorMode
 	WB_SHARPNESS_DEFAULT,	// Sharpness
 	WB_SHARPNESS_DEFAULT,	// Sharpness_All
 	WB_SHARPNESS_DEFAULT,	// Sharpness_Text
 	0,	// ImageZoom
 	0,	// AspectRatio

	#if(BOARD_USED == DELL_93xx_BOARD)
 	{
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ On, UNIFORMITY_OFF },
		{ On, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF },
		{ Off, UNIFORMITY_OFF }
	},
	#else
	{
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF },
		{ UNIFORMITY_OFF }
	},	
	#endif
	0,	// ResponseTime
 	Gamma_PC,	// GammaMode
 	WB_SATURATION_YPbPr_DEFAULT,	// Saturation_YPbPr
 	PRINTER_PREVUE1,	// PresetPrinterPrevue
 	TNR_HIGH,	// PresetPrinterPrevue
 	0,	//AcmRedHue
 	0,	//AcmYellowHue
 	0,	//AcmBlueSat
 	0,	//AcmCyanSat
 	0,	//AcmGreenSat
 	0,	//AcmMagentSat
 	0,	//AcmRedSat
 	0,	//AcmYellowSat
 	VIVID,	//MFMImageScheme
 	0,	//InputColorDomain
 	0,	//OutputColorType
	MPEG_NR_GAUSS,
 }
#endif

};
#else

PortSchemeDependentUserPrefType ROM SchemeFactoryDefaultInROM[NUM_IMAGE_SCHEMES] =
{
   {	// 0 Normal
      0,	// Acm3BlueLuma
      0,	// Acm3CyanLuma
      0,	// Acm3GreenLuma
      0,	// Acm3MagentaLuma
      0,	// Acm3RedLuma
      0,	// Acm3YellowLuma
      0,	// Acm3BlackLuma
      0,	// Acm3WhiteLuma

      0,	// Acm3BlueHue
      0,	// Acm3CyanHue
      0,	// Acm3GreenHue
      0,	// Acm3MagentaHue
      0,	// Acm3RedHue
      0,	// Acm3YellowHue
      0,        // Acm3BlackHue
      0,        // Acm3WhiteHue

      50,	// Acm3BlueSat
      50,	// Acm3CyanSat
      50,	// Acm3GreenSat
      50,	// Acm3MagentaSat
      50,	// Acm3RedSat
      50,	// Acm3YellowSat
      0,	// Acm3BlackSat
      0,	// Acm3WhiteSat
      
      0,	// AcmBlueHue
      0,	// AcmCyanHue
      0,	// AcmGreenHue
      0,	// AcmMagentaHue
      0,	// AcmRedHue
      0,	// AcmYellowHue
      0,	// AcmBlueSat
      0,	// AcmCyanSat
      0,	// AcmGreenSat
      0,	// AcmMagentaSat
      0,	// AcmRedSat
      0,	// AcmYellowSat
      0,	// Brightness
      256,	// Contrast
      256,	// Saturation
      0,		// Hue
      256,	// UserRedGain
      256,	// UserGreenGain
      256,	// UserBlueGain
      0,	// UserRedOff
      0,	// UserGreenOff
      0,	// UserBlueOff
      CTEMP_6500K,	// ColorTemp
      GAMMA_TABLE_22, // Gamma Table
      USER, //MFMImageScheme
      0,
      0,
      0,
   },
   {	// 1  Vivid
      0,	// Acm3BlueLuma
      0,	// Acm3CyanLuma
      0,	// Acm3GreenLuma
      0,	// Acm3MagentaLuma
      0,	// Acm3RedLuma
      0,	// Acm3YellowLuma
      0,	// Acm3BlackLuma
      0,	// Acm3WhiteLuma      
      
      0,	// Acm3BlueHue
      0,	// Acm3CyanHue
      0,	// Acm3GreenHue
      0,	// Acm3MagentaHue
      0,	// Acm3RedHue
      0,	// Acm3YellowHue
      0, // Acm3BlackHue
      0, // Acm3WhiteHue      
      
      50,	// Acm3BlueSat
      50,	// Acm3CyanSat
      50,	// Acm3GreenSat
      50,	// Acm3MagentaSat
      50,	// Acm3RedSat
      50,	// Acm3YellowSat
      0,	// Acm3BlackSat
      0,	// Acm3WhiteSat
      
      0,	// AcmBlueHue
      0,	// AcmCyanHue
      0,	// AcmGreenHue
      0,	// AcmMagentaHue
      0,	// AcmRedHue
      0,	// AcmYellowHue
      0,	// AcmBlueSat
      0,	// AcmCyanSat
      0,	// AcmGreenSat
      0,	// AcmMagentaSat
      0,	// AcmRedSat
      0,	// AcmYellowSat
      0,
      256,	// Contrast
      317,
      0,	// Hue
      256,	// UserRedGain
      256,	// UserGreenGain
      256,	// UserBlueGain
      0,	// UserRedOff
      0,	// UserGreenOff
      0,	// UserBlueOff
      CTEMP_9300K,	// ColorTemp
      GAMMA_TABLE_22,  //Gamma Table
      VIVID, //MFMImageScheme
      0,
      0,
      0,
   },
   {	// 2 Cinema
      0,	// Acm3BlueLuma
      0,	// Acm3CyanLuma
      0,	// Acm3GreenLuma
      0,	// Acm3MagentaLuma
      0,	// Acm3RedLuma
      0,	// Acm3YellowLuma
      0,	// Acm3BlackLuma
      0,	// Acm3WhiteLuma      
      
      0,	// Acm3BlueHue
      0,	// Acm3CyanHue
      0,	// Acm3GreenHue
      0,	// Acm3MagentaHue
      0,	// Acm3RedHue
      0,	// Acm3YellowHue
      0, // Acm3BlackHue
      0, // Acm3WhiteHue
      
      50,  // Acm3BlueSat
      50,  // Acm3CyanSat
      50,  // Acm3GreenSat
      50,  // Acm3MagentaSat
      50,  // Acm3RedSat
      50,  // Acm3YellowSat
      0,	// Acm3BlackSat
      0,	// Acm3WhiteSat
      
      0,	// AcmBlueHue
      0,	// AcmCyanHue
      0,	// AcmGreenHue
      0,	// AcmMagentaHue
      0,	// AcmRedHue
      0,	// AcmYellowHue
      0,	// AcmBlueSat
      0,	// AcmCyanSat
      0,	// AcmGreenSat
      0,	// AcmMagentaSat
      0,	// AcmRedSat
      0,	// AcmYellowSat
      0,	// Brightness
      256,	// Contrast
      317,	// Saturation
      0,	// Hue
      256,	// UserRedGain
      256,	// UserGreenGain
      256,	// UserBlueGain
      0,	// UserRedOff
      0,	// UserGreenOff
      0,	// UserBlueOff
      CTEMP_6500K,	// ColorTemp
      GAMMA_TABLE_22, // Gamma table
      CINEMA, //MFMImageScheme
      0,
      0,
      0,
   },
   { // 3 Game
      0,	// Acm3BlueLuma
      0,	// Acm3CyanLuma
      0,	// Acm3GreenLuma
      0,	// Acm3MagentaLuma
      0,	// Acm3RedLuma
      0,	// Acm3YellowLuma
      0,	// Acm3BlackLuma
      0,	// Acm3WhiteLuma      
      
      0,	// Acm3BlueHue
      0,	// Acm3CyanHue
      0,	// Acm3GreenHue
      0,	// Acm3MagentaHue
      0,	// Acm3RedHue
      0,	// Acm3YellowHue
      0, // Acm3BlackHue
      0, // Acm3WhiteHue
      
      50,	// Acm3BlueSat
      50,	// Acm3CyanSat
      50,	// Acm3GreenSat
      50,	// Acm3MagentaSat
      50,	// Acm3RedSat
      50,	// Acm3YellowSat
      0,	// Acm3BlackSat
      0,	// Acm3WhiteSat
      
      0,	// AcmBlueHue
      0,	// AcmCyanHue
      0,	// AcmGreenHue
      0,	// AcmMagentaHue
      0,	// AcmRedHue
      0,	// AcmYellowHue
      0,	// AcmBlueSat
      0,	// AcmCyanSat
      0,	// AcmGreenSat
      0,	// AcmMagentaSat
      0,	// AcmRedSat
      0,	// AcmYellowSat
      0,	// Brightness
      256,	// Contrast
      256,	// Saturation
      0,	// Hue
      256,	// UserRedGain
      256,	// UserGreenGain
      256,	// UserBlueGain
      0,	// UserRedOff
      0,	// UserGreenOff
      0,	// UserBlueOff
      CTEMP_6500K,	// ColorTemp
      GAMMA_TABLE_22, // Gamma Table
      GAME, //MFMImageScheme
      0,
      0,
      0,
   },
   {	// 4  Sport
      0,	// Acm3BlueLuma
      0,	// Acm3CyanLuma
      0,	// Acm3GreenLuma
      0,	// Acm3MagentaLuma
      0,	// Acm3RedLuma
      0,	// Acm3YellowLuma
      0,	// Acm3BlackLuma
      0,	// Acm3WhiteLuma      
      
      0,	// Acm3BlueHue
      0,	// Acm3CyanHue
      0,	// Acm3GreenHue
      0,	// Acm3MagentaHue
      0,	// Acm3RedHue
      0,	// Acm3YellowHue
      0, // Acm3BlackHue
      0, // Acm3WhiteHue
      
      50,	// Acm3BlueSat
      50,	// Acm3CyanSat
      50,	// Acm3GreenSat
      50,	// Acm3MagentaSat
      50,	// Acm3RedSat
      50,	// Acm3YellowSat
      0,	// Acm3BlackSat
      0,	// Acm3WhiteSat
      
      0,	// AcmBlueHue
      0,	// AcmCyanHue
      0,	// AcmGreenHue
      0,	// AcmMagentaHue
      0,	// AcmRedHue
      0,	// AcmYellowHue
      0,	// AcmBlueSat
      0,	// AcmCyanSat
      0,	// AcmGreenSat
      0,	// AcmMagentaSat
      0,	// AcmRedSat
      0,	// AcmYellowSat
      0,	// Brightness
      256,	// Contrast
      256,	// Saturation
      0,	// Hue
      256,	// UserRedGain
      256,	// UserGreenGain
      256,	// UserBlueGain
      0,	// UserRedOff
      0,	// UserGreenOff
      0,	// UserBlueOff
      CTEMP_9300K,	// ColorTemp
      GAMMA_TABLE_22, // Gamma Table
      SPORT, //MFMImageScheme
      0,
      0,
      0,
   },
   //{	// 5 Custom
   //	0,	// Acm3BlueLuma
   //	0,	// Acm3CyanLuma
   //	0,	// Acm3GreenLuma
   //	0,	// Acm3MagentaLuma
   //	0,	// Acm3RedLuma
   //	0,	// Acm3YellowLuma
   // 0,	// Acm3BlackLuma
   // 0,	// Acm3WhiteLuma   
   
   //	0,	// Acm3BlueHue
   //	0,	// Acm3CyanHue
   //	0,	// Acm3GreenHue
   //	0,	// Acm3MagentaHue
   //	0,	// Acm3RedHue
   //	0,	// Acm3YellowHue
   // 0, // Acm3BlackHue
   // 0, // Acm3WhiteHue
   
   //	0,	// Acm3BlueSat
   //	0,	// Acm3CyanSat
   //	0,	// Acm3GreenSat
   //	0,	// Acm3MagentaSat
   //	0,	// Acm3RedSat
   //	0,	// Acm3YellowSat
   // 0,	// Acm3BlackSat
   // 0,	// Acm3WhiteSat
   
   //	0,	// AcmBlueHue
   //	0,	// AcmCyanHue
   //	0,	// AcmGreenHue
   //	0,	// AcmMagentaHue
   //	0,	// AcmRedHue
   //	0,	// AcmYellowHue
   //	0,	// AcmBlueSat
   //	0,	// AcmCyanSat
   //	0,	// AcmGreenSat
   //	0,	// AcmMagentaSat
   //	0,	// AcmRedSat
   //	0,	// AcmYellowSat
   //	0,	// Brightness
   //	256,	// Contrast
   //	256,	// Saturation
   //	0,		// Hue
   //	256,	// UserRed
   //	256,	// UserGreen
   //	256,	// UserBlue
   //	CTEMP_6500K,	// ColorTemp
   //	CUSTOM
   //0,
   //0,
   //0,
   //}
};
#endif


