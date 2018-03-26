/********************************************************************
COPYRIGHT (C) STMicroelectronics 2009.
All rights reserved. This document contains proprietary and
confidential information of the STMicroelectronics Group. This
document is not to be copied in whole or part. STMicroelectronics
assumes no responsibility for the consequences of use of such
information nor for any infringement of patents or other rights of
third parties which may result from its use. No license is granted by
implication or otherwise under any patent or patent rights of
STMicroelectronics.
STMicroelectronics
products are not authorized for use as critical components in life
support devices or systems without express written approval of
STMicroelectronics.

File name       : sthdmirx.h
Description     : Interface for HDMI Receiver

Date                Modification               Name
----                ------------               ----
09 APRIL 2009       Created                    Pravin Kumar
19 August 2009      Migrated to V0.6           Pravin Kumar 
26 February 2010    Migrated to V0.7           Pravin Kumar 
29 April,2010       Migrated to V0.81          Pravin Kumar
***********************************************************************/

/* Define to prevent recursive inclusion */
#ifndef __STHDMIRX_H__
#define __STHDMIRX_H__

#define STHDMIRX_X86
#define STHDMIRX_DDVI
#define STHDMIRX_ATHENA
#define STHDMIRX_REPEATER

//#define STHDMIRX_HDCP_ENABLE_RAM_LOG
#ifdef STHDMIRX_X86
#undef STHDMIRX_CSM_IP
#undef STHDMIRX_CEC_IP
#endif

/* Includes --------------------------------------------------------------------- */

#include "HdmiDvi\stddefs.h"
#ifndef STHDMIRX_X86
#include "stevt.h"
#include "stpio.h"
#include "stos.h"
#include "sttbx.h"
#endif
/* Dual Support C & C++ */
#ifdef __cplusplus
extern "C" {
#endif
/* Exported Constants ----------------------------------------------------------- */

/* Driver ID is the HDMIRX API Document number, API doc is not released so used as dummy number8*/
#ifdef STHDMIRX_X86
#define     STHDMIRX_DRIVER_ID              0   /*This is the HDMIRX API DOC number.*/
#else
#define     STHDMIRX_DRIVER_ID              480   /*This is the HDMIRX API DOC number.*/
#endif
#define     STHDMIRX_DRIVER_BASE            (STHDMIRX_DRIVER_ID<<16)
#ifdef STHDMIRX_REPEATER
typedef void far (* HDMI_Ext_Sha1Reset_t)(void);
typedef void far (* HDMI_Ext_Sha1Input_t)(const U8 * Bp_Message, U16 W_Length);
typedef U8  * far (* HDMI_Ext_Sha1Result_t)(void);
typedef void far (* HDMI_Ext_SetHPD_t)(void);
typedef void far (* HDMI_Ext_ClearHPD_t)(void);
#endif


#ifndef STHDMIRX_X86
enum
{
    STHDMIRX_ERROR_DEVICE_BIST_FAIL= STHDMIRX_DRIVER_BASE,
    STHDMIRX_ERROR_EDID_READ_FAIL,
    STHDMIRX_ERROR_EDID_WRITE_FAIL,
    STHDMIRX_ERROR_PIO_
};
#endif


#ifdef STHDMIRX_X86
typedef enum
{
    STHDMIRX_SIGNAL_STATUS_CHANGE_EVT = 0x01,
    STHDMIRX_VIDEO_PROPERTY_CHANGE_EVT = 0x02,
    STHDMIRX_AUDIO_PROPERTY_CHANGE_EVT = 0x04,
    STHDMIRX_INFORMATION_AUDIOINFO_EVT = 0x08,
    STHDMIRX_INFORMATION_ACSINFO_EVT = 0x10,
    STHDMIRX_INFORMATION_VSINFO_EVT = 0x20,
    STHDMIRX_INFORMATION_SPDINFO_EVT = 0x40,
    STHDMIRX_INFORMATION_MPEGSOURCEINFO_EVT = 0x80,
    STHDMIRX_INFORMATION_ISRCINFO_EVT = 0x100,
    STHDMIRX_INFORMATION_ACPINFO_EVT = 0x200,
    STHDMIRX_INFORMATION_ACRINFO_EVT = 0x400,
    STHDMIRX_INFORMATION_GCPINFO_EVT = 0x800,
    STHDMIRX_INFORMATION_METAINFO_EVT = 0x1000,
    STHDMIRX_INFORMATION_UNKNOWN=0x2000       /*Keep this as last Id*/
}STHDMIRX_Event_t;
#else
typedef enum
{
    STHDMIRX_SIGNAL_STATUS_CHANGE_EVT = STHDMIRX_DRIVER_BASE,
    STHDMIRX_VIDEO_PROPERTY_CHANGE_EVT,
    STHDMIRX_AUDIO_PROPERTY_CHANGE_EVT,
    STHDMIRX_INFO_UPDATE_EVT,
    STHDMIRX_DDC2Bi_MSG_RX_EVT,
    STHDMIRX_SOURCE_PLUG_EVT,
    STHDMIRX_CEC_MSG_TX_EVT	,
    STHDMIRX_CEC_MSG_RX_EVT,
    STHDMIRX_NB_REGISTERED_EVENTS_IDS       /*Keep this as last Id*/
}STHDMIRX_Event_t;
#endif

typedef enum
{
    STHDMIRX_DEVICE_TYPE_FREEMAN,
    STHDMIRX_DEVICE_TYPE_FREEMAN_PUP_PRI,
    STHDMIRX_DEVICE_TYPE_FREEMAN_PUP_SEC,
#ifdef STHDMIRX_ATHENA
	STHDMIRX_DEVICE_TYPE_ATHENA_HDMI,
	STHDMIRX_DEVICE_TYPE_ATHENA_DDVI
#endif	
} STHDMIRX_DeviceType_t;

typedef enum
{
    STHDMIRX_INPUT_PORT_1,
    STHDMIRX_INPUT_PORT_2,
    STHDMIRX_INPUT_PORT_3,
    STHDMIRX_INPUT_PORT_4
} STHDMIRX_InputPort_t;

typedef enum
{
    STHDMIRX_ROUTE_1,
    STHDMIRX_ROUTE_2
} STHDMIRX_Route_t;

typedef enum
{
    STHDMIRX_MODE_DVI	    = 0x01,
	STHDMIRX_MODE_DDVI	    = 0x02,
    STHDMIRX_MODE_HDMI	    = 0x04,
    STHDMIRX_MODE_AUTO	    = 0x08
} STHDMIRX_Mode_t;

typedef enum
{
    STHDMIRX_SIGNAL_STATUS_PRESENT,
    STHDMIRX_SINGAL_STATUS_NOT_PRESENT
} STHDMIRX_SignalStatus_t;

typedef enum
{
    STHDMIRX_SIGNAL_TYPE_DVI,
    STHDMIRX_SINGAL_TYPE_HDMI,
	STHDMIRX_SIGNAL_TYPE_DDVI
} STHDMIRX_SignalType_t;

typedef enum
{
    STHDMIRX_SIGNAL_SCAN_TYPE_INTERLACED,
    STHDMIRX_SINGAL_SCAN_TYPE_PROGRESSIVE
} STHDMIRX_SignalScanType_t;

typedef enum
{
    STHDMIRX_SIGNAL_POLARITY_NEGATIVE,
    STHDMIRX_SINGAL_POLARITY_POSITIVE
} STHDMIRX_SignalPolarity_t;

typedef enum
{
    STHDMIRX_EQ_MODE_DISABLE,
    STHDMIRX_EQ_MODE_AUTO,
    STHDMIRX_EQ_MODE_LOW_GAIN,
    STHDMIRX_EQ_MODE_MEDIUM_GAIN,
    STHDMIRX_EQ_MODE_HIGH_GAIN,
	STHDMIRX_EQ_MODE_CUSTOM,
}STHDMIRX_EqualizationMode_t;

#if 0
#ifdef STHDMIRX_X86
typedef struct
{
   gm_AVDdsSrcTypes_t    estVidDds;
   gm_AVDdsSrcTypes_t    estAudDds;
}STHDMIRX_DDSConfigParams_t;
#endif
#endif

typedef struct
	{
		U32	LowFreqGain;
		U32	HighFreqGain;
	} STHDMIRX_EqualizationGain_t;

typedef enum
{
    STHDMIRX_COLOR_DEPTH_24BPP,
    STHDMIRX_COLOR_DEPTH_30BPP,
    STHDMIRX_COLOR_DEPTH_36BPP,
    STHDMIRX_COLOR_DEPTH_48BPP
} STHDMIRX_ColorDepth_t;

typedef enum
{
    STHDMIRX_COLOR_FORMAT_RGB,
    STHDMIRX_COLOR_FORMAT_YUV_422,
    STHDMIRX_COLOR_FORMAT_YUV_444
} STHDMIRX_ColorFormat_t;

typedef enum
{
    STHDMIRX_COLORIMETRY_STD_DEFAULT,
    STHDMIRX_COLORIMETRY_STD_BT_601,
    STHDMIRX_COLORIMETRY_STD_BT_709,
    STHDMIRX_COLORIMETRY_STD_XVYCC_601,
    STHDMIRX_COLORIMETRY_STD_XVYCC_709,
    STHDMIRX_COLORIMETRY_STD_sYCC_601,
	STHDMIRX_COLORIMETRY_STD_AdobeYCC_601,
	STHDMIRX_COLORIMETRY_STD_AdobeRGB
} STHDMIRX_ColorimetryStd_t;

typedef enum
{
    STHDMIRX_SCAN_INFO_NO_DATA,
    STHDMIRX_SCAN_INFO_FOR_OVERSCANNED_DISPLAY,
    STHDMIRX_SCAN_INFO_FOR_UNDER_SCANNED_DISPLAY,
    STHDMIRX_SCAN_INFO_RESERVED
} STHDMIRX_ScanInfo_t;

typedef enum
{
    STHDMIRX_SCALING_INFO_NO_SCALING,
    STHDMIRX_SCALING_INFO_H_SCALED,
    STHDMIRX_SCALING_INFO_V_SCALED,
    STHDMIRX_SCALING_INFO_HV_SCALED
} STHDMIRX_ScalingInfo_t;

typedef enum
{
    STHDMIRX_RGB_QUANT_RANGE_DEFAULT,
    STHDMIRX_RGB_QUANT_RANGE_LIMITED,
    STHDMIRX_RGB_QUANT_RANGE_FULL,
    STHDMIRX_RGB_QUANT_RANGE_RESERVED
} STHDMIRX_RGBQuantRange_t;

typedef enum
{
    STHDMIRX_PIXEL_REPEAT_FACTOR_1,
    STHDMIRX_PIXEL_REPEAT_FACTOR_2,
    STHDMIRX_PIXEL_REPEAT_FACTOR_3,
    STHDMIRX_PIXEL_REPEAT_FACTOR_4,
    STHDMIRX_PIXEL_REPEAT_FACTOR_5,
    STHDMIRX_PIXEL_REPEAT_FACTOR_6,
    STHDMIRX_PIXEL_REPEAT_FACTOR_7,
    STHDMIRX_PIXEL_REPEAT_FACTOR_8,
    STHDMIRX_PIXEL_REPEAT_FACTOR_9,
    STHDMIRX_PIXEL_REPEAT_FACTOR_10
} STHDMIRX_PixelRepeatFactor_t;

typedef enum
{
    STHDMIRX_ASPECT_RATIO_NONE,
    STHDMIRX_ASPECT_RATIO_4_3,
    STHDMIRX_ASPECT_RATIO_16_9,
    STHDMIRX_ASPECT_RATIO_16_9_TOP,
    STHDMIRX_ASPECT_RATIO_14_9_TOP,
    STHDMIRX_ASPECT_RATIO_4_3_CENTER,
    STHDMIRX_ASPECT_RATIO_16_9_CENTER,
    STHDMIRX_ASPECT_RATIO_14_9_CENTER,
    STHDMIRX_ASPECT_RATIO_4_3_CENTER_14_9_SP,
    STHDMIRX_ASPECT_RATIO_16_9_CENTER_14_9_SP,
    STHDMIRX_ASPECT_RATIO_16_9_CENTER__4_3_SP
} STHDMIRX_AspectRatio_t;

typedef enum
{
    STHDMIRX_IEC_AUDIO_STD_60958_3,
    STHDMIRX_IEC_AUDIO_STD_60958_4,
    STHDMIRX_IEC_AUDIO_STD_61937,
    STHDMIRX_IEC_AUDIO_STD_SMPTE_337M
} STHDMIRX_IECAudioStd_t;

typedef enum
{
    STHDMIRX_AUDIO_STREAM_TYPE_UNKNOWN      =0x00,
    STHDMIRX_AUDIO_STREAM_TYPE_IEC          =0x01,
    STHDMIRX_AUDIO_STREAM_TYPE_DSD          =0x02,
    STHDMIRX_AUDIO_STREAM_TYPE_DST          =0x04,
    STHDMIRX_AUDIO_STREAM_TYPE_HBR          =0x08
} STHDMIRX_AudioStreamType_t;

typedef enum
{
    STHDMIRX_AUDIO_CODING_TYPE_NONE,
    STHDMIRX_AUDIO_CODING_TYPE_PCM,
    STHDMIRX_AUDIO_CODING_TYPE_AC3,
    STHDMIRX_AUDIO_CODING_TYPE_MPEG1,
    STHDMIRX_AUDIO_CODING_TYPE_MP3,
    STHDMIRX_AUDIO_CODING_TYPE_MPEG2,
    STHDMIRX_AUDIO_CODING_TYPE_AAC,
    STHDMIRX_AUDIO_CODING_TYPE_DTS,
    STHDMIRX_AUDIO_CODING_TYPE_ATRAC,
    STHDMIRX_AUDIO_CODING_TYPE_DSD,
    STHDMIRX_AUDIO_CODING_TYPE_DDPLUS,
    STHDMIRX_AUDIO_CODING_TYPE_DTS_HD,
    STHDMIRX_AUDIO_CODING_TYPE_MAT,
    STHDMIRX_AUDIO_CODING_TYPE_DST,
    STHDMIRX_AUDIO_CODING_TYPE_WMA_PRO
} STHDMIRX_AudioCodingType_t;

typedef enum
{
    STHDMIRX_AUDIO_SAMPLE_SIZE_NONE,
    STHDMIRX_AUDIO_SAMPLE_SIZE_16_BITS,
    STHDMIRX_AUDIO_SAMPLE_SIZE_17_BITS,
    STHDMIRX_AUDIO_SAMPLE_SIZE_18_BITS,
    STHDMIRX_AUDIO_SAMPLE_SIZE_19_BITS,
    STHDMIRX_AUDIO_SAMPLE_SIZE_20_BITS,
    STHDMIRX_AUDIO_SAMPLE_SIZE_21_BITS,
    STHDMIRX_AUDIO_SAMPLE_SIZE_22_BITS,
    STHDMIRX_AUDIO_SAMPLE_SIZE_23_BITS,
    STHDMIRX_AUDIO_SAMPLE_SIZE_24_BITS
} STHDMIRX_AudioSampleSize_t;

typedef enum
{
    STHDMIRX_AUDIO_CLOCK_ACCURACY_LEVEL_1,
    STHDMIRX_AUDIO_CLOCK_ACCURACY_LEVEL_2,
    STHDMIRX_AUDIO_CLOCK_ACCURACY_LEVEL_3,
    STHDMIRX_AUDIO_CLOCK_ACCURACY_UNMATCHED
} STHDMIRX_AudioClockAccuracy_t;

typedef enum
{
    STHDMIRX_AUDIO_CP_TYPE_GENERIC,
    STHDMIRX_AUDIO_CP_TYPE_IEC60958,
    STHDMIRX_AUDIO_CP_TYPE_DVD,
    STHDMIRX_AUDIO_CP_TYPE_SACD
} STHDMIRX_AudioCPType_t;

typedef enum
{
    STHDMIRX_HDCP_STATUS_NOT_AUTHENTICATED,
    STHDMIRX_HDCP_STATUS_AUTHENTICATED,
    STHDMIRX_HDCP_STATUS_AUTHENTICATED_NOISE_DETECTED
} STHDMIRX_HDCPStatus_t;

typedef enum
{
    STHDMIRX_OUTPUT_PIXEL_WIDTH_8_BITS      =0x01,
    STHDMIRX_OUTPUT_PIXEL_WIDTH_10_BITS     =0x02,
    STHDMIRX_OUTPUT_PIXEL_WIDTH_12_BITS     =0x04,
    STHDMIRX_OUTPUT_PIXEL_WIDTH_16_BITS     =0x08
} STHDMIRX_OutputPixelWidth_t;

typedef enum
{
    STHDMIRX_INFORMATION_VSINFO,
    STHDMIRX_INFORMATION_SPDINFO,
    STHDMIRX_INFORMATION_MPEGSOURCEINFO,
    STHDMIRX_INFORMATION_ISRCINFO,
    STHDMIRX_INFORMATION_ACPINFO,
    STHDMIRX_INFORMATION_GBDINFO
} STHDMIRX_InfoType_t;

typedef enum
{
    STHDMIRX_GBD_PROFILE_0,
    STHDMIRX_GBD_PROFILE_1,
    STHDMIRX_GBD_PROFILE_2,
    STHDMIRX_GBD_PROFILE_3
} STHDMIRX_GBDProfile_t;

typedef enum
{
    STHDMIRX_GBD_FORMAT_VERTEX,
    STHDMIRX_GBD_FORMAT_RGBRANGE
} STHDMIRX_GBDFormat_t;

typedef enum
{
    STHDMIRX_MPEG_PICTURE_TYPE_I,
    STHDMIRX_MPEG_PICTURE_TYPE_P,
    STHDMIRX_MPEG_PICTURE_TYPE_B
} STHDMIRX_MPEGPictureType_t;

#ifndef STHDMIRX_ATHENA
typedef enum
{
    STHDMIRX_SOURCE_PLUG_STATUS_IN,
    STHDMIRX_SOURCE_PLUG_STATUS_OUT
} STHDMIRX_SourcePlugStatus_t;

typedef enum
{
    STHDMIRX_CEC_TX_STATUS_SUCCESS,
    STHDMIRX_CEC_TX_STATUS_FAIL_BUSY,
    STHDMIRX_CEC_TX_STATUS_FAIL_NACK
} STHDMIRX_CECMsgTxStatus_t;


typedef enum
{
	STHDMIRX_EDID_CONFIG_EXTERNAL,
	STHDMIRX_EDID_CONFIG_INTERNAL
}STHDMIRX_EDIDConfig_t;

#endif

typedef enum
{
	STHDMIRX_YC_QUANT_RANGE_LIMITED,
	STHDMIRX_YC_QUANT_RANGE_FULL,
	STHDMIRX_YC_QUANT_RANGE_RESERVED
} STHDMIRX_YCQuantRange_t;

typedef enum
{
	STHDMIRX_CONTENT_TYPE_UNKNOWN,
	STHDMIRX_CONTENT_TYPE_GRAPHICS,
	STHDMIRX_CONTENT_TYPE_PHOTO,
	STHDMIRX_CONTENT_TYPE_CINEMA,
	STHDMIRX_CONTENT_TYPE_GAME
} STHDMIRX_ContentType_t;

typedef enum
{
	STHDMIRX_AUDIO_LFE_PLAYBACK_LEVEL_UNKNOWN,
	STHDMIRX_AUDIO_LFE_PLAYBACK_LEVEL_0dB,
	STHDMIRX_AUDIO_LFE_PLAYBACK_LEVEL_10dBs
} STHDMIRX_AudioLFEPlayBackLevel_t;

typedef enum
{
    STHDMIRX_3D_FORMAT_UNKNOWN,
	STHDMIRX_3D_FORMAT_FRAME_PACK,
	STHDMIRX_3D_FORMAT_FIELD_ALT,
	STHDMIRX_3D_FORMAT_LINE_ALT,
	STHDMIRX_3D_FORMAT_SBYS_FULL,
	STHDMIRX_3D_FORMAT_SBYS_HALF,
	STHDMIRX_3D_FORMAT_L_D,
	STHDMIRX_3D_FORMAT_L_D_G_GMINUSD,
	STHDMIRX_3D_FORMAT_TOP_AND_BOTTOM
} STHDMIRX_3DFormat_t;

typedef enum
{
	STHDMIRX_3D_METADATA_NONE,
	STHDMIRX_3D_METADATA_ISO23002_3
} STHDMIRX_3DMetaDataType_t;

typedef enum
{
	STHDMIRX_3D_SBYS_HALF_SAMPLING_HORZ_OLOR,
	STHDMIRX_3D_SBYS_HALF_SAMPLING_HORZ_OLER,
	STHDMIRX_3D_SBYS_HALF_SAMPLING_HORZ_ELOR,
	STHDMIRX_3D_SBYS_HALF_SAMPLING_HORZ_ELER,
	STHDMIRX_3D_SBYS_HALF_SAMPLING_QUIN_OLOR,
	STHDMIRX_3D_SBYS_HALF_SAMPLING_QUIN_OLER,
	STHDMIRX_3D_SBYS_HALF_SAMPLING_QUIN_ELOR,
	STHDMIRX_3D_SBYS_HALF_SAMPLING_QUIN_ELER
} STHDMIRX_3DSideBySideHalfSampling_t;

typedef enum
{
   STHDMIRX_CORE_CID  = 0x01,
   STHDMIRX_VIDEO_CID = 0x02,
   STHDMIRX_AUDIO_CID = 0x04,
   STHDMIRX_EQ_CID    = 0x08,
   STHDMIRX_CID_LAST
} STHDMIRX_DriverComponentIds_t;

typedef enum
{
   INFO,
   WARNING,
   ERROR,
} STHDMIRX_DebugMsgType_t;

typedef enum
{
   LEVEL0,
   LEVEL1,
   LEVEL2,
   LEVEL3
} STHDMIRX_DebugMsgLevel_t;


/* Exported Types --------------------------------------------------------- ---------------*/

typedef U32	 STHDMIRX_Handle_t;
typedef U32	 STHDMIRX_InputHandle_t;
typedef U8	 STHDMIRX_AudioChannelStatus_t[24];
#ifdef STHDMIRX_DDVI
typedef struct
{
	U16 HWidth;
	U16 VWidth;
}STHDMIRX_DDVIExceptionModeTable_t;
#endif

typedef struct
{
	STHDMIRX_DeviceType_t			DeviceType;
	void*                         	DeviceBaseAddress_p;
	void*                          	BaseAddress_p;
    void*							ClockGenBaseAddress_p;
    void*                           PhyBaseAddress_p;
#ifdef STHDMIRX_DDVI
	void*                           PhyBaseAddress_p2;	
    STHDMIRX_DDVIExceptionModeTable_t far*  DDVITablePtr;
#endif
#ifndef STHDMIRX_X86	
    U32								RxCoreInterruptNumber ;
    U32								RxCoreInterruptLevel;
    U32								MuteInterruptNumber;
    U32								MuteInterruptLevel;
    U32								CSMInterruptNumber;
    U32								CSMInterruptLevel;
    U32								IFMInterruptNumber;
    U32								IFMInterruptLevel;
    U32                             PHYInterruptNumber;
    U32                             PHYInterruptLevel;
	ST_DeviceName_t					EventHandlerName;
#endif	
	U32								MaxOpen;
	U32								MaxInputOpen;
	U32								MeasureClockFreqHz;
	U16                             I2SOutClkScalingFactor;
#ifdef STHDMIRX_REPEATER
    HDMI_Ext_Sha1Reset_t						Sha1Reset_pf;						
    HDMI_Ext_Sha1Input_t						Sha1Input_pf;						
    HDMI_Ext_Sha1Result_t						Sha1Result_pf;
    HDMI_Ext_SetHPD_t               SetHPD_pf;
    HDMI_Ext_ClearHPD_t             ClearHPD_pf;    
#endif
} STHDMIRX_InitParams_t;

typedef struct
{
	BOOL	ForceTerminate;
} STHDMIRX_TermParams_t;

typedef struct
{
	U32								NumberOfInputPortsSupported;
	U32								NumberOfRoutesSupported;
	BOOL							SPDSupport;
	BOOL							CECInterfaceSupport;
	BOOL							DDCInterfaceSupport;
	STHDMIRX_OutputPixelWidth_t		SupportedOutputPixelWidth;
	STHDMIRX_AudioStreamType_t		SupportedAudioStreams;
	BOOL							VideoSupport3D;
#ifdef STHDMIRX_DDVI
	BOOL                            DDVISupport;
#endif	
} STHDMIRX_Capability_t;

typedef struct
{
	U32		NotUsed;
} STHDMIRX_OpenParams_t;

typedef struct
{
    BOOL            InternalEDID;
    BOOL            ListenDDC2Bi;
#ifndef STHDMIRX_X86
    STPIO_PIOBit_t  HDPPIO;
    STPIO_PIOBit_t  PDPIO;
    STPIO_PIOBit_t  DDCSlaveSCLPIO;
    STPIO_PIOBit_t  DDCSlaveSDAPIO;
#endif    
} STHDMIRX_PortSetting_t;

typedef struct
{
	STHDMIRX_InputPort_t			Port;
	STHDMIRX_Route_t				Route;
	STHDMIRX_Mode_t					ModeOfOperation;
	STHDMIRX_OutputPixelWidth_t	    OutputPixelWidth;		
	STHDMIRX_EqualizationMode_t	    EqMode;
#ifdef STHDMIRX_X86
	U8                              CustomRterm;
    STHDMIRX_DDSConfigParams_t       DdsConfigInfo;
#endif
} STHDMIRX_InputConfiguration_t;

typedef struct
{
	BOOL	HBarValid;
	U16		EndOfLeftBar;
	U16		StartOfRightBar;
	BOOL	VBarValid;
	U16		EndOfTopBar;
	U16 	StartOfBottomBar;
} STHDMIRX_BarInfo_t;

typedef struct
{
	STHDMIRX_3DMetaDataType_t	Type;
	U8							DataLen;
	U8							MetaData[32];
} STHDMIRX_3DMetaData_t;

typedef struct
{
	STHDMIRX_3DSideBySideHalfSampling_t	SamplingMode;
} STHDMIRX_3DSideBySideHalfExtData_t;

typedef union
{
	STHDMIRX_3DSideBySideHalfExtData_t SideBySideHalf;
}STHDMIRX_3DExtData_t;


typedef struct
{
	STHDMIRX_3DFormat_t	    Format;
	STHDMIRX_3DExtData_t	ExtData;
	STHDMIRX_3DMetaData_t	MetaData;
} STHDMIRX_3DVideoProperty_t;


typedef struct
{
	STHDMIRX_ColorDepth_t			ColorDepth;
	STHDMIRX_ColorFormat_t			ColorFormat;
	STHDMIRX_ColorimetryStd_t		Colorimetery;
	STHDMIRX_BarInfo_t				BarInfo;
	STHDMIRX_ScanInfo_t			    ScanInfo;
	STHDMIRX_ScalingInfo_t			ScalingInfo;
	STHDMIRX_AspectRatio_t	 		PictureAR;
	STHDMIRX_AspectRatio_t			ActiveFormatAR;
	STHDMIRX_RGBQuantRange_t		RGBQuantRange;
	STHDMIRX_PixelRepeatFactor_t	PixelRepeatFactor;
	BOOL							ITContent;
	U32								VideoTimingCode;
    STHDMIRX_YCQuantRange_t		    YCQuantRange;
    STHDMIRX_ContentType_t			ContentType;
    BOOL							HDMIVideoFormat;
    BOOL							Video3D;
    STHDMIRX_3DVideoProperty_t		VideoProperty3D;
} STHDMIRX_VideoProperty_t;

typedef struct
{
	STHDMIRX_SignalScanType_t		ScanType;
	U32								PixelClockHz;
	STHDMIRX_SignalPolarity_t		HSyncPolarity;
	U16								HActiveStart;
	U16								HActive ;
	U16								HTotal;
	STHDMIRX_SignalPolarity_t		VSyncPolarity;
	U16								VActiveStart;
	U16								VActive ;
	U16								VTotal;
} STHDMIRX_SignalTiming_t;

typedef struct
{
	STHDMIRX_AudioStreamType_t		    StreamType;
	STHDMIRX_AudioCodingType_t		    CodingType;
	U8								    ChannelCount;
	U8								    ChannelAllocation;
	U32								    SamplingFrequency;
	STHDMIRX_AudioSampleSize_t		    SampleSize;
	BOOL						    	DownMixInhibit;
	U8								    LevelShift;
	STHDMIRX_AudioLFEPlayBackLevel_t    LFEPlayBackLevel;
} STHDMIRX_AudioProperty_t;


typedef struct
{
	STHDMIRX_SignalType_t		SignalType;
	STHDMIRX_SignalTiming_t	    Timing;
	STHDMIRX_VideoProperty_t	Video;
	STHDMIRX_AudioProperty_t	Audio;
} STHDMIRX_SignalProperty_t;

typedef struct
{
	U32		IEEERegId;
	U8		Length;
	U8		Payload[30];	
} STHDMIRX_VSInfo_t;

typedef struct
{
	U8		VendorName[8];
	U8		Description[16];
	U8		DeviceInfo;
} STHDMIRX_SPDInfo_t;

typedef struct
{
	U32								BitRate;
	BOOL							FieldRepeat;
	STHDMIRX_MPEGPictureType_t		PictureType;
} STHDMIRX_MPEGSourceInfo_t;

typedef struct
{
	U8	Status;
	U8	Length;
	U8  Info[32];
} STHDMIRX_ISRCInfo_t;

typedef struct
{
	STHDMIRX_AudioCPType_t		ACPType;
	U8						 	ACPData[28];	
} STHDMIRX_ACPInfo_t;

typedef struct
{
	BOOL						FacetPresent;
	U8							Precision;
	STHDMIRX_ColorimetryStd_t	Colorimetry;
	U16							NumberOfVertices;
	U16*						Vertices;
	U16							NumberOfFacets;
	U16*						Facets;
} STHDMIRX_GBDVertexFormat_t;

typedef struct
{
	U8							Precision;
	STHDMIRX_ColorimetryStd_t	Colorimetry;
	U16							MinRed;
	U16							MaxRed;
	U16							MinGreen;
	U16							MaxGreen;
	U16							MinBlue;
	U16							MaxBlue;
} STHDMIRX_GBDRangeFormat_t;

typedef struct
{
    U8                          Precision;
    STHDMIRX_ColorimetryStd_t   Colorimetry;
    U16                         Vertices[4];
}STHDMIRX_GBDVertexFormatP0_t;

#ifdef STHDMIRX_ATHENA
typedef struct
{
    U8 GBD[28];
} STHDMIRX_GBDInfo_t;
#endif

#ifndef STHDMIRX_ATHENA
typedef struct
{
	STHDMIRX_GBDProfile_t				Profile;
	STHDMIRX_GBDFormat_t			    Format;
	union
	{
		STHDMIRX_GBDVertexFormatP0_t	VertexFormP0;
		STHDMIRX_GBDRangeFormat_t		RangeForm;
	} Desc;

} STHDMIRX_GBDInfo_t;
typedef struct
{
    STPIO_PIOBit_t CECPIO;
} STHDMIRX_CECSetting_t;

typedef struct
{
	U8	SrcAddr;
	U8	DestAddr;
	U8	OpCode;
	U8	LengthOfOperand;
	U8	Operand[16];
} STHDMIRX_CECMsg_t;

typedef struct
{
	U32         Size;
	U8*         EDIDData;
} STHDMIRX_EDIDInitParams_t;

typedef U8 STHDMIRX_EDIDBlock_t[128];

typedef struct
{
    U8      DestAddr;
    U8      SrcAddr;
    U8      Length;
    U8      Data[127];
    U8      Checksum;
} STHDMIRX_DDC2BiMsg_t;

typedef struct
{
    STHDMIRX_InputPort_t    Port;
    STHDMIRX_DDC2BiMsg_t    Msg;
} STHDMIRX_DDC2BiMsgRxEvtData_t;

typedef struct
{
	STHDMIRX_InputHandle_t		InputHandle;
	STHDMIRX_HDCPStatus_t		Status;	
} STHDMIRX_HDCPStatusEvtData_t;

typedef struct
{
	STHDMIRX_InputPort_t			Port;
	STHDMIRX_SourcePlugStatus_t	    Status;
} STHDMIRX_SourcePlugEvtData_t;
#endif

typedef struct
{
	STHDMIRX_InputHandle_t		InputHandle;
	STHDMIRX_SignalStatus_t	    Status;
	STHDMIRX_SignalProperty_t	Property;
	STHDMIRX_HDCPStatus_t       HDCPStatus;
} STHDMIRX_SignalStatusEvtData_t;

typedef struct
{
	STHDMIRX_InputHandle_t		InputHandle;
	STHDMIRX_VideoProperty_t	Property;
} STHDMIRX_VideoPropertyEvtData_t;

typedef struct
{
	STHDMIRX_InputHandle_t		InputHandle;
	STHDMIRX_AudioProperty_t	Property;
} STHDMIRX_AudioPropertyEvtData_t;

typedef struct
{
	STHDMIRX_InputHandle_t			InputHandle;
	STHDMIRX_InfoType_t			    InfoType;
	union
	{
		STHDMIRX_VSInfo_t			VSInfo;
		STHDMIRX_SPDInfo_t			SPDInfo;
		STHDMIRX_MPEGSourceInfo_t	MPEGInfo;
		STHDMIRX_ISRCInfo_t			ISRCInfo;
		STHDMIRX_ACPInfo_t			ACPInfo;
		STHDMIRX_GBDInfo_t			GBDInfo;
	} Type;
} STHDMIRX_InfoUpdateEvtData_t;




/* Exported Variables ------------------------------------------------------------------- */

/* Exported Macros --------------------------------------------------------------------- */

/* Exported Functions ------------------------------------------------------------------- */

ST_Revision_t   STHDMIRX_GetRevision(void);

ST_ErrorCode_t  STHDMIRX_GetCapability(
		        ST_DeviceName_t				DeviceName,
		        STHDMIRX_Capability_t*		Capability
		        );

ST_ErrorCode_t  STHDMIRX_Init(
        		ST_DeviceName_t			DeviceName,
        		STHDMIRX_InitParams_t*	InitParams
        		);

ST_ErrorCode_t  STHDMIRX_Open(
        		ST_DeviceName_t			DeviceName,
        		STHDMIRX_OpenParams_t*	OpenParams,
        		STHDMIRX_Handle_t*		Handle
        		);

ST_ErrorCode_t  STHDMIRX_Close(
        		STHDMIRX_Handle_t	Handle
        		);

ST_ErrorCode_t  STHDMIRX_Term(
        		ST_DeviceName_t			DeviceName,
        		STHDMIRX_TermParams_t*	TermParams
        		);

ST_ErrorCode_t  STHDMIRX_OpenInput(
        		STHDMIRX_Handle_t				Handle,
        		STHDMIRX_InputConfiguration_t*	Configuration,
        		STHDMIRX_InputHandle_t*			InputHandle);

ST_ErrorCode_t  STHDMIRX_CloseInput(
		        STHDMIRX_InputHandle_t		InputHandle
		        );

ST_ErrorCode_t  STHDMIRX_GetInputConfiguration(
		        STHDMIRX_InputHandle_t				InputHandle,
		        STHDMIRX_InputConfiguration_t*		Configuration
		        );

ST_ErrorCode_t  STHDMIRX_StartSignalMonitor(
		        STHDMIRX_InputHandle_t	InputHandle
		        );

ST_ErrorCode_t  STHDMIRX_StopSignalMonitor(
		        STHDMIRX_InputHandle_t	InputHandle
		        );

ST_ErrorCode_t  STHDMIRX_GetAudioChannelStatus(
		        STHDMIRX_InputHandle_t				InputHandle,
		        STHDMIRX_AudioChannelStatus_t*		ChannelStatus
		        );

ST_ErrorCode_t  STHDMIRX_StartAudio(
		        STHDMIRX_InputHandle_t	InputHandle
		        );

ST_ErrorCode_t  STHDMIRX_StopAudio(
		        STHDMIRX_InputHandle_t	InputHandle
		        );

ST_ErrorCode_t  STHDMIRX_ReCaptureInfo(
        		STHDMIRX_InputHandle_t		InputHandle,
        		STHDMIRX_InfoType_t			InfoType
        		);

ST_ErrorCode_t  STHDMIRX_InitPortSetting(
                STHDMIRX_Handle_t           Handle,
                STHDMIRX_InputPort_t        Port,
                STHDMIRX_PortSetting_t*     PortSetting
                );

#ifndef STHDMIRX_ATHENA
ST_ErrorCode_t  STHDMIRX_GetHDCPStatus(
		        STHDMIRX_InputHandle_t	InputHandle,
		        STHDMIRX_HDCPStatus_t*  Status
		        );

ST_ErrorCode_t  STHDMIRX_StartSourceDetection(
		        STHDMIRX_Handle_t	Handle
		        );

ST_ErrorCode_t  STHDMIRX_StopSourceDetection(
		        STHDMIRX_Handle_t	Handle
		        );


ST_ErrorCode_t  STHDMIRX_ClearHPDSignal(
		        STHDMIRX_Handle_t		Handle,
		        STHDMIRX_InputPort_t    PortId
		        );

ST_ErrorCode_t  STHDMIRX_SetHPDSignal(
		        STHDMIRX_Handle_t		Handle,
		        STHDMIRX_InputPort_t	PortId
		        );
ST_ErrorCode_t  STHDMIRX_InitCECSetting(
                STHDMIRX_Handle_t       Handle,
                STHDMIRX_CECSetting_t*  CECSetting
                );
ST_ErrorCode_t  STHDMIRX_CECSetLogicalAddress(
		        STHDMIRX_Handle_t	Handle,
		        U8					LogicalAddress
		        );

ST_ErrorCode_t  STHDMIRX_CECSendMsg(
		        STHDMIRX_Handle_t			Handle,
		        STHDMIRX_CECMsg_t*		    Msg
		        );




ST_ErrorCode_t  STHDMIRX_InitializeEDID(
        		STHDMIRX_Handle_t				Handle,
		        STHDMIRX_InputPort_t			PortID,
		        STHDMIRX_EDIDInitParams_t*		EDIDInit
		        );

ST_ErrorCode_t  STHDMIRX_ReadEDID(
		        STHDMIRX_Handle_t			Handle,
		        STHDMIRX_InputPort_t		PortId,
		        U8							BlockNum,
		        STHDMIRX_EDIDBlock_t		Block
		        );

ST_ErrorCode_t  STHDMIRX_UpdateEDID(
		        STHDMIRX_Handle_t		Handle,
		        STHDMIRX_InputPort_t	PortId,
		        U8						BlockNum,
		        STHDMIRX_EDIDBlock_t	Block
		        );

ST_ErrorCode_t  STHDMIRX_SendDDC2BiMsg(
                STHDMIRX_Handle_t       Handle,
                STHDMIRX_InputPort_t    Port,
                STHDMIRX_DDC2BiMsg_t*   Msg
                );
#endif

ST_ErrorCode_t STHDMIRX_SetEqualizationGain(
		STHDMIRX_InputHandle_t			InputHandle,
		STHDMIRX_EqualizationGain_t		*EqGain
	);

void STHdmiRx_SetRawSyncForInterlaced(BOOL b_SetRawSync);
BOOL STHdmiRx_GetRawSyncForInterlaced(void);
void far STHDMIRX_DebugMsgControl(U8 B_InDrvId, U8 InMsgType, U8 InMsgLevel);

#ifdef STHDMIRX_X86
void  STHDMIRX_InterruptHandler (STHDMIRX_InputHandle_t	InputHandle);
//void  STHDMIRX_InterruptHandler (void);
				
STHDMIRX_Event_t STHDMIRX_Handler(
		        STHDMIRX_Handle_t	Handle
	            );

void            STHDMIRX_GetEventData(
		        STHDMIRX_Handle_t	Handle,
                STHDMIRX_Event_t   EVENT,
                void*              EvtData
	            );
BOOL  STHDMIRX_GetHwMuteStatus(
		        STHDMIRX_InputHandle_t	Handle
	            );
#endif


#ifdef STHDMIRX_REPEATER
#if 0
typedef enum
{
	RxRequest_GetStatus				= 1,
	RxRequest_SetRepeaterBit		= 2,
	RxRequest_ClearRepeaterBit		= 3,
	RxRequest_SetBinfo				= 4,
	RxRequest_SetKsvList			= 5,
	RxRequest_SetReadyBit           = 6,
	RxRequest_StartReAuthentication = 7,
	RxRequest_RepeaterAuthenticationStarted = 8
} HdcpRepeaterRxRequest_t;


void  far STHDMIRX_HdcpRptrEntry(U32 InputHandle,
										HdcpRepeaterRxRequest_t	RxRequest_e,
										U8*							RxRequestData_pv);

#endif

void far  STHDMIRX_HdcpKsvServiceRoutine (STHDMIRX_InputHandle_t	InputHandle);


#endif

/* Dual Support C & C++ */
#ifdef __cplusplus
}
#endif
#endif /*__STHDMIRX_H__*/


