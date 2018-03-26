/*
	$Workfile:   Lm171w01.h  $
	$Revision: 1.3 $
	$Date: 2011/05/11 06:36:10 $
*/

#ifdef INCLUDE_LG_WXGA_LM171W01

#include "..\..\Panel\Wxga\LG\LM171W01\OverdriveLut.h"
#include "..\..\Panel\Wxga\LG\LM171W01\GammaLut.h"

char ROM PanelName_WXGA_LM171W01[] = "LG_LM171_W01";

ROM WORD sRGB2rgb_LM171W01[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};

ROM gmt_PanelData Panel_LG_WXGA_LM171W01 =
{
	PanelName_WXGA_LM171W01,			//;
	LG_WXGA_LM171W01,	// Panel ID (refer to enum defined in panel.h)	
	PANEL_WXGA,			// gmd_PanelType PanelType;
	FORMAT_ORG_2D,    // gmd_PanelDataFormat;
	ONE,				//BYTE PixPerClock;
	8,					//BYTE  Depth;
	1280,				//WORD  Width;
	768,					//WORD  Height;
	77,					//BYTE  MaxVFreq;
	50,					//BYTE  MinVFreq;
	80000L,				//DWORD MaxPClk; 		 // KHz
	1344,				//WORD W_MinHTotal
	1594,				//W_TypHTotal
	// TODO: UPDATE 22 and 33 H/V and VTypical totals with spreadsheet
	2038,				//W_22HTotal		
	1350,				//W_33HTotal
	4,					//BYTE  MinHSyncWidth;
	12,					//BYTE  MinHSyncBackPorch;
	788,					//W_22VTotal				
	793,					//W_33VTotal			
	1000,				//WORD  MaxVtotal;
	788,					//WORD  MinVtotal;
	812,					//WORD TypVTotal;
	3,					//BYTE  MinVSyncWidth;	
	6,					//BYTE  MinVSyncBackPorch;	
	
	0x9696,				//WORD  PowerUpTiming;
	0xffff,				//WORD  PowerDownTiming;
	0xC,					//WORD AccToPixelVal;
	0x0,					//BYTE  SpreadSpectrumCtrl;		// Value for Spread_Spectrum_Control register
	0,					//BYTE  DClkDelay;		 		// DCLK display timing delay adjustment in ns unit.
	Wa_OverDriveLUT_LM171W01,	//	OverDriveLut_LM171W01,
	sRGB2rgb_LM171W01,
	Sta_GammaLUT_LM171W01,
	0,					//WORD  Invert_DVS 				: 1;
	0,					//WORD  Invert_DHS 				: 1;
	0,					//WORD  Invert_DCLK 			: 1;
	0,					//WORD  Invert_DEN 				: 1;
	0xff,				//WORD  DportSwap				: 1;
	TRUE,				//WORD  LVDSInterface			: 1;
	SINGLE_BUS,			//WORD  W_LVDS_BusType 			: 1;	
	LVDS_EIGHT_BIT_MODE,		//WORD  LvdsPanelMode	: 2; 
	0xff,				//WORD  Spread_Spect_En 		: 1; // 0xFF : Enable spectrum, 0x00 : Disable
	SWAP,				//WORD	LvdsPosNegSwap			: 1;	
	0,      			//WORD  PadDrive;
	SEVEN_PT_ONE_MA,	//WORD  W_LvdsCurrentCtrl		: 2;		
	OUT_24_48_BIT,		//WORD  W_BitOutput				: 1;
	STPANEL_LVDS_MAP_STANDARD1,			// WORD W_LVDSSignalMap
	0x0063,				//WORD  W_LvdsClkData
};

extern ROM gmt_PanelData Panel_LG_WXGA_LM171W01;

#else
	#define Panel_LG_WXGA_LM171W01	(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == LG_WXGA_LM171W01)
		#pragma message "Default panel will not bootup unless INCLUDE_LG_WXGA_LM171W01 is defined!"
	#endif
#endif
