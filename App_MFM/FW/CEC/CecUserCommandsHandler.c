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
#include "StdLibInc\gm_Register.h"
#include "StdLibInc\gm_Mcu186.h"
#include "CEC\CecUserCommandsHandler.h"
#include "CEC\CecHandler.h"
#include <embedded.h>
#include <mem.h>
#include <string.h>

//******************************************************************************
//  E X T E R N S
//******************************************************************************
extern BYTE curPresetNum;
extern WORD ActiveSourcePhisicalAddr;
/*********************************************************************************************
*
* User /Tester/ initiated activity handler
*
* Interface:
*
* from User:
*   gmt_OsdControlCode  gm_OsdToCecCommand;     - this is a code of User Command
*   BYTE                gm_CommandParameter;    - this is a parameter of User Command
*   gmt_CecDeviceType   gm_CecDeviceType;       - this is a logical address of dest device
*   BOOL                gm_CommandLatch;        - this is a signal to start command
*
* to User (OSD subsystem)
*   These are messages for acknowledge or non-acknowledge events
*
*   OSDRequestMsg(gmvb_MainChannel, MTO_CEC_Command_OK, REQUEST_NOW);
*                                   ==================
*   OSDRequestMsg(gmvb_MainChannel, MTO_CEC_Command_Failed, REQUEST_NOW);
*                                   ======================
*
*   void PrintStatusInfo(BYTE B_DeviceLogAddr,
*                        const char *str1,
*                        const char *str2,
*                        const char *str3);
*
*   OSDRequestMsg(gmvb_MainChannel, MTO_CEC_Text4UserReady, REQUEST_NOW);
*                                   ======================
* to User (Not OSD subsystem if any)
*   void PrintStatusInfo(BYTE B_DeviceLogAddr,
*                        const char *str1,
*                        const char *str2,
*                        const char *str3);
*
*   BOOL                gm_CecAnswerReady = FALSE;      - this is a signal to read results
*   gmt_CecAckResult    gm_AckResult = gmd_AR_Unknown;  - this is an Acknowledge result
*
* WARNING!!! This handler can not handle several requests at a time, so
* normal scenario is follow:
*
* 1) Client: gm_OsdToCecCommand     := something;
* 2) Client: gm_CommandParameter    := something;
* 3) Client: gm_CecDeviceType       := something;
* 4) Client: gm_CommandLatch        := TRUE;
*
* 5) Handler: gm_CommandLatch       := FALSE;
* 5) Handler: PrintStatusInfo();
* 6) Handler: gm_AckResult          := {gmd_AR_Ok, gmd_AR_Failed}
* 6) Handler: gm_CecAnswerReady     := TRUE;
*
* this should be Ok because we have only user.
*/
#if (FEATURE_CEC == ENABLE)

static gmt_CecAppUserCommandHandlerInterface gm_UserCommandInterface;

//BOOL SubmitUserCommand(gmt_CecDeviceType LA, BYTE B_Command, BYTE* B_Parameters)
BOOL CecAppUserCommandHandlerRequest(gmt_CecUserCommand *UserCommand)
{
   int i;
   if (gm_UserCommandInterface.Stage == gmd_UCS_idle)
   {
      gm_UserCommandInterface.Command = *UserCommand;
      gm_UserCommandInterface.Command.BriefCommand.AckResult = gmd_AR_Unknown;
      gm_UserCommandInterface.Stage = gmd_UCS_request;
      gm_Print("Command has been submitted",0);
      return TRUE;
   }
   else
   {
      gm_Print("Command hasn't been submitted",0);
      return FALSE;
   }
}

gmt_UserCommandState CecAppUserCommandHandlerGetStatus(void)
{
   return gm_UserCommandInterface.Stage;
}

BOOL CecAppUserCommandHandlerResetStatus(void)
{
   if (gm_UserCommandInterface.Stage == gmd_UCS_complete)
   {
      gm_UserCommandInterface.Stage = gmd_UCS_idle;
      return TRUE;
   }
   else
   {
      return FALSE;
   }
}

void CecAppUserCommandHandlerGetResults(gmt_CecUserCommand *CecUserCommand)
{
   *CecUserCommand = gm_UserCommandInterface.Command;
}


#define msg(message)    gm_Print(message,0)

//Selecting analogue broadcast type
#pragma argsused
void FillAnalogBroadcastTypeField(BYTE *B_Field)
{
}

//this function writes frequency which corresponds to B_Channel into 2 bytes
//starting form B_Field (the frequency is number = Freq in Hz / 62500)
#pragma argsused
void FillFrequencyField(BYTE *B_Field, WORD W_Channel)
{
}

#pragma argsused
void FillTVSystemField(BYTE *B_Field)
{
}

#pragma argsused
void PrepareRecordOnPacket(BYTE CecMessageBuffer[], BYTE*B_Length, gmt_RecordSourceType RecordSource, WORD W_ExtPhysAddr)
{
}

void PrepareRecordOnPacket2(BYTE RecorderLogAddr, BYTE CecMessageBuffer[], BYTE*B_Length)
{
   if (IsHDMIInput(UserPrefCurrentInputMain) == gmd_TRUE)
   {
      //Current input is HDMI
      msg("Current input is HDMI");
      if (RecorderLogAddr == GetActiveSourceLogicalAddress())
      {
         //Active source is recorder
         msg("Active source is recorder");
         if (gm_CecUtilExtractDigitFromPhysAddr(GetActiveSourcePhysicalAddress(), 1) ==
               gm_CecUtilExtractDigitFromPhysAddr(InputPortToPhysicalAddress(UserPrefCurrentInputMain), 1))
         {
            msg("Active source routed to current HDMI input, record Own Source");
            PrepareRecordOnPacket(CecMessageBuffer, B_Length, gmd_RSTRecordOwnSource, 0);
         }
         else
         {
            msg("Active source is not routed to current HDMI input");
            *B_Length = 0;
         }
      }
      else
      {
         //Active source is not recorder
         msg("Active source is not recorder");
         if (GetActiveSourcePhysicalAddress() != 0xFFFF)
         {
            //Physical address of active source is valid
            msg("Physical address of active source is valid, record Ext Phys Address");
            PrepareRecordOnPacket(CecMessageBuffer, B_Length, gmd_RSTRecordExternalPhysicalAddress, GetActiveSourcePhysicalAddress());
         }
         else
         {
            //Physical address of active source is invalid
            msg("Attempt to record signal from external physical address, but no <active source> message precedes this attempt");
            *B_Length = 0;
         }
      }
   }
   else
   {
      {
         PrepareRecordOnPacket(CecMessageBuffer, B_Length, gmd_RSTRecordExternalPlug, UserPrefCecRecordOnExtPlugNumber);
         gm_Print("Record external plug Nr %d",UserPrefCecRecordOnExtPlugNumber);
         //*B_Length = 0;
      }
   }
}

typedef enum
{
   gmd_UCH_Idle,
   gmd_UCH_SendCommand,
   gmd_UCH_WaitAck,
   gmd_UCH_WaitQueryResult,
}gmt_UserCommandHandlerStates;

//timeout = 10 seconds
#define USER_COMMAND_TIMEOUT 10000

// this function is here temporarily
// there is the same function in Cec.lib
// but library hasn't been updated so far
BOOL far gm_CecUtilIsMessageNotReplied(gmt_CEC_MNGR_MESSAGE_STATUS fs)
{
   if ((fs == gmd_CEC_MNGR_TX_STS_NOT_ACKNOWLEDGED) ||
         (fs == gmd_CEC_MNGR_TX_STS_ERROR))
      return TRUE;
   else return FALSE;
}

//CUSTOMER APPLICATION
void CecAppUserCommandHandler(void)
{
   static gmt_UserCommandHandlerStates State = gmd_UCH_Idle;
   gmt_UserCommandHandlerStates NewState;
   static BYTE B_MsgId = INVALID_MSG_ID;
   BYTE B_ResponseCode;
   static BYTE B_ResponseBuffer[20];
   gmt_CEC_MNGR_MESSAGE_STATUS FrameStatus;
   BYTE B_DestAddr;
   BYTE Ba_Data[16];
   BYTE B_Len;
   gmt_RecordSourceType RecSource;
   gmt_CEC_MNGR_TX_MESSAGE_HANDLE MsgHandle;
   static DWORD RequestTime;

   if ((State != gmd_UCH_Idle) && ((gm_GetSystemTime() - RequestTime) > USER_COMMAND_TIMEOUT))
   {
      gm_UserCommandInterface.Command.BriefCommand.AckResult = gmd_AR_Failed;
      gm_UserCommandInterface.Stage = gmd_UCS_complete;
      if (B_MsgId != INVALID_MSG_ID)
         gm_CecMngrGetTxMessageHandle(B_MsgId);
      State = gmd_UCH_Idle;
   }

   switch (State)
   {
      case gmd_UCH_Idle :
         if (gm_UserCommandInterface.Stage != gmd_UCS_request) return;
         B_DestAddr = gm_UserCommandInterface.Command.BriefCommand.LogicalAddress;
         NewState = gmd_UCH_Idle;
         B_Len = 0;
         switch (gm_UserCommandInterface.Command.BriefCommand.OpCode)
         {
// 1-byte messages (consists only of header byte, B_Len = 0)
            case gmd_OCC_Ping:
               NewState = gmd_UCH_WaitAck;
               break;
// 2-byte, query
            default:
               B_Len = 1;
               Ba_Data[0] = gm_UserCommandInterface.Command.BriefCommand.OpCode;
               NewState = gmd_UCH_WaitQueryResult;
               switch (gm_UserCommandInterface.Command.BriefCommand.OpCode)
               {
                  case gmd_OCC_GiveAudioStatus:
                     NewState = gmd_UCH_WaitAck;
                     break;
                  case gmd_OCC_GiveSystemAudioModeStatus:
                     B_ResponseCode = CEC_MSG_SYSTEM_AUDIO_MODE_STATUS;
                     //NewState = gmd_UCH_WaitAck;
                     break;
                  case gmd_OCC_Abort:
                     B_ResponseCode = CEC_MSG_FEATURE_ABORT;
                     break;
                  case gmd_OCC_GetPowerStatus:
                     B_ResponseCode = CEC_MSG_REPORT_POWER_STATUS;
                     break;
                  case gmd_OCC_GetCecVersion:
                     B_ResponseCode = CEC_MSG_CEC_VERSION;
                     break;
                  case gmd_OCC_GetPhysicalAddress:
                     B_ResponseCode = CEC_MSG_REPORT_PHYSICAL_ADDRESS;
                     break;
                  case gmd_OCC_GetMenuLanguage:
                     B_ResponseCode = CEC_MSG_SET_MENU_LANGUAGE;
                     break;
                  case gmd_OCC_RecordOff:
                     B_ResponseCode = CEC_MSG_RECORD_STATUS;
                     break;
                  case gmd_OCC_GiveOsdName:
                     B_ResponseCode = CEC_MSG_SET_OSD_NAME;
                     break;
                  default:
// 3-byte, query
                     B_Len = 2;
                     Ba_Data[0] = gm_UserCommandInterface.Command.BriefCommand.OpCode;
                     Ba_Data[1] = gm_UserCommandInterface.Command.Data.Common.Parameters[0];
                     NewState = gmd_UCH_WaitQueryResult;
                     switch (gm_UserCommandInterface.Command.BriefCommand.OpCode)
                     {
                        case gmd_OCC_Play:
                        case gmd_OCC_DeckControl:
                           B_ResponseCode = CEC_MSG_DECK_STATUS;
                           break;
                        case gmd_OCC_GetDeckStatus:
                           B_ResponseCode = CEC_MSG_DECK_STATUS;
                           break;
                        default:
// other commands
                           switch (gm_UserCommandInterface.Command.BriefCommand.OpCode)
                           {
                              case gmd_OCC_SystemAudioModeRequest:
                                 Ba_Data[0] = CEC_MSG_SYSTEM_AUDIO_MODE_REQUEST;
                                 if (gm_UserCommandInterface.Command.Data.Common.Parameters[0] == gmd_SAS_ON)
                                 {
                                    *((WORD*)&Ba_Data[1]) = ActiveSourcePhisicalAddr;
                                    B_Len = 3;
                                 }
                                 else
                                 {
                                    B_Len = 1;
                                 }
                                 B_ResponseCode = CEC_MSG_SET_SYSTEM_AUDIO_MODE;
                                 break;
                              case gmd_OCC_RecordOn:
                                 PrepareRecordOnPacket2(gm_UserCommandInterface.Command.BriefCommand.LogicalAddress,Ba_Data,&B_Len);
                                 if (B_Len != 0) NewState = gmd_UCH_WaitQueryResult;
                                 B_ResponseCode = CEC_MSG_RECORD_STATUS;
                                 break;
                              case gmd_OCC_Standby:
                                 Ba_Data[0] = CEC_MSG_STANDBY;
                                 B_Len = 1;
                                 NewState = gmd_UCH_WaitAck;
                                 break;
                              case gmd_OCC_SetAnalogueTimer:
                              case gmd_OCC_ClearAnalogueTimer:
                                 Ba_Data[0] = gm_UserCommandInterface.Command.BriefCommand.OpCode;
                                 //Copying date, time and duration
                                 _fmemcpy(&Ba_Data[1],&gm_UserCommandInterface.Command.Data.DelayedRecord.TimeStamp,6);
                                 Ba_Data[7] = 0; //Record once
                                 FillAnalogBroadcastTypeField(&Ba_Data[8]);
                                 FillFrequencyField(&Ba_Data[9],gm_UserCommandInterface.Command.Data.DelayedRecord.Special.ChannelNumber);
                                 FillTVSystemField(&Ba_Data[11]);
                                 B_Len = 12;
                                 if (gm_UserCommandInterface.Command.BriefCommand.OpCode ==
                                       gmd_OCC_SetAnalogueTimer)
                                    B_ResponseCode = CEC_MSG_TIMER_STATUS;
                                 else
                                    B_ResponseCode = CEC_MSG_TIMER_CLEARED_STATUS;
                                 break;
                              case gmd_OCC_SetExternalTimer:
                              case gmd_OCC_ClearExternalTimer:
                                 Ba_Data[0] = gm_UserCommandInterface.Command.BriefCommand.OpCode;
                                 _fmemcpy(&Ba_Data[1],&gm_UserCommandInterface.Command.Data.DelayedRecord.TimeStamp,6);
                                 Ba_Data[7] = 0; //Record once
                                 Ba_Data[8] = gm_UserCommandInterface.Command.Data.DelayedRecord.InputSource;
                                 if (Ba_Data[8] == gmd_ExternalPlug)
                                 {
                                    Ba_Data[9] = gm_UserCommandInterface.Command.Data.DelayedRecord.Special.ExtPlugNumber;
                                    B_Len = 10;
                                 }
                                 else
                                 {
                                    *((WORD*)(&(Ba_Data[10]))) = gm_UserCommandInterface.Command.Data.DelayedRecord.Special.ExtPhysAddr;
                                    B_Len = 11;
                                 }
                                 if (gm_UserCommandInterface.Command.BriefCommand.OpCode ==
                                       gmd_OCC_SetExternalTimer)
                                    B_ResponseCode = CEC_MSG_TIMER_STATUS;
                                 else
                                    B_ResponseCode = CEC_MSG_TIMER_CLEARED_STATUS;
                                 break;
                                 //fall through
#if 0
                                 Ba_Data[0] = gm_UserCommandInterface.Command.BriefCommand.OpCode;
                                 //Copying date, time and duration
                                 _fmemcpy(&Ba_Data[1],&gm_UserCommandInterface.Command.Data.DelayedRecord.TimeStamp,6);
                                 Ba_Data[7] = 0; //Record once
                                 if (IsHDMIInput(gm_UserCommandInterface.Command.Data.DelayedRecord.CurrentInput))
                                 {
                                    if (GetActiveSourcePhysicalAddress() != 0)
                                    {

                                    }
                                 }
                                 B_Len = 12;
                                 break;
#endif
                              default :
                                 gm_Print("UCH::Unknown user command with Id == %d", gm_UserCommandInterface.Command.BriefCommand.OpCode);
                                 NewState = gmd_UCH_Idle;
                                 break;
                           }
                     }
               }
         }

         switch (NewState)
         {
            case gmd_UCH_Idle:
               gm_Print("UCH::Message hasn't been sent",0);
               gm_UserCommandInterface.Command.BriefCommand.AckResult = gmd_AR_Failed;
               gm_UserCommandInterface.Stage = gmd_UCS_complete;
               break;
            case gmd_UCH_WaitAck:
               B_MsgId = gm_CecMngrTxMessage(B_DestAddr, B_Len, &(Ba_Data[0]), 1, 1);
               gm_Print("UCH::Sending simple message",0);
               gm_Print("UCH::Stage == pending",0);
               gm_UserCommandInterface.Stage = gmd_UCS_pending;
               break;
            case gmd_UCH_WaitQueryResult:
               B_MsgId = gm_CecMngrTxQueryMessage(B_DestAddr, B_Len, Ba_Data, 10, B_ResponseCode, B_ResponseBuffer, 20);
               gm_Print("UCH::Sending query message",0);
               gm_Print("UCH::Stage == pending",0);
               gm_UserCommandInterface.Stage = gmd_UCS_pending;
               break;
            default:
               gm_Print("UCH::Incorrect NewState value = %d",NewState);
               NewState = gmd_UCH_Idle;
               gm_UserCommandInterface.Command.BriefCommand.AckResult = gmd_AR_Failed;
               gm_UserCommandInterface.Stage = gmd_UCS_complete;
         }
         State = NewState;
         RequestTime = gm_GetSystemTime();
         break;
      case gmd_UCH_SendCommand:
         break;
      case gmd_UCH_WaitAck:
         FrameStatus = gm_CecMngrGetTxMessageStatus(B_MsgId);
         if (gm_CecMngrIsTerminalState(FrameStatus))
         {
            gm_CecMngrGetTxMessageHandle(B_MsgId);
            if (FrameStatus == gmd_CEC_MNGR_TX_STS_ACTED_ON)
            {
               //OSDRequestMsg(gmvb_MainChannel, MTO_CEC_Command_OK, REQUEST_NOW);
               gm_UserCommandInterface.Command.BriefCommand.AckResult = gmd_AR_Ok_NotReplied;
            }
            if ((FrameStatus == gmd_CEC_MNGR_TX_STS_NOT_ACKNOWLEDGED) ||
                  (FrameStatus == gmd_CEC_MNGR_TX_STS_ERROR) ||
                  (FrameStatus == gmd_CEC_MNGR_TX_STS_NOT_REPLIED))
            {
               //OSDRequestMsg(gmvb_MainChannel, MTO_CEC_Command_Failed, REQUEST_NOW);
               gm_UserCommandInterface.Command.BriefCommand.AckResult = gmd_AR_Failed;
            }
            if (FrameStatus == gmd_CEC_MNGR_TX_STS_ABORTED)
            {
               gm_UserCommandInterface.Command.BriefCommand.AckResult = gmd_AR_Ok_Aborted;
            }
            State = gmd_UCH_Idle;
            gm_UserCommandInterface.Stage = gmd_UCS_complete;
         }
         break;
      case gmd_UCH_WaitQueryResult:
         FrameStatus = gm_CecMngrGetTxMessageStatus(B_MsgId);
         if (gm_CecMngrIsTerminalState(FrameStatus))
         {
            MsgHandle = gm_CecMngrGetTxMessageHandle(B_MsgId);
            if (FrameStatus == gmd_CEC_MNGR_TX_STS_REPLIED)
            {
               //replied with Deck Status or Record Status
               //gm_Print("UCH::Replied with %02x %02x %02x %02x %02x %02x %02x %02x",
               //gm_Print("UCH::Replied with %d %d %d %d %d %d %d %d",
               gm_Print("UCH::Replied with %x %x %x %x %x %x %x %x",
                        B_ResponseBuffer[0],B_ResponseBuffer[1],B_ResponseBuffer[2],B_ResponseBuffer[3],
                        B_ResponseBuffer[4],B_ResponseBuffer[5],B_ResponseBuffer[6],B_ResponseBuffer[7]);
               //gm_Print("UCH::             %02x %02x %02x %02x %02x %02x %02x %02x",
               gm_Print("UCH::             %x %x %x %x %x %x %x %x",
                        B_ResponseBuffer[8],B_ResponseBuffer[9],B_ResponseBuffer[10],B_ResponseBuffer[11],
                        B_ResponseBuffer[12],B_ResponseBuffer[13],B_ResponseBuffer[14],B_ResponseBuffer[15]);
               switch (MsgHandle.B_RespOpCode)
               {
                  case CEC_MSG_DECK_STATUS:
                  case CEC_MSG_RECORD_STATUS:
                  case CEC_MSG_REPORT_POWER_STATUS:
                  case CEC_MSG_CEC_VERSION:
                  case CEC_MSG_REPORT_PHYSICAL_ADDRESS:
                  case CEC_MSG_SET_MENU_LANGUAGE:
                  case CEC_MSG_SET_OSD_NAME:
                  case CEC_MSG_SET_SYSTEM_AUDIO_MODE:
                  case CEC_MSG_TIMER_STATUS:
                  case CEC_MSG_TIMER_CLEARED_STATUS:
                     gm_UserCommandInterface.Command.BriefCommand.OpCode = MsgHandle.B_RespOpCode;
                     _fmemcpy(gm_UserCommandInterface.Command.Data.Common.Parameters,
                              B_ResponseBuffer,
                              MsgHandle.B_RespLen);
                     gm_UserCommandInterface.Command.Data.Common.Length = MsgHandle.B_RespLen;
                     break;
               }
               gm_UserCommandInterface.Command.BriefCommand.AckResult = gmd_AR_Ok_Replied;
            }
            if (FrameStatus == gmd_CEC_MNGR_TX_STS_NOT_REPLIED)
            {
               //acknowledged but not replied
               gm_Print("UCH::message has been acknowledged but hasn't been replied",0);
               gm_UserCommandInterface.Command.BriefCommand.AckResult = gmd_AR_Ok_NotReplied;
            }
            if (FrameStatus == gmd_CEC_MNGR_TX_STS_ABORTED)
            {
               //feature aborted
               if (MsgHandle.B_RespOpCode == CEC_MSG_FEATURE_ABORT)
               {
                  gm_UserCommandInterface.Command.BriefCommand.OpCode = MsgHandle.B_RespOpCode;
                  gm_UserCommandInterface.Command.BriefCommand.AckResult = gmd_AR_Ok_Replied;
                  gm_Print("UCH::<Abort> successfully <Feature aborted>",0);
               }
               else
               {
                  gm_Print("UCH::message has been acknowledged and aborted",0);
                  gm_UserCommandInterface.Command.BriefCommand.AckResult = gmd_AR_Ok_Aborted;
                  gm_UserCommandInterface.Command.BriefCommand.OpCode = MsgHandle.B_OpCode;
                  /*gm_UserCommandInterface.Data.Parameters[0] = B_ResponseBuffer[0];
                  gm_UserCommandInterface.Data.Parameters[1] = B_ResponseBuffer[1];
                  gm_UserCommandInterface.Data.Parameters[2] = B_ResponseBuffer[2];
                  gm_UserCommandInterface.Data.Length = MsgHandle.B_RespLen;*/
               }
            }
            if (gm_CecUtilIsMessageNotReplied(FrameStatus))
            {
               //not acknowledged or error
               gm_Print("UCH::message hasn't been acknowledged or error occured",0);
               gm_UserCommandInterface.Command.BriefCommand.AckResult = gmd_AR_Failed;
            }
            State = gmd_UCH_Idle;
            gm_UserCommandInterface.Stage = gmd_UCS_complete;
         }
         break;
   }
}

#endif

