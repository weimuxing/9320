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
#pragma pack(push)
#pragma pack(1)


typedef struct SMT_RegReadStruct
{
   BYTE  Cmd;
   WORD W_Addr;
} SMT_RegRead_t;

typedef struct SMT_RegWriteStruct
{
   BYTE  Cmd;
   WORD W_Addr;
   WORD W_Data;
} SMT_RegWrite_t;

typedef struct SMT_MsgFwdStruct
{
   BYTE Cmd;
   BYTE HWPort;			//hardwire port
   BYTE SMT_Port_hi;	//smt port handler hi
   BYTE SMT_Port_lo;	//smt port handler lo
   BYTE Payload[];
} SMT_MsgFwd_t;



#pragma pack(pop)


BYTE SMT_MsgFwdHandler(MsgPacket_t *message, BYTE comm_use);
gmt_RET_PASSFAIL SMT_CloseLinkedTransaction(SMT_TransactionStruct_t SMT_RAM * pTransaction);
void SMT_ForwardResponse(MsgPacket_t * pBuffer, SMT_TransactionStruct_t SMT_RAM *  pTransaction);

#endif
