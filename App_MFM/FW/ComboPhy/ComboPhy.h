/*
   $Workfile:   ComboPhy.h  $
   $Revision: 1.5 $
   $Date: 2012/08/31 02:49:59 $
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
//******************************************************************************

//==============================================================================
//
// MODULE:		ComboPhy.h
//
//******************************************************************************

#ifndef __COMBOPHY_H__
#define __COMBOPHY_H__
//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************

//******************************************************************************
//  G L O B A L  D E F I N I T I O N S
//******************************************************************************
//#define TOTAL_COMBOPHY_NUM				3
//******************************************************************************
//  G L O B A L  T Y P E S  D E F I N I T I O N S
//******************************************************************************

typedef enum CABLE_STATE
{
   CAB_DISCONNECTED=0,
   CAB_PLUGGING,
   CAB_CONNECTED,
   CAB_UNPLUGGING,
}gmt_CABLE_STATE;

typedef enum SRCPWR_STATE
{
   SRCPWR_OFF=0,
   SRCPWR_LOSTING,
   SRCPWR_DETECTING,
   SRCPWR_ON,
}gmt_SRCPWR_STATE;

//**************************************************************************
//  F U N C T I O N S    P R O T O T Y P E S
//**************************************************************************
void ComboPhyCableStateInit(void);
//BOOL IsComboPhyCableConnected(gmt_COMBOPHY_AID b_PhyNo);
gmt_CABLE_STATE GetComboPhyCableState(gmt_COMBOPHY_AID b_PhyNo);
gmt_CABLE_STATE GetDpRxComboPhyCableState(gmt_COMBOPHY_AID b_PhyNo);
gmt_CABLE_STATE UpdateComboPhyCableState(gmt_COMBOPHY_AID b_PhyNo);

//BOOL IsComboPhySrcPwrDetected(gmt_COMBOPHY_AID b_PhyNo);
gmt_SRCPWR_STATE GetComboPhySrcPwrState(gmt_COMBOPHY_AID b_PhyNo);
gmt_SRCPWR_STATE UpdateComboPhySrcPwrState(gmt_COMBOPHY_AID b_PhyNo);

BOOL IsDpCableConnected(BYTE InputConnector);
BOOL IsDpSrcPwrDetected(BYTE InputConnector);

BOOL IsHdmiDviCableConnected(BYTE InputConnector);
#endif
