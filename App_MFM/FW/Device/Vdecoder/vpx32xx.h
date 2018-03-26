/*
	$Workfile:   vpx32xx.h  $
	$Revision: 1.3 $
	$Date: 2011/12/15 03:52:11 $
*/

#ifndef __VPX32XX_H__
#define __VPX32XX_H__

//*******************************************************************************
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
// MODULE:	VPC3230.H
//
// USAGE:	Header file for Video Decoder "VPC3230"
//
//*******************************************************************************

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************

//********************************************************************************************
/*           G L O B A L    F U N C T I O N    P R O T O T Y P E S  F O R   V P X 3 2 2 6 E */
//********************************************************************************************

void dev_vpx32xxFPWrite	(BYTE VidDecId, WORD addr, WORD wrdata);
WORD dev_vpx32xxFPRead	(BYTE VidDecId, WORD addr);
void dev_vpx32xxWordWrite	(BYTE VidDecId, BYTE addr, WORD wrdata);
void dev_vpx32xxByteWrite	(BYTE VidDecId, BYTE addr, BYTE wrdata);
WORD dev_vpx32xxWordRead(BYTE VidDecId, BYTE addr);
BYTE dev_vpx32xxByteRead(BYTE VidDecId, BYTE addr);
void dev_vpx32xxSetVideoInput(BYTE VidDecId,BYTE In_Type);
void dev_vpx32xxWriteFPRamRandom(WORD *Wp_AddrTable, WORD *Wp_DataTable);
void dev_vpx32xxWriteBufRandom(BYTE *Bp_AddrTable, BYTE *Bp_DataTable);
BYTE dev_vpx32xxGetVideoMode(BYTE VidDecId)  ;
BYTE dev_vpx32xxGetVideoInput(BYTE VidDecId);
BYTE dev_vpx32xxGetVideoStatus(BYTE VidDecId);
BYTE dev_vpx32xxGetVideoInterlaceInfo(BYTE VidDecId);
WORD dev_vpx32xxGetVideoLinesPerField(BYTE VidDecId);
BYTE dev_vpx32xxGetVideoSubMode(BYTE VidDecId);
void dev_vpx32xxConfigVideoMode(BYTE VidDecId,BYTE mode);
void dev_vpx32xxVideoMixerControl(BYTE VidDecId,BYTE OnOff);

void dev_vpx32xxEnableDisplay ();//enable pixel bus, clock and syncs. //ST
void dev_vpx32xxProgramDevice(BYTE B_Mode);//sailesh
void dev_vpx32xxSetVideoOutputFormat(BYTE,BYTE);

void VpxInit (void);


//******************************************************************************
//	G L O B A L    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

#if (NUM_DECODER == 1)						// Single decoder
#ifdef SAA7119_DEV_ID
#define vdd_ConfigVideoMode		dev_saa7119ConfigVideoMode
#define vdd_SetVideoInput	   	dev_saa7119SetVideoInput
#define vdd_GetVideoInput      	dev_saa7119GetVideoInput
#define vdd_GetVideoMode		dev_saa7119GetVideoMode
#define vdd_PowerUp            	dev_saa7119PowerUp
#define vdd_PowerDown			dev_saa7119PowerDown
#endif

#ifdef VPC3230_DEV_ID
#define vdd_ConfigVideoMode			dev_vpc323ConfigVideoMode
#define vdd_SetVideoOutputFormat    dev_vpc323SetVideoOutputFormat
#define vdd_SetVideoInput	   		dev_vpc323SetVideoInput
#define vdd_GetVideoInput       	dev_vpc323GetVideoInput
#define vdd_GetVideoMode			dev_vpc323GetVideoMode
#define vdd_PowerUp             	dev_vpc323PowerUp
#define vdd_PowerDown				dev_vpc323PowerDown
#endif

//Sailesh
#ifdef VPX322xF_DEV_ID
#define vdd_ConfigVideoMode			dev_vpx32xxConfigVideoMode
#define vdd_SetVideoOutputFormat    dev_vpx32xxSetVideoOutputFormat
#define vdd_SetVideoInput	   		dev_vpx32xxSetVideoInput
#define vdd_GetVideoInput       	dev_vpx32xxGetVideoInput
#define vdd_GetVideoMode			dev_vpx32xxGetVideoMode
#define vdd_PowerUp             	dev_vpx32xxPowerUp
#define vdd_PowerDown				dev_vpx32xxPowerDown
#define vdd_ConfigureVbiSlicer      dev_vpx32xxConfigureVbiSlicer
#endif
#endif

//******************************************************************************
// Registers for the VPX3226E decoder
//******************************************************************************
#define  VPX_I2C_JEDEC  	      0x00
#define  VPX_I2C_PARTNUM_hi	      0x01
#define  VPX_I2C_PARTNUM_lo 	  0x02
#define  VPX_I2C_JEDEC_2	      0x03
#define  VPX_I2C_llc              0xAA
#define  VPX_I2C_bstatus	      0xAB
#define  VPX_I2C_softerrcnt       0xB3
#define  VPX_I2C_sync_stat	      0xB4
#define  VPX_I2C_sync_cnt	      0xB5
#define  VPX_I2C_coeff_rd	      0xB6
#define  VPX_I2C_level_rd	      0xB7
#define  VPX_I2C_mask_hi	      0xB8
#define  VPX_I2C_mask_mid	      0xB9
#define  VPX_I2C_mask_lo	      0xBA
#define  VPX_I2C_reference_hi	  0xBB
#define  VPX_I2C_reference_mid    0xBC
#define  VPX_I2C_reference_lo	  0xBD
#define  VPX_I2C_soft_sliser      0xC0
#define  VPX_I2C_ttx_freqLSB      0xC1
#define  VPX_I2C_ttx_freqMSB	  0xC2
#define  VPX_I2C_coeff            0xC5
#define  VPX_I2C_data_sliser	  0xC6
#define  VPX_I2C_accu		      0xC7
#define  VPX_I2C_sync_sliser	  0xC8
#define  VPX_I2C_standard	      0xC9
#define  VPX_I2C_tolerance        0xCE
#define  VPX_I2C_byte_cnt	      0xCF
#define  VPX_I2C_oena             0xF2
#define  VPX_I2C_driver_a         0xF8
#define  VPX_I2C_driver_b	      0xF9

/****************************************************************************/
/*               VPX3226E I2C FP Interface register definitions             */
/****************************************************************************/

#define  VPX_I2C_FPSTA            0x35
#define  VPX_I2C_FPRD		      0x36
#define  VPX_I2C_FPWR		      0x37
#define  VPX_I2C_FPDAT		      0x38

//sailesh
#define VPX_STATUS_TWC_REG                      0x35
#define VPX_FPREAD_ADDR_TWC_REG                 0x36
#define VPX_FPWRITE_ADDR_TWC_REG                0x37
#define VPX_FPDATA_TWC_REG                      0x38

/****************************************************************************/
/*                VPX3226E FP-RAM Interface register definitions (12 bits)  */
/****************************************************************************/

#define    VPX_FP_gp_ctrl         0x012
#define    VPX_FP_asr		      0x013
#define    VPX_FP_vcnt            0x015
#define    VPX_FP_sdt             0x020
#define    VPX_FP_insel           0x021
#define    VPX_FP_sfif		      0x022
#define    VPX_FP_idly		      0x023
#define    VPX_FP_comb_uc         0x028
#define    VPX_FP_acc_sat         0x030
#define    VPX_FP_bampl           0x036
#define    VPX_FP_kilvl           0x039
#define    VPX_FP_kilhy           0x03A
#define    VPX_FP_sampl           0x074
#define    VPX_FP_autolock        0x0B5
#define    VPX_FP_nlpf            0x0CB
#define    VPX_FP_tint            0x0DC
#define    VPX_FP_version         0x0F0
#define    VPX_FP_xlck            0x0F7
#define    VPX_FP_dvco            0x0F8
#define    VPX_FP_adjust          0x0F9
#define    VPX_FP_vact_dly1       0x10F
#define    VPX_FP_vact_dly2       0x11F
#define    VPX_FP_vbegin1         0x120
#define    VPX_FP_vlinesin1       0x121
#define    VPX_FP_vlinesout1	  0x122
#define    VPX_FP_hbeg1		      0x123
#define    VPX_FP_hlen1		      0x124
#define    VPX_FP_npix1		      0x125
#define    VPX_FP_peaking1	      0x126
#define    VPX_FP_brightness1	  0x127
#define    VPX_FP_contrast1	      0x128
#define    VPX_FP_vbegin2         0x12A
#define    VPX_FP_vlinesin2       0x12B
#define    VPX_FP_vlinesout2      0x12C
#define    VPX_FP_hbeg2		      0x12D
#define    VPX_FP_hlen2		      0x12E
#define    VPX_FP_npix2    	      0x12F
#define    VPX_FP_peaking2        0x130
#define    VPX_FP_brightness2	  0x131
#define    VPX_FP_contrast2	      0x132
#define    VPX_FP_start_even      0x134
#define    VPX_FP_end_even 	      0x135
#define    VPX_FP_start_odd	      0x136
#define    VPX_FP_end_odd	      0x137
#define    VPX_FP_vbicontrol	  0x138
#define    VPX_FP_slsize	      0x139
#define    VPX_FP_ControlWord     0x140
#define    VPX_FP_InfoWord	      0x141
#define    VPX_FP_format_sel      0x150
#define    VPX_FP_pval_start      0x151
#define    VPX_FP_pval_stop	      0x152
#define    VPX_FP_refsig	      0x153
#define    VPX_FP_outmux	      0x154
#define    VPX_FP_tdecframes      0x157
#define    VPX_FP_asr_enable      0x158
#define    VPX_FP_asr_status      0x15e
#define    VPX_FP_mcv_status      0x170
#define    VPX_FP_mcv_start       0x171
#define    VPX_FP_mcv_stop	      0x172


#endif
//*********************************  END  **************************************
