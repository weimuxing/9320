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

#ifndef __HDCPREPLIB_H__
#define __HDCPREPLIB_H__

//******************************************************************************
//	C O M P I L E R    O P T I O N S										
//******************************************************************************
// Force byte alignment
#ifdef __PARADIGM__
#pragma pack(push, 1)
#endif
#pragma option -b- //as a byte
//******************************************************************************
//  C O N S T A N T   D E F I N E S
//******************************************************************************
#define		MAX_NUM_KSVS		127
#define		KSV_SIZE			5
#define		KSV_ARRAY_SIZE		MAX_NUM_KSVS * KSV_SIZE


//******************************************************************************
//  M A C R O   D E F I N E S
//******************************************************************************
#define UNUSED_VAR(x)	((x) = (x))


//******************************************************************************
//  T Y P E D E F S 
//******************************************************************************

typedef enum
{
	TxRequest_GetStatus				= 1,
	TxRequest_DisableEncryption	    = 2,
	TxRequest_StartAuthentication	= 3,
	TxRequest_GetBinfo				= 4,
	TxRequest_GetBksv				= 5,
	TxRequest_EnableEncryption      = 6,
} HdcpRepeaterTxRequest_t;

typedef enum
{
	RxRequest_GetStatus							= 1,
	RxRequest_SetRepeaterBit					= 2,
	RxRequest_ClearRepeaterBit					= 3,
	RxRequest_SetBinfo							= 4,
	RxRequest_SetKsvList						= 5,
	RxRequest_SetReadyBit           			= 6,
	RxRequest_StartReAuthentication 			= 7,
	RxRequest_RepeaterAuthenticationStarted		= 8	
} HdcpRepeaterRxRequest_t;

typedef enum
{
	HDCP_Repeater_UnAuthenticated,
	HDCP_Repeater_StartAuthenticationOnDownstream,
	HDCP_Repeater_WaitForFirstPhaseCompletion,
	HDCP_Repeater_FirstPhaseComplete,
	HDCP_Repeater_WaitForKsvList,
	HDCP_Repeater_WaitForBinfo,
	HDCP_Repeater_Authenticated
}HdcpRepeaterState_t;


typedef struct
{
	U16		HDCP_Unauthenticated	: 1;
	U16		HDCP_SinkConnected		: 1;
	U16		HDCP_RptrConnected		: 1;
	U16		HDCP_1stPhasePass		: 1;
	U16		HDCP_1stPhaseFail		: 1;
	U16		HDCP_WaitForKsvRead		: 1;
	U16		HDCP_KsvReady			: 1;
	U16		HDCP_2ndPhasePass		: 1;
	U16		HDCP_2ndPhaseFail		: 1;
	U16		HDCP_LinkIntegrityFail	: 1;
} HdcpRepeaterTxState_t;

typedef struct
{
	U16		HdcpUnauthenticated			: 1;
	U16		HdcpAuthenticationStarted	: 1;
	U16    	HdcpEncrypted				: 1;
} HdcpRepeaterRxState_t;

typedef struct
{
	U16		DownStreamDeviceType_Sink			: 1;
	U16		DownStreamDeviceType_Repeater		: 1;
} HdcpRepeaterDownStreamDeviceType_t;


typedef void FAR (* HdcpRepeaterTxEntry_t)(U32							TxDeviceIdentifier_32,
												HdcpRepeaterTxRequest_t		TxRequest_e,
												U8 *						TxReply_pv);

typedef void FAR (* HdcpRepeaterRxEntry_t)(U32							RxDeviceIdentifier_32,
												HdcpRepeaterRxRequest_t		RxRequest_e,
												U8*							RxRequestData_pv);										

typedef struct
{
	U32								DeviceIdentifier_8;
	HdcpRepeaterTxEntry_t			HdcpRepeaterTxEntry_pf;
	HdcpRepeaterTxState_t			HdcpRepeaterTxState_e;
} HdcpDownStreamDeviceInfo_t;

typedef struct
{
	U32								DeviceIdentifier_8;
	HdcpRepeaterRxEntry_t			HdcpRepeaterRxEntry_pf;
	HdcpRepeaterRxState_t			HdcpRepeaterRxState_e;
} HdcpUpStreamDeviceInfo_t;


typedef struct HdcpRepeaterInstanceData_s
{
	U32											HdcpRepeaterIdentifier_8;
	U8											NumUpStreamPorts_4		: 4;
	U8											NumDownStreamPorts_4	: 4;
	HdcpDownStreamDeviceInfo_t *				DownStreamDevices_ps;
	HdcpUpStreamDeviceInfo_t *					UpStreamDevices_ps;
	HdcpRepeaterDownStreamDeviceType_t			HdcpRepeaterDownStreamType_s;	
	HdcpRepeaterState_t						    HdcpRepeaterState_e;
	U8											HdcpTxDevs_4			: 4;
	U8											HdcpRxDevs_4			: 4;
	U32											FirstPhaseTimeOut_32;
	U32											SecondPhaseTimeOut_32;
	U16											KsvIndex_16;
	U8											KsvList_p8[KSV_ARRAY_SIZE];
	struct HdcpRepeaterInstanceData_s *			Next_ps;
} HdcpRepeaterInstanceData_t;

//******************************************************************************
//  F U N C T I O N   P R O T O T Y P E S
//******************************************************************************
void HDCP_EXT_FUNC HdcpRptr_RxClearRepeaterBit(void);
void HDCP_EXT_FUNC HdcpRptr_RxSetRepeaterBit(void);
U8 * HDCP_EXT_FUNC HdcpRepeater_GetKsvBuffer (U8 HdcpRepeaterIdentifier_8,U8 DeviceCount_8);
U32 EXT_FUNC  HDCP_Ext_GetSystemTime(void);
void HDCP_EXT_FUNC RunHdcpRepeater(U32 HdcpRepeaterIdentifier_8);
BOOL HDCP_EXT_FUNC CreateHdcpRepeater(HdcpRepeaterInstanceData_t * InstanceData_ps);
void HDCP_EXT_FUNC HdcpRepeater_PrintVersion(void);
void HDCP_EXT_FUNC HDCP_EnablePrints(BOOL Enable_b);
//#pragma warn +bbf
#pragma option -b.
#ifdef __PARADIGM__
#pragma pack(pop)
#endif
#endif
