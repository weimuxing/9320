/*
	$Workfile:   M236H5_L02_60.h  $
	$Revision: 1.9 $
	$Date: 2011/11/18 08:14:28 $
*/

#ifdef INCLUDE_CMO_1080P_M236H5_L02_60

#include "..\..\Panel\1080P\CMO\M236H5_L02_60\OverdriveLut.h"
#include "..\..\Panel\1080P\CMO\M236H5_L02_60\GammaLut.h"

char ROM PanelName_M236H5_L02_60[] = "CMO_1080P_M236H5_L02_60";

ROM WORD sRGB2rgb_M236H5_L02_60[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};

ROM gmt_PanelData Panel_CMO_1080P_M236H5_L02_60 =
{
	PanelName_M236H5_L02_60,
	CMO_1080P_M236H5_L02_60,			// Panel ID (refer to enum defined in panel.h)
	PANEL_1080P,						// gmd_PanelType PanelType
	FORMAT_ORG_2D,    					// gmd_PanelDataFormat;
	FOUR, // TWO,								// BYTE PixPerClock
	8,									// BYTE  Depth
	1920,								// WORD  Width
	1080,								// WORD  Height
	122, // 60,									// BYTE  MaxVFreq
	58,									// BYTE  MinVFreq
	161800L, 							// DWORD MaxPClk (KHz)
	2000,								// WORD W_MinHTotal
	2200,								// W_TypHTotal
	2248,								// W_22HTotal
	2248,								// W_33HTotal
	44,									// BYTE  MinHSyncWidth
	36,  								// BYTE  MinHSyncBackPorch
	1180,								// W_22VTotal
	1180,								// W_33VTotal
	1180,								// WORD  MaxVtotal
	1100,								// WORD  MinVtotal
	1125,								// WORD TypVTotal
	5,									// BYTE  MinVSyncWidth
	15, 									// BYTE  MinVSyncBackPorch
	0xCF0C, // 0xFFFF, 								// WORD  PowerUpTiming
	0x0CCF, // 0xFFFF, 								// WORD  PowerDownTiming
	0x14,								// WORD AccToPixelVal
	0x0,								// BYTE  SpreadSpectrumCtrl
	0,									// DCLK display timing delay adjustment in ns unit
	Wa_OverDriveLUT_M236H5_L02_60,		// Overdrive look up table
	sRGB2rgb_M236H5_L02_60,
	Sta_GammaLUT_M236H5_L02_60,
	0,									// WORD  Invert_DVS
	0,									// WORD  Invert_DHS
	0,									// WORD  Invert_DCLK
	0,									// WORD  Invert_DEN
	
	0xFF,								// WORD  DportSwap
	TRUE,								// WORD  LVDSInterface
	QUAD_BUS, // DUAL_BUS,							// WORD  W_LVDS_BusType
	LVDS_EIGHT_BIT_MODE,				// WORD  LvdsPanelMode
	0xFF,								// WORD  Spread_Spect_En
	NO_SWAP,								// WORD	LvdsPosNegSwap
	STPLF_DISPLAY_PAD_DRIVE_1_25,		// WORD PadDrive	
	STPLF_DISPLAY_LVDS_CURRENT_3_5,		// WORD  W_LvdsCurrentCtrl
	OUT_24_48_BIT,						// WORD  W_BitOutput
	STPANEL_LVDS_MAP_CUSTOM, // STPANEL_LVDS_MAP_STANDARD2,			// WORD W_LVDSSignalMap
	0x0063,								// WORD  W_LvdsClkData
};

extern ROM gmt_PanelData Panel_CMO_1080P_M236H5_L02_60;

#else
	#define Panel_CMO_1080P_M236H5_L02_60	(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == CMO_1080P_M236H5_L02_60)
		#pragma message "Default panel will not bootup unless INCLUDE_CMO_1080P_M236H5_L02_60 is defined!"
	#endif
#endif

