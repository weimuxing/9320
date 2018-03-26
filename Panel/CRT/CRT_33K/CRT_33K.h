/*
	$Workfile:   CRT_33K.h  $
	$Revision: 1.3 $
	$Date: 2011/05/11 06:36:07 $
*/

#ifdef INCLUDE_CRT_33K

#include "..\..\Panel\CRT\CRT_33K\OverdriveLut.h"
#include "..\..\Panel\CRT\CRT_33K\GammaLut.h"

char ROM PanelName_CRT_33K[] = "CRT 33K";

ROM WORD sRGB2rgb_CRT_33K[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};								

ROM gmt_PanelData Panel_CRT_33K =
{
	PanelName_CRT_33K,			//;
	CRT_33K,					// Panel ID (refer to enum defined in panel.h)	
	PANEL_WXGA,			// gmd_PanelType PanelType;
	FORMAT_ORG_2D,    // gmd_PanelDataFormat;
	ONE,				//BYTE PixPerClock;
	8,					//BYTE  Depth;
	1440,				//WORD  Width;
	480,					//WORD  Height;
	75,					//BYTE  MaxVFreq;
	50,					//BYTE  MinVFreq;
	70000L,				//DWORD MaxPClk; 		 // KHz
	1800,				//WORD W_MinHTotal
	1800,				//W_TypHTotal
	1800,				//W_22HTotal		
	1800,				//W_33HTotal
	126,					//BYTE  MinHSyncWidth;
	180,					//BYTE  MinHSyncBackPorch;
	562,					//W_22VTotal				
	562,					//W_33VTotal			
	562,					//WORD  MaxVtotal;
	562,					//WORD  MinVtotal;
	562,					//WORD TypVTotal;
	5,					//BYTE  MinVSyncWidth;	
	48,					//BYTE  MinVSyncBackPorch;	
	
	// TODO: Check the power up and power down timing for this panel!!!
	0x1811,				//WORD  PowerUpTiming;
	0x1811,				//WORD  PowerDownTiming;
	0xC,					//WORD AccToPixelVal;
	0x0,					//BYTE  SpreadSpectrumCtrl;		// Value for Spread_Spectrum_Control register
	0,					//BYTE  DClkDelay;		 		// DCLK display timing delay adjustment in ns unit.
	Wa_OverDriveLUT_CRT_33K,		//OverDriveLut_CRT_33K,
	sRGB2rgb_CRT_33K,
	Sta_GammaLUT_CRT_33K,
	1,					//WORD  Invert_DVS 				: 1;
	1,					//WORD  Invert_DHS 				: 1;
	1,					//WORD  Invert_DCLK 			: 1;
	1,					//WORD  Invert_DEN 				: 1;
	0xff,				//WORD  DportSwap				: 1;
	FALSE,				//WORD  LVDSInterface			: 1;
	SINGLE_BUS,			//WORD  W_LVDS_BusType 			: 1;
	LVDS_EIGHT_BIT_MODE,		//WORD  LvdsPanelMode	: 2; 
	0xff,				//WORD  Spread_Spect_En 		: 1; // 0xFF : Enable spectrum, 0x00 : Disable 
	SWAP,				//WORD	LvdsPosNegSwap			: 1;		
	0,      			//WORD  PadDrive;
	FIVE_PT_SIX_MA,		//WORD  W_LvdsCurrentCtrl		: 2;
	OUT_24_48_BIT,		//WORD  W_BitOutput				: 1;
	STPANEL_LVDS_MAP_STANDARD1,			// WORD W_LVDSSignalMap
	0x0063,				//WORD  W_LvdsClkData
};
extern ROM gmt_PanelData Panel_CRT_33K;

#else
	#define Panel_CRT_33K	(ROM gmt_PanelData *)PanelArray	
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == CRT_33K)
		#pragma message "Default panel will not bootup unless INCLUDE_CRT_33K is defined!"
	#endif
#endif
