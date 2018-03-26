/*
    $Workfile:   gm_HdmiInfo.h  $
    $Revision: 1.9 $
    $Date: 2012/06/20 09:55:10 $
*/
#ifndef _HDMI_INFO_H_
#define _HDMI_INFO_H_
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
// MODULE : gm_HdmiInfo.h
//
// USAGE  :
//
//*******************************************************************************
#if (FEATURE_HDMI == ENABLE)


#include "HdmiDvi\sthdmirx.h"



#ifndef __CEA_861__
#define __CEA_861__
//Color space values of AVI InfoFrame (see CEA-861-D ,table  8)
typedef enum
{
   gmd_CEA861_AVI_Y_RGB = 0,
   gmd_CEA861_AVI_Y_YUV422,
   gmd_CEA861_AVI_Y_YUV444,
   gmd_CEA861_AVI_Y_RSVD
} gmt_CEA861_AVI_Y;

//Active information present field of AVI InfoFrame (CEA-861-D, table 8)
typedef enum
{
   gmd_CEA861_AVI_A_NODATA = 0,
   gmd_CEA861_AVI_A_DATA_VALID
} gmt_CEA861_AVI_A;

//Bar info data valid field of AVI InfoFrame (CEA-861-D, table 8)
typedef enum
{
   gmd_CEA861_AVI_B_NO_BAR = 0,
   gmd_CEA861_AVI_B_V_BAR,
   gmd_CEA861_AVI_B_H_BAR,
   gmd_CEA861_AVI_B_HV_BAR
} gmt_CEA861_AVI_B;

//Scan information field of AVI InfoFrame (CEA-861-D, table 8)
typedef enum
{
   gmd_CEA861_AVI_S_NO_DATA = 0,
   gmd_CEA861_AVI_S_OVERSCAN_TV,
   gmd_CEA861_AVI_S_UNDERSCAN,
   gmd_CEA861_AVI_S_RSVD
} gmt_CEA861_AVI_S;

//Colorimetry field of AVI InfoFrame (CEA-861-D, table 9)
typedef enum
{
   gmd_CEA861_AVI_C_NO_DATA = 0,
   gmd_CEA861_AVI_C_ITU_BT_601,
   gmd_CEA861_AVI_C_BT_709,
   gmd_CEA861_AVI_C_EXTENDED
} gmt_CEA861_AVI_C;

//Picture aspect ratio field (4:3, 16:9) of AVI InfoFrame (CEA-861-D, table 9)
typedef enum
{
   gmd_CEA861_AVI_M_NO_DATA = 0,
   gmd_CEA861_AVI_M_4_3,
   gmd_CEA861_AVI_M_16_9,
   gmd_CEA861_AVI_M_RSVD
} gmt_CEA861_AVI_M;

//Non-uniform picture scaling field (4:3, 16:9) of AVI InfoFrame (CEA-861-D, table 11)
typedef enum
{
   gmd_CEA861_AVI_SC_NONE = 0,
   gmd_CEA861_AVI_SC_H,
   gmd_CEA861_AVI_SC_V,
   gmd_CEA861_AVI_SC_HV,
} gmt_CEA861_AVI_SC;

//Audio coding type (CEA-861-D, table 17)
typedef enum
{
   gmd_CEA861_AUDIO_CT_NONE = 0,
   gmd_CEA861_AUDIO_CT_PCM,
   gmd_CEA861_AUDIO_CT_AC3,
   gmd_CEA861_AUDIO_CT_MPEG1,
   gmd_CEA861_AUDIO_CT_MP3,
   gmd_CEA861_AUDIO_CT_MPEG2,
   gmd_CEA861_AUDIO_CT_AAC,
   gmd_CEA861_AUDIO_CT_DTS,
   gmd_CEA861_AUDIO_CT_ATRAC,
   gmd_CEA861_AUDIO_CT_DSD,    // One Bit Audio
   gmd_CEA861_AUDIO_CT_DOLBY_DIGITAL_PLUS,
   gmd_CEA861_AUDIO_CT_DTS_HD,
   gmd_CEA861_AUDIO_CT_MAT,
   gmd_CEA861_AUDIO_CT_DST,
   gmd_CEA861_AUDIO_CT_WMA_PRO,
   gmd_CEA861_AUDIO_CT_RSVD
} gmt_CEA861_AUDIO_CT;
#endif //#ifndef __CEA_861__



#define HDMI_AVI_S_NO_DATA     gmd_CEA861_AVI_S_NO_DATA
#define HDMI_AVI_S_OVERSCAN_TV gmd_CEA861_AVI_S_OVERSCAN_TV
#define HDMI_AVI_S_UNDERSCAN   gmd_CEA861_AVI_S_UNDERSCAN
#define HDMI_AVI_S_END         gmd_CEA861_AVI_S_RSVD
#define HDMI_AVI_S             gmt_CEA861_AVI_S

#define HDMI_AVI_Y_RGB    gmd_CEA861_AVI_Y_RGB
#define HDMI_AVI_Y_YUV422 gmd_CEA861_AVI_Y_YUV422
#define HDMI_AVI_Y_YUV444 gmd_CEA861_AVI_Y_YUV444
#define HDMI_AVI_Y_RSVD   gmd_CEA861_AVI_Y_RSVD
#define HDMI_AVI_Y        gmt_CEA861_AVI_Y

#define HDMI_AUDIO_CT	gmt_CEA861_AUDIO_CT

#define HDMI_PIXEL_REPEAT_THRESHOLD 1

gmt_CEA861_AVI_Y gm_HdmiGetAviColorSpace(BYTE InputConnector);
BOOL gm_HdmiIsStableState(BYTE InputConnector);
BOOL gm_HdmiIsTmingMatched(gmt_PHY_CH Channel, gmt_FrontEndSignalTiming_t* Ptr_Timing);
BOOL gm_IsHdmi3DTiming(BYTE InputConnector);
BOOL gm_IsHdmiTiming(BYTE InputConnector);
BYTE gm_HdmiGetAviPixelRepetition(BYTE InputConnector);
gmt_CEA861_AVI_S gm_HdmiGetAviScanInfo(BYTE InputConnector);
STHDMIRX_ColorimetryStd_t HdmiGetColorimetry(BYTE InputConnector);
STHDMIRX_RGBQuantRange_t HdmiGetRGBQuantRange(BYTE InputConnector);
STHDMIRX_YCQuantRange_t HdmiGetYCQuantRange(BYTE InputConnector);
void GeneralHdmiDviPowerUp(void);
void GeneralHdmiDviPowerDown(void);
#endif

#endif _HDMI_INFO_H_
