/*
    $Workfile:   PortSelection.c  $
    $Revision: 1.63 $
    $Date: 2013/03/26 05:43:49 $
*/
#define __PORTSELECTION_C__
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
// MODULE:      PortSelection.c
//
// USAGE:       This module contains function that is used for Input Port
//                Selection.  Which will setup the following
//                1. Input MUX config for video path
//                2. Audio path for selected video
//                3. Load selected port ADC calibration values
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#include "System\All.h"
#include "StdLibInc\gm_Register.h"
#include "StdLibInc\gm_GlobalVariables.h"
#include "Board\PortAttrib.h"
//#include "Main\AppGlobalVariables.h"	// Application Globals
//#include "System\ScartInputHandler.h"

//******************************************************************************
//  D E F I N E S
//******************************************************************************

//#define _DEBUG_MSG
#ifdef _DEBUG_MSG
#define msg(x)            gm_Print(x,0)
#define msg_i(x,y)        gm_Print(x,y)
#else
#define msg(x)
#define msg_i(x,y)
#endif

//******************************************************************************
//  L O C A L    F U N C T I O N S
//******************************************************************************

void PortChangeCommon(gmt_PHY_CH B_Channel, BYTE B_Port);
#ifdef ST_4K2K_93xx_BOARD //Add by ChengLin 20130326
void PatchOf_4K2K_AVDDS_Toggle(BYTE B_Channel);
#endif

//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************

BOOL B_MainCcFieldChangeFlag;
BOOL B_PipCcFieldChangeFlag;

#if (FEATURE_CEC == ENABLE)
extern WORD UserExecutionTimeoutMain;
extern WORD UserExecutionTimeoutPip;
extern BYTE B_UserCommandExecution;
#endif

#ifdef VIDEO_CAPTURE
extern BYTE CDATA_Flag;
#endif

#if(FEATURE_SOURCE_SCAN == ENABLE)
extern BYTE SourceScanStatus;//120608 Edward for selectinput source scan check
#endif
//IF Compensation curve selection
/*================================================================|
                        NOTES:
==================================================================|
PS: Refer to the Document IF_Compensation_Block_FrequencyResponse_Curves.doc in
    GmAFEVidec\Documents\Decoder_Design .

1. Figure 1 and 2 refer to the High pass filter Frequency curves for
    NTSC tuner Compensation. Figure 3 and 4 refer to the High pass filter Frequency curves for
    PAL tuner Compensation.

2. As per the tuner low pass frequency magnitude response , a corresponding
    High pass frequency magnitude response is to be selected for the IF compensation
   to work correctly by the decoder.

3. There are 16 frequency response curve selections available for each
    tuner.

4.  Value of 1-8 selects the curves of the Figure 1 and 9-16 selects the curves from
     Figure 2.
    1  - Lower curve/response of Figure 1.
    8  - Upper curve/response of Figure 1.
    9  - Lower curve/response of Figure 2.
    16 - Upper curve/response of Figure 2.
    Same holds good for both NTSC and PAL curves.

Please Note : This option is meant only for customer factory people and not meant for software development trials.
Based on the recomendation of the factory its one time set. This is beacause only based on tuner characteristics this
compensation is to be done ( In Production Line ).
===================================================================*/
#define FREQ_CURVE_SELECT ( 0x07 )

//******************************************************************************
//  C O D E
//******************************************************************************

//******************************************************************************
//
// FUNCTION       : void InitInput(void)
//
// USAGE          : Initializes the input variable and pointers
//                  (Note: MUST be initialized in the ApplicationInit before library
//                  initalization start)
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : gmvb_NumOfConnectors        (W)
//                  gmvb_NumOfDecoderRegs        (W)
//                  gmvs_InputConfigTblPtr        (W)
//                  gmvw_AFERegAddressPtr        (W)
//                  gmvb_AFERegistersValsPtr    (W)
//                  gmvb_AdcUsagePtr            (W)
//                  gB_CurrentInputConnector       (W)
//                  gmvs_InputConfigTbl               (R)
//                  gmvw_AFERegAddress               (R)
//                  gmvb_AFERegistersVals           (R)
//                  gmvb_AdcUsage                (R)
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void InitInput(void)
{
   //Initialization - For use by Library
   gmvs_InputConfigTblPtr     = gmvs_InputConfigTbl;
   gmvb_AFERegistersValsPtr= gmvb_AFERegistersVals;
   gmvb_AdcUsagePtr         = gmvb_AdcUsage;
   gB_CurrentInputConnector[CH_A] = NOT_USED;
   gB_CurrentInputConnector[CH_B] = NOT_USED;
}

//******************************************************************************
//
// FUNCTION       : void SelectInput(gmt_PHY_CH B_Channel, gmt_UserDefConnNames B_InputConnector,
//                         gmt_PortSelectMode B_SelectMode)
//
// USAGE          : Call this function to Connect given input to Main or Pip Channel
//
// INPUT          : B_Channel - Current input channel
//                  B_InputConnector - Input Connector to be connected
//                  B_SelectMode - Port selection mode
//
// OUTPUT         : None
//
// GLOBALS        : gmvb_MainChannel (R)
//                  gmvb_PipChannel    (R)
//                  gmvw_InputModeAttr (W)
//                  gB_CurrentInputConnector (W)
//                  gmvb_InputConnector (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void SelectInput(gmt_PHY_CH B_Channel, gmt_UserDefConnNames B_InputConnector, gmt_PortSelectMode B_SelectMode)
{
   PortAttrib ROM *pPort = &AppPortAttrib[B_InputConnector];
   UserPrefFreeze = 0; //setting UserPrefFreeze to unfreeze on port change

   SysChFlag[B_Channel] |= SysCh_InputChange;
   PortChangeCommon(B_Channel, B_InputConnector);

#if FEATURE_AIP == ENABLE
   if ((B_SelectMode & NORMAL_MODE) && (GetPortProc(B_InputConnector) == AIP)
      && (GetPortProc(gmvb_InputConnector[gm_OtherChannel(B_Channel)]) == AIP)
      && (GetPortConnType(B_InputConnector) != GetPortConnType(gmvb_InputConnector[gm_OtherChannel(B_Channel)])))
   {
      msg_i("Input Collision - AIP",0);
      if (gm_IsValidSignal(gm_OtherChannel(B_Channel)))
         return;

      if (B_Channel == CH_B)
         return;
   }
#endif
	SetDisplayOff(B_Channel,  BLENDER_OFF);   // Neil 140701 for "No signal" background issue
   //If the pip is not supported,then there is no point to change it
#ifdef NO_PIP_SUPPORT
   if (B_Channel == CH_B)
   return;
#endif

   if (B_Channel == CH_A)
      gm_ClearRegBitsWord(MC_FIFO_EN_lo, MC_FIFO_MAIN_WR);
   else
      gm_ClearRegBitsWord(MC_FIFO_EN_lo, MC_FIFO_PIP_WR);
      
   gB_CurrentInputConnector[B_Channel] = B_InputConnector; // Save current port number
	#ifdef PORT_CHANGE_SEAMLESS
		if(B_Channel == gmvb_MainChannel)	//only save main channel input
			CurrentInputOnScreen = B_InputConnector;
	#endif

#ifdef VIDEO_CAPTURE
   if (CDATA_Flag == 4)
   {
      CDATA_Flag = 9;
   }
#endif

#ifdef AUTO_SCAN_INPUT
   if (B_SelectMode & (NORMAL_MODE | SMART_SWAP_MODE))
      AutoScanInit(B_Channel);
#endif

#if (FEATURE_AIP == ENABLE) && (FEATURE_ADC_AOC == ENABLE)
   // Disable automatic offset compensation
   if ((B_SelectMode & NORMAL_MODE) && (GetPortProc(B_InputConnector) == AIP))
      gm_AOCEnable(gmd_ADC_0, FALSE);
#endif

   // Clear current mode status
   if (B_SelectMode & NORMAL_MODE)
        CLRBIT(stInput[B_Channel].ModeFlags, gmd_MODE_VALID | gmd_MODE_COMPLETE|gmd_MODE_OUT_RANGE|gmd_MODE_INTERLACED);		//Neil120801* Add gmd_OUT_OF_RANGE

   // Control display off and set system to freerun
   if (B_SelectMode & (NORMAL_MODE | CLONE_CH_MODE))
   {
      PreModeChange(B_Channel);
      ModeSetupFreerun(B_Channel);
   }
#if FEATURE_AIP == ENABLE
   // Setup clamp method
   if (GetPortProc(B_InputConnector) == AIP)
      gmvb_SyncDetectClamp[gmd_ADC_0] = (B_SelectMode & (CLONE_CH_MODE | SMART_SWAP_MODE)) ? FALSE : TRUE;
#endif

   // Clear CC when port changed (when CC is enabled)
   CC_BlockDisplay(B_Channel);
#if (FEATURE_HDMI == ENABLE)
   HdmiDviCloseInactivePort();
#endif

   if (GetPortProc(B_InputConnector) == DIP)
   {
      gm_DipConfig(B_Channel,B_InputConnector, ((gmt_DipProperties  far *)
			(GetPortPropertyTable(B_InputConnector)))->B_RGBSwap);
	#ifdef EXTERNAL_DECODER
	#if 1//(DEFAULT_DIP_DAUGHTER_BOARD_OUT == TVP5160_YCbCr422)
	   if(B_Channel == gmvb_MainChannel)
	   {
		   gm_SetRegBitsWord(MIFM_CTRL, 0x7000);
		   //Hyb130429 clear IMP_IBD_MEASWINDOW_EN for digital input otherwise will no signal change from video port to other ports;
		   //gm_ClearAndSetBitsWord(IMP_IBD_CONTROL, IMP_MEASURE_DE_nDATA, IMP_IBD_MEASWINDOW_EN);
		   gm_ClearRegBitsWord(IMP_IBD_CONTROL, IMP_IBD_MEASWINDOW_EN);
	   }
	   else
	   {
		   gm_SetRegBitsWord(PIFM_CTRL, 0x8000);
		   //gm_ClearAndSetBitsWord(IPP_IBD_CONTROL, IMP_MEASURE_DE_nDATA, IMP_IBD_MEASWINDOW_EN);
		   gm_ClearRegBitsWord(IPP_IBD_CONTROL, IMP_IBD_MEASWINDOW_EN);
	   }
	#endif
	   
	   gm_IbdConfig(B_Channel,gmd_IBD_THRESHOLD,0x05);		//Why set to 0x0A before? ... 
	   
	   if(B_Channel == gmvb_MainChannel)
	   {
		   gm_ClearRegBitsWord(IMP_IBD_CONTROL, IMP_IBD_EN);
		   gm_SetRegBitsWord(IMP_IBD_CONTROL, IMP_IBD_EN);
	   }
	   else
	   {
		   gm_ClearRegBitsWord(IPP_IBD_CONTROL, IPP_IBD_EN);
		   gm_SetRegBitsWord(IPP_IBD_CONTROL, IPP_IBD_EN);
	   }	
	#if 0//def TVP5160_DEV_ID 	//Hyb130515- since we move it to LPM
		//Avoid initial failed in any case;
		if(Mission_TVP5160ReadReg(0x33) != 0x41)
			Mission_TVP5160_InitDecoder();
	#endif

		#if DECODER_IN_LPM
		Mission_TVP5160SetVideoInput(B_Channel, B_InputConnector);
		#else
			#if ((defined(TVP5160_DEV_ID)))
			MIS_TVP5160_ChangeToPort(B_Channel, B_InputConnector);
			#else
			MIS_TVP5151_ChangeToPort(B_Channel, B_InputConnector);
			#endif
		#endif
	#endif
	  
    }

#if 0//FEATURE_AIP == ENABLE   
   else if ((GetPortProc(B_InputConnector) == AIP) && (B_SelectMode & (CLONE_CH_MODE | SMART_SWAP_MODE)))
   {
      gm_AipClockSourceSelect(B_Channel, B_InputConnector, B_SelectMode);
   }
#endif
   else
   {
	   //Hyb130429 clear IMP_IBD_MEASWINDOW_EN for digital input otherwise will no signal change from video port to other ports;
	   if(B_Channel == gmvb_MainChannel)
		   gm_ClearRegBitsWord(IMP_IBD_CONTROL, IMP_IBD_MEASWINDOW_EN); //no port need setup this bits after test;
	   else
		   gm_ClearRegBitsWord(IPP_IBD_CONTROL, IMP_IBD_MEASWINDOW_EN);//no port need setup this bits after test;
	   
	   #if 0 // (FEATURE_DISPLAY_PORT == ENABLE)
	      if (GetPortConnType(B_InputConnector) == CT_DISPLAY_PORT)
	         DpRxUpdaateVddsInitFreq();
	   #endif
	   #if (FEATURE_AIP == ENABLE)
	      if (GetPortProc(B_InputConnector) == AIP)
	      {
	         gm_AipClockSourceSelect(B_Channel, B_InputConnector, B_SelectMode);
	      }
	   #endif
	      gm_InputConfigPort(B_Channel, B_InputConnector);// Configure AFE MUX

	#ifdef ST_4K2K_93xx_BOARD //Add by ChengLin 20130326
	      PatchOf_4K2K_AVDDS_Toggle(B_Channel);
	#endif  
		//ReInit_AVDDS(B_Channel);

	   #if (FEATURE_HDMI == ENABLE)
	      if ((GetPortConnType(B_InputConnector) == HDMI) || (GetPortConnType(B_InputConnector) == DVI))
	      {
	         HdmiDviInputPortInit(B_Channel,B_InputConnector);
			ReInitHDMIVDDS(B_Channel);
	      }
	   #endif //#if (FEATURE_HDMI == ENABLE)

#if (FEATURE_DISPLAY_PORT == ENABLE)
	      if (GetPortConnType(B_InputConnector) == CT_DISPLAY_PORT)
	      {
	         DpRxPortChange(B_InputConnector);
	         DpRxUpdaateVddsInitFreq();
	      }
#endif
   }

#if  (DPRX_PANEL != ENABLE)	//Disable set as USE_DPTX_DYNAMIC_SWITCH// move to init;
#if (FEATURE_DISPLAY_PORT == ENABLE) && (SELECT_DPTX_TYPE==USE_DPTX_DYNAMIC_SWITCH)
      if (B_Channel == gmvb_MainChannel)
      {
         if(gm_GetDpDigitalID(B_InputConnector)==DID_DPRX0)
         {
            DPTxPortSwitch (iDpTxPort,MstTxPort);
         }
         else
         {
            DPTxPortSwitch (MstTxPort, iDpTxPort);
         }
      }
#endif
#endif	//(DPRX_PANEL == ENABLE)
    // Set up port attributes via port attribute table
    gm_SetDeviceId(B_Channel, pPort->B_DeviceID); // Set Internal/External Decoder
    
	#ifdef DELL_U2410F
	//Neil120818+ restart audio detection if change on current port
	if (((UserPrefAudioMainInput == gmd_AUDIO_SRC_AUTO_Main) && (B_Channel == CH_A))
	 || ((UserPrefAudioMainInput == gmd_AUDIO_SRC_AUTO_Pxp)  && (B_Channel == CH_B)))
   	{
		AUDIO_MUTE();
		B_AudioTimerCtrl &= ~Aud_Tmr_AudSampleRateChange;	//Neil120922+ for audio routine control
   		//gm_AUDIO_DisableVolume();
		//gm_AUDIO_UnstableRoutine(B_Channel);
	}

	//Neil120818+ for main 1080i input, change pxp port main image distortion;
    if(B_Channel == CH_B)
		gm_WriteRegWord(IPP_FRAME_RESET_LINE, 0);	//
	#endif
	//Neil120818* move here before restart mode detect engine for sync detected routine faster
    // Setup Madi specific parameters dependent on input port
    MadiSetup(B_Channel);

    // PortDependent structure needs to be retrieved every time input changes
    {
	#ifdef DELL_U2410F	// Modified by ST_hansPENG 090312
		BYTE B_Index;

		if (B_Channel == gmvb_MainChannel)
			B_Index = InputConvertScheme(UserPrefCurrentInputMain);
		else
			B_Index = InputConvertScheme(UserPrefCurrentInputPip);
	#else    
        #if (PORT_SCHEME_DEPENDENT == PORT_DEPENDENT)
            BYTE B_Index = gmvb_InputConnector[gmvb_MainChannel];
        #else
            BYTE B_Index = UserPrefImageScheme;
        #endif
	#endif

        if(CheckPortSchemeDependentEntry(B_Index))
        {
            msg_i("PSD: Port change: User preferences retreived. %d", B_SelectMode);
			
	#ifdef DELL_U2410F	// Modified by ST_hansPENG 090312
			if (B_Channel == gmvb_MainChannel)
			{
				UserPrefImageScheme = B_Index;
				ReadPortSchemeDependentEntry(B_Index); // PSD
				UserPrefTNR = UserPrefPortTNR;			//Hyb121226, TNR change to Port dependent
			}
			else
			{
				msg_i("PSD: ReadPortSchemeDependentEntry. B_Index = %d", B_Index);
				UserPrefPipImageScheme = B_Index;
				ReadPipPortSchemeDependent();
			}
			ReadPresetModesSettings();
			StorePresetModes(); //KevinL 20090417
	#else			
            ReadPortSchemeDependentEntry(B_Index); // PSD
            
            #if (PORT_SCHEME_DEPENDENT == PORT_DEPENDENT)
                UserPrefImageScheme = UserPrefMFMImageScheme;
            #endif
	#endif

        }
        else
            msg_i("PSD: Port change: User preferences WERE NOT retreived.", 0);        
    }
	#if (SPEEDUP_MODE_CHANGE == DISABLE)	//Hyb130608+ for reduce change port time, save it after image setup, ~ 40ms
	SaveModeIndependentSettings();		//Hyb130429 save for port change
	#endif
   // Setup decoder detection mode
   if (B_SelectMode & (SMART_SWAP_MODE | CLONE_CH_MODE))
   {
      // Special mode for channel clone and smart swap which suppress mode redetection
      // PLEASE NOTE: This function has the following embedded logic:
      // If (gmvb_InputConnector[B_Channel] == gmvb_InputConnector[OtherChannel]), then channel clone is
      //   assumed; otherwise, channel swap is assumed.
      // In the case of channel swap, the calls to SelectInput(Main, ?, SMART_SWAP_MODE) and
      //   SelectInput(Pip, ?, SMART_SWAP_MODE) are preferred to be one immediately after the other;
      //   otherwise, mismatches between software states and hardware settings may cause smart swap to
      //   fail.
      gm_StateDetectionConfig(B_Channel, DET_FORCED_STABLE);
   }
   else
   {
      if ((B_Channel == gmvb_MainChannel) && (pPort->B_DetectionConfig == DET_RESTART))  // SCR 2852
         B_SignalStatus = STATUS_NONE;
      gm_StateDetectionConfig(B_Channel, pPort->B_DetectionConfig); // Init and Restart mode detection
   }

   gm_ModeDetectionConfig(B_Channel, DET_STANDARD); // Enable standard mode detection
   gm_SetSyncTypeOrder(B_Channel, pPort->Ba_Sync[0], pPort->Ba_Sync[1], pPort->Ba_Sync[2], pPort->Ba_Sync[3]);
   gmvb_STABLE_COUNT[B_Channel] = pPort->B_StableCount;
   gmvb_UNSTABLE_COUNT[B_Channel] = pPort->B_UnstableCount;
   gm_IbdConfig(B_Channel, gmd_IBD_MEASURE_SOURCE, pPort->B_IbdSourceType);

   gm_SetAutoMode(B_Channel, pPort->B_AutoMode); // Set auto mute mode

   // Field ID generation - IFM block or external
   gm_SetIFMCtrlBitField((B_Channel == CH_A) ? MIFM : PIFM, gmd_IFM_INT_GEN_ODD_EVEN_FIELD_ID,
   (pPort->B_SyncFlags & gmd_SYNC_ODD_TYPE_EXT) ? FALSE : TRUE);

   // Delay HSync if the flag is set
   gm_SetIFMCtrlBitField((B_Channel == CH_A) ? MIFM : PIFM, gmd_IFM_OFFSET_HSYNC,
   (pPort->B_SyncFlags & gmd_SYNC_OFFSET_HSYNC) ? TRUE : FALSE);

   // Configure external inputs
   if (pPort->ConfigureExtDevices
      && (((B_Channel == gmvb_MainChannel) && (B_InputConnector != gmvb_InputConnector[gmvb_PipChannel]))
      || ((B_Channel != gmvb_MainChannel) && (B_InputConnector != gmvb_InputConnector[gmvb_MainChannel]))))
      (pPort->ConfigureExtDevices)(B_Channel, B_InputConnector);

   // Load port specific parameters
   if (B_SelectMode & NORMAL_MODE)
   {
      switch (GetPortConnType(B_InputConnector))
      {
         case DIPX:
            msg_i("***DIPX***", 0);
         break;

#if FEATURE_AIP == ENABLE
         case COMPONENT:
         {
            gmt_ADC_SETTING AdcSettings; // ADC nvram settings
            BOOL status = ReadAdcSettings(gmd_ADC_0, (BYTE*)&AdcSettings, ADC_SETTING_HD);
            gm_SetVPulseWidth(15);
            if (status)
               gm_SetAdcGainOffset(gmd_ADC_0, &AdcSettings);
            break;
         }

         case VGA:
         {
            gmt_ADC_SETTING AdcSettings; // ADC nvram settings
            BOOL status;

            gm_SetVPulseWidth(10);
            status = ReadAdcSettings(gmd_ADC_0, (BYTE*)&AdcSettings, ADC_SETTING_GRAPHIC);
            if (status)
               gm_SetAdcGainOffset(gmd_ADC_0, &AdcSettings);
            break;
         }
#endif // FEATURE_AIP
         case DVI:
            // Use correct sync polarity to ensure IBD hstart/vstart
            // measurments taken w.r.t. the proper reference point
            gm_ClearRegBitsWord(ip_source[B_Channel], (ip_hs_inv|ip_vs_inv));
            break;

         case HDMI:
            gm_SetRegBitsWord(ip_source[B_Channel],IMP_SOURCE_CLK_INV);
            break;

      }
      // Update ADC settings
   }
        
#if (FEATURE_CEC == ENABLE)
   if (B_Channel == gmvb_MainChannel)
   {
      UserExecutionTimeoutMain = 0;
      B_UserCommandExecution |= BIT0;  // main
   }
   else//(B_Channel == gmvb_PipChannel)
   {
      UserExecutionTimeoutPip = 0;
      B_UserCommandExecution |= BIT1;  // pip
   }
#endif
}

//******************************************************************************
//
// FUNCTION       : BOOL IsDecoderUsed(gmt_PHY_CH B_Channel)
//
// USAGE          : Call this function to check if decoder is used on
//                  current selected channel
//
// INPUT          : B_Channel - Channel to which the decoder is used
//
// OUTPUT         : TRUE or FALSE
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
BOOL IsDecoderUsed(gmt_PHY_CH B_Channel)
{
   BOOL RetVal;

   switch (GetChannelConnType(B_Channel))
   {
      case COMPOSITE:
      case SVIDEO:
      case TUNER:
         RetVal = TRUE;
         break;

      case COMPONENT:
      case VGA:
      case HDMI:
      default:
         RetVal = FALSE;
         break;
   }

   return(RetVal);
}



BOOL IsPortConflict(gmt_UserDefConnNames Port1, gmt_UserDefConnNames Port2)
{
   switch (GetPortProc(Port1))
   {
      case  AIP:
         if (GetPortProc(Port2)!=AIP)
            return FALSE;
         break;

      case DIP:
         if (GetPortProc(Port2)!=DIP)
            return FALSE;
		/*		//Hyb130413- if all from DIP then conflict, no else
		else
		{
			if(GetDIPPins(Port1) != GetDIPPins(Port2))
				return FALSE;
		}
		*/
         break;
#if (FEATURE_HDMI==ENABLE)
      case DVI_PORT:
         if (GetPortProc(Port2)!=DVI_PORT)
            return FALSE;
         else
         {
            if (GetPortConnType(Port1)!=GetPortConnType(Port2))
            {
               gmt_COMBOPHY_AID Aid_Port2=gm_GetHdmiDviAnaPhyID(Port2);
               switch (gm_GetHdmiDviAnaPhyID(Port1))
               {
                  case AID_PHY0:
                     if (Aid_Port2!=AID_PHY0)
                        return FALSE;
                     break;
                  case AID_PHY1:
                     if ((Aid_Port2!=AID_PHY1) && (Aid_Port2!=AID_PHY12))
                        return FALSE;
                     break;
                  case AID_PHY2:
                     if ((Aid_Port2!=AID_PHY2) && (Aid_Port2!=AID_PHY12))
                        return FALSE;
                     break;

                  case AID_PHY12:
                     if ((Aid_Port2!=AID_PHY1) && (Aid_Port2!=AID_PHY2))
                        return FALSE;
                     break;
               }
            }
         }
         break;
#endif
#if (FEATURE_DISPLAY_PORT==ENABLE)
      case DP_PORT:
         if (GetPortProc(Port2)!=DP_PORT)
            return FALSE;
         else
         {
            if (gm_GetDpDigitalID(Port1)!=gm_GetDpDigitalID(Port2))
               return FALSE;
         }
         break;
#endif
   }
   return TRUE;
}



gmt_UserDefConnNames MovePortFromConflictPort(gmt_UserDefConnNames Port)
{
	#if 0
   gmt_UserDefConnNames Port_MoveTo;
   BYTE i=NUM_MASTER_LIST;
#if 0
   if (GetPortProc(Port)!=AIP)
   {
      if (GetPortConnName(ALL_BOARDS_COMP1) != NOT_USED)
         return ALL_BOARDS_COMP1;

      if (GetPortConnName(ALL_BOARDS_VGA1) != NOT_USED)
         return ALL_BOARDS_VGA1;
   }
#endif
   Port_MoveTo=Port;
   while (i--)
   {
      if (Port_MoveTo)
         Port_MoveTo--;
      else
         Port_MoveTo=NUM_MASTER_LIST-1;
      if (GetPortConnName(Port_MoveTo) != NOT_USED)
      {
         if (Port_MoveTo!=Port)
         {
            if (IsPortConflict(Port, Port_MoveTo)==FALSE)

               return Port_MoveTo;
         }
      }
   }

   gm_Print("######## Cannot get a non-conflict port######",0);   //Note: this should not happen !!!
   return ALL_BOARDS_VGA1;
	#else
	Port = Port;
	return GetNextPxpPort();
   #endif   
}

//******************************************************************************
//
// FUNCTION       : BOOL IsMainSourceValid(BYTE B_DesiredMain)
//
// USAGE          : Call this function to check if Main source is valid (if it
//                  conflicts with Pip source)
//
// INPUT          : B_DesiredMain - Source that Main wants to change to
//
// OUTPUT         : TRUE or FALSE
//
// GLOBALS        : gmvb_PipChannel            (R)
//                  UserPrefPipMode            (R)
//                  UserPrefCurrentInputPip    (R)
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BOOL IsMainSourceValid(BYTE B_DesiredMain)
{
   if (GetPortConnName(B_DesiredMain) == NOT_USED)
   {
	  return FALSE;
   }

#ifdef CHANNEL_CLONE
	if(B_DesiredMain == UserPrefCurrentInputPip)
		return TRUE;
#endif

#ifndef CHANNEL_CLONE	 //Hyb130825 for channel clone
   if ((B_DesiredMain == UserPrefCurrentInputPip) && (UserPrefPipMode == NO_PIP))
   {
	  UserPrefCurrentInputPip=MovePortFromConflictPort(B_DesiredMain);
	  AdjustCurrentInputPip();
	  return TRUE;
   }
#endif


   if (IsPortConflict(B_DesiredMain, UserPrefCurrentInputPip)==TRUE)
	  return FALSE;
   
   return TRUE; //otherwize (all other case are allowed
}


//******************************************************************************
//
// FUNCTION       : BOOL IsPipSourceValid(BYTE B_DesiredPip)
//
// USAGE          : Call this function to check if Pip source is valid
//                  (if it conflicts with Main source)
//
// INPUT          : B_DesiredPip - Source that Pip wants to change to
//
// OUTPUT         : TRUE or FALSE
//
// GLOBALS        : gmvb_MainChannel            (R)
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BOOL IsPipSourceValid(BYTE B_DesiredPip)
{
   gmt_ConnTypes ct_main, ct_pip;

   if (GetPortConnName(B_DesiredPip) == NOT_USED)
   {
      return FALSE;
   }

   if (IsPortConflict(B_DesiredPip, UserPrefCurrentInputMain)==TRUE)
      return FALSE;

   return TRUE;
}

//******************************************************************************
//
// FUNCTION       : BOOL IsAdcCalibrationRequired(void)
//
// USAGE          : Call this function to check if Adc calibration for
//                  current input is required(Possible)
//
// INPUT          : None
//
// OUTPUT         : TRUE or FALSE
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
BOOL IsAdcCalibrationRequired(void)
{
   if (INTERNAL == GetChannelProcType(gmvb_MainChannel))
   {
      switch (GetChannelConnType(gmvb_MainChannel))
      {
            // switch without break;
         case COMPONENT:
         case VGA:
         case COMPOSITE:
         case SVIDEO:
            return (TRUE);
      }
   }
   return (FALSE);
}

//******************************************************************************
//
// FUNCTION       : BOOL IsCCValid(void)
//
// USAGE          : Call this function to check if CC should be allowed
// This function returns TRUE under the following condition
// RF,COMPOSITE,SVIDEO,COMPONENT - 480ionly, SideBySide PIP
//
// INPUT          : None
//
// OUTPUT         : TRUE or FALSE
//
// GLOBALS        : gmvb_MainChannel        (R)
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BOOL IsCCValid(void)
{
// CC should be allowed under the following condition
// RF,COMPOSITE,SVIDEO,COMPONENT - 480ionly, SideBySide PIP
   BYTE B_InputConnector;
#ifndef NO_PIP_SUPPORT
   if (B_FocusBorder == gmvb_PipChannel)
   {
      if (UserPrefPipMode == SIDE_BY_SIDE || UserPrefPipMode == SIDE_BY_SIDE_TALL)
      {
         B_InputConnector = UserPrefCurrentInputPip;
      }
      else
      {
         return FALSE;
      }
   }
   else
#endif
      B_InputConnector = UserPrefCurrentInputMain;
   switch (GetPortConnType(B_InputConnector))
   {
      case TUNER:
      case COMPOSITE:
      case SVIDEO:
         return TRUE;
      case COMPONENT:
         //480i (input to Cortez)
         if ( gm_GetCurrentInputModeType(B_FocusBorder) == gmd_NTSC_480I )
         {
#ifdef UserPrefMainRouteModeComponent
            if (UserPrefMainRouteModeComponent == gmd_ComponentThroughDecoder)
            {
               return TRUE;
            }
#if ( defined(COMPONENT_VBI_THROUGH_OAK_SLICER) &&     (FEATURE_OAK_VBI_SLICER == REV1 || FEATURE_OAK_VBI_SLICER == REV2 || FEATURE_OAK_VBI_SLICER == REV3) )
            else if (UserPrefMainRouteModeComponent == gmd_ComponentBypassDecoder)
            {
               return TRUE;
            }
#endif
            else
            {
               return FALSE;
            }
#endif
         }
         else
         {
            return FALSE;
         }
      default:
         return FALSE;
   }
}

//******************************************************************************
//
// FUNCTION       : BOOL IsTtxValid(void)
//
// USAGE          : Call this function to check if Ttx should be allowed
// This function returns TRUE under the following condition
// RF,COMPOSITE,SVIDEO,COMPONENT - 576I Pal only, Scart, SideBySide Main
//
// INPUT          : None
//
// OUTPUT         : TRUE or FALSE
//
// GLOBALS        : gmvb_MainChannel        (R)
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BOOL IsTtxValid(void)
{
// Ttx should be allowed under the following condition
// RF,COMPOSITE,SVIDEO,COMPONENT - 576I Pal, Scart, SideBySide MAIN
   switch (GetPortConnType(UserPrefCurrentInputMain))
   {
      case TUNER:
      case COMPOSITE:
      case SVIDEO:
      case SCART:
         return TRUE;
      case COMPONENT:
         //576Ii (input to Cortez)
         if ( gm_GetCurrentInputModeType(CH_A) == gmd_PAL_576I )
         {
            return TRUE;
         }
         else
         {
            return FALSE;
         }
      default:
         return FALSE;
   }
}

//******************************************************************************
//
// FUNCTION       : void ConfigureExtDevices(gmt_PHY_CH B_Channel,
//                                              gmt_UserDefConnNames B_InputConnector)
//
// USAGE          : This is a wrapper for functions that configure external devices.
//                  This is function will be called  through a pointer in the ports attributes table and
//                  provides flexibiliy with different functions associated with different ports.
//
// INPUT          : B_Channel - Current input channel
//                  B_InputConnector - Input connector to be connected to the channel
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
void ConfigureExtDevices(gmt_PHY_CH B_Channel,
                         gmt_UserDefConnNames B_InputConnector)
{
   B_Channel = B_Channel;
   ConfigureExtDevicesVideo(B_InputConnector);
}

//******************************************************************************
//
// FUNCTION       : void PortChangeCommon(gmt_PHY_CH B_Channel, BYTE B_Port)
//
// USAGE          : Port change common
//
// INPUT          : Main/PIP channel, Input port
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
void PortChangeCommon(gmt_PHY_CH B_Channel, BYTE B_Port)
{
#ifdef INACTIVE_DEVICE_POWER_SAVING 
   BYTE B_PreviousPort = (B_Channel == gmvb_MainChannel) ? gB_CurrentInputConnector[gmvb_MainChannel] : gB_CurrentInputConnector[gmvb_PipChannel];
   BYTE B_OtherPort = (B_Channel == gmvb_MainChannel) ? gB_CurrentInputConnector[gmvb_PipChannel] : gB_CurrentInputConnector[gmvb_MainChannel];
   BYTE maxPortNum = NUM_OF_CONNECTORS;

   //Do nothing if port is out of range
   if(B_Port >= maxPortNum)
      return;

   msg_i("Channel: %d",(WORD)B_Channel);
   msg_i("Prev port: %d",(WORD)B_PreviousPort);
   msg_i("Change to port: %d",(WORD)B_Port);

   if (B_PreviousPort != B_Port)
   {
#ifndef NO_PIP_SUPPORT
      if (UserPrefPipMode != NO_PIP && B_PreviousPort == B_OtherPort)
      {
        // if power down device is same as other port, don't power it down.
      }
      else
#endif   
      {
        PowerDownDevice(B_PreviousPort);
      }
      PowerUpDevice(B_Port);
   }
#endif
}

#ifdef ST_4K2K_93xx_BOARD
void PatchOf_4K2K_AVDDS_Toggle(BYTE B_Channel)
{
   if(GetChipID_4K2K() != CHIPID_4K2K_FE)
   {
      if(B_Channel == CH_A)
      {
          /*ChengLin 20130312: Patch from CK for toggle AVDDS1 when DP source changed, it will solve 4k2k platform PBP mode back to Full Screen mode then miss sync issue*/
          gm_WriteRegWord(AVDDS1_CONTROL0,0x0001); //Force Open Loop & AVDDS1_BYPASS_MN_LOOP
          gm_WriteRegByte(AVDDS1_INIT,0x11);
          //End Of Patch
          gm_WriteRegWord(AVDDS1_CONTROL0,0x3000);      
          gm_WriteRegWord(AVDDS1_CONTROL1,0xC6);
          gm_WriteRegWord(AVDDS1_FREQ_DELTA,0);
          gm_WriteRegByte(AVDDS1_INIT,0x11);
      }
      else
      {
          /*ChengLin 20130312: Patch from CK for toggle AVDDS2 when DP source changed, it will solve 4k2k platform PBP mode back to Full Screen mode then miss sync issue*/
          gm_WriteRegWord(AVDDS2_CONTROL0,0x0001); //Force Open Loop & AVDDS2_BYPASS_MN_LOOP
          gm_WriteRegByte(AVDDS2_INIT,0x11);
          //End Of Patch
          gm_WriteRegWord(AVDDS2_CONTROL0,0x3000);
          gm_WriteRegWord(AVDDS2_CONTROL1,0xC6);
          gm_WriteRegWord(AVDDS2_FREQ_DELTA,0);
          gm_WriteRegByte(AVDDS2_INIT,0x1);
   
      }
   }
}
#endif
/***********************************  END  ************************************/

