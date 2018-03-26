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

#ifndef __STDPBUAPI_H__
#define __STDPBUAPI_H__
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

typedef enum
{
	BU_DeviceType_UPKT_RX,
	BU_DeviceType_UPKT_TX,	
}BU_DeviceType_t;

//******************************************************************************
//  T Y P E D E F S   ( F U N C T I O N   P O I N T E R S )
//******************************************************************************

//******************************************************************************
//  T Y P E D E F S   ( S T R U C T S )
//******************************************************************************
typedef struct
{
	DP_BaseAddr_t			BridgeTopRegMemOffset_p;
	DP_BaseAddr_t			RoutingLogicTopRegMemOffset_p;
}DPBU_RLInitParams_t;

typedef struct
{
	DP_PortType_t				PortType_e;
	U8							ApplicationPortId_8;
	DP_RoutingLogicPortNum_t    RoutingLogicPortNum_e;
	void far *					PortInitParams_ps;
} DP_PortInitParams_t;


typedef struct
{
	U32	HPARAM;
	U32	VPARAM;
	U32	ACTIVE;
	U32	SYNC;
	U32	MVALUE;
	U32	NVALUE;
	U32	PCLKINC;
	U8  Bpp;
} DP_RlPatgenParam_t;

//******************************************************************************
//  V A R I A B L E   E X T E R N S
//******************************************************************************

//******************************************************************************
//  F U N C T I O N   E X T E R N S
//******************************************************************************

//******************************************************************************
//  F U N C T I O N   P R O T O T Y P E S
//******************************************************************************

void DP_EXT_FUNC DPBU_Version(DP_LibVersion_t * DpBuVersionInfo_ps);
void DP_EXT_FUNC DPBU_Handler(void); 
void DP_EXT_FUNC DP_PrintBUVersion(void);
BOOL DP_EXT_FUNC DPBU_AreInternalRegistersOK(U8 PortId_8);

void  DP12DPCDinit(void);
void DP12ModulesInitialization(void);
void  DP12BridgeInterruptInit(void);

#if DP_MULTISTREAM_SOURCE
void DP_EXT_FUNC DP_ProgramPatgen(U8 PatgenId_8,DP_RlPatgenParam_t *DP_RlPatgenParam_ps);
BOOL DP_EXT_FUNC BU_SourceSendMsg(U8 far* SideBandMsg_p, U8 SideBandMsgSize_8, SBM_DPCDBUFF_TYPES SideBandMsgType);
BOOL DP_EXT_FUNC BU_SourceGetMsg(U8 far* SideBandMsg_p, U8 * SideBandMsgSize_p8, SBM_DPCDBUFF_TYPES SideBandMsgType);
U8 DP_EXT_FUNC BU_SideBandMsgReady(void);
#endif


#pragma option -b.
#ifdef __PARADIGM__
#pragma pack(pop)
#endif
#endif	/*__STDPBUAPI_H__*/

