/*
	$Workfile:   LD420WUB_4K2K.h  $
	$Revision:   1.0  $
	$Date:   May 12 2008 09:51:18  $		//Peter 10090120
*/
#ifdef INCLUDE_LG_1080P_LD420WUB_4K2K

#include "..\..\Panel\1080P\LG\LD420WUB_4K2K\OverdriveLut.h"
#include "..\..\Panel\1080P\LG\LD420WUB_4K2K\GammaLut.h"


char ROM PanelName_LG_1080P_LD420WUB_4K2K[] = "LG_LD420WUB_4K2K";

ROM WORD sRGB2rgb_LD420WUB_4K2K[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};

//	gm_SetRegBitsWord(LVDS_DIGITAL_CTRL, EVEN_ODD_SWAP);

ROM gmt_PanelData Panel_1080P_LG_LD420WUB_4K2K =
{
	PanelName_LG_1080P_LD420WUB_4K2K,
	LG_1080P_LD420WUB_4K2K,			// Panel ID (refer to enum defined in panel.h)
	PANEL_1080P,						// gmd_PanelType PanelType
	FORMAT_ORG_2D,						// gmd_PanelDataFormat;
	TWO,								// BYTE PixPerClock
	10, 	// Org = 8								// BYTE  Depth
	1920,								// WORD  Width
	1080,								// WORD  Height
	60, 								// BYTE  MaxVFreq
	60, 								// BYTE  MinVFreq
	74250L,//74250L,//148500UL,//154000L,							// DWORD MaxPClk (KHz)
	2200,								// WORD W_MinHTotal
	2200,//2200,								// W_TypHTotal
	2200,								// W_22HTotal
	2200,								// W_33HTotal
	8, 								// BYTE  MinHSyncWidth
	16, 								// BYTE  MinHSyncBackPorch
	1125,								// W_22VTotal
	1125,								// W_33VTotal
	1125,								// WORD  MaxVtotal
	1125,								// WORD  MinVtotal
	1125,								// WORD TypVTotal
	6,									// BYTE  MinVSyncWidth
	35, 								// BYTE  MinVSyncBackPorch
	0xff08, 							// WORD  PowerUpTiming
	0x08FA, 							// WORD  PowerDownTiming
	0xF,								// WORD AccToPixelVal
	0x0,								// BYTE  SpreadSpectrumCtrl
	0,									// DCLK display timing delay adjustment in ns unit
	Wa_OverDriveLUT_LD420WUB_4K2K,		// Overdrive look up table
	sRGB2rgb_LD420WUB_4K2K,
	Sta_GammaLUT_LG_LD420WUB_4K2K,
	1,									// WORD  Invert_DVS		//Hyb130922* for 4K2K 1080P output image slaver image not stable
	0,									// WORD  Invert_DHS
	0,									// WORD  Invert_DCLK
	0,									// WORD  Invert_DEN
	
	0, // 0xFF, 							// WORD  DportSwap
	TRUE,								// WORD  LVDSInterface
	QUAD_BUS,							// WORD  W_LVDS_BusType
	LVDS_EIGHT_BIT_MODE,				// WORD  LvdsPanelMode
	0x00,								// WORD  Spread_Spect_En
	NO_SWAP,								// WORD LvdsPosNegSwap
	STPLF_DISPLAY_PAD_DRIVE_1_35,		// WORD PadDrive	
	STPLF_DISPLAY_LVDS_CURRENT_4_0, 	// WORD  W_LvdsCurrentCtrl
	OUT_24_48_BIT, 						// WORD  W_BitOutput
	STPANEL_LVDS_MAP_STANDARD1, 		// WORD W_LVDSSignalMap
	0x0063, 							// WORD  W_LvdsClkData
};
extern ROM gmt_PanelData Panel_1080P_LG_LD420WUB_4K2K;

#else
	#define Panel_1080P_LG_LD420WUB_4K2K	(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == LG_1080P_LD420WUB_4K2K)
		#pragma message "Default panel will not bootup unless INCLUDE_LG_1080P_LD420WUB_4K2K is defined!"
	#endif
#endif
