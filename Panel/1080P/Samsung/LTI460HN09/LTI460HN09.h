/*
	$Workfile:   LTI460HN09.h  $
	$Revision:   1.0  $
	$Date:   May 12 2008 09:51:18  $		//Peter 10090120
*/
#ifdef INCLUDE_SAMSUNG_1080P_LTI460HN09

#include "..\..\Panel\1080P\Samsung\LTI460HN09\OverdriveLut.h"
#include "..\..\Panel\1080P\Samsung\LTI460HN09\GammaLut.h"


char ROM PanelName_SAMSUNG_LTI460HN09[] = "SAMSUNG_LTI460HN09";

ROM WORD sRGB2rgb_LTI460HN09[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};

//	gm_SetRegBitsWord(LVDS_DIGITAL_CTRL, EVEN_ODD_SWAP);

ROM gmt_PanelData Panel_1080P_SAMSUNG_LTI460HN09 =
{
	PanelName_SAMSUNG_LTI460HN09,	// Panel Name
	SAMSUNG_1080P_LTI460HN09,				// Panel ID (refer to enum defined in panel.h)
	PANEL_1080P,					// gmd_PanelType PanelType;
	FORMAT_ORG_2D,    // gmd_PanelDataFormat;
	TWO,							//BYTE  PixPerClock;
	8,								//BYTE  Depth;
	1920,							//WORD  Width;
	1080,							//WORD  Height;
	63,								//BYTE  MaxVFreq;
	57,								//BYTE  MinVFreq;
	156000L,						//DWORD MaxPClk;
	2120,							//WORD  W_MinHTotal;
	2200,							//WORD  W_TypHTotal;
	// TODO: UPDATE 22 and 33 H/V and VTypical totals with spreadsheet
	2200,							//WORD  W_22HTotal;	
	2200,							//WORD  W_33HTotal;
	8,								//BYTE  MinHSyncWidth;
	16,								//BYTE  MinHSyncBackPorch;
	1125,							//WORD  W_22VTotal;			
	1125,							//WORD  W_33VTotal;
	1149,							//WORD  MaxVtotal;
	1100,							//WORD  MinVtotal;
	1125,							//WORD  TypVTotal;
	6,								//BYTE  MinVSyncWidth;	
	35,								//BYTE  MinVSyncBackPorch;	
	
	0xFF10,							//WORD  PowerUpTiming;
	0x0140,							//WORD  PowerDownTiming;
	0x21,//0xF,							//WORD  AccToPixelVal;
	0x1203,							//BYTE  SpreadSpectrumCtrl;
	0,								//BYTE  DClkDelay;
	Wa_OverDriveLUT_LTI460HN09,		// Overdrive look up table
	sRGB2rgb_LTI460HN09,
	Sta_GammaLUT_LTI460HN09,
	0,								//WORD  Invert_DVS			: 1;
	0,								//WORD  Invert_DHS			: 1;
	0,								//WORD  Invert_DCLK			: 1;
	0,								//WORD  Invert_DEN			: 1;
	0x0,							//WORD  DportSwap			: 1;
	TRUE,							//WORD  LVDSInterface		: 1;
	DUAL_BUS,						//WORD  W_LVDS_BusType		: 1;	
	LVDS_EIGHT_BIT_MODE,			//WORD  LvdsPanelMode		: 2;
	0xff,							//WORD  Spread_Spect_En		: 1;
	NO_SWAP,							//WORD  LvdsPosNegSwap		: 1;
	STPLF_DISPLAY_PAD_DRIVE_1_25,//STPLF_DISPLAY_PAD_DRIVE_1_25,//0,      						//WORD  PadDrive;
	STPLF_DISPLAY_LVDS_CURRENT_3_5,//SEVEN_PT_ONE_MA,				//WORD  W_LvdsCurrentCtrl	: 2;			
	OUT_24_48_BIT,					//WORD  W_BitOutput			: 1;
	STPANEL_LVDS_MAP_STANDARD1,//STPANEL_LVDS_MAP_STANDARD2,			// WORD W_LVDSSignalMap
	0x0063,							//WORD  W_LvdsClkData;
};

extern ROM gmt_PanelData Panel_1080P_SAMSUNG_LTI460HN09;
#else
	#define Panel_1080P_SAMSUNG_LTI460HN09	(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == SAMSUNG_1080P_LTI460HN09)
		#pragma message "Default panel will not bootup unless INCLUDE_SAMSUNG_1080P_LTI460HN09 is defined!"
	#endif
#endif
