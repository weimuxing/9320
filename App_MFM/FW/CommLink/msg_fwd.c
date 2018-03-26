//******************************************************************
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
//================================================================
//
// MODULE:      Msg_Fwd.c
//
//******************************************************************

#include <string.h>
#include "System\all.h"
#include "CommLink\d_uart.h"


/* mapping for communication channel */
COM_CHANNEL ComChannel[] =
{
   {SERIAL_USE, 0},		//UART0
   {DDC2BI_USE, 0}		//I2C ring 0
   /* add more as required*/
};

#define NUM_OF_COMCHANNELS  (sizeof(ComChannel) / sizeof(COM_CHANNEL))


/***I2C CODE ****************************************************************/
BYTE * DDC2BI_GetResponse(BYTE B_DevId, BYTE * Bp_ResBuf)
{
   BYTE checksum = 0, recvChecksum = 0;

   if ( gm_2WireStart(B_DevId, gmd_2WIRE_RD) == gmd_PASS)
   {
      BYTE * currentByte = Bp_ResBuf;
      BYTE msgLength = 0;
      int i = 0;

      /* get destination byte */
      gm_2WireGetByte (I2C_RD, currentByte);
      checksum ^= *currentByte;
      currentByte++;

      /*get length byte*/
      gm_2WireGetByte(I2C_RD, currentByte);
      msgLength = DDC2Bi_MSG_LENGTH(*currentByte);
      checksum ^= *currentByte;
      currentByte++;

      /*get remainder of message */
      for (i = 0; i < msgLength; i++, currentByte++)
      {
         gm_2WireGetByte(I2C_RD, currentByte);
         checksum ^= *currentByte;
      }

      /* receive checksum byte */
      gm_2WireGetByte(I2C_RD|I2C_ACK, currentByte);

      recvChecksum = *currentByte;
      currentByte++;

      gm_2WireStop();
   }
   else
   {
      gm_2WireStop();
      return NULL_PTR;
   }

   /*TODO: calculate checksum of received data.  if checksum doesnt match,
   		return NULL_PTR,  if received a valid null msg, return NULL_PTR.
   		remove hardcoded 0x50 value, and figure out why 0x50 didnt get
   		transmitted in the raw data.. use a scope, it should be the first byte received
   */
   if (recvChecksum != (checksum ^= 0x50))
   {
      return NULL_PTR;

   }


   return (Bp_ResBuf + (offsetof(DDC2Bi_Packet, Message) - 1));
}

BYTE CalcDDC2BiPktChecksum(DDC2Bi_Packet *DDC2BiPkt)
{
   if (DDC2BiPkt)
   {
      int i = 0;
      BYTE checksum = 0;
      BYTE *currentByte = (BYTE*)DDC2BiPkt;

      /* calculate checksum up until Message */
      for (i = 0; i < offsetof(DDC2Bi_Packet, Message); i++, currentByte++)
      {
         checksum ^= *currentByte;
      }

      /* Calculate checksum of payload. Payload length is stored in first byte of message */
      for (i = 0, currentByte = DDC2BiPkt->Message; i < DDC2Bi_MSG_LENGTH(*DDC2BiPkt->Message) -1; i++, currentByte++)
      {
         checksum ^= *currentByte;
      }

      return checksum;
   }
   return 0;
}

gmt_RET_PASSFAIL CreateDDC2BiPkt(DDC2Bi_Packet *DDC2BiPkt, BYTE Dest, BYTE * Bp_Buf)
{
   if (!Bp_Buf)
      return gmd_FAIL;

   DDC2BiPkt->Dest = Dest;
   DDC2BiPkt->Source = 0x51; //TODO: change to global define

   /* length is 0x80 | (size of VCP + length of payload message without checksum of payload)*/
   DDC2BiPkt->Length = DDC2Bi_CONTROL_STATUS_FLAG | (sizeof(DDC2Bi_VCP_Prefix) + (*Bp_Buf - 1) );
   DDC2BiPkt->VCP.CmdCode = 0xC2; //TODO: change to global define
   DDC2BiPkt->VCP.CmdPage = 0; //TODO: change to global define for manufacterer id
   DDC2BiPkt->Message = Bp_Buf;
   DDC2BiPkt->Checksum = CalcDDC2BiPktChecksum(DDC2BiPkt);

   return gmd_PASS;
}

#pragma argsused
gmt_RET_PASSFAIL SendSlave2WirePkt(DDC2Bi_Packet *DDC2BiPkt, BYTE WhichI2CPort)
{

   if (DDC2BiPkt)
   {
      int i = 0;

      BYTE *currentByte;

      if (gm_2WireStart(DDC2BiPkt->Dest, gmd_2WIRE_WR) == gmd_FAIL)
      {
//gm_Print("send 0x%x", DDC2BiPkt->Dest);
         goto failed;
      }
      /* send bytes following dest byte*/
      for (i = 0, currentByte = &(DDC2BiPkt->Source); i < offsetof(DDC2Bi_Packet, Message) - 1; i++, currentByte++)
      {
//gm_Print("send 0x%x", *currentByte);
         if (gm_2WirePutByte(currentByte) == gmd_FAIL)
            return gmd_FAIL;
      }

      /* send remaining bytes in DDC2BiPkt->Message as they appear in BYTE order*/
      for (i = 0, currentByte = DDC2BiPkt->Message; i < DDC2Bi_MSG_LENGTH(*DDC2BiPkt->Message) - 1; i++, currentByte++)
      {
//gm_Print("send 0x%x", *currentByte);
         if (gm_2WirePutByte(currentByte) == gmd_FAIL)
            return gmd_FAIL;
      }
//gm_Print("send 0x%x", DDC2BiPkt->Checksum);
      if (gm_2WirePutByte(&DDC2BiPkt->Checksum) == gmd_FAIL)
         return gmd_FAIL;

      /* indicate transfer finished */
      gm_2WireStop();

      return gmd_PASS;

failed:
      gm_2WireStop();
   }

   return gmd_FAIL;
}


void FwdI2CMsg(BYTE far * Bp_Buf, BYTE WhichI2CPort)
{

   /* i2c forward message format

   	1: Length
   	2: CMD_I2CFWD
   	3: Which serial port
   	4: Message payload
   	5: Checksum

   	Strip everything except message payload, and send message payload over the i2c port defined by Which i2c port

   */
   BYTE FwdBuffer[SIO_RxBufLen];
   DDC2Bi_Packet DDC2BiPkt;

   memset(FwdBuffer, 0, sizeof(FwdBuffer));

   /*prepare forward buffer data, length of data to send stored in fp.Message field*/
   memcpy((void*)FwdBuffer, (void*)Bp_Buf, *Bp_Buf);
   CreateDDC2BiPkt(&DDC2BiPkt, I2C_SLAVE_ADDR, FwdBuffer);
   SendSlave2WirePkt(&DDC2BiPkt, WhichI2CPort);

}


BOOL IsNullMsg(BYTE * Bp_Buf)
{
#define DDC2BI_NULL_MSG_LENGTH 0x80
#define DDC2BI_DEVID	0x6E

   if (Bp_Buf == NULL_PTR)
      return TRUE;

   if (Bp_Buf[0] == DDC2BI_DEVID && Bp_Buf[1] == DDC2BI_NULL_MSG_LENGTH)
      return TRUE;

   return FALSE;
}

/*SERIAL CODE ****************************************************************/
#pragma argsused
void SendSerialPkt(BYTE far *Bp_Buf, BYTE WhichSerialPort)
{
   /*TODO: implement sending routine to different serial ports */

   int i = 0;
   BYTE B_Length = (*Bp_Buf) - 1;
   BYTE B_Checksum = 0;

   for (i=0; i < B_Length; i++)						// Process the whole packet
   {
      B_Checksum += *Bp_Buf;					// Update checksum
      gm_UartTrxChar(*Bp_Buf++);				// Send data
   }
   B_Checksum = ~B_Checksum + 1;				// Update checksum
   gm_UartTrxChar(B_Checksum);				// Send checksum byte
}

void FwdSerialMsg(BYTE far *Bp_Buf, BYTE WhichSerialPort)
{
   BYTE FwdBuffer[SIO_RxBufLen];

   memset(FwdBuffer, 0, sizeof(FwdBuffer));

   /*prepare forward buffer data, length of data to send stored in first byte*/
   memcpy((void*)FwdBuffer, (void*)Bp_Buf, *Bp_Buf);
   /* prepare the start of receive of response message before sending request*/
   gm_StartRcvMsg0();

   SendSerialPkt(FwdBuffer, WhichSerialPort);
}

/*****************************************************************************/

BOOL ResponseSerialMsgCompleted(void)
{
   BYTE len, checksum;
   BYTE  * Bp_Buf = gm_GetCommBuf0();
   WORD W_RcvCount = gm_Uart_GetRcvCnt0();
   //
   // Check total packet length received
   // if the driver already receives enough data, process it
   //
   if ((W_RcvCount > 0) && (W_RcvCount >= Bp_Buf[0]))	// Packet received ?
   {												      // Yes
      len = Bp_Buf[0];             // packet length
      //
      //	prepare for the next receive message
      //
      gm_Uart_SetRcvCnt0(0);                       // reset for next packet

      //
      //	verify the message correctness
      //
      for ( checksum=0; len > 0;  )          // Calculate packet checksum
      {
         checksum += Bp_Buf[--len];
      }
      if (checksum == 0)                     // Checksum correct
      {
         gm_StopRcvMsg0();
         return TRUE;                        // Yes
      }
   }
   return FALSE;
}


void SetResponseTimer(BYTE TimeOut10ms)
{
   gm_TimerStart(FWD_MSG_RESPONSE_10MS_TMR, (WORD)TimeOut10ms);
}

void StopResponseTimer(void)
{
   gm_TimerStop(FWD_MSG_RESPONSE_10MS_TMR);
}

BOOL MsgResponseTimedOut()
{
   return (gm_TimerCheck(FWD_MSG_RESPONSE_10MS_TMR) == TIMER_TMO);
}


#pragma argsused
BYTE * GetReply(BYTE * Bp_ResponseBuf, HANDLER_USE_t WhichRequestPort, HANDLER_USE_t WhichReplyPort, BYTE TimeOut10ms, BYTE Delay10ms)
{
   /* when receive a message wrapped as a fwd msg, then reply back to port with reply status */

   PARSER_STATUS_t RecvStatus;
   BYTE * responseBuf = NULL_PTR;
   BYTE * Bp_CommBuf;

   memset(Bp_ResponseBuf, 0, SIO_RxBufLen);
   SetResponseTimer(TimeOut10ms);

   switch (WhichRequestPort)
   {
      case SERIAL_USE:
         Bp_CommBuf = gm_GetCommBuf0();
         while (ResponseSerialMsgCompleted() == FALSE)
         {
            if (MsgResponseTimedOut())
            {
               gm_Print("timeout!!!", 0);
               goto response_timeout;
            }
         }
         memcpy(Bp_ResponseBuf, Bp_CommBuf, *Bp_CommBuf);
         responseBuf = Bp_ResponseBuf;
         break;

      case DDC2BI_USE:
         {
            BYTE NoAttempts = 0;

            do
            {
               if (MsgResponseTimedOut())
               {
                  gm_Print("timeout!!!", 0);
                  goto response_timeout;
               }

               gm_Delay10ms(Delay10ms);
               //TODO: allan - use correct i2c address for querying slave reply
               NoAttempts++;
               responseBuf = DDC2BI_GetResponse(I2C_SLAVE_ADDR, Bp_ResponseBuf);

            } while (IsNullMsg(Bp_ResponseBuf));
         }
         break;

      default:
         return NULL_PTR;
   }
response_timeout:

   StopResponseTimer();

   return responseBuf;
}

void FwdResponse(BYTE far* Bp_Buf, HANDLER_USE_t WhichReplyPort)
{
   FWD_MSG_Header far* fmp = (FWD_MSG_Header *)Bp_Buf;
   int i = 0;

   BYTE WhichComChannel = fmp->WhichComChannel;
   BYTE ReadCount = fmp->ReadCount;
   BYTE Delay = fmp->Delay;
   BYTE Timeout = fmp->Timeout;

   /* expecting a response? If so, how many packets back*/
   for (i = 0; i < ReadCount; i++)
   {
      //TODO: set the timeout here.  reset the timeout if got a valid reply, but need more

      /* wait between successive request for response */

      BYTE ResponseBuf[SIO_RxBufLen];
      BYTE * Bp_Response = GetReply(ResponseBuf, ComChannel[WhichComChannel].HandlerType, WhichReplyPort, Timeout, Delay);
      if (Bp_Response)
      {
         gm_WriteCmdPkt(Bp_Response, WhichReplyPort);
      }
   }
}

BYTE far* FwdMsg(BYTE far* Bp_Buf)
{
   FWD_MSG_Header far* fmp = (FWD_MSG_Header *)Bp_Buf;

   BYTE WhichComChannel = fmp->WhichComChannel;

   /* message payload starts at this offset */
   BYTE far* MsgPayload = Bp_Buf + sizeof(FWD_MSG_Header);

   if (WhichComChannel > NUM_OF_COMCHANNELS - 1)
   {
      gm_Print("Error -- invalid com channel specified in msg", 0);
      return NULL_PTR;
   }

   switch (ComChannel[WhichComChannel].HandlerType)
   {
      case SERIAL_USE:
         FwdSerialMsg(MsgPayload, ComChannel[WhichComChannel].ID);
         break;
      case DDC2BI_USE:
         FwdI2CMsg(MsgPayload, ComChannel[WhichComChannel].ID);
         break;
      default:
         break;
   }

   return MsgPayload;

}


