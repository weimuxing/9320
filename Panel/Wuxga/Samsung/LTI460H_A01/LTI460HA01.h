/*
	$Workfile:   LTI460H_A01.h  $
	$Revision:   1.10  $
	$Date:   Dec 12 2008 15:55:32  $
*/

#ifdef INCLUDE_SAMSUNG_1080P_LTI460HA01

#include "..\..\Panel\WUxga\Samsung\LTI460H_A01\OverdriveLut.h"
#include "..\..\Panel\WUxga\Samsung\LTI460H_A01\GammaLut.h"

char ROM PanelName_LTI460HA01[] = "SAMSUNG_1080P_LTI460HA01";

ROM WORD sRGB2rgb_LTI460HA01[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};

ROM gmt_PanelData Panel_SAMSUNG_1080P_LTI460HA01 =
{
	PanelName_LTI460HA01,
	SAMSUNG_1080P_LTI460HA01,			// Panel ID (refer to enum defined in panel.h)
	PANEL_1080P,						// gmd_PanelType PanelType
	TWO,								// BYTE PixPerClock
	8,									// BYTE  Depth
	1920,								// WORD  Width
	1080,								// WORD  Height
	60,									// BYTE  MaxVFreq
	56,									// BYTE  MinVFreq
	77500L,								// DWORD MaxPClk (KHz)
	2090,								// WORD W_MinHTotal
	2200,								// W_TypHTotal
	2200,								// W_22HTotal
	2200,								// W_33HTotal
	16,									// BYTE  MinHSyncWidth
	120,								// BYTE  MinHSyncBackPorch
	1240,								// W_22VTotal
	1238,								// W_33VTotal
	1380,								// WORD  MaxVtotal
	1092,								// WORD  MinVtotal
	1125,								// WORD TypVTotal
	3,									// BYTE  MinVSyncWidth
	33,	//Bossen, fix the under line noise.	43,									// BYTE  MinVSyncBackPorch
	0x775522L,							// DWORD PadDrive
	0xFA0C,	//Bossen, for SIT Board.	20090917//0xFA09,								// WORD  PowerUpTiming
	0x0220,								// WORD  PowerDownTiming
	0xE,								// WORD AccToPixelVal
	0x1203,								//Bossen, spectrume value from TPV.		20090624.	// BYTE  SpreadSpectrumCtrl
	0,									// DCLK display timing delay adjustment in ns unit
	Wa_OverDriveLUT_LTI460H_A01,		// Overdrive look up table
	sRGB2rgb_LTI460HA01,
	Sta_GammaLUT_LTI460H_A01,
	0,									// WORD  Invert_DVS
	0,									// WORD  Invert_DHS
	0,									// WORD  Invert_DCLK
	0,									// WORD  Invert_DEN
	0,									// WORD  DportSwap
	TRUE,								// WORD  LVDSInterface
	DUAL_BUS,							// WORD  W_LVDS_BusType
	LVDS_EIGHT_BIT_MODE,				// WORD  LvdsPanelMode
	0xFF,								// WORD  Spread_Spect_En
	NO_SWAP,								// WORD	LvdsPosNegSwap
	FOUR_PT_SIX_MA,			//Bossen, Follow TPV FQ request.	20091028.	//Bossen,	follow TPV request.	20091022.	SEVEN_PT_ONE_MA,					// WORD  W_LvdsCurrentCtrl
	OUT_24_48_BIT,						// WORD  W_BitOutput
	0x0063,								// WORD  W_LvdsClkData
};

extern ROM gmt_PanelData Panel_SAMSUNG_1080P_LTI460HA01;

#else
	#define Panel_SAMSUNG_1080P_LTI460HA01	(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == SAMSUNG_1080P_LTI460HA01)
		#pragma message "Default panel will not bootup unless INCLUDE_SAMSUNG_1080P_LTI460HA01 is defined!"
	#endif
#endif
