/*
   $Workfile:   gm_System_Stdp93xx.h  $
   $Revision: 1.12 $
   $Date: 2012/04/06 08:46:00 $
*/
#ifndef __GM_SYSTEM_H__
#define __GM_SYSTEM_H__
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
// MODULE:      gm_System.h
//
// USAGE:       Header file for system/board level definition
//
//******************************************************************************
//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************

#define REV_CODE_2_0          0x01   	// host register value in PRODUCT_REV
#define REV_CODE_2_1          0x02   	// host register value in PRODUCT_REV

//******************************************************************************
//  D E F I N E S
//******************************************************************************

//
// Clock Frequencies
//
#define FREQ_157MHz           157286400L              // 157.2864 MHz
#define FREQ_108MHz           108000000L              // 108.00000 MHz
//#define TCLK_FREQ  			  19660800L				  // 19.6608MHz
#define TCLK_FREQ  			  27000000L				  // 27.0000MHz

//
// Clocks Ratio ( Ratio = (Frequency/RClk) * 2^24 )
//
#define FCLK_FOR_200MHz       0xD904                  // 200.0000 MHz
#define FCLK_FOR_170MHz       0xB876                  // 170.0000 MHz
//#define SDDS_FOR_157MHz       0x00AAAAAAL             // 157.2864 MHz   // old value = 157.2863
//#define EDDS_FOR_108MHz       0x00753000L             // 108.0000 MHz
//#define FCLK_FOR_190MHz       0xCE29                  // 190.0000 MHz

//
// Minimal Hsync and Vsync supported values
//
#define H_SYNC_MIN		15000L								// measured in Hz
#define V_SYNC_MIN		30									// measured in Hz
// 20081218 #3, For Aostro DP 480i (1440x480)
#define H_WATCHDOG		(((TCLK_FREQ/H_SYNC_MIN) + (TCLK_FREQ/H_SYNC_MIN))>>6)
//#define H_WATCHDOG		(((TCLK_FREQ/H_SYNC_MIN) + (TCLK_FREQ/H_SYNC_MIN)/2)>>6)
#define V_WATCHDOG		((((TCLK_FREQ/511)/V_SYNC_MIN) + ((TCLK_FREQ/511)/V_SYNC_MIN)/2)>>7)
#define IFMWATCHDOG_VAL ((V_WATCHDOG<<8)|H_WATCHDOG)

//
// Conversion from normal RGB format to RGB 565 format
//
#define RGB(R,G,B)   ((DWORD)(B) | ((DWORD)(G) << 8) | ((DWORD)(R) << 16))
#define RGB_R(RGB)   ((WORD)(((RGB) >> 16) & 0xff))
#define RGB_G(RGB)   ((WORD)(((RGB) >> 8) & 0xff))
#define RGB_B(RGB)   ((WORD)((RGB) & 0xff))
#define RGB_565(RGB) ((WORD)((RGB_B(RGB)) >> 3) | ((WORD)((RGB_G(RGB)) >> 2) << 5) | ((WORD)((RGB_R(RGB)) >> 3) << 11))

#define  BLUE        RGB(0,0,255)
#define  GREEN       RGB(0,255,0)
#define  RED         RGB(255,0,0)
#define  BLACK       RGB(0,0,0)
#define  WHITE       RGB(255,255,255)
#define  YELLOW      RGB(255,255,0)
#define  TEAL        RGB(31,190,203)
#define  PURPLE      RGB(84,63,171)
#define  LIGHTBLUE   RGB(50,50,150)
#define  DARKGREY    RGB(50,50,50)

//
// Color Space Sine table (for Hue adjustments)
//
//#define   CSC_ONE_DEGREE_HUE         // Default uses larger 1/4 degree hue table


//#define PATCH_DVIIFM					//Fixed in Cut2
//#define PATCH_DPRX1_IFM				//Fixed in Cut2

//******************************************************************************
//  E X T E R N S
//******************************************************************************
//
// System Clock values
//
extern ROM DWORD  gmc_TCLK_FREQ;
extern ROM DWORD  gmc_RCLK_FREQ;
extern ROM DWORD  gmc_OCLK_FREQ;
extern ROM BYTE   gmc_TCLK_Divisor;

//
// Panel
//
extern ROM DWORD  gmc_DEF_DISPLAY_RATE;

//
// Memory allocation and system constants
//
extern ROM BYTE     	gmc_MemoryAllocationMethod;

extern ROM DWORD	gmc_MemoryOsdSize;
extern ROM DWORD	gmc_MemoryCodeSize;
extern ROM DWORD	gmc_CodeStartAddress;
extern ROM DWORD	gmc_MemoryDataSize;
extern ROM DWORD	gmc_DataStartAddress;
extern ROM BYTE		gmc_MemoryCacheBurst;
extern ROM BYTE		gmc_FLASH_CLK_DIV_N;
extern ROM BYTE     	gmc_SPI_CE_WIDTH;
extern ROM WORD     	gmc_MemoryVbiSize;
extern ROM DWORD	gmc_MemoryTeletextSize;
#if (FEATURE_OVERDRIVE == ENABLE)
extern ROM WORD     	gmc_MemoryLcdOverdriveBits;
#endif
extern ROM DWORD	gmc_Memory3dDecoderSize;
extern ROM DWORD	gmc_Memory3dDecoder2Size;
extern ROM WORD		gmc_Memory3dDecoderWidth;

extern ROM DWORD	gmc_MemoryTotalSize;
extern ROM WORD		gmc_MemoryBurstWidth;

//Configuration for 9bit gain registers.
extern ROM WORD     gmc_StartGain;
extern ROM WORD     gmc_EndGain;
extern ROM WORD     gmc_GainStep;
//extern ROM DWORD  gmc_DPRX_LT_FREQUENCY;
extern ROM WORD     gmc_DPRX_LTCLK_INIT;
extern ROM BYTE     gmc_DPRX_VCLK1;
extern ROM BYTE     gmc_DPRX_VCLK2;
extern ROM BYTE     gmc_DPRX_AUCLK1;
extern ROM BYTE     gmc_DPRX_AUCLK2;
extern ROM BYTE     gmc_DPRX_AUXCLK;
extern ROM BYTE     gmc_DPRX_AUXCLK_INVERSE;

extern ROM BYTE     gmc_DPRX_IMPCLK;
extern ROM BYTE     gmc_DPRX_IPPCLK;


extern ROM BYTE     gmc_DPTX_VCLK1;
extern ROM BYTE     gmc_DPTX_VCLK2;
extern ROM BYTE     gmc_DPTX_AUCLK1;
extern ROM BYTE     gmc_DPTX_AUCLK2;
extern ROM BYTE     gmc_DPTX_AUXCLK;
extern ROM BYTE     gmc_DPTX_AUXCLK_INVERSE;

#endif
//*********************************  END  **************************************

