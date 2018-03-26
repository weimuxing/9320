/*
	$Workfile:   DipMapping.c  $
	$Revision: 1.3 $
	$Date: 2011/12/13 10:32:40 $
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
// MODULE:      DipMapping.c
//
// USAGE:       This module contains EV1_32626H_BOARD specific Dip mapping info
//
//******************************************************************************

//******************************************************************************
//
// FUNCTION       : void gm_InitDipMapping(void)
//
// USAGE          : Dumps the generated Dip mapping register block specific
//                  to the board layout
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : DIP_MAIN_CTRL  (W)
//                  DIP_MAIN_MAP0  (W)
//                  DIP_MAIN_MAP1  (W)
//                  DIP_MAIN_MAP2  (W)
//                  DIP_MAIN_MAP3  (W)
//                  DIP_MAIN_MAP4  (W)
//                  DIP_MAIN_MAP5  (W)
//                  DIP_MAIN_MAP6  (W)
//                  DIP_MAIN_MAP7  (W)
//                  DIP_MAIN_MAP8  (W)
//                  DIP_MAIN_MAP9  (W)
//                  DIP_MAIN_MAP10 (W)
//                  DIP_MAIN_MAP11 (W)
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void gm_InitDipMapping(void)
{
   gm_WriteRegWord(DIP_MAIN_CTRL,  0x0000);
   gm_WriteRegWord(DIP_MAIN_MAP0,  0x07DF);
   gm_WriteRegWord(DIP_MAIN_MAP1,  0xC204);
   gm_WriteRegWord(DIP_MAIN_MAP2,  0x1440);
   gm_WriteRegWord(DIP_MAIN_MAP3,  0xF1C6);
   gm_WriteRegWord(DIP_MAIN_MAP4,  0x507D);
   gm_WriteRegWord(DIP_MAIN_MAP5,  0x4D24);
   gm_WriteRegWord(DIP_MAIN_MAP6,  0x6554);
   gm_WriteRegWord(DIP_MAIN_MAP7,  0xDF5D);
   gm_WriteRegWord(DIP_MAIN_MAP8,  0x2487);
   gm_WriteRegWord(DIP_MAIN_MAP9,	0xC2CA);
   gm_WriteRegWord(DIP_MAIN_MAP10, 0xCE34);
   gm_WriteRegWord(DIP_MAIN_MAP11, 0x0003);
}
