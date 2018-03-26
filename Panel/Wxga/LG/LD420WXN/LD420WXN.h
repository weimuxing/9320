/*
	$Workfile:   LC320w01.h  $
	$Revision:   1.9  $
	$Date:   Sep 11 2006 16:41:08  $
*/

#ifdef INCLUDE_LG_WXGA_LD420WXN

#include "..\..\Panel\Wxga\LG\LD420WXN\OverdriveLut.h"
#include "..\..\Panel\Wxga\LG\LD420WXN\GammaLut.h"

char ROM PanelName_WXGA_LD420WXN[] = "LG_LD420WXN";

ROM WORD sRGB2rgb_LD420WXN[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};

ROM gmt_PanelData Panel_LG_WXGA_LD420WXN =
{
	PanelName_WXGA_LD420WXN,// Panel Name
	LG_WXGA_LD420WXN,	// Panel ID (refer to enum defined in panel.h)
	PANEL_WXGA,			// gmd_PanelType PanelType;
	ONE,				//BYTE PixPerClock;
	8,					//BYTE  Depth;
	1366,				//WORD  Width;
	768,					//WORD  Height;
	63,					//BYTE  MaxVFreq;
	47,					//BYTE  MinVFreq;
	80000L,				//DWORD MaxPClk; 		 // KHz
	1456,				//WORD W_MinHTotal
	1528,				//W_TypHTotal
	// TODO: UPDATE 22 and 33 H/V and VTypical totals with spreadsheet
	1528,				//W_22HTotal		
	1528,				//W_33HTotal
	8,					//BYTE  MinHSyncWidth;
	16,					//BYTE  MinHSyncBackPorch;
	790,					//W_22VTotal				
	790,					//W_33VTotal 
	1063,				//WORD  MaxVtotal;
	776,					//WORD  MinVtotal;
	790,					//WORD TypVTotal;
	5,					//BYTE  MinVSyncWidth;	
	15,					//BYTE  MinVSyncBackPorch;	
	0x775522L,			//DWORD PadDrive;
	0x3A10,			//Bossen,	for new panel spec.	20090922.	//0x2019,			//Bossen ,	20090904								// WORD  PowerUpTiming
	0x439,								// WORD  PowerDownTiming
	0xC,					//WORD AccToPixelVal;
	0x1203,					//BYTE  SpreadSpectrumCtrl;		// Value for Spread_Spectrum_Control register
	0,					//BYTE  DClkDelay;		 		// DCLK display timing delay adjustment in ns unit.
	Wa_OverDriveLUT_LD420WXN, //OverDriveLut_LC320W01,
	sRGB2rgb_LD420WXN,
	Sta_GammaLUT_LD420WXN,
	0,					//WORD  Invert_DVS 				: 1;
	0,					//WORD  Invert_DHS 				: 1;
	0,					//WORD  Invert_DCLK 			: 1;
	0,					//WORD  Invert_DEN 				: 1;
	0,				//WORD  DportSwap				: 1;
	TRUE,				//WORD  LVDSInterface			: 1;
	SINGLE_BUS,			//WORD  W_LVDS_BusType 			: 1;	
	LVDS_EIGHT_BIT_MODE,		//WORD  LvdsPanelMode	: 2; 
	0xff,				//WORD  Spread_Spect_En 		: 1; // 0xFF : Enable spectrum, 0x00 : Disable 
	NO_SWAP,				//WORD	LvdsPosNegSwap			: 1;
	FOUR_PT_SIX_MA,			//Bossen, Follow TPV FQ request.	20091028.	FIVE_PT_SIX_MA,		//Bossen, follow TPV request.	20091022.	SEVEN_PT_ONE_MA,	//WORD  W_LvdsCurrentCtrl		: 2;			
	OUT_24_48_BIT,		//WORD  W_BitOutput				: 1;
	0x0063,				//WORD  W_LvdsClkData
};
extern ROM gmt_PanelData Panel_LG_WXGA_LD420WXN;

#else
	#define Panel_LG_WXGA_LD420WXN	(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == LG_WXGA_LD420WXN)
		#pragma message "Default panel will not bootup unless INCLUDE_LG_WXGA_LD420WXN is defined!"
	#endif
#endif

