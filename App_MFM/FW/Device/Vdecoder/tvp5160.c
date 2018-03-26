/*
   $Workfile:   TVP5160.C  $
   $Revision: 1.3 $
   $Date: 2011/12/15 03:52:11 $
*/
#define __TVP5160_C__
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
// MODULE:     tvp5160.c
//
// USAGE:      Contains Functions related to video decoder tvp5160
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#include "System\all.h"

#ifdef TVP5160_DEV_ID


#define DEBUG_VDD 0

#if DEBUG_VDD && DEBUG_MSG
#define msg(a,b)   gm_Print((const char far *)a,b)
#else
#define msg(a,b)
#endif

//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************
#define TVP5160_STD_STATUS_MASK   	 	0x0f
#define TVP5160_VID_FIELD_FRQ_MASK   	0x20

//#define TVP5160_DEV_ID          			0xB8

// Register Address of TVP5160
#define TVP5160_AIN1_ADDR       			0x02

#define TVP5160_BRT_ADDR        			0x09
#define TVP5160_CON_ADDR        			0x0a
#define TVP5160_SAT_ADDR        			0x0b
#define TVP5160_HUE_ADDR        			0x0c

#define TVP5160_DEC_STAT_ADDR			0x3a	//status addr
#define TVP5160_DEC_STAT_ADDR2	    	0x3b
#define TVP5160_STD_STATUS_ADDR     	0x3f  //video standard status addr
#define TVP5160_IP_STAT_ADDR	  		0xdb	//interlace/progressive status addr

#define TVP5160_VBI_LINE_REG_NUM 		23


//******************************************************************************
//   G L O B A L    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
static void __near dev_TVP5160ConfigVideoMode      (BYTE B_Mode);
static BYTE __near dev_TVP5160GetVideoMode         (void);
static void __near dev_TVP5160SetVideoInput         (BYTE B_InType);
static void __near dev_TVP5160SetVideoBrightness   (BYTE B_BrightVal);
static void __near dev_TVP5160SetVideoContrast      (BYTE B_ContrastVal);
static void __near dev_TVP5160SetVideoHue            (BYTE B_HueVal);
static void __near dev_TVP5160SetVideoSaturation   (BYTE B_SaturationVal);
static BYTE __near dev_TVP5160GetVideoStatus         (void);
static BYTE __near dev_TVP5160GetVideoInterlace      (void);
static void __near dev_TVP5160PowerUp               (void);
static void __near dev_TVP5160PowerDown            (void);


//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************

//******************************************************************************
//  S T A T I C    V A R I A B L E S
//******************************************************************************
static BYTE VID_DECODER_ID = TVP5160_DEV_ID; // used when more than one decoder used in a system.
static BYTE TWO_WIRE_PORT;

//******************************************************************************
//  S T A T I C    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

static BYTE __near dev_Read(BYTE B_RegAddr);
static void __near dev_Write(BYTE B_RegAddr, BYTE B_Value);

BYTE ROM TVP5160InitTable[][2] =
{

   {0xE8,0x02},  {0xE9,0x00},  {0xEA,0x80},  {0xE0,0x01},
   {0xE8,0x60},  {0xE9,0x00},  {0xEA,0xB0},  {0xE0,0x01},
   {0xE0,0x00},

   {0xe8,0x01},  {0xe9,0x7c},  {0xea,0x40},  {0xe0,0x07},
   {0x00,0x01},  {0x02,0x00},  {0x04,0x23},  {0x32,0x04},
   {0x33,0x00},  {0x34,0x11},  {0x31,0x05},  {0x35,0xfa},
   {0x36,0xaf},  {0x07,0x84},  {0x50,0xff},  {0x51,0xd6},
   {0x01,0x0F},  {0x08,0x0E},  {0x09,0xC0},  {0x0A,0x80},

   {0x0B,0x80},  {0x0C,0x00},  {0x2E,0x88},  {0x46,0x50},
   {0x47,0x50},  {0x48,0x50},  {0x49,0x50},  {0x4A,0x00},
   {0x4B,0x08},  {0x4C,0x00},  {0x4D,0x08},  {0x4E,0x00},
   {0x4F,0x08},  {0x50,0x00},  {0x51,0x08}
};


//******************************************************************************
//  C O D E
//******************************************************************************

//******************************************************************************
//
// FUNCTION     :   static BYTE dev_Read(BYTE B_RegAddr)
// USAGE        :   Wrapper function for 2-Wire Read for tvp5160
// INPUT        :   the register address that need to be read
// OUTPUT       :   The value read from the register
// GLOBALS      :   None
// USED_REGS    :
//******************************************************************************
static BYTE __near dev_Read(BYTE B_RegAddr)
{
   BYTE B_Value;

   gm_WriteTwoWire(TWO_WIRE_PORT,VID_DECODER_ID, &B_RegAddr, 1, 0);
   gm_ReadTwoWire(TWO_WIRE_PORT,VID_DECODER_ID, &B_Value, 1, 1);

   return B_Value;
}

//******************************************************************************
//
// FUNCTION     :   static void dev_Write(BYTE B_RegAddr, BYTE B_Value)
// USAGE        :   Wrapper function for 2-Wire Write for tvp5160
// INPUT        :   the register address , the value
// OUTPUT       :   None
// GLOBALS      :   None
// USED_REGS    :
//******************************************************************************
static void __near dev_Write(BYTE B_RegAddr, BYTE B_Value)
{
   WORD temp;

   ((BYTE *)&temp)[0] = B_RegAddr;
   ((BYTE *)&temp)[1] = B_Value;
   gm_WriteTwoWire(TWO_WIRE_PORT,VID_DECODER_ID,(BYTE *)&temp, 2, 1);
}

//******************************************************************************
//
// FUNCTION     :   void dev_TVP5160ConfigVideoMode(BYTE B_Mode)
// USAGE        :   This function initializes tvp5160 in specified video mode
// INPUT        :   mode - NTSC/PAL/SECAM
// OUTPUT       :   None
// GLOBALS      :   None
// USED_REGS    :
//******************************************************************************
static void __near dev_TVP5160ConfigVideoMode(BYTE B_Mode)
{
   WORD i,RegSize;

   //turn VBI lines to default

   if (B_Mode == VDD_MODE_CHANGE)
   {
      msg("+++VDD_MODE_CHANGE", 0);
      //determine input type by hw means and do the same as for manually passed signal system
      B_Mode = dev_TVP5160GetVideoMode();

      if (B_Mode == VDD_MODE_NOVIDEO)
      {
         //try to start from NTSC
         B_Mode = VDD_MODE_NTSC;
      }
   }

   switch (B_Mode)
   {
      case VDD_MODE_INIT:
         msg("+++VDD_MODE_INIT",0);
         RegSize = sizeof(TVP5160InitTable) / 2;
         dev_Write(0x34, 0x11);         //enable autoswitch
         for (i=0; i < RegSize; i++)
            dev_Write(TVP5160InitTable[i][0],TVP5160InitTable[i][1]);
         dev_Write(0x30, 0x0F);         //enable component 480i,480p,576i,576p autoswitch
         dev_Write(0x59, 0x04);
         dev_Write(0xd, 0x80);
         break;
      case VDD_MODE_NTSC:
         msg("+++VDD_MODE_NTSC",0);
         dev_Write(VIDEO_STANDARD_ADDR, 0x00);         //enable autoswitch
         break;
      case VDD_MODE_PAL:
         msg("+++VDD_MODE_PAL",0);
         dev_Write(VIDEO_STANDARD_ADDR, 0x00);
         break;
      case VDD_MODE_SECAM:
         msg("+++VDD_MODE_SECAM",0);
         dev_Write(VIDEO_STANDARD_ADDR, 0x00);
         break;
      default:
         break;
   } // end switch - mode
}

//******************************************************************************
//
// FUNCTION     :   BYTE dev_TVP5160GetVideoMode(void)
// USAGE        :   This function returns the current input video mode
// INPUT        :   None
// OUTPUT       :   returns the mode - NO_VIDEO, NTSC, PAL, SECAM
// GLOBALS      :   None
// USED_REGS    :
//******************************************************************************
static BYTE __near dev_TVP5160GetVideoMode(void)
{
   BYTE B_Status;

   B_Status = dev_Read(TVP5160_STD_STATUS_ADDR) & TVP5160_STD_STATUS_MASK;

   switch (B_Status)
   {
      case 0x01:   //(M,J)NTSC
      case 0x05:	//NTSC 4.43
         return VDD_MODE_NTSC;
      case 0x02:   //(B,D,G,H,I,N) PAL
      case 0x03:   //(M) PAL
      case 0x04:   //(Combination-N) PAL
      case 0x07:   //PAL 60
         return VDD_MODE_PAL;
      case 0x06:
         return VDD_MODE_SECAM;

      default:
         return VDD_MODE_NTSC;
   }
}

//******************************************************************************
//
// FUNCTION     :   void dev_TVP5160SetVideoInput(BYTE B_InType)
// USAGE        :   This function sets tvp5160 input video type
// INPUT        :   input video type - COMPOSITE/SVIDEO
// OUTPUT       :   None
// GLOBALS      :   None
// USED_REGS    :
//******************************************************************************
static void __near dev_TVP5160SetVideoInput(BYTE B_InType)
{
   BYTE temp;

   switch (B_InType)
   {
      case VDD_INTYP_COMPOSITE1:
         msg("+++COMPOSITE..",0);
         dev_Write(INPUT_SELECT_ADDR, 0x00);
#if 0
         temp = dev_Read(LUMINANCE_PROCESSING_CONTROL2_ADDR);
         dev_Write(LUMINANCE_PROCESSING_CONTROL2_ADDR, (temp&0x3F) );
#else	//Gnsskam060731	follow EE request 
         dev_Write(LUMINANCE_PROCESSING_CONTROL2_ADDR, 0);
         dev_Write(LUMINANCE_PROCESSING_CONTROL3_ADDR, 0);
#endif

         break;

      case VDD_INTYP_COMPONENT:
         msg("+++COMPONENT..",0);
         dev_Write(INPUT_SELECT_ADDR, 0x95);
#if 0
         temp = dev_Read(LUMINANCE_PROCESSING_CONTROL2_ADDR);
         dev_Write(LUMINANCE_PROCESSING_CONTROL2_ADDR, (temp | 0x80) );
#else	//Gnsskam060731	follow EE request 
         dev_Write(LUMINANCE_PROCESSING_CONTROL2_ADDR, 0);
         dev_Write(LUMINANCE_PROCESSING_CONTROL3_ADDR, 0);
#endif
         break;

      case VDD_INTYP_SVIDEO:
         msg("+++SVIDEO..",0);
         dev_Write(INPUT_SELECT_ADDR, 0x44);
#if 0
         temp = dev_Read(LUMINANCE_PROCESSING_CONTROL2_ADDR);
         dev_Write(LUMINANCE_PROCESSING_CONTROL2_ADDR, (temp | 0x80));
#else	//Gnsskam060731	follow EE request 
         dev_Write(LUMINANCE_PROCESSING_CONTROL2_ADDR, 0);
         dev_Write(LUMINANCE_PROCESSING_CONTROL3_ADDR, 0);
#endif
         break;

      default:
         return;
   }

}

//******************************************************************************
//
// FUNCTION     :   void dev_TVP5160SetVideoBrightness(BYTE B_BrightVal)
// USAGE        :   This function sets the brightness value for tvp5160
// INPUT        :   brightness value
// OUTPUT       :   None
// GLOBALS      :   None
// USED_REGS    :
//******************************************************************************
static void __near dev_TVP5160SetVideoBrightness(BYTE B_BrightVal)
{
   BYTE TempBrightness;
#if 0
   if (UserPrefCurrentInputMain==IN_SV)		TempBrightness	= UserPrefSV_Brightness;
   if (UserPrefCurrentInputMain==IN_AV)		TempBrightness 	= UserPrefAV_Brightness;
   dev_Write(TVP5160_BRT_ADDR, TempBrightness+79); //79~179
   //dev_Write(TVP5160_BRT_ADDR, B_BrightVal);
#endif
}


//******************************************************************************
//
// FUNCTION     :   void dev_TVP5160SetVideoContrast(BYTE B_ContrastVal)
// USAGE        :   This function sets the contrast value for tvp5160
// INPUT        :   contrast value
// OUTPUT       :   None
// GLOBALS      :   None
// USED_REGS    :
//******************************************************************************
static void __near dev_TVP5160SetVideoContrast(BYTE B_ContrastVal)
{
   dev_Write(TVP5160_CON_ADDR,B_ContrastVal);
}

//******************************************************************************
//
// FUNCTION     :   void dev_TVP5160SetVideoHue(BYTE B_HueVal)
// USAGE        :   This function sets the hue value for tvp5160
// INPUT        :   hue value
// OUTPUT       :   None
// GLOBALS      :   None
// USED_REGS    :
//******************************************************************************
static void __near dev_TVP5160SetVideoHue(BYTE B_HueVal)
{
   dev_Write(TVP5160_HUE_ADDR,B_HueVal);
}

//******************************************************************************
//
// FUNCTION     :   void dev_TVP5160SetVideoSaturation(BYTE B_SaturationVal)
// USAGE        :   This function sets the saturation value for tvp5160
// INPUT        :   saturation value
// OUTPUT       :   None
// GLOBALS      :   None
// USED_REGS    :
//******************************************************************************
static void __near dev_TVP5160SetVideoSaturation(BYTE B_SaturationVal)
{
   dev_Write(TVP5160_SAT_ADDR, B_SaturationVal);
}

//******************************************************************************
//
// FUNCTION     :   BYTE dev_TVP5160GetVideoStatus(void)
// USAGE        :   This function returns the status of the incoming video
// INPUT        :   None
// OUTPUT       :   0 - Locked
//                  1 - Not Locked
// GLOBALS      :   None
// USED_REGS    :
//******************************************************************************
static BYTE __near dev_TVP5160GetVideoStatus(void)
{
   BYTE B_Status;
   B_Status = ( (dev_Read(TVP5160_DEC_STAT_ADDR)) & 0x6 );

   if (dev_TVP5160oStatus != B_Status)
   {
      dev_TVP5160StableCounter++;

      if (dev_TVP5160StableCounter >= 10)
      {
         dev_TVP5160StableCounter = 0;
         dev_TVP5160oStatus       = B_Status;

      }
   }
   else
      dev_TVP5160StableCounter = 0;

   if (dev_TVP5160oStatus == 0x06)
      return 0;
   else
      return (BIT1 | BIT0);

}

//******************************************************************************
//
//  FUNCTION     :   void dev_TVP5160PowerUp(void)
//  USAGE        :   This function powers up the tvp5160 modules.
//  INPUT        :   None
//  OUTPUT       :   None
//  GLOBALS      :   None
//  USED_REGS    :   Power Save Control
//
//******************************************************************************
static void __near dev_TVP5160PowerUp(void)
{
#if 0
   VideoDecoderPowerHigh();

   VideoDecoderResetHigh();
   gm_Delay1ms(10);
   VideoDecoderResetLow();
   gm_Delay1ms(50);
   VideoDecoderResetHigh();
   gm_Delay1ms(10);
#endif

   VideoDecoderPowerLow();
}

//******************************************************************************
//
//  FUNCTION     :   void dev_TVP5160PowerDown(void)
//  USAGE        :   This function powers down the tvp5160 modules.
//  INPUT        :   None
//  OUTPUT       :   None
//  GLOBALS      :   None
//  USED_REGS    :   Power Save Control
//
//******************************************************************************
static void __near dev_TVP5160PowerDown(void)
{
//   dev_Write(0X88,0X0B);
}

//******************************************************************************
//
//  FUNCTION     :   BYTE dev_TVP5160GetVideoInterlace(void)
//  USAGE        :   This function returns the current input video interlace status
//  INPUT        :   None
//  OUTPUT       :   0 - Non Interlaced
//                   1 - Interlaced
//  GLOBALS      :   None
//  USED_REGS    :
//******************************************************************************
static BYTE __near dev_TVP5160GetVideoInterlace(void)
{
   BYTE B_Status;

   B_Status = dev_Read(TVP5160_IP_STAT_ADDR);

   if (B_Status & 0x1)
      return D_NONINTERLACED;
   return D_INTERLACED;
}

//******************************************************************************
//
//  FUNCTION     :   vpc323_VideoDecoderEntry
//  USAGE        :   This function is main entry point into the video decoder
//                   driver.
//  INPUT        :   driver function and WORD value.
//  OUTPUT       :   returns WORD value.
//  GLOBALS      :   None
//  USED_REGS    :   None
//
//******************************************************************************
WORD TVP5160_VideoDecoderEntry(BYTE func, void *p1, void *p2)
{
   WORD val = (WORD)p1;

   WORD ret = 0;


   switch (func)
   {
      case gmd_VID_SET_TWOWIRE_PORT   :
         TWO_WIRE_PORT = (BYTE)val;
         break;
      case gmd_VID_SET_INPUT          :
         dev_TVP5160SetVideoInput((BYTE)val);
         break;
      case gmd_VID_SET_OUTPUT_FORMAT  :
         // default is 656 output.
         break;
      case gmd_VID_SET_MODE           :
         dev_TVP5160ConfigVideoMode((BYTE)val);
         break;
      case gmd_VID_SET_MIXER          :
         // No support for mixer.
         break;
      case gmd_VID_SET_BRIGHTNESS     :
         dev_TVP5160SetVideoBrightness((BYTE)val);
         break;
      case gmd_VID_SET_CONTRAST       :
         dev_TVP5160SetVideoContrast((BYTE)val);
         break;
      case gmd_VID_SET_SATURATION     :
         dev_TVP5160SetVideoSaturation((BYTE)val);
         break;
      case gmd_VID_SET_HUE            :
         dev_TVP5160SetVideoHue((BYTE)val);
         break;
      case gmd_VID_SET_SHARPNESS      :
         // No sharpness support
         break;
      case gmd_VID_GET_STATUS         :
         ret = (WORD)dev_TVP5160GetVideoStatus();
         break;
      case gmd_VID_GET_INTERLACE      :
         ret = (WORD)dev_TVP5160GetVideoInterlace();
         break;
      case gmd_VID_GET_LINES_PER_FIELD:
         // No support
         ret = 0;
         break;
      case gmd_VID_GET_SUB_MODE       :
         ret = (WORD)dev_TVP5160GetVideoMode();
         break;
      case gmd_VID_GET_MODE           :
         ret = (WORD)dev_TVP5160GetVideoMode();
         break;
      case gmd_VID_PWR_UP             :
         dev_TVP5160PowerUp();
         break;
      case gmd_VID_PWR_DOWN           :
         dev_TVP5160PowerDown();
         break;
      case gmd_VID_RAW_READ           :
         ret = (WORD)dev_Read(val);
         break;
      case gmd_VID_RAW_WRITE          :
         dev_Write((BYTE)val,(BYTE)p2);
         break;
      default :
         msg("+++tvp5160: No Video Decoder Function!!",0);
         break;
   }
   return ret;
}


void TVP5147_VideoDecoderEntry(BYTE func, BYTE value)
{
   WORD temp = value;

   switch (func)
   {
      case gmd_VID_SET_BRIGHTNESS     :
         value = ((BYTE) (((temp * (MAX_VD_BRI-MIN_VD_BRI))/100)+MIN_VD_BRI));
         msg("+++BRIGHTNESS = %d", value);
         dev_TVP5160SetVideoBrightness(value);
         break;

      case gmd_VID_SET_CONTRAST       :
         value = ((BYTE) (((temp * (MAX_VD_CON-MIN_VD_CON))/100)+MIN_VD_CON));
         msg("+++CONTRAST = %d", value);
         dev_TVP5160SetVideoContrast(value);
         break;

      case gmd_VID_SET_SATURATION     :
         value = ((BYTE) (((temp * (MAX_VD_SAT-MIN_VD_SAT))/100)+MIN_VD_SAT));
         msg("+++SATURATION = %d", value);
         dev_TVP5160SetVideoSaturation(value);
         break;

      case gmd_VID_SET_HUE            :
         if (value<=50)
            value = 50-value;
         else
            value = 306-value;
         msg("+++HUE = %d", value);
         dev_TVP5160SetVideoHue(value);
         break;
      case gmd_VID_SET_SHARPNESS      :
         break;
   }
}

void TVP5147_VideoDecodePowerOn(void)
{
   gm_SetRegBitsByte(GPOUTPUT1, GPIO4_OUT);
}
void TVP5147_VideoDecodePowerOff(void)
{
   gm_ClearRegBitsByte(GPOUTPUT1, GPIO4_OUT);
}
void TVP5147_VideoDecodeInit(void)
{
   /*   //bertonchange 2006/08/14 for sometimes can't search S-Video input
   		BYTE i,RegSize;
   		dev_TVP5160PowerUp();


           msg("+++VDD_MODE_INIT",0);

           RegSize = sizeof(TVP5160InitTable) / 2;
           dev_Write(0x34, 0x11);         					//enable autoswitch
           for(i=0; i < RegSize; i++)
           	dev_Write(TVP5160InitTable[i][0],TVP5160InitTable[i][1]);

           msg("+++VDD_MODE_NTSC",0);
           dev_Write(VIDEO_STANDARD_ADDR, 0x00);         //enable autoswitch

   		dev_TVP5160SetVideoInput(VDD_INTYP_COMPOSITE);
   */
}

#endif

