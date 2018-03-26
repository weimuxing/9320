/*
   $Workfile:   I2cGpioMaster.c  $
   $Revision: 1.5 $
   $Date: 2011/12/14 10:35:32 $
*/
#define __I2C_GPIO_MASTER_C__
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
//  MODULE: I2cGpioMaster.c
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "System\All.h"

#if 0 // (FEATURE_DISPLAY_PORT == ENABLE)
#if DPRX_FEATURE_EDID_MODE == DPRX_EDID_CACHE_EEPROM_IN_RAM

//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************
#define DEBUG_MSG_PRINT 0

#if DEBUG_MSG_PRINT
#define msg(a,b) gm_Print((const char far *)a,b)
#else
#define msg(a,b)
#endif

#define I2cMasterGpioDelay()    gm_Delay1us(B_DelayValue)   // delay on I2C half-tick (5 mks for 100 kHz)
#define I2cMasterGpioHalfDelay()    gm_Delay1us(B_DelayValue/2)

#define I2cMasterGpioSetSda()     gm_SetRegBitsWord(GPOUTPUT5, GPIO93_OUT)
#define I2cMasterGpioSetScl()     gm_SetRegBitsWord(GPOUTPUT5, GPIO94_OUT)

#define I2cMasterGpioClearSda()   gm_ClearRegBitsWord(GPOUTPUT5, GPIO93_OUT)
#define I2cMasterGpioClearScl()   gm_ClearRegBitsWord(GPOUTPUT5, GPIO94_OUT)

#define I2cMasterGpioCheckSda()   (gm_ReadRegWord(GPINPUT5) & GPIO93_IN)
#define I2cMasterGpioCheckScl()   (gm_ReadRegWord(GPINPUT5) & GPIO94_IN)

static BYTE B_DelayValue;
static BOOL Bt_I2cMasterBusy = FALSE;

BOOL I2cMasterGpioWaitForSclHigh(void);
BOOL I2cMasterGpioStart(void);
BOOL I2cMasterGpioStop(void);
BOOL I2cMasterGpioWriteBit(BOOL B_Bit);
BOOL I2cMasterGpioWriteByte(BYTE B_Data, BOOL * Bp_IsNakAsserted);
BOOL I2cMasterGpioGetAck(BOOL * Bp_IsNakAsserted);
BOOL I2cMasterGpioSendAck(BOOL B_Ack);
BOOL I2cMasterGpioReadByte(BYTE * Bp_Data, BOOL B_DoNotNak);

//******************************************************************************
//  C O D E
//******************************************************************************

//******************************************************************************
// FUNCTION:    void I2cMasterGpioInit(WORD W_I2cClock)
// USAGE:       Initialise I2C master driver and hardware.
// INPUT:       None
// OUTPUT:      None
// GLOBALS:     None
// USED_REGS:   MFP_MISC2_ALT_PINS, GPIO_5_DIRCTRL, GPIO_5_OPENDRAIN_EN, GPIO_5_ENABLE
//******************************************************************************
void I2cMasterGpioInit(WORD W_I2cClock)
{
   // Config GPIO pins
   gm_ClearRegBitsByte(MFP_MISC2_ALT_PINS, MFP_EN_DPRX_EDID);
   gm_SetRegBitsWord(GPIO_5_DIRCTRL, (GPIO93_IO | GPIO94_IO));
   gm_SetRegBitsWord(GPIO_5_OPENDRAIN_EN, (GPIO93_OD | GPIO94_OD));
   gm_SetRegBitsWord(GPIO_5_ENABLE, (GPIO93_EN | GPIO94_EN));

   B_DelayValue = 1000 / W_I2cClock / 2;
}

//******************************************************************************
// FUNCTION:    BOOL I2cMasterGpioWaitForSclHigh(void)
// USAGE:       Wait while SCL line in stretch.
// INPUT:       None
// OUTPUT:      TRUE - operation success, FALSE - operation fail/timeout.
// GLOBALS:     None
// USED_REGS:   None
//******************************************************************************
BOOL I2cMasterGpioWaitForSclHigh(void)
{
   BYTE B_Index;

   // wait no more than 5 clock cycles (50 mks for 100 kHz)
   for (B_Index=0; B_Index < 10; B_Index++)
   {
      if (I2cMasterGpioCheckScl())
         return TRUE;
      I2cMasterGpioDelay();
   }
   return FALSE;
}

//******************************************************************************
// FUNCTION:    BOOL I2cMasterGpioStart(void)
// USAGE:       Write start condition to I2C bus.
// INPUT:       None
// OUTPUT:      TRUE - operation success, FALSE - operation fail/timeout.
// GLOBALS:     None
// USED_REGS:   None
//******************************************************************************
BOOL I2cMasterGpioStart(void)
{
   I2cMasterGpioDelay();
   I2cMasterGpioSetSda();
   I2cMasterGpioSetScl();
   if (I2cMasterGpioWaitForSclHigh())
   {
      I2cMasterGpioDelay();
      I2cMasterGpioClearSda();
      I2cMasterGpioDelay();
      I2cMasterGpioClearScl();

      Bt_I2cMasterBusy = TRUE;
      return TRUE;
   }

   return FALSE;
}

//******************************************************************************
// FUNCTION:    BOOL I2cMasterGpioStop(void)
// USAGE:       Write stop condition to I2C bus.
// INPUT:       None
// OUTPUT:      TRUE - operation success, FALSE - operation fail/timeout.
// GLOBALS:     None
// USED_REGS:   None
//******************************************************************************
BOOL I2cMasterGpioStop(void)
{
   I2cMasterGpioClearSda();
   I2cMasterGpioClearScl();
   I2cMasterGpioDelay();
   I2cMasterGpioSetScl();
   if (I2cMasterGpioWaitForSclHigh())
   {
      I2cMasterGpioDelay();
      I2cMasterGpioSetSda();
      I2cMasterGpioDelay();

      Bt_I2cMasterBusy = FALSE;
      return TRUE;
   }

   return FALSE;
}

//******************************************************************************
// FUNCTION:    BOOL I2cMasterGpioWriteBit(BOOL B_Bit)
// USAGE:       Write data bit to I2C bus.
// INPUT:       BOOL B_Bit - bit to write
// OUTPUT:      TRUE - operation success, FALSE - operation fail/timeout.
// GLOBALS:     None
// USED_REGS:   None
//******************************************************************************
// SCL in LOW level before calling this function
BOOL I2cMasterGpioWriteBit(BOOL B_Bit)
{
   I2cMasterGpioHalfDelay();

   // SCL in LOW -> time to change data
   if (B_Bit)
      I2cMasterGpioSetSda();
   else
      I2cMasterGpioClearSda();

   I2cMasterGpioHalfDelay();

   // set SCL to HIGH - note SLAVE what DATA's stable
   I2cMasterGpioSetScl();  // first half-period of clock
   if (!I2cMasterGpioWaitForSclHigh())
   {
      I2cMasterGpioStop();
      return FALSE;
   }
   I2cMasterGpioDelay();

   I2cMasterGpioClearScl(); // second half-period of clock

   return TRUE;
}

//******************************************************************************
// FUNCTION:    BOOL I2cMasterGpioWriteByte(BYTE B_Data, BOOL *Bp_IsNakAsserted)
// USAGE:       Write data byte to I2C bus.
// INPUT:       BYTE B_Data - data byte to write.
//              BOOL *Bp_IsNakAsserted - non null pointer. Value will set to TRUE if
//                    written data byte was not acknowledged.
// OUTPUT:      TRUE - operation success, FALSE - operation fail/timeout.
// GLOBALS:     None
// USED_REGS:   None
//******************************************************************************
BOOL I2cMasterGpioWriteByte(BYTE B_Data, BOOL *Bp_IsNakAsserted)
{
   BYTE B_Index, B_Mask = 0x80;
   BOOL B_DataBit;

   for (B_Index = 0; B_Index < 8; B_Index++)
   {
      if ((B_Data & B_Mask) == B_Mask)
         B_DataBit = TRUE;
      else
         B_DataBit = FALSE;

      if (!I2cMasterGpioWriteBit(B_DataBit))
         return FALSE;

      B_Mask >>= 1;
   }

   if (!I2cMasterGpioGetAck(Bp_IsNakAsserted))
      return FALSE;

   return TRUE;
}

//******************************************************************************
// FUNCTION:    BOOL I2cMasterGpioGetAck(BOOL * Bp_IsNakAsserted)
// USAGE:       Get acknowledge status from I2C bus.
// INPUT:       BYTE B_Data - data byte to write.
//              BOOL *Bp_IsNakAsserted - non null pointer. Value will set to TRUE if
//                    written data byte was not acknowledged.
// OUTPUT:      TRUE - operation success, FALSE - operation fail/timeout.
// GLOBALS:     None
// USED_REGS:   None
//******************************************************************************
BOOL I2cMasterGpioGetAck(BOOL * Bp_IsNakAsserted)
{
   I2cMasterGpioDelay();

   I2cMasterGpioSetSda();
   I2cMasterGpioSetScl();  // release lines
   if (!I2cMasterGpioWaitForSclHigh())
   {
      I2cMasterGpioStop();
      return FALSE;
   }
   I2cMasterGpioDelay();

   *Bp_IsNakAsserted = I2cMasterGpioCheckSda();

   I2cMasterGpioClearScl();

   return TRUE;
}

//******************************************************************************
// FUNCTION:    BOOL I2cMasterGpioSendAck(BOOL B_Ack)
// USAGE:       Write acknowledge status to I2C bus.
// INPUT:       BOOL B_Ack - TRUE if need to send ACK, FALSE - send NACK
// OUTPUT:      TRUE - operation success, FALSE - operation fail/timeout.
// GLOBALS:     None
// USED_REGS:   None
//******************************************************************************
BOOL I2cMasterGpioSendAck(BOOL B_Ack)
{
   I2cMasterGpioHalfDelay();

   if (B_Ack)
      I2cMasterGpioClearSda();    // LOW SDA level - ACK
   else
      I2cMasterGpioSetSda();      // HIGH SDA level - NACK

   I2cMasterGpioHalfDelay();

   I2cMasterGpioSetScl();               //  ---
   if (!I2cMasterGpioWaitForSclHigh())  //__|    - SCL
   {
      I2cMasterGpioStop();
      return FALSE;
   }
   I2cMasterGpioDelay();

   I2cMasterGpioClearScl();

   return TRUE;
}

//******************************************************************************
// FUNCTION:    BOOL I2cMasterGpioReadByte(BYTE * Bp_Data, BOOL B_DoNotNak
// USAGE:       Read data byte from I2C bus and assert specified responce (acknowledgement).
// INPUT:       BYTE * Bp_Data - pointer to memory that accept read data.
//              BOOL B_DoNotNak - TRUE - make ACK, FALSE - make NAK.
// OUTPUT:      TRUE - operation success, FALSE - operation fail/timeout.
// GLOBALS:     None
// USED_REGS:   None
//******************************************************************************
BOOL I2cMasterGpioReadByte(BYTE * Bp_Data, BOOL B_DoNotNak)
{
   BYTE B_Mask = 0x80;

   *Bp_Data = 0;

   I2cMasterGpioSetSda();       // release SDA line

   while (B_Mask)
   {
      I2cMasterGpioDelay();

      I2cMasterGpioSetScl();   // Set SCL
      if (!I2cMasterGpioWaitForSclHigh())
      {
         I2cMasterGpioStop();
         return FALSE;
      }
      I2cMasterGpioDelay();

      if (I2cMasterGpioCheckSda())
         *Bp_Data |= B_Mask;    // Set data bit

      I2cMasterGpioClearScl();       // Clear SCL

      B_Mask >>= 1;           // Shift bit position to right
   }

   if (!I2cMasterGpioSendAck(!B_DoNotNak))
      return FALSE;

   return TRUE;
}

//******************************************************************************
// FUNCTION:    BOOL I2cMasterGpioWriteNext(BYTE const *Bp_Data, WORD W_Length, BOOL *Bp_IsNakAsserted, BOOL B_DoNotStop)
// USAGE:       Continue last opened write transaction. Funtion will fails if transaction was not opened.
// INPUT:       BYTE const *Bp_Data - data to write.
//              WORD W_Length - size of data to write.
//              BOOL *Bp_IsNakAsserted - will be set to TRUE if device ID or any of data byte was not ACK-ed.
//              BOOL B_DoNotStop - TRUE - keep I2C transaction in open state.
// OUTPUT:      TRUE - operation success, FALSE - operation fail/timeout.
// GLOBALS:     None
// USED_REGS:   None
//******************************************************************************
BOOL I2cMasterGpioWriteNext(BYTE const *Bp_Data, WORD W_Length, BOOL *Bp_IsNakAsserted, BOOL B_DoNotStop)
{
   WORD W_Cnt;

   if (Bt_I2cMasterBusy)
   {
      for (W_Cnt = 0; W_Cnt < W_Length; W_Cnt++)
      {
         if (!I2cMasterGpioWriteByte(Bp_Data[W_Cnt], Bp_IsNakAsserted))
            return FALSE;

         if (*Bp_IsNakAsserted)
         {
            if (!I2cMasterGpioStop())
               return FALSE;

            return TRUE;
         }
      }

      if (!B_DoNotStop)
         if (!I2cMasterGpioStop())
            return FALSE;

      return TRUE;
   }
   else
   {
      return FALSE;
   }
}

//******************************************************************************
// FUNCTION:    BOOL I2cMasterGpioWriteEx(BYTE B_DevAddress, BYTE const *Bp_Data, WORD W_Length, BOOL *Bp_IsNakAsserted, BOOL B_DoNotStop)
// USAGE:       Perform or initiate write transaction.
// INPUT:       BYTE B_DevAddress - device ID.
//              BYTE const *Bp_Data - data to write.
//              WORD W_Length - size of data to write.
//              BOOL *Bp_IsNakAsserted - will be set to TRUE if device ID or any of data byte was not ACK-ed.
//              BOOL B_DoNotStop - TRUE - keep I2C transaction in open state.
// OUTPUT:      TRUE - operation success, FALSE - operation fail/timeout.
// GLOBALS:     None
// USED_REGS:   None
//******************************************************************************
BOOL I2cMasterGpioWriteEx(BYTE B_DevAddress, BYTE const *Bp_Data, WORD W_Length, BOOL *Bp_IsNakAsserted, BOOL B_DoNotStop)
{
   if (!I2cMasterGpioStart())
      return FALSE;

   // Write device address
   if (!I2cMasterGpioWriteByte(B_DevAddress & 0xFE, Bp_IsNakAsserted))
      return FALSE;

   if (*Bp_IsNakAsserted)
   {
      if (!I2cMasterGpioStop())
         return FALSE;

      return TRUE;
   }

   if (!I2cMasterGpioWriteNext(Bp_Data, W_Length, Bp_IsNakAsserted, B_DoNotStop))
      return FALSE;

   return TRUE;
}

//******************************************************************************
// FUNCTION:    BOOL I2cMasterGpioWrite(BYTE B_DevAddress, BYTE const *Bp_Data, WORD W_Length, BOOL * Bp_IsNakAsserted)
// USAGE:       Perform write transaction.
// INPUT:       BYTE B_DevAddress - device ID.
//              BYTE const *Bp_Data - data to write.
//              WORD W_Length - size of data to write.
//              BOOL *Bp_IsNakAsserted - will be set to TRUE if device ID or any of data byte was not ACK-ed.
// OUTPUT:      TRUE - operation success, FALSE - operation fail/timeout.
// GLOBALS:     None
// USED_REGS:   None
//******************************************************************************
BOOL I2cMasterGpioWrite(BYTE B_DevAddress, BYTE const *Bp_Data, WORD W_Length, BOOL * Bp_IsNakAsserted)
{
   return I2cMasterGpioWriteEx(B_DevAddress, Bp_Data, W_Length, Bp_IsNakAsserted, FALSE);
}

//******************************************************************************
// FUNCTION:    BOOL gm_I2cMasterReadNext(BYTE *Bp_Buffer, WORD W_Length, BOOL *Bp_IsNakAsserted, BOOL B_DoNotStop)
// USAGE:       Continue last opened read transaction. Funtion will fails if transaction was not opened.
// INPUT:       BYTE *Bp_Buffer - buffer for read data.
//              WORD W_Length - size of data to read.
//              BOOL *Bp_IsNakAsserted - will be set to TRUE if defice ID was NAK-ed.
//              BOOL B_DoNotStop - TRUE: keep transaction opened, otherwise will closed by STOP.
// OUTPUT:      TRUE - operation success, FALSE - operation fail/timeout.
// GLOBALS:     None
// USED_REGS:   I2C_MASTER_STATUS
//******************************************************************************
BOOL I2cMasterGpioReadNext(BYTE *Bp_Buffer, WORD W_Length, BOOL *Bp_IsNakAsserted, BOOL B_DoNotStop)
{
   WORD W_Cnt;

   if (Bt_I2cMasterBusy)
   {
      for (W_Cnt = 0; W_Cnt < W_Length; W_Cnt++)
      {
         if (!I2cMasterGpioReadByte(Bp_Buffer + W_Cnt, ((W_Cnt < W_Length-1) ? FALSE : TRUE)))
            return FALSE;

         if (*Bp_IsNakAsserted)
         {
            if (!I2cMasterGpioStop())
               return FALSE;

            return TRUE;
         }
      }

      if (!B_DoNotStop)
         if (!I2cMasterGpioStop())
            return FALSE;

      return TRUE;
   }
   else
   {
      return FALSE;
   }
}

//******************************************************************************
// FUNCTION:    BOOL I2cMasterGpioReadEx(BYTE B_DevAddress, BYTE *Bp_Buffer, WORD W_Length, BOOL *Bp_IsNakAsserted, BYTE B_DoNotStop)
// USAGE:       Perform or open read transaction.
// INPUT:       BYTE B_DevAddress - device ID.
//              BYTE *Bp_Buffer - buffer for read data.
//              WORD W_Length - size of data to read.
//              BOOL *Bp_IsNakAsserted - will be set to TRUE if defice ID was NAK-ed.
//              BOOL B_DoNotStop - TRUE: keep transaction opened, otherwise will closed by STOP.
// OUTPUT:      TRUE - operation success, FALSE - operation fail/timeout.
// GLOBALS:     None
// USED_REGS:   None
//******************************************************************************
BOOL I2cMasterGpioReadEx(BYTE B_DevAddress, BYTE *Bp_Buffer, WORD W_Length, BOOL *Bp_IsNakAsserted, BYTE B_DoNotStop)
{
   if (!I2cMasterGpioStart())
      return FALSE;

   // Write device address
   if (!I2cMasterGpioWriteByte(B_DevAddress | BIT0, Bp_IsNakAsserted))
      return FALSE;

   if (*Bp_IsNakAsserted)
   {
      if (!I2cMasterGpioStop())
         return FALSE;

      return TRUE;
   }

   if (!I2cMasterGpioReadNext(Bp_Buffer, W_Length, Bp_IsNakAsserted, B_DoNotStop))
      return FALSE;

   return TRUE;
}

//******************************************************************************
// FUNCTION:    BOOL I2cMasterGpioRead(BYTE B_DevAddress, BYTE *Bp_Buffer, WORD W_Length, BOOL *Bp_IsNakAsserted)
// USAGE:       Perform read transaction.
// INPUT:       BYTE B_DevAddress - device ID.
//              BYTE *Bp_Buffer - buffer for read data.
//              WORD W_Length - size of data to read.
//              BOOL *Bp_IsNakAsserted - will be set to TRUE if defice ID was NAK-ed.
// OUTPUT:      TRUE - operation success, FALSE - operation fail/timeout.
// GLOBALS:     None
// USED_REGS:   None
//******************************************************************************
BOOL I2cMasterGpioRead(BYTE B_DevAddress, BYTE *Bp_Buffer, WORD W_Length, BOOL *Bp_IsNakAsserted)
{
   return I2cMasterGpioReadEx(B_DevAddress, Bp_Buffer, W_Length, Bp_IsNakAsserted, FALSE);
}

#endif // #if DPRX_FEATURE_EDID_MODE == DPRX_EDID_CACHE_EEPROM_IN_RAM
#endif


