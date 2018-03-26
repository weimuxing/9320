/*
	$Workfile:   LTM240W1_L01.h  $
	$Revision:   1.8  $
	$Date:   Jun 22 2006 00:44:16  $
*/

#ifdef INCLUDE_SAMSUNG_1080P_LTM230HT03

#include "..\..\Panel\1080P\Samsung\LTM230HT03\OverdriveLut.h"
#include "..\..\Panel\1080P\Samsung\LTM230HT03\GammaLut.h"

char ROM PanelName_LTM230HT03[] = "SAMSUNG_1080P_LTM230HT03";

ROM WORD sRGB2rgb_LTM230HT03[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};

ROM gmt_PanelData Panel_SAMSUNG_1080P_LTM230HT03 =
{
	PanelName_LTM230HT03,
	SAMSUNG_1080P_LTM230HT03,		// Panel ID (refer to enum defined in panel.h)
	PANEL_1080P,						// gmd_PanelType PanelType
	TWO,								// BYTE PixPerClock
	8,									// BYTE  Depth
	1920,								// WORD  Width
	1080,								// WORD  Height
	75,									// BYTE  MaxVFreq
	49,									// BYTE  MinVFreq
	174408UL,							// DWORD MaxPClk (KHz)
	1980,								// WORD W_MinHTotal
	2020,								// W_TypHTotal
	2020,								// W_22HTotal
	2020,								// W_33HTotal
	16,		 							// BYTE  MinHSyncWidth
	32,									// BYTE  MinHSyncBackPorch
	1111,								// W_22VTotal
	1111,								// W_33VTotal
	1118,								// WORD  MaxVtotal
	1105,								// WORD  MinVtotal
	1111,								// WORD TypVTotal
	2,									// BYTE  MinVSyncWidth
	5,									// BYTE  MinVSyncBackPorch
	0x775522L,							// DWORD PadDrive
	0x9C15,								// WORD  PowerUpTiming
	0x059C,								// WORD  PowerDownTiming  //High Byte T3 : Panel Power -> Data
	0xE,									// WORD AccToPixelVal
	0x3F03,								// WORD  SpreadSpectrumCtrl
	0,									// DCLK display timing delay adjustment in ns unit
	Wa_OverDriveLUT_LTM230HT03,	// Overdrive look up table
	sRGB2rgb_LTM230HT03,
	Sta_GammaLUT_LTM230HT03,
	0,									// WORD  Invert_DVS
	0,									// WORD  Invert_DHS
	0,									// WORD  Invert_DCLK
	0,									// WORD  Invert_DEN
	0x00,								// WORD  DportSwap
	TRUE,								// WORD  LVDSInterface
	DUAL_BUS,							// WORD  W_LVDS_BusType
	LVDS_EIGHT_BIT_MODE,				// WORD  LvdsPanelMode
	0xFF,								// WORD  Spread_Spect_En
	NO_SWAP,							// WORD	LvdsPosNegSwap
	SEVEN_PT_ONE_MA,					// WORD  W_LvdsCurrentCtrl
	OUT_24_48_BIT,						// WORD  W_BitOutput
	0x0063,								// WORD  W_LvdsClkData
};

extern ROM gmt_PanelData Panel_SAMSUNG_1080P_LTM230HT03;

#else
	#define Panel_SAMSUNG_1080P_LTM230HT03	(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == SAMSUNG_1080P_LTM230HT03)
		#pragma message "Default panel will not bootup unless INCLUDE_SAMSUNG_1080P_LTM230HT03 is defined!"
	#endif
#endif

