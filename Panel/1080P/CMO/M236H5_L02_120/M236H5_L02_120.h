/*
	$Workfile:   M236H5_L02_120.h  $
	$Revision: 1.10 $
	$Date: 2013/01/29 10:07:47 $
*/

#ifdef INCLUDE_CMO_1080P_M236H5_L02_120

#include "..\..\Panel\1080P\CMO\M236H5_L02_120\OverdriveLut.h"
//#include "..\..\Panel\1080P\CMO\M236H5_L02_120\GammaLut.h" // use 60Hz table


char ROM PanelName_M236H5_L02_120[] = "CMO_1080P_M236H5_L02_120";

ROM WORD sRGB2rgb_M236H5_L02_120[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};

ROM gmt_PanelData Panel_CMO_1080P_M236H5_L02_120 =
{
	PanelName_M236H5_L02_120,
	CMO_1080P_M236H5_L02_120,			// Panel ID (refer to enum defined in panel.h)
	PANEL_1080P,						// gmd_PanelType PanelType
	FORMAT_3D_FRAME_SEQUENTIAL,    					// gmd_PanelDataFormat;
	FOUR,								// BYTE PixPerClock
	8,									// BYTE  Depth

	1920,								// WORD  Width
	1080,								// WORD  Height
	120,									// BYTE  MaxVFreq
	60,									// BYTE  MinVFreq
	148500L,							// DWORD MaxPClk (KHz)
	2000, // 2080,								// WORD W_MinHTotal
	2200,								// W_TypHTotal
	2248, // 2700,								// W_22HTotal
	2248, // 2700,								// W_33HTotal
	40,									// BYTE  MinHSyncWidth
	40, 								// BYTE  MinHSyncBackPorch
	1180, // 1100,								// W_22VTotal
	1180, // 1100,								// W_33VTotal
   1180, // 1580,								// WORD  MaxVtotal
   1100, // 1580,								// WORD  MinVtotal
   1125, // 1580,								// WORD TypVTotal
	6,									// BYTE  MinVSyncWidth
	14, // 45,									// BYTE  MinVSyncBackPorch
	0xCF0C, // 0x1101,								// WORD  PowerUpTiming
	0xCF0C, // 0x1101,								// WORD  PowerDownTiming
	0x14,								// WORD AccToPixelVal
	0x0,								// BYTE  SpreadSpectrumCtrl
	0,									// DCLK display timing delay adjustment in ns unit
	Wa_OverDriveLUT_M236H5_L02_120,		// Overdrive look up table
	sRGB2rgb_M236H5_L02_120,
	//Sta_GammaLUT_M236H5_L02_120,
	Sta_GammaLUT_M236H5_L02_60,
	0,									// WORD  Invert_DVS
	0,									// WORD  Invert_DHS
	0,									// WORD  Invert_DCLK
	0,									// WORD  Invert_DEN
	
	0xFF,								// WORD  DportSwap
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

extern ROM gmt_PanelData Panel_CMO_1080P_M236H5_L02_120;

#else
	#define Panel_CMO_1080P_M236H5_L02_120	(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == CMO_1080P_M236H5_L02_120)
		#pragma message "Default panel will not bootup unless INCLUDE_CMO_1080P_M236H5_L02_120 is defined!"
	#endif
#endif

