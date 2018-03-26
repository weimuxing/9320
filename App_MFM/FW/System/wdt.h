/*
	$Workfile:   wdt.h  $
	$Revision: 1.3 $
	$Date: 2011/12/15 06:09:16 $
*/
#ifndef __WDT_H__
#define __WDT_H__
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
// MODULE:		wdt.h
//
// USAGE:		Header file for watch dog timer definition
//
//******************************************************************************

#ifdef WDT_SERVICE_ENABLE

//******************************************************************************
// USAGE : Reset system thru GPIO and external scematics routed to reset
void WdtResetSystem(void);

#endif //WDT_SERVICE_ENABLE

#endif // __WDT_H_
/*********************************  END  **************************************/
