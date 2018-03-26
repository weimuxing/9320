/*
	$Workfile:   osd_timer.h  $
	$Revision: 1.3 $
	$Date: 2011/12/15 03:12:03 $
*/
#ifndef __OSD_TIMER_H__
#define __OSD_TIMER_H__
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
// MODULE:      osd_timer.h
//
// USAGE:
//
//******************************************************************************


#define TICKS_PER_MINUTE	60	//Sleeptimer



typedef enum SleepTimerEnum
{
   SLEEP_OFF = 0,
   SLEEP_15MIN = 15,
   SLEEP_30MIN = 30,
   SLEEP_60MIN = 60,
   SLEEP_90MIN = 90,
   SLEEP_120MIN = 120
}SleepTimer;


enum
{
   WAKE_ON = 1,
   WAKE_OFF = 0
};

//******************************************************************************
//  E X T E R N A L    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
//Real Time Clock functions:
BOOL IsSleepAlarmUsed(void);
BOOL IsWakeAlarmUsed(void);
void SetWakeAlarm(void);
void SetSleepAlarm(void);
void SetDateTime(void);
void GetDateTime(void);
void GetSleepMinsLeft(void);
void SleepAlarmCalc(BYTE B_AddedMinutes);
BOOL IsSleepAlarmEarlier(void);
WORD ConvertToMinutes(BYTE B_AmPm, BYTE B_Hr, BYTE B_Mins);
// TIME VALUE FUNCTIONS:
void AdjustMin(void);
void AdjustHour(void);
void ToggleAMPM(void);
void AdjustDate(void);
void AdjustMonth(void);
void AdjustYear(void);
void AdjustWakeMin(void);
void AdjustWakeHour(void);
void AdjustWakeDay(void);
void ToggleWakeAMPM(void);

void ResetTimerUserPrefs(void);

#endif
