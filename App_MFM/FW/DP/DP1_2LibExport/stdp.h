/************************************************************************************************************

                                  COPYRIGHT (C) STMicroelectronics 2004.

	All rights reserved. This document contains proprietary and confidential information of the STMicro-
	electronics Group. This document is not to be copied in whole or part. STMicroelectronics assumes no
	responsibility for the consequences of use of such information nor for any infringement	of patents
	or other rights of third parties which may result from its use. No license is granted by implication
	or otherwise under any patent or patent rights of STMicroelectronics. STMicroelectronics products are
	not authorized for use as critical components in life support devices or systems without express writ-
	ten approval of STMicroelectronics.

File name 		:  
Author 			: 
Description		: 
Creation 		: 
*************************************************************************************************************/

/* Define to prevent recursive inclusion */
#ifndef __STDP_H__
#define __STDP_H__
//******************************************************************************
//	C O M P I L E R    O P T I O N S										
//******************************************************************************
// Force byte alignment
#ifdef __PARADIGM__
#pragma pack(push, 1)
#endif
#pragma option -b- //as a byte
//******************************************************************************
//  I N C L U D E   F I L E S
//******************************************************************************

//******************************************************************************
//  C O N S T A N T   D E F I N E S
//******************************************************************************


#define DISABLED							0
#define ENABLED								1
#define DP_MULTISTREAM_SOURCE				DISABLED

#define DP_HUB 								DISABLED

#define LQA_DEBUG 							DISABLED
#define MAIN_LINK_LOG 						DISABLED

#define LT_RESULT_CHECK
//******************************************************************************


#define  MAX_AUX_DATA_LEN			16

#define DPRX_MAX_SDP_SIZE			28
#define DPCD_MST_BUFF_SIZE			512
#define MAX_SBM_SIZE				48
#define MAX_MSG_LEN_PER_TRANS		36
#define MAX_NUM_PAYLOADS			8			// Max supported by HW
#define MAX_NUMBER_OF_TIMESLOTS					63

#define AUX_REPLY_CMD_AUX_ACK     0x00
#define AUX_REPLY_CMD_AUX_NACK    0x01
#define AUX_REPLY_CMD_AUX_DEFER   0x02
#define AUX_REPLY_CMD_I2C_NACK    0x04
#define AUX_REPLY_CMD_I2C_DEFER   0x08
//******************************************************************************
//  M A C R O   D E F I N E S
//******************************************************************************

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

//******************************************************************************
//  T Y P E D E F S 
//******************************************************************************

//******************************************************************************
//  T Y P E D E F S   ( E N U M S )
//******************************************************************************

//Which DisplayPort Ip Version should be used
typedef enum
{
	DP_IpVersion_0,						
	DP_IpVersion_1,						
	DP_IpVersion_2						
} DP_IpVersion_t;


// Which DisplayPort specification should be used.
typedef enum
{
	DP_Standard_Dp,							// The DisplayPort specification.
	DP_Standard_Edp,						// The eDP specification.
	DP_Standard_Idp,						// The iDP specification.
	DP_Standard_MyDp						// The MYDP specification.
} DP_Standard_t;

// Specifies which Link Transport mode is supported or is being used.
typedef enum
{
	DP_Version1_0 = 0x10, 
	DP_Version1_1 = 0x11,
	DP_Version1_2 = 0x12
} DP_Version_t;

typedef struct DP_LibVersion_s
{
	U8		DpStdMajor_8;					// The major DisplayPort version supported.
	U8		DpStdMinor_8;					// The minor DisplayPort version supported.
	U8		DpIpVersion_8;					// The major hardware DisplayPort IP block version supported.
	U8		DpIpRevision_8;					// The minor hardware DisplayPort IP block version supported.
	U16		LibReleaseNumber_16;			// The release number of this library
	U16		LibBuildNumber_16;				// The build number of this library
} DP_LibVersion_t;

typedef enum
{
	DP_MainLinkRate_None		= 0x00,		// Main Link is not established
	DP_MainLinkRate_Rbr		= 0x06,		// 1.62Gbps per lane (RBR)
	DP_MainLinkRate_Hbr		= 0x0A,		// 2.7Gbps per lane (HBR)
	DP_MainLinkRate_Turbo		= 0x0B,		// 2.97Gbps per lane (TBR)
	DP_MainLinkRate_Turbo2		= 0x0C,		// 3.24Gbps per lane (TBR2) used in iDP
	DP_MainLinkRate_Hbr2		= 0x14		// 5.4Gbps per lane (HBR2)
} DP_MainLinkRate_t;

typedef enum
{
	DP_MainLinkNumLanes_Error		= 0,		// Link is not trained or invalid number of lanes
	DP_MainLinkNumLanes_1Lane	= 1,		// 1 Main Link Lane
	DP_MainLinkNumLanes_2Lanes	= 2,		// 2 Main Link Lanes
	DP_MainLinkNumLanes_4Lanes	= 4			// 4 Main Link Lanes
} DP_MainLinkNumLanes_t;


typedef enum
{
   DP_PowerState_Off	,
   DP_PowerState_On,
   DP_PowerState_Standby,
   DP_PowerState_Sleep,
   DP_PowerState_Invalid		= 255
} DP_PowerState_t;

typedef enum
{
	DP_CablePlugStatus_Unplugged,
	DP_CablePlugStatus_Plugged
} DP_CablePlugStatus_t;

// ClockType bit field
typedef enum
{
	DP_MsaClockType_Async			= 0x0,
	DP_MsaClockType_Sync			= 0x1
} DP_MsaClockType_t;

// ColorEncodingFormat bit field
typedef enum
{
	DP_MsaColorFormat_RGB				= 0x0,
	DP_MsaColorFormat_xvYCC422Bt601	= 0x1,
	DP_MsaColorFormat_xvYCC444Bt601	= 0x2,
	DP_MsaColorFormat_WideFixedRGB	= 0x3,
	DP_MsaColorFormat_CeaRGB			= 0x4,
	DP_MsaColorFormat_YCbCr422Bt601	= 0x5,
	DP_MsaColorFormat_YCbCr444Bt601	= 0x6,
	DP_MsaColorFormat_DCI_P3			= 0x7,
	DP_MsaColorFormat_xvYCC422Bt709	= 0x9,
	DP_MsaColorFormat_xvYCC444Bt709	= 0xA,
	DP_MsaColorFormat_WideFloatRGB	= 0xB,
	DP_MsaColorFormat_AdobeRGB		= 0xC,
	DP_MsaColorFormat_YCbCr422Bt709	= 0xD,
	DP_MsaColorFormat_YCbCr444Bt709	= 0xE,
	DP_MsaColorFormat_ColorProfile		= 0xF
} DP_MsaColorFormat_t;


typedef enum
{
	DP_MsaColorMode_All	= 0x0,
	DP_MsaColorMode_YOnly	= 0x1
} DP_MsaColorMode_t;

// ColorDepth bit field
typedef enum
{
	DP_MsaVideoMode_Progressivel	= 0x0,
	DP_MsaVideoMode_Interlaced	= 0x1
} DP_MsaVideoMode_t;
	
typedef enum
{
	DP_MsaHsyncPolarity_Positive	= 0x0,
	DP_MsaHsyncPolarity_Negative	= 0x1
} DP_MsaHsyncPolarity_t;

typedef enum
{
	DP_MsaVsyncPolarity_Positive	= 0x0,
	DP_MsaVsyncPolarity_Negative	= 0x1
} DP_MsaVsyncPolarity_t;


// ColorDepth bit field
typedef enum
{
	DP_MsaColorDepth_6Bits		= 0x0,
	DP_MsaColorDepth_8Bits		= 0x1,
	DP_MsaColorDepth_10Bits	= 0x2,
	DP_MsaColorDepth_12Bits	= 0x3,
	DP_MsaColorDepth_16Bits	= 0x4
} DP_MsaColorDepth_t;


// VTotalEven bit field
typedef enum
{
	DP_MsaVTotalOddEven_Odd	= 0x0,
	DP_MsaVTotalOddEven_Even	= 0x1
} DP_MsaVTotalOddEven_t;


// StereoVideoAttribute bit field
typedef enum
{
	DP_MsaStereoModeType_No3d		= 0x0,
	DP_MsaStereoModeType_Right		= 0x1,
	DP_MsaStereoModeType_Reserved	= 0x2,
	DP_MsaStereoModeType_Left			= 0x3
} DP_MsaStereoModeType_t;


//Audio Ports
typedef enum
{
	DP_AudioPort_I2s,						
	DP_AudioPort_Spdif,						
} DP_AudioPort_t;

typedef enum
{
	DP_SdpInfoType_Ats	= 0x01, // Audio time stamp
	DP_SdpInfoType_AStr = 0x02, // Audio Stream, will handed in SDP_Audio module
	DP_SdpInfoType_Ext = 0x04, // Extended packet
	DP_SdpInfoType_ACRM = 0x05, // Audio Copy Right Management
	DP_SdpInfoType_ISRC = 0x06, // ISRC refer to 1.2 SPEC
	DP_SdpInfoType_Vsc 	= 0x07, // Video Stereo config
	DP_SdpInfoType_Vsi 		= 0x81, 
	DP_SdpInfoType_Avi 	= 0x82, 
	DP_SdpInfoType_Spd 	= 0x83, 
	DP_SdpInfoType_Aif 		= 0x84, 
	DP_SdpInfoType_Mpg 	= 0x85,
}DP_SdpInfoType_t;

typedef enum
{
	DP_AudioCodingType_NONE=0,
	DP_AudioCodingType_PCM=1,
	DP_AudioCodingType_AC3=2,
	DP_AudioCodingType_MPEG1=3,
	DP_AudioCodingType_MP3=4,
	DP_AudioCodingType_MPEG2=5,
	DP_AudioCodingType_AAC=6,
	DP_AudioCodingType_DTS=7,
	DP_AudioCodingType_ATRAC=8,
	DP_AudioCodingType_DSD=9,
	DP_AudioCodingType_DDPlus=10,
	DP_AudioCodingType_DTS_HD=11,
	DP_AudioCodingType_MAT=12,
	DP_AudioCodingType_DST=13,
	DP_AudioCodingType_WMA_PRO=14,
	DP_AudioCodingType_RESERVED=15
} DP_AudioCodingType_t;

typedef enum
{
	DP_AudioSampleSize_None=0,
	DP_AudioSampleSize_16Bits=1,
	DP_AudioSampleSize_20Bits=2,
	DP_AudioSampleSize_24Bits=3,
	DP_AudioSampleSize_17Bits,
	DP_AudioSampleSize_18Bits,
	DP_AudioSampleSize_19Bits,
	DP_AudioSampleSize_21Bits,
	DP_AudioSampleSize_22Bits,
	DP_AudioSampleSize_23Bits,
	DP_AudioSampleSize_32Bits
} DP_AudioSampleSize_t;


typedef enum
{
	DP_AudioSamplingFrequency_None=0,
	DP_AudioSamplingFrequency_32_Khz=1,
	DP_AudioSamplingFrequency_44_1_Khz=2,
	DP_AudioSamplingFrequency_48_Khz=3,
	DP_AudioSamplingFrequency_88_2_Khz=4,
	DP_AudioSamplingFrequency_96_Khz=5,
	DP_AudioSamplingFrequency_176_4_Khz=6,
	DP_AudioSamplingFrequency_192_Khz=7,
	DP_AudioSamplingFrequency_22_05_Khz=8,
	DP_AudioSamplingFrequency_24_Khz=9,
	DP_AudioSamplingFrequency_768_Khz=10	
}DP_AudioSamplingFrequency_t;


typedef enum
{
	DP_AudioStreamType_Unknown,
	DP_AudioStreamType_Iec,
	DP_AudioStreamType_Dsd,
	DP_AudioStreamType_Dst,
	DP_AudioStreamType_Hbr
} DP_AudioStreamType_t;

typedef enum
{
	DP_AudioType_Compressed,
	DP_AudioType_LPCM
}DP_AudioType_t;

typedef enum
{
	DP_PortType_DigitalTx ,
	DP_PortType_MstCapableTx,
	DP_PortType_SstOnlyTx,
	DP_PortType_MstCapableRx,
	DP_PortType_SstOnlyRx,
	DP_PortType_DigitalRx,
} DP_PortType_t;

typedef enum
{
	DP_PortState_Active,
	DP_PortState_TransitionToActive,
	DP_PortState_TransitionToInActive,
	DP_PortState_InActive,
}DP_PortState_t;

typedef enum
{
	DP_RoutingLogicPortNum_0,
	DP_RoutingLogicPortNum_1,
	DP_RoutingLogicPortNum_2,
	DP_RoutingLogicPortNum_3
	
}DP_RoutingLogicPortNum_t;

typedef enum
{
	STDP_3D_LEFT_VIEW = 0,	
	STDP_3D_RIGHT_VIEW,
} STDP_3DView_t;

typedef enum
{
	STDP_3D_VSC_METHOD_NON_STEREO_VIDEO = 0,	
	STDP_3D_VSC_METHOD_FRAME_SEQUENTIAL,
	STDP_3D_VSC_METHOD_STACKED_FRAME,
	STDP_3D_VSC_METHOD_PIXEL_INTERLEAVED,
	STDP_3D_VSC_METHOD_SIDE_BY_SIDE,
	STDP_3D_VSC_METHOD_RESERVED,
} STDP_3DVSCMethod_t;

typedef enum
{
	STDP_3D_FORMAT_NO_3D_VIDEO = 0,
	STDP_3D_FORMAT_UNKNOWN,		
	STDP_3D_FORMAT_FRAME_SEQ_MISC,
	STDP_3D_FORMAT_FRAME_SEQ_RIGHT,
	STDP_3D_FORMAT_FRAME_SEQ_LEFT,
	STDP_3D_FORMAT_STACKED_FRAME,
	STDP_3D_FORMAT_INTERLEAVED_RIGHT_EVEN,
	STDP_3D_FORMAT_INTERLEAVED_RIGHT_ODD,
	STDP_3D_FORMAT_INTERLEAVED_CHECKERBOARD,
	STDP_3D_FORMAT_INTERLEAVED_VERTICAL_LEFT,
	STDP_3D_FORMAT_INTERLEAVED_VERTICAL_RIGHT,	
	STDP_3D_FORMAT_SBS_RHALF_RIGHT,
	STDP_3D_FORMAT_SBS_RHALF_LEFT,
	STDP_3D_FORMAT_LAST_VALUE = STDP_3D_FORMAT_SBS_RHALF_LEFT,
} STDP_3DFormat_t;

typedef void far*		DP_BaseAddr_t;

//******************************************************************************
//  T Y P E D E F S   ( F U N C T I O N   P O I N T E R S )
//******************************************************************************

typedef void far (* DP_Ext_Sha1Reset_t)(void);

typedef void far (* DP_Ext_Sha1Input_t)(const U8 far * Bp_Message, U16 W_Length);

typedef U8 DP_RAM * far (* DP_Ext_Sha1Result_t)(void);

typedef void far (* DP_Ext_DpConfigChange_t)(U16 ConfigParam_8);

typedef BOOL far (* DP_Ext_GetLogicalPortBksv_t)(U8 * Bksv_p8);

//******************************************************************************
//  T Y P E D E F S   ( S T R U C T S )
//******************************************************************************
#define CommonPortStructureEntries								\
		U8							PortId;						\
		U8							PortNum;					\
		DP_PortType_t				PortType;					\
		DP_RoutingLogicPortNum_t	RoutingLogicPortNum_e;		\
		DP_PortState_t				PortState_e;			    \
		U32							NextListItem;				\
		

typedef struct PortStructure_S
{
	CommonPortStructureEntries
	
} PortStructure_t;

typedef struct
{
	U16		DpClockType_bf1			: 1;
	U16		DpColorFormat_bf4			: 4;
	U16		DpColorDepth_bf3			: 3;
	U16		DpVTotalEvenOdd_bf1		: 1;
	U16		DpStereoMode_bf2			: 2;
	U16		DpMsaReserved_bf4			: 4;
	U16		DpMsaColorMode_bf1			: 1;
} DP_MainStreamVideoAttributesBF_t;

typedef union
{
	U16									Msa_16;
	DP_MainStreamVideoAttributesBF_t		Msa_bfs;
} DP_MainStreamVideoAttributes_t;

typedef struct
{
	U16		Value_bf15				: 15;
	U16		Polarity_bf1				: 1;
} DP_PolarityValue_t;

typedef struct
{
	U32								PixelClockHz_32;
	U16								HTotal_16;
	U16								HActiveStart_16;
	U16								HActiveWidth_16;
	DP_PolarityValue_t					HSyncWidth_16;
	U16								VTotal_16;
	U16								VActiveStart_16;
	U16								VActiveWidth_16;
	DP_PolarityValue_t					VSyncWidth_16;
	DP_MainStreamVideoAttributes_t		MsaFlags_16;
	DP_MsaVideoMode_t				MsaVideoMode_e;
	STDP_3DFormat_t					Ms3DFormat;
} DP_MsaParameters_t;

typedef struct
{
	BOOL				IsUpdated_b;
	U16					PacketSize_16;
	U8					PacketID_8;   
	DP_SdpInfoType_t		PacketType_e;    
	U8					DpVersion_8;
	U8					PacketData_8[DPRX_MAX_SDP_SIZE];
}DP_SdpInfoFrame_t;

typedef struct
{
	U8							ChannelCount_8;
	U8							ChannelAllocation_8;
	DP_AudioCodingType_t		   	CodingType_e;	
	DP_AudioSamplingFrequency_t	SamplingFrequency_e;
	DP_AudioSampleSize_t			SampleSize_e;
	DP_AudioPort_t				AudioPort_e;
	DP_AudioStreamType_t			AudioStreamType_e;
	DP_AudioType_t				AudioType_e;
	U8							AudioDacsUsed_8;
} DP_AudioConfig_t;


typedef enum
{
	AUX_REQ_NEW,					// Request is newly initiated
	AUX_REQ_SENT,					// When the Request or part of request
									//  is sent over the AUX 
	AUX_REP_ACK,					// When the reply rxd for the request is ACK
	AUX_REP_NACK,					// When Reply = NACK
	AUX_REP_DEFER,					// When Reply = DEFER
	AUX_REQ_FAIL,					// This status is returned when AUX request has failied
									//  (Nack, deffered and retry count is 0, buffers not allocated correctly)
	AUX_REQ_SUCCESS,				// Reply = ACK and the request is handled completely
	AUX_REQ_STATUS_CHANGE = 0x80
}Aux_Transaction_Status_t;
typedef struct
{
	U8							InProgress;
	U32 						Addr_32;					// Holds the Address in the AUX request
	BOOL						ReadRequest;				// DP_TRUE if I2C transaction is a read request
	U8 							Cmd_8;						// Holds the command
	U16 						RequestDataLen_16;			// Holds the length of the requests
	U16 						HandledDataLen_16;			// Holds the number of bytes transferred in AUX
    U8 far * 					ReqBuff_p;					// Buff which hold the request content
	Aux_Transaction_Status_t 	AuxTransactionStatus_t;		// Status of the Request
	U8						 	Retrycounter_8;				// Hold the retry count
	U16						 	Length_8;					// Holds the Number of bytes to read 
	U8 far *					RepBuff_p;  				//	Pointer where the reply should be stored
	U8							NumberOfReads_8; 
}Aux_data_buffer_t;


//******************************************************************************
//  V A R I A B L E   E X T E R N S
//******************************************************************************

//******************************************************************************
//  F U N C T I O N   E X T E R N S
//******************************************************************************

//******************************************************************************
//  F U N C T I O N   P R O T O T Y P E S
//******************************************************************************

//External Functions (Application side should fill up these functions)

void EXT_FUNC DP_Ext_SendEndOfInterrupt(void);

U32 EXT_FUNC DP_Ext_GetSystemTime (void);

void  EXT_FUNC DP_Ext_Delay1Us(U32 count_32);

void  EXT_FUNC DP_Ext_Print(const far char *String_p8, U16 Value_16);

void DP_EXT_FUNC DP_ConfigLib (DP_IpVersion_t DP_IpVersion_e);

//SHA-1 functions prototypes
void EXT_FUNC STDPDpRx_ExtSha1Reset(void);
void EXT_FUNC STDPDpRx_ExtSha1Input(const U8 DP_RAM* Message_p8, U16 Length_16);
U8 DP_RAM* EXT_FUNC STDPDpRx_ExtSha1Result(void);
U32 EXT_FUNC GetRepeaterId(U8 PortId_8);

void DP_EXT_FUNC DP_EnablePrints(BOOL Enable_b);

void DP_EXT_FUNC DP_SSTInAudioReset(U8 PortId);

#if (DP_HUB == ENABLED)
U8 DP_EXT_FUNC	LogicalPortCableStatus(void);
void DP_EXT_FUNC    ClearLogicalPortHPDEvent(void);
void  gm_2WireSetTimeout(U16 W_Timeout);
unsigned int  NVRam_ReadDPCD(U32 W_RegAddr, U8 *Bp_Buf, U16 W_Length);
unsigned int	gm_2WireWriteAddrBuffer(U8 B_DevAddr, U8 far *Bp_RegAddr, U8 B_AddrLen, U8 far *Bp_Data, U16 W_Len);
#endif
#pragma option -b.
#ifdef __PARADIGM__
#pragma pack(pop)
#endif
#endif		// STDP_H

