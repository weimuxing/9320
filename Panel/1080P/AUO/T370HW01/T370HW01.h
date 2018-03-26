/*
	$Workfile:   T370HW01.h  $
	$Revision:   1.6  $
	$Date:   Jun 22 2006 00:44:04  $
*/

#ifdef INCLUDE_AUO_1080P_T370HW01

#include "..\..\Panel\1080p\AUO\T370HW01\OverdriveLut.h"
#include "..\..\Panel\1080p\AUO\T370HW01\GammaLut.h"

char ROM PanelName_AUO_1080P_T370HW01[] = "AUO_1080P_T370HW01";

ROM WORD sRGB2rgb_T370HW01[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};							

ROM gmt_PanelData Panel_1080P_AUO_T370HW01 =
{
	PanelName_AUO_1080P_T370HW01,	// Panel Name
	AUO_1080P_T370HW01,				// Panel ID (refer to enum defined in panel.h)
	PANEL_1080P,					// gmd_PanelType PanelType;
	FORMAT_ORG_2D,    // gmd_PanelDataFormat;
	TWO,							//BYTE  PixPerClock;
	8,								//BYTE  Depth;
	1920,							//WORD  Width;
	1080,							//WORD  Height;
	78,								//BYTE  MaxVFreq;
	46,								//BYTE  MinVFreq;
	170000L,						//DWORD MaxPClk;
	2000,							//WORD  W_MinHTotal;
	2200,							//WORD  W_TypHTotal;
	// TODO: UPDATE 22 and 33 H/V and VTypical totals with spreadsheet
	2700,							//WORD  W_22HTotal;	
	2700,							//WORD  W_33HTotal;
	8,								//BYTE  MinHSyncWidth;
	16,								//BYTE  MinHSyncBackPorch;
	1090,							//WORD  W_22VTotal;			
	1100,							//WORD  W_33VTotal;
	1200,							//WORD  MaxVtotal;
	1088,							//WORD  MinVtotal;
	1130,							//WORD  TypVTotal;
	6,								//BYTE  MinVSyncWidth;	
	35,								//BYTE  MinVSyncBackPorch;	
	
	0x9696,							//WORD  PowerUpTiming;
	0xffff,							//WORD  PowerDownTiming;
	0xF,							//WORD  AccToPixelVal;
	0x0,							//BYTE  SpreadSpectrumCtrl;
	0,								//BYTE  DClkDelay;
	Wa_OverDriveLUT_T370HW01,		//WORD ROM * ROM *Wa_OverDrvLut;
	sRGB2rgb_T370HW01,				//WORD ROM *Wp_SRGBTable;
	Sta_GammaLUT_T370HW01,			//gmt_GammaDescr ROM *ROM *Sta_GammaLut;
	0,								//WORD  Invert_DVS			: 1;
	0,								//WORD  Invert_DHS			: 1;
	0,								//WORD  Invert_DCLK			: 1;
	0,								//WORD  Invert_DEN			: 1;
	0xff,							//WORD  DportSwap			: 1;
	TRUE,							//WORD  LVDSInterface		: 1;
	DUAL_BUS,						//WORD  W_LVDS_BusType		: 1;	
	LVDS_EIGHT_BIT_MODE,			//WORD  LvdsPanelMode		: 2;
	0xff,							//WORD  Spread_Spect_En		: 1;
	SWAP,							//WORD  LvdsPosNegSwap		: 1;
	0,								//WORD  PadDrive;
	SEVEN_PT_ONE_MA,				//WORD  W_LvdsCurrentCtrl	: 2;			
	OUT_24_48_BIT,					//WORD  W_BitOutput			: 1;
	STPANEL_LVDS_MAP_STANDARD1,			// WORD W_LVDSSignalMap
	0x0063,							//WORD  W_LvdsClkData;
};
extern ROM gmt_PanelData Panel_1080P_AUO_T370HW01;

#else
	#define Panel_1080P_AUO_T370HW01	(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == AUO_1080P_T370HW01)
		#pragma message "Default panel will not bootup unless INCLUDE_AUO_1080P_T370HW01 is defined!"
	#endif
#endif

