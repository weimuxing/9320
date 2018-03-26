/*
	$Workfile:   LC320EXN.h  $
	$Revision: 1.3 $
	$Date: 2011/05/11 06:36:10 $
*/

#ifdef INCLUDE_LG_WXGA_LC320EXN

#include "..\..\Panel\Wxga\LG\LC320EXN\OverdriveLut.h"
#include "..\..\Panel\Wxga\LG\LC320EXN\GammaLut.h"

char ROM PanelName_WXGA_LC320EXN[] = "LG_LC320_W01";

ROM WORD sRGB2rgb_LC320EXN[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};

ROM gmt_PanelData Panel_LG_WXGA_LC320EXN =
{
	PanelName_WXGA_LC320EXN,// Panel Name
	LG_WXGA_LC320EXN,	// Panel ID (refer to enum defined in panel.h)
	PANEL_WXGA,			// gmd_PanelType PanelType;
	FORMAT_ORG_2D,    // gmd_PanelDataFormat;
	ONE,				//BYTE PixPerClock;
	8,					//BYTE  Depth;
	1366,				//WORD  Width;
	768,					//WORD  Height;
	77,					//BYTE  MaxVFreq;
	50,					//BYTE  MinVFreq;
	80000L,				//DWORD MaxPClk; 		 // KHz
	1528,				//WORD W_MinHTotal
	1528,				//W_TypHTotal
	// TODO: UPDATE 22 and 33 H/V and VTypical totals with spreadsheet
	1528,				//W_22HTotal		
	1528,				//W_33HTotal
	8,					//BYTE  MinHSyncWidth;
	16,					//BYTE  MinHSyncBackPorch;
	790,					//W_22VTotal				
	790,					//W_33VTotal 
	790,				//WORD  MaxVtotal;
	790,					//WORD  MinVtotal;
	790,					//WORD TypVTotal;
	5,					//BYTE  MinVSyncWidth;	
	15,					//BYTE  MinVSyncBackPorch;	
	
	0x9696,				//WORD  PowerUpTiming;
	0xffff,				//WORD  PowerDownTiming;
	0xC,					//WORD AccToPixelVal;
	0x0,					//BYTE  SpreadSpectrumCtrl;		// Value for Spread_Spectrum_Control register
	0,					//BYTE  DClkDelay;		 		// DCLK display timing delay adjustment in ns unit.
	Wa_OverDriveLUT_LC320EXN, //OverDriveLut_LC320EXN,
	sRGB2rgb_LC320EXN,
	Sta_GammaLUT_LC320EXN,
	0,					//WORD  Invert_DVS 				: 1;
	0,					//WORD  Invert_DHS 				: 1;
	0,					//WORD  Invert_DCLK 			: 1;
	0,					//WORD  Invert_DEN 				: 1;
	0xff,				//WORD  DportSwap				: 1;
	TRUE,				//WORD  LVDSInterface			: 1;
	SINGLE_BUS,			//WORD  W_LVDS_BusType 			: 1;	
	LVDS_EIGHT_BIT_MODE,		//WORD  LvdsPanelMode	: 2; 
	0xff,				//WORD  Spread_Spect_En 		: 1; // 0xFF : Enable spectrum, 0x00 : Disable 
	NO_SWAP,				//WORD	LvdsPosNegSwap			: 1;
	STPLF_DISPLAY_PAD_DRIVE_1_25,      			//WORD  PadDrive;
	STPLF_DISPLAY_LVDS_CURRENT_3_5,	//WORD  W_LvdsCurrentCtrl		: 2;			
	OUT_24_48_BIT,		//WORD  W_BitOutput				: 1;
	STPANEL_LVDS_MAP_STANDARD2,			// WORD W_LVDSSignalMap
	0x0063,				//WORD  W_LvdsClkData
};
extern ROM gmt_PanelData Panel_LG_WXGA_LC320EXN;

#else
	#define Panel_LG_WXGA_LC320EXN	(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == LG_WXGA_LC320EXN)
		#pragma message "Default panel will not bootup unless INCLUDE_LG_WXGA_LC320EXN is defined!"
	#endif
#endif
