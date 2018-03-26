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
// MODULE:      smt_msgfwd.h
//
//******************************************************************

#ifdef USE_SMT
//*************************************************************************
//
//*************************************************************************
#include "CommLink\smt.h"

#define DDC2BI_RESPONSE_PING_TIME		0//in 10ms increments
#define SERIAL_CHUNK_SIZE 	250
/*serial - maximum number of bytes for a buffer
  that can be sent in one smt packet, excluding
  length, smt command, smt header and checksum field
  overhead of this is 5 bytes, so if send buffer is 256 bytes,
  then max size is 251 for buffer*/

#define DDC2BI_CHUNK_SIZE	122
/*ddc2bi - maximum number of bytes for a buffer
  that can be sent in one smt packet, excluding
  length, smt command, smt header, vcp, and checksum field
  overhead*/


void SMT_FormatReply(BYTE * Bp_Response, SMT_TransactionStruct_t SMT_RAM *  pSlaveTransaction);
SMT_TransactionStruct_t SMT_RAM * SendSMTMsg(BYTE SMTPort, BYTE HWPort, BYTE *pBuffer, WORD Size, BOOL MoreDataPending);
SMT_TransactionStruct_t  SMT_RAM * OpenTransaction(BYTE SMTPort, BYTE HWPort);
int CloseTransaction(BYTE ID);
int ClosePTransaction(SMT_TransactionStruct_t SMT_RAM * pTransaction);
gmt_RET_PASSFAIL SMT_MasterTransactionInit(void);
void SMT_CloseExpiredMasterTransactions(void);

gmt_RET_PASSFAIL SMT_Reply(SMT_TransactionStruct_t  SMT_RAM *  pTransaction, BYTE SMT_RAM * Bp_SendBuf);
gmt_RET_PASSFAIL SMT_Send(SMT_TransactionStruct_t SMT_RAM *  pTransaction, BYTE SMT_RAM *Bp_SendBuf);

void SetResponseTimer(BYTE TimeOut10ms);
void SMT_MasterHandler(void);
gmt_RET_PASSFAIL SMT_LinkTransactions(SMT_TransactionStruct_t SMT_RAM *pT1, SMT_TransactionStruct_t SMT_RAM *pT2);
SMT_TransactionStruct_t SMT_RAM * SMT_GetMasterTransaction(SMT_TransactionStruct_t SMT_RAM * pLink);

#endif
