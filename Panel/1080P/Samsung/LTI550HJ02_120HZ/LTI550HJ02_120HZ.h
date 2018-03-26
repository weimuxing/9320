/*
	$Workfile:   LTI550HJ02_120HZ.h  $
	$Revision:   1.0  $
	$Date:   May 12 2008 09:51:18  $		//Peter 10090120
*/
#ifdef INCLUDE_SAMSUNG_LTI550HJ02_120HZ

#include "..\..\Panel\1080P\Samsung\LTI550HJ02_120HZ\OverdriveLut.h"
#include "..\..\Panel\1080P\Samsung\LTI550HJ02_120HZ\GammaLut.h"


char ROM PanelName_SAMSUNG_LTI550HJ02_120HZ[] = "SAMSUNG_LTI550HJ02_120HZ";

ROM WORD sRGB2rgb_LTI550HJ02_120HZ[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};

//	gm_SetRegBitsWord(LVDS_DIGITAL_CTRL, EVEN_ODD_SWAP);

ROM gmt_PanelData Panel_1080P_SAMSUNG_LTI550HJ02_120HZ =
{
	PanelName_SAMSUNG_LTI550HJ02_120HZ,	// Panel Name
	SAMSUNG_LTI550HJ02_120HZ,				// Panel ID (refer to enum defined in panel.h)
	PANEL_1080P,			//gmd_PanelType PanelType;
	TWO,				//BYTE PixPerClock;
	10,					//BYTE  Depth;
	1920,				//WORD  Width;
	1080,				//WORD  Height;
#if MEMC_FUNCTION == FALSE //KevinL 20110413* Output to MEMC or Panel. 
	63,//63,					//BYTE  MaxVFreq;
	47,//57,					//BYTE  MinVFreq;
	158000L,				//DWORD MaxPClk; 		 // KHz
	2100,				//WORD  W_MinHTotal;
   	2200,//2080,       			//W_TypHTotal   
	2220,//2700,				//W_22HTotal
	2220,//2700,				//W_33HTotal
	8,					//BYTE  MinHSyncWidth;
	16,					//BYTE  MinHSyncBackPorch;
	1120,				//W_22VTotal
	1120,				//W_33VTotal
	1135,				//WORD  MaxVtotal;
	1120,				//WORD  MinVtotal;
	1130,				//WORD TypVTotal;
	6,					//BYTE  MinVSyncWidth;
	35,					//BYTE  MinVSyncBackPorch;		
#else
	61,					//BYTE  MaxVFreq;
	59,					//BYTE  MinVFreq;
	148500L,				//DWORD MaxPClk; 		 // KHz
	2100,				//WORD  W_MinHTotal;
   	2200,       			//W_TypHTotal   
	2200,				//W_22HTotal
	2200,				//W_33HTotal
	8,					//BYTE  MinHSyncWidth;
	16,					//BYTE  MinHSyncBackPorch;
	1125,				//W_22VTotal
	1125,				//W_33VTotal
	1125,				//WORD  MaxVtotal;
	1125,				//WORD  MinVtotal;
	1125,				//WORD TypVTotal;
	6,					//BYTE  MinVSyncWidth;
	35,					//BYTE  MinVSyncBackPorch;	
#endif	
	0x775522L,			//DWORD PadDrive;
// @Modify By Ray --Start-- 2010-04-26	modify panel/inverter power sequence.
    	0xFF10,	//0xA010,	//0xA012,//0xff08,       	//WORD  PowerUpTiming; // @Modify By Ray --Start-- 2010-03-05	//20100106 Steward modified for panel spec.
    	0x0140,	//0x08FA,//0x0826,       	//WORD  PowerDownTiming;     //Andy Chiu  20091113  For FQ request
// @Modify By Ray --End-- 2010-04-26
	0x14,	// @Modify By Ray --Start-- 2011-01-10 modify AccToPixelVal to 0x14(spec).	//0xF,					//WORD AccToPixelVal;
	0x603,//0x0,					//BYTE  SpreadSpectrumCtrl;		// Value for Spread_Spectrum_Control register
	0,					//BYTE  DClkDelay;		 		// DCLK display timing delay adjustment in ns unit.
	Wa_OverDriveLUT_LTI550HJ02_120HZ, 	//OverDriveLut_LK520D3LZ8x,
	sRGB2rgb_LTI550HJ02_120HZ,
	Sta_GammaLUT_LTI550HJ02_120HZ,
	0,					//WORD  Invert_DVS 			: 1;
	0,					//WORD  Invert_DHS 			: 1;
	0,					//WORD  Invert_DCLK 			: 1;
	0,					//WORD  Invert_DEN 			: 1;
	0,					//WORD  DportSwap				: 1;
	TRUE,				//WORD  LVDSInterface			: 1;
	DUAL_BUS,			//WORD  W_LVDS_BusType 		: 1;
    	LVDS_EIGHT_BIT_MODE,        	//WORD  LvdsPanelMode   : 2; // Control Pin -> Lo
	0x00,				//WORD  Spread_Spect_En 		: 1; // 0xFF : Enable spectrum, 0x00 : Disable
	NO_SWAP,			//WORD	LvdsPosNegSwap		: 1;
	FOUR_PT_SIX_MA,			//WORD  W_LvdsCurrentCtrl		: 2;			/
	OUT_24_48_BIT,			//WORD  W_BitOutput			: 1;
	0x0063,				//WORD	W_LvdsClkData
};
extern ROM gmt_PanelData Panel_1080P_SAMSUNG_LTI550HJ02_120HZ;

#else
	#define Panel_1080P_SAMSUNG_LTI550HJ02_120HZ	(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == SAMSUNG_LTI550HJ02_120HZ)
		#pragma message "Default panel will not bootup unless INCLUDE_SAMSUNG_LTI550HJ02_120HZ is defined!"
	#endif
#endif
