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
#ifndef __DP12PORT_H__
#define __DP12PORT_H__


//******************************************************************************
//	C O M P I L E R    O P T I O N S										
//******************************************************************************
// Force byte alignment
#ifdef __PARADIGM__
#pragma pack(push, 1)
#endif
//******************************************************************************
//  C O N S T A N T   D E F I N E S
//******************************************************************************
#define PORT_SWITCH_TO 500

//******************************************************************************
//	G L O B A L    D E F I N I T I O N S
//******************************************************************************

typedef struct
{
	CommonPortStructureEntries

	DpTxMstBeInstanceData_t		*TxBeData_ps;
	DpRxSstBeInstanceData_t		*RxBeData_ps;
	
}DPTX_DigitalTransmitter_t;

typedef struct
{
	CommonPortStructureEntries
	DpTxSstBeInstanceData_t		*TxBeData_ps;
	DpRxMstBeInstanceData_t		*RxBeData_ps;
}DPRX_DigitalReceiver_t;


#define DigitalTxBeInit 			\
	DP_BaseAddr_t						RxBeMemBase_p;				\
	DP_BaseAddr_t						RxBeSDPMemBase_p;			\
    DPRX_AudioClockMode_t				DPAudioClockMode_e;\
    DP_Ext_DpConfigChange_t				DPConfigChange_pf; \

typedef struct 
{
	TxBeCommonInit
	DigitalTxInit
	DigitalTxBeInit

} DP12_DigitalTxInitParams_t;

typedef struct 
{
  //	RxMSTBeInit
	TxBeCommonInit
	DP_BaseAddr_t						SDPMemBase_p;  
} DP12_DigitalRxInitParams_t;

typedef struct 
{
	TxFeSstMstCommonInit
	TxBeCommonInit
	DigitalTxInit 
	DigitalTxBeInit  
} DP12_TxMstAndDigitalInitParams_t;

typedef enum
{
	DP12RX0_RL_CONFIG_ADDRESS,
	DP12RX1_RL_CONFIG_ADDRESS,		
	DP12RX2_RL_CONFIG_ADDRESS,	
	DP12RX3_RL_CONFIG_ADDRESS,	
	DP12TX0_RL_CONFIG_ADDRESS,
	DP12TX1_RL_CONFIG_ADDRESS,
	DP12TX2_RL_CONFIG_ADDRESS,
	DP12TX3_RL_CONFIG_ADDRESS

}DP_RLBaseAddress_t;

extern PortStructure_t *	DP_PortList;
//******************************************************************************
//	G L O B A L    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
PortStructure_t * DP_EXT_FUNC DP_GetPortList (void);
extern DP_BaseAddr_t DP_EXT_FUNC DP_GetBeBaseAddress (DP_RLBaseAddress_t BeAddress_e);
PortStructure_t * DP_EXT_FUNC DP_GetRxInstanceByPortId(U8 PortId_8);
PortStructure_t * DP_EXT_FUNC DP12_SetCurrentRxByPortId(U8 PortId_8);
PortStructure_t * DP_EXT_FUNC FindPortWithBeBaseAddress(DP_BaseAddr_t BaseAddress_p);
PortStructure_t * DP_EXT_FUNC DP12_GetPortStructureByPortId(U8 PortId_8);

DP_BaseAddr_t DP_EXT_FUNC DP_GetBeBaseAddress (DP_RLBaseAddress_t BeAddress_e);
DP_PortType_t DP_EXT_FUNC GetPortType (PortStructure_t** PortStructure_ps);

void DP_EXT_FUNC DP_SetTxInstanceData(void);
void DP_EXT_FUNC DP_SetRxInstanceData(void);
void DP_EXT_FUNC DPTX_MstCapablePortHandler (void);
void DP_EXT_FUNC DPRX_MstCapablePortHandler (void);
PortStructure_t * DP_EXT_FUNC DP_GetRxInstanceByPortId(U8 PortId_8);
PortStructure_t * DP_EXT_FUNC DP12_SetCurrentRxByPortId(U8 PortId_8);
void DP_EXT_FUNC DP12_InitRxMstPort(DP12_RxMstCapableInitParams_t far* InitParams_ps);
void DP_EXT_FUNC DP12_InitRxSstPort(DPRX_SstOnlyInitParams_t far* InitParams_ps);
void DP_EXT_FUNC DP12_InitLogicalPort(DP12_DigitalTxInitParams_t far* InitParams_ps);
void DP_EXT_FUNC DP12_InitDigitalTransmitter(DP12_DigitalTxInitParams_t far* InitParams_ps);
void DP_EXT_FUNC DPTX_DigitalTxHandler (void);
void DP_EXT_FUNC DPTX_MstCapablePortHandler (void);
void DP_EXT_FUNC DPTX_SstOnlyPortHandler (void);
void DP_EXT_FUNC DPRX_SstOnlyPortHandler (void);
void DP_EXT_FUNC DPRX_Init(void far* DP_PortInitParams_ps);
void DP_EXT_FUNC DP12_SetTxActivePort(U8 PortIdToDeactivate_8, U8 PortIdToActivate_8);
void DP_EXT_FUNC DP12_TxPortSwitchHandler (void);
DP_PortType_t DP_EXT_FUNC GetPortType (PortStructure_t** PortStructure_ps);
void DP_EXT_FUNC DP12_DeactivateMstSstTxPorts(U8 PortIdToDeactivate_8);
void DP_EXT_FUNC DP12_ActivateSstTxPort(U8 PortIdToActivate_8);
void DP_EXT_FUNC DP12_DeactivateSstTxPort(U8 PortIdToDeactivate_8);

U8 DP_EXT_FUNC DP12_GetNumberVirtualChannelsAllocated(U8 PortId_8);

#ifdef __PARADIGM__
#pragma pack(pop)
#endif

#endif	/*__DP12PORT_H__*/

