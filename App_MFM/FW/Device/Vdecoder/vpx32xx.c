/*
	$Workfile:   vpx32xx.c  $
	$Revision: 1.3 $
	$Date: 2011/12/15 03:52:11 $
*/

#define __VPX32XX_C__

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
// MODULE:	VPX32XX.c
//
// USAGE:	This file has functions for the VPC3230 video decoder.
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "System\all.h"

#ifdef VPX322xF_DEV_ID
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

VIDEO_DECODER_ GVideoDecoder[NUM_DECODER];





ROM BYTE Ba_Reg_I2C_Table_Init[] =
{
   /*1*/    VPX_I2C_llc,              	 //0xAA
   /*2*/    VPX_I2C_mask_lo,	    	//0xBA
   /*3*/    VPX_I2C_mask_mid,
   /*4*/    VPX_I2C_mask_hi,
   /*5*/    VPX_I2C_reference_lo,
   /*6*/    VPX_I2C_reference_mid,
   /*7*/    VPX_I2C_reference_hi,
   /*8*/    VPX_I2C_soft_sliser,
   /*9*/    VPX_I2C_ttx_freqLSB,
   /*10*/    VPX_I2C_ttx_freqMSB,
   /*11*/    VPX_I2C_coeff,
   /*12*/    VPX_I2C_data_sliser,
   /*13*/    VPX_I2C_accu,
   /*14*/    VPX_I2C_sync_sliser,
   /*15*/    VPX_I2C_standard,
   /*16*/    VPX_I2C_tolerance,
   /*17*/    VPX_I2C_byte_cnt,
   /*18*/    VPX_I2C_oena,
   /*19*/    VPX_I2C_driver_a,
   /*20*/    VPX_I2C_driver_b,
   0x00

};

/****************************************************************************/
/* VPX3226E NTSC part 1 Init (data for I2C registers)                       */
/****************************************************************************/

ROM BYTE Ba_VPX_ntsc1_Init[] =
{
   /*1*/	0x40,         	//llc           (AA)   Active mode,out of Power Down
   /*2*/	0x00,			//mask_lo       (BA)
   /*3*/    0x00,			//mask_mid      (B9)
   /*4*/    0x00,			//mask_hi       (B8)
   /*5*/	0x55, 			//reference_lo  (BD)
   /*6*/	0x55, 			//reference_mid (BC)
   /*7*/	0x27, 			//reference_hi  (BB)
   /*8*/   	0x10,			//soft_sliser   (C0)
   /*9*/	0xBE,			//ttx_freqLSB   (C1)
   /*10*/	0x0A,			//ttx_freqMSB   (C2)
   /*11*/	0x07,			//coeff			(C5)
   /*12*/	0x40,			//data_sliser   (C6)
   /*13*/	0x34,			//accu          (C7)
   /*14*/	0x40,			//sync_sliser   (C8)
   /*15*/	0x19,			//standard      (C9)
   /*16*/	0x15,	 		//tolerance		(CE)
   /*17*/	0x6B,			//byte_cnt		(CF)
   /*18*/    0x8D,           //oena          (F2)   Enable Pixel Bus, Clock and Syncs
   /*19*/	0x24,			//driver_a		(F8)   Select Standard Drivers Strength
   /*20*/	0x24			//driver_b		(F9)

};

/****************************************************************************/
/*         FP-RAM  registers for Init. (NTSC)                               */
/****************************************************************************/

ROM WORD Wa_Reg_FP_Table_Init[] =
{
   /*1*/	VPX_FP_gp_ctrl,
   /*2*/	VPX_FP_sdt,
   /*3*/    VPX_FP_insel,
   /*4*/   	VPX_FP_sfif,
   /*5*/   	VPX_FP_idly,
   /*6*/  	VPX_FP_comb_uc,
   /*7*/   	VPX_FP_acc_sat,
   /*8*/   	 VPX_FP_kilvl,
   /*9*/    	VPX_FP_kilhy,
   /*10*/    VPX_FP_autolock,
   /*11*/    VPX_FP_tint,
   /*12*/ 	VPX_FP_xlck,
   /*13*/    VPX_FP_dvco,
   /*14*/   VPX_FP_vbegin1,
   /* 15*/  VPX_FP_vlinesin1,
   /*16*/    VPX_FP_vlinesout1,
   /*17*/    VPX_FP_hbeg1,
   /*18*/    VPX_FP_hlen1,
   /*19*/    VPX_FP_npix1,
   /*20*/    VPX_FP_peaking1,
   /*21*/    VPX_FP_brightness1,
   /*22*/    VPX_FP_contrast1,
   /*23*/    VPX_FP_vbegin2,
   /*24*/    VPX_FP_vlinesin2,
   /*25*/    VPX_FP_vlinesout2,
   /*26*/    VPX_FP_hbeg2,
   /*27*/    VPX_FP_hlen2,
   /*28*/    VPX_FP_npix2,
   /*29*/    VPX_FP_peaking2,
   /*30*/    VPX_FP_brightness2,
   /*31*/    VPX_FP_contrast2,
   /*32*/    VPX_FP_start_even,
   /*33*/    VPX_FP_end_even,
   /*34*/    VPX_FP_start_odd,
   /*35*/    VPX_FP_end_odd,
   /*36*/    VPX_FP_vbicontrol,
   /* 37*/   VPX_FP_slsize,
   /*38*/    VPX_FP_ControlWord,
   /*39*/    VPX_FP_format_sel,
   /*40*/    VPX_FP_pval_start,
   /*41*/    VPX_FP_pval_stop,
   /*42*/    VPX_FP_refsig,
   /*43*/    VPX_FP_outmux,
   /*44*/    VPX_FP_tdecframes,
   /*45*/    VPX_FP_asr_enable,
   /*46*/    VPX_FP_asr_status,
   /*47*/    VPX_FP_mcv_start,
   /*48*/	VPX_FP_mcv_stop,
   0x00
};

/****************************************************************************/
/* VPX3226E PAL part 2 Init (data for FP-RAM registers)                     */
/****************************************************************************/

ROM WORD Wa_VPX_ntsc2_Init[] =
{
   /*1*/	0x0200,			//gp_ctrl		(012)
   /*2*/	0x0047,			//sdt       	(020)  - select NTSC COMB
   /*3*/	0x0666,			//insel			(021)  - SVIDEO INPUT
   /*4*/	0x0000,			//sfif			(022)
   /*5*/	0x0F80,			//idly			(023)  luma/chroma delay: default 0x0000
   /*6*/	0x00E7,			//comb_uc		(028)
   /*7*/	0x0816,			//acc_sat		(030)
   /*8*/	0x0019,			//kilvl			(039)
   /*9*/	0x0005,			//kilhy			(03A)
   /*10*/	0x0190,			//autolock		(0B5)
   /*11*/	0x0000,			//tint			(0DC)
   /*12*/	0x0000,			//xlck			(0F7)
   /*13*/	0x0D30,			//dvco      	(0F8)
   /*14*/	0X000C,			//vbegin1		(120)
   /*15*/	0x0000,			//vlinesin1		(121)
   /*16*/	0x0000,			//vlinesout1 	(122)
   /*17*/	0x0000,			//hbeg1			(123)
   /*18*/	0x02C0,			//hlen1			(124)
   /*19*/	0x02C0,			//npix1			(125)
   /*20*/	0x0200,			//peaking1		(126)
   /*21*/	0x0000,			//brightness1	(127)
   /*22*/	0x0020,			//contrast1		(128)
   /*23*/	0x0016,			//vbegin2		(12A) - Active Video Vertical Begin Line = 22
   /*24*/	0x01F4,			//vlinesin2		(12B)
   /*25*/	0x01F4,			//vlinesout2	(12C) - Vertical line input = Vertical line output = 500
   /*26*/	0x0008,			//hbeg2			(12D) - Horisontal begin = 8
   /*27*/	0x02D0,			//hlen2			(12E) - Active Output Pixel = 720
   /*28*/	0x02DA,			//npix2			(12F) - Internal Pixels after down scale = 730
   /*29*/	0x0200,			//peaking2		(130)
   /*30*/	0x00E8,			//brightness2	(131)
   /*31*/	0X0020,			//contrast2		(132)
   /*32*/	0x0110,			//start_even	(134)
   /*33*/	0x011B,			//end_even		(135)
   /*34*/	0x000A,			//start_odd		(136)
   /*35*/	0x0015,			//end_odd		(137)
   /*36*/	0x0000,			//vbicontrol	(138) - disable VBI window,disable Sliced Data Mode
   /*37*/	0x0000,			//slsize		(139)
   /*38*/	0x0060,			//ControlWord	(140) - Latch values in the chip
   /*39*/	0x0401,			//format_sel	(150) - ITU 656 Ycbcr 4:2:2
   /*40*/	0x0028,			//pval_start	(151)
   /*41*/	0x02D0,			//pval_stop		(152)
   /*42*/   0x0000,			//refsig		(153)
   /*43*/	0x0300,			//outmux		(154) - Port A[15:8],27 MHZ Mode
   /*44*/	0x0BB8,			//tdecframes	(157)
   /*45*/	0x0007,			//asr_enable	(158) - NTSC, PAL, SECAM enabled
   /*46*/	0x0000,			//asr_status	(15E)
   /*47*/	0x0006,			//mcv_start		(171)
   /*48*/	0x000F,			//mcv_stop		(172)

};


/****************************************************************************/
/*             Register I2C table for standard was detected                 */
/****************************************************************************/

ROM BYTE Ba_Reg_I2C_Table[] =
{
   VPX_I2C_llc,
   VPX_I2C_oena,
   VPX_I2C_driver_a,
   0x00,

};

/****************************************************************************/
/*             Register FP-RAM table for standard was detected              */
/****************************************************************************/

ROM WORD Wa_Reg_FP_Table[] =
{
   VPX_FP_format_sel,
   VPX_FP_outmux,
   VPX_FP_vbegin2,
   VPX_FP_vlinesout2,
   VPX_FP_hbeg2,
   VPX_FP_hlen2,
   VPX_FP_npix2,
   VPX_FP_ControlWord,
   VPX_FP_vbicontrol,
   0x00,

};


/****************************************************************************/
/* VPX3226E NTSC part 1 (data for I2C registers)                            */
/****************************************************************************/

ROM BYTE Ba_VPX_ntsc1[] =
{
   0x40,     //reg.AA - active mode,out of Power Down
   0x8D,     //reg.F2 - enable Pixel Bus, Clock and Syncs
   0x24	  //reg.F8 - Select Standard Drivers Strength
};

/****************************************************************************/
/* VPX3226E NTSC part 2 (data for FP-RAM registers)                         */
/****************************************************************************/

//ROM
WORD Wa_VPX_ntsc2[] =
{
   0x0401,    //reg.0150 - ITU 656 Ycbcr 4:2:2
   0x0300,    //reg.0154 - Port A[15:8],27 MHZ Mode
   0x0016,    //reg.012A - Active Video Vertical Begin Line = 22
   0x01F4,    //reg.012C - Vertical line input = Vertical line output = 500
   0x0008,    //reg.012D - Horisontal begin = 8
   0x02D0,    //reg.012E - Active Output Pixel = 720
   0x02DA,    //reg.012F - Internal Pixels after down scale = 730
   0x0060,    //reg.0140 - Latch values in the chip
   0x0000   //reg.0138 - disable VBI window,disable Sliced Data Mode

};


/****************************************************************************/
/* VPX3226E PAL part 1 (data for I2C registers)                             */
/****************************************************************************/

ROM BYTE Ba_VPX_pal1[] =
{
   0x00,	  //reg.AA - active mode,out of Power Down
   0x8D,     //reg.F2 - enable Pixel Bus, Clock and Syncs
   0x24      //reg.F8 - Select Standard Drivers Strength
};
/****************************************************************************/
/* VPX3226E PAL part 2 (data for FP-RAM registers)                          */
/****************************************************************************/

ROM WORD Wa_VPX_pal2[] =
{
   0x0401,      //reg.0150 - ITU 656 Ycbcr 4:2:2
   0x0300,      //reg.0154 - Port A[15:8],27 MHZ Mode
   0x0018,      //reg.012A - Active Video Vertical Begin Line = 24
   0x0258,      //reg.012C - Vertical line input = Vertical line output = 600
   0x0008,      //reg.012D - Horisontal begin = 8
   0x02D0,		 //reg.012E - Active Output Pixel = 720
   0x02DA,      //reg.012F - Internal Pixels after down scale = 730
   0x0060,      //reg.0140 - Latch values in the chip
   0x0000     //reg.0138 - disable VBI window,disable Sliced Data Mode

};




//******************************************************************************
//  S T A T I C    V A R I A B L E S
//******************************************************************************

//******************************************************************************
//  S T A T I C    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************



//******************************************************************************
//  C O D E
//******************************************************************************

//******************************************************************************
//
// FUNCTION       : BYTE vdd_Poll_FlBusy (void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : OK or ERR_BUSY
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

BYTE vdd_Poll_FlBusy (void)
{
   BYTE B_cnt, B_Dat;
   //
   // FP-processor READY-flag polling
   //
   for (B_cnt = 0; B_cnt < 21; B_cnt++)
   {
      B_Dat = dev_vpx32xxByteRead(VPX322xF_DEV_ID,VPX_I2C_FPSTA);  // FP-processor ready
      if  (!(B_Dat & BIT2))                 // for communication?
         return(OK);                       // Yes
      else
         gm_Delay1ms(5);                      // Waiting for response
   }

   return(ERR_BUSY);                              // No response from FP
}

//******************************************************************************
//
// FUNCTION       :   void dev_vpx32xxWordWrite(BYTE VidDecId,BYTE addr,WORD wrdata)
//
// USAGE          : Write 16-bit data to a location on vpc323
//
// INPUT          : VidDecId
//                  addr
//                  wrdata
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
void dev_vpx32xxWordWrite(BYTE VidDecId,BYTE addr,WORD wrdata)
{
   BYTE  temp[3];
   if (ERR_BUSY == vdd_Poll_FlBusy())
      return;
   VidDecId++;						// Dummy
   temp[0] = addr;
   temp[1] = (wrdata >> 8) & 0x00ff;
   temp[2] = wrdata & 0x00ff;



   gm_Write2WireBlock(0, VPX322xF_DEV_ID, (BYTE *)&temp, 3, TRUE);

}

//******************************************************************************
//
// FUNCTION       :  void dev_vpx32xxByteWrite(BYTE VidDecId,BYTE addr,BYTE wrdata)
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
void dev_vpx32xxByteWrite(BYTE VidDecId,BYTE addr,BYTE wrdata)
{
   BYTE  temp[2];

   VidDecId++;						// Dummy
   temp[0] = addr;
   temp[1] = wrdata;

   gm_Write2WireBlock(0, VPX322xF_DEV_ID, (BYTE *)&temp, 2, TRUE);

}


//******************************************************************************
//
// FUNCTION       :   WORD dev_vpx32xxWordRead(BYTE VidDecId, BYTE addr)
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
WORD dev_vpx32xxWordRead(BYTE VidDecId, BYTE addr)
{
   BYTE address;
   WORD rdata;
   WORD temp;

   VidDecId++;						// Dummy
   address = addr;

   gm_Write2WireBlock(0, VPX322xF_DEV_ID, (BYTE *)&address, 1, FALSE);
   gm_Read2WireBlock(0, VPX322xF_DEV_ID, (BYTE *)&temp, 2, TRUE);

   rdata = (temp << 8) & 0xff00;
   rdata = rdata | ((temp >> 8) & 0x00ff);

   return rdata;
}

//******************************************************************************
//
// FUNCTION       :   BYTE dev_vpx32xxByteRead(BYTE VidDecId, BYTE addr)
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
BYTE dev_vpx32xxByteRead(BYTE VidDecId, BYTE addr)
{
   BYTE rdata;
   BYTE address;

   VidDecId++;						// Dummy
   address = addr;

   gm_Write2WireBlock(0, VPX322xF_DEV_ID, (BYTE *)&address, 1, FALSE);

   gm_Read2WireBlock(0, VPX322xF_DEV_ID, (BYTE *)&rdata, 1, TRUE);


   return rdata;
}

//******************************************************************************
//
// FUNCTION       : void dev_vpx32xxFPWrite(BYTE VidDecId,WORD addr,WORD wrdata)
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
void dev_vpx32xxFPWrite(BYTE VidDecId,WORD addr,WORD wrdata)
{
   dev_vpx32xxWordWrite(VidDecId,VPX_FPWRITE_ADDR_TWC_REG,addr);
   dev_vpx32xxWordWrite(VidDecId,VPX_FPDATA_TWC_REG,wrdata);

}

//******************************************************************************
//
// FUNCTION       : WORD dev_vpx32xxFPRead(BYTE VidDecId,WORD addr)
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
WORD dev_vpx32xxFPRead(BYTE VidDecId,WORD addr)
{
   dev_vpx32xxWordWrite(VidDecId,VPX_FPREAD_ADDR_TWC_REG,addr);
   return dev_vpx32xxWordRead(VidDecId,VPX_FPDATA_TWC_REG);

}


//******************************************************************************
//
// FUNCTION       : void dev_vpx32xxWriteFPRamRandom(WORD *Wp_AddrTable, WORD *Wp_DataTable)
//
// USAGE          :
//
// INPUT          : Wp_AddrTable
//                  Wp_DataTable
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
void dev_vpx32xxWriteFPRamRandom(WORD *Wp_AddrTable, WORD *Wp_DataTable)
{
   WORD temp1;
   while (*Wp_AddrTable)
   {
      //vdd_WriteFP_Ram(*Wp_AddrTable, *Wp_DataTable);
      dev_vpx32xxFPWrite(VPX322xF_DEV_ID,*Wp_AddrTable,*Wp_DataTable);

      Wp_AddrTable++;						// Inc address table pointer
      Wp_DataTable++;						// Inc data table pointer


   }
}


//******************************************************************************
//
// FUNCTION       : void dev_vpx32xxWriteBufRandom(BYTE *Bp_AddrTable, BYTE *Bp_DataTable)
//
// USAGE          :
//
// INPUT          : Bp_AddrTable
//                  Bp_DataTable
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
void dev_vpx32xxWriteBufRandom(BYTE *Bp_AddrTable, BYTE *Bp_DataTable)
{
   BYTE temp1;
   while (*Bp_AddrTable)
   {
      dev_vpx32xxByteWrite(VPX322xF_DEV_ID, *Bp_AddrTable, *Bp_DataTable);
      //i2c_SendData(B_DeviceAddr, *Bp_AddrTable, *Bp_DataTable);


      Bp_AddrTable++;						// Inc address table pointer
      Bp_DataTable++;						// Inc data table pointer


   }
}


//******************************************************************************
//
// FUNCTION       : void dev_vpx32xxProgramDevice(BYTE B_Mode)
//
// USAGE          :
//
// INPUT          : B_Mode
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
void dev_vpx32xxProgramDevice(BYTE B_Mode)
{
   WORD W_VidStat;

   W_VidStat = dev_vpx32xxFPRead(VPX322xF_DEV_ID,VPX_FP_insel);
   dev_vpx32xxFPWrite(VPX322xF_DEV_ID,VPX_FP_asr_enable,0x0000);     //Disable recognition.
   dev_vpx32xxFPWrite(VPX322xF_DEV_ID,VPX_FP_format_sel,0xd51);
   dev_vpx32xxByteWrite(VPX322xF_DEV_ID,VPX_I2C_oena,0x0f);

   if (B_Mode == VDD_MODE_NTSC)										// NTSC
   {
      dev_vpx32xxWriteBufRandom((BYTE*)Ba_Reg_I2C_Table,(BYTE*)Ba_VPX_ntsc1);	//NTSC mode parameters

      dev_vpx32xxWriteFPRamRandom((WORD*)Wa_Reg_FP_Table,(WORD *)Wa_VPX_ntsc2);//loading


      if (W_VidStat == 0x0E66||W_VidStat == 0x0806)	// for SVIDEO_INPUT:
      {							     				//  S-VHS mode on,
         dev_vpx32xxFPWrite(VPX322xF_DEV_ID,VPX_FP_sdt,0x0041);    		//  Comb mode off,NTSC set
      }	                                   			//

      else if (W_VidStat == 0x0E61)					// for VIDEO_INPUT:
      {												//  S-VHS mode off,
         dev_vpx32xxFPWrite(VPX322xF_DEV_ID,VPX_FP_sdt,0x0021);			//  Comb mode onn,NTSC set
      }												//

      else if (W_VidStat == 0x0E64)					// for TV_INPUT:
      {												//  S-VHS mode off,
         dev_vpx32xxFPWrite(VPX322xF_DEV_ID,VPX_FP_sdt,0x0007);			//  Select NTSC COMBO TV standard
      }												//
   }
   else if (B_Mode == VDD_MODE_PAL)
   {
      dev_vpx32xxWriteBufRandom((BYTE*)Ba_Reg_I2C_Table,(BYTE*)Ba_VPX_pal1);   //PAL mode
      dev_vpx32xxWriteFPRamRandom((WORD*)Wa_Reg_FP_Table,(WORD*)Wa_VPX_pal2);

      if ((W_VidStat&0x0006) == 0x0006)
      {
         dev_vpx32xxFPWrite(VPX322xF_DEV_ID,VPX_FP_sdt,0x0040);
      }
      else if ((W_VidStat&0x0001) == 0x0001)
      {
         dev_vpx32xxFPWrite(VPX322xF_DEV_ID,VPX_FP_sdt,0x0020);
      }
      else if ((W_VidStat&0x0004) == 0x0004)
      {
         dev_vpx32xxFPWrite(VPX322xF_DEV_ID,VPX_FP_sdt,0x0020);  // Select PAL TV with 4H Combo Filter
      }
   }
   else if (B_Mode == VDD_MODE_SECAM)
   {
   }
   dev_vpx32xxFPWrite(VPX322xF_DEV_ID,VPX_FP_asr_enable,0x0007);       //Enable recognition.
}

//******************************************************************************
//
// FUNCTION       : void dev_vpx32xxSetVideoInput(BYTE VidDecId,BYTE In_Type)
//
// USAGE          : Set video input type for video decoder device
//
// INPUT          : VidDecId
//                  In_Type
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
void dev_vpx32xxSetVideoInput(BYTE VidDecId,BYTE In_Type)
{
   WORD W_Port,W_VidStat;

   dev_vpx32xxFPWrite(VPX322xF_DEV_ID,VPX_FP_format_sel,0xd51);
   dev_vpx32xxByteWrite(VPX322xF_DEV_ID,VPX_I2C_oena,0x0f);
   dev_vpx32xxFPWrite(VidDecId,VPX_FP_asr_enable,0x0000); // Disable recognition
   W_VidStat = dev_vpx32xxFPRead(VidDecId,VPX_FP_sdt);

   if (In_Type == VDD_INTYP_SVIDEO)            // Svideo input ?
   {											// Yes
      W_Port = 0x0666;						// SVIDEO_INPUT select
      W_VidStat |= 0x0040;	                // Enable S_VHS mode
      W_VidStat &= 0x07DF;                    // Combo mode disable
   }
   else if (In_Type == VDD_INTYP_COMPOSITE1)             // Composite input ?
   {											// Yes
      W_Port = 0x0661;						// VIDEO_INPUT select
      W_VidStat &= 0x07BF;                    // Composite mode
      W_VidStat |= 0x0020; 					// Combo mode enable
   }
   dev_vpx32xxFPWrite(VidDecId,VPX_FP_sdt,W_VidStat);
   gm_Delay10ms(3);// writing process into this register
   dev_vpx32xxFPWrite(VidDecId,VPX_FP_insel,W_Port);		// Input selection.
   dev_vpx32xxFPRead(VidDecId,VPX_FP_insel);//ST051905
   dev_vpx32xxFPWrite(VidDecId,VPX_FP_asr_enable,0x0007);	// Enable recognition
}

//******************************************************************************
//
// FUNCTION       : BYTE dev_vpx32xxGetVideoMode(BYTE VidDecId)
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
#if (0)

BYTE dev_vpx32xxGetVideoMode(BYTE VidDecId)
{
   BYTE status,submode;

   submode = dev_vpx32xxGetVideoSubMode(VidDecId);
   submode = submode & 0x0007;
   status = dev_vpx32xxGetVideoStatus(VidDecId);

   if (dev_vpx32xxGetVideoInput(VidDecId) == VDD_INTYP_TUNER)
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

#endif

//******************************************************************************
//
// FUNCTION       : BYTE dev_vp32xxGetVideoSubMode(BYTE VidDecId)
//
// USAGE          :
//
// INPUT          : VidDecId
//
// OUTPUT         :
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
BYTE dev_vp32xxGetVideoSubMode(BYTE VidDecId)
{
   WORD submode;


   submode = dev_vpx32xxFPRead(VidDecId,VPX_FP_sdt);
   submode = submode & 0x0007;
   GVideoDecoder[VidDecId].SubMode = (BYTE)submode;
   return GVideoDecoder[VidDecId].SubMode;
}

//******************************************************************************
//
// FUNCTION       : BYTE dev_vpx32xxGetVideoMode(BYTE VidDecId)
//
// USAGE          :
//
// INPUT          : VidDecId
//
// OUTPUT         :
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
BYTE dev_vpx32xxGetVideoMode(BYTE VidDecId)
{

#if 0
   WORD W_status,B_VideoMode;

//W_status = vdd_ReadFP_Ram(VPX_FP_asr_status);    // Read decoder status
   W_status = dev_vpx32xxFPRead(VidDecId,VPX_FP_asr_status);
   if ((W_status & 0x001F) != 0)                    // Signal exists?
      //return (NO_VIDEO_SIGNAL);                    // No
      return	VDD_MODE_NOVIDEO;
   //W_status = vdd_ReadFP_Ram(VPX_FP_sdt);           // Read video standard
   W_status = dev_vpx32xxFPRead(VPX322xF_DEV_ID,VPX_FP_sdt);
   W_status &= 0x0007;
   if ((W_status == 0x0007)||(W_status == 0x0001))  // NTSC COMBO,NTSCM(60HZ)
      return (VDD_MODE_NTSC);
   else if (W_status == 0x0002)
      return (VDD_MODE_SECAM);                              // SECAM(50Hz)
   else if (W_status == 0x0000)
      return (VDD_MODE_PAL);                                // PAL B,G,H,I(50Hz)
   else return(VDD_MODE_NOVIDEO);                        // Detected standard
#endif



   BYTE status,submode;

   submode = dev_vpx32xxGetVideoSubMode(VidDecId);
   submode = submode & 0x0007;
   status = dev_vpx32xxGetVideoStatus(VidDecId);

   if (dev_vpx32xxGetVideoInput(VidDecId) == VDD_INTYP_TUNER)
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

}                                                    // not enabled

//******************************************************************************
//
// FUNCTION       : BYTE dev_vpx32xxGetVideoInput(BYTE VidDecId)
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
BYTE dev_vpx32xxGetVideoInput(BYTE VidDecId)
{
   return GVideoDecoder[VidDecId].InpType;
}



//******************************************************************************
//
// FUNCTION       : void dev_vpx32xxSetVideoOutputFormat(BYTE VidDecId,BYTE outtype)
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
void dev_vpx32xxSetVideoOutputFormat(BYTE VidDecId,BYTE outtype)
{
   GVideoDecoder[VidDecId].OutType = outtype;
   switch (outtype)
   {
      case VDD_OUTTYPE_601:
         msg_vdd("ITU601 ",0);
         dev_vpx32xxFPWrite(VidDecId, VPX_FP_format_sel,0x400);

         break;

      case VDD_OUTTYPE_656:
         msg_vdd("ITU656 ",0);
         dev_vpx32xxFPWrite(VidDecId, VPX_FP_format_sel,0xd51);
         break;

      default:
         break;
   }
}


//******************************************************************************
//
// FUNCTION       : BYTE dev_vpx32xxGetVideoOutputFormat(BYTE VidDecId)
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
#if 0
BYTE dev_vpx32xxGetVideoOutputFormat(BYTE VidDecId)
{
   return GVideoDecoder[VidDecId].OutType;
}
#endif

//******************************************************************************
//
// FUNCTION       : BYTE dev_vpx32xxGetVideoStatus(BYTE VidDecId)
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
BYTE dev_vpx32xxGetVideoStatus(BYTE VidDecId)
{
   WORD status;
   status = dev_vpx32xxFPRead(VidDecId,VPX_FP_asr);

   dev_vpx32xxFPWrite(VPX322xF_DEV_ID,VPX_FP_format_sel,0xd51);
   dev_vpx32xxByteWrite(VPX322xF_DEV_ID,VPX_I2C_oena,0x0f);

   msg_vdd("Video decoder status = %x",status);
   /*
   // there is not necessary to check for color presence for the composite input !!!
     if (
     	(dev_vpx32xxGetVideoInput(VidDecId) != VDD_INTYP_TUNER)  &&
     	(dev_vpx32xxGetVideoInput(VidDecId) != VDD_INTYP_COMPONENT) &&
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
// FUNCTION       : BYTE dev_vpx32xxGetVideoInterlaceInfo(BYTE VidDecId)
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
BYTE dev_vpx32xxGetVideoInterlaceInfo(BYTE VidDecId)
{
   WORD temp;

   dev_vpx32xxFPWrite(VPX322xF_DEV_ID,VPX_FP_format_sel,0xd51);
   dev_vpx32xxByteWrite(VPX322xF_DEV_ID,VPX_I2C_oena,0x0f);

   temp = dev_vpx32xxFPRead(VidDecId,VPX_FP_asr);

   if (temp & 0x0080)
      GVideoDecoder[VidDecId].Interlace = D_INTERLACED;
   else
      GVideoDecoder[VidDecId].Interlace = D_NONINTERLACED;

   return GVideoDecoder[VidDecId].Interlace;
}

//******************************************************************************
//
// FUNCTION       : WORD dev_vpx32xxGetVideoLinesPerField(BYTE VidDecId)
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
WORD dev_vpx32xxGetVideoLinesPerField(BYTE VidDecId)
{
   WORD temp;

   temp = dev_vpx32xxFPRead(VidDecId,VPX_FP_nlpf);
   GVideoDecoder[VidDecId].LPF = temp;
   return GVideoDecoder[VidDecId].LPF;
}

//******************************************************************************
//
// FUNCTION       : BYTE dev_vpx32xxGetVideoSubMode(BYTE VidDecId)
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
BYTE dev_vpx32xxGetVideoSubMode(BYTE VidDecId)
{
   WORD submode;

   dev_vpx32xxFPWrite(VPX322xF_DEV_ID,VPX_FP_format_sel,0xd51);
   dev_vpx32xxByteWrite(VPX322xF_DEV_ID,VPX_I2C_oena,0x0f);

   submode = dev_vpx32xxFPRead(VidDecId,VPX_FP_sdt);
   submode = submode & 0x0007;
   GVideoDecoder[VidDecId].SubMode = (BYTE)submode;
   return GVideoDecoder[VidDecId].SubMode;
}




//******************************************************************************
//
// FUNCTION       : void dev_vpx32xxEnableDisplay (void)
//
// USAGE          :
//
// INPUT          : None
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

void dev_vpx32xxEnableDisplay (void)
{
   dev_vpx32xxByteWrite(VPX322xF_DEV_ID,VPX_I2C_oena, 0x8D);      //enable pixel bus, clock and syncs.
}


//******************************************************************************
//
// FUNCTION       : void dev_vpx32xxConfigVideoMode(BYTE VidDecId,BYTE mode)
//
// USAGE          : Initializes video decoder device with a specified video mode
//
// INPUT          : VidDecId - video decoder device logical ID (VDD_1, VDD_2)
//                  mode - VDD_MODE_NOVIDEO, VDD_MODE_NTSC, VDD_MODE_PAL,
//                  VDD_MODE_SECAM, VDD_MODE_INIT
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
void dev_vpx32xxConfigVideoMode(BYTE VidDecId,BYTE mode)
{

   BYTE temp;
   WORD W_VidStat;
   switch (mode)
   {
      case VDD_MODE_INIT:
         //
         //	Default setting: NTSC, SVIDEO_INPUT.
         //  For correct operation, changes of registers 20h and 21h are allowed
         //	while standard recognition is disabled.
         //
         //	vdd_WriteFP_Ram(VPX_FP_asr_enable,0x0000);
         dev_vpx32xxByteWrite(VidDecId,VPX_I2C_llc,BIT2);//1  ST051905 Softrest.


         dev_vpx32xxFPWrite(VidDecId,VPX_FP_asr_enable, 0x0000); //Disable recognition.

         //	vdd_WriteBufRandom((BYTE *)Ba_Reg_I2C_Table_Init,(BYTE *)Ba_VPX_ntsc1_Init);   //Parameters loading
         dev_vpx32xxWriteBufRandom((BYTE *)Ba_Reg_I2C_Table_Init,(BYTE *)Ba_VPX_ntsc1_Init);

         //	vdd_WriteFP_RamRandom(Wa_Reg_FP_Table_Init,Wa_VPX_ntsc2_Init); //
         dev_vpx32xxWriteFPRamRandom((WORD*)Wa_Reg_FP_Table_Init,(WORD*)Wa_VPX_ntsc2_Init);

         //	vdd_WriteFP_Ram(VPX_FP_asr_enable,0x0007);                     //Enable recognition.
         dev_vpx32xxFPWrite(VidDecId, VPX_FP_asr_enable,0x0007);

         //vdd_SelectInputPort(SVIDEO_INPUT);	   //Input Selector setting with polling process.


         dev_vpx32xxSetVideoInput(VidDecId,VDD_INTYP_SVIDEO);  //Sailesh
         break;

      case VDD_MODE_NTSC:
         {
            W_VidStat = dev_vpx32xxFPRead(VidDecId,VPX_FP_insel);
            dev_vpx32xxFPWrite(VidDecId,VPX_FP_asr_enable,0x0000);     //Disable recognition.

            dev_vpx32xxWriteBufRandom((BYTE*) Ba_Reg_I2C_Table,(BYTE*)Ba_VPX_ntsc1);	//NTSC mode parameters
            dev_vpx32xxWriteFPRamRandom((WORD*)Wa_Reg_FP_Table,(WORD*)Wa_VPX_ntsc2);//loading

            if (W_VidStat == 0x0E66||W_VidStat == 0x0806)	// for SVIDEO_INPUT:
            {							     				//  S-VHS mode on,
               dev_vpx32xxFPWrite(VidDecId,VPX_FP_sdt,0x0041);    		//  Comb mode off,NTSC set
            }	                                   			//

            else if (W_VidStat == 0x0E61)					// for VIDEO_INPUT:
            {												//  S-VHS mode off,
               dev_vpx32xxFPWrite(VidDecId,VPX_FP_sdt,0x0021);			//  Comb mode onn,NTSC set
            }												//

            else if (W_VidStat == 0x0E64)					// for TV_INPUT:
            {												//  S-VHS mode off,
               dev_vpx32xxFPWrite(VidDecId,VPX_FP_sdt,0x0007);			//  Select NTSC COMBO TV standard
            }												//
         }
         break;

      case VDD_MODE_PAL:
         {
            dev_vpx32xxWriteBufRandom((BYTE*)Ba_Reg_I2C_Table,(BYTE*)Ba_VPX_pal1);   //PAL mode
            dev_vpx32xxWriteFPRamRandom((WORD*)Wa_Reg_FP_Table,(WORD*)Wa_VPX_pal2);

            if ((W_VidStat & 0x0006) == 0x0006)
            {
               dev_vpx32xxFPWrite(VidDecId,VPX_FP_sdt,0x0040);
            }
            else if (W_VidStat & BIT0)
            {
               dev_vpx32xxFPWrite(VidDecId,VPX_FP_sdt,0x0020);
            }
            else if (W_VidStat & BIT2)
            {
               dev_vpx32xxFPWrite(VidDecId,VPX_FP_sdt,0x0020);			// Select PAL TV with 4H Combo Filter
            }
         }
         break;

      case VDD_MODE_SECAM:
         {
            dev_vpx32xxFPWrite(VidDecId,VPX_FP_asr_enable, 0x0007);			//Enable recognition
            break;
         }
   }
   dev_vpx32xxEnableDisplay ();//enable pixel bus, clock and syncs. //ST
}

//******************************************************************************
//
// FUNCTION       : void dev_vpx32xxVideoMixerControl(BYTE VidDecId,BYTE OnOff)
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
void dev_vpx32xxVideoMixerControl(BYTE VidDecId,BYTE OnOff)
{
   BYTE temp;

   temp = dev_vpx32xxByteRead(VidDecId,VPX_FP_npix2);
   temp = temp & 0x3F;

   if (OnOff == D_MIXER_ON)
   {
      temp = temp | 0x40;
   }
   else
   {
      temp = temp | 0xC0;
   }
   dev_vpx32xxByteWrite(VidDecId,VPX_FP_npix2,temp);

}

//******************************************************************************
//
// FUNCTION       : void dev_vpx32xxSetVideoBrightness(BYTE VidDecId,BYTE bval)
//
// USAGE          : Set the brightness for video decoder device
//
// INPUT          : VidDecId - video decoder device logical ID (VDD_1, VDD_2)
//                  :   bval - brightness value
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
#ifdef VPX_COLORS_USED //ST
void dev_vpx32xxSetVideoBrightness(BYTE VidDecId,BYTE bval)
{
   WORD temp, temp1;

   GVideoDecoder[VidDecId].Brightness = bval;
   temp1 = (((WORD)bval*0xFF)/100);
   temp1 = temp1 & 0x00FF;
   if (temp1 > 0x7f)
      temp1 = temp1 - 0x80;
   else
      temp1 = temp1 + 0x80;
   temp = dev_vpx32xxFPRead(VidDecId,VPX_FP_brightness2);
   temp = temp & 0xFF00;
   temp = temp | temp1;
   dev_vpx32xxFPWrite(VidDecId,VPX_FP_brightness2,temp | 0x0100);
   temp = dev_vpx32xxFPRead(VidDecId,VPC_SCMODE_FP_REG);
   temp &= 0x7ff;
   dev_vpx32xxFPWrite(VidDecId,VPC_SCMODE_FP_REG,temp);
}

//******************************************************************************
//
// FUNCTION       : BYTE dev_vpx32xxGetVideoBrightness(BYTE VidDecId)
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
BYTE dev_vpx32xxGetVideoBrightness(BYTE VidDecId)
{
   return GVideoDecoder[VidDecId].Brightness;
}

//******************************************************************************
//
// FUNCTION       : void dev_vpx32xxSetVideoContrast(BYTE VidDecId,BYTE cval)
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
void dev_vpx32xxSetVideoContrast(BYTE VidDecId,BYTE cval)
{
   WORD temp, temp1;

   GVideoDecoder[VidDecId].Contrast = cval;
   temp1 = (((WORD)cval*0x3F)/100);
   temp1 = temp1 & 0x003F;
   temp = dev_vpx32xxFPRead(VidDecId,VPC_SCCT_FP_REG);
   temp = temp & 0xFFC0;
   temp = temp | temp1;
   dev_vpx32xxFPWrite(VidDecId,VPC_SCCT_FP_REG,temp);
   temp = dev_vpx32xxFPRead(VidDecId,VPC_SCMODE_FP_REG);
   temp &= 0x7ff;
   dev_vpx32xxFPWrite(VidDecId,VPC_SCMODE_FP_REG,temp);
}

//******************************************************************************
//
// FUNCTION       : BYTE dev_vpx32xxGetVideoContrast(BYTE VidDecId)
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
BYTE dev_vpx32xxGetVideoContrast(BYTE VidDecId)
{
   return GVideoDecoder[VidDecId].Contrast;
}

//******************************************************************************
//
// FUNCTION       : void dev_vpx32xxSetVideoSaturation(BYTE VidDecId,BYTE sval)
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
void dev_vpx32xxSetVideoSaturation(BYTE VidDecId,BYTE sval)
{
   WORD temp;

   GVideoDecoder[VidDecId].Saturation = sval;
   temp = (((WORD)sval*0xFF)/100);
   temp = temp << 4;
   dev_vpx32xxFPWrite(VidDecId,VPC_ACC_SAT_FP_REG,temp);
}
//******************************************************************************
//
// FUNCTION       : BYTE dev_vpx32xxGetVideoSaturation(BYTE VidDecId)
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
BYTE dev_vpx32xxGetVideoSaturation(BYTE VidDecId)
{
   return GVideoDecoder[VidDecId].Saturation;
}

//******************************************************************************
//
// FUNCTION       : void dev_vpx32xxSetVideoHue(BYTE VidDecId,BYTE hval)
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
void dev_vpx32xxSetVideoHue(BYTE VidDecId,BYTE hval)
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
   dev_vpx32xxFPWrite(VidDecId,VPC_TINT_FP_REG,temp);
}

//******************************************************************************
//
// FUNCTION       : BYTE dev_vpx32xxGetVideoHue(BYTE VidDecId)
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
BYTE dev_vpx32xxGetVideoHue(BYTE VidDecId)
{
   return GVideoDecoder[VidDecId].Hue;
}

//******************************************************************************
//
// FUNCTION       : void dev_vpx32xxSetVideoSharpness(BYTE VidDecId,BYTE sharpval)
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
void dev_vpx32xxSetVideoSharpness(BYTE VidDecId,BYTE sharpval)
{
   WORD temp, temp1;

   GVideoDecoder[VidDecId].Sharpness = sharpval;
   temp1 = (WORD)sharpval;
   temp1 = temp1 & 0x0007;
   temp = dev_vpx32xxFPRead(VidDecId,VPC_SCCT_FP_REG);
   temp = temp & 0xF8FF;
   temp = temp | (temp1<<8);
   dev_vpx32xxFPWrite(VidDecId,VPC_SCCT_FP_REG,temp);
   temp = dev_vpx32xxFPRead(VidDecId,VPC_SCMODE_FP_REG);
   temp &= 0x7ff;
   dev_vpx32xxFPWrite(VidDecId,VPC_SCMODE_FP_REG,temp);
   temp = dev_vpx32xxFPRead(VidDecId,VPC_COMB_UC_FP_REG);
   temp = temp & 0xF0FF;
   temp = temp | (temp1<<9);
   dev_vpx32xxFPWrite(VidDecId,VPC_COMB_UC_FP_REG,temp);
}

//******************************************************************************
//
// FUNCTION       : BYTE dev_vpx32xxGetVideoSharpness(BYTE VidDecId)
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
BYTE dev_vpx32xxGetVideoSharpness(BYTE VidDecId)
{
   return GVideoDecoder[VidDecId].Sharpness;
}

//????
//******************************************************************************
//
// FUNCTION       : void dev_vpx32xxPatGenControl(BYTE VidDecId,BYTE OnOff)
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
void dev_vpx32xxPatGenControl(BYTE VidDecId,BYTE OnOff)
{
   WORD Temp;
   Temp = dev_vpx32xx_WordRead(VidDecId,VPC_AVO_STOP_TWC_REG);
   if (OnOff)
   {
      Temp = Temp | 0x9800;
      dev_vpx32xx_WordWrite(VidDecId,VPC_AVO_STOP_TWC_REG,Temp);
   }
   else
   {
      Temp = Temp & ~(0x9800);
      dev_vpx32xx_WordWrite(VidDecId,VPC_AVO_STOP_TWC_REG,Temp);
   }
}
//******************************************************************************
//
// FUNCTION       : void dev_vpx32xxPowerUp(BYTE VidDecId)
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
#endif //#ifdef VPX_COLORS_USED ST
void dev_vpx32xxPowerUp(BYTE VidDecId)
{
   VidDecId++;							// Dummy
}

//******************************************************************************
//
// FUNCTION       : void dev_vpx32xxPowerDown(BYTE VidDecId)
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

void dev_vpx32xxPowerDown(BYTE VidDecId)
{
   VidDecId++;							// Dummy
}


//******************************************************************************
//
// FUNCTION       : void VpxInit (void)
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
void VpxInit (void)
{
#ifdef VPX322xF_DEV_ID
   //
   // I2C Writes
   //
   dev_vpx32xxByteWrite (VPX322xF_DEV_ID, 0xAA, 0x04); // Soft Reset
   dev_vpx32xxByteWrite (VPX322xF_DEV_ID, 0xAA, 0x00); // Out of power down
   dev_vpx32xxByteWrite (VPX322xF_DEV_ID, 0xB8, 0x00); // High mask clock run-in
   dev_vpx32xxByteWrite (VPX322xF_DEV_ID, 0xB9, 0x00); // Mid mask clock run-in
   dev_vpx32xxByteWrite (VPX322xF_DEV_ID, 0xBA, 0x00); // Low mask clock run-in
   dev_vpx32xxByteWrite (VPX322xF_DEV_ID, 0xBB, 0x27); // Framing Code for PAL WST/Teletext
   dev_vpx32xxByteWrite (VPX322xF_DEV_ID, 0xBC, 0x55); // Clock run-in high
   dev_vpx32xxByteWrite (VPX322xF_DEV_ID, 0xBD, 0x55); // Clock run-in low
   dev_vpx32xxByteWrite (VPX322xF_DEV_ID, 0xC0, 0x10); // Bit-Slicer level (default)
   dev_vpx32xxByteWrite (VPX322xF_DEV_ID, 0xC1, 0xBE); // Bit-Slicer frequency LSB
   dev_vpx32xxByteWrite (VPX322xF_DEV_ID, 0xC2, 0x0A); // Bit-Slicer frequency MSB + Phase inc. (default 702 for WST PAL)
   dev_vpx32xxByteWrite (VPX322xF_DEV_ID, 0xC5, 0x07); // Power On default value
   dev_vpx32xxByteWrite (VPX322xF_DEV_ID, 0xC6, 0x40); // Power On default value
   dev_vpx32xxByteWrite (VPX322xF_DEV_ID, 0xC7, 0x3D); // Disable soft error correction and one shot reset
   dev_vpx32xxByteWrite (VPX322xF_DEV_ID, 0xC8, 0x40); // Sync-Slicer level (default)
   dev_vpx32xxByteWrite (VPX322xF_DEV_ID, 0xC9, 0x19); // Enable PAL WST/Teletext, VPS and WSS
   dev_vpx32xxByteWrite (VPX322xF_DEV_ID, 0xCE, 0x15); // Bit Error tolerance
//	dev_vpx32xxByteWrite (VPX322xF_DEV_ID, 0xCF, 0xAA);
   dev_vpx32xxByteWrite (VPX322xF_DEV_ID, 0xCF, 0x2B); // Output all 43 Bytes *******
   dev_vpx32xxByteWrite (VPX322xF_DEV_ID, 0xD0, 0x00); // Power On default value
   dev_vpx32xxByteWrite (VPX322xF_DEV_ID, 0xD1, 0x00); // Power On default value
   dev_vpx32xxByteWrite (VPX322xF_DEV_ID, 0xD2, 0x80); // Power On default value
   dev_vpx32xxByteWrite (VPX322xF_DEV_ID, 0xD3, 0x00); // Power On default value
   dev_vpx32xxByteWrite (VPX322xF_DEV_ID, 0xD4, 0x3D); // Power On default value
   dev_vpx32xxByteWrite (VPX322xF_DEV_ID, 0xDC, 0x00); // Power On default value
   dev_vpx32xxByteWrite (VPX322xF_DEV_ID, 0xF2, 0x0F); // Enable Port A, Port B, PIXCLK Output, HREF, VREF, FIELD, VACT, LLC, LLC2
   dev_vpx32xxByteWrite (VPX322xF_DEV_ID, 0xF8, 0x24); // Set driver strengh of Port A, PIXCLK, LLC and VACT to 4
   //
   // FP Writes
   //
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x012,0x100); // Power On default value
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x014,0x027); // Power On default value
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x020,0x820); // Select PAL with 4H Comb-Mode and latch
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x021,0x660); // Input Selction: *Vin3* (for GNSS daughter board), Cin1, no IF Compensation, CBW=wide,
   // adaptive SECAM notch filter no luma lowpass filter, no change for hpll speed
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x022,0x000); // Power On default value
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x023,0x000); // Power On default value
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x028,0x0e7); // Notch Filter max. peaked, ddr="1", hdg="2", vdg="3", vpk="0"
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x02F,0x000); // Power On default value
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x030,0x47d); // Color Saturation = 1149 (from possible 0...4094)
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x036,0x1A5); // Power On default value
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x037,0x000); // Power On default value
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x038,0x00E); // Power On default value
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x039,0x01E); // Power On default value
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x03A,0x00A); // Power On default value
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x055,0x000); // Power On default value
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x0DC,0x000); // Power On default value
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x0F7,0xFFA); // Power On default value
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x0F8,0x530); // Power On default value
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x120,0x00C); // Power On default value
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x121,0x000); // Power On default value
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x122,0x000); // Power On default value
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x123,0x000); // Power On default value
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x124,0x2D0); //modified
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x125,0x2D0); //modified
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x126,0x200); // Power On default value
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x127,0x000); // Power On default value
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x128,0x020); // Power On default value
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x12A,0x016); // Vertical Begin (first active video line) at line 22
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x12B,0x120); // Number of input lines per field = 288
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x12C,0x120); // Number of output lines per field = 288
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x12D,0x000); // Horizontal Begin (relativ to npix) = 8
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x12E,0x2D0); // Horizontal length of Window (relativ to npix) = 721 Pixel ??????????????
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x12F,0x2D0); // Number of active Pixel for the full active line = 730 Pixel ?????????????
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x130,0x200); // Power On default value
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x131,0x000); // Power On default value
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x132,0x020); // Power On default value

   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x134,0x13E); // start VBI even Field
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x135,0x14F); // end VBI even Field
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x136,0x006); // start VBI odd Field = line 6
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x137,0x016); // end VBI odd Field = line 23
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x138,0x803); // enable VBI Window, enable sliced, mark VBI-lines as ACTIVE, latch Window
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x139,0x000); // Power On default value
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x140,0x060); // Control and Latch: H/V-Sync are tracking the Inputsignal,
   // VACT-length corresponds to the size of current window,
   // Latch Window #1 and #2 automatically
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x150,0x351); // Output as 4:2:2 ITU-656 ????????????????????????????????
   // SUPPRESS HEADERS for blank lines
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x151,0x028); // Power On default value
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x152,0x2D0); // Power On default value
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x153,0x000); // Power On default value
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x154,0x200); // Output Multiplexer: "VBI active" is connected to TDO pin
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x157,0xBB8); // Power On default value
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x158,0x000); // Power On default value
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x15E,0x000); // Power On default value
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x171,0x005); // Power On default value
   dev_vpx32xxFPWrite (VPX322xF_DEV_ID,0x172,0x011); // Power On default value
   msg_vdd ("VPX Pal",0);
#endif
}


#endif
//**********************************  END  *************************************
