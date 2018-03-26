/*
	$Workfile:   LTM213U4_L01.h  $
	$Revision: 1.3 $
	$Date: 2011/05/11 06:36:08 $
*/

#ifdef INCLUDE_SAMSUNG_UXGA_LTM213U4_L01

#include "..\..\Panel\Uxga\Samsung\LTM213U4_L01\OverdriveLut.h"
#include "..\..\Panel\Uxga\Samsung\LTM213U4_L01\GammaLut.h"

char ROM PanelName_LTM213U4_L01[] = "SAMSUNG_UXGA_LTM213U4_L01";

ROM WORD sRGB2rgb_LTM213U4_L01[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};

ROM gmt_PanelData Panel_SAMSUNG_UXGA_LTM213U4_L01 =
{
	PanelName_LTM213U4_L01,
	SAMSUNG_UXGA_LTM213U4_L01,			// Panel ID (refer to enum defined in panel.h)
	PANEL_UXGA,							// gmd_PanelType PanelType
	FORMAT_ORG_2D,    // gmd_PanelDataFormat;
	TWO,								// BYTE PixPerClock
	8,									// BYTE  Depth
	1600,								// WORD  Width
	1200,								// WORD  Height
	77,									// BYTE  MaxVFreq
	50,									// BYTE  MinVFreq
	81000L,								// DWORD MaxPClk (KHz)
	1644,								// WORD W_MinHTotal
	2160,								// W_TypHTotal
	1800,								// W_22HTotal
	1800,								// W_33HTotal
	8,									// BYTE  MinHSyncWidth
	16,									// BYTE  MinHSyncBackPorch
	1240,								// W_22VTotal
	1238,								// W_33VTotal
	1280,								// WORD  MaxVtotal
	1206,								// WORD  MinVtotal
	1240,								// WORD TypVTotal
	5,									// BYTE  MinVSyncWidth
	15,									// BYTE  MinVSyncBackPorch
	
	0x9696,								// WORD  PowerUpTiming
	0xFFFF,								// WORD  PowerDownTiming
	0xC,								// WORD AccToPixelVal
	0x0,								// BYTE  SpreadSpectrumCtrl
	0,									// DCLK display timing delay adjustment in ns unit
	Wa_OverDriveLUT_LTM213U4_L01,		// Overdrive look up table
	sRGB2rgb_LTM213U4_L01,
	Sta_GammaLUT_LTM213U4_L01,
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
	STPANEL_LVDS_MAP_STANDARD1,			// WORD W_LVDSSignalMap
	0x0063,								// WORD  W_LvdsClkData
};
extern ROM gmt_PanelData Panel_SAMSUNG_UXGA_LTM213U4_L01;

#else
	#define Panel_SAMSUNG_UXGA_LTM213U4_L01	(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == SAMSUNG_UXGA_LTM213U4_L01)
		#pragma message "Default panel will not bootup unless INCLUDE_SAMSUNG_UXGA_LTM213U4_L01 is defined!"
	#endif
#endif
