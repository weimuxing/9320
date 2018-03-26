/*
    $Workfile:   gm_InputConfig.c  $
    $Revision: 1.6 $
    $Date: 2013/01/08 04:07:21 $
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
// MODULE :gm_InputConfig.c
//
// USAGE  :This module contains Tables and defines used for Configuration of
//         Front-End (AFE+DFE).
//
// NOTE   :This file is generated from the spreadsheet Cortez_InputConfig.xls on,
//         4/19/2004 at 4:45:44 PM
//*******************************************************************************

//******************************************************************************
//  IMPORTANT : P L E A S E   D O   N O T   E D I T  T H I S  F I L E
//******************************************************************************

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************

#include "System\all.h"
#include "StdLibInc\gm_Register.h"

#if defined(EV1_93xx_BOARD)
#include    	"Board\EV1-93xx\NO_TUNER\gm_InputConfig.c"
#include    	"Board\EV1-93xx\NO_TUNER\gm_ExtInputConfig.c"
#elif defined(EV2_93xx_BOARD)
#include    	"Board\EV2-93xx\NO_TUNER\gm_InputConfig.c"
#include    	"Board\EV2-93xx\NO_TUNER\gm_ExtInputConfig.c"
#elif defined(RD1_93xx_BOARD)
#include    	"Board\RD1-93xx\NO_TUNER\gm_InputConfig.c"
#include    	"Board\RD1-93xx\NO_TUNER\gm_ExtInputConfig.c"
#elif defined(ST_4K2K_93xx_BOARD)
#include    	"Board\ST4K2K\NO_TUNER\gm_InputConfig.c"
#include    	"Board\ST4K2K\NO_TUNER\gm_ExtInputConfig.c"
#endif

//
// NOTE : Application specific
// Get current input port processing type “gmt_InputProcessing?for the channel
// (CH_A or CH_B)
//      INTERNAL = internal data path (e.g. internal decoder)
//     EXTERNAL = external data path (e.g. external decoder)
//
//******************************************************************************
//
// FUNCTION       : BYTE GetChannelProcType(gmt_PHY_CH B_Channel)
//
// USAGE          :
//
// INPUT          : B_Channel
//
// OUTPUT         :
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
BYTE GetChannelProcType(gmt_PHY_CH B_Channel)
{
   if (B_Channel == CH_A)
      return(gmvs_InputConfigTblPtr[gmvb_InputConnector[CH_A]].B_InputProcessing);
   else
      return(gmvs_InputConfigTblPtr[gmvb_InputConnector[CH_B]].B_InputProcessing);
}

//
// NOTE : Application specific
// Get current input connector name “gmt_UserDefConnNames?for the channel
// (CH_A or CH_B)
//     RF1, RF2, CVBS1, CVBS2, CVBS3, SVIDEO1, SVIDEO2,
//     COMP1, COMP2, COMP3, VGA1, VGA2, SCART1
//
//******************************************************************************
//
// FUNCTION       : BYTE GetChannelConnName(gmt_PHY_CH B_Channel)
//
// USAGE          :
//
// INPUT          : B_Channel
//
// OUTPUT         :
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
BYTE GetChannelConnName(gmt_PHY_CH B_Channel)
{
   if (B_Channel == CH_A)
      return (gmvs_InputConfigTblPtr[gmvb_InputConnector[CH_A]].B_ConnName);
   else
      return (gmvs_InputConfigTblPtr[gB_CurrentInputConnector[CH_B]].B_ConnName);
}

//
// NOTE : Application specific
// Get current input port connector type “gmt_ConnTypes?for the channel
// (CH_A or CH_B)
//      COMPOSITE, TUNER, SVIDEO, COMPONENT, VGA, SCART, SCARTT2
//
//******************************************************************************
//
// FUNCTION       : BYTE GetChannelConnType(gmt_PHY_CH B_Channel)
//
// USAGE          :
//
// INPUT          : B_Channel
//
// OUTPUT         :
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
BYTE GetChannelConnType(gmt_PHY_CH B_Channel)
{
   if (B_Channel == CH_A)
      return (gmvs_InputConfigTblPtr[gmvb_InputConnector[CH_A]].B_ConnType);
   else
      return (gmvs_InputConfigTblPtr[gB_CurrentInputConnector[CH_B]].B_ConnType);
}

//******************************************************************************
//
// FUNCTION       : BOOL IsInternalHdmiDviPort(gmt_PHY_CH B_Channel)
//
// USAGE          : Determines if the physical channel is mapped to the internal
//               HDMI port
//
// INPUT          : B_Channel - CH_A or CH_B
//
// OUTPUT         : TRUE if channel is mapped to internal HDMI port, else FALSE
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
BOOL IsInternalHdmiDviPort(gmt_PHY_CH B_Channel)
{
   if (((GetChannelConnType(B_Channel) == HDMI) || (GetChannelConnType(B_Channel) == DVI)) && (GetChannelProcType(B_Channel) == INTERNAL))
      return TRUE;
   else
      return FALSE;
}


BOOL IsInternalHdmiPort(gmt_PHY_CH B_Channel)
{
   if ((GetChannelConnType(B_Channel) == HDMI) && (GetChannelProcType(B_Channel) == INTERNAL))
      return TRUE;
   else
      return FALSE;
}


BOOL IsInternalDviPort(gmt_PHY_CH B_Channel)
{
   if ((GetChannelConnType(B_Channel) == DVI) && (GetChannelProcType(B_Channel) == INTERNAL))
      return TRUE;
   else
      return FALSE;
}


//*********************************  END  **************************************
