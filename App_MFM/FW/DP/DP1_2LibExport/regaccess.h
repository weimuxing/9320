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

#ifndef __REGACCESS_H__
#define __REGACCESS_H__
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

#define  Dp12UpReqOffset			((U32)(RxMstFeData_ps->DpcdUpReqBase_p))
#define  Dp12UpReplyOffset			((U32)(RxMstFeData_ps->DpcdUpRepBase_p))
#define  Dp12DownReqOffset 			((U32)(RxMstFeData_ps->DpcdDownReqBase_p))
#define  Dp12DownReplyOffset		((U32)(RxMstFeData_ps->DpcdDownRepBase_p))

#define  Dp12BridgeTopOffset		((U32)(DPRL_RlParam_ps->BridgeTopRegMemOffset_p))
#define  Dp12RLTopOffset			((U32)(DPRL_RlParam_ps->RoutingLogicTopRegMemOffset_p))

#define  DPTX_FE_REG_MEM_BASE		((U16)(TxMstSstFeCommonData_ps->FeMemBase_p))
#define  DPTX_PHY_REG_MEM_BASE		((U16)(TxMstSstFeCommonData_ps->PhyMemBase_p))

#define  DPTX_BE_REG_MEM_BASE	    ((U16)(TxSstBeData_ps->BeMemBase_p))
#define  DPTX_SDP_REG_MEM_BASE		((U16)(TxSstBeData_ps->SDPMemBase_p))
#define  DPTX_PATGEN_REG_MEM_BASE	((U16)(TxSstBeData_ps->PatGenMemBase_p))

#define  DPTX_MST_BE_REG_MEM_BASE   ((U16)(TxMstBeData_ps->BeMemBase_p))

#define  DpRxFeRegMemBase			((U16)(RxMstSstFeCommonData_ps->FEMemBase_p))
#define  DpRxBeRegMemBase			((U16)(RxSstBeData_ps->BEMemBase_p))
#define  DpRxMstBeRegMemBase		((U16)(RxMstBeData_ps->BEMemBase_p))
#define  DpRxComboPhyRegMemBase		((U16)(RxMstSstFeCommonData_ps->PhyMemBase_p))
#define  DpRxSDPRegMemBase			((U32)(RxSstBeData_ps->SdpMemBase_p))
#define  DpRxDPCDRegMemBase			((U32)(RxMstSstFeCommonData_ps->DpcdMemBase_p))
#define  DpRxGTCRegMemBase			((U16)(RxMstBeData_ps->GTCMemBase_p))

#define  DpRxRLIpOffset				(DpRxMstBeRegMemBase)
#define  DpTxRLIpOffset				(DPTX_MST_BE_REG_MEM_BASE)
#define  DpTxPatIpOffset			(DPTX_PATGEN_REG_MEM_BASE)
#define  DpRLIpOffset				((U16)(Dp12RLTopOffset))

#define  DPRxFeIpOffset				(DpRxFeRegMemBase)
#define  DpDpcdOffset				(DpRxDPCDRegMemBase)
#define  DpRxSdpMemBase				(DpRxSDPRegMemBase)

//******************************************************************************
//  M A C R O   D E F I N E S
//******************************************************************************
#define MEMORY_READ_PATCH
#define READ_REGX_U8(RegAddr_16, Offset_16)		(*(((U8 volatile * volatile)RegAddr_16) + (Offset_16)))

#ifdef MEMORY_READ_PATCH
#define READ_REGX_U16(RegAddr_16, Offset_16)		(READ_REGX_U8(RegAddr_16, Offset_16) | ((U16)READ_REGX_U8(RegAddr_16, Offset_16 + 1) << 8))

#define READ_REG_U32(RegAddr_16)					(READ_REGX_U16(RegAddr_16, 0) | ((U32)READ_REGX_U16(RegAddr_16 + 2, 0)	<< 16))	//(*(U32 volatile *)(RegAddr_16))

#define READ_REG_U24(RegAddr_16)					(READ_REGX_U16(RegAddr_16, 0) | ((U32)READ_REGX_U8(RegAddr_16, 2) << 16))
#else
#define READ_REG_U32(RegAddr_16)					(*(U32 volatile *)(RegAddr_16))

#define READ_REG_U24(RegAddr_16)					(READ_REG_U32((RegAddr_16)) & 0x00ffffffUL)
#define READ_REGX_U16(RegAddr_16, Offset_16)		(*(U16 volatile * volatile)((RegAddr_16) + (Offset_16)))
#endif

#define READ_REG0_U16(RegAddr_16)					READ_REGX_U16((RegAddr_16), (0))

#define READ_REG1_U16(RegAddr_16)					READ_REGX_U16((RegAddr_16), (2))

#define READ_REG0_U8(RegAddr_16)					READ_REGX_U8((RegAddr_16), (0))

#define READ_REG1_U8(RegAddr_16)					READ_REGX_U8((RegAddr_16), (1))

#define READ_REG2_U8(RegAddr_16)					READ_REGX_U8((RegAddr_16), (2))

#define READ_REG3_U8(RegAddr_16)					READ_REGX_U8((RegAddr_16), (3))


#define WRITE_REG_U32(RegAddr_16, RegValue_32)	(*(U32 volatile * volatile)(RegAddr_16) = (U32)(RegValue_32))

#define WRITE_REG_U24(RegAddr_16, RegValue_32)	WRITE_REG0_U16((RegAddr_16), (RegValue_32));           \
													WRITE_REG2_U8((RegAddr_16), (RegValue_32))

#define WRITE_REGX_U16(RegAddr_16, Offset_16, RegValue_16)			\
							(*(U16 volatile * volatile)((RegAddr_16) + (Offset_16)) = (U16)(RegValue_16))

#define WRITE_REG0_U16(RegAddr_16, RegValue_32)	WRITE_REGX_U16((RegAddr_16), (0), ((RegValue_32) >>  0))

#define WRITE_REG1_U16(RegAddr_16, RegValue_32)	WRITE_REGX_U16((RegAddr_16), (1), ((RegValue_32) >> 16))

#define WRITE_REGX_U8(RegAddr_16, Offset_16, RegValue_8)				\
							(*(U8 volatile * volatile)((RegAddr_16) + (Offset_16)) = (U8)(RegValue_8))

#define WRITE_REG0_U8(RegAddr_16, RegValue_32)		WRITE_REGX_U8((RegAddr_16), (0), ((RegValue_32) >>  0))

#define WRITE_REG1_U8(RegAddr_16, RegValue_32)		WRITE_REGX_U8((RegAddr_16), (1), ((RegValue_32) >>  8))

#define WRITE_REG2_U8(RegAddr_16, RegValue_32)		WRITE_REGX_U8((RegAddr_16), (2), ((RegValue_32) >> 16))

#define WRITE_REG3_U8(RegAddr_16, RegValue_32)		WRITE_REGX_U8((RegAddr_16), (3), ((RegValue_32) >> 24))


#define SET_REG_BITS_U32(RegAddr_16, RegValue_32)					\
					WRITE_REG_U32((RegAddr_16), (U32 volatile)(READ_REG_U32((RegAddr_16)) | (U32 volatile)(RegValue_32)))

#define CLEAR_REG_BITS_U32(RegAddr_16, RegValue_32)					\
					WRITE_REG_U32((RegAddr_16), (U32 volatile)(READ_REG_U32((RegAddr_16)) & (~(U32 volatile)(RegValue_32))))


#define CLEAR_AND_SET_BITS_U32(RegAddr_16, AndData_32, OrData_32)			\
					WRITE_REG_U32((RegAddr_16), (READ_REG_U32((RegAddr_16)) & (~(U32 volatile)(AndData_32))) | ((U32 volatile)(OrData_32)))

// Far register access
#define READ_REG_U8Far(DW_RegBase, W_RegOffs) \
    (*((U8 FAR volatile*)(DW_RegBase) + (U16)(W_RegOffs)))

#define WRITE_REG_U16Far(DW_RegBase, W_RegOffs, W_RegValue) \
    (* (U16 FAR volatile*)((U8 FAR *)(DW_RegBase) + (U16)(W_RegOffs)) = (U16)(W_RegValue))

#ifdef MEMORY_READ_PATCH
#define READ_REG_U16Far(DW_RegBase, W_RegOffs) \
    (READ_REG_U8Far(DW_RegBase, W_RegOffs) | (READ_REG_U8Far(DW_RegBase, (W_RegOffs) + 1) << 8))
#else
#define READ_REG_U16Far(DW_RegBase, W_RegOffs) \
    (* (U16 FAR volatile*)((U8 FAR *)(DW_RegBase) + (U16)(W_RegOffs)))
#endif



#define WRITE_REG_U8Far(DW_RegBase, W_RegOffs, B_RegValue) \
    (*((U8 FAR volatile*)(DW_RegBase) + (U16)(W_RegOffs)) = (U8)(B_RegValue))

#define SET_REG_BITS_U8Far(DW_RegBase, W_RegOffs, B_RegValue) \
    WRITE_REG_U8Far(DW_RegBase, W_RegOffs, READ_REG_U8Far(DW_RegBase, W_RegOffs) | (U8)(B_RegValue))

#define CLEAR_REG_BITS_U8Far(DW_RegBase, W_RegOffs, B_RegValue) \
    WRITE_REG_U8Far(DW_RegBase, W_RegOffs, READ_REG_U8Far(DW_RegBase, W_RegOffs) & ~(U8)(B_RegValue))

#define SET_REG_BITS_U16Far(DW_RegBase, W_RegOffs, W_RegValue) \
    WRITE_REG_U16Far(DW_RegBase, W_RegOffs, READ_REG_U16Far(DW_RegBase, W_RegOffs) | (U16)(W_RegValue))

#define CLEAR_REG_BITS_U16Far(DW_RegBase, W_RegOffs, W_RegValue) \
    WRITE_REG_U16Far(DW_RegBase, W_RegOffs, READ_REG_U16Far(DW_RegBase, W_RegOffs) & ~(U16)(W_RegValue))

#define CLEAR_AND_SET_BITS_U8Far(DW_RegBase, W_RegOffs, B_AndData, B_OrData) \
    WRITE_REG_U8Far(DW_RegBase, W_RegOffs, (READ_REG_U8Far(DW_RegBase, W_RegOffs) & ~(U8)(B_AndData)) | (U8)(B_OrData))

#define CLEAR_AND_SET_BITS_U16Far(DW_RegBase, W_RegOffs, W_AndData, W_OrData) \
    WRITE_REG_U16Far(DW_RegBase, W_RegOffs, (READ_REG_U16Far(DW_RegBase, W_RegOffs) & ~(U16)(W_AndData)) | (U16)(W_OrData))


// DP TX SDP memory access
#define DPTX_SDP_INDEX_BASE 1
#define DPTX_SDP_HEADER_SIZE  0x04

#define SDP_WRITE_U8(ByteAddr_16, Value_8) \
    WRITE_REG_U8Far (TxSstBeData_ps->SDPMemBase_p, ByteAddr_16, Value_8)
    
#define SDP_WRITE_U16(ByteAddr_16, Value_16) \
    WRITE_REG_U16Far (TxSstBeData_ps->SDPMemBase_p, ByteAddr_16, Value_16)
    
#define DPTX_SDP_WRITE_HEADER_BYTE(n, v) SDP_WRITE_U8(n, v)
#define DPTX_SDP_WRITE_BODY_BYTE(n, v)   SDP_WRITE_U8((n) + DPTX_SDP_HEADER_SIZE, v)


//DP RX SDP memory access
#define DPRX_SDP_HEADER_SIZE  0x04

#define SDP_RX_READ_U8(W_RegAddr) \
    READ_REG_U8Far(DpRxSdpMemBase, W_RegAddr)

#define DPRX_SDP_HEADER_BYTE(n) SDP_RX_READ_U8(n) 
#define DPRX_SDP_BODY_BYTE(n) 	SDP_RX_READ_U8(DPRX_SDP_HEADER_SIZE+n)			



// DPCD Read and Write Macros. 

#define DPCD_WRITE_U8(W_RegAddr, B_RegValue) \
    WRITE_REG_U8Far(DpDpcdOffset, W_RegAddr, B_RegValue)

#define DPCD_READ_U8(W_RegAddr) \
    READ_REG_U8Far(DpDpcdOffset, W_RegAddr)

#define DPCD_SET_U8(W_RegAddr, B_RegValue) \
    SET_REG_BITS_U8Far(DpDpcdOffset, W_RegAddr, B_RegValue)

#define DPCD_CLEAR_U8(DPCDAddr_16,Value_8) \
			CLEAR_REG_BITS_U8Far(DpDpcdOffset,DPCDAddr_16, Value_8)
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
//  V A R I A B L E   E X T E R N S
//******************************************************************************

//******************************************************************************
//  F U N C T I O N   E X T E R N S
//******************************************************************************

//******************************************************************************
//  F U N C T I O N   P R O T O T Y P E S
//******************************************************************************
#pragma option -b.
#ifdef __PARADIGM__
#pragma pack(pop)
#endif
#endif
