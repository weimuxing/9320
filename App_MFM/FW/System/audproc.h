/*
	$Workfile:   audproc.h  $
	$Revision: 1.18 $
	$Date: 2013/04/01 08:48:14 $
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
// MODULE:	AUDPROC.H
//
// USAGE:	This file has Definitions for Audio Device.
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************



//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************

#ifndef AUDPROC_H_
#define AUDPROC_H_

#define D_MUTE_OFF 				FALSE
#define D_MUTE_ON 				TRUE

#define USE_EXT_SPDIF_DEVICE     DISABLE
#define USE_EXT_I2S_DEVICE       DISABLE
#define USE_EXT_HP_DEVICE        DISABLE
#define USE_EXT_SPEAKER_DEVICE   ENABLE

   typedef struct
   {
                       BOOL AutoScanEnable;     // enable/disable of autoscan
      gmt_AUDIO_INPORT_TYPE AutoScanIndex;      // port index for scanning
                       BOOL AutoScanApplied;    // new port applied

   } gmt_AUDIO_CUS_ADJUSTER;

#define AUDIO_IS_AS_ENABLE             (Audio_Get_Adjuster()->AutoScanEnable)
#define AUDIO_IS_AS_INDEX              (Audio_Get_Adjuster()->AutoScanIndex)
#define AUDIO_IS_AS_APPLIED            (Audio_Get_Adjuster()->AutoScanApplied)

       gmt_AUDIO_INPORT_TYPE AAPI_Audio_Get_CurrentInput(gmt_PHY_CH B_Channel);
      gmt_AUDIO_OUTPORT_TYPE AAPI_Audio_Get_CurrentOutput(gmt_PHY_CH B_Channel);
                        BYTE AAPI_Audio_Get_CurrentOutputBits(gmt_PHY_CH B_Channel);
                        BYTE AAPI_Audio_Get_CurrentOutputChannelCount(gmt_PHY_CH B_Channel);

                        WORD AAPI_Audio_Get_CLK_MUL(gmt_PHY_CH B_Channel);

                        BOOL AAPI_Audio_Get_AliveMode(void);
                  gmt_PHY_CH AAPI_Audio_Get_SourceChannel(void);

                        void AAPI_Audio_Cus_Mute(gmt_AUDIO_OUTPORT_TYPE output);
                        void AAPI_Audio_Cus_UnMute(gmt_AUDIO_OUTPORT_TYPE output);
                        void AAPI_Audio_Cus_Mute_1st(gmt_AUDIO_OUTPORT_TYPE output);
                        void AAPI_Audio_Cus_UnMute_1st(void);
                        void AAPI_Audio_Cus_Mute_By_OSD(gmt_AUDIO_OUTPORT_TYPE output);

                        void AAPI_Audio_Cus_Volume_Adjust(gmt_AUDIO_OUTPORT_TYPE output, BYTE vol);

                        void AAPI_Audio_Cus_Initialize(void);

                        void AAPI_Audio_Init_Adjuster(void);
                        BOOL AAPI_Audio_Adjuster(gmt_PHY_CH B_Channel);

                        void AAPI_Audio_AutoScan_Handler(void);

                        WORD AAPI_Audio_UnMUTE(gmt_PHY_CH B_Channel);
                      WORD * AAPI_Audio_Get_UnMUTE_Status(void);
                        void AAPI_Audio_UnMUTE_Status(WORD encode);

                        BOOL AAPI_Audio_IsVideoSetupCompleted(gmt_PHY_CH B_Channel);

                        void Audio_Cus_SPDIF_Ext_Ampflier_Volume_Adjust(BYTE vol);
                        void Audio_Cus_SPDIF_Ext_Ampflier_0_Volume(void);
                        void Audio_Cus_SPDIF_Ext_Ampflier_Mute_1st(void);
                        void Audio_Cus_SPDIF_Ext_Ampflier_Mute(void);
                        void Audio_Cus_SPDIF_Ext_Ampflier_UnMute(void);
                        void Audio_Cus_SPDIF_Ext_Ampflier_Mute_Initialize(void);

                        void Audio_Cus_I2S_Ext_Ampflier_Volume_Adjust(BYTE vol);
                        void Audio_Cus_I2S_Ext_Ampflier_0_Volume(void);
                        void Audio_Cus_I2S_Ext_Ampflier_Mute_1st(void);
                        void Audio_Cus_I2S_Ext_Ampflier_Mute(void);
                        void Audio_Cus_I2S_Ext_Ampflier_UnMute(void);
                        void Audio_Cus_I2S_Ext_Ampflier_Mute_Initialize(void);

                        void Audio_Cus_HP_Volume_Adjust(BYTE vol);
                        void Audio_Cus_HP_0_Volume(void);
                        void Audio_Cus_HP_Mute_1st(void);
                        void Audio_Cus_HP_Mute(void);
                        void Audio_Cus_HP_UnMute(void);
                        void Audio_Cus_HP_Mute_Initialize(void);

                        void Audio_Cus_Speaker_Volume_Adjust(BYTE vol);
                        void Audio_Cus_Speaker_0_Volume(void);
                        void Audio_Cus_Speaker_Mute_1st(void);
                        void Audio_Cus_Speaker_Mute(void);
                        void Audio_Cus_Speaker_UnMute(void);
                        void Audio_Cus_Speaker_Mute_Initialize(void);

                        void Audio_VideoSyncLost(gmt_PHY_CH B_Channel);
                        void Audio_VideoSyncDetected(gmt_PHY_CH B_Channel);

       gmt_AUDIO_INPORT_TYPE Audio_AutoScan_Get_NextPort(void);
                        void Audio_AutoScan_Get_NextIndex(void);
                        void Audio_AutoScan_Enable(void);
                        void Audio_AutoScan_Disable(void);

                        void Audio_Initialize(void);
                        void Audio_Handler(void);

    gmt_AUDIO_CUS_ADJUSTER * Audio_Get_Adjuster(void);

#endif
