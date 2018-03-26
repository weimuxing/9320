/*
	$Workfile:   gm_Register.h  $
	$Revision: 1.5 $
	$Date: 2011/12/14 09:46:50 $
*/

/****************************************************************************/
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
/*==========================================================================*/

#ifndef __REGISTER_H__
#define __REGSISTER_H__
#if (CHIP_ID == STDP93xx)
#include	"StdLibInc\gm_Register_STDP93xx.h"
#else
#error Wrong CHIP_ID Definition.
#endif

#endif
//**************************************************************************

