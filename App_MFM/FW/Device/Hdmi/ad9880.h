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
// MODULE:      ad9880.h
//
//******************************************************************************

#ifndef __AD9880_H__
#define __AD9880_H__

#ifndef __SYSTEM_H__
#include "System\System.h"
#endif


//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************

#define AD9880_I2C_ADDR		0x98	// The AD9880 device address.  Can only be 0x98 or 0x9A.
#define AD9880_I2C_PORT		0		// The FLI8532 I2C port connected to the AD9880


//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

BYTE AD9880_ByteRead(BYTE Address);
void AD9880_ByteWrite(BYTE Address, BYTE Data);
void AD9880_OutputEnable(void);
void AD9880_OutputDisable(void);
void AD9880_Init(void);


//*********************************  END  **************************************
#endif
