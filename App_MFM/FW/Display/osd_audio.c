/*
    $Workfile:   osd_audio.c  $
    $Revision: 1.17 $
    $Date: 2013/01/29 09:01:22 $
*/
#define __OSD_AUDIO_C__
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
// MODULE:         osd_audio.c
//
// USAGE:        This module contains standard osd adjusters for
//                audio control (e.g. volume, mute, balance)
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "system\all.h"

//******************************************************************************
//  E X T E R N S
//******************************************************************************

//******************************************************************************
//  D E F I N E S
//******************************************************************************

//#define DEBUG_AUDIO_OSD
#ifdef DEBUG_AUDIO_OSD
#define gm_AUDIO_OSD_Print  gm_Print
#else
#define gm_AUDIO_OSD_Print  (void)
#endif

//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************

//******************************************************************************
// S T A T I C   F U N C T I O N S
//******************************************************************************

//******************************************************************************
//  C O D E
//******************************************************************************
void AdjustAudio(void)
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

#if defined(ST_4K2K_93xx_BOARD)
   case BEHAVIOR_ID_93xx_ST_4K2K_FE:
   case BEHAVIOR_ID_93xx_ST_4K2K_L:
      break;

   case BEHAVIOR_ID_93xx_ST_4K2K_R:
   #if ENABLE_SOC_IPC_BUSY_CHECK
      if (!SocIPCIsSlaveLeftReady())
      {
         // Return it once "LEFT" is busy
         return;
      }
   #endif

      SOCIPC_SyncOSDAdjustHandler(SI_OSD_SYNC_AUDIO);
      SOCIPC_SyncOSDAdjustHandler(SI_OSD_SYNC_AUDIO_2);
      return;
#endif
   }

#if (FEATURE_AUDIO == ENABLE)
   SYS_API_AUDIO_Adjuster();
#endif
}

void AdjustAudioVolume(void)
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

#if defined(ST_4K2K_93xx_BOARD)
   case BEHAVIOR_ID_93xx_ST_4K2K_FE:
   case BEHAVIOR_ID_93xx_ST_4K2K_L:
      break;

   case BEHAVIOR_ID_93xx_ST_4K2K_R:
   #if ENABLE_SOC_IPC_BUSY_CHECK
      if (!SocIPCIsSlaveLeftReady())
      {
         // Return it once "LEFT" is busy
         return;
      }
   #endif

      SOCIPC_SyncOSDAdjustHandler(SI_OSD_SYNC_AUDIO_2);
      return;
#endif
   }

#if (FEATURE_AUDIO == ENABLE)
   SYS_API_AUDIO_Adjuster();
#endif
}

void ToggleMute(void)
{
   UserPrefAudioMute ^= TRUE;

#if (FEATURE_AUDIO == ENABLE)
   SYS_API_AUDIO_Adjuster();
#endif
}

BYTE GetMaximumVolumeOSDStep(void)
{
   return (BYTE) MAXIMAL_VOLUME_VALUE;
}

void VolumeUp(BYTE key)
{
   static BYTE prevAudioVolume = 0;

   switch (key)
   {
   case BUTTON_PRESSED:

      if (UserPrefAudioVolume >= GetMaximumVolumeOSDStep())
      {
         UserPrefAudioVolume = GetMaximumVolumeOSDStep();
      }
      else
      {
         UserPrefAudioVolume += 1;
      }

      if (prevAudioVolume != UserPrefAudioVolume)
      {
         prevAudioVolume = UserPrefAudioVolume;

         AdjustAudioVolume();

         OSDRequestMsg(gmvb_MainChannel, MTO_DrawVolumeSlider, REQUEST_NOW);
      }
      break;

   case BUTTON_RELEASED:
      break;
   }
}

void VolumeDn(BYTE key)
{
   static BYTE prevAudioVolume = 0;

   switch (key)
   {
   case BUTTON_PRESSED:

      if (UserPrefAudioVolume == 0)
      {
         UserPrefAudioVolume = 0;
      }
      else
      {
         UserPrefAudioVolume -= 1;
      }

      if (prevAudioVolume != UserPrefAudioVolume)
      {
         prevAudioVolume = UserPrefAudioVolume;

         AdjustAudioVolume();

         OSDRequestMsg(gmvb_MainChannel, MTO_DrawVolumeSlider, REQUEST_NOW);
      }
      break;

   case BUTTON_RELEASED:
      break;
   }
}

BYTE GetUserPrefAudioVolume(void)
{
#if defined(UserPrefAudioVolume)
   return (BYTE) UserPrefAudioVolume;
#else
   return 0;
#endif
}

BYTE GetUserPrefAudioMute(void)
{
#if defined(UserPrefAudioMute)
   return (BYTE) UserPrefAudioMute;
#else
   return 0;
#endif
}

BYTE GetUserPrefAudioDownMix(void)
{
#if defined(UserPrefAudioDownMix)
   return (BYTE) UserPrefAudioDownMix;
#else
   return 0;
#endif
}

WORD GetUserPrefAVSDelayTime(void)
{
#if defined(UserPrefAVSDelayTime)
   return (WORD) UserPrefAVSDelayTime;
#else
   return 0;
#endif
}

BYTE GetUserPrefAudioMainInput(void)
{
#if defined(UserPrefAudioMainInput)
   return (BYTE) UserPrefAudioMainInput;
#else
   return 0;
#endif
}

BYTE GetUserPrefAudioMainOutput(void)
{
#if (FEATURE_DP_HUB == ENABLE)
    return gmd_AUDIO_TAR_SPDIF_OUT;
#endif

#if defined(UserPrefAudioMainOutput)
   return (BYTE) UserPrefAudioMainOutput;
#else
   return 0;
#endif
}

BYTE GetUserPrefAudioPipInput(void)
{
#if defined(UserPrefAudioPipInput)
   return (BYTE) UserPrefAudioPipInput;
#else
   return 0;
#endif
}

BYTE GetUserPrefAudioPipOutput(void)
{
#if defined(UserPrefAudioPipOutput)
   return (BYTE) UserPrefAudioPipOutput;
#else
   return 0;
#endif
}

BYTE GetUserPrefPipMode(void)
{
#if defined(UserPrefPipMode)
   return (BYTE) UserPrefPipMode;
#else
   return 0;
#endif
}

BYTE GetUserPrefCurrentInputMain(void)
{
#if defined(UserPrefCurrentInputMain)
   return (BYTE) UserPrefCurrentInputMain;
#else
   return 0;
#endif
}

BYTE GetUserPrefCurrentInputPip(void)
{
#if defined(UserPrefCurrentInputPip)
   return (BYTE) UserPrefCurrentInputPip;
#else
   return 0;
#endif
}

//*********************************  END  **************************************
