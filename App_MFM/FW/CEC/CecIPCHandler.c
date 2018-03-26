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
// MODULE:      CecIPCHandler.c
//
// USAGE:       This file contains functions for CEC IPC functionality
//
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "system\all.h"
#include "StdLibInc\gm_Register.h"
#include "StdLibInc\gm_Mcu186.h"
#include <embedded.h>
#include <mem.h>
#include <string.h>

#if (FEATURE_CEC == ENABLE) && defined(LOW_POWER_MONITOR_SUPPORT)

#include "CEC\CecHandler.h"
#include "StdLibInc\gm_LPM_IPC.h"
#include "CEC\CecIPCHandler.h"
#include "CEC\CecHandler.h"

//******************************************************************************
//  G L O B A L    F U N C T I O N S
//******************************************************************************

//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************

#ifdef LPM_PERMANENTLY_CONTROLS_PERIPHERAL

#if 0
DWORD DW_SystemTimeLKM;
DWORD DW_SystemTimeHost;
#endif

extern BYTE B_CecFrameStatusLpm;
extern BYTE B_LogAddressLpm;
extern BYTE B_CecBusStatusLpm;
extern DWORD DW_CecTxTimeLpm;

#endif

extern BYTE Ba_CecRxData[CEC_FRAMES_BUFFER_SIZE];
extern BYTE B_CecRxWrIndex;
extern BYTE B_CecRxRdIndex;

//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************
#ifdef _DEBUG_MSG
#define msg(x)            gm_Print(x,0)
#define msg_i(x,y)        gm_Print(x,y)
#else
#define msg(x)
#define msg_i(x,y)
#endif

//******************************************************************************
//  L O C A L    F U N C T I O N S
//******************************************************************************

BOOL CecWriteByteToRxBuf(BYTE B_Data);

//******************************************************************************
//  L O C A L    V A R I A B L E S
//******************************************************************************

//******************************************************************************
//  M A C R O   D E F I N I T I O N
//******************************************************************************

//******************************************************************************
//  C O D E
//******************************************************************************

//******************************************************************************
// USAGE:   CecIPCHandler
//
// INPUT:   None
// OUTPUT:	None
// GLOBALS:
//******************************************************************************

void CecIpcHandler(void)
{
   BYTE Ba_Data[20];
   BYTE i;
   DWORD DW_Time;

   WORD W_MessageLength;

#ifdef LPM_PERMANENTLY_CONTROLS_PERIPHERAL
#if 0
   static WORD W_PrevTime = 0;

   if ((gm_GetSystemTime()/1000) != W_PrevTime)
   {
      W_PrevTime = (WORD)(gm_GetSystemTime()/1000);

      Ba_Data[0] = CEC_IPC_GET_TIME;

      gm_MessageSend(gmd_MESSAGE_CEC, Ba_Data, 1);
   }
#endif
#endif

   W_MessageLength = gm_RegMsgRv(gmd_MESSAGE_CEC, (BYTE far *)&Ba_Data[0]);

   if (W_MessageLength)
   {
      msg_i("MsgLength %d",W_MessageLength);

      switch (Ba_Data[0])
      {
         case CEC_IPC_DATA:
            msg("CEC_DATA");

            Ba_Data[3] += 4;

            // Copy data from Temporary buffer to queue
            for (i = 0;i < (W_MessageLength - 1); i++)
            {
               CecWriteByteToRxBuf(Ba_Data[i + 1]);
               msg_i("0x%x", Ba_Data[i + 1]);
            }

            {
               DWORD D_Time = gm_GetSystemTime();
               BYTE* B_Time = (BYTE*) &D_Time;

               for (i = 0; i < 4; i++)
               {
                  CecWriteByteToRxBuf((BYTE)*(BYTE*)(B_Time++));
               }
            }


#ifdef LPM_PERMANENTLY_CONTROLS_PERIPHERAL
            B_CecFrameStatusLpm |= gmd_CEC_FRAME_RX_DONE;
#endif
            break;

#ifdef LPM_PERMANENTLY_CONTROLS_PERIPHERAL
         case CEC_IPC_FRAME_STATUS:

            // Needn't analyse gmd_CEC_FRAME_RX_DONE on Mission side
            B_CecFrameStatusLpm = ((B_CecFrameStatusLpm & gmd_CEC_FRAME_RX_DONE) | (Ba_Data[1] & (~gmd_CEC_FRAME_RX_DONE)));

            if ((B_CecFrameStatusLpm & (gmd_CEC_FRAME_TX_DONE | gmd_CEC_FRAME_TX_ACK)) == (gmd_CEC_FRAME_TX_DONE | gmd_CEC_FRAME_TX_ACK))
            {
#if 0
               DW_CecTxTimeLpm = 0;
               DW_Time = 0;

               for (i = 2; i < 6; i++)
               {
                  msg_i("0x%x", Ba_Data[i]);
                  //gm_Print("0x%x",(BYTE)*(BYTE*)(B_Time++));

                  DW_CecTxTimeLpm += ((DWORD)Ba_Data[i] << (8 * (i - 2)));
               }

               msg_i("DW_CecTxTimeLPM %d", (WORD)DW_CecTxTimeLpm);

               for (i = 6; i < 10; i++)
               {
                  msg_i("0x%x", Ba_Data[i]);
                  //gm_Print("0x%x",(BYTE)*(BYTE*)(B_Time++));

                  DW_Time += ((DWORD)Ba_Data[i] << (8 * (i - 6)));
               }

               msg_i("DW_Time %d", (WORD)DW_Time);

               DW_CecTxTimeLpm = gm_GetSystemTime() -  (DW_Time - DW_CecTxTimeLpm);
#endif

               DW_CecTxTimeLpm = gm_GetSystemTime();

               msg_i("DW_CecTxTimeLPM %d", (WORD)DW_CecTxTimeLpm);

            }

            msg_i("CEC_FRAME_STATUS 0x%x",B_CecFrameStatusLpm);
            break;

         case CEC_IPC_BUS_STATUS:
            B_CecBusStatusLpm = Ba_Data[1];

            msg_i("CEC_BUS_STATUS 0x%x",B_CecBusStatusLpm);
            //gm_Print(" 0x%x",Ba_Data[2]);
            break;

#if 0
         case CEC_IPC_GET_TIME:
            {
               static DWORD DW_SystemTimeLKMPrev = 0;

               DW_SystemTimeHost = gm_GetSystemTime();

               DW_SystemTimeLKM = 0;

               for (i = 1; i < 5; i++)
               {
                  //gm_Print("0x%x", Ba_Data[i]);
                  //gm_Print("0x%x",(BYTE)*(BYTE*)(B_Time++));

                  DW_SystemTimeLKM += ((DWORD)Ba_Data[i] << (8 * (i - 1)));
               }

               msg_i("LPM time: %d", (WORD)(DW_SystemTimeLKM - DW_SystemTimeLKMPrev));

               DW_SystemTimeLKMPrev = DW_SystemTimeLKM;

#if 0
               //DW_Time = Ba_Data[1] + ((DWORD)Ba_Data[2] << 8) + ((DWORD)Ba_Data[3] << 16) + ((DWORD)Ba_Data[4] << 24);

               if (DW_Time > D_Time)
               {
                  msg_i("%d", DW_Time - D_Time);

                  if ((DW_Time - D_Time) > B_CecMaxTime)
                  {
                     B_CecMaxTime = (BYTE)(DW_Time - D_Time);

                     msg_i("B_CecMaxTime %d", B_CecMaxTime);
                  }

                  if ((DW_Time - D_Time) < B_CecMinTime)
                  {
                     B_CecMinTime = (BYTE)(DW_Time - D_Time);

                     msg_i("B_CecMinTime %d", B_CecMinTime);
                  }
               }
               else
               {
                  msg_i("%d", D_Time - DW_Time);

                  if ((D_Time - DW_Time) > B_CecMaxTime)
                  {
                     B_CecMaxTime = (BYTE)(D_Time - DW_Time);

                     msg_i("B_CecMaxTime %d", B_CecMaxTime);
                  }

                  if ((D_Time - DW_Time) < B_CecMinTime)
                  {
                     B_CecMinTime = (BYTE)(D_Time - DW_Time);

                     msg_i("B_CecMinTime %d", B_CecMinTime);
                  }
               }
#endif
            }
            break;
#endif
#endif
      }
   }
}

//******************************************************************************
// USAGE:  Write byte to Rx buffer
//
// INPUT:
// OUTPUT:
// GLOBALS:
//******************************************************************************
BOOL CecWriteByteToRxBuf(BYTE B_Data)
{
   if ( ( ((B_CecRxWrIndex + 1) == CEC_FRAMES_BUFFER_SIZE) && (B_CecRxRdIndex == 0) ) ||
         ((B_CecRxWrIndex + 1) == B_CecRxRdIndex) )
   {
      return 0;
   }

   Ba_CecRxData[B_CecRxWrIndex] = B_Data;

   B_CecRxWrIndex++;

   if (B_CecRxWrIndex == CEC_FRAMES_BUFFER_SIZE)
   {
      B_CecRxWrIndex = 0;
   }

   return	1;
}

#endif