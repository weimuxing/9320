//==============================================================================
//
// MODULE:      ina3221.c
//
//******************************************************************************

#define __INA3221_C__


//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "System\all.h"

#ifdef DRIVE_INA3221
//******************************************************************************
//  C O D E
//******************************************************************************

//******************************************************************************
//
// FUNCTION       : BYTE INA3221_ByteRead(BYTE Address)
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
WORD INA3221_ByteRead(BYTE Address)
{
   WORD Data;
   BYTE Addr = Address;

   gm_Write2WireBlock(INA3221_I2C_PORT, INA3221_I2C_ADDR, (BYTE*) &Addr, 1, FALSE);
   gm_Read2WireBlock(INA3221_I2C_PORT, INA3221_I2C_ADDR, (BYTE*) &Data, 2, TRUE);//owen 121031

   return Data;
}


//******************************************************************************
//
// FUNCTION       : void INA3221_ByteWrite(BYTE Address, BYTE Data)
//
// USAGE          :
//
// INPUT          : Address
//                      WData
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
void INA3221_ByteWrite(BYTE Address, WORD WData)
{
   BYTE Buffer[3];

   Buffer[0] = Address;
   Buffer[1] = (WData >> 8) & 0x00ff;
   Buffer[2] = WData & 0x00ff;

   
   gm_Write2WireBlock(INA3221_I2C_PORT, INA3221_I2C_ADDR, (BYTE*)Buffer, 3, TRUE);
}

//******************************************************************************
//
// FUNCTION       : void INA3221_Init(void)
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
void INA3221_Init(void)
{

}


#endif //INA3221

