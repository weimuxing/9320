/*
	$Workfile:   LC370WU1.h  $
	$Revision:   1.1  $
	$Date:   Dec 15 2006 14:34:12  $
*/

#ifdef INCLUDE_LG_1080P_LC370WU1

#include "..\..\Panel\1080p\LG\LC370WU1\OverdriveLut.h"
#include "..\..\Panel\1080p\LG\LC370WU1\GammaLut.h"

char ROM PanelName_LG_1080P_LC370WU1[] = "LG_1080P_LC370WU1";

ROM WORD sRGB2rgb_LC370WU1[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};							

ROM gmt_PanelData Panel_1080P_LG_LC370WU1 =
{
	PanelName_LG_1080P_LC370WU1,	// Panel Name
	LG_1080P_LC370WU1,				// Panel ID (refer to enum defined in panel.h)
	PANEL_1080P,					// gmd_PanelType PanelType;
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
	0x775522L,						//DWORD PadDrive;
	0x0f0f,							//WORD  PowerUpTiming;
	0x0f0f,							//WORD  PowerDownTiming;
	0xF,							//WORD  AccToPixelVal;
	0x0,							//BYTE  SpreadSpectrumCtrl;
	0,								//BYTE  DClkDelay;
	Wa_OverDriveLUT_LC370WU1,		//WORD ROM * ROM *Wa_OverDrvLut;
	sRGB2rgb_LC370WU1,				//WORD ROM *Wp_SRGBTable;
	Sta_GammaLUT_LC370WU1,			//gmt_GammaDescr ROM *ROM *Sta_GammaLut;
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
	SEVEN_PT_ONE_MA,				//WORD  W_LvdsCurrentCtrl	: 2;			
	OUT_24_48_BIT,					//WORD  W_BitOutput			: 1;
	0x0063,							//WORD  W_LvdsClkData;
};
extern ROM gmt_PanelData Panel_1080P_LG_LC370WU1;

#else
	#define Panel_1080P_LG_LC370WU1	(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == LG_1080P_LC370WU1)
		#pragma message "Default panel will not bootup unless INCLUDE_LG_1080P_LC370WU1 is defined!"
	#endif
#endif

