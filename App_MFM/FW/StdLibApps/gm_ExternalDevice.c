/*
	$Workfile:   gm_ExternalDevice.c  $
	$Revision: 1.9 $
	$Date: 2012/01/19 08:18:50 $
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
// MODULE:		gm_ExternalDevice.c
//
// USAGE:		This module contains external device mode detection algorithm
//				which expected to be replace in application.
//				The following logic is specific for RD code with one external
//				video decoder.
//
//******************************************************************************

//******************************************************************************
//	I N C L U D E	 F I L E S
//******************************************************************************

#include "System\All.h"


//******************************************************************************
//	C O D E
//******************************************************************************



//******************************************************************************
//
// FUNCTION       : BYTE GetDeviceID(gmt_PHY_CH B_Channel)
//
// USAGE          : Get external device status
//
// INPUT          : B_Channel = CH_A or CH_B
//
// OUTPUT         : External device i2c address
//
// GLOBALS        : None
//
// USED_REGS      :
//
// PRE_CONDITION  : None
//
// POST_CONDITION : None
//
//******************************************************************************
BYTE GetDeviceID(gmt_PHY_CH B_Channel)
{
   if (B_Channel == CH_A)
   {
#ifdef VPC3230_DEV_ID
      return VPC3230_DEV_ID;
#endif
#ifdef SAA7119_DEV_ID
      return SAA7119_DEV_ID;
#endif
#ifdef VPX322xF_DEV_ID
      return VPX322xF_DEV_ID;
#endif
#ifdef TVP5160_DEV_ID
      return TVP5160_DEV_ID;
#endif

#ifdef TVP5151_DEV_ID
      return TVP5151_DEV_ID;
#endif

#ifdef ANX9021_DEV_ID
		 return ANX9021_DEV_ID;
#endif

   }
   return -1;
}

//******************************************************************************
//
// FUNCTION       : gmt_DEVICE_ID gm_GetDeviceId(gmt_PHY_CH B_Channel)
//
// USAGE          : This function is no longer used.
//                  Use gm_SetDeviceId() instead to set the device ID when configuring
//                  the port in SelectInput().
//
// INPUT          : B_Channel
//
// OUTPUT         : 0
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
gmt_DEVICE_ID gm_GetDeviceId(gmt_PHY_CH B_Channel)
{
   // This function is no longer used.
   // Use gm_SetDeviceId() instead to set the device ID when configuring the port in SelectInput().

   B_Channel = B_Channel;
   return 0;
}

//******************************************************************************
//
// FUNCTION       : BYTE gm_GetExternalDeviceStatus (gmt_PHY_CH B_Channel)
//
// USAGE          : Get external device status
//
// INPUT          : B_Channel = CH_A or CH_B
//
// OUTPUT         : FALSE = signal not locked
//                  TRUE  = signal locked
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
#pragma argsused
BYTE gm_GetExternalDeviceStatus (gmt_PHY_CH B_Channel)
{
#ifdef EXTERNAL_DECODER
  // BYTE B_DeviceId = GetDeviceID(B_Channel);

#ifdef ANX9021_DEV_ID
   return ANX9021_IsSignalStable();
#endif

#ifdef VPC3230_DEV_ID
      return ((dev_vpc323GetVideoStatus(VDD_1) == D_LOCKED)
              && (dev_vpc323GetVideoMode(VDD_1) != VDD_MODE_NOVIDEO));
#endif

#ifdef SAA7119_DEV_ID
      return ((dev_saa7119GetVideoStatus(VDD_1) == D_LOCKED)
              && (dev_saa7119GetVideoMode(VDD_1) == VDD_MODE_NOVIDEO));
#endif

#if ((defined( TVP5160_DEV_ID))||(defined( TVP5151_DEV_ID)))
	return IsDecoderInputStable(B_Channel);
      //return ((dev_TVP5160GetVideoStatus() == D_LOCKED)
       //       && (dev_TVP5160GetVideoMode() == VDD_MODE_NOVIDEO));
#else
#pragma message "need to complete!!!!!! Decoder code"

#endif

#ifdef TVP5158_DEV_ID	
	return IsDecoderInputStable(B_Channel);
 //     return ((dev_TVP5158GetVideoStatus(B_Channel) == D_LOCKED)
//             && (dev_TVP5158GetVideoMode() == VDD_MODE_NOVIDEO));
     //  return gmd_TRUE;
#endif
//#pragma message "need to complete!!!!!! Decoder code"

#else
	return (FALSE);
#endif
}
//*********************************  END  **************************************
