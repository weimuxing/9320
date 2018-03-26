/*
	$Workfile:   CustomerWxga1.h  $
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
// MODULE:      CustomerWxga1.h    
//
// USAGE:       Header file containing the CustomerWxga1 panel specifications
//
//******************************************************************************

#ifdef INCLUDE_CUSTOMER_WXGA_PANEL_1

char ROM PanelName_WXGA_CUST_TIMING_1[] = "CUSTOMER_WXGA_1366";

ROM WORD OverDriveLut_WXGA_CUST_TIMING_1[] = 
{
	0x01C, 0x03E, 0x058, 0x076, 0x098, 0x0B9, 0x0DE, 0x177,
	0x000, 0x03C, 0x05C, 0x07B, 0x09D, 0x0BA, 0x0DE, 0x102,
	0x000, 0x022, 0x05E, 0x07F, 0x09F, 0x0BD, 0x0DE, 0x101,
	0x000, 0x021, 0x042, 0x07E, 0x09E, 0x0BD, 0x0DF, 0x0FF,
	0x000, 0x021, 0x042, 0x062, 0x09E, 0x0BE, 0x0DE, 0x0FE,
	0x000, 0x021, 0x042, 0x063, 0x082, 0x0BD, 0x0DD, 0x0FD,
	0x000, 0x022, 0x043, 0x062, 0x080, 0x0A1, 0x0DF, 0x0FF,
	0x000, 0x018, 0x037, 0x05C, 0x07B, 0x09F, 0x0BF, 0x0FF,
	0x000, 0x01A, 0x03F, 0x05F, 0x07D, 0x0D0, 0x0D7, 0x0E0,
};

WORD ROM * ROM Wa_OverDriveLUT_WXGA_CUST_TIMING_1[] =
{
	OverDriveLut_WXGA_CUST_TIMING_1,	// DEFAULT
	NULL_PTR,							// Signifies no more tables
};

gmt_GammaDescr ROM * ROM Sta_GammaLUT_WXGA_CUST_TIMING_1[] =
{
	NULL_PTR						// Signifies no more gamma tables	
};

ROM WORD sRGB2rgb_WXGA_CUST_TIMING_1[] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};								

ROM gmt_PanelData Panel_CUSTOMER_WXGA_PANEL_1 =
{
	PanelName_WXGA_CUST_TIMING_1,			//;
	CUSTOMER_WXGA_PANEL_1,	// Panel ID (refer to enum defined in panel.h)	
	PANEL_WXGA,			//		 	gmd_PanelType PanelType;
	FORMAT_ORG_2D,    // gmd_PanelDataFormat;
	ONE,				//BYTE PixPerClock;
	8,					//BYTE  Depth;
	1366,				//WORD  Width;
	768,					//WORD  Height;
	65,					//BYTE  MaxVFreq;
	58,					//BYTE  MinVFreq;
	85000L,				//DWORD MaxPClk; 		 // KHz
	1634,				//WORD W_MinHTotal
	1696,				//W_TypHTotal
	1634,				//W_22HTotal		
	1634,				//W_33HTotal
	8,					//BYTE  MinHSyncWidth;
	8,					//BYTE  MinHSyncBackPorch;
	1046,					//W_22VTotal				
	697,					//W_33VTotal		
	806,					//WORD  MaxVtotal;	// TEMPORARILY
	806,					//WORD  MinVtotal;
	806,					//WORD TypVTotal;
	2,					//BYTE  MinVSyncWidth;	
	20,					//BYTE  MinVSyncBackPorch;	
	0x775522L,			//DWORD PadDrive;
	0x9696,				//WORD  PowerUpTiming;
	0xffff,				//WORD  PowerDownTiming;
	0xD,					//WORD AccToPixelVal;
	0x0,					//BYTE  SpreadSpectrumCtrl;		// Value for Spread_Spectrum_Control register
	0,					//BYTE  DClkDelay;		 		// DCLK display timing delay adjustment in ns unit.
	Wa_OverDriveLUT_WXGA_CUST_TIMING_1,		//OverDriveLut_TOSHIBA_WXGA_1366,
	sRGB2rgb_WXGA_CUST_TIMING_1,
	Sta_GammaLUT_WXGA_CUST_TIMING_1,
	1,					//WORD  Invert_DVS 				: 1;
	1,					//WORD  Invert_DHS 				: 1;
	1,					//WORD  Invert_DCLK 			: 1;
	0,					//WORD  Invert_DEN 				: 1;
	0xff,				//WORD  DportSwap				: 1;
	TRUE,				//WORD  LVDSInterface			: 1;
	SINGLE_BUS,			//WORD  W_LVDS_BusType 			: 1;		
	NO_LVDS,			//WORD  LvdsPanelMode			: 2; 
	0xff,				//WORD  Spread_Spect_En 		: 1; // 0xFF : Enable spectrum, 0x00 : Disable 
	NO_SWAP,			//WORD	LvdsPosNegSwap			: 1;	
	FIVE_PT_SIX_MA,		//WORD  W_LvdsCurrentCtrl		: 2;	
	OUT_24_48_BIT,		//WORD  W_BitOutput				: 1;
	STPANEL_LVDS_MAP_STANDARD1,			// WORD W_LVDSSignalMap
	0x0063,				//WORD  W_LvdsClkData
};
extern ROM gmt_PanelData Panel_CUSTOMER_WXGA_PANEL_1;

#else
	#define Panel_CUSTOMER_WXGA_PANEL_1		(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == CUSTOMER_WXGA_PANEL_1)
		#pragma message "Default panel will not bootup unless INCLUDE_WXGA_CUST_TIMING_1 is defined!"
	#endif
#endif
