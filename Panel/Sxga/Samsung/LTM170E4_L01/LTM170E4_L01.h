/*
	$Workfile:   LTM170E4_L01.h  $
	$Revision: 1.3 $
	$Date: 2011/05/11 06:36:08 $
*/

#ifdef INCLUDE_SAMSUNG_SXGA_LTM170E4_L01

#include "..\..\Panel\Sxga\Samsung\LTM170E4_L01\OverdriveLut.h"
#include "..\..\Panel\Sxga\Samsung\LTM170E4_L01\GammaLut.h"

char ROM PanelName_SAMSUNG_SXGA_LTM170E4_L01[] = "SAMSUNG_SXGA_LTM170E4_L01";

ROM WORD sRGB2rgb_LTM170E4_L01[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};

ROM gmt_PanelData Panel_SAMSUNG_SXGA_LTM170E4_L01 =
{
	PanelName_SAMSUNG_SXGA_LTM170E4_L01,			//;
	SAMSUNG_SXGA_LTM170E4_L01,	// Panel ID (refer to enum defined in panel.h)	
	PANEL_SXGA,			//		 	gmd_PanelType PanelType;
	FORMAT_ORG_2D,    // gmd_PanelDataFormat;
	TWO,				//BYTE PixPerClock;
	8,					//BYTE  Depth;
	1280,				//WORD  Width;
	1024,					//WORD  Height;
	77,					//BYTE  MaxVFreq;
	50,					//BYTE  MinVFreq;
	108000L,				//DWORD MaxPClk; 		 // KHz
	1344,				//WORD W_MinHTotal
	1594,				//W_TypHTotal
	1770,				//W_22HTotal		
	1346,				//W_33HTotal
	16,					//BYTE  MinHSyncWidth;
	5,					//BYTE  MinHSyncBackPorch;
	1200,					//W_22VTotal				
	1052,					//W_33VTotal			
	1300,				//WORD  MaxVtotal;
	1042,					//WORD  MinVtotal;
	1066,					//WORD TypVTotal;
	3,					//BYTE  MinVSyncWidth;	
	6,					//BYTE  MinVSyncBackPorch;	
	
	0x9408,				//WORD  PowerUpTiming;
	0x9408,				//WORD  PowerDownTiming;
	0xD,					//WORD AccToPixelVal;
	0x0,					//BYTE  SpreadSpectrumCtrl;		// Value for Spread_Spectrum_Control register
	0,					//BYTE  DClkDelay;		 		// DCLK display timing delay adjustment in ns unit.
	Wa_OverDriveLUT_LTM170E4_L01,		//	OverDriveLut_LTM170E4_L01,
	sRGB2rgb_LTM170E4_L01,
	Sta_GammaLUT_LTM170E4_L01,
	1,					//WORD  Invert_DVS 				: 1;
	1,					//WORD  Invert_DHS 				: 1;
	0,					//WORD  Invert_DCLK 			: 1;
	0,					//WORD  Invert_DEN 				: 1;
	0xff,				//WORD  DportSwap				: 1;
	TRUE,				//WORD  LVDSInterface			: 1;
	DUAL_BUS,			//WORD  W_LVDS_BusType 			: 1;		
	LVDS_EIGHT_BIT_MODE,		//WORD  LvdsPanelMode	: 2; 
	0xff,				//WORD  Spread_Spect_En 		: 1; // 0xFF : Enable spectrum, 0x00 : Disable
	SWAP,				//WORD	LvdsPosNegSwap			: 1;
	0,      			//WORD  PadDrive;
	FIVE_PT_SIX_MA,		//WORD  W_LvdsCurrentCtrl		: 2;			
	OUT_24_48_BIT,		//WORD  W_BitOutput				: 1;
	STPANEL_LVDS_MAP_STANDARD1,			// WORD W_LVDSSignalMap
	0x0063,				//WORD  W_LvdsClkData
};
extern ROM gmt_PanelData Panel_SAMSUNG_SXGA_LTM170E4_L01;

#else
	#define Panel_SAMSUNG_SXGA_LTM170E4_L01	(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == SAMSUNG_SXGA_LTM170E4_L01)
		#pragma message "Default panel will not bootup unless INCLUDE_SAMSUNG_SXGA_LTM170E4_L01 is defined!"
	#endif
#endif
