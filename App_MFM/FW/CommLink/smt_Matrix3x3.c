/*
   $Workfile:   smt_Matrix3x3.c  $
   $Revision: 1.3 $
   $Date: 2011/12/15 03:52:11 $
*/

//******************************************************************
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
//================================================================
//
// MODULE:      smt_acm3.c
//
//******************************************************************

#include "CommLink\smt.h"
#include "CommLink\smt_Matrix3x3.h"
#include <math.h>
//#include "StdLibInc\gm_Register.h"


typedef union
{
   SWORD sw;
   WORD w;
   BYTE b[2];
}tMultCoef;

//#define _DEBUG_MSG
#ifdef _DEBUG_MSG
#define msg(x,y)		gm_Printf(x,y)
#else
#define msg(x,y)
#endif

#ifdef USE_SMT


//******************************************************************************
//
// FUNCTION     :   BYTE ColorControl_SMT_MsgHandler(MsgPacket_t *message, BYTE comm_use)
// USAGE        :   This function parse the SMT messages for Color Control feature
//
// INPUT        :   MsgPacket_t *message = SMT message
//                  BYTE comm_use  = SERIAL_USE or DDC2BI_USE
// OUTPUT       :   OK = if no error,
//
//******************************************************************************
#pragma argsused
BYTE Matrix3x3_MsgHandler(MsgPacket_t *message, BYTE comm_use)
{
   BYTE * Bp_InMsg = (BYTE *)message->msgpkt;
   WORD W_Command = (WORD)(((WORD)Bp_InMsg[1]<<8)+(WORD)Bp_InMsg[0]);
   BYTE B_Ix;

   //get pointer to output message length setting
   BYTE* len=GetOutMsgLenPtr();

   // Get pointer to output message buffer
   BYTE * Bp_msg = GetOutMsgBufPtr();

   if ((len == NULL_PTR) || (Bp_msg == NULL_PTR))
   {
      return (BYTE)PORT_MSG_HNDLR_ERROR;
   }

   Bp_msg[0] = Matrix3x3_OK;

   //parse command
   switch (W_Command)
   {
      case MATRIX_GET_VERSION:
         Bp_msg[1] = 0;
         Bp_msg[2] = 1;
         *len = 3;
         break;

      case MATRIX_READ:
         {
            WORD W_WinId = gm_ReadRegWord(MAIN_MULT_CTRL) & 0x3;

            for (B_Ix = 0; B_Ix < 24; B_Ix+=2)
            {
               tMultCoef W_Reg;
               W_Reg.w = gm_ReadRegWord(((W_WinId==1)? MAIN_MULT_COEF11_B: MAIN_MULT_COEF11_A) + B_Ix);
               Bp_msg[B_Ix+1] = W_Reg.b[0];
               Bp_msg[B_Ix+2] = W_Reg.b[1];
            }
            *len=B_Ix+1;
            break;
         }
      case MATRIX_WRITE:
         {
            WORD W_WinId = gm_ReadRegWord(MAIN_MULT_CTRL) & 0x3;
            for (B_Ix = 0; B_Ix < 24; B_Ix+=2)
            {
               tMultCoef MultCoef;
               MultCoef.b[0] = Bp_InMsg[B_Ix+2];
               MultCoef.b[1] = Bp_InMsg[B_Ix+3];
               gm_WriteRegWord(((W_WinId==1)? MAIN_MULT_COEF11_B: MAIN_MULT_COEF11_A) + B_Ix, MultCoef.w);
            }
            *len=1;
            break;
         }
      case MATRIX_SETWINDOW:
         gm_ClearAndSetBitsWord(MAIN_MULT_CTRL, MAIN_MULT_WIN_CTRL, ((WORD)Bp_InMsg[2]) & MAIN_MULT_WIN_CTRL);
         *len=1;
         break;

      default:

         Bp_msg[0] = Matrix3x3_INVALIDPARAM;
         *len = 1;
         return (BYTE)PORT_MSG_HNDLR_OK;
   };

   //Get adj. data
   return (BYTE)PORT_MSG_HNDLR_OK;

}

#endif


