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
// MODULE:      CecLibReplace.c
//
// USAGE:       This file contains overrided functions from CEC library
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

#if (FEATURE_CEC == ENABLE) && defined(LPM_PERMANENTLY_CONTROLS_PERIPHERAL)

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

BYTE B_CecFrameStatusLpm = 0;
BYTE B_LogAddressLpm = 0;
BYTE B_CecBusStatusLpm = 0;
DWORD DW_CecTxTimeLpm = 0;

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

#define CEC_RX_DATA_BUFFER_SIZE CEC_FRAMES_BUFFER_SIZE
//******************************************************************************
//  L O C A L    F U N C T I O N S
//******************************************************************************

BYTE CecReadByteFromRxBuf(void);

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
// USAGE: This function is used for transmission of entire frame according to
//		frame requirements (HDMI Specification Version 1.3a, CEC 6). It starts
//		transmission process which proceeds in background mode. In case of
//		unsuccessful transmission of frame it will be retransmitted in predefined
//		period of time.
//
// INPUT:
// OUTPUT:
// GLOBALS:
//******************************************************************************
BOOL gm_CecTxFrame(BYTE B_DestLogAddr, BYTE B_DataLen, BYTE * Bp_Data)
{
   BYTE Ba_Data[20];
   BYTE i;

   msg("App: gm_CecTxFrame");

   Ba_Data[0] = CEC_IPC_DATA;
   Ba_Data[1] = B_DestLogAddr;
   Ba_Data[2] = B_DataLen;

   msg_i("DestAddr 0x%x", B_DestLogAddr);
   msg_i("B_DataLen 0x%x", B_DataLen);

   for (i = 0; i < B_DataLen; i++)
   {
      Ba_Data[i + 3] = Bp_Data[i];
   }

   for (i = 0; i < (B_DataLen + 3); i++)
   {
      msg_i("Ba_Data 0x%x", Ba_Data[i]);
   }

   msg_i("Time %d", (WORD)gm_GetSystemTime());

   gm_RegMsgSd(gmd_MESSAGE_CEC, Ba_Data, B_DataLen + 3);
   //gm_MessageSend(gmd_MESSAGE_CEC, (BYTE far *)&Ba_Data[0], 6);

   return TRUE;
}

//******************************************************************************
// USAGE: This function is used to get the oldest frame from Rx buffer; frame is
//		removed from Rx buffer after this function call.
//
// INPUT:
// OUTPUT:
// GLOBALS:
//******************************************************************************
BOOL gm_CecRxFrame(BYTE * B_SrcLogAddr, BYTE * B_DestLogAddr, BYTE * B_DataLen, BYTE * Bp_Data)
{
   BYTE i;

   if (B_CecRxWrIndex == B_CecRxRdIndex)
   {// CEC Rx buffer is empty
      *(BYTE*) B_DataLen = 0;
      return FALSE;
   }

   *(BYTE*) B_SrcLogAddr   = CecReadByteFromRxBuf();
   *(BYTE*) B_DestLogAddr  = CecReadByteFromRxBuf();
   *(BYTE*) B_DataLen      = CecReadByteFromRxBuf();

   msg("gm_CecRxFrame");

   msg_i("B_SrcLogAddr 0x%x", *(BYTE*) B_SrcLogAddr);
   msg_i("B_DestLogAddr 0x%x", *(BYTE*) B_DestLogAddr);
   msg_i("B_DataLen 0x%x", *(BYTE*) B_DataLen);

   if ((*(BYTE*) B_DataLen) == 0)
   {
      return FALSE;
   }

   for (i=0;i<(*(BYTE*) B_DataLen);i++)
   {
      Bp_Data[i] = CecReadByteFromRxBuf();
      msg_i("Bp_Data 0x%x", Bp_Data[i]);
   }

   return TRUE;
}

//******************************************************************************
// USAGE:  This function is used to get frame status bits.
//
// INPUT:   None
// OUTPUT:	None
// GLOBALS:
//******************************************************************************
BYTE gm_CecGetFrameStatus(void)
{
   //gm_Print("App: CecGetFrameStatus",0);
   return B_CecFrameStatusLpm;
}

//******************************************************************************
// USAGE:  This function is used to clear frame status bits.
//
// INPUT:   None
// OUTPUT:	None
// GLOBALS:
//******************************************************************************
void CEC_EXT_FUNC gm_CecClearFrameStatusBits(BYTE W_CecFrameStatusMask)
{
   BYTE Ba_Data[2];

   msg_i("ClearFrameStatusBits 0x%x", W_CecFrameStatusMask);

   B_CecFrameStatusLpm &= ~ W_CecFrameStatusMask;

   Ba_Data[0] = CEC_IPC_FRAME_STATUS;
   Ba_Data[1] = W_CecFrameStatusMask;

   gm_RegMsgSd(gmd_MESSAGE_CEC, Ba_Data, 2);
}

//******************************************************************************
// USAGE:  This function is used to set TV logical address
//
// INPUT:   None
// OUTPUT:	None
// GLOBALS:
//******************************************************************************
void gm_CecSetLogAddr(BYTE B_LogAddr)
{
   BYTE Ba_Data[2];

   B_LogAddressLpm = B_LogAddr;

   Ba_Data[0] = CEC_IPC_SET_LOG_ADDR;
   Ba_Data[1] = B_LogAddressLpm;

   gm_RegMsgSd(gmd_MESSAGE_CEC, Ba_Data, 2);
}

//******************************************************************************
// USAGE: This function is used to get TV logical address
//
// INPUT:   None
// OUTPUT:	None
// GLOBALS:
//******************************************************************************
BYTE gm_CecGetLogAddr(void)
{
   return B_LogAddressLpm;
}

//******************************************************************************
// USAGE:  This function is used to get status of CEC bus
//
// INPUT:   None
// OUTPUT:	None
// GLOBALS:
//******************************************************************************
BYTE gm_CecGetBusStatus(void)
{
   return B_CecBusStatusLpm;
}

//******************************************************************************
// USAGE: This function is used to get time when last frame transmission was
//		finished
//
// INPUT:   None
// OUTPUT:	None
// GLOBALS:
//******************************************************************************
DWORD CEC_EXT_FUNC gm_CecGetTxTime(void)
{
   //gm_Print("gm_CecGetTxTime %d", (WORD)DW_CecTxTimeLpm);
   return DW_CecTxTimeLpm;
}

//******************************************************************************
// USAGE:   Read byte from Rx buffer
//
// INPUT:
// OUTPUT:
// GLOBALS:
//******************************************************************************
BYTE CecReadByteFromRxBuf(void)
{
   BYTE B_RxData = Ba_CecRxData[B_CecRxRdIndex];

   B_CecRxRdIndex++;

   if (B_CecRxRdIndex >= CEC_RX_DATA_BUFFER_SIZE)
   {
      B_CecRxRdIndex = 0;
   }

   return B_RxData;
}

#endif // #if (FEATURE_CEC == ENABLE) && defined(LPM_PERMANENTLY_CONTROLS_PERIPHERAL)
