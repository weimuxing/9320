/*
	$Workfile:   osd_timer.c  $
	$Revision: 1.6 $
	$Date: 2011/12/15 03:12:03 $
*/
#define __OSD_TIMER_C__
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
// MODULE:     	osd_timer.c
//
// USAGE:		This module contains standard osd adjusters for
//				TV timer control (e.g. set time, wake time, sleep time)
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

//
// Debug print message
//
#define _DEBUG
#ifdef _DEBUG
#define  msg(a)            		gm_Print(a,0)
#define  msg_i(a,b)        	 	gm_Print(a,b)
#else
#define  msg(a)
#define  msg_i(a,b)
#endif

//
// RTC definition for calendar
//
#define NUM_DAYS_PER_WEEK			7
#define NUM_MINS_PER_HOUR			60
#define NUM_HOURS_PER_AM_PM			12
#define NUM_MONTHS_PER_YEAR			12
#define NUM_DAYS_PER_MONTH			MaxDaysInMonth()
#define NUM_YEARS					255

enum
{
   JANUARY = 1,
   FEBRUARY,
   MARCH,
   APRIL,
   MAY,
   JUNE,
   JULY,
   AUGUST,
   SEPTEMBER,
   OCTOBER,
   NOVEMBER,
   DECEMBER
};

enum
{
   AM,
   PM
};

//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************

//******************************************************************************
// S T A T I C   F U N C T I O N S
//******************************************************************************

static void AdjustMinMaxBaseByte	(BYTE* Bp_Value, BYTE B_Min, WORD B_Max);
static void CalcSleepAlarm			(BYTE B_Minutes);

//******************************************************************************
//  C O D E
//******************************************************************************
#ifdef ADJUSTER_YEAR_DECLARED
//******************************************************************************
//
// FUNCTION       : static BOOL IsLeapYear(void)
//
// USAGE          : Checks if UserPrefYear is a leap year
//
// INPUT          : None
//
// OUTPUT         : TRUE or FALSE
//
// GLOBALS        : UserPrefYear	(R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
static BOOL IsLeapYear(void)
{
   if (UserPrefYear % 100 != 0)
      return (UserPrefYear % 4 == 0);
   else
      return (UserPrefYear % 400 == 0);
}
#endif

#if defined(ADJUSTER_MONTH_DECLARED) || defined(ADJUSTER_YEAR_DECLARED)
//******************************************************************************
//
// FUNCTION       : static BYTE MaxDaysInMonth(void)
//
// USAGE          : Returns number of days in current month
//
// INPUT          : None
//
// OUTPUT         : Number of days in current month
//
// GLOBALS        : UserPrefMonth	(R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
static BYTE MaxDaysInMonth(void)
{
   BYTE B_NumDays;

   switch (UserPrefMonth)
   {
      case JANUARY:
      case MARCH:
      case MAY:
      case JULY:
      case AUGUST:
      case OCTOBER:
      case DECEMBER:
         B_NumDays = 31;
         break;

      case FEBRUARY:
         if (IsLeapYear())
            B_NumDays = 29;
         else
            B_NumDays = 28;
         break;

      default:
         B_NumDays = 30;
         break;
   }
   return B_NumDays;
}
#endif

#if defined(ADJUSTER_MONTH_DECLARED) || defined(ADJUSTER_YEAR_DECLARED)
//******************************************************************************
//
// FUNCTION       : static void CheckDays(void)
//
// USAGE          : Makes sure that UserPrefDate cannot be greater than NUM_DAYS_PER_MONTH
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : UserPrefDate	(R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
static void CheckDays(void)
{
   if (UserPrefDate > NUM_DAYS_PER_MONTH)
      UserPrefDate = NUM_DAYS_PER_MONTH;
}
#endif

#ifdef ADJUSTER_MIN_DECLARED
//******************************************************************************
//
// FUNCTION       : void AdjustMin(void)
//
// USAGE          : Increments to next minute
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : UserPrefMin	(R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AdjustMin(void)
{
   AdjustMinMaxBaseByte((BYTE *)&UserPrefMin, 0, NUM_MINS_PER_HOUR-1);
}
#endif

#ifdef ADJUSTER_HOUR_DECLARED
//******************************************************************************
//
// FUNCTION       : void AdjustHour(void)
//
// USAGE          : Increments to next hour
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : UserPrefHour	(R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AdjustHour(void)
{
   AdjustMinMaxBaseByte((BYTE *)&UserPrefHour, 1, NUM_HOURS_PER_AM_PM);
}
#endif

#ifdef ADJUSTER_AMPM_DECLARED
//******************************************************************************
//
// FUNCTION       : void ToggleAMPM(void)
//
// USAGE          : Toggles UserPrefAMPM to be either AM or PM (time)
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : UserPrefAMPM	(R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void ToggleAMPM(void)
{
   UserPrefAMPM ^= 1;
}
#endif

#ifdef ADJUSTER_DATE_DECLARED
//******************************************************************************
//
// FUNCTION       : void AdjustDate(void)
//
// USAGE          : Increments to next day of the month
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : UserPrefDate	(R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AdjustDate(void)
{
   AdjustMinMaxBaseByte((BYTE *)&UserPrefDate, 1, NUM_DAYS_PER_MONTH);
}
#endif

#ifdef ADJUSTER_MONTH_DECLARED
//******************************************************************************
//
// FUNCTION       : void AdjustMonth(void)
//
// USAGE          : Increments to next month
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : UserPrefMonth	(R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AdjustMonth(void)
{
   AdjustMinMaxBaseByte((BYTE *)&UserPrefMonth, 1, NUM_MONTHS_PER_YEAR);
   //
   // avoid setting month with incorrect days in current month
   //
   CheckDays();
}
#endif

#ifdef ADJUSTER_YEAR_DECLARED
//******************************************************************************
//
// FUNCTION       : void AdjustYear(void)
//
// USAGE          : Increments to next year
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : UserPrefYear	(R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AdjustYear(void)
{
   AdjustMinMaxBaseByte((BYTE *)&UserPrefYear, 0, NUM_YEARS);
   CheckDays();	//avoid setting year with incorrect days in current month
}
#endif

#ifdef ADJUSTER_WAKEMIN_DECLARED
//******************************************************************************
//
// FUNCTION       : void far AdjustWakeMin(void)
//
// USAGE          : Adjusts Wake minute when user is entering the wake alarm
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : UserPrefWakeMin	(R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void far AdjustWakeMin(void)
{
   AdjustMinMaxBaseByte((BYTE *)&UserPrefWakeMin, 0, NUM_MINS_PER_HOUR-1);
}
#endif

#ifdef ADJUSTER_WAKEHOUR_DECLARED
//******************************************************************************
//
// FUNCTION       : void far AdjustWakeHour(void)
//
// USAGE          : Adjusts Wake hour when user is entering the wake alarm
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : UserPrefWakeHour	(R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void far AdjustWakeHour(void)
{
   AdjustMinMaxBaseByte((BYTE *)&UserPrefWakeHour, 1, NUM_HOURS_PER_AM_PM);
}
#endif

#ifdef ADJUSTER_WAKEDAY_DECLARED
//******************************************************************************
//
// FUNCTION       : void far AdjustWakeDay(void)
//
// USAGE          : Adjusts Wake day (Mon, Tues, etc.) when user is entering the wake alarm
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : UserPrefWakeHour	(R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void far AdjustWakeDay(void)
{
   AdjustMinMaxBaseByte((BYTE *)&UserPrefWakeDay, 1, NUM_DAYS_PER_WEEK);
}
#endif

#ifdef ADJUSTER_WAKEAMPM_DECLARED
//******************************************************************************
//
// FUNCTION       : void far ToggleWakeAMPM(void)
//
// USAGE          : Adjusts Wake AM/PM when user is entering the wake alarm
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : UserPrefWakeHour	(R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void far ToggleWakeAMPM(void)
{
   gm_Printf("1 WakeAMPM=%d", UserPrefWakeAMPM);
   UserPrefWakeAMPM ^= 1;
   gm_Printf("2 WakeAMPM=%d", UserPrefWakeAMPM);
}
#endif

//******************************************************************************
//
// FUNCTION       : static void AdjustMinMaxBaseByte(BYTE* Bp_Value, BYTE B_Min, WORD B_Max)
//
// USAGE          :
//
// INPUT          : Bp_Value
//                  B_Min
//                  B_Max
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
static void AdjustMinMaxBaseByte(BYTE* Bp_Value, BYTE B_Min, WORD B_Max)
{
//
// TO DO:  Must not use MIN_TV_CHANNEL and MAX_TV_CHANNEL being defined and passed.  Instead, call a FW function
//         that will check TV Tuner CH to see if channel is valid.

   BYTE B_Direction = gm_OsdGetActionByteParameter();
   switch (B_Direction)
   {
      case UP:
         if (*Bp_Value < B_Max)
            (*Bp_Value)++;
         else
            *Bp_Value = B_Min;
         break;

      case DOWN:
         if (*Bp_Value > B_Min)
            (*Bp_Value)--;
         else
            *Bp_Value = B_Max;
         break;

      default:
         break;
   }
}


#ifdef Action_SetDateTime_WB_Used
//******************************************************************************
//
// FUNCTION       : void SetDateTime(void)
//
// USAGE          : Sets the real time clock with user pref variables
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : UserPrefDate (w)
//                  UserPrefAMPM		(W)
//                  UserPrefMin (W)
//                  UserPrefHour  (W)
//                  UserPrefYear		(W)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void SetDateTime(void)
{
   BYTE B_DayTemp;
   WORD B_YearTemp = UserPrefYear + 2000;

   SetRealTimeClockDate(UserPrefDate);
   SetRealTimeClockYears(UserPrefYear);
   SetRealTimeClockMonths(UserPrefMonth);
   SetRealTimeClockHour(UserPrefHour, AMPM_MODE, UserPrefAMPM);
   SetRealTimeClockMinutes(UserPrefMin);
   SetRealTimeClockSeconds(0);

   // Taken from  Journal of Recrational Mathematics, Vol. 22, No. 4, 1990, p. 280:
   if (UserPrefMonth < 3)
   {
      B_YearTemp--;
      B_DayTemp = ( (23*UserPrefMonth/9) + UserPrefDate + 4 + (UserPrefYear + 2000)
                    + (B_YearTemp/4) - (B_YearTemp/100) + (B_YearTemp/400)) % 7;
   }
   else
   {
      B_DayTemp = ( (23*UserPrefMonth/9) + UserPrefDate + 4 + (UserPrefYear + 2000)
                    + (B_YearTemp/4) - (B_YearTemp/100) + (B_YearTemp/400) - 2) % 7;
   }

   if (B_DayTemp == 0)
      B_DayTemp = SUNDAY;
   //gm_Print("Day of week: %d",B_DayTemp);

   SetRealTimeClockDay(B_DayTemp);

   if (UserPrefSleepDay)		//if Sleep alarm is on, we need to recalculate the sleep alarm
   {
      CalcSleepAlarm(UserPrefSleepMinsLeft);	//set it with the amount of time left
      gm_Print("recalculate and set sleep alarm",0);
   }

}
#endif

#ifdef Action_SetSleepAlarm_WB_Used
//******************************************************************************
//
// FUNCTION       : void SetSleepAlarm(void)
//
// USAGE          : Sets sleep alarm by first converting the parameter into day, ampm, min, hour
//                  Stores the UserPref variables into the alarm clock register if it's earlier than
//                  the wake alarm.
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : B_RtcProcessingRequired	(W)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void SetSleepAlarm(void)
{
   // not support RTC
}
#endif

#ifdef Action_SetSleepAlarm_WB_Used
//******************************************************************************
//
// FUNCTION       : static void CalcSleepAlarm(BYTE B_Minutes)
//
// USAGE          : Given minutes, calculates when sleep alarm should be triggered in real time.
//                  Then sets the alarm if sleep alarm occurs earlier.
//
// INPUT          : B_Minutes
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
static void CalcSleepAlarm(BYTE B_Minutes)
{
   UNUSED_VAR(B_Minutes);
   // not support RTC
}
#endif

#ifdef Action_SetWakeAlarm_WB_Used
//******************************************************************************
//
// FUNCTION       : void SetWakeAlarm(void)
//
// USAGE          : Sets wake alarm.
//                  Stores the UserPref variables into the alarm clock register if it's earlier than
//                  the wake alarm.
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : B_RtcProcessingRequired	(W)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void SetWakeAlarm(void)
{
   // not support RTC
}
#endif
#ifdef Action_SetWakeAlarm_WB_Used
//******************************************************************************
//
// FUNCTION       : void GetSleepMinsLeft(void)
//
// USAGE          : Find sthe amount of time that sleep timer will kick in.  Stores this value in
//                  UserPrefSleepMinsLeft
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void GetSleepMinsLeft(void)
{
   // not support RTC
}
#endif


//******************************************************************************
//
// FUNCTION       : BOOL IsSleepAlarmUsed(void)
//
// USAGE          : Checks to see if sleep alarm is being used by the application
//
// INPUT          : None
//
// OUTPUT         : TRUE or FALSE
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BOOL IsSleepAlarmUsed(void)
{
   if (UserPrefSleepHour || UserPrefSleepMin || UserPrefSleepAMPM || UserPrefSleepDay)
      return TRUE;
   else
      return FALSE;
}

//******************************************************************************
//
// FUNCTION       : BOOL IsWakeAlarmUsed(void)
//
// USAGE          : Checks to see if sleep alarm is being used by the application
//
// INPUT          : None
//
// OUTPUT         : TRUE or FALSE
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BOOL IsWakeAlarmUsed(void)
{
   if (UserPrefWakeHour || UserPrefWakeMin || UserPrefWakeAMPM || UserPrefWakeDay)
      return TRUE;
   else
      return FALSE;
}

//******************************************************************************
//
// FUNCTION       : BOOL IsSleepAlarmEarlier(void)
//
// USAGE          : Checks what timer should be stored in RTC
//
// INPUT          : None
//
// OUTPUT         : TRUE - sleep alarm is earlier
//                  FALSE - wake alarm is earlier
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BOOL IsSleepAlarmEarlier(void)
{
   // not support RTC
   return TRUE;
}

//******************************************************************************
//
// FUNCTION       : void SleepAlarmCalc(BYTE B_AddedMinutes)
//
// USAGE          : Modifies the user pref sleep time parameters by adding B_AddedMinutes to the
//                  current time. This makes the sleep time parameters equal to the time that
//                  sleep will occur.
//
// INPUT          : B_AddedMinutes
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void SleepAlarmCalc(BYTE B_AddedMinutes)
{
   UNUSED_VAR(B_AddedMinutes);
   // not support RTC
}

//******************************************************************************
//
// FUNCTION       : WORD ConvertToMinutes(BYTE B_AmPm, BYTE B_Hr, BYTE B_Mins)
//
// USAGE          : Converts time in 12 hour am/pm mode into minutes from midnight
//
// INPUT          : B_AmPm
//                  B_Hr
//                  B_Mins
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
WORD ConvertToMinutes(BYTE B_AmPm, BYTE B_Hr, BYTE B_Mins)
{
   if (B_Hr == 12)
      B_Hr = 0;

   if (B_AmPm)
      B_Hr += 12;

   return((B_Hr*60) + B_Mins);
}

//******************************************************************************
//
// FUNCTION       : void ResetTimerUserPrefs(void)
//
// USAGE          : Resets the user prefs related to the real time clock
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void ResetTimerUserPrefs(void)
{
   UserPrefSleepMin = 0;
   UserPrefSleepHour = 0;
   UserPrefSleepAMPM= 0;
   UserPrefSleepDay = 0;
   UserPrefSleepMinsLeft = SLEEP_OFF;
}
//*********************************  END  **************************************
