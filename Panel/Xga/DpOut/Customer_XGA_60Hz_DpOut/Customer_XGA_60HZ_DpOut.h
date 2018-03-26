/*
	$Workfile:   Customer_XGA_60HZ_DpOut.h  $
	$Revision:   1.0  $
	$Date:   Dec 01 2008 07:22:32  $
*/

#ifdef INCLUDE_CUSTOMER_XGA_60HZ_DPOUT

#include "..\..\Panel\Xga\DpOut\Customer_XGA_60Hz_DpOut\OverdriveLut.h"
#include "..\..\Panel\Xga\DpOut\Customer_XGA_60Hz_DpOut\GammaLut.h"


char ROM PanelName_Customer_XGA_60Hz_DpOut[] = "CUSTOMER_XGA_60HZ_DPOUT";

ROM WORD sRGB2rgb_60Hz_DpOut_XGA[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};

ROM gmt_PanelData Panel_Customer_XGA_60Hz_DpOut =
{
	PanelName_Customer_XGA_60Hz_DpOut,			//;
	CUSTOMER_XGA_60HZ_DPOUT,	// Panel ID (refer to enum defined in panel.h)	
	PANEL_XGA,			// gmd_PanelType PanelType;
	ONE,				//BYTE PixPerClock;
	8,					//BYTE  Depth;
	1024,				//WORD  Width;
	768,					//WORD  Height;
	60,					//BYTE  MaxVFreq;
	60,					//BYTE  MinVFreq;
	65000L,				//DWORD MaxPClk; 		 // KHz
	1344,				//WORD W_MinHTotal
	1344,				//W_TypHTotal - XGA at 60hz
	1344,				//W_22HTotal
	1344,				//W_33HTotal
	136,				//BYTE  MinHSyncWidth;
	160,				//BYTE  MinHSyncBackPorch;
	806,				//W_22VTotal
	806,				//W_33VTotal
	806,				//WORD  MaxVtotal;
	806,				//WORD  MinVtotal;
	806,				//WORD TypVTotal;
	6,					//BYTE  MinVSyncWidth;
	29,					//BYTE  MinVSyncBackPorch;
	0x775522L,			//DWORD PadDrive;
	// TODO: Check the power up and power down timing for this panel!!!
	0x9696,				//WORD  PowerUpTiming;
	0xffff,				//WORD  PowerDownTiming;
	0xC,				//WORD AccToPixelVal;
	0x0,				//BYTE  SpreadSpectrumCtrl;		// Value for Spread_Spectrum_Control register
	0,					//BYTE  DClkDelay;		 		// DCLK display timing delay adjustment in ns unit.
	Wa_OverDriveLUT_60Hz_DpOut_XGA,	//OverDriveLut_LM150X06_VGA,
	sRGB2rgb_60Hz_DpOut_XGA,
	Sta_GammaLUT_60Hz_DpOut_XGA,
	0,					//WORD  Invert_DVS 				: 1;
	0,					//WORD  Invert_DHS 				: 1;
	0,					//WORD  Invert_DCLK 			: 1;
	0,					//WORD  Invert_DEN 				: 1;  
	0xff,				//WORD  DportSwap				: 1;
	FALSE,				//WORD  LVDSInterface			: 1;  
	SINGLE_BUS,			//WORD  W_LVDS_BusType 			: 1;	
	NO_LVDS,    		//WORD  LvdsPanelMode	: 2;
	0xff,				//WORD  Spread_Spect_En 		: 1; // 0xFF : Enable spectrum, 0x00 : Disable 
	SWAP,				//WORD	LvdsPosNegSwap			: 1;		
	FIVE_PT_SIX_MA,		//WORD  W_LvdsCurrentCtrl		: 2;
	OUT_24_48_BIT,		//WORD  W_BitOutput				: 1;
	0x0063,				//WORD  W_LvdsClkData
};
extern ROM gmt_PanelData Panel_Customer_XGA_60Hz_DpOut;

#else
	#define Panel_Customer_XGA_60Hz_DpOut (ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == CUSTOMER_XGA_60HZ_DPOUT)
		#pragma message "Default panel will not bootup unless INCLUDE_CUSTOMER_XGA_60HZ_DPOUT is defined!"
	#endif
#endif
