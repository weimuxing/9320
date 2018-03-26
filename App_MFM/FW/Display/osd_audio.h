/*
	$Workfile:   osd_audio.h  $
	$Revision: 1.10 $
	$Date: 2012/08/14 08:27:00 $
*/
#ifndef __OSD_AUDIO_H__
#define __OSD_AUDIO_H__
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
// MODULE:      osd_audio.h
//
// USAGE:
//
//******************************************************************************
typedef enum
{
   BUTTON_RELEASED,
   BUTTON_PRESSED
} VolumeButtonState;

typedef enum
{
   #if 1
   DEFAULT_VOLUME_VALUE = 14,
   MINIMAL_VOLUME_VALUE = 0,
   MAXIMAL_VOLUME_VALUE = 22

   #else
   DEFAULT_VOLUME_VALUE = 20,
   MINIMAL_VOLUME_VALUE = 0,
   MAXIMAL_VOLUME_VALUE = 32
   #endif
} VolumeConstraints;

//******************************************************************************
//  E X T E R N A L    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
void AdjustAudio(void);
void AdjustAudioVolume(void);

void ToggleMute(void);

BYTE GetMaximumVolumeOSDStep(void);
void VolumeUp(BYTE key);
void VolumeDn(BYTE key);

BYTE GetUserPrefAudioVolume(void);
BYTE GetUserPrefAudioMute(void);
BYTE GetUserPrefAudioDownMix(void);
WORD GetUserPrefAVSDelayTime(void);
BYTE GetUserPrefAudioMainInput(void);
BYTE GetUserPrefAudioMainOutput(void);
BYTE GetUserPrefAudioPipInput(void);
BYTE GetUserPrefAudioPipOutput(void);
BYTE GetUserPrefPipMode(void);
BYTE GetUserPrefCurrentInputMain(void);
BYTE GetUserPrefCurrentInputPip(void);

#endif
