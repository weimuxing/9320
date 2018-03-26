#ifndef __DDC2BI_HANDLER_H__
#define __DDC2BI_HANDLER_H__

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
// MODULE:      ddc2bi.h
//
// USAGE:       Header file for the ddc2bi parser handler.
//
//******************************************************************************

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************


//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************
extern BYTE ROM VGAvcpstr[];
extern BYTE ROM strCapabilitiesRequest[];

//******************************************************************************
//  G L O B A L    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
void Ddc2biCmdHandler(void);
WORD far GetCapabilitiesStringSize(void);

void far I2CSlaveInitAll(void);
void far I2CSlaveChannelHandler(SLAVE_CH B_SlaveCh);

void EH_FactoryCommand(void *gExtDebugCmdPtr);
typedef struct FactoryCommandFeature
{
	BYTE Function ;
	BYTE Item ;
	BYTE Length;
	BYTE ROM *StringTbl;
	BYTE ROM *AdjusterFunc;	
}EHFactoryCommandFeature;
#define COMMSTR_LENGTH		3
#define MAX_ITEM_NUM        79//69       //calo update for DELL's request 090518
#define AlignFunc_Enter_alignment           	0x00, 0x00, COMMSTR_LENGTH, tEnter_Align  
#define AlignFunc_Exit_alignment             	0x00, 0x01, COMMSTR_LENGTH, tExit_Align
#define AlignFunc_Set_User_Mode            	0x01, 0x00, COMMSTR_LENGTH, tSet_User
#define AlignFunc_Set_SRGB_Mode          	0x01, 0x01, COMMSTR_LENGTH, tSet_Srgb
#define AlignFunc_Set_Standard_Mode          	0x01, 0x02, COMMSTR_LENGTH, tSet_Standard
#define AlignFunc_Set_AdobeRGB_Mode          	0x01, 0x0A, COMMSTR_LENGTH, tSet_AdobeRGB
#define AlignFunc_switch_to_DVI                 0x01, 0x11, COMMSTR_LENGTH, tSwitchtoDVI
#define AlignFunc_switch_to_HDMI                0x01, 0x16, COMMSTR_LENGTH, tSwitchtoHDMI
#define AlignFunc_switch_to_NormalDP            0x01, 0x20, COMMSTR_LENGTH, tSwitchtoNormalDP //fsf //need complete
#define AlignFunc_Set_5000k                    	0x05, 0x00, COMMSTR_LENGTH, tSet_5000K
#define AlignFunc_Set_5700k                    	0x05, 0x07, COMMSTR_LENGTH, tSet_5700K
#define AlignFunc_Set_6500k                    	0x06, 0x05, COMMSTR_LENGTH, tSet_6500K
#define AlignFunc_Set_7200k                    	0x07, 0x02, COMMSTR_LENGTH, tSet_7200K
#define AlignFunc_Set_7500k                    	0x07, 0x05, COMMSTR_LENGTH, tSet_7500K
#define AlignFunc_Set_9300k                    	0x09, 0x03, COMMSTR_LENGTH, tSet_9300K
#define AlignFunc_Set_10000k                    0x0D, 0x2C, COMMSTR_LENGTH, tSet_10000K
#define AlignFunc_Power_OFF        		0x0D, 0x03, COMMSTR_LENGTH, tPower_Off
#define AlignFunc_Burnin_ON         		0x0D, 0x04, COMMSTR_LENGTH, tBurnin_On
#define AlignFunc_Burnin_OFF        		0x0D, 0x05, COMMSTR_LENGTH, tBurnin_Off
#define AlignFunc_Enter_DDC_Write        	    0x0D, 0x06, COMMSTR_LENGTH, tEnter_DDCW
#define AlignFunc_Exit_DDC_Write        	    0x0D, 0x07, COMMSTR_LENGTH, tExit_DDCW
#define AlignFunc_Read_Panel_Type        		0x0D, 0x0C, COMMSTR_LENGTH,	tPanel_SN
#define AlignFunc_Enter_HiDCR			0x0D, 0x1E, COMMSTR_LENGTH,	tEnable_HiDCR
#define AlignFunc_Exit_HiDCR			0x0D, 0x1F, COMMSTR_LENGTH,	tDisable_HiDCR
#define AlignFunc_Energy_Smart_On               0x0D, 0x20, COMMSTR_LENGTH,	tEnergySmart_On
#define AlignFunc_Energy_Smart_Off              0x0D, 0x21, COMMSTR_LENGTH,	tEnergySmart_Off
#define AlignFunc_Set_Text_Mode                 0x0D, 0x2B, COMMSTR_LENGTH,	tSet_Text
#define AlignFunc_ReCall_Factory    		0x0E, 0x00, COMMSTR_LENGTH, tRecall
#define AlignFunc_Show_Version   		       0x0E, 0x03, NULL_PTR,			NULL_PTR
#define AlignFunc_Clear_NVRAM_WP		0x0F, 0x02, COMMSTR_LENGTH + 1,	tDisable_NvWP
#define AlignFunc_Set_NVRAM_WP			0x0F, 0x03, COMMSTR_LENGTH + 1,	tEnable_NvWP
#define AlignFunc_Reply_checksum			    0x0F, 0x0F, NULL_PTR,	NULL_PTR
#define AlignFunc_SRGB_Adobe_COLOR_RED1	       	0x0F, 0x20, COMMSTR_LENGTH + 1,	tAdjust_SRGBR1 //////LANDY ADD IT For SRGB Color accuraty.
#define AlignFunc_SRGB_Adobe_COLOR_RED2			0x0F, 0x21, COMMSTR_LENGTH + 1,	tAdjust_SRGBR2
#define AlignFunc_SRGB_Adobe_COLOR_RED3	       	0x0F, 0x22, COMMSTR_LENGTH + 1,	tAdjust_SRGBR3
#define AlignFunc_SRGB_Adobe_COLOR_GREEN1		0x0F, 0x23, COMMSTR_LENGTH + 1,	tAdjust_SRGBG1
#define AlignFunc_SRGB_Adobe_COLOR_GREEN2	       0x0F, 0x24, COMMSTR_LENGTH + 1,	tAdjust_SRGBG2
#define AlignFunc_SRGB_Adobe_COLOR_GREEN3		0x0F, 0x25, COMMSTR_LENGTH + 1,	tAdjust_SRGBG3
#define AlignFunc_SRGB_Adobe_COLOR_BLUE1	       	0x0F, 0x26, COMMSTR_LENGTH + 1,	tAdjust_SRGBB1
#define AlignFunc_SRGB_Adobe_COLOR_BLUE2			0x0F, 0x27, COMMSTR_LENGTH + 1,	tAdjust_SRGBB2
#define AlignFunc_SRGB_Adobe_COLOR_BLUE3	       	0x0F, 0x28, COMMSTR_LENGTH + 1,	tAdjust_SRGBB3
#define AlignFunc_SRGB_Adobe_SAVE                              0x0F, 0x29, COMMSTR_LENGTH + 1,	tAdjust_SAVE
#define AlignFunc_RETURN_COLORVALUEDELTAE                              0x0F, 0x2A, NULL_PTR,	NULL_PTR
#define AlignFunc_SRGB_Gamma_Select                         0x0F, 0x2B, COMMSTR_LENGTH + 1,	tAdjust_SRGBGAMMA
#define AlignFunc_AdobeRGB_Gamma_Select                 0x0F, 0x2C, COMMSTR_LENGTH + 1,	tAdjust_AdobeRGBGAMMA
#define AlignFunc_RETURN_GammaSelect            0x0F, 0x2D, NULL_PTR,	NULL_PTR
#define AlignFunc_SELECT_CAL1                   0x0F, 0x2D, NULL_PTR,	tSelect_Cal1
#define AlignFunc_SELECT_CAL2                   0x0F, 0x2D, NULL_PTR,	tSelect_Cal2
#define AlignFunc_U2413_MODEL_confirm     0x0F, 0xFF , NULL_PTR,	NULL_PTR  //calo update for the ISP mode confirm 090413
#define AlignFunc_Save_User_Mode             	0x11, 0x10, COMMSTR_LENGTH, tSave_User
#define AlignFunc_Save_SRGB_Mode             	0x11, 0x11, COMMSTR_LENGTH, tSave_Srgb
#define AlignFunc_Save_AdobeRGB_Mode          	0x11, 0x1A, COMMSTR_LENGTH, tSave_AdobeRGB
#define AlignFunc_Save_5000k                  	0x15, 0x10, COMMSTR_LENGTH, tSave_5000K
#define AlignFunc_Save_5700k                  	0x15, 0x17, COMMSTR_LENGTH, tSave_5700K
#define AlignFunc_Save_6500k                  	0x16, 0x15, COMMSTR_LENGTH, tSave_6500K
#define AlignFunc_Save_7200k                    0x17, 0x12, COMMSTR_LENGTH, tSave_7200K
#define AlignFunc_Save_7500k                  	0x17, 0x15, COMMSTR_LENGTH, tSave_7500K
#define AlignFunc_Save_9300k                  	0x19, 0x13, COMMSTR_LENGTH, tSave_9300K
#define AlignFunc_Save_10000k                  	0x1A, 0x10, COMMSTR_LENGTH, tSave_10000K
#define AlignFunc_switch_to_MiniDP              0x1A, 0x11, COMMSTR_LENGTH, tSwitchtoMiniDP
#define AlignFunc_Select_UNIFORMITY_Table1 		0x1A, 0x12, COMMSTR_LENGTH, tUNIFORMITYTABLE1
#define AlignFunc_Select_UNIFORMITY_Table2 		0x1A, 0x13, COMMSTR_LENGTH, tUNIFORMITYTABLE2
#define AlignFunc_Turnoff_UNIFORMITY_Table		0x1A, 0x14, COMMSTR_LENGTH,	tUNIFORMITYOFF
#define AlignFunc_Write_brightness        	    0x40, 0x00, COMMSTR_LENGTH, tWrite_Bri
#define AlignFunc_Write_Contrast           	    0x40, 0x01, COMMSTR_LENGTH, tWrite_Con
#define AlignFunc_Write_RGain                	0x40, 0x02, COMMSTR_LENGTH, tWrite_RGain
#define AlignFunc_Write_GGain                	0x40, 0x03, COMMSTR_LENGTH, tWrite_GGain
#define AlignFunc_Write_BGain                	0x40, 0x04, COMMSTR_LENGTH, tWrite_BGain
#define AlignFunc_Write_PWM                  	0x40, 0x05, COMMSTR_LENGTH, tWrite_PWM
#define AlignFunc_Save_PWM                   	0x40, 0x06, COMMSTR_LENGTH, tSave_PWM
#define AlignFunc_Read_PWM                   	0x40, 0x07, NULL_PTR,			NULL_PTR
#define AlignFunc_ColorReset               		0x40, 0x08, COMMSTR_LENGTH, tReset_Color
#define AlignFunc_Read_RGain                	0x50, 0x02, NULL_PTR,			NULL_PTR
#define AlignFunc_Read_GGain                	0x50, 0x03, NULL_PTR,			NULL_PTR
#define AlignFunc_Read_BGain                	0x50, 0x04, NULL_PTR,			NULL_PTR
#define AlignFunc_Read_Brightness		        0x50, 0x0d, NULL_PTR,			NULL_PTR
#define AlignFunc_Read_Contrast			        0x50, 0x0e, NULL_PTR,			NULL_PTR
#define AlignFunc_ReadBurninTime          0x50, 0x20, NULL_PTR,	NULL_PTR
#define AlignFunc_NULL					0xff, 0xff, NULL_PTR,			NULL_PTR
//#define	DDC2Bi_CONTROL_STATUS_FLAG		0x80

//DDC2Bi commands
#define DDC2B_CMD_DEBUG						0xC2
#define DDC2B_CMD_GetVCPFeature				0x01
#define DDC2B_CMD_GetVCPFeature_Reply			0x02
#define DDC2B_CMD_SetVCPFeature				0x03
#define DDC2B_CMD_SaveCurrentSettings			0x0C
#define DDC2B_CMD_CapabilitiesRequest			0xF3
#define DDC2B_CMD_CapabilitiesRequest_Reply		0xE3
#define DDC2B_CMD_GetTimingReport				0x07
#define DDC2B_CMD_GetTimingReport_Reply		0x4E

#define DDC2B_CMD_EH_FactoryCommand		0x6E

//NMV Custom commands
#define DDC2B_CMD_NMV_GetVCPPage				0xC4
#define DDC2B_CMD_NMV_GetVCPPage_Reply		0xC5
#define DDC2B_CMD_NMV_GetPagedVCP			0xC6
#define DDC2B_CMD_NMV_GetPagedVCP_Reply		0xC7
#define DDC2B_CMD_NMV_SetPagedVCP			0xC8
#define DDC2B_CMD_NMV_SetPagedVCP_Reply		0xC7
#define DDC2B_CMD_NMV_EEPROM					0xC0
#define DDC2B_CMD_NMV_EEPROM_Reply			0xC1

//NMV EEPROM commands
#define DDC2B_CMD_NMV_EEPROMRead				0x09
#define DDC2B_CMD_NMV_AssetRead				0x0B
#define DDC2B_CMD_NMV_AssetWrite				0x0E

//NMV EEPROM commands
//#define DDC2B_CMD_NMV_EEPROMRead			 0x09
#define DDC2B_CMD_NMV_EEPROMWrite               	0x02 
//#define DDC2B_CMD_NMV_AssetRead			 	0x0B
//#define DDC2B_CMD_NMV_AssetWrite			 	0x0E

#define DDC2B_CMD_NMV_ASSET_READ_Wistron                  0x99 
#define DDC2B_CMD_NMV_ASSET_WRITE_Wistron                 0x55

#define DDC2B_CMD_NMV_ModeChangeRequest   	0x11 
#define DDC2B_WISTRON_SERIAL_NUMBER           	0xAA
#define DDC2B_WISTRON_BACKLIGHT_HOUR         	0xBB
#define DDC2B_WISTRON_FW_VERSION                 	0xCC
#define DDC2B_WISTRON_MODELNAME                  	0xDD
#define DDC2B_WISTRON_GET_FW_PN                  	0xEE

#define DDC2B_WISTRON_EDID_WP                      	0xF2
#define DDC2B_WISTRON_SET_AGING_MODE        	0x77
#define DDC2B_CMD_VCP_TextBrightness				0x11
#define DDC2B_CMD_VCP_sRGBBrightness				0x17//Hyb131204* Org = 0x13 
#define DDC2B_CMD_VCP_AdobeBrightness				0x15
#define DDC2B_CMD_VCP_UNIFORMITY_Brightness				0x13//Hyb131204* Org = 0x17


//VCP commands Page0
#define DDC2B_CMD_VCP_NewControlVal			0x02		//To make MCCS 2.0 Compliant
#define DDC2B_CMD_VCP_ALLReset				0x04
#define DDC2B_CMD_VCP_RestoreContrastBrightness		0x05
#define DDC2B_CMD_VCP_GeometryReset			0x06
#define DDC2B_CMD_VCP_ColorReset				0x08
#define DDC2B_CMD_VCP_Clock					0x0E
#define DDC2B_CMD_VCP_Brightness				0x10
#define DDC2B_CMD_VCP_Contrast					0x12
#define DDC2B_CMD_VCP_Backlight						0x13		//Hyb131204+ for 
#define DDC2B_CMD_VCP_SelectColorPreset		0x14
#define DDC2B_CMD_VCP_RedGain					0x16
#define DDC2B_CMD_VCP_R_VT					0x17
#define DDC2B_CMD_VCP_GreenGain				0x18
#define DDC2B_CMD_VCP_G_VT					0x19
#define DDC2B_CMD_VCP_BlueGain					0x1A
#define DDC2B_CMD_VCP_B_VT					0x1B
#define DDC2B_CMD_VCP_AutoSetUp				0x1E
#define DDC2B_CMD_VCP_HPosition				0x20
//#define DDC2B_CMD_VCP_OSD						0xCA
#define DDC2B_CMD_VCP_VPosition				0x30
#define DDC2B_CMD_VCP_ClockPhase				0x3E
#define DDC2B_CMD_VCP_SourceSelect				0x60
#define DDC2B_CMD_VCP_AudioVolume				0x62
#define DDC2B_CMD_VCP_Language				0x68
#define DDC2B_CMD_VCP_TVSaturation				0x8A
#define DDC2B_CMD_VCP_TVSharpness				0x8C
#define DDC2B_CMD_VCP_RED						0x9B
#define DDC2B_CMD_VCP_YELLOW					0x9C
#define DDC2B_CMD_VCP_GREEN					0x9D
#define DDC2B_CMD_VCP_CYAN					0x9E
#define DDC2B_CMD_VCP_BLUE					0x9F
#define DDC2B_CMD_VCP_MAGENDA				0xA0
#define DDC2B_CMD_VCP_SyncType				0xA8
#define DDC2B_CMD_VCP_HFreq					0xAC    //To make MCCS 2.0 Compliant
#define DDC2B_CMD_VCP_VFreq					0xAE    //To make MCCS 2.0 Compliant
#define DDC2B_CMD_VCP_RGBodering				0xB4
#define DDC2B_CMD_VCP_MonitorType				0xB6
//#define DDC2B_CMD_VCP_ColorTemp				0xE0
//#define DDC2B_CMD_VCP_ControlLock				0xE3
//#define DDC2B_CMD_VCP_AutoColor				0xE4
#define DDC2B_CMD_VCP_Settings					0xB0
#define DDC2B_CMD_VCP_DisplayUsageTime		0xC0    //To make MCCS 2.0 Compliant
#define DDC2B_CMD_VCP_ApplnEnKey				0xC6    //To make MCCS 2.0 Compliant
#define DDC2B_CMD_VCP_DispControllerType		0xC8    //To make MCCS 2.0 Compliant
#define DDC2B_CMD_VCP_DispFirmwareLevel		0xC9    //To make MCCS 2.0 Compliant
#define DDC2B_CMD_VCP_OSD						0xCA    //To make MCCS 2.0 Compliant
#define DDC2B_CMD_VCP_OsdLanguage			0xCC
#define DDC2B_CMD_VCP_DisplayPowerMode		0xD6
#define DDC2B_CMD_VCP_MCCSVersion				0xDF    //To make MCCS 2.0 Compliant
//#define DDC2B_CMD_VCP_Power						0xE1
#define DDC2B_CMD_VCP_BlackLevel				0x1D
#define DDC2B_CMD_VCP_Red_Video_Black_Level     0x6C
#define DDC2B_CMD_VCP_Green_Video_Black_Level   0x6E
#define DDC2B_CMD_VCP_Blue_Video_Black_Level    0x70
#define DDC2B_CMD_VCP_PresetMode				0xDC  //owen 20120531-->
#define DDC2B_CMD_VCP_PowerNap1					0xE0	
#define DDC2B_CMD_VCP_PowerNap2					0xE1	
#define DDC2B_CMD_VCP_AllColor					0xE2	
#define DDC2B_CMD_VCP_SmartVideoEnhance			0xE3
#define DDC2B_CMD_VCP_Uniformity				0xE4
#define DDC2B_CMD_VCP_ColorSpace				0xF0
#define DDC2B_CMD_VCP_Dell_Feature_Support		0xF1
#define DDC2B_CMD_VCP_DellFeatureStatus			0xF2//owen 20120531---<
#define DDC2B_CMD_VCP_AutoRotation				0xAA
#define DDC2B_CMD_VCP_ManufacturerSpcific       0xFD  //chunbao

#define DDC2B_CMD_VCP_LUT_SIZE						0x73
#define DDC2B_CMD_VCP_LUT_SAVE						0x75

#define DDC2B_Max_VCP_Pages					2
//VCP commands Page2
#define DDC2B_CMD_VCP_ContrastAuto				0x37
#define DDC2B_CMD_VCP_OSDHPosition			0x38
#define DDC2B_CMD_VCP_OSDVPosition				0x39

//Size of Message Packet to send upon Capabilities String Request
#define VCP_GETCAPABILITIES_REPLYMSG_SIZE 0x23

//Supported DDC2B_CMD_VCP_SourceSelect Input sources
#define VCP_SourceSelect_AnalogVideo_1		0x01 		// 1 for Analog video(R/G/B) 1
#define VCP_SourceSelect_AnalogVideo_2		0x02 		// 2 for Analog video(R/G/B) 2
#define VCP_SourceSelect_DigitalVideo_1		0x03 		// 3 for Digital video(TMDS) 1
#define VCP_SourceSelect_DigitalVideo_2		0x04 		// 4 for Digital video(TMDS) 2
#define VCP_SourceSelect_CompositeVideo_1 	0x05 		// 5 for Composite video 1
#define VCP_SourceSelect_CompositeVideo_2 	0x06 		// 6 for Composite video 2
#define VCP_SourceSelect_SVideo_1 			0x07 		// 7 for S video 1
#define VCP_SourceSelect_SVideo_2 			0x08 		// 8 for S video 2
#define VCP_SourceSelect_Tuner_1 			0x09 		// 9 for Tuner 1
#define VCP_SourceSelect_Tuner_2 			0x0A 		// 10 for Tuner 2
#define VCP_SourceSelect_Tuner_3 			0x0B 		// 11 for Tuner 3
#define VCP_SourceSelect_ComponentVideo_1 	0x0C 		// 12 for Component video(YPrPb/YCrCb) 1
#define VCP_SourceSelect_ComponentVideo_2 	0x0D 		// 13 for Component video(YPrPb/YCrCb) 2
#define VCP_SourceSelect_ComponentVideo_3 	0x0E 		// 14 for Component video(YPrPb/YCrCb) 3
#define VCP_SourceSelect_DisplayPort_1		0x0F 		// 15 for DisplayPort 1
#define VCP_SourceSelect_DisplayPort_2		0x10 		// 16 for DisplayPort 1
#define VCP_SourceSelect_HdmiVideo_1		0x11 		// 17 for Component video(YPrPb/YCrCb) 3
#define VCP_SourceSelect_HdmiVideo_2		0x12 		// 18 for Component video(YPrPb/YCrCb) 3

// receive/transmit buffer syntax defines
#define	LENGTH								0
#define	COMMAND							1
#define	PARAMETER							2

#define ADJUSTER_RBLACKLEVEL_DECLARED
#define ADJUSTER_GBLACKLEVEL_DECLARED
#define ADJUSTER_BBLACKLEVEL_DECLARED







#ifdef USE_XRite
#define DDC2B_CMD_Xrite_Set						 		0xF1
#define DDC2B_CMD_Xrite_Get						 		0xF0
#define DDC2B_CMD_Xrite_SetCurrentColorSpaceData		0x60
#define DDC2B_CMD_Xrite_SetCurrentColorSpaceData_Reset	0x00
#define DDC2B_CMD_Xrite_SetCurrentColorSpaceData_Upload	0xFF
#define DDC2B_CMD_Xrite_SetInputGamma				0x34
#define DDC2B_CMD_Xrite_SetInputGammaRed			0x10
#define DDC2B_CMD_Xrite_SetInputGammaGreen			0x11
#define DDC2B_CMD_Xrite_SetInputGammaBlue			0x12
#define DDC2B_CMD_Xrite_SetInputGammaRed_Steep		0x20
#define DDC2B_CMD_Xrite_SetInputGammaGreen_Steep    0x21
#define DDC2B_CMD_Xrite_SetInputGammaBlue_Steep		0x22
#define DDC2B_CMD_Xrite_SetOutputGamma				0x30
#define DDC2B_CMD_Xrite_SetOutputGammaRed			0x10
#define DDC2B_CMD_Xrite_SetOutputGammaGreen			0x11
#define DDC2B_CMD_Xrite_SetOutputGammaBlue			0x12
#define DDC2B_CMD_Xrite_SetOutputGammaRed_Steep		0x20
#define DDC2B_CMD_Xrite_SetOutputGammaGreen_Steep	0x21
#define DDC2B_CMD_Xrite_SetOutputGammaBlue_Steep	0x22
#define DDC2B_CMD_Xrite_SetMatrix					0x20
#define DDC2B_CMD_Xrite_GetReadyToReceiveStatus		0x50
#define DDC2B_CMD_Xrite_CloseCurrentColorSpace		0x66
#define DDC2B_CMD_Xrite_GetSerialNumber				0x44
#define DDC2B_CMD_Xrite_CheckColorStatus			0x55
#define DDC2B_CMD_Xrite_CheckColorStatus_Recovery	0x56

#define DDC2B_CMD_Xrite_EnterFactoryOSD				0x80
typedef struct DDC2Bi_CGS_VCPCmdStruct
{
   BYTE dummy1;
   BYTE length;
   BYTE command;	   //Table command
   BYTE vcp_opcode;   //0xF0
   BYTE vcp_subopcode;
   BYTE data_length;
   BYTE dummy2;
   BYTE Blocknumber;
   BYTE chk;
}  DDC2Bi_CGS_VCPCmd_t;

typedef struct DDC2Bi_CGetVCPFeature_ReplyCmdStruct
{
   BYTE	length;
   BYTE	command;
   BYTE	vcp_opcode;
   BYTE result;
}DDC2Bi_CGetVCPFeature_ReplyCmd_t;

#ifdef Qubyx_Uniformity
#define DDC2B_CMD_Uniformity_Start			0x70
#define DDC2B_CMD_Uniformity_XY_ColorMono	0x71
#define DDC2B_CMD_Uniformity_Data			0x72
#define DDC2B_CMD_Uniformity_Save			0x73
#define DDC2B_CMD_Xrite_GetDisplayTime		0x75
#endif

#endif //USE_XRite

#ifdef USE_COLOR_FIDELITY
#if 1//USE_DYNAMIC_GAMUTMAPPING_SUPPORT
#define DDC2B_CMD_VCP_SimplifiedColorProfile    0xF0    //steven le, 8/14/2009
#define DDC2B_CMD_Table_Write						 0xE7
#define DDC2B_CMD_Table_Read							 0xE2
#define DDC2B_CMD_Table_Reply						 0xE4

typedef struct DDC2Bi_WGS_VCPCmdStruct
{
   BYTE length;
   BYTE command;      //Table command
   BYTE vcp_opcode;   //0xF0
   //WORD value;
   BYTE value_H;
   BYTE value_L;
   BYTE mode;
   BYTE par;
   BYTE chk;
}  DDC2Bi_WGS_VCPCmd_t;

typedef struct DDC2Bi_WGS_ReplyCmdStruct
{
   BYTE length;
   BYTE command;      //Table command
   BYTE vcp_opcode;   //0xF0
   //WORD value;
   BYTE value_H;
   BYTE value_L;
   //WORD status;
   BYTE status_H;
   BYTE status_L;
   BYTE chk;
}  DDC2Bi_WGS_ReplyCmd_t;

#endif
#endif // USE_COLOR_FIDELITY

typedef enum
{
   DDCCI_CTEMP_ADOBERGB = 0,    // 20121019, Nicholas Pan
   DDCCI_CTEMP_SRGB = 1,
   DDCCI_CTEMP_DISPLAY_NATIVE,	//02
   DDCCI_CTEMP_4000K,			//03
   DDCCI_CTEMP_5000K,			//04
   DDCCI_CTEMP_6500K,			//05
   DDCCI_CTEMP_7500K,			//06
   DDCCI_CTEMP_8200K,			//07
   DDCCI_CTEMP_9300K,			//08
   DDCCI_CTEMP_10000K,			//09
   DDCCI_CTEMP_11500K,			//0A
   DDCCI_CTEMP_5700K,			//0B 120502 Edward
   DDCCI_CTEMP_USER1,			//0C
   DDCCI_CTEMP_USER2,			//0D
   DDCCI_CTEMP_USER3			//0E
} DDC_COLOR_TEMP;
typedef struct DDC2Bi_GetVCPFeatureCmdStruct
{
   BYTE length;
   BYTE command;
   BYTE vcp_opcode;
   BYTE chk;
}DDC2Bi_GetVCPFeatureCmd_t;

typedef struct VCPFeaturePktBodyStruct
{
   BYTE vcp_opcode;
   BYTE vcp_type;
   WORD max_BigEndian;
   WORD value_BigEndian;
}VCPFeaturePktType;


#pragma pack(push)
#pragma pack(1)
typedef struct DDC2Bi_GetVCPFeature_ReplyCmdStruct
{
   BYTE				length;
   BYTE				command;
   BYTE				result;
   VCPFeaturePktType	a_VCPFeature;
   BYTE chk;
}DDC2Bi_GetVCPFeature_ReplyCmd_t;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct DDC2Bi_SetVCPFeatureCmdStruct
{
   BYTE length;
   BYTE command;
   BYTE vcp_opcode;
   WORD val_BigEndian;
}DDC2Bi_SetVCPFeatureCmd_t;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct DDC2Bi_GetPagedVCPCmdStruct
{
   BYTE length;
   BYTE command;
   BYTE vcp_page;
   BYTE vcp_opcode;
}DDC2Bi_GetPagedVCPCmd_t;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct DDC2Bi_GetPagedVCP_ReplyCmdStruct
{
   BYTE				length;
   BYTE				command;
   BYTE				result;
   BYTE				vcp_page;
   VCPFeaturePktType	b_VCPFeature;
   BYTE chk;
}DDC2Bi_GetPagedVCP_ReplyCmd_t;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct DDC2Bi_SetPagedVCPCmdStruct
{
   BYTE length;
   BYTE command;
   BYTE vcp_page;
   BYTE vcp_opcode;
   WORD value_BigEndian;
   BYTE chk;
}DDC2Bi_SetPagedVCPCmd_t;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct DDC2Bi_GetVCPPage_ReplyCmdStruct
{
   BYTE length;
   BYTE command;
   BYTE vcp_page;
   BYTE vcp_maxpage;
}DDC2Bi_GetVCPPage_ReplyCmd_t;
#pragma pack(pop)

//NMV EEPROM commands definitions
#pragma pack(push)
#pragma pack(1)
typedef struct DDC2Bi_EEPROMCmdStruct
{
   BYTE length;
   BYTE command;
   BYTE eeprom_cmd;
}DDC2Bi_EEPROMCmd_t;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct DDC2Bi_EEPROMReadCmdStruct
{
   BYTE length;
   BYTE command;
   BYTE eeprom_cmd;
   BYTE leng;
   WORD addr;
   BYTE chk;
}DDC2Bi_EEPROMReadCmd_t;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct DDC2Bi_AssetReadCmdStruct
{
   BYTE length;
   BYTE command;
   BYTE eeprom_cmd;
   BYTE offset;
   BYTE leng;
   BYTE chk;
}DDC2Bi_AssetReadCmd_t;
#pragma pack(pop)

#if 1//
#pragma pack(push)
#pragma pack(1)
typedef struct DDC2Bi_AssetWriteCmdStruct
{
   BYTE length;
   BYTE command;
   BYTE eeprom_cmd;
   BYTE offset;
   BYTE data[];
}DDC2Bi_AssetWriteCmd_t;
#pragma pack(pop)
#else
#pragma pack(push)
#pragma pack(1)
typedef struct DDC2Bi_AssetWriteCmdStruct
{
   BYTE length;
   BYTE command;
   BYTE eeprom_cmd;
   BYTE offset;
}DDC2Bi_AssetWriteCmd_t;
#pragma pack(pop)
#endif

#pragma pack(push)
#pragma pack(1)
typedef struct
{
   BYTE length;
   BYTE command;
   WORD offset_BigEndian;
   BYTE chk;
}DdcciCapabilitiesRequest;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct DDC2Bi_GetTiming_ReplyCmd
{
   BYTE length;
   BYTE command;
   BYTE status;
   WORD h_freq_BigEndian;
   WORD v_freq_BigEndian;
} DDC2Bi_GetTiming_ReplyCmd_t;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct
{
   BYTE length;
   BYTE command;
   WORD offset_BigEndian;
   BYTE data;
} DdcciCapabilitiesReply;
#pragma pack(pop)


#pragma pack(push)
#pragma pack(1)
typedef struct DDC2Bi_ModeChangeRequestStruct
{
		BYTE length;
		BYTE command;
		BYTE eeprom_cmd;
		BYTE mode_id;
		BYTE chk;
		}DDC2Bi_ModeChangeRequest_t;
#pragma pack(pop)


#pragma pack(push)
#pragma pack(1)
typedef struct DDC2Bi_ModeChange_ReplyCmdStruct
{
   BYTE length;
   BYTE command;
   BYTE eeprom_cmd;
   BYTE mode_id;
   BYTE chk;
  }DDC2Bi_ModeChange_ReplyCmd_t;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct DDC2Bi_EEPROMWriteCmdStruct
{
   BYTE length;
   BYTE command;
   BYTE eeprom_cmd;
   BYTE leng;
   WORD addr;
   BYTE data[];
   BYTE chk;
}DDC2Bi_EEPROMWriteCmd_t;
#pragma pack(pop)


#pragma pack(push)
#pragma pack(1)
typedef struct DDC2Bi_EEPROMWrite_ReplyCmdStruct
{
   BYTE length;
   BYTE command;
   BYTE eeprom_cmd;
   BYTE RC;
   BYTE chk;
  }DDC2Bi_EEPROMWrite_ReplyCmd_t;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct DDC2Bi_EEPROMRead_ReplyCmdStruct
{
   BYTE length;
   BYTE command;
   BYTE eeprom_cmd;
   BYTE data[];
   BYTE chk;
   }DDC2Bi_EEPROMRead_ReplyCmd_t;
#pragma pack(pop)


typedef enum SendAckNackDataStruct
{
    SND_NAK = 0,
    SND_ACK,
    SND_DATA,
    SND_NOTHING
}SendAckNackData_t;

#define SetBit(VAR,BIT)         ( VAR |= BIT )
#define ClrBit(VAR,BIT)         ( VAR &= ~BIT )
#define CheckBit(VAR,BIT)       ( VAR | BIT )
#define RetBit(VAR,BIT)         (( VAR & BIT ) != 0 )

#define	GET_VCP	0
#define	SET_VCP	1

#define Start_of_GprobeCmd_in_DDC2BiMsg   5   // 3 (device id + length + cmd) + 2 (manufacturer id)
#define I2C_as_Gprobe_Cmd(CH)			(Sp_I2CSlavePtrGlobal[CH]->Buffer[2])
#define I2C_ManufacturerID(CH)			(*(WORD*)(&(Sp_I2CSlavePtrGlobal[CH]->Buffer[3])))

#endif //#ifndef __DDC2BI_HANDLER_H___
//*********************************  END  **************************************
