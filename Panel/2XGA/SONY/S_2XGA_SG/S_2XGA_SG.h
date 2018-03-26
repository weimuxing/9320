/*
	$Workfile:   S_2XGA_SG.h  $
	$Revision: 1.2 $
	$Date: 2012/02/23 06:45:33 $
*/

#ifdef INCLUDE_SONY_2XGA_S_2XGA_SG

#include "..\..\Panel\2XGA\SONY\S_2XGA_SG\OverdriveLut.h"
#include "..\..\Panel\2XGA\SONY\S_2XGA_SG\GammaLut.h"

char ROM PanelName_S_2XGA_SG[] = "SONY_2XGA_S_2XGA_SG";

ROM WORD sRGB2rgb_S_2XGA_SG[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};							

ROM gmt_PanelData Panel_SONY_2XGA_S_2XGA_SG =
{
	PanelName_S_2XGA_SG,			//;
	SONY_2XGA_S_2XGA_SG,					// Panel ID (refer to enum defined in panel.h)	
	PANEL_2XGA,			// gmd_PanelType PanelType;
	FORMAT_3D_SIDE_BY_SIDE,    // gmd_PanelDataFormat;
	ONE,				//BYTE PixPerClock;
	8,					//BYTE  Depth;
	1024,				//WORD  Width;
	768,					//WORD  Height;
	60,					//BYTE  MaxVFreq;
	60,					//BYTE  MinVFreq;
	66000L,				//DWORD MaxPClk; 		 // KHz
	1096,				//WORD W_MinHTotal
	1344,				//W_TypHTotal
	1344,				//W_22HTotal		
	1112,				//W_33HTotal
	16,					//BYTE  MinHSyncWidth;
	16,					//BYTE  MinHSyncBackPorch;
	834,					//W_22VTotal				
	813,					//W_33VTotal			
	840,					//WORD  MaxVtotal;
	780,					//WORD  MinVtotal;
	806,					//WORD TypVTotal;
	2,					//BYTE  MinVSyncWidth;	
	2,					//BYTE  MinVSyncBackPorch;	
	
	0x1811,				//WORD  PowerUpTiming;
	0x1811,				//WORD  PowerDownTiming;
	0xC,					//WORD AccToPixelVal;
	0x0,					//BYTE  SpreadSpectrumCtrl;		// Value for Spread_Spectrum_Control register
	0,					//BYTE  DClkDelay;		 		// DCLK display timing delay adjustment in ns unit.
	Wa_OverDriveLUT_S_2XGA_SG,		//WORD ROM * ROM *Wa_OverDrvLut;
	sRGB2rgb_S_2XGA_SG,				//WORD ROM *Wp_SRGBTable;
	Sta_GammaLUT_S_2XGA_SG,			//gmt_GammaDescr ROM *ROM *Sta_GammaLut;
	0,								//WORD  Invert_DVS			: 1;
	0,								//WORD  Invert_DHS			: 1;
	0,								//WORD  Invert_DCLK			: 1;
	0,								//WORD  Invert_DEN			: 1;
	0xff,				//WORD  DportSwap				: 1;
	TRUE,				//WORD  LVDSInterface			: 1;
	SINGLE_BUS,//DUAL_BUS,			//WORD  W_LVDS_BusType 			: 1;
	LVDS_EIGHT_BIT_MODE,		//WORD  LvdsPanelMode	: 2; 
	0xff,				//WORD  Spread_Spect_En 		: 1; // 0xFF : Enable spectrum, 0x00 : Disable 
	NO_SWAP,				//WORD	LvdsPosNegSwap			: 1;		
	STPLF_DISPLAY_PAD_DRIVE_1_25,		// WORD PadDrive		
	STPLF_DISPLAY_LVDS_CURRENT_4_0,				//WORD  W_LvdsCurrentCtrl	: 2;			
	OUT_24_48_BIT,		//WORD  W_BitOutput				: 1;
	STPANEL_LVDS_MAP_STANDARD2,			// WORD W_LVDSSignalMap
	0x0063,				//WORD  W_LvdsClkData
};

extern ROM gmt_PanelData Panel_2XGA_SONY_S_2XGA_SG;

#else   
	#define Panel_SONY_2XGA_S_2XGA_SG	(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == SONY_2XGA_S_2XGA_SG)
		#pragma message "Default panel will not bootup unless INCLUDE_SONY_2XGA_S_2XGA_SG is defined!"
	#endif
#endif

