/*
	$Workfile:   osd_cmd_sram.c  $
	$Revision: 1.4 $
	$Date: 2012/04/26 05:59:53 $
*/
#define __OSD_CMD_SRAM_C__
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
// MODULE:      osd_cmd_sram.c
//
// USAGE:       Reserved space for OSD COMMAND SRAM
//
//******************************************************************************

#pragma option -zR_OSD_CMDBUF -zTOSD_CMDBUF

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "System\all.h"

//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************
//
// global multipurpose buffer for OSD, the table loader, etc.
//
BYTE TempBuffer[TEMP_BUFFER_SIZE] = {0};


//*********************************  END  **************************************

