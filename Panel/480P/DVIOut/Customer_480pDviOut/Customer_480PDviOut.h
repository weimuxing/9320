/*
	$Workfile:   Customer_480PDviOut.h  $
	$Revision: 1.4 $
	$Date: 2011/12/15 06:22:26 $
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
// MODULE:      Customer480P.h
//
// USAGE:       Header file containing the Customer480P panel specifications
//
//******************************************************************************

#ifdef INCLUDE_CUSTOMER_480P_DVIOUT

#include "..\..\Panel\480P\DVIOut\Customer_480pDviOut\OverdriveLut.h"
#include "..\..\Panel\480P\DVIOut\Customer_480pDviOut\GammaLut.h"

char ROM PanelName_CUSTOMER_480P_DVIOUT[] = "CUSTOMER_480P_DVIOUT";

ROM WORD sRGB2rgb_CUSTOMER_480P_DVIOUT[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};								

ROM gmt_PanelData Panel_CUSTOMER_480P_DVIOUT =
{
	PanelName_CUSTOMER_480P_DVIOUT,			//;
	CUSTOMER_480P_DVIOUT,	// Panel ID (refer to enum defined in panel.h)	
	PANEL_480P,							// gmd_PanelType PanelType
	FORMAT_ORG_2D,    // gmd_PanelDataFormat;
	ONE,				//BYTE PixPerClock;
	8,					//BYTE  Depth;
	720,				//WORD  Width;
	480,					//WORD  Height;
	78,					//BYTE  MaxVFreq;
	25,					//BYTE  MinVFreq;
	30000L,				//DWORD MaxPClk; 		 // KHz
	858,				//WORD W_MinHTotal
	858,				//W_TypHTotal
	// TODO: UPDATE 22 and 33 H/V and VTypical totals with spreadsheet
	//22H, 23H, 22V and 23V are calculated by "Film Mode Timing Tool_F85XX-OTH-03A.xls"
	858,				//W_22HTotal		
	858,				//W_33HTotal
	62, //50,					//BYTE  MinHSyncWidth;
	60, //180,				//BYTE  MinHSyncBackPorch;
	525,				//W_22VTotal				
	525,				//W_33VTotal 
 	525,				//WORD  MaxVtotal;
	525,				//WORD  MinVtotal;
	525,				//WORD TypVTotal;       
	6, //3,					//BYTE  MinVSyncWidth;	
	30, //17,					//BYTE  MinVSyncBackPorch;	
	
	0x9696,				//WORD  PowerUpTiming;
	0xffff,				//WORD  PowerDownTiming;
	0xC,					//WORD AccTotalPixel;
	0x0,					//BYTE  SpreadSpectrumCtrl;		// Value for Spread_Spectrum_Control register
	1,					//BYTE  DClkDelay;		 		// DCLK display timing delay adjustment in ns unit.
	Wa_OverDriveLUT_CUSTOMER_480P_DVIOUT,			//OverDriveLut_CUSTOMER_WXGA_720P,
	sRGB2rgb_CUSTOMER_480P_DVIOUT,
	Sta_GammaLUT_CUSTOMER_480P_DVIOUT,
	1,					//WORD  Invert_DVS 				: 1;
	1,					//WORD  Invert_DHS 				: 1;
	0,					//WORD  Invert_DCLK 			: 1;
	0,					//WORD  Invert_DEN 				: 1;
	
	0xff,				//WORD  DportSwap				: 1;
	FALSE,				//WORD  LVDSInterface			: 1;
	SINGLE_BUS,			//WORD  W_LVDS_BusType 			: 1;		
	NO_LVDS,			//WORD  LvdsPanelMode				: 2; 
	0xff,				//WORD  Spread_Spect_En 		: 1; // 0xFF : Enable spectrum, 0x00 : Disable 
	SWAP,				//WORD	LvdsPosNegSwap			: 1;
	0,					//DWORD PadDrive;
	SEVEN_PT_ONE_MA,	//WORD  W_LvdsCurrentCtrl		: 2;			
	OUT_24_48_BIT,		//WORD  W_BitOutput				: 1;
	STPANEL_LVDS_MAP_STANDARD1,			// WORD W_LVDSSignalMap
	0x0063,				//WORD  W_LvdsClkData
};
extern ROM gmt_PanelData Panel_CUSTOMER_480P_DVIOUT;

#else
	#define Panel_CUSTOMER_480P_DVIOUT	(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == CUSTOMER_480P_DVIOUT)
		#pragma message "Default panel will not bootup unless INCLUDE_CUSTOMER_480P_DVIOUT is defined!"
	#endif
#endif
