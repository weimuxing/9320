/*
	$Workfile:   LM190E01.h  $
	$Revision: 1.4 $
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
// MODULE:      LM190E01.h    
//
// USAGE:       Header file containing the LM190E01 panel specifications
//
//******************************************************************************

#ifdef INCLUDE_LG_SXGA_LM190E01

#include "..\..\Panel\Sxga\LG\LM190E01\OverdriveLut.h"
#include "..\..\Panel\Sxga\LG\LM190E01\GammaLut.h"

char ROM PanelName_SXGA_LG_LM190E01[] = "LG_SXGA_LM190E01";


ROM WORD sRGB2rgb_SXGA_LG_LM190E01[] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};

ROM gmt_PanelData Panel_LG_SXGA_LM190E01 =
{
	PanelName_SXGA_LG_LM190E01,			//;
	LG_SXGA_LM190E01,	// Panel ID (refer to enum defined in panel.h)	
	PANEL_SXGA,			//		 	gmd_PanelType PanelType;
	FORMAT_ORG_2D,    // gmd_PanelDataFormat;
	TWO,				//BYTE PixPerClock;
	8,					//BYTE  Depth;
	1280,				//WORD  Width;
	1024,					//WORD  Height;
	77,					//BYTE  MaxVFreq;
	50,					//BYTE  MinVFreq;
	108000L,				//DWORD MaxPClk; 		 // KHz
	1368,				//WORD W_MinHTotal	
	1618,				//W_TypHTotal
	1838,				//W_22HTotal		
	1378,				//W_33HTotal
	16,					//BYTE  MinHSyncWidth;
	30,					//BYTE  MinHSyncBackPorch;
	1046,					//W_22VTotal				
	1043,					//W_33VTotal		
	1300,					//WORD  MaxVtotal;
	1042,					//WORD  MinVtotal;
	1066,					//WORD TypVTotal;
	3,					//BYTE  MinVSyncWidth;	
	26,					//BYTE  MinVSyncBackPorch;	
	
	0x9408,				//WORD  PowerUpTiming;
	0x9408,				//WORD  PowerDownTiming;
	0xD,					//WORD AccToPixelVal;
	0x0a,					//BYTE  SpreadSpectrumCtrl;		// Value for Spread_Spectrum_Control register
	0,					//BYTE  DClkDelay;		 		// DCLK display timing delay adjustment in ns unit.
	Wa_OverDriveLUT_SXGA_LG_LM190E01,		//OverDriveLut_TOSHIBA_WXGA_1366,
	sRGB2rgb_SXGA_LG_LM190E01,
	Sta_GammaLUT_SXGA_LG_LM190E01,
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
extern ROM gmt_PanelData Panel_LG_SXGA_LM190E01;

#else
	#define Panel_LG_SXGA_LM190E01		(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == LG_SXGA_LM190E01) 
		#pragma message "Default panel will not bootup unless INCLUDE_SXGA_LG_LM190E01 is defined!"
	#endif
#endif
