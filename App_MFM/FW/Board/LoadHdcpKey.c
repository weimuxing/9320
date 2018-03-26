/*
	$Workfile:   LoadHdcpKey.c  $
	$Revision: 1.4 $
	$Date: 2011/12/15 06:09:12 $
*/
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
// MODULE:      LoadHdcpKey.c
//
// USAGE:       This module contains functions to load an HDCP key and
//              associated data from outside the firmware.
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include <string.h>
#include "System\All.h"
#include "Board\LoadHdcpKey.h"

#if (FEATURE_HDMI==ENABLE)

//******************************************************************************
//  D E B U G
//******************************************************************************

//#define DEBUG_HDCP_KEY_MSG
#ifdef DEBUG_HDCP_KEY_MSG
#define hdcp_msg(x)			gm_Print(x, 0)
#define hdcp_msg_i(x, y)	gm_Print(x, y)
#else
#define hdcp_msg(x)
#define hdcp_msg_i(x, y)
#endif

//#define HDCP_PRINT_KEY_INFO

//******************************************************************************
//  L O C A L   D E F I N I T I O N S
//******************************************************************************

//******************************************************************************
//  E X T E R N A L    F U N C T I O N   P R O T O T Y P E S
//******************************************************************************

//******************************************************************************
//  S T A T I C    F U N C T I O N   P R O T O T Y P E S
//******************************************************************************

//******************************************************************************
//  S T A T I C    V A R I A B L E S
//******************************************************************************

//******************************************************************************
//  C O D E
//******************************************************************************

//******************************************************************************
//
// FUNCTION       : gmt_RET_STAT LoadHdcpKey(BYTE *B_Ksv, BYTE *B_HdcpKey)
//
// USAGE          : Call this function to populate HDCP ksv and key buffers.
//
// INPUT          : B_Ksv - pointer to BYTE buffer for KSV data
//                  B_Key - pointer to BYTE buffer for key data
//
// OUTPUT         : Returns KSV and HDCP key
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************

//******************************************************************************
//
// FUNCTION       : void PrintInfoHdcpData(WORD W_Param)
//
// USAGE          : Call this function to print out information for each field
//                  of the HDCP data that is stored outside firmware.
//
// INPUT          : W_Param - the particular field of the HDCP structure to print
//
// OUTPUT         : HDCP key field information printed to the console
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void PrintInfoHdcpData(WORD W_Param)
{
#ifdef HDCP_PRINT_KEY_INFO
   ST_HDCP_KEY_DATA ST_Data;
   BYTE Ba_RegAddr[1];
   gmt_RET_STAT W_Result;
   WORD i;

   Ba_RegAddr[0] = (BYTE)(HDCP_KEY_START_ADDR);			// sub-address

   W_Result = gm_2WireReadAddrBuffer(HDCP_KEY_DEVICE_ADDR, Ba_RegAddr, 1,
                                     (BYTE *)&ST_Data, sizeof(ST_Data));

   if (W_Result != OK)
   {
      gm_Print("*** Error reading HDCP data! ***", 0);
      return;
   }

   switch (W_Param)
   {
      case HDCP_STRUCTURE_REVISION_FIELD:
         gm_Print("---Structure Revision (2 Bytes)---------", 0);
         gm_Printf("%x %x",
                   ST_Data.B_StructureRevision[0],  ST_Data.B_StructureRevision[1]);
         break;

      case HDCP_CHECKCODE_FIELD:
         gm_Print("---Check Code (1 Byte)---------------", 0);
         gm_Printf("%x", ST_Data.B_CheckCode[0]);
         break;

      case HDCP_CUSTOMER_ID_FIELD:
         gm_Print("---Customer ID (4 Bytes)-------------", 0);
         gm_Printf("%x %x %x %x",
                   ST_Data.B_CustomerID[0], ST_Data.B_CustomerID[1],
                   ST_Data.B_CustomerID[2], ST_Data.B_CustomerID[3]);
         break;

      case HDCP_KSV_FIELD:
         gm_Print("---KSV (5 Bytes)--------------------", 0);
         gm_Printf("%x %x %x %x %x",
                   ST_Data.St_GencryptedHdcpKey.B_Ksv[0], ST_Data.St_GencryptedHdcpKey.B_Ksv[1],
                   ST_Data.St_GencryptedHdcpKey.B_Ksv[2], ST_Data.St_GencryptedHdcpKey.B_Ksv[3],
                   ST_Data.St_GencryptedHdcpKey.B_Ksv[4]);
         break;

      case HDCP_KEY_FIELD:
         gm_Print("---HDCP Key (max 287 Bytes)---------", 0);
         for (i = 0; i < HDCP_KEY_LENGTH; i += 7)
         {
            gm_Printf("%x %x %x %x %x %x %x",
                      ST_Data.St_GencryptedHdcpKey.B_HdcpKey[i], ST_Data.St_GencryptedHdcpKey.B_HdcpKey[i + 1],
                      ST_Data.St_GencryptedHdcpKey.B_HdcpKey[i + 2], ST_Data.St_GencryptedHdcpKey.B_HdcpKey[i + 3],
                      ST_Data.St_GencryptedHdcpKey.B_HdcpKey[i + 4], ST_Data.St_GencryptedHdcpKey.B_HdcpKey[i + 5],
                      ST_Data.St_GencryptedHdcpKey.B_HdcpKey[i + 6]);
         }
         break;

      case HDCP_SERIAL_NUMBER_FIELD:
         gm_Print("---Key Serial Number (8 Bytes)--------", 0);
         gm_Printf("%x %x %x %x %x %x %x %x",
                   ST_Data.B_SerialNumber[0], ST_Data.B_SerialNumber[1],
                   ST_Data.B_SerialNumber[2], ST_Data.B_SerialNumber[3],
                   ST_Data.B_SerialNumber[4], ST_Data.B_SerialNumber[5],
                   ST_Data.B_SerialNumber[6], ST_Data.B_SerialNumber[7]);
         break;

      default:
         break;
   }
#else
   W_Param = W_Param;
#endif //#ifdef HDCP_PRINT_KEY_INFO
}

#endif
/*********************************  END  **************************************/
