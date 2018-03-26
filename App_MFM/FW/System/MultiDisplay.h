/*
	$Workfile:   MultiDisplay.h  $
	$Revision: 1.3 $
	$Date: 2011/12/15 03:12:03 $
*/

#ifndef __MULTIDISPLAY_H__
#define __MULTIDISPLAY_H__
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
// MODULE:      MultiDisplay.h
//
// USAGE:       Header file for application Multi-Display feature
//
//******************************************************************************

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************

//******************************************************************************
//  E X T E R N     V A R I A B L E
//******************************************************************************

//******************************************************************************
//  G L O B A L    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

void MultiDisplaySetup(WORD W_OutputLeftBorderWidth, WORD W_OutputRightBorderWidth,
                       WORD W_OutputTopBorderHeight, WORD W_OutputBottomBorderHeight,
                       WORD W_PanelWidth, WORD W_PanelHeight, WORD W_PanelBezelWidth,
                       WORD W_PanelBezelHeight, BYTE B_NumOfColumn, BYTE B_NumOfRow);
void GetMultiDisplayWinAttrib(BYTE B_WinNum,
                              gmt_WindowStruct* pInputWinDim, gmt_WindowStruct* pOutputWinDim);

#endif	//#ifndef __MULTIDISPLAY_H__
//*********************************  END  **************************************