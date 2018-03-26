/*
	$Workfile:   UserPref.c  $
	$Revision: 1.23 $
	$Date: 2013/01/24 09:15:26 $
*/
#define __USERPREF_C__
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
// MODULE:      UserPref.c
//
// USAGE:       This module contains functions for user preference store and
//				restore control to NVRAM.
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include <string.h>
#include <math.h>

#include "System\All.h"

//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************

//#define _DEBUG
#ifdef _DEBUG
#define msg(a)		gm_Print(a,0);
#define msg_i(a,b)	gm_Print(a,b);
#else
#define msg(a)
#define msg_i(a,b)
#endif
extern BOOL IsPortConflict(gmt_UserDefConnNames Port1, gmt_UserDefConnNames Port2);//20120926 Add by KevinChen for supporting port check in AD/DC on.

//*****************************************************************************
//  G L O B A L    V A R I A B L E S
//*****************************************************************************

DWORD DW_MonitorPowerOnTime;
extern PortSchemeDependentUserPrefType ROM SchemeFactoryDefaultInROM[NUM_IMAGE_SCHEMES]; // PSD
#ifdef DDCCI_VALUE_CHANGE
extern BYTE B_DDCCINewControlVal;
#endif

//******************************************************************************
//  S T A T I C    V A R I A B L E S
//******************************************************************************

//******************************************************************************
//  S T A T I C    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

static void SaveModeIndependentChangedSettings	(void);

void SaveModeIndependentUserPrefValue (gmt_OSD_Adjuster_Info * AdjusterInfo);
void SavePortSchemeDependentUserPrefValue (gmt_OSD_Adjuster_Info * AdjusterInfo);
void ReadModeIndependentUserPrefValue (gmt_OSD_Adjuster_Info * AdjusterInfo);
void ReadPortSchemeDependentUserPrefValue (gmt_OSD_Adjuster_Info * AdjusterInfo);

// Customize
void CustomizeForAuto(void);
void CustomizeAfterAuto(void);

//******************************************************************************
//  C O D E
//******************************************************************************

//******************************************************************************
//
// FUNCTION       : void NvramCheck(void)
//
// USAGE          : Check current NVRAM checksum and load current values into RAM or
//                  reload NVRAM default value if checksum not match.
//
// INPUT          :
//
// OUTPUT         : None
//
// GLOBALS        : AdcDefaultSetting				(RO)
//                  ModeIndependentUserPreferences	(R)
//                  gmvs_InputConfigTbl				(R)
//                  gmvb_InputConnector				(W)
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void NvramCheck(void)
{
   BYTE B_Checksum1, B_Checksum2;
   BYTE i;

#if FEATURE_AIP == ENABLE
   gmt_ADC_SETTING AdcSettings;

   //
   // check integrity of ADC calibration settings for each port
   //
   for (i=0; i<NumberOfAdcSetting; i++)
   {
      if (ReadAdcSettings(gmd_ADC_0, (BYTE *)&AdcSettings, i) == FALSE)
      {
         AdcSettings = AdcDefaultSetting[i];
         SaveAdcSettings(gmd_ADC_0, (BYTE *)&AdcSettings, i);
      }
   }
#endif

   //
   // 1) Read the mode independent struct first.
   // 2) Then read saved mode independent checksum.
   // 3) Calculate mode independent checksum.
   //
   ReadModeIndependentSettings();

#ifndef DELL_U2410F	//120718 Edward copy from 2410  Removed by ST_hansPENG 090319
   B_Checksum1 = NVRam_ReadByte (ModeIndependentChecksumStart);
   B_Checksum2 = CalculateChecksum (&ModeIndependentUserPreferences,
									  ModeIndependentUserPrefSize);
#endif	

   //
   // Verify checksum and version number
   // if not equal, initialize NVRAM
   //
#if 1
	if (UserPrefVersion != NVRAM_CHECKSUM)
#else
	if ((B_Checksum1 != B_Checksum2) || (UserPrefVersion != WB_NVRAM_STRUCTURE_VERSION))
#endif		
	{
		InitNvram ();
		UserPrefRunBurnInModeFlag = TRUE; //calo add for nvram empty enter into factory mode 090305
		SaveModeIndependentSettings();
		gm_Print ("...DONE");
	}
	//
	// Check if ModeDependentDesciptors are valid
	// If not, reinitialize Non-Preset Mode Dependent Tables
	//
#ifdef DELL_U2410F // Increased by ST_hansPENG 090402
	ReadPortSchemeDependentEntry(UserPrefImageScheme);
	ReadPipPortSchemeDependent();
	ReadPresetModesSettings();
	ReadFactorySettings();
	
	//Hyb140327+ disable BURN in for AC on;
	UserPrefRunBurnInModeFlag = gmd_FALSE;
	//120803 Edward add factory block check
	if(UserPrefFactoryVersion != FACTORY_NVRAM_VERSION)
	{
		InitNvram_FactoryBlock();		
		gm_Print ("!!!NVRAM Factory block reset...DONE");
	}	

	if(CheckModeDependentDescriptor()== FALSE)
		gm_Print("Check CheckModeDependentDescriptor Setting in NVRam error!", 0);
	if(CheckPortSchemeDependentDescriptor() == FALSE)
		gm_Print("Check CheckPortSchemeDependentDescriptor Setting in NVRam error!", 0);
	if(CheckPresetModesDependentDescriptor() == FALSE)
        gm_Print("Check CheckPresetModesDependentDescriptor Setting in NVRam error!", 0);
	// Read the Monitor Power On Time from NVRAM.
	ReadMonitorPowerOnTimefromNVRAM(&DW_MonitorPowerOnTime);

	if (CheckUserPrefSettingRange() == FALSE)
		gm_Print("Check UserPref Setting in NVRam error!", 0);
#else
   ReadModeIndependentSettings();
   B_Checksum1 = NVRam_ReadByte (ModeIndependentChecksumStart);
   B_Checksum2 = CalculateChecksum (&ModeIndependentUserPreferences,
                                    ModeIndependentUserPrefSize);

   //
   // Verify checksum and version number
   // if not equal, initialize NVRAM
   //
   if ((B_Checksum1 != B_Checksum2) || (UserPrefVersion != WB_NVRAM_STRUCTURE_VERSION))
   {
      gm_Print ("ModeIndependent still error after NVRAM init, use ROM setting");
      ModeIndependentUserPreferences = FactoryDefaultInROM;
      //PortSchemeDependentUserPreferences = SchemeFactoryDefaultInROM[0];
#if FEATURE_AIP == ENABLE
      for (i=0; i<NumberOfAdcSetting; i++)
      {
         AdcSettings = AdcDefaultSetting[i];
         gm_SetAdcGainOffset(gmd_ADC_0, &AdcSettings);
         //SaveAdcSettings(gmd_ADC_0, (BYTE *)&AdcSettings, i);
      }
#endif
   }

   CheckPortSchemeDependentEntry(UserPrefImageScheme);

   B_Checksum1 = NVRam_ReadByte (PortSchemeDependentEntryStart + ChecksumSize);
   B_Checksum2 = CalculateChecksum (&PortSchemeDependentUserPreferences,
                                    PortSchemeDependentUserPrefSize);
   if ((B_Checksum1 != B_Checksum2) || (UserPrefVersion != WB_NVRAM_STRUCTURE_VERSION))
   {
      gm_Print ("PortScheme still error after NVRAM init, use ROM setting");
      //ModeIndependentUserPreferences = FactoryDefaultInROM;
      PortSchemeDependentUserPreferences = SchemeFactoryDefaultInROM[0];
      
#if FEATURE_AIP == ENABLE
      for (i=0; i<NumberOfAdcSetting; i++)
      {
         AdcSettings = AdcDefaultSetting[i];
         gm_SetAdcGainOffset(gmd_ADC_0, &AdcSettings);
         //SaveAdcSettings(gmd_ADC_0, (BYTE *)&AdcSettings, i);
      }
#endif
      gm_Print ("PortScheme error = ....................");
      //gm_Print ("UserPrefColorTemp = ....... %d", UserPrefColorTemp);
   }

#if FEATURE_AIP == ENABLE
   //
   // Check if ModeDependentDesciptors are valid
   // If not, reinitialize Non-Preset Mode Dependent Tables
   //
   if (!CheckModeDependentDescriptor())
   {
      // Re-initialize mode dependent descriptor and lookup table.
      InitModeDependentDescr();
      InitModeDependentLookUp();
   }
#endif

   //
   // Check if PortDependentDesciptor is valid (if number of connectors are the same)
   // If not, reinitialize Port Dependent entries
   //
   if (!CheckPortSchemeDependentDescriptor())
   {
      // Re-initialize port dependent descriptor and entries.
      InitPortSchemeDependentDescr();
      InitPortSchemeDependent();
   }
   // Read the Monitor Power On Time from NVRAM.
	if(!CheckPresetModesDependentDescriptor())
		{
		InitPresetModesDescr();
		InitPresetModesDependent();
		}
	ReadMonitorPowerOnTimefromNVRAM(&DW_MonitorPowerOnTime);
#endif
}

//******************************************************************************
//
// FUNCTION       : void InitNvram (void)
//
// USAGE          : Initialize the NVRAM with default values
//
// INPUT          :
//
// OUTPUT         : None
//
// GLOBALS        : ModeIndependentUserPreferences		(R/W)
//                  FactoryDefaultInROM					(RO)
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void InitNvram (void)
{
   BYTE B_Checksum1;
   BYTE i;
   gm_Print ("Re-initialize NVRAM...");

   //
   // 1) Get initialize mode independent settings from ROM
   // 2) Save mode independent settings in NVRAM.
   // 3) Calculate mode independent checksum.
   // 4) Update mode independent checksum into NVRAM.
   //
   ModeIndependentUserPreferences = FactoryDefaultInROM;

	#ifdef DELL_U2410F
	UserPrefPersonalKey2  = ShortCut_PresetMode_All;
	UserPrefPersonalKey3  = ShortCut_BrightnessContrast;
	UserPrefButtonSoundOff = OFF;	//U2913 default on
	#endif

   NVRam_WriteBuffer (ModeIndependentStart,
                      (BYTE *)&ModeIndependentUserPreferences,
                      ModeIndependentUserPrefSize);

   B_Checksum1 = CalculateChecksum( &ModeIndependentUserPreferences,
                                    ModeIndependentUserPrefSize);

   NVRam_WriteByte (ModeIndependentChecksumStart, B_Checksum1);

   AutoScanInput_InitDefault();

#if FEATURE_AIP == ENABLE
   //
   // Re-initialize mode dependent descriptor and lookup table.
   //
   InitModeDependentDescr();
   InitModeDependentLookUp();
#endif

   //
   // Re-initialize port dependent settings using Factory defaults in ROM
   //
   InitPortSchemeDependentDescr();
   InitPortSchemeDependent();

#ifdef DELL_U2410F	// Increased by ST_hansPENG 090206
	//Neil 20120528+
	UserPrefImageScheme = InputConvertScheme(UserPrefCurrentInputMain);
	UserPrefPipImageScheme = InputConvertScheme(UserPrefCurrentInputPip);

	InitPresetModesDescr();
	InitPresetModesDependent();
	#if 0//120803 Edward move the factory setting reset by other checking function 
	FactoryUserPreferences = FactoryCalibDefaultsInROM;
	NVRam_WriteBuffer (FactorySettingStart, (BYTE *)&FactoryUserPreferences, FactoryUserPrefSize);
	B_Checksum1 = CalculateChecksum(&FactoryUserPreferences, FactoryUserPrefSize);
	NVRam_WriteByte(FactorySettingChecksumStart, B_Checksum1);
	#endif
#endif	
}


//******************************************************************************
//
// FUNCTION       : void InitNvram_FactoryBlock (void)
//
// USAGE          : Initialize the Facotory NVRAM with default values
//
// INPUT          : 
//
// OUTPUT         : None
//
// GLOBALS        :  FactoryDefaultInROM	(RO)
//
// USED_REGS      : 
//
// PRE_CONDITION  : 
//
// POST_CONDITION : 
//
//******************************************************************************

void InitNvram_FactoryBlock()//120803 Edward add factory block check
{
	BYTE B_Checksum;
	BYTE i;
	gm_Print ("Re-initialize Factory block NVRAM...");

	FactoryUserPreferences = FactoryCalibDefaultsInROM;
	NVRam_WriteBuffer (FactorySettingStart, (BYTE *)&FactoryUserPreferences, FactoryUserPrefSize);
	B_Checksum = CalculateChecksum(&FactoryUserPreferences, FactoryUserPrefSize);
	NVRam_WriteByte(FactorySettingChecksumStart, B_Checksum);

}

//******************************************************************************
//
// FUNCTION       : BYTE CalculateChecksum(BYTE * Bp_Buffer, BYTE B_Size)
//
// USAGE          : Common function to calculate checksum
//                  Returns a BYTE representing the summ of the size bytes in
//                  the pointed buffer.
//
// INPUT          : Bp_Buffer
//                  B_Size
//
// OUTPUT         : BYTE - B_Checksum
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
BYTE CalculateChecksum(void const* Bp_Buffer, WORD B_Size)
{
	BYTE B_Checksum = 0;
	while (B_Size--)
		B_Checksum += *(((BYTE*)Bp_Buffer)++);
	return B_Checksum;
}

#ifdef Gamma_DUM_Initial_Check
BYTE CalculateChecksum_far(BYTE far* Bp_Buffer, WORD B_Size)
{
	BYTE B_Checksum = 0;

	while (B_Size--)
		{
		B_Checksum += *(((BYTE far*)Bp_Buffer)++);
		}
	return B_Checksum;
}

BYTE CalculateChecksum_Word(WORD const far* Bp_Buffer, WORD B_Size)
{
	BYTE B_Checksum = 0,i=0;
	BYTE B_Buf[256];
   BYTE *B_Ptr=B_Buf;

	for(i=0; i< B_Size; i++)
	{
		B_Buf[i*2]= Bp_Buffer[i]&0xFF;
		B_Buf[i*2+1]= Bp_Buffer[i]>>8;
	}
	B_Size *= 2;
	
	while (B_Size--)
	{
		B_Checksum += *(((BYTE*)B_Ptr)++);
	}
	return B_Checksum;
}
#endif
//*****************************************************************************
//
// MODE INDEPENDENT SETTINGS
//
//*****************************************************************************

//******************************************************************************
//
// FUNCTION       : void ReadModeIndependentSettings(void)
//
// USAGE          : Read mode independent settings from NVRAM into RAM
//
// INPUT          :
//
// OUTPUT         : None
//
// GLOBALS        : ModeIndependentUserPreferences	(R)
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void ReadModeIndependentSettings(void)
{
   NVRam_ReadBuffer(ModeIndependentStart,
                    (BYTE *)&ModeIndependentUserPreferences,
                    ModeIndependentUserPrefSize);
}

//******************************************************************************
//
// FUNCTION       : void SaveModeIndependentSettings(void)
//
// USAGE          : Save mode independent settings in NVRAM
//
// INPUT          :
//
// OUTPUT         : None
//
// GLOBALS        : ModeIndependentUserPreferences	(R)
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void SaveModeIndependentSettings(void)
{
#ifdef USERPREF_SAVE_ONLY_CHANGED_SETTINGS
   SaveModeIndependentChangedSettings();
#else
   BYTE B_Checksum;

   #ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()==CHIPID_4K2K_R)
   	{
	#if ENABLE_SOC_IPC_BUSY_CHECK
	/*ChengLin 20130124: New 4k2k board BUSY status check*/
	if(!SocIPCIsSlaveLeftReady())
			return;
	#endif                                                                
	   SOCIPC_SyncOSDAdjustHandler( SI_OSD_SYNC_SAVE_MODE_INDEPEDENT );
   	}
	#endif
#ifdef DDCCI_VALUE_CHANGE 
	BYTE B_CheckSumTemp;
	WORD W_CheckSumAdd = ModeIndependentStart + ModeIndependentUserPrefSize;
	NVRam_ReadBuffer(W_CheckSumAdd, (BYTE *)(&B_CheckSumTemp), 1);
#endif


   NVRam_WriteBuffer(ModeIndependentStart,
                     (BYTE *)&ModeIndependentUserPreferences,
                     ModeIndependentUserPrefSize);
   //
   // Calculate mode independent checksum.
   //
   B_Checksum = CalculateChecksum( &ModeIndependentUserPreferences,
                                   ModeIndependentUserPrefSize);
   //
   // Update mode independent checksum.
   //
   NVRam_WriteByte(ModeIndependentChecksumStart, B_Checksum);
	
#ifdef DDCCI_VALUE_CHANGE
	if(B_CheckSumTemp != B_Checksum)
	{
		B_DDCCINewControlVal = 1;
	}
#endif

#endif
}

//******************************************************************************
//
// FUNCTION       : void SaveUserPref(gmt_OSD_Adjuster_Info * AdjusterInfo)
//
// USAGE          : Save single UserPref in NVRAM.  This function is called by the osd library by
//                       setting up function pointer during init.
//
// INPUT          :  AdjusterInfo holds the structure id, offset, value, and size of the UserPref.
//                       The values are populated in the osd library before calling this function.
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  : None
//
// POST_CONDITION : None
//
//******************************************************************************
void SaveUserPref(gmt_OSD_Adjuster_Info * AdjusterInfo)
{
   //
   // Call appropriate Save function according to structure (ie. ModeIndependent, PortDependent, ModeDependent)
   //
   if (AdjusterInfo->AdjStructId == AGID_ModeIndependent)
   {
      SaveModeIndependentUserPrefValue(AdjusterInfo);
   }
   else if (AdjusterInfo->AdjStructId == AGID_PortSchemeDependent)  // PSD
   {
      SavePortSchemeDependentUserPrefValue(AdjusterInfo);
   }
#if FEATURE_AIP == ENABLE
   else if (AdjusterInfo->AdjStructId == AGID_ModeDependent)
   {
      SaveModeDependentUserPrefValue(gmvb_MainChannel, AdjusterInfo);
   }
#endif
}

//******************************************************************************
//
// FUNCTION       : void RetrieveUserPref(gmt_OSD_Adjuster_Info * AdjusterInfo)
//
// USAGE          : Retrieve single UserPref from NVRAM in order to set the UserPref back to this value.
//                       This function is called by the osd library by setting up function pointer during init.
//
// INPUT          :  AdjusterInfo holds the structure id, offset, value, and size of the UserPref.
//                       The values are populated in the osd library before calling this function.
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  : None
//
// POST_CONDITION : None
//
//******************************************************************************
void RetrieveUserPref(gmt_OSD_Adjuster_Info * AdjusterInfo)
{
   if (AdjusterInfo->AdjStructId == AGID_ModeIndependent)
   {
      ReadModeIndependentUserPrefValue(AdjusterInfo);
   }
   else if (AdjusterInfo->AdjStructId == AGID_PortSchemeDependent)  // PSD
   {
      ReadPortSchemeDependentUserPrefValue(AdjusterInfo);
   }
#if FEATURE_AIP == ENABLE
   else if (AdjusterInfo->AdjStructId == AGID_ModeDependent)
   {
      RetrieveModeDependentUserPrefValue(gmvb_MainChannel, AdjusterInfo);
   }
#endif
}

//******************************************************************************
//
// FUNCTION       : void SaveModeIndependentUserPrefValue(gmt_OSD_Adjuster_Info * AdjusterInfo)
//
// USAGE          : Save single UserPref (ModeIndependent) into NVRAM.
//                       This function does the actual write into NVRAM.
//
// INPUT          :  AdjusterInfo holds the structure id, offset, value, and size of the UserPref.
//                       The values are populated in the osd library before calling this function.
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  : None
//
// POST_CONDITION : None
//
//******************************************************************************
void SaveModeIndependentUserPrefValue (gmt_OSD_Adjuster_Info * AdjusterInfo)
{
   BYTE B_Checksum;
   WORD W_UserPrefStartAddr;
   WORD W_UserPrefGroupSize;
   ModeIndependentUserPrefType LocalCopy;
#ifdef DDCCI_VALUE_CHANGE
	BYTE B_CheckSumTemp;
	WORD W_CheckSumAdd = ModeIndependentStart + ModeIndependentUserPrefSize;
	NVRam_ReadBuffer(W_CheckSumAdd, (BYTE *)&B_CheckSumTemp, 1);
#endif

   W_UserPrefStartAddr = ModeIndependentStart;
   W_UserPrefGroupSize = ModeIndependentUserPrefSize;

   //
   // Write UserPref value into NVRAM
   //
   NVRam_WriteBuffer(W_UserPrefStartAddr + AdjusterInfo->AdjOffset,
                     AdjusterInfo->CurrentAdjValue,
                     AdjusterInfo->CurrentAdjSize);

   //
   // Read UserPref structure to compute updated checksum
   //
   NVRam_ReadBuffer(W_UserPrefStartAddr,
                    (BYTE *)&LocalCopy,
                    W_UserPrefGroupSize);

   //
   // Calculate mode independent checksum.
   //
   B_Checksum = CalculateChecksum( &LocalCopy,
                                   W_UserPrefGroupSize);

   //
   // Write updated checksum into NVRAM
   //
   NVRam_WriteByte(W_UserPrefStartAddr + W_UserPrefGroupSize, B_Checksum);

#ifdef DDCCI_VALUE_CHANGE
	if(B_CheckSumTemp != B_Checksum)
	{
		B_DDCCINewControlVal = 1;
	}
#endif
}

//******************************************************************************
//
// FUNCTION       : void SavePortDependentUserPrefValue(gmt_OSD_Adjuster_Info * AdjusterInfo)
//
// USAGE          : Save single UserPref (PortDependent) into NVRAM.
//                       This function does the actual write into NVRAM.
//
// INPUT          :  AdjusterInfo holds the structure id, offset, value, and size of the UserPref.
//                       The values are populated in the osd library before calling this function.
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  : None
//
// POST_CONDITION : None
//
//******************************************************************************
void SavePortSchemeDependentUserPrefValue (gmt_OSD_Adjuster_Info * AdjusterInfo)
{
   BYTE B_Checksum;
   WORD W_UserPrefStartAddr;
   WORD W_UserPrefGroupSize;
   PortSchemeDependentUserPrefType LocalCopy;
   BYTE B_Index   = gmvb_InputConnector[gmvb_MainChannel];

   W_UserPrefStartAddr = PortSchemeDependentEntryStart + ((WORD)B_Index * (PortSchemeDependentUserPrefSize + ChecksumSize));
   W_UserPrefGroupSize = PortSchemeDependentUserPrefSize;

   //
   // Write UserPref value into NVRAM
   //
   NVRam_WriteBuffer(W_UserPrefStartAddr + AdjusterInfo->AdjOffset,
                     AdjusterInfo->CurrentAdjValue,
                     AdjusterInfo->CurrentAdjSize);

   //
   // Read UserPref structure to compute updated checksum
   //
   NVRam_ReadBuffer(W_UserPrefStartAddr,
                    (BYTE *)&LocalCopy,
                    W_UserPrefGroupSize);

   //
   // Calculate Port dependent checksum.
   //
   B_Checksum = CalculateChecksum(&LocalCopy,
                                  W_UserPrefGroupSize);

   //
   // Write updated checksum into NVRAM
   //
   NVRam_WriteByte(W_UserPrefStartAddr + W_UserPrefGroupSize, B_Checksum);

}

//******************************************************************************
//
// FUNCTION       : void ReadModeIndependentUserPrefValue(gmt_OSD_Adjuster_Info * AdjusterInfo)
//
// USAGE          : Read single UserPref (ModeIndependent) from NVRAM.
//                       This function does the actual read from NVRAM.
//
// INPUT          :  AdjusterInfo holds the structure id, offset, value, and size of the UserPref.
//                       The values are populated in the osd library before calling this function.
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  : None
//
// POST_CONDITION : None
//
//******************************************************************************
void ReadModeIndependentUserPrefValue(gmt_OSD_Adjuster_Info * AdjusterInfo)
{
   WORD W_Address;					// holds the computed NVRAM address

   W_Address = ModeIndependentStart;

   NVRam_ReadBuffer(W_Address + AdjusterInfo->AdjOffset, AdjusterInfo->CurrentAdjValue, AdjusterInfo->CurrentAdjSize);
}

//******************************************************************************
//
// FUNCTION       : void ReadPortDependentUserPrefValue(gmt_OSD_Adjuster_Info * AdjusterInfo)
//
// USAGE          : Read single UserPref (PortDependent) from NVRAM.
//                       This function does the actual read from NVRAM.
//
// INPUT          :  AdjusterInfo holds the structure id, offset, value, and size of the UserPref.
//                       The values are populated in the osd library before calling this function.
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  : None
//
// POST_CONDITION : None
//
//******************************************************************************
void ReadPortSchemeDependentUserPrefValue(gmt_OSD_Adjuster_Info * AdjusterInfo)
{
   WORD W_Address;					// holds the computed NVRAM address
   BYTE B_Index   = gmvb_InputConnector[gmvb_MainChannel];

   W_Address = PortSchemeDependentEntryStart;

   W_Address += (WORD)B_Index * (PortSchemeDependentUserPrefSize + ChecksumSize);

   NVRam_ReadBuffer(W_Address + AdjusterInfo->AdjOffset, AdjusterInfo->CurrentAdjValue, AdjusterInfo->CurrentAdjSize);
}

#ifdef USERPREF_SAVE_ONLY_CHANGED_SETTINGS
//******************************************************************************
//
// FUNCTION       : static void SaveModeIndependentChangedSettings(void)
//
// USAGE          : Save only the changed mode independent settings in NVRAM
//
// INPUT          :
//
// OUTPUT         : None
//
// GLOBALS        : ModeIndependentUserPreferences		(R)
//                  ModeIndependentBackup				(R/W)
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
static void SaveModeIndependentChangedSettings(void)
{
   BYTE B_Temp, nvramData;
   BYTE *Bp_Buf1,*Bp_Buf2;
   WORD i;

   //
   // Check each byte and updated only the changed data into NVRAM
   //
   Bp_Buf1 = (BYTE *) &ModeIndependentUserPreferences;
   Bp_Buf2 = (BYTE *) &ModeIndependentBackup;

   for (i = 0; i < ModeIndependentUserPrefSize; i++)
   {
      if ((*(Bp_Buf1+i)) != (*(Bp_Buf2+i)))
      {
         *(Bp_Buf2+i) = *(Bp_Buf1+i);
         nvramData = *(Bp_Buf1+i);
         NVRam_WriteByte(ModeIndependentStart+i, nvramData);
      }
   }

   //
   // Update checksum into NVRAM
   //
   B_Temp = CalculateChecksum( &ModeIndependentUserPreferences,
                               ModeIndependentUserPrefSize);
   NVRam_WriteByte(ModeIndependentChecksumStart, B_Temp);

   //
   // Update backup Preferences
   //
   ModeIndependentBackup = ModeIndependentUserPreferences;
}
#endif

#if FEATURE_AIP == ENABLE

//*****************************************************************************
//
// ADC SETTINGS
//
//*****************************************************************************

#ifdef ADC_BANDGAP_CALIBRATION
//******************************************************************************
//
// FUNCTION      	: BOOL ReadBandgapSetting(BYTE *BandgapSetting)
//
// USAGE          	: Read the Bandgap setting from NVRAM
//
// INPUT          	: BandgapSetting - Bandgap setting
//
// OUTPUT         	: TRUE  - data read back ok
//                  		FALSE - get data failed
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
BOOL ReadBandgapSetting(BYTE *BandgapSetting)
{
   BYTE B_Checksum;
   WORD W_Addr;

   //
   // 1) Get Bandgap data from NVRAM
   // 2) Calculate Bandgap setting checksum.
   //
   W_Addr = AdcSettingsStart;

   NVRam_ReadBuffer(W_Addr, BandgapSetting, BandgapSettingSize);
   B_Checksum = CalculateChecksum(BandgapSetting, BandgapSettingSize);
   W_Addr += BandgapSettingSize;

   //
   // check bandgap settings checksum.
   //
   if (NVRam_ReadByte(W_Addr) == B_Checksum)		// Checksum OK
      return TRUE;							// yes
   else
      return FALSE;                               // no
}

//******************************************************************************
//
// FUNCTION       : BOOL SaveBandgapSettings(BYTE *BandgapSetting)
//
// USAGE          	: Save the Bandgap setting to NVRAM
//
// INPUT          	: BandgapSetting - Bandgap setting
//
// OUTPUT         	: TRUE  - data read back ok
//                  	  FALSE - get data failed
//
// GLOBALS        	: None
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BOOL SaveBandgapSetting(BYTE *BandgapSetting)
{
   BYTE B_Checksum;
   WORD W_Addr;

   //
   // Save Bandgap setting.
   //
   W_Addr = AdcSettingsStart;

   NVRam_WriteBuffer(W_Addr, BandgapSetting, BandgapSettingSize);

   //
   // Calculate Bandgap setting checksum.
   // Update Bandgap setting checksum.
   //
   B_Checksum = CalculateChecksum(BandgapSetting, BandgapSettingSize);
   W_Addr += BandgapSettingSize;

   NVRam_WriteByte(W_Addr, B_Checksum);

   return (TRUE);
}
#endif // ADC_BANDGAP_CALIBRATION

//******************************************************************************
//
// FUNCTION       : BOOL ReadAdcSettings(gmt_ADC_ID B_AdcId, BYTE *AdcSettings, BYTE B_Port)
//
// USAGE          : Read ADC settings from NVRAM
//
// INPUT          : B_AdcId - decoder for main (0) or pip (1) channel
//                  AdcSettings - internal ADC settings
//                  B_Port - port number to store ADC settings
//
// OUTPUT         : TRUE  - data read back ok
//                  FALSE - get data failed
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
BOOL ReadAdcSettings(gmt_ADC_ID B_AdcId, BYTE *AdcSettings, BYTE B_Port)
{
   BYTE B_Checksum;
   WORD W_Addr;

   UNUSED_VAR(B_AdcId);

   //
   // Ensure valid port number
   //

   if (B_Port >= NumberOfAdcSetting)
      return FALSE;

   //
   // 1) Get ADC data from NVRAM
   // 2) Calculate ADC settings checksum.
   //
   if (B_Port < NumberOfAdcSetting)
   {
      W_Addr = AdcSettingsStart + ( BandgapSettingSize + ChecksumSize) +
               ((AdcSettingsSize+ChecksumSize) * B_Port);

      NVRam_ReadBuffer(W_Addr, AdcSettings, AdcSettingsSize);
      B_Checksum = CalculateChecksum(AdcSettings, AdcSettingsSize);
      W_Addr += AdcSettingsSize;
   }

   //
   // check ADC settings checksum.
   //
   if (NVRam_ReadByte(W_Addr) == B_Checksum)		// Checksum OK
      return TRUE;								// yes
   else
      return FALSE;                               // no
}

//******************************************************************************
//
// FUNCTION       : BOOL SaveAdcSettings(gmt_ADC_ID B_AdcId, BYTE *AdcSettings, BYTE B_Port)
//
// USAGE          : Save ADC settings from NVRAM
//
// INPUT          : B_AdcId - decoder for main (0) or pip (1) channel
//                  AdcSettings - internal ADC settings
//                  B_Port - port number to store ADC settings
//
// OUTPUT         : TRUE  - data read back ok
//                  FALSE - get data failed
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
BOOL SaveAdcSettings(gmt_ADC_ID B_AdcId, BYTE *AdcSettings, BYTE B_Port)
{
   BYTE B_Checksum;
   WORD W_Addr;

   UNUSED_VAR(B_AdcId);
   //
   // Ensure valid port number
   //

   if (B_Port >= NumberOfAdcSetting)
      return FALSE;

   if (B_Port < NumberOfAdcSetting)
   {
      //
      // Save ADC settings.
      //
      W_Addr = AdcSettingsStart + ( BandgapSettingSize + ChecksumSize) +
               ((AdcSettingsSize+ChecksumSize) * B_Port);

      NVRam_WriteBuffer(W_Addr, AdcSettings, AdcSettingsSize);
      //
      // Calculate ADC settings checksum.
      // Update ADC settings checksum.
      //
      B_Checksum = CalculateChecksum(AdcSettings, AdcSettingsSize);
      W_Addr += AdcSettingsSize;
   }
   NVRam_WriteByte(W_Addr, B_Checksum);

   return (TRUE);
}

#endif // FEATURE_AIP == ENABLE

//******************************************************************************
//
// FUNCTION       : BOOL ReadMonitorPowerOnTimefromNVRAM(DWORD *pDW_MonitorPowerOnTime)
//
// USAGE          : Read Monitor Power On Time from NVRAM.
//
// INPUT          : DWORD *pDW_MonitorPowerOnTime
//
// OUTPUT         : TRUE  - data read back ok
//                 FALSE - get data failed
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  : None
//
// POST_CONDITION : None
//
//******************************************************************************
BOOL ReadMonitorPowerOnTimefromNVRAM(DWORD *pDW_MonitorPowerOnTime)
{
   BYTE B_Checksum;

   NVRam_ReadBuffer(MonitorPowerOnTimeStart, (BYTE*)pDW_MonitorPowerOnTime, sizeof(DWORD)); // (DWORD)->4 bytes are used for Monitor Power On information in NVRAM.

   B_Checksum = CalculateChecksum((BYTE*)pDW_MonitorPowerOnTime, sizeof(DWORD));

   //
   // check Monitor Power On Time checksum.
   //
   if (NVRam_ReadByte(MonitorChecksumStart) == B_Checksum)	   // Checksum OK
      return TRUE; 							   // yes

   else
   {
      DW_MonitorPowerOnTime = 0;
      WriteMonitorPowerOnTimeintoNVRAM(&DW_MonitorPowerOnTime);
      gm_Printf("Warning: Monitor checksum did not match!", 0);
      return FALSE;
   }
}

//******************************************************************************
//
// FUNCTION       : void WriteMonitorPowerOnTime_into_NVRAM(DWORD *pDW_MonitorPowerOnTime)
//
// USAGE          : Write Monitor Power On Time into NVRAM.
//
// INPUT          : DWORD *pDW_MonitorPowerOnTime
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  : None
//
// POST_CONDITION : None
//
//******************************************************************************
void WriteMonitorPowerOnTimeintoNVRAM(DWORD *pDW_MonitorPowerOnTime)
{
   BYTE B_Checksum;

   NVRam_WriteBuffer(MonitorPowerOnTimeStart, (BYTE*)pDW_MonitorPowerOnTime, sizeof(DWORD));

   //
   // Calculate Monitor Power On checksum.
   // Update Monitor Power On checksum in NVRAM.
   //
   B_Checksum = CalculateChecksum((BYTE*)pDW_MonitorPowerOnTime, sizeof(DWORD));

   NVRam_WriteByte(MonitorChecksumStart, B_Checksum);
}


//******************************************************************************
//
// FUNCTION       : void ReadMonitorSerialNumber_from_NVRAM(char *pBa_MonitorSerialNumberWriteBuffer)
//
// USAGE          : Read Monitor Serial Number from NVRAM.
//
// INPUT          : char *pBa_MonitorSerialNumberWriteBuffer
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  : None
//
// POST_CONDITION : None
//
//******************************************************************************
void ReadMonitorSerialNumberfromNVRAM(BYTE *pBa_MonitorSerialNumberWriteBuffer)
{
#if 1
	NVRam_ReadBuffer(EH_MonitorSerialNumberStart, (BYTE*)pBa_MonitorSerialNumberWriteBuffer, B_MONITOR_SERIAL_NUMBER_LENGHT); // size of Monitor Serial Number string.
#else
   NVRam_ReadBuffer(MonitorSerialNumberStart, (BYTE*)pBa_MonitorSerialNumberWriteBuffer, B_MONITOR_SERIAL_NUMBER_LENGHT); // size of Monitor Serial Number string.
#endif
}

#ifdef WRITE_MONITOR_SERIAL_NUMBER
//******************************************************************************
//
// FUNCTION       : void WriteMonitorSerialNumber_into_NVRAM(char *pBa_MonitorSerialNumberWriteBuffer)
//
// USAGE          : Write Monitor Serial Number into NVRAM.
//
// INPUT          : char *pBa_MonitorSerialNumberWriteBuffer
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  : None
//
// POST_CONDITION : None
//
//******************************************************************************
void WriteMonitorSerialNumberintoNVRAM(BYTE *pBa_MonitorSerialNumberWriteBuffer)
{
#if 1
	NVRam_WriteBuffer(EH_MonitorSerialNumberStart, (BYTE*)pBa_MonitorSerialNumberWriteBuffer, B_MONITOR_SERIAL_NUMBER_LENGHT); // size of Monitor Serial Number string.
#else
	NVRam_WriteBuffer(MonitorSerialNumberStart, (BYTE*)pBa_MonitorSerialNumberWriteBuffer, B_MONITOR_SERIAL_NUMBER_LENGHT); // size of Monitor Serial Number string.
#endif
}
#endif
/************************************* END ***********************************/

