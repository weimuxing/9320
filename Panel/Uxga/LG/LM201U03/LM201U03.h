/*
	$Workfile:   LM201U03.h  $
	$Revision: 1.4 $
	$Date: 2012/02/23 08:25:58 $
*/

#ifdef INCLUDE_LG_UXGA_LM201U03

#include "..\..\Panel\Uxga\LG\LM201U03\OverdriveLut.h"
#include "..\..\Panel\Uxga\LG\LM201U03\GammaLut.h"

char ROM PanelName_LG_UXGA_LM201U03[] = "LG_UXGA_LM201U03";

ROM WORD sRGB2rgb_LM201U03[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};

ROM gmt_PanelData Panel_LG_UXGA_LM201U03 =
{
	PanelName_LG_UXGA_LM201U03,
	LG_UXGA_LM201U03		,			// Panel ID (refer to enum defined in panel.h)
	PANEL_UXGA,							// gmd_PanelType PanelType
	FORMAT_ORG_2D,    // gmd_PanelDataFormat;
	TWO,								// BYTE PixPerClock
	8,									// BYTE  Depth
	1600,								// WORD  Width
	1200,								// WORD  Height
	61,									// BYTE  MaxVFreq
	59,									// BYTE  MinVFreq
	127000L,							// DWORD MaxPClk (KHz)
	1680,								// WORD W_MinHTotal
	2160,								// W_TypHTotal
	1800,								// W_22HTotal
	1800,								// W_33HTotal
	8,									// BYTE  MinHSyncWidth
	32,									// BYTE  MinHSyncBackPorch
	1240,								// W_22VTotal
	1238,								// W_33VTotal
	1250,								// WORD  MaxVtotal
	1208,								// WORD  MinVtotal
	1250,								// WORD TypVTotal
	3,									// BYTE  MinVSyncWidth
	25,									// BYTE  MinVSyncBackPorch
	
	0x1101,								// WORD  PowerUpTiming
	0x1101,								// WORD  PowerDownTiming
	0xE,								// WORD AccToPixelVal
	0x0,								// BYTE  SpreadSpectrumCtrl
	0,									// DCLK display timing delay adjustment in ns unit
	Wa_OverDriveLUT_LM201U03,			// Overdrive look up table
	sRGB2rgb_LM201U03,
	Sta_GammaLUT_LM201U03,
	0,									// WORD  Invert_DVS
	0,									// WORD  Invert_DHS
	0,									// WORD  Invert_DCLK
	0,									// WORD  Invert_DEN
	0,								// WORD  DportSwap
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

extern ROM gmt_PanelData Panel_LG_UXGA_LM201U03;

#else
	#define Panel_LG_UXGA_LM201U03		(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == LG_UXGA_LM201U03)
		#pragma message "Default panel will not bootup unless INCLUDE_LG_UXGA_LM201U03 is defined!"
	#endif
#endif
