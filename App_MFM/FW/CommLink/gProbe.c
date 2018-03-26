/*
    $Workfile:   gProbe.c  $
    $Revision: 1.18 $
    $Date: 2013/01/08 03:38:24 $
*/

#define __GPROBE_C__
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
// MODULE:      GProbe.c
//
// USAGE:       This module contains communication extension parser commands
//              The user should add his extension code in the TODO sections only
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include <string.h>
#include "System\all.h"
#include "CommLink\smt.h"
#include "CommLink\d_uart.h"
#include <mem.h>

//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************
#define WRITE_VARSIZE_REG_PATCH      //To enable this patch, need to open the same definition in DrvLib and rebuild/update
#define RESET_CMD_PATCH          // To enable this patch, need to open the same definition in DrvLib and rebuild/update
//#define TTX_EXTENDED_GPROBE_CMD

//#define SERIAL_CMD                    (*((BYTE*)gExtDebugCmdPtr+1))
// JasonL: port to ddc2bi.c
//#define DDC2BI_PKT_LEN                (*((BYTE*)gExtDebugCmdPtr+1))
//#define DDC2BI_CMD                    (*((BYTE*)gExtDebugCmdPtr+2))
//#define SERIAL_IN_DDC2BI_CMD        (*((BYTE*)gExtDebugCmdPtr+6))
#define CMD_SMTProtocol 0xFE

//******************************************************************************
//  E X T E R N A L
//******************************************************************************

#ifdef USER_EXTENDED_GPROBE_CMD
extern BYTE gBa_AppsTestParam[];
extern BYTE gBa_AppsTestParamCount;
#endif
#ifdef WRITE_VARSIZE_REG_PATCH   
void far WriteVarSizeRegisters(BYTE far* gExtDebugCmdPtr, HANDLER_USE_t B_whichHandler);
#endif
#ifdef RESET_CMD_PATCH
void far ResetCmdEx(BYTE far* gExtDebugCmdPtr, HANDLER_USE_t B_whichHandler);
#endif

//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************

ROM BYTE AckPakt[] =     {3,    CMD_ACK};
ROM BYTE NakPakt[] =     {3,    CMD_NAK};

//BYTE gDdc2biFuncNum;

extern    WORD gm_Uart1RcvBuf(BYTE fdata* pdata);//jmp_tbl

//******************************************************************************
//  S T A T I C    V A R I A B L E S
//******************************************************************************


//******************************************************************************
//  S T A T I C    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

//******************************************************************************
//  T T X    T E S T I N G
//******************************************************************************

#ifdef TTX_EXTENDED_GPROBE_CMD
extern BYTE gmvb_ValidIrData;

/***********************************************************************
 * TYPE: TT_CMD_RETURN_BUFFER
 *
 * DESCRIPTION:
 *   Response packet for read packet command.
 ***********************************************************************/
#define TT_CMD_POS    4

#define TT_CMD_NAK             CMD_NAK
#define TT_CMD_ACK             CMD_ACK
#define TT_CMD_INVALID         CMD_INVALID

typedef struct
{
   unsigned char   length;      // length of this response packet
   unsigned char   cmd;         // response packet type
   unsigned char   value;          // value read from register
   unsigned char   chk;         // checksum of packet

} TT_CMD_RETURN_BUFFER;

/***********************************************************************
 * TYPE: XTtxCommand
 ***********************************************************************/
typedef struct
{
   BYTE    CmdCode;
   BYTE    ScanCode;
//    BYTE    KeyEvent;
} ST_VIRTUAL_IR_EVENT;

typedef enum
{
   CMD_TTX_CREATE                     = 0x00,
   CMD_TTX_DESTROY                    = 0x01,
   CMD_TTX_RESET                      = 0x02,
   CMD_TTX_ACQ_ON                     = 0x03,
   CMD_TTX_ACQ_OFF                    = 0x04,
   CMD_TTX_UPDATE                     = 0x05,
   CMD_TTX_GET_STATUS                 = 0x06,
   CMD_TTX_GET_VERSION                = 0x07,
   CMD_TTX_DISPLAY_ON                 = 0x08,
   CMD_TTX_DISPLAY_OFF                = 0x09,
   CMD_TTX_SET_DISPLAY_PARAM          = 0x0A,
   CMD_TTX_GET_DISPLAY_PARAM          = 0x0B,
   CMD_TTX_ZOOM_MODE                  = 0x0C,
   CMD_TTX_UPDATE_MODE                = 0x0D,
   CMD_TTX_MIX_MODE                   = 0x0E,
   CMD_TTX_OVERWRITE_MODE             = 0x0F,

   CMD_TTX_NAVIGATION_UP              = 0x20,
   CMD_TTX_NAVIGATION_DOWN            = 0x21,
   CMD_TTX_NAVIGATION_LEFT            = 0x22,
   CMD_TTX_NAVIGATION_RIGHT           = 0x23,
   CMD_TTX_NAVIGATION_OK              = 0x24,

   CMD_TTX_NAVIGATION_DIGIT_0         = 0x30,
   CMD_TTX_NAVIGATION_DIGIT_1         = 0x31,
   CMD_TTX_NAVIGATION_DIGIT_2         = 0x32,
   CMD_TTX_NAVIGATION_DIGIT_3         = 0x33,
   CMD_TTX_NAVIGATION_DIGIT_4         = 0x34,
   CMD_TTX_NAVIGATION_DIGIT_5         = 0x35,
   CMD_TTX_NAVIGATION_DIGIT_6         = 0x36,
   CMD_TTX_NAVIGATION_DIGIT_7         = 0x37,
   CMD_TTX_NAVIGATION_DIGIT_8         = 0x38,
   CMD_TTX_NAVIGATION_DIGIT_9         = 0x39,
   CMD_TTX_NAVIGATION_RED             = 0x3A,
   CMD_TTX_NAVIGATION_GREEN           = 0x3B,
   CMD_TTX_NAVIGATION_YELLOW          = 0x3C,
   CMD_TTX_NAVIGATION_CYAN            = 0x3D,

   CMD_TTX_NAVIGATION_INDEX           = 0x40,
   CMD_TTX_NAVIGATION_CLOCK           = 0x41,
   CMD_TTX_NAVIGATION_REVEAL          = 0x42,
   CMD_TTX_NAVIGATION_HOLD            = 0x43,
   CMD_TTX_NAVIGATION_LIST            = 0x44,
   CMD_TTX_NAVIGATION_TOP_MENU        = 0x45,
   CMD_TTX_NAVIGATION_PAGE_UP         = 0x46,
   CMD_TTX_NAVIGATION_PAGE_DOWN       = 0x47,
   CMD_TTX_NAVIGATION_SUBTITLE        = 0x48,
   CMD_TTX_NAVIGATION_PREVIOUS_PAGE   = 0x49,
   CMD_TTX_NAVIGATION_PAGE            = 0x4A,

   CMD_TTX_SET_LANGUAGE_GROUP         = 0x50,
   CMD_TTX_SET_LIST_MODE_DATA         = 0x51,
   CMD_TTX_GET_LIST_MODE_DATA         = 0x52,
   CMD_TTX_SET_OSD_LANGUAGE           = 0x53,

   CMD_TTX_GET_HEADER                 = 0x60,
   CMD_TTX_GET_CLOCK_DATA             = 0x61,
   CMD_TTX_GET_PAGE_DATA              = 0x62,
   CMD_TTX_GET_DPB_CHECKSUM           = 0x63,
   CMD_TTX_GET_DPB_LINE               = 0x64,

} XTtxCommand;

/*
** The following table defines the virtual IR event that to be generated
** for the associated Remote command from RMRemote.
** The first column defines the command code from RMRemote tool.
** The second column define the physical IR scan code, an event is
** ... mapped to the scan code by the WB (defined in IRValues[] in Osd_Trtbl.c).
** When a Remote Command is received, the command code is search from the table
** ... to determine the scan code, then the Key Event is determine from table
** ... IRValue[].  The Key Event is generated for the OSD as if the event was
** ... from the IR Remote
**
** Note: A quicker implementation is to define CmdCode and the Key Event code
**            ... in the table.  The function first determine the scan code from the
**            ... command code, then index the KeyEvent immediately;
**            ... but it lose flexiblity that if WB remap the scan codes and event
**            ... codes, this file need to be change to synchronize the change.
*/
ST_VIRTUAL_IR_EVENT ROM VirtualIrEvent_Tbl[] =
{
   { CMD_TTX_NAVIGATION_RIGHT,   0x1b}, // Key0002 },    //IR_RIGHT
   { CMD_TTX_NAVIGATION_LEFT,    0x19}, // Key0003 },    //IR_LEFT
   { CMD_TTX_NAVIGATION_DOWN,    0x1d}, // Key0005 },    //IR_DOWN
   { CMD_TTX_NAVIGATION_UP,      0x17}, // Key0006 },    //IR_UP

   { CMD_TTX_NAVIGATION_DIGIT_0, 0x00}, // Key0014 },    //IR_ZERO
   { CMD_TTX_NAVIGATION_DIGIT_1, 0x01}, // Key0015 },    //IR_ONE
   { CMD_TTX_NAVIGATION_DIGIT_2, 0x02}, // Key0016 },    //IR_TWO
   { CMD_TTX_NAVIGATION_DIGIT_3, 0x03}, // Key0017 },    //IR_THREE
   { CMD_TTX_NAVIGATION_DIGIT_4, 0x04}, // Key0018 },    //IR_FOUR
   { CMD_TTX_NAVIGATION_DIGIT_5, 0x05}, // Key0019 },    //IR_FIVE
   { CMD_TTX_NAVIGATION_DIGIT_6, 0x06}, // Key0020 },    //IR_SIX
   { CMD_TTX_NAVIGATION_DIGIT_7, 0x07}, // Key0021 },    //IR_SEVEN
   { CMD_TTX_NAVIGATION_DIGIT_8, 0x08}, // Key0022 },    //IR_EIGHT
   { CMD_TTX_NAVIGATION_DIGIT_9, 0x09}, // Key0023 },    //IR_NINE

   { CMD_TTX_DISPLAY_ON,         0x2e}, // Key0026 },    //IR_TTX_CC
   { CMD_TTX_MIX_MODE,           0x2e}, // Key0026 },    //IR_TTX_CC
   { CMD_TTX_DISPLAY_OFF,        0x2e}, // Key0026 },    //IR_TTX_CC

   { CMD_TTX_NAVIGATION_RED,     0x28}, // Key0055 },    //IR_TTX_RED
   { CMD_TTX_NAVIGATION_GREEN,   0x2a}, // Key0056 },    //IR_TTX_GREEN
   { CMD_TTX_NAVIGATION_YELLOW,  0x2f}, // Key0057 },    //IR_TTX_YELLOW
   { CMD_TTX_NAVIGATION_CYAN,    0x30}, // Key0058 },    //IR_TTX_CYAN

   { CMD_TTX_NAVIGATION_SUBTITLE,0x3c}, // Key0059 },    //IR_SUB
   { CMD_TTX_NAVIGATION_INDEX,   0x27}, // Key0060 },    //IR_INDEX
   { CMD_TTX_ZOOM_MODE,          0x2b}, // Key0061 },    //IR_SIZE
   { CMD_TTX_NAVIGATION_REVEAL,  0x2c}, // Key0062 },    //IR_REVEAL
   { CMD_TTX_NAVIGATION_HOLD,    0x29}, // Key0063 },    //IR_HOLD

};

#define    SZ_VIRTUAL_IR_EVENT_TBL            \
 
(sizeof(VirtualIrEvent_Tbl)/sizeof(ST_VIRTUAL_IR_EVENT))

#endif

//******************************************************************************
//  C O D E
//******************************************************************************

//******************************************************************************
//
// FUNCTION       : PARSER_STATUS_t SerialCmdHandler(void)
//
// USAGE          : Serial commands handler to process gProbe commands
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : gExtDebugCmdPtr (R/W)
//                  gmvb_CommsMessageIsGprobe (W)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void gmp_SwitchClockRateforSerial(void);
PARSER_STATUS_t SerialCmdHandler(void)
{
#ifndef USE_SECOND_UART
//#define UART_SERIAL_CMD_HANDLER UART0
//#define SERIAL_X_USE SERIAL_USE_UART0

#define UART_SERIAL_CMD_HANDLER OSD_UART_DEF
#define SERIAL_X_USE OSD_UART_DEF
#else
#define UART_SERIAL_CMD_HANDLER UART1
#define SERIAL_X_USE SERIAL_USE_UART1
#endif
   PARSER_STATUS_t status;


#if (FEATURE_GPROBE_UART == REV2)
   {
      BYTE buf[SIO_RxBufLen];
      gDebugCmdPtr = (BYTE far*)buf;
      switch (gm_UartReadCmdPkt(UserPrefUartSelect, buf))
      {
         case READ_PKT_OK: // received packet is correct
            if ((status = gm_GprobeCmdParser(UserPrefUartSelect, buf)) == UNKNOWN_MSG_CMD) //process in IROM
               GprobeCmdParser(buf, UserPrefUartSelect); // Process command in XROM
            break;

         case READ_PKT_WRONG_CS: //wrong checksum
            gm_UartWriteCmdPkt(UserPrefUartSelect, NakPakt);// Send NAK
            break;
      }
   }
#endif
   return status;
}

//******************************************************************************
//
// FUNCTION       : void CommonGprobeCmdParser(BYTE B_Command,
//                                                          HANDLER_USE_t B_whichHandler)
//
// USAGE          : Common command parser, used for Gprobe commands
//
// INPUT          : B_Command - gProbe command number
//                  B_whichHandler - communication channel being used
//                     = SERIAL_USE (serial port)
//                     = DDC2BI_USE (DC2bi port)
//
// OUTPUT         : None
//
// GLOBALS        : gExtDebugCmdPtr  (R)
//                  ExtAckPkt        (RO)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void GprobeCmdParser(BYTE far* gExtDebugCmdPtr, HANDLER_USE_t B_whichHandler)
{
#define PRINT_PORT_TIMEOUT_MLS 10000

   gmt_RET_STAT status;
   static DWORD g_PrintPortTimer=0;
   // to support ddc2bi print: if there are no request from GProbe to print out messages thru ddc2bi port
   //(CMD_DDC2BiPrintPortSelect command) for a definite amount (PRINT_PORT_TIMEOUT_MLS) of time,
   //to use serial port instead.
   if ((g_PrintPortTimer < PRINT_PORT_TIMEOUT_MLS) ^ (g_PrintPortTimer < gm_GetSystemTime()))
//#ifndef USE_SECOND_UART
//     gm_SetPrintPort(SERIAL_USE);
//#else
      gm_SetPrintPort(UserPrefUartSelect);
//#endif

   switch (*(gExtDebugCmdPtr+1))
   {
      case CMD_WrNvramBlock:            // write Nvram from RAM buffer
         //
         // Write NVRAM data from RAM buffer
         //
         status = NVRam_WriteBuffer(
                     (((WrNvramBlockCmd_t*)gExtDebugCmdPtr)->NvramAddr_hi << 8) |
                     ((WrNvramBlockCmd_t*)gExtDebugCmdPtr)->NvramAddr_lo,
                     (BYTE *)((((WrNvramBlockCmd_t*)gExtDebugCmdPtr)->RamAddr_hi << 8) |
                              ((WrNvramBlockCmd_t*)gExtDebugCmdPtr)->RamAddr_lo),
                     (((WrNvramBlockCmd_t*)gExtDebugCmdPtr)->Length_hi << 8) |
                     ((WrNvramBlockCmd_t*)gExtDebugCmdPtr)->Length_lo);

         if (status == OK)
         {                                   // Send ACK response
            gm_SendAckOrNack(SND_ACK, B_whichHandler, gExtDebugCmdPtr);
         }
         break;

      case CMD_RdNvram:                        // Read data packet from nvram
         //
         // Format repsonse packet length
         //
         ((RdCmdPkt_t*)gExtDebugCmdPtr)->length  =
            ((RdNvramCmd_t*)gExtDebugCmdPtr)->length  + 3;
         //
         // Read NVRAM data into buffer
         //
         status = NVRam_ReadBuffer(
                     (((RdNvramCmd_t*)gExtDebugCmdPtr)->NvramAddr_hi<<8)|
                     ((RdNvramCmd_t*)gExtDebugCmdPtr)->NvramAddr_lo,
                     ((RdCmdPkt_t*)gExtDebugCmdPtr)->value,
                     ((RdNvramCmd_t*)gExtDebugCmdPtr)->length);

         if (status == OK)
         {                                   // Send packet response
            gm_WriteCmdPkt ((BYTE far*)gExtDebugCmdPtr , B_whichHandler);
         }
         break;

      case CMD_WrNvram:                        // Write data packet from nvram

         break;

      case CMD_DeviceId: //---0x30--------------------------------------------------
      {       
          gmt_DeviceIdIndex DevIdIndex = ((DevIdCmd_t fdata*)gExtDebugCmdPtr)->index;
          char *Bp_IdString = (char *)&((DevIdResp_t fdata*)gExtDebugCmdPtr)->idstring;
          FLASH_DEVICE_PARAMETER_SET far *Sp_FlashParam = gm_SpiGetParameters();
          
          memset(Bp_IdString, 0, ID_STRING_LEN);
          
          switch(DevIdIndex)
          {
             case DEVID_CHIP_ID:
                Bp_IdString[0] = 'A';   //"ATH20: means Athena 2.0"
                Bp_IdString[1] = 'T';
                Bp_IdString[2] = 'H';
                Bp_IdString[3] = '2';
                Bp_IdString[4] = '0';
                break;
             case DEVID_VENDOR_ID:
                Bp_IdString[0] = 'S';
                Bp_IdString[1] = 'T';            
                break;
             case DEVID_PRODUCT_ID:
                #if defined(EV3_93xx_BOARD)
                Bp_IdString[0] = 'E';
                Bp_IdString[1] = 'V';   
                Bp_IdString[2] = '3';
                #elif defined(EV1_93xx_BOARD)
                Bp_IdString[0] = 'E';
                Bp_IdString[1] = 'V';   
                Bp_IdString[2] = '1';
                #elif defined(EV2_93xx_BOARD)
                Bp_IdString[0] = 'E';
                Bp_IdString[1] = 'V';   
                Bp_IdString[2] = '2';                
                #elif defined(RD3_93xx_BOARD)
                Bp_IdString[0] = 'R';
                Bp_IdString[1] = 'D';   
                Bp_IdString[2] = '3';  
					 #elif defined(ST_4K2K_93xx_BOARD)
                Bp_IdString[0] = 'S';
                Bp_IdString[1] = '4';   
                Bp_IdString[2] = 'K'; 
                #elif defined(RD2_93xx_BOARD)
                Bp_IdString[0] = 'R';
                Bp_IdString[1] = 'D';   
                Bp_IdString[2] = '2';                   
                #elif defined(RD1_93xx_BOARD)
                Bp_IdString[0] = 'R';
                Bp_IdString[1] = 'D';   
                Bp_IdString[2] = '1';               
                #endif
                Bp_IdString[3] = '-';                  
                Bp_IdString[4] = 'A';  //board revision               
                break;
             case DEVID_FLASH_MAUFACTURE_ID:
             {
                gm_SpiDetectFlashDevice(FLASH_READID_TYPE1, CMD_READ_ID);   
                if((Sp_FlashParam->B_ManID == 0)||(Sp_FlashParam->B_ManID == 0xff))
                   gm_SpiDetectFlashDevice(FLASH_READID_TYPE2, CMD_READ_ID2);

                gm_ByteToHexChars(Sp_FlashParam->B_ManID, Bp_IdString); 
                break;
             }
             case DEVID_FLASH_DEVICE_ID:
             {
                gm_SpiDetectFlashDevice(FLASH_READID_TYPE1, CMD_READ_ID);   
                if((Sp_FlashParam->B_DevID == 0)||(Sp_FlashParam->B_DevID == 0xff))
                {
                   gm_SpiDetectFlashDevice(FLASH_READID_TYPE2, CMD_READ_ID2);
                   gm_ByteToHexChars(Sp_FlashParam->B_TypeID, &Bp_IdString[0]);  
                   gm_ByteToHexChars(Sp_FlashParam->B_DevID,  &Bp_IdString[2]);  
                }
                else
                {
                   gm_ByteToHexChars(Sp_FlashParam->B_DevID,  &Bp_IdString[0]);
                }
                break;
             }
             case DEVID_IROM_MAJOR_VERSION:
             {
                itoa(IROM_MAJOR, Bp_IdString, 10);
                break;            
             }
             case DEVID_IROM_MINOR_VERSION:
             {
                itoa(IROM_MINOR, Bp_IdString, 10);
                break;
             }
             case DEVID_FW_MAJOR_VERSION:
             {
                TXromConfigData far * pXromConfigData = (TXromConfigData far *)(((DWORD)W_SegEndExtRom<<16)+CONFIG_DATA_OFFSET);
                gm_ByteToHexChars(pXromConfigData->fw_major,  Bp_IdString);
                //itoa(pXromConfigData->fw_major, Bp_IdString, 10);
                break;
             }              
             case DEVID_FW_MINOR_VERSION:
             {
                TXromConfigData far * pXromConfigData = (TXromConfigData far *)(((DWORD)W_SegEndExtRom<<16)+CONFIG_DATA_OFFSET);
                gm_ByteToHexChars(pXromConfigData->fw_minor,  Bp_IdString);
                //itoa(pXromConfigData->fw_minor, Bp_IdString, 10);
                break;
             }
             default: 
                gm_SendAckOrNack(SND_NAK, B_whichHandler, gExtDebugCmdPtr);
                break;
          }
          
          ((DevIdResp_t fdata*)gExtDebugCmdPtr)->length = strlen(Bp_IdString)+3;//+3:length,command,null-terminated
          gm_WriteCmdPkt(gExtDebugCmdPtr, B_whichHandler);
          break;
      }

#ifdef WRITE_VARSIZE_REG_PATCH
      case CMD_VarLenRegWrite: //---0x47, Variable length register (memory) write----------
          WriteVarSizeRegisters(gExtDebugCmdPtr, B_whichHandler);
          break;
#endif
#ifdef RESET_CMD_PATCH
      case CMD_Reset:
         ResetCmdEx(gExtDebugCmdPtr, B_whichHandler);
         break;
#endif

#ifdef USER_EXTENDED_GPROBE_CMD
      case CMD_SetParam:
         _fmemcpy(gBa_AppsTestParam+(gExtDebugCmdPtr[2]-1)*sizeof(DWORD),   gExtDebugCmdPtr+3, sizeof(DWORD));
         gm_SendAckOrNack(SND_ACK, B_whichHandler, gExtDebugCmdPtr);
         gBa_AppsTestParamCount++;
         break;

      case CMD_AppsTest:
 #if 0//def TTX_EXTENDED_GPROBE_CMD
         // hook to TARA's TTX Remote Test tool
         if (gExtDebugCmdPtr[2] == 3) // Teletext commands
         {
            if (!gm_VirtualIrEventSet( (BYTE *)gExtDebugCmdPtr))
               TT_CMDHandler( gExtDebugCmdPtr );
            gm_WriteCmdPkt (gExtDebugCmdPtr, B_whichHandler);
         }
         else
            gm_Print("parser extension is working - others",0);
 #else
         {
            BYTE B_TestNum = ((AppsFuncCmd_t*)gExtDebugCmdPtr)->Index;
            gm_SendAckOrNack(SND_ACK, B_whichHandler, gExtDebugCmdPtr);	            
            UserExtendableCmd(B_whichHandler,B_TestNum); //Process AppsTest
            gBa_AppsTestParamCount = 0;
         }
 #endif
         break;
#endif // USER_EXTENDED_GPROBE_CMD

      case CMD_SMTProtocol:
#ifdef USE_SMT
         SMT_MessageHandler((MsgPacket_t *)gExtDebugCmdPtr, B_whichHandler);
#else
         gm_Print("ERROR: SMT protocol is not supported",0);
         gm_SendAckOrNack(SND_NAK, B_whichHandler, gExtDebugCmdPtr);
#endif
         break;

      case CMD_FWDMSG:
         FwdMsg(gExtDebugCmdPtr);
         FwdResponse(gExtDebugCmdPtr, B_whichHandler);
         break;

      case CMD_DDC2BiPrintPortSelect:
         gm_SetPrintPort(B_whichHandler);
         g_PrintPortTimer = gm_GetSystemTime() + PRINT_PORT_TIMEOUT_MLS;
         gm_SendAckOrNack(SND_ACK, B_whichHandler, gExtDebugCmdPtr);
         break;

      default:
         return;
   }
}

#ifdef USER_EXTENDED_GPROBE_CMD
#ifdef TTX_EXTENDED_GPROBE_CMD

//******************************************************************************
//
// FUNCTION       : BOOL gm_VirtualIrEventSet (BYTE *aCommandBuffer)
//
// USAGE          : The function handles the remote command request from
//                  Tara's GMRemote test tool.  The request buffer contains
//                  command to control the TTX decoder functions, such as
//                  display On/Off, decoder create/destroy etc ...
//                  In order to simulate an user event to the gm_OsdEventHander
//                  for multi-thread, an virtual IR event is created based on
//                  the input in the command buffer.  For requests that are
//                  related to user input, no virtual event will be created.
//                  The table VirtualIrEvent_Tbl[] defines all remote commands
//                  that an event need to be created.
//
// INPUT          : aCommandBuffer - points to the input request
//
// OUTPUT         : TRUE if event was created; FALSE if no event created
//
// GLOBALS        : VirtualIrEvent_Tbl         (RO)
//                  NUMBER_OF_IRValues        (RO)
//                  IRValues                    (RO)
//                  gmvb_ValidIrData                (W)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BOOL gm_VirtualIrEventSet (BYTE *aCommandBuffer)
{
   BYTE CmdCode = aCommandBuffer[TT_CMD_POS];
   TT_CMD_RETURN_BUFFER* rtb = (TT_CMD_RETURN_BUFFER*)aCommandBuffer;
   ST_VIRTUAL_IR_EVENT ROM *ptr;

   /*
   ** For each incomming request, first determine from the VirtualIrEvent_Tbl
   ** if an event is to be created; if so, then find the logical event from
   ** the IRValues[] table against the scan code.
   */
   for (ptr=VirtualIrEvent_Tbl; ptr<&VirtualIrEvent_Tbl[SZ_VIRTUAL_IR_EVENT_TBL]; ptr++)
   {
      if (ptr->CmdCode == CmdCode)
      {
         BYTE thisScanCode = ptr->ScanCode;
         /*
         ** For the valid command, create a virtual IR key press event for
         ** the associated Ir key, followed by a key release event.
         */
         for (CmdCode = 0; CmdCode < NUMBER_OF_IRValues; CmdCode++)
         {
            if (thisScanCode == IRValues[CmdCode].Code)
            {
               gmvb_ValidIrData = IRValues[CmdCode].KeyIndex;
#if (WB_MAJOR == 0x0004)
               EnqueueOsdKey(thisScanCode);
#else
               EnqueueOsdKey();
#endif

               /*
               ** Emulate key release event
               */
               gmvb_ValidIrData = Key_None;
#if (WB_MAJOR == 0x0004)
               EnqueueOsdKey(thisScanCode);
#else
               EnqueueOsdKey();
#endif

               /*
               ** Setup command response to GMRemote
               */
               rtb->cmd = TT_CMD_ACK;
               rtb->length = 3;

               return TRUE;        // indicates request has been handled
            }
         }
      }
   }

   /*
   ** If the request is not defined as an user input, don't create any
   ** virtual event
   */
   return FALSE;        // indicated request is not handled
}



#endif

//******************************************************************************
//
// FUNCTION       : DWORD GetParam(BYTE B_Index)
//
// USAGE          : Get command parameters from buffer, parameters are POP
//                  out based on data type (BYTE, WORD or DWORD)
//
// INPUT          : B_Index = parameter index number (start from 0)
//
// OUTPUT         : parameters in 4 bytes
//
// GLOBALS        : gBa_AppsTestParam (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
DWORD GetParam(BYTE B_Index)
{
   DWORD DW_Data = 0;
   BYTE  B_NumOfByte = sizeof(DWORD);     // Make sure is 4 bytes
   BYTE *Bp_Param = gBa_AppsTestParam + B_Index * sizeof(DWORD);// Point to MSB of parameter

   while (1)  // Number of byte to get
   {
      DW_Data  |= *Bp_Param++;
      if (!--B_NumOfByte)
         break;            // Add data
      DW_Data <<= 8;
   }
   return (DW_Data);                       // return parameter
}
#else
// To remove compiler error when disable it.
DWORD GetParam(BYTE B_Index){return B_Index;}

#endif // #ifdef USER_EXTENDED_GPROBE_CMD
//*********************************  END  **************************************

