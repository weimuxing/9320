/*
	$Workfile:   Panel.c  $
	$Revision: 1.104 $
	$Date: 2013/05/20 07:01:13 $
*/
#define __PANEL_C__
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
// MODULE:      panel.c
//
// USAGE:       This module contains functions for panel parameter programming.
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "system\all.h"
#include "StdLibInc\gm_Register.h"

#if(defined(BOARD_93XX_4K2K))
#include "..\..\Panel\4K2K\ZDT_4K2K_STD\ZDT_4K2K_STD.h"
#include "..\..\Panel\4K2K\ZDT_4K2K\ZDT_4K2K_QFHD.h"
#include "..\..\Panel\4K2K\ZDT_4K2K_CMI\ZDT_4K2K_CMI.h"
#include "..\..\Panel\4K2K\ZDT_4K2K_CMI_39\ZDT_4K2K_CMI_39.h"
#include "..\..\Panel\4K2K\ZDT_4K2K_CMI_65\ZDT_4K2K_CMI_65.h"
#include "..\..\Panel\4K2K\ZDT_4K2K_CMI_50\ZDT_4K2K_CMI_50.h"
#else
//#include "..\..\Panel\Xga\LG\LM150X06\LM150X06.h"
//#include "..\..\Panel\Xga\LG\LM150X06_VGA\LM150X06_VGA.h"
//#include "..\..\Panel\Wxga\LG\LC171W03\LC171W03.h"
//#include "..\..\Panel\Wxga\LG\LC171W03_VGA\LC171W03_VGA.h"
//#include "..\..\Panel\Wxga\LG\LM171W01\LM171W01.h"
//#include "..\..\Panel\Wxga\LG\LC230W01\LC230W01.h"
//#include "..\..\Panel\Wxga\LG\LC300w01\LC300w01.h"
//#include "..\..\Panel\Wxga\LG\LC320w01\LC320w01.h"
//#include "..\..\Panel\Wxga\Samsung\Ltm170w01\Ltm170w01.h"
//#include "..\..\Panel\Wxga\Sanyo\TM290WX\TM290WX.h"
//#include "..\..\Panel\Wxga\AUO\T315XW01\T315XW01.h"
//#include "..\..\Panel\Sxga\LG\LM190E01\LM190E01.h"
//#include "..\..\Panel\Sxga\Samsung\LTM170E4_L01\LTM170E4_L01.h"
//#include "..\..\Panel\Uxga\Samsung\LTM213U4_L01\LTM213U4_L01.h"
//#include "..\..\Panel\Wxga\LG\LC200WX1\LC200WX1.h"
//#include "..\..\Panel\WUxga\Samsung\LTM240W1_L01\LTM240W1_L01.h"
//#include "..\..\Panel\WUxga\LG\LM260WU2_SLA1\LM260WU2_SLA1.h"
//#include "..\..\Panel\WUxga\LG\LM240WU5\LM240WU5.h"
//#include "..\..\Panel\1080p\AUO\T370HW01\T370HW01.h"
//#include "..\..\Panel\1080p\AUO\T370HW02\T370HW02.h"
//#include "..\..\Panel\1080p\CMO\V370H1_L01\V370H1_L01.h"
//#include "..\..\Panel\UXGA\LG\LM201U03\LM201U03.h"
//#include "..\..\Panel\1080p\DVIOut\Customer_1080pDviOut\Customer_1080pDviOut.h"
//#include "..\..\Panel\1080p\LG\LC370WU1\LC370WU1.h"
//#include "..\..\Panel\1080p\IPS\IPS4PS_2608_AX094F002E\AX094F002E.h"
//#include "..\..\Panel\1080p\Samsung\LTA400HH_LH1\LTA400HH_LH1.h"
//#include "..\..\Panel\Wsxga\LG\LM220WE1_TLD1\LM220WE1_TLD1.h"
#include "..\..\Panel\WUxga\LG\LM240WU4_SLB1\LM240WU4_SLB1.h"
#include "..\..\Panel\WUxga\LG\LM240WU9_SLA1\LM240WU9_SLA1.h" //DELL2413
//#include "..\..\Panel\Wxga\CustomerWxga1.h"
//#include "..\..\Panel\Wxga\CustomerWxga2.h"
//#include "..\..\Panel\Wxga\Customer720P.h"
//#include "..\..\Panel\Wxga\Customer1366.h"
//#include "..\..\Panel\CRT\CRT_33K\CRT_33K.h"
//#include "..\..\Panel\CRT\CRT_38K\CRT_38K.h"
//#include "..\..\Panel\Interlaced\1080i\1080i.h"
//#include "..\..\Panel\Wxga\LG\LC370wx2\LC370WX2.h"
//#include "..\..\Panel\720p\DVIOut\Customer_720pDviOut\Customer_720pDviOut.h"
//#include "..\..\Panel\Interlaced\DVIOut\Customer_1080iDviOut\Customer_1080iDviOut.h"
//#include "..\..\Panel\480p\DVIOut\Customer_480pDviOut\Customer_480pDviOut.h"
#include "..\..\Panel\WQXGA\LG\LM300WQ5_SLA1\LM300WQ5_SLA1.h"
#include "..\..\Panel\1080P\LG\LM230WF8_TLA1\LM230WF8_TLA1.h"
//#include "..\..\Panel\1080P\CMO\M236H5_L02_60\M236H5_L02_60.h"
//#include "..\..\Panel\1080P\CMO\M236H5_L02_120\M236H5_L02_120.h"
//#include "..\..\Panel\1080P\LG\LM270WF3_120\LM270WF3_120.h"
//#include "..\..\Panel\WQXGA\ST\VEGA_WQXGA\VEGA_WQXGA.h"
//#include "..\..\Panel\1080P\ST\VEGA_1080P\VEGA_1080P.h"
#include "..\..\Panel\WQHD\LG\LM270WQ1\LM270WQ1.h"
//#include "..\..\Panel\WQHD\ST\VEGA_WQHD\VEGA_WQHD.h"
//#include "..\..\Panel\2XGA\SONY\S_2XGA_SG\S_2XGA_SG.h"
#include "..\..\Panel\2K2K\ST\VEGA_2K2K\VEGA_2K2K.h"
//#include "..\..\Panel\1080P\LG\LC470EUF_120\LC470EUF_120.h"
//#include "..\..\Panel\2K2K\CMO\CMO_V562D1_L03\CMO_V562D1_L03.h"
#include "..\..\Panel\WHD\LG\LM290WW1\LM290WW1.h"
//#include "..\..\Panel\2K2K\Sharp\LD_T120001A\LD_T120001A.h"
//#include "..\..\Panel\Wsxga\CMO\M220Z1_L09\M220Z1_L09.h"


#include "..\..\Panel\1080P\LG\LC320EUN_SDV1_60\LC320EUN.h"
//#include "..\..\Panel\Wxga\Samsung\LTI260AP01\LTI260AP01.h"
#include "..\..\Panel\Wxga\LG\LC320EXN\LC320EXN.h"
#include "..\..\Panel\1080P\Samsung\LTI460HM03\LTI460HM03.h"
//#include "..\..\Panel\Wxga\Samsung\LTI460AA05\LTI460AA05.h"
#include "..\..\Panel\Wsxga\CMO\M220Z1_L01\M220Z1_L01.h"
#include "..\..\Panel\WQXGA\LG\LM300WQ6_SLA1\LM300WQ6_SLA1.h"
//#include "..\..\Panel\1080P\LG\LD420WUB_4K2K\LD420WUB_4K2K.h"
//#include "..\..\Panel\1080P\CMO\M236H5_4K2K\M236H5_L02_4K2K.h"
//#include "..\..\Panel\1080P\LG\LD420WUB_8BIT\LD420WUB_8BIT.h"
//#include "..\..\Panel\1080P\LG\LD420WUB_10BIT\LD420WUB_10BIT.h"
//#include "..\..\Panel\1080P\LG\LD470WUB_8BIT\LD470WUB_8BIT.h"
//#include "..\..\Panel\1080P\LG\LD470WUB_10BIT\LD470WUB_10BIT.h"
#include "..\..\Panel\1080P\AUO\T550HVN01V9_8BIT\T550HVN01V9_8BIT.h"
#include "..\..\Panel\1080P\AUO\T650HVN02_10BIT\T650HVN02_10BIT.h"
//#include "..\..\Panel\1080P\SHARP\LD900D3LE1\LD900D3LE1.h"

//#include "..\..\Panel\2K2K\NLT\NLT_NL204153AM21\NLT_NL204153AM21.h"
#endif	//(defined(BOARD_93XX_4K2K))

#ifdef PANEL_120_OR_100_HZ
#ifndef PANEL_120_OR_100_HZ_COMPATIBLE
#error "120 or 100 Hz panel support enabled but no 120 or 100 Hz compatible panel defined."
#endif
#endif

//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************
#define DUAL_BUS_EN BIT0

#define TX_MAP_COUNT 8
#define LVDS_STD_MAP_COUNT 2
#define TTL_STD_MAP_COUNT 1

// Test 20121218
#if 0//defined(Athena_4K2K_Chip_R) || defined(Athena_4K2K_Chip_L)
	#ifdef Athena_4K2K_Chip_R
		WORD panelDeltaWidth = 32;
	#endif
	#ifdef Athena_4K2K_Chip_L
		WORD panelDeltaWidth = 0;
	#endif
#else
WORD panelDeltaWidth = 0;
#endif


//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************

//#pragma warn -sus
ROM gmt_PanelData * ROM PanelArray[] =
{
   NULL_PTR,	// Do not remove - place holder for default panel selected by PANEL definition
	//&Panel_1080P_LG_LD420WUB_4K2K,		   // 1
#if(defined(BOARD_93XX_4K2K))
	   &Panel_ZDT_4K2K_STD, 						// 6
	   &Panel_ZDT_4K2K_QFHD,						  // 5
	   &Panel_ZDT_4K2K_CMI, 						// 6
	   &Panel_ZDT_4K2K_CMI_39,						// 6
	   &Panel_ZDT_4K2K_CMI_65,						// 6
	   &Panel_ZDT_4K2K_CMI_50, 					 // 6
#else   
	&Panel_LG_WUXGA_LM240WU9_SLA1,		  // 2
	&Panel_LG_WQHD_LM270WQ1, 			   // 3
	&Panel_LG_WQXGA_LM300WQ5_SLA1,		   // 4
	&Panel_LG_WHD_LM290WW1,

   //&Panel_LG_XGA_LM150X06,
   //&Panel_LG_XGA_LM150X06_VGA,
  // &Panel_LG_WXGA_LM171W01,
   //&Panel_LG_WXGA_LC171W03,
   //&Panel_LG_WXGA_LC230W01,
   //&Panel_LG_WXGA_LC300_W01,
   //&Panel_LG_WXGA_LC320W01,
   //&Panel_SAMSUNG_WXGA_LTM170W01,
  // &Panel_SAMSUNG_SXGA_LTM170E4_L01,
  // &Panel_SAMSUNG_UXGA_LTM213U4_L01,
   //&Panel_LG_SXGA_LM190E01,
   //&Panel_CUSTOMER_WXGA_PANEL_1,
   //&Panel_CUSTOMER_WXGA_1366,
   //&Panel_CUSTOMER_WXGA_DLP,
   //&Panel_CUSTOMER_WXGA_720P,
   //&Panel_LG_WXGA_LC171W03_VGA,
   //&Panel_Sanyo_TM290WX_71N31,
   //&Panel_AUO_WXGA_T315XW01,
   //&Panel_LG_WXGA_LC200WX1,
   //&Panel_1080P_AUO_T370HW01,
   //&Panel_1080P_AUO_T370HW02,
  // &Panel_1080P_CMO_V370H1_L01,
   //&Panel_SAMSUNG_WUXGA_LTM240W1_L01,
   //&Panel_LG_WUXGA_LM260WU2_SLA1,
   //&Panel_LPL_WUXGA_LM240WU5,
   //&Panel_LG_UXGA_LM201U03,
   //&Panel_CUSTOMER_1080P_DVIOUT,
  // &Panel_1080P_LG_LC370WU1,
   //&Panel_1080P_IPS_AX094F002E,
   //&Panel_SAMSUNG_1080P_LTA400HH_LH1,
   //&Panel_LG_WSXGA_LM220WE1_TLD1,
   &Panel_LG_WUXGA_LM240WU4_SLB1,			// 4
   //&Panel_CRT_33K,
   //&Panel_CRT_38K,
   //&Panel_1080I,
   //&Panel_LG_WXGA_LC370WX2,
  // &Panel_CUSTOMER_720P_DVIOUT,
   //&Panel_CUSTOMER_1080I_DVIOUT,
   //&Panel_CUSTOMER_480P_DVIOUT,
   //&Panel_LG_1080P_LM230WF8_TLA1,
   //&Panel_CMO_1080P_M236H5_L02_60,
   //&Panel_CMO_1080P_M236H5_L02_120,
   //&Panel_LG_1080P_LM270WF3_120,
   //&Panel_ST_WQXGA_VEGA_WQXGA,
   //&Panel_ST_1080P_VEGA_1080P,

	
   //&Panel_ST_WQHD_VEGA_WQHD,
   //&Panel_SONY_2XGA_S_2XGA_SG,
   //&Panel_ST_2K2K_VEGA_2K2K,   
   //&Panel_LG_1080P_LC470EUF_120,
   //&Panel_2K2K_CMO_V562D1_L03,
   //&Panel_CMO_WSXGA_M220Z1_L09,
   
   //&Panel_2K2K_LD_T120001A,
   &Panel_1080P_LG_LC320EUN,

   //&Panel_SAMSUNG_WXGA_LTI260AP01,
   &Panel_LG_WXGA_LC320EXN,
   &Panel_1080P_SAMSUNG_LTI460HM03,
   //&Panel_SAMSUNG_LTI460AA05,

   &Panel_CMO_WSXGA_M220Z1_L01,
   &Panel_LG_WQXGA_LM300WQ6_SLA1,		  // 4
   //&Panel_CMO_1080P_M236H5_4K2K, 						// 6
	
   //&Panel_1080P_LG_LD420WUB_8BIT,
   //&Panel_1080P_LG_LD420WUB_10BIT,
	//&Panel_1080P_LG_LD470WUB_8BIT,
	//&Panel_1080P_LG_LD470WUB_10BIT,

   &Panel_1080P_AUO_T550HVN01V9_8BIT,
   &Panel_1080P_AUO_T650HVN02_10BIT,
//   &Panel_1080P_SHARP_LD900D3LE1,				// 6
//   &Panel_2K2K_NLT_NL204153AM21,				// 6
//   &Panel_SAMSUNG_WQHD_LTM270DL02 ,
#endif

   NULL_PTR,	// Do not remove - place holder for end of list
};
//#pragma warn +sus

#if 1//(NON_SCALING_OUTPUT==1)
gmt_PanelData  CurPanelData;
gmt_PanelData far * CurrentPanelData =(gmt_PanelData far *) &CurPanelData;
#else
gmt_PanelData  CurPanelData;
gmt_PanelData  ROM* CurrentPanelData;
#endif

//******************************************************************************
//  S T A T I C    V A R I A B L E S
//******************************************************************************
DWORD LVDSMapData[LVDS_STD_MAP_COUNT][TX_MAP_COUNT] =
{
   /*Standard 1 */ // LSB in CH_3
   {0xC824A2D6, 0xA6E69C61, 0xC725D865, 0xA8A6AAF0,
      0x5515925D, 0x4E28F610, 0x21D820D2,0x00014118},

   /* Standard 2 */ // MSB in CH_3
   {0x461C8254, 0x86649441, 0xC725565D, 0xA69E8A70,
    0xB69BAABD, 0x4E28F628, 0x21D820D2, 0x00014118}
};


/* Blue [11:0] mapped to PD[11:0] */
/* Green [11:0] mapped to PD[23:12] */
/* Red [11:0] mapped to PD[35:24] */

DWORD TTLMapData[TTL_STD_MAP_COUNT][TX_MAP_COUNT] =
{
   /* Case1 */
   {0x628E4966, 0xAAD04608, 0x5D89E8A6, 0xD24D4556,
      0xA6C00420, 0x1C824A65, 0x00004146,0x02C00000},
};

//******************************************************************************
//  S T A T I C    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

//******************************************************************************
//  C O D E
//******************************************************************************

//******************************************************************************
//
// FUNCTION       : void DisplaySetTiming (void)
//
// USAGE          : Initializes the output display registers with panel data from the
//                  selected panel header file
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : DH_HS_WIDTH			(W)
//                  DH_ACTIVE_START		(W)
//                  DH_DE_START			(W)
//                  DH_ACTIVE_WIDTH		(W)
//                  DH_DE_END			(W)
//                  DH_TOTAL			(W)
//                  DISPLAY_VSYNC		(W)
//                  DV_ACTIVE_START		(W)
//                  DV_DE_START			(W)
//                  DV_ACTIVE_LENGTH	(W)
//                  DV_DE_END			(W)
//                  DV_TOTAL			(W)
//                  PB_BKGND_WIDTH		(W)
//                  PB_BKGND_HEIGHT		(W)
//                  PANEL_PWR_UP		(W)
//                  PANEL_PWR_DN		(W)
//                  DDDS_CONTROL		(W)
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DisplaySetTiming (void)
{
   //
   //Panel Horizontal timing
   //

   //
   // Panel Spread Spectrum Ctrl
   //
   if (Panel_Spread_Spect_En == 0)							// Spectrum Disable
   {														// Yes
      gm_ClearRegBitsWord(DDDS1_ESM_CTRL, DDDS1_SPREAD_SP_EN);
   }
   else													// Spectrum enable
   {
		InitPanelSpectrum();
      gm_WriteRegWord(DDDS1_ESM_CTRL, PanelSpreadSpectrumCtrl);    // Louis ... To Do --> Current mapping for SP data.
      gm_SetRegBitsWord(DDDS1_ESM_CTRL, DDDS1_SPREAD_SP_EN);		//
   }
   //
   // Panel hsync width should be programmed in terms of 4*DCLK
   //

   gm_WriteRegWord(DH_HS_WIDTH, 		PanelMinHSyncWidth/4);
   gm_WriteRegWord(DH_ACTIVE_START,	PanelMinHSyncWidth + PanelMinHSyncBackPorch);
   gm_WriteRegWord(DH_DE_START, 		PanelMinHSyncWidth + PanelMinHSyncBackPorch);
   gm_WriteRegWord(DH_ACTIVE_WIDTH, 	PanelWidth);
   gm_WriteRegWord(DH_DE_END, 			PanelMinHSyncWidth + PanelMinHSyncBackPorch + PanelWidth);
   gm_WriteRegWord(DH_TOTAL, 			gmv_OUTPUT_HTOTAL);
   //
   //Panel Vertical timing
   //
   gm_WriteRegWord(DISPLAY_VSYNC, 		(PanelMinVSyncWidth << 8));
   gm_WriteRegWord(DV_ACTIVE_START,	PanelMinVSyncWidth + PanelMinVSyncBackPorch);
   gm_WriteRegWord(DV_DE_START, 		PanelMinVSyncWidth + PanelMinVSyncBackPorch);
   gm_WriteRegWord(DV_ACTIVE_LENGTH, 	PanelHeight);
   gm_WriteRegWord(DV_DE_END, 			PanelMinVSyncWidth + PanelMinVSyncBackPorch + PanelHeight);
   gm_WriteRegWord(DV_TOTAL, 			gmv_OUTPUT_VTOTAL);
   //
   // Program the blender background width and height
   //

   gm_WriteRegWord(PB_BKGND_WIDTH, PanelWidth);
   gm_WriteRegWord(PB_BKGND_HEIGHT,PanelHeight);



   if (Panel_Invert_DVS)
      gm_SetRegBitsWord(DISPLAY_CONTROL, DVS_INV);
   if (Panel_Invert_DHS)
      gm_SetRegBitsWord(DISPLAY_CONTROL, DHS_INV);
   if (Panel_Invert_DEN)
      gm_SetRegBitsWord(DISPLAY_CONTROL, DEN_INV);
   if (Panel_Invert_DCLK)
      gm_SetRegBitsWord (TIMING_CONFIG, DCLK_INV);


   if ((PanelPixPerClk >= TWO) && (LvdsInterface != TRUE))
      gm_SetRegBitsWord(DISPLAY_CONTROL, D2PIXWIDE);
   
   if ((PanelBitOutput == OUT_30_BIT) && (LvdsInterface == FALSE)) // This is for TTL output 30-bit ...
      gm_SetRegBitsWord (DISPLAY_CONTROL, ENABLE_30BIT_OUTPUT);
   
   if ((PanelType == PANEL_480I) || (PanelType == PANEL_576I) || (PanelType == PANEL_1080I))
   {
      gm_SetRegBitsWord (DISPLAY_CONTROL, INTERLACED_TIMING);

      gm_SetRegBitsWord (INTERLACE_CTRL, INTERLACE_DATA_EN);

   }
   
   //
   //Panel power up and power down timing
   // 20121106
   #ifdef ST_4K2K_93xx_BOARD
		// GPIO init
		//gm_SetRegBitsWord(GPIO_1_ENABLE, (GPIO25_EN| GPIO29_EN)); // Panel power and backlight
		//gm_SetRegBitsWord(GPIO_1_DIRCTRL, (GPIO25_EN| GPIO29_EN));	
	#else
	   gm_WriteRegWord(PANEL_PWR_UP, PanelPwrUpTiming);
	   gm_WriteRegWord(PANEL_PWR_DN, PanelPwrDnTiming);

	   if ((PanelPwrUpTiming != 0xFFFF) && (PanelPwrDnTiming != 0xFFFF))
	   {
	      gm_WriteRegWord(PANEL_PWR_SEQ_TCLK_TICK, 0x3); // 4x power sequence
	   }
	#endif

	#ifdef ST_4K2K_93xx_BOARD
		// 20130306
		ImageSync_RL();
	#endif //ST_4K2K_93xx_BOARD
}

void ConfigureLVDSClock(void)
{
   ST_ErrorCode_t	ErrorCode = ST_NO_ERROR;
   DWORD LvdsClockConfig = 0;
   DWORD LvdsDllCtrlFreq = 0;


   switch (LvdsBusType)
   {
      case SINGLE_BUS:
         if (PanelMinVFreq>= 49 && PanelMaxVFreq <= 61)
         {
            if (PanelWidth==1920 && PanelHeight==1080)
            {
               LvdsClockConfig = 0xC;
               LvdsDllCtrlFreq = 0xE8;
            }
            else if ((PanelWidth==1366 && PanelHeight==768) ||\
                     (PanelWidth==1280 && PanelHeight==768) ||\
                     (PanelWidth==1280 && PanelHeight==720) ||\
                     (PanelWidth==1024 && PanelHeight==768))
            {
               LvdsClockConfig = 0x1D;
               LvdsDllCtrlFreq = 0x20E4;
            }
            else if ((PanelWidth==1400 && PanelHeight==900) ||\
                     (PanelWidth==1280 && PanelHeight==1024))
            {
               LvdsClockConfig = 0x1C;
               LvdsDllCtrlFreq = 0xE8;
            }
            else
            {
               gm_Print("HAL_ConfigureLVDSClock: Not Supported Panel Size \n", 0);
               ErrorCode = ST_ERROR_BAD_PARAMETER;
            }
         }
         else if (PanelMinVFreq>= 99 && PanelMaxVFreq <= 121)
         {
            if ((PanelWidth==1366 && PanelHeight==768) ||\
                  (PanelWidth==1280 && PanelHeight==720)||\
                  (PanelWidth==1280 && PanelHeight==768))
            {
               LvdsClockConfig = 0x1C;
               LvdsDllCtrlFreq = 0xE8;
            }
            else
            {
               gm_Print("HAL_ConfigureLVDSClock: Not Supported Panel Size \n", 0);
               ErrorCode = ST_ERROR_BAD_PARAMETER;
            }
         }
         else if(PanelMinVFreq>= 49 && PanelMaxVFreq <= 77)
         {
		if ((PanelWidth==1366 && PanelHeight==768) ||\
		(PanelWidth==1280 && PanelHeight==768) ||\
		(PanelWidth==1280 && PanelHeight==720) ||\
		(PanelWidth==1024 && PanelHeight==768))
		{
			LvdsClockConfig = 0x1D;
			LvdsDllCtrlFreq = 0x20E4;
		}
		else if ((PanelWidth==1400 && PanelHeight==900) ||\
		 (PanelWidth==1280 && PanelHeight==1024))
		{
			LvdsClockConfig = 0x1C;
			LvdsDllCtrlFreq = 0xE8;
		}
		else
		{
			gm_Print("HAL_ConfigureLVDSClock: Not Supported Panel Size \n", 0);
			ErrorCode = ST_ERROR_BAD_PARAMETER;
		}		
         }
	else
	{
            gm_Print("HAL_ConfigureLVDSClock: Not Supported FrameRate \n", 0);
            ErrorCode = ST_ERROR_BAD_PARAMETER;
         }		
         break;

#ifdef Athena_C   // 20110720, Athena-C only Dual LVDS bus
      case QUAD_BUS:
#endif
      case DUAL_BUS:
         LvdsClockConfig = 0x1C;
         LvdsDllCtrlFreq = 0xE8;
         break;

#ifdef Athena_U   // 20110720, Athena-C only Dual LVDS bus
      case QUAD_BUS:
         LvdsClockConfig = 0x14;
         LvdsDllCtrlFreq = 0x78;  //0xE8;
         break;
#endif

      default:
         ErrorCode = ST_ERROR_BAD_PARAMETER;
         break;
   }

   if (ErrorCode == ST_NO_ERROR)
   {
   	  gm_WriteRegDWord_LE(LVDS_DLL_CTRL_FREQ, LvdsDllCtrlFreq);
      gm_WriteRegDWord_LE(LVDS_CLOCK_CONFIG, LvdsClockConfig);
      
   }
   else
   {
   	  gm_WriteRegDWord_LE(LVDS_DLL_CTRL_FREQ, 0xE8);
      gm_WriteRegDWord_LE(LVDS_CLOCK_CONFIG, 0x14);

      gm_Print("ConfigureLVDSClock Error 0x%x", ErrorCode);
   }

	// 20130306
	#ifdef ST_4K2K_93xx_BOARD
	if (PanelID == LD_T120001A)
	{
		if(GetChipID_4K2K()== CHIPID_4K2K_L)
			gm_WriteRegWord(LVDS_DLL_CTRL_FREQ, 0xe8);
	}
	#endif

   // For CUT2.1
   if(IsCUT2_1())
   {
   gm_ClearRegBitsWord(LVDS_RESET_CTRL, LVDSTX_SOFT_RESET);
   gm_SetRegBitsWord(LVDS_RESET_CTRL, LVDSTX_SOFT_RESET);
   gm_ClearRegBitsWord(LVDS_RESET_CTRL, LVDSTX_SOFT_RESET);   
   }   
}

//******************************************************************************
//
// FUNCTION       : void ProgramPanelOutput (void)
//
// USAGE          : Programs the LVDS block
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : LVDS_POWER				(W)
//                  LVDS_DIGITAL_CTRL		(W)
//                  LVDS_PLL_CTRL			(W)
//                  LVDS_MISC_CTRL			(W)
//                  LVDS_P2S_CTRL			(W)
//                  LVDS_TEST_CTRL			(W)
//                  LVDS_TEST_DATA			(W)
//                  LVDS_CLK_DATA			(W)
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void ProgramPanelOutput (void)
{

   // Make sure Test bus is disabled
   if(IsCUT2_1())
   {
	#if ((DEFAULT_PANEL == LG_WQXGA_LM300WQ5_SLA1)||(DEFAULT_PANEL == LG_WQXGA_LM300WQ6_SLA1))	//U3014 for Panel Crash (Vertical Line) test
	      	gm_WriteRegDWord_LE(LVDS_TEST_CTRL, 0x30000);
	#else
      gm_WriteRegDWord_LE(LVDS_TEST_CTRL, 0x20000);
	#endif
   }
   else
   {
      gm_WriteRegDWord_LE(LVDS_TEST_CTRL, 0);
   }
   
   /* Bias Configuration */
   gm_WriteRegByte(LVDS_BIAS_CTRL, BIAS_EN
        | (LvdsCurrentCtrl << ICTRL_SHIFT)
        | (PanelPadDrive << RES_BIAS_SHIFT));

   /* LVDS Clock Configuration */
   ConfigureLVDSClock();


#if 1 //(PANEL_OUTPUT_CONTROL != PANEL_DPTX)
   {
      BYTE cnt;

      // LVDS
      if (LvdsInterface == TRUE)
      {
         /* LVDS mapping */
         if (PanelDepth > 8) // 10-bit and up ...
         {
			if(PanelID == SHARP_1080P_LD900D3LE1)
			{
				//8Bits Custom, need change to 10Bits
				#ifdef UART_DEBUG_MODE
				gm_Print("panel******SHARP_1080P_LD900D3LE1",0);
				#endif
				gm_WriteRegDWord_LE(LVDS_TX_MAP0,  0xC824A2D6);
				gm_WriteRegDWord_LE(LVDS_TX_MAP1,  0xA6E69C61);
				gm_WriteRegDWord_LE(LVDS_TX_MAP2,  0xC725D865);
				gm_WriteRegDWord_LE(LVDS_TX_MAP3,  0xA8A6AAF0);
				gm_WriteRegDWord_LE(LVDS_TX_MAP4,  0x5515925D);
				gm_WriteRegDWord_LE(LVDS_TX_MAP5,  0x4E28F610);
				gm_WriteRegDWord_LE(LVDS_TX_MAP6,  0x21D820D2);
				gm_WriteRegDWord_LE(LVDS_TX_MAP7,  0x00014118);
			}
			else
			{
	            gm_WriteRegDWord_LE(LVDS_TX_MAP0,  0xC41461D2);
	            gm_WriteRegDWord_LE(LVDS_TX_MAP1,  0x65E28C20);
	            gm_WriteRegDWord_LE(LVDS_TX_MAP2,  0xC724D455);
	            gm_WriteRegDWord_LE(LVDS_TX_MAP3,  0xA49669F0);
	            gm_WriteRegDWord_LE(LVDS_TX_MAP4,  0x9619A29D);
	            gm_WriteRegDWord_LE(LVDS_TX_MAP5,  0x6EAAF620);
	            gm_WriteRegDWord_LE(LVDS_TX_MAP6,  0xB6D8A2DA);
	            gm_WriteRegDWord_LE(LVDS_TX_MAP7,  0x0DB6DB6D);
			}
         }
         else if (LVDSSignalMap == STPANEL_LVDS_MAP_STANDARD1 ||
                  LVDSSignalMap == STPANEL_LVDS_MAP_STANDARD2) // 8-bit standard
         {
            for (cnt =0; cnt < TX_MAP_COUNT; ++cnt)
            {
               gm_WriteRegDWord_LE((LVDS_TX_MAP0 + cnt*4), LVDSMapData[LVDSSignalMap][cnt]);
            }
         }
         else // if(LVDSSignalMap == STPANEL_LVDS_MAP_CUSTOM) // 8-bit custom
         {
            if(PanelID == CMO_1080P_M236H5_L02_60)
            {
               // LVDS map standard 2 but remove H/V sync as spec request
               // Found 100Hz will make panel unstable
               gm_WriteRegDWord_LE(LVDS_TX_MAP0,  0x461C8254);
               gm_WriteRegDWord_LE(LVDS_TX_MAP1,  0x86649441);
               gm_WriteRegDWord_LE(LVDS_TX_MAP2,  0xDB25565D);
               gm_WriteRegDWord_LE(LVDS_TX_MAP3,  0xA69E8A76);
               gm_WriteRegDWord_LE(LVDS_TX_MAP4,  0xB69BAABD);
               #if 1 // set low bit to logic_0
               gm_WriteRegDWord_LE(LVDS_TX_MAP5,  0xDB6DB628);
               gm_WriteRegDWord_LE(LVDS_TX_MAP6,  0xB6DB6DB6);
               gm_WriteRegDWord_LE(LVDS_TX_MAP7,  0x0DB6DB6D);
               #else
               gm_WriteRegDWord_LE(LVDS_TX_MAP5,  0x4E28F628);
               gm_WriteRegDWord_LE(LVDS_TX_MAP6,  0x21D820D2);
               gm_WriteRegDWord_LE(LVDS_TX_MAP7,  0x00014118);
               #endif
            }
            else if(PanelID== LD_T120001A)
            {
               // LVDS map standard 2 but remove H/V sync as spec request
               // Found 100Hz will make panel unstable
#ifdef VEGA_TX_OUTPUT_DEBUG
               gm_Print("panel******VEGA-TX for debug",0);
               gm_WriteRegDWord_LE(LVDS_TX_MAP0,  0x461C8254);
               gm_WriteRegDWord_LE(LVDS_TX_MAP1,  0x86649441);
               gm_WriteRegDWord_LE(LVDS_TX_MAP2,  0xC725565D);
               gm_WriteRegDWord_LE(LVDS_TX_MAP3,  0xA69E8A70);
               gm_WriteRegDWord_LE(LVDS_TX_MAP4,  0xB69BAABD);
               gm_WriteRegDWord_LE(LVDS_TX_MAP5,  0x4E28F628);
               gm_WriteRegDWord_LE(LVDS_TX_MAP6,  0x21D820D2);
               gm_WriteRegDWord_LE(LVDS_TX_MAP7,  0x00014118);
#else
               gm_Print("panel******LD_T120001A",0);
               gm_WriteRegDWord_LE(LVDS_TX_MAP0,  0xC824A2D6);
               gm_WriteRegDWord_LE(LVDS_TX_MAP1,  0xA6E69C61);
               gm_WriteRegDWord_LE(LVDS_TX_MAP2,  0xC725D865);
               gm_WriteRegDWord_LE(LVDS_TX_MAP3,  0xA8A6AAF0);
               gm_WriteRegDWord_LE(LVDS_TX_MAP4,  0x5515925D);
               gm_WriteRegDWord_LE(LVDS_TX_MAP5,  0x4E28F610);
               gm_WriteRegDWord_LE(LVDS_TX_MAP6,  0x21D820D2);
               gm_WriteRegDWord_LE(LVDS_TX_MAP7,  0x00014118);
#endif               
            }
         }

         #if 0 // Enable Pad CTRL for GPIO change LVDS issue.
         if(IsCUT2_1())
            gm_ClearRegBitsWord(LVDS_DIGITAL_CTRL, PWR_SEQ_SEL);
         else
         #endif
         {
            // Workaround for LVDS panel power sequence
         	gm_SetRegBitsWord(LVDS_DIGITAL_CTRL, PWR_SEQ_SEL);
         }

         #ifdef Athena_C
         gm_ClearAndSetBitsDWord(LVDS_DIGITAL_CTRL, WIDTH_SEL, DUAL_BUS);
         #else
         gm_ClearAndSetBitsDWord(LVDS_DIGITAL_CTRL, WIDTH_SEL, LvdsBusType);
         #endif

         if (DportSwap != 0x00)
         {
            if (PanelID == CMO_1080P_M236H5_L02_60)
            {
            if (PanelPixPerClk == FOUR)
               gm_SetRegBitsDWord(LVDS_DIGITAL_CTRL, AB_SWAP_CD);
            else
               gm_SetRegBitsDWord(LVDS_DIGITAL_CTRL, A_B_SWAP);
            }
            else
            {
               gm_SetRegBitsDWord(LVDS_DIGITAL_CTRL, AB_SWAP_CD);
            }
         }


	if(Panel_Invert_DEN)
	{
		if(PanelID == LG_XGA_150X06)
			gm_SetRegBitsDWord(LVDS_DIGITAL_CTRL, LVDSTX_DEN_INV);
	}
         // Power on default is no swap.
         if (LvdsPosNegSwap == SWAP)
         {
            gm_SetRegBitsDWord(LVDS_PN_SWAP, A_POS_NEG_SWAP|B_POS_NEG_SWAP|C_POS_NEG_SWAP|D_POS_NEG_SWAP);
         }

         /*  Duty cycle Clock Configuration */
         gm_ClearAndSetBitsDWord(LVDS_DIGITAL_CTRL, CLK_DATA, (DWORD)LvdsClkData << 16);	// Panel dependent parameter defined in panel header file
         gm_WriteRegDWord_LE(LVDS_PLL_BIST_CTRL, 0);
         gm_WriteRegDWord_LE(LVDS_PLL_BIST_CNT_SETTINGS, 0);
         gm_WriteRegDWord_LE(LVDS_RX_DATA_SET, 0);
      }
#ifdef Athena_U // Athena-C no TTL bus
      else // TTL
      {
         if (LVDSSignalMap == STPANEL_LVDS_MAP_STANDARD1)
         {
            for (cnt =0; cnt < TX_MAP_COUNT; ++cnt)
            {
               gm_WriteRegDWord_LE((LVDS_TX_MAP0 + cnt*4), TTLMapData[LVDSSignalMap][cnt]);
            }
         }
         else //if( LVDSSignalMap == STPANEL_LVDS_MAP_CUSTOM)
         {
            gm_Print("Custom TTL", 0);
         }
      }
#endif

   }
#endif

   #if (PANEL_DATA_LEFT_RIGHT == ENABLE)
   gm_WriteRegWord(ODP_PANEL_LINBUF_CTRL, 0xCA);
   #endif

   #if 0//def ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()!= CHIPID_4K2K_FE)
      gm_WriteRegWord(ODP_PANEL_LINBUF_CTRL, 0xCA);

	if (PanelID == LD_T120001A)
	   #ifdef VEGA_TX_OUTPUT_DEBUG
   	gm_WriteRegWord(ODP_PANEL_LINBUF_CTRL, 0x0);
   	#else
      gm_WriteRegWord(ODP_PANEL_LINBUF_CTRL, 0x8A);
   	#endif
	else
		gm_WriteRegWord(ODP_PANEL_LINBUF_CTRL, 0x00); // FE
   #endif

   PanelPwrOn();
}

gmd_PanelDataFormat GetPanelDataFormat(void)
{
   return PanelDataFormat;
}

BYTE GetPanelMinHSyncWidth(void)
{
   return PanelMinHSyncWidth;
}

BYTE GetPanelMinHSyncBackPorch(void)
{
   return PanelMinHSyncBackPorch;
}

BYTE GetPanelMinVSyncWidth(void)
{
   return PanelMinVSyncWidth;
}

BYTE GetPanelMinVSyncBackPorch(void)
{
   return PanelMinVSyncBackPorch;
}

WORD GetPanelWidth(void)
{
   return PanelWidth;
}

WORD GetPanelHeight(void)
{
   return PanelHeight;
}

WORD GetPanelTypVTotal(void)
{
   return PanelTypVTotal;
}

WORD GetPanelMaxVTotal(void)
{
   return PanelMaxVTotal;
}

WORD GetPanelTypHTotal(void)
{
   return PanelTypHTotal;
}

WORD GetPanelMinHTotal(void)
{
   return PanelMinHTotal;
}

void PanelPwrOn(void)
{
#if (FEATURE_DISPLAY_PORT == ENABLE) && (DP_NUM_SST_ONLY_TX == 1)
//&& (NON_SCALING_OUTPUT ==0)
	#ifdef OUTPUT_MODE_CHANGE
   if(UserPrefOutputMode == OPM_Normal)
	#endif   
   {
#if (EnableDpTxDelayCount == 1)
      BYTE ExtraDelayCount=ExtraDpTxEnableDelayCount;
#endif

#ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()==CHIPID_4K2K_FE)
#endif
   {

      if(DP_IsTxHpdHigh() == TRUE)
      {
         if(DP_IsTxActive(iDpTxPort)==TRUE) 
         {
            gm_TimerStart(DP_PANEL_PWR_ON_WAIT_100ms_TMR,20);
            while (gm_TimerCheck(DP_PANEL_PWR_ON_WAIT_100ms_TMR)!= TIMER_TMO)
            {
               DP_Handler();
#if (EnableDpTxDelayCount==1)
               if (ExtraDelayCount==0)
               {
                  Set_SstOnlyTx_EnVdoFlag(TRUE);
                  //gm_Print("DPTX_IsLinkReady",0);
                  break;
               }
#endif

               if (DPTX_IsLinkReady(iDpTxPort)==TRUE)
               {
#if (EnableDpTxDelayCount==1)
                  ExtraDelayCount--;
                  //gm_Print("ExD",0);
#else
                  Set_SstOnlyTx_EnVdoFlag(TRUE);
                  //gm_Print("DPTX_IsLinkReady",0);
                  break;
#endif
               }
            }
            DpTxEnableVideo(DPTX_TIMING_BY_OUTPUT);
         }
      }
    }
   }
#endif
//#if(NON_SCALING_OUTPUT == 1)
#if ((DPRX_PANEL == ENABLE)||(SELECT_DPTX_TYPE==USE_IDP_DPTX_OUTPUT))
	SetupDPPanel(); 	//Hyb130508+ for DP panel 
#endif
#ifdef OUTPUT_MODE_CHANGE
if(UserPrefOutputMode != OPM_Normal)
DpTxEnableVideo(DPTX_TIMING_BY_INPUT);
#endif
//#endif
#ifdef DPTX_ONLY
// Power On Default
#else
   if (LvdsInterface == TRUE)
   {  //LVDS
      #if 0 // Enable Pad CTRL for GPIO change LVDS issue.
      if(IsCUT2_1())
		{
      	// Enable LVDS Channel A
      	gm_SetRegBitsDWord(LVDS_BUS_AB_PAD_CTRL, BUS_A_CH0_ENABLE|BUS_A_CH1_ENABLE|BUS_A_CH2_ENABLE|BUS_A_CH3_ENABLE);      	
      	if(PanelDepth >= 8) // 8-bit // CH4_ENABLE is mapped to HW_CH_3
         	gm_SetRegBitsDWord(LVDS_BUS_AB_PAD_CTRL, BUS_A_CH4_ENABLE);
         if(PanelDepth == 10) // 10-bit // CH5_ENABLE is mapped to HW_CH_4 // CLK_ENABLE is mapped to HW_CH_5 which is not support by Athena
         	gm_SetRegBitsDWord(LVDS_BUS_AB_PAD_CTRL, BUS_A_CH5_ENABLE);

      	if ((PanelPixPerClk >= TWO) || (PanelDataFormat != FORMAT_ORG_2D))
      	{
         	// Enable LVDS Channel B
         	gm_SetRegBitsDWord(LVDS_BUS_AB_PAD_CTRL, (DWORD)BUS_B_CH0_ENABLE|BUS_B_CH1_ENABLE|BUS_B_CH2_ENABLE|BUS_B_CH3_ENABLE);
         	if (PanelDepth >= 8) // 8-bit // CH4_ENABLE is mapped to HW_CH_3
            	gm_SetRegBitsDWord(LVDS_BUS_AB_PAD_CTRL,BUS_B_CH4_ENABLE);            
         	if (PanelDepth == 10) // 10-bit // CH5_ENABLE is mapped to HW_CH_4 // CLK_ENABLE is mapped to HW_CH_5 which is not support by Athena
            	gm_SetRegBitsDWord(LVDS_BUS_AB_PAD_CTRL,BUS_B_CH5_ENABLE);
      	}

#if (DEFAULT_PANEL != CMO_1080P_M236H5_L02_60)
      	if ((PanelPixPerClk == FOUR) || (PanelDataFormat != FORMAT_ORG_2D))
#else
      	// CMO enable all channels
#endif
      	{
         	// Enable LVDS Channel C
         //20120905 Kordonm comment Bus C channel 0 for panel crash issue
         	gm_SetRegBitsDWord(LVDS_BUS_CD_PAD_CTRL, /*BUS_C_CH0_ENABLE|*/BUS_C_CH1_ENABLE|BUS_C_CH2_ENABLE|BUS_C_CH3_ENABLE);
         	if (PanelDepth >= 8) // 8-bit // CH4_ENABLE is mapped to HW_CH_3
            	gm_SetRegBitsDWord(LVDS_BUS_CD_PAD_CTRL, BUS_C_CH4_ENABLE);
         	if (PanelDepth == 10) // 10-bit // CH5_ENABLE is mapped to HW_CH_4 // CLK_ENABLE is mapped to HW_CH_5 which is not support by Athena
            	gm_SetRegBitsDWord(LVDS_BUS_CD_PAD_CTRL, BUS_C_CH5_ENABLE);

         	// Enable LVDS Channel D
        	 	gm_SetRegBitsDWord(LVDS_BUS_CD_PAD_CTRL, (DWORD)BUS_D_CH0_ENABLE|BUS_D_CH1_ENABLE|BUS_D_CH2_ENABLE|BUS_D_CH3_ENABLE);
         	if (PanelDepth >= 8) // 8-bit // CH4_ENABLE is mapped to HW_CH_3
            	gm_SetRegBitsDWord(LVDS_BUS_CD_PAD_CTRL, BUS_D_CH4_ENABLE);
         	if (PanelDepth == 10) // 10-bit // CH5_ENABLE is mapped to HW_CH_4 // CLK_ENABLE is mapped to HW_CH_5 which is not support by Athena
            	gm_SetRegBitsDWord(LVDS_BUS_CD_PAD_CTRL, BUS_D_CH5_ENABLE);
      	}
		}
      #endif
   }
   else
   {  //TTL
      gm_WriteRegDWord_LE(LVDS_BUS_AB_PAD_CTRL , 0x3FFF3FFF);
      gm_WriteRegDWord_LE(LVDS_BUS_CD_PAD_CTRL , 0x3FFF3FFF);   
   }
#endif     
}



void PanelPwrOff(void)
{
   //Todo: to be implemented.
}

#if 1//(NON_SCALING_OUTPUT==1)|| defined(ST_4K2K_93xx_BOARD)
void UpdateCurrentPanelData(gmt_PHY_CH B_Channel)
{
	#ifndef ST_4K2K_93xx_BOARD
	
	if(gm_IsInterlaced(gmvb_MainChannel))
	{
	   if(stInput[gmvb_MainChannel].Vtotal % 2)
	   {
		  stInput[gmvb_MainChannel].Vtotal -= 1;
	   } 
	}
	
   PanelWidth = stInput[B_Channel].Hactive;
   PanelHeight = stInput[B_Channel].Vactive;
   PanelMaxVFreq = (stInput[gmvb_MainChannel].Vfrq + 5) / 10;
   PanelMinVFreq = (stInput[gmvb_MainChannel].Vfrq + 5) / 10;
   PanelMaxPClk = ((DWORD)stInput[B_Channel].Htotal * stInput[B_Channel].Vtotal/10) * stInput[B_Channel].Vfrq/1000;
   PanelMinHTotal = stInput[B_Channel].Htotal;
   PanelTypHTotal = stInput[B_Channel].Htotal;
   Panel22HTotal = stInput[B_Channel].Htotal;
   Panel33HTotal = stInput[B_Channel].Htotal;
   
   {
      WORD W_Hperiod;
      WORD W_Hpulse;
      W_Hperiod =gm_ReadRegWord(MIFM_HS_PERIOD);
      W_Hpulse = gm_ReadRegWord(MIFM_HS_PULSE);

      if (W_Hpulse > (W_Hperiod / 2))
         PanelMinHSyncWidth   = W_Hperiod - W_Hpulse;
      else
         PanelMinHSyncWidth   = W_Hpulse;

      PanelMinHSyncWidth= (BYTE)((DWORD)(PanelTypHTotal*PanelMinHSyncWidth) / W_Hperiod);
      PanelMinHSyncWidth &= 0xF8;
      
      if (PanelMinHSyncWidth < 8)
         PanelMinHSyncWidth = 8;
   }
   PanelMinHSyncBackPorch = stInput[B_Channel].Hstart - PanelMinHSyncWidth;
   PanelMinHSyncBackPorch &= 0xFFF8;

   Panel22VTotal=stInput[B_Channel].Vtotal;
   Panel33VTotal=stInput[B_Channel].Vtotal;
   PanelMaxVTotal=stInput[B_Channel].Vtotal;
   PanelTypVTotal=stInput[B_Channel].Vtotal;
   {
      WORD W_Vperiod;
      WORD W_Vpulse;
      
      W_Vperiod = gm_ReadRegWord(MIFM_VS_PERIOD);
      W_Vpulse = gm_ReadRegWord(MIFM_VS_PULSE);
      
      if (W_Vpulse > (W_Vperiod / 2))
         PanelMinVSyncWidth = W_Vperiod - W_Vpulse;
      else
         PanelMinVSyncWidth = W_Vpulse;
   }

   PanelMinVSyncBackPorch = stInput[B_Channel].Vstart - PanelMinVSyncWidth;
   #else
   UNUSED_VAR(B_Channel); //Add by ChengLin 20130115 for avoid compile warning
   #endif


//#if (DEINTERLACED_OUTPUT == 1) && (PASS_THRU_MODE == 0)
#ifdef OUTPUT_MODE_CHANGE
	if(UserPrefOutputMode == OPM_NoScaling)
	{
		if (stInput[B_Channel].ModeFlags & gmd_MODE_INTERLACED)
		{
			PanelHeight = stInput[B_Channel].Vactive * 2;
			PanelMaxPClk = ((DWORD)stInput[B_Channel].Htotal * (stInput[B_Channel].Vtotal*2 + 1)/10) * stInput[B_Channel].Vfrq/1000;
			Panel22VTotal = stInput[B_Channel].Vtotal * 2 + 1;
			Panel33VTotal = stInput[B_Channel].Vtotal * 2 + 1;
			PanelMaxVTotal = stInput[B_Channel].Vtotal * 2 + 1;
			PanelTypVTotal = stInput[B_Channel].Vtotal * 2 + 1;
			gmv_OUTPUT_VTOTAL = PanelTypVTotal;
		}
	}
   else
   {
	   if(gm_IsInterlaced(gmvb_MainChannel))
	   {
	      PanelHeight = stInput[gmvb_MainChannel].Vactive * 2;
	      PanelMaxPClk = ((DWORD)stInput[gmvb_MainChannel].Htotal * (stInput[gmvb_MainChannel].Vtotal*2 + 1)/10) * stInput[gmvb_MainChannel].Vfrq/1000;
	      Panel22VTotal = stInput[gmvb_MainChannel].Vtotal * 2 + 1;
	      Panel33VTotal = stInput[gmvb_MainChannel].Vtotal * 2 + 1;
	      PanelMaxVTotal = stInput[gmvb_MainChannel].Vtotal * 2 + 1;
	      PanelTypVTotal = stInput[gmvb_MainChannel].Vtotal * 2 + 1;
	   }
   }
#endif

gmv_OUTPUT_HTOTAL = PanelTypHTotal;    // Typical
gmv_OUTPUT_VTOTAL = PanelTypVTotal;    // Typical
gmv_OUTPUT_HTOTAL_22 = Panel22HTotal;  // HTotal for 2:2 (60Hz to 48Hz conversion)
gmv_OUTPUT_VTOTAL_22 = Panel22VTotal;  // VTotal for 2:2 (60Hz to 48Hz conversion)
gmv_OUTPUT_HTOTAL_33 = Panel33HTotal;  // HTotal for 3:3 (60Hz to 72Hz conversion)
gmv_OUTPUT_VTOTAL_33 = Panel33VTotal;  // VTotal for 3:3 (60Hz to 72Hz conversion)
gmv_OUTPUT_HACTIVE = PanelWidth;
gmv_OUTPUT_VACTIVE = PanelHeight;


//#endif
}

#endif

void ClearOdpTiming(void)
{
   gm_WriteRegWord(DH_HS_WIDTH, 		0);
   gm_WriteRegWord(DH_ACTIVE_START,	0);
   gm_WriteRegWord(DH_DE_START, 		0);
   gm_WriteRegWord(DH_ACTIVE_WIDTH, 0);
   gm_WriteRegWord(DH_DE_END, 		0);
   gm_WriteRegWord(DH_TOTAL, 			0);
   //
   //Panel Vertical timing
   //
   gm_WriteRegWord(DISPLAY_VSYNC, 	0);
   gm_WriteRegWord(DV_ACTIVE_START,	0);
   gm_WriteRegWord(DV_DE_START, 		0);
   gm_WriteRegWord(DV_ACTIVE_LENGTH,0);
   gm_WriteRegWord(DV_DE_END, 		0);
   gm_WriteRegWord(DV_TOTAL, 			0);
}

#ifdef ST_4K2K_93xx_BOARD
static BYTE  PanelControlStatus=0;
#define PANEL_PWR_ON_DELAY_TIME_10ms      22    // 240 ms
#define PANEL_BKL_ON_DELAY_TIME_10ms      50    // 500 ms
#define PANEL_PWR_OFF_DELAY_TIME_10ms     2    // 20 ms
#define PANEL_BKL_OFF_DELAY_TIME_10ms     1    // 10 ms
#define PANEL_PWR_OFF_TIMEOUT_WAIT_10ms   ((PANEL_PWR_OFF_DELAY_TIME_10ms + PANEL_BKL_OFF_DELAY_TIME_10ms)*5)

static void TurnOnPanelPWR_4Kx2K(void)
{
   if(gm_ReadRegWord(GPOUTPUT1) & GPIO29_OUT)
      return;

   gm_SetRegBitsWord(GPOUTPUT1, GPIO29_OUT);
   ppc_msg("PPC> PWR_ON_T=%Ld",gm_GetSystemTime());
}

static void TurnOffPanelPWR_4Kx2K(void)
{
	if(!(gm_ReadRegWord(GPOUTPUT1) & GPIO29_OUT))
			return;
	
	gm_ClearRegBitsWord(GPOUTPUT1, GPIO29_OUT);
   ppc_msg("PPC> PWR_OFF_T=%Ld",gm_GetSystemTime());	
}

static void TurnOnBcaklightPWR_4Kx2K(void)
{
	if(gm_ReadRegWord(GPOUTPUT1) & GPIO25_OUT)
			return;
			
	gm_SetRegBitsWord(GPOUTPUT1, GPIO25_OUT);
	ppc_msg("PPC> BKL_ON_T=%Ld",gm_GetSystemTime());
}

static void TurnOffBcaklightPWR_4Kx2K(void)
{
	if(!(gm_ReadRegWord(GPOUTPUT1) & GPIO25_OUT))
			return;
	
	gm_ClearRegBitsWord(GPOUTPUT1, GPIO25_OUT);	
	ppc_msg("PPC> BKL_OFF_T=%Ld",gm_GetSystemTime());
}


static BOOL Is_LVDS_SYNC_Ready(void)
{
   if(GetChipID_4K2K()==CHIPID_4K2K_R)
   {
      if(gm_ReadRegWord(GPINPUT5) & GPIO82_IN)	
         return TRUE;
   }
   else if(GetChipID_4K2K()==CHIPID_4K2K_L)
   {
      if(gm_ReadRegWord(GPINPUT5) & GPIO84_IN)		
         return TRUE;
   }
   return FALSE;
}

static void Set_LVDS_SYNC_ReadyPin(void)
{
   if(GetChipID_4K2K()==CHIPID_4K2K_R)
   {
      gm_SetRegBitsWord(GPOUTPUT5,GPIO82_OUT);
   }
   else if(GetChipID_4K2K()==CHIPID_4K2K_L)
   {
      gm_SetRegBitsWord(GPOUTPUT5,GPIO84_OUT);
   }
}

static BOOL Is_Set_LVDS_SYNC_ReadyPin_OK(void)
{
   if(GetChipID_4K2K()==CHIPID_4K2K_R)
   {
      if(gm_ReadRegWord(GPOUTPUT5) & GPIO82_OUT)	
         return TRUE;
   }
   else if(GetChipID_4K2K()==CHIPID_4K2K_L)
   {
      if(gm_ReadRegWord(GPOUTPUT5) & GPIO84_OUT)		
         return TRUE;
   }
   return FALSE;
}

void SetPanelControlStatus(BYTE b_Status)
{
   PanelControlStatus=b_Status;
}

void PanelPowerOn4k2k(void)
{
   ppc_msg("PPC> ON_REQ_T=%Ld",gm_GetSystemTime());
   TurnOnPanelPWR_4Kx2K();
   gm_TimerStart(Panel_PWR_10ms_TMR, PANEL_PWR_ON_DELAY_TIME_10ms);      

   Image_ReadyFlag(FALSE); 
   INTERRUPT_DISABLE(DISPLAY_DEC_IRQ5MASK, IMP_AFR_DETECT_MASK);
   ImageSyncHandleSetState(IMAGE_LVDS_ON_STATE); //Set Athena Right to "IMAGE OFF" sate
   SOCIPC_SetAthena_L_ImageSyncState(IMAGE_LVDS_ON_STATE); //Set Athena Left GPIO84 to Low and Image sync state to "IMAGE OFF"    

   
   PanelControlStatus|= PANEL_CTRL_LVDS_ON_REQUEST;   
}


void PanelPowerOff4k2k(void)
{
   PanelControlStatus|= PANEL_CTRL_LVDS_OFF_REQUEST;
   ppc_msg("PPC> OFF_REQ_T=%Ld",gm_GetSystemTime());

   TurnOffBcaklightPWR_4Kx2K();
   gm_TimerStart(Panel_BKL_10ms_TMR,PANEL_BKL_OFF_DELAY_TIME_10ms);

   Image_ReadyFlag(FALSE); 
   INTERRUPT_DISABLE(DISPLAY_DEC_IRQ5MASK, IMP_AFR_DETECT_MASK);
   ImageSyncHandleSetState(IMAGE_LVDS_OFF_STATE); //Set Athena Right to "IMAGE OFF" sate
   SOCIPC_SetAthena_L_ImageSyncState(IMAGE_LVDS_OFF_STATE); //Set Athena Left GPIO84 to Low and Image sync state to "IMAGE OFF"    

   gm_TimerStart(Panel_PWR_OFFWAIT_10ms_TMR, PANEL_PWR_OFF_TIMEOUT_WAIT_10ms);

   while(gm_TimerCheck(Panel_PWR_OFFWAIT_10ms_TMR)==TIMER_OK) //Timer Running
   {
      PanelControl_MasterHandler();
   }
}


void PanelControl_MasterHandler(void)
{
   if(PanelControlStatus & PANEL_CTRL_LVDS_ON_REQUEST)
   {
      if(Is_Set_LVDS_SYNC_ReadyPin_OK()==FALSE)
      {  // Ready Pin is not set
         if(gm_TimerCheck(Panel_PWR_10ms_TMR) != TIMER_OK)
         {
            Set_LVDS_SYNC_ReadyPin();     // Set Ready to Sync LVDS_ON control
            ppc_msg("PPC> R_LVDS_RDY_T=%Ld",gm_GetSystemTime());
         }
      }

   	if(Is_LVDS_SYNC_Ready()==TRUE)
   	{  
   	   // Turn On LVDS signal
   		gm_DisplayOnOff_4Kx2K(ON);
   		ppc_msg("PPC> R_LVDS_ON_T=%Ld",gm_GetSystemTime());

   		// Backlight On delay Start
   		gm_TimerStart(Panel_BKL_10ms_TMR, PANEL_BKL_ON_DELAY_TIME_10ms);
         PanelControlStatus |= PANEL_CTRL_BKL_ON_REQUEST;

   		#if ENABLE_ATHENA_R_L_IMAGE_SYNC_HANDLER
   		// Request to change IMG_RDY pin from IMAGE_LVDS_ON_STATE to IMAGE_OFF_STATE
         gm_TimerStart(Panel_PWR_10ms_TMR,5);
         PanelControlStatus |=PANEL_CTRL_REQ_2_IMAGE_SYNC_STATE;
   		#endif

   		PanelControlStatus &= (~PANEL_CTRL_LVDS_ON_REQUEST);
   	}
	}

   if(PanelControlStatus & PANEL_CTRL_BKL_ON_REQUEST)
   {
   	if(gm_TimerCheck(Panel_BKL_10ms_TMR) != TIMER_OK)
   	{  // Turn on Backlight 
         TurnOnBcaklightPWR_4Kx2K();
         PanelControlStatus &= (~PANEL_CTRL_BKL_ON_REQUEST);
   	}
   }

	if(PanelControlStatus & PANEL_CTRL_REQ_2_IMAGE_SYNC_STATE)
   {
      if(gm_TimerCheck(Panel_PWR_10ms_TMR) != TIMER_OK)  
      {
         #if ENABLE_ATHENA_R_L_IMAGE_SYNC_HANDLER     
         Image_ReadyFlag(FALSE);                   //Set Athena Right GPIO82 to Low    
         ImageSyncHandleSetState(IMAGE_OFF_STATE); //Set Athena Right to "IMAGE OFF" sate
         SOCIPC_SetAthena_L_ImageSyncState(IMAGE_OFF_STATE); //Set Athena Left GPIO84 to Low and Image sync state to "IMAGE OFF"       
         ImageSyncHandler_msg("Image Sync State for Athena Right and Left  = IMAGE_OFF_STATE.",0);    
         PanelControlStatus &= (~PANEL_CTRL_REQ_2_IMAGE_SYNC_STATE);
         #endif
      }
   }

   if(PanelControlStatus & PANEL_CTRL_LVDS_OFF_REQUEST)
   {
      if(Is_Set_LVDS_SYNC_ReadyPin_OK()==FALSE)
      {  // Ready Pin is not set
         if(gm_TimerCheck(Panel_BKL_10ms_TMR) != TIMER_OK)
         {
            Set_LVDS_SYNC_ReadyPin();     // Set Ready to Sync LVDS_ON control
            ppc_msg("PPC> R_LVDS_RDY_T=%Ld",gm_GetSystemTime());
         }
      }

   	if(Is_LVDS_SYNC_Ready()==TRUE)
   	{  
   	   // Turn Off LVDS signal
   		gm_DisplayOnOff_4Kx2K(OFF);
   		ppc_msg("PPC> R_LVDS_OFF_T=%Ld",gm_GetSystemTime());

   		// Power Off delay Start
   		gm_TimerStart(Panel_PWR_10ms_TMR, PANEL_PWR_OFF_DELAY_TIME_10ms);
         PanelControlStatus |= PANEL_CTRL_PWR_OFF_REQUEST;

   		PanelControlStatus &= (~PANEL_CTRL_LVDS_OFF_REQUEST);
   	}
   }

   if(PanelControlStatus & PANEL_CTRL_PWR_OFF_REQUEST)
   {
      if(gm_TimerCheck(Panel_PWR_10ms_TMR) != TIMER_OK)
      {  // Turn on Backlight 
         TurnOffPanelPWR_4Kx2K();
         gm_TimerStop(Panel_PWR_OFFWAIT_10ms_TMR);
         PanelControlStatus &= (~PANEL_CTRL_PWR_OFF_REQUEST);
      }
   }
}


void PanelControl_SlaveHandler(void)
{

   if(PanelControlStatus & PANEL_CTRL_LVDS_ON_REQUEST)
   {
      //if(gm_ReadRegDWord(LVDS_BUS_AB_PAD_CTRL) == 0)
      {
         if(Is_Set_LVDS_SYNC_ReadyPin_OK()==FALSE)
         {
            Set_LVDS_SYNC_ReadyPin();     // Set Ready to Sync LVDS_ON control
            ppc_msg("PPC> L_LVDS_RDY_T=%Ld",gm_GetSystemTime());
         }

         if(Is_LVDS_SYNC_Ready()==TRUE)
         {     
            gm_DisplayOnOff_4Kx2K(On);      
            ppc_msg("PPC> L_LVDS_ON_T=%Ld",gm_GetSystemTime());
            PanelControlStatus &= (~PANEL_CTRL_LVDS_ON_REQUEST);            
         }
      }
   }
   else if(PanelControlStatus & PANEL_CTRL_LVDS_OFF_REQUEST)
   {
      //if(gm_ReadRegDWord(LVDS_BUS_AB_PAD_CTRL) == 0)
      {
         if(Is_Set_LVDS_SYNC_ReadyPin_OK()==FALSE)
         {
            Set_LVDS_SYNC_ReadyPin();     // Set Ready to Sync LVDS_ON control
            ppc_msg("PPC> L_LVDS_RDY_T=%Ld",gm_GetSystemTime());
         }

         if(Is_LVDS_SYNC_Ready()==TRUE)
         {     
            gm_DisplayOnOff_4Kx2K(Off);      
            ppc_msg("PPC> L_LVDS_OFF_T=%Ld",gm_GetSystemTime());
            PanelControlStatus &= (~PANEL_CTRL_LVDS_OFF_REQUEST);            
         }
      }
   }
}

void gm_DisplayOnOff_4Kx2K(BOOL Bt_OnOff)
{

   //panlctrl_msg("gm_DisplayOnOff_4Kx2K(%d)",Bt_OnOff);
   // Control the display engine, display control and display data

	if (Bt_OnOff == Off)
   {
      gm_ClkSelect(CLK_ODP, ODP_SEL_DCLK);
		gm_ClearRegBitsWord(DISPLAY_CONTROL, DTG_RUN_EN | DCNTL_EN | DDATA_EN);

      {    
         gm_ClearRegBitsDWord(LVDS_BUS_AB_PAD_CTRL, BUS_A_CH0_ENABLE|BUS_A_CH1_ENABLE|BUS_A_CH2_ENABLE|BUS_A_CH3_ENABLE|BUS_A_CH4_ENABLE|BUS_A_CH5_ENABLE|BUS_A_CLK_ENABLE|
                                                  BUS_B_CH0_ENABLE|BUS_B_CH1_ENABLE|BUS_B_CH2_ENABLE|BUS_B_CH3_ENABLE|BUS_B_CH4_ENABLE|BUS_B_CH5_ENABLE|BUS_B_CLK_ENABLE );
                     
         gm_ClearRegBitsDWord(LVDS_BUS_CD_PAD_CTRL, BUS_C_CH0_ENABLE|BUS_C_CH1_ENABLE|BUS_C_CH2_ENABLE|BUS_C_CH3_ENABLE|BUS_C_CH4_ENABLE|BUS_C_CH5_ENABLE|BUS_C_CLK_ENABLE|
                                                  BUS_D_CH0_ENABLE|BUS_D_CH1_ENABLE|BUS_D_CH2_ENABLE|BUS_D_CH3_ENABLE|BUS_D_CH4_ENABLE|BUS_D_CH5_ENABLE|BUS_D_CLK_ENABLE );
      }  
      #if 0 //For Debug 
      gm_ClearRegBitsWord(GPOUTPUT1, GPIO18_OUT);
      //gm_SetRegBitsWord(GPOUTPUT1, GPIO18_OUT);
      #endif
   }
	else
   {
		gm_SetRegBitsWord(DISPLAY_CONTROL, DTG_RUN_EN | DCNTL_EN | DDATA_EN);

      {
         {
            gm_SetRegBitsDWord(LVDS_BUS_AB_PAD_CTRL, BUS_A_CH0_ENABLE|BUS_A_CH1_ENABLE|BUS_A_CH2_ENABLE|BUS_A_CH3_ENABLE|BUS_A_CH4_ENABLE|BUS_A_CH5_ENABLE|
                                                     BUS_B_CH0_ENABLE|BUS_B_CH1_ENABLE|BUS_B_CH2_ENABLE|BUS_B_CH3_ENABLE|BUS_B_CH4_ENABLE|BUS_B_CH5_ENABLE);
         
            gm_SetRegBitsDWord(LVDS_BUS_CD_PAD_CTRL, BUS_C_CH0_ENABLE|BUS_C_CH1_ENABLE|BUS_C_CH2_ENABLE|BUS_C_CH3_ENABLE|BUS_C_CH4_ENABLE|BUS_C_CH5_ENABLE|
                                                     BUS_D_CH0_ENABLE|BUS_D_CH1_ENABLE|BUS_D_CH2_ENABLE|BUS_D_CH3_ENABLE|BUS_D_CH4_ENABLE|BUS_D_CH5_ENABLE);
            

         }                                                     
      }
      
      #if (FEATURE_3D == ENABLE)
      if (gm_3D_Is3DModeEnabled() && gm_3D_IsODPin400MHz())
      {
         gm_ClkSelect(CLK_ODP, ODP_SEL_FS432_SYNTH_CLK2);
      }
      else
      #endif // (FEATURE_3D == ENABLE)
      {
         gm_ClearAndSetBitsWord(LVDS_CLOCK_CONFIG, DCLK_SEL, BIT2); // Set back to PLL
         gm_ClkSelect(CLK_ODP, ODP_SEL_ODP_PLL_CLK);   
      }
      #if 0 //For Debug
      //gm_ClearRegBitsWord(GPOUTPUT1, GPIO18_OUT);
      gm_SetRegBitsWord(GPOUTPUT1, GPIO18_OUT);
      #endif
   }
}


// 20130305
extern BYTE BackFlag;
extern BYTE TempMainPort;
extern BYTE SplashAspectRatio;
extern BOOL AllowKeypadAction;
void ChangeModeHandler(void)
{
   BYTE CmdBuf[1] = {0};
   
	if(BackFlag)
	{
		BackFlag = 0;

      /*ChengLin 20130315: Roll Back Original Aspect Ratio Setting*/
      if(SplashAspectRatio != OUTPUT_AR_FULL)
      {
         UserPrefOutputAspectRatio = SplashAspectRatio; //For Athena Right
         CmdBuf[0] = FALSE; //Update Flag          
         SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_DisplaySetup_CONTRL, CmdBuf); //For Athena Left
      }

      /*ChengLin 20130322: Roll Back original color setting after splash screen*/
      SOCIPC_LoadSplashScreenColorSettings(FALSE);

		if(TempMainPort != ALL_BOARDS_DP2)
		{
			//gm_Print("TempMainPort 1 ............... %d", TempMainPort);
			UserPrefCurrentInputMain = TempMainPort;
			AdjustCurrentInputMain();
		}
      
      /*ChengLin 20130315: Avoid OSD Menu Showing  during Splash Screen*/      
      AllowKeypadAction = TRUE;      
	}
}

void ImageSync_RL(void)
{
	if(GetChipID_4K2K()==CHIPID_4K2K_R)
	{
		#if (ENABLE_DFL == 0)
   		gm_WriteRegWord(SYNC_CONTROL, 0);
		#endif
	
   	gm_WriteRegWord(DV_LOCK_LOAD, 8); //gm_ReadRegWord(DV_ACTIVE_START) - 1);
   	gm_ClearRegBitsWord(GPIO_5_ENABLE, (GPIO88_EN | GPIO86_EN));
   	gm_SetRegBitsWord(ODP_MISC_CTRL1, BIT11);	
	}
	else if(GetChipID_4K2K()==CHIPID_4K2K_L)
	{
		if(UserPrefDPSWitch == DP_Input_11)
		{
			gm_WriteRegWord(IMP_H_ACT_WIDTH, stInput[gmvb_MainChannel].Hactive + 8);
		}
		else
		{
			gm_WriteRegWord(IMP_H_ACT_WIDTH, stInput[gmvb_MainChannel].Hactive);
		}
	 
	 	// Lock config 20120309
	 	gm_SetRegBitsWord(GPIO_5_ENABLE, GPIO84_EN);
	 	gm_SetRegBitsWord(GPIO_5_DIRCTRL, GPIO84_EN);

		gm_SetRegBitsWord(IPP_IBD_CONTROL, BIT0);
	 	gm_WriteRegWord(IPP_H_ACT_START, gm_ReadRegWord(IPP_IBD_HSTART));
	 	gm_WriteRegWord(IPP_V_ACT_START_ODD, 8); //gm_ReadRegWord(IPP_IBD_VSTART));
	 	gm_WriteRegWord(IPP_V_ACT_START_EVEN, 8); //gm_ReadRegWord(IPP_IBD_VSTART));
	 	gm_WriteRegWord(IPP_VLOCK_EVENT, 0x1);
	 	gm_ClearRegBitsWord(IPP_CONTROL, IPP_EXT_DV_EN);

		#if (ENABLE_DFL == 0) && (DEFAULT_PANEL!=ST_1080P_VEGA_1080P)   //if panel is ST_1008P_VEGA, it's for test by using 2 VEGA-TX board. Just set Athena L to FreeRun
      	gm_ClearAndSetBitsWord(CLOCK_CONFIG2, (BIT11 | BIT12 | BIT13 | BIT14), (BIT12 | BIT13));
      	gm_WriteRegWord(EXT_CLK_SEL_0,0xf);
      	gm_WriteRegWord(DV_LOCK_LOAD, 8); // gm_ReadRegWord(DV_ACTIVE_START));
      	gm_WriteRegWord(DH_LOCK_LOAD, 0xA4); //1);
			gm_SetRegBitsWord(SYNC_CONTROL, (BIT2 | BIT3));
		#endif
	}
	gm_ForceUpdate(CH_A,INPUT_OUTPUT_CLOCK);
	gm_ForceUpdate(CH_B,INPUT_OUTPUT_CLOCK);
}

void ImageSync_HStart(void)
{
	gm_SetRegBitsWord(IPP_IBD_CONTROL, IPP_MEASURE_DE_nDATA);
	gm_WriteRegWord(IPP_H_ACT_START, gm_ReadRegWord(IPP_IBD_HSTART));
	gm_SetRegBitsWord(SYNC_CONTROL, (BIT2 | BIT3));
	gm_ForceUpdate(CH_A,INPUT_OUTPUT_CLOCK);
	gm_ForceUpdate(CH_B,INPUT_OUTPUT_CLOCK);
}

void ImageSync_FE(void)
{
   if(gm_ReadRegWord(BYPASS) & PASS_THROUGH) //Add by ChengLin 20130327
   {
  	UpdateCurrentPanelData(gmvb_MainChannel);
  	DisplaySetTiming();
  	gm_ClearRegBitsWord(BYPASS,PASS_THROUGH);
  	gm_ClkSelect(CLK_ODP, ODP_SEL_DCLK);
  	gm_ClkSelect(CLK_LVTX_IN, LVTX_IN_SEL_DDDS);		

  	// Test
  	gm_SetRegBitsWord(LVDS_CLOCK_CONFIG, DCLK_SEL);
  	gm_ClkSelect(CLK_ODP_CAPTURE, ODP_CAPTURE_SEL_DCLK);
   	
  	gm_ForceUpdate(CH_A,INPUT_OUTPUT_CLOCK);
  	DpTxEnableVideo(DPTX_TIMING_BY_OUTPUT);
   }
}

// 20130306
// Patch for IPP IBD abnormal delay issue.
// Set correct H start to IPP, if get different IBD value during no sync. 
void Image_Hstart_Patch(void)
{
	if((gm_ReadRegWord(IPP_IBD_HSTART) != gm_ReadRegWord(IPP_H_ACT_START)) && !(stInput[CH_A].ModeFlags & gmd_MODE_COMPLETE))
   {
   	gm_WriteRegWord(IPP_H_ACT_START, gm_ReadRegWord(IPP_IBD_HSTART));
		gm_ForceUpdate(CH_B,INPUT_CLOCK);
   }
}
#endif

void gm_4Kx2k_LVDS_Control(void)
{
#ifdef ST_4K2K_93xx_BOARD
	if((GetChipID_4K2K()==CHIPID_4K2K_R) || (GetChipID_4K2K()==CHIPID_4K2K_L))
	{
		//gm_Print("reture LVDS command ... %d", 1);	
		return;
	}
#endif	
}

//*********************************  END  **************************************
