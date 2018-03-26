/*
	$Workfile:   vpc_regs.h  $
	$Revision: 1.3 $
	$Date: 2011/12/15 03:52:11 $
*/

#ifndef _VPC_REGS_H_
#define _VPC_REGS_H_

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
// MODULE:     	viddec.h
//
// USAGE:  	Header file for Micronas video decoder register names
//
//******************************************************************************

//Micronas VPC323x Registers

//FP Interface Registers
#define VPC_STATUS_TWC_REG                      0x35
#define VPC_FPREAD_ADDR_TWC_REG                 0x36
#define VPC_FPWRITE_ADDR_TWC_REG                0x37
#define VPC_FPDATA_TWC_REG                      0x38

//Black Line Detector Registers
#define VPC_BLK_LINE_DET_TWC_REG                0x12

//Pin Circuit Registers
#define VPC_PIN_CTRL_TWC_REG                    0x1F
#define VPC_SYNCGEN_CTRL_TWC_REG                0x20
#define VPC_OP_STENGTH_TWC_REG                  0x23
#define VPC_VSYNC_DLY_TWC_REG                   0x30

//656 Interface Registers
#define VPC_OUT_INTERFACE_TWC_REG               0x24

//Sync Generator Registers
#define VPC_LINE_LEN_TWC_REG                    0x21
#define VPC_HC_START_TWC_REG                    0x26
#define VPC_HC_STOP_TWC_REG                     0x27
#define VPC_AVO_START_TWC_REG                   0x28
#define VPC_AVO_STOP_TWC_REG                    0x29
#define VPC_NEWLINE_TWC_REG                     0x22

//PIP Control Registers
#define VPC_VPCMODE_TWC_REG                     0x84
#define VPC_PIPMODE_TWC_REG                     0x85
#define VPC_PIPOPER_TWC_REG                     0x83
#define VPC_COLBGD_TWC_REG                      0x80
#define VPC_COLFR1_TWC_REG                      0x81
#define VPC_COLFR2_TWC_REG                      0x82
#define VPC_LINOFFS_TWC_REG                     0x86
#define VPC_PIXOFFS_TWC_REG                     0x89
#define VPC_VSTR_TWC_REG                        0x87
#define VPC_HSTR_TWC_REG                        0x8A
#define VPC_NLIN_TWC_REG                        0x88
#define VPC_NPIX_TWC_REG                        0x8B
#define VPC_NPFB_TWC_REG                        0x8C

//CIP Control Registers
#define VPC_CIPSAT_TWC_REG                      0x90
#define VPC_CIPTNT_TWC_REG                      0x91
#define VPC_CIPBRCT_TWC_REG                     0x92
#define VPC_CIPMIX1_TWC_REG                     0x94
#define VPC_CIPMIX2_TWC_REG                     0x95
#define VPC_CIPCNTL_TWC_REG                     0x96
#define VPC_CIPMON_TWC_REG                      0x97

//Hardware ID
#define VPC_HW_VER_NUM_TWC_REG                  0x9F

//**********************************
//Fast Processor Registers
//**********************************
//Standard Selection
#define VPC_SDT_FP_REG                          0x20
#define VPC_ASR_ENABLE_FP_REG                   0x148
#define VPC_ASR_STATUS_FP_REG                   0x14E
#define VPC_INSEL_FP_REG                        0x21
#define VPC_SFIF_FP_REG                         0x22
#define VPC_LDLY_FP_REG                         0x23
#define VPC_HLP_DLY_FP_REG                      0x29
#define VPC_CIP_MATCH_FP_REG                    0x27
#define VPC_VGA_C_FP_REG                        0x2F

//Comb Filter Registers
#define VPC_COMB_UC_FP_REG                      0x28

//Color Processing Registers
#define VPC_ACC_SAT_FP_REG                      0x30
#define VPC_CR_ATT_FP_REG                       0x17A
#define VPC_ACCH_FP_REG                         0x17D
#define VPC_KILVL_FP_REG                        0x39
#define VPC_KILHY_FP_REG                        0x3A
#define VPC_HLPDIS_FP_REG                       0x16C
#define VPC_TINT_FP_REG                         0xDC

//DVCO Registers
#define VPC_DVCO_FP_REG                         0xF8
#define VPC_ADJUST_FP_REG                       0xF9
#define VPC_XCLK_FP_REG                         0xF7
#define VPC_AUTOCLK_FP_REG                      0xB5

//FP Status Registers
#define VPC_GEN_PURPOSE_FP_REG                  0x12
#define VPC_ASR_FP_REG                          0x13
#define VPC_NOISE_FP_REG                        0x14
#define VPC_NLPF_FP_REG                         0xCB
#define VPC_VCNT_FP_REG                         0x15
#define VPC_SAMPL_FP_REG                        0x74
#define VPC_BAMPL_FP_REG                        0x36
#define VPC_FW_VER_FP_REG                       0xF0
#define VPC_MCV_STATUS_FP_REG                   0x170
#define VPC_SCMODE_FP_REG                       0x40
#define VPC_SCPIP_FP_REG                        0x41
#define VPC_FFLIM_FP_REG                        0x42
#define VPC_SCINC1_FP_REG                       0x43
#define VPC_SCINC2_FP_REG                       0x44
#define VPC_SCINC_FP_REG                        0x45
#define VPC_SCW1_0_FP_REG                       0x47
#define VPC_SCW1_1_FP_REG                       0x48
#define VPC_SCW1_2_FP_REG                       0x49
#define VPC_SCW1_3_FP_REG                       0x4A
#define VPC_SCW1_4_FP_REG                       0x4B
#define VPC_SCW2_0_FP_REG                       0x4C
#define VPC_SCW2_1_FP_REG                       0x4D
#define VPC_SCW2_2_FP_REG                       0x4E
#define VPC_SCW2_3_FP_REG                       0x4F
#define VPC_SCW2_4_FP_REG                       0x50
#define VPC_SCBRI_FP_REG                        0x52
#define VPC_SCCT_FP_REG                         0x53

//LLC Control Registers
#define VPC_LLC_START_FP_REG                    0x65
#define VPC_LLC_STOP_FP_REG                     0x66
#define VPC_LLC_CLOCKH_FP_REG                   0x69
#define VPC_LLC_CLOCKL_FP_REG                   0x6A
#define VPC_LLC_DFLIMIT_FP_REG                  0x61
#define VPC_LLC_CLKC_FP_REG                     0x6D

#endif //_VPC_REGS_H_
