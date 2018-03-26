/*
	$Workfile:   LM220WE1_TLD1.h  $
	$Revision: 1.5 $
	$Date: 2011/09/16 02:52:15 $
*/

#ifdef INCLUDE_LG_WSXGA_LM220WE1_TLD1

#include "..\..\Panel\Wsxga\LG\LM220WE1_TLD1\OverdriveLut.h"
#include "..\..\Panel\Wsxga\LG\LM220WE1_TLD1\GammaLut.h"

char ROM PanelName_LM220WE1_TLD1[] = "LG_WSXGA_LM220WE1_TLD1";

ROM WORD sRGB2rgb_LM220WE1_TLD1[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};

ROM gmt_PanelData Panel_LG_WSXGA_LM220WE1_TLD1 =
{
	PanelName_LM220WE1_TLD1,
	LG_WSXGA_LM220WE1_TLD1,			// Panel ID (refer to enum defined in panel.h)
	PANEL_WSXGA,						// gmd_PanelType PanelType
	FORMAT_ORG_2D,                // gmd_PanelDataFormat;
	ONE, // TWO,								// BYTE PixPerClock
	8,									// BYTE  Depth
	1680, // 1920,								// WORD  Width
	1050, // 1200,								// WORD  Height
	75, // 60,									// BYTE  MaxVFreq
	50, // 57,									// BYTE  MinVFreq
	164000UL, // 97500L,								// DWORD MaxPClk (KHz)
	1840, // 2200,								// WORD W_MinHTotal
	1840, // 2200,								// W_TypHTotal
	2400, // 2200,								// W_22HTotal
	2400, // 2200,								// W_33HTotal
	8,									   // BYTE  MinHSyncWidth
	16,								   // BYTE  MinHSyncBackPorch
	1080, // 1240,								// W_22VTotal
	1080, // 1238,								// W_33VTotal
	1300, // 1250,								// WORD  MaxVtotal
	1060, // 1208,								// WORD  MinVtotal
	1080, // 1250,								// WORD TypVTotal
	3,									// BYTE  MinVSyncWidth
	5, // 43,									// BYTE  MinVSyncBackPorch
	
	0x1101,								// WORD  PowerUpTiming
	0x1101,								// WORD  PowerDownTiming
	0xE,								   // WORD AccToPixelVal
	0x0,								   // BYTE  SpreadSpectrumCtrl
	0,									   // DCLK display timing delay adjustment in ns unit
	Wa_OverDriveLUT_LM220WE1_TLD1,		// Overdrive look up table
	sRGB2rgb_LM220WE1_TLD1,
	Sta_GammaLUT_LM220WE1_TLD1,
	0,									// WORD  Invert_DVS
	0,									// WORD  Invert_DHS
	0,									// WORD  Invert_DCLK
	0,									// WORD  Invert_DEN
	0xFF,								// WORD  DportSwap
	TRUE,								// WORD  LVDSInterface
	DUAL_BUS,							// WORD  W_LVDS_BusType
	LVDS_EIGHT_BIT_MODE,				// WORD  LvdsPanelMode
	0xFF,								// WORD  Spread_Spect_En
	SWAP,								// WORD	LvdsPosNegSwap
	0,      							//WORD  PadDrive;
	SEVEN_PT_ONE_MA,					// WORD  W_LvdsCurrentCtrl
	OUT_24_48_BIT,						// WORD  W_BitOutput
	STPANEL_LVDS_MAP_STANDARD2,			// WORD W_LVDSSignalMap
	0x0063,								// WORD  W_LvdsClkData
};

extern ROM gmt_PanelData Panel_LG_WSXGA_LM220WE1_TLD1;

#else
	#define Panel_LG_WSXGA_LM220WE1_TLD1	(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == LG_WSXGA_LM220WE1_TLD1)
		#pragma message "Default panel will not bootup unless INCLUDE_LG_WSXGA_LM220WE1_TLD1 is defined!"
	#endif
#endif

