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

/* Define to prevent recursive inclusion */
#ifndef __STDPTX_H__
#define __STDPTX_H__
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

#define SUPPORT_TX_HDCP			1

//******************************************************************************
//  M A C R O   D E F I N E S
//******************************************************************************

//******************************************************************************
//  T Y P E D E F S 
//******************************************************************************

//******************************************************************************
//  T Y P E D E F S   ( E N U M S )
//******************************************************************************



//******************************************************************************
//  T Y P E D E F S   ( S T R U C T S )
//******************************************************************************



//******************************************************************************
//  T Y P E D E F S   ( F U N C T I O N   P O I N T E R S )
//******************************************************************************

typedef void far (* DPTX_SetExtClkPower_t)(U8 PortId_8,BOOL PowerUp_b);

typedef void far (* DPTX_SetExtClkSpreadSpectrum_t)(BOOL Enable_b);

typedef U8 far (* DPTX_Ext_GetHardwareVersion_t)(void);

typedef U8 far (* DPTX_Ext_GetFirmwareVersion_t)(void);

typedef void far (*DPTX_Ext_LinkAutomationPatGen_t)(U8 PortId_8,DP_MsaParameters_t DP_RAM * LinkAutomationDpcdMsaParams_ps);

typedef void far (*DPTX_Ext_ExecuteLogicalPortI2c_t)(U8 PortId_8, Aux_data_buffer_t * I2cTransaction, U8 stopbit);

typedef U8 far (* DPTX_Ext_GetHpdStatus_t)(void);
//******************************************************************************
//  T Y P E D E F S   ( S T R U C T S )
//******************************************************************************


typedef struct
{
	U8					Number_SDP_Stream_Sinks1		: 4;
	U8					Number_SDP_Streams1				: 4;
} SdpStreams_t;

#define TxFeSstMstCommonInit 						                   \
	DP_BaseAddr_t						FeMemBase_p;				   \
	DP_BaseAddr_t						PhyMemBase_p;				   \
	DP_BaseAddr_t						KeyHostReg_p;				   \
	DP_Standard_t						Standard_e;				       \
	DP_MainLinkRate_t					LinkRate_e; 				   \
	DP_MainLinkNumLanes_t				LaneCount_e;				   \
	BOOL								AutoCalibEnable_b;			   \
	U8									Gain_8;						   \
	U8									PreEmp_8;					   \
	DPTX_SetExtClkPower_t				SetExtClkPower_pf;			   \
	DPTX_SetExtClkSpreadSpectrum_t 		SetExtClkSpreadSpectrum_pf;	   \
	DP_Ext_Sha1Reset_t					Sha1Reset_pf;                  \
	DP_Ext_Sha1Input_t					Sha1Input_pf;                  \
	DP_Ext_Sha1Result_t					Sha1Result_pf;				   \
	DPTX_Ext_GetHardwareVersion_t		GetHardwareVersion_pf;		   \
	DPTX_Ext_GetFirmwareVersion_t		GetFirmwareVersion_pf;         \
	BOOL							    HdcpProdKeys;                  \
	DPTX_Ext_GetHpdStatus_t				GetHpdStatus_pf;         	   \

#define TxBeCommonInit 			\
	DP_BaseAddr_t						BeMemBase_p;				\
	DP_BaseAddr_t						PatGenMemBase_p;			\
	DPTX_Ext_LinkAutomationPatGen_t		LinkAutomationPatGen_pf;    \
	DPTX_Ext_ExecuteLogicalPortI2c_t	ExecuteLogicalPortI2c_pf;	\


#define DigitalTxInit 			\
	SdpStreams_t						SdpStreamsInfo;				\
	DPTX_SetExtClkPower_t				DigitalTx_SetExtClkPower_pf;			\
	DPTX_SetExtClkSpreadSpectrum_t 		DigitalTx_SetExtClkSpreadSpectrum_pf;	\
	DPTX_Ext_LinkAutomationPatGen_t		DigitalTx_LinkAutomationPatGen_pf;	\
	DP_Ext_GetLogicalPortBksv_t			DPGetLogicalPortBksv_pf;	\
   	
typedef struct
{
	TxFeSstMstCommonInit
}TxFeSstMstCommonInit_t;

typedef struct 
{
	TxFeSstMstCommonInit
	TxBeCommonInit
	DP_BaseAddr_t						SDPMemBase_p;
	//Features
	BOOL 								IdpHdpMode_b;
} DPTX_SstOnlyInitParams_t;


typedef struct
{
	TxFeSstMstCommonInit
	TxBeCommonInit
} DP12_TxMstInitParams_t;

typedef struct
{
	TxBeCommonInit
	DigitalTxInit	
} DPTX_LogicalInitParams_t;

typedef  DPTX_LogicalInitParams_t DPTX_DigitalTxInitParams_t;

typedef struct
{
	TxBeCommonInit
	DP_BaseAddr_t						SDPMemBase_p;  	
} DPTX_DigitalRxInitParams_t;

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
void DP_EXT_FUNC DPTX_GetVersion(DP_LibVersion_t * DpTxVersionInfo_ps);



//Handler
void DP_EXT_FUNC DPTX_Handler(void);


//ISR
void DP_EXT_FUNC DPTX_Isr(void);
void DP_EXT_FUNC DPTX_RLIsr (void);

//HPD
DP_CablePlugStatus_t DP_EXT_FUNC DPTX_GetCableStatus(U8 PortId_8);

//DPTX MS
void DP_EXT_FUNC DPTX_EnableVideo(U8 PortId_8);
void DP_EXT_FUNC DPTX_DisableVideo(U8 PortId_8);
void DP_EXT_FUNC DPTX_SetVideoParameters(U8 PortId_8, DP_MsaParameters_t DP_RAM* MsaParams_ps );
void DP_EXT_FUNC DPTX_GetVideoParameters(U8 PortId_8, DP_MsaParameters_t DP_RAM * MsaParams_ps);

//I2C2Aux
BOOL DP_EXT_FUNC  DPTX_EnableI2COverAuxDeviceId(U8 PortId_8, U8 I2CSlaveDeviceId_8 );
BOOL DP_EXT_FUNC  DPTX_DisableI2COverAuxDeviceId(U8 PortId_8, U8 I2CSlaveDeviceId_8 );


//Audio
void DP_EXT_FUNC DPTX_EnableAudio(U8 PortId_8);
void DP_EXT_FUNC DPTX_DisableAudio(U8 PortId_8);
void DP_EXT_FUNC DPTX_MuteAudio(U8 PortId_8, BOOL B_Mute_b);

void DP_EXT_FUNC DPTX_SetAudioConfig(U8 PortId_8,DP_AudioConfig_t DP_RAM* AudioConfig_ps);
void DP_EXT_FUNC DPTX_GetAudioConfig(U8 PortId_8,DP_AudioConfig_t DP_RAM* AudioConfig_ps);

//Aux
#if DP_MULTISTREAM_SOURCE
BOOL DP_EXT_FUNC DPTX_Fe_AuxDpcdRead(U8 PortId_u8, U32 Addr_32, U8 far * InOutBuff_p8, U16 InLen_16);
BOOL DP_EXT_FUNC DPTX_Fe_AuxDpcdWrite(U8 PortId_u8, U32 Addr_32, U8 far * InOutBuff_p8, U16 InLen_16);
BOOL DP_EXT_FUNC DPTX_Fe_AuxDpcdWriteByte(U8 PortId_u8, U32 Addr_32, U8 DP_RAM OutBuff_8);
U8 DP_EXT_FUNC DpTx_PatgenAllocatePayload (U8 RxedId_8,U16 far* RequestedPbn_p16);
void DP_EXT_FUNC DP_SourceEnableEncryption (U32 Ecf_0,U32 Ecf_1);
#endif

//SDP
void DP_EXT_FUNC DPTX_TransmitSDPInfoFrames (U8 PortId_8, DP_SdpInfoFrame_t FAR * InfoFrame_ps);

//HDCP  TBD


//DPTX Status
BOOL DP_EXT_FUNC  DPTX_IsLinkReady (U8 PortId_8);
void DP_EXT_FUNC  DPTX_GetLinkConfig (U8 PortId_8, DP_MainLinkRate_t * LinkRate_e, DP_MainLinkNumLanes_t* LaneCount_e);


//Register Test
BOOL DP_EXT_FUNC DPTX_AreInternalRegistersOK (U8 PortId_8);


//PhyAutomation
BOOL DP_EXT_FUNC DPTX_IsPhyAutomationInProgress(U8 PortId_u8);


#pragma option -b.
#ifdef __PARADIGM__
#pragma pack(pop)
#endif
#endif
/* End of stdptx.h */

