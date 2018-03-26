/*
	$Workfile:   gm_ModeDetect.h  $
	$Revision: 1.5 $
	$Date: 2011/12/14 03:44:48 $
*/
#ifndef __GM_MODEDETECT_H__
#define __GM_MODEDETECT_H__
//******************************************************************************
// COPYRIGHT (C) STMicroelectronics 2011.
//
// All rights reserved. This document contains proprietary and
// confidential information of the STMicroelectronics Group. This
// document is not to be copied in whole or part. STMicroelectronics
// assumes no responsibility for the consequences of use of such
// information nor for any infringement of patents or other rights of
// third parties which may result from its use. No license is granted by
// implication or otherwise under any patent or patent rights of
// STMicroelectronics.
// STMicroelectronics
// products are not authorized for use as critical components in life
// support devices or systems without express written approval of
// STMicroelectronics.
//==============================================================================
//
// MODULE:      gm_ModeDetect.h
//
// USAGE:       Header file for system library utility layer mode detection
//				definition
//
//******************************************************************************
//#define DECODER_COMPONENT
//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************
#define DETECTION_TMO		(2*(1000/V_SYNC_MIN))	// timeout measured in msec
#define DETECTION_TMO_DEC   ( 40 - 5 )    //40msec for decoder handlers.( 5msec is accounted for the loop time)

// time quanta allocated for each sync type
#define DSS_MAX_COUNT		2			// 2*DETECTION_TMO
#define DCS_MAX_COUNT		2			// 2*DETECTION_TMO
#define SOG_MAX_COUNT		5			// 5*DETECTION_TMO


// sample point
#define SAMPLE14                    0x000
#define SAMPLE38                    0x004
#define SAMPLE12                    0x008
#define SAMPLE58                    0x00C

//
// Sync status transition for application
//
typedef enum
{
	CHANGE_IDLE,						// Idle, flag cleared
	IDLE_UNSTABLE,
	IDLE_STABLE,	
	CHANGE_TO_STABLE,					// From un-stable to stable
	CHANGE_TO_UNSTABLE					// From stable to un-stable
}gmt_TransitionState;

//
// System state for detection state machine
//
typedef enum
{
	DETECTION_STATE_IDLE,       		// Idle state
	DETECTION_STATE_BECOMES_UNSTABLE,
	DETECTION_STATE_UNSTABLE,			
	DETECTION_STATE_BECOMES_STABLE,
	DETECTION_STATE_STABLE,		
	DETECTION_STATE_PENDING,
}gmt_DetectionState;



typedef struct
{
	gmt_SYNC_TYPE	Ba_Sync[MAX_SYNC_TYPE];
} DetModeStruct;


#define IMP_SYNC_STATUS_MASK (IMP_NO_HS|IMP_NO_VS)
#define IPP_SYNC_STATUS_MASK (IPP_NO_HS|IPP_NO_VS)

#define IMP_TIMING_STATUS_MASK (IMP_HS_PERIOD_ERR|IMP_VS_PERIOD_ERR)
#define IPP_TIMING_STATUS_MASK (IPP_HS_PERIOD_ERR|IPP_VS_PERIOD_ERR)

#define IMP_STATUS_MASK (IMP_SYNC_STATUS_MASK | IMP_TIMING_STATUS_MASK)
#define IPP_STATUS_MASK (IPP_SYNC_STATUS_MASK | IPP_TIMING_STATUS_MASK)


//******************************************************************************
//	S T A T I C    V A R I A B L E S
//******************************************************************************
extern BYTE B_StabilityCount[CH_PHY_NUM];			// Sync stable count

//
// Mode detect enable flag
// Mode detect state
// Mode transition status
//
extern gmt_DET_MODE ModeDetectFlag[CH_PHY_NUM] ;
extern gmt_DET_STATE StateDetectFlag[CH_PHY_NUM] ;
extern gmt_DetectionState B_DetectTimingState[CH_PHY_NUM];
extern gmt_TransitionState B_InputDetectionTransition[CH_PHY_NUM];
extern gmt_AutoMode B_AutoMode[CH_PHY_NUM];
extern gmt_DEVICE_ID B_DeviceId[CH_PHY_NUM];

//******************************************************************************
//	S T A T I C    F U N C T I O N	  P R O T O T Y P E S
//******************************************************************************

//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E
//******************************************************************************
void gmp_ModeDetectInit		(void);
void gm_DetectionEngine(void);

#endif // #ifndef __GM_MODEDETECT_H__
//*********************************  END  **************************************
