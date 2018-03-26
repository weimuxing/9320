/*
	$Workfile:   LTI460HM01.h  $
	$Revision:   1.10  $
	$Date:   Dec 12 2008 15:55:32  $
*/

#ifdef INCLUDE_SAMSUNG_LTI460HM01

#include "..\..\Panel\1080P\Samsung\LTI460HM01\OverdriveLut.h"
#include "..\..\Panel\1080P\Samsung\LTI460HM01\GammaLut.h"

char ROM PanelName_LTI460HM01[] = "SAMSUNG_LTI460HM01";

ROM WORD sRGB2rgb_LTI460HM01[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};

ROM gmt_PanelData Panel_SAMSUNG_LTI460HM01 =
{
	PanelName_LTI460HM01,
	SAMSUNG_LTI460HM01,			// Panel ID (refer to enum defined in panel.h)
	PANEL_1080P,						// gmd_PanelType PanelType
	TWO,								// BYTE PixPerClock
	8,									// BYTE  Depth
	1920,								// WORD  Width
	1080,								// WORD  Height
	65,									// BYTE  MaxVFreq
	45,									// BYTE  MinVFreq
	160000L,								// DWORD MaxPClk (KHz)
	2090,								// WORD W_MinHTotal
	2200,								// W_TypHTotal
	2200,								// W_22HTotal
	2200,								// W_33HTotal
	8,									//BYTE  MinHSyncWidth;
	16,									//BYTE  MinHSyncBackPorch;
	1125,				//W_22VTotal
	1125,				//W_33VTotal
	1380,								// WORD  MaxVtotal
	1092,								// WORD  MinVtotal
	1125,								// WORD TypVTotal
	6,					//BYTE  MinVSyncWidth;
	35,					//BYTE  MinVSyncBackPorch;
	0x775522L,			//DWORD PadDrive;
// @Modify By Ray --Start-- 2010-04-26	modify panel/inverter power sequence.
    	0xA010,	//0xA012,//0xff08,       	//WORD  PowerUpTiming; // @Modify By Ray --Start-- 2010-03-05	//20100106 Steward modified for panel spec.
    	0x0140,	//0x08FA,//0x0826,       	//WORD  PowerDownTiming;     //Andy Chiu  20091113  For FQ request
// @Modify By Ray --End-- 2010-04-26
	0x14,	// @Modify By Ray --Start-- 2011-01-10 modify AccToPixelVal to 0x14(spec).	//0xF,					//WORD AccToPixelVal;
	0x604,	//0x603,// @Modify By Ray --Start-- 2011-01-12 modify SSC setting for EMI request.	//0x0,					//BYTE  SpreadSpectrumCtrl;		// Value for Spread_Spectrum_Control register
	0,									// DCLK display timing delay adjustment in ns unit
	Wa_OverDriveLUT_LTI460HM01,		// Overdrive look up table
	sRGB2rgb_LTI460HM01,
	Sta_GammaLUT_LTI460HM01,
	0,									// WORD  Invert_DVS
	0,									// WORD  Invert_DHS
	0,									// WORD  Invert_DCLK
	0,									// WORD  Invert_DEN
	0,									// WORD  DportSwap
	TRUE,								// WORD  LVDSInterface
	DUAL_BUS,							// WORD  W_LVDS_BusType
	LVDS_EIGHT_BIT_MODE,				// WORD  LvdsPanelMode
	0xFF,								// WORD  Spread_Spect_En
	NO_SWAP,								// WORD	LvdsPosNegSwap
	FOUR_PT_SIX_MA,			//Bossen, Follow TPV FQ request.	20091028.	//Bossen,	follow TPV request.	20091022.	SEVEN_PT_ONE_MA,					// WORD  W_LvdsCurrentCtrl
	OUT_24_48_BIT,						// WORD  W_BitOutput
	0x0063,								// WORD  W_LvdsClkData
};

extern ROM gmt_PanelData Panel_SAMSUNG_LTI460HM01;

#else
	#define Panel_SAMSUNG_LTI460HM01	(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == SAMSUNG_LTI460HM01)
		#pragma message "Default panel will not bootup unless INCLUDE_SAMSUNG_LTI460HM01 is defined!"
	#endif
#endif
