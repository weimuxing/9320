#ifndef __MEMORY_H__
#define __MEMORY_H__

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
// MODULE:		Memory.h
//
//******************************************************************************

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************

//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

void MemoryInit(void);
void MemoryAllocate(gmt_PHY_CH B_Channel);
void MemoryAddCustomBuffer(gmt_CUSTOM_BUFF_TYPE B_CustomBuffer);
void MemoryRemoveCustomBuffer(gmt_CUSTOM_BUFF_TYPE B_CustomBuffer);
SDWORD MemoryCalculateRemainingSDRAM(void);
void gm_MemoryAdjust_VFlip(gmt_PHY_CH B_Channel,DWORD D_StartAddr, WORD W_Width, WORD W_Height);

//*********************************  END  **************************************
#endif	// __MEMORY_H__

