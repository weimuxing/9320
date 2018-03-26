/*
	$Workfile:   DpRxDdcci.c  $
	$Revision: 1.14 $
	$Date: 2013/05/30 07:02:04 $
*/
#define __DPRXDDCCI_C__

//******************************************************************************
// COPYRIGHT (C) STMicroelectronics 2010 All rights reserved.
// This document contains proprietary and confidential information of the
// STMicroelectronics Group. This document is not to be copied in whole or part.
// STMicroelectronics assumes no responsibility for the consequences of use of
// such information nor for any infringement of patents or other rights of third
// parties which may result from its use. No license is granted by implication
// or otherwise under any patent or patent rights of STMicroelectronics.
// STMicroelectronics products are not authorized for use as critical components
// in life support devices or systems without express written approval of
// STMicroelectronics.
//******************************************************************************

//==============================================================================
//
// MODULE:      DpRxDdcci.c
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#include "System\all.h"
//#include <string.h>

//******************************************************************************
//  D E F I N E S
//******************************************************************************
#if (FEATURE_DISPLAY_PORT == ENABLE) && defined(DDCCi_PROTOCOL_SUPPORT) 

//#define _DEBUG_MSG
#ifdef _DEBUG_MSG
#define msg(x,y)		gm_Print(x,y)
#else
#define msg(x,y)
#endif

#define EXEC_MEM_DEBUG                106

//******************************************************************************
//  E X T E R N S
//******************************************************************************
void Ddcci(void* gExtDebugCmdPtr, HANDLER_USE_t handler);

//******************************************************************************
//  G L O B AL
//******************************************************************************

//******************************************************************************
//  L O C A L S
//******************************************************************************

//******************************************************************************
//
//  C O D E
//
//******************************************************************************
//******************************************************************************
// FUNCTION:   void DpRxAuxHandler(void)
// USAGE:	   DDC command handler for all DP Aux Channels
// INPUT:	   None
// OUTPUT:	   None
// GLOBALS: 
// USED_REGS:
//******************************************************************************
void DpRxAuxHandler(void)
{
   DpRxDdcCmdHandler(AUX_CH_C0);
   DpRxDdcCmdHandler(AUX_CH_C2);   
}

//******************************************************************************
// FUNCTION:   void DpRxDdcCmdHandler(AUX_CH B_AuxCh)
// USAGE:	   DDC command handler for DP Aux Channel
// INPUT:	   AUX_CH B_AuxCh
// OUTPUT:	   None
// GLOBALS:
// USED_REGS:
//******************************************************************************
void DpRxDdcCmdHandler(AUX_CH B_AuxCh)
{
   if ( ExecuteMemTest[B_AuxCh] == EXEC_MEM_DEBUG )
   {
      BYTE far* Bp_Buff = pSW_Transaction_ModuleDataGlobal[B_AuxCh]->AB_Data;
      
      ExecuteMemTest[B_AuxCh] = 0;
      gDebugCmdPtr = Bp_Buff + Start_of_GprobeCmd_in_DDC2BiMsg;
      if((Bp_Buff[2] == 0xC2) && (Bp_Buff[3] == 0) && (Bp_Buff[4] == 0))
      {
         gm_GprobeCmdParser(B_AuxCh + AUX_C0_USE, (BYTE *)(pSW_Transaction_ModuleDataGlobal[B_AuxCh]->AB_Data));
      }
      else
      {      
     #if 1//defined(CMI_73XX_BOARD)
	     if (UserPrefDdcciOnOffFlag == ENABLE) //chunbao for osd->ddcci disable ddc channel)
	     	{
      Ddcci((void *)pSW_Transaction_ModuleDataGlobal[B_AuxCh]->AB_Data, B_AuxCh + AUX_C0_USE);
   }
	 #else
		 Ddcci((void *)pSW_Transaction_ModuleDataGlobal[B_AuxCh]->AB_Data, B_AuxCh + AUX_C0_USE);
	 #endif
      }
}
}

//******************************************************************************
// FUNCTION       : void DpRxDdcciAuxSetReply(BYTE *B_BuffPtr, AUX_CH B_AuxCh)
// USAGE          : Reply DDCCI via AuxRx channels
// INPUT          : B_BuffPtr
// OUTPUT         : None
// GLOBALS        : None
// USED_REGS      : None
//******************************************************************************
void DpRxDdcciAuxSetReply(BYTE *B_BuffPtr, AUX_CH B_AuxCh)
{
   BYTE B_Checksum, B_Length, Index;
   SW_TransactionStruct_t *SW_TrnsData = pSW_Transaction_ModuleDataGlobal[B_AuxCh];

   B_Checksum = ((DDC2B_SRC_ADDRESS-1) ^ SW_TrnsData->B_DevID);
   B_Length = B_BuffPtr[0] ^ DDC2Bi_CONTROL_STATUS_FLAG;

   for (Index=0; Index < B_Length + 1; Index++)			// Process the whole packet
   {
      B_Checksum ^= B_BuffPtr[Index]; 			  // Update checksum
      //gm_Print("R 0x%X",B_BuffPtr[Index]);
   }

   B_BuffPtr[B_Length + 1] = B_Checksum;
   B_BuffPtr[B_Length + 2] = DDC2B_DEST_ADDRESS;

   SW_TrnsData->B_ReplyReady = TRUE;

}

/*ChengLin 20130514: Enable DP1.2 DDC/CI Supporting*/
#if (ENABLE_DP12_DDCCI_SUPPORT)
//******************************************************************************
// FUNCTION       : DDC_I2CSlaveCheckCS
// USAGE          : Calculate Checksum for DP 1.2 DDCCI packages
// INPUT          : B_DevID, B_Len, *Bp_Buff
// OUTPUT         : TRUE or FALSE
// GLOBALS        : None
// USED_REGS      : None
//******************************************************************************
BOOL DDC_I2CSlaveCheckCS(BYTE B_DevID, BYTE B_Len, BYTE far*Bp_Buff)
{
   BYTE B_Count;
   BYTE B_Checksum = (B_DevID & 0xFE);           // Init checksum

   for (B_Count = 0; B_Count < (B_Len + 2); B_Count++)       // Calculate packet checksum
   {
      B_Checksum ^= Bp_Buff[B_Count];
      //gm_Print("0x%X",Bp_Buff[B_Count]);
   }

   if (B_Checksum == Bp_Buff[(B_Len + 2)])                  // Checksum correct?
   {
      return TRUE;                                          // Yes
   }
   return FALSE;                                            // No
}

//******************************************************************************
// FUNCTION       : DpRxDdcciMultiStreamTransaction
// USAGE          : Process DDCCI protocols over AUXI2C in DP1.2 mode
// INPUT          :  *I2cTransaction, stopbit
// OUTPUT         : None
// GLOBALS        : None
// USED_REGS      : None
//******************************************************************************
void DpRxDdcciMultiStreamTransaction(Aux_data_buffer_t * I2cTransaction, U8 stopbit)
{
   WORD wCount = 0;
   BYTE B_Count = 0;

   if((I2cTransaction->Addr_32 != (U32)(DDC2BI_ADDR_ID>>1)))
      return;

   #if 0 //For Debug Only 
      gm_Print("\nInProgress = %d.\n",I2cTransaction->InProgress);
      gm_Print("\nAddr_32 = 0x%x.\n",I2cTransaction->Addr_32);            
      gm_Print("\nReadRequest = %d.\n",I2cTransaction->ReadRequest); 
      gm_Print("\nCmd_8= 0x%x.\n",I2cTransaction->Cmd_8); 
      gm_Print("\nRequestDataLen_16= %d.\n",I2cTransaction->RequestDataLen_16); 
      gm_Print("\nHandledDataLen_16= %d.\n",I2cTransaction->HandledDataLen_16);
      gm_Print("\nAuxTransactionStatus_t= %d.\n",I2cTransaction->AuxTransactionStatus_t);
      gm_Print("\nRetrycounter_8= %d.\n",I2cTransaction->Retrycounter_8);
      gm_Print("\nLength_8= %d.\n",I2cTransaction->Length_8);
      gm_Print("\nNumberOfReads_8= %d.\n",I2cTransaction->NumberOfReads_8);
      gm_Print("\nstopbit= %d.\n",stopbit);
      gm_Print("\nI2cTransaction->RequestDataLen_16 = %d.\n",I2cTransaction->RequestDataLen_16 ); 
      gm_Print("\npSW_Transaction_ModuleDataGlobal[AUX_CH_C0]->W_SendDevID= %d.\n",pSW_Transaction_ModuleDataGlobal[AUX_CH_C0]->W_SendDevID);    
      gm_Print("\npSW_Transaction_ModuleDataGlobal[AUX_CH_C0]->W_CurCount= %d.\n",pSW_Transaction_ModuleDataGlobal[AUX_CH_C0]->W_CurCount);
      gm_Print("\npSW_Transaction_ModuleDataGlobal[AUX_CH_C0]->B_NullMsgPtr= %d.\n",pSW_Transaction_ModuleDataGlobal[AUX_CH_C0]->B_NullMsgPtr);  
      gm_Print("\n\n\n",0);            
   #endif    

   if (I2cTransaction->AuxTransactionStatus_t == AUX_REQ_NEW)
   {
      if(pSW_Transaction_ModuleDataGlobal[AUX_CH_C0]->B_Dir != (!I2cTransaction->ReadRequest))
      {
          //open new transaction 
          pSW_Transaction_ModuleDataGlobal[AUX_CH_C0]->W_CurCount = 0;
          pSW_Transaction_ModuleDataGlobal[AUX_CH_C0]->B_Dir = (!I2cTransaction->ReadRequest);   
          pSW_Transaction_ModuleDataGlobal[AUX_CH_C0]->W_SendDevID = 0;
          pSW_Transaction_ModuleDataGlobal[AUX_CH_C0]->B_DevID = (DDC2BI_ADDR_ID & 0xFE);
      }
      I2cTransaction->AuxTransactionStatus_t = AUX_REP_DEFER;
   }
   else
   {
      if((!I2cTransaction->ReadRequest))
      {  // we have received data             
           for(wCount = 0; wCount < I2cTransaction->RequestDataLen_16; wCount++)
              pSW_Transaction_ModuleDataGlobal[AUX_CH_C0]->AB_Data[ (pSW_Transaction_ModuleDataGlobal[AUX_CH_C0]->W_CurCount++)] = I2cTransaction->ReqBuff_p[wCount];

            if(stopbit)
            {
                BYTE  B_Len;
                pSW_Transaction_ModuleDataGlobal[AUX_CH_C0]->B_ReplyReady = FALSE;
                pSW_Transaction_ModuleDataGlobal[AUX_CH_C0]->B_NullMsgPtr = 0;

                // Check total packet length received 
                B_Len =  (~DDC2Bi_CONTROL_STATUS_FLAG) &  pSW_Transaction_ModuleDataGlobal[AUX_CH_C0]->AB_Data[1]; // packet length 
         
                if( pSW_Transaction_ModuleDataGlobal[AUX_CH_C0]->W_CurCount >= (B_Len + 3))
                {                                                                                // Message is received 
                    if(DDC_I2CSlaveCheckCS( pSW_Transaction_ModuleDataGlobal[AUX_CH_C0]->B_DevID, B_Len, & pSW_Transaction_ModuleDataGlobal[AUX_CH_C0]->AB_Data[0]))
                    {
                       ExecuteMemTest[AUX_CH_C0] = EXEC_MEM_DEBUG;
                    }
                }
            }
      }
      else
      {
         if(I2cTransaction->RequestDataLen_16 == 0)
           return;
 
         if((pSW_Transaction_ModuleDataGlobal[AUX_CH_C0]->B_ReplyReady) && (pSW_Transaction_ModuleDataGlobal[AUX_CH_C0]->B_NullMsgPtr == 0))
         {
             if ((pSW_Transaction_ModuleDataGlobal[AUX_CH_C0]->W_CurCount == 0) && (pSW_Transaction_ModuleDataGlobal[AUX_CH_C0]->W_SendDevID == 0))
             {
                pSW_Transaction_ModuleDataGlobal[AUX_CH_C0]->W_SendDevID = 1;
                I2cTransaction->RepBuff_p[0] = pSW_Transaction_ModuleDataGlobal[AUX_CH_C0]->B_DevID;
                B_Count = 1;
             }
             else
             {
                B_Count = 0;                       
             }
                                    
             for(; B_Count < (I2cTransaction->RequestDataLen_16); B_Count++)
                I2cTransaction->RepBuff_p[B_Count] = pSW_Transaction_ModuleDataGlobal[AUX_CH_C0]->AB_Data[(pSW_Transaction_ModuleDataGlobal[AUX_CH_C0]->W_CurCount++)];

                           
              /*if(stopbit)
                            {
                               pSW_Transaction_ModuleDataGlobal[AUX_CH_C0]->B_ReplyReady = FALSE;
                            }*/
          }
          else
          {
             for(B_Count = 0; B_Count < I2cTransaction->RequestDataLen_16; B_Count++)
             {
                switch( pSW_Transaction_ModuleDataGlobal[AUX_CH_C0]->B_NullMsgPtr++)
                {
                    case 1: I2cTransaction->RepBuff_p[B_Count] = NULL_MSG_SECOND_BYTE;
                           break;
                    case 2: I2cTransaction->RepBuff_p[B_Count] = INIT_NULL_MSG_CS ^ pSW_Transaction_ModuleDataGlobal[AUX_CH_C0]->B_DevID;
                           break;
                    case 0:
                    default: I2cTransaction->RepBuff_p[B_Count] = pSW_Transaction_ModuleDataGlobal[AUX_CH_C0]->B_DevID;
                           break;
                }
                           
                if( pSW_Transaction_ModuleDataGlobal[AUX_CH_C0]->B_NullMsgPtr == NULL_MSG_LENGTH)
                   pSW_Transaction_ModuleDataGlobal[AUX_CH_C0]->B_NullMsgPtr = 0;
             }
                           
             if(stopbit)
             {
                pSW_Transaction_ModuleDataGlobal[AUX_CH_C0]->B_NullMsgPtr = 0;
             }
          }
      }
         I2cTransaction->AuxTransactionStatus_t = AUX_REQ_SUCCESS; 
    }
}
#endif

#endif

