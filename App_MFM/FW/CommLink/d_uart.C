/*
	$Workfile:   d_uart.C  $
	$Revision: 1.15 $
	$Date: 2012/01/13 08:59:22 $
*/
#define __D_UART_C__
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
// MODULE:      d_uart.c
//
// USAGE:       This module contains functions for UART communication
//              (VAutomation Turbo-186)
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include <string.h>
#include "StdLibInc\gm_Global.h"
#include "StdLibInc\gm_Mcu186.h"
#include "StdLibInc\gm_DrvLib.h"
#include "system\all.h"
#include "StdLibInc\gm_Register.h"
#include "commlink\d_uart.h"


typedef void (far *  const ptVoidFunct)(void);


extern WORD uart0RcvCount;
extern WORD uartRcvCount;
extern void  far gm_Uart_SetRcvCnt                (WORD W_RcvCount);

//extern const far BYTE AckPkt[];
//extern const far BYTE NakPkt[];


//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
void far interrupt Uart0Int		(void);

//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************
//
// common for Serial and Ddc2bi communication
//
//****Global**** enum SERIAL_STATES gB_RcvStatus;		// Serial packet status
//****Global**** WORD gW_RcvCount;						// Serial buffer count
//****Global**** BYTE gBa_CommBuf[SIO_RxBufLen+1]; 		// Serial buffer
#if UART_SEPARATE_BUFFER
BYTE gB_RcvStatus0;
WORD W_TrxCount0;
WORD gW_RcvCount0;
BYTE * gBp_Uart0CommBuf0;
BYTE gBa_CommBuf0[SIO_RxBufLen];
BYTE * Bp_TrxBuf0;
WORD gW_Uart0CommBufLen0 = SIO_RxBufLen;

//******************************************************************************
//  S T A T I C     V A R I A B L E S
//******************************************************************************

//****Global**** static WORD W_TrxCount;					// Transmit buffer count
//****Global**** static BYTE *Bp_TrxBuf;					// Transmit buffer pointer

//******************************************************************************
//  C O D E
//******************************************************************************

//******************************************************************************
//
// FUNCTION:    void gm_UartInit(DWORD D_CpuClock, DWORD D_BaudRate)
// USAGE:       Initialization of UART_0 Control Registers, setup baudrate and
//				interrupt service routine.
//
// INPUT:       D_CpuClock = MCU clock
//				D_BaudRate = UART baud rate
// OUTPUT:      None
// GLOBALS:     gB_RcvStatus, gW_RcvCount, W_TrxCount
// USED_REGS:	None
//
//******************************************************************************

void gm_UartInit0(DWORD D_CpuClock, DWORD D_BaudRate)
{
   DWORD D_BaudRegister;

   //
   // Initialize local variables
   //
   gB_RcvStatus0 = RCV_EMPTY;
   W_TrxCount0  = 0;
   gW_RcvCount0  = 0;

   // Initialize the Uart0 comm buffer to the beginning of System Comm Buffer
   gBp_Uart0CommBuf0 = gBa_CommBuf0;

   //
   // Setup Serial Port0 - SP0BAUD = D_CpuClock / (D_BaudRate * 16)
   //
   D_BaudRate *= 16;
   D_BaudRegister = D_CpuClock / D_BaudRate;  	// Calculate baudrate value
   if ((((D_CpuClock % D_BaudRate)*100)/D_BaudRate) > 60)
      D_BaudRegister++;

   SET_VECTOR(UART0_VECTOR, Uart0Int);			// Set interrupt vector
   WRITE_PCB_REG(SP0BAUD, D_BaudRegister);		// Set baudrate
   WRITE_PCB_REG(SP0CT, (RxNoDMATxNoDMA|RXIE|TMODE|RMODE|DATAMODE1));
   CLEAR_PCB_REG_BITS(IMASK, SP0);
}

//******************************************************************************
//
// FUNCTION:   	void gm_UartTrxChar(BYTE B_Data)
// USAGE:      	Transmit a single byte of data through UART
//
// INPUT:      	B_Data - data to send
// OUTPUT:      None
// GLOBALS:		None
// USED_REGS:   None
//
//******************************************************************************
void far gm_UartTrxChar0(BYTE B_Data)
{
   while (gm_UartIsTrxBufCompleted0()==FALSE); // Make sure the trx buffer is cleared
   WRITE_PCB_REG(SP0TD, B_Data);				// Send data
   while (!(READ_PCB_REG(SP0STS) & THRE))		// wait for tx ready to be set
      {}
}

//******************************************************************************
//
// FUNCTION:   	void gm_UartTrxBuf(BYTE *Bp_Data, WORD W_Len)
// USAGE:      	Transmit a buffer of data through UART
//
// INPUT:      	Bp_Data - buffer pointer
//				W_Len   - number of byte to transmit
// OUTPUT:      None
// GLOBALS:     Bp_TrxBuf, W_TrxCount
// USED_REGS:   None
// NOTES:		Application need to check before or after transmission
//				to confirm data have been transmitted.
//				The function "UartIsTrxCompleted" should be used.
//
//				It is using the serial interrupt transmission service
//
//******************************************************************************
void far gm_UartTrxBuf0(BYTE *Bp_Data, WORD W_Len)
{
   while (gm_UartIsTrxBufCompleted0()==FALSE);  // Make sure the trx buffer is cleared
   Bp_TrxBuf0  = Bp_Data;						// Save buffer pointer
   W_TrxCount0 = W_Len; 						// Adjust counter
   SET_PCB_REG_BITS(SP0CT, TXIE);				// Enable transmit interrupt
}
//******************************************************************************
//
// FUNCTION:   	BOOL gm_UartIsTrxBufCompleted(void)
// USAGE:      	Check whether all data in transmit buffer is completed.
//
// INPUT:      	None
// OUTPUT:      TRUE if transmission complete flag is set and
//                      number of bytes left in the buffer is 0
//				FALSE if number of bytes left in the buffer is not 0
// GLOBALS:		W_TrxCount
// USED_REGS:	None
//
// NOTES:       The UART is double buffered, checking the TEMT is to
//              garantee the last bit has been sent.
//******************************************************************************
BOOL far gm_UartIsTrxBufCompleted0(void)
{
   if ( (READ_PCB_REG(SP0STS) & TEMT) && (W_TrxCount0==0) )
      return TRUE;

   return FALSE;
}

//******************************************************************************
//
// FUNCTION:    interrupt Uart0Int(void)
// USAGE:       Mcu UART0 interrupt service routine and save UART data into
//				receive buffer.
//
// INPUT:       None
// OUTPUT:      None
// GLOBALS:		gB_RcvStatus, gW_RcvCount, W_TrxCount, gBp_Uart0CommBuf
// USED_REGS:	None
//
//******************************************************************************
void far interrupt Uart0Int(void)
{
   volatile BYTE dummy;

   asm
   {
      sti 									// Allow other interrupt
   }                                           //

   if (READ_PCB_REG(SP0STS)& RDR)				// Check if new char received
   {
      if (gB_RcvStatus0 == RCV_EMPTY &&		// Packet not received yet
      gW_RcvCount0 < gW_Uart0CommBufLen0)	// Buffer not full
      {										// Get data from UART
         gm_TimerStart(UART_10ms_TMR,10);	// Start receive timer
         gBp_Uart0CommBuf0[gW_RcvCount0] = READ_PCB_REG(SP0RD);
         gW_RcvCount0++;
      }
      else
      {
         dummy = READ_PCB_REG(SP0RD);		// Read data to clear interrupt
      }
   }
   if (W_TrxCount0)                     	    // Data available to transmit ?
   {											// Yes
      if (READ_PCB_REG(SP0STS) & THRE)		// Ready to transmit ?
      {
         WRITE_PCB_REG(SP0TD, *Bp_TrxBuf0);	// Send data
         W_TrxCount0--;           			// Dec trx count
         Bp_TrxBuf0++;                   		// Point to next byte
         if (W_TrxCount0 == 0)				// No more data ?
            CLEAR_PCB_REG_BITS(SP0CT, TXIE);// Disable transmit interrupt
      }
   }

   asm { cli } //to avoid interrupt between next command and iret

   WRITE_PCB_REG(EOI, UART0_VTYPE);			// Clear interrupt
}


extern BYTE far * far gDebugCmdPtr;

BYTE isSerialMsgCompleted0(void)
{
   BYTE len, checksum;

   //
   // Check total packet length received
   // if the driver already receives enough data, process it
   //
   if ((gW_RcvCount0 > 0) && (gW_RcvCount0 >= gBp_Uart0CommBuf0[0]))	// Packet received ?
   {												      // Yes
      len = gBp_Uart0CommBuf0[0];             // packet length
      //
      //	prepare for the next receive message
      //
      gW_RcvCount0 = 0;                       // reset for next packet
      gDebugCmdPtr = (BYTE *)gBp_Uart0CommBuf0; // point to beginning of cmd packet

      //
      //	verify the message correctness
      //
      for ( checksum=0; len > 0;  )          // Calculate packet checksum
      {
         checksum += gBp_Uart0CommBuf0[--len];
      }

      if (checksum == 0)                     // Checksum correct
      {
         return TRUE;                        // Yes
      }
      else
      {
         gm_WriteCmdPkt((BYTE far*)NakPkt, SERIAL_USE);// Send NAK
         return FALSE;                       // No
      }
   }

   //
   // If inter-character timeout occured, reset the receive counter
   // and discard the received data
   //
   if (gW_RcvCount0 > 0 && (gm_TimerCheck(UART_10ms_TMR)==TIMER_TMO) )
   {
      gW_RcvCount0 = 0;
   }
   return FALSE;
}

#endif // UART_SEPARATE_BUFFER

void far gm_StopRcvMsg0(void)
{
#if UART_SEPARATE_BUFFER
   gB_RcvStatus0 = RCV_READY;						// stop receiving messages
#else
   gm_StopRcvMsg();
#endif

}


void far gm_StartRcvMsg0(void)
{
#if UART_SEPARATE_BUFFER
   gB_RcvStatus0 = RCV_EMPTY;						// start receiving messages
#else
   gm_StartRcvMsg();
#endif
}

#if 0 // not used
PARSER_STATUS_t far gm_SystemDebug0(void)
{
#if UART_SEPARATE_BUFFER
   PARSER_STATUS_t B_Status;

   if (isSerialMsgCompleted0() == TRUE)		 		// Check whether packet OK
   {
      gm_StopRcvMsg0();                           // Stop receive anymore data
      B_Status = gm_GprobeCmdParser(SERIAL_USE); // Process command

      if ((B_Status != UNKNOWN_MSG_CMD) || (gm_GetRunModeStatus() == INTERNAL_ROM_MODE))
         gm_StartRcvMsg0();						// Re-start receive

      return B_Status;
   }
   return NO_MSG_PARSED;
#else
   return gm_SystemDebug();
#endif
}
#endif

BYTE near * far gm_GetCommBuf0(void)
{
#if UART_SEPARATE_BUFFER
   return gBa_CommBuf0;
#else
   return gm_GetCommBuf();
#endif

}

void far gm_Uart_SetRcvCnt0 (WORD W_RcvCount)
{
#if UART_SEPARATE_BUFFER
   gW_RcvCount0 = W_RcvCount;
#else

#if (CHIP_ID == STDP93xx)
	#ifdef UART_DEBUG_MODE
		#ifdef USE_SECOND_UART
		((WORD)(gmp_uart[UART1]->rcvCount)) = W_RcvCount;
		#else
		if(UserPrefUartSelect == UART0)
		((WORD)(gmp_uart[UART0]->rcvCount)) = W_RcvCount;
		else
		((WORD)(gmp_uart[UART1]->rcvCount)) = W_RcvCount;
		#endif
	#else
		#if(OSD_UART_DEF == UART0)
		((WORD)(gmp_uart[UART0]->rcvCount)) = W_RcvCount;
		#else
		((WORD)(gmp_uart[UART1]->rcvCount)) = W_RcvCount;
		#endif
	#endif   	
#endif

#endif
}

WORD far gm_Uart_GetRcvCnt0(void)
{
	#if UART_SEPARATE_BUFFER
		return gW_RcvCount0;
	#else
	#ifdef UART_DEBUG_MODE
		#ifdef USE_SECOND_UART
		return gm_Uart_GetRcvCnt(UART1);
		#else
		if(UserPrefUartSelect == UART0)
		return gm_Uart_GetRcvCnt(UART0);
		else
		return gm_Uart_GetRcvCnt(UART1);
		#endif
	#else
	#if(OSD_UART_DEF == UART0)
		return gm_Uart_GetRcvCnt(UART0);
	#else
		return gm_Uart_GetRcvCnt(UART1);
	#endif
	#endif	
	#endif
}

#if (CHIP_ID == STDP93xx)
BYTE * huge gm_GetCommBuf(void)
{
	#ifdef UART_DEBUG_MODE
	#ifdef USE_SECOND_UART
	return (BYTE *)(gmp_uart[UART1]->rcvBuf);	//Hyb130407+
	#else
	if(UserPrefUartSelect == UART0)
		return (BYTE *)(gmp_uart[UART0]->rcvBuf);
	else
		return (BYTE *)(gmp_uart[UART1]->rcvBuf);	//Hyb130407+
	#endif
	#else
	#if(OSD_UART_DEF == UART0)
		return (BYTE *)(gmp_uart[UART0]->rcvBuf);
	#else
		return (BYTE *)(gmp_uart[UART1]->rcvBuf);	//Hyb130407+
	#endif
	#endif
}
#endif

//*********************************  END  **************************************

