/*
	$Workfile:   LM300WQ6_SLA1.h  $
	$Revision: 1.6 $
	$Date: 2011/06/17 04:37:16 $
*/

#ifdef INCLUDE_LG_WQXGA_LM300WQ6_SLA1

#include "..\..\Panel\WQXGA\LG\LM300WQ6_SLA1\OverdriveLut.h"
#include "..\..\Panel\WQXGA\LG\LM300WQ6_SLA1\GammaLut.h"

char ROM PanelName_LM300WQ6_SLA1[] = "LG_WQXGA_LM300WQ6_SLA1";

ROM WORD sRGB2rgb_LM300WQ6_SLA1[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};

ROM gmt_PanelData Panel_LG_WQXGA_LM300WQ6_SLA1 =
{
	PanelName_LM300WQ6_SLA1,
	LG_WQXGA_LM300WQ6_SLA1,			// Panel ID (refer to enum defined in panel.h)
	PANEL_WQXGA,						// gmd_PanelType PanelType
	FORMAT_ORG_2D,    // gmd_PanelDataFormat;
	FOUR,								// BYTE PixPerClock
	10,									// BYTE  Depth
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
									// BYTE  MinVSyncBackPorch
	0xE00A,	//0x1101,								// WORD  PowerUpTiming
	0x0A60,	//0x0060,	//0x1101,								// WORD  PowerDownTiming
	0x21,								// WORD AccToPixelVal
	0x3F6, //0x3FA,	//0x3F4,	//0x0,								// BYTE  SpreadSpectrumCtrl
	0,									// DCLK display timing delay adjustment in ns unit
	Wa_OverDriveLUT_LM300WQ6_SLA1,		// Overdrive look up table
	sRGB2rgb_LM300WQ6_SLA1,
	Sta_GammaLUT_LM300WQ6_SLA1,
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
	STPANEL_LVDS_MAP_CUSTOM,			// WORD W_LVDSSignalMap
	0x0063,								// WORD  W_LvdsClkData
};

extern ROM gmt_PanelData Panel_LG_WQXGA_LM300WQ6_SLA1;

#else
	#define Panel_LG_WQXGA_LM300WQ6_SLA1	(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == LG_WQXGA_LM300WQ6_SLA1)
		#pragma message "Default panel will not bootup unless INCLUDE_LG_WQXGA_LM300WQ6_SLA1 is defined!"
	#endif
#endif

