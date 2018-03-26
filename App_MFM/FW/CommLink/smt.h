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
// MODULE:      smt.h
//
// *****************************************************************

#ifndef __SMT_H__
#define __SMT_H__

#include "System\all.h"
#include "Stdlibinc\gm_Register.h"

#define I2C_SLAVE_ADDR				0x6E	/*slave address used in smt requests */
#define UART_SEPARATE_BUFFER 		0		/*use a separate buffer for uart and i2c*/
#define SMT_IGNORE_EVENODD 			0 // 1		/*ignore the even/odd feature of smt */
#define SEND_BUSY_RESPONSE 			0		/*whether or not to send a busy response to host, if required*/
#define SMT_TIMEOUT					1		/*use smt timeout of transactions*/
#define SMT_SLAVE_RESPONSE_BIT		BIT3
#define SMT_TRANSID_NOT_ASSIGNED	0xFF

#pragma pack(push)
#pragma pack(1)

typedef struct SMT_MsgPktStruct     // SMT messages struct
{
   BYTE  len;
   BYTE  cmd;
   BYTE  msgType;
   BYTE  control;
   BYTE  msgpkt[];
} MsgPacket_t;

#pragma pack(pop)

typedef BYTE (far * FPT_SMT_MsgHandler)(MsgPacket_t *message, BYTE comm_use);

#include "CommLink/smt_Ports.h"

typedef struct gmt_MsgStruct_s
{
   BYTE  len; // length of this command packet
   BYTE  cmd;
   BYTE BUF[254];
} gmt_MsgStruct;


//*************************************************************************
//	Defines from Streaming Message Transport protocol document
//*************************************************************************
#define  CM_TM_BIT			BIT0	// Control/Transport message flag
#define  EVEN_ODD_BIT		BIT1	// Even/Odd message count
#define  MORE_DATA_BIT		BIT2	// More Data Pending flag for Transport message
#ifndef  MAX_TRANSACTION_NUMBER
#define  MAX_TRANSACTION_NUMBER  1
#endif

#ifndef MAX_SMT_MASTER_TRANSACTIONS
#define MAX_SMT_MASTER_TRANSACTIONS 1
#endif
#ifdef ST_4K2K_93xx_BOARD
#define  SMT_TRANSACTION_TIMEOUT 4000       //2000 // in milliseconds
#else
#define  SMT_TRANSACTION_TIMEOUT 2000 // in milliseconds
#endif
#define	 SMT_HEADER_SIZE		4
#define	CHECKSUM_SIZE		1

// Low-level SMT protocol commands and responses
#define  OPEN_TRANSACTION_CMD		0
#define  TRANSACTION_OPENED		1
#define  CLOSE_TRANSACTION_CMD		2
#define  TRANSACTION_CLOSED 		3
#define  RE_TRANSMIT_CMD			4
#define  FAILED						5
#define  BUSY_PROCESSING			6
#define  GET_STATUS_CMD			7

#define SMT_RAM far // set SMT VAR to DDR if define this to far or define " " for normal iram.

//******************************************************************************
//  T Y P E    D E F I N I T I O N S
//******************************************************************************

// Hi-level protocol commands for example
typedef enum HiLevelSMTCommandEnum
{
   IDLE_CMD = 10,
   RAM_WRITE_CMD,
   RAM_READ_CMD,
   REG_READ_CMD,
   REG_WRITE_CMD,
   FWD_I2C_WRITE = 50,
   FWD_I2C_READ,
   FWD_I2C_WRITE_READ,
   FWD_DDC2BI_READ,
   FWD_SERIAL_WRITE,
   FWD_SERIAL_READ,
} HiLevelSMTCommandType;

typedef enum ErrorsOfSMTEnum
{
   GENERIC_ER,
   TRANS_ID_ER,
   CHECK_SUM_ER,
   CORRUPT_MSG_ER
} ErrorsOfSMTType;

#pragma pack(push)
#pragma pack(1)


typedef struct SMT_TransactionStruct   // SMT Transactions Struct
{

   BYTE  B_Status;
   BYTE  B_CurCmd;
   BYTE  B_TmrNum;
   BYTE  EvenOdd;

   BYTE  ID;
   WORD  Port;
   BYTE HWPort;
   WORD  BuffSize;
	BYTE SMT_RAM *Bp_Buff;
   WORD  W_Count;

	//pSMT_TransactionStruct  TransactionLink;		/* master's use to link slave transaction*/
	BYTE  SMT_RAM *TransactionLink;		/* master's use to link slave transaction*/
   /* slave's use to link to master transaction*/

   BOOL MoreDataPending;		/* used to indicate that the incoming data packet requires more data*/
   HANDLER_USE_t comm_use;
	BYTE SMT_RAM *Bp_RxBuff;
   BYTE  Ba_Buff[SIO_RxBufLen * 3]; // * 3 For collect all the data at one buffer to program
   BYTE Ba_RxBuff[SIO_RxBufLen * 3]; // * 3 For collect all the data at one buffer to program
}SMT_TransactionStruct_t;

typedef enum
{
   TS_OK,
   TS_NOT_SUPPORTED,
   TS_ERR = TS_NOT_SUPPORTED
}TSerr_t;


#pragma pack(pop)
//******************************************************************************
//  G L O B A L    F U N C T I O N S
//******************************************************************************
extern void SMT_SendStatusSMT(BYTE msgpkt[], BYTE comm_use, BYTE EvenOdd);
extern void SMT_SendFailed(BYTE comm_use, BYTE EvenOdd, BYTE B_Reason);
extern void SMT_SendReTransmit(BYTE TransID, BYTE comm_use, BYTE EvenOdd, WORD W_Delay_ms);

extern BYTE SMT_MessageHandler(MsgPacket_t * message, BYTE comm_use);
extern BYTE SMT_GetBuffer(BYTE  B_TransactionID,  BYTE far * far * Bp_Buff, WORD W_Size);
extern BYTE GetBuffer(BYTE far * far * Bp_Buff, WORD W_Size);



extern void MoreData(BOOL NoMoreData);
extern BOOL MoreDataExpected(void);
extern WORD GetMaxDataLength (void);
extern BYTE * GetOutMsgLenPtr(void);
extern BYTE * GetOutMsgBufPtr(void);
extern WORD GetMesageLength (void);
extern gmt_RET_PASSFAIL SMT_TransactionInit(void);
extern gmt_RET_PASSFAIL SMT_ResetTransaction(SMT_TransactionStruct_t SMT_RAM * pTransaction, BYTE SingleOnly);

// Support SMT
extern SMT_TransactionStruct_t SMT_RAM SMT_TransArray[MAX_TRANSACTION_NUMBER];
extern gmt_MsgStruct  OutMsg;

BYTE SMT_GetTransID(MsgPacket_t * message);
SMT_TransactionStruct_t SMT_RAM * IsTransIDValid(BYTE TransID);
void SMT_SendBusy(BYTE TransID, BYTE comm_use, BYTE EvenOdd, WORD W_Delay_ms);

//#if DEBUG_SMT
//extern BYTE SMTDebug[];
//extern BYTE SMTCnt;
//extern BYTE SMTCNT(void);
//
// MACROs
//
//#define SMTINIT 		(SMTCnt=0)
//#define SMTDEBUG 		(SMTDebug[SMTCNT()])
//#define SMTCHECKSUM(a) (SMTDebug[SMTCNT()] = a)
//#endif
/*
#if USE_ENHANCED_ACC
extern BYTE EACCBuffer[];
extern BYTE B_NumOfTemplate;
extern BYTE B_NumOfBin;
extern BYTE B_NumOfPoints;
extern void InitializeRealTimeACC16(void);
#endif

//*************************************************************************
// Important.
// This enum must be the same for all series of chips to keep consistency
//*************************************************************************
typedef enum SMT_Port
{
	SMTPORT_ACC16 = 0,	// ACC 16 dedicated SMT Port ID
	SMTPORT_ACM3 = 1,	// ACM3 dedicated SMT Port ID
	SMTPORT_ACC5 = 2,
	SMTPORT_MATRIX_3x3 = 6,
	SMTPORT_COLOR_CONTROL = 7,
	SMTPORT_MSG = 8,	//generic message port id
	SMTPORT_GAMMA = 9,
	SMTPORT_FILTERS = 10, // TODO: SMT protocol and Filter protocol recommend different port ID for this feature. Check and FIx
	MAX_SMTPORT
} SMT_Port_t;

typedef BYTE (far * FPT_SMT_MsgHandler)(MsgPacket_t *message, BYTE comm_use);

typedef struct SMT_PortMapping
{
	SMT_Port_t	PortID;
	FPT_SMT_MsgHandler Fp_SMT_MsgHandler;
} SMT_PortMapping_t;

// PortID <-> Handler mapping
extern SMT_PortMapping_t ROM SMT_Port2HandlerTable[];

// */
// Return codes or memory buffer allocator
//
enum
{
   FIRST_REQUEST = 0,
   NEXT_REQUEST = 1,
   NO_MEMORY = 0xFF,
};

typedef enum
{
   PORT_MSG_HNDLR_OK,
   PORT_MSG_HNDLR_ERROR,
   PORT_MSG_HNDLR_RETRANSMIT,
   PORT_MSG_HNDLR_BUSY,
   PORT_MSG_HNDLR_INVALID_PARAM,
   PORT_MSG_HNDLR_SELFCARED
}PortMsgHndlrReplyType;


#ifdef ST_4K2K_93xx_BOARD
PortMsgHndlrReplyType SMT_ForwardMessageToSlave(MsgPacket_t *message, BYTE Addr);
//void SMT_Debug_Print(char far * str);
#endif

#endif

