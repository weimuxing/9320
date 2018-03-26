/*
   $Workfile:   I2cGpioMaster.h  $
   $Revision: 1.2 $
   $Date: 2011/12/14 10:35:32 $
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
//  MODULE: I2cGpioMaster.h
//
//******************************************************************************

#ifndef I2C_GPIO_MASTER_H
#define I2C_GPIO_MASTER_H

//******************************************************************************
//  G L O B A L    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

void I2cMasterGpioInit(WORD W_I2cClock);
BOOL I2cMasterGpioWriteEx(BYTE B_DevAddress, BYTE const *Bp_Data, WORD W_Length, BOOL *Bp_IsNakAsserted, BOOL B_DoNotStop);
BOOL I2cMasterGpioWriteNext(BYTE const *Bp_Data, WORD W_Length, BOOL *Bp_IsNakAsserted, BOOL B_DoNotStop);
BOOL I2cMasterGpioWrite(BYTE B_DevAddress, BYTE const *Bp_Data, WORD W_Length, BOOL * Bp_IsNakAsserted);
BOOL I2cMasterGpioReadEx(BYTE B_DevAddress, BYTE *Bp_Buffer, WORD W_Length, BOOL *Bp_IsNakAsserted, BYTE B_DoNotStop);
BOOL I2cMasterGpioReadNext(BYTE *Bp_Buffer, WORD W_Length, BOOL *Bp_IsNakAsserted, BOOL B_DoNotStop);
BOOL I2cMasterGpioRead(BYTE B_DevAddress, BYTE *Bp_Buffer, WORD W_Length, BOOL *Bp_IsNakAsserted);

#endif // #ifndef I2C_GPIO_MASTER_H
