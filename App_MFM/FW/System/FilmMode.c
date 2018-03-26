/*
	$Workfile:   FilmMode.c  $
	$Revision: 1.3 $
	$Date: 2011/12/13 10:32:44 $
*/
#define __FILMMODE_C__

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
// MODULE:      FilmMode.c
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "System\all.h"

//******************************************************************************
//  D E F I N E S
//******************************************************************************

//#define _DEBUG_MSG
#ifdef _DEBUG_MSG
#define msg(x,y)		gm_Print(x,y)
#else
#define msg(x,y)
#endif

//******************************************************************************
//  E X T E R N S
//******************************************************************************

extern void	GpioLedFilmMode1Init(void);
extern void	GpioLedFilmMode0Init(void);
extern void	GpioLedFilmMode0(BYTE B_State);
extern void	GpioLedFilmMode1(BYTE B_State);

//******************************************************************************
//  L O C A L S
//******************************************************************************

static WORD W_LastFilmModeState = 0;
static BOOL B_StaticImage=FALSE;

//******************************************************************************
//
// FUNCTION       : void FilmModeHdlr(void)
//
// USAGE          : Called from Main Loop to Handler Film Mode Changes
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
void FilmModeHdlr(void)
{
   //
   // Mode Stable and Complete and Film Mode Detection Enabled
   //
   if ((stInput[CH_A].ModeFlags & gmd_MODE_COMPLETE) && (gm_IsFilmModeEnabled() == TRUE))
   {
      //
      // Change to Film Status
      //
      if (gmvw_FilmModeState != W_LastFilmModeState)
      {
         // TODO: The changes required by a Film Change are caught by gm_HardwareUpdate

         W_LastFilmModeState = gmvw_FilmModeState;
      }
   }

   //
   // Debug LEDS for MADI/2:2/3:2 (Toggling LEDs on and off)
   //
   //	-----------
   //	|D400|D402|<- RD1
   //	|D600|D602|<- RD2
   //	------------------
   //	|OFF |OFF | MADI | (VT_MODE)
   //	------------------
   //	|ON  |OFF | 2:2  | (GFX_MODE)
   //	------------------
   //	|ON  |ON  | 3:2  | (AFP_MODE)
   //	------------------
   //	|OFF |ON  |SPARE | (others)
   //	------------------


   //
   //LED402/602
   //

   switch ( gmvw_FilmModeState )
   {
      case VT_MODE:
      case GFX_MODE:
      default:
         GpioLedFilmMode1(FALSE);	 //LED402/602 OFF when MADI or 2:2
         break;

      case AFP_MODE:
      case SM_MODE:
      case AFP_TEMP_VT_MODE:
      case SWITCHTO_AFP_MODE:
      case GFX_TEMP_VT_MODE:
         GpioLedFilmMode1(TRUE);	//LED402/602 ON when 3:2 or others
         break;
   }

   //
   //LED400/600
   //

   switch ( gmvw_FilmModeState )
   {
      case VT_MODE:
      case SM_MODE:
      case AFP_TEMP_VT_MODE:
      case SWITCHTO_AFP_MODE:
      case GFX_TEMP_VT_MODE:
      default:
         GpioLedFilmMode0(FALSE);	//LED400/600 OFF when MADI or others
         break;

      case GFX_MODE:
      case AFP_MODE:
         GpioLedFilmMode0(TRUE);	//LED400/600 ON when 3:2 or 2:2
         break;
   }

   //
   // Mode Stable and Complete
   // Monitor the status of input static image
   //
   if (stInput[CH_A].ModeFlags & gmd_MODE_COMPLETE)
   {
      gm_StaticImageDetectHandle();				// Monitor status
      if (gm_StaticImageDetectStatus(CH_A) != B_StaticImage)
      {
         B_StaticImage = gm_StaticImageDetectStatus(CH_A);
         if (B_StaticImage == TRUE)
            msg("STATIC image",0);
         else
            msg("MOTION image",0);
      }
   }
}
/***********************************  END  ************************************/

