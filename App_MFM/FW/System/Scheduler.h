/*
    $Workfile:   Scheduler.h  $
    $Revision: 1.5 $
    $Date: 2012/06/26 08:27:57 $
*/
#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

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
// MODULE:      Scheduler.h
//
// USAGE:       Provides a Time Based Scheduler for Modules in the Application
//                Modules can Post Events and Processes them in their handlers
//                when the Time Arrives for the Event.
//
//******************************************************************************

//******************************************************************************
//  D E F I N E S
//******************************************************************************
#define NO_INDEX 0xffff     // reserved index value - No index return
#define REQUEST_NOW    0        // W_Time Parameter meaning schedule immediately (no delay)
#define PARAM_NONE    0        // reserved parameter value
#define PARAM_ANY 0xffff    // reserved parameter value

//******************************************************************************
//  T Y P E S
//******************************************************************************
typedef struct
{
   BYTE        B_Request;    // Request ID
   WORD        W_Param;    // Parameter data for Request
   DWORD        DW_Time;    // Time for request execution
} t_REQUEST;

typedef struct
{
   BYTE        B_MaxSize;  // Max Queue Size
   t_REQUEST  *ST_pRequests;   // Pointer to Requests
}t_QUEUE;

//******************************************************************************
//  E N U M S
//******************************************************************************

typedef enum // Common Requests
{
   REQ_NONE                = 0,        // Invalid, Cancelled or Serviced Requests
   REQ_INIT                = 1,        // Initialize Module
   REQ_FIRST               = 2,        // First Module Request in enum
   REQ_DRAM_EXECUTE        = 240,      // DRAM code exection Request
   REQ_DEBUG               = 250,      // Debug Requests
   REQ_ANY                 = 0xff,     // Wildcard Requests (match any request)
} t_COM_REQUEST;

typedef enum // Mode Setup Requests
{
   REQ_MS_SETUP            = REQ_FIRST, //(2)Perform a Complete Setup of Channel, set when timing becomes stable
   REQ_MS_FREERUN          = 3,        //Freerun the Channel when timing becomes unstable
   REQ_MS_INPUT_CHANGE     = 4,        //Modify Input Capture/Display for aspect ratio defined in UserPreferences
   REQ_MS_SWAP             = 5,        //Swap Main and PIP
   REQ_MS_DISPLAY_CHANGE= 6,           //Pip Size change or Side by Side
   REQ_MS_AUTO_ADJUST      = 7,        //Perform an Auto Adust
   REQ_MS_WSS              = 8,        //WSS is Detected by ModeDetection
   REQ_MS_DISPLAY_TIMING   = 9,        //Change the Display Timing(50Hz,60Hz,3:3 Conversion, 2:2 Conversion, etc.)
   REQ_MS_USER_PREF        = 10,       //User Pref Change
   REQ_MS_BLENDER          = 11,       //Request Blender ON
   REQ_MS_DYNAMIC          = 12,       //Request Dynamic Scaling Start, Stop, Step, etc.
   REQ_MS_RESTART          = 13,
   REQ_MS_AUDIO            = 14,       //Request Audio Change: On, Off, Mute, etc.
   REQ_MS_TUNER            = 15,       //Tuner Change: Channel, etc.
} t_MS_REQUEST;

typedef enum // OSD Requests
{
   REQ_OSD_MSG             = REQ_FIRST, //Send an OSD Message
} t_OSD_REQUEST;

// CEC power Requests
typedef enum
{
   REQ_CP_STANDBY          = REQ_FIRST, //Send an CEC power Message
} t_CEC_POWER_REQUEST;


typedef enum // Request Modules
{
   MODULE_MODE_SETUP_CH_A,
   MODULE_MODE_SETUP_CH_B,
   MODULE_OSD_CH_A,
   MODULE_OSD_CH_B,
#if (FEATURE_CEC == ENABLE)
   MODULE_CEC,
#endif
   // Add new Modules Here
   MODULE_NUMBER
} t_MODULE;

//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

// Scheduler Primitives
void SchedulerInit(void);
WORD SchedulerFind(t_MODULE id, BYTE B_Request, WORD W_Param);
WORD SchedulerFindOldest(t_MODULE id);
gmt_RET_STAT SchedulerPut(   t_MODULE id, BYTE B_Request, WORD W_Param, DWORD DW_Time);
gmt_RET_STAT SchedulerCancel(t_MODULE id, BYTE B_Request, WORD W_Param);
void SchedulerGet(t_MODULE id_module, t_REQUEST *pST_Request);


// ModeSetup Wrappers
extern ROM t_MODULE SchedulerModePack[CH_PHY_NUM];

#define ModeSetupRequest(B_Channel, B_Request, W_Param, W_Time)\
    SchedulerPut(SchedulerModePack[B_Channel], B_Request, W_Param, W_Time)

#define ModeSetupRequestCancel(B_Channel, B_Request, W_Param)\
    SchedulerCancel(SchedulerModePack[B_Channel], B_Request, W_Param)

#define ModeSetupRequestGet(B_Channel, B_Request, pST_Request)\
    SchedulerGet(SchedulerModePack[B_Channel], pST_Request)

//Gets the ModeSetup index at a specific request
#define ModeSetupRequestCheck(B_Channel, B_Request)\
    SchedulerFind(SchedulerModePack[B_Channel], B_Request, PARAM_ANY)


// OSD Wrappers
extern ROM t_MODULE SchedulerOsdPack[CH_PHY_NUM];

#define OSDRequestMsg(B_Channel, W_Param, W_Time)\
    SchedulerPut(SchedulerOsdPack[B_Channel], REQ_OSD_MSG, W_Param, W_Time)

#define OSDRequestCancelMsg(B_Channel, W_Param)\
    SchedulerCancel(SchedulerOsdPack[B_Channel], REQ_OSD_MSG, W_Param)

#define OSDRequestGet(B_Channel, B_Request, pST_Request)\
    SchedulerGet(SchedulerOsdPack[B_Channel], pST_Request)

// CEC Power Wrappers
#if (FEATURE_CEC == ENABLE)
#define CECPowerRequest(B_Request, W_Param, W_Time)\
    SchedulerPut(MODULE_CEC, B_Request, W_Param, W_Time)

#define CECPowerRequestCancel(B_Request, W_Param)\
    SchedulerCancel(MODULE_CEC, B_Request, W_Param)

#define CECPowerRequestGet(B_Request, pST_Request)\
    SchedulerGet(MODULE_CEC, pST_Request)
#endif

#endif

//*********************************  END  **************************************


