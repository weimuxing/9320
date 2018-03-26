/*
	$Workfile:   Sdram.h  $
	$Revision: 1.26 $
	$Date: 2013/06/11 06:32:47 $
*/
#ifndef __SDRAM_H__
#define __SDRAM_H__
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
// MODULE:		Sdram.h
//
// USAGE:		File containing varion SDRAM memory configuration setting
//
//******************************************************************************

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************

#if (SDRAM_MEM_SIZE == 0x100000UL)
#define VAL_MC_CONFIG 0x44C1	// Total size 512Mb

// Mixer
#define VAL_BankRegLUT				0x2D6A

#define VAL_MIXER_ROW_ADDR_MASK_lo	0xF000
#define VAL_MIXER_ROW_ADDR_MASK_hi	0xFFFF
#elif (SDRAM_MEM_SIZE == 0x200000UL)
#define VAL_MC_CONFIG 0x4501	// Total size 1Gb

// Mixer
#define VAL_BankRegLUT				 0x316A

#define VAL_MIXER_ROW_ADDR_MASK_lo	0xE000
#define VAL_MIXER_ROW_ADDR_MASK_hi	0xFFFF
#else
#define VAL_MC_CONFIG 0x4541	// Total size 2Gb

// Mixer
#define VAL_BankRegLUT				 0x316A

#define VAL_MIXER_ROW_ADDR_MASK_lo	0xE000
#define VAL_MIXER_ROW_ADDR_MASK_hi	0xFFFF
#endif

#define OPCODE_MASK		0x7UL
#define OPCODE_NOP		0
#define OPCODE_PREA		1
#define OPCODE_REF		2
#define OPCODE_MRS		3
#define OPCODE_ZQCL		5

#define VAL_OPCODE_MRS_MR1_OcdCal	(VAL_OPCODE_MRS_MR1 | 0x00003800)
#define VAL_OPCODE_MRS_MR0_ActDll	(VAL_OPCODE_MRS_MR0 & ~0x00001000)

#define Val_PCTL32_MCMD	0x02100000

#if (defined(Qisda_93XX_BOARD))//20120927;modfied for all DDR with clock delay from Ted's request ;Barbara Lin 
#define VAL_PCTL32_PHY_ACBDLR    0x00010410
#else
#define VAL_PCTL32_PHY_ACBDLR    0x00000000
#endif
#define FineTune_WDQD				0x00
#define FineTune_RDQSD				0x00
#if (DDRType == DDR2) // For DDR2 system Enable_2T may cause system hang or parameters wrong (Nanya DDR2)
#define Enable_2T	0
#else
#define Enable_2T	1
#endif
#define Enable_DRR	0 // Enable Double-Refresh-Rate


#ifdef Athena_U
#ifdef EV1_93xx_BOARD
	#define VAL_PCTL32_PHY_ZQ0CR		0x47D00000	// 60-34	DDR3 	 
#else
#ifdef EV2_93xx_BOARD
	#define VAL_PCTL32_PHY_ZQ0CR		0x45B00000	// DDR2 80-40 (73.6-37.5)
#else
	#if defined(RD3_93xx_BOARD) || defined(ST_4K2K_93xx_BOARD)
	#if 0	// DDR_DEVICE == DDR3_SEC_K4B1G1646GBCH9
	#define VAL_PCTL32_PHY_ZQ0CR		0x46800000	// Read out 59.3-44.8 DDR3	 
	#else
	#define VAL_PCTL32_PHY_ZQ0CR     0x47D00000  // 60-34 DDR3   
	#endif
	#elif defined(RD1_93xx_BOARD)
	#if 1	// RD1B
	#define VAL_PCTL32_PHY_ZQ0CR		0x45B00000	// 73.6-40 DDR2
	#else	// RD1A
	#define VAL_PCTL32_PHY_ZQ0CR		0x37B319EF	// 73.6-30.3	DDR2
	#endif
	#else // RD2_93xx_BOARD
	#define VAL_PCTL32_PHY_ZQ0CR		0x45B00000	// 73.6-40 DDR2
#endif
#endif
#endif
#else	// Athena_C
	#ifdef EV1_93xx_BOARD
	#define VAL_PCTL32_PHY_ZQ0CR		0x45B00000	// 73.6-40 DDR2
	#else
	#ifdef RD1_93xx_BOARD
	#define VAL_PCTL32_PHY_ZQ0CR		0x47D00000	// 60-34 DDR3	 
	#else
	#define VAL_PCTL32_PHY_ZQ0CR		0x45B00000	// 73.6-40 DDR2
	#endif
	#endif
#endif

#define VAL_CTL_PLL_DDR_FREQ_lo		((WORD)VAL_CTL_PLL_DDR_FREQ)
#define VAL_CTL_PLL_DDR_FREQ_hi		((WORD)(VAL_CTL_PLL_DDR_FREQ >> 16))

// Mixer
#define VAL_MIXER_DDR_BASE_ADDR_lo		0x0000
#define VAL_MIXER_DDR_BASE_ADDR_hi		0x00C0

//#define VAL_MIXER_GEN_CTRL_lo			0x0000	// Enable Mixer for debug only.
#define VAL_MIXER_GEN_CTRL_lo			0x0001
#define VAL_MIXER_GEN_CTRL_hi			0x0000

#if (ENABLE_DDR_X16 == ENABLE)
#define VAL_MIXER_GEN_PURPOSE_REG0_lo		0x1FC9
#define VAL_MIXER_GEN_PURPOSE_REG0_hi		0x1040

#define VAL_MIXER_DDR_PARAMETER		((VAL_MIXER_DDR_PARAMETER_x32 & (~0x0000000C)) | 0x00000004)
#else
#define VAL_MIXER_GEN_PURPOSE_REG0_lo		0x1FCA
#define VAL_MIXER_GEN_PURPOSE_REG0_hi		0x1040

#define VAL_MIXER_DDR_PARAMETER		VAL_MIXER_DDR_PARAMETER_x32
#endif
#define VAL_MIXER_DDR_PARAMETER_lo		((WORD)VAL_MIXER_DDR_PARAMETER)
#define VAL_MIXER_DDR_PARAMETER_hi		((WORD)(VAL_MIXER_DDR_PARAMETER >> 16))

// PCTL
#define VAL_PCTL32_ODTCFG	0x00000008

#define VAL_PCTL32_PHYPVTCFG	0x000045D0
#define VAL_PCTL32_PHY_PLLCR	0x0001C000

#if (ENABLE_DDR_X16 == 1)
#define VAL_PCTL32_PPCFG		0x00000001
#define VAL_PCTL32_PUB_PPMCFG	0x00000001

#define VAL_PCTL32_PHY_DX0GCR	0x000001F0
#define VAL_PCTL32_PHY_DX1GCR	0x000001F0
#define VAL_PCTL32_PHY_DX2GCR	0x000001F0
#define VAL_PCTL32_PHY_DX3GCR	0x000001F0
#else
#define VAL_PCTL32_PPCFG		0x00000000
#define VAL_PCTL32_PUB_PPMCFG	0x00000000

#define VAL_PCTL32_PHY_DX0GCR	0x000001F1
#define VAL_PCTL32_PHY_DX1GCR	0x000001F1
#define VAL_PCTL32_PHY_DX2GCR	0x000001F1
#define VAL_PCTL32_PHY_DX3GCR	0x000001F1
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////
#if DDR_DEVICE == DDR2_NANYA_NT5TU64M16GGBE
// Spreadsheet - SDRAM_H_DDR2_NT5TU64M16GGBE.xlsx

#define DdrDevice               "DDR2 - NT5TU64M16GG-BE"

// DDR Clock
#define VAL_CTL_PLL_DDR_FREQ    0x000023A6    // 261MHz

// Mixer
#define VAL_MIXER_DDR_PARAMETER_x32 0x3C31D028    // It was 0x43321008

// PCTL
#define VAL_PCTL32_MCFG         0x00060001    // Open Page. Close page makes BW issue while Mixer is bypassed.

#define VAL_PCTL32_DQSECFG      0x00112122
#define VAL_PCTL32_ODTCFG1      0x07070606

#define VAL_PCTL32_TOGCNT1U     0x00000105
#define VAL_PCTL32_TINIT        0x000000C8
#define VAL_PCTL32_TRSTH        0x00000000
#define VAL_PCTL32_TOGCNT100N   0x0000001B

#define VAL_PCTL32_TREFI	0x0000004E
#define VAL_PCTL32_TMRD		0x00000002
#define VAL_PCTL32_TRFC         0x00000043
#define VAL_PCTL32_TRP		0x00000007
#define VAL_PCTL32_TRTW         0x00000005
#define VAL_PCTL32_TAL		0x00000003
#define VAL_PCTL32_TCL		0x00000007
#define VAL_PCTL32_TCWL		0x00000006
#define VAL_PCTL32_TRAS		0x00000018
#define VAL_PCTL32_TRC		0x0000001F
#define VAL_PCTL32_TRCD		0x00000007
#define VAL_PCTL32_TRRD		0x00000006
#define VAL_PCTL32_TRTP		0x00000004
#define VAL_PCTL32_TWR		0x00000008
#define VAL_PCTL32_TWTR		0x00000004
#define VAL_PCTL32_TEXSR	0x000000C8
#define VAL_PCTL32_TXP		0x00000003
#define VAL_PCTL32_TXPDLL	0x00000007
#define VAL_PCTL32_TCKE		0x00000003

#define VAL_PCTL32_PHYIOCRV1    0x00005505    // 0x00006501 for Static ODT
#define VAL_PCTL32_PUB_SLCR     0x00112100    // DDR doesn't work. Bit[7:4] and [1:0] must be kept as 0 to make it working.


#define VAL_OPCODE_MRS_MR2			0x00040000
#define VAL_OPCODE_MRS_MR3			0x00060000
#define VAL_OPCODE_MRS_MR1      0x000201E0    // ODT = 75ohm
#define VAL_OPCODE_MRS_MR0			0x0000F730
#endif	// DDR_DEVICE == DDR2_NANYA_NT5TU64M16GGBE
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
#if DDR_DEVICE == DDR3_NANYA_NT5CB64M16DPCF
// Spreadsheet - SDRAM_H_DDR3_NT5CB64M16DPCF.xlsx

#define DdrDevice 			"DDR3 - NT5CB64M16DP-CF"

// DDR Clock
#define VAL_CTL_PLL_DDR_FREQ    0x000023A6    // 261MHz

// Mixer
#define VAL_MIXER_DDR_PARAMETER_x32 0x4C21D029    // It was 0x60529829

// PCTL
#define VAL_PCTL32_MCFG         0x000A0021    // Open Page. Close page makes BW issue while Mixer is bypassed.

#define VAL_PCTL32_DQSECFG      0x00111122
#define VAL_PCTL32_ODTCFG1      0x06060200

#define VAL_PCTL32_TOGCNT1U     0x00000105
#define VAL_PCTL32_TINIT    	0x000000C8
#define VAL_PCTL32_TRSTH    	0x000001F4
#define VAL_PCTL32_TOGCNT100N   0x0000001B

#define VAL_PCTL32_TREFI	0x0000004E
#define VAL_PCTL32_TMRD		0x00000004
#define VAL_PCTL32_TRFC         0x0000003A
#define VAL_PCTL32_TRP		0x00000007
#define VAL_PCTL32_TRTW         0x00000007
#define VAL_PCTL32_TAL		0x00000000
#define VAL_PCTL32_TCL		0x00000008
#define VAL_PCTL32_TCWL         0x00000006
#define VAL_PCTL32_TRAS         0x00000013
#define VAL_PCTL32_TRC		0x0000001A
#define VAL_PCTL32_TRCD		0x00000007
#define VAL_PCTL32_TRRD		0x00000004
#define VAL_PCTL32_TRTP		0x00000004
#define VAL_PCTL32_TWR		0x00000008
#define VAL_PCTL32_TWTR		0x00000004
#define VAL_PCTL32_TEXSR	0x00000200
#define VAL_PCTL32_TXP		0x00000004
#define VAL_PCTL32_TXPDLL	0x0000000D
#define VAL_PCTL32_TCKE		0x00000003

#define VAL_PCTL32_TZQSCS       0x00000040
#define VAL_PCTL32_TZQSCSI      0x00000000
#define VAL_PCTL32_TDQS         0x00000000
#define VAL_PCTL32_TCKSRE       0x00000006
#define VAL_PCTL32_TCKSRX       0x00000006
#define VAL_PCTL32_TMOD         0x0000000C
#define VAL_PCTL32_TRSTL        0x00000064
#define VAL_PCTL32_TZQCL        0x00000200

#define VAL_PCTL32_PHYIOCRV1    0x00004305    // 0x00004301 for Static ODT
#define VAL_PCTL32_PUB_SLCR     0x00111100


#define VAL_OPCODE_MRS_MR2      0x00040080
#define VAL_OPCODE_MRS_MR3      0x00060000
#define VAL_OPCODE_MRS_MR1      0x00020440
#define VAL_OPCODE_MRS_MR0      0x00019400    // ODT = 40ohm
#endif // DDR_DEVICE == DDR3_NANYA_NT5CB64M16DPBE
/////////////////////////////////////////////////////////////////////////////////////////////////////

	
//////////////////////////////////////////////////////////////////////////////////////	
#if DDR_DEVICE == DDR3_SEC_K4B1G1646EHCF8
// Spreadsheet - SDRAM_H_DDR3_K4B1G1646EHCF8.xlsx

#define DdrDevice               "DDR3 - K4B1G1646E-HCF8"

// DDR Clock
#define VAL_CTL_PLL_DDR_FREQ    0x000023A6    // 261MHz

// Mixer
#define VAL_MIXER_DDR_PARAMETER_x32 0x45B1D029    // It was 0x60529829

// PCTL
#define VAL_PCTL32_MCFG         0x000A0021    // Open Page. Close page makes BW issue while Mixer is bypassed.

#define VAL_PCTL32_DQSECFG		0x00111122
#define VAL_PCTL32_ODTCFG1      0x06060100

#define VAL_PCTL32_TOGCNT1U     0x00000105
#define VAL_PCTL32_TINIT        0x000000C8
#define VAL_PCTL32_TRSTH        0x000001F4
#define VAL_PCTL32_TOGCNT100N   0x0000001B

#define VAL_PCTL32_TREFI        0x0000004E
#define VAL_PCTL32_TMRD         0x00000004
#define VAL_PCTL32_TRFC         0x0000003A
#define VAL_PCTL32_TRP          0x00000007
#define VAL_PCTL32_TRTW         0x00000007
#define VAL_PCTL32_TAL          0x00000000
#define VAL_PCTL32_TCL          0x00000007
#define VAL_PCTL32_TCWL         0x00000006
#define VAL_PCTL32_TRAS         0x00000014
#define VAL_PCTL32_TRC          0x0000001B
#define VAL_PCTL32_TRCD         0x00000007
#define VAL_PCTL32_TRRD         0x00000006
#define VAL_PCTL32_TRTP         0x00000004
#define VAL_PCTL32_TWR          0x00000008
#define VAL_PCTL32_TWTR         0x00000004
#define VAL_PCTL32_TEXSR        0x00000200
#define VAL_PCTL32_TXP          0x00000004
#define VAL_PCTL32_TXPDLL       0x0000000D
#define VAL_PCTL32_TCKE         0x00000003

#define VAL_PCTL32_TZQSCS       0x00000040
#define VAL_PCTL32_TZQSCSI      0x00000000
#define VAL_PCTL32_TDQS         0x00000000
#define VAL_PCTL32_TCKSRE       0x00000006
#define VAL_PCTL32_TCKSRX       0x00000006
#define VAL_PCTL32_TMOD         0x0000000C
#define VAL_PCTL32_TRSTL        0x00000064
#define VAL_PCTL32_TZQCL        0x00000200

#define VAL_PCTL32_PHYIOCRV1    0x00004305    // 0x00004301 for Static ODT
#define VAL_PCTL32_PUB_SLCR		0x00111100


#define VAL_OPCODE_MRS_MR2      0x00040080
#define VAL_OPCODE_MRS_MR3			0x00060000
#define VAL_OPCODE_MRS_MR1			0x00020440
#define VAL_OPCODE_MRS_MR0      0x00019340    // ODT = 40ohm
#endif // DDR_DEVICE == DDR3_SEC_K4B1G1646EBE
/////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////
#if DDR_DEVICE == DDR2_SEC_K4T51163QJBCF7
// Spreadsheet - SDRAM_H_DDR2_K4T51163QJBCF7.xlsx

#define DdrDevice               "DDR2 - K4T51163QJ-BCF7"

// DDR Clock
#define VAL_CTL_PLL_DDR_FREQ    0x000022C6    // 198MHz

// Mixer
#define VAL_MIXER_DDR_PARAMETER_x32 0x39219028    // It was 0x43321008

// PCTL
#define VAL_PCTL32_MCFG         0x00060001    // Open Page. Close page makes BW issue while Mixer is bypassed.

#define VAL_PCTL32_DQSECFG      0x00112122
#define VAL_PCTL32_ODTCFG1      0x06060606

#define VAL_PCTL32_TOGCNT1U    0x000000C6
#define VAL_PCTL32_TINIT    0x000000C8
#define VAL_PCTL32_TRSTH    0x00000000
#define VAL_PCTL32_TOGCNT100N    0x00000014

#define VAL_PCTL32_TREFI    0x0000004E
#define VAL_PCTL32_TMRD    0x00000002
#define VAL_PCTL32_TRFC    0x0000002A
#define VAL_PCTL32_TRP    0x00000006
#define VAL_PCTL32_TRTW    0x00000005
#define VAL_PCTL32_TAL    0x00000003
#define VAL_PCTL32_TCL    0x00000006
#define VAL_PCTL32_TCWL    0x00000005
#define VAL_PCTL32_TRAS    0x00000012
#define VAL_PCTL32_TRC    0x00000018
#define VAL_PCTL32_TRCD    0x00000006
#define VAL_PCTL32_TRRD    0x00000004
#define VAL_PCTL32_TRTP    0x00000003
#define VAL_PCTL32_TWR    0x00000006
#define VAL_PCTL32_TWTR    0x00000003
#define VAL_PCTL32_TEXSR    0x000000C8
#define VAL_PCTL32_TXP    0x00000002
#define VAL_PCTL32_TXPDLL    0x00000005
#define VAL_PCTL32_TCKE    0x00000003

#define VAL_PCTL32_PHYIOCRV1    0x00005405    // 0x00006501 for Static ODT
#define VAL_PCTL32_PUB_SLCR     0x00112100    // DDR doesn't work. Bit[7:4] and [1:0] must be kept as 0 to make it working.


#define VAL_OPCODE_MRS_MR2			0x00040000
#define VAL_OPCODE_MRS_MR3			0x00060000
#define VAL_OPCODE_MRS_MR1      0x000201E0    // ODT = 75ohm
#define VAL_OPCODE_MRS_MR0			0x0000B630
#endif	// DDR_DEVICE == DDR2_SEC_K4T51163QJBCF7
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
#if DDR_DEVICE == DDR3_SEC_K4B1G1646GBCH9
// Spreadsheet - SDRAM_H_DDR3_K4B1G1646GBCH9.xlsx

#define DdrDevice               "DDR3 - K4B1G1646G-BCH9"

// DDR Clock
#define VAL_CTL_PLL_DDR_FREQ    0x000023A6    // 261MHz

// Mixer
#define VAL_MIXER_DDR_PARAMETER_x32 0x60221029

// PCTL
#define VAL_PCTL32_MCFG         0x000A0021    // Open Page. Close page makes BW issue while Mixer is bypassed.

#define VAL_PCTL32_DQSECFG      0x00111122
#define VAL_PCTL32_ODTCFG1      0x06060200

#define VAL_PCTL32_TOGCNT1U     0x00000105
#define VAL_PCTL32_TINIT        0x000000C8
#define VAL_PCTL32_TRSTH        0x000001F4
#define VAL_PCTL32_TOGCNT100N   0x0000001B

#define VAL_PCTL32_TREFI        0x0000004E
#define VAL_PCTL32_TMRD         0x00000004
#define VAL_PCTL32_TRFC         0x0000003A
#define VAL_PCTL32_TRP          0x00000008
#define VAL_PCTL32_TRTW         0x00000007
#define VAL_PCTL32_TAL          0x00000000
#define VAL_PCTL32_TCL          0x00000008
#define VAL_PCTL32_TCWL         0x00000006
#define VAL_PCTL32_TRAS         0x00000013
#define VAL_PCTL32_TRC          0x0000001A
#define VAL_PCTL32_TRCD         0x00000008
#define VAL_PCTL32_TRRD         0x00000004
#define VAL_PCTL32_TRTP         0x00000004
#define VAL_PCTL32_TWR          0x00000008
#define VAL_PCTL32_TWTR         0x00000004
#define VAL_PCTL32_TEXSR        0x00000200
#define VAL_PCTL32_TXP          0x00000004
#define VAL_PCTL32_TXPDLL       0x0000000D
#define VAL_PCTL32_TCKE         0x00000003

#define VAL_PCTL32_TZQSCS       0x00000040
#define VAL_PCTL32_TZQSCSI      0x00000000
#define VAL_PCTL32_TDQS         0x00000000
#define VAL_PCTL32_TCKSRE       0x00000006
#define VAL_PCTL32_TCKSRX       0x00000006
#define VAL_PCTL32_TMOD         0x0000000C
#define VAL_PCTL32_TRSTL        0x00000064
#define VAL_PCTL32_TZQCL        0x00000200

#define VAL_PCTL32_PHYIOCRV1    0x00004305    // 0x00004301 for Static ODT
#define VAL_PCTL32_PUB_SLCR     0x00111100


#define VAL_OPCODE_MRS_MR2      0x00040080
#define VAL_OPCODE_MRS_MR3      0x00060000
#define VAL_OPCODE_MRS_MR1      0x00020440    // ODT_W = 40ohm / ODT_R = 40ohm
#define VAL_OPCODE_MRS_MR0      0x00019400
#endif // DDR_DEVICE == DDR3_SEC_K4B1G1646GBCH9
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
#if DDR_DEVICE == DDR3_WINBOND_W631GG6KB15
// Spreadsheet - SDRAM_H_DDR3_W631GG6KB15.xlsx

#define DdrDevice               "DDR3 - W631GG6KB-15"

// DDR Clock
#define VAL_CTL_PLL_DDR_FREQ    0x000023A6    // 261MHz

// Mixer
#define VAL_MIXER_DDR_PARAMETER_x32 0x60221029

// PCTL
#define VAL_PCTL32_MCFG         0x000A0021    // Open Page. Close page makes BW issue while Mixer is bypassed.

#define VAL_PCTL32_DQSECFG      0x00111122
#define VAL_PCTL32_ODTCFG1      0x06060200

#define VAL_PCTL32_TOGCNT1U     0x00000105
#define VAL_PCTL32_TINIT        0x000000C8
#define VAL_PCTL32_TRSTH        0x000001F4
#define VAL_PCTL32_TOGCNT100N   0x0000001B

#define VAL_PCTL32_TREFI        0x0000004E
#define VAL_PCTL32_TMRD         0x00000004
#define VAL_PCTL32_TRFC         0x0000003A
#define VAL_PCTL32_TRP          0x00000008
#define VAL_PCTL32_TRTW         0x00000007
#define VAL_PCTL32_TAL          0x00000000
#define VAL_PCTL32_TCL          0x00000008
#define VAL_PCTL32_TCWL         0x00000006
#define VAL_PCTL32_TRAS         0x00000013
#define VAL_PCTL32_TRC          0x0000001A
#define VAL_PCTL32_TRCD         0x00000008
#define VAL_PCTL32_TRRD         0x00000004
#define VAL_PCTL32_TRTP         0x00000004
#define VAL_PCTL32_TWR          0x00000008
#define VAL_PCTL32_TWTR         0x00000004
#define VAL_PCTL32_TEXSR        0x00000200
#define VAL_PCTL32_TXP          0x00000004
#define VAL_PCTL32_TXPDLL       0x0000000D
#define VAL_PCTL32_TCKE         0x00000003

#define VAL_PCTL32_TZQSCS       0x00000040
#define VAL_PCTL32_TZQSCSI      0x00000000
#define VAL_PCTL32_TDQS         0x00000000
#define VAL_PCTL32_TCKSRE       0x00000006
#define VAL_PCTL32_TCKSRX       0x00000006
#define VAL_PCTL32_TMOD         0x0000000C
#define VAL_PCTL32_TRSTL        0x00000064
#define VAL_PCTL32_TZQCL        0x00000200

#define VAL_PCTL32_PHYIOCRV1    0x00004305
#define VAL_PCTL32_PUB_SLCR     0x00111100


#define VAL_OPCODE_MRS_MR2      0x00040080
#define VAL_OPCODE_MRS_MR3      0x00060000
#define VAL_OPCODE_MRS_MR1      0x00020440    // ODT_W = 40ohm / ODT_R = 40ohm
#define VAL_OPCODE_MRS_MR0      0x00019400
#endif // DDR_DEVICE == DDR3_WINBOND_W631GG6KB15
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
#if DDR_DEVICE == DDR3_ESMT_M15F1G1664A8CBG
// Spreadsheet - SDRAM_H_DDR3_M15F1G1664A8CBG.xlsx

#define DdrDevice               "DDR3 - M15F1G1664A-8CBG"

// DDR Clock
#define VAL_CTL_PLL_DDR_FREQ    0x000023A6    // 261MHz

// Mixer
#define VAL_MIXER_DDR_PARAMETER_x32 0x4C21D029    // It was 0x60529829

// PCTL
#define VAL_PCTL32_MCFG         0x000A0021    // Open Page. Close page makes BW issue while Mixer is bypassed.

#define VAL_PCTL32_DQSECFG      0x00111122
#define VAL_PCTL32_ODTCFG1      0x06060200

#define VAL_PCTL32_TOGCNT1U     0x00000105
#define VAL_PCTL32_TINIT        0x000000C8
#define VAL_PCTL32_TRSTH        0x000001F4
#define VAL_PCTL32_TOGCNT100N   0x0000001B

#define VAL_PCTL32_TREFI        0x0000004E
#define VAL_PCTL32_TMRD         0x00000004
#define VAL_PCTL32_TRFC         0x0000003A
#define VAL_PCTL32_TRP          0x00000007
#define VAL_PCTL32_TRTW         0x00000007
#define VAL_PCTL32_TAL          0x00000000
#define VAL_PCTL32_TCL          0x00000008
#define VAL_PCTL32_TCWL         0x00000006
#define VAL_PCTL32_TRAS         0x00000013
#define VAL_PCTL32_TRC          0x0000001A
#define VAL_PCTL32_TRCD         0x00000007
#define VAL_PCTL32_TRRD         0x00000004
#define VAL_PCTL32_TRTP         0x00000004
#define VAL_PCTL32_TWR          0x00000008
#define VAL_PCTL32_TWTR         0x00000004
#define VAL_PCTL32_TEXSR        0x00000200
#define VAL_PCTL32_TXP          0x00000004
#define VAL_PCTL32_TXPDLL       0x0000000D
#define VAL_PCTL32_TCKE         0x00000003

#define VAL_PCTL32_TZQSCS       0x00000040
#define VAL_PCTL32_TZQSCSI      0x00000000
#define VAL_PCTL32_TDQS         0x00000000
#define VAL_PCTL32_TCKSRE       0x00000006
#define VAL_PCTL32_TCKSRX       0x00000006
#define VAL_PCTL32_TMOD         0x0000000C
#define VAL_PCTL32_TRSTL        0x00000064
#define VAL_PCTL32_TZQCL        0x00000200

#define VAL_PCTL32_PHYIOCRV1    0x00004305    // 0x00004301 for Static ODT
#define VAL_PCTL32_PUB_SLCR     0x00111100


#define VAL_OPCODE_MRS_MR2      0x00040080
#define VAL_OPCODE_MRS_MR3      0x00060000
#define VAL_OPCODE_MRS_MR1      0x00020440    // ODT = 40ohm
#define VAL_OPCODE_MRS_MR0      0x00019400
#endif // DDR_DEVICE == DDR3_ESMT_M15F1G1664ABE
///////////////////////////////////////////////////////////////////////////////////////////////////


#define NUMBYTES_TestConfig			8
#define NvramAddr_TestConfig		0x9F0	// 0x7F0 MonitorSerialNumberEnd
// Bit3: FW execution from
//        0 - DDR
//        1 - Flash
// Bit2: Enable delays fine tuning for RDQSD
//        0 - No fine tuning.
//        1 - Delays will be tuned base on the setting of NvramAddr_NumDelay_RDQSD.
// Bit1: Enable delays fine tuning for WDQD
//        0 - No fine tuning.
//        1 - Delays will be tuned base on the setting of NvramAddr_NumDelay_WDQD.
// Bit0: OCM Clock.
//        0 - 200MHz
//        1 - 100MHz
#define NvramAddr_LongHrErr			(NvramAddr_TestConfig + 1)	//0x7FF
#define NvramAddr_NumDelay_WDQD		(NvramAddr_LongHrErr + 1)
// Bit7: Sign bit.
//		  0 - Number of delays increased
//		  1 - Number of delays decreased
// Bit[6:0]: Number of delays
#define NvramAddr_NumDelay_RDQSD	(NvramAddr_NumDelay_WDQD + 1)
// Bit7: Sign bit.
//		  0 - Number of delays increased
//		  1 - Number of delays decreased
// Bit[6:0]: Number of delays

#define Size_NumDelays			26*4
#define NvramAddr_DelayCur		0xA00	// 0x800
#define NvramAddr_DelayMin		(NvramAddr_DelayCur + Size_NumDelays)
#define NvramAddr_DelayMax		(NvramAddr_DelayMin + Size_NumDelays)
#define NvramAddr_DelayMin_VT	(NvramAddr_DelayMax + Size_NumDelays)
#define NvramAddr_DelayMax_VT	(NvramAddr_DelayMin_VT + Size_NumDelays)

#define Size_NumZQs				5*10
#define NvramAddr_ZQs			(NvramAddr_DelayMax_VT + Size_NumDelays)

#if ENABLE_DDR_X16 == ENABLE
#define NUM_DATAx8	2
#else
#define NUM_DATAx8	4
#endif

#define SET_DLY			0
#define GET_DLY			1

#define DLY_WBD			0
#define DLY_DMWBD		1
#define DLY_DSWBD		2
#define DLY_DSOEBD		3
#define DLY_DQOEBD		4
#define DLY_WLD			10
#define DLY_WDQD		11

#define DLY_RBD			5
#define DLY_DMRBD		6
#define DLY_DSRBD		7
#define DLY_RDQSD		12
#define DLY_DQSGD		13

#define DLY_DbgOnly_1		240

#define DLY_QUALIFY			250
#define DLY_DataBits			252

typedef struct
{
   BYTE B_WBD[4][8];
   BYTE B_DMWBD[4];
   BYTE B_DSWBD[4];
   BYTE B_DSOEBD[4];
   BYTE B_DQOEBD[4];
   BYTE B_WLD[4];
   BYTE B_WDQD[4];

   BYTE B_RBD[4][8];
   BYTE B_DMRBD[4];
   BYTE B_DSRBD[4];
   BYTE B_RDQSD[4];
   BYTE B_DQSGD[4];
} LmiDelays;


#define PCTL_ReadReg(W_RegAddr) \
	   ((DWORD)(*(volatile WORD hostreg*)(W_RegAddr)) | ((DWORD)(*(volatile WORD hostreg*)(W_RegAddr+2)) << 16))

#define PCTL_WriteReg(W_RegAddr, DW_Data) \
   (*(volatile WORD hostreg*)(W_RegAddr) = (WORD)(DW_Data)); \
   (*(volatile WORD hostreg*)(W_RegAddr+2) = (WORD)((DW_Data) >> 16))

extern BYTE DDR_RW_Flag;
extern DWORD DW_DdrTmp0, DW_DdrTmp1, DW_DdrTmp2;

void InitMemIF(void);
void InitDdr(void);
void PctlRegProgramming(WORD W_Reg, DWORD DW_Val);

void DDR_RW_HANDLER(void);
void DdrTest_LongHours(DWORD DW_NumAccess);

void SetDdrPhyDly_Handler(BYTE B_DlyType, BYTE B_ByteID, BYTE B_BitID, BYTE B_DlyVal);
void LmiDbg(BYTE B_Type);

BYTE GetDdrPhyDly(BYTE B_DlyType, BYTE B_ByteID, BYTE B_BitID);
void SetDdrPhyDly(BYTE B_DlyType, BYTE B_ByteID, BYTE B_BitID, BYTE B_DlyVal);
BYTE AdjustBitDelayLine(BYTE B_RW, BYTE B_DlyType, BYTE B_ByteID, BYTE B_BitID, BYTE B_Delay);
void DisableVTUpdate(void);
void EnableVTUpdate(void);

#if (DDR_TEST_CODE == ENABLE)
//void SetDdrPhyDly(BYTE B_DlyType, BYTE B_ByteID, BYTE B_BitID, BYTE B_DlyVal);
void SetDataPattern(BYTE B_PatLut);
void DqsBdlTest(WORD B_WriteWidth, WORD W_WriteHeight, WORD W_DqsStartEnd, BYTE B_NumRetry, BYTE B_Opt);
void MemIFTest(BYTE B_DlyType, BYTE B_NumRetry, BYTE B_Opt, BYTE B_TestedBit);
void DdrTest_DataBit(BYTE B_TestedBit, BYTE B_Opt, BYTE B_NumAccess);
void DataWrite_4H4L(DWORD DW_DdrAddr);
void DataRead_64Bytes(DWORD DW_DdrAddr);
void TurnOnFsbTesOSD(void);
void TurnOffFsbTesOSD(void);
gmt_RET_STAT TestFsbReadWrite(BYTE B_Mode, DWORD DW_Addr, WORD B_WriteWidth, WORD W_WriteHeight, BYTE B_Option, BYTE B_TestedBit);
#endif

#endif
//*********************************  END  **************************************

