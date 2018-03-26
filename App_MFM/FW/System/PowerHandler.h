/*
	$Workfile:   PowerHandler.h  $
	$Revision: 1.18 $
	$Date: 2013/04/25 03:42:58 $
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
// MODULE:      PowerHandler.H
//
// USAGE:
//
//******************************************************************************

#ifndef POWERHANDLER_H
#define	POWERHANDLER_H

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "System\all.h"


//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************
#define PWR_TRANSITION_COUNT DEEPSLEEP_TIME // 20
#define POWER_STATE_OFF PWR_SW_OFF // 0
#define POWER_STATE_ON  PWR_SW_ON  // 1
#define FORCE_SLEEP		0x80//120618 Edward add special flag
#define FORCE_SLEEP_NoCable		0x8000//120627 Edward for cable status send to LPM due to DP is get from mission if mission is alive


typedef enum
{
   Pwr_On,			//normal functioning (syncs on both VS and HS)
   Pwr_Sleep,
   Pwr_DeepSleep,    // syncs missing both VS and HS
   Pwr_MissionSleep = Pwr_DeepSleep, // syncs missing both VS and HS
   Pwr_Off,			// soft power off (no sync monitoring, only power key can wake up system)
   Pwr_Transition
} gmt_PwrStateType;

// 20100519, For Athena powerhandler
// Wakeup event from LPM to Mission, Mission wakeup level will base on different case.
typedef enum
{
   gmd_L2M_None	= BIT0,
   gmd_AdcKeyPowerUp_Requested	= BIT1,
   gmd_IrPowerUp_Requested	= BIT2,
   gmd_DdcPowerUp_Requested = BIT3,
   gmd_CecPowerUp_Requestd = BIT3,
   gmd_Sync_Detected	= BIT4,
} gmt_MESSAGE_L2M;

//For IPC command gmd_MESSAGE_CHECK_STATUS
typedef enum
{
   PWR_SW_OFF,
   PWR_SW_ON,
   PWR_SW_SIGDET_ON,
   PWR_SW_NUM
}gmt_Power_Mode_Switch;

typedef enum
{
   LPM_WAKEUP_CPHY0,
   LPM_WAKEUP_CPHY1,
   LPM_WAKEUP_CPHY2,
   LPM_WAKEUP_ASDET,
   LPM_WAKEUP_KEYPAD,
   LPM_WAKEUP_AC_ON,
   LPM_WAKEUP_DET_NUM
}gmt_LPM_Wakeup_Detect;

#ifdef AUTO_SCAN_INPUT
extern /*ALL_BOARDS_INPUT_PORT*/BYTE ROM ASInputRings[AUTOSCANINPUT_RINGS_SIZE];
#endif

//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
void PowerHandler(void);
void PowerRequestOff(void);
void PowerRequestOn(void);
void PowerDeepSleepInRequest(void);
void PowerDeepSleepOutRequest(void);
void PowerTransition(void);
void PwrTransitionKeyPressed(BOOL flag);
gmt_PwrStateType GetPowerState(void);
#ifdef INACTIVE_DEVICE_POWER_SAVING 
void PowerUpDevice(BYTE B_Port);
void PowerDownDevice(BYTE B_Port);
#endif
BOOL IsPBP_AthenaL_SyncValid(void);
BOOL IsSyncOn(void);

WORD LpmWakeupPortInfo(gmt_UserDefConnNames B_InputConnector);//120405 Edward move to here

// Louis ... Patch for 7770, 7870, 7970 into LPM mode issue
void HDMI_Input_IBD_Reset(void);

#endif
//*********************************  END  **************************************
