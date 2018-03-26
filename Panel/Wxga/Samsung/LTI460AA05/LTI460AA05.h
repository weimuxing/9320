/*
	$Workfile:   Ltm170w01.h  $
	$Revision:   1.14  $
	$Date:   Jun 22 2006 18:01:02  $
*/

#ifdef INCLUDE_SAMSUNG_WXGA_LTI460AA05

#include "..\..\Panel\Wxga\Samsung\LTI460AA05\OverdriveLut.h"
#include "..\..\Panel\Wxga\Samsung\LTI460AA05\GammaLut.h"

char ROM PanelName_SAMSUNG_LTI460AA05[] = "Samsung_LTI460AA05";

ROM WORD sRGB2rgb_LTI460AA05[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};

ROM gmt_PanelData Panel_SAMSUNG_LTI460AA05 =
{
	PanelName_SAMSUNG_LTI460AA05,// Panel Name
	SAMSUNG_WXGA_LTI460AA05,	// Panel ID (refer to enum defined in panel.h)
	PANEL_WXGA, 			//	gmd_PanelType PanelType;
	FORMAT_ORG_2D,	  // gmd_PanelDataFormat;
	ONE,				//BYTE PixPerClock;
	8,					//BYTE	Depth;
	1366,				//WORD	Width;
	768,					//WORD	Height;
	77, 				//BYTE	MaxVFreq;
	50, 				//BYTE	MinVFreq;
	80000L, 			//DWORD MaxPClk;		 // KHz
	1600,				//WORD W_MinHTotal
	1600,				//W_TypHTotal
	// TODO: UPDATE 22 and 33 H/V and VTypical totals with spreadsheet
	1600,				//W_22HTotal		
	1600,				//W_33HTotal
	8,					//BYTE	MinHSyncWidth;
	16, 				//BYTE	MinHSyncBackPorch;
	838,					//W_22VTotal				
	838,					//W_33VTotal 
	838,				//WORD	MaxVtotal;
	838,					//WORD	MinVtotal;
	838,					//WORD TypVTotal;
	5,					//BYTE	MinVSyncWidth;	
	15, 				//BYTE	MinVSyncBackPorch;	

	0xFA0C, 			//WORD	PowerUpTiming;
	0x041F, 			//WORD	PowerDownTiming;
	0xC,					//WORD AccToPixelVal;
	0x0,					//BYTE	SpreadSpectrumCtrl; 	// Value for Spread_Spectrum_Control register
	0,					//BYTE	DClkDelay;				// DCLK display timing delay adjustment in ns unit.
	Wa_OverDriveLUT_LTI460AA05, //OverDriveLut_LC320W01,
	sRGB2rgb_LTI460AA05,
	Sta_GammaLUT_LTI460AA05,  
	0,					//WORD	Invert_DVS				: 1;
	0,					//WORD	Invert_DHS				: 1;
	0,					//WORD	Invert_DCLK 			: 1;
	0,					//WORD	Invert_DEN				: 1;
	0xff,				//WORD	DportSwap				: 1;
	TRUE,				//WORD	LVDSInterface			: 1;
	SINGLE_BUS, 		//WORD	W_LVDS_BusType			: 1;	
	LVDS_EIGHT_BIT_MODE,		//WORD	LvdsPanelMode	: 2; 
	0xff,				//WORD	Spread_Spect_En 		: 1; // 0xFF : Enable spectrum, 0x00 : Disable 
	NO_SWAP,				//WORD	LvdsPosNegSwap			: 1;
	//In cortez 8532 the LvdsPosNegSwap is reversed so please opt NO_SWAP if you want to swap.
	STPLF_DISPLAY_PAD_DRIVE_1_25,		// WORD PadDrive	
	STPLF_DISPLAY_LVDS_CURRENT_3_5, 	// WORD  W_LvdsCurrentCtrl
	OUT_24_48_BIT,		//WORD	W_BitOutput 			: 1;	
	STPANEL_LVDS_MAP_STANDARD1, 		// WORD W_LVDSSignalMap
	0x0063, 			//WORD	W_LvdsClkData
};

extern ROM gmt_PanelData Panel_SAMSUNG_LTI460AA05;

#else
		#define Panel_SAMSUNG_LTI460AA05	(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == SAMSUNG_WXGA_LTI460AA05)
		#pragma message "Default panel will not bootup unless INCLUDE_SAMSUNG_WXGA_LTI460AA05 is defined!"
	#endif
#endif
