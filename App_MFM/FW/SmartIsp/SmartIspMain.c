/*
	$Workfile:   SmartIspMain.c  $
	$Revision: 1.30 $
	$Date	 :     $
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
// MODULE:      SmartIspMain.c
//
// USAGE:       This module is to support In System Programming in Application
//              Running mode. Currenly support chisp device is Mars only.
//
//******************************************************************************


//Revision: 4.2      2012-May-25
//Note: 2.1 -> 2.2 : No change to SmartIspMain,
//                   but update SmartIsp Lib to v2.2 for enabling SRWD and BPx
//                   in flash status register after FlashWr is done
//Note: 2.2 -> 2.3 : Add SmartIsp Auto Detection function.
//                   Need to cooperate with SmartIsp Utility v0.7 or later.
//                   No Change to SmartIsp Lib (Remaining v2.2 Lib)
//Note: 2.3 -> 2.4 : Support MainChannel is CH_B (PIP_BUF)..
//                   Assign FmBinBufStart in Application layer for 4M/2M flash size.
//                   Eliminate screen flashing while FRC_WBUF shifting down.
//                   Need to cooperate with SmartIsp Utility v0.9 or later.
//                   No Change to SmartIsp Lib (Remaining v2.2 Lib)
//Note: 3.0        : Athena Supported. (Without DualFlash Support)
//Note: 3.1        : 24bit/30bit memorybpp supported.
//Note: 3.2        : Add SDRAM_MEM_SIZE to IspInfo Structure for Athena-C support
//Note: 4.0        : Move Flash Driver to App layer
//Note: 4.1        : Dual Flash Support
//Note: 4.2        : Modified DUALFLASH_ENABLE_PATCH_MODE to match "Dual Firmware Images Support_v2.doc"


//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#include "System\All.h"

//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************
#ifdef SMART_ISP

#define DEBUG
#define DEBUG_ISPAUTO

#ifdef DEBUG
#define  msg(a)             		gm_Print(a,0)
#define  msg_i(a,b)         		gm_Print(a,b)
#define  msg_ii(a,b,c)         	gm_Printf(a,b,c)
#define  msg_iii(a,b,c,d)        gm_Printf(a,b,c,d)
#else
#define  msg(a)
#define  msg_i(a,b)
#define  msg_ii(a,b,c)
#define  msg_iii(a,b,c,d)
#endif

#ifdef DEBUG_EXT
#define  msgx(a)             		gm_Print(a,0)
#define  msgx_i(a,b)         		gm_Printf(a,b)
#define  msgx_ii(a,b,c)         	gm_Printf(a,b,c)
#define  msgx_iii(a,b,c,d)        gm_Printf(a,b,c,d)
#else
#define  msgx(a)
#define  msgx_i(a,b)
#define  msgx_ii(a,b,c)
#define  msgx_iii(a,b,c,d)
#endif


#ifdef DEBUG_ISPAUTO
#define  IspAuto_msg(a)             		gm_Print(a,0)
#define  IspAuto_msgx_i(a,b)         		gm_Print(a,b)
#define  IspAuto_msg_ii(a,b,c)         	gm_Printf(a,b,c)
#define  IspAuto_msg_iii(a,b,c,d)        gm_Printf(a,b,c,d)
#define  IspAuto_msg_iiii(a,b,c,d,e)        gm_Printf(a,b,c,d,e)
#else
#define  IspAuto_msg(a)
#define  IspAuto_msg_i(a,b)
#define  IspAuto_msg_ii(a,b,c)
#define  IspAuto_msg_iii(a,b,c,d)
#define  IspAuto_msg_iiii(a,b,c,d,e)
#endif


#define VER_LEN		4		//Length of f/w version, please refer to C0Starup.asm
#ifdef ROM_SIZE_4MB
#define DEFAULT_FWSIZE		0x400400L	// 4 MBytes + 1K
#else  // ROM_SIZE_2MB
#define DEFAULT_FWSIZE		0x200400L	// 2 MBytes + 1K
#endif

//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************

//******************************************************************************
//  G L O B A L    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
#ifdef ST_4K2K_93xx_BOARD
void gm_4K2K_MainCapture(void);
#endif

//******************************************************************************
//  S T A T I C    V A R I A B L E S
//******************************************************************************
static BYTE IspStep=ISP_STEP_NONE;
static BYTE ISPStepStatus=ISP_SSTATUS_PROCESSING;
static BYTE LastWrPercent;
static BYTE ActionConfirm;
static BOOL b_IsIspAutoMode=FALSE;
//static BOOL b_Isp8bitsMemRequest=FALSE;
#ifdef SMART_ISP_AUTO_DETECTION
#define PATTERN_LENGTH					15
#define PREFIX_NUM_MAX					24
#define SMART_ISP_DETECTION_INTERVAL	30	//x 100ms
#define V_BOARDER_TOP					75
#define V_BOARDER_BOT					25
#define V_BOARDER						(V_BOARDER_TOP + V_BOARDER_BOT)
#define H_BOARDER_L						4
static BYTE b_IspDetSts=ISP_DETHDLR_STOP;
static ROM PixRGB_Struct PrefixPattern   ={0x30,0x10,0x30};
static ROM PixRGB_Struct PrefixPatternEnd={0x40,0xD0,0x20};
static ROM PixRGB_Struct IspDetPattern[PATTERN_LENGTH]=
{
   {0x20,0x20,0x40},
   {0x60,0x00,0xE0},
   {0x80,0xB0,0x90},
   {0x30,0xF0,0x20},
   {0xA0,0x60,0x50},
   {0xC0,0xB0,0xD0},
   {0xF0,0x90,0x90},
   {0xB0,0xD0,0x20},
   {0x30,0x70,0x60},
   {0x60,0xE0,0x80},
   {0x00,0x10,0xA0},
   {0x50,0x90,0xF0},
   {0x10,0x30,0x50},
   {0xE0,0xE0,0xC0},
   {0xA0,0x90,0xA0},
};
#endif

static ROM WORD RegFrcCtrl[CH_PHY_NUM]={MFRC_CTRL,PFRC_CTRL};
static ROM WORD RegFrcWbufStart[CH_PHY_NUM]={MFRC_WBUF_START_hi,PFRC_WBUF_START_hi};
static ROM WORD RegFrcRbufStart[CH_PHY_NUM]={MFRC_RBUF_START_hi,PFRC_R0_RBUF_START_hi};
static ROM WORD MaskOfNumBuf[CH_PHY_NUM]={MFRC_NUMBUF_MINUS_ONE,PFRC_NUMBUF_MINUS_ONE};
static ROM WORD RegFrcBufStride[CH_PHY_NUM]={MFRC_BUF_STRIDE_hi,PFRC_BUF_STRIDE_hi};


//******************************************************************************
//  S T A T I C    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
static void DeliverIspEvent2OSD(void);
static void OsdWirtePercentage(DWORD sizeTotal, DWORD sizeWrited);

//******************************************************************************
//  C O D E
//******************************************************************************
#ifdef ST_4K2K_93xx_BOARD
static BYTE SlaveControl=ISP_SLAVE_CMD_NONE;
void SmartIspSlaveControl(BYTE Control)
{
   SlaveControl=Control;
}

void SmartIspSlaveHandler(void)
{
   if(SlaveControl & ISP_SLAVE_CMD_RUN_DIRECT)
   {
      IspThruDisplay(ISP_EXECUTE_DIRECT);
   }
   else if(SlaveControl & ISP_SLAVE_CMD_RUN_KEYPAD)
   {
      IspThruDisplay(ISP_EXECUTE_KEYPAD);
   }
   else if(SlaveControl & ISP_SLAVE_CMD_HARD_RESET)
   {
      SysteHardReset();
   }
   SlaveControl= ISP_SLAVE_CMD_NONE;
}
#endif



static void DeliverIspEvent2OSD(void)
{
#ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()!=CHIPID_4K2K_R)
      return;
#endif
   if (!gmvb_DebugOnly)
   {
      OSDRequestMsg(gmvb_MainChannel, MTO_IspStatusChanged, REQUEST_NOW);
      OsdEventHandler();
      while (SchedulerFind(SchedulerOsdPack[gmvb_MainChannel], REQ_OSD_MSG, MTO_IspStatusChanged)!= NO_INDEX)
         OsdEventHandler();
   }
}
//******************************************************************************
//
// FUNCTION     :   void IspCallBack_OsdWirtePercentage(void)
// USAGE        :   This a callback function for SmartIspLib usage.
//                  IspLib call this function during FlashWrite period to report
//                  homw many bytes are written, so that the function can convert
//                  it into percentage for OSD indication.
// INPUT        :   sizeTotal - Total bystes flash image.
//                  sizeWrited - how many bytes were written.
// OUTPUT       :   None
// GLOBALS      :   None
// USED_REGS    :   None
//
//******************************************************************************
void IspCallBack_OsdWirtePercentage(DWORD sizeTotal, DWORD sizeWrited)
{
   sizeTotal=sizeTotal>>13;
   sizeWrited=sizeWrited>>13;
   if (sizeTotal==0)
      UserPrefIspFlashWrPercent=0;
   else
   {
      UserPrefIspFlashWrPercent=(BYTE) ((WORD)(sizeWrited*100)/(WORD)sizeTotal);
   }
   if (UserPrefIspFlashWrPercent!=LastWrPercent)
   {
      //gm_Printf("%d",UserPrefIspFlashWrPercent);
      DeliverIspEvent2OSD();
      LastWrPercent=UserPrefIspFlashWrPercent;
   }
}

void IspForceBkGndControl(BOOL Enable)
{
   static BOOL en = FALSE;
   static WORD wPB_CTRL;

   if (Enable)
   {
      wPB_CTRL = gm_ReadRegWord(PB_CTRL);

      en = TRUE;

      gm_DisplayForceBkgnd(CH_A, TRUE, 0);
      gm_DisplayForceBkgnd(CH_B, TRUE, 0);
   }
   else if (en)
   {
      en = FALSE;

      if (wPB_CTRL && PB_FORCE_MAIN_BKGND)
      {
         gm_DisplayForceBkgnd(CH_A, FALSE, 0);
      }

      if (wPB_CTRL && PB_FORCE_PIP_BKGND)
      {
         gm_DisplayForceBkgnd(CH_B, FALSE, 0);
      }
   }
}

//******************************************************************************
//
// FUNCTION     :   BOOL IspThruDisplay(void)
// USAGE        :   To perfrom In System Programming during application running
//                  mode.
// INPUT        :   TRUE - Direct perform Flash Erase/Write without confirmation
//                  FALSE - Need confirmation before perform Flash Erase/Write
// OUTPUT       :   TRUE/FALSE
// GLOBALS      :   None
// USED_REGS    :   None
//
//******************************************************************************
BOOL IspThruDisplay(BYTE ExeMode)
{
   BYTE fBuf[PIX_BUF_SIZE+22];
   BYTE PLbuf[BIN_BUF_SIZE+2];
   IspInfoStruct IspInfo;
   WORD i,j,r;
   BYTE ret;
#ifdef ST_4K2K_93xx_BOARD
   BYTE SocIPCmd = ISP_SLAVE_CMD_NONE;
#endif   

#ifdef DEBUG
   WORD time1,time2;
   time1=(WORD)(gm_GetSystemTime());
#endif

#ifdef ST_4K2K_93xx_BOARD

   if(ExeMode == ISP_EXECUTE_KEYPAD)
      SocIPCmd = ISP_SLAVE_CMD_RUN_KEYPAD;
   else
      SocIPCmd = ISP_SLAVE_CMD_RUN_DIRECT;
      
   if(GetChipID_4K2K()==CHIPID_4K2K_R)
   {
      #if ENABLE_SOC_IPC_BUSY_CHECK
      /*ChengLin 20130124: New 4k2k board BUSY status check*/
      if(!SocIPCIsSlaveLeftReady())
      {
         msg("SlaveLeft Not Ready!");
         return FALSE;  
      }

      if(!SocIPCIsSlaveFEReady())
      {
         msg("SlaveFe Not Ready!");
         return FALSE;  
      }
      #endif	
      SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_SMARTISP_CTRL, &SocIPCmd);	//To Athena_L	    
      SocIPCSendCmdPacketParser(SOCIPC_DEVICEFE_DST_ADDRESS, SICMD_RW_SMARTISP_CTRL, &SocIPCmd);	//To Ahtnea_FE	          
   }  

   if(GetChipID_4K2K()!=CHIPID_4K2K_FE)
   {
      UserPrefCaptureM = CAPTURE_MODE_HALF_HORIZONTAL;
      gm_4K2K_MainCapture();
      if(GetChipID_4K2K()==CHIPID_4K2K_L)   
         gm_WriteRegWord(MFRC_PIXELS_OUT,gm_ReadRegWord(MFRC_PIXELS_OUT)-8);

//      gm_ForceUpdate(gmvb_MainChannel,OUTPUT_CLOCK);
   }
#endif      
   
   //IspForceBkGndControl(TRUE);

   //ExeMode = ISP_Mode_Flag;
   //ISP_Mode_Flag = 0;
   msg_i("IspExeMode (%d)",ExeMode);

   Print_SmartIspLibVersion();

   #ifdef ST_4K2K_93xx_BOARD
   if ((ExeMode==ISP_EXECUTE_DIRECT) || (GetChipID_4K2K()!=CHIPID_4K2K_R))
   #else
   if (ExeMode==ISP_EXECUTE_DIRECT)
   #endif
      IspInfo.OsdWirtePercentage=NULL_PTR;
   else
      IspInfo.OsdWirtePercentage=IspCallBack_OsdWirtePercentage;

   IspInfo.FrameWidth =stInput[gmvb_MainChannel].Hactive;
   IspInfo.FrameHeight=stInput[gmvb_MainChannel].Vactive;
   IspInfo.BurstAdjust=(BYTE)((BURST_SIZE-(IspInfo.FrameWidth*gmvb_MemoryBpp[gmvb_MainChannel]/BIT_PER_BYTE)%BURST_SIZE)& 0x3F);
   IspInfo.BitsPerPix =gmvb_MemoryBpp[gmvb_MainChannel];
   IspInfo.SdramSize  = SDRAM_MEM_SIZE;
   if (gmvb_MainChannel == CH_A)
      IspInfo.FrameBufStart=gm_MemoryGetInfo(BUF_MAIN, BUF_START_ADDRESS)* BURST_SIZE;
   else
      IspInfo.FrameBufStart=gm_MemoryGetInfo(BUF_PIP, BUF_START_ADDRESS)* BURST_SIZE;

   gm_SetRegBitsWord(HOST_CONTROL,IMP_FORCE_UPDATE|ODP_FORCE_UPDATE);

   msg_i ("Main_Channel  : %d",gmvb_MainChannel);
   msg_i ("MemoryNumBuf  : %d",gmvb_MemoryNumBuffers[gmvb_MainChannel]);
   msg_i ("MemoryBpp     : %d",gmvb_MemoryBpp[gmvb_MainChannel]);
   msg_ii("FrameBufStart : 0x%lX (0x%lX)",IspInfo.FrameBufStart, IspInfo.FrameBufStart >> BURST_SIZE_SHIFT);
   msg_ii("Input Active  : %d x %d", IspInfo.FrameWidth, IspInfo.FrameHeight);
   msg_i ("BurstAdjust   : %d",IspInfo.BurstAdjust);
   msg_i ("BitsPerPix    : %d",IspInfo.BitsPerPix);

   IspStep=ISP_STEP_INIT;
   ISPStepStatus=ISP_SSTATUS_NONE;
   //Step 0: Initial and Condition Checking
   DeliverIspEvent2OSD();

   //It's application layer's responsibilty to assign a spare space (about 2M byte) for the extracted f/w binary storing.
   //If FmBinBufStart=NULL_PTR, SmartIspLib will locate it in address 62M (64M-(2M+1K)).
   //IspInfo.FmBinBufStart=NULL_PTR;
   IspInfo.FmBinBufStart=SDRAM_MEM_SIZE*BURST_SIZE-DEFAULT_FWSIZE;


   IspStep=ISP_STEP_SEARCH_PIC;
   DeliverIspEvent2OSD();
   Isp_FrameCatch(&IspInfo, FCATCH_START);
   //msg_i("SystemTime0:%ld",gm_GetSystemTime());


   //Step 1: Search HexPicture Signature
   //Step 2: Extract Pic Width and Height
   if (Isp_PicSearch(&IspInfo, fBuf)!=TRUE)
   {
      goto Failed_Exit;
   }



   IspStep=ISP_STEP_EXTRACT_PICINFO;
   DeliverIspEvent2OSD();


   //Step 3: Extract Pic Line0 Information
   if (Isp_ExtractPicL0(&IspInfo, fBuf, PLbuf)==FALSE)
      goto Failed_Exit;
   msg_ii("Input Picture : %d x %d", IspInfo.PicWidth, IspInfo.PicHeight);
   msg_i ("Total PicNum  : %d", IspInfo.TotalPicNum);
   msg_i ("FwBinTotal    : 0x%lX",IspInfo.FwBinTotal);


   //Step 4: Extract HexBinary from Pic L1 to Ln and Store to another SDRAM space
   IspStep=ISP_STEP_EXTRACT_HEXBINARY;
   DeliverIspEvent2OSD();
   if (Isp_ExtractBin(&IspInfo, fBuf, PLbuf)==FALSE)
      goto Failed_Exit;
   msg_ii("FmBinBufStart : 0x%lX (0x%lX)",IspInfo.FmBinBufStart, IspInfo.FmBinBufStart >> BURST_SIZE_SHIFT);

   //Step 5: Verify whole HexBinary Checksum
   IspStep=ISP_STEP_VERIFY_CHECKSUM;
   DeliverIspEvent2OSD();
   if (Isp_VerifyBin(&IspInfo, PLbuf)==FALSE)
      goto Failed_Exit;


   //Step 6: Verify Binary Signature and Get Firmware Version
   //		  If return true, new version string will be located in PLbuf
   IspStep=ISP_STEP_VERIFY_SIGNATURE;
   DeliverIspEvent2OSD();
   if (Isp_VerifySig_GetVer(&IspInfo, PLbuf, VER_LEN)==FALSE)
      goto Failed_Exit;

   UserPrefInFwMajor=PLbuf[0];
   UserPrefInFwMinor=PLbuf[1];
   UserPrefInFwRc=PLbuf[2];

#ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()==CHIPID_4K2K_R)
   {
      if (ExeMode==ISP_EXECUTE_KEYPAD)
      {
         BYTE L_Verified=FALSE;
         BYTE FE_Verified=FALSE;
         gm_TimerStart(SMART_ISP_SLAVE_COMM_1s_TMR,10);
         while(gm_TimerCheck(SMART_ISP_SLAVE_COMM_1s_TMR)==TIMER_OK)  //Runing
         {
            msg("Get L ISP_Status...");
            if(SocIPCGetSlaveSmartIspStatus(SOCIPC_DEVICEL_DST_ADDRESS)==TRUE)
            {
               msg_ii("L_Step=%d    Status=%d",gAthena_LEFT_FE_Slave_Parameters.SmartIspStep,gAthena_LEFT_FE_Slave_Parameters.SmartIspStatus);
               if(gAthena_LEFT_FE_Slave_Parameters.SmartIspStep==ISP_STEP_GET_CONFIRMATION)
               {
                  L_Verified=TRUE;
               }
               else
               {
                  msg("L IMAGE Validation failed!");

               }
               break;
            }
            gm_Delay100ms(10);
         }

         if(L_Verified==FALSE)
            goto Failed_Exit;         

         gm_TimerStart(SMART_ISP_SLAVE_COMM_1s_TMR,2);
         while(gm_TimerCheck(SMART_ISP_SLAVE_COMM_1s_TMR)==TIMER_OK)  //Runing
         {
            msg("Get FE ISP_Status...");
            if(SocIPCGetSlaveSmartIspStatus(SOCIPC_DEVICEFE_DST_ADDRESS)==TRUE)
            {
               msg_ii("FE_Step=%d    Status=%d",gAthena_LEFT_FE_Slave_Parameters.SmartIspStep,gAthena_LEFT_FE_Slave_Parameters.SmartIspStatus);
               if(gAthena_LEFT_FE_Slave_Parameters.SmartIspStep==ISP_STEP_GET_CONFIRMATION)
               {
                  FE_Verified=TRUE;
               }
               else
               {
                  msg("FE IMAGE Validation failed!");
               }
               break;
            }
            gm_Delay100ms(10);
         }

         if(FE_Verified==FALSE)
            goto Failed_Exit;         
      }
   }
#endif

   //Step 7: Get confirmation to perfrom Flash Erase/Write
   ActionConfirm=ISP_ACTION_UNCONFIRMED;
   IspStep=ISP_STEP_GET_CONFIRMATION;
   DeliverIspEvent2OSD();
   #ifdef ST_4K2K_93xx_BOARD
   if ((ExeMode==ISP_EXECUTE_KEYPAD) && (GetChipID_4K2K()==CHIPID_4K2K_R))
   #else
   if (ExeMode==ISP_EXECUTE_KEYPAD)
   #endif
   {
      //ActionConfirm=ISP_ACTION_UNCONFIRMED;
      msg("Waiting for confirmation..");
      //gm_TimerStart(ISP_WAIT_CONFIRM_1s_TMR, 35);	  //Waiting for 35 sec since OSD timer is 30 second
      while (ActionConfirm==ISP_ACTION_UNCONFIRMED)
      {
         DeliverIspEvent2OSD();
      }

      if (ActionConfirm!=ISP_ACTION_CONFIRMED)
      {
         #ifdef ST_4K2K_93xx_BOARD
         if(GetChipID_4K2K()==CHIPID_4K2K_R)
         {
            SocIPCmd = ISP_SLAVE_CMD_ERASE_REJECT;
            SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_SMARTISP_CTRL, &SocIPCmd);  //To Athena_L       
            SocIPCSendCmdPacketParser(SOCIPC_DEVICEFE_DST_ADDRESS, SICMD_RW_SMARTISP_CTRL, &SocIPCmd);  //To Athena_FE                   
         }
         #endif
         msg("ISP Action Rejected!");
         goto Failed_Exit;
      }
   }

	FLASH_WP_DISABLE();
	gm_WriteRegWord(GPIO_5_ENABLE,0x0300);
	gm_WriteRegWord(GPIO_5_DIRCTRL,0x0300);
	gm_WriteRegWord(GPIO_5_OPENDRAIN_EN,0);
	gm_WriteRegWord(GPOUTPUT5,0x0300);

#ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()==CHIPID_4K2K_R)
   {
      SocIPCmd = ISP_SLAVE_CMD_ERASE_START;
      
      if (ExeMode!=ISP_EXECUTE_KEYPAD)
         gm_Delay100ms(20);   
         
      msg("R_ISP Send CMD to Slave: Erase confirmed");
      SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_SMARTISP_CTRL, &SocIPCmd);	//To Athena_L	     
      SocIPCSendCmdPacketParser(SOCIPC_DEVICEFE_DST_ADDRESS, SICMD_RW_SMARTISP_CTRL, &SocIPCmd);	//To Athena_FE	           
   }


   if((GetChipID_4K2K()!=CHIPID_4K2K_R) && (ExeMode==ISP_EXECUTE_KEYPAD))
   {
      msg("FE_L_ISP Waiting for Confirmation...");
      gm_TimerStart(SMART_ISP_SLAVE_COMM_1s_TMR,30);
      while(gm_TimerCheck(SMART_ISP_SLAVE_COMM_1s_TMR)==TIMER_OK)
      {
         Ddc2biCmdHandler();
         if(SlaveControl & (ISP_SLAVE_CMD_ERASE_REJECT|ISP_SLAVE_CMD_ERASE_START))
         {
            break;
         }
         
      }
      if(SlaveControl & ISP_SLAVE_CMD_ERASE_START)
      {
         msg("FE_L_ISP Action Erase Start!");
      }
      else if(SlaveControl & ISP_SLAVE_CMD_ERASE_REJECT)
      {
         msg("FE_L_ISP Action Rejected!");
         goto Failed_Exit;
      }
      else
      {
         msg("FE_L_ISP TimeOut: Not Get Confirmation!");
         goto Failed_Exit;
      }
   }
#endif

//   msg("ISP temporary EXIT!");
//   goto Failed_Exit;

   IspStep=ISP_STEP_FLASH_ERASE;
   DeliverIspEvent2OSD();

   {
      SmartIspFlashDriver FlashDrv;
      FlashDrv.FlashInit   = SerialFlashInit;
      FlashDrv.FlashErSectr= Flash_EraseSector;
      FlashDrv.WrBuf2Flash = WriteBuf2Flash;
      FlashDrv.GetFlashInfo= GetSpiFlashInfo;
      if (Isp_LoadFlashDrv(&FlashDrv)==FALSE)
      {
         msg("Load Flash Driver Failed!"); 
         goto Failed_Exit;
      }
   }

   //Step 8: Flash Erase
   {
      BYTE b_DualMode=SISP_DUALFLASH_DISABLE;
      
      #ifdef SMART_ISP_DUAL_FLASH
      {
         FlashInfo_t FlashInfo;
         GetSpiFlashInfo(&FlashInfo);
         #if 1 //Match for Dual Firmware Images Support_v2.doc
            b_DualMode=SISP_DUALFLASH_ENABLE_PATCH_MODE;
         #else
         if(FlashInfo.DW_Size!=0x800000UL)
            b_DualMode=SISP_DUALFLASH_ENABLE_PATCH_MODE;
         else
            b_DualMode=SISP_DUALFLASH_ENABLE_NORMAL;
         #endif
      }
      #endif
      if (Isp_FlashErase(0,0,SECT_ER_DEST_EXCLUDED, b_DualMode)==FALSE) 
      {
         msg("Flash Erase Failed!");
         goto Failed_Exit;
      }
   }
   msg("Flash Erase Success");
  // goto Failed_Exit; 
   IspStep=ISP_STEP_FLASH_WRITE;
   UserPrefIspFlashWrPercent=0;
   DeliverIspEvent2OSD();


   //Step 9: Write Flash
   LastWrPercent=0xFF;
   ISPStepStatus=ISP_SSTATUS_PROCESSING;
   if (Isp_FlashWrite(&IspInfo, PLbuf)==FALSE)
   {
      msg("Flash Write Failed!");
      goto Failed_Exit;
   }

   EnableWriteProtection();

	FLASH_WP_ENABLE();

   UserPrefIspFlashWrPercent=100;
   IspStep=ISP_STEP_FINISHED;
   ISPStepStatus=ISP_SSTATUS_SUCCESS;
   ret=TRUE;
   goto Isp_Exit;

Failed_Exit:
   ret=FALSE;
   ISPStepStatus=ISP_SSTATUS_FAILED;

#ifdef ST_4K2K_93xx_BOARD
   if(ExeMode==ISP_EXECUTE_KEYPAD)
   {
      if(GetChipID_4K2K()==CHIPID_4K2K_R)
      {
         SocIPCmd = ISP_SLAVE_CMD_FAIL_EXIT;
         SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_SMARTISP_CTRL, &SocIPCmd);	//To Athena_L	     
         SocIPCSendCmdPacketParser(SOCIPC_DEVICEFE_DST_ADDRESS, SICMD_RW_SMARTISP_CTRL, &SocIPCmd);	//To Athena_FE
      }
      if(GetChipID_4K2K()!=CHIPID_4K2K_R)
      {
         msg("FE_L_ISP Waiting for Fail Exit...");
         gm_TimerStart(SMART_ISP_SLAVE_COMM_1s_TMR,30);
         while(gm_TimerCheck(SMART_ISP_SLAVE_COMM_1s_TMR)==TIMER_OK)
         {
            Ddc2biCmdHandler();
            if(SlaveControl & (ISP_SLAVE_CMD_FAIL_EXIT))
            {
               break;
            }
         }
         msg("FE_L_ISP Fail-Exit!");
      }
      goto Isp_Exit2;
   }
#endif   
   //IspForceBkGndControl(FALSE);
   DeliverIspEvent2OSD();
	UserPrefStartSmartISP = gmd_FALSE;

Isp_Exit:

#ifdef ST_4K2K_93xx_BOARD
   if (ExeMode==ISP_EXECUTE_KEYPAD)
   {
      if(GetChipID_4K2K()==CHIPID_4K2K_R)
      {
         BYTE L_Finished=FALSE;
         BYTE FE_Finished=FALSE;
         gm_TimerStart(SMART_ISP_SLAVE_COMM_1s_TMR,15);
         while((gm_TimerCheck(SMART_ISP_SLAVE_COMM_1s_TMR)==TIMER_OK) && ((L_Finished==FALSE) || (FE_Finished==FALSE)))  //Runing
         {
            msg("Get L ISP_Status...");
            if(SocIPCGetSlaveSmartIspStatus(SOCIPC_DEVICEL_DST_ADDRESS)==TRUE)
            {
               msg_ii("L_Step=%d    Status=%d",gAthena_LEFT_FE_Slave_Parameters.SmartIspStep,gAthena_LEFT_FE_Slave_Parameters.SmartIspStatus);
               if(gAthena_LEFT_FE_Slave_Parameters.SmartIspStep==ISP_STEP_FINISHED)
               {
                  L_Finished=TRUE;
               }
//               else
//               {
//                  msg("L ISP failed!");
//               }
            }

            msg("Get FE ISP_Status...");
            if(SocIPCGetSlaveSmartIspStatus(SOCIPC_DEVICEFE_DST_ADDRESS)==TRUE)
            {
               msg_ii("FE_Step=%d    Status=%d",gAthena_LEFT_FE_Slave_Parameters.SmartIspStep,gAthena_LEFT_FE_Slave_Parameters.SmartIspStatus);
               if(gAthena_LEFT_FE_Slave_Parameters.SmartIspStep==ISP_STEP_FINISHED)
               {
                  FE_Finished=TRUE;
               }
//               else
//               {
//                  msg("FE ISP failed!");
//               }
            }
            gm_Delay100ms(10);
         }
         SocIPCmd = ISP_SLAVE_CMD_NORMAL_EXIT;
         SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_SMARTISP_CTRL, &SocIPCmd);	//To Athena_L	     
         SocIPCSendCmdPacketParser(SOCIPC_DEVICEFE_DST_ADDRESS, SICMD_RW_SMARTISP_CTRL, &SocIPCmd);	//To Athena_FE
      }
      else
      {
         msg("FE_L_ISP Waiting for EXIT...");
         gm_TimerStart(SMART_ISP_SLAVE_COMM_1s_TMR,15);
         while(gm_TimerCheck(SMART_ISP_SLAVE_COMM_1s_TMR)==TIMER_OK)
         {
            Ddc2biCmdHandler();
            if(SlaveControl & (ISP_SLAVE_CMD_NORMAL_EXIT|ISP_SLAVE_CMD_FAIL_EXIT))
            {
               break;
            }
            
         }

      }
   }

Isp_Exit2:

   if(GetChipID_4K2K()==CHIPID_4K2K_R )
   {
      UserPrefCaptureM = CAPTURE_MODE_RIGHT;
      gm_4K2K_MainCapture();
   }
   else if(GetChipID_4K2K()==CHIPID_4K2K_L)   
   {
      UserPrefCaptureM = CAPTURE_MODE_LEFT;
      gm_4K2K_MainCapture();
//      gm_WriteRegWord(MFRC_PIXELS_OUT,gm_ReadRegWord(MFRC_PIXELS_OUT)-8);
   }

//      gm_ForceUpdate(gmvb_MainChannel,OUTPUT_CLOCK);
#endif   

   Isp_FrameCatch(&IspInfo, FCATCH_END);

   if(ActionConfirm!=ISP_ACTION_REJECTED)
   {
   DeliverIspEvent2OSD();
   }
//   else
//      gm_Print("@@@Bypass DeliverIsp",0);
	UserPrefStartSmartISP = gmd_FALSE;
   IspStep=ISP_STEP_NONE;
   ISPStepStatus=ISP_SSTATUS_NONE;
   b_IsIspAutoMode=FALSE;
#ifdef DEBUG
   time2=(WORD)(gm_GetSystemTime());
   msg_i("$$$Total Time: %dms $$$",time2-time1);
#endif
   return ret;
}


BYTE GetIspStep(void)
{
   return IspStep;
}

BYTE GetIspStepStatus(void)
{
   return ISPStepStatus;
}

void SetIspActionConfirm(BYTE answer)
{
   if (answer==ISP_ACTION_CONFIRMED)
      ActionConfirm=answer;
   else
      ActionConfirm=ISP_ACTION_REJECTED;
}


BYTE GetIspActionConfirm(void)
{
   return ActionConfirm;
}

void SysteHardReset(void)
{
#ifdef ST_4K2K_93xx_BOARD
   #if 1
      return;
   #else
   if(GetChipID_4K2K()==CHIPID_4K2K_R)
   {
      BYTE SocIPCmd = ISP_SLAVE_CMD_HARD_RESET;
      gm_Delay100ms(10);
      SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_SMARTISP_CTRL, &SocIPCmd); //To Athena_L       
      SocIPCSendCmdPacketParser(SOCIPC_DEVICEFE_DST_ADDRESS, SICMD_RW_SMARTISP_CTRL, &SocIPCmd);   //To Athena_FE
   }
   gm_Delay100ms(10);
   #endif
#else
   #ifdef SMART_ISP_AUTO_DETECTION
   gm_Delay100ms(15);
   #endif
   gm_WriteRegWord(OCM_NR_SCRATCHPAD, 0);
   gm_ClearRegBitsWord(CLOCK_CONFIG1,OCM_CLK_SEL);
   gm_WriteRegWord(HARD_RESETS_2, 0xFFFF);
   gm_WriteRegWord(AUDIO_HARD_RESETS, 0xFFFF);
   //Reset LVDS to avoid DCLK from PLL is gated off inside LVDS loop that kill ODP source clock.
   gm_WriteRegWord(LVDS_RESET_CTRL, LVDSTX_HARD_RESET);
   gm_WriteRegWord(HARD_RESETS  , 0xFFFF);
#endif  
}

DWORD Isp_FrameCatch(IspInfoStruct* InfoPtr, BYTE Cmd)
{
   static DWORD Bkup_WbufStart, Bkup_WbufEnd;
   static WORD  Bkup_FrcControl;
   DWORD  BufStride=gm_ReadRegDWord_BE(RegFrcBufStride[gmvb_MainChannel]);

   InfoPtr=InfoPtr; //Avoid compiler warning

   switch (Cmd)
   {
      case FCATCH_START:
         Bkup_FrcControl=gm_ReadRegWord(RegFrcCtrl[gmvb_MainChannel]);
         Bkup_WbufStart=gm_ReadRegDWord_BE(RegFrcWbufStart[gmvb_MainChannel]);
         Bkup_WbufEnd=gm_ReadRegDWord_BE(RegFrcWbufStart[gmvb_MainChannel]+4);

         msgx_i("BkupFrcControl: 0x%X",Bkup_FrcControl);
         msgx_i("BkupWbufStart : 0x%lX",Bkup_WbufStart);
         msgx_i("BkupWbufEnd   : 0x%lX",Bkup_WbufEnd);
         msgx_i("BkupBufStride : 0x%lX",BufStride);

         gm_WriteRegWord(RegFrcCtrl[gmvb_MainChannel],Bkup_FrcControl & (~MaskOfNumBuf[gmvb_MainChannel]));
         msgx_i("NewFrcControl : 0x%X",gm_ReadRegWord(RegFrcCtrl[gmvb_MainChannel]));

         gm_WriteRegDWord_BE(RegFrcWbufStart[gmvb_MainChannel],Bkup_WbufStart + BufStride);
         gm_WriteRegDWord_BE(RegFrcWbufStart[gmvb_MainChannel]+4,Bkup_WbufEnd + BufStride);
         gm_WriteRegDWord_BE(RegFrcRbufStart[gmvb_MainChannel],Bkup_WbufStart + BufStride);
         gm_WriteRegDWord_BE(RegFrcRbufStart[gmvb_MainChannel]+4,Bkup_WbufEnd + BufStride);
         msgx_i("NewWbufStart  : 0x%lX",Bkup_WbufStart + BufStride);
         msgx_i("NewWbufEnd    : 0x%lX",Bkup_WbufEnd + BufStride);

         break;

      case FCATCH_RESTART:
         gm_WriteRegDWord_BE(RegFrcWbufStart[gmvb_MainChannel],Bkup_WbufStart);
         gm_WriteRegDWord_BE(RegFrcWbufStart[gmvb_MainChannel]+4,Bkup_WbufEnd);
         gm_WriteRegDWord_BE(RegFrcRbufStart[gmvb_MainChannel],Bkup_WbufStart);
         gm_WriteRegDWord_BE(RegFrcRbufStart[gmvb_MainChannel]+4,Bkup_WbufEnd);
         gm_SyncUpdateWait(gmvb_MainChannel, INPUT_OUTPUT_CLOCK);
         gm_InputWaitVsync(gmvb_MainChannel , 1);
         gm_WriteRegDWord_BE(RegFrcWbufStart[gmvb_MainChannel],Bkup_WbufStart + BufStride);
         gm_WriteRegDWord_BE(RegFrcWbufStart[gmvb_MainChannel]+4,Bkup_WbufEnd + BufStride);
         gm_WriteRegDWord_BE(RegFrcRbufStart[gmvb_MainChannel],Bkup_WbufStart + BufStride);
         gm_WriteRegDWord_BE(RegFrcRbufStart[gmvb_MainChannel]+4,Bkup_WbufEnd + BufStride);
         break;

      case FCATCH_END:
         gm_WriteRegDWord_BE(RegFrcWbufStart[gmvb_MainChannel],Bkup_WbufStart);
         gm_WriteRegDWord_BE(RegFrcWbufStart[gmvb_MainChannel]+4,Bkup_WbufEnd);
         gm_WriteRegDWord_BE(RegFrcRbufStart[gmvb_MainChannel],Bkup_WbufStart);
         gm_WriteRegDWord_BE(RegFrcRbufStart[gmvb_MainChannel]+4,Bkup_WbufEnd);
         gm_WriteRegWord(RegFrcCtrl[gmvb_MainChannel],Bkup_FrcControl);
         break;
   }
   gm_InputWaitVsync(gmvb_MainChannel , 1);
   gm_SyncUpdateWait(gmvb_MainChannel, INPUT_OUTPUT_CLOCK);
   return gm_GetSystemTime();
}


BOOL IsIspAutoDetMode(void)
{
   return b_IsIspAutoMode;
}

void SetIspAutoDetMode(BOOL b_Set)
{
   if (b_Set==FALSE)
   {
      msg("SetIspAutoDetMode=FALSE");
      b_IsIspAutoMode=FALSE;
   }
   else
   {
      msg("SetIspAutoDetMode=TRUE");
      b_IsIspAutoMode=TRUE;
   }
}



#ifdef SMART_ISP_AUTO_DETECTION
static BOOL IsMatch(PixRGB_Struct* Src,ROM PixRGB_Struct* Dst)
{
   if ((Src->b_Red & 0xF0) ==Dst->b_Red)
      if ((Src->b_Grn & 0xF0)==Dst->b_Grn)
         if ((Src->b_Blu & 0xF0)==Dst->b_Blu)
            return TRUE;
   return FALSE;
}

void SmartIspDetectHandler(void)
{
   static WORD w_Xcord;
   static WORD w_Ycord;
   static BYTE b_PrefixNum;
   static BYTE b_PatternNum;
   PixRGB_Struct PixRead;

#ifdef DEBUG_ISPAUTO
   static BYTE pre_Sts=0xFF;
#endif

	//#ifdef DELL_U2410F		//Neil120714+
	//if((UserPrefFactoryFlag == FALSE)||(UserPrefStartSmartISP == FALSE))
	//	return;
	//#endif
	if((UserPrefStartSmartISP == FALSE))
		return;
	
   if (gm_TimerCheck(SMART_ISP_DET_100ms_TMR)==TIMER_OK)
      return;

   if ((b_IspDetSts==ISP_DETHDLR_SEARCH_PREFIX)||(b_IspDetSts==ISP_DETHDLR_SEARCH_PATTERN))
   {
      if (gm_ReadRegWord(MAIN_INPUT_STATUS) & IMP_LINEFLAG)
      {
         PixRead.b_Red=(BYTE)(gm_ReadRegWord(IMP_PIXGRAB_RED)>>2);
         PixRead.b_Grn=(BYTE)(gm_ReadRegWord(IMP_PIXGRAB_GRN)>>2);
         PixRead.b_Blu=(BYTE)(gm_ReadRegWord(IMP_PIXGRAB_BLU)>>2);
      }
      else
         return;
   }


   switch (b_IspDetSts)
   {
      case ISP_DETHDLR_RESET:
#ifdef DEBUG_ISPAUTO
         if (pre_Sts!=b_IspDetSts)
         {
            IspAuto_msg("---ISP_DETHDLR_RESET---");
            pre_Sts=b_IspDetSts;
         }
#endif
         b_IspDetSts++;
         gm_TimerStart(SMART_ISP_DET_100ms_TMR,SMART_ISP_DETECTION_INTERVAL);
         return;

      case ISP_DETHDLR_START:
#ifdef DEBUG_ISPAUTO
         if (pre_Sts!=b_IspDetSts)
         {
            IspAuto_msg("---ISP_DETHDLR_START---");
            pre_Sts=b_IspDetSts;
         }
#endif
         w_Xcord=H_BOARDER_L+
                 (PREFIX_NUM_MAX>>1)+
                 gm_ReadRegWord(IMP_IBD_HSTART);
         w_Ycord=((gm_ReadRegWord(IMP_IBD_VLENGTH)-V_BOARDER)>>1)+
                 V_BOARDER_TOP+
                 gm_ReadRegWord(IMP_IBD_VSTART);
         IspAuto_msg_ii("X(%d),Y(%d)",w_Xcord-gm_ReadRegWord(IMP_IBD_HSTART),w_Ycord-gm_ReadRegWord(IMP_IBD_VSTART));
         gm_WriteRegWord(IMP_PIXGRAB_V, w_Ycord);            // co-ords of the pixel we want
         gm_WriteRegWord(IMP_PIXGRAB_H, w_Xcord);
         gm_WriteRegWord(IMP_FLAGLINE, (w_Ycord + 1));         // wait for the correct line
         gm_SetRegBitsWord(IMP_CONTROL, IMP_PIXGRAB_EN);         // enable pixel grab
         gm_SetRegBitsWord(MAIN_INPUT_STATUS, IMP_LINEFLAG);      // clear IP_LINEFLAG
         b_PrefixNum=PREFIX_NUM_MAX;
         b_PatternNum=0;
         b_IspDetSts++;
         return;
      case ISP_DETHDLR_SEARCH_PREFIX:
#ifdef DEBUG_ISPAUTO
         if (pre_Sts!=b_IspDetSts)
         {
            IspAuto_msg("---ISP_DETHDLR_SEARCH_PREFIX---");
            pre_Sts=b_IspDetSts;
         }
#endif

         if (IsMatch(&PixRead, &PrefixPattern)==TRUE)
         {
            if (b_PrefixNum)
            {
               w_Xcord++;
               gm_WriteRegWord(IMP_PIXGRAB_H, w_Xcord);
               b_PrefixNum--;
               gm_SetRegBitsWord(MAIN_INPUT_STATUS, IMP_LINEFLAG);      // clear IP_LINEFLAG
            }
            else
            {
               b_IspDetSts=ISP_DETHDLR_RESET;
            }
         }
         else
         {
            if (b_PrefixNum<PREFIX_NUM_MAX)
            {
               if (IsMatch(&PixRead, &PrefixPatternEnd)==TRUE)
               {

                  w_Xcord++;
                  gm_WriteRegWord(IMP_PIXGRAB_H, w_Xcord);
                  gm_SetRegBitsWord(MAIN_INPUT_STATUS, IMP_LINEFLAG);      // clear IP_LINEFLAG
                  b_PatternNum=0;
                  b_IspDetSts++;
               }
               else
               {
                  b_IspDetSts=ISP_DETHDLR_RESET;

               }
            }
            else
            {
               b_IspDetSts=ISP_DETHDLR_RESET;
            }
         }
         break;
      case ISP_DETHDLR_SEARCH_PATTERN:
#ifdef DEBUG_ISPAUTO
         if (pre_Sts!=b_IspDetSts)
         {
            IspAuto_msg("---ISP_DETHDLR_SEARCH_PATTERN---");
            pre_Sts=b_IspDetSts;
         }
#endif

         if (b_PatternNum<PATTERN_LENGTH)
         {
            if (IsMatch(&PixRead,&IspDetPattern[b_PatternNum])==TRUE)
            {
               w_Xcord++;
               gm_WriteRegWord(IMP_PIXGRAB_H, w_Xcord);
               gm_SetRegBitsWord(MAIN_INPUT_STATUS, IMP_LINEFLAG); 	 // clear IP_LINEFLAG
               IspAuto_msg_iiii("P(%d)=(%X,%X,%X)",b_PatternNum,PixRead.b_Red,PixRead.b_Grn,PixRead.b_Blu);
               b_PatternNum++;
               return;
            }
            else
            {
               b_IspDetSts=ISP_DETHDLR_RESET;
               return;
            }
         }
         else
         {
            b_IspDetSts++;
            gm_ClearRegBitsWord(IMP_CONTROL, IMP_PIXGRAB_EN);
         }
      case ISP_DETHDLR_PATTERN_MATCH:
#ifdef DEBUG_ISPAUTO
         if (pre_Sts!=b_IspDetSts)
         {
            IspAuto_msg("---ISP_DETHDLR_PATTERN_MATCH---");
            pre_Sts=b_IspDetSts;
         }
#endif

         b_IspDetSts=ISP_DETHDLR_STOP;

#if (defined(DELL_U2410F) && defined(SMART_ISP) && defined(SMART_ISP_AUTO_DETECTION))//111216 Edward add temp for non ISP spec 
	  IspThruDisplay(TRUE);
#else
         OSDRequestMsg(gmvb_MainChannel, MTO_IspAutoDetected, REQUEST_NOW);
#endif
         break;
      case ISP_DETHDLR_STOP:
#ifdef DEBUG_ISPAUTO
         if (pre_Sts!=b_IspDetSts)
         {
            IspAuto_msg("---ISP_DETHDLR_STOP---");
            pre_Sts=b_IspDetSts;
         }
#endif

      default:
         break;
   }
}

void EnableSmartIspDetect(BOOL b_En)
{
   if (b_En==TRUE)
   {
      b_IspDetSts=ISP_DETHDLR_RESET;
      msg("EnableSmartIspDetect=TRUE");
   }
   else
   {
      b_IspDetSts=ISP_DETHDLR_STOP;
      gm_ClearRegBitsWord(IMP_CONTROL, IMP_PIXGRAB_EN);
      msg("EnableSmartIspDetect=FALSE");
      //b_IsIspAutoMode=FALSE;
   }
}

#endif //SMART_ISP_AUTO_DETECTION





#else //SMART_ISP

//***************************************************************
//***** Below APIs is to make compiler happy for Standard   *****
//***** OSD project. If a project doesn't enable SMART_ISP, *****
//***** It should not include any access to SMART ISP APIs. *****
//***************************************************************

BOOL IspThruDisplay(BYTE ExeMode)
{
   UNUSED_VAR(ExeMode);
   return FALSE;
}

BYTE GetIspStep(void)
{
   return ISP_STEP_NONE;
}

BYTE GetIspStepStatus(void)
{
   return ISP_SSTATUS_FAILED;
}

void SetIspActionConfirm(BYTE answer)
{
   UNUSED_VAR(answer);
}

BYTE GetIspActionConfirm(void)
{
   return ISP_ACTION_REJECTED;
}

void SysteHardReset(void)
{
}

BOOL IsIspAutoDetMode(void)
{
   return FALSE;
}

void SetIspAutoDetMode(BOOL b_Set)
{
   UNUSED_VAR(b_Set);
}

#endif //SMART_ISP

