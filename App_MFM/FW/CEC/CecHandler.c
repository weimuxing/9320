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
// MODULE:      CECHandler.c
//
// USAGE:       This file contains functions for CEC driver functionality
//
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "system\all.h"

//this is a special variable to eliminate build expression from workbench project
//it equals FALSE when CEC disabled
#if (FEATURE_CEC == DISABLE)
const BOOL CEC_ENABLED = gmd_FALSE;
#else
const BOOL CEC_ENABLED = gmd_TRUE;
#endif

#if (FEATURE_CEC == ENABLE)

#include "StdLibInc\gm_Register.h"
#include "StdLibInc\gm_Mcu186.h"
#include <embedded.h>
#include <mem.h>
#include <string.h>
#include "CEC\CecUserCommandsHandler.h"
#include "CEC\CecHandler.h"

#if (FEATURE_LOW_POWER_MONITOR == ENABLE)
#include "StdLibInc\gm_LPM_IPC.h"
#include "CEC\CecIPCHandler.h"
#endif

#include "CEC\CecHandler.h"

// For use CEC_DEBUG mode it is necessary to substitute CecDebug.lib with name "Cec.Lib"
// instead Cec.lib into folder StdLib

#define xCEC_DEBUG

#define DEFAULT_INTERNAL_SOURCE     VGA


//******************************************************************************
//  G L O B A L    F U N C T I O N S
//******************************************************************************

//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************

#define CEC_REG_BASE_ADDR             CEC_CLK_DIV //0x0DC64


// Size of Rx buffer
#define CEC_FRAMES_BUFFER_SIZE  60 //100

BYTE Ba_CecRxData[CEC_FRAMES_BUFFER_SIZE];

BYTE B_CecRxWrIndex = 0;
BYTE B_CecRxRdIndex = 0;

#ifdef CEC_DEBUG
#define CEC_SIZE_OF_DEBUG_LOG 200
WORD Wa_CecLogData[CEC_SIZE_OF_DEBUG_LOG];
#endif

// MsgId for transmitted <Abort> message
BYTE B_AbortMsdId;

// Saved UserPrefAudioVolume we should save it when switching to AVR and restore when switching back to TV
BYTE B_SavedAudioVolume;
BYTE B_SavedMuteStatus;

//extern gmt_CecAppUserCommandHandlerInterface gm_UserCommandInterface;

extern void CecAppPowerHandler(void);

#ifdef FEATURE_CEC_GPIO
extern BYTE B_ErrorCode;
extern WORD W_TempTickCount;
extern WORD W_TempCurTime;
#endif

extern gmt_PHY_CH B_FocusBorder; // Channel with Focus Border

#if (FEATURE_CEC == ENABLE)
extern BYTE B_UserCommandExecution;
#endif
//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************

#define CEC_PHYSICAL_ADDRESS        0x0000

#define CEC_TV_ADDRESS              0x00
#define CEC_FREEUSE_ADDRESS         0x0E
#define CEC_UNREG_ADDRESS           0x0F

#define ADVANCED_CEC_FEATURES

#ifdef  ADVANCED_CEC_FEATURES

#define CEC_CFG_PH_ADDR         0
#define CEC_CFG_LANGUAGE        4
#define CEC_CFG_VERSION         8
#define CEC_CFG_PWR_STAT       10
#define CEC_CFG_VENDOR_ID      12
#define CEC_CFG_OSD_NAME       16

#define CEC_DEVICE_TYPE         0

#endif

#define msg(message)    gm_Print(message,0)
//#define msg(message)

//******************************************************************************
//  F U N C T I O N S '    P R O T O T Y P E S
//******************************************************************************
void far gm_Print (const char far*Bp_Str, ...);

void CecAppLogAddrHandler(void);

BOOL CecAppLogAddrHandlerStatusObtained(void);

BOOL CecCustomRxMessageProcess(void);

void CecOsdHandler(void);
void CecSourceChangeHandler(void);

void CecAppShowOsdStringHandler(void);
BOOL CecAppShowOsdStringHandlerRequest(gmt_OSDSTRINGCOMMAND OsdCommand, BYTE Str[],BYTE B_Len);

#ifdef CEC_GET_MENU_LANGUAGE_CAPABLE
gmt_CEC_LANGUAGE_INDEX CecGetLanguageIdx(BYTE OSDLanguage);
#endif

#ifdef CEC_SET_MENU_LANGUAGE_CAPABLE
gmt_RET_STAT CecGetLanguage(char *Bp_Data, BYTE *Bp_Language);
#endif

#ifdef CEC_GET_CEC_VERSION_CAPABLE
void CecFeatureCecVersion(BYTE B_DestLogAddr);
#endif

#ifdef CEC_GET_CEC_VERSION_CAPABLE
void CecFeatureGetCecVersion(BYTE B_DestLogAddr);
#endif

#ifdef CEC_GIVE_DEVICE_POWER_STATUS_CAPABLE
void CecFeatureGiveDevicePowerStatus(BYTE B_DestLogAddr);
#endif

#ifdef CEC_REPORT_POWER_STATUS_CAPABLE
void CecFeatureReportPowerStatus(BYTE B_DestLogAddr);
#endif

#ifdef CEC_ACTIVE_SOURCE_CAPABLE
void CecFeatureActiveSource(BYTE B_DestLogAddr);
#endif

#ifndef LPM_PERMANENTLY_CONTROLS_PERIPHERAL
void far gm_CecIRQHandler(void);
BOOL CecAppLogAddrHandlerStatusObtained(void);
#endif

static void CecFeatureAbort(BYTE B_FeatureOpCode, gmt_MNGR_CEC_ABORT_REASONS FeatureAbortReason, BYTE B_DestLogAddr);

void ReportPhysicalAddress(BYTE B_DestAddr);

void PrepareRecordOnPacket2(BYTE RecorderLogAddr, BYTE CecMessageBuffer[], BYTE*B_Length);

#ifdef LOW_POWER_MONITOR_SUPPORT
void CecCfgLpmHandler(void);
void CecLpmConfig(BYTE B_CecConfigState);
#endif

//******************************************************************************
//  L O C A L    V A R I A B L E S
//******************************************************************************
#ifdef LOW_POWER_MONITOR_SUPPORT
BYTE B_CecCfgState = 0;
BYTE Ba_CecCfgData[20];
#endif

BYTE CECString[20];

// TO DO this variables initialized incorrectly
// The best decision is creating special handler which is active after
// board started work, checks active input and sets these variables to corresp. values
WORD ActiveSourcePhisicalAddr = CEC_PHYSICAL_ADDRESS;
static BYTE ActiveSourceLogicalAddr = 0;

BYTE Ba_StatusLine[STATUSLINELENGTH+1] = "                    ";

// for TV 0000 is correct physical address, for other devices - it is not
// correct physical address, so 0x0000 is good default value.
gmt_CEC_DEVICERECORD a_CECDevices[16]=
{{0x0000,NULL},{0x0000,NULL},
   {0x0000,NULL},{0x0000,NULL},
   {0x0000,NULL},{0x0000,NULL},
   {0x0000,NULL},{0x0000,NULL},
   {0x0000,NULL},{0x0000,NULL},
   {0x0000,NULL},{0x0000,NULL},
   {0x0000,NULL},{0x0000,NULL},
   {0x0000,NULL},{0x0000,NULL}
};

char ROM *ROM Cec_Language_codes[] =
{
   "eng",
   "fre",
   "fra",
   "ger",
   "deu"
};

//******************************************************************************
//  M A C R O   D E F I N I T I O N
//******************************************************************************
//Number of retransmissions is transmission was unsuccessful (1-5)
#define CEC_RETRANSMIT_NUM      5

//if you received cec message with PHYSICAL_ADDRESS in bytes byteN,byteN+1,
//(for example 1.0.0.0) then it will be word 0x0010
#define PHYS_ADDR_TO_WORD(DIG1,DIG2,DIG3,DIG4)  (WORD)(((((WORD)(DIG1))&0x0F)<<4)|\
                                                        (((WORD)(DIG2))&0x0F)|\
                                                       ((((WORD)(DIG3))&0x0F)<<12)|\
                                                       ((((WORD)(DIG4))&0x0F)<<8))

//Previous attempt to send frame unsuccessful
#define CEC_RETRANSMIT_SFT      4

#if 1
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
// The following code is subject to remove after CEC library will be updated
//
//#define ABORT_INCORRECT                 0x0100
#define CEC_BROADCAST                   0x0080
#define CEC_DIRECT                      0x0040
#define CEC_REJECT_FROM_UNREGISTERED    0x0020
#define LEN_GREATER                     0x0010
#define LEN_UNKNOWN                     0
#define LEN_MASK                        0x000F
#define LEN(I)               ((BYTE)(I & LEN_MASK))

#define DEBUGON
//incorrect messages will be FEATURE_ABORTED

//#define MIN(a,b) ((a) > (b) ? (b) : (a))
#define MIN(a,b) (((a)<(b))?(a):(b))


BYTE CEC_ROM Ba_CECMessagesRules[]=
{
   CEC_DIRECT|LEN(3),                  // <0x00 Feature abort>
   0x00,                               // 0x01
   0x00,                               // 0x02
   0x00,                               // 0x03
   CEC_DIRECT|LEN(1),                  // <0x04 Image View On>
   CEC_DIRECT,                         // <0x05 Tuner Step Increment>        TV doesn't process this message as follower
   CEC_DIRECT,                         // <0x06 Tuner Step Decrement>        TV doesn't process this message as follower
   CEC_DIRECT,                         // <0x07 Tuner Device Status>
   CEC_DIRECT,                         // <0x08 Give Tuner Device Status>    TV doesn't process this message as follower
   CEC_DIRECT,                         // <0x09 Record On>                   TV doesn't process this message as follower
   CEC_DIRECT|LEN(2),                  // <0x0A Record Status>
   CEC_DIRECT,                         // <0x0B Record Off>                  TV doesn't process this message as follower
   0x00,                               // 0x0C
   CEC_DIRECT,                         // <0x0D Text View On>
   0x00,                               // 0x0E
   CEC_DIRECT,                         // <0x0F Record TV Screen>

   0x00,                               // 0x10
   0x00,                               // 0x11
   0x00,                               // 0x12
   0x00,                               // 0x13
   0x00,                               // 0x14
   0x00,                               // 0x15
   0x00,                               // 0x16
   0x00,                               // 0x17
   0x00,                               // 0x18
   0x00,                               // 0x19
   CEC_DIRECT,                         // <0x1A Give Deck Status>            TV doesn't process this message as follower
   CEC_DIRECT|LEN(2),                  // <0x1B Deck Status>
   0x00,                               // 0x1C
   0x00,                               // 0x1D
   0x00,                               // 0x1E
   0x00,                               // 0x1F

   0x00,                               // 0x20
   0x00,                               // 0x21
   0x00,                               // 0x22
   0x00,                               // 0x23
   0x00,                               // 0x24
   0x00,                               // 0x25
   0x00,                               // 0x26
   0x00,                               // 0x27
   0x00,                               // 0x28
   0x00,                               // 0x29
   0x00,                               // 0x2A
   0x00,                               // 0x2B
   0x00,                               // 0x2C
   0x00,                               // 0x2D
   0x00,                               // 0x2E
   0x00,                               // 0x2F

   0x00,                               // 0x30
   0x00,                               // 0x31
   CEC_BROADCAST | LEN(4),             // <0x32 Set Menu Language>
   CEC_DIRECT,                         // <0x33 Clear Analog Timer>          TV doesn't process this message as follower
   CEC_DIRECT,                         // <0x34 Set Analog Timer>            TV doesn't process this message as follower
   CEC_DIRECT |LEN_GREATER|LEN(1),     // <0x35 Timer Status>
   CEC_DIRECT|CEC_BROADCAST,           // <0x36 Standby>
   0x00,                               // 0x37
   0x00,                               // 0x38
   0x00,                               // 0x39
   0x00,                               // 0x3A
   0x00,                               // 0x3B
   0x00,                               // 0x3C
   0x00,                               // 0x3D
   0x00,                               // 0x3E
   0x00,                               // 0x3F

   0x00,                               // 0x40
   CEC_DIRECT|CEC_BROADCAST,           // <0x41 Play>                        TV doesn't process this message as follower
   CEC_DIRECT|CEC_BROADCAST,           // <0x42 Deck Control>                TV doesn't process this message as follower
   CEC_DIRECT|LEN(2),                  // <0x43 Timer Cleared Status>
   CEC_DIRECT,                         // <0x44 User Control Pressed>
   CEC_DIRECT,                         // <0x45 User Control Released>
   CEC_DIRECT,                         // <0x46 Give OSD Name>
   CEC_DIRECT|LEN_GREATER|LEN(1),      // <0x47 Set OSD Name>
   0x00,                               // 0x48
   0x00,                               // 0x49
   0x00,                               // 0x4A
   0x00,                               // 0x4B
   0x00,                               // 0x4C
   0x00,                               // 0x4D
   0x00,                               // 0x4E
   0x00,                               // 0x4F

   0x00,                               // 0x50
   0x00,                               // 0x51
   0x00,                               // 0x52
   0x00,                               // 0x53
   0x00,                               // 0x54
   0x00,                               // 0x55
   0x00,                               // 0x56
   0x00,                               // 0x57
   0x00,                               // 0x58
   0x00,                               // 0x59
   0x00,                               // 0x5A
   0x00,                               // 0x5B
   0x00,                               // 0x5C
   0x00,                               // 0x5D
   0x00,                               // 0x5E
   0x00,                               // 0x5F

   0x00,                               // 0x60
   0x00,                               // 0x61
   0x00,                               // 0x62
   0x00,                               // 0x63
   CEC_DIRECT|LEN_GREATER|LEN(1),      // <0x64 Set OSD String>
   0x00,                               // 0x65
   0x00,                               // 0x66
   CEC_DIRECT,                         // <0x67 Set Timer Program Title>
   0x00,                               // 0x68
   0x00,                               // 0x69
   0x00,                               // 0x6A
   0x00,                               // 0x6B
   0x00,                               // 0x6C
   0x00,                               // 0x6D
   0x00,                               // 0x6E
   0x00,                               // 0x6F

   CEC_DIRECT,                         // <0x70 System Audio Mode Request>
   CEC_DIRECT,                         // <0x71 Give Audio Status>
   CEC_DIRECT|CEC_BROADCAST | LEN(2),  // <0x72 Set System Audio Mode>
   0x00,                               // 0x73
   0x00,                               // 0x74
   0x00,                               // 0x75
   0x00,                               // 0x76
   0x00,                               // 0x77
   0x00,                               // 0x78
   0x00,                               // 0x79
   CEC_DIRECT | LEN(2),                // <0x7A Report Audio Status>
   0x00,                               // 0x7B
   0x00,                               // 0x7C
   CEC_DIRECT,                         // <0x7D Give System Audio Mode Status>
   CEC_DIRECT | LEN(2),                // <0x7E System Audio Mode Status>
   0x00,                               // 0x7F

   CEC_BROADCAST,                      // <0x80 Routing Change>
   CEC_BROADCAST,                      // <0x81 Routing Information>
   CEC_BROADCAST | LEN(3),             // <0x82 Active Source>
   CEC_DIRECT,                         // <0x83 Give Physical Address>
   CEC_BROADCAST | LEN(4) | CEC_REJECT_FROM_UNREGISTERED,
   // <0x84 Report Physical Address>

   CEC_BROADCAST,                      // <0x85 Request Active Source>
   CEC_BROADCAST,                      // <0x86 Set Stream Path>
   CEC_BROADCAST,                      // <0x87 Device Vendor Id>
   0x00,                               // 0x88
   CEC_DIRECT,                         // <0x89 Vendor Command>
   CEC_DIRECT|CEC_BROADCAST,           // <0x8A Vendor Remote Button Down>
   CEC_DIRECT|CEC_BROADCAST,           // <0x8B Vendor Remote Button Up>
   CEC_DIRECT,                         // <0x8C Give Device Vendor Id>
   CEC_DIRECT,                         // <0x8D Menu Request>
   CEC_DIRECT,                         // <0x8E Menu Status>
   CEC_DIRECT,                         // <0x8F Power Status>

   CEC_DIRECT | LEN(2),                // <0x90 Report Power Status>
   CEC_DIRECT,                         // <0x91 Get Menu Language>
   CEC_DIRECT,                         // <0x92 Select Analogue Service>
   CEC_DIRECT,                         // <0x93 Select Digital Service>
   0x00,                               // 0x94
   0x00,                               // 0x95
   0x00,                               // 0x96
   CEC_DIRECT,                         // <0x97 Set Digital Timer>
   0x00,                               // 0x98
   CEC_DIRECT,                         // <0x99 Clear Digital Timer>
   CEC_DIRECT,                         // <0x9A Set Audio Rate>
   0x00,                               // <0x9B
   0x00,                               // <0x9C
   CEC_DIRECT | LEN(3),                // <0x9D Inactive Source>
   CEC_DIRECT | LEN(2),                // <0x9E CEC Version>
   CEC_DIRECT,                         // <0x9F Get CEC Version>

   CEC_DIRECT|CEC_BROADCAST,           // <0xA0 Vendor Command With Id>
   CEC_DIRECT,                         // <0xA1 Clear External Timer>
   CEC_DIRECT,                         // <0xA2 Set External Timer>
   0x00,                               // 0xA3
   0x00,                               // 0xA4
   0x00,                               // 0xA5
   0x00,                               // 0xA6
   0x00,                               // 0xA7
   0x00,                               // 0xA8
   0x00,                               // 0xA9
   0x00,                               // 0xAA
   0x00,                               // 0xAB
   0x00,                               // 0xAC
   0x00,                               // 0xAD
   0x00,                               // 0xAE
   0x00,                               // 0xAF

   0x00,                               // 0xB0
   0x00,                               // 0xB1
   0x00,                               // 0xB2
   0x00,                               // 0xB3
   0x00,                               // 0xB4
   0x00,                               // 0xB5
   0x00,                               // 0xB6
   0x00,                               // 0xB7
   0x00,                               // 0xB8
   0x00,                               // 0xB9
   0x00,                               // 0xBA
   0x00,                               // 0xBB
   0x00,                               // 0xBC
   0x00,                               // 0xBD
   0x00,                               // 0xBE
   0x00,                               // 0xBF

   0x00,                               // 0xC0
   0x00,                               // 0xC1
   0x00,                               // 0xC2
   0x00,                               // 0xC3
   0x00,                               // 0xC4
   0x00,                               // 0xC5
   0x00,                               // 0xC6
   0x00,                               // 0xC7
   0x00,                               // 0xC8
   0x00,                               // 0xC9
   0x00,                               // 0xCA
   0x00,                               // 0xCB
   0x00,                               // 0xCC
   0x00,                               // 0xCD
   0x00,                               // 0xCE
   0x00,                               // 0xCF

   0x00,                               // 0xD0
   0x00,                               // 0xD1
   0x00,                               // 0xD2
   0x00,                               // 0xD3
   0x00,                               // 0xD4
   0x00,                               // 0xD5
   0x00,                               // 0xD6
   0x00,                               // 0xD7
   0x00,                               // 0xD8
   0x00,                               // 0xD9
   0x00,                               // 0xDA
   0x00,                               // 0xDB
   0x00,                               // 0xDC
   0x00,                               // 0xDD
   0x00,                               // 0xDE
   0x00,                               // 0xDF

   0x00,                               // 0xE0
   0x00,                               // 0xE1
   0x00,                               // 0xE2
   0x00,                               // 0xE3
   0x00,                               // 0xE4
   0x00,                               // 0xE5
   0x00,                               // 0xE6
   0x00,                               // 0xE7
   0x00,                               // 0xE8
   0x00,                               // 0xE9
   0x00,                               // 0xEA
   0x00,                               // 0xEB
   0x00,                               // 0xEC
   0x00,                               // 0xED
   0x00,                               // 0xEE
   0x00,                               // 0xEF

   0x00,                               // 0xF0
   0x00,                               // 0xF1
   0x00,                               // 0xF2
   0x00,                               // 0xF3
   0x00,                               // 0xF4
   0x00,                               // 0xF5
   0x00,                               // 0xF6
   0x00,                               // 0xF7
   0x00,                               // 0xF8
   0x00,                               // 0xF9
   0x00,                               // 0xFA
   0x00,                               // 0xFB
   0x00,                               // 0xFC
   0x00,                               // 0xFD
   0x00,                               // 0xFE
   CEC_DIRECT                          // <0xFF Abort>
};


gmt_CEC_MSG_VALIDITY_CHECK_RESULT far gm_CecUtilMessageValidityCheck1(BYTE B_Code, BYTE B_Length, BYTE B_SrcAddr, BOOL Directed)
{
   gmt_CEC_MSG_VALIDITY_CHECK_RESULT rslt = gmd_SC_OK;

   if (B_Length == 0)
   {
      rslt = gmd_SC_ERROR_LEN;
      return rslt;
   }

#ifdef DEBUGON
   gm_Print("Ba_CECMessagesRules[0x%x]=0x%x",B_Code,Ba_CECMessagesRules[B_Code]);
#endif

   //this message doesn't exist
   if (Ba_CECMessagesRules[B_Code] == 0)
   {
      rslt |= gmd_SC_ERROR_OP_CODE;
#ifdef DEBUGON
      gm_Print("Invalid opcode %d",B_Code);
#endif
   }

   if (Directed)
   {
      if ((Ba_CECMessagesRules[B_Code] & CEC_DIRECT) == 0)
      {
         rslt |= gmd_SC_ERROR_DIR;
#ifdef DEBUGON
         gm_Print("Invalid direction 1",0);
#endif
      }
   }
   else
   {
      if ((Ba_CECMessagesRules[B_Code] & CEC_BROADCAST) == 0)
      {
         rslt |= gmd_SC_ERROR_DIR;
#ifdef DEBUGON
         gm_Print("Invalid direction 2",0);
#endif
      }
   }

   if (B_Length > 15)
   {
      rslt |= gmd_SC_ERROR_LEN;
   }
   if ((Ba_CECMessagesRules[B_Code] & LEN_MASK) != LEN_UNKNOWN)
   {
      if (Ba_CECMessagesRules[B_Code] & LEN_GREATER)
      {
         if (LEN(B_Length) <= (Ba_CECMessagesRules[B_Code] & LEN_MASK))
         {
            rslt |= gmd_SC_ERROR_LEN;
         }
      }
      else if ((Ba_CECMessagesRules[B_Code] & LEN_MASK) != LEN(B_Length))
      {
         rslt |= gmd_SC_ERROR_LEN;
#ifdef DEBUGON
         gm_Print("Invalid length",0);
#endif
      }
   }
   if ((Ba_CECMessagesRules[B_Code] & CEC_REJECT_FROM_UNREGISTERED) && (B_SrcAddr == gmd_Unregistered))
      rslt |= gmd_SC_ERROR_SOURCE;
#if 0
   if ((rslt != gmd_SC_OK) && (Ba_CECMessagesRules[B_Code] & ABORT_INCORRECT) && (Directed == TRUE))
      rslt |= gmd_SC_ERROR_ABORT;
#endif

   return rslt;
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#endif

//******************************************************************************
//  C O D E
//******************************************************************************
WORD GetActiveSourcePhysicalAddress(void)
{
   return ActiveSourcePhisicalAddr;
}

BYTE GetActiveSourceLogicalAddress(void)
{
   return ActiveSourceLogicalAddr;
}


//*********************************************************************************************
//
// PrintToOsdHandler
//

//APPLICATION

#define PRINT_OSD_BUFFER_SIZE   60
static const char far * PrintOsdBuffer[PRINT_OSD_BUFFER_SIZE];
static int PrintOsdWrIndex = 0;
static int PrintOsdRdIndex = 0;
static int PrintOsdRdStrIndex = 0;

void InitPrintToOsd(void)
{
   UserPrefCecStatusText1_seg = FP_SEG(Ba_StatusLine);
   UserPrefCecStatusText1_ofs = FP_OFF(Ba_StatusLine);
   UserPrefCecStatusText2_seg = FP_SEG(Ba_StatusLine);
   UserPrefCecStatusText2_ofs = FP_OFF(Ba_StatusLine);
   UserPrefCecStatusText3_seg = FP_SEG(Ba_StatusLine);
   UserPrefCecStatusText3_ofs = FP_OFF(Ba_StatusLine);

   //initialization for <Show OSD Text>
   UserPrefPtrSegment = FP_SEG(CECString);
   UserPrefPtrOffset = FP_OFF(CECString);

   //UserPrefOSDIsReadyToShowCECString = 1;
}

void ClearPrintOsdBuffer(void)
{
   int i;
   for (i = 0; i < PRINT_OSD_BUFFER_SIZE; i++)
   {
      PrintOsdBuffer[i] = (const char far * )NULL;
   }
   PrintOsdWrIndex = 0;
   PrintOsdRdIndex = 0;
   PrintOsdRdStrIndex = 0;
   UserPrefCecStatusLineCounter = 0;
}

void PrintToOsd(const char far *str)
{
   if (PrintOsdWrIndex < PRINT_OSD_BUFFER_SIZE)
   {
      PrintOsdBuffer[PrintOsdWrIndex++] = str;
   }
}

void PrintToOsdHandler(void)
{
   int RAMBufferCtr = 0;
   char tmp;
   _fmemset(Ba_StatusLine,' ',STATUSLINELENGTH);
   //for(;PrintOsdRdIndex < PRINT_OSD_BUFFER_SIZE; )
   while (PrintOsdRdIndex < PRINT_OSD_BUFFER_SIZE)
   {
      if (PrintOsdBuffer[PrintOsdRdIndex] == (const char far * )NULL)
      {
         break;
      }
      for ( ; ; PrintOsdRdStrIndex++)
      {
         if ((*(PrintOsdBuffer[PrintOsdRdIndex]+PrintOsdRdStrIndex) == 0))
         {
            PrintOsdRdIndex++;
            PrintOsdRdStrIndex = 0;
            break;
         }
         tmp = *(PrintOsdBuffer[PrintOsdRdIndex]+PrintOsdRdStrIndex);
         if (tmp == '\n')
         {
            RAMBufferCtr = STATUSLINELENGTH;
            PrintOsdRdStrIndex++;
            break;
         }
         if (tmp == '\r')
         {
            continue;
         }
         if (RAMBufferCtr >= STATUSLINELENGTH)
         {
            break;
         }
         Ba_StatusLine[RAMBufferCtr++] = tmp;
      }
      if (RAMBufferCtr >= STATUSLINELENGTH)
      {
         break;
      }
      else
      {
         PrintOsdRdStrIndex = 0;
      }
   }
   if (RAMBufferCtr > 0)
   {
      OSDRequestMsg(gmvb_MainChannel, MTO_CEC_Text4UserReady, REQUEST_NOW);
   }
}

//******************************************************************************
// USAGE:   This function inits CEC subsystem. It should be called befor other CEC
//          code.
// INPUT:
// OUTPUT:
// GLOBALS:
//******************************************************************************
//APPLICATION
void CecInit(void)
{
   gmt_CEC_INIT S_InitStructure;

#ifdef CEC_DEBUG
   gmt_CEC_DBG_INIT S_DebugInitStructure;
#endif

#ifndef FEATURE_CEC_GPIO
#ifndef LPM_PERMANENTLY_CONTROLS_PERIPHERAL

   gm_Print("CecInit",0);
   gm_Print(__DATE__"  "__TIME__,0);

   S_InitStructure.W_CecRegBaseAddress         = CEC_REG_BASE_ADDR;
   S_InitStructure.B_NumberOfRetransmissions   = CEC_RETRANSMIT_NUM;
   S_InitStructure.B_SizeOfRxBuff              = CEC_FRAMES_BUFFER_SIZE;
   S_InitStructure.Bp_CecRxBufferAddress       = &Ba_CecRxData[0];
   S_InitStructure.DW_TClkHz                   = gmc_TCLK_FREQ;

   //gm_Print("App CLK LO %d",(WORD)(S_InitStructure.DW_TClkHz & 0xFFFF));
   //gm_Print("App CLK HI %d",(WORD)((S_InitStructure.DW_TClkHz >> 16) & 0xFFFF));

   gm_CecInit(&S_InitStructure);

#ifdef CEC_DEBUG
   S_DebugInitStructure.B_SizeOfLogBuff        = CEC_SIZE_OF_DEBUG_LOG;
   S_DebugInitStructure.Wp_CecLogBufferAddress = &Wa_CecLogData[0];

   gm_CecDebugInit(&S_DebugInitStructure);
#endif

   gm_InterruptSetFunc(IRQ1, gm_CecIRQHandler);

   INTERRUPT_ENABLE(MISC_OCMMASK, CEC_IRQ_MASK);
#else
   gm_Print("CecInit under LPM control",0);

   gm_CecSetLogAddr(0);
#endif
#else
   S_InitStructure.B_NumberOfRetransmissions   = CEC_RETRANSMIT_NUM;
   S_InitStructure.B_SizeOfRxBuff                = CEC_FRAMES_BUFFER_SIZE;
   S_InitStructure.B_TimeRetransmission           = CEC_RETRANSMIT_SFT;

   gm_CecInit(&S_InitStructure);
   gm_Print("CecInit for GPIO-driver",0);
#endif
   InitPrintToOsd();
   gm_CecMngrTxManagerReset();
}


//******************************************************************************
// USAGE:
//
// INPUT:   None
// OUTPUT:  None
// GLOBALS:
//******************************************************************************
void CecHandler(void)
{
   BYTE B_Stop = 0;

#ifdef CEC_DEBUG
   gm_CecPrintLog();
#endif

#ifdef LOW_POWER_MONITOR_SUPPORT
   CecIpcHandler();
#endif

   if (!CecAppLogAddrHandlerStatusObtained())
   {
      CecAppLogAddrHandler();
   }
   else
   {
      CecAppPowerHandler();

      do
      {
         gm_CecMngrRxHandler();

         B_Stop = CecCustomRxMessageProcess();

      } while (!B_Stop);

      gm_CecMngrTxHandler();

      CecOsdHandler();
      CecSourceChangeHandler();
      CecAppSystemAudioHandler();

#ifdef LOW_POWER_MONITOR_SUPPORT
      CecCfgLpmHandler();
#endif

#if 0
      gm_CecAppsTestHandler(0, gmd_None);
#endif
   }
}


//LIBRARY
#ifndef LPM_PERMANENTLY_CONTROLS_PERIPHERAL
//******************************************************************************
// USAGE:
//
// INPUT:
// OUTPUT:
// GLOBALS:
//******************************************************************************
void far gm_CecIRQHandler(void)
{
   if (gm_ReadRegWord(MISC_OCM_STATUS) & BIT1)
   {
      //B_IRQFlag = 1;
      gm_CecIsr();
      gm_WriteRegWord(MISC_OCM_STATUS, BIT1);
   }
}
#endif

#if 1
//LIBRARY
//******************************************************************************
// USAGE:
//
// INPUT:
// OUTPUT:
// GLOBALS:
//******************************************************************************
// CEC logical address allocation machine state
// added to library with name gm_CecAppLogAddrHandler and other names with gm_ prefix

typedef enum
{
   LAA_StartPolling,
   LAA_Polling,
   LAA_NotAvaible,
   LAA_obtained
}gmt_CEC_LAA_State;

static BYTE B_CEC_LAA_State = LAA_StartPolling;

// added to library with name
// BOOL gm_CecAppLogAddrHandlerStatusObtained(void)
BOOL CecAppLogAddrHandlerStatusObtained(void)
{
   return (B_CEC_LAA_State == LAA_obtained);
}

// added to library with name
// BOOL gm_CecAppLogAddrHandlerRequest(void)
BOOL CecAppLogAddrHandlerRequest(void)
{
   if (B_CEC_LAA_State == LAA_obtained)
   {
      B_CEC_LAA_State = LAA_StartPolling;
      return gmd_TRUE;
   }
   else
      return gmd_FALSE;
}

// added to library with name
// void gm_CecAppLogAddrHandler(void)
void CecAppLogAddrHandler(void)
{
   BYTE Bp_TxData[3];

   if (B_CEC_LAA_State == LAA_obtained) return;

   switch (B_CEC_LAA_State)
   {
      case LAA_StartPolling:
         //if(gm_TimingIsStable(CH_A))
         {
#if CEC_PHYSICAL_ADDRESS == 0x00
            gm_CecSetLogAddr(CEC_TV_ADDRESS);
#else
            gm_CecSetLogAddr(CEC_FREEUSE_ADDRESS);
#endif
            B_CEC_LAA_State = LAA_Polling;

            gm_CecTxFrame((gm_CecGetLogAddr() << 4) | (gm_CecGetLogAddr() & 0x0F), 0, Bp_TxData);
            //gm_CecMngrTxMessage(gm_CecGetLogAddr(),0,Bp_TxData,0,0);
            gm_Print("Ping CEC %d",gm_CecGetLogAddr());
         }
         break;

      case LAA_Polling:
#if 1
         if (gm_CecGetFrameStatus() & gmd_CEC_FRAME_TX_DONE)
         {
            gm_CecClearFrameStatusBits(gmd_CEC_FRAME_TX_DONE);

            if (gm_CecGetFrameStatus() & gmd_CEC_FRAME_TX_ACK)
            {
               gm_Print("LA is busy %d",gm_CecGetLogAddr());
               B_CEC_LAA_State = LAA_NotAvaible;
            }
            else
            {
               //   gm_Print("LA is obtained %d",gm_CecGetLogAddr());
               B_CEC_LAA_State = LAA_obtained;
            }
         }
#endif
         break;

      case LAA_NotAvaible:
         if (gm_CecGetLogAddr() == CEC_TV_ADDRESS)
         {
            gm_CecSetLogAddr(CEC_FREEUSE_ADDRESS);
            B_CEC_LAA_State = LAA_Polling;
            //gm_CECTxFrame((gm_CecDrvGetLogAddr() << 4) | (gm_CecDrvGetLogAddr() & 0x0F), 0, &(Ba_CECTxFrameBuffer[2]));
            gm_CecTxFrame((gm_CecGetLogAddr() << 4) | (gm_CecGetLogAddr() & 0x0F), 0, Bp_TxData);
            //gm_CecMngrTxMessage(gm_CecGetLogAddr(),0,Bp_TxData,0,0);
         }
         else
         {
            gm_CecSetLogAddr(CEC_UNREG_ADDRESS);
            gm_Print("LA is obtained %d",gm_CecGetLogAddr());
            B_CEC_LAA_State = LAA_obtained;
         }
         break;
   }
}
#endif

//LIBRARY
//added to library with name
//void gm_CecUtilRequestAudioRcvrAudioStatus(void)
#if 1
void RequestAudioReceiverAudioStatus(void)
{
   BYTE Ba_CecMsg[1];
   Ba_CecMsg[0] = CEC_MSG_GIVE_AUDIO_STATUS;
   gm_CecMngrTxMessage(gmd_AudioSystem, 1, Ba_CecMsg, 0, 0);
}
#endif

//Application routine
void HandleSetSystemAudioStatus(BYTE B_SystemAudioMode)
{
   BYTE B_SystemAudioStatus = UserPrefCecSystemAudioStatus;
   UserPrefCecSystemAudioStatus = B_SystemAudioMode;
   OSDRequestMsg(gmvb_MainChannel, MTO_SystemAudioStatusChanged, REQUEST_NOW);
   if (B_SystemAudioMode == gmd_SAS_ON)
   {
      //mute tv speakers
      if (B_SystemAudioStatus == gmd_SAS_OFF)
      {
         gm_Print("Saving Volume and Speakers",0);
         B_SavedAudioVolume = UserPrefAudioVolume;
         B_SavedMuteStatus = UserPrefAudioMute; // UserPrefSpeakers;
      }
      gm_Print("Mute",0);
      RequestAudioReceiverAudioStatus();
   }
   else
   {
      //unmute tv speakers
      //restore saved values only if previous status was AVR ON
      if (B_SystemAudioStatus == gmd_SAS_ON)
      {
         gm_Print("Restoring Volume and Speakers",0);
         UserPrefAudioVolume = B_SavedAudioVolume;
         //UserPrefSpeakers = B_SavedMuteStatus;
         UserPrefAudioMute = B_SavedMuteStatus;
      }
      gm_Print("Unmute",0);
      AdjustAudioVolume();
      OSDRequestMsg(gmvb_MainChannel, MTO_DrawVolumeSlider, REQUEST_NOW);
   }
}

//******************************************************************************
// USAGE: If device potentially can record signal this function returns TRUE,
//        else returns false.
// INPUT:
// OUTPUT:
// GLOBALS:
//******************************************************************************
// this function added to library with name
// BOOL gm_CecUtilIsDeviceAbleToRecord(BYTE B_LogAddr)
static BOOL CanDeviceRecord(BYTE B_LogAddr)
{
   if ((B_LogAddr == gmd_Recorder1) ||
         (B_LogAddr == gmd_Recorder2) ||
         (B_LogAddr == gmd_Recorder3) ||
         (B_LogAddr == gmd_Reserved12) ||
         (B_LogAddr == gmd_Reserved13))
      return TRUE;
   else
      return FALSE;
}

//******************************************************************************
// USAGE:   This is the main handler which handles incoming CEC messages
//
// INPUT:   None
// OUTPUT:  returns true if received message implies one more message should be
//          handled as soon as possible
// GLOBALS:
//******************************************************************************
BOOL CecCustomRxMessageProcess(void)
{
   static BYTE B_LastAudioVolume;           //necessary to supress redundant drawing slider (to redraw slider only on change)
   BYTE B_DestLogAddr;
   BYTE B_SrcLogAddr;
   BYTE B_DataLen;
   BYTE Ba_Data[18];
   BYTE i;
   WORD W_MessageLength;
   gmt_CEC_MSG_VALIDITY_CHECK_RESULT rslt;
   BYTE B_IsBroadcastMsg;

   if (gm_CecMngrRxMessage(&B_SrcLogAddr, &B_DestLogAddr, &B_DataLen, Ba_Data) == TRUE)
   {
      //gm_Print("B_DestLogAddr 0x%x",B_DestLogAddr);
      //gm_Print("B_SrcLogAddr 0x%x",B_SrcLogAddr);
      //gm_Print("B_DataLen 0x%x",B_DataLen);

      // for(i = 0;i < B_DataLen; i++)
      //     gm_Print("Ba_Data 0x%x",Ba_Data[i]);
      B_IsBroadcastMsg = (B_DestLogAddr == CEC_BROADCAST_LOG_ADDR);
#if 1
      rslt = gm_CecUtilMessageValidityCheck1(Ba_Data[0], B_DataLen, B_SrcLogAddr, (!B_IsBroadcastMsg));
      if (rslt & gmd_SC_ERROR_DIR) msg("Incorrect CEC message format: invalid direction");
      if (rslt & gmd_SC_ERROR_LEN) msg("Incorrect CEC message format: invalid length");
      if (rslt & gmd_SC_ERROR_OP_CODE) msg("Incorrect CEC message format: invalid opcode");
      if (rslt & gmd_SC_ERROR_SOURCE) msg("Incorrect CEC message format: incorrect source address");
      if (rslt == gmd_SC_OK) msg("CEC message passed format check");
      if (rslt & gmd_SC_ERROR_ABORT)
      {
         CecFeatureAbort(Ba_Data[0],CEC_AR_UNRECOGN_OPCODE, B_SrcLogAddr);
         return TRUE;
      }
      if (rslt != gmd_SC_OK)
         return TRUE;
#endif
      switch (Ba_Data[0])  // Opcode
      {
         case CEC_MSG_TIMER_STATUS: // <Get CEC version>
            //do nothing
            //this case just to avoid sending FEATURE_ABORT as reply on incoming <timer status>
            break;
         case CEC_MSG_FEATURE_ABORT: // <Get CEC version>
            //do nothing
            //this case just to avoid sending FEATURE_ABORT as reply on incoming <feature abort>
            break;
         case CEC_MSG_DECK_STATUS:
            //probably here we can print at screen deck status,
            //but it is not clear is it necessary or not.
            break;
         case CEC_MSG_RECORD_STATUS:
            //probably here we can print at screen record status,
            //but it is not clear is it necessary or not.
            break;
         case CEC_MSG_SET_SYSTEM_AUDIO_MODE:
         case CEC_MSG_SYSTEM_AUDIO_MODE_STATUS:
            HandleSetSystemAudioStatus(Ba_Data[1]);
            break;
         case CEC_MSG_REPORT_AUDIO_STATUS:
            if ((B_SrcLogAddr != gmd_AudioSystem) ||
                  (UserPrefCecSystemAudioStatus != gmd_SAS_ON)) break;

            //if mute status changed then we are updating mute icon
            //if volume changed we are drawing volume slider
            //if (UserPrefSpeakers != ((Ba_Data[1] & 0x80) == 0x80 ? D_MUTE_ON : D_MUTE_OFF))
            if (UserPrefAudioMute!= ((Ba_Data[1] & 0x80) == 0x80 ? D_MUTE_ON : D_MUTE_OFF))
            {
               //UserPrefSpeakers = ((Ba_Data[1] & 0x80) == 0x80 ? D_MUTE_ON : D_MUTE_OFF);
               UserPrefAudioMute = ((Ba_Data[1] & 0x80) == 0x80 ? D_MUTE_ON : D_MUTE_OFF);
               OSDRequestMsg(gmvb_MainChannel, MTO_UpdateMuteIcon, REQUEST_NOW);
            }
            if ((B_LastAudioVolume != (Ba_Data[1] & 0x7F)) ||
                  (UserPrefAudioVolume != (BYTE)((((WORD)(B_LastAudioVolume)) * MAXIMAL_VOLUME_VALUE) / 100)))
            {
               B_LastAudioVolume = (Ba_Data[1] & 0x7F);
               UserPrefAudioVolume = (BYTE)((((WORD)(B_LastAudioVolume)) * MAXIMAL_VOLUME_VALUE) / 100);
               OSDRequestMsg(gmvb_MainChannel, MTO_DrawVolumeSlider, REQUEST_NOW);
            }
            break;
#ifdef CEC_RECORD_TV_SCREEN
         case CEC_MSG_RECORD_TV_SCREEN:
            if (!CanDeviceRecord(B_SrcLogAddr))
               break;
            PrepareRecordOnPacket2(B_SrcLogAddr, Ba_Data, &B_DataLen);
            if (B_DataLen == 0)
            {
               CecFeatureAbort(CEC_MSG_RECORD_TV_SCREEN, CEC_AR_CANNOT_PROVIDE_SRC, B_SrcLogAddr);
               break;
            }
            //we don't monitor message status because this is a request on <Record TV Screen>
            //so its a recorder's task to handle result of transaction.
            gm_CecMngrTxMessage(B_SrcLogAddr, B_DataLen, Ba_Data, 0, 0);
            break;
#endif

#ifdef CEC_FEATURE_ABORT
         case CEC_MSG_ABORT: // <Abort> message
            {
               if (!B_IsBroadcastMsg)
               {// Send <Feature Abort> message
                  CecFeatureAbort(CEC_MSG_ABORT,CEC_AR_UNRECOGN_OPCODE, B_SrcLogAddr);
               }
            }
            break;
#endif

#ifdef CEC_IMAGE_VIEW_ON_CAPABLE
         case CEC_MSG_IMAGE_VIEW_ON: // <Image View On> message

            if (!UserPrefPowerState)
            {
               OSDRequestMsg(gmvb_MainChannel, MTO_WakeUp, REQUEST_NOW);
               gm_Print("MTO_WakeUp",0);
            }
            break;
#endif
#ifdef CEC_TEXT_VIEW_ON_CAPABLE
         case CEC_MSG_TEXT_VIEW_ON:  // <Text View On> message
            {
               if (!UserPrefPowerState)
               {// Send MTO_WakeUp message to OSD handler
                  OSDRequestMsg(gmvb_MainChannel, MTO_WakeUp, REQUEST_NOW);
               }


#ifndef NO_PIP_SUPPORT
               if (UserPrefPipMode != NO_PIP)
               {
                  if (IsInternalHdmiDviPort(gmvb_MainChannel) == TRUE)
                  {
                     UserPrefPipMode = NO_PIP;
                     AdjustPipMode();
                  }
               }
#endif

               if (IsInternalHdmiDviPort(gmvb_MainChannel) == TRUE)
                  OSDRequestMsg(gmvb_MainChannel, MTO_RemoveOSD, REQUEST_NOW);
            }
            break;
#endif

            //CUSTOMER APPLICATION
#ifdef CEC_SET_MENU_LANGUAGE_CAPABLE
         case CEC_MSG_SET_MENU_LANGUAGE: // <Set Menu Language>
            {
               Ba_Data[4] = '\0';

               if (CecGetLanguage((char*)&Ba_Data[1],&UserPrefOsdLanguage))
               {
                  AdjustOsdLanguage();

#ifdef LOW_POWER_MONITOR_SUPPORT
                  CecLpmConfig(0);
#endif
               }
               else
               {// Send <Feature Abort> message
                  CecFeatureAbort(CEC_MSG_STANDBY, CEC_AR_INCORRECT_MODE, B_SrcLogAddr);
               }
            }
            break;
#endif

            //CUSTOMER APPLICATION
#ifdef CEC_STANDBY_CAPABLE
         case CEC_MSG_STANDBY: //<Standby> message
            {
               if (UserPrefPowerState)
               {
                  OSDRequestMsg(gmvb_MainChannel, MTO_GoingToSleep, REQUEST_NOW);
               }
               else
               { // Send <Feature Abort> message
                  CecFeatureAbort(CEC_MSG_STANDBY, CEC_AR_INCORRECT_MODE, B_SrcLogAddr);
               }
            }
            break;
#endif

#ifdef CEC_SET_OSD_STRING_CAPABLE
         case CEC_MSG_SET_OSD_STRING: // <Set OSD String> message
            {
               //gm_Print("CEC_MSG_SET_OSD_STRING",0);
               //if (UserPrefOSDIsReadyToShowCECString)
               {
                  //gm_Print("UserPrefOSDIsReadyToShowCECString = true",0);
                  CecAppShowOsdStringHandlerRequest((gmt_OSDSTRINGCOMMAND)Ba_Data[1], Ba_Data+2,B_DataLen - 2);
               }
#if 0
               else
               {
                  CecFeatureAbort(CEC_MSG_SET_OSD_STRING, CEC_AR_REFUSED, B_SrcLogAddr);
               }
#endif
            }
            break;
#endif

#ifdef CEC_ACTIVE_SOURCE_CAPABLE
         case CEC_MSG_ACTIVE_SOURCE: // <Active Source> message
            ActiveSourcePhisicalAddr = *((WORD*)(&Ba_Data[1]));
            ActiveSourceLogicalAddr = B_SrcLogAddr;

            if ((B_UserCommandExecution & (BIT1|BIT0)) ==0)
            {
               switch ((Ba_Data[1] & 0xF0))
               {
                  case 0x10:
                     //OSDRequestMsg(gmvb_MainChannel, MTO_ChangeInputToHDMI, REQUEST_NOW);
                     if ((UserPrefCurrentInputMain != ALL_BOARDS_HDMI) &&
                           (UserPrefCurrentInputPip != ALL_BOARDS_HDMI))
                        UserPrefPreviousInputMain = UserPrefCurrentInputMain;
                     UserPrefCurrentInputMain = ALL_BOARDS_HDMI;
                     AdjustCurrentInputMain();
                     break;
                  case 0x20:
                     //OSDRequestMsg(gmvb_MainChannel, MTO_ChangeInputToHDMI2, REQUEST_NOW);
                     if ((UserPrefCurrentInputMain != ALL_BOARDS_HDMI2) &&
                           (UserPrefCurrentInputPip != ALL_BOARDS_HDMI2))
                        UserPrefPreviousInputMain = UserPrefCurrentInputMain;
                     UserPrefCurrentInputMain = ALL_BOARDS_HDMI2;
                     AdjustCurrentInputMain();
                     break;
               }
            }
            break;
#endif

#ifdef CEC_REQUEST_ACTIVE_SOURCE_CAPABLE
         case CEC_MSG_REQUEST_ACTIVE_SOURCE: // <Request Active Source> message
            {
               //if (IsCecInternal(UserPrefCurrentInputMain))
               if (IsHDMIInput(UserPrefCurrentInputMain) == gmd_FALSE)
               {
#ifdef CEC_ACTIVE_SOURCE_CAPABLE
                  CecFeatureActiveSource(CEC_BROADCAST_LOG_ADDR);
#endif
               }
            }
            break;
#endif

#ifdef CEC_GIVE_DEVICE_POWER_STATUS_CAPABLE
         case CEC_MSG_GIVE_DEVICE_POWER_STATUS: // <Give Device Power Status>
            {
               // Send <Report Power Status> message
               CecFeatureReportPowerStatus(B_SrcLogAddr);
            }
            break;
#endif

#ifdef CEC_GIVE_PHYSICAL_ADDRESS_CAPABLE
         case CEC_MSG_GIVE_PHYSICAL_ADDRESS: // <Give Physical Address> message
            {
               ReportPhysicalAddress(CEC_BROADCAST_LOG_ADDR);
            }
            break;
#endif

#ifdef CEC_REPORT_POWER_STATUS_CAPABLE
         case CEC_MSG_REPORT_POWER_STATUS: // <Report Power Status>
            {
               //a_CECDevices[B_SrcLogAddr].PowerStatus = Ba_Data[1];
               //do nothing power status handled in special handler
               //this message used only for test purposes
            }
            break;
#endif

#ifdef CEC_GET_MENU_LANGUAGE_CAPABLE
         case CEC_MSG_GET_MENU_LANGUAGE: // <Get Menu Language>
            {
               BYTE B_LangIdx = CecGetLanguageIdx(UserPrefOsdLanguage);

               //  Send <Set Menu Language> message
               Ba_Data[0] = CEC_MSG_SET_MENU_LANGUAGE;     // Opcode

               _fmemcpy((void far*)(Ba_Data + 1),
                        ((void far*)(Cec_Language_codes [B_LangIdx])), 3);

               gm_CecMngrTxMessage(CEC_BROADCAST_LOG_ADDR, 4, &(Ba_Data[0]), 0, 0);
            }
            break;
#endif

#ifdef CEC_INACTIVE_SOURCE_CAPABLE
         case CEC_MSG_INACTIVE_SOURCE: // <Inactive Source>
            {//CEC Custom Application
               if (InputPortToPhysicalAddress(UserPrefCurrentInputMain) == *((WORD*)(&Ba_Data[1])))
               {
                  UserPrefCurrentInputMain = DEFAULT_INTERNAL_SOURCE;
                  AdjustCurrentInputMain();
               }
               if (InputPortToPhysicalAddress(UserPrefCurrentInputPip) == *((WORD*)(&Ba_Data[1])))
               {
                  UserPrefCurrentInputPip = DEFAULT_INTERNAL_SOURCE;
                  AdjustCurrentInputPip();
               }
#ifdef CEC_ACTIVE_SOURCE_CAPABLE
               CecFeatureActiveSource(CEC_BROADCAST_LOG_ADDR);
#endif
            }
            break;
#endif

#ifdef CEC_MSG_REPORT_PHYSICAL_ADDRESS_CAPABLE
         case CEC_MSG_REPORT_PHYSICAL_ADDRESS: // <Report physical address>
            {
               if (B_SrcLogAddr < 15)
               {
                  a_CECDevices[B_SrcLogAddr].PhysicalAddr = *((WORD*)(Ba_Data+1));
                  //a_CECDevices[B_SrcLogAddr].PhysicalAddr[1] = Ba_Data[2];
                  Ba_Data[0] = CEC_MSG_GIVE_OSD_NAME;                         // Opcode
                  gm_CecMngrTxMessage(B_SrcLogAddr, 1, &(Ba_Data[0]), 0, 0);
               }
            }
            break;
#endif
#ifdef CEC_SET_OSD_NAME_CAPABLE
         case CEC_MSG_SET_OSD_NAME: // <Report device name>
            {
               //_fmemset((void far*)(Ba_CurrentDeviceName), ' ', DEVICENAMELENGTH);
               if (UserPrefCecDeviceType == B_SrcLogAddr)
               {
                  // _fmemcpy((void far*)(Ba_CurrentDeviceName), (void far*)(Ba_Data + 1), (B_DataLen - 1));
                  //OSDRequestMsg(gmvb_MainChannel, MTO_CEC_NewDeviceDetected, REQUEST_NOW);
                  //OSDRequestMsg(gmvb_MainChannel, MTO_CEC_DevDataBaseChange, REQUEST_NOW);
                  Ba_Data[B_DataLen] = 0;
                  //PrintStatusInfo((const char *)"Dev. OSD name:", (const char *)Ba_Data+1, NULL);
                  OSDRequestMsg(gmvb_MainChannel, MTO_CEC_Text4UserReady, REQUEST_NOW);
                  //gm_CecAnswerReady = TRUE;
               }
            }
            break;
#endif

#ifdef CEC_GIVE_OSD_NAME_CAPABLE
         case CEC_MSG_GIVE_OSD_NAME:// <My name request>
            {
               _fstrcpy((char *)(&(Ba_Data[1])),(char*)("Sequoia"));
               Ba_Data[0] = CEC_MSG_SET_OSD_NAME;
               gm_CecMngrTxMessage(B_SrcLogAddr, 10, &(Ba_Data[0]), 0, 0);
            }
            break;
#endif
#ifdef CEC_CEC_VERSION_CAPABLE
         case CEC_MSG_CEC_VERSION: // <CEC version>
            {
               //a_CECDevices[B_SrcLogAddr].CecVersion = Ba_Data[1];
               //do nothing CEC version handled in special handler
               //this message used only for test purposes
            }
            break;
#endif

#ifdef CEC_GET_CEC_VERSION_CAPABLE
         case CEC_MSG_GET_CEC_VERSION: // <Get CEC version>
            {
               CecFeatureCecVersion(B_SrcLogAddr);
            }
            break;
#endif
         default:
            {
               //we received command which is not supported
               //so far we are sending feature abort
               //in the future we'll make special handling of
               //such default behavior.
               if (!B_IsBroadcastMsg)
               {
                  CecFeatureAbort(Ba_Data[0], CEC_AR_UNRECOGN_OPCODE, B_SrcLogAddr);
               }
            }
      }
   }
   return TRUE;
}

//CUSTOMER APPLICATION
void InitCECString(void)
{
   //Update pointers before displaying dynamic text
   UserPrefPtrSegment = FP_SEG(CECString);
   UserPrefPtrOffset = FP_OFF(CECString);
}

//LIBRARY
static const char far * CecVersionToStr(gmt_CEC_VERSIONS Version)
{
   switch (Version)
   {
      case CEC_VERSION_1_1 :
         return "CEC 1.1";
      case CEC_VERSION_1_2 :
         return "CEC 1.2";
      case CEC_VERSION_1_2a :
         return "CEC 1.2a";
      case CEC_VERSION_1_3 :
         return "CEC 1.3";
      case CEC_VERSION_1_3a :
         return "CEC 1.3a";
      default :
         return "Unknown CEC version";
   }
}

//CUSTOMER APPLICATION
void OsdToCecBridge(WORD command)
{
   //BYTE B_Parameters[USER_INTERFACE_PARAMETERS_COUNT];
   //BYTE B_Command;
   gmt_CecUserCommand UserCommand;

   UserCommand.BriefCommand.OpCode = (gmt_OsdControlCode)((command & OSD_CEC_COMMAND_MASK)>>8);
   UserCommand.BriefCommand.LogicalAddress = UserPrefCecDeviceType;
   switch (UserCommand.BriefCommand.OpCode)
   {
      case gmd_OCC_SetAnalogueTimer:
      case gmd_OCC_SetDigitalTimer:
      case gmd_OCC_SetExternalTimer:
         UserCommand.Data.DelayedRecord.TimeStamp.RecordDay = UserPrefCecRecordDateDay;
         UserCommand.Data.DelayedRecord.TimeStamp.RecordMonth = UserPrefCecRecordDateMonth;
         UserCommand.Data.DelayedRecord.TimeStamp.RecordHours = UserPrefCecRecordTimeHours;
         UserCommand.Data.DelayedRecord.TimeStamp.RecordMinutes = UserPrefCecRecordTimeMinutes;
         UserCommand.Data.DelayedRecord.TimeStamp.DurationHours = UserPrefCecRecordDurationHours;
         UserCommand.Data.DelayedRecord.TimeStamp.DurationMinutes = UserPrefCecRecordDurationMinutes;
         //UserCommand.Data.DelayedRecord.CurrentInput = UserPrefCurrentInputMain;
#if 0
         if (UserCommand.BriefCommand.OpCode == gmd_OCC_SetAnalogueTimer)
         {
            UserCommand.Data.DelayedRecord.Special.ChannelNumber = UserPrefTVChannelMain;
         }
         else
#endif
            if (UserCommand.BriefCommand.OpCode == gmd_OCC_SetExternalTimer)
            {
               //if (IsHDMIInput((gmt_UserDefConnNames)UserPrefCurrentInputMain))
               if (UserPrefCecDeviceSelectedFromHDMITree != 0)
               {
                  UserCommand.Data.DelayedRecord.InputSource = gmd_ExternalPhysicalAddress;
                  UserCommand.Data.DelayedRecord.Special.ExtPhysAddr = a_CECDevices[UserPrefCecDeviceSelectedFromHDMITree].PhysicalAddr;
               }
               else
               {
                  UserCommand.Data.DelayedRecord.InputSource = gmd_ExternalPlug;
                  UserCommand.Data.DelayedRecord.Special.ExtPlugNumber = UserPrefCecRecordOnExtPlugNumber;
               }
            }
         break;
      case gmd_OCC_SystemAudioModeRequest:
         if ((BYTE)(command & OSD_CEC_PARAMETER_MASK) == gmd_SAS_OFF)
            HandleSetSystemAudioStatus(gmd_SAS_OFF);
         //fall through!!!
      default:
         UserCommand.Data.Common.Parameters[0] = (BYTE)(command & OSD_CEC_PARAMETER_MASK);
         break;
   }
   if (!CecAppUserCommandHandlerRequest(&UserCommand))
   {
      OSDRequestMsg(gmvb_MainChannel, MTO_CEC_Command_Failed, REQUEST_NOW);
   }
}

//LIBRARY
static char nibble_to_hex(BYTE nibble)
{
   if (nibble < 10) return (nibble + '0');
   else return (nibble + 'A' - 10);
}

//LIBRARY
static char far * PhysicalAddressToStr(WORD PhAddr, char far * buffer)
{
   if (buffer == NULL) return NULL;
   buffer[0] = nibble_to_hex((PhAddr & 0x00F0) >> 4);
   buffer[1] = '.';
   buffer[2] = nibble_to_hex(PhAddr & 0x000F);
   buffer[3] = '.';
   buffer[4] = nibble_to_hex((PhAddr & 0xF000) >> 12);
   buffer[5] = '.';
   buffer[6] = nibble_to_hex((PhAddr & 0x0F00) >> 8);
   buffer[7] = 0;
   return buffer;
}

static const char far * StrCecDevices[]=
{
   "TV\r\n",
   "Recorder 1\r\n",
   "Recorder 2\r\n",
   "Tuner 1\r\n",
   "Player 1\r\n",
   "Audio\r\n",
   "Tuner 2\r\n",
   "Tuner 3\r\n",
   "Player 2\r\n",
   "Recorder 3\r\n",
   "Tuner 4\r\n",
   "Player 3\r\n",
   "Unknown\r\n",
   "Unknown\r\n",
   "Free use\r\n",
   "Switch\r\n"
};

static const char far * StrLogicalAddresses[] =
{
   "0  ",
   "1  ",
   "2  ",
   "3  ",
   "4  ",
   "5  ",
   "6  ",
   "7  ",
   "8  ",
   "9  ",
   "10 ",
   "11 ",
   "12 ",
   "13 ",
   "14 ",
   "15 "
};

static const char far * Spaces[]=
{
   "",
   " ",
   "  ",
   "   ",
   "    "
};

//LIBRARY
//Record status
static const char far * StrRecCurrentSource                 = "Recording current\r\nsource";
static const char far * StrRecDigitalService                = "Recording digital\r\nservice";
static const char far * StrRecAnalogueService               = "Recording analogue\r\nservice";
static const char far * StrRecExternalInput                 = "Recording external\r\ninput";
static const char far * StrNoRecording                      = "No recording :\r\n";
static const char far * StrNoRecDigitalService              = "Unable to record\r\ndigital service";
static const char far * StrNoRecAnalogueService             = "Unable to record\r\nanalogue service";
static const char far * StrNoRecUnableSelectRequiredService = "Unable to select\r\nrequired service";
static const char far * StrNoRecInvalidExtPlugNr            = "Invalid external\r\nplug number";
static const char far * StrNoRecInvalidExtPhysicalAddress   = "Invalid external\r\nphysical address";
static const char far * StrNoRecCASystemNotSupported        = "CA system not\r\nsupported";
static const char far * StrNoRecInsufficientCAEntitlements  = "No or insufficient\r\nCA elements";
static const char far * StrNoRecNotAllowedToCopySource      = "Not allowed to\r\ncopy source";
static const char far * StrNoRecNoFurtherCopiesAllowed      = "No further copies\r\nallowed";
static const char far * StrAlreadyRecording                 = "Already recording";
static const char far * StrNoRecMediaProtected              = "Media protected";
static const char far * StrNoRecNoSourceSignal              = "No source signal";
static const char far * StrNoRecMediaProblem                = "Media problem";
static const char far * StrNoRecNotEnoughSpace              = "Not enough space\r\navailable";
static const char far * StrNoRecParentalLock                = "Parental lock";
static const char far * StrRecordingTerminatedNormally      = "Recording terminated\r\nnormally";
static const char far * StrRecorlingHasAlreadyTerminated    = "Recording has al-\r\nready terminated";
static const char far * StrNoRecOtherReason                 = "Other reason";

//Deck status
static const char far * StrPlay                             = "Playing";
static const char far * StrRecord                           = "Recording";
static const char far * StrPlayReverse                      = "Play reverse";
static const char far * StrPlayStill                        = "Play still";
static const char far * StrPlaySlow                         = "Play slow";
static const char far * StrPlaySlowReverse                  = "Slow reverse";
static const char far * StrPlayFastForward                  = "Fast forward";
static const char far * StrPlayFastReverse                  = "Fast reverse";
static const char far * StrNoMedia                          = "No media";
static const char far * StrStop                             = "Stop";
static const char far * StrSkipForwardWind                  = "Skip forward or\r\nWind";
static const char far * StrSkipReverseRewind                = "Skip reverse or\r\nRewind";
static const char far * StrIndexSearchForward               = "Index search \r\nforward";
static const char far * StrIndexSearchReverse               = "Index search \r\nreverse";
static const char far * StrOtherStatus                      = "Other status";

//Power status
static const char far * StrPowerOn                          = "Power On";
static const char far * StrPowerOff                         = "Standby";
static const char far * StrTransitionStandbyToOn            = "In transition\r\nStandby to On";
static const char far * StrTransitionOnToStandby            = "In transition\r\nOn to Standby";

const char far * DeckStatusToStr(gmt_DeckInfo DeckStatus)
{
   switch (DeckStatus)
   {
      case gmd_DI_Play:
         return StrPlay;
      case gmd_DI_Record:
         return StrRecord;
      case gmd_DI_PlayReverse:
         return StrPlayReverse;
      case gmd_DI_Still:
         return StrPlayStill;
      case gmd_DI_Slow:
         return StrPlaySlow;
      case gmd_DI_SlowReverse:
         return StrPlaySlowReverse;
      case gmd_DI_FastForward:
         return StrPlayFastForward;
      case gmd_DI_FastReverse:
         return StrPlayFastReverse;
      case gmd_DI_NoMedia:
         return StrNoMedia;
      case gmd_DI_Stop:
         return StrStop;
      case gmd_DI_SkipForwardWind:
         return StrSkipForwardWind;
      case gmd_DI_SkipReverseRewind:
         return StrSkipReverseRewind;
      case gmd_DI_IndexSearchFwd:
         return StrIndexSearchForward;
      case gmd_DI_IndexSearchRev:
         return StrIndexSearchReverse;
      case gmd_DI_OtherStatus:
         return StrOtherStatus;
      default:
         return "Incorrect deck info";
   }
}

const char far * RecordStatusToStr(gmt_RecordStatusInfo RecordStatus)
{
   switch (RecordStatus)
   {
      case gmd_RSI_RecCurrentSource:
         return StrRecCurrentSource;
      case gmd_RSI_RecDigitalService:
         return StrRecDigitalService;
      case gmd_RSI_RecAnalogueService:
         return StrRecAnalogueService;
      case gmd_RSI_RecExternalInput:
         return StrRecExternalInput;
      case gmd_RSI_NoRecDigitalService:
         return StrNoRecDigitalService;
      case gmd_RSI_NoRecAnalogueService:
         return StrNoRecAnalogueService;
      case gmd_RSI_NoRecUnableSelectRequiredService:
         return StrNoRecUnableSelectRequiredService;
      case gmd_RSI_NoRecInvalidExtPlugNr:
         return StrNoRecInvalidExtPlugNr;
      case gmd_RSI_NoRecInvalidExtPhysicalAddress:
         return StrNoRecInvalidExtPhysicalAddress;
      case gmd_RSI_NoRecCASystemNotSupported:
         return StrNoRecCASystemNotSupported;
      case gmd_RSI_NoRecInsufficientCAEntitlements:
         return StrNoRecInsufficientCAEntitlements;
      case gmd_RSI_NoRecNotAllowedToCopySource:
         return StrNoRecNotAllowedToCopySource;
      case gmd_RSI_NoRecNoFurtherCopiesAllowed:
         return StrNoRecNoFurtherCopiesAllowed;
      case gmd_RSI_NoRecNoMedia:
         return StrNoMedia;
      case gmd_RSI_NoRecPlaying:
         return StrPlay;
      case gmd_RSI_NoRecAlreadyRecording:
         return StrAlreadyRecording;
      case gmd_RSI_NoRecMediaProtected:
         return StrNoRecMediaProtected;
      case gmd_RSI_NoRecNoSourceSignal:
         return StrNoRecNoSourceSignal;
      case gmd_RSI_NoRecMediaProblem:
         return StrNoRecMediaProblem;
      case gmd_RSI_NoRecNotEnoughSpace:
         return StrNoRecNotEnoughSpace;
      case gmd_RSI_NoRecParentalLock:
         return StrNoRecParentalLock;
      case gmd_RSI_RecordingTerminatedNormally:
         return StrRecordingTerminatedNormally;
      case gmd_RSI_RecorlingHasAlreadyTerminated:
         return StrRecorlingHasAlreadyTerminated;
      case gmd_RSI_NoRecOtherReason:
         return StrNoRecOtherReason;
      default:
         return "Incorrect record\r\nstatus info";
   }
}

const char far * PowerStatusToStr(gmt_PowerStatus PowerStatus)
{
   switch (PowerStatus)
   {
      case gmd_CecPowerStatusOn:
         return StrPowerOn;
      case gmd_CecPowerStatusStandby:
         return StrPowerOff;
      case gmd_CecPowerStatusTransitionStandbyToOn:
         return StrTransitionStandbyToOn;
      case gmd_CecPowerStatusTransitionOnToStandby:
         return StrTransitionOnToStandby;
      default:
         return "Unknown power status";
   }
}

void CecAppUserCommandResultsToOSDHandler(void)
{
   gmt_CecUserCommand Result;
   static char TemporaryOutputBuffer[16];

   if (CecAppUserCommandHandlerGetStatus() == gmd_UCS_complete)
   {
      CecAppUserCommandHandlerGetResults(&Result);
      CecAppUserCommandHandlerResetStatus();

      gm_Print("UCR::Read data: %x %x %x %x %x %x %x %x",
               Result.Data.Common.Parameters[0],
               Result.Data.Common.Parameters[1],
               Result.Data.Common.Parameters[2],
               Result.Data.Common.Parameters[3],
               Result.Data.Common.Parameters[4],
               Result.Data.Common.Parameters[5],
               Result.Data.Common.Parameters[6],
               Result.Data.Common.Parameters[7]);
      gm_Print("UCR::Read data: %x %x %x %x %x %x %x %x",
               Result.Data.Common.Parameters[8],
               Result.Data.Common.Parameters[9],
               Result.Data.Common.Parameters[10],
               Result.Data.Common.Parameters[11],
               Result.Data.Common.Parameters[12],
               Result.Data.Common.Parameters[13],
               Result.Data.Common.Parameters[14],
               Result.Data.Common.Parameters[15]);

      switch (Result.BriefCommand.AckResult)
      {
         case gmd_AR_Failed:
            ClearPrintOsdBuffer();
            PrintToOsd("\r\n\r\n\r\n");
            PrintToOsdHandler();
            gm_Print("MTO_CEC_Command_Failed",0);
            OSDRequestMsg(gmvb_MainChannel, MTO_CEC_Command_Failed, REQUEST_NOW);
            break;
         case gmd_AR_Ok_NotReplied:
            ClearPrintOsdBuffer();
            if (Result.BriefCommand.OpCode == gmd_OCC_Ping) PrintToOsd("Ping OK");
            else PrintToOsd("Status hasn't been\r\nreturned");
            PrintToOsdHandler();
            OSDRequestMsg(gmvb_MainChannel, MTO_CEC_Command_OK, REQUEST_NOW);
            gm_Print("MTO_CEC_Command_Succeed",0);
            break;
         case gmd_AR_Ok_Replied:
            switch (Result.BriefCommand.OpCode)
            {
               case CEC_MSG_TIMER_CLEARED_STATUS:
                  ClearPrintOsdBuffer();
                  switch (Result.Data.Common.Parameters[1])
                  {
                     case gmd_TCS_TIMER_NOT_CLEARED_RECORDING:
                        PrintToOsd("Not cleared\r\nRecording...");
                        break;
                     case gmd_TCS_TIMER_NOT_CLEARED_NO_MATCHING:
                        PrintToOsd("Not cleared\r\nNo matching block");
                        break;
                     case gmd_TCS_TIMER_NOT_CLEARED_NO_INFO_AVAILABLE:
                        PrintToOsd("Not cleared\r\nUnknown reason");
                        break;
                     case gmd_TCS_TIMER_CLEARED:
                        PrintToOsd("Cleared");
                        break;
                  }
                  PrintToOsdHandler();
                  break;
               case CEC_MSG_TIMER_STATUS:
                  ClearPrintOsdBuffer();
                  switch (Result.Data.Common.Parameters[1] & gmd_TSD_TIMER_OVERLAP_WARNING_MASK)
                  {
                     case gmd_TSD_NO_OVERLAP:
                        PrintToOsd("Block doesnt overlap\r\n");
                        break;
                     case gmd_TSD_TIMER_BLOCKS_OVERLAP:
                        PrintToOsd("Block overlaps\r\n");
                        break;
                  }
                  switch (Result.Data.Common.Parameters[1] & gmd_TSD_MEDIA_PRESENCE_MASK)
                  {
                     case gmd_TSD_MEDIA_PRESENT_AND_NOT_PROTECTED:
                        PrintToOsd("Media ok\r\n");
                        break;
                     case gmd_TSD_MEDIA_PRESENT_BUT_PROTECTED:
                        PrintToOsd("Media protected\r\n");
                        break;
                     case gmd_TSD_MEDIA_NOT_PRESENT:
                        PrintToOsd("Media not present\r\n");
                        break;
                  }
                  switch (Result.Data.Common.Parameters[1] & gmd_TSD_PROGRAMMED_MASK)
                  {
                     case gmd_TSD_NOT_PROGRAMMED:
                        PrintToOsd("Not programmed\r\n");
                        break;
                     case gmd_TSD_PROGRAMMED:
                        PrintToOsd("Programmed\r\n");
                        break;
                  }
                  PrintToOsdHandler();
                  break;
               case CEC_MSG_SYSTEM_AUDIO_MODE_STATUS:
               case CEC_MSG_SET_SYSTEM_AUDIO_MODE:
                  HandleSetSystemAudioStatus(Result.Data.Common.Parameters[1]);
                  break;
               case CEC_MSG_DECK_STATUS:
                  ClearPrintOsdBuffer();
                  PrintToOsd(DeckStatusToStr(Result.Data.Common.Parameters[1]));
                  PrintToOsdHandler();
                  break;
               case CEC_MSG_RECORD_STATUS:
                  ClearPrintOsdBuffer();
                  if (((Result.Data.Common.Parameters[1] >= gmd_RSI_NoRecDigitalService) &&
                        (Result.Data.Common.Parameters[1] <= gmd_RSI_NoRecParentalLock)      ) ||
                        (Result.Data.Common.Parameters[1] == gmd_RSI_NoRecOtherReason))
                     PrintToOsd(StrNoRecording);
                  PrintToOsd(RecordStatusToStr(Result.Data.Common.Parameters[1]));
                  PrintToOsdHandler();
                  break;
               case CEC_MSG_REPORT_POWER_STATUS:
                  ClearPrintOsdBuffer();
                  PrintToOsd(PowerStatusToStr(Result.Data.Common.Parameters[1]));
                  PrintToOsdHandler();
                  break;
               case CEC_MSG_REPORT_PHYSICAL_ADDRESS:
                  gm_Print("!Physical address = 0x%x", *((WORD*)(&Result.Data.Common.Parameters[1])));
                  PhysicalAddressToStr(*((WORD*)(&Result.Data.Common.Parameters[1])) , TemporaryOutputBuffer);
                  ClearPrintOsdBuffer();
                  PrintToOsd(TemporaryOutputBuffer);
                  PrintToOsdHandler();
                  break;
               case CEC_MSG_CEC_VERSION:
                  ClearPrintOsdBuffer();
                  PrintToOsd(CecVersionToStr(Result.Data.Common.Parameters[1]));
                  PrintToOsdHandler();
                  break;
               case CEC_MSG_SET_MENU_LANGUAGE:
                  ClearPrintOsdBuffer();
                  TemporaryOutputBuffer[0] = Result.Data.Common.Parameters[1];
                  TemporaryOutputBuffer[1] = Result.Data.Common.Parameters[2];
                  TemporaryOutputBuffer[2] = Result.Data.Common.Parameters[3];
                  TemporaryOutputBuffer[3] = 0;
                  PrintToOsd(TemporaryOutputBuffer);
                  PrintToOsdHandler();
                  break;
               case CEC_MSG_SET_OSD_NAME:
                  ClearPrintOsdBuffer();
                  _fmemcpy(TemporaryOutputBuffer, &Result.Data.Common.Parameters[1], Result.Data.Common.Length-1);
                  PrintToOsd("Device name:\r\n");
                  PrintToOsd(TemporaryOutputBuffer);
                  PrintToOsdHandler();
                  break;
               case CEC_MSG_FEATURE_ABORT:
                  ClearPrintOsdBuffer();
                  PrintToOsd("Device replied with\r\n'Feature abort' on\r\n'Abort'-test passed");
                  PrintToOsdHandler();
                  break;
            }
            break;
         case gmd_AR_Ok_Aborted:
            ClearPrintOsdBuffer();
            PrintToOsd("Replied with Feature\r\nabort probably it\r\nis not supported");
            PrintToOsdHandler();
            break;
      }
   }
}

//********************************************************************************************
//
//  Show OSD string handler
//
typedef struct
{
   gmt_OSDSTRINGCOMMAND OsdCommand;
   //BYTE *Ba_Str;
   //BYTE B_Len;
}gmt_ShowOsdStringHandlerInterface;

static gmt_ShowOsdStringHandlerInterface ShowOsdStringHandlerInterface = {gmd_NoCommand};

BOOL CecAppShowOsdStringHandlerRequest(gmt_OSDSTRINGCOMMAND OsdCommand, BYTE Str[],BYTE B_Len)
{
   if (ShowOsdStringHandlerInterface.OsdCommand == gmd_NoCommand)
   {
      //ShowOsdStringHandlerInterface.Ba_Str = Str;
      _fmemset((void far*)(CECString), ' ',sizeof(CECString));
      _fmemcpy((void far*)(CECString),
               (void far*)(Str),
               B_Len);

      //ShowOsdStringHandlerInterface.B_Len = B_Len;
      ShowOsdStringHandlerInterface.OsdCommand = OsdCommand;
      return gmd_TRUE;
   }
   else
   {
      return gmd_FALSE;
   }
}

//CUSTOMER APPLICATION
//void ShowOsdStringHandler(gmt_OSDSTRINGCOMMAND OsdCommand, BYTE Str[],BYTE B_Len)
void CecAppShowOsdStringHandler(void)
{
   static BYTE B_NeedHideOsd = gmd_FALSE;
   static WORD W_OsdShowTime;
   switch (ShowOsdStringHandlerInterface.OsdCommand)
   {
      case gmd_NoCommand:
         if (B_NeedHideOsd)
         {
            if ((WORD)gm_GetSystemTime()- W_OsdShowTime >= 5000)
            {
               OSDRequestMsg(gmvb_MainChannel, MTO_HideCECString, REQUEST_NOW);
               B_NeedHideOsd = gmd_FALSE;
            }
         }
         break;
      case gmd_ShowOsdStringDefaultTime:
         B_NeedHideOsd = gmd_TRUE;
         W_OsdShowTime = (WORD)gm_GetSystemTime();
         //fall through
      case gmd_ShowOsdString:
         OSDRequestMsg(gmvb_MainChannel, MTO_ShowCECString, REQUEST_NOW);
         break;
      case gmd_HideOsdString:
         OSDRequestMsg(gmvb_MainChannel, MTO_HideCECString, REQUEST_NOW);
         break;
      default :
         break;
   }
   ShowOsdStringHandlerInterface.OsdCommand = gmd_NoCommand;
}


//CUSTOMER APPLICATION
BOOL IsHDMIInput(gmt_UserDefConnNames Port)
{
   if ((Port == ALL_BOARDS_HDMI) || (Port == ALL_BOARDS_HDMI2))
   {
      return gmd_TRUE;
   }
   else
   {
      return gmd_FALSE;
   }
}

#if 1
//LIBRARY
//added to the library with name
//void gm_CecUtilReportPhysicalAddress(BYTE B_DestAddr, BYTE B_Type);
void ReportPhysicalAddress(BYTE B_DestAddr)
{
   BYTE Ba_Data[5];
   Ba_Data[0] = CEC_MSG_REPORT_PHYSICAL_ADDRESS;       // Opcode
   // TO DO
   // generally physical address and device type should be taken from database
   Ba_Data[1] = 0;
   Ba_Data[2] = 0;
   Ba_Data[3] = 0;
   gm_CecMngrTxMessage(B_DestAddr, 4, &(Ba_Data[0]), 0, 0);
}
#endif


#if 1
//LIBRARY
// added to the library with name
// void gm_CecUtilRequestSystemAudioStatus(void)
void RequestSystemAudioStatus(void)
{
   BYTE Ba_Data[2];
   Ba_Data[0] = CEC_MSG_GIVE_SYSTEM_AUDIO_MODE_STATUS;
   gm_CecMngrTxMessage(gmd_AudioSystem, 1, &(Ba_Data[0]), 0, 0);
}
#endif


//CUSTOMER APPLICATION
WORD InputPortToPhysicalAddress(gmt_UserDefConnNames Port)
{
   WORD Result;
   switch (Port)
   {
      case ALL_BOARDS_HDMI:
         Result = 0x0010;            //Physical address = 1.0.0.0
         break;
      case ALL_BOARDS_HDMI2:
         Result = 0x0020;            //Physical address = 2.0.0.0
         break;
      default:
         Result = 0x0000;
   }
   return Result;
}


/*
TRUTH TABLE
===========

Current     Next        Other           RoutChg     ActiveSrc
non-HDMI    non-HDMI    ANY
non-HDMI    HDMI        ANY
HDMIx       non-HDMI    non-HDMI                    +
HDMIx       non-HDMI    HDMIx
HDMIx       non-HDMI    HDMIy                       +
HDMIx       HDMIy       HDMIx
HDMIx       HDMIy       HDMIy
HDMIx       HDMIy       non-HDMI        +

but really if both inputs are HDMIx and HDMIy then always x == y

so TRUTH TABLE will be:
Current     Next        Other           RoutChg     ActiveSrc
non-HDMI    non-HDMI    ANY
non-HDMI    HDMI        ANY
HDMIx       HDMIy       ANY             +
HDMI        non-HDMI    ANY                         +
*/
//CUSTOMER APPLICATION
void gm_CecProcessSourceChange(gmt_UserDefConnNames CurrentPort, gmt_UserDefConnNames NextPort, gmt_UserDefConnNames OtherInput)
{
   BYTE Ba_Data[5];
   BOOL NextIsHDMI = IsHDMIInput(NextPort);
   BOOL CurrentIsHDMI = IsHDMIInput(CurrentPort);
   //this "if" added to avoid routing change when
   //main = HDMI3 pip = HDMI3 main -> VGA  => NO ROUTING CHANGE
   //main = VGA   pip = HDMI  main -> HDMI => NO ROUTING CHANGE
//    if ((!(CurrentIsHDMI && (!NextIsHDMI) && IsHDMIInput(OtherInput))) &&
//        (!((!CurrentIsHDMI) && NextIsHDMI && IsHDMIInput(OtherInput))))
   //gm_Print("ProcessSourceChange current=%d next=%d other=%d",CurrentPort,NextPort,OtherInput);
   if (!((CurrentIsHDMI && (!NextIsHDMI) && IsHDMIInput(OtherInput)) ||
         ((!CurrentIsHDMI) && NextIsHDMI && IsHDMIInput(OtherInput))))
   {
      if ((CurrentIsHDMI || NextIsHDMI)) // && (NextPort != CurrentPort)
      {
         Ba_Data[0] = CEC_MSG_ROUTING_CHANGE;        // Opcode
         *((WORD*)&Ba_Data[1]) = InputPortToPhysicalAddress(CurrentPort); // Previous Phys.Addr
         *((WORD*)&Ba_Data[3]) = InputPortToPhysicalAddress(NextPort);    // Next Phys.Addr
         gm_CecMngrTxMessage(CEC_BROADCAST_LOG_ADDR, 5, &(Ba_Data[0]), 0, 0);
         //CecFeatureReportPowerStatus(CEC_BROADCAST_LOG_ADDR);
         if (NextIsHDMI)
         {
#if 0
            Ba_Data[0] = CEC_MSG_SET_STREAM_PATH;
            *((WORD*)&Ba_Data[1]) = InputPortToPhysicalAddress(NextPort);    // Next Phys.Addr
            gm_CecMngrTxMessage(CEC_BROADCAST_LOG_ADDR, 3, &(Ba_Data[0]), 0, 0);
            //CecFeatureReportPowerStatus(CEC_BROADCAST_LOG_ADDR);
#endif
         }
         else
         {
            if (CurrentIsHDMI)
            {
               CecFeatureActiveSource(CEC_BROADCAST_LOG_ADDR);
               //CecFeatureReportPowerStatus(CEC_BROADCAST_LOG_ADDR);
            }
         }
      }
   }
}


typedef enum
{
   gmd_MPS_None    = 0,
   gmd_MPS_Main    = BIT0,
   gmd_MPS_Pip     = BIT1
}gmt_MainPipSelect;

#define SOURCE_SWITCH_REACTION_DELAY_TIMEOUT_MS   3000

#if 1
//CUSTOMER APPLICATION
void CecSourceChangeHandler(void)
{
   static gmt_UserDefConnNames Main;
   static gmt_UserDefConnNames Pip;
   //static gmt_UserDefConnNames PrevMain;
   //static gmt_UserDefConnNames PrevPip;
   //static BYTE PipMode = 0xFF;
   static WORD W_MainTimer;
   static WORD W_PipTimer;
   static gmt_MainPipSelect MainPipSelect = gmd_MPS_None;

   //detecting swap
   if (UserPrefPipMode == 1)
   {
      if (((Main == UserPrefCurrentInputPip) && (Pip == UserPrefCurrentInputMain)) && (UserPrefCurrentInputMain != UserPrefCurrentInputPip))
      {
#if 0
         gm_Print("::: Swap detected",0);
#endif
         Main = UserPrefCurrentInputPip;
         Pip = UserPrefCurrentInputMain;
         return;
      }

      if (Pip != UserPrefCurrentInputPip)
      {
#if 0
         gm_Print("::: Pip timer started",0);
#endif
         W_PipTimer = (WORD)gm_GetSystemTime();
         MainPipSelect |= gmd_MPS_Pip;
         Pip = UserPrefCurrentInputPip;
      }

      if (((WORD)gm_GetSystemTime() - W_PipTimer >= SOURCE_SWITCH_REACTION_DELAY_TIMEOUT_MS) &&
            ((MainPipSelect & gmd_MPS_Pip) != 0))
      {
#if 0
         gm_Print("::: Pip change detected",0);
#endif
         gm_CecProcessSourceChange(UserPrefPrevPipSource, UserPrefCurrentInputPip, UserPrefCurrentInputMain);
         MainPipSelect &= ~gmd_MPS_Pip;
         //PrevPip = UserPrefCurrentInputPip;
      }
   }

   if (Main != UserPrefCurrentInputMain)
   {
#if 0
      gm_Print("::: Main timer started",0);
#endif
      W_MainTimer = (WORD)gm_GetSystemTime();
      MainPipSelect |= gmd_MPS_Main;
      Main = UserPrefCurrentInputMain;
   }

   //if timeout expired then perform reaction on change of source
   if (((WORD)gm_GetSystemTime() - W_MainTimer >= SOURCE_SWITCH_REACTION_DELAY_TIMEOUT_MS) &&
         ((MainPipSelect & gmd_MPS_Main) != 0))
   {
#if 0
      gm_Print("::: Main change detected",0);
#endif
      gm_CecProcessSourceChange(UserPrefPreviousInputMain, UserPrefCurrentInputMain, UserPrefCurrentInputPip);
      MainPipSelect &= ~gmd_MPS_Main;
      //PrevMain = UserPrefCurrentInputMain;
   }


}
#endif

#ifdef PAL_TV_SYSTEM
extern BYTE currentPreset;
#endif

//CUSTOMER APPLICATION
void CecOsdHandler(void)
{
   CecAppUserCommandHandler();
   CecAppUserCommandResultsToOSDHandler();
   CecAppShowOsdStringHandler();
}

//CUSTOMER APPLICATION
#ifdef CEC_GET_MENU_LANGUAGE_CAPABLE
gmt_CEC_LANGUAGE_INDEX CecGetLanguageIdx(BYTE OSDLanguage)
{
   switch (OSDLanguage)
   {
      case 1:
         return CEC_ENG_LANGUAGE;
   }

   return CEC_ENG_LANGUAGE;
}
#endif

#ifdef CEC_SET_MENU_LANGUAGE_CAPABLE
gmt_RET_STAT CecGetLanguage(char *Bp_Data, BYTE *Bp_Language)
{
   BYTE B_LangIdx = CEC_MAX_LANGUAGE;
   do
   {
      B_LangIdx--;

      if (_fstrcmp(Bp_Data, (char ROM*)Cec_Language_codes[B_LangIdx]) == 0)
      {
         *Bp_Language = B_LangIdx;
         return gmd_TRUE;
      }
   }
   while (B_LangIdx);
   return gmd_FALSE;
}
#endif

#if 1
//added to library with name
// void gm_CecUtilSayCecVersion1_3(BYTE B_DestLogAddr)
#ifdef CEC_GET_CEC_VERSION_CAPABLE
void CecFeatureCecVersion(BYTE B_DestLogAddr)
{
   BYTE Ba_Data[2];

   // Send <CEC version> message
   Ba_Data[0] = CEC_MSG_CEC_VERSION;       // Opcode
   Ba_Data[1] = CEC_VERSION_1_3a;              // [Power Status]

   gm_CecMngrTxMessage(B_DestLogAddr, 2, &(Ba_Data[0]), 0, 0);
}
#endif
#endif

#if 1
//added to library with name
// void gm_CecUtilGetCecVersion(BYTE B_DestLogAddr)
#ifdef CEC_GET_CEC_VERSION_CAPABLE
void CecFeatureGetCecVersion(BYTE B_DestLogAddr)
{
   BYTE Ba_Data[2];

   // Send <CEC version> message
   Ba_Data[0] = CEC_MSG_GET_CEC_VERSION;       // Opcode

   gm_CecMngrTxMessage(B_DestLogAddr, 1, &(Ba_Data[0]), 0, 0);
}
#endif
#endif

#if 1
// added to library with name
// void gm_CecUtilGiveDevicePowerStatus(BYTE B_DestLogAddr)
#ifdef CEC_GIVE_DEVICE_POWER_STATUS_CAPABLE
void CecFeatureGiveDevicePowerStatus(BYTE B_DestLogAddr)
{
   BYTE Ba_Data[2];

   // Send <Give Device Power Status> message
   Ba_Data[0] = CEC_MSG_GIVE_DEVICE_POWER_STATUS;      // Opcode

   gm_CecMngrTxMessage(B_DestLogAddr, 1, &(Ba_Data[0]), 0, 0);
}
#endif
#endif

#if 1
// added to library with name
// void gm_CecUtilReportPowerStatus(BYTE B_DestLogAddr,BYTE B_PowerStatus)
#ifdef CEC_REPORT_POWER_STATUS_CAPABLE
void CecFeatureReportPowerStatus(BYTE B_DestLogAddr)
{
   BYTE Ba_Data[2];
   BYTE PowerStatus;

   if (UserPrefPowerState)
   {
      PowerStatus = gmd_CecPowerStatusOn;
   }
   else
   {
      PowerStatus = gmd_CecPowerStatusStandby;
   }

   // Send <Report Power Status> message
   Ba_Data[0] = CEC_MSG_REPORT_POWER_STATUS;       // Opcode
   Ba_Data[1] = PowerStatus;                       // [Power Status]

   gm_CecMngrTxMessage(B_DestLogAddr, 2, &(Ba_Data[0]), 0, 0);
}
#endif
#endif


#if 1
// added to library with name
// void gm_CecUtilSayIamActiveSource(BYTE B_DestLogAddr)
#ifdef CEC_ACTIVE_SOURCE_CAPABLE
void CecFeatureActiveSource(BYTE B_DestLogAddr)
{
   BYTE Ba_Data[3];

   // Send <Active Source> message
   Ba_Data[0] = CEC_MSG_ACTIVE_SOURCE;     // Opcode

   // TO DO : Physical Address
   Ba_Data[1] = 0x00;
   Ba_Data[2] = 0x00;

   gm_CecMngrTxMessage(B_DestLogAddr, 3, &(Ba_Data[0]), 0, 0);

   ActiveSourcePhisicalAddr = CEC_PHYSICAL_ADDRESS;
   ActiveSourceLogicalAddr = gm_CecGetLogAddr();
}
#endif
#endif

#if 1
// added to library with name
// void gm_CecUtilRequestActiveSource(BYTE B_DestLogAddr)
#ifdef CEC_REQUEST_ACTIVE_SOURCE_CAPABLE
void CecFeatureRequestActiveSource(BYTE B_DestLogAddr)
{
   BYTE Ba_Data[2];

   // Send <Request Active Source> message
   Ba_Data[0] = CEC_MSG_REQUEST_ACTIVE_SOURCE;     // Opcode

   gm_CecMngrTxMessage(B_DestLogAddr, 1, &(Ba_Data[0]), 0, 0);
}
#endif
#endif

#if 1
// added to library with name
// void gm_CecUtilFeatureAbort(BYTE B_DestLogAddr, BYTE B_FeatureOpCode, BYTE B_Feature
#ifdef CEC_FEATURE_ABORT_CAPABLE
static void CecFeatureAbort(BYTE B_FeatureOpCode, gmt_MNGR_CEC_ABORT_REASONS FeatureAbortReason, BYTE B_DestLogAddr)
{
   BYTE Ba_Data[3];
   Ba_Data[0] = CEC_MSG_FEATURE_ABORT;     // Opcode
   Ba_Data[1] = B_FeatureOpCode;             // [Feature Opcode]
   Ba_Data[2] = FeatureAbortReason;

   gm_CecMngrTxMessage(B_DestLogAddr, 3, &(Ba_Data[0]), 0, 0);

}
#endif
#endif

void  EXT_FUNC gm_SclCecPrint(char EXT_ROM *Bp_String, WORD W_Value)
{
   gm_Print(Bp_String, W_Value);
}

#ifdef LOW_POWER_MONITOR_SUPPORT
void CecCfgLpmHandler(void)
{
   if (B_CecCfgState == 4)
   {
      return;
   }

   CecLpmConfig(B_CecCfgState);

   B_CecCfgState++;
}

void CecLpmConfig(BYTE B_CecConfigState)
{
   switch (B_CecConfigState)
   {
      case 0:
         Ba_CecCfgData[0] = CEC_IPC_CONFIG_DATA;

         Ba_CecCfgData[1] = CEC_CFG_PH_ADDR;

         Ba_CecCfgData[2] = CEC_MSG_REPORT_PHYSICAL_ADDRESS;

         Ba_CecCfgData[3] = (CEC_PHYSICAL_ADDRESS) >> 0x8;
         Ba_CecCfgData[4] = (CEC_PHYSICAL_ADDRESS) & 0xFF;
         Ba_CecCfgData[5] = CEC_DEVICE_TYPE;

         Ba_CecCfgData[6] = CEC_MSG_SET_MENU_LANGUAGE;

         Ba_CecCfgData[7] = Cec_Language_codes[CecGetLanguageIdx(UserPrefOsdLanguage)][0];
         Ba_CecCfgData[8] = Cec_Language_codes[CecGetLanguageIdx(UserPrefOsdLanguage)][1];
         Ba_CecCfgData[9] = Cec_Language_codes[CecGetLanguageIdx(UserPrefOsdLanguage)][2];

         //gm_Print("UserPrefOsdLanguage %d",UserPrefOsdLanguage);

         gm_RegMsgSd(gmd_MESSAGE_CEC, Ba_CecCfgData, 10);
         //gm_Print("Cec Init area 1",0);
         break;

      case 1:
         Ba_CecCfgData[0] = CEC_IPC_CONFIG_DATA;

         Ba_CecCfgData[1] = CEC_CFG_VERSION;
         Ba_CecCfgData[2] = CEC_MSG_CEC_VERSION;
         Ba_CecCfgData[3] = CEC_VERSION_1_3a;

         Ba_CecCfgData[4] = CEC_MSG_REPORT_POWER_STATUS;
         Ba_CecCfgData[5] = UserPrefPowerState;

         Ba_CecCfgData[6] = CEC_MSG_DEVICE_VENDOR_ID;
         Ba_CecCfgData[7] = 0x77;
         Ba_CecCfgData[8] = 0x77;
         Ba_CecCfgData[9] = 0x77;

         gm_RegMsgSd(gmd_MESSAGE_CEC, Ba_CecCfgData, 10);

         //gm_Print("Cec Init area 2",0);
         break;

      case 2:
         Ba_CecCfgData[0] = CEC_IPC_CONFIG_DATA;

         Ba_CecCfgData[1] = 16;// CEC_CFG_OSD_NAME;
         Ba_CecCfgData[2] = CEC_MSG_SET_OSD_NAME;

         _fstrcpy((char *)(&(Ba_CecCfgData[3])),(char*)("Athena"));
         //memcpy(&(Ba_CecCfgData[3]),(char*)("Sequoia"),sizeof("Sequoia"));

         gm_RegMsgSd(gmd_MESSAGE_CEC, Ba_CecCfgData, 9);

         //gm_Print("Cec Init area 3",0);
         break;

      case 3:
         Ba_CecCfgData[8] =  CEC_IPC_CONFIG_DATA;

         Ba_CecCfgData[9]  = CEC_CFG_OSD_NAME + 7;
         Ba_CecCfgData[16] = sizeof("Athena") + 1;
         //Ba_CecCfgData[17] = 5;

         gm_RegMsgSd(gmd_MESSAGE_CEC, (BYTE*)&Ba_CecCfgData[8], 9);

         //gm_Print("Cec Init area 4",0);
         break;

      default:
         //gm_Print("Cec Init area X",0);
         break;
   }
}
#endif

// added to library with name
// gm_CecUtilEnumerateCecDevicesWithinSameType
void SwitchLogicalAddresses(BYTE increment, gmt_DeviceType B_DeviceType)
{
   gm_Print("Old device = %d", UserPrefCecDeviceType);
   switch (UserPrefCecDeviceType)
   {
      case gmd_Recorder1:
         if (increment) UserPrefCecDeviceType = gmd_Recorder2;
         else UserPrefCecDeviceType = gmd_Unregistered;
         break;
      case gmd_Recorder2:
         if (increment) UserPrefCecDeviceType = gmd_Recorder3;
         else UserPrefCecDeviceType = gmd_Recorder1;
         break;
      case gmd_Recorder3:
         if (increment) UserPrefCecDeviceType = gmd_FreeUse;
         else UserPrefCecDeviceType = gmd_Recorder2;
         break;

      case gmd_Player1:
         if (increment) UserPrefCecDeviceType = gmd_Player2;
         else UserPrefCecDeviceType = gmd_Unregistered;
         break;
      case gmd_Player2:
         if (increment) UserPrefCecDeviceType = gmd_Player3;
         else UserPrefCecDeviceType = gmd_Player1;
         break;
      case gmd_Player3:
         if (increment) UserPrefCecDeviceType = gmd_FreeUse;
         else UserPrefCecDeviceType = gmd_Player2;
         break;

      case gmd_Tuner1:
         if (increment) UserPrefCecDeviceType = gmd_Tuner2;
         else UserPrefCecDeviceType = gmd_Unregistered;
         break;
      case gmd_Tuner2:
         if (increment) UserPrefCecDeviceType = gmd_Tuner3;
         else UserPrefCecDeviceType = gmd_Tuner1;
         break;
      case gmd_Tuner3:
         if (increment) UserPrefCecDeviceType = gmd_Tuner4;
         else UserPrefCecDeviceType = gmd_Tuner2;
         break;
      case gmd_Tuner4:
         if (increment) UserPrefCecDeviceType = gmd_FreeUse;
         else UserPrefCecDeviceType = gmd_Tuner3;
         break;

      case gmd_AudioSystem:
         if (increment) UserPrefCecDeviceType = gmd_FreeUse;
         else UserPrefCecDeviceType = gmd_Unregistered;
         break;

      case gmd_FreeUse:
         if (increment) UserPrefCecDeviceType = gmd_Unregistered;
         else
            switch (B_DeviceType)
            {
               case gmd_Recorder1:
               case gmd_Recorder2:
               case gmd_Recorder3:
                  UserPrefCecDeviceType = gmd_Recorder3;
                  break;
               case gmd_Player1:
               case gmd_Player2:
               case gmd_Player3:
                  UserPrefCecDeviceType = gmd_Player3;
                  break;
               case gmd_Tuner1:
               case gmd_Tuner2:
               case gmd_Tuner3:
               case gmd_Tuner4:
                  UserPrefCecDeviceType = gmd_Tuner4;
                  break;
               case gmd_AudioSystem:
                  UserPrefCecDeviceType = gmd_AudioSystem;
                  break;
            }
         break;

      case gmd_Unregistered:
         if (increment)
            switch (B_DeviceType)
            {
               case gmd_Recorder1:
               case gmd_Recorder2:
               case gmd_Recorder3:
                  UserPrefCecDeviceType = gmd_Recorder1;
                  break;
               case gmd_Player1:
               case gmd_Player2:
               case gmd_Player3:
                  UserPrefCecDeviceType = gmd_Player1;
                  break;
               case gmd_Tuner1:
               case gmd_Tuner2:
               case gmd_Tuner3:
               case gmd_Tuner4:
                  UserPrefCecDeviceType = gmd_Tuner1;
                  break;
               case gmd_AudioSystem:
                  UserPrefCecDeviceType = gmd_AudioSystem;
                  break;
            }
         else UserPrefCecDeviceType = gmd_FreeUse;
         break;
   }
   gm_Print("New device = %d", UserPrefCecDeviceType);
}

// this function is intended
// for switching sources in CEC control dialog
void SwitchMainSources(void)
{
   gm_Print("Old source =%d", UserPrefCurrentInputMain);
   switch (UserPrefCurrentInputMain)
   {
      case ALL_BOARDS_HDMI :
         UserPrefCurrentInputMain = ALL_BOARDS_HDMI2;
         break;
      case ALL_BOARDS_HDMI2:
         UserPrefCurrentInputMain = ALL_BOARDS_CVBS1;
         break;
      case ALL_BOARDS_CVBS1:
         UserPrefCurrentInputMain = ALL_BOARDS_SVIDEO1;
         break;
      case ALL_BOARDS_SVIDEO1:
         UserPrefCurrentInputMain = ALL_BOARDS_COMP1;
         break;
      case ALL_BOARDS_COMP1:
         UserPrefCurrentInputMain = ALL_BOARDS_VGA1;
         break;
      case ALL_BOARDS_VGA1:
         UserPrefCurrentInputMain = ALL_BOARDS_HDMI;
         break;
      default:
         UserPrefCurrentInputMain = ALL_BOARDS_HDMI;
   }
   AdjustCurrentInputMain();
   gm_Print("New source =%d", UserPrefCurrentInputMain);
}

BYTE PhysicalAddrLength(WORD W_PhysicalAddress)
{
   if (W_PhysicalAddress == 0) return 0;
   else if ((W_PhysicalAddress & 0xFF0F) == 0) return 1;
   else if ((W_PhysicalAddress & 0xFF00) == 0) return 2;
   else if ((W_PhysicalAddress & 0x0F00) == 0) return 3;
   else return 4;
}

BOOL PhysAddrPrefixesEqual(WORD W_PhAddr1, WORD W_PhAddr2, BYTE B_PrefixLen)
{
   switch (B_PrefixLen)
   {
      case 0:
         return gmd_TRUE;
      case 1:
         return ((W_PhAddr1 & 0x00F0) == (W_PhAddr2 & 0x00F0));
      case 2:
         return ((W_PhAddr1 & 0x00FF) == (W_PhAddr2 & 0x00FF));
      case 3:
         return ((W_PhAddr1 & 0xF0FF) == (W_PhAddr2 & 0xF0FF));
      case 4:
         return (W_PhAddr1  == W_PhAddr2);
      default :
         return gmd_FALSE;
   }
}

WORD CutPhysicalAddressToLen(WORD W_PhAddr, BYTE B_NewLen)
{
   switch (B_NewLen)
   {
      case 0:
         return 0;
      case 1:
         return (W_PhAddr & 0x00F0);
      case 2:
         return (W_PhAddr & 0x00FF);
      case 3:
         return (W_PhAddr & 0xF0FF);
      case 4:
      default:
         return W_PhAddr;
   }
}


//returns number of child devices
BYTE NumberOfDescendants(WORD NodePhisicalAddress, WORD *pW_DescendantsMask, WORD *pW_ChildrenMask, BOOL MarkDescendants)
{
   BYTE i;
   BYTE count = 0;
   BYTE NodePhAddrLen;
   BYTE TmpPhAddrLen;

   NodePhAddrLen = PhysicalAddrLength(NodePhisicalAddress);
   if (pW_ChildrenMask != NULL)
   {
      *pW_ChildrenMask = 0;
   }
   if (pW_DescendantsMask != NULL)
   {
      if (MarkDescendants) *pW_DescendantsMask = 0;
   }
   for (i = 0x01; i < 0x0F; i++)
   {
      TmpPhAddrLen = PhysicalAddrLength(a_CECDevices[i].PhysicalAddr);
      //if ((TmpPhAddrLen > 0) &&
      //    (PhysAddrPrefixesEqual(NodePhisicalAddress, a_CECDevices[i].PhysicalAddr, NodePhAddrLen)))
      if ((TmpPhAddrLen > NodePhAddrLen) &&
            (PhysAddrPrefixesEqual(NodePhisicalAddress, a_CECDevices[i].PhysicalAddr, NodePhAddrLen)))
      {
         count++;
         if (pW_DescendantsMask != NULL)
         {
            if (MarkDescendants) *pW_DescendantsMask |= (0x0001U << i);
            else *pW_DescendantsMask &= (~(0x0001U << i));
         }
         if (pW_ChildrenMask != NULL)
         {
            if (TmpPhAddrLen - NodePhAddrLen == 1)
               *pW_ChildrenMask |= (0x0001U << i);
         }
      }
      if ((NodePhisicalAddress == a_CECDevices[i].PhysicalAddr) && (MarkDescendants == FALSE))
         if (pW_DescendantsMask != NULL)
         {
            *pW_DescendantsMask &= (~(0x0001U << i));
         }
   }
   return count;
}


static BYTE GetLogicalAddr(WORD W_PhysicalAddress)
{
   BYTE i;
   for (i = 0; i < 0x0F; i++)
   {
      if (a_CECDevices[i].PhysicalAddr == W_PhysicalAddress) return i;
   }
   return 15;
}

static const char far * StrDeviceWithPhysicalAddress(WORD W_PhysicalAddress)
{
   BYTE i;
   for (i = 0; i < 0x0F; i++)
   {
      if (a_CECDevices[i].PhysicalAddr == W_PhysicalAddress) return StrCecDevices[i];
   }
   return "Switch\r\n";
}

// Warning!!!
// Recursive algorithm !!!
// Potential stack overflow
// One nested call takes 7 bytes of data stack + 4 bytes of returns stack
// The depth of recursion restricted with 4
#define PRINT_BUFFER_LENGTH 15
void PrintHDMISubTree(WORD NodePhysicalAddress)
{
   BYTE B_ChildId;
   WORD W_ChildrenMask;
   WORD W_DescendantsMask;
   WORD W_tmpChildMask;
   static BYTE B_NodeLA;
   NumberOfDescendants(NodePhysicalAddress, &W_DescendantsMask, &W_ChildrenMask, gmd_TRUE);
   //_fmemset(printbuffer,' ',PRINT_BUFFER_LENGTH);
   //PhysicalAddressToStr(NodePhysicalAddress, printbuffer+PhysicalAddrLength(NodePhysicalAddress));
   //gm_Print(printbuffer,0);
   B_NodeLA = GetLogicalAddr(NodePhysicalAddress);
   PrintToOsd(StrLogicalAddresses[B_NodeLA]);
   PrintToOsd(Spaces[PhysicalAddrLength(NodePhysicalAddress)]);
   //PrintToOsd(StrDeviceWithPhysicalAddress(NodePhysicalAddress));
   PrintToOsd(StrCecDevices[B_NodeLA]);

   //gm_Print(StrCecDevices[NodeId],0);
   if (W_ChildrenMask > 0)
   {
      for (W_tmpChildMask = 0x0001U, B_ChildId = 0;
            W_tmpChildMask > 0;
            W_tmpChildMask <<= 1, B_ChildId++)
      {
         if (W_ChildrenMask & W_tmpChildMask)
         {
            PrintHDMISubTree(a_CECDevices[B_ChildId].PhysicalAddr);
            NumberOfDescendants(a_CECDevices[B_ChildId].PhysicalAddr, &W_DescendantsMask, NULL, FALSE);
         }
      }
   }
   if (W_DescendantsMask > 0)
   {
      WORD W_SwitchPhisicalAddr;

      for (W_tmpChildMask = 0x0001U, B_ChildId = 0;
            W_tmpChildMask > 0;
            W_tmpChildMask <<= 1, B_ChildId++)
      {
         if (W_DescendantsMask & W_tmpChildMask)
         {
            W_SwitchPhisicalAddr = CutPhysicalAddressToLen(a_CECDevices[B_ChildId].PhysicalAddr,
                                   PhysicalAddrLength(NodePhysicalAddress)+1);

            PrintHDMISubTree(W_SwitchPhisicalAddr);
            NumberOfDescendants(W_SwitchPhisicalAddr, &W_DescendantsMask, NULL, FALSE);
         }
      }
   }
}

void PrintHDMITree(void)
{
   /*
       a_CECDevices[1].PhysicalAddr = PHYS_ADDR_TO_WORD(1, 0, 0, 0);
       a_CECDevices[2].PhysicalAddr = PHYS_ADDR_TO_WORD(2, 0, 0, 0);
       a_CECDevices[3].PhysicalAddr = PHYS_ADDR_TO_WORD(1, 1, 0, 0);
       a_CECDevices[4].PhysicalAddr = PHYS_ADDR_TO_WORD(1, 2, 0, 0);
       a_CECDevices[5].PhysicalAddr = PHYS_ADDR_TO_WORD(1, 3, 0, 0);
       a_CECDevices[6].PhysicalAddr = PHYS_ADDR_TO_WORD(3, 1, 1, 1);
       a_CECDevices[7].PhysicalAddr = PHYS_ADDR_TO_WORD(3, 1, 1, 2);
       a_CECDevices[8].PhysicalAddr = PHYS_ADDR_TO_WORD(3, 1, 1, 3);
       a_CECDevices[9].PhysicalAddr = PHYS_ADDR_TO_WORD(1, 1, 1, 4);
   */
   ClearPrintOsdBuffer();
   PrintHDMISubTree(0);
   PrintToOsdHandler();
}


typedef enum
{
   gmd_VBS_Idle,
   gmd_VBS_VolUpPressed,
   gmd_VBS_VolDnPressed,
}gmt_VolumeButtonsStates;

typedef enum
{
   gmd_VCM_Inactive,
   gmd_VCM_Active
}gmt_VolumeCtrlMode;

//timeout in ms which defines how long after last keypress we are requesting
#define VOLUME_CTRL_MODE_ACTIVE_TIMEOUT 1000
#define AUDIO_STATUS_REQUEST_TIMEOUT 500
#define KEY_AUTO_RELEASE_TIMEOUT 480

gmt_SystemAudioCommand SystemAudioHandlerInterface = gmd_SAC_NoneCommand;

BOOL CecAppSystemAudioHandlerRequest(gmt_SystemAudioCommand Command)
{
   if (SystemAudioHandlerInterface == gmd_SAC_NoneCommand)
   {
      SystemAudioHandlerInterface = Command;
      return TRUE;
   }
   else
   {
      return FALSE;
   }
}


void CecAppSystemAudioHandler(void)
{
   gmt_SystemAudioCommand Command;
   static gmt_VolumeButtonsStates State;
   static gmt_VolumeCtrlMode VolumeCtrlMode = gmd_VCM_Inactive;
   static WORD LastVolumeRequestTime;
   static WORD LastKeyPressTime;
   static WORD EnterKeyPressedStateTime;
   BYTE Ba_CecMsg[3];
   BYTE B_Len;
   gmt_CEC_MNGR_MESSAGE_STATUS FrameStatus;

   Command = SystemAudioHandlerInterface;
   SystemAudioHandlerInterface = gmd_SAC_NoneCommand;

   if (UserPrefCecSystemAudioStatus == gmd_SAS_ON)
   {
      // Are we continueing work with volume ?
      switch (Command)
      {
         case gmd_SAC_VolDnPress:
         case gmd_SAC_VolUpPress:
         case gmd_SAC_VolDnRelease:
         case gmd_SAC_VolUpRelease:
            LastKeyPressTime = (WORD) gm_GetSystemTime();
            VolumeCtrlMode = gmd_VCM_Active;
            break;
         default:
            if ((WORD) gm_GetSystemTime() - LastKeyPressTime > VOLUME_CTRL_MODE_ACTIVE_TIMEOUT)
            {
               VolumeCtrlMode = gmd_VCM_Inactive;
            }
            break;
      }

      // State machine for requesting current volume level
      // if user is changing volume at this moment it is necessary to request
      // volume level from receiver periodically
      if (((VolumeCtrlMode == gmd_VCM_Active) &&
            ((WORD)gm_GetSystemTime() - LastVolumeRequestTime > AUDIO_STATUS_REQUEST_TIMEOUT)) ||
            (Command == gmd_SAC_RequestVolume))
      {
         //Ba_CecMsg[0] = CEC_MSG_GIVE_AUDIO_STATUS;
         //gm_CecMngrTxMessage(gmd_AudioSystem, 1, Ba_CecMsg, 0, 0);
         RequestAudioReceiverAudioStatus();
         LastVolumeRequestTime = (WORD)gm_GetSystemTime();
      }

      // handling keys press and release
      switch (State)
      {
         case gmd_VBS_Idle:
            switch (Command)
            {
               case gmd_SAC_VolUpPress:
                  State = gmd_VBS_VolUpPressed;
                  EnterKeyPressedStateTime = (WORD) gm_GetSystemTime();
                  Ba_CecMsg[0] = CEC_MSG_USER_CONTROL_PRESSED;
                  Ba_CecMsg[1] = gmd_UCCVolumeUp;
                  gm_CecMngrTxMessage(gmd_AudioSystem, 2, Ba_CecMsg, 0, 0);
                  break;
               case gmd_SAC_VolDnPress:
                  State = gmd_VBS_VolDnPressed;
                  EnterKeyPressedStateTime = (WORD) gm_GetSystemTime();
                  Ba_CecMsg[0] = CEC_MSG_USER_CONTROL_PRESSED;
                  Ba_CecMsg[1] = gmd_UCCVolumeDn;
                  gm_CecMngrTxMessage(gmd_AudioSystem, 2, Ba_CecMsg, 0, 0);
                  break;
               case gmd_SAC_ToggleMute:
                  Ba_CecMsg[0] = CEC_MSG_USER_CONTROL_PRESSED;
                  Ba_CecMsg[1] = gmd_UCCMute;
                  gm_CecMngrTxMessage(gmd_AudioSystem, 2, Ba_CecMsg, 0, 0);
                  Ba_CecMsg[0] = CEC_MSG_USER_CONTROL_RELEASED;
                  Ba_CecMsg[1] = gmd_UCCMute;
                  gm_CecMngrTxMessage(gmd_AudioSystem, 2, Ba_CecMsg, 0, 0);
                  RequestAudioReceiverAudioStatus();
                  LastVolumeRequestTime = (WORD)gm_GetSystemTime();
                  break;
               default:
                  break;
            }
            break;
         case gmd_VBS_VolDnPressed:
            if ((Command == gmd_SAC_VolDnRelease) ||
                  ((WORD)gm_GetSystemTime() - EnterKeyPressedStateTime > KEY_AUTO_RELEASE_TIMEOUT))
            {
               Ba_CecMsg[0] = CEC_MSG_USER_CONTROL_RELEASED;
               Ba_CecMsg[1] = gmd_UCCVolumeDn;
               gm_CecMngrTxMessage(gmd_AudioSystem, 2, Ba_CecMsg, 0, 0);
               State = gmd_VBS_Idle;
            }
            break;
         case gmd_VBS_VolUpPressed:
            if ((Command == gmd_SAC_VolUpRelease) ||
                  ((WORD)gm_GetSystemTime() - EnterKeyPressedStateTime > KEY_AUTO_RELEASE_TIMEOUT))
            {
               Ba_CecMsg[0] = CEC_MSG_USER_CONTROL_RELEASED;
               Ba_CecMsg[1] = gmd_UCCVolumeUp;
               gm_CecMngrTxMessage(gmd_AudioSystem, 2, Ba_CecMsg, 0, 0);
               State = gmd_VBS_Idle;
            }
            break;
      }
   }
}

#elif (FEATURE_CEC == DISABLE)

#pragma warn -par

void PrintToOsdHandler(void)
{
   //do nothing - for compatibility with older chips
}

void OsdToCecBridge(WORD command)
{
   //do nothing - for compatibility with older chips
}

void SwitchLogicalAddresses(BYTE increment, gmt_DeviceType B_DeviceType)
{

}

void InitCECString(void)
{
   //do nothing - for compatibility with older chips
}

void SwitchMainSources(void)
{
   //do nothing - for compatibility with older chips
}

void PrintHDMITree(void)
{
   //do nothing - for compatibility with older chips
}

void CecAppSystemAudioHandler(void)
{

}

#pragma warn .par

#endif //#if (FEATURE_CEC == ENABLE)

///////////////////////////////////////////////////////////////////////////////
#ifdef RC_VIA_RC_PATH_THROUGH
if (gm_ButtonState == gmd_ButtonPressed)
   Ba_Data[0] = CEC_MSG_USER_CONTROL_PRESSED;
else
   Ba_Data[0] = CEC_MSG_USER_CONTROL_RELEASED;
Ba_Data[1] = gm_OsdToCecCommand;
B_MsgId = gm_CecMngrTxMessage(B_DestAddr, 2, &(Ba_Data[0]), 1, 0);
State = gmd_CecOsdWaitAckState;

gm_OsdToCecCommand = gmd_OsdCecIdle;
break;
#endif


