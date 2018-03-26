/*
	$Workfile:   LC171w03_VGA.h  $
	$Revision: 1.3 $
	$Date: 2011/05/11 06:36:09 $
*/

#ifdef INCLUDE_LG_WXGA_LC171W03_VGA

#include "..\..\Panel\Wxga\LG\LC171W03_VGA\OverdriveLut.h"
#include "..\..\Panel\Wxga\LG\LC171W03_VGA\GammaLut.h"

//
// NOTE:
// This modification is used by one of our customers with their RD 
// connected with VGA daughter card in order to output WXGA signals.
//


char ROM PanelName_WXGA_LC171W03_VGA[] = "LG_LC171W03_VGA";

ROM WORD sRGB2rgb_LC171W03_VGA[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};	

ROM gmt_PanelData Panel_LG_WXGA_LC171W03_VGA =
{
	PanelName_WXGA_LC171W03_VGA,	//;
	LG_WXGA_LC171W03_VGA,			// Panel ID (refer to enum defined in panel.h)	
	PANEL_WXGA,						// gmd_PanelType PanelType;
	FORMAT_ORG_2D,    // gmd_PanelDataFormat;
	ONE,				//BYTE PixPerClock;
	8,					//BYTE  Depth;
	1280,				//WORD  Width;
	768,				//WORD  Height;
	77,					//BYTE  MaxVFreq;
	50,					//BYTE  MinVFreq;
	80000L,				//DWORD MaxPClk; 		 // KHz
	1352,				//WORD W_MinHTotal
	1620,				//W_TypHTotal - XGA at 60hz
	1740,				//W_22HTotal		
	1360,				//W_33HTotal
	136,				//BYTE  MinHSyncWidth;  
	160,				//BYTE  MinHSyncBackPorch;
	945,				//W_22VTotal				
	806,				//W_33VTotal			
	1000,				//WORD  MaxVtotal;
	788,				//WORD  MinVtotal;
	812,				//WORD TypVTotal;
	6,					//BYTE  MinVSyncWidth;	 
	29,					//BYTE  MinVSyncBackPorch;
	
	// TODO: Check the power up and power down timing for this panel!!!
	0x1811,				//WORD  PowerUpTiming;
	0x1811,				//WORD  PowerDownTiming;
	0xC,				//WORD AccToPixelVal;
	0x0,				//BYTE  SpreadSpectrumCtrl;		// Value for Spread_Spectrum_Control register
	0,					//BYTE  DClkDelay;		 		// DCLK display timing delay adjustment in ns unit.
	Wa_OverDriveLUT_LC171W03_VGA,	//OverDriveLut_LC171W03_VGA,
	sRGB2rgb_LC171W03_VGA,
	Sta_GammaLUT_LC171W03_VGA,
	1,					//WORD  Invert_DVS 				: 1;
	1,					//WORD  Invert_DHS 				: 1;
	1,					//WORD  Invert_DCLK 			: 1;
	0,					//WORD  Invert_DEN 				: 1;  
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
extern ROM gmt_PanelData Panel_LG_WXGA_LC171W03_VGA;

#else
	#define Panel_LG_WXGA_LC171W03_VGA	(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == LG_WXGA_LC171W03_VGA)
		#pragma message "Default panel will not bootup unless INCLUDE_LG_WXGA_LC171W03_VGA is defined!"
	#endif
#endif
