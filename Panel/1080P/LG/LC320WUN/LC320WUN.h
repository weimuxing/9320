/*
	$Workfile:   LC320WUN.h  $
	$Revision:   1.0  $
	$Date:   May 12 2008 09:51:18  $		//Peter 10090120
*/
#ifdef INCLUDE_LG_1080P_LC320WUN

#include "..\..\Panel\1080P\LG\LC320WUN\OverdriveLut.h"
#include "..\..\Panel\1080P\LG\LC320WUN\GammaLut.h"


char ROM PanelName_LG_1080P_LC320WUN[] = "LG_LC320WUN";

ROM WORD sRGB2rgb_LC320WUN[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};

//	gm_SetRegBitsWord(LVDS_DIGITAL_CTRL, EVEN_ODD_SWAP);

ROM gmt_PanelData Panel_1080P_LG_LC320WUN =
{
	PanelName_LG_1080P_LC320WUN,	// Panel Name
	LG_1080P_LC320WUN,				// Panel ID (refer to enum defined in panel.h)
	PANEL_1080P,			//gmd_PanelType PanelType;
	TWO,				//BYTE PixPerClock;
	10,					//BYTE  Depth;
	1920,				//WORD  Width;
	1080,				//WORD  Height;
	62,//63, //Andy Chiu 20110926 for MQE request					//BYTE  MaxVFreq;
	58,//57, //Andy Chiu 20110926 for MQE request					//BYTE  MinVFreq;
	154000L,				//DWORD MaxPClk; 		 // KHz
	2120,				//WORD  W_MinHTotal;
   	2200,//2080,       			//W_TypHTotal   
	2400,//2700,				//W_22HTotal
	2400,//2700,				//W_33HTotal
	8,					//BYTE  MinHSyncWidth;
	16,					//BYTE  MinHSyncBackPorch;
	1090,				//W_22VTotal
	1100,				//W_33VTotal
	1149,				//WORD  MaxVtotal;
	1091,				//WORD  MinVtotal;
	1125,				//WORD TypVTotal;
	6,					//BYTE  MinVSyncWidth;
	35,					//BYTE  MinVSyncBackPorch;
	0x775522L,			//DWORD PadDrive;
    0xA012,//0xff08,       	//WORD  PowerUpTiming; // @Modify By Ray --Start-- 2010-01-28	//20100106 Steward modified for panel spec.
    0x20FA,	//=>Andy Chiu 20110804  0x08FA,//0x0826,       	//WORD  PowerDownTiming;     //Andy Chiu  20091113  For FQ request
	0x14,//1080p  0xF,					//WORD AccToPixelVal;
	0x603,//0x0,					//BYTE  SpreadSpectrumCtrl;		// Value for Spread_Spectrum_Control register
	0,					//BYTE  DClkDelay;		 		// DCLK display timing delay adjustment in ns unit.
	Wa_OverDriveLUT_LC320WUN, 	//OverDriveLut_LK520D3LZ8x,
	sRGB2rgb_LC320WUN,
	Sta_GammaLUT_LC320WUN,
	0,					//WORD  Invert_DVS 			: 1;
	0,					//WORD  Invert_DHS 			: 1;
	0,					//WORD  Invert_DCLK 			: 1;
	0,					//WORD  Invert_DEN 			: 1;
	0,					//WORD  DportSwap				: 1;
	TRUE,				//WORD  LVDSInterface			: 1;
	DUAL_BUS,			//WORD  W_LVDS_BusType 		: 1;
    	LVDS_EIGHT_BIT_MODE,        	//WORD  LvdsPanelMode   : 2; // Control Pin -> Lo
	0xff,				//WORD  Spread_Spect_En 		: 1; // 0xFF : Enable spectrum, 0x00 : Disable
	NO_SWAP,			//WORD	LvdsPosNegSwap		: 1;
	THREE_PT_SIX_MA,	//=>Andy Chiu 20110804 for VID  //FOUR_PT_SIX_MA,			//WORD  W_LvdsCurrentCtrl		: 2;			/
	OUT_24_48_BIT,			//WORD  W_BitOutput			: 1;
	0x0063,				//WORD	W_LvdsClkData
};
extern ROM gmt_PanelData Panel_1080P_LG_LC320WUN;

#else
	#define Panel_1080P_LG_LC320WUN	(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == LG_1080P_LC320WUN)
		#pragma message "Default panel will not bootup unless INCLUDE_LG_1080P_LC320WUN is defined!"
	#endif
#endif
