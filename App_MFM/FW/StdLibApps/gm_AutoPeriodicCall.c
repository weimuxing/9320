/*
	$Workfile:   gm_AutoPeriodicCall.c  $
	$Revision: 1.3 $
	$Date: 2011/12/13 10:32:44 $
*/
//******************************************************************
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
//================================================================
//
//  MODULE: gm_AutoPeriodicCall.c
//
//  USAGE : This module contains the stub for gm_AutoPeriodicCall which
//			can be used to abort the AutoAdjust before it complete
//
//******************************************************************

//******************************************************************
//                  I N C L U D E    F I L E S
//******************************************************************

#include "system\all.h"

//******************************************************************
//              G L O B A L    V A R I A B L E S
//******************************************************************

//******************************************************************
//              L O C A L    F U N C T I O N S
//******************************************************************

//******************************************************************************
//
// FUNCTION       : gmt_RET_STAT gm_AutoPeriodicCall(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : gmd_FALSE or gmd_TRUE
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
gmt_RET_STAT gm_AutoPeriodicCall(void)
{
   if (B_AutoAdjustStatus[gmvb_MainChannel] == AA_ABORT)	// Abort AA
      return gmd_FALSE;				// Yes
   else
      return gmd_TRUE;				// No
}
