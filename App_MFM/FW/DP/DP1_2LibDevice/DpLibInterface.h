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

#ifndef __DPLIBINTERFACE_H__
#define __DPLIBINTERFACE_H__
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
#include "HDCP\HdcpRepeaterLibExport\HdcpRepeaterBasicInclude.h"
#include "HDCP\HdcpRepeaterLibExport\HdcpRepeater.h"
#include "DP\DP1_2LibExport\DP12BasicInclude.h"
#include "DP\DP1_2LibExport\stdp.h"
#include "DP\DP1_2LibExport\stdprx.h"
#include "DP\DP1_2LibExport\stdptx.h"
#include "DP\DP1_2LibExport\stdpsbm.h"
#include "DP\DP1_2LibExport\stdpbu.h"
#include "DP\DP1_2LibExport\DpRxLib.h"
#include "DP\DP1_2LibExport\DpTxLib.h"
#include "DP\DP1_2LibExport\DP12_Rx.h"
#include "DP\DP1_2LibExport\DP12_Port.h"

#if 1
#include "Board\DisplayPortConfig.h"
#else
//******************************************************************************
//  C O N S T A N T   D E F I N E S
//******************************************************************************



#define DP_NUM_SST_ONLY_RX 					0
#define DP_NUM_SST_ONLY_TX	   				0		

/**********************
** ROUTING LOGIC
***********************/
#define DP_RL_NUM_UPSTRM_PORTS				1
#define DP_RL_NUM_DOWNSTREAM_PHYSICAL_PORTS	1
#define DP_RL_NUM_DOWNSTREAM_LOGICAL_PORTS	1

#define DP_RL_NUM_DWNSTRM_PORTS				((DP_RL_NUM_DOWNSTREAM_PHYSICAL_PORTS) + (DP_RL_NUM_DOWNSTREAM_LOGICAL_PORTS))

#define DP_TOTAL_NUM_DWNSTRM_PORTS			((DP_RL_NUM_DWNSTRM_PORTS) + (DP_NUM_SST_ONLY_TX))
#define DP_TOTAL_NUM_UPSTRM_PORTS			((DP_RL_NUM_UPSTRM_PORTS) + (DP_NUM_SST_ONLY_RX))
#define DP_TOTAL_NUM_PORTS					((DP_TOTAL_NUM_DWNSTRM_PORTS) + (DP_TOTAL_NUM_UPSTRM_PORTS))
#endif


//******************************************************************************
//  M A C R O   D E F I N E S
//******************************************************************************
//#define DP_HUB_2_LOG_PORTS

//#define DP_HUB_2_LOG_PORTS_2nd_Athena DISABLE

//#if (DP_HUB_2_LOG_PORTS_2nd_Athena == ENABLE)
//#define SetDpRx0_As_DP1_1                    1
//#endif

#define PCLK_VALUE_FROM_AVDDS             0			//Can be set to 0 since PCLK 
                                                    //reading from AVDDS is moved
                                                    //to modeDetermine in 120710

//******************************************************************************
//  T Y P E D E F S 
//******************************************************************************

//******************************************************************************
//  T Y P E D E F S   ( E N U M S )
//******************************************************************************

//******************************************************************************
//  T Y P E D E F S   ( F U N C T I O N   P O I N T E R S )
//******************************************************************************

//******************************************************************************
//  T Y P E D E F S   ( S T R U C T S )
//******************************************************************************

//******************************************************************************
//  E N U M S
//******************************************************************************

enum
{
	SSTLegacyRxPort		= 0,
	MstRxPort,
	LogicalTxPort,
	MstTxPort,
	iDpTxPort
};

//******************************************************************************
//  V A R I A B L E   E X T E R N S
//******************************************************************************

//******************************************************************************
//  F U N C T I O N   E X T E R N S
//******************************************************************************

//******************************************************************************
//  F U N C T I O N   P R O T O T Y P E S
//******************************************************************************

void DP_EXT_FUNC DP_Init(DP_PortInitParams_t far* DP_PortInitParams_ps);
void DP_EXT_FUNC DP_ConfigLib (DP_IpVersion_t DP_IpVersion_e);
void DP_EXT_FUNC DP_SetRxPower(U8 RxPortId_8, DP_PowerState_t NewPowerState_e);
void DP_EXT_FUNC DP_SetTxPower(U8 TxPortId_8, DP_PowerState_t NewPowerState_e);
void DP_EXT_FUNC DP_Handler(void);
void DP_EXT_FUNC DP_Interrupt(void);
extern void DP_EXT_FUNC AddPortStructureToPortList(PortStructure_t ** List, PortStructure_t * NewItem);
extern void DP_EXT_FUNC DP12_InitRoutingLogic (void);
void DP_EXT_FUNC DP12RX_Isr(void);
void DP_EXT_FUNC DPRX_BE_Isr(void);
void DP_EXT_FUNC DP12TX_Isr(void);
void DP_EXT_FUNC DP12RL_Isr (void);
void DP_EXT_FUNC DPBU_GtcInit(void);
void DP_EXT_FUNC DP_GtcInit (DP_PortType_t PortType);
void DP_EXT_FUNC DP_GetRxVideoParameters(U8 PortId_8, DP_MsaParameters_t DP_RAM * MsaParam_ps);
BOOL DP_EXT_FUNC DP_IsRxVideoPresent(U8 PortId_8);
void DP_EXT_FUNC DP_ResetRxLink(U8 PortId_8);
void DP_EXT_FUNC DP_SetHpdHigh(U8 PortId_8);
void DP_EXT_FUNC DP_SetHpdLow(U8 PortId_8);
BOOL DP_EXT_FUNC DP_WriteGUID(U8 FAR* Guid_Val_p,U8 Num_bytes_8);
BOOL DP_EXT_FUNC DP_IsTxActive(U8 PortId_8);
BOOL DP_EXT_FUNC DP_IsTxHpdHigh(void);
STDP_3DFormat_t DP_EXT_FUNC DP_GetRx3DFormat(U8 Port_ID);
DPRXLSMStatus_t DP_EXT_FUNC DP_GetLinkStatus(U8 PortId_8);
void DP_EXT_FUNC DP_SendVSCPacket(U8 Port_ID, STDP_3DFormat_t ThreeDFormat);
void DP_EXT_FUNC DP_SetLeftRight(U8 Port_ID, STDP_3DView_t ThreeDView);



#pragma option -b.
#ifdef __PARADIGM__
#pragma pack(pop)
#endif
#endif	/*__STDPBUAPI_H__*/
