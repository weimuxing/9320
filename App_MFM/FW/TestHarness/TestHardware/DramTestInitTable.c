/*
	$Workfile:   DramTestInitTable.c  $
	$Revision: 1.8 $
	$Date: 2012/07/09 06:33:49 $
*/
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
// MODULE:      DramTestRegInitTable.c
//
// USAGE:       This module containes register initialisation table for APPSTEST 112
//
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "system\all.h"
#include "TestHarness\Appstest.h"
#include "StdLibInc\gm_Register.h"

#ifdef DRAM_DQS_APPSTEST_ENABLE // Not work for new DDR interface

ROM tWa_LutSignature Wa_LutSignature ={0x00,0xff,0x00,0xff,0x00,0xff};

ROM WORD Wa_DqsRegistersList[] =
#if (FEATURE_MC_TIMING_CONFIGURATION < REV3)
   {
      MC_WRITE_DATA_TIMING,
      MC_READ_DQS0_TIMING,
      MC_READ_DQS1_TIMING,
      MC_READ_DQS2_TIMING,
      MC_READ_DQS3_TIMING,
      0
   };
#else
{
   MEMC_WR_DATA_TIMING_BYTE0,
   MEMC_WR_DATA_TIMING_BYTE1,
   MEMC_WR_DATA_TIMING_BYTE2,
   MEMC_WR_DATA_TIMING_BYTE3,
   MEMC_RD_TIMING_BYTE0_EVEN ,
   MEMC_RD_TIMING_BYTE0_ODD ,
   MEMC_RD_TIMING_BYTE1_EVEN,
   MEMC_RD_TIMING_BYTE1_ODD ,
   MEMC_RD_TIMING_BYTE2_EVEN,
   MEMC_RD_TIMING_BYTE2_ODD ,
   MEMC_RD_TIMING_BYTE3_EVEN,
   MEMC_RD_TIMING_BYTE3_ODD,
   0
};
#endif
#if (FEATURE_MC_TIMING_CONFIGURATION < REV3)
ROM BYTE Name1[] = "MC_WRITE_DATA_TIMING";
ROM BYTE Name2[] = "MC_READ_DQS0_TIMING";
ROM BYTE Name3[] = "MC_READ_DQS1_TIMING";
ROM BYTE Name4[] = "MC_READ_DQS2_TIMING";
ROM BYTE Name5[] = "MC_READ_DQS3_TIMING";
#else
ROM BYTE Name1[] = "MEMC_WR_DATA_TIMING_BYTE0";
ROM BYTE Name2[] = "MEMC_WR_DATA_TIMING_BYTE1";
ROM BYTE Name3[] = "MEMC_WR_DATA_TIMING_BYTE2";
ROM BYTE Name4[] = "MEMC_WR_DATA_TIMING_BYTE3";
ROM BYTE Name5[] = "MEMC_RD_TIMING_BYTE0_EVEN";
ROM BYTE Name6[] = "MEMC_RD_TIMING_BYTE0_ODD";
ROM BYTE Name7[] = "MEMC_RD_TIMING_BYTE1_EVEN";
ROM BYTE Name8[] = "MEMC_RD_TIMING_BYTE1_ODD";
ROM BYTE Name9[] = "MEMC_RD_TIMING_BYTE2_EVEN";
ROM BYTE Namea[] = "MEMC_RD_TIMING_BYTE2_ODD";
ROM BYTE Nameb[] = "MEMC_RD_TIMING_BYTE3_EVEN";
ROM BYTE Namec[] = "MEMC_RD_TIMING_BYTE3_ODD";
#endif

ROM BYTE * Wa_DqsRegsNameTbl[] =
{
   Name1,
   Name2,
   Name3,
   Name4,
   Name5,
#if (FEATURE_MC_TIMING_CONFIGURATION == REV3)
   Name6,
   Name7,
   Name8,
   Name9,
   Namea,
   Nameb,
   Namec,
#endif
   0
};



ROM WORD Wa_RegInitSequence[] =
{
#if (CHIP_ID == STDP93xx)
   // Louis ... To Do --> Add memory config
#endif
   CLOCK_CONFIG1,    0x28B,
   MEMC_CONFIG,    0x0,
   MC_CONFIG,    0x4390,
   SOFT_RESETS,    0x100,
   SOFT_RESETS,    0x0,
   MEMC_HIT_PATTERN_BYTE0,    0x1FF8,
   MEMC_HIT_PATTERN_BYTE1,    0x1FF8,
   MEMC_HIT_PATTERN_BYTE2,    0x1FF8,
   MEMC_HIT_PATTERN_BYTE3,    0x1FF8,
   MEMC_HIT_PATTERN2_BYTE0,    0x134,
   MEMC_HIT_PATTERN2_BYTE1,    0x134,
   MEMC_HIT_PATTERN2_BYTE2,    0x134,
   MEMC_HIT_PATTERN2_BYTE3,    0x134,
   MEMC_PREAMBLE_CONFIG_BYTE0,    0x188,
   MEMC_PREAMBLE_CONFIG_BYTE1,    0x188,
   MEMC_PREAMBLE_CONFIG_BYTE2,    0x188,
   MEMC_PREAMBLE_CONFIG_BYTE3,    0x188,
   MEMC_DRIVE_ADJUST,    0x702,
   MEMC_DDR_PHY_CTL,    0x2,
   MEMC_DDR_PHY_CTL,    0x0,
   MEMC_DDR_PHY_CTL,    0x1,
   MEMC_DDR_PHY_CTL,    0x0,
   MEMC_DLL_STATUS,    0x11C,
   MEMC_DLL_CONTROL,    0x2,
   MEMC_DLL_CONTROL,    0x3,
   MEMC_DLL_CONTROL,    0x2,
   MC_TINIT_TIME,    0x2BC,
   MEMC_CONFIG,    0x4028,
   MEMC_INIT_MR,    0x952,
   MEMC_INIT_EMR1,    0x386,
   MEMC_DDR_INIT_EMR2,    0x0,
   MEMC_DDR_INIT_EMR3,    0x0,
   MEMC_EXT_DLL_LOCK_TIME,    0xC8,
   MEMC_DDR_INIT_TIME1,    0xE686,
   MEMC_DDR_INIT_TIME2,    0x11D8,
   MEMC_TIMING_1,    0x1C65,
   MEMC_TIMING_2,    0x2CC,
   MEMC_TIMING_3,    0x8FD,
   MEMC_TIMING_4,    0x71E,
   MEMC_TIMING_5,    0x40F,
   WR_SDRAM_CONTROL,    0x30C0,
   WR_SDRAM_CONTROL,    0x20C0,
   MC_CONFIG,    0x4391,
   MEMC_CONFIG,    0x612A,
   MC_REQUEST_CTRL,    0x6,
   MC_MEMORY_CONFIG,    0x0,
   MEMC_WR_DATA_TIMING_BYTE0,    0x0,
   MEMC_WR_DATA_TIMING_BYTE1,    0x0,
   MEMC_WR_DATA_TIMING_BYTE2,    0x0,
   MEMC_WR_DATA_TIMING_BYTE3,    0x0,
   MEMC_RD_TIMING_BYTE0_EVEN,    0x0,
   MEMC_RD_TIMING_BYTE0_ODD,    0x0,
   MEMC_RD_TIMING_BYTE1_EVEN,    0x0,
   MEMC_RD_TIMING_BYTE1_ODD,    0x0,
   MEMC_RD_TIMING_BYTE2_EVEN,    0x0,
   MEMC_RD_TIMING_BYTE2_ODD,    0x0,
   MEMC_RD_TIMING_BYTE3_EVEN,    0x0,
   MEMC_RD_TIMING_BYTE3_ODD,    0x0,
   CLOCK_CONFIG1,    0x28B,
   CLOCK_CONFIG2,    0x3319,
   CLOCK_CONFIG3,    0x2A,
#if (CHIP_ID == STDP93xx)
   DDDS1_INITIAL_FREQUENCY,    0x47,
   (DDDS1_INITIAL_FREQUENCY + 2),    0x7911,
   DDDS1_INIT,    0x1,
   DDDS1_CONTROL,    0x90,
   DDDS1_COMP_INTEGER,    0x552,
   DDDS1_COMP_FRACTION,    0x10,
   (DDDS1_COMP_FRACTION + 2),    0x8421,
   DDDS1_REF_LINE_VAL,    0x540,
#endif
   PANEL_PWR_UP,    0x3939,
   PANEL_PWR_DN,    0x3939,
   DIP_MAIN_CTRL,    0x0,
   DIP_MAIN_MAP0,    0x7DF,
   DIP_MAIN_MAP1,    0xC204,
   DIP_MAIN_MAP2,    0x1440,
   DIP_MAIN_MAP3,    0xF1C6,
   DIP_MAIN_MAP4,    0x507D,
   DIP_MAIN_MAP5,    0x4D24,
   DIP_MAIN_MAP6,    0x6554,
   DIP_MAIN_MAP7,    0xDF5D,
   DIP_MAIN_MAP8,    0x2487,
   DIP_MAIN_MAP9,    0xC2CA,
   DIP_MAIN_MAP10,    0xCE34,
   DIP_MAIN_MAP11,    0x3,
   IMP_SOURCE,    0x8,
   IMP_CONTROL,    0x101,
   IMP_H_ACT_START,    0x122,
   IMP_H_ACT_WIDTH,    0x400,
   IMP_V_ACT_START_ODD,    0x23,
   IMP_V_ACT_START_EVEN,    0x23,
   IMP_V_ACT_LENGTH,    0x300,
   IMP_VLOCK_EVENT,    0x4,
   IMP_FRAME_RESET_LINE,    0x21,
   IMP_RFF_READY_LINE,    0x23,
   DIP_PIP_CTRL,    0x0,
   DIP_PIP_MAP0,    0x7DF,
   DIP_PIP_MAP1,    0xC204,
   DIP_PIP_MAP2,    0x1440,
   DIP_PIP_MAP3,    0xF1C6,
   DIP_PIP_MAP4,    0x507D,
   DIP_PIP_MAP5,    0x4D24,
   DIP_PIP_MAP6,    0x6554,
   DIP_PIP_MAP7,    0xDF5D,
   DIP_PIP_MAP8,    0x2487,
   DIP_PIP_MAP9,    0xC2CA,
   DIP_PIP_MAP10,    0xCE34,
   DIP_PIP_MAP11,    0x3,
   IPP_SOURCE,    0x8,
   IPP_CONTROL,    0x101,
   IPP_H_ACT_START,    0x122,
   IPP_H_ACT_WIDTH,    0x400,
   IPP_V_ACT_START_ODD,    0x23,
   IPP_V_ACT_START_EVEN,    0x23,
   IPP_V_ACT_LENGTH,    0x300,
   IPP_VLOCK_EVENT,    0x4,
   IPP_FRAME_RESET_LINE,    0x21,
   MC_MEMORY_CONFIG,    0x0,
   MFRC_CTRL,    0x20,
   MFRC_PIXELS_OUT,    0x400,
   MFRC_LINES_OUT,    0x2BC,
   MFRC_BUF_WIDTH,    0x3C,
   MFRC_BUF_STRIDE_hi,    0x0,
   MFRC_BUF_STRIDE_lo,    0xA410,
   MFRC_WR_WIDTH,    0x3C,
   MFRC_WBUF_START_hi,    0x0,
   MFRC_WBUF_START_lo,    0x0,
   MFRC_WBUF_END_hi,    0x0,
   MFRC_WBUF_END_lo,    0xA3D4,
   MFRC_RD_WIDTH,    0x3C,
   MFRC_RBUF_START_hi,    0x0,
   MFRC_RBUF_START_lo,    0x0,
   MFRC_RBUF_END_hi,    0x0,
   MFRC_RBUF_END_lo,    0xA3D4,
   PFRC_CTRL,    0x30,
   PFRC_BUF_STRIDE_hi,    0x0,
   PFRC_BUF_STRIDE_lo,    0x6E28,
   PFRC_WR_WIDTH,    0x2F,
   PFRC_WBUF_START_hi,    0x1,
   PFRC_WBUF_START_lo,    0xEC30,
   PFRC_WBUF_END_hi,    0x2,
   PFRC_WBUF_END_lo,    0x5A29,
   PFRC_R0_PIXELS_OUT,    0x320,
   PFRC_R0_LINES_OUT,    0x258,
   PFRC_R0_BUF_WIDTH,    0x2F,
   PFRC_R0_RD_WIDTH,    0x2F,
   PFRC_R0_RBUF_START_hi,    0x1,
   PFRC_R0_RBUF_START_lo,    0xEC30,
   PFRC_R0_RBUF_END_hi,    0x2,
   PFRC_R0_RBUF_END_lo,    0x5A29,
   PFRC_R1_PIXELS_OUT,    0x0,
   PFRC_R1_LINES_OUT,    0x0,
   PFRC_R2_PIXELS_OUT,    0x0,
   PFRC_R2_LINES_OUT,    0x0,
   MOTION_PROC_CTRL,    0x8,
   MHF_CTRL,    0x7,
   MHF_SHARPNESS,    0x0,
   MHF_NOISE_CORING_CTRL,    0x0,
   MHF_NONLINEAR_CTRL1,    0x47FF,
   MHF_NONLINEAR_CTRL2,    0x47FF,
   MHF_SCALE_VALUE_hi,    0xE,
   MHF_SCALE_VALUE_lo,    0xE501,
   MVF_CTRL,    0x1B,
   MVF_SHARPNESS,    0x0,
   MVF_NOISE_CORING_CTRL,    0x0,
   MVF_NONLINEAR_CTRL1,    0x47FF,
   MVF_NONLINEAR_CTRL2,    0x47FF,
   MVF_SV_hi,    0x4,
   MVF_SV_lo,    0x6379,
   MVF_OFFSET0_hi,    0x2,
   MVF_OFFSET0_lo,    0x800,
   MVF_OFFSET1_hi,    0x2,
   MVF_OFFSET1_lo,    0x800,
   PHF_CTRL,    0x1,
   PHF_SHARPNESS,    0x0,
   PHF_NOISE_CORING_CTRL,    0x0,
   PHF_NONLINEAR_CTRL1,    0x47FF,
   PHF_NONLINEAR_CTRL2,    0x47FF,
   PHF_SCALE_VALUE_hi,    0x14,
   PHF_SCALE_VALUE_lo,    0x7AE1,
   PVF_CTRL,    0x1,
   PVF_SHARPNESS,    0x0,
   PVF_NOISE_CORING_CTRL,    0x0,
   PVF_NONLINEAR_CTRL1,    0x47FF,
   PVF_NONLINEAR_CTRL2,    0x47FF,
   PVF_SV_hi,    0x5,
   PVF_SV_lo,    0x1EB8,
   PVF_OFFSET0_hi,    0x2,
   PVF_OFFSET0_lo,    0x800,
   PVF_OFFSET1_hi,    0x2,
   PVF_OFFSET1_lo,    0x800,
#if (CHIP_ID == STDP93xx)
   // Louis ... To Do --> Modify RGB2YUV later
#endif
#if (CHIP_ID == STDP93xx)
   COLOR_MATRIX_CTRL,    0x0,
   MAIN_P_MULT_COEF11,    0x100,
   MAIN_P_MULT_COEF12,    0x0,
   MAIN_P_MULT_COEF13,    0x0,
   MAIN_P_IN_OFFSET1,    0x0,
   MAIN_P_MULT_COEF21,    0x0,
   MAIN_P_MULT_COEF22,    0x100,
   MAIN_P_MULT_COEF23,    0x0,
   MAIN_P_IN_OFFSET2,    0x0,
   MAIN_P_MULT_COEF31,    0x0,
   MAIN_P_MULT_COEF32,    0x0,
   MAIN_P_MULT_COEF33,    0x100,
   MAIN_P_IN_OFFSET3,    0x0,
   PB_MAIN_HSTART,    0x46,
   PB_MAIN_HWIDTH,    0x44C,
   PB_MAIN_VSTART,    0x0,
   PB_MAIN_VHEIGHT,    0x2BC,
   PB_BKGND,    0xF9E7,
   COLOR_MATRIX_CTRL,    0x0,
   PIP_MULT_COEF11,    0x100,
   PIP_MULT_COEF12,    0x0,
   PIP_MULT_COEF13,    0x0,
   PIP_IN_OFFSET1,    0x0,
   PIP_MULT_COEF21,    0x0,
   PIP_MULT_COEF22,    0x100,
   PIP_MULT_COEF23,    0x0,
   PIP_IN_OFFSET2,    0x0,
   PIP_MULT_COEF31,    0x0,
   PIP_MULT_COEF32,    0x0,
   PIP_MULT_COEF33,    0x100,
   PIP_IN_OFFSET3,    0x0,
#endif
   PB_PIP_HSTART,    0x0,
   PB_PIP_HWIDTH,    0x320,
   PB_PIP_VSTART,    0x0,
   PB_PIP_VHEIGHT,    0x258,
   PB_PIP_MASK,    0x1,
   MAIN_SCALING_FILTER_CTRL,    0xD,
   PIP_SCALING,    0x5,
//    ODP_CTRL,    0x3,
   TIMING_CONFIG,    0x1,
   PB_CTRL,    0xB,
   PB_BKGND_WIDTH,    0x500,
   PB_BKGND_HEIGHT,    0x300,
   PB_BKGND,    0xF9E7,
   DH_TOTAL,    0x548,
   DH_HS_WIDTH,    0x2,
   DH_DE_START,    0x18,
   DH_ACTIVE_START,    0x18,
   DH_ACTIVE_WIDTH,    0x500,
   DH_DE_END,    0x518,
   DISPLAY_VSYNC,    0x600,
   DV_TOTAL,    0x32C,
   DV_DE_START,    0x29,
   DV_ACTIVE_START,    0x29,
   DV_ACTIVE_LENGTH,    0x300,
   DV_DE_END,    0x329,
   SYNC_CONTROL,    0x1,
   LVDS_DIGITAL_CTRL,    0x1000,
   ODP_MISC_CTRL,    0x10,
#if (CHIP_ID == STDP93xx)
   //ODP_LVDS_MISC_CTRL,    0x888,
#if 1
   LVDS_TX_MAP0, 0x8A66,
   LVDS_TX_MAP1,    0x628E,
   LVDS_TX_MAP2,    0x5608,
   LVDS_TX_MAP3,    0x454,
   LVDS_TX_MAP4,    0xD24D,
   LVDS_TX_MAP5,    0xC09,
   LVDS_TX_MAP6,    0x4942,
   LVDS_TX_MAP7,    0xD65E,
   //LVDS_TX_MAP8,    0x1461,
   //LVDS_TX_MAP9,    0x8644,
   //LVDS_TX_MAP10,    0xB605,
   //LVDS_TX_MAP11,    0xDB6D,
   //LVDS_TX_MAP12,    0x8276,
   //LVDS_TX_MAP13,    0x0,
#else
   ODP_LVDS_TX_MAP0,    0x8A66,
   ODP_LVDS_TX_MAP1,    0x628E,
   ODP_LVDS_TX_MAP2,    0x5608,
   ODP_LVDS_TX_MAP3,    0x454,
   ODP_LVDS_TX_MAP4,    0xD24D,
   ODP_LVDS_TX_MAP5,    0xC09,
   ODP_LVDS_TX_MAP6,    0x4942,
   ODP_LVDS_TX_MAP7,    0xD65E,
   ODP_LVDS_TX_MAP8,    0x1461,
   ODP_LVDS_TX_MAP9,    0x8644,
   ODP_LVDS_TX_MAP10,    0xB605,
   ODP_LVDS_TX_MAP11,    0xDB6D,
   ODP_LVDS_TX_MAP12,    0x8276,
   ODP_LVDS_TX_MAP13,    0x0,
#endif
#endif
   HOST_CONTROL,    0xF0,
#if (CHIP_ID == STDP93xx)
   //SOFT_PD,    0x7FFF,
   DISPLAY_CONTROL,    0x7,
   //LVDS_POWER,    0x3F,
   //LVDS_DIGITAL_CTRL,    0x2C,
   //LVDS_MISC_CTRL,    0x1C8,
   // Louis ... To Do --> Add LVDS Initial
#endif
   AIP_STG_VTOTAL,    0x326,
#if (CHIP_ID == STDP93xx)
   // No AIP2
#endif
   SDDS1_CONTROL,    0x91,
#if (CHIP_ID == STDP93xx)
   (SDDS1_INI_FREQ  + 2),    0x41,
   SDDS1_INI_FREQ ,    0x7D00,
#endif
   SDDS1_HTOTAL,    0x540,
   SDDS1_INIT,    0x1,
#if (CHIP_ID == STDP93xx)
   // Louis ... To Do --> SDDS2??
   //(SDDS2_INITIAL_FREQ + 2),    0x41,
   //SDDS2_INITIAL_FREQ,    0x7D00,
   //SDDS2_HTOTAL,    0x540,
   //SDDS2_INIT,    0x1,
#endif
   IMP_PATGEN_CONTROL,    0x23,
   IMP_PATGEN_BLUE,    0x1F,
   IMP_PATGEN_GRN,    0x1F,
   IMP_PATGEN_RED,    0x1F,
   IPP_PATGEN_CONTROL,    0x23,
   IPP_PATGEN_BLUE,    0x1F,
   IPP_PATGEN_GRN,    0x1F,
   IPP_PATGEN_RED,    0x1F,
   IMP_SOURCE,    0x1E,
   IPP_SOURCE,    0x1E,
   CLOCK_CONFIG2,    0x2A2A,
#if (CHIP_ID == STDP93xx)
   // Louis ... To Do --> Check DDS later
   //DDS_RD_SRC_SEL,    0x0,
   //DDS_FREQ_0,    0x7D00,
   //DDS_FREQ_1,    0x41,
   //DDS_CUR_TRACK_ERR,    0x6,
#endif
   0,0
};
#endif //#ifdef DRAM_DQS_APPSTEST_ENABLE 
