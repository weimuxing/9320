/*
   $Workfile:   osd_adj.c  $
   $Revision: 1.70 $
   $Date: 2013/06/14 02:38:02 $
*/
#define __OSD_ADJ_C__
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
// MODULE:        osd_adj.c
//
// USAGE:      This module contains miscelaneous osd adjusters like:
//            RequestSoftPowerOn/Off, FactoryReset, DynamicScale, etc.
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#ifndef LCDTV_PAL
#include <string.h>    // Needed for memset in FactoryReset in NTSC case
#endif

#include "System\All.h"
#include "StdLibInc\gm_Register.h"
#include "Board\PortAttrib.h"


//******************************************************************************
//  E X T E R N S
//******************************************************************************
extern WORD   W_TimeKeyHeldDown; //Extern from osd_input.c and used for Ballistic Acceleration
extern BOOL B_MainCcFreezedFlag;
extern BOOL B_PipCcFreezedFlag;
extern PortSchemeDependentUserPrefType ROM SchemeFactoryDefaultInROM[NUM_IMAGE_SCHEMES]; // PSD
extern BOOL IsPortConflict(gmt_UserDefConnNames Port1, gmt_UserDefConnNames Port2);//120502 Edward merge 2410

//******************************************************************************
//  D E F I N E S
//******************************************************************************

#define _DEBUG

#ifdef _DEBUG
#define  msg(a)                  gm_Print(a,0)
#define  msg_i(a,b)               gm_Print(a,b)
#else
#define  msg(a)
#define  msg_i(a,b)
#endif


#define   MAX_ADJUST_VALUE         255         // max adjust value
//
// decimal base system
//
#define NUMBER_SYSTEM_BASE         10


//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************
WORD Temp_MVF_Hi = 0;
//******************************************************************************
// S T A T I C   F U N C T I O N S
//******************************************************************************

//******************************************************************************
//  C O D E
//******************************************************************************

//******************************************************************************
//
// FUNCTION       : BYTE GetChipID(void)
//
// USAGE          : Returns the Chip ID
//
// INPUT          : None
//
// OUTPUT         : Chip ID
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
BYTE GetChipID(void)
{
#if (CHIP_ID == STDP93xx)
   return STDP93xx;
#endif
}
//******************************************************************************
//
// FUNCTION       : WORD BallisticAcceleration(void)
//
// USAGE          : Ballistic acceleration of auto repeat for IR remote buttons
//
// INPUT          : None
//
// OUTPUT         : Step size for adjuster
//
// GLOBALS        : W_TimeKeyHeldDown      (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
WORD BallisticAcceleration(void)
{
   if (W_TimeKeyHeldDown < BALLASTIC_ACCELERATION_SPEED)
      return 1;
   else
      return (WORD)(W_TimeKeyHeldDown/BALLASTIC_ACCELERATION_SPEED);
}

#ifdef Action_SaveGrModeDependentSettings_WB_Used
//******************************************************************************
//
// FUNCTION       : void SaveGrModeDependentSettings(void)
//
// USAGE          : If source input for Main is BOARD_VGA, then SaveModeDependentSettings
//                  is invoked to save mode dependent settings for channel A. Save mode
//                  dependent settings for IGP channel in NVRAM
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : ModeIndependentUserPreferences.CurrentInputMain   (R)
//                  gmvb_MainChannel                        (R)
//                  gmvw_InputHFreq                           (R)
//                  gmvw_InputVFreq                           (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void SaveGrModeDependentSettings(void)
{
#if FEATURE_AIP == ENABLE
   ModeDependentLookUpType LookUpType;

   if (IsInputVGA(gmvb_MainChannel))
   {
      LookUpType.W_Hfreq = stInput[gmvb_MainChannel].Hfrq;
      LookUpType.W_Vfreq = stInput[gmvb_MainChannel].Vfrq;
      LookUpType.W_Vtotal = stInput[gmvb_MainChannel].Vtotal;

      // Save Updated Values to NVRAM
      SaveModeDependentSettings(gmvb_MainChannel, &LookUpType);
   }
#endif
}
#endif //#ifdef Action_SaveGrModeDependentSettings_WB_Used

//******************************************************************************
//
// FUNCTION       : void RequestSoftPowerOff()
//
// USAGE          : Request power off to power handler.
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : UserPrefWakeHour (R), UserPrefWakeAMPM(R), UserPrefWakeAMPM(R)
//                  UserPrefWakeDay (R), B_PowerModeSetupReq
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void RequestSoftPowerOff()
{
   AUDIO_MUTE();
   gm_DisplayOnOff(OFF);
   SetDisplayOff(gmvb_MainChannel, BACKGROUND_ONLY);
   if(Is_PIP_ON())
   	SetDisplayOff(gmvb_PipChannel, BLENDER_OFF);		//Hyb140730+ for Power off Pxp off first
   
   UserPrefFactoryFlag = FALSE;
   UserPrefRunBurnInModeFlag = gmd_FALSE;   
   SaveModeIndependentSettings();
   PowerRequestOff();
}

//******************************************************************************
//
// FUNCTION       : void RequestSoftPowerOn()
//
// USAGE          : Request power on to power handler.
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : B_PowerModeSetupReq
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void RequestSoftPowerOn()
{
   PowerRequestOn();
}

//******************************************************************************
//
// FUNCTION       : BOOL IsVideoEnhancementValid (gmt_VIDEO_PROCESSING D_Mode)
//
// USAGE          : Call this function to check if the video enhancment is valid
//                  for current input. This function will only be called by OSD WB.
//
// INPUT          : D_Mode - VIDEO_MADI
//                    VIDEO_DCDI
//                    VIDEO_FILMMODE
//                    VIDEO_CCS
//                    VIDEO_TNR
//                    VIDEO_MNR
//                    VIDEO_GNR
//                    VIDEO_ACC
//                    VIDEO_ACM
//                    VIDEO_HEM
//
// OUTPUT         : TRUE or FALSE
//
// GLOBALS        : None
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BOOL IsVideoEnhancementValid (gmt_VIDEO_PROCESSING D_Mode)
{
   if (gm_VideoProcessingValidate (D_Mode) &  D_Mode)
      return TRUE;
   else
      return FALSE;
}

#ifdef Action_FactoryReset_WB_Used
//******************************************************************************
//
// FUNCTION       : void FactoryReset(void)
//
// USAGE          : Reset mode independent and mode dependent settings
//                  to factory default values. Input and output of both channels
//                  A and B are turned off. Output format is set based on paneltype
//                  OSD active channel is set to Main. Mode indepedent variable
//                  ModeIndependentUserPreferences is initialized to rom setting
//                  variable FactoryDefaultInROM. Graphics channel is chosen as
//                  default for Main and video for PIP. Select source inputs for
//                  Main and PIP. set Window output H,VStart, Height and Width for
//                  each channel.Reset and adjust mode dependent settings for ADC
//                  input. Power up the   system and both channels. the Hpos, Vpos,
//                  Htotal, Hphase are determined by mode setup. Reset mode
//                  independent and mode dependent settings.
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : FactoryDefaultInROM   (R)
//                  gmvb_MainChannel   (R)
//                  gmvb_PipChannel      (R)
//                  St_ColorAdjust      (W)
//                  ModeIndependentUserPreferences (W) - mode independent preferences
//                  UserPrefCurrentInputMain      (R)
//                  UserPrefCurrentInputPip         (R)
//                  UserPrefOsdLanguage            (R)
//                  UserPrefBrightness            (R)
//                  UserPrefContrast            (R)
//                  UserPrefSaturation            (R)
//                  UserPrefHue                  (R)
//                  UserPrefHStart              (W)- setting for HStart
//                  UserPrefVStart              (W)- setting for VStart
//                  UserPrefHSyncPhase            (W)- setting for Phase
//
// USED_REGS      : NONE
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void FactoryReset(void)
{
   BYTE B_Temp;
   BYTE i;

#ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()==CHIPID_4K2K_R)
   {
#if ENABLE_SOC_IPC_BUSY_CHECK
	/*ChengLin 20130124: New 4k2k board BUSY status check*/
		if(!SocIPCIsSlaveLeftReady())
			return;	
#endif
   	SOCIPC_SyncOSDAdjustHandler( SI_OSD_SYNC_SETUP);
 	}
#endif

#ifdef DELL_U2410F
	BYTE UserInputMain = ALL_BOARDS_DP1;//UserPrefCurrentInputMain;
	BYTE UserInputPip = UserPrefCurrentInputPip;	//20120912 yvonne for pip source keep not reset
	/*BYTE B_93_R = UserPrefColorT9300_R;
	BYTE B_93_G = UserPrefColorT9300_G;
	BYTE B_93_B = UserPrefColorT9300_B;
	BYTE B_65_R = UserPrefColorT6500_R;
	BYTE B_65_G = UserPrefColorT6500_G;
	BYTE B_65_B = UserPrefColorT6500_B;
	BYTE B_57_R = UserPrefColorT5700_R;
	BYTE B_57_G = UserPrefColorT5700_G;
	BYTE B_57_B = UserPrefColorT5700_B;	
	WORD W_TimeUsedHour = UserPrefTimeUsedHour;
	BYTE B_TimeUsedMin = UserPrefTimeUsedMin;
	*/
	BYTE B_OSDLanguage=UserPrefOsdLanguage;	
#endif
	B_NoMainIconFlag = gmd_TRUE;
#if 0 // not support RTC
   // Reset Real time clock to Jan 1st, 2000, 12:00am 0 sec
   SetRealTimeClockDate(1);
   SetRealTimeClockMonths(1);
   SetRealTimeClockYears(0);
   SetRealTimeClockDay(1);
   SetRealTimeClockMinutes(0);
   SetRealTimeClockHour(0, AMPM_MODE, AM_MODE);
   SetRealTimeClockSeconds(0);
   B_Temp = ReadFromRealTimeClock(RTC_CONTROL);
   B_Temp &= ~(RTC_CTRL_EOSC|RTC_CTRL_BBSQI|RTC_CTRL_A1IE|RTC_CTRL_A2IE);
   B_Temp |= (RTC_CTRL_INTCN);         //enable interrupt
   WriteToRealTimeClock(RTC_CONTROL, B_Temp);
#endif


   if(UserPrefVwdFlag != OFF) //KevinL 20120920 Stop VWD for color space setting incorrect issue that condition related to ACM enable. 
   {    
       VWD_Stop();
       gm_ClearRegBitsWord(VIDEO_WIN_CTRL, VIDEO_WIN_EN); //KevinL 20120923 clear Video Window for incorrect color space issue. 
   }

   //Neil 20081114+ for factory reset turn pip off, Buglist 192
   if(UserPrefPipMode > NO_PIP)
   {
	   UserPrefPipMode = NO_PIP;
	   AdjustPipMode();
	   SetPipBorder(gmvb_PipChannel, FALSE);
   }
   //Neil120809+ for audio no output after factory reset
   AUDIO_MUTE();
   //gm_AUDIO_ReStartDetection();

   ModeSetupFreerun(gmvb_MainChannel);
   ModeSetupFreerun(gmvb_PipChannel);
/*ChengLin 20130314: Reset "UserPrefCaptureM" to default value*/
#ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()==CHIPID_4K2K_FE)
      UserPrefCaptureM = CAPTURE_MODE_FULL;
   else if(GetChipID_4K2K()==CHIPID_4K2K_R)
      UserPrefCaptureM = CAPTURE_MODE_RIGHT;
   else if(GetChipID_4K2K()==CHIPID_4K2K_L)
      UserPrefCaptureM = CAPTURE_MODE_LEFT;
#endif
   if(UserPrefFactoryFlag == gmd_TRUE)
   {
   		//yvonne 20120828 dell spec to clear CAL1/CAL2 NVRAM 
	   if(NVRAM_TableCheck(NVRAM_Table_Gamma,2)|| NVRAM_TableCheck(NVRAM_Table_Gamma,3))
	   {
	   		gm_Print("##Reset NVRAM Calx",0);
	   		NVRam_WriteBuffer_Clear_NVRAM(C_GAMMA_COMPONENT_SAVED_START_OFFSET+0x2000*2,(WORD)0x4000);
	   	}
		//dell spec to clear NVRAM Uniformity2
	   //if((NVRam_ReadByte(UniformityRegisterSettingStart2)&BIT0) && (NVRam_ReadByte(UniformityRegisterSettingStart2) != 0xFF))
	   if(NVRAM_TableCheck(NVRAM_Table_DUM, UNIFORMITY_Table2))
		{
			gm_Print("##Reset NVRAM Uniformity",0);
	   		NVRam_WriteBuffer_Clear_NVRAM(UniformityRegisterSettingStart2,(WORD)0x2100);
	   	}
   	}
   // reset and adjust mode dependent settings only for ADC input

#if FEATURE_AIP == ENABLE
   if (UserPrefCurrentInputMain == ALL_BOARDS_VGA1)
   {
      // Re-initialize mode dependent descriptor and lookup table.
      InitModeDependentDescr();
      InitModeDependentLookUp();

      UserPrefHStart = gmvw_InputHStartOrg[gmvb_MainChannel];
      UserPrefVStart = gmvw_InputVStartOrg[gmvb_MainChannel];
      UserPrefHTotal = gmvw_InputHTotalOrg[gmvb_MainChannel];
      UserPrefHSyncPhase = 0;
      UserPrefHSyncDelay = 0;
      UserPrefVSyncDelay = 0;
   }
#endif

   //120831 Disable game first if reset at game mode enable , otherwise system will no image.
   gm_Print("@@# game mode %d",UserPrefGameMode);
   if(UserPrefGameMode == On)
   {
		UserPrefGameMode = Off;	
		gm_WriteRegWord(SYNC_CONTROL, 2); 							  
		#if (ENABLE_DFL == 1) 
			B_DisplayLockMethod = DISP_LOCK_DFL;
			//gm_Printf("~~GameModeControl~~ Change Back to DFL ~~~~", 0);
		#else
			B_DisplayLockMethod = DISP_FREERUN;
			//gm_Printf("~~GameModeControl~~ Change Back to Free Run ~~~~", 0);		  
		#endif
		gm_SetRegBitsWord(DDDS1_CONTROL, DDDS1_FORCE_OPLOOP);		    
		UserPrefStateTempFlag = INPUT_COLOR_FORMAT_STATE;
		gm_Print("@@#Reset game mode",0);
	}	
	
   // Re-initialize mode independent settings in ROM
   ReadModeIndependentSettings();
   
#if 1//(DEFAULT_PANEL != LG_WQHD_LM270WQ1)
   if(UserPrefDPSWitch != FactoryDefaultInROM.DPSWitch)
	  AdjustDPSwitch();//120808 Edward restore DP setting
#endif	

   ModeIndependentUserPreferences = FactoryDefaultInROM;



   // Re-initialize port dependent settings in ROM
   InitPortSchemeDependentDescr();
   InitPortSchemeDependent();
   #if 0//done in AdjustPresetMode
   { // To reset 6-axis
      AdjustImageScheme();
      
      #if (FEATURE_COLORWARP == REV1)
      for(i=0; i<6; i++)
      #else
      for(i=0; i<8; i++)
      #endif
      {
         #if (FEATURE_COLORWARP == REV1)
         AdjustSixAxisColor(i);  
         #else // (FEATURE_COLORWARP == REV2)
         AdjustColorWarp(i);
         #endif // (FEATURE_COLORWARP == REV1)
      }
      
#if (FEATURE_DUM == ENABLE)
	  //UserPrefDUMSwitch = UNIFORMITY_OFF;	   //Neil120703+ Set to off after factory reset. //120806 Edward already implement in the NVRAM setting
      DisplayUniformityModule(UserPrefDUMSwitch, FALSE, 1, 1, PanelWidth, PanelHeight);      
#endif
   } 
   #endif
         UserPrefAudioDownMix = 0;//Barbara Lin;20121210;Audio level became half of the original level when AudioDownMix was on.
 
#ifdef DELL_U2410F 
	   //Factory mode, reset Main&Pxp port, and language;
	   if(UserPrefFactoryFlag == gmd_FALSE)
	   {
		   UserPrefCurrentInputMain = UserInputMain;
		   UserPrefCurrentInputPip = UserInputPip;	//20120912 yvonne for pip source keep not reset
		   #if 0
		   if (IsPortConflict(UserPrefCurrentInputPip, UserPrefCurrentInputMain)) //KevinL 20081216
			   UserPrefCurrentInputPip =  GetNextPipInputPort(UserPrefCurrentInputPip);
		   #endif
		   UserPrefOsdLanguage = B_OSDLanguage;    
	   	}
	#if 1	// Increased by ST_hansPENG 090209
	   UserPrefImageScheme = InputConvertScheme(UserPrefCurrentInputMain);
	   UserPrefPipImageScheme = InputConvertScheme(UserPrefCurrentInputPip);
	   InitPresetModesDependent();	
	   ReadPortSchemeDependentEntry(UserPrefImageScheme); // PSD
	#endif
#else	

   #if FEATURE_AIP == ENABLE
       UserPrefCurrentInputMain = ALL_BOARDS_VGA1;
   #else
       UserPrefCurrentInputMain = ALL_BOARDS_HDMI;
   #endif // FEATURE_AIP

	#ifdef EXTERNAL_DECODER
       UserPrefCurrentInputPip = ALL_BOARDS_CVBS1;
	#else
	UserPrefCurrentInputPip = ALL_BOARDS_DP1;
	#endif
#endif
	UserPrefPortAspectRatio = AR_FULL_SCREEN;
	UserPrefPortTNR = TNR_OFF;
	UserPrefGammaTable = GAMMA_TABLE_OFF;

	//Hyb140118+ factory reset
	if(IsFactoryMode())
	{
		UserPrefFactoryFlag = FALSE;
		UserPrefRunBurnInModeFlag = gmd_FALSE;
		UserPrefBoardID =  WB_BOARD_ID_DEF;
		UserPrefLOGO_Ctrl = WB_LOGO_DEF;
		UserPrefUartBaudRate = WB_BAUDRATE_DEF;
		UserPrefPanelSelect = DEFAULT_PANEL;
		
		#ifdef TOUCH_KEY
		{
			BYTE Buff_Osd[5];

			B_SplashComplete = TRUE;

			Buff_Osd[0] = 0xED;
			Buff_Osd[1] = B_SplashComplete;
			Buff_Osd[2] = UserPrefFactoryFlag;
			Buff_Osd[3] = Pwr_On;// PwrState;
			Buff_Osd[4] = ZERO_KEYSTATE;//MenuKeyState;
			if ( gm_MessageSend(gmd_MESSAGE_USER, &Buff_Osd, sizeof(Buff_Osd)) )
			{
				#ifdef UART_DEBUG_MODE
				gm_Print("Buff_Osd[0] = %d",Buff_Osd[0]);
				gm_Print("Buff_Osd[1] = %d",Buff_Osd[1]);
				gm_Print("Buff_Osd[2] = %d",Buff_Osd[2]);
				gm_Print("Buff_Osd[3] = %d",Buff_Osd[3]);
				gm_Print("Buff_Osd[4] = %d",Buff_Osd[4]);
				#endif
			}
			else
			{
				#ifdef UART_DEBUG_MODE
				gm_Print("==> MenuKeyState send failed",0);
				#endif
			}
		}
		#endif
	

	}
	
#ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()==CHIPID_4K2K_FE)
   {
		if( UserPrefCurrentInputMain != ALL_BOARDS_HDMI)
		   gmvb_InputConnector[gmvb_MainChannel] = UserPrefCurrentInputMain = ALL_BOARDS_HDMI; //Default Source	
	}  
   else if(GetChipID_4K2K()==CHIPID_4K2K_R)
   { 
      gmvb_InputConnector[gmvb_MainChannel] = UserPrefCurrentInputMain = ALL_BOARDS_DP2; //HDMI path
      
      if(UserPrefCaptureM != CAPTURE_MODE_HALF_HORIZONTAL)
	   {
	      BYTE TempPort=UserPrefCurrentInputMain;
		   SocIPCGetSlavePortStatus(SOCIPC_DEVICEL_DST_ADDRESS);
		   gm_Delay10ms(1);
/*ChengLin 20130222: Athena Right and Left Must black screen at the same time*/		
#if ENABLE_ATHENA_R_L_IMAGE_SYNC_HANDLER
		   SOCIPC_SetAthena_Right_Left_PB_Ctrl_Status(FALSE);
		   gm_Delay10ms(1); //Added by ChengLin 20130306 for avoid below I2C warite command failure
#endif
		   gm_ClearRegBitsWord(PB_CTRL, PB_MAIN_ENABLE);
		   gm_SetRegBitsWord(PB_CTRL, PB_FORCE_MAIN_BKGND);
		   gm_ForceUpdate(CH_A , INPUT_OUTPUT_CLOCK);

		   if(UserPrefCurrentInputMain == ALL_BOARDS_DP3)
		      UserPrefCurrentInputMain = ALL_BOARDS_DP1;

		   while(gAthena_LEFT_FE_Slave_Parameters.PortStatus != UserPrefCurrentInputMain)
		   {
			   SOCIPC_SyncOSDAdjustHandler( SI_OSD_SYNC_SEL_INPUT_MAIN );
			   //gm_Print("L port is %d", gAthena_LEFT_FE_Slave_Parameters.PortStatus);
			   gm_Delay10ms(1);
			   SocIPCGetSlavePortStatus(SOCIPC_DEVICEL_DST_ADDRESS);			
		   }
         UserPrefCurrentInputMain = TempPort;
  	  }
   }
   else if(GetChipID_4K2K()==CHIPID_4K2K_L)
   {
      gmvb_InputConnector[gmvb_PipChannel] = UserPrefCurrentInputPip = ALL_BOARDS_DIP1;
      if(UserPrefDPSWitch == DP_Input_11)
         gm_SetRegBitsWord(DISPLAY_CONTROL, D2PIXWIDE);
      else
         gm_ClearRegBitsWord(DISPLAY_CONTROL, D2PIXWIDE);
   }
#elif defined(FEATURE_HOSTIF)
   //Set current input main/pip here if necessary....refer to Athena_4K2K_Chip_L, Athena_4K2K_Chip_FE
#endif //ST_4K2K_93xx_BOARD

#ifdef ST_4K2K_93xx_BOARD
   /*ChengLin 20130314: Athena Right also to chnage Athena Left source, Athena Left don't need to execute switch function here*/
   if(GetChipID_4K2K() == CHIPID_4K2K_FE || GetChipID_4K2K() == CHIPID_4K2K_R)
      SelectInput(gmvb_MainChannel, UserPrefCurrentInputMain, NORMAL_MODE); 
#endif
#if 0
   // the input configuration
   SelectInput(gmvb_MainChannel, UserPrefCurrentInputMain, NORMAL_MODE);
   //gm_ModeDetectionConfig(B_Channel, DET_RESTART);
#ifndef NO_PIP_SUPPORT
   SelectInput(gmvb_PipChannel, UserPrefCurrentInputPip, NORMAL_MODE);
#endif
   //gm_ModeDetectionConfig(B_Channel, DET_RESTART);
#endif

   //adjust all mode independent settings accordingly
   gm_SetOsdLanguage(UserPrefOsdLanguage);
   //CscUpdateColors(gmvb_MainChannel);//done in AdjustPresetModes()
   //CscUpdateColors(gmvb_PipChannel);   
   AdjustPresetModes();
   AdjustSharpness();
   //AdjustDPSwitch();//120808 Edward restore DP setting

#if (FEATURE_CCS == ENABLE)  
   gm_VideoProcessingDisable(VIDEO_CCS);
#endif

#if 0
   AdjustColorTemp();

   // Done by ModeSetup
   AdjustPipMode();
   AdjustFilmMode();
   AdjustDCDi();
   Adjust3DNR();
   AdjustMPEGNR();
   AdjustAspectRatioMain();
   AdjustAspectRatioPip();
 #endif
   
#ifdef DELL_U2410F
   //AdjustOsdRotation(); //KevinL 20090213
   StorePresetModes(); //KevinL 20090323	   
#endif   

   //AudioSpeakerControl(UserPrefSpeakers);

	UserPrefPersonalKey2  = ShortCut_PresetMode_All;
	UserPrefPersonalKey3  = ShortCut_BrightnessContrast;

	UserPrefButtonSoundOff = OFF;	//U2913 default on
        UserPrefImageRotation = OSD_NON_ROTATED;   //wei 140723 for factory reset OSD can't rotate from 180 to 0 
        AdjustImageRotation();
	UserPrefGlobalRotationEnabled = 0;	//20121029 Kordonm+ for factory reset image can't rotate from 180 to 0
	UserPrefOsdRotation = OSD_NON_ROTATED;   
	AdjustOsdRotation();
	SaveModeIndependentSettings();
	SavePortSchemeDependentSettings();
	SaveFactorySettingsInNVR();

	//UserPrefUartSelect = OSD_UART_DEF;
	//AdjustUartSelect();	//Hyb130920- This make system hangup when reset//Hyb130602+ 
   //Neil120818* move here to buy our some time avoid show no signal OSD too easy.
   // the input configuration
   SelectInput(gmvb_MainChannel, UserPrefCurrentInputMain, NORMAL_MODE);
   //gm_ModeDetectionConfig(B_Channel, DET_RESTART);
   #ifndef NO_PIP_SUPPORT
   SelectInput(gmvb_PipChannel, UserPrefCurrentInputPip, NORMAL_MODE);
   #endif
   //gm_ModeDetectionConfig(B_Channel, DET_RESTART);
   DDC_CI_CapabilitiesString();//20121228 for ddcci set capability sting


}
#endif //#ifdef Action_FactoryReset_WB_Used

#ifdef Action_DynamicScale_WB_Used
//******************************************************************************
//
// FUNCTION       : void DynamicScale(void)
//
// USAGE          : Demo for dynamic scale on side-by-side pip
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
void DynamicScale(void)
{
   BYTE B_Direction = gm_OsdGetActionByteParameter();
   DynamicScalerSideBySide(B_Direction);
}
#endif

#ifdef Action_DynamicScale_WB_Used   //  #ifdef Action_DynamicScaling_DTV_WB_Used
//******************************************************************************
//
// FUNCTION       : void DynamicScaling_DTV(gmt_PHY_CH B_Channel, WORD Width, WORD Height, WORD Xpos, WORD Ypos)
//
// USAGE          : Change the channel display window in a dynamic mode leaving the other channel window unchanged.
//
// INPUT          : gmt_PHY_CH B_Channel   - Selected channel
//               WORD Width            - the end width size of the window
//               WORD Height            - the end height size of the window
//               WORD Xpos            - the end x position of the window
//               WORD Ypos            - the end width y position of the window
//
// OUTPUT         : None
//
// GLOBALS        : SW_DynScalePosition      (W)
//               gmvw_OutputHActive      (W)
//               gmvw_OutputVActive      (W)
//               gmvw_OutputHStart      (W)
//               gmvw_OutputVStart      (W)
//
// USED_REGS      : None
//
// PRE_CONDITION  : The system is in a known stable state.
//
// POST_CONDITION : The slected window will have the desired size and position
//
//******************************************************************************
void DynamicScaling_DTV(gmt_PHY_CH B_Channel, WORD Width, WORD Height, WORD Xpos, WORD Ypos)
{
   BYTE B_TVChannel;

   SW_DynScalePosition = 0;

   // starting position which use the previous window characterisitics
   SetDynamicScalerSizeBegin   (B_Channel);


   //
   // Setup Dynamic scaling size end
   //
   gmvw_OutputHActive[B_Channel] = Width;
   gmvw_OutputVActive[B_Channel] = Height;
   gmvw_OutputHStart[B_Channel] = Xpos;
   gmvw_OutputVStart[B_Channel] = Ypos;

   Freeze(B_Channel, FALSE);

   //
   // Setup Dynamic scaling size Start
   //
   //      SetDynamicScalerSizeBegin(B_Channel);

   // Setup Display
   if (B_Channel == gmvb_MainChannel)
   {
      //gm_Print("Main: Scale Dynamically in steps of 16...",0);
      SetDynamicScalerStep(gmvb_MainChannel, 4);
      SetDynamicScalerStep(gmvb_PipChannel, 0);   // turn off pip scaling
      ApplyOutputAspect(gmvb_MainChannel, B_AspectMethod[gmvb_MainChannel]);
#ifndef NO_PIP_SUPPORT      
      SetPipBorder(gmvb_MainChannel, TRUE) ;
#endif
      gmvw_OutputHActive[gmvb_MainChannel] =
         ((gmvw_OutputHActive[gmvb_MainChannel] + 1) & ~BIT0);
      gm_AfmSetup(W_OverscanEnabled[gmvb_MainChannel]);
   }
#ifndef NO_PIP_SUPPORT   
   else
   {
      //gm_Print("Pip: Scale Dynamically in steps of 4...",0);
      SetDynamicScalerStep(gmvb_MainChannel, 0);   // turn off Main scaling
      SetDynamicScalerStep(gmvb_PipChannel, 4);
      ApplyOutputAspect(gmvb_PipChannel, B_AspectMethod[gmvb_PipChannel]);
      SetPipBorder(gmvb_PipChannel, TRUE) ;

      gmvw_OutputHActive[gmvb_PipChannel] =
         ((gmvw_OutputHActive[gmvb_PipChannel] + 1) & ~BIT0);
   }
#endif

   //
   DynamicScalerStart( B_Channel );
   DynamicScalerProcess(FALSE);
   //UpdateFilters() ;//this function is called at the end of DynamicScalerStop()
   DynamicScalerStop();
   // Request Blenders ON and User Prefs
   ModeSetupBlender(B_Channel, DISPLAY_DEFER_TMO_NORMAL);
}
#endif // #ifdef Action_DynamicScale_WB_Used

#ifdef Action_TurnCcOnOff_WB_Used
//******************************************************************************
//
// FUNCTION       : void TurnCcOnOff(void)
//
// USAGE          : Turn on or off the Closed Caption
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
void TurnCcOnOff(void)
{
}
#endif

#if ((defined ADJUSTER_CC_CONFIG_DECLARED) && (defined ADJUSTER_CC_ON_DECLARED))
//******************************************************************************
//
// FUNCTION       : void AdjustCCConfig(void)
//
// USAGE          : Adjust CC Configuration (CC1, CC2, CC3, CC4, Text1, Text2, Text3, Text4)
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
void AdjustCCConfig(void)
{
}
#endif

//******************************************************************************
//
// FUNCTION       : BOOL IsCC_On(gmt_PHY_CH B_Channel)
//
// USAGE          : Check whether selected channel CC is being enabled
//
// INPUT          : B_Channel - CH_A or CH_B
//
// OUTPUT         : TRUE  = CC is on;
//                  FALSE = CC is off
//
// GLOBALS        : UserPrefCC_On
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BOOL IsCC_On(gmt_PHY_CH B_Channel)
{
#ifdef UserPrefCC_On
   if (B_FocusBorder == B_Channel)         // Current focus channel
   {                           // Yes
      if (UserPrefCC_On)            // User enable CC
         return (TRUE);            // Yes
      else
         return (FALSE);
   }
   else
      return (FALSE);
#else
   return (FALSE);
#endif
}

//******************************************************************************
//
// FUNCTION       : BYTE GetCC_Config(void)
//
// USAGE          : Returns value of UserPrefCC_Config
//
// INPUT          : None
//
// OUTPUT         : typedef enum
//                  {
//                  CC_SERVICE_OFF,            // CC display off
//                  CC_SERVICE_CC1,            // CC1
//                  CC_SERVICE_CC2,            // CC2
//                  CC_SERVICE_CC3,            // CC3
//                  CC_SERVICE_CC4,            // CC4
//                  CC_SERVICE_T1,             // Text1
//                  CC_SERVICE_T2,             // Text2
//                  CC_SERVICE_T3,             // Text3
//                  CC_SERVICE_T4,             // Text4
//                  CC_SERVICE_MAX
//                  } gmt_CC_SERVICE;
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
BYTE GetCC_Config(void)
{
#ifdef UserPrefCC_Config
   return UserPrefCC_Config;
#else
   return 0;
#endif
}

//******************************************************************************
// HELPER FUNCTIONS
//******************************************************************************

#ifdef Action_ToggleScrollMenuItem_WB_Used
//******************************************************************************
//
// FUNCTION       : void ToggleScrollMenuItem(void)
//
// USAGE          : Used for on/off menu items to keep track of what user is highlighting
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : UserPrefScrollMenuItem      (R/W)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void ToggleScrollMenuItem(void)
{
   UserPrefScrollMenuItem ^= 1;
}
#endif


//******************************************************************************
//
// FUNCTION       : void TurnOffMainBlender(void)
//
// USAGE          : function to turn off blender
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : gmvb_MainChannel      (R/W)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void TurnOffMainBlender(void)
{
   SetDisplayOff(gmvb_MainChannel, BACKGROUND_ONLY);
}
//******************************************************************************
//
// FUNCTION       : void TurnOnMainBlender(void)
//
// USAGE          : Functon to turn on blender
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : gmvb_MainChannel      (R/W)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void TurnOnMainBlender(void)
{
   SetDisplayOn(gmvb_MainChannel);
}

#ifdef Action_ListModeHandler_WB_Used
//******************************************************************************
//
// FUNCTION       : void ListModeHandler(void)
//
// USAGE          : Handle number keys and color keys being pressed while in List Mode
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
void ListModeHandler(void)
{
}
#endif

#ifdef Action_SubPageModeHandler_WB_Used
//******************************************************************************
//
// FUNCTION       : void SubPageModeHandler(void)
//
// USAGE          : Handle keys concerning sub page mode in ttx
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
void SubPageModeHandler(void)
{
}
#endif

#if (   defined(Action_ScaleOsdToFullScreen_WB_Used) && \
    defined (UserPrefOsdFullScreenXPercent) && \
    defined (UserPrefOsdFullScreenYPercent))
//******************************************************************************
//
// FUNCTION       : void ScaleOsdToFullScreen(void)
//
// USAGE          : Scales OSD to WXGA screen
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
void ScaleOsdToFullScreen(void)
{
#define WB_PANEL_HACTIVE   1920
#define WB_PANEL_VACTIVE   1200

   /*
   Osd Designed for WXGA 1280x768.  Recalculate Scaling factor based on current panel
   */
	
   UserPrefOsdFullScreenXPercent = (WORD)((DWORD)PanelWidth * 100 / WB_PANEL_HACTIVE);
   UserPrefOsdFullScreenYPercent = (WORD)((DWORD)PanelHeight * 100 / WB_PANEL_VACTIVE);


#undef WB_PANEL_HACTIVE
#undef WB_PANEL_VACTIVE

}
#endif //Action_ScaleOsdToPanel_WB_Used


//******************************************************************************
//
// FUNCTION       : void ToggleValue(BYTE *B_Value)
//
// USAGE          : Toggle the passed value
//
// INPUT          : B_Value
//
// OUTPUT         : 1 if B_Value == 0; 0, otherwise
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
void ToggleValue(BYTE *B_Value)
{
   if (*B_Value)
   {
      *B_Value = 0;
   }
   else
   {
      *B_Value = 1;
   }
}

#if 0//def Action_ToggleMainRouteModeComponent_WB_Used
//******************************************************************************
//
// FUNCTION       : void ToggleMainRouteModeComponent(void)
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
void ToggleMainRouteModeComponent(void)
{
   ToggleValue(&UserPrefMainRouteModeComponent);

   //TODO: fill in functionality here
   gm_SetComponentRouteMode(UserPrefMainRouteModeComponent);

}

//******************************************************************************
//
// FUNCTION       : void SetMainRouteModeComponent( BYTE B_UserPrefMainRouteModeComponent )
//
// USAGE          :
//
// INPUT          : B_UserPrefMainRouteModeComponent
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
void SetMainRouteModeComponent( BYTE B_UserPrefMainRouteModeComponent )
{
   UserPrefMainRouteModeComponent = ( B_UserPrefMainRouteModeComponent ? 1 : 0 );
}

//******************************************************************************
//
// FUNCTION       : BYTE GetMainRouteModeComponent( void )
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : UserPrefMainRouteModeComponent
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
BYTE GetMainRouteModeComponent( void )
{
   return UserPrefMainRouteModeComponent;
}
#endif

#if 0//def Action_TogglePipRouteModeComponent_WB_Used
//******************************************************************************
//
// FUNCTION       : void TogglePipRouteModeComponent(void)
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
void TogglePipRouteModeComponent(void)
{
   ToggleValue(&UserPrefPipRouteModeComponent);
}

//******************************************************************************
//
// FUNCTION       : void SetPipRouteModeComponent( BYTE B_UserPrefPipRouteModeComponent )
//
// USAGE          :
//
// INPUT          : B_UserPrefPipRouteModeComponent
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
void SetPipRouteModeComponent( BYTE B_UserPrefPipRouteModeComponent )
{
   UserPrefPipRouteModeComponent = ( B_UserPrefPipRouteModeComponent ? 1 : 0 );
}

//******************************************************************************
//
// FUNCTION       : BYTE GetPipRouteModeComponent( void )
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : UserPrefPipRouteModeComponent
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
BYTE GetPipRouteModeComponent( void )
{
   return UserPrefPipRouteModeComponent;
}
#endif


#if 0//def Action_TogglePipRouteModeScartRGB_WB_Used
//******************************************************************************
//
// FUNCTION       : void TogglePipRouteModeScartRGB(void)
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
void TogglePipRouteModeScartRGB(void)
{
   ToggleValue(&UserPrefPipRouteModeScartRGB);
}

//******************************************************************************
//
// FUNCTION       : void SetPipRouteModeScartRGB( BYTE B_UserPrefPipRouteModeScartRGB )
//
// USAGE          :
//
// INPUT          : B_UserPrefPipRouteModeScartRGB
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
void SetPipRouteModeScartRGB( BYTE B_UserPrefPipRouteModeScartRGB )
{
   UserPrefPipRouteModeScartRGB = ( B_UserPrefPipRouteModeScartRGB ? 1 : 0 );
}

//******************************************************************************
//
// FUNCTION       : BYTE GetPipRouteModeScartRGB( void )
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : UserPrefPipRouteModeScartRGB
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
BYTE GetPipRouteModeScartRGB( void )
{
   return UserPrefPipRouteModeScartRGB;
}
#endif

#ifdef Action_AdjustNativeMode_WB_Used
//******************************************************************************
//
// FUNCTION       : void AdjustNativeMode(void)
//
// USAGE          : Adjuster function for setting native mode. Specifically,
//                  The function will enable an output-to-input screen ratio of 1:1.
//                  Native mode can be enabled with filters on or off.
//                  FILTERS OFF mode refers to the fact that all scaling filters are disabled.
//                  FILTERS ON mode refers to scaling filters set to 1:1 scaling.
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : gmvb_MainChannel         (R)
//                  gmvw_OutputHActive         (RW)
//                  gmvw_OutputVActive         (RW)
//                  gmvw_OutputHStart         (RW)
//                  gmvw_OutputVStart         (RW)
//                  gmvw_InputHActive         (R)
//                  gmvw_InputVActive         (R)
//                  gmvw_InputModeAttr         (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AdjustNativeMode(void)
{
   SWORD hStart = 0;
   SWORD vStart = 0;

   switch (UserPrefNativeMode)
   {
      case NATIVE_MODE_OFF:
         gmvw_OutputHActive[gmvb_MainChannel] = PanelWidth;
         gmvw_OutputVActive[gmvb_MainChannel] = PanelHeight;
         gmvw_OutputHStart[gmvb_MainChannel] = 0;
         gmvw_OutputVStart[gmvb_MainChannel] = 0;
         ModeSetupDisplayChange(gmvb_MainChannel, DISPLAY_CHANGE_NORMAL);
         break;

      case NATIVE_MODE_NO_FILTERS:
         //
         //NOTE: This mode works by bypassing the scaling path and disabling scaling.
         //The MAIN_SCALING_FILTER_CTRL register should be set such that
         //vertical and horizontal scaling filters are disabled.
         //

         gmvw_OutputHActive[gmvb_MainChannel] = stInput[gmvb_MainChannel].Hactive;
         if ((stInput[gmvb_MainChannel].ModeFlags & gmd_MODE_INTERLACED) >> 2)
            gmvw_OutputVActive[gmvb_MainChannel] = stInput[gmvb_MainChannel].Vactive * 2;
         else
            gmvw_OutputVActive[gmvb_MainChannel] = stInput[gmvb_MainChannel].Vactive;

         //
         // Disable cropping and set aspect ratio to input ratio:
         //
         B_AspectMethod[gmvb_MainChannel] = AR_FULL_SCREEN;
         if ((gmvb_MainChannel == CH_A))
         {
            gm_SetupInputMain();
            gm_AfmSetup(0);
         }
      #ifndef NO_PIP_SUPPORT
         else
            gm_SetupInputPip();
      #endif

         //
         // Center output image on panel:
         //
         hStart = (PanelWidth - gmvw_OutputHActive[gmvb_MainChannel]) / 2;
         vStart = (PanelHeight -gmvw_OutputVActive[gmvb_MainChannel]) / 2;
         if (hStart > 0 && vStart > 0)
         {
            gmvw_OutputHStart[gmvb_MainChannel] = hStart;
            gmvw_OutputVStart[gmvb_MainChannel] = vStart;
         }
         else
         {
            gmvw_OutputHStart[gmvb_MainChannel] = 0;
            gmvw_OutputVStart[gmvb_MainChannel] = 0;
         }

         gm_ScalerDynamicEnable(gmvb_MainChannel, FALSE);
         gm_DisplaySetSize(gmvb_MainChannel);
         gm_DisplaySetPosition(gmvb_MainChannel);
         gm_ScalerSetup(gmvb_MainChannel);
         gm_MemorySizeAdjust(gmvb_MainChannel);
         gm_SyncUpdate(gmvb_MainChannel, INPUT_OUTPUT_CLOCK);
         break;

      case NATIVE_MODE_WITH_FILTERS:
         //
         //NOTE: This mode works by following the normal scaling model but using a 1:1
         //scaling. This is done by calling SetDynamicScalerSizeBegin() and
         //SetDynamicScalerSizeEnd() and having end resolution equal to start resolution.
         //
         //This is a workaround since ModeSetupDisplayChange() calls SetMainDisplay(), which
         //appears to set the output size to the panel size.

         gmvw_OutputHActive[gmvb_MainChannel] = stInput[gmvb_MainChannel].Hactive;
         if ((stInput[gmvb_MainChannel].ModeFlags & gmd_MODE_INTERLACED) >> 2)
            gmvw_OutputVActive[gmvb_MainChannel] = stInput[gmvb_MainChannel].Vactive * 2;
         else
            gmvw_OutputVActive[gmvb_MainChannel] = stInput[gmvb_MainChannel].Vactive;

         //
         // Center output image on panel:
         //
         hStart = (PanelWidth - gmvw_OutputHActive[gmvb_MainChannel]) / 2;
         vStart = (PanelHeight -gmvw_OutputVActive[gmvb_MainChannel]) / 2;
         if (hStart > 0 && vStart > 0)
         {
            gmvw_OutputHStart[gmvb_MainChannel] = hStart;
            gmvw_OutputVStart[gmvb_MainChannel] = vStart;
         }
         else
         {
            gmvw_OutputHStart[gmvb_MainChannel] = 0;
            gmvw_OutputVStart[gmvb_MainChannel] = 0;
         }

         //
         //Set scaling start and end to be equal and proceed with normal setup:
         //
         SetDynamicScalerSizeBegin(gmvb_MainChannel);
         SetDynamicScalerStep(gmvb_MainChannel, 4);
         ApplyOutputAspect(gmvb_MainChannel, B_AspectMethod[gmvb_MainChannel]);
#ifndef NO_PIP_SUPPORT
         SetPipBorder(gmvb_MainChannel, TRUE);
#endif
         if ((gmvb_MainChannel == CH_A))
            gm_AfmSetup(W_OverscanEnabled[gmvb_MainChannel]);
         SetDynamicScalerSizeEnd(gmvb_MainChannel);
         DynamicScalerStart(gmvb_MainChannel);
         DynamicScalerProcess(FALSE);
         DynamicScalerStop();
         ModeSetupBlender(gmvb_MainChannel, DISPLAY_DEFER_TMO_NORMAL);
         break;

   }
}
#endif

//******************************************************************************
//
// FUNCTION       : void AdjustVBIService(void);
//
// USAGE          : Adjuster function for setting VBI Service Preference when No Tuner option is enabled
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : UserPrefVBI_ServicePreferred
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AdjustVBIService(void)
{
}

//******************************************************************************
//
// FUNCTION       : BOOL IsOutputBlocked(void)
//
// USAGE          : Returns whether focused output is channel or parental blocked.
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : UserPrefChannelBlockStateFlag (R)
//                  B_MainProgramBlocked (R)
//                  B_PipProgramBlocked (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BOOL IsOutputBlocked(void)
{
#if 0
   if (UserPrefSideBySideFocus == 0)   // focus is on Main channel
   {
      return ((UserPrefChannelBlockStateFlag == 1) || (B_MainProgramBlocked == TRUE));
   }
   else   // focus is on pip channel (Side by Side)
   {
      return ((UserPrefChannelBlockStateFlagPip == 1) || (B_PipProgramBlocked == TRUE));
   }
#else
   return 0;
#endif
}

//******************************************************************************
//
// FUNCTION       : BOOL ForceSameAudioSource(void)
//
// USAGE          : Returns whether speaker and headphones needs to be forced as same audio source
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : gmvb_MainChannel (R)
//                  gmvb_PipChannel (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BOOL ForceSameAudioSource(void)
{
   //If there is only one audio decoder, the speaker and headphone must be
   //forced to be from the same source(Main or PIP)
   return TRUE;
}

//******************************************************************************
//
// FUNCTION       : BOOL IsAnyTTXDataAvailable(void)
//
// USAGE          : Returns whether there is any TTX data available.
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
BOOL IsAnyTTXDataAvailable(void)
{
   return FALSE;
}

//******************************************************************************
//
// FUNCTION       : BOOL IsTtxSubtitlePage(void)
//
// USAGE          : Returns whether decoder status shows that current page is a subtitle page.
//
// INPUT          : None
//
// OUTPUT         : TRUE/FALSE
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
BOOL IsTtxSubtitlePage(void)
{
   return FALSE;
}

//******************************************************************************
//
// FUNCTION       : BOOL IsTtxSubtitlePageReEnableMode(void)
//
// USAGE          : Returns whether the feature to go back into ttx subtitle page after cable is
//                        unplugged and plugged back in is defined or not.
//
// INPUT          : None
//
// OUTPUT         : TRUE/FALSE
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
BOOL IsTtxSubtitlePageReEnableMode(void)
{
   return FALSE;
}

#ifdef Action_ResetParentalSettings_WB_Used
//******************************************************************************
//
// FUNCTION       : void ResetParentalSettings(void)
//
// USAGE          : Resets Parental settings.
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
void ResetParentalSettings(void)
{

}
#endif

//******************************************************************************
//
// FUNCTION       : void ImageSchemeReset(void)
//
// USAGE          : Resets ImageScheme settings.
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
void ImageSchemeReset(void)
{
#if (PORT_SCHEME_DEPENDENT == PORT_DEPENDENT)
   UserPrefMFMImageScheme = UserPrefImageScheme = FactoryDefaultInROM.ImageScheme;
#endif

   PortSchemeDependentUserPreferences = SchemeFactoryDefaultInROM[UserPrefImageScheme];
   SavePortSchemeDependentSettings();

   AdjustImageScheme();
}

//******************************************************************************
//
// FUNCTION       : void AdjustSub()
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : None
//
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AdjustSub(void)
{
#ifdef 	MULTIPANEL_DATA_IN_ONE_HEX
   //gm_Print ("Enter Sub method",0);
   {
      BYTE B_Param;
      char temp[30];
      BYTE i;

      if (UserPrefPanelSelect == 33)
         B_Param = 35;// 480p
      else if (UserPrefPanelSelect == 35)
         B_Param = 25;//1080p
      else
         B_Param = 33;//720p
      //
      // Force Reinitialization of NVRAM so that new panel boots up with known state
      //
      InitNvram();
      msg ("NVRAM re-initialized to default.");
      //
      // Now process panel update request and save to NVRAM
      //
      UserPrefPanelSelect = B_Param;
      SaveModeIndependentSettings();
      //
      // Search for panel specified
      //
      for (i=1; PanelArray[i] != NULL_PTR; i++)
      {
         if (PanelArray[i] == PanelArray)
         {
            continue;
         }
         //
         // For manually modified panel, search for panel ID based on userpreference
         //
         if (B_Param)
         {
            if (PanelArray[i]->ID == B_Param)
            {
               msg_i("Panel chosen was No.%d", B_Param);
               msg ("Panel ID found");
               CurrentPanelData = PanelArray[i];
               _fstrcpy (temp, PanelArray[i]->C_Name);
               msg (temp);
               msg ("Please make sure connections are correct, and power cycle the board");
               return;
            }
         }
         //
         // Otherwise, use default panel
         //
         else
         {
            if (PanelArray[i]->ID == DEFAULT_PANEL)
            {
               CurrentPanelData = PanelArray[i];
               msg ("Using DEFAULT_PANEL defined in <<panel.h>>");
               msg_i ("Panel ID = %d", CurrentPanelData->ID);
               _fstrcpy (temp, CurrentPanelData->C_Name);
               msg (temp);
               msg ("Please make sure connections are correct, and power cycle the board");
               return;
            }
         }
      }
      msg ("Panel ID given is not supported");
   }
#endif
}

BOOL IsGotoSleep(void)
{
#ifdef DEEP_SLEEP_MAIN_AND_PIP
   if(IsVideoInput(UserPrefCurrentInputMain))
      return FALSE;
   else if (IsPipNoSync())
      return TRUE;
   else
      return FALSE;
#else // Only check Main channel
   return TRUE;
#endif
}

void AdjustColorDomain(void)
{
   //if (IsAVInfoSource())
   //      {
   //            gm_Print("HDMI and DP port get input color domain by Info frame", 0);
   //            return;
   //      }
   // else
         {
      #ifdef DELL_U2410F//120523 Edward remapping for standard code
            Input_Color_Domain = UserPrefColorFormat;//UserPrefColorFormat + 1;
            Output_Color_type =  Output_Auto;//UserPrefColorMode + 1;

			if(UserPrefGameMode ==1)	//20120913 for Gamma mode change from RGB to YUV, display abnormal issue.
				gm_ClearRegBitsByte(SYNC_CONTROL,BIT2);
			
	     ModeSetupRequest(gmvb_MainChannel, REQ_MS_RESTART, PARAM_NONE, REQUEST_NOW);		
	  #else
	     	Input_Color_Domain = UserPrefInputColorDomain;
            Output_Color_type = UserPrefOutputColorType;

	    ModeSetupRequest(gmvb_MainChannel, REQ_MS_RESTART, PARAM_NONE, 100);			
	  #endif
            
         }
}

BOOL IsAVInfoSource(void)
{
   if (((GetChannelConnType(CH_A) == HDMI) && (gm_IsHdmiTiming(gB_CurrentInputConnector[CH_A])))
         || (GetChannelConnType(CH_A) == CT_DISPLAY_PORT))
      return TRUE;
   else
      return FALSE;
}

#ifndef NO_PIP_SUPPORT
// 20111004 #1, PIP demo
extern BYTE PIP_Index;
extern BYTE PIP_Oder;
extern void PIP_Demo_Handler(void);

void PIP_Demo(void)
{

   PIP_Index = 0;
   PIP_Oder = 0;
   gm_TimerStart(BLINKING_LED_TMR, 5);
   PIP_Demo_Handler();
}
#else
void PIP_Demo(void){}
#endif // NO_PIP_SUPPORT

void AdjustDUMSwitch(void)
{
#if (FEATURE_DUM == ENABLE)

#ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()==CHIPID_4K2K_R)
   {
#if ENABLE_SOC_IPC_BUSY_CHECK
	/*ChengLin 20130124: New 4k2k board BUSY status check*/
		if(!SocIPCIsSlaveLeftReady())
			return; 
#endif   
   	SOCIPC_SyncOSDAdjustHandler( SI_OSD_SYNC_COLOR_CTRL );
   }
#endif
   //gm_Print("Adjust DUM state ........ %d", UserPrefDUMSwitch);

	DisplayUniformityModule(UserPrefDUMSwitch, FALSE, 1, 1, PanelWidth, PanelHeight);
   	gm_SyncUpdateWait(gmvb_MainChannel, OUTPUT_CLOCK);

	//120908 Edward for DUM off need to set BK back due to Brightness is different from Table1/2
	if(UserPrefDUMSwitch == UNIFORMITY_OFF)
	{
		AdjustBackLight();
		//gm_Print("####DUM adjust bk again",0);
	}
#endif
}

#ifndef DELL_U2410F//111202 Edward disable for DELL OSD no this control
void AdjustOSDBlending(void)
{
   WORD blendvalue;

   blendvalue = (gm_ReadRegWord(OSD_BLEND_TABLE0) & 0xFFF0) | UserPrefOSDG1Blend;

   gm_WriteRegWord(OSD_BLEND_TABLE0, blendvalue);
   gm_WriteRegWord(HOST_CONTROL, 0x0008);
}

void AdjustOSDBlending1(void)
{
   WORD blendvalue;

   blendvalue = (gm_ReadRegWord(OSD_BLEND_TABLE0) & 0xFF0F) | UserPrefOSDG2Blend << 4;

   gm_WriteRegWord(OSD_BLEND_TABLE0, blendvalue);
   gm_WriteRegWord(HOST_CONTROL, 0x0008);
}

void AdjustOSDBlending2(void)
{
   WORD blendvalue;

   blendvalue = (gm_ReadRegWord(OSD_BLEND_TABLE0) & 0xF0FF) | UserPrefOSDG3Blend << 8;

   gm_WriteRegWord(OSD_BLEND_TABLE0, blendvalue);
   gm_WriteRegWord(HOST_CONTROL, 0x0008);
}
#endif


void AdjustCaptureMain(void)
{
   //gm_Print("Capture Main = .............. %d", UserPrefCaptureM);
   ModeSetupRequest(gmvb_MainChannel, REQ_MS_RESTART, PARAM_NONE, 100);
}

void gm_ImageHFlipControl(BOOL Bt_State)
{
	if (Bt_State == TRUE)
   {  
		gm_SetRegBitsWord(ODP_PANEL_LINBUF_CTRL, (BIT9 | BIT10 | BYPASS_HFLIP_4LVDSn | 
         BYPASS_HFLIP_4DPTXn | LINE_EN | HFLIP_EN | NON_SPLIT| PERMU_SWAP_EN));
   }
	else
   {  
		gm_ClearRegBitsWord(ODP_PANEL_LINBUF_CTRL, (BIT9 | BIT10 | BYPASS_HFLIP_4LVDSn | 
         BYPASS_HFLIP_4DPTXn | LINE_EN | HFLIP_EN | NON_SPLIT| PERMU_SWAP_EN));
   }
	//if((UserPrefPanelSelect == AUO_1080P_T550HVN01V9_8BIT)||(UserPrefPanelSelect == AUO_1080P_T650HVN02_10BIT))
	#if((DEFAULT_PANEL == ZDT_4K2K_CMI)||(DEFAULT_PANEL == ZDT_4K2K_CMI_39)||(DEFAULT_PANEL == ZDT_4K2K_CMI_65)||(DEFAULT_PANEL == ZDT_4K2K_CMI_50))
		gm_SetRegBitsWord(ODP_PANEL_LINBUF_CTRL, PERMU_SWAP_EN);		
	#else
		gm_ClearRegBitsWord(ODP_PANEL_LINBUF_CTRL, PERMU_SWAP_EN);		
	#endif
}

void gm_ImageVFlipControl(BOOL Bt_State)
{
	#if 0
	if (Bt_State == TRUE)
   {  
   		gm_WriteRegWord(0xA872,0);
		gm_ClearRegBitsDWord(EXMFRC_CTRL, (BIT23 | BIT24 | BIT25));
      	gm_ClearRegBitsDWord(EXMFRC_PA_CTRL, (BIT16 | BIT17 | BIT20));
		
   		if(stInput[CH_A].ModeFlags & gmd_MODE_INTERLACED)
   		{
   			gm_WriteRegWord(0xA872,0x2000);
   		}
		else
		{
			gm_SetRegBitsDWord(EXMFRC_CTRL, (BIT23 | BIT24 | BIT25));
      		gm_SetRegBitsDWord(EXMFRC_PA_CTRL, (BIT16 | BIT17 | BIT20));
		}
   }
	else
   {  
   		if(stInput[CH_A].ModeFlags & gmd_MODE_INTERLACED)
   		{
   			gm_WriteRegWord(0xA872,0);
   		}
		else
		{
			gm_ClearRegBitsDWord(EXMFRC_CTRL, (BIT23 | BIT24 | BIT25));
      		gm_ClearRegBitsDWord(EXMFRC_PA_CTRL, (BIT16 | BIT17 | BIT20));
		}
   }
	#else
	gm_WriteRegWord(0xA872,0);
	gm_WriteRegWord(EXMFRC_CTRL,0);

	if(Temp_MVF_Hi == 0)
		Temp_MVF_Hi = gm_ReadRegWord(MVF_OFFSET1_hi);
	
	if (Bt_State == TRUE)
   	{
   		gm_WriteRegWord(0xA872,0x2000);

		if((gm_IsVideo(gmvb_MainChannel)) && (!gm_IsInterlaced(gmvb_MainChannel)))
		{
			if(stInput[gmvb_MainChannel].Vactive < PanelHeight)
			{
				gm_WriteRegWord(MVF_OFFSET1_hi,0x01);
			}
		}
	}
	else
	{
		gm_WriteRegWord(0xA872,0);

		gm_WriteRegWord(MVF_OFFSET1_hi,Temp_MVF_Hi);
	}

	if(UserPrefPipMode != NO_PIP)
	{
		DWORD V_StartAddr = gm_ReadRegDWord(PFRC_WBUF_START_lo);
		WORD V_BWidth = gm_ReadRegWord(PFRC_WR_WIDTH);
		WORD V_BHeight = gm_ReadRegWord(PFRC_R0_LINES_OUT);

		gm_MemoryAdjust_VFlip(gmvb_PipChannel, V_StartAddr, V_BWidth, V_BHeight);
	}
	
	#endif
}

#if (defined(BOARD_93XX_4K2K))
void AdjustImageRotation(void)
{
#ifdef ENABLE_PA168
	PA168_ImageFlip(UserPrefImageRotation);
#elif((DEFAULT_PANEL == ZDT_4K2K_CMI_39)||(DEFAULT_PANEL == ZDT_4K2K_CMI_65)||(DEFAULT_PANEL == ZDT_4K2K_CMI_50))
	switch(UserPrefImageRotation)
	{
		case OSD_NON_ROTATED:
			ZDT_4K2K_CMI_Panel_RPF(UserPrefPanelSelect,0);
		break;
		case OSD_ROTATED_180:
			ZDT_4K2K_CMI_Panel_RPF(UserPrefPanelSelect,1);
		break;
	}
#endif
}
#else
void AdjustImageRotation(void)
{
	BYTE TEMPPIP = NO_PIP;
	static BYTE PreImageRot = OSD_NON_ROTATED;

	if(PreImageRot == UserPrefImageRotation)
		return;
	
   switch(UserPrefImageRotation)
   {
	  case	OSD_NON_ROTATED:
		 gm_ImageHFlipControl(FALSE);
		 gm_ImageVFlipControl(FALSE);
		 break;

	  case	OSD_ROTATED_90:
		 gm_ImageHFlipControl(TRUE);
		 gm_ImageVFlipControl(FALSE);
		 break;

	  case	OSD_ROTATED_180:
		 gm_ImageHFlipControl(FALSE);
		 gm_ImageVFlipControl(TRUE);
		 break;

	  case	OSD_ROTATED_270:
		 gm_ImageHFlipControl(TRUE);
		 gm_ImageVFlipControl(TRUE);
		 break;

	  default:
		 //turn off OSD h and V flip
		 gm_ImageHFlipControl(FALSE);
		 gm_ImageVFlipControl(FALSE);
		 break;
   }
   
   PreImageRot = UserPrefImageRotation;

	if(UserPrefPipMode != NO_PIP)
   {
		TEMPPIP = UserPrefPipMode;
		
		UserPrefPipMode = NO_PIP;
		AdjustPipMode();
		gm_Delay1ms(3);

		UserPrefPipMode = TEMPPIP;
		AdjustPipMode();
		//Hyb140316+ for PBP main channel full screen first after AC on.
		AdjustMainMode();
   }

}

#endif
void AdjustDPSwitch(void)
{
   #if (DPRX0_DYNAMIC_INIT_BY_NVRAM!=0) && (defined UserPrefDPSWitch)

/*ChengLin 20130318: Send Userpref to Athena Left Only*/
//ChengLin 21030613: Don't init DPApp for Athena L side   
#if 0//def ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()==CHIPID_4K2K_R)
   {
#if ENABLE_SOC_IPC_BUSY_CHECK
      /*ChengLin 20130124: New 4k2k board BUSY status check*/
      if(!SocIPCIsSlaveLeftReady())
         return;
#endif                                                                
      SOCIPC_SyncOSDAdjustHandler( SI_OSD_SYNC_DP_SWITCH );
   }
#endif

   	DpAppInit();
	#if  (DPRX_PANEL == ENABLE) //Disable set as USE_DPTX_DYNAMIC_SWITCH
		DPTxPortSwitch (MstTxPort, iDpTxPort);
		//Hyb130508+ for DP panel adjust DP switch will cause DP Panel blank screen;
		if((gm_ReadRegWord(HOST_DVI_DP_SETUP)&DPTX_PHY_SRC_SELECT) != 0x00)	//Force to ODP path if DP panel;
			gm_ClearRegBitsByte(HOST_DVI_DP_SETUP,DPTX_PHY_SRC_SELECT);
	#else
		#if (FEATURE_DISPLAY_PORT == ENABLE) 
			#if((SELECT_DPTX_TYPE==USE_DPTX_DYNAMIC_SWITCH)||(SELECT_DPTX_TYPE==USE_MST_DPTX_OUTPUT))
			DPTxPortSwitch (iDpTxPort,MstTxPort);	//Not DP panel, set to daisy chain;
			#elif (SELECT_DPTX_TYPE==USE_IDP_DPTX_OUTPUT)
			DPTxPortSwitch (MstTxPort, iDpTxPort);
			#endif
		#endif
	#endif
	
   #endif  
}


#ifdef ST_4K2K_93xx_BOARD
void AdjustUARTSwitch(void)
{
   External_UART_Switch(UserPrefUARTSwitch);
}

void External_UART_Switch(BYTE number)
{
   if(GetChipID_4K2K()==CHIPID_4K2K_R)
   {
		if(number > 3)
		{
			gm_Print("Number 0 -> Athena R", 0);
			gm_Print("Number 1 -> Athena L", 0);
			gm_Print("Number 2 -> Athena FE", 0);
			gm_Print("Number 3 -> VEGA", 0);
		}
		else
		{
			switch (number)
			{
				case UART_TO_ATHENA_R: // 0:
					//gm_Print("External UART connect to R", 0);
					gm_ClearRegBitsWord(GPOUTPUT0, (GPIO6_OUT | GPIO9_OUT));
					break;
				case UART_TO_ATHENA_L: // 1:
					//gm_Print("External UART connect to L", 0);
					gm_ClearAndSetBitsWord(GPOUTPUT0,GPIO6_OUT,GPIO9_OUT);
					break;
				case UART_TO_ATHENA_FE: // 2:
					//gm_Print("External UART connect to FE", 0);
					gm_ClearAndSetBitsWord(GPOUTPUT0,GPIO9_OUT,GPIO6_OUT);
					break;
				case UART_TO_VEGA: // 3:
					//gm_Print("External UART connect to VEGA", 0);
					gm_SetRegBitsWord(GPOUTPUT0, (GPIO6_OUT | GPIO9_OUT));
					break;
			}
		}
	}
}

extern void gm_4K2K_MainCapture(void);
extern void EXT_FUNC Dp1ExtPortSetup_ID0(void);
extern void EXT_FUNC Dp1ExtPortSetup_ID1(void);
void AdjustPBP(void)
{
	BYTE StorePort = 0;

#if ENABLE_SOC_IPC_BUSY_CHECK
	if(GetChipID_4K2K()==CHIPID_4K2K_R)
	{
		/*ChengLin 20130124: New 4k2k board BUSY status check*/
		if(!SocIPCIsSlaveLeftReady())
			return;
	}
#endif   

	/*ChengLin 20130306: Force to Non-Ready mode for clear "IMAGE_ON_STATE" and re-check signals status*/
#ifdef ST_4K2K_93xx_BOARD 
#if ENABLE_ATHENA_R_L_IMAGE_SYNC_HANDLER
	Image_ReadyFlag(FALSE); //Set Athena Right GPIO82 to Low		
	ImageSyncHandleSetState(IMAGE_OFF_STATE); //Set Athena Right to "IMAGE_OFF_STATE" sate
	SOCIPC_SetAthena_L_ImageSyncState(IMAGE_OFF_STATE); //Set Athena Left GPIO84 to Low and Image sync state to "IMAGE_OFF_STATE" 		
#endif
#endif

	switch(UserPrefPBPMode)
	{
		case PBP_OFF: // 0:
		   if(GetChipID_4K2K()==CHIPID_4K2K_R)
		   {
		   		StorePort = UserPrefCurrentInputMain;
				UserPrefCaptureM = CAPTURE_MODE_LEFT; // CAPTURE_MODE_FULL;
				SOCIPC_SyncOSDAdjustHandler(SI_OSD_SYNC_SAVE_PBP_Mode);
				gm_ClearRegBitsWord(PB_CTRL, PB_MAIN_ENABLE);
				gm_SetRegBitsWord(PB_CTRL, PB_FORCE_MAIN_BKGND);
				gm_ForceUpdate(CH_A , INPUT_OUTPUT_CLOCK);
				//SOCIPC_SyncOSDAdjustHandler( SI_OSD_SYNC_SEL_INPUT_MAIN );

				if(UserPrefCurrentInputMain == ALL_BOARDS_DP3)
					UserPrefCurrentInputMain = ALL_BOARDS_DP1;
				
				while(gAthena_LEFT_FE_Slave_Parameters.PortStatus != UserPrefCurrentInputMain)
				{
					SOCIPC_SyncOSDAdjustHandler( SI_OSD_SYNC_SEL_INPUT_MAIN );
					//gm_Print("L port is %d", gAthena_B_C_Parameters.PortStatus);
					gm_Delay10ms(1);
					SocIPCGetSlavePortStatus(SOCIPC_DEVICEL_DST_ADDRESS);
				}

				UserPrefCurrentInputMain = StorePort;
#ifdef DP_HUB_2_LOG_PORTS //Add by ChengLin 20130530  
            if(gm_GetDpDigitalID(gB_CurrentInputConnector[gmvb_MainChannel])==DID_DPRX0)
            {
               ResetDpRx0MstInfoUpdatFlag();
               ResetDpRx0VirtualChannelsAllocated();
            }
            else
            {
               UserPrefCaptureM = CAPTURE_MODE_RIGHT;
               gm_4K2K_MainCapture();
            }
#else //Original
            UserPrefCaptureM = CAPTURE_MODE_RIGHT;
            gm_4K2K_MainCapture();
#endif
			}
			break;
			
		case PBP_DP1_HDMI: // 1
			if(GetChipID_4K2K()==CHIPID_4K2K_R)
			{
				//Dp1ExtPortSetup_ID0();
				/*ChengLin 20130305: If switch to different source then execute "DpRxPortChange()" for avoid black screen*/
				if(UserPrefCurrentInputMain != ALL_BOARDS_DP1)
					DpRxPortChange(ALL_BOARDS_DP1);
				UserPrefCaptureM = CAPTURE_MODE_HALF_HORIZONTAL;
				SOCIPC_SyncOSDAdjustHandler(SI_OSD_SYNC_SAVE_PBP_Mode);	

				StorePort = ALL_BOARDS_DP2;
				if(UserPrefCurrentInputMain == ALL_BOARDS_DP2)
					//gm_4K2K_MainCapture();
					ModeSetupRequest(gmvb_MainChannel, REQ_MS_SETUP, PARAM_NONE, 0);
				else
				{
					UserPrefCurrentInputMain = ALL_BOARDS_DP2;
					AdjustCurrentInputMain();
				}

				//StorePort = UserPrefCurrentInputMain;
				//if(UserPrefCurrentInputMain == 11)
				//	UserPrefCurrentInputMain = 10;
				//else
				UserPrefCurrentInputMain = ALL_BOARDS_DP1;	// Hdmi
				
				//SOCIPC_SyncOSDAdjustHandler( SI_OSD_SYNC_SEL_INPUT_MAIN );	
				while(gAthena_LEFT_FE_Slave_Parameters.PortStatus != UserPrefCurrentInputMain)
				{
					SOCIPC_SyncOSDAdjustHandler( SI_OSD_SYNC_SEL_INPUT_MAIN );
					//gm_Print("L port is %d", gAthena_B_C_Parameters.PortStatus);
					gm_Delay10ms(1);
					SocIPCGetSlavePortStatus(SOCIPC_DEVICEL_DST_ADDRESS);
				}
				UserPrefCurrentInputMain = StorePort;
				//gm_SetRegBitsWord(PB_CTRL, PB_MAIN_ENABLE);
				//gm_ClearRegBitsWord(PB_CTRL, PB_FORCE_MAIN_BKGND);
				//gm_ForceUpdate(CH_A , INPUT_OUTPUT_CLOCK);
			}
			break;

		case PBP_DP2_HDMI: // 2:
			if(GetChipID_4K2K()==CHIPID_4K2K_R)
			{
				//Dp1ExtPortSetup_ID1();
				/*ChengLin 20130305: If switch to different source then execute "DpRxPortChange()" for avoid black screen*/				
				if(UserPrefCurrentInputMain != ALL_BOARDS_DP3)			
					DpRxPortChange(ALL_BOARDS_DP3);
				UserPrefCaptureM = CAPTURE_MODE_HALF_HORIZONTAL;
				SOCIPC_SyncOSDAdjustHandler(SI_OSD_SYNC_SAVE_PBP_Mode);	

				StorePort = ALL_BOARDS_DP2;
				if(UserPrefCurrentInputMain == ALL_BOARDS_DP2)
					gm_4K2K_MainCapture();
				else
				{
					UserPrefCurrentInputMain = ALL_BOARDS_DP2;
					AdjustCurrentInputMain();
				}

				//StorePort = UserPrefCurrentInputMain;
				//if(UserPrefCurrentInputMain == 11)
				//	UserPrefCurrentInputMain = 10;
				//else
				UserPrefCurrentInputMain = ALL_BOARDS_DP1;	// DP2
				
				//SOCIPC_SyncOSDAdjustHandler( SI_OSD_SYNC_SEL_INPUT_MAIN );	
				while(gAthena_LEFT_FE_Slave_Parameters.PortStatus != UserPrefCurrentInputMain)
				{
					SOCIPC_SyncOSDAdjustHandler( SI_OSD_SYNC_SEL_INPUT_MAIN );
					//gm_Print("L port is %d", gAthena_B_C_Parameters.PortStatus);
					gm_Delay10ms(1);
					SocIPCGetSlavePortStatus(SOCIPC_DEVICEL_DST_ADDRESS);
				}
				UserPrefCurrentInputMain = StorePort;
				//gm_SetRegBitsWord(PB_CTRL, PB_MAIN_ENABLE);
				//gm_ClearRegBitsWord(PB_CTRL, PB_FORCE_MAIN_BKGND);
				//gm_ForceUpdate(CH_A , INPUT_OUTPUT_CLOCK);
			}
			break;
			
		case PBP_HDMI_DP1: // 3:
         if(GetChipID_4K2K()==CHIPID_4K2K_R)
			{
				//Dp1ExtPortSetup_ID0();
				UserPrefCaptureM = CAPTURE_MODE_HALF_HORIZONTAL;
				SOCIPC_SyncOSDAdjustHandler(SI_OSD_SYNC_SAVE_PBP_Mode);	

				StorePort = ALL_BOARDS_DP1;
				if(UserPrefCurrentInputMain == ALL_BOARDS_DP1)
					//gm_4K2K_MainCapture();
					ModeSetupRequest(gmvb_MainChannel, REQ_MS_SETUP, PARAM_NONE, 0);
				else
				{
					UserPrefCurrentInputMain = ALL_BOARDS_DP1;
					AdjustCurrentInputMain();
				}

				//StorePort = UserPrefCurrentInputMain;
				//if(UserPrefCurrentInputMain == 11)
				//	UserPrefCurrentInputMain = 10;
				//else
				UserPrefCurrentInputMain = ALL_BOARDS_DP2;	// Hdmi
				
				//SOCIPC_SyncOSDAdjustHandler( SI_OSD_SYNC_SEL_INPUT_MAIN );	
				while(gAthena_LEFT_FE_Slave_Parameters.PortStatus != UserPrefCurrentInputMain)
				{
					SOCIPC_SyncOSDAdjustHandler( SI_OSD_SYNC_SEL_INPUT_MAIN );
					//gm_Print("L port is %d", gAthena_B_C_Parameters.PortStatus);
					gm_Delay10ms(1);
					SocIPCGetSlavePortStatus(SOCIPC_DEVICEL_DST_ADDRESS);
				}
				UserPrefCurrentInputMain = StorePort;
				//gm_SetRegBitsWord(PB_CTRL, PB_MAIN_ENABLE);
				//gm_ClearRegBitsWord(PB_CTRL, PB_FORCE_MAIN_BKGND);
				//gm_ForceUpdate(CH_A , INPUT_OUTPUT_CLOCK);
			}
			break;
			
		case PBP_HDMI_DP2: // 4
			if(GetChipID_4K2K()==CHIPID_4K2K_R)
			{
				//Dp1ExtPortSetup_ID1();
				UserPrefCaptureM = CAPTURE_MODE_HALF_HORIZONTAL;
				SOCIPC_SyncOSDAdjustHandler(SI_OSD_SYNC_SAVE_PBP_Mode);	

				StorePort = ALL_BOARDS_DP3;
				if(UserPrefCurrentInputMain == ALL_BOARDS_DP3)
					//gm_4K2K_MainCapture();
					ModeSetupRequest(gmvb_MainChannel, REQ_MS_SETUP, PARAM_NONE, 0);
				else
				{
					UserPrefCurrentInputMain = ALL_BOARDS_DP3;
					AdjustCurrentInputMain();
				}

				//StorePort = UserPrefCurrentInputMain;
				//if(UserPrefCurrentInputMain == 11)
				//	UserPrefCurrentInputMain = 10;
				//else
				UserPrefCurrentInputMain = ALL_BOARDS_DP2;	// Hdmi
				
				//SOCIPC_SyncOSDAdjustHandler( SI_OSD_SYNC_SEL_INPUT_MAIN );	
				while(gAthena_LEFT_FE_Slave_Parameters.PortStatus != UserPrefCurrentInputMain)
				{
					SOCIPC_SyncOSDAdjustHandler( SI_OSD_SYNC_SEL_INPUT_MAIN );
					//gm_Print("L port is %d", gAthena_B_C_Parameters.PortStatus);
					gm_Delay10ms(1);
					SocIPCGetSlavePortStatus(SOCIPC_DEVICEL_DST_ADDRESS);
				}
				UserPrefCurrentInputMain = StorePort;
			}
			break;
	}
	

	//gm_Print("PBP Mode = ... %d", mode); 
}


#define glyph_N	17
#define glyph_T	18
#define glyph_S	19
#define glyph_C	20
#define glyph_P	21
#define glyph_A	22
#define glyph_L	23
#define dTimingInfoSecamOffset		26
#define glyph_i_lower	28	//Hyb130613* Org = 31
#define glyph_p_lower	32	//Hyb130613* Org = 32
#define dTimingInfoNoSignalOffset		34

extern WORD FE_HWidth[2];
extern WORD FE_VLength[2];
extern WORD FE_VFreq[2];
extern WORD FE_InputType[2];

void DrawSTimingInfo(void)
{
	gmt_OSD_TIMINGINFO_CTRL ROM *Sp_TimingInfoCtrl;
	BYTE ROM *Bp_ColorMap;
	BYTE B_NumOfGlyphs = 0;
	BYTE Ba_TextString[20] = {0};  //that's displaying 20 digits
	WORD W_XOffset, W_YOffset;
//    WORD Wa_CtrlProp[4];
	BYTE i = 0;
	BYTE TestChar[500];
	BYTE ROM *Bp_Text;
	//WORD w_vfreq;
	BYTE B_Input;	
	BYTE B_Channel;
	WORD ST4Kx2K_HWidth[2];
	WORD ST4Kx2K_VLength[2];
	WORD ST4Kx2K_VFreq[2];
	BOOL IsVideoMode = 0;
	BOOL ISInterlace = 0;
	
	// Get pointer
	gm_OsdGetActionOperand();
	Sp_TimingInfoCtrl = (gmt_OSD_TIMINGINFO_CTRL ROM *)gmv_ActionOpPtr;
	Bp_Text = Sp_TimingInfoCtrl->UsedGlyphs;
       //gm_Print("DrawDTimingInfo",0);
	// We uncompress the text string if Gb_Osd_Compr_Txt is set.
	if(gmc_OsdComprTxt)
	{
		gm_GmcaCycleDecompress((BYTE far *)Bp_Text, gmv_OsdTextStringDecompBuffer, DecompressWrapper);
		Bp_Text = (BYTE ROM *)gmv_OsdTextStringDecompBuffer;
	}

	if(Sp_TimingInfoCtrl->Channel == 0)
	{
		B_Input = UserPrefCurrentInputMain;
		B_Channel = gmvb_MainChannel;
	}
	else
	{
		B_Input = UserPrefCurrentInputPip;
		B_Channel = gmvb_PipChannel;
	}	


	if(!gm_IsValidSignal(B_Channel))
	{

		for (i = 0; i < 9; i++)
		{
			Ba_TextString[i] = *(Bp_Text + 2 + dTimingInfoNoSignalOffset + i);
		}
		B_NumOfGlyphs = i;	
		
		// Calculate offsets for alignment
		W_XOffset = CalculateXAlignmentOffset(B_NumOfGlyphs, Sp_TimingInfoCtrl->Horizontal, Ba_TextString, *(Bp_Text), Sp_TimingInfoCtrl->XSize);

		W_YOffset = CalculateYAlignmentOffset(Sp_TimingInfoCtrl->Vertical, *(Bp_Text), Sp_TimingInfoCtrl->YSize);
			
		Bp_ColorMap = Sp_TimingInfoCtrl->Colors;

		// Check to see if a color mapping exists.
		if(Bp_ColorMap != NULL_PTR)
		{
			Bp_ColorMap += 2;
		}

		#ifndef ROTATION_ON_THE_FLY
			DrawRotatedText((BYTE ROM *)Sp_TimingInfoCtrl, Bp_ColorMap, W_XOffset, W_YOffset, (BYTE)*(Bp_Text), (BYTE ROM*)Ba_TextString, B_NumOfGlyphs);
		#else
			DrawRotatedText((BYTE ROM *)Sp_TimingInfoCtrl, Bp_ColorMap, W_XOffset, W_YOffset, *Bp_Text, Ba_TextString, B_NumOfGlyphs );
		#endif	//ROTATION_ON_THE_FLY
		return;
	}

	if((B_Input == ALL_BOARDS_DP1)||(B_Input == ALL_BOARDS_DP3))	// DP
	{
		if(gm_IsVideo(B_Channel))
			IsVideoMode = TRUE;

		if (gm_IsInterlaced(B_Channel))
			ISInterlace = TRUE;
	}
	else if(B_Input == ALL_BOARDS_DP2) 	// FE HDMI
	{
		if((FE_InputType[B_Channel] & gmd_MODE_SIGNAL_MASK) > gmd_MODE_GRAPHICS)
			IsVideoMode = TRUE;

		if(FE_InputType[B_Channel] & gmd_MODE_INTERLACED)
		{
			IsVideoMode = TRUE;
			ISInterlace = TRUE;
		}

		if(FE_HWidth[B_Channel] == 0)
		{
			if(SocIPCGetSlaveTimingInfo(SOCIPC_DEVICEFE_DST_ADDRESS))
			{
				//gm_Print("Read FE info error, read again", 0);
				FE_HWidth[B_Channel] = gAthena_LEFT_FE_Slave_Parameters.HWidth;
				FE_VLength[B_Channel] = gAthena_LEFT_FE_Slave_Parameters.VLength;
				FE_VFreq[B_Channel] = gAthena_LEFT_FE_Slave_Parameters.VFreq;
				FE_InputType[B_Channel] = gAthena_LEFT_FE_Slave_Parameters.ModeFlags;
			}

			if(FE_HWidth[B_Channel] == 0)
			{
				gm_Print("Read FE info error, Communication abnormal", 0);
			}
		}
	}

	if(IsVideoMode)								
	{
              //gm_Print("Draw Vodeo timing",0);    

		//if((B_Input == ALL_BOARDS_COMP1) || (B_Input == ALL_BOARDS_DP1) || (B_Input == ALL_BOARDS_DP3) ||
		//	(B_Input == ALL_BOARDS_HDMI) || (B_Input == ALL_BOARDS_DVI1) || 
		//	(B_Input == ALL_BOARDS_DVI2) || (B_Input == ALL_BOARDS_VGA1))	
      {

         if((B_Input == ALL_BOARDS_DP1)||(B_Input == ALL_BOARDS_DP3))
			 	ST4Kx2K_VLength[B_Channel] = gmvw_InputVActiveOrg[B_Channel] = gm_ReadRegWord(DP12RX0_MS_VACT_WIDTH);
			else if (B_Input == ALL_BOARDS_DP2)
				ST4Kx2K_VLength[B_Channel] = FE_VLength[B_Channel];
				
			
			if (ISInterlace)
			{
				WORD W_VActive;

				//gm_Print("--------%di ", ST4Kx2K_VLength[B_Channel]<< 1);			
				// convert height from number to glyphs
				//W_VActive = CheckVideoTimingResolution(B_Channel, gmvw_InputVActiveOrg[B_Channel]);
				W_VActive = CheckVideoTimingResolution(B_Channel, ST4Kx2K_VLength[B_Channel]);
				if(W_VActive == 0)
					W_VActive = ST4Kx2K_VLength[B_Channel];
				B_NumOfGlyphs += NumberToGlyphs((DWORD)(W_VActive<< 1), Ba_TextString + B_NumOfGlyphs, (BYTE far *)(Bp_Text + 2));

				Ba_TextString[B_NumOfGlyphs++] = *(Bp_Text + 2 + glyph_i_lower);
			}
			else// Progressive mode  // only show input Height + p
			{
				WORD W_VActive;
				//gm_Print("--------%dP ", ST4Kx2K_VLength[B_Channel]);			

				//W_VActive = CheckVideoTimingResolution(B_Channel, gmvw_InputVActiveOrg[B_Channel]);
				W_VActive = CheckVideoTimingResolution(B_Channel, ST4Kx2K_VLength[B_Channel]);
				if(W_VActive == 0)
					W_VActive = ST4Kx2K_VLength[B_Channel];
				
				// convert height from number to glyphs
				B_NumOfGlyphs += NumberToGlyphs((DWORD)W_VActive, Ba_TextString + B_NumOfGlyphs, (BYTE far *)(Bp_Text + 2));

				Ba_TextString[B_NumOfGlyphs++] = *(Bp_Text + 2 + glyph_p_lower);
			}
		}
	}
	else 
	{
	
//              gm_Print("Draw Graphic timing",0);     
	#if 1 //def 
		if(B_Channel == gmvb_MainChannel)
		{
			if((UserPrefCurrentInputMain == ALL_BOARDS_DP1)||(UserPrefCurrentInputMain == ALL_BOARDS_DP3))
			{			
			   	gmvw_InputHActiveOrg[B_Channel] = gm_ReadRegWord(DP12RX0_MS_HACT_WIDTH);
 		  	      gmvw_InputVActiveOrg[B_Channel] = gm_ReadRegWord(DP12RX0_MS_VACT_WIDTH);

					if(DpRxIsMST_Stream(UserPrefCurrentInputMain)==FALSE)
						ST4Kx2K_HWidth[B_Channel] = gmvw_InputHActiveOrg[B_Channel]; // Test 0312
					else
						ST4Kx2K_HWidth[B_Channel] = gmvw_InputHActiveOrg[B_Channel] * 2; 

						//gm_Print("ST4Kx2K_HWidth[B_Channel] = .......... %d", ST4Kx2K_HWidth[B_Channel]);
					
					ST4Kx2K_VLength[B_Channel] = gmvw_InputVActiveOrg[B_Channel];
			}
			else if(UserPrefCurrentInputMain == ALL_BOARDS_DP2)
			{

				ST4Kx2K_HWidth[B_Channel] = FE_HWidth[B_Channel];
				ST4Kx2K_VLength[B_Channel] = FE_VLength[B_Channel];
			}
		}
	 	else
	 	{
			if((UserPrefCurrentInputPip == ALL_BOARDS_DP1)||(UserPrefCurrentInputPip == ALL_BOARDS_DP3))			
			{			
			   	gmvw_InputHActiveOrg[B_Channel] = gm_ReadRegWord(DP12RX0_MS_HACT_WIDTH);
 		  	      gmvw_InputVActiveOrg[B_Channel] = gm_ReadRegWord(DP12RX0_MS_VACT_WIDTH);

					if(DpRxIsMST_Stream(UserPrefCurrentInputMain)==FALSE)
						ST4Kx2K_HWidth[B_Channel] = gmvw_InputHActiveOrg[B_Channel]; // Test 0312
					else
						ST4Kx2K_HWidth[B_Channel] = gmvw_InputHActiveOrg[B_Channel] * 2; 
					
					ST4Kx2K_VLength[B_Channel] = gmvw_InputVActiveOrg[B_Channel];
			}
			else if(UserPrefCurrentInputPip == ALL_BOARDS_DP2)
			{

				ST4Kx2K_HWidth[B_Channel] = FE_HWidth[B_Channel];
				ST4Kx2K_VLength[B_Channel] = FE_VLength[B_Channel];
			}
	 	}		
	#endif	
		
		gmvw_InputHActiveOrg[B_Channel] &= 0xFFFE;
		
	 	if(UserPrefOsdRotation == OSD_NON_ROTATED) 
		{	
			// Convert width from number to glyphs
			//B_NumOfGlyphs += NumberToGlyphs((DWORD)gmvw_InputHActiveOrg[B_Channel], Ba_TextString + B_NumOfGlyphs, (BYTE far *)(Bp_Text + 2));
			B_NumOfGlyphs += NumberToGlyphs((DWORD)ST4Kx2K_HWidth[B_Channel], Ba_TextString + B_NumOfGlyphs, (BYTE far *)(Bp_Text + 2));
			
			Ba_TextString[B_NumOfGlyphs++] = *(Bp_Text + 2 + glyph_x);

			if( ST4Kx2K_VLength[CH_A]==722) //force to show 1280x720  timing info for CVT mode
				B_NumOfGlyphs += NumberToGlyphs((DWORD)720, Ba_TextString + B_NumOfGlyphs, (BYTE far *)(Bp_Text + 2));
			else  // convert height from number to glyphs
				B_NumOfGlyphs += NumberToGlyphs((DWORD)ST4Kx2K_VLength[B_Channel], Ba_TextString + B_NumOfGlyphs, (BYTE far *)(Bp_Text + 2));
		}
		else
		{
			// Convert width from number to glyphs
			B_NumOfGlyphs += NumberToGlyphs((DWORD)ST4Kx2K_HWidth[B_Channel], Ba_TextString + B_NumOfGlyphs, (BYTE far *)(Bp_Text + 2));

			Ba_TextString[B_NumOfGlyphs++] = *(Bp_Text + 2 + glyph_x);

			// convert height from number to glyphs
			B_NumOfGlyphs += NumberToGlyphs((DWORD)ST4Kx2K_VLength[B_Channel], Ba_TextString + B_NumOfGlyphs, (BYTE far *)(Bp_Text + 2));
		}
		//Debugmessage_i("--------B_NumOfGlyphs=%d ",B_NumOfGlyphs);

		Ba_TextString[B_NumOfGlyphs++] = *(Bp_Text + 2 + glyph_sp);
		Ba_TextString[B_NumOfGlyphs++] = *(Bp_Text + 2 + 33);
		Ba_TextString[B_NumOfGlyphs++] = *(Bp_Text + 2 + glyph_sp);

		//gm_Print("stInput[B_Channel].Vfrq ......... %d", stInput[B_Channel].Vfrq);

		if(UserPrefCurrentInputMain == ALL_BOARDS_DP2)
			ST4Kx2K_VFreq[B_Channel] = FE_VFreq[B_Channel];
		else
			ST4Kx2K_VFreq[B_Channel] = stInput[B_Channel].Vfrq;
		
		//gm_Print("~~~~ ST4Kx2K_VFreq[B_Channel] = %d", ST4Kx2K_VFreq[B_Channel]);
		{
			DWORD DW_TempVHz = (DWORD)ST4Kx2K_VFreq[B_Channel]/10;
			//DWORD DW_TempVHz = (DWORD)(stInput[B_Channel].Vfrq%10)/10;
			if((ST4Kx2K_VFreq[B_Channel]%10) >= 4)
				DW_TempVHz += 1;
			if(DW_TempVHz == 61)
				DW_TempVHz = 60;
			B_NumOfGlyphs += NumberToGlyphs(DW_TempVHz, Ba_TextString + B_NumOfGlyphs, (BYTE far *)(Bp_Text + 2));
		}

		Ba_TextString[B_NumOfGlyphs++] = *(Bp_Text + 2 + glyph_H);
		Ba_TextString[B_NumOfGlyphs++] = *(Bp_Text + 2 + glyph_z);

	}

	// Calculate offsets for alignment
	W_XOffset = CalculateXAlignmentOffset(B_NumOfGlyphs, Sp_TimingInfoCtrl->Horizontal, Ba_TextString, *(Bp_Text), Sp_TimingInfoCtrl->XSize);
	if(W_XOffset > 100) //20130312 ... Debug later ... It over flow after port change
		W_XOffset = 32;

	W_YOffset = CalculateYAlignmentOffset(Sp_TimingInfoCtrl->Vertical, *(Bp_Text), Sp_TimingInfoCtrl->YSize);


		//gm_Print("W_XOffset ........... %d", W_XOffset);


	Bp_ColorMap = Sp_TimingInfoCtrl->Colors;

	// Check to see if a color mapping exists.
	if(Bp_ColorMap != NULL_PTR)
	{
		Bp_ColorMap += 2;
	}

#ifndef ROTATION_ON_THE_FLY
	DrawRotatedText((BYTE ROM *)Sp_TimingInfoCtrl, Bp_ColorMap, W_XOffset, W_YOffset, (BYTE)*(Bp_Text), (BYTE ROM*)Ba_TextString, B_NumOfGlyphs);
#else
	DrawRotatedText((BYTE ROM *)Sp_TimingInfoCtrl, Bp_ColorMap, W_XOffset, W_YOffset, *Bp_Text, Ba_TextString, B_NumOfGlyphs );
#endif	//ROTATION_ON_THE_FLY
}

WORD CheckVideoTimingResolution(gmt_PHY_CH B_Channel, WORD W_VActive)
{
	#define TimingTHold			8
	WORD W_VLength = 0;

	if(gm_IsVideo(B_Channel))
	{
		if(gm_IsInterlaced(B_Channel))		// interlace 480i, 576i, 1080i
		{
			if(abs(W_VActive - 240)< TimingTHold)
				W_VLength = 240;
			else if(abs(W_VActive - 288)< TimingTHold)
				W_VLength = 288;
			else if(abs(W_VActive - 540)< TimingTHold)
				W_VLength = 540;
			else
				W_VLength = 0;
		}
		else
		{
			if(abs(W_VActive - 480)< TimingTHold)
				W_VLength = 480;
			else if(abs(W_VActive - 576)< TimingTHold)
				W_VLength = 576;
			else if(abs(W_VActive - 720)< TimingTHold)
				W_VLength = 720;
			else if(abs(W_VActive - 1080)< TimingTHold)
				W_VLength = 1080;
			else
				W_VLength = 0;			
		}
	}
	return 	W_VLength;
}
#endif 

//*********************************  END  **************************************
