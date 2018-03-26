/*
	$Workfile:   LM230WF8_TLA1.h  $
	$Revision: 1.6 $
	$Date: 2012/04/03 08:28:42 $
*/

#ifdef INCLUDE_LG_1080P_LM230WF8_TLA1

#include "..\..\Panel\1080p\LG\LM230WF8_TLA1\OverdriveLut.h"
#include "..\..\Panel\1080p\LG\LM230WF8_TLA1\GammaLut.h"

char ROM PanelName_LM230WF8_TLA1[] = "LG_1080P_LM230WF8_TLA1";

ROM WORD sRGB2rgb_LM230WF8_TLA1[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};							


ROM gmt_PanelData Panel_LG_1080P_LM230WF8_TLA1 =
{
	PanelName_LM230WF8_TLA1,		// Panel Name
	LG_1080P_LM230WF8_TLA1,			// Panel ID (refer to enum defined in panel.h)
	PANEL_1080P,					// gmd_PanelType PanelType;
	FORMAT_3D_LINE_ALTERNATIVE,				    // gmd_PanelDataFormat;
	TWO,							//BYTE  PixPerClock;
	6,								//BYTE  Depth;
	1920,							//WORD  Width;
	1080,							//WORD  Height;
	60,   //75,								//BYTE  MaxVFreq;
	50,								//BYTE  MinVFreq;
	175000L,						//DWORD MaxPClk;
	2048, //2000,							//WORD  W_MinHTotal;
	2176, //2200,							//WORD  W_TypHTotal;
	// TODO: UPDATE 22 and 33 H/V and VTypical totals with spreadsheet
	2240, //2700,							//WORD  W_22HTotal;	
	2240, //2700,							//WORD  W_33HTotal;
	16, // 8,								//BYTE  MinHSyncWidth;
	32, //16,								//BYTE  MinHSyncBackPorch;
	1100, //1090,							//WORD  W_22VTotal;			
	1100,							//WORD  W_33VTotal;
	1160, // 1200,							//WORD  MaxVtotal;
	1090, // 1088,							//WORD  MinVtotal;
	1100, // 1130,							//WORD  TypVTotal;
	2, // 6,								//BYTE  MinVSyncWidth;	
	5, // 35,								//BYTE  MinVSyncBackPorch;		
	0x0f0f,							//WORD  PowerUpTiming;
	0x0f0f,							//WORD  PowerDownTiming;
	0x14,							//WORD  AccToPixelVal;
	0x0,							//BYTE  SpreadSpectrumCtrl;
	0,								//BYTE  DClkDelay;
	Wa_OverDriveLUT_LM230WF8_TLA1,		//WORD ROM * ROM *Wa_OverDrvLut;
	sRGB2rgb_LM230WF8_TLA1,				//WORD ROM *Wp_SRGBTable;
	Sta_GammaLUT_LM230WF8_TLA1,			//gmt_GammaDescr ROM *ROM *Sta_GammaLut;
	0,								//WORD  Invert_DVS			: 1;
	0,								//WORD  Invert_DHS			: 1;
	0,								//WORD  Invert_DCLK			: 1;
	0,								//WORD  Invert_DEN			: 1;
	
	0xFF,							//WORD  DportSwap			: 1;
	TRUE,							//WORD  LVDSInterface		: 1;
	DUAL_BUS,						//WORD  W_LVDS_BusType		: 1;	
	LVDS_EIGHT_BIT_MODE,			//WORD  LvdsPanelMode		: 2;
	0xff,							//WORD  Spread_Spect_En		: 1;
	NO_SWAP,							//WORD  LvdsPosNegSwap		: 1;
	STPLF_DISPLAY_PAD_DRIVE_1_25,		// WORD PadDrive		
	STPLF_DISPLAY_LVDS_CURRENT_4_0,				//WORD  W_LvdsCurrentCtrl	: 2;			
	OUT_24_48_BIT,					//WORD  W_BitOutput			: 1;
	STPANEL_LVDS_MAP_STANDARD2,			// WORD W_LVDSSignalMap
	0x0063,							//WORD  W_LvdsClkData;
};
extern ROM gmt_PanelData Panel_1080P_LG_LM230WF8_TLA1;

#else   
	#define Panel_LG_1080P_LM230WF8_TLA1	(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == LG_1080P_LM230WF8_TLA1)
		#pragma message "Default panel will not bootup unless INCLUDE_LG_1080P_LM230WF8_TLA1 is defined!"
	#endif
#endif

