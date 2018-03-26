/*
	$Workfile:   d_uart.h  $
	$Revision: 1.6 $
	$Date: 2012/06/06 08:42:31 $
*/
#ifndef __D_UART_H__
#define __D_UART_H__
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
// MODULE:      d_uart.h
//
// USAGE:       Header file for MCU UART driver definition
//
//******************************************************************************

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************

#define SIO_RxBufLen		256
enum SERIAL_STATES
{
   RCV_EMPTY,                              // No data packet
   RCV_READY                               // Data packet received
};

//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************

#ifndef __D_UART_C__
//****EXTERN**** 	extern enum SERIAL_STATES gB_RcvStatus;	// Serial packet status
//****EXTERN**** 	extern WORD gW_RcvCount;					// Serial buffer count
//****EXTERN**** 	extern BYTE gBa_CommBuf[SIO_RxBufLen];	// Serial buffer
#endif

BYTE * huge gm_GetCommBuf(void);


//******************************************************************************
//  G L O B A L    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

#if UART_SEPARATE_BUFFER
void far gm_UartInit0				(DWORD D_CpuClock, DWORD D_BaudRate);
void far gm_UartTrxBuf0				(BYTE *Bp_Data, WORD W_Len);
gmt_RET_STAT gm_UartTrxChar0				(BYTE B_Data);
BOOL far gm_UartIsTrxBufCompleted0	(void);
#endif
BYTE near * far gm_GetCommBuf0(void);
void far gm_StartRcvMsg0(void);
void far gm_StopRcvMsg0(void);
void far gm_Uart_SetRcvCnt0 (WORD W_RcvCount);
WORD far gm_Uart_GetRcvCnt0(void);



#endif
//*********************************  END  **************************************
