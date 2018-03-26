/*
	$Workfile:   IoExpander.c  $
	$Revision: 1.4 $
	$Date: 2011/12/14 10:35:31 $
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
// MODULE:      IoExpander.c
//
// USAGE:
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "System\all.h"
#ifdef IO_EXPANDER
//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************
static WORD W_Data[8]={0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};

//******************************************************************************
//  C O D E
//******************************************************************************

//******************************************************************************
//
// FUNCTION       : void IoExpanderSetBitWord(BYTE B_Addr, WORD W_Bit)
//
// USAGE          :
//
// INPUT          : B_Addr - Address of io expander and bits to set
//                  W_Bit
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
void IoExpanderSetBitWord(BYTE B_Addr, WORD W_Bit)
{
   WORD* pw_data = &(W_Data[(B_Addr & 0x0E)>>1]);
   *pw_data |= W_Bit;
   if (gm_Write2WireBlock(0, (B_Addr&0xFE) , (BYTE *)pw_data, 2, TRUE) != gmd_PASS)
      gm_Print ("Fail to Write - IO - SetBit Addr 0x%x",B_Addr);
}
//******************************************************************************
//
// FUNCTION       : void IoExpanderClearBitWord(BYTE B_Addr, WORD W_Bit)
//
// USAGE          :
//
// INPUT          : B_Addr - Address of io expander and bits to set
//                  W_Bit
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
void IoExpanderClearBitWord(BYTE B_Addr, WORD W_Bit)
{
   WORD* pw_data = &(W_Data[(B_Addr & 0x0E)>>1]);
   *pw_data &= ~W_Bit;
   if (gm_Write2WireBlock(0, (B_Addr&0xFE), (BYTE *)pw_data, 2, TRUE) != gmd_PASS)
      gm_Print ("Fail to Write - IO - ClearBit Addr 0x%x",B_Addr);
}

//******************************************************************************
// USAGE          :
// INPUT          : B_Addr - Address of io expander
//                  W_Mask - mask to apply
//                  W_SetBit - bit to set
// OUTPUT         : None
// GLOBALS        : None
// USED_REGS      : None
//******************************************************************************
void IoExpanderSetBitMaskWord(BYTE B_Addr, WORD W_Mask, WORD W_SetBit)
{
   WORD* pw_data = &(W_Data[(B_Addr & 0x0E)>>1]);
   *pw_data = (*pw_data &~W_Mask) | (W_SetBit & W_Mask);
   if (gm_Write2WireBlock(0, (B_Addr&0xFE), (BYTE *)pw_data, 2, TRUE) != gmd_PASS)
      gm_Print ("Fail to Write - IO - SetBit Addr 0x%x",B_Addr);
}

//******************************************************************************
//
// FUNCTION       : WORD IoExpanderGetBitWord(BYTE B_Addr, WORD W_Bit)
//
// USAGE          :
//
// INPUT          : B_Addr - Address of io expander
//                  W_Bit - mask that defines which bit(s) are read
//
// OUTPUT         : result of reading
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
WORD IoExpanderGetBitWord(BYTE B_Addr, WORD W_Bit)
{
   WORD* pw_data = &(W_Data[(B_Addr & 0x0E)>>1]);
   *pw_data |= W_Bit;
   if (gm_Write2WireBlock(0, (B_Addr&0xFE) , (BYTE *)pw_data, 2, TRUE) != gmd_PASS)
      gm_Print ("Fail to Write - IO - Addr 0x%x",0);
   if (gm_Read2WireBlock(0, (B_Addr&0xFE), (BYTE *)pw_data, 2, TRUE) != gmd_PASS)
      gm_Print ("Fail to Read - IO - Addr 0x%x",0);
   return ((*pw_data) & W_Bit);
}

#endif //IO_EXPANDER

