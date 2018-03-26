/*
	$Workfile:   gm_MultiPip.h  $
	$Revision: 1.2 $
	$Date: 2011/12/14 09:20:57 $
*/

#ifndef __GM_MULTIPIP_H__
#define __GM_MULTIPIP_H__
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
// MODULE:      gm_MultiPip.h
//
// USAGE:       Header file for system library utility layer multi pip
//              definitions
//
//******************************************************************************

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************

#define MAX_MULTIPIP_ROWS 		4	// Maximum number of multi pip rows
#define MAX_MULTIPIP_COLUMNS	4   // Maximum number of multi pip columns

//
// Structure defining parameters for a memory region
//
typedef struct
{
	BYTE Height;
	BYTE Width;
  	WORD PipMask;
} RegionStruct;

//******************************************************************************
//	S T A T I C    V A R I A B L E S
//******************************************************************************

extern  RegionStruct Sa_RegionArray[3];  // Array of RegionStruct, one element
										 // for each of the 3 regions

//extern	BYTE B_EnabledPipArray[16];      // Array containing the mapping of
										 // logical to physical pip numbers
                                         // in a multi pip configuration

extern  DWORD gd_MemoryPipStart;         // Start of pip area in memory

#endif

//*********************************  END  **************************************

