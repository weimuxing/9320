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
// MODULE:      smt.c
//
//******************************************************************

#include <stdlib.h>
#include <mem.h>
#include "CommLink\smt.h"
#include "CommLink\smt_msgfwd.h"
#include "CommLink\smt_master.h"
#include "CommLink\smt_Ports.h"

#ifdef USE_SMT
#define  DEBUG_SMT		  		OFF
// Support SMT
#if (DEBUG_SMT == ON)
#define	msgs(a,b)	gm_Print((const char far *)a,b)
#else
#define	msgs(a,b)
#endif


//#if SMT_DEBUG
//BYTE SMTDebug[100]; 	// TODO: WHY 100 ??
//BYTE SMTCnt=0;

//BYTE SMTCNT(void)
//{
// 	SMTCnt++;
//   if (SMTCnt > 100)	// TODO: WHY 100 ??
//   	SMTCnt = 0;
//   return SMTCnt;
//}
//#endif

SMT_TransactionStruct_t SMT_RAM SMT_TransArray[MAX_TRANSACTION_NUMBER];
//remove this extern SMT_TransactionStruct_t SMT_MasterTransactions[];
gmt_MsgStruct  OutMsg;

//static BYTE OldEvOdd = 0xFF;
static struct
{
   int OldEvOdd : 8;
   int CommType : 1;
   int NoMoreInputData : 1;
   int ActivePortNumber: 4; 
   int Reserved : 2;
   int Length:16;
}SMT_Features;
#define OldEvOdd SMT_Features.OldEvOdd
static void SMT_OpenTransaction(BYTE msgpkt[], BYTE comm_use, BYTE EvenOdd);
static void SMT_CloseTransaction(BYTE msgpkt[], BYTE comm_use, BYTE EvenOdd);

#define SMT_FWD_MESSAGE_MAX_LEN (SOCIPC_TRANS_MSG_MAX_SIZE - 1)

#ifdef ST_4K2K_93xx_BOARD
BYTE SMTMsgLen = 0;//global variable for inter-chip SMT communication
#endif

gmt_RET_PASSFAIL SMT_ResetTransaction(SMT_TransactionStruct_t SMT_RAM * pTransaction, BYTE SingleOnly)
{
   if (!pTransaction)
      return gmd_FAIL;

   gm_TimerStop(pTransaction->B_TmrNum);

   pTransaction->B_Status = TRANSACTION_CLOSED;
   pTransaction->B_CurCmd = 0;
   pTransaction->EvenOdd = 0;
   pTransaction->ID = SMT_TRANSID_NOT_ASSIGNED;
   pTransaction->Port = 0;
   pTransaction->BuffSize = 0;
   pTransaction->Bp_Buff = 0;
   pTransaction->W_Count = 0;
   pTransaction->comm_use = 0;
   pTransaction->Bp_RxBuff = pTransaction->Ba_RxBuff;
#if 1
   _fmemset(pTransaction->Ba_Buff, 0, SMT_MAX_BUF_SIZE);
   _fmemset(pTransaction->Ba_RxBuff, 0, SMT_MAX_BUF_SIZE);
#else
   memset(pTransaction->Ba_Buff, 0, SMT_MAX_BUF_SIZE);
   memset(pTransaction->Ba_RxBuff, 0, SMT_MAX_BUF_SIZE);
#endif



   /* recursively reset linked transaction*/
   if ((!SingleOnly) && (pTransaction->TransactionLink != NULL_PTR))
   {
      SMT_TransactionStruct_t SMT_RAM * pTempTrans = (SMT_TransactionStruct_t SMT_RAM * ) pTransaction->TransactionLink;
      pTransaction->TransactionLink = NULL_PTR;
      SMT_ResetTransaction(pTempTrans, FALSE);
   }
   else
   {
      pTransaction->TransactionLink = NULL_PTR;
   }
   return gmd_PASS;
}

// BYTE B_ReturnBusy = 0;
// BYTE B_ODD_EVEN_OverRide = 0;
BYTE B_ReplyEvenOdd = 0;

//******************************************************************************
// SMT functions:
//******************************************************************************
//******************************************************************************
//
// FUNCTION     :   void SMT_OpenTransaction(BYTE msgpkt[], BYTE comm_use, BYTE EvenOdd)
// USAGE        :   This function open transaction and send response to PC
//
// INPUT        :   BYTE msgpkt[] = msgpkt array from SMT message
//                  BYTE comm_use  = SERIAL_USE or DDC2BI_USE
//                  BYTE EvenOdd = Even/Odd message count
// OUTPUT       :   None
//
//******************************************************************************
void SMT_OpenTransaction(BYTE msgpkt[], BYTE comm_use, BYTE EvenOdd)
{
   WORD W_Port;
   WORD W_Index;
   BYTE CurrentTransNumber;
   BYTE B_Count;
   static BYTE B_LastId = SMT_TRANSID_NOT_ASSIGNED;

   //Initiate SMT_TransArray if it is first transaction.
   //TODO: Initiator must be modified if more possible transaction statuses are added
   if ((SMT_TransArray[0].B_Status!=TRANSACTION_CLOSED)&&(SMT_TransArray[0].B_Status!=TRANSACTION_OPENED))
   {
      for (B_Count = 0; B_Count < MAX_TRANSACTION_NUMBER; B_Count++)
      {
         SMT_TransArray[B_Count].ID = SMT_TRANSID_NOT_ASSIGNED;
         SMT_TransArray[B_Count].B_Status = TRANSACTION_CLOSED;
      }
   }

   // Look for vacant transaction context
   for (B_Count = 0; B_Count < MAX_TRANSACTION_NUMBER; B_Count++)
   {
      if (SMT_TransArray[B_Count].B_Status == TRANSACTION_CLOSED)	//.ID == 0)
      {
         CurrentTransNumber = B_Count;
         break;
      }
   }

   // Handle no vacant context situation
   if (B_Count == MAX_TRANSACTION_NUMBER)
   {
      // Send Failed
      msgs("ERROR: No Vacant Transaction %d",B_Count);
      SMT_SendFailed(comm_use, EvenOdd, GENERIC_ER);
      return;
   }

   //extract port number from received packet
   //and check port's valdidity
   W_Port = ((WORD)msgpkt[0] << 8) | (WORD)msgpkt[1];
   if (W_Port >= MAX_SMTPORT)
   {
      msgs("Port 0x%X is invalid ",W_Port);
      SMT_SendFailed(comm_use, EvenOdd, GENERIC_ER);
      return;
   }

   //look for port entry in the handlers LUT
   for (W_Index=0; SMT_Port2HandlerTable[W_Index].PortID != MAX_SMTPORT;W_Index++)
   {
      if (SMT_Port2HandlerTable[W_Index].PortID == W_Port)
      {
         if (SMT_Port2HandlerTable[W_Index].Fp_SMT_MsgHandler != NULL_PTR)
         {
            msgs("Port 0x%X is supported",W_Port);
            break;
         }
         else
         {
            msgs("Port 0x%X is not supported",W_Port);
            SMT_SendFailed(comm_use, EvenOdd, GENERIC_ER);
            return;
         }
      }
   }

   if (W_Index == MAX_SMTPORT)
   {
      msgs("ERROR: Transaction was not opened due to Port 0x%X is invalid",W_Port);
      SMT_SendFailed(comm_use, EvenOdd, GENERIC_ER);
      return;
   }

   //define transaction ID
   for ( W_Index=0; W_Index<=255; W_Index++)
   {
      B_LastId++ ;

      /* don't assign an ID with value SMT_TRANSID_NOT_ASSIGNED*/
      if (B_LastId == SMT_TRANSID_NOT_ASSIGNED)
         B_LastId = 0;

      // check if transaction with this ID is not opened yet
      for ( B_Count = 0;B_Count < MAX_TRANSACTION_NUMBER; B_Count++ )
      {
         if ( (SMT_TransArray[B_Count].B_Status == TRANSACTION_OPENED) &&
               (SMT_TransArray[B_Count].ID == B_LastId))
            break;
      }
      // continue if ID used
      if (B_Count == MAX_TRANSACTION_NUMBER)
         break;
   }

   if (W_Index > 255)
   {
      msgs("ERROR: Can not find ID for this transaction ",0);
      SMT_SendFailed(comm_use, EvenOdd, GENERIC_ER);
      return;
   }

   msgs("Open Transaction: Port = 0x%d ",W_Port);

   SMT_TransArray[CurrentTransNumber].ID = B_LastId;
   SMT_TransArray[CurrentTransNumber].Port = W_Port;
   SMT_TransArray[CurrentTransNumber].Bp_Buff =  NULL_PTR;
   SMT_TransArray[CurrentTransNumber].BuffSize = 0;
   SMT_TransArray[CurrentTransNumber].B_Status = TRANSACTION_OPENED;
   SMT_TransArray[CurrentTransNumber].EvenOdd = EvenOdd;
   SMT_TransArray[CurrentTransNumber].comm_use = comm_use;

   // Sending response:

   OutMsg.len =  6;
   OutMsg.cmd =  CMD_SMTProtocol;
   #if 1 // 1
   if(B_ReplyEvenOdd%2)
      EvenOdd = EVEN_ODD_BIT;
   else
      EvenOdd = 0;
   B_ReplyEvenOdd++; // EvenOdd change
   OutMsg.BUF[0] = CM_TM_BIT | EvenOdd | SMT_SLAVE_RESPONSE_BIT;
   #else
   OutMsg.BUF[0] = CM_TM_BIT | EvenOdd | SMT_SLAVE_RESPONSE_BIT;
   #endif
   OutMsg.BUF[1] = TRANSACTION_OPENED;
   OutMsg.BUF[2] = SMT_TransArray[CurrentTransNumber].ID;

   gm_TimerStart(SMT_TransArray[CurrentTransNumber].B_TmrNum, SMT_TRANSACTION_TIMEOUT );
   
   msgs("Transaction OPENED, ID = 0x%x ",SMT_TransArray[CurrentTransNumber].ID);
   msgs("Transaction OPENED, NUM = 0x%x ",CurrentTransNumber);

   if((comm_use == I2C_SLAVE1_USE)||(comm_use == I2C_SLAVE2_USE)
      ||(comm_use == I2C_SLAVE3_USE)||(comm_use == I2C_SLAVE4_USE))
   {
      SLAVE_CH B_SlaveCh = comm_use - I2C_SLAVE1_USE;
      I2C_SLAVE_TYPE far* Sp_I2CSlavePtr = Sp_I2CSlavePtrGlobal[B_SlaveCh];
      _fmemcpy(&Sp_I2CSlavePtr->Buffer[Start_of_GprobeCmd_in_DDC2BiMsg], &OutMsg, OutMsg.len-1);
      gm_WriteCmdPkt((BYTE far*)&Sp_I2CSlavePtr->Buffer[Start_of_GprobeCmd_in_DDC2BiMsg], comm_use);
   }
   else
   {
      gm_WriteCmdPkt((BYTE far*)&OutMsg, comm_use);
   }
}

//******************************************************************************
//
// FUNCTION     :   void SMT_CloseTransaction(BYTE msgpkt[], BYTE comm_use, BYTE EvenOdd)
// USAGE        :   This function close transaction and send response to PC
//
// INPUT        :   BYTE msgpkt[] = msgpkt array from SMT message
//                  BYTE comm_use  = SERIAL_USE or DDC2BI_USE
//                  BYTE EvenOdd = Even/Odd message count
// OUTPUT       :   None
//
//******************************************************************************
void SMT_CloseTransaction(BYTE msgpkt[], BYTE comm_use, BYTE EvenOdd)
{
   BYTE TransID;
   BYTE CurrentTransNumber = SMT_TRANSID_NOT_ASSIGNED;
   BYTE B_Count;

   TransID = msgpkt[0];

   for (B_Count = 0; B_Count < MAX_TRANSACTION_NUMBER; B_Count++)
   {
      if (SMT_TransArray[B_Count].ID == TransID)
      {
         if (SMT_TransArray[B_Count].B_Status != TRANSACTION_CLOSED)
            CurrentTransNumber = B_Count;
         break;
      }
   }
   msgs("CloseTransaction(). Transaction table index = %d ",CurrentTransNumber);
   if (CurrentTransNumber == SMT_TRANSID_NOT_ASSIGNED)
   {
      msgs("ERROR: Invalid Transaction ID - NOT FOUND TRANSACTION WITH THIS ID  = %x ",TransID);
      SMT_SendFailed(comm_use, EvenOdd, TRANS_ID_ER);
      return;
   }

   if (SMT_TransArray[CurrentTransNumber].B_Status == TRANSACTION_CLOSED)// what if trasaction is in a BUSY state?
   {																	//WHY? may be just close trasaction without checking status?
      // Send Failed
      msgs("ERROR: Invalid Transaction ID ",0);
      SMT_SendFailed(comm_use, EvenOdd, TRANS_ID_ER);
      return;
   }

   OutMsg.len = 6;
   OutMsg.cmd  = CMD_SMTProtocol;
   #if 1 // 1
   if(B_ReplyEvenOdd%2)
      EvenOdd = EVEN_ODD_BIT;
   else
      EvenOdd = 0;
   B_ReplyEvenOdd++; // EvenOdd change   
   OutMsg.BUF[0]  = CM_TM_BIT | EvenOdd | SMT_SLAVE_RESPONSE_BIT;
   #else
   OutMsg.BUF[0]  = CM_TM_BIT | EvenOdd | SMT_SLAVE_RESPONSE_BIT;
   #endif
   OutMsg.BUF[1]  = TRANSACTION_CLOSED;
   OutMsg.BUF[2]  = SMT_TransArray[CurrentTransNumber].ID;

   msgs("Transaction 0x%x was closed.",OutMsg.BUF[2]);



   /* request on the SMT master side to close transaction linked to this slave transaction */

   if (SMT_CloseLinkedTransaction(&SMT_TransArray[CurrentTransNumber]) == gmd_PASS)
   {
      /* there is a connected fowarding smt transaction, so wait for the close ack before propagate back to host*/

      /* save proper reply to be sent later.*/
#if 1
      _fmemcpy(SMT_TransArray[CurrentTransNumber].Bp_Buff, &OutMsg, OutMsg.len);
#else
      memcpy(SMT_TransArray[CurrentTransNumber].Bp_Buff, &OutMsg, OutMsg.len);
#endif
   }
   else
   {
      SMT_ResetTransaction(&SMT_TransArray[CurrentTransNumber], TRUE);

      /* there is no connected forwarding smt transaction, so reply immediately*/
      if((comm_use == I2C_SLAVE1_USE)||(comm_use == I2C_SLAVE2_USE)
         ||(comm_use == I2C_SLAVE3_USE)||(comm_use == I2C_SLAVE4_USE))
      {
         SLAVE_CH B_SlaveCh = comm_use - I2C_SLAVE1_USE;
         I2C_SLAVE_TYPE far* Sp_I2CSlavePtr = Sp_I2CSlavePtrGlobal[B_SlaveCh];
         _fmemcpy(&Sp_I2CSlavePtr->Buffer[Start_of_GprobeCmd_in_DDC2BiMsg], &OutMsg, OutMsg.len-1);
         gm_WriteCmdPkt((BYTE far*)&Sp_I2CSlavePtr->Buffer[Start_of_GprobeCmd_in_DDC2BiMsg], comm_use);
      }
      else
      {
         gm_WriteCmdPkt((BYTE far*)&OutMsg, comm_use);
      }
   }
}



gmt_RET_PASSFAIL SMT_TransactionInit(void)
{
   BYTE i = 0;
   for (i = 0; i < MAX_TRANSACTION_NUMBER; i++)
   {
      /* assign the slave SMT timer to each transction*/
      SMT_TransArray[i].B_TmrNum = SMT_TIMEOUT_0_TMR + i;

      SMT_ResetTransaction(&SMT_TransArray[i], TRUE);
   }
   return SMT_MasterTransactionInit();
}

//******************************************************************************
//
// FUNCTION     :   void SMT_SendStatusSMT(BYTE msgpkt[], BYTE comm_use, BYTE EvenOdd)
// USAGE        :   This function send status of transaction to PC
//
// INPUT        :   BYTE msgpkt[] = msgpkt array from SMT message
//                  BYTE comm_use  = SERIAL_USE or DDC2BI_USE
//                  BYTE EvenOdd = Even/Odd message count
// OUTPUT       :   None
//
//******************************************************************************
void SMT_SendStatusSMT(BYTE msgpkt[], BYTE comm_use, BYTE EvenOdd)
{
   BYTE TransID;
   BYTE CurrentTransNumber = 0xFF;
   BYTE B_Count;

   TransID = msgpkt[0];

   for (B_Count = 0; B_Count < MAX_TRANSACTION_NUMBER; B_Count++)
   {
      if (SMT_TransArray[B_Count].ID == TransID)
      {
         CurrentTransNumber = B_Count;
         break;
      }
   }
   msgs("Transaction table index = = %d ",CurrentTransNumber);
   if (CurrentTransNumber == 0xFF)
   {
      msgs("ERROR: Invalid Transaction ID - NOT FOUND TRANSACTION WITH THIS ID  = %x ",TransID);
      SMT_SendFailed(comm_use, EvenOdd, TRANS_ID_ER);
      return;
   }


   OutMsg.len = 7;
   OutMsg.cmd  = CMD_SMTProtocol;
   #if 1 // 1
   if(B_ReplyEvenOdd%2)
      EvenOdd = EVEN_ODD_BIT;
   else
      EvenOdd = 0;
   B_ReplyEvenOdd++; // EvenOdd change   
   OutMsg.BUF[0] = CM_TM_BIT | EvenOdd | SMT_SLAVE_RESPONSE_BIT;
   #else
   OutMsg.BUF[0] = CM_TM_BIT | EvenOdd | SMT_SLAVE_RESPONSE_BIT;
   #endif
   OutMsg.BUF[1] = GET_STATUS_CMD;
   OutMsg.BUF[2] = TransID;
   OutMsg.BUF[3] = SMT_TransArray[CurrentTransNumber].B_Status;

   gm_TimerStart(SMT_TransArray[CurrentTransNumber].B_TmrNum, SMT_TRANSACTION_TIMEOUT );

   if((comm_use == I2C_SLAVE1_USE)||(comm_use == I2C_SLAVE2_USE)
      ||(comm_use == I2C_SLAVE3_USE)||(comm_use == I2C_SLAVE4_USE))
   {
      SLAVE_CH B_SlaveCh = comm_use - I2C_SLAVE1_USE;
      I2C_SLAVE_TYPE far* Sp_I2CSlavePtr = Sp_I2CSlavePtrGlobal[B_SlaveCh];
      _fmemcpy(&Sp_I2CSlavePtr->Buffer[Start_of_GprobeCmd_in_DDC2BiMsg], &OutMsg, OutMsg.len-1);
      gm_WriteCmdPkt((BYTE far*)&Sp_I2CSlavePtr->Buffer[Start_of_GprobeCmd_in_DDC2BiMsg], comm_use);
   }
   else
   {
      gm_WriteCmdPkt((BYTE far*)&OutMsg, comm_use);
   }
}

//******************************************************************************
//
// FUNCTION     :   void SMT_SendFailed(BYTE comm_use, BYTE EvenOdd, BYTE B_Reason)
// USAGE        :   This function send response "Failed" to PC
//
// INPUT        :   BYTE comm_use  = SERIAL_USE or DDC2BI_USE
//                  BYTE EvenOdd = Even/Odd message count
//                  BYTE B_Reason = The reason of fail
// OUTPUT       :   None
//
//******************************************************************************
void SMT_SendFailed(BYTE comm_use, BYTE EvenOdd, BYTE B_Reason)
{

   OutMsg.len = 6;
   OutMsg.cmd  = CMD_SMTProtocol;
   #if 1 // 1
   if(B_ReplyEvenOdd%2)
      EvenOdd = EVEN_ODD_BIT;
   else
      EvenOdd = 0;
   B_ReplyEvenOdd++; // EvenOdd change   
   OutMsg.BUF[0] = CM_TM_BIT | EvenOdd | SMT_SLAVE_RESPONSE_BIT;
   #else
   OutMsg.BUF[0] = CM_TM_BIT | EvenOdd | SMT_SLAVE_RESPONSE_BIT;
   #endif
   OutMsg.BUF[1] = FAILED;
   OutMsg.BUF[2] = B_Reason;

   if((comm_use == I2C_SLAVE1_USE)||(comm_use == I2C_SLAVE2_USE)
      ||(comm_use == I2C_SLAVE3_USE)||(comm_use == I2C_SLAVE4_USE))
   {
      SLAVE_CH B_SlaveCh = comm_use - I2C_SLAVE1_USE;
      I2C_SLAVE_TYPE far* Sp_I2CSlavePtr = Sp_I2CSlavePtrGlobal[B_SlaveCh];
      _fmemcpy(&Sp_I2CSlavePtr->Buffer[Start_of_GprobeCmd_in_DDC2BiMsg], &OutMsg, OutMsg.len-1);
      gm_WriteCmdPkt((BYTE far*)&Sp_I2CSlavePtr->Buffer[Start_of_GprobeCmd_in_DDC2BiMsg], comm_use);
   }
   else
   {
      gm_WriteCmdPkt((BYTE far*)&OutMsg, comm_use);
   }
}
//******************************************************************************
//
// FUNCTION     :   void SMT_SendData(BYTE comm_use, BYTE EvenOdd,BYTE ID)
// USAGE        :   This function sends data packet to PC as a transport message
//					It is used as reply to a transport message from PC side
//
// INPUT        :   BYTE comm_use  = SERIAL_USE or DDC2BI_USE
//                  BYTE EvenOdd = Even/Odd message count
//                  BYTE ID = Transaction ID number
// USED			:   Global OutMsg buffer 256 Bytes length;
//					The length of packet (OutMsg.len) and payload data (OutMsg.BUF[2]...) are set by a message handler
// OUTPUT       :   None
//
//******************************************************************************
void SMT_SendData(BYTE comm_use, BYTE EvenOdd,BYTE ID)
{

   OutMsg.len += SMT_HEADER_SIZE+CHECKSUM_SIZE;
   OutMsg.cmd	= CMD_SMTProtocol;
   #if 1 // 1
   if(B_ReplyEvenOdd%2)
      EvenOdd = EVEN_ODD_BIT;
   else
      EvenOdd = 0;
   B_ReplyEvenOdd++; // EvenOdd change
   OutMsg.BUF[0] = EvenOdd | SMT_SLAVE_RESPONSE_BIT | (OutMsg.BUF[0] & MORE_DATA_BIT);//transport message
   #else
   OutMsg.BUF[0] = EvenOdd | SMT_SLAVE_RESPONSE_BIT | (OutMsg.BUF[0] & MORE_DATA_BIT);//transport message
   #endif
   OutMsg.BUF[1] = ID;

   if((comm_use == I2C_SLAVE1_USE)||(comm_use == I2C_SLAVE2_USE)
      ||(comm_use == I2C_SLAVE3_USE)||(comm_use == I2C_SLAVE4_USE))
   {
      SLAVE_CH B_SlaveCh = comm_use - I2C_SLAVE1_USE;
      I2C_SLAVE_TYPE far* Sp_I2CSlavePtr = Sp_I2CSlavePtrGlobal[B_SlaveCh];
      _fmemcpy(&Sp_I2CSlavePtr->Buffer[Start_of_GprobeCmd_in_DDC2BiMsg], &OutMsg, OutMsg.len-1);
      gm_WriteCmdPkt((BYTE far*)&Sp_I2CSlavePtr->Buffer[Start_of_GprobeCmd_in_DDC2BiMsg], comm_use);
   }
   else
   {
      gm_WriteCmdPkt((BYTE far*)&OutMsg, comm_use);
   }
}
//******************************************************************************
//
// FUNCTION     :   void SMT_SendReTransmit(BYTE TransID, BYTE comm_use, BYTE EvenOdd, WORD W_Delay_ms)
// USAGE        :   This function send Re-Transmit command to PC
//
// INPUT        :   BYTE TransID = Transition ID
//                  BYTE comm_use  = SERIAL_USE or DDC2BI_USE
//                  BYTE EvenOdd = Even/Odd message count
//                  WORD W_Delay_ms = Delay in ms
// OUTPUT       :   None
//
//******************************************************************************
void SMT_SendReTransmit(BYTE TransID, BYTE comm_use, BYTE EvenOdd, WORD W_Delay_ms)
{
   BYTE B_Count;

   for (B_Count = 0; B_Count < MAX_TRANSACTION_NUMBER; B_Count++)
   {
      if (SMT_TransArray[B_Count].ID == TransID)
      {
         break;
      }
   }
   OutMsg.len = 8;
   OutMsg.cmd = CMD_SMTProtocol;
   #if 1 // 1
   if(B_ReplyEvenOdd%2)
      EvenOdd = EVEN_ODD_BIT;
   else
      EvenOdd = 0;
   B_ReplyEvenOdd++; // EvenOdd change   
   OutMsg.BUF[0] = CM_TM_BIT | EvenOdd | SMT_SLAVE_RESPONSE_BIT;
   #else
   OutMsg.BUF[0] = CM_TM_BIT | EvenOdd | SMT_SLAVE_RESPONSE_BIT;
   #endif
   OutMsg.BUF[1] = RE_TRANSMIT_CMD;
   OutMsg.BUF[2] = TransID;
   OutMsg.BUF[3] = W_Delay_ms & 0xFF;
   OutMsg.BUF[4] = (W_Delay_ms >> 8) & 0xFF;
   gm_TimerStart(SMT_TransArray[B_Count].B_TmrNum, W_Delay_ms/1000+1 );

   if((comm_use == I2C_SLAVE1_USE)||(comm_use == I2C_SLAVE2_USE)
      ||(comm_use == I2C_SLAVE3_USE)||(comm_use == I2C_SLAVE4_USE))
   {
      SLAVE_CH B_SlaveCh = comm_use - I2C_SLAVE1_USE;
      I2C_SLAVE_TYPE far* Sp_I2CSlavePtr = Sp_I2CSlavePtrGlobal[B_SlaveCh];
      _fmemcpy(&Sp_I2CSlavePtr->Buffer[Start_of_GprobeCmd_in_DDC2BiMsg], &OutMsg, OutMsg.len-1);
      gm_WriteCmdPkt((BYTE far*)&Sp_I2CSlavePtr->Buffer[Start_of_GprobeCmd_in_DDC2BiMsg], comm_use);
   }
   else
   {
      gm_WriteCmdPkt((BYTE far*)&OutMsg, comm_use);
   }
}

//******************************************************************************
//
// FUNCTION     :   void SMT_SendReTransmit(BYTE TransID, BYTE comm_use, BYTE EvenOdd, WORD W_Delay_ms)
// USAGE        :   This function send Busy command to PC
//
// INPUT        :   BYTE TransID = Transition ID
//                  BYTE comm_use  = SERIAL_USE or DDC2BI_USE
//                  BYTE EvenOdd = Even/Odd message count
//                  WORD W_Delay_ms = Delay in ms
// OUTPUT       :   None
//
//******************************************************************************
void SMT_SendBusy(BYTE TransID, BYTE comm_use, BYTE EvenOdd, WORD W_Delay_ms)
{
   BYTE B_Count;

   for (B_Count = 0; B_Count < MAX_TRANSACTION_NUMBER; B_Count++)
   {
      if (SMT_TransArray[B_Count].ID == TransID)
      {
         break;
      }
   }
   OutMsg.len = 8;
   OutMsg.cmd = CMD_SMTProtocol;
   #if 1 // 1
   if(B_ReplyEvenOdd%2)
      EvenOdd = EVEN_ODD_BIT;
   else
      EvenOdd = 0;
   B_ReplyEvenOdd++; // EvenOdd change   
   OutMsg.BUF[0] = CM_TM_BIT | EvenOdd | SMT_SLAVE_RESPONSE_BIT;
   #else
   OutMsg.BUF[0] = CM_TM_BIT | EvenOdd | SMT_SLAVE_RESPONSE_BIT;
   #endif
   OutMsg.BUF[1] = BUSY_PROCESSING;
   OutMsg.BUF[2] = TransID;
   #if 1 // little endian
   OutMsg.BUF[3] = (W_Delay_ms >> 8) & 0xFF;
   OutMsg.BUF[4] = W_Delay_ms & 0xFF;   
   #else // big endian
   OutMsg.BUF[3] = W_Delay_ms & 0xFF;
   OutMsg.BUF[4] = (W_Delay_ms >> 8) & 0xFF;
   #endif
   
   gm_TimerStart(SMT_TransArray[B_Count].B_TmrNum, W_Delay_ms + 1 );

   if((comm_use == I2C_SLAVE1_USE)||(comm_use == I2C_SLAVE2_USE)
      ||(comm_use == I2C_SLAVE3_USE)||(comm_use == I2C_SLAVE4_USE))
   {
      SLAVE_CH B_SlaveCh = comm_use - I2C_SLAVE1_USE;
      I2C_SLAVE_TYPE far* Sp_I2CSlavePtr = Sp_I2CSlavePtrGlobal[B_SlaveCh];
      _fmemcpy(&Sp_I2CSlavePtr->Buffer[Start_of_GprobeCmd_in_DDC2BiMsg], &OutMsg, OutMsg.len-1);
      gm_WriteCmdPkt((BYTE far*)&Sp_I2CSlavePtr->Buffer[Start_of_GprobeCmd_in_DDC2BiMsg], comm_use);
   }
   else
   {
      gm_WriteCmdPkt((BYTE far*)&OutMsg, comm_use);
   }
}


/* return a transaction ID from the message */
BYTE SMT_GetTransID(MsgPacket_t * message)
{
   if (message->msgType & CM_TM_BIT)
   {
      switch (message->control)
      {
         case TRANSACTION_OPENED:
         case CLOSE_TRANSACTION_CMD:
         case TRANSACTION_CLOSED:
         case RE_TRANSMIT_CMD:
         case BUSY_PROCESSING:
         case GET_STATUS_CMD:
            /* control message. Transaction id stored in first parameter field*/
            return message->msgpkt[0];

         default:
            break;
      }
   }
   else
   {
      /* transport message.  Transaction id stored in control field*/
      return message->control;
   }
   return SMT_TRANSID_NOT_ASSIGNED;
}


/* return the transaction index from a given SMT ID*/
BYTE SMT_GetTransIndex(BYTE TransID)
{
   BYTE i = 0;
   for (i = 0; i < MAX_TRANSACTION_NUMBER; i++)
   {
      if (TransID == SMT_TransArray[i].ID)
         return i;
   }
   return 0;
}


/* Search through entire list of transactions to see if transaction id is valid*/
SMT_TransactionStruct_t SMT_RAM * IsTransIDValid(BYTE TransID)
{
   BYTE i = 0;

   if (TransID == SMT_TRANSID_NOT_ASSIGNED)
      return NULL_PTR;

   for (i = 0; i < MAX_TRANSACTION_NUMBER; i++)
   {
      if (TransID == SMT_TransArray[i].ID)
         return &SMT_TransArray[i];
   }
   return NULL_PTR;
}

void SMT_CloseExpiredTransactions(void)
{
#if SMT_TIMEOUT
   BYTE B_Count;

   // close expired slave transactions
   for (B_Count = 0; B_Count<MAX_TRANSACTION_NUMBER;B_Count++)
   {
      if ((SMT_TransArray[B_Count].B_Status != TRANSACTION_CLOSED)
            && (gm_TimerCheck(SMT_TransArray[B_Count].B_TmrNum) != TIMER_OK))
      {
         gm_Print("timed out slave B_Count - %d", B_Count);

         SMT_ResetTransaction(&SMT_TransArray[B_Count], FALSE);
         msgs("Transaction 0x%X is closed because of timeout ",SMT_TransArray[B_Count].ID);
      }
   }
#endif
}


//******************************************************************************
//
// FUNCTION     :   BYTE SMT_MessageHandler(MsgPacket_t *message, BYTE comm_use)
// USAGE        :   This function parse the SMT messages
//
// INPUT        :   MsgPacket_t *message = SMT message
//                  BYTE comm_use  = SERIAL_USE or DDC2BI_USE
// OUTPUT       :   0 = if no error, 1 = if error is found
//
//******************************************************************************

BYTE SMT_MessageHandler(MsgPacket_t * message, BYTE comm_use)
{
   BYTE ExpectedEvenOdd;
   BYTE B_Count;
   BYTE B_TransactionID;
   WORD W_PortID;
   WORD W_TblIndex;
   SMT_TransactionStruct_t SMT_RAM * pCurrentTransaction;
   WORD CurrentTransNumber;
   #ifdef ST_4K2K_93xx_BOARD
	SMT_Port_t  SMTPort;
   #endif

   /* 	is this message coming from a slave response.
   	if so, do not parse the message, since slave handler
   	does not take care of other slave responses.
   */
   if (message->msgType & SMT_SLAVE_RESPONSE_BIT)
      return 0;

   // delay for overwork VB script problem
//	gm_Delay10ms(10);
   //Initiate transaction context table SMT_TransArray if it is first transaction.
   //TODO: Initiator must be modified if more possible transaction statuses are added
   if ((SMT_TransArray[0].B_Status!=TRANSACTION_CLOSED)&&(SMT_TransArray[0].B_Status!=TRANSACTION_OPENED))
   {
      for (B_Count = 0; B_Count < MAX_TRANSACTION_NUMBER; B_Count++)
      {
         SMT_TransArray[B_Count].ID = SMT_TRANSID_NOT_ASSIGNED;
         SMT_TransArray[B_Count].B_Status = TRANSACTION_CLOSED;
         SMT_TransArray[B_Count].B_TmrNum= SMT_TIMEOUT_0_TMR+B_Count;
      }
   }

   ExpectedEvenOdd = message->msgType & EVEN_ODD_BIT;

   #if 0 // 1
   if(B_ODD_EVEN_OverRide)
   {
      if(ExpectedEvenOdd)
      {
         ExpectedEvenOdd = 0;
         //gm_Printf("OverRide000", 0);
      }
      else
      {
         ExpectedEvenOdd = EVEN_ODD_BIT; 
         //gm_Printf("OverRide111", 0);
      }            
   }
   #endif
         
   B_TransactionID = SMT_GetTransID(message);
   pCurrentTransaction = IsTransIDValid(B_TransactionID);
   CurrentTransNumber = SMT_GetTransIndex(B_TransactionID);

	#ifdef ST_4K2K_93xx_BOARD
	//if OPEN_TRANSACTION_CMD, get port from smt message
	if ((message->msgType & CM_TM_BIT) && (message->control == OPEN_TRANSACTION_CMD))
		SMTPort = ((WORD)message->msgpkt[0] << 8) | (WORD)message->msgpkt[1];
	else
		SMTPort = pCurrentTransaction->Port;
	#endif

#if (!SMT_IGNORE_EVENODD)
	#ifdef ST_4K2K_93xx_BOARD
	if((GetChipID_4K2K()==CHIPID_4K2K_L) || (GetChipID_4K2K()==CHIPID_4K2K_FE) &&
		((SMTPort ==SMTPORT_GAMMA || SMTPort ==SMTPORT_LBC || SMTPort ==SMTPORT_ACT)))
	{
	//L and FE do not check odd/even for port SMTPORT_GAMMA, SMTPORT_LBC and SMTPORT_ACT
	}
	else
	#endif
	{
   if (pCurrentTransaction)
   {
      if (pCurrentTransaction->EvenOdd != ExpectedEvenOdd)
      {
         /* correct sequence. update transaction's evenodd count*/
         pCurrentTransaction->EvenOdd = ExpectedEvenOdd;
      }
      else
      {
         #if 1
         gm_Printf("ERROR: EVEN/ODD Resend", 0);
         //SMT_SendData(comm_use, ExpectedEvenOdd, pCurrentTransaction->ID);

         if((comm_use == I2C_SLAVE1_USE)||(comm_use == I2C_SLAVE2_USE)
            ||(comm_use == I2C_SLAVE3_USE)||(comm_use == I2C_SLAVE4_USE))
         {
            SLAVE_CH B_SlaveCh = comm_use - I2C_SLAVE1_USE;
            I2C_SLAVE_TYPE far* Sp_I2CSlavePtr = Sp_I2CSlavePtrGlobal[B_SlaveCh];
            _fmemcpy(&Sp_I2CSlavePtr->Buffer[Start_of_GprobeCmd_in_DDC2BiMsg], &OutMsg, OutMsg.len-1);
            gm_WriteCmdPkt((BYTE far*)&Sp_I2CSlavePtr->Buffer[Start_of_GprobeCmd_in_DDC2BiMsg], comm_use);
         }
         else
         {
         	gm_WriteCmdPkt((BYTE far*)&OutMsg, comm_use);
         }         
         
         return 0; // success
         #else
         /*incorrect sequence*/
         SMT_SendFailed(comm_use, ExpectedEvenOdd, CHECK_SUM_ER);
         msgs("ERROR OF SMT: Even/Odd message count is wrong.",0);
         return 1;
         #endif
      }
   }
	}
#endif

   SMT_CloseExpiredTransactions();

   //parse message
   if (message->msgType & CM_TM_BIT)     // Is it a Control Message ?
   {
      msgs("Message Control = %d ",message->control);
      #ifdef ST_4K2K_93xx_BOARD
      if(GetChipID_4K2K()==CHIPID_4K2K_R)
		{
			if (SMTPort ==SMTPORT_GAMMA || SMTPort ==SMTPORT_LBC || SMTPort ==SMTPORT_ACT)
			{
				SMT_SendBusy(B_TransactionID,comm_use,ExpectedEvenOdd,SMT_TRANSACTION_TIMEOUT);
            if((SMT_ForwardMessageToSlave(message, SOCIPC_DEVICEL_DST_ADDRESS) != PORT_MSG_HNDLR_OK) ||
                (SMT_ForwardMessageToSlave(message, SOCIPC_DEVICEFE_DST_ADDRESS) != PORT_MSG_HNDLR_OK))
			{
                SMT_SendFailed(comm_use, ExpectedEvenOdd, GENERIC_ER);
                gm_Print("Cmd %d Failed to FE/L",message->control);
                return 1;
				}
			}
		}
		#endif

      switch (message->control)     // Yes
      {
         case OPEN_TRANSACTION_CMD:  // Open Transaction
            SMT_OpenTransaction(message->msgpkt, comm_use, ExpectedEvenOdd);
            break;
         case CLOSE_TRANSACTION_CMD: // Close Transaction
            SMT_CloseTransaction(message->msgpkt, comm_use, ExpectedEvenOdd);
            break;
         case GET_STATUS_CMD:       // Get Status
            SMT_SendStatusSMT(message->msgpkt, comm_use, ExpectedEvenOdd);
            break;
         case FAILED:				//Failed
            return 1;
         default:
            msgs("ERROR: Unknown Message Control Code = %d ",message->control);
            SMT_SendFailed(comm_use, ExpectedEvenOdd, CORRUPT_MSG_ER);
            return 1;
      };
   }
   else if (pCurrentTransaction == NULL_PTR)
   {
      /* invalid transport message*/
      msgs("ERROR: Invalid Transaction ID - NOT FOUND TRANSACTION WITH THIS ID	= %x ",B_TransactionID);
      SMT_SendFailed(comm_use, ExpectedEvenOdd, TRANS_ID_ER);
      return 1;
   }
   else		// This is a Transport Message
   {
      WORD W_CMD;

      if (pCurrentTransaction->B_Status != TRANSACTION_OPENED)
      {
         msgs("ERROR: Invalid Transaction ID. THIS TRANSACTION IS NOT OPENED. = %x ",B_TransactionID);
         SMT_SendFailed(comm_use, ExpectedEvenOdd, TRANS_ID_ER);
         return 1;
      }

      // Get port ID numnber
      W_PortID = pCurrentTransaction->Port;
      SMT_Features.CommType = (comm_use == SERIAL_USE) ? 1:0;
      SMT_Features.NoMoreInputData = (message->msgType & MORE_DATA_BIT) ? 1:0;
      SMT_Features.Length= (int)(message->len);

      // Call an apprpriate Port Handler depending on Port ID
      for (W_TblIndex = 0;;W_TblIndex++)
      {
         //look for port ID in the handlers LUT
         WORD W_TblPort = SMT_Port2HandlerTable[W_TblIndex].PortID;

         //End of LUT is reached?
         if (W_TblPort < MAX_SMTPORT)
         {
            // Is it desirted port?
            if (W_PortID ==W_TblPort)
            {
               // Is the pointer to handler valid?
               if (SMT_Port2HandlerTable[W_TblIndex].Fp_SMT_MsgHandler != NULL_PTR)
               {
                  BYTE ErrCode;
                  //execute the handler
                  SMT_Features.ActivePortNumber = CurrentTransNumber;
                  OutMsg.BUF[0] = 0;
                  msgs("Transport message to port 0x%X",W_PortID);
                  gm_TimerStart(pCurrentTransaction->B_TmrNum, SMT_TRANSACTION_TIMEOUT);
                  ErrCode = SMT_Port2HandlerTable[W_TblIndex].Fp_SMT_MsgHandler(message, comm_use);

                  #if 0 // 1
                  if(B_ReturnBusy)
                  {
                     B_ReturnBusy = 0;

                     if(B_ODD_EVEN_OverRide)
                        B_ODD_EVEN_OverRide = 0;
                     else
                        B_ODD_EVEN_OverRide = 1;

                     // toggle the even odd...
                     if(ExpectedEvenOdd)
                     {
                        ExpectedEvenOdd = 0;
                     }
                     else
                     {
                        ExpectedEvenOdd = EVEN_ODD_BIT; 
                     }
                     
                  }
                  #endif
                                   
                  //handle error code
                  switch (ErrCode)
                  {
                     case PORT_MSG_HNDLR_OK:
                        SMT_SendData(comm_use, ExpectedEvenOdd, pCurrentTransaction->ID);
                     case PORT_MSG_HNDLR_SELFCARED: //SMT parser don't have to care about a reply. The message handler replied by itself.
                        ErrCode = 0;
                        break;
                     case PORT_MSG_HNDLR_RETRANSMIT: //SMT parser don't have to care about a reply. The message handler replied by itself.
                        SMT_SendReTransmit(pCurrentTransaction->ID,comm_use,ExpectedEvenOdd,(WORD)(OutMsg.BUF[3])+((WORD)(OutMsg.BUF[4])<<8));
                        ErrCode = 0;
                        break;
                     case PORT_MSG_HNDLR_BUSY:
                        SMT_SendBusy(pCurrentTransaction->ID,comm_use,ExpectedEvenOdd,(WORD)(OutMsg.BUF[3])+((WORD)(OutMsg.BUF[4])<<8));
                        ErrCode = 0;
                        break;
                     default:
                        msgs("ERROR: Port 0x%X Message Handler failed.",W_PortID);
                        SMT_SendFailed(comm_use, ExpectedEvenOdd, GENERIC_ER);
                        ErrCode = 1;
                  }
                  SMT_Features.ActivePortNumber = MAX_TRANSACTION_NUMBER;
                  return ErrCode;
               }
               else
                  break; // port is not supported
            }
            // continue

         }
         else
            break; // end of table is reached
      }

      //treat situation when no port id is found in the hadlers LUT
      msgs("ERROR: Port 0x%X is not supported.",W_PortID);
      SMT_SendFailed(comm_use, ExpectedEvenOdd, GENERIC_ER);
      return 1;
   }

   return 0;
}

//******************************************************************************
//
// FUNCTION     :   BYTE SMT_GetBuffer(BYTE  B_TransactionID, BYTE far * Bp_Buff, WORD W_Size)
// USAGE        :   This function allocates memory for parsers
//
// INPUT        :   MsgPacket_t *message = SMT message
//                  BYTE comm_use = SERIAL_USE or DDC2BI_USE
//
// OUTPUT       :   0 = if no error and buffer is free, 1 = if no error and buffer has data from previous messages
//					0xFF if error;
//
//******************************************************************************
BYTE SMT_GetBuffer(BYTE  B_TransactionID,  BYTE far * far * Bp_Buff, WORD W_Size)
{
   BYTE B_Index;
   BYTE B_RetCode = 1;

   //check requested memory size
   if (W_Size <= SMT_MAX_BUF_SIZE)
   {
      //look for transaction ID
      for (B_Index = 0; B_Index < MAX_TRANSACTION_NUMBER; B_Index++)
      {
         if (SMT_TransArray[B_Index].ID == B_TransactionID)
         {
            if (SMT_TransArray[B_Index].Bp_Buff == NULL_PTR)
            {
               //pointer is null when it is a first transport message
               //write buffer pointer to transaction structure and return 0
               SMT_TransArray[B_Index].Bp_Buff = (BYTE far *)&SMT_TransArray[B_Index].Ba_Buff[0];
               SMT_TransArray[B_Index].BuffSize = W_Size;
               B_RetCode = 0;
            }
            *Bp_Buff = (BYTE far *)SMT_TransArray[B_Index].Bp_Buff;
            return B_RetCode;
         }
      }
   }
   return 0xFF;// memory allocating error
}

//******************************************************************************
//
// FUNCTION     :   BYTE GetBuffer(BYTE far * Bp_Buff, WORD W_Size)
// USAGE        :   This function allocates memory for port message handler
//
// INPUT        :   MsgPacket_t *message = SMT message
//                  BYTE comm_use = SERIAL_USE or DDC2BI_USE
//
// OUTPUT       :   0 = if no error and buffer is free, 1 = if no error and buffer has data from previous messages
//					0xFF if error;
//
//******************************************************************************
BYTE GetBuffer( BYTE far * far * Bp_Buff, WORD W_Size)
{
   return SMT_GetBuffer(SMT_TransArray[SMT_Features.ActivePortNumber].ID, Bp_Buff, W_Size);
}


//******************************************************************************
//
// FUNCTION     :   BYTE * GetOutMsgLenPtr(void)
// USAGE        :   Returns pointer to a byte contaning output message length. Port handler must determine the output message length.
//
// INPUT        :   No
//
// OUTPUT       :   Pointer to the buffer length byte; Pointer to buffer can be get using extern GetOutMsgBufPtr() function
//
//******************************************************************************
BYTE * GetOutMsgLenPtr(void)
{
   return &(OutMsg.len);
}

//******************************************************************************
//
// FUNCTION     :   extern BYTE * GetOutMsgBufPtr(void)
// USAGE        :   Returns pointer to an output message buffer. Port handler can write output data to the buffer.
//					Length of payload data in the buffer must be written to the byte pointed by GetOutMsgLenPtr() function;
//
// INPUT        :   No
//
// OUTPUT       :   Pointer to the output message buffer.
//
//******************************************************************************
BYTE * GetOutMsgBufPtr(void)
{
   return (BYTE*)&(((MsgPacket_t *)(&OutMsg))->msgpkt);
}

//******************************************************************************
//
// FUNCTION     :    BOOL MoreDataExpected(void)
// USAGE        :   Informs Port message handler that current message is the last in the sequnce of transport messages.
//
//
// INPUT        :   No
//
// OUTPUT       :   TRUE if it is the last transport message;
//					FLASE if more transport messages are expected;
//
//******************************************************************************
BOOL MoreDataExpected(void)
{
   return (SMT_Features.NoMoreInputData) ? TRUE : FALSE ;
}

//******************************************************************************
//
// FUNCTION     :    BOOL MoreData(void)
// USAGE        :   Used by a Port message handler to inform that al data is transfered.
//
//
// INPUT        :   TRUE 	if there is no more data to be sent;
//					FLASE 	more data must be sent;
//
// OUTPUT       :   No
//
//******************************************************************************
void MoreData(BOOL MoreData)
{
   if (MoreData)
      OutMsg.BUF[0] |= MORE_DATA_BIT;
   else
      OutMsg.BUF[0] &= ~MORE_DATA_BIT;
}

//******************************************************************************
//
// FUNCTION     :   WORD GetMaxDataLength (void)
// USAGE        :   Informs a Port message handler about maximum output message length
//					which can be written to a buffer which pointer is returned by GetOutMsgBufPtr() function
//
//
// INPUT        :   No
//
// OUTPUT       :   Length in 16 bit integer;
//
//******************************************************************************
WORD GetMaxDataLength (void)
{

   return ((SMT_Features.CommType) ? SIO_RxBufLen  - SMT_HEADER_SIZE -CHECKSUM_SIZE : 123 - SMT_HEADER_SIZE);
   //TODO: specify exact value for DDC2BI message size and replace it wth correct definitions.
}

//******************************************************************************
//
// FUNCTION     :   WORD GetMesageLength (void)
// USAGE        :   Returns payload data length of current tramsport message;
//
//
// INPUT        :   Len
//
// OUTPUT       :   Length in 16 bit integer;
//
//******************************************************************************
WORD GetMesageLength (void)
{

   return (WORD)(SMT_Features.Length - SMT_HEADER_SIZE -CHECKSUM_SIZE);
}
//#endif //Debug

#ifdef ST_4K2K_93xx_BOARD
//******************************************************************************
//
// FUNCTION     :   PortMsgHndlrReplyType SMT_ForwardMessageToSlave(MsgPacket_t *message, BYTE Addr)
// USAGE        :   master Athena forwards SMT message to slave Athena's via I2C
//
//
// INPUT        :   MsgPacket_t *message : pointer to SMT message
//                  Addr - Slave Athena I2C address
//
// OUTPUT       :   none
//
//RETURN        :   PortMsgHndlrReplyType - PORT_MSG_HNDLR_ERROR / PORT_MSG_HNDLR_OK
//
//******************************************************************************
PortMsgHndlrReplyType SMT_ForwardMessageToSlave(MsgPacket_t *message, BYTE Addr)
{
    PortMsgHndlrReplyType   ErrorCode = PORT_MSG_HNDLR_OK;
    BYTE MsgBuf[SOCIPC_TRANS_MSG_MAX_SIZE];
    BYTE num_frames = (message->len + SMT_FWD_MESSAGE_MAX_LEN -1) / SMT_FWD_MESSAGE_MAX_LEN; //reserve 1 byte for frame counter MsgBuf[0]
    BYTE frame_count = 0, ptr_pos_offset = 0;
    
//refer to doc "4Kx2K System FW SMT Design Gamma Calibration and Uniformity Compensation"
    //message->len is the total byte number of message structure, 
    //message->msgpkt[] includes command, message data and padding + checksum
    
    //inter-chip communication to Athena_L
    //before forwarding smt to slave, send longer timeout time
    for (frame_count = num_frames; frame_count > 0; frame_count--)
    {
        MsgBuf[0] = frame_count - 1;//0x00 is the last frame

        if (MsgBuf[0] == 0x00)  //the last frame (or only 1 frame to forward)
            SMTMsgLen = ((message->len - 1) % SMT_FWD_MESSAGE_MAX_LEN) + 1;
        else
            SMTMsgLen = SMT_FWD_MESSAGE_MAX_LEN;
        memcpy(&(MsgBuf[1]), (BYTE *)((BYTE *)message + ptr_pos_offset), SMTMsgLen); 
        ptr_pos_offset = (num_frames - frame_count + 1)*SMTMsgLen;
        
        SMTMsgLen++; //plus MsgBuf[0] - frame counter
        if(SocIPCSendCmdPacketParser(Addr, SICMD_WO_SMT_COMMAND, MsgBuf) == FALSE)
        {
            BYTE ReTryCounter = 3;

            while (ReTryCounter)
            {
                if(SocIPCSendCmdPacketParser(Addr, SICMD_WO_SMT_COMMAND, MsgBuf) == TRUE)
                    break;
                ReTryCounter --;
            }
            if(ReTryCounter==0)
            {
                ErrorCode = PORT_MSG_HNDLR_ERROR;
                break;
            }
        }
    }
    return ErrorCode;
}
//******************************************************************************
//
// FUNCTION     :   BOOL SMT_SlaveMessageHandler( /*BYTE SMT_Port_Num, */BYTE * data, BYTE comm_use)
// USAGE        :   slave handles SMT commands
//
//
// INPUT        :   data : pointer to SMT message   [0] Length
//                                                  [1] Command (inter-chip communication command)
//                                                  [2] "Message Control" byte
//                                                  [3] - [(max) 254] MsgPacket_t 
//                  comm_use : comm port to use - should be "0", 
//                             although it is ignored by slave Athena's, some data could be still sent to this port
//
// OUTPUT       :   none
//
//RETURN        :   BOOL -  TRUE (1) Failed
//                          FALSE(0) OK
//
//******************************************************************************
BOOL SMT_SlaveMessageHandler( /*BYTE SMT_Port_Num, */BYTE * data, BYTE comm_use)
{

    #define MAX_SMT_MESSAGE_BUF_SIZE    255
    
    static BYTE DataBuf[MAX_SMT_MESSAGE_BUF_SIZE];
    static BYTE DataBufIndex = 0;
    BYTE SMTMsgLen = *data - 4; //len cmd msgCtrl SMTMsg Checksum
    BYTE FrameCounter = *(data + 2);//msgCtrl - frame counter
    BOOL ErrorCode = FALSE;

    if (SMTMsgLen > SMT_FWD_MESSAGE_MAX_LEN)
        SMTMsgLen = SMT_FWD_MESSAGE_MAX_LEN;
    memcpy(&DataBuf[DataBufIndex], (data + 3), SMTMsgLen);
    DataBufIndex += SMTMsgLen;
    if (FrameCounter == 0)  //0 - the last message frame or only 1 frame
    {
        SMT_MessageHandler((MsgPacket_t *)DataBuf, comm_use);
        DataBufIndex = 0; //reset buf index
    }
    return ErrorCode;
}

void SMT_InterChip_Comm_Test(void)
{
    if((gm_ReadRegWord(PIP_IN_OFFSET1) & 0xFF) == 0xFF) //debug print
    {
    
        gm_WriteRegWord(PIP_IN_OFFSET1, 0);
//        SMT_Debug_Print("Athena_R");
    }
    else if((gm_ReadRegWord(PIP_IN_OFFSET1) & 0xFF) == 0xFE) //OPEN_TRANSACTION_CMD
    {
        BYTE MsgBuf[30] = {0x00, 0x07, 0xFE, 0x03, 0x00,
                0x00, 0x1B, 0x00, 0xDD}; //port - LBC (27)
        gm_WriteRegWord(PIP_IN_OFFSET1, 0);
        SMTMsgLen = 9;
        if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_WO_SMT_COMMAND, MsgBuf) == FALSE)
        {
            gm_Print("Send port error L - LBC_Open_Command", 0);
        }
        if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEFE_DST_ADDRESS, SICMD_WO_SMT_COMMAND, MsgBuf) == FALSE)
        {
            gm_Print("Send port error FE - LBC_Open_Command", 0);
        }
    }
    else if((gm_ReadRegWord(PIP_IN_OFFSET1) & 0xFF) == 0xFD) //CLOSE_TRANSACTION_CMD
    {
        BYTE MsgBuf[30] = {0x00, 0x06, 0xFE, 0x03, 0x02,
                0x03, 0xF4, 0xF7};
        gm_WriteRegWord(PIP_IN_OFFSET1, 0);
        SMTMsgLen = 8;
        if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_WO_SMT_COMMAND, MsgBuf) == FALSE)
        {
            gm_Print("Send port error L - Close", 0);
        }
        if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEFE_DST_ADDRESS, SICMD_WO_SMT_COMMAND, MsgBuf) == FALSE)
        {
            gm_Print("Send port error FE - Close", 0);
        }
    }
    else if((gm_ReadRegWord(PIP_IN_OFFSET1) & 0xFF) == LBC_ShowBorder)
    {
    
        BYTE MsgBuf[30] = {0x00, 0x1B, 0xFE, 0x00, 0x02,
                LBC_ShowBorder, 
                0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF,
                0x00, 0x00, 0x05, 0x05, 0x01, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        gm_WriteRegWord(PIP_IN_OFFSET1, 0);
//refer to doc "4Kx2K System FW SMT Design Gamma Calibration and Uniformity Compensation"
            //message->len is the total byte number of message structure, 
            //message->msgpkt[] includes command, message data and padding + checksum
            //for this command, 
            //message->len = 27, length of msgpkt[] is 23 : 1 command (0x08), 20 bytes data, 1 padding byte and 1 checksum byte
        SMTMsgLen = 28;
        if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEFE_DST_ADDRESS, SICMD_WO_SMT_COMMAND, MsgBuf) == FALSE)
        {
            gm_Print("Send port error FE - LBC_ShowBorder", 0);
        }
        if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_WO_SMT_COMMAND, MsgBuf) == FALSE)
        {
            gm_Print("Send port error L - LBC_ShowBorder", 0);
        }
    }
    else if((gm_ReadRegWord(PIP_IN_OFFSET1) & 0xFF) == LBC_DisplayField)
    {
        BYTE MsgBuf[30] = {0x00, 0x1B, 0xFE, 0x02, 0x01,
                LBC_DisplayField, 
                0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        gm_WriteRegWord(PIP_IN_OFFSET1, 0);
        SMTMsgLen = 28;
        if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_WO_SMT_COMMAND, MsgBuf) == FALSE)
        {
            gm_Print("Send port error L - LBC_DisplayField", 0);
        }
    }
}

#if 0
void SMT_Debug_Print(char far * str)
{
    char buf[120], i;
    SMTMsgLen = 30;
gm_Print("1. %s", str);
gm_Printf("1.5 len = %d", SMTMsgLen);

    for (i=0;i<SMTMsgLen;i++)
        buf[i] = str[i];
    if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_WO_SMT_DEBUG_PRINT, buf) == FALSE)
    {
        gm_Print("Send port error L - SMT_Debug_Print", 0);
    }
}
#endif
#endif

#endif //USE_SMT

