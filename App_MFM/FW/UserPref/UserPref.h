/*
	$Workfile:   UserPref.h  $
	$Revision: 1.8 $
	$Date: 2012/09/04 10:08:03 $
*/
#ifndef __USERPREF_H__
#define __USERPREF_H__
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
// MODULE:      userpref.H
//
// USAGE:       This module contains functions and definitions for the user
//				preferences saved in NVRAM.
//				Also includes the NVRAM map.
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#define WRITE_MONITOR_SERIAL_NUMBER
#define NVRAM_RCOSC_TRIM_SIZE 1
#define NVRAM_RCOSC_TRIM_ADDR (WORD)(EH_MonitorSerialNumberStart - 1)

#define B_MONITOR_SERIAL_NUMBER_LENGHT 32 //lenght in bytes of the Monitor Serial Number information in NVRAM.

#define NVRAM_DP_EDID_SIZE   256
#if 1
	#define NVRAM_SIZE 65536UL	// 512K bits
#else
	#define NVRAM_SIZE 16384UL	// 128K bits
#endif

#define NVRAM_DP_EDID_ADDR (WORD)(NVRAM_SIZE - 256)		//0xFF00




//******************************************************************************
//  E X T E R N
//******************************************************************************

extern BYTE B_AutoAdjustCount[NUM_CHANNELS];

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************
#pragma pack(push)
#pragma pack(1)
typedef struct
{
   DWORD DW_MonitorPowerOnTime;
   char Ba_MonitorSerialNumberWriteBuffer[B_MONITOR_SERIAL_NUMBER_LENGHT];
} gmt_MonitorInfo;
#pragma pack(pop)

//
// NVRAM blocks Size definition
//
#define ModeDependentLookUpSize 		sizeof(ModeDependentLookUpType)
#define ChecksumSize					sizeof(BYTE)
#define MPipChannelInfoSize 				0	// For multipip favorites but not sure 
// if it will be implemented changed
// from 32 to 0

//
// Boundary modes definitions
//
#define NonSavedModeLookUpIndex     0xFF
#define EndOfLookUpIndex            0xFF
#define NonStandardModeIndex        0xFF

//
// The maximum delta when comparing with mode saved in NVRAM.
// Normally, these deltas are smaller then ones for standard modes.
//
#define SavedHFreqDelta					5	// 0.5KHz
#define SavedVFreqDelta					5	// 0.5Hz
#define SavedVTotalDelta				5	// 5 lines
#define SavedHperiodDelta				5	// 5 lines

//
// Number of ADC setting (graphic, SD/HD, Composite-NTSC, Composite-PAL, SIF)
//
enum
{
   ADC_SETTING_GRAPHIC,
   ADC_SETTING_HD,
   ADC_SETTING_HD2,
   NumberOfAdcSetting
};


//******************************************************************************
// N V R A M address map 
//******************************************************************************

#ifdef DELL_U2410F	// Modified by ST_hansPENG for DELL NVRam remapping

#if NVRAM_SIZE>4096
//
// ADC calibration
//
#define AdcSettingsStart				0
#define AdcSettingsChecksumStart		(WORD)(AdcSettingsStart + (BandgapSettingSize + ChecksumSize) +\
												((AdcSettingsSize + ChecksumSize) * (NumberOfAdcSetting)))
#define AdcSettingEnd					(WORD)(AdcSettingsChecksumStart + ChecksumSize)	

//
// Monitor Power On Time information
//
#define MonitorPowerOnTimeStart			(WORD)AdcSettingEnd
#define MonitorPowerOnTimeEnd			(WORD)(MonitorPowerOnTimeStart + sizeof(DWORD))
#define MonitorChecksumStart			(WORD)MonitorPowerOnTimeEnd
#define MonitorChecksumEnd				(WORD)(MonitorChecksumStart + ChecksumSize)

//
// Port dependent setting
//
#define	PortSchemeDependentIndexStart	(WORD)MonitorChecksumEnd
#define	PortSchemeDependentEntryStart	(WORD)(PortSchemeDependentIndexStart + sizeof(BYTE))
#define	PortSchemeDependentEnd			(WORD)(PortSchemeDependentEntryStart + \
										((PortSchemeDependentUserPrefSize + ChecksumSize) * NUM_OF_IMAGE_SCHEMES_ENTRIES))
//
// Preset Mode Setting
//
#define	PresetModeSettingIndexStart 	(WORD)PortSchemeDependentEnd
#define	PresetModeSettingStart			(WORD)(PresetModeSettingIndexStart + sizeof(BYTE))
#define	PresetModeSettingEnd			(WORD)(PresetModeSettingStart + \
										((PresetModesUserPrefSize + ChecksumSize) * NUM_OF_IMAGE_SCHEMES_ENTRIES))
//
// Mode Independent setting
//
#define	ModeIndependentStart			(WORD)PresetModeSettingEnd
#define	ModeIndependentChecksumStart	(WORD)(ModeIndependentStart + ModeIndependentUserPrefSize)
#define	ModeIndependentEnd				(WORD)(ModeIndependentChecksumStart + ChecksumSize)

//
// Mode dependent setting
//
#define	ModeDependentUserIndexStart		(WORD)ModeIndependentEnd
#if 1//def NVRAM_PRESET_MODES_SUPPORT
#define	ModeDependentPresetEntryStart	(WORD)(ModeDependentUserIndexStart + sizeof(BYTE))
#define	ModeDependentUserLookUpStart	(WORD)(ModeDependentPresetEntryStart + ((ModeDependentUserPrefSize + sizeof(BYTE)+ ChecksumSize) *\
											NUM_GRAPHICS_PRESETMODE))
#else
#define	ModeDependentUserLookUpStart	(WORD)(ModeDependentUserIndexStart + sizeof(BYTE))
#endif
#define	ModeDependentUserEntryStart		(WORD)(ModeDependentUserLookUpStart + ((ModeDependentLookUpSize + ChecksumSize) * MAX_USER_MODES_SAVED))

#ifdef CVT_MODES_SUPPORT
#define	ModeDependentCvtIndexStart		(WORD)(ModeDependentUserEntryStart + (ModeDependentUserPrefSize + sizeof(BYTE) + ChecksumSize) * MAX_USER_MODES_SAVED)
#define	ModeDependentCvtLookUpStart		(WORD)(ModeDependentCvtIndexStart + 1)
#define	ModeDependentCvtEntryStart		(WORD)(ModeDependentCvtLookUpStart + (ModeDependentLookUpSize + ChecksumSize) * MAX_CVT_MODES_SAVED)
#define	ModeDependentEnd				(WORD)(ModeDependentCvtEntryStart + (ModeDependentUserPrefSize + sizeof(BYTE) + ChecksumSize) * MAX_CVT_MODES_SAVED)
#else
#define	ModeDependentEnd				(WORD)(ModeDependentUserEntryStart + (ModeDependentUserPrefSize + sizeof(BYTE) + ChecksumSize) * MAX_USER_MODES_SAVED)
#endif
	
//
// Mode Dependent Settings PIP
//

//just use the same NVRAM area for both channels
#define ModeDependent_PIPUserIndexStart		(WORD)ModeDependentUserIndexStart
#if 1//def NVRAM_PRESET_MODES_SUPPORT
#define	ModeDependent_PIPPresetEntryStart	(WORD)ModeDependentPresetEntryStart
#endif
#define	ModeDependent_PIPUserLookUpStart	(WORD)ModeDependentUserLookUpStart
#define	ModeDependent_PIPUserEntryStart		(WORD)ModeDependentUserEntryStart
#define	ModeDependent_PIPEnd				(WORD)ModeDependentEnd

//
// AutoScanInput default VIDEO Input setting
//
#define	AutoScanInputStart				(WORD)ModeDependent_PIPEnd
#ifdef AUTOSCANINPUT_NUM_RINGS
#define	AutoScanInputEnd				(WORD)(AutoScanInputStart + sizeof(BYTE) * 2 * AUTOSCANINPUT_NUM_RINGS) // num of rings * 2 channels
#else
#define	AutoScanInputEnd				(WORD)AutoScanInputStart
#endif

//
// Monitor Serial Number information
//
#define	MonitorSerialNumberStart		(WORD)AutoScanInputEnd
#ifdef WRITE_MONITOR_SERIAL_NUMBER
	#if 1//// 120803 Wistron set specific address in NVRAM
	#define MonitorSerialNumberEnd			(WORD)MonitorSerialNumberStart
	#else
	#define	MonitorSerialNumberEnd			(WORD)(MonitorSerialNumberStart + B_MONITOR_SERIAL_NUMBER_LENGHT)//bytes used to store Monitor Serial Number information into NVRAM.
	#endif
#else
#define	MonitorSerialNumberEnd			(WORD)MonitorSerialNumberStart
#endif

//
// Factory Setting
//
#if 1//20120803 Add by KevinChen to fixed factory address
#define FactorySettingStart           0x1000
#else
#define FactorySettingStart             (WORD)MonitorSerialNumberEnd
#endif
#define FactorySettingChecksumStart		(WORD)(FactorySettingStart + FactoryUserPrefSize)
#define	FactorySettingEnd				(WORD)(FactorySettingChecksumStart + ChecksumSize)


//
// Service Setting
//
#define ServiceSettingStart 			(WORD)FactorySettingEnd
#define ServiceSettingChecksumStart 	(WORD)(ServiceSettingStart + ServiceBlockUserPrefSize)
#define ServiceSettingEnd				(WORD)(ServiceSettingChecksumStart + ChecksumSize)

//For color quick match function 512 bytes
#define F_Color_Quick_Match_StartAddr   (WORD) 0x1E00       
#define F_Color_Quick_Match_EndAddr     (WORD) 0x1FFF       

#if MonitorSerialNumberEnd > FactorySettingStart
  #error NVRAM overlap with FactorySettingStart!
#endif

//
// Color Gamut 
//
//C1- 0x2000-0x3FFF (8K)
///////////////////////////////////////////////////////////////////////////////////
// 0x2004 Register 3x3
// 0x2022 Gamma_Lut2 Gamma_POST_Steep registers
// 0x203C Post Lut R
// 0x243C Post Lut G
// 0x283C Post Lut B
// 0x2C3C Gamma_Lut1,Gamma_PRE_Steep registers
// 0x2C56 Pre Lut R
// 0x3056 Pre Lut G
// 0x3456 Pre Lut B
// 0x3856 Post Steep R
// 0x3956 Post Steep G
// 0x3A56 Post Steep B
// 0x3B56 Pre Steep R
//////////////////////////////////////////////////////////////////////////////////
//C2- 0x4000
//C3- 0x6000
//C4- 0x8000
#define C_GAMMA_COMPONENT_SAVED_START_OFFSET	0x2000
#define C_GAMMA_COMPONENT_SAVED_SIZE 			4
#define C_GAMMA_REGISTER_START_OFFSET 			(C_GAMMA_COMPONENT_SAVED_START_OFFSET+C_GAMMA_COMPONENT_SAVED_SIZE)
#define C_GAMMA_REGISTER_SIZE 					30 // 82 // 15 registers
#define C_POST_LUT_CONFIG_REG_START_OFFSET 		(C_GAMMA_REGISTER_START_OFFSET+C_GAMMA_REGISTER_SIZE)
#define C_POST_LUT_CONFIG_REG_SIZE 				26 // 1 + 12 registers
#define C_POST_LUT_START_OFFSET 				(C_POST_LUT_CONFIG_REG_START_OFFSET+C_POST_LUT_CONFIG_REG_SIZE) // (GAMMA_REGISTER_START_OFFSET+GAMMA_REGISTER_SIZE)
#define C_POST_LUT_SIZE 						1024*3
#define C_PRE_LUT_CONFIG_REG_START_OFFSET 		(C_POST_LUT_START_OFFSET+C_POST_LUT_SIZE) // (GAMMA_REGISTER_START_OFFSET+GAMMA_REGISTER_SIZE)
#define C_PRE_LUT_CONFIG_REG_SIZE 				26 // 1 + 12 registers
#define C_PRE_LUT_START_OFFSET 					(C_PRE_LUT_CONFIG_REG_START_OFFSET+C_PRE_LUT_CONFIG_REG_SIZE)
#define C_PRE_LUT_SIZE 							1024*3
#define C_POST_STEEP_START_OFFSET 				(C_PRE_LUT_START_OFFSET+C_PRE_LUT_SIZE)
#define C_POST_STEEP_SIZE 						256*3
#define C_PRE_STEEP_START_OFFSET 				(C_POST_STEEP_START_OFFSET+C_POST_STEEP_SIZE)

#define C_ONE_CHANNEL_GAMMA_SIZE				1024
#define C_ONE_CHANNEL_GAMMA_STEEP_SIZE			256

//
//Uniformity
//
#define UniformityRegisterSettingStart	(WORD) 0xA000	//
#define UniformityIndexODDStart			(WORD) 0xA100	//0xB100-0xC0FF 4Kbyte
#define UniformityIndexEvenStart		(WORD) 0xB100	//0xA100-0xB0FF 4Kbyte
#define	Uniformity_SIZE					8192
#define UniformityEnd					(WORD) (UniformityIndexODDStart + Uniformity_SIZE)

//Uniformity2
#define UniformityRegisterSettingStart2	(WORD) 0xD000	//
#define UniformityIndexEvenStart2		(WORD) 0xD100	//0xD100-0xE0FF 4Kbyte
#define UniformityIndexODDStart2		(WORD) 0xE100	//0xE100-0xF0FF 4Kbyte
#define	Uniformity_SIZE					8192
#define UniformityEnd2					(WORD) (UniformityIndexODDStart2 + Uniformity_SIZE)


//NVRAM table check type
enum NVRAM_Table_Type
{
	NVRAM_Table_Gamma = 0,
	NVRAM_Table_DUM,
};
////////////////////////////////////////////////
#if 1//
	//Save RC OSC

	#define  RcOscTrimStart           		(WORD)NVRAM_RCOSC_TRIM_ADDR
	#define  RcOscTrimEnd              		(WORD)NVRAM_RCOSC_TRIM_ADDR + NVRAM_RCOSC_TRIM_SIZE
	#define EH_MonitorSerialNumberStart      (WORD)(NVRAM_DP_EDID_ADDR-32)
	
#if EH_MonitorSerialNumberStart < UniformityEnd2
  #error NVRAM overlap with UniformityEnd2!
	#endif
#endif
////////////////////////////////////////////////

//
//DP EDID
//
#define	 NVRamEnd						(WORD)UniformityEnd
#define  DpEdidTableStart          		(WORD)NVRAM_DP_EDID_ADDR
#define  DpEdidTableEnd            		(WORD)NVRAM_DP_EDID_ADDR + NVRAM_DP_EDID_SIZE


#if ServiceSettingEnd > C_GAMMA_COMPONENT_SAVED_START_OFFSET
  #error NVRAM overlap with C_GAMMA_COMPONENT_SAVED_START_OFFSET!
#endif

#else

enum NVRAM_MAP
{
	//
	// ADC calibration
	//
	AdcSettingsStart 		 	= 0,
	AdcSettingsChecksumStart 	= AdcSettingsStart + (BandgapSettingSize + ChecksumSize) + ((AdcSettingsSize + ChecksumSize) * 
									(NumberOfAdcSetting
									)),
	AdcSettingEnd 			 	= AdcSettingsChecksumStart + ChecksumSize,

	//
	// Monitor Power On Time information
	//
	MonitorPowerOnTimeStart 	= AdcSettingEnd,
	MonitorPowerOnTimeEnd		= MonitorPowerOnTimeStart + sizeof(DWORD), // (DWORD)->4 bytes are used to store Monitor Power On Time information in NVRAM.
	
	MonitorChecksumStart		= MonitorPowerOnTimeEnd,
	MonitorChecksumEnd			= MonitorChecksumStart + ChecksumSize,

	//
	// Factory Setting
	//
	FactorySettingStart				= MonitorChecksumEnd,
	FactorySettingChecksumStart		= FactorySettingStart + FactoryUserPrefSize,
	FactorySettingEnd				= FactorySettingChecksumStart + ChecksumSize,

	//
	// Service Setting
	//
	ServiceSettingStart				= FactorySettingEnd,
	ServiceSettingChecksumStart		= ServiceSettingStart + ServiceBlockUserPrefSize,
	ServiceSettingEnd				= ServiceSettingChecksumStart + ChecksumSize,

	//
	// Port dependent setting
	//
	PortSchemeDependentIndexStart	= ServiceSettingEnd,
	PortSchemeDependentEntryStart	= PortSchemeDependentIndexStart + sizeof(BYTE),
	PortSchemeDependentEnd		= PortSchemeDependentEntryStart + ((PortSchemeDependentUserPrefSize + ChecksumSize) * NUM_OF_IMAGE_SCHEMES_ENTRIES),	// PSD

	//
	// Preset Mode Setting
	//
	PresetModeSettingIndexStart	= PortSchemeDependentEnd,
	PresetModeSettingStart		= PresetModeSettingIndexStart + sizeof(BYTE),
	PresetModeSettingEnd		= PresetModeSettingStart + ((PresetModesUserPrefSize + ChecksumSize) * NUM_OF_IMAGE_SCHEMES_ENTRIES),

	//
	// Mode Independent setting
	//
	ModeIndependentStart			= PresetModeSettingEnd,
	ModeIndependentChecksumStart	= ModeIndependentStart + ModeIndependentUserPrefSize,
	ModeIndependentEnd			= ModeIndependentChecksumStart + ChecksumSize,

	//
	// Mode dependent setting
	//
	ModeDependentUserIndexStart	= ModeIndependentEnd,
#if 1//def NVRAM_PRESET_MODES_SUPPORT
	ModeDependentPresetEntryStart	= ModeDependentUserIndexStart + sizeof(BYTE),
	ModeDependentUserLookUpStart	= ModeDependentPresetEntryStart + ((ModeDependentUserPrefSize + sizeof(BYTE)+ ChecksumSize) * NUM_GRAPHICS_PRESETMODE),
#else
	ModeDependentUserLookUpStart	= ModeDependentUserIndexStart + sizeof(BYTE),
#endif
	ModeDependentUserEntryStart	= ModeDependentUserLookUpStart + ((ModeDependentLookUpSize + ChecksumSize) * MAX_USER_MODES_SAVED),

#if 1//def NVRAM_CVT_MODES_SUPPORT
	ModeDependentCvtIndexStart		= ModeDependentUserEntryStart + (ModeDependentUserPrefSize + sizeof(BYTE) + ChecksumSize) * MAX_USER_MODES_SAVED,
	ModeDependentCvtLookUpStart	= ModeDependentCvtIndexStart + 1,
	ModeDependentCvtEntryStart		= ModeDependentCvtLookUpStart + (ModeDependentLookUpSize + ChecksumSize) * MAX_CVT_MODES_SAVED,
	ModeDependentEnd				= ModeDependentCvtEntryStart + (ModeDependentUserPrefSize + sizeof(BYTE) + ChecksumSize) * MAX_CVT_MODES_SAVED,
#else
	ModeDependentEnd				= ModeDependentUserEntryStart + (ModeDependentUserPrefSize + sizeof(BYTE) + ChecksumSize) * MAX_USER_MODES_SAVED,
#endif

	//
	// Mode Dependent Settings PIP
	//

	//just use the same NVRAM area for both channels
	ModeDependent_PIPUserIndexStart	= ModeDependentUserIndexStart,
#if 1//def NVRAM_PRESET_MODES_SUPPORT
	ModeDependent_PIPPresetEntryStart	= ModeDependentPresetEntryStart,
#endif
	ModeDependent_PIPUserLookUpStart	= ModeDependentUserLookUpStart,
	ModeDependent_PIPUserEntryStart	= ModeDependentUserEntryStart,
	ModeDependent_PIPEnd				= ModeDependentEnd,

	//
	// AutoScanInput default VIDEO Input setting
	//
	AutoScanInputStart	= ModeDependent_PIPEnd,
#ifdef AUTOSCANINPUT_NUM_RINGS
	AutoScanInputEnd	= AutoScanInputStart + sizeof(BYTE) * 2 * AUTOSCANINPUT_NUM_RINGS, // num of rings * 2 channels
#else
	AutoScanInputEnd	= AutoScanInputStart,
#endif

	//
	// Monitor Serial Number information
	//
	MonitorSerialNumberStart	= AutoScanInputEnd,
#ifdef WRITE_MONITOR_SERIAL_NUMBER
	MonitorSerialNumberEnd		= MonitorSerialNumberStart + B_MONITOR_SERIAL_NUMBER_LENGHT,//bytes used to store Monitor Serial Number information into NVRAM.
#else
	MonitorSerialNumberEnd		= MonitorSerialNumberStart,
#endif

	 NVRamEnd	=  MonitorSerialNumberEnd,
        DpEdidTableStart          = NVRAM_DP_EDID_ADDR,
        DpEdidTableEnd            = NVRAM_DP_EDID_ADDR + NVRAM_DP_EDID_SIZE,
};
#endif //end if NVRAM size check

#if MonitorSerialNumberEnd > DpEdidTableStart
  #error NVRAM overlap with DP EDID!
#endif

#if (NVRamEnd >= 65535)
	#error NVRAM usage is beyond hardware limited 64k-byte.
#endif

#else//else orginal firmware

enum NVRAM_MAP
{
#ifdef HDCP_KEY_IN_USERPREF_NVRAM
   //
   // HDCP table
   // Note : HDCP_SIZE define as WORD size
   //
   HdcpBksv        = 0,
   HdcpTableStart  = 5,
   HdcpTableEnd    = HdcpTableStart + (287),
#else
   HdcpTableEnd    = 0,
#endif
   //
   // ADC calibration
   //
   AdcSettingsStart           = HdcpTableEnd,
   AdcSettingsChecksumStart 	= 	AdcSettingsStart + (BandgapSettingSize + ChecksumSize) +
   ((AdcSettingsSize + ChecksumSize)*(NumberOfAdcSetting)),
   AdcSettingEnd              = AdcSettingsChecksumStart + ChecksumSize,

   //
   // Mode dependent setting
   //
   ModeDependentUserIndexStart   = AdcSettingEnd,
   ModeDependentPresetEntryStart = ModeDependentUserIndexStart + 1,
   ModeDependentUserLookUpStart  = ModeDependentPresetEntryStart + ((ModeDependentUserPrefSize +
   //sizeof(B_AutoAdjustCount[gmvb_MainChannel])+
   sizeof(BYTE) + ChecksumSize) * NUM_GRAPHICS_PRESETMODE),
   ModeDependentUserEntryStart   = ModeDependentUserLookUpStart + ((ModeDependentLookUpSize +
   ChecksumSize) * MAX_USER_MODES_SAVED),

#ifdef CVT_MODES_SUPPORT
   ModeDependentCvtIndexStart		= ModeDependentUserEntryStart +
   (ModeDependentUserPrefSize + /*sizeof(B_AutoAdjustCount[gmvb_MainChannel])*/sizeof(BYTE) + ChecksumSize) * MAX_USER_MODES_SAVED,
   ModeDependentCvtLookUpStart   = ModeDependentCvtIndexStart + 1,
   ModeDependentCvtEntryStart	   = ModeDependentCvtLookUpStart + (ModeDependentLookUpSize + ChecksumSize) * MAX_CVT_MODES_SAVED,
   ModeDependentEnd              = ModeDependentCvtEntryStart +
   (ModeDependentUserPrefSize + /*sizeof(B_AutoAdjustCount[gmvb_MainChannel])*/sizeof(BYTE) + ChecksumSize) * MAX_CVT_MODES_SAVED,
#else
   ModeDependentEnd              = ModeDependentUserEntryStart +
   (ModeDependentUserPrefSize + /*sizeof(B_AutoAdjustCount[gmvb_MainChannel])*/sizeof(BYTE) + ChecksumSize) * MAX_USER_MODES_SAVED,
#endif

	//
	// Mode Dependent Settings PIP
	//
	//just use the same NVRAM area for both channels
	ModeDependent_PIPUserIndexStart = ModeDependentUserIndexStart,
	ModeDependent_PIPPresetEntryStart = ModeDependentPresetEntryStart,
	ModeDependent_PIPUserLookUpStart = ModeDependentUserLookUpStart,
	ModeDependent_PIPUserEntryStart = ModeDependentUserEntryStart,
	ModeDependent_PIPEnd = ModeDependentEnd,

	//
	// Port dependent setting
	//
		PortSchemeDependentIndexStart  	= ModeDependent_PIPEnd,
	PortSchemeDependentEntryStart 	= PortSchemeDependentIndexStart + 1,
   PortSchemeDependentEnd  = PortSchemeDependentEntryStart + ((PortSchemeDependentUserPrefSize + ChecksumSize) *
   NUM_OF_IMAGE_SCHEMES_ENTRIES),    // PSD

   ModeIndependentStart         = PortSchemeDependentEnd,
   ModeIndependentChecksumStart = ModeIndependentStart + ModeIndependentUserPrefSize,
   ModeIndependentEnd           = ModeIndependentChecksumStart + ChecksumSize,

   //
   // AutoScanInput default VIDEO Input setting
   //

   AutoScanInputStart  = ModeIndependentEnd,
#ifdef AUTOSCANINPUT_NUM_RINGS
   AutoScanInputEnd    = AutoScanInputStart + sizeof(BYTE) * 2 * AUTOSCANINPUT_NUM_RINGS, // num of rings * 2 channels
#else
   AutoScanInputEnd    = AutoScanInputStart,
#endif

   //
   // Monitor Power On Time information
   //
   MonitorPowerOnTimeStart  = AutoScanInputEnd,
   MonitorPowerOnTimeEnd    = MonitorPowerOnTimeStart + sizeof(DWORD), // (DWORD)->4 bytes are used to store Monitor Power On Time information in NVRAM.

   MonitorChecksumStart	    = MonitorPowerOnTimeEnd,
   MonitorChecksumEnd       = MonitorChecksumStart + ChecksumSize,

   //
   // Monitor Serial Number information
   //
   MonitorSerialNumberStart  = MonitorChecksumEnd,
   MonitorSerialNumberEnd    = MonitorSerialNumberStart + B_MONITOR_SERIAL_NUMBER_LENGHT, //bytes used to store Monitor Serial Number information into NVRAM.

   DpEdidTableStart          = NVRAM_DP_EDID_ADDR,
   DpEdidTableEnd            = NVRAM_DP_EDID_ADDR + NVRAM_DP_EDID_SIZE,
};

#if MonitorSerialNumberEnd > DpEdidTableStart
#error NVRAM overlap with DP EDID!
#endif

#if ( MonitorSerialNumberEnd >= NVRAM_SIZE)
#error NVRAM usage is beyond hardware limited 4k-byte.
#endif

#endif// endif DELL 2410


//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

void InitNvram						(void);
void InitNvram_FactoryBlock		(void);
void NvramCheck						(void);
BYTE CalculateChecksum 				(void const* Bp_Buffer, WORD B_Size);
#ifdef Gamma_DUM_Initial_Check
BYTE CalculateChecksum_far 				(BYTE far* Bp_Buffer, WORD B_Size);
BYTE CalculateChecksum_Word(WORD const far* Bp_Buffer, WORD B_Size);
#endif

void ReadModeIndependentSettings	(void);
void SaveModeIndependentSettings	(void);

void InitModeDependentDescr			(void);
void InitModeDependentLookUp(void);
BOOL CheckModeDependentDescriptor(void);
void SaveModeDependentSettings(gmt_PHY_CH B_Channel, ModeDependentLookUpType * LookUpTypePtr);
void RetrieveGraphicsSettings(gmt_PHY_CH B_Channel, ModeDependentLookUpType * LookUpTypePtr);

BOOL ReadAdcSettings				(gmt_ADC_ID B_AdcId, BYTE *AdcSettings, BYTE B_Port);
BOOL SaveAdcSettings				(gmt_ADC_ID B_AdcId, BYTE *AdcSettings, BYTE B_Port);
BOOL ReadBandgapSetting		(BYTE *BandgapSetting);
BOOL SaveBandgapSetting		(BYTE *BandgapSetting);
BOOL ReadDecoderTbcSettings			(BYTE *AdcSettings, BYTE B_Port);
BOOL SaveDecoderTbcSettings			(BYTE *AdcSettings, BYTE B_Port);

void InitPortSchemeDependentDescr			(void);
void InitPortSchemeDependent				(void);
BOOL CheckPortSchemeDependentEntry		(BYTE B_Index);
BOOL CheckPortSchemeDependentDescriptor	(void);
BOOL ReadPortSchemeDependentEntry(BYTE B_Index);
void SavePortSchemeDependentSettings		(void);

// Functions used for Saving single UserPref into NVRAM solution
void SaveUserPref(gmt_OSD_Adjuster_Info * AdjusterInfo);
void RetrieveUserPref(gmt_OSD_Adjuster_Info * AdjusterInfo);
void SaveModeDependentUserPrefValue(gmt_PHY_CH B_Channel, gmt_OSD_Adjuster_Info * AdjusterInfo);
void RetrieveModeDependentUserPrefValue(gmt_PHY_CH B_Channel, gmt_OSD_Adjuster_Info * AdjusterInfo);
// Function used for reading the Monitor Power On Time from NVRAM.
BOOL ReadMonitorPowerOnTimefromNVRAM(DWORD *pDW_MonitorPowerOnTime);

// Function used for saving the Monitor Power On Time into NVRAM.
void WriteMonitorPowerOnTimeintoNVRAM(DWORD *pDW_MonitorPowerOnTime);


// Function used for reading the Monitor Serial Number from NVRAM.
void ReadMonitorSerialNumberfromNVRAM(BYTE*pBa_MonitorSerialNumberWriteBuffer);


#ifdef WRITE_MONITOR_SERIAL_NUMBER
// This function is used only for testing since the Monitor Serial Number is burned in NVRAM in the manufacturing process.
void WriteMonitorSerialNumberintoNVRAM(BYTE *pBa_MonitorSerialNumberWriteBuffer);
#endif



#endif

void EditChannelList(void);
#ifdef DELL_U2410F 
void ReadFactorySettings(void);
#endif


//********************************  END  **************************************
