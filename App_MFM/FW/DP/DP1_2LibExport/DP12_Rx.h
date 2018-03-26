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

#ifndef __DP12_RX_INCLUDED__
#define __DP12_RX_INCLUDED__

//******************************************************************************
//	C O M P I L E R    O P T I O N S										
//******************************************************************************
// Force byte alignment
#ifdef __PARADIGM__
#pragma pack(push, 1)
#endif
//******************************************************************************
//  I N C L U D E   F I L E S
//******************************************************************************

//******************************************************************************
//  C O N S T A N T   D E F I N E S
//******************************************************************************
#pragma option -b- //as a byte


//******************************************************************************
//  M A C R O   D E F I N E S
//******************************************************************************

//******************************************************************************
//  T Y P E D E F S 
//******************************************************************************

//******************************************************************************
//  T Y P E D E F S   ( E N U M S )
//******************************************************************************
/* This enum is used for bitwise operation for the micropacket RX member ConnectedTxCapabilities */
typedef enum
{
	ConnectedTxCapabilites_MSTEn 		= DP_BIT0,
	ConnectedTxCapabilites_UpReqEn 		= DP_BIT1,
	ConnectedTxCapabilites_Connected	= DP_BIT2,
	ConnectedTxCapabilites_linksuccess	= DP_BIT3,
	ConnectedTxCapabilites_UnplugPlug   = DP_BIT4,
	ConnectedTxLastBitPos				= ConnectedTxCapabilites_UnplugPlug
}ConnectedTxCapabilites_e;
//******************************************************************************
//  T Y P E D E F S   ( F U N C T I O N   P O I N T E R S )
//******************************************************************************

//******************************************************************************
//  T Y P E D E F S   ( S T R U C T S )
//******************************************************************************

typedef struct
{
	CommonPortStructureEntries

	DpRxSstFeInstanceData_t		*FeData_ps;
	DpRxSstBeInstanceData_t		*BeData_ps;

}DPRX_SstOnly_t;

typedef struct
{
	CommonPortStructureEntries		
	DpRxMstFeInstanceData_t		*FeData_ps;
	DpRxMstBeInstanceData_t		*BeData_ps;

}DPRX_MstCapable_t;

//******************************************************************************
//  V A R I A B L E   E X T E R N S
//******************************************************************************

extern PortStructure_t *		DPRX_URxNode_ps;
extern PortStructure_t 	*       DPRX_URXNodeOld_ps;

extern DpRxFeSstMstCommonData_t *RxMstSstFeCommonData_ps;

extern DpRxSstFeInstanceData_t *RxSstFeData_ps;
extern DpRxMstFeInstanceData_t *RxMstFeData_ps;
extern DpRxSstBeInstanceData_t *RxSstBeData_ps;
extern DpRxMstBeInstanceData_t *RxMstBeData_ps;

//******************************************************************************
//  F U N C T I O N   E X T E R N S
//******************************************************************************

//******************************************************************************
//  F U N C T I O N   P R O T O T Y P E S
//******************************************************************************
void BU_Sbm_Handler(void);
void DP_EXT_FUNC RXSBM_SendDownReplyMsgs(void);
void SB_ProcessRxedDownReqMsg(void);
void SB_ProcessForwardMsgs(void);
PortStructure_t * DP_EXT_FUNC DP12_SetCurrentRxByPortNumber(U8 PortNumber);
PortStructure_t * DP_EXT_FUNC DP_GetPortList (void);
void FAR AdjustRad(U8  PortNum, U8 FAR * RAD, U8 SB_Lct_Lcr_8);
U8 SB_FindTxVidFromRxVidTxPort(U8 RxVid_8, U8 TxPortNum_8);
void DP_EXT_FUNC DPRX_MstSstDynamicInit(void);
BOOL DP_EXT_FUNC DP_RX_FE_WriteGUID(U8 FAR* Guid_Val_p);
#pragma option -b.
#ifdef __PARADIGM__
#pragma pack(pop)
#endif
#endif // __DP12_RX_INCLUDED__

