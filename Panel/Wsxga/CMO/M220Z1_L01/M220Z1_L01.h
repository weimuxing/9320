/*
	$Workfile:   M220Z1_L01.h  $
	$Revision: 1.5 $
	$Date: 2011/09/16 02:52:15 $
*/

#ifdef INCLUDE_CMO_WSXGA_M220Z1_L01

#include "..\..\Panel\Wsxga\CMO\M220Z1_L01\OverdriveLut.h"
#include "..\..\Panel\Wsxga\CMO\M220Z1_L01\GammaLut.h"

char ROM PanelName_M220Z1_L01[] = "CMO_WSXGA_M220Z1_L01";

ROM WORD sRGB2rgb_M220Z1_L01[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};

ROM gmt_PanelData Panel_CMO_WSXGA_M220Z1_L01 =
{
	PanelName_M220Z1_L01,
	CMO_WSXGA_M220Z1_L01,			// Panel ID (refer to enum defined in panel.h)
	PANEL_WSXGA,						// gmd_PanelType PanelType
	FORMAT_ORG_2D,    					// gmd_PanelDataFormat;
	TWO,								// BYTE PixPerClock
	8,									// BYTE  Depth

	1680,								// WORD  Width
	1050,								// WORD  Height
	60,									// BYTE  MaxVFreq
	60,									// BYTE  MinVFreq
	148500L,							// DWORD MaxPClk (KHz)
	1840, // 2080,								// WORD W_MinHTotal
	1840,								// W_TypHTotal
	1840, // 2700,								// W_22HTotal
	1840, // 2700,								// W_33HTotal
	40,									// BYTE  MinHSyncWidth
	40, 								// BYTE  MinHSyncBackPorch
	1180, // 1100,								// W_22VTotal
	1180, // 1100,								// W_33VTotal
   1180, // 1580,								// WORD  MaxVtotal
   1180, // 1580,								// WORD  MinVtotal
   1180, // 1580,								// WORD TypVTotal
	6,									// BYTE  MinVSyncWidth
	14, // 45,									// BYTE  MinVSyncBackPorch
	0xCF0C, // 0x1101,								// WORD  PowerUpTiming
	0xCF0C, // 0x1101,								// WORD  PowerDownTiming
	0x14,								// WORD AccToPixelVal
	0x0,								// BYTE  SpreadSpectrumCtrl
	0,									// DCLK display timing delay adjustment in ns unit
		Wa_OverDriveLUT_M220Z1_L01, 	// Overdrive look up table
		sRGB2rgb_M220Z1_L01,
		Sta_GammaLUT_M220Z1_L01,

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
	STPLF_DISPLAY_PAD_DRIVE_1_25,		// WORD PadDrive	
	STPLF_DISPLAY_LVDS_CURRENT_3_5,		// WORD  W_LvdsCurrentCtrl
	OUT_24_48_BIT,						// WORD  W_BitOutput
	STPANEL_LVDS_MAP_STANDARD2,			// WORD W_LVDSSignalMap
	0x0063,								// WORD  W_LvdsClkData
};

extern ROM gmt_PanelData Panel_CMO_WSXGA_M220Z1_L01;

#else
	#define Panel_CMO_WSXGA_M220Z1_L01	(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == CMO_WSXGA_M220Z1_L01)
		#pragma message "Default panel will not bootup unless INCLUDE_CMO_WSXGA_M220Z1_L01 is defined!"
	#endif
#endif

