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

#ifndef __STDPRX_H__
#define __STDPRX_H__
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

#define SUPPORT_RX_POWER_MANAGEMENT			0
#define SUPPORT_RX_TA_MODE					1
#define SUPPORT_RX_HDCP						1
#define SUPPORT_RX_CRC						1

//******************************************************************************
//  M A C R O   D E F I N E S
//******************************************************************************

//******************************************************************************
//  T Y P E D E F S 
//******************************************************************************

//******************************************************************************
//  T Y P E D E F S   ( E N U M S )
//******************************************************************************
typedef enum
{
	DP_EqualizationLevel_0_0dB=0,
	DP_EqualizationLevel_1_5dB,
	DP_EqualizationLevel_2_9dB,
	DP_EqualizationLevel_4_5dB,
	DP_EqualizationLevel_6_2dB,
	DP_EqualizationLevel_7_5dB,
	DP_EqualizationLevel_8_2dB,
	DP_EqualizationLevel_9_1dB,
} DPRX_EqualizationLevel_t;

typedef enum
{
	DPRX_PwrTransitionEvent_OffToOn			= 0,
	DPRX_PwrTransitionEvent_StandbyToOn,
	DPRX_PwrTransitionEvent_ToSleep,
	DPRX_PwrTransitionEvent_OnToStandby,
	DPRX_PwrTransitionEvent_SleepToStandby,
	DPRX_PwrTransitionEvent_OffToStandby,
	DPRX_PwrTransitionEvent_ToSoftOff,
	DPRX_PwrTransitionEvent_ToHardOff
} DPRX_PwrTransitionEvent_t;

// Audio status
typedef enum
{
	AUDIO_STATUS_NO_CHANGES			= 0,
	AUDIO_STATUS_MUTE_STATE_CHANGED	= 1,
	AUDIO_STATUS_CONFIG_CHANGED		= (1 << 1),
} DPRX_AudioStatus_t;

typedef enum
{
	DPRX_AudioClockMode_128_fs = 128,
	DPRX_AudioClockMode_192_fs = 192, 
	DPRX_AudioClockMode_256_fs = 256,
	DPRX_AudioClockMode_384_fs = 384,
	DPRX_AudioClockMode_512_fs = 512,
	DPRX_AudioClockMode_640_fs = 640,
	DPRX_AudioClockMode_768_fs = 768,
	DPRX_AudioClockMode_1024_fs = 1024
} DPRX_AudioClockMode_t;

//Enum to denote power level
typedef enum
{
	DP_VOLTAGELVL_3_3V 	= 0x0,	//3.3V
	DP_VOLTAGELVL_5V 	= 0x1,	//5V
	DP_VOLTAGELVL_12V	= 0x2,	//12V
	DP_VOLTAGELVL_18V	= 0x4,	//18V
	DP_VOLTAGELVL_NONE  = 0xF	//No voltage Required
}DPRX_VoltageLvl_t;


//******************************************************************************
//  T Y P E D E F S   ( F U N C T I O N   P O I N T E R S )
//******************************************************************************

typedef void DP_EXT_FUNC (* DPRX_DpRxExtFuncClkPower_t)(BOOL Enable_b);
typedef U8   DP_EXT_FUNC (* DPRX_DpRxExtFuncGetCableStatus_t)(U8 ApplicationPort_8);
typedef void DP_EXT_FUNC (* DPRX_DpRxExtFuncPwrEventNotification_t)(DPRX_PwrTransitionEvent_t EventId_e);
typedef BOOL DP_EXT_FUNC (* DPRX_DpRxExtFuncAUXI2CHandler_t)(U16 Address_16, BOOL Dir_b, U8 *Buffer_p8, U16 Length_16, U8 Stop_8);
typedef void DP_EXT_FUNC (* DPRX_DpRxExtFuncPWStateUpdate_t)(DP_PowerState_t PowerState_e);
typedef void DP_EXT_FUNC (* DPRX_DpRxExtFuncAudioDetectedNotify_t)(void);
typedef BOOL DP_EXT_FUNC (* DP_Ext_RXSetVoltLvl_t)(DPRX_VoltageLvl_t VoltLvl_e);


//******************************************************************************
//  T Y P E D E F S   ( S T R U C T S )
//******************************************************************************
#define RxMSTBeInit						\
	DP_BaseAddr_t							BEMemBase_p;						\
	DP_BaseAddr_t							GTCMemBase_p;						\
	
#define RxFeSstMstCommonInit													\
	DP_BaseAddr_t							FEMemBase_p;						\
	DP_BaseAddr_t							PhyMemBase_p;						\
	DP_BaseAddr_t							DpcdMemBase_p;						\
	DP_BaseAddr_t							KeyHostReg_p;						\
	BOOL									AutoCalibEnable_b; 				    \
	DP_Standard_t							DpStandard_e;						\
	DP_Version_t							DpVersion_e;						\
	DPRX_EqualizationLevel_t				EqualizationLevel_e;				\
	BOOL									AuxActiveInPowerOff_b;				\
	U16										HpdLongPulseWidth_16;				\
	DP_MainLinkRate_t						LinkRate_e;							\
	DP_MainLinkNumLanes_t					LaneCount_e;						\
	DPRX_DpRxExtFuncClkPower_t				DpRxExtFuncClkPower_pf;				\
	DPRX_DpRxExtFuncGetCableStatus_t		DpRxExtFuncGetCableStatus_pf;		\
	DPRX_DpRxExtFuncPwrEventNotification_t 	DpRxExtFuncPwrEventNotification_pf;	\
	DPRX_DpRxExtFuncAUXI2CHandler_t			DpRxExtFuncAuxI2cHandler_pf;		\
	DPRX_DpRxExtFuncPWStateUpdate_t			DpRxExtFuncPWStateUpdate_pf;		\
	DP_Ext_Sha1Reset_t						Sha1Reset_pf;						\
	DP_Ext_Sha1Input_t						Sha1Input_pf;						\
	DP_Ext_Sha1Result_t						Sha1Result_pf;						\
	U8										DpcdMajorVersion_8;					\
	U8										DpcdMinorVersion_8;					\
	BOOL									HdcpProdKeys;						\
	U8  *									DpcdOui_8;							\
	DPRX_VoltageLvl_t						VoltLvl_e;							\
	DP_Ext_RXSetVoltLvl_t					DPSetVoltLvl_pf;

typedef struct
{
	RxFeSstMstCommonInit
}RxFeSstMstCommonInit_t;

typedef struct 
{
	RxFeSstMstCommonInit
		
	// base address
	DP_BaseAddr_t							BEMemBase_p;
	DP_BaseAddr_t							SdpMemBase_p;

	// features
	BOOL 									IdpUseHpd_b;				
	DPRX_AudioClockMode_t					DPAudioClockMode_e;		
	BOOL									Aux2I2Cenable_b;//TRUE if AUX2I2C should be enabled and False if AUX2OCM should be enabled.

	// callbacks
	DPRX_DpRxExtFuncAudioDetectedNotify_t	DpRxExtFuncAudioDectedNotify_pf;
	DP_Ext_DpConfigChange_t					DPConfigChange_pf;
}DPRX_SstOnlyInitParams_t;


typedef struct 
{
	RxFeSstMstCommonInit
	DP_BaseAddr_t							DpcdUpReqBase_p;
	DP_BaseAddr_t							DpcdUpRepBase_p;
	DP_BaseAddr_t							DpcdDownReqBase_p;
	DP_BaseAddr_t							DpcdDownRepBase_p;
	RxMSTBeInit
}DP12_RxMstCapableInitParams_t;

typedef struct
{

	DP_BaseAddr_t						RxBeMemBase_p;
	DP_BaseAddr_t						RxBeSDPMemBase_p;
	DPRX_AudioClockMode_t				DPAudioClockMode_e;
	DP_Ext_DpConfigChange_t				DPConfigChange_pf;	
}DPRX_DigitalTxInitParams_t;

typedef struct
{
	RxMSTBeInit	
}DPRX_DigitalRxInitParams_t;
//******************************************************************************
//  V A R I A B L E   E X T E R N S
//******************************************************************************

//******************************************************************************
//  F U N C T I O N   E X T E R N S
//******************************************************************************

//******************************************************************************
//  F U N C T I O N   P R O T O T Y P E S
//******************************************************************************

//Lib Version
void DPRX_GetVersion(DP_LibVersion_t * DpRxVersionInfo_ps);


//Handler (used by DP_Handler)
void DP_EXT_FUNC DPRX_Handler(void);

//ISR (used by DP12RX_Isr)
void DP_EXT_FUNC DPRX_Isr(void);
void DP_EXT_FUNC DPMstRX_Isr(void);


//Power
DP_PowerState_t DP_EXT_FUNC DPRX_GetPower(U8 PortId_8);
void DP_EXT_FUNC DPRX_SetPower(U8 PortId_8, DP_PowerState_t NewPowerState_e);

//test
BOOL DP_EXT_FUNC DPRX_AreInternalRegistersOK(U8 PortId_8);

//Video
BOOL DP_EXT_FUNC DPRX_IsVideoPresent(BOOL LogicalPort, BOOL VideoEnabled);

//Audio
void DP_EXT_FUNC DPRX_EnableAudio(U8 PortId_8);
void DP_EXT_FUNC DPRX_ResetAudio(U8 PortId_8);
void DP_EXT_FUNC DPRX_DisableAudio(U8 PortId_8);
void DP_EXT_FUNC DPRX_MuteAudio(U8 PortId_8, BOOL Mute_b);
DPRX_AudioStatus_t DP_EXT_FUNC DPRX_GetAudioStatus(U8 PortId_8);
void DP_EXT_FUNC DPRX_GetAudioConfig(U8 PortId_8, DP_AudioConfig_t DP_RAM * AudioConfig_ps);
BOOL DP_EXT_FUNC DPRX_GetAudioMuteState(U8 PortId_8);

//Sdp
void DPRX_ReceiveSDP(U8 PortId_8, DP_SdpInfoFrame_t * SdpFramToReceive_ps);

// MSA info

void DP_EXT_FUNC DPRX_GetVideoParameters(DP_MsaParameters_t DP_RAM * SysMsParam_ps);
U32 DP_EXT_FUNC DPRX_GetMvidN(void);
U32 DP_EXT_FUNC DPRX_GetMvidM(void);

//HDCP
void DP_EXT_FUNC DPRX_HdcpSetReadyBit(U8 PortId_8);
void DP_EXT_FUNC DPRX_HdcpSetBinfo(U8 PortId_8, U16 Binfo_16);
void DP_EXT_FUNC DPRX_HdcpSetKsvList(U8 PortId_8, const U8 DP_RAM * KsvList_p8, U8 KsvNum_b);
void DP_EXT_FUNC DPRX_HdcpSetRepeaterBit(U8 PortId_8);
void DP_EXT_FUNC DPRX_HdcpClearRepeaterBit(U8 PortId_8);


// APIs
void DP_EXT_FUNC DPRX_LinkReset(void);

//HPD
void DP_EXT_FUNC HPD_AppHpdSetLow(void);
void DP_EXT_FUNC HPD_HpdSetLow(void);
void DP_EXT_FUNC HPD_AppHpdSetHigh(void);
void DP_EXT_FUNC HPD_HpdSetHigh(void);

//Link
DP_MainLinkRate_t DP_EXT_FUNC DPRX_GetLinkRate(void);


#if DP_MULTISTREAM_SOURCE
void DP_EXT_FUNC DP_SourceStartAuthentication(void);
#endif


#pragma option -b.
#ifdef __PARADIGM__
#pragma pack(pop)
#endif
#endif
/* End of stdprxapi.h */


