/*
	$Workfile:   gm_HostReg.h  $
	$Revision: 1.13 $
	$Date: 2012/06/06 08:42:39 $
*/
#ifndef __HOSTREG_H__
#define __HOSTREG_H__
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
// MODULE:      hostreg.h
//
// USAGE:       This module contains functions to access host registers value.
//
// NOTE:		gm_SetRegBits and gm_ClearRegBits are written in the way
//				to avoid host registers access problem in a case when
//				the second parameter is a BYTE constant. Paradigm will use BYTE
//				access instead of WORD access in this case. If it's necessary
//				to modify these macros, please, be sure that Paradigm will not
//				produce BYTE access code.
//******************************************************************************

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************
#define DEC_REGBASE			0xEE00				// Host register DEC base addr

//******************************************************************************
//  G L O B A L    F U N C T I O N S    P R O T O T Y P E
//******************************************************************************
BYTE 	  gm_ReadRegByteSafe		(WORD W_Addr);
WORD 	  gm_ReadRegWordSafe		(WORD W_Addr);
void 	  gm_WriteRegTriBytes 		(WORD W_RegAddr, DWORD D_Value);
DWORD 	  gm_ReadRegTriBytes 		(WORD W_RegAddr);

//******************************************************************************
//  M A C R O   D E F I N I T I O N
//******************************************************************************

// Byte access - Read and Write functions.
#define gm_WriteRegByte(W_RegAddr,B_RegValue) (*(volatile BYTE *)(W_RegAddr) = (BYTE)(B_RegValue))
#define gm_ReadRegByte(W_RegAddr) (*(volatile BYTE *)(W_RegAddr))

// Byte access - Clears and then sets specified bits
#define gm_ClearAndSetBitsByte(W_RegAddr, B_Clear, B_Set) (*(volatile BYTE *)(W_RegAddr) = \
    ((*(volatile BYTE *)(W_RegAddr)) & ~((BYTE)(B_Clear))) | (BYTE)(B_Set))

// Byte access - Clears specified bits
#define gm_ClearRegBitsByte(W_RegAddr, B_Clear) (*(volatile BYTE *)(W_RegAddr) &= ~((BYTE)(B_Clear)))

// Byte access - Sets specified bits
#define gm_SetRegBitsByte(W_RegAddr, B_Set) (*(volatile BYTE *)(W_RegAddr) |= (BYTE)(B_Set))

#if 1
// Word access - Read and Write functions
#define gm_WriteRegWord(W_RegAddr, W_RegValue) (*(volatile WORD *)(W_RegAddr) = (WORD)(W_RegValue))
#define gm_ReadRegWord(W_RegAddr) (((WORD)(*(volatile BYTE *)((WORD)(W_RegAddr)+1))<<8) | (*(volatile BYTE *)(W_RegAddr)))

// Word access - Clears and then sets specified bits
#define gm_ClearAndSetBitsWord(W_RegAddr, W_Clear, W_Set) (*(volatile WORD *)(W_RegAddr) = \
    ((gm_ReadRegWord(W_RegAddr) & ~(WORD)(W_Clear))) | (WORD)(W_Set))

// Word access - Clears specified bits
#define gm_ClearRegBitsWord(W_RegAddr, W_Clear) (*(volatile WORD *)(W_RegAddr) = (gm_ReadRegWord(W_RegAddr) & ~(WORD)(W_Clear)))

// Word access - Sets specified bits
#define gm_SetRegBitsWord(W_RegAddr, W_Set) (*(volatile WORD *)(W_RegAddr) = (gm_ReadRegWord(W_RegAddr) | (WORD)(W_Set)))

// DWord
#define gm_ReadRegDWord_BE(W_RegAddr) \
	   (((DWORD)(gm_ReadRegWord(W_RegAddr)) << 16) | gm_ReadRegWord(W_RegAddr+2))

#define gm_ReadRegDWord_LE(W_RegAddr) \
	   (((DWORD)(gm_ReadRegWord(W_RegAddr+2)) << 16) | gm_ReadRegWord(W_RegAddr))

#define gm_WriteRegDWord_LE(W_RegAddr, DW_Data) \
   (*(volatile WORD hostreg*)(W_RegAddr+2) = (WORD)((DW_Data) >> 16)); \
   (*(volatile WORD hostreg*)(W_RegAddr) = (WORD)(DW_Data))

#define gm_WriteRegDWord_BE(W_RegAddr, DW_Data) \
   (*(volatile WORD hostreg*)(W_RegAddr) = (WORD)((DW_Data) >> 16)); \
   (*(volatile WORD hostreg*)(W_RegAddr+2) = (WORD)(DW_Data))

#define gm_ClearAndSetBitsDWord(DW_RegAddr, DW_Clear, DW_Set) (*(volatile DWORD *)(DW_RegAddr) = \
    ((gm_ReadRegDWord_LE(DW_RegAddr) & ~(DWORD)(DW_Clear))) | (DWORD)(DW_Set))

#define gm_ClearRegBitsDWord(DW_RegAddr, DW_Clear) (*(volatile DWORD *)(DW_RegAddr) = (gm_ReadRegDWord_LE(DW_RegAddr) & ~(DWORD)(DW_Clear)))

#define gm_SetRegBitsDWord(DW_RegAddr, DW_Set) (*(volatile DWORD *)(DW_RegAddr) = (gm_ReadRegDWord_LE(DW_RegAddr) | (DWORD)(DW_Set)))

#define gm_ReadRegDWord(W_RegAddr) gm_ReadRegDWord_LE(W_RegAddr)
#define gm_WriteRegDWord(W_RegAddr, DW_Data) gm_WriteRegDWord_LE(W_RegAddr, DW_Data)

#else
// Word access - Read and Write functions
#define gm_WriteRegWord(W_RegAddr, W_RegValue) (*(volatile WORD *)(W_RegAddr) = (WORD)(W_RegValue))
#define gm_ReadRegWord(W_RegAddr) (*(volatile WORD *)(W_RegAddr))

// Word access - Clears and then sets specified bits
#define gm_ClearAndSetBitsWord(W_RegAddr, W_Clear, W_Set) (*(volatile WORD *)(W_RegAddr) = \
    ((*(volatile WORD*)(W_RegAddr) & ~(WORD)(W_Clear))) | (WORD)(W_Set))

// Word access - Clears specified bits
#define gm_ClearRegBitsWord(W_RegAddr, W_Clear) (*(volatile WORD *)(W_RegAddr) &= ~(WORD)(W_Clear))

// Word access - Sets specified bits
#define gm_SetRegBitsWord(W_RegAddr, W_Set) (*(volatile WORD *)(W_RegAddr) |= (WORD)(W_Set))

// DWord
#define gm_ReadRegDWord_BE(W_RegAddr) \
	   (((DWORD)(*(volatile WORD hostreg*)(W_RegAddr)) << 16) | *(volatile WORD hostreg*)(W_RegAddr+2))

#define gm_ReadRegDWord_LE(W_RegAddr) \
	   (((DWORD)(*(volatile WORD hostreg*)(W_RegAddr+2)) << 16) | *(volatile WORD hostreg*)(W_RegAddr))

#define gm_WriteRegDWord_LE(W_RegAddr, DW_Data) \
   (*(volatile WORD hostreg*)(W_RegAddr+2) = (WORD)((DW_Data) >> 16)); \
   (*(volatile WORD hostreg*)(W_RegAddr) = (WORD)(DW_Data))

#define gm_WriteRegDWord_BE(W_RegAddr, DW_Data) \
   (*(volatile WORD hostreg*)(W_RegAddr) = (WORD)((DW_Data) >> 16)); \
   (*(volatile WORD hostreg*)(W_RegAddr+2) = (WORD)(DW_Data))




#define gm_ClearAndSetBitsDWord(DW_RegAddr, DW_Clear, DW_Set) (*(volatile DWORD *)(DW_RegAddr) = \
    ((*(volatile DWORD*)(DW_RegAddr) & ~(DWORD)(DW_Clear))) | (DWORD)(DW_Set))

#define gm_ClearRegBitsDWord(DW_RegAddr, DW_Clear) (*(volatile DWORD *)(DW_RegAddr) &= ~(DWORD)(DW_Clear))

#define gm_SetRegBitsDWord(DW_RegAddr, DW_Set) (*(volatile DWORD *)(DW_RegAddr) |= (DWORD)(DW_Set))

#endif

//Macros to access addresses>0xFFFF (as soon old macros do not allow it)
// Where:
//      REG8_ADDR32 - byte(8 bit access)
//      REG16_ADDR32 - word(16bit access)
//      REG32_ADDR32 - DWORD(32bit access)
// Usage:
//      WORD w = REG16_ADDR32(0x11000); //to read 16bit from 0x11000 address
//      REG8_ADDR32(0x1FFFFF) = 0x23; //to write value 0x23 to 0x1fffff address

#ifdef __ADDR24__
#define REG8_ADDR32(addr) (*(volatile BYTE far *const)\
        ((void _seg *)(((WORD)((addr) >> 16)) << 8) + (void __near *)((WORD)(addr))))
#define REG16_ADDR32(addr) (*(volatile WORD far *const)\
        ((void _seg *)(((WORD)((addr) >> 16)) << 8) + (void __near *)((WORD)(addr))))
#define REG32_ADDR32(addr) (*(volatile DWORD far *const)\
        ((void _seg *)(((WORD)((addr) >> 16)) << 8) + (void __near *)((WORD)(addr))))
#else
#define REG8_ADDR32(addr) (*(volatile BYTE far *const)\
        ((void _seg *)(((WORD)((addr) >> 16)) << 12) + (void __near *)((WORD)(addr))))
#define REG16_ADDR32(addr) (*(volatile WORD far *const)\
        ((void _seg *)(((WORD)((addr) >> 16)) << 12) + (void __near *)((WORD)(addr))))
#define REG32_ADDR32(addr) (*(volatile DWORD far *const)\
        ((void _seg *)(((WORD)((addr) >> 16)) << 12) + (void __near *)((WORD)(addr))))
#endif

//#define SEGBIT2SHIFT 4 // 20-bit mode
#define SEGBIT2SHIFT 8 // 24-bit mode
#define LINEAR_TO_FPTR(Linear_Address) ((void far *)\
           ((void _seg *)(((WORD)(Linear_Address >> 16)) << (16 - SEGBIT2SHIFT)) +\
           (void __near *)((WORD)Linear_Address)))
#define LINEAR_TO_FPTR_B(Linear_Address) ((BYTE far *)\
              ((void _seg *)(((WORD)(Linear_Address >> 16)) << (16 - SEGBIT2SHIFT)) +\
              (void __near *)((WORD)Linear_Address)))
#define LINEAR_TO_FPTR_W(Linear_Address) ((WORD far *)\
           ((void _seg *)(((WORD)(Linear_Address >> 16)) << (16 - SEGBIT2SHIFT)) +\
           (void __near *)((WORD)Linear_Address)))
#define LINEAR_TO_FPTR_DW(Linear_Address) ((DWORD far *)\
              ((void _seg *)(((WORD)(Linear_Address >> 16)) << (16 - SEGBIT2SHIFT)) +\
              (void __near *)((WORD)Linear_Address)))

#define gm_RegBitShiftValue(BitName, Value) ((Value) << (BitName ## _SHIFT))

#endif
//*********************************  END  **************************************

