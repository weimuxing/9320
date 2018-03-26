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
// MODULE:      ad9880.c
//
//******************************************************************************

#define __AD9880_C__


//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "System\all.h"

#ifdef HDMI_AD9880
//******************************************************************************
//  C O D E
//******************************************************************************

//******************************************************************************
//
// FUNCTION       : BYTE AD9880_ByteRead(BYTE Address)
//
// USAGE          :
//
// INPUT          : Address
//
// OUTPUT         : data
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BYTE AD9880_ByteRead(BYTE Address)
{
   BYTE Data;
   BYTE Addr = Address;

   gm_Write2WireBlock(AD9880_I2C_PORT, AD9880_I2C_ADDR, (BYTE*) &Addr, 1, FALSE);
   gm_Read2WireBlock(AD9880_I2C_PORT, AD9880_I2C_ADDR, (BYTE*) &Data, 1, TRUE);

   return Data;
}


//******************************************************************************
//
// FUNCTION       : void AD9880_ByteWrite(BYTE Address, BYTE Data)
//
// USAGE          :
//
// INPUT          : Address
//                  Data
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AD9880_ByteWrite(BYTE Address, BYTE Data)
{
   BYTE Buffer[2];

   Buffer[0] = Address;
   Buffer[1] = Data;
   gm_Write2WireBlock(AD9880_I2C_PORT, AD9880_I2C_ADDR, (BYTE*)Buffer, 2, TRUE);
}


//******************************************************************************
//
// FUNCTION       : void AD9880_OutputEnable(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AD9880_OutputEnable(void)
{
   AD9880_ByteWrite(0x26, 0x00);
}


//******************************************************************************
//
// FUNCTION       : void AD9880_OutputDisable(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AD9880_OutputDisable(void)
{
   AD9880_ByteWrite(0x26, 0x80);
}


//******************************************************************************
//
// FUNCTION       : void AD9880_HDMI_Init(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AD9880_HDMI_Init(void)
{
   AD9880_ByteWrite(0x00, 0x01);
   AD9880_ByteWrite(0x01, 0x69);
   AD9880_ByteWrite(0x02, 0xD0);
   AD9880_ByteWrite(0x03, 0x48);
   AD9880_ByteWrite(0x04, 0x80);
   AD9880_ByteWrite(0x05, 0x80);
   AD9880_ByteWrite(0x06, 0x80);
   AD9880_ByteWrite(0x07, 0x80);
   AD9880_ByteWrite(0x08, 0x00);
   AD9880_ByteWrite(0x09, 0x80);
   AD9880_ByteWrite(0x0A, 0x00);
   AD9880_ByteWrite(0x0B, 0x80);
   AD9880_ByteWrite(0x0C, 0x00);
   AD9880_ByteWrite(0x0D, 0x80);
   AD9880_ByteWrite(0x0E, 0x20);
   AD9880_ByteWrite(0x0F, 0x40);
   AD9880_ByteWrite(0x10, 0x40);
   AD9880_ByteWrite(0x11, 0x03);
   AD9880_ByteWrite(0x12, 0xA9);
   AD9880_ByteWrite(0x13, 0x00);
   AD9880_ByteWrite(0x14, 0x00);
   AD9880_ByteWrite(0x15, 0x00);
   AD9880_ByteWrite(0x16, 0xDB);
   AD9880_ByteWrite(0x17, 0x02);
   AD9880_ByteWrite(0x18, 0x74);
   AD9880_ByteWrite(0x19, 0x08);
   AD9880_ByteWrite(0x1A, 0x14);
   AD9880_ByteWrite(0x1B, 0x06);
   AD9880_ByteWrite(0x1C, 0x4E);
   AD9880_ByteWrite(0x1D, 0x08);
   AD9880_ByteWrite(0x1E, 0x20);
   AD9880_ByteWrite(0x1F, 0x32);
   AD9880_ByteWrite(0x20, 0x1A);
   AD9880_ByteWrite(0x21, 0xCC);
   AD9880_ByteWrite(0x22, 0x18);
   AD9880_ByteWrite(0x23, 0x0C);
   AD9880_ByteWrite(0x24, 0xFE);
   AD9880_ByteWrite(0x25, 0x52);
   AD9880_ByteWrite(0x26, 0x80);
   AD9880_ByteWrite(0x27, 0x80);
   AD9880_ByteWrite(0x28, 0x61);
   AD9880_ByteWrite(0x29, 0x04);
   AD9880_ByteWrite(0x2A, 0x05);
   AD9880_ByteWrite(0x2B, 0x00);
   AD9880_ByteWrite(0x2C, 0x02);
   AD9880_ByteWrite(0x2D, 0xD0);
   AD9880_ByteWrite(0x2E, 0x18);
   AD9880_ByteWrite(0x2F, 0x6F);
   AD9880_ByteWrite(0x30, 0x30);
   AD9880_ByteWrite(0x31, 0x96);
   AD9880_ByteWrite(0x32, 0x0D);
   AD9880_ByteWrite(0x33, 0x95);
   AD9880_ByteWrite(0x34, 0x80);
   AD9880_ByteWrite(0x35, 0x2C);
   AD9880_ByteWrite(0x36, 0x52);
   AD9880_ByteWrite(0x37, 0x08);
   AD9880_ByteWrite(0x38, 0x00);
   AD9880_ByteWrite(0x39, 0x00);
   AD9880_ByteWrite(0x3A, 0x00);
   AD9880_ByteWrite(0x3B, 0x19);
   AD9880_ByteWrite(0x3C, 0xD7);
   AD9880_ByteWrite(0x3D, 0x1C);
   AD9880_ByteWrite(0x3E, 0x54);
   AD9880_ByteWrite(0x3F, 0x08);
   AD9880_ByteWrite(0x40, 0x00);
   AD9880_ByteWrite(0x41, 0x1E);
   AD9880_ByteWrite(0x42, 0x89);
   AD9880_ByteWrite(0x43, 0x02);
   AD9880_ByteWrite(0x44, 0x92);
   AD9880_ByteWrite(0x45, 0x00);
   AD9880_ByteWrite(0x46, 0x00);
   AD9880_ByteWrite(0x47, 0x08);
   AD9880_ByteWrite(0x48, 0x00);
   AD9880_ByteWrite(0x49, 0x0E);
   AD9880_ByteWrite(0x4A, 0x87);
   AD9880_ByteWrite(0x4B, 0x18);
   AD9880_ByteWrite(0x4C, 0xBD);
   AD9880_ByteWrite(0x4D, 0x38);
   AD9880_ByteWrite(0x4E, 0x38);
   AD9880_ByteWrite(0x4F, 0x13);
   AD9880_ByteWrite(0x50, 0x80);
   AD9880_ByteWrite(0x51, 0x40);
   AD9880_ByteWrite(0x52, 0xC1);
   AD9880_ByteWrite(0x53, 0x00);
   AD9880_ByteWrite(0x54, 0x00);
   AD9880_ByteWrite(0x55, 0x00);
   AD9880_ByteWrite(0x56, 0x0F);
   AD9880_ByteWrite(0x57, 0x00);
   AD9880_ByteWrite(0x58, 0x00);
   AD9880_ByteWrite(0x59, 0x00);
   AD9880_ByteWrite(0x5A, 0x00);
   AD9880_ByteWrite(0x5B, 0x07);
   AD9880_ByteWrite(0x5C, 0x01);
   AD9880_ByteWrite(0x5D, 0x01);
   AD9880_ByteWrite(0x9B, 0x00);
   AD9880_ByteWrite(0x9C, 0x00);
   AD9880_ByteWrite(0x9D, 0x00);
   AD9880_ByteWrite(0x9E, 0x00);
   AD9880_ByteWrite(0x9F, 0x00);
}


//******************************************************************************
//
// FUNCTION       : void AD9880_Init(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AD9880_Init(void)
{
   AD9880_OutputDisable();
   AD9880_HDMI_Init();
   AD9880_OutputEnable();
}

#endif //HDMI_AD9880

