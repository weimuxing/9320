/*
	$Workfile:   vpc3230.c  $
	$Revision: 1.5 $
	$Date: 2011/12/15 03:52:11 $
*/

#define __VPC3230_C__

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
// MODULE:	VPC3230.c
//
// USAGE:	This file has functions for the VPC3230 video decoder.
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "System\all.h"
#include "Device\Vdecoder\vpc_regs.h"

#ifdef VPC3230_DEV_ID
//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************

//#define DEBUG_VDD
#ifdef DEBUG_VDD
#define	msg_vdd	gm_Print
#else
#define msg_vdd(a,b)
#endif

//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************

//VIDEO_DECODER_ GVideoDecoder[NUM_DECODER];
VIDEO_DECODER_ GVideoDecoder[1];


//******************************************************************************
//  S T A T I C    V A R I A B L E S
//******************************************************************************

//******************************************************************************
//  S T A T I C    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

static void dev_vpc323FPWrite	(BYTE VidDecId, WORD addr, WORD wrdata);
static WORD dev_vpc323FPRead	(BYTE VidDecId, WORD addr);
static void dev_vpc323_WordWrite	(BYTE VidDecId, BYTE addr, WORD wrdata);
static void dev_vpc323_ByteWrite	(BYTE VidDecId, BYTE addr, BYTE wrdata);
static WORD dev_vpc323_WordRead(BYTE VidDecId, BYTE addr);
static BYTE dev_vpc323_ByteRead(BYTE VidDecId, BYTE addr);

//******************************************************************************
//  C O D E
//******************************************************************************

//******************************************************************************
//
// FUNCTION       : static void dev_vpc323_WordWrite(BYTE VidDecId,BYTE addr,WORD wrdata)
//
// USAGE          : Write 16-bit data to a location on vpc323
//
// INPUT          : VidDecId - video decoder device logical ID (VDD_1, VDD_2)
//                  addr - location on the device to write data to
//                  wrdata - data
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
static void dev_vpc323_WordWrite(BYTE VidDecId,BYTE addr,WORD wrdata)
{
   BYTE  temp[3];

   VidDecId++;						// Dummy
   temp[0] = addr;
   temp[1] = (wrdata >> 8) & 0x00ff;
   temp[2] = wrdata & 0x00ff;

   gm_Write2WireBlock(0, VPC3230_DEV_ID, (BYTE *)temp, 3, TRUE);
}

//******************************************************************************
//
// FUNCTION       : static void dev_vpc323_ByteWrite(BYTE VidDecId,BYTE addr,BYTE wrdata)
//
// USAGE          : Write 8-bit data to a location on vpc323
//
// INPUT          : VidDecId - video decoder device logical ID (VDD_1, VDD_2)
//                  addr - location on the device to write data to
//                  wrdata - data
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
static void dev_vpc323_ByteWrite(BYTE VidDecId,BYTE addr,BYTE wrdata)
{
   BYTE  temp[2];

   VidDecId++;						// Dummy
   temp[0] = addr;
   temp[1] = wrdata;

   gm_Write2WireBlock(0, VPC3230_DEV_ID, (BYTE *)temp, 2, TRUE);
}


//******************************************************************************
//
// FUNCTION       : static WORD dev_vpc323_WordRead(BYTE VidDecId, BYTE addr)
//
// USAGE          : Read 16-bit data from a location on vpc323
//
// INPUT          : VidDecId - video decoder device logical ID (VDD_1, VDD_2)
//                  addr - location on the device to write data to
//
// OUTPUT         : Data read
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
static WORD dev_vpc323_WordRead(BYTE VidDecId, BYTE addr)
{
   BYTE address;
   WORD rdata;
   WORD temp;

   VidDecId++;						// Dummy
   address = addr;

   gm_Write2WireBlock(0, VPC3230_DEV_ID, (BYTE *)&address, 1, FALSE);
   gm_Read2WireBlock(0, VPC3230_DEV_ID, (BYTE *)&temp, 2, TRUE);

   rdata = (temp << 8) & 0xff00;
   rdata = rdata | ((temp >> 8) & 0x00ff);

   return rdata;
}

//******************************************************************************
//
// FUNCTION       : static BYTE dev_vpc323_ByteRead(BYTE VidDecId, BYTE addr)
//
// USAGE          : Read 8-bit data from a location on vpc323
//
// INPUT          : VidDecId - video decoder device logical ID (VDD_1, VDD_2)
//                  addr - location on the device to write data to
//
// OUTPUT         : Data read
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
static BYTE dev_vpc323_ByteRead(BYTE VidDecId, BYTE addr)
{
   BYTE rdata;
   BYTE address;

   VidDecId++;						// Dummy
   address = addr;

   gm_Write2WireBlock(0, VPC3230_DEV_ID, (BYTE *)&address, 1, FALSE);
   gm_Read2WireBlock(0, VPC3230_DEV_ID, (BYTE *)&rdata, 1, TRUE);

   return rdata;
}

//******************************************************************************
//
// FUNCTION       : void dev_vpc323FPWrite(BYTE VidDecId,WORD addr,WORD wrdata)
//
// USAGE          : Write 16-bit data to a location on video decoder device
//
// INPUT          : VidDecId - video decoder device logical ID (VDD_1, VDD_2)
//                  addr - location on the device to write data to
//                  wrdata - data
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void dev_vpc323FPWrite(BYTE VidDecId,WORD addr,WORD wrdata)
{
   dev_vpc323_WordWrite(VidDecId,VPC_FPWRITE_ADDR_TWC_REG,addr);
   dev_vpc323_WordWrite(VidDecId,VPC_FPDATA_TWC_REG,wrdata);
}

//******************************************************************************
//
// FUNCTION       : WORD dev_vpc323FPRead(BYTE VidDecId,WORD addr)
//
// USAGE          : Read 16-bit data from a location on video decoder device
//
// INPUT          : VidDecId - video decoder device logical ID (VDD_1, VDD_2)
//                  addr - location on the device to write data to
//
// OUTPUT         : Data read
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
WORD dev_vpc323FPRead(BYTE VidDecId,WORD addr)
{
   dev_vpc323_WordWrite(VidDecId,VPC_FPREAD_ADDR_TWC_REG,addr);
   return dev_vpc323_WordRead(VidDecId,VPC_FPDATA_TWC_REG);
}


//******************************************************************************
//
// FUNCTION       : void dev_vpc323SetVideoInput(BYTE VidDecId,BYTE InType)
//
// USAGE          : Set video input type for video decoder device
//
// INPUT          : VidDecId - video decoder device logical ID (VDD_1, VDD_2)
//                  InType - video input type: VDD_INTYP_NONE, VDD_INTYP_COMPOSITE,
//                     VDD_INTYP_SVIDEO, VDD_INTYP_COMPOSITE2, VDD_INTYP_TUNER,
//                     VDD_INTYP_COMPONENT
//
// OUTPUT         : None
//
// GLOBALS        : GVideoDecoder	(W)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void dev_vpc323SetVideoInput(BYTE VidDecId,BYTE InType)
{
   WORD temp;

   //Disable ASR Detection
   dev_vpc323FPWrite(VidDecId,VPC_ASR_ENABLE_FP_REG,0x0000);
   GVideoDecoder[VidDecId].InpType = InType;

   switch (InType)
   {
      case VDD_INTYP_COMPOSITE2:
         //gm_Print("CVBS2 ",0);
         temp = dev_vpc323FPRead(VidDecId,VPC_SDT_FP_REG);
         temp &= 0x70f;
         temp |= 0x020;
         //Comb Filter Active Selection
         dev_vpc323FPWrite(VidDecId,VPC_SDT_FP_REG,temp);

         //NOTE: This can change from one card to another
         //luma select from VIN3

         temp = dev_vpc323FPRead(VidDecId,VPC_INSEL_FP_REG);

         temp &= 0x7fc;
         temp |= 0x00;
         temp = (0x0766 & 0x7fc ) | 0x00;
         dev_vpc323FPWrite(VidDecId,VPC_INSEL_FP_REG,temp);

         gm_Delay1ms(20);
         dev_vpc323_ByteWrite(VidDecId,VPC_CIPMIX2_TWC_REG,0xDF);
         dev_vpc323_ByteWrite(VidDecId,VPC_CIPCNTL_TWC_REG,0x40);
         break;


      case VDD_INTYP_SVIDEO:
         msg_vdd("SVIDEO ",0);
         temp = dev_vpc323FPRead(VidDecId,VPC_SDT_FP_REG);
         temp &= 0x70f;
         temp |= 0x040;
         //Comb Filter Active Selection
         msg_vdd("FP @ x20 to be sent =%x", temp);
         //S-VHS input
         dev_vpc323FPWrite(VidDecId,VPC_SDT_FP_REG,temp);
         //temp = dev_vpc323FPRead(VidDecId,VPC_SDT_FP_REG);
         //gm_Print("FP @ x20 read =%x", temp);
         //NOTE: This can change from one card to another
         //luma selct from VIN1 and chroma select from CIN
         temp = dev_vpc323FPRead(VidDecId,VPC_INSEL_FP_REG);
         temp &= 0x7fc;
         temp |= 0x02;
         temp = (0x0766 & 0x7fc ) | 0x02;
         //gm_Print("FP @ x21 to be sent =%x", temp);
         dev_vpc323FPWrite(VidDecId,VPC_INSEL_FP_REG,temp);

         gm_Delay1ms(20);
         //         temp = dev_vpc323FPRead(VidDecId,VPC_INSEL_FP_REG);
         //		 msg_vdd("FP @ x21 read =%x", temp);
         dev_vpc323_ByteWrite(VidDecId,0x95,0xDF);
         dev_vpc323_ByteWrite(VidDecId,0x96,0x40);
         break;

      case VDD_INTYP_COMPOSITE1:
         msg_vdd("CVBS2",0);
         temp = dev_vpc323FPRead(VidDecId,VPC_SDT_FP_REG);
         temp &= 0x70f;
         temp |= 0x020;
         //comb filter active
         dev_vpc323FPWrite(VidDecId,VPC_SDT_FP_REG,temp);

//		temp = dev_vpc323FPRead(VidDecId,0x0021);
//		temp &= 0x7fc;
//		temp |= 0x03;
         temp = (0x0766 & 0x7fc ) | 0x01;
         //NOTE: This can change from one card to another
         //luma select from VIN2
         dev_vpc323FPWrite(VidDecId,VPC_INSEL_FP_REG,temp);

         gm_Delay1ms(20);
         dev_vpc323_ByteWrite(VidDecId,VPC_CIPMIX2_TWC_REG,0xDF);
         dev_vpc323_ByteWrite(VidDecId,VPC_CIPCNTL_TWC_REG,0x40);
         break;

      case VDD_INTYP_COMPONENT:
         msg_vdd("COMPONENT",0);
         temp = dev_vpc323FPRead(VidDecId, VPC_SDT_FP_REG);
         temp &= 0x70f;
         temp |= 0x020;
         //comb filter active
         dev_vpc323FPWrite(VidDecId,VPC_SDT_FP_REG, temp);

         //NOTE: This can change from one card to another
         // luma select from VIN4
         // used as sync source, even if the mixer use only the yuv path

         temp = dev_vpc323FPRead(VidDecId,VPC_INSEL_FP_REG);
         temp &= 0x7fc;
         temp |= 0x03;
//		temp = (0x0766 & 0x7fc ) | 0x00;
         dev_vpc323FPWrite(VidDecId,VPC_INSEL_FP_REG,temp);

         gm_Delay1ms(20);
         // force the mixer for yuv input path
         dev_vpc323_ByteWrite(VidDecId,VPC_CIPMIX2_TWC_REG,0x1F);
         //enable the bypass filter
         dev_vpc323_ByteWrite(VidDecId,VPC_CIPCNTL_TWC_REG,0x84);
         break;

      default:
         break;
   }
   //Enable ASR (automat standard recognisition) Detection for all of them
   dev_vpc323FPWrite(VidDecId,VPC_ASR_ENABLE_FP_REG,0x7f);
}

//******************************************************************************
//
// FUNCTION       : BYTE dev_vpc323GetVideoInput(BYTE VidDecId)
//
// USAGE          : Get video input type of video decoder device
//
// INPUT          : VidDecId - video decoder device logical ID (VDD_1, VDD_2)
//
// OUTPUT         : InType - video input type: VDD_INTYP_NONE,
//                  VDD_INTYP_COMPOSITE, VDD_INTYP_SVIDEO, VDD_INTYP_COMPOSITE2,
//                  VDD_INTYP_TUNER, VDD_INTYP_COMPONENT
//
// GLOBALS        : GVideoDecoder	(R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BYTE dev_vpc323GetVideoInput(BYTE VidDecId)
{
   return GVideoDecoder[VidDecId].InpType;
}

//******************************************************************************
//
// FUNCTION       : void dev_vpc323SetVideoOutputFormat(BYTE VidDecId,BYTE outtype)
//
// USAGE          : Set video output format for video decoder device
//
// INPUT          : VidDecId - video decoder device logical ID (VDD_1, VDD_2)
//                     OutType - output type: VDD_OUTTYPE_NONE, VDD_OUTTYPE_601,
//                     VDD_OUTTYPE_656
//                  outtype
//
// OUTPUT         : None
//
// GLOBALS        : GVideoDecoder	(W)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void dev_vpc323SetVideoOutputFormat(BYTE VidDecId,BYTE outtype)
{
   GVideoDecoder[VidDecId].OutType = outtype;
   switch (outtype)
   {
      case VDD_OUTTYPE_601:
         msg_vdd("ITU601 ",0);
         dev_vpc323_ByteWrite(VidDecId,VPC_OUT_INTERFACE_TWC_REG,0x20);
         break;

      case VDD_OUTTYPE_656:
         msg_vdd("ITU656 ",0);
         dev_vpc323_ByteWrite(VidDecId,VPC_OUT_INTERFACE_TWC_REG,0x38);
         break;

      default:
         break;
   }
}

//******************************************************************************
//
// FUNCTION       : BYTE dev_vpc323GetVideoOutputFormat(BYTE VidDecId)
//
// USAGE          : Get video output format of a video decoder device
//
// INPUT          : VidDecId - video decoder device logical ID (VDD_1, VDD_2)
//
// OUTPUT         : Output type: VDD_OUTTYPE_NONE, VDD_OUTTYPE_601,
//                  VDD_OUTTYPE_656
//
// GLOBALS        : GVideoDecoder	(R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BYTE dev_vpc323GetVideoOutputFormat(BYTE VidDecId)
{
   return GVideoDecoder[VidDecId].OutType;
}

//******************************************************************************
//
// FUNCTION       : BYTE dev_vpc323GetVideoStatus(BYTE VidDecId)
//
// USAGE          : Get status from video decoder device
//
// INPUT          : VidDecId - video decoder device logical ID (VDD_1, VDD_2)
//
// OUTPUT         : Status: D_LOCKED, D_NOTLOCKED
//
// GLOBALS        : GVideoDecoder	(R/W)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BYTE dev_vpc323GetVideoStatus(BYTE VidDecId)
{
   WORD status;
   status = dev_vpc323FPRead(VidDecId,VPC_ASR_FP_REG);
   msg_vdd("Video decoder status = %x",status);
   /*
   // there is not necessary to check for color presence for the composite input !!!
     if (
     	(dev_vpc323GetVideoInput(VidDecId) != VDD_INTYP_TUNER)  &&
     	(dev_vpc323GetVideoInput(VidDecId) != VDD_INTYP_COMPONENT) &&
     	(status & 0x0020)
     	)
     {
       //Color is killed
       GVideoDecoder[VidDecId].Status = D_NOTLOCKED;
     }
     else
   */
   {
      msg_vdd("Here in case of tuner OR status is not 0x20",0);
      // test if is locked on H and V
      if ((status & 0x0003) == 0x0003)
         GVideoDecoder[VidDecId].Status = D_LOCKED;
      else
         GVideoDecoder[VidDecId].Status = D_NOTLOCKED;
   }
   return GVideoDecoder[VidDecId].Status;
}

//******************************************************************************
//
// FUNCTION       : BYTE dev_vpc323GetVideoInterlaceInfo(BYTE VidDecId)
//
// USAGE          : Get video interlaced infor of video decoder device
//
// INPUT          : VidDecId - video decoder device logical ID (VDD_1, VDD_2)
//
// OUTPUT         : D_INTERLACED, D_NONINTERLACED
//
// GLOBALS        : GVideoDecoder	(R/W)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BYTE dev_vpc323GetVideoInterlaceInfo(BYTE VidDecId)
{
   WORD temp;

   temp = dev_vpc323FPRead(VidDecId,VPC_ASR_FP_REG);
   if (temp & 0x0080)
      GVideoDecoder[VidDecId].Interlace = D_INTERLACED;
   else
      GVideoDecoder[VidDecId].Interlace = D_NONINTERLACED;
   return GVideoDecoder[VidDecId].Interlace;
}

//******************************************************************************
//
// FUNCTION       : WORD dev_vpc323GetVideoLinesPerField(BYTE VidDecId)
//
// USAGE          : Get number of lines per field of a specified video decoder device
//
// INPUT          : VidDecId - video decoder device logical ID (VDD_1, VDD_2)
//
// OUTPUT         : # of lines per field
//
// GLOBALS        : GVideoDecoder	(R/W)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
WORD dev_vpc323GetVideoLinesPerField(BYTE VidDecId)
{
   WORD temp;

   temp = dev_vpc323FPRead(VidDecId,VPC_NLPF_FP_REG);
   GVideoDecoder[VidDecId].LPF = temp;
   return GVideoDecoder[VidDecId].LPF;
}

//******************************************************************************
//
// FUNCTION       : BYTE dev_vpc323GetVideoSubMode(BYTE VidDecId)
//
// USAGE          : Get video sub mode from a specified video decoder device
//
// INPUT          : VidDecId - video decoder device logical ID (VDD_1, VDD_2)
//
// OUTPUT         : Submode -  VDD_SUBMODE_PAL_BGHI, VDD_SUBMODE_NTSC_M,
//                  VDD_SUBMODE_SECAM, VDD_SUBMODE_NTSC_44,
//                  VDD_SUBMODE_PAL_M, VDD_SUBMODE_PAL_N
//                  VDD_SUBMODE_PAL_60, VDD_SUBMODE_NTSC_COMB
//
// GLOBALS        : GVideoDecoder	(R/W)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BYTE dev_vpc323GetVideoSubMode(BYTE VidDecId)
{
   WORD submode;

   submode = dev_vpc323FPRead(VidDecId,VPC_SDT_FP_REG);
   submode = submode & 0x0007;
   GVideoDecoder[VidDecId].SubMode = (BYTE)submode;
   return GVideoDecoder[VidDecId].SubMode;
}


//******************************************************************************
//
// FUNCTION       : BYTE dev_vpc323GetVideoMode(BYTE VidDecId)
//
// USAGE          : Get video mode from a specified video decoder device
//
// INPUT          : VidDecId - video decoder device logical ID (VDD_1, VDD_2)
//
// OUTPUT         : Mode -  VDD_MODE_NOVIDEO, VDD_MODE_NTSC, VDD_MODE_PAL
//                  VDD_MODE_SECAM, VDD_MODE_INIT
//
// GLOBALS        : GVideoDecoder	(R/W)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BYTE dev_vpc323GetVideoMode(BYTE VidDecId)
{
   BYTE status,submode;

   submode = dev_vpc323GetVideoSubMode(VidDecId);
   submode = submode & 0x0007;
   status = dev_vpc323GetVideoStatus(VidDecId);

   if (dev_vpc323GetVideoInput(VidDecId) == VDD_INTYP_TUNER)
   {
      GVideoDecoder[VidDecId].Mode = VDD_MODE_NTSC;
      return GVideoDecoder[VidDecId].Mode;
   }

   if (D_NOTLOCKED == status)
      GVideoDecoder[VidDecId].Mode = VDD_MODE_NOVIDEO;
   else
   {
      if ((submode == VDD_SUBMODE_NTSC_M) ||
            (submode == VDD_SUBMODE_NTSC_44)||
            (submode == VDD_SUBMODE_PAL_M)  ||
            (submode == VDD_SUBMODE_PAL_60) ||
            (submode == VDD_SUBMODE_NTSC_COMB))
      {
         GVideoDecoder[VidDecId].Mode = VDD_MODE_NTSC;
      }
      else if ((submode == VDD_SUBMODE_PAL_BGHI) ||
               (submode == VDD_SUBMODE_PAL_N))
      {
         GVideoDecoder[VidDecId].Mode = VDD_MODE_PAL;
      }
      else if (submode == VDD_SUBMODE_SECAM)
         GVideoDecoder[VidDecId].Mode = VDD_MODE_SECAM;
      else
         GVideoDecoder[VidDecId].Mode = VDD_MODE_NOVIDEO;
   }
   return GVideoDecoder[VidDecId].Mode;
}


//******************************************************************************
//
// FUNCTION       : void dev_vpc323ConfigVideoMode(BYTE VidDecId,BYTE mode)
//
// USAGE          : Initializes video decoder device with a specified video mode
//
// INPUT          : VidDecId - video decoder device logical ID (VDD_1, VDD_2)
//                     :   Mode -  VDD_MODE_NOVIDEO, VDD_MODE_NTSC, VDD_MODE_PAL
//                     VDD_MODE_SECAM, VDD_MODE_INIT
//                  mode
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void dev_vpc323ConfigVideoMode(BYTE VidDecId,BYTE mode)
{
   BYTE temp;
   switch (mode)
   {
      case VDD_MODE_INIT:
         dev_vpc323_WordWrite(VidDecId,VPC_PIN_CTRL_TWC_REG,0x0e00);
         //gm_Print("@ x1f = %x",  dev_vpc323_WordRead(VidDecId,VPC_PIN_CTRL_TWC_REG) );
         dev_vpc323_ByteWrite(VidDecId,VPC_SYNCGEN_CTRL_TWC_REG,0x28);
         dev_vpc323_WordWrite(VidDecId,VPC_OP_STENGTH_TWC_REG,0x0000);
         dev_vpc323_WordWrite(VidDecId,VPC_LINE_LEN_TWC_REG,0x0359);
         dev_vpc323_WordWrite(VidDecId,VPC_HC_START_TWC_REG,0x0100);
         dev_vpc323_WordWrite(VidDecId,VPC_HC_STOP_TWC_REG,0x013e);
         dev_vpc323_WordWrite(VidDecId,VPC_AVO_START_TWC_REG,0x0088);
         //dev_vpc323_WordWrite(VidDecId,VPC_AVO_START_TWC_REG,0x0078);	// _FIX_
         dev_vpc323_WordWrite(VidDecId,VPC_AVO_STOP_TWC_REG,0x0348);
         dev_vpc323_WordWrite(VidDecId,VPC_NEWLINE_TWC_REG,0x0088);
         //dev_vpc323_WordWrite(VidDecId,VPC_NEWLINE_TWC_REG,0x0072);	// _FIX_
         dev_vpc323FPWrite(VidDecId,VPC_LLC_CLOCKH_FP_REG,0x002a);
         dev_vpc323FPWrite(VidDecId,VPC_LLC_CLOCKL_FP_REG,0x0aab);
         dev_vpc323FPWrite(VidDecId,VPC_LLC_CLKC_FP_REG,0x0005);
         dev_vpc323FPWrite(VidDecId,VPC_FFLIM_FP_REG,0x02d0);
         dev_vpc323FPWrite(VidDecId,VPC_SCINC1_FP_REG,0x0600);
         //dev_vpc323FPWrite(VidDecId,VPC_SCBRI_FP_REG,0x0132);
         //dev_vpc323FPWrite(VidDecId,VPC_SCCT_FP_REG,0x0030);
         dev_vpc323FPWrite(VidDecId,VPC_SCMODE_FP_REG,0x0000);
         dev_vpc323_ByteWrite(VidDecId,VPC_PIPOPER_TWC_REG,0xff);
         dev_vpc323FPWrite(VidDecId,VPC_SCMODE_FP_REG,0x0000);
         dev_vpc323FPWrite(VidDecId,VPC_LDLY_FP_REG,0x0f40);	// rc15
         //dev_vpc323FPWrite(VidDecId,VPC_LDLY_FP_REG,0x0f80);	// FineTuned Values
         dev_vpc323FPWrite(VidDecId,VPC_COMB_UC_FP_REG,0x00e7);
         dev_vpc323FPWrite(VidDecId,VPC_INSEL_FP_REG,0x0775);		// rc15
         //dev_vpc323FPWrite(VidDecId,VPC_INSEL_FP_REG,0x0766);		// FineTuned Values
         dev_vpc323FPWrite(VidDecId,VPC_SDT_FP_REG,0x0021);
         dev_vpc323FPWrite(VidDecId,VPC_BAMPL_FP_REG,0x0001);
         dev_vpc323FPWrite(VidDecId,0x006e,0x000b);
         dev_vpc323FPWrite(VidDecId,0x00aa,0x001c);
         dev_vpc323FPWrite(VidDecId,0x00ab,0x0298);
         dev_vpc323FPWrite(VidDecId,0x00ac,0x05d1);
         dev_vpc323FPWrite(VidDecId,VPC_ASR_ENABLE_FP_REG,0x7f);

         dev_vpc323_ByteWrite(VidDecId,VPC_CIPMIX1_TWC_REG,0xF0);
//         dev_vpc323_ByteWrite(VidDecId,0x95,0xDF);	// softmixer control
         dev_vpc323_ByteWrite(VidDecId,VPC_CIPMIX2_TWC_REG,0x1F);
         dev_vpc323_WordWrite(VidDecId,VPC_CIPSAT_TWC_REG,(40 <<6) | 40);	// saturation
         dev_vpc323_ByteWrite(VidDecId,VPC_CIPCNTL_TWC_REG,0x40);
// bit7 yuv, not rgb
// bit2 port2
//
//         dev_vpc323_ByteWrite(VidDecId,VPC_CIPCNTL_TWC_REG,0xc4);

         dev_vpc323FPWrite(VidDecId,VPC_SCBRI_FP_REG,0x10);
         dev_vpc323FPWrite(VidDecId,VPC_SCCT_FP_REG,0x2F);
         dev_vpc323FPWrite(VidDecId,VPC_ACC_SAT_FP_REG,0x816);
         dev_vpc323FPWrite(VidDecId,VPC_TINT_FP_REG,0x00);

         temp = dev_vpc323FPRead(VidDecId,VPC_SCMODE_FP_REG);
         temp &= 0x7ff;
         dev_vpc323FPWrite(VidDecId,VPC_SCMODE_FP_REG,temp);

         break;

      case VDD_MODE_NTSC:
         // NO. of pixels/line in NTSC - 1 = 857
         dev_vpc323_WordWrite(VidDecId,VPC_LINE_LEN_TWC_REG,0x0359);
         dev_vpc323_WordWrite(VidDecId,VPC_AVO_STOP_TWC_REG,0x0358);
         dev_vpc323FPWrite(VidDecId,VPC_COMB_UC_FP_REG,0x00e7);
         break;

      case VDD_MODE_PAL:
         //No. pixels per line in PAL - 1 = 863
         dev_vpc323_WordWrite(VidDecId,VPC_LINE_LEN_TWC_REG,0x035f);
         dev_vpc323_WordWrite(VidDecId,VPC_AVO_STOP_TWC_REG,0x0330);
         // Comb Filter Initialisation for PAL
         dev_vpc323FPWrite(VidDecId,VPC_COMB_UC_FP_REG,0x00e7);
         break;

      case VDD_MODE_SECAM:
//         PRINT(("SECAM INIT\n"));
         //No. pixels per line in SECAM - 1 = 863
         dev_vpc323_WordWrite(VidDecId,VPC_LINE_LEN_TWC_REG,0x035f);
         dev_vpc323_WordWrite(VidDecId,VPC_AVO_STOP_TWC_REG,0x0330);
         // Comb Filter Initialisation for secam
         dev_vpc323FPWrite(VidDecId,VPC_COMB_UC_FP_REG,0x00e7);
         break;

      default:
         msg_vdd("DEFAULT",0);
         break;
   } // end switch - mode
}

//******************************************************************************
//
// FUNCTION       : void dev_vpc323VideoMixerControl(BYTE VidDecId,BYTE OnOff)
//
// USAGE          : Turn on/off video mixer control
//
// INPUT          : VidDecId - video decoder device logical ID (VDD_1, VDD_2)
//                  OnOff - D_MIXER_ON, D_MIXER_OFF
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void dev_vpc323VideoMixerControl(BYTE VidDecId,BYTE OnOff)
{
   BYTE temp;

   temp = dev_vpc323_ByteRead(VidDecId,VPC_CIPMIX2_TWC_REG);
   temp = temp & 0x3F;

   if (OnOff == D_MIXER_ON)
   {
      temp = temp | 0x40;
   }
   else
   {
      temp = temp | 0xC0;
   }
   dev_vpc323_ByteWrite(VidDecId,VPC_CIPMIX2_TWC_REG,temp);

}

//******************************************************************************
//
// FUNCTION       : void dev_vpc323SetVideoBrightness(BYTE VidDecId,BYTE bval)
//
// USAGE          : Set the brightness for video decoder device
//
// INPUT          : VidDecId - video decoder device logical ID (VDD_1, VDD_2)
//                  bval - brightness value
//
// OUTPUT         : None
//
// GLOBALS        : GVideoDecoder	(R/W)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void dev_vpc323SetVideoBrightness(BYTE VidDecId,BYTE bval)
{
   WORD temp, temp1;

   GVideoDecoder[VidDecId].Brightness = bval;
   temp1 = (((WORD)bval*0xFF)/100);
   temp1 = temp1 & 0x00FF;
   if (temp1 > 0x7f)
      temp1 = temp1 - 0x80;
   else
      temp1 = temp1 + 0x80;
   temp = dev_vpc323FPRead(VidDecId,VPC_SCBRI_FP_REG);
   temp = temp & 0xFF00;
   temp = temp | temp1;
   dev_vpc323FPWrite(VidDecId,VPC_SCBRI_FP_REG,temp | 0x0100);
   temp = dev_vpc323FPRead(VidDecId,VPC_SCMODE_FP_REG);
   temp &= 0x7ff;
   dev_vpc323FPWrite(VidDecId,VPC_SCMODE_FP_REG,temp);
}

//******************************************************************************
//
// FUNCTION       : BYTE dev_vpc323GetVideoBrightness(BYTE VidDecId)
//
// USAGE          : Get the brightness value from video decoder device
//
// INPUT          : VidDecId - video decoder device logical ID (VDD_1, VDD_2)
//
// OUTPUT         : Brightness value
//
// GLOBALS        : GVideoDecoder	(R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BYTE dev_vpc323GetVideoBrightness(BYTE VidDecId)
{
   return GVideoDecoder[VidDecId].Brightness;
}

//******************************************************************************
//
// FUNCTION       : void dev_vpc323SetVideoContrast(BYTE VidDecId,BYTE cval)
//
// USAGE          : Set the contrast for video decoder device
//
// INPUT          : VidDecId - video decoder device logical ID (VDD_1, VDD_2)
//                  cval - contrast value
//
// OUTPUT         : None
//
// GLOBALS        : GVideoDecoder	(W)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void dev_vpc323SetVideoContrast(BYTE VidDecId,BYTE cval)
{
   WORD temp, temp1;

   GVideoDecoder[VidDecId].Contrast = cval;
   temp1 = (((WORD)cval*0x3F)/100);
   temp1 = temp1 & 0x003F;
   temp = dev_vpc323FPRead(VidDecId,VPC_SCCT_FP_REG);
   temp = temp & 0xFFC0;
   temp = temp | temp1;
   dev_vpc323FPWrite(VidDecId,VPC_SCCT_FP_REG,temp);
   temp = dev_vpc323FPRead(VidDecId,VPC_SCMODE_FP_REG);
   temp &= 0x7ff;
   dev_vpc323FPWrite(VidDecId,VPC_SCMODE_FP_REG,temp);
}

//******************************************************************************
//
// FUNCTION       : BYTE dev_vpc323GetVideoContrast(BYTE VidDecId)
//
// USAGE          : Get the constast value from video decoder device
//
// INPUT          : VidDecId - video decoder device logical ID (VDD_1, VDD_2)
//
// OUTPUT         : Contrast value
//
// GLOBALS        : GVideoDecoder	(R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BYTE dev_vpc323GetVideoContrast(BYTE VidDecId)
{
   return GVideoDecoder[VidDecId].Contrast;
}

//******************************************************************************
//
// FUNCTION       : void dev_vpc323SetVideoSaturation(BYTE VidDecId,BYTE sval)
//
// USAGE          : Set the saturation for video decoder device
//
// INPUT          : VidDecId - video decoder device logical ID (VDD_1, VDD_2)
//                  sval - saturation value
//
// OUTPUT         : None
//
// GLOBALS        : GVideoDecoder	(W)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void dev_vpc323SetVideoSaturation(BYTE VidDecId,BYTE sval)
{
   WORD temp;

   GVideoDecoder[VidDecId].Saturation = sval;
   temp = (((WORD)sval*0xFF)/100);
   temp = temp << 4;
   dev_vpc323FPWrite(VidDecId,VPC_ACC_SAT_FP_REG,temp);
}
//******************************************************************************
//
// FUNCTION       : BYTE dev_vpc323GetVideoSaturation(BYTE VidDecId)
//
// USAGE          : Get the saturation value from video decoder device
//
// INPUT          : VidDecId - video decoder device logical ID (VDD_1, VDD_2)
//
// OUTPUT         : Saturation value
//
// GLOBALS        : GVideoDecoder	(R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BYTE dev_vpc323GetVideoSaturation(BYTE VidDecId)
{
   return GVideoDecoder[VidDecId].Saturation;
}

//******************************************************************************
//
// FUNCTION       : void dev_vpc323SetVideoHue(BYTE VidDecId,BYTE hval)
//
// USAGE          : Set the Hue for video decoder device
//
// INPUT          : VidDecId - video decoder device logical ID (VDD_1, VDD_2)
//                  hval - hue value
//
// OUTPUT         : None
//
// GLOBALS        : GVideoDecoder	(W)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void dev_vpc323SetVideoHue(BYTE VidDecId,BYTE hval)
{
   WORD temp;

   GVideoDecoder[VidDecId].Hue = hval;
   temp = (((WORD)hval*0xFF)/100);
   temp = temp & 0x00FF;
   if (temp > 0x7f)
   {
      temp = temp - 0x80;
      temp = temp << 1;
   }
   else
   {
      temp = 0x80 - temp;
      temp = temp << 1;
      temp = 0xFFFF-temp;
   }
   dev_vpc323FPWrite(VidDecId,VPC_TINT_FP_REG,temp);
}

//******************************************************************************
//
// FUNCTION       : BYTE dev_vpc323GetVideoHue(BYTE VidDecId)
//
// USAGE          : Get the Hue value from video decoder device
//
// INPUT          : VidDecId - video decoder device logical ID (VDD_1, VDD_2)
//
// OUTPUT         : Hue value
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BYTE dev_vpc323GetVideoHue(BYTE VidDecId)
{
   return GVideoDecoder[VidDecId].Hue;
}

//******************************************************************************
//
// FUNCTION       : void dev_vpc323SetVideoSharpness(BYTE VidDecId,BYTE sharpval)
//
// USAGE          : Set value for video sharpness.
//
// INPUT          : VidDecId - video decoder device logical ID (VDD_1, VDD_2)
//                  sharpval - sharpness value
//
// OUTPUT         : None
//
// GLOBALS        : GVideoDecoder	(W)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void dev_vpc323SetVideoSharpness(BYTE VidDecId,BYTE sharpval)
{
   WORD temp, temp1;

   GVideoDecoder[VidDecId].Sharpness = sharpval;
   temp1 = (WORD)sharpval;
   temp1 = temp1 & 0x0007;
   temp = dev_vpc323FPRead(VidDecId,VPC_SCCT_FP_REG);
   temp = temp & 0xF8FF;
   temp = temp | (temp1<<8);
   dev_vpc323FPWrite(VidDecId,VPC_SCCT_FP_REG,temp);
   temp = dev_vpc323FPRead(VidDecId,VPC_SCMODE_FP_REG);
   temp &= 0x7ff;
   dev_vpc323FPWrite(VidDecId,VPC_SCMODE_FP_REG,temp);
   temp = dev_vpc323FPRead(VidDecId,VPC_COMB_UC_FP_REG);
   temp = temp & 0xF0FF;
   temp = temp | (temp1<<9);
   dev_vpc323FPWrite(VidDecId,VPC_COMB_UC_FP_REG,temp);
}

//******************************************************************************
//
// FUNCTION       : BYTE dev_vpc323GetVideoSharpness(BYTE VidDecId)
//
// USAGE          : Get value of video sharpness.
//
// INPUT          : VidDecId - video decoder device logical ID (VDD_1, VDD_2)
//
// OUTPUT         : Sharpness value
//
// GLOBALS        : GVideoDecoder	(R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BYTE dev_vpc323GetVideoSharpness(BYTE VidDecId)
{
   return GVideoDecoder[VidDecId].Sharpness;
}

//????
//******************************************************************************
//
// FUNCTION       : void dev_vpc323PatGenControl(BYTE VidDecId,BYTE OnOff)
//
// USAGE          : Turn on/off PatGenControl.
//
// INPUT          : VidDecId - video decoder device logical ID (VDD_1, VDD_2)
//                  OnOff
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void dev_vpc323PatGenControl(BYTE VidDecId,BYTE OnOff)
{
   WORD Temp;
   Temp = dev_vpc323_WordRead(VidDecId,VPC_AVO_STOP_TWC_REG);
   if (OnOff)
   {
      Temp = Temp | 0x9800;
      dev_vpc323_WordWrite(VidDecId,VPC_AVO_STOP_TWC_REG,Temp);
   }
   else
   {
      Temp = Temp & ~(0x9800);
      dev_vpc323_WordWrite(VidDecId,VPC_AVO_STOP_TWC_REG,Temp);
   }
}
//******************************************************************************
//
// FUNCTION       : void dev_vpc323PowerUp(BYTE VidDecId)
//
// USAGE          : This function powers up the vpc323 modules.
//
// INPUT          : VidDecId - video decoder device logical ID (VDD_1, VDD_2)
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : Power Save Control
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void dev_vpc323PowerUp(BYTE VidDecId)
{
   VidDecId++;							// Dummy
}

//******************************************************************************
//
// FUNCTION       : void dev_vpc323PowerDown(BYTE VidDecId)
//
// USAGE          : This function powers down the vpc323 modules.
//
// INPUT          : VidDecId - video decoder device logical ID (VDD_1, VDD_2)
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : Power Save Control
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void dev_vpc323PowerDown(BYTE VidDecId)
{
   VidDecId++;							// Dummy
}
#endif
//**********************************  END  *************************************
