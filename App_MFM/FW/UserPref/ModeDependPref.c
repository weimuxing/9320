/*
	$Workfile:   ModeDependPref.c  $
	$Revision: 1.8 $
	$Date: 2013/03/19 06:12:16 $
*/
#define __MODEDEPEND_C__
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
//=============================================================
//
// MODULE:      ModeDependPref.c
//
// USAGE:       This module contains functions for user preference lookup and storage
//				 in the NVRAM for Mode Dependent Setting.
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include <string.h>
#include <math.h>
#include "System\all.h"


#if FEATURE_AIP == ENABLE

//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************

//#define _DEBUG
//#define _DEBUG_EXT

#ifdef _DEBUG
#define msg(a)		gm_Print(a,0);
#define msg_i(a,b)	gm_Print(a,b);
#else
#define msg(a)
#define msg_i(a,b)
#endif

#ifdef _DEBUG_EXT
#define msgx(a)		gm_Print(a,0);
#define msgx_i(a,b)	gm_Print(a,b);
#else
#define msgx(a)
#define msgx_i(a,b)
#endif


typedef enum
{
   gmd_LookupEntry,
   gmd_PresetEntry,
   gmd_UserEntry,
#ifdef CVT_MODES_SUPPORT
   gmd_CvtLookupEntry,
   gmd_CvtEntry
#endif
} gmt_ModeDependentEntryType;

//******************************************************************************
//  S T A T I C    V A R I A B L E S
//******************************************************************************

#ifdef CVT_MODES_SUPPORT
static BYTE ModeDependentCvtIndex;
#endif

static BYTE ModeDependentUserIndex;

//*****************************************************************************
//  G L O B A L    V A R I A B L E S
//*****************************************************************************

BYTE B_AutoAdjustCount[NUM_CHANNELS];
#ifdef DDCCI_VALUE_CHANGE
extern BYTE B_DDCCINewControlVal;
#endif

//******************************************************************************
//  S T A T I C    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

static void UpdateModeDependentDescr		(void);
static void ClearModeDependentLookUpEntry(WORD W_Address);
static void SaveModeDependentEntry(WORD W_Address,
                                   ModeDependentUserPrefType * UserPrefTypePtr,
                                   BYTE B_AutoAdjustCount);
static void SaveModeDependentUserPrefEntry(WORD W_Address,
      ModeDependentUserPrefType * UserPrefTypePtr,
      BYTE B_AutoAdjustCount,
      gmt_OSD_Adjuster_Info * AdjusterInfo);
static void SaveModeDependentLookUpEntry(WORD W_Address,
      ModeDependentLookUpType * LookUpTypePtr);
static BOOL IsCheckSumConsistent(WORD W_Address,
                                 BYTE* ST_Temp,	gmt_ModeDependentEntryType Type);
static BOOL CheckModeDependentPresetEntry(WORD W_Address);
static BYTE CheckModeDependentLookUpEntry(WORD W_Address,
      ModeDependentLookUpType * LookUpTypePtr);
static BOOL ReadModeDependentEntry(WORD W_Address,
                                   ModeDependentUserPrefType * UserPrefTypePtr, gmt_ModeDependentEntryType Type,
                                   WORD W_LookUpEntryAddress);
static BOOL ReadModeDependentUserPrefEntry(WORD W_Address,
      ModeDependentUserPrefType * UserPrefTypePtr, gmt_ModeDependentEntryType Type,
      WORD W_LookUpEntryAddress);
static WORD GetStartAddressOfEntry(BYTE B_Index, gmt_ModeDependentEntryType Type);

static BYTE CheckModeDependentEntry(BYTE	B_TableIndex, gmt_PHY_CH B_Channel,
                                    ModeDependentLookUpType * LookUpTypePtr);

//******************************************************************************
//  C O D E
//******************************************************************************

//******************************************************************************
//
// FUNCTION       : void InitModeDependentDescr(void)
//
// USAGE          : Initialize mode dependent descriptor and user index
//
// INPUT          :
//
// OUTPUT         : None
//
// GLOBALS        : ModeDependentUserIndex		(R)
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void InitModeDependentDescr(void)
{
   ModeDependentUserIndex = 0;


#ifdef CVT_MODES_SUPPORT
   ModeDependentCvtIndex = 0;
#endif

   UpdateModeDependentDescr();
}

//******************************************************************************
//
// FUNCTION       : static void UpdateModeDependentDescr(void)
//
// USAGE          : Update mode dependent descriptor with user index
//
// INPUT          :
//
// OUTPUT         : None
//
// GLOBALS        : ModeDependentUserIndex	(R)
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
static void UpdateModeDependentDescr(void)
{
#ifdef CVT_MODES_SUPPORT
   NVRam_WriteByte(ModeDependentCvtIndexStart, ModeDependentCvtIndex);
#endif	// CVT_MODES_SUPPORT

   NVRam_WriteByte(ModeDependentUserIndexStart, ModeDependentUserIndex);
}

//******************************************************************************
//
// FUNCTION       : void InitModeDependentLookUp(void)
//
// USAGE          : Initialize mode dependent timing tags look-up table.
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
void InitModeDependentLookUp(void)
{
   BYTE							B_CheckSum;
   BYTE							B_CheckSum1;
   BYTE							i;
   WORD							W_Address;
   WORD							W_Address1;
   ModeDependentLookUpType			Stp_tmp;
   ModeDependentUserPrefType		Stp_tmp1;

   BYTE 							B_Index;

   //
   // clear the local copy
   //
   memset(&Stp_tmp, 0, ModeDependentLookUpSize);
   memset(&Stp_tmp1, 0, ModeDependentUserPrefSize);

   // Calculate incorrect checksum -- the rest of code depends on this?

   B_CheckSum = CalculateChecksum(&Stp_tmp,
                                  ModeDependentLookUpSize) + 1;
   B_CheckSum1 = CalculateChecksum(&Stp_tmp1,
                                   ModeDependentUserPrefSize) + 1;

   //
   // Clear all user entries
   //
   W_Address  = GetStartAddressOfEntry(0, gmd_LookupEntry);
   W_Address1  = GetStartAddressOfEntry(0, gmd_UserEntry);

   for (i = 0; i < MAX_USER_MODES_SAVED; i++)
   {
      NVRam_WriteBuffer(W_Address,
                        (BYTE *)&Stp_tmp,
                        ModeDependentLookUpSize);

      NVRam_WriteBuffer(W_Address1,
                        (BYTE *)&Stp_tmp1,
                        ModeDependentUserPrefSize);

      W_Address  += ModeDependentLookUpSize;
      W_Address1 += ModeDependentUserPrefSize;

      NVRam_WriteByte(W_Address1, 0);
      W_Address1 += sizeof(BYTE);

      NVRam_WriteBuffer(W_Address,
                        (BYTE *)&B_CheckSum,
                        ChecksumSize);

      NVRam_WriteBuffer(W_Address1,
                        (BYTE *)&B_CheckSum1,
                        ChecksumSize);

      W_Address  += ChecksumSize;
      W_Address1 += ChecksumSize;
   }

#ifdef CVT_MODES_SUPPORT
   // Clear all CVT entries
   W_Address = GetStartAddressOfEntry(0, gmd_CvtLookupEntry);
   W_Address1 = GetStartAddressOfEntry(0, gmd_CvtEntry);

   for (i = 0; i < MAX_CVT_MODES_SAVED; i++)
   {
      NVRam_WriteBuffer(W_Address,
                        (BYTE *)&Stp_tmp,
                        ModeDependentLookUpSize);

      NVRam_WriteBuffer(W_Address1,
                        (BYTE *)&Stp_tmp1,
                        ModeDependentUserPrefSize);

      W_Address  += ModeDependentLookUpSize;
      W_Address1 += ModeDependentUserPrefSize;

      NVRam_WriteByte(W_Address1, 0);
      W_Address1 += sizeof(BYTE);

      NVRam_WriteBuffer(W_Address,
                        (BYTE *)&B_CheckSum,
                        ChecksumSize);

      NVRam_WriteBuffer(W_Address1,
                        (BYTE *)&B_CheckSum1,
                        ChecksumSize);

      W_Address  += ChecksumSize;
      W_Address1 += ChecksumSize;
   }
#endif

   // Clear all preset entries
   W_Address  = GetStartAddressOfEntry(0, gmd_PresetEntry);

   for (i = 0; i < NumOfGraphicsPresetMode; i++)
   {
      msgx_i("i = %d", i);
      msgx_i("address: 0x%x", W_Address);

      NVRam_WriteBuffer(W_Address,
                        (BYTE *)&Stp_tmp1,
                        ModeDependentUserPrefSize);

      W_Address += ModeDependentUserPrefSize;

      NVRam_WriteByte(W_Address, 0);

      W_Address += sizeof(BYTE) + ChecksumSize;
   }
}

//******************************************************************************
//
// FUNCTION       : static WORD GetStartAddressOfEntry(BYTE B_Index, gmt_ModeDependentEntryType Type)
//
// USAGE          : Computes the starting address of the entry corresponding to
//                  B_Index, B_Channel and Type
//
// INPUT          : B_Index - index in NVRAM
//                  B_Channel - CH_A or CH_B
//                  Type - specify which of the following is requested:
//                     gmd_LookupEntry, gmd_PresetEntry and gmd_UserEntry
//
// OUTPUT         : starting address in WORD
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
static WORD GetStartAddressOfEntry(BYTE B_Index, gmt_ModeDependentEntryType Type)
{
   WORD W_Address, W_EntrySize;

   switch (Type)
   {
      case gmd_LookupEntry:
         W_Address = ModeDependentUserLookUpStart;
         W_EntrySize = ModeDependentLookUpSize + ChecksumSize;
         break;

      case gmd_PresetEntry:
         W_Address = ModeDependentPresetEntryStart;
         W_EntrySize = ModeDependentUserPrefSize + sizeof(BYTE)
                       + ChecksumSize;
         break;

      case gmd_UserEntry:
      default:
         W_Address = ModeDependentUserEntryStart;
         W_EntrySize = ModeDependentUserPrefSize + sizeof(BYTE)
                       + ChecksumSize;
         break;
   }

#ifdef CVT_MODES_SUPPORT
   if (Type == gmd_CvtLookupEntry)
   {
      W_Address = ModeDependentCvtLookUpStart;
      W_EntrySize = ModeDependentLookUpSize + ChecksumSize;
   }
   else if (Type == gmd_CvtEntry)
   {
      W_Address = ModeDependentCvtEntryStart;
      W_EntrySize = ModeDependentUserPrefSize + sizeof(BYTE) + ChecksumSize;
   }
#endif

   // compute the NVRAM address
   W_Address += (WORD)B_Index * W_EntrySize;

   return W_Address;
}

//******************************************************************************
//
// FUNCTION       : static BOOL IsCheckSumConsistent(WORD W_Address, BYTE* ST_Temp,
//                  						gmt_ModeDependentEntryType Type)
//
// USAGE          : Checks if the checksum in the NVRAM is consistent with the
//                  userpref data stored in the NVRAM.
//
// INPUT          : W_Address - starting address of the entry of type Type
//                  ST_Temp - pointer pointing to a pre-allocated structure to store
//                     the buffer data corresponding to B_Index
//                  Type - specify the nature of the entry:
//                     gmd_LookupEntry, gmd_PresetEntry and gmd_UserEntry
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
static BOOL IsCheckSumConsistent(WORD W_Address, BYTE* ST_Temp,
                                 gmt_ModeDependentEntryType Type)
{
   BYTE B_Checksum1, B_Checksum2;
   WORD W_BufferSize, W_CheckSumAddress;

   switch (Type)
   {
      case gmd_LookupEntry:
         W_CheckSumAddress = W_Address + ModeDependentLookUpSize;
         W_BufferSize = ModeDependentLookUpSize;
         break;

      case gmd_PresetEntry:
      case gmd_UserEntry:
#ifdef CVT_MODES_SUPPORT
      case gmd_CvtEntry:
#endif
      default:
         W_CheckSumAddress = W_Address + ModeDependentUserPrefSize
                             + sizeof(BYTE);
         W_BufferSize = ModeDependentUserPrefSize;
   }

   // read the checksum byte
   B_Checksum1 = NVRam_ReadByte(W_CheckSumAddress);

   // read the ModeDependentUserPref/ModeDependentLookUp record and save it
   if (NVRam_ReadBuffer(W_Address, ST_Temp, W_BufferSize) == OK)
   {

      // calculate checksum.
      B_Checksum2 = CalculateChecksum(ST_Temp, W_BufferSize);

      if (B_Checksum1 == B_Checksum2)
      {
         return TRUE;
      }
      else
      {
         msgx_i("checksum1 = %x", B_Checksum1);
         msgx_i("checksum2 = %x", B_Checksum2);
         return FALSE;
      }
   }
   else
   {
      return FALSE;
   }
}

//******************************************************************************
//
// FUNCTION       : static BOOL ReadModeDependentEntry(WORD W_Address,
//                  	ModeDependentUserPrefType * UserPrefTypePtr, gmt_ModeDependentEntryType Type,
//                  	WORD W_LookUpEntryAddress)
//
// USAGE          : Checks if the checksum is consistent with the entry stored in
//                  the NVRAM. If consistent then updates global user preferences
//                  and returns TRUE. Otherwise clear the
//                  user preferences and returns FALSE.
//
// INPUT          : W_Address - starting address of the entry
//                  UserPrefTypePtr - pointer to a structure that is to be overwritten
//                  Type - type of the above structure
//                  W_LookUpEntryAddress - starting address of the lookup entry
//
// OUTPUT         : TRUE if NVRAM checksum is consistent with the entry
//                  FALSE otherwise
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
static BOOL ReadModeDependentEntry(WORD W_Address,
                                   ModeDependentUserPrefType * UserPrefTypePtr, gmt_ModeDependentEntryType Type,
                                   WORD W_LookUpEntryAddress)
{
   BYTE B_Checksum1, B_Checksum2;	// holds the computed checksum values
   ModeDependentUserPrefType ST_Temp;

   if (IsCheckSumConsistent(W_Address, (BYTE *)&ST_Temp, Type))
   {
      msgx_i("Address of retrieved value 0x%x", W_Address);

      memcpy(UserPrefTypePtr,
             (WORD*)(&ST_Temp.HStart),
             ModeDependentUserPrefSize);
      return TRUE;
   }
   else
   {
      // clear this entry.
      memset(UserPrefTypePtr, 0, ModeDependentUserPrefSize);

      SaveModeDependentEntry(W_Address, UserPrefTypePtr, 0);
      ClearModeDependentLookUpEntry(W_LookUpEntryAddress);

      msg_i("NVRAM entry checksum different",0);

      return FALSE;
   }
}

//******************************************************************************
//
// FUNCTION       : static BOOL ReadModeDependentUserPrefEntry(WORD W_Address,
//                  	ModeDependentUserPrefType * UserPrefTypePtr, gmt_ModeDependentEntryType Type,
//                  	WORD W_LookUpEntryAddress)
//
// USAGE          : Checks if the checksum is consistent with the entry stored in
//                  the NVRAM. If consistent then updates global user preferences
//                  and returns TRUE. Otherwise clear the
//                  user preferences and returns FALSE.
//
// INPUT          : W_Address - starting address of the entry
//                  UserPrefTypePtr - pointer to a structure that is to be overwritten
//                  Type - type of the above structure
//                  W_LookUpEntryAddress - starting address of the lookup entry
//
// OUTPUT         : TRUE if NVRAM checksum is consistent with the entry
//                  FALSE otherwise
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
static BOOL ReadModeDependentUserPrefEntry(WORD W_Address,
      ModeDependentUserPrefType * UserPrefTypePtr, gmt_ModeDependentEntryType Type,
      WORD W_LookUpEntryAddress)
{
   BYTE B_Checksum1, B_Checksum2;	// holds the computed checksum values
   ModeDependentUserPrefType ST_Temp;

   if (IsCheckSumConsistent(W_Address, (BYTE *)&ST_Temp, Type))
   {
      msgx_i("Address of retrieved value 0x%x", W_Address);

      memcpy(UserPrefTypePtr,
             (WORD*)(&ST_Temp.HStart),
             ModeDependentUserPrefSize);
      return TRUE;
   }
   else
   {
      // clear this entry.
      memset(UserPrefTypePtr, 0, ModeDependentUserPrefSize);

      SaveModeDependentEntry(W_Address, UserPrefTypePtr, 0);
      ClearModeDependentLookUpEntry(W_LookUpEntryAddress);

      msg_i("NVRAM entry checksum different",0);

      return FALSE;
   }
}

//******************************************************************************
//
// FUNCTION       : static BOOL CheckModeDependentPresetEntry(WORD W_Address)
//
// USAGE          : Checks if the checksum is consistent with the entry stored in
//                  the NVRAM. Returns TRUE if the checksum is consistent and the
//                  HTotal in the NVRAM entry is non-zero. Returns FALSE otherwise.
//
// INPUT          : W_Address - starting address of the preset entry
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
static BOOL CheckModeDependentPresetEntry(WORD W_Address)
{
   ModeDependentUserPrefType	St_PresetRecord;
   BYTE						B_Checksum1;
   BYTE						B_Checksum2;

   msg_i("Read address: 0x%x", W_Address);

   // read the checksum byte
   msg_i("checksum address 0x%x",W_Address + ModeDependentUserPrefSize + sizeof(BYTE));

   // We need to make sure the data the we retrieved is valid otherwise
   // return FALSE
   if (IsCheckSumConsistent(W_Address, (BYTE *)&St_PresetRecord, gmd_PresetEntry))
   {
      msg_i("Preset HTotal: %d", St_PresetRecord.HTotal);

      if (St_PresetRecord.HTotal)
      {
         return TRUE;
      }
   }
   else
   {
      msg("Preset entry Checksum failed");
      return FALSE;
   }

   return FALSE;
}


//******************************************************************************
//
// FUNCTION       : static BYTE CheckModeDependentLookUpEntry(WORD W_Address,
//                  									ModeDependentLookUpType * LookUpTypePtr)
//
// USAGE          : Checks if the checksum is consistent with the entry stored in
//                  the NVRAM. Returns EndOfLookUpIndex if checksum is consistent
//                  and no element of the NVRAM entry is zero. Returns TRUE if
//                  checksum is consistent and the elements of the entry are within
//                  tolerance. Returns FALSE otherwise.
//
// INPUT          : W_Address - starting address of the lookup entry
//                  LookUpTypePtr - pointer to an input data structure for comparison
//                     with the saved data in NVRAM
//
// OUTPUT         : TRUE, FALSE or EndOfLookUpIndex
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
static BYTE CheckModeDependentLookUpEntry(WORD W_Address,
      ModeDependentLookUpType * LookUpTypePtr)
{
   BYTE B_Checksum1;
   BYTE B_Checksum2;
   ModeDependentLookUpType St_SavedLookUpRecord;	// holds lookup record

   if (IsCheckSumConsistent(W_Address, (BYTE *)&St_SavedLookUpRecord, gmd_LookupEntry))
   {
      // if the timing tags are 0, no record with this index
      if (St_SavedLookUpRecord.W_Hfreq   == 0
         || St_SavedLookUpRecord.W_Vfreq   == 0
         || St_SavedLookUpRecord.W_Vtotal  == 0)
      {
         return EndOfLookUpIndex;
      }

      // if H&Vfreq, Vtotal and polarity match, we found the entry
      else if ((abs(St_SavedLookUpRecord.W_Hfreq - LookUpTypePtr->W_Hfreq)) <= SavedHFreqDelta
              && (abs(St_SavedLookUpRecord.W_Vfreq - LookUpTypePtr->W_Vfreq)) <= SavedVFreqDelta
              && (abs(St_SavedLookUpRecord.W_Vtotal - LookUpTypePtr->W_Vtotal)) <= SavedVTotalDelta)
      {
         return TRUE;
      }
      else
      {
         msgx_i("St_SavedLookUpRecord.W_Hfreq       = %d",
               St_SavedLookUpRecord.W_Hfreq);
         msgx_i("LookUpTypePtr->W_HFreq = %d",
               LookUpTypePtr->W_Hfreq);
         msgx_i("St_SavedLookUpRecord.W_Vfreq       = %d",
               St_SavedLookUpRecord.W_Vfreq);
         msgx_i("LookUpTypePtr->W_VFreq = %d",
               LookUpTypePtr->W_Vfreq);
         msgx_i("St_SavedLookUpRecord.W_Vtotal       = %d",
               St_SavedLookUpRecord.W_Vtotal);
         msgx_i("LookUpTypePtr->W_VTotal = %d",
               LookUpTypePtr->W_Vtotal);
         msgx("User Lookup Checksum OK but values do not match");
      }
   }
   else
   {
      msgx("User Lookup Checksum failed");
   }
   return FALSE;
}

//******************************************************************************
//
// FUNCTION       : BOOL CheckModeDependentDescriptor(void)
//
// USAGE          : Updates mode dependent descriptor from NVRAM and checks to
//                  ensure it is within range.
//
// INPUT          :
//
// OUTPUT         : TRUE if user index is within range
//                  FALSE otherwise
//
// GLOBALS        : ModeDependentUserIndex		(R/W)
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BOOL CheckModeDependentDescriptor(void)
{
   ModeDependentUserIndex = NVRam_ReadByte(ModeDependentUserIndexStart);

#ifdef CVT_MODES_SUPPORT
   ModeDependentCvtIndex = NVRam_ReadByte(ModeDependentCvtIndexStart);

   if (ModeDependentUserIndex < MAX_USER_MODES_SAVED &&
         ModeDependentCvtIndex  < MAX_CVT_MODES_SAVED)
   {
      return TRUE;
   }

#else
   if (ModeDependentUserIndex < MAX_USER_MODES_SAVED)
   {
      return TRUE;
   }
   
#endif
   return FALSE;
}

//******************************************************************************
//
// FUNCTION       : void SaveModeDependentSettings(gmt_PHY_CH B_Channel,
//                  									ModeDependentLookUpType * LookUpTypePtr)
//
// USAGE          : Save mode dependent user preference settings by index to NVRAM.
//                  Overwrite entry if there is a matching entry in the NVRAM.
//                  Otherwise increment user index and saves the current entry.
//
// INPUT          : B_Channel - CH_A or CH_B
//                  LookUpTypePtr - pointer to an input data structure
//
// OUTPUT         : None
//
// GLOBALS        : ModeDependentUserIndex		(R/W)
//                  ModeDependent_HUDUserIndex  (R/W)
//                  B_ModeIndex					(R)
//                  B_AutoAdjustCount			(R)
//                  ModeDependentUserPreferences (R)
//                  ModeDependent_HUDUserPreferences (R)
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void SaveModeDependentSettings(gmt_PHY_CH B_Channel,
                               ModeDependentLookUpType * LookUpTypePtr)
{
   WORD i;
   BYTE B_Index = stInput[B_Channel].ModeIndex;
   WORD W_Address;
   ModeDependentUserPrefType * UserPrefTypePtr;
   gmt_ModeDependentEntryType Type;

#ifdef CVT_MODES_SUPPORT
   BYTE	TableSize = IsCvtMode(B_Channel) ? MAX_CVT_MODES_SAVED : MAX_USER_MODES_SAVED;
#else
   BYTE	TableSize = MAX_USER_MODES_SAVED;
#endif

   if (B_Index == 0xFF)
      return;

   UserPrefTypePtr = &ModeDependentUserPreferences;

   msg_i("SaveModeDependentSettings: %d",B_Index);

   // If not preset mode we need to update the user modes.
#ifdef CVT_MODES_SUPPORT
   if (IsCvtMode(B_Channel) || !IsPresetMode(B_Channel))
#else
   if (!IsPresetMode(B_Channel))
#endif
   {
      for (i = 0; i < TableSize; i++)
      {
         if (CheckModeDependentEntry(i,B_Channel, LookUpTypePtr) != FALSE)
         {
            break;
         }
      }

      // If we didn't find a match then we must write a new value to the user
      // mode table otherwise we do nothing.

      if (i == TableSize)
      {
#ifdef CVT_MODES_SUPPORT
         if (IsCvtMode(B_Channel))
         {
            // Note: Cyclic update scheme is used when the table is full.
            B_Index = ModeDependentCvtIndex;

            msg_i("ModeDependentCvtIndex = %d", ModeDependentCvtIndex);

            ModeDependentCvtIndex++;
            ModeDependentCvtIndex %= MAX_CVT_MODES_SAVED;
         }
         else
#endif
         {
            // Note: Cyclic update scheme is used when the table is full.
            B_Index = ModeDependentUserIndex;

            msg_i("ModeDependentUserIndex = %d", ModeDependentUserIndex);

            ModeDependentUserIndex++;
            ModeDependentUserIndex %= MAX_USER_MODES_SAVED;  
         }
         msg_i("new saved mode lookup index = %d", B_Index);
         stInput[B_Channel].ModeIndex = B_Index;
         UpdateModeDependentDescr();
      }
      else
      {
         msg_i("old saved mode lookup index = %d", B_Index);
         B_Index = i;
      }
      W_Address = GetStartAddressOfEntry(B_Index, gmd_LookupEntry);
#ifdef CVT_MODES_SUPPORT
      if (IsCvtMode(B_Channel))
      {
         W_Address = GetStartAddressOfEntry(B_Index, gmd_CvtLookupEntry);
      }
#endif
      SaveModeDependentLookUpEntry(W_Address, LookUpTypePtr);

      W_Address = GetStartAddressOfEntry(B_Index, gmd_UserEntry);
#ifdef CVT_MODES_SUPPORT
      if (IsCvtMode(B_Channel))
      {
         W_Address = GetStartAddressOfEntry(B_Index, gmd_CvtEntry);
      }
#endif
      SaveModeDependentEntry(W_Address, UserPrefTypePtr, B_AutoAdjustCount[B_Channel]);
   }
   else
   {
      W_Address = GetStartAddressOfEntry(B_Index, gmd_PresetEntry);
      SaveModeDependentEntry(W_Address, UserPrefTypePtr, B_AutoAdjustCount[B_Channel]);
   }
}

//******************************************************************************
//
// FUNCTION       : void SaveModeDependentUserPrefValue(gmt_PHY_CH B_Channel,
//                  									gmt_OSD_Adjuster_Info * AdjusterInfo)
//
// USAGE          : Save mode dependent user preference settings by index to NVRAM.
//                  Overwrite entry if there is a matching entry in the NVRAM.
//                  Otherwise increment user index and saves the current entry.
//                   This function is similar to SaveModeDependentSettings but is written for saving
//                   single UserPref into NVRAM
//
// INPUT          : B_Channel - CH_A or CH_B
//                  gmt_OSD_Adjuster_Info * AdjusterInfo - pointer to an structure of particular UserPref
//
// OUTPUT         : None
//
// GLOBALS        : ModeDependentUserIndex		(R/W)
//                  ModeDependent_HUDUserIndex  (R/W)
//                  B_ModeIndex					(R)
//                  B_AutoAdjustCount			(R)
//                  ModeDependentUserPreferences (R)
//                  ModeDependent_HUDUserPreferences (R)
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void SaveModeDependentUserPrefValue(gmt_PHY_CH B_Channel, gmt_OSD_Adjuster_Info * AdjusterInfo)
{
   WORD i;
   BYTE B_Index = stInput[B_Channel].ModeIndex;
   WORD W_Address;
   ModeDependentUserPrefType * UserPrefTypePtr;
   gmt_ModeDependentEntryType Type;
   ModeDependentLookUpType LookUpType;

#ifdef CVT_MODES_SUPPORT
   BYTE TableSize = IsCvtMode(B_Channel) ? MAX_CVT_MODES_SAVED : MAX_USER_MODES_SAVED;
#else
   BYTE TableSize = MAX_USER_MODES_SAVED;
#endif

   if (B_Index == 0xFF)
      return;

   LookUpType.W_Hfreq = stInput[B_Channel].Hfrq;
   LookUpType.W_Vfreq = stInput[B_Channel].Vfrq;
   LookUpType.W_Vtotal = stInput[B_Channel].Vtotal;

   UserPrefTypePtr= &ModeDependentUserPreferences;

   msg_i("Parent index: %d", B_Index);

   // If not preset mode we need to update the user modes.
#ifdef CVT_MODES_SUPPORT
   if (IsCvtMode(B_Channel) || !IsPresetMode(B_Channel))
#else
   if (!IsPresetMode(B_Channel))
#endif
   {
      for (i = 0; i < TableSize; i++)
      {
         if (CheckModeDependentEntry(i, B_Channel, &LookUpType) != FALSE)
         {
            break;
         }
      }

      // If we didn't find a match then we must write a new value to the user
      // mode table otherwise we do nothing.

      if (i == TableSize)
      {
#ifdef CVT_MODES_SUPPORT
         if (IsCvtMode(B_Channel))
         {
            // Note: Cyclic update scheme is used when the table is full.
            B_Index = ModeDependentCvtIndex;

            msg_i("ModeDependentCvtIndex = %d", ModeDependentCvtIndex);

            ModeDependentCvtIndex++;
            ModeDependentCvtIndex %= MAX_CVT_MODES_SAVED;
         }
         else
#endif
         {
            // Note: Cyclic update scheme is used when the table is full.
            B_Index = ModeDependentUserIndex;

            msg_i("ModeDependentUserIndex = %d", ModeDependentUserIndex);

            ModeDependentUserIndex++;
            ModeDependentUserIndex %= MAX_USER_MODES_SAVED;
         }
         msg_i("new saved mode lookup index = %d", B_Index);
         stInput[B_Channel].ModeIndex = B_Index;
         UpdateModeDependentDescr();
      }
      else
      {
         msg_i("old saved mode lookup index = %d", B_Index);
         B_Index = i;
      }
      W_Address = GetStartAddressOfEntry(B_Index, gmd_LookupEntry);
#ifdef CVT_MODES_SUPPORT
      if (IsCvtMode(B_Channel))
      {
         W_Address = GetStartAddressOfEntry(B_Index, gmd_CvtLookupEntry);
      }
#endif
      SaveModeDependentLookUpEntry(W_Address, &LookUpType);

      W_Address = GetStartAddressOfEntry(B_Index, gmd_UserEntry);
#ifdef CVT_MODES_SUPPORT
      if (IsCvtMode(B_Channel))
      {
         W_Address = GetStartAddressOfEntry(B_Index, gmd_CvtEntry);
      }
#endif
      if (i == TableSize)
      {
         SaveModeDependentEntry(W_Address, UserPrefTypePtr, B_AutoAdjustCount[B_Channel]);
      }
      else
      {
         SaveModeDependentUserPrefEntry(W_Address, UserPrefTypePtr, B_AutoAdjustCount[B_Channel], AdjusterInfo);
      }
   }
   else
   {
      W_Address = GetStartAddressOfEntry(B_Index, gmd_PresetEntry);
      SaveModeDependentUserPrefEntry(W_Address, UserPrefTypePtr, B_AutoAdjustCount[B_Channel], AdjusterInfo);
   }
}

//******************************************************************************
//
// FUNCTION       : void RetrieveGraphicsSettings(gmt_PHY_CH B_Channel,
//                  										ModeDependentLookUpType * LookUpTypePtr)
//
// USAGE          : Searches NVRAM for matching mode.  If mode is found uses
//                  timing parameters saved in NVRAM to display the input mode.
//                  If mode isn't found the new mode's timing parameters are
//                  saved in the NVRAM.
//
// INPUT          : B_Channel = CH_A or CH_B
//                     Note : The value of CH_A or CH_B is stored in
//                     gmvb_MainChannel and gmvb_PipChannel
//                  LookUpTypePtr - pointer to an input data structure
//
// OUTPUT         : None
//
// GLOBALS        : B_ModeIndex			(R)
//                  B_AutoAdjustCount	(W)
//                  UserPrefHTotal		(R/W)
//                  gmvw_InputHTotal	(R)
//                  ModeDependentUserPreferences	(R/W)
//                  ModeDependent_HUDUserPreferences (R/W)
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void RetrieveGraphicsSettings(gmt_PHY_CH B_Channel,
                              ModeDependentLookUpType * LookUpTypePtr)
{
   BYTE B_Result;			// temporary variable
   BYTE B_TableIndex;		// loop variable
   WORD W_Address, W_LookUpAddress;
   gmt_ModeDependentEntryType Type;
   ModeDependentUserPrefType* UserPrefTypePtr;

   UserPrefTypePtr = &ModeDependentUserPreferences;

#ifdef CVT_MODES_SUPPORT
   if (IsCvtMode(B_Channel))
   {
      for (B_TableIndex=0; B_TableIndex<MAX_CVT_MODES_SAVED; B_TableIndex++)
      {

         // check NVRAM to see whether the input had been saved before.
         B_Result = CheckModeDependentEntry(B_TableIndex, B_Channel, LookUpTypePtr);

         if (B_Result == TRUE)
         {
            break;
         }
      }

      if (B_Result == TRUE)
      {
         // read mode dependent entry and if succesfull,
         // save the index in the input structure (note preferred
         // settings are saved in the UserPreferences structure
         // (mode dependent)
         W_Address = GetStartAddressOfEntry(B_TableIndex, gmd_CvtEntry);
         W_LookUpAddress = GetStartAddressOfEntry(B_TableIndex, gmd_CvtLookupEntry);
         if (ReadModeDependentEntry(W_Address, UserPrefTypePtr, gmd_CvtEntry, W_LookUpAddress))
         {
            B_AutoAdjustCount[B_Channel]= NVRam_ReadByte(W_Address + ModeDependentUserPrefSize);
            stInput[B_Channel].ModeIndex = B_TableIndex;
         }
         else
            B_AutoAdjustCount[B_Channel]= 0;
      }
      else
      {
         //
         // We don't need to concern ourselves with the EndOfLookUpIndex
         // return value from CheckModeDependentLookUpEntry() function.
         // The SaveModeDependentSettings() function will handle it.
         //
         B_AutoAdjustCount[B_Channel] = 0;
         SaveModeDependentSettings(B_Channel, LookUpTypePtr);
      }
   }
   else
#endif
      if (IsPresetMode(B_Channel))
      {  
         // compute the start address of the pointed ModeDependentUserPref
         W_Address = GetStartAddressOfEntry(stInput[B_Channel].ModeIndex, gmd_PresetEntry);

         if (CheckModeDependentPresetEntry(W_Address))
         {
            W_LookUpAddress = GetStartAddressOfEntry(stInput[B_Channel].ModeIndex, gmd_LookupEntry);
            if (ReadModeDependentEntry(W_Address, UserPrefTypePtr, gmd_PresetEntry, W_LookUpAddress))
               B_AutoAdjustCount[B_Channel]= NVRam_ReadByte(W_Address+ModeDependentUserPrefSize);
            else
               B_AutoAdjustCount[B_Channel]= 0;
            
            if (UserPrefHTotal == 0 ||
                  (abs(UserPrefHTotal - stInput[B_Channel].Htotal)) >=
                  stInput[B_Channel].Htotal / 5)
            {
               UserPrefHTotal = stInput[B_Channel].Htotal;
            }
         }
         else
         {
            B_AutoAdjustCount[B_Channel] = 0;

            UserPrefHTotal = stInput[B_Channel].Htotal;
         }
      }
      else
      {
         for (B_TableIndex=0; B_TableIndex<MAX_USER_MODES_SAVED; B_TableIndex++)
         {
            W_Address = GetStartAddressOfEntry(B_TableIndex, gmd_LookupEntry);

            // check NVRAM to see whether the input had been saved before.
            B_Result = CheckModeDependentLookUpEntry(W_Address, LookUpTypePtr);
            if (B_Result == TRUE)
            {
               break;
            }
         }

         if (B_Result == TRUE)
         {
            // read mode dependent entry and if succesfull,
            // save the index in the input structure (note preferred
            // settings are saved in the UserPreferences structure
            // (mode dependent)
            msg_i("User Table Index = %d",B_TableIndex);
            W_Address = GetStartAddressOfEntry(B_TableIndex, gmd_UserEntry);
            W_LookUpAddress = GetStartAddressOfEntry(B_TableIndex, gmd_LookupEntry);
            if (ReadModeDependentEntry(W_Address, UserPrefTypePtr, gmd_UserEntry, W_LookUpAddress))
            {
               B_AutoAdjustCount[B_Channel]= NVRam_ReadByte(W_Address+ModeDependentUserPrefSize);
               stInput[B_Channel].ModeIndex = B_TableIndex;
            }
            else
               B_AutoAdjustCount[B_Channel]= 0;
         }
         else
         {
            //
            // We don't need to concern ourselves with the EndOfLookUpIndex
            // return value from CheckModeDependentLookUpEntry() function.
            // The SaveModeDependentSettings() function will handle it.
            //
            B_AutoAdjustCount[B_Channel] = 0;
            SaveModeDependentSettings(B_Channel, LookUpTypePtr);
         }
      }
}

//******************************************************************************
//
// FUNCTION       : void RetrieveModeDependentUserPrefValue(gmt_PHY_CH B_Channel,
//                  										gmt_OSD_Adjuster_Info * AdjusterInfo)
//
// USAGE          : Searches NVRAM for matching mode.  If mode is found uses
//                  timing parameters saved in NVRAM to display the input mode.
//                  If mode isn't found the new mode's timing parameters are
//                  saved in the NVRAM.  This function is similar to RetrieveGraphicsSettings() but is
//                  written for saving single UserPref into NVRAM
//
// INPUT          : B_Channel = CH_A or CH_B
//                     Note : The value of CH_A or CH_B is stored in
//                     gmvb_MainChannel and gmvb_PipChannel
//                  gmt_OSD_Adjuster_Info * AdjusterInfo - pointer to an structure of particular UserPref
//
// OUTPUT         : None
//
// GLOBALS        : B_ModeIndex			(R)
//                  B_AutoAdjustCount	(W)
//                  UserPrefHTotal		(R/W)
//                  gmvw_InputHTotal	(R)
//                  ModeDependentUserPreferences	(R/W)
//                  ModeDependent_HUDUserPreferences (R/W)
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void RetrieveModeDependentUserPrefValue(gmt_PHY_CH B_Channel, gmt_OSD_Adjuster_Info * AdjusterInfo)
{
   BYTE	B_Result;			// temporary variable
   BYTE	B_TableIndex;		// loop variable
   WORD	W_Address, W_LookUpAddress;
   gmt_ModeDependentEntryType Type;
   ModeDependentLookUpType LookUpType;

   LookUpType.W_Hfreq = stInput[gmvb_MainChannel].Hfrq;
   LookUpType.W_Vfreq = stInput[gmvb_MainChannel].Vfrq;
   LookUpType.W_Vtotal = stInput[gmvb_MainChannel].Vtotal;

#ifdef CVT_MODES_SUPPORT
   if (IsCvtMode(B_Channel))
   {
      for (B_TableIndex=0; B_TableIndex<MAX_CVT_MODES_SAVED; B_TableIndex++)
      {
         // check NVRAM to see whether the input had been saved before.
         B_Result = CheckModeDependentEntry(B_TableIndex, B_Channel, &LookUpType);
         if (B_Result == TRUE)
         {
            break;
         }
      }

      if (B_Result == TRUE)
      {
         // read mode dependent entry and if succesfull,
         // save the index in the input structure (note preferred
         // settings are saved in the UserPreferences structure
         // (mode dependent)
         W_Address = GetStartAddressOfEntry(B_TableIndex, gmd_CvtEntry);
         NVRam_ReadBuffer(W_Address + AdjusterInfo->AdjOffset, AdjusterInfo->CurrentAdjValue, AdjusterInfo->CurrentAdjSize);
      }
   }
   else
#endif
      if (IsPresetMode(B_Channel))
      {
         // compute the start address of the pointed ModeDependentUserPref
         W_Address = GetStartAddressOfEntry(stInput[B_Channel].ModeIndex, gmd_PresetEntry);
         NVRam_ReadBuffer(W_Address + AdjusterInfo->AdjOffset, AdjusterInfo->CurrentAdjValue, AdjusterInfo->CurrentAdjSize);
      }
      else
      {
         for (B_TableIndex=0; B_TableIndex<MAX_USER_MODES_SAVED; B_TableIndex++)
         {
            W_Address = GetStartAddressOfEntry(B_TableIndex, gmd_LookupEntry);

            // check NVRAM to see whether the input had been saved before.
            B_Result = CheckModeDependentLookUpEntry(W_Address, &LookUpType);
            if (B_Result == TRUE)
            {
               break;
            }
         }

         if (B_Result == TRUE)
         {
            // read mode dependent entry and if succesfull,
            // save the index in the input structure (note preferred
            // settings are saved in the UserPreferences structure
            // (mode dependent)
            W_Address = GetStartAddressOfEntry(B_TableIndex, gmd_UserEntry);
            NVRam_ReadBuffer(W_Address + AdjusterInfo->AdjOffset, AdjusterInfo->CurrentAdjValue, AdjusterInfo->CurrentAdjSize);
         }
      }
}
//******************************************************************************
//
// FUNCTION       : static void SaveModeDependentLookUpEntry(WORD W_Address,
//                  										ModeDependentLookUpType * LookUpTypePtr)
//
// USAGE          : Save the input timing tags to look-up table in NVRAM
//
// INPUT          : W_Address - starting address of the lookup entry
//                  LookUpTypePtr - pointer to an input data structure
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
static void SaveModeDependentLookUpEntry(WORD W_Address,
      ModeDependentLookUpType * LookUpTypePtr)
{
   BYTE B_Checksum;

// TODO: Need to review
// Don't need to look at Hperiod, we do not use it in application so no global variable is declared for it
//	St_tmp.W_Hperiod    = gmvw_InputHTotal[B_Channel];
//	St_tmp.SyncPolarity = gmvw_InputFlags[B_Channel] & SyncPolarityMask;

   // Calculate mode lookup checksum.
   B_Checksum = CalculateChecksum(LookUpTypePtr, ModeDependentLookUpSize);

   NVRam_WriteBuffer(W_Address, (BYTE *)LookUpTypePtr, ModeDependentLookUpSize);

   // update mode lookup checksum
   NVRam_WriteByte(W_Address + ModeDependentLookUpSize, B_Checksum);

   msgx_i("Saved checksum %d", B_Checksum);
}

//******************************************************************************
//
// FUNCTION       : static void SaveModeDependentEntry(WORD W_Address,
//                  								ModeDependentUserPrefType * UserPrefTypePtr,
//                  								BYTE B_AutoAdjustCount)
//
// USAGE          : Writes the ModeDependentUserPreferences record, auto-adjust count
//                  and checksum in NVRAM at the pointed address.
//
// INPUT          : W_Address - starting address of the mode dependent entry to be
//                     overwritten
//                  UserPrefTypePtr - userpref structure to be written to NVRAM
//                  B_AutoAdjustCount - autoadjust value to be written to NVRAM
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
static void SaveModeDependentEntry(WORD W_Address,
                                   ModeDependentUserPrefType * UserPrefTypePtr,
                                   BYTE B_AutoAdjustCount)
{
   BYTE B_checksum;	// holds the computed checksum
   gmt_ModeDependentEntryType Type;
	
#ifdef DDCCI_VALUE_CHANGE 
	BYTE B_CheckSumTemp;
	WORD W_CheckSumAdd = W_Address + ModeDependentUserPrefSize + sizeof(BYTE);	// sizeof(BYTE) is the size for save variable B_AutoAdjustCount
	NVRam_ReadBuffer(W_CheckSumAdd, (BYTE *)(&B_CheckSumTemp), 1);
#endif

   // write out the graphics port settings
   NVRam_WriteBuffer(W_Address, (BYTE *)UserPrefTypePtr, ModeDependentUserPrefSize);

   // calculate mode independent checksum
   B_checksum = CalculateChecksum(UserPrefTypePtr, ModeDependentUserPrefSize);

   // update Auto Adjust Count variable
   NVRam_WriteByte(W_Address + ModeDependentUserPrefSize, B_AutoAdjustCount);

   // update mode dependent checksum
   msgx_i("checksum address write 0x%X",W_Address + ModeDependentUserPrefSize + sizeof(BYTE));

   NVRam_WriteByte(W_Address + ModeDependentUserPrefSize + sizeof(BYTE), B_checksum);

#ifdef DDCCI_VALUE_CHANGE
	if(B_CheckSumTemp != B_checksum)
	{
		B_DDCCINewControlVal = 1;
	}
#endif

	msg_i("Checksum saved %d", B_checksum);
	msg_i("Address of: 0x%x",W_Address);
}

//******************************************************************************
//
// FUNCTION       : static void SaveModeDependentUserPrefEntry(WORD W_Address,
//                  								ModeDependentUserPrefType * UserPrefTypePtr,
//                  								BYTE B_AutoAdjustCount)
//
// USAGE          : Writes the ModeDependentUserPreferences record, auto-adjust count
//                  and checksum in NVRAM at the pointed address.
//
// INPUT          : W_Address - starting address of the mode dependent entry to be
//                     overwritten
//                  UserPrefTypePtr - userpref structure to be written to NVRAM
//                  B_AutoAdjustCount - autoadjust value to be written to NVRAM
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

static void SaveModeDependentUserPrefEntry(WORD W_Address,
      ModeDependentUserPrefType * UserPrefTypePtr,
      BYTE B_AutoAdjustCount,
      gmt_OSD_Adjuster_Info * AdjusterInfo)
{
   BYTE B_checksum;	// holds the computed checksum
   ModeIndependentUserPrefType LocalCopy;

   NVRam_WriteBuffer(W_Address + AdjusterInfo->AdjOffset,
                     AdjusterInfo->CurrentAdjValue,
                     AdjusterInfo->CurrentAdjSize);

   NVRam_ReadBuffer(W_Address,
                    (BYTE *)&LocalCopy,
                    ModeDependentUserPrefSize);

   // calculate mode independent checksum
   B_checksum = CalculateChecksum(UserPrefTypePtr, ModeDependentUserPrefSize);

   // update Auto Adjust Count variable
   NVRam_WriteByte(W_Address + ModeDependentUserPrefSize, B_AutoAdjustCount);

   // update mode dependent checksum
   msgx_i("checksum address write 0x%x",W_Address + ModeDependentUserPrefSize + sizeof(BYTE));

   NVRam_WriteByte(W_Address + ModeDependentUserPrefSize + sizeof(BYTE), B_checksum);

   msgx_i("Checksum: 0x%X saved", B_checksum);
   msgx_i("Address of: 0x%x",W_Address);
}

//******************************************************************************
//
// FUNCTION       : static void ClearModeDependentLookUpEntry(WORD W_Address)
//
// USAGE          : Fill with zeroes the timing tags in the pointed
//                  ModeDependentEntry array in NVRAM.
//
// INPUT          : W_Address - starting address of the lookup entry to be cleared
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
static void ClearModeDependentLookUpEntry(WORD W_Address)
{
   ModeDependentLookUpType	Stp_tmp;	// RAM copy of the NVRAM entry

   // clear the local copy
   memset(&Stp_tmp, 0, sizeof(ModeDependentLookUpType));

   // write the zeroed record in NVRAM
   NVRam_WriteBuffer(W_Address, (BYTE *)&Stp_tmp, ModeDependentLookUpSize);

   // We leave the invalid checksum in, it will get overwritten and make sure
   // that this entry is not used.
}


//******************************************************************************
//
// FUNCTION       : static BYTE CheckModeDependentEntry(BYTE B_TableIndex,
//              										gmt_PHY_CH B_Channel,
//														ModeDependentLookUpType * LookUpTypePtr
//
// USAGE          : Checks if the entry stored in the NVRAM. Checks for CVT modes
//					extra HTotal for excluding overlapped modes.
//                  Returns TRUE if mode saved in the NVRAM.
//					Returns FALSE otherwise.
//
// INPUT          : B_TableIndex - index in NVRAM
//					B_Channel = CH_A or CH_B
//                  LookUpTypePtr - pointer to an input data structure for
//                     comparison with the saved data in NVRAM
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
static BYTE CheckModeDependentEntry(BYTE B_TableIndex, gmt_PHY_CH B_Channel,
                                    ModeDependentLookUpType * LookUpTypePtr)
{
   WORD	W_Address, W_LookUpAddress;

#ifdef CVT_MODES_SUPPORT

   ModeDependentUserPrefType UserPrefTypePtr;

   if (IsCvtMode(B_Channel))
   {
      W_Address = GetStartAddressOfEntry(B_TableIndex, gmd_CvtLookupEntry);

      if (CheckModeDependentLookUpEntry(W_Address, LookUpTypePtr) == TRUE)
      {
         W_LookUpAddress = W_Address;
         W_Address = GetStartAddressOfEntry(B_TableIndex, gmd_CvtEntry);

         if (ReadModeDependentEntry(W_Address, (ModeDependentUserPrefType *)&UserPrefTypePtr, gmd_CvtEntry, W_LookUpAddress)
            && ((abs(UserPrefTypePtr.HTotal - stInput[B_Channel].Htotal)) <= SavedHperiodDelta ))
         {
            return TRUE;
         }
      }
   }
#endif

   if (IsEstimatedMode(B_Channel))
   {
      W_Address = GetStartAddressOfEntry(B_TableIndex, gmd_LookupEntry);

      // check NVRAM to see whether the input had been saved before.
      return CheckModeDependentLookUpEntry(W_Address, LookUpTypePtr);
   }

   return FALSE;
}

#endif // FEATURE_AIP == ENABLE

/*********************************  END  **************************************/
