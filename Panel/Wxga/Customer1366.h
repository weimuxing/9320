/*
	$Workfile:   Customer1366.h  $
	$Revision: 1.4 $
	$Date: 2012/02/14 08:19:29 $
*/

//******************************************************************************
// COPYRIGHT (C) STMicroelectronics 2011.
//
// All rights reserved. This document contains proprietary and
// confidential information of the STMicroelectronics Group. This
// document is not to be copied in whole or part. STMicroelectronics
// assumes no responsibility for the consequences of use of such
// information nor for any infringement of patents or other rights of
// third parties which may result from its use. No license is granted by
// implication or otherwise under any patent or patent rights of
// STMicroelectronics.
// STMicroelectronics
// products are not authorized for use as critical components in life
// support devices or systems without express written approval of
// STMicroelectronics.
//==============================================================================
//
// MODULE:      Customer1366.h
//
// USAGE:       Header file containing the Customer panel (1366x768) specifications
//
//******************************************************************************

#ifdef INCLUDE_CUSTOMER_WXGA_1366

char ROM PanelName_CUSTOMER_WXGA_1366[] = "CUSTOMER_WXGA_1366";

ROM WORD OverDriveLut_CUSTOMER_WXGA_1366[] =
{
	0x41, 0x7D, 0x9C, 0xB3, 0xC7, 0xD9, 0xF9, 0x119,
	0x0, 0x56, 0x7D, 0x9C, 0xBA, 0xD2, 0xED, 0x105,
	0x0, 0xF, 0x6A, 0x90, 0xB2, 0xCE, 0xEB, 0x103,
	0x0, 0xA, 0x32, 0x87, 0xAC, 0xCA, 0xE8, 0x102,
	0x0, 0x8, 0x28, 0x56, 0xA6, 0xC6, 0xE6, 0x101,
	0x0, 0x7, 0x21, 0x4D, 0x78, 0xC3, 0xE4, 0x100,
	0x0, 0x6, 0x1C, 0x46, 0x71, 0x9A, 0xE2, 0xFF,
	0x3FF, 0x5, 0x18, 0x3C, 0x68, 0x92, 0xBA, 0xFF,
	0x3FF, 0x4, 0x15, 0x34, 0x60, 0x8C, 0xB5, 0xDD,
};

WORD ROM * ROM Wa_OverDriveLUT_CUSTOMER_WXGA_1366[] =
{
	OverDriveLut_CUSTOMER_WXGA_1366,
	NULL_PTR,						// Signifies no more tables
};

gmt_GammaDescr ROM * ROM Sta_GammaLUT_CUSTOMER_WXGA_1366[] =
{
	NULL_PTR						// Signifies no more gamma tables	
};

ROM WORD sRGB2rgb_CUSTOMER_WXGA_1366[] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};								

ROM gmt_PanelData Panel_CUSTOMER_WXGA_1366 =
{
	PanelName_CUSTOMER_WXGA_1366,			//;
	CUSTOMER_WXGA_1366,	// Panel ID (refer to enum defined in panel.h)	
	PANEL_WXGA,			//gmd_PanelType PanelType;
	FORMAT_ORG_2D,    // gmd_PanelDataFormat;
	ONE,				//BYTE PixPerClock;
	8,					//BYTE  Depth;
	1366,				//WORD  Width;
	768,					//WORD  Height;
	63,					//BYTE  MaxVFreq;
	47,					//BYTE  MinVFreq;
	85000L,				//DWORD MaxPClk; 		 // KHz
	1500,				//WORD W_MinHTotal
	1728,				//W_TypHTotal
	2160,				//W_22HTotal		
	1440,				//W_33HTotal
	48,					//BYTE  MinHSyncWidth;
	32,					//BYTE  MinHSyncBackPorch;
	806,					//W_22VTotal				
	806,					//W_33VTotal			
	980,					//WORD  MaxVtotal;
	800,					//WORD  MinVtotal;
	806,					//WORD TypVTotal;
	3,					//BYTE  MinVSyncWidth;	
	16,					//BYTE  MinVSyncBackPorch;	
	0x775522L,			//DWORD PadDrive;
	0x9696,				//WORD  PowerUpTiming;
	0xffff,				//WORD  PowerDownTiming;
	0xD,					//WORD AccToPixelVal;
	0x0,					//BYTE  SpreadSpectrumCtrl;		// Value for Spread_Spectrum_Control register
	0,					//BYTE  DClkDelay;		 		// DCLK display timing delay adjustment in ns unit.
	Wa_OverDriveLUT_CUSTOMER_WXGA_1366,	//OverDriveLut_CUSTOMER_WXGA_1366,
	sRGB2rgb_CUSTOMER_WXGA_1366,
	Sta_GammaLUT_CUSTOMER_WXGA_1366,
	0,					//WORD  Invert_DVS 				: 1;
	0,					//WORD  Invert_DHS 				: 1;
	0,					//WORD  Invert_DCLK 			: 1;
	0,					//WORD  Invert_DEN 				: 1;
	0xff,				//WORD  DportSwap				: 1;
	FALSE,				//WORD  LVDSInterface			: 1;
	SINGLE_BUS,			//WORD  W_LVDS_BusType 			: 1;		
	NO_LVDS,		//WORD  LvdsPanelMode				: 2; 
	0xff,				//WORD  Spread_Spect_En 		: 1; // 0xFF : Enable spectrum, 0x00 : Disable 
	SWAP,				//WORD	LvdsPosNegSwap			: 1;
	FIVE_PT_SIX_MA,		//WORD  W_LvdsCurrentCtrl		: 2;			
	OUT_24_48_BIT,		//WORD  W_BitOutput				: 1;
	STPANEL_LVDS_MAP_STANDARD1,			// WORD W_LVDSSignalMap
	0x0063,				//WORD  W_LvdsClkData
};
extern ROM gmt_PanelData Panel_CUSTOMER_WXGA_1366;

#else
	#define Panel_CUSTOMER_WXGA_1366	(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == CUSTOMER_WXGA_1366)
		#pragma message "Default panel will not bootup unless INCLUDE_CUSTOMER_WXGA_1366 is defined!"
	#endif
#endif
