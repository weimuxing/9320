/*
	$Workfile:   LM260WU2_SLA1.h  $
	$Revision: 1.3 $
	$Date: 2011/05/11 06:36:09 $
*/

#ifdef INCLUDE_LG_WUXGA_LM260WU2_SLA1

#include "..\..\Panel\WUxga\LG\LM260WU2_SLA1\OverdriveLut.h"
#include "..\..\Panel\WUxga\LG\LM260WU2_SLA1\GammaLut.h"

char ROM PanelName_LM260WU2_SLA1[] = "LG_WUXGA__LM260WU2_SLA1";

ROM WORD sRGB2rgb_LM260WU2_SLA1[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};

ROM gmt_PanelData Panel_LG_WUXGA_LM260WU2_SLA1 =
{
	PanelName_LM260WU2_SLA1,
	LG_WUXGA_LM260WU2_SLA1,			// Panel ID (refer to enum defined in panel.h)
	PANEL_WUXGA,						// gmd_PanelType PanelType
	FORMAT_ORG_2D,    // gmd_PanelDataFormat;
	TWO,								// BYTE PixPerClock
	8,									// BYTE  Depth
	1920,								// WORD  Width
	1200,								// WORD  Height
	61,									// BYTE  MaxVFreq
	59,									// BYTE  MinVFreq
	156000L,								// DWORD MaxPClk (KHz)
	2072,								// WORD W_MinHTotal
	2080,								// W_TypHTotal
	2088,								// W_22HTotal
	2088,								// W_33HTotal
	32,									// BYTE  MinHSyncWidth
	72,								// BYTE  MinHSyncBackPorch
	1235,								// W_22VTotal
	1235,								// W_33VTotal
	1237,								// WORD  MaxVtotal
	1233,								// WORD  MinVtotal
	1235,								// WORD TypVTotal
	6,									// BYTE  MinVSyncWidth
	25,									// BYTE  MinVSyncBackPorch
	
	0x1101,								// WORD  PowerUpTiming
	0x1101,								// WORD  PowerDownTiming
	0xE,								// WORD AccToPixelVal
	0x0,								// BYTE  SpreadSpectrumCtrl
	0,									// DCLK display timing delay adjustment in ns unit
	Wa_OverDriveLUT_LM260WU2_SLA1,		// Overdrive look up table
	sRGB2rgb_LM260WU2_SLA1,
	Sta_GammaLUT_LM260WU2_SLA1,
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

extern ROM gmt_PanelData Panel_LG_WUXGA_LM260WU2_SLA1;

#else
	#define Panel_LG_WUXGA_LM260WU2_SLA1	(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == LG_WUXGA_LM260WU2_SLA1)
		#pragma message "Default panel will not bootup unless INCLUDE_LG_LM260WU2_SLA1 is defined!"
	#endif
#endif

