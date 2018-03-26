/*
   $Workfile:   smt_ACC5bin.c  $
   $Revision: 1.4 $
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
// MODULE:      smt_Acc5bin.c
//
//******************************************************************
//
// USAGE: Provides remote access to ACC module through transport communication interfaces like SMT
//
//******************************************************************
// DOCUMENT:ACC5_SMT_interface.r2.doc; See SCR#2719 attachment
// Document describes high level protocol between external tool and this message handler
//******************************************************************

#include <mem.h>
#include "CommLink\smt.h"
#include "CommLink\smt_Acc5bin.h"
#include "stdlibreplace\gm_UploadSRAMBlock.h"

#if(defined( USE_SMT) && (FEATURE_ACC == ENABLE))
#if FEATURE_ACC_RAM_SIZE == 10

//=============================================================================
//                LOCAL DEFINITIONS according specs
//=============================================================================
#define FEATURE_ACC_BINS 3

#define ACC_VLUT_SIZE  1024 /* in WORDs */

#define HISTOGRAMS_OFFS_5bin ACC_VLUT_SIZE *sizeof(WORD)
#define HISTOGRAMS_SIZE_5bin 80 /* in WORDs */
#define TRANS_FUNC_OFFS_5bin HISTOGRAMS_OFFS_5bin + HISTOGRAMS_SIZE_5bin  * sizeof(WORD)
#define TRANS_FUNC_SIZE_5bin  129 * 5 /* in WORDs */

#define HISTOGRAMS_OFFS_3bin ACC_VLUT_SIZE * sizeof(WORD)
#define HISTOGRAMS_SIZE_3bin 48 /* in WORDs */
#define TRANS_FUNC_OFFS_3bin HISTOGRAMS_OFFS_3bin + HISTOGRAMS_SIZE_3bin  * sizeof(WORD)
#define TRANS_FUNC_SIZE_3bin  257 * 3 /* in WORDs */

//=============================================================================
//                C O D E
//=============================================================================

//******************************************************************************
//
// FUNCTION     :   BYTE Acc5_MsgHandler(MsgPacket_t *message, BYTE comm_use)
// USAGE        :   This function parse the SMT messages for Color Control feature
//
// INPUT        :   MsgPacket_t *message = SMT message
//                  BYTE comm_use  = SERIAL_USE or DDC2BI_USE
// OUTPUT       :   OK = if no error,
//
//******************************************************************************
#pragma argsused
BYTE Acc5_MsgHandler(MsgPacket_t *message, BYTE comm_use)
{
   BYTE * Bp_msg = (BYTE *)message->msgpkt;
   WORD W_Command;
   t_Acc5Context * pContext;
   //get pointer to output message length setting
   BYTE* Bp_len=GetOutMsgLenPtr();
   BYTE * Bp_out= GetOutMsgBufPtr();

   BYTE B_MemAllocResult = GetBuffer((BYTE far * far * )&pContext, (WORD)sizeof(t_Acc5Context));


   if (B_MemAllocResult == 0 )
   {// buffer is just allocated; initialize the context

      if (pContext == NULL_PTR)
         return PORT_MSG_HNDLR_ERROR;

      // buffer initialisation
      memset ((BYTE*)pContext,0,sizeof(t_Acc5Context));

      // get command id from input buffer
      W_Command = ((ACC5_WRITE_TYPE *)Bp_msg)->id;
   }
   else if (B_MemAllocResult == 1 )
   {// buffer is already in use. Data from previous message is stored in the buffer
      //use command ID from context if is not the first message in multy message transport transaction

      W_Command = pContext->CurrentCommand;
   }
   else // memory is not allocated(B_MemAllocResult == 0xff)
   {
      // handle memory allocation error
      return PORT_MSG_HNDLR_ERROR;
   }

   if ((Bp_len == NULL_PTR) || (Bp_msg == NULL_PTR))
   {
      return (BYTE)PORT_MSG_HNDLR_ERROR;
   }

   Bp_out[0] = Acc5_OK;
   *Bp_len = 1; //output message length

   //parse command
   switch (W_Command)
   {
      case Acc5_GET_VERSION:
         // return 1;

         Bp_out[1] = 1;
         Bp_out[2] = 0;
         *Bp_len = 3;//output message length
         break;
//===============================ACC_WRITE======================================================
      case Acc5_WRITE:
         {
            WORD W_TabOffs;
            WORD W_TabSize;
            BYTE * Bp_TransferTable= Bp_msg;
            WORD W_TransferSyze = GetMesageLength();

            if (pContext->Shift == 0)
            {//Handle the first message in the sequense
               ACC5_WRITE_TYPE * msg= (ACC5_WRITE_TYPE *) Bp_msg;

               if (W_TransferSyze < (sizeof(ACC5_WRITE_TYPE)+1))
                  return PORT_MSG_HNDLR_ERROR;

               //program control registers
               gm_WriteRegWord(ACC_LWR_END,		msg->low_end);
               gm_WriteRegWord(ACC_MID_START,		msg->mid_start);
               gm_WriteRegWord(ACC_MID_END,		msg->mid_end);
               gm_WriteRegWord(ACC_UPR_START,		msg->upper_start);
#if FEATURE_ACC_BINS == 5
               gm_ClearRegBitsWord(ACC_MISC_CTRL, ACC_5BIN_EN);
               if (msg->bins == 5)
               {
                  gm_SetRegBitsWord(ACC_MISC_CTRL, ACC_5BIN_EN);
                  gm_WriteRegWord(ACC_BIN2_RLIMIT,	msg->bin2_r);
                  gm_WriteRegWord(ACC_BIN3_LLIMIT,	msg->bin3_l);
                  gm_WriteRegWord(ACC_BIN3_RLIMIT,	msg->bin3_r);
                  gm_WriteRegWord(ACC_BIN4_LLIMIT,	msg->bin4_l);
               }
#endif
               gm_WriteRegWord(YC_LINK_CTRL,			msg->YCLink);
               gm_WriteRegWord(YCL_MAX_DELTA,			msg->YCLmaxdelta);
               gm_WriteRegWord(YCL_SCL_THRESHOLD,		msg->SCLtreshold);
               gm_WriteRegWord(YCL_SCALE,				msg->YCLscale);
               gm_WriteRegWord(ACC_Y_MIN,				msg->Ymin);
               gm_WriteRegWord(ACC_Y_MAX,				msg->Ymax);

               Bp_TransferTable = msg->data;
               W_TransferSyze -= (sizeof(ACC5_WRITE_TYPE)-1);
            }

#if FEATURE_ACC_BINS == 5
            W_TabOffs = (gm_ReadRegWord(ACC_MISC_CTRL)&ACC_5BIN_EN) ? TRANS_FUNC_OFFS_5bin : TRANS_FUNC_OFFS_3bin;
            W_TabSize = sizeof(WORD) * ((gm_ReadRegWord(ACC_MISC_CTRL)&ACC_5BIN_EN) ? TRANS_FUNC_SIZE_5bin: TRANS_FUNC_SIZE_3bin);
#else
            W_TabOffs = TRANS_FUNC_OFFS_3bin;
            W_TabSize = sizeof(WORD) * TRANS_FUNC_SIZE_3bin;
#endif

            if (pContext->Shift + W_TransferSyze > W_TabSize)
            {
               // cut off data if the table can be owerflowed
               W_TransferSyze = W_TabSize - pContext->Shift;
               Bp_out[0] = Acc5_INVALIDPARAM;
            }

            //copy Transfer Function data to temporary buffer
            //use stored byte if present
            if (pContext->StoredFlag)
               TempBuffer[0] = pContext->StoredDataByte;

            //take data form input buffer
            memcpy((BYTE*)(TempBuffer + pContext->StoredFlag), (BYTE*)Bp_TransferTable, W_TransferSyze);

            // correct length value of data block if a byte was stored in previous message
            W_TransferSyze += pContext->StoredFlag;

            if (W_TransferSyze % sizeof(WORD))
            {// handle situation when only part of last word in data block was received;

               // set a flag
               pContext->StoredFlag = 1;

               // Store the byte for next message
               pContext->StoredDataByte = Bp_TransferTable[W_TransferSyze-1];
            }
            else
               pContext->StoredFlag =0;

            //Load prepared data to ACC table
            if (W_TransferSyze != gm_LoadSRAMBlockEx(gmd_TAB_ACC_OUT_LUT,	gmd_OCM_STALLING_MODE,	gmd_HIGH_PRIORITY_MODE,
                  (WORD far *) TempBuffer,W_TabOffs + pContext->Shift, W_TransferSyze))
               return PORT_MSG_HNDLR_ERROR;

            //Store number of loeaded bytes
            pContext->Shift += W_TransferSyze;

            break;
         }
//===============================ACC_READ======================================================
      case Acc5_READ :
         {
            WORD W_TabOffs, W_TabSize;
            WORD  W_TransferSyze = GetMaxDataLength()&(-2);// -2 = 0xFFFE  - make the length of ouput message even
            ACC5_READ_TYPE * msg = (ACC5_READ_TYPE *) Bp_out;
#if FEATURE_ACC_BINS == 5
            W_TabOffs =  (gm_ReadRegWord(ACC_MISC_CTRL)&ACC_5BIN_EN) ? TRANS_FUNC_OFFS_5bin : TRANS_FUNC_OFFS_3bin;
            W_TabSize = sizeof(WORD) * ((gm_ReadRegWord(ACC_MISC_CTRL)&ACC_5BIN_EN) ? TRANS_FUNC_SIZE_5bin: TRANS_FUNC_SIZE_3bin);
#else
            W_TabOffs = TRANS_FUNC_OFFS_3bin;
            W_TabSize = sizeof(WORD) * TRANS_FUNC_SIZE_3bin;
#endif


            if (pContext->Shift == 0)
            {//Prepare the first message in the sequense
               // only the first element of table is transfered in first message

               if (W_TransferSyze < (sizeof(ACC5_READ_TYPE)+1))
                  return PORT_MSG_HNDLR_ERROR;

               msg->low_end  =		gm_ReadRegWord(ACC_LWR_END);
               msg->mid_start =	gm_ReadRegWord(ACC_MID_START);
               msg->mid_end  =		gm_ReadRegWord(ACC_MID_END);
               msg->upper_start =	gm_ReadRegWord(ACC_UPR_START	);
#if FEATURE_ACC_BINS == 5
               if (gm_ReadRegWord(ACC_MISC_CTRL)&ACC_5BIN_EN)
               {
                  msg->bin2_r = gm_ReadRegWord(ACC_BIN2_RLIMIT);
                  msg->bin3_l = gm_ReadRegWord(ACC_BIN3_LLIMIT);
                  msg->bin3_r = gm_ReadRegWord(ACC_BIN3_RLIMIT);
                  msg->bin4_l = gm_ReadRegWord(ACC_BIN4_LLIMIT);
                  msg->bins = 5;
                  msg->curves= 5;
               }
               else
#endif
               {
                  msg->bin2_r = 0;
                  msg->bin3_l = 0;
                  msg->bin3_r = 0;
                  msg->bin4_l = 0;
                  msg->bins = 3;
                  msg->curves= 3;
               }
               msg->YCLink = 		gm_ReadRegWord(YC_LINK_CTRL);
               msg->YCLmaxdelta = 	gm_ReadRegWord(YCL_MAX_DELTA);
               msg->SCLtreshold = 	gm_ReadRegWord(YCL_SCL_THRESHOLD);
               msg->YCLscale = 	gm_ReadRegWord(YCL_SCALE);
               msg->Ymin = 		gm_ReadRegWord(ACC_Y_MIN);
               msg->Ymax = 		gm_ReadRegWord(ACC_Y_MAX);
               msg->curvsize= 		W_TabSize;
               Bp_out 	+= 			sizeof(ACC5_READ_TYPE)-1;//msg->data;

               // first message response length
               * Bp_len = sizeof(ACC5_READ_TYPE)-1+sizeof(WORD);

               // during the first reading message only one element of table is transfering
               W_TransferSyze = sizeof(WORD);

            }
            else
            {
               W_TransferSyze = (W_TransferSyze < W_TabSize -pContext->Shift) ? W_TransferSyze : W_TabSize -pContext->Shift;
               * Bp_len = (BYTE)W_TransferSyze;
            }

            //Get data to ACC table
            //use TempBuffer first because output buffer Bp_out is BYTE aligned
            if (OK != gm_UploadSRAMBlock(gmd_TAB_ACC_OUT_LUT,	gmd_OCM_STALLING_MODE,	gmd_HIGH_PRIORITY_MODE,
                                         (WORD far *) TempBuffer, W_TabOffs + pContext->Shift, W_TransferSyze))
               return PORT_MSG_HNDLR_ERROR;

            memcpy((BYTE*)Bp_out, (BYTE*)TempBuffer, W_TransferSyze);

            //store number of received words to be used in the next message
            pContext->Shift += W_TransferSyze;

            // if not all expected data has been recevied ask fore more data
            MoreData((pContext->Shift <W_TabSize));

            break;
         }
         /**************************************************************************************************************/
      case Acc5_READ_BIN:
         {
            //read the histogram

#if FEATURE_ACC_BINS == 5
            WORD W_TabSize = sizeof(WORD) *((gm_ReadRegWord(ACC_MISC_CTRL)&ACC_5BIN_EN) ? HISTOGRAMS_SIZE_5bin : HISTOGRAMS_SIZE_3bin);
            WORD  W_TabOffs = (gm_ReadRegWord(ACC_MISC_CTRL)&ACC_5BIN_EN) ? HISTOGRAMS_OFFS_5bin : HISTOGRAMS_OFFS_3bin;
            Bp_out[1] = (gm_ReadRegWord(ACC_MISC_CTRL)&ACC_5BIN_EN) ? 5 : 3;
#else
            WORD W_TabSize = HISTOGRAMS_SIZE_3bin *sizeof(WORD) ;
            WORD W_TabOffs = HISTOGRAMS_OFFS_3bin;
            Bp_out[1] = 3;
#endif
            if (OK != gm_UploadSRAMBlock(gmd_TAB_ACC_OUT_LUT, gmd_OCM_STALLING_MODE,   gmd_HIGH_PRIORITY_MODE,
                                         (WORD far *)&Bp_out[2], W_TabOffs , W_TabSize))
               return PORT_MSG_HNDLR_ERROR;
            *Bp_len = 2 + W_TabSize;
            break;
         }
         /**************************************************************************************************************/
      case Acc5_UPDATE_BIN:
         {
            // write uptated value to ACC control registers
            ACC5_BIN_TYPE * bin = (ACC5_BIN_TYPE *)Bp_msg;

            gm_WriteRegWord(ACC_LWR_END, 		bin->low_end);
            gm_WriteRegWord(ACC_MID_START,		bin->mid_start);
            gm_WriteRegWord(ACC_MID_END,		bin->mid_end);
            gm_WriteRegWord(ACC_UPR_START,		bin->upper_start);
#if FEATURE_ACC_BINS == 5
            gm_ClearAndSetBitsWord(ACC_MISC_CTRL,  	ACC_5BIN_EN, (bin->bins == 5) ? ACC_5BIN_EN : 0);
            if (bin->bins == 5)
            {
               gm_Printf("5 Bins",0);
               gm_WriteRegWord(ACC_BIN2_RLIMIT,	bin->bin2_r);
               gm_WriteRegWord(ACC_BIN3_LLIMIT,	bin->bin3_l);
               gm_WriteRegWord(ACC_BIN3_RLIMIT,	bin->bin3_r);
               gm_WriteRegWord(ACC_BIN4_LLIMIT,	bin->bin4_l);
            }
#endif
            break;
         }
         /**************************************************************************************************************/
      case Acc5_ENABLE:

         gm_ClearAndSetBitsWord(ACC_CTRL, ACC_WIN_CTRL,   ACC_WIN_CTRL & Bp_msg[2]);
         break;
         /**************************************************************************************************************/
      case Acc5_DISABLE:

         gm_ClearRegBitsWord(ACC_CTRL,ACC_WIN_CTRL);
         break;

         /**************************************************************************************************************/
      case Acc5_READ_OUTPUT_CURVE:
         {
            WORD  W_TransferSyze = GetMaxDataLength()&(-2);// -2 = 0xFFFE  - make the length of ouput message even
            tACC5_READ_OUTPUT_CURVE * msg = (tACC5_READ_OUTPUT_CURVE *) Bp_out;
            WORD W_TabOffs = 0;
            WORD W_TabSize = sizeof(WORD) * ACC_VLUT_SIZE;

            if (pContext->Shift == 0)
            {//Prepare the first message in the sequense
               // only the first element of table is transfered in first message

               if (W_TransferSyze < (sizeof(ACC5_READ_TYPE)+1))
                  return PORT_MSG_HNDLR_ERROR;

               msg->size = sizeof(WORD) * ACC_VLUT_SIZE;
               Bp_out 	+= 	sizeof(tACC5_READ_OUTPUT_CURVE)-1;//msg->data;

               // first message response length
               * Bp_len = sizeof(tACC5_READ_OUTPUT_CURVE)-1+sizeof(WORD);

               // during the first reading message only one element of table is transfering
               W_TransferSyze = sizeof(WORD);

            }
            else
            {
               W_TransferSyze = (W_TransferSyze < W_TabSize -pContext->Shift) ? W_TransferSyze : W_TabSize -pContext->Shift;
               * Bp_len = (BYTE)W_TransferSyze;
            }

            //Get data from ACC table
            //use TempBuffer first because output buffer Bp_out is BYTE aligned
            if (OK != gm_UploadSRAMBlock(gmd_TAB_ACC_OUT_LUT,	gmd_OCM_STALLING_MODE,	gmd_HIGH_PRIORITY_MODE,
                                         (WORD far *) TempBuffer, W_TabOffs + pContext->Shift, W_TransferSyze))
               return PORT_MSG_HNDLR_ERROR;

            memcpy((BYTE*)Bp_out, (BYTE*)TempBuffer, W_TransferSyze);

            //store number of received words to be used in the next message
            pContext->Shift += W_TransferSyze;

            // if not all expected data has been recevied ask fore more data
            MoreData((pContext->Shift <W_TabSize));
            break;
         }
         /**************************************************************************************************************/
      default:
         Bp_msg[0] = Acc5_INVALIDPARAM;
   };

   //store current command ID in the context.
   pContext->CurrentCommand = W_Command;

   gm_WriteRegWord(HOST_CONTROL,ODP_SYNC_UPDATE);

   return (BYTE)PORT_MSG_HNDLR_OK;
}
#endif
#endif
