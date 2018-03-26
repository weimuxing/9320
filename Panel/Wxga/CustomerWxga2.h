/*
	$Workfile:   CustomerWxga2.h  $
	$Revision: 1.3 $
	$Date: 2011/12/15 06:22:27 $
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
// MODULE:      CustomerWxga2.h
//
// USAGE:       Header file containing the CustomerWxga2 panel specifications
//
//******************************************************************************

#ifdef INCLUDE_CUSTOMER_WXGA_DLP

char ROM PanelName_CUSTOMER_WXGA_DLP[] = "CUSTOMER_WXGA_DLP";

WORD ROM * ROM Wa_OverDriveLUT_CUSTOMER_WXGA_DLP[] =
{
	NULL_PTR,						// Signifies no more tables
};

gmt_GammaDescr ROM * ROM Sta_GammaLUT_CUSTOMER_WXGA_DLP[] =
{
	NULL_PTR						// Signifies no more gamma tables	
};

ROM WORD sRGB2rgb_CUSTOMER_WXGA_DLP[] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};								

ROM gmt_PanelData Panel_CUSTOMER_WXGA_DLP =
{
	PanelName_CUSTOMER_WXGA_DLP,			//;
	CUSTOMER_WXGA_DLP,	// Panel ID (refer to enum defined in panel.h)	
	PANEL_WXGA,			//		 	gmd_PanelType PanelType;
	FORMAT_ORG_2D,    // gmd_PanelDataFormat;
	ONE,				//BYTE PixPerClock;
	8,					//BYTE  Depth;
	1280,				//WORD  Width;
	720,					//WORD  Height;
	65,					//BYTE  MaxVFreq;
	24,					//BYTE  MinVFreq;
	74250L,				//DWORD MaxPClk; 		 // KHz
	1634,				//WORD W_MinHTotal
	1650,				//W_TypHTotal
	1634,				//W_22HTotal		
	1634,				//W_33HTotal
	4,					//BYTE  MinHSyncWidth;
	8,					//BYTE  MinHSyncBackPorch;
	947,					//W_22VTotal				
	631,					//W_33VTotal	
	750,					//WORD  MaxVtotal;	
	750,					//WORD  MinVtotal;
	750,					//WORD TypVTotal;
	2,					//BYTE  MinVSyncWidth;	
	20,					//BYTE  MinVSyncBackPorch;	
	0x775522L,			//DWORD PadDrive;
	0x9696,				//WORD  PowerUpTiming;
	0xffff,				//WORD  PowerDownTiming;
	0xC,					//WORD AccTotalPixel;
	0x0,					//BYTE  SpreadSpectrumCtrl;		// Value for Spread_Spectrum_Control register
	0,					//BYTE  DClkDelay;		 		// DCLK display timing delay adjustment in ns unit.
	Wa_OverDriveLUT_CUSTOMER_WXGA_DLP,		//OverDriveLut_CUSTOMER_WXGA_DLP,
	sRGB2rgb_CUSTOMER_WXGA_DLP,
	Sta_GammaLUT_CUSTOMER_WXGA_DLP,
	1,					//WORD  Invert_DVS 				: 1;
	1,					//WORD  Invert_DHS 				: 1;
	1,					//WORD  Invert_DCLK 			: 1;
	0,					//WORD  Invert_DEN 				: 1;
	0xff,				//WORD  DportSwap				: 1;
	TRUE,				//WORD  LVDSInterface			: 1;
	SINGLE_BUS,			//WORD  W_LVDS_BusType 			: 1;			
	NO_LVDS,			//WORD  LvdsPanelMode			: 2; 
	0xff,				//WORD  Spread_Spect_En 		: 1; // 0xFF : Enable spectrum, 0x00 : Disable 
	SWAP,				//WORD	LvdsPosNegSwap			: 1;
	FIVE_PT_SIX_MA,		//WORD  W_LvdsCurrentCtrl		: 2;		
	OUT_30_BIT,			//WORD  W_BitOutput				: 1;
	0x0063,				//WORD  W_LvdsClkData
};
extern ROM gmt_PanelData Panel_CUSTOMER_WXGA_DLP;

#else
	#define Panel_CUSTOMER_WXGA_DLP	(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == CUSTOMER_WXGA_DLP)
		#pragma message "Default panel will not bootup unless INCLUDE_CUSTOMER_WXGA_DLP is defined!"
	#endif
#endif
