/*
	$Workfile:   ExternalDeviceConfig.c  $
	$Revision: 1.13 $
	$Date: 2011/12/13 10:32:40 $
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
// MODULE:      ExtDeviceConfig.c
//
// USAGE:       This module contains functions that is used for
//				Configuration of External Devices based in Connector
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "System\all.h"

//******************************************************************************
//  D E F I N E S
//******************************************************************************

//******************************************************************************
//  S T A T I C   V A R I A B L E S
//******************************************************************************

#ifdef ANX9021_DEV_ID
static BOOL Bt_HDMI_Connected = FALSE;
#endif
//******************************************************************************
//  C O D E
//******************************************************************************

//******************************************************************************
//
// FUNCTION       : void ConfigureExtDevicesVideo(gmt_UserDefConnNames B_InputConnector)
//
// USAGE          : Call this function to configure External Devices based on
//                  input Connector
//
// INPUT          : B_InputConnector - Input Connector
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void ConfigureExtDevicesVideo(gmt_UserDefConnNames B_InputConnector)
{
   //
   // Digital Input Processing
   //
   switch (B_InputConnector)
   {
      default:
         break;
   }//End of Switch
}

//******************************************************************************
//
// FUNCTION       : void ExternalDeviceInit (void)
//
// USAGE          : Initializes the external device on board
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  : This function should be called after NVRAM userprefs are available
//
// POST_CONDITION :
//
//******************************************************************************
void ExternalDeviceInit (void)
{
   //
   // Initialize External Decoder
   //
#ifdef EXTERNAL_DECODER

#ifdef VPC3230_DEV_ID

   vdd_ConfigVideoMode(VDD_1,VDD_MODE_INIT);

#if (DEFAULT_DIP_DAUGHTER_BOARD_OUT == VPC3230_656)
   vdd_SetVideoOutputFormat(VDD_1,VDD_OUTTYPE_656);
   gm_Print("656", 0);
#endif
#if (DEFAULT_DIP_DAUGHTER_BOARD_OUT == VPC3230_601)
   gm_Print("601", 0);
   vdd_SetVideoOutputFormat(VDD_1,VDD_OUTTYPE_601);
#endif


#if (DEFAULT_DIP_DAUGHTER_BOARD_IN == VPC3230_COMPONENT_IN)
   gm_Print("component", 0);
   vdd_SetVideoInput(VDD_1,VDD_INTYP_COMPONENT);
#endif
#if (DEFAULT_DIP_DAUGHTER_BOARD_IN == VPC3230_COMPOSITE_IN)
   gm_Print("composite", 0);
   vdd_SetVideoInput(VDD_1,VDD_INTYP_COMPOSITE2);
#endif

   vdd_ConfigVideoMode(VDD_1,VDD_MODE_NTSC);

#endif

	#if 0//Init it ln the last of init function //def TVP5160_DEV_ID
		TVP5160_InitDecoder();
	#endif

	#ifdef TVP5158_DEV_ID
		dev_TVP5158_InitOutput();
	#endif

#ifdef ANX9021_DEV_ID
   if (ANX9021_IsConnected())
   {
      ANX9021_Init();
      Bt_HDMI_Connected = TRUE;
      gm_Printf("HDMI ANX9021 module initialized",0);
   }
   else
   {
      gm_Printf("HDMI ANX9021 module absent",0);
   }
#endif
#endif
}

//******************************************************************************
//
// FUNCTION       : BOOL IsHDMI_Connected(void)
//
// USAGE          : Return if HDMI module is connected
//
// INPUT          : None
//
// OUTPUT         : TRUE or FALSE
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  : None
//
// POST_CONDITION : None
//
//******************************************************************************
BOOL IsHDMI_Connected(void)
{
#ifdef ANX9021_DEV_ID
   return Bt_HDMI_Connected;
#else
   return TRUE;//just to be safe
#endif
}

//******************************************************************************
//
// FUNCTION       : BOOL IsDVI_Connected(void)
//
// USAGE          : Return if DVI module is connected
//
// INPUT          : None
//
// OUTPUT         : TRUE or FALSE
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  : None
//
// POST_CONDITION : None
//
//******************************************************************************
BOOL IsDVI_Connected(void)
{
   return TRUE;//just to be safe
}

