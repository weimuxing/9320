/*
   $Workfile:   smt_ColorControl.c  $
   $Revision: 1.5 $
   $Date: 2012/09/14 07:09:58 $
*/

//******************************************************************
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
//================================================================
//
// MODULE:      smt_acm3.c
//
//******************************************************************

#include "CommLink\smt.h"
#include "CommLink\smt_ColorControl.h"
//#include "StdLibInc\gm_Register.h"

//#define _DEBUG_MSG
#ifdef _DEBUG_MSG
#define msg(x,y)		gm_Printf(x,y)
#else
#define msg(x,y)
#endif

#ifdef USE_SMT

enum
{
   SD_CSC = 0,
   HD_CSC = 1,
   Graph_CSC =2
};

SWORD ROM GraphicsCSC[9] =
{
   4096,	0,		0,
   0,		4096,	0,
   0,		0,		4096
};

//******************************************************************************
//
// FUNCTION     :   BYTE ColorControl_SMT_MsgHandler(MsgPacket_t *message, BYTE comm_use)
// USAGE        :   This function parse the SMT messages for Color Control feature
//
// INPUT        :   MsgPacket_t *message = SMT message
//                  BYTE comm_use  = SERIAL_USE or DDC2BI_USE
// OUTPUT       :   OK = if no error,
//
//******************************************************************************
BYTE ColorControl_MsgHandler(MsgPacket_t *message, BYTE comm_use)
{
   BYTE * Bp_msg = (BYTE *)message->msgpkt;
   WORD W_Command = (WORD)(((WORD)Bp_msg[1]<<8)+(WORD)Bp_msg[0]);
   AdjValEnum AdjParam;
   ValTarget VDir = ValWrite;
   SDWORD sDW_OutVal;
   SDWORD sDW_InVal = (SDWORD)(((SDWORD)Bp_msg[3]<<8)+((SDWORD)Bp_msg[2]));
   SDWORD sDW_InMin =0;
   SDWORD sDW_OutMin = 0;
   SDWORD sDW_InMax = 0xFFFFl;
   SDWORD  sDW_OutMax = 0xFFFFl;

   //get pointer to output message length setting
   BYTE* len=GetOutMsgLenPtr();
   comm_use = comm_use;

   // Get pointer to output message buffer
   Bp_msg = GetOutMsgBufPtr();

   if ((len == NULL_PTR) || (Bp_msg == NULL_PTR))
   {
      return (BYTE)PORT_MSG_HNDLR_ERROR;
   }

   Bp_msg[0] = ColorControl_OK;

   //parse command
   switch (W_Command)
   {

      case ColorControl_GET_VERSION:
         // return 1;
         Bp_msg[1] = 0;
         Bp_msg[2] = 1;
         *len = 3;
         return (BYTE)PORT_MSG_HNDLR_OK;

      case ColorControl_GET_STATUS :
         // TODO: return real status after find out how to calculate this
         Bp_msg[1] = 0;
         Bp_msg[2] = 0;
         *len = 3;
         return (BYTE)PORT_MSG_HNDLR_OK;

      case ColorControl_COLOR_SPACE_READ :
         // Color space of main channel only
         {
            BYTE Inp = (BYTE)(stInput[gmvb_MainChannel].ModeFlags & gmd_MODE_SIGNAL_MASK);
            BYTE Inx;
            #if 1
            gmt_GetCSC CSC;

            // get pointers to color space coefficients array
            if(Input_YUV_Range == 1) // Full Range 
               if(Inp == gmd_MODE_HDVIDEO)
                  CSC = gm_GetCsc(3);
               else
                  CSC = gm_GetCsc(2);
            else
               if(Inp == gmd_MODE_HDVIDEO)
                  CSC = gm_GetCsc(1);
               else
                  CSC = gm_GetCsc(0);
            #else
            // get pointers to color space coefficients array
            gmt_GetCSC CSC = gm_GetCsc((Inp == gmd_MODE_HDVIDEO)?1:0);
            #endif
            if (Inp == gmd_MODE_GRAPHICS)
               CSC.Coeff = GraphicsCSC ;


            if (CSC.Coeff == NULL_PTR)
            {
               // return an error if wrong color space coefficients found
               Bp_msg[0] = ColorControl_ERROR;
               // message length
               *len = 1;
               return (BYTE)PORT_MSG_HNDLR_OK;
            }

            for (Inx = 0;Inx<=9;Inx++)
            {
               //reorder and copy color coefficients to output buffer
               Bp_msg[CSC.Shift[Inx]+1] = (BYTE)(CSC.Coeff[Inx]);
               Bp_msg[CSC.Shift[Inx]+2] = (BYTE)(CSC.Coeff[Inx]>>8);
            }
            *len = (2*Inx-1);
            return (BYTE)PORT_MSG_HNDLR_OK;
         }

      case ColorControl_BRIGHTNESS_READ:
         VDir = ValRead;
      case ColorControl_BRIGTNESS_WRITE:
         AdjParam = ADJV_Brightness;
         break;

      case ColorControl_CONTRAST_READ:
         VDir = ValRead;
      case ColorControl_CONTRAST_WRITE:
         AdjParam = ADJV_Contrast;
         break;

      case ColorControl_HUE_READ:
         VDir = ValRead;
      case ColorControl_HUE_WRITE:
         AdjParam = ADJV_Hue;
         break;

      case ColorControl_SATURATION_READ:
         VDir = ValRead;
      case ColorControl_SATURATION_WRITE:
         AdjParam = ADJV_Saturation;
         break;

      case ColorControl_RED_GAIN_READ:
         VDir = ValRead;
      case ColorControl_RED_GAIN_WRITE:
         AdjParam = ADJV_UserRedGain;
         break;

      case ColorControl_GREEN_GAIN_READ:
         VDir = ValRead;
      case ColorControl_GREEN_GAIN_WRITE:
         AdjParam = ADJV_UserGreenGain;
         break;

      case ColorControl_BLUE_GAIN_READ:
         VDir = ValRead;
      case ColorControl_BLUE_GAIN_WRITE:
         AdjParam = ADJV_UserBlueGain;
         break;

      default:

         Bp_msg[0] = ColorControl_INVALIDPARAM;
         *len = 1;//+HEADER_SIZE+CHECKSUM_SIZE;		// message length
         return (BYTE)PORT_MSG_HNDLR_OK;
   };


   //Get adj. data
   if (VDir == ValWrite)
   {
      sDW_OutMin = gm_GetAdjusterMin(AdjParam);
      sDW_OutMax= gm_GetAdjusterMax(AdjParam);
#ifdef _DEBUG_MSG
      (((SWORD)sDW_OutMin)&0x1000) ? msg("Min=-%d",(0-(SWORD)sDW_OutMin)) : msg("Min=%d",(SWORD)sDW_OutMin);
      (((SWORD)sDW_OutMax)&0x1000) ? msg("Max=-%d",(0-(SWORD)sDW_OutMax)) : msg("Max=%d",(SWORD)sDW_OutMax);
#endif
   }
   else
   {
      sDW_InMin = gm_GetAdjusterMin(AdjParam);
      sDW_InMax= gm_GetAdjusterMax(AdjParam);
      sDW_InVal = gm_GetAdjuster(AdjParam);

#ifdef _DEBUG_MSG
      (((SWORD)sDW_InMin)&0x1000) ? msg("Min=-%d",0-(SWORD)sDW_InMin) : msg("Min=%d",(SWORD)sDW_InMin);
      (((SWORD)sDW_InMax)&0x1000) ? msg("Max=-%d",0-(SWORD)sDW_InMax) : msg("Max=%d",(SWORD)sDW_InMax);
      (((SWORD)sDW_InVal)&0x1000) ? msg("Out=-%d",0-(SWORD)sDW_InVal) : msg("Out=%d",(SWORD)sDW_InVal);
#endif
   }

   // scale data to meet protocol and application requirements
   sDW_OutVal = ((sDW_InVal -sDW_InMin) * (sDW_OutMax - sDW_OutMin)+(sDW_InMax -sDW_InMin)/2) / (sDW_InMax -sDW_InMin) +  sDW_OutMin;

   // Write data
   if (VDir == ValWrite)
   {
      gm_SetAdjusterValue((WORD)AdjParam, (WORD)sDW_OutVal);
      gm_SetRegBitsWord(HOST_CONTROL,ODP_SYNC_UPDATE);
      *len = 1;//+HEADER_SIZE+CHECKSUM_SIZE;		// message length
#ifdef _DEBUG_MSG
      (((SWORD)sDW_OutVal)&0x1000) ? gm_Printf("Out=-%d",0-(SWORD)sDW_OutVal) : gm_Printf("Out=%d",(SWORD)sDW_OutVal);
#endif
   }
   else
   {
      Bp_msg[1] = (BYTE) sDW_OutVal ;
      Bp_msg[2] = (BYTE)(sDW_OutVal>>8);

      *len = 3;//+HEADER_SIZE+CHECKSUM_SIZE;		// message length
   }
   return (BYTE)PORT_MSG_HNDLR_OK;

}

#endif


