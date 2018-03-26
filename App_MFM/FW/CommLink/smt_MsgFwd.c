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
//===================================================
//
// MODULE:      smt_msgfwd.c
//
//******************************************************************

#include <mem.h>
#include <stdlib>
#include "System\all.h"
#include "CommLink\smt.h"
#include "CommLink\msg_fwd.h"
#include "CommLink\smt_MsgFwd.h"
#include "CommLink\smt_master.h"
#include "CommLink\d_uart.h"


#ifdef USE_SMT

void SMT_ForwardResponse(MsgPacket_t * pBuffer, SMT_TransactionStruct_t SMT_RAM *  pTransaction)
{
   /* given the input buffer, forward the response back to the outgoing transaction
   associated with the TransID parameter.  The buffer contains a response from a
   SMT slave, that needs to be forwarded from the receiving master through to
   the slave request*/

   /* given master's transID, lookup transaction id of slave */
	SMT_TransactionStruct_t SMT_RAM * pSlaveTransaction = (SMT_TransactionStruct_t SMT_RAM *) pTransaction->TransactionLink;

   /*forward only if there is a connected slave transaction*/
   if (pSlaveTransaction)
   {
      if (pSlaveTransaction->EvenOdd)
         pBuffer->msgType |= EVEN_ODD_BIT;
      else
         pBuffer->msgType &= ~EVEN_ODD_BIT;

      if (!pBuffer->msgType & CM_TM_BIT)
      {
         /* insert correct slave transaction ID for forwarding if transport msg*/
         pBuffer->control = pSlaveTransaction->ID;
      }

      SMT_Reply(pSlaveTransaction,(BYTE far*)pBuffer);

      /* reset the incoming receive buffer's pointer for new incoming data */
      pTransaction->Bp_RxBuff = pTransaction->Ba_RxBuff;
   }
}

//
// Cortez chip specific for 8 bit (BYTE) register boundry
//
#define BYTE_RANGE_START	0xAC00		// Start of register block with Byte Access
#define BYTE_RANGE_END		0xADFF		// End of register block with Byte Access

static BYTE I2C_GetResponse(BYTE B_Address, BYTE B_Size, BYTE * Bp_Buffer)
{
   if ( gm_2WireStart(B_Address, gmd_2WIRE_RD) == gmd_PASS)
   {
      WORD i = 0;
      BYTE * currentByte = Bp_Buffer;

      for (i = 0; i < B_Size; i++, currentByte++)
      {
         gm_2WireGetByte(I2C_RD, currentByte);
      }

      gm_2WireStop();
      return B_Size;
   }
   gm_2WireStop();
   return 0;
}

gmt_RET_PASSFAIL Serial_GetResponse(BYTE maxTimeout)
{
   BYTE far * recBuffer = gm_GetCommBuf();
   BYTE len, checksum;
   gmt_RET_PASSFAIL status = gmd_FAIL;

   gm_TimerStart(WORK1_10MS_TMR, maxTimeout);					//max wait time

   /* get the response back from serial port */
   gm_StartRcvMsg();
   while (1)
   {
      len = recBuffer[0];             // packet length

      //
      //	verify the message correctness
      //
      for ( checksum=0; len > 0;  )          // Calculate packet checksum
      {
         checksum += recBuffer[--len];
      }

      if (checksum == 0)                     // Checksum correct
      {
         status = gmd_PASS;
         break;
      }

      if (gm_TimerCheck(WORK1_10MS_TMR) == TIMER_TMO)
      {
         status = gmd_FAIL;
         break;
      }
   }
   gm_StopRcvMsg();
   return status;

}

BYTE SMT_MsgFwdHandler(MsgPacket_t *message, BYTE comm_use)
{
   BYTE B_Command = *message->msgpkt;

   BYTE B_Ix;

   //get pointer to output message length setting
   BYTE* len=GetOutMsgLenPtr();

   // Get pointer to output message buffer
   BYTE * Bp_msg = GetOutMsgBufPtr();

   if ((len == NULL_PTR) || (Bp_msg == NULL_PTR))
   {
      return (BYTE)PORT_MSG_HNDLR_ERROR;
   }

   switch (B_Command)
   {
      case FWD_I2C_WRITE:
         {
            BYTE * Bp_Buf = message->msgpkt;
            WORD W_Bufferlen = message->len - sizeof (MsgPacket_t) - 2 /*length and checksum bytes*/;
            BYTE B_Address;
            gmt_RET_STAT retStatus;
            Bp_Buf++;	//dont send command

            /* account for first byte being address, so dont send it as the buffer */
            B_Address = *(Bp_Buf++);
            W_Bufferlen--;

            retStatus = gm_2WireWriteAddrBuffer (B_Address,	NULL_PTR, 0, (BYTE far *)Bp_Buf, W_Bufferlen);

            if (retStatus == OK)
            {
               *Bp_msg = CMD_ACK;
               *len = 1;
            }
            else
            {
               *Bp_msg = CMD_NAK;
               *len = 1;
            }
            return PORT_MSG_HNDLR_OK;
         }
      case FWD_I2C_READ:
         {
            WORD i = 0;
            BYTE * Bp_Buf = message->msgpkt;
            BYTE Ba_RxBuffer[SIO_RxBufLen];
            BYTE B_Size;
            BYTE B_Address;
            memset (Ba_RxBuffer, 0, SIO_RxBufLen);

            Bp_Buf++; //dont send command
            B_Address = *Bp_Buf++;
            B_Size = *Bp_Buf++;

            if (I2C_GetResponse(B_Address, B_Size, Ba_RxBuffer))
            {
               *len = B_Size;
               for (i = 0; i < *len; i++)
               {
                  *Bp_msg++ = Ba_RxBuffer[i];
               }
               return PORT_MSG_HNDLR_OK;
            }
            return PORT_MSG_HNDLR_SELFCARED;
         }
      case FWD_I2C_WRITE_READ:
         {
            WORD i = 0;
            BYTE Ba_RxBuffer[SIO_RxBufLen];
            BYTE * Bp_Buf = message->msgpkt;
            WORD W_Bufferlen = message->len - sizeof (MsgPacket_t) - 2 /*length and checksum bytes*/;
            BYTE B_Address;
            BYTE B_ReadSize;
            Bp_Buf++;	//dont send command

            /* account for first byte being address, so dont send it as the buffer */
            B_Address = *Bp_Buf++;
            B_ReadSize = *Bp_Buf++;
            W_Bufferlen-=2;

            gm_2WireWriteAddrBuffer (B_Address,	NULL_PTR, 0, (BYTE far *)Bp_Buf, W_Bufferlen);

            memset (Ba_RxBuffer, 0, SIO_RxBufLen);

            if (I2C_GetResponse(B_Address, B_ReadSize, Ba_RxBuffer))
            {
               *len = B_ReadSize;
               for (i = 0; i < *len; i++)
               {
                  *Bp_msg++ = Ba_RxBuffer[i];
               }
               return PORT_MSG_HNDLR_OK;
            }
            return PORT_MSG_HNDLR_SELFCARED;
         }
      case FWD_DDC2BI_READ:
         {
            WORD i = 0;
            BYTE * Bp_Buf = message->msgpkt;
            BYTE Ba_RxBuffer[SIO_RxBufLen];
            BOOL isNullArray = TRUE;
            memset (Ba_RxBuffer, 0, SIO_RxBufLen);

            Bp_Buf++; //dont send command

            DDC2BI_GetResponse(*Bp_Buf, Ba_RxBuffer);

            for (i = 0; i < SIO_RxBufLen; i++)
            {
               if (Ba_RxBuffer[i] != 0)
               {
                  isNullArray = FALSE;
                  break;
               }
            }

            if (Bp_Buf != NULL_PTR && isNullArray == FALSE)
            {
               *len = DDC2Bi_MSG_LENGTH(Ba_RxBuffer[1]) + 2 /*add two because of address byte and length byte*/;
               for (i = 0; i < *len; i++)
               {
                  *Bp_msg++ = Ba_RxBuffer[i];
               }
               return PORT_MSG_HNDLR_OK;
            }
            return PORT_MSG_HNDLR_SELFCARED;
         }
      case FWD_SERIAL_WRITE:
         {
            BYTE * Bp_Buf = message->msgpkt;
            BYTE B_Timeout;
            Bp_Buf++; //dont send command

            /* set the timeout */
            B_Timeout = *Bp_Buf;

            /* move the buffer pointer to the msg payload, and send */
            Bp_Buf++;
            SendSerialPkt((BYTE far *)Bp_Buf, 0);

            /* wait for response */
            if (Serial_GetResponse(B_Timeout) == gmd_PASS)
            {
               /* received something from serial port read */

               WORD i = 0;
               BYTE *BpDebugCmdPtr = gm_GetCommBuf();

               /* package the data to be sent back. First byte is length byte */
               *len = *BpDebugCmdPtr;
               for (i = 0; i < *len; i++)
               {
                  *Bp_msg++ = BpDebugCmdPtr[i];
               }
               return PORT_MSG_HNDLR_OK;
            }

            /* no data received here, so do nothing (no reply) */
            return PORT_MSG_HNDLR_SELFCARED;
         }
      case FWD_SERIAL_READ:
         {
            BYTE * Bp_Buf = message->msgpkt;
            BYTE B_Timeout;
            Bp_Buf++; //dont send command

            B_Timeout = *Bp_Buf;

            if (Serial_GetResponse(B_Timeout) == gmd_PASS)
            {
               /* received something from serial port read */

               WORD i = 0;
               BYTE *BpDebugCmdPtr = gm_GetCommBuf();

               /* package the data to be sent back. First byte is length byte */
               *len = *BpDebugCmdPtr;
               for (i = 0; i < *len; i++)
               {
                  *Bp_msg++ = BpDebugCmdPtr[i];
               }
               return PORT_MSG_HNDLR_OK;
            }

            /* no data received here, so do nothing (no reply) */
            return PORT_MSG_HNDLR_SELFCARED;
         }
      case RAM_WRITE_CMD:
         return PORT_MSG_HNDLR_OK;

      case RAM_READ_CMD:
         return PORT_MSG_HNDLR_OK;

      case CMD_gmRead:
         {

            SMT_RegRead_t *regRead = (SMT_RegRead_t *)message->msgpkt;
            WORD W_Addr = regRead->W_Addr;
            WORD W_Data;

            //
            // Check for BYTE or WORD read
            //
            if ((W_Addr >= BYTE_RANGE_START) && (W_Addr <= BYTE_RANGE_END))
               W_Data = (BYTE)gm_ReadRegByte(W_Addr);
            else
               W_Data = gm_ReadRegWord(W_Addr);


            *Bp_msg = (BYTE)(W_Data >> 8);
            Bp_msg++;
            *Bp_msg = (BYTE)(W_Data);
            *len = 2;

            return PORT_MSG_HNDLR_OK;
         }
      case CMD_gmWrite:
         {
            SMT_RegWrite_t *regWrite = (SMT_RegWrite_t *)message->msgpkt;
            WORD W_Addr = regWrite->W_Addr;
            WORD W_Data = regWrite->W_Data;

            //
            // Determine if write should be 8 bits or 16 bits based on location of address
            //
            if ((W_Addr >= BYTE_RANGE_START) && (W_Addr <= BYTE_RANGE_END))
               gm_WriteRegByte(W_Addr, (BYTE)W_Data);
            else
               gm_WriteRegWord(W_Addr, W_Data);

            *Bp_msg = CMD_ACK;
            *len = 1;

            return PORT_MSG_HNDLR_OK;
         }

      case CMD_FWDMSG:
         {
            BYTE SlaveTransID = message->control;
            SMT_MsgFwd_t *msgFwd = (SMT_MsgFwd_t *)message->msgpkt;
            MsgPacket_t * SMT_Msg = (MsgPacket_t*)	msgFwd->Payload;
            BYTE length = SMT_Msg->len;//msgFwd->Payload[0];
            BYTE FwdCmd = SMT_Msg->cmd;//msgFwd->Payload[1];

				SMT_TransactionStruct_t SMT_RAM * pSlaveTransaction = NULL_PTR;

            BYTE i = 0;
            for (i = 0; i < MAX_TRANSACTION_NUMBER; i++)
            {
               if (SMT_TransArray[i].ID == SlaveTransID)
               {
                  pSlaveTransaction = &SMT_TransArray[i];
                  break;
               }
            }

            if (FwdCmd == CMD_SMTProtocol)
            {
					SMT_TransactionStruct_t SMT_RAM * pMasterTransaction;
               BOOL MoreDataPending = SMT_Msg->msgType & MORE_DATA_BIT;

               /* Forward an SMT message through SMT protocol. If no connection, then open one now*/
               WORD SMT_Port = (WORD)((WORD)(msgFwd->SMT_Port_hi << 8) |(WORD)(msgFwd->SMT_Port_lo));


               pMasterTransaction = SendSMTMsg(SMT_Port, msgFwd->HWPort, SMT_Msg->msgpkt, length - 4/*offsetof(MsgPacket_t, msgpkt)*/ - 1/*checksum size*/, MoreDataPending);
               SMT_LinkTransactions(pMasterTransaction, pSlaveTransaction);

            }
            else if (FwdCmd == CMD_FWDMSG)
            {
               /* the forward message payload is a non SMT message, so and wait for reply*/
               BYTE i;
               FWD_MSG_Header * fmp = (FWD_MSG_Header *)msgFwd->Payload;
               BYTE WhichComChannel = fmp->WhichComChannel;
               BYTE ReadCount = fmp->ReadCount;
               BYTE Delay = fmp->Delay;
               BYTE Timeout = fmp->Timeout;

               BYTE far* FwdBuffer =  FwdMsg((BYTE far*)msgFwd->Payload);
               switch (FwdBuffer[1])
               {
                  case CMD_gmRead:
                     {
                        /* expecting a response? If so, how many packets back*/
                        for (i = 0; i < ReadCount; i++)
                        {
                           //TODO: set the timeout here.  reset the timeout if got a valid reply, but need more

                           /* wait between successive request for response */
                           BYTE ResponseBuf[SIO_RxBufLen];
                           BYTE * Bp_Response = GetReply(ResponseBuf, ComChannel[WhichComChannel].HandlerType, comm_use, Timeout, Delay);
                           if (Bp_Response)
                           {
                              SMT_FormatReply(Bp_Response, pSlaveTransaction);
                              SMT_Send(pSlaveTransaction, Bp_Response);
                           }
                        }

                     }
                     break;
                  default:
                     break;
               }
            }
            else
            {
               switch (ComChannel[msgFwd->HWPort].HandlerType)
               {
                  case SERIAL_USE:
                     FwdSerialMsg((BYTE far*)msgFwd->Payload, ComChannel[msgFwd->HWPort].ID);
                     break;

                  case DDC2BI_USE:
                     FwdI2CMsg((BYTE far*)msgFwd->Payload, ComChannel[msgFwd->HWPort].ID);
                     break;
                  default:
                     break;
               }


               {
                  //TODO: set the timeout here.  reset the timeout if got a valid reply, but need more

                  /* wait between successive request for response */
                  BYTE ResponseBuf[SIO_RxBufLen];
                  BYTE * Bp_Response = GetReply(ResponseBuf, ComChannel[msgFwd->HWPort].HandlerType, comm_use, 25, 0);

                  if (Bp_Response)
                  {
                     int i = 0;
                     for (i = 0; i < Bp_Response[0]; i++)
                     {
                        *Bp_msg = Bp_Response[i];
                        Bp_msg++;
                     }
                     *len = Bp_Response[0];
                  }
               }

               return PORT_MSG_HNDLR_OK;


            }
         }
      default:
         break;

   }

   return (BYTE)PORT_MSG_HNDLR_SELFCARED;
}

gmt_RET_PASSFAIL SMT_CloseLinkedTransaction(SMT_TransactionStruct_t SMT_RAM * pTransaction)
{
   // if there is a linked transaction, close it, return gmd_PASS
   //if no linked tranaction found, return gmd_FAIL

   if (!pTransaction->TransactionLink)
      return gmd_FAIL;

	ClosePTransaction((SMT_TransactionStruct_t SMT_RAM *)pTransaction->TransactionLink);
   return gmd_PASS;
}

#endif //#ifdef USE_SMT


