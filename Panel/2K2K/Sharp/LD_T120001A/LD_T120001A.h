/*
	$Workfile:   LD_T120001A.h  $
	$Revision: 1.2 $
	$Date: 2013/01/29 10:07:48 $
*/

#ifdef INCLUDE_CMO_2K2K_LD_T120001A

#include "..\..\Panel\2K2K\Sharp\LD_T120001A\OverdriveLut.h"
#include "..\..\Panel\2K2K\Sharp\LD_T120001A\GammaLut.h"


char ROM PanelName_LD_T120001A[] = "LD_T120001A";

ROM WORD sRGB2rgb_LD_T120001A[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};

ROM gmt_PanelData Panel_2K2K_LD_T120001A =
{
	PanelName_LD_T120001A,
	LD_T120001A,						// Panel ID (refer to enum defined in panel.h)
	PANEL_2K2K,							// gmd_PanelType PanelType
	FORMAT_ORG_2D,    					// gmd_PanelDataFormat;
	FOUR,								// BYTE PixPerClock
	8,									// BYTE  Depth
	1920,								// WORD  Width
	2160,								// WORD  Height
	63, //60,									// BYTE  MaxVFreq
	57, // 47, //57,									// BYTE  MinVFreq
	302000L, //288000L,							// DWORD MaxPClk (KHz)  
	2168, //2200,								// WORD W_MinHTotal
	2200,								// W_TypHTotal
	2200,								// W_22HTotal
	2200,								// W_33HTotal
	32,//40,									// BYTE  MinHSyncWidth
	80,//110, 								// BYTE  MinHSyncBackPorch
	2218, //2200,								// W_22VTotal
	2218, //2200,								// W_33VTotal
	3000, //2300,								// WORD  MaxVtotal
	2218, //2200,								// WORD  MinVtotal
	2218, //2200,								// WORD TypVTotal
	3,									// BYTE  MinVSyncWidth
	7,									// BYTE  MinVSyncBackPorch
	0x1101,								// WORD  PowerUpTiming
	0x1101,								// WORD  PowerDownTiming
	0x21,								// WORD AccToPixelVal
	0x0,								// BYTE  SpreadSpectrumCtrl
	0,									// DCLK display timing delay adjustment in ns unit
	Wa_OverDriveLUT_LD_T120001A,		// Overdrive look up table
	sRGB2rgb_LD_T120001A,
	Sta_GammaLUT_LD_T120001A,
	0,									// WORD  Invert_DVS
	0,									// WORD  Invert_DHS
	0,									// WORD  Invert_DCLK
	0,									// WORD  Invert_DEN
	
	0x00, //0xFF,								// WORD  DportSwap
	TRUE,								// WORD  LVDSInterface
	QUAD_BUS,							// WORD  W_LVDS_BusType
	LVDS_EIGHT_BIT_MODE,				// WORD  LvdsPanelMode
	0xFF,								// WORD  Spread_Spect_En
	NO_SWAP,							// WORD	LvdsPosNegSwap
	STPLF_DISPLAY_PAD_DRIVE_1_25,		// WORD PadDrive	
	STPLF_DISPLAY_LVDS_CURRENT_3_5,		// WORD  W_LvdsCurrentCtrl
	OUT_24_48_BIT,						// WORD  W_BitOutput
	STPANEL_LVDS_MAP_CUSTOM,			// WORD W_LVDSSignalMap
	0x0063,								// WORD  W_LvdsClkData
};

extern ROM gmt_PanelData Panel_2K2K_LD_T120001A;

#else
	#define Panel_2K2K_LD_T120001A	(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == LD_T120001A)
		#pragma message "Default panel will not bootup unless INCLUDE_LD_T120001A is defined!"
	#endif
#endif

