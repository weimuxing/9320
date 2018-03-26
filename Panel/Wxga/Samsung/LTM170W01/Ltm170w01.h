/*
	$Workfile:   Ltm170w01.h  $
	$Revision:   1.14  $
	$Date:   Jun 22 2006 18:01:02  $
*/

#ifdef INCLUDE_SAMSUNG_WXGA_LTM170W01

#include "..\..\Panel\Wxga\Samsung\LTM170W01\OverdriveLut.h"
#include "..\..\Panel\Wxga\Samsung\LTM170W01\GammaLut.h"

char ROM PanelName_WXGA_LTM170W01[] = "Samsung_LTM170_W01";

ROM WORD sRGB2rgb_LTM170W01[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};

ROM gmt_PanelData Panel_SAMSUNG_WXGA_LTM170W01 =
{
	PanelName_WXGA_LTM170W01,			//;
	SAMSUNG_WXGA_LTM170W01,	// Panel ID (refer to enum defined in panel.h)	
	PANEL_WXGA,			//		 	gmd_PanelType PanelType;
	ONE,				//BYTE PixPerClock;
	8,					//BYTE  Depth;
	1280,				//WORD  Width;
	768,				//WORD  Height;
	77,					//BYTE  MaxVFreq;
	50,					//BYTE  MinVFreq;
	80000L,				//DWORD MaxPClk; 		 // KHz
	1350,				//WORD W_MinHTotal
	1602,				//W_TypHTotal
	1708,				//W_22HTotal		
	1360,				//W_33HTotal
	4,					//BYTE  MinHSyncWidth;
	12,					//BYTE  MinHSyncBackPorch;
	952,					//W_22VTotal				
	797,					//W_33VTotal	
	1000,				//WORD  MaxVtotal;
	788,					//WORD  MinVtotal;
	806,					//WORD TypVTotal;
	3,					//BYTE  MinVSyncWidth;	
	6,					//BYTE  MinVSyncBackPorch;	
	0x775522L,			//DWORD PadDrive;
	0x1811,				//WORD  PowerUpTiming;
	0x1811,				//WORD  PowerDownTiming;
	0xC,					//WORD AccToPixelVal;
	0x0,					//BYTE  SpreadSpectrumCtrl;		// Value for Spread_Spectrum_Control register
	0,					//BYTE  DClkDelay;		 		// DCLK display timing delay adjustment in ns unit.
	Wa_OverDriveLUT_LTM170W01, //OverDriveLut_LTM170W01,
	sRGB2rgb_LTM170W01,
	Sta_GammaLUT_LTM170W01,
	1,					//WORD  Invert_DVS 				: 1;
	1,					//WORD  Invert_DHS 				: 1;
	1,					//WORD  Invert_DCLK 			: 1;
	0,					//WORD  Invert_DEN 				: 1;
	0xff,				//WORD  DportSwap				: 1;
	TRUE,				//WORD  LVDSInterface			: 1;
	SINGLE_BUS,			//WORD  W_LVDS_BusType 			: 1;	
	LVDS_EIGHT_BIT_MODE,//WORD  LvdsPanelMode			: 2; 
	0xff,				//WORD  Spread_Spect_En 		: 1; // 0xFF : Enable spectrum, 0x00 : Disable 
	SWAP,				//WORD	LvdsPosNegSwap			: 1;
	FIVE_PT_SIX_MA,		//WORD  W_LvdsCurrentCtrl		: 2;			
	OUT_24_48_BIT,		//WORD  W_BitOutput				: 1;
	0x0063,				//WORD  W_LvdsClkData
};
extern ROM gmt_PanelData Panel_SAMSUNG_WXGA_LTM170W01;

#else
		#define Panel_SAMSUNG_WXGA_LTM170W01	(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == SAMSUNG_WXGA_LTM170W01)
		#pragma message "Default panel will not bootup unless INCLUDE_SAMSUNG_WXGA_LTM170W01 is defined!"
	#endif
#endif
