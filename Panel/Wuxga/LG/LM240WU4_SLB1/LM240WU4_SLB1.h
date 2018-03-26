/*
	$Workfile:   LTM240W1_L01.h  $
	$Revision: 1.6 $
	$Date: 2011/11/18 08:14:28 $
*/

#ifdef INCLUDE_LG_WUXGA_LM240WU4_SLB1

#include "..\..\Panel\WUxga\LG\LM240WU4_SLB1\OverdriveLut.h"
#include "..\..\Panel\WUxga\LG\LM240WU4_SLB1\GammaLut.h"

char ROM PanelName_LM240WU4_SLB1[] = "LG_WUXGA_LM240WU4_SLB1";

ROM WORD sRGB2rgb_LM240WU4_SLB1[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};

ROM gmt_PanelData Panel_LG_WUXGA_LM240WU4_SLB1 =
{
	PanelName_LM240WU4_SLB1,
	LG_WUXGA_LM240WU4_SLB1,		// Panel ID (refer to enum defined in panel.h)
	PANEL_WUXGA,						// gmd_PanelType PanelType
	FORMAT_ORG_2D,    // gmd_PanelDataFormat;
	TWO,								// BYTE PixPerClock
	10,									// BYTE  Depth
	1920,								// WORD  Width
	1200,								// WORD  Height
	61,									// BYTE  MaxVFreq
	59,									// BYTE  MinVFreq
	78000L,								// DWORD MaxPClk (KHz)
	2072,								// WORD W_MinHTotal
	2080,								// W_TypHTotal
	2080,								// W_22HTotal
	2080,								// W_33HTotal
	16,									// BYTE  MinHSyncWidth
	72,									// BYTE  MinHSyncBackPorch
	1235,								// W_22VTotal
	1235,								// W_33VTotal
	1237,								// WORD  MaxVtotal
	1233,								// WORD  MinVtotal
	1235,								// WORD TypVTotal
	6,									// BYTE  MinVSyncWidth
	25,									// BYTE  MinVSyncBackPorch
	
	0xCF0C, // 0xFF10,								// WORD  PowerUpTiming
	0x0CCF, // 0x10FF,								// WORD  PowerDownTiming
	0xE,									// WORD AccToPixelVal
	0x3F04,								// WORD  SpreadSpectrumCtrl
	0,									// DCLK display timing delay adjustment in ns unit
	Wa_OverDriveLUT_LM240WU4_SLB1,	// Overdrive look up table
	sRGB2rgb_LM240WU4_SLB1,
	Sta_GammaLUT_LM240WU4_SLB1,
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
	0,      							//WORD  PadDrive;
	FOUR_PT_SIX_MA,						// WORD  W_LvdsCurrentCtrl
	OUT_30_BIT,						       // WORD  W_BitOutput
	STPANEL_LVDS_MAP_STANDARD2,			// WORD W_LVDSSignalMap
	0x0063,								// WORD  W_LvdsClkData
};

extern ROM gmt_PanelData Panel_LG_WUXGA_LM240WU4_SLB1;

#else
	#define Panel_LG_WUXGA_LM240WU4_SLB1	(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == LG_WUXGA_LM240WU4_SLB1)
		#pragma message "Default panel will not bootup unless INCLUDE_LG_WUXGA_LM240WU4_SLB1 is defined!"
	#endif
#endif

