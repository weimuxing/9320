/*
	$Workfile:   LTA400HH_LH1.H  $
	$Revision:   1.0  $
	$Date:   Jan 23 2008 15:45:52  $
*/

#ifdef INCLUDE_SAMSUNG_1080P_LTA400HH_LH1

#include "..\..\Panel\1080P\Samsung\LTA400HH_LH1\OverdriveLut.h"
#include "..\..\Panel\1080P\Samsung\LTA400HH_LH1\GammaLut.h"

char ROM PanelName_LTA400HH_LH1[] = "SAMSUNG_1080P_LTA400HH_LH1";

ROM WORD sRGB2rgb_LTA400HH_LH1[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};

ROM gmt_PanelData Panel_SAMSUNG_1080P_LTA400HH_LH1 =
{
	PanelName_LTA400HH_LH1,
	SAMSUNG_1080P_LTA400HH_LH1,			// Panel ID (refer to enum defined in panel.h)
	PANEL_1080P,						// gmd_PanelType PanelType
	TWO,								// BYTE PixPerClock
	10,									// BYTE  Depth
	1920,								// WORD  Width
	1080,								// WORD  Height
	65,									// BYTE  MaxVFreq
	45,									// BYTE  MinVFreq
	155000L,								// DWORD MaxPClk (KHz)
	2160,								// WORD W_MinHTotal
	2200,								// W_TypHTotal
	2200,								// W_22HTotal
	2200,								// W_33HTotal
	10,									// BYTE  MinHSyncWidth
	10,								// BYTE  MinHSyncBackPorch
	1125,								// W_22VTotal
	1125,								// W_33VTotal
	1550,								// WORD  MaxVtotal
	1118,								// WORD  MinVtotal
	1125,								// WORD TypVTotal
	3,									// BYTE  MinVSyncWidth
	4,									// BYTE  MinVSyncBackPorch
	0x775522L,							// DWORD PadDrive
	0x1101,								// WORD  PowerUpTiming
	0x1101,								// WORD  PowerDownTiming
	0xE,								// WORD AccToPixelVal
	0x0,								// BYTE  SpreadSpectrumCtrl
	0,									// DCLK display timing delay adjustment in ns unit
	Wa_OverDriveLUT_LTA400HH_LH1,		// Overdrive look up table
	sRGB2rgb_LTA400HH_LH1,
	Sta_GammaLUT_LTA400HH_LH1,
	0,									// WORD  Invert_DVS
	0,									// WORD  Invert_DHS
	0,									// WORD  Invert_DCLK
	0,									// WORD  Invert_DEN
	0x00,								// WORD  DportSwap
	TRUE,								// WORD  LVDSInterface
	DUAL_BUS,							// WORD  W_LVDS_BusType
	LVDS_EIGHT_BIT_MODE,				// WORD  LvdsPanelMode
	0xFF,								// WORD  Spread_Spect_En
	NO_SWAP,								// WORD	LvdsPosNegSwap
	SEVEN_PT_ONE_MA,					// WORD  W_LvdsCurrentCtrl
	OUT_30_BIT,						// WORD  W_BitOutput
	0x0063,								// WORD  W_LvdsClkData
};

extern ROM gmt_PanelData Panel_SAMSUNG_1080P_LTA400HH_LH1;

#else
	#define Panel_SAMSUNG_1080P_LTA400HH_LH1	(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == SAMSUNG_1080P_LTA400HH_LH1)
		#pragma message "Default panel will not bootup unless INCLUDE_SAMSUNG_1080P_LTA400HH_LH1 is defined!"
	#endif
#endif

