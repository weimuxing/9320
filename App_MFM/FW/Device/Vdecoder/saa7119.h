
#ifndef __SAA7119_H__
#define __SAA7119_H__
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
// MODULE: 		saa7119.h
//
// USAGE:		Header file for Philips "saa7119" video decoder
//
//******************************************************************************

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************

typedef enum//fo 3230, 7119 not need
{
   FORCE_RGB_TO_CIP,
   FORCE_YUV_TO_CIP,
   NORMAL_MIX_MODE
} gmt_VDD_MIXER_SETTING;

extern const BYTE gmc_videoDecoderOutputFormat[NUM_DECODER];

#define SAA7119_DEV_ID_B			0x48 //7119 need 2 slave address

#define SAA7119A_AVOUT_CHANNEL1      1
#define SAA7119A_AVOUT_CHANNEL2      2
#define SAA7119A_AVOUT_CHANNEL3      3
#define SAA7119A_AVOUT_CHANNEL4      4

// Register Address of SA7119
#define SAA7119_AIN1_ADDR       0x02
#define SAA7119_BRT_ADDR        0x0A
#define SAA7119_CON_ADDR        0x0B
#define SAA7119_SAT_ADDR        0x0C
#define SAA7119_HUE_ADDR        0x0D
#define SAA7119_DEC_STAT_ADDR   0x1E

/* Front-End ADC Control-Register */
#define SAA7119_INPUT0      0x01
#define SAA7119_INPUT1      0x02  //Bit0~5 Selected Input Mode
#define SAA7119_INPUT2      0x03
#define SAA7119_DGA1        0x04
#define SAA7119_DGA2        0x05
#define SAA7119_DGA3        0x22
#define SAA7119_DGA4        0x23
#define SAA7119_EXMCE       0x21
#define SAA7119_AOSL        0x14
/* End */
#define SAA7119_LUMINANCE   0x09
#define SAA7119_OVERLAY     0x27
#define SAA7119_OSDOVERLAY  0x77

//******************************************************************************
//	G L O B A L    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

void dev_saa7119ConfigVideoMode			(BYTE VidDecId,BYTE B_Mode);
BYTE dev_saa7119GetVideoMode			(BYTE VidDecId);
void dev_saa7119SetVideoInput			(BYTE VidDecId,BYTE B_InType);
BYTE dev_saa7119GetVideoInput			(BYTE VidDecId);
void dev_saa7119SetVideoBrightness		(BYTE VidDecId,BYTE B_BrightVal);
BYTE dev_saa7119GetVideoBrightness		(BYTE VidDecId);
void dev_saa7119SetVideoContrast		(BYTE VidDecId,BYTE B_ContrastVal);
BYTE dev_saa7119GetVideoContrast		(BYTE VidDecId);
void dev_saa7119SetVideoHue				(BYTE VidDecId,BYTE B_HueVal);
BYTE dev_saa7119GetVideoHue				(BYTE VidDecId);
void dev_saa7119SetVideoSaturation		(BYTE VidDecId,BYTE B_SaturationVal);
BYTE dev_saa7119GetVideoSaturation		(BYTE VidDecId);
BYTE dev_saa7119GetVideoStatus			(BYTE VidDecId);
BYTE dev_saa7119GetVideoInterlaceInfo	(BYTE VidDecId);
void dev_saa7119PowerUp					(BYTE VidDecId);
void dev_saa7119PowerDown				(BYTE VidDecId);

#endif
//*********************************  END  **************************************

