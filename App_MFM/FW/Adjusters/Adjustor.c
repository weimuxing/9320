/*
	$Workfile:   Adjustor.c  $
	$Revision: 1.8 $
	$Date: 2012/07/09 06:40:44 $
*/
#define __ADJUSTOR_C__

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
// MODULE:      Adjustor.c
//
// USAGE:       This module contains set of utility functions called from
//				OSD adjusters
//
//******************************************************************************


//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "System\all.h"
#include "StdLibInc\gm_Register.h"

//******************************************************************************
//  D E F I N E S
//******************************************************************************

//#define _DEBUG
#ifdef _DEBUG
#define msg(string)         	gm_Print(string,0)
#define msg_i(string,value) 	gm_Print(string,value)
#else
#define msg(string)
#define msg_i(string,value)
#endif

//*****************************************************************************
//  C O D E
//*****************************************************************************

//******************************************************************************
//
// FUNCTION       : void MadiSetup(gmt_PHY_CH B_Channel)
//
// USAGE          : Call this function to configure MADI,TNR,CCS based on input port
//
// INPUT          : B_Channel        : Current input channel
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : Setup set of TNR, MADI, CCS registers in gm_AdaptiveGlobalMotion()
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void MadiSetup(gmt_PHY_CH B_Channel)
{
   if (B_Channel)
   {
   }
}

//******************************************************************************
//
// FUNCTION       : void SetupHTotal(WORD W_Htotal)
//
// USAGE          : Calls gm_SetupHTotal() to set up the input pixel clock frequency.
//
// INPUT          : W_Htotal = input H total value
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
#if FEATURE_AIP == ENABLE
void SetupHTotal(WORD W_Htotal)
{
   BYTE B_Channel;
   
   if (gm_IsInputTripleADC(gmvb_MainChannel))
      B_Channel = gmvb_MainChannel;
   else if (gm_IsInputTripleADC(gmvb_PipChannel))
      B_Channel = gmvb_PipChannel;
   else
      return;

   gm_SetupHTotal(W_Htotal, B_Channel);
   stInput[B_Channel].Htotal = W_Htotal;
}
#endif

/***********************************  END  ************************************/
