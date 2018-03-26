/*
	$Workfile:   gm_Memory.h  $
	$Revision: 1.6 $
	$Date: 2011/12/14 03:44:47 $
*/

#ifndef __GM_MEMORY_H__
#define __GM_MEMORY_H__
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
// MODULE:		gm_Memory.h
//
// USAGE:		This file contains declaration of the library pricate services
//				provided by the memory driver.
//
//******************************************************************************

//******************************************************************************
//  D E B U G
//******************************************************************************

//#define DEBUG_MEMORY_ERROR		// Print memory error messages


//******************************************************************************
//  E X T E R N A L   V A R I A B L E S
//******************************************************************************

extern ROM WORD gmc_MemoryBurstWidth;


//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************

// SDRAM burst size (in bits)
#define MEMORY_BURST					(gmc_MemoryBurstWidth)

//even if memory burst is less than 512, address and width should be
//aligned to 512 bit due to hardware design restriction
#define MEMORY_ALIGN 512

// Motion vector memory definitions
#define MEMORY_MOTION_BPP				((BYTE)(2))

// 3D decoder memory definitions
#define MEMORY_3D_DECODER_WIDTH			(36)

// VBI memory definitions
#define MEMORY_VBI_WIDTH				(2)

// AVS memory definitions
#define AVS_BITS_PER_DATAPOINT			((WORD)64)
#define AVS_LINE_LENGTH					((WORD)16)

// TNR memory definitions
#define TNR_BPP							(2)

//******************************************************************************
//  F U N C T I O N   P R O T O T Y P E S
//******************************************************************************

DWORD gmp_MemoryAllocateOsd(DWORD D_StartAddr);
//DWORD gmp_MemoryAllocateTeletext(DWORD D_StartAddr);
//DWORD gmp_MemoryAllocateDecoder(DWORD D_StartAddr);
//DWORD gmp_MemoryAllocateDecoder2(DWORD D_StartAddr);
#if (FEATURE_OVERDRIVE == ENABLE)
DWORD gmp_MemoryAllocateLcdOverdrive(DWORD D_StartAddr, WORD W_Width, WORD W_Height, BYTE B_Bpp);
#endif
//DWORD gmp_MemoryAllocateVbi(DWORD D_StartAddr);
DWORD gmp_MemoryAllocateAvs(DWORD D_StartAddr);
DWORD gmp_MemoryAllocateMotion(DWORD D_StartAddr, WORD W_Width, WORD W_Height, BYTE B_NumBuffers);
//DWORD gmp_MemoryAllocateCCS(DWORD D_StartAddr, WORD W_Width, WORD W_Height, BYTE B_Bpp);
DWORD gmp_MemoryAllocateTNR(DWORD D_StartAddr, WORD W_Width, WORD W_Height);
DWORD gmp_MemoryAllocateMain(DWORD D_StartAddr, WORD W_Width, WORD W_Height, BYTE B_Bpp, BYTE B_NumBuffers);
DWORD gmp_MemoryAllocatePip(DWORD D_StartAddr, WORD W_Width, WORD W_Height, BYTE B_Bpp, BYTE B_NumBuffers);

DWORD gmp_MemoryComputeSize(WORD W_Width, WORD W_Height, BYTE B_Bpp, BYTE B_BuffNum, gmt_BUFF_TYPE B_BuffType);

gmt_RET_STAT gmp_MemorySizeAdjustMotion(WORD W_Width);
//gmt_RET_STAT gmp_MemorySizeAdjustCCS(WORD W_Width, WORD W_Height, BYTE B_Bpp);
gmt_RET_STAT gmp_MemorySizeAdjustTNR(WORD W_Width, WORD W_Height);
gmt_RET_STAT gmp_MemorySizeAdjustMain(WORD W_Width, WORD W_Height, BYTE B_Bpp);
gmt_RET_STAT gmp_MemorySizeAdjustPip(WORD W_Width, WORD W_Height, BYTE B_Bpp);


//*********************************  END  **************************************
#endif	// __GM_MEMORY_H__

