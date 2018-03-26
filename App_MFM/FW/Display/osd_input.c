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
//
//    ---
//
//    MODULE:     osd_input.c
//
//    USAGE:      Get OSD input event (IR and keypad)
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "system\all.h"
#if (FEATURE_LOW_POWER_MONITOR == ENABLE)
#include "StdLibInc\gm_LPM_IPC.h"
#endif
extern void UpdateMonitorPowerOnCounter(void);

//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************

//#define _DEBUG
#ifdef _DEBUG
#define msg(a,b)  gm_Print(a,b)
#else
#define msg(a,b)
#endif

//
// Number of 10 ms timer counter
// Explain??
//
// TODO: find out what the following lines are for:
/*
#define FIRST_TIMER_EVENT     OE_OSDTimeout
*/

#define OsdKeyQueueLng     2     // Number keys saved in queue

enum
{
#ifdef PORT_GPIO_USED
   GPIO_KEY,
#endif
#ifdef PORT_ADC_1_USED
   ADC1_KEY,
#endif
#ifdef PORT_ADC_2_USED
   ADC2_KEY,
#endif
#ifdef PORT_ADC_3_USED
   ADC3_KEY,
#endif
#ifdef PORT_IR_USED
   IR_KEY,
#endif
   MAX_KEY_TYPE
};

#if(WB_MAJOR == 0x0004)
#define  NUMBER_OF_MultiplePortsValuesTranslation  WBK_NumberMultiplePortsValues // WB4
#define  MultiplePortsValuesTranslation   WBK_MultiplePortsValuesTranslation        // WB4
#endif

//******************************************************************************
//  EXTERN    V A R I A B L E S
//******************************************************************************

//extern BYTE ROM * ROM TrtblLUT[];
extern BYTE ROM TrtblMaxEntries[];
extern BYTE ROM B_NumberOfThreads;
extern WBK_KEYPAD_DEFINITION ROM Keypad_IR;
extern BOOL DDCPowerNapFlag;
//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************
typedef struct
{
   ST_KEYPAD_STATE St_PreviousKS;
   ST_KEYPAD_STATE St_CurrentKS;
   BYTE B_ScanDelay; // = 0;
   ST_TRANSLATION_TABLE_ENTRY ROM *Stp_TransTable;
} gmt_KEY_THREAD;

static gmt_KEY_THREAD gmv_KeyThreadArray [NUMBER_OF_THREADS];

WORD  W_TimeKeyHeldDown;// = 0; Used in Ballistic Acceleration

//******************************************************************************
//  S T A T I C    V A R I A B L E S
//******************************************************************************

//
//  State of keypad driver
//
#if (WB_MAJOR == 0x0004)
static KEY_INDEX  B_ScanCode;
#else
static KEY_INDEX  B_ScanCode[NUMBER_OF_INPUT_SOURCE];
#endif

//TODO: find out about Abort key execution
//BOOL gB_AbortExecutionEnabled = FALSE;

//
// Osd Key queue parameters and initialization
//
#if (WB_MAJOR == 0x0004)
static BYTE B_OsdKeyQueue[OsdKeyQueueLng];   // WB4
#else
static BYTE B_OsdKeyQueue[MAX_KEY_TYPE][OsdKeyQueueLng];
#endif
static BYTE B_OsdKeyQueueRdIdx;//= 0;                 // queue read index
static BYTE B_OsdKeyQueueLen;//= 0;                // queue length

//
// Mode Handler to OSD event
// NOTE : MUST be align with typedef "MessageToOsdType" in ModeHandler.h
//
#if( WB_STATE_MACHINE_VERSION == 0x0005 )
static WORD ROM MainSystemOsdEvents[MTO_MAX] =
#else
static BYTE ROM MainSystemOsdEvents[MTO_MAX] =
#endif
{
   OE_None,
   OE_SystemStartUp,             // MTO_SystemStartUp
   OE_StartPowerDown,               // MTO_StartPowerDown
   OE_PowerDownNow,              // MTO_PowerDownNow
   OE_PowerUp,                   // MTO_PowerUp
   OE_DisplayOn,                 // MTO_DisplayOn
   OE_DisplayOff,                // MTO_DisplayOff
   //
   OE_SplashScreen,                 // MTO_SplashScreen
   OE_StartPowerSaving,          // MTO_StartPowerSaving
   OE_PowerSavingNow,               // MTO_PowerSavingNow
   OE_ExitPowerSaving,              // MTO_ExitPowerSaving
   //
   OE_GoingToSleep,              // MTO_GoingToSleep
   OE_WakeUp,                 // MTO_WakeUp
   //
   OE_SyncPresent_Main,          // MTO_SyncPresent
   OE_ValidMode_Main,               // MTO_ValidMode
   OE_InvalidMode_Main,          // MTO_InvalidMode
   OE_OutOfRange_Main,              // MTO_OutOfRange
   OE_PseudoMode_Main,              // MTO_PseudoMode
   OE_Overlapped_Main,              // MTO_Overlapped
   OE_NoSync_Main,                  // MTO_NoSync
   OE_NoCable_Main,              // MTO_NoCable
   OE_ModeUnstable_Main,            // MTO_ModeUnstable
   OE_NoSignal_Main,             // MTO_NoSignal
   //
   OE_AutoAdjustStart,              // MTO_AutoAdjustStart
   OE_AutoAdjustCompleted,          // MTO_AutoAdjustCompleted
   OE_AutoAdjustFailed,          // MTO_AutoAdjustFailed
   //
   OE_TV_AutoScanStart,          // MTO_TV_AutoScanStart
   OE_TV_ChannelChanged,            // MTO_TV_ChannelChanged
   OE_TV_AutoScanCompleted,         // MTO_TV_AutoScanCompleted
#ifdef LCDTV_PAL
   OE_TV_FrequencyChanged,       //MTO_TV_FrequencyChanged
#endif
   OE_TV_ProgramBlocked,         //MTO_TV_ProgramBlocked,
   OE_TV_ProgramUnBlocked,       // MTO_TV_ProgramUnBlocked
   OE_TV_ChannelBlocked,         // MTO_TV_ChannelBlocked
   OE_TV_ChannelUnBlocked,       // MTO_TV_ChannelUnBlocked
   OE_AudioMute,                 // MTO_AudioMute
   OE_AudioOn,                   // MTO_AudioOn
   OE_AudioSourceChanged,           // MTO_AudioSourceChanged
   OE_AudioTypeChanged,             // MTO_AudioTypeChanged
   OE_MinuteUpdate,                 // MTO_MinuteUpdate
   OE_CloseCaptionOn,                  // MTO_CloseCaptionOn
   OE_CloseCaptionOff,                 // MTO_CloseCaptionOff
   OE_UpdateMultiPipChannel,              //MTO_UpdateMultiPipChannel,
   OE_MultiPipOff,                     //MTO_MultiPipOff
   OE_UnFreeze,                     // MTO_UnFreeze
   OE_DisplayBlockedChannelNum,     // parental blocked channel number display
   OE_EraseBlockedChannelNum,    // parental blocked channel number display
   OE_EnterPowerDeepSleep,       // Enter deep sleep
   OE_ExitPowerDeepSleep,        // Exit deep sleep
   OE_TtxStatusDisplayOn,
   OE_TV_ManualScanCompleted,
   OE_TV_ManualScanStart,
   OE_TeletextOff,
   OE_PipIsNotAllowed,            //due to bandwith restriction or other reason PIP is not allowed in current condition
   OE_AspectRatioUpdate,			//update aspect ratio
   OE_ADCCalibrationCompleted, //update ADC calibration values
   OE_RemoveOSD,                  // Remove OSD from screen
   OE_ChangeInputToHDMI,			// Change Input to HDMI
   OE_ShowCECString,             // Show CEC String
   OE_CEC_NewDeviceDetected,
   OE_CEC_DevDataBaseChange,
   OE_CEC_Command_OK,            //
   OE_CEC_Command_Failed,         //
   OE_CEC_Command_Pwr_Off,
   OE_CEC_Command_Pwr_On,
   OE_HideCECString,
   OE_ChangeInputToHDMI2,			// Change Input to HDMI2
   OE_ChangeInputToHDMI3,            // Change Input to HDMI3
   OE_CEC_Text4UserReady,
   OE_DrawVolumeSlider,
   OE_UpdateMuteIcon,
   OE_SystemAudioStatusChanged,
   OE_IspStatusChanged,				// SMART_ISP
   OE_IspAutoDetected,				// SMART_ISP_AUTO_DETECTION
   OE_OSDPort_4kx2k,		// 20120418
#ifdef DELL_U2410F
	OE_ScanFinished, // 70
	OE_RemoveMenu_Main, // 71
	//OEK_D_ADC_Menu_Key, // 72
	//OEK_D_PowerDown, // 73
	//OEK_D_ADC_Up_Key, // 74
	//OEK_D_ADC_Down_Key, // 75
	//OEK_D_ADC_Input_Key, // 76
	//OEK_D_PIP, // 77
	OE_HardLock, // 78
	//OE_SplashScreenChangeState, // 79
	OE_IconShow, //80
	OE_PIPIconShow, //81
	OE_DiagnosticTestPattern, // 82 
	OE_PIPMShow, 		// 79 //83
	OE_PIPMHide, 			// 80//84
	OE_PIPMessages, //85
	OE_AutoColorCompleted, // 86
	OE_AutoColorFailed, // 87
	OE_IconHide, // 88
	OE_HaveCable_Main, // 89
	OE_PIPIconHide, // 90
	OE_ForcePowerSaving, // 91
	OE_StopLCDCondition,		//92
	OE_NoCableTimer,   //93  //KevinL 20100921
	OE_UpdateEnergyStar,   //94  //KevinL 20101018
    OE_VWD_Enable,          //95 MTO_VWD_Enable  //owen 20120917
    OE_VWD_Lost,              //96 MTO_VWD_Lost //owen 20120917
    OE_SundBox_OSD_Eanble,                  //97 owen 120919
	OE_SundBox_OSD_Disable,                 //98 owen 120919
	OE_AudioPCM_Message,                      //99 owen 120919
	OE_ShowInfo,						// 100
	OE_BoardIDStart,					// 101
	OE_BoardIDEnd,					// 101
	OE_EnterFactoryMenu,					// 102
	OE_ExitFactoryMenu,					// 103
	OE_CMD_PowerSwitch,			// 104	
	//OE_CMD_PresetMode,// 105
   	//OE_CMD_CTAdjust,			// 106
   	//OE_CMD_CTUpdate,			// 107
   #endif
};

#if( WB_STATE_MACHINE_VERSION == 0x0005 )
static WORD ROM PipSystemOsdEvents[MTO_MAX] =
#else
static BYTE ROM PipSystemOsdEvents[MTO_MAX] =
#endif
{
   OE_None,
   OE_SystemStartUp,             // MTO_SystemStartUp
   OE_StartPowerDown,               // MTO_StartPowerDown
   OE_PowerDownNow,              // MTO_PowerDownNow
   OE_PowerUp,                   // MTO_PowerUp
   OE_DisplayOn,                 // MTO_DisplayOn
   OE_DisplayOff,                // MTO_DisplayOff
   //
   OE_SplashScreen,                 // MTO_SplashScreen
   OE_StartPowerSaving,          // MTO_StartPowerSaving
   OE_PowerSavingNow,               // MTO_PowerSavingNow
   OE_ExitPowerSaving,              // MTO_ExitPowerSaving
   //
   OE_GoingToSleep,              // MTO_GoingToSleep
   OE_WakeUp,                 // MTO_WakeUp
   //
   OE_SyncPresent_Pip,              // MTO_SyncPresent
   OE_ValidMode_Pip,             // MTO_ValidMode
   OE_InvalidMode_Pip,              // MTO_InvalidMode
   OE_OutOfRange_Pip,               // MTO_OutOfRange
   OE_PseudoMode_Pip,               // MTO_PseudoMode
   OE_Overlapped_Pip,               // MTO_Overlapped
   OE_NoSync_Pip,                // MTO_NoSync
   OE_NoCable_Pip,                  // MTO_NoCable
   OE_ModeUnstable_Pip,          // MTO_ModeUnstable
   OE_NoSignal_Pip,              // MTO_NoSignal
   //
   OE_AutoAdjustStart,              // MTO_AutoAdjustStart
   OE_AutoAdjustCompleted,          // MTO_AutoAdjustCompleted
   OE_AutoAdjustFailed,          // MTO_AutoAdjustFailed
   //
   OE_TV_AutoScanStart,          // MTO_TV_AutoScanStart
   OE_TV_ChannelChanged,            // MTO_TV_ChannelChanged
   OE_TV_AutoScanCompleted,         // MTO_TV_AutoScanCompleted
#ifdef LCDTV_PAL
   OE_TV_FrequencyChanged,       //MTO_TV_FrequencyChanged
#endif
   OE_TV_ProgramBlocked,         //MTO_TV_ProgramBlocked,
   OE_TV_ProgramUnBlocked,       // MTO_TV_ProgramUnBlocked
   OE_TV_ChannelBlocked,         // MTO_TV_ChannelBlocked
   OE_TV_ChannelUnBlocked,       // MTO_TV_ChannelUnBlocked
   OE_AudioMute,                 // MTO_AudioMute
   OE_AudioOn,                   // MTO_AudioOn
   OE_AudioSourceChanged,           // MTO_AudioSourceChanged
   OE_AudioTypeChanged,          // MTO_AudioTypeChanged
   OE_MinuteUpdate,              //MTO_MinuteUpdate
   OE_CloseCaptionOn,                  // MTO_CloseCaptionOn
   OE_CloseCaptionOff,                 // MTO_CloseCaptionOff
   OE_UpdateMultiPipChannel,              //MTO_UpdateMultiPipChannel
   OE_MultiPipOff,                  //MTO_MultiPipOff
   OE_UnFreeze,                  // MTO_UnFreeze
   OE_DisplayBlockedChannelNum,     // parental blocked channel number display
   OE_EraseBlockedChannelNum,       // parental blocked channel number display
   OE_EnterPowerDeepSleep,       // Enter deep sleep
   OE_ExitPowerDeepSleep,        // Exit deep sleep
   OE_TtxStatusDisplayOn,
   OE_TV_ManualScanCompleted,
   OE_TV_ManualScanStart,
   OE_TeletextOff,
   OE_PipIsNotAllowed,            //due to bandwith restriction or other reason PIP is not allowed in current condition
   OE_AspectRatioUpdate,		 //aspect ratio update
   OE_ADCCalibrationCompleted, //update ADC calibration values
   OE_RemoveOSD,                  // Remove OSD from screen
   OE_ChangeInputToHDMI,			// Change Input to HDMI
   OE_ShowCECString,             // Show CEC String
   OE_CEC_NewDeviceDetected,
   OE_CEC_DevDataBaseChange,
   OE_CEC_Command_OK,            //
   OE_CEC_Command_Failed,         //
   OE_CEC_Command_Pwr_Off,
   OE_CEC_Command_Pwr_On,
   OE_HideCECString,
   OE_ChangeInputToHDMI2,			// Change Input to HDMI2
   OE_ChangeInputToHDMI3,            // Change Input to HDMI3
   OE_CEC_Text4UserReady,
   OE_DrawVolumeSlider,
   OE_UpdateMuteIcon,
   OE_SystemAudioStatusChanged,
   OE_IspStatusChanged,				// SMART_ISP
   OE_IspAutoDetected,				// SMART_ISP_AUTO_DETECTION
   OE_OSDPort_4kx2k,		// 20120418
#ifdef DELL_U2410F
	OE_ScanFinished, // 70
	OE_RemoveMenu_Main, // 71
	//OEK_D_ADC_Menu_Key, // 72
	//OEK_D_PowerDown, // 73
	//OEK_D_ADC_Up_Key, // 74
	//OEK_D_ADC_Down_Key, // 75
	//OEK_D_ADC_Input_Key, // 76
	//OEK_D_PIP, // 77
	OE_HardLock, // 78
	//OE_SplashScreenChangeState, // 79
	OE_IconShow, //80
	OE_PIPIconShow, 	//77 //81
	OE_DiagnosticTestPattern, // 82 
	OE_PIPMShow, 		// 79	//83
	OE_PIPMHide, 		// 80	//84
	OE_PIPMessages, //85
	OE_AutoColorCompleted, // 86
	OE_AutoColorFailed, // 87
	OE_IconHide, // 88
	OE_HaveCable_Main, // 89
	OE_PIPIconHide, // 90
	OE_ForcePowerSaving, // 91
	OE_StopLCDCondition,		//92
	OE_NoCableTimer,   //93  //KevinL 20100921	
	OE_UpdateEnergyStar,   //94  //KevinL 20101018	
	OE_VWD_Enable,          //95 MTO_VWD_Enable //owen 20120917
       OE_VWD_Lost,              //96 MTO_VWD_Lost //owen 20120917
       OE_SundBox_OSD_Eanble,                  //97 owen 120919
	OE_SundBox_OSD_Disable,                 //98 owen 120919
	OE_AudioPCM_Message,                      //99 owen 120919
	OE_ShowInfo,						// 100
	OE_BoardIDStart,					// 101
	OE_BoardIDEnd,					// 101
	OE_EnterFactoryMenu,					// 102
	OE_ExitFactoryMenu,					// 103
	OE_CMD_PowerSwitch,			// 104	

#endif	
};

//******************************************************************************
//  S T A T I C    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

static void ProcessKeyOsdEvent      (void);
static void OsdKeyQueueInit         (void);
static BYTE ModeHdlrMsgToOsdEvent   (void);
#if (WB_MAJOR == 0x0004)
static void InitKeypadsDrivers(void);     // WB4
#endif
//******************************************************************************
//  C O D E
//******************************************************************************

//******************************************************************************
//
// FUNCTION       : void OsdEventHandler(void)
//
// USAGE          : Application level OSD event handler.
//                  Check whether any OSD input source has a request of generating
//                  OSD event.  Call standard library osd state machine to process
//                  event.
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
static t_REQUEST ST_Request;
void OsdEventHandler(void)
{
   WORD W_OsdEvent;
   BYTE B_Channel;
   BYTE B_Thread;

#if (FEATURE_LOW_POWER_MONITOR == ENABLE)
   {
      BYTE B_Buffer;

      if (gm_MessageReceived(gmd_MESSAGE_POWER, (BYTE far *)&B_Buffer, sizeof(B_Buffer)))
      {
         if (B_Buffer == TRUE)//this means mission should shut down
         {
#ifdef WBK_FUNCTION_GETADCVALUE_USED
            EnqueueOsdKey(Key_KEY_PHYSICAL_POWER);
#endif
         }
      }
   }
#endif

#ifdef DEMO_SPLASH_SCREEN        // Do not call OsdBlendSplashScreen() if not defined
   #ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()==CHIPID_4K2K_FE)
   #endif
   {
      if (B_SplashComplete == FALSE)   // Do not call OsdBlendSplashScreen() if complete
         OsdBlendSplashScreen();    // Blend one pass in every main loop pass
   }
#endif
   // Updates the Monitor Power On Counter.
	#ifndef DELL_U2410F //KevinL 20090326 to avoid write NVRAM every 1 Min
   UpdateMonitorPowerOnCounter();
	#endif

   //
   // Process Mode handler event first
   //
   for (B_Channel = CH_A; B_Channel <= CH_B; ++B_Channel)
   {
      OSDRequestGet(B_Channel, REQ_ANY, &ST_Request);
      while (ST_Request.B_Request != REQ_NONE)
      {
         switch (ST_Request.B_Request)
         {

               //
               // OSG Msgs
               //
            case REQ_OSD_MSG:
               if (B_Channel == gmvb_MainChannel)
               {
                  W_OsdEvent = MainSystemOsdEvents[ ST_Request.W_Param ];
               }
               else
               {
                  W_OsdEvent = PipSystemOsdEvents[ ST_Request.W_Param ];
               }

               //gm_Printf("OE Event: %d -> %d", ST_Request.W_Param, B_OsdEvent);
               for (B_Thread = 0; B_Thread < NUMBER_OF_THREADS; B_Thread++)
               {
                  gm_SwitchOsdThread(B_Thread);
#if( WB_STATE_MACHINE_VERSION == 0x0005 )
                  gm_OsdEventHandlerSM5(W_OsdEvent);  // WB5
#elif( WB_STATE_MACHINE_VERSION == 0x0004 )
                  gm_OsdEventHandlerSM4(W_OsdEvent);  // WB4
#else
                  gm_OsdEventHandler((BYTE)W_OsdEvent);
#endif
               }
               break;

               // TODO: Could Handle other OSD Requests here (Keypad, IR ?)

         }
         OSDRequestGet(B_Channel, REQ_ANY, &ST_Request);
      }

   } // for Channel

   //
   // process timer events second
   //
#ifdef OSD_TIMERS_NUM
   //A OSD timer handler to read the current value of the timer and  decrement each OSD timers accordinly.
   //If an OSD timer is  timeout, it returns the event that is associated with that timer.
   {
      // Process OSD Timer
      static WORD W_PrevTimerRead = 0;
      WORD W_TimerRead = gm_GetSystemTimeW();
      WORD W_Time  = W_TimerRead - W_PrevTimerRead;
      WORD w = W_OsdTimerResolution*10;
      BYTE B_Count;
      if ((w <= W_Time) && (W_Time < (WORD)(-w))) //check if the time of timer resolution has been passed
      {
         W_PrevTimerRead = W_TimerRead;

         for (B_Count = 0 ; B_Count < OSD_TIMERS_NUM ; B_Count++) //check all OSD timers
         {
            if (gB_OsdTimerStart[B_Count] == OSD_TIMER_START)
            {
               if (gW_OsdTimers[B_Count]) //decrement timer
                  gW_OsdTimers[B_Count]--;

               if (gW_OsdTimers[B_Count] == 0) //timer expired
               {
                  W_OsdEvent = gB_OsdTimerEvents[B_Count];
                  gB_OsdTimerStart[B_Count] = OSD_TIMER_STOP;

                  for (B_Thread = 0; B_Thread < NUMBER_OF_THREADS; B_Thread++)
                  {
                     gm_SwitchOsdThread(B_Thread);
                     if (W_OsdEvent != OE_None)
                     {
#if( WB_STATE_MACHINE_VERSION == 0x0005 )
                        gm_OsdEventHandlerSM5(W_OsdEvent);  // WB5
#elif( WB_STATE_MACHINE_VERSION == 0x0004 )
                        gm_OsdEventHandlerSM4(W_OsdEvent);     // WB4
#else
                        gm_OsdEventHandler((BYTE)W_OsdEvent);
#endif
                     }
                  }
               }
            }
         }
      }
   }
#endif

   //
   // process posted events third
   //
   W_OsdEvent = gm_GetPostedEvent();

   for (B_Thread = 0; B_Thread < NUMBER_OF_THREADS; B_Thread++)
   {
      gm_SwitchOsdThread(B_Thread);
      if (W_OsdEvent != OE_None)
      {
#if( WB_STATE_MACHINE_VERSION == 0x0005 )
         gm_OsdEventHandlerSM5(W_OsdEvent);  // WB5
#elif( WB_STATE_MACHINE_VERSION == 0x0004 )
         gm_OsdEventHandlerSM4(W_OsdEvent);     // WB4
#else
         gm_OsdEventHandler((BYTE)W_OsdEvent);
#endif
      }
   }

   //
   // Process keyboard and user events (one time every KEYPAD_DEBOUNCE_PERIOD ms)
   //
   {
      static WORD W_PrevTimerRead = 0;
      WORD W_TimerRead = gm_GetSystemTimeW();
      WORD W_Time  = W_TimerRead - W_PrevTimerRead;
      if ((KEYPAD_DEBOUNCE_PERIOD <= W_Time) && (W_Time < (WORD)(-KEYPAD_DEBOUNCE_PERIOD)))
      {
         W_PrevTimerRead = W_TimerRead;
         W_TimeKeyHeldDown += W_Time/(KEYPAD_DEBOUNCE_PERIOD);// is number of ticks between this and previous ProcessKeyOsdEvent.
         ProcessKeyOsdEvent();
      }
   }

   return;
}


//******************************************************************************
//
// FUNCTION       : void FlushOsdKeyQueue(void)
//
// USAGE          : Flushes the osd key queue by calling osd key queue init.
//                  In addition, this function also clears all the key states
//                  thus far.
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : gmv_KeyThreadArray    (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void FlushOsdKeyQueue(void)
{
   BYTE B_Thread;
   for (B_Thread = 0; B_Thread < NUMBER_OF_THREADS; B_Thread++)
   {
      gmt_KEY_THREAD* p = &gmv_KeyThreadArray [B_Thread];
      p->B_ScanDelay = 0;
      p->Stp_TransTable = NULL;
      p->St_CurrentKS.B_iButton = p->St_PreviousKS.B_iButton = Key_None;
      p->St_CurrentKS.B_iTimer = p->St_PreviousKS.B_iTimer = 0;
   }
   OsdKeyQueueInit();
}

//******************************************************************************
//
// FUNCTION       : void OSDKeyInit(void)
//
// USAGE          : initialize the keypad structures for multithread OSD
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : gmv_KeyThreadArray    (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void OSDKeyInit(void)
{
   FlushOsdKeyQueue();
#if (WB_MAJOR == 0x0004)
   InitKeypadsDrivers();   // WB4
#endif
}

//******************************************************************************
//
// FUNCTION       :  static void ProcessKeyOsdEvent(void)
//
// USAGE          : Processes a user key event.
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
static void ProcessKeyOsdEvent(void)
{
   KEY_INDEX KeyValue = Key_None;

#if (WB_MAJOR == 0x0004)
   if (DequeueOsdKey(&KeyValue))
   {
      B_ScanCode = KeyValue;
   }
   else
      KeyValue = B_ScanCode;
#else

   //GetKey: Get OSD keys from the 2-D OSD key queue. Check if we have simultaneous key press.
   //If so, translate the key combintion to a new OSD key
   {
      BYTE B_CountKey = 0;
      BYTE B_TempKeyTypeCnt = MAX_KEY_TYPE; //5 = max number of input ports: GPIO, ADC1,2,3, IR

      //Get key from queue
      DequeueOsdKey();     //put values into the B_ScanCode array
      //Set up the B_ScanCode array - it represents all the keys that are detected simultaneously
      while (B_TempKeyTypeCnt)
      {
         B_TempKeyTypeCnt--;
         if (B_ScanCode[B_TempKeyTypeCnt] != Key_None) //if the dequeued value isn't Key_None - the initial value
         {
            B_CountKey++;
            KeyValue = B_ScanCode[B_TempKeyTypeCnt];
         }
      }

      // Check if we have Multiple Ports keypad key.
      if (B_CountKey > 1)
      {
         KeyValue = Key_None;
         if (NUMBER_OF_MultiplePortsValuesTranslation)
         {
            BYTE B_CountI;
            for (B_CountI = 0; B_CountI < NUMBER_OF_MultiplePortsValuesTranslation; B_CountI++)
            {
               BYTE B_Found = 0xff;
               BYTE B_CountJ;
               for (B_CountJ = 0; B_CountJ < NUMBER_OF_INPUT_SOURCE; B_CountJ++)
               {
                  B_Found &= MultiplePortsValuesTranslation[B_CountI].Codes[B_CountJ] == B_ScanCode[B_CountJ];
               }
               if (B_Found)
               {
                  KeyValue = MultiplePortsValuesTranslation[B_CountI].KeyIndex;
                  break;
               }
            }
         }
      }
   }
#endif

   //---Find appropriate Osd event in each tread for current key combination and process it (if it is not OE_None) in OSD SM---
   {
      BYTE B_Thread;
      for (B_Thread = 0; B_Thread < NUMBER_OF_THREADS; B_Thread++)
      {
         WORD W_OsdEvent = OE_None;
         gmt_KEY_THREAD* pKeyThread = &gmv_KeyThreadArray [B_Thread];
         ST_TRANSLATION_TABLE_ENTRY ROM * pCurTrTable;

         gm_SwitchOsdThread(B_Thread);

         // If the key changed since last time.
         if (KeyValue != pKeyThread->St_CurrentKS.B_iButton)
         {
            pKeyThread->St_PreviousKS = pKeyThread->St_CurrentKS;
            pKeyThread->St_PreviousKS.B_iTimer = pKeyThread->St_CurrentKS.B_iTimer;
            pKeyThread->St_CurrentKS.B_iButton = KeyValue;
            pKeyThread->B_ScanDelay = 0;
            W_TimeKeyHeldDown = 0;
         }

         pKeyThread->St_CurrentKS.B_iTimer = (BYTE)W_TimeKeyHeldDown;

         //---Translates keypad and IR key to OSD event using translation table ------
         {
            BYTE j= gm_OsdGetCurrentStateTranslTable();
            pCurTrTable = GlobalTranslTableSet[j];
            if (pCurTrTable)
            {
               ST_TRANSLATION_TABLE_ENTRY ROM * entryPtr = pCurTrTable;
               BYTE curTime = 0;
               BYTE bLastPrevTimer = 0;
               
               for (j = TrtblMaxEntries[j]; j-- != 0; entryPtr++)// ScanRemoteKey to see if a new value was found.
               {
                  if (// For IR the codes must match exactly, so returning the value when the codes are exactly equal.
                     (entryPtr->St_CurrentKS.B_iButton == pKeyThread->St_CurrentKS.B_iButton) && //key-state is same
                     (entryPtr->St_PreviousKS.B_iButton == pKeyThread->St_PreviousKS.B_iButton) && //previous key-state is same
                     (entryPtr->St_CurrentKS.B_iTimer <= pKeyThread->St_CurrentKS.B_iTimer) && //current time has gone
                     (entryPtr->St_PreviousKS.B_iTimer <= pKeyThread->St_PreviousKS.B_iTimer)) //current time has gone
                  {
                     // Find biggest time for Previous Timer
                     if (entryPtr->St_PreviousKS.B_iTimer >= bLastPrevTimer)
                     {
                        bLastPrevTimer = entryPtr->St_PreviousKS.B_iTimer;
                        pCurTrTable = entryPtr;
                        W_OsdEvent = entryPtr->W_iEvent;

                        // Find biggest time for Current Timer
                        if (entryPtr->St_CurrentKS.B_iTimer >= curTime)
                        {
                           curTime = entryPtr->St_CurrentKS.B_iTimer;
                           pCurTrTable = entryPtr;
                           W_OsdEvent = entryPtr->W_iEvent;
                        }
                     }
                  }
                  else
                     if (W_OsdEvent != OE_None)
                        break;
               }
               if (W_OsdEvent == OE_None)
                  pCurTrTable = NULL;
            }
         }

         //new event has been found
         if (((pKeyThread->Stp_TransTable != NULL) && (pKeyThread->Stp_TransTable->W_iEvent != W_OsdEvent))
               ||((pKeyThread->Stp_TransTable == NULL) && (W_OsdEvent != OE_None)))
         {
            pKeyThread->Stp_TransTable = pCurTrTable;
         }
         //if event is same, check it for autorepeat
         else if (pCurTrTable && (pCurTrTable->W_iAutorepeatEnable))
         {
            BYTE j = pKeyThread->St_CurrentKS.B_iTimer - pKeyThread->B_ScanDelay;

            if (((pKeyThread->B_ScanDelay == 0) && (j > pCurTrTable->B_iAutorepeatStartDelay)) ||
                  ((pKeyThread->B_ScanDelay != 0) && (j > pCurTrTable->W_iAutorepeatDelay)))
            {
               pKeyThread->B_ScanDelay = pKeyThread->St_CurrentKS.B_iTimer;
            }
            else
               W_OsdEvent = OE_None;
         }
         else
            W_OsdEvent = OE_None;

         //---process OSD event in OSD SM----
         if (W_OsdEvent != OE_None)
         {
#if( WB_STATE_MACHINE_VERSION == 0x0005 )
            gm_OsdEventHandlerSM5(W_OsdEvent);  // WB5
#elif( WB_STATE_MACHINE_VERSION == 0x0004 )
            gm_OsdEventHandlerSM4(W_OsdEvent);  // WB4
#else
            gm_OsdEventHandler((BYTE)W_OsdEvent);
#endif
         }
      }
   }

}

//******************************************************************************
//
// FUNCTION       : static void OsdKeyQueueInit(void)
//
// USAGE          : This function will flush the queue with Key_None
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
static void OsdKeyQueueInit(void)      //Flush the queue
{
   BYTE B_TempQueLn;

#if (WB_MAJOR == 0x0004)
   B_TempQueLn = OsdKeyQueueLng;
   while (B_TempQueLn)
   {
      B_TempQueLn--;
      B_OsdKeyQueue[B_TempQueLn] = Key_None;
   }
#else
   BYTE B_TempKeyType = MAX_KEY_TYPE;    //get the right index

   while (B_TempKeyType)
   {
      B_TempKeyType--;
      B_TempQueLn = OsdKeyQueueLng;
      while (B_TempQueLn)
      {
         B_TempQueLn--;
         B_OsdKeyQueue[B_TempKeyType][B_TempQueLn] = Key_None;
      }
   }
#endif
}

#if (WB_MAJOR == 0x0004)
// WB4 Functions for WB4
//******************************************************************************
//
// FUNCTION       : BYTE EnqueueOsdKey(KEY_INDEX B_DataKey)
//
// USAGE          : Enqueue an OSD key from each defined input
//
// INPUT          : B_DataKey - key that needs to be enqueued
//
// OUTPUT         : TRUE or FALSE
//
// GLOBALS        : B_OsdKeyQueue[]    (W)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BYTE EnqueueOsdKey(KEY_INDEX B_DataKey)
{
   BYTE B_OsdKeyQueueWrIdx;

   if (B_OsdKeyQueueLen >= OsdKeyQueueLng) // if queue is full write new key over oldest one
   {
      B_OsdKeyQueueRdIdx++;
      B_OsdKeyQueueRdIdx %= OsdKeyQueueLng;

      B_OsdKeyQueueLen = OsdKeyQueueLng - 1;
   }

   B_OsdKeyQueueWrIdx = B_OsdKeyQueueRdIdx + B_OsdKeyQueueLen;
   B_OsdKeyQueueWrIdx %= OsdKeyQueueLng;
   msg("enqueuing",0);
	#if 1 //st_haiwen 20120531  : the Power_key is the most priority level, None can erasure the PHYICAL_KEY_POWER 
		#if defined(CMI_73XX_BOARD) 
		if(  B_OsdKeyQueue[B_OsdKeyQueueWrIdx] !=  Key_KEY_PHYSICAL_POWER )
		#endif 
	#endif 
   B_OsdKeyQueue[B_OsdKeyQueueWrIdx] = B_DataKey;
   B_OsdKeyQueueLen++;

   if(GetPowerState() == Pwr_Transition)
   {
      if(B_DataKey != Key_None && B_DataKey != Key_KEY_PHYSICAL_POWER)
      {
         PwrTransitionKeyPressed(TRUE);
		 DDCPowerNapFlag= FALSE;
      }
   }

  	//if(UserPrefEnergySmartOnOffFlag == On)  //KevinL 20101124 reset EnergySmart timer when Key pressed
    	 //gm_TimerStart(EnergySmart_100ms_TMR, 20); 		

	return TRUE;
}

//******************************************************************************
//
// FUNCTION       : BYTE DequeueOsdKey(KEY_INDEX *Bp_DataKey)
//
// USAGE          : Dequeue Osd into B_DataKey
//
// INPUT          : Bp_DataKey - pointer to a variable to be over-written if there is new
//								key; otherwise, original value is untouched.
//
// OUTPUT         : TRUE or FALSE
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
BYTE DequeueOsdKey(KEY_INDEX *Bp_DataKey)
{
   if (B_OsdKeyQueueLen == 0)    // if queue empty return FALSE
   {
      //gm_Printf("DequeueOSD empty",0);
      return FALSE;
   }
   *Bp_DataKey = B_OsdKeyQueue[B_OsdKeyQueueRdIdx];

   B_OsdKeyQueueRdIdx++;
   B_OsdKeyQueueRdIdx %= OsdKeyQueueLng;

   B_OsdKeyQueueLen--;
   return TRUE;
}

//******************************************************************************
//
// FUNCTION       : static void InitKeypadsDrivers(void)
//
// USAGE          : Initialization of keypads drivers
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : WBK_Keypads[]
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
static void InitKeypadsDrivers(void)
{
   BYTE i;
   WBK_KEYPAD_DEFINITION  ROM *Sp_Keypad;

   for ( i = 0; i < WB_NUMBER_OF_INPUT_SOURCE; i++)
   {
      Sp_Keypad  = (WBK_KEYPAD_DEFINITION ROM*) WBK_Keypads[i];
      Sp_Keypad->InitKeypadDriver( i );
   }

   return;
}

//******************************************************************************
//
// FUNCTION       : static SBYTE CompareCodes(WORD W_TableCode, WORD W_Value, BYTE B_Delta)
//
// USAGE          : Comparing value with table code (from WB)
//
// INPUT          : W_TableCode
//                  W_Value
//                  B_Delta
//
// OUTPUT         : 0 = the table code equals value
//                  1 = the table code less value     
//                  -1 = the table code greater value
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
static SBYTE CompareCodes(WORD W_TableCode, WORD W_Value, BYTE B_Delta)//???
{
	// If W_Value is within the B_Delta range we return 0 because it matchs
	// otherwise we return 1 if table code is less than W_Value and -1 if table
	// code is greater than W_Value
	SBYTE SB_Ret = 1;
	WORD  diff;

	if (W_TableCode > W_Value)
	{
		diff = W_TableCode - W_Value;
		SB_Ret = -1;
	}
	else
		diff = W_Value - W_TableCode;

	if ( diff <= (WORD)B_Delta)
		SB_Ret = 0;

	return SB_Ret;
}


//******************************************************************************
//
// FUNCTION       : KEY_INDEX TranslateCodeToKey (BYTE  ROM *Bp_KeyValue, BYTE B_KeysNumber,
//                                              BYTE B_CodeLen, BYTE B_Delta, WORD W_Value)
//
// USAGE          : Comparing W_Value with table code by the method binary tree
//
// INPUT          : Bp_KeyValue
//                  B_KeysNumber
//                  B_CodeLen
//                  B_Delta
//                  W_Value
//
// OUTPUT         : KEY_INDEX - id pushed key. If none of the key is pressed, return value Key_None
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
KEY_INDEX TranslateCodeToKey (BYTE  ROM *Bp_KeyValue, BYTE B_KeysNumber,
                              BYTE B_CodeLen, BYTE B_Delta, WORD W_Value)
{
   BYTE i;
   for ( i = B_KeysNumber; i--; Bp_KeyValue+=(B_CodeLen + 1))
   {
      WORD W_TableCode = (B_CodeLen == 2) ? (*(WORD ROM*)Bp_KeyValue) : (*Bp_KeyValue);
      if (abs(W_TableCode - W_Value) <= (WORD)B_Delta)
         return *(Bp_KeyValue + B_CodeLen);
   }
   return Key_None;
}

//******************************************************************************
//
// FUNCTION       : void ClearOsdKeyQueue(void)
//
// USAGE          : Clears the OsdKeyQueue
//
// INPUT          : None
//
// OUTPUT         : none
//
// GLOBALS        : B_ScanCode[]        (W)
//                  B_OsdKeyQueue[]    (W)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void ClearOsdKeyQueue(void)
{   
	OsdKeyQueueInit();
	B_ScanCode = Key_None;  	
}

BYTE GetOSDQueueLen(void) //KevinL 20101124
{
   return B_OsdKeyQueueLen;
}

// WB4 End of functions for WB4
#else

//******************************************************************************
//
// FUNCTION       : BYTE EnqueueOsdKey(void)
//
// USAGE          : Enqueue an OSD key from each defined input
//
// INPUT          : None
//
// OUTPUT         : TRUE or FALSE
//
// GLOBALS        : B_AdcKeyData       (R)
//                  B_ValidIrData         (R)
//                  B_TranslatedGpioValue (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BYTE EnqueueOsdKey(void)
{
   BYTE B_OsdKeyQueueWrIdx;

   if (B_OsdKeyQueueLen >= OsdKeyQueueLng)      // if queue full return FALSE
   {
      OsdKeyQueueInit();
      B_OsdKeyQueueRdIdx++;
      B_OsdKeyQueueRdIdx %= OsdKeyQueueLng;

      B_OsdKeyQueueLen = OsdKeyQueueLng - 1;
      return FALSE;
   }
   B_OsdKeyQueueWrIdx = B_OsdKeyQueueRdIdx + B_OsdKeyQueueLen;
   B_OsdKeyQueueWrIdx %= OsdKeyQueueLng;
   msg("enqueuing",0);

#ifdef PORT_GPIO_USED
   B_OsdKeyQueue[GPIO_KEY][B_OsdKeyQueueWrIdx] = B_TranslatedGpioValue;
#endif
#ifdef PORT_ADC_1_USED
   B_OsdKeyQueue[ADC1_KEY][B_OsdKeyQueueWrIdx] = B_AdcKeyData[0];
#endif
#ifdef PORT_ADC_2_USED
   B_OsdKeyQueue[ADC2_KEY][B_OsdKeyQueueWrIdx] = B_AdcKeyData[1];
#endif
#ifdef PORT_ADC_3_USED
   B_OsdKeyQueue[ADC3_KEY][B_OsdKeyQueueWrIdx] = B_AdcKeyData[2];
#endif
#ifdef PORT_IR_USED
   B_OsdKeyQueue[IR_KEY][B_OsdKeyQueueWrIdx] = gmvb_ValidIrData;
#endif

   B_OsdKeyQueueLen++;

   return TRUE;
}

//******************************************************************************
//
// FUNCTION       : BYTE DequeueOsdKey(void)
//
// USAGE          : Dequeue Osd into the B_ScanCode Array - which signifies the
//                  combination of keys being pressed at the same time.
//
// INPUT          : None
//
// OUTPUT         : TRUE or FALSE
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
BYTE DequeueOsdKey(void)
{
   if (B_OsdKeyQueueLen == 0)    // if queue empty return FALSE
   {
      return FALSE;
   }

#ifdef PORT_GPIO_USED
   B_ScanCode[0] = B_OsdKeyQueue[GPIO_KEY][B_OsdKeyQueueRdIdx];
#endif
#ifdef PORT_ADC_1_USED
   B_ScanCode[1] = B_OsdKeyQueue[ADC1_KEY][B_OsdKeyQueueRdIdx];
#endif
#ifdef PORT_ADC_2_USED
   B_ScanCode[2] = B_OsdKeyQueue[ADC2_KEY][B_OsdKeyQueueRdIdx];
#endif
#ifdef PORT_ADC_3_USED
   B_ScanCode[3] =  B_OsdKeyQueue[ADC3_KEY][B_OsdKeyQueueRdIdx];
#endif
#ifdef PORT_IR_USED
   B_ScanCode[4] =  B_OsdKeyQueue[IR_KEY][B_OsdKeyQueueRdIdx];
#endif

   B_OsdKeyQueueRdIdx++;
   B_OsdKeyQueueRdIdx %= OsdKeyQueueLng;

   B_OsdKeyQueueLen--;
   return TRUE;
}

#endif

/**********************************  END  *************************************/
