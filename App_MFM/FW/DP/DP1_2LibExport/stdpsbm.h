/************************************************************************************************************

                                  COPYRIGHT (C) STMicroelectronics 2004.

	All rights reserved. This document contains proprietary and confidential information of the STMicro-
	electronics Group. This document is not to be copied in whole or part. STMicroelectronics assumes no
	responsibility for the consequences of use of such information nor for any infringement	of patents
	or other rights of third parties which may result from its use. No license is granted by implication
	or otherwise under any patent or patent rights of STMicroelectronics. STMicroelectronics products are
	not authorized for use as critical components in life support devices or systems without express writ-
	ten approval of STMicroelectronics.

File name 		:  
Author 			: 
Description		: 
Creation 		: 
*************************************************************************************************************/

#ifndef __STDPSBM_H__
#define __STDPSBM_H__
//******************************************************************************
//	C O M P I L E R    O P T I O N S										
//******************************************************************************
// Force byte alignment
#ifdef __PARADIGM__
#pragma pack(push, 1)
#endif
#pragma option -b- //as a byte

//******************************************************************************
//  I N C L U D E   F I L E S
//******************************************************************************

//******************************************************************************
//  C O N S T A N T   D E F I N E S
//******************************************************************************
#define MIN_SBM_HDR_LEN				4
#define MAX_SBM_HDR_LEN					6
#define	SBMSG_SPLITBUFF_NUM				8
#define   SB_DPCD_BUFF_SIZE                        512
#define	SBMSG_SPLITBUFF_SIZE			(SB_DPCD_BUFF_SIZE / SBMSG_SPLITBUFF_NUM)
#define SBRX_DOWNREQ_BUFFNUM 2
#define SBTX_UPREQ_BUFFNUM 2
#define SBTX_UPREP_BUFFNUM 2
#define SBTX_DNREP_BUFFNUM 2
#define SBTX_DNREQ_BUFFNUM 2

#define HDR_SIZE_BROADCAST 3
#define SBM_INVALID_BUFFOFFSET		0xFFFF
#define NO_RAD_SB_HEADER_SIZE		3
//******************************************************************************
//  M A C R O   D E F I N E S
//******************************************************************************

#define CALC_BITS_2_BYTES(a)			(( a % 8) != 0 ? ((a / 8) +1) : (a / 8))
#define GUID_SIZE_IN_BITS				128
#define GUID_SIZE_BYTES					CALC_BITS_2_BYTES(GUID_SIZE_IN_BITS)
#define SBM_START_BIT				((U8)0x80)
#define SBM_END_BIT					((U8)0x40)

#define DPCD_DOWN_REQ		0x1000
#define DPCD_UP_REP			0x1200
#define DPCD_DOWN_REP		0x1400
#define DPCD_UP_REQ		0x1600

#define DEVICE_SERVICE_IRQ_VECTOR_ESI0	 		0x2003

#define DPRX_DPCD_DOWN_REP_MSG_RDY				DP_BIT4
#define DPRX_DPCD_UP_REQ_MSG_RDY					DP_BIT5
//******************************************************************************
//  M A C R O   D E F I N E S
//******************************************************************************

#define ABS(a,b) ((a>b)?(a-b):(b-a))

//******************************************************************************
//	G L O B A L    V A R I A B L E S
//******************************************************************************

//******************************************************************************
//  T Y P E D E F S 
//******************************************************************************

//******************************************************************************
//  T Y P E D E F S   ( E N U M S )
//******************************************************************************

typedef enum
{
	SBM_NONE,					// Request originated from top/source, would be DOWN_REQ, then associated would be DOWN_REP (from sink)
								// Request originated from down/SINK, would be UP_REQ, then associated with UP_REP (from top)
	SBM_DOWNREQ,				// R/W,	request to downstream
	SBM_UPREQ,					// R only,	request from downstream 
	SBM_DOWNREP,				// R only, reply from downstream
	SBM_UPREP,					// R/W,	reply to downstream
	SBM_DOWNREQ_ORIGINATOR,		// R/W,	reply to downstream
	SBM_ParseType_SendToSource	// Send the reply or request to the PC source tool
}SBM_DPCDBUFF_TYPES;



typedef enum
{
	SB_ParseType_NONE,
	SB_ParseType_EXEC,
	SB_ParseType_FWD,
}SB_ParseType_t;
//******************************************************************************
//  T Y P E D E F S   ( F U N C T I O N   P O I N T E R S )
//******************************************************************************

//******************************************************************************
//  T Y P E D E F S   ( S T R U C T S )
//******************************************************************************
typedef struct
{
	U16 MTBuffPtr;
	U8 MTMsgLen;

}MT_MsgParsing;

typedef struct
{
	U8 SB2MT_TxedPortNum		: 4;
	U8 SB2MT_RxedPortNum		: 4;
	U8 MT2SB_MsgLen				: 6;
	U8 MT2SB_FwdMsg				: 1;
	U8 MT2SB_StartOfMsg			: 1;
	U8 SB2MT_MsgLen				: 6;
	U8 SB_Msn					: 1;
	U8 NodeMsg              	: 1;  
	U8 MT2SB_unused				: 4;
	U8 SB2MT_TargetNode			: 1;
	U8 SB2MT_Broadcast          : 1;
	U8 MT2SB_BuffFilled    		: 1;
	U8 MT2SB_MsgExecDone		: 1;
	U32 SB2MT_NextLongMessage;

}MT_ReplyInfo_t;


typedef struct
{
	U8 MT2SB_BuffFilled    		:1;
	U8 MT2SB_FwdMsg			:1;
	U8 SB_ReqIdentifier		:7;	
	U8 InpPortNum			:4;
}MT_RequestInfo_t;


typedef struct
{
	U8 LinkCountTotal			:4;
	U8 LinkCountRemaining		:4;	
	U32 Rad						;
	U8 SB_Broadcast				:1;
	U8 SB_Path					:1;
	U8 SB_MSG_Body_Length		:6;
	U8 StartOfMsg				:1;
	U8 EndOfMsg					:1;
	U8 Msn						:1;
}SB_RequestHeaderInfo_t;

typedef struct
{
	SB_RequestHeaderInfo_t SB_RequestHeaderInfo_s;
	MT_RequestInfo_t		MT_RequestInfo_s;	

}SB_RequestInfo_t;


typedef struct	SB_Msg
{
	U8 volatile		Data[MAX_SBM_SIZE];					
	U8 volatile   	SB_Broadcast				: 1;
	U8 volatile		SB_Path						: 1;
	U8 volatile		MT_DataLen					: 6;
 	U8 volatile		TrackFwdedTxPorts			  ;
	U8 volatile		SB_ReqRep_Type				: 3;
	U8 volatile		MT_Data_Start				: 3;
	U8 volatile		TargetNode					: 1;
	U8 volatile		Zeros						: 1;

	U8 volatile		TrackFwdedRxPorts			  ;
	U8 volatile		SB_RxedPortNum				: 4;
	U8 volatile		SB_TxedPortNum				: 4;
	U8 volatile		Unused						  ;
	U8 volatile		SB_Msn						: 1;
	U8 volatile		SB_AgingTmr					: 7;
	U32 volatile	SB_NxtLngMsgAdr			      ;
	U32 volatile	SB_NxtMsgAdr				  ;
	
}SB_MsgBuffer_t;





//******************************************************************************
//  V A R I A B L E   E X T E R N S
//******************************************************************************

//******************************************************************************
//  F U N C T I O N   E X T E R N S
//******************************************************************************

//******************************************************************************
//  F U N C T I O N   P R O T O T Y P E S
//******************************************************************************
void SideBandInit(void);

void DP_EXT_FUNC SBM_Isr(void);
void SB_InterruptDisable (void);
void SB_InterruptEnable(void);
void SB_GetDownReqMsg(void);
void SB_SetDownRepMsg(void);
void RXSBM_ReAdjust2NextReqBuff(void);
void RXSBM_ReAdjust2NextReplyBuff(void);
void SBM_ConstructDownReplyMsg(U8 MTCmd, U8 FAR *DownReplyBuffPtr, U16 MsgReplyLen);
void DP_EXT_FUNC RXSBM_SendDownReplyMsgs(void);
void RXSBM_SendUpReqMsgs(void);
void SB_UpReqHandler(void);
#if (DP_HUB == ENABLED)
void SB_LogiacalPortUpReqHandler(void);
#endif

void RXSBM_ReAdjust2NextBuff(void);
void SBM_ConstructReplyMsgs(SB_MsgBuffer_t FAR*RxedMsgsList, SB_MsgBuffer_t FAR * FreeRplyBuffPtr,MT_ReplyInfo_t *MT_ReplyInfo_ps);

SB_MsgBuffer_t FAR* Tx_Sb_GetFreeBuffer  (SBM_DPCDBUFF_TYPES BufferType);
void  DP_EXT_FUNC Tx_Sb_SendUpReplyMsgs(void);
void DP_EXT_FUNC Tx_Sb_SendDnReqMsgs(void);
void DP_EXT_FUNC SB_TxReceiveMsgs(void);
SB_MsgBuffer_t FAR** Rx_Sb_GetFreeBuffer  (SBM_DPCDBUFF_TYPES BufferType);
SB_MsgBuffer_t FAR** Rx_Sb_GetSendBuffer  (SBM_DPCDBUFF_TYPES BufferType);


void  SBM_ConstructUpReplyMsgs(SB_MsgBuffer_t FAR*RxedMsgsList, SB_MsgBuffer_t FAR * FreeRplyBuffPtr,MT_ReplyInfo_t *MT_Rep_ps);
#pragma option -b.
#ifdef __PARADIGM__
#pragma pack(pop)
#endif
#endif	/*__STDPSBM_H__*/


