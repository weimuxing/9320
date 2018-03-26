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
// MODULE:      CecFeatures.h
//
// USAGE:       Header file for CEC features
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "Board\gm_InputConfig.h"		//
#include "PowerHandler.h"

//******************************************************************************
//  G L O B A L  T Y P E S  D E F I N I T I O N S
//******************************************************************************

typedef enum
{
   ReqPwrOn        = Pwr_On,			//normal functioning (syncs on both VS and HS)
   ReqPwrSleep     = Pwr_Sleep,
   ReqPwrDeepSleep = Pwr_DeepSleep,	// syncs missing both VS and HS
   ReqPwrOff       = Pwr_Off,			// soft power off (no sync monitoring, only power key can wake up system)
   ReqNone         = 0xFF
}gmt_PwrCecRequestType;

//Cec Messages Opcodes
typedef enum
{
   CEC_MSG_FEATURE_ABORT	                    = 0x00,  // <Feature Abort> message
   CEC_MSG_IMAGE_VIEW_ON	                    = 0x04,  // <Image View On> message
   CEC_MSG_TUNER_STEP_INCREMENT                = 0x05,  // <Tuner Step Increment> message
   CEC_MSG_TUNER_STEP_DECREMENT                = 0x06,  // <Tuner Step Decrement> message
   CEC_MSG_TUNER_DEVICE_STATUS                 = 0x07,  // <Tuner Device Status>
   CEC_MST_GIVE_TUNER_DEVICE_STATUS            = 0X08,
   CEC_MSG_RECORD_ON                           = 0x09,  // <Record On> message
   CEC_MSG_RECORD_STATUS                       = 0x0A,
   CEC_MSG_RECORD_OFF                          = 0x0B,
   CEC_MSG_TEXT_VIEW_ON                        = 0x0D,  // <Text View On> message
   CEC_MSG_RECORD_TV_SCREEN                    = 0x0F,
   CEC_MSG_GIVE_DECK_STATUS                    = 0x1A,  // <Give Deck Status> message
   CEC_MSG_DECK_STATUS                         = 0x1B,
   CEC_MSG_SET_MENU_LANGUAGE                   = 0x32,  // <Set Menu Language>
   CEC_MSG_CLEAR_ANALOGUE_TIMER                = 0x33,  // <Clear analogue timer>
   CEC_MSG_SET_ANALOGUE_TIMER                  = 0x34,  // <Set analogue timer>
   CEC_MSG_TIMER_STATUS                        = 0x35,  // <Timer status>
   CEC_MSG_STANDBY                             = 0x36,  // <Standby> message
   CEC_MSG_PLAY                                = 0x41,
   CEC_MSG_DECK_CONTROL                        = 0x42,
   CEC_MSG_TIMER_CLEARED_STATUS                = 0x43,  // <Timer cleared status>
   CEC_MSG_USER_CONTROL_PRESSED                = 0x44,
   CEC_MSG_USER_CONTROL_RELEASED               = 0x45,
   CEC_MSG_GIVE_OSD_NAME                       = 0x46,  // <Give OSD Name> message
   CEC_MSG_SET_OSD_NAME                        = 0x47,  // <Set OSD Name> message
   CEC_MSG_SET_OSD_STRING                      = 0x64,  // <Set OSD String> message
   CEC_MSG_SET_TIMER_PROGRAM_TITLE             = 0x67,  // <Set timer program title>
   CEC_MSG_SYSTEM_AUDIO_MODE_REQUEST           = 0x70,
   CEC_MSG_GIVE_AUDIO_STATUS                   = 0x71,  // <Give audio status>
   CEC_MSG_SET_SYSTEM_AUDIO_MODE               = 0x72,
   CEC_MSG_REPORT_AUDIO_STATUS                 = 0x7A,  // <Report Audio Status>
   CEC_MSG_GIVE_SYSTEM_AUDIO_MODE_STATUS       = 0x7D,  // <Give System Audio Mode Status>
   CEC_MSG_SYSTEM_AUDIO_MODE_STATUS            = 0x7E,  // <System Audio Mode Status>
   CEC_MSG_ROUTING_CHANGE                      = 0x80,  // <Routing Change> message
   CEC_MSG_ROUTING_INFORMATION                 = 0x81,  // <Routing information>
   CEC_MSG_ACTIVE_SOURCE                       = 0x82,  // <Active Source> message
   CEC_MSG_GIVE_PHYSICAL_ADDRESS               = 0x83,  // <Give Physical Address> message
   CEC_MSG_REPORT_PHYSICAL_ADDRESS             = 0x84,  // <Report physical address> message; this message is received when new device connect to network
   CEC_MSG_REQUEST_ACTIVE_SOURCE               = 0x85,  // <Request active source>
   CEC_MSG_SET_STREAM_PATH                     = 0x86,  // <Set Stream Path>
   CEC_MSG_DEVICE_VENDOR_ID                    = 0x87,  // <Device Vendor ID>
   CEC_MSG_VENDOR_COMMAND                      = 0x89,  // <Vendor Command>
   CEC_MSG_VENDOR_REMOTE_BUTTON_DOWN           = 0x8A,
   CEC_MSG_VENDOR_REMOTE_BUTTON_UP             = 0x8B,
   CEC_MSG_GIVE_DEVICE_VENDOR_ID               = 0x8C,
   CEC_MSG_MENU_REQUEST                        = 0x8D,
   CEC_MSG_MENU_STATUS                         = 0x8E,
   CEC_MSG_GIVE_DEVICE_POWER_STATUS            = 0x8F,  // <Give Device Power Status>
   CEC_MSG_REPORT_POWER_STATUS                 = 0x90,  // <Report Power Status>
   CEC_MSG_GET_MENU_LANGUAGE                   = 0x91,  // <Get Menu Language>
   CEC_MSG_SELECT_ANALOGUE_SERVICE             = 0x92,
   CEC_MSG_SELECT_DIGITAL_SERVICE              = 0x93,

   CEC_MSG_SET_DIGITAL_TIMER                   = 0x97,  // <Set Digital Timer>
   CEC_MSG_CLEAR_DIGITAL_TIMER                 = 0x99,  // <Clear digital timer>
   CEC_MSG_SET_AUDIO_RATE                      = 0x9A,
   CEC_MSG_INACTIVE_SOURCE                     = 0x9D,  // <Inactive Source>
   CEC_MSG_CEC_VERSION                         = 0x9E,  // <CEC version>
   CEC_MSG_GET_CEC_VERSION                     = 0x9F,  // <Get CEC version>
   CEC_MSG_VENDOR_COMMAND_WITH_ID              = 0xA0,
   CEC_MSG_CLEAR_EXTERNAL_TIMER                = 0xA1,  // <Clear external timer>
   CEC_MSG_SET_EXTERNAL_TIMER                  = 0xA2,  // <Set external timer>
   CEC_MSG_ABORT                               = 0xFF   // <Abort> message
} gmt_MNGR_CEC_FEATURES_ENUM;

typedef enum
{
   CEC_AR_UNRECOGN_OPCODE              		= 0,
   CEC_AR_INCORRECT_MODE               		= 1,
   CEC_AR_CANNOT_PROVIDE_SRC           		= 2,
   CEC_AR_INVALID_OPERAND              		= 3,
   CEC_AR_REFUSED                      		= 4
} gmt_MNGR_CEC_ABORT_REASONS;

typedef enum
{
   CEC_ENG_LANGUAGE                            = 0,
   //CEC_FRA_LANGUAGE,
   //CEC_FRE_LANGUAGE,
   //CEC_GER_LANGUAGE,
   //CEC_DEU_LANGUAGE,
   CEC_MAX_LANGUAGE
} gmt_CEC_LANGUAGE_INDEX;

//CEC versions
typedef enum
{
   CEC_VERSION_1_1 							= 0x00,
   CEC_VERSION_1_2                             = 0x01,
   CEC_VERSION_1_2a                            = 0x02,
   CEC_VERSION_1_3                             = 0x03,
   CEC_VERSION_1_3a                            = 0x04
}gmt_CEC_VERSIONS;

//<Play>.[Play Mode]
typedef enum
{
   gmd_PM_FastForwardMinSpeed                  = 0x05,
   gmd_PM_FastForwardMediumSpeed               = 0x06,
   gmd_PM_FastForwardMaxSpeed                  = 0x07,
   gmd_PM_FastReverseMinSpeed                  = 0x09,
   gmd_PM_FastReverseMediumSpeed               = 0x0A,
   gmd_PM_FastReverseMaxSpeed                  = 0x0B,
   gmd_PM_SlowForwardMinSpeed                  = 0x15,
   gmd_PM_SlowForwardMediumSpeed               = 0x16,
   gmd_PM_SlowForwardMaxSpeed                  = 0x17,
   gmd_PM_SlowReverseMinSpeed                  = 0x19,
   gmd_PM_SlowReverseMediumSpeed               = 0x1A,
   gmd_PM_SlowReverseMaxSpeed                  = 0x1B,
   gmd_PM_PlayReverse                          = 0x20,
   gmd_PM_PlayForward                          = 0x24,
   gmd_PM_PlayStill                            = 0x25
}gmt_PlayMode;

//<Deck control>.[Deck Control Mode]
typedef enum
{
   gmd_DCMSkipForward_Wind                     = 0x01,
   gmd_DCMSkipReverse_Rewind                   = 0x02,
   gmd_DCMStop                                 = 0x03,
   gmd_DCMEject                                = 0x04
}gmt_DeckControlMode;

typedef enum
{
   gmd_SAS_OFF = 0,
   gmd_SAS_ON = 1
}gmt_SystemAudioStatus;

//Types of signal source for recorder
typedef enum
{
   gmd_RSTRecordOwnSource                      = 0x01,
   gmd_RSTRecordDigitalService                 = 0x02,
   gmd_RSTRecordAnalogueService                = 0x03,
   gmd_RSTRecordExternalPlug                   = 0x04,
   gmd_RSTRecordExternalPhysicalAddress        = 0x05
} gmt_RecordSourceType;

//Analog broadcast inputs
typedef enum
{
   gmd_ABT_Cable                               = 0,
   gmd_ABT_Sattelite                           = 1,
   gmd_ABT_Terrestial                          = 2
} gmt_AnalogBroadcastType;

//Status codes which recorder or player can return in <Deck Status>
typedef enum
{
   gmd_DI_Play                                 = 0x11,
   gmd_DI_Record                               = 0x12,
   gmd_DI_PlayReverse                          = 0x13,
   gmd_DI_Still                                = 0x14,
   gmd_DI_Slow                                 = 0x15,
   gmd_DI_SlowReverse                          = 0x16,
   gmd_DI_FastForward                          = 0x17,
   gmd_DI_FastReverse                          = 0x18,
   gmd_DI_NoMedia                              = 0x19,
   gmd_DI_Stop                                 = 0x1A,
   gmd_DI_SkipForwardWind                      = 0x1B,
   gmd_DI_SkipReverseRewind                    = 0x1C,
   gmd_DI_IndexSearchFwd                       = 0x1D,
   gmd_DI_IndexSearchRev                       = 0x1E,
   gmd_DI_OtherStatus                          = 0x1F
}gmt_DeckInfo;

//Status codes which recorder can return in <Record Status>
typedef enum
{
   gmd_RSI_RecCurrentSource                    = 0x01,
   gmd_RSI_RecDigitalService                   = 0x02,
   gmd_RSI_RecAnalogueService                  = 0x03,
   gmd_RSI_RecExternalInput                    = 0x04,
   gmd_RSI_NoRecDigitalService                 = 0x05,
   gmd_RSI_NoRecAnalogueService                = 0x06,
   gmd_RSI_NoRecUnableSelectRequiredService    = 0x07,
   gmd_RSI_NoRecInvalidExtPlugNr               = 0x09,
   gmd_RSI_NoRecInvalidExtPhysicalAddress      = 0x0A,
   gmd_RSI_NoRecCASystemNotSupported           = 0x0B,
   gmd_RSI_NoRecInsufficientCAEntitlements     = 0x0C,
   gmd_RSI_NoRecNotAllowedToCopySource         = 0x0D,
   gmd_RSI_NoRecNoFurtherCopiesAllowed         = 0x0E,
   gmd_RSI_NoRecNoMedia                        = 0x10,
   gmd_RSI_NoRecPlaying                        = 0x11,
   gmd_RSI_NoRecAlreadyRecording               = 0x12,
   gmd_RSI_NoRecMediaProtected                 = 0x13,
   gmd_RSI_NoRecNoSourceSignal                 = 0x14,
   gmd_RSI_NoRecMediaProblem                   = 0x15,
   gmd_RSI_NoRecNotEnoughSpace                 = 0x16,
   gmd_RSI_NoRecParentalLock                   = 0x17,
   gmd_RSI_RecordingTerminatedNormally         = 0x1A,
   gmd_RSI_RecorlingHasAlreadyTerminated       = 0x1B,
   gmd_RSI_NoRecOtherReason                    = 0x1F
}gmt_RecordStatusInfo;

//TV systems from CEC viewpoint
typedef enum
{
   gmd_BS_PAL_B_G                              = 0,
   gmd_BS_SECAM_L_DASH                         = 1,
   gmd_BS_PAL_M                                = 2,
   gmd_BS_NTSC_M                               = 3,
   gmd_BS_PAL_I                                = 4,
   gmd_BS_SECAM_DK                             = 5,
   gmd_BS_SECAM_B_G                            = 6,
   gmd_BS_SECAM_L                              = 7,
   gmd_BS_PAL_DK                               = 8,
   gmd_BS_OtherSystem                          = 31
} gmt_BroadcastSystem;

#if 0
typedef enum
{
   gmd_None,
   gmd_CecVersion,
   gmd_PowerStatus,
   gmd_PhysicalAddress,
   gmd_MenuLanguage
}gmt_CecAppsTestReqType;
#endif
typedef enum
{
   gmd_CecPowerStatusOn                      = 0x00,
   gmd_CecPowerStatusStandby                 = 0x01,
   gmd_CecPowerStatusTransitionStandbyToOn   = 0x02,
   gmd_CecPowerStatusTransitionOnToStandby   = 0x03
}gmt_PowerStatus;

typedef enum
{
   gmd_SR_On,
   gmd_SR_Off,
   gmd_SR_Once
}gmt_StatusRequest;

typedef enum
{
   gmd_AR_Ok_Aborted,                          //message replied with feature abort
   gmd_AR_Ok_Replied,                          //message means reply and has been replied
   gmd_AR_Ok_NotReplied,                       //message means reply but hasn't been replied
   gmd_AR_Failed,                              //message doesn't mean reply - hasn't been acknowledged
   gmd_AR_Unknown
}gmt_CecAckResult;

//Device logical addresses
typedef enum
{
   gmd_TV                                      = 0x00,
   gmd_Recorder1                               = 0x01,
   gmd_Recorder2                               = 0x02,
   gmd_Tuner1                                  = 0x03,
   gmd_Player1                                 = 0x04,
   gmd_AudioSystem                             = 0x05,
   gmd_Tuner2                                  = 0x06,
   gmd_Tuner3                                  = 0x07,
   gmd_Player2                                 = 0x08,
   gmd_Recorder3                               = 0x09,
   gmd_Tuner4                                  = 0x0A,
   gmd_Player3                                 = 0x0B,
   gmd_Reserved12                              = 0x0C,
   gmd_Reserved13                              = 0x0D,
   gmd_FreeUse                                 = 0x0E,
   gmd_Unregistered                            = 0x0F
}gmt_DeviceType;


//Remote Control Pass Through feature codes
typedef enum
{
   gmd_UCCSelect                               = 0x0000,
   gmd_UCCUp                                   = 0x0001,
   gmd_UCCDown                                 = 0x0002,
   gmd_UCCLeft                                 = 0x0003,
   gmd_UCCRight                                = 0x0004,
   gmd_UCCRightUp                              = 0x0005,
   gmd_UCCRightDown                            = 0x0006,
   gmd_UCCLeftUp                               = 0x0007,
   gmd_UCCLeftDown                             = 0x0008,

   gmd_UCCVolumeUp                             = 0x0041,
   gmd_UCCVolumeDn                             = 0x0042,
   gmd_UCCMute                                 = 0x0043,

   gmd_UCCPlay                                 = 0x0044,
   gmd_UCCStop                                 = 0x0045,
   gmd_UCCPause                                = 0x0046,
   gmd_UCCRecord                               = 0x0047,
   gmd_UCCRewind                               = 0x0048,
   gmd_UCCFastForward                          = 0x0049,
   gmd_UCCEject                                = 0x004A,
   gmd_UCCStopRecording                        = 0x004D,
   gmd_OsdCecIdle                              = 0x00FF
}gmt_UserControlCommand;


typedef enum
{
   gmd_OCC_RecordOn                            = CEC_MSG_RECORD_ON,                //= 0x09,
   gmd_OCC_Ping                                = 0x0A,
   gmd_OCC_RecordOff                           = CEC_MSG_RECORD_OFF,               //= 0x0B,
   gmd_OCC_GetDeckStatus                       = CEC_MSG_GIVE_DECK_STATUS,         //= 0x1A,
   gmd_OCC_ClearAnalogueTimer                  = CEC_MSG_CLEAR_ANALOGUE_TIMER,     //= 0x33,
   gmd_OCC_SetAnalogueTimer                    = CEC_MSG_SET_ANALOGUE_TIMER,       //= 0x34,
   gmd_OCC_Standby                             = CEC_MSG_STANDBY,                  //= 0x36,
   gmd_OCC_Play                                = CEC_MSG_PLAY,                     //= 0x41,

   /*  Use follow codes as parameter:
       ==============================
       gmd_PM_FastForwardMinSpeed                   = 0x05,
       gmd_PM_FastForwardMediumSpeed                = 0x06,
       gmd_PM_FastForwardMaxSpeed                   = 0x07,
       gmd_PM_FastReverseMinSpeed                   = 0x09,
       gmd_PM_FastReverseMediumSpeed                = 0x0A,
       gmd_PM_FastReverseMaxSpeed                   = 0x0B,
       gmd_PM_SlowForwardMinSpeed                   = 0x15,
       gmd_PM_SlowForwardMediumSpeed                = 0x16,
       gmd_PM_SlowForwardMaxSpeed                   = 0x17,
       gmd_PM_SlowReverseMinSpeed                   = 0x19,
       gmd_PM_SlowReverseMediumSpeed                = 0x1A,
       gmd_PM_SlowReverseMaxSpeed                   = 0x1B,
       gmd_PM_PlayReverse                           = 0x20,
       gmd_PM_PlayForward                           = 0x24,
       gmd_PM_PlayStill                             = 0x25     */

   gmd_OCC_DeckControl                         = CEC_MSG_DECK_CONTROL,             //= 0x42

   /*  Use follow codes as parameter:
       ==============================
       gmd_DCMSkipForward_Wind                     = 0x01,
       gmd_DCMSkipReverse_Rewind                   = 0x02,
       gmd_DCMStop                                 = 0x03,
       gmd_DCMEject                                = 0x04
   */
   gmd_OCC_GiveOsdName                         = CEC_MSG_GIVE_OSD_NAME,            //= 0x46,
   gmd_OCC_SystemAudioModeRequest              = CEC_MSG_SYSTEM_AUDIO_MODE_REQUEST,//= 0x70,
   gmd_OCC_GiveAudioStatus                     = CEC_MSG_GIVE_AUDIO_STATUS,        //= 0x71
   gmd_OCC_SetSystemAudioMode                  = CEC_MSG_SET_SYSTEM_AUDIO_MODE,    //= 0x72,
   gmd_OCC_GiveSystemAudioModeStatus           = CEC_MSG_GIVE_SYSTEM_AUDIO_MODE_STATUS, //= 0x7D

   /*  Use follow codes as parameter:
       ==============================
       gmd_SAS_ON,
       gmd_SAS_OFF
   */

   gmd_OCC_GetPhysicalAddress                  = CEC_MSG_GIVE_PHYSICAL_ADDRESS,    //= 0x83,
   gmd_OCC_GetPowerStatus                      = CEC_MSG_GIVE_DEVICE_POWER_STATUS, //= 0x8F,
   gmd_OCC_GetMenuLanguage                     = CEC_MSG_GET_MENU_LANGUAGE,        //= 0x91,
   gmd_OCC_SetDigitalTimer                     = CEC_MSG_SET_DIGITAL_TIMER,        //= 0x97,
   gmd_OCC_ClearDigitalTimer                   = CEC_MSG_CLEAR_DIGITAL_TIMER,      //= 0x99,
   gmd_OCC_GetCecVersion                       = CEC_MSG_GET_CEC_VERSION,          //= 0x9F,
   gmd_OCC_ClearExternalTimer                  = CEC_MSG_CLEAR_EXTERNAL_TIMER,     //= 0xA1,
   gmd_OCC_SetExternalTimer                    = CEC_MSG_SET_EXTERNAL_TIMER,       //= 0xA2,
   gmd_OCC_RCPassThroughPressed                = 0xFD,
   gmd_OCC_RCPassThroughReleased               = 0xFE,
   gmd_OCC_Abort                               = CEC_MSG_ABORT                     //= 0xFF,
}gmt_OsdControlCode;


typedef enum
{
   gmd_UCS_idle,       //handler is ready for new command
   gmd_UCS_request,    //handler requested to run command
   gmd_UCS_pending,    //running command in progress
   gmd_UCS_complete,   //command complete, result code is accessible
   gmd_UCS_timeout     //timeout expired
}gmt_UserCommandState;

#define USER_INTERFACE_PARAMETERS_COUNT         15

typedef struct
{
   BYTE    RecordDay;
   BYTE    RecordMonth;
   BYTE    RecordHours;
   BYTE    RecordMinutes;
   BYTE    DurationHours;
   BYTE    DurationMinutes;
}gmt_CecRecordTimeStamp;

typedef struct
{
   gmt_DeviceType          LogicalAddress;
   BYTE                    OpCode;
   gmt_CecAckResult        AckResult;
}gmt_CecBriefCommand;

/*
typedef struct
{
    BYTE                    ExternalPlugNumber;
}gmt_ExtPlugDelayedRecord;

typedef struct
{
    WORD                    ExternalPhysAddr;
}gmt_ExtPhysicalAddrDelayedRecord;

typedef struct
{
    WORD                    CurrentChannel;
}gmt_AnalogDelayedRecord;
*/

typedef union
{
   BYTE    ExtPlugNumber;
   WORD    ExtPhysAddr;
   WORD    ChannelNumber;
}gmt_DelayedRecordSpecialData;

typedef enum
{
   gmd_ExternalPlug            = 0x04,
   gmd_ExternalPhysicalAddress = 0x05
}gmt_ExternalSourceSpecifier;

typedef struct
{
   gmt_CecRecordTimeStamp          TimeStamp;
   gmt_ExternalSourceSpecifier     InputSource;
   gmt_DelayedRecordSpecialData    Special;
}gmt_DelayedRecord;

typedef struct
{
   BYTE                    Parameters[USER_INTERFACE_PARAMETERS_COUNT];
   BYTE                    Length;
}gmt_CommonCommand;

typedef union
{
   gmt_DelayedRecord       DelayedRecord;
   gmt_CommonCommand       Common;
}gmt_CecUserCommandData;

typedef struct
{
   gmt_CecBriefCommand     BriefCommand;
   gmt_CecUserCommandData  Data;
}gmt_CecUserCommand;

typedef struct
{
   gmt_CecUserCommand      Command;
   gmt_UserCommandState    Stage;
}gmt_CecAppUserCommandHandlerInterface;

typedef enum
{
   gmd_SAC_NoneCommand,
   gmd_SAC_VolUpPress,
   gmd_SAC_VolUpRelease,
   gmd_SAC_VolDnPress,
   gmd_SAC_VolDnRelease,
   gmd_SAC_RequestVolume,
   gmd_SAC_ToggleMute
}gmt_SystemAudioCommand;

typedef enum
{
   gmd_TSD_NO_OVERLAP                          = 0x00,
   gmd_TSD_TIMER_BLOCKS_OVERLAP                = 0x01,
   gmd_TSD_TIMER_OVERLAP_WARNING_MASK          = 0x01,

   gmd_TSD_MEDIA_PRESENT_AND_NOT_PROTECTED     = 0x00,
   gmd_TSD_MEDIA_PRESENT_BUT_PROTECTED         = 0x02,
   gmd_TSD_MEDIA_NOT_PRESENT                   = 0x04,
   gmd_TSD_MEDIA_PRESENCE_MASK                 = 0x06,

   gmd_TSD_NOT_PROGRAMMED                      = 0x00,
   gmd_TSD_NO_FREE_TIMER_AVAILABLE             = 0x10,
   gmd_TSD_DATE_OUT_OF_RANGE                   = 0x20,
   gmd_TSD_RECORDING_SEQUENCE_ERROR            = 0x30,
   gmd_TSD_INVALID_EXTERNAL_PLUG_NUMBER        = 0x40,
   gmd_TSD_INVALID_EXTERNAL_PHYSICAL_ADDRESS   = 0x50,
   gmd_TSD_CA_SYSTEM_NOT_SUPPORTED             = 0x60,
   gmd_TSD_NO_OR_INSUFFICIENT_CA_ENTITLEMENTS  = 0x70,
   gmd_TSD_DOES_NOT_SUPPORT_RESOLUTION         = 0x80,
   gmd_TSD_PARENTAL_LOCK_ON                    = 0x90,
   gmd_TSD_CLOCK_FAILURE                       = 0xA0,
   gmd_TSD_DUPLICATE_ALREADY_PROGRAMMED        = 0xE0,
   gmd_TSD_NOT_PROGRAMMED_INFO                 = 0xF0,

   gmd_TSD_PROGRAMMED                          = 0x08,
   gmd_TSD_ENOUGH_SPACE_AVAILABLE              = 0x80,
   gmd_TSD_NOT_ENOUGH_SPACE_AVAILABLE          = 0x90,
   gmd_TSD_NO_MEDIA_INFO_AVAILABLE             = 0xA0,
   gmd_TSD_MAY_NOT_BE_ENOUGH_SPACE_AVAILABLE   = 0xB0,
   gmd_TSD_PROGRAMMED_INFO                     = 0xF0,

   gmd_TSD_PROGRAMMED_MASK                     = 0x08
}gmt_TIMER_STATUS_DATA;

typedef enum
{
   gmd_TCS_TIMER_NOT_CLEARED_RECORDING         = 0x00,
   gmd_TCS_TIMER_NOT_CLEARED_NO_MATCHING       = 0x01,
   gmd_TCS_TIMER_NOT_CLEARED_NO_INFO_AVAILABLE = 0x02,
   gmd_TCS_TIMER_CLEARED                       = 0x80
}gmt_TIMER_CLEARED_STATUS_DATA;

#if 0
#define OSD_CEC_FUNCTION_MASK                   0x0F00
#define OSD_CEC_BUTTON_STATE_MASK               0x1000
#endif
#define OSD_CEC_COMMAND_MASK                    0xFF00
#define OSD_CEC_PARAMETER_MASK                  0x00FF
#define OsdToCecCommand(Command,Parameter)  ((((WORD)Command) << 8)| Parameter)

//******************************************************************************
//  M A C R O   D E F I N I T I O N
//******************************************************************************
#define CEC_FEATURE_ABORT_CAPABLE

#define CEC_FEATURE_ONE_TOUCH_PLAY

#ifdef CEC_FEATURE_ONE_TOUCH_PLAY
#define CEC_IMAGE_VIEW_ON_CAPABLE
#define CEC_TEXT_VIEW_ON_CAPABLE
#define CEC_ACTIVE_SOURCE_CAPABLE
#endif

#define CEC_FEATURE_ONE_TOUCH_RECORD

#ifdef CEC_FEATURE_ONE_TOUCH_RECORD
#define CEC_RECORD_TV_SCREEN
#define CEC_RECORD_ON
#define CEC_RECORD_OFF
#define CEC_RECORD_STATUS
#endif

#define CEC_FEATURE_STANDBY

#ifdef CEC_FEATURE_STANDBY
#define CEC_STANDBY_CAPABLE
#endif

#define CEC_FEATURE_SYSTEM_INFO

#ifdef CEC_FEATURE_SYSTEM_INFO
#define CEC_CEC_VERSION_CAPABLE
#define CEC_GET_CEC_VERSION_CAPABLE
#define CEC_MSG_REPORT_PHYSICAL_ADDRESS_CAPABLE
#define CEC_GIVE_PHYSICAL_ADDRESS_CAPABLE
#define CEC_GET_MENU_LANGUAGE_CAPABLE
#define CEC_SET_MENU_LANGUAGE_CAPABLE
//#define CEC_FEATURE_POLLING MESSAGE - implemented in driver level
#endif

#define CEC_FEATURE_ROUTING_CONTROL_CAPABLE

#ifdef CEC_FEATURE_ROUTING_CONTROL_CAPABLE
#define CEC_ACTIVE_SOURCE_CAPABLE
#define CEC_INACTIVE_SOURCE_CAPABLE
#define CEC_REQUEST_ACTIVE_SOURCE_CAPABLE
#define REQUEST_ACTIVE_SOURCE_ON_POWER_ON
#define CEC_ROUTING_CHANGE_CAPABLE      //this function isn't supported because of no platforms with 2 or greater HDMI ports
//#define CEC_ROUTING_INFORMATION         //this function is supported by CEC-switches only
//#define CEC_SET_STREAM_PATH             //this function is supported by CEC-switches only
#endif

#define CEC_FEATURE_OSD_DISPLAY

#ifdef CEC_FEATURE_OSD_DISPLAY
#define CEC_SET_OSD_STRING_CAPABLE
#endif

#define CEC_FEATURE_OSD_TRANSFER

#ifdef CEC_FEATURE_OSD_TRANSFER
#define CEC_SET_OSD_NAME_CAPABLE
#define CEC_GIVE_OSD_NAME_CAPABLE
#endif

#define CEC_FEATURE_POWER_STATUS

#ifdef CEC_FEATURE_POWER_STATUS
#define CEC_GIVE_DEVICE_POWER_STATUS_CAPABLE
#define CEC_REPORT_POWER_STATUS_CAPABLE
#endif

#define CEC_FEATURE_ABORT

#if defined(ALL_BOARDS_HDMI3)
#define IsCecInternal(Input)    ((Input != ALL_BOARDS_HDMI) && (Input != ALL_BOARDS_HDMI2) && (Input != ALL_BOARDS_HDMI3))
#elif defined(ALL_BOARDS_HDMI2)
#define IsCecInternal(Input)    ((Input != ALL_BOARDS_HDMI) && (Input != ALL_BOARDS_HDMI2))
#elif defined(ALL_BOARDS_HDMI)
#define IsCecInternal(Input)    (Input != ALL_BOARDS_HDMI)
#endif

#define CEC_BROADCAST_LOG_ADDR  0x0f

//******************************************************************************
//  G L O B A L    F U N C T I O N S
//******************************************************************************
void InitCECString(void);
void OsdToCecBridge(WORD command);
void SwitchMainSources(void);
void SwitchLogicalAddresses(BYTE increment, gmt_DeviceType B_DeviceType);
void gm_CecProcessSourceChange(gmt_UserDefConnNames CurrentPort, gmt_UserDefConnNames NextPort, gmt_UserDefConnNames OtherInput);
void gm_CecProcessGoToStandby(BYTE B_DestAddr);
void PrintToOsdHandler(void);
void PrintHDMITree(void);
void CecAppSystemAudioHandler(void);
void RequestSystemAudioStatus(void);
BOOL CecAppPowerHandlerRequest(gmt_PwrCecRequestType Request);

//extern gmt_CecAppUserCommandHandlerInterface gm_UserCommandInterface;
extern const BOOL CEC_ENABLED;
extern BYTE Ba_StatusLine[];
