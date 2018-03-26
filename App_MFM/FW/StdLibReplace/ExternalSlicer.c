/*
	$Workfile:   ExternalSlicer.c  $
	$Revision: 1.4 $
	$Date: 2011/12/13 10:32:44 $
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
// MODULE:      ExternalSlicer.c
//
// USAGE:
//
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "System\all.h"

#ifdef EXTERNAL_SLICER

#include "StdLibInc\gm_HostReg.h"
#include "StdLibInc\gm_Register.h"

//******************************************************************************
//  E X T E R N A L
//******************************************************************************

//extern void far gm_PipInputIsrProcess (WORD W_PipIppStatus);


//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************
//#define _DEBUG_MSG
#ifdef _DEBUG_MSG
#define msg(x,y)		gm_Print(x,y)
#else
#define msg(x,y)
#endif

//******************************************************************************
//  L O C A L S
//******************************************************************************

WORD W_NumOfPackets;	// 0


//******************************************************************************
//  L O C A L    F U N C T I O N S
//******************************************************************************

//******************************************************************************
//
// FUNCTION       : void far VBI_FlagLine_PIP_ISR (WORD dummy)
//
// USAGE          : The function services the IMP_FLAGLINE interrupt to monitor new
//                  data in the VBI circular buffer.
//
// INPUT          : dummy - not used
//
// OUTPUT         : None
//
// GLOBALS        : VBI_Data_Ready 			(R) update to indicate if data in
//                                            circular buffer
//                  VBI_BufWr_Addr			(R/W)
//
// USED_REGS      : VBI_BUF_WR_ADDR_hi		(R)
//                  VBI_BUF_WR_ADDR_lo		(R)
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
#if (FEATURE_INTERRUPT_SERVICE == REV2)
void far VBI_FlagLine_PIP_ISR (void)
#endif
{
   W_NumOfPackets += NBR_OF_TTX_LINE;
}

//******************************************************************************
//
// FUNCTION       : void EnableExternalSlicer(void)
//
// USAGE          : Setup external decoder to slice VBI data through the PIP which is configured
//                  to receive the data directly into the VBI circular buffer
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : VBI_BufWrAddr	(W)
//                  VBI_BufRdAddr	(W)
//
// USED_REGS      : VBI_CTRL					(W)
//                  CLOCK_CONFIG1				(W)
//                  CLOCK_CONFIG2				(W)
//                  TIMING_CONFIG				(W)
//                  DIP_PIP_CTRL				(W)
//                  IPP_SOURCE					(W)
//                  IPP_CONTROL					(W)
//                  IPP_H_ACT_START				(W)
//                  IPP_VLOCK_EVENT				(W)
//                  IPP_HS_DELAY				(W)
//                  IPP_VS_DELAY				(W)
//                  IPP_V_ACT_START_ODD			(W)
//                  IPP_V_ACT_START_EVEN		(W)
//                  IPP_V_ACT_LENGTH			(W)
//                  IPP_H_ACT_START				(W)
//                  IPP_H_ACT_WIDTH				(W)
//                  IPP_FLAGLINE				(W)
//                  PFRC_CTRL					(W)
//                  PFRC_R0_RD_WIDTH			(W)
//                  PFRC_R0_PIXELS_OUT			(W)
//                  PFRC_R0_LINES_OUT			(W)
//                  PFRC_R0_BUF_WIDTH			(W)
//                  PFRC_WR_WIDTH				(W)
//                  PFRC_WBUF_START_hi			(W)
//                  PFRC_WBUF_START_lo			(W)
//                  PFRC_WBUF_END_hi			(W)
//                  PFRC_WBUF_END_lo			(W)
//                  PIP_SCALING					(W)
//                  AUTO_BKGND_CONTROL			(W)
//                  MOTION_PROC_CTRL			(W)
//                  HOST_CONTROL				(W)
//                  IPP_FRAME_RESET_LINE		(W)
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************

void EnableExternalSlicer(void)
{
   //
   // Initialize TTX decoder and database
   //
   TT_Main_Process_Create(NULL_PTR);	   // initialize the TTX decoder
   gm_VBI_ServiceEnable();
   W_NumOfPackets = 0;
   // Set up memory pointers for external VBI
   gm_ResetVbiBufferOverflowStatus();

   //
   // Disable Internal Slicer's interrupt processing
   //
   gm_WriteRegByte(VBI_CTRL, gm_ReadRegByte(VBI_CTRL) & ~VBI_INT_PROC_EN);

#ifdef VPX322xF_DEV_ID
   VpxInit();
   // Set up cortez registers
   gm_WriteRegWord(CLOCK_CONFIG1,0x068A);
   gm_WriteRegWord(CLOCK_CONFIG2, 0x114A);
   gm_WriteRegWord(TIMING_CONFIG,0x0001);
   gm_WriteRegWord(DIP_PIP_CTRL, 0x108);
   gm_WriteRegWord(IPP_SOURCE,0x0000);
#if (FEATURE_IMP_LOCKUP_ISSUE_FIX == REV2)
   gm_WriteRegWord(IPP_CONTROL,0x0405 | IPP_HSVS_DEL_LIMIT_EN);
#else
   gm_WriteRegWord(IPP_CONTROL,0x0405);
#endif
   gm_WriteRegWord(IPP_H_ACT_START,0x0007);
   gm_WriteRegWord(IPP_VLOCK_EVENT,0x0004);
   gm_WriteRegWord(IPP_HS_DELAY, 0);
   gm_WriteRegWord(IPP_VS_DELAY, 0);
   gm_WriteRegWord(IPP_V_ACT_START_ODD, 0x02);
   gm_WriteRegWord(IPP_V_ACT_START_EVEN, 0x02);
   gm_WriteRegWord(IPP_V_ACT_LENGTH, 0x0012);   // 18 lines
   gm_WriteRegWord(IPP_H_ACT_START, 0x02B2);    // 64 bytes wide
   gm_WriteRegWord(IPP_H_ACT_WIDTH, 0x0040);    // 64 * 2 bursts = 128 bytes wide
   gm_WriteRegWord(IPP_FLAGLINE, 0x2B);
   gm_WriteRegWord(IPP_FRAME_RESET_LINE,0x0001);
   gm_WriteRegWord(PFRC_CTRL,0x0000);
   gm_WriteRegWord(PFRC_R0_RD_WIDTH,0x0002);
   gm_WriteRegWord(PFRC_R0_PIXELS_OUT,0x0040);
   gm_WriteRegWord(PFRC_R0_LINES_OUT,0x0012);
   gm_WriteRegWord(PFRC_R0_BUF_WIDTH,0x0002);
   //
   // Use PIP buffered data mapped to VBI Circular buffer
   //
   gm_WriteRegWord(PFRC_WR_WIDTH,0x0002);
   gm_WriteRegDWord(PFRC_WBUF_START_hi,VBI_BufWrAddr); // Set to same start address as VBI buffer
   gm_WriteRegDWord(PFRC_WBUF_END_hi,(VBI_BufWrAddr+gmc_MemoryVbiSize));
   gm_WriteRegWord(PIP_SCALING,0x0000);
   gm_WriteRegWord(PB_PIP_VHEIGHT,0x0040);
   gm_WriteRegWord(PB_PIP_HSTART,0x030C);
   gm_WriteRegWord(AUTO_BKGND_CONTROL,0x0000);
   gm_WriteRegWord(MOTION_PROC_CTRL,0x0000);
   gm_WriteRegWord(HOST_CONTROL,0x0070);
   gm_Delay1ms (50);// Need to wait at least 2 fields for hardware registers to update
   gm_WriteRegWord(IPP_FRAME_RESET_LINE,0xFFFF);    // Use invalid Frame Reset so that 1 buffer acts like a circular buffer
   gm_ClearRegBitsWord(IPP_CONTROL, IPP_RUN_EN);
   //
   // Install Flagline interrupt function
   //
#if (FEATURE_INTERRUPT_SERVICE == REV2)
   gm_InterruptSetFunc(IRQ4, VBI_FlagLine_PIP_ISR);
//    	INTERRUPT_ENABLE(PIP_INPUT_IRQ4MASK, IPP_LINEFLAG_MASK);
#endif
   gm_SetFlagLineProcess(CH_B, ENABLE);
   gm_SetRegBitsWord(IPP_CONTROL, IPP_RUN_EN);
#endif
   return;
}

//******************************************************************************
//
// FUNCTION       : void DisableExternalSlicer(void)
//
// USAGE          : Disables the external slicer setup
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : none
//
// USED_REGS      : none
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DisableExternalSlicer(void)
{
   //
   // Disable the previous interrupt function and destroy the Telext Database
   //
   gm_SetFlagLineProcess(CH_B, DISABLE);
   //#if (FEATURE_INTERRUPT_SERVICE == REV2)
   //    INTERRUPT_DISABLE(PIP_INPUT_IRQ4MASK, IPP_LINEFLAG_MASK);
   //#endif
   TT_Main_Process_Destroy();
   if (GetPortProcType(UserPrefCurrentInputMain) == EXTERNAL)
   {
      SelectInput(gmvb_MainChannel, UserPrefCurrentInputMain, NORMAL_MODE);
   }
   if (GetPortProcType(UserPrefCurrentInputPip) == EXTERNAL)
   {
      SelectInput(gmvb_PipChannel, UserPrefCurrentInputPip, NORMAL_MODE);
   }
   return;
}
#endif	// #ifdef EXTERNAL_SLICER
