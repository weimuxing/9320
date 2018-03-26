#define __DDC2BI_COMM_C__

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
// MODULE:      Ddc2bi.c
//
// USAGE:       This module contains ddc2bi parser commands
//
//
//******************************************************************************
#include <string.h>
#include "System\all.h"
#include "StdLibInc\gm_Register.h"

#define DEBUG_DDC2BI_VCP
#ifdef DEBUG_DDC2BI_VCP
#define msg_ddc gm_Printf
#define msg_ddc1 gm_Printf
#else
#define msg_ddc(a,b)
#define msg_ddc1(a,b,c)
#endif

/////////Wistron Lin Chihlung/////////////////
#define SWAP_BYTES(a) ((a >> 8) | (a << 8))
BYTE Factory_Command;
BYTE Temp_Data;
WORD Temp_Data1;
void Factory_AdjustBackLight(BYTE W_Pulse);
///////////////////////////////////////////

#define DDC2BI_MAX_MSG_LENGTH    0x7F
//#define DDC2B_SRC_ADDRESS  0x51
//#define DDC2BI_DEVICE_ID 0x6E

//To make MCCS 2.0 Compliant
//Definitions related to VCP response
#define DispControllerType  0x0D // 13 for STMicro // 0x02    // 2 for Genesis
#define ApplnEnableKey      0x45CC  //define a key value here
#if 1
#define ManufacturerSpcific         0x66 //chunbao for DDC2B_CMD_VCP_ManufacturerSpcific
EHFactoryCommandFeature EH_CommFeature;
#elif defined(Qisda_93XX_BOARD)
#define ManufacturerSpcific         0x62 //chunbao for DDC2B_CMD_VCP_ManufacturerSpcific
#else	//Wistron
#define ManufacturerSpcific         0x74 //chunbao for DDC2B_CMD_VCP_ManufacturerSpcific
#endif
#define OSD_Enable          0x02    // 2 for enable OSD
#define OSD_Disable         0x01    // 2 for disable OSD
#define VCP_Version        0x0201   // version 2, Revision 0
#define DispTechnologyType  0x03    // 3 for TFT
#define NewControlVal       0x02    // 2 for a new control value has been saved
#define NoNewControlVal     0x01    // 1 for a NO new control value

extern PortSchemeDependentUserPrefType ROM SchemeFactoryDefaultInROM[NUM_IMAGE_SCHEMES]; // PSD

static BYTE VCPPage;
static VCPFeaturePktType VCPFeature;
static BYTE DdcciCmdParsed = FALSE;
extern gmt_PwrStateType PwrState;
BYTE B_DDCCINewControlVal;
static BYTE DDCCI_Channel=0xFF;//120910 Edward for adjust DDM in pxp do not effect on main channel
BOOL DDCPowerNapFlag = FALSE;
#ifdef VALUE_MATCH_WITH_OSD		
#define OSD_BrightnessMax	100
#define OSD_BrightnessMin	0

#define OSD_ContrastMax		100
#define OSD_ContrastMin		0

#define OSD_DEFAULT_MAX_VALUE 	100
#define OSD_DEFAULT_MIN_VALUE 	0
#endif

#ifdef DDCCi_PROTOCOL_SUPPORT
//The I2C bus specification requires that a host be able to send and receive
//messages of any size up to the maximum of 131 bytes (127 + 4). But for DDC/CI,
//the maximum permitted length is 32 bytes of data to minimize the risk of bus
//contention and to keep the required buffer size in the display to a reasonable
//size/cost. Messages longer than 32 bytes must be transmitted as a number of 32
//byte fragments, the last fragment may be less than 32 bytes if message is not
//a multiple of 32 bytes.
#define DDCCI_MSG_SIZE 32

void Ddcci(void*, HANDLER_USE_t);


BYTE DDC2Bi_VCP_ParseCommand(BYTE SetVcp);
static WORD EndianConv(WORD W_Input);
#endif

#ifdef USE_XRite
void DDC2Bi_VCP_ParseCommand_XRite(void* gExtDebugCmdPtr);
extern gmt_RET_STAT gm_LoadDUMTable(WORD W_StartIndex, WORD W_Size, WORD far *Wp_Data);

BYTE MarsInBusyState = FALSE; 
BYTE PrevUserPrefGraphicsImageMode=0;
extern BOOL IsPwrTransitionKeyPressed;
//#define DEBUG_DDC2BI_VCP_XRite
#ifdef DEBUG_DDC2BI_VCP_XRite
#define msg_ddc_xrite gm_Printf
#else
#define msg_ddc_xrite(a,b)
#endif

#endif

void PresetModeSettings(void);

static const far BYTE NakPkt[] = {3, 0x0B};

#ifdef USE_COLOR_FIDELITY
            static BYTE ROM strCapabilitiesRequest[] =
               "(type(LCD)(vcp(02 04 05 06 08 0E 10 12 14(05 08 0A 0B) 16 18 1A 1E 20 30 3E 6C 6E 70 AC AE B6 C6 C8 C9 CA(01 02) DF F0)mccs_ver(2.0)asset_eep(32)mswhql(1))";
#else
BYTE ROM VGAvcpstr[]="(vcp(02 04 05 06 08 0E 10 12 14(01 04 05 06 08 09 0B 0C) 16 18 1A 1E 20 30 3E 60(01 03 0F 10 11) 6C 6E 70 AA AC AE B6 C6 C8 C9 D6(01 04 05) DC(00 02 03 04 05 06) DF E0 E1 E2(00 01 02 04 0B 0C 0D 0F 10 11 13 14 19) E4(00 01) F0(01 02 03 04 05 06 07) F1 F2 FD)type(LCD)mccs_ver(2.0)asset_eep(32)mswhql(1))"; 

//BYTE ROM strCapabilitiesRequest[] = "(vcp(02 04 05 08 10 12 14(01 04 05 06 08 09 0B 0C)16 18 1A 60(01 03 0F 10 11) 6C 6E 70 AA(01 02) AC AE B6 C6 C8 C9 D6(01 04 05) DC(00 02 03 05) DF E0 E1 E2(00 01 02 04 0B 0C 0D 0F 10 11 13 14 19) E3(00 01 02)"
//" E4(00 01) F0(02 05 06 08) F1 F2 FD)type(LCD)mccs_ver(2.1)asset_eep(32)mswhql(1))";
//BYTE ROM strCapabilitiesRequest[] = "(vcp(02 04 05 08 10 12 14(01 04 05 06 08 09 0B 0C)16 18 1A 60(03 0F 10 11) 6C 6E 70 AA(01 02) AC AE B6 C6 C8 C9 D6(01 04 05) DC(00 02 03 05) DF E0 E1 E2(00 01 02 04 07 0A 15 0B 0C 0D 0F 10 11 13 14 19) E3(00 01 02)"
//" E4(00 01) F0(02 05 06 08) F1 F2 FD)type(LCD)mccs_ver(2.1)asset_eep(32)mswhql(1))";
BYTE ROM strCapabilitiesRequest[] = "(vcp(02 04 05 08 10 12 14(01 04 05 06 08 09 0B 0C)16 18 1A 60(01 03 0F 10 11) 6C 6E 70 73 75 AA(01 02) AC AE B6 C6 C8 C9 D6(01 04 05) DC(00 02 03 05) DF E0 E1 E2(00 01 02 04 0B 0C 0D 0F 10 11 13 14 19) E3(00 01 02)"
" E4(00 01) F0(02 05 06 08) F1 F2 FD)type(LCD)mccs_ver(2.1)asset_eep(32)mswhql(1))";

#endif


//******************************************************************************
// FUNCTION:   I2CSlaveInitAll
// USAGE:      Initialization of All I2C slave channels
// INPUT:      None
// OUTPUT:     None
// GLOBALS:
// USED_REGS:  None
//******************************************************************************
void far I2CSlaveInitAll(void)
{
	SLAVE_CH B_SlaveCh;

   //gm_ProgramLpmClkandPbus();
   gm_ClearAndSetBitsWord(CLOCK_CONFIG1, LPM_CLK_SEL, 1<<LPM_CLK_SEL_SHIFT); //LPM_CLK_SEL=OCM_CLK
   CsmI2cSlvRoute = LPM_I2CSLV_PINS_TO_SLAVE;
   gm_I2CSlaveInitAll();
   //Enable all i2c slave1 for LPM i2c master
   gm_WriteRegWord(I2C_SLAVE1_CTRL1, I2C_SLAVE1_EN);
   #ifdef LPM_UART_SUPPORT
   gm_ClearRegBitsByte(I2C_SLAVE4_CTRL1, I2C_SLAVE4_EN);
   #endif
   for(B_SlaveCh=I2C_SLAVE2_CH; B_SlaveCh<=I2C_SLAVE3_CH ; B_SlaveCh++)
   {
      gm_I2CSlaveInit((BYTE *)&I2CSlaveTable[B_SlaveCh], I2C_SLAVE_RD_LEN, B_SlaveCh);
      gm_I2CSlaveAddCommHandler((I2C_SLAVE_HANDLER_TYPE)gm_I2CSlaveGProbeHandler, DDC2BI_ADDR_ID, B_SlaveCh);
      gm_I2CSlaveAddI2CAddress(DDC2BI_ADDR_ID, B_SlaveCh);		
   }  

#if ( ((FEATURE_SOCIPC == ENABLE) && !defined(Athena_4K2K_Chip_A)) || defined(FEATURE_HOSTIF) )
	for(B_SlaveCh=I2C_SLAVE1_CH; B_SlaveCh<I2C_SLAVE_NUM_CH ; B_SlaveCh++)
	{
		#ifdef Athena_4K2K_Chip_B
		//Athena B Registration
		gm_I2CSlaveAddCommHandler((I2C_SLAVE_HANDLER_TYPE)SoCIPC_SlavDevHandler, SOCIPC_DEVICEB_DST_ADDRESS, B_SlaveCh);
		gm_I2CSlaveAddI2CAddress(SOCIPC_DEVICEB_DST_ADDRESS, B_SlaveCh);
		#endif

		#ifdef Athena_4K2K_Chip_C
		//Athena C Registration
		gm_I2CSlaveAddCommHandler((I2C_SLAVE_HANDLER_TYPE)SoCIPC_SlavDevHandler, SOCIPC_DEVICEC_DST_ADDRESS, B_SlaveCh);
		gm_I2CSlaveAddI2CAddress(SOCIPC_DEVICEC_DST_ADDRESS, B_SlaveCh);	
		#endif

		#ifdef FEATURE_HOSTIF
		gm_I2CSlaveAddCommHandler((I2C_SLAVE_HANDLER_TYPE)SoCIPC_SlavDevHandler, HOSTIF_DEVICE_DST_ADDRESS, B_SlaveCh);
		gm_I2CSlaveAddI2CAddress(HOSTIF_DEVICE_DST_ADDRESS, B_SlaveCh);
		#endif      
	}
#endif
   
}

//******************************************************************************
// FUNCTION:   I2CSlaveChannelHandler
// USAGE:      Handler for specific I2C Slave channel 
// INPUT:      None
// OUTPUT:     None
// GLOBALS:
// USED_REGS:  None
//******************************************************************************
void far I2CSlaveChannelHandler(SLAVE_CH B_SlaveCh)
{
   PARSER_STATUS_t B_Status = NO_MSG_PARSED;
   I2C_SLAVE_TYPE far* Sp_I2CSlavePtr = Sp_I2CSlavePtrGlobal[B_SlaveCh];   

   gm_I2CSlaveSetChannel(B_SlaveCh);
   
   if(Sp_I2CSlavePtr == NULL_PTR)
   {
      gB_Ddc2Bi_I2C_Status[B_SlaveCh] = B_Status;
	  return;
   }

   B_Status = gm_I2CSlaveExecCommHandler(B_SlaveCh);
   if(B_Status == UNKNOWN_MSG_CMD)
   {
      BYTE B_whichHandler = B_SlaveCh + I2C_SLAVE1_USE;
      
      if((I2C_as_Gprobe_Cmd(B_SlaveCh) == gB_GprobeCmdSet) &&
         (I2C_ManufacturerID(B_SlaveCh) == gW_ManufacturerID))
   	{
         GprobeCmdParser(&Sp_I2CSlavePtr->Buffer[Start_of_GprobeCmd_in_DDC2BiMsg], UserPrefUartSelect);
    }
	else
	{
#ifdef DDCCi_PROTOCOL_SUPPORT   
	//if ((UserPrefDdcciOnOffFlag == ENABLE)) //chunbao for osd->ddcci disable ddc channel
    	{
    	Ddcci((BYTE*)&Sp_I2CSlavePtr->Buffer, B_whichHandler);
    	}
      //gm_Print("DDCCI commands!",0);
#endif      
   }
   }

   if((B_Status == NO_MSG_PARSED) 
    /*|| (B_Status == UNKNOWN_MSG_CMD) && (B_ModeStatus == INTERNAL_ROM_MODE)*/)
   {
      gm_I2CSlaveDefaultHandler(B_SlaveCh);
   }

   gB_Ddc2Bi_I2C_Status[B_SlaveCh] = B_Status;

}

//******************************************************************************
// USAGE          : DDC2bi port commands parser. Gets the message and passes
//                  it on to the appropriate parser; Gprobe or DDC, based on
//                  the type of message received. Passes control to the Customer
//                  Protocol handler if that is being used.
//******************************************************************************
void Ddc2biCmdHandler(void)
{
#if 1
   I2CSlaveChannelHandler(I2C_SLAVE1_CH);
   I2CSlaveChannelHandler(I2C_SLAVE2_CH);
   I2CSlaveChannelHandler(I2C_SLAVE3_CH);
   I2CSlaveChannelHandler(I2C_SLAVE4_CH);
#else

   BYTE buf[DDC2BI_MAX_MSG_LENGTH+1];
   PARSER_STATUS_t status;

   gDebugCmdPtr = (BYTE far*)buf;
   switch (gm_DDC2BiRead(buf))
   {
      case READ_PKT_OK:
         if ((buf[1] == 0xC2) && (*(WORD*)(&buf[2]) == 0))
         {
#if (FEATURE_GPROBE_UART == REV2)
            if (gm_GprobeCmdParser(DDC2BI_USE, (BYTE far*)(&buf[4])) != UNKNOWN_MSG_CMD)
               return;
#endif
            GprobeCmdParser(&buf[4], DDC2BI_USE);
         }
         else // DDC2Ci command parser
         {
#ifdef DDCCi_PROTOCOL_SUPPORT
            Ddcci(buf, DDC2BI_USE);
#endif
         }
         break;

      case READ_PKT_WRONG_CS:
         gm_DDC2BiWriteCmdPkt((BYTE far*)NakPkt);
         return;
   }
#endif
}

//******************************************************************************
#ifdef DDCCi_PROTOCOL_SUPPORT

//******************************************************************************

//******************************************************************************
// USAGE          : Swaps the upper and the lower byte of the input Word
// INPUT          : W_Input
// OUTPUT         : The converted word
//******************************************************************************
static WORD EndianConv(WORD W_Input)
{
   return (W_Input >> 8) | (W_Input << 8);
}

#ifdef USE_COLOR_FIDELITY
#if 1
void DDC2Bi_TableRw_ParseCommand(void* gExtDebugCmdPtr, BYTE Rw_VCP)
{
   BYTE length;
   BYTE command;
   BYTE B_vcp_opcode;
   WORD value;
   BYTE B_mode;
   BYTE B_par;
   BYTE chk;

#if 0
   B_vcp_opcode = *(((BYTE*)gExtDebugCmdPtr)+2);
   value = *(((WORD*)gExtDebugCmdPtr)+3);
   B_mode = *(((BYTE*)gExtDebugCmdPtr)+5);
   B_par = *(((BYTE*)gExtDebugCmdPtr)+6);
#else
   B_vcp_opcode = (((DDC2Bi_WGS_VCPCmd_t*)gExtDebugCmdPtr)->vcp_opcode);
   value = ((((DDC2Bi_WGS_VCPCmd_t*)gExtDebugCmdPtr)->value_H) << 8) | (((DDC2Bi_WGS_VCPCmd_t*)gExtDebugCmdPtr)->value_L);
   B_mode = (((DDC2Bi_WGS_VCPCmd_t*)gExtDebugCmdPtr)->mode);
   B_par = (((DDC2Bi_WGS_VCPCmd_t*)gExtDebugCmdPtr)->par);
#endif


   // 20100209 #1, Added Color Fidelity
   Rw_VCP = Rw_VCP;

   switch (B_vcp_opcode)
   {
      case DDC2B_CMD_VCP_SimplifiedColorProfile:
         {
            gm_Print(" SimplifiedColorProfile ... 0x%x",B_mode);
            gm_Print("  Value = 0x%x",value);
            gm_Print("  Par = 0x%x",B_par);

            switch (B_mode)
            {
               case WGSM_QUERY_CAPABILITY:
                  GetWGSCapabilities((WGSCapStruct*)(&(((DDC2Bi_WGS_ReplyCmd_t*)gExtDebugCmdPtr)->status_H)));
                  ((DDC2Bi_WGS_ReplyCmd_t*)gExtDebugCmdPtr)->length    = 0x8D;    //
                  break;

               case WGSM_SWITCH_COLORMODE:
                  if (SetColorSpaceMode(B_par)==0x00)
                  {
                     //(((DDC2Bi_WGS_ReplyCmd_t*)gExtDebugCmdPtr)->status) = 0x0000;
                     (((DDC2Bi_WGS_ReplyCmd_t*)gExtDebugCmdPtr)->status_H) = 0x00;
                     (((DDC2Bi_WGS_ReplyCmd_t*)gExtDebugCmdPtr)->status_L) = 0x00;
                  }
                  else
                  {
                     //(((DDC2Bi_WGS_ReplyCmd_t*)gExtDebugCmdPtr)->status) = 0xFFFF;
                     (((DDC2Bi_WGS_ReplyCmd_t*)gExtDebugCmdPtr)->status_H) = 0xFF;
                     (((DDC2Bi_WGS_ReplyCmd_t*)gExtDebugCmdPtr)->status_L) = 0xFF;
                  }

                  ((DDC2Bi_WGS_ReplyCmd_t*)gExtDebugCmdPtr)->value_H = 0x00;
                  ((DDC2Bi_WGS_ReplyCmd_t*)gExtDebugCmdPtr)->value_L = 0x00;
                  ((DDC2Bi_WGS_ReplyCmd_t*)gExtDebugCmdPtr)->length    = 0x86;    //
                  break;

               case WGSM_UPDATE_COLORMODE:
                  {
                     //BYTE* ptr = &(B_par);
                     BYTE* ptr = &(((DDC2Bi_WGS_VCPCmd_t*)gExtDebugCmdPtr)->par);

                     gm_Print("  WGSM_UPDATE_COLORMODE ... %d",(*ptr));
                     //gm_Print("  1 ... 0x%x",*(ptr+1));
                     //gm_Print("  2 ... 0x%x",*(ptr+2));
                     //gm_Print("  3 ... 0x%x",*(ptr+3));
                     //gm_Print("  4 ... 0x%x",*(ptr+4));
                     //gm_Print("  5 ... 0x%x",*(ptr+5));
                     //gm_Print("  6 ... 0x%x",*(ptr+6));
                     //gm_Print("  7 ... 0x%x",*(ptr+7));
                     //gm_Print("  8 ... 0x%x",*(ptr+8));
                     if (*ptr==WGSM_CS_MODE_PROFILE && *(ptr+1)==WGSM_UPDATE_SRC_PROFILE) //update simplified color profile
                     {
                        BYTE i, size;

                        //to swap bytes
                        ptr = ptr + 2; //
                        for (i=0; i<sizeof(GamutDataStruct)+1; i=i+2)
                        {
                           size = *ptr;
                           *ptr = *(ptr+1);
                           *(ptr+1) = size;

                           ptr = ptr+2;
                        }

                        ptr = &(((DDC2Bi_WGS_VCPCmd_t*)gExtDebugCmdPtr)->par);
                        size = *(ptr+2);

                        if (size ==0x1C || size==0x2C) //right size, 28 or 44 bytes
                        {
                           if (UpdateSimplifiedColorProfile((GamutDataStruct*)(ptr+4), size-2, *(ptr+1)) == 0x00)
                           {
                              //DoGamutMapping(gamutDataAddr, 0);
                              {
                                 //(((DDC2Bi_WGS_ReplyCmd_t*)gExtDebugCmdPtr)->status) = 0x0000;
                                 (((DDC2Bi_WGS_ReplyCmd_t*)gExtDebugCmdPtr)->status_H) = 0x00;
                                 (((DDC2Bi_WGS_ReplyCmd_t*)gExtDebugCmdPtr)->status_L) = 0x00;
                              }
                           }
                           else
                           {
                              //(((DDC2Bi_WGS_ReplyCmd_t*)gExtDebugCmdPtr)->status) = 0xFFFF;
                              (((DDC2Bi_WGS_ReplyCmd_t*)gExtDebugCmdPtr)->status_H) = 0xFF;
                              (((DDC2Bi_WGS_ReplyCmd_t*)gExtDebugCmdPtr)->status_L) = 0xFF;
                           }
                        }

                     }
                     else
                     {
                        {
                           //(((DDC2Bi_WGS_ReplyCmd_t*)gExtDebugCmdPtr)->status) = 0xFFFF;
                           (((DDC2Bi_WGS_ReplyCmd_t*)gExtDebugCmdPtr)->status_H) = 0xFF;
                           (((DDC2Bi_WGS_ReplyCmd_t*)gExtDebugCmdPtr)->status_L) = 0xFF;
                        }
                     }

                     ((DDC2Bi_WGS_ReplyCmd_t*)gExtDebugCmdPtr)->value_H = 0x00;
                     ((DDC2Bi_WGS_ReplyCmd_t*)gExtDebugCmdPtr)->value_L = 0x00;
                     ((DDC2Bi_WGS_ReplyCmd_t*)gExtDebugCmdPtr)->length    = 0x86;    //
                     break;
                  }

               case WGSM_QUERY_COLORMODE:
               case WGSM_QUERY_NATIVEPROFILE:
               case WGSM_UPDATE_NATIVEPROFILE:
                  break;

               case WGSM_DISPLAY_COLOR:
                  {
                     //add code here for display pattern color
                     if (((DDC2Bi_WGS_ReplyCmd_t*)gExtDebugCmdPtr)->length==0x8C)
                     {
                     }

                     //enable / disable color processing
                     //if(txBuffer[LENGTH]==0x86 || txBuffer[LENGTH]==0x8C)
                     {
                        if (B_par==WGSM_DISPLAY_COLOR_DISABLE)
                           DisableColorProcessing();
                        else
                           EnableColorProcessing();
                     }

                     {
                        //(((DDC2Bi_WGS_ReplyCmd_t*)gExtDebugCmdPtr)->status) = 0x0000;
                        (((DDC2Bi_WGS_ReplyCmd_t*)gExtDebugCmdPtr)->status_H) = 0x00;
                        (((DDC2Bi_WGS_ReplyCmd_t*)gExtDebugCmdPtr)->status_L) = 0x00;
                     }
                     ((DDC2Bi_WGS_ReplyCmd_t*)gExtDebugCmdPtr)->value_H = 0x00;
                     ((DDC2Bi_WGS_ReplyCmd_t*)gExtDebugCmdPtr)->value_L = 0x00;
                     ((DDC2Bi_WGS_ReplyCmd_t*)gExtDebugCmdPtr)->length    = 0x86;    //
                     break;
                  }

               case WGSM_CALIBRATION_MODE:
                  break;

               default:
                  ;
            }
         }
         break;

      default:
         ;
   }

}
#endif
#endif // USE_COLOR_FIDELITY

//120910 Edward for some DDCCI command do not effect main if command from sub
//DP-->AUX_C0_USE
//DVI-->I2C_SLAVE2_USE
//HDMI-->I2C_SLAVE3_USE
//VGA -->I2C_SLAVE4_USE
//Touch I2C -->I2C_SLAVE1_USE
BOOL Is_DDCCI_Command_SubChannel(BYTE Command_Handler)
{
	if(Is_PIP_ON())
	{
		//gm_Print("###Command_Handler  %d",Command_Handler);
		
		if((GetPortConnType(UserPrefCurrentInputPip)==CT_DISPLAY_PORT)&&(Command_Handler==AUX_C0_USE))
			return TRUE;
		else if(IsPIPDVID1Input()&&(Command_Handler==I2C_SLAVE2_USE))
			return TRUE;
		else if(IsPIPHDMIInput()&&(Command_Handler==I2C_SLAVE3_USE))
			return TRUE;
		else if(IsPIPVGAInput()&&(Command_Handler==I2C_SLAVE4_USE))
			return TRUE;
		else
			return FALSE;
	}
	else
		return FALSE;
}

//return the size of capabilities string for segment crossing case
WORD far GetCapabilitiesStringSize(void)
{
		if	(UserPrefCurrentInputMain == ALL_BOARDS_VGA1)
			return sizeof(VGAvcpstr);
		else
   			return sizeof(strCapabilitiesRequest);
}

//******************************************************************************
// USAGE          : DDCCi command parser. Parses, Interprets and executes the
//                  DDCCi commands. It also prepares the response to be sent.
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : gExtDebugCmdPtr: Points to the communication buffer.
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
#pragma argsused
//#define DDC2BiCmdPtr gExtDebugCmdPtr
void Ddcci(void* gExtDebugCmdPtr, HANDLER_USE_t handler)
{
   BYTE readExpected = 1;
   BYTE EndCapStrFlag = 0; 
   BYTE *DDC2BiCmdPtr = gExtDebugCmdPtr;  
   BYTE CapStringReply = 1;
   WORD W_FillDataNum, W_FillZerosNum = 0;   
  
   /*ChengLin: 20120309: Add "AUX_C0_USE" and "AUX_C2_USE" for shift MCCS Data Buffer*/   
   if((handler == I2C_SLAVE1_USE)||(handler == I2C_SLAVE2_USE)||
      (handler == I2C_SLAVE3_USE)||(handler == I2C_SLAVE4_USE))
   {
      DDC2BiCmdPtr++;
      CapStringReply = 0;      
   }
   else if((handler == AUX_C0_USE)||(handler == AUX_C2_USE))
   {
      DDC2BiCmdPtr++;
      
   }

   switch (*(((BYTE*)DDC2BiCmdPtr)+1))
   {
      case DDC2B_CMD_GetVCPFeature:
         VCPPage = 0; //select Page0
         VCPFeature.value_BigEndian = 0;
         VCPFeature.max_BigEndian = 0xFFFF;
         VCPFeature.vcp_opcode = (((DDC2Bi_GetVCPFeatureCmd_t*)DDC2BiCmdPtr)->vcp_opcode);
		 
		 if(VCPFeature.vcp_opcode == DDC2B_CMD_VCP_LUT_SIZE)
		 {
			 //BYTE Bp_Buff[9] = {0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x0E, 0x0E, 0x0E};
			 BYTE i;

			for(i = 0; i <((*(((BYTE*)DDC2BiCmdPtr)))&0x0F)+ 1; i++ )
			{
				gm_Print("Input:DDC2BiCmdPtr = 0x%x ",(*(((BYTE*)DDC2BiCmdPtr)+i)));
			}

			 (((DDC2Bi_GetVCPFeature_ReplyCmd_t*)gExtDebugCmdPtr)->length) = 0x0C | DDC2Bi_CONTROL_STATUS_FLAG;
			 (((DDC2Bi_GetVCPFeature_ReplyCmd_t*)gExtDebugCmdPtr)->command) = 0xE4;
			 //(((DDC2Bi_GetVCPFeature_ReplyCmd_t*)gExtDebugCmdPtr)->result) = DDC2Bi_VCP_ParseCommand(GET_VCP);
				 //_fmemcpy(&(((DDC2Bi_CGetVCPFeature_ReplyCmd_t*)gExtDebugCmdPtr)->result),
					//	  &Bp_Buff[0], 9);			 
			 //(((DDC2Bi_GetVCPFeature_ReplyCmd_t*)gExtDebugCmdPtr)->a_VCPFeature) = VCPFeature;
			 *(((BYTE*)DDC2BiCmdPtr)+1) = 0x00;
			 *(((BYTE*)DDC2BiCmdPtr)+2) = 0x00;
			 *(((BYTE*)DDC2BiCmdPtr)+3) = 0x04;
			 *(((BYTE*)DDC2BiCmdPtr)+4) = 0x00;			 
			 *(((BYTE*)DDC2BiCmdPtr)+5) = 0x04;
			 *(((BYTE*)DDC2BiCmdPtr)+6) = 0x00;
			 *(((BYTE*)DDC2BiCmdPtr)+7) = 0x04;
			 *(((BYTE*)DDC2BiCmdPtr)+8) = 0x00;
			 *(((BYTE*)DDC2BiCmdPtr)+9) = 0x0E;
			 *(((BYTE*)DDC2BiCmdPtr)+10) = 0x0E;
			 *(((BYTE*)DDC2BiCmdPtr)+11) = 0x0E;
			 for(i = 0; i <((*(((BYTE*)gExtDebugCmdPtr)))&0x0F)+ 1; i++ )
			 {
				 gm_Print("Output: gExtDebugCmdPtr = 0x%x ",(*(((BYTE*)gExtDebugCmdPtr)+i)));
			 }
				
			 
		 }
		 else
		 
		 {
			 (((DDC2Bi_GetVCPFeature_ReplyCmd_t*)gExtDebugCmdPtr)->length) = 0x08 | DDC2Bi_CONTROL_STATUS_FLAG;
			 (((DDC2Bi_GetVCPFeature_ReplyCmd_t*)gExtDebugCmdPtr)->command) = DDC2B_CMD_GetVCPFeature_Reply;
			 (((DDC2Bi_GetVCPFeature_ReplyCmd_t*)gExtDebugCmdPtr)->result) = DDC2Bi_VCP_ParseCommand(GET_VCP);
			 (((DDC2Bi_GetVCPFeature_ReplyCmd_t*)gExtDebugCmdPtr)->a_VCPFeature) = VCPFeature;
		 }
         msg_ddc1("Get VCP opcode: %x, res: %x", VCPFeature.vcp_opcode,
                  (((DDC2Bi_GetVCPFeature_ReplyCmd_t*)gExtDebugCmdPtr)->result));		 
         break;

      case DDC2B_CMD_SetVCPFeature:
         VCPPage = 0; //select Page0
         VCPFeature.vcp_opcode   = (((DDC2Bi_SetVCPFeatureCmd_t*)DDC2BiCmdPtr)->vcp_opcode);
         VCPFeature.value_BigEndian    = (((DDC2Bi_SetVCPFeatureCmd_t*)DDC2BiCmdPtr)->val_BigEndian);
         DDC2Bi_VCP_ParseCommand(SET_VCP);
         *(BYTE far*)gExtDebugCmdPtr = 0x80;
         readExpected = 0;
         msg_ddc1("Set VCP opcode: %x, res: %x", VCPFeature.vcp_opcode, VCPFeature.value_BigEndian);
         break;

      case DDC2B_CMD_SaveCurrentSettings:
         msg_ddc("Save Current Settings",0);
		 	if(UserPrefFactoryFlag)
		 		SaveFactorySettingsInNVR();
		SavePortSchemeDependentSettings();
         	SaveModeIndependentSettings();
         *(BYTE far*)gExtDebugCmdPtr = 0x80; //no messages back
         readExpected = 0;
         break;

      case DDC2B_CMD_CapabilitiesRequest:
         {
            //capabilities reply thru I2C slave channel is moved to FastDdcciParser()
            //DO NOT handle here to avoid data collision!!
            if(CapStringReply == 0)
               return;
            else
            {
            //Note. Next code and data block is a template for Capabilities stings support.
            //Each CpReqStr entry must not exceed 32 bytes by DDCCI standard
            //#if USE_DYNAMIC_GAMUTMAPPING_SUPPORT
            WORD W_offset;
            WORD W_length;
			BYTE B_Cnt;	
            W_offset = (((DdcciCapabilitiesRequest*)DDC2BiCmdPtr)->offset_BigEndian);
            (((DdcciCapabilitiesReply*)gExtDebugCmdPtr)->length) = VCP_GETCAPABILITIES_REPLYMSG_SIZE | DDC2Bi_CONTROL_STATUS_FLAG;
            (((DdcciCapabilitiesReply*)gExtDebugCmdPtr)->command) = DDC2B_CMD_CapabilitiesRequest_Reply;
            (((DdcciCapabilitiesReply*)gExtDebugCmdPtr)->offset_BigEndian) = W_offset;
            W_offset = EndianConv(W_offset);
            msg_ddc("Get Capabilities offset %d", W_offset);

	if  (UserPrefCurrentInputMain == ALL_BOARDS_VGA1)
	{
		if (W_offset < sizeof(VGAvcpstr))
         	{
	       	  W_length = sizeof(VGAvcpstr) - W_offset;

			  
               	if (W_length >= DDCCI_MSG_SIZE)
              	{
                 	 W_length = DDCCI_MSG_SIZE + 3; //32 bytes cap str + 2 bytes offset + 1 byte checksum
                 	 W_FillDataNum = DDCCI_MSG_SIZE;                   
                  	W_FillZerosNum = 0;                  
           	 }
            	else
               {
                  W_FillDataNum = W_length;               
                     W_length += 2;    //add 2 bytes for offset 				  
                  W_FillZerosNum = DDCCI_MSG_SIZE - W_length; 
                     EndCapStrFlag = 1;				  
               }

			_fmemcpy(&(((DdcciCapabilitiesReply*)gExtDebugCmdPtr)->data),
                        &VGAvcpstr[W_offset], W_FillDataNum);

              // for(B_Cnt=0; B_Cnt<W_FillZerosNum; B_Cnt++)
              //    *(&(((DdcciCapabilitiesReply*)gExtDebugCmdPtr)->data)+W_FillDataNum+B_Cnt) = 0;
                          
              // W_length = DDCCI_MSG_SIZE + 3; //E3+offs_h+offs_l           
        }
        else  //Fill null messages to buffer for handling extra data read
        {
               BYTE B_SlaveCh = handler - I2C_SLAVE1_USE;
               
               for(B_Cnt=0; B_Cnt<DDCCI_MSG_SIZE; B_Cnt++)
                  *(&(((DdcciCapabilitiesReply*)gExtDebugCmdPtr)->data)+B_Cnt) = gm_I2CSlaveCreateNullMsg(0, DDC2BI_ADDR_ID, B_SlaveCh);
             
               W_length = 3;
               EndCapStrFlag = 2;              
        }
   }
   else	
   {
			 if (W_offset < sizeof(strCapabilitiesRequest))
            {
               W_length = sizeof(strCapabilitiesRequest) - W_offset;

               if (W_length >= DDCCI_MSG_SIZE)
               {
                     W_length = DDCCI_MSG_SIZE + 3; //32 bytes cap str + 2 bytes offset + 1 byte checksum
                  W_FillDataNum = DDCCI_MSG_SIZE;                   
                  W_FillZerosNum = 0;                  
            }
            else
               {
                  W_FillDataNum = W_length;               
                     W_length += 2;    //add 2 bytes for offset  
                  W_FillZerosNum = DDCCI_MSG_SIZE - W_length; 
                     EndCapStrFlag = 1;
               }
                  
               _fmemcpy(&(((DdcciCapabilitiesReply*)gExtDebugCmdPtr)->data),
                        &strCapabilitiesRequest[W_offset], W_FillDataNum);
            }
            else  //Fill null messages to buffer for handling extra data read
            {
               BYTE B_SlaveCh = handler - I2C_SLAVE1_USE;
               
               for(B_Cnt=0; B_Cnt<DDCCI_MSG_SIZE; B_Cnt++)
                  *(&(((DdcciCapabilitiesReply*)gExtDebugCmdPtr)->data)+B_Cnt) = gm_I2CSlaveCreateNullMsg(0, DDC2BI_ADDR_ID, B_SlaveCh);
             
               W_length = 3;
                  EndCapStrFlag = 2;              
            }
}
		

            ((DdcciCapabilitiesReply*)gExtDebugCmdPtr)->length = (BYTE)W_length | DDC2Bi_CONTROL_STATUS_FLAG;
            break;
         }
         }
      case DDC2B_CMD_GetTimingReport:
         {
			#define OutOfRangeBit		BIT7
			#define UnstblCountBit		BIT6
			#define HSyncPolarityBit	BIT1
			#define VSyncPolarityBit	BIT0
            BYTE B_Status = 0;
            gmt_INPUT_FLAGS B_Flags = stInput[gmvb_MainChannel].ModeFlags;
            msg_ddc("Get Timing Report",0);

            if (B_Flags & gmd_MODE_OUT_RANGE ) //Out of range
               SetBit(B_Status, OutOfRangeBit);
            if (B_Flags & gmd_MODE_UNKNOWN)
               SetBit(B_Status, UnstblCountBit);
            if (!(B_Flags & gmd_MODE_NEG_HSYNC))
               SetBit(B_Status, HSyncPolarityBit);
            if (!(B_Flags & gmd_MODE_NEG_VSYNC))
               SetBit(B_Status, VSyncPolarityBit);
            (((DDC2Bi_GetTiming_ReplyCmd_t*)gExtDebugCmdPtr)->length) = 0x06;
            (((DDC2Bi_GetTiming_ReplyCmd_t*)gExtDebugCmdPtr)->command) = DDC2B_CMD_GetTimingReport_Reply;
            (((DDC2Bi_GetTiming_ReplyCmd_t*)gExtDebugCmdPtr)->status) = B_Status;
            (((DDC2Bi_GetTiming_ReplyCmd_t*)gExtDebugCmdPtr)->h_freq_BigEndian) = EndianConv((WORD)(((DWORD)stInput[gmvb_MainChannel].Hfrq *100) >> 8));
            (((DDC2Bi_GetTiming_ReplyCmd_t*)gExtDebugCmdPtr)->v_freq_BigEndian) = EndianConv(stInput[gmvb_MainChannel].Vfrq *10);
            break;
         }

      case DDC2B_CMD_NMV_EEPROM:
           { 
         msg_ddc("EEPROM",0);
	            switch( (((DDC2Bi_EEPROMCmd_t*)DDC2BiCmdPtr)->eeprom_cmd))
	            {
		         case DDC2B_CMD_NMV_ModeChangeRequest:
			  Factory_Command=((DDC2Bi_ModeChangeRequest_t*)DDC2BiCmdPtr)->mode_id;
		        if((((DDC2Bi_ModeChangeRequest_t*)DDC2BiCmdPtr)->mode_id) == 3)
			   {
				UserPrefFactoryFlag = TRUE;
				UserPrefRunBurnInModeFlag = TRUE;     //20120803
		               SaveModeIndependentSettings();  
			    }
			else if((((DDC2Bi_ModeChangeRequest_t*)DDC2BiCmdPtr)->mode_id)== 0)
			  {
				UserPrefFactoryFlag = FALSE;
				UserPrefRunBurnInModeFlag = FALSE;
		               SaveModeIndependentSettings();  
				
			   }
                       msg_ddc("ModeChangeRequest: %d", UserPrefFactoryFlag);                     
                       (((DDC2Bi_ModeChange_ReplyCmd_t*)gExtDebugCmdPtr)->length) = 0x03 | DDC2Bi_CONTROL_STATUS_FLAG;
                       (((DDC2Bi_ModeChange_ReplyCmd_t*)gExtDebugCmdPtr)->command) = DDC2B_CMD_NMV_EEPROM_Reply;
                       (((DDC2Bi_ModeChange_ReplyCmd_t*)gExtDebugCmdPtr)->eeprom_cmd) = DDC2B_CMD_NMV_ModeChangeRequest;
                       (((DDC2Bi_ModeChange_ReplyCmd_t*)gExtDebugCmdPtr)->mode_id) = Factory_Command;
                    		 		
         break;

		      case DDC2B_CMD_NMV_EEPROMRead:
			Temp_Data=(((DDC2Bi_EEPROMReadCmd_t*)DDC2BiCmdPtr)->leng );
			if (Temp_Data>64)
			 Temp_Data=64;
                     msg_ddc("EEPROMRead: leng = %d",(((DDC2Bi_EEPROMReadCmd_t*)DDC2BiCmdPtr)->leng));
			Temp_Data1=((DDC2Bi_EEPROMReadCmd_t*)DDC2BiCmdPtr)->addr;
                     Temp_Data1=SWAP_BYTES(Temp_Data1);	
                     NVRam_ReadBuffer( Temp_Data1,(((DDC2Bi_EEPROMRead_ReplyCmd_t*)gExtDebugCmdPtr)->data),Temp_Data);		
			 (((DDC2Bi_EEPROMRead_ReplyCmd_t*)gExtDebugCmdPtr)->length) = ((2+Temp_Data)| DDC2Bi_CONTROL_STATUS_FLAG);
                     (((DDC2Bi_EEPROMRead_ReplyCmd_t*)gExtDebugCmdPtr)->command) = DDC2B_CMD_NMV_EEPROM_Reply;
                     (((DDC2Bi_EEPROMRead_ReplyCmd_t*)gExtDebugCmdPtr)->eeprom_cmd) = DDC2B_CMD_NMV_EEPROMRead;
                    
	              break;

		case DDC2B_CMD_NMV_EEPROMWrite:
			Temp_Data=(((DDC2Bi_EEPROMWriteCmd_t*)DDC2BiCmdPtr)->leng);
			if(Temp_Data>32)
			Temp_Data=32;
			msg_ddc("EEPROMWrite: leng = %d",Temp_Data);
			Temp_Data1= (((DDC2Bi_EEPROMReadCmd_t*)DDC2BiCmdPtr)->addr);
                       Temp_Data1=SWAP_BYTES(Temp_Data1);
                        NVRam_WriteBuffer(Temp_Data1,(((DDC2Bi_EEPROMWriteCmd_t*)DDC2BiCmdPtr)->data),Temp_Data);					
 				
				(((DDC2Bi_EEPROMWrite_ReplyCmd_t*)gExtDebugCmdPtr)->length)=3 | DDC2Bi_CONTROL_STATUS_FLAG;
				(((DDC2Bi_EEPROMWrite_ReplyCmd_t*)gExtDebugCmdPtr)->command)=DDC2B_CMD_NMV_EEPROM_Reply;	
				(((DDC2Bi_EEPROMWrite_ReplyCmd_t*)gExtDebugCmdPtr)->eeprom_cmd)=DDC2B_CMD_NMV_EEPROMWrite;	
				 (((DDC2Bi_EEPROMWrite_ReplyCmd_t*)gExtDebugCmdPtr)->RC) = 0;
		      
		      break;
	  case DDC2B_CMD_NMV_ASSET_READ_Wistron : 
	  	   Temp_Data=(((DDC2Bi_AssetReadCmd_t*)DDC2BiCmdPtr)->leng );
			if (Temp_Data>64)
			 Temp_Data=64;
                  if ((((DDC2Bi_AssetReadCmd_t*)DDC2BiCmdPtr)->offset )==DDC2B_WISTRON_SERIAL_NUMBER)
                  	{
                  	  (((DDC2Bi_EEPROMRead_ReplyCmd_t*)gExtDebugCmdPtr)->length) = ((2+Temp_Data)| DDC2Bi_CONTROL_STATUS_FLAG);
                       (((DDC2Bi_EEPROMRead_ReplyCmd_t*)gExtDebugCmdPtr)->command) = DDC2B_CMD_NMV_EEPROM_Reply;
			   (((DDC2Bi_EEPROMRead_ReplyCmd_t*)gExtDebugCmdPtr)->eeprom_cmd) = DDC2B_WISTRON_SERIAL_NUMBER;
                  	   ReadMonitorSerialNumberfromNVRAM( ((DDC2Bi_EEPROMRead_ReplyCmd_t*)gExtDebugCmdPtr)->data);
                  	   //not ready
                  	}
			
                   if ((((DDC2Bi_AssetReadCmd_t*)DDC2BiCmdPtr)->offset )==DDC2B_WISTRON_BACKLIGHT_HOUR)  
                   	{
			               			 
			 (((DDC2Bi_EEPROMRead_ReplyCmd_t*)gExtDebugCmdPtr)->length) = ((2+Temp_Data)| DDC2Bi_CONTROL_STATUS_FLAG);
                     (((DDC2Bi_EEPROMRead_ReplyCmd_t*)gExtDebugCmdPtr)->command) = DDC2B_CMD_NMV_EEPROM_Reply;
					 
			  Temp_Data1=	UserPrefTimeUsedHour;
                   	 * (((DDC2Bi_EEPROMRead_ReplyCmd_t*)gExtDebugCmdPtr)->data)=SWAP_BYTES(Temp_Data1)&0x00FF;//Temp_Data1&0x00FF; 
			 * ((((DDC2Bi_EEPROMRead_ReplyCmd_t*)gExtDebugCmdPtr)->data)+1)=Temp_Data1&0x00FF;//SWAP_BYTES(Temp_Data1)&0x00FF; 	
			  * ((((DDC2Bi_EEPROMRead_ReplyCmd_t*)gExtDebugCmdPtr)->data)+2)=UserPrefTimeUsedMin;
                     (((DDC2Bi_EEPROMRead_ReplyCmd_t*)gExtDebugCmdPtr)->eeprom_cmd) = DDC2B_CMD_NMV_ASSET_READ_Wistron;
                   	}
	 
	  	   break;
          case DDC2B_CMD_NMV_ASSET_WRITE_Wistron:   
		  	if ((((DDC2Bi_AssetWriteCmd_t*)DDC2BiCmdPtr)->offset )==DDC2B_WISTRON_SERIAL_NUMBER)
                  	{
				 WriteMonitorSerialNumberintoNVRAM(((DDC2Bi_AssetWriteCmd_t*)DDC2BiCmdPtr)->data);
                  	   //not ready
                  	}
			
                   if ((((DDC2Bi_AssetWriteCmd_t*)DDC2BiCmdPtr)->offset )==DDC2B_WISTRON_BACKLIGHT_HOUR)  
                   	{
			UserPrefTimeUsedHour= * (((DDC2Bi_AssetWriteCmd_t*)DDC2BiCmdPtr)->data)<<8|* ((((DDC2Bi_AssetWriteCmd_t*)DDC2BiCmdPtr)->data)+1);
			UserPrefTimeUsedMin=* ((((DDC2Bi_AssetWriteCmd_t*)DDC2BiCmdPtr)->data)+2);
			SaveFactorySettingsInNVR();
                   	}
				   readExpected=0;
		  	break;
	           
	         }
      	}        
         break;
/////////////////////////////////////////////////////////////////////////////////////////////////  
/////////////////////////////////////////////////////////////////////////////////////////////////

      case DDC2B_CMD_NMV_GetVCPPage:
         msg_ddc("GetVCPPage",0);
         (((DDC2Bi_GetVCPPage_ReplyCmd_t*)gExtDebugCmdPtr)->length) = 0x03 |DDC2Bi_CONTROL_STATUS_FLAG;
         (((DDC2Bi_GetVCPPage_ReplyCmd_t*)gExtDebugCmdPtr)->command) = DDC2B_CMD_NMV_GetVCPPage_Reply;
         (((DDC2Bi_GetVCPPage_ReplyCmd_t*)gExtDebugCmdPtr)->vcp_page) = VCPPage;
         (((DDC2Bi_GetVCPPage_ReplyCmd_t*)gExtDebugCmdPtr)->vcp_maxpage) = DDC2B_Max_VCP_Pages;
         break;

      case DDC2B_CMD_NMV_GetPagedVCP: //NMV custom commands
         msg_ddc("GetPagedVCP",0);
         VCPPage = (((DDC2Bi_GetPagedVCPCmd_t*)DDC2BiCmdPtr)->vcp_page);
         VCPFeature.value_BigEndian = 0;
         VCPFeature.max_BigEndian = 0xFFFF;
         VCPFeature.vcp_opcode = (((DDC2Bi_GetPagedVCPCmd_t*)DDC2BiCmdPtr)->vcp_opcode);
         (((DDC2Bi_GetPagedVCP_ReplyCmd_t*)gExtDebugCmdPtr)->length) = 0x09 | DDC2Bi_CONTROL_STATUS_FLAG;
         (((DDC2Bi_GetPagedVCP_ReplyCmd_t*)gExtDebugCmdPtr)->command) = DDC2B_CMD_NMV_GetPagedVCP_Reply;
         (((DDC2Bi_GetPagedVCP_ReplyCmd_t*)gExtDebugCmdPtr)->result) = DDC2Bi_VCP_ParseCommand(GET_VCP);
         (((DDC2Bi_GetPagedVCP_ReplyCmd_t*)gExtDebugCmdPtr)->vcp_page) = VCPPage;
         (((DDC2Bi_GetPagedVCP_ReplyCmd_t*)gExtDebugCmdPtr)->b_VCPFeature)= VCPFeature;
         break;

      case DDC2B_CMD_NMV_SetPagedVCP:
         msg_ddc("SetPagedVCP",0);
         VCPPage = (((DDC2Bi_SetPagedVCPCmd_t*)DDC2BiCmdPtr)->vcp_page);
         VCPFeature.vcp_opcode = (((DDC2Bi_SetPagedVCPCmd_t*)DDC2BiCmdPtr)->vcp_opcode);
         VCPFeature.value_BigEndian    = (((DDC2Bi_SetPagedVCPCmd_t*)DDC2BiCmdPtr)->value_BigEndian);
         (((DDC2Bi_GetPagedVCP_ReplyCmd_t*)gExtDebugCmdPtr)->length) = 0x09 | DDC2Bi_CONTROL_STATUS_FLAG;
         (((DDC2Bi_GetPagedVCP_ReplyCmd_t*)gExtDebugCmdPtr)->command) = DDC2B_CMD_NMV_SetPagedVCP_Reply;
         (((DDC2Bi_GetPagedVCP_ReplyCmd_t*)gExtDebugCmdPtr)->result) = DDC2Bi_VCP_ParseCommand(SET_VCP);
         (((DDC2Bi_GetPagedVCP_ReplyCmd_t*)gExtDebugCmdPtr)->vcp_page) = VCPPage;
         (((DDC2Bi_GetPagedVCP_ReplyCmd_t*)gExtDebugCmdPtr)->b_VCPFeature) = VCPFeature;
         break;
#ifdef USE_XRite
	  case DDC2B_CMD_Xrite_Set:
		  VCPPage = 0; //select Page0
		  VCPFeature.vcp_opcode   = (((DDC2Bi_SetVCPFeatureCmd_t*)DDC2BiCmdPtr)->vcp_opcode);
		  VCPFeature.value_BigEndian	= (((DDC2Bi_SetVCPFeatureCmd_t*)DDC2BiCmdPtr)->val_BigEndian);
		  if(VCPFeature.vcp_opcode == DDC2B_CMD_Xrite_GetReadyToReceiveStatus)
		  {
			  (((DDC2Bi_CGetVCPFeature_ReplyCmd_t*)gExtDebugCmdPtr)->length) = 0x83 ;
			  (((DDC2Bi_CGetVCPFeature_ReplyCmd_t*)gExtDebugCmdPtr)->command) = 0xF1;
			  (((DDC2Bi_CGetVCPFeature_ReplyCmd_t*)gExtDebugCmdPtr)->vcp_opcode ) = DDC2B_CMD_Xrite_GetReadyToReceiveStatus;
			  if(MarsInBusyState == TRUE)
					(((DDC2Bi_CGetVCPFeature_ReplyCmd_t*)gExtDebugCmdPtr)->result) = 0xFF;
			  else	
			  (((DDC2Bi_CGetVCPFeature_ReplyCmd_t*)gExtDebugCmdPtr)->result) = 0x00;
			  msg_ddc1("Get VCP opcode: %x, res: %x", VCPFeature.vcp_opcode,(((DDC2Bi_CGetVCPFeature_ReplyCmd_t*)gExtDebugCmdPtr)->result));
		  }
		  else
		  {
			  DDC2Bi_VCP_ParseCommand_XRite(gExtDebugCmdPtr);
			  *(BYTE far*)gExtDebugCmdPtr = 0x80;
			  readExpected = 0;
			  msg_ddc1("Set XRite VCP opcode: %x, res: %x", VCPFeature.vcp_opcode, VCPFeature.value_BigEndian);
		  }
		  break;
		  
		case DDC2B_CMD_Xrite_Get:
      {
			 WORD DW_NvramAddr;
			 BYTE Bp_Buf[12];
			 VCPFeature.vcp_opcode = (((DDC2Bi_GetVCPFeatureCmd_t*)DDC2BiCmdPtr)->vcp_opcode);
			 if(VCPFeature.vcp_opcode==DDC2B_CMD_Xrite_GetSerialNumber)
			 {
				 (((DDC2Bi_CGetVCPFeature_ReplyCmd_t*)gExtDebugCmdPtr)->length) = 0x8E;
				 (((DDC2Bi_CGetVCPFeature_ReplyCmd_t*)gExtDebugCmdPtr)->command) = 0xF0;
				 (((DDC2Bi_CGetVCPFeature_ReplyCmd_t*)gExtDebugCmdPtr)->vcp_opcode ) = DDC2B_CMD_Xrite_GetSerialNumber;
				 DW_NvramAddr = DpEdidTableStart+0x4D;
				 NVRam_ReadBuffer(DW_NvramAddr,Bp_Buf,12);
				 _fmemcpy(&(((DDC2Bi_CGetVCPFeature_ReplyCmd_t*)gExtDebugCmdPtr)->result),
						  &Bp_Buf[0], 12);
				 msg_ddc1("Xrite Get VCP opcode: %x, res: %x", VCPFeature.vcp_opcode,
					  (((DDC2Bi_CGetVCPFeature_ReplyCmd_t*)gExtDebugCmdPtr)->result));
			 }

			 if(VCPFeature.vcp_opcode==DDC2B_CMD_Xrite_GetDisplayTime)
			 {
				 (((DDC2Bi_CGetVCPFeature_ReplyCmd_t*)gExtDebugCmdPtr)->length) = 0x8E;
				 (((DDC2Bi_CGetVCPFeature_ReplyCmd_t*)gExtDebugCmdPtr)->command) = 0xF0;
				 (((DDC2Bi_CGetVCPFeature_ReplyCmd_t*)gExtDebugCmdPtr)->vcp_opcode ) = DDC2B_CMD_Xrite_GetDisplayTime;
				 Bp_Buf[0]=UserPrefTimeUsedHour & 0xFF;
				 Bp_Buf[1]=(UserPrefTimeUsedHour & 0xFF00) >>8;			 
				 Bp_Buf[2]=UserPrefTimeUsedMin;
				 _fmemcpy(&(((DDC2Bi_CGetVCPFeature_ReplyCmd_t*)gExtDebugCmdPtr)->result),
						  &Bp_Buf[0], 3);
				 msg_ddc1("Xrite Get DDC2B_CMD_Xrite_GetDisplayTime: %x, res: %x", VCPFeature.vcp_opcode,
             (((DDC2Bi_CGetVCPFeature_ReplyCmd_t*)gExtDebugCmdPtr)->result));
			 }
			 break;
      }
#endif		 
#ifdef USE_COLOR_FIDELITY
#if USE_DYNAMIC_GAMUTMAPPING_SUPPORT
      case DDC2B_CMD_Table_Write:
         {
            BYTE cmd;
            //BYTE wgs_Mode = *(((BYTE*)DDC2BiCmdPtr)+5);

            cmd = ((((DDC2Bi_WGS_VCPCmd_t*)gExtDebugCmdPtr)->mode) & 0x1) || ((((DDC2Bi_WGS_VCPCmd_t*)gExtDebugCmdPtr)->mode)==WGSM_DISPLAY_COLOR);
            //cmd = ((wgs_Mode & 0x1) || (wgs_Mode == WGSM_DISPLAY_COLOR));

            gm_Print(">> Mode1 = 0x%x",(((DDC2Bi_WGS_VCPCmd_t*)gExtDebugCmdPtr)->mode));
            gm_Print(">> Mode2 = 0x%x",(((DDC2Bi_WGS_VCPCmd_t*)DDC2BiCmdPtr)->mode));

            DDC2Bi_TableRw_ParseCommand(gExtDebugCmdPtr, cmd);

            (((DDC2Bi_WGS_ReplyCmd_t*)DDC2BiCmdPtr)->command)  = DDC2B_CMD_Table_Reply;
            //*(((BYTE*)DDC2BiCmdPtr)+1) = DDC2B_CMD_Table_Reply;

            msg_ddc("TableWrite VCP",0);

            break;
         }
      case DDC2B_CMD_Table_Read:
         {
            msg_ddc("TableRead VCP",0);
            break;
         }
#endif
#endif // USE_COLOR_FIDELITY

      case DDC2B_CMD_EH_FactoryCommand:
	{
            EH_FactoryCommand(gExtDebugCmdPtr);
			break;
      	}
	  
      default:
         *(BYTE far*)gExtDebugCmdPtr = 0x80; //no messages back
         readExpected = 0; 
         msg_ddc("Unsupported DDC2ci Command [%d]", *((BYTE*)gExtDebugCmdPtr+2));
   }
#if (FEATURE_DISPLAY_PORT == ENABLE)
   if (handler == AUX_C0_USE || handler == AUX_C2_USE)
   	{
	   /*ChengLin: 20120309: For Fix system shutdown when operate DP MCCS Set commands*/   	
      if(readExpected)
      {
      DpRxDdcciAuxSetReply((BYTE*)gExtDebugCmdPtr,handler-AUX_C0_USE);
      }
	  else
	  {
		pSW_Transaction_ModuleDataGlobal[handler-AUX_C0_USE]->W_CurCount = 0;  
	  }
   	}
   else
#endif //(FEATURE_DISPLAY_PORT == ENABLE)

#if 1
   if((handler == I2C_SLAVE1_USE)||(handler == I2C_SLAVE2_USE)||
      (handler == I2C_SLAVE3_USE)||(handler == I2C_SLAVE4_USE))
   {
      BYTE B_SlaveCh = handler - I2C_SLAVE1_USE;
      BYTE B_Length = (*(BYTE*)gExtDebugCmdPtr) & 0x7F;
      BYTE B_Checksum, i;
      
      B_Checksum = ((DDC2B_SRC_ADDRESS-1) ^ (Sp_I2CSlavePtrGlobal[B_SlaveCh]->B_DevID & I2C_ADDRESS_BITS));
      for(i = 0; i <= B_Length; i++)            // Process the whole packet 
          B_Checksum ^= ((BYTE far*)gExtDebugCmdPtr)[i];  // Update checksum 

      ((BYTE far*)gExtDebugCmdPtr)[B_Length+1] = B_Checksum;

      if(EndCapStrFlag == 1)  //fill zeros for WFIFO's spare space
      {
         BYTE B_Cnt;
         for(B_Cnt=0; B_Cnt<W_FillZerosNum+3; B_Cnt++)
            *(&(((DdcciCapabilitiesReply*)gExtDebugCmdPtr)->data)+W_FillDataNum+B_Cnt) = 0;
      }

      if(readExpected)
      {
         if(EndCapStrFlag)
         {
            gm_I2CSlaveSetReply(DDCCI_MSG_SIZE+6, B_SlaveCh);  //Last cap string fragment with zeros as ending
            EndCapStrFlag = 0;
         }
         else
         {
            gm_I2CSlaveSetReply(B_Length+3, B_SlaveCh);
         }
      }
      else
      {
         //Prepare null message for unsupported commands & unexpected read
         gm_I2CSlaveSetReply(B_Length+3, B_SlaveCh);
         Sp_I2CSlavePtrGlobal[B_SlaveCh]->W_RcvCount = 0;
         Sp_I2CSlavePtrGlobal[B_SlaveCh]->W_AbsRcvCount = 0;
      }
   }
#else
   if(handler == DDC2BI_USE)
      gm_DDC2BiWrite((BYTE*)gExtDebugCmdPtr+1, (*(BYTE*)gExtDebugCmdPtr) & 0x7F);
#endif
}


//******************************************************************************
//
// FUNCTION       : BYTE DDC2Bi_VCP_ParseCommand(BYTE SetVcp)
//
// USAGE          : VCP Commands Parser.
//
// INPUT          : SetVcp
//
// OUTPUT         : Returns 1 if feature is supported and 0 otherwise.
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
#if (defined(ST_4K2K_93xx_BOARD) && (defined(Athena_4K2K_Chip_FE)) )

BYTE DDC2Bi_VCP_ParseCommand(BYTE SetVcp)
{
   WORD W_Temp;
   BYTE B_Temp;
   BOOL IsFeatureSupported = TRUE;

   //default values for Max and Current Values
   WORD W_VCPValue = 0;
   //WORD W_VCPMax = 0x100;
   WORD W_VCPMax = 0xFFFF;
   BYTE B_VCPType = 0x01;
   ModeDependentLookUpType LookUpType;
   BYTE DDC_Cmd_Buff[3]={0};

   //converts values to Little Endian
   if (SetVcp)
      W_VCPValue = EndianConv(VCPFeature.value_BigEndian);

   if (VCPPage == 0)
   {
      switch (VCPFeature.vcp_opcode)
      {
         case DDC2B_CMD_VCP_NewControlVal: //To make MCCS 2.0 Compliant
            {
               static BYTE B_DDCCINewControlVal;
               if (SetVcp == SET_VCP && W_VCPValue)
               {
                  W_VCPValue = NoNewControlVal;
                  B_DDCCINewControlVal = 0;
               }
               else if (SetVcp == GET_VCP)
               {  //Check for modeIndependent & mode dependent variables saved in NVRAM
                  W_VCPValue = B_DDCCINewControlVal ? NewControlVal: NoNewControlVal; // 2 - a new control value has been saved
                  msg_ddc("NewControlVal: %d", W_VCPValue);
               }
               break;
            }

         case DDC2B_CMD_VCP_MCCSVersion: //To make MCCS 2.0 Compliant
            if (SetVcp == GET_VCP)
            {
               W_VCPValue = VCP_Version; //Version 2, revision 0 of VESA Monitor control command set document
               msg_ddc("MCCSVersion: %d", W_VCPValue);
            }
            break;

         case DDC2B_CMD_VCP_HFreq: //To make MCCS 2.0 Compliant
            W_VCPValue = stInput[gmvb_MainChannel].Hfrq*100;
            W_VCPMax = (WORD)(((DWORD)stInput[gmvb_MainChannel].Hfrq*100) >> 16); // W_VCPMax is the high byte
            msg_ddc("DDC2B_CMD_VCP_HFreq %d", W_VCPValue);
            break;

         case DDC2B_CMD_VCP_VFreq:   //To make MCCS 2.0 Compliant
            W_VCPValue = stInput[gmvb_MainChannel].Vfrq*10;
            msg_ddc("DDC2B_CMD_VCP_VFreq %d", W_VCPValue);
            break;

         case DDC2B_CMD_VCP_MonitorType:
            W_VCPValue = DispTechnologyType; //To make MCCS 2.0 Compliant
            break;

         case DDC2B_CMD_VCP_DisplayUsageTime:
            {
               if (!SetVcp)
               {
                  extern DWORD DW_MonitorPowerOnTime;

                  W_VCPValue = (WORD)(DW_MonitorPowerOnTime/60); // A 2byte value, DW_MonitorPowerOnTime is in minute
               }
               break;
            }

         case DDC2B_CMD_VCP_ApplnEnKey: //To make MCCS 2.0 Compliant
            W_VCPValue = ApplnEnableKey; //A 2byte value
            break;

         case DDC2B_CMD_VCP_DispControllerType:
            //byte 1 = 2 for Genesis Controller, byte 2 = controller type (product_ID)
            W_VCPValue = DispControllerType | ((WORD)gm_ReadRegByte(PRODUCT_ID) << 8); // 
            W_VCPMax = gm_ReadRegWord(PRODUCT_ID) >> 8;
            //Byte 3 - provide a numeric indication of controller type (Rev ID) - ???
            break;

         case DDC2B_CMD_VCP_DispFirmwareLevel:
            W_VCPValue = 0x201; //A 2byte value - f/w level installed in the display
            break;

         case DDC2B_CMD_VCP_ALLReset:
            if (SetVcp && W_VCPValue)
            {
               msg_ddc( "Factory Reset", 0);
				//Send SocIPC DDC/CI Command to Athena B/C
				DDC_Cmd_Buff[0] = DDC2B_CMD_VCP_ALLReset;
				DDC_Cmd_Buff[1] = 0;
				DDC_Cmd_Buff[2] = 0;
					
				if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEB_DST_ADDRESS, SICMD_RW_DDCI_CONTRL, DDC_Cmd_Buff) == FALSE)
					msg_ddc("\n<Soc IPC Error> Device B DDC Send Reset Command Failure.\n",0);

				if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEC_DST_ADDRESS, SICMD_RW_DDCI_CONTRL, DDC_Cmd_Buff) == FALSE)
					msg_ddc("\n<Soc IPC Error> Device C DDC Send Reset Command Failure.\n",0);					
            }
            else
            {
				//DDC/CI handled by Athena A and system will always return Athrna B status via DDC/CI protocol
				//Available Sequences: Athena B-> Athena C -> Athena A
				DDC_Cmd_Buff[0] = DDC2B_CMD_VCP_ALLReset;

				if(SocIPCCheckDevice(SOCIPC_DEVICEB_DST_ADDRESS))
				{
					//Athena-B
					if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEB_DST_ADDRESS, SICMD_RO_DDCI_Status, DDC_Cmd_Buff) == FALSE)
					{
						W_VCPMax = 1;
               			B_VCPType = 1;						
						msg_ddc("\n<Soc IPC Error> [GetVCP]Device B DDC Send Reset Command Failure.\n",0);
					}
					else
					{
						W_VCPMax = gAthena_B_C_Parameters.DDC_VCP_Max;
               			B_VCPType = gAthena_B_C_Parameters.DDC_VCP_Type;	
						W_VCPValue = gAthena_B_C_Parameters.DDC_Value;
					}
				}
				else
				{
					//Athena-C
					if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEC_DST_ADDRESS, SICMD_RO_DDCI_Status, DDC_Cmd_Buff) == FALSE)
					{
						W_VCPMax = 1;
						B_VCPType = 1;
						msg_ddc("\n<Soc IPC Error> [GetVCP]Device C DDC Send Reset Command Failure.\n",0);
					}
					else
					{
						W_VCPMax = gAthena_B_C_Parameters.DDC_VCP_Max;
               			B_VCPType = gAthena_B_C_Parameters.DDC_VCP_Type;	
						W_VCPValue = gAthena_B_C_Parameters.DDC_Value;
					}					
				}
            }
            break;

#ifdef ADJUSTER_BRIGHTNESS_DECLARED
         case DDC2B_CMD_VCP_RestoreContrastBrightness:
            {
               if (SetVcp && W_VCPValue)
               {
				//Send SocIPC DDC/CI Command to Athena B/C
				DDC_Cmd_Buff[0] = DDC2B_CMD_VCP_RestoreContrastBrightness;
				DDC_Cmd_Buff[1] = 0;
				DDC_Cmd_Buff[2] = 0;
	
				if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEB_DST_ADDRESS, SICMD_RW_DDCI_CONTRL, DDC_Cmd_Buff) == FALSE)
					msg_ddc("\n<Soc IPC Error> Device B DDC Send Restore Contrast Brightness Command Failure.\n",0);

				if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEC_DST_ADDRESS, SICMD_RW_DDCI_CONTRL, DDC_Cmd_Buff) == FALSE)
					msg_ddc("\n<Soc IPC Error> Device C DDC Send Restore Contrast Brightness Command Failure.\n",0);					
               }
               else
               {
					//DDC/CI handled by Athena A and system will always return Athrna B status via DDC/CI protocol
					//Available Sequences: Athena B-> Athena C -> Athena A
									  	  
					DDC_Cmd_Buff[0] = DDC2B_CMD_VCP_RestoreContrastBrightness;
									  
					if(SocIPCCheckDevice(SOCIPC_DEVICEB_DST_ADDRESS))
					{
						//Athena-B
						if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEB_DST_ADDRESS, SICMD_RO_DDCI_Status, DDC_Cmd_Buff) == FALSE)
						{
							W_VCPMax = 1;				  
							msg_ddc("\n<Soc IPC Error> [GetVCP]Device B DDC Send Restore Contrast Brightness Command Failure.\n",0);
						}
						else
						{
							W_VCPMax = gAthena_B_C_Parameters.DDC_VCP_Max;
							B_VCPType = gAthena_B_C_Parameters.DDC_VCP_Type;    
							W_VCPValue = gAthena_B_C_Parameters.DDC_Value;
						}
					}
					else
					{
						//Athena-C
						if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEC_DST_ADDRESS, SICMD_RO_DDCI_Status, DDC_Cmd_Buff) == FALSE)
						{
                  			W_VCPMax = 1;
							msg_ddc("\n<Soc IPC Error> [GetVCP]Device C DDC Send Restore Contrast Brightness Command Failure.\n",0);
						}
						else
						{
							W_VCPMax = gAthena_B_C_Parameters.DDC_VCP_Max;
							B_VCPType = gAthena_B_C_Parameters.DDC_VCP_Type;    
							W_VCPValue = gAthena_B_C_Parameters.DDC_Value;
						}				  
					}
               }
               break;
            }
#endif

#ifdef ADJUSTER_USERREDGAIN_DECLARED
         case DDC2B_CMD_VCP_ColorReset:
            {
               if (SetVcp && W_VCPValue)
               {
				//Send SocIPC DDC/CI Command to Athena B/C
				DDC_Cmd_Buff[0] = DDC2B_CMD_VCP_ColorReset;
				DDC_Cmd_Buff[1] = 0;
				DDC_Cmd_Buff[2] = 0;
	
				if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEB_DST_ADDRESS, SICMD_RW_DDCI_CONTRL, DDC_Cmd_Buff) == FALSE)
					msg_ddc("\n<Soc IPC Error> Device B DDC Send Color Reset Command Failure.\n",0);

				if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEC_DST_ADDRESS, SICMD_RW_DDCI_CONTRL, DDC_Cmd_Buff) == FALSE)
					msg_ddc("\n<Soc IPC Error> Device C DDC Send Color Reset Command Failure.\n",0);					
               }
               else
               {
				//DDC/CI handled by Athena A and system will always return Athrna B status via DDC/CI protocol
				//Available Sequences: Athena B-> Athena C -> Athena A
														 									 
				DDC_Cmd_Buff[0] = DDC2B_CMD_VCP_ColorReset;
														 
				if(SocIPCCheckDevice(SOCIPC_DEVICEB_DST_ADDRESS))
				{
					//Athena-B
					if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEB_DST_ADDRESS, SICMD_RO_DDCI_Status, DDC_Cmd_Buff) == FALSE)
					{
                  		W_VCPMax = 1;
						msg_ddc("\n<Soc IPC Error> [GetVCP]Device B DDC Send Color Reset Command Failure.\n",0);
					}
					else
					{
						W_VCPMax = gAthena_B_C_Parameters.DDC_VCP_Max;
						B_VCPType = gAthena_B_C_Parameters.DDC_VCP_Type;    
						W_VCPValue = gAthena_B_C_Parameters.DDC_Value;
					}
				}
				else
				{
					//Athena-C
					if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEC_DST_ADDRESS, SICMD_RO_DDCI_Status, DDC_Cmd_Buff) == FALSE)
					{
						W_VCPMax = 1;				 
						msg_ddc("\n<Soc IPC Error> [GetVCP]Device C DDC Send Color Reset Command Failure.\n",0);
					}
					else
					{
						W_VCPMax = gAthena_B_C_Parameters.DDC_VCP_Max;
						B_VCPType = gAthena_B_C_Parameters.DDC_VCP_Type;    
						W_VCPValue = gAthena_B_C_Parameters.DDC_Value;
					}				 
				}
               }
               break;
            }
#endif

#ifdef ADJUSTER_BRIGHTNESS_DECLARED
         case DDC2B_CMD_VCP_Brightness:
            {
               if (SetVcp)
               {
				//Send SocIPC DDC/CI Command to Athena B/C
				DDC_Cmd_Buff[0] = DDC2B_CMD_VCP_Brightness;
				DDC_Cmd_Buff[1] = (BYTE)(W_VCPValue>>8);
				DDC_Cmd_Buff[2] = (BYTE)(W_VCPValue & 0x00FF);
					
				if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEB_DST_ADDRESS, SICMD_RW_DDCI_CONTRL, DDC_Cmd_Buff) == FALSE)
					msg_ddc("\n<Soc IPC Error> Device B DDC Send Brightness Command Failure.\n",0);

				if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEC_DST_ADDRESS, SICMD_RW_DDCI_CONTRL, DDC_Cmd_Buff) == FALSE)
					msg_ddc("\n<Soc IPC Error> Device C DDC Send Brightness Command Failure.\n",0);					
               }
               else
               {
					
				//DDC/CI handled by Athena A and system will always return Athrna B status via DDC/CI protocol
				//Available Sequences: Athena B-> Athena C -> Athena A
				   
				DDC_Cmd_Buff[0] = DDC2B_CMD_VCP_Brightness;
				   
				if(SocIPCCheckDevice(SOCIPC_DEVICEB_DST_ADDRESS))
				{
					//Athena-B
					if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEB_DST_ADDRESS, SICMD_RO_DDCI_Status, DDC_Cmd_Buff) == FALSE)
					{
						W_VCPValue = UserPrefBrightness - (WORD)gm_GetAdjusterMin(ADJV_Brightness);
						W_VCPMax = (WORD)gm_GetAdjusterMax(ADJV_Brightness) - (WORD)gm_GetAdjusterMin(ADJV_Brightness);
						B_VCPType = 1;					   
						msg_ddc("\n<Soc IPC Error> [GetVCP]Device B DDC Send Brightness Command Failure.\n",0);
					}
					else
					{
						W_VCPMax = gAthena_B_C_Parameters.DDC_VCP_Max;
						B_VCPType = gAthena_B_C_Parameters.DDC_VCP_Type;    
						W_VCPValue = gAthena_B_C_Parameters.DDC_Value;
					}
				}
				else
				{
					//Athena-C
					if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEC_DST_ADDRESS, SICMD_RO_DDCI_Status, DDC_Cmd_Buff) == FALSE)
					{
						W_VCPValue = UserPrefBrightness - (WORD)gm_GetAdjusterMin(ADJV_Brightness);
						W_VCPMax = (WORD)gm_GetAdjusterMax(ADJV_Brightness) - (WORD)gm_GetAdjusterMin(ADJV_Brightness);
						B_VCPType = 1;					   
						msg_ddc("\n<Soc IPC Error> [GetVCP]Device C DDC Send Brightness Command Failure.\n",0);
					}
					else
					{
						W_VCPMax = gAthena_B_C_Parameters.DDC_VCP_Max;
						B_VCPType = gAthena_B_C_Parameters.DDC_VCP_Type;    
						W_VCPValue = gAthena_B_C_Parameters.DDC_Value;
					}				   
				}	
               }
               break;
            }
#endif

#ifdef ADJUSTER_RBLACKLEVEL_DECLARED
         case DDC2B_CMD_VCP_Red_Video_Black_Level:
            {
            }
#endif
#ifdef ADJUSTER_GBLACKLEVEL_DECLARED
         case DDC2B_CMD_VCP_Green_Video_Black_Level:
            {
            }
#endif
#ifdef ADJUSTER_BBLACKLEVEL_DECLARED
         case DDC2B_CMD_VCP_Blue_Video_Black_Level:
            {
            }
#endif
#ifdef ADJUSTER_CONTRAST_DECLARED
         case DDC2B_CMD_VCP_Contrast:
            {
               if (SetVcp)
               {
				//Send SocIPC DDC/CI Command to Athena B/C
				DDC_Cmd_Buff[0] = DDC2B_CMD_VCP_Contrast;
				DDC_Cmd_Buff[1] = (BYTE)(W_VCPValue>>8);
				DDC_Cmd_Buff[2] = (BYTE)(W_VCPValue & 0x00FF);
									
				if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEB_DST_ADDRESS, SICMD_RW_DDCI_CONTRL, DDC_Cmd_Buff) == FALSE)
					msg_ddc("\n<Soc IPC Error> Device B DDC Send Contrast Command Failure.\n",0);
				
				if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEC_DST_ADDRESS, SICMD_RW_DDCI_CONTRL, DDC_Cmd_Buff) == FALSE)
					msg_ddc("\n<Soc IPC Error> Device C DDC Send Contrast Command Failure.\n",0);					
               }
               else
               {
				//DDC/CI handled by Athena A and system will always return Athrna B status via DDC/CI protocol
				//Available Sequences: Athena B-> Athena C -> Athena A
														 								 
				DDC_Cmd_Buff[0] = DDC2B_CMD_VCP_Contrast;
														 
				if(SocIPCCheckDevice(SOCIPC_DEVICEB_DST_ADDRESS))
				{
						//Athena-B
						if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEB_DST_ADDRESS, SICMD_RO_DDCI_Status, DDC_Cmd_Buff) == FALSE)
						{
                  			W_VCPValue = UserPrefContrast - (WORD)gm_GetAdjusterMin(ADJV_Contrast);
                  			W_VCPMax = (WORD)gm_GetAdjusterMax(ADJV_Contrast) - (WORD)gm_GetAdjusterMin(ADJV_Contrast);
                  			B_VCPType = 0;			 
							msg_ddc("\n<Soc IPC Error> [GetVCP]Device B DDC Send Contrast Command Failure.\n",0);
						}
						else
						{
							W_VCPMax = gAthena_B_C_Parameters.DDC_VCP_Max;
							B_VCPType = gAthena_B_C_Parameters.DDC_VCP_Type;    
							W_VCPValue = gAthena_B_C_Parameters.DDC_Value;
						}
				}
				else
				{
					//Athena-C
					if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEC_DST_ADDRESS, SICMD_RO_DDCI_Status, DDC_Cmd_Buff) == FALSE)
					{
                  		W_VCPValue = UserPrefContrast - (WORD)gm_GetAdjusterMin(ADJV_Contrast);
                  		W_VCPMax = (WORD)gm_GetAdjusterMax(ADJV_Contrast) - (WORD)gm_GetAdjusterMin(ADJV_Contrast);
                  		B_VCPType = 0;			 
						msg_ddc("\n<Soc IPC Error> [GetVCP]Device C DDC Send Contrast Command Failure.\n",0);
					}
					else
					{
						W_VCPMax = gAthena_B_C_Parameters.DDC_VCP_Max;
						B_VCPType = gAthena_B_C_Parameters.DDC_VCP_Type;    
						W_VCPValue = gAthena_B_C_Parameters.DDC_Value;
					}				 
				}
               }
               break;
            }
#endif
#ifdef UserPrefColorTemp
         case DDC2B_CMD_VCP_SelectColorPreset:
            {
               if (SetVcp)
               {
				//Send SocIPC DDC/CI Command to Athena B/C			
				DDC_Cmd_Buff[0] = DDC2B_CMD_VCP_SelectColorPreset;
				DDC_Cmd_Buff[1] = (BYTE)(W_VCPValue>>8);
				DDC_Cmd_Buff[2] = (BYTE)(W_VCPValue & 0x00FF);
									
				if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEB_DST_ADDRESS, SICMD_RW_DDCI_CONTRL, DDC_Cmd_Buff) == FALSE)
					msg_ddc("\n<Soc IPC Error> Device B DDC Send Color Preset Command Failure.\n",0);
				
				if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEC_DST_ADDRESS, SICMD_RW_DDCI_CONTRL, DDC_Cmd_Buff) == FALSE)
					msg_ddc("\n<Soc IPC Error> Device C DDC Send Color Preset Command Failure.\n",0);					
               }
               else
               {
				//DDC/CI handled by Athena A and system will always return Athrna B status via DDC/CI protocol
				//Available Sequences: Athena B-> Athena C -> Athena A
																																	
				DDC_Cmd_Buff[0] = DDC2B_CMD_VCP_SelectColorPreset;
																			
				if(SocIPCCheckDevice(SOCIPC_DEVICEB_DST_ADDRESS))
				{
					//Athena-B
					if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEB_DST_ADDRESS, SICMD_RO_DDCI_Status, DDC_Cmd_Buff) == FALSE)
					{
                  		switch (UserPrefColorTemp)
                  		{
                     		case CTEMP_6500K:
                        		W_VCPValue = DDCCI_CTEMP_6500K;
                        		break;
                                 
                     		case CTEMP_7500K:
                        		W_VCPValue = DDCCI_CTEMP_7500K;
                        		break;                                 
                                 
                    	 	case CTEMP_9300K:
                        		W_VCPValue = DDCCI_CTEMP_9300K;
                        		break;
                                 
                     		//case CTEMP_11500K:
                        	//	W_VCPValue = DDCCI_CTEMP_11500K;
                        	//	break;
                        	/*case CTEMP_USER:
                            				W_VCPValue = DDCCI_CTEMP_USER1;
                            				UserPrefColorTemp
                            				break;*/
                        	//The below default task was added to pass Windows DTM
                        	//Without it, DTM tries to set UserPrefColorTemp to 0
                        	//As well the above case was commented out to pass DTM
                       	 	//This is a known Microsoft bug
                     		default:
                        		W_VCPValue = DDCCI_CTEMP_6500K;
                        		break;
                  		}
                  		B_VCPType    = 0x01;    //Set Parameter
#if 1
                  		W_VCPMax = 0xFFFF;    //this is the number of possible choices for this non-continuous VCP value
#else
                  		W_VCPMax = 3;    //this is the number of possible choices for this non-continuous VCP value
#endif			
						msg_ddc("\n<Soc IPC Error> [GetVCP]Device B DDC Send Color Preset Command Failure.\n",0);
					}
					else
					{
						W_VCPMax = gAthena_B_C_Parameters.DDC_VCP_Max;
						B_VCPType = gAthena_B_C_Parameters.DDC_VCP_Type;    
						W_VCPValue = gAthena_B_C_Parameters.DDC_Value;
					}
				}
				else
				{
					//Athena-C
					if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEC_DST_ADDRESS, SICMD_RO_DDCI_Status, DDC_Cmd_Buff) == FALSE)
					{
                  		switch (UserPrefColorTemp)
                  		{
                     		case CTEMP_6500K:
                        		W_VCPValue = DDCCI_CTEMP_6500K;
                        		break;
                                 
                     		case CTEMP_7500K:
                        		W_VCPValue = DDCCI_CTEMP_7500K;
                        		break;
                                 
                     		case CTEMP_9300K:
                        		W_VCPValue = DDCCI_CTEMP_9300K;
                        		break;
                                 
                     		//case CTEMP_11500K:
                        	//	W_VCPValue = DDCCI_CTEMP_11500K;
                        	//	break;
                        	/*case CTEMP_USER:
                            				W_VCPValue = DDCCI_CTEMP_USER1;
                            				UserPrefColorTemp
                            				break;*/
                        	//The below default task was added to pass Windows DTM
                        	//Without it, DTM tries to set UserPrefColorTemp to 0
                        	//As well the above case was commented out to pass DTM
                        	//This is a known Microsoft bug
                     		default:
                        		W_VCPValue = DDCCI_CTEMP_6500K;
                        		break;
                  		}
                  		B_VCPType    = 0x01;    //Set Parameter
#if 1
                  		W_VCPMax = 0xFFFF;    //this is the number of possible choices for this non-continuous VCP value
#else
                  		W_VCPMax = 3;    //this is the number of possible choices for this non-continuous VCP value
#endif		
						msg_ddc("\n<Soc IPC Error> [GetVCP]Device C DDC Send Color Preset Command Failure.\n",0);
					}
					else
					{
						W_VCPMax = gAthena_B_C_Parameters.DDC_VCP_Max;
						B_VCPType = gAthena_B_C_Parameters.DDC_VCP_Type;    
						W_VCPValue = gAthena_B_C_Parameters.DDC_Value;
					}				
				}
               }
               break;
            }
#endif
#ifdef Action_HideOsd_WB_Used
         case DDC2B_CMD_VCP_OSD:
            {
               if (SetVcp)
               {
				//Send SocIPC DDC/CI Command to Athena B/C			
				DDC_Cmd_Buff[0] = DDC2B_CMD_VCP_OSD;
				DDC_Cmd_Buff[1] = (BYTE)(W_VCPValue>>8);
				DDC_Cmd_Buff[2] = (BYTE)(W_VCPValue & 0x00FF);
													
				if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEB_DST_ADDRESS, SICMD_RW_DDCI_CONTRL, DDC_Cmd_Buff) == FALSE)
					msg_ddc("\n<Soc IPC Error> Device B DDC Send OSD Command Failure.\n",0);
								
				if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEC_DST_ADDRESS, SICMD_RW_DDCI_CONTRL, DDC_Cmd_Buff) == FALSE)
					msg_ddc("\n<Soc IPC Error> Device C DDC Send OSD Command Failure.\n",0);					
               }
               else
               {
				//DDC/CI handled by Athena A and system will always return Athrna B status via DDC/CI protocol
				//Available Sequences: Athena B-> Athena C -> Athena A
														 					 
				DDC_Cmd_Buff[0] = DDC2B_CMD_VCP_OSD;
														 
				if(SocIPCCheckDevice(SOCIPC_DEVICEB_DST_ADDRESS))
				{
					//Athena-B
					if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEB_DST_ADDRESS, SICMD_RO_DDCI_Status, DDC_Cmd_Buff) == FALSE)
					{
                  		B_VCPType = 0;
                  		W_VCPMax = 2;		 
						msg_ddc("\n<Soc IPC Error> [GetVCP]Device B DDC Send OSD Command Failure.\n",0);
					}
					else
					{
						W_VCPMax = gAthena_B_C_Parameters.DDC_VCP_Max;
						B_VCPType = gAthena_B_C_Parameters.DDC_VCP_Type;    
						W_VCPValue = gAthena_B_C_Parameters.DDC_Value;
					}
				}
				else
				{
					//Athena-C
					if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEC_DST_ADDRESS, SICMD_RO_DDCI_Status, DDC_Cmd_Buff) == FALSE)
					{
                  		B_VCPType = 0;
                  		W_VCPMax = 2;			 
						msg_ddc("\n<Soc IPC Error> [GetVCP]Device C DDC Send OSD Command Failure.\n",0);
					}
					else
					{
						W_VCPMax = gAthena_B_C_Parameters.DDC_VCP_Max;
						B_VCPType = gAthena_B_C_Parameters.DDC_VCP_Type;    
						W_VCPValue = gAthena_B_C_Parameters.DDC_Value;
					}				 
				}
               }
               break;
            }
#endif

/*ChengLin 20120312: Disable this feature due to 4k2k platform don't support VGA source.*/
#if 0//FEATURE_AIP == ENABLE 

#ifdef ADJUSTER_HSTART_DECLARED
         case DDC2B_CMD_VCP_HPosition:
            {            
               if (SetVcp)
               {           
                  msg_ddc("Set H Position [%d]", W_VCPValue);
                  UserPrefHSyncDelay = MaxHStart() - W_VCPValue;
                  AdjustHStart();			  
               }
               else
               {
                  W_VCPValue = MaxHStart() - UserPrefHSyncDelay;
                  W_VCPMax = MaxHStart() -MinHStart();
               }
               break;
            }
#endif

#ifdef ADJUSTER_VSTART_DECLARED
         case DDC2B_CMD_VCP_VPosition:
            {
               if (SetVcp)
               {            
                  msg_ddc("Set V Position [%d]", W_VCPValue);
                  UserPrefVSyncDelay = W_VCPValue + MinVStart();
                  AdjustVStart();			  
               }
               else
               {
                  W_VCPValue = UserPrefVSyncDelay - MinVStart();
                  W_VCPMax = MaxVStart() - MinVStart();
               }
               break;
            }
#endif

#ifdef Action_AdjustHTotal_WB_Used
         case DDC2B_CMD_VCP_Clock:
            {
               if (SetVcp)
               {              
                  UserPrefHTotal = W_VCPValue + MinHTotal();
                  AdjustHTotal();			  
               }
               else
               {
                  W_VCPValue = UserPrefHTotal - MinHTotal();
                  W_VCPMax = MaxHTotal() - MinHTotal();
               }
               break;
            }
#endif

#ifdef Action_AdjustHSyncPhase_WB_Used
         case DDC2B_CMD_VCP_ClockPhase:
            {
               if (SetVcp)
               {             
                  UserPrefHSyncPhase = W_VCPValue;
                  AdjustHSyncPhase();			  
               }
               else
               {
                  W_VCPValue = UserPrefHSyncPhase;
                  W_VCPMax = 0x3F;
               }
               break;
            }
#endif
#endif // FEATURE_AIP == ENABLE 

#ifdef ADJUSTER_USERREDGAIN_DECLARED
         case DDC2B_CMD_VCP_RedGain:
            {
               if (SetVcp)
               {
				//Send SocIPC DDC/CI Command to Athena B/C								
				DDC_Cmd_Buff[0] = DDC2B_CMD_VCP_RedGain;
				DDC_Cmd_Buff[1] = (BYTE)(W_VCPValue>>8);
				DDC_Cmd_Buff[2] = (BYTE)(W_VCPValue & 0x00FF);
																	
				if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEB_DST_ADDRESS, SICMD_RW_DDCI_CONTRL, DDC_Cmd_Buff) == FALSE)
					msg_ddc("\n<Soc IPC Error> Device B DDC Send Red Gain Command Failure.\n",0);
												
				if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEC_DST_ADDRESS, SICMD_RW_DDCI_CONTRL, DDC_Cmd_Buff) == FALSE)
					msg_ddc("\n<Soc IPC Error> Device C DDC Send Red Gain Command Failure.\n",0);					
               }
               else
               {
				//DDC/CI handled by Athena A and system will always return Athrna B status via DDC/CI protocol
				//Available Sequences: Athena B-> Athena C -> Athena A
														 						 
				DDC_Cmd_Buff[0] = DDC2B_CMD_VCP_RedGain;
														 
				if(SocIPCCheckDevice(SOCIPC_DEVICEB_DST_ADDRESS))
				{
					//Athena-B
					if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEB_DST_ADDRESS, SICMD_RO_DDCI_Status, DDC_Cmd_Buff) == FALSE)
					{
                  		W_VCPValue = UserPrefUserRedGain - (WORD)gm_GetAdjusterMin(ADJV_UserRedGain);
                  		W_VCPMax = (WORD)gm_GetAdjusterMax(ADJV_UserRedGain) - (WORD)gm_GetAdjusterMin(ADJV_UserRedGain);
                  		B_VCPType = 0;
						msg_ddc("\n<Soc IPC Error> [GetVCP]Device B DDC Send Red Gain Command Failure.\n",0);
					}
					else
					{
						W_VCPMax = gAthena_B_C_Parameters.DDC_VCP_Max;
						B_VCPType = gAthena_B_C_Parameters.DDC_VCP_Type;    
						W_VCPValue = gAthena_B_C_Parameters.DDC_Value;
					}
				}
				else
				{
					//Athena-C
					if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEC_DST_ADDRESS, SICMD_RO_DDCI_Status, DDC_Cmd_Buff) == FALSE)
					{
                  		W_VCPValue = UserPrefUserRedGain - (WORD)gm_GetAdjusterMin(ADJV_UserRedGain);
                  		W_VCPMax = (WORD)gm_GetAdjusterMax(ADJV_UserRedGain) - (WORD)gm_GetAdjusterMin(ADJV_UserRedGain);
                  		B_VCPType = 0;		 
						msg_ddc("\n<Soc IPC Error> [GetVCP]Device C DDC Send Red Gain Command Failure.\n",0);
					}
					else
					{
						W_VCPMax = gAthena_B_C_Parameters.DDC_VCP_Max;
						B_VCPType = gAthena_B_C_Parameters.DDC_VCP_Type;    
						W_VCPValue = gAthena_B_C_Parameters.DDC_Value;
					}				 
				}
               }
               break;
            }
#endif

#ifdef ADJUSTER_USERGREENGAIN_DECLARED
         case DDC2B_CMD_VCP_GreenGain:
            {
               if (SetVcp)
               {
				//Send SocIPC DDC/CI Command to Athena B/C										
				DDC_Cmd_Buff[0] = DDC2B_CMD_VCP_GreenGain;
				DDC_Cmd_Buff[1] = (BYTE)(W_VCPValue>>8);
				DDC_Cmd_Buff[2] = (BYTE)(W_VCPValue & 0x00FF);
																					
				if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEB_DST_ADDRESS, SICMD_RW_DDCI_CONTRL, DDC_Cmd_Buff) == FALSE)
					msg_ddc("\n<Soc IPC Error> Device B DDC Send Green Gain Command Failure.\n",0);
																
				if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEC_DST_ADDRESS, SICMD_RW_DDCI_CONTRL, DDC_Cmd_Buff) == FALSE)
					msg_ddc("\n<Soc IPC Error> Device C DDC Send Green Gain Command Failure.\n",0);					
               }
               else
               {
				//DDC/CI handled by Athena A and system will always return Athrna B status via DDC/CI protocol
				//Available Sequences: Athena B-> Athena C -> Athena A
														 					 
				DDC_Cmd_Buff[0] = DDC2B_CMD_VCP_GreenGain;
														 
				if(SocIPCCheckDevice(SOCIPC_DEVICEB_DST_ADDRESS))
				{
					//Athena-B
					if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEB_DST_ADDRESS, SICMD_RO_DDCI_Status, DDC_Cmd_Buff) == FALSE)
					{
                  		W_VCPValue = UserPrefUserGreenGain - (WORD)gm_GetAdjusterMin(ADJV_UserGreenGain);
                  		W_VCPMax = (WORD)gm_GetAdjusterMax(ADJV_UserGreenGain) - (WORD)gm_GetAdjusterMin(ADJV_UserGreenGain);
                  		B_VCPType = 0; 
						msg_ddc("\n<Soc IPC Error> [GetVCP]Device B DDC Send Green Gain Command Failure.\n",0);
					}
					else
					{
						W_VCPMax = gAthena_B_C_Parameters.DDC_VCP_Max;
						B_VCPType = gAthena_B_C_Parameters.DDC_VCP_Type;    
						W_VCPValue = gAthena_B_C_Parameters.DDC_Value;
					}
				}
				else
				{
					//Athena-C
					if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEC_DST_ADDRESS, SICMD_RO_DDCI_Status, DDC_Cmd_Buff) == FALSE)
					{
                  		W_VCPValue = UserPrefUserGreenGain - (WORD)gm_GetAdjusterMin(ADJV_UserGreenGain);
                  		W_VCPMax = (WORD)gm_GetAdjusterMax(ADJV_UserGreenGain) - (WORD)gm_GetAdjusterMin(ADJV_UserGreenGain);
                  		B_VCPType = 0;	 
						msg_ddc("\n<Soc IPC Error> [GetVCP]Device C DDC Send Green Gain Command Failure.\n",0);
					}
					else
					{
						W_VCPMax = gAthena_B_C_Parameters.DDC_VCP_Max;
						B_VCPType = gAthena_B_C_Parameters.DDC_VCP_Type;    
						W_VCPValue = gAthena_B_C_Parameters.DDC_Value;
					}				 
				}
               }
               break;
            }
#endif

#ifdef ADJUSTER_USERBLUEGAIN_DECLARED
         case DDC2B_CMD_VCP_BlueGain:
            {
               if (SetVcp)
               {
				//Send SocIPC DDC/CI Command to Athena B/C															
				DDC_Cmd_Buff[0] = DDC2B_CMD_VCP_BlueGain;
				DDC_Cmd_Buff[1] = (BYTE)(W_VCPValue>>8);
				DDC_Cmd_Buff[2] = (BYTE)(W_VCPValue & 0x00FF);
																									
				if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEB_DST_ADDRESS, SICMD_RW_DDCI_CONTRL, DDC_Cmd_Buff) == FALSE)
					msg_ddc("\n<Soc IPC Error> Device B DDC Send Blue Gain Command Failure.\n",0);
																				
				if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEC_DST_ADDRESS, SICMD_RW_DDCI_CONTRL, DDC_Cmd_Buff) == FALSE)
					msg_ddc("\n<Soc IPC Error> Device C DDC Send Blue Gain Command Failure.\n",0);					
               }
               else
               {
				//DDC/CI handled by Athena A and system will always return Athrna B status via DDC/CI protocol
				//Available Sequences: Athena B-> Athena C -> Athena A
														 								 
				DDC_Cmd_Buff[0] = DDC2B_CMD_VCP_BlueGain;
														 
				if(SocIPCCheckDevice(SOCIPC_DEVICEB_DST_ADDRESS))
				{
					//Athena-B
					if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEB_DST_ADDRESS, SICMD_RO_DDCI_Status, DDC_Cmd_Buff) == FALSE)
					{
						W_VCPValue = UserPrefUserBlueGain - (WORD)gm_GetAdjusterMin(ADJV_UserBlueGain);
						W_VCPMax = (WORD)gm_GetAdjusterMax(ADJV_UserBlueGain) - (WORD)gm_GetAdjusterMin(ADJV_UserBlueGain);
						B_VCPType = 0;
						msg_ddc("\n<Soc IPC Error> [GetVCP]Device B DDC Send Blue Gain Command Failure.\n",0);
					}
					else
					{
						W_VCPMax = gAthena_B_C_Parameters.DDC_VCP_Max;
						B_VCPType = gAthena_B_C_Parameters.DDC_VCP_Type;    
						W_VCPValue = gAthena_B_C_Parameters.DDC_Value;
					}
				}
				else
				{
					//Athena-C
					if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEC_DST_ADDRESS, SICMD_RO_DDCI_Status, DDC_Cmd_Buff) == FALSE)
					{
        				W_VCPValue = UserPrefUserBlueGain - (WORD)gm_GetAdjusterMin(ADJV_UserBlueGain);
                  		W_VCPMax = (WORD)gm_GetAdjusterMax(ADJV_UserBlueGain) - (WORD)gm_GetAdjusterMin(ADJV_UserBlueGain);
                  		B_VCPType = 0; 
						msg_ddc("\n<Soc IPC Error> [GetVCP]Device C DDC Send Blue Gain Command Failure.\n",0);
					}
					else
					{
						W_VCPMax = gAthena_B_C_Parameters.DDC_VCP_Max;
						B_VCPType = gAthena_B_C_Parameters.DDC_VCP_Type;    
						W_VCPValue = gAthena_B_C_Parameters.DDC_Value;
					}				 
				}
               }
               break;
            }
#endif

#ifdef ADJUSTER_SHARPNESS_DECLARED
         case DDC2B_CMD_VCP_TVSharpness:
            {
               if (SetVcp)
               {
				//Send SocIPC DDC/CI Command to Athena B/C																	
				DDC_Cmd_Buff[0] = DDC2B_CMD_VCP_TVSharpness;
				DDC_Cmd_Buff[1] = (BYTE)(W_VCPValue>>8);
				DDC_Cmd_Buff[2] = (BYTE)(W_VCPValue & 0x00FF);
																													
				if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEB_DST_ADDRESS, SICMD_RW_DDCI_CONTRL, DDC_Cmd_Buff) == FALSE)
					msg_ddc("\n<Soc IPC Error> Device B DDC Send Sharpness Command Failure.\n",0);
																								
				if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEC_DST_ADDRESS, SICMD_RW_DDCI_CONTRL, DDC_Cmd_Buff) == FALSE)
					msg_ddc("\n<Soc IPC Error> Device C DDC Send Sharpness Command Failure.\n",0); 				
               }
               else
               {
				//DDC/CI handled by Athena A and system will always return Athrna B status via DDC/CI protocol
				//Available Sequences: Athena B-> Athena C -> Athena A
																														
				DDC_Cmd_Buff[0] = DDC2B_CMD_VCP_TVSharpness;
																			
				if(SocIPCCheckDevice(SOCIPC_DEVICEB_DST_ADDRESS))
				{
					//Athena-B
					if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEB_DST_ADDRESS, SICMD_RO_DDCI_Status, DDC_Cmd_Buff) == FALSE)
					{
						W_VCPValue = UserPrefSharpness;
						msg_ddc("\n<Soc IPC Error> [GetVCP]Device B DDC Send Sharpness Command Failure.\n",0);
					}
					else
					{
						W_VCPMax = gAthena_B_C_Parameters.DDC_VCP_Max;
						B_VCPType = gAthena_B_C_Parameters.DDC_VCP_Type;    
						W_VCPValue = gAthena_B_C_Parameters.DDC_Value;
					}
				}
				else
				{
					//Athena-C
					if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEC_DST_ADDRESS, SICMD_RO_DDCI_Status, DDC_Cmd_Buff) == FALSE)
					{
                  		W_VCPValue = UserPrefSharpness;
						msg_ddc("\n<Soc IPC Error> [GetVCP]Device C DDC Send Sharpness Command Failure.\n",0);
					}
					else
					{
						W_VCPMax = gAthena_B_C_Parameters.DDC_VCP_Max;
						B_VCPType = gAthena_B_C_Parameters.DDC_VCP_Type;    
						W_VCPValue = gAthena_B_C_Parameters.DDC_Value;
					}				
				}
               }
               break;
            }
#endif

#ifdef ADJUSTER_SATURATION_DECLARED
         case DDC2B_CMD_VCP_TVSaturation:
            {
               if (SetVcp)
               {
				//Send SocIPC DDC/CI Command to Athena B/C																					
				DDC_Cmd_Buff[0] = DDC2B_CMD_VCP_TVSaturation;
				DDC_Cmd_Buff[1] = (BYTE)(W_VCPValue>>8);
				DDC_Cmd_Buff[2] = (BYTE)(W_VCPValue & 0x00FF);
																																	
				if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEB_DST_ADDRESS, SICMD_RW_DDCI_CONTRL, DDC_Cmd_Buff) == FALSE)
					msg_ddc("\n<Soc IPC Error> Device B DDC Send Saturation Command Failure.\n",0);
																												
				if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEC_DST_ADDRESS, SICMD_RW_DDCI_CONTRL, DDC_Cmd_Buff) == FALSE)
					msg_ddc("\n<Soc IPC Error> Device C DDC Send Saturation Command Failure.\n",0); 				
               }
               else
               {
				//DDC/CI handled by Athena A and system will always return Athrna B status via DDC/CI protocol
				//Available Sequences: Athena B-> Athena C -> Athena A
																															
				DDC_Cmd_Buff[0] = DDC2B_CMD_VCP_TVSaturation;
																			
				if(SocIPCCheckDevice(SOCIPC_DEVICEB_DST_ADDRESS))
				{
					//Athena-B
					if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEB_DST_ADDRESS, SICMD_RO_DDCI_Status, DDC_Cmd_Buff) == FALSE)
					{
						W_VCPValue = UserPrefSaturation;
						msg_ddc("\n<Soc IPC Error> [GetVCP]Device B DDC Send Saturation Command Failure.\n",0);
					}
					else
					{
						W_VCPMax = gAthena_B_C_Parameters.DDC_VCP_Max;
						B_VCPType = gAthena_B_C_Parameters.DDC_VCP_Type;    
						W_VCPValue = gAthena_B_C_Parameters.DDC_Value;
					}
				}
				else
				{
					//Athena-C
					if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEC_DST_ADDRESS, SICMD_RO_DDCI_Status, DDC_Cmd_Buff) == FALSE)
					{
						W_VCPValue = UserPrefSaturation;
						msg_ddc("\n<Soc IPC Error> [GetVCP]Device C DDC Send Saturation Command Failure.\n",0);
					}
					else
					{
						W_VCPMax = gAthena_B_C_Parameters.DDC_VCP_Max;
						B_VCPType = gAthena_B_C_Parameters.DDC_VCP_Type;    
						W_VCPValue = gAthena_B_C_Parameters.DDC_Value;
					}				
				}
               }
               break;
            }
#endif

#ifdef Action_AutoAdjust_WB_Used
/*ChengLin 20120312: Disable this feature due to 4k2k platform don't support VGA source.*/
#if 0//FEATURE_AIP == ENABLE

         case DDC2B_CMD_VCP_AutoSetUp:
            {
               if (SetVcp && W_VCPValue)
               {             
                  ModeSetupAutoAdjust(gmvb_MainChannel);
               }
               else
               {
                  W_VCPMax = 1;
                  B_VCPType = 1;
               }
               break;
            }
#endif
#endif


#ifdef ADJUSTER_OSDLANGUAGE_DECLARED
         case DDC2B_CMD_VCP_OsdLanguage:
            {
               if (SetVcp)
               {
				//Send SocIPC DDC/CI Command to Athena B/C																										
				DDC_Cmd_Buff[0] = DDC2B_CMD_VCP_OsdLanguage;
				DDC_Cmd_Buff[1] = (BYTE)(W_VCPValue>>8);
				DDC_Cmd_Buff[2] = (BYTE)(W_VCPValue & 0x00FF);
																																					
				if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEB_DST_ADDRESS, SICMD_RW_DDCI_CONTRL, DDC_Cmd_Buff) == FALSE)
					msg_ddc("\n<Soc IPC Error> Device B DDC Send OSD Language Command Failure.\n",0);
																																
				if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEC_DST_ADDRESS, SICMD_RW_DDCI_CONTRL, DDC_Cmd_Buff) == FALSE)
					msg_ddc("\n<Soc IPC Error> Device C DDC Send OSD Language Command Failure.\n",0);				
               }
               else
               {
				//DDC/CI handled by Athena A and system will always return Athrna B status via DDC/CI protocol
				//Available Sequences: Athena B-> Athena C -> Athena A
																										
				DDC_Cmd_Buff[0] = DDC2B_CMD_VCP_OsdLanguage;
																			
				if(SocIPCCheckDevice(SOCIPC_DEVICEB_DST_ADDRESS))
				{
					//Athena-B
					if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEB_DST_ADDRESS, SICMD_RO_DDCI_Status, DDC_Cmd_Buff) == FALSE)
					{
						if (UserPrefOsdLanguage == 1)
							W_VCPValue = 1;
						else if (UserPrefOsdLanguage == 0)
							W_VCPValue = 2;
						else W_VCPValue = 0;
						W_VCPMax = 11;
						msg_ddc("\n<Soc IPC Error> [GetVCP]Device B DDC Send OSD Language Command Failure.\n",0);
					}
					else
					{
						W_VCPMax = gAthena_B_C_Parameters.DDC_VCP_Max;
						B_VCPType = gAthena_B_C_Parameters.DDC_VCP_Type;    
						W_VCPValue = gAthena_B_C_Parameters.DDC_Value;
					}
				}
				else
				{
					//Athena-C
					if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEC_DST_ADDRESS, SICMD_RO_DDCI_Status, DDC_Cmd_Buff) == FALSE)
					{
                  		if (UserPrefOsdLanguage == 1)
                     		W_VCPValue = 1;
                  		else if (UserPrefOsdLanguage == 0)
                     		W_VCPValue = 2;
                  		else W_VCPValue = 0;
						W_VCPMax = 11;
						msg_ddc("\n<Soc IPC Error> [GetVCP]Device C DDC Send OSD Language Command Failure.\n",0);
					}
					else
					{
						W_VCPMax = gAthena_B_C_Parameters.DDC_VCP_Max;
						B_VCPType = gAthena_B_C_Parameters.DDC_VCP_Type;    
						W_VCPValue = gAthena_B_C_Parameters.DDC_Value;
					}				
				}
               }
               break;
            }
#endif

            // TODO: JasonL - revisit these functions and implement

#ifdef ADJUSTER_CURRENTINPUTMAIN_DECLARED
         case DDC2B_CMD_VCP_SourceSelect:
            {
               B_Temp = UserPrefCurrentInputMain;
               if (SetVcp)
               {
                  switch (W_VCPValue)
                  {
                     case VCP_SourceSelect_AnalogVideo_1:
                        B_Temp = ALL_BOARDS_VGA1;
                        break;
                     case VCP_SourceSelect_CompositeVideo_1:
                        B_Temp = ALL_BOARDS_CVBS1;
                        break;
                     case VCP_SourceSelect_SVideo_1:
                        B_Temp = ALL_BOARDS_SVIDEO1;
                        break;
                     case VCP_SourceSelect_ComponentVideo_1:
                        B_Temp = ALL_BOARDS_COMP1;
                        break;
                     case VCP_SourceSelect_AnalogVideo_2:
                     case VCP_SourceSelect_DigitalVideo_2:
                     case VCP_SourceSelect_CompositeVideo_2:
                     case VCP_SourceSelect_SVideo_2:
                     case VCP_SourceSelect_Tuner_1:
                     case VCP_SourceSelect_Tuner_2:
                     case VCP_SourceSelect_Tuner_3:
                     case VCP_SourceSelect_ComponentVideo_2:
                     case VCP_SourceSelect_ComponentVideo_3:
                     default:
                        IsFeatureSupported = FALSE;
                        break;
                  }

                  if (B_Temp != UserPrefCurrentInputMain)
                  {
                     UserPrefCurrentInputMain = B_Temp;
                     AdjustCurrentInputMain();
                  }
               }
               else
               {
                  //W_VCPMax = VCP_SourceSelect_ComponentVideo_1;
                  W_VCPMax = 0xFFFF;
                  switch (UserPrefCurrentInputMain)
                  {
                     case ALL_BOARDS_VGA1:
                        W_VCPValue = VCP_SourceSelect_AnalogVideo_1;
                        break;
                     case ALL_BOARDS_CVBS1:
                        W_VCPValue = VCP_SourceSelect_CompositeVideo_1;
                        break;
                     case ALL_BOARDS_SVIDEO1:
                        W_VCPValue = VCP_SourceSelect_SVideo_1;
                        break;
                     case ALL_BOARDS_COMP1:
                        W_VCPValue = VCP_SourceSelect_ComponentVideo_1;
                        break;
#if 1 // To match Chandler for PhotoTune ...
                     case ALL_BOARDS_HDMI:
                     case ALL_BOARDS_HDMI2:
                     case ALL_BOARDS_DP1:
//&*&*&*S 20120315:SWRD4001: Kordonm(0)  Modify for OSD Workbench
                     case ALL_BOARDS_DP3:
//&*&*&*E 20120315 : Kordonm(1)  Modify for OSD Workbench
                        W_VCPValue = 0x0300;
                        break;
#endif

                     default:
                        IsFeatureSupported = FALSE;
                        break;
                  }
               }
               break;
            }
#endif


#ifdef ADJUSTER_OSDLANGUAGE_DECLARED
         case DDC2B_CMD_VCP_Language:
            {
               if (SetVcp)
               {
				//Send SocIPC DDC/CI Command to Athena B/C																															
				DDC_Cmd_Buff[0] = DDC2B_CMD_VCP_Language;
				DDC_Cmd_Buff[1] = (BYTE)(W_VCPValue>>8);
				DDC_Cmd_Buff[2] = (BYTE)(W_VCPValue & 0x00FF);
																																									
				if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEB_DST_ADDRESS, SICMD_RW_DDCI_CONTRL, DDC_Cmd_Buff) == FALSE)
					msg_ddc("\n<Soc IPC Error> Device B DDC Send Language Command Failure.\n",0);
																																				
				if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEC_DST_ADDRESS, SICMD_RW_DDCI_CONTRL, DDC_Cmd_Buff) == FALSE)
					msg_ddc("\n<Soc IPC Error> Device C DDC Send Language Command Failure.\n",0);				
               }
               else
               {
				//DDC/CI handled by Athena A and system will always return Athrna B status via DDC/CI protocol
				//Available Sequences: Athena B-> Athena C -> Athena A
																							   																	   
				DDC_Cmd_Buff[0] = DDC2B_CMD_VCP_Language;
																							   
				if(SocIPCCheckDevice(SOCIPC_DEVICEB_DST_ADDRESS))
				{
					//Athena-B
					if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEB_DST_ADDRESS, SICMD_RO_DDCI_Status, DDC_Cmd_Buff) == FALSE)
					{
						W_VCPValue = UserPrefOsdLanguage + 1;
						W_VCPMax = 2;
						msg_ddc("\n<Soc IPC Error> [GetVCP]Device B DDC Send Language Command Failure.\n",0);
					}
					else
					{
						W_VCPMax = gAthena_B_C_Parameters.DDC_VCP_Max;
						B_VCPType = gAthena_B_C_Parameters.DDC_VCP_Type;    
						W_VCPValue = gAthena_B_C_Parameters.DDC_Value;
					}
				}
				else
				{
					//Athena-C
					if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEC_DST_ADDRESS, SICMD_RO_DDCI_Status, DDC_Cmd_Buff) == FALSE)
					{
						W_VCPValue = UserPrefOsdLanguage + 1;
						W_VCPMax = 2;
						msg_ddc("\n<Soc IPC Error> [GetVCP]Device C DDC Send Language Command Failure.\n",0);
					}
					else
					{
						W_VCPMax = gAthena_B_C_Parameters.DDC_VCP_Max;
						B_VCPType = gAthena_B_C_Parameters.DDC_VCP_Type;    
						W_VCPValue = gAthena_B_C_Parameters.DDC_Value;
					}			   
				}
               }
               break;
            }
#endif

         case DDC2B_CMD_VCP_Settings:
            {
               if (SetVcp)
               {
                  SaveModeIndependentSettings();
				/*ChengLin 20120312: Disable this feature due to 4k2k platform don't support VGA source.*/				  
               #if 0//FEATURE_AIP == ENABLE
                  LookUpType.W_Hfreq = stInput[gmvb_MainChannel].Hfrq;
                  LookUpType.W_Vfreq = stInput[gmvb_MainChannel].Vfrq;
                  LookUpType.W_Vtotal = stInput[gmvb_MainChannel].Vtotal;
                  SaveModeDependentSettings(gmvb_MainChannel, &LookUpType);
               #endif
               }
               else
               {
                  B_VCPType =1;
                  W_VCPMax =1;
               }
               break;
            }

         default:
            {
               //error condition - unsupported vcp opcode

               IsFeatureSupported = FALSE;
               break;
            }
      }
   }
   else if (VCPPage == 2)
   {
      switch (VCPFeature.vcp_opcode)
      {
#ifdef ADJUSTER_OSDHPOSITION_DECLARED
         case DDC2B_CMD_VCP_OSDHPosition:
            {
               if (SetVcp)
               {
				//Send SocIPC DDC/CI Command to Athena B/C																																			
				DDC_Cmd_Buff[0] = DDC2B_CMD_VCP_OSDHPosition;
				DDC_Cmd_Buff[1] = (BYTE)(W_VCPValue>>8);
				DDC_Cmd_Buff[2] = (BYTE)(W_VCPValue & 0x00FF);
																																													
				if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEB_DST_ADDRESS, SICMD_RW_DDCI_CONTRL, DDC_Cmd_Buff) == FALSE)
					msg_ddc("\n<Soc IPC Error> Device B DDC Send OSD HPosition Command Failure.\n",0);
																																								
				if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEC_DST_ADDRESS, SICMD_RW_DDCI_CONTRL, DDC_Cmd_Buff) == FALSE)
					msg_ddc("\n<Soc IPC Error> Device C DDC Send OSD HPosition Command Failure.\n",0);				
               }
               else
               {
				//DDC/CI handled by Athena A and system will always return Athrna B status via DDC/CI protocol
				//Available Sequences: Athena B-> Athena C -> Athena A
																							   
				BYTE DDC_Cmd_Buff[1]={0};
																							   
				DDC_Cmd_Buff[0] = DDC2B_CMD_VCP_OSDHPosition;
																							   
				if(SocIPCCheckDevice(SOCIPC_DEVICEB_DST_ADDRESS))
				{
					//Athena-B
					if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEB_DST_ADDRESS, SICMD_RO_DDCI_Status, DDC_Cmd_Buff) == FALSE)
					{
						W_VCPValue= UserPrefOsdHPosition;
						msg_ddc("\n<Soc IPC Error> [GetVCP]Device B DDC Send OSD HPosition Command Failure.\n",0);
					}
					else
					{
						W_VCPMax = gAthena_B_C_Parameters.DDC_VCP_Max;
						B_VCPType = gAthena_B_C_Parameters.DDC_VCP_Type;    
						W_VCPValue = gAthena_B_C_Parameters.DDC_Value;
					}
				}
				else
				{
					//Athena-C
					if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEC_DST_ADDRESS, SICMD_RO_DDCI_Status, DDC_Cmd_Buff) == FALSE)
					{
                  		W_VCPValue= UserPrefOsdHPosition;
						msg_ddc("\n<Soc IPC Error> [GetVCP]Device C DDC Send OSD HPosition Command Failure.\n",0);
					}
					else
					{
						W_VCPMax = gAthena_B_C_Parameters.DDC_VCP_Max;
						B_VCPType = gAthena_B_C_Parameters.DDC_VCP_Type;    
						W_VCPValue = gAthena_B_C_Parameters.DDC_Value;
					}			   
				}
               }
               break;
            }
#endif

#ifdef ADJUSTER_OSDVPOSITION_DECLARED
         case DDC2B_CMD_VCP_OSDVPosition:
            {
               if (SetVcp)
               {
				//Send SocIPC DDC/CI Command to Athena B/C																																				
				DDC_Cmd_Buff[0] = DDC2B_CMD_VCP_OSDVPosition;
				DDC_Cmd_Buff[1] = (BYTE)(W_VCPValue>>8);
				DDC_Cmd_Buff[2] = (BYTE)(W_VCPValue & 0x00FF);
																																																	
				if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEB_DST_ADDRESS, SICMD_RW_DDCI_CONTRL, DDC_Cmd_Buff) == FALSE)
					msg_ddc("\n<Soc IPC Error> Device B DDC Send OSD VPosition Command Failure.\n",0);
																																												
				if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEC_DST_ADDRESS, SICMD_RW_DDCI_CONTRL, DDC_Cmd_Buff) == FALSE)
					msg_ddc("\n<Soc IPC Error> Device C DDC Send OSD VPosition Command Failure.\n",0); 			
               }
               else
               {
				//DDC/CI handled by Athena A and system will always return Athrna B status via DDC/CI protocol
				//Available Sequences: Athena B-> Athena C -> Athena A
																							   																   
				DDC_Cmd_Buff[0] = DDC2B_CMD_VCP_OSDVPosition;
																							   
				if(SocIPCCheckDevice(SOCIPC_DEVICEB_DST_ADDRESS))
				{
					//Athena-B
					if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEB_DST_ADDRESS, SICMD_RO_DDCI_Status, DDC_Cmd_Buff) == FALSE)
					{
						W_VCPValue= UserPrefOsdVPosition;
						msg_ddc("\n<Soc IPC Error> [GetVCP]Device B DDC Send OSD VPosition Command Failure.\n",0);
					}
					else
					{
						W_VCPMax = gAthena_B_C_Parameters.DDC_VCP_Max;
						B_VCPType = gAthena_B_C_Parameters.DDC_VCP_Type;    
						W_VCPValue = gAthena_B_C_Parameters.DDC_Value;
					}
				}
				else
				{
					//Athena-C
					if(SocIPCSendCmdPacketParser(SOCIPC_DEVICEC_DST_ADDRESS, SICMD_RO_DDCI_Status, DDC_Cmd_Buff) == FALSE)
					{
                  		W_VCPValue= UserPrefOsdVPosition;
						msg_ddc("\n<Soc IPC Error> [GetVCP]Device C DDC Send OSD VPosition Command Failure.\n",0);
					}
					else
					{
						W_VCPMax = gAthena_B_C_Parameters.DDC_VCP_Max;
						B_VCPType = gAthena_B_C_Parameters.DDC_VCP_Type;    
						W_VCPValue = gAthena_B_C_Parameters.DDC_Value;
					}			   
				}
               }
               break;
            }
#endif

         default:
            {
               //error condition - unsupported vcp opcode
               IsFeatureSupported = FALSE;
               break;
            }
      }
   }
   else
   { //error condition - unsupported page

      //set current page to 0
      VCPPage = 0;
      // NOTE:
      //readExpected = 0; //no message back if page is not supported

      IsFeatureSupported = FALSE;
   }

   if (!SetVcp)
   {
      VCPFeature.max_BigEndian = EndianConv(W_VCPMax);
      VCPFeature.value_BigEndian = EndianConv(W_VCPValue);
      VCPFeature.vcp_type = B_VCPType;
   }

   if (IsFeatureSupported)
      return 0;
   else
   {
      //VCPFeature.max_BigEndian = 0;
      VCPFeature.value_BigEndian = 0;

      VCPFeature.max_BigEndian = 0xffff;

      return 1;
   }
}

#else //Original

BYTE DDC2Bi_VCP_ParseCommand(BYTE SetVcp)
{
   WORD W_Temp;
   BYTE B_Temp;
   BOOL IsFeatureSupported = TRUE;

   //default values for Max and Current Values
   WORD W_VCPValue = 0;
   //WORD W_VCPMax = 0x100;
   WORD W_VCPMax = 0xFFFF;
   BYTE B_VCPType = 0x01;
   ModeDependentLookUpType LookUpType;

   //converts values to Little Endian
   if (SetVcp)
      W_VCPValue = EndianConv(VCPFeature.value_BigEndian);

   if (VCPPage == 0)
   {

	  if(SetVcp==SET_VCP)
	  {
	  	UserPrefHideIconFlag=TRUE;	//20120912for menu OSD disappear input icon shown at left upper side issue
   	  OSDRequestMsg(gmvb_MainChannel, MTO_RemoveOSD, 0); //OSD should be closed if there is any Valid VCP commands received. 
	  }
   	  
      switch (VCPFeature.vcp_opcode)
      {
         case DDC2B_CMD_VCP_NewControlVal: //To make MCCS 2.0 Compliant
            {
               //static BYTE B_DDCCINewControlVal;
               if (SetVcp == SET_VCP && W_VCPValue)
               {
                  W_VCPValue = NoNewControlVal;
                  B_DDCCINewControlVal = 0;
               }
               else if (SetVcp == GET_VCP)
               {  //Check for modeIndependent & mode dependent variables saved in NVRAM
                  W_VCPValue = B_DDCCINewControlVal ? NewControlVal: NoNewControlVal; // 2 - a new control value has been saved
                  msg_ddc("NewControlVal: %d", W_VCPValue);
               }
               break;
            }

         case DDC2B_CMD_VCP_MCCSVersion: //To make MCCS 2.0 Compliant
            if (SetVcp == GET_VCP)
            {
               W_VCPValue = VCP_Version; //Version 2, revision 0 of VESA Monitor control command set document
               msg_ddc("MCCSVersion: %d", W_VCPValue);
            }
            break;

         case DDC2B_CMD_VCP_HFreq: //To make MCCS 2.0 Compliant
            W_VCPValue = stInput[gmvb_MainChannel].Hfrq*100;
            W_VCPMax = (WORD)(((DWORD)stInput[gmvb_MainChannel].Hfrq*100) >> 16); // W_VCPMax is the high byte
            msg_ddc("DDC2B_CMD_VCP_HFreq %d", W_VCPValue);
            break;

         case DDC2B_CMD_VCP_VFreq:   //To make MCCS 2.0 Compliant
            W_VCPValue = stInput[gmvb_MainChannel].Vfrq*10;
            msg_ddc("DDC2B_CMD_VCP_VFreq %d", W_VCPValue);
            break;

         case DDC2B_CMD_VCP_MonitorType:
            W_VCPValue = DispTechnologyType; //To make MCCS 2.0 Compliant
            break;

         case DDC2B_CMD_VCP_DisplayUsageTime:
            {
               if (!SetVcp)
               {
                  extern DWORD DW_MonitorPowerOnTime;

                  W_VCPValue = (WORD)(DW_MonitorPowerOnTime/60); // A 2byte value, DW_MonitorPowerOnTime is in minute
               }
               break;
            }

         case DDC2B_CMD_VCP_ApplnEnKey: //To make MCCS 2.0 Compliant
            W_VCPValue = ApplnEnableKey; //A 2byte value
            break;

         case DDC2B_CMD_VCP_DispControllerType:
            //byte 1 = 2 for Genesis Controller, byte 2 = controller type (product_ID)
            W_VCPValue = DispControllerType | ((WORD)gm_ReadRegByte(PRODUCT_ID) << 8); // 
            W_VCPMax = gm_ReadRegWord(PRODUCT_ID) >> 8;
            //Byte 3 - provide a numeric indication of controller type (Rev ID) - ???
            break;

         case DDC2B_CMD_VCP_DispFirmwareLevel:
            W_VCPValue = 0x201; //A 2byte value - f/w level installed in the display
            break;

         case DDC2B_CMD_VCP_ALLReset:
            if (SetVcp && W_VCPValue)
            {              
#ifdef DELL_U2410F	// Modified by ST_hansPENG 081007					
 ////////////////Wistron Lin Chihlung ///////////////////////////
		 if(UserPrefFactoryFlag)
                {
                           UserPrefTimeUsedHour=0;
                           UserPrefTimeUsedMin=0;
				 SaveFactorySettingsInNVR(); 		   
                }
//////////////////////////////////////////////////////
				FactoryReset();								   
#else
                BYTE B_Channel;

                msg_ddc( "Factory Reset", 0);
               //FactoryReset();
               UserPrefImageScheme = FactoryDefaultInROM.ImageScheme;
               UserPrefBrightness = SchemeFactoryDefaultInROM[UserPrefImageScheme].Brightness;
               UserPrefContrast = SchemeFactoryDefaultInROM[UserPrefImageScheme].Contrast;
               UserPrefUserRedGain = SchemeFactoryDefaultInROM[UserPrefImageScheme].UserRedGain;
               UserPrefUserGreenGain = SchemeFactoryDefaultInROM[UserPrefImageScheme].UserGreenGain;
               UserPrefUserBlueGain = SchemeFactoryDefaultInROM[UserPrefImageScheme].UserBlueGain;
               UserPrefUserRedOff = SchemeFactoryDefaultInROM[UserPrefImageScheme].UserRedOff;
               UserPrefUserGreenOff = SchemeFactoryDefaultInROM[UserPrefImageScheme].UserGreenOff;
               UserPrefUserBlueOff = SchemeFactoryDefaultInROM[UserPrefImageScheme].UserBlueOff;
               AdjustBrightness();
               AdjustContrast();
               AdjustRedGain();
               AdjustBlueGain();
               AdjustGreenGain();
               AdjustRedOff();
               AdjustBlueOff();
               AdjustGreenOff();

            #if FEATURE_AIP == ENABLE
               if (gm_IsInputTripleADC(gmvb_MainChannel))
                  B_Channel = gmvb_MainChannel;
               else if (gm_IsInputTripleADC(gmvb_PipChannel))
                  B_Channel = gmvb_PipChannel;
               else
                  break;
               
               UserPrefHTotal = gmvw_InputHTotalOrg[B_Channel];
               UserPrefHStart = gmvw_InputHStartOrg[B_Channel];
               UserPrefHSyncDelay = gmvsw_InputHSyncOffset[B_Channel];
               UserPrefVStart = gmvw_InputVStartOrg[B_Channel];
               gmvsw_InputHSyncOffset[B_Channel] = 0;
               gmvsw_InputVSyncOffset[B_Channel] = 0;
               UserPrefVSyncDelay = gmvsw_InputVSyncOffset[B_Channel];
               UserPrefHSyncPhase = gmvb_InputSamplingPhase[B_Channel];
               
               gm_InputSetHPosition(B_Channel);
               gm_InputSetVPosition(B_Channel);
               SetupHTotal(UserPrefHTotal);
               gm_AutoGeometry(B_Channel, gmd_DEFAULT_AUTO);
            #endif
            
            #ifdef USE_COLOR_FIDELITY
             #if USE_DYNAMIC_GAMUTMAPPING_SUPPORT
               DisableColorProcessing();
             #endif
            #endif // USE_COLOR_FIDELITY
#endif
            }
            else
            {
               W_VCPMax = 1;
               B_VCPType = 1;		   
            }
            break;

#ifdef ADJUSTER_BRIGHTNESS_DECLARED
         case DDC2B_CMD_VCP_RestoreContrastBrightness:
            {
               if (SetVcp && W_VCPValue)
               {		
                  UserPrefBrightness = SchemeFactoryDefaultInROM[UserPrefImageScheme].Brightness;
                  UserPrefContrast = SchemeFactoryDefaultInROM[UserPrefImageScheme].Contrast;
                  AdjustBrightness();
                  AdjustContrast();	  
				  
				  //==============120808 Kordonm for DDM change preset mode reload brightness============
				  if(IsPresetMode_GRAPHICS_TEXT)
				  {
					UserPrefBrightness_Text = UserPrefBrightness;					
					UserPrefContrast_Text = UserPrefContrast;					
				  }
				  else if(IsColorSpace_GRAPHICS_CAL1)//120806 Edward add for CAL1/CAL2 save seperate
				  {
					UserPrefBrightness_CAL1 = UserPrefBrightness;					
					UserPrefContrast_CAL1= UserPrefContrast;					
				   }
				  else if(IsColorSpace_GRAPHICS_CAL2)
				  {
					UserPrefBrightness_CAL2 = UserPrefBrightness;
					UserPrefContrast_CAL2 = UserPrefContrast;
				   }
				  else
		  		  {
					UserPrefBrightness_All = UserPrefBrightness;
					UserPrefContrast_All = UserPrefContrast;
				  }
				  //====================================================================
               }
               else
               {	   
                  W_VCPMax = 1;
               }
               break;
            }
#endif

#ifdef ADJUSTER_USERREDGAIN_DECLARED
         case DDC2B_CMD_VCP_ColorReset:
            {
               if (SetVcp && W_VCPValue)
               {	               
               #ifdef DELL_U2410F
			   		ResetColorSettings();
			   #else
                  UserPrefUserRedGain = SchemeFactoryDefaultInROM[UserPrefImageScheme].UserRedGain;
                  UserPrefUserGreenGain = SchemeFactoryDefaultInROM[UserPrefImageScheme].UserGreenGain;
                  UserPrefUserBlueGain = SchemeFactoryDefaultInROM[UserPrefImageScheme].UserBlueGain;
                  UserPrefUserRedOff = SchemeFactoryDefaultInROM[UserPrefImageScheme].UserRedOff;
                  UserPrefUserGreenOff = SchemeFactoryDefaultInROM[UserPrefImageScheme].UserGreenOff;
                  UserPrefUserBlueOff = SchemeFactoryDefaultInROM[UserPrefImageScheme].UserBlueOff;
                  AdjustRedGain();
                  AdjustBlueGain();
                  AdjustGreenGain();
                  AdjustRedOff();
                  AdjustBlueOff();
                  AdjustGreenOff();			  
			   #endif
               }
               else
               {
                  W_VCPMax = 1;
               }
               break;
            }
#endif

#if FEATURE_AIP == ENABLE
         case DDC2B_CMD_VCP_GeometryReset:
            {            
               if (SetVcp && W_VCPValue)
               {          
                  BYTE B_Channel;
                  if (gm_IsInputTripleADC(gmvb_MainChannel))
                     B_Channel = gmvb_MainChannel;
                  else if (gm_IsInputTripleADC(gmvb_PipChannel))
                     B_Channel = gmvb_PipChannel;
                  else
                     break;
	                    
                  UserPrefHTotal = gmvw_InputHTotalOrg[B_Channel];
                  UserPrefHSyncPhase = gmvb_InputSamplingPhase[B_Channel];
                  UserPrefHStart = gmvw_InputHStartOrg[B_Channel];
                  UserPrefVStart = gmvw_InputVStartOrg[B_Channel];
                  gmvsw_InputHSyncOffset[B_Channel] = 0;
                  gmvsw_InputVSyncOffset[B_Channel] = 0;
                  UserPrefHSyncDelay = gmvsw_InputHSyncOffset[B_Channel];
                  UserPrefVSyncDelay = gmvsw_InputVSyncOffset[B_Channel];
                  
                  gm_InputSetHPosition(B_Channel);
                  gm_InputSetVPosition(B_Channel);
                  SetupHTotal(UserPrefHTotal);
                  gm_AutoGeometry(B_Channel, gmd_DEFAULT_AUTO);			  
               }
               else
               {
                  W_VCPMax = 1;
               }
               break;
            }
#endif

 case DDC2B_WISTRON_SET_AGING_MODE:
 	 if (SetVcp)  //set value
 	 	{
 		      if(W_VCPValue)
 		          {
		 	     UserPrefRunBurnInModeFlag = TRUE;
		           UserPrefFactoryFlag = TRUE;
		 		}
			else
				{
				 UserPrefRunBurnInModeFlag = FALSE;
		               UserPrefFactoryFlag = FALSE;
				}
		                 SaveModeIndependentSettings();
		 }
	   break;
 /*case DDC2B_WISTRON_EDID_WP:
               if(UserPrefFactoryFlag)
		 	{
                        if (SetVcp)  //set value
                        	{
                        	     if(W_VCPValue)
                                 NVRAM_WP_ENABLE();
                                else
                                    NVRAM_WP_DISABLE();
               
                        
			          }
               	}
	            break;*/
    case DDC2B_CMD_VCP_TextBrightness:
	if(UserPrefFactoryFlag)
		 	{
                        if (SetVcp)  //set value
                        	{                        	                     	
                        	  UserPrefBriBase_Text=W_VCPValue ;
		                Factory_AdjustBackLight(W_VCPValue);				
				 W_VCPMax=MAX_BACKLIGHT_PWM;
				 SaveFactorySettingsInNVR();
                        	}
                        else
                        	{
                        	W_VCPValue = UserPrefBriBase_Text;
                            W_VCPMax = MAX_BACKLIGHT_PWM;
                            B_VCPType = 1;
                        	}
			}
	            break;
///////////////////CAL 1 /CAL 2 backligh base/////////////////////////////////	
    case DDC2B_CMD_VCP_UNIFORMITY_Brightness:
	if(UserPrefFactoryFlag)
		 	{
                        if (SetVcp)  //set value
                        	{                        	                     	
                        	  UserPrefBriBase_Table1=W_VCPValue ;
				  UserPrefBriBase_Table2=W_VCPValue ;	//U2913WM same as table 1		  
		                Factory_AdjustBackLight(W_VCPValue);				
				 W_VCPMax=MAX_BACKLIGHT_PWM;
				 SaveFactorySettingsInNVR();
                        	}
                        else
                        	{
                        	W_VCPValue = UserPrefBriBase_Table1;
                            W_VCPMax = MAX_BACKLIGHT_PWM;
                            B_VCPType = 1;
                        	}
			}
	            break;



//////////////////////////////////////////////////////////////////////////

    case  DDC2B_CMD_VCP_sRGBBrightness:	
		if(UserPrefFactoryFlag)
		 	{
                        if (SetVcp)  //set value
                        	{                        	                     	
                        	  UserPrefBriBase_sRGB=W_VCPValue ;
		                Factory_AdjustBackLight(W_VCPValue);				
				 W_VCPMax=MAX_BACKLIGHT_PWM;
				 SaveFactorySettingsInNVR();
                        	}
                        else
                        	{
                        	W_VCPValue = UserPrefBriBase_sRGB;
                            W_VCPMax = MAX_BACKLIGHT_PWM;
                            B_VCPType = 1;
                        	}
			}
	            break;
    case DDC2B_CMD_VCP_AdobeBrightness:	
		if(UserPrefFactoryFlag)
		 	{
                        if (SetVcp)  //set value
                        	{                        	                     	
                        	  UserPrefBriBase_AdobeRGB=W_VCPValue ;
		                Factory_AdjustBackLight(W_VCPValue);				
				 W_VCPMax=MAX_BACKLIGHT_PWM;
				 SaveFactorySettingsInNVR();
                        	}
                        else
                        	{
                        	W_VCPValue = UserPrefBriBase_AdobeRGB;
                            W_VCPMax = MAX_BACKLIGHT_PWM;
                            B_VCPType = 1;
                        	}
			}
	            break;
				

#ifdef ADJUSTER_BRIGHTNESS_DECLARED
         case DDC2B_CMD_VCP_Brightness:
            {

		////////////////////////////////////////////////////////
		 ////////////////Wistron Lin Chihlung ///////////////////////////
		 	if(UserPrefFactoryFlag)
		 		{
                   if (SetVcp)  //set value
                   	{
                	   	 UserPrefBriBase_6500K=W_VCPValue ;
		                Factory_AdjustBackLight(W_VCPValue);				
				 		W_VCPMax=MAX_BACKLIGHT_PWM;
				 		SaveFactorySettingsInNVR();                    
                        	 
                     }
                     else
                     {
                       	W_VCPValue = UserPrefBriBase_6500K;
                        W_VCPMax = MAX_BACKLIGHT_PWM;
                        B_VCPType = 1;
                     }
				   	break;
		 		}
		//////////////////////////////////////////////////
		
               if (SetVcp)
               {              
				   if (W_VCPValue > 100)
				   	{
				   	 break; //chunbao some time DDM send Value above 100, so ignore if Value above 100
					  // W_VCPValue = 100; //calo update
				   	}
#ifdef VALUE_MATCH_WITH_OSD
					   vConverT.SDW_SrcMax = OSD_BrightnessMax;
					   vConverT.SDW_SrcMin = OSD_BrightnessMin;
					   vConverT.SDW_AdjValue=  (SDWORD) W_VCPValue;
					   vConverT.SW_DstMaxValue = (SWORD)gm_GetAdjusterMax(ADJV_Brightness);    
					   vConverT.SW_DstMinValue = (SWORD)gm_GetAdjusterMin(ADJV_Brightness);
					   ValueScaleConvert(&vConverT);
					   UserPrefBrightness = (SBYTE) vConverT.SDW_AdjValue;
					   //if (vConverT.SDW_AdjValue < gm_GetAdjusterMax(ADJV_Brightness))
						   //UserPrefBrightness += 1;
#else
                  UserPrefBrightness = W_VCPValue + (WORD)gm_GetAdjusterMin(ADJV_Brightness);
#endif
                  msg_ddc("calling AdjustBrightness [%d]", UserPrefBrightness);
                  AdjustBrightness();
				  //==============120808 Kordonm for DDM change preset mode reload brightness============
				  if(IsPresetMode_GRAPHICS_TEXT)
				  {
					UserPrefBrightness_Text = UserPrefBrightness;					
				  }
				  else if(IsColorSpace_GRAPHICS_CAL1)//120806 Edward add for CAL1/CAL2 save seperate
				  {
					UserPrefBrightness_CAL1 = UserPrefBrightness;					
				   }
				  else if(IsColorSpace_GRAPHICS_CAL2)
				  {
					UserPrefBrightness_CAL2 = UserPrefBrightness;
				   }
				  else
		  		  {
					UserPrefBrightness_All = UserPrefBrightness;
				  }
				  //====================================================================
               }
               else
               {
               
#ifdef VALUE_MATCH_WITH_OSD 		
				   vConverT.SDW_SrcMax = gm_GetAdjusterMax(ADJV_Brightness);
				   vConverT.SDW_SrcMin = gm_GetAdjusterMin(ADJV_Brightness);
				   vConverT.SDW_AdjValue = (SDWORD) UserPrefBrightness;
				   vConverT.SW_DstMaxValue = OSD_BrightnessMax;    
				   vConverT.SW_DstMinValue = OSD_BrightnessMin;
				   ValueScaleConvert(&vConverT);
				   W_VCPValue=(WORD) vConverT.SDW_AdjValue;
				   W_VCPMax=OSD_BrightnessMax-OSD_BrightnessMin;
#else
                  W_VCPValue = UserPrefBrightness - (WORD)gm_GetAdjusterMin(ADJV_Brightness);
                  W_VCPMax = (WORD)gm_GetAdjusterMax(ADJV_Brightness) - (WORD)gm_GetAdjusterMin(ADJV_Brightness);
#endif
                  B_VCPType = 1;		   
               }
               break;
            }
#endif

#ifdef ADJUSTER_RBLACKLEVEL_DECLARED
         case DDC2B_CMD_VCP_Red_Video_Black_Level:
            {
				break;
            }
#endif
#ifdef ADJUSTER_GBLACKLEVEL_DECLARED
         case DDC2B_CMD_VCP_Green_Video_Black_Level:
            {
				break;
            }
#endif
#ifdef ADJUSTER_BBLACKLEVEL_DECLARED
         case DDC2B_CMD_VCP_Blue_Video_Black_Level:
            {
				break;
            }
#endif

        case DDC2B_CMD_VCP_LUT_SIZE:
        {
			switch(W_VCPValue)
			{
			case 0x00:
				W_VCPValue = 0x400;
				W_VCPMax = 0x400;
				B_VCPType = 0;	

			break;
			case 0x01:
				W_VCPValue = 0x400;
				W_VCPMax = 0x400;
				B_VCPType = 0;	

			break;
			case 0x02:
				W_VCPValue = 0x400;
				W_VCPMax = 0x400;
				B_VCPType = 0;	

			break;

			case 0x03:
				W_VCPValue = 0x400;
				W_VCPMax = 0x400;
				B_VCPType = 0;	

			break;

			case 0x04:
				W_VCPValue = 0x400;
				W_VCPMax = 0x400;
				B_VCPType = 0;	

			break;
			}

		break;
        }

        case DDC2B_CMD_VCP_LUT_SAVE:
        {


		break;
        }

#ifdef ADJUSTER_CONTRAST_DECLARED
         case DDC2B_CMD_VCP_Contrast:
            {
               if (SetVcp)
               {            
                  //no need to conv to LittleEndian because
                  //we need to do word to byte conversion anyways
				   if(W_VCPValue < 25)
					 W_VCPValue = 25;

						  
						  if (W_VCPValue > 100)
						  	{
						  	 break; //chunbao some time DDM send Value above 100, so ignore if Value above 100
							 // W_VCPValue = 100; //calo update
						  	}
#ifdef VALUE_MATCH_WITH_OSD
					  vConverT.SDW_SrcMax= OSD_ContrastMax;
					  vConverT.SDW_SrcMin= OSD_ContrastMin;
					  vConverT.SDW_AdjValue= (SDWORD) W_VCPValue;
					  vConverT.SW_DstMaxValue= (SWORD)gm_GetAdjusterMax(ADJV_Contrast);   
					  vConverT.SW_DstMinValue= (SWORD)gm_GetAdjusterMin(ADJV_Contrast);
					  ValueScaleConvert(&vConverT);
					  UserPrefContrast=(SWORD) vConverT.SDW_AdjValue ;
#else
					  UserPrefContrast = W_VCPValue + (SWORD)gm_GetAdjusterMin(ADJV_Contrast);
				#endif        
                  msg_ddc("calling adjustcontrast [%d]", UserPrefContrast);
                  AdjustContrast();			
				  //==============120808 Kordonm for DDM change preset mode reload brightness============
				  if(IsPresetMode_GRAPHICS_TEXT)
				  {
					UserPrefContrast_Text = UserPrefContrast;					
				  }
				  else if(IsColorSpace_GRAPHICS_CAL1)//120806 Edward add for CAL1/CAL2 save seperate
				  {
					UserPrefContrast_CAL1= UserPrefContrast;					
				   }
				  else if(IsColorSpace_GRAPHICS_CAL2)
				  {
					UserPrefContrast_CAL2 = UserPrefContrast;
				   }
				  else
		  		  {
					UserPrefContrast_All = UserPrefContrast;
				  }
				  //====================================================================
               }
               else
               {	               
               
#ifdef VALUE_MATCH_WITH_OSD 		
				   vConverT.SDW_SrcMax= gm_GetAdjusterMax(ADJV_Contrast);
				   vConverT.SDW_SrcMin= gm_GetAdjusterMin(ADJV_Contrast);
				   vConverT.SDW_AdjValue= (SDWORD) UserPrefContrast;
				   vConverT.SW_DstMaxValue= OSD_ContrastMax;   
				   vConverT.SW_DstMinValue= OSD_ContrastMin;
				   ValueScaleConvert(&vConverT);
				   W_VCPValue=(WORD) vConverT.SDW_AdjValue;
				   W_VCPMax=OSD_ContrastMax-OSD_ContrastMin;
#else
                  W_VCPValue = UserPrefContrast - (WORD)gm_GetAdjusterMin(ADJV_Contrast);
                  W_VCPMax = (WORD)gm_GetAdjusterMax(ADJV_Contrast) - (WORD)gm_GetAdjusterMin(ADJV_Contrast);
                  B_VCPType = 0;			  
#endif
               }
               break;
            }
#endif
#ifdef UserPrefColorTemp
  #if 1 //Dell FY13 dispaly VCP Code Definition owen 20120531
      case DDC2B_CMD_VCP_SelectColorPreset:
            {		
               if (SetVcp)
               {               
                  switch (W_VCPValue)
                  {
                      case DDCCI_CTEMP_ADOBERGB: // 20121019, Nicholas Pan
                          UserPrefGraphicsImageMode = Graphics_COLOR_SPACE;
                          UserPrefPresetColorSpace = CSPACE_AdobeRGB;
                          AdjustPresetModes();
                          DisableGameMode();
                          SavePortSchemeDependentSettings();
                          StorePresetModes();
                          break;

					case DDCCI_CTEMP_SRGB:
						  UserPrefGraphicsImageMode = Graphics_COLOR_SPACE;
						  UserPrefPresetColorSpace = CSPACE_sRGB; 
						  AdjustPresetModes();
						  DisableGameMode();
						  SavePortSchemeDependentSettings();
						  StorePresetModes();					  
						  break;
					case DDCCI_CTEMP_5000K    :
						UserPrefGraphicsImageMode = Graphics_COLOR_TEMP;
						AdjustPresetModes();
						StorePresetModes();
						DisableGameMode();	
						UserPrefPresetColorTemp= CTEMP_5000K;
						break;
						
					case DDCCI_CTEMP_5700K    ://5700k
						UserPrefGraphicsImageMode = Graphics_COLOR_TEMP;
						AdjustPresetModes();
						StorePresetModes();
						DisableGameMode();	
						UserPrefPresetColorTemp= CTEMP_5700K;
                        break;
						
                     case DDCCI_CTEMP_6500K    :
						UserPrefGraphicsImageMode = Graphics_COLOR_TEMP;
					   	AdjustPresetModes();
						StorePresetModes();
						DisableGameMode();	
						UserPrefPresetColorTemp= CTEMP_6500K;
                        break;
						
                     case DDCCI_CTEMP_7500K    :
					    UserPrefGraphicsImageMode = Graphics_COLOR_TEMP;
						AdjustPresetModes();
						StorePresetModes();
						DisableGameMode();	
						UserPrefPresetColorTemp= CTEMP_7500K;
                        break;
						
                     case DDCCI_CTEMP_9300K        :
						UserPrefGraphicsImageMode = Graphics_COLOR_TEMP;
						AdjustPresetModes();
						StorePresetModes();
						DisableGameMode();	
						UserPrefPresetColorTemp = CTEMP_9300K;
                        break;
						
                     case DDCCI_CTEMP_10000K    :
					    UserPrefGraphicsImageMode = Graphics_COLOR_TEMP;
						AdjustPresetModes();
						StorePresetModes();
						DisableGameMode();	
						UserPrefPresetColorTemp = CTEMP_10000K;
                        break;
						
					case DDCCI_CTEMP_USER1:	//Custom Color
						UserPrefGraphicsImageMode = Graphics_CUSTOM_COLOR;	
						AdjustPresetModes();
						SavePortSchemeDependentSettings();
						StorePresetModes();
						DisableGameMode();
                        break;
				case DDCCI_CTEMP_USER2    :  //for 7200K 20121010
					    UserPrefGraphicsImageMode = Graphics_COLOR_TEMP;
						AdjustPresetModes();
						StorePresetModes();
						DisableGameMode();	
						UserPrefPresetColorTemp = CTEMP_7200K;
                        break;
                  }

					msg_ddc("calling adjustcolor_temp [%d]", UserPrefPresetColorTemp);
					AdjustColorTemp();	
					SavePortSchemeDependentSettings();
               }
               else
               { 
            		switch (UserPrefGraphicsImageMode)
            		{
                   		case Graphics_CUSTOM_COLOR:
							W_VCPValue = DDCCI_CTEMP_USER1;//Custom Color   	
		     				break;

                   		case Graphics_COLOR_SPACE:
						if(UserPrefPresetColorSpace == CSPACE_sRGB)
                            {
							W_VCPValue = DDCCI_CTEMP_SRGB;
                            }
					  
                            else if(UserPrefPresetColorSpace == CSPACE_AdobeRGB) // 20121019, Nicholas Pan
                            {
                                W_VCPValue = DDCCI_CTEMP_ADOBERGB;
                            }
						break;

		     		case Graphics_COLOR_TEMP:	   
                   			switch (UserPrefPresetColorTemp)
                   			{
                     			case CTEMP_5000K    :
                        			W_VCPValue = DDCCI_CTEMP_5000K;
                        			break;

                     			case CTEMP_5700K    ://5700k
                        			W_VCPValue = DDCCI_CTEMP_5700K;
                        			break;

                     			case CTEMP_6500K    :
                        			W_VCPValue = DDCCI_CTEMP_6500K;
                        			break;

                     			case CTEMP_7500K    :
                        			W_VCPValue = DDCCI_CTEMP_7500K;
                        			break;

                     			case CTEMP_9300K    :
                        			W_VCPValue = DDCCI_CTEMP_9300K;
                        			break;

                     			case CTEMP_10000K    :
                        			W_VCPValue = DDCCI_CTEMP_10000K;
                        			break;

                     			case CTEMP_USER:
                        			W_VCPValue = DDCCI_CTEMP_USER1;
                        			break;									
						case CTEMP_7200K    :   //20121010
                        			W_VCPValue = DDCCI_CTEMP_USER2;
                        			break;
                     			default    :
                        			W_VCPValue = DDCCI_CTEMP_6500K;
                        			break;		
                    		}					
                     	break;
               		}
                  B_VCPType    = 0x01;    //Set Parameter
#if 1
                  W_VCPMax = 0xFFFF;    //this is the number of possible choices for this non-continuous VCP value
#else
                  W_VCPMax = 3;    //this is the number of possible choices for this non-continuous VCP value
#endif
               }
               break;
            }
  #else
         case DDC2B_CMD_VCP_SelectColorPreset:
            {
               if (SetVcp)
               {               
                  switch (W_VCPValue)
                  {
                     case DDCCI_CTEMP_6500K    :
                        UserPrefColorTemp= CTEMP_6500K;
                        break;
                     case DDCCI_CTEMP_7500K    :
                        UserPrefColorTemp= CTEMP_7500K;
                        break;
                     case DDCCI_CTEMP_9300K        :
                        UserPrefColorTemp = CTEMP_9300K;
                        break;
                     //case DDCCI_CTEMP_11500K    :
                     //   UserPrefColorTemp = CTEMP_11500K;
                     //   break;
                     case DDCCI_CTEMP_USER1     :
                        UserPrefColorTemp = CTEMP_USER;//120502 Edward
                        break;
                  }

                  msg_ddc("calling adjustcolor_temp [%d]", UserPrefColorTemp);
                  AdjustColorTemp();			  
               }
               else
               { 
                  switch (UserPrefColorTemp)
                  {
                     case CTEMP_6500K    :
                        W_VCPValue = DDCCI_CTEMP_6500K;
                        break;

                     case CTEMP_7500K    :
                        W_VCPValue = DDCCI_CTEMP_7500K;
                        break;

                     case CTEMP_9300K    :
                        W_VCPValue = DDCCI_CTEMP_9300K;
                        break;

                     //case CTEMP_11500K    :
                     //   W_VCPValue = DDCCI_CTEMP_11500K;
                     //   break;
                        /*case CTEMP_USER     :
                            W_VCPValue = DDCCI_CTEMP_USER1;
                            UserPrefColorTemp
                            break;*/
                        //The below default task was added to pass Windows DTM
                        //Without it, DTM tries to set UserPrefColorTemp to 0
                        //As well the above case was commented out to pass DTM
                        //This is a known Microsoft bug
                     default    :
                        W_VCPValue = DDCCI_CTEMP_6500K;
                        break;

                  }
                  B_VCPType    = 0x01;    //Set Parameter
#if 1
                  W_VCPMax = 0xFFFF;    //this is the number of possible choices for this non-continuous VCP value
#else
                  W_VCPMax = 3;    //this is the number of possible choices for this non-continuous VCP value
#endif
               }
               break;
            }
#endif
#endif
#ifdef Action_HideOsd_WB_Used
         case DDC2B_CMD_VCP_OSD:
            {
               if (SetVcp)
               {               
                  if (W_VCPValue == 1) // hide osd if it's on
                  {
                     gm_OsdHide();
                  }
                  else if (W_VCPValue == 2) //show osd
                  {
                     gm_OsdShow();
                  }			  
               }
               else
               {                
                  B_VCPType = 0;
                  W_VCPMax = 2;			  
               }
               break;
            }
#endif

#if FEATURE_AIP == ENABLE 

#ifdef ADJUSTER_HSTART_DECLARED
         case DDC2B_CMD_VCP_HPosition:
            {            
			#ifdef DELL_U2410F
				//Lvjie 20081028 moved from 2x09
				if (!IsVGA()) // softMCCS did't check string and enable this when DVI inputs ...
				{
					IsFeatureSupported = FALSE;
					break;
				}

				if (SetVcp) 
				{					
				if (W_VCPValue > 100)
					W_VCPValue = 100; //calo update
					msg_ddc("Set H Position [%d]", W_VCPValue);		
					vConverT.SDW_SrcMax = OSD_DEFAULT_MAX_VALUE;
					vConverT.SDW_SrcMin = OSD_DEFAULT_MIN_VALUE;
					vConverT.SDW_AdjValue = (SDWORD) (OSD_DEFAULT_MAX_VALUE - W_VCPValue);
					vConverT.SW_DstMaxValue = (SWORD)MaxHStart();
					vConverT.SW_DstMinValue = (SWORD)MinHStart();
					ValueScaleConvert(&vConverT);
					UserPrefHSyncDelay = (WORD) vConverT.SDW_AdjValue;

					AdjustHStart();

				#if 0
					OSDRequestMsg(gmvb_MainChannel, MTO_RemoveMenu, REQUEST_NOW);
				#endif
				}
				else
				{
					vConverT.SDW_SrcMax = MaxHStart();
					vConverT.SDW_SrcMin = MinHStart();
					vConverT.SDW_AdjValue = (SDWORD) UserPrefHSyncDelay;
					vConverT.SW_DstMaxValue = OSD_DEFAULT_MAX_VALUE;
					vConverT.SW_DstMinValue = OSD_DEFAULT_MIN_VALUE;
					ValueScaleConvert(&vConverT);
					W_VCPValue = (WORD) (OSD_DEFAULT_MAX_VALUE - vConverT.SDW_AdjValue);
					W_VCPMax = OSD_DEFAULT_MAX_VALUE - OSD_DEFAULT_MIN_VALUE;

					B_VCPType = 0;	//Set Parameter
				}
				break;
			#else
               if (SetVcp)
               {               
                  msg_ddc("Set H Position [%d]", W_VCPValue);
                  UserPrefHSyncDelay = MaxHStart() - W_VCPValue;
                  AdjustHStart();			  
               }
               else
               {
                  W_VCPValue = MaxHStart() - UserPrefHSyncDelay;
                  W_VCPMax = MaxHStart() -MinHStart();
               }
				break;	//Lvjie 20081028
			#endif
				
            }
#endif

#ifdef ADJUSTER_VSTART_DECLARED
         case DDC2B_CMD_VCP_VPosition:
            {
			#ifdef DELL_U2410F
				//Lvjie 20081028 moved from 2x09
				if (!IsVGA()) // softMCCS did't check string and enable this when DVI inputs ...
				{
					IsFeatureSupported = FALSE;
					break;
				}

				if (SetVcp)
				{				
				if (W_VCPValue > 100)
					W_VCPValue = 100; //calo update
					vConverT.SDW_SrcMax = OSD_DEFAULT_MAX_VALUE;
					vConverT.SDW_SrcMin = OSD_DEFAULT_MIN_VALUE;
					vConverT.SDW_AdjValue = (SDWORD) W_VCPValue;
					vConverT.SW_DstMaxValue = (SWORD)MaxVStart();	
					vConverT.SW_DstMinValue = (SWORD)MinVStart();
					ValueScaleConvert(&vConverT);
					UserPrefVSyncDelay = (WORD) vConverT.SDW_AdjValue;

					AdjustVStart();            
				}
				else 
				{
					vConverT.SDW_SrcMax = MaxVStart();
					vConverT.SDW_SrcMin = MinVStart();
					vConverT.SDW_AdjValue = (SDWORD) UserPrefVSyncDelay;
					vConverT.SW_DstMaxValue = OSD_DEFAULT_MAX_VALUE;	
					vConverT.SW_DstMinValue = OSD_DEFAULT_MIN_VALUE;
					ValueScaleConvert(&vConverT);
					W_VCPValue = (WORD) vConverT.SDW_AdjValue;
					W_VCPMax = OSD_DEFAULT_MAX_VALUE - OSD_DEFAULT_MIN_VALUE;
					B_VCPType = 0;	//Set Parameter
				}
				break;				
			#else
               if (SetVcp)
               {             
                  msg_ddc("Set V Position [%d]", W_VCPValue);
                  UserPrefVSyncDelay = W_VCPValue + MinVStart();
                  AdjustVStart();			  
               }
               else
               {
                  W_VCPValue = UserPrefVSyncDelay - MinVStart();
                  W_VCPMax = MaxVStart() - MinVStart();
               }
               break;
 			#endif
            }
#endif

#ifdef Action_AdjustHTotal_WB_Used
         case DDC2B_CMD_VCP_Clock:
            {
			#ifdef DELL_U2410F
				//Lvjie 20081028 moved from 2x09
				if (!IsVGA()) // softMCCS did't check string and enable this when DVI inputs ...
				{
					IsFeatureSupported = FALSE;
					break;
				}

				if (SetVcp)
				{				
				if (W_VCPValue > 100)
					W_VCPValue = 100; //calo update
					vConverT.SDW_SrcMax = OSD_DEFAULT_MAX_VALUE;
					vConverT.SDW_SrcMin = OSD_DEFAULT_MIN_VALUE;
					vConverT.SDW_AdjValue = (SDWORD) W_VCPValue;
					vConverT.SW_DstMaxValue = (SWORD)MaxHTotal();
					vConverT.SW_DstMinValue = (SWORD)MinHTotal();
					ValueScaleConvert(&vConverT);
					UserPrefHTotal = (WORD) vConverT.SDW_AdjValue;

					AdjustHTotal();
				}
				else
				{
					vConverT.SDW_SrcMax = MaxHTotal();
					vConverT.SDW_SrcMin = MinHTotal();
					vConverT.SDW_AdjValue = (SDWORD) UserPrefHTotal;
					vConverT.SW_DstMaxValue = OSD_DEFAULT_MAX_VALUE;
					vConverT.SW_DstMinValue = OSD_DEFAULT_MIN_VALUE;
					ValueScaleConvert(&vConverT);
					W_VCPValue = (WORD) vConverT.SDW_AdjValue;
					W_VCPMax = OSD_DEFAULT_MAX_VALUE - OSD_DEFAULT_MIN_VALUE;
					B_VCPType = 0;	//Set Parameter
				}
				break;
			#else
               if (SetVcp)
               {               
                  UserPrefHTotal = W_VCPValue + MinHTotal();
                  AdjustHTotal();			  
               }
               else
               {
                  W_VCPValue = UserPrefHTotal - MinHTotal();
                  W_VCPMax = MaxHTotal() - MinHTotal();
               }
               break;
			#endif
            }
#endif

#ifdef Action_AdjustHSyncPhase_WB_Used
         case DDC2B_CMD_VCP_ClockPhase:
            {
			#ifdef DELL_U2410F
				//Lvjie 20081028 moved from 2x09
				if (!IsVGA()) // softMCCS did't check string and enable this when DVI inputs ...
				{
					IsFeatureSupported = FALSE;
					break;
				}

				if (SetVcp)
				{				
				if (W_VCPValue > 100)
					W_VCPValue = 100; //calo update
					vConverT.SDW_SrcMax = OSD_DEFAULT_MAX_VALUE;
					vConverT.SDW_SrcMin = OSD_DEFAULT_MIN_VALUE;
					vConverT.SDW_AdjValue = (SDWORD) W_VCPValue;
					//vConverT.SW_DstMaxValue = (SWORD)0x3F;
					vConverT.SW_DstMaxValue = (SWORD)0x64;//calo update
					vConverT.SW_DstMinValue = (SWORD)0;
					ValueScaleConvert(&vConverT);
					UserPrefHSyncPhase = (WORD) vConverT.SDW_AdjValue;
					AdjustHSyncPhase();
				}
				else
				{
					//vConverT.SDW_SrcMax = 0x3F;
					vConverT.SDW_SrcMax = 0x64;//calo update
					vConverT.SDW_SrcMin = 0;
					vConverT.SDW_AdjValue = (SDWORD) UserPrefHSyncPhase;
					vConverT.SW_DstMaxValue = OSD_DEFAULT_MAX_VALUE;
					vConverT.SW_DstMinValue = OSD_DEFAULT_MIN_VALUE;
					ValueScaleConvert(&vConverT);
					W_VCPValue = (WORD) vConverT.SDW_AdjValue;
					W_VCPMax = OSD_DEFAULT_MAX_VALUE - OSD_DEFAULT_MIN_VALUE;
					B_VCPType = 0;	//Set Parameter
				}
				break;
			#else
               if (SetVcp)
               {              
                  UserPrefHSyncPhase = W_VCPValue;
                  AdjustHSyncPhase();			  
               }
               else
               {
                  W_VCPValue = UserPrefHSyncPhase;
                  W_VCPMax = 0x3F;
               }
               break;
			#endif
            }
#endif
#endif // FEATURE_AIP == ENABLE 

#ifdef ADJUSTER_USERREDGAIN_DECLARED
         case DDC2B_CMD_VCP_RedGain:
            {

		 if(UserPrefFactoryFlag)
		 	{
                   if (SetVcp)  //set value
	               {

                       if((UserPrefGraphicsImageMode == Graphics_COLOR_SPACE)&&( UserPrefPresetColorSpace == CSPACE_sRGB))
                        UserPrefsRGBColor_R= W_VCPValue;   

			else
				{
	                if(UserPrefPresetColorTemp == CTEMP_5000K)
			     UserPrefColorT5000_R = W_VCPValue;
		        else if(UserPrefPresetColorTemp == CTEMP_5700K)
			     UserPrefColorT5700_R = W_VCPValue;
		        else if(UserPrefPresetColorTemp == CTEMP_6500K)
			     UserPrefColorT6500_R = W_VCPValue; 
		        else if(UserPrefPresetColorTemp == CTEMP_7500K)
			     UserPrefColorT7500_R = W_VCPValue;
		        else if(UserPrefPresetColorTemp == CTEMP_9300K)
			     UserPrefColorT9300_R = W_VCPValue;
		        else if(UserPrefPresetColorTemp == CTEMP_10000K)
			    UserPrefColorT10000_R = W_VCPValue;
		        else if(UserPrefPresetColorTemp == CTEMP_USER)
			    UserPrefUserRedGain = ((WORD)W_VCPValue * 100)/255;		
			 else if(UserPrefPresetColorTemp == CTEMP_7200K) //20121010
			 	UserPrefColorT7200_R = W_VCPValue;
			 
				}
			 
	                CscUpdateColors(gmvb_MainChannel);
					SaveFactorySettingsInNVR();
                       W_VCPMax=MAX_F_CT_GAIN;
                   }
		  else  //Get value
                  {   
                       if((UserPrefGraphicsImageMode == Graphics_COLOR_SPACE)&&( UserPrefPresetColorSpace == CSPACE_sRGB))
				  W_VCPValue=UserPrefsRGBColor_R;
			  else
			  	{
                       if(UserPrefPresetColorTemp == CTEMP_5000K)
			     W_VCPValue=UserPrefColorT5000_R ;
		        else if(UserPrefPresetColorTemp == CTEMP_5700K)
			     W_VCPValue=UserPrefColorT5700_R ;
		        else if(UserPrefPresetColorTemp == CTEMP_6500K)
			     W_VCPValue=UserPrefColorT6500_R ; 
		        else if(UserPrefPresetColorTemp == CTEMP_7500K)
			     W_VCPValue=UserPrefColorT7500_R;
		        else if(UserPrefPresetColorTemp == CTEMP_9300K)
			     W_VCPValue=UserPrefColorT9300_R ;
		        else if(UserPrefPresetColorTemp == CTEMP_10000K)
			    W_VCPValue=UserPrefColorT10000_R ;
		        else if(UserPrefPresetColorTemp == CTEMP_USER)
			    W_VCPValue = ConvertRGBGain(UserPrefUserRedGain) ;	

				
			 else if(UserPrefPresetColorTemp == CTEMP_7200K)//20121010
			    W_VCPValue=UserPrefColorT7200_R ;
			  	}
                        W_VCPMax = MAX_F_CT_GAIN;
                        B_VCPType = 0;			  
                  }
		  break;
		 }
		 
               #endif  
				
               if (SetVcp)
               {           
			      if(W_VCPValue < 30)
				  	W_VCPValue = 30;
				  if (UserPrefColorTemp != CTEMP_USER)
					{
						UserPrefColorTemp = CTEMP_USER;
						AdjustColorMode();
					}
					if ((OSD_IsGraphics) && (UserPrefGraphicsImageMode != Graphics_CUSTOM_COLOR))
					{
						UserPrefGraphicsImageMode = Graphics_CUSTOM_COLOR;
					    AdjustPresetModes();//calo update
					    StorePresetModes();
					}
                  UserPrefUserRedGain = W_VCPValue + (WORD)gm_GetAdjusterMin(ADJV_UserRedGain);
                  AdjustRedGain();			  
               }
               else
               {               
                  W_VCPValue = UserPrefUserRedGain - (WORD)gm_GetAdjusterMin(ADJV_UserRedGain);
                  W_VCPMax = (WORD)gm_GetAdjusterMax(ADJV_UserRedGain) - (WORD)gm_GetAdjusterMin(ADJV_UserRedGain);
                  B_VCPType = 0;			  
               }
               break;
            }

#ifdef ADJUSTER_USERGREENGAIN_DECLARED
         case DDC2B_CMD_VCP_GreenGain:
            {

		 if(UserPrefFactoryFlag)
		 	{
                   if (SetVcp)  //set value
	               {
                       if((UserPrefGraphicsImageMode == Graphics_COLOR_SPACE)&&( UserPrefPresetColorSpace == CSPACE_sRGB))
                        UserPrefsRGBColor_G= W_VCPValue;  
			 else
			 	{
	                if(UserPrefPresetColorTemp == CTEMP_5000K)
			     UserPrefColorT5000_G = W_VCPValue;
		        else if(UserPrefPresetColorTemp == CTEMP_5700K)
			     UserPrefColorT5700_G = W_VCPValue;
		        else if(UserPrefPresetColorTemp == CTEMP_6500K)
			     UserPrefColorT6500_G = W_VCPValue; 
		        else if(UserPrefPresetColorTemp == CTEMP_7500K)
			     UserPrefColorT7500_G = W_VCPValue;
		        else if(UserPrefPresetColorTemp == CTEMP_9300K)
			     UserPrefColorT9300_G = W_VCPValue;
		        else if(UserPrefPresetColorTemp == CTEMP_10000K)
			    UserPrefColorT10000_G = W_VCPValue;
		        else if(UserPrefPresetColorTemp == CTEMP_USER)
			    UserPrefUserGreenGain = ((WORD)W_VCPValue*100)/255;		

				
			  else if(UserPrefPresetColorTemp == CTEMP_7200K)//20121010
			    UserPrefColorT7200_G = W_VCPValue;
			 	}
	                CscUpdateColors(gmvb_MainChannel);
					SaveFactorySettingsInNVR();
                        W_VCPMax=MAX_F_CT_GAIN;
                   }
		  else  //Get value
                  {   
                   if((UserPrefGraphicsImageMode == Graphics_COLOR_SPACE)&&( UserPrefPresetColorSpace == CSPACE_sRGB))
                        W_VCPValue=UserPrefsRGBColor_G;  
			else
				{
                       if(UserPrefPresetColorTemp == CTEMP_5000K)
			     W_VCPValue=UserPrefColorT5000_G ;
		        else if(UserPrefPresetColorTemp == CTEMP_5700K)
			     W_VCPValue=UserPrefColorT5700_G ;
		        else if(UserPrefPresetColorTemp == CTEMP_6500K)
			     W_VCPValue=UserPrefColorT6500_G ; 
		        else if(UserPrefPresetColorTemp == CTEMP_7500K)
			     W_VCPValue=UserPrefColorT7500_G;
		        else if(UserPrefPresetColorTemp == CTEMP_9300K)
			     W_VCPValue=UserPrefColorT9300_G ;
		        else if(UserPrefPresetColorTemp == CTEMP_10000K)
			    W_VCPValue=UserPrefColorT10000_G ;
		        else if(UserPrefPresetColorTemp == CTEMP_USER)
			    W_VCPValue = ConvertRGBGain(UserPrefUserGreenGain);		

				
			 else if(UserPrefPresetColorTemp == CTEMP_7200K)//20121010
			    W_VCPValue=UserPrefColorT7200_G ;
				}
                        W_VCPMax = MAX_F_CT_GAIN;
                        B_VCPType = 0;			  
                  }
		  break;
		 }
		 
               if (SetVcp)
               {                
		      if(W_VCPValue < 30)
			  	W_VCPValue = 30;
			  if (UserPrefColorTemp != CTEMP_USER)
				{
					UserPrefColorTemp = CTEMP_USER;
					AdjustColorMode();
				}
				if ((OSD_IsGraphics) && (UserPrefGraphicsImageMode != Graphics_CUSTOM_COLOR))
				{
					UserPrefGraphicsImageMode = Graphics_CUSTOM_COLOR;
				    AdjustPresetModes();//calo update
				    StorePresetModes();
				}
                  UserPrefUserGreenGain = W_VCPValue + (WORD)gm_GetAdjusterMin(ADJV_UserGreenGain);
                  AdjustGreenGain();			  
               }
               else
               {               
                  W_VCPValue = UserPrefUserGreenGain - (WORD)gm_GetAdjusterMin(ADJV_UserGreenGain);
                  W_VCPMax = (WORD)gm_GetAdjusterMax(ADJV_UserGreenGain) - (WORD)gm_GetAdjusterMin(ADJV_UserGreenGain);
                  B_VCPType = 0;			  
               }
               break;
            }
#endif

#ifdef ADJUSTER_USERBLUEGAIN_DECLARED
         case DDC2B_CMD_VCP_BlueGain:
            {


		 if(UserPrefFactoryFlag)
		 	{
                   if (SetVcp)  //set value
	               {
                          if((UserPrefGraphicsImageMode == Graphics_COLOR_SPACE)&&( UserPrefPresetColorSpace == CSPACE_sRGB))
                        UserPrefsRGBColor_B= W_VCPValue; 
			else
				{
	                if(UserPrefPresetColorTemp == CTEMP_5000K)
			     UserPrefColorT5000_B = W_VCPValue;
		        else if(UserPrefPresetColorTemp == CTEMP_5700K)
			     UserPrefColorT5700_B = W_VCPValue;
		        else if(UserPrefPresetColorTemp == CTEMP_6500K)
			     UserPrefColorT6500_B = W_VCPValue; 
		        else if(UserPrefPresetColorTemp == CTEMP_7500K)
			     UserPrefColorT7500_B = W_VCPValue;
		        else if(UserPrefPresetColorTemp == CTEMP_9300K)
			     UserPrefColorT9300_B = W_VCPValue;
		        else if(UserPrefPresetColorTemp == CTEMP_10000K)
			    UserPrefColorT10000_B = W_VCPValue;
		        else if(UserPrefPresetColorTemp == CTEMP_USER)
			    UserPrefUserBlueGain = ((WORD)W_VCPValue*100)/255;				
			 else if(UserPrefPresetColorTemp == CTEMP_7200K)
			    UserPrefColorT7200_B = W_VCPValue; //20121010
				}
	                CscUpdateColors(gmvb_MainChannel);
					SaveFactorySettingsInNVR();
                       W_VCPMax=MAX_F_CT_GAIN;
                   }
		  else  //Get value
                  {   
                     if((UserPrefGraphicsImageMode == Graphics_COLOR_SPACE)&&( UserPrefPresetColorSpace == CSPACE_sRGB))
                         W_VCPValue=UserPrefsRGBColor_G; 
			else
			  {
                       if(UserPrefPresetColorTemp == CTEMP_5000K)
			     W_VCPValue=UserPrefColorT5000_B;
		        else if(UserPrefPresetColorTemp == CTEMP_5700K)
			     W_VCPValue=UserPrefColorT5700_B ;
		        else if(UserPrefPresetColorTemp == CTEMP_6500K)
			     W_VCPValue=UserPrefColorT6500_B ; 
		        else if(UserPrefPresetColorTemp == CTEMP_7500K)
			     W_VCPValue=UserPrefColorT7500_B;
		        else if(UserPrefPresetColorTemp == CTEMP_9300K)
			     W_VCPValue=UserPrefColorT9300_B ;
		        else if(UserPrefPresetColorTemp == CTEMP_10000K)
			    W_VCPValue=UserPrefColorT10000_B ;		      
		        else if(UserPrefPresetColorTemp == CTEMP_USER)
			    W_VCPValue= ConvertRGBGain(UserPrefUserBlueGain);		
			else if(UserPrefPresetColorTemp == CTEMP_7200K) //20121010
			    W_VCPValue=UserPrefColorT7200_B ;
                     	}
                        W_VCPMax = MAX_F_CT_GAIN;
                        B_VCPType = 0;			  
                  }
		  break;
		 }
		 


               if (SetVcp)
               {              
			      if(W_VCPValue < 30)
				  	W_VCPValue = 30;
				  if (UserPrefColorTemp != CTEMP_USER)
					{
						UserPrefColorTemp = CTEMP_USER;
						AdjustColorMode();
					}
					if ((OSD_IsGraphics) && (UserPrefGraphicsImageMode != Graphics_CUSTOM_COLOR))
					{
						UserPrefGraphicsImageMode = Graphics_CUSTOM_COLOR;
					    AdjustPresetModes();//calo update
					    StorePresetModes();
					}
					
                  UserPrefUserBlueGain = W_VCPValue + (WORD)gm_GetAdjusterMin(ADJV_UserBlueGain);
                  AdjustBlueGain();			  
               }
               else
               {	               
                  W_VCPValue = UserPrefUserBlueGain - (WORD)gm_GetAdjusterMin(ADJV_UserBlueGain);
                  W_VCPMax = (WORD)gm_GetAdjusterMax(ADJV_UserBlueGain) - (WORD)gm_GetAdjusterMin(ADJV_UserBlueGain);
                  B_VCPType = 0;			  
               }
               break;
            }
#endif

#ifdef ADJUSTER_SHARPNESS_DECLARED
         case DDC2B_CMD_VCP_TVSharpness:
            {
               if (SetVcp)
               {           
                  UserPrefSharpness= W_VCPValue;
                  AdjustSharpness();			  
               }
               else
               {	   
                  W_VCPValue = UserPrefSharpness;
               }
               break;
            }
#endif

#ifdef ADJUSTER_SATURATION_DECLARED
         case DDC2B_CMD_VCP_TVSaturation:
            {
               if (SetVcp)
               {              
                  UserPrefSaturation = W_VCPValue;
                  AdjustSaturation();			  
               }
               else
               {              
                  W_VCPValue = UserPrefSaturation;
               }
               break;
            }
#endif

#ifdef Action_AutoAdjust_WB_Used
#if FEATURE_AIP == ENABLE

         case DDC2B_CMD_VCP_AutoSetUp:
            {
               if (SetVcp && W_VCPValue)
               {              
                  ModeSetupAutoAdjust(gmvb_MainChannel);
               }
               else
               {
                  W_VCPMax = 1;
                  B_VCPType = 1;
               }
               break;
            }
#endif
#endif


#ifdef ADJUSTER_OSDLANGUAGE_DECLARED
         case DDC2B_CMD_VCP_OsdLanguage:
            {
               if (SetVcp)
               {               
                  switch (W_VCPValue)
                  {
                     case 1://chinese
                        UserPrefOsdLanguage = 0;
                        AdjustOsdLanguage();
                        break;
                     case 2: //english
                        UserPrefOsdLanguage = 1;
                        AdjustOsdLanguage();
                  }			  
               }
               else
               {                              
                  if (UserPrefOsdLanguage == 1)
                     W_VCPValue = 1;
                  else if (UserPrefOsdLanguage == 0)
                     W_VCPValue = 2;
                  else W_VCPValue = 0;

                  W_VCPMax = 11;			  
               }
               break;
            }
#endif

            // TODO: JasonL - revisit these functions and implement

#ifdef ADJUSTER_CURRENTINPUTMAIN_DECLARED
         case DDC2B_CMD_VCP_SourceSelect:
            {
               B_Temp = UserPrefCurrentInputMain;
               if (SetVcp)
               {
                  switch (W_VCPValue)
                  {
					  case VCP_SourceSelect_DigitalVideo_1:
						B_Temp = ALL_BOARDS_DVI1;
						break;
					  case VCP_SourceSelect_HdmiVideo_1:
						  B_Temp = ALL_BOARDS_HDMI;
						  break;
					  case VCP_SourceSelect_DisplayPort_1:
						  B_Temp = ALL_BOARDS_DP1;
						  break;
					  case VCP_SourceSelect_DisplayPort_2:
						  B_Temp = ALL_BOARDS_DP3;
						  break;
						
                     case VCP_SourceSelect_AnalogVideo_1:
                        B_Temp = ALL_BOARDS_VGA1;
	                    break;

                     case VCP_SourceSelect_AnalogVideo_2:
                     case VCP_SourceSelect_DigitalVideo_2:
					 case VCP_SourceSelect_CompositeVideo_1:
                     case VCP_SourceSelect_CompositeVideo_2:
					 case VCP_SourceSelect_SVideo_1:
                     case VCP_SourceSelect_SVideo_2:
                     case VCP_SourceSelect_Tuner_1:
                     case VCP_SourceSelect_Tuner_2:
                     case VCP_SourceSelect_Tuner_3:
					 case VCP_SourceSelect_ComponentVideo_1:				 	
                     case VCP_SourceSelect_ComponentVideo_2:
                     case VCP_SourceSelect_ComponentVideo_3:
                     default:
                        IsFeatureSupported = FALSE;
                        break;
                  }

                  if (B_Temp != UserPrefCurrentInputMain)
                  {
                     UserPrefCurrentInputMain = B_Temp;
                     AdjustCurrentInputMain();
                  }
               }
               else
               {
                  //W_VCPMax = VCP_SourceSelect_ComponentVideo_1;
				  W_VCPMax = 0xFFFF;
				  switch(UserPrefCurrentInputMain) 
				  {
					  case ALL_BOARDS_VGA1:
						  W_VCPValue = VCP_SourceSelect_AnalogVideo_1;
						  break;
					  case ALL_BOARDS_DVI1:
						  W_VCPValue = VCP_SourceSelect_DigitalVideo_1;
						  break;
				  
					  case ALL_BOARDS_HDMI:
						  W_VCPValue = VCP_SourceSelect_HdmiVideo_1;
						  break;
				  
					  case ALL_BOARDS_DP1:
						  W_VCPValue = VCP_SourceSelect_DisplayPort_1;
						  break;
						  
					  case ALL_BOARDS_DP2:
						  W_VCPValue = VCP_SourceSelect_DisplayPort_2;
						  break;
					  default:
						  IsFeatureSupported = FALSE;
						  break;
                  }
               }
               break;
            }
#endif


#ifdef ADJUSTER_OSDLANGUAGE_DECLARED
         case DDC2B_CMD_VCP_Language:
            {
               if (SetVcp)
               {           
                  if (W_VCPValue)
                  {
                     UserPrefOsdLanguage = W_VCPValue - 1;
                     AdjustOsdLanguage();
                  }			  
               }
               else
               {
                  W_VCPValue = UserPrefOsdLanguage + 1;
                  W_VCPMax = 2;			  
               }
               break;
            }
#endif

         case DDC2B_CMD_VCP_Settings:
            {
               if (SetVcp)
               {
                  SaveModeIndependentSettings();
               #if FEATURE_AIP == ENABLE
                  LookUpType.W_Hfreq = stInput[gmvb_MainChannel].Hfrq;
                  LookUpType.W_Vfreq = stInput[gmvb_MainChannel].Vfrq;
                  LookUpType.W_Vtotal = stInput[gmvb_MainChannel].Vtotal;
                  SaveModeDependentSettings(gmvb_MainChannel, &LookUpType);
               #endif
               }
               else
               {
                  B_VCPType =1;
                  W_VCPMax =1;
               }
               break;
            }






 case DDC2B_CMD_VCP_PresetMode://owen 20120531
 	  {
               if (SetVcp)
               {
               	  //120910 Edward add for DDM change preset mode not apply to main
				  if(!Is_DDCCI_Command_SubChannel(DDCCI_Channel))
				  {
					switch (W_VCPValue)
					{
						case 0:						
							UserPrefGraphicsImageMode = Graphics_STANDARD;	
							PresetModeSettings();
							if(PrevUserPrefGraphicsImageMode == Graphics_GAME)
							{							
								ReadPresetModesSettings();
								DisableGameMode();
							}
							else
							AdjustPresetModes();
							SavePortSchemeDependentSettings();
							StorePresetModes();
							PrevUserPrefGraphicsImageMode=UserPrefGraphicsImageMode;
							break;

						case 2:
							UserPrefGraphicsImageMode = Graphics_MULTIMEDIA;	
							PresetModeSettings();
							if(PrevUserPrefGraphicsImageMode == Graphics_GAME)
							{							
								ReadPresetModesSettings();
								DisableGameMode();
							}
							else
							AdjustPresetModes();
							SavePortSchemeDependentSettings();
							StorePresetModes();
							PrevUserPrefGraphicsImageMode=UserPrefGraphicsImageMode;
							break;

						case 3:
							UserPrefGraphicsImageMode = Graphics_MOVIE;	
							PresetModeSettings();
							if(PrevUserPrefGraphicsImageMode == Graphics_GAME)
							{							
								ReadPresetModesSettings();
								DisableGameMode();
							}
							else
							AdjustPresetModes();
							SavePortSchemeDependentSettings();
							StorePresetModes();
							PrevUserPrefGraphicsImageMode=UserPrefGraphicsImageMode;
							break;		 
		 
						case 5:
							UserPrefGraphicsImageMode = Graphics_GAME;	
							PresetModeSettings();
							ReadPresetModesSettings();
							SavePortSchemeDependentSettings();
							StorePresetModes();
							EnableGameMode();
							if (!IsGameModeEnable())	//20121115 yvonne don't load presetmode twice
								AdjustPresetModes();
							PrevUserPrefGraphicsImageMode=UserPrefGraphicsImageMode;
			                break;
							
						default:			
						break;	    		   
                  }
				 }	
               }
	        else 
	        {
	            switch (UserPrefGraphicsImageMode)
                  {
                     case Graphics_STANDARD:
	            		W_VCPValue=0;//Standard
						break;		 

					case Graphics_MULTIMEDIA:
	            		W_VCPValue=2;//Multimedia
						break;		 

					case Graphics_MOVIE:
	            		W_VCPValue=3;//Movie
						break;		 


					case Graphics_GAME:
	            		W_VCPValue=5;//Game
						break;	

					default:
					break;	
	            }			
	        }
               break;		
 	  }
	case DDC2B_CMD_VCP_AutoRotation:
	{
		if(SetVcp == GET_VCP)
		{
			W_VCPValue=0x00;

			#ifdef PIVOT_SENSOR_ENABLE//120528 Edward update
			if(UserPrefOsdRotation_Auto == ON)
			{
			if(Is_SENSOR_ROTATE90())
				W_VCPValue|=0x02;
			else
				W_VCPValue|=0x01;
			}
			else
				W_VCPValue|=0x01;
				
			#else
				W_VCPValue|=0x01;
			#endif
		}

		break;
	}
  
 case DDC2B_CMD_VCP_ColorSpace://owen 20120531
 	  {
               if (SetVcp)
               {
                    switch (W_VCPValue)
                  {
                     case 8://Text
						UserPrefGraphicsImageMode= Graphics_TEXT;	
						AdjustPresetModes();
						DisableGameMode();
						SavePortSchemeDependentSettings();
						StorePresetModes();	
						PrevUserPrefGraphicsImageMode=Graphics_TEXT;
						break;

					case 2://AdobeRGB
					    UserPrefGraphicsImageMode = Graphics_COLOR_SPACE;
						UserPrefPresetColorSpace = CSPACE_AdobeRGB;	
						AdjustPresetModes();
						DisableGameMode();
						SavePortSchemeDependentSettings();
						StorePresetModes();						
						PrevUserPrefGraphicsImageMode=Graphics_COLOR_SPACE;
						break;
#if 0
					case 3://xvMode
						UserPrefGraphicsImageMode = Graphics_COLOR_SPACE;
						//UserPrefPresetColorSpace = CSPACE_xvMode;	
						UserPrefPresetColorSpace = CSPACE_sRGB;	//Neil120612- U2413 don't have xvMode, force xvMode to sRGB, no matter graphic or video mode
						AdjustPresetModes();
						DisableGameMode();
						SavePortSchemeDependentSettings();
						StorePresetModes();
						break;
#endif
			
					case 5://CAL1
						UserPrefGraphicsImageMode = Graphics_COLOR_SPACE;
						UserPrefPresetColorSpace = CSPACE_CAL1;	
						AdjustPresetModes();
						DisableGameMode();
						SavePortSchemeDependentSettings();
						StorePresetModes();	
						PrevUserPrefGraphicsImageMode=Graphics_COLOR_SPACE;
		                break;
					 
					case 6://CAL2
						UserPrefGraphicsImageMode = Graphics_COLOR_SPACE;
						UserPrefPresetColorSpace = CSPACE_CAL2;	
						AdjustPresetModes();
						DisableGameMode();
						SavePortSchemeDependentSettings();
						StorePresetModes();
						PrevUserPrefGraphicsImageMode=Graphics_COLOR_SPACE;
		                break;	
					default:			
					break;
                  }
            }
	        else 
	        {
	            switch (UserPrefGraphicsImageMode)
                  {
                     case Graphics_TEXT:
	            		W_VCPValue=8;//W_VCPValue=1;//Text
						break;		
			
                     case Graphics_COLOR_SPACE:	 	
                           switch(UserPrefPresetColorSpace)
                           {
                             	case CSPACE_AdobeRGB:
									W_VCPValue = 2;	//AdobeRGB			
									break;

								case CSPACE_CAL1:
									W_VCPValue = 5;	//CAL1			
									break;

								case CSPACE_CAL2:
									W_VCPValue = 6;	//CAL2			
									break;		
                            }		
							break;	

					default:
					break;				
	            }			
	        }
            break;		
 	  }

		  case DDC2B_CMD_VCP_SmartVideoEnhance://owen 20120531
		  {
			  if (SetVcp)
			  {
				  switch(W_VCPValue)
				  {
					  case 0:
						  UserPrefVwdFlag = OFF;
						  UserPrefVwdFlag =VWD_DEMO_OFF;
						  VWD_Switch();
						  VWD_ApplyHighLightWindow();
						  SaveModeIndependentSettings();
						  break;
		
					  case 1:
						  UserPrefVwdFlag = ON;
						  UserPrefVwdDemo=VWD_DEMO_FULL;
						  VWD_Switch();
						  VWD_ApplyHighLightWindow();
						  SaveModeIndependentSettings();
						  break;
		
					  case 2:
						  UserPrefVwdFlag = ON;
						  UserPrefVwdDemo=VWD_DEMO_ADVANCE; 
						  VWD_Switch();
						  VWD_ApplyHighLightWindow();
						  SaveModeIndependentSettings();
						  break;
				  
				  }
			}
			  else
			  {
				  switch (UserPrefVwdDemo)
				  {
					  case VWD_DEMO_FULL:
						  W_VCPValue=1;//SVE Movie
						  break;	   
		
					  case VWD_DEMO_ADVANCE:
						  W_VCPValue=2;//SVE Advance
						  break;	   
		
					  case VWD_DEMO_OFF:
						  W_VCPValue=0;//SVE Off
						break;
		
					  default:
					  break;  
				  } 		  
			}
			 break;
		} 


		case DDC2B_CMD_VCP_Uniformity://owen 20120531
        {		
			if(SetVcp)
         	{
				switch(W_VCPValue)
         	  	{
         	  	   case 0:
					UserPrefDUMSwitch=UNIFORMITY_OFF;
					AdjustDUMSwitch();
					AdjustBackLight();
					SaveModeIndependentSettings();					
					break;	
				
         	  	   case 1:
              		UserPrefDUMSwitch=UNIFORMITY_Table1;
					AdjustDUMSwitch();
					AdjustBackLight();
					SaveModeIndependentSettings();
					//ApplyDUMTable();
			    	break;

				   case 2:
              		UserPrefDUMSwitch=UNIFORMITY_Table2;
					AdjustDUMSwitch();
					AdjustBackLight();
					SaveModeIndependentSettings();					
					//ApplyDUMTable();					
					break;
         	  	}
         	}
			else
			{
				switch(UserPrefDUMSwitch)
				{
					case UNIFORMITY_OFF:
						W_VCPValue=UNIFORMITY_OFF;
						break;
					
					case UNIFORMITY_Table1:
						W_VCPValue=UNIFORMITY_Table1;
						break;
			
					case UNIFORMITY_Table2:
						W_VCPValue=UNIFORMITY_Table2;								
						break;
	             }
		 	}
		break;	
        }
 
		case DDC2B_CMD_VCP_AllColor://owen 20120531
		{
			if(SetVcp == GET_VCP)
			{
				W_VCPValue=0;
				switch (UserPrefGraphicsImageMode)
				{
			 		case Graphics_STANDARD:
	            		W_VCPValue=0;//Standard
						break;		 

					case Graphics_MULTIMEDIA:
			            W_VCPValue=1;//Multimedia
						break;		 

					case Graphics_MOVIE:
			            W_VCPValue=2;//Movie
						break;		 

					case Graphics_GAME:
			            W_VCPValue=4;//Game
						break;	

					case Graphics_TEXT:
						W_VCPValue=0x19;//Paper
						break;	

					case Graphics_CUSTOM_COLOR:
						W_VCPValue=0x14;//
						break;	
					
					case Graphics_COLOR_TEMP:
                 		switch (UserPrefPresetColorTemp)
                  		{
                     		case CTEMP_5000K    :
                        		W_VCPValue = 0x0C;
                        		break;

                     		case CTEMP_5700K    ://5700k
                        		W_VCPValue = 0x0D;
                        		break;

                     		case CTEMP_6500K    :
                        		W_VCPValue = 0x0F;
                        		break;

                     		case CTEMP_7500K    :
                       		 	W_VCPValue = 0x10;
                        		break;

                     		case CTEMP_9300K    :
                        		W_VCPValue = 0x11;
                        		break;

                     		case CTEMP_10000K    :
                        		W_VCPValue = 0x13;
                        		break;
								
				case CTEMP_USER:
					W_VCPValue = 0x14;		//whatever
					break;

								
                 		}
						break;	

					case Graphics_COLOR_SPACE:
						switch(UserPrefPresetColorSpace)
						{
							case CSPACE_AdobeRGB:
								W_VCPValue = 7;	//AdobeRGB			
								break;

							case CSPACE_sRGB:
								W_VCPValue = 0x0B; //sRGB			
								break;

							case CSPACE_CAL1:
								W_VCPValue = 0x0a;	//CAL1			
								break;

							case CSPACE_CAL2:
								W_VCPValue = 0x15;	//CAL2			
								break;

							default:
							break;				
						}				
						break;	
		 			}
				}
 			break;
		}

		case DDC2B_CMD_VCP_PowerNap1:
		{
			BYTE OrignalBrightnessValue;
			if(SetVcp)
			{
				switch(W_VCPValue)
         	  	{
         	  		case 0:
						UserPrefBrightnessItemGreyout=0;  //yvonne 20120703 for DDM 0xE0 command
						//UserPrefBrightness = OrignalBrightnessValue;
						AdjustBackLight();
						break;
				   
				   //1. Reduced brightness to 40 nits +/- 15 nits
				   //2. The 'Brightness' slider control should be grayed out with value set at "0".
					case 1: //
						//gm_WriteRegWord(PWM3_CTRL ,0xCFFF);
                                          	Factory_AdjustBackLight(0x40);
						UserPrefBrightnessItemGreyout=1;  //yvonne 20120703 for DDM 0xE0 command
						break;
				}
			}
		}
		break;

		case DDC2B_CMD_VCP_PowerNap2:
			if(SetVcp)
			{
				switch(W_VCPValue)
         	  	{
#if POWERNAP_SLEEP
         	  		case 0:	//20121120 yvonne to turn on monitor
						gm_DisplayOnOff(On);	
						DDCPowerNapFlag=FALSE;
						PwrState = Pwr_On;
#ifdef DP_NOSYNC_LED_FADING //120808 Kordonm add
						{	BYTE command = 0;	   // 0x5A: 0x600 Normal mode, fake sleep led fading	 0: Exit fake sleep led fading
						
							if (gm_MessageSend(gmd_MESSAGE_CableStatus, &command, sizeof(command)) == FALSE)
									  gm_Print("##### Send DP Sleep Power Status Fail !!");
						}
#endif
						break;
					case 1: //Goes to sleep mode	//20121120 yvonne to turn off display only not real sleep.				
						if((GetPortConnType(UserPrefCurrentInputMain)==CT_DISPLAY_PORT))
						{
						PwrState = Pwr_Transition;
						gm_DisplayOnOff(Off);					
						DDCPowerNapFlag= TRUE;
#ifdef DP_NOSYNC_LED_FADING //120808 Kordonm add
						{	BYTE command = 0x5A;   // 0x5A: 0x600 Normal mode, fake sleep led fading	 0: Exit fake sleep led fading
							if (gm_MessageSend(gmd_MESSAGE_CableStatus, &command, sizeof(command)) == FALSE)
								gm_Print("##### Send DP Sleep Power Status Fail !!");
						}
#endif
						 IsPwrTransitionKeyPressed = FALSE;
						 gm_TimerStart(PWR_TRANSITION_100ms_TMR, 600);
						}
						else
							{
								 WORD Power_state[5];
								 Power_state[0] = (Pwr_Sleep | FORCE_SLEEP);
								 Power_state[1] = LpmWakeupPortInfo(UserPrefCurrentInputMain);
								 Power_state[2] = LpmWakeupPortInfo(UserPrefCurrentInputPip);
								 Power_state[3] = 0xFFFF;
								 Power_state[4] = 0xFFFF;				 
							
								 if(UserPrefPipMode != NO_PIP)
								   Power_state[0] |= 2<<8; // Main and PIP channel
								 else
								   Power_state[0] |= 1<<8; // Main channel only 
							 
									//gm_Print("LPM Sleep!");
								 gm_MessageSend(gmd_MESSAGE_POWER, &Power_state[0], sizeof(Power_state));
								 {	 //wait for shut down by LPM
									gmt_STimeEx timer;
									gm_TimerStartEx(&timer, 1000, 0); //timeout = 1s
									while (TRUE)
									{
										if (gm_TimerCheckEx(&timer,0,0) == TIMER_TMO)
										{
											gm_Print("Error LPM: OCM is not being shutdown by LPM!");
											gm_TimerStartEx(&timer, 1000, 0);
										}
									}
								 }
							}
						
						break;
#else
					case 1:
					{
					 WORD Power_state[5];
					 Power_state[0] = (Pwr_Sleep | FORCE_SLEEP);
					 Power_state[1] = LpmWakeupPortInfo(UserPrefCurrentInputMain);
					 Power_state[2] = LpmWakeupPortInfo(UserPrefCurrentInputPip);
					 Power_state[3] = 0xFFFF;
					 Power_state[4] = 0xFFFF;				 

					 if(UserPrefPipMode != NO_PIP)
					   Power_state[0] |= 2<<8; // Main and PIP channel
					 else
					   Power_state[0] |= 1<<8; // Main channel only 
				 
					 	//gm_Print("LPM Sleep!");
					 gm_MessageSend(gmd_MESSAGE_POWER, &Power_state[0], sizeof(Power_state));
					 {	 //wait for shut down by LPM
						gmt_STimeEx timer;
						gm_TimerStartEx(&timer, 1000, 0); //timeout = 1s
						while (TRUE)
						{
							if (gm_TimerCheckEx(&timer,0,0) == TIMER_TMO)
							{
								gm_Print("Error LPM: OCM is not being shutdown by LPM!");
								gm_TimerStartEx(&timer, 1000, 0);
							}
						}
					 }
					}
#endif

				}
			}
			break;

	#if 1//defined(CMI_73XX_BOARD)
		case DDC2B_CMD_VCP_DisplayPowerMode:
		   {
			if(SetVcp)
				{
				if(W_VCPValue == 0x01)
					{
						if(PwrState == Pwr_On)
							break;
					}
				else if(W_VCPValue == 0x04)
					{
					 WORD Power_state[5];
					 Power_state[0] = (Pwr_Sleep | FORCE_SLEEP);
					 Power_state[1] = LpmWakeupPortInfo(UserPrefCurrentInputMain);
					 Power_state[2] = LpmWakeupPortInfo(UserPrefCurrentInputPip);
					 Power_state[3] = 0xFFFF;
					 Power_state[4] = 0xFFFF;
				 

					 if(UserPrefPipMode != NO_PIP)
					   Power_state[0] |= 2<<8; // Main and PIP channel
					 else
					   Power_state[0] |= 1<<8; // Main channel only 
				 
					 gm_Print("LPM Sleep!");
					 gm_MessageSend(gmd_MESSAGE_POWER, &Power_state[0], sizeof(Power_state));
					 {	 //wait for shut down by LPM
						gmt_STimeEx timer;
						gm_TimerStartEx(&timer, 1000, 0); //timeout = 1s
						while (TRUE)
						{
							if (gm_TimerCheckEx(&timer,0,0) == TIMER_TMO)
							{
								gm_Print("Error LPM: OCM is not being shutdown by LPM!");
								gm_TimerStartEx(&timer, 1000, 0);
							}
						}
					 }
					}
				else if(W_VCPValue == 0x05)
					{
						WORD Power_state[1]; 
						Power_state[0] = Pwr_Off;
						gm_MessageSend(gmd_MESSAGE_POWER, &Power_state[0], sizeof(Power_state)); 
					}
				}
			else
				{
					if(PwrState == Pwr_On)
					 W_VCPValue = 0x01;
					else if(PwrState == Pwr_DeepSleep)				   
					 W_VCPValue = 0x04; 		
					else if(PwrState == Pwr_Off) 			   
					 W_VCPValue = 0x05;
				}
			   break;
		   }

        case DDC2B_CMD_VCP_ManufacturerSpcific:
		 	{

				W_VCPValue = ManufacturerSpcific;
				break;
        	}
	#endif	//end of CMI_73XX_BOARD
 
		case DDC2B_CMD_VCP_Dell_Feature_Support://owen 20120531
		{
			if(SetVcp == GET_VCP)
			{
				W_VCPValue=0x03;
			}

			break;
		}
	
		case DDC2B_CMD_VCP_DellFeatureStatus://owen 21020531
		{
			if(SetVcp == GET_VCP)
			{
				W_VCPValue=0x00;
			   
				if(IsPresetMode_DCR_ON())
					W_VCPValue=(W_VCPValue | UserPrefDCR); 	

                if(UserPrefEnergySmartOnOffFlag)
					W_VCPValue=(W_VCPValue | 0X02); 			

                if((IsVwdDetected()==TRUE) && (UserPrefVwdFlag==TRUE))
					W_VCPValue=(W_VCPValue | 0X04); 	                
			}

               else
               	{
         //case DDC2B_WISTRON_EDID_WP:  ///??????
               if(UserPrefFactoryFlag)
		 	{
                        if (SetVcp)  //set value
                        	{
                        	     if(W_VCPValue)
                        	     	{
                                     EDID_WP_ENABLE();
					  UserPrefEDIDWP_Temp=1;
                        	     	}
                                else
                                	{
                                    EDID_WP_DISABLE();
					  UserPrefEDIDWP_Temp=0;
                                	}
         
                        
			          }
               	     }
	            
               	}
			break;
		}
#if defined(Qisda_93XX_BOARD)
		case DDC2B_CMD_EnableAddress0x7C:
		{
			if(SetVcp == SET_VCP)
			{
				gm_Print("Set VCP",0);
				UserPrefEnableDDC2BiAddress0x7C = ENABLE;//owen 121018
				b_DDCCI_Return_Flag = TRUE;
				W_VCPValue=0x00;
			}
		}
		break;
#endif



         default:
            {
               //error condition - unsupported vcp opcode

               IsFeatureSupported = FALSE;
               break;
            }
      }
   }
   else if (VCPPage == 2)
   {
      switch (VCPFeature.vcp_opcode)
      {

  	  case DDC2B_CMD_VCP_ContrastAuto:
	  		W_VCPMax=1;
			W_VCPValue=0;
	  	 if (SetVcp)
	  	  	{
	  	         ADCCalibration(); 
				 W_VCPValue=1;// need to add return calibration result? 
				 B_VCPType=0;
	  	  	}
	  		
	  	break;

#ifdef ADJUSTER_OSDHPOSITION_DECLARED
         case DDC2B_CMD_VCP_OSDHPosition:
            {
               if (SetVcp)
               {          
                  UserPrefOsdHPosition = W_VCPValue;
                  AdjustOsdHPosition();			  
               }
               else
               {	               
                  W_VCPValue= UserPrefOsdHPosition;
               }
               break;
            }
#endif

#ifdef ADJUSTER_OSDVPOSITION_DECLARED
         case DDC2B_CMD_VCP_OSDVPosition:
            {
               if (SetVcp)
               {             
                  UserPrefOsdVPosition = W_VCPValue;
                  AdjustOsdVPosition();			  
               }
               else
               {	               
                  W_VCPValue= UserPrefOsdVPosition;
               }
               break;
            }
#endif

         default:
            {
               //error condition - unsupported vcp opcode
               IsFeatureSupported = FALSE;
               break;
            }
      }
   }
   else
   { //error condition - unsupported page

      //set current page to 0
      VCPPage = 0;
      // NOTE:
      //readExpected = 0; //no message back if page is not supported

      IsFeatureSupported = FALSE;
   }

   if (!SetVcp)
   {
      VCPFeature.max_BigEndian = EndianConv(W_VCPMax);
      VCPFeature.value_BigEndian = EndianConv(W_VCPValue);
      VCPFeature.vcp_type = B_VCPType;
   }

   if (IsFeatureSupported)
      return 0;
   else
   {
      //VCPFeature.max_BigEndian = 0;
      VCPFeature.value_BigEndian = 0;

      VCPFeature.max_BigEndian = 0xffff;

      return 1;
   }
}

#endif //ST_4K2K_93xx_BOARD

void Factory_AdjustBackLight  (BYTE W_Pulse)
{
	 #if BACKLIGHT_PWM_INVERT
          W_Pulse  =0xFF-W_Pulse;//Invert W_Pulse Value
	//W_Pulse = MAX_BACKLIGHT_PWM - W_Pulse;	//Invert W_Pulse Value
	#endif
	///////////////////////////////////////////////////////////
	//gm_Print("W_Pulse = 0x%x",W_Pulse);
	
     //20120507 Set 1Khz for PWM1,3 by KevinChen
     //gm_WriteRegWord(BACKLIGHT_PWM_CTRL, MAX_PWMPERIOD |(W_Pulse << 8));
	 #if(DEFAULT_PANEL != SHARP_1080P_LD900D3LE1)
     gm_WriteRegWord(BACKLIGHT_B_PWM_CTRL, (BYTE)MAX_PWMPERIOD |(W_Pulse << 8));
     gm_WriteRegWord(BACKLIGHT_B_PWM_CTRL_MSB, (MAX_PWMPERIOD >> 8) | (W_Pulse & 0xFF00));
	 #endif
     gm_WriteRegWord(BACKLIGHT_G_PWM_CTRL, (BYTE)MAX_PWMPERIOD |(W_Pulse << 8));
     gm_WriteRegWord(BACKLIGHT_G_PWM_CTRL_MSB, (MAX_PWMPERIOD >> 8) | (W_Pulse & 0xFF00));        
}

extern char CustomApp_FWVersion[]; //For USB ISP use
BYTE TempReadValue = 0;
BYTE TempReadValue1 = 0;//calo+
BYTE AdjustValue = 0;
BYTE R_GainValue=0;         // yza 20120720 add
BYTE G_GainValue=0;
BYTE B_GainValue=0;

BYTE SlaveAddress = 0;
BYTE ROM tEnter_Align[] 	= "ena";
BYTE ROM tExit_Align[] 	= "exa";
BYTE ROM tSet_User[] 	= "t10";
BYTE ROM tSet_Srgb[] 	= "t11";
BYTE ROM tSet_AdobeRGB[] 	= "t1a";
BYTE ROM tSet_Standard[] 	= "tps";
BYTE ROM tSet_Text[]    = "stm";
BYTE ROM tSet_5000K[] 	= "t50";
BYTE ROM tSet_5700K[] 	= "t57";
BYTE ROM tSet_6500K[] 	= "t65";
BYTE ROM tSet_7200K[] 	= "t72";
BYTE ROM tSet_7500K[] 	= "t75";
BYTE ROM tSet_9300K[] 	= "t93";
BYTE ROM tSet_10000K[] 	= "t0a";  //as TE request
BYTE ROM tSave_User[] 	= "s10";
BYTE ROM tSave_Srgb[] 	= "s11";
BYTE ROM tSave_AdobeRGB[] 	= "s1a";
BYTE ROM tSave_5000K[] 	= "s50";
BYTE ROM tSave_5700K[] 	= "s57";
BYTE ROM tSave_6500K[] 	= "s65";
BYTE ROM tSave_7200K[]  = "s72";
BYTE ROM tSave_7500K[] 	= "s75";
BYTE ROM tSave_9300K[] 	= "s93";
BYTE ROM tSave_10000K[] 	= "s0a";   //as TE request
BYTE ROM tWrite_Bri[] 	= "wbr";
BYTE ROM tWrite_Con[] 	= "wco";
BYTE ROM tWrite_RGain[] 	= "wrg";
BYTE ROM tWrite_GGain[] 	= "wgg";
BYTE ROM tWrite_BGain[] 	= "wbg";
BYTE ROM tWrite_PWM[] 	= "wpw";
BYTE ROM tSave_PWM[] 	= "wps";
BYTE ROM tReset_Color[] 	= "sdr";
BYTE ROM tWrite_Lang[]	= "wel";
BYTE ROM tAuto_Adj[] 	= "aad";
BYTE ROM tAuto_Color[] 	= "aco";
BYTE ROM tPower_On[] 	= "pon";
BYTE ROM tPower_Off[] 	= "pof";
BYTE ROM tBurnin_On[] 	= "bon";
BYTE ROM tBurnin_Off[] 	= "bof";
BYTE ROM tEnter_DDCW[] 	= "edc";
BYTE ROM tExit_DDCW[] 	= "ndc";
BYTE ROM tGo_Normal[] 	= "gnm";
BYTE ROM tWrite_SN[] 	= "wds";
BYTE ROM tSet_Mute[] 	= "mut";
BYTE ROM tSet_MaxVol[] 	= "smv";
BYTE ROM tRecall[] 		= "rec";
BYTE ROM tDisable_NvWP[]= "owp0";
BYTE ROM tEnable_NvWP[] = "owp1";
BYTE ROM tEnable_HiDCR[] = "edr";
BYTE ROM tDisable_HiDCR[] = "xdr";
BYTE ROM tAdjust_SRGBR1[]= "SR1";
BYTE ROM tAdjust_SRGBR2[]= "SR2";
BYTE ROM tAdjust_SRGBR3[]= "SR3";
BYTE ROM tAdjust_SRGBG1[]= "SG1";
BYTE ROM tAdjust_SRGBG2[]= "SG2";
BYTE ROM tAdjust_SRGBG3[]= "SG3";
BYTE ROM tAdjust_SRGBB1[]= "SB1";
BYTE ROM tAdjust_SRGBB2[]= "SB2";
BYTE ROM tAdjust_SRGBB3[]= "SB3";
BYTE ROM tAdjust_SRGBGAMMA[]= "SE3";
BYTE ROM tAdjust_AdobeRGBGAMMA[]= "AE3";
BYTE ROM tAdjust_SAVE[]= "SAV";
BYTE ROM tWrite_SBri[] 	= "sbw";
BYTE ROM tWrite_ABri[] 	= "abw";
BYTE ROM tPanel_SN[] 	= "LGD";  //calo+
BYTE ROM tSwitchtoDVI[] 	= "td1";  
BYTE ROM tSwitchtoHDMI[] 	= "th1";  
BYTE ROM tSwitchtoNormalDP[] 	= "tdp";
BYTE ROM tSwitchtoMiniDP[] 	= "tmp";
BYTE ROM tExitInterPattern[] 	= "eip";  
BYTE ROM tSwitchtoXvmode[] = "exv";
BYTE ROM tUNIFORMITYOFF[] 	= "st3";
BYTE ROM tUNIFORMITYTABLE1[] 	= "st1";
BYTE ROM tUNIFORMITYTABLE2[] 	= "st2";
BYTE ROM tSelect_Cal1[] = "cl1";
BYTE ROM tSelect_Cal2[] = "cl2";
BYTE ROM tEnergySmart_On[] = "enn";
BYTE ROM tEnergySmart_Off[] = "exn";


void AlignFunc_EnterFactoryMode(void)
{
	UserPrefFactoryFlag = TRUE;
	SaveFactorySettingsInNVR();
}
void AlignFunc_ExitFactoryMode(void)
{
	UserPrefFactoryFlag = FALSE;	
	SaveFactorySettingsInNVR();
}
void AlignFunc_EnterDDCWrite(void)
{
	gm_ClearRegBitsWord(EDID_WP_PORT,EDID_WP_PIN);  
}
void AlignFunc_ExitDDCWrite(void)
{
	gm_SetRegBitsWord(EDID_WP_PORT,EDID_WP_PIN);  
}

BYTE  ReadColorGain(BYTE Indexcolor)
{
   BYTE temp;
 if(IsPresetMode_GRAPHICS_COLOR_TEMP)
  {
   if(UserPrefPresetColorTemp == CTEMP_5000K)
   {
     if(Indexcolor == 0)
	 	temp = UserPrefColorT5000_R;
	 if(Indexcolor == 1)
	 	temp = UserPrefColorT5000_G; 
	 if(Indexcolor == 2)
	 	temp = UserPrefColorT5000_B;
   }   
   if(UserPrefPresetColorTemp == CTEMP_5700K)
   {
     if(Indexcolor == 0)
	 	temp = UserPrefColorT5700_R;
	 if(Indexcolor == 1)
	 	temp = UserPrefColorT5700_G; 
	 if(Indexcolor == 2)
	 	temp = UserPrefColorT5700_B;
   }
   if(UserPrefPresetColorTemp == CTEMP_6500K)
   {
     if(Indexcolor == 0)
	 	temp = UserPrefColorT6500_R;
	 if(Indexcolor == 1)
	 	temp = UserPrefColorT6500_G; 
	 if(Indexcolor == 2)
	 	temp = UserPrefColorT6500_B;
   }
   if(UserPrefPresetColorTemp == CTEMP_7500K)
   {
     if(Indexcolor == 0)
	 	temp = UserPrefColorT7500_R;
	 if(Indexcolor == 1)
	 	temp = UserPrefColorT7500_G; 
	 if(Indexcolor == 2)
	 	temp = UserPrefColorT7500_B;
   }   
   if(UserPrefPresetColorTemp == CTEMP_9300K)
   {
     if(Indexcolor == 0)
	 	temp = UserPrefColorT9300_R;
	 if(Indexcolor == 1)
	 	temp = UserPrefColorT9300_G; 
	 if(Indexcolor == 2)
	 	temp = UserPrefColorT9300_B;
   }
   if(UserPrefPresetColorTemp == CTEMP_10000K)
   {
     if(Indexcolor == 0)
	 	temp = UserPrefColorT10000_R;
	 if(Indexcolor == 1)
	 	temp = UserPrefColorT10000_G; 
	 if(Indexcolor == 2)
	 	temp = UserPrefColorT10000_B;
   }
   if(UserPrefPresetColorTemp == CTEMP_USER)
   {
     if(Indexcolor == 0)
	 	temp = ConvertRGBGain(UserPrefUserRedGain);
	 if(Indexcolor == 1)
	 	temp = ConvertRGBGain(UserPrefUserGreenGain); 
	 if(Indexcolor == 2)
	 	temp = ConvertRGBGain(UserPrefUserBlueGain);
   }

   
  }
 
 if(UserPrefGraphicsImageMode == Graphics_MULTIMEDIA)//7200K
 {
   if(Indexcolor == 0)
	  temp = UserPrefColorT7200_R;
   if(Indexcolor == 1)
	  temp = UserPrefColorT7200_G; 
   if(Indexcolor == 2)
	  temp = UserPrefColorT7200_B;
 }	 
 if(UserPrefGraphicsImageMode == Graphics_STANDARD)
  {
     if(Indexcolor == 0)
	 	temp = UserPrefColorT6500_R;
	 if(Indexcolor == 1)
	 	temp = UserPrefColorT6500_G; 
	 if(Indexcolor == 2)
	 	temp = UserPrefColorT6500_B;
  }
 if(IsPresetMode_GRAPHICS_COLOR_SPACE)
  {
   if(UserPrefPresetColorSpace == CSPACE_AdobeRGB)
   {
     if(Indexcolor == 0)
	 	temp = UserPrefAdobeRGBColor_R;
	 if(Indexcolor == 1)
	 	temp = UserPrefAdobeRGBColor_G; 
	 if(Indexcolor == 2)
	 	temp = UserPrefAdobeRGBColor_B;
   }
   if(UserPrefPresetColorSpace == CSPACE_sRGB)
   {
     if(Indexcolor == 0)
	 	temp = UserPrefsRGBColor_R;
	 if(Indexcolor == 1)
	 	temp = UserPrefsRGBColor_G; 
	 if(Indexcolor == 2)
	 	temp = UserPrefsRGBColor_B;
   }
  }
   return temp;
}

void AlignFunc_ColorUserMode(void)
{     
	UserPrefGraphicsImageMode = Graphics_CUSTOM_COLOR;
	UserPrefColorTemp = CTEMP_USER;
	StorePresetModes();	
	AdjustPresetModes();//calo+
	SavePortSchemeDependentSettings();
}

void AlignFunc_AdobeRGBMode(void)
{
	UserPrefGraphicsImageMode = Graphics_COLOR_SPACE;
	UserPrefPresetColorSpace = CSPACE_AdobeRGB;	
	AdjustPresetModes();
	StorePresetModes();	
	SavePortSchemeDependentSettings();
}

void AlignFunc_ColorSrgbMode(void)
{
	UserPrefGraphicsImageMode = Graphics_COLOR_SPACE;
	UserPrefPresetColorSpace = CSPACE_sRGB;	
	AdjustPresetModes();
	StorePresetModes();	
	SavePortSchemeDependentSettings();

}

void AlignFunc_Standard_Mode(void)
{
	UserPrefGraphicsImageMode = Graphics_STANDARD;
	AdjustPresetModes();
	StorePresetModes();
	SavePortSchemeDependentSettings();
}

void AlignFunc_Text_Mode(void)
{
	UserPrefGraphicsImageMode = Graphics_TEXT;
	AdjustPresetModes();
	StorePresetModes();
	SavePortSchemeDependentSettings();
}

void AlignFunc_Color5000K(void)
{
	UserPrefGraphicsImageMode = Graphics_COLOR_TEMP;
	AdjustPresetModes();
	StorePresetModes();
	//UserPrefColorTemp = CTEMP_5000K; 
	UserPrefPresetColorTemp = CTEMP_5000K; 
	AdjustColorTemp();
	SavePortSchemeDependentSettings();
}

void AlignFunc_Color5700K(void)
{
	UserPrefGraphicsImageMode = Graphics_COLOR_TEMP;
	AdjustPresetModes();
	StorePresetModes();
	UserPrefPresetColorTemp = CTEMP_5700K; 
	AdjustColorTemp();
	SavePortSchemeDependentSettings();
}

void AlignFunc_Color6500K(void)
{
	UserPrefGraphicsImageMode = Graphics_COLOR_TEMP;
	AdjustPresetModes();
	StorePresetModes();
	UserPrefPresetColorTemp = CTEMP_6500K; 
	AdjustColorTemp();
	SavePortSchemeDependentSettings();
}

void AlignFunc_Color7200K(void) //7200k multimedia
{
	UserPrefGraphicsImageMode = Graphics_MULTIMEDIA;	
	AdjustPresetModes();
	SavePortSchemeDependentSettings();
	StorePresetModes();
	DisableGameMode();	
}

void AlignFunc_Color7500K(void)
{
	UserPrefGraphicsImageMode = Graphics_COLOR_TEMP;
	AdjustPresetModes();
	StorePresetModes();
	UserPrefPresetColorTemp = CTEMP_7500K; 
	AdjustColorTemp();
	SavePortSchemeDependentSettings();
}

void AlignFunc_Color9300K(void)
   	{ 
	UserPrefGraphicsImageMode = Graphics_COLOR_TEMP;
	AdjustPresetModes();
	StorePresetModes();
	UserPrefPresetColorTemp = CTEMP_9300K; 
	AdjustColorTemp();
	SavePortSchemeDependentSettings();
}

void AlignFunc_Color10000K(void)
{
	UserPrefGraphicsImageMode = Graphics_COLOR_TEMP;
	AdjustPresetModes();
	StorePresetModes();
	UserPrefPresetColorTemp = CTEMP_10000K; 
	AdjustColorTemp();
	SavePortSchemeDependentSettings();
}

void AlignFunc_ColorUser(void)
{
	UserPrefGraphicsImageMode = Graphics_COLOR_TEMP;
	AdjustPresetModes();
	StorePresetModes();
	UserPrefPresetColorTemp = CTEMP_USER; 
	AdjustColorTemp();
	SavePortSchemeDependentSettings();
}

  
void AlignFunc_PwoerOff(void)
{
	//UserPrefPowerState = POWER_STATE_OFF;
	
	WORD Power_state[1]; 
	Power_state[0] = Pwr_Off;
	gm_MessageSend(gmd_MESSAGE_POWER, &Power_state[0], sizeof(Power_state)); 
}

void AlignFunc_BurninOn(void)
{
  UserPrefRunBurnInModeFlag = TRUE;
  UserPrefFactoryFlag = TRUE;
  SaveModeIndependentSettings();
}

void AlignFunc_BurninOff(void)
{
  UserPrefRunBurnInModeFlag = FALSE;
  UserPrefFactoryFlag = FALSE;
  SaveModeIndependentSettings();
}

void AlignFunc_EnergySmart_On(void)
{
  UserPrefEnergySmartOnOffFlag = On;
  SaveModeIndependentSettings();
}

void AlignFunc_EnergySmart_Off(void)
{
  UserPrefEnergySmartOnOffFlag = Off;
  SaveModeIndependentSettings();
}

void AlignFunc_ReadPanelType(void)
{
;}

void AlignFunc_Reset(void)
{
  FactoryReset();
}

void AlignFunc_GetFWVersion(void)  //calo update this function for factory read firmware version 090421
{
  DDCCIRunFlag = 1;
  //DrawDynamicText1();
  //TempReadValue = Factory_FWVersion[1];
  //TempReadValue1 = Factory_FWVersion[2];
}

void AlignFunc_U2410ModelConfirm(void)//calo add for the model confirm 090413
{
	//WORD U2410Model = 0x2413;			//calo update for the DELL' S model confirm
	DDCCIRunFlag = 1;
	TempReadValue = 24;//(BYTE)((U2410Model & 0xff00) >> 8) ;//calo+
	TempReadValue1 = 13;//(BYTE)(U2410Model & 0xff);//calo+
}

void AlignFunc_ReadBurnInTimer(void)  //calo update for DELL's request 090518
{
	DDCCIRunFlag = 1;
	TempReadValue = UserPrefTimeUsedHour;
	TempReadValue1 = UserPrefTimeUsedMin;

}

void AlignFunc_EnableHiDCR(void) //calo+
{
	gm_WriteRegWord(BACKLIGHT_DCR_PWM_CTRL, (WORD)(HIGHT_DCR_BACKLIGHT_PWM&PWM0_PERIOD)<<8 |(MAX_DCR_BACKLIGHT_PWM&PWM0_PERIOD));
}

void AlignFunc_DisableHiDCR(void) //calo+
{
    DWORD brightness;
	brightness = ((((DWORD)(UserPrefBrightness)) * (MAX_DCR_BACKLIGHT_PWM - MIN_DCR_BACKLIGHT_PWM) / 30 + MIN_DCR_BACKLIGHT_PWM) << 8) + MAX_DCR_BACKLIGHT_PWM;
    gm_WriteRegWord(BACKLIGHT_DCR_PWM_CTRL, brightness);
}

void AlignFunc_ReplyCheckSum(void) //calo+
{
	DWORD addr=0xA00000UL;
	DWORD dw_size=0x200000UL;
	
	BYTE far * w_hugeAddr;
	WORD chksum=0;
	DWORD i;

	DDCCIRunFlag = 1;
	
	while(dw_size)
	{
		w_hugeAddr=((addr & 0xFF0000UL)<<8) + (addr & 0xFFFFUL);
		
		for(i=0;i<0x10000;i++)
		{
			chksum+=*w_hugeAddr;
			w_hugeAddr++;
		}
		
		addr+=0x10000UL;
		dw_size-=0x10000UL;
	}
	//chksum=(~chksum)+1;

	
	TempReadValue = (BYTE)((chksum & 0xff00) >> 8);//calo+
	TempReadValue1 = (BYTE)(chksum & 0xff);//calo+

	//gm_Print("chsksum=======high bit 0x%x",TempReadValue);
	//gm_Print("chsksum=======low bit 0x%x",TempReadValue1);
}

void AlignFunc_DisableNvramWP(void)
{
   NVRAM_WP_DISABLE();
}

void AlignFunc_EnableNvramWP(void)
{
   NVRAM_WP_ENABLE();
}

void AlignFunc_SaveUserColor(void)
{
	UserPrefColorTemp = CTEMP_USER;
	SaveFactorySettingsInNVR();
    SavePortSchemeDependentSettings();
}

void AlignFunc_SaveSrgbColor(void)
{
	UserPrefPresetColorSpace = CSPACE_sRGB;
	SaveFactorySettingsInNVR();
    SavePortSchemeDependentSettings();
}

void AlignFunc_SaveAdobeRGBColor(void)
{
	UserPrefPresetColorSpace = CSPACE_AdobeRGB;	
	SaveFactorySettingsInNVR();
    SavePortSchemeDependentSettings();
}

void AlignFunc_Save5000KColor(void)
{
	UserPrefPresetColorTemp = CTEMP_5000K;	
	SaveFactorySettingsInNVR();
    SavePortSchemeDependentSettings();
}

void AlignFunc_Save5700KColor(void)
{
	UserPrefPresetColorTemp = CTEMP_5700K;	
	SaveFactorySettingsInNVR();
    SavePortSchemeDependentSettings();
}

void AlignFunc_Save6500KColor(void)
{
	UserPrefPresetColorTemp = CTEMP_6500K;	
	SaveFactorySettingsInNVR();
    SavePortSchemeDependentSettings();
}

void AlignFunc_Save7200KColor(void)
{
	UserPrefPresetColorTemp = CTEMP_7200K;	
	SaveFactorySettingsInNVR();
    SavePortSchemeDependentSettings();
}

void AlignFunc_Save7500KColor(void)
{
	UserPrefPresetColorTemp = CTEMP_7500K;	
	SaveFactorySettingsInNVR();
    SavePortSchemeDependentSettings();
}

void AlignFunc_Save9300KColor(void)
{
	UserPrefPresetColorTemp = CTEMP_9300K;	
	SaveFactorySettingsInNVR();
    SavePortSchemeDependentSettings();
}

void AlignFunc_Save10000KColor(void)
{
	UserPrefPresetColorTemp = CTEMP_10000K;	
	SaveFactorySettingsInNVR();
    SavePortSchemeDependentSettings();
}

void AlignFunc_AdjustBrightness(void)
{
	//UserPrefColorT5700_R = AdjustValue;
	if(AdjustValue > 100)
		AdjustValue = 100;
	UserPrefBrightness = AdjustValue;
	AdjustBackLight();
	SavePortSchemeDependentSettings();//calo add for save 090305
}

void AlignFunc_AdjustContrast(void)
{
	if(AdjustValue > 100)
		AdjustValue = 100;
   UserPrefContrast = AdjustValue;// + 206;
   AdjustContrast();
   SavePortSchemeDependentSettings();//calo add for save 090305
}
/////landy add it  
/*
	1440, 602, 4, 
	81, 1773, 48, 
	17, 58, 1890, 

*/
void AlignFunc_AdjustRed1Value(void)//TT
{

       WORD W_Addr;
	W_Addr = MAIN_P_MULT_COEF11;	//D5C8		// use MAIN coefficients
	if(UserPrefPresetColorSpace ==CSPACE_sRGB)
	{
	gm_WriteRegWord (W_Addr , AdjustValue+1249);  ///0--250
	UserPrefColor_R1=AdjustValue;
	}
       if(UserPrefPresetColorSpace ==CSPACE_AdobeRGB)
	{
	gm_WriteRegWord (W_Addr , AdjustValue+1785);  ///0--200
	UserPrefAdobeColor_R1=AdjustValue;
       }
        
}

void AlignFunc_AdjustRed2Value(void)//TT
{
    WORD W_Addr;
    W_Addr = MAIN_P_MULT_COEF12;            // use MAIN coefficients
    if(UserPrefPresetColorSpace ==CSPACE_sRGB)
    {
        gm_WriteRegWord (W_Addr, AdjustValue+447);//0--100
     UserPrefColor_R2=AdjustValue;
    }
       if(UserPrefPresetColorSpace ==CSPACE_AdobeRGB)
       {
    gm_WriteRegWord (W_Addr, AdjustValue+0);  ///0--200
    UserPrefAdobeColor_R2=AdjustValue;
       }    
}



void AlignFunc_AdjustRed3Value(void)//TT
{
  	WORD W_Addr;
	W_Addr = MAIN_P_MULT_COEF13;			// use MAIN coefficients
	if(UserPrefPresetColorSpace ==CSPACE_sRGB)
	{
	gm_WriteRegWord (W_Addr, AdjustValue+0);//0--10
	UserPrefColor_R3=AdjustValue;
	}
      if(UserPrefPresetColorSpace ==CSPACE_AdobeRGB)
      	{
	gm_WriteRegWord (W_Addr, AdjustValue+0);  ///0--100
	UserPrefAdobeColor_R3=AdjustValue;
      	}
}

void AlignFunc_AdjustGreen1Value(void)//TT
{
  	WORD W_Addr;
	W_Addr = MAIN_P_MULT_COEF21;			// use MAIN coefficients
	if(UserPrefPresetColorSpace ==CSPACE_sRGB)
	{
	gm_WriteRegWord (W_Addr, AdjustValue+0);//0--10
	UserPrefColor_G1=AdjustValue;
	}
      if(UserPrefPresetColorSpace ==CSPACE_AdobeRGB)
      	{
	gm_WriteRegWord (W_Addr, AdjustValue+0);  ///0--10
	UserPrefAdobeColor_G1=AdjustValue;
      	}
}

void AlignFunc_AdjustGreen2Value(void)//TT
{
  	WORD W_Addr;
	W_Addr = MAIN_P_MULT_COEF22;			// use MAIN coefficients
	if(UserPrefPresetColorSpace ==CSPACE_sRGB)
	{
	gm_WriteRegWord (W_Addr, AdjustValue+1665);//0--100
       UserPrefColor_G2=AdjustValue;
	}
	if(UserPrefPresetColorSpace ==CSPACE_AdobeRGB)
	{
	gm_WriteRegWord (W_Addr, AdjustValue+1604);  ///0--100
	UserPrefAdobeColor_G2=AdjustValue;
	}
}

void AlignFunc_AdjustGreen3Value(void)//TT
{
  	WORD W_Addr;
	W_Addr = MAIN_P_MULT_COEF23;			// use MAIN coefficients
	if(UserPrefPresetColorSpace ==CSPACE_sRGB)
	{
	gm_WriteRegWord (W_Addr, AdjustValue+0);//0--30
       UserPrefColor_G3=AdjustValue;
	}
	if(UserPrefPresetColorSpace ==CSPACE_AdobeRGB)
	{
	gm_WriteRegWord (W_Addr, AdjustValue+0);  ///0--100
	UserPrefAdobeColor_G3=AdjustValue;
	}
}

void AlignFunc_AdjustBlue1Value(void)//TT
{
  	WORD W_Addr;
	W_Addr = MAIN_P_MULT_COEF31;			// use MAIN coefficients
	
	 if(UserPrefPresetColorSpace ==CSPACE_sRGB)
	{
	gm_WriteRegWord (W_Addr, AdjustValue+0);//0--10
        UserPrefColor_B1=AdjustValue;

	}
	if(UserPrefPresetColorSpace ==CSPACE_AdobeRGB)
	{
	gm_WriteRegWord (W_Addr, AdjustValue+0);  ///0--100
	UserPrefAdobeColor_B1=AdjustValue;
	}
	
}

void AlignFunc_AdjustBlue2Value(void)//TT
{
  	WORD W_Addr;
	W_Addr = MAIN_P_MULT_COEF32;			// use MAIN coefficients
      if(UserPrefPresetColorSpace ==CSPACE_sRGB)
	{
	gm_WriteRegWord (W_Addr, AdjustValue+0);//0--20
        UserPrefColor_B2=AdjustValue;

	}
	if(UserPrefPresetColorSpace ==CSPACE_AdobeRGB)
	{
	gm_WriteRegWord (W_Addr, AdjustValue+0);  ///0--100
	UserPrefAdobeColor_B2=AdjustValue;
	}
}

void AlignFunc_AdjustBlue3Value(void)//TT
{
  	WORD W_Addr;
	W_Addr = MAIN_P_MULT_COEF33;			// use MAIN coefficients
	if(UserPrefPresetColorSpace ==CSPACE_sRGB)
	{
	gm_WriteRegWord (W_Addr, AdjustValue+1779);//0--20
       UserPrefColor_B3=AdjustValue;

	}
	if(UserPrefPresetColorSpace ==CSPACE_AdobeRGB)
	{
	gm_WriteRegWord (W_Addr, AdjustValue+1815);  ///0--100
	UserPrefAdobeColor_B3=AdjustValue;
	}

}
void AlignFunc_SAVE_COLORACCURATY(void)//TT
{  
	SaveFactorySettingsInNVR();
	AdjustColorTemp();
}

void AlignFunc_Returnvalue_GammaSelect(void)//TT
{
	switch (AdjustValue)
	{
		case 0:
			if(UserPrefPresetColorSpace ==CSPACE_sRGB)
			{
				TempReadValue=UserPrefSRGB_GAMMASELECT;
			}
			break;
		case 1:
			if(UserPrefPresetColorSpace ==CSPACE_AdobeRGB)
			{
				TempReadValue=UserPrefAdobeRGB_GAMMASELECT;
			}
			break;
	}
}

void AdjustInternalPatternRGB(BYTE TempRValue, BYTE TempGValue, BYTE TempBValue)
{
	gm_WriteRegWord(IMP_PATGEN_CONTROL, IMP_TPG_EN | IMP_PAT_SET);
	
	gm_WriteRegWord(IMP_PATGEN_RED, TempRValue);	///0--255
	gm_WriteRegWord(IMP_PATGEN_GRN, TempGValue);	///0--255	
	gm_WriteRegWord(IMP_PATGEN_BLUE, TempBValue);	///0--255
}

void AlignFunc_InternalPattern_Disable(void)
{
	gm_WriteRegWord (IMP_PATGEN_CONTROL, 0x00);

	gm_WriteRegWord(IMP_PATGEN_BLUE, 0x00);	
	gm_WriteRegWord(IMP_PATGEN_RED, 0x00);	
	gm_WriteRegWord(IMP_PATGEN_GRN, 0x00);	
}


void AlignFunc_SwitchToXvmode(void)
{
  if(UserPrefCurrentInputMain == ALL_BOARDS_HDMI)
   {
	UserPrefColorMode = ColorMode_VIDEO;
	UserPrefVideoImageMode = Video_xvMode;
	AdjustPresetModes();
   }
}
void AlignFunc_Returnvalue_COLORACCURATY(void)//TT
{
	switch (AdjustValue)
	{
		case 0:
			if (UserPrefPresetColorSpace == CSPACE_sRGB)
			{
				TempReadValue = UserPrefColor_R1;
			}
			if (UserPrefPresetColorSpace == CSPACE_AdobeRGB)
			{
				TempReadValue = UserPrefAdobeColor_R1;
			}
			break;

		case 1:
			if (UserPrefPresetColorSpace == CSPACE_sRGB)
			{
				TempReadValue = UserPrefColor_R2;
			}
			if (UserPrefPresetColorSpace == CSPACE_AdobeRGB)
			{
				TempReadValue = UserPrefAdobeColor_R2;
			}
			break;

		case 2:
			if (UserPrefPresetColorSpace == CSPACE_sRGB)
			{
				TempReadValue = UserPrefColor_R3;
			}
			if (UserPrefPresetColorSpace == CSPACE_AdobeRGB)
			{
				TempReadValue = UserPrefAdobeColor_R3;
			}
			break;

		case 3:
			if (UserPrefPresetColorSpace == CSPACE_sRGB)
			{
				TempReadValue = UserPrefColor_G1;
			}
			if (UserPrefPresetColorSpace == CSPACE_AdobeRGB)
			{
				TempReadValue = UserPrefAdobeColor_G1;
			}
			break;

		case 4:
			if (UserPrefPresetColorSpace == CSPACE_sRGB)
			{
				TempReadValue = UserPrefColor_G2;
			}
			if (UserPrefPresetColorSpace == CSPACE_AdobeRGB)
			{
				TempReadValue = UserPrefAdobeColor_G2;
			}
			break;

		case 5:
			if (UserPrefPresetColorSpace == CSPACE_sRGB)
			{
				TempReadValue = UserPrefColor_G3;
			}
			if (UserPrefPresetColorSpace == CSPACE_AdobeRGB)
			{
				TempReadValue = UserPrefAdobeColor_G3;
			}
			break;

		case 6:
			if (UserPrefPresetColorSpace == CSPACE_sRGB)
			{
				TempReadValue = UserPrefColor_B1;
			}
			if (UserPrefPresetColorSpace == CSPACE_AdobeRGB)
			{
				TempReadValue = UserPrefAdobeColor_B1;
			}
			break;

		case 7:
			if (UserPrefPresetColorSpace == CSPACE_sRGB)
			{
				TempReadValue = UserPrefColor_B2;
			}
			if (UserPrefPresetColorSpace == CSPACE_AdobeRGB)
			{
				TempReadValue = UserPrefAdobeColor_B2;
			}
			break;

		case 8:
			if (UserPrefPresetColorSpace == CSPACE_sRGB)
			{
				TempReadValue = UserPrefColor_B3;
			}
			if (UserPrefPresetColorSpace == CSPACE_AdobeRGB)
			{
				TempReadValue = UserPrefAdobeColor_B3;
			}
			break;

		default:
			if (UserPrefPresetColorSpace == CSPACE_sRGB)
			{
				TempReadValue = UserPrefColor_B3;
			}
			if (UserPrefPresetColorSpace == CSPACE_AdobeRGB)
			{
				TempReadValue = UserPrefAdobeColor_B3;
			}
			break;
	}
}

void AlignFunc_SRGBGAMMASELECT(void)
{
	UserPrefSRGB_GAMMASELECT = AdjustValue;
}

void AlignFunc_AdobeRGBGAMMASELECT(void)
{
	UserPrefAdobeRGB_GAMMASELECT = AdjustValue;
}

void AlignFunc_SwitchToDVI(void)
{
	UserPrefCurrentInputMain = ALL_BOARDS_DVI1;
	AdjustCurrentInputMain(); 	
}

void AlignFunc_SwitchToHDMI(void)
{
	UserPrefCurrentInputMain = ALL_BOARDS_HDMI;
	AdjustCurrentInputMain(); 	
}

void AlignFunc_SwitchToNormalDP(void)
{
	UserPrefCurrentInputMain = ALL_BOARDS_DP1;
	AdjustCurrentInputMain(); 	
}

void AlignFunc_SwitchToMiniDP(void)
{
	UserPrefCurrentInputMain = ALL_BOARDS_DP3;
	AdjustCurrentInputMain(); 	
}
///////////////////////////


void AlignFunc_AdjustRedGain(void)
{
 if(IsPresetMode_GRAPHICS_COLOR_TEMP)
  {
	if(UserPrefPresetColorTemp == CTEMP_5000K)
		  UserPrefColorT5000_R = AdjustValue;
	else if(UserPrefPresetColorTemp == CTEMP_5700K)
		  UserPrefColorT5700_R = AdjustValue;
	else if(UserPrefPresetColorTemp == CTEMP_6500K)
		  UserPrefColorT6500_R = AdjustValue; 
	else if(UserPrefPresetColorTemp == CTEMP_7500K)
		  UserPrefColorT7500_R = AdjustValue;
	else if(UserPrefPresetColorTemp == CTEMP_9300K)
		  UserPrefColorT9300_R = AdjustValue;
	else if(UserPrefPresetColorTemp == CTEMP_10000K)
		  UserPrefColorT10000_R = AdjustValue;
	else if(UserPrefPresetColorTemp == CTEMP_USER)
		  UserPrefUserRedGain = AdjustValue;	
  } 
 if(UserPrefGraphicsImageMode == Graphics_MULTIMEDIA)
  {
	 UserPrefColorT7200_R = AdjustValue; 
  }
 if(UserPrefGraphicsImageMode == Graphics_STANDARD)
  {
	 UserPrefColorT6500_R = AdjustValue; 
  }
 if(IsPresetMode_GRAPHICS_COLOR_SPACE)
  {
	if(UserPrefPresetColorSpace == CSPACE_sRGB)
		  UserPrefsRGBColor_R = AdjustValue;
	else if(UserPrefPresetColorSpace == CSPACE_AdobeRGB)
		  UserPrefAdobeRGBColor_R = AdjustValue;
  }
  
    CscUpdateColors(gmvb_MainChannel);
}

void AlignFunc_AdjustGreenGain(void)
{
 if(IsPresetMode_GRAPHICS_COLOR_TEMP)
  {
	if(UserPrefPresetColorTemp == CTEMP_5000K)
		 UserPrefColorT5000_G = AdjustValue;
	 else if(UserPrefPresetColorTemp == CTEMP_5700K)
		 UserPrefColorT5700_G = AdjustValue;
	 else if(UserPrefPresetColorTemp == CTEMP_6500K)
		 UserPrefColorT6500_G = AdjustValue; 
	 else if(UserPrefPresetColorTemp == CTEMP_7500K)
		 UserPrefColorT7500_G = AdjustValue;
	 else if(UserPrefPresetColorTemp == CTEMP_9300K)
		 UserPrefColorT9300_G = AdjustValue;
	 else if(UserPrefPresetColorTemp == CTEMP_10000K)
		 UserPrefColorT10000_G = AdjustValue;
	 else if(UserPrefPresetColorTemp == CTEMP_USER)
		   UserPrefUserGreenGain= AdjustValue;


  
  }
 if(UserPrefGraphicsImageMode == Graphics_MULTIMEDIA)
  {
	 UserPrefColorT7200_G = AdjustValue; 
  }
 if(UserPrefGraphicsImageMode == Graphics_STANDARD)
  {
	 UserPrefColorT6500_G = AdjustValue; 
  }
 if(IsPresetMode_GRAPHICS_COLOR_SPACE)
  {
	 if(UserPrefPresetColorSpace == CSPACE_sRGB)
		 UserPrefsRGBColor_G = AdjustValue;
	 else if(UserPrefPresetColorSpace == CSPACE_AdobeRGB)
		 UserPrefAdobeRGBColor_G = AdjustValue;
  }
	
	CscUpdateColors(gmvb_MainChannel);
}

void AlignFunc_AdjustBlueGain(void)
{
 if(IsPresetMode_GRAPHICS_COLOR_TEMP)
  {
	if(UserPrefPresetColorTemp == CTEMP_5000K)
		 UserPrefColorT5000_B = AdjustValue;
	 else if(UserPrefPresetColorTemp == CTEMP_5700K)
		 UserPrefColorT5700_B = AdjustValue;
	 else if(UserPrefPresetColorTemp == CTEMP_6500K)
		 UserPrefColorT6500_B = AdjustValue; 
	 else if(UserPrefPresetColorTemp == CTEMP_7500K)
		 UserPrefColorT7500_B = AdjustValue;
	 else if(UserPrefPresetColorTemp == CTEMP_9300K)
		 UserPrefColorT9300_B = AdjustValue;
	 else if(UserPrefPresetColorTemp == CTEMP_10000K)
		 UserPrefColorT10000_B = AdjustValue;
	 else if(UserPrefPresetColorTemp == CTEMP_USER)
		   UserPrefUserBlueGain = AdjustValue;

  
  }
 if(UserPrefGraphicsImageMode == Graphics_MULTIMEDIA)
  {
	 UserPrefColorT7200_B = AdjustValue; 
  }
 if(UserPrefGraphicsImageMode == Graphics_STANDARD)
  {
	 UserPrefColorT6500_B = AdjustValue; 
  }
 if(IsPresetMode_GRAPHICS_COLOR_SPACE)
  {
	 if(UserPrefPresetColorSpace == CSPACE_sRGB)
		 UserPrefsRGBColor_B = AdjustValue;
	 else if(UserPrefPresetColorSpace == CSPACE_AdobeRGB)
		 UserPrefAdobeRGBColor_B = AdjustValue;
  }
	
	CscUpdateColors(gmvb_MainChannel);
}

void AlignFunc_AdjustPWM3(void)
{
if(IS_UNIFORMITY_Off())
{
 if(IsPresetMode_GRAPHICS_COLOR_TEMP)
  {
  if(UserPrefPresetColorTemp == CTEMP_6500K)
    UserPrefBriBase_6500K = AdjustValue;
  }
 if(UserPrefGraphicsImageMode == Graphics_STANDARD)
  {
    UserPrefBriBase_6500K = AdjustValue;
  }
 if(UserPrefGraphicsImageMode == Graphics_TEXT)
  {
    UserPrefBriBase_Text= AdjustValue;
  }
 if(IsPresetMode_GRAPHICS_COLOR_SPACE)
  {
  if(UserPrefPresetColorSpace == CSPACE_sRGB)
	  UserPrefBriBase_sRGB = AdjustValue;
  else if(UserPrefPresetColorSpace == CSPACE_AdobeRGB)
	  UserPrefBriBase_AdobeRGB = AdjustValue;
  }
}
 else
  {
   if(UserPrefDUMSwitch == UNIFORMITY_Table1)
   	{
   	 UserPrefBriBase_Table1 = AdjustValue;
   	}
   else if(UserPrefDUMSwitch == UNIFORMITY_Table2)
   	{
   	 UserPrefBriBase_Table2 = AdjustValue;
   	}
  }
  AdjustBackLight();
}

void AlignFunc_SavePWM3(void)
{
   //SavePortSchemeDependentSettings();
   SaveFactorySettingsInNVR();
}

void AlignFunc_ReadPWM3(void)
{
if(IS_UNIFORMITY_Off())
{
 if(IsPresetMode_GRAPHICS_COLOR_TEMP)
  {
  if(UserPrefPresetColorTemp <= CTEMP_10000K)
   TempReadValue = UserPrefBriBase_6500K;
  }
 if(UserPrefGraphicsImageMode == Graphics_STANDARD)
  {
   TempReadValue = UserPrefBriBase_6500K;
  }
 if(IsPresetMode_GRAPHICS_COLOR_SPACE)
  {
  if(UserPrefPresetColorSpace == CSPACE_sRGB)
   TempReadValue = UserPrefBriBase_sRGB;
  else if(UserPrefPresetColorSpace == CSPACE_AdobeRGB)
   TempReadValue = UserPrefBriBase_AdobeRGB;
}
}
else
  {
   if(UserPrefDUMSwitch == UNIFORMITY_Table1)
   	{
   	 TempReadValue = UserPrefBriBase_Table1;
   	}
   else if(UserPrefDUMSwitch == UNIFORMITY_Table2)
   	{
   	 TempReadValue = UserPrefBriBase_Table2;
   	}
  }
}
void AlignFunc_ResetColor(void) //calo factory
{
	UserPrefBrightness = SchemeFactoryDefaultInROM[UserPrefImageScheme].Brightness;
	UserPrefContrast = SchemeFactoryDefaultInROM[UserPrefImageScheme].Contrast;
	UserPrefDCR = OFF;
	AdjustBrightness();	
	AdjustContrast();
	
	UserPrefColorMode = ColorMode_GRAPHIC;		
	UserPrefGraphicsImageMode = Graphics_STANDARD;
	AdjustColorMode();
	
	StorePresetModes();
	SavePortSchemeDependentSettings();	
}

void AlignFunc_WriteOSDLang(void)
{
}

void AlignFunc_ReadRedGain(void)
{
    TempReadValue = ReadColorGain(0);
}

void AlignFunc_ReadGreenGain(void)
{
	TempReadValue = ReadColorGain(1);
}

void AlignFunc_ReadBlueGain(void)
{
	TempReadValue = ReadColorGain(2);
}

void AlignFunc_ReadBrightness(void)
{
    TempReadValue = UserPrefBrightness;
}


void AlignFunc_ReadContrast(void)
{
    TempReadValue = UserPrefContrast;// - 206;
}

void AlignFunc_ReadADCRedGain(void)
{
	TempReadValue = (UserPrefGlobalGain_R)/2;
}

void AlignFunc_ReadADCGreenGain(void)
{
	TempReadValue = (UserPrefGlobalGain_G)/2;
}

void AlignFunc_ReadADCBlueGain(void)
{
	TempReadValue = (UserPrefGlobalGain_B)/2;
}

void AlignFunc_ReadADCRedOffset(void)
{  
   TempReadValue = UserPrefGlobalOffset2_R;
}
void AlignFunc_ReadADCGreenOffset(void)
{
   TempReadValue = UserPrefGlobalOffset2_G;
}

void AlignFunc_ReadADCBlueOffset(void)
{
   TempReadValue = UserPrefGlobalOffset2_B;
}

void AlignFunc_NULLPTR(void)
{
}

void AlignFunc_Select_UNIFORMITYTable1(void)
{
	UserPrefDUMSwitch = UNIFORMITY_Table1;
	AdjustDUMSwitch();
	AdjustBackLight();
	SaveModeIndependentSettings();
}

void AlignFunc_Select_UNIFORMITYTable2(void)
{
	UserPrefDUMSwitch = UNIFORMITY_Table2;
	AdjustDUMSwitch();
	AdjustBackLight();
	SaveModeIndependentSettings();
}

void AlignFunc_Turnoff_UNIFORMITYTable(void)
{
	UserPrefDUMSwitch = UNIFORMITY_OFF;
	AdjustDUMSwitch();
	AdjustBackLight();
	SaveModeIndependentSettings();
}

void AlignFunc_SELECTCAL1(void)
{
	UserPrefGraphicsImageMode = Graphics_COLOR_SPACE;
	UserPrefPresetColorSpace = CSPACE_CAL1; 
	AdjustPresetModes();
	EnableGameMode();
	SavePortSchemeDependentSettings();
	StorePresetModes(); 
}

void AlignFunc_SELECTCAL2(void)
{
	UserPrefGraphicsImageMode = Graphics_COLOR_SPACE;
	UserPrefPresetColorSpace = CSPACE_CAL2; 
	AdjustPresetModes();
	EnableGameMode();
	SavePortSchemeDependentSettings();
	StorePresetModes(); 
}

void AlignFunc_WriteRGBGain(void)                    //yza 20120720 Add
{
 if(IsPresetMode_GRAPHICS_COLOR_TEMP)
  {
	if(UserPrefPresetColorTemp == CTEMP_5000K)
	{
	    UserPrefColorT5000_R = R_GainValue;
	    UserPrefColorT5000_G = G_GainValue;
	    UserPrefColorT5000_B = B_GainValue;
	}
	else if(UserPrefPresetColorTemp == CTEMP_5700K)
	{
	    UserPrefColorT5700_R = R_GainValue;
	    UserPrefColorT5700_G = G_GainValue;
	    UserPrefColorT5700_B = B_GainValue;
	}
	else if(UserPrefPresetColorTemp == CTEMP_6500K)
	{
	    UserPrefColorT6500_R = R_GainValue;
	    UserPrefColorT6500_G = G_GainValue;
	    UserPrefColorT6500_B = B_GainValue;
	}
	else if(UserPrefPresetColorTemp == CTEMP_7500K)
	{
	    UserPrefColorT7500_R = R_GainValue;
	    UserPrefColorT7500_G = G_GainValue;
	    UserPrefColorT7500_B = B_GainValue;
	}
	else if(UserPrefPresetColorTemp == CTEMP_9300K)
	{
	    UserPrefColorT9300_R = R_GainValue;
	    UserPrefColorT9300_G = G_GainValue;
	    UserPrefColorT9300_B = B_GainValue;
	}
	else if(UserPrefPresetColorTemp == CTEMP_10000K)
	{
	    UserPrefColorT10000_R = R_GainValue;
	    UserPrefColorT10000_G = G_GainValue;
	    UserPrefColorT10000_B = B_GainValue;
	}
	else if(UserPrefPresetColorTemp == CTEMP_USER)
	{
	    UserPrefUserRedGain = ((WORD)R_GainValue*100)/255;
	    UserPrefUserGreenGain = ((WORD)G_GainValue*100)/255;
	    UserPrefUserBlueGain = ((WORD)B_GainValue*100)/255;
	}
	
  } 
 if(UserPrefGraphicsImageMode == Graphics_MULTIMEDIA)//7200K
   {
	    UserPrefColorT7200_R = R_GainValue;
	    UserPrefColorT7200_G = G_GainValue;
	    UserPrefColorT7200_B = B_GainValue;
   }
 if(UserPrefGraphicsImageMode == Graphics_STANDARD)
   {
	    UserPrefColorT6500_R = R_GainValue;
	    UserPrefColorT6500_G = G_GainValue;
	    UserPrefColorT6500_B = B_GainValue;
   }
 if(IsPresetMode_GRAPHICS_COLOR_SPACE)
  {
	if(UserPrefPresetColorSpace == CSPACE_sRGB)
	{
	     UserPrefsRGBColor_R = R_GainValue;
	     UserPrefsRGBColor_G = G_GainValue;
	     UserPrefsRGBColor_B = B_GainValue;
	}
	else if(UserPrefPresetColorSpace == CSPACE_AdobeRGB)
	{
	     UserPrefAdobeRGBColor_R = R_GainValue;
	     UserPrefAdobeRGBColor_G = G_GainValue;
	     UserPrefAdobeRGBColor_B = B_GainValue;
	}
  }
 
    CscUpdateColors(gmvb_MainChannel);
}

void AlignFunc_ReadRGBGain(void)        //yza 20120720 Add
{
     R_GainValue = ReadColorGain(0);
     G_GainValue = ReadColorGain(1);
     B_GainValue = ReadColorGain(2);
}


EHFactoryCommandFeature ROM EH_CommandAdj[MAX_ITEM_NUM] =
{    
	{AlignFunc_Enter_alignment,			(BYTE ROM *)AlignFunc_EnterFactoryMode,},
	{AlignFunc_Exit_alignment,			(BYTE ROM *)AlignFunc_ExitFactoryMode,},
	{AlignFunc_Enter_DDC_Write,		(BYTE ROM *)AlignFunc_EnterDDCWrite,},
	{AlignFunc_Exit_DDC_Write,			(BYTE ROM *)AlignFunc_ExitDDCWrite,},
	{AlignFunc_Set_User_Mode,			(BYTE ROM *)AlignFunc_ColorUserMode,},
	{AlignFunc_Set_SRGB_Mode,		(BYTE ROM *)AlignFunc_ColorSrgbMode,},	
	{AlignFunc_Set_AdobeRGB_Mode,	(BYTE ROM *)AlignFunc_AdobeRGBMode,},	
	{AlignFunc_Set_Standard_Mode,			(BYTE ROM *)AlignFunc_Standard_Mode,},
	{AlignFunc_Set_Text_Mode,			(BYTE ROM *)AlignFunc_Text_Mode,},
	{AlignFunc_Set_5000k,					(BYTE ROM *)AlignFunc_Color5000K,},
	{AlignFunc_Set_5700k,				(BYTE ROM *)AlignFunc_Color5700K,},
	{AlignFunc_Set_6500k,				(BYTE ROM *)AlignFunc_Color6500K,},
	{AlignFunc_Set_7200k,				(BYTE ROM *)AlignFunc_Color7200K,},
	{AlignFunc_Set_7500k,					(BYTE ROM *)AlignFunc_Color7500K,},
	{AlignFunc_Set_9300k,				(BYTE ROM *)AlignFunc_Color9300K,},
	{AlignFunc_Set_10000k,					(BYTE ROM *)AlignFunc_Color10000K,},
	{AlignFunc_Save_User_Mode,		(BYTE ROM *)AlignFunc_SaveUserColor,},
	{AlignFunc_Save_SRGB_Mode,		(BYTE ROM *)AlignFunc_SaveSrgbColor,},
	{AlignFunc_Save_AdobeRGB_Mode,	(BYTE ROM *)AlignFunc_SaveAdobeRGBColor,},
	{AlignFunc_Save_5000k,					(BYTE ROM *)AlignFunc_Save5000KColor,},
	{AlignFunc_Save_5700k,			(BYTE ROM *)AlignFunc_Save5700KColor,},
	{AlignFunc_Save_6500k,			(BYTE ROM *)AlignFunc_Save6500KColor,},
	{AlignFunc_Save_7200k,					(BYTE ROM *)AlignFunc_Save7200KColor,},
	{AlignFunc_Save_7500k,					(BYTE ROM *)AlignFunc_Save7500KColor,},
	{AlignFunc_Save_9300k,			(BYTE ROM *)AlignFunc_Save9300KColor,},
	{AlignFunc_Save_10000k,					(BYTE ROM *)AlignFunc_Save10000KColor,},
	{AlignFunc_Write_brightness,		(BYTE ROM *)AlignFunc_AdjustBrightness,},
	{AlignFunc_Write_Contrast,			(BYTE ROM *)AlignFunc_AdjustContrast,},
	{AlignFunc_Write_RGain,			(BYTE ROM *)AlignFunc_AdjustRedGain,},
	{AlignFunc_Write_GGain,			(BYTE ROM *)AlignFunc_AdjustGreenGain,},
	{AlignFunc_Write_BGain,			(BYTE ROM *)AlignFunc_AdjustBlueGain,},
	{AlignFunc_Write_PWM,				(BYTE ROM *)AlignFunc_AdjustPWM3,},
	{AlignFunc_Save_PWM,				(BYTE ROM *)AlignFunc_SavePWM3,},
	{AlignFunc_Read_PWM,				(BYTE ROM *)AlignFunc_ReadPWM3,},
	{AlignFunc_ColorReset,				(BYTE ROM *)AlignFunc_ResetColor,},
	{AlignFunc_Read_RGain,			(BYTE ROM *)AlignFunc_ReadRedGain,},
	{AlignFunc_Read_GGain,			(BYTE ROM *)AlignFunc_ReadGreenGain,},
	{AlignFunc_Read_BGain,			(BYTE ROM *)AlignFunc_ReadBlueGain,},
	{AlignFunc_Read_Brightness,		(BYTE ROM *)AlignFunc_ReadBrightness,},
	{AlignFunc_Read_Contrast,			(BYTE ROM *)AlignFunc_ReadContrast,},
	{AlignFunc_Power_OFF,					(BYTE ROM *)AlignFunc_PwoerOff,},
	{AlignFunc_Burnin_ON,				(BYTE ROM *)AlignFunc_BurninOn,},
	{AlignFunc_Burnin_OFF,				(BYTE ROM *)AlignFunc_BurninOff,},
	 {AlignFunc_Read_Panel_Type,		(BYTE ROM *)AlignFunc_ReadPanelType,},
	{AlignFunc_ReCall_Factory,			(BYTE ROM *)AlignFunc_Reset,},
	 {AlignFunc_Show_Version,			(BYTE ROM *)AlignFunc_GetFWVersion,},
	{AlignFunc_Clear_NVRAM_WP,		(BYTE ROM *)AlignFunc_DisableNvramWP,},
	{AlignFunc_Set_NVRAM_WP,			(BYTE ROM *)AlignFunc_EnableNvramWP,},
	{AlignFunc_Enter_HiDCR,			(BYTE ROM *)AlignFunc_EnableHiDCR,},
	{AlignFunc_Exit_HiDCR,				(BYTE ROM *)AlignFunc_DisableHiDCR,},
	{AlignFunc_Reply_checksum,		(BYTE ROM *)AlignFunc_ReplyCheckSum,},
	{AlignFunc_SRGB_Adobe_COLOR_RED1,		(BYTE ROM *)AlignFunc_AdjustRed1Value,},
	{AlignFunc_SRGB_Adobe_COLOR_RED2,		(BYTE ROM *)AlignFunc_AdjustRed2Value,},
	{AlignFunc_SRGB_Adobe_COLOR_RED3,		(BYTE ROM *)AlignFunc_AdjustRed3Value,},
	{AlignFunc_SRGB_Adobe_COLOR_GREEN1,		(BYTE ROM *)AlignFunc_AdjustGreen1Value,},
	{AlignFunc_SRGB_Adobe_COLOR_GREEN2,		(BYTE ROM *)AlignFunc_AdjustGreen2Value,},
	{AlignFunc_SRGB_Adobe_COLOR_GREEN3,		(BYTE ROM *)AlignFunc_AdjustGreen3Value,},
	{AlignFunc_SRGB_Adobe_COLOR_BLUE1,		(BYTE ROM *)AlignFunc_AdjustBlue1Value,},
	{AlignFunc_SRGB_Adobe_COLOR_BLUE2,		(BYTE ROM *)AlignFunc_AdjustBlue2Value,},
	{AlignFunc_SRGB_Adobe_COLOR_BLUE3,		(BYTE ROM *)AlignFunc_AdjustBlue3Value,},
	{AlignFunc_SRGB_Adobe_SAVE,				(BYTE ROM *)AlignFunc_SAVE_COLORACCURATY,},
	{AlignFunc_RETURN_COLORVALUEDELTAE,		(BYTE ROM *)AlignFunc_Returnvalue_COLORACCURATY,},
	{AlignFunc_RETURN_GammaSelect,			(BYTE ROM *)AlignFunc_Returnvalue_GammaSelect,},
	{AlignFunc_SRGB_Gamma_Select,			(BYTE ROM *)AlignFunc_SRGBGAMMASELECT,},
	{AlignFunc_AdobeRGB_Gamma_Select,		(BYTE ROM *)AlignFunc_AdobeRGBGAMMASELECT,},
	{AlignFunc_switch_to_DVI,				(BYTE ROM *)AlignFunc_SwitchToDVI,},
	{AlignFunc_switch_to_HDMI,			(BYTE ROM *)AlignFunc_SwitchToHDMI,},
	{AlignFunc_switch_to_NormalDP,			(BYTE ROM *)AlignFunc_SwitchToNormalDP,},
	{AlignFunc_switch_to_MiniDP,			(BYTE ROM *)AlignFunc_SwitchToMiniDP,},
	{AlignFunc_U2413_MODEL_confirm,			(BYTE ROM *)AlignFunc_U2410ModelConfirm,}, //calo update 090413
	{AlignFunc_ReadBurninTime,			(BYTE ROM *)AlignFunc_ReadBurnInTimer,}, //calo update for DELL's request 090518
	{AlignFunc_Select_UNIFORMITY_Table1,			(BYTE ROM *)AlignFunc_Select_UNIFORMITYTable1,}, 
	{AlignFunc_Select_UNIFORMITY_Table2,			(BYTE ROM *)AlignFunc_Select_UNIFORMITYTable2,}, 
	{AlignFunc_Turnoff_UNIFORMITY_Table,			(BYTE ROM *)AlignFunc_Turnoff_UNIFORMITYTable,},
	{AlignFunc_SELECT_CAL1,                  (BYTE ROM *)AlignFunc_SELECTCAL1,},
	{AlignFunc_SELECT_CAL2,                  (BYTE ROM *)AlignFunc_SELECTCAL2,},
	{AlignFunc_Energy_Smart_On, 			 (BYTE ROM *)AlignFunc_EnergySmart_On,},
	{AlignFunc_Energy_Smart_Off,              (BYTE ROM *)AlignFunc_EnergySmart_Off,},
	{AlignFunc_NULL,					(BYTE ROM *)AlignFunc_NULLPTR,},
};


void EH_FactoryCommand(void* gExtDebugCmdPtr)
	{
	BYTE EH_CommandAdjNumber=0;
	BYTE  i,j,k,m;
	BYTE DPEDID[8];
	BYTE *EDIDPoint;
	BYTE ReadEDIDValue[8];
    BYTE *Color_Buffer;
	BYTE nDataAddress=0,nDataLength=0;
	BYTE DataCheckSum=0;
	
    BYTE *DDC2BiCmdPtr = (BYTE *)gExtDebugCmdPtr;
	EH_CommFeature.Function = (*(((BYTE*)DDC2BiCmdPtr)+3));
	EH_CommFeature.Item = (*(((BYTE*)DDC2BiCmdPtr)+4));
	AdjustValue = (*(((BYTE*)DDC2BiCmdPtr)+5));
	gm_Print("EH_CommFeature.Function=================%d",EH_CommFeature.Function);
	gm_Print("NL_CommFeature.Item=================%d",EH_CommFeature.Item);
	gm_Print("AdjustValue=================%d",AdjustValue);
	
	EH_CommandAdjNumber = sizeof(EH_CommandAdj) / sizeof(EHFactoryCommandFeature);

     if ((EH_CommFeature.Function == 0x0F) && ((EH_CommFeature.Item == 0x42)||(EH_CommFeature.Item == 0x44)))            // Yin Add 20120721
	{      
          //   a.  (Item == 0x42) : Write Matrix Function         
          //   b.  (Item == 0x44): Write Gain 
          //Format : 6E Function Item Address Length Data( Data1 ... Datan ) CheckSum
          //eg:         6e    0F        42     0          116   
	      nDataAddress=(*(((BYTE*)DDC2BiCmdPtr) + 5 ));
	      nDataLength= (*(((BYTE*)DDC2BiCmdPtr) + 6 ));
	
		for( i=0; i<nDataLength; i++)
		{
			Color_Buffer[i]= (*(((BYTE*)DDC2BiCmdPtr) + 7+ i));
			DataCheckSum ^= Color_Buffer[i];
		}
		Color_Buffer[nDataLength+7]=DataCheckSum;
					
		NVRam_WriteBuffer(F_Color_Quick_Match_StartAddr + nDataAddress,Color_Buffer,nDataLength);	

		if ((*(((BYTE*)DDC2BiCmdPtr) + 7+ nDataLength))==DataCheckSum )
		{
		        (*(((BYTE*)DDC2BiCmdPtr) + 0)) = 0x80+0x03;
		       if ((EH_CommFeature.Function == 0x0F) && (EH_CommFeature.Item == 0x42))            // Yin Add 20120721
	             {
	                  (*(((BYTE*)DDC2BiCmdPtr) + 0)) = 0x83;
		           (*(((BYTE*)DDC2BiCmdPtr) + 1)) = 'w';	  
		           (*(((BYTE*)DDC2BiCmdPtr) + 2)) = 'm';	  
		           (*(((BYTE*)DDC2BiCmdPtr) + 3)) = 'b';	
		     }
		    else if ((EH_CommFeature.Function == 0x0F) && (EH_CommFeature.Item == 0x44))     // Yin Add 20120721
		    {
		          (*(((BYTE*)DDC2BiCmdPtr) + 0)) = 0x83;
		           (*(((BYTE*)DDC2BiCmdPtr) + 1)) = 'w';	  
		           (*(((BYTE*)DDC2BiCmdPtr) + 2)) = 'g';	  
		           (*(((BYTE*)DDC2BiCmdPtr) + 3)) = 'b';	
		    }
		}
		else
		{
		     (*(((BYTE*)DDC2BiCmdPtr) + 0)) = 0x80+0x02;
		     (*(((BYTE*)DDC2BiCmdPtr) + 1)) = 'n';	  
		     (*(((BYTE*)DDC2BiCmdPtr) + 2)) = 'g';	  
		}
	
	}

	if ((EH_CommFeature.Function == 0x0F) &&( (EH_CommFeature.Item == 0x43)||(EH_CommFeature.Item == 0x45)))            // Yin Add 20120721
	{
             //   a.  (Item == 0x43) : Read Matrix Function         
             //   b.  (Item == 0x45):  Read Gain 
	     //Format : 6E Function Item Address Length Data( Data1 ... Datan ) CheckSum
	     //eg:         6e 0F          43   0            116 
	      nDataAddress=(*(((BYTE*)DDC2BiCmdPtr) + 5 ));
	      nDataLength= (*(((BYTE*)DDC2BiCmdPtr) + 6 ));
		  
		NVRam_ReadBuffer(NVRamEnd + 0x05F+nDataAddress,Color_Buffer,nDataLength);	

		(*(((BYTE*)DDC2BiCmdPtr) + 0)) = 0x80+nDataLength;
		
		for (m = 0; m <nDataLength; m++)
		{
			(*(((BYTE*)DDC2BiCmdPtr) + 1 + m)) = Color_Buffer[m];
 		}
	}

	if ((EH_CommFeature.Function == 0x0F) && (EH_CommFeature.Item == 0x46))            // Yin Add 20120721
	{
	     R_GainValue=0;
	     G_GainValue=0;
	     B_GainValue=0;	 
	     R_GainValue=(*(((BYTE*)DDC2BiCmdPtr) + 5 ));
	     G_GainValue=(*(((BYTE*)DDC2BiCmdPtr) + 6 ));
	     B_GainValue=(*(((BYTE*)DDC2BiCmdPtr) + 7 ));
	    AlignFunc_WriteRGBGain();
	   (*(((BYTE*)DDC2BiCmdPtr) + 0)) = 0x83;
	   (*(((BYTE*)DDC2BiCmdPtr) + 1)) = 'w';	  
	   (*(((BYTE*)DDC2BiCmdPtr) + 2)) = 'a';	  
	   (*(((BYTE*)DDC2BiCmdPtr) + 3)) = 'g';	
	}

	if ((EH_CommFeature.Function == 0x0F) && (EH_CommFeature.Item == 0x47))            // Yin Add 20120721
	{
	     R_GainValue=0;
	     G_GainValue=0;
	     B_GainValue=0;
	     AlignFunc_ReadRGBGain();	 
	    (*(((BYTE*)DDC2BiCmdPtr) + 0)) = 0x83;
	    (*(((BYTE*)DDC2BiCmdPtr) + 1)) = R_GainValue;	  
	    (*(((BYTE*)DDC2BiCmdPtr) + 2)) =G_GainValue;	  
	    (*(((BYTE*)DDC2BiCmdPtr) + 3)) =B_GainValue;	
	}

	

if((EH_CommFeature.Function == 0x0D) && (EH_CommFeature.Item == 0x08))
{
 //WORD Power_state[1];
 //Power_state[0] = Pwr_Sleep;
 //gm_MessageSend(gmd_MESSAGE_POWER, &Power_state[0], sizeof(Power_state)); // Replace by gmd_MESSAGE_M2L later!

	 // To do ... store system parameters for mission power down for system sleep.
	 WORD Power_state[5];
	 Power_state[0] = (Pwr_Sleep | FORCE_SLEEP);
	 Power_state[1] = LpmWakeupPortInfo(UserPrefCurrentInputMain);
	 Power_state[2] = LpmWakeupPortInfo(UserPrefCurrentInputPip);
	 Power_state[3] = 0xFFFF;
	 Power_state[4] = 0xFFFF;
 

	 if(UserPrefPipMode != NO_PIP)
	   Power_state[0] |= 2<<8; // Main and PIP channel
	 else
	   Power_state[0] |= 1<<8; // Main channel only 
 
		(*(((BYTE*)DDC2BiCmdPtr) + 0)) = 0x83;
		(*(((BYTE*)DDC2BiCmdPtr) + 1)) = 'g';	  
		(*(((BYTE*)DDC2BiCmdPtr) + 2)) = 's';	  
		(*(((BYTE*)DDC2BiCmdPtr) + 3)) = 'p';	
		//gm_Printf("")
	 gm_Print("LPM Sleep!");
	 gm_MessageSend(gmd_MESSAGE_POWER, &Power_state[0], sizeof(Power_state));
	 {	 //wait for shut down by LPM
		gmt_STimeEx timer;
		gm_TimerStartEx(&timer, 1000, 0); //timeout = 1s
		while (TRUE)
		{
			if (gm_TimerCheckEx(&timer,0,0) == TIMER_TMO)
			{
				gm_Print("Error LPM: OCM is not being shutdown by LPM!");
				gm_TimerStartEx(&timer, 1000, 0);
			}
		}
	 }
}

if ((EH_CommFeature.Function == 0x0F) && (EH_CommFeature.Item == 0x04))
{
	SlaveAddress = AdjustValue;
	for (k = 0; k < 8; k++)
	{
		DPEDID[k] = (*(((BYTE*)DDC2BiCmdPtr) + 7 + k));//6
	//	gm_Print("DPEDID[%d]========%d",k,DPEDID[k]);
	}  

	EDIDPoint = DPEDID;
	
	//gm_ClearRegBitsWord(GPOUTPUT4,NVRAM_WP_PIN);  
	NVRam_WriteBuffer((NVRAM_DP_EDID_ADDR+SlaveAddress), EDIDPoint, 8);
    //#pragma message "Need change NVRAM_SIZE to 65536UL @ P3 stage, DP EDID shift 1 byte"
	if ((DPEDID[0] ^ DPEDID[1] ^ DPEDID[2] ^ DPEDID[3] ^ DPEDID[4] ^ DPEDID[5] ^ DPEDID[6] ^ DPEDID[7]) == (*(((BYTE*)DDC2BiCmdPtr) + 15)))//14
	{
		(*(((BYTE*)DDC2BiCmdPtr) + 0)) = 0x83;
		(*(((BYTE*)DDC2BiCmdPtr) + 1)) = 'a';	  
		(*(((BYTE*)DDC2BiCmdPtr) + 2)) = 'd';	  
		(*(((BYTE*)DDC2BiCmdPtr) + 3)) = 'p';	  
	}
	else
	{
		(*(((BYTE*)DDC2BiCmdPtr) + 0)) = 0x81;
		(*(((BYTE*)DDC2BiCmdPtr) + 1)) = 0x4E;
	}
}

if ((EH_CommFeature.Function == 0x0F) && (EH_CommFeature.Item == 0x05))
{
	SlaveAddress = AdjustValue;
	NVRam_ReadBuffer((NVRAM_DP_EDID_ADDR+SlaveAddress), ReadEDIDValue, (*(((BYTE*)DDC2BiCmdPtr) + 6)));
	(*(((BYTE*)DDC2BiCmdPtr) + 0)) = (0x80 + (*(((BYTE*)DDC2BiCmdPtr) + 6)));//0.5

	for (m = 0; m < 8; m++)
	{
		(*(((BYTE*)DDC2BiCmdPtr) + 1 + m)) = ReadEDIDValue[m];
	//	gm_Print("DPEDID[%d]========%d",m,ReadEDIDValue[m]);
	}
}

if ((EH_CommFeature.Function == 0x0F) && (EH_CommFeature.Item == 0xFE))
{

	(*(((BYTE*)DDC2BiCmdPtr) + 0)) = 0x86;
	(*(((BYTE*)DDC2BiCmdPtr) + 1)) = CustomApp_FWVersion[0];	  
	(*(((BYTE*)DDC2BiCmdPtr) + 2)) = CustomApp_FWVersion[1];  
	(*(((BYTE*)DDC2BiCmdPtr) + 3)) = CustomApp_FWVersion[2];
	(*(((BYTE*)DDC2BiCmdPtr) + 4)) = CustomApp_FWVersion[3];
	(*(((BYTE*)DDC2BiCmdPtr) + 5)) = CustomApp_FWVersion[4];
	(*(((BYTE*)DDC2BiCmdPtr) + 6)) = CustomApp_FWVersion[5];
	
}

	for (i = 0;i < EH_CommandAdjNumber; i++)
	{    
	if((EH_CommFeature.Function == 0x0D) && (EH_CommFeature.Item == 0x08))
		{
		return;		
		}
	
	if ((EH_CommFeature.Function == 0x0F) && (EH_CommFeature.Item == 0x04))
		{
		return;
		}
	
	if ((EH_CommFeature.Function == 0x0F) && (EH_CommFeature.Item == 0x05))
		{
		return;
		}
	
	if ((EH_CommFeature.Function == 0x0F) && (EH_CommFeature.Item == 0xFE))
		{
		return;
		}

	if ((EH_CommFeature.Function == 0x0F) && ((EH_CommFeature.Item == 0x42) ||(EH_CommFeature.Item == 0x43) ))   // Yin Add 20120721
	    return;
	if ((EH_CommFeature.Function == 0x0F) && ((EH_CommFeature.Item == 0x44) ||(EH_CommFeature.Item == 0x45) ))  // Yin Add 20120721
	    return;
	if ((EH_CommFeature.Function == 0x0F) && ((EH_CommFeature.Item == 0x46) ||(EH_CommFeature.Item == 0x47) ))  // Yin Add 20120721
	    return;
	
	if (((EH_CommFeature.Function == EH_CommandAdj[i].Function) && (EH_CommFeature.Item == EH_CommandAdj[i].Item))
			|| (i == EH_CommandAdjNumber - 1))
		{     
			((FPT_AdjustFunc)(EH_CommandAdj[i].AdjusterFunc))();
			gm_Delay1ms(5);
			if (EH_CommandAdj[i].Length != NULL_PTR)
			{
				*((BYTE*)DDC2BiCmdPtr) = 0x80 | EH_CommandAdj[i].Length;
               // gm_Print("*((BYTE*)DDC2BiCmdPtr)=============%x",*((BYTE*)DDC2BiCmdPtr-1));
				for (j = 1; j <= EH_CommandAdj[i].Length; j++)
				{    
					*(((BYTE*)DDC2BiCmdPtr) + j) = EH_CommandAdj[i].StringTbl[j - 1];
				//	gm_Print("*(((BYTE*)DDC2BiCmdPtr) + j)--------------%d",*(((BYTE*)DDC2BiCmdPtr) + j));
				}
			}
			else
			{
				if (DDCCIRunFlag == 0)
				{
					(*(((BYTE*)DDC2BiCmdPtr) + 0)) = 0x81;
					(*(((BYTE*)DDC2BiCmdPtr) + 1)) = TempReadValue;
				}
				else
				{
					DDCCIRunFlag = 0;
					(*(((BYTE*)DDC2BiCmdPtr) + 0)) = 0x82;
					(*(((BYTE*)DDC2BiCmdPtr) + 1)) = TempReadValue;			
					(*(((BYTE*)DDC2BiCmdPtr) + 2)) = TempReadValue1;//calo+
				}
			}
			break;
		}
		else
		{
			continue;
		}
	}	
	
}

#endif

#ifdef USE_XRite
#define C_Gamma_PostLUT_Component   BIT0
#define C_Gamma_PreLUT_Component    BIT1
#define C_Gamma_Matrix_Component    BIT2
#define C_Gamma_PostSteep_Component BIT3
#define C_Gamma_PreSteep_Component  BIT4
#define C_Gamma_Athena_Default_Component       0x1F

#define INPUT_GAMMA_R_MEM_START 0x1C000L
#define INPUT_GAMMA_R_STEEP_MEM_START 0x1C400L
#define INPUT_GAMMA_G_MEM_START 0x1C800L
#define INPUT_GAMMA_G_STEEP_MEM_START 0x1CC00L
#define INPUT_GAMMA_B_MEM_START 0x1D000L
#define INPUT_GAMMA_B_STEEP_MEM_START 0x1D400L
#define OUTPUT_GAMMA_R_MEM_START 0x1D800L
#define OUTPUT_GAMMA_R_STEEP_MEM_START 0x1DC00L
#define OUTPUT_GAMMA_G_MEM_START 0x1E000L
#define OUTPUT_GAMMA_G_STEEP_MEM_START 0x1E400L
#define OUTPUT_GAMMA_B_MEM_START 0x1E800L
#define OUTPUT_GAMMA_B_STEEP_MEM_START 0x1EC00L

#define DataSize 100
//#define SVAE_AT_THE_SAME_TIME
BYTE ComponentUsed=0;

BOOL CurrentVWDFalg=0;
BYTE CurrentVWDDemo=0; //VWD_DEMO_FULL-movie, VWD_DEMO_ADVANCE
BOOL CurrentDCR=0;
BOOL CurrentEnergySmart=0;
BYTE CurrentDUMSwitch=0;

void DDC2Bi_VCP_ParseCommand_XRite(void* gExtDebugCmdPtr)
{
	BYTE far * pTransferTable= gExtDebugCmdPtr;
	BYTE B_vcp_opcode;
	BYTE B_vcp_subopcode;
	BYTE B_data_length;
	BYTE B_blocknumber;
	BYTE * Bp_TempTable = TempBuffer;
	WORD temp_buf[128];
	WORD W_Size;
	BYTE C_Selected_Profile=0;
	WORD DW_NvramAddr;
	int i;
	//Steep registers
	BYTE Steep_Used_Ctrl, Steep_Steep; 
	WORD Steep_Start, Steep_End, Steep_Final;

	B_vcp_opcode = (((DDC2Bi_CGS_VCPCmd_t*)gExtDebugCmdPtr)->vcp_opcode);
	B_vcp_subopcode = (((DDC2Bi_CGS_VCPCmd_t*)gExtDebugCmdPtr)->vcp_subopcode);
	B_data_length = (((DDC2Bi_CGS_VCPCmd_t*)gExtDebugCmdPtr)->data_length);
	B_blocknumber = (((DDC2Bi_CGS_VCPCmd_t*)gExtDebugCmdPtr)->Blocknumber);
	
	msg_ddc_xrite("B_vcp_opcode=%x",B_vcp_opcode);
	msg_ddc_xrite("B_vcp_subopcode=%x",B_vcp_subopcode);
	msg_ddc_xrite("length=%d",B_data_length);
	msg_ddc_xrite("blocknumber=%x",B_blocknumber);
	switch (B_vcp_opcode)
	{
			case DDC2B_CMD_Xrite_SetCurrentColorSpaceData: 
			{
				switch(B_vcp_subopcode)
				{
				
					case DDC2B_CMD_Xrite_SetCurrentColorSpaceData_Reset: 
					{
						MarsInBusyState	= TRUE;
						// set DebugOn mode
						//gmvb_DebugOnly = TRUE;
						ComponentUsed=0;						
						// Bypass ODP LUT and Matrix
						gm_WriteRegWord(DISP_LUT_CONTROL, 0); // Bypass VLUT
						gm_WriteRegWord(GAMMA_LUT1_CTRL, 0); // Bypass LUT1
						gm_WriteRegWord(GAMMA_LUT2_CTRL, 0); // Bypass LUT2 and Matrix
						
						gm_SetRegBitsWord(HOST_CONTROL,ODP_SYNC_UPDATE);
						
						gm_ClearRegBitsDWord(C9VODP_DUM_CONTROL_0,BIT0);
						
						MarsInBusyState = FALSE;
						msg_ddc_xrite("XRite_Gamma_StartCalibration",0);
					}
					break;
					
					case DDC2B_CMD_Xrite_SetCurrentColorSpaceData_Upload: 
					{
						MarsInBusyState	= TRUE;
						gmvb_DebugOnly = FALSE;
						gm_WriteRegWord (DISP_LUT_CONTROL, BIT0|BIT1); // sRGB, Dither, OD need to set the window for main only ...
						
						if(UserPrefPresetColorSpace == CSPACE_CAL1)
							C_Selected_Profile=2;
						else
							C_Selected_Profile=3;	//CAL2
			
						msg_ddc_xrite("ComponentUsed=%x",ComponentUsed);
						
						if(ComponentUsed!=0)
						{
							DW_NvramAddr = C_GAMMA_COMPONENT_SAVED_START_OFFSET+ (0x2000*C_Selected_Profile); //
							NVRam_WriteByte(DW_NvramAddr,ComponentUsed);					
						}
			
            while(ComponentUsed)
			{
               DWORD DW_LUTAddr;
			   BYTE far * Wp_Dest;
               BYTE B_temp[256];
			   	   
               if(ComponentUsed & 0x1B) // other than Matrix
               {
                  WORD W_size;
                  BYTE i, j;

                  for(j = 0; j < 3; j++)
                  {
                     if(j == 0) // R
                     {
                        if(ComponentUsed & C_Gamma_PostLUT_Component)
                        {
                           if(gm_ReadRegWord(GAMMA_LUT2_CTRL)&GAMMA_LUT2_CTL)
                           {
                              DW_LUTAddr = OUTPUT_GAMMA_R_MEM_START;
							  DW_NvramAddr = C_POST_LUT_START_OFFSET+ (0x2000*C_Selected_Profile); // 
                              W_size = 1024;
                           }    
						   else
						   {
                           	  CLRBIT(ComponentUsed, C_Gamma_PostLUT_Component);
							  break;
						   }
                        }
                        else if(ComponentUsed & C_Gamma_PreLUT_Component)
                        {
                           if(gm_ReadRegWord(GAMMA_LUT1_CTRL)&GAMMA_LUT1_CTL)
                           {
                              DW_LUTAddr = INPUT_GAMMA_R_MEM_START;
							  DW_NvramAddr = C_PRE_LUT_START_OFFSET+ (0x2000*C_Selected_Profile); // 
                              W_size = 1024;
                           }
						   else
						   {
                           	  CLRBIT(ComponentUsed, C_Gamma_PreLUT_Component);
							  break;
						   }
                        }                        
                        else if(ComponentUsed & C_Gamma_PostSteep_Component)
                        {
                           if(gm_ReadRegWord(GAMMA_LUT2_CTRL)&GAMMA_LUT2_STEEP_CT)
                           {
                              DW_LUTAddr = OUTPUT_GAMMA_R_STEEP_MEM_START;
							  DW_NvramAddr = C_POST_STEEP_START_OFFSET+ (0x2000*C_Selected_Profile); // 
                              W_size = 128;
                           }
							else
							{
								CLRBIT(ComponentUsed, C_Gamma_PostSteep_Component);
								break;
							}
                        }                        
                        else if(ComponentUsed & C_Gamma_PreSteep_Component)
                        {
                           if(gm_ReadRegWord(GAMMA_LUT1_CTRL)&GAMMA_LUT1_STEEP_CT)
                           {
                              DW_LUTAddr = INPUT_GAMMA_R_STEEP_MEM_START;
							  DW_NvramAddr = C_PRE_STEEP_START_OFFSET+ (0x2000*C_Selected_Profile); // 
                              W_size = 128;
                           }
						   else
						   {
							   CLRBIT(ComponentUsed, C_Gamma_PreSteep_Component);
							   break;
						   }
                        }                        
                     }
                     else if(j == 1) // G
                     {
                        if(ComponentUsed & C_Gamma_PostLUT_Component)
                        {
                           DW_LUTAddr = OUTPUT_GAMMA_G_MEM_START;
						   DW_NvramAddr = C_POST_LUT_START_OFFSET + (0x2000*C_Selected_Profile)+ C_ONE_CHANNEL_GAMMA_SIZE; 
                           W_size = 1024;
                        }
                        else if(ComponentUsed & C_Gamma_PreLUT_Component)
                        {
                           DW_LUTAddr = INPUT_GAMMA_G_MEM_START;
						   DW_NvramAddr = C_PRE_LUT_START_OFFSET + (0x2000*C_Selected_Profile)+ C_ONE_CHANNEL_GAMMA_SIZE; 
                           W_size = 1024;
                        }                        
                        else if(ComponentUsed & C_Gamma_PostSteep_Component)
                        {
                           DW_LUTAddr = OUTPUT_GAMMA_G_STEEP_MEM_START;
						   DW_NvramAddr = C_POST_STEEP_START_OFFSET + (0x2000*C_Selected_Profile)+ C_ONE_CHANNEL_GAMMA_STEEP_SIZE; 
                           W_size = 128;
                        }                        
                        else if(ComponentUsed & C_Gamma_PreSteep_Component)
                        {
                           DW_LUTAddr = INPUT_GAMMA_G_STEEP_MEM_START;
						   DW_NvramAddr = C_PRE_STEEP_START_OFFSET + (0x2000*C_Selected_Profile)+ C_ONE_CHANNEL_GAMMA_STEEP_SIZE; 
                           W_size = 128;
                        }                        
                     }
                     else // B
                     {
                        if(ComponentUsed & C_Gamma_PostLUT_Component)
                        {
                           DW_LUTAddr = OUTPUT_GAMMA_B_MEM_START;
						   DW_NvramAddr = C_POST_LUT_START_OFFSET + (0x2000*C_Selected_Profile)+ C_ONE_CHANNEL_GAMMA_SIZE*2; 
                           W_size = 1024;
                           //CLRBIT(DW_ComponentToSave, Gamma_PostLUT_Component);
                        }
                        else if(ComponentUsed & C_Gamma_PreLUT_Component)
                        {
                           DW_LUTAddr = INPUT_GAMMA_B_MEM_START;
						   DW_NvramAddr = C_PRE_LUT_START_OFFSET + (0x2000*C_Selected_Profile)+ C_ONE_CHANNEL_GAMMA_SIZE*2; 
                           W_size = 1024;
                           //CLRBIT(DW_ComponentToSave, Gamma_PreLUT_Component);
                        }                        
                        else if(ComponentUsed & C_Gamma_PostSteep_Component)
                        {
                           DW_LUTAddr = OUTPUT_GAMMA_B_STEEP_MEM_START;
						   DW_NvramAddr = C_POST_STEEP_START_OFFSET + (0x2000*C_Selected_Profile)+ C_ONE_CHANNEL_GAMMA_STEEP_SIZE*2; 
                           W_size = 128;
                           CLRBIT(ComponentUsed, C_Gamma_PostSteep_Component);
                        }                        
                        else if(ComponentUsed & C_Gamma_PreSteep_Component)
                        {
                           DW_LUTAddr = INPUT_GAMMA_B_STEEP_MEM_START;
						   DW_NvramAddr = C_PRE_STEEP_START_OFFSET + (0x2000*C_Selected_Profile)+ C_ONE_CHANNEL_GAMMA_STEEP_SIZE*2; 
                           W_size = 128;
                           CLRBIT(ComponentUsed, C_Gamma_PreSteep_Component);
                        }                        
                     }
					 
                        if(W_size == 128)
                        {				   
                        	for(i=0; i<128;)
                        	{
						   		B_temp[i*2] = *LINEAR_TO_FPTR_B(DW_LUTAddr);   
								B_temp[i*2+1] = *LINEAR_TO_FPTR_B(DW_LUTAddr+1);
								i+=1;
								DW_LUTAddr+=4;
                        	}
						    NVRam_WriteBuffer(DW_NvramAddr, B_temp, 256);
                        }
                        else
                        {   
							Wp_Dest = LINEAR_TO_FPTR_B(DW_LUTAddr);    
							NVRam_WriteBuffer_DUM_Gamma(DW_NvramAddr, Wp_Dest, 1024);
                     }
                  }

				  if(ComponentUsed & C_Gamma_PostLUT_Component)
                  {
                     //gm_Printf(" Save Post LUT Config Registers", 0);
                     DW_NvramAddr = C_POST_LUT_CONFIG_REG_START_OFFSET+ (0x2000*C_Selected_Profile); //
                     NVRam_WriteWORD(DW_NvramAddr, gm_ReadRegWord(GAMMA_LUT2_CTRL));   

                     DW_NvramAddr += 2;
                     DW_LUTAddr = GAMMA_LUT2_STEEP_STRT_R;            

                     for(i = 0; i < 12; i++)
                     {
                        NVRam_WriteWORD(DW_NvramAddr, gm_ReadRegWord(DW_LUTAddr));
                        DW_LUTAddr += 2;
                        DW_NvramAddr += 2;            
                     }
                     CLRBIT(ComponentUsed, C_Gamma_PostLUT_Component);
					 
                  }
                  else if(ComponentUsed & C_Gamma_PreLUT_Component)
                  {
                     //gm_Printf(" Save Pre LUT Config Registers", 0);
                     DW_NvramAddr = C_PRE_LUT_CONFIG_REG_START_OFFSET + (0x2000*C_Selected_Profile); // 2 sector for 1 color space
                     NVRam_WriteWORD(DW_NvramAddr, gm_ReadRegWord(GAMMA_LUT1_CTRL));   
                   
                     DW_NvramAddr += 2;
                     DW_LUTAddr = GAMMA_LUT1_STEEP_STRT_R;            

                     for(i = 0; i < 12; i++)
                     {
                        NVRam_WriteWORD(DW_NvramAddr, gm_ReadRegWord(DW_LUTAddr));
                        DW_LUTAddr += 2;
                        DW_NvramAddr += 2;            
                     }
                     CLRBIT(ComponentUsed, C_Gamma_PreLUT_Component);
                  }                  
               }
               else if(ComponentUsed & C_Gamma_Matrix_Component)
               {
                  BYTE i;
                  DW_LUTAddr = GAMMA_LUT_MATRIX_MULT_COEF11; // GAMMA_LUT1_CTRL;
                  DW_NvramAddr = C_GAMMA_REGISTER_START_OFFSET+ (0x2000*C_Selected_Profile);

                  //for(i = 0; i < 41; i++) // 41 registers
                  for(i = 0; i < 15; i++) // 15 registers
                  {                     
                     NVRam_WriteWORD(DW_NvramAddr, gm_ReadRegWord(DW_LUTAddr));
                     DW_LUTAddr += 2;
                     DW_NvramAddr += 2;
                     
                  }
                  CLRBIT(ComponentUsed, C_Gamma_Matrix_Component);
               }
			   SavePortSchemeDependentSettings();	//save brightness/contrast
            }
						MarsInBusyState = FALSE;
					}
					break;
				}	
			}
			break;
	
	
			case DDC2B_CMD_Xrite_CloseCurrentColorSpace: 
				gmvb_DebugOnly = FALSE;
				break;
				
			case DDC2B_CMD_Xrite_SetInputGamma: 
			{
				switch(B_vcp_subopcode)
				{
					case DDC2B_CMD_Xrite_SetInputGammaRed: 
					{
						MarsInBusyState	= TRUE;
						msg_ddc_xrite("~~DDC2B_CMD_Xrite_SetInputGammaRed ~~",0);
						W_Size = B_data_length;
						pTransferTable += 8;
						//take data from input buffer
						memcpy(&Bp_TempTable[0], (BYTE*)pTransferTable, W_Size);
						if(W_Size < DataSize)
						{
							gm_LoadImpVlutTable(RED_VLUT_COMPONENT, DataSize *B_blocknumber, W_Size/2, (WORD far*)Bp_TempTable, 0);
						}
						else
							gm_LoadImpVlutTable(RED_VLUT_COMPONENT, W_Size*B_blocknumber, W_Size/2, (WORD far*)Bp_TempTable, 0);
						
						if(gm_ReadRegWord(DISP_LUT_CONTROL) != 0x3)
						   gm_WriteRegWord(DISP_LUT_CONTROL, 0x3);
						
						gm_SetRegBitsWord(GAMMA_LUT1_CTRL, GAMMA_LUT1_CTL);
						
						gm_SyncUpdate(gmvb_MainChannel, OUTPUT_CLOCK);

						ComponentUsed = ComponentUsed | C_Gamma_PreLUT_Component;						

#ifdef SVAE_AT_THE_SAME_TIME
						//save to NVRAM
						if(UserPrefPresetColorSpace == CSPACE_CAL1)
							C_Selected_Profile=2;
						else
							C_Selected_Profile=3;	//CAL2
						
						DW_NvramAddr = C_PRE_LUT_START_OFFSET+ (0x2000*C_Selected_Profile)  ; //
						DW_NvramAddr += W_Size*B_blocknumber;

						msg_ddc_xrite("DW_NvramAddr=%x",DW_NvramAddr);
						
						for( i = 0; i < W_Size; i++)
	                    {                     
							if(W_Size == 128)
	                        {
	                           NVRam_WriteByte(DW_NvramAddr, Bp_TempTable[i]);
	                           DW_NvramAddr += 2;
	                        }
	                        else
	                        {                           
	                           NVRam_WriteByte(DW_NvramAddr, Bp_TempTable[i]);
	                           DW_NvramAddr += 1;
	                        }                                                                       
	                     }
#endif
						MarsInBusyState = FALSE;
					}
					break;
					
					case DDC2B_CMD_Xrite_SetInputGammaGreen: 
					{
						MarsInBusyState	= TRUE;
						msg_ddc_xrite("~DDC2B_CMD_Xrite_SetInputGammaGreen ~~",0);
						W_Size = B_data_length;
						pTransferTable += 8;
						//take data from input buffer
						memcpy(&Bp_TempTable[0], (BYTE*)pTransferTable, W_Size);
						if(W_Size < DataSize)
						{
							gm_LoadImpVlutTable(GREEN_VLUT_COMPONENT, DataSize *B_blocknumber, W_Size/2, (WORD far*)Bp_TempTable, 0);
						}
						else
							gm_LoadImpVlutTable(GREEN_VLUT_COMPONENT, W_Size*B_blocknumber, W_Size/2, (WORD far*)Bp_TempTable, 0);

						ComponentUsed = ComponentUsed | C_Gamma_PreLUT_Component;						

#ifdef SVAE_AT_THE_SAME_TIME
						if(UserPrefPresetColorSpace == CSPACE_CAL1)
							C_Selected_Profile=2;
						else
							C_Selected_Profile=3;	//CAL2
						
						DW_NvramAddr = (0x2000*C_Selected_Profile) + C_PRE_LUT_START_OFFSET + C_ONE_CHANNEL_GAMMA_SIZE; 
						DW_NvramAddr += W_Size*B_blocknumber;

						msg_ddc_xrite("DW_NvramAddr=%x",DW_NvramAddr);
						
						for( i = 0; i < W_Size; i++)
	                    {                     
							if(W_Size == 128)
	                        {
	                           NVRam_WriteByte(DW_NvramAddr, Bp_TempTable[i]);
	                           DW_NvramAddr += 2;
	                        }
	                        else
	                        {                           
	                           NVRam_WriteByte(DW_NvramAddr, Bp_TempTable[i]);
	                           DW_NvramAddr += 1;
	                        }                                                                       
	                     }					
#endif
						MarsInBusyState = FALSE;
					}
					break;
					
					case DDC2B_CMD_Xrite_SetInputGammaBlue: 
					{
						MarsInBusyState	= TRUE;
						msg_ddc_xrite("~DDC2B_CMD_Xrite_SetInputGammaBlue ~~",0);
						W_Size = B_data_length;
						pTransferTable += 8;
						//take data from input buffer
						memcpy(&Bp_TempTable[0], (BYTE*)pTransferTable, W_Size);
						if(W_Size < DataSize)
						{
							gm_LoadImpVlutTable(BLUE_VLUT_COMPONENT, DataSize *B_blocknumber, W_Size/2, (WORD far*)Bp_TempTable, 0);
						}
						else
							gm_LoadImpVlutTable(BLUE_VLUT_COMPONENT, W_Size*B_blocknumber, W_Size/2, (WORD far*)Bp_TempTable, 0);

						ComponentUsed = ComponentUsed | C_Gamma_PreLUT_Component;						

#ifdef SVAE_AT_THE_SAME_TIME
						if(UserPrefPresetColorSpace == CSPACE_CAL1)
							C_Selected_Profile=2;
						else
							C_Selected_Profile=3;	//CAL2
						
						DW_NvramAddr = C_PRE_LUT_START_OFFSET+ (0x2000*C_Selected_Profile)  + C_ONE_CHANNEL_GAMMA_SIZE*2; 
						DW_NvramAddr += W_Size*B_blocknumber;
						
						msg_ddc_xrite("DW_NvramAddr=%x",DW_NvramAddr);
						
						for( i = 0; i < W_Size; i++)
						{					  
							if(W_Size == 128)
							{
							   NVRam_WriteByte(DW_NvramAddr, Bp_TempTable[i]);
							   DW_NvramAddr += 2;
							}
							else
							{							
							   NVRam_WriteByte(DW_NvramAddr, Bp_TempTable[i]);
							   DW_NvramAddr += 1;
							}																		
						 }					
#endif
						MarsInBusyState = FALSE;
					}
					break;
					
					case DDC2B_CMD_Xrite_SetInputGammaRed_Steep: 
					{
						MarsInBusyState	= TRUE;
						msg_ddc_xrite("~~DDC2B_CMD_Xrite_SetInputGammaRed_Steep ~~",0);
						
						if(B_blocknumber==0)
						{
						//Steep Registers
						W_Size = 8;
						pTransferTable += 8;
						//take data from input buffer
						memcpy(&Bp_TempTable[0], (BYTE*)pTransferTable, 8);
											
						Steep_Used_Ctrl = Bp_TempTable[0] & 0x03;
						if(Steep_Used_Ctrl)
							Steep_Used_Ctrl=0x03;
						Steep_Start = Bp_TempTable[1] | Bp_TempTable[2]<<8;
						if(Steep_Start)
							Steep_Start = (Steep_Start << 4) - 1;
						Steep_End = Bp_TempTable[3] | Bp_TempTable[4]<<8; 
						if(Steep_End)
							Steep_End = (Steep_End << 4) - 1;
						Steep_Steep = Bp_TempTable[5] & 0x03;
						Steep_Final = Bp_TempTable[6] | Bp_TempTable[7]<<8; 
						
						msg_ddc_xrite("Steep_Used_Ctrl=%x",Steep_Used_Ctrl);
						msg_ddc_xrite("Steep_Start=%x",Steep_Start);
						msg_ddc_xrite("Steep_End=%x",Steep_End);
						msg_ddc_xrite("Steep_Steep=%x",Steep_Steep);
						msg_ddc_xrite("Steep_Final=%x",Steep_Final);						
											
						gm_ClearAndSetBitsWord (GAMMA_LUT1_CTRL, GAMMA_LUT1_STEEP_CT, Steep_Used_Ctrl<<8);
						gm_WriteRegWord(GAMMA_LUT1_STEEP_STRT_R, Steep_Start);
						gm_WriteRegWord(GAMMA_LUT1_STEEP_END_R, Steep_End);
						gm_WriteRegWord(GAMMA_LUT1_STEEP_FINAL_R, Steep_Final);
						gm_ClearAndSetBitsWord(GAMMA_LUT1_CTRL, GAMMA_LUT1_STEEP_ST,Steep_Steep<<12 );
						gm_WriteRegWord(GAMMA_LUT1_FINAL_ENTRY_R, 0x4000);
						gm_WriteRegWord(GAMMA_LUT1_FINAL_ENTRY_G, 0x4000);
						gm_WriteRegWord(GAMMA_LUT1_FINAL_ENTRY_B, 0x4000);
						}
#ifdef SVAE_AT_THE_SAME_TIME						
						Bp_TempTable[8]=0x00;
						Bp_TempTable[9]=0x40;
						Bp_TempTable[10]=0x00;
						Bp_TempTable[11]=0x40;
						Bp_TempTable[12]=0x00;
						Bp_TempTable[13]=0x40;
						
						W_Size +=6;
											
						//save to NVRAM
						if(UserPrefPresetColorSpace == CSPACE_CAL1)
							C_Selected_Profile=2;
						else
							C_Selected_Profile=3;	//CAL2
						
						DW_NvramAddr = C_PRE_LUT_CONFIG_REG_START_OFFSET+ (0x2000*C_Selected_Profile); // 
						msg_ddc_xrite("DW_NvramAddr=%x",DW_NvramAddr);
						
						for( i = 0; i < W_Size; i++)
						{					  
							{							
								NVRam_WriteByte(DW_NvramAddr, Bp_TempTable[i]);
								DW_NvramAddr += 1;
							}																		
						}
#endif					
						if(gm_ReadRegWord(GAMMA_LUT1_CTRL)&GAMMA_LUT1_STEEP_CT)
						{
						//Steep Data
						
						if(B_blocknumber==0)
						W_Size = B_data_length - 8;
						else
							W_Size = B_data_length;
						
						pTransferTable += 8;
						//take data from input buffer
						memcpy(&Bp_TempTable[0], (BYTE*)pTransferTable, W_Size);
											
						for(i=0;i<(W_Size/2);i++)
						{
							temp_buf[i]=Bp_TempTable[i*2]|Bp_TempTable[i*2+1]<<8;
							temp_buf[i] = temp_buf[i] >> 2;
							Bp_TempTable[i*2]= (BYTE)temp_buf[i];
							Bp_TempTable[i*2+1]=temp_buf[i]>>8;
						}
							
						if(B_blocknumber==0)
							gm_LoadImpVlutTable(RED_STEEP_VLUT_COMPONENT, 0, W_Size/2, (WORD far*)Bp_TempTable, 0);
						else if(B_blocknumber==1)
							gm_LoadImpVlutTable(RED_STEEP_VLUT_COMPONENT, 92*2, W_Size/2, (WORD far*)Bp_TempTable, 0);
						else
							gm_LoadImpVlutTable(RED_STEEP_VLUT_COMPONENT, 192*2, W_Size/2, (WORD far*)Bp_TempTable, 0);

						ComponentUsed = ComponentUsed | C_Gamma_PreSteep_Component;						
						
#ifdef SVAE_AT_THE_SAME_TIME
						if(UserPrefPresetColorSpace == CSPACE_CAL1)
							C_Selected_Profile=2;
						else
							C_Selected_Profile=3;	//CAL2
						
						DW_NvramAddr = C_PRE_STEEP_START_OFFSET+ (0x2000*C_Selected_Profile); // 
						DW_NvramAddr += W_Size*B_blocknumber;
						
						msg_ddc_xrite("DW_NvramAddr=%x",DW_NvramAddr);
						
						for( i = 0; i < W_Size; i++)
						{					  
							{							
								NVRam_WriteByte(DW_NvramAddr, Bp_TempTable[i]);
								DW_NvramAddr += 1;
							}																		
						}
#endif
						}
						MarsInBusyState = FALSE;
					}
					break;
					
					case DDC2B_CMD_Xrite_SetInputGammaGreen_Steep: 
					{
						MarsInBusyState	= TRUE;
						msg_ddc_xrite("~DDC2B_CMD_Xrite_SetInputGammaGreen_Steep ~~",0);
						
						if(B_blocknumber==0)
						{
						//Steep Registers
						W_Size = 8;
						pTransferTable += 8;
						//take data from input buffer
						memcpy(&Bp_TempTable[0], (BYTE*)pTransferTable, 8);
											
						Steep_Used_Ctrl = Bp_TempTable[0] & 0x03;
						if(Steep_Used_Ctrl)
							Steep_Used_Ctrl=0x03;
						Steep_Start = Bp_TempTable[1] | Bp_TempTable[2]<<8;
						if(Steep_Start)
							Steep_Start = (Steep_Start << 4) - 1;
						Steep_End = Bp_TempTable[3] | Bp_TempTable[4]<<8; 
						if(Steep_End)
							Steep_End = (Steep_End << 4) - 1;
						Steep_Steep = Bp_TempTable[5] & 0x03;
						Steep_Final = Bp_TempTable[6] | Bp_TempTable[7]<<8; 
						
						msg_ddc_xrite("Steep_Used_Ctrl=%x",Steep_Used_Ctrl);
						msg_ddc_xrite("Steep_Start=%x",Steep_Start);
						msg_ddc_xrite("Steep_End=%x",Steep_End);
						msg_ddc_xrite("Steep_Steep=%x",Steep_Steep);
						msg_ddc_xrite("Step_Final=%x",Steep_Final);						
											
						gm_ClearAndSetBitsWord (GAMMA_LUT1_CTRL, GAMMA_LUT1_STEEP_CT, Steep_Used_Ctrl<<8);
						gm_WriteRegWord(GAMMA_LUT1_STEEP_STRT_G, Steep_Start);
						gm_WriteRegWord(GAMMA_LUT1_STEEP_END_G, Steep_End);
						gm_WriteRegWord(GAMMA_LUT1_STEEP_FINAL_G, Steep_Final);
						gm_ClearAndSetBitsWord(GAMMA_LUT1_CTRL, GAMMA_LUT1_STEEP_ST,Steep_Steep<<12 );
						gm_WriteRegWord(GAMMA_LUT1_FINAL_ENTRY_R, 0x4000);
						gm_WriteRegWord(GAMMA_LUT1_FINAL_ENTRY_G, 0x4000);
						gm_WriteRegWord(GAMMA_LUT1_FINAL_ENTRY_B, 0x4000);
						}	

#ifdef SVAE_AT_THE_SAME_TIME						
						Bp_TempTable[8]=0x00;
						Bp_TempTable[9]=0x40;
						Bp_TempTable[10]=0x00;
						Bp_TempTable[11]=0x40;
						Bp_TempTable[12]=0x00;
						Bp_TempTable[13]=0x40;
						
						W_Size +=6;
											
						//save to NVRAM
						if(UserPrefPresetColorSpace == CSPACE_CAL1)
							C_Selected_Profile=2;
						else
							C_Selected_Profile=3;	//CAL2
						
						DW_NvramAddr = C_PRE_LUT_CONFIG_REG_START_OFFSET+ (0x2000*C_Selected_Profile); // 
						msg_ddc_xrite("DW_NvramAddr=%x",DW_NvramAddr);
						
						for( i = 0; i < W_Size; i++)
						{					  
							{							
								NVRam_WriteByte(DW_NvramAddr, Bp_TempTable[i]);
								DW_NvramAddr += 1;
							}																		
						}
#endif
						if(gm_ReadRegWord(GAMMA_LUT1_CTRL)&GAMMA_LUT1_STEEP_CT)
						{
						
						//Steep Data
						
						if(B_blocknumber==0)
						W_Size = B_data_length - 8;
						else
							W_Size = B_data_length;
						
						pTransferTable += 8;
						//take data from input buffer
						memcpy(&Bp_TempTable[0], (BYTE*)pTransferTable, W_Size);
											
						for(i=0;i<(W_Size/2);i++)
						{
							temp_buf[i]=Bp_TempTable[i*2]|Bp_TempTable[i*2+1]<<8;
							temp_buf[i] = temp_buf[i] >> 2;
							Bp_TempTable[i*2]= (BYTE)temp_buf[i];
							Bp_TempTable[i*2+1]=temp_buf[i]>>8;
						}
											
						if(B_blocknumber==0)
							gm_LoadImpVlutTable(GREEN_STEEP_VLUT_COMPONENT, 0, W_Size/2, (WORD far*)Bp_TempTable, 0);
						else if(B_blocknumber==1)
							gm_LoadImpVlutTable(GREEN_STEEP_VLUT_COMPONENT, 92*2, W_Size/2, (WORD far*)Bp_TempTable, 0);
						else
							gm_LoadImpVlutTable(GREEN_STEEP_VLUT_COMPONENT, 192*2, W_Size/2, (WORD far*)Bp_TempTable, 0);

						ComponentUsed = ComponentUsed | C_Gamma_PreSteep_Component;						
						
#ifdef SVAE_AT_THE_SAME_TIME
						if(UserPrefPresetColorSpace == CSPACE_CAL1)
							C_Selected_Profile=2;
						else
							C_Selected_Profile=3;	//CAL2
						
						DW_NvramAddr = C_PRE_STEEP_START_OFFSET+ (0x2000*C_Selected_Profile)+C_ONE_CHANNEL_GAMMA_STEEP_SIZE; // 
						DW_NvramAddr += W_Size*B_blocknumber;
						
						msg_ddc_xrite("DW_NvramAddr=%x",DW_NvramAddr);
						
						for( i = 0; i < W_Size; i++)
						{					  
							{							
								NVRam_WriteByte(DW_NvramAddr, Bp_TempTable[i]);
								DW_NvramAddr += 1;
							}																		
						}
#endif
						}
						MarsInBusyState = FALSE;
					}
					break;
					
					case DDC2B_CMD_Xrite_SetInputGammaBlue_Steep: 
					{
						msg_ddc_xrite("~DDC2B_CMD_Xrite_SetInputGammaBlue_Steep ~~",0);
						MarsInBusyState	= TRUE;
						
						if(B_blocknumber==0)
						{
						//Steep Registers
						W_Size = 8;
						pTransferTable += 8;
						//take data from input buffer
						memcpy(&Bp_TempTable[0], (BYTE*)pTransferTable, 8);
											
						Steep_Used_Ctrl = Bp_TempTable[0] & 0x03;
						if(Steep_Used_Ctrl)
							Steep_Used_Ctrl=0x03;
						Steep_Start = Bp_TempTable[1] | Bp_TempTable[2]<<8;
						if(Steep_Start)
							Steep_Start = (Steep_Start << 4) - 1;
						Steep_End = Bp_TempTable[3] | Bp_TempTable[4]<<8; 
						if(Steep_End)
							Steep_End = (Steep_End << 4) - 1;
						Steep_Steep = Bp_TempTable[5] & 0x03;
						Steep_Final = Bp_TempTable[6] | Bp_TempTable[7]<<8; 
						
						msg_ddc_xrite("Steep_Used_Ctrl=%x",Steep_Used_Ctrl);
						msg_ddc_xrite("Steep_Start=%x",Steep_Start);
						msg_ddc_xrite("Steep_End=%x",Steep_End);
						msg_ddc_xrite("Steep_Steep=%x",Steep_Steep);
						msg_ddc_xrite("Steep_Final=%x",Steep_Final);						
											
						gm_ClearAndSetBitsWord (GAMMA_LUT1_CTRL, GAMMA_LUT1_STEEP_CT, Steep_Used_Ctrl<<8);
						gm_WriteRegWord(GAMMA_LUT1_STEEP_STRT_B, Steep_Start);
						gm_WriteRegWord(GAMMA_LUT1_STEEP_END_B, Steep_End);
						gm_WriteRegWord(GAMMA_LUT1_STEEP_FINAL_B, Steep_Final);
						gm_ClearAndSetBitsWord(GAMMA_LUT1_CTRL, GAMMA_LUT1_STEEP_ST,Steep_Steep<<12 );
						gm_WriteRegWord(GAMMA_LUT1_FINAL_ENTRY_R, 0x4000);
						gm_WriteRegWord(GAMMA_LUT1_FINAL_ENTRY_G, 0x4000);
						gm_WriteRegWord(GAMMA_LUT1_FINAL_ENTRY_B, 0x4000);
						}

#ifdef SVAE_AT_THE_SAME_TIME						
						Bp_TempTable[8]=0x00;
						Bp_TempTable[9]=0x40;
						Bp_TempTable[10]=0x00;
						Bp_TempTable[11]=0x40;
						Bp_TempTable[12]=0x00;
						Bp_TempTable[13]=0x40;
						
						W_Size +=6;
											
						//save to NVRAM
						if(UserPrefPresetColorSpace == CSPACE_CAL1)
							C_Selected_Profile=2;
						else
							C_Selected_Profile=3;	//CAL2
						
						DW_NvramAddr = C_PRE_LUT_CONFIG_REG_START_OFFSET+ (0x2000*C_Selected_Profile); // 
						msg_ddc_xrite("DW_NvramAddr=%x",DW_NvramAddr);
						
						for( i = 0; i < W_Size; i++)
						{					  
							{							
								NVRam_WriteByte(DW_NvramAddr, Bp_TempTable[i]);
								DW_NvramAddr += 1;
							}																		
						}
#endif
						if(gm_ReadRegWord(GAMMA_LUT1_CTRL)&GAMMA_LUT1_STEEP_CT)
						{
						
						//Steep Data
						if(B_blocknumber==0)
						W_Size = B_data_length - 8;
						else
							W_Size = B_data_length;
						
						pTransferTable += 8;
						//take data from input buffer
						memcpy(&Bp_TempTable[0], (BYTE*)pTransferTable, W_Size);
											
						for(i=0;i<(W_Size/2);i++)
						{
							temp_buf[i]=Bp_TempTable[i*2]|Bp_TempTable[i*2+1]<<8;
							temp_buf[i] = temp_buf[i] >> 2;
							Bp_TempTable[i*2]= (BYTE)temp_buf[i];
							Bp_TempTable[i*2+1]=temp_buf[i]>>8;
						}
											
						if(B_blocknumber==0)
							gm_LoadImpVlutTable(BLUE_STEEP_VLUT_COMPONENT, 0, W_Size/2, (WORD far*)Bp_TempTable, 0);
						else if(B_blocknumber==1)
							gm_LoadImpVlutTable(BLUE_STEEP_VLUT_COMPONENT, 92*2, W_Size/2, (WORD far*)Bp_TempTable, 0);
						else
							gm_LoadImpVlutTable(BLUE_STEEP_VLUT_COMPONENT, 192*2, W_Size/2, (WORD far*)Bp_TempTable, 0);

						ComponentUsed = ComponentUsed | C_Gamma_PreSteep_Component;						
						
#ifdef SVAE_AT_THE_SAME_TIME
						if(UserPrefPresetColorSpace == CSPACE_CAL1)
							C_Selected_Profile=2;
						else
							C_Selected_Profile=3;	//CAL2
						
						DW_NvramAddr = C_PRE_STEEP_START_OFFSET+ (0x2000*C_Selected_Profile)+C_ONE_CHANNEL_GAMMA_STEEP_SIZE*2; // 
						DW_NvramAddr += W_Size*B_blocknumber;
						
						msg_ddc_xrite("DW_NvramAddr=%x",DW_NvramAddr);
						
						for( i = 0; i < W_Size; i++)
						{					  
							{							
								NVRam_WriteByte(DW_NvramAddr, Bp_TempTable[i]);
								DW_NvramAddr += 1;
							}																		
						}
#endif
						}
						MarsInBusyState = FALSE;
					}
					break;				
				}
			}
			break;

			case DDC2B_CMD_Xrite_SetOutputGamma: 
			{
				switch(B_vcp_subopcode)
				{
					case DDC2B_CMD_Xrite_SetOutputGammaRed: 
					{
						MarsInBusyState	= TRUE;
						msg_ddc_xrite("~DDC2B_CMD_Xrite_SetOutputGammaRed ~~",0);
						W_Size = B_data_length;
						pTransferTable += 8;
						//take data from input buffer
						memcpy(&Bp_TempTable[0], (BYTE*)pTransferTable, W_Size);

						if(W_Size < DataSize)
						{
							//W_Size= DataSize *B_blocknumber;
							gm_LoadVlutTable(RED_VLUT_COMPONENT, DataSize *B_blocknumber, W_Size/2, (WORD far*)Bp_TempTable, 0);
						}
						else
							gm_LoadVlutTable(RED_VLUT_COMPONENT, W_Size*B_blocknumber, W_Size/2, (WORD far*)Bp_TempTable, 0);

						
						if(gm_ReadRegWord(DISP_LUT_CONTROL) != 0x3)
						   gm_WriteRegWord(DISP_LUT_CONTROL, 0x3);
						
						gm_SetRegBitsWord(GAMMA_LUT2_CTRL, GAMMA_LUT2_CTL);
						
						gm_SyncUpdate(gmvb_MainChannel, OUTPUT_CLOCK);
						
						ComponentUsed = ComponentUsed | C_Gamma_PostLUT_Component;						

#ifdef SVAE_AT_THE_SAME_TIME
						if(UserPrefPresetColorSpace == CSPACE_CAL1)
							C_Selected_Profile=2;
						else
							C_Selected_Profile=3;	//CAL2
						
						DW_NvramAddr = C_POST_LUT_START_OFFSET+ (0x2000*C_Selected_Profile)  ; // 
						DW_NvramAddr += W_Size*B_blocknumber;
						
						msg_ddc_xrite("DW_NvramAddr=%x",DW_NvramAddr);
						
						for( i = 0; i < W_Size; i++)
						{					  
							if(W_Size == 128)
							{
							   NVRam_WriteByte(DW_NvramAddr, Bp_TempTable[i]);
							   DW_NvramAddr += 2;
							}
							else
							{							
							   NVRam_WriteByte(DW_NvramAddr, Bp_TempTable[i]);
							   DW_NvramAddr += 1;
							}																		
						 }					
#endif
						MarsInBusyState = FALSE;
					}
					break;
					
					case DDC2B_CMD_Xrite_SetOutputGammaGreen: 
					{
						MarsInBusyState	= TRUE;
						msg_ddc_xrite("~DDC2B_CMD_Xrite_SetOutputGammaGreen ~~",0);
						W_Size = B_data_length;
						pTransferTable += 8;
						//take data from input buffer
						memcpy(&Bp_TempTable[0], (BYTE*)pTransferTable, W_Size);

						if(W_Size < DataSize)
						{
							//W_Size= DataSize *B_blocknumber;
							gm_LoadVlutTable(GREEN_VLUT_COMPONENT, DataSize *B_blocknumber, W_Size/2, (WORD far*)Bp_TempTable, 0);
						}
						else
							gm_LoadVlutTable(GREEN_VLUT_COMPONENT, W_Size*B_blocknumber, W_Size/2, (WORD far*)Bp_TempTable, 0);

						
						ComponentUsed = ComponentUsed | C_Gamma_PostLUT_Component;						

#ifdef SVAE_AT_THE_SAME_TIME
						if(UserPrefPresetColorSpace == CSPACE_CAL1)
							C_Selected_Profile=2;
						else
							C_Selected_Profile=3;	//CAL2
						
						DW_NvramAddr = C_POST_LUT_START_OFFSET+ (0x2000*C_Selected_Profile) + C_ONE_CHANNEL_GAMMA_SIZE; // 
						DW_NvramAddr += W_Size*B_blocknumber;

						msg_ddc_xrite("DW_NvramAddr=%x",DW_NvramAddr);
						
						for( i = 0; i < W_Size; i++)
	                    {                     
							if(W_Size == 128)
	                        {
	                           NVRam_WriteByte(DW_NvramAddr, Bp_TempTable[i]);
	                           DW_NvramAddr += 2;
	                        }
	                        else
	                        {                           
	                           NVRam_WriteByte(DW_NvramAddr, Bp_TempTable[i]);
	                           DW_NvramAddr += 1;
	                        }                                                                       
	                     }					
#endif
						MarsInBusyState = FALSE;
					}
					break;
					
					case DDC2B_CMD_Xrite_SetOutputGammaBlue: 
					{
						MarsInBusyState	= TRUE;
						msg_ddc_xrite("~DDC2B_CMD_Xrite_SetOutputGammaBlue ~~",0);
						W_Size = B_data_length;
						pTransferTable += 8;
						//take data from input buffer
						memcpy(&Bp_TempTable[0], (BYTE*)pTransferTable, W_Size);
						
						if(W_Size < DataSize)
						{
							//W_Size= DataSize *B_blocknumber;
							gm_LoadVlutTable(BLUE_VLUT_COMPONENT, DataSize *B_blocknumber, W_Size/2, (WORD far*)Bp_TempTable, 0);
						}
						else
							gm_LoadVlutTable(BLUE_VLUT_COMPONENT, W_Size*B_blocknumber, W_Size/2, (WORD far*)Bp_TempTable, 0);


						ComponentUsed = ComponentUsed | C_Gamma_PostLUT_Component;
						
#ifdef SVAE_AT_THE_SAME_TIME
						if(UserPrefPresetColorSpace == CSPACE_CAL1)
							C_Selected_Profile=2;
						else
							C_Selected_Profile=3;	//CAL2
						
						DW_NvramAddr = C_POST_LUT_START_OFFSET+ (0x2000*C_Selected_Profile) + C_ONE_CHANNEL_GAMMA_SIZE*2; // 
						DW_NvramAddr += W_Size*B_blocknumber;

						msg_ddc_xrite("DW_NvramAddr=%x",DW_NvramAddr);
						
						for( i = 0; i < W_Size; i++)
	                    {                     
							if(W_Size == 128)
	                        {
	                           NVRam_WriteByte(DW_NvramAddr, Bp_TempTable[i]);
	                           DW_NvramAddr += 2;
	                        }
	                        else
	                        {                           
	                           NVRam_WriteByte(DW_NvramAddr, Bp_TempTable[i]);
	                           DW_NvramAddr += 1;
	                        }                                                                       
	                     }					
#endif
						MarsInBusyState = FALSE;
					}
					break;

					
					case DDC2B_CMD_Xrite_SetOutputGammaRed_Steep: 
					{
						msg_ddc_xrite("~DDC2B_CMD_Xrite_SetOutputGammaRed_Steep ~~",0);
						
						MarsInBusyState	= TRUE;
						if(B_blocknumber==0)
						{
						//Steep Registers
						W_Size = 8;
						pTransferTable += 8;
						//take data from input buffer
						memcpy(&Bp_TempTable[0], (BYTE*)pTransferTable, 8);
											
							Steep_Used_Ctrl = Bp_TempTable[0]& 0x03;
						if(Steep_Used_Ctrl)
							Steep_Used_Ctrl=0x03;
						Steep_Start = Bp_TempTable[1] | Bp_TempTable[2]<<8;
						if(Steep_Start)
							Steep_Start = (Steep_Start << 4) - 1;
						Steep_End = Bp_TempTable[3] | Bp_TempTable[4]<<8; 
						if(Steep_End)
							Steep_End = (Steep_End << 4) - 1;
							Steep_Steep = Bp_TempTable[5] & 0x03;
						Steep_Final = Bp_TempTable[6] | Bp_TempTable[7]<<8; 
						
						msg_ddc_xrite("Steep_Used_Ctrl=%x",Steep_Used_Ctrl);
						msg_ddc_xrite("Steep_Start=%x",Steep_Start);
						msg_ddc_xrite("Steep_End=%x",Steep_End);
						msg_ddc_xrite("Steep_Steep=%x",Steep_Steep);
						msg_ddc_xrite("Steep_Final=%x",Steep_Final);						
											
						gm_ClearAndSetBitsWord (GAMMA_LUT2_CTRL, GAMMA_LUT2_STEEP_CT, Steep_Used_Ctrl<<4);
						gm_WriteRegWord(GAMMA_LUT2_STEEP_STRT_R, Steep_Start);
						gm_WriteRegWord(GAMMA_LUT2_STEEP_END_R, Steep_End);
						gm_WriteRegWord(GAMMA_LUT2_STEEP_FINAL_R, Steep_Final);
						gm_ClearAndSetBitsWord(GAMMA_LUT2_CTRL, GAMMA_LUT2_STEEP_ST,Steep_Steep<<8);
						gm_WriteRegWord(GAMMA_LUT2_FINAL_ENTRY_R, 0x4000);
						gm_WriteRegWord(GAMMA_LUT2_FINAL_ENTRY_G, 0x4000);
						gm_WriteRegWord(GAMMA_LUT2_FINAL_ENTRY_B, 0x4000);
						
						}
#ifdef SVAE_AT_THE_SAME_TIME
						Bp_TempTable[8]=0x00;
						Bp_TempTable[9]=0x40;
						Bp_TempTable[10]=0x00;
						Bp_TempTable[11]=0x40;
						Bp_TempTable[12]=0x00;
						Bp_TempTable[13]=0x40;
						
						W_Size +=6;
											
						//save to NVRAM
						if(UserPrefPresetColorSpace == CSPACE_CAL1)
							C_Selected_Profile=2;
						else
							C_Selected_Profile=3;	//CAL2
						
						DW_NvramAddr = C_POST_LUT_CONFIG_REG_START_OFFSET+ (0x2000*C_Selected_Profile); // 
						msg_ddc_xrite("DW_NvramAddr=%x",DW_NvramAddr);
						
						for( i = 0; i < W_Size; i++)
						{					  
							{							
								NVRam_WriteByte(DW_NvramAddr, Bp_TempTable[i]);
								DW_NvramAddr += 1;
							}																		
						}
#endif
						
						if(gm_ReadRegWord(GAMMA_LUT2_CTRL)&GAMMA_LUT2_STEEP_CT)
						{
						//Steep Data
						if(B_blocknumber==0)
						W_Size = B_data_length - 8;
						else
							W_Size = B_data_length ;

						
						pTransferTable += 8;

						//take data from input buffer
						memcpy(&Bp_TempTable[0], (BYTE*)pTransferTable, W_Size);
											
						for(i=0;i<(W_Size/2);i++)
						{
							temp_buf[i]=Bp_TempTable[i*2]|Bp_TempTable[i*2+1]<<8;
							temp_buf[i] = temp_buf[i] >> 2;
							Bp_TempTable[i*2]= (BYTE)temp_buf[i];
							Bp_TempTable[i*2+1]=temp_buf[i]>>8;
						}
											

						if(B_blocknumber==0)
							gm_LoadVlutTable(RED_STEEP_VLUT_COMPONENT, 0, W_Size/2, (WORD far*)Bp_TempTable, 0);
						else if(B_blocknumber==1)
							gm_LoadVlutTable(RED_STEEP_VLUT_COMPONENT, 92*2, W_Size/2, (WORD far*)Bp_TempTable, 0);
						else
							gm_LoadVlutTable(RED_STEEP_VLUT_COMPONENT, 192*2, W_Size/2, (WORD far*)Bp_TempTable, 0);

						
 						ComponentUsed = ComponentUsed | C_Gamma_PostSteep_Component;						

#ifdef SVAE_AT_THE_SAME_TIME						
						if(UserPrefPresetColorSpace == CSPACE_CAL1)
							C_Selected_Profile=2;
						else
							C_Selected_Profile=3;	//CAL2
						
						DW_NvramAddr = C_POST_STEEP_START_OFFSET+ (0x2000*C_Selected_Profile); // 
						DW_NvramAddr += W_Size*B_blocknumber;
						
						msg_ddc_xrite("DW_NvramAddr=%x",DW_NvramAddr);
						
						for( i = 0; i < W_Size; i++)
						{					  
							{							
								NVRam_WriteByte(DW_NvramAddr, Bp_TempTable[i]);
								DW_NvramAddr += 1;
							}																		
						}
#endif
						}
						MarsInBusyState = FALSE;
					}
					break;
					
					case DDC2B_CMD_Xrite_SetOutputGammaGreen_Steep: 
					{
						msg_ddc_xrite("~DDC2B_CMD_Xrite_SetOutputGammaGreen_Steep ~~",0);
						
						MarsInBusyState	= TRUE;
						if(B_blocknumber==0)
						{
						//Steep Registers
						W_Size = 8;
						pTransferTable += 8;
						//take data from input buffer
						memcpy(&Bp_TempTable[0], (BYTE*)pTransferTable, 8);
											
						Steep_Used_Ctrl = Bp_TempTable[0]&0x03;
						if(Steep_Used_Ctrl)
							Steep_Used_Ctrl=0x03;
						Steep_Start = Bp_TempTable[1] | Bp_TempTable[2]<<8;
						if(Steep_Start)
							Steep_Start = (Steep_Start << 4) - 1;
						Steep_End = Bp_TempTable[3] | Bp_TempTable[4]<<8; 
						if(Steep_End)
							Steep_End = (Steep_End << 4) - 1;
						Steep_Steep = Bp_TempTable[5]&0x03;
						Steep_Final = Bp_TempTable[6] | Bp_TempTable[7]<<8; 
						
						msg_ddc_xrite("Steep_Used_Ctrl=%x",Steep_Used_Ctrl);
						msg_ddc_xrite("Steep_Start=%x",Steep_Start);
						msg_ddc_xrite("Steep_End=%x",Steep_End);
						msg_ddc_xrite("Steep_Steep=%x",Steep_Steep);
						msg_ddc_xrite("Steep_Final=%x",Steep_Final);						
											
						gm_ClearAndSetBitsWord (GAMMA_LUT2_CTRL, GAMMA_LUT2_STEEP_CT, Steep_Used_Ctrl<<4);
						gm_WriteRegWord(GAMMA_LUT2_STEEP_STRT_G, Steep_Start);
						gm_WriteRegWord(GAMMA_LUT2_STEEP_END_G, Steep_End);
						gm_WriteRegWord(GAMMA_LUT2_STEEP_FINAL_G, Steep_Final);
						gm_ClearAndSetBitsWord(GAMMA_LUT2_CTRL, GAMMA_LUT2_STEEP_ST,Steep_Steep<<8 );
						gm_WriteRegWord(GAMMA_LUT2_FINAL_ENTRY_R, 0x4000);
						gm_WriteRegWord(GAMMA_LUT2_FINAL_ENTRY_G, 0x4000);
						gm_WriteRegWord(GAMMA_LUT2_FINAL_ENTRY_B, 0x4000);
						
						}
#ifdef SVAE_AT_THE_SAME_TIME
						Bp_TempTable[8]=0x00;
						Bp_TempTable[9]=0x40;
						Bp_TempTable[10]=0x00;
						Bp_TempTable[11]=0x40;
						Bp_TempTable[12]=0x00;
						Bp_TempTable[13]=0x40;
						
						W_Size +=6;
											
						//save to NVRAM
						if(UserPrefPresetColorSpace == CSPACE_CAL1)
							C_Selected_Profile=2;
						else
							C_Selected_Profile=3;	//CAL2
						
						DW_NvramAddr = C_POST_LUT_CONFIG_REG_START_OFFSET+ (0x2000*C_Selected_Profile); //
						msg_ddc_xrite("DW_NvramAddr=%x",DW_NvramAddr);
						
						for( i = 0; i < W_Size; i++)
						{					  
							{							
								NVRam_WriteByte(DW_NvramAddr, Bp_TempTable[i]);
								DW_NvramAddr += 1;
							}																		
						}
#endif
						
						if(gm_ReadRegWord(GAMMA_LUT2_CTRL)&GAMMA_LUT2_STEEP_CT)
						{
						//Steep Data
						
						if(B_blocknumber==0)
						W_Size = B_data_length - 8;
						else
							W_Size = B_data_length;
						
						pTransferTable += 8;
						//take data from input buffer
						memcpy(&Bp_TempTable[0], (BYTE*)pTransferTable, W_Size);
											
						for(i=0;i<(W_Size/2);i++)
						{
							temp_buf[i]=Bp_TempTable[i*2]|Bp_TempTable[i*2+1]<<8;
							temp_buf[i] = temp_buf[i] >> 2;
							Bp_TempTable[i*2]= (BYTE)temp_buf[i];
							Bp_TempTable[i*2+1]=temp_buf[i]>>8;
						}
											
						if(B_blocknumber==0)
							gm_LoadVlutTable(GREEN_STEEP_VLUT_COMPONENT, 0, W_Size/2, (WORD far*)Bp_TempTable, 0);
						else if(B_blocknumber==1)
							gm_LoadVlutTable(GREEN_STEEP_VLUT_COMPONENT, 92*2, W_Size/2, (WORD far*)Bp_TempTable, 0);
						else
							gm_LoadVlutTable(GREEN_STEEP_VLUT_COMPONENT, 192*2, W_Size/2, (WORD far*)Bp_TempTable, 0);
						
						ComponentUsed = ComponentUsed | C_Gamma_PostSteep_Component;						
						
#ifdef SVAE_AT_THE_SAME_TIME
						if(UserPrefPresetColorSpace == CSPACE_CAL1)
							C_Selected_Profile=2;
						else
							C_Selected_Profile=3;	//CAL2
						
						DW_NvramAddr = C_POST_STEEP_START_OFFSET+ (0x2000*C_Selected_Profile)+C_ONE_CHANNEL_GAMMA_STEEP_SIZE; // 
						DW_NvramAddr += W_Size*B_blocknumber;
						
						msg_ddc_xrite("DW_NvramAddr=%x",DW_NvramAddr);
						
						for( i = 0; i < W_Size; i++)
						{					  
							{							
								NVRam_WriteByte(DW_NvramAddr, Bp_TempTable[i]);
								DW_NvramAddr += 1;
							}																		
						}
#endif
						}
						MarsInBusyState = FALSE;
					}
					break;
					
					case DDC2B_CMD_Xrite_SetOutputGammaBlue_Steep: 
					{
						msg_ddc_xrite("~DDC2B_CMD_Xrite_SetOutputGammaBlue_Steep ~~",0);
						
						MarsInBusyState	= TRUE;
						
						if(B_blocknumber==0)
						{
						//Steep Registers
						W_Size = 8;
						pTransferTable += 8;
						//take data from input buffer
						memcpy(&Bp_TempTable[0], (BYTE*)pTransferTable, 8);
											
						Steep_Used_Ctrl = Bp_TempTable[0]&0x03;
						if(Steep_Used_Ctrl)
							Steep_Used_Ctrl=0x03;
						Steep_Start = Bp_TempTable[1] | Bp_TempTable[2]<<8;
						if(Steep_Start)
							Steep_Start = (Steep_Start << 4) - 1;
						Steep_End = Bp_TempTable[3] | Bp_TempTable[4]<<8; 
						if(Steep_End)
							Steep_End = (Steep_End << 4) - 1;
						Steep_Steep = Bp_TempTable[5]&0x03;
						Steep_Final = Bp_TempTable[6] | Bp_TempTable[7]<<8; 
						
						msg_ddc_xrite("Steep_Used_Ctrl=%x",Steep_Used_Ctrl);
						msg_ddc_xrite("Steep_Start=%x",Steep_Start);
						msg_ddc_xrite("Steep_End=%x",Steep_End);
						msg_ddc_xrite("Steep_Steep=%x",Steep_Steep);
						msg_ddc_xrite("Steep_Final=%x",Steep_Final);						
											
						gm_ClearAndSetBitsWord (GAMMA_LUT2_CTRL, GAMMA_LUT2_STEEP_CT, Steep_Used_Ctrl<<4);
						gm_WriteRegWord(GAMMA_LUT2_STEEP_STRT_B, Steep_Start);
						gm_WriteRegWord(GAMMA_LUT2_STEEP_END_B, Steep_End);
						gm_WriteRegWord(GAMMA_LUT2_STEEP_FINAL_B, Steep_Final);
						gm_ClearAndSetBitsWord(GAMMA_LUT2_CTRL, GAMMA_LUT2_STEEP_ST,Steep_Steep<<8 );
						gm_WriteRegWord(GAMMA_LUT2_FINAL_ENTRY_R, 0x4000);
						gm_WriteRegWord(GAMMA_LUT2_FINAL_ENTRY_G, 0x4000);
						gm_WriteRegWord(GAMMA_LUT2_FINAL_ENTRY_B, 0x4000);
						
						}
#ifdef SVAE_AT_THE_SAME_TIME
						Bp_TempTable[8]=0x00;
						Bp_TempTable[9]=0x40;
						Bp_TempTable[10]=0x00;
						Bp_TempTable[11]=0x40;
						Bp_TempTable[12]=0x00;
						Bp_TempTable[13]=0x40;
						
						W_Size +=6;
											
						//save to NVRAM
						if(UserPrefPresetColorSpace == CSPACE_CAL1)
							C_Selected_Profile=2;
						else
							C_Selected_Profile=3;	//CAL2
						
						DW_NvramAddr = C_POST_LUT_CONFIG_REG_START_OFFSET+ (0x2000*C_Selected_Profile); // 
						msg_ddc_xrite("DW_NvramAddr=%x",DW_NvramAddr);
						
						for( i = 0; i < W_Size; i++)
						{					  
							{							
								NVRam_WriteByte(DW_NvramAddr, Bp_TempTable[i]);
								DW_NvramAddr += 1;
							}																		
						}
#endif						
						if(gm_ReadRegWord(GAMMA_LUT2_CTRL)&GAMMA_LUT2_STEEP_CT)
						{
						//Steep Data
						
						if(B_blocknumber==0)
						W_Size = B_data_length - 8;
						else
							W_Size = B_data_length;
						
						pTransferTable += 8;
						//take data from input buffer
						memcpy(&Bp_TempTable[0], (BYTE*)pTransferTable, W_Size);
											
						for(i=0;i<(W_Size/2);i++)
						{
							temp_buf[i]=Bp_TempTable[i*2]|Bp_TempTable[i*2+1]<<8;
							temp_buf[i] = temp_buf[i] >> 2;
							Bp_TempTable[i*2]= (BYTE)temp_buf[i];
							Bp_TempTable[i*2+1]=temp_buf[i]>>8;
						}
											
						if(B_blocknumber==0)
							gm_LoadVlutTable(BLUE_STEEP_VLUT_COMPONENT, 0, W_Size/2, (WORD far*)Bp_TempTable, 0);
						else if(B_blocknumber==1)
							gm_LoadVlutTable(BLUE_STEEP_VLUT_COMPONENT, 92*2, W_Size/2, (WORD far*)Bp_TempTable, 0);
						else
							gm_LoadVlutTable(BLUE_STEEP_VLUT_COMPONENT, 192*2, W_Size/2, (WORD far*)Bp_TempTable, 0);
						
						ComponentUsed = ComponentUsed | C_Gamma_PostSteep_Component;						

#ifdef SVAE_AT_THE_SAME_TIME
						if(UserPrefPresetColorSpace == CSPACE_CAL1)
							C_Selected_Profile=2;
						else
							C_Selected_Profile=3;	//CAL2
						
						DW_NvramAddr = C_POST_STEEP_START_OFFSET+ (0x2000*C_Selected_Profile)+C_ONE_CHANNEL_GAMMA_STEEP_SIZE*2; // 
						DW_NvramAddr += W_Size*B_blocknumber;
						
						msg_ddc_xrite("DW_NvramAddr=%x",DW_NvramAddr);
						
						for( i = 0; i < W_Size; i++)
						{					  
							{							
								NVRam_WriteByte(DW_NvramAddr, Bp_TempTable[i]);
								DW_NvramAddr += 1;
							}																		
						}
#endif
						}
						MarsInBusyState = FALSE;	
					}
					break;				
				}
			}
			break;

			case DDC2B_CMD_Xrite_SetMatrix: 
			{
				WORD DW_TempCoeff;
				MarsInBusyState = TRUE;
				msg_ddc_xrite("~~ DDC2B_CMD_Xrite_SetMatrix ~~",0);
				W_Size = 30;
				pTransferTable += 6;
				//take data from input buffer
				memcpy(&Bp_TempTable[0], (BYTE*)pTransferTable, 30);
				//for(i=0;i<30;i++)
				//	msg_ddc_xrite("Bp_TempTable[%d]=%x",i,Bp_TempTable[i]);

				DW_TempCoeff = Bp_TempTable[8]| Bp_TempTable[9]<<8 ;
				gm_WriteRegWord(GAMMA_LUT_MATRIX_MULT_COEF11, (WORD)DW_TempCoeff);//
				DW_TempCoeff = Bp_TempTable[10]| Bp_TempTable[11]<<8 ;
				gm_WriteRegWord(GAMMA_LUT_MATRIX_MULT_COEF12, (WORD)DW_TempCoeff);//
				DW_TempCoeff = Bp_TempTable[6]| Bp_TempTable[7]<<8 ;
				gm_WriteRegWord(GAMMA_LUT_MATRIX_MULT_COEF13, (WORD)DW_TempCoeff);//
				DW_TempCoeff = Bp_TempTable[14]| Bp_TempTable[15]<<8 ;
				gm_WriteRegWord(GAMMA_LUT_MATRIX_MULT_COEF21, (WORD)DW_TempCoeff);//
				DW_TempCoeff = Bp_TempTable[16]| Bp_TempTable[17]<<8 ;
				gm_WriteRegWord(GAMMA_LUT_MATRIX_MULT_COEF22, (WORD)DW_TempCoeff);//
				DW_TempCoeff = Bp_TempTable[12]| Bp_TempTable[13]<<8 ;
				gm_WriteRegWord(GAMMA_LUT_MATRIX_MULT_COEF23, (WORD)DW_TempCoeff);//
				DW_TempCoeff = Bp_TempTable[2]| Bp_TempTable[3]<<8 ;
				gm_WriteRegWord(GAMMA_LUT_MATRIX_MULT_COEF31, (WORD)DW_TempCoeff);//
				DW_TempCoeff = Bp_TempTable[4]| Bp_TempTable[5]<<8 ;
				gm_WriteRegWord(GAMMA_LUT_MATRIX_MULT_COEF32, (WORD)DW_TempCoeff);//
				DW_TempCoeff = Bp_TempTable[0]| Bp_TempTable[1]<<8 ;
				gm_WriteRegWord(GAMMA_LUT_MATRIX_MULT_COEF33, (WORD)DW_TempCoeff);//		

				//in
				
				DW_TempCoeff = Bp_TempTable[18]| Bp_TempTable[19]<<8 ;
				gm_WriteRegWord(GAMMA_LUT_MATRIX_IN_OFFSET1, (WORD)DW_TempCoeff);//
				DW_TempCoeff = Bp_TempTable[20]| Bp_TempTable[21]<<8 ;
				gm_WriteRegWord(GAMMA_LUT_MATRIX_IN_OFFSET2, (WORD)DW_TempCoeff);//
				DW_TempCoeff = Bp_TempTable[22]| Bp_TempTable[23]<<8 ;
				gm_WriteRegWord(GAMMA_LUT_MATRIX_IN_OFFSET3, (WORD)DW_TempCoeff);// 
				
				//out
				DW_TempCoeff = Bp_TempTable[24]| Bp_TempTable[25]<<8 ;
				gm_WriteRegWord(GAMMA_LUT_MATRIX_OUT_OFFSET1, (WORD)DW_TempCoeff);//
				DW_TempCoeff = Bp_TempTable[26]| Bp_TempTable[27]<<8 ;
				gm_WriteRegWord(GAMMA_LUT_MATRIX_OUT_OFFSET2, (WORD)DW_TempCoeff);//
				DW_TempCoeff = Bp_TempTable[28]| Bp_TempTable[29]<<8 ;
				gm_WriteRegWord(GAMMA_LUT_MATRIX_OUT_OFFSET3, (WORD)DW_TempCoeff);// 

				if(gm_ReadRegWord(DISP_LUT_CONTROL) != 0x3)
				   gm_WriteRegWord(DISP_LUT_CONTROL, 0x3);
				
				gm_SetRegBitsWord(GAMMA_LUT2_CTRL, BIT12);
				
				gm_SyncUpdate(gmvb_MainChannel, OUTPUT_CLOCK);

				ComponentUsed = ComponentUsed | C_Gamma_Matrix_Component;						

#ifdef SVAE_AT_THE_SAME_TIME
				if(UserPrefPresetColorSpace == CSPACE_CAL1)
					C_Selected_Profile=2;
				else
					C_Selected_Profile=3;	//CAL2
				
				DW_NvramAddr = C_GAMMA_REGISTER_START_OFFSET+ (0x2000*C_Selected_Profile) ; // 
				
				msg_ddc_xrite("DW_NvramAddr=%x",DW_NvramAddr);
				
				for( i = 0; i < W_Size; i++)
				{					  
					{							
					   NVRam_WriteByte(DW_NvramAddr, Bp_TempTable[i]);
					   DW_NvramAddr += 1;
					}																		
				 }					
#endif
				MarsInBusyState = FALSE;
			}
			break;

			case DDC2B_CMD_Xrite_EnterFactoryOSD:
				UserPrefFactoryFlag =1;
			break;
				
			case DDC2B_CMD_Xrite_CheckColorStatus:
				msg_ddc_xrite("DDC2B_CMD_Xrite_CheckColorStatus",0);
				CurrentVWDFalg=UserPrefVwdFlag;
				CurrentVWDDemo=UserPrefVwdDemo;	//VWD_DEMO_FULL-movie, VWD_DEMO_ADVANCE
				CurrentDCR= UserPrefDCR;
				CurrentEnergySmart=UserPrefEnergySmartOnOffFlag;
				CurrentDUMSwitch = UserPrefDUMSwitch;
				if(CurrentVWDFalg!=0)	//check smart video enhanced
				{
					UserPrefVwdFlag=0;
					UserPrefVwdFlag = VWD_DEMO_OFF;
				}
				if(CurrentDCR!=0)	//check DCR
					UserPrefDCR =OFF;
				
				if(CurrentEnergySmart!=0)	//check energy smart
					UserPrefEnergySmartOnOffFlag = OFF;

				if(CurrentDUMSwitch!=0)	//check DUM
					UserPrefDUMSwitch =OFF;

				break;

		case DDC2B_CMD_Xrite_CheckColorStatus_Recovery:
				msg_ddc_xrite("DDC2B_CMD_Xrite_CheckColorStatus_Recovery",0);
				UserPrefVwdFlag = CurrentVWDFalg;
				UserPrefVwdDemo = CurrentVWDDemo;
				UserPrefDCR = CurrentDCR;
				UserPrefEnergySmartOnOffFlag =CurrentEnergySmart;
				UserPrefDUMSwitch = CurrentDUMSwitch;
				break;

#ifdef Qubyx_Uniformity
		case DDC2B_CMD_Uniformity_Start:
            // Bypass ODP 3x3 and disable DUM
            gm_ClearRegBitsWord(GAMMA_LUT2_CTRL, GAMMA_MATRIX_CTRL);
            gm_ClearRegBitsWord(C9VODP_DUM_CONTROL_0, DUM_ENABLE);
            gm_SyncUpdate(gmvb_MainChannel, OUTPUT_CLOCK);
			break;
			
		case DDC2B_CMD_Uniformity_XY_ColorMono:
			{
			   WORD DUM_XGRID_SIZE1, DUM_YGRID_SIZE1, DUM_RSTART_XGRID1, DUM_NUM_XGRIDS1, DUM_NUM_YGRIDS1;
               DWORD DUM_XGRID_IINC1, DUM_YGRID_IINC1;
			   msg_ddc_xrite("~DDC2B_CMD_Uniformity_XY_ColorMono ~~",0);
			   
			   pTransferTable += 6;

			   memcpy(&Bp_TempTable[0], (BYTE*)pTransferTable, 5);

               // Set Grid
               // X
               DUM_NUM_XGRIDS1 = Bp_TempTable[0]|Bp_TempTable[1]<<8;

               // Y
               DUM_NUM_YGRIDS1 = Bp_TempTable[2]|Bp_TempTable[3]<<8;

         		DUM_XGRID_SIZE1 = PanelWidth / (DUM_NUM_XGRIDS1 - 1);
         		DUM_YGRID_SIZE1 = PanelHeight / (DUM_NUM_YGRIDS1 - 1);		

               DUM_XGRID_IINC1 = (DWORD)(524288 / DUM_XGRID_SIZE1);
               DUM_YGRID_IINC1 = (DWORD)(524288 / DUM_YGRID_SIZE1);

               DUM_RSTART_XGRID1 = (DUM_NUM_XGRIDS1 / 2);

               if (Bp_TempTable[4])
                  gm_SetRegBitsWord(C9VODP_DUM_CONTROL_0, DUM_3_COLOR);
               else
                  gm_ClearRegBitsWord(C9VODP_DUM_CONTROL_0, DUM_3_COLOR);

               // Also can enable (DUM_GRID_DOTS | DUM_3_COLOR for test Dots and color)
         		gm_WriteRegDWord_LE(C9VODP_DUM_NUM_GRIDS, ((DWORD)DUM_NUM_YGRIDS1 << 9)|DUM_NUM_XGRIDS1);
               gm_WriteRegDWord_LE(C9VODP_DUM_GRID_SZ, ((DWORD)DUM_YGRID_SIZE1 << 12)|(DUM_XGRID_SIZE1));
               gm_WriteRegWord(C9VODP_DUM_GLOBAL0, 0xFFFF);
               gm_WriteRegWord(C9VODP_DUM_GLOBAL0 + 2, 0x03FF);
               gm_WriteRegWord(C9VODP_DUM_GLOBAL1, 0x1FFF);
               gm_WriteRegDWord_LE(C9VODP_DUM_XGRID_IINC, DUM_XGRID_IINC1);
               gm_WriteRegDWord_LE(C9VODP_DUM_YGRID_IINC, DUM_YGRID_IINC1);
               gm_WriteRegWord(C9VODP_DUM_RSTART, DUM_RSTART_XGRID1);
			   
			   gm_ClearRegBitsWord(C9VODP_DUM_CONTROL_0, DUM_ENABLE);
				// Clear DUM ram
			   _fmemset(LINEAR_TO_FPTR(0x2E000L),0xFF,0x2000);	 
			}
         	break;
		 
		case DDC2B_CMD_Uniformity_Data:
			msg_ddc_xrite("~DDC2B_CMD_Uniformity_Data ~~",0);
			W_Size = B_vcp_subopcode;//B_data_length;
			B_blocknumber = (((DDC2Bi_CGS_VCPCmd_t*)gExtDebugCmdPtr)->dummy2);
			pTransferTable += 7;
			//take data from input buffer
			memcpy(&Bp_TempTable[0], (BYTE*)pTransferTable, W_Size);
						
			if(W_Size < DataSize)
            {
               gm_ClearRegBitsWord(C9VODP_DUM_CONTROL_0, DUM_ENABLE); // Must clear DUM_ENABLE for upload data.
               gm_LoadDUMTable(DataSize *B_blocknumber, W_Size/2, (WORD far*)Bp_TempTable);
            }
			else
			{
			   gm_ClearRegBitsWord(C9VODP_DUM_CONTROL_0, DUM_ENABLE); // Must clear DUM_ENABLE for upload data.
			   gm_LoadDUMTable(W_Size*B_blocknumber, W_Size/2, (WORD far*)Bp_TempTable);
			}
			break;

		case DDC2B_CMD_Uniformity_Save:
			{
				WORD W_RegAddr;
				BYTE far * Wp_Dest;
				WORD DW_NvramAddr = UniformityRegisterSettingStart2; //;
			
				msg_ddc_xrite("~DDC2B_CMD_Uniformity_Save ~~",0);
							 
				NVRam_WriteBuffer_Clear_NVRAM(0xD000,0x2100);
							 
				gm_SetRegBitsWord(C9VODP_DUM_CONTROL_0, DUM_ENABLE);
							 
							 // Save new data				   
				for(W_RegAddr = C9VODP_DUM_CONTROL_0; W_RegAddr < C9VODP_DUM_RSTART; )
				{
					NVRam_WriteWORD(DW_NvramAddr, gm_ReadRegWord(W_RegAddr));
					W_RegAddr += 2;
					DW_NvramAddr += 2;
				}

				DW_NvramAddr = UniformityIndexODDStart2; //
				gm_ClearRegBitsWord(C9VODP_DUM_CONTROL_0, DUM_ENABLE); // Must clear DUM_ENABLE for upload data.
				
				Wp_Dest = LINEAR_TO_FPTR_B(0x2E000);	//ODD 0x2E
				NVRam_WriteBuffer_DUM_Gamma(DW_NvramAddr, Wp_Dest, 4096);
					
				Wp_Dest = LINEAR_TO_FPTR_B(0x2F000);	//EVEN 0x2F
				DW_NvramAddr = UniformityIndexEvenStart2;
				NVRam_WriteBuffer_DUM_Gamma(DW_NvramAddr, Wp_Dest, 4096);
				msg_ddc_xrite("~DDC2B_CMD_Uniformity_Save end~~",0);
			}
			break;
			
#endif
	   	}

}
#endif

void PresetModeSettings(void)
{
	if(IsPresetMode_GRAPHICS_TEXT)
	{
		UserPrefBrightness = UserPrefBrightness_Text;
		UserPrefContrast = UserPrefContrast_Text;
		UserPrefPortSharpness = UserPrefPortSharpness_Text;
	}
	else if(IsColorSpace_GRAPHICS_CAL1)//120806 Edward add for CAL1/CAL2 save seperate
	{
		UserPrefBrightness = UserPrefBrightness_CAL1;
		UserPrefContrast = UserPrefContrast_CAL1;
		UserPrefPortSharpness = UserPrefPortSharpness_All;
	}
	else if(IsColorSpace_GRAPHICS_CAL2)
	{
		UserPrefBrightness = UserPrefBrightness_CAL2;
		UserPrefContrast = UserPrefContrast_CAL2;
		UserPrefPortSharpness = UserPrefPortSharpness_All;
	}
	else
	{
		UserPrefBrightness = UserPrefBrightness_All;
		UserPrefContrast = UserPrefContrast_All;
		UserPrefPortSharpness = UserPrefPortSharpness_All;
	}	
	
	if(IsPresetMode_GRAPHICS_MOVIE)
	{
			UserPrefHue = UserPrefHue_Movie;
			UserPrefSaturation = UserPrefSaturation_Movie;
	}
	else if(IsPresetMode_GRAPHICS_GAME)
	{
			UserPrefHue = UserPrefHue_Game;
			UserPrefSaturation = UserPrefSaturation_Game;
	}
	else
	{
			UserPrefHue = 0;
			UserPrefSaturation = WB_SATURATION_DEFAULT;
	}	

}
#undef __DDC2BI_COMM_C__


