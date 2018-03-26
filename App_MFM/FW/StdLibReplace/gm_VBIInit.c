/*
	$Workfile:   gm_VBIInit.c  $
	$Revision: 1.3 $
	$Date: 2011/12/14 10:35:32 $
*/
#define __GM_VBIINIT_C__
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
// MODULE:      gm_VBIInit.c
//
// USAGE:       This file contains standard library function for the
//				Cortez VBI Buffer Handler Initialization
//
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include <math.h>
#include "System\all.h"

#ifdef EXTERNAL_SLICER

#include "StdLibInc\gm_Global.h"
#include "StdLibInc\gm_HostReg.h"
#include "StdLibInc\gm_Register.h"
#include "StdLibInc\gm_StdLib.h"
#include "StdLibInc\gm_Mcu186.h"
#include "StdLibInc\gm_Vbi.h"

#include "StdLibReplace\ExternalSlicer.h"


//******************************************************************************
//  E X T E R N
//******************************************************************************

extern void SetTeletextDataSdramStartAddress(DWORD D_Addr);

//******************************************************************************
//  L O C A L    V A R I A B L E S
//******************************************************************************

//******************************************************************************
//
// FUNCTION       : void gm_VbiInit (void)
//
// USAGE          : The function initiates the VBI slicer driver for registers that
//                  are common to all VBI services, registers that need a know value,
//                  and the TTX driver statement machine to idle.
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : VBI_BufWrAddr     (W)
//                  VBI_BufRdAddr     (W)
//                  VBI_Data_Ready    (W)
//                  parser            (R/W)
//                  B_VBI_DrvInit     (W)
//                  Gd_TeletextDataSdramStartAddress (RW)
//
// USED_REGS      : VBI_BUF_WR_ADDR_hi      (R)
//                  VBI_BUF_START_hi        (R)
//                  VBI_SLICER_LVL_MIN_lo   (RW)
//                  VBI_SLICER_LVL_MIN_hi   (RW)
//                  VBI_HCOUNT_RESET_lo     (RW)
//                  VBI_HCOUNT_RESET_hi     (RW)
//                  VBI_HS_ADJUST_CNT_lo    (RW)
//                  VBI_HS_ADJUST_CNT_hi    (RW)
//                  VBI_SLICER_LEVEL_lo     (RW)
//                  VBI_SLICER_LEVEL_hi     (RW)
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void gm_VbiInit (void)
{
   BYTE image;

   //
   // Disable VBI interrupt
   //
   gm_WriteRegByte(VBI_CTRL, gm_ReadRegByte(VBI_CTRL) & ~VBI_INT_PROC_EN);

   // Reset the VBI logic and register
   image = gm_ReadRegByte(TDEC_SOFTRESET2);
   gm_WriteRegByte(TDEC_SOFTRESET2, image & ~(VBILOGIC_RESET|VBI_REGISTER_RESET));
   gm_WriteRegByte(TDEC_SOFTRESET2, image);

   //
   // Perform a VBI client reset, and disable the auto flush
   //
   gm_WriteRegWord(AIP_VBI_CLIENT_RESET, AIP_VBI_FIFO_NO_FLUSH|BIT0);
   gm_WriteRegWord(AIP_VBI_CLIENT_RESET, AIP_VBI_FIFO_NO_FLUSH);

   //
   // Flush the VBI FIFO, this also initiate the VBI hardware write pointer
   // to the begining of the circular buffer
   //
   gm_WriteRegWord(AIP_VBI_CLIENT_RESET,
                   gm_ReadRegWord(AIP_VBI_CLIENT_RESET)|AIP_VBI_FIFO_FL_FORCE);

   gm_WriteRegWord(AIP_VBI_CLIENT_RESET,
                   gm_ReadRegWord(AIP_VBI_CLIENT_RESET)& ~AIP_VBI_FIFO_FL_FORCE);

   //
   // Initialize slicer registers that are common to all VBI services
   //
   // *** The following setting were provided by PDEV as a result of validation
   gm_WriteRegByte( VBI_SLICER_LVL_MIN_lo,   0x10 );
   gm_WriteRegByte( VBI_SLICER_LVL_MIN_hi,   0x01 );
   gm_WriteRegByte( VBI_HCOUNT_RESET_lo,     0xA4 );
   gm_WriteRegByte( VBI_HCOUNT_RESET_hi,     0x00 );
   gm_WriteRegByte( VBI_HS_ADJUST_CNT_lo,    0xD3 );
   gm_WriteRegByte( VBI_HS_ADJUST_CNT_hi,    0x03 );

   //
   // Initialize the global variables for TTX Database interface functions
   // Assume the memory has been allocated, and the value is adjusted to
   // burst boundary here.
   //
   Gd_TeletextDataSdramStartAddress = gm_MemoryGetInfo(BUF_TELETEXT, BUF_START_ADDRESS);
   Gd_TeletextDataSdramStartAddress <<= 6;      // 64 bytes per fs busrt
   //
   // TODO: Setup IROM variable for Rev-BC
   //		 Can be optimized later when Rev-AB is discard.
   //
   SetTeletextDataSdramStartAddress(Gd_TeletextDataSdramStartAddress);

   //
   // Initiate driver variable
   //
   gm_ResetVbiBufferOverflowStatus();

   VBI_Drv.Stat = VBI_DRV_INIT;           // indicate driver initialized
   VBI_Drv.Service = VBI_SERVICE_NONE;    // indicate need configuration
}

#endif //#ifdef EXTERNAL_SLICER

