/*
	$Workfile:   gm_OsdSystem.c  $
	$Revision: 1.4 $
	$Date: 2011/12/14 09:46:48 $
*/
#define __GM_OSD_SYSTEM_C__
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
// MODULE:      gm_OsdSystem.c
//
// USAGE:       This module contains gmc_ Osd library constants for
//              configuring the Osd library.
//
//              Any of the variables may be overrided in Application by placing
//              the file in the application with the overrided value for the
//              desired variable
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#include "system\all.h"
#include "StdLibInc\gm_Register.h"

//******************************************************************************
//  D E F I N I T I O N S
//******************************************************************************

//******************************************************************************
//  O S D     L I B R A R Y    C O N S T A N T S
//******************************************************************************

/* If set to TRUE, debug messages will be printed in GProbe for all state transitions.
** Both entering and exiting a state will be printed out.
** For WB3, the state id number will be printed out.
** For WB4, the state name will be printed out.
*/
#ifdef WB_ENABLE_DEBUG_INFO_STATES
ROM BYTE gmc_OsdDebugStateEnabled = TRUE;
#else
ROM BYTE gmc_OsdDebugStateEnabled = FALSE;
#endif

#ifndef WB_ENABLE_DEBUG_INFO_STATES
char ROM* ROM* ROM gmv_ThreadsDebugArrayDef[] =
{
   NULL_PTR
};

char ROM* ROM gmv_ThreadsDebugInfo[] =
{
   "0"
};
#endif


//************************************* END ************************************

