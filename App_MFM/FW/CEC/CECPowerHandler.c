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
// MODULE:      CECHandler.c
//
// USAGE:       This file contains functions for CEC driver functionality
//
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#include "system\all.h"

#if (FEATURE_CEC == ENABLE)
//******************************************************************************
//  G L O B A L    F U N C T I O N S
//******************************************************************************
void ReportPhysicalAddress(BYTE B_DestAddr);
void CecFeatureRequestActiveSource(BYTE B_DestLogAddr);
//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************
//extern gmt_PwrStateType PwrState;

//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************

//******************************************************************************
//  L O C A L    F U N C T I O N S
//******************************************************************************

//******************************************************************************
//  L O C A L    V A R I A B L E S
//******************************************************************************

//******************************************************************************
//  M A C R O   D E F I N I T I O N
//******************************************************************************

#define	RESP_TIME_1_SEC 10
//******************************************************************************
//  C O D E
//******************************************************************************
static gmt_PwrCecRequestType CecAppPowerHandlerInterface = ReqNone;

BOOL CecAppPowerHandlerRequest(gmt_PwrCecRequestType Request)
{
   //  gm_Print("Power request",0);
   if (CecAppPowerHandlerInterface == ReqNone)
   {
      //    gm_Print("Successful",0);
      CecAppPowerHandlerInterface = Request;
      return TRUE;
   }
   else return FALSE;
}

//******************************************************************************
//
// FUNCTION       : void CecAppPowerHandler(void)
//
// USAGE          : Main()
//                  CecAppPowerHandler()
//                  CEC Power Handler functions for Main and PIP
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : gmvb_MainChannel				(RO)
//                  gmvb_PipChannel					(RO)
//                  B_ModeSetupQSize				(RW)
//                  ST_ModeSetupQ					(RW)
//                  B_PrevPipMode					(RW)
//                  B_BlenderBothRequired			(RW)
//                  B_DisableDynamicScale			(RW)
//                  W_OverscanEnabled				(RW)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
#define BOOTSTRAP_TIMEOUT 500

typedef enum
{
   gmd_DUTY,
   gmd_WAIT_TIMEOUT
}gmt_CecAppPowerHandlerStates;

void CecAppPowerHandler(void)
{
   static gmt_CecAppPowerHandlerStates State = gmd_DUTY;
   //static WORD W_Time;
   switch (State)
   {
      case gmd_DUTY:
         if (CecAppPowerHandlerInterface != ReqNone)
         {
            switch (CecAppPowerHandlerInterface)
            {
               case ReqPwrOn:
                  //  gm_Print("Cec power on handler - Ok",0);
#ifdef REQUEST_ACTIVE_SOURCE_ON_POWER_ON
                  CecFeatureRequestActiveSource(CEC_BROADCAST_LOG_ADDR);
#endif
                  ReportPhysicalAddress(CEC_BROADCAST_LOG_ADDR);
                  //State = gmd_WAIT_TIMEOUT;
                  //W_Time = (WORD)gm_GetSystemTime();
                  RequestSystemAudioStatus();
                  break;
               default:
                  break;
            }
            CecAppPowerHandlerInterface = ReqNone;
         }
         break;
      case gmd_WAIT_TIMEOUT:
#if 0
         if ((WORD)gm_GetSystemTime() - W_Time >= BOOTSTRAP_TIMEOUT)
         {
            State = gmd_DUTY;
            RequestSystemAudioStatus(gmd_AudioSystem);
         }
#endif
         break;
   }
}

#ifdef CEC_STANDBY_CAPABLE
void gm_CecProcessGoToStandby(BYTE B_DestAddr)
{
   BYTE Ba_Data[1];
   Ba_Data[0] = CEC_MSG_STANDBY;                      // Opcode
   gm_Print("B_DestAddr = %d",B_DestAddr);
   gm_CecMngrTxMessage(B_DestAddr, 1, &(Ba_Data[0]), 0, 0);
   //gm_CecTxFrame(B_DestAddr, 1, &(Ba_Data[0]));
}
#endif

#endif
