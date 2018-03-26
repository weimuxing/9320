/*
	$Workfile:   AX094F002E.h  $
	$Revision: 1.3 $
	$Date: 2011/05/11 06:36:07 $
*/

#ifdef INCLUDE_IPS_1080P_AX094F002E

#include "..\..\Panel\1080p\IPS\IPS4PS_2608_AX094F002E\OverdriveLut.h"
#include "..\..\Panel\1080p\IPS\IPS4PS_2608_AX094F002E\GammaLut.h"

char ROM PanelName_IPS_1080P_AX094F002E[] = "IPS_1080P_AX094F002E";

ROM WORD sRGB2rgb_AX094F002E[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};							

ROM gmt_PanelData Panel_1080P_IPS_AX094F002E =
{
	PanelName_IPS_1080P_AX094F002E,	// Panel Name
	IPS_1080P_AX094F002E,				// Panel ID (refer to enum defined in panel.h)
	PANEL_1080P,					// gmd_PanelType PanelType;
	FORMAT_ORG_2D,    // gmd_PanelDataFormat;
	TWO,							//BYTE  PixPerClock;
	8,								//BYTE  Depth;
	1920,							//WORD  Width;
	1080,							//WORD  Height;
	62,								//BYTE  MaxVFreq;
	58,								//BYTE  MinVFreq;
	147000L,						//DWORD MaxPClk;
	1980,							//WORD  W_MinHTotal;
	2000,							//WORD  W_TypHTotal;
	// TODO: UPDATE 22 and 33 H/V and VTypical totals with spreadsheet
	2070,							//WORD  W_22HTotal;
	2070,							//WORD  W_33HTotal;
	8,								//BYTE  MinHSyncWidth;
	16,								//BYTE  MinHSyncBackPorch;
	1090,							//WORD  W_22VTotal;
	1150,							//WORD  W_33VTotal;
	1150,							//WORD  MaxVtotal;
	1090,							//WORD  MinVtotal;
	1100,							//WORD  TypVTotal;
	2,								//BYTE  MinVSyncWidth;
	5,								//BYTE  MinVSyncBackPorch;
	
	0x0f0f,							//WORD  PowerUpTiming;
	0x0f0f,							//WORD  PowerDownTiming;
	0xF,							//WORD  AccToPixelVal;
	0x0,							//BYTE  SpreadSpectrumCtrl;
	0,								//BYTE  DClkDelay;
	Wa_OverDriveLUT_AX094F002E,		//WORD ROM * ROM *Wa_OverDrvLut;
	sRGB2rgb_AX094F002E,				//WORD ROM *Wp_SRGBTable;
	Sta_GammaLUT_AX094F002E,			//gmt_GammaDescr ROM *ROM *Sta_GammaLut;
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
	0,      						//WORD  PadDrive;
	SEVEN_PT_ONE_MA,				//WORD  W_LvdsCurrentCtrl	: 2;			
	OUT_24_48_BIT,					//WORD  W_BitOutput			: 1;
	STPANEL_LVDS_MAP_STANDARD1,			// WORD W_LVDSSignalMap
	0x0063,							//WORD  W_LvdsClkData;
};
extern ROM gmt_PanelData Panel_1080P_IPS_AX094F002E;

#else
	#define Panel_1080P_IPS_AX094F002E	(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == IPS_1080P_AX094F002E)
		#pragma message "Default panel will not bootup unless INCLUDE_IPS_1080P_AX094F002E is defined!"
	#endif
#endif

