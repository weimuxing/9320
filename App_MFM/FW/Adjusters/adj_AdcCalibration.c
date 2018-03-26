/*
	$Workfile:   adj_AdcCalibration.c  $
	$Revision: 1.34 $
	$Date: 2013/02/22 09:45:22 $
*/
#define __ADJ_ADC_CALIBRATION_C__

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
// MODULE:      Adj_AdcCalibration.c
//
// USAGE:       This module contains set of utility functions called from
//				OSD adjusters for ADC calibration control.
//
//******************************************************************************


//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "System\all.h"
#include "StdLibInc\gm_Register.h"

#if FEATURE_AIP == ENABLE

//******************************************************************************
//  D E F I N E S
//******************************************************************************
//#define DEBUG_PRINT 
//#define DEBUG_PRINT_TIME
//#define _DEBUG

#ifdef _DEBUG
#define msg(string)         	gm_Print(string,0)
#define msg_i(string,value) 	gm_Print(string,value)
#else
#define msg(string)
#define msg_i(string,value)
#endif

//******************************************************************************
//  S T A T I C    V A R I A B L E S
//******************************************************************************
static WORD W_ADC_FAS0_Save;
static WORD W_AFE_THA_CTRL_0_Save;
static WORD W_AFE_THA_CTRL_1_Save;
static WORD W_ADC_CONTROL0_Save;
static WORD W_ADC_CONTROL1_Save;
static BYTE B_AFEDIG_CTRL1_Save;

#if (FEATURE_ADC_AOC == ENABLE)
static BYTE B_Aoc_Ctrl0Save;
#endif

static WORD W_IMP_CONTROL_Save;
static WORD W_IPP_CONTROL_Save;

static WORD W_AIP_SPARE_03_SAVE;
static WORD W_AIP_SPARE_04_SAVE;
static WORD W_AIP_SPARE_07_SAVE;
static WORD W_AIP_SPARE_08_SAVE;
static WORD W_AIP_SPARE_10_SAVE;
static WORD W_AIP_SPARE_12_SAVE;

//******************************************************************************
//  S T A T I C    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
static void SaveAfeStatus( void);
static void RestoreAfeStatus( void);
static void SaveDitherDACStatus( void);
static void RestoreDitherDACStatus( void);
static void DisableDitherDAC( void);

//*****************************************************************************
//  E X T R E N
//*****************************************************************************

extern BYTE ROM gmc_NumOfReg;
extern void gm_Delay1us(WORD W_Delay);

//*****************************************************************************
//  C O D E
//*****************************************************************************


//******************************************************************************
//
// FUNCTION       : void PerformAdcCalibration(void)
//
// USAGE          : Perform ADC Calibration and save setting into NVRAM
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : gmvs_InputConfigTbl		(R)
//                  gmvb_InputConnector		(R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
#pragma warn -aus    //to disable warning of non-used variable
void PerformAdcCalibration(void)
{
   gmt_ADC_SETTING AdcSettings;
   gmt_PHY_CH AdcChannel = gmvb_MainChannel;
   WORD W_IntDacRefSelection = 0;
   gmt_ADC_CALIBRATION_INPUT AdcCalInput = ADC_CAL_NONE;
   gmt_RET_STAT AdcCalFailed = FALSE;
	BYTE B_CaliTimes = 0;
   //
   // Side-by-side for PIP calibration
   //
   if (UserPrefPipMode == SIDE_BY_SIDE || UserPrefPipMode == SIDE_BY_SIDE_TALL)
   {
      return;
   }

   if (UserPrefADCCalibrationMethod_InternalDac == RESISTIVE_LADDER)
      W_IntDacRefSelection = ADC_DAC_RESISTIVE_LADDER;

#ifdef DEBUG_PRINT
   gm_Printf(" ConnType  = %d", GetChannelConnType(AdcChannel));
   gm_Printf(" SyncState = %d", gmvb_CurrentSyncState[AdcChannel]);
#endif

   switch (GetChannelConnType(AdcChannel))
   {
      case COMPONENT:
         // Eight Color Bar: (White, Yellow, Cyan, Green, Pink, Red, Blue, Black)
         if (UserPrefADCCalibrationMethod_Comp == EIGHT_COLOR_BAR_100)
            AdcCalInput = ADC_COMPONENT_100_PERCENT;
         else if (UserPrefADCCalibrationMethod_Comp == EIGHT_COLOR_BAR_75)
            AdcCalInput = ADC_COMPONENT_75_PERCENT;
         else if (UserPrefADCCalibrationMethod_Comp == INTERNAL_DAC_COMP)
            AdcCalInput = ADC_COMPONENT_DAC | W_IntDacRefSelection;
         else
            AdcCalInput = ADC_COMPONENT_DAC | W_IntDacRefSelection;
         break;

      case VGA:
#ifdef ALLOW_COMP_CALIB_ON_VGA
         if ((gmvb_CurrentSyncState[AdcChannel] == SOG_STATE) && gm_IsVideo(AdcChannel))
         {
            if (UserPrefADCCalibrationMethod_Comp == EIGHT_COLOR_BAR_100)
               AdcCalInput = ADC_COMPONENT_100_PERCENT;
            else if (UserPrefADCCalibrationMethod_Comp == EIGHT_COLOR_BAR_75)
               AdcCalInput = ADC_COMPONENT_75_PERCENT;
            else if (UserPrefADCCalibrationMethod_Comp == INTERNAL_DAC_COMP)
               AdcCalInput = ADC_COMPONENT_DAC | W_IntDacRefSelection;
            else
               AdcCalInput = ADC_COMPONENT_DAC | W_IntDacRefSelection;
            break;
         }
#endif
         if (UserPrefADCCalibrationMethod_VGA == PIXEL_GRAB)
            AdcCalInput = ADC_GRAPHICS_PIXEL_GRAB;
         else if (UserPrefADCCalibrationMethod_VGA == INTERNAL_DAC_VGA)
            AdcCalInput = ADC_GRAPHICS_DAC | W_IntDacRefSelection;
         else if (UserPrefADCCalibrationMethod_VGA == MIN_MAX)
            AdcCalInput = ADC_GRAPHICS_MIN_MAX;
         else //if (UserPrefADCCalibrationMethod_VGA == INTERNAL_DAC_MIN_MAX)
            AdcCalInput = ADC_GRAPHICS_DAC_MIN_MAX;
         break;

      //case COMPOSITE:
      //case SVIDEO:
      default:
         return;
   }

	
   // backup ADC settings
   gm_GetAdcGainOffset(gmd_ADC_0, &AdcSettings);
   
   // save status of Dither DAC
   SaveDitherDACStatus();

   // Disable Dither DAC
   DisableDitherDAC();

   // Save current AFE status
   SaveAfeStatus();

   // Invoke the calibrated BandGap value
#ifdef ADC_BANDGAP_CALIBRATION
   AdcBandgapCalibrate();
   AdcBandgapInvoke();
#endif

RedoAdcCali: 	   //Hyb130503+ somehow it failed , give it secound chance to do it;

   AdcCalFailed = gm_AdcAutoColorBalance(gmd_ADC_0, AdcCalInput);

   // restore status of Dither DAC
   RestoreDitherDACStatus();
   gm_ClearRegBitsWord(ADC_TESTDAC0, STDAC);

   //restore AFE status
   RestoreAfeStatus();

   if (AdcCalFailed)
   {
      gm_Print("!!! ADC calibration failed !!!",0);
      gm_SetAdcGainOffset(gmd_ADC_0, &AdcSettings);
      return;
   }
   
   //
   // Get values from driver
   // Save values into NVRAM
   //
   gm_GetAdcGainOffset(gmd_ADC_0, &AdcSettings);

	//Hyb130503+ for ADC settings check
	if((CheckADCCaliData(&AdcSettings) == gmd_FALSE)&&B_CaliTimes < 3)	// already did 3 times, bypass ,whatever;
	{
		B_CaliTimes++;
		//gm_Print("!!! ADC calibration Error, Value not Right==> Redo!!!",0);
		goto RedoAdcCali;
	}

	#ifdef DELL_U2410F //KevinL 20081025 for Factory mode
	UserPrefGlobalOffset1_R = AdcSettings.AdcRedOffset1;
	UserPrefGlobalOffset1_G = AdcSettings.AdcGreenOffset1;
	UserPrefGlobalOffset1_B = AdcSettings.AdcBlueOffset1;

	UserPrefGlobalOffset2_R = AdcSettings.AdcRedOffset2 - 5;
	UserPrefGlobalOffset2_G = AdcSettings.AdcGreenOffset2 - 5;
	UserPrefGlobalOffset2_B = AdcSettings.AdcBlueOffset2 - 5 ;

	UserPrefGlobalGain_R = AdcSettings.AdcRedGain + 2;
	UserPrefGlobalGain_G = AdcSettings.AdcGreenGain + 2;
	UserPrefGlobalGain_B = AdcSettings.AdcBlueGain + 2;
	#endif
   switch (GetChannelConnType(AdcChannel))
   {
      case COMPONENT:
         if (gm_IsSDVideo(gmvb_MainChannel))
            AdcSettings.AdcGreenGain += 51;  //SCR3818 refer the attached document.

         SaveAdcSettings(gmd_ADC_0, (BYTE *)&AdcSettings, ADC_SETTING_HD);
         SaveAdcSettings(gmd_ADC_0, (BYTE *)&AdcSettings, ADC_SETTING_HD2);
         break;

      case VGA:
#ifdef ALLOW_COMP_CALIB_ON_VGA
         if ((gmvb_CurrentSyncState[AdcChannel] == SOG_STATE) && gm_IsVideo(AdcChannel))
         {
            SaveAdcSettings(gmd_ADC_0, (BYTE *)&AdcSettings, ADC_SETTING_HD2);
            break;
         }
#endif
       #if 0 // Increase ADC Gain for brighter image. Depends on project situation.
         AdcSettings.AdcRedGain += 5;
         AdcSettings.AdcGreenGain += 5;
         AdcSettings.AdcBlueGain += 5;
         gm_SetAdcGainOffset(gmd_ADC_0, &AdcSettings);
       #endif
         SaveAdcSettings(gmd_ADC_0, (BYTE *)&AdcSettings, ADC_SETTING_GRAPHIC);
         break;

      case COMPOSITE:
      case SVIDEO:
         break;

      default:
         return;
   }   

#ifdef DEBUG_PRINT
   PrintADCRegisterVal();  //print out calibrated register values
#endif
}
#pragma warn +aus

//******************************************************************************
//
// FUNCTION       : BOOL IsPortValidForADCCalibration(BYTE B_ADCCalibrationType)
//
// USAGE          : Check to see if current input port is valid for the type of
//					ADC Calibration chosen
//
// INPUT          : B_ADCCalibrationType - specifies what type of ADC user is
//										   trying to perform
//
// OUTPUT         : TRUE, FALSE
//
// GLOBALS        :
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BOOL IsPortValidForADCCalibration(BYTE B_ADCCalibrationType)
{
   //
   // Side-by-side for PIP calibration
   //
   if (UserPrefPipMode == SIDE_BY_SIDE || UserPrefPipMode == SIDE_BY_SIDE_TALL)
   {
      switch (B_ADCCalibrationType)
      {
         case ADC_GRAPHICS:
#ifdef ALLOW_COMP_CALIB_ON_VGA
            if (gmvb_CurrentSyncState[gmvb_PipChannel]==SOG_STATE)
               return TRUE;
#endif
            if (!gm_IsGraphics(gmvb_PipChannel))
               return FALSE;
            if (UserPrefCurrentInputPip==ALL_BOARDS_VGA1)
               return TRUE;
            else
               return FALSE;

			case ADC_COMPOSITE:
				if (UserPrefCurrentInputPip==ALL_BOARDS_SVIDEO1)
		 			return TRUE;
				#ifdef EXTERNAL_DECODER
				if (UserPrefCurrentInputPip==ALL_BOARDS_CVBS1)
					return FALSE;
				#else
				if (UserPrefCurrentInputPip==ALL_BOARDS_CVBS1)
					return TRUE;
				else
					return FALSE;
				#endif

         case ADC_COMPONENT:
            if (UserPrefCurrentInputPip==ALL_BOARDS_COMP1)
               return TRUE;
            else
               return FALSE;

         case ADC_SVIDEO:
            if (UserPrefCurrentInputPip==ALL_BOARDS_SVIDEO1)
               return TRUE;
            else
               return FALSE;
      }
      return FALSE;
   }

   //
   // Otherwise for MAIN calibration
   //
   switch (B_ADCCalibrationType)
   {
      case ADC_GRAPHICS:
#ifdef ALLOW_COMP_CALIB_ON_VGA
         if (gmvb_CurrentSyncState[gmvb_MainChannel]==SOG_STATE)
            return TRUE;
#endif
         if (!gm_IsGraphics(gmvb_MainChannel))
            return FALSE;
         if (UserPrefCurrentInputMain==ALL_BOARDS_VGA1)
            return TRUE;
         else
            return FALSE;

		case ADC_COMPOSITE:
			if (UserPrefCurrentInputMain==ALL_BOARDS_SVIDEO1)
	 			return TRUE;
			#ifdef EXTERNAL_DECODER
			if (UserPrefCurrentInputMain==ALL_BOARDS_CVBS1)
				return FALSE;
			#else
			if (UserPrefCurrentInputMain==ALL_BOARDS_CVBS1)
				return TRUE;
			else
				return FALSE;
			#endif

      case ADC_COMPONENT:
         if ((UserPrefCurrentInputMain==ALL_BOARDS_COMP1))
            return TRUE;
         else
            return FALSE;

      case ADC_SVIDEO:
         if (UserPrefCurrentInputMain==ALL_BOARDS_SVIDEO1)
            return TRUE;
         else
            return FALSE;

      case ADC_SCART:
         if (UserPrefCurrentInputMain==ALL_BOARDS_VGA1)
            return TRUE;
         else
            return FALSE;
   }
   return FALSE;
}


//******************************************************************************
//
// FUNCTION       : static void SaveAfeStatus(void)
//
// USAGE          : Save the current AFE status
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : 
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
static void SaveAfeStatus( void)
{
   //
   // Save current AFE status
   //
   W_ADC_FAS0_Save = gm_ReadRegWord( ADC_FAS0);
   W_AFE_THA_CTRL_0_Save = gm_ReadRegWord(AFE_THA_CTRL_0);
   W_AFE_THA_CTRL_1_Save = gm_ReadRegWord(AFE_THA_CTRL_1);
   W_ADC_CONTROL0_Save = gm_ReadRegWord(ADC_CONTROL0);
   W_ADC_CONTROL1_Save = gm_ReadRegWord(ADC_CONTROL1);
   B_AFEDIG_CTRL1_Save = gm_ReadRegByte(AFEDIG_CTRL1);

#if (FEATURE_ADC_AOC == ENABLE)
   B_Aoc_Ctrl0Save = gm_ReadRegByte(AOC_CTRL0);
#endif

   W_IMP_CONTROL_Save = gm_ReadRegWord(IMP_CONTROL);
   W_IPP_CONTROL_Save = gm_ReadRegWord(IPP_CONTROL);
}

//******************************************************************************
//
// FUNCTION       : static void RestoreAfeStatus(void)
//
// USAGE          : Restore the AFE status from saved state
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
static void RestoreAfeStatus( void)
{
   //
   // Restore AFE status
   //
   gm_WriteRegWord(ADC_FAS0, W_ADC_FAS0_Save);
   gm_WriteRegWord(AFE_THA_CTRL_0, W_AFE_THA_CTRL_0_Save);
   gm_WriteRegWord(AFE_THA_CTRL_1, W_AFE_THA_CTRL_1_Save);
   gm_WriteRegWord(ADC_CONTROL0, W_ADC_CONTROL0_Save);
   gm_WriteRegWord(ADC_CONTROL1, W_ADC_CONTROL1_Save);
   gm_SetRegBitsByte(AFEDIG_CTRL1, B_AFEDIG_CTRL1_Save);

#if (FEATURE_ADC_AOC == ENABLE)
   gm_WriteRegByte(AOC_CTRL0 , B_Aoc_Ctrl0Save);
#endif

   gm_WriteRegWord(IMP_CONTROL, W_IMP_CONTROL_Save);
   gm_WriteRegWord(IPP_CONTROL, W_IPP_CONTROL_Save);
}


#ifdef ADC_BANDGAP_CALIBRATION
//******************************************************************************
//
// FUNCTION      	: gmt_RET_STAT AdcBandgapCalibrate(void)
//
// USAGE          	: Performs calibration of the Internal DAC bandgap.
//
// INPUT          	: None
//
// OUTPUT         	: OK
//                  	FEATURE_NO_AVAIL
//                  	ERR_PARAM
//
// GLOBALS        : None
//
// USED_REGS      :
//						ADC_TESTDAC0
//						ADC_BG_STATUS
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
gmt_RET_STAT AdcBandgapCalibrate(void)
{
   BYTE B_Bandgap_Settings;
   gmt_RET_STAT W_Status = OK;

   // save status of Dither DAC
   SaveDitherDACStatus();

   // Disable Dither DAC
   DisableDitherDAC();

   // Power down BandGap calibration
   gm_WriteRegWord(AFE_ANALOG_CTRL1, AIP_PD_BGCAL);

   // wait for settling
   gm_Delay1ms(10);

   // Enable the BandGap calibration by setting AIP_PD_BGCAL = 0
   // Disable manual trim of the BandGap by setting AIP_EN_MAN_BGTRM = 0
   gm_ClearRegBitsWord(AFE_ANALOG_CTRL1, AIP_PD_BGCAL);

   // Toggle the BandGap Trim Pulse from 0 - 1 (wait 75 ns)  - 0 by toggling AIP_EN_BG_TRM
   gm_SetRegBitsWord(AFE_ANALOG_CTRL1, AIP_EN_BG_TRM);
   gm_Delay1us(75);
   gm_ClearRegBitsWord(AFE_ANALOG_CTRL1, AIP_EN_BG_TRM);

   // Wait for at least 10000 cycles of TCLK. delay = (1/TCLK_FREQ)*10000 <-time in seconds
   // for TCLK_FREQ = 27000000L, delay (us) = 370
   gm_Delay1us(370);
   gm_Delay1ms(10);

   // Read and store values of the calibrated BandGap in the NVRAM
   B_Bandgap_Settings = gm_ReadRegWord(ADC_BG_STATUS) & ADC_BG_CAL;

#ifdef DEBUG_PRINT
   gm_Printf("ADC_BG_STATUS is 0x%X", B_Bandgap_Settings);
#endif

   SaveBandgapSetting((BYTE *)&B_Bandgap_Settings);

   // restore status of Dither DAC
   RestoreDitherDACStatus();

   return (W_Status);
}

//******************************************************************************
//
// FUNCTION       : gmt_RET_STAT AdcBandgapInvoke(void)
//
// USAGE          : Read the calibrated Internal DAC bandgap value from NVRAM and write this
//						  value to AIP_BG_OS_TRM.
//
// INPUT          : None
//
// OUTPUT         : OK
//                  FEATURE_NO_AVAIL
//                  ERR_PARAM
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
gmt_RET_STAT AdcBandgapInvoke(void)
{
   BYTE B_Bandgap_Settings;
   WORD W_Reg;
   gmt_RET_STAT W_Status = OK;

   // Enable the BandGap calibration by setting AIP_PD_BGCAL = 0
   gm_ClearRegBitsWord(AFE_ANALOG_CTRL1, AIP_PD_BGCAL);

   // Enable manual trim of the BandGap by setting AIP_EN_MAN_BGTRM = 1
   gm_SetRegBitsWord(AFE_ANALOG_CTRL1, AIP_EN_MAN_BGTRM);

   // Read the value of the calibrated Bandgap from NVRAM and
   //		store in AIP_BG_OS_TRM
   ReadBandgapSetting((BYTE *)&B_Bandgap_Settings);
   W_Reg = gm_ReadRegWord(AFE_ANALOG_CTRL1);
   W_Reg = (W_Reg & ~AIP_BG_OS_TRM) + (B_Bandgap_Settings << 5); // shift by 5 bits to match AIP_BG_OS_TRM bit mask
   gm_WriteRegWord(AFE_ANALOG_CTRL1, W_Reg );

   return (W_Status);
}
#endif // ADC_BANDGAP_CALIBRATION

//******************************************************************************
//
// FUNCTION       : static void SaveDitherDACStatus( void)
//
// USAGE          : Save the current Dither DAC status
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
static void SaveDitherDACStatus( void)
{
   //
   // Save current Dither DAC status
   //
   W_AIP_SPARE_03_SAVE = gm_ReadRegWord( AIP_SPARE_03);
   W_AIP_SPARE_04_SAVE = gm_ReadRegWord( AIP_SPARE_04);
   W_AIP_SPARE_07_SAVE = gm_ReadRegWord( AIP_SPARE_07);
   W_AIP_SPARE_08_SAVE = gm_ReadRegWord( AIP_SPARE_08);
   W_AIP_SPARE_10_SAVE = gm_ReadRegWord( AIP_SPARE_10);
   W_AIP_SPARE_12_SAVE = gm_ReadRegWord( AIP_SPARE_12);

}

//******************************************************************************
//
// FUNCTION       : static void RestoreDitherDACStatus( void)
//
// USAGE          : Restore the Dither DAC status from saved state
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
static void RestoreDitherDACStatus( void)
{
   //
   // Restore Dither DAC status
   //
   gm_WriteRegWord( AIP_SPARE_03, W_AIP_SPARE_03_SAVE);
   gm_WriteRegWord( AIP_SPARE_04, W_AIP_SPARE_04_SAVE);
   gm_WriteRegWord( AIP_SPARE_07, W_AIP_SPARE_07_SAVE);
   gm_WriteRegWord( AIP_SPARE_08, W_AIP_SPARE_08_SAVE);
   gm_WriteRegWord( AIP_SPARE_10, W_AIP_SPARE_10_SAVE);
   gm_WriteRegWord( AIP_SPARE_12, W_AIP_SPARE_12_SAVE);

}

//******************************************************************************
//
// FUNCTION       : static void DisableDitherDAC( void)
//
// USAGE          : Disable the Dither DAC status from saved state
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
static void DisableDitherDAC( void)
{
   //
   // Disable Dither DAC
   //
   gm_WriteRegWord( AIP_SPARE_03, 0x0000);
   gm_WriteRegWord( AIP_SPARE_04, 0x0000);
   gm_WriteRegWord( AIP_SPARE_07, 0x0000);
   gm_WriteRegWord( AIP_SPARE_08, 0x0000);
   gm_WriteRegWord( AIP_SPARE_10, 0x0000);
   gm_WriteRegWord( AIP_SPARE_12, 0x0000);

}

//******************************************************************************
//
// FUNCTION       : static void PrintADCRegisterVal( void)
//
// USAGE          : Prints the ADC Register Values (called after calibration)
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
#ifdef DEBUG_PRINT
void PrintADCRegisterVal(void)
{
   //Offset 1 Values
   gm_Printf("ADC A Offset1: 0x%X", gm_ReadRegWord(ADC_A_OFFSET1) & A_OFFSET_1);
   gm_Printf("ADC B Offset1: 0x%X", gm_ReadRegWord(ADC_B_OFFSET1) & B_OFFSET_1);
   gm_Printf("ADC C Offset1: 0x%X", gm_ReadRegWord(ADC_C_OFFSET1) & C_OFFSET_1);

   //Offset 2 Values
   gm_Printf("ADC A Offset2: 0x%X", gm_ReadRegWord(ADC_A_OFFSET2) & A_OFFSET_2);
   gm_Printf("ADC B Offset2: 0x%X", gm_ReadRegWord(ADC_B_OFFSET2) & B_OFFSET_2);
   gm_Printf("ADC C Offset2: 0x%X", gm_ReadRegWord(ADC_C_OFFSET2) & C_OFFSET_2);

   //Gain Values
   gm_Printf("ADC A Gain0: 0x%X", gm_ReadRegWord(ADC_A_GAIN_0));
   gm_Printf("ADC B Gain0: 0x%X", gm_ReadRegWord(ADC_B_GAIN_0));
   gm_Printf("ADC C Gain0: 0x%X", gm_ReadRegWord(ADC_C_GAIN_0));
}
#endif

#else
BOOL IsPortValidForADCCalibration(BYTE B_ADCCalibrationType)
{
	UNUSED_VAR(B_ADCCalibrationType);
	return FALSE;
}


#endif // FEATURE_AIP == ENABLE
/***********************************  END  ************************************/
