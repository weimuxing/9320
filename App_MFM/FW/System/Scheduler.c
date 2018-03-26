/*
    $Workfile:   Scheduler.c  $
    $Revision: 1.5 $
    $Date: 2011/12/15 03:52:12 $
*/
#define __SCHEDULER_C__

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
// MODULE:      Scheduler.c
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include <string.h>
#include "System\all.h"

//******************************************************************************
//  D E F I N E S
//******************************************************************************

//#define DEBUG_MODULE MODULE_MODE_SETUP_CH_A


//******************************************************************************
//  G L O B A L   V A R I A B L E S
//******************************************************************************
// Queue Sizes -     The Scheduler optimizes the Queue by updating the timestamps
// of Requests that are Scheduled Multiple Times (if the parameter is also the same.)
// This means that a given Request should only appear once per Q.  So, the Max Q Size
// can be "about" the same as the number of possible Requests in that Module. Of
// course  most requests will never happen at the same time.

#define MODESETUP_MAX_Q_SIZE        12                // TODO: Review Size
#define OSD_MAX_Q_SIZE               8                // TODO: Review Size
#if (FEATURE_CEC == ENABLE)
#define CEC_MAX_Q_SIZE               1    // CEC
#endif


// Request Buffers
t_REQUEST ST_ModeSetupRequests[NUM_CHANNELS][MODESETUP_MAX_Q_SIZE];
t_REQUEST ST_OSDRequests[NUM_CHANNELS][OSD_MAX_Q_SIZE];
#if (FEATURE_CEC == ENABLE)
t_REQUEST ST_CECPowerRequests[CEC_MAX_Q_SIZE];
#endif
// TODO: Add new Buffers here

// Queues
ROM t_QUEUE    ST_Queue[MODULE_NUMBER] =
{
   {MODESETUP_MAX_Q_SIZE,   ST_ModeSetupRequests[CH_A]  },
   {MODESETUP_MAX_Q_SIZE,   ST_ModeSetupRequests[CH_B]  },
   {OSD_MAX_Q_SIZE,         ST_OSDRequests[CH_A]        },
   {OSD_MAX_Q_SIZE,         ST_OSDRequests[CH_B]        },
#if (FEATURE_CEC == ENABLE)
   {CEC_MAX_Q_SIZE,         ST_CECPowerRequests         },
#endif
   //TODO: Add new Queues Here
};

//*****************************************************************************
//  C O D E
//*****************************************************************************

//******************************************************************************
//
// FUNCTION       : void SchedulerInit(void)
//
// USAGE          : Called in Init.c to Prepare the Q's for use.  This
//                  function also automatically posts the EV_INIT events
//                  for each Module.  These events are fired AFTER all
//                  other initialization, but before any other events.
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : ST_pAllQueues                    (RO)
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void SchedulerInit(void)
{
   BYTE B_Index;
   for (B_Index = MODULE_NUMBER; B_Index--;)
   {
      BYTE B_Inx;
      for (B_Inx = ST_Queue[B_Index].B_MaxSize; B_Inx--;)
         ST_Queue[B_Index].ST_pRequests[B_Inx].B_Request = REQ_NONE;
      SchedulerPut(B_Index, REQ_INIT, 0, REQUEST_NOW);
   }
}

//******************************************************************************
// USAGE          : Used to Find and return definite Request from the Queue.
//  if B_Request == REQ_NONE than function is searching oldest expired events.
//                  This routine does not remove the event from the Queue,
//                  it marks it as servied.  The owning Handler must call
//                  SchedulerCleanup() after servicing all Requests.
//
// INPUT          : id_module - module id
//                  B_Request - Request to get
//
// OUTPUT         : index of request in appropriate queue
//******************************************************************************
WORD SchedulerFind(t_MODULE id_module, BYTE B_Request, WORD W_Param)
{
   WORD index = NO_INDEX;
   if (id_module >= MODULE_NUMBER)
   {
      gm_Printf("Scheduler::Wrong module ID", 0);
   }
   else
   {
      WORD counter = ST_Queue[id_module].B_MaxSize;
      t_REQUEST *pQ = ST_Queue[id_module].ST_pRequests+(counter-1);
      for (; counter--; pQ--) // search for apropriate request (nevertheless expiration time)
         if (
            ((B_Request == pQ->B_Request) ||
             (B_Request == REQ_ANY) && (pQ->B_Request != REQ_NONE)) // Request Match?
            &&
            ((W_Param == PARAM_ANY) || (W_Param == pQ->W_Param))) // Param Match?
         {
            index = counter;
            break;
         }
   }
   return index;
}

//******************************************************************************
// USAGE          : Used to Find and return definite Request from the Queue.
//  if B_Request == REQ_NONE than function is searching oldest expired events.
//                  This routine does not remove the event from the Queue,
//                  it marks it as servied.  The owning Handler must call
//                  SchedulerCleanup() after servicing all Requests.
//
// INPUT          : id_module - module id
//                  B_Request - Request to get
//
// OUTPUT         : index of request in appropriate queue
//******************************************************************************
WORD SchedulerFindOldest(t_MODULE id_module)
{
   WORD index = NO_INDEX;
   if (id_module >= MODULE_NUMBER)
   {
      gm_Printf("Scheduler Find Oldest ::Wrong module ID", 0);
   }
   {
      DWORD time_cur = gm_GetSystemTime();
      DWORD d_time = 0; //oldest request delta time
      WORD counter = ST_Queue[id_module].B_MaxSize;
      t_REQUEST *pQ = ST_Queue[id_module].ST_pRequests+(counter-1);
      for (; counter--; pQ--) // search for oldest expired request
         if (pQ->B_Request != REQ_NONE)
         {
            DWORD d_time_cnd = time_cur - pQ->DW_Time; //candidate for oldest request delta time
            if (((d_time_cnd & 0x80000000ul) == 0) && (d_time_cnd>d_time))
            {
               d_time = d_time_cnd;
               index = counter;
            }
         }
   }
   return index;
}

//******************************************************************************
// USAGE          : Used to schedule an event to occur at a time "DW_Time" milliseconds from now.
//
// INPUT          : id_module - module id
//                  B_Request -    REQ_MS_SETUP, REQ_MS_FREERUN etc (see Scheduler.h)
//                  W_Param     -    Request Parameter (any data for handler)
//                  DW_Time    - Time delta (ms) to execute request in
//
// OUTPUT         : ERR_PARAM -    Invalid id
//                  ERR_OVERFLOW - Queue is full
//                  OK - Event Scheduled properly
//
// PRE_CONDITION  : SchedulerInit() must be called before events can be
//                  scheduled.
//******************************************************************************
gmt_RET_STAT SchedulerPut(t_MODULE id_module, BYTE B_Request, WORD W_Param, DWORD DW_Time)
{
   WORD index = SchedulerFind(id_module, REQ_NONE, PARAM_ANY);
   if (index == NO_INDEX)
   {
      gm_Printf("*** Scheduler::Module(%d): Request(%d): Queue Full.", id_module, B_Request);
      return ERR_OVERFLOW;
   }
   else
   {
      t_REQUEST *pQ = ST_Queue[id_module].ST_pRequests + index;
      pQ->B_Request = B_Request;
      pQ->W_Param = W_Param;
      pQ->DW_Time = gm_GetSystemTime() + DW_Time;

#ifdef DEBUG_MODULE
      if (id_module == DEBUG_MODULE)
         gm_Printf("  Put::Mod(%d) Req(%d) Par(%d) Delay(%ld ms)",
                   id_module, B_Request, W_Param, DW_Time);
#endif
      return OK;
   }
}

//******************************************************************************
// USAGE  : Used to remove a scheduled request from the scheduler before it is handled.
//
// INPUT  : id_module - module id
//          B_Request - REQ_MS_SETUP REQ_MS_FREERUN etc (see Scheduler.h)
//          W_Param    - Parameter to match (or PARAM_NONE)
//
// OUTPUT         : ERR_NO_DATA -                    Event is not in Queue
//                  ERR_POINTER     -                Invalid Q Pointer
//                  OK -                        Event found and removed
//******************************************************************************
gmt_RET_STAT SchedulerCancel(t_MODULE id_module , BYTE B_Request, WORD W_Param)
{
   WORD index;

   if (B_Request == REQ_NONE)
   {
      gm_Printf("Scheduler::Wrong request", 0);
      return ERR_PARAM;
   }

   while ((index = SchedulerFind(id_module, B_Request, W_Param)) != NO_INDEX)
   {
      ST_Queue[id_module].ST_pRequests[index].B_Request = REQ_NONE;
#ifdef DEBUG_MODULE
      if (id_module == DEBUG_MODULE)
         gm_Printf("   Del::Mod(%d) Req(%d) Par(%d)", id_module, B_Request, W_Param);
#endif
   }
   return OK;
}

//******************************************************************************
// USAGE          : Used to return a oldest expired request from the Queue.
//
// INPUT          : ST_pQ  - Pointer to Queue
//                  B_Request    -Request to get(Normally use REQ_ANY)
//
// OUTPUT         : t_REQUEST - The Request
//******************************************************************************
void SchedulerGet(t_MODULE id_module, t_REQUEST *pST_Request)
{
   WORD index = SchedulerFindOldest(id_module);
   pST_Request->B_Request = REQ_NONE;
   if (index == NO_INDEX)
      return ;

   *pST_Request = ST_Queue[id_module].ST_pRequests[index];
   ST_Queue[id_module].ST_pRequests[index].B_Request = REQ_NONE; //Mark Request as Serviced

#ifdef DEBUG_MODULE
   if (id_module == DEBUG_MODULE)
      gm_Printf("  Get::Mod(%d) Req(%d) Par(%d) Wait(%ld ms)",
                id_module, pST_Request->B_Request, pST_Request->W_Param,
                gm_GetSystemTime()-pST_Request->DW_Time);
#endif

   return ;
}

//******************************************************************************
ROM t_MODULE SchedulerModePack[CH_PHY_NUM] =
{
   MODULE_MODE_SETUP_CH_A,
   MODULE_MODE_SETUP_CH_B
};

ROM t_MODULE SchedulerOsdPack[CH_PHY_NUM] =
{
   MODULE_OSD_CH_A,
   MODULE_OSD_CH_B
};

/***********************************  END  ************************************/
