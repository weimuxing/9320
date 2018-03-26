/*
	$Workfile:   M220Z1_L09.h  $
	$Revision: 1.5 $
	$Date: 2011/09/16 02:52:15 $
*/

#ifdef INCLUDE_CMO_WSXGA_M220Z1_L09

#include "..\..\Panel\Wsxga\CMO\M220Z1_L09\OverdriveLut.h"
#include "..\..\Panel\Wsxga\CMO\M220Z1_L09\GammaLut.h"

char ROM PanelName_M220Z1_L09[] = "CMO_WSXGA_M220Z1_L09";

ROM WORD sRGB2rgb_M220Z1_L09[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};

ROM gmt_PanelData Panel_CMO_WSXGA_M220Z1_L09 =
{
	PanelName_M220Z1_L09,
	CMO_WSXGA_M220Z1_L09,			// Panel ID (refer to enum defined in panel.h)
	PANEL_WSXGA,						// gmd_PanelType PanelType
	FORMAT_ORG_2D,                // gmd_PanelDataFormat;
	TWO, //TWO,								   // BYTE PixPerClock
	8,									   // BYTE  Depth
	1680,								   // WORD  Width
	1050,								   // WORD  Height
	120, 									// BYTE  MaxVFreq
	55,									// BYTE  MinVFreq
	59500UL,								// DWORD MaxPClk (KHz)
	1760,							      // WORD W_MinHTotal
	1840,								   // W_TypHTotal
	2400,								   // W_22HTotal
	2400,								   // W_33HTotal
	8,									   // BYTE  MinHSyncWidth
	16,								   // BYTE  MinHSyncBackPorch
	1180,	// 1300							   // W_22VTotal
	1180,								   // W_33VTotal
	1195,								   // WORD  MaxVtotal
	1160,								   // WORD  MinVtotal
	1180,								   // WORD TypVTotal
	3,									   // BYTE  MinVSyncWidth
	5,									   // BYTE  MinVSyncBackPorch
	
	0x1101,								// WORD  PowerUpTiming
	0x1101,								// WORD  PowerDownTiming
	0xE,								   // WORD AccToPixelVal
	0x0,								   // BYTE  SpreadSpectrumCtrl
	0,									   // DCLK display timing delay adjustment in ns unit
	0,//Wa_OverDriveLUT_M220Z1_L09,		// Overdrive look up table
	0,//sRGB2rgb_M220Z1_L09,
	0,//Sta_GammaLUT_M220Z1_L09,
	0,									// WORD  Invert_DVS
	0,									// WORD  Invert_DHS
	0,									// WORD  Invert_DCLK
	0,									// WORD  Invert_DEN
	0xFF,								// WORD  DportSwap
	TRUE,								// WORD  LVDSInterface
	QUAD_BUS,							// WORD  W_LVDS_BusType
	LVDS_EIGHT_BIT_MODE,				// WORD  LvdsPanelMode
	0xFF,								// WORD  Spread_Spect_En
	SWAP,								// WORD	LvdsPosNegSwap
	0,      							//WORD  PadDrive;
	SEVEN_PT_ONE_MA,					// WORD  W_LvdsCurrentCtrl
	OUT_24_48_BIT,						// WORD  W_BitOutput
	STPANEL_LVDS_MAP_STANDARD1,			// WORD W_LVDSSignalMap
	0x0063,								// WORD  W_LvdsClkData
};

extern ROM gmt_PanelData Panel_CMO_WSXGA_M220Z1_L09;

#else
	#define Panel_CMO_WSXGA_M220Z1_L09	(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == CMO_WSXGA_M220Z1_L09)
		#pragma message "Default panel will not bootup unless INCLUDE_CMO_WSXGA_M220Z1_L09 is defined!"
	#endif
#endif

