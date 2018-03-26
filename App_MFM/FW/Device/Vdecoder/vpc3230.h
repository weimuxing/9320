/*
	$Workfile:   vpc3230.h  $
	$Revision: 1.3 $
	$Date: 2011/12/15 03:52:11 $
*/

#ifndef __VPC3230_H__
#define __VPC3230_H__

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



// Register definitions
#define VPC323_FP_READ_ADDR             (0x36)
#define VPC323_FP_WRITE_ADDR            (0x37)
#define VPC323_FP_DATA_ADDR             (0x38)
#define VPC323_FP_RW_DATA_ADDR          0x38

#define VPC323_SYNC_PIN_CTRL            (0x1F)
#define VPC323_FP_SCALER                (0x0040)
#define VPC323_FP_STANDARD_SELECT       (0x0020)
#define VPC323_FP_INPUT_SELECT          (0x0021)
#define VPC323_FP_COMBFILTERCONTROL     (0x0028)
#define VPC323_FP_ACTIVEVIDEOLENGTH     (0x0042)
#define VPC323_FP_SCALAR1COEFF          (0x0043)
#define VPC323_FP_LUMACHROMADELAY       (0x0023)
#define VPC323_SOFTMIXER2               (0x95)
#define VPC323_CIP_CNTL                 (0x96)

#define VPC323_FP_ASR_ENABLE            (0x0148)
#define VPC323_FP_ASR_STATUS            (0x014E)
#define VPC323_FP_SR_STATUS             (0x0013)
#define VPC323_FP_NLPF                  (0x00CB)

#define VPC323_FP_BRIGHTNESS            (0x0052)
#define VPC323_FP_CONTRAST              (0x0053)
#define VPC323_FP_SATURATION            (0x0030)
#define VPC323_FP_NTSC_TINT             (0x00DC)

//******************************************************************************
//	G L O B A L    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

void dev_vpc323SetVideoInput		(BYTE VidDecId,BYTE InType);
BYTE dev_vpc323GetVideoInput		(BYTE VidDecId);
void dev_vpc323SetVideoOutputFormat	(BYTE VidDecId,BYTE OutType);
BYTE dev_vpc323GetVideoOutputFormat	(BYTE VidDecId);
BYTE dev_vpc323GetVideoStatus		(BYTE VidDecId);
BYTE dev_vpc323GetVideoInterlaceInfo(BYTE VidDecId);
WORD dev_vpc323GetVideoLinesPerField(BYTE VidDecId);
BYTE dev_vpc323GetVideoMode			(BYTE VidDecId);
BYTE dev_vpc323GetVideoSubMode		(BYTE VidDecId);
void dev_vpc323ConfigVideoMode		(BYTE VidDecId,BYTE mode);
void dev_vpc323VideoMixerControl	(BYTE VidDecId,BYTE OnOff);
void dev_vpc323SetVideoBrightness	(BYTE VidDecId,BYTE bval);
BYTE dev_vpc323GetVideoBrightness	(BYTE VidDecId);
void dev_vpc323SetVideoContrast		(BYTE VidDecId,BYTE cval);
BYTE dev_vpc323GetVideoContrast		(BYTE VidDecId);
void dev_vpc323SetVideoSaturation	(BYTE VidDecId,BYTE sval);
BYTE dev_vpc323GetVideoSaturation	(BYTE VidDecId);
void dev_vpc323SetVideoHue			(BYTE VidDecId,BYTE hval);
BYTE dev_vpc323GetVideoHue			(BYTE VidDecId);
void dev_vpc323SetVideoSharpness	(BYTE VidDecId,BYTE sharpval);
BYTE dev_vpc323GetVideoSharpness	(BYTE VidDecId);
void dev_vpc323PatGenControl		(BYTE VidDecId,BYTE OnOff);
void dev_vpc323PowerUp				(BYTE VidDecId);
void dev_vpc323PowerDown			(BYTE VidDecId);

#endif
//*********************************  END  **************************************
