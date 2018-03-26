/*
	$Workfile:   V370H1_L01.h  $
	$Revision: 1.3 $
	$Date: 2011/05/11 06:36:06 $
*/

#ifdef INCLUDE_CMO_1080P_V370H1_L01

#include "..\..\Panel\1080P\CMO\V370H1_L01\OverdriveLut.h"
#include "..\..\Panel\1080P\CMO\V370H1_L01\GammaLut.h"


char ROM PanelName_CMO_1080P_V370H1_L01[] = "CMO_V370H1_L01";

ROM WORD sRGB2rgb_V370H1_L01[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};

ROM gmt_PanelData Panel_1080P_CMO_V370H1_L01 =
{
	PanelName_CMO_1080P_V370H1_L01,// Panel Name
	CMO_1080P_V370H1_L01,	// Panel ID (refer to enum defined in panel.h)
	PANEL_1080P,			//	gmd_PanelType PanelType;
	FORMAT_ORG_2D,    // gmd_PanelDataFormat;
	TWO,				//BYTE PixPerClock;
	8,					//BYTE  Depth;
	1920,				//WORD  Width;
	1080,					//WORD  Height;
	63,					//BYTE  MaxVFreq;
	57,					//BYTE  MinVFreq;
	160000L,				//DWORD MaxPClk; 		 // KHz
	2100,				//WORD W_MinHTotal
	2200,				//W_TypHTotal
	// TODO: UPDATE 22 and 33 H/V and VTypical totals with spreadsheet
	2700,				//W_22HTotal		
	2700,				//W_33HTotal
	8,				//BYTE  MinHSyncWidth;
	16,					//BYTE  MinHSyncBackPorch;
	1090,					//W_22VTotal				
	1100,					//W_33VTotal 
	1135,				//WORD  MaxVtotal;
	1115,				//WORD  MinVtotal;
	1125,					//WORD TypVTotal;
	6,					//BYTE  MinVSyncWidth;	
	35,					//BYTE  MinVSyncBackPorch;	
	
	0x9696,				//WORD  PowerUpTiming;
	0xffff,				//WORD  PowerDownTiming;
	0xF,					//WORD AccToPixelVal;
	0x0,					//BYTE  SpreadSpectrumCtrl;		// Value for Spread_Spectrum_Control register
	0,					//BYTE  DClkDelay;		 		// DCLK display timing delay adjustment in ns unit.
	Wa_OverDriveLUT_V370H1_L01, //OverDriveLut_V370H1_L01,
	sRGB2rgb_V370H1_L01,
	Sta_GammaLUT_V370H1_L01,
	0,					//WORD  Invert_DVS 				: 1;
	0,					//WORD  Invert_DHS 				: 1;
	0,					//WORD  Invert_DCLK 			: 1;
	0,					//WORD  Invert_DEN 				: 1;
	0xff,				//WORD  DportSwap				: 1;
	TRUE,				//WORD  LVDSInterface			: 1;
	DUAL_BUS,			//WORD  W_LVDS_BusType 			: 1;	
	LVDS_EIGHT_BIT_MODE,		//WORD  LvdsPanelMode	: 2; 
	0xff,				//WORD  Spread_Spect_En 		: 1; // 0xFF : Enable spectrum, 0x00 : Disable 
	SWAP,				//WORD	LvdsPosNegSwap			: 1;
	0,      			//WORD  PadDrive;
	SEVEN_PT_ONE_MA,	//WORD  W_LvdsCurrentCtrl		: 2;			
	OUT_24_48_BIT,		//WORD  W_BitOutput				: 1;
	STPANEL_LVDS_MAP_STANDARD1,			// WORD W_LVDSSignalMap
	0x0063,				//WORD	W_LvdsClkData
};
extern ROM gmt_PanelData Panel_1080P_CMO_V370H1_L01;

#else
	#define Panel_1080P_CMO_V370H1_L01	(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == CMO_1080P_V370H1_L01)
		#pragma message "Default panel will not bootup unless INCLUDE_CMO_1080P_V370H1_L01 is defined!"
	#endif
#endif
