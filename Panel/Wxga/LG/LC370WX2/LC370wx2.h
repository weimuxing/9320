/*
	$Workfile:   LC370wx2.h  $
	$Revision: 1.3 $
	$Date: 2011/05/11 06:36:10 $
*/

#ifdef INCLUDE_LG_WXGA_LC370WX2

#include "..\..\Panel\Wxga\LG\LC370WX2\OverdriveLut.h"
#include "..\..\Panel\Wxga\LG\LC370WX2\GammaLut.h"

#define PANEL_120_OR_100_HZ_COMPATIBLE

char ROM PanelName_WXGA_LC370WX2[] = "LG_LC370_WX2";

ROM WORD sRGB2rgb_LC370WX2[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};

ROM gmt_PanelData Panel_LG_WXGA_LC370WX2 =
{
	PanelName_WXGA_LC370WX2,// Panel Name
	LG_WXGA_LC370WX2,	// Panel ID (refer to enum defined in panel.h)
	PANEL_WXGA,			// gmd_PanelType PanelType;
	FORMAT_ORG_2D,    // gmd_PanelDataFormat;
	TWO,				//BYTE PixPerClock;
	10, // 8,					//BYTE  Depth;
	1366,				//WORD  Width;
	768,					//WORD  Height;
	123,					//BYTE  MaxVFreq;
	97,					//BYTE  MinVFreq;
	160000L,				//DWORD MaxPClk; 		 // KHz
	1528,				//WORD W_MinHTotal
	1528,				//W_TypHTotal
	// TODO: UPDATE 22 and 33 H/V and VTypical totals with spreadsheet
	1528,				//W_22HTotal		
	1528,				//W_33HTotal
	32,					//BYTE  MinHSyncWidth;
	80,					//BYTE  MinHSyncBackPorch;
	800,					//W_22VTotal				
	800,					//W_33VTotal 
	800,				//WORD  MaxVtotal;
	780,					//WORD  MinVtotal;
	790,					//WORD TypVTotal;
	5,					//BYTE  MinVSyncWidth;	
	15,					//BYTE  MinVSyncBackPorch;	
	
	0x9696,				//WORD  PowerUpTiming;
	0xffff,				//WORD  PowerDownTiming;
	0xC,					//WORD AccToPixelVal;
	0x0,					//BYTE  SpreadSpectrumCtrl;		// Value for Spread_Spectrum_Control register
	0,					//BYTE  DClkDelay;		 		// DCLK display timing delay adjustment in ns unit.
	Wa_OverDriveLUT_LC370WX2, //OverDriveLut_LC370WX2,
	sRGB2rgb_LC370WX2,
	Sta_GammaLUT_LC370WX2,
	0,					//WORD  Invert_DVS 				: 1;
	0,					//WORD  Invert_DHS 				: 1;
	0,					//WORD  Invert_DCLK 			: 1;
	0,					//WORD  Invert_DEN 				: 1;
	0x0,				//WORD  DportSwap				: 1;
	TRUE,				//WORD  LVDSInterface			: 1;
	DUAL_BUS,			//WORD  W_LVDS_BusType 			: 1;	
	LVDS_EIGHT_BIT_MODE,		//WORD  LvdsPanelMode	: 2; 
	0xff,				//WORD  Spread_Spect_En 		: 1; // 0xFF : Enable spectrum, 0x00 : Disable 
	NO_SWAP,				//WORD	LvdsPosNegSwap			: 1;
	0,      			//WORD  PadDrive;
	SEVEN_PT_ONE_MA,	//WORD  W_LvdsCurrentCtrl		: 2;			
	OUT_24_48_BIT,		//WORD  W_BitOutput				: 1;
	STPANEL_LVDS_MAP_STANDARD1,			// WORD W_LVDSSignalMap
	0x0063,				//WORD  W_LvdsClkData
};
extern ROM gmt_PanelData Panel_LG_WXGA_LC370WX2;

#else
	#define Panel_LG_WXGA_LC370WX2	(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == LG_WXGA_LC370WX2)
		#pragma message "Default panel will not bootup unless INCLUDE_LG_WXGA_LC370WX2 is defined!"
	#endif
#endif
