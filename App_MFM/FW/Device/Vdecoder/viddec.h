/*
	$Workfile:   viddec.h  $
	$Revision: 1.5 $
	$Date: 2011/12/15 03:52:11 $
*/

#ifndef __VIDDEC_H__
#define __VIDDEC_H__
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
// USAGE:  	Header file for standard video decoder definition
//
//******************************************************************************

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************

#define VDD_1           	0			// First device

// Local decoder setting
typedef struct
{
   BYTE    InpType;
   BYTE    OutType;
   BYTE    Status;
   BYTE    Interlace;
   BYTE    SubMode;
   BYTE    Mode;
   WORD    LPF;
   BYTE    Brightness;
   BYTE    Saturation;
   BYTE    Contrast;
   BYTE    Hue;
   BYTE    Sharpness;
} VIDEO_DECODER_;

// Video types
#define VDD_INTYP_NONE          0
#define VDD_INTYP_COMPOSITE1    1
#define VDD_INTYP_SVIDEO        2
#define VDD_INTYP_COMPOSITE2    3
#define VDD_INTYP_TUNER         4
#define VDD_INTYP_COMPONENT     5


#define VDD_OUTTYPE_NONE        0
#define VDD_OUTTYPE_601 	  	1
#define VDD_OUTTYPE_656 	  	2

#define D_MIXER_ON              1
#define D_MIXER_OFF             0

#define D_NOTLOCKED             0
#define D_LOCKED				1

#define D_NONINTERLACED         0
#define D_INTERLACED 			1

// Video modes
#if 0
#define VDD_MODE_NOVIDEO       	gmd_DEC_MODE_UNKNOWN
#define VDD_MODE_NTSC          	gmd_DEC_MODE_NTSC
#define VDD_MODE_PAL           		gmd_DEC_MODE_PAL
#define VDD_MODE_SECAM         	gmd_DEC_MODE_SECAM
#define VDD_MODE_INIT	       	12//0xA0

//video sub modes
#define VDD_SUBMODE_PAL_BGHI  			0
#define VDD_SUBMODE_NTSC_M    			1
#define VDD_SUBMODE_SECAM     			2
#define VDD_SUBMODE_NTSC_44   			3
#define VDD_SUBMODE_PAL_M     			4
#define VDD_SUBMODE_PAL_N     			5
#define VDD_SUBMODE_PAL_60    			6
#define VDD_SUBMODE_NTSC_COMB 		7
#endif
//******************************************************************************
//	G L O B A L    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
#if (defined (EXTERNAL_DECODER))
#ifdef SAA7119_DEV_ID
#define vdd_ConfigVideoMode		dev_saa7119ConfigVideoMode
#define vdd_SetVideoInput	   	dev_saa7119SetVideoInput
#define vdd_GetVideoInput      	dev_saa7119GetVideoInput
#define vdd_GetVideoMode		dev_saa7119GetVideoMode
#define vdd_PowerUp            		dev_saa7119PowerUp
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

#ifdef TVP5160_DEV_ID		//kam todo
#if 1
#define vdd_ConfigVideoMode		TVP5160ConfigVideoMode_Mission
#define vdd_SetVideoInput	   	TVP5160SetVideoInput_Mission
//#define vdd_GetVideoInput      	dev_TVP5160GetVideoStatus
#define vdd_GetVideoMode		TVP5160GetVideoMode_Mission
#define vdd_PowerUp            		TVP5160PowerUp_Mission
#define vdd_PowerDown			TVP5160PowerDown_Mission

#else
#define vdd_ConfigVideoMode		dev_TVP5160ConfigVideoMode
#define vdd_SetVideoInput	   	dev_TVP5160SetVideoInput
//#define vdd_GetVideoInput      	dev_TVP5160GetVideoStatus
#define vdd_GetVideoMode		dev_TVP5160GetVideoMode
#define vdd_PowerUp            		dev_TVP5160PowerUp
#define vdd_PowerDown			dev_TVP5160PowerDown
#endif
#endif

#ifdef TVP5158_DEV_ID		
#define vdd_ConfigVideoMode		dev_TVP5158ConfigVideoMode
#define vdd_SetVideoInput	   	dev_TVP5158SetVideoInput
#define vdd_GetVideoInput      	dev_TVP5158GetVideoStatus
#define vdd_GetVideoMode		dev_TVP5158GetVideoMode
#define vdd_PowerUp            		dev_TVP5158PowerUp
#define vdd_PowerDown			dev_TVP5158PowerDown
#endif


#endif //(defined (EXTERNAL_DECODER))
#endif
//*********************************  END  **************************************

