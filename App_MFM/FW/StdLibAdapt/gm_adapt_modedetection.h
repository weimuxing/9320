/*
   $Workfile:   gm_adapt_modedetection.h  $
   $Revision: 1.3 $
   $Date: 2011/12/13 10:32:44 $
*/
#ifndef __GM_ADAPT_MODEDETECTION_H__
#define __GM_ADAPT_MODEDETECTION_H__
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
// MODULE:     gm_adapt_modedetection.h
//
// USAGE:      Header file to interface to the Standard library
//
//******************************************************************************



//******************************************************************************
//  D E F I N E S
//******************************************************************************


//******************************************************************************
//  E N U M S
//******************************************************************************


//******************************************************************************
//  P R O T O T Y P E S
//******************************************************************************

#define gm_adapt_TimingIsStable						gm_TimingIsStable
#define gm_adapt_InputTimingTransitionToStable		gm_InputTimingTransitionToStable

#define gm_adapt_InputTimingTransitionFromStable		gm_InputTimingTransitionFromStable
//BOOL           gm_adapt_InputTimingTransitionFromStable(gmt_PHY_CH Channel);

#endif // __GM_ADAPT_MODEDETECTION_H__