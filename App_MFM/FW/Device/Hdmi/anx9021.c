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
// MODULE:      anx9021.c
//
//******************************************************************************

#define __ANX9021_C__


//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "System\all.h"
#ifdef ANX9021_DEV_ID
//******************************************************************************
//  S T A T I C
//******************************************************************************

static BYTE ANX9021_ByteRead(BYTE Address);
static void ANX9021_ByteWrite(BYTE Address, BYTE Data);
static BYTE ANX9021_AudioByteRead(BYTE Address);
static void ANX9021_AudioByteWrite(BYTE Address, BYTE Data);

//******************************************************************************
//  C O D E
//******************************************************************************

static BYTE ANX9021_ByteRead(BYTE B_Address)
{
   BYTE B_Data;
   BYTE B_Addr = B_Address;

   gm_Write2WireBlock(ANX9021_I2C_PORT, ANX9021_I2C_ADDR, (BYTE*) &B_Addr, 1, FALSE);
   gm_Read2WireBlock(ANX9021_I2C_PORT, ANX9021_I2C_ADDR, (BYTE*) &B_Data, 1, TRUE);
   return B_Data;
}

static void ANX9021_ByteWrite(BYTE B_Address, BYTE B_Data)
{
   BYTE Ba_Buffer[2];

   Ba_Buffer[0] = B_Address;
   Ba_Buffer[1] = B_Data;

   gm_Write2WireBlock(ANX9021_I2C_PORT, ANX9021_I2C_ADDR, Ba_Buffer, 2, TRUE);
}

static BYTE ANX9021_AudioByteRead(BYTE B_Address)
{
   BYTE B_Data;
   BYTE B_Addr = B_Address;

   gm_Write2WireBlock(ANX9021_I2C_PORT, ANX9021_I2C_AUDIO_ADDR, (BYTE*) &B_Addr, 1, FALSE);
   gm_Read2WireBlock(ANX9021_I2C_PORT, ANX9021_I2C_AUDIO_ADDR, (BYTE*) &B_Data, 1, TRUE);
   return B_Data;
}

static void ANX9021_AudioByteWrite(BYTE B_Address, BYTE B_Data)
{
   BYTE Ba_Buffer[2];

   Ba_Buffer[0] = B_Address;
   Ba_Buffer[1] = B_Data;

   gm_Write2WireBlock(ANX9021_I2C_PORT, ANX9021_I2C_AUDIO_ADDR, Ba_Buffer, 2, TRUE);
}

//******************************************************************************
//
// FUNCTION		:	gmt_HDMI_SIG_FORMAT ANX9021_GetSignalFormat(void)
//
// USAGE		:	Return the format of the signal
//
// INPUT		:	None
//
// OUTPUT		:	RGB, YUV422SD, YUV422HD, YUV444SD, YUV444HD
//
// GLOBALS		:	None
//
// USED_REGS	:	None
//******************************************************************************
gmt_HDMI_SIG_FORMAT ANX9021_GetSignalFormat(void)
{
   BYTE B_AVI_Byte1 = ANX9021_AudioByteRead(0x44);
   BYTE B_AVI_Byte2 = ANX9021_AudioByteRead(0x45);
   BYTE B_AVI_Byte5 = ANX9021_AudioByteRead(0x48);
   BYTE B_SCTRL1;
   BOOL Bt_IsYUV422;
   BOOL Bt_IsYUVHD;

   //Read Byte5 to determine pixel repetition clock;
   //  please refer to Table 8-3 in HDMI Ver. 1.1 for details
   B_SCTRL1 = ANX9021_ByteRead(SCTRL1) & ~(BIT5|BIT4);
   switch (B_AVI_Byte5)
   {
      case 0x00://no repetition
         break;

      case 0x01:// 2x clock
         B_SCTRL1 |= BIT4;
         break;

      case 0x03:// 4x clock
      default:
         B_SCTRL1 |= (BIT5|BIT4);
   }
   ANX9021_ByteWrite(SCTRL1, B_SCTRL1);


   //when input is DVI, B_AVI_Bytes1 reads 0x00, so default to RGB
   if ((B_AVI_Byte1 & (BIT5|BIT6)) == 0)// RGB
   {
      return gmd_HDMI_RGB;
   }
   else //YUV
   {
      if ((B_AVI_Byte1 & (BIT5|BIT6)) == BIT5)// 4:2:2 YCbCr
         Bt_IsYUV422 = TRUE;
      else //if (B_AVI_Byte1 & (BIT5|BIT6)) == BIT6)// 4:4:4 YCbCr
         Bt_IsYUV422 = FALSE;

      if ((B_AVI_Byte2 & (BIT6|BIT7)) == BIT7)// YUVHD
         Bt_IsYUVHD = TRUE;
      else
         Bt_IsYUVHD = FALSE;

      if (Bt_IsYUV422 && Bt_IsYUVHD)
         return gmd_HDMI_YUV422_HD;
      else if (Bt_IsYUV422)
         return gmd_HDMI_YUV422_SD;
      else if (Bt_IsYUVHD)
         return gmd_HDMI_YUV444_HD;
      else
         return gmd_HDMI_YUV444_SD;
   }
}



gmt_HDMI_SIG_FORMAT ANX9021_GetBoardOutputFormat(void)
{
   BYTE B_Vid_Aof = ANX9021_ByteRead(VID_AOF);

   if (B_Vid_Aof <= 0x07)
      return gmd_HDMI_RGB;
   else if (B_Vid_Aof <= 0x83)
      return gmd_HDMI_YUV444_HD;
   else if (B_Vid_Aof <= 0x87)
      return gmd_HDMI_YUV444_SD;
   else if (B_Vid_Aof <= 0xC8)
      return gmd_HDMI_YUV422_HD;
   else
      return gmd_HDMI_YUV422_SD;
}

void ANX9021_SetBoardOutputFormat(BYTE B_mode)
{
   ANX9021_ByteWrite(VID_AOF, B_mode);
}

//******************************************************************************
//
// FUNCTION		:	BOOL ANX9021_IsSignalStable(void)
//
// USAGE		:	Return if stable HDMI signal is coming into the device or not
//
// INPUT		:	None
//
// OUTPUT		:	TRUE
//
// GLOBALS		:	None
//
// USED_REGS	:	None
//******************************************************************************
BOOL ANX9021_IsSignalStable(void)
{
   BYTE B_Temp = ANX9021_ByteRead(SSTATUS);

   //if cable plugged and HDMI sync and clock detected?
   return ((B_Temp & (BIT3|BIT1|BIT0)) == (BIT3|BIT1|BIT0));
}

/*void HDMI_Monitor(void)
{
	static BOOL Bt_PreviousState = FALSE; //default to not connected
	static BYTE B_Counter = 0;
	BOOL Bt_CurrentState;

	if (++B_Counter != 0)
		return;//skip 255 check times to reduce the load on I2C bus

	Bt_CurrentState = ANX9021_IsConnected();

	if ((Bt_CurrentState == TRUE) && (Bt_PreviousState == FALSE))
	{
		ANX9021_Init();
		gm_Printf("ANX9021 HDMI module reconnected",0);
	}
	else if ((Bt_CurrentState == FALSE) && (Bt_PreviousState == TRUE))
		gm_Printf("ANX9021 HDMI module unplugged",0);

	Bt_PreviousState = Bt_CurrentState;
}*/

//******************************************************************************
//
// FUNCTION		:	void ANX9021_PowerUp(BOOL Bt_State)
//
// USAGE		:	Power up/down the device
//
// INPUT		:	Bt_State - power up/down the device
//
// OUTPUT		:	None
//
// GLOBALS		:	None
//
// USED_REGS	:	None
//******************************************************************************
void ANX9021_PowerUp(BOOL Bt_State)
{
   BYTE B_Temp = ANX9021_ByteRead(SCTRL1);

   if (Bt_State)
      ANX9021_ByteWrite(SCTRL1, B_Temp | BIT0);
   else
      ANX9021_ByteWrite(SCTRL1, B_Temp & BIT0);
}

//******************************************************************************
//
// FUNCTION		:	void ANX9021_EnablePorts(BOOL Bt_State)
//
// USAGE		:	Enable/disable port_0/port_1
//
// INPUT		:	Bt_State - whether port should be enabled/disabled
//
// OUTPUT		:	None
//
// GLOBALS		:	None
//
// USED_REGS	:	None
//******************************************************************************
void ANX9021_EnablePorts(BYTE B_PortNum, BOOL Bt_State)
{
   BYTE B_Temp = ANX9021_ByteRead(PORTSEL);

   switch (B_PortNum)
   {
      case 0://PORT_0
         if (Bt_State)
            B_Temp |= (BIT4|BIT0);
         else
            B_Temp &= ~(BIT4|BIT0);
         break;

      default://PORT_1
         if (Bt_State)
            B_Temp |= (BIT5|BIT1);
         else
            B_Temp &= ~(BIT5|BIT1);
   }

   ANX9021_ByteWrite(PORTSEL, B_Temp);
}

//******************************************************************************
//
// FUNCTION		:	BOOL ANX9021_IsSignalHDMI(void)
//
// USAGE		:	Return if the incoming signal is HDMI or DVI
//
// INPUT		:	None
//
// OUTPUT		:	TRUE - signal is HDMI
//					FALSE - signal is DVI
//
// GLOBALS		:	None
//
// USED_REGS	:	None
//******************************************************************************
BOOL ANX9021_IsSignalHDMI(void)
{
   if (ANX9021_ByteRead(BSTATUS) & BIT4)
      return TRUE;//incoming signal is HDMI
   else
      return FALSE;//incoming signal is DVI
}

//******************************************************************************
//
// FUNCTION		:	BOOL ANX9021_IsConnected(void)
//
// USAGE		:	Try to read Vender ID and Device ID from the device to make
//						sure it is the correct device and it is properly connected
//
// INPUT		:	None
//
// OUTPUT		:	TRUE - connected and correct device
//					FALSE - not connected or not correct device
//
// GLOBALS		:	None
//
// USED_REGS	:	None
//******************************************************************************
BOOL ANX9021_IsConnected(void)
{
   if ((ANX9021_ByteRead(VID)==0xAA) && (ANX9021_ByteRead(VID+1)==0xAA) &&
         (ANX9021_ByteRead(DID)==0x21) && (ANX9021_ByteRead(DID+1)==0x90))
   {
      return TRUE;
   }
   else
   {
      return FALSE;
   }
}

//******************************************************************************
//
// FUNCTION		:	void ANX9021_Init(void)
//
// USAGE		:	Initialize the device
//
// INPUT		:	None
//
// OUTPUT		:	None
//
// GLOBALS		:	None
//
// USED_REGS	:	None
//******************************************************************************
void ANX9021_Init(void)
{
   ANX9021_ByteWrite(BCAP_SET, BIT7);//enable HDMI capability
   ANX9021_EnablePorts(0, TRUE);
   ANX9021_ByteWrite(SCTRL1, BIT0|BIT2);//power on;12-bit mode to remove noise in dark area
   ANX9021_ByteWrite(0xB5, BIT2);//enable auto video control
#if (DEFAULT_DIP_DAUGHTER_BOARD_OUT == ANX9021_RGB444)
   ANX9021_ByteWrite(0x5F, 0x00);//slect output modes
#elif (DEFAULT_DIP_DAUGHTER_BOARD_OUT == ANX9021_YCbCr444)
   ANX9021_ByteWrite(0x5F, 0x80);//slect output modes
#elif (DEFAULT_DIP_DAUGHTER_BOARD_OUT == ANX9021_YCbCr422)
   ANX9021_ByteWrite(0x5F, 0xC0);//slect output modes
#else
   ANX9021_ByteWrite(0x5F, 0x00);//slect output modes
#endif
   ANX9021_ByteWrite(SWRST, BIT7|BIT6|BIT4);//automatic HDCP, ACR and software reset
   ANX9021_AudioByteWrite(0x26, 0x00);//the default value 0x40 is noisy
   ANX9021_AudioByteWrite(0x27, 0xF9);//enable all I2S channels and MCLKOUT and send PCM data only
   ANX9021_AudioByteWrite(0x29, 0x35);//enable I2S and SPDIF
}

#endif //ANX9021_DEV_ID

