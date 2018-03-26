/*
	$Workfile:   T315XW01.h  $
	$Revision:   1.9  $
	$Date:   Jun 22 2006 17:51:08  $
*/

#ifdef INCLUDE_AUO_WXGA_P315XW01

#include "..\..\Panel\Wxga\AUO\P315XW01\OverdriveLut.h"
#include "..\..\Panel\Wxga\AUO\P315XW01\GammaLut.h"

char ROM PanelName_AUO_WXGA_P315XW01[] = "AUO_WXGA_P315XW01";

ROM WORD sRGB2rgb_P315XW01[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};

ROM gmt_PanelData Panel_WXGA_AUO_P315XW01 =
{
	PanelName_AUO_WXGA_P315XW01,			//;
	AUO_WXGA_P315XW01,	// Panel ID (refer to enum defined in panel.h)
	PANEL_WXGA,			//		 	gmd_PanelType PanelType;
	ONE,				//BYTE PixPerClock;
	8,					//BYTE  Depth;
	1366,				//WORD  Width;
	768,				//WORD  Height;
	75,					//BYTE  MaxVFreq;
	50,					//BYTE  MinVFreq;
	85000L,				//DWORD MaxPClk; 		 // KHz
	1600,				//WORD W_MinHTotal
	1600,				//W_TypHTotal
	// TODO: UPDATE 22 and 33 H/V and VTypical totals with spreadsheet
	2356,				//W_22HTotal
	1602,				//W_33HTotal
	10,					//BYTE  MinHSyncWidth;
	28,					//BYTE  MinHSyncBackPorch;
	797,					//W_22VTotal
	780,					//W_33VTotal
	960,				//WORD  MaxVtotal;
	774,					//WORD  MinVtotal;
	806,					//WORD TypVTotal;
	14,					//BYTE  MinVSyncWidth;	
	12,					//BYTE  MinVSyncBackPorch;	
	0x775522L,			//DWORD PadDrive;
	0x9696,				//WORD  PowerUpTiming;
	0xffff,				//WORD  PowerDownTiming;
	0xC,					//WORD AccToPixelVal;
	0x203,//0x0,					//BYTE  SpreadSpectrumCtrl;		// Value for Spread_Spectrum_Control register
	0,					//BYTE  DClkDelay;		 		// DCLK display timing delay adjustment in ns unit.
	Wa_OverDriveLUT_P315XW01,	//	OverDriveLut_T315XW01,
	sRGB2rgb_P315XW01,
	Sta_GammaLUT_P315XW01,
	0,					//WORD  Invert_DVS 				: 1;
	0,					//WORD  Invert_DHS 				: 1;
	1,					//WORD  Invert_DCLK 			: 1;
	0,					//WORD  Invert_DEN 				: 1;
	0,					//WORD  DportSwap				: 1;
	TRUE,				//WORD  LVDSInterface			: 1;
	SINGLE_BUS,			//WORD  W_LVDS_BusType 			: 1;	
	LVDS_EIGHT_BIT_MODE,		//WORD  LvdsPanelMode	: 2; 
	0xff,				//WORD  Spread_Spect_En 		: 1; // 0xFF : Enable spectrum, 0x00 : Disable
	NO_SWAP,			//WORD	LvdsPosNegSwap			: 1;	
	FIVE_PT_SIX_MA,	//WORD  W_LvdsCurrentCtrl		: 2;
	OUT_24_48_BIT,		//WORD  W_BitOutput				: 1;
	0x0063,				//WORD  W_LvdsClkData
};
extern ROM gmt_PanelData Panel_WXGA_AUO_P315XW01;

#else
	#define Panel_WXGA_AUO_P315XW01	(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == AUO_WXGA_P315XW01)
		#pragma message "Default panel will not bootup unless INCLUDE_AUO_WXGA_P315XW01 is defined!"
	#endif
#endif
