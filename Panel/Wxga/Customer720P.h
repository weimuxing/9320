/*
	$Workfile:   Customer720P.h  $
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
// MODULE:      Customer720P.h
//
// USAGE:       Header file containing the Customer720P panel specifications
//
//******************************************************************************

#ifdef INCLUDE_CUSTOMER_WXGA_720P


char ROM PanelName_CUSTOMER_WXGA_720P[] = "CUSTOMER_720P";

WORD ROM * ROM Wa_OverDriveLUT_CUSTOMER_WXGA_720P[] =
{
	NULL_PTR,						// Signifies no more tables
};

gmt_GammaDescr ROM * ROM Sta_GammaLUT_CUSTOMER_WXGA_720P[] =
{
	NULL_PTR						// Signifies no more gamma tables	
};

ROM WORD sRGB2rgb_CUSTOMER_WXGA_720P[] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};								

ROM gmt_PanelData Panel_CUSTOMER_WXGA_720P =
{
	PanelName_CUSTOMER_WXGA_720P,			//;
	CUSTOMER_WXGA_720P,	// Panel ID (refer to enum defined in panel.h)	
	PANEL_WXGA,			//gmd_PanelType PanelType;
	FORMAT_ORG_2D,    // gmd_PanelDataFormat;
	ONE,				//BYTE PixPerClock;
	8,					//BYTE  Depth;
	1280,				//WORD  Width;
	720,					//WORD  Height;
	77,					//BYTE  MaxVFreq;
	50,					//BYTE  MinVFreq;
	80000L,				//DWORD MaxPClk; 		 // KHz
	1352,				//WORD W_MinHTotal
	1650,				//W_TypHTotal (1650 for NTSC 60Hz)
//	1980,				//W_TypHTotal (1980 for PAL  50Hz)
	2436,				//W_22HTotal		
	1624,				//W_33HTotal
	44,					//BYTE  MinHSyncWidth;
	218,					//BYTE  MinHSyncBackPorch;
	762,					//W_22VTotal				
	762,					//W_33VTotal	
	780,					//WORD  MaxVtotal;	
	750,					//WORD  MinVtotal;
	750,					//WORD TypVTotal;
	5,					//BYTE  MinVSyncWidth;	
	10,					//BYTE  MinVSyncBackPorch;	
	0x775522L,			//DWORD PadDrive;
	0x9696,				//WORD  PowerUpTiming;
	0xffff,				//WORD  PowerDownTiming;
	0xC,					//WORD AccTotalPixel;
	0x0,					//BYTE  SpreadSpectrumCtrl;		// Value for Spread_Spectrum_Control register
	0,					//BYTE  DClkDelay;		 		// DCLK display timing delay adjustment in ns unit.
	Wa_OverDriveLUT_CUSTOMER_WXGA_720P,			//OverDriveLut_CUSTOMER_WXGA_720P,
	sRGB2rgb_CUSTOMER_WXGA_720P,
	Sta_GammaLUT_CUSTOMER_WXGA_720P,
	0,					//WORD  Invert_DVS 				: 1;
	0,					//WORD  Invert_DHS 				: 1;
	0,					//WORD  Invert_DCLK 			: 1;
	0,					//WORD  Invert_DEN 				: 1;
	0xff,				//WORD  DportSwap				: 1;
	FALSE,				//WORD  LVDSInterface			: 1;
	SINGLE_BUS,			//WORD  W_LVDS_BusType 			: 1;		
	LVDS_EIGHT_BIT_MODE,//WORD  LvdsPanelMode			: 2; 
	0xff,				//WORD  Spread_Spect_En 		: 1; // 0xFF : Enable spectrum, 0x00 : Disable 
	SWAP,				//WORD	LvdsPosNegSwap			: 1;
	FIVE_PT_SIX_MA,		//WORD  W_LvdsCurrentCtrl		: 2;		
	OUT_24_48_BIT,		//WORD  W_BitOutput				: 1;
	STPANEL_LVDS_MAP_STANDARD1,			// WORD W_LVDSSignalMap
	0x0063,				//WORD  W_LvdsClkData
};
extern ROM gmt_PanelData Panel_CUSTOMER_WXGA_720P;

#else
	#define Panel_CUSTOMER_WXGA_720P	(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == CUSTOMER_WXGA_720P)
		#pragma message "Default panel will not bootup unless INCLUDE_CUSTOMER_WXGA_720P is defined!"
	#endif
#endif
