/********************************************************************
COPYRIGHT (C) STMicroelectronics 2004.
All rights reserved. This document contains proprietary and
confidential information of the STMicroelectronics Group. This
document is not to be copied in whole or part. STMicroelectronics
assumes no responsibility for the consequences of use of such
information nor for any infringement of patents or other rights of
third parties which may result from its use. No license is granted by
implication or otherwise under any patent or patent rights of
STMicroelectronics.
STMicroelectronics
products are not authorized for use as critical components in life
support devices or systems without express written approval of
STMicroelectronics.
File name : LL.h
Author :
Description :
Creation :
********************************************************************/
#ifndef __DP12INIT_H__
#define __DP12INIT_H__
//******************************************************************************
//	C O M P I L E R    O P T I O N S										
//******************************************************************************
// Force byte alignment
#ifdef __PARADIGM__
#pragma pack(push, 1)
#endif
#pragma option -b- //as a byte

//******************************************************************************
//	M A C R O S    
//******************************************************************************
#if (DP_HUB == ENABLED)
#define LOGICAL_PORT 			2
#else
#define LOGICAL_PORT 			8
#endif

#define KSV_SIZE				5
#define HDCP_MAX_DEVICES		MAX_NUM_HDCP_KSVS

#define KSV_LIST_SIZE			(HDCP_MAX_DEVICES * KSV_SIZE)

//******************************************************************************
//	G L O B A L    D E F I N I T I O N S
//******************************************************************************

typedef enum
{
	APSTATE_NONE,
	APSTATE_ASSIGN_RXTXID,
	APSTATE_TXBE_ALLOCATE,
	APSTATE_CLEARRX_TX_ID,
	APSTATE_TXBE_CLEAR_PLID,
	APSTATE_NACK,
}ALLOCATE_PATH_STATES_t;

typedef struct
{
	U16  						APTimer;
	U8							APNoErrONTx;
	U8							UsedTxPayLoadId_16;
	U16 							MappedTxId;
	ALLOCATE_PATH_STATES_t		APPathState_e;
	ALLOCATE_PATH_STATES_t		APUpdatePathState_e;
	ALLOCATE_PATH_STATES_t         APClearPayLoadTableState_e;
	U16 							AP_TxRxUpdateTimer;
	U16 							AP_TxRxUpdateReplyTimer;
	U16 							AP_TxRxClearPayLoadTimer;
	U8 							NumMstTxPorts;
	U8 							ClearPayloadInProcess:1;
}AP_BUParam_t;

typedef enum
{
	PeerDeviceType_NODEVICE		= 0,
	PeerDeviceType_SOURCE		= 1,
	PeerDeviceType_BRANCH		= 2,
	PeerDeviceType_SSTSINK		= 3,
	PeerDeviceType_DP2LEGACY	= 4
}PeerDeviceType;


typedef struct
{
	U8					        NumberUpStreamPorts;
	U8					        NumberDownStreamPhysicalPorts;
	U8							NumberDownStreamLogicalPorts;
	DP_BaseAddr_t				BridgeTopRegMemOffset_p;
	DP_BaseAddr_t				RoutingLogicTopRegMemOffset_p;
	BOOL						LeftAndRightPanel;

	SB_MsgBuffer_t FAR *		SB_DnReqRxedLL;
	SB_MsgBuffer_t FAR *		SB_DnReqRxedLngLL;
	
	SB_MsgBuffer_t FAR *		SB_DnReqExecLL;
	SB_MsgBuffer_t FAR *		SB_DnReqExecLngMgsLL;	
	SB_MsgBuffer_t FAR *		SB_DnReplyRxedLL;
   	SB_MsgBuffer_t FAR *		SB_DnReplyExecLL;
	SB_MsgBuffer_t FAR *		SB_FwdLL;	

#if DP_MULTISTREAM_SOURCE
	SB_MsgBuffer_t FAR *		SB_DnRepSendToSourceLL;
	SB_MsgBuffer_t FAR *		SB_UpReqSendToSourceLL;
#endif

	SB_MsgBuffer_t FAR *		SB_UpReqRxedLL;
	SB_MsgBuffer_t FAR *		SB_UpReqExecLL;

	SB_MsgBuffer_t FAR *		SB_UpRepRxedLL;
	SB_MsgBuffer_t FAR *		SB_UpRepExecLL;	
	AP_BUParam_t				AP_BuParam;	
	U8 							TrackMstOutTxPorts;	
	U8 							TrackMstOutRxPorts;
} DPRL_Params_t;

extern DPRL_Params_t *			DPRL_RlParam_ps;
//******************************************************************************
//	G L O B A L    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
void DP12ModulesInitialization(void);
void EXT_FUNC BU_Sys_Handler(void);

PortStructure_t * DP_EXT_FUNC DP_GetPortList (void);
PortStructure_t * DP_EXT_FUNC SetRlUpStreamPortInstance (PortStructure_t * PortStructure_ps);
PortStructure_t * DP_EXT_FUNC SetRlDownStreamLogicalPortInstance (PortStructure_t * PortStructure_ps);
PortStructure_t * DP_EXT_FUNC SetRlDownStreamPhysicalPortInstance (PortStructure_t * PortStructure_ps);

void DP_INT_FUNC BU_RxHandleSidebandPayloadOnTxPortSwitch (void);
void DP_EXT_FUNC DP12_InitRoutingLogic(void);
void DP_EXT_FUNC DP12_InitSideband (void);

void DP_EXT_FUNC Util_RlTxPatGenSelect(U8 PatGenId_8);
void DP_EXT_FUNC Util_RlProgPatgenTiming(U8 PatGenId_8, DP_RlPatgenParam_t *DP_RlPatgenParam_ps);
void DP_EXT_FUNC Util_RlTxPatGenEnable(U8 PatGenId_8, BOOL IfEnable_b);
void DP_EXT_FUNC Util_RlTxPatGenBpp(U8 PatGenId_8, U8 Bpp_8);
void DP_EXT_FUNC Util_RlTxPatGenSelPat(U8 PatGenId_8, U8 PatSel_8);
void DP_EXT_FUNC Util_RlTxPatGenClkSrc(U8 PatGenId_8, U8 ClkSel_8);
void DP_EXT_FUNC Util_RlTxPatGenComp(U8 PatGenId_8, U32 Comp_8);
		
#pragma option -b.
#ifdef __PARADIGM__
#pragma pack(pop)
#endif
#endif	/*__STDPBUAPI_H__*/
