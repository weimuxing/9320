/*
	$Workfile:   gm_Decoder_Stdp93xx.h  $
	$Revision: 1.4 $
	$Date: 2011/12/13 10:32:44 $
*/

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
// MODULE:      gm_Decoder.h
//
// USAGE:
//
//******************************************************************************
#ifndef __GM_DECODER_H
#define __GM_DECODER_H

#define NUM_DECODER_VIDEO_MODES	7
#define SINGLE_DECODER
#define gmd_DEC_CORTEZ_ID  gmd_DEC_CORTEZ_ID1    //  for  compatiblity with application code
#define gmd_DEC_HUDSON_ID  gmd_DEC_CORTEZ_ID1    //  since application  do not  want to change the name

typedef enum
{

#ifdef SINGLE_DECODER
   gmd_DEC_CORTEZ_ID1,
#else
   gmd_DEC_CORTEZ_ID1,
   gmd_DEC_CORTEZ_ID2,
#endif  // SINGLE_DECODER

   gmd_DEC_NUM_DECODERS
}gmt_DECODER_ID;



typedef enum
{
   gmd_DEC_SIGNAL_PRESENT			= BIT0,
   gmd_DEC_MODE_CHANGE				= BIT1,
   gmd_DEC_COLOR_BURST_PRESENT		= BIT2,
   gmd_DEC_SUB_CARRIER_LOCK_STATUS	= BIT3,
   gmd_DEC_LINE525_625_MODE		= BIT4,
   gmd_DEC_COLOR_KILL_STATUS		= BIT5,
   gmd_DEC_3D_COMB_STATUS			= BIT6,
   gmd_DEC_VHS_FLAG_STATUS			= BIT7,
   gmd_DEC_MACROVISION_MODE		= BIT8 | BIT9 | BIT10,
   gmd_DEC_STABLE_UNSTABLE_STATUS,
   gmd_DEC_NOISE_VALUE,
   gmd_DEC_NOISE_BIN,
   gmd_DEC_COLOR_BURST_VALUE,
   gmd_DEC_SYNC_VALUE,
   gmd_DEC_SIGNAL_VALUE,
   gmd_DEC_LUMA_VALUE,
   gmd_DEC_COLOR_BURST_WIDTH,
   gmd_DEC_SUB_CARRIER_FREQ,
   gmd_DEC_HSYNC_WIDTH,
   gmd_DEC_HSYNC_FREQ,
   gmd_DEC_LINECOUNT,
   gmd_DEC_TBC_HLENGTH,
   gmd_DEC_VHS_TRICK_FLAG_STATUS	= BIT11,
   gmd_DEC_NO_SYNC					= BIT12,
   gmd_DEC_UNSUPPORTED_MODE		= BIT13,
   gmd_DEC_FREERUN_STATUS			= BIT14
}gmt_DECODER_STATUS;

typedef enum
{
   gmd_DEC_TYPE0_NO_MACROVISON,                                  // 0      0     0     0
   gmd_DEC_TYPE1_PSEUDO_SYNC,                                    // 0      0     0     1
   gmd_DEC_TYPE1_EOF_AGC,                                        // 0      0     1     0
   gmd_DEC_TYPE1_EOF_AGC_AND_PSEUDO_SYNC,                        // 0      0     1     1
///For  SEQUOIA onwards
   gmd_DEC_TPYE2_2LINE_CLR_STRIPE,                               // 0      1     0     0
   gmd_DEC_TYPE2_2LINE_CLR_STRIPE_AND_PSEUDO_SYNC,               // 0      1     0     1
   gmd_DEC_TYPE2_2LINE_CLR_STRIPE_AND_EOF_AGC,                   // 0      1     1     0
   gmd_DEC_TYPE2_2LINE_CLR_STRIPE_AND_EOF_AGC_AND_PSEUDO_SYNC,   // 0      1     1     1
   gmd_DEC_TYPE3_4LINE_CLR_STRIPE,                               // 1      0     0     0
   gmd_DEC_TYPE3_4LINE_CLR_STRIPE_AND_PSEUDO_SYNC,               // 1      0     0     1
   gmd_DEC_TYPE3_4LINE_CLR_STRIPE_AND_EOF_AGC,                   // 1      0     1     0
   gmd_DEC_TYPE3_4LINE_CLR_STRIPE_AND_EOF_AGC_AND_PSEUDO_SYNC,   // 1      0     1     1
///For  Cortez Advacned, Plus, chapin, Torino, H2, there is not  distinction of  2 or  4  lines color  stripe
   //CLR     AGC  PSEUDO
   gmd_DEC_CLR_STRIPE,                               // 1     0     0
   gmd_DEC_CLR_STRIPE_AND_PSEUDO_SYNC,               // 1     0     1
   gmd_DEC_CLR_STRIPE_AND_EOF_AGC,                   // 1     1     0
   gmd_DEC_CLR_STRIPE_AND_EOF_AGC_AND_PSEUDO_SYNC,   // 1     1     1
}gmt_DECODER_MACROVISION_STATUS;

typedef enum
{
   gmd_DEC_LINE625_MODE = 0,
   gmd_DEC_LINE525_MODE = 1,
   gmd_DEC_LINE_MODE_INIT = 2
}gmt_LINE525_625_MODE;

typedef enum
{
   SWAGC = 0,
   AGC_OFF = 1,
   DAGC = 2

}gmt_AGCMethod;

typedef enum
{
   gmd_DEC_STATUS_LOW_BIN,
   gmd_DEC_STATUS_MED_BIN,
   gmd_DEC_STATUS_HIGH_BIN,
}gmt_DECODER_STATUS_BINS;

//Decoder Video Standards Supported
typedef enum
{
   gmd_DEC_MODE_NTSC,
   gmd_DEC_MODE_PAL,
   gmd_DEC_MODE_PAL_N,
   gmd_DEC_MODE_PAL_M,
   gmd_DEC_MODE_SECAM,
   gmd_DEC_MODE_NTSC_443,
   gmd_DEC_MODE_PAL_60,
   gmd_DEC_MODE_UNKNOWN_525,
   gmd_DEC_MODE_UNKNOWN_625,
   gmd_DEC_MODE_UNKNOWN
}gmt_DECODER_VIDEO_MODE;




typedef enum
{
   gmd_DEC_MODE_STABLE,
   gmd_DEC_MODE_UNSTABLE,
}gmt_DECODER_MODE_STATUS;


typedef  enum
{
   gmd_DEC_SOFTRESET_BLOCKS,
   gmd_DEC_SOFTRESET_REGS,
   gmd_DEC_SOFTRESET_PORT
}gmt_DECODER_RESET_MODE;



typedef enum
{
   gmd_DECODER_POSITIVE_CLAMP,
   gmd_DECODER_NEGATIVE_CLAMP,
   gmd_DECODER_NEGATIVE_PEAK_CLAMP,
   gmd_DEC_NEGATIVE_WITH_NPC
}gmt_DECODER_CLAMPING_TYPE;

//Chaplin application decalarations start
typedef enum
{
   gmd_JAPAN_NTSC_ENABLE,
   gmd_AGC_ENABLE,
   gmd_FREE_RUN_MODE_SELECTION,
   gmd_FREE_RUN_CLAMP_MODE_SELECTION,
   gmd_TBC_OUTDELAY_MANUAL_MODE_ENABLE,
   gmd_SECAM_FILTER_SEL,
   gmd_SELECTIVE_MODE_DETECTION_ENABLE,
}st_AVDecoderUserConfigFlags;
typedef enum
{
   gmd_DEC_FREE_RUN_AUTO_MODE = 0,
   gmd_DEC_FREE_RUN_MANUAL_MODE,
}st_AVDecoderFreeRunMode;
typedef enum
{
   gmd_DEC_CLAMP_AUTO_MODE = 0,
   gmd_DEC_CLAMP_MANUAL_MODE,
}st_AVDecoderFreeRunClampMode;
typedef enum
{
   gmd_DEC_SECAM_SHARP_FILTER = 0,
   gmd_DEC_SECAM_SOFT_FILTER,
   gmd_DEC_SECAM_ADAPTIVE_FILTER
}gmt_AVDEC_SECAM_FILTER_SEL;
typedef struct
{
   WORD NtscMdelay;
   WORD PalIdelay;
   WORD PalNdelay;
   WORD PALMdelay;
   WORD Secamdelay;
   WORD Ntsc443Delay;
   WORD Pal60Delay;
}gm_AVDecoderTbcdelay;
typedef struct
{
   gm_AVDecoderTbcdelay *ptrCompositeTBCDelays;
   gm_AVDecoderTbcdelay *ptrSvideoTBCDelays;
   BYTE ID;
} gm_AvDecoderConfigTbcDelays;
typedef struct
{
   BOOL B_JapanNTSCEnable ;                   //0 for US NTSC, 1  for Japan  NTSC
   BOOL B_AGCEnable;
   st_AVDecoderFreeRunMode B_FreeRunAutoEnable;//st_AVDecoderFreeRunMode B_FreeRunAutoEnable;
   st_AVDecoderFreeRunClampMode B_FreeRunClampAutoEnable;//st_AVDecoderFreeRunClampMode B_FreeRunClampAutoEnable;
   BOOL B_TBCOutDelayManualControlEnable;      //0 :   auto  mode, 1: manual mode
   gmt_AVDEC_SECAM_FILTER_SEL B_SECAMFilterSel;//B_SECAMSoftFilterEnable;       // 0 for Sharp  filter, 1  for Soft  filter
   //gmt_AVDEC_SECAM_FILTER_SEL B_SECAMFilterSel;
   BOOL B_SelectiveModeDetecionEnable;   //0 Automatic all mode  selection, 1 - modes to
   BYTE ID;//BYTE ID;
} gm_AVDecoderConfig;
typedef struct COLOR_KILL_LIMIT
{
   BYTE B_KillLevel;
   BYTE B_RecoverLevel;
   BYTE ID;
}gm_AVDECODER_COLOR_KILL;
typedef struct
{
   BYTE B_PortSignalType;
   BYTE *B_AdcUsagePtr;
   BYTE B_InputConnector;
   BYTE ID;
}gm_AvDecoderConfigAfe;
typedef enum
{
   gmd_COLOR_KILL_VALUE,
   gmd_COLOR_RECOVER_VALUE,
}st_AVDecoderColorKillRecoverVal;
typedef enum
{
   gmd_TUNER_STATUS_OK = 0,
   gmd_TUNER_STATUS_NOT_OK
}gm_AVDECODER_TUNER_STATUS;

void gm_AVDecoderConfigTBcOutdelays(gmt_DECODER_ID B_VidDecId, gm_AvDecoderConfigTbcDelays *pTBCPointer);
void gm_AvDecoderConfig (gmt_DECODER_ID B_VidDecId, gm_AVDecoderConfig *DecoderConfig);
void gm_AVDecoderColorKill (gmt_DECODER_ID B_VidDecId, gm_AVDECODER_COLOR_KILL *pColorKillPointer);
gm_AVDECODER_TUNER_STATUS gm_AVDecoderTunerSignalStatus(gmt_DECODER_ID B_VidDecId);
//Chaplin application decalarations end


typedef enum
{
   gmd_NTSC_SELECT		= BIT0,			// NTSC - 60Hz
   gmd_PAL_SELECT		= BIT1,			// PAL(B,D,G,H,I)  - 50Hz
   gmd_PAL_N_SELECT	= BIT2,			// PAL N
   gmd_PAL_M_SELECT	= BIT3,			// PAL M
   gmd_SECAM_SELECT	= BIT4,			// SECAM
   gmd_NTSC_443_SELECT = BIT5,   		// NTSC 4.43
   gmd_PAL_60_SELECT	= BIT6,			// PAL -60Hz
   gmd_ALL_MODES_SELECT = (BIT0|BIT1|BIT2|BIT3|BIT4|BIT5|BIT6)
}gmt_VIDEO_DECODER_MODE_SELECT;

typedef enum
{
   gmd_3D_FILTER_STANDARD,
   gmd_3D_FILTER_PREFER_MOTION,
   gmd_3D_FILTER_PREFER_STILL
}gmt_DECODER_3D_FILTER_SELECT;


typedef enum
{
   gmd_YC_FILTER_SELECT_AUTO = 0,
   gmd_YC_FILTER_SELECT_NARROW = 2,
   gmd_YC_FILTER_SELECT_WIDE = 3
}gmt_DECODER_YC_FILTER_SELECT;

typedef enum
{
   gmd_DEC_RAW_HSYNC,
   gmd_DEC_NCO_HSYNC,
   gmd_DEC_4FSC_HSYNC
}gmt_DECODER_HSYNC_SELECT;

typedef enum
{
   gmd_DEC_CKILL_AUTO,
   gmd_DEC_CKILL_ENABLE,
   gmd_DEC_CKILL_DISABLE
}gmt_DECODER_CKILL_SELECT;

typedef enum
{
   gmd_DEC_3D_AUTO,
   gmd_3D_MANUAL_ENABLE,
   gmd_3D_MANUAL_DISABLE
}gmt_DECODER_3D_CONTROL;

typedef enum
{
   gmd_RESET,
   gmd_ENABLE,
   gmd_DISABLE
} gmt_DEC_MEMORY;

typedef  enum
{
   DEC_AGC_FAST_RESPONSE,
   DEC_AGC_SLOW_RESPONSE
} AGC_RESPONSE_TYPE;

typedef enum
{
   gmd_DEC_NEW_BELL_WITH_HPF_CENTRE_FREQ_4p286MHz,
   gmd_DEC_NEW_BELL_WITH_BOOST2_CENTRE_FREQ_4p286MHz,
   gmd_DEC_NEW_BELL_WITH_BOOST1_CENTRE_FREQ_4p286MHz,
   gmd_DEC_NEW_BELL_PROG_2ND_AND_1ST_IIR_IN_CASCADE,
   gmd_DEC_NEW_BELL_MAX_SEL,
}gmt_BEL_FIL_SEL;

//CTI filter selection option is available from Sequoia onwards.
typedef enum
{
   gmd_DEC_CTI_HIGHGAINSETTINGS,
   gmd_DEC_CTI_DEFAULT,
   gmd_DEC_CTI_ENHANCED,
}gmt_CTI_FILTER_SEL;
typedef enum
{
   gmt_DEC_2D3DCOMBFILTER_DEFAULT,
   gmt_DEC_2D3DCOMBFILTER_ENHANCED,
}gmt_2D3DCOMBFILTER_SEL;

typedef enum
{
   gmd_DEC_RCLK_255,
   gmd_DEC_RCLK_235,
}gmt_DEC_RCLK_FREQ_SEL;
//Folllowing four decalartions are for  chaplin application
/*
BYTE gm_DecoderReadRegisterByte(gmt_DECODER_ID B_VidDecId, DWORD RegOffset);
void gm_DecoderWriteRegisterByte(gmt_DECODER_ID B_VidDecId, DWORD RegOffset, BYTE RegValue);
void gm_DecoderSetRegisterBits(gmt_DECODER_ID B_VidDecId, DWORD RegOffset, BYTE BitSignature);
void gm_DecoderResetRegisterBits(gmt_DECODER_ID B_VidDecId, DWORD RegOffset, BYTE BitSignature);
*/

//Folllowing four decalartions are for non chaplin applications
BYTE gm_DecoderReadRegisterByte(gmt_DECODER_ID B_VidDecId, WORD RegOffset);
void gm_DecoderWriteRegisterByte(gmt_DECODER_ID B_VidDecId, WORD RegOffset, BYTE RegValue);
void gm_DecoderSetRegisterBits(gmt_DECODER_ID B_VidDecId, WORD RegOffset, BYTE BitSignature);
void gm_DecoderResetRegisterBits(gmt_DECODER_ID B_VidDecId, WORD RegOffset, BYTE BitSignature);
BYTE gm_DecoderIndirectReadRegisterByte(gmt_DECODER_ID B_VidDecId, BYTE B_WhichModule,  BYTE B_RegIndex);
void gm_DecoderIndirectWriteRegisterByte(gmt_DECODER_ID B_VidDecId, BYTE B_WhichModule, BYTE B_RegIndex, BYTE B_DataToWrite);


gmt_DECODER_MODE_STATUS gm_DecoderIsModeStable(gmt_DECODER_ID B_VidDecId);
void gm_DecoderModeDetectionEnable(gmt_DECODER_ID B_VidDecId, BOOL B_EnableModeDetection);
void gm_DecoderForceMode(gmt_DECODER_ID B_VidDecId, gmt_DECODER_VIDEO_MODE B_Mode);
void gm_DecoderSetClamping(gmt_DECODER_ID B_VidDecId,gmt_DECODER_CLAMPING_TYPE B_ClampType);

gmt_DECODER_VIDEO_MODE gm_DecoderGetVideoMode(gmt_DECODER_ID B_VidDecId);
WORD gm_DecoderGetStatus(gmt_DECODER_ID B_VidDecId, gmt_DECODER_STATUS B_Status);

void gm_DecoderInit(gmt_DECODER_ID B_VidDecId);
void gm_DecoderReset(gmt_DECODER_ID B_VidDecId, gmt_DECODER_RESET_MODE B_ResetMode);

void gm_DecoderAGCEnable(gmt_DECODER_ID B_VidDecId, BOOL B_Enable);
void gm_DecoderFreeRunModeEnable(gmt_DECODER_ID B_VidDecId, BOOL B_Enable);

void gm_DecoderYCDelay(gmt_DECODER_ID B_VidDecId, SBYTE B_Delay);
void gm_DecoderSelectModesFordetection(gmt_DECODER_ID B_VidDecId, gmt_VIDEO_DECODER_MODE_SELECT B_VideoModes);

void gm_Decoder3DCombFilterSelect(gmt_DECODER_ID B_VidDecId, gmt_DECODER_3D_FILTER_SELECT B_FilterSelect);
void gm_Decoder3DControl(gmt_DECODER_ID B_VidDecId, gmt_DECODER_3D_CONTROL  B_3DControl);

void gm_DecoderYCSeparationFilterSelect(gmt_DECODER_ID B_VidDecId, gmt_DECODER_YC_FILTER_SELECT B_FilterSelect);
BYTE gm_DecoderTintControl(gmt_DECODER_ID B_VidDecId, SBYTE sb_Tint);
void gm_DecoderForceCkill(gmt_DECODER_ID B_VidDecId, gmt_DECODER_CKILL_SELECT B_CkillSelect);
void gm_DecoderDisableDFEClampLoop(gmt_DECODER_ID B_VidDecId, BYTE B_Enable);
void gm_DecoderSelectHSync(gmt_DECODER_ID B_VidDecId, gmt_DECODER_HSYNC_SELECT B_HSyncSelect);
void gm_Decoder4FscHsyncEnable(gmt_DECODER_ID B_VidDecId, BYTE B_Enable);
void gm_DecoderDisableAGC(gmt_DECODER_ID B_VidDecid, BYTE B_Disable);

//If VHS is overridden means VHS would always be FALSE i.e. not been used
//This is useful for Tuner inpurs
void gm_DecoderOverrideVHS(gmt_DECODER_ID B_VidDecid, BYTE B_OverRide);
void gm_DecoderCofigTBCForNTSC_J(gmt_DECODER_ID B_VidDecid);
BYTE gm_DecoderMemory(gmt_DECODER_ID B_VidDecId, gmt_DEC_MEMORY W_Mode);

void debugPrintStatus(gmt_DECODER_ID B_VidDecId);

void gmp_DecoderUpdateStatus(gmt_DECODER_ID B_VidDecId);

void gmp_DecoderAfeConfigInput(gmt_DECODER_ID B_VidDecId,BYTE B_InputConnector);

void gmp_DecoderAfeConfigInputChaplin(gmt_DECODER_ID B_VidDecId, gm_AvDecoderConfigAfe *pAfeConfigPointer );

void gm_DecoderGetAllIndexedRegValues(gmt_DECODER_ID B_VidDecId);
void gm_DecoderEnableDisableTunerIFCompensation(gmt_DECODER_ID B_VidDecId, BYTE B_Enable , WORD W_FreqCurveSel_0_15);
//B_AGCResponse  can be  DEC_AGC_FAST_RESPONSE  or DEC_AGC_SLOW_RESPONSE
//B_PeakWhiteHandleEnable can be  TRUE or FALSE
void gm_ConfigAGC(gmt_DECODER_ID B_VidDecId, BYTE B_AGCResponseSel, BYTE B_PeakWhiteHandleEnable );


BYTE gm_DecoderGetHNCOLockStatus(gmt_DECODER_ID B_VidDecId) ;
void gm_DecoderEnableAntiAliasingFilter(gmt_DECODER_ID B_VidDecId);
void gm_DecoderDisableAntiAliasingFilter(gmt_DECODER_ID B_VidDecId);
void  gm_DecoderHandler(gmt_DECODER_ID B_VidDecId);
BYTE gm_DecoderGetCBPresentLumaPath(gmt_DECODER_ID B_VidDecId);
void  gm_DecoderSyncAdcPowerDown(gmt_DECODER_ID B_VidDecId);
void  gm_DecoderSyncAdcPowerUp(gmt_DECODER_ID B_VidDecId);
//CTI filter selection option is available from Sequoia onwards.
void gm_DecoderCTIFilterSelect(gmt_DECODER_ID B_VidDecId, gmt_CTI_FILTER_SEL B_FilterSelect);
void gm_DecoderEnableCTI(gmt_DECODER_ID B_VidDecId);
void gm_DecoderDisableCTI(gmt_DECODER_ID B_VidDecId);
void gm_DecoderSelectSECAMBellFilter(gmt_DECODER_ID B_VidDecId,  BYTE B_Selection);
void gm_DecoderPALMParamVarSupportBeyond400Hz(gmt_DECODER_ID B_VidDecId, BYTE B_TrueOrFalse);

BYTE gm_DecoderGetMacrovisionStatus(gmt_DECODER_ID B_VidDecId);
void gm_Decoder2D3DCombFilterSelect(gmt_DECODER_ID B_VidDecId, gmt_2D3DCOMBFILTER_SEL B_FilterSelect);
void gm_DecoderSelectRCLKFrequency( gmt_DECODER_ID B_VidDecId, gmt_DEC_RCLK_FREQ_SEL B_FreqSelect );

#endif //__GM_DECODER_H
