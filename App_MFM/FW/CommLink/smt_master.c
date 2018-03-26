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
// MODULE:      smt_master.c
//
//******************************************************************

#include <mem>
#include <stdlib>
#include "CommLink\smt.h"
#include "CommLink\msg_fwd.h"
#include "CommLink\smt_MsgFwd.h"
#include "CommLink\smt_master.h"
#include "CommLink\d_uart.h"
#include "System\all.h"

#ifdef USE_SMT
/* predefined SMT response messages */
ROM BYTE CloseTransactionMsg[] =
{
   0x06, 						//length
   CMD_SMTProtocol,				//SMT msg
   CM_TM_BIT, 					//control msg
   CLOSE_TRANSACTION_CMD, 	//close transaction
   0x00						//transaction id
};

ROM BYTE BusyProcessingMsg[] =
{
   0x08,						//length
   CMD_SMTProtocol,				//SMT msg
   CM_TM_BIT,					//control msg
   BUSY_PROCESSING,			//busy processing
   0x00,						//transaction id
   0xFA,						//timeout 250ms
   0x00						//timeout
};

/*****************************************************************************/

SMT_TransactionStruct_t SMT_RAM SMT_MasterTransactions[MAX_SMT_MASTER_TRANSACTIONS];

static SMT_TransactionStruct_t SMT_RAM * SMT_GetAvailTransaction(void)
{
   int i = 0;
   for (i = 0; i < MAX_SMT_MASTER_TRANSACTIONS; i++)
   {
      if (SMT_MasterTransactions[i].ID == SMT_TRANSID_NOT_ASSIGNED)
         return &SMT_MasterTransactions[i];
   }
   return NULL_PTR;
}

static SMT_TransactionStruct_t SMT_RAM * SMT_ID2Transaction(BYTE ID)
{
   int i = 0;
   for (i = 0; i < MAX_SMT_MASTER_TRANSACTIONS; i++)
   {
      if (SMT_MasterTransactions[i].ID == ID && SMT_MasterTransactions[i].B_Status != TRANSACTION_CLOSED)
      {
         return &SMT_MasterTransactions[i];
      }
   }
   return (SMT_TransactionStruct_t SMT_RAM *)NULL_PTR;
}

/* given a non SMT response, format it with the proper SMT response header*/
void SMT_FormatReply(BYTE * Bp_Response, SMT_TransactionStruct_t SMT_RAM *  pSlaveTransaction)
{
   BYTE TempBuffer[SIO_RxBufLen];
   MsgPacket_t * Bp_SMTReply = (MsgPacket_t *)TempBuffer;
   Bp_SMTReply->len = (Bp_Response[0] - 1) + sizeof(MsgPacket_t); /*don't resend length byte in Bp_Response*/
   Bp_SMTReply->cmd = CMD_SMTProtocol;
   Bp_SMTReply->msgType = pSlaveTransaction->EvenOdd | SMT_SLAVE_RESPONSE_BIT;

   Bp_SMTReply->control = pSlaveTransaction->ID;
   /* don't resend length byte in Bp_Response, so start at Bp_Response+1*/
   memcpy(Bp_SMTReply->msgpkt, Bp_Response+1, *Bp_Response);

   memcpy(Bp_Response, Bp_SMTReply, Bp_SMTReply->len);
}



/* determines if this transaction has more data required to be sent */
static BOOL SMT_SendDataQueued(SMT_TransactionStruct_t SMT_RAM * pTransaction)
{
   return (pTransaction->BuffSize?TRUE:FALSE);
}


/* send a data chunk that is queued*/
static gmt_RET_PASSFAIL SMT_SendPendingData(SMT_TransactionStruct_t SMT_RAM * pTransaction)
{
   if (!pTransaction || pTransaction->BuffSize)
   {
      MsgPacket_t *SMT_MsgPkt = (MsgPacket_t *)pTransaction->Ba_Buff;
      BYTE ChunkSize = pTransaction->BuffSize;
      BOOL MoreDataPending = pTransaction->MoreDataPending;

      if (ComChannel[pTransaction->HWPort].HandlerType == SERIAL_USE)
      {
         if (pTransaction->BuffSize > SERIAL_CHUNK_SIZE)
         {
            ChunkSize = SERIAL_CHUNK_SIZE;
            MoreDataPending = TRUE;
         }
      }
      else
      {
         if (pTransaction->BuffSize > DDC2BI_CHUNK_SIZE)
         {
            ChunkSize = DDC2BI_CHUNK_SIZE;
            MoreDataPending = TRUE;
         }
      }

      /* add length field to first byte. consists of 4byte SMT header size + buffersize +  1 crcbyte*/
      SMT_MsgPkt->len = 4 + ChunkSize + 1;
      SMT_MsgPkt->cmd = CMD_SMTProtocol;
      SMT_MsgPkt->msgType = 0;

      if (MoreDataPending)
      {
         SMT_MsgPkt->msgType |= MORE_DATA_BIT;		/* set the more data pending flag */
      }
      else
      {
         SMT_MsgPkt->msgType &= ~MORE_DATA_BIT;	/* clear the more data pending flag */
      }

      /*copy buffer to memory*/
      _fmemcpy((pTransaction->Ba_Buff + 4/* 4 offsetof(MsgPacket_t, msgpkt)*/), (void*)pTransaction->Bp_Buff, ChunkSize);
      SMT_Send(pTransaction, pTransaction->Ba_Buff);
      pTransaction->Bp_Buff += ChunkSize;
      pTransaction->BuffSize -= ChunkSize;

      return gmd_PASS;
   }

   return gmd_FAIL;
}


static gmt_RET_PASSFAIL SMT_AttachSendBuffer(SMT_TransactionStruct_t SMT_RAM * pTransaction, BYTE * pBuffer, WORD Size)
{

   if (!pTransaction || !pBuffer)
      return gmd_FAIL;
   else
   {
      pTransaction->Bp_Buff = pBuffer;

// attach buffer using a Buffer pointer, not attach to Ba_Buff... needs a free function for malloc
//		pTransaction->Bp_Buff = pTransaction->Ba_Buff;

      pTransaction->BuffSize = Size;

      /*	store original SMT header into Transaction context's buffer.  This data
      	will be used be each subsequent data chunk, with modified length field
      */
      _fmemcpy(pTransaction->Ba_Buff, (void*)pBuffer, Size); // SMT_MAX_BUF_SIZE/*offsetof(MsgPacket_t, msgpkt)*/);

   }


   return gmd_PASS;
}


static void SMT_BufferReceiveData(MsgPacket_t * pBuffer, SMT_TransactionStruct_t SMT_RAM * pTransaction)
{
   /* TODO: what is appropriate thing to do here. buffer a string of smt replies, or always over write the previous
   	reply with the latest, then send only the latest reply back to master*/

   _fmemcpy(pTransaction->Bp_RxBuff, (void *)pBuffer, pBuffer->len);
   pTransaction->Bp_RxBuff += pBuffer->len;
}


SMT_TransactionStruct_t SMT_RAM * SendSMTMsg(BYTE SMTPort, BYTE HWPort, BYTE *pBuffer, WORD Size, BOOL MoreDataPending)
{
   int i = 0;
   SMT_TransactionStruct_t SMT_RAM *pTransaction = NULL_PTR;

   /* search smt master table for existing transaction entry.  if one exists, use this one*/
   for (i = 0; i < MAX_SMT_MASTER_TRANSACTIONS; i++)
   {
      if ((SMT_MasterTransactions[i].Port == SMTPort) && (SMT_MasterTransactions[i].HWPort == HWPort))
      {
         pTransaction = &SMT_MasterTransactions[i];
         break;
      }
   }

   if (pTransaction == NULL_PTR)
   {
      /* no existing transaction found, so open a new one with this SMTPort and HWPort*/
      pTransaction = OpenTransaction(SMTPort, HWPort);

      /* set the more data pending flag for this transaction stream*/
      pTransaction->MoreDataPending = MoreDataPending;

      /* Attach a send buffer to be sent after the transaction has been established*/
      SMT_AttachSendBuffer(pTransaction, pBuffer, Size);
   }
   else
   {
      /* set the more data pending flag for this transaction stream*/
      pTransaction->MoreDataPending = MoreDataPending;

      /* Attach a send buffer, and send immediately since connection established*/
      SMT_AttachSendBuffer(pTransaction, pBuffer, Size);
      SMT_SendPendingData(pTransaction);
   }

   return pTransaction;
}

SMT_TransactionStruct_t  SMT_RAM * OpenTransaction(BYTE SMTPort, BYTE HWPort)
{
   BYTE OpenTransactionMsg[] =
   {
      0x07,						//length
      CMD_SMTProtocol, 			//SMT msg
      CM_TM_BIT, 					//control msg
      OPEN_TRANSACTION_CMD, 		//open transaction
      0x00, 						//port
      0x00						//port
   };

   SMT_TransactionStruct_t SMT_RAM *pTransaction = SMT_GetAvailTransaction();

   if (pTransaction)
   {
      pTransaction->Port = SMTPort;
      pTransaction->HWPort = HWPort;
      pTransaction->B_Status = OPEN_TRANSACTION_CMD;
      pTransaction->Bp_Buff = pTransaction->Ba_Buff;
      ((MsgPacket_t*)(OpenTransactionMsg))->msgpkt[0] = (BYTE)(SMTPort >> 8);	//check the endian of this
      ((MsgPacket_t*)(OpenTransactionMsg))->msgpkt[1] = (BYTE)SMTPort;

      SMT_Send (pTransaction, (BYTE *)OpenTransactionMsg);
      SetResponseTimer(DDC2BI_RESPONSE_PING_TIME);
      gm_TimerStart(pTransaction->B_TmrNum, SMT_TRANSACTION_TIMEOUT);
   }
   return pTransaction;
}


int CloseTransaction(BYTE ID)
{
   return (ClosePTransaction(SMT_ID2Transaction(ID)));
}


int ClosePTransaction(SMT_TransactionStruct_t SMT_RAM * pTransaction)
{
   BYTE closeTransCmd[sizeof(CloseTransactionMsg)];
   memcpy (closeTransCmd, (BYTE *)CloseTransactionMsg, sizeof(CloseTransactionMsg));

   if (pTransaction)
   {
      closeTransCmd[4] = pTransaction->ID;
      SMT_Send(pTransaction, closeTransCmd);
      return gmd_PASS;
   }
   return gmd_FAIL;
}


gmt_RET_PASSFAIL SMT_MasterTransactionInit()
{
   BYTE i = 0;
   for (i = 0; i < MAX_SMT_MASTER_TRANSACTIONS; i++)
   {
      /* assign the master SMT timers to the transaction numbers */
      SMT_MasterTransactions[i].B_TmrNum = SMT_MASTER_TIMEOUT_0_TMR + i;

      SMT_ResetTransaction(&SMT_MasterTransactions[i], TRUE);
   }
   return gmd_PASS;
}


void SMT_CloseExpiredMasterTransactions(void)
{
#if SMT_TIMEOUT
   BYTE B_Count = 0;

   // close expired master transactions
   for (B_Count = 0; B_Count<MAX_SMT_MASTER_TRANSACTIONS;B_Count++)
   {
      if ((SMT_MasterTransactions[B_Count].B_Status != TRANSACTION_CLOSED)
            && (gm_TimerCheck(SMT_MasterTransactions[B_Count].B_TmrNum) != TIMER_OK))
      {
         gm_Print("timed out master B_Count - %d", B_Count);

         SMT_ResetTransaction(&SMT_MasterTransactions[B_Count], FALSE);
      }
   }
#endif
}

#pragma argsused
void SMT_SendBusyResponse(SMT_TransactionStruct_t SMT_RAM * pTransaction, WORD UpdateTime)
{
#if SEND_BUSY_RESPONSE
   BYTE Buffer[SIO_RxBufLen];
   MsgPacket_t * pBuffer = (MsgPacket_t *)Buffer;

   memcpy((void*)pBuffer, (void*)BusyProcessingMsg, BusyProcessingMsg[0]);

   if (pTransaction->EvenOdd)
      pBuffer->msgType |= EVEN_ODD_BIT;
   else
      pBuffer->msgType &= ~EVEN_ODD_BIT;


   /* insert correct slave transaction ID for forwarding*/
   pBuffer->msgpkt[0] = pTransaction->ID;

   /* insert update time*/
   #if 1 // little Endian
   pBuffer->msgpkt[1]  = (BYTE)(UpdateTime >> 8);
   pBuffer->msgpkt[2] = (BYTE)(UpdateTime);
   #else // big Endian
   pBuffer->msgpkt[1]  = (BYTE)(UpdateTime);
   pBuffer->msgpkt[2] = (BYTE)(UpdateTime >> 8);
   #endif

   SMT_Reply(pTransaction, (BYTE *)pBuffer);

#endif
}



/* slave send a reply*/
gmt_RET_PASSFAIL SMT_Reply(SMT_TransactionStruct_t  SMT_RAM *  pTransaction, BYTE SMT_RAM * Bp_SendBuf)
{
   if((pTransaction->comm_use == I2C_SLAVE1_USE)||(pTransaction->comm_use == I2C_SLAVE2_USE)
      ||(pTransaction->comm_use == I2C_SLAVE3_USE)||(pTransaction->comm_use == I2C_SLAVE4_USE))
   {
      SLAVE_CH B_SlaveCh = pTransaction->comm_use - I2C_SLAVE1_USE;
      I2C_SLAVE_TYPE far* Sp_I2CSlavePtr = Sp_I2CSlavePtrGlobal[B_SlaveCh];
      _fmemcpy(&Sp_I2CSlavePtr->Buffer[Start_of_GprobeCmd_in_DDC2BiMsg], Bp_SendBuf, Bp_SendBuf[0]-1);
      gm_WriteCmdPkt((BYTE far*)&Sp_I2CSlavePtr->Buffer[Start_of_GprobeCmd_in_DDC2BiMsg], pTransaction->comm_use);
   }
   else
   {
      gm_WriteCmdPkt(Bp_SendBuf, pTransaction->comm_use);
   }

   /* reset this transactions timeout */
   gm_TimerStart(pTransaction->B_TmrNum, SMT_TRANSACTION_TIMEOUT);

   return gmd_PASS;
}

/* master send a message*/
gmt_RET_PASSFAIL SMT_Send(SMT_TransactionStruct_t SMT_RAM *  pTransaction, BYTE SMT_RAM *Bp_SendBuf)
{
   MsgPacket_t * pMsg = (MsgPacket_t *) Bp_SendBuf;

   BYTE OutPort = pTransaction->HWPort;

   /*set this as SMT message*/
   pMsg->cmd = CMD_SMTProtocol;

   /*reassign proper transaction id if this is a transport message*/
   if (!(pMsg->msgType & CM_TM_BIT))
   {
      pMsg->control = pTransaction->ID;
   }

   pTransaction->W_Count %= 2;

   if (pTransaction->EvenOdd)
      pMsg->msgType |= EVEN_ODD_BIT;
   else
      pMsg->msgType &= ~EVEN_ODD_BIT;


   switch (ComChannel[OutPort].HandlerType)
   {
      case DDC2BI_USE:
         FwdI2CMsg(Bp_SendBuf, ComChannel[OutPort].ID);
         break;
      case SERIAL_USE:
         FwdSerialMsg((BYTE far *)Bp_SendBuf, ComChannel[OutPort].ID);
         break;
      default:
         break;
   }

   /*toggle even/odd bit*/
   pTransaction->EvenOdd ^= EVEN_ODD_BIT;

   /* reset this transactions timeout */
   gm_TimerStart(pTransaction->B_TmrNum, SMT_TRANSACTION_TIMEOUT);

   /* send a busy_processing message back to slave listening node, to restart it's SMT timeout*/
   SMT_SendBusyResponse((SMT_TransactionStruct_t SMT_RAM * )pTransaction->TransactionLink, SMT_TRANSACTION_TIMEOUT);

   return gmd_PASS;
}


/*Get Master SMT's response*/
BYTE * SMT_GetResponse(BYTE HWPort)
{
   MsgPacket_t * pCommBuf;

   switch (ComChannel[HWPort].HandlerType)
   {

      case SERIAL_USE:
         if (gmvb_SerialStatus == UNKNOWN_MSG_CMD)
         {
            pCommBuf = (MsgPacket_t*)gm_GetCommBuf0();
            return ( BYTE *)pCommBuf;
         }
         break;

         /*
         		case SERIAL_USE:
         			Bp_CommBuf = gm_GetCommBuf0();
         			while (ResponseSerialMsgCompleted() == FALSE)
         			{
         				if (MsgResponseTimedOut())
         				{
         					gm_Print("timeout!!!", 0);
         					goto response_timeout;
         				}
         			}
         			memcpy(Bp_ResponseBuf, Bp_CommBuf, *Bp_CommBuf);
         			responseBuf = Bp_ResponseBuf;
         			break;
         */
      case DDC2BI_USE:
         if (MsgResponseTimedOut())
         {
            BYTE tempBuffer[SIO_RxBufLen];
            pCommBuf = (MsgPacket_t *)DDC2BI_GetResponse(I2C_SLAVE_ADDR, tempBuffer);

            /* reset the periodic DDC2BI response check timer*/
            SetResponseTimer(DDC2BI_RESPONSE_PING_TIME);

            if (!IsNullMsg(tempBuffer))
            {
               return (BYTE *)pCommBuf;
            }
         }

         break;

      default:
         break;
   }

   return NULL_PTR;
}

BYTE SMT_GetOpenResponse(BYTE HWPort)
{
   MsgPacket_t * pCommBuf;

   switch (ComChannel[HWPort].HandlerType)
   {
      case SERIAL_USE:
         if (gmvb_SerialStatus != UNKNOWN_MSG_CMD)
            return SMT_TRANSID_NOT_ASSIGNED;

         pCommBuf = (MsgPacket_t*)gm_GetCommBuf0();

         /* is this an SMT control message? */
         if ((pCommBuf->cmd == CMD_SMTProtocol) && (pCommBuf->msgType & CM_TM_BIT))
         {
            switch (pCommBuf->control)
            {
                  extern WORD W_TrxCount0;
               case TRANSACTION_OPENED:
                  gmvb_SerialStatus = MSG_PARSED;
                  gm_StartRcvMsg0();

                  /* return transaction id */
                  return pCommBuf->msgpkt[0];


               case FAILED:
                  gmvb_SerialStatus = MSG_PARSED;
                  gm_StartRcvMsg0();
                  break;

               default:
                  break;
            }
         }
         break;

      case DDC2BI_USE:
         if (MsgResponseTimedOut())
         {
            BYTE tempBuffer[SIO_RxBufLen];
            pCommBuf = (MsgPacket_t *)DDC2BI_GetResponse(I2C_SLAVE_ADDR, tempBuffer);

            /* reset the periodic DDC2BI response check timer*/
            SetResponseTimer(DDC2BI_RESPONSE_PING_TIME);

            if (IsNullMsg(tempBuffer))
            {
               return SMT_TRANSID_NOT_ASSIGNED;
            }
            /* is this an SMT control message? */
            if ((pCommBuf->cmd == CMD_SMTProtocol) && (pCommBuf->msgType & CM_TM_BIT))
            {
               if (pCommBuf->control == TRANSACTION_OPENED)
               {
                  return pCommBuf->msgpkt[0];
               }
            }
         }
         break;

      default:
         break;
   }

   return SMT_TRANSID_NOT_ASSIGNED;

}


void SMT_ProcessControlMsg(MsgPacket_t * pBuffer, SMT_TransactionStruct_t SMT_RAM * pTransaction)
{
   /* when the master transaction is opened, and the master handler receives a control msg,
   	handle it in this routine
   */
   switch (pBuffer->control)
   {
      case TRANSACTION_CLOSED:
         {
            SMT_TransactionStruct_t SMT_RAM * pSlaveTransaction = (SMT_TransactionStruct_t SMT_RAM * )pTransaction->TransactionLink;
            /* send response back to host */
            SMT_Reply(pSlaveTransaction, pSlaveTransaction->Bp_Buff);

            /* reset the host transaction */
            SMT_ResetTransaction(pTransaction, TRUE);

            /* reset the slave transaction */
            SMT_ResetTransaction(pSlaveTransaction, TRUE);
         }
         break;

      case RE_TRANSMIT_CMD:
         //resend whats already in SMT_MasterTransactions[TransactionIdx].BpBuff;
         break;

      case FAILED:
         //reset transaction
         //forward failed processing message back to listening node
         break;
      case BUSY_PROCESSING:
         {
            WORD W_Timeout = (WORD)((WORD)(pBuffer->msgpkt[2] << 8) | (WORD)(pBuffer->msgpkt[1]));

            /*reset current transaction timeout on pTransaction, wait for another 250ms */
            gm_TimerStart(pTransaction->B_TmrNum, W_Timeout);

            SMT_SendBusyResponse((SMT_TransactionStruct_t SMT_RAM * )pTransaction->TransactionLink,  W_Timeout);

         }
         break;
      case GET_STATUS_CMD:

      default:
         break;

   }


}


void SMT_MasterHandler(void)
{
   BYTE i = 0;

   SMT_CloseExpiredMasterTransactions();

   for (i = 0; i < MAX_SMT_MASTER_TRANSACTIONS; i++)
   {
      BYTE HWPort;
      HWPort = SMT_MasterTransactions[i].HWPort;

      switch (SMT_MasterTransactions[i].B_Status)
      {
         case TRANSACTION_CLOSED:
            break;

         case TRANSACTION_OPENED:
            {
               MsgPacket_t *pReplyBuffer = (MsgPacket_t *)SMT_GetResponse(HWPort);
               if (pReplyBuffer)
               {
                  BYTE Ba_TempBuffer[SIO_RxBufLen];

                  memcpy((void*)Ba_TempBuffer, (void*) pReplyBuffer, pReplyBuffer->len);

                  /*forward only non control messages */
                  if (pReplyBuffer->msgType & CM_TM_BIT)
                  {
                     /*handle the control message*/
                     SMT_ProcessControlMsg(pReplyBuffer, &SMT_MasterTransactions[i]);
                  }
                  else
                  {

                     //buffer the incoming data here
                     SMT_BufferReceiveData((MsgPacket_t*)Ba_TempBuffer, &SMT_MasterTransactions[i]);

                     if (SMT_SendPendingData(&SMT_MasterTransactions[i]) ==  gmd_FAIL)
                     {
                        /* this transaction is not pending more send data. pass the response back*/
                        SMT_ForwardResponse((MsgPacket_t *)SMT_MasterTransactions[i].Ba_RxBuff, &SMT_MasterTransactions[i]);
                     }
                  }


               }
            }
            break;

         case OPEN_TRANSACTION_CMD:
            {
               BYTE TransID = SMT_GetOpenResponse(HWPort);

               if (TransID != SMT_TRANSID_NOT_ASSIGNED)
               {

                  //get transaction id from port

                  //setup transaction data
                  SMT_MasterTransactions[i].ID = TransID;
                  SMT_MasterTransactions[i].B_Status = TRANSACTION_OPENED;

                  /* send the pending outgoing msg over the newly opened port*/
                  SMT_SendPendingData(&SMT_MasterTransactions[i]);


               }
            }
            break;

         default:
            break;
      }

   }

}



gmt_RET_PASSFAIL SMT_LinkTransactions(SMT_TransactionStruct_t SMT_RAM *pT1, SMT_TransactionStruct_t SMT_RAM *pT2)
{
   if (pT1 && pT2)
   {
      #if 1
      pT1->TransactionLink = (BYTE far *)pT2;
      pT2->TransactionLink = (BYTE far *)pT1;      
      #else
      pT1->TransactionLink = (pSMT_TransactionStruct)pT2;
      pT2->TransactionLink = (pSMT_TransactionStruct)pT1;
      #endif
      return gmd_PASS;
   }
   return gmd_FAIL;
}

/* see if this link pointer exists in any of the master transaction indexes*/
SMT_TransactionStruct_t SMT_RAM * SMT_GetMasterTransaction(SMT_TransactionStruct_t SMT_RAM * pLink)
{
   int i = 0;

   for (i = 0; i < MAX_SMT_MASTER_TRANSACTIONS; i++)
   {
      if (SMT_MasterTransactions[i].TransactionLink == (BYTE far *)pLink)
         return &SMT_MasterTransactions[i];
   }
   return NULL_PTR;
}

#endif //USE_SMT


