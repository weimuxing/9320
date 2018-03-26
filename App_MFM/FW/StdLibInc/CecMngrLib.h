#ifndef __GM_CEC_MNGR_H__
#define __GM_CEC_MNGR_H__

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
// MODULE:      CecMngrLib.h
//
// USAGE:       Header file for CEC manager library
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

//******************************************************************************
//  T Y P E    D E F I N I T I O N S
//******************************************************************************
/*
Status of the message posted to transmission
*/
typedef enum
{
   // Message hasen't any status. Init state.
   gmd_CEC_MNGR_TX_STS_NONE,
   //Message is wating to be transmitted. Transition state
   gmd_CEC_MNGR_TX_STS_WAITING_FOR_TRANSMISSION,
   //Message is being transmitted. Transition state
   gmd_CEC_MNGR_TX_STS_TRANSMITTING,
   //Message has been transmitted and acknowledged. Transition state
   gmd_CEC_MNGR_TX_STS_ACKNOWLEDGED,
   //Message has been transmitted but not acknowledged. Terminal state
   gmd_CEC_MNGR_TX_STS_NOT_ACKNOWLEDGED,
   //Message transmission terminated with error. Terminal state
   gmd_CEC_MNGR_TX_STS_ERROR,
   //Message is considered and acted on by follower. 1 sec after it has been acknowledged.
   //Terminal state
   gmd_CEC_MNGR_TX_STS_ACTED_ON,
   //Message has been aborted. <Feature Abort> returned for this message.
   //Terminal state
   gmd_CEC_MNGR_TX_STS_ABORTED,
   //Query message has been replied. Parameters of reply are avalible at Bp_RespResult.
   //Terminal state
   gmd_CEC_MNGR_TX_STS_REPLIED,
   //Query message has not been replied. No response within 1 sec
   //Terminal state
   gmd_CEC_MNGR_TX_STS_NOT_REPLIED,
} gmt_CEC_MNGR_MESSAGE_STATUS;

#ifdef __PARADIGM__
#pragma pack(push)
#pragma pack(1)
#endif

typedef struct
{
   BYTE B_DestLogAddr;
   BYTE B_OpCode; //Init value should be 0xFF, <Abort>, indicating element is free for use
   DWORD DW_TxTime; //Time at which message was transmitted, ms
   gmt_CEC_MNGR_MESSAGE_STATUS B_Status; // Being transmitted, Transmitted, Error, Acknoleged, Acted on/Aborted, Replied/Timed out
   WORD W_RespActTime; //Time after expiration of which a message considered as not acted on or not responded, sec, 1 - default value, 0 - time is not monitored
   BYTE B_RespOpCode; //OpCode that is expected as response, if no response is expected then 0xFF
   BYTE* Bp_RespResult; //Pointer to the buffer where the data of response message shall be stored to; the buffer is allocated by caller
   BYTE B_RespLen; // Length of coming frame
} gmt_CEC_MNGR_TX_MESSAGE_HANDLE;

#ifdef __PARADIGM__
#pragma pack(pop)
#endif

//******************************************************************************
//  G L O B A L    F U N C T I O N S
//******************************************************************************
BYTE gm_CecMngrTxMessage(BYTE B_DestLogAddr, BYTE B_DataLen, BYTE CEC_RAM * Bp_Data, BYTE B_LogFlag,
                         WORD W_RespActTime);

BYTE gm_CecMngrTxQueryMessage(BYTE B_DestLogAddr, BYTE B_DataLen, BYTE CEC_RAM * Bp_Data,
                              WORD W_RespActTime, BYTE B_RespOpCode, BYTE CEC_RAM * Bp_RespResult, BYTE B_RespLen);

BOOL gm_CecMngrRxMessage(BYTE CEC_RAM * Bp_SrcLogAddr, BYTE CEC_RAM * Bp_DestLogAddr,
                         BYTE CEC_RAM * Bp_DataLen, BYTE CEC_RAM * Bp_Data);

gmt_CEC_MNGR_MESSAGE_STATUS 	gm_CecMngrGetTxMessageStatus(BYTE B_MsgId);
gmt_CEC_MNGR_TX_MESSAGE_HANDLE 	gm_CecMngrGetTxMessageHandle(BYTE B_MsgId);

void gm_CecMngrRxHandler(void);
void gm_CecMngrTxHandler(void);

void gm_CecMngrTxManagerReset(void);

BOOL gm_CecMngrIsTerminalState(gmt_CEC_MNGR_MESSAGE_STATUS B_Status);


//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************

//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************

//******************************************************************************
//  L O C A L    F U N C T I O N S
//******************************************************************************

//******************************************************************************
//  L O C A L    V A R I A B L E S
//******************************************************************************

//******************************************************************************
//  M A C R O   D E F I N I T I O N
//******************************************************************************
#define	NULL_MSG_ID 0

#ifdef NULL_MSG_ID
#define INVALID_MSG_ID	0xFF
#else
#define INVALID_MSG_ID	0
#endif

#endif //#ifndef __GM_CEC_MNGR_H__
//*********************************  END  **************************************