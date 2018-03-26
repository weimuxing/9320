/*
	$Workfile:   gm_timer.h  $
	$Revision: 1.3 $
	$Date: 2011/12/14 10:35:32 $
*/
#ifndef __GM_TIMER_H__
#define __GM_TIMER_H__
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
// MODULE:      gm_timer.h
//
// USAGE:       Header file for system timer definition
//
//******************************************************************************

enum
{
   GSEL_START_1ms = MAX_TMR,				// Start of 1ms
   CHA_DETECT_1ms_TMR=GSEL_START_1ms, 		// Channel A mode detection
   CHB_DETECT_1ms_TMR,				   		// Channel B mode detection
   KBD_1ms_TMR,							// Keyboard timer
   IR_1ms_TMR,								// IR timer
   CC_1ms_TMR,								// CC for PIP timer
   RTS_1s_TMR,								// RTC status request timer
   CA_1ms_TMR,								// PIP Content Advisory or Program Rating timer
   GSEL_END_1ms,							//
   //
   GSEL_START_10ms = GSEL_END_1ms,			// Start of 10ms
   KBD_10ms_TMR = GSEL_START_10ms,			// Keyboard timer
   GSEL_END_10ms,							//
   //
   GSEL_START_100ms = GSEL_END_10ms,		// Start of 100ms
   GSEL_END_100ms = GSEL_START_100ms,		//
   //
   GSEL_START_1s = GSEL_END_100ms,			// Start of 1s
   GSEL_END_1s = GSEL_START_1s,			//
};

#define NUM_GSEL_1ms_TIMER		(GSEL_END_1ms  - GSEL_START_1ms)
#define NUM_GSEL_10ms_TIMER     (GSEL_END_10ms - GSEL_START_10ms)
#define NUM_GSEL_100ms_TIMER    (GSEL_END_100ms- GSEL_START_100ms)
#define NUM_GSEL_1s_TIMER       (GSEL_END_1s   - GSEL_START_1s)
#define NUM_GSEL_TIMER          (GSEL_END_1s   - GSEL_START_1ms)

#endif

//*********************************  END  **************************************
