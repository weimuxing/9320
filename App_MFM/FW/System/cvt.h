#ifndef __CVT_H__
#define __CVT_H_

//*****************************************************************************
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
//=============================================================================
//
// MODULE:      cvt.h
//
// USAGE:       This file contains global definitions and function
//				prototypes for cvt mode detection code.
//
//*****************************************************************************
#ifdef CVT_MODES_SUPPORT

// Use this function to determine if the current mode is cvt

BOOL cvt_IsCVT(gmt_InputMeasurementStruct *Stp_Input, gmt_PHY_CH B_Channel);

// Use this function to calculate the cvt values

void cvt_CalcTiming(gmt_InputMeasurementStruct 	*	Stp_Input,
                    gmt_IbdStruct 				*	Stp_CVT);

// Use this function to determine if the cvt
// pixel clock frequency is in valid range

BOOL IsCVTPixelClockRange(gmt_InputMeasurementStruct *	Stp_CVT,
                          gmt_IbdStruct  			 *  Out_CVT,
                          gmt_PHY_CH					B_Channel);

#endif		// CVT_MODES_SUPPORT

#endif		// __CVT_H_
