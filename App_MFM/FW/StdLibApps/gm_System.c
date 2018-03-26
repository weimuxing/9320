/*
   $Workfile:   gm_System.c  $
   $Revision: 1.26 $
   $Date: 2012/10/03 08:41:20 $
*/
#define __GM_SYSTEM_C__
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
// MODULE:      gm_System.c
//
// USAGE:       This module contains gmc_ standard library constants for system
//          setup and configuration parameters
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "system\all.h"
#include "StdLibInc\gm_Register.h"

//******************************************************************************
//  S T A N D A R D    L I B R A R Y    C O N S T A N T S
//******************************************************************************

//
// System Clock values
//
ROM DWORD 	gmc_TCLK_FREQ = TCLK_FREQ; // 27MHZ

#if RCLK_FREQ_ARGS == RCLKSEL_432M
 ROM DWORD 	gmc_RCLK_FREQ = 432000000L;
#elif RCLK_FREQ_ARGS == RCLKSEL_378M
 ROM DWORD 	gmc_RCLK_FREQ = 378000000L;
#elif RCLK_FREQ_ARGS == RCLKSEL_270M
 ROM DWORD 	gmc_RCLK_FREQ = 270000000L;
#endif

// Max. OCM_CLK is 250Mhz
#if OCLK_FREQ_ARGS == FSCLK_250M
ROM DWORD 	gmc_OCLK_FREQ = 250000000L;
#elif OCLK_FREQ_ARGS == FSCLK_200M
ROM DWORD 	gmc_OCLK_FREQ = 200000000L;
#elif OCLK_FREQ_ARGS == FSCLK_176_9M
ROM DWORD 	gmc_OCLK_FREQ = 176900000L;
#elif OCLK_FREQ_ARGS == FSCLK_150M
ROM DWORD 	gmc_OCLK_FREQ = 150000000L;
#elif OCLK_FREQ_ARGS == FSCLK_135M
ROM DWORD 	gmc_OCLK_FREQ = 135000000L;
#elif OCLK_FREQ_ARGS == FSCLK_100M
ROM DWORD 	gmc_OCLK_FREQ = 100000000L;
#elif OCLK_FREQ_ARGS == FSCLK_27M
ROM DWORD 	gmc_OCLK_FREQ = 27000000L;
#endif

// TCLK divisor for Deep Sleep mode, has being changed from 2 to 0 in Sequoia due to lack of perfomance
// during VGA port input monitoring.
ROM BYTE gmc_TCLK_Divisor = 0;   // (value range from 1 to 255; 0 means TCLK)
//
// Panel
//
ROM DWORD   gmc_DEF_DISPLAY_RATE = 60;          // 60 Hz

//
// Memory allocation method:
//
// STATIC - Static memory allocation means that the MAIN channel and the PIP channel
//          are allocated memory at startup based on the worst case scenario.
//          The amount of memory allocated to each channel cannot change.  This means
//          that a mode change in one channel will not affect the other channel.
//
// DYNAMIC -   Dynamic memory allocation means that the MAIN channel and the PIP channel
//          are NOT allocated a specific amount of memory at startup.  Each channel
//          is allocated memory after a mode change as needed.  This allows for a more
//          efficient use of memory.  However, a mode change in the MAIN channel could
//          effect the PIP channel.  For example, if the PIP channel is using a large
//          amount of memory (UXGA input) and the MAIN channel changes to a mode
//          requiring more memory than currently allocated, the PIP channel will need
//          to give up some of its memory (by lowering the number of BPP or number of
//          buffers used).  This will require for the PIP channel to momentarily turn off.
//
ROM BYTE    gmc_MemoryAllocationMethod          = gmd_MEMORY_ALLOCATE_STATIC;

//that value is multiplied to 512 and divided to SDRAM_MEM_BURST, because it has been calculated
//by WorkBench in 512bit bursts, without regard to actual sdram memory burst size
ROM DWORD   gmc_MemoryOsdSize       = (512UL * (MAX_OSD_MEMORY + SDRAM_StartAddress)) / SDRAM_MEM_BURST;


ROM DWORD gmc_MemoryPalAutoInstallSize = 0;

//
// DRAM_CODE_EXECUTION
//
#ifdef DRAM_CODE_EXECUTION
ROM DWORD gmc_MemoryCodeSize = CODE_SIZE; //size of loaded application in bytes
ROM DWORD gmc_CodeStartAddress = CODE_START_ADDRESS; //address in bytes
#else
ROM DWORD gmc_MemoryCodeSize = 0;
ROM DWORD gmc_CodeStartAddress = 0;
#endif

ROM BYTE gmc_MemoryCacheBurst = DRAM_CACHE_BURST_SIZE; //DRAM cache burst size CACHE_CTRL3:CACHE_BURST_SIZE_DRAM (64 or 128)

ROM DWORD gmc_MemoryDataSize = DATA_DRAM_SIZE;
#ifdef DATA_DRAM_ADDR
ROM DWORD gmc_DataStartAddress = DATA_DRAM_ADDR; //address in bytes
#else
ROM DWORD gmc_DataStartAddress = 0; //address in bytes
#endif

//SERIAL_FLASH_CTRL2:FLASH_CLK_DIV_N - Number to divide host clock frequency to generate
//LCLK (for LPC flash) or SCLK (for SPI flash); values of 2,3,4 are supported; 5 and 6 values are supported in FLI326xx
#ifdef SPI_FLASH_CLK_DIVIDE
ROM BYTE gmc_FLASH_CLK_DIV_N = SPI_FLASH_CLK_DIVIDE;
#else
#if OCLK_FREQ_ARGS == FSCLK_250M
ROM BYTE gmc_FLASH_CLK_DIV_N = 6;
#elif OCLK_FREQ_ARGS == FSCLK_150M
ROM BYTE gmc_FLASH_CLK_DIV_N = 4;
#elif OCLK_FREQ_ARGS == FSCLK_100M
ROM BYTE gmc_FLASH_CLK_DIV_N = 3;
#else
ROM BYTE gmc_FLASH_CLK_DIV_N = 5;
#endif
#endif

ROM BYTE gmc_SPI_CE_WIDTH = 100; //1ns unit, 100 = 100ns, SERIAL_FLASH_CTRL3:SPI_CE_WIDTH

//
// <ttx database in DDR bursts>
//       = <# of packets in library> x <44 bytes per packet> / <64 bytes per burst>
//       = TTX_DB_NUM_OF_PACKETS x <44 bytes per packet> / <64 bytes per burst>
//       = (65535 x 44)/64
//       = 45056 (0xB000)
// <ttx pages support> = <# of packets in library> / <average packets per page>
// <ttx pages support> = TTX_DB_NUM_OF_PACKETS / <average packets per page>
//                = 65535 / (30 to 35)
//                = 2184 to 1872 pages
//
// allocate SDRAM for ACI buffer:
//        = sizeof(TT_ACI_RESULT_ITEM) * ACI_MAX_NO_OF_CHANNELS
//        = (8 * 7 + 8 + 32) * 99 bits
//        = 9504 bits/8 bits per byte
//        = 1188 bytes
//
// to ensure gmc_MemoryTeletextSize is an even number,  divide by 2, add 1, and multiply by 2
//
#if 1 // not support TELETEXT
ROM DWORD gmc_MemoryTeletextSize = 0;
#endif

#ifdef EXTERNAL_SLICER
ROM WORD gmc_MemoryVbiSize          = 0x2160;      // Double the size but make it divisible by (18 lines * 2bursts) = 32
#else
ROM WORD gmc_MemoryVbiSize          = (WORD)(2UL*1024*1024/SDRAM_MEM_BURST); // 2 MBits, declared in bursts
#endif

#if (FEATURE_OVERDRIVE == ENABLE)
ROM WORD gmc_MemoryLcdOverdriveBits    = 24;          // 24 or 16
#endif

ROM DWORD   gmc_Memory3dDecoderSize    = (44UL*1024*1024/SDRAM_MEM_BURST);

ROM WORD gmc_Memory3dDecoderWidth   = (18*1024/SDRAM_MEM_BURST);

//
// SDRAM memory parameters (application will set the proper values)
//
ROM DWORD   gmc_MemoryTotalSize = SDRAM_MEM_SIZE;
ROM WORD gmc_MemoryBurstWidth  = SDRAM_MEM_BURST;

//9 bit gain register settings
ROM WORD gmc_StartGain = 0x100;
ROM WORD gmc_EndGain   = 0x100;
ROM WORD gmc_GainStep  = 0x40;

//
// 3x3 YUV to RGB adjustment coefficients
//

#if 1
ROM SWORD  gmc_CSCCoeff [4][9] =    // CSC COEFFICIENTS: 3x3 matrix in                                
{
   {                          // applies to: improper SD to RGB output 601
      0x2541, -0x0C87, -0x1A08,     // {1.1641, -0.3914, -0.8135}    [Row] x [Coloum]
      0x2541,  0x4092, -0x000A,     // {1.1641,  2.0178, -0.0013}      [G, B, R] x [Y, Cb, Cr]
      0x2541, -0x000F,  0x3311      // {1.1641, -0.0018,  1.5958}
   },
   {                          // applies to: Improper HD to RGB output ITU709
      0x2541, -0x06D2, -0x1119,     // {1.1641, -0.2131, -0.5343}     [Row] x [Coloum]
      0x2541,  0x43A2,  0x0008,     // {1.1641,  2.1136,  0.0010}      [G, B, R] x [Y, Pb, Pr]
      0x2541, -0x0009,  0x395B      // {1.1641, -0.0011,  1.7924}
   },
   {                          // applies to: Improper HD to RGB output 1:1 ITU601
      0x2000, -0x0AC5, -0x1659,     // {1.0000, -0.3365, -0.6984}      [G, B, R] x [Y, Pb, Pr]
      0x2000,  0x377A, -0x000D,     // {1.0000,  1.7734, -0.0016}
      0x2000, -0x0002,  0x2BE5      // {1.0000, -0.0003,  1.3717}     [Row] x [Coloum]
   },  
   {                          // applies to: improper SD to RGB output 1:1 ITU709
      0x2000, -0x05DC, -0x0EAE,     // {1.0000,  -0.1831,   -0.4588}    [G, B, R] x [Y, Cb, Cr]
      0x2000,  0x3A1C, -0x0009,     // {1.0000,   1.8160,   -0.0010}
      0x2000,  0x0006,  0x314A      // {1.0000,   0.0008,    1.5403}    [Row] x [Coloum]
   }   
};
#else
ROM SWORD  gmc_CSCCoeff [2][9] =    // CSC COEFFICIENTS: 3x3 matrix in
{
   {                          // applies to: improper SD to RGB output
      0x12A0, -0x0646, -0x0D01,     // {1.164, -0.392, -0.813}    [Row] x [Coloum]
      0x12A0, 0x2046,  0x0000,      // {1.164,  2.017,   0.000}      [G, B, R] x [Y, Cb, Cr]
      0x12A0, 0x0000,   0x1989         // {1.164,  0.000,   1.596}
   },
   {                          // applies to: Improper HD to RGB output
      0x12A0, -0x0370, -0x0890,     // {1.164, -0.215 -0.535}     [Row] x [Coloum]
      0x12A0,  0x21D0, 0x0000,      // {1.164,  2.113,   0.000}      [G, B, R] x [Y, Pb, Pr]
      0x12A0,  0x0000, 0x1CB0       // {1.164,  0.000,   1.793}
   }
};
#endif

//ACCACM for Modulation support
#if 1
ROM BYTE gmc_EnableAccAcmOnGraphic = 1;
#else
ROM BYTE gmc_EnableAccAcmOnGraphic = 0;
#endif

//
// NOTE :	All the Panoramic tables are being moved to gm_PanoramicTbl.c
//			Make sure the Panoramic tables are removed from gm_System.c
//			gmc_StandardVideoPanoramicTableNonHDMI
//			gmc_NUM_OF_MODES_IN_PANORAMIC_TABLE_NON_HDMI
//          gmc_StandardVideoPanoramicTableHDMI
//          gmc_NUM_OF_MODES_IN_PANORAMIC_TABLE_HDMI
//

#if 0 //(FEATURE_DISPLAY_PORT == ENABLE)
//ROM DWORD    gmc_DPRX_LT_FREQUENCY = 135000000UL;
ROM WORD     gmc_DPRX_LTCLK_INIT = 0x8737; // for 255MHz RCLK
ROM BYTE     gmc_DPRX_VCLK1  = DPRX_VCLK1_SEL_VDDS1;
ROM BYTE     gmc_DPRX_VCLK2  = DPRX_VCLK2_SEL_VDDS1;
ROM BYTE     gmc_DPRX_AUCLK1 = DPRX_AUCLK1_SEL_AUDDS2_CLK;
ROM BYTE     gmc_DPRX_AUCLK2 = DPRX_AUCLK2_SEL_AUDDS2_CLK;

ROM BYTE     gmc_DPRX_IMPCLK = IMP_CLK_SEL_DPRX1_CLK;
ROM BYTE     gmc_DPRX_IPPCLK = IPP_CLK_SEL_DPRX1_CLK;

ROM BYTE     gmc_DPRX_AUXCLK = DPRX_AUXCLK_SEL_TCLK_DIV10;  // for 19.66 MHz
ROM BYTE     gmc_DPRX_AUXCLK_INVERSE = TRUE;

ROM BYTE     gmc_DPTX_VCLK1  = DPTX_VCLK1_SEL_DCLK;
ROM BYTE     gmc_DPTX_VCLK2  = DPTX_VCLK2_SEL_DCLK;
ROM BYTE     gmc_DPTX_AUCLK1 = DPTX_AUCLK1_SEL_AUDDS2_CLK;
ROM BYTE     gmc_DPTX_AUCLK2 = DPTX_AUCLK2_SEL_AUDDS2_CLK;

ROM BYTE     gmc_DPTX_AUXCLK = DPTX_AUXCLK_SEL_TCLK_DIV10; // for 19.66 MHz
ROM BYTE     gmc_DPTX_AUXCLK_INVERSE = FALSE;
#endif // #if (FEATURE_DISPLAY_PORT == ENABLE)

//************************************* END ************************************

