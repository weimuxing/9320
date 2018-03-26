//&*&*&*S 20120525 :SWRD4010: Kordonm(0) Add New Panel Parameter LG WHD LM290WW1
/*
	$Workfile:   LM290WW1.h  $
	$Revision: 1.1 $
	$Date: 2011/09/08 06:56:13 $
*/

#ifdef INCLUDE_LG_WHD_LM290WW1_SSA1

#include "..\..\Panel\WHD\LG\LM290WW1\OverdriveLut.h"
#include "..\..\Panel\WHD\LG\LM290WW1\GammaLut.h"

char ROM PanelName_LM290WW1[] = "LG_WHD_LM290WW1";

ROM WORD sRGB2rgb_LM290WW1[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};

ROM gmt_PanelData Panel_LG_WHD_LM290WW1 =
{
	PanelName_LM290WW1,
	LG_WHD_LM290WW1_SSA1,			// Panel ID (refer to enum defined in panel.h)
	PANEL_WQHD,						// gmd_PanelType PanelType
	FORMAT_ORG_2D,    // gmd_PanelDataFormat;
	FOUR,								// BYTE PixPerClock
	8,									// BYTE  Depth
	2560,								// WORD  Width
	1080,								// WORD  Height
	75,	//60,									// BYTE  MaxVFreq
	50,	//60,									// BYTE  MinVFreq
	185580L,	//181250L							// DWORD MaxPClk (KHz)
	2720,								// WORD W_MinHTotal
	2784,	//2720,								// W_TypHTotal
	2784,	//2720,								// W_22HTotal
	2784,	//2720,								// W_33HTotal
	32,									// BYTE  MinHSyncWidth
	80, 								// BYTE  MinHSyncBackPorch
	1330,	//1111,								// W_22VTotal
	1330,	//1111,								// W_33VTotal
	1330,	//1111,								// WORD  MaxVtotal
	1093,	//1111,								// WORD  MinVtotal
	1112,	//1111,	//1111,								// WORD TypVTotal	//20121004 Torisa +1 for game mode
	10,									// BYTE  MinVSyncWidth
	18,									// BYTE  MinVSyncBackPorch
	0xE00A,								// WORD  PowerUpTiming
	0x0A60,	//0x0060,								// WORD  PowerDownTiming
	0x21,								// WORD AccToPixelVal
	0x3F4,								// BYTE  SpreadSpectrumCtrl	//0x04 = Amplitude 0x2, Period 0x3F
	0,									// DCLK display timing delay adjustment in ns unit
	Wa_OverDriveLUT_LM290WW1,		// Overdrive look up table
	sRGB2rgb_LM290WW1,
	Sta_GammaLUT_LM290WW1,
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
	STPANEL_LVDS_MAP_STANDARD2,			// WORD W_LVDSSignalMap
	0x0063,								// WORD  W_LvdsClkData
};

extern ROM gmt_PanelData Panel_LG_WHD_LM290WW1;

#else
	#define Panel_LG_WHD_LM290WW1	(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == LG_WHD_LM290WW1_SSA1)
		#pragma message "Default panel will not bootup unless INCLUDE_LG_WHD_LM290WW1_SSA1 is defined!"
	#endif
#endif
//&*&*&*E 20120525 : Kordonm(1) Add New Panel Parameter LG WHD LM290WW1
