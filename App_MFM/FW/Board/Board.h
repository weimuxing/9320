/*
	$Workfile:   Board.h  $
	$Revision: 1.25 $
	$Date: 2013/02/05 08:26:23 $
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
// MODULE:      Board.h
//
// USAGE:       Header file for Board Specific configuration
//
//******************************************************************************

#ifndef __BOARD_H__
#define __BOARD_H__

#include "StdLibInc\gm_ChipFeature.h"

//******************************************************************************
// B O A R D    O P T I O N S    S E L E C T I O N
//******************************************************************************
//
// CHANNEL_CLONE:
//       This should only be defined for boards with a single AIP!
//       When a channel is set to an AIP port which is already used by the
//       other channel and the other channel is stable, there is no need for
//       re-setting up the AIP, or for the mode detection process.
//       Only the backend must be setup and the mode detection state is set
//       directly to stable.
//
// PIP_PORT_INDEPENDENCE:
//       This should only be defined for boards with a single AIP!
//       This should only be defined if CHANNEL_CLONE is also defined!
//       A port selection scheme that does not cause the PIP port to follow
//       the Main port when the PIP is hidden and the Main port is switched to
//       an input that causes a port-collision with the PIP.

//
// IC soldered/socketed selection
//
//#define SOCKETED					// Chip is socketed (undefined == soldered)


//******************************************************************************
// S D R A M     M E M O R Y    S E L E C T I O N
//******************************************************************************
#define SDRAM_MEM_BURST		512			// 32-bit interface = 512 bits/burst

#if (ENABLE_DDR_X16==ENABLE)
//#define SDRAM_MEM_SIZE		0x100000UL	//   512 Mbits = 512 bits/burst * 0x100000 bursts
#define SDRAM_MEM_SIZE		0x200000UL	//   1 Gbits = 512 bits/burst * 0x200000 bursts
#else
#define SDRAM_MEM_SIZE		0x400000UL	//   2 Gbits = 512 bits/burst * 0x400000 bursts
#endif

#define DDR2_NANYA_NT5TU64M16GGBE   0
#define DDR3_NANYA_NT5CB64M16DPCF   1
#define DDR2_SEC_K4T51163QJBCF7     2
#define DDR3_SEC_K4B1G1646EHCF8     3
#define DDR3_SEC_K4B1G1646GBCH9	    4
#define DDR3_WINBOND_W631GG6KB15    5
#define DDR3_ESMT_M15F1G1664A8CBG   6
#define DDR3_NANYA_NT5CB64M16DPDH   7

//#define DDR_DEVICE	DDR3_NANYA_NT5CB64M16DPCF // DDR2_SEC_K4T51163QJBCF7

// To Do ... May move to each board level, ... config by each board
#define DDR2	0
#define DDR3	1

#ifdef Athena_C
	#define DDRType   	DDR3
	#define DDR_DEVICE  DDR3_NANYA_NT5CB64M16DPCF
#else
#define DDRType   	DDR3
#define DDR_DEVICE  DDR3_NANYA_NT5CB64M16DPCF
//#define DDRType		DDR2				//RD board use DDR2
//#define DDR_DEVICE	DDR2_NANYA_NT5TU64M16GGBE
#endif

//******************************************************************************
// S E L F    R E S E T
//******************************************************************************
#ifdef WDT_SERVICE_ENABLE

#define WDT_RESET_BY_HARD_RESETS_REG
#ifndef WDT_RESET_BY_HARD_RESETS_REG
#error Self-Reset GPIO is not defined for this board
#endif
#endif //WDT_SERVICE_ENABLE

#endif //__BOARD_H__
//*********************************  END  **************************************

