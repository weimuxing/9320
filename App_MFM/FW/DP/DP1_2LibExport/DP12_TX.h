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

#ifndef __DP12_TX_INCLUDED__
#define __DP12_TX_INCLUDED__
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
//  T Y P E D E F S   ( F U N C T I O N   P O I N T E R S )
//******************************************************************************

//******************************************************************************
//  T Y P E D E F S   ( S T R U C T S )
//******************************************************************************
typedef struct DPTX_SstOnly_S
{
	CommonPortStructureEntries
	
	DpTxSstFeInstanceData_t		*FeData_ps;
	
	DpTxSstBeInstanceData_t		*BeData_ps;
	
}DPTX_SstOnly_t;

typedef struct DPTX_MstCapable_S
{
	CommonPortStructureEntries

	DpTxMstFeInstanceData_t		*FeData_ps;
	
	DpTxMstBeInstanceData_t		*BeData_ps;
	
}DPTX_MstCapable_t;



//******************************************************************************
//  V A R I A B L E   E X T E R N S
//******************************************************************************

extern PortStructure_t  *		DPTX_UTxNode_ps;
extern PortStructure_t 	*       DPTX_UTXNodeOld_ps;

extern DpTxFeSstMstCommonData_t *TxMstSstFeCommonData_ps;

extern DpTxSstFeInstanceData_t *TxSstFeData_ps;
extern DpTxMstFeInstanceData_t *TxMstFeData_ps;
extern DpTxSstBeInstanceData_t *TxSstBeData_ps;
extern DpTxMstBeInstanceData_t *TxMstBeData_ps;
extern DpTxMstBePortSpecificInstanceData_t	*TxMstBePortSpecificData_ps;

//******************************************************************************
//  F U N C T I O N   E X T E R N S
//******************************************************************************
void DP_EXT_FUNC TX_SetPower (DP_PowerState_t  NewPowerState_e);
PortStructure_t * EXT_FUNC DP12_SetCurrentTxByPortId(U8 Portid);
PortStructure_t * EXT_FUNC DP12_SetCurrentTxByPortNumber(U8 PortNumber);
void DP_EXT_FUNC DP_SetTxInstanceData (void);
U8 DP_EXT_FUNC DP12_GetTxNumber(U8 PortNumber);
PortStructure_t * DP_EXT_FUNC DP12_SetCurrentTxByRoutingLogicPortNum(U8 HardwareMap_8);
PortStructure_t * DP_EXT_FUNC DP12_SetCurrentRxByRoutingLogicPortNum(U8 HardwareMap_8);
DP_CablePlugStatus_t DP_INT_FUNC DPTX_GetCableStatusIntFn (void);
PortStructure_t * DP_EXT_FUNC DP_GetPortList (void);
void DP_EXT_FUNC DP12_InitTxSstPort(DPTX_SstOnlyInitParams_t far* InitParams_ps);
void DP_EXT_FUNC DP12_InitTxMstPort(DP12_TxMstInitParams_t far* InitParams_ps);
BOOL DP_EXT_FUNC Tx_Fe_HdcpGetKsvList(U8 DP_RAM* Buffer_p8);
void DP_EXT_FUNC DPTX_MstSstDynamicInit(void);
//******************************************************************************
//  F U N C T I O N   P R O T O T Y P E S
//******************************************************************************
#pragma option -b.
#ifdef __PARADIGM__
#pragma pack(pop)
#endif
#endif		// __DP12_TX_INCLUDED__

