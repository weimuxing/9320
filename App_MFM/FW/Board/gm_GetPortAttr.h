/*
    $Workfile:   gm_GetPortAttr.h  $
    $Revision: 1.8 $
    $Date: 2012/08/20 03:43:17 $
*/
#ifndef __GM_GETINPUTCONFIG_H__
#define __GM_GETINPUTCONFIG_H__
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
// MODULE :
//
// USAGE  :
//
//*******************************************************************************
gmt_InputProcessing		gm_GetPortProcType(gmt_UserDefConnNames B_InputConnector);
gmt_InputProcessingPort	gm_GetPortProc(gmt_UserDefConnNames B_InputConnector);
gmt_ConnTypes			gm_GetPortConnType(gmt_UserDefConnNames B_InputConnector);
gmt_AipSignalType		gm_GetPortSignalType(gmt_UserDefConnNames B_InputConnector);
gmt_COMBOPHY_AID        gm_GetDpAnaPhyID(gmt_UserDefConnNames B_InputConnector);
gmt_DPRX_DID            gm_GetDpDigitalID(gmt_UserDefConnNames B_InputConnector);


gmt_COMBOPHY_AID gm_GetHdmiDviAnaPhyID(gmt_UserDefConnNames B_InputConnector);
gmt_HDMI_DVI_DID gm_GetHdmiDviDigitalID(gmt_UserDefConnNames B_InputConnector);
//void gm_GetHdmiDviInputConnectorInit(void);
//gmt_UserDefConnNames gm_GetHdmiDviInputConnector(gmt_HDMI_DVI_DID b_DID);
#endif// __GM_GETINPUTCONFIG_H__