/*
	$Workfile:   gm_adapt_modedetection.c  $
	$Revision: 1.4 $
	$Date: 2011/12/13 10:32:44 $
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
// MODULE:      gm_adapt_modedetection.c
//
// USAGE:       This module contains changes to the library detection faunctions
//				behavior changes
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#include "System\All.h"

//******************************************************************************
//  E X T E R N A L    F U N C T I O N S
//******************************************************************************

//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************

//******************************************************************************
//  D E F I N E S
//******************************************************************************


//******************************************************************************
//  S T A T I C    V A R I A B L E S
//******************************************************************************

//*****************************************************************************
//  S T A T I C    F U N C T I O N    P R O T O T Y P E S
//*****************************************************************************

//*****************************************************************************
//  C O D E
//*****************************************************************************

//******************************************************************************
//
// FUNCTION       : BOOL gm_adapt_InputTimingTransitionFromStable(gmt_PHY_CH Channel)
//
// USAGE          :
//
// INPUT          : Channel
//
// OUTPUT         : TRUE or FALSE
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BOOL gm_adapt_InputTimingTransitionFromStable(gmt_PHY_CH Channel)
{
   if (IsDecoderUsed(Channel) == TRUE)
      return FALSE;
   else
      return gm_InputTimingTransitionFromStable(Channel);
}


