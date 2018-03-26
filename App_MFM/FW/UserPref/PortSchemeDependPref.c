/*
	$Workfile:   PortSchemeDependPref.c  $
	$Revision: 1.5 $
	$Date: 2013/01/24 09:15:26 $
*/
#define __PORTDEPEND_C__
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
// MODULE:      PortDependPref.c
//
// USAGE:       This module contains functions for user preference store and
//				restore control to NVRAM for Port Dependent Setting.
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include <string.h>
#include "System\all.h"

//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************

//#define _DEBUG
#ifdef _DEBUG
#define msg(a,b,c,d)		gm_Print(a,b,c,d);
#define msg_i(a,b,c)		gm_Printf(a,b,c);
#else
#define msg(a,b,c,d)
#define msg_i(a,b,c)
#endif


extern PortSchemeDependentUserPrefType ROM SchemeFactoryDefaultInROM[NUM_IMAGE_SCHEMES];
#ifdef DDCCI_VALUE_CHANGE
extern BYTE B_DDCCINewControlVal;
#endif

//******************************************************************************
//  S T A T I C    V A R I A B L E S
//******************************************************************************

//******************************************************************************
//  S T A T I C    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

//******************************************************************************
//  C O D E
//******************************************************************************

#pragma pack(push)
#pragma pack(1)
typedef struct
{
   PortSchemeDependentUserPrefType st; //structure PSD
   BYTE cs; //check sum
} PortSchemeDependentUserPrefTypeCS; // PSD
#pragma pack(pop)

//******************************************************************************
//
// FUNCTION       : void InitPortDependentDescr(void)
//
// USAGE          : Initialize port dependent descriptor.  This descriptor stores the
//                  NUM_OF_CONNECTORS value in FW.  It is used to compare to see if a new
//                  board project is being used and therefore nvram should be reinitialized
//                  so that the port dependent entries are reflected.
//
// INPUT          :
//
// OUTPUT         : None
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
void InitPortSchemeDependentDescr(void)
{
   NVRam_WriteByte(PortSchemeDependentIndexStart, NUM_OF_IMAGE_SCHEMES_ENTRIES);
}

//******************************************************************************
//
// FUNCTION       : void InitPortDependent(void)
//
// USAGE          : Initialize Port Dependent settings to Factory defaults in ROM
//
// INPUT          :
//
// OUTPUT         : None
//
// GLOBALS        : SchemeFactoryDefaultInROM[UserPrefImageScheme]		(RO)
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void InitPortSchemeDependent(void)
{
   WORD W_Address;
   BYTE i;
   PortSchemeDependentUserPrefTypeCS Stp_tmp; // PSD

   W_Address  = PortSchemeDependentEntryStart;

   for (i = 0; i < NUM_OF_IMAGE_SCHEMES_ENTRIES; i++)
   {
#if (PORT_SCHEME_DEPENDENT == SCHEME_DEPENDENT)
      Stp_tmp.st = SchemeFactoryDefaultInROM[i]; // PSD
      Stp_tmp.cs = CalculateChecksum(&Stp_tmp.st, sizeof(Stp_tmp.st));
#else
      Stp_tmp.st = SchemeFactoryDefaultInROM[VIVID]; // PSD
      Stp_tmp.cs = CalculateChecksum(&Stp_tmp.st, sizeof(Stp_tmp.st));
#endif

      NVRam_WriteBuffer(W_Address, (BYTE *)&Stp_tmp, sizeof(Stp_tmp));
      W_Address += sizeof(Stp_tmp);
   }
}

//******************************************************************************
//
// FUNCTION       : BOOL ReadPortDependentEntry(BYTE B_Index)
//
// USAGE          : Reads the pointed PortDependentUserPreferences record from
//                  NVRAM, checking the record checksum as well.
//
// INPUT          : B_Index
//
// OUTPUT         : TRUE if NVRAM entry found and CRC match,
//                  FALSE if calculated CRC don't match with NVRAM CRC.
//
// GLOBALS        : SchemeFactoryDefaultInROM[UserPrefImageScheme]	(RO)
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BOOL ReadPortSchemeDependentEntry(BYTE B_Index)
{
   WORD W_Address;					// holds the computed NVRAM address
   BYTE B_Checksum1, B_Checksum2;	// holds the computed checksum values

   // compute the start address
   W_Address = PortSchemeDependentEntryStart + (WORD)B_Index * (PortSchemeDependentUserPrefSize + ChecksumSize);

   // read the PortDependentUserPreferences record and save it
   NVRam_ReadBuffer(W_Address, (BYTE *)(&PortSchemeDependentUserPreferences), PortSchemeDependentUserPrefSize);

//	UserPrefImageScheme = UserPrefMFMImageScheme;

   // read the checksum byte
   B_Checksum1 = NVRam_ReadByte(W_Address + PortSchemeDependentUserPrefSize);
   // calculate checksum.
   //B_Checksum2 = CalculateChecksum(st_PD, PortSchemeDependentUserPrefSize);
   B_Checksum2 = CalculateChecksum((&PortSchemeDependentUserPreferences), PortSchemeDependentUserPrefSize);

   if (B_Checksum1 != B_Checksum2)
   {
      PortSchemeDependentUserPreferences = SchemeFactoryDefaultInROM[B_Index];
      NVRam_WriteBuffer(W_Address, (BYTE *)(&PortSchemeDependentUserPreferences), PortSchemeDependentUserPrefSize);
      msg_i("PSD: NVRAM entry %d checksum different: %x != %x", B_Index, B_Checksum1 );
      return FALSE;
   }

   msg_i("PSD: NVRAM entry %d found matched, address 0x%x; ", B_Index, W_Address);
   return TRUE;
}

//******************************************************************************
//
// FUNCTION       : BOOL CheckPortDependentEntry(BYTE B_Index )
//
// USAGE          : Checks checksum of pointed entry to see if it is valid to be read
//
// INPUT          : BYTE index - entry index in the PortDependentEntry array.
//                     Values are from gmt_UserDefConnNames enum
//
// OUTPUT         : TRUE, FALSE
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
BOOL CheckPortSchemeDependentEntry(BYTE B_Index )
{
   PortSchemeDependentUserPrefTypeCS st_PD;

   if (NVRam_ReadBuffer(PortSchemeDependentEntryStart + B_Index * (PortSchemeDependentUserPrefSize + ChecksumSize),
                        (BYTE*)&st_PD, sizeof(st_PD)) != OK)
      return FALSE;

   // calculate mode dependent checksum.
   B_Index = CalculateChecksum(&st_PD.st, PortSchemeDependentUserPrefSize);

   // We need to make sure the data the we retrieved is valid otherwise return FALSE
   if (B_Index != st_PD.cs)
   {
      msg_i("PSD: PD entry Checksum failed: 0x%x != 0x%x",B_Index, st_PD.cs);
      return FALSE;
   }
   return TRUE;
}

//******************************************************************************
//
// FUNCTION       : BOOL CheckPortDependentDescriptor(void)
//
// USAGE          : Check port dependent descriptor
//
// INPUT          :
//
// OUTPUT         : TRUE if number of connectors is the same
//                  FALSE if we have differing number of connectors
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
BOOL CheckPortSchemeDependentDescriptor(void)
{
   BYTE B_NumEntries = NVRam_ReadByte(PortSchemeDependentIndexStart);

   if (B_NumEntries != NUM_OF_IMAGE_SCHEMES_ENTRIES)
   {
      msg_i("PSD: NUM_OF_CONNECTORS defined in FW: %d; Descriptor value: %d", NUM_OF_IMAGE_SCHEMES_ENTRIES, B_NumEntries);
      return FALSE;
   }
   return TRUE;
}


//******************************************************************************
//
// FUNCTION       : void SavePortDependentSettings(void)
//
// USAGE          : Save port dependent user preference settings by index to NVRam
//
// INPUT          :
//
// OUTPUT         : None
//
// GLOBALS        : gmvb_InputConnector[]			(RO)
//                  PortDependentUserPreferences	(R)
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void SavePortSchemeDependentSettings(void)
{
#if (PORT_SCHEME_DEPENDENT == SCHEME_DEPENDENT)
   BYTE B_Index = UserPrefImageScheme;
#else
   BYTE B_Index = gmvb_InputConnector[gmvb_MainChannel];
#endif

   WORD W_Address = PortSchemeDependentEntryStart + // holds the computed NVRAM address
                    (WORD)B_Index * (PortSchemeDependentUserPrefSize + ChecksumSize);

#ifdef DDCCI_VALUE_CHANGE
		WORD W_CheckSumAdd = PortSchemeDependentEntryStart + // holds the computed NVRAM address
							(WORD)B_Index * (PortSchemeDependentUserPrefSize + ChecksumSize) + PortSchemeDependentUserPrefSize ;
		BYTE B_CheckSumTemp;
		
		NVRam_ReadBuffer(W_CheckSumAdd, (BYTE *)(&B_CheckSumTemp), 1);
#endif

		//120530 Edward save the Brightness / contrast / Sharpness by scheme mode
		if(IsPresetMode_GRAPHICS_TEXT)
		{
			UserPrefBrightness_Text = UserPrefBrightness;
			UserPrefContrast_Text = UserPrefContrast;
			UserPrefPortSharpness_Text = UserPrefPortSharpness;
		}
		else if(IsColorSpace_GRAPHICS_CAL1)//120806 Edward add for CAL1/CAL2 save seperate
		{
			UserPrefBrightness_CAL1 = UserPrefBrightness;
			UserPrefContrast_CAL1 = UserPrefContrast;
			UserPrefPortSharpness_All = UserPrefPortSharpness;
		}
		else if(IsColorSpace_GRAPHICS_CAL2)
		{
			UserPrefBrightness_CAL2 = UserPrefBrightness;
			UserPrefContrast_CAL2 = UserPrefContrast;
			UserPrefPortSharpness_All = UserPrefPortSharpness;
		}
		else
		{
			UserPrefBrightness_All = UserPrefBrightness;
			UserPrefContrast_All = UserPrefContrast;
			UserPrefPortSharpness_All = UserPrefPortSharpness;
		}
		//120608 Edward save the Hue / Saturation 
		if(IsPresetMode_GRAPHICS_MOVIE)
		{
			UserPrefHue_Movie = UserPrefHue;
			UserPrefSaturation_Movie = UserPrefSaturation;

			//120726 Edward add for spec change DCR on Movie / Game are the same
			UserPrefPortSchemeDependentPresetModesWB_MovieDCR =\
			UserPrefPortSchemeDependentPresetModesWB_GameDCR = UserPrefDCR;
			//gm_Print("save  Movie DCR %d",UserPrefDCR);
			//gm_Print("save  UserPrefPortSchemeDependentPresetModesWB_MovieDCR %d",UserPrefPortSchemeDependentPresetModesWB_MovieDCR);
		}
		else if(IsPresetMode_GRAPHICS_GAME)
		{
			UserPrefHue_Game = UserPrefHue;
			UserPrefSaturation_Game = UserPrefSaturation;

			//120726 Edward add for spec change DCR on Movie / Game are the same
			UserPrefPortSchemeDependentPresetModesWB_GameDCR =\
			UserPrefPortSchemeDependentPresetModesWB_MovieDCR = UserPrefDCR;
			//gm_Print("save  Game DCR %d",UserPrefDCR);			
			//gm_Print("save  UserPrefPortSchemeDependentPresetModesWB_GameDCR %d",UserPrefPortSchemeDependentPresetModesWB_GameDCR);
		}
		else
		{
			UserPrefHue = 0;
			UserPrefSaturation = WB_SATURATION_DEFAULT;
		}	
		
		
#if (PORT_SCHEME_DEPENDENT == PORT_DEPENDENT)
   if (UserPrefImageScheme != USER)
   {
      PortSchemeDependentUserPrefType PortSchemeDependentUserPreferences_tmp =
         PortSchemeDependentUserPreferences;
      ReadPortSchemeDependentEntry(B_Index);
      PortSchemeDependentUserPreferences.MFMImageScheme = PortSchemeDependentUserPreferences_tmp.MFMImageScheme;
      NVRam_WriteBuffer(W_Address, (BYTE *)(&PortSchemeDependentUserPreferences), PortSchemeDependentUserPrefSize);
      NVRam_WriteByte(W_Address + PortSchemeDependentUserPrefSize,
                      CalculateChecksum(&PortSchemeDependentUserPreferences, PortSchemeDependentUserPrefSize));
      PortSchemeDependentUserPreferences = PortSchemeDependentUserPreferences_tmp;
   }
   else
   {
      NVRam_WriteBuffer(W_Address, (BYTE *)(&PortSchemeDependentUserPreferences), PortSchemeDependentUserPrefSize);
      NVRam_WriteByte(W_Address + PortSchemeDependentUserPrefSize,
                      CalculateChecksum(&PortSchemeDependentUserPreferences, PortSchemeDependentUserPrefSize));
   }
#else
   NVRam_WriteBuffer(W_Address, (BYTE *)(&PortSchemeDependentUserPreferences), PortSchemeDependentUserPrefSize);
   NVRam_WriteByte(W_Address + PortSchemeDependentUserPrefSize,
                   CalculateChecksum(&PortSchemeDependentUserPreferences, PortSchemeDependentUserPrefSize));
#endif

#ifdef DDCCI_VALUE_CHANGE
	if(B_CheckSumTemp != CalculateChecksum(&PortSchemeDependentUserPreferences, PortSchemeDependentUserPrefSize))
	{
		B_DDCCINewControlVal = 1;
	}
#endif

	msg_i("PSD: Address of: 0x%x; saved NVRAM mode entry %d",W_Address, B_Index);
}
/*********************************  END  **************************************/
