/*
    $Workfile:   gm_Input.h  $
    $Revision: 1.27 $
    $Date: 2013/02/22 09:46:15 $
*/
#ifndef __GM_INPUT_H__
#define __GM_INPUT_H__
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
// MODULE :gm_Input.h
//
// USAGE  :Include this to use all the AFE definitions
//
//*******************************************************************************

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************
//
// Standard Color Space
//
typedef enum
{
	CS_RED,
	CS_GREEN,
	CS_BLUE
}gmt_ColorRGB;

typedef enum
{
	CS_V,
	CS_Y,
	CS_U
}gmt_ColorYUV;

//
//Video Decoder Modes
//
#define DEC_NUM_VIDEO_MODES 	7

#define BW_525    128
#define BW_625    129
#define NP_AUTO   254
#define S_AUTO    255

//
//AFE Channel Definitions..
//
typedef enum
{
	AFE_CH1,	//Y-Channel
	AFE_CH2,	//Pr-Channel
	AFE_CH3,	//Pb-Channel
	MAX_N0_OF_AFE_CHNL
}gmt_AFEChannels;

//
//Mux Definitions
//
typedef enum
{
	AFE_AMUX = 0,
	AFE_BMUX,
	AFE_CMUX,	
	MAX_NO_OF_MUX
}gmt_AFE_MUX;

//
//ADC Definitions
//
typedef enum
{
	AFE_ADCA,
	AFE_ADCB,
	AFE_ADCC,
	MAX_NO_OF_ADC
}gmt_ADC;

//
//ADC Band Width
//
typedef enum
{
	ADC_BW_10MHZ = 0,	//Used for SDTV
	ADC_BW_20MHZ,		//Used for 480P Component Input
	ADC_BW_40MHZ,		//Used for 720P/1080i Component
	ADC_BW_FULL			//Used for Graphics and Overlay mode
}gmt_ADC_BW;

//
//DIP Input Selection
//
typedef enum
{
	DIP_16_24_BIT = 0,	//16/24 Bit Processing
	DIP_RESERVED,		//Reserved - Check Reg Spec
	DIP_8BIT,			//8 Bit Processing, enables 656
	DIP_8BIT_SS			//8 Bit Processing, Seperate Sync
}gmt_DIP_Select;

//
//AIP Sync Selection
//
typedef enum
{
	SOG_INT_SOG = 0,	//Internal Analog S0G/S0Y
	SOG_OVL_SYNC,		//Used for Overlay Clamping Mode
	SOG_DEC_SYNC		//Video Decoder Sync
}gmt_SoG_Sync;

//
//Analog Sync Slicer
//
typedef enum
{
	ASE_SS_ST =0,		//Sync Tip Slicer
	ASE_SS_BP			//Back Porch Slicer
}gmt_ASE_SYNCSLICER;

typedef enum
{
	ASE_SLICE_LEVEL_75mV=0,	//+ve for ST and -ve for BP
	ASE_SLICE_LEVEL_87mV,
	ASE_SLICE_LEVEL_100mV,
	ASE_SLICE_LEVEL_112mV,
	ASE_SLICE_LEVEL_125mV,	//+ve for ST and -ve for BP
	ASE_SLICE_LEVEL_137mV,
	ASE_SLICE_LEVEL_150mV,	//+ve for ST and -ve for BP
	ASE_SLICE_LEVEL_162mV,	//+ve for ST and -ve for BP
}gmt_ASE_SS_LEVEL;

typedef enum
{
	VID_HARD_CLAMP_BP = 0,	//Hard Clamp - Back Porch
	VID_HARD_CLAMP_ST,    	//Hard Clamp - Sync Tip
	VID_MID_SCALE_CLAMP,	//Mid Scale Clamping
	VID_CLAMP_LOOP,			//Loop Control using Clamp DAC
}gmt_AIP_VID_CLAMP_MODE;

typedef enum
{
	HCLAMP_DISABLED = 0,	
	HCLAMP_BP,    		//Hard Clamp - Back Porch
	HCLAMP_ST,			//Hard Clamp - Sync Tip
	HCLAMP_GND,			//Hard Clamp - Static
}gmt_AFE_HCLAMP_MODE;

//
// ADC block gain/offset
//
#pragma pack(push)
#pragma pack(1)
typedef struct
{
       BYTE AdcRedOffset1;
       BYTE AdcGreenOffset1;
       BYTE AdcBlueOffset1;
       WORD AdcRedOffset2;
       WORD AdcGreenOffset2;
       WORD AdcBlueOffset2;
       WORD AdcRedGain;
       WORD AdcGreenGain;
       WORD AdcBlueGain;
} gmt_ADC_SETTING;
#pragma pack(pop)
#define AdcSettingsSize             sizeof(gmt_ADC_SETTING)
#define BandgapSettingSize		sizeof(BYTE)

typedef enum
{
   ADC_CAL_NONE,
	ADC_VIDEO_NTSC,
	ADC_VIDEO_PAL,
	ADC_SCART_VGA,
	ADC_SCART_VGA_DAC,
	ADC_GRAPHICS_PIXEL_GRAB,
	ADC_GRAPHICS_MIN_MAX,
	ADC_GRAPHICS_DAC,
	ADC_GRAPHICS_DAC_MIN_MAX,
	ADC_COMPONENT_100_PERCENT,
	ADC_COMPONENT_75_PERCENT,
	ADC_COMPONENT_DAC,
	ADC_VIDEO_NTSC_EXT_SRC,
	ADC_VIDEO_PAL_EXT_SRC,	
	ADC_VIDEO_OVERLAY,	
	ADC_VIDEO_OVERLAY_DAC,
	ADC_SIF_DAC = BIT11,
	ADC_DAC_RESISTIVE_LADDER = BIT12,
	ADC_VIDEO_OFFSET2 = BIT13,	
	ADC_VIDEO_SINGLE_ADC = BIT14,
} gmt_ADC_CALIBRATION_INPUT;

// used in gm_OverlayEnable.c
typedef enum
{
	SCART_OVERLAY_DISABLE =	0,
	SCART_OVERLAY_ENABLE =	1,
	SCART_OVERLAY_FORCED =	2,	
}gmt_SCART_OVERLAY_TYPE;
//
// Decoder block gain bias
//
typedef struct
{
	BYTE DecoderTbcYBias;
	WORD DecoderTbcYGain;
	WORD DecoderTbcCGain;	
} gmt_DecoderTbcSettingsType;

#define DecoderTbcSettingsSize             sizeof(gmt_DecoderTbcSettingsType)

//
// Port selection modes
//
typedef enum
{
	NORMAL_MODE			= BIT0,
	SMART_SWAP_MODE	= BIT1,
	CLONE_CH_MODE		= BIT2,
} gmt_PortSelectMode;

//******************************************************************************
//  G L O B A L    M A C R O
//******************************************************************************
//
// NOTE : For internal AFE only
// Get current input port processing type “gmt_InputProcessing” for the channel 
// (CH_A or CH_B)
//		INTERNAL = internal data path (e.g. internal decoder)
//  	EXTERNAL = external data path (e.g. external decoder)
//
#define AfeGetChannelProcType(Channel) \
	(gmvs_InputConfigTblPtr[gmvb_InputConnector[Channel]].B_InputProcessing)

//
// NOTE : For internal AFE only
// Get current input port processing type “gmt_InputProcessingPort” for the 
// channel (CH_A or CH_B)
//		AIP = Analog port
//  	DIP = digital port
//
#define AfeGetChannelPortProc(Channel) \
	(gmvs_InputConfigTblPtr[gmvb_InputConnector[Channel]].B_InputProcessingPort)
	
//
// NOTE : For internal AFE only
// Get current input connector name “gmt_UserDefConnNames” for the channel 
// (CH_A or CH_B)
//  	RF1, RF2, CVBS1, CVBS2, CVBS3, SVIDEO1, SVIDEO2, 
//  	COMP1, COMP2, COMP3, VGA1, VGA2, SCART1
//
#define AfeGetChannelConnName(Channel) \
	(gmvs_InputConfigTblPtr[gmvb_InputConnector[Channel]].B_ConnName)

//
// NOTE : For internal AFE only
// Get current input port connector type “gmt_ConnTypes” for the channel 
// (CH_A or CH_B)
//		COMPOSITE, TUNER, SVIDEO, COMPONENT, VGA, SCART, SCARTT2
//
#define AfeGetChannelConnType(Channel) \
	(gmvs_InputConfigTblPtr[gmvb_InputConnector[Channel]].B_ConnType)
	
//
// Get selected input port processing type “gmt_InputProcessing”
//  	INTERNAL = internal data path (e.g. internal decoder)
//  	EXTERNAL = external data path (e.g. external decoder)
//
#define GetPortProcType(Connector) \
	(gmvs_InputConfigTblPtr[Connector].B_InputProcessing)

//
// Get current input port processing type “gmt_InputProcessingPort” 
//		AIP = Analog port
//  	DIP = digital port
//
#define GetPortProc(Connector) \
	(gmvs_InputConfigTblPtr[Connector].B_InputProcessingPort)
	
//
// Get selected input port connector type “gmt_ConnTypes” 
//  	COMPOSITE, TUNER, SVIDEO, COMPONENT, VGA, SCART, SCARTT2
//
#define GetPortConnType(Connector) \
	(gmvs_InputConfigTblPtr[Connector].B_ConnType)

//
// Get selected input port signal type gmt_AipSignalType 
//  SIG_COMPOSITE SIG_TUNER_COMPOSITE SIG_TUNER_SVIDEO SIG_SVIDEO
//  SIG_COMPONENT SIG_VGA SIG_OVL_TYPE2 SIG_OVL_TYPE3 SIG_OVL_TYPE4
//
#define GetPortSignalType(Connector) \
	(gmvs_InputConfigTblPtr[Connector].B_SignalType)

//
// Get selected input connector name “gmt_UserDefConnNames”
//  	RF1, RF2, CVBS1, CVBS2, CVBS3, SVIDEO1, SVIDEO2, 
//  	COMP1, COMP2, COMP3, VGA1, VGA2, SCART1
//
#define GetPortConnName(Connector) \
	(gmvs_InputConfigTblPtr[Connector].B_ConnName)

//
// Get selected input connector name “gmt_UserDefConnNames”
//  	RF1, RF2, CVBS1, CVBS2, CVBS3, SVIDEO1, SVIDEO2, 
//  	COMP1, COMP2, COMP3, VGA1, VGA2, SCART1
//
#define GetPortPropertyTable(Connector) \
	(gmvs_InputConfigTblPtr[Connector].PropertyTable)
	
//******************************************************************************
//  G L O B A L    D E C L A R A T I O N
//******************************************************************************
extern WORD ROM   gmvw_AFERegAddress[];
extern BYTE ROM   *gmvb_AFERegistersValsPtr;		//
extern BYTE ROM   *gmvb_AdcUsagePtr;				//
extern gmt_InputConfigTable ROM *gmvs_InputConfigTblPtr; 	// AFE table pointer

//
//Video Decoder
//
extern WORD ROM gmvw_VideoConfigRegs[NUM_OF_VIDEO_REGISTERS];
extern BYTE ROM gmvb_VideoConfigTable_NTSC[NUM_OF_VIDEO_REGISTERS];
extern BYTE ROM gmvb_VideoConfigTable_PAL[NUM_OF_VIDEO_REGISTERS];
extern BYTE ROM gmvb_VideoConfigTable_NTSC_PAL[NUM_OF_VIDEO_REGISTERS];
extern BYTE ROM gmvb_VideoConfigTable_SECAM[NUM_OF_VIDEO_REGISTERS];
extern BYTE ROM gmvb_VideoConfigTable_AUTO_SECAM[NUM_OF_VIDEO_REGISTERS];
extern BYTE ROM gmvb_VideoConfigTable_NTSC443[NUM_OF_VIDEO_REGISTERS];
extern BYTE ROM gmvb_VideoConfigTable_PALN[NUM_OF_VIDEO_REGISTERS];
extern BYTE ROM gmvb_VideoConfigTable_PALM[NUM_OF_VIDEO_REGISTERS];
extern BYTE ROM gmvb_VideoConfigTable_PAL60[NUM_OF_VIDEO_REGISTERS];

//******************************************************************************
//  F U N C T I O N   P R O T O T Y P E
//******************************************************************************
//*********************************************
//Function Used By PORT SELECTION
//*********************************************
gmt_RET_PASSFAIL 	gm_InputConfigPort(gmt_PHY_CH B_Channel, BYTE InputConnector);
gmt_RET_STAT		gm_DipSelectClock(gmt_PHY_CH B_Channel, gmt_DipClock B_InputClock);
gmt_RET_STAT		gm_DipSetInputBus(gmt_PHY_CH B_Channel, BYTE B_Input);
gmt_RET_STAT		gm_DipSwapBus( gmt_PHY_CH B_Channel, BYTE B_DIP_1,BYTE B_DIP_2,BYTE B_DIP_3);
gmt_RET_STAT		gm_DipFlipBus(gmt_PHY_CH B_Channel,BYTE B_Flip);
gmt_RET_STAT		gm_DipConfig(gmt_PHY_CH B_Channel, BYTE InputConnector, BYTE PinMap);
void		gm_DipPowerSave(void);
gmt_RET_STAT gmp_DipSetInputConfigType(BYTE B_Source, BYTE B_InputConfigType);

#if ( FEATURE_HDMI == ENABLE )
gmt_RET_STAT		gm_DvipConfig(gmt_PHY_CH B_Channel, BYTE InputConnector);
#endif

#if (FEATURE_DISPLAY_PORT == ENABLE )
gmt_RET_STAT        gm_DprxpConfig(gmt_PHY_CH B_Channel, BYTE InputConnector);
#endif

//******************
//Analog Input Port
//******************
gmt_RET_STAT		gm_AipDecoderDataSelect(gmt_ADC_ID B_AdcId,gmt_AFEChannels B_DFECh,gmt_ADC B_ADC);
gmt_RET_STAT		gm_AipHDGfxPathEnable(gmt_ADC_ID B_AdcId,BYTE B_Enable);
gmt_RET_STAT 		gm_AipSetHDGfxBusSwap(gmt_ADC_ID B_AdcId, BYTE B_ADC_A_Clr,BYTE B_ADC_B_Clr,BYTE B_ADC_C_Clr);
void					gm_AipUpdateAFERegisters(BYTE InputConnector);
gmt_RET_STAT		gm_AipClockSourceSelect(gmt_PHY_CH B_Channel, BYTE InputConnector, gmt_PortSelectMode B_SelectMode);
gmt_RET_STAT		gm_AipConfig(gmt_PHY_CH B_Channel, BYTE InputConnector);
gmt_RET_STAT 		gm_AOCEnable(gmt_ADC_ID B_AdcId, BYTE B_Enable);
//******************
//ADC
//******************
gmt_RET_STAT 		gm_AdcPowerDown(gmt_ADC_ID B_AdcId, gmt_ADC B_ADC); //Will also be used by Power Handler
gmt_RET_STAT 		gm_AdcPowerUp(gmt_ADC_ID B_AdcId, gmt_ADC B_ADC);   //Will also be used by Power Handler

//*********************************************
//Function Used By Mode Detection
//*********************************************
//******************
//Digital Input Port
//******************
void gmx_DipSyncTypeControl(gmt_SYNC_TYPE B_SyncType);
#define gm_DipSyncTypeControl(Channel, SyncType) gmx_DipSyncTypeControl(SyncType)

//******************
//Analog Input Port
//******************
gmt_RET_STAT 		gm_AipEnableDSS(gmt_ADC_ID B_AdcId, BYTE B_Enable);
gmt_RET_STAT 		gm_AipEnableDCS(gmt_ADC_ID B_AdcId, BYTE B_Enable);
gmt_RET_STAT 		gm_AipEnableACS(gmt_ADC_ID B_AdcId, BYTE B_Enable);
gmt_RET_STAT 		gm_AipSelectSoG(gmt_ADC_ID B_AdcId, gmt_SoG_Sync B_Sync);
gmt_RET_STAT 		gm_AseSetSyncSliceLevel (gmt_ADC_ID B_AdcId, gmt_ASE_SYNCSLICER B_Ss, gmt_ASE_SS_LEVEL B_Level);

//*********************************************
//Function Used By Mode Set Up
//*********************************************
//******************
//Digital Input Port
//******************
gmt_RET_STAT		gm_DipProgramCoast(WORD W_CoastStart,WORD W_CoastEnd);
gmt_RET_STAT		gm_DipProgramHClamp(WORD W_ClampStart,WORD W_ClampWidth);
gmt_RET_STAT		gm_DipProgramVClamp(WORD W_ClampStart,WORD W_ClampEnd);
gmt_RET_STAT		gm_DipProgramHSMask(WORD W_HSMaskEnd);

//******************
//Analog Input Port
//******************
gmt_RET_STAT		gm_AipQBFilterEnable(gmt_ADC_ID B_AdcId,BYTE B_Enable);
gmt_RET_STAT		gm_AipHBFilterEnable(gmt_ADC_ID B_AdcId,BYTE B_Enable);

gmt_RET_STAT  		gm_AipProgramCoast(gmt_ADC_ID B_AdcId, WORD W_CoastStart,WORD W_CoastEnd);
gmt_RET_STAT 		gm_AipProgramHClamp(gmt_ADC_ID B_AdcId,WORD W_ClampStart,WORD W_ClampWidth);
gmt_RET_STAT 		gm_AipProgramHSMask(gmt_ADC_ID B_AdcId,WORD W_HSMaskEnd);
gmt_RET_STAT  		gm_AipProgramVClamp(gmt_ADC_ID B_AdcId,WORD W_ClampStart,WORD W_ClampEnd);
gmt_RET_STAT 		gm_AipSetVideoClampMode(gmt_ADC_ID B_AdcId, gmt_AFE_MUX B_Mux,gmt_AIP_VID_CLAMP_MODE B_Mode);
gmt_RET_STAT 		gm_AipClampDacEnable(gmt_ADC_ID B_AdcId, gmt_AFE_MUX B_Mux,BYTE B_Enable);
void				gm_AipDecimationFilterSetup(gmt_PHY_CH B_Channel);
#if (FEATURE_GLITCH_FILTER==ENABLE)
void                gm_GlitchFilterSetup(gmt_PHY_CH B_Channel);
#endif
gmt_RET_STAT 		gm_AOCEnable(gmt_ADC_ID B_AdcId, BYTE B_Enable);
//****************************************
//Additional Functions
//	1. Used for SCART
//	2. ADC Calibration
//****************************************
//******************
//Analog Input Port
//******************
gmt_RET_STAT		gm_AipMixVideoOut(gmt_ADC_ID B_AdcId, BYTE B_Val);
gmt_RET_STAT		gm_AipVideoOutMixerEnable(gmt_ADC_ID B_AdcId, BYTE B_Enable);


//******************
//ADC
//******************
// Write adc calibration parameters to the registers from structure
void		 		gm_SetAdcGainOffset(gmt_ADC_ID B_AdcId, gmt_ADC_SETTING *);
// Reads adc calibration parameters from registers and populate strucutre
void		 		gm_GetAdcGainOffset(gmt_ADC_ID B_AdcId, gmt_ADC_SETTING *);

//************************************
//Utility functions ADC calibration
//************************************
// Calibrate input adc linearity
gmt_RET_STAT 		gm_AdcCalibrateLinearity(void);
// Adjust first offset
gmt_RET_STAT 		gm_AdcAutoCalibrateOffset1(gmt_ADC_ID B_AdcId);
// Calibration input adc offset 2 and gain for all 3 channels
gmt_RET_STAT 		gm_AdcAutoColorBalance(gmt_ADC_ID B_AdcId, gmt_ADC_CALIBRATION_INPUT B_Input);
// ADC calibration initialization
gmt_RET_STAT gm_AdcCalibrateInit(gmt_ADC_ID B_AdcId, BYTE UserPrefSIF, BYTE IsSCARTRGB);


// printout AFE values
void gm_PrintAfeValues(gmt_ADC_ID B_AdcId);

//************************************
//Utility functions 
//************************************
WORD			gm_GetAdcOffset1Addr(gmt_ADC_ID B_AdcId, BYTE B_Adc_Color);
WORD			gm_GetAdcOffset2Addr(gmt_ADC_ID B_AdcId, BYTE B_Adc_Color);
WORD			gm_GetAdcGain0Addr(gmt_ADC_ID B_AdcId, BYTE B_Adc_Color);
WORD 			gm_GetAdcSelected(gmt_ADC_ID B_AdcId, BYTE B_Adc_Color);
WORD			gm_GetAdcDataAddr(gmt_ADC_ID B_AdcId, BYTE B_Adc_Color);
#define gm_GetAipRegAddr(Address, B_AdcId) (Address)

gmt_PHY_CH		gm_GetChanneltoAip(gmt_ADC_ID B_AdcId);
void   			gmp_WriteAdcGain		(gmt_ADC_ID B_AdcId, BYTE B_AdcColor, WORD W_Gain);
void 			gmp_ReadAdcGain(gmt_ADC_ID B_AdcId, BYTE B_AdcColor, WORD * W_Gain );

//******************************************************************************
//  E X T E R N A L  R E F E R E N C E S
//******************************************************************************
extern BYTE ROM gmc_NumOfReg;
extern const char aS_AdcColor[3][6];

//******************************************************************************
//  M A C R O S
//******************************************************************************
#define ADC_RED   0
#define ADC_GREEN 1
#define ADC_BLUE  2

#define ADC_CH_A	0
#define ADC_CH_B	1
#define ADC_CH_C	2

//******************************************************************************
//  BIT FIELD
//******************************************************************************
#define HC_EN_A_SHIFT 0
#define HC_EN_AN_SHIFT 8
#define HC_EN_BN_SHIFT 10
#define HC_EN_CN_SHIFT 12

#define HC_EN_GGN_SHIFT 10

#define PU_A_SHIFT 0
#define PU_AHS_SHIFT 3

#endif//__GM_INPUT_H__
