/*
   $Workfile:   tvp5160.h  $
   $Revision: 1.3 $
   $Date: 2011/12/15 03:52:11 $
*/
#ifndef __TVP5160_H__
#define __TVP5160_H__


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
// MODULE:       TVP5160.h
//
// USAGE:      Header file for TI "TVP5160" video decoder
//
//******************************************************************************


//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************
#ifdef EXTERNAL_DECODER


WORD Decoder_GetStatusFromLPM(VideoDriverFuncType Decoder_Setting,BYTE B_DecoderValue);
void Decoder_SendCMDToLPM(VideoDriverFuncType Decoder_Setting, BYTE B_DecoderValue1, BYTE B_DecoderValue2);
void  Mission_TVP5160ConfigVideoMode(BYTE B_Mode);
void  Mission_TVP5160SetVideoInput(gmt_PHY_CH B_Channel ,BYTE B_InType);
BYTE  Mission_TVP5160GetVideoMode(void);
void  Mission_TVP5160PowerUp_(void);
void  Mission_TVP5160PowerDown(void);
WORD  Mission_TVP5160GetChipID(void);
void Mission_TVP5160WriteReg(WORD W_RegAddr, BYTE B_Data);
WORD Mission_TVP5160ReadReg(WORD W_RegAddr);
void Mission_TVP5160_InitDecoder(void);
WORD Mission_TVP5160_GetVideoStatus(gmt_PHY_CH B_Channel);
BOOL Mission_TVP5160_CheckOutput(void);
//#else


#define TVP5160_DEV_ID_MIS          			0xB8

//*************************************************
// Register definitions
#define	INPUT_SELECT_ADDR								0x00
#define	AFE_GAIN_CONTROL_ADDR						0x01
#define	VIDEO_STANDARD_ADDR							0x02
#define	OPERATION_MODE_ADDR							0x03
#define	AUTO_SWITCH_MASK_ADDR						0x04
#define	COLOR_KILLER_ADDR								0x05
#define	LUMINANCE_PROCESSING_CONTROL1_ADDR		0x06
#define	LUMINANCE_PROCESSING_CONTROL2_ADDR		0x07
#define	LUMINANCE_PROCESSING_CONTROL3_ADDR		0x08
#define	LUMINANCE_BRIGHTNESS_ADDR					0x09
#define	LUMINANCE_CONTRAST_ADDR						0x0A
#define	CHROMINANCE_SATURATION_ADDR				0x0B
#define	CHROMA_HUE_ADDR								0x0C
#define	CHROMINANCE_PROCESSING_CONTROL1_ADDR	0x0D
#define	CHROMINANCE_PROCESSING_CONTROL2_ADDR	0x0E
#define	COMPONENT_PR_SATURATION_ADDR				0x10
#define	COMPONENT_Y_CONTRAST_ADDR					0x11
#define	COMPONENT_PB_SATURATION_ADDR				0x12
#define	COMPONENT_Y_BRIGHTNESS_ADDR				0x14
#define	AVID_START_PIXEL_LOW_ADDR					0x16
#define	AVID_START_PIXEL_HIGH_ADDR					0x17
#define	AVID_STOP_PIXEL_LOW_ADDR					0x18
#define	AVID_STOP_PIXEL_HIGH_ADDR					0x19
#define	HSYNC_START_PIXEL_LOW_ADDR					0x1A
#define	HSYNC_START_PIXEL_HIGH_ADDR				0x1B
#define	HSYNC_STOP_PIXEL_LOW_ADDR					0x1C
#define	HSYNC_STOP_PIXEL_HIGH_ADDR					0x1D
#define	VSYNC_START_LINE_LOW_ADDR					0x1E
#define	VSYNC_START_LINE_HIGH_ADDR					0x1F
#define	VSYNC_STOP_LINE_LOW_ADDR					0x20
#define	VSYNC_STOP_LINE_HIGH_ADDR					0x21
#define	VBLK_START_LINE_LOW_ADDR					0x22
#define	VBLK_START_LINE_HIGH_ADDR					0x23
#define	VBLK_STOP_LINE_LOW_ADDR						0x24
#define	VBLK_STOP_LINE_HIGH_ADDR					0x25

#define	EMBEDDED_SYNC_OFFSET_CONTROL1_ADDR		0x26
#define	EMBEDDED_SYNC_OFFSET_CONTROL2_ADDR		0x27

#define	FAST_SWITCH_CONTROL_ADDR					0x28
#define	FAST_SWITCH_OVERLAY_DELAY_ADDR			0x29
#define	FAST_SWITCH_SCART_DELAY_ADDR				0x2A
#define	OVERLAY_DELAY_ADDR							0x2B
#define	SCART_DELAY_ADDR								0x2C
//#define	CTI_DELAY_ADDR									0x2D
#define	CTI_CONTROL_ADDR								0x2E
#define  COMPONENT_AUTOSWITCH_MASK_ADDR			0x30

#define	SYNC_CONTROL_ADDR								0x32
#define	OUTPUT_FORMATTER1_ADDR						0x33
#define	OUTPUT_FORMATTER2_ADDR						0x34
#define	OUTPUT_FORMATTER3_ADDR						0x35
#define	OUTPUT_FORMATTER4_ADDR						0x36
#define	OUTPUT_FORMATTER5_ADDR						0x37
#define	OUTPUT_FORMATTER6_ADDR						0x38
#define	CLEAR_LOST_LOCK_DETECT_ADDR				0x39

#define	STATUS1_ADDR									0x3A
#define	STATUS2_ADDR									0x3B

#define	AGC_GAIN_STATUS_LOW_ADDR					0x3C
#define	AGC_GAIN_STATUS_HIGH_ADDR					0x3D
//#define	SCH_PHASE_STATUS_ADDR						0x3E
#define	VIDEO_STANDARD_STATUS_ADDR					0x3F
#define	GPIO_INPUT1_ADDR								0x40
#define	GPIO_INPUT2_ADDR								0x41
#define  BACK_END_AGC_STATUS_ADDR					0x44

#define	AFE_COARSE_GAIN_FOR_CH1_ADDR				0x46
#define	AFE_COARSE_GAIN_FOR_CH2_ADDR				0x47
#define	AFE_COARSE_GAIN_FOR_CH3_ADDR				0x48
#define	AFE_COARSE_GAIN_FOR_CH4_ADDR				0x49
#define	AFE_FINE_GAIN_FOR_PB_B_LOW_ADDR			0x4A
#define	AFE_FINE_GAIN_FOR_PB_B_HIGH_ADDR			0x4B
#define	AFE_FINE_GAIN_FOR_Y_G_CHROMA_LOW_ADDR	0x4C
#define	AFE_FINE_GAIN_FOR_Y_G_CHROMA_HIGH_ADDR	0x4D
#define	AFE_FINE_GAIN_FOR_PR_R_LOW_ADDR			0x4E
#define	AFE_FINE_GAIN_FOR_PR_R_HIGH_ADDR			0x4F
#define	AFE_FINE_GAIN_FOR_CVBS_LUMA_LOW_ADDR	0x50
#define	AFE_FINE_GAIN_FOR_CVBS_LUMA_HIGH_ADDR	0x51

#define  TVP_656_VERSION_ADDR								0x57
#define  SDRAM_CONTROL_ADDR							0x59
#define	Y_NOISE_SENSITIVITY_ADDR					0x5A
#define	UV_NOISE_SENSITIVITY_ADDR					0x5B
#define	Y_CORING_THRESHOLD_ADDR						0x5C
#define	UV_CORING_THRESHOLD_ADDR					0x5D
#define	LOW_NOISE_LIMIT_ADDR							0x5E
#define  BLUE_SCREEN_Y_ADDR							0x5F
#define  BLUE_SCREEN_CB_ADDR							0x60
#define  BLUE_SCREEN_CR_ADDR							0x61
#define  BLUE_SCREEN_LSB_ADDR							0x62
#define  TVP_3DNR_NOISE_MEASUREMENT_LSB_ADDR	  		0x64
#define  TVP_3DNR_NOISE_MEASUREMENT_MSB_ADDR	  		0x65
#define  Y_CORE0_ADDR									0x66
#define  UV_CORE0_ADDR									0x67
#define  F_AND_V_BIT_DECODE_CONTROL_ADDR	  		0x69
#define  BACK_END_AGC_CONTROL_ADDR					0x6C
#define  AGC_DECREMENT_SPEED_ADDR					0x6F

#define	ROM_VERSION_ADDR								0x70
//#define	RAM_VERSION_ADDR								0x71
//#define	AGC_SYNC_HEIGHT_ADDR							0x72
#define	AGC_WHITE_PEAK_PROCESSING_ADDR			0x74
#define  F_AND_V_BIT_CONTROL_ADDR	  				0x75

#define	AGC_INCREMENT_SPEED_ADDR					0x78
#define	AGC_INCREMENT_DELAY_ADDR					0x79
#define  ANALOG_OUTPUT_CONTROL1_ADDR				0x7F

#define  CHIP_ID_MSB_ADDR								0x80
#define  CHIP_ID_LSB_ADDR								0x81

#define  COLOR_PLL_SPEED_CONTROL_ADDR				0x83
#define  TVP_3DYC_LUMA_CORING_LSB_ADDR					0x84
#define  TVP_3DYC_CHROMA_CORING_LSB_ADDR 				0x85
#define  TVP_3DYC_CHROMA_LUMA_MSB_ADDR 					0x86
#define  TVP_3DYC_LUMA_GAIN_ADDR 							0x87
#define  TVP_3DYC_CHROMA_GAIN_ADDR 						0x88
#define  TVP_3DYC_SIGNAL_QUALITY_GAIN_ADDR 			0x89
#define  TVP_3DYC_SIGNAL_QUALITY_CORING_LOW_ADDR 	0x8A
#define  TVP_3DYC_SIGNAL_QUALITY_CORING_HIGH_ADDR 	0x8B
#define  IF_COMPENSATION_CONTROL_ADDR    			0x8D
#define  IF_DIFFERENTIAL_GAIN_CONTROL_ADDR		0x8E
#define  IF_LOW_FREQUENCY_GAIN_CONTROL_ADDR		0x8F
#define  IF_HIGH_FREQUENCY_GAIN_CONTROL_ADDR		0x90
#define  WEAK_SIGNAL_HIGH_THRESHOLD_ADDR			0x95
#define  WEAK_SIGNAL_LOW_THRESHOLD_ADDR			0x96
#define  STATUS_REQUEST_ADDR							0x97
#define  TVP_3DYC_NTSC_VCR_THRESHOLD_ADDR				0x98
#define  TVP_3DYC_PAL_VCR_THRESHOLD_ADDR				0x99
#define  VERTICAL_LINE_COUNT_LOW_ADDR				0x9A
#define  VERTICAL_LINE_COUNT_HIGH_ADDR				0x9B
#define  AGC_DECREMENT_DELAY_ADDR					0x9E


#define	VDP_TTX_FILTER1_MASK1_ADDR					0xB1
#define	VDP_TTX_FILTER1_MASK2_ADDR					0xB2
#define	VDP_TTX_FILTER1_MASK3_ADDR					0xB3
#define	VDP_TTX_FILTER1_MASK4_ADDR					0xB4
#define	VDP_TTX_FILTER1_MASK5_ADDR					0xB5
#define	VDP_TTX_FILTER2_MASK1_ADDR					0xB6
#define	VDP_TTX_FILTER2_MASK2_ADDR					0xB7
#define	VDP_TTX_FILTER2_MASK3_ADDR					0xB8
#define	VDP_TTX_FILTER2_MASK4_ADDR					0xB9
#define	VDP_TTX_FILTER2_MASK5_ADDR					0xBA
#define	VDP_TTX_FILTER_CONTROL_ADDR				0xBB
#define	VDP_FIFO_WORD_COUNT_ADDR					0xBC
#define	VDP_FIFO_INTERRUPT_THRESHOLD_ADDR		0xBD
#define	VDP_FIFO_RESET_ADDR							0xBF
#define	VDP_FIFO_OUTPUT_CONTROL_ADDR				0xC0
#define	VDP_LINE_NUMBER_INTERRUPT_ADDR			0xC1
#define	VDP_PIXEL_ALIGNMENT_LOW_ADDR				0xC2
#define	VDP_PIXEL_ALIGNMENT_HIGH_ADDR				0xC3
#define	VDP_LINE_START_ADDR							0xD6
#define	VDP_LINE_STOP_ADDR							0xD7
#define	VDP_GLOBAL_LINE_MODE_ADDR					0xD8
#define	VDP_FULL_FIELD_ENABLE_ADDR					0xD9
#define	VDP_FULL_FIELD_MODE_ADDR					0xDA
#define  INTERLACED_PROGRESSIVE_STATUS_ADDR		0xDB

#define	VBUS_DATA_WITHOUT_ADDRESS_INCREMENT_ADDR	0xE0
#define	VBUS_DATA_WITH_ADDRESS_INCREMENT_ADDR	0xE1
#define	VDP_FIFO_ADDR									0xE2
#define	VBUS_ADDRESS_LOW_ADDR						0xE8
#define	VBUS_ADDRESS_MID_ADDR						0xE9
#define	VBUS_ADDRESS_HIGH_ADDR						0xEA
#define	INTERRUPT_RAW_STATUS0_ADDR					0xF0
#define	INTERRUPT_RAW_STATUS1_ADDR					0xF1
#define	INTERRUPT_STATUS0_ADDR						0xF2
#define	INTERRUPT_STATUS1_ADDR						0xF3
#define	INTERRUPT_MASK0_ADDR							0xF4
#define	INTERRUPT_MASK1_ADDR							0xF5
#define	INTERRUPT_CLEAR0_ADDR						0xF6
#define	INTERRUPT_CLEAR1_ADDR						0xF7

//Register bits definition
//#define	LUMINANCE_PROCESSING_CONTROL2_ADDR		0x07
#define  LUMA_COMB_TRAP_FILTER_BYPASSED			0xC0
#define  PEAKING_SIGNAL_GAIN_MASK					0x0C


//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************
//WORD LPM_TVP5160_VideoDecoderEntry(BYTE func, void *p1, void *p2);
WORD MIS_TVP5160_VideoDecoderEntry(BYTE func, BYTE p1, BYTE p2);
gmt_RET_STAT MIS_TVP5160CheckDevice(void);
BYTE MIS_TVP5160ReadByte(WORD W_RegAddr);
gmt_RET_STAT MIS_TVP5160WriteByte(WORD W_RegAddr, BYTE B_Data);
BYTE  MIS_TVP5160_IsSignalPresent(gmt_PHY_CH B_Channel);
BYTE  MIS_TVP5160GetVideoStatus(gmt_PHY_CH B_Channel);
BYTE  MIS_TVP5160GetVideoMode(void);
void MIS_TVP5160_InitDecoder(void);
WORD MIS_TVP5160_GetChipID(void);
void MIS_TVP5160_ChangeToPort(gmt_PHY_CH B_Channel, BYTE B_Port);
BYTE MIS_TVP5160_GetVideoMode(gmt_PHY_CH B_Channel);
BOOL MIS_TVP5160_CheckOutput(void);
void MIS_TVP5160_AnalogOutGainWrite(BYTE GainLevel);
BYTE MIS_TVP5160_AnalogOutGainRead(void);


//#endif
#endif


#endif
//*********************************  END  **************************************

