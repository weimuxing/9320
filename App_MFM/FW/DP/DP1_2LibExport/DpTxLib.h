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

#ifndef __DPTXLIB_H__
#define __DPTXLIB_H__
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

#define DP_AUX_MAX_BYTES_IN_REQUEST			16
#define DPTX_MAX_SDP_SIZE					28
#define MAX_CLIENTS							16	

#define DPTX_SDP_VENDOR_READY				DP_BIT1
#define DPTX_SDP_AVINFO_READY				DP_BIT2
#define DPTX_SDP_SRCPD_READY				DP_BIT3
#define DPTX_SDP_AUINFO_READY				DP_BIT4
#define DPTX_SDP_MPEGSRC_READY				DP_BIT5
#define DPTX_SDP_EXTENT_READY				DP_BIT6

#define DPTX_SDP_COUNT						6

#define MAX_NUM_HDCP_KSVS					15
#define HDCP_KSV_SIZE						5
//******************************************************************************
//  M A C R O   D E F I N E S
//******************************************************************************

//******************************************************************************
//  T Y P E D E F S 
//******************************************************************************

//******************************************************************************
//  T Y P E D E F S   ( E N U M S )
//******************************************************************************
/* This enum is used for bitwise operation for the micropacket TX member ConnectedRxCapabilities */
typedef enum
{
	ConnectedRxCapabilites_Connected	 		=DP_BIT0,
	ConnectedRxCapabilites_MSTCap		 		=DP_BIT1,
	ConnectedRxCapabilites_DPDownStrPresent 	=DP_BIT2,
 	ConnectedRxCapabilites_LinkSuccess	 		=DP_BIT3,
	ConnectedRxCapabilites_HDCPRepeater	 		=DP_BIT4,
	ConnectedRxCapabilites_HDCPStreamPresent 	=DP_BIT5,
	ConnectedRxCapabilites_HDCPCPCapable		=DP_BIT6,
	ConnectedRxCapabilites_HDCPAuthenticated	=DP_BIT7,
	ConnectedRxCapabilites_HDCPStarted			=DP_BIT8,
}ConnectedRxCapabilites_e;
typedef enum
{
	APTxState_None,
	APTxState_UpdateTxTimeSLotTbl,
	APTxState_UpdateRxPayload,
	APTxState_WaitForUpdateComplete,
	APTxState_WaitForActComplete
} AllocatePayloadState_t;


typedef enum
{
	DPTX_RxInSel_DpVersion11FromRx0,
	DPTX_RxInSel_DpVersion11FromRx1,
	DPTX_RxInSel_DpVersion11FromRx2,
	DPTX_RxInSel_DpVersion11FromRx3,
	DPTX_RxInSel_DpVersion12FromRx0,	
	DPTX_RxInSel_DpVersion12FromRx1,	
	DPTX_RxInSel_DpVersion12FromRx2,	
	DPTX_RxInSel_DpVersion12FromRx3,	

}DPTX_RxInSel_t;

typedef enum
{
	DPTX_AuxReplyStatus_Ack = 0,
    DPTX_AuxReplyStatus_Nack = 1,
    DPTX_AuxReplyStatus_Defer = 2,
    DPTX_AuxReplyStatus_I2CNack = 4,
    DPTX_AuxReplyStatus_I2CDefer = 8,
    DPTX_AuxReplyStatus_Timeout=3,
    DPTX_AuxReplyStatus_Terminated=5
}DPTX_AuxReplyStatus_t;


typedef enum
{
	//Specification states
	DPTX_HdcpHandlerState_DetermineRxHdcpCapable,		// 0x00
	DPTX_HdcpHandlerState_ExchnageKsvs,					// 0x01
	DPTX_HdcpHandlerState_Computation,					// 0x02
	DPTX_HdcpHandlerState_ValidateRx,					// 0x03
	DPTX_HdcpHandlerState_Authenticated,				// 0x04
	DPTX_HdcpHandlerState_TestForRepeater,				// 0x05
	DPTX_HdcpHandlerState_WaitForReady,					// 0x06
	DPTX_HdcpHandlerState_ReadKsvList,					// 0x07
	DPTX_HdcpHandlerState_UnAuthenticated,				// 0x08
	//Additional states
	DPTX_HdcpHandlerState_Disabled,						// 0x09
	DPTX_HdcpHandlerState_PreAuthenticatedSubState,		// 0x0A
	DPTX_HdcpHandlerState_KsvRdySubState,				// 0x0B
	DPTX_HdcpHandlerState_AnalysisSubState,				// 0x0C
	DPTX_HdcpHandlerState_FirstPhaseComplete			// 0x0D
} DPTX_HdcpHandlerState_t;

typedef enum
{
	DPTX_LinkTrainingStatus_None,
	DPTX_LinkTrainingStatus_InProgress,
	DPTX_LinkTrainingStatus_Failed,
	DPTX_LinkTrainingStatus_Successful
} DPTX_LinkTrainingStatus_t;

typedef enum
{
	DPTX_MsIdpDataPackingMode_1Lane 	= 0x01,
	DPTX_MsIdpDataPackingMode_2Lane 	= 0x02,
	DPTX_MsIdpDataPackingMode_3Lane 	= 0x03,
	DPTX_MsIdpDataPackingMode_4Lane 	= 0x04
} DPTX_MsIdpDataPackingMode_t;

typedef enum
{
    DPTX_SdpUpdate_NO				= 0,
    DPTX_SdpUpdate_VSI 				= DPTX_SDP_VENDOR_READY,
    DPTX_SdpUpdate_AVI 				= DPTX_SDP_AVINFO_READY,
    DPTX_SdpUpdate_SPD 				= DPTX_SDP_SRCPD_READY,
    DPTX_SdpUpdate_AIF 				= DPTX_SDP_AUINFO_READY,
    DPTX_SdpUpdate_MPG 				= DPTX_SDP_MPEGSRC_READY,
    DPTX_SdpUpdate_UIF 				= DPTX_SDP_EXTENT_READY,
    DPTX_SdpUpdate_ALL 				= DPTX_SdpUpdate_VSI   |
										DPTX_SdpUpdate_AVI |
										DPTX_SdpUpdate_SPD |
										DPTX_SdpUpdate_AIF |
										DPTX_SdpUpdate_MPG |
										DPTX_SdpUpdate_UIF
} DPTX_SdpUpdate_t;

typedef enum
{
	DPTX_MstUpdateActMode_StartAtAnyLegalMtph,
	DPTX_MstUpdateActMode_StartOnlyAt4MtphPriorToANewMst, 	
}DPTX_MstUpdateActMode_t;
//******************************************************************************
//  T Y P E D E F S   ( F U N C T I O N   P O I N T E R S )
//******************************************************************************
typedef void (*ResetSwTxStsAfterAPDone_t)(void);
//******************************************************************************
//  T Y P E D E F S   ( S T R U C T S )
//******************************************************************************

typedef struct
{
	U8								iDPState;
	U8								iDPGain;
	U8								iDPPreEmp;
	BOOL							iDPHpdMonitor;
	DPTX_MsIdpDataPackingMode_t		iDPMsDataPackingMode_e;
}DPTX_IdpInternalData_t;

typedef struct
{
    BOOL		IsUpdated_b;
    U16			PacketSize_16;					// packet size in bytes
    U8			PacketID_8;					// 
    U8			DpVersion_8;
    U8			PacketData_au8[DPTX_MAX_SDP_SIZE];
}DPTX_SdpBuffer_t;

typedef DPTX_SdpBuffer_t DP_RAM *	DPTX_SdpBuffer_PTR_t;

typedef struct
{
    BOOL					Enabled_b;
    DPTX_SdpUpdate_t		EnableMask_e;
    DPTX_SdpBuffer_PTR_t	Buffers_aps[DPTX_SDP_COUNT + 1];
} DPTX_SdpModuleData_t;

typedef struct
{
	DP_CablePlugStatus_t	HpdStatus_e;
	U8						HPDShortPulseReceived_8;
} DPTX_HpdInternalData_t;

typedef struct
{
	DPTX_LinkTrainingStatus_t	LinkTrainingStatus_e;
} DPTX_LinkInternalData_t;

typedef struct {

	unsigned int			LinkFailureDetect_bf1			: 1; //bit 0: USED
	unsigned int			R0Mismatch_bf1					: 1; //bit 1: USED
	unsigned int			KsvListIntegrityCheckFailed_bf1	: 1; //bit 2: USED
	unsigned int			NotHdcpCapable_bf1				: 1; //bit 3: USED
	unsigned int			BadBksv_bf1						: 1; //bit 4: USED
	unsigned int			BKsvIsRevoked_bf1				: 1; //bit 5: USED
	unsigned int			WaitForReadyFailedTimeOut_bf1	: 1; //bit 6: USED
	unsigned int			MaxDevsExceeded_bf1				: 1; //bit 7: USED
	unsigned int			MaxCascadeExceeded_bf1			: 1; //bit 8: USED
	unsigned int			ValidateRxFailedTimeOut_bf1		: 1; //bit 9: USED
	unsigned int			AuxReadError_bf1				: 1; //bit 10: USED
	unsigned int			KsvRdyFailedTimeOut_bf1			: 1; //bit 11: USED
	unsigned int			NoBufferForKsvList_bf1			: 1; //bit 12: USED
	unsigned int			IsAttachedDeviceRepeater_bf1	: 1; //bit 13: USED
	unsigned int			ThereIsFatalError_bf1			: 1; //bit 14: USED
	unsigned int											: 1; //bit 15: NOT USED
} DPTX_HdcpSoftwareStatusFlags_t;

typedef struct {

	unsigned int			AmIRepeater_bf1					: 1; //bit 0: USED: 0 - Tx is not part of Repeater; 1 - Tx is part of Repeater
	unsigned int			ImmediatelyEncryptEnable_bf1	: 1; //bit 1: USED
	unsigned int			UseGetKsvListFunction_bf1		: 1; //bit 2: USED: 1 - Use; 0 - not Use (normal operation)
	unsigned int			LinkFailureProcessingEnable_bf1	: 1; //bit 3: USED
	unsigned int			ContinuousAuthEnable_bf1		: 1; //bit 4: USED
	unsigned int			BypassBKsvCheck_bf1				: 1; //bit 5: USED: for internal use; 1 - Bypass
	unsigned int											: 2; //bit 6-7: NOT USED
	unsigned int			UpstreamAuthRequest_bf1			: 1; //bit 8: USED
	unsigned int			BKsvChecked_bf1					: 1; //bit 9: USED: for internal use
	unsigned int			BKsvIsOk_bf1					: 1; //bit 10: USED: for internal use
	unsigned int			Sha1Checked_bf1					: 1; //bit 11: USED: for internal use
	unsigned int			Sha1IsOk_bf1					: 1; //bit 12: USED: for internal use
	unsigned int											: 1; //bit 13: NOT USED
	unsigned int											: 1; //bit 14: USED
	unsigned int			NewProcessingOfKsvList_bf1		: 1; //bit 15: USED: for internal use
} DPTX_HdcpSoftwareCtrlFlags_t;

typedef struct
{
    DPTX_HdcpHandlerState_t			DpTxHdcpState_16;
    U16								W_DpTxHdcpTimeOut;
    U16		 						W_DpTxHdcpAuxTimeOut;
    U16								W_Binfo;
    U8 *							Bp_BufferForKsvList;
    DPTX_HdcpSoftwareCtrlFlags_t	S_CtrlFlags;
    DPTX_HdcpSoftwareStatusFlags_t	S_StsFlags;
    U8								B_DeviceCount;			//for Read KSV from List support
    U16								W_ValidateRxTimeOut;	//for support of custom value of Validate Rx TimeOut
    U8								Ba_CustomAn[8];			// TODO: what does this mean ==>temporary fix for custom An (SL_BF)
} DPTX_HdcpInternalData_t;

typedef struct
{
    U32				SinkAuxMemoryBase_32;    // AUX address space used for SINK packets
    U16				SinkAuxMemorySize_16;    // size of memory allocated for SINK packets

    U32				SourceAuxMemoryBase_32;  // AUX address space base allocated for SOURCE packets
    U16				SourceAuxMemorySize_16;  // size of memory allocated for SOURCE packets
} DPTX_AuxMsgAddressMap_t;

typedef DPTX_AuxMsgAddressMap_t DP_RAM *		AuxMsgAddressMap_ps_t;

typedef void  far (* DPTX_Ext_AuxMsgClientHandlerType_t)(U8 ClientID_8, U8 *Buff_p8, U8 Length_8);

typedef struct
{
   U8                         IsClientMsgEnabled_8;
   U8              	     	MaxFuncNum_8;
   U8                   	CurFuncNum_8;
   U8                   	ClientArray_au8[MAX_CLIENTS];
   DPTX_Ext_AuxMsgClientHandlerType_t  AuxMsgClientHandlerType_pf[MAX_CLIENTS];
} DPTX_AuxMsgClientType_t;


typedef struct
{
   DPTX_AuxMsgAddressMap_t	 	AuxMsgAddressMap_s;
   U16                      	InputOffset_16;
   U16                      	OutputOffset_16;
   DPTX_AuxMsgClientType_t		AuxMsgClientModuleData_s;
} DPTX_AuxMsgInternalData_t;

typedef struct
{
    BOOL						IsI2C_b;
    BOOL						IsWrite_b;
    U8							OutLen_8;
    DPTX_AuxReplyStatus_t		AuxReplyStatus_e;
    U8							Buff_au8[DP_AUX_MAX_BYTES_IN_REQUEST];
} DPTX_AuxNonWaitingParams_t;


typedef struct
{
    BOOL						IsBusy_b;
    BOOL						IsTerminationDemanded_b;
    U8							RequestDataLength_8;
    DPTX_AuxNonWaitingParams_t				AuxNonWaitingParams_s;
   
} DPTX_AuxInternalData_t;


//DPTX_Module_Data_t contains all sub-modules data 
typedef struct 
{
	DPTX_AuxInternalData_t			AuxInternalData_s;
	DPTX_AuxMsgInternalData_t		AuxMsgInternalData_s;	
	DPTX_HdcpInternalData_t			HdcpInternalData_s;
	DPTX_LinkInternalData_t			LinkInternalData_s;
	DPTX_HpdInternalData_t			HpdInternalData_s;
	
}DPTX_CommonModuleData_t;

typedef struct
{
	// Add new status to the end of enum. Do not add anything in between.
	U16		StsUpdated				: 1;
	U16		TxCableStsToggle		: 1;
	U16		PBN_Bw_NotSupported		: 1;
	U16		SinkDevice_Changed		: 1;
	U16 	PeerDevice_Changed	 : 1;
	U16 	LinkTrainingOnShortPulseInitiated: 1;
	U16 	LinkTrainingOnShortPulseSuccess: 1;
	U16 	LinkTrainingOnShortPulseFail :1;
	U16 	ResourceChangeStatus: 1;
	U16 	AuthenticationDone: 1;

}DpMicroPktTx_Sts;

typedef struct
{
	DP_BaseAddr_t						FeMemBase_p ;				
	DP_BaseAddr_t						PhyMemBase_p ;				
	DP_BaseAddr_t						KeyHostReg_p;				
	DP_Standard_t						Standard_e;		
	U8									PreHpdStatus_8;
	BOOL							   	AutoCalibEnable_b;			
	DPTX_SetExtClkPower_t				SetExtClkPower_pf;			
	DPTX_SetExtClkSpreadSpectrum_t 		SetExtClkSpreadSpectrum_pf; 
	DP_Ext_Sha1Reset_t					Sha1Reset_pf;				
	DP_Ext_Sha1Input_t					Sha1Input_pf;				
	DP_Ext_Sha1Result_t					Sha1Result_pf;				
	DPTX_Ext_GetHardwareVersion_t		GetHardwareVersion_pf;		
	DPTX_Ext_GetFirmwareVersion_t		GetFirmwareVersion_pf;		
   	U8									CableStatus_8;				
	U8									CableStatusPrev_8;			
	U8									PeerDeviceSinkCount_8;	
	U8									DpTxAutoCalibResult_8;		
	U32									AuxStructurePtr_32;			
	U8									ClearLg_8;					
	DPTX_HdcpHandlerState_t				HdcpState_e;				
	BOOL								CanEnableEncryption_b;		
	U32									LtStartTimeout_32;		
	U32									LtSuccessWaitTime_16;
	BOOL								DryRunLt_b;
	U8									LinkStatusChecked_8;		
	U8									TxUARTEnable_8;				
	U8									TurboModeEnabled_8;			
	BOOL								IsSinkSupportsTurboMode_b;	
	U8									BwSelected_8;				
	DPTX_CommonModuleData_t				DpTxCommonModuleData_s;		
	DP_PowerState_t 					PowerState_e;				
	DP_CablePlugStatus_t				DpTxUartPassThroughHpdEventMonitor_PrevHpdStatus;	
	DP_PowerState_t						DpTxUartPassThroughStateMonitor_OldPowerState;		
	DP_CablePlugStatus_t				Tx_Sys_SpreadSpectrumHpdEventMonitor_PrevHpdStatus; 
	DpMicroPktTx_Sts					DpTx_IpSts;											
	HdcpRepeaterTxState_t				DpTx_HdcpSts;		
	BOOL								HdcpProdKeys;
	U32									PortSwitchTimer_32;
	DPTX_Ext_GetHpdStatus_t				GetHpdStatus_pf; // MYDPTX
	BOOL								HpdPlugDetected_b;
	BOOL								PhyAutomationInProgress_b;
}DpTxFeSstMstCommonData_t;

typedef struct DpTxSstFeInstanceData_S
{
	DpTxFeSstMstCommonData_t			*DpTxFeSstMstCommonData_ps;

	DP_MainLinkRate_t					InitLinkRate_e;
	DP_MainLinkRate_t					PrevLinkRate_e;				
	DP_MainLinkRate_t					LinkRate_e;					
	DP_MainLinkNumLanes_t			    InitLaneCount_e;
	DP_MainLinkNumLanes_t			    PrevLaneCount_e;			
	DP_MainLinkNumLanes_t			    LaneCount_e;	
	
	BOOL								IdpHdpMode_b;
	U8									Gain_8;						//G = VDIFF / 4.6875mV Where: G is the decimal value of G[7:0]
	U8									PreEmp_8;						// P = ((10^(Pre-emphasis/20))-1)/.03125

	//Data types internal to SST FE 
	DPTX_IdpInternalData_t				IdpInternalData_s;	
	
} DpTxSstFeInstanceData_t;

typedef struct
{
	U8 	RxedId_8;
	U16 PBN_16;
}DptxVIdMapping_t;
typedef enum
{
	ResourceChangeNotify_Clear,
	ResourceChangeNotify_CheckClearSuccess,
	ResourceChangeNotify_1c0,
	ResourceChangeNotify_CheckUpdateBit,
	ResourceChangeNotify_SendACT,
	ResourceChangeNotify_CheckACT,
}ResouceChangeNotifyState;

typedef struct
{
	U8 PayloadResourceChange;	//Holds the index of the DpTxVidMapping
	BOOL ResourceChangeInProgress; // 1 means pending 0x1c0 native aux commands
	U8  ResoureChangeState;
	U8  StartPayloadNumber;
}ResourceChangeNotify;

typedef enum
{
	SB_I2CREADSTATE_INIT,
	SB_I2CREADSTATE_WRITE,
	SB_I2CREADSTATE_START,	
	SB_I2CREADSTATE_READ,
	
}SB_I2CReadState_t;

typedef enum
{
	SB_I2CWRITESTATE_INIT,
	SB_I2CWRITESTATE_WRITE,
}SB_I2CWriteState_t;
typedef struct DpTxMstFeInstanceData_S
{
	DpTxFeSstMstCommonData_t			*DpTxFeSstMstCommonData_ps;

	DP_MainLinkRate_t					InitLinkRate_e;
	DP_MainLinkRate_t					PrevLinkRate_e;				
	DP_MainLinkRate_t					LinkRate_e;					
	DP_MainLinkNumLanes_t			    InitLaneCount_e;
	DP_MainLinkNumLanes_t			    PrevLaneCount_e;			
	DP_MainLinkNumLanes_t			    LaneCount_e;	

	//Data types internal to MST FE 
	SB_MsgBuffer_t				 		SB_UpReqRxBuffer[SBTX_UPREQ_BUFFNUM];
	SB_MsgBuffer_t 						SB_UpRepTxBuff[SBTX_UPREP_BUFFNUM];	
	SB_MsgBuffer_t 						SB_DnReqSndBuff[SBTX_DNREQ_BUFFNUM];
	SB_MsgBuffer_t 						SB_DnRepRxBuff[SBTX_DNREP_BUFFNUM];	
	U8									UpReqReceived_8 : 1;
	U8              					DnRepReceived_8 : 1;
	ConnectedRxCapabilites_e			ConnectedRxCapabilities; //BIT0:4 - (0) Connected, (1)MST_CAP,(2)DPDownstreamPresent,(3)MST_EN,(4)UP_REQ_EN
	DptxVIdMapping_t					DpTxVidMapping[MAX_NUM_PAYLOADS];
	ResourceChangeNotify				RC_s;
	U16									EpLinkBandwidth_16;
	DP_MainLinkRate_t					EpPrevLinkRate_e;	
	DP_MainLinkNumLanes_t				EpPrevLaneCount_e;
	BOOL								EpCheckLinkBw_b;
} DpTxMstFeInstanceData_t;


typedef struct DpTxSstBeInstanceData_S
{
	//Init Param Inputs
	DP_BaseAddr_t						BeMemBase_p ;
	DP_BaseAddr_t						SDPMemBase_p;
	DP_BaseAddr_t						PatGenMemBase_p;
	DPTX_Ext_LinkAutomationPatGen_t		LinkAutomationPatGen_pf;		

	//Data types internal to DP Lib
	DPTX_SdpBuffer_t					SdpBufferVsi_s;					/*gm_DpTxSdp.c*/
	DPTX_SdpBuffer_t					SdpBufferAvi_s;
	DPTX_SdpBuffer_t					SdpBufferSpd_s;
	DPTX_SdpBuffer_t					SdpBufferAif_s;
	DPTX_SdpBuffer_t					SdpBufferMpg_s;
	DPTX_SdpBuffer_t					SdpBufferUif_s;					// undefined info frame

	DPTX_SdpModuleData_t			    SdpModuleData_s;

	DP_MsaParameters_t					MsaParameters_s;
	DP_AudioConfig_t					AudioConfig_s;

	BOOL								Tx_Be_AudioHandler_IsStatusChanged_b;
	
} DpTxSstBeInstanceData_t;

typedef struct
{
	AllocatePayloadState_t			AllocatePayloadState_e;
	U16							AP_RxUpdateTimer;
	ResetSwTxStsAfterAPDone_t		ResetSwTxStsAfterAPDone_pf;
	U8              					AllocatePayLoadDone : 1;
	U8              					ClearPayLoadFwdDone : 1;
	U16							PrevDTGSts;
	U16							CurrDTGSts;


}DPTxVCPLModule_t;


typedef struct 
{
	U8		ReadPtr;
	U8		Count;
	U8 		stopbit;
	U8		BytestoRead;
	U8 		DeviceId;
	U8		NumberOfBytes;
	U8		delay;
	U8		Hdrlen;
	
}I2CReadStaticVar_t;

typedef struct 
{
	U8					WritePtr;
	U8 					DeviceId;
	U8					NumberOfBytes;
	SB_I2CWriteState_t	I2CWriteState;
	U8					StopBit;
}I2CWriteStaticVar_t;
typedef struct DpTxMstBePortSpecificInstanceData_S
{
	U8									VideoEnabled;
	U32									PrevMval_32;
	U16 								DTG_MsMeasValue;
	BOOL 								DTG_Stable;
	SB_I2CReadState_t 					SB_RemoteI2CReadState_8;
	U8									I2CBuff_a8[MAX_MSG_LEN_PER_TRANS];
	I2CReadStaticVar_t					I2CReadStaticVar_s;
	I2CWriteStaticVar_t					I2CWriteStaticVar_s;
	Aux_data_buffer_t 					SB_AuxBuffer_s;
	Aux_data_buffer_t 					*P_AUX_current_SB_req;		
	Aux_data_buffer_t 					*P_AUX_current_Native_req;
	Aux_data_buffer_t 					*P_AUX_current_I2C_req;
	Aux_data_buffer_t 					*P_AUX_current_req;

	SB_MsgBuffer_t FAR*					SB_DnReqTxExecLL;
	U8      							MsgUnderProcess; 
	U8									MsgUnderProcessMsn:1;

	U8									APReplyUnderProcess:1;
	U8									APReqUnderProcess:1;
	
	U8 									RxPortNum:4;
	DPTX_LinkTrainingStatus_t 			APLinkTrainingStatus_e;
	U32									APLtStartTimeout_32;	
	U8									APReasonForNack_e;
}DpTxMstBePortSpecificInstanceData_t;
typedef struct DpTxMstBeInstanceData_S
{
	//Init Param Inputs	
	DP_BaseAddr_t						BeMemBase_p;
	DP_BaseAddr_t						PatGenMemBase_p;
	DPTX_Ext_LinkAutomationPatGen_t		LinkAutomationPatGen_pf;
	DPTX_Ext_ExecuteLogicalPortI2c_t	ExecuteLogicalPortI2c_pf;
	DP_Ext_GetLogicalPortBksv_t			DPGetLogicalPortBksv_pf;
	SdpStreams_t						SdpStreamsInfo;	
	//Data types internal to DP Lib
	DPTxVCPLModule_t					DpTxVCPLData;
	U16 								LogPortStsReadTimerVal_32;
	U16 								PhyPortStsReadTimerVal_32;
	U8 									CddsChangeFreq_8;
	U16 								MreadTimer_16;
	U8 									CddsRegTblCount_8;
	BOOL 								CheckIfmStatus_b;
	U16 								IfmTimer_16;
	U16 								StartCddsRoutineTimer_16;
	BOOL 								StartCddsRoutineTimerState_b;
	U8 									IfmCount_8;
	BOOL								CheckForDtgStable_b;
	U8 									counter;
	DpTxMstBePortSpecificInstanceData_t	*DPTx_MstBePortSpecific_ps;
	
} DpTxMstBeInstanceData_t;



//******************************************************************************
//  V A R I A B L E   E X T E R N S
//******************************************************************************

//******************************************************************************
//  F U N C T I O N   E X T E R N S
//******************************************************************************

//******************************************************************************
//  F U N C T I O N   P R O T O T Y P E S
//******************************************************************************

void DP_EXT_FUNC Tx_Be_LastBranchDeviceDisableInMst(void);
void DP_EXT_FUNC Tx_Be_LastBranchDeviceEnable(BOOL SstOutputTimingEnable_b);
void DP_EXT_FUNC Tx_Be_RxInSelect (DPTX_RxInSel_t DPTX_RxInSel_e);
DPTX_RxInSel_t DP_EXT_FUNC Tx_Be_GetRxIn (void);
BOOL DP_EXT_FUNC Tx_Be_MsAllocatePayload(U8 PayloadId_8, U16 RequestedPbn_16, U16 * AllocatedPbn_p16,BOOL IsCPL);
void DP_EXT_FUNC DpTX_clearPayloadTable(void);
U8 DP_EXT_FUNC DpTx_ClearTimeslotTable (U8 RxedId_8);
U8 CalculateTimeSlots(U16 Pbn_16);
U8 MapVidTxtoRx(U8 TxId_8);

// APIs related to 3D Video
void DP_EXT_FUNC SDP_SendVSCPacket(STDP_3DFormat_t ThreeDFormat);
void DP_EXT_FUNC SDP_SetLeftRight(STDP_3DView_t ThreeDView);
BOOL DP_EXT_FUNC DPTX_IsHdcpEnabled(void);
void DP_EXT_FUNC Tx_Fe_MstUpdateAct(DPTX_MstUpdateActMode_t MstUpdateActMode_e);
BOOL UpdateRxPayloadTable(	 U8	PayloadId_8, U8 StartPayloadNumber_8, U8 RequestedNumberOfPayloads_8);
/*************************L I N K   T  R A  I N N I N G ******************************/
void DP_EXT_FUNC Tx_Fe_LinkRestartTrainingHandler(void);
void DP_EXT_FUNC Tx_Fe_ForceLinkTraining (void);
//******************************************************************************
void DP_EXT_FUNC Tx_Fe_HdcpGetBksv(U8 DP_RAM * Bksv_p8);
void DP_EXT_FUNC Tx_Fe_HdcpDisableEncryption (void);
void DP_EXT_FUNC Tx_Fe_HdcpEnableEncryption(void);
BOOL DP_EXT_FUNC Tx_Fe_IsHdcpEncryptionEnabled(void);

DP_CablePlugStatus_t DP_EXT_FUNC Tx_Fe_HpdGetCableStatus(void);
DPTX_LinkTrainingStatus_t DP_EXT_FUNC Tx_Fe_LinkGetLastLtStatus(void);
BOOL DP_EXT_FUNC Tx_Fe_IsLinkTrainingRequired (void);
BOOL DP_EXT_FUNC Tx_Be_IsDtgMeasPresent(void);
void FAR DpHdcpRptrTxEntry(U32						    TxDeviceIdentifier_32,
								 HdcpRepeaterTxRequest_t	    TxRequest_e,
								 U8 *							TxReply_pv);

void FAR DpHdcpRptrTxEntryLogicalPort (U32	TxDeviceIdentifier_32,
										HdcpRepeaterTxRequest_t	RxRequest_e,
										U8* TxReply_pv);

#pragma option -b.
#ifdef __PARADIGM__
#pragma pack(pop)
#endif
#endif // __DPTXLIB_H__
//********************************* END ****************************************
