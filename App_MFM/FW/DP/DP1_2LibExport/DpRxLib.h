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

#ifndef __DPRXLIB_INCLUDED__
#define __DPRXLIB_INCLUDED__
#include <setjmp.h>
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

#define DPRX_SDP_COUNT			10

#define MAX_NUM_TX_PORTS		4			// Max supported by HW

#define SIZE_OF_NONCE			7

#define MAX_GOOD_PHASE 		0x3F
#define MID_GOOD_PHASE 		0x20
#define MIN_GOOD_PHASE 		0x0
#define MIN_PHASE 			0x14 
#define MAX_PHASE 			0x2C
#define SEARCH_STEP     	2

#define MAX_GOOD_CPTRIM	 	0x20+4
#define MID_GOOD_CPTRIM 	0x20
#define MIN_GOOD_CPTRIM 	0x20-4

#define L3_OK 				DP_BIT15
#define L2_OK 				DP_BIT14
#define L1_OK 				DP_BIT13
#define L0_OK 				DP_BIT12

#define GOOD_MIN_MAX_RANGE	12
#define SEARCH_RANGE        (GOOD_MIN_MAX_RANGE +8)  

#define LQA_MAX_INDEX 				12
#define LQA_TA_MAX_INDEX 			4
#define LQA_SYM_LOCK_EQ_ITERATION	5
#define LQA_SETTING_STEP 			3  

#define LSM_LINK_ERR_COUNT_MAX		5
#define LSM_LINK_ERR_ACC_PER_LANE   4
#define LSM_LINK_ERR_ANALYSIS_TIME  33

//******************************************************************************
//  M A C R O   D E F I N E S
//******************************************************************************

//******************************************************************************
//  T Y P E D E F S 
//******************************************************************************

typedef DP_SdpInfoFrame_t DP_RAM *		DPRxSDPBufferPtr;

//******************************************************************************
//  T Y P E D E F S   ( E N U M S )
//******************************************************************************

typedef enum
{
    DpRxLTStatus_NONE,
    DpRxLTStatus_CR_FINISHED,
    DpRxLTStatus_FINAL_SETTING,
    DpRxLTStatus_SYMLOCK_FINISHED,
    DpRxLTStatus_DERIVED_SYMLOCK_FINISHED

} DpRxLTStatus_t;

typedef enum
{
	DPRXLSMStatus_NONE,
	DPRXLSMStatus_NOLINK,
	DPRXLSMStatus_CR_DONE,
	DPRXLSMStatus_SL_DONE
} DPRXLSMStatus_t;


typedef enum
{
    DPRxSDPUpdate_NO_UPDATES   = 0,

    DPRxSDPUpdate_ATS_UPDATED  = (0x1UL << 2), //DPRX_SDP_AUD_MN_CHNG_IRQ_STS,  // Audio time stamp
    DPRxSDPUpdate_EXT_UPDATED  = (0x1UL << 5), //DPRX_EXT_PACK_CHNG_IRQ_STS,    // Extended packet

    DPRxSDPUpdate_VSI_UPDATED  = (0x1UL << 8),  //DPRX_SDP_IF_VEND_STS,         // Vendor Specific infoframe
    DPRxSDPUpdate_AVI_UPDATED  = (0x1UL << 9),  //DPRX_SDP_IF_AVI_STS,          // Auxiliary Video Information infoframe
    DPRxSDPUpdate_SPD_UPDATED  = (0x1UL << 10), //DPRX_SDP_IF_SPD_STS,          // Source Product Description
    DPRxSDPUpdate_AIF_UPDATED  = (0x1UL << 11), //DPRX_SDP_IF_AUDIO_STS,        // Audio info frame
    DPRxSDPUpdate_MPG_UPDATED  = (0x1UL << 12), //DPRX_SDP_IF_MPEG_STS,         // MPEG source
    DPRxSDPUpdate_ACM_UPDATED  = (0x1UL << 14), //DPRX_SDP_IF_AU_CPYMGMENT_STS,
    DPRxSDPUpdate_ISRC_UPDATED = (0x1UL << 15), //DPRX_SDP_IF_ISRC_STS,
	DPRxSDPUpdate_VSC_UPDATED  = (0x1UL << 16), //DPRX_SDP_IF_VSC_STS,    

//	DPRxSDPUpdate_MSA_UPDATED	= DPRX_MN_STR_ATTR_CHNG_IRQ_STS,	// Main stream attribute packet

    DPRxSDPUpdate_ALL_UPDATED = DPRxSDPUpdate_ATS_UPDATED  | DPRxSDPUpdate_EXT_UPDATED  |
								DPRxSDPUpdate_VSI_UPDATED  | DPRxSDPUpdate_AVI_UPDATED  |
								DPRxSDPUpdate_SPD_UPDATED  | DPRxSDPUpdate_AIF_UPDATED  |
								DPRxSDPUpdate_MPG_UPDATED  | DPRxSDPUpdate_ACM_UPDATED |
								DPRxSDPUpdate_ISRC_UPDATED | DPRxSDPUpdate_VSC_UPDATED
} DPRxSDPUpdate_t;


typedef enum
{
	DPRxHandlerState_NONE,
	DPRxHandlerState_DO_PULSE,
	DPRxHandlerState_PULSE_LOW,
	DPRxHandlerState_SHRT_PULSE_GEN,
	DPRxHandlerState_SEND_PULSE
} DPRxHandlerState_t;

typedef enum
{
	DPRxHDCPHandleState_UNAUTHENTICATED_STATE,
	DPRxHDCPHandleState_COMPUTATIONS_STATE,
	DPRxHDCPHandleState_DISABLED_STATE,
	DPRxHDCPHandleState_LINK_FAILURE_TO_STATE,
	DPRxHDCPHandleState_COMPUTATIONS_DONE_STATE,
	DPRxHDCPHandleState_ENCRYPTION_ENABLED_STATE
} DPRxHDCPHandleState_t;

typedef enum
{
	DPEventVideoColorSpace = 1,
	DPEventAudioMute = (1 << 1),
	DPEventAudioConfig = (1 << 2),
} DPRxEvents;

//******************************************************************************
//  T Y P E D E F S   ( F U N C T I O N   P O I N T E R S )
//******************************************************************************

typedef void EXT_FUNC (*DPRxFwLtExtHandler_t)(void);
typedef U8 EXT_FUNC (*DPRxFWLtCBGetCurrentScore_t)(U8 NumLanes_8);
typedef void EXT_FUNC (*DPRxFWLtCBSetExtStatus_t)(U8 NumLanes_8,BOOL IsLTIncluded);
typedef void EXT_FUNC (*DPRxFWLtCBNotderivedCRIteration_t)(U8 NumLanes_8);
typedef void EXT_FUNC(*ClearAllocateTimerStatusonTx_t)(void);
//******************************************************************************
//  T Y P E D E F S   ( S T R U C T S )
//******************************************************************************

typedef struct
{
	BOOL 						AutoCalibEnable_b;
	BOOL						AuxOnUponPowerOff_b;
	U16							HpdLongPulseWidth_16;
	DP_MainLinkRate_t			LinkRate_e;
	DP_MainLinkNumLanes_t		LaneCount_e;
}	DPRxFeatureConfigParam_t;

typedef struct
{
	DP_Standard_t				DpStandard_e;				/* iDP, eDP, normal DP        */
	DP_Version_t				DpVersion_e;				/* DP 1.0 or DP 1.1 or DP 1.2 */
}	DPRxDPModeConfigParam_t;

typedef struct
{
	U16						LastTime_16;
	U16						TimeSpan_16;
	U8						SpansNum_8;
	DPRXLSMStatus_t			LastLinkStatus_e;
} DPRxLSMInternalData_t;

typedef struct
{
	U8							LinkErrorCount_8;
	U16 						LinkErrorCount_16[16];
	U16							LinkErrorCountMax_16;
	BOOL						FailLtIntentionally_b;
	DP_MainLinkRate_t			FailLinkRate_e;
	DP_MainLinkNumLanes_t		FailLaneCount_e;
	U16							LinkErrorAnalysisTimer_16;
} DPRxLinkErrorAnalysis_t;

typedef struct
{
	BOOL					IsAUDEnabled_b;
	BOOL					IsVSCPktRcvd_b;  /* To identify if we have valid VSC data in the hardware bank */
	BOOL					IsEnabled_b;
	DPRxSDPUpdate_t			Updated;
	DPRxSDPUpdate_t			EnableMask;
	DPRxSDPBufferPtr		SDPBufferPtrs_p[DPRX_SDP_COUNT];
} DPRxSDPModuleData_t;

//HPD types

typedef struct
{
	DPRxHandlerState_t		State_e;
	U16						LongPulseDuration_16;
	U16						ShortPulseDuration_16;
	U16						LongPulseStartTime_16;
	U16						ShortPulseStartTime_16;
	U8						ShortPulseDelay_8;
	BOOL					GenerateHPDOnLnkLost_b;
} DPRxHPDHandlerInternalData_t;

typedef struct
{
	DPRxHDCPHandleState_t	DpRxHdcpState_e;
	U16						DpRxHdcpTimeOut_16;
	U8						DevCnt_8;
	U8						CurrentDevCnt_8;
	U8						LinkIntergrityFilter_8;
	const U8 DP_RAM *		KsvsList_p8;
	const U8 DP_RAM *		CurrentKsv_p8;

}DPRxHDCPInternalData_t;

typedef struct
{
	BOOL				IsEnabled_b;
	BOOL				IsMuteStateChanged_b;
	BOOL				IsAudioConfigChanged_b;
	BOOL				IsMuteState_b;
	DP_AudioConfig_t	AudioConfig_e;
} DPRxAudioModuleData_t;

typedef struct
{
    U16			HBR2DrvCur			: 4;
    U16			HBR2PreEmph			: 4; 	
    U16			HBR2Eq				: 4;
    U16			HBR2Fp				: 4;
    U16			HBR2EqPhyAuto    	: 4;
    U16			HBR2FpPhyAuto		: 4;	
    U16			HBRDrvCur			: 4;
    U16			HBRPreEmph			: 4; 	
    U16			HBREq				: 4;
    U16			HBRFp				: 4;	
    U16			HBREqPhyAuto        : 4;
    U16			HBRFpPhyAuto	    : 4;	
    U16			RBRDrvCur			: 4;
    U16			RBRPreEmph			: 4; 	
    U16			RBREq				: 4;
    U16			RBRFp				: 4;	
    U16			RBREqPhyAuto        : 4;
    U16			RBRFpPhyAuto        : 4;	
}DPRXFwLtAdjustLevel_t;

typedef struct
{
	U8									DPCDLinkPrevBW_8;
	U8									TurboModeLinkPrevBW_8;
	DpRxLTStatus_t						Status_e;						// ERxLT_States
	BOOL								TriggerLSM_b;
	BOOL								TriggerLQA_b;
	U16									LQATimer_16;
	BOOL								Pat2Pat1_b;
	U8									CurrLevel_8;
	U8									CurrLevelAlt_8;
	BOOL								FwSLAltMode_b;
	BOOL								AllPreGainTried_b;

	DPRxFWLtCBGetCurrentScore_t			cbGetCurrentScore_pf;
	DPRxFWLtCBSetExtStatus_t			cbSetExternalStatus_pf;
	DPRxFwLtExtHandler_t				cbLtReInitialize_pf;
	DPRxFWLtCBNotderivedCRIteration_t	cbNotDerivedCRIteration_pf;

	DPRxFwLtExtHandler_t				cbFinishRxLtIter_pf;
	DPRxFwLtExtHandler_t				cbToggleRxLtHPD_pf;

	DPRXFwLtAdjustLevel_t DP_ROM *		P_AdjLevelArrary;
	DPRXFwLtAdjustLevel_t DP_ROM *		P_AdjLevelArrary_Alt;
	U8									MaxFwLtIter_8;
	U8									MaxFwLtIterAlt_8;
	BOOL								StopTXLtAfterHPDPulse_b;
} DPRxFWLtInternalData_t;

typedef struct
{
	U8	Eq_bf3 				:3 ;
	U8	DR_bf1 				:1 ;
	U8	DriveCurPreEmp_bf4  :4 ;
	U8  CurrLaneCount_bf2   :2 ;		//0=1L/1=2L/ 3=4L
	U8	CurrBW_bf2			:2 ; 		//0=RBR/1=HBR/2=HBR2
	U8	L0Status_bf1		:1 ;
	U8	L1Status_bf1		:1 ;
	U8	L2Status_bf1		:1 ;
	U8	L3Status_bf1		:1 ;	

	U8	L0EyeHeight_bf4		:4 ;		//Eye height/2
	U8	L1EyeHeight_bf4		:4 ;	
	U8	L2EyeHeight_bf4		:4 ;
	U8	L3EyeHeight_bf4		:4 ;
}LinkQualityResults_t;


typedef struct
{
	U8 DR_bf2 			:2;
	U8 Eq_bf3  			:3;
	U8 EyeHeight_8;
	
}LinkQualityTaResults_t;


typedef struct
{
	LinkQualityResults_t 		LinkQualityResults_s[LQA_MAX_INDEX];
	LinkQualityTaResults_t	    LinkQualityTaResults_s[LQA_TA_MAX_INDEX];
	U8							SymLockIterationEqResult_a8[LQA_SYM_LOCK_EQ_ITERATION];
	U8							SymLockIteration_8;
	U8							LqaTaIndex_8;
	U8 							LqaIndex_8;
	U8 							MaxPhase_a8[4];
	U8							MinPhase_a8[4];
	U8 							LqaOk_8;
	U8							LqaDone_8;
	BOOL						LqaSetAuxNotReady_b;
	DPRXFwLtAdjustLevel_t   	RxFWLTAdjTable_Ram[4];	
	U8							AdjReqCnt_8;	
	U32							Lx_CTRL_0[4];
	U32							Lx_CTRL_4[4];
}DPRxLqaInternalData_t;

typedef struct
{
	DP_PowerState_t			PowerHandlerState_e;
	DP_PowerState_t			PrevPowerHandlerState_e;
	U8						DPCDPowerState_8;
	BOOL					IsDPCablePlugged_b;
	BOOL					IsNotifyPMTransition_b;
	BOOL					IsDPCDTimerStart_b;
	U16						DPCDPowerDownTimeOut_16;
	BOOL					IsAudioEnableOnStandby_b;
	BOOL					Is_StandbyTimerStart_b;
	U16						StandbyTimeOut_16;
	BOOL					IsDPCableDetectTimerStart_b;
	BOOL					IsDeBouncetimerStarted;
	U16						DebounceTimeOut_16;
	U8						PreviousCableStatus_8;
}DPRxPMInternalModule_t;

typedef struct
{
	U8						TxPortId 	   : 4;
	U8						QSESReplyOk	   : 1;
	U8						QSESReplyRxd   : 1;
	U8						TxPortInfoRsvd : 2;
	U8						TxStreamId;
} PayloadMappingTxPortInfo_t;

typedef struct
{
	U8							RxStreamId_8;
	U16 						RxedPBN_16;
	U8							NumberOfTimeSlots;
	U8							StreamEncryptionInfo;
	U8							QsesNonce[SIZE_OF_NONCE];
	PayloadMappingTxPortInfo_t	TxPortInfo[MAX_NUM_TX_PORTS];
}PayloadMappingData_t;

typedef struct
{
	U8							UsedPayLoadId_16;
	PayloadMappingData_t		PayLoadData[MAX_NUM_PAYLOADS + 1];
} DPRxVCPLModule_t;

typedef struct
{
	U16				LinkStatus_16;
	U16				PreviousLinkStatus_16;
	U16				Temp_16;
	U8				PreviouState_8;					// = 0xFF;
	U8				State_8;						// = 0x00;
	U16				LastHPDAssertTime_16;			// = 0x0;
	BOOL			IfLastHPDAssertTimeOut_b;		// = TRUE;
	BOOL			TimerStarted_b;					// = FALSE;
	U16				LastTime_16;
	U16				TimerValue_16;
} DPRxIDPInternalData_t;
#if 0
typedef struct
{
	//		Add new status to the end of enum. Do not add anything in between.
	U8		StsUpdated       : 1;
	U8		RxCableStsToggle : 1;
}DpMicroPktRx_Sts;
#else
typedef struct
{
	// Add new status to the end. Do not add anything in between.
	U8 RxCableStsDisconnect         	: 1;
	U8 Reserved						: 7;
	U8 ConnectedTxCapabilites_Crlost   : 1;	
}DpMicroPktRx_Sts;
#endif

typedef struct
{
	DPRxPMInternalModule_t					PMModuleData;
	DPRxFWLtInternalData_t					FWLTModuleData;
	DPRxLqaInternalData_t					LqaModuleData_s;
	DPRxHDCPInternalData_t					HdcpModuleData;
	DPRxHPDHandlerInternalData_t			HpdHandlerModuleData;
	DPRxLSMInternalData_t					LSMModuleData;
	DPRxLinkErrorAnalysis_t					LinkErrorAnalysisModuleData;
	BOOL									POTLinkConfigOK_b;
	BOOL									UsePOTLink_b;
	U8										POTLinkConfigIndex_8;
	BOOL									POTTimerStart_b;
	BOOL									POTOnInit_b;
	BOOL									PowerManagmentAFDFlag_b;
	U8										AutoCalibResult_8;
	U8										LinkFailure_8;
	BOOL									HPDShortPulseMonitor_b;
	BOOL									SendHPD_b;
	DP_BaseAddr_t							FEMemBase_p;
	DP_BaseAddr_t							PhyMemBase_p;
	DP_BaseAddr_t							DpcdMemBase_p;
	DP_BaseAddr_t							HdcpKeyHostBase_p;
	DPRX_DpRxExtFuncClkPower_t				DpRxExtFuncClkPower_pf;
	DPRX_DpRxExtFuncGetCableStatus_t		DpRxExtFuncGetCableStatus_pf;
	DPRX_DpRxExtFuncPwrEventNotification_t 	DpRxExtFuncPwrEventNotification_pf;
	DPRX_DpRxExtFuncPWStateUpdate_t			DpRxExtFuncPWStateUpdate_pf;
   DPRX_DpRxExtFuncAUXI2CHandler_t			DpRxExtFuncAuxI2cHandler_pf;
	DP_Ext_Sha1Reset_t						Sha1Reset_pf;
	DP_Ext_Sha1Input_t						Sha1Input_pf;
	DP_Ext_Sha1Result_t						Sha1Result_pf;
	DPRxHDCPHandleState_t					HdcpState_e;
	U8										LinkControl1RegVal_8;
	BOOL									RxUARTEnable_b;
	DP_PowerState_t							OldPowerState_e;
	BOOL									HPDShortPulseReIntrnt_b;
	U16										POTTimeOut_16;
	U16										POTTimerValue_16;
	DPRxDPModeConfigParam_t					DpRxDPModeConfig_s;
	DPRxFeatureConfigParam_t				DpRxFeatureConfig_s;
	DpMicroPktRx_Sts						DpRx_IpSts;
	HdcpRepeaterRxState_t					DpRx_HdcpSts;
	BOOL 									DprxLinkStsEvent;
	BOOL									HdcpProdKeys;
	BOOL									Aux2I2c_b;				
	BOOL 									HpdAppStatus;
	U8										DoLongJmp_8;
	jmp_buf 								jmpb;	
	BOOL									PrbsDetected_b ; 
	BOOL 									Hbr2PatternDetected_b;
	DP_MainLinkRate_t						PrevLinkRateForLqaDone_e;
	DP_MainLinkNumLanes_t					PrevLaneCountForLqaDone_e;
	BOOL									UseLqaSettings_b;
	DP_Ext_RXSetVoltLvl_t					DpRxVoltLvl_pf;
	DPRX_VoltageLvl_t						PrevVoltLvl_t;
}DpRxFeSstMstCommonData_t;

typedef struct
{
	DpRxFeSstMstCommonData_t			    *DpRxFeSstMstCommonData_ps;

	DPRxIDPInternalData_t					iDPModuleData;
	// callbacks
	BOOL 									IdpUseHpd_b;
	DPRX_EqualizationLevel_t				EqualizationLevel_e;


} DpRxSstFeInstanceData_t;

typedef enum
{
	SB_LINKADDRESS_INIT,
	SB_LINKADDRESS_REPLY,
}SB_LinkAddressProcessState_t;


typedef struct
{
	SB_LinkAddressProcessState_t	SB_LAReadState_8;
	U8								SB_LA_NumDownStreamInfoSent;
	PortStructure_t 				*LAPortInstance_ps;
}SB_LAProcess_s;

typedef struct 
{
	DpRxFeSstMstCommonData_t			    *DpRxFeSstMstCommonData_ps;

	DP_BaseAddr_t							DpcdUpReqBase_p;			
	DP_BaseAddr_t							DpcdUpRepBase_p;			
	DP_BaseAddr_t							DpcdDownReqBase_p;			
	DP_BaseAddr_t							DpcdDownRepBase_p;			
		
	SB_MsgBuffer_t FAR *					SB_DnReqFreeLL;
	SB_MsgBuffer_t FAR *					SB_DnReplyFreeLL;
	SB_MsgBuffer_t FAR *					SB_DnReplySendLL;

	SB_MsgBuffer_t FAR *					SB_UpRepFreeLL;
	SB_MsgBuffer_t FAR *					SB_UpReqFreeLL;
	SB_MsgBuffer_t FAR *					SB_UpReqSendLL;

	U8										SSTModeTxPortNum;

	U8			                 			SB_TrackReplyBuffSent;
	U8                          			SB_TrackReqBuffSent;

	U8										ConnectedTxCapabilities; //BIT0:2 - (0)UP_REQ_EN,(1)MST_EN,(2) connected
	U32										PrevEcf_32[2];
	//Data types internal to MST FE 
	SB_MsgBuffer_t				 			SB_DownReqOrignatorBuffer[SBRX_DOWNREQ_BUFFNUM];
	Aux_data_buffer_t 						I2C_AuxBuffer_s;
	SB_LAProcess_s 							SB_LAProcessVal_s;
	U8									    MsgUnderProcess;
	U8 										MsgUnderProcessMsn:1;
	U8 										RxPortNum:4;
}DpRxMstFeInstanceData_t;

typedef struct 
{
	DPRxAudioModuleData_t					AudioModuleData; 
	DPRxSDPModuleData_t						SDPModuleData;
	DPRX_AudioClockMode_t					DPAudioClockMode_e;
	
	U8										VideoFormat_8;
	U8										FrameWait_8;
	U8										CRCTestCount_8;
	U16										PrevCRC_16[3];
	U16										CurrCRC_16[3];
	U16										DPRX_Events;
	BOOL									TestSinkStart_b;			

	DP_SdpInfoFrame_t						SDPBufferAts;						// Audio timestamp packet buffer
	DP_SdpInfoFrame_t						SDPBufferExt;						// Extenstion packet buffer
	DP_SdpInfoFrame_t						SDPBufferVsi;						// Vendor specific infoframe buffer
	DP_SdpInfoFrame_t						SDPBufferAvi;						// Auxilary video inforamation infoframe buffer
	DP_SdpInfoFrame_t						SDPBufferSpd;						// Source product description infoframe buffer
	DP_SdpInfoFrame_t						SDPBufferAif;						// Audio infoframe buffer
	DP_SdpInfoFrame_t						SDPBufferMpg;						// MPEG source infoframe buffer
	DP_SdpInfoFrame_t						SDPBufferACRM;						//audio copy right management
	DP_SdpInfoFrame_t						SDPBufferISRC;						//ISRC
	DP_SdpInfoFrame_t						SDPBufferVsc;						// video stereo config

	DP_BaseAddr_t							BEMemBase_p;						/* base Address for Back-End registers */
	DP_BaseAddr_t							SdpMemBase_p;						/*DPRX1<-0x10800UL, DPRX2<-0x1A000UL*/

	// callbacks
	DPRX_DpRxExtFuncAudioDetectedNotify_t	DpRxExtFuncAudioDectedNotify_pf;	/* notify if Audio detected */
	DP_Ext_DpConfigChange_t					DPConfigChange_pf;

}DpRxSstBeInstanceData_t;

typedef struct 
{
	DP_BaseAddr_t							BEMemBase_p;						/* base Address for Back-End registers */
	DP_BaseAddr_t							GTCMemBase_p;
	DPRxVCPLModule_t						DpRxVCPLData;						/* data for VC payload handler*/
	
}DpRxMstBeInstanceData_t;

//******************************************************************************
//  V A R I A B L E   E X T E R N S
//******************************************************************************

//******************************************************************************
//  F U N C T I O N   E X T E R N S
//******************************************************************************

//******************************************************************************
//  F U N C T I O N   P R O T O T Y P E S
//******************************************************************************

void DP_EXT_FUNC PM_SetPower(DP_PowerState_t PowerState_e);

DP_PowerState_t DP_EXT_FUNC PM_GetPower(void);
DPRXLSMStatus_t DP_EXT_FUNC LSM_LsmGetStatus(void);
PayloadMappingData_t * 			FindPayloadMappingDataFromRxStreamId(U8 RxStreamId_8);
void FAR DpHdcpRptrRxEntry(U32						RxDeviceIdentifier_32,
								 HdcpRepeaterRxRequest_t	    RxRequest_e,
								 U8 *							RxRequestData_pv);
void DP_EXT_FUNC DPRX_HDCP_SetSSCBit(BOOL SendHPD);
STDP_3DFormat_t  DP_EXT_FUNC SDP_Get3DConfig(void );
void DP_EXT_FUNC DP_Monitor_PwrNeed(void);

#pragma option -b.
#ifdef __PARADIGM__
#pragma pack(pop)
#endif
#endif // __DPRXLIB_INCLUDED__

