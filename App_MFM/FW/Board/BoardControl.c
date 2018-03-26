/*
	$Workfile:   BoardControl.c  $
	$Revision: 1.16 $
	$Date: 2013/01/23 03:11:31 $
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
// MODULE:      BoardControl.c
//
// USAGE:       This module contains board specific initialization
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "System\all.h"
#include "StdLibInc\gm_Register.h"

extern BYTE ROM IR_Values[64];

// Add 3 boards
#if	defined(EV1_93xx_BOARD)
#include	"Board\EV1-93xx\GpioControl.c"
#elif	defined(EV2_93xx_BOARD)
#include	"Board\EV2-93xx\GpioControl.c"
#elif	defined(RD1_93xx_BOARD)
#include	"Board\RD1-93xx\GpioControl.c"
#elif	defined(ST_4K2K_93xx_BOARD)
#include	"Board\ST4K2K\GpioControl.c"
#endif

//******************************************************************************
//
// FUNCTION       : void BoardInit(void)
//
// USAGE          : Initialize all the system board devices by calling individual functions
//
// INPUT          :
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
void BoardInit(void)
{
   // Initialize I2C
   I2C_Init();
   // wake-up the system periphereals
   InitSystemGpio();
   // SRESET
   GpioSoftReset(3);

   // Turn on keypad LED Green
   // 20110208 #1, Do not allow Amber LED control before LPM initial finish.
   // (Green LED control by Mission, Amber LED control by LPM).
   //KeypadLightControl(KEYPAD_LED_GREEN, LED_NORMAL);
   GpioLed1Keypad(TRUE);
}

BYTE GetCurrentBoardInfo(void)
{
#if defined(RD3_93xx_BOARD)
	#if(BOARD_USED == BOARD_93xx_CJ2)
		return BOARD_CJ2;

	#else
        return BOARD_RD3;
	#endif
#elif defined(RD2_93xx_BOARD)
                                    return BOARD_RD2;
#elif defined(RD1_93xx_BOARD_REV_B)
                                    return BOARD_RD1_REV_B;
#elif defined(RD1_93xx_BOARD)

	#if(BOARD_USED == BOARD_93xx_CJ2)
		return BOARD_CJ2;		
	#else
		return BOARD_RD1;
	#endif
	
#elif defined(EV3_93xx_BOARD)
                                    return BOARD_EV3;
#elif defined(EV2_93xx_BOARD)
                                    return BOARD_EV2;
#elif defined(EV1_93xx_BOARD)
                                    return BOARD_EV1;
#elif defined(ST_4K2K_93xx_BOARD)
                                    return BOARD_4K2K;
#endif
}

BYTE GetIrPowerKeyVal(BYTE KeyItem)
{
   BYTE i = 1;
   while (i < 64)
   {
      if (IR_Values[i] == KeyItem)
         return IR_Values[i-1];
      i += 2;
   }
   return 0;
}

gmt_BEHAVIOR_ID Get_Behavior_ID(void)
{
#if defined(RD3_93xx_BOARD)
                                    return BEHAVIOR_ID_93xx_RD3;
#elif defined(RD2_93xx_BOARD)
                                    return BEHAVIOR_ID_93xx_RD2;
#elif defined(RD1_93xx_BOARD_REV_B)
                                    return BEHAVIOR_ID_93xx_RD1_REV_B;
#elif defined(RD1_93xx_BOARD)
                                    return BEHAVIOR_ID_93xx_RD1;
#elif defined(EV3_93xx_BOARD)
                                    return BEHAVIOR_ID_93xx_EV3;
#elif defined(EV2_93xx_BOARD)
                                    return BEHAVIOR_ID_93xx_EV2;
#elif defined(EV1_93xx_BOARD)
                                    return BEHAVIOR_ID_93xx_EV1;
#elif defined(ST_4K2K_93xx_BOARD)
   switch (GetChipID_4K2K())
   {
   default:
   case CHIPID_4K2K_NONE:           return BEHAVIOR_ID_UNKNOWN;
   case CHIPID_4K2K_L:              return BEHAVIOR_ID_93xx_ST_4K2K_L;
   case CHIPID_4K2K_FE:             return BEHAVIOR_ID_93xx_ST_4K2K_FE;
   case CHIPID_4K2K_R:              return BEHAVIOR_ID_93xx_ST_4K2K_R;
   }
#else
                                    return BEHAVIOR_ID_UNKNOWN;
#endif
}

/*********************************  END  **************************************/
