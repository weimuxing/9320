/*
	$Workfile:   VEGA_1080P.h  $
	$Revision: 1.1 $
	$Date: 2011/06/28 11:30:05 $
*/

#ifdef INCLUDE_ST_1080P_VEGA_1080P

#include "..\..\Panel\1080P\ST\VEGA_1080P\OverdriveLut.h"
#include "..\..\Panel\1080P\ST\VEGA_1080P\GammaLut.h"

char ROM PanelName_VEGA_1080P[] = "ST_1080P_VEGA_1080P";

ROM WORD sRGB2rgb_VEGA_1080P[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};

ROM gmt_PanelData Panel_ST_1080P_VEGA_1080P =
{
	PanelName_VEGA_1080P,
	ST_1080P_VEGA_1080P,			// Panel ID (refer to enum defined in panel.h)
	PANEL_1080P,						// gmd_PanelType PanelType
	FORMAT_ORG_2D,    // gmd_PanelDataFormat;
	FOUR,								// BYTE PixPerClock
	8,									// BYTE  Depth
	1920,								// WORD  Width
	1080,								// WORD  Height
	60,									// BYTE  MaxVFreq
	60,									// BYTE  MinVFreq
    148500L,							// DWORD MaxPClk (KHz)
	2080,								// WORD W_MinHTotal
	2200,								// W_TypHTotal
	2700,								// W_22HTotal
	2700,								// W_33HTotal
	44,									// BYTE  MinHSyncWidth
	148, 								// BYTE  MinHSyncBackPorch
	1090,								// W_22VTotal
	1100,								// W_33VTotal
	1125,								// WORD  MaxVtotal
	1125,								// WORD  MinVtotal
	1125,								// WORD TypVTotal
	5,									// BYTE  MinVSyncWidth
	36,									// BYTE  MinVSyncBackPorch
	0xFFFF, // 0x1101,								// WORD  PowerUpTiming
	0xFFFF, // 0x1101,								// WORD  PowerDownTiming
	0x14,								// WORD AccToPixelVal
	0x0,								// BYTE  SpreadSpectrumCtrl
	0,									// DCLK display timing delay adjustment in ns unit
	Wa_OverDriveLUT_VEGA_1080P,		// Overdrive look up table
	sRGB2rgb_VEGA_1080P,
	Sta_GammaLUT_VEGA_1080P,
	0,									// WORD  Invert_DVS
	0,									// WORD  Invert_DHS
	0,									// WORD  Invert_DCLK
	0,									// WORD  Invert_DEN
	
	0x0,								// WORD  DportSwap
	TRUE,								// WORD  LVDSInterface
	QUAD_BUS,							// WORD  W_LVDS_BusType
	LVDS_EIGHT_BIT_MODE,				// WORD  LvdsPanelMode
	0xFF,								// WORD  Spread_Spect_En
	NO_SWAP,								// WORD	LvdsPosNegSwap
	STPLF_DISPLAY_PAD_DRIVE_1_25,		// WORD PadDrive	
	STPLF_DISPLAY_LVDS_CURRENT_3_5,		// WORD  W_LvdsCurrentCtrl
	OUT_24_48_BIT,						// WORD  W_BitOutput
	STPANEL_LVDS_MAP_STANDARD2,			// WORD W_LVDSSignalMap
	0x0063,								// WORD  W_LvdsClkData
};

extern ROM gmt_PanelData Panel_ST_1080P_VEGA_1080P;

#else
	#define Panel_ST_1080P_VEGA_1080P	(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == ST_1080P_VEGA_1080P)
		#pragma message "Default panel will not bootup unless INCLUDE_ST_1080P_VEGA_1080P is defined!"
	#endif
#endif

