/*
	$Workfile:   LoadHdcpKey.h  $
	$Revision: 1.5 $
	$Date: 2011/12/28 07:19:29 $
*/
#ifndef __LOAD_HDCP_KEY_H__
#define __LOAD_HDCP_KEY_H__
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
// MODULE:      LoadHdcpKey.h
//
// USAGE:       Header file for loading external HDCP keys
//
//******************************************************************************
#if (FEATURE_HDMI==ENABLE)

//******************************************************************************
//  D E F I N E S
//******************************************************************************

#define HDCP_KEY_DEVICE_ADDR					0xAC
#define HDCP_KEY_START_ADDR					0x0

#define HDCP_STRUCTURE_REVISION_LENGTH		2
#define HDCP_CHECK_CODE_LENGTH				1
#define HDCP_CUSTOMER_ID_LENGTH				4
#define HDCP_KSV_LENGTH							5
#define HDCP_KEY_LENGTH							287
#define HDCP_SERIAL_NUMBER_LENGTH			8

typedef struct ST_GENCRYPTED_HDCP_KEY
{
   BYTE B_Ksv[HDCP_KSV_LENGTH];
   BYTE B_HdcpKey[HDCP_KEY_LENGTH];
} ST_GENCRYPTED_HDCP_KEY;

typedef struct
{
   BYTE B_StructureRevision[HDCP_STRUCTURE_REVISION_LENGTH];
   BYTE B_CheckCode[HDCP_CHECK_CODE_LENGTH];
   BYTE B_CustomerID[HDCP_CUSTOMER_ID_LENGTH];
   struct ST_GENCRYPTED_HDCP_KEY St_GencryptedHdcpKey;
   BYTE B_SerialNumber[HDCP_SERIAL_NUMBER_LENGTH];
} ST_HDCP_KEY_DATA;

//******************************************************************************
//  E N U M S
//******************************************************************************

typedef enum
{
   HDCP_STRUCTURE_REVISION_FIELD,
   HDCP_CHECKCODE_FIELD,
   HDCP_CUSTOMER_ID_FIELD,
   HDCP_KSV_FIELD,
   HDCP_KEY_FIELD,
   HDCP_SERIAL_NUMBER_FIELD,
} HDCP_KEY_DATA_FIELDS;

//******************************************************************************
//  P R O T O T Y P E S
//******************************************************************************
void PrintInfoHdcpData(WORD W_Param);

//******************************************************************************
//  U T I L I T Y
//******************************************************************************

//******************************************************************************
//  M A C R O S
//******************************************************************************

#endif // (FEATURE_HDMI==ENABLE)
#endif //__LOAD_HDCP_KEY_H__
/*********************************  END  **************************************/
