/*
	$Workfile:   VEGA_WQXGA.h  $
	$Revision: 1.2 $
	$Date: 2011/08/19 10:03:49 $
*/

#ifdef INCLUDE_ST_WQXGA_VEGA_WQXGA

#include "..\..\Panel\WQXGA\ST\VEGA_WQXGA\OverdriveLut.h"
#include "..\..\Panel\WQXGA\ST\VEGA_WQXGA\GammaLut.h"

char ROM PanelName_VEGA_WQXGA[] = "ST_WQXGA_VEGA_WQXGA";

ROM WORD sRGB2rgb_VEGA_WQXGA[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};

ROM gmt_PanelData Panel_ST_WQXGA_VEGA_WQXGA =
{
	PanelName_VEGA_WQXGA,
	ST_WQXGA_VEGA_WQXGA,			// Panel ID (refer to enum defined in panel.h)
	PANEL_WQXGA,						// gmd_PanelType PanelType
	FORMAT_ORG_2D,    // gmd_PanelDataFormat;
	FOUR,								// BYTE PixPerClock
	8,									// BYTE  Depth
	2560,								// WORD  Width
	1600,								// WORD  Height
	60,									// BYTE  MaxVFreq
	60,									// BYTE  MinVFreq
	67125L,								// DWORD MaxPClk (KHz)
	2720,								// WORD W_MinHTotal
	2720,								// W_TypHTotal
	2720,								// W_22HTotal
	2720,								// W_33HTotal
	32,									// BYTE  MinHSyncWidth
	80, 								// BYTE  MinHSyncBackPorch
	1646,								// W_22VTotal
	1646,								// W_33VTotal
	1646,								// WORD  MaxVtotal
	1646,								// WORD  MinVtotal
	1646,								// WORD TypVTotal
	6,									// BYTE  MinVSyncWidth
	38,									// BYTE  MinVSyncBackPorch
	0x1101,								// WORD  PowerUpTiming
	0x1101,								// WORD  PowerDownTiming
	0x21,								// WORD AccToPixelVal
	0x0,								// BYTE  SpreadSpectrumCtrl
	0,									// DCLK display timing delay adjustment in ns unit
	Wa_OverDriveLUT_VEGA_WQXGA,		// Overdrive look up table
	sRGB2rgb_VEGA_WQXGA,
	Sta_GammaLUT_VEGA_WQXGA,
	0,									// WORD  Invert_DVS
	0,									// WORD  Invert_DHS
	0,									// WORD  Invert_DCLK
	0,									// WORD  Invert_DEN
	
	0, // 0xFF,								// WORD  DportSwap
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

extern ROM gmt_PanelData Panel_ST_WQXGA_VEGA_WQXGA;

#else
	#define Panel_ST_WQXGA_VEGA_WQXGA	(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == ST_WQXGA_VEGA_WQXGA)
		#pragma message "Default panel will not bootup unless INCLUDE_ST_WQXGA_VEGA_WQXGA is defined!"
	#endif
#endif

