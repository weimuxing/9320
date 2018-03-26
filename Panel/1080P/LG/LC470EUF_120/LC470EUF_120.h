/*
	$Workfile:   LC470EUF_120.h  $
	$Revision: 1.1 $
	$Date: 2012/02/01 05:58:46 $
*/

#ifdef INCLUDE_LG_1080P_LC470EUF_120

#include "..\..\Panel\1080P\LG\LC470EUF_120\OverdriveLut.h"
#include "..\..\Panel\1080P\LG\LC470EUF_120\GammaLut.h"

char ROM PanelName_LC470EUF_120[] = "LG_1080P_LC470EUF_120";

ROM WORD sRGB2rgb_LC470EUF_120[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};

ROM gmt_PanelData Panel_LG_1080P_LC470EUF_120 =
{
	PanelName_LC470EUF_120,
	LG_1080P_LC470EUF_120,			// Panel ID (refer to enum defined in panel.h)
	PANEL_1080P,						// gmd_PanelType PanelType
	FORMAT_3D_LINE_ALTERNATIVE,    					// gmd_PanelDataFormat;
	FOUR,								// BYTE PixPerClock
	8,									// BYTE  Depth
	1920,								// WORD  Width
	1080,								// WORD  Height
	120,									// BYTE  MaxVFreq
	60,									// BYTE  MinVFreq
	148500L,							// DWORD MaxPClk (KHz)
	2080,								// WORD W_MinHTotal
	2080,//2200,								// W_TypHTotal
	2700,								// W_22HTotal
	2700,								// W_33HTotal
	40,									// BYTE  MinHSyncWidth
	40, 								// BYTE  MinHSyncBackPorch
	1090,								// W_22VTotal
	1100,								// W_33VTotal
	1580,								// WORD  MaxVtotal
	1580,								// WORD  MinVtotal
	1580,								// WORD TypVTotal
	6,									// BYTE  MinVSyncWidth
	45,									// BYTE  MinVSyncBackPorch
	0x1101,								// WORD  PowerUpTiming
	0x1101,								// WORD  PowerDownTiming
	0x14,								// WORD AccToPixelVal
	0x0,								// BYTE  SpreadSpectrumCtrl
	0,									// DCLK display timing delay adjustment in ns unit
	Wa_OverDriveLUT_LC470EUF_120,		// Overdrive look up table
	sRGB2rgb_LC470EUF_120,
	Sta_GammaLUT_LC470EUF_120,
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
	OUT_30_BIT, 						// WORD  W_BitOutput
	STPANEL_LVDS_MAP_STANDARD2,			// WORD W_LVDSSignalMap
	0x0063,								// WORD  W_LvdsClkData
};

extern ROM gmt_PanelData Panel_LG_1080P_LC470EUF_120;

#else
	#define Panel_LG_1080P_LC470EUF_120	(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == LG_1080P_LC470EUF_120)
		#pragma message "Default panel will not bootup unless INCLUDE_LG_1080P_LC470EUF_120 is defined!"
	#endif
#endif

