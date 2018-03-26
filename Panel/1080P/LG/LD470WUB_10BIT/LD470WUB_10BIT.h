/*
	$Workfile:   LD470WUB_10BIT.h  $
	$Revision:   1.0  $
	$Date:   May 12 2008 09:51:18  $		//Peter 10090120
*/
#ifdef INCLUDE_LG_1080P_LD470WUB_10BIT

#include "..\..\Panel\1080P\LG\LD470WUB_10BIT\OverdriveLut.h"
#include "..\..\Panel\1080P\LG\LD470WUB_10BIT\GammaLut.h"


char ROM PanelName_LG_1080P_LD470WUB_10BIT[] = "LG_LD470WUB_10BIT";

ROM WORD sRGB2rgb_LD470WUB_10BIT[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};

//	gm_SetRegBitsWord(LVDS_DIGITAL_CTRL, EVEN_ODD_SWAP);

ROM gmt_PanelData Panel_1080P_LG_LD470WUB_10BIT =
{
	PanelName_LG_1080P_LD470WUB_10BIT,
	LG_1080P_LD470WUB_10BIT,			// Panel ID (refer to enum defined in panel.h)
	PANEL_1080P,						// gmd_PanelType PanelType
	FORMAT_ORG_2D,						// gmd_PanelDataFormat;
	TWO,								// BYTE PixPerClock
	10, 	// Org = 8								// BYTE  Depth
	1920,								// WORD  Width
	1080,								// WORD  Height
	63, 								// BYTE  MaxVFreq
	57, 								// BYTE  MinVFreq
	154000L,							// DWORD MaxPClk (KHz)
	2120,								// WORD W_MinHTotal
	2200,//2200,								// W_TypHTotal
	2200,								// W_22HTotal
	2200,								// W_33HTotal
	8, 								// BYTE  MinHSyncWidth
	16, 								// BYTE  MinHSyncBackPorch
	1090,								// W_22VTotal
	1100,								// W_33VTotal
	1149,								// WORD  MaxVtotal
	1091,								// WORD  MinVtotal
	1125,								// WORD TypVTotal
	6,									// BYTE  MinVSyncWidth
	35, 								// BYTE  MinVSyncBackPorch
	0xff08, 							// WORD  PowerUpTiming
	0x08FA, 							// WORD  PowerDownTiming
	0xF,								// WORD AccToPixelVal
	0x0,								// BYTE  SpreadSpectrumCtrl
	0,									// DCLK display timing delay adjustment in ns unit
	Wa_OverDriveLUT_LD470WUB_10BIT,		// Overdrive look up table
	sRGB2rgb_LD470WUB_10BIT,
	Sta_GammaLUT_LG_LD470WUB_10BIT,
	0,									// WORD  Invert_DVS
	0,									// WORD  Invert_DHS
	0,									// WORD  Invert_DCLK
	0,									// WORD  Invert_DEN
	
	0, // 0xFF, 							// WORD  DportSwap
	TRUE,								// WORD  LVDSInterface
	DUAL_BUS,							// WORD  W_LVDS_BusType
	LVDS_EIGHT_BIT_MODE,				// WORD  LvdsPanelMode
	0xFF,								// WORD  Spread_Spect_En
	NO_SWAP,								// WORD LvdsPosNegSwap
	STPLF_DISPLAY_PAD_DRIVE_1_25,		// WORD PadDrive	
	STPLF_DISPLAY_LVDS_CURRENT_3_5, 	// WORD  W_LvdsCurrentCtrl
	OUT_30_BIT, 						// WORD  W_BitOutput
	STPANEL_LVDS_MAP_STANDARD2, 		// WORD W_LVDSSignalMap
	0x0063, 							// WORD  W_LvdsClkData
};
extern ROM gmt_PanelData Panel_1080P_LG_LD470WUB_10BIT;

#else
	#define Panel_1080P_LG_LD470WUB_10BIT	(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == LG_1080P_LD470WUB_10BIT)
		#pragma message "Default panel will not bootup unless INCLUDE_LG_1080P_LD470WUB_10BIT is defined!"
	#endif
#endif