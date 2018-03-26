/*
	$Workfile:   LTM240W1_L01.h  $
	$Revision: 1.4 $
	$Date: 2011/07/29 05:47:10 $
*/

#ifdef INCLUDE_SAMSUNG_WUXGA_LTM240W1_L01

#include "..\..\Panel\WUxga\Samsung\LTM240W1_L01\OverdriveLut.h"
#include "..\..\Panel\WUxga\Samsung\LTM240W1_L01\GammaLut.h"

char ROM PanelName_LTM240W1_L01[] = "SAMSUNG_WUXGA_LTM240W1_L01";

ROM WORD sRGB2rgb_LTM240W1_L01[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};

ROM gmt_PanelData Panel_SAMSUNG_WUXGA_LTM240W1_L01 =
{
	PanelName_LTM240W1_L01,
	SAMSUNG_WUXGA_LTM240W1_L01,			// Panel ID (refer to enum defined in panel.h)
	PANEL_WUXGA,						// gmd_PanelType PanelType
	FORMAT_ORG_2D,    // gmd_PanelDataFormat;
	TWO,								// BYTE PixPerClock
	8,									// BYTE  Depth
	1920,								// WORD  Width
	1200,								// WORD  Height
	60,									// BYTE  MaxVFreq
	57,									// BYTE  MinVFreq
	97500L,								// DWORD MaxPClk (KHz)
	2200,								// WORD W_MinHTotal
	2200,								// W_TypHTotal
	2200,								// W_22HTotal
	2200,								// W_33HTotal
	16,									// BYTE  MinHSyncWidth
	120,								// BYTE  MinHSyncBackPorch
	1240,								// W_22VTotal
	1238,								// W_33VTotal
	1250,								// WORD  MaxVtotal
	1208,								// WORD  MinVtotal
	1250,								// WORD TypVTotal
	3,									// BYTE  MinVSyncWidth
	43,									// BYTE  MinVSyncBackPorch
	
	0x1101,								// WORD  PowerUpTiming
	0x1101,								// WORD  PowerDownTiming
	0xE,								// WORD AccToPixelVal
	0x0,								// BYTE  SpreadSpectrumCtrl
	0,									// DCLK display timing delay adjustment in ns unit
	Wa_OverDriveLUT_LTM240W1_L01,		// Overdrive look up table
	sRGB2rgb_LTM240W1_L01,
	Sta_GammaLUT_LTM240W1_L01,
	0,									// WORD  Invert_DVS
	0,									// WORD  Invert_DHS
	0,									// WORD  Invert_DCLK
	0,									// WORD  Invert_DEN
	0xFF,								// WORD  DportSwap
	TRUE,								// WORD  LVDSInterface
	DUAL_BUS,							// WORD  W_LVDS_BusType
	LVDS_EIGHT_BIT_MODE,				// WORD  LvdsPanelMode
	0xFF,								// WORD  Spread_Spect_En
	NO_SWAP,								// WORD	LvdsPosNegSwap
	0,      							//WORD  PadDrive;
	SEVEN_PT_ONE_MA,					// WORD  W_LvdsCurrentCtrl
	OUT_24_48_BIT,						// WORD  W_BitOutput
	STPANEL_LVDS_MAP_STANDARD2,			// WORD W_LVDSSignalMap
	0x0063,								// WORD  W_LvdsClkData
};

extern ROM gmt_PanelData Panel_SAMSUNG_WUXGA_LTM240W1_L01;

#else
	#define Panel_SAMSUNG_WUXGA_LTM240W1_L01	(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == SAMSUNG_WUXGA_LTM240W1_L01)
		#pragma message "Default panel will not bootup unless INCLUDE_SAMSUNG_WUXGA_LTM240W1_L01 is defined!"
	#endif
#endif

