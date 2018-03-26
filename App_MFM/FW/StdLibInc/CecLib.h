/*
   $Workfile:   CecLib.h  $
   $Revision: 1.4 $
   $Date: 2012/04/13 09:11:14 $
*/

//#include "Inc\CecMngrLib.h"

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
//******************************************************************************
//
//  MODULE: CecLib.h
//
//
//  USAGE : Contains declarations for Cec Library
//
//******************************************************************************
#ifndef __CEC_LIB_H__
#define __CEC_LIB_H__

//******************************************************************************
//  G L O B A L  T Y P E S  D E F I N I T I O N S
//******************************************************************************
#ifdef __PARADIGM__
#pragma pack(push)
#pragma pack(1)
#endif


//******************************************************************************
//  U T I L I T Y   ( A P P   L E V E L )   T Y P E S
//******************************************************************************
typedef enum
{
   gmd_SC_OK                                   = 0x00,
   gmd_SC_ERROR_LEN                            = 0x01,
   gmd_SC_ERROR_DIR                            = 0x02,
   gmd_SC_ERROR_OP_CODE                        = 0x04,
   gmd_SC_ERROR_SOURCE                         = 0x08,
   gmd_SC_ERROR_MASK                           = 0x0F,
   gmd_SC_ERROR_ABORT                          = 0x80
}gmt_CEC_MSG_VALIDITY_CHECK_RESULT;

//******************************************************************************
//  M A N A G E R   T Y P E S
//******************************************************************************
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
//  D R I V E R   T Y P E S
//******************************************************************************
typedef	struct
{
   WORD W_CecRegBaseAddress;
   BYTE B_NumberOfRetransmissions;         // Number of retransmissions
   BYTE B_SizeOfRxBuff;                    // size of Rx buffer
   BYTE CEC_RAM * Bp_CecRxBufferAddress;   // Pointer to CEC Rx buffer
   DWORD DW_TClkHz;                        // clock
} gmt_CEC_INIT;

typedef struct
{
   BYTE B_CecStdMajor;
   BYTE B_CecStdMinor;

   BYTE B_CecIpVersionMajor;
   BYTE B_CecIpVersionMinor;
   BYTE B_CecIpRevision;

   WORD W_LibReleaseNumber;
   WORD W_LibBuildNumber;

}gmt_CEC_LIB_VERSION;

#ifdef __PARADIGM__
#pragma pack(pop)
#endif

typedef enum
{
   gmd_CEC_FRAME_RX_DONE       =   BIT0,   // CEC Rx buffer contains at least one frame that is ready for reading
   gmd_CEC_FRAME_RX_FULL       =   BIT1,   // CEC Rx buffer is full and receiving is stopped.
   gmd_CEC_FRAME_TX_BUSY       =   BIT2,   // CEC line was busy at the moment Tx driver wants to start transmission
   gmd_CEC_FRAME_TX_ERROR      =   BIT3,   // The last CEC frame transmission was unsuccessful
   gmd_CEC_FRAME_TX_ARBITER    =   BIT4,   // another device capture the line during transmission of initiator address
   gmd_CEC_FRAME_TX_ACK        =   BIT5,   // The last CEC frame transmission has been acknowledged
   gmd_CEC_FRAME_TX_DONE       =   BIT6    // The last CEC frame transmission has been completed
} gmt_CEC_FRAME_STATUS;

typedef enum
{
   gmd_CEC_BUS_FREE            =   BIT0,   // CEC bus is free and ready for use
   gmd_CEC_BUS_RX_BUSY         =   BIT1,   // CEC frame is being received
   gmd_CEC_BUS_TX_BUSY         =   BIT2,   // CEC frame is being transmitted
   gmd_CEC_BUS_RX_NACK         =   BIT3,   // Respond with NACK for directly addressed message and ACK for broadcast
   gmd_CEC_BUS_ERROR           =   BIT4,   // CEC Line error
   gmd_CEC_BUS_INACTIVE        =   BIT5,   // CEC bus is inactive during Free Time
}gmt_CEC_BUS_STATUS;

//******************************************************************************
//  M A C R O   D E F I N I T I O N
//******************************************************************************
//******************************************************************************
//  M A N A G E R   M A C R O   D E F I N I T I O N S
//******************************************************************************
#define	NULL_MSG_ID 0

#ifdef NULL_MSG_ID
#define INVALID_MSG_ID	0xFF
#else
#define INVALID_MSG_ID	0
#endif

//******************************************************************************
//  G L O B A L  M A C R O S  D E F I N I T I O N S
//******************************************************************************


//******************************************************************************
//  G L O B A L    F U N C T I O N S '   P R O T O T Y P E S
//******************************************************************************
//******************************************************************************
//  U T I L I T Y ' S ( A P P   L E V E L )   P R O T O T Y P E S
//******************************************************************************
gmt_CEC_MSG_VALIDITY_CHECK_RESULT far gm_CecUtilMessageValidityCheck(BYTE B_Code, BYTE B_Length, BOOL Directed);
BYTE far gm_CecUtilExtractDigitFromPhysAddr(WORD PhysAddr, BYTE Digit);
BOOL far gm_CecUtilIsMessageResponsed(gmt_CEC_MNGR_MESSAGE_STATUS fs);

//******************************************************************************
//  M A N A G E R ' S   P R O T O T Y P E S
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
//  D R I V E R ' S   P R O T O T Y P E S
//******************************************************************************
void    CEC_EXT_FUNC gm_CecInit(gmt_CEC_INIT CEC_RAM * Sp_CECInitValuesPtr);

BOOL    CEC_EXT_FUNC gm_CecTxFrame(BYTE B_DestLogAddr, BYTE B_DataLen, BYTE CEC_RAM * Bp_Data);
BOOL    CEC_EXT_FUNC gm_CecRxFrame(BYTE CEC_RAM * Bp_SrcLogAddr, BYTE CEC_RAM * Bp_DestLogAddr, BYTE CEC_RAM * Bp_DataLen, BYTE CEC_RAM * Bp_Data);

void    CEC_EXT_FUNC gm_CecIsr(void);

BYTE    CEC_EXT_FUNC gm_CecGetFrameStatus(void);
void    CEC_EXT_FUNC gm_CecClearFrameStatusBits(BYTE W_CECFrameStatusMask);
void    CEC_EXT_FUNC gm_CecSetAckControl(BOOL B_AckCtrl);
BOOL    CEC_EXT_FUNC gm_CecGetAckControl(void);
void    CEC_EXT_FUNC gm_CecSetLogAddr(BYTE B_LogAddr);
BYTE    CEC_EXT_FUNC gm_CecGetLogAddr(void);
DWORD   CEC_EXT_FUNC gm_CecGetTxTime(void);
BYTE    CEC_EXT_FUNC gm_CecGetBusStatus(void);
void    CEC_EXT_FUNC gm_CecEnable(void);
void    CEC_EXT_FUNC gm_CecDisable(void);
void    CEC_EXT_FUNC gm_CecClearRxBuffer(void);

gmt_CEC_LIB_VERSION CEC_EXT_FUNC gm_CecGetLibVersion(void);

//******************************************************************************
//  E X T E R N A L   F U N C T I O N S
//******************************************************************************
//extern  DWORD    EXT_FUNC gm_ReadRegDWord(WORD W_RegAddr);
extern  DWORD    EXT_FUNC gm_GetSystemTime(void);
extern  DWORD    EXT_FUNC gm_SclCecGetSystemTime100us(void);
extern  void     EXT_FUNC gm_SclCecPrint(char EXT_ROM *Bp_String, WORD W_Value);
extern  WORD     EXT_FUNC (*gm_SclCecGetSystemTime100usRAM)(void);

//******************************************************************************
//  CEC  DRV D E B U G: this part is for debug version of library
//******************************************************************************
#ifdef __PARADIGM__
#pragma pack(push)
#pragma pack(1)
#endif

typedef	struct
{
   BYTE B_SizeOfLogBuff;                    // size of Log buffer
   WORD CEC_RAM * Wp_CecLogBufferAddress;   // Pointer to CEC Log buffer
} gmt_CEC_DBG_INIT;

#ifdef __PARADIGM__
#pragma pack(pop)
#endif

typedef enum
{
   gmd_CEC_DBG_FRAME_BUS        = 0,
   gmd_CEC_DBG_REG              = BIT14,
#pragma warn -sig    // BIT15 Causes "Conversion may lose significant digits" warning because we are using the sign bit
   gmd_CEC_DBG_DATA             = BIT15,
   gmd_CEC_DBG_TYPES_MASK       = BIT15 | BIT14,
#pragma warn +sig
   gmd_CEC_DBG_MAX_TYPES
}gmt_CEC_DBG_DATA_TYPES;


typedef enum
{
   gmd_CEC_DBG_WR_IDX           = BIT11,
   gmd_CEC_DBG_RD_IDX           = BIT12,
   gmd_CEC_DBG_RX_DATA          = BIT12 | BIT11,
   gmd_CEC_DBG_DATA_MASK        = BIT13 | BIT12 | BIT11
}gmt_CEC_DBG_DATA_SUBTYPES;

typedef enum
{
   gmd_CEC_DBG_NONE     = 0,
   gmd_CEC_DBG_INIT     = 1,
   gmd_CEC_DBG_DRV      = (1 << 1),
}gmt_CEC_DBG_MODULES;

void CEC_EXT_FUNC gm_CecDebugInit(gmt_CEC_DBG_INIT CEC_RAM * Sp_CecDbgInitValuesPtr );
BYTE CEC_EXT_FUNC gm_CecGetLogCount(void);
void CEC_EXT_FUNC gm_CecPrintLog(void);

#endif // __CEC_LIB_H__
