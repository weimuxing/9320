/*
	$Workfile:   LM270WQ1.h  $
	$Revision: 1.1 $
	$Date: 2011/09/08 06:56:13 $
*/

#ifdef INCLUDE_LG_WQHD_LM270WQ1

#include "..\..\Panel\WQHD\LG\LM270WQ1\OverdriveLut.h"
#include "..\..\Panel\WQHD\LG\LM270WQ1\GammaLut.h"

char ROM PanelName_LM270WQ1[] = "LG_WQHD_LM270WQ1";

ROM WORD sRGB2rgb_LM270WQ1[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};
ROM gmt_PanelData Panel_LG_WQHD_LM270WQ1 =
{
	PanelName_LM270WQ1,
	LG_WQHD_LM270WQ1,			// Panel ID (refer to enum defined in panel.h)
	PANEL_WQHD,						// gmd_PanelType PanelType
	FORMAT_ORG_2D,    // gmd_PanelDataFormat;
	FOUR,								// BYTE PixPerClock
	10,	// 10 bits								// BYTE  Depth	
	2560,								// WORD  Width
	1440,								// WORD  Height
	60,									// BYTE  MaxVFreq
	60,									// BYTE  MinVFreq
	241500L,								// DWORD MaxPClk (KHz)
	2720,								// WORD W_MinHTotal
	2720,								// W_TypHTotal
	2720,								// W_22HTotal
	2720,								// W_33HTotal
	32,									// BYTE  MinHSyncWidth
	80, 								// BYTE  MinHSyncBackPorch
	1481,								// W_22VTotal
	1481,								// W_33VTotal
	1481,								// WORD  MaxVtotal
	1481,								// WORD  MinVtotal
	1481,								// WORD TypVTotal
	5,									// BYTE  MinVSyncWidth
	33,									// BYTE  MinVSyncBackPorch
	0xE016,								// WORD  PowerUpTiming
	0x0060,								// WORD  PowerDownTiming
	0x21,								// WORD AccToPixelVal
	0,	//Hyb130520* Org = 0x2F4,								// BYTE  SpreadSpectrumCtrl
	0,									// DCLK display timing delay adjustment in ns unit
	Wa_OverDriveLUT_LM270WQ1,		// Overdrive look up table
	sRGB2rgb_LM270WQ1,
	Sta_GammaLUT_LM270WQ1,
	0,									// WORD  Invert_DVS
	0,									// WORD  Invert_DHS
	0,									// WORD  Invert_DCLK
	0,									// WORD  Invert_DEN
	
	0, // 0xFF,								// WORD  DportSwap
	TRUE,								// WORD  LVDSInterface
	QUAD_BUS,							// WORD  W_LVDS_BusType
	LVDS_EIGHT_BIT_MODE,				// WORD  LvdsPanelMode
	0,	//Hyb130520* Org = 0xFF, DP has its own Spectrum;							// WORD  Spread_Spect_En
	NO_SWAP,								// WORD	LvdsPosNegSwap
	STPLF_DISPLAY_PAD_DRIVE_1_25,		// WORD PadDrive	
	STPLF_DISPLAY_LVDS_CURRENT_3_5,		// WORD  W_LvdsCurrentCtrl
	OUT_24_48_BIT,						// WORD  W_BitOutput
	STPANEL_LVDS_MAP_STANDARD2,			// WORD W_LVDSSignalMap
	0x0063,								// WORD  W_LvdsClkData
};
extern ROM gmt_PanelData Panel_LG_WQHD_LM270WQ1;

#else
	#define Panel_LG_WQHD_LM270WQ1	(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == LG_WQHD_LM270WQ1)
		#pragma message "Default panel will not bootup unless INCLUDE_LG_WQHD_LM270WQ1 is defined!"
	#endif
#endif

