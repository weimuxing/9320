/*
	$Workfile:   CableHandler.h  $
	$Revision: 1.8 $
	$Date: 2012/08/13 09:38:26 $
*/


#ifndef __CABLEHANDLER_H__
#define __CABLEHANDLER_H__
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
// MODULE:      CableHandler.h
//
// USAGE:	Header file for Cable Status
//
//******************************************************************************
//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************

//******************************************************************************
//	D E F I N I T I O N
//******************************************************************************


//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

void CableStatusHandler(void);
//WORD Get_CableStatus(void);
void Get_CableStatus(CableInfo * CableUpdate);
BOOL isCableConnected(gmt_UserDefConnNames B_InputConnector);
BOOL isSourcePowerConnected(gmt_UserDefConnNames B_InputConnector);
void RequestUpdateCableStatus(void);


//*********************************  END  **************************************
#endif

