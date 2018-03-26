/*
	$Workfile:   Ltm170w01.h  $
	$Revision:   1.14  $
	$Date:   Jun 22 2006 18:01:02  $
*/

#ifdef INCLUDE_SAMSUNG_1080P_LTI32AA02

#include "..\..\Panel\Wxga\Samsung\LTI320AA02\OverdriveLut.h"
#include "..\..\Panel\Wxga\Samsung\LTI320AA02\GammaLut.h"

char ROM PanelName_SAMSUNG_LTI32AA02[] = "Samsung_LTI320AA02";

ROM WORD sRGB2rgb_LTI32AA02[9] =
{
	0x1000, 0x0000, 0x0000,
	0x0000, 0x1000, 0x0000,
	0x0000, 0x0000, 0x1000
};

ROM gmt_PanelData Panel_SAMSUNG_LTI32AA02 =
{
	PanelName_SAMSUNG_LTI32AA02,			//;
	SAMSUNG_1080P_LTI32AA02,	// Panel ID (refer to enum defined in panel.h)	
	PANEL_WXGA,			//		 	gmd_PanelType PanelType;
	ONE,				//BYTE PixPerClock;
	8,					//BYTE  Depth;
	1366,				//WORD  Width;
	768,				//WORD  Height;
	66,					//BYTE  MaxVFreq;
	50,					//BYTE  MinVFreq;
	82000L,				//DWORD MaxPClk; 		 // KHz
	1460,				//WORD W_MinHTotal
	1600,				//W_TypHTotal
	1708,				//W_22HTotal		
	1360,				//W_33HTotal
	4,					//BYTE  MinHSyncWidth;
	12,					//BYTE  MinHSyncBackPorch;
	952,					//W_22VTotal				
	797,					//W_33VTotal	
	1200,				//WORD  MaxVtotal;
	773,					//WORD  MinVtotal;
	806,					//WORD TypVTotal;
	3,					//BYTE  MinVSyncWidth;	
	6,					//BYTE  MinVSyncBackPorch;	
	0x775522L,			//DWORD PadDrive;
#ifdef Philip_PD	//Addy Chiu 20090916
	0x030C,    //0xFA09,        // WORD  PowerUpTiming
	0x2C0A,   //0x41F,         // WORD  PowerDownTiming
#else
	0xFA0C,	//Bossen, for SIT board.	20090917//0xFA09,								// WORD  PowerUpTiming
	0x41F,								// WORD  PowerDownTiming
#endif
	0xC,					//WORD AccToPixelVal;
	0x1203,				//Bossen, Value from TPV.	20090624.		0x0,					//BYTE  SpreadSpectrumCtrl;		// Value for Spread_Spectrum_Control register
	0,					//BYTE  DClkDelay;		 		// DCLK display timing delay adjustment in ns unit.
	Wa_OverDriveLUT_LTI320AA02, //OverDriveLut_LTM170W01,
	sRGB2rgb_LTI32AA02,
	Sta_GammaLUT_LTI320AA02,
	1,					//WORD  Invert_DVS 				: 1;
	1,					//WORD  Invert_DHS 				: 1;
	1,					//WORD  Invert_DCLK 			: 1;
	0,					//WORD  Invert_DEN 				: 1;
	0,				//WORD  DportSwap				: 1;
	TRUE,				//WORD  LVDSInterface			: 1;
	SINGLE_BUS,			//WORD  W_LVDS_BusType 			: 1;	
	LVDS_EIGHT_BIT_MODE,//WORD  LvdsPanelMode			: 2; 
	0xff,				//WORD  Spread_Spect_En 		: 1; // 0xFF : Enable spectrum, 0x00 : Disable 
	1,				//WORD	LvdsPosNegSwap			: 1;
        #ifndef Philip_PD //Mark Liao 091103 
	FOUR_PT_SIX_MA,			//Bossen, Follow TPV FQ request.	20091028.	FIVE_PT_SIX_MA,		//WORD  W_LvdsCurrentCtrl		: 2;	
        #else
	THREE_PT_SIX_MA,    //FIVE_PT_SIX_MA,		//WORD  W_LvdsCurrentCtrl		: 2;			
        #endif		
	OUT_24_48_BIT,		//WORD  W_BitOutput				: 1;
	0x0063,				//WORD  W_LvdsClkData
};
extern ROM gmt_PanelData Panel_SAMSUNG_LTI32AA02;

#else
		#define Panel_SAMSUNG_LTI32AA02	(ROM gmt_PanelData *)PanelArray
	//
	// If panel file is not included in the project, but the panel is selected to be
	// the default panel during bootup, issue warning message
	//
	#if (DEFAULT_PANEL == SAMSUNG_1080P_LTI32AA02)
		#pragma message "Default panel will not bootup unless INCLUDE_SAMSUNG_1080P_LTI32AA02 is defined!"
	#endif
#endif
