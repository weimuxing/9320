/*
	$Workfile:   VEGA_WQHD.h  $
	$Revision: 1.1 $
	$Date: 2011/09/08 06:56:13 $
*/

#ifdef INCLUDE_EH_QFHD_4K2K

#include "..\..\Panel\4K2K\EH\EH_QFHD\OverdriveLut.h"
#include "..\..\Panel\4K2K\EH\EH_QFHD\GammaLut.h"

char ROM PanelName_EH_QFHD[] = "EH_QFHD_4K2K";

ROM WORD sRGB2rgb_EH_QFHD[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};

ROM gmt_PanelData Panel_EH_QFHD =
#if 1
{
	PanelName_EH_QFHD,
	EH_QFHD_4K2K,			// Panel ID (refer to enum defined in panel.h)
	PANEL_2K2K,						// gmd_PanelType PanelType
	FORMAT_ORG_2D,    // gmd_PanelDataFormat;
	FOUR,								// BYTE PixPerClock
	10,									// BYTE  Depth
	3840,								// WORD  Width
	2160,								// WORD  Height
	30, //30,									// BYTE  MaxVFreq
	30,//30,									// BYTE  MinVFreq
	297000L,//297000L,								// DWORD MaxPClk (KHz)  
	4400, //4400,	//4090	//Typ: 4400							// WORD W_MinHTotal
	4400,								// W_TypHTotal
	4400,								// W_22HTotal
	4400,								// W_33HTotal
	48, //80,		//80// 32						// BYTE  MinHSyncWidth
	80, 		//160//80						// BYTE  MinHSyncBackPorch
	2250,	//2250							// W_22VTotal
	2250,								// W_33VTotal
	2250,								// WORD  MaxVtotal
	2250,								// WORD  MinVtotal
	2250,								// WORD TypVTotal
	8,		// 3								// BYTE  MinVSyncWidth
	10,		// 7							// BYTE  MinVSyncBackPorch
	0x1101,								// WORD  PowerUpTiming
	0x1101,								// WORD  PowerDownTiming
	0x21,								// WORD AccToPixelVal
	0x0,								// BYTE  SpreadSpectrumCtrl
	0,									// DCLK display timing delay adjustment in ns unit
	0, //Wa_OverDriveLUT_EH_4K2K,		// Overdrive look up table
	0,//sRGB2rgb_EH_QFHD,
	0,//Sta_GammaLUT_EH_4K2K,
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
	STPANEL_LVDS_MAP_STANDARD1,//STPANEL_LVDS_MAP_STANDARD2,			// WORD W_LVDSSignalMap
	0x0063,								// WORD  W_LvdsClkData
};

#else
{
	PanelName_EH_QFHD,
	EH_QFHD_4K2K,			// Panel ID (refer to enum defined in panel.h)
	PANEL_2K2K,						// gmd_PanelType PanelType
	FORMAT_ORG_2D,    // gmd_PanelDataFormat;
	FOUR,								// BYTE PixPerClock
	10,									// BYTE  Depth
	3840,								// WORD  Width
	2160,								// WORD  Height
	30, //30,									// BYTE  MaxVFreq
	30,//30,									// BYTE  MinVFreq
	297000L,//297000L,								// DWORD MaxPClk (KHz)  
	4400, //4400,	//4090	//Typ: 4400							// WORD W_MinHTotal
	4400,								// W_TypHTotal
	4400,								// W_22HTotal
	4400,								// W_33HTotal
	80, //80,		//80// 32						// BYTE  MinHSyncWidth
	160, 		//160//80						// BYTE  MinHSyncBackPorch
	2250,	//2250							// W_22VTotal
	2250,								// W_33VTotal
	2250,								// WORD  MaxVtotal
	2250,								// WORD  MinVtotal
	2250,								// WORD TypVTotal
	8,		// 3								// BYTE  MinVSyncWidth
	20,		// 7							// BYTE  MinVSyncBackPorch
	0x1101,								// WORD  PowerUpTiming
	0x1101,								// WORD  PowerDownTiming
	0x21,								// WORD AccToPixelVal
	0x0,								// BYTE  SpreadSpectrumCtrl
	0,									// DCLK display timing delay adjustment in ns unit
	0, //Wa_OverDriveLUT_EH_4K2K,		// Overdrive look up table
	0,//sRGB2rgb_EH_QFHD,
	0,//Sta_GammaLUT_EH_4K2K,
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
	STPANEL_LVDS_MAP_STANDARD1,//STPANEL_LVDS_MAP_STANDARD2,			// WORD W_LVDSSignalMap
	0x0063,								// WORD  W_LvdsClkData
};
#endif
extern ROM gmt_PanelData Panel_EH_QFHD;

#else
	#define Panel_EH_QFHD	(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == EH_QFHD_4K2K)
		#pragma message "Default panel will not bootup unless INCLUDE_EH_QFHD_4K2K is defined!"
	#endif
#endif

