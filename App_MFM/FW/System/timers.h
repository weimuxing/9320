/*
	$Workfile:   timers.h  $
	$Revision: 1.40 $
	$Date: 2013/06/04 03:34:24 $
*/
#ifndef __TIMER_H__
#define __TIMER_H__
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
// MODULE:		timers.h
//
// USAGE:		Header file for application timers definition
//
//******************************************************************************

//#define DEBUG_BUFFER_ON
//******************************************************************************
//  E X T E R N S
//******************************************************************************

#ifdef DEBUG_BUFFER_ON
extern WORD Wa_Buffer[75];
extern BYTE B_BufferIndex;
extern BYTE B_DebugIndex;
#endif

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************
//
// Applicaiotn Timer1 timeout resolution
//
#define AppsTimer_1ms	1					//
#define AppsTimer_2ms	2					//
#define AppsTimer_4ms	4					//

//
// Application timer definition (using system Timer2)
//
enum
{
   //
   USER_START_1ms = GSEL_END_1s,			// Start of 1ms
   APPS1_1ms_TMR = USER_START_1ms,		// APPS_1ms_TMR (to be renamed)
//	APPS2_1ms_TMR,							// e.g. next timer start
   H2CLINK_TMR,
   SRAM_TABLE_READ_TIMEOUT,

//----ATTENTION!----values from SMT_TIMEOUT_0_TMR to SMT_TIMERS_END are reserved for SMT transactions timeout timers
#ifdef USE_SMT
   SMT_TIMEOUT_0_TMR,
   SMT_TIMERS_END = SMT_TIMEOUT_0_TMR + MAX_TRANSACTION_NUMBER -1,
   SMT_MASTER_TIMEOUT_0_TMR,
   SMT_MASTER_TIMERS_END = SMT_MASTER_TIMEOUT_0_TMR + MAX_SMT_MASTER_TRANSACTIONS - 1,
#endif
	#ifdef DELL_U2410F
	SPLASHSCREEN_1ms_TMR, //For DELL OSD Mikhail 08.08.2008
	#endif
	DP_ShortPulse_1ms_TMR, // 20090402 #4, reject HPD in 120ms, if HPD already set. 
	DP_ShortPulseWaitTime_1ms_TMR,	// 20090708 #7, avoid short pulse duriong short time SL lost link. Recommend from Pei-Hsin.
/*ChengLin 20130425: Enable PBP Deep Sleep Check*/
#ifdef ST_4K2K_93xx_BOARD
   #if ENABLE_PBP_ATHENA_L_DEEP_SLEEP_CHECK   
   PBP_SLEEP_CHECK_TIMEOUT_1ms_TMR,
   #endif
   #if ENABLE_TWO_LOGICAL_PORTS_SLEEP_CHECK   
      DP_TWO_LOGICAL_PORTS_SLEEP_CHECK_TIMEOUT_1ms_TMR,
   #endif   
   SOCIPC_SENDCMD_WAIT_1ms_TMR,
#endif     
   USER_END_1ms,							// End of 1ms
   //
   USER_START_10ms = USER_END_1ms,		// Start of 10ms
   KEYSCAN_10ms_TMR = USER_START_10ms,	// Key sacn interval
   AUTOSCAN_CHA_10ms_TMR,				// Modehandler Auto scan timers
   AUTOSCAN_CHB_10ms_TMR,				// - CHA and CHB
   TV_AUTOSCAN_10ms_TMR,				// Auto scan timer for tuner
   MULTIPIP_10ms_TMR,
   DEEP_SLEEP_10ms_TMR,						// 100ms Deep Sleep timer
#ifdef DELL_U2410F						
       AudioPCM_10ms_TMR,                                      //owen 120920
	ScanSource_10ms_TMR,	
#endif
   FWD_MSG_RESPONSE_10MS_TMR,
   NVRAM_Timer_10ms_TMR,
#if (FEATURE_HDMI==ENABLE)
   HDMI_HPD_TIME_Main_10ms_TMR,
   HDMI_HPD_TIME_Pip_10ms_TMR,
   HDMI_POWERON_READY_10ms_TMR,
   HDMI_SIG_STABLE_Main_10ms_TMR,   //HDMI_PKT_ERROR_HPD =1
   HDMI_SIG_STABLE_Pip_10ms_TMR,    //HDMI_PKT_ERROR_HPD =1
#endif
#ifdef DELL_U2410F
	AUDIO_WAIT_10ms_TMR,					//Neil 20090416+ 
	Display_Enable_Main_10ms_TMR, 				//Neil120801+
	Display_Enable_Pxp_10ms_TMR, 				//Neil120801+

//#endif
	DetectTouchPad_10ms,  //calotest
//#ifdef U2410_HIGH_DCR	// Increased by ST_hansPENG 090311
	DCR_CHECK_10ms_TMR,
#endif
#ifdef USER_CMD
	UC_UART0_10ms_TMR,
	UC_UART1_10ms_TMR,
#endif
#if (FEATURE_DISPLAY_PORT==ENABLE)
   DP1_AUX_TRIGGER_10ms_TMR,
   DP1_SRCPWR_OFF_DEBOUNCE_10ms_TMR,
   DP2_AUX_TRIGGER_10ms_TMR,
   DP2_SRCPWR_OFF_DEBOUNCE_10ms_TMR,
   RX0_HPD_OVERRIDED_10ms_TMR,
   RX1_HPD_OVERRIDED_10ms_TMR,   
   RX0_PHYON_NOTIFY_10ms_TMR,
   RX1_PHYON_NOTIFY_10ms_TMR,   
   DPTX_AUD_MONITOR_10ms_TMR,       // #if (PATCH_DPTX_NO_AUD_BY_ZERO_OF_SDP_AUD_M_MEAS !=0)
#endif
   /*ChengLin 20130129: Enable Bidirectional BUSY Status Check*/   
#if (defined(ST_4K2K_93xx_BOARD) && ENABLE_BIDIRECTIONAL_BUSY_CHECK)
	SOCIPC_LEFT_FE_RIGHT_SYNC_WAIT_10ms_TMR,
#endif
   /*ChengLin 20130222: For 4k2k board Image Sync Handler Time out handler*/  
#if (defined(ST_4K2K_93xx_BOARD) && ENABLE_ATHENA_R_L_IMAGE_SYNC_HANDLER)
	IMAGE_SYNC_HANDLER_TIMEOUT_10ms_TMR,
#if ENABLE_DP_MST_TIMEOUT_CHECK_FOR_OWN_READY_BIT	
	IMAGE_SYNC_DP_MST_TIMEOUT_10ms_TMR,	
#endif	
#endif
#if (defined(ST_4K2K_93xx_BOARD) && ENABLE_VEGA_AVAIL_CHECK_VIA_FE_AC_ON)
   VEGA_AVAIL_CHECK_VIA_FE_10ms_TMR,
#endif
#ifdef ST_4K2K_93xx_BOARD   
   Panel_PWR_10ms_TMR, 
   Panel_BKL_10ms_TMR,
   Panel_PWR_OFFWAIT_10ms_TMR,
   AthenaL_SourceChange_Timeout_10ms_TMR,
   AthenaR_GetFEInfo_Timeout_10ms_TMR,
/*ChengLin 20130425: Enable PBP Deep Sleep Check*/
   PBP_DEEP_SLEEP_TIMEOUT_10ms_TMR,
   #if ENABLE_TWO_LOGICAL_PORTS_SLEEP_CHECK   
   TWO_LOGICAL_PORTS_DEEP_SLEEP_TIMEOUT_10ms_TMR,
   #endif   
   #ifdef DP_HUB_2_LOG_PORTS
   DpRx0_WaitMstInfoTimeOut_10ms_TMR,
   DpRx0_EnableReModeSetupCheck_10ms_TMR,   
   #endif
#endif

   USER_END_10ms,						// End of 10ms
   //
   USER_START_100ms = USER_END_10ms,		// Start of 100ms
   DEBUG_100ms_TMR = USER_START_100ms,	// Debug Handler for message display
   TV_BACKGROUNDAFT_100ms_TMR,			// Background auto fine tune timer for PAL tuner
   PWR_TRANSITION_100ms_TMR,			// Power transition
	HDMI_Recover_Patch_100ms_TMR, // Louis 20120920 ... HDMI patch for 7770, 7870, 7970  
	DP_Recover_100ms_TMR,			//Hyb140327+ for DP signal lost when Pxp with DIP decoder input;
   MULTIPIP_REFRESH_100ms_TMR,			// Refresh timer for multi pip
#if (defined(SMART_ISP) && defined(SMART_ISP_AUTO_DETECTION))
   SMART_ISP_DET_100ms_TMR,
#endif
   DEEP_SLEEP_100ms_TMR,						// 100ms Deep Sleep timer
   DP_PANEL_PWR_ON_WAIT_100ms_TMR,
   BLINKING_LED_TMR,
   LED_BLINKING_100ms_TMR,
#ifdef DELL_U2410F
    BURNIN_100ms_TMR,
    #ifdef INPUTSCAN
	INPUT_SCAN_TMR,						//For input scan timer
	#endif
    SystemPowerOn_TMR, //KevinL 20090112	
    #ifdef CHECK_PxP_STATES_AC_ON
    MainNoSyncCheck_100ms_TMR, //KevinL 20090325 for power on to determine PIP/PBP should be turned on or not.	
    #endif
	#ifdef USE_TIMER_CTRL_AUD_PWR
	AudioSpeakerPowerUp_100ms_TMR,		//Neil 20090218+ for audio power up from power saving pop noise;
	#endif
	CableDection_100ms_TMR, //KevinL for Innolux board	
    NoCablePowerDown_100ms_TMR, 	
    RotationSensor_100ms_TMR,			//Neil120717 move here
    WaitDelay_100ms_TMR,			//120921 Edward for scan source test
    //EnergySmart_100ms_TMR, //KevinL 20101116
#endif
   LPM_CHECK_STATUS_100ms_TMR,
#if (FEATURE_ACR == ENABLE)
   ACR_ADJUST_100ms_TMR,
#endif
#if (FEATURE_VWD == ENABLE) // BLACK_BORDER_FIX
   VWD_DITHER_CHECK_100ms_TMR,
#endif
	#if(defined(BOARD_93XX_4K2K))
	ZDT_4K2K_Reset168_100ms_TMR,
	#endif
   USER_END_100ms,						// End of 100ms
   //
   USER_START_1s = USER_END_100ms,		// Start of 1s
   APPS1_1s_TMR = USER_START_1s,			// APPS_1ms_TMR (to be renamed)
#if (defined(SMART_ISP) && defined(ST_4K2K_93xx_BOARD))
   SMART_ISP_SLAVE_COMM_1s_TMR,
#endif   
   TV_CH_NAME_1s_TMR,					// Channel name timer used for tuner app
   MULTIPIP_1s_TMR,
   DEEP_SLEEP_1s_TMR,							// 1s Deep Sleep timer
#ifdef DELL_U2410F
	HardLock_1s_TMR, 					//For DELL OSD Mikhail 08.08.2008											
  	DiagnosticTestPattern_1s_TMR,
	Monitor_OnTime_1s_TMR,
	FactoryModeAdmit_1s_TMR, 			//KevinL 20081022
	PipMessage_1s_TMR, 					//KevinL 20090506
    ACCUpdateCheck_1s_TMR, 				//KevinL 20090513
	HIGH_DCR_ACTIVATION_1s_TMR,
	HIGH_DCR_PANEL_WARM_UP_1s_TMR,
	HIGH_DCR_TEST_1s_TMR,  				//KevinL 20090115 for DELL DCR Test	
	HIGH_DCR_BlackScreen_1s_TMR,		//120924 Edward for black screen check		
#ifdef VWD_FULL_WINDOW_CHECK //KevinL 20120922 	
	VWD_FULL_WIDNOW_1S_TMR,
#endif //#ifdef VWD_FULL_WINDOW_CHECK 		
#endif
	   Key_Check_1s_TMR,
      

   DPCD_SINK_TEST_1s_TMR,              // ENABLE_DPCD_SINK_TEST_SLEEP_DELAY
#ifdef TTX_STATUS_DISPLAY
   TTX_STATUS_DISPLAY_TMR,				// Ttx Status Display timer
#endif
   PanelProtect_1s_TMR,                     //wei 140530 for 1 second timer
//---------------------
   USER_END_1s							// End of 1s
};

//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E
//******************************************************************************

void UserTimerInit(void);
void ApplicationTimer1Init(WORD timeout);

#ifdef DEBUG_BUFFER_ON
void PrintFromBufferDex		(void);
void PrintFromBufferHex		(void);
#endif

#endif
/*********************************  END  **************************************/
