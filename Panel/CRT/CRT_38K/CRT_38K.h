/*
	$Workfile:   CRT_38K.h  $
	$Revision: 1.3 $
	$Date: 2011/05/11 06:36:07 $
*/

#ifdef INCLUDE_CRT_38K

#include "..\..\Panel\CRT\CRT_38K\OverdriveLut.h"
#include "..\..\Panel\CRT\CRT_38K\GammaLut.h"

char ROM PanelName_CRT_38K[] = "CRT 38K";


ROM WORD sRGB2rgb_CRT_38K[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};								

ROM gmt_PanelData Panel_CRT_38K =
{
	PanelName_CRT_38K,			//;
	CRT_38K,					// Panel ID (refer to enum defined in panel.h)	
	PANEL_WXGA,			// gmd_PanelType PanelType;
	FORMAT_ORG_2D,    // gmd_PanelDataFormat;
	ONE,				//BYTE PixPerClock;
	8,					//BYTE  Depth;
	1440,				//WORD  Width;
	556,					//WORD  Height;
	75,					//BYTE  MaxVFreq;
	50,					//BYTE  MinVFreq;
	70000L,				//DWORD MaxPClk; 		 // KHz
	1800,				//WORD W_MinHTotal
	1800,				//W_TypHTotal
	1800,				//W_22HTotal		
	1800,				//W_33HTotal
	126,					//BYTE  MinHSyncWidth;
	180,					//BYTE  MinHSyncBackPorch;
	632,					//W_22VTotal				
	632,					//W_33VTotal			
	632,					//WORD  MaxVtotal;
	632,					//WORD  MinVtotal;
	632,					//WORD TypVTotal;
	5,					//BYTE  MinVSyncWidth;	
	8,					//BYTE  MinVSyncBackPorch;	
	
	// TODO: Check the power up and power down timing for this panel!!!
	0x1811,				//WORD  PowerUpTiming;
	0x1811,				//WORD  PowerDownTiming;
	0xC,					//WORD AccToPixelVal;
	0x0,					//BYTE  SpreadSpectrumCtrl;		// Value for Spread_Spectrum_Control register
	0,					//BYTE  DClkDelay;		 		// DCLK display timing delay adjustment in ns unit.
	Wa_OverDriveLUT_CRT_38K,		//OverDriveLut_CRT_38K,
	sRGB2rgb_CRT_38K,
	Sta_GammaLUT_CRT_38K,
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
extern ROM gmt_PanelData Panel_CRT_38K;

#else
	#define Panel_CRT_38K	(ROM gmt_PanelData *)PanelArray	
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == CRT_38K)
		#pragma message "Default panel will not bootup unless INCLUDE_CRT_38K is defined!"
	#endif
#endif
