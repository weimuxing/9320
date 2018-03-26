/*
$Workfile:   gm_ExtInputConfig.c  $
$Revision: 1.4 $
$Date: 2011/12/13 10:32:41 $
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
// USAGE  :This module contains Tables and defines used for External Input
//			Configuration
//*******************************************************************************

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************


//Audio Input Configuration Structure
gmt_AudioInputExtMux ROM gmvs_AudioInputExtMux[NUM_MASTER_LIST] =
{
   /*CVBS1*/       {EXT_MUX_NOT_USED},
   /*SVIDEO1*/     {EXT_MUX_NOT_USED},
   /*COMP1*/       {EXT_MUX_NOT_USED},
   /*VGA1*/        {EXT_MUX_IN1},
   /*NOT_USED*/    {EXT_MUX_NOT_USED},
   /*NOT_USED*/    {EXT_MUX_NOT_USED},
   /*HDMI1*/       {EXT_MUX_NOT_USED},
   /*HDMI2*/       {EXT_MUX_NOT_USED},
   /*DP1*/       {EXT_MUX_NOT_USED},
   /*DP2*/    {EXT_MUX_NOT_USED},
   /*NOT_USED*/    {EXT_MUX_NOT_USED},
};//End of gmvs_AudioInputExtMux

//*********************************  END  **************************************
