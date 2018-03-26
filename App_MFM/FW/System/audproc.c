/*
   $Workfile:   audproc.c  $
   $Revision: 1.32 $
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
// MODULE:      IoExpander.c
//
// USAGE:
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "System\all.h"

#if (FEATURE_AUDIO == ENABLE)

//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************

//******************************************************************************
//  E X T E R N S
//******************************************************************************

//******************************************************************************
//  S T A T I C    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

//******************************************************************************
//  C O D E
//******************************************************************************

gmt_AUDIO_INPORT_TYPE AAPI_Audio_Get_CurrentInput(gmt_PHY_CH B_Channel)
{
   gmt_ConnTypes InputConnType;
   gmt_AUDIO_SOURCE_MODE source;

   InputConnType  = GetPortConnType(gmvb_InputConnector[B_Channel]);

   if (Audio_AutoScan_Get_NextPort() != gmd_AUDIO_IN_UNKNOWN)
   {
      return Audio_AutoScan_Get_NextPort();
   }

   if (B_Channel == gmvb_MainChannel)
   {
      source = GetUserPrefAudioMainInput();
	  
	  switch (source)
	  {
	  default:
	  case gmd_AUDIO_SRC_AUTO_Main:
	  
		 switch (InputConnType)
		 {
		 default:
		 case COMPOSITE:
		 case TUNER:
		 case SVIDEO:
		 case COMPONENT:
		 case VGA:
		 case DVI:
		 case SCART:
		 case SCARTT2:
		 case OVERLAY_SCART:
		 case SLAVE:
		 case DIPX:
			return gmd_AUDIO_IN_LINE_IN;
	  
#if (FEATURE_HDMI == ENABLE)
		 case HDMI:
			return gmd_AUDIO_IN_HDMI_I2S;
#endif		   
	  
#if (FEATURE_DISPLAY_PORT== ENABLE)
		 case CT_DISPLAY_PORT:
			switch (gm_GetDpDigitalID(gmvb_InputConnector[B_Channel]))
			{
			default:
			case DID_DPRX0:   return gmd_AUDIO_IN_DPRx1_I2S;
			case DID_DPRX1:   return gmd_AUDIO_IN_DPRx2_I2S;
			}
#endif		   
		 }
	  
	  case gmd_AUDIO_SRC_LINE_IN:	return gmd_AUDIO_IN_LINE_IN;
	  case gmd_AUDIO_SRC_I2S_IN:	return gmd_AUDIO_IN_I2S_IN;
	  case gmd_AUDIO_SRC_SPDIF_IN:	return gmd_AUDIO_IN_SPDIF_IN;
	  case gmd_AUDIO_SRC_AUTO_Pxp: return gmd_AUDIO_SRC_AUTO_Pxp;
	  }
   }
   else
   {
      source = GetUserPrefAudioPipInput();
	  
	  switch (source)
	  {
	  default:
	  case gmd_AUDIO_SRC_AUTO_Pxp:
	  
		 switch (InputConnType)
		 {
		 default:
		 case COMPOSITE:
		 case TUNER:
		 case SVIDEO:
		 case COMPONENT:
		 case VGA:
		 case DVI:
		 case SCART:
		 case SCARTT2:
		 case OVERLAY_SCART:
		 case SLAVE:
		 case DIPX:
			return gmd_AUDIO_IN_LINE_IN;
	  
#if (FEATURE_HDMI == ENABLE)
		 case HDMI:
			return gmd_AUDIO_IN_HDMI_I2S;
#endif		   
	  
#if (FEATURE_DISPLAY_PORT== ENABLE)
		 case CT_DISPLAY_PORT:
			switch (gm_GetDpDigitalID(gmvb_InputConnector[B_Channel]))
			{
			default:
			case DID_DPRX0:   return gmd_AUDIO_IN_DPRx1_I2S;
			case DID_DPRX1:   return gmd_AUDIO_IN_DPRx2_I2S;
			}
#endif		   
		 }
	  
	  case gmd_AUDIO_SRC_LINE_IN:	return gmd_AUDIO_IN_LINE_IN;
	  case gmd_AUDIO_SRC_I2S_IN:	return gmd_AUDIO_IN_I2S_IN;
	  case gmd_AUDIO_SRC_SPDIF_IN:	return gmd_AUDIO_IN_SPDIF_IN;
	  case gmd_AUDIO_SRC_AUTO_Main: return gmd_AUDIO_SRC_AUTO_Main;	  
	  }   
   }


}

gmt_AUDIO_OUTPORT_TYPE AAPI_Audio_Get_CurrentOutput(gmt_PHY_CH B_Channel)
{
   gmt_AUDIO_TARGET_MODE target;

   #if 0
   B_Channel = B_Channel;
   target = GetUserPrefAudioMainOutput();
   #else
   if (B_Channel == gmvb_MainChannel)
   {
      target = GetUserPrefAudioMainOutput();
   }
   else
   {
      	target = GetUserPrefAudioPipOutput();
   }
   #endif

   switch (target)
   {
   case gmd_AUDIO_TAR_I2S_OUT:         return gmd_AUDIO_OUT_I2S_OUT;
   case gmd_AUDIO_TAR_SPDIF_OUT:       return gmd_AUDIO_OUT_SPDIF_OUT;
   case gmd_AUDIO_TAR_DPTx_OUT:        return gmd_AUDIO_OUT_DPTx_OUT;
   case gmd_AUDIO_TAR_HeadPhone_OUT:   return gmd_AUDIO_OUT_HeadPhone_OUT;
   case gmd_AUDIO_TAR_LINE_OUT:        return gmd_AUDIO_OUT_LINE_OUT;
   default:
   	return gmd_AUDIO_OUT_HeadPhone_OUT;
   }
}

BYTE AAPI_Audio_Get_CurrentOutputBits(gmt_PHY_CH B_Channel)
{
   gmt_AUDIO_OUTPORT_TYPE output;
   BYTE ret;

   ret = SYS_API_AUDIO_GetCurrentInputBits(B_Channel);

   output = AAPI_Audio_Get_CurrentOutput(B_Channel);

   switch (output)
   {
   case gmd_AUDIO_OUT_I2S_OUT:   ret = 16;   break;
   }

   return (BYTE) ret;
}

BYTE AAPI_Audio_Get_CurrentOutputChannelCount(gmt_PHY_CH B_Channel)
{
   gmt_AUDIO_OUTPORT_TYPE output;
   BYTE ret;

   output = AAPI_Audio_Get_CurrentOutput(B_Channel);

   switch (output)
   {
   case gmd_AUDIO_OUT_I2S_OUT:
   case gmd_AUDIO_OUT_SPDIF_OUT:
   case gmd_AUDIO_OUT_DPTx_OUT:
   default:
   case gmd_AUDIO_OUT_HeadPhone_OUT:
   case gmd_AUDIO_OUT_LINE_OUT:

      ret = SYS_API_AUDIO_GetCurrentInputChannelCount(B_Channel);

      break;
   }

   return (BYTE) ret;
}

WORD AAPI_Audio_Get_CLK_MUL(gmt_PHY_CH B_Channel)
{
   WORD ret = 0;

   gmt_AUDIO_INPORT_TYPE input;
   gmt_AUDIO_OUTPORT_TYPE output;

   input  = AAPI_Audio_Get_CurrentInput(B_Channel);
   output = AAPI_Audio_Get_CurrentOutput(B_Channel);

   switch (output)
   {
   case gmd_AUDIO_OUT_I2S_OUT:
   case gmd_AUDIO_OUT_SPDIF_OUT:
   case gmd_AUDIO_OUT_DPTx_OUT:
   case gmd_AUDIO_OUT_HeadPhone_OUT:
   case gmd_AUDIO_OUT_LINE_OUT:
   default:
   case gmd_AUDIO_OUT_UNKNOWN:
      break;
   }

   switch (input)
   {
   case gmd_AUDIO_IN_DPRx1_I2S:
   case gmd_AUDIO_IN_DPRx2_I2S:
   case gmd_AUDIO_IN_HDMI_I2S:
   case gmd_AUDIO_IN_I2S_IN:
   case gmd_AUDIO_IN_SPDIF_IN:
   case gmd_AUDIO_IN_DAC_TPG:
   case gmd_AUDIO_IN_AVS:
   case gmd_AUDIO_IN_TPG:
   case gmd_AUDIO_IN_LINE_IN:
   default:
   case gmd_AUDIO_IN_UNKNOWN:
      break;
   }

   return ret;
}

BOOL AAPI_Audio_Get_AliveMode(void)
{
   // this function is used to control audio alive mode(audio alive without video)

   return FALSE;
}

gmt_PHY_CH AAPI_Audio_Get_SourceChannel(void)
{
// now, there is no audio source channel control in standard OSD.
// So, set it to main channel as default setting.
// By customer's requirement, the variable can be controlled by OSD.
#define GET_OSD_AUDIO_SOURCE_CH     gmvb_MainChannel

#define AUDIO_SYNC_LOST_MAIN        SYS_API_AUDIO_Is_SyncLost(gmvb_MainChannel)
#define AUDIO_SYNC_LOST_PIP         SYS_API_AUDIO_Is_SyncLost(gmvb_PipChannel)
#define AUDIO_SYNC_DETECTED_MAIN    SYS_API_AUDIO_Is_SyncDetected(gmvb_MainChannel)
#define AUDIO_SYNC_DETECTED_PIP     SYS_API_AUDIO_Is_SyncDetected(gmvb_PipChannel)

   static BOOL FirstTimeExecuted = TRUE;
   static BOOL changed;
   static gmt_PHY_CH previous_ch;

   gmt_PHY_CH current_ch;

   if (FirstTimeExecuted)
   {
      // initialize for local variables

      FirstTimeExecuted = FALSE;

      previous_ch = GET_OSD_AUDIO_SOURCE_CH;

      changed = FALSE;
   }

   if (B_SplashComplete == FALSE)
   {
      // AC power on and waiting for splash screen complete
      return gmvb_MainChannel;
   }

   current_ch = previous_ch;

   if (GetUserPrefPipMode() == 0)
   {
      // pip mode is off
      current_ch = gmvb_MainChannel;
   }
   else
   {
      if (changed == FALSE)
      {
         if (GET_OSD_AUDIO_SOURCE_CH == gmvb_MainChannel)
         {
            if (previous_ch == gmvb_MainChannel)
            {
               if (AUDIO_SYNC_DETECTED_MAIN)
               {
               }
               else
               {
                  if (AUDIO_SYNC_LOST_MAIN
                   || AUDIO_SYNC_DETECTED_PIP)
                  {
                     current_ch = gmvb_PipChannel;

                     changed = TRUE;
                  }
               }
            }
            else
            {
               if (AUDIO_SYNC_DETECTED_PIP
                && AUDIO_SYNC_LOST_MAIN)
               {
                  // pip channel is already the active channel
                  changed = TRUE;
               }
               else
               {
                  if (AUDIO_SYNC_LOST_PIP
                   || AUDIO_SYNC_DETECTED_MAIN)
                  {
                     current_ch = gmvb_MainChannel;

                     changed = TRUE;
                  }
               }
            }
         }
         else
         {
            if (previous_ch == gmvb_PipChannel)
            {
               if (AUDIO_SYNC_DETECTED_PIP)
               {
               }
               else
               {
                  if (AUDIO_SYNC_LOST_PIP
                   || AUDIO_SYNC_DETECTED_MAIN)
                  {
                     current_ch = gmvb_MainChannel;

                     changed = TRUE;
                  }
               }
            }
            else
            {
               if (AUDIO_SYNC_DETECTED_MAIN
                && AUDIO_SYNC_LOST_PIP)
               {
                  // main channel is already the active channel
                  changed = TRUE;
               }
               else
               {
                  if (AUDIO_SYNC_LOST_MAIN
                   || AUDIO_SYNC_DETECTED_PIP)
                  {
                     current_ch = gmvb_PipChannel;

                     changed = TRUE;
                  }
               }
            }
         }
      }
      else
      {
         if (GET_OSD_AUDIO_SOURCE_CH == gmvb_MainChannel)
         {
            if (AUDIO_SYNC_DETECTED_MAIN)
            {
               changed = FALSE;
            }
         }
         else
         {
            if (AUDIO_SYNC_DETECTED_PIP)
            {
               changed = FALSE;
            }
         }
      }
   }

   if (previous_ch != current_ch)
   {
      previous_ch = current_ch;
   }

   return previous_ch;
}

void AAPI_Audio_Cus_Mute(gmt_AUDIO_OUTPORT_TYPE output)
{
	gm_AUDIO_Print("AUDIO - CUS - AAPI_Audio_Cus_Mute()");

   switch (output)
   {
   case gmd_AUDIO_OUT_I2S_OUT:         Audio_Cus_I2S_Ext_Ampflier_Mute();     break;
   case gmd_AUDIO_OUT_HeadPhone_OUT:   Audio_Cus_HP_Mute();                   break;
   case gmd_AUDIO_OUT_LINE_OUT:        Audio_Cus_Speaker_Mute();              break;
   case gmd_AUDIO_OUT_SPDIF_OUT:       Audio_Cus_SPDIF_Ext_Ampflier_Mute();   break;

   case gmd_AUDIO_OUT_DPTx_OUT:
   default:;
   }
}

void AAPI_Audio_Cus_UnMute(gmt_AUDIO_OUTPORT_TYPE output)
{
	gm_AUDIO_Print("AUDIO - CUS - AAPI_Audio_Cus_UnMute()");

   switch (output)
   {
   case gmd_AUDIO_OUT_I2S_OUT:         Audio_Cus_I2S_Ext_Ampflier_UnMute();   break;
   case gmd_AUDIO_OUT_HeadPhone_OUT:   Audio_Cus_HP_UnMute();                 break;
   case gmd_AUDIO_OUT_LINE_OUT:        Audio_Cus_Speaker_UnMute();            break;
   case gmd_AUDIO_OUT_SPDIF_OUT:       Audio_Cus_SPDIF_Ext_Ampflier_UnMute(); break;

   case gmd_AUDIO_OUT_DPTx_OUT:
   default:;
   }
   //Hyb130425+ 
   if(gm_ReadRegWord(AUDIO_DAC_CTRL2)&MUTE_DAC_EN)
   	gm_ClearRegBitsWord(AUDIO_DAC_CTRL2,MUTE_DAC_EN);
}

void AAPI_Audio_Cus_Mute_1st(gmt_AUDIO_OUTPORT_TYPE output)
{
   switch (output)
   {
   case gmd_AUDIO_OUT_I2S_OUT:         Audio_Cus_I2S_Ext_Ampflier_Mute_1st();    break;
   case gmd_AUDIO_OUT_HeadPhone_OUT:   Audio_Cus_HP_Mute_1st();                  break;
   case gmd_AUDIO_OUT_LINE_OUT:        Audio_Cus_Speaker_Mute_1st();             break;
   case gmd_AUDIO_OUT_SPDIF_OUT:       Audio_Cus_SPDIF_Ext_Ampflier_Mute_1st();  break;

   case gmd_AUDIO_OUT_DPTx_OUT:
   default:;
   }
}

void AAPI_Audio_Cus_UnMute_1st(void)
{
}

void AAPI_Audio_Cus_Mute_By_OSD(gmt_AUDIO_OUTPORT_TYPE output)
{
	gm_AUDIO_Print("AUDIO - CUS - AAPI_Audio_Cus_Mute_By_OSD()");

   switch (output)
   {
   case gmd_AUDIO_OUT_I2S_OUT:         Audio_Cus_I2S_Ext_Ampflier_0_Volume();    break;
   case gmd_AUDIO_OUT_HeadPhone_OUT:   Audio_Cus_HP_0_Volume();                  break;
   case gmd_AUDIO_OUT_LINE_OUT:        Audio_Cus_Speaker_0_Volume();             break;
   case gmd_AUDIO_OUT_SPDIF_OUT:       Audio_Cus_SPDIF_Ext_Ampflier_0_Volume();  break;

   case gmd_AUDIO_OUT_DPTx_OUT:
   default:;
   }

   AAPI_Audio_Cus_Mute(output);
}

void AAPI_Audio_Cus_Volume_Adjust(gmt_AUDIO_OUTPORT_TYPE output, BYTE vol)
{
	gm_AUDIO_Print("AUDIO - CUS - AAPI_Audio_Cus_Volume_Adjust()");

   switch (output)
   {
   case gmd_AUDIO_OUT_I2S_OUT:         Audio_Cus_I2S_Ext_Ampflier_Volume_Adjust(vol);     break;
   case gmd_AUDIO_OUT_HeadPhone_OUT:   Audio_Cus_HP_Volume_Adjust(vol);                   break;
   case gmd_AUDIO_OUT_LINE_OUT:        Audio_Cus_Speaker_Volume_Adjust(vol);              break;
   case gmd_AUDIO_OUT_SPDIF_OUT:       Audio_Cus_SPDIF_Ext_Ampflier_Volume_Adjust(vol);   break;

   case gmd_AUDIO_OUT_DPTx_OUT:
   default:;
   }
}

void AAPI_Audio_Cus_Adjuster_Initialize(void)
{
   AUDIO_IS_AS_ENABLE  = FALSE;
   AUDIO_IS_AS_INDEX   = AAPI_Audio_Get_CurrentInput(AAPI_Audio_Get_SourceChannel());
   AUDIO_IS_AS_APPLIED = FALSE;
}

BOOL AAPI_Audio_Cus_Adjuster(gmt_PHY_CH B_Channel)
{
   BOOL ret = FALSE;

   if (B_Channel == gmvb_MainChannel)
   {
   }
   else if (B_Channel == gmvb_PipChannel)
   {
   }

   if (ret)
   {
      SYS_API_AUDIO_Cus_MUTE_By_OSD(B_Channel);
   }

   return ret;
}

void AAPI_Audio_Cus_Initialize(void)
{
   gmt_AUDIO_CAPABILITY_INFO support =
   {
      gmd_AUDIO_CODING_PCM |
      gmd_AUDIO_CODING_DTS |
      gmd_AUDIO_CODING_AC3,

      22000L,
      96000L,

      16,
      32,

      2,
      8
   };

   Audio_Cus_SPDIF_Ext_Ampflier_Mute_Initialize();
   Audio_Cus_I2S_Ext_Ampflier_Mute_Initialize();
   Audio_Cus_HP_Mute_Initialize();
   Audio_Cus_Speaker_Mute_Initialize();

   SYS_API_AUDIO_Set_Capability_I2S_IN(&support);
   SYS_API_AUDIO_Set_Capability_SPDIF_IN(&support);
   SYS_API_AUDIO_Set_Capability_HDMI_IN(&support);
   SYS_API_AUDIO_Set_Capability_DPRx1_IN(&support);
   SYS_API_AUDIO_Set_Capability_DPRx2_IN(&support);

   SYS_API_AUDIO_Set_DAC_VolumeStep(0);
   SYS_API_AUDIO_Set_DAC_GainMUL(10);



}

void AAPI_Audio_AutoScan_Handler(void)
{
   if (GetUserPrefPipMode() != 0)
   {
      if ((SYS_API_AUDIO_Is_SyncLost(gmvb_MainChannel))
       && (SYS_API_AUDIO_Is_SyncLost(gmvb_PipChannel)))
      {
         if ((SYS_API_AUDIO_Is_Fast_To_NextPort())
          || (((gm_GetSystemTime() - SYS_API_AUDIO_Get_SyncLostTime(gmvb_MainChannel)) >= 3000L) &&
              ((gm_GetSystemTime() - SYS_API_AUDIO_Get_SyncLostTime(gmvb_PipChannel)) >= 3000L)))
         {
            Audio_AutoScan_Enable();
            Audio_AutoScan_Get_NextIndex();

            AUDIO_IS_AS_APPLIED = FALSE;
         }
      }
      else
      {
         Audio_AutoScan_Disable();
      }
   }
   else
   {
      if (SYS_API_AUDIO_Is_SyncLost(gmvb_MainChannel))
      {
         if ((SYS_API_AUDIO_Is_Fast_To_NextPort())
          || ((gm_GetSystemTime() - SYS_API_AUDIO_Get_SyncLostTime(gmvb_MainChannel)) >= 3000L))
         {
            Audio_AutoScan_Enable();
            Audio_AutoScan_Get_NextIndex();

            AUDIO_IS_AS_APPLIED = FALSE;
         }
      }
      else
      {
         Audio_AutoScan_Disable();
      }
   }
}

WORD AAPI_Audio_UnMUTE(gmt_PHY_CH B_Channel)
{
#define WAIT_TIME_FOR_INTERNEL_DAC     20

   static DWORD check_point_main = 0;
   static DWORD check_point_pip = 0;

   BYTE ret_main = 0;
   BYTE ret_pip = 0;
   BYTE *pRET;
   DWORD *pCP;

#define RET_VALUE    (*pRET)
#define CHECK_POINT  (*pCP)
#define RET          (((WORD) (ret_main) << 12) | ((WORD) (ret_pip) << 8))

   if (B_SplashComplete == FALSE)
   {
      // Wait for splash screen completed
      return 1;
   }

   if (SYS_API_AUDIO_Is_AVS_Changing() == TRUE)
   {
      // Wait for AVS transition completed
      return 2;
   }

   if (GetUserPrefAudioMute() == TRUE)
   {
      return 3;
   }

   if (B_Channel == gmvb_PipChannel)
   {
      if (GetUserPrefPipMode() == NO_PIP)
      {
         return 4;
      }
   }

   if (SYS_API_AUDIO_Is_SetupDone(B_Channel) == FALSE)
   {
      return 5;
   }

   if (SYS_API_AUDIO_Is_Video_Synchronized(B_Channel) == FALSE)
   {
      return 6;
   }

   if (SYS_API_AUDIO_Is_AVMUTE(B_Channel) == TRUE)
   {
      return 7;
   }

	//Hyb130425+ for redo gain before output audio, mute before mode setup complete;
	if(AAPI_Audio_IsVideoSetupCompleted(B_Channel) == gmd_FALSE)
		return 7;
	if(B_NeedRedoDACGain == gmd_TRUE)
		Audio_RedoDACGain();

   if (B_Channel == gmvb_MainChannel)
   {
      pRET = &ret_main;
      pCP  = &check_point_main;
   }
   else
   {
      pRET = &ret_pip;
      pCP  = &check_point_pip;
   }



   if (SYS_API_AUDIO_UnMute_Apply_Check(B_Channel) == TRUE)
   {
      RET_VALUE = 8;

      gm_AUDIO_Print("AUDIO - UNMUTE(%d)", B_Channel);

      SYS_API_AUDIO_Output_UnMUTE(B_Channel);

      SYS_API_AUDIO_Enable_Interrupt(B_Channel);

      CHECK_POINT = gm_GetSystemTime();
   }
   else
   {
      RET_VALUE = 9;

      if ((CHECK_POINT != 0)
       && ((gm_GetSystemTime() - CHECK_POINT) >= WAIT_TIME_FOR_INTERNEL_DAC))
      {
         RET_VALUE = 10;

         SYS_API_AUDIO_Cus_UnMUTE(B_Channel);

         CHECK_POINT = 0;
      }
   }

   return RET;
}

WORD * AAPI_Audio_Get_UnMUTE_Status(void)
{
   static WORD UnMute_Auto_status = 0;

   return &UnMute_Auto_status;
}

void AAPI_Audio_UnMUTE_Status(WORD encode)
{
#define UnMute_Auto_status    (*AAPI_Audio_Get_UnMUTE_Status())

   if (UnMute_Auto_status != encode)
   {
      UnMute_Auto_status = encode;

      gm_AUDIO_Print("AUDIO - Audio_UnMUTE_Status() = 0x%X", UnMute_Auto_status);
   }
}

BOOL AAPI_Audio_IsVideoSetupCompleted(gmt_PHY_CH B_Channel)
{
   return (stInput[B_Channel].ModeFlags & gmd_MODE_COMPLETE) ? TRUE : FALSE;
}

void Audio_Cus_SPDIF_Ext_Ampflier_Volume_Adjust(BYTE vol)
{
#if (USE_EXT_SPDIF_DEVICE == ENABLE)
   switch (Get_Behavior_ID())
   {
   default:
   case BEHAVIOR_ID_UNKNOWN:
   case BEHAVIOR_ID_93xx_EV1:
   case BEHAVIOR_ID_93xx_EV2:
   case BEHAVIOR_ID_93xx_EV3:
   case BEHAVIOR_ID_93xx_RD1:
   case BEHAVIOR_ID_93xx_RD1_REV_B:
   case BEHAVIOR_ID_93xx_RD2:
   case BEHAVIOR_ID_93xx_RD3:
   case BEHAVIOR_ID_93xx_ST_4K2K_FE:
   case BEHAVIOR_ID_93xx_ST_4K2K_R:
   case BEHAVIOR_ID_93xx_ST_4K2K_L:
      UNUSED_VAR(vol);
      break;
   }
#else
   UNUSED_VAR(vol);
#endif
}

void Audio_Cus_SPDIF_Ext_Ampflier_0_Volume(void)
{
#if (USE_EXT_SPDIF_DEVICE == ENABLE)
   switch (Get_Behavior_ID())
   {
   default:
   case BEHAVIOR_ID_UNKNOWN:
   case BEHAVIOR_ID_93xx_EV1:
   case BEHAVIOR_ID_93xx_EV2:
   case BEHAVIOR_ID_93xx_EV3:
   case BEHAVIOR_ID_93xx_RD1:
   case BEHAVIOR_ID_93xx_RD1_REV_B:
   case BEHAVIOR_ID_93xx_RD2:
   case BEHAVIOR_ID_93xx_RD3:
   case BEHAVIOR_ID_93xx_ST_4K2K_FE:
   case BEHAVIOR_ID_93xx_ST_4K2K_R:
   case BEHAVIOR_ID_93xx_ST_4K2K_L:
      break;
   }
#endif
}

void Audio_Cus_SPDIF_Ext_Ampflier_Mute_1st(void)
{
#if (USE_EXT_SPDIF_DEVICE == ENABLE)
   switch (Get_Behavior_ID())
   {
   default:
   case BEHAVIOR_ID_UNKNOWN:
   case BEHAVIOR_ID_93xx_EV1:
   case BEHAVIOR_ID_93xx_EV2:
   case BEHAVIOR_ID_93xx_EV3:
   case BEHAVIOR_ID_93xx_RD1:
   case BEHAVIOR_ID_93xx_RD1_REV_B:
   case BEHAVIOR_ID_93xx_RD2:
   case BEHAVIOR_ID_93xx_RD3:
   case BEHAVIOR_ID_93xx_ST_4K2K_FE:
   case BEHAVIOR_ID_93xx_ST_4K2K_R:
   case BEHAVIOR_ID_93xx_ST_4K2K_L:
      break;
   }
#endif
}

void Audio_Cus_SPDIF_Ext_Ampflier_Mute(void)
{
#if (USE_EXT_SPDIF_DEVICE == ENABLE)
   switch (Get_Behavior_ID())
   {
   default:
   case BEHAVIOR_ID_UNKNOWN:
   case BEHAVIOR_ID_93xx_EV1:
   case BEHAVIOR_ID_93xx_EV2:
   case BEHAVIOR_ID_93xx_EV3:
   case BEHAVIOR_ID_93xx_RD1:
   case BEHAVIOR_ID_93xx_RD1_REV_B:
   case BEHAVIOR_ID_93xx_RD2:
   case BEHAVIOR_ID_93xx_RD3:
   case BEHAVIOR_ID_93xx_ST_4K2K_FE:
   case BEHAVIOR_ID_93xx_ST_4K2K_R:
   case BEHAVIOR_ID_93xx_ST_4K2K_L:
      break;
   }
#endif
}

void Audio_Cus_SPDIF_Ext_Ampflier_UnMute(void)
{
#if (USE_EXT_SPDIF_DEVICE == ENABLE)
   switch (Get_Behavior_ID())
   {
   default:
   case BEHAVIOR_ID_UNKNOWN:
   case BEHAVIOR_ID_93xx_EV1:
   case BEHAVIOR_ID_93xx_EV2:
   case BEHAVIOR_ID_93xx_EV3:
   case BEHAVIOR_ID_93xx_RD1:
   case BEHAVIOR_ID_93xx_RD1_REV_B:
   case BEHAVIOR_ID_93xx_RD2:
   case BEHAVIOR_ID_93xx_RD3:
   case BEHAVIOR_ID_93xx_ST_4K2K_FE:
   case BEHAVIOR_ID_93xx_ST_4K2K_R:
   case BEHAVIOR_ID_93xx_ST_4K2K_L:
      break;
   }
#endif
}

void Audio_Cus_SPDIF_Ext_Ampflier_Mute_Initialize(void)
{
#if (USE_EXT_SPDIF_DEVICE == ENABLE)
   switch (Get_Behavior_ID())
   {
   default:
   case BEHAVIOR_ID_UNKNOWN:
   case BEHAVIOR_ID_93xx_EV1:
   case BEHAVIOR_ID_93xx_EV2:
   case BEHAVIOR_ID_93xx_EV3:
   case BEHAVIOR_ID_93xx_RD1:
   case BEHAVIOR_ID_93xx_RD1_REV_B:
   case BEHAVIOR_ID_93xx_RD2:
   case BEHAVIOR_ID_93xx_RD3:
   case BEHAVIOR_ID_93xx_ST_4K2K_FE:
   case BEHAVIOR_ID_93xx_ST_4K2K_R:
   case BEHAVIOR_ID_93xx_ST_4K2K_L:
      break;
   }

   Audio_Cus_SPDIF_Ext_Ampflier_Mute();
#endif
}

void Audio_Cus_I2S_Ext_Ampflier_Volume_Adjust(BYTE vol)
{
#if (USE_EXT_I2S_DEVICE == ENABLE)
   switch (Get_Behavior_ID())
   {
   default:
   case BEHAVIOR_ID_UNKNOWN:
   case BEHAVIOR_ID_93xx_EV1:
   case BEHAVIOR_ID_93xx_EV2:
   case BEHAVIOR_ID_93xx_EV3:
   case BEHAVIOR_ID_93xx_RD1:
   case BEHAVIOR_ID_93xx_RD1_REV_B:
   case BEHAVIOR_ID_93xx_RD2:
   case BEHAVIOR_ID_93xx_RD3:
   case BEHAVIOR_ID_93xx_ST_4K2K_FE:
   case BEHAVIOR_ID_93xx_ST_4K2K_R:
   case BEHAVIOR_ID_93xx_ST_4K2K_L:
      UNUSED_VAR(vol);
      break;
   }
#else
   UNUSED_VAR(vol);
#endif
}

void Audio_Cus_I2S_Ext_Ampflier_0_Volume(void)
{
#if (USE_EXT_I2S_DEVICE == ENABLE)
   switch (Get_Behavior_ID())
   {
   default:
   case BEHAVIOR_ID_UNKNOWN:
   case BEHAVIOR_ID_93xx_EV1:
   case BEHAVIOR_ID_93xx_EV2:
   case BEHAVIOR_ID_93xx_EV3:
   case BEHAVIOR_ID_93xx_RD1:
   case BEHAVIOR_ID_93xx_RD1_REV_B:
   case BEHAVIOR_ID_93xx_RD2:
   case BEHAVIOR_ID_93xx_RD3:
   case BEHAVIOR_ID_93xx_ST_4K2K_FE:
   case BEHAVIOR_ID_93xx_ST_4K2K_R:
   case BEHAVIOR_ID_93xx_ST_4K2K_L:
      break;
   }
#endif
}

void Audio_Cus_I2S_Ext_Ampflier_Mute_1st(void)
{
#if (USE_EXT_I2S_DEVICE == ENABLE)
   switch (Get_Behavior_ID())
   {
   default:
   case BEHAVIOR_ID_UNKNOWN:
   case BEHAVIOR_ID_93xx_EV1:
   case BEHAVIOR_ID_93xx_EV2:
   case BEHAVIOR_ID_93xx_EV3:
   case BEHAVIOR_ID_93xx_RD1:
   case BEHAVIOR_ID_93xx_RD1_REV_B:
   case BEHAVIOR_ID_93xx_RD2:
   case BEHAVIOR_ID_93xx_RD3:
   case BEHAVIOR_ID_93xx_ST_4K2K_FE:
   case BEHAVIOR_ID_93xx_ST_4K2K_R:
   case BEHAVIOR_ID_93xx_ST_4K2K_L:
      break;
   }
#endif
}

void Audio_Cus_I2S_Ext_Ampflier_Mute(void)
{
#if (USE_EXT_I2S_DEVICE == ENABLE)
   switch (Get_Behavior_ID())
   {
   default:
   case BEHAVIOR_ID_UNKNOWN:
   case BEHAVIOR_ID_93xx_EV1:
   case BEHAVIOR_ID_93xx_EV2:
   case BEHAVIOR_ID_93xx_EV3:
   case BEHAVIOR_ID_93xx_RD1:
   case BEHAVIOR_ID_93xx_RD1_REV_B:
   case BEHAVIOR_ID_93xx_RD2:
   case BEHAVIOR_ID_93xx_RD3:
   case BEHAVIOR_ID_93xx_ST_4K2K_FE:
   case BEHAVIOR_ID_93xx_ST_4K2K_R:
   case BEHAVIOR_ID_93xx_ST_4K2K_L:
      break;
   }
#endif
}

void Audio_Cus_I2S_Ext_Ampflier_UnMute(void)
{
#if (USE_EXT_I2S_DEVICE == ENABLE)
   switch (Get_Behavior_ID())
   {
   default:
   case BEHAVIOR_ID_UNKNOWN:
   case BEHAVIOR_ID_93xx_EV1:
   case BEHAVIOR_ID_93xx_EV2:
   case BEHAVIOR_ID_93xx_EV3:
   case BEHAVIOR_ID_93xx_RD1:
   case BEHAVIOR_ID_93xx_RD1_REV_B:
   case BEHAVIOR_ID_93xx_RD2:
   case BEHAVIOR_ID_93xx_RD3:
   case BEHAVIOR_ID_93xx_ST_4K2K_FE:
   case BEHAVIOR_ID_93xx_ST_4K2K_R:
   case BEHAVIOR_ID_93xx_ST_4K2K_L:
      break;
   }
#endif
}

void Audio_Cus_I2S_Ext_Ampflier_Mute_Initialize(void)
{
#if (USE_EXT_I2S_DEVICE == ENABLE)
   switch (Get_Behavior_ID())
   {
   default:
   case BEHAVIOR_ID_UNKNOWN:
   case BEHAVIOR_ID_93xx_EV1:
   case BEHAVIOR_ID_93xx_EV2:
   case BEHAVIOR_ID_93xx_EV3:
   case BEHAVIOR_ID_93xx_RD1:
   case BEHAVIOR_ID_93xx_RD1_REV_B:
   case BEHAVIOR_ID_93xx_RD2:
   case BEHAVIOR_ID_93xx_RD3:
   case BEHAVIOR_ID_93xx_ST_4K2K_FE:
   case BEHAVIOR_ID_93xx_ST_4K2K_R:
   case BEHAVIOR_ID_93xx_ST_4K2K_L:
      break;
   }

   Audio_Cus_I2S_Ext_Ampflier_Mute();
#endif
}

void Audio_Cus_HP_Volume_Adjust(BYTE vol)
{
#if (USE_EXT_HP_DEVICE == ENABLE)
   switch (Get_Behavior_ID())
   {
   default:
   case BEHAVIOR_ID_UNKNOWN:
   case BEHAVIOR_ID_93xx_EV1:
   case BEHAVIOR_ID_93xx_EV2:
   case BEHAVIOR_ID_93xx_EV3:
   case BEHAVIOR_ID_93xx_RD1:
   case BEHAVIOR_ID_93xx_RD1_REV_B:
   case BEHAVIOR_ID_93xx_RD2:
   case BEHAVIOR_ID_93xx_RD3:
   case BEHAVIOR_ID_93xx_ST_4K2K_FE:
   case BEHAVIOR_ID_93xx_ST_4K2K_R:
   case BEHAVIOR_ID_93xx_ST_4K2K_L:
      UNUSED_VAR(vol);
      break;
   }
#else
   UNUSED_VAR(vol);
#endif
}

void Audio_Cus_HP_0_Volume(void)
{
#if (USE_EXT_HP_DEVICE == ENABLE)
   switch (Get_Behavior_ID())
   {
   default:
   case BEHAVIOR_ID_UNKNOWN:
   case BEHAVIOR_ID_93xx_EV1:
   case BEHAVIOR_ID_93xx_EV2:
   case BEHAVIOR_ID_93xx_EV3:
   case BEHAVIOR_ID_93xx_RD1:
   case BEHAVIOR_ID_93xx_RD1_REV_B:
   case BEHAVIOR_ID_93xx_RD2:
   case BEHAVIOR_ID_93xx_RD3:
   case BEHAVIOR_ID_93xx_ST_4K2K_FE:
   case BEHAVIOR_ID_93xx_ST_4K2K_R:
   case BEHAVIOR_ID_93xx_ST_4K2K_L:
      break;
   }
#endif
}

void Audio_Cus_HP_Mute_1st(void)
{
#if (USE_EXT_HP_DEVICE == ENABLE)
   switch (Get_Behavior_ID())
   {
   default:
   case BEHAVIOR_ID_UNKNOWN:
   case BEHAVIOR_ID_93xx_EV1:
   case BEHAVIOR_ID_93xx_EV2:
   case BEHAVIOR_ID_93xx_EV3:
   case BEHAVIOR_ID_93xx_RD1:
   case BEHAVIOR_ID_93xx_RD1_REV_B:
   case BEHAVIOR_ID_93xx_RD2:
   case BEHAVIOR_ID_93xx_RD3:
   case BEHAVIOR_ID_93xx_ST_4K2K_FE:
   case BEHAVIOR_ID_93xx_ST_4K2K_R:
      break;

   case BEHAVIOR_ID_93xx_ST_4K2K_L:
      gm_ClearRegBitsWord(GPOUTPUT5, GPIO92_OUT);
      break;
   }
#endif
}

void Audio_Cus_HP_Mute(void)
{
#if (USE_EXT_HP_DEVICE == ENABLE)
   switch (Get_Behavior_ID())
   {
   default:
   case BEHAVIOR_ID_UNKNOWN:
   case BEHAVIOR_ID_93xx_EV1:
   case BEHAVIOR_ID_93xx_EV2:
   case BEHAVIOR_ID_93xx_EV3:
   case BEHAVIOR_ID_93xx_RD1:
   case BEHAVIOR_ID_93xx_RD1_REV_B:
   case BEHAVIOR_ID_93xx_RD2:
   case BEHAVIOR_ID_93xx_ST_4K2K_FE:
   case BEHAVIOR_ID_93xx_ST_4K2K_R:
      break;
	case BEHAVIOR_ID_93xx_RD3:
		#if((BOARD_USED == BOARD_93xx_CJ2))
		HEADPHONE_MUTE();
		#endif
	break;

   case BEHAVIOR_ID_93xx_ST_4K2K_L:
      gm_ClearRegBitsWord(GPOUTPUT5, GPIO92_OUT);
      break;
   }
#endif
}

void Audio_Cus_HP_UnMute(void)
{
#if (USE_EXT_HP_DEVICE == ENABLE)
   switch (Get_Behavior_ID())
   {
   default:
   case BEHAVIOR_ID_UNKNOWN:
   case BEHAVIOR_ID_93xx_EV1:
   case BEHAVIOR_ID_93xx_EV2:
   case BEHAVIOR_ID_93xx_EV3:
   case BEHAVIOR_ID_93xx_RD1:
   case BEHAVIOR_ID_93xx_RD1_REV_B:
   case BEHAVIOR_ID_93xx_RD2:
   case BEHAVIOR_ID_93xx_ST_4K2K_FE:
   case BEHAVIOR_ID_93xx_ST_4K2K_R:
      break;
	case BEHAVIOR_ID_93xx_RD3:
	#if((BOARD_USED == BOARD_93xx_CJ2))
   	HEADPHONE_UNMUTE();
	#endif
	break;

   case BEHAVIOR_ID_93xx_ST_4K2K_L:
      gm_SetRegBitsWord(GPOUTPUT5, GPIO92_OUT);
      break;
   }
#endif
}

void Audio_Cus_HP_Mute_Initialize(void)
{
#if (USE_EXT_HP_DEVICE == ENABLE)

   switch (Get_Behavior_ID())
   {
   default:
   case BEHAVIOR_ID_UNKNOWN:
   case BEHAVIOR_ID_93xx_EV1:
   case BEHAVIOR_ID_93xx_EV2:
   case BEHAVIOR_ID_93xx_EV3:
   case BEHAVIOR_ID_93xx_RD1:
   case BEHAVIOR_ID_93xx_RD1_REV_B:
   case BEHAVIOR_ID_93xx_RD2:
   case BEHAVIOR_ID_93xx_RD3:
   case BEHAVIOR_ID_93xx_ST_4K2K_FE:
   case BEHAVIOR_ID_93xx_ST_4K2K_R:
       break;

	case BEHAVIOR_ID_93xx_RD3:
		#if((BOARD_USED == BOARD_93xx_CJ2))
		gm_SetRegBitsWord(GPIO_5_ENABLE,HEADPHONE_MUTE_EN);
		gm_SetRegBitsWord(GPIO_5_DIRCTRL,HEADPHONE_MUTE_DIR);		
		gm_SetRegBitsWord(GPIO_5_OPENDRAIN_EN,HEADPHONE_MUTE_OD);	
		#endif	
	break;

  case BEHAVIOR_ID_93xx_ST_4K2K_L:
      gm_SetRegBitsWord(GPIO_5_ENABLE, GPIO92_EN);
      gm_SetRegBitsWord(GPIO_5_DIRCTRL, GPIO92_IO);
      gm_ClearRegBitsWord(GPIO_5_OPENDRAIN_EN, GPIO92_OD);
      break;
   }

   Audio_Cus_HP_Mute();
#endif
}

void Audio_Cus_Speaker_Volume_Adjust(BYTE vol)
{
#if (USE_EXT_SPEAKER_DEVICE == ENABLE)
   switch (Get_Behavior_ID())
   {
   default:
   case BEHAVIOR_ID_UNKNOWN:
   case BEHAVIOR_ID_93xx_EV1:
   case BEHAVIOR_ID_93xx_EV2:
   case BEHAVIOR_ID_93xx_EV3:
   case BEHAVIOR_ID_93xx_RD1:
   case BEHAVIOR_ID_93xx_RD1_REV_B:
   case BEHAVIOR_ID_93xx_RD2:
   case BEHAVIOR_ID_93xx_RD3:
   case BEHAVIOR_ID_93xx_ST_4K2K_FE:
   case BEHAVIOR_ID_93xx_ST_4K2K_R:
   case BEHAVIOR_ID_93xx_ST_4K2K_L:
      UNUSED_VAR(vol);
      break;
   }
#else
   UNUSED_VAR(vol);
#endif
}

void Audio_Cus_Speaker_0_Volume(void)
{
#if (USE_EXT_SPEAKER_DEVICE == ENABLE)
   switch (Get_Behavior_ID())
   {
   default:
   case BEHAVIOR_ID_UNKNOWN:
   case BEHAVIOR_ID_93xx_EV1:
   case BEHAVIOR_ID_93xx_EV2:
   case BEHAVIOR_ID_93xx_EV3:
   case BEHAVIOR_ID_93xx_RD1:
   case BEHAVIOR_ID_93xx_RD1_REV_B:
   case BEHAVIOR_ID_93xx_RD2:
   case BEHAVIOR_ID_93xx_RD3:
   case BEHAVIOR_ID_93xx_ST_4K2K_FE:
   case BEHAVIOR_ID_93xx_ST_4K2K_R:
   case BEHAVIOR_ID_93xx_ST_4K2K_L:
      break;
   }
#endif
}

void Audio_Cus_Speaker_Mute_1st(void)
{
#if (USE_EXT_SPEAKER_DEVICE == ENABLE)
   switch (Get_Behavior_ID())
   {
   default:
   case BEHAVIOR_ID_UNKNOWN:
   case BEHAVIOR_ID_93xx_EV1:
   case BEHAVIOR_ID_93xx_EV2:
   case BEHAVIOR_ID_93xx_EV3:
   case BEHAVIOR_ID_93xx_ST_4K2K_FE:
   case BEHAVIOR_ID_93xx_ST_4K2K_R:
      break;

   case BEHAVIOR_ID_93xx_RD1:
   case BEHAVIOR_ID_93xx_RD1_REV_B:
   case BEHAVIOR_ID_93xx_RD2:
   case BEHAVIOR_ID_93xx_RD3:
   case BEHAVIOR_ID_93xx_ST_4K2K_L:
      TDA7491_DISABLE();
      break;
   }
#endif
}

void Audio_Cus_Speaker_Mute(void)
{
#if (USE_EXT_SPEAKER_DEVICE == ENABLE)
   switch (Get_Behavior_ID())
   {
   default:
   case BEHAVIOR_ID_UNKNOWN:
   case BEHAVIOR_ID_93xx_EV1:
   case BEHAVIOR_ID_93xx_EV2:
   case BEHAVIOR_ID_93xx_EV3:
   case BEHAVIOR_ID_93xx_ST_4K2K_FE:
   case BEHAVIOR_ID_93xx_ST_4K2K_R:
      break;

   case BEHAVIOR_ID_93xx_RD1:
   case BEHAVIOR_ID_93xx_RD1_REV_B:
   case BEHAVIOR_ID_93xx_RD2:
   case BEHAVIOR_ID_93xx_RD3:
   case BEHAVIOR_ID_93xx_ST_4K2K_L:
      TDA7491_DISABLE();
      break;
   }
#endif
}

void Audio_Cus_Speaker_UnMute(void)
{
#if (USE_EXT_SPEAKER_DEVICE == ENABLE)
   switch (Get_Behavior_ID())
   {
   default:
   case BEHAVIOR_ID_UNKNOWN:
   case BEHAVIOR_ID_93xx_EV1:
   case BEHAVIOR_ID_93xx_EV2:
   case BEHAVIOR_ID_93xx_EV3:
   case BEHAVIOR_ID_93xx_ST_4K2K_FE:
   case BEHAVIOR_ID_93xx_ST_4K2K_R:
      break;

   case BEHAVIOR_ID_93xx_RD1:
   case BEHAVIOR_ID_93xx_RD1_REV_B:
   case BEHAVIOR_ID_93xx_RD2:
   case BEHAVIOR_ID_93xx_RD3:
   case BEHAVIOR_ID_93xx_ST_4K2K_L:
      TDA7491_ENABLE();
      break;
   }
#endif
}

void Audio_Cus_Speaker_Mute_Initialize(void)
{
#if (USE_EXT_SPEAKER_DEVICE == ENABLE)
   switch (Get_Behavior_ID())
   {
   default:
   case BEHAVIOR_ID_UNKNOWN:
   case BEHAVIOR_ID_93xx_EV1:
   case BEHAVIOR_ID_93xx_EV2:
   case BEHAVIOR_ID_93xx_EV3:
   case BEHAVIOR_ID_93xx_ST_4K2K_FE:
   case BEHAVIOR_ID_93xx_ST_4K2K_R:
      break;

   case BEHAVIOR_ID_93xx_RD1:
   case BEHAVIOR_ID_93xx_RD1_REV_B:
   case BEHAVIOR_ID_93xx_RD2:
   case BEHAVIOR_ID_93xx_RD3:
   case BEHAVIOR_ID_93xx_ST_4K2K_L:
      TDA7491_Initialize();
      break;
   }

   Audio_Cus_Speaker_Mute();
#endif
}

void Audio_VideoSyncLost(gmt_PHY_CH B_Channel)
{
   SYS_API_AUDIO_Set_VideoSynchronized(B_Channel, FALSE);
}

void Audio_VideoSyncDetected(gmt_PHY_CH B_Channel)
{
   SYS_API_AUDIO_Set_VideoSynchronized(B_Channel, TRUE);
}

gmt_AUDIO_INPORT_TYPE Audio_AutoScan_Get_NextPort(void)
{
   if (AUDIO_IS_AS_ENABLE == FALSE)
   {
      return gmd_AUDIO_IN_UNKNOWN;
   }

   if (AUDIO_IS_AS_APPLIED)
   {
      return (gmt_AUDIO_INPORT_TYPE) AUDIO_IS_AS_INDEX;
   }

   AUDIO_IS_AS_ENABLE = FALSE;

   if ((GetUserPrefAudioPipInput() == gmd_AUDIO_SRC_AUTO_Pxp)
    && (AUDIO_IS_AS_INDEX == AAPI_Audio_Get_CurrentInput(gmvb_PipChannel))
    && (AUDIO_IS_AS_APPLIED == FALSE))
   {
      Audio_AutoScan_Get_NextIndex();
   }

   AUDIO_IS_AS_ENABLE  = TRUE;
   AUDIO_IS_AS_APPLIED = TRUE;

   return (gmt_AUDIO_INPORT_TYPE) AUDIO_IS_AS_INDEX;
}

void Audio_AutoScan_Get_NextIndex(void)
{
   if (AUDIO_IS_AS_INDEX >= gmd_AUDIO_IN_UNKNOWN)
   {
      AUDIO_IS_AS_INDEX = 0;
   }
   else
   {
      AUDIO_IS_AS_INDEX ++;
   }
}

void Audio_AutoScan_Enable(void)
{
   AUDIO_IS_AS_ENABLE = TRUE;
}

void Audio_AutoScan_Disable(void)
{
   AUDIO_IS_AS_ENABLE = FALSE;
}

void Audio_Initialize(void)
{
   switch (Get_Behavior_ID())
   {
   default:
   case BEHAVIOR_ID_UNKNOWN:
      return;

   case BEHAVIOR_ID_93xx_EV1:
   case BEHAVIOR_ID_93xx_EV2:
   case BEHAVIOR_ID_93xx_EV3:
   case BEHAVIOR_ID_93xx_RD1:
   case BEHAVIOR_ID_93xx_RD1_REV_B:
   case BEHAVIOR_ID_93xx_RD2:
   case BEHAVIOR_ID_93xx_RD3:
      break;

   case BEHAVIOR_ID_93xx_ST_4K2K_FE:
      UserPrefAudioMainInput  = gmd_AUDIO_SRC_AUTO_Main;
      UserPrefAudioMainOutput = gmd_AUDIO_TAR_DPTx_OUT;
      break;

   case BEHAVIOR_ID_93xx_ST_4K2K_L:
      UserPrefAudioMainInput  = gmd_AUDIO_SRC_AUTO_Main;
      // UserPrefAudioMainOutput = gmd_AUDIO_TAR_HeadPhone_OUT;
      break;

   case BEHAVIOR_ID_93xx_ST_4K2K_R:
      return;
   }

   SYS_API_AUDIO_Initialize();
	
}

void Audio_Handler(void)
{
   switch (Get_Behavior_ID())
   {
   default:
   case BEHAVIOR_ID_UNKNOWN:
      return;

   case BEHAVIOR_ID_93xx_EV1:
   case BEHAVIOR_ID_93xx_EV2:
   case BEHAVIOR_ID_93xx_EV3:
   case BEHAVIOR_ID_93xx_RD1:
   case BEHAVIOR_ID_93xx_RD1_REV_B:
   case BEHAVIOR_ID_93xx_RD2:
   case BEHAVIOR_ID_93xx_RD3:
   case BEHAVIOR_ID_93xx_ST_4K2K_FE:
   case BEHAVIOR_ID_93xx_ST_4K2K_L:
      break;

   case BEHAVIOR_ID_93xx_ST_4K2K_R:
      return;
   }

   if(UserPrefAudioMainInput != gmd_AUDIO_SRC_AUTO_Pxp)  // Neil 140816 for audio noise when pxp source change
   	SYS_API_AUDIO_Handler(gmvb_MainChannel);
   if(UserPrefAudioMainInput == gmd_AUDIO_SRC_AUTO_Pxp)   // Neil 140816 for audio noise when pxp source change
   	SYS_API_AUDIO_Handler(gmvb_PipChannel);
}

gmt_AUDIO_CUS_ADJUSTER * Audio_Get_Adjuster(void)
{
   static gmt_AUDIO_CUS_ADJUSTER Adjuster;

   return (gmt_AUDIO_CUS_ADJUSTER *) &Adjuster;
}

#endif

