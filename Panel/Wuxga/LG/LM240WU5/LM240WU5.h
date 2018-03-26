
#ifdef INCLUDE_LPL_WUXGA_LM240WU5

#include "..\..\Panel\WUxga\LG\LM240WU5\OverdriveLut.h"
#include "..\..\Panel\WUxga\LG\LM240WU5\GammaLut.h"

char ROM PanelName_LM240WU5[] = "LPL_WUXGA_LM240WU5";

ROM WORD sRGB2rgb_LM240WU5[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};

ROM gmt_PanelData Panel_LPL_WUXGA_LM240WU5 =
{
	PanelName_LM240WU5,
	LPL_WUXGA_LM240WU5,				// Panel ID (refer to enum defined in panel.h)
	PANEL_WUXGA,						// gmd_PanelType PanelType
	FORMAT_ORG_2D,    // gmd_PanelDataFormat;
	ONE,								// BYTE PixPerClock
	10,									// BYTE  Depth
	1920,								// WORD  Width
	1200,								// WORD  Height
	60,									// BYTE  MaxVFreq
	60,									// BYTE  MinVFreq
	79000L,								// DWORD MaxPClk (KHz)
	2072,								// WORD W_MinHTotal
	2080,								// W_TypHTotal
	2080,								// W_22HTotal
	2080,								// W_33HTotal
	40,									// BYTE  MinHSyncWidth
	72,									// BYTE  MinHSyncBackPorch
	1245,								// W_22VTotal
	1245,								// W_33VTotal
	1247,								// WORD  MaxVtotal
	1243,								// WORD  MinVtotal
	1245,								// WORD TypVTotal
	6,									// BYTE  MinVSyncWidth
	35,									// BYTE  MinVSyncBackPorch
	
	0xFF30,// 0x1101,								// WORD  PowerUpTiming
	0xFF30,// 0x1101,								// WORD  PowerDownTiming
	0xE,								// WORD AccToPixelVal
	0x33,//0x0,								// BYTE  SpreadSpectrumCtrl
	0,									// DCLK display timing delay adjustment in ns unit
	Wa_OverDriveLUT_LM240WU5,		// Overdrive look up table
	sRGB2rgb_LM240WU5,
	Sta_GammaLUT_LM240WU5,
	0,									// WORD  Invert_DVS
	0,									// WORD  Invert_DHS
	0,									// WORD  Invert_DCLK
	0,									// WORD  Invert_DEN
	0,									// WORD  DportSwap
	TRUE,								// WORD  LVDSInterface
	DUAL_BUS,							// WORD  W_LVDS_BusType
	LVDS_EIGHT_BIT_MODE,					// WORD  LvdsPanelMode
	0xFF,								// WORD  Spread_Spect_En
	NO_SWAP,							// WORD	LvdsPosNegSwap
	STPLF_DISPLAY_PAD_DRIVE_1_25,      							//WORD  PadDrive;
	STPLF_DISPLAY_LVDS_CURRENT_3_5,					// WORD  W_LvdsCurrentCtrl
	OUT_30_BIT,							// WORD  W_BitOutput
	STPANEL_LVDS_MAP_STANDARD1,			// WORD W_LVDSSignalMap
	0x0063,								// WORD  W_LvdsClkData
};

extern ROM gmt_PanelData Panel_LPL_WUXGA_LM240WU5;

#else
	#define Panel_LPL_WUXGA_LM240WU5	(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == LPL_WUXGA_LM240WU5)
		#pragma message "Default panel will not bootup unless INCLUDE_LPL_WUXGA_LM240WU5 is defined!"
	#endif
#endif

