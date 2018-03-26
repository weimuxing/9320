/*
	$Workfile:   gm_GetNextPacket.c  $
	$Revision: 1.3 $
	$Date: 2011/12/13 10:32:44 $
*/
#define __GM_GETNEXTPACKET_C__
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
// MODULE:      gm_GetNextPacket.c
//
// USAGE:       This file contains standard library function for the
//				Cortez VBI slicer
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "System\all.h"

#ifdef EXTERNAL_SLICER

#include "StdLibInc\gm_Global.h"
#include "StdLibInc\gm_HostReg.h"
#include "StdLibInc\gm_Register.h"
#include "StdLibInc\gm_StdLib.h"
#include "StdLibInc\gm_Vbi.h"

#include "StdLibReplace\ExternalSlicer.h"


//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************
static DWORD VBI_BufRdAddr;
static DWORD VBI_BufWrAddr; // last address of VBI write buffer
static BYTE *ptrRead;    // position of next read from buffer

// Transfer buffer. The size of buffer is chosen to be the same as the
// resolution of the VBI_BUF_WR_ADDRS.
// The size must be multiple of (gmc_MemoryBurstWidth/8) (64/128)
static WORD bufCache[64]; // must be word buffer (FIFO required)

//******************************************************************************
//  C O D E
//******************************************************************************

//******************************************************************************
// Kept for legacy reasons only
//******************************************************************************
BOOL gm_GetVbiBufferOverflowStatus(void)
{
   return FALSE;
}

//******************************************************************************
//
// FUNCTION       : void gm_ResetVbiBufferOverflowStatus(void)
//
// USAGE          : Reset the status of the VBI overflow flag to FALSE
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : B_VbiBufferOverflow         (W) VBI overflow flag
//
// USED_REGS      : none
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void gm_ResetVbiBufferOverflowStatus(void)
{
   VBI_BufRdAddr = VBI_BufWrAddr = gm_ReadRegDWord(VBI_BUF_WR_ADDR_hi);
   // force a buffer read by setting the ptrRead to the end of the cache buffer
   ptrRead = (BYTE *)bufCache + sizeof(bufCache);
}

//******************************************************************************
//
// FUNCTION       : BOOL gm_IsVbiBufferAboveThreshold( const WORD W_VbiThreshold )
//
// USAGE          : Determine if the VBI circular buffer is above a specified threshold
//
// INPUT          : W_VbiThreshold - VBI circular buffer threshold
//
// OUTPUT         : Return TRUE if the VBI circular is above threshold, otherwise return FALSE
//
// GLOBALS        : VBI_BufRdAddr             (R) VBI circular buffer read index
//                  gmc_MemoryVbiSize         (R) VBI circular buffer size
//
// USED_REGS      : none
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
extern WORD W_NumOfPackets;
BOOL gm_IsVbiBufferAboveThreshold( const WORD W_VbiThreshold )
{
   if ( W_NumOfPackets > W_VbiThreshold)
   {
      return TRUE;
   }
   return FALSE;
}


//******************************************************************************
//
// FUNCTION       : gmt_RET_PASSFAIL gm_GetNextPacket (BYTE *Bp_Dest, WORD *Wp_Len, WORD maxLen)
//
// USAGE          : The function copies the next available Teletext data packet
//                  from the current position of the VBI circular buffer to the
//                  destination buffer specified by the caller.  The read pointer
//                  is advanced to the end of the packet afterwards.
//
// INPUT          : *Bp_Dest - points to the destination buffer where the data is
//                     to be written to.
//                  *Wp_Len - points to a variable location to which the length
//                     (number of bytes) of the return data packet is
//                     written to.
//                  maxLen - maximum number of bytes to write to destination
//                     buffer.
//
// OUTPUT         : gmd_PASS if a data packet is copied to the destination
//                  successfully
//                  gmd_FAIL if no more data from the VBI buffer
//
// GLOBALS        : VBI_Data_Ready		(R)
//                  VBI_BufRdAddr		(R)
//                  parser				(R/W)
//
// USED_REGS      : none
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
extern WORD W_NumOfPackets;

#define VPX_PACKET_SIZE_IN_BURSTS			128 	// bytes
#define VPX_PACKET_SIZE_IN_BYTES			42*2	// bytes (including Chroma data)
#define VPX_SEARCH_VALID_PACKET				VPX_PACKET_SIZE_IN_BURSTS - VPX_PACKET_SIZE_IN_BYTES

// Notes:	 *** This function is to be integrated into StdLib
gmt_RET_PASSFAIL gm_GetNextPacket (BYTE *Bp_Dest, WORD *Wp_Len, WORD maxLen)
{
   enum
   {
      STATE_VBI_IDLE,                // searching for 1st header byte
      STATE_VBI_HEADER_0,            // 1st header byte found
      STATE_VBI_HEADER_1,            // 2nd header byte found
      STATE_VBI_HEADER_2,            // 3rd header byte found
      STATE_VBI_STATUS,            // status byte processed
      STATE_VBI_LEN,                // length byte processed
      STATE_VBI_DATA,                // saving packet data
   } state = STATE_VBI_IDLE;

   BYTE data, count = 0;
   WORD len, saveLen, len2Copy;
   WORD i;
   BYTE *ptrEnd = (BYTE *)bufCache + sizeof(bufCache);

   maxLen = maxLen;	// for compiler warning

   *Wp_Len = 0;
   while (W_NumOfPackets > 0)
   {
      if (ptrRead >= ptrEnd)
      {
         // update the read pointer to next burst. Wrap around if reach end of buffer
         if (VBI_BufRdAddr >= gm_ReadRegDWord(VBI_BUF_END_hi)+EXT_XFER_BLOCK_ADJUST)
            VBI_BufRdAddr = gm_ReadRegDWord(VBI_BUF_START_hi);
         else
            VBI_BufRdAddr += EXT_NBR_VBI_XFER_BLOCK;

         gm_SdramRead_HA (VBI_BufRdAddr << 6, EXT_SZ_VBI_CACHE_BUF, EXT_SZ_VBI_CACHE_BUF, 1, 0, parser.cache);

         // Fill Block with FFs to remove erroneous readback of old data
         gm_SdramFill(CONSTANT_AUTOFILL_MODE, VBI_BufRdAddr << 6, EXT_SZ_VBI_CACHE_BUF, EXT_SZ_VBI_CACHE_BUF, 1, 0, 0xFFFFFFFF);

         if (W_NumOfPackets != 0)
            W_NumOfPackets--;

         count = 0;
         ptrRead = (BYTE *)bufCache;	// points to start of buffer
      }

      for ( ; ptrRead < ptrEnd; ptrRead++ )
      {
         data = *ptrRead;
         //
         // Optimization, skip cache block if no header found within
         // VPX_SEARCH_VALID_PACKET bytes of search
         //
         if (++count > VPX_SEARCH_VALID_PACKET && state < 4)
         {
            ptrRead = ptrEnd;		// skip this packet
            break;
         }
         //
         // Parse Data state machine
         // NOTE:
         // Data should be in format <00><FF><FF><.......><27><.......>[DATA]
         //
         switch (state)
         {
            case 0:
               if (data == 0x00)
                  state = 1;
               // optimization (no need for else clause below)
               /*
               else
               	state = 0;
               */
               break;

            case 1:
               if (data == 0xFF)
                  state = 2;
               else if (data != 0x00)
                  state = 0;
               // optimization (no need for else clause below)
               /*
               else
               	state = 1;
               */
               break;

            case 2:
               if (data == 0xFF)
               {
                  state = 3;
                  ptrRead = ptrRead+8;
               }
               else if (data == 0)
               {
                  state = 1;
               }
               else
               {
                  state = 0;
               }
               break;

            case 3: 	// check for framing code
               if (data == 0x27)
               {
                  state = 4;
                  ptrRead = ptrRead+1;
                  len = 42;
                  saveLen = len;
               }
               else if (data == 0)
               {
                  state = 1;
               }
               else
               {
                  state = 0;
                  ptrRead = ptrEnd;		// skip this packet
               }
               break;

            case 4: 	// copy the rest of the data as parsed data
               {
                  len2Copy = (WORD)((ptrEnd - ptrRead)/2);
                  if (len2Copy > len)
                     len2Copy = len;		// just copy the desired length

                  for (i= 0; i < len2Copy; i++)
                  {
                     *(Bp_Dest++) = *ptrRead;
                     ptrRead = ptrRead+2; // skip chroma data
                  }
                  len -= len2Copy;

                  if (len == 0)
                  {
                     *Wp_Len = saveLen;	// update the return length
                     ptrRead = ptrEnd;		// skip this packet
                     return gmd_PASS;
                  }
                  break;
               }
         }
      }
   }
   return gmd_FAIL;
}

//******************************************************************************
//
// FUNCTION       : gmt_RET_PASSFAIL gm_GetFirstPacket (BYTE *Bp_Dest, WORD *Wp_Len, WORD maxLen)
//
// USAGE          : The function advances the internal read pointer of the firmware
//                  driver to the first Teletext data packet in the next available
//                  field, then copies the data packet from the VBI circular buffer
//                  to the destination buffer specified by the caller.  The read
//                  pointer is advanced to the end of the packet afterwards.
//
// INPUT          : Bp_Dest - points to the destination buffer where the data is
//                     to be written to
//                  Wp_Len - points to a variable location to which the length
//                     (number of bytes) of the return data packet is
//                     written to
//                  maxLen - maximum number of bytes to write to destination
//                     buffer.
//
// OUTPUT         : gmd_PASS - A first packet is available
//                  gmd_FAIL - Next field of data is not available
//
// GLOBALS        : parser    (R/W)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION : Any unread packets between the current position
//                  of the read pointer to the new
//                  position of the read pointer are lost.
//
//******************************************************************************
gmt_RET_PASSFAIL gm_GetFirstPacket (BYTE *Bp_Dest, WORD *Wp_Len, WORD maxLen)
{
   ptrRead = (BYTE *)bufCache + sizeof(bufCache); // force to read from next block
   return gm_GetNextPacket( Bp_Dest, Wp_Len, maxLen);
}

#endif //EXTERNAL_SLICER
