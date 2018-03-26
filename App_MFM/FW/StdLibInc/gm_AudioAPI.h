#ifndef __GM_AUDIO_API_H__
#define __GM_AUDIO_API_H__

//#define DEBUG_AUDIO
#ifdef DEBUG_AUDIO
   #define gm_AUDIO_Print  gm_Print
#else
   #define gm_AUDIO_Print  (void)
#endif

// audio input select rule for OSD
   typedef enum
   {
      gmd_AUDIO_SRC_AUTO_Main,
      gmd_AUDIO_SRC_I2S_IN,
      gmd_AUDIO_SRC_SPDIF_IN,
      gmd_AUDIO_SRC_LINE_IN,     // 1122 new added
      gmd_AUDIO_SRC_AUTO_Pxp,
   } gmt_AUDIO_SOURCE_MODE;

// audio output select rule for OSD
   typedef enum
   {
      gmd_AUDIO_TAR_LINE_OUT,
      gmd_AUDIO_TAR_HeadPhone_OUT,
      gmd_AUDIO_TAR_SPDIF_OUT,
      gmd_AUDIO_TAR_I2S_OUT,
      gmd_AUDIO_TAR_DPTx_OUT,
   } gmt_AUDIO_TARGET_MODE;

// define of audio input port
   typedef enum
   {
      gmd_AUDIO_IN_I2S_IN,
      gmd_AUDIO_IN_HDMI_I2S,
      gmd_AUDIO_IN_SPDIF_IN,
      gmd_AUDIO_IN_DPRx1_I2S,
      gmd_AUDIO_IN_DPRx2_I2S,
      gmd_AUDIO_IN_DAC_TPG,
      gmd_AUDIO_IN_AVS,
      gmd_AUDIO_IN_TPG,
      gmd_AUDIO_IN_LINE_IN,
      gmd_AUDIO_IN_UNKNOWN,
      gmd_AUDIO_IN_MAX_PORT = gmd_AUDIO_IN_UNKNOWN,
   }  gmt_AUDIO_INPORT_TYPE;

// define of audio output port
   typedef enum
   {
      gmd_AUDIO_OUT_I2S_OUT,
      gmd_AUDIO_OUT_SPDIF_OUT,
      gmd_AUDIO_OUT_DPTx_OUT,
      gmd_AUDIO_OUT_HeadPhone_OUT,
      gmd_AUDIO_OUT_LINE_OUT,
      gmd_AUDIO_OUT_UNKNOWN,
      gmd_AUDIO_OUT_MAX_PORT = gmd_AUDIO_OUT_UNKNOWN,
   }  gmt_AUDIO_OUTPORT_TYPE;

   typedef enum
   {
      gmd_AUDIO_CODING_UNSUPPORTED = BIT0,
      gmd_AUDIO_CODING_PCM         = BIT1,
      gmd_AUDIO_CODING_AC3         = BIT2,
      gmd_AUDIO_CODING_DTS         = BIT3,
      gmd_AUDIO_CODING_DEFAULT     = gmd_AUDIO_CODING_PCM | gmd_AUDIO_CODING_DTS | gmd_AUDIO_CODING_AC3,
   } gmt_AUDIO_CODING_TYPE;

   typedef struct
   {
      WORD CodingType;
      DWORD SampleRate_Min;
      DWORD SampleRate_Max;
      BYTE SampleWidth_Min;
      BYTE SampleWidth_Max;
      BYTE ChannelCount_Min;
      BYTE ChannelCount_Max;
   } gmt_AUDIO_CAPABILITY_INFO;

// --------------------------------
// Declaration for local function
// --------------------------------
                        void SYS_API_AUDIO_SOFT_RESET(void);
                        void SYS_API_AUDIO_HARD_RESET(void);

                        void SYS_API_AUDIO_POWER_UP(void);
                        void SYS_API_AUDIO_POWER_DOWN(void);

                        void SYS_API_AUDIO_SafeLeaving(void);
                        void SYS_API_AUDIO_Enable_Interrupt(gmt_PHY_CH B_Channel);
                        void SYS_API_AUDIO_AVS_SetDelayOnOff(BOOL Enable);

       gmt_AUDIO_CODING_TYPE SYS_API_AUDIO_GetCurrentCodingType(gmt_PHY_CH B_Channel);
                       DWORD SYS_API_AUDIO_GetCurrentSampleRate(gmt_PHY_CH B_Channel);
                        BYTE SYS_API_AUDIO_GetCurrentInputChannelCount(gmt_PHY_CH B_Channel);
                        BYTE SYS_API_AUDIO_GetCurrentInputBits(gmt_PHY_CH B_Channel);
       gmt_AUDIO_TARGET_MODE SYS_API_AUDIO_GetCurrentOutput(gmt_PHY_CH B_Channel);

                        void SYS_API_AUDIO_HDMI_Set_AVMUTE(void);
                        void SYS_API_AUDIO_HDMI_Clear_AVMUTE(void);
                        void SYS_API_AUDIO_HDMI_Set_PropertyChange(void);
                        void SYS_API_AUDIO_HDMI_Clear_PropertyChange(void);
                        void SYS_API_AUDIO_DPRx_Set_AVMUTE(void);
                        void SYS_API_AUDIO_DPRx_Clear_AVMUTE(void);
                        void SYS_API_AUDIO_DPRx_Set_PropertyChange(void);
                        void SYS_API_AUDIO_DPRx_Clear_PropertyChange(void);

                        void SYS_API_AUDIO_Cus_UnMUTE(gmt_PHY_CH B_Channel);
                        void SYS_API_AUDIO_Cus_MUTE_By_OSD(gmt_PHY_CH B_Channel);

                        void SYS_API_AUDIO_Adjuster(void);

                        BOOL SYS_API_AUDIO_UnMute_Apply_Check(gmt_PHY_CH B_Channel);

                       DWORD SYS_API_AUDIO_Get_SyncLostTime(gmt_PHY_CH B_Channel);
                        void SYS_API_AUDIO_Set_Capability_I2S_IN(gmt_AUDIO_CAPABILITY_INFO * support);
                        void SYS_API_AUDIO_Set_Capability_SPDIF_IN(gmt_AUDIO_CAPABILITY_INFO * support);
                        void SYS_API_AUDIO_Set_Capability_HDMI_IN(gmt_AUDIO_CAPABILITY_INFO * support);
                        void SYS_API_AUDIO_Set_Capability_DPRx1_IN(gmt_AUDIO_CAPABILITY_INFO * support);
                        void SYS_API_AUDIO_Set_Capability_DPRx2_IN(gmt_AUDIO_CAPABILITY_INFO * support);
                        void SYS_API_AUDIO_Set_DAC_VolumeStep(BYTE step);
                        void SYS_API_AUDIO_Set_DAC_GainMUL(BYTE GainMUL);
                        void SYS_API_AUDIO_Set_VideoSynchronized(gmt_PHY_CH B_Channel, BOOL flag);

                        BOOL SYS_API_AUDIO_Is_DAC_VolumeSetupDone(void);
                        BOOL SYS_API_AUDIO_Is_Input_AC3(gmt_PHY_CH B_Channel);
                        BOOL SYS_API_AUDIO_Is_Video_Synchronized(gmt_PHY_CH B_Channel);
                        BOOL SYS_API_AUDIO_Is_SyncLost(gmt_PHY_CH B_Channel);
                        BOOL SYS_API_AUDIO_Is_SyncDetected(gmt_PHY_CH B_Channel);
                        BOOL SYS_API_AUDIO_Is_SetupDone(gmt_PHY_CH B_Channel);
                        BOOL SYS_API_AUDIO_Is_Fast_To_NextPort(void);
                        BOOL SYS_API_AUDIO_Is_AVS_Changing(void);
                        BOOL SYS_API_AUDIO_Is_AVMUTE(gmt_PHY_CH B_Channel);

                        void SYS_API_AUDIO_Output_UnMUTE(gmt_PHY_CH B_Channel);

                        void SYS_API_AUDIO_Handler(gmt_PHY_CH B_Channel);

                        void SYS_API_AUDIO_Initialize(void);
#endif
