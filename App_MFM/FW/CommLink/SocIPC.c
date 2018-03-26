/*
    $Workfile:   SocIPC.c  $
    $Revision: 1.110 $
    $Date: 2013/05/23 06:31:39 $
*/

#define __SOCIPC_C__
//******************************************************************************
// COPYRIGHT (C) STMicroelectronics 2012.
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
// MODULE:      SocIPC.c
//
// USAGE:       This module contains communication functions between Main & Sub Socs.
//
//******************************************************************************


//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include <string.h>
#include "System\all.h"
#include "ACR_Handler.h"

#if ((FEATURE_SOCIPC == ENABLE) || defined(FEATURE_HOSTIF))
//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************

//******************************************************************************
//  E X T E R N A L
//******************************************************************************

#if ENABLE_SOC_IPC_KEYPAD_VIA_QUEUE
KeypadRecvFormat_t gKeypadQueueBuffer[KEYPAD_BUFFER_MAX];
#else //Original
extern BOOL g_Keypad_GetValidKeys;
extern BYTE g_Keypad_Channel;
extern BYTE g_Keypad_Value;
#endif

#if ENABLE_SOC_IPC_IR_VIA_QUEUE
IRRecvFormat_t gIRKeyQueueBuffer[IR_BUFFER_MAX];
#else //Original
extern BOOL g_IR_GetValidKeys;
extern BYTE g_IR_Key;
#endif

extern BOOL IsInitialFinished;
BYTE Athena_RIGHT_CurrentCableStatus = 0;

extern gmt_PwrStateType PwrState;


#ifdef USE_SMT
extern BYTE SMTMsgLen;//global variable for inter-chip SMT communication
#define SMT_INTERCHIP_COMM_DELAY_TIME       55  //in ms
#define SMT_INTERCHIP_COMM_TIMEOUT          2000  //in ms
//BYTE SMTDelayTime = SMT_INTERCHIP_COMM_DELAY_TIME;
extern BOOL SMT_SlaveMessageHandler( /*BYTE SMT_Port_Num, */BYTE * data, BYTE comm_use);
#endif

extern PortSchemeDependentUserPrefType ROM SchemeFactoryDefaultInROM[NUM_IMAGE_SCHEMES]; // PSD
extern ModeIndependentUserPrefType ROM FactoryDefaultInROM;
extern void UpdateDpRx0MstInfo(BYTE MstInfo);

//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************
SBYTE TempBrightness = 0;
WORD TempContrast = 0;
BYTE TempSharpness = 0;
BYTE TempColorTemp = 0;
SBYTE TempImageScheme = 0;

BOOL IgnoreInterChipCmd = FALSE;

#if ENABLE_VEGA_AVAIL_CHECK_VIA_FE_AC_ON
BOOL VEGA_Initialed = FALSE;
#endif

ROM BYTE IPC_AckPakt[] =     {3,    SICMD_ACK};
ROM BYTE IPC_NakPakt[] =     {3,    SICMD_NAK};


SoCIPCPara_t gAthena_RIGHT_Master_Parameters = {
	0,// PowerStatus
	/*ChengLin 20130110: Color Temp - User Settings*/
	0,//CTUserRGain
	0,//CTUserGGain
	0,//CTUserBGain
	0,//CTUserROffset
	0,//CTUserGOffset
	0,//CTUserBOffset		
	/*****************************************/		
	0,// CableStatus
	0,// ModeFlags
	0,// HWidth
	0,// VLength
	0,// HTotal
	0,// VTotal
	0,// HStart
	0,// VStart
	0,// PortStatus
	0,// Brightness
	0,// Contrast
	0,// Sharpness
	0,// ColorSettings
	0,// VideoProgress
	0,// NRSettings
	0,// VideoProgress_1
	0,// DisplaySetup
	0,// Audio1
	0,// Audio2
	0,// Setup
	0,// OSDSync
	0,// DDC_VCPCode
	0,// DDC_Value
	0,// DDC_VCP_Type
	0,// WORD DDC_VCP_Max	
	0,// SlaveAddress
	0,// SlaveWriteData
	0, // SlaveReadData
	0, // DPSwitch
	0xFFFF,  // DCR Result	
/*ChengLin 20130110: 6-axis Saturation, Hue, Brightness controls*/
	0,//SixAxisSatRed
	0,//SixAxisSatGreen
	0,//SixAxisSatBlue
	0,//SixAxisSatYellow
	0,//SixAxisSatMagenta
	0,//SixAxisSatCyan
	0,//SixAxisSatBlack
	0,//SixAxisSatWhite
	0,//SixAxisHueRed
	0,//SixAxisHueGreen
	0,//SixAxisHueBlue
	0,//SixAxisHueYellow
	0,//SixAxisHueMagenta
	0,//SixAxisHueCyan
	0,//SixAxisHueBlack
	0,//SixAxisHueWhite
	0,//SixAxisBrightRed
	0,//SixAxisBrightGreen
	0,//SixAxisBrightBlue
	0,//SixAxisBrightYellow
	0,//SixAxisBrightMagenta
	0,//SixAxisBrightCyan
	0,//SixAxisBrightBlack
	0,//SixAxisBrightWhite
	0,//CaptureMain
	0, //DpRx0MstInfo
	0, // DpRx0_IsStable	
	0, //DPRX0_ReModeSetup	
/*****************************************/
/*ChengLin 20130128: New Sync Handler State Control*/	
	IMAGE_LVDS_ON_STATE, //ImageSyncState;
/*********************************************/
/*ChengLin 20130222: Add for sync Left/Right PB Control Status*/
#if ENABLE_ATHENA_R_L_IMAGE_SYNC_HANDLER
	0, //ImageSyncBlackScreen
#endif
#ifdef SMART_ISP
   0,//SmartIspCtrl
   0,//SmartIspStep
   0,//SmartIspStatus
#endif
   FALSE, //SplashLoadDefault
	0 //GammaCorrection
};



SoCIPCPara_t gAthena_LEFT_FE_Slave_Parameters = {
	0,// PowerStatus
	/*ChengLin 20130110: Color Temp - User Settings*/
	0,//CTUserRGain
	0,//CTUserGGain
	0,//CTUserBGain
	0,//CTUserROffset
	0,//CTUserGOffset
	0,//CTUserBOffset		
	/*****************************************/		
	0,// CableStatus
	0,// ModeFlags
	0,// HWidth
	0,// VLength
	0,// HTotal
	0,// VTotal
	0,// HStart
	0,// VStart
	0, // VFreq
	0,// PortStatus
	0,// Brightness
	0,// Contrast
	0,// Sharpness
	0,// ColorSettings
	0,// VideoProgress
	0,// NRSettings
	0,// VideoProgress_1
	0,// DisplaySetup
	0,// Audio1
	0,// Audio2
	0,// Setup
	0,// OSDSync
	0,// DDC_VCPCode
	0,// DDC_Value	
	0,// DDC_VCP_Type
	0,// WORD DDC_VCP_Max	
	0,// SlaveAddress
	0,// SlaveWriteData
	0, // SlaveReadData
	0, // DPSwitch
	0xFFFF,  // DCR Result
/*ChengLin 20130110: 6-axis Saturation, Hue, Brightness controls*/
	0,//SixAxisSatRed
	0,//SixAxisSatGreen
	0,//SixAxisSatBlue
	0,//SixAxisSatYellow
	0,//SixAxisSatMagenta
	0,//SixAxisSatCyan
	0,//SixAxisSatBlack
	0,//SixAxisSatWhite
	0,//SixAxisHueRed
	0,//SixAxisHueGreen
	0,//SixAxisHueBlue
	0,//SixAxisHueYellow
	0,//SixAxisHueMagenta
	0,//SixAxisHueCyan
	0,//SixAxisHueBlack
	0,//SixAxisHueWhite
	0,//SixAxisBrightRed
	0,//SixAxisBrightGreen
	0,//SixAxisBrightBlue
	0,//SixAxisBrightYellow
	0,//SixAxisBrightMagenta
	0,//SixAxisBrightCyan
	0,//SixAxisBrightBlack
	0,//SixAxisBrightWhite
	0,//CaptureMain
	0, //DpRx0MstInfo
	0, // DpRx0_IsStable	
	0, //DPRX0_ReModeSetup
/*****************************************/
/*ChengLin 20130128: New Sync Handler State Control*/	
	IMAGE_LVDS_ON_STATE, //ImageSyncState;
/*********************************************/
/*ChengLin 20130222: Add for sync Left/Right PB Control Status*/
#if ENABLE_ATHENA_R_L_IMAGE_SYNC_HANDLER
	0, //ImageSyncBlackScreen
#endif
#ifdef SMART_ISP
   0,//SmartIspCtrl
   0,//SmartIspStep
   0,//SmartIspStatus
#endif
   FALSE, //SplashLoadDefault
	0 //GammaCorrection
};

/*ChengLin 20120302: SocIPC Transmitter Packet Global Variable*/
SocIPCTransPkt_t gSoCIPCTransPacket ={
	SOCIPC_DEVICEL_DST_ADDRESS,	//DestDevSlaveAddr
	SOCIPC_TX_SRC_ADDRESS,		//SrcDevMasterAddr
	0x80,						//TransPacketLength
	SOC_IPC_TX_VCP_PREFIX_CMD,	//VCPPrefixCmd		
	(BYTE)((SOC_IPC_TX_VCP_PREFIX_CMD_PAGE>>8)& 0x00FF),//VCPPrefixPage1
	(BYTE)(SOC_IPC_TX_VCP_PREFIX_CMD_PAGE & 0x00FF),	//VCPPrefixPage2
	0x00,						//MessageLength
	0x00,						//MessageCmd
	{0},						//MessageBuf[SOCIPC_TRANS_MSG_MAX_SIZE]
	0x00						//PacketChkSum
};

BOOL gGetAckFromSlave = FALSE;
//BOOL Send_Flag = 0;
BYTE Change_Flag = 0;


// 20120418
BYTE B_Brightness = 0;
WORD B_Contrast = 256;
BYTE B_Sharpness = 12;
BYTE B_AudioVolume = 14;
BYTE B_AudioSettings = 0;

/*ChengLin 20130128: New Sync Handler State Control*/	
gmt_ImageSyncState gCurrentAthenaRIGHT_SyncState = IMAGE_LVDS_ON_STATE;
gmt_ImageSyncState gCurrentAthenaLEFT_FE_SyncState = IMAGE_LVDS_ON_STATE;
/*********************************************/

//******************************************************************************
//  S T A T I C    V A R I A B L E S
//******************************************************************************

//******************************************************************************
//  S T A T I C    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

//******************************************************************************
//  C O D E
//******************************************************************************

//******************************************************************************
//
// FUNCTION       : SoCIPC_SlavDevCmdParser
//
// USAGE          : Athena-LEFT/FE commands parser, used for SocIPC commands
//
// INPUT          : B_Command - SocIPC command number
//                      B_whichHandler - communication channel being used
//
// OUTPUT         : PARSER STATUS
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
PARSER_STATUS_t SoCIPC_SlavDevCmdParser(BYTE far* gExtDebugCmdPtr, HANDLER_USE_t B_whichHandler)
{
	PARSER_STATUS_t PS_Status = MSG_PARSED;
	BYTE SendDataType = RECV_NACK; //ChengLin: system must to parser commands then send out messages for I2C Master to read.

	SOCIPC_SlaveUpdateStatus_t SOCIPC_Slave_UpdateFlag ={
	CT_USER_NONE, // Update_CT_User_Index
	FALSE, // Update_PortControl
	FALSE, // Update_Color
	FALSE, // Update_Sharpness
	FALSE, // Update_ColorTemperature
	FALSE, // Update_ImageScheme
	FALSE, // Update_CSC
	FALSE, // Update_DUM
	FALSE, // Update_DCDI
	FALSE, // Update_MADI
	FALSE, // Update_OD
	FALSE, // Update_TNR
	FALSE, // Update_MEPG_NR
	FALSE, // Update_FilmMode
	FALSE, // Update_AspectRatio
	FALSE, // Update_DCR
	FALSE, // Update_FactoryReset
	FALSE, // Update_DPSwitch
	FALSE, // Update_UserData
	FALSE, // Update_DDCCI
	SIX_AXIS_COLOR_NONE, // Update_SixAxisColor_Index
/*ChengLin 20130222: Add for sync Left/Right PB Control Status*/
#if ENABLE_ATHENA_R_L_IMAGE_SYNC_HANDLER	
	FALSE, //Update_PB_Ctrl
#endif
   FALSE, //Update_GammaCorrection
   FALSE, //Update_SplashLoadDef    
   FALSE, //Update_Redo_ModeSetup  
	};

#if ENABLE_SOC_IPC_BUSY_CHECK
/*ChengLin 20130124: New 4k2k board BUSY status check*/
	if(GetChipID_4K2K() == CHIPID_4K2K_L)
		SocIPCSetClearSlaveLeftBusyStatus(TRUE);
	else if(GetChipID_4K2K() == CHIPID_4K2K_FE)
		SocIPCSetClearSlaveFEBusyStatus(TRUE);		
#endif
	
   switch (*(gExtDebugCmdPtr+1))
   {
   
	case SICMD_RW_Power_CONTROL:
		//Command code from Athena R; 00: Normal; 01: B off; 10/11: Reserved
		//ChengLin 20120109: Here for control Athena LEFT/FE AC power, below coding just for test only, we will modify it later
		{
			BYTE PowerCtrl = (((PowerCtrlFuncCmd_t*)gExtDebugCmdPtr)->Power_Ctrl)&0x03;
			if( (PowerCtrl&0x03) == 0x00 )
			{	
				socipc_recv_msg("\nPower Control - Normal.\n",0);
				_fmemcpy((BYTE far*)gExtDebugCmdPtr, (BYTE far*)IPC_AckPakt, 3);
				SocIPCI2CSlaveWriteCmdPkt((BYTE far*)gExtDebugCmdPtr , B_whichHandler-I2C_SLAVE1_USE);	
				PowerRequestOn(); //Test function only!!
				SendDataType = RECV_NOTHING;	

#if ENABLE_SOC_IPC_BUSY_CHECK
				/*ChengLin 20130124: New 4k2k board BUSY status check*/
				if(GetChipID_4K2K() == CHIPID_4K2K_L)
					SocIPCSetClearSlaveLeftBusyStatus(FALSE);
				else if(GetChipID_4K2K() == CHIPID_4K2K_FE)
					SocIPCSetClearSlaveFEBusyStatus(FALSE);	
#endif
				return PS_Status;				
			}
			else if( (PowerCtrl&0x03) == 0x01 )
			{		
				socipc_recv_msg("\nPower Control - Power Off.\n",0);
				_fmemcpy((BYTE far*)gExtDebugCmdPtr, (BYTE far*)IPC_AckPakt, 3);
				SocIPCI2CSlaveWriteCmdPkt((BYTE far*)gExtDebugCmdPtr , B_whichHandler-I2C_SLAVE1_USE);	
				PowerRequestOff(); //Test function only!!
				SendDataType = RECV_NOTHING;	

#if ENABLE_SOC_IPC_BUSY_CHECK
				/*ChengLin 20130124: New 4k2k board BUSY status check*/
				if(GetChipID_4K2K() == CHIPID_4K2K_L)
					SocIPCSetClearSlaveLeftBusyStatus(FALSE);
				else if(GetChipID_4K2K() == CHIPID_4K2K_FE)
					SocIPCSetClearSlaveFEBusyStatus(FALSE); 
#endif
				return PS_Status;					
			}
			else
			{			
				socipc_recv_msg("\nPower Control - Reserved.\n",0);
				SendDataType = RECV_NACK;	
			}
			
		}			
		break;
	case SICMD_RO_Power_Status:
		//Responses from Athena LEFT/FE; 00: Normal; 01: B off; 10/11: Reserved
		//ChengLin 20120109: Below coding just for test only, we will modify it later.
		if( PwrState == Pwr_On )
		{
			((PowerStatusResPkt_t fdata*)gExtDebugCmdPtr)->Power_Status = 0x00;
		}
		else
		{
			((PowerStatusResPkt_t fdata*)gExtDebugCmdPtr)->Power_Status = 0x01;
		}
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		break;
		
/*ChengLin 20130110: Color Temp - User Settings*/
	case SICMD_RW_CT_RedGain_Ctrl:
		gAthena_RIGHT_Master_Parameters.CTUserRGain= ((((CTUserRGainCtrlFuncCmd_t*)gExtDebugCmdPtr)->CT_User_RGain_HighByte)<<8)|(((CTUserRGainCtrlFuncCmd_t*)gExtDebugCmdPtr)->CT_User_RGain_LowByte);			
		socipc_recv_msg("\nAthena RIGHT CT User Red Gain = %d.\n",gAthena_RIGHT_Master_Parameters.CTUserRGain);	
		UserPrefUserRedGain = gAthena_RIGHT_Master_Parameters.CTUserRGain;
		SOCIPC_Slave_UpdateFlag.Update_CT_User_Index = CT_USER_RED_GAIN;
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));
		break;
	case SICMD_RW_CT_GreenGain_Ctrl:
		gAthena_RIGHT_Master_Parameters.CTUserGGain= ((((CTUserGGainCtrlFuncCmd_t*)gExtDebugCmdPtr)->CT_User_GGain_HighByte)<<8)|(((CTUserGGainCtrlFuncCmd_t*)gExtDebugCmdPtr)->CT_User_GGain_LowByte);			
		socipc_recv_msg("\nAthena RIGHT CT User Green Gain = %d.\n",gAthena_RIGHT_Master_Parameters.CTUserGGain);	
		UserPrefUserGreenGain = gAthena_RIGHT_Master_Parameters.CTUserGGain;
		SOCIPC_Slave_UpdateFlag.Update_CT_User_Index = CT_USER_GREEN_GAIN;		
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));
		break;
	case SICMD_RW_CT_BlueGain_Ctrl:
		gAthena_RIGHT_Master_Parameters.CTUserBGain= ((((CTUserBGainCtrlFuncCmd_t*)gExtDebugCmdPtr)->CT_User_BGain_HighByte)<<8)|(((CTUserBGainCtrlFuncCmd_t*)gExtDebugCmdPtr)->CT_User_BGain_LowByte);			
		socipc_recv_msg("\nAthena RIGHT CT User Blue Gain = %d.\n",gAthena_RIGHT_Master_Parameters.CTUserBGain);	
		UserPrefUserBlueGain = gAthena_RIGHT_Master_Parameters.CTUserBGain;
		SOCIPC_Slave_UpdateFlag.Update_CT_User_Index = CT_USER_BLUE_GAIN;		
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));
		break;
	case SICMD_RW_CT_RedOffset_Ctrl:
		gAthena_RIGHT_Master_Parameters.CTUserROffset = (SBYTE)(((CTUserROffsetCtrlFuncCmd_t*)gExtDebugCmdPtr)->CT_User_ROffset);			
		socipc_recv_msg("\nAthena RIGHT CT User Red Offset = 0x%X.\n",(BYTE)gAthena_RIGHT_Master_Parameters.CTUserROffset);
		UserPrefUserRedOff = gAthena_RIGHT_Master_Parameters.CTUserROffset;
		SOCIPC_Slave_UpdateFlag.Update_CT_User_Index = CT_USER_RED_OFFSET;	
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		break;
	case SICMD_RW_CT_GreenOffset_Ctrl:
		gAthena_RIGHT_Master_Parameters.CTUserGOffset = (SBYTE)(((CTUserGOffsetCtrlFuncCmd_t*)gExtDebugCmdPtr)->CT_User_GOffset);			
		socipc_recv_msg("\nAthena RIGHT CT User Green Offset = 0x%X.\n",(BYTE)gAthena_RIGHT_Master_Parameters.CTUserGOffset);
		UserPrefUserGreenOff = gAthena_RIGHT_Master_Parameters.CTUserGOffset;
		SOCIPC_Slave_UpdateFlag.Update_CT_User_Index = CT_USER_GREEN_OFFSET;		
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		break;
	case SICMD_RW_CT_BlueOffset_Ctrl:
		gAthena_RIGHT_Master_Parameters.CTUserBOffset = (SBYTE)(((CTUserBOffsetCtrlFuncCmd_t*)gExtDebugCmdPtr)->CT_User_BOffset);			
		socipc_recv_msg("\nAthena RIGHT CT User Blue Offset = 0x%X.\n",(BYTE)gAthena_RIGHT_Master_Parameters.CTUserBOffset);
		UserPrefUserBlueOff = gAthena_RIGHT_Master_Parameters.CTUserBOffset;
		SOCIPC_Slave_UpdateFlag.Update_CT_User_Index = CT_USER_BLUE_OFFSET;	
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		break;
/*****************************************/

	case SICMD_RW_Cables_Status:
		//Update cable status from Athena R to LEFT / FE		
		gAthena_RIGHT_Master_Parameters.CableStatus = (((CableStatusFuncCmd_t*)gExtDebugCmdPtr)->CableStatus);		
		socipc_recv_msg("\nAthena RIGHT Cable Status = 0x%X.\n",gAthena_RIGHT_Master_Parameters.CableStatus);
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		break;
	case SICMD_RW_Mode_Info_CONTRL:
		//Command code from Athena R and Reply code form Athena LEFT and FE
		gAthena_RIGHT_Master_Parameters.ModeFlags = ((((ModeInfoCtrlFuncCmd_t*)gExtDebugCmdPtr)->ModeInfo_HighByte)<<8)|(((ModeInfoCtrlFuncCmd_t*)gExtDebugCmdPtr)->ModeInfo_LowByte);		
		socipc_recv_msg("\nAthena RIGHT Mode Flags = 0x%X.\n",gAthena_RIGHT_Master_Parameters.ModeFlags);
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));
		break;
	case SICMD_RW_Mode_Info_STATUS:
		//Get Athena LEFT/FE Mode Info Status
		((ModeInfoStatusResPkt_t fdata*)gExtDebugCmdPtr)->ModeInfo_HighByte = (BYTE)((stInput[CH_A].ModeFlags>>8)&0x00FF);
		((ModeInfoStatusResPkt_t fdata*)gExtDebugCmdPtr)->ModeInfo_LowByte = (BYTE)(stInput[CH_A].ModeFlags&0x00FF);		
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));				
		break;
	case SICMD_RW_Timing_H_Width:
		gAthena_RIGHT_Master_Parameters.HWidth = ((((TimingHWidthCtrlFuncCmd_t*)gExtDebugCmdPtr)->HWidth_HighByte)<<8)|(((TimingHWidthCtrlFuncCmd_t*)gExtDebugCmdPtr)->HWidth_LowByte);			
		socipc_recv_msg("\nAthena RIGHT H Width = %d.\n",gAthena_RIGHT_Master_Parameters.HWidth);
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));				
		break;
	case SICMD_RW_Timing_V_Length:
		gAthena_RIGHT_Master_Parameters.VLength = ((((TimingVLengthCtrlFuncCmd_t*)gExtDebugCmdPtr)->VLength_HighByte)<<8)|(((TimingVLengthCtrlFuncCmd_t*)gExtDebugCmdPtr)->Vength_LowByte);			
		socipc_recv_msg("\nAthena RIGHT V Length = %d.\n",gAthena_RIGHT_Master_Parameters.VLength);		
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));				
		break;
	case SICMD_RW_Timing_H_Total:
		gAthena_RIGHT_Master_Parameters.HTotal = ((((TimingHTotalCtrlFuncCmd_t*)gExtDebugCmdPtr)->HTotal_HighByte)<<8)|(((TimingHTotalCtrlFuncCmd_t*)gExtDebugCmdPtr)->HTotal_LowByte);			
		socipc_recv_msg("\nAthena RIGHT H Total = %d.\n",gAthena_RIGHT_Master_Parameters.HTotal);		
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		break;
	case SICMD_RW_Timing_V_Total:
		gAthena_RIGHT_Master_Parameters.VTotal = ((((TimingVTotalCtrlFuncCmd_t*)gExtDebugCmdPtr)->VTotal_HighByte)<<8)|(((TimingVTotalCtrlFuncCmd_t*)gExtDebugCmdPtr)->VTotal_LowByte);			
		socipc_recv_msg("\nAthena RIGHT V Total = %d.\n",gAthena_RIGHT_Master_Parameters.VTotal);		
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		break;
	case SICMD_RW_Timing_H_Start:
		gAthena_RIGHT_Master_Parameters.HStart = ((((TimingHStartCtrlFuncCmd_t*)gExtDebugCmdPtr)->HStart_HighByte)<<8)|(((TimingHStartCtrlFuncCmd_t*)gExtDebugCmdPtr)->HStart_LowByte);		
		socipc_recv_msg("\nAthena RIGHT H Start = %d.\n",gAthena_RIGHT_Master_Parameters.HStart);		
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		break;
	case SICMD_RW_Timing_V_Start:
		gAthena_RIGHT_Master_Parameters.VStart = ((((TimingVStartCtrlFuncCmd_t*)gExtDebugCmdPtr)->VStart_HighByte)<<8)|(((TimingVStartCtrlFuncCmd_t*)gExtDebugCmdPtr)->VStart_LowByte);			
		socipc_recv_msg("\nAthena RIGHT V Start = %d.\n",gAthena_RIGHT_Master_Parameters.VStart);		
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		break;
	case SICMD_RO_Timing_H_Width_Slave:
		//Get Athena LEFT/FE H Active
		((TimingHWidthSlaveResPkt_t fdata*)gExtDebugCmdPtr)->HWidth_HighByte = (BYTE)((stInput[CH_A].Hactive>>8)&0x00FF);
		((TimingHWidthSlaveResPkt_t fdata*)gExtDebugCmdPtr)->HWidth_LowByte = (BYTE)(stInput[CH_A].Hactive&0x00FF);		
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		break;
	case SICMD_RO_Timing_V_Length_Slave:
		//Get Athena LEFT/FE V Active
		((TimingVLengthSlaveResPkt_t fdata*)gExtDebugCmdPtr)->VLength_HighByte = (BYTE)((stInput[CH_A].Vactive>>8)&0x00FF);
		((TimingVLengthSlaveResPkt_t fdata*)gExtDebugCmdPtr)->VLength_LowByte = (BYTE)(stInput[CH_A].Vactive&0x00FF);		
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		break;
	case SICMD_RO_Timing_H_Total_Slave:
		//Get Athena LEFT/FE H Total
		((TimingHTotalSlaveResPkt_t fdata*)gExtDebugCmdPtr)->HTotal_HighByte = (BYTE)((stInput[CH_A].Htotal>>8)&0x00FF);
		((TimingHTotalSlaveResPkt_t fdata*)gExtDebugCmdPtr)->HTotal_LowByte = (BYTE)(stInput[CH_A].Htotal&0x00FF);		
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));		
		break;
	case SICMD_RO_Timing_V_Total_Slave:
		//Get Athena LEFT/FE V Total
		((TimingVTotalSlaveResPkt_t fdata*)gExtDebugCmdPtr)->VTotal_HighByte = (BYTE)((stInput[CH_A].Vtotal>>8)&0x00FF);
		((TimingVTotalSlaveResPkt_t fdata*)gExtDebugCmdPtr)->VTotal_LowByte = (BYTE)(stInput[CH_A].Vtotal&0x00FF);		
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));		
		break;
	case SICMD_RO_Timing_H_Start_Slave:
		//Get Athena LEFT/FE H Start
		((TimingHStartSlaveResPkt_t fdata*)gExtDebugCmdPtr)->HStart_HighByte = (BYTE)((stInput[CH_A].Hstart>>8)&0x00FF);
		((TimingHStartSlaveResPkt_t fdata*)gExtDebugCmdPtr)->HStart_LowByte = (BYTE)(stInput[CH_A].Hstart&0x00FF);		
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		break;
	case SICMD_RO_Timing_V_Start_Slave:
		//Get Athena LEFT/FE V Start
		((TimingVStartSlaveResPkt_t fdata*)gExtDebugCmdPtr)->VStart_HighByte = (BYTE)((stInput[CH_A].Vstart>>8)&0x00FF);
		((TimingVStartSlaveResPkt_t fdata*)gExtDebugCmdPtr)->VStart_LowByte = (BYTE)(stInput[CH_A].Vstart&0x00FF);		
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		break;
	case SICMD_RO_Timing_V_Freq_Slave:
		gm_Print("stInput[CH_A].Vfrq ........... %d", stInput[CH_A].Vfrq);
		((TimingVFreqSlaveResPkt_t fdata*)gExtDebugCmdPtr)->VFreq_HighByte = (BYTE)((stInput[CH_A].Vfrq>>8)&0x00FF);
		((TimingVFreqSlaveResPkt_t fdata*)gExtDebugCmdPtr)->VFreq_LowByte = (BYTE)(stInput[CH_A].Vfrq&0x00FF);
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		break;
	case SICMD_RW_Port_CONTRL:
		gAthena_RIGHT_Master_Parameters.PortStatus = (((PortCtrlFuncCmd_t*)gExtDebugCmdPtr)->PortCtrlBits);	
		socipc_recv_msg("\nAthena RIGHT Port Status = %d.\n",gAthena_RIGHT_Master_Parameters.PortStatus);
		SOCIPC_Slave_UpdateFlag.Update_PortControl = TRUE;
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));
		break;
	case SICMD_RO_Port_Status:
		//Get Athena LEFT/FE Port Status
		((PortStatusResPkt_t fdata*)gExtDebugCmdPtr)->PortStatus = gmvb_InputConnector[CH_A];	
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));			
		break;
	case SICMD_RW_Signal_Sync_CONTRL:
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));			
	  	PS_Status = UNKNOWN_MSG_CMD;	
		break;
	case SICMD_RO_Signal_Sync_Status:
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));			
	  	PS_Status = UNKNOWN_MSG_CMD;	
		break;
	case SICMD_RW_Brightness_CONTRL:
		gAthena_RIGHT_Master_Parameters.Brightness = (SBYTE)(((BrightnessCtrlFuncCmd_t*)gExtDebugCmdPtr)->Brightness);			
		socipc_recv_msg("\nAthena RIGHT Brightness = 0x%X.\n",(BYTE)gAthena_RIGHT_Master_Parameters.Brightness);
		UserPrefBrightness = gAthena_RIGHT_Master_Parameters.Brightness;
		SOCIPC_Slave_UpdateFlag.Update_Color = TRUE;		
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		break;
	case SICMD_RO_Brightness_Status:
		//Get Athena LEFT/FE Brightness
		//Range : -108~+107
		((BrightnessStatusResPkt_t fdata*)gExtDebugCmdPtr)->Brightness = (BYTE)UserPrefBrightness;			
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));		
		break;
	case SICMD_RW_Contrast_CONTRL:
		gAthena_RIGHT_Master_Parameters.Contrast = ((((ContrastCtrlFuncCmd_t*)gExtDebugCmdPtr)->Contrast_HighByte)<<8)|(((ContrastCtrlFuncCmd_t*)gExtDebugCmdPtr)->Contrast_LowByte);			
		socipc_recv_msg("\nAthena RIGHT Contrast = %d.\n",gAthena_RIGHT_Master_Parameters.Contrast);	
		UserPrefContrast = gAthena_RIGHT_Master_Parameters.Contrast;
		SOCIPC_Slave_UpdateFlag.Update_Color = TRUE;	
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));
		break;
	case SICMD_RO_Contrast_Status:
		//Get Athena LEFT/FE Contrast Setting
		((ContrastStatusResPkt_t fdata*)gExtDebugCmdPtr)->Contrast_HighByte = (BYTE)((UserPrefContrast>>8)&0x00FF);
		((ContrastStatusResPkt_t fdata*)gExtDebugCmdPtr)->Contrast_LowByte = (BYTE)(UserPrefContrast&0x00FF);		
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));
		break;
	case SICMD_RW_Sharpness_CONTRL:
		gAthena_RIGHT_Master_Parameters.Sharpness = ((SharpnessCtrlFuncCmd_t*)gExtDebugCmdPtr)->Sharpness;			
		socipc_recv_msg("\nAthena RIGHT Sharpness = %d.\n",gAthena_RIGHT_Master_Parameters.Sharpness);
		
		/*ChengLin 20120216: Fix Sharpness can't set/get correct value issue.*/
		SocIPCSetCorrectSharpnessSetting(gAthena_RIGHT_Master_Parameters.Sharpness);
		SOCIPC_Slave_UpdateFlag.Update_Sharpness = TRUE;
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		break;
	case SICMD_RO_Sharpness_Status:
		//Get Athena LEFT/FE Sharpness Setting
		/*ChengLin 20120216: Fix Sharpness can't set/get correct value issue.*/
		((SharpnessStatusResPkt_t fdata*)gExtDebugCmdPtr)->Sharpness = SocIPCGetCorrectSharpnessSetting();
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		break;
	case SICMD_RW_Color_CONTRL:
		{										  
			gAthena_RIGHT_Master_Parameters.ColorSettings = ((((ColorCtrlFuncCmd_t*)gExtDebugCmdPtr)->ColorSettings_HighByte)<<8)|(((ColorCtrlFuncCmd_t*)gExtDebugCmdPtr)->ColorSettings_LowByte);			
			socipc_recv_msg("\nAthena RIGHT Color Settings = 0x%X.\n",gAthena_RIGHT_Master_Parameters.ColorSettings);	

			//Send Athena RIGHT Color related settings to Athena LEFT/FE (Base on current OSD Layout)
			//0:3	Color temp control bits 			000: 6500K; 001: 7500K; 010: 9300K; 011: User; 100/101/110/111: Reserved
			//4:7	Theme mode control bits 		000: Normal; 001: Vivid; 010: Cinema; 011: Game; 100:Sport ;101/110/111: Reserved
			//8:11	Color space control bits 			000: sRGB; 001: Adobe98; 010: xvYCC; 011:DICOM;  100/101/110/111: Reserved
			//12:15  DUM Switch control bits			000: Off; 001: On; 010~111: Reserved

		/*ChengLin 20130114: Check Athena L/FE current CT settings and compare with Athena R inputs*/
			if( (((gAthena_RIGHT_Master_Parameters.ColorSettings & 0x000F) == 0) && (UserPrefColorTemp != CTEMP_6500K)) || \
				(((gAthena_RIGHT_Master_Parameters.ColorSettings & 0x000F) == 1) && (UserPrefColorTemp != CTEMP_7500K)) || \
				(((gAthena_RIGHT_Master_Parameters.ColorSettings & 0x000F) == 2) && (UserPrefColorTemp != CTEMP_9300K)) || \
				(((gAthena_RIGHT_Master_Parameters.ColorSettings & 0x000F) == 3) && (UserPrefColorTemp != USER_TEMP)))
			{
					if((gAthena_RIGHT_Master_Parameters.ColorSettings & 0x000F) == 0)
						UserPrefColorTemp = CTEMP_6500K;
					else if((gAthena_RIGHT_Master_Parameters.ColorSettings & 0x000F) == 1)
						UserPrefColorTemp = CTEMP_7500K;
					else if((gAthena_RIGHT_Master_Parameters.ColorSettings & 0x000F) == 2)
						UserPrefColorTemp = CTEMP_9300K;							
					else if((gAthena_RIGHT_Master_Parameters.ColorSettings & 0x000F) == 3)
						UserPrefColorTemp = USER_TEMP;
					else
						UserPrefColorTemp = CTEMP_6500K;	  

					SOCIPC_Slave_UpdateFlag.Update_ColorTemperature = TRUE;		
			}
		
		/*ChengLin 20130114: Check Athena L/FE current Image Scheme settings and compare with Athena R inputs*/										  
			if(((SBYTE)((gAthena_RIGHT_Master_Parameters.ColorSettings & 0x00F0)>>4)) != UserPrefImageScheme)
			{
				UserPrefImageScheme = (SBYTE)((gAthena_RIGHT_Master_Parameters.ColorSettings & 0x00F0)>>4);
				SOCIPC_Slave_UpdateFlag.Update_ImageScheme = TRUE;
			}
		
		/*ChengLin 20130114: Check Athena L/FE current Color Space settings and compare with Athena R inputs*/										  
			if(((BYTE)((gAthena_RIGHT_Master_Parameters.ColorSettings & 0x0F00)>>8)) != UserPrefColorSpace)
			{
				UserPrefColorSpace = (BYTE)((gAthena_RIGHT_Master_Parameters.ColorSettings & 0x0F00)>>8);
				SOCIPC_Slave_UpdateFlag.Update_CSC = TRUE;
			}
		
		/*ChengLin 20130114: Check Athena L/FE current DUM settings and compare with Athena R inputs*/							
			if(((SBYTE)((gAthena_RIGHT_Master_Parameters.ColorSettings & 0xF000)>>12)) != UserPrefDUMSwitch)
			{
				UserPrefDUMSwitch = (SBYTE)((gAthena_RIGHT_Master_Parameters.ColorSettings & 0xF000)>>12);
				SOCIPC_Slave_UpdateFlag.Update_DUM = TRUE;
			}
			SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));		
		}
		break;
	case SICMD_RO_Color_Status:
		//Get Athena LEFT/RIGHT Color Settings
		{
			//0:3	Color temp control bits 			000: 6500K; 001: 7500K; 010: 9300K; 011: User; 100/101/110/111: Reserved
			//4:7	Theme mode control bits 		000: Normal; 001: Vivid; 010: Cinema; 011: Game; 100:Sport ;101/110/111: Reserved
			//8:11	Color space control bits 			000: sRGB; 001: Adobe98; 010: xvYCC; 011:DICOM;  100/101/110/111: Reserved
			//12:15  DUM Switch control bits			000: Off; 001: On; 010~111: Reserved		
						
			WORD ColorSettings = 0;
			BYTE cColorTempNewIndex = 0;
			
			if(UserPrefColorTemp == CTEMP_6500K)
				cColorTempNewIndex = 0;
			else if(UserPrefColorTemp == CTEMP_7500K)
				cColorTempNewIndex = 1;
			else if(UserPrefColorTemp == CTEMP_9300K)
				cColorTempNewIndex = 2;
			else if(UserPrefColorTemp == USER_TEMP)
				cColorTempNewIndex = 3;
			else
				cColorTempNewIndex = 0x0F;	//Mismatch case 		
			ColorSettings = (((UserPrefDUMSwitch&0x0F)<<12)|((UserPrefColorSpace&0x0F)<<8)|((UserPrefImageScheme&0x0F)<<4)|(cColorTempNewIndex&0x0F));

			((ColorStatusResPkt_t fdata*)gExtDebugCmdPtr)->ColorSettings_HighByte = (BYTE)((ColorSettings>>8)&0x00FF);
			((ColorStatusResPkt_t fdata*)gExtDebugCmdPtr)->ColorSettings_LowByte = (BYTE)(ColorSettings&0x00FF);		
		 	SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));			
		}
		break;
	case SICMD_RW_Video_progress_CONTRL:
		{
			gAthena_RIGHT_Master_Parameters.VideoProgress = ((((VideoProgressCtrlFuncCmd_t*)gExtDebugCmdPtr)->VideoProgress_HighByte)<<8)|(((VideoProgressCtrlFuncCmd_t*)gExtDebugCmdPtr)->VideoProgress_LowByte);			
			socipc_recv_msg("\nAthena RIGHT Video Progress = 0x%X.\n",gAthena_RIGHT_Master_Parameters.VideoProgress);	

			//Send Video Progress Settings to Athena LEFT/FE (Base on current OSD layout)
			//Video_progress_CONTRL
			//0:3	DCDI control bits	000: off; 001: On; 010/011/100/101/110/111: Reserved
			//4:7	MADI control bits	000: off; 001: Adaptive; 010: Normal; 011/100/101/110/111: Reserved
			//8:11	OD control bits 	000: off; 001: Medium; 010: Low; 011: High; 100/101/110/111: Reserved
			//12:15  Reserved

			/*ChengLin 20130115: Check Athena L/FE current DCDI settings and compare with Athena R inputs*/
			if(((SBYTE)(gAthena_RIGHT_Master_Parameters.VideoProgress & 0x000F)) != UserPrefDCDiMain)
			{
				UserPrefDCDiMain = (SBYTE)(gAthena_RIGHT_Master_Parameters.VideoProgress & 0x000F);
				SOCIPC_Slave_UpdateFlag.Update_DCDI = TRUE;
			}

			/*ChengLin 20130115: Check Athena L/FE current MADI settings and compare with Athena R inputs*/
			if(((SBYTE)((gAthena_RIGHT_Master_Parameters.VideoProgress & 0x00F0)>>4)) != UserPrefMADI)
			{
				UserPrefMADI = (SBYTE)((gAthena_RIGHT_Master_Parameters.VideoProgress & 0x00F0)>>4);
				SOCIPC_Slave_UpdateFlag.Update_MADI = TRUE;
			}

			/*ChengLin 20130115: Check Athena L/FE current OD settings and compare with Athena R inputs*/
			if(((BYTE)((gAthena_RIGHT_Master_Parameters.VideoProgress & 0x0F00)>>8)) != UserPrefLCDOverdrive)
			{
				UserPrefLCDOverdrive = (BYTE)((gAthena_RIGHT_Master_Parameters.VideoProgress & 0x0F00)>>8);
				SOCIPC_Slave_UpdateFlag.Update_OD = TRUE;
			}
			SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));
		}
		break;
	case SICMD_RO_Video_progress_Status:
		//Get Athena LEFT/FE Video Progress Settings
		{
			//0:3	DCDI control bits	000: off; 001: On; 010/011/100/101/110/111: Reserved
			//4:7	MADI control bits	000: off; 001: Adaptive; 010: Normal; 011/100/101/110/111: Reserved
			//8:11	OD control bits 	000: off; 001: Medium; 010: Low; 011: High; 100/101/110/111: Reserved
			//12:15  Reserved
			
			WORD VideoProgress = 0;
		
			VideoProgress = (((UserPrefLCDOverdrive&0x0F)<<8)|((UserPrefMADI&0x0F)<<4)|(UserPrefDCDiMain&0x0F));

			((VideoProgressStatusResPkt_t fdata*)gExtDebugCmdPtr)->VideoProgress_HighByte = (BYTE)((VideoProgress>>8)&0x00FF);
			((VideoProgressStatusResPkt_t fdata*)gExtDebugCmdPtr)->VideoProgress_LowByte = (BYTE)(VideoProgress&0x00FF);		
		 	SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));			
		}
		break;
	case SICMD_RW_NR_CONTRL:
		{
			gAthena_RIGHT_Master_Parameters.NRSettings = ((((NRCtrlFuncCmd_t*)gExtDebugCmdPtr)->NR_HighByte)<<8)|(((NRCtrlFuncCmd_t*)gExtDebugCmdPtr)->NR_LowByte);			
			socipc_recv_msg("\nAthena RIGHT NR Settings = 0x%X.\n",gAthena_RIGHT_Master_Parameters.NRSettings);		

			//Send NR Settings to Athena LEFT/FE (Base on current OSD layout)			
			//NR_CONTRL
			//0:3	Dynamic NR control bits 	000: off; 001: Adaptive; 010: Low; 011: Medium; 100: High; 101/110/111: Reserved
			//4:7	MPEG NR control bits		000: off; 001: On; 010/011/100/101/110/111: Reserved

			/*ChengLin 20130115: Check Athena L/FE current TNR settings and compare with Athena R inputs*/
			if(((SBYTE)(gAthena_RIGHT_Master_Parameters.NRSettings & 0x000F)) != UserPrefTNR)
			{
				UserPrefTNR = (SBYTE)(gAthena_RIGHT_Master_Parameters.NRSettings & 0x000F);
				SOCIPC_Slave_UpdateFlag.Update_TNR = TRUE;
			}

			/*ChengLin 20130115: Check Athena L/FE current MPEG NR settings and compare with Athena R inputs*/
			if(((BYTE)((gAthena_RIGHT_Master_Parameters.NRSettings & 0x00F0)>>4)) != UserPrefMPEGNRMode)
			{
				UserPrefMPEGNRMode = (BYTE)((gAthena_RIGHT_Master_Parameters.NRSettings & 0x00F0)>>4);
				SOCIPC_Slave_UpdateFlag.Update_MEPG_NR = TRUE;
			}
			SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		}
		break;
	case SICMD_RO_NR_Status:
		//Get Athena LEFT/FE NR Settings
		{
			//0:3	Dynamic NR control bits 	000: off; 001: Adaptive; 010: Low; 011: Medium; 100: High; 101/110/111: Reserved
			//4:7	MPEG NR control bits		000: off; 001: On; 010/011/100/101/110/111: Reserved
						
			WORD NRStatus = 0;			
						
			NRStatus = (((UserPrefMPEGNRMode&0x0F)<<4)|(UserPrefTNR&0x0F));
							
			((NRStatusResPkt_t fdata*)gExtDebugCmdPtr)->NR_HighByte = (BYTE)((NRStatus>>8)&0x00FF);
			((NRStatusResPkt_t fdata*)gExtDebugCmdPtr)->NR_LowByte = (BYTE)(NRStatus&0x00FF);		
		 	SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));								
		}
		break;
	case SICMD_RW_Video_progress1_CONTRL:
		{
			gAthena_RIGHT_Master_Parameters.VideoProgress_1 = ((((VideoProgress_1CtrlFuncCmd_t*)gExtDebugCmdPtr)->VideoProgress_1_HighByte)<<8)|(((VideoProgress_1CtrlFuncCmd_t*)gExtDebugCmdPtr)->VideoProgress_1_LowByte);			
			socipc_recv_msg("\nAthena RIGHT Video Progress_1 Settings = 0x%X.\n",gAthena_RIGHT_Master_Parameters.VideoProgress_1);		

			//Send Video Progress_1 Settings to Athena LEFT/FE (Base on current OSD layout)
			//0:3	Film mode detect	000: off; 001: Video-3:2-2:2; 010: Video-3:2; 011: Video-2:2; 100/101/110/111: Reserved 
			//4:7	Reserved	

			/*ChengLin 20130115: Check Athena L/FE current Film Mode settings and compare with Athena R inputs*/
			if(((SBYTE)(gAthena_RIGHT_Master_Parameters.VideoProgress_1 & 0x000F)) != UserPrefFilmMode)
			{
				UserPrefFilmMode = (SBYTE)(gAthena_RIGHT_Master_Parameters.VideoProgress_1 & 0x000F);
				SOCIPC_Slave_UpdateFlag.Update_FilmMode = TRUE;				
			}

			SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		}
		break;
	case SICMD_RO_Video_progress1_Status:
		//Get Athena LEFT/FE Video Progress_1 Settings			
		{
			//0:3	Film mode detect	000: off; 001: Video-3:2-2:2; 010: Video-3:2; 011: Video-2:2; 100/101/110/111: Reserved 
			//4:7	Reserved			
			WORD VideoProgress_1 = 0;			
				
			VideoProgress_1 = (UserPrefFilmMode&0x0F);

			((VideoProgress_1StatusResPkt_t fdata*)gExtDebugCmdPtr)->VideoProgress_1_HighByte = (BYTE)((VideoProgress_1>>8)&0x00FF);
			((VideoProgress_1StatusResPkt_t fdata*)gExtDebugCmdPtr)->VideoProgress_1_LowByte = (BYTE)(VideoProgress_1&0x00FF);		
		 	SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));				
		}	
		break;
	case SICMD_RW_DisplaySetup_CONTRL:
		{
			gAthena_RIGHT_Master_Parameters.DisplaySetup = ((((DisplaySetupCtrlFuncCmd_t*)gExtDebugCmdPtr)->DisplaySetup_HighByte)<<8)|(((DisplaySetupCtrlFuncCmd_t*)gExtDebugCmdPtr)->DisplaySetup_LowByte);			
			socipc_recv_msg("\nAthena RIGHT Display Setup Settings = 0x%X.\n",gAthena_RIGHT_Master_Parameters.DisplaySetup);		

			//Send Display Setup Settings to Athena LEFT/FE (Base on current OSD layout)

			/*ChengLin 20130115: Check Athena L/FE current Aspect Ratio settings and compare with Athena R inputs*/
         if((gAthena_RIGHT_Master_Parameters.DisplaySetup & 0x000F) != UserPrefOutputAspectRatio)
			{
            UserPrefOutputAspectRatio = gAthena_RIGHT_Master_Parameters.DisplaySetup & 0x000F;
				SOCIPC_Slave_UpdateFlag.Update_AspectRatio = (BOOL)((DisplaySetupCtrlFuncCmd_t*)gExtDebugCmdPtr)->UpdateFlag; //ChengLin 20130315: Special for Splash screen					
			}

			SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
         SOCIPC_Slave_UpdateFlag.Update_UserData = TRUE;
		}
		break;
	case SICMD_RO_DisplaySetup_Status:
		//Get Athena LEFT/FE Display Setup Settings
		{
			WORD DisplaySetup = 0;
			
			DisplaySetup = (UserPrefOutputAspectRatio&0x0F);
		
			((DisplaySetupStatusResPkt_t fdata*)gExtDebugCmdPtr)->DisplaySetup_HighByte = (BYTE)((DisplaySetup>>8)&0x00FF);
			((DisplaySetupStatusResPkt_t fdata*)gExtDebugCmdPtr)->DisplaySetup_LowByte = (BYTE)(DisplaySetup&0x00FF);		
			SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		}
		break;
	case SICMD_RW_Audio1_CONTRL:
		{
         gAthena_RIGHT_Master_Parameters.Audio1 = ((((Audio1CtrlFuncCmd_t*)gExtDebugCmdPtr)->Audio1_HighByte)<<8)
                                                | (((Audio1CtrlFuncCmd_t*)gExtDebugCmdPtr)->Audio1_LowByte);			

         socipc_recv_msg("\nAthena RIGHT Audio1 Settings = 0x%X.\n",gAthena_RIGHT_Master_Parameters.Audio1);		

         //Send Audio1 Settings to Athena LEFT/FE (Base on current OSD layout)
         //0:3	Main ouput	000: Line out; 010:SPDIF out; Others: Reserved
         //4:7	Mute 		000: Off; 001: On; Others: Reserved
         //8:11	DownMix 		000: Off; 001: On; Others: Reserved	

         /*ChengLin 20130118: Check Athena L/FE current Main Audio Output settings and compare with Athena R inputs*/
         if (((BYTE)(gAthena_RIGHT_Master_Parameters.Audio1 & 0x000F)) != UserPrefAudioMainOutput)
         {
            UserPrefAudioMainOutput = (BYTE)(gAthena_RIGHT_Master_Parameters.Audio1 & 0x000F);
         }

         /*ChengLin 20130118: Check Athena L/FE current Mute settings and compare with Athena R inputs*/
         if (((BYTE)((gAthena_RIGHT_Master_Parameters.Audio1 & 0x00F0)>>4)) != UserPrefAudioMute)
         {
            UserPrefAudioMute = (BYTE)((gAthena_RIGHT_Master_Parameters.Audio1 & 0x00F0)>>4);
         }

         /*ChengLin 20130118: Check Athena L/FE current Mute settings and compare with Athena R inputs*/
         if (((BYTE)((gAthena_RIGHT_Master_Parameters.Audio1 & 0x0F00)>>8)) != UserPrefAudioDownMix)
         {
            UserPrefAudioDownMix = (BYTE)((gAthena_RIGHT_Master_Parameters.Audio1 & 0x0F00)>>8);
         }

         SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		}
		break;
	case SICMD_RO_Audio1_Status:
		//Get Athena LEFT/FE Audio1 Settings			
		{
			//0:3	Main ouput	000: Line out; 010:SPDIF out; Others: Reserved
			//4:7	Mute 		000: Off; 001: On; Others: Reserved
			//8:11	DownMix 		000: Off; 001: On; Others: Reserved				

			WORD Audio_1 = 0;
							
			Audio_1 = (((UserPrefAudioDownMix&0x0F)<<8)|((UserPrefAudioMute&0x0F)<<4)|(UserPrefAudioMainOutput&0x0F));

			((Audio1StatusResPkt_t fdata*)gExtDebugCmdPtr)->Audio1_HighByte = (BYTE)((Audio_1>>8)&0x00FF);
			((Audio1StatusResPkt_t fdata*)gExtDebugCmdPtr)->Audio1_LowByte = (BYTE)(Audio_1&0x00FF);		
			SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));
		}	
		break;
	case SICMD_RW_Audio2_CONTRL:
		{
         gAthena_RIGHT_Master_Parameters.Audio2 = ((((Audio2CtrlFuncCmd_t*)gExtDebugCmdPtr)->Audio2_HighByte)<<8)
                                                | (((Audio2CtrlFuncCmd_t*)gExtDebugCmdPtr)->Audio2_LowByte);			

         socipc_recv_msg("\nAthena RIGHT Audio2 Settings = 0x%X.\n",gAthena_RIGHT_Master_Parameters.Audio2);		

         //Send Audio2 Settings to Athena LEFT/FE (Base on current OSD layout)
         //0:5	Volume control	0-32
         //6:15	Audio delay 		0-999

         /*ChengLin 20130115: Check Athena L/FE current Volume settings and compare with Athena R inputs*/
         if (((BYTE)(gAthena_RIGHT_Master_Parameters.Audio2 & 0x003F)) != UserPrefAudioVolume)
         {
            UserPrefAudioVolume = (BYTE)(gAthena_RIGHT_Master_Parameters.Audio2 & 0x003F);
         }

         /*ChengLin 20130115: Check Athena L/FE current Audio Delay settings and compare with Athena R inputs*/
         if (((WORD)((gAthena_RIGHT_Master_Parameters.Audio2 & 0xFFC0)>>6)) != UserPrefAVSDelayTime)
         {
            UserPrefAVSDelayTime = (WORD)((gAthena_RIGHT_Master_Parameters.Audio2 & 0xFFC0)>>6);
         }

         SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		}
		break;
	case SICMD_RO_Audio2_Status:
		//Get Athena LEFT/FE Audio2 Settings			
		{
			//0:5	Volume control	0-32
			//6:15	Audio delay 		0-999
			
			WORD Audio_2 = 0;

			Audio_2 = ((UserPrefAVSDelayTime<<6)|UserPrefAudioVolume);
			
			((Audio2StatusResPkt_t fdata*)gExtDebugCmdPtr)->Audio2_HighByte = (BYTE)((Audio_2>>8)&0x00FF);
			((Audio2StatusResPkt_t fdata*)gExtDebugCmdPtr)->Audio2_LowByte = (BYTE)(Audio_2&0x00FF);		
			SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));				
		}	
		break;
	case SICMD_RW_Setup_CONTRL:
		{
			gAthena_RIGHT_Master_Parameters.Setup = ((((SetupCtrlFuncCmd_t*)gExtDebugCmdPtr)->Setup_HighByte)<<8)|(((SetupCtrlFuncCmd_t*)gExtDebugCmdPtr)->Setup_LowByte);			
			socipc_recv_msg("\nAthena RIGHT Setup Settings = 0x%X.\n",gAthena_RIGHT_Master_Parameters.Setup);

			//Send Setup Settings to Athena LEFT/FE (Base on current OSD layout)	
			//0:3	DCR control 	00: No; 01:Yes; 	10 / 11: Reserved
			//4:7	Factory Reset	00: No, 01:Yes; 	10 / 11: Reserved
			//8:15 Reserved

			/*ChengLin 20130130: Invert Athena Right DCR setting for avoid Athena Left DCR can't work*/
			if((BYTE)(gAthena_RIGHT_Master_Parameters.Setup & 0x000F) == 1)
				UserPrefDCR = 0;
			else
				UserPrefDCR = 1;			
			SOCIPC_Slave_UpdateFlag.Update_DCR = TRUE;

			UserPrefScrollMenuItem = (BYTE)((gAthena_RIGHT_Master_Parameters.Setup & 0x00F0)>>4);
			if(UserPrefScrollMenuItem == 1) //ChengLin 20130110: Execute it when Athena Right Press "Enter" key only
				SOCIPC_Slave_UpdateFlag.Update_FactoryReset = TRUE;
			
			SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		}
		break;
	case SICMD_RO_Setup_Status:
		//Get Athena LEFT/FE Setup Settings		
		{
			//0:3	DCR control 	00: No; 01:Yes; 	10 / 11: Reserved
			//4:7	Factory Reset	00: No, 01:Yes; 	10 / 11: Reserved
			//8:15 Reserved				

			WORD Setup = 0;	
				
			Setup = (((UserPrefScrollMenuItem&0x0F)<<4)|(UserPrefDCR&0x0F));
			((SetupStatusResPkt_t fdata*)gExtDebugCmdPtr)->Setup_HighByte = (BYTE)((Setup>>8)&0x00FF);
			((SetupStatusResPkt_t fdata*)gExtDebugCmdPtr)->Setup_LowByte = (BYTE)(Setup&0x00FF);		
			SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));					
		}	
		break;
	// 20130117
	case SICMD_RW_PBP_CONTRL:
		{
			gAthena_RIGHT_Master_Parameters.CaptureMain = (BYTE)(((MainCapCtrlFuncCmd_t*)gExtDebugCmdPtr)->CaptureMain);			
			socipc_recv_msg("\nAthena RIGHT CaptureMain = 0x%X.\n",(BYTE)gAthena_RIGHT_Master_Parameters.CaptureMain);
			UserPrefCaptureM= gAthena_RIGHT_Master_Parameters.CaptureMain;
			ModeSetupRequest(gmvb_MainChannel, REQ_MS_RESTART, PARAM_NONE, 50);
			gm_Print("Main Capture command from R side %d", UserPrefCaptureM);
			SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		}	
		break;
	case SICMD_RO_DCR_Result:
		//Get Athena LEFT DCR result
		((DcrResultResPkt_t fdata*)gExtDebugCmdPtr)->DcrDat_HighByte = (BYTE)(GetCurrentAcrResult() >> 8);
		((DcrResultResPkt_t fdata*)gExtDebugCmdPtr)->DcrDat_LowByte = (BYTE)(GetCurrentAcrResult());
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));
		break;
	case SICMD_RW_OSD_Sync_CONTRL:
		gAthena_RIGHT_Master_Parameters.OSDSync = (((OSDSyncCtrlFuncCmd_t*)gExtDebugCmdPtr)->OSDSync);		
		socipc_recv_msg("\nAthena RIGHT OSD Sync = 0x%X.\n",gAthena_RIGHT_Master_Parameters.OSDSync);

		if(gAthena_RIGHT_Master_Parameters.OSDSync)
		{
			//Clear all OSD related parameters before get all Athena RIGHT OSD parameters
			gAthena_RIGHT_Master_Parameters.Brightness = 0;
			gAthena_RIGHT_Master_Parameters.Contrast = 0;
			gAthena_RIGHT_Master_Parameters.Sharpness = 0;
			gAthena_RIGHT_Master_Parameters.ColorSettings = 0;
			gAthena_RIGHT_Master_Parameters.VideoProgress = 0;
			gAthena_RIGHT_Master_Parameters.NRSettings = 0;
			gAthena_RIGHT_Master_Parameters.VideoProgress_1 = 0;
			gAthena_RIGHT_Master_Parameters.DisplaySetup = 0;
			gAthena_RIGHT_Master_Parameters.Audio1 = 0;
			gAthena_RIGHT_Master_Parameters.Audio2 = 0;
			gAthena_RIGHT_Master_Parameters.Setup = 0;						
		}
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		break;
	case SICMD_RO_OSD_Sync_Status:
		//1: OSD Parameters are sync. 0: Athena RIGHT and Athena LEFT/FE OSD parameters are different		
		{
			WORD ColorSettings = 0;
			BYTE cColorTempNewIndex = 0;
			WORD VideoProgress = 0;
			WORD NRStatus = 0;
			WORD VideoProgress_1 = 0;
			WORD DisplaySetup = 0;
			WORD Audio_1 = 0;
			WORD Audio_2 = 0;
			WORD Setup = 0;				

			//Color Settings
			if(UserPrefColorTemp == CTEMP_6500K)
				cColorTempNewIndex = 0;
			else if(UserPrefColorTemp == CTEMP_7500K)
				cColorTempNewIndex = 1;
			else if(UserPrefColorTemp == CTEMP_9300K)
				cColorTempNewIndex = 2;
			else if(UserPrefColorTemp == USER_TEMP)
				cColorTempNewIndex = 3;
			else
				cColorTempNewIndex = 0x0F;	//Mismatch case 
			/*ChengLin 20130115: Modify for fit new items*/	
			ColorSettings = (((UserPrefDUMSwitch&0x0F)<<12)|((UserPrefColorSpace&0x0F)<<8)|((UserPrefImageScheme&0x0F)<<4)|(cColorTempNewIndex&0x0F));
			
			//Video Progress
			VideoProgress = (((UserPrefLCDOverdrive&0x0F)<<8)|((UserPrefMADI&0x0F)<<4)|(UserPrefDCDiMain&0x0F));

			//NR Settings
			NRStatus = (((UserPrefMPEGNRMode&0x0F)<<4)|(UserPrefTNR&0x0F));

			//Video Progress_1
			VideoProgress_1 = (UserPrefFilmMode&0x0F);

			//Display Setup
			DisplaySetup = (UserPrefOutputAspectRatio&0x0F);

			//Audio1
			Audio_1 = (((UserPrefAudioDownMix&0x0F)<<8)|((UserPrefAudioMute&0x0F)<<4)|(UserPrefAudioMainOutput&0x0F));

			//Audio2
			Audio_2 = ((UserPrefAVSDelayTime<<6)|UserPrefAudioVolume);

			//Setup
			Setup = (((UserPrefScrollMenuItem&0x0F)<<4)|(UserPrefDCR&0x0F));

		
		if( (gAthena_RIGHT_Master_Parameters.Brightness == UserPrefBrightness) && 
			(gAthena_RIGHT_Master_Parameters.Contrast == UserPrefContrast) && 
			(gAthena_RIGHT_Master_Parameters.Sharpness == SocIPCGetCorrectSharpnessSetting()) &&
			(gAthena_RIGHT_Master_Parameters.ColorSettings == ColorSettings) && 
			(gAthena_RIGHT_Master_Parameters.VideoProgress == VideoProgress) && 
			(gAthena_RIGHT_Master_Parameters.NRSettings == NRStatus) && 
			(gAthena_RIGHT_Master_Parameters.VideoProgress_1 == VideoProgress_1) && 
			(gAthena_RIGHT_Master_Parameters.DisplaySetup == DisplaySetup) && 
			(gAthena_RIGHT_Master_Parameters.Audio1 == Audio_1) && 
			(gAthena_RIGHT_Master_Parameters.Audio2 == Audio_2) && 
			(gAthena_RIGHT_Master_Parameters.Setup == Setup))
		{
			((OSDSyncStatusResPkt_t fdata*)gExtDebugCmdPtr)->OSDSync = 0x01; //Athena RIGHT and Athena LEFT/FE OSD Settings are the same
		}
		else
		{
			((OSDSyncStatusResPkt_t fdata*)gExtDebugCmdPtr)->OSDSync = 0x00; //Athena RIGHT and Athena LEFT/FE OSD Settings are different
		}
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));
		}
		break;

		// 20120418
	case SICMD_RO_OSD_Sync_changes:
		//Get differece
		{
      		BYTE Change_Flag = 0;
         #ifdef ST_4K2K_93xx_BOARD 
			if(GetChipID_4K2K()==CHIPID_4K2K_L)
			{
				if(B_Brightness != UserPrefBrightness)
				{
					B_Brightness = UserPrefBrightness;
					Change_Flag = SICMD_RO_Brightness_Status;
				}
				else if(B_Contrast != UserPrefContrast)
				{
					B_Contrast = UserPrefContrast;
					Change_Flag = SICMD_RO_Contrast_Status;
				}
				else if(B_Sharpness != UserPrefSharpnessRGB)
				{
					B_Sharpness = UserPrefSharpnessRGB;
					Change_Flag = SICMD_RO_Sharpness_Status;
				}
			}

			
			// Athena L
			if(GetChipID_4K2K()==CHIPID_4K2K_L)
			{
				BYTE Audio_1 = 0;

				Audio_1 = (((UserPrefAudioMainOutput&0x0F)<<4)|(UserPrefAudioMainInput&0x0F));
				if(B_AudioSettings != Audio_1)
				{
					B_AudioSettings = Audio_1;
					Change_Flag = SICMD_RO_Audio1_Status;
				}
				else if(B_AudioVolume != UserPrefAudioVolume)
				{
					B_AudioVolume = UserPrefAudioVolume;
					Change_Flag = SICMD_RO_Audio2_Status;
				}
			}
			#endif

			socipc_recv_msg("Change Flag = .......... 0x%x", (BYTE)Change_Flag);
			((ChangeFlagStatusResPkt_t fdata*)gExtDebugCmdPtr)->ChangeFlag = (BYTE)Change_Flag;			
			SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	

		}
		break;

	case SICMD_RW_OSD_DP_Switch :
		gAthena_RIGHT_Master_Parameters.DP_Switch= (BYTE)(((DPSwitchCtrlFuncCmd_t*)gExtDebugCmdPtr)->DP_Switch);			
		socipc_recv_msg("\nAthena RIGHT DP_Switch = 0x%X.\n",(BYTE)gAthena_RIGHT_Master_Parameters.DP_Switch);

		//UserPrefDPSWitch = gAthena_RIGHT_Master_Parameters.DP_Switch; //Keep Athena Left in DP1.2 mode
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		//socipc_recv_msg("UserPrefDPSWitch L = .................. %d", UserPrefDPSWitch);
		SOCIPC_Slave_UpdateFlag.Update_DPSwitch = TRUE;
		//SOCIPC_Slave_UpdateFlag.Update_UserData = TRUE;
		break;
		
	case SICMD_RW_DDCI_CONTRL:
		gAthena_RIGHT_Master_Parameters.DDC_VCPCode = (((DDCCICtrlFuncCmd_t*)gExtDebugCmdPtr)->DDC_VCP_Code);
		gAthena_RIGHT_Master_Parameters.DDC_Value = ((((DDCCICtrlFuncCmd_t*)gExtDebugCmdPtr)->DDC_Value_HighByte)<<8)|(((DDCCICtrlFuncCmd_t*)gExtDebugCmdPtr)->DDC_Value_LowByte);
		/*ChengLin 20130117: Modified for Sync DDC/CI commands for Athena Right/Left*/
		SOCIPC_Slave_UpdateFlag.Update_DDCCI = TRUE;
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		break;
	case SICMD_RO_DDCI_Status:
		//Base on DDC/CI commands to get Athena LEFT/FE current values to Athena RIGHT DDC/CI handler
		{
			BYTE DDC_VCP_Code = ((DDCCIStatusResPkt_t fdata*)gExtDebugCmdPtr)->DDC_VCP_Code;
			
			if(SocIPC_SlaveGetDDCCIStatusHandler(DDC_VCP_Code,(DDCCIStatusResPkt_t*)gExtDebugCmdPtr))
			{
				SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
			}
			else
			{
				SendDataType = RECV_NACK;
			}
		}
		break;

	/*ChengLin 20130110: 6-axis Saturation, Hue, Brightness controls*/
	case SICMD_RW_6Axis_Sat_Red_Ctrl:
		gAthena_RIGHT_Master_Parameters.SixAxisSatRed = (SBYTE)(((SixAxisSatRedCtrlFuncCmd_t*)gExtDebugCmdPtr)->SixAxisSatRed);			
		socipc_recv_msg("\nAthena RIGHT 6 Axis Saturation Red = 0x%X.\n",(BYTE)gAthena_RIGHT_Master_Parameters.SixAxisSatRed);
		UserPrefAcm3RedSat = gAthena_RIGHT_Master_Parameters.SixAxisSatRed;
		SOCIPC_Slave_UpdateFlag.Update_SixAxisColor_Index = SIX_AXIS_COLOR_RED_SAT;
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));
		break;
	case SICMD_RW_6Axis_Sat_Green_Ctrl:
		gAthena_RIGHT_Master_Parameters.SixAxisSatGreen = (SBYTE)(((SixAxisSatGreenCtrlFuncCmd_t*)gExtDebugCmdPtr)->SixAxisSatGreen);			
		socipc_recv_msg("\nAthena RIGHT 6 Axis Saturation Green = 0x%X.\n",(BYTE)gAthena_RIGHT_Master_Parameters.SixAxisSatGreen);
		UserPrefAcm3GreenSat = gAthena_RIGHT_Master_Parameters.SixAxisSatGreen;
		SOCIPC_Slave_UpdateFlag.Update_SixAxisColor_Index = SIX_AXIS_COLOR_GREEN_SAT;		
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		break;
	case SICMD_RW_6Axis_Sat_Blue_Ctrl:
		gAthena_RIGHT_Master_Parameters.SixAxisSatBlue = (SBYTE)(((SixAxisSatBlueCtrlFuncCmd_t*)gExtDebugCmdPtr)->SixAxisSatBlue);			
		socipc_recv_msg("\nAthena RIGHT 6 Axis Saturation Blue = 0x%X.\n",(BYTE)gAthena_RIGHT_Master_Parameters.SixAxisSatBlue);
		UserPrefAcm3BlueSat = gAthena_RIGHT_Master_Parameters.SixAxisSatBlue;
		SOCIPC_Slave_UpdateFlag.Update_SixAxisColor_Index = SIX_AXIS_COLOR_BLUE_SAT;		
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		break;
	case SICMD_RW_6Axis_Sat_Yellow_Ctrl:
		gAthena_RIGHT_Master_Parameters.SixAxisSatYellow = (SBYTE)(((SixAxisSatYellowCtrlFuncCmd_t*)gExtDebugCmdPtr)->SixAxisSatYellow);			
		socipc_recv_msg("\nAthena RIGHT 6 Axis Saturation Yellow = 0x%X.\n",(BYTE)gAthena_RIGHT_Master_Parameters.SixAxisSatYellow);
		UserPrefAcm3YellowSat = gAthena_RIGHT_Master_Parameters.SixAxisSatYellow;
		SOCIPC_Slave_UpdateFlag.Update_SixAxisColor_Index = SIX_AXIS_COLOR_YELLOW_SAT;		
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		break;
	case SICMD_RW_6Axis_Sat_Magenta_Ctrl:
		gAthena_RIGHT_Master_Parameters.SixAxisSatMagenta= (SBYTE)(((SixAxisSatMagentaCtrlFuncCmd_t*)gExtDebugCmdPtr)->SixAxisSatMagenta);			
		socipc_recv_msg("\nAthena RIGHT 6 Axis Saturation Magenta = 0x%X.\n",(BYTE)gAthena_RIGHT_Master_Parameters.SixAxisSatMagenta);
		UserPrefAcm3MagentaSat = gAthena_RIGHT_Master_Parameters.SixAxisSatMagenta;
		SOCIPC_Slave_UpdateFlag.Update_SixAxisColor_Index = SIX_AXIS_COLOR_MAGENTA_SAT;		
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		break;
	case SICMD_RW_6Axis_Sat_Cyan_Ctrl:
		gAthena_RIGHT_Master_Parameters.SixAxisSatCyan= (SBYTE)(((SixAxisSatCyanCtrlFuncCmd_t*)gExtDebugCmdPtr)->SixAxisSatCyan);			
		socipc_recv_msg("\nAthena RIGHT 6 Axis Saturation Cyan = 0x%X.\n",(BYTE)gAthena_RIGHT_Master_Parameters.SixAxisSatCyan);
		UserPrefAcm3CyanSat = gAthena_RIGHT_Master_Parameters.SixAxisSatCyan;
		SOCIPC_Slave_UpdateFlag.Update_SixAxisColor_Index = SIX_AXIS_COLOR_CYAN_SAT;		
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		break;
	case SICMD_RW_6Axis_Sat_Black_Ctrl:
		gAthena_RIGHT_Master_Parameters.SixAxisSatBlack= (SBYTE)(((SixAxisSatBlackCtrlFuncCmd_t*)gExtDebugCmdPtr)->SixAxisSatBlack);			
		socipc_recv_msg("\nAthena RIGHT 6 Axis Saturation Black = 0x%X.\n",(BYTE)gAthena_RIGHT_Master_Parameters.SixAxisSatBlack);
		UserPrefAcm3BlackSat = gAthena_RIGHT_Master_Parameters.SixAxisSatBlack;
		SOCIPC_Slave_UpdateFlag.Update_SixAxisColor_Index = SIX_AXIS_COLOR_BLACK_SAT;		
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		break;
	case SICMD_RW_6Axis_Sat_White_Ctrl:
		gAthena_RIGHT_Master_Parameters.SixAxisSatWhite= (SBYTE)(((SixAxisSatWhiteCtrlFuncCmd_t*)gExtDebugCmdPtr)->SixAxisSatWhite);			
		socipc_recv_msg("\nAthena RIGHT 6 Axis Saturation White = 0x%X.\n",(BYTE)gAthena_RIGHT_Master_Parameters.SixAxisSatWhite);
		UserPrefAcm3WhiteSat = gAthena_RIGHT_Master_Parameters.SixAxisSatWhite;
		SOCIPC_Slave_UpdateFlag.Update_SixAxisColor_Index = SIX_AXIS_COLOR_WHITE_SAT;		
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		break;
	case SICMD_RW_6Axis_Hue_Red_Ctrl:
		gAthena_RIGHT_Master_Parameters.SixAxisHueRed= (SBYTE)(((SixAxisHueRedCtrlFuncCmd_t*)gExtDebugCmdPtr)->SixAxisHueRed);			
		socipc_recv_msg("\nAthena RIGHT 6 Axis Hue Red = 0x%X.\n",(BYTE)gAthena_RIGHT_Master_Parameters.SixAxisHueRed);
		UserPrefAcm3RedHue = gAthena_RIGHT_Master_Parameters.SixAxisHueRed;
		SOCIPC_Slave_UpdateFlag.Update_SixAxisColor_Index = SIX_AXIS_COLOR_RED_HUE;		
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		break;
	case SICMD_RW_6Axis_Hue_Green_Ctrl:
		gAthena_RIGHT_Master_Parameters.SixAxisHueGreen= (SBYTE)(((SixAxisHueGreenCtrlFuncCmd_t*)gExtDebugCmdPtr)->SixAxisHueGreen);			
		socipc_recv_msg("\nAthena RIGHT 6 Axis Hue Green = 0x%X.\n",(BYTE)gAthena_RIGHT_Master_Parameters.SixAxisHueGreen);
		UserPrefAcm3GreenHue = gAthena_RIGHT_Master_Parameters.SixAxisHueGreen;
		SOCIPC_Slave_UpdateFlag.Update_SixAxisColor_Index = SIX_AXIS_COLOR_GREEN_HUE;		
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		break;
	case SICMD_RW_6Axis_Hue_Blue_Ctrl:
		gAthena_RIGHT_Master_Parameters.SixAxisHueBlue= (SBYTE)(((SixAxisHueBlueCtrlFuncCmd_t*)gExtDebugCmdPtr)->SixAxisHueBlue);			
		socipc_recv_msg("\nAthena RIGHT 6 Axis Hue Blue = 0x%X.\n",(BYTE)gAthena_RIGHT_Master_Parameters.SixAxisHueBlue);
		UserPrefAcm3BlueHue = gAthena_RIGHT_Master_Parameters.SixAxisHueBlue;
		SOCIPC_Slave_UpdateFlag.Update_SixAxisColor_Index = SIX_AXIS_COLOR_BLUE_HUE;		
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		break;
	case SICMD_RW_6Axis_Hue_Yellow_Ctrl:
		gAthena_RIGHT_Master_Parameters.SixAxisHueYellow= (SBYTE)(((SixAxisHueYellowCtrlFuncCmd_t*)gExtDebugCmdPtr)->SixAxisHueYellow);			
		socipc_recv_msg("\nAthena RIGHT 6 Axis Hue Yellow = 0x%X.\n",(BYTE)gAthena_RIGHT_Master_Parameters.SixAxisHueYellow);
		UserPrefAcm3YellowHue = gAthena_RIGHT_Master_Parameters.SixAxisHueYellow;
		SOCIPC_Slave_UpdateFlag.Update_SixAxisColor_Index = SIX_AXIS_COLOR_YELLOW_HUE;		
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		break;
	case SICMD_RW_6Axis_Hue_Magenta_Ctrl:
		gAthena_RIGHT_Master_Parameters.SixAxisHueMagenta= (SBYTE)(((SixAxisHueMagentaCtrlFuncCmd_t*)gExtDebugCmdPtr)->SixAxisHueMagenta);			
		socipc_recv_msg("\nAthena RIGHT 6 Axis Hue Magenta = 0x%X.\n",(BYTE)gAthena_RIGHT_Master_Parameters.SixAxisHueMagenta);
		UserPrefAcm3MagentaHue = gAthena_RIGHT_Master_Parameters.SixAxisHueMagenta;
		SOCIPC_Slave_UpdateFlag.Update_SixAxisColor_Index = SIX_AXIS_COLOR_MAGENTA_HUE;		
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		break;
	case SICMD_RW_6Axis_Hue_Cyan_Ctrl:
		gAthena_RIGHT_Master_Parameters.SixAxisHueCyan= (SBYTE)(((SixAxisHueCyanCtrlFuncCmd_t*)gExtDebugCmdPtr)->SixAxisHueCyan);			
		socipc_recv_msg("\nAthena RIGHT 6 Axis Hue Cyan = 0x%X.\n",(BYTE)gAthena_RIGHT_Master_Parameters.SixAxisHueCyan);
		UserPrefAcm3CyanHue = gAthena_RIGHT_Master_Parameters.SixAxisHueCyan;
		SOCIPC_Slave_UpdateFlag.Update_SixAxisColor_Index = SIX_AXIS_COLOR_CYAN_HUE;		
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		break;
	case SICMD_RW_6Axis_Hue_Black_Ctrl:
		gAthena_RIGHT_Master_Parameters.SixAxisHueBlack= (SBYTE)(((SixAxisHueBlackCtrlFuncCmd_t*)gExtDebugCmdPtr)->SixAxisHueBlack);			
		socipc_recv_msg("\nAthena RIGHT 6 Axis Hue Black = 0x%X.\n",(BYTE)gAthena_RIGHT_Master_Parameters.SixAxisHueBlack);
		UserPrefAcm3BlackHue = gAthena_RIGHT_Master_Parameters.SixAxisHueBlack;
		SOCIPC_Slave_UpdateFlag.Update_SixAxisColor_Index = SIX_AXIS_COLOR_BLACK_HUE;		
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		break;
	case SICMD_RW_6Axis_Hue_White_Ctrl:
		gAthena_RIGHT_Master_Parameters.SixAxisHueWhite= (SBYTE)(((SixAxisHueWhiteCtrlFuncCmd_t*)gExtDebugCmdPtr)->SixAxisHueWhite);			
		socipc_recv_msg("\nAthena RIGHT 6 Axis Hue White = 0x%X.\n",(BYTE)gAthena_RIGHT_Master_Parameters.SixAxisHueWhite);
		UserPrefAcm3WhiteHue = gAthena_RIGHT_Master_Parameters.SixAxisHueWhite;
		SOCIPC_Slave_UpdateFlag.Update_SixAxisColor_Index = SIX_AXIS_COLOR_WHITE_HUE;		
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		break;
	case SICMD_RW_6Axis_Bright_Red_Ctrl:
		gAthena_RIGHT_Master_Parameters.SixAxisBrightRed= (SBYTE)(((SixAxisBrightRedCtrlFuncCmd_t*)gExtDebugCmdPtr)->SixAxisBrightRed);			
		socipc_recv_msg("\nAthena RIGHT 6 Axis Brightness Red = 0x%X.\n",(BYTE)gAthena_RIGHT_Master_Parameters.SixAxisBrightRed);
		UserPrefAcm3RedLuma= gAthena_RIGHT_Master_Parameters.SixAxisBrightRed;
		SOCIPC_Slave_UpdateFlag.Update_SixAxisColor_Index = SIX_AXIS_COLOR_RED_LUMA;		
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		break;
	case SICMD_RW_6Axis_Bright_Green_Ctrl:
		gAthena_RIGHT_Master_Parameters.SixAxisBrightGreen= (SBYTE)(((SixAxisBrightGreenCtrlFuncCmd_t*)gExtDebugCmdPtr)->SixAxisBrightGreen);			
		socipc_recv_msg("\nAthena RIGHT 6 Axis Brightness Green = 0x%X.\n",(BYTE)gAthena_RIGHT_Master_Parameters.SixAxisBrightGreen);
		UserPrefAcm3GreenLuma= gAthena_RIGHT_Master_Parameters.SixAxisBrightGreen;
		SOCIPC_Slave_UpdateFlag.Update_SixAxisColor_Index = SIX_AXIS_COLOR_GREEN_LUMA;		
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		break;
	case SICMD_RW_6Axis_Bright_Blue_Ctrl:
		gAthena_RIGHT_Master_Parameters.SixAxisBrightBlue= (SBYTE)(((SixAxisBrightBlueCtrlFuncCmd_t*)gExtDebugCmdPtr)->SixAxisBrightBlue);			
		socipc_recv_msg("\nAthena RIGHT 6 Axis Brightness Blue = 0x%X.\n",(BYTE)gAthena_RIGHT_Master_Parameters.SixAxisBrightBlue);
		UserPrefAcm3BlueLuma= gAthena_RIGHT_Master_Parameters.SixAxisBrightBlue;
		SOCIPC_Slave_UpdateFlag.Update_SixAxisColor_Index = SIX_AXIS_COLOR_BLUE_LUMA;		
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		break;
	case SICMD_RW_6Axis_Bright_Yellow_Ctrl:
		gAthena_RIGHT_Master_Parameters.SixAxisBrightYellow= (SBYTE)(((SixAxisBrightYellowCtrlFuncCmd_t*)gExtDebugCmdPtr)->SixAxisBrightYellow);			
		socipc_recv_msg("\nAthena RIGHT 6 Axis Brightness Yellow = 0x%X.\n",(BYTE)gAthena_RIGHT_Master_Parameters.SixAxisBrightYellow);
		UserPrefAcm3YellowLuma= gAthena_RIGHT_Master_Parameters.SixAxisBrightYellow;
		SOCIPC_Slave_UpdateFlag.Update_SixAxisColor_Index = SIX_AXIS_COLOR_YELLOW_LUMA;		
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		break;
	case SICMD_RW_6Axis_Bright_Magenta_Ctrl:
		gAthena_RIGHT_Master_Parameters.SixAxisBrightMagenta= (SBYTE)(((SixAxisBrightMagentaCtrlFuncCmd_t*)gExtDebugCmdPtr)->SixAxisBrightMagenta);			
		socipc_recv_msg("\nAthena RIGHT 6 Axis Brightness Magenta = 0x%X.\n",(BYTE)gAthena_RIGHT_Master_Parameters.SixAxisBrightMagenta);
		UserPrefAcm3MagentaLuma= gAthena_RIGHT_Master_Parameters.SixAxisBrightMagenta;
		SOCIPC_Slave_UpdateFlag.Update_SixAxisColor_Index = SIX_AXIS_COLOR_MAGENTA_LUMA;		
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		break;
	case SICMD_RW_6Axis_Bright_Cyan_Ctrl:
		gAthena_RIGHT_Master_Parameters.SixAxisBrightCyan= (SBYTE)(((SixAxisBrightCyanCtrlFuncCmd_t*)gExtDebugCmdPtr)->SixAxisBrightCyan);			
		socipc_recv_msg("\nAthena RIGHT 6 Axis Brightness Cyan = 0x%X.\n",(BYTE)gAthena_RIGHT_Master_Parameters.SixAxisBrightCyan);
		UserPrefAcm3CyanLuma= gAthena_RIGHT_Master_Parameters.SixAxisBrightCyan;
		SOCIPC_Slave_UpdateFlag.Update_SixAxisColor_Index = SIX_AXIS_COLOR_CYAN_LUMA;		
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		break;
	case SICMD_RW_6Axis_Bright_Black_Ctrl:
		gAthena_RIGHT_Master_Parameters.SixAxisBrightBlack= (SBYTE)(((SixAxisBrightBlackCtrlFuncCmd_t*)gExtDebugCmdPtr)->SixAxisBrightBlack);			
		socipc_recv_msg("\nAthena RIGHT 6 Axis Brightness Black = 0x%X.\n",(BYTE)gAthena_RIGHT_Master_Parameters.SixAxisBrightBlack);
		UserPrefAcm3BlackLuma= gAthena_RIGHT_Master_Parameters.SixAxisBrightBlack;
		SOCIPC_Slave_UpdateFlag.Update_SixAxisColor_Index = SIX_AXIS_COLOR_BLACK_LUMA;		
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		break;
	case SICMD_RW_6Axis_Bright_White_Ctrl:
		gAthena_RIGHT_Master_Parameters.SixAxisBrightWhite= (SBYTE)(((SixAxisBrightWhiteCtrlFuncCmd_t*)gExtDebugCmdPtr)->SixAxisBrightWhite);			
		socipc_recv_msg("\nAthena RIGHT 6 Axis Brightness White = 0x%X.\n",(BYTE)gAthena_RIGHT_Master_Parameters.SixAxisBrightWhite);
		UserPrefAcm3WhiteLuma= gAthena_RIGHT_Master_Parameters.SixAxisBrightWhite;
		SOCIPC_Slave_UpdateFlag.Update_SixAxisColor_Index = SIX_AXIS_COLOR_WHITE_LUMA;		
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		break;
	/*****************************************/

//inter-chip SMT communication
//refer to doc "4Kx2K System FW SMT Design Gamma Calibration and Uniformity Compensation"
   #ifdef USE_SMT
        case SICMD_WO_SMT_COMMAND:
        //Len + Command + "Message Control" byte + SMT message
		SMT_SlaveMessageHandler((BYTE *) (gExtDebugCmdPtr), 0); 
            SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));
            break;
        
   #endif            

#ifdef DP_HUB_2_LOG_PORTS      
   case SICMD_RW_DpRx0_MstInfo: 
         {
            gAthena_RIGHT_Master_Parameters.DPRX0_MstInfo = (BYTE)(((DpRx0MstInfoFuncCmd_t*)gExtDebugCmdPtr)->DpRx0MstInfo);         
            socipc_recv_msg("\nAthena LEFT DPRX0_MstInfo = 0x%X.\n",(BYTE)gAthena_RIGHT_Master_Parameters.DPRX0_MstInfo);
            UpdateDpRx0MstInfo(gAthena_RIGHT_Master_Parameters.DPRX0_MstInfo);
            SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1)); 
         }  
        break;

   case SICMD_RO_DpRx0_IsStable:
      ((DpRx0IsStableResPkt_t fdata*)gExtDebugCmdPtr)->DpRx0_IsStable = IsDpRx0_Ready2UpdateMstInfo();
      SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));   
      break;

   case SICMD_RW_DPRx0_ReModeSetup:
      gAthena_RIGHT_Master_Parameters.DPRX0_ReModeSetup = (BYTE)(((DpRx0ReModeSetupFuncCmd_t*)gExtDebugCmdPtr)->DpRx0ReModeSetup);         
      socipc_recv_msg("\nAthena LEFT DPRX0_ReModeSetup = 0x%X.\n",(BYTE)gAthena_RIGHT_Master_Parameters.DPRX0_ReModeSetup);
      if(gAthena_RIGHT_Master_Parameters.DPRX0_ReModeSetup)
      {
         SOCIPC_Slave_UpdateFlag.Update_Redo_ModeSetup = TRUE;
      }
      SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1)); 
      break;     
#endif

	case SICMD_RW_Keypad_CONTRL:
		{
			BYTE B_Index = 0;
    		WBK_KEYPAD_DEFINITION ROM* Sp_Keypad;			
			BYTE Key_Channel = ((ADCKeypadFuncCmd_t*)gExtDebugCmdPtr)->Key_Channel;
			BYTE Key_Value = ((ADCKeypadFuncCmd_t*)gExtDebugCmdPtr)->Key_Value;		
			socipc_recv_msg("\nKeypad Channel = 0x%X.\n",Key_Channel);
			socipc_recv_msg("\nKeypad Value = 0x%X.\n",Key_Value);

			for (B_Index = 0;;)
			{
				Sp_Keypad = WBK_Keypads[B_Index];
				if (Sp_Keypad && (Sp_Keypad->GetKeyValue == GetADCValue) && (Sp_Keypad->B_ChanNumber == Key_Channel)) //chack channel number
					break;
				if (B_Index++>=WB_NUMBER_OF_INPUT_SOURCE)
						goto Keypad_Ctrl_NACK;
			}
			
			if ( Sp_Keypad->B_KeysNumber == 0 )
				goto Keypad_Ctrl_NACK;

			EnqueueOsdKey((Key_Value <= 0x10) ? Key_None : TranslateCodeToKey(Sp_Keypad->Bp_KeyValues, Sp_Keypad->B_KeysNumber,Sp_Keypad->B_CodeLen, Sp_Keypad->B_Delta,Key_Value));
			SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));
			goto Keypad_Ctrl_End;
		}

Keypad_Ctrl_NACK:
	SendDataType = RECV_NACK;
Keypad_Ctrl_End:
	
		break;
	case SICMD_RW_IR_CONTROL:
		{
			BYTE IR_KeyValue = ((IRFuncCmd_t*)gExtDebugCmdPtr)->IR_Value;
			EnqueueOsdKey(IR_KeyValue);
			SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));			
		}	
		break;

	/*ChengLin 20130128: New Sync Handler State Control*/	
	case SICMD_RW_IMAGE_SYNC_STATE_Ctrl:
		gAthena_RIGHT_Master_Parameters.ImageSyncState = (SBYTE)(((ImageSyncStateCtrlFuncCmd_t*)gExtDebugCmdPtr)->ImageSyncState);
		gCurrentAthenaLEFT_FE_SyncState = gAthena_RIGHT_Master_Parameters.ImageSyncState;

		if(gCurrentAthenaLEFT_FE_SyncState==IMAGE_LVDS_ON_STATE)
		{
         SetPanelControlStatus(PANEL_CTRL_LVDS_ON_REQUEST);
        }
		else if(gCurrentAthenaLEFT_FE_SyncState==IMAGE_LVDS_OFF_STATE)
		{
		   SetPanelControlStatus(PANEL_CTRL_LVDS_OFF_REQUEST);
	    }

		if(gCurrentAthenaLEFT_FE_SyncState != IMAGE_LVDS_ON_STATE)
			Image_ReadyFlag(FALSE); //Set Athena Left GPIO84 to Low		
		socipc_recv_msg("\nAthena RIGHT Image Sync State = 0x%X.\n",(BYTE)gAthena_RIGHT_Master_Parameters.ImageSyncState);
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		break;
	case SICMD_RW_IMAGE_SYNC_STATE_Status:
		//Get Athena LEFT/FE Sync State Status
		/*	 IMAGE_LVDS_ON_STATE = 0,
			IMAGE_OFF_STATE,
			IMAGE_RDY_WAIT_STATE,
			IMAGE_ON_STATE,
			IMAGE_LOST_WAIT_STATE,
			IMAGE_FAIL_STATE
		*/
		((ImageSyncStateStatusResPkt_t fdata*)gExtDebugCmdPtr)->ImageSyncState = (BYTE)gCurrentAthenaLEFT_FE_SyncState;
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		break;
	/*********************************************/ 
	/*ChengLin 20130222: Add for sync Left/Right PB Control Status*/
#if ENABLE_ATHENA_R_L_IMAGE_SYNC_HANDLER
	case SICMD_RW_IMAGE_SYNC_BLACK_SCREEN_CTRL:
		gAthena_RIGHT_Master_Parameters.ImageSyncBlackScreen = (SBYTE)(((ImageSyncBlackScreenCtrlFuncCmd_t*)gExtDebugCmdPtr)->ImageSyncBlackScreen);	
		ImageSyncHandler_msg("\nAthena RIGHT Image Sync Black Screen = 0x%X.\n",(BYTE)gAthena_RIGHT_Master_Parameters.ImageSyncBlackScreen);
		if(GetChipID_4K2K()==CHIPID_4K2K_L)
		SOCIPC_Slave_UpdateFlag.Update_PB_Ctrl = TRUE;
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		break;
#endif
	case SICMD_RW_GAMMA_CORRECTION_CONTRL: //Add by ChengLin 20130226
		gAthena_RIGHT_Master_Parameters.GammaCorrection = (((GammaCorrectionCtrlFuncCmd_t*)gExtDebugCmdPtr)->GammaCorrection);			
		socipc_recv_msg("\nAthena RIGHT Gamma Correction = 0x%X.\n",(BYTE)gAthena_RIGHT_Master_Parameters.GammaCorrection);
		SOCIPC_Slave_UpdateFlag.Update_GammaCorrection = TRUE;		
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));
		break;

#if ENABLE_VEGA_ISP_CMD_VIA_UART_ISP_PORT
   /*ChengLin 20130411: Enable Athena FE or Left Debug Mode from Athena Right*/
   case SICMD_RW_ATHENA_FE_L_DEBUG_MODE_EN_CTRL:
		gAthena_RIGHT_Master_Parameters.AthenaFE_L_DebugMode = (((AthenaFE_L_DebugModeCtrlFuncCmd_t*)gExtDebugCmdPtr)->DebugMode);			
		socipc_recv_msg("\nAthena RIGHT Debug Mode = 0x%X.\n",(BYTE)gAthena_RIGHT_Master_Parameters.AthenaFE_L_DebugMode);
		gmvb_DebugOnly = gAthena_RIGHT_Master_Parameters.AthenaFE_L_DebugMode;		
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));
      break;
#endif

#ifdef SMART_ISP
   case SICMD_RW_SMARTISP_CTRL:
		gAthena_RIGHT_Master_Parameters.SmartIspCtrl = (SBYTE)(((SmartIspCtrlFuncCmd_t*)gExtDebugCmdPtr)->SmartIspControl);	
		socipc_recv_msg("\nAthena RIGHT SmartIsp Control = 0x%X.\n",(BYTE)gAthena_RIGHT_Master_Parameters.SmartIspCtrl);
		SmartIspSlaveControl(gAthena_RIGHT_Master_Parameters.SmartIspCtrl);
      //SOCIPC_Slave_UpdateFlag.Update_SmartIsp = TRUE;
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
		break;

   case SICMD_RO_SMARTISP_STATUS:
      ((SmartIspStatusResPkt_t fdata*)gExtDebugCmdPtr)->SmartIspStep = GetIspStep();
      ((SmartIspStatusResPkt_t fdata*)gExtDebugCmdPtr)->SmartispStatus = GetIspStepStatus();
      //gm_Print("Step=%d",GetIspStep());
      //gm_Print("Status=%d",GetIspStepStatus());
      SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));   
      //gm_Print("RO_SMARTISP_STATUS",0);
      break;
#endif
   /*ChengLin 20130322: Load Default Settings or Original Setting Before/After Splash Screen*/
   case SICMD_RW_LOAD_SPLASH_SETTING_CTRL: 
		gAthena_RIGHT_Master_Parameters.SplashLoadDefault = (BYTE)(((SplashScreenLoadDefCtrlFuncCmd_t*)gExtDebugCmdPtr)->SplashLoadDefault);
		socipc_recv_msg("\nAthena RIGHT Set Splash Load Def = 0x%X.\n",(BYTE)gAthena_RIGHT_Master_Parameters.SplashLoadDefault);
		SOCIPC_Slave_UpdateFlag.Update_SplashLoadDef = TRUE;      
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));
      break;
      
	case SICMD_RW_Slave_Registers_addr_CONTRL:
		gAthena_RIGHT_Master_Parameters.SlaveAddress = ((((SlaveRegistersAddrFuncCmd_t*)gExtDebugCmdPtr)->Address_HighByte)<<8)|(((SlaveRegistersAddrFuncCmd_t*)gExtDebugCmdPtr)->Address_LowByte);			
		socipc_recv_msg("\nSlave Register Address = 0x%X.\n",gAthena_RIGHT_Master_Parameters.SlaveAddress);		
		SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));		
		break;
	case SICMD_RW_Slave_Registers_data_CONTRL:
		if( gAthena_RIGHT_Master_Parameters.SlaveAddress != 0 )
		{
			gAthena_RIGHT_Master_Parameters.SlaveWriteData = ((((SlaveRegistersDataFuncCmd_t*)gExtDebugCmdPtr)->Data_HighByte)<<8)|(((SlaveRegistersDataFuncCmd_t*)gExtDebugCmdPtr)->Data_LowByte);		
			socipc_recv_msg("\nSlave Register Data = 0x%X.\n",gAthena_RIGHT_Master_Parameters.SlaveWriteData);

			//Execute Register Write
			gm_WriteRegWord(gAthena_RIGHT_Master_Parameters.SlaveAddress, gAthena_RIGHT_Master_Parameters.SlaveWriteData);		
			SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));			
		}
		else //Invalid Address
		{
			SendDataType = RECV_NACK;	
		}
		break;
	case SICMD_RO_Slave_Registers_read_data:
		{
			WORD SlaveRegReadData = 0;
			
			if( gAthena_RIGHT_Master_Parameters.SlaveAddress != 0 )
			{
				SlaveRegReadData = gm_ReadRegWord(gAthena_RIGHT_Master_Parameters.SlaveAddress);
				((SlaveRegistersDataResPkt_t fdata*)gExtDebugCmdPtr)->Data_HighByte = (BYTE)((SlaveRegReadData>>8)&0x00FF);
				((SlaveRegistersDataResPkt_t fdata*)gExtDebugCmdPtr)->Data_LowByte = (BYTE)(SlaveRegReadData&0x00FF);		
				SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));
			}
			else //Invalid Address
			{
				SlaveRegReadData = 0xFFFF;
				((SlaveRegistersDataResPkt_t fdata*)gExtDebugCmdPtr)->Data_HighByte = (BYTE)((SlaveRegReadData>>8)&0x00FF);
				((SlaveRegistersDataResPkt_t fdata*)gExtDebugCmdPtr)->Data_LowByte = (BYTE)(SlaveRegReadData&0x00FF);		
				SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	
			}
		}
		break;
	case SICMD_RW_SaveModeIndepedent_Ctrl:
		{
			SOCIPC_Slave_UpdateFlag.Update_UserData = TRUE;			
			SendDataType = SocIPCGetResponseDataType(*(gExtDebugCmdPtr+1));	//Added by ChengLin 20130124			
		}
		break;

	default:
		_fmemcpy((BYTE far*)gExtDebugCmdPtr, (BYTE far*)IPC_NakPakt, 3);
		SocIPCI2CSlaveWriteCmdPkt((BYTE far*)gExtDebugCmdPtr , B_whichHandler-I2C_SLAVE1_USE);
		SendDataType = RECV_NACK;			
	  	PS_Status = UNKNOWN_MSG_CMD;		
		break;
   }

 	if( SendDataType == RECV_ACK )
 	{
		_fmemcpy((BYTE far*)gExtDebugCmdPtr, (BYTE far*)IPC_AckPakt, 3);
		SocIPCI2CSlaveWriteCmdPkt((BYTE far*)gExtDebugCmdPtr , B_whichHandler-I2C_SLAVE1_USE);	

	}
	else if( SendDataType == RECV_NACK )
	{
		_fmemcpy((BYTE far*)gExtDebugCmdPtr, (BYTE far*)IPC_NakPakt, 3);
		SocIPCI2CSlaveWriteCmdPkt((BYTE far*)gExtDebugCmdPtr , B_whichHandler-I2C_SLAVE1_USE);

	}
	else if( SendDataType == RECV_DATA )
	{
		switch (*(gExtDebugCmdPtr+1))
		{
		 	case SICMD_RO_Power_Status:
				((PowerStatusResPkt_t fdata*)gExtDebugCmdPtr)->Length = (sizeof(PowerStatusResPkt_t)-2)+3;//+3:length,command,null-terminated
				SocIPCI2CSlaveWriteCmdPkt((BYTE far*)gExtDebugCmdPtr , B_whichHandler-I2C_SLAVE1_USE);	
				break;			
			case SICMD_RO_Port_Status:
				((PortStatusResPkt_t fdata*)gExtDebugCmdPtr)->Length = (sizeof(PortStatusResPkt_t)-2)+3;//+3:length,command,null-terminated
				SocIPCI2CSlaveWriteCmdPkt((BYTE far*)gExtDebugCmdPtr , B_whichHandler-I2C_SLAVE1_USE);	
				break;
			case SICMD_RO_Brightness_Status:
				((BrightnessStatusResPkt_t fdata*)gExtDebugCmdPtr)->Length = (sizeof(BrightnessStatusResPkt_t)-2)+3;//+3:length,command,null-terminated
				SocIPCI2CSlaveWriteCmdPkt((BYTE far*)gExtDebugCmdPtr , B_whichHandler-I2C_SLAVE1_USE);
				break;
			// 20120418
			case SICMD_RO_OSD_Sync_changes:
				((ChangeFlagStatusResPkt_t fdata*)gExtDebugCmdPtr)->Length = (sizeof(ChangeFlagStatusResPkt_t)-2)+3;//+3:length,command,null-terminated
				SocIPCI2CSlaveWriteCmdPkt((BYTE far*)gExtDebugCmdPtr , B_whichHandler-I2C_SLAVE1_USE);
				break;
			case SICMD_RO_Sharpness_Status:
				((SharpnessStatusResPkt_t fdata*)gExtDebugCmdPtr)->Length = (sizeof(SharpnessStatusResPkt_t)-2)+3;//+3:length,command,null-terminated
				SocIPCI2CSlaveWriteCmdPkt((BYTE far*)gExtDebugCmdPtr , B_whichHandler-I2C_SLAVE1_USE);
				break;
			case SICMD_RO_OSD_Sync_Status:				
				((OSDSyncStatusResPkt_t fdata*)gExtDebugCmdPtr)->Length = (sizeof(OSDSyncStatusResPkt_t)-2)+3;//+3:length,command,null-terminated
				SocIPCI2CSlaveWriteCmdPkt((BYTE far*)gExtDebugCmdPtr , B_whichHandler-I2C_SLAVE1_USE);	
				break;
			case SICMD_RW_Mode_Info_STATUS:
				((ModeInfoStatusResPkt_t fdata*)gExtDebugCmdPtr)->Length = (sizeof(ModeInfoStatusResPkt_t)-2)+3;//+3:length,command,null-terminated
				SocIPCI2CSlaveWriteCmdPkt((BYTE far*)gExtDebugCmdPtr , B_whichHandler-I2C_SLAVE1_USE);
				break;
			case SICMD_RO_Timing_H_Width_Slave:
				((TimingHWidthSlaveResPkt_t fdata*)gExtDebugCmdPtr)->Length = (sizeof(TimingHWidthSlaveResPkt_t)-2)+3;//+3:length,command,null-terminated
				SocIPCI2CSlaveWriteCmdPkt((BYTE far*)gExtDebugCmdPtr , B_whichHandler-I2C_SLAVE1_USE);
				break;
			case SICMD_RO_Timing_V_Length_Slave:
				((TimingVLengthSlaveResPkt_t fdata*)gExtDebugCmdPtr)->Length = (sizeof(TimingVLengthSlaveResPkt_t)-2)+3;//+3:length,command,null-terminated
				SocIPCI2CSlaveWriteCmdPkt((BYTE far*)gExtDebugCmdPtr , B_whichHandler-I2C_SLAVE1_USE);
				break;				
			case SICMD_RO_Timing_H_Total_Slave:
				((TimingHTotalSlaveResPkt_t fdata*)gExtDebugCmdPtr)->Length = (sizeof(TimingHTotalSlaveResPkt_t)-2)+3;//+3:length,command,null-terminated
				SocIPCI2CSlaveWriteCmdPkt((BYTE far*)gExtDebugCmdPtr , B_whichHandler-I2C_SLAVE1_USE);
				break;				
			case SICMD_RO_Timing_V_Total_Slave:
				((TimingVTotalSlaveResPkt_t fdata*)gExtDebugCmdPtr)->Length = (sizeof(TimingVTotalSlaveResPkt_t)-2)+3;//+3:length,command,null-terminated
				SocIPCI2CSlaveWriteCmdPkt((BYTE far*)gExtDebugCmdPtr , B_whichHandler-I2C_SLAVE1_USE);
				break;					
			case SICMD_RO_Timing_H_Start_Slave:
				((TimingHStartSlaveResPkt_t fdata*)gExtDebugCmdPtr)->Length = (sizeof(TimingHStartSlaveResPkt_t)-2)+3;//+3:length,command,null-terminated
				SocIPCI2CSlaveWriteCmdPkt((BYTE far*)gExtDebugCmdPtr , B_whichHandler-I2C_SLAVE1_USE);
				break;				
			case SICMD_RO_Timing_V_Start_Slave:
				((TimingVStartSlaveResPkt_t fdata*)gExtDebugCmdPtr)->Length = (sizeof(TimingVStartSlaveResPkt_t)-2)+3;//+3:length,command,null-terminated
				SocIPCI2CSlaveWriteCmdPkt((BYTE far*)gExtDebugCmdPtr , B_whichHandler-I2C_SLAVE1_USE);
				break;	
			case SICMD_RO_Timing_V_Freq_Slave:
				((TimingVFreqSlaveResPkt_t fdata*)gExtDebugCmdPtr)->Length = (sizeof(TimingVFreqSlaveResPkt_t)-2)+3;//+3:length,command,null-terminated
				SocIPCI2CSlaveWriteCmdPkt((BYTE far*)gExtDebugCmdPtr , B_whichHandler-I2C_SLAVE1_USE);
				break;
			case SICMD_RO_Contrast_Status:
				((ContrastStatusResPkt_t fdata*)gExtDebugCmdPtr)->Length = (sizeof(ContrastStatusResPkt_t)-2)+3;//+3:length,command,null-terminated
				SocIPCI2CSlaveWriteCmdPkt((BYTE far*)gExtDebugCmdPtr , B_whichHandler-I2C_SLAVE1_USE);
				break;					
			case SICMD_RO_Color_Status:
				((ColorStatusResPkt_t fdata*)gExtDebugCmdPtr)->Length = (sizeof(ColorStatusResPkt_t)-2)+3;//+3:length,command,null-terminated
				SocIPCI2CSlaveWriteCmdPkt((BYTE far*)gExtDebugCmdPtr , B_whichHandler-I2C_SLAVE1_USE);
				break;					
			case SICMD_RO_Video_progress_Status:
				((VideoProgressStatusResPkt_t fdata*)gExtDebugCmdPtr)->Length = (sizeof(VideoProgressStatusResPkt_t)-2)+3;//+3:length,command,null-terminated
				SocIPCI2CSlaveWriteCmdPkt((BYTE far*)gExtDebugCmdPtr , B_whichHandler-I2C_SLAVE1_USE);
				break;					
			case SICMD_RO_NR_Status:
				((NRStatusResPkt_t fdata*)gExtDebugCmdPtr)->Length = (sizeof(NRStatusResPkt_t)-2)+3;//+3:length,command,null-terminated
				SocIPCI2CSlaveWriteCmdPkt((BYTE far*)gExtDebugCmdPtr , B_whichHandler-I2C_SLAVE1_USE);
				break;				
			case SICMD_RO_Video_progress1_Status:
				((VideoProgress_1StatusResPkt_t fdata*)gExtDebugCmdPtr)->Length = (sizeof(VideoProgress_1StatusResPkt_t)-2)+3;//+3:length,command,null-terminated
				SocIPCI2CSlaveWriteCmdPkt((BYTE far*)gExtDebugCmdPtr , B_whichHandler-I2C_SLAVE1_USE);
				break;					
			case SICMD_RO_DisplaySetup_Status:
				((DisplaySetupStatusResPkt_t fdata*)gExtDebugCmdPtr)->Length = (sizeof(DisplaySetupStatusResPkt_t)-2)+3;//+3:length,command,null-terminated
				SocIPCI2CSlaveWriteCmdPkt((BYTE far*)gExtDebugCmdPtr , B_whichHandler-I2C_SLAVE1_USE);
				break;				
			case SICMD_RO_Audio1_Status:
				((Audio1StatusResPkt_t fdata*)gExtDebugCmdPtr)->Length = (sizeof(Audio1StatusResPkt_t)-2)+3;//+3:length,command,null-terminated
				SocIPCI2CSlaveWriteCmdPkt((BYTE far*)gExtDebugCmdPtr , B_whichHandler-I2C_SLAVE1_USE);
				break;				
			case SICMD_RO_Audio2_Status:
				((Audio2StatusResPkt_t fdata*)gExtDebugCmdPtr)->Length = (sizeof(Audio2StatusResPkt_t)-2)+3;//+3:length,command,null-terminated
				SocIPCI2CSlaveWriteCmdPkt((BYTE far*)gExtDebugCmdPtr , B_whichHandler-I2C_SLAVE1_USE);
				break;					
			case SICMD_RO_Setup_Status:
				((SetupStatusResPkt_t fdata*)gExtDebugCmdPtr)->Length = (sizeof(SetupStatusResPkt_t)-2)+3;//+3:length,command,null-terminated
				SocIPCI2CSlaveWriteCmdPkt((BYTE far*)gExtDebugCmdPtr , B_whichHandler-I2C_SLAVE1_USE);
				break;
         case SICMD_RO_DCR_Result:
				((DcrResultResPkt_t fdata*)gExtDebugCmdPtr)->Length = (sizeof(DcrResultResPkt_t)-2)+3;//+3:length,command,null-terminated
				SocIPCI2CSlaveWriteCmdPkt((BYTE far*)gExtDebugCmdPtr , B_whichHandler-I2C_SLAVE1_USE);
				break;
			/*ChengLin 20130128: New Sync Handler states*/				
			case SICMD_RW_IMAGE_SYNC_STATE_Status:
				((ImageSyncStateStatusResPkt_t fdata*)gExtDebugCmdPtr)->Length = (sizeof(ImageSyncStateStatusResPkt_t)-2)+3;//+3:length,command,null-terminated
				SocIPCI2CSlaveWriteCmdPkt((BYTE far*)gExtDebugCmdPtr , B_whichHandler-I2C_SLAVE1_USE);
				break;
			/***************************************/	
			case SICMD_RO_Slave_Registers_read_data:				
				((SlaveRegistersDataResPkt_t fdata*)gExtDebugCmdPtr)->Length = (sizeof(SlaveRegistersDataResPkt_t)-2)+3;//+3:length,command,null-terminated
				SocIPCI2CSlaveWriteCmdPkt((BYTE far*)gExtDebugCmdPtr , B_whichHandler-I2C_SLAVE1_USE);	
				break;
			case SICMD_RO_DDCI_Status:
				((DDCCIStatusResPkt_t fdata*)gExtDebugCmdPtr)->Length = (sizeof(DDCCIStatusResPkt_t)-2)+3;//+3:length,command,null-terminated
				SocIPCI2CSlaveWriteCmdPkt((BYTE far*)gExtDebugCmdPtr , B_whichHandler-I2C_SLAVE1_USE);	
				break;
            
      #ifdef DP_HUB_2_LOG_PORTS            
         case SICMD_RO_DpRx0_IsStable:
            ((DpRx0IsStableResPkt_t fdata*)gExtDebugCmdPtr)->Length = (sizeof(DpRx0IsStableResPkt_t)-2)+3;//+3:length,command,null-terminated
            SocIPCI2CSlaveWriteCmdPkt((BYTE far*)gExtDebugCmdPtr , B_whichHandler-I2C_SLAVE1_USE); 
         break;         
      #endif
      
      #ifdef SMART_ISP
         case SICMD_RO_SMARTISP_STATUS:
            ((SmartIspStatusResPkt_t fdata*)gExtDebugCmdPtr)->Length = (sizeof(SmartIspStatusResPkt_t)-2)+3;//+3:length,command,null-terminated
            SocIPCI2CSlaveWriteCmdPkt((BYTE far*)gExtDebugCmdPtr , B_whichHandler-I2C_SLAVE1_USE); 
         break;         
      #endif
			default: //ACK
		_fmemcpy((BYTE far*)gExtDebugCmdPtr, (BYTE far*)IPC_AckPakt, 3);
		SocIPCI2CSlaveWriteCmdPkt((BYTE far*)gExtDebugCmdPtr , B_whichHandler-I2C_SLAVE1_USE);	
				break;
		}
	}
	else
	{
		_fmemcpy((BYTE far*)gExtDebugCmdPtr, (BYTE far*)IPC_NakPakt, 3);
		SocIPCI2CSlaveWriteCmdPkt((BYTE far*)gExtDebugCmdPtr , B_whichHandler-I2C_SLAVE1_USE);
	}

#if ENABLE_SOC_IPC_BUSY_CHECK
/*ChengLin 20130124: New 4k2k board BUSY status check*/
	if(GetChipID_4K2K() == CHIPID_4K2K_L)
		SocIPCSetClearSlaveLeftBusyStatus(FALSE);
	else if(GetChipID_4K2K() == CHIPID_4K2K_FE)
		SocIPCSetClearSlaveFEBusyStatus(FALSE); 
#endif
   #ifdef ST_4K2K_93xx_BOARD 
   if(GetChipID_4K2K()!=CHIPID_4K2K_R)
   {
	   SOCIPC_EndOfSlaveCmdParser(&SOCIPC_Slave_UpdateFlag);
   }
   #endif

	return PS_Status;
   
}

//******************************************************************************
// FUNCTION:   SOCIPC_EndOfSlaveCmdParser
// USAGE:  Based on  Athena Right Commands to process Adjustment functions
//
// INPUT:  Update
// OUTPUT: None
// GLOBALS:
//******************************************************************************
void SOCIPC_EndOfSlaveCmdParser(SOCIPC_SlaveUpdateStatus_t* Update)
{

	if(Update->Update_CT_User_Index)
	{
	/*ChengLin 20130129: Enable Bidirectional BUSY Status Check*/
	#if (ENABLE_BIDIRECTIONAL_BUSY_CHECK)
			SocIPCWaitAthenaMasterReady();
	#endif		
		switch(Update->Update_CT_User_Index)
		{
			case CT_USER_RED_GAIN:		
				AdjustRedGain();
			break;
			case CT_USER_GREEN_GAIN:
				AdjustGreenGain();				
			break;
			case CT_USER_BLUE_GAIN:
				AdjustBlueGain();				
			break;
			case CT_USER_RED_OFFSET:
				AdjustRedOff();				
			break;
			case CT_USER_GREEN_OFFSET:
				AdjustGreenOff();				
			break;
			case CT_USER_BLUE_OFFSET:
				AdjustBlueOff();				
			break;
			default:
			break;	
		}
		Update->Update_CT_User_Index = CT_USER_NONE;		
	}

	if(Update->Update_PortControl)
	{
		Update->Update_PortControl = FALSE;	
		UserPrefCurrentInputMain = gAthena_RIGHT_Master_Parameters.PortStatus;	
		gm_ClearRegBitsWord(PB_CTRL, PB_MAIN_ENABLE);
		gm_SetRegBitsWord(PB_CTRL, PB_FORCE_MAIN_BKGND);
		gm_ForceUpdate(CH_A , INPUT_OUTPUT_CLOCK);
		//gm_Delay10ms(40); // 20130110
		AdjustCurrentInputMain();
	}
   
	if(Update->Update_Color)
	{
		Update->Update_Color = FALSE;
		socipc_recv_msg("Update color data .... %d", 111);
	/*ChengLin 20130129: Enable Bidirectional BUSY Status Check*/
	#if (ENABLE_BIDIRECTIONAL_BUSY_CHECK)
		SocIPCWaitAthenaMasterReady();		
	#else //Original
		gm_Delay1ms(50);
	#endif			
		CscUpdateColors(gmvb_MainChannel);
	}

	if(Update->Update_Sharpness)
	{
		Update->Update_Sharpness = FALSE;
	/*ChengLin 20130129: Enable Bidirectional BUSY Status Check*/
	#if (ENABLE_BIDIRECTIONAL_BUSY_CHECK)
		SocIPCWaitAthenaMasterReady();		
	#endif			
		AdjustSharpness();
		AdjustSharpnessRGB(); //Added by ChengLin 20130108
	}

	if(Update->Update_ColorTemperature)
	{
		Update->Update_ColorTemperature = FALSE;
	/*ChengLin 20130129: Enable Bidirectional BUSY Status Check*/		
	#if (ENABLE_BIDIRECTIONAL_BUSY_CHECK)
		SocIPCWaitAthenaMasterReady();		
	#else //Original
		gm_Delay1ms(170); //Add for sync Athena R and L 	
	#endif		
		AdjustColorTemp();
	}

	if(Update->Update_ImageScheme)
	{
		Update->Update_ImageScheme = FALSE;
	/*ChengLin 20130129: Enable Bidirectional BUSY Status Check*/		
	#if (ENABLE_BIDIRECTIONAL_BUSY_CHECK)
		SocIPCWaitAthenaMasterReady();		
	#else //Original
		gm_Delay1ms(170); //Add for sync Athena R and L 	
	#endif			
		AdjustImageScheme();
	}

	if(Update->Update_CSC)
	{
		Update->Update_CSC = FALSE;
	/*ChengLin 20130129: Enable Bidirectional BUSY Status Check*/
	#if (ENABLE_BIDIRECTIONAL_BUSY_CHECK)
		SocIPCWaitAthenaMasterReady();		
	#endif		
		AdjustColorSpace();
		socipc_recv_msg("\nCSC_Update = TRUE\n",0);
	}
		
	if(Update->Update_DUM)
	{
		Update->Update_DUM = FALSE;
	/*ChengLin 20130129: Enable Bidirectional BUSY Status Check*/
	#if (ENABLE_BIDIRECTIONAL_BUSY_CHECK)
		SocIPCWaitAthenaMasterReady();		
	#endif		
		AdjustDUMSwitch();
		socipc_recv_msg("\nDUM_Update = TRUE\n",0);		
	}

	if(Update->Update_DCDI)
	{
		Update->Update_DCDI = FALSE;
	/*ChengLin 20130129: Enable Bidirectional BUSY Status Check*/
	#if (ENABLE_BIDIRECTIONAL_BUSY_CHECK)
		SocIPCWaitAthenaMasterReady();		
	#endif			
		AdjustDCDiMain();
		socipc_recv_msg("\nDCDI_Update = TRUE\n",0);
	}
		
	if(Update->Update_MADI)
	{
		Update->Update_MADI = FALSE;
	/*ChengLin 20130129: Enable Bidirectional BUSY Status Check*/
	#if (ENABLE_BIDIRECTIONAL_BUSY_CHECK)
		SocIPCWaitAthenaMasterReady();		
	#endif			
		AdjustMADI();
		socipc_recv_msg("\nMADI_Update = TRUE\n",0);		
	}
		
	if(Update->Update_OD)
	{
		Update->Update_OD = FALSE;
	/*ChengLin 20130129: Enable Bidirectional BUSY Status Check*/
	#if (ENABLE_BIDIRECTIONAL_BUSY_CHECK)
		SocIPCWaitAthenaMasterReady();		
	#endif			
		AdjustLCDOverdrive();
		socipc_recv_msg("\nOD_Setting_Update = TRUE\n",0);		
	}

	if(Update->Update_TNR)
	{
		Update->Update_TNR = FALSE;
	/*ChengLin 20130129: Enable Bidirectional BUSY Status Check*/
	#if (ENABLE_BIDIRECTIONAL_BUSY_CHECK)
		SocIPCWaitAthenaMasterReady();		
	#endif			
		AdjustTNR();
		socipc_recv_msg("\nTNR_Update = TRUE\n",0);			
	}
		
	if(Update->Update_MEPG_NR)
	{
		Update->Update_MEPG_NR = FALSE;
	/*ChengLin 20130129: Enable Bidirectional BUSY Status Check*/
	#if (ENABLE_BIDIRECTIONAL_BUSY_CHECK)
		SocIPCWaitAthenaMasterReady();		
	#endif			
		AdjustMPEGNRMode();
		socipc_recv_msg("\nMPEG_NR_Update = TRUE\n",0);		
	}

	if(Update->Update_FilmMode)
	{
		Update->Update_FilmMode = FALSE; 
	/*ChengLin 20130129: Enable Bidirectional BUSY Status Check*/
	#if (ENABLE_BIDIRECTIONAL_BUSY_CHECK)
		SocIPCWaitAthenaMasterReady();		
	#endif			
		AdjustFilmMode();
		socipc_recv_msg("\nFilm_Mode_Update = TRUE\n",0);		
	}

	if(Update->Update_AspectRatio)
	{
		Update->Update_AspectRatio = FALSE;		
	/*ChengLin 20130129: Enable Bidirectional BUSY Status Check*/		
	#if (ENABLE_BIDIRECTIONAL_BUSY_CHECK)
		SocIPCWaitAthenaMasterReady();		
	#else //Original
		gm_Delay1ms(200); //Add for sync Athena R and L		
	#endif		
		AdjustAspectRatioMain();
	}

	if(Update->Update_DCR)
	{
		Update->Update_DCR = FALSE;
	/*ChengLin 20130129: Enable Bidirectional BUSY Status Check*/
	#if (ENABLE_BIDIRECTIONAL_BUSY_CHECK)
		SocIPCWaitAthenaMasterReady();		
	#endif			
		AdjustDCR();
		socipc_recv_msg("\nDCR_Update = TRUE\n",0);			
	}
		
	if(Update->Update_FactoryReset)
	{
		Update->Update_FactoryReset = FALSE;
	/*ChengLin 20130129: Enable Bidirectional BUSY Status Check*/
	#if (ENABLE_BIDIRECTIONAL_BUSY_CHECK)
		SocIPCWaitAthenaMasterReady();		
	#endif			
		FactoryReset();
		socipc_recv_msg("\nFactoryReset_Update = TRUE\n",0);		
	}
		
	if(Update->Update_DPSwitch)
	{
		Update->Update_DPSwitch = FALSE;
		DpAppInit();	
	}

	if(Update->Update_UserData)
	{
		Update->Update_UserData = FALSE;
		socipc_recv_msg("Update User data .... %d", 222);
		SaveModeIndependentSettings();
		SavePortSchemeDependentSettings();
	}

	/*ChengLin 20130117: Modified for Sync DDC/CI commands for Athena Right/Left*/
	if(Update->Update_DDCCI)
	{
		Update->Update_DDCCI = FALSE;
		socipc_recv_msg("\nAthena RIGHT DDC VCP Code = 0x%X.\n",gAthena_RIGHT_Master_Parameters.DDC_VCPCode);
		socipc_recv_msg("\nAthena RIGHT DDC Value = 0x%X.\n",gAthena_RIGHT_Master_Parameters.DDC_Value);
		SocIPC_SlaveSetDDCCIActionHandler(gAthena_RIGHT_Master_Parameters.DDC_VCPCode, gAthena_RIGHT_Master_Parameters.DDC_Value);	
	}
		
	if(Update->Update_SixAxisColor_Index)
	{
		/*ChengLin 20130129: Enable Bidirectional BUSY Status Check*/
	#if (ENABLE_BIDIRECTIONAL_BUSY_CHECK)
			SocIPCWaitAthenaMasterReady();
	#endif
   		switch(Update->Update_SixAxisColor_Index)
   		{
   			//Saturation
	  		case SIX_AXIS_COLOR_RED_SAT:
	   			AdjustAcmRedSat();
	   		break;
	  		case SIX_AXIS_COLOR_GREEN_SAT:
	   			AdjustAcmGreenSat();
	   		break;
	  		case SIX_AXIS_COLOR_BLUE_SAT:
	   			AdjustAcmBlueSat();
	   		break;
	  		case SIX_AXIS_COLOR_YELLOW_SAT:
	   			AdjustAcmYellowSat();
	  	 	break;
	  		case SIX_AXIS_COLOR_MAGENTA_SAT:
	   			AdjustAcmMagentaSat();
	   		break;
	  		case SIX_AXIS_COLOR_CYAN_SAT:
	   			AdjustAcmCyanSat();
	   		break;
	  		case SIX_AXIS_COLOR_BLACK_SAT:
	   			AdjustAcmBlackSat();
	   		break;
	  		case SIX_AXIS_COLOR_WHITE_SAT:
	   			AdjustAcmWhiteSat();
	   		break;
			//Hue
	  		case SIX_AXIS_COLOR_RED_HUE:
	   			AdjustAcmRedHue();
	   		break;
	  		case SIX_AXIS_COLOR_GREEN_HUE:
	   			AdjustAcmGreenHue();
	   		break;
	  		case SIX_AXIS_COLOR_BLUE_HUE:
	   			AdjustAcmBlueHue();
	   		break;
	  		case SIX_AXIS_COLOR_YELLOW_HUE:
	   			AdjustAcmYellowHue();
	   		break;
	  		case SIX_AXIS_COLOR_MAGENTA_HUE:
	   			AdjustAcmMagentaHue();
	   		break;
	  		case SIX_AXIS_COLOR_CYAN_HUE:
	   			AdjustAcmCyanHue();
	   		break;
	  		case SIX_AXIS_COLOR_BLACK_HUE:
	   			AdjustAcmBlackHue();
	   		break;
	  		case SIX_AXIS_COLOR_WHITE_HUE:
	   			AdjustAcmWhiteHue();
	   		break;
   			//Luma
	  		case SIX_AXIS_COLOR_RED_LUMA:
	   			AdjustAcmRedLuma();
	   		break;
	  		case SIX_AXIS_COLOR_GREEN_LUMA:
	   			AdjustAcmGreenLuma();
	   		break;
	  		case SIX_AXIS_COLOR_BLUE_LUMA:
	   			AdjustAcmBlueLuma();
	   		break;
	  		case SIX_AXIS_COLOR_YELLOW_LUMA:
	   			AdjustAcmYellowLuma();
	   		break;
	  		case SIX_AXIS_COLOR_MAGENTA_LUMA:
	   			AdjustAcmMagentaLuma();
	   		break;
	  		case SIX_AXIS_COLOR_CYAN_LUMA:
	   			AdjustAcmCyanLuma();
	   		break;
	  		case SIX_AXIS_COLOR_BLACK_LUMA:
	   			AdjustAcmBlackLuma();
	   		break;
	  		case SIX_AXIS_COLOR_WHITE_LUMA:
	   			AdjustAcmWhiteHue();
	   		break; 
	  		default:
	   		break;
   			}
		Update->Update_SixAxisColor_Index = SIX_AXIS_COLOR_NONE;
	}

/*ChengLin 20130222: Add for sync Left/Right PB Control Status*/
#if ENABLE_ATHENA_R_L_IMAGE_SYNC_HANDLER
	if(Update->Update_PB_Ctrl)
	{
	#if (ENABLE_BIDIRECTIONAL_BUSY_CHECK)
		SocIPCWaitAthenaMasterReady();		
	#endif
		if(GetChipID_4K2K()==CHIPID_4K2K_L)	
		SOCIPC_SetAthena_Right_Left_PB_Ctrl_Status((BOOL)gAthena_RIGHT_Master_Parameters.ImageSyncBlackScreen);		
		Update->Update_PB_Ctrl = FALSE;
	}
#endif

	if(Update->Update_GammaCorrection) //Add by ChengLin 20130226
	{
	#if (ENABLE_BIDIRECTIONAL_BUSY_CHECK)
		SocIPCWaitAthenaMasterReady();		
	#endif
		AdjustGammaCorrection_Ext();
		Update->Update_GammaCorrection = FALSE;
	}	

   if(Update->Update_SplashLoadDef) //Add by ChengLin 20130322
   {
#if (ENABLE_BIDIRECTIONAL_BUSY_CHECK)
      SocIPCWaitAthenaMasterReady();      
#endif
      SOCIPC_LoadSplashScreenColorSettings((BOOL)gAthena_RIGHT_Master_Parameters.SplashLoadDefault);
      Update->Update_SplashLoadDef = FALSE;
   }

#ifdef DP_HUB_2_LOG_PORTS
      if(Update->Update_Redo_ModeSetup) //Add by ChengLin 20130322
      {
#if (ENABLE_BIDIRECTIONAL_BUSY_CHECK)
         SocIPCWaitAthenaMasterReady();      
#endif
#if (ENABLE_DPRX0_RE_MODE_SETUP)  
         AthenaR_L_Redo_ModeSetup();
#endif
         Update->Update_Redo_ModeSetup = FALSE;
      }     
#endif

}

//******************************************************************************
// FUNCTION:   SoCIPC_SlavDevHandler
// USAGE:  SoC IPC Athena-LEFT/FE commands handler
//
// INPUT:  None
// OUTPUT:	Parser status 
// GLOBALS        : 
//******************************************************************************
BYTE SoCIPC_SlavDevHandler(SLAVE_CH B_SlaveCh)
{
   PARSER_STATUS_t B_Status = NO_MSG_PARSED;
   I2C_SLAVE_TYPE* Sp_I2CSlavePtr = Sp_I2CSlavePtrGlobal[B_SlaveCh];
			
   gm_I2CSlaveSetChannel(B_SlaveCh);
			
   if(IsCompleted_SOC_IPC_I2C_Msg(B_SlaveCh))
	{
      if ((I2C_SOC_IPC_CMD(B_SlaveCh) == SOC_IPC_TX_VCP_PREFIX_CMD) &&
         (I2C_SOC_ManufacturerID(B_SlaveCh) == SOC_IPC_TX_VCP_PREFIX_CMD_PAGE))
		{
         BYTE B_whichHandler = B_SlaveCh + I2C_SLAVE1_USE;
         B_Status = SoCIPC_SlavDevCmdParser((BYTE far*)&Sp_I2CSlavePtr->Buffer[Start_of_SoC_IPC_Cmd_in_DDC2BiMsg],B_whichHandler);		 
			
         // clear read fifo
         gm_SetRegBitsByte(I2C_SLAVE_CTRL2, I2C_SLAVE1_READ_FIFO_RESET);
         gm_ClearRegBitsByte(I2C_SLAVE_CTRL2, I2C_SLAVE1_READ_FIFO_RESET);
		}
		else
		{
         B_Status = UNKNOWN_MSG_CMD;// extern it will be solved the ddc2bi commands,
		}
	}

   return B_Status;
}


//******************************************************************************
// FUNCTION:   SocIPCSrcPacketTransmitter
// USAGE:  Tansmit gSoCIPCTransPacket via I2C Bus
//
// INPUT:  Cmd
// OUTPUT:	TRUE(I2C Wrire/Read Finished) , FALSE(I2C Write/Read Failed)
// GLOBALS:
//******************************************************************************
BOOL SocIPCSrcPacketTransmitter(gmt_SocIPCCommandCode Cmd)
{
	BYTE CmdTxBuff[SOC_IPC_TRX_BUF_SIZE];
	WORD wCount = 0;

	memset(CmdTxBuff, 0, SOC_IPC_TRX_BUF_SIZE);

	//Calculate Transmitter Packet Length(Base on Message Packet Length)
	gSoCIPCTransPacket.TransPacketLength = (0x80|(gSoCIPCTransPacket.MessageLength+2));

	//Prepare Tansmitter Packet
	CmdTxBuff[0] = gSoCIPCTransPacket.DestDevSlaveAddr;
	CmdTxBuff[1] = gSoCIPCTransPacket.SrcDevMasterAddr;
	CmdTxBuff[2] = gSoCIPCTransPacket.TransPacketLength;
	CmdTxBuff[3] = gSoCIPCTransPacket.VCPPrefixCmd;
	CmdTxBuff[4] = gSoCIPCTransPacket.VCPPrefixPage1;	
	CmdTxBuff[5] = gSoCIPCTransPacket.VCPPrefixPage2;
	CmdTxBuff[6] = gSoCIPCTransPacket.MessageLength;
	CmdTxBuff[7] = gSoCIPCTransPacket.MessageCmd;

	//Prepare Message Buffer Bytes if needed
	if( gSoCIPCTransPacket.MessageLength > SOCIPC_STD_MSG_LEN)
	{
		for(wCount = 0; wCount < (gSoCIPCTransPacket.MessageLength - SOCIPC_STD_MSG_LEN); wCount++)			
			CmdTxBuff[8+wCount] = gSoCIPCTransPacket.MessageBuf[wCount];
	}

	//Calculate Packet CheckSum and determine chksum byte
	CmdTxBuff[(gSoCIPCTransPacket.TransPacketLength&0x7F)+3] = SocIPCTxGetCheckSum(CmdTxBuff,(gSoCIPCTransPacket.TransPacketLength&0x7F)+4);


	if(CmdTxBuff != NULL)
	{
#if ENABLE_SOC_IPC_BUSY_CHECK
		if( gSoCIPCTransPacket.DestDevSlaveAddr == SOCIPC_DEVICEL_DST_ADDRESS )
		{
			if(!SocIPCIsSlaveLeftReady())
			{		
				socipc_busychk_msg("\n<Soc IPC Error> SocIPC Slave Device LEFT Not Ready.\n",0);
				return FALSE;
			}			
		}
		else if( gSoCIPCTransPacket.DestDevSlaveAddr == SOCIPC_DEVICEFE_DST_ADDRESS )
		{	
			if(!SocIPCIsSlaveFEReady())
			{		
				socipc_busychk_msg("\n<Soc IPC Error> SocIPC Slave Device FE Not Ready.\n",0);
				return FALSE;
			}
		}
#endif //ENABLE_SOC_IPC_BUSY_CHECK	


/*ChengLin 20130129: Enable Bidirectional BUSY Status Check*/
#if (ENABLE_BIDIRECTIONAL_BUSY_CHECK)
	if(IsInitialFinished) //ChengLin 20130129: Bi-directional will be enabled after Athena R Initial finished
	{
		if( gSoCIPCTransPacket.DestDevSlaveAddr == SOCIPC_DEVICEL_DST_ADDRESS )		
			SocIPCSetClearSlaveLeftBusyStatus(TRUE); //Set Athena R to Busy for sync Athena Left
		else if( gSoCIPCTransPacket.DestDevSlaveAddr == SOCIPC_DEVICEFE_DST_ADDRESS )
			SocIPCSetClearSlaveFEBusyStatus(TRUE); //Set Athena R to Busy for sync Athena FE			
	}
#endif	
		if(gm_Write2WireBlock(0, gSoCIPCTransPacket.DestDevSlaveAddr, &CmdTxBuff[1], (gSoCIPCTransPacket.TransPacketLength&0x7F)+3, TRUE) == gmd_PASS)
		{
#if ENABLE_SOC_IPC_DEBUG	
			for(wCount = 0; wCount < ((gSoCIPCTransPacket.TransPacketLength&0x7F)+4) ; wCount++)
			{
				socipc_msg_ex("\nCmdTxBuff[%d] = 0x%X.\n",wCount, CmdTxBuff[wCount]);
			}
			socipc_msg("\n",0);
#endif //ENABLE_SOC_IPC_DEBUG

         #ifdef USE_SMT
            if (Cmd == SICMD_WO_SMT_COMMAND)
            {
            //separated SMT command. some SMT needs more time to get response
                BYTE ReTryCounter = 2;
//              DWORD  CurrentTime = gm_GetSystemTime();

                while(ReTryCounter)
                {
                    gm_TimerStart(DELAY_1ms_TMR, SMT_INTERCHIP_COMM_TIMEOUT);//start 2000ms timer
                    while(gm_TimerCheck(DELAY_1ms_TMR) != TIMER_TMO)
                    {
                        /*ChengLin 20120208: If set this delay time too small, it will cause Athena LEFT or Athena FE to miss commands*/
                        //gm_Delay1ms(100); //Wait time for read status
//                      if(gm_ReadRegWord(OCM_SCRATCHPAD))
//                         SMTDelayTime = gm_ReadRegWord(OCM_SCRATCHPAD);
                        gm_Delay1ms(/*SMTDelayTime*/SMT_INTERCHIP_COMM_DELAY_TIME); //Wait time for read status

						gm_2WireSelectIdx (MASTER_2WIRE_2);
                        gGetAckFromSlave = SocIPCTxReceivePacketCheck( gSoCIPCTransPacket.DestDevSlaveAddr, gSoCIPCTransPacket.MessageCmd ); //Get Ack or valid data
						gm_2WireSelectIdx (MASTER_2WIRE_1);

                        if( gGetAckFromSlave )
                        {
                            gGetAckFromSlave = FALSE;

							/*ChengLin 20130129: Enable Bidirectional BUSY Status Check*/							
							#if (ENABLE_BIDIRECTIONAL_BUSY_CHECK)
							if(IsInitialFinished)//ChengLin 20130129: Bi-directional will be enabled after Athena R Initial finished
							{
								if( gSoCIPCTransPacket.DestDevSlaveAddr == SOCIPC_DEVICEL_DST_ADDRESS )
									SocIPCSetClearSlaveLeftBusyStatus(FALSE); //Set READY to Athena Left
								else if( gSoCIPCTransPacket.DestDevSlaveAddr == SOCIPC_DEVICEFE_DST_ADDRESS )
									SocIPCSetClearSlaveFEBusyStatus(FALSE); //Set READY to Athena FE									
							}
							#endif							
//                          gm_Print("SMT Cmd resp time = %d",(WORD)(gm_GetSystemTime() - CurrentTime));
                            return TRUE;
                        }
                    }
                    ReTryCounter--;
                    if(gm_Write2WireBlock(0, gSoCIPCTransPacket.DestDevSlaveAddr, &CmdTxBuff[1], (gSoCIPCTransPacket.TransPacketLength&0x7F)+3, TRUE) != gmd_PASS)
                        break;
                }
				/*ChengLin 20130129: Enable Bidirectional BUSY Status Check*/							
				#if (ENABLE_BIDIRECTIONAL_BUSY_CHECK)
				if(IsInitialFinished)//ChengLin 20130129: Bi-directional will be enabled after Athena R Initial finished
				{
					if( gSoCIPCTransPacket.DestDevSlaveAddr == SOCIPC_DEVICEL_DST_ADDRESS )
						SocIPCSetClearSlaveLeftBusyStatus(FALSE); //Set READY to Athena Left
					else if( gSoCIPCTransPacket.DestDevSlaveAddr == SOCIPC_DEVICEFE_DST_ADDRESS )
						SocIPCSetClearSlaveFEBusyStatus(FALSE); //Set READY to Athena FE									
				}
				#endif	
                return FALSE;
            }
            else
            #endif
            {

			/*ChengLin 20130129: If set this delay time too small, it will cause Athena LEFT or Athena FE to miss commands.
								Add for avoid error checksum calaulation (Based on I2C 800Kbps)
			*/
			if(Cmd == SICMD_RW_DisplaySetup_CONTRL)
				gm_Delay1ms(220); //Wait time for read status
			else if(Cmd == SICMD_RW_Sharpness_CONTRL) 
				gm_Delay1ms(140); //Wait time for read status				
			else if(Cmd == SICMD_RW_Color_CONTRL)
				gm_Delay1ms(170); //Wait time for read status
			else if(Cmd == SICMD_RW_Video_progress_CONTRL)
				gm_Delay1ms(75); //Wait time for read status	
			else if(Cmd == SICMD_RW_NR_CONTRL)
				gm_Delay1ms(80); //Wait time for read status
			else if(Cmd == SICMD_RW_Video_progress1_CONTRL)
				gm_Delay1ms(85); //Wait time for read status				
			else
				gm_Delay1ms(55); //Wait time for read status
			gm_2WireSelectIdx (MASTER_2WIRE_2);
			gGetAckFromSlave = 	SocIPCTxReceivePacketCheck( gSoCIPCTransPacket.DestDevSlaveAddr, gSoCIPCTransPacket.MessageCmd ); //Get Ack or valid data
			gm_2WireSelectIdx (MASTER_2WIRE_1);

			if( gGetAckFromSlave )
			{
				gGetAckFromSlave = FALSE;
				/*ChengLin 20130129: Enable Bidirectional BUSY Status Check*/							
				#if (ENABLE_BIDIRECTIONAL_BUSY_CHECK)
				if(IsInitialFinished)//ChengLin 20130129: Bi-directional will be enabled after Athena R Initial finished
				{
					if( gSoCIPCTransPacket.DestDevSlaveAddr == SOCIPC_DEVICEL_DST_ADDRESS )
						SocIPCSetClearSlaveLeftBusyStatus(FALSE); //Set READY to Athena Left
					else if( gSoCIPCTransPacket.DestDevSlaveAddr == SOCIPC_DEVICEFE_DST_ADDRESS )
						SocIPCSetClearSlaveFEBusyStatus(FALSE); //Set READY to Athena FE									
				}
				#endif			
				return TRUE;
			}
			else //Re-Send commands
			{
				if(gm_Write2WireBlock(0, gSoCIPCTransPacket.DestDevSlaveAddr, &CmdTxBuff[1], (gSoCIPCTransPacket.TransPacketLength&0x7F)+3, TRUE) == gmd_PASS)
				{
#if ENABLE_SOC_IPC_DEBUG	
					for(wCount = 0; wCount < ((CmdTxBuff[2]&0x7f)+4) ; wCount++)
					{
						socipc_msg_ex("\nRe-Send CmdTxBuff[%d] = 0x%X.\n",wCount, CmdTxBuff[wCount]);
					}
					socipc_msg("\n",0);
#endif //ENABLE_SOC_IPC_DEBUG
			/*ChengLin 20130129: If set this delay time too small, it will cause Athena LEFT or Athena FE to miss commands.
								Add for avoid error checksum calaulation (Based on I2C 800Kbps)
			*/				
					if(Cmd == SICMD_RW_DisplaySetup_CONTRL)
						gm_Delay1ms(220); //Wait time for read status
					else if(Cmd == SICMD_RW_Sharpness_CONTRL) 
						gm_Delay1ms(140); //Wait time for read status				
					else if(Cmd == SICMD_RW_Color_CONTRL)
						gm_Delay1ms(170); //Wait time for read status
					else if(Cmd == SICMD_RW_Video_progress_CONTRL)
						gm_Delay1ms(75); //Wait time for read status	
					else if(Cmd == SICMD_RW_NR_CONTRL)
						gm_Delay1ms(80); //Wait time for read status
					else if(Cmd == SICMD_RW_Video_progress1_CONTRL)
						gm_Delay1ms(85); //Wait time for read status				
					else
						gm_Delay1ms(55); //Wait time for read status

					gm_2WireSelectIdx (MASTER_2WIRE_2);
					gGetAckFromSlave =	SocIPCTxReceivePacketCheck( gSoCIPCTransPacket.DestDevSlaveAddr, gSoCIPCTransPacket.MessageCmd ); //Get Ack or valid data
					gm_2WireSelectIdx (MASTER_2WIRE_1);

					if( gGetAckFromSlave )
					{
						gGetAckFromSlave = FALSE;					
						socipc_msg("\n<Soc IPC Message> Re-Send Command Successful.\n",0);
						/*ChengLin 20130129: Enable Bidirectional BUSY Status Check*/							
						#if (ENABLE_BIDIRECTIONAL_BUSY_CHECK)
						if(IsInitialFinished)//ChengLin 20130129: Bi-directional will be enabled after Athena R Initial finished
						{
							if( gSoCIPCTransPacket.DestDevSlaveAddr == SOCIPC_DEVICEL_DST_ADDRESS )
								SocIPCSetClearSlaveLeftBusyStatus(FALSE); //Set READY to Athena Left
							else if( gSoCIPCTransPacket.DestDevSlaveAddr == SOCIPC_DEVICEFE_DST_ADDRESS )
								SocIPCSetClearSlaveFEBusyStatus(FALSE); //Set READY to Athena FE									
						}
						#endif						
						return TRUE;
					}
					else
					{					
						socipc_msg("\n<Soc IPC Error> Re-Send Command Failure.\n",0);
						/*ChengLin 20130129: Enable Bidirectional BUSY Status Check*/							
						#if (ENABLE_BIDIRECTIONAL_BUSY_CHECK)
						if(IsInitialFinished)//ChengLin 20130129: Bi-directional will be enabled after Athena R Initial finished
						{
							if( gSoCIPCTransPacket.DestDevSlaveAddr == SOCIPC_DEVICEL_DST_ADDRESS )
								SocIPCSetClearSlaveLeftBusyStatus(FALSE); //Set READY to Athena Left
							else if( gSoCIPCTransPacket.DestDevSlaveAddr == SOCIPC_DEVICEFE_DST_ADDRESS )
								SocIPCSetClearSlaveFEBusyStatus(FALSE); //Set READY to Athena FE									
						}
						#endif						
						return FALSE;
					}
				}
				else
				{				
					socipc_msg("\n<Soc IPC Error> Re-Send Command I2C Failure.\n",0);
					/*ChengLin 20130129: Enable Bidirectional BUSY Status Check*/							
					#if (ENABLE_BIDIRECTIONAL_BUSY_CHECK)
					if(IsInitialFinished)//ChengLin 20130129: Bi-directional will be enabled after Athena R Initial finished
					{
						if( gSoCIPCTransPacket.DestDevSlaveAddr == SOCIPC_DEVICEL_DST_ADDRESS )
							SocIPCSetClearSlaveLeftBusyStatus(FALSE); //Set READY to Athena Left
						else if( gSoCIPCTransPacket.DestDevSlaveAddr == SOCIPC_DEVICEFE_DST_ADDRESS )
							SocIPCSetClearSlaveFEBusyStatus(FALSE); //Set READY to Athena FE									
					}
					#endif					
					return FALSE;
				}
			}
		}
		}
		else
		{	
			socipc_msg("\n<Soc IPC Error> IIC Write Command Failure.\n",0);
			/*ChengLin 20130129: Enable Bidirectional BUSY Status Check*/							
			#if (ENABLE_BIDIRECTIONAL_BUSY_CHECK)
			if(IsInitialFinished)//ChengLin 20130129: Bi-directional will be enabled after Athena R Initial finished
			{
				if( gSoCIPCTransPacket.DestDevSlaveAddr == SOCIPC_DEVICEL_DST_ADDRESS )
					SocIPCSetClearSlaveLeftBusyStatus(FALSE); //Set READY to Athena Left
				else if( gSoCIPCTransPacket.DestDevSlaveAddr == SOCIPC_DEVICEFE_DST_ADDRESS )
					SocIPCSetClearSlaveFEBusyStatus(FALSE); //Set READY to Athena FE									
			}
			#endif			
			return FALSE;
		}
	}
	else
	{	
		socipc_msg("\n<Soc IPC Error> Invalid Command.\n",0);
		/*ChengLin 20130129: Enable Bidirectional BUSY Status Check*/							
		#if (ENABLE_BIDIRECTIONAL_BUSY_CHECK)
		if(IsInitialFinished)//ChengLin 20130129: Bi-directional will be enabled after Athena R Initial finished
		{
			if( gSoCIPCTransPacket.DestDevSlaveAddr == SOCIPC_DEVICEL_DST_ADDRESS )
				SocIPCSetClearSlaveLeftBusyStatus(FALSE); //Set READY to Athena Left
			else if( gSoCIPCTransPacket.DestDevSlaveAddr == SOCIPC_DEVICEFE_DST_ADDRESS )
				SocIPCSetClearSlaveFEBusyStatus(FALSE); //Set READY to Athena FE									
		}
		#endif
		return FALSE;
	}

}

//******************************************************************************
// FUNCTION:   SocIPCSendCmdPacketParser
// USAGE:  Send Soc IPC commands via I2C Master
//
// INPUT:  DeviceID, Cmd, MsgParaBuf
// OUTPUT:	TRUE(I2C Wrire/Read Finished) , FALSE(I2C Write/Read Failed)
// GLOBALS:
//******************************************************************************
BOOL SocIPCSendCmdPacketParser(BYTE DeviceID, gmt_SocIPCCommandCode Cmd, BYTE* MsgParaBuf)
{
	BOOL IsTxCmdSupported = FALSE;
	BOOL IsTransmitted = FALSE;


	if( (DeviceID != SOCIPC_DEVICEL_DST_ADDRESS) && (DeviceID != SOCIPC_DEVICEFE_DST_ADDRESS))
	{	
		socipc_msg("\n<Soc IPC Error> Invalid Device ID.\n",0);
		return FALSE;
	}

   while(gm_TimerCheck(SOCIPC_SENDCMD_WAIT_1ms_TMR)==TIMER_OK)
   {
      //gm_Print("@w",0);
   }

	//Reset previous dettings to default and set common settings
	gSoCIPCTransPacket.DestDevSlaveAddr = DeviceID;
	gSoCIPCTransPacket.MessageLength = 0x00;	
	gSoCIPCTransPacket.MessageCmd = Cmd;
	memset(gSoCIPCTransPacket.MessageBuf, 0, SOCIPC_TRANS_MSG_MAX_SIZE);

	switch( Cmd )
	{

		case SICMD_RW_Power_CONTROL:
			//Command code from Athena RIGHT; 00: Normal; 01: B off; 10/11: Reserved
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(1);		
			gSoCIPCTransPacket.MessageBuf[0]= (MsgParaBuf[0]&0x03); // [1:0] Power control bits	
			IsTxCmdSupported = TRUE;
			break;
		case SICMD_RO_Power_Status:
			//Get Athena LEFT/FE Power Status; 00: Normal; 01: B off; 10/11: Reserved
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(0);
			IsTxCmdSupported = TRUE;		
			break;
	/*ChengLin 20130110: Color Temp - User Settings*/
		case SICMD_RW_CT_RedGain_Ctrl:
			//Send Athena RIGHT CT User Mode setting to Athena LEFT/FE
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(2);
			//Get Athena RIGHT User R Gain Value
			gSoCIPCTransPacket.MessageBuf[0]= ((UserPrefUserRedGain >> 8)&0x00FF); //High Byte
			gSoCIPCTransPacket.MessageBuf[1]= (UserPrefUserRedGain)&0x00FF; //Low Byte			
			IsTxCmdSupported = TRUE;
			socipc_msg("\nUserPrefUserRedGain = %d.\n",UserPrefUserRedGain);
			break;
		case SICMD_RW_CT_GreenGain_Ctrl:
			//Send Athena RIGHT CT User Mode setting to Athena LEFT/FE
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(2);
			//Get Athena RIGHT User G Gain Value
			gSoCIPCTransPacket.MessageBuf[0]= ((UserPrefUserGreenGain >> 8)&0x00FF); //High Byte
			gSoCIPCTransPacket.MessageBuf[1]= (UserPrefUserGreenGain)&0x00FF; //Low Byte			
			IsTxCmdSupported = TRUE;
			socipc_msg("\nUserPrefUserGreenGain = %d.\n",UserPrefUserGreenGain);			
			break;
		case SICMD_RW_CT_BlueGain_Ctrl:
			//Send Athena RIGHT CT User Mode setting to Athena LEFT/FE
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(2);
			//Get Athena RIGHT User B Gain Value
			gSoCIPCTransPacket.MessageBuf[0]= ((UserPrefUserBlueGain >> 8)&0x00FF); //High Byte
			gSoCIPCTransPacket.MessageBuf[1]= (UserPrefUserBlueGain)&0x00FF; //Low Byte			
			IsTxCmdSupported = TRUE;
			socipc_msg("\nUserPrefUserBlueGain = %d.\n",UserPrefUserBlueGain);			
			break;
		case SICMD_RW_CT_RedOffset_Ctrl:
			//Send Athena RIGHT CT User Mode setting to Athena LEFT/FE
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(1);
			gSoCIPCTransPacket.MessageBuf[0]= (BYTE)UserPrefUserRedOff;		
			IsTxCmdSupported = TRUE;
			socipc_msg("\nUserPrefUserRedOff = %d.\n",UserPrefUserRedOff);
			break;
		case SICMD_RW_CT_GreenOffset_Ctrl:
			//Send Athena RIGHT CT User Mode setting to Athena LEFT/FE
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(1);
			gSoCIPCTransPacket.MessageBuf[0]= (BYTE)UserPrefUserGreenOff;		
			IsTxCmdSupported = TRUE;
			socipc_msg("\nUserPrefUserGreenOff = %d.\n",UserPrefUserGreenOff);
			break;
		case SICMD_RW_CT_BlueOffset_Ctrl:
			//Send Athena RIGHT CT User Mode setting to Athena LEFT/FE
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(1);
			gSoCIPCTransPacket.MessageBuf[0]= (BYTE)UserPrefUserBlueOff;		
			IsTxCmdSupported = TRUE;
			socipc_msg("\nUserPrefUserBlueOff = %d.\n",UserPrefUserBlueOff);
			break;
	/*****************************************/				
		case SICMD_RW_Cables_Status:
			//Update cable status from Athena RIGHT to LEFT / FE
			{
			//Get Athena RIGHT Cable status first
   			CableInfo CableUpdate;
			BYTE cStatusByte = 0;			
   			Get_CableStatus(&CableUpdate);
			cStatusByte = (BYTE)(((CableUpdate.VGA_SourcePowerStatus&0x01)<<7) | //Athena RIGHT Cable Status
						  ((CableUpdate.VGA_CableStatus&0x01)<<6) | 
						  ((CableUpdate.PHY3_SourcePowerStatus&0x01)<<5) | 	
						  ((CableUpdate.PHY3_CableStatus&0x01)<<4) | 	
				          ((CableUpdate.PHY2_SourcePowerStatus&0x01)<<3) | 
				          ((CableUpdate.PHY2_CableStatus&0x01)<<2) |
				          ((CableUpdate.PHY1_SourcePowerStatus&0x01)<<1) | 
				          (CableUpdate.PHY1_CableStatus&0x01));			

			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(2);			
			gSoCIPCTransPacket.MessageBuf[0]= 0x00; //High Byte[15:8] Reserved
			gSoCIPCTransPacket.MessageBuf[1]= cStatusByte; //Low Byte[7:0] Cables status bits
			Athena_RIGHT_CurrentCableStatus = cStatusByte;			
			IsTxCmdSupported = TRUE;
			}
			break;
		case SICMD_RW_Mode_Info_CONTRL:
			//Send Athena RIGHT Mode Infomation Status to Athena LEFT/FE
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(2);
			//Get Athena RIGHT Mode Info Flags.
			gSoCIPCTransPacket.MessageBuf[0]= ((stInput[CH_A].ModeFlags >> 8)&0x00FF); //High Byte
			gSoCIPCTransPacket.MessageBuf[1]= (stInput[CH_A].ModeFlags)&0x00FF; //Low Byte			
			IsTxCmdSupported = TRUE;			
			break;
		case SICMD_RW_Mode_Info_STATUS:
			//Get Athena LEFT/FE Mode Infomation Status
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(0);		
			IsTxCmdSupported = TRUE;		
			break;
		case SICMD_RW_Timing_H_Width:
			//Send Athena RIGHT H Active to Athena LEFT/FE
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(2);
			//Get Athena RIGHT Timing H_Active
			gSoCIPCTransPacket.MessageBuf[0]= ((stInput[CH_A].Hactive >> 8)&0x00FF); //High Byte
			gSoCIPCTransPacket.MessageBuf[1]= (stInput[CH_A].Hactive)&0x00FF; //Low Byte			
			IsTxCmdSupported = TRUE;				
			break;
		case SICMD_RW_Timing_V_Length:
			//Send Athena RIGHT V Length to Athena LEFT/FE
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(2);
			//Get Athena RIGHT Timing V_Active
			gSoCIPCTransPacket.MessageBuf[0]= ((stInput[CH_A].Vactive >> 8)&0x00FF); //High Byte
			gSoCIPCTransPacket.MessageBuf[1]= (stInput[CH_A].Vactive)&0x00FF; //Low Byte				
			IsTxCmdSupported = TRUE;				
			break;
		case SICMD_RW_Timing_H_Total:
			//Send Athena RIGHT H Total to Athena LEFT/FE
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(2);
			//Get Athena RIGHT Timing H_Total
			gSoCIPCTransPacket.MessageBuf[0]= ((stInput[CH_A].Htotal >> 8)&0x00FF); //High Byte
			gSoCIPCTransPacket.MessageBuf[1]= (stInput[CH_A].Htotal)&0x00FF; //Low Byte				
			IsTxCmdSupported = TRUE;		
			break;
		case SICMD_RW_Timing_V_Total:
			//Send Athena RIGHT V Total to Athena LEFT/FE
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(2);
			//Get Athena RIGHT Timing V_Total
			gSoCIPCTransPacket.MessageBuf[0]= ((stInput[CH_A].Vtotal >> 8)&0x00FF); //High Byte
			gSoCIPCTransPacket.MessageBuf[1]= (stInput[CH_A].Vtotal)&0x00FF; //Low Byte				
			IsTxCmdSupported = TRUE;			
			break;
		case SICMD_RW_Timing_H_Start:
			//Send Athena RIGHT H Start to Athena LEFT/FE
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(2);
			//Get Athena RIGHT Timing H_Start
			gSoCIPCTransPacket.MessageBuf[0]= ((stInput[CH_A].Hstart >> 8)&0x00FF); //High Byte
			gSoCIPCTransPacket.MessageBuf[1]= (stInput[CH_A].Hstart)&0x00FF; //Low Byte				
			IsTxCmdSupported = TRUE;			
			break;
		case SICMD_RW_Timing_V_Start:
			//Send Athena RIGHT V Start to Athena LEFT/FE
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(2);
			//Get Athena RIGHT Timing V_Start
			gSoCIPCTransPacket.MessageBuf[0]= ((stInput[CH_A].Vstart >> 8)&0x00FF); //High Byte
			gSoCIPCTransPacket.MessageBuf[1]= (stInput[CH_A].Vstart)&0x00FF; //Low Byte				
			IsTxCmdSupported = TRUE;		
			break;
		case SICMD_RO_Timing_H_Width_Slave:
			//Get Athena LEFT/FE H Active
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(0);				
			IsTxCmdSupported = TRUE;		
			break;
		case SICMD_RO_Timing_V_Length_Slave:
			//Get Athena LEFT/FE V Active
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(0);				
			IsTxCmdSupported = TRUE;		
			break;
		case SICMD_RO_Timing_H_Total_Slave:
			//Get Athena LEFT/FE H Total
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(0);				
			IsTxCmdSupported = TRUE;		
			break;
		case SICMD_RO_Timing_V_Total_Slave:	
			//Get Athena LEFT/FE V Total
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(0);			
			IsTxCmdSupported = TRUE;			
			break;
		case SICMD_RO_Timing_H_Start_Slave:
			//Get Athena LEFT/FE H Start
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(0);				
			IsTxCmdSupported = TRUE;			
			break;
		case SICMD_RO_Timing_V_Start_Slave:
			//Get Athena LEFT/FE V Start
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(0);			
			IsTxCmdSupported = TRUE;				
			break;
		case SICMD_RO_Timing_V_Freq_Slave:
			//Get Athena LEFT/FE VFreq
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(0);			
			IsTxCmdSupported = TRUE;				
			break;
		case SICMD_RW_Port_CONTRL:
			//Send Athena RIGHT Port Status to Athena LEFT/FE
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(1);
			gSoCIPCTransPacket.MessageBuf[0]= UserPrefCurrentInputMain; //[7:0] Port Control bits			
			IsTxCmdSupported = TRUE;	
			break;
		case SICMD_RO_Port_Status:
			//Get Athena LEFT/FE Port Status to Athena RIGHT
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(0);			
			IsTxCmdSupported = TRUE;				
			break;
		case SICMD_RW_Signal_Sync_CONTRL:
			//TODO: Send Signal Sync infomation to Athena LEFT/FE
			break;
		case SICMD_RO_Signal_Sync_Status:
			//TODO: Get Signal Sync infomation from Athena LEFT/FE			
			break;
		case SICMD_RW_Brightness_CONTRL:
			//Send Athena RIGHT Userperf Brightness setting to Athena LEFT/FE
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(1);
			gSoCIPCTransPacket.MessageBuf[0]= (BYTE)UserPrefBrightness;		
			IsTxCmdSupported = TRUE;
			break;
		case SICMD_RO_Brightness_Status:
			//Get Athena LEFT/FE Brightness
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(0);			
			IsTxCmdSupported = TRUE;				
			break;
		case SICMD_RW_Contrast_CONTRL:
			//Send Athena RIGHT Contrast setting to Athena LEFT/FE
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(2);
			//Get Athena RIGHT Contrast Value
			gSoCIPCTransPacket.MessageBuf[0]= ((UserPrefContrast >> 8)&0x00FF); //High Byte
			gSoCIPCTransPacket.MessageBuf[1]= (UserPrefContrast)&0x00FF; //Low Byte			
			IsTxCmdSupported = TRUE;				
			break;
		case SICMD_RO_Contrast_Status:
			//Get Athena LEFT/FE Contrast Setting
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(0);			
			IsTxCmdSupported = TRUE;		
			break;
		case SICMD_RW_Sharpness_CONTRL:
			//Send Athena RIGHT Userperf Sharpness setting to Athena LEFT/FE
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(1);	
			gSoCIPCTransPacket.MessageBuf[0]= SocIPCGetCorrectSharpnessSetting();		
			IsTxCmdSupported = TRUE;			
			break;
		case SICMD_RO_Sharpness_Status:
			//Get Athena LEFT/FE Sharpness Setting
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(0);				
			IsTxCmdSupported = TRUE;		
			break;
		case SICMD_RW_Color_CONTRL:	
			//Send Athena RIGHT Color related settings to Athena LEFT/FE (Base on current OSD Layout)
			//0:3 	Color temp control bits 			000: 6500K; 001: 7500K; 010: 9300K; 011: User; 100/101/110/111: Reserved
			//4:7 	Theme mode control bits 		000: Normal; 001: Vivid; 010: Cinema; 011: Game; 100:Sport ;101/110/111: Reserved
			//8:11	Color space control bits 			000: sRGB; 001: Adobe98; 010: xvYCC; 011:DICOM;  100/101/110/111: Reserved
			//12:15  DUM Switch control bits			000: Off; 001: On; 010~111: Reserved	
			{
				WORD ColorSettings = 0;
				BYTE cColorTempNewIndex = 0;
				
				if(UserPrefColorTemp == CTEMP_6500K)
					cColorTempNewIndex = 0;
				else if(UserPrefColorTemp == CTEMP_7500K)
					cColorTempNewIndex = 1;
				else if(UserPrefColorTemp == CTEMP_9300K)
					cColorTempNewIndex = 2;
				else if(UserPrefColorTemp == USER_TEMP)
					cColorTempNewIndex = 3;
				else
					cColorTempNewIndex = 0x0F;	//Mismatch case			
				ColorSettings = (((UserPrefDUMSwitch&0x0F)<<12)|((UserPrefColorSpace&0x0F)<<8)|((UserPrefImageScheme&0x0F)<<4)|(cColorTempNewIndex&0x0F));

				gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(2);	
				gSoCIPCTransPacket.MessageBuf[0]= ((ColorSettings >> 8)&0x00FF); //High Byte
				gSoCIPCTransPacket.MessageBuf[1]= (ColorSettings)&0x00FF; //Low Byte					
				IsTxCmdSupported = TRUE;
			}
			break;
		case SICMD_RO_Color_Status:
			//Get Athena LEFT/FE Color Settings
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(0);			
			IsTxCmdSupported = TRUE;			
			break;
		case SICMD_RW_Video_progress_CONTRL:
			//Send Video Progress Settings to Athena LEFT/FE (Base on current OSD layout)
			//Video_progress_CONTRL
			//0:3 	DCDI control bits	000: off; 001: On; 010/011/100/101/110/111: Reserved
			//4:7 	MADI control bits	000: off; 001: Adaptive; 010: Normal; 011/100/101/110/111: Reserved
			//8:11	OD control bits 	000: off; 001: Medium; 010: Low; 011: High; 100/101/110/111: Reserved
			//12:15  Reserved
			{
				WORD VideoProgress = 0;
			
				VideoProgress = (((UserPrefLCDOverdrive&0x0F)<<8)|((UserPrefMADI&0x0F)<<4)|(UserPrefDCDiMain&0x0F));

				gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(2);
				gSoCIPCTransPacket.MessageBuf[0]= ((VideoProgress >> 8)&0x00FF); //High Byte
				gSoCIPCTransPacket.MessageBuf[1]= (VideoProgress)&0x00FF; //Low Byte					
				IsTxCmdSupported = TRUE;				
			}
			break;
		case SICMD_RO_Video_progress_Status:
			//Get Athena LEFT/FE Video Progress Settings
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(0);				
			IsTxCmdSupported = TRUE;			
			break;
		case SICMD_RW_NR_CONTRL:
			//Send NR Settings to Athena LEFT/FE (Base on current OSD layout)			
			//NR_CONTRL
			//0:3 	Dynamic NR control bits 	000: off; 001: Adaptive; 010: Low; 011: Medium; 100: High; 101/110/111: Reserved
			//4:7 	MPEG NR control bits		000: off; 001: On; 010/011/100/101/110/111: Reserved
			{
				WORD NRStatus = 0;			
			
				NRStatus = (((UserPrefMPEGNRMode&0x0F)<<4)|(UserPrefTNR&0x0F));
				
				gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(2);	
				gSoCIPCTransPacket.MessageBuf[0]= ((NRStatus >> 8)&0x00FF); //High Byte
				gSoCIPCTransPacket.MessageBuf[1]= (NRStatus)&0x00FF; //Low Byte					
				IsTxCmdSupported = TRUE;
			}
			break;
		case SICMD_RO_NR_Status:
			//Get Athena LEFT/FE NR Settings
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(0);				
			IsTxCmdSupported = TRUE;			
			break;
		case SICMD_RW_Video_progress1_CONTRL:
			//Send Video Progress_1 Settings to Athena LEFT/FE (Base on current OSD layout)
			//0:3	Film mode detect	000: off; 001: Video-3:2-2:2; 010: Video-3:2; 011: Video-2:2; 100/101/110/111: Reserved 
			//4:7	Reserved			
			{
				WORD VideoProgress_1 = 0;			
				
				VideoProgress_1 = (UserPrefFilmMode&0x0F);
				
				gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(2);
				gSoCIPCTransPacket.MessageBuf[0]= ((VideoProgress_1 >> 8)&0x00FF); //High Byte
				gSoCIPCTransPacket.MessageBuf[1]= (VideoProgress_1)&0x00FF; //Low Byte					
				IsTxCmdSupported = TRUE;
			}
			break;
		case SICMD_RO_Video_progress1_Status:
			//Get Athena LEFT/FE Video Progress_1 Settings
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(0);			
			IsTxCmdSupported = TRUE;			
			break;
		case SICMD_RW_DisplaySetup_CONTRL:
			//Send Display Setup Settings to Athena LEFT/FE (Base on current OSD layout)
			{
				WORD DisplaySetup = 0;
				
				DisplaySetup = (UserPrefOutputAspectRatio&0x0F);

				gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(3);	
				gSoCIPCTransPacket.MessageBuf[0]= ((DisplaySetup >> 8)&0x00FF); //High Byte
				gSoCIPCTransPacket.MessageBuf[1]= (DisplaySetup)&0x00FF; //Low Byte	
				gSoCIPCTransPacket.MessageBuf[2]= MsgParaBuf[0]; //UpdateFlag
				IsTxCmdSupported = TRUE;
			}
			break;
		case SICMD_RO_DisplaySetup_Status:
			//Get Athena LEFT/FE Display Setup Settings
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(0);			
			IsTxCmdSupported = TRUE;			
			break;
		case SICMD_RW_Audio1_CONTRL:
			//Send Audio1 Settings to Athena LEFT/FE (Base on current OSD layout)
			//0:3	Main ouput	000: Line out; 010:SPDIF out; Others: Reserved
			//4:7	Mute 		000: Off; 001: On; Others: Reserved
			//8:11	DownMix 		000: Off; 001: On; Others: Reserved		
			{
				WORD Audio_1 = 0;
							
				Audio_1 = (((UserPrefAudioDownMix&0x0F)<<8)|((UserPrefAudioMute&0x0F)<<4)|(UserPrefAudioMainOutput&0x0F));

				gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(2);
				gSoCIPCTransPacket.MessageBuf[0]= ((Audio_1 >> 8)&0x00FF); //High Byte
				gSoCIPCTransPacket.MessageBuf[1]= (Audio_1)&0x00FF; //Low Byte					
				IsTxCmdSupported = TRUE;
			}
			break;
		case SICMD_RO_Audio1_Status:
			//Get Athena LEFT/FE Audio1 Settings
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(0);				
			IsTxCmdSupported = TRUE;				
			break;
		case SICMD_RW_Audio2_CONTRL:
			//Send Audio2 Settings to Athena LEFT/FE (Base on current OSD layout)
			//0:5	Volume control	0-32
			//6:15	Audio delay 		0-999
			{
				WORD Audio_2 = 0;

				Audio_2 = ((UserPrefAVSDelayTime<<6)|UserPrefAudioVolume);
				
				gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(2);	
				gSoCIPCTransPacket.MessageBuf[0]= ((Audio_2 >> 8)&0x00FF); //High Byte
				gSoCIPCTransPacket.MessageBuf[1]= (Audio_2)&0x00FF; //Low Byte					
				IsTxCmdSupported = TRUE;
			}
			break;
		case SICMD_RO_Audio2_Status:
			//Get Athena LEFT/FE Audio2 Settings
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(0);				
			IsTxCmdSupported = TRUE;		
			break;
		case SICMD_RW_Setup_CONTRL:
			//Send Setup Settings to Athena LEFT/FE (Base on current OSD layout)	
			//0:3	DCR control 	00: No; 01:Yes; 	10 / 11: Reserved
			//4:7	Factory Reset	00: No, 01:Yes; 	10 / 11: Reserved
			//8:15 Reserved				
			{
				WORD Setup = 0;	
				
				Setup = (((UserPrefScrollMenuItem&0x0F)<<4)|(UserPrefDCR&0x0F));
				gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(2);	
				gSoCIPCTransPacket.MessageBuf[0]= ((Setup >> 8)&0x00FF); //High Byte
				gSoCIPCTransPacket.MessageBuf[1]= (Setup)&0x00FF; //Low Byte					
				IsTxCmdSupported = TRUE;
			}
			break;
		case SICMD_RO_Setup_Status:
			//Get Athena LEFT/FE Setup Settings
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(0);			
			IsTxCmdSupported = TRUE;				
			break;
		case SICMD_RW_PBP_CONTRL:
			//Send Athena RIGHT Main capture Switch setting to Athena LEFT
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(1);	
			gSoCIPCTransPacket.MessageBuf[0]= (BYTE)UserPrefCaptureM;
			gm_Print("Main Capture mode is %d", UserPrefCaptureM);
			IsTxCmdSupported = TRUE;
			break;
		case SICMD_RO_DCR_Result:
			//Get Athena LEFT DCR result
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(0);
			IsTxCmdSupported = TRUE;
			break;
		case SICMD_RW_OSD_Sync_CONTRL:
			//Command code from Athena RIGHT; 00: Enable OSD Sync; 01: Disable OSD Sync; 10/11: Reserved
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(1);	
			gSoCIPCTransPacket.MessageBuf[0]= (MsgParaBuf[0]&0x03); // [1:0] OSD Sync Control bits			
			IsTxCmdSupported = TRUE;		
			break;
		case SICMD_RO_OSD_Sync_Status:
			//Get Athena LEFT/FE OSD Sync Status and check OSD Sync is finish or not.
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(0);				
			IsTxCmdSupported = TRUE;		
			break;
		case SICMD_RO_OSD_Sync_changes:
			//Get Athena LEFT/FE OSD Sync Changes Status
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(0);			
			IsTxCmdSupported = TRUE;				
			break;
		case SICMD_RW_OSD_DP_Switch:
			//Send Athena RIGHT DP Switch setting to Athena LEFT/FE
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(1);	
			//socipc_msg("UserPrefDPSWitch = ..............%d", UserPrefDPSWitch);
			gSoCIPCTransPacket.MessageBuf[0]= (BYTE)UserPrefDPSWitch;
			IsTxCmdSupported = TRUE;
			break;
		case SICMD_RW_DDCI_CONTRL:
			//Send Athena RIGHT DDC/CI OPCODE and Value to Athena LEFT/FE
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(3);	
			gSoCIPCTransPacket.MessageBuf[0]= MsgParaBuf[0]; //DDC/CI VCP OP Code
			gSoCIPCTransPacket.MessageBuf[1]= MsgParaBuf[1]; //DDC/CI Value High Byte
			gSoCIPCTransPacket.MessageBuf[2]= MsgParaBuf[2]; //DDC/CI Value Low Byte
			IsTxCmdSupported = TRUE;
			break;
		case SICMD_RO_DDCI_Status:
			//Base on DDC/CI commands to get Athena LEFT/FE current values to Athena RIGHT DDC/CI handler
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(1);	
			gSoCIPCTransPacket.MessageBuf[0]= MsgParaBuf[0]; //DDC/CI VCP OP Code
			IsTxCmdSupported = TRUE;
			break;

		/*ChengLin 20130110: 6-axis Saturation, Hue, Brightness controls*/
		case SICMD_RW_6Axis_Sat_Red_Ctrl:
			//Send Athena RIGHT 6-Axis setting to Athena LEFT/FE
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(1);
			gSoCIPCTransPacket.MessageBuf[0]= (BYTE)UserPrefAcm3RedSat;		
			IsTxCmdSupported = TRUE;
			socipc_msg("\nUserPrefAcm3RedSat = %d.\n",UserPrefAcm3RedSat);
			break;
		case SICMD_RW_6Axis_Sat_Green_Ctrl:
			//Send Athena RIGHT 6-Axis setting to Athena LEFT/FE
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(1);
			gSoCIPCTransPacket.MessageBuf[0]= (BYTE)UserPrefAcm3GreenSat;		
			IsTxCmdSupported = TRUE;
			socipc_msg("\nUserPrefAcm3GreenSat = %d.\n",UserPrefAcm3GreenSat);		
			break;
		case SICMD_RW_6Axis_Sat_Blue_Ctrl:
			//Send Athena RIGHT 6-Axis setting to Athena LEFT/FE
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(1);
			gSoCIPCTransPacket.MessageBuf[0]= (BYTE)UserPrefAcm3BlueSat;		
			IsTxCmdSupported = TRUE;
			socipc_msg("\nUserPrefAcm3BlueSat = %d.\n",UserPrefAcm3BlueSat);		
			break;
		case SICMD_RW_6Axis_Sat_Yellow_Ctrl:
			//Send Athena RIGHT 6-Axis setting to Athena LEFT/FE
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(1);
			gSoCIPCTransPacket.MessageBuf[0]= (BYTE)UserPrefAcm3YellowSat;		
			IsTxCmdSupported = TRUE;
			socipc_msg("\nUserPrefAcm3YellowSat = %d.\n",UserPrefAcm3YellowSat);		
			break;
		case SICMD_RW_6Axis_Sat_Magenta_Ctrl:
			//Send Athena RIGHT 6-Axis setting to Athena LEFT/FE
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(1);
			gSoCIPCTransPacket.MessageBuf[0]= (BYTE)UserPrefAcm3MagentaSat;		
			IsTxCmdSupported = TRUE;
			socipc_msg("\nUserPrefAcm3MagentaSat = %d.\n",UserPrefAcm3MagentaSat);			
			break;
		case SICMD_RW_6Axis_Sat_Cyan_Ctrl:
			//Send Athena RIGHT 6-Axis setting to Athena LEFT/FE
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(1);
			gSoCIPCTransPacket.MessageBuf[0]= (BYTE)UserPrefAcm3CyanSat;		
			IsTxCmdSupported = TRUE;
			socipc_msg("\nUserPrefAcm3CyanSat = %d.\n",UserPrefAcm3CyanSat);		
			break;
		case SICMD_RW_6Axis_Sat_Black_Ctrl:
			//Send Athena RIGHT 6-Axis setting to Athena LEFT/FE
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(1);
			gSoCIPCTransPacket.MessageBuf[0]= (BYTE)UserPrefAcm3BlackSat;		
			IsTxCmdSupported = TRUE;
			socipc_msg("\nUserPrefAcm3BlackSat = %d.\n",UserPrefAcm3BlackSat);			
			break;
		case SICMD_RW_6Axis_Sat_White_Ctrl:
			//Send Athena RIGHT 6-Axis setting to Athena LEFT/FE
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(1);
			gSoCIPCTransPacket.MessageBuf[0]= (BYTE)UserPrefAcm3WhiteSat;		
			IsTxCmdSupported = TRUE;
			socipc_msg("\nUserPrefAcm3WhiteSat = %d.\n",UserPrefAcm3WhiteSat);		
			break;
		case SICMD_RW_6Axis_Hue_Red_Ctrl:
			//Send Athena RIGHT 6-Axis setting to Athena LEFT/FE
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(1);
			gSoCIPCTransPacket.MessageBuf[0]= (BYTE)UserPrefAcm3RedHue;		
			IsTxCmdSupported = TRUE;
			socipc_msg("\nUserPrefAcm3RedHue = %d.\n",UserPrefAcm3RedHue);		
			break;
		case SICMD_RW_6Axis_Hue_Green_Ctrl:
			//Send Athena RIGHT 6-Axis setting to Athena LEFT/FE
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(1);
			gSoCIPCTransPacket.MessageBuf[0]= (BYTE)UserPrefAcm3GreenHue;		
			IsTxCmdSupported = TRUE;
			socipc_msg("\nUserPrefAcm3GreenHue = %d.\n",UserPrefAcm3GreenHue);		
			break;
		case SICMD_RW_6Axis_Hue_Blue_Ctrl:
			//Send Athena RIGHT 6-Axis setting to Athena LEFT/FE
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(1);
			gSoCIPCTransPacket.MessageBuf[0]= (BYTE)UserPrefAcm3BlueHue;		
			IsTxCmdSupported = TRUE;
			socipc_msg("\nUserPrefAcm3BlueHue = %d.\n",UserPrefAcm3BlueHue);		
			break;
		case SICMD_RW_6Axis_Hue_Yellow_Ctrl:
			//Send Athena RIGHT 6-Axis setting to Athena LEFT/FE
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(1);
			gSoCIPCTransPacket.MessageBuf[0]= (BYTE)UserPrefAcm3YellowHue;		
			IsTxCmdSupported = TRUE;
			socipc_msg("\nUserPrefAcm3YellowHue = %d.\n",UserPrefAcm3YellowHue);			
			break;
		case SICMD_RW_6Axis_Hue_Magenta_Ctrl:
			//Send Athena RIGHT 6-Axis setting to Athena LEFT/FE
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(1);
			gSoCIPCTransPacket.MessageBuf[0]= (BYTE)UserPrefAcm3MagentaHue;		
			IsTxCmdSupported = TRUE;
			socipc_msg("\nUserPrefAcm3MagentaHue = %d.\n",UserPrefAcm3MagentaHue);			
			break;
		case SICMD_RW_6Axis_Hue_Cyan_Ctrl:
			//Send Athena RIGHT 6-Axis setting to Athena LEFT/FE
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(1);
			gSoCIPCTransPacket.MessageBuf[0]= (BYTE)UserPrefAcm3CyanHue;		
			IsTxCmdSupported = TRUE;
			socipc_msg("\nUserPrefAcm3CyanHue = %d.\n",UserPrefAcm3CyanHue);		
			break;
		case SICMD_RW_6Axis_Hue_Black_Ctrl:
			//Send Athena RIGHT 6-Axis setting to Athena LEFT/FE
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(1);
			gSoCIPCTransPacket.MessageBuf[0]= (BYTE)UserPrefAcm3BlackHue;		
			IsTxCmdSupported = TRUE;
			socipc_msg("\nUserPrefAcm3BlackHue = %d.\n",UserPrefAcm3BlackHue);			
			break;
		case SICMD_RW_6Axis_Hue_White_Ctrl:
			//Send Athena RIGHT 6-Axis setting to Athena LEFT/FE
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(1);
			gSoCIPCTransPacket.MessageBuf[0]= (BYTE)UserPrefAcm3WhiteHue;		
			IsTxCmdSupported = TRUE;
			socipc_msg("\nUserPrefAcm3WhiteHue = %d.\n",UserPrefAcm3WhiteHue);		
			break;
		case SICMD_RW_6Axis_Bright_Red_Ctrl:
			//Send Athena RIGHT 6-Axis setting to Athena LEFT/FE
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(1);
			gSoCIPCTransPacket.MessageBuf[0]= (BYTE)UserPrefAcm3RedLuma;		
			IsTxCmdSupported = TRUE;
			socipc_msg("\nUserPrefAcm3RedLuma = %d.\n",UserPrefAcm3RedLuma);		
			break;
		case SICMD_RW_6Axis_Bright_Green_Ctrl:
			//Send Athena RIGHT 6-Axis setting to Athena LEFT/FE
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(1);
			gSoCIPCTransPacket.MessageBuf[0]= (BYTE)UserPrefAcm3GreenLuma;		
			IsTxCmdSupported = TRUE;
			socipc_msg("\nUserPrefAcm3GreenLuma = %d.\n",UserPrefAcm3GreenLuma);			
			break;
		case SICMD_RW_6Axis_Bright_Blue_Ctrl:
			//Send Athena RIGHT 6-Axis setting to Athena LEFT/FE
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(1);
			gSoCIPCTransPacket.MessageBuf[0]= (BYTE)UserPrefAcm3BlueLuma;		
			IsTxCmdSupported = TRUE;
			socipc_msg("\nUserPrefAcm3BlueLuma = %d.\n",UserPrefAcm3BlueLuma);		
			break;
		case SICMD_RW_6Axis_Bright_Yellow_Ctrl:
			//Send Athena RIGHT 6-Axis setting to Athena LEFT/FE
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(1);
			gSoCIPCTransPacket.MessageBuf[0]= (BYTE)UserPrefAcm3YellowLuma;		
			IsTxCmdSupported = TRUE;
			socipc_msg("\nUserPrefAcm3YellowLuma = %d.\n",UserPrefAcm3YellowLuma);		
			break;
		case SICMD_RW_6Axis_Bright_Magenta_Ctrl:
			//Send Athena RIGHT 6-Axis setting to Athena LEFT/FE
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(1);
			gSoCIPCTransPacket.MessageBuf[0]= (BYTE)UserPrefAcm3MagentaLuma;		
			IsTxCmdSupported = TRUE;
			socipc_msg("\nUserPrefAcm3MagentaLuma = %d.\n",UserPrefAcm3MagentaLuma);		
			break;
		case SICMD_RW_6Axis_Bright_Cyan_Ctrl:
			//Send Athena RIGHT 6-Axis setting to Athena LEFT/FE
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(1);
			gSoCIPCTransPacket.MessageBuf[0]= (BYTE)UserPrefAcm3CyanLuma;		
			IsTxCmdSupported = TRUE;
			socipc_msg("\nUserPrefAcm3CyanLuma = %d.\n",UserPrefAcm3CyanLuma);			
			break;
		case SICMD_RW_6Axis_Bright_Black_Ctrl:
			//Send Athena RIGHT 6-Axis setting to Athena LEFT/FE
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(1);
			gSoCIPCTransPacket.MessageBuf[0]= (BYTE)UserPrefAcm3BlackLuma;		
			IsTxCmdSupported = TRUE;
			socipc_msg("\nUserPrefAcm3BlackLuma = %d.\n",UserPrefAcm3BlackLuma);		
			break;
		case SICMD_RW_6Axis_Bright_White_Ctrl:
			//Send Athena RIGHT 6-Axis setting to Athena LEFT/FE
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(1);
			gSoCIPCTransPacket.MessageBuf[0]= (BYTE)UserPrefAcm3WhiteLuma;		
			IsTxCmdSupported = TRUE;
			socipc_msg("\nUserPrefAcm3WhiteLuma = %d.\n",UserPrefAcm3WhiteLuma);			
			break;
		/*****************************************/
      
//inter-chip SMT communication
//refer to doc "4Kx2K System FW SMT Design Gamma Calibration and Uniformity Compensation"
      #ifdef USE_SMT
        case SICMD_WO_SMT_COMMAND:
            gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(SMTMsgLen);
            memcpy(&(gSoCIPCTransPacket.MessageBuf[0]), MsgParaBuf, SMTMsgLen);
            IsTxCmdSupported = TRUE;
            break;
      #endif
      
   #ifdef DP_HUB_2_LOG_PORTS
		case SICMD_RW_DpRx0_MstInfo: 
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(1);	
			gSoCIPCTransPacket.MessageBuf[0]= (BYTE)MsgParaBuf[0];
			IsTxCmdSupported = TRUE;
			break;
      case SICMD_RO_DpRx0_IsStable:
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(0);			
			IsTxCmdSupported = TRUE;			
			break;      
      case SICMD_RW_DPRx0_ReModeSetup:
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(1);	
			gSoCIPCTransPacket.MessageBuf[0]= (BYTE)TRUE;
			IsTxCmdSupported = TRUE;
         break;       
   #endif

		case SICMD_RW_Keypad_CONTRL:
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(2);	
			gSoCIPCTransPacket.MessageBuf[0]= MsgParaBuf[0]; // ADC Value
			gSoCIPCTransPacket.MessageBuf[1]= MsgParaBuf[1]; // ADC Channel			
			IsTxCmdSupported = TRUE;		
			break;
		case SICMD_RW_IR_CONTROL:
			//Send IR Key to Athena LEFT/FE
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(1);	
			gSoCIPCTransPacket.MessageBuf[0]= MsgParaBuf[0]; // [7:0] IR value bits
			IsTxCmdSupported = TRUE;	
			break;
	/*ChengLin 20130128: New Sync Handler State Control*/	
		case SICMD_RW_IMAGE_SYNC_STATE_Ctrl:
			//Send Athena RIGHT assigned sync state to Athena LEFT/FE
			/*   IMAGE_LVDS_ON_STATE = 0,
  				IMAGE_OFF_STATE,
   				IMAGE_RDY_WAIT_STATE,
   				IMAGE_ON_STATE,
   				IMAGE_LOST_WAIT_STATE,
   				IMAGE_FAIL_STATE
   			*/
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(1);
			gSoCIPCTransPacket.MessageBuf[0]= (BYTE)MsgParaBuf[0];		
			IsTxCmdSupported = TRUE;
			break;
		case SICMD_RW_IMAGE_SYNC_STATE_Status:
			//Get Athena LEFT/FE Sync State
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(0);			
			IsTxCmdSupported = TRUE;	
			break;
	/*********************************************/
/*ChengLin 20130128: New Sync Handler State Control*/	
#if ENABLE_ATHENA_R_L_IMAGE_SYNC_HANDLER
		case SICMD_RW_IMAGE_SYNC_BLACK_SCREEN_CTRL:
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(1);
			gSoCIPCTransPacket.MessageBuf[0]= (BYTE)MsgParaBuf[0];		
			IsTxCmdSupported = TRUE;
			break;
#endif	
		case SICMD_RW_GAMMA_CORRECTION_CONTRL: //Add by ChengLin 20130226
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(0);	
			IsTxCmdSupported = TRUE;
			break;
#if ENABLE_VEGA_ISP_CMD_VIA_UART_ISP_PORT         
/*ChengLin 20130411: Enable Athena FE or Left Debug Mode from Athena Right*/
      case SICMD_RW_ATHENA_FE_L_DEBUG_MODE_EN_CTRL:
         gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(0);
			gSoCIPCTransPacket.MessageBuf[0]= (BYTE)TRUE;   
         IsTxCmdSupported = TRUE;
         break;
#endif         
#ifdef SMART_ISP
      case SICMD_RW_SMARTISP_CTRL:
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(1);
			gSoCIPCTransPacket.MessageBuf[0]= (BYTE)MsgParaBuf[0];		
			IsTxCmdSupported = TRUE;
			break;

      case SICMD_RO_SMARTISP_STATUS:
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(0);			
			IsTxCmdSupported = TRUE;			
			break;
#endif
      /*ChengLin 20130322: Load Default Settings or Original Setting Before/After Splash Screen*/
      case SICMD_RW_LOAD_SPLASH_SETTING_CTRL: 
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(1);
			gSoCIPCTransPacket.MessageBuf[0]= (BYTE)MsgParaBuf[0];		
			IsTxCmdSupported = TRUE;
			break;

		case SICMD_RW_Slave_Registers_addr_CONTRL:
			//Send Register Address to Athena LEFT/FE
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(2);
			gSoCIPCTransPacket.MessageBuf[0]= MsgParaBuf[0]; //High Byte
			gSoCIPCTransPacket.MessageBuf[1]= MsgParaBuf[1]; //Low Byte			
			IsTxCmdSupported = TRUE;
			break;
		case SICMD_RW_Slave_Registers_data_CONTRL:
			//Send Register Data to Athena LEFT/FE (After assign a data, Athena LEFT/FE will execute this function
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(2);
			gSoCIPCTransPacket.MessageBuf[0]= MsgParaBuf[0]; //High Byte
			gSoCIPCTransPacket.MessageBuf[1]= MsgParaBuf[1]; //Low Byte			
			IsTxCmdSupported = TRUE;			
			break;
		case SICMD_RO_Slave_Registers_read_data:
			//Get Register Value from Athena LEFT/FE
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(0);			
			IsTxCmdSupported = TRUE;		
			break;
		case SICMD_RW_SaveModeIndepedent_Ctrl:
			gSoCIPCTransPacket.MessageLength = SOCIPC_TRANS_MSG_LEN(0); 		
			IsTxCmdSupported = TRUE;	
		break;
		default:
			break;		
	}

	if(IsTxCmdSupported)
	{
		gm_2WireSelectIdx (MASTER_2WIRE_2);	
		IsTransmitted = SocIPCSrcPacketTransmitter(Cmd);
		gm_2WireSelectIdx (MASTER_2WIRE_1);

      gm_TimerStart(SOCIPC_SENDCMD_WAIT_1ms_TMR, 10);
		return IsTransmitted;
	}
	else
	{
	   gm_TimerStart(SOCIPC_SENDCMD_WAIT_1ms_TMR, 10);
		return FALSE;
	}

}

//******************************************************************************
// FUNCTION:   SocIPCTxReceivePacketCheck
// USAGE:  Read Soc IPC Responses from slave SoCs via I2C Master
// INPUT:  DeviceID, Cmd
// OUTPUT:	TRUE(I2C RX Successful), FALSE(I2C RX Fail)
// GLOBALS:
//******************************************************************************
BOOL SocIPCTxReceivePacketCheck( BYTE DeviceID, gmt_SocIPCCommandCode Cmd )
{
#if (!ENABLE_GET_I2C_READ_BYTE_VIA_SOCIPC_RECV_PACKET)
	BYTE RecvPacketLen = 0;
#endif //(!ENABLE_GET_I2C_READ_BYTE_VIA_SOCIPC_RECV_PACKET)
	BYTE CmdRxBuff[SOC_IPC_TRX_BUF_SIZE];
#if ENABLE_SOC_IPC_DEBUG	
	BYTE Count = 0;
#endif
	BYTE ReTryCount = 0;
	BYTE RecvDataType = RECV_NACK;

	if( (DeviceID != SOCIPC_DEVICEL_DST_ADDRESS) && (DeviceID != SOCIPC_DEVICEFE_DST_ADDRESS))
	{		
		socipc_msg("\n<Soc IPC Error> Invalid Device ID.\n",0);
		return FALSE;
	}

#if (!ENABLE_GET_I2C_READ_BYTE_VIA_SOCIPC_RECV_PACKET)
	RecvPacketLen = SocIPCGetResponseDataLength( Cmd );
#endif //(!ENABLE_GET_I2C_READ_BYTE_VIA_SOCIPC_RECV_PACKET)
	RecvDataType = SocIPCGetResponseDataType( Cmd );

RX_RETRY:
	
	if( RecvDataType != RECV_NOTHING )
	{
#if ENABLE_GET_I2C_READ_BYTE_VIA_SOCIPC_RECV_PACKET		
		if( SocIPCAutoLengthRead2WireBlock(DeviceID, (BYTE *)CmdRxBuff,TRUE) == gmd_PASS )
#else //Original
		if( gm_Read2WireBlock(0, DeviceID, (BYTE *)CmdRxBuff, RecvPacketLen, TRUE) == gmd_PASS )
#endif //ENABLE_GET_I2C_READ_BYTE_VIA_SOCIPC_RECV_PACKET		
		{
			if( SocIPCTxRecvBufCheckCS(CmdRxBuff,CmdRxBuff[1]&0x7f)) //Packet Checksum is valid
			{
#if ENABLE_SOC_IPC_DEBUG
				for(Count=0; Count < ((CmdRxBuff[1]&0x7f)+3); Count++)
					socipc_msg_ex("\nCmdRxBuff[%d] = 0x%X.\n",Count, CmdRxBuff[Count]);
#endif

				if( CmdRxBuff[1] == 0x80 && CmdRxBuff[2] == 0xBE) //Null Message
				{				
					socipc_msg("\n<Soc IPC Message> Get Null Message.\n",0);
					if( ReTryCount <= SOC_IPC_RECV_RETRY_TIME)
					{
						ReTryCount++;
						goto RX_RETRY;
					}
					else
					{					
						socipc_msg("\n<Soc IPC Error> Retry over %d times.\n",SOC_IPC_RECV_RETRY_TIME);
						ReTryCount = 0;
						goto RX_END;
					}
				}
				else //Normal Responses
				{
					if(RecvDataType == RECV_ACK || RecvDataType == RECV_NACK)
					{
						if( CmdRxBuff[5] == 0x03 && CmdRxBuff[6] == 0x0C)
						{					
							socipc_msg("\n<Soc IPC Message> Command ID[0x%X]-> Successful.\n",Cmd);
							return TRUE;
						}
						else
						{
							socipc_msg("\n<Soc IPC Message> Command ID[0x%X]-> Failure.\n",Cmd);
							return FALSE;
						}
					}
					else if(RecvDataType == RECV_DATA )
					{					
						socipc_msg("\n<Soc IPC Message> Command ID[0x%X]-> Get Data Infomation.\n",Cmd);
						SoCIPC_UpdateRecvDataFromDevice_LEFT_FE(&CmdRxBuff[Start_of_SoC_IPC_Cmd_in_DDC2BiMsg]);
						return TRUE;
					}
					else
					{					
						socipc_msg("\n<Soc IPC Error> Command ID[0x%X]-> Not Assign RecvDataType.\n",Cmd);
						return FALSE;
					}
				}
			}
			else //invalid checksum
			{
				socipc_msg("\n<Soc IPC Error> Packet Invalid Checksum.\n",0);
				return FALSE;
			}
		
		}
		else
		{
			socipc_msg("\n<Soc IPC Error> IIC Communication Failure.\n",0);
			return FALSE;
		}

	}
	else
	{
		socipc_msg("\n<Soc IPC Error> Invalid Receive Packet Type.\n",0);
		return FALSE;
	}

RX_END:
	return FALSE;

}

//******************************************************************************
// FUNCTION:   SoCIPC_UpdateRecvDataFromDevice_LEFT_FE
// USAGE:  Update all information from Athena LEFT and FE
// INPUT:  gParabufferPtr
// OUTPUT:	None
// GLOBALS:
//******************************************************************************
void SoCIPC_UpdateRecvDataFromDevice_LEFT_FE(BYTE far* gParabufferPtr)
{
	switch(*(gParabufferPtr+1)) //Commands
	{
		case SICMD_RO_Power_Status:
			gAthena_LEFT_FE_Slave_Parameters.PowerStatus = (((PowerStatusResPkt_t fdata*)gParabufferPtr)->Power_Status);			
			socipc_msg("\ngAthena_LEFT_FE_PowerStatus = 0x%X.\n",gAthena_LEFT_FE_Slave_Parameters.PowerStatus);
			break;
		case SICMD_RW_Mode_Info_STATUS:
			gAthena_LEFT_FE_Slave_Parameters.ModeFlags = (WORD)(((((ModeInfoStatusResPkt_t fdata*)gParabufferPtr)->ModeInfo_HighByte)<<8)|
									(((ModeInfoStatusResPkt_t fdata*)gParabufferPtr)->ModeInfo_LowByte));			
			socipc_msg("\ngAthena_LEFT_FE_ModeFlags = 0x%X.\n",gAthena_LEFT_FE_Slave_Parameters.ModeFlags);
			break;
		case SICMD_RO_Timing_H_Width_Slave:
			gAthena_LEFT_FE_Slave_Parameters.HWidth = (WORD)(((((TimingHWidthSlaveResPkt_t fdata*)gParabufferPtr)->HWidth_HighByte)<<8)|
									(((TimingHWidthSlaveResPkt_t fdata*)gParabufferPtr)->HWidth_LowByte));			
			socipc_msg("\ngAthena_LEFT_FE_H_Width = %d.\n",gAthena_LEFT_FE_Slave_Parameters.HWidth);
			break;
		case SICMD_RO_Timing_V_Length_Slave:
			gAthena_LEFT_FE_Slave_Parameters.VLength = (WORD)(((((TimingVLengthSlaveResPkt_t fdata*)gParabufferPtr)->VLength_HighByte)<<8)|
									(((TimingVLengthSlaveResPkt_t fdata*)gParabufferPtr)->VLength_LowByte));			
			socipc_msg("\ngAthena_LEFT_FE_V_Length = %d.\n",gAthena_LEFT_FE_Slave_Parameters.VLength);
			break;
		case SICMD_RO_Timing_H_Total_Slave:
			gAthena_LEFT_FE_Slave_Parameters.HTotal = (WORD)(((((TimingHTotalSlaveResPkt_t fdata*)gParabufferPtr)->HTotal_HighByte)<<8)|
									(((TimingHTotalSlaveResPkt_t fdata*)gParabufferPtr)->HTotal_LowByte));			
			socipc_msg("\ngAthena_LEFT_FE_H_Total = %d.\n",gAthena_LEFT_FE_Slave_Parameters.HTotal);
			break;
		case SICMD_RO_Timing_V_Total_Slave:
			gAthena_LEFT_FE_Slave_Parameters.VTotal = (WORD)(((((TimingVTotalSlaveResPkt_t fdata*)gParabufferPtr)->VTotal_HighByte)<<8)|
									(((TimingVTotalSlaveResPkt_t fdata*)gParabufferPtr)->VTotal_LowByte));		
			socipc_msg("\ngAthena_LEFT_FE_V_Total = %d.\n",gAthena_LEFT_FE_Slave_Parameters.VTotal);
			break;
		case SICMD_RO_Timing_H_Start_Slave:
			gAthena_LEFT_FE_Slave_Parameters.HStart = (WORD)(((((TimingHStartSlaveResPkt_t fdata*)gParabufferPtr)->HStart_HighByte)<<8)|
									(((TimingHStartSlaveResPkt_t fdata*)gParabufferPtr)->HStart_LowByte));			
			socipc_msg("\ngAthena_LEFT_FE_H_Start = %d.\n",gAthena_LEFT_FE_Slave_Parameters.HStart);
			break;
		case SICMD_RO_Timing_V_Start_Slave:
			gAthena_LEFT_FE_Slave_Parameters.VStart = (WORD)(((((TimingVStartSlaveResPkt_t fdata*)gParabufferPtr)->VStart_HighByte)<<8)|
									(((TimingVStartSlaveResPkt_t fdata*)gParabufferPtr)->VStart_LowByte));			
			socipc_msg("\ngAthena_LEFT_FE_V_Start = %d.\n",gAthena_LEFT_FE_Slave_Parameters.VStart);
			break;
		case SICMD_RO_Timing_V_Freq_Slave:
			gAthena_LEFT_FE_Slave_Parameters.VFreq= (WORD)(((((TimingVFreqSlaveResPkt_t fdata*)gParabufferPtr)->VFreq_HighByte)<<8)|
									(((TimingVFreqSlaveResPkt_t fdata*)gParabufferPtr)->VFreq_LowByte));			
			socipc_msg("\ngAthena_LEFT_FE_V_Fre	 = %d.\n",gAthena_LEFT_FE_Slave_Parameters.VFreq);
			break;
		case SICMD_RO_Port_Status:
			gAthena_LEFT_FE_Slave_Parameters.PortStatus = ((PortStatusResPkt_t fdata*)gParabufferPtr)->PortStatus;			
			socipc_msg("\ngAthena_LEFT_FE_Port Status = %d.\n",gAthena_LEFT_FE_Slave_Parameters.PortStatus);
			break;
		case SICMD_RO_Brightness_Status:
			gAthena_LEFT_FE_Slave_Parameters.Brightness = (SBYTE)(((BrightnessStatusResPkt_t fdata*)gParabufferPtr)->Brightness);			
			socipc_msg("\ngAthena_LEFT_FE_Brightness = 0x%X.\n",(BYTE)gAthena_LEFT_FE_Slave_Parameters.Brightness);
			break;
		case SICMD_RO_Contrast_Status:
			gAthena_LEFT_FE_Slave_Parameters.Contrast = (WORD)(((((ContrastStatusResPkt_t fdata*)gParabufferPtr)->Contrast_HighByte)<<8)|
									(((ContrastStatusResPkt_t fdata*)gParabufferPtr)->Contrast_LowByte));			
			socipc_msg("\ngAthena_LEFT_FE_Contrast = %d.\n",gAthena_LEFT_FE_Slave_Parameters.Contrast);
			break;
		case SICMD_RO_Sharpness_Status:
			gAthena_LEFT_FE_Slave_Parameters.Sharpness = ((SharpnessStatusResPkt_t fdata*)gParabufferPtr)->Sharpness;			
			socipc_msg("\ngAthena_LEFT_FE_Sharpness = %d.\n",gAthena_LEFT_FE_Slave_Parameters.Sharpness);
			break;
		case SICMD_RO_Color_Status:
			gAthena_LEFT_FE_Slave_Parameters.ColorSettings = (WORD)(((((ColorStatusResPkt_t fdata*)gParabufferPtr)->ColorSettings_HighByte)<<8)|
									(((ColorStatusResPkt_t fdata*)gParabufferPtr)->ColorSettings_LowByte));			
			socipc_msg("\ngAthena_LEFT_FE_Color Settings = 0x%X.\n",gAthena_LEFT_FE_Slave_Parameters.ColorSettings);
			break;
		case SICMD_RO_Video_progress_Status:
			gAthena_LEFT_FE_Slave_Parameters.VideoProgress = (WORD)(((((VideoProgressStatusResPkt_t fdata*)gParabufferPtr)->VideoProgress_HighByte)<<8)|
									(((VideoProgressStatusResPkt_t fdata*)gParabufferPtr)->VideoProgress_LowByte));		
			socipc_msg("\ngAthena_LEFT_FE_Video Progress Settings = 0x%X.\n",gAthena_LEFT_FE_Slave_Parameters.VideoProgress);
			break;
		case SICMD_RO_NR_Status:
			gAthena_LEFT_FE_Slave_Parameters.NRSettings = (WORD)(((((NRStatusResPkt_t fdata*)gParabufferPtr)->NR_HighByte)<<8)|
									(((NRStatusResPkt_t fdata*)gParabufferPtr)->NR_LowByte));			
			socipc_msg("\ngAthena_LEFT_FE_NR Settings = 0x%X.\n",gAthena_LEFT_FE_Slave_Parameters.NRSettings);
			break;
		case SICMD_RO_Video_progress1_Status:
			gAthena_LEFT_FE_Slave_Parameters.VideoProgress_1 = (WORD)(((((VideoProgress_1StatusResPkt_t fdata*)gParabufferPtr)->VideoProgress_1_HighByte)<<8)|
									(((VideoProgress_1StatusResPkt_t fdata*)gParabufferPtr)->VideoProgress_1_LowByte));			
			socipc_msg("\ngAthena_LEFT_FE_Video Progress_1 Settings = 0x%X.\n",gAthena_LEFT_FE_Slave_Parameters.VideoProgress_1);
			break;
		case SICMD_RO_DisplaySetup_Status:
			gAthena_LEFT_FE_Slave_Parameters.DisplaySetup = (WORD)(((((DisplaySetupStatusResPkt_t fdata*)gParabufferPtr)->DisplaySetup_HighByte)<<8)|
									(((DisplaySetupStatusResPkt_t fdata*)gParabufferPtr)->DisplaySetup_LowByte));		
			socipc_msg("\ngAthena_LEFT_FE_Display Setup Settings = 0x%X.\n",gAthena_LEFT_FE_Slave_Parameters.DisplaySetup);
			break;
		case SICMD_RO_Audio1_Status:
			gAthena_LEFT_FE_Slave_Parameters.Audio1 = (WORD)(((((Audio1StatusResPkt_t fdata*)gParabufferPtr)->Audio1_HighByte)<<8)|
									(((Audio1StatusResPkt_t fdata*)gParabufferPtr)->Audio1_LowByte));			
			socipc_msg("\ngAthena_LEFT_FE_Audio1 Settings = 0x%X.\n",gAthena_LEFT_FE_Slave_Parameters.Audio1);
			break;
		case SICMD_RO_Audio2_Status:
			gAthena_LEFT_FE_Slave_Parameters.Audio2 = (WORD)(((((Audio2StatusResPkt_t fdata*)gParabufferPtr)->Audio2_HighByte)<<8)|
									(((Audio2StatusResPkt_t fdata*)gParabufferPtr)->Audio2_LowByte));			
			socipc_msg("\ngAthena_LEFT_FE_Audio2 Settings = 0x%X.\n",gAthena_LEFT_FE_Slave_Parameters.Audio2);
			break;
		case SICMD_RO_Setup_Status:
			gAthena_LEFT_FE_Slave_Parameters.Setup = (WORD)(((((SetupStatusResPkt_t fdata*)gParabufferPtr)->Setup_HighByte)<<8)|
									(((SetupStatusResPkt_t fdata*)gParabufferPtr)->Setup_LowByte));			
			socipc_msg("\ngAthena_LEFT_FE_Setup Settings = 0x%X.\n",gAthena_LEFT_FE_Slave_Parameters.Setup);
			break;
      case SICMD_RO_DCR_Result:
			gAthena_LEFT_FE_Slave_Parameters.DcrResult = (WORD)(((((DcrResultResPkt_t fdata*)gParabufferPtr)->DcrDat_HighByte)<<8)|
									(((DcrResultResPkt_t fdata*)gParabufferPtr)->DcrDat_LowByte));			
			socipc_msg("\ngAthena_LEFT_DCR Result = %d.\n",gAthena_LEFT_FE_Slave_Parameters.DcrResult);
			break;
		case SICMD_RO_OSD_Sync_Status:
			gAthena_LEFT_FE_Slave_Parameters.OSDSync = ((OSDSyncStatusResPkt_t fdata*)gParabufferPtr)->OSDSync;			
			socipc_msg("\ngAthena_LEFT_FE_OSD Sync Status = 0x%X.\n",gAthena_LEFT_FE_Slave_Parameters.OSDSync);
			break;
		case SICMD_RO_OSD_Sync_changes:
			Change_Flag = (SBYTE)(((ChangeFlagStatusResPkt_t fdata*)gParabufferPtr)->ChangeFlag);			
			socipc_msg("\ngAthena_LEFT_FE_Change Flag = 0x%X.\n",(BYTE)Change_Flag);
			break;
/*ChengLin 20130128: New Sync Handler State Control*/				
		case SICMD_RW_IMAGE_SYNC_STATE_Status:
			gAthena_LEFT_FE_Slave_Parameters.ImageSyncState = (SBYTE)(((ImageSyncStateStatusResPkt_t fdata*)gParabufferPtr)->ImageSyncState);			
			socipc_msg("\ngAthena_LEFT_FE_ImageSyncState = 0x%X.\n",(BYTE)gAthena_LEFT_FE_Slave_Parameters.ImageSyncState);
			break;
/*********************************************/			
		case SICMD_RO_Slave_Registers_read_data:
			gAthena_LEFT_FE_Slave_Parameters.SlaveReadData = (WORD)(((((SlaveRegistersDataResPkt_t fdata*)gParabufferPtr)->Data_HighByte)<<8)|
									(((SlaveRegistersDataResPkt_t fdata*)gParabufferPtr)->Data_LowByte));			
			socipc_msg("\ngAthena_LEFT_FE_Register Read Data = 0x%X.\n",gAthena_LEFT_FE_Slave_Parameters.SlaveReadData);
			break;			
		case SICMD_RO_DDCI_Status:
			gAthena_LEFT_FE_Slave_Parameters.DDC_VCPCode = ((DDCCIStatusResPkt_t fdata*)gParabufferPtr)->DDC_VCP_Code;			
			gAthena_LEFT_FE_Slave_Parameters.DDC_VCP_Type = ((DDCCIStatusResPkt_t fdata*)gParabufferPtr)->DDC_VCP_Type;			
			gAthena_LEFT_FE_Slave_Parameters.DDC_VCP_Max = (WORD)(((((DDCCIStatusResPkt_t fdata*)gParabufferPtr)->DDC_VCP_Max_HighByte)<<8)|
									(((DDCCIStatusResPkt_t fdata*)gParabufferPtr)->DDC_VCP_Max_LowByte));
			gAthena_LEFT_FE_Slave_Parameters.DDC_Value = (WORD)(((((DDCCIStatusResPkt_t fdata*)gParabufferPtr)->DDC_Value_HighByte)<<8)|
									(((DDCCIStatusResPkt_t fdata*)gParabufferPtr)->DDC_Value_LowByte));			
			socipc_msg("\ngAthena_LEFT_FE_DDC_VCP Code = 0x%X.\n",gAthena_LEFT_FE_Slave_Parameters.DDC_VCPCode);
			socipc_msg("gAthena_LEFT_FE_DDC_VCP_Type = 0x%X.\n",gAthena_LEFT_FE_Slave_Parameters.DDC_VCP_Type);
			socipc_msg("gAthena_LEFT_FE_DDC_VCP_Max = 0x%X.\n",gAthena_LEFT_FE_Slave_Parameters.DDC_VCP_Max);
			socipc_msg("gAthena_LEFT_FE_DDC_Value = 0x%X.\n",gAthena_LEFT_FE_Slave_Parameters.DDC_Value);					
			break;
         
#ifdef DP_HUB_2_LOG_PORTS
      case SICMD_RO_DpRx0_IsStable:
         gAthena_LEFT_FE_Slave_Parameters.DPRX0_IsStable = (((DpRx0IsStableResPkt_t fdata*)gParabufferPtr)->DpRx0_IsStable);         
         break;         
#endif         

#ifdef SMART_ISP
      case SICMD_RO_SMARTISP_STATUS:
         gAthena_LEFT_FE_Slave_Parameters.SmartIspStep = (((SmartIspStatusResPkt_t fdata*)gParabufferPtr)->SmartIspStep);         
         gAthena_LEFT_FE_Slave_Parameters.SmartIspStatus= (((SmartIspStatusResPkt_t fdata*)gParabufferPtr)->SmartispStatus);                  
         //gm_Print("\ngAthena_LEFT_FE_SmartIspStep = 0x%X.\n",gAthena_LEFT_FE_Slave_Parameters.SmartIspStep);
         //gm_Print("\ngAthena_LEFT_FE_SmartIspStatus = 0x%X.\n",gAthena_LEFT_FE_Slave_Parameters.SmartIspStatus);
         break;         
#endif
		default:			
			break;
	}

}

//******************************************************************************
// FUNCTION:   SocIPCTxGetCheckSum
// USAGE:  Calculate Tx package checksum
// INPUT:  Bp_Buff, B_Len
// OUTPUT:	B_Checksum
// GLOBALS:
//******************************************************************************
BYTE SocIPCTxGetCheckSum(BYTE *Bp_Buff,BYTE B_Len)
{
   BYTE B_Count;
   BYTE B_Checksum = 0;

   for (B_Count = 0; B_Count < B_Len; B_Count++)
   {
      B_Checksum ^= Bp_Buff[B_Count];
   }

	return B_Checksum;
}

//******************************************************************************
// FUNCTION:   SocIPCTxRecvBufCheckCS
// USAGE:  Check Soc IPC Response package checksum is valid or not
// INPUT:  Bp_Buff, B_Len
// OUTPUT:	TRUE or FALSE
// GLOBALS:
//******************************************************************************
BOOL SocIPCTxRecvBufCheckCS(BYTE *Bp_Buff,BYTE B_Len)
{
   BYTE B_Count;
   BYTE B_Checksum = (0x50 & 0xFE);

   for (B_Count = 0; B_Count < (B_Len + 2); B_Count++)
   {
      B_Checksum ^= Bp_Buff[B_Count];
   }

	socipc_msg("\n<Soc IPC Message> Recv Buff Checksum = 0x%X.\n",B_Checksum);
	socipc_msg("\n<Soc IPC Message> Coming Packet Checksum = 0x%X.\n",Bp_Buff[(B_Len + 2)]);	

   if (B_Checksum == Bp_Buff[(B_Len + 2)])
   {
      return TRUE;
   }
   return FALSE;
}

//******************************************************************************
//
// FUNCTION     :  BYTE IsCompleted_SOC_IPC_I2C_Msg(void)
// USAGE        :   Checks for new command in the serial buffer.
//                The function checks for a new compelte command
//                  in the serial input buffer and if the command
//                  is incomplete checks for time out. If time out
//                  is detected it resynchronize the serial link and
//                  exits.
//
// INPUT        :   None
// OUTPUT       :   Returns gmd_TRUE if a command was found in the serial
//                  receive buffer or gmd_FALSE if not.
// GLOBALS      :   gBa_CommBuf
//                  	gW_RcvCount
//                  	gDebugCmdPtr
// USED_REGS    :   None
//
//******************************************************************************
BOOL IsCompleted_SOC_IPC_I2C_Msg(SLAVE_CH B_SlaveCh)
{
   BYTE B_Length;
   WORD W_Isr_RcvI2CCount;
   I2C_SLAVE_TYPE* Sp_I2CSlavePtr = Sp_I2CSlavePtrGlobal[B_SlaveCh];   

   gm_I2CSlaveSetChannel(B_SlaveCh);
	
   if (Sp_I2CSlavePtr != NULL_PTR)
   {
      if (Sp_I2CSlavePtr->State != I2C_SLAVE_STATE_STOP_RD)
      {
         return FALSE;
      }

      // only after is received a read stop condition
      W_Isr_RcvI2CCount = Sp_I2CSlavePtr->W_RcvCount;
      
      // Check total packet length received
      B_Length =  (~DDC2Bi_CONTROL_STATUS_FLAG) & Sp_I2CSlavePtr->Buffer[1];  // packet length

      if((W_Isr_RcvI2CCount > 0) && (W_Isr_RcvI2CCount >= (B_Length + 3)))
      {  // Message is received

         if(gm_I2CSlaveCheckCS(Sp_I2CSlavePtr->B_DevID, B_Length, &Sp_I2CSlavePtr->Buffer[0]))
         {
            Sp_I2CSlavePtr->State = I2C_SLAVE_STATE_WR;
            return TRUE;                                                 // message to be parsed
         }
         else
         {
            BYTE B_whichHandler = B_SlaveCh + I2C_SLAVE1_USE;
            gm_SendAckOrNack(SND_NAK, B_whichHandler, (BYTE*)&Sp_I2CSlavePtr->Buffer[Start_of_SoC_IPC_Cmd_in_DDC2BiMsg]);

            // clear read fifo
            gm_SetRegBitsByte(I2C_SLAVE_CTRL2, I2C_SLAVE1_READ_FIFO_RESET);
            gm_ClearRegBitsByte(I2C_SLAVE_CTRL2, I2C_SLAVE1_READ_FIFO_RESET);

            Sp_I2CSlavePtr->State = I2C_SLAVE_STATE_WR;
            return FALSE;                                                // Nothing to parse
         }
      }
   }

   return FALSE;                                                         // Nothing to parse
}



//******************************************************************************
// FUNCTION:   SocIPCI2CSlaveWriteCmdPkt
// USAGE:  SoC IPC Write Response Packet to FIFO
//
// INPUT:  pBuf, B_SlaveCh
// OUTPUT:	Parser status 
// GLOBALS:
//******************************************************************************
BOOL huge SocIPCI2CSlaveWriteCmdPkt(void far* pBuf, SLAVE_CH B_SlaveCh)
{
	BYTE i, B_Length, B_Checksum;
	
    I2C_SLAVE_TYPE* Sp_I2CData = gm_I2CSlaveGetAddrBuffer(B_SlaveCh);
    B_Checksum = ((SOCIPC_TX_SRC_ADDRESS-1) ^ (Sp_I2CData->B_DevID & I2C_ADDRESS_BITS));
    B_Length = *((BYTE far*)pBuf);

    (BYTE far*)pBuf -= Start_of_SoC_IPC_Cmd_in_DDC2BiMsg; //move back to the beginning of DDC2Bi packet 
    ((BYTE far*)pBuf)[0] = (B_Length + 2) | 0x80;
    // length for DDC2Bi msg 
    ((BYTE far*)pBuf)[1] = SOC_IPC_TX_VCP_PREFIX_CMD;    // Specify Soc IPC command 
    ((BYTE far*)pBuf)[2] = (BYTE)(SOC_IPC_TX_VCP_PREFIX_CMD_PAGE>>8); 	// manufacturer id 
    ((BYTE far*)pBuf)[3] = (BYTE)SOC_IPC_TX_VCP_PREFIX_CMD_PAGE; 		// manufacturer id 

    // Copy response message. It is possibly ACK/NACK template or message that is allready in gDebugCmdPtr 
    _fmemcpy(((BYTE far*)pBuf + 4), ((BYTE far*)pBuf + Start_of_SoC_IPC_Cmd_in_DDC2BiMsg), (B_Length - 1));
    // 
    for (i=0; i < B_Length + 3; i++)            // Process the whole packet 
    {
        B_Checksum ^= ((BYTE far*)pBuf)[i];    	// Update checksum 
    }
    // 
    // add to not be forced to discriminate the stop condition 
    // 
    ((BYTE far*)pBuf)[B_Length + 3] = B_Checksum;
    ((BYTE far*)pBuf)[B_Length + 4] = gB_DDC2BiDevId;	

	gm_I2CSlaveSetReply((B_Length + 5), B_SlaveCh);

	return TRUE;
}



//******************************************************************************
// FUNCTION:   SocIPCSendKeypadEvent
// USAGE:  Send Keypad status to slave Soc via I2C Master
//
// INPUT:  None
// OUTPUT:	None 
// GLOBALS:
//******************************************************************************
#if ENABLE_SOC_IPC_KEYPAD_VIA_QUEUE
void SocIPCSendKeypadEvent(void)
{
	BYTE SocIPCBuffer[2] = {0};
	KeypadRecvFormat_t GetKeypadKey = {0,0,0};

   	if( IsInitialFinished )
    {
		while( SocIPCIsKeypadBufferExistValidKey() )
		{	
			GetKeypadKey = SocIPCKeypadBufferGetValidKey();
				
			SocIPCBuffer[0] = GetKeypadKey.KeyValue;		//ChengLin: Send High byte first			 
			SocIPCBuffer[1] = GetKeypadKey.KeyChannel;
				
			socipc_kbd_ir_msg("V = %d", GetKeypadKey.KeyValue);
			socipc_kbd_ir_msg("C = %d", GetKeypadKey.KeyChannel);

#if ENABLE_SOC_IPC_BUSY_CHECK
			if( SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_Keypad_CONTRL, SocIPCBuffer) == FALSE)
         	{
       			//Send command again
					if( SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_Keypad_CONTRL, SocIPCBuffer) == FALSE)
					{			
						socipc_busychk_msg("\n<Soc IPC Error> Send Device LEFT Keypad Command Failure.\n",0);				
					}                                            
       		}

			if( SocIPCSendCmdPacketParser(SOCIPC_DEVICEFE_DST_ADDRESS, SICMD_RW_Keypad_CONTRL, SocIPCBuffer) == FALSE)
    		{
        		//Send command again
				if( SocIPCSendCmdPacketParser(SOCIPC_DEVICEFE_DST_ADDRESS, SICMD_RW_Keypad_CONTRL, SocIPCBuffer) == FALSE)
				{				
					socipc_busychk_msg("\n<Soc IPC Error> Send Device FE Keypad Command Failure.\n",0);
				}
  			}   
#else //Original
			{
    			SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_Keypad_CONTRL, SocIPCBuffer);   
				SocIPCSendCmdPacketParser(SOCIPC_DEVICEFE_DST_ADDRESS, SICMD_RW_Keypad_CONTRL, SocIPCBuffer);   
			}
#endif 					
		}	
     }
}

#else //Original
void SocIPCSendKeypadEvent(void)
{
	BYTE SocIPCBuffer[2] = {0};

	if(g_Keypad_GetValidKeys == TRUE)
  	{
   		if( IsInitialFinished )
      	{
      		SocIPCBuffer[0] = g_Keypad_Value;       //ChengLin: Send High byte first             
     		SocIPCBuffer[1] = g_Keypad_Channel;

			socipc_kbd_ir_msg("V = %d", g_Keypad_Value);
			socipc_kbd_ir_msg("C = %d", g_Keypad_Channel);
				

#if ENABLE_SOC_IPC_BUSY_CHECK
			if( SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_Keypad_CONTRL, SocIPCBuffer) == FALSE)
         	{
       			//Send command again
					if( SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_Keypad_CONTRL, SocIPCBuffer) == FALSE)
					{			
						socipc_busychk_msg("\n<Soc IPC Error> Send Device LEFT Keypad Command Failure.\n",0);			
					}                                            
       		}

				if( SocIPCSendCmdPacketParser(SOCIPC_DEVICEFE_DST_ADDRESS, SICMD_RW_Keypad_CONTRL, SocIPCBuffer) == FALSE)
    		{
        		//Send command again
					if( SocIPCSendCmdPacketParser(SOCIPC_DEVICEFE_DST_ADDRESS, SICMD_RW_Keypad_CONTRL, SocIPCBuffer) == FALSE)
					{				
						socipc_busychk_msg("\n<Soc IPC Error> Send Device FE Keypad Command Failure.\n",0);
    				}           
			}
#else //Original
			{
    			SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_Keypad_CONTRL, SocIPCBuffer);   
				SocIPCSendCmdPacketParser(SOCIPC_DEVICEFE_DST_ADDRESS, SICMD_RW_Keypad_CONTRL, SocIPCBuffer);   
			}
#endif    

     	}

		g_Keypad_Channel = 0x00;
        g_Keypad_Value = 0x00;
    	g_Keypad_GetValidKeys = FALSE;
	}
}
#endif


//******************************************************************************
// FUNCTION:   SocIPCSendIREvent
// USAGE:  Send IR status to slave Soc via I2C Master
//
// INPUT:  None
// OUTPUT:	None 
// GLOBALS:
//******************************************************************************
#if ENABLE_SOC_IPC_IR_VIA_QUEUE
void SocIPCSendIREvent(void)
{
	BYTE SocIPCBuffer[1] = {0};
	IRRecvFormat_t GetIRKey = {0,0};

	if( IsInitialFinished )
   	{
		while( SocIPCIsIRBufferExistValidKey() )
		{	

			GetIRKey = SocIPCIRBufferGetValidKey();
       		SocIPCBuffer[0] = GetIRKey.IRKey;				
      		socipc_kbd_ir_msg("\nIR Key Value = 0x%x.\n",GetIRKey.IRKey);                                 

#if ENABLE_SOC_IPC_BUSY_CHECK
    		if( SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_IR_CONTROL, SocIPCBuffer) == FALSE)
       		{
   				//Send command again
            	if( SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_IR_CONTROL, SocIPCBuffer) == FALSE)
				{				
         			socipc_busychk_msg("\n<Soc IPC Error> Send Device LEFT IR Command Failure.\n",0);			
				}
          	}

      		if( SocIPCSendCmdPacketParser(SOCIPC_DEVICEFE_DST_ADDRESS, SICMD_RW_IR_CONTROL, SocIPCBuffer) == FALSE)
      		{
         		//Send command again
           		if( SocIPCSendCmdPacketParser(SOCIPC_DEVICEFE_DST_ADDRESS, SICMD_RW_IR_CONTROL, SocIPCBuffer) == FALSE)
				{			
        			socipc_busychk_msg("\n<Soc IPC Error> Send Device FE IR Command Failure.\n",0);			
				}
         	}
#else //Original
			{
     			SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_IR_CONTROL, SocIPCBuffer);
     			SocIPCSendCmdPacketParser(SOCIPC_DEVICEFE_DST_ADDRESS, SICMD_RW_IR_CONTROL, SocIPCBuffer);
			}
#endif   
		}
    }
}
#else //Original
void SocIPCSendIREvent(void)
{
	BYTE SocIPCBuffer[1] = {0};

 	if(g_IR_GetValidKeys == TRUE)
 	{
		if( IsInitialFinished )
   		{                  
      		socipc_kbd_ir_msg("\nIR Key Value = 0x%x.\n",g_IR_Key);                                 
       		SocIPCBuffer[0] = g_IR_Key;

#if ENABLE_SOC_IPC_BUSY_CHECK
    		if( SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_IR_CONTROL, SocIPCBuffer) == FALSE)
       		{
   				//Send command again
            	if( SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_IR_CONTROL, SocIPCBuffer) == FALSE)
				{				
         			socipc_busychk_msg("\n<Soc IPC Error> Send Device LEFT IR Command Failure.\n",0);			
				}
          	}

      		if( SocIPCSendCmdPacketParser(SOCIPC_DEVICEFE_DST_ADDRESS, SICMD_RW_IR_CONTROL, SocIPCBuffer) == FALSE)
      		{
         		//Send command again
           		if( SocIPCSendCmdPacketParser(SOCIPC_DEVICEFE_DST_ADDRESS, SICMD_RW_IR_CONTROL, SocIPCBuffer) == FALSE)
				{			
        			socipc_busychk_msg("\n<Soc IPC Error> Send Device FE IR Command Failure.\n",0);			
				}
         	}
#else //Original
			{
     		SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_IR_CONTROL, SocIPCBuffer);
     		SocIPCSendCmdPacketParser(SOCIPC_DEVICEFE_DST_ADDRESS, SICMD_RW_IR_CONTROL, SocIPCBuffer);
			}
#endif                                  
    	}
     	g_IR_GetValidKeys = FALSE;
     	g_IR_Key = 0;
 	}
}
#endif

#if ENABLE_SOC_IPC_BUSY_CHECK
//******************************************************************************
// FUNCTION:   SocIPCSetClearSlaveLeftBusyStatus
// USAGE:  Set Athena Left to busy/ready status
//
// INPUT:  Busy
// OUTPUT:	None 
// GLOBALS:
//******************************************************************************
void SocIPCSetClearSlaveLeftBusyStatus(BOOL Busy)
{
	// LEFT Busy pin GPIO95
	if(Busy) //busy
		gm_ClearRegBitsWord(GPOUTPUT5, GPIO95_OUT);
	else //Ready
		gm_SetRegBitsWord(GPOUTPUT5, GPIO95_OUT);
}

//******************************************************************************
// FUNCTION:   SocIPCSetClearSlaveFEBusyStatus
// USAGE:  Set Athena FE to busy/ready status
//
// INPUT:  Busy
// OUTPUT:	None 
// GLOBALS:
//******************************************************************************
void SocIPCSetClearSlaveFEBusyStatus(BOOL Busy)
{
	// FE Busy pin GPIO94
	if(Busy) //busy
		gm_ClearRegBitsWord(GPOUTPUT5, GPIO94_OUT);
	else //Ready
		gm_SetRegBitsWord(GPOUTPUT5, GPIO94_OUT);
}

//******************************************************************************
// FUNCTION:   SocIPCIsSlaveLeftReady
// USAGE:  Check Athena Left is Busy or not before send out I2C protocol
//
// INPUT:  None
// OUTPUT:	None 
// GLOBALS:
//******************************************************************************
BOOL SocIPCIsSlaveLeftReady(void)
{
	//ChengLin 20120116: Check Athena Right GPIO95 for confirm Athena Left is ready for receive I2C commands
	
	if( gm_ReadRegWord(GPINPUT5)&0x8000 ) //Bit 15
		return TRUE;
	else
		return FALSE;
}

//******************************************************************************
// FUNCTION:   SocIPCIsSlaveFEReady
// USAGE:  Check Athena FE is Busy or not before send out I2C protocol
//
// INPUT:  None
// OUTPUT:	None 
// GLOBALS:
//******************************************************************************
BOOL SocIPCIsSlaveFEReady(void)
{
	//ChengLin 20120201: Check Athena Right GPIO94 for confirm Athena FE is ready for receive I2C commands (Low is Ready)
	
	if( gm_ReadRegWord(GPINPUT5)&0x4000 ) //Bit 14
		return TRUE;
	else
		return FALSE;
}
#endif

/*ChengLin 20130129: Enable Bidirectional BUSY Status Check*/
#if(ENABLE_BIDIRECTIONAL_BUSY_CHECK)
//******************************************************************************
// FUNCTION:   SocIPCWaitAthenaMasterReady
// USAGE:  Check Athena Right Master is SET READY or not for Sync Athena R and Athena L/FE
//
// INPUT:  None
// OUTPUT:	bReady (TRUE: READY, FALSE: BUSY) 
// GLOBALS:
//******************************************************************************
BOOL SocIPCWaitAthenaMasterReady(void)
{
	BOOL bReady = FALSE;

	gm_TimerStart(SOCIPC_LEFT_FE_RIGHT_SYNC_WAIT_10ms_TMR, 50);
	
	while(1)
	{
		if((GetChipID_4K2K() == CHIPID_4K2K_L && SocIPCIsSlaveLeftReady()) \
			||(GetChipID_4K2K() == CHIPID_4K2K_FE && SocIPCIsSlaveFEReady()))
		{
			bReady = TRUE;		
			break;
		}
	
		if (gm_TimerCheck(SOCIPC_LEFT_FE_RIGHT_SYNC_WAIT_10ms_TMR) == TIMER_TMO)
		{
		   	gm_Print("<SOCIPC TimeOut> Athena Right not to set READY bit", 0);
			bReady = FALSE;				   
		   	break;
		}
		//gm_Print("\nGET READY From Athena R......NON OK!!\n",0);			
	}
	gm_TimerStop(SOCIPC_LEFT_FE_RIGHT_SYNC_WAIT_10ms_TMR);
	//gm_Print("\nGET READY From Athena R......OK!!\n",0);

	return bReady;
}
#endif

#if ENABLE_SOC_IPC_OSD_SYNC //ChengLin 20130115: Disable this function first for new 4k2k board
//******************************************************************************
// FUNCTION:   SocIPCMasterSlaveOSDSyncSettings
// USAGE:  Sync Athena RIGHT/LEFT/FE  OSD Settings
//
// INPUT: 
// OUTPUT:	TURE or FALSE
// GLOBALS:
//******************************************************************************
BOOL SocIPCMasterSlaveOSDSyncSettings(void)
{
	BYTE ReadDevID = 0x00;
	BYTE WriteDevID = 0x00;	
	BYTE CmdBuf[2]={0};

	/*ChengLin 20130115: System will always to sync OSD settings via Athena LEFT, if Athena LEFT is unavailable, system will change to Athena FE for OSD sync.
		Case 1: Athena LEFT is available: Athena LEFT -> Athena RIGHT -> Athena FE.
		Case 2: Athena LEFT is unavailable: Athena FE-> Athena RIGHT -> Athena LEFT.
	*/

	//Step 0: Check which slave device will be used for OSD sync feature.
	if(SocIPCCheckDevice(SOCIPC_DEVICEL_DST_ADDRESS))
	{
		ReadDevID = SOCIPC_DEVICEL_DST_ADDRESS;
		WriteDevID = SOCIPC_DEVICEFE_DST_ADDRESS;				
		socipc_osd_sync_msg("\n<Soc IPC Message> OSD Sync via Athena L.\n",0);	
	}
	else
	{
		ReadDevID = SOCIPC_DEVICEFE_DST_ADDRESS;
		WriteDevID = SOCIPC_DEVICEL_DST_ADDRESS;			
		socipc_osd_sync_msg("\n<Soc IPC Message> OSD Sync via Athena FE.\n",0);
	}
	else
	{
		socipc_osd_sync_msg("\n<Soc IPC Message> OSD Sync Failure.\n",0);
		return FALSE;
	}


	//Step1: Reset all Athena RIGHT OSD related OSD Settings
	CmdBuf[0] = 0x01;
	if(SocIPCSendCmdPacketParser(ReadDevID, SICMD_RW_OSD_Sync_CONTRL, CmdBuf) == FALSE)
	{	
		socipc_osd_sync_msg("\n<Soc IPC Error> Device RIGHT OSD Sync-Sync Control Failure.\n",0);
		return FALSE;
	}

	//Step2: Read Athena LEFT/FE Brightness and save to Athena RIGHT NVRAM
	if(SocIPCSendCmdPacketParser(ReadDevID, SICMD_RO_Brightness_Status, NULL) == FALSE)
	{	
		socipc_osd_sync_msg("\n<Soc IPC Error> Device RIGHT OSD Sync-Get Brightness Failure.\n",0);
		return FALSE;
	}
	else
	{
		UserPrefBrightness = gAthena_LEFT_FE_Slave_Parameters.Brightness;
	}

	//Step3: Read Athena LEFT/FE Contrast and save to Athena RIGHT NVRAM
	if(SocIPCSendCmdPacketParser(ReadDevID, SICMD_RO_Contrast_Status, NULL) == FALSE)
	{	
		socipc_osd_sync_msg("\n<Soc IPC Error> Device RIGHT OSD Sync-Get Contrast Failure.\n",0);
		return FALSE;
	}
	else
	{
		UserPrefContrast = gAthena_LEFT_FE_Slave_Parameters.Contrast;
	}

	//Step4: Read Athena LEFT/FE Sharpness and save to Athena RIGHT NVRAM
	if(SocIPCSendCmdPacketParser(ReadDevID, SICMD_RO_Sharpness_Status, NULL) == FALSE)
	{	
		socipc_osd_sync_msg("\n<Soc IPC Error> Device RIGHT OSD Sync-Get Sharpness Failure.\n",0);
		return FALSE;
	}
	else
	{
		SocIPCSetCorrectSharpnessSetting(gAthena_LEFT_FE_Slave_Parameters.Sharpness);	
	}

	//Step5: Read Athena LEFT/FE Color Settings and save to Athena RIGHT NVRAM
	if(SocIPCSendCmdPacketParser(ReadDevID, SICMD_RO_Color_Status, NULL) == FALSE)
	{	
		socipc_osd_sync_msg("\n<Soc IPC Error> Device RIGHT OSD Sync-Get Color Settings Failure.\n",0);
		return FALSE;
	}
	else
	{
		switch( (BYTE)(gAthena_LEFT_FE_Slave_Parameters.ColorSettings & 0x000F) )
		{
			case 0:
				UserPrefColorTemp = CTEMP_6500K;
				break;
			case 1:
				UserPrefColorTemp = CTEMP_7500K;
				break;
			case 2:
				UserPrefColorTemp = CTEMP_9300K;
				break;
			case 3:
				UserPrefColorTemp = USER_TEMP;
				break;
			default:
				UserPrefColorTemp = CTEMP_6500K;				
				break;
		}

		UserPrefImageScheme = (SBYTE)((gAthena_LEFT_FE_Slave_Parameters.ColorSettings & 0x00F0)>>4);
		UserPrefColorSpace = (BYTE)((gAthena_LEFT_FE_Slave_Parameters.ColorSettings & 0x0F00)>>8);	
		UserPrefDUMSwitch = (SBYTE)((gAthena_LEFT_FE_Slave_Parameters.ColorSettings & 0xF000)>>12);
	}		
	
	//Step6: Read Athena LEFT/FE Video Progress and save to Athena RIGHT NVRAM
	if(SocIPCSendCmdPacketParser(ReadDevID, SICMD_RO_Video_progress_Status, NULL) == FALSE)
	{	
		socipc_osd_sync_msg("\n<Soc IPC Error> Device RIGHT OSD Sync-Get Video Progress Failure.\n",0);
		return FALSE;
	}
	else
	{
		UserPrefDCDiMain = (SBYTE)(gAthena_LEFT_FE_Slave_Parameters.VideoProgress & 0x000F);
		UserPrefMADI = (SBYTE)((gAthena_LEFT_FE_Slave_Parameters.VideoProgress & 0x00F0)>>4);
		UserPrefLCDOverdrive = (BYTE)((gAthena_LEFT_FE_Slave_Parameters.VideoProgress & 0x0F00)>>8);
	}	

	//Step7: Read Athena LEFT/FE NR Settings and save to Athena RIGHT NVRAM
	if(SocIPCSendCmdPacketParser(ReadDevID, SICMD_RO_NR_Status, NULL) == FALSE)
	{	
		socipc_osd_sync_msg("\n<Soc IPC Error> Device RIGHT OSD Sync-Get NR Settings Failure.\n",0);
		return FALSE;
	}
	else
	{
		UserPrefTNR = (SBYTE)(gAthena_LEFT_FE_Slave_Parameters.NRSettings & 0x000F);
		UserPrefMPEGNRMode = (BYTE)((gAthena_LEFT_FE_Slave_Parameters.NRSettings & 0x00F0)>>4);
	}

	//Step8: Read Athena LEFT/FE Video Progress 1 and save to Athena RIGHT NVRAM
	if(SocIPCSendCmdPacketParser(ReadDevID, SICMD_RO_Video_progress1_Status, NULL) == FALSE)
	{	
		socipc_osd_sync_msg("\n<Soc IPC Error> Device RIGHT OSD Sync-Get Video Progress1 Failure.\n",0);
		return FALSE;
	}
	else
	{
		UserPrefFilmMode = (SBYTE)(gAthena_LEFT_FE_Slave_Parameters.VideoProgress_1 & 0x000F);
	}

	//Step9: Read Athena LEFT/FE Display Setup and save to Athena RIGHT NVRAM
	if(SocIPCSendCmdPacketParser(ReadDevID, SICMD_RO_DisplaySetup_Status, NULL) == FALSE)
	{	
		socipc_osd_sync_msg("\n<Soc IPC Error> Device RIGHT OSD Sync-Get Display Setup Failure.\n",0);
		return FALSE;
	}
	else
	{
		UserPrefOutputAspectRatio = (BYTE)(gAthena_LEFT_FE_Slave_Parameters.DisplaySetup & 0x000F);
	}	

	//Step10: Read Athena LEFT/FE Audio1 Settings and save to Athena RIGHT NVRAM
	if(SocIPCSendCmdPacketParser(ReadDevID, SICMD_RO_Audio1_Status, NULL) == FALSE)
	{
		socipc_osd_sync_msg("\n<Soc IPC Error> Device RIGHT OSD Sync-Get Audio1 Failure.\n",0);
		return FALSE;
	}
	else
	{
		//0:3	Main ouput	000: Line out; 010:SPDIF out; Others: Reserved
		//4:7	Mute		000: Off; 001: On; Others: Reserved
		//8:11	DownMix 		000: Off; 001: On; Others: Reserved

		UserPrefAudioMainOutput = (BYTE)(gAthena_LEFT_FE_Slave_Parameters.Audio1 & 0x000F);
		UserPrefAudioMute = (BYTE)((gAthena_LEFT_FE_Slave_Parameters.Audio1 & 0x00F0)>>4);		
		UserPrefAudioDownMix = (BYTE)((gAthena_LEFT_FE_Slave_Parameters.Audio1 & 0x0F00)>>8);		
	}

	//Step11: Read Athena LEFT/FE Audio2 Settings and save to Athena RIGHT NVRAM
	if(SocIPCSendCmdPacketParser(ReadDevID, SICMD_RO_Audio2_Status, NULL) == FALSE)
	{	
		socipc_osd_sync_msg("\n<Soc IPC Error> Device RIGHT OSD Sync-Get Audio2 Failure.\n",0);
		return FALSE;
	}
	else
	{
		UserPrefAudioVolume = (BYTE)(gAthena_LEFT_FE_Slave_Parameters.Audio2 & 0x003F);
		UserPrefAVSDelayTime = (WORD)((gAthena_LEFT_FE_Slave_Parameters.Audio2 & 0xFFC0)>>6);
	}	

	//Step12: Read Athena LEFT/FE Setup Settings and save to Athena RIGHT NVRAM
	if(SocIPCSendCmdPacketParser(ReadDevID, SICMD_RO_Setup_Status, NULL) == FALSE)
	{	
		socipc_osd_sync_msg("\n<Soc IPC Error> Device RIGHT OSD Sync-Get Setup Failure.\n",0);
		return FALSE;
	}
	else
	{
		UserPrefDCR = (BYTE)(gAthena_LEFT_FE_Slave_Parameters.Setup & 0x000F);
		UserPrefScrollMenuItem = (BYTE)((gAthena_LEFT_FE_Slave_Parameters.Setup & 0x00F0)>>4);
	}

	//Step13: Save above change into Athena RIGHT NVRAM
	SaveModeIndependentSettings();
	SavePortSchemeDependentSettings();

	//Step14: Send New Athena RIGHT OSD Settings to Athena FE or Athena LEFT based on "SocIPCCheckDevice()" status
	if(SocIPCSendCmdPacketParser(WriteDevID, SICMD_RW_Brightness_CONTRL, NULL) == FALSE)
	{	
		socipc_osd_sync_msg("\n<Soc IPC Error> Device RIGHT OSD Sync-Brightness_CONTRL Failure.\n",0);
		return FALSE;
	}

	if(SocIPCSendCmdPacketParser(WriteDevID, SICMD_RW_Contrast_CONTRL, NULL) == FALSE)
	{	
		socipc_osd_sync_msg("\n<Soc IPC Error> Device RIGHT OSD Sync-Contrast_CONTRL Failure.\n",0);
		return FALSE;
	}	

	if(SocIPCSendCmdPacketParser(WriteDevID, SICMD_RW_Sharpness_CONTRL, NULL) == FALSE)
	{	
		socipc_osd_sync_msg("\n<Soc IPC Error> Device RIGHT OSD Sync-Sharpness_CONTRL Failure.\n",0);
		return FALSE;
	}

	if(SocIPCSendCmdPacketParser(WriteDevID, SICMD_RW_Color_CONTRL, NULL) == FALSE)
	{	
		socipc_osd_sync_msg("\n<Soc IPC Error> Device RIGHT OSD Sync-Color_CONTRL Failure.\n",0);
		return FALSE;
	}

	if(SocIPCSendCmdPacketParser(WriteDevID, SICMD_RW_Video_progress_CONTRL, NULL) == FALSE)
	{	
		socipc_osd_sync_msg("\n<Soc IPC Error> Device RIGHT OSD Sync-Video_progress_CONTRL Failure.\n",0);
		return FALSE;
	}

	if(SocIPCSendCmdPacketParser(WriteDevID, SICMD_RW_NR_CONTRL, NULL) == FALSE)
	{	
		socipc_osd_sync_msg("\n<Soc IPC Error> Device RIGHT OSD Sync-NR_CONTRL Failure.\n",0);
		return FALSE;
	}

	if(SocIPCSendCmdPacketParser(WriteDevID, SICMD_RW_Video_progress1_CONTRL, NULL) == FALSE)
	{	
		socipc_osd_sync_msg("\n<Soc IPC Error> Device RIGHT OSD Sync-Video_progress1_CONTRL Failure.\n",0);
		return FALSE;
	}

   /*ChengLin 20130315: Add for splash screen case*/
   CmdBuf[0] = TRUE; //Update Flag 
	if(SocIPCSendCmdPacketParser(WriteDevID, SICMD_RW_DisplaySetup_CONTRL, CmdBuf) == FALSE)
	{	
		socipc_osd_sync_msg("\n<Soc IPC Error> Device RIGHT OSD Sync-DisplaySetup_CONTRL Failure.\n",0);
		return FALSE;
	}

	if(SocIPCSendCmdPacketParser(WriteDevID, SICMD_RW_Audio1_CONTRL, NULL) == FALSE)
	{	
		socipc_osd_sync_msg("\n<Soc IPC Error> Device RIGHT OSD Sync-Audio1_CONTRL Failure.\n",0);
		return FALSE;
	}
	
	if(SocIPCSendCmdPacketParser(WriteDevID, SICMD_RW_Audio2_CONTRL, NULL) == FALSE)
	{		
		socipc_osd_sync_msg("\n<Soc IPC Error> Device RIGHT OSD Sync-Audio2_CONTRL Failure.\n",0);
		return FALSE;
	}	

	if(SocIPCSendCmdPacketParser(WriteDevID, SICMD_RW_Setup_CONTRL, NULL) == FALSE)
	{		
		socipc_osd_sync_msg("\n<Soc IPC Error> Device RIGHT OSD Sync-Setup_CONTRL Failure.\n",0);
		return FALSE;
	}

	//Step15: Check Athena RIGHT and Athena LEFT/FE OSD settings are the same or not?
	if(SocIPCSendCmdPacketParser(WriteDevID, SICMD_RO_OSD_Sync_Status, CmdBuf) == FALSE)
	{	
		socipc_osd_sync_msg("\n<Soc IPC Error> Device RIGHT OSD Sync-Get OSD Sync Status Failure.\n",0);
		return FALSE;
	}
	else
	{
		if(gAthena_LEFT_FE_Slave_Parameters.OSDSync)
			return TRUE; //OSD Sync Successful
		else
			return FALSE; //OSD Sync Failure			
	}

}
#endif

//******************************************************************************
// FUNCTION:   SocIPCCheckDevice
// USAGE:  Check Athena LEFT or Athena FE is exist or not
//
// INPUT:  DevID
// OUTPUT:	TURE or FALSE
// GLOBALS:
//******************************************************************************
BOOL SocIPCCheckDevice(BYTE DevID)
{
	BOOL bRet = FALSE;

	if( (DevID != SOCIPC_DEVICEL_DST_ADDRESS) && (DevID != SOCIPC_DEVICEFE_DST_ADDRESS))
	{	
		socipc_msg("\n<Soc IPC Error> Invalid Device ID.\n",0);
		return FALSE;
	}
	gm_2WireSelectIdx (MASTER_2WIRE_2); //Added by ChengLin for avoid appstest can't use
	if (gm_2WireCheck(0, DevID) == gmd_PASS)
		bRet = TRUE;
	else
		bRet = FALSE;
	gm_2WireSelectIdx (MASTER_2WIRE_1); //Added by ChengLin for avoid appstest can't use	

	return bRet;
}

#if ENABLE_SOC_IPC_KEYPAD_VIA_QUEUE
//******************************************************************************
// FUNCTION:   SocIPCKeypadBufferInitial
// USAGE:  Initial Soc IPC Keypad Queue
//
// INPUT:  None
// OUTPUT:	None 
// GLOBALS:
//******************************************************************************
void SocIPCKeypadBufferInitial(void)
{
	WORD wCount = 0;

	for( wCount = 0; wCount < KEYPAD_BUFFER_MAX; wCount++ )
	{
		gKeypadQueueBuffer[wCount].ValidKey = 0;
		gKeypadQueueBuffer[wCount].KeyChannel = 0;
		gKeypadQueueBuffer[wCount].KeyValue = 0;
	}
	
#if ENABLE_SOC_IPC_KEYPAD_IR_QUEUE_DEBUG
	for( wCount = 0; wCount < KEYPAD_BUFFER_MAX; wCount++ )
	{
		socipc_kbd_ir_queue_msg("\nKeypad Buffer Index = %d.\n",wCount);	
		socipc_kbd_ir_queue_msg("ValidKey = %d.\n",gKeypadQueueBuffer[wCount].ValidKey);
		socipc_kbd_ir_queue_msg("KeyChannel = %d.\n",gKeypadQueueBuffer[wCount].KeyChannel);
		socipc_kbd_ir_queue_msg("KeyValue = %d.\n",gKeypadQueueBuffer[wCount].KeyValue);
		socipc_kbd_ir_queue_msg("\n\n",0);
	}	
#endif
}

//******************************************************************************
// FUNCTION:   SocIPCKeypadBufferInsertKey
// USAGE:  Insert a valid keypad value to keypad Queue
//
// INPUT:  KeypadValidKey
// OUTPUT:	None 
// GLOBALS:
//******************************************************************************
void SocIPCKeypadBufferInsertKey(KeypadRecvFormat_t KeypadValidKey)
{
	WORD wCount = 0;

	//Only for valid keypad keys	
	if(KeypadValidKey.ValidKey == FALSE)
		return;

	//Get First Available Index
	for( wCount = 0; wCount < KEYPAD_BUFFER_MAX; wCount++ )
	{
		if(gKeypadQueueBuffer[wCount].ValidKey == FALSE)
			break;
	}

	//Check Buffer is full or not?
	if(wCount == KEYPAD_BUFFER_MAX)
	{	
		socipc_kbd_ir_queue_msg("\nKeypad Buffer is Full!!\n",0);
		return;
	}

	//Insert new vaild key to buffer
	gKeypadQueueBuffer[wCount].ValidKey = KeypadValidKey.ValidKey;
	gKeypadQueueBuffer[wCount].KeyChannel = KeypadValidKey.KeyChannel;
	gKeypadQueueBuffer[wCount].KeyValue = KeypadValidKey.KeyValue;
	
}

//******************************************************************************
// FUNCTION:   SocIPCIsKeypadBufferExistValidKey
// USAGE:  Check Keypad Queue have valid keys or not
//
// INPUT:  None
// OUTPUT:	TRUE or FALSE 
// GLOBALS:
//******************************************************************************
BOOL SocIPCIsKeypadBufferExistValidKey(void)
{
	WORD wCount = 0;

	for( wCount = 0; wCount < KEYPAD_BUFFER_MAX; wCount++ )
	{
		if(gKeypadQueueBuffer[wCount].ValidKey == TRUE)
			return TRUE; //Buffer exist valid keys
	}
	return FALSE;
}

//******************************************************************************
// FUNCTION:   SocIPCKeypadBufferGetValidKey
// USAGE:  GET a valid keypad key from keypad Queue
//
// INPUT:  None
// OUTPUT:	RetValidKey
// GLOBALS:
//******************************************************************************
KeypadRecvFormat_t SocIPCKeypadBufferGetValidKey(void)
{
	WORD wCount = 0;
	WORD wSortCount = 0;	
	KeypadRecvFormat_t RetValidKey = {0,0,0};
	KeypadRecvFormat_t TempKey = {0,0,0};

	//Get First Valid Key Index
	for( wCount = 0; wCount < KEYPAD_BUFFER_MAX; wCount++ )
	{
		if(gKeypadQueueBuffer[wCount].ValidKey == TRUE)
			break;
	}

	if(wCount == KEYPAD_BUFFER_MAX)
	{	
		socipc_kbd_ir_queue_msg("\nKeypad Buffer is Empty!!\n",0);
		return RetValidKey;
	}

	//Get Valid Key
	RetValidKey.ValidKey = gKeypadQueueBuffer[wCount].ValidKey;
	RetValidKey.KeyChannel = gKeypadQueueBuffer[wCount].KeyChannel;
	RetValidKey.KeyValue = gKeypadQueueBuffer[wCount].KeyValue;		

	//Clear Valid Key
 	gKeypadQueueBuffer[wCount].ValidKey = FALSE;
	gKeypadQueueBuffer[wCount].KeyChannel = 0;
	gKeypadQueueBuffer[wCount].KeyValue = 0;

	//Sort Buffer
	for( wSortCount = 0; wSortCount < (KEYPAD_BUFFER_MAX-1); wSortCount++ )
	{
		for( wCount = wSortCount; wCount < (KEYPAD_BUFFER_MAX-1); wCount++ )
		{
		
			if(gKeypadQueueBuffer[wCount].ValidKey == TRUE)
			{
				continue;
			}
			else
			{
				TempKey.ValidKey = gKeypadQueueBuffer[wCount+1].ValidKey;
				TempKey.KeyChannel = gKeypadQueueBuffer[wCount+1].KeyChannel;			
				TempKey.KeyValue = gKeypadQueueBuffer[wCount+1].KeyValue;
				gKeypadQueueBuffer[wCount].ValidKey = TempKey.ValidKey;
				gKeypadQueueBuffer[wCount].KeyChannel = TempKey.KeyChannel;			
				gKeypadQueueBuffer[wCount].KeyValue = TempKey.KeyValue;
				gKeypadQueueBuffer[wCount+1].ValidKey = FALSE;
				gKeypadQueueBuffer[wCount+1].KeyChannel = 0; 		
				gKeypadQueueBuffer[wCount+1].KeyValue = 0;				
			}
		
		}	
	}
	
	return RetValidKey;	
}
#endif

#if ENABLE_SOC_IPC_IR_VIA_QUEUE
//******************************************************************************
// FUNCTION:   SocIPCIRBufferInitial
// USAGE:  Initial Soc IPC IR Key Queue
//
// INPUT:  None
// OUTPUT:	None 
// GLOBALS:
//******************************************************************************
void SocIPCIRBufferInitial(void)
{
	WORD wCount = 0;

	for( wCount = 0; wCount < IR_BUFFER_MAX; wCount++ )
	{
		gIRKeyQueueBuffer[wCount].ValidKey = 0;
		gIRKeyQueueBuffer[wCount].IRKey = 0;
	}
	
#if ENABLE_SOC_IPC_KEYPAD_IR_QUEUE_DEBUG
	for( wCount = 0; wCount < IR_BUFFER_MAX; wCount++ )
	{
		socipc_kbd_ir_queue_msg("\nIR Buffer Index = %d.\n",wCount);	
		socipc_kbd_ir_queue_msg("ValidKey = %d.\n",gIRKeyQueueBuffer[wCount].ValidKey);
		socipc_kbd_ir_queue_msg("IRKey = %d.\n",gIRKeyQueueBuffer[wCount].IRKey);
		socipc_kbd_ir_queue_msg("\n\n",0);
	}	
#endif
}

//******************************************************************************
// FUNCTION:   SocIPCIRBufferInsertKey
// USAGE:  Insert a valid IR value to IR Key Queue
//
// INPUT:  IRValidKey
// OUTPUT:	None 
// GLOBALS:
//******************************************************************************
void SocIPCIRBufferInsertKey(IRRecvFormat_t IRValidKey)
{
	WORD wCount = 0;

	//Only for valid keypad keys	
	if(IRValidKey.ValidKey == FALSE)
		return;

	//Get First Available Index
	for( wCount = 0; wCount < IR_BUFFER_MAX; wCount++ )
	{
		if(gIRKeyQueueBuffer[wCount].ValidKey == FALSE)
			break;
	}

	//Check Buffer is full or not?
	if(wCount == IR_BUFFER_MAX)
	{
		socipc_kbd_ir_queue_msg("\nIR Key Buffer is Full!!\n",0);
		return;
	}

	//Insert new vaild key to buffer
	gIRKeyQueueBuffer[wCount].ValidKey = IRValidKey.ValidKey;
	gIRKeyQueueBuffer[wCount].IRKey = IRValidKey.IRKey;
	
}

//******************************************************************************
// FUNCTION:   SocIPCIsIRBufferExistValidKey
// USAGE:  Check IR Key Queue have valid keys or not
//
// INPUT:  None
// OUTPUT:	TRUE or FALSE 
// GLOBALS:
//******************************************************************************
BOOL SocIPCIsIRBufferExistValidKey(void)
{
	WORD wCount = 0;

	for( wCount = 0; wCount < IR_BUFFER_MAX; wCount++ )
	{
		if(gIRKeyQueueBuffer[wCount].ValidKey == TRUE)
			return TRUE; //Buffer exist valid keys
	}
	return FALSE;
}

//******************************************************************************
// FUNCTION:   SocIPCIRBufferGetValidKey
// USAGE:  GET a valid IR key from IR Key Queue
//
// INPUT:  None
// OUTPUT:	RetValidKey
// GLOBALS:
//******************************************************************************
IRRecvFormat_t SocIPCIRBufferGetValidKey(void)
{
	WORD wCount = 0;
	WORD wSortCount = 0;	
	IRRecvFormat_t RetValidKey = {0,0};
	IRRecvFormat_t TempKey = {0,0};

	//Get First Valid Key Index
	for( wCount = 0; wCount < IR_BUFFER_MAX; wCount++ )
	{
		if(gIRKeyQueueBuffer[wCount].ValidKey == TRUE)
			break;
	}

	if(wCount == IR_BUFFER_MAX)
	{	
		socipc_kbd_ir_queue_msg("\nIR Key Buffer is Empty!!\n",0);
		return RetValidKey;
	}

	//Get Valid Key
	RetValidKey.ValidKey = gIRKeyQueueBuffer[wCount].ValidKey;
	RetValidKey.IRKey = gIRKeyQueueBuffer[wCount].IRKey;	

	//Clear Valid Key
 	gIRKeyQueueBuffer[wCount].ValidKey = FALSE;
	gIRKeyQueueBuffer[wCount].IRKey = 0;

	//Sort Buffer
	for( wSortCount = 0; wSortCount < (IR_BUFFER_MAX-1); wSortCount++ )
	{
		for( wCount = wSortCount; wCount < (IR_BUFFER_MAX-1); wCount++ )
		{
		
			if(gIRKeyQueueBuffer[wCount].ValidKey == TRUE)
			{
				continue;
			}
			else
			{
				TempKey.ValidKey = gIRKeyQueueBuffer[wCount+1].ValidKey;
				TempKey.IRKey = gIRKeyQueueBuffer[wCount+1].IRKey;			
				gIRKeyQueueBuffer[wCount].ValidKey = TempKey.ValidKey;
				gIRKeyQueueBuffer[wCount].IRKey = TempKey.IRKey;			
				gIRKeyQueueBuffer[wCount+1].ValidKey= FALSE;
				gIRKeyQueueBuffer[wCount+1].IRKey = 0; 		
			}
		
		}	
	}
	
	return RetValidKey;	
}
#endif

//******************************************************************************
// FUNCTION:   SocIPCWriteRegWord
// USAGE:  Write register value to register address
//
// INPUT:  DevID, W_RegAddr, W_RegVal
// OUTPUT:	None
// GLOBALS:
//******************************************************************************
void SocIPCWriteRegWord(BYTE DevID, WORD W_RegAddr, WORD W_RegVal)
{
	BYTE ParaBuf[2] = {0};

	if((DevID != SOCIPC_DEVICEL_DST_ADDRESS) && (DevID != SOCIPC_DEVICEFE_DST_ADDRESS))
		return;

	//Write Register Address
	ParaBuf[0] = (BYTE)((W_RegAddr&0xFF00)>>8);
	ParaBuf[1] = (BYTE)(W_RegAddr&0x00FF);	
	SocIPCSendCmdPacketParser(DevID,SICMD_RW_Slave_Registers_addr_CONTRL,ParaBuf);

	//Write Register Data
	ParaBuf[0] = (BYTE)((W_RegVal&0xFF00)>>8);
	ParaBuf[1] = (BYTE)(W_RegVal&0x00FF);	
	SocIPCSendCmdPacketParser(DevID,SICMD_RW_Slave_Registers_data_CONTRL,ParaBuf);	
}

//******************************************************************************
// FUNCTION:   SocIPCReadRegWord
// USAGE:  Read register value from register address
//
// INPUT:  DevID, W_RegAddr
// OUTPUT:	W_RegVal
// GLOBALS:
//******************************************************************************
WORD SocIPCReadRegWord(BYTE DevID, WORD W_RegAddr)
{
	WORD W_RegVal = 0;
	BYTE ParaBuf[2] = {0};
	gAthena_LEFT_FE_Slave_Parameters.SlaveReadData = 0; //Clear old data

	if((DevID != SOCIPC_DEVICEL_DST_ADDRESS) && (DevID != SOCIPC_DEVICEFE_DST_ADDRESS))
		return 0;

	//Write Register Address
	ParaBuf[0] = (BYTE)((W_RegAddr&0xFF00)>>8);
	ParaBuf[1] = (BYTE)(W_RegAddr&0x00FF);	
	SocIPCSendCmdPacketParser(DevID,SICMD_RW_Slave_Registers_addr_CONTRL,ParaBuf);

	//Write Register Data
	ParaBuf[0] = (BYTE)((W_RegVal&0xFF00)>>8);
	ParaBuf[1] = (BYTE)(W_RegVal&0x00FF);	
	SocIPCSendCmdPacketParser(DevID,SICMD_RO_Slave_Registers_read_data,ParaBuf);	

	W_RegVal = gAthena_LEFT_FE_Slave_Parameters.SlaveReadData;

	return W_RegVal;
}

//******************************************************************************
// FUNCTION:   SocIPCGetSlaveTimingInfo
// USAGE:  Get Athena LEFT or Athena FE Current Timing Information
//
// INPUT:  DevID
// OUTPUT:	TRUE or FALSE
// GLOBALS:
//******************************************************************************
BOOL SocIPCGetSlaveTimingInfo(BYTE DevID)
{
	if((DevID != SOCIPC_DEVICEL_DST_ADDRESS) && (DevID != SOCIPC_DEVICEFE_DST_ADDRESS))
		return FALSE;

	gAthena_LEFT_FE_Slave_Parameters.HTotal = 0;
	gAthena_LEFT_FE_Slave_Parameters.VTotal = 0;				
	gAthena_LEFT_FE_Slave_Parameters.HStart = 0;
	gAthena_LEFT_FE_Slave_Parameters.VStart = 0;
	gAthena_LEFT_FE_Slave_Parameters.HWidth = 0;
	gAthena_LEFT_FE_Slave_Parameters.VLength = 0;
	gAthena_LEFT_FE_Slave_Parameters.VFreq = 0;

	if( SocIPCSendCmdPacketParser(DevID, SICMD_RO_Timing_H_Width_Slave, NULL) == FALSE)
	{
		socipc_msg("\n<Soc IPC Error> Read Slave H Width Failure.\n",0);
		return FALSE;
	}

	if( SocIPCSendCmdPacketParser(DevID, SICMD_RO_Timing_V_Length_Slave, NULL) == FALSE)
	{	
		socipc_msg("\n<Soc IPC Error> Read Slave V Length Failure.\n",0);
		return FALSE;
	}

	if( SocIPCSendCmdPacketParser(DevID, SICMD_RO_Timing_H_Total_Slave, NULL) == FALSE)
	{	
		socipc_msg("\n<Soc IPC Error> Read Slave H Total Failure.\n",0);
		return FALSE;
	}

	if( SocIPCSendCmdPacketParser(DevID, SICMD_RO_Timing_V_Total_Slave, NULL) == FALSE)
	{	
		socipc_msg("\n<Soc IPC Error> Read Slave V Total Failure.\n",0);
		return FALSE;
	}

	if( SocIPCSendCmdPacketParser(DevID, SICMD_RO_Timing_H_Start_Slave, NULL) == FALSE)
	{	
		socipc_msg("\n<Soc IPC Error> Read Slave H Start Failure.\n",0);
		return FALSE;
	}

	if( SocIPCSendCmdPacketParser(DevID, SICMD_RO_Timing_V_Start_Slave, NULL) == FALSE)
	{
		socipc_msg("\n<Soc IPC Error> Read Slave V Start Failure.\n",0);
		return FALSE;
	}

	if( SocIPCSendCmdPacketParser(DevID, SICMD_RO_Timing_V_Freq_Slave, NULL) == FALSE)
	{
		socipc_msg("\n<Soc IPC Error> Read Slave V Start Failure.\n",0);
		return FALSE;
	}

	return TRUE;
}

//******************************************************************************
// FUNCTION:   SocIPCGetSlaveModeInfo
// USAGE:  Get Athena LEFT or Athena FE Current Mode Information
//
// INPUT:  DevID
// OUTPUT:	TRUE or FALSE
// GLOBALS:
//******************************************************************************
BOOL SocIPCGetSlaveModeInfo(BYTE DevID)
{
	if((DevID != SOCIPC_DEVICEL_DST_ADDRESS) && (DevID != SOCIPC_DEVICEFE_DST_ADDRESS))
		return FALSE;

	gAthena_LEFT_FE_Slave_Parameters.ModeFlags = 0;

	if( SocIPCSendCmdPacketParser(DevID, SICMD_RW_Mode_Info_STATUS, NULL) == FALSE)
	{
		socipc_msg("\n<Soc IPC Error> Read Slave Mode Info Failure.\n",0);
		return FALSE;
	}

	return TRUE;
}

//******************************************************************************
// FUNCTION:   SocIPCGetSlavePortStatus
// USAGE:  Get Athena LEFT or Athena FE Current Port Status
//
// INPUT:  DevID
// OUTPUT:	TRUE or FALSE
// GLOBALS:
//******************************************************************************
BOOL SocIPCGetSlavePortStatus(BYTE DevID)
{
	if((DevID != SOCIPC_DEVICEL_DST_ADDRESS) && (DevID != SOCIPC_DEVICEFE_DST_ADDRESS))
		return FALSE;
	
	gAthena_LEFT_FE_Slave_Parameters.PortStatus = 0;	

	if( SocIPCSendCmdPacketParser(DevID, SICMD_RO_Port_Status, NULL) == FALSE)
	{	
		socipc_msg("\n<Soc IPC Error> Read Slave Port Status Failure.\n",0);
		return FALSE;
	}

	return TRUE;
}

//******************************************************************************
// FUNCTION : SocIPCGetDcrResult
// USAGE    : Get DCR result from Athena LEFT
// INPUT    : DevID
// OUTPUT   : TRUE or FALSE
// GLOBALS  :
//******************************************************************************
BOOL SocIPCGetDcrResult(BYTE DevID)
{
   if ((DevID != SOCIPC_DEVICEL_DST_ADDRESS) && (DevID != SOCIPC_DEVICEFE_DST_ADDRESS))
      return FALSE;

   gAthena_LEFT_FE_Slave_Parameters.DcrResult = INVALID_RESULT;

   if (SocIPCSendCmdPacketParser(DevID, SICMD_RO_DCR_Result, NULL) == FALSE)
   {
      socipc_msg("\n<Soc IPC Error> Read DCR Result Failure.\n",0);
      return FALSE;
   }

   return TRUE;
}

//******************************************************************************
// FUNCTION:   SocIPCSendMasterCableStatus
// USAGE:  Send Athena RIGHT current cable status to Athena LEFT or Athena FE
//
// INPUT:  DevID
// OUTPUT:	TRUE or FALSE
// GLOBALS:
//******************************************************************************
BOOL SocIPCSendMasterCableStatus(BYTE DevID)
{
	if((DevID != SOCIPC_DEVICEL_DST_ADDRESS) && (DevID != SOCIPC_DEVICEFE_DST_ADDRESS))
		return FALSE;	

	if( SocIPCSendCmdPacketParser(DevID, SICMD_RW_Cables_Status, NULL) == FALSE)
	{	
		socipc_msg("\n<Soc IPC Error> Send Master Cable Status Failure.\n",0);
		return FALSE;
	}

	return TRUE;
}

//******************************************************************************
// FUNCTION:   SocIPCSendMasterTimingInfo
// USAGE:  Send Athena RIGHT current timing information to Athena LEFT or Athena FE
//
// INPUT:  DevID
// OUTPUT:	TRUE or FALSE
// GLOBALS:
//******************************************************************************
BOOL SocIPCSendMasterTimingInfo(BYTE DevID)
{
	if((DevID != SOCIPC_DEVICEL_DST_ADDRESS) && (DevID != SOCIPC_DEVICEFE_DST_ADDRESS))
		return FALSE;

	if( SocIPCSendCmdPacketParser(DevID, SICMD_RW_Timing_H_Width, NULL) == FALSE)
	{	
		socipc_msg("\n<Soc IPC Error> Send Master H Width Failure.\n",0);
		return FALSE;
	}

	if( SocIPCSendCmdPacketParser(DevID, SICMD_RW_Timing_V_Length, NULL) == FALSE)
	{	
		socipc_msg("\n<Soc IPC Error> Send Master V Length Failure.\n",0);
		return FALSE;
	}

	if( SocIPCSendCmdPacketParser(DevID, SICMD_RW_Timing_H_Total, NULL) == FALSE)
	{	
		socipc_msg("\n<Soc IPC Error> Send Master H Total Failure.\n",0);
		return FALSE;
	}

	if( SocIPCSendCmdPacketParser(DevID, SICMD_RW_Timing_V_Total, NULL) == FALSE)
	{	
		socipc_msg("\n<Soc IPC Error> Send Master V Total Failure.\n",0);
		return FALSE;
	}

	if( SocIPCSendCmdPacketParser(DevID, SICMD_RW_Timing_H_Start, NULL) == FALSE)
	{	
		socipc_msg("\n<Soc IPC Error> Send Master H Start Failure.\n",0);
		return FALSE;
	}

	if( SocIPCSendCmdPacketParser(DevID, SICMD_RW_Timing_V_Start, NULL) == FALSE)
	{	
		socipc_msg("\n<Soc IPC Error> Send Master V Start Failure.\n",0);
		return FALSE;
	}

	return TRUE;
}

//******************************************************************************
// FUNCTION:   SocIPCSendMasterModeInfo
// USAGE:  Send Athena RIGHT current mode information to Athena LEFT or Athena FE
//
// INPUT:  DevID
// OUTPUT:	TRUE or FALSE
// GLOBALS:
//******************************************************************************
BOOL SocIPCSendMasterModeInfo(BYTE DevID)
{
	if((DevID != SOCIPC_DEVICEL_DST_ADDRESS) && (DevID != SOCIPC_DEVICEFE_DST_ADDRESS))
		return FALSE;

	if( SocIPCSendCmdPacketParser(DevID, SICMD_RW_Mode_Info_CONTRL, NULL) == FALSE)
	{	
		socipc_msg("\n<Soc IPC Error> Send Master Mode Info Failure.\n",0);
		return FALSE;
	}

	return TRUE;
}

//******************************************************************************
// FUNCTION:   SocIPCSendMasterPortStatus
// USAGE:  Send Athena RIGHT current port status to Athena LEFT or Athena FE
//
// INPUT:  DevID
// OUTPUT:	TRUE or FALSE
// GLOBALS:
//******************************************************************************
BOOL SocIPCSendMasterPortStatus(BYTE DevID)
{
	if((DevID != SOCIPC_DEVICEL_DST_ADDRESS) && (DevID != SOCIPC_DEVICEFE_DST_ADDRESS))
		return FALSE;
	
	if( SocIPCSendCmdPacketParser(DevID, SICMD_RW_Port_CONTRL, NULL) == FALSE)
	{
		socipc_msg("\n<Soc IPC Error> Send Master Port Status Failure.\n",0);
		return FALSE;
	}

	return TRUE;
}

#if ENABLE_GET_I2C_READ_BYTE_VIA_SOCIPC_RECV_PACKET	
//******************************************************************************
// FUNCTION:   SocIPCAutoLengthRead2WireBlock
// USAGE:  Get I2C Read Byte Length from SocIPC Recv Length and Execute I2C Burst Read
//
// INPUT:  B_DevAddr,*Bp_Buffer,B_Stop 
// OUTPUT:	gmd_FAIL or gmd_PASS
// GLOBALS:
//******************************************************************************
gmt_RET_PASSFAIL SocIPCAutoLengthRead2WireBlock( BYTE B_DevAddr, BYTE *Bp_Buffer, gmt_2WIRE_STOP B_Stop )
{
	gmt_RET_PASSFAIL result;
	BYTE B_Length = 0;

	// Validate input parameters
	if (Bp_Buffer == NULL)	return gmd_FAIL;
	
	
	// Regular FW (byte by byte) access to I2C Master
	result = gm_2WireStart(B_DevAddr, gmd_2WIRE_RD);

	//Get Data Length via Recv Packet
	if(result == gmd_PASS)
	{
		result = gm_2WireGetByte(I2C_RD, Bp_Buffer++); //Get First Byte for Device ID

		if(result == gmd_PASS)
		{
			result = gm_2WireGetByte(I2C_RD, &B_Length);
			*Bp_Buffer = B_Length;
			B_Length = (B_Length&0x7F)+1; //+1 means checksum byte
			Bp_Buffer++;
			socipc_msg("\n<Soc IPC Message> Recv Length = %d.\n",B_Length);
//       gm_Print("\n<Soc IPC Message> Recv Length = %d.\n",B_Length);
		}
	}
	
	//If the ACK is Rxd,Start Address & Data Transmission
	while ((result == gmd_PASS) && (B_Length--))
	{
		result = gm_2WireGetByte(((B_Length == 0) ? (I2C_ACK | I2C_RD) : (I2C_RD)),Bp_Buffer++);
	}

	if(B_Stop == STOP_BIT)
		gm_2WireStop();

	return result;

}
#endif //ENABLE_GET_I2C_READ_BYTE_VIA_SOCIPC_RECV_PACKET



//******************************************************************************
// FUNCTION:   SocIPC_SlaveSetDDCCIActionHandler
// USAGE:  Process DDC/CI Setting Commands from Athena RIGHT 
//
// INPUT:  DDC_Cmd, W_VCPValue
// OUTPUT:	TURE or FALSE
// GLOBALS:
//******************************************************************************
BOOL SocIPC_SlaveSetDDCCIActionHandler(BYTE DDC_Cmd , WORD W_VCPValue )
{
	BOOL IsSupportedFeature = FALSE;

	switch( DDC_Cmd )
	{
		case DDC2B_CMD_VCP_ALLReset:
			{
				BYTE B_Channel;
				IsSupportedFeature = TRUE;						
				socipc_ddc_proc_msg( "\nFactory Reset.\n", 0);

   				//FactoryReset();
   				UserPrefImageScheme = FactoryDefaultInROM.ImageScheme;
   				UserPrefBrightness = SchemeFactoryDefaultInROM[UserPrefImageScheme].Brightness;
  				UserPrefContrast = SchemeFactoryDefaultInROM[UserPrefImageScheme].Contrast;
   				UserPrefUserRedGain = SchemeFactoryDefaultInROM[UserPrefImageScheme].UserRedGain;
   				UserPrefUserGreenGain = SchemeFactoryDefaultInROM[UserPrefImageScheme].UserGreenGain;
   				UserPrefUserBlueGain = SchemeFactoryDefaultInROM[UserPrefImageScheme].UserBlueGain;
   				UserPrefUserRedOff = SchemeFactoryDefaultInROM[UserPrefImageScheme].UserRedOff;
   				UserPrefUserGreenOff = SchemeFactoryDefaultInROM[UserPrefImageScheme].UserGreenOff;
   				UserPrefUserBlueOff = SchemeFactoryDefaultInROM[UserPrefImageScheme].UserBlueOff;
   				AdjustBrightness();
   				AdjustContrast();
   				AdjustRedGain();
   				AdjustBlueGain();
   				AdjustGreenGain();
   				AdjustRedOff();
   				AdjustBlueOff();
   				AdjustGreenOff();

		#if FEATURE_AIP == ENABLE
   				if (gm_IsInputTripleADC(gmvb_MainChannel))
	  				B_Channel = gmvb_MainChannel;
   				else if (gm_IsInputTripleADC(gmvb_PipChannel))
	  				B_Channel = gmvb_PipChannel;
   				else
	  				break;
   
   				UserPrefHTotal = gmvw_InputHTotalOrg[B_Channel];
   				UserPrefHStart = gmvw_InputHStartOrg[B_Channel];
   				UserPrefHSyncDelay = gmvsw_InputHSyncOffset[B_Channel];
   				UserPrefVStart = gmvw_InputVStartOrg[B_Channel];
   				gmvsw_InputHSyncOffset[B_Channel] = 0;
   				gmvsw_InputVSyncOffset[B_Channel] = 0;
   				UserPrefVSyncDelay = gmvsw_InputVSyncOffset[B_Channel];
   				UserPrefHSyncPhase = gmvb_InputSamplingPhase[B_Channel];
   
   				gm_InputSetHPosition(B_Channel);
   				gm_InputSetVPosition(B_Channel);
   				SetupHTotal(UserPrefHTotal);
   				gm_AutoGeometry(B_Channel, gmd_DEFAULT_AUTO);
		#endif

		#ifdef USE_COLOR_FIDELITY
 		#if USE_DYNAMIC_GAMUTMAPPING_SUPPORT
   				DisableColorProcessing();
 		#endif
		#endif // USE_COLOR_FIDELITY
			}
			break;
			
#ifdef ADJUSTER_BRIGHTNESS_DECLARED			
		case DDC2B_CMD_VCP_RestoreContrastBrightness:
				IsSupportedFeature = TRUE;							
				socipc_ddc_proc_msg( "\nDDC2B_CMD_VCP_RestoreContrastBrightness.\n", 0);			
				UserPrefBrightness = SchemeFactoryDefaultInROM[UserPrefImageScheme].Brightness;
				UserPrefContrast = SchemeFactoryDefaultInROM[UserPrefImageScheme].Contrast;
				AdjustBrightness();
				AdjustContrast();
			break;
#endif

#ifdef ADJUSTER_USERREDGAIN_DECLARED
		case DDC2B_CMD_VCP_ColorReset:
				IsSupportedFeature = TRUE;							
				socipc_ddc_proc_msg( "\nDDC2B_CMD_VCP_ColorReset.\n", 0);			
				UserPrefUserRedGain = SchemeFactoryDefaultInROM[UserPrefImageScheme].UserRedGain;
				UserPrefUserGreenGain = SchemeFactoryDefaultInROM[UserPrefImageScheme].UserGreenGain;
				UserPrefUserBlueGain = SchemeFactoryDefaultInROM[UserPrefImageScheme].UserBlueGain;
				UserPrefUserRedOff = SchemeFactoryDefaultInROM[UserPrefImageScheme].UserRedOff;
				UserPrefUserGreenOff = SchemeFactoryDefaultInROM[UserPrefImageScheme].UserGreenOff;
				UserPrefUserBlueOff = SchemeFactoryDefaultInROM[UserPrefImageScheme].UserBlueOff;
				AdjustRedGain();
				AdjustBlueGain();
				AdjustGreenGain();
				AdjustRedOff();
				AdjustBlueOff();
				AdjustGreenOff();
			break;
#endif

#ifdef ADJUSTER_BRIGHTNESS_DECLARED
		case DDC2B_CMD_VCP_Brightness:
				IsSupportedFeature = TRUE;				
				UserPrefBrightness = W_VCPValue + (WORD)gm_GetAdjusterMin(ADJV_Brightness);				
				socipc_ddc_proc_msg("\ncalling AdjustBrightness [%d]\n", UserPrefBrightness);		
				AdjustBrightness();
			break;
#endif

#ifdef ADJUSTER_CONTRAST_DECLARED
		case DDC2B_CMD_VCP_Contrast:
				IsSupportedFeature = TRUE;				
				UserPrefContrast = W_VCPValue + (WORD)gm_GetAdjusterMin(ADJV_Contrast);					
				socipc_ddc_proc_msg("\ncalling adjustcontrast [%d]\n", UserPrefContrast);		
				AdjustContrast();
			break;
#endif

#ifdef UserPrefColorTemp
		case DDC2B_CMD_VCP_SelectColorPreset:
				IsSupportedFeature = TRUE;				
				switch (W_VCPValue)
				{
					case DDCCI_CTEMP_6500K:
						UserPrefColorTemp= CTEMP_6500K;
				  		break;
					case DDCCI_CTEMP_7500K:
						UserPrefColorTemp= CTEMP_7500K;
				  		break;
					case DDCCI_CTEMP_9300K:
						UserPrefColorTemp = CTEMP_9300K;
						break;
					//case DDCCI_CTEMP_11500K:
					//	UserPrefColorTemp = CTEMP_11500K;
					//	break;
					case DDCCI_CTEMP_USER1:
						UserPrefColorTemp = USER_TEMP;
						break;
				}				
				socipc_ddc_proc_msg("\ncalling adjustcolor_temp [%d]\n", UserPrefColorTemp);
				AdjustColorTemp();
			break;
#endif

#ifdef Action_HideOsd_WB_Used
		case DDC2B_CMD_VCP_OSD:
				IsSupportedFeature = TRUE;								
				socipc_ddc_proc_msg("\nDDC2B_CMD_VCP_OSD = %d.\n", W_VCPValue);
				
				if (W_VCPValue == 1) // hide osd if it's on
				{
					gm_OsdHide();
				}
				else if (W_VCPValue == 2) //show osd
				{
					gm_OsdShow();
				}
			break;
#endif

#ifdef ADJUSTER_USERREDGAIN_DECLARED
		case DDC2B_CMD_VCP_RedGain:
				IsSupportedFeature = TRUE;				
				UserPrefUserRedGain = W_VCPValue + (WORD)gm_GetAdjusterMin(ADJV_UserRedGain);				
				socipc_ddc_proc_msg("\nDDC2B_CMD_VCP_RedGain = %d\n", UserPrefUserRedGain);				
				AdjustRedGain();
			break;
#endif

#ifdef ADJUSTER_USERGREENGAIN_DECLARED
		case DDC2B_CMD_VCP_GreenGain:
				IsSupportedFeature = TRUE;				
				UserPrefUserGreenGain = W_VCPValue + (WORD)gm_GetAdjusterMin(ADJV_UserGreenGain);					
				socipc_ddc_proc_msg("\nDDC2B_CMD_VCP_GreenGain = %d\n", UserPrefUserGreenGain);			
				AdjustGreenGain();
			break;
#endif

#ifdef ADJUSTER_USERBLUEGAIN_DECLARED
		case DDC2B_CMD_VCP_BlueGain:
				IsSupportedFeature = TRUE;				
				UserPrefUserBlueGain = W_VCPValue + (WORD)gm_GetAdjusterMin(ADJV_UserBlueGain);					
				socipc_ddc_proc_msg("\nDDC2B_CMD_VCP_BlueGain = %d\n", UserPrefUserBlueGain);					
				AdjustBlueGain();
			break;
#endif

#ifdef ADJUSTER_SHARPNESS_DECLARED
		case DDC2B_CMD_VCP_TVSharpness:
				IsSupportedFeature = TRUE;				
				UserPrefSharpness= W_VCPValue;				
				socipc_ddc_proc_msg("\nDDC2B_CMD_VCP_TVSharpness = %d\n", UserPrefSharpness);			
				AdjustSharpness();
			break;
#endif

#ifdef ADJUSTER_SATURATION_DECLARED
		case DDC2B_CMD_VCP_TVSaturation:
				IsSupportedFeature = TRUE;				
				UserPrefSaturation = W_VCPValue;					
				socipc_ddc_proc_msg("\nDDC2B_CMD_VCP_TVSaturation = %d\n", UserPrefSaturation);					
				AdjustSaturation();
			break;
#endif

#ifdef ADJUSTER_OSDLANGUAGE_DECLARED			
		case DDC2B_CMD_VCP_OsdLanguage:
				IsSupportedFeature = TRUE;				
				switch (W_VCPValue)
				{
					case 1://chinese
						UserPrefOsdLanguage = 0;				
						AdjustOsdLanguage();
				  		break;
			   		case 2: //english
				  		UserPrefOsdLanguage = 1;
				  		AdjustOsdLanguage();
				}					
				socipc_ddc_proc_msg("\nDDC2B_CMD_VCP_OsdLanguage = %d\n", UserPrefOsdLanguage);
			break;

		case DDC2B_CMD_VCP_Language:
				IsSupportedFeature = TRUE;				
				if (W_VCPValue)
				{
					UserPrefOsdLanguage = W_VCPValue - 1;
					AdjustOsdLanguage();
				}				
				socipc_ddc_proc_msg("\nDDC2B_CMD_VCP_Language = %d\n", UserPrefOsdLanguage);			
			break;
#endif

#ifdef ADJUSTER_OSDHPOSITION_DECLARED			
		case DDC2B_CMD_VCP_OSDHPosition:
				IsSupportedFeature = TRUE;				
				UserPrefOsdHPosition = W_VCPValue;
				AdjustOsdHPosition();					
				socipc_ddc_proc_msg("\nDDC2B_CMD_VCP_OSDHPosition = %d\n", UserPrefOsdHPosition);					
			break;
#endif

#ifdef ADJUSTER_OSDVPOSITION_DECLARED			
		case DDC2B_CMD_VCP_OSDVPosition:
				IsSupportedFeature = TRUE;				
				UserPrefOsdVPosition = W_VCPValue;
				AdjustOsdVPosition();				
				socipc_ddc_proc_msg("\nDDC2B_CMD_VCP_OSDVPosition = %d\n", UserPrefOsdVPosition);	
			break;
#endif

		default:
			break;
	}

	return IsSupportedFeature;

}

//******************************************************************************
// FUNCTION:   SocIPC_SlaveGetDDCCIStatusHandler
// USAGE:  Reply DDC/CI status to Athena RIGHT 
//
// INPUT:  DDC_Cmd
// OUTPUT:	TURE or FALSE and *GetVcpReply
// GLOBALS:
//******************************************************************************
BOOL SocIPC_SlaveGetDDCCIStatusHandler(BYTE DDC_Cmd , DDCCIStatusResPkt_t *GetVcpReply )
{
	BOOL IsSupportedFeature = FALSE;
	WORD W_VCPValue = 0;
	WORD W_VCPMax = 0xFFFF;
	BYTE B_VCPType = 0x01;

	switch( DDC_Cmd )
	{
		case DDC2B_CMD_VCP_ALLReset:
				IsSupportedFeature = TRUE;
				W_VCPMax = 1;
				B_VCPType = 1;
				GetVcpReply->DDC_Value_HighByte = (BYTE)((W_VCPValue&0xFF00)>>8);
				GetVcpReply->DDC_Value_LowByte = (BYTE)(W_VCPValue&0x00FF);				
				GetVcpReply->DDC_VCP_Max_HighByte = (BYTE)((W_VCPMax&0xFF00)>>8);
				GetVcpReply->DDC_VCP_Max_LowByte = (BYTE)(W_VCPMax&0x00FF);
				GetVcpReply->DDC_VCP_Type = B_VCPType;
				socipc_ddc_proc_msg( "\nVCP Reply - DDC2B_CMD_VCP_ALLReset.\n", 0);	
			break;
			
#ifdef ADJUSTER_BRIGHTNESS_DECLARED			
		case DDC2B_CMD_VCP_RestoreContrastBrightness:
				IsSupportedFeature = TRUE;
				W_VCPMax = 1; 
				GetVcpReply->DDC_Value_HighByte = (BYTE)((W_VCPValue&0xFF00)>>8);
				GetVcpReply->DDC_Value_LowByte = (BYTE)(W_VCPValue&0x00FF);				
				GetVcpReply->DDC_VCP_Max_HighByte = (BYTE)((W_VCPMax&0xFF00)>>8);
				GetVcpReply->DDC_VCP_Max_LowByte = (BYTE)(W_VCPMax&0x00FF);
				GetVcpReply->DDC_VCP_Type = B_VCPType;
				socipc_ddc_proc_msg( "\nVCP Reply - DDC2B_CMD_VCP_RestoreContrastBrightness.\n", 0);			
			break;
#endif

#ifdef ADJUSTER_USERREDGAIN_DECLARED
		case DDC2B_CMD_VCP_ColorReset:
				IsSupportedFeature = TRUE;
				W_VCPMax = 1;
				GetVcpReply->DDC_Value_HighByte = (BYTE)((W_VCPValue&0xFF00)>>8);
				GetVcpReply->DDC_Value_LowByte = (BYTE)(W_VCPValue&0x00FF);				
				GetVcpReply->DDC_VCP_Max_HighByte = (BYTE)((W_VCPMax&0xFF00)>>8);
				GetVcpReply->DDC_VCP_Max_LowByte = (BYTE)(W_VCPMax&0x00FF);
				GetVcpReply->DDC_VCP_Type = B_VCPType;				
				socipc_ddc_proc_msg( "\nVCP Reply - DDC2B_CMD_VCP_ColorReset.\n", 0);			
			break;
#endif

#ifdef ADJUSTER_BRIGHTNESS_DECLARED
		case DDC2B_CMD_VCP_Brightness:
				IsSupportedFeature = TRUE;
				W_VCPValue = UserPrefBrightness - (WORD)gm_GetAdjusterMin(ADJV_Brightness);
				W_VCPMax = (WORD)gm_GetAdjusterMax(ADJV_Brightness) - (WORD)gm_GetAdjusterMin(ADJV_Brightness);
				B_VCPType = 1;
				GetVcpReply->DDC_Value_HighByte = (BYTE)((W_VCPValue&0xFF00)>>8);
				GetVcpReply->DDC_Value_LowByte = (BYTE)(W_VCPValue&0x00FF);				
				GetVcpReply->DDC_VCP_Max_HighByte = (BYTE)((W_VCPMax&0xFF00)>>8);
				GetVcpReply->DDC_VCP_Max_LowByte = (BYTE)(W_VCPMax&0x00FF);
				GetVcpReply->DDC_VCP_Type = B_VCPType;				
				socipc_ddc_proc_msg( "\nVCP Reply - DDC2B_CMD_VCP_Brightness.\n", 0);
			break;
#endif

#ifdef ADJUSTER_CONTRAST_DECLARED
		case DDC2B_CMD_VCP_Contrast:
				IsSupportedFeature = TRUE;
				W_VCPValue = UserPrefContrast - (WORD)gm_GetAdjusterMin(ADJV_Contrast);
				W_VCPMax = (WORD)gm_GetAdjusterMax(ADJV_Contrast) - (WORD)gm_GetAdjusterMin(ADJV_Contrast);
				B_VCPType = 0;
				GetVcpReply->DDC_Value_HighByte = (BYTE)((W_VCPValue&0xFF00)>>8);
				GetVcpReply->DDC_Value_LowByte = (BYTE)(W_VCPValue&0x00FF);				
				GetVcpReply->DDC_VCP_Max_HighByte = (BYTE)((W_VCPMax&0xFF00)>>8);
				GetVcpReply->DDC_VCP_Max_LowByte = (BYTE)(W_VCPMax&0x00FF);
				GetVcpReply->DDC_VCP_Type = B_VCPType;				
				socipc_ddc_proc_msg( "\nVCP Reply - DDC2B_CMD_VCP_Contrast.\n", 0);		
			break;
#endif

#ifdef UserPrefColorTemp
		case DDC2B_CMD_VCP_SelectColorPreset:
				IsSupportedFeature = TRUE;

				switch (UserPrefColorTemp)
				{
					case CTEMP_6500K:
						W_VCPValue = DDCCI_CTEMP_6500K;
						break;
					case CTEMP_7500K:
						W_VCPValue = DDCCI_CTEMP_7500K;
						break;
					case CTEMP_9300K:
						W_VCPValue = DDCCI_CTEMP_9300K;
						break;
					//case CTEMP_11500K:
					//	W_VCPValue = DDCCI_CTEMP_11500K;
					//	break;
					/*case USER_TEMP :
						W_VCPValue = DDCCI_CTEMP_USER1;
						UserPrefColorTemp
						break;*/
					//The below default task was added to pass Windows DTM
					//Without it, DTM tries to set UserPrefColorTemp to 0
					//As well the above case was commented out to pass DTM
					//This is a known Microsoft bug
					default	:
						W_VCPValue = DDCCI_CTEMP_6500K;
						break;				
				}
					B_VCPType    = 0x01;	  //Set Parameter
#if 1
					W_VCPMax = 0xFFFF;	//this is the number of possible choices for this non-continuous VCP value
#else
					W_VCPMax = 3;    //this is the number of possible choices for this non-continuous VCP value
#endif
				GetVcpReply->DDC_Value_HighByte = (BYTE)((W_VCPValue&0xFF00)>>8);
				GetVcpReply->DDC_Value_LowByte = (BYTE)(W_VCPValue&0x00FF);				
				GetVcpReply->DDC_VCP_Max_HighByte = (BYTE)((W_VCPMax&0xFF00)>>8);
				GetVcpReply->DDC_VCP_Max_LowByte = (BYTE)(W_VCPMax&0x00FF);
				GetVcpReply->DDC_VCP_Type = B_VCPType;				
				socipc_ddc_proc_msg( "\nVCP Reply - DDC2B_CMD_VCP_SelectColorPreset.\n", 0);	
			break;
#endif

#ifdef Action_HideOsd_WB_Used
		case DDC2B_CMD_VCP_OSD:
				IsSupportedFeature = TRUE;
				B_VCPType = 0;
				W_VCPMax = 2;
				GetVcpReply->DDC_Value_HighByte = (BYTE)((W_VCPValue&0xFF00)>>8);
				GetVcpReply->DDC_Value_LowByte = (BYTE)(W_VCPValue&0x00FF);				
				GetVcpReply->DDC_VCP_Max_HighByte = (BYTE)((W_VCPMax&0xFF00)>>8);
				GetVcpReply->DDC_VCP_Max_LowByte = (BYTE)(W_VCPMax&0x00FF);
				GetVcpReply->DDC_VCP_Type = B_VCPType;				
				socipc_ddc_proc_msg( "\nVCP Reply - DDC2B_CMD_VCP_OSD.\n", 0);
			break;
#endif

#ifdef ADJUSTER_USERREDGAIN_DECLARED
		case DDC2B_CMD_VCP_RedGain:
				IsSupportedFeature = TRUE;
				W_VCPValue = UserPrefUserRedGain - (WORD)gm_GetAdjusterMin(ADJV_UserRedGain);
				W_VCPMax = (WORD)gm_GetAdjusterMax(ADJV_UserRedGain) - (WORD)gm_GetAdjusterMin(ADJV_UserRedGain);
				B_VCPType = 0;
				GetVcpReply->DDC_Value_HighByte = (BYTE)((W_VCPValue&0xFF00)>>8);
				GetVcpReply->DDC_Value_LowByte = (BYTE)(W_VCPValue&0x00FF);				
				GetVcpReply->DDC_VCP_Max_HighByte = (BYTE)((W_VCPMax&0xFF00)>>8);
				GetVcpReply->DDC_VCP_Max_LowByte = (BYTE)(W_VCPMax&0x00FF);
				GetVcpReply->DDC_VCP_Type = B_VCPType;				
				socipc_ddc_proc_msg( "\nVCP Reply - DDC2B_CMD_VCP_RedGain.\n", 0);				
			break;
#endif

#ifdef ADJUSTER_USERGREENGAIN_DECLARED
		case DDC2B_CMD_VCP_GreenGain:
				IsSupportedFeature = TRUE;
				W_VCPValue = UserPrefUserGreenGain - (WORD)gm_GetAdjusterMin(ADJV_UserGreenGain);
				W_VCPMax = (WORD)gm_GetAdjusterMax(ADJV_UserGreenGain) - (WORD)gm_GetAdjusterMin(ADJV_UserGreenGain);
				B_VCPType = 0;
				GetVcpReply->DDC_Value_HighByte = (BYTE)((W_VCPValue&0xFF00)>>8);
				GetVcpReply->DDC_Value_LowByte = (BYTE)(W_VCPValue&0x00FF);				
				GetVcpReply->DDC_VCP_Max_HighByte = (BYTE)((W_VCPMax&0xFF00)>>8);
				GetVcpReply->DDC_VCP_Max_LowByte = (BYTE)(W_VCPMax&0x00FF);
				GetVcpReply->DDC_VCP_Type = B_VCPType;				
				socipc_ddc_proc_msg( "\nVCP Reply - DDC2B_CMD_VCP_GreenGain.\n", 0);			
			break;
#endif

#ifdef ADJUSTER_USERBLUEGAIN_DECLARED
		case DDC2B_CMD_VCP_BlueGain:
				IsSupportedFeature = TRUE;
				W_VCPValue = UserPrefUserBlueGain - (WORD)gm_GetAdjusterMin(ADJV_UserBlueGain);
				W_VCPMax = (WORD)gm_GetAdjusterMax(ADJV_UserBlueGain) - (WORD)gm_GetAdjusterMin(ADJV_UserBlueGain);
				B_VCPType = 0;
				GetVcpReply->DDC_Value_HighByte = (BYTE)((W_VCPValue&0xFF00)>>8);
				GetVcpReply->DDC_Value_LowByte = (BYTE)(W_VCPValue&0x00FF);				
				GetVcpReply->DDC_VCP_Max_HighByte = (BYTE)((W_VCPMax&0xFF00)>>8);
				GetVcpReply->DDC_VCP_Max_LowByte = (BYTE)(W_VCPMax&0x00FF);
				GetVcpReply->DDC_VCP_Type = B_VCPType;				
				socipc_ddc_proc_msg( "\nVCP Reply - DDC2B_CMD_VCP_BlueGain.\n", 0);
			break;
#endif

#ifdef ADJUSTER_SHARPNESS_DECLARED
		case DDC2B_CMD_VCP_TVSharpness:
				IsSupportedFeature = TRUE;
				W_VCPValue = UserPrefSharpness;
				GetVcpReply->DDC_Value_HighByte = (BYTE)((W_VCPValue&0xFF00)>>8);
				GetVcpReply->DDC_Value_LowByte = (BYTE)(W_VCPValue&0x00FF);				
				GetVcpReply->DDC_VCP_Max_HighByte = (BYTE)((W_VCPMax&0xFF00)>>8);
				GetVcpReply->DDC_VCP_Max_LowByte = (BYTE)(W_VCPMax&0x00FF);
				GetVcpReply->DDC_VCP_Type = B_VCPType;				
				socipc_ddc_proc_msg( "\nVCP Reply - DDC2B_CMD_VCP_TVSharpness.\n", 0);		
			break;
#endif

#ifdef ADJUSTER_SATURATION_DECLARED
		case DDC2B_CMD_VCP_TVSaturation:
				IsSupportedFeature = TRUE;
				W_VCPValue = UserPrefSaturation;
				GetVcpReply->DDC_Value_HighByte = (BYTE)((W_VCPValue&0xFF00)>>8);
				GetVcpReply->DDC_Value_LowByte = (BYTE)(W_VCPValue&0x00FF);				
				GetVcpReply->DDC_VCP_Max_HighByte = (BYTE)((W_VCPMax&0xFF00)>>8);
				GetVcpReply->DDC_VCP_Max_LowByte = (BYTE)(W_VCPMax&0x00FF);
				GetVcpReply->DDC_VCP_Type = B_VCPType;				
				socipc_ddc_proc_msg( "\nVCP Reply - DDC2B_CMD_VCP_TVSaturation.\n", 0);	
			break;
#endif

#ifdef ADJUSTER_OSDLANGUAGE_DECLARED			
		case DDC2B_CMD_VCP_OsdLanguage:
				IsSupportedFeature = TRUE;
				
				if (UserPrefOsdLanguage == 1)
					W_VCPValue = 1;
				else if (UserPrefOsdLanguage == 0)
					W_VCPValue = 2;
				else 
					W_VCPValue = 0;
				
				W_VCPMax = 11;
				GetVcpReply->DDC_Value_HighByte = (BYTE)((W_VCPValue&0xFF00)>>8);
				GetVcpReply->DDC_Value_LowByte = (BYTE)(W_VCPValue&0x00FF);				
				GetVcpReply->DDC_VCP_Max_HighByte = (BYTE)((W_VCPMax&0xFF00)>>8);
				GetVcpReply->DDC_VCP_Max_LowByte = (BYTE)(W_VCPMax&0x00FF);
				GetVcpReply->DDC_VCP_Type = B_VCPType;				
				socipc_ddc_proc_msg( "\nVCP Reply - DDC2B_CMD_VCP_OsdLanguage.\n", 0);	
			break;

		case DDC2B_CMD_VCP_Language:
				IsSupportedFeature = TRUE;
				W_VCPValue = UserPrefOsdLanguage + 1;
				W_VCPMax = 2;
				GetVcpReply->DDC_Value_HighByte = (BYTE)((W_VCPValue&0xFF00)>>8);
				GetVcpReply->DDC_Value_LowByte = (BYTE)(W_VCPValue&0x00FF);				
				GetVcpReply->DDC_VCP_Max_HighByte = (BYTE)((W_VCPMax&0xFF00)>>8);
				GetVcpReply->DDC_VCP_Max_LowByte = (BYTE)(W_VCPMax&0x00FF);
				GetVcpReply->DDC_VCP_Type = B_VCPType;				
				socipc_ddc_proc_msg( "\nVCP Reply - DDC2B_CMD_VCP_Language.\n", 0);	
			break;
#endif

#ifdef ADJUSTER_OSDHPOSITION_DECLARED			
		case DDC2B_CMD_VCP_OSDHPosition:
				IsSupportedFeature = TRUE;
				W_VCPValue= UserPrefOsdHPosition;
				GetVcpReply->DDC_Value_HighByte = (BYTE)((W_VCPValue&0xFF00)>>8);
				GetVcpReply->DDC_Value_LowByte = (BYTE)(W_VCPValue&0x00FF);				
				GetVcpReply->DDC_VCP_Max_HighByte = (BYTE)((W_VCPMax&0xFF00)>>8);
				GetVcpReply->DDC_VCP_Max_LowByte = (BYTE)(W_VCPMax&0x00FF);
				GetVcpReply->DDC_VCP_Type = B_VCPType;				
				socipc_ddc_proc_msg( "\nVCP Reply - DDC2B_CMD_VCP_OSDHPosition.\n", 0);	
			break;
#endif

#ifdef ADJUSTER_OSDVPOSITION_DECLARED			
		case DDC2B_CMD_VCP_OSDVPosition:
				IsSupportedFeature = TRUE;
				W_VCPValue= UserPrefOsdVPosition;
				GetVcpReply->DDC_Value_HighByte = (BYTE)((W_VCPValue&0xFF00)>>8);
				GetVcpReply->DDC_Value_LowByte = (BYTE)(W_VCPValue&0x00FF);				
				GetVcpReply->DDC_VCP_Max_HighByte = (BYTE)((W_VCPMax&0xFF00)>>8);
				GetVcpReply->DDC_VCP_Max_LowByte = (BYTE)(W_VCPMax&0x00FF);
				GetVcpReply->DDC_VCP_Type = B_VCPType;				
				socipc_ddc_proc_msg( "\nVCP Reply - DDC2B_CMD_VCP_OSDVPosition.\n", 0);	
			break;
#endif

		default:
			break;
	}

	return IsSupportedFeature;

}



#if (ENABLE_SOC_IPC_LEFT_RIGHT_SLAVE_INT2_FROM_FE_MASTER) 
//******************************************************************************
// FUNCTION:   SocIPCSyncInterruptViaSlaveFE
// USAGE:  Control Athena FE LPM_GPIO83 to trigger INT_2 interrupt
//
// INPUT:  None
// OUTPUT:	None 
// GLOBALS:
//******************************************************************************
void SocIPCSyncInterruptViaSlaveFE(void)
{
	//ChengLin 20130123: GPIO83 for trigger Athena RIGHT OCM_INT (High Level Trigger)
	
	if(GetChipID_4K2K() == CHIPID_4K2K_FE)
	{
		gm_SetRegBitsWord(GPOUTPUT5, GPIO83_OUT);
		gm_ClearRegBitsWord(GPOUTPUT5, GPIO83_OUT);
	}
}
#endif 
//******************************************************************************
// FUNCTION:   SocIPCAppsTestHandler
// USAGE:  Handle Apps Test Commands for SocIPC function (Base on definitation for Master or Slave SoCs)
//
// INPUT:  None
// OUTPUT: None
// GLOBALS:
//******************************************************************************
void SocIPCAppsTestHandler(void)
{
   if(GetChipID_4K2K()==CHIPID_4K2K_R)
   {
      BYTE SocIPCCmd = (BYTE)GetParam(0);
      BYTE SlaveDevice = (BYTE)GetParam(1); //0: Athena LEFT, 1: Athena FE
      BYTE DeviceID = 0;
      BYTE ParaBuff[10]={0};

      if( SlaveDevice == 0)
         DeviceID = SOCIPC_DEVICEL_DST_ADDRESS;			
      else
         DeviceID = SOCIPC_DEVICEFE_DST_ADDRESS;			

      switch(SocIPCCmd)
      {
         case SIAPPTESTCMD_KEYPAD_TEST:
         {
            BYTE ADCValue = 0;
            BYTE ADCChannel = 0;

            ADCValue = (BYTE)GetParam(2); //ADC Value
            ADCChannel = (BYTE)GetParam(3); //ADC Channel

            //Send ADC Key
            ParaBuff[0] = ADCValue;
            ParaBuff[1] = ADCChannel;				
            if(SocIPCSendCmdPacketParser(DeviceID, SICMD_RW_Keypad_CONTRL, ParaBuff) == FALSE)
               gm_Print("\n<Soc IPC Error> Keypad Control Command Failure .\n",0);

            gm_Delay1ms(50);
            //Send Release Key
            ParaBuff[0] = 0x00;
            ParaBuff[1] = ADCChannel;				
            if(SocIPCSendCmdPacketParser(DeviceID, SICMD_RW_Keypad_CONTRL, ParaBuff) == FALSE)
               gm_Print("\n<Soc IPC Error> Keypad Control Command Failure .\n",0); 
         }
         break;
         case SIAPPTESTCMD_IR_TEST:
         {
            BYTE IRKey = 0;

            IRKey = (BYTE)GetParam(2); //IR Key

            //Send IR Key
            ParaBuff[0] = IRKey;			
            if(SocIPCSendCmdPacketParser(DeviceID, SICMD_RW_IR_CONTROL, ParaBuff) == FALSE)
               gm_Print("\n<Soc IPC Error> IR Control Command Failure .\n",0);

            gm_Delay1ms(50);
            //Send IR Release Key
            ParaBuff[0] = 0x00; 			
            if(SocIPCSendCmdPacketParser(DeviceID, SICMD_RW_IR_CONTROL, ParaBuff) == FALSE)
               gm_Print("\n<Soc IPC Error> IR Control Command Failure .\n",0); 
         }
         break;

      #if ENABLE_SOC_IPC_OSD_SYNC				
         case SIAPPTESTCMD_OSD_SYNC_TEST:
            if(SocIPCMasterSlaveOSDSyncSettings())
               gm_Print("\nAthena RIGHT Sync Athena LEFT/FE OSD Successful.\n",0);
            else
               gm_Print("\nAthena RIGHT Sync Athena LEFT/FE OSD Failure.\n",0);							
            break;
      #endif	
      
         case SIAPPTESTCMD_SEND_CMD_TEST:
         {
            BYTE SICMD = 0;
            BYTE Buffer[2]={0};

            SICMD = (BYTE)GetParam(2);
            Buffer[0] = (BYTE)GetParam(3);
            Buffer[1] = (BYTE)GetParam(4);					

            if(SocIPCSendCmdPacketParser(DeviceID, SICMD, Buffer) == FALSE)
               gm_Print("\n<Soc IPC Error> AppsTest Send SocIPC Command Failure.\n",0);				
         }
         break;
         
         case SIAPPTESTCMD_DDC_TEST:
         {
            BYTE Buffer[3]={0};

            Buffer[0] = (BYTE)GetParam(2); //DDC VCP Code
            Buffer[1] = (BYTE)GetParam(3); //Value High Byte
            Buffer[2] = (BYTE)GetParam(4); //Value Low Byte 				

            if(SocIPCSendCmdPacketParser(DeviceID, SICMD_RW_DDCI_CONTRL, Buffer) == FALSE)
               gm_Print("\n<Soc IPC Error> AppsTest Send SOCIPC DDC Command Failure.\n",0);				
         }
         break;
         
         case SIAPPTESTCMD_DEV_CHK_TEST:	
            if(SocIPCCheckDevice(DeviceID))
               gm_Print("\nSlave Device is available!!\n");
            else
               gm_Print("\nSlave Device is unavailable!!\n");	
         break;

         case SIAPPTESTCMD_KEYPAD_BURST_SEND_CMD_TEST:
         {
            BYTE SocIPCBuffer[2];
            WORD cDeviceBCommandFailCount = 0;
            WORD cDeviceCCommandFailCount = 0;
            WORD cDeviceBReleaseCommandFailCount = 0;	
            WORD cDeviceCReleaseCommandFailCount = 0;
            WORD TestCount = (WORD)GetParam(2);

            gm_Print("\nKeypad SocIPC Burst Send Command Test ----- Test Time = %d.\n",TestCount);


            while(TestCount)
            {
               //Key Pressed
               SocIPCBuffer[0] = 255;
               SocIPCBuffer[1] = 1;					

               if( SocIPCSendCmdPacketParser(DeviceID, SICMD_RW_Keypad_CONTRL, SocIPCBuffer) == FALSE)
               {			
               //Send command again
                  if( SocIPCSendCmdPacketParser(DeviceID, SICMD_RW_Keypad_CONTRL, SocIPCBuffer) == FALSE)
                  {
                     if( DeviceID == SOCIPC_DEVICEL_DST_ADDRESS )						
                        cDeviceBCommandFailCount++;
                     else
                        cDeviceCCommandFailCount++;
                  }											 
               }

               //Key Released
               SocIPCBuffer[0] = 0;
               SocIPCBuffer[1] = 1;					

               if( SocIPCSendCmdPacketParser(DeviceID, SICMD_RW_Keypad_CONTRL, SocIPCBuffer) == FALSE)
               {				
                  //Send command again
                  if( SocIPCSendCmdPacketParser(DeviceID, SICMD_RW_Keypad_CONTRL, SocIPCBuffer) == FALSE)
                  {
                     if( DeviceID == SOCIPC_DEVICEL_DST_ADDRESS )						
                        cDeviceBReleaseCommandFailCount++;
                     else
                        cDeviceCReleaseCommandFailCount++;
                  }											 
               }
               TestCount--;
            }

            gm_Print("\nKeypad SocIPC Burst Send Command Test ----- Test Results.\n",0);				
            if( DeviceID == SOCIPC_DEVICEL_DST_ADDRESS )
            {
               gm_Print("\ncDeviceBCommandFailCount = %d.",cDeviceBCommandFailCount);
               gm_Print("\ncDeviceBReleaseCommandFailCount = %d.",cDeviceBReleaseCommandFailCount);					
            }
            else if( DeviceID == SOCIPC_DEVICEFE_DST_ADDRESS )
            {
               gm_Print("\ncDeviceCCommandFailCount = %d.",cDeviceCCommandFailCount);
               gm_Print("\ncDeviceCReleaseCommandFailCount = %d.",cDeviceCReleaseCommandFailCount);
            }
         }
         break;
         
         case SIAPPTESTCMD_REG_WRITE_TEST:
         {
            WORD WriteRegister = (WORD)GetParam(2);
            WORD WriteRegisterData = (WORD)GetParam(3);

            gm_Print("\nSocIPC WrRegAddr = 0x%X.",WriteRegister);
            gm_Print("\nSocIPC WrRegVal = 0x%X.",WriteRegisterData);					
            SocIPCWriteRegWord(DeviceID, WriteRegister, WriteRegisterData);
            gm_Print("\nSocIPC Write Done.",0);
         }
         break;
         
         case SIAPPTESTCMD_REG_READ_TEST:
         {
            WORD ReadRegister = (WORD)GetParam(2);
            WORD ReadRegisterData = 0;

            gm_Print("\nSocIPC RdRegAddr = 0x%X.",ReadRegister);
            ReadRegisterData = SocIPCReadRegWord(DeviceID, ReadRegister);
            gm_Print("\nSocIPC RdRegVal = 0x%X.",ReadRegisterData);
            gm_Print("\nSocIPC Read Done.",0);					
         }
         break;
         
         case SIAPPTESTCMD_SLAVE_CURRENT_INFO_TEST:
            if(!SocIPCCheckDevice(DeviceID))
            {
               gm_Print("\n<Soc IPC Error> Slave Device is Unavailable.\n");
               break;
            }

            if(DeviceID == SOCIPC_DEVICEL_DST_ADDRESS)
               gm_Print("\n ========= Athena-LEFT Current Infomation ========",0);
            else
               gm_Print("\n ========= Athena- FE  Current Infomation ========",0);


            gm_Print("\n\n Input Timing: ",0);	

            if(SocIPCGetSlaveTimingInfo(DeviceID))
            {
               gm_Print("\n H Start = %d. ",gAthena_LEFT_FE_Slave_Parameters.HStart);			
               gm_Print("\n V Start = %d. ",gAthena_LEFT_FE_Slave_Parameters.VStart);
               gm_Print("\n H Total = %d. ",gAthena_LEFT_FE_Slave_Parameters.HTotal);			
               gm_Print("\n V Total = %d. ",gAthena_LEFT_FE_Slave_Parameters.VTotal);
               gm_Print("\n H Width = %d. ",gAthena_LEFT_FE_Slave_Parameters.HWidth);			
               gm_Print("\n V Length = %d. ",gAthena_LEFT_FE_Slave_Parameters.VLength);
					gm_Print("\n V Freq = %d. ",gAthena_LEFT_FE_Slave_Parameters.VFreq);
               gm_Print("\n ==============================================",0);
            }
            else
            {
               gm_Print("\n Read Slave Timing Information Failure. ",0);
               gm_Print("\n ==============================================",0);
            }

            gm_Print("\n\n CH-A Mode Info: ",0);

            if(SocIPCGetSlaveModeInfo(DeviceID))
            {
               if(gAthena_LEFT_FE_Slave_Parameters.ModeFlags & gmd_MODE_GRAPHICS)
                  gm_Print("\n- Graphics Mode.",0);

               if(gAthena_LEFT_FE_Slave_Parameters.ModeFlags & gmd_MODE_SDVIDEO)
                  gm_Print("\n- S Video Mode.",0);

               if(gAthena_LEFT_FE_Slave_Parameters.ModeFlags & gmd_MODE_HDVIDEO)
                  gm_Print("\n- HD Video Mode.",0);

               if(gAthena_LEFT_FE_Slave_Parameters.ModeFlags & gmd_MODE_INTERLACED)
                  gm_Print("\n- Interlaced Mode.",0);

               if(gAthena_LEFT_FE_Slave_Parameters.ModeFlags & gmd_MODE_NEG_HSYNC)
                  gm_Print("\n- HSync is Negative.",0);

               if(gAthena_LEFT_FE_Slave_Parameters.ModeFlags & gmd_MODE_NEG_VSYNC)
                  gm_Print("-- VSync is Negative.",0);		

               if(gAthena_LEFT_FE_Slave_Parameters.ModeFlags & gmd_MODE_SOG)
                  gm_Print("\n- SOG Sync.",0);

               if(gAthena_LEFT_FE_Slave_Parameters.ModeFlags & gmd_MODE_CSYNC)
                  gm_Print("\n- CSync.",0);

               if(gAthena_LEFT_FE_Slave_Parameters.ModeFlags & gmd_MODE_CAPTURE_CROP)
                  gm_Print("\n- Capture Crop Mode.",0);

               if(gAthena_LEFT_FE_Slave_Parameters.ModeFlags & gmd_MODE_CVT)
                  gm_Print("\n- CVT Mode.",0);

               if(gAthena_LEFT_FE_Slave_Parameters.ModeFlags & gmd_MODE_ESTIMATED)
                  gm_Print("\n- Estimated Mode.",0);

               if(gAthena_LEFT_FE_Slave_Parameters.ModeFlags & gmd_MODE_OUT_RANGE)
                  gm_Print("\n- Mode is Out of Range.",0);

               if(gAthena_LEFT_FE_Slave_Parameters.ModeFlags & gmd_MODE_SUB_SAMPLE)
                  gm_Print("\n- Input is Sub-Samping.",0);

               if(gAthena_LEFT_FE_Slave_Parameters.ModeFlags & gmd_MODE_UNKNOWN)
                  gm_Print("\n- Input is Unknown Mode.",0);

               if(gAthena_LEFT_FE_Slave_Parameters.ModeFlags & gmd_MODE_VALID)
                  gm_Print("\n- Input Mode is Valid and Stable.",0);

               if(gAthena_LEFT_FE_Slave_Parameters.ModeFlags & gmd_MODE_COMPLETE)
                  gm_Print("\n- Input Mode has been Configured.",0);


               if(gAthena_LEFT_FE_Slave_Parameters.ModeFlags & gmd_MODE_EMBEDDED_VIDEO)
                  gm_Print("\n- Mode is a Video Image in a Graphic signal.",0);
                  gm_Print("\n ==============================================",0);
            }
            else
            {
               gm_Print("\n Read Slave Mode Information Failure.",0);			
               gm_Print("\n ==============================================",0);
            }


            gm_Print("\n\n CH-A Port Status: ",0);

            if(SocIPCGetSlavePortStatus(DeviceID))
            {
               if( gAthena_LEFT_FE_Slave_Parameters.PortStatus == ALL_BOARDS_CVBS1)
                  gm_Print("\n- Input is CVBS1 Port.",0);
               else if( gAthena_LEFT_FE_Slave_Parameters.PortStatus == ALL_BOARDS_SVIDEO1)
                  gm_Print("\n- Input is SVIDEO1 Port.",0);
               else if( gAthena_LEFT_FE_Slave_Parameters.PortStatus == ALL_BOARDS_COMP1)
                  gm_Print("\n- Input is COMP1 Port.",0);
               else if( gAthena_LEFT_FE_Slave_Parameters.PortStatus == ALL_BOARDS_VGA1)
                  gm_Print("\n- Input is VGA1 Port.",0);
               else if( gAthena_LEFT_FE_Slave_Parameters.PortStatus == ALL_BOARDS_DVI1)
                  gm_Print("\n- Input is DVI1 Port.",0);
               else if( gAthena_LEFT_FE_Slave_Parameters.PortStatus == ALL_BOARDS_DVI2)
                  gm_Print("\n- Input is DVI2 Port.",0);				
               else if( gAthena_LEFT_FE_Slave_Parameters.PortStatus == ALL_BOARDS_DIP1)
                  gm_Print("\n- Input is DIP1 Port.",0);	
               else if( gAthena_LEFT_FE_Slave_Parameters.PortStatus == ALL_BOARDS_HDMI)
                  gm_Print("\n- Input is HDMI Port.",0);
               else if( gAthena_LEFT_FE_Slave_Parameters.PortStatus == ALL_BOARDS_HDMI2)
                  gm_Print("\n- Input is HDMI2 Port.",0);			
               else if( gAthena_LEFT_FE_Slave_Parameters.PortStatus == ALL_BOARDS_HDMI3)
                  gm_Print("\n- Input is HDMI3 Port.",0);	
               else if( gAthena_LEFT_FE_Slave_Parameters.PortStatus == ALL_BOARDS_DP1)
                  gm_Print("\n- Input is DP1 Port.",0);	
               else if( gAthena_LEFT_FE_Slave_Parameters.PortStatus == ALL_BOARDS_DP2)
                  gm_Print("\n- Input is DP2 Port.",0);
               else if( gAthena_LEFT_FE_Slave_Parameters.PortStatus == ALL_BOARDS_SLAVE1)
                  gm_Print("\n- Input is SLAVE1 Port.",0);
               else
                  gm_Print("\n- Input is Unknown Port.",0);			
               gm_Print("\n ==============================================",0);

            }
            else
            {
               gm_Print("\n Read Slave Port Status Failure.",0);			
               gm_Print("\n ==============================================",0);
            }
         break;

         case SIAPPTESTCMD_MASTER_CURRENT_INFO_TEST:
            if(!SocIPCCheckDevice(DeviceID))
            {
               gm_Print("\n<Soc IPC Error> Slave Device is Unavailable.\n");
               break;
            }

            gm_Print("\n ========= Athena-RIGHT Current Infomation ========",0);

            gm_Print("\n\n Cable Status: ",0);	

            if(SocIPCSendMasterCableStatus(DeviceID))
            {
               if(Athena_RIGHT_CurrentCableStatus & 0x80 )
                  gm_Print("\n VGA_SourcePowerStatus = 1.",0);
               else
                  gm_Print("\n VGA_SourcePowerStatus = 0.",0);

               if(Athena_RIGHT_CurrentCableStatus & 0x40 )
                  gm_Print("\n VGA_CableStatus = 1.",0);
               else
                  gm_Print("\n VGA_CableStatus = 0.",0);

               if(Athena_RIGHT_CurrentCableStatus & 0x20 )
                  gm_Print("\n PHY3_SourcePowerStatus = 1.",0);
               else
                  gm_Print("\n PHY3_SourcePowerStatus = 0.",0);				

               if(Athena_RIGHT_CurrentCableStatus & 0x10 )
                  gm_Print("\n PHY3_CableStatus = 1.",0);
               else
                  gm_Print("\n PHY3_CableStatus = 0.",0);	

               if(Athena_RIGHT_CurrentCableStatus & 0x08 )
                  gm_Print("\n PHY2_SourcePowerStatus = 1.",0);
               else
                  gm_Print("\n PHY2_SourcePowerStatus = 0.",0);

               if(Athena_RIGHT_CurrentCableStatus & 0x04 )
                  gm_Print("\n PHY2_CableStatus = 1.",0);
               else
                  gm_Print("\n PHY2_CableStatus = 0.",0);

               if(Athena_RIGHT_CurrentCableStatus & 0x02 )
                  gm_Print("\n PHY1_SourcePowerStatus = 1.",0);
               else
                  gm_Print("\n PHY1_SourcePowerStatus = 0.",0);

               if(Athena_RIGHT_CurrentCableStatus & 0x01 )
                  gm_Print("\n PHY1_CableStatus = 1.",0);
               else
                  gm_Print("\n PHY1_CableStatus = 0.",0);	

               gm_Print("\n ==============================================",0);
            }
            else
            {
               gm_Print("\n Send Master Cable Status Failure. ",0);
               gm_Print("\n ==============================================",0);
            }


            gm_Print("\n\n Input Timing: ",0);	

            if(SocIPCSendMasterTimingInfo(DeviceID))
            {
               gm_Print("\n H Start = %d. ",stInput[CH_A].Hstart);			
               gm_Print("\n V Start = %d. ",stInput[CH_A].Vstart);
               gm_Print("\n H Total = %d. ",stInput[CH_A].Htotal);			
               gm_Print("\n V Total = %d. ",stInput[CH_A].Vtotal);
               gm_Print("\n H Width = %d. ",stInput[CH_A].Hactive);			
               gm_Print("\n V Length = %d. ",stInput[CH_A].Vactive);
               gm_Print("\n ==============================================",0);
            }
            else
            {
               gm_Print("\n Send Master Timing Information Failure. ",0);
               gm_Print("\n ==============================================",0);
            }

            gm_Print("\n\n CH-A Mode Info: ",0);

            if(SocIPCSendMasterModeInfo(DeviceID))
            {
               if(stInput[CH_A].ModeFlags & gmd_MODE_GRAPHICS)
                  gm_Print("\n- Graphics Mode.",0);

               if(stInput[CH_A].ModeFlags & gmd_MODE_SDVIDEO)
                  gm_Print("\n- S Video Mode.",0);

               if(stInput[CH_A].ModeFlags & gmd_MODE_HDVIDEO)
                  gm_Print("\n- HD Video Mode.",0);

               if(stInput[CH_A].ModeFlags & gmd_MODE_INTERLACED)
                  gm_Print("\n- Interlaced Mode.",0);

               if(stInput[CH_A].ModeFlags & gmd_MODE_NEG_HSYNC)
                  gm_Print("\n- HSync is Negative.",0);

               if(stInput[CH_A].ModeFlags & gmd_MODE_NEG_VSYNC)
                  gm_Print("-- VSync is Negative.",0);		

               if(stInput[CH_A].ModeFlags & gmd_MODE_SOG)
                  gm_Print("\n- SOG Sync.",0);

               if(stInput[CH_A].ModeFlags & gmd_MODE_CSYNC)
                  gm_Print("\n- CSync.",0);

               if(stInput[CH_A].ModeFlags & gmd_MODE_CAPTURE_CROP)
                  gm_Print("\n- Capture Crop Mode.",0);

               if(stInput[CH_A].ModeFlags & gmd_MODE_CVT)
                  gm_Print("\n- CVT Mode.",0);

               if(stInput[CH_A].ModeFlags & gmd_MODE_ESTIMATED)
                  gm_Print("\n- Estimated Mode.",0);

               if(stInput[CH_A].ModeFlags & gmd_MODE_OUT_RANGE)
                  gm_Print("\n- Mode is Out of Range.",0);

               if(stInput[CH_A].ModeFlags & gmd_MODE_SUB_SAMPLE)
                  gm_Print("\n- Input is Sub-Samping.",0);

               if(stInput[CH_A].ModeFlags & gmd_MODE_UNKNOWN)
                  gm_Print("\n- Input is Unknown Mode.",0);

               if(stInput[CH_A].ModeFlags & gmd_MODE_VALID)
                  gm_Print("\n- Input Mode is Valid and Stable.",0);

               if(stInput[CH_A].ModeFlags & gmd_MODE_COMPLETE)
                  gm_Print("\n- Input Mode has been Configured.",0);

               if(stInput[CH_A].ModeFlags & gmd_MODE_EMBEDDED_VIDEO)
                  gm_Print("\n- Mode is a Video Image in a Graphic signal.",0);

               gm_Print("\n ==============================================",0);
            }
            else
            {
               gm_Print("\n Send Master Mode Information Failure.",0);			
               gm_Print("\n ==============================================",0);
            }


            gm_Print("\n\n CH-A Port Status: ",0);

            if(SocIPCSendMasterPortStatus(DeviceID))
            {
               if( gmvb_InputConnector[CH_A] == ALL_BOARDS_CVBS1)
                  gm_Print("\n- Input is CVBS1 Port.",0);
               else if( gmvb_InputConnector[CH_A] == ALL_BOARDS_SVIDEO1)
                  gm_Print("\n- Input is SVIDEO1 Port.",0);
               else if( gmvb_InputConnector[CH_A] == ALL_BOARDS_COMP1)
                  gm_Print("\n- Input is COMP1 Port.",0);
               else if( gmvb_InputConnector[CH_A] == ALL_BOARDS_VGA1)
                  gm_Print("\n- Input is VGA1 Port.",0);
               else if( gmvb_InputConnector[CH_A] == ALL_BOARDS_DVI1)
                  gm_Print("\n- Input is DVI1 Port.",0);
               else if( gmvb_InputConnector[CH_A] == ALL_BOARDS_DVI2)
                  gm_Print("\n- Input is DVI2 Port.",0);				
               else if( gmvb_InputConnector[CH_A] == ALL_BOARDS_DIP1)
                  gm_Print("\n- Input is DIP1 Port.",0);	
               else if( gmvb_InputConnector[CH_A] == ALL_BOARDS_HDMI)
                  gm_Print("\n- Input is HDMI Port.",0);
               else if( gmvb_InputConnector[CH_A] == ALL_BOARDS_HDMI2)
                  gm_Print("\n- Input is HDMI2 Port.",0); 		
               else if( gmvb_InputConnector[CH_A] == ALL_BOARDS_HDMI3)
                  gm_Print("\n- Input is HDMI3 Port.",0); 
               else if( gmvb_InputConnector[CH_A] == ALL_BOARDS_DP1)
                  gm_Print("\n- Input is DP1 Port.",0);	
               else if( gmvb_InputConnector[CH_A] == ALL_BOARDS_DP2)
                  gm_Print("\n- Input is DP2 Port.",0);
               else if( gmvb_InputConnector[CH_A] == ALL_BOARDS_SLAVE1)
                  gm_Print("\n- Input is SLAVE1 Port.",0);
               else
                  gm_Print("\n- Input is Unknown Port.",0);			

               gm_Print("\n ==============================================",0);
            }
            else
            {
               gm_Print("\n Send Master Port Status Failure.",0);			
               gm_Print("\n ==============================================",0);
            }
         break;

         case SIAPPTESTCMD_SET_AND_GET_SHARPNESS_TEST:
         {
            BYTE SetValue = (BYTE)GetParam(2);
            if(SocIPCSetCorrectSharpnessSetting(SetValue))
               gm_Print("\nMaster Current Sharpness = %d.\n",SocIPCGetCorrectSharpnessSetting());

            AdjustSharpness();
            AdjustSharpnessRGB();				
         }			
         break;

         case SIAPPTESTCMD_VEGA_HW_RESET_VIA_RIGHT:
            SOCIPC_ResetVEGAViaAthenaRight();
            gm_Print("\nReset VEGA Successful!!\n",0);
         break;

#if ENABLE_VEGA_ISP_CMD_VIA_UART_ISP_PORT //Add by ChengLin 20130412
         case SIAPPTESTCMD_EN_VEGA_ISP_MODE:
            SOCIPC_VEGA_ISP_Enable();
         break;
#endif
         default:
         break;
      }					
   }
   else //Athena LEFT or Athena FE 
   {
      BYTE SocIPCCmd = (BYTE)GetParam(0);

      switch(SocIPCCmd)
      {
      #if (ENABLE_SOC_IPC_LEFT_RIGHT_SLAVE_INT2_FROM_FE_MASTER) && defined(ST_4K2K_93xx_BOARD)
         case SIAPPTESTCMD_INT2_TRIGGER_TEST: //LPM_GPIO0 Trigger INT2 interrupt
            if(GetChipID_4K2K()==CHIPID_4K2K_FE)
            {
               SocIPCSyncInterruptViaSlaveFE();
               gm_Print("\n<Soc IPC Message> AppsTest Trigger GPIO83.\n",0);
            }
         break;
      #endif				
         case SIAPPTESTCMD_SET_AND_GET_SHARPNESS_TEST:
         {
            BYTE SetValue = (BYTE)GetParam(1);
            if(SocIPCSetCorrectSharpnessSetting(SetValue))
               gm_Print("\nSlave Current Sharpness = %d.\n",SocIPCGetCorrectSharpnessSetting());
            AdjustSharpness();
            AdjustSharpnessRGB();					
         }
         break;

		case SIAPPTESTCMD_VEGA_I2C_WRITE_VIA_FE:
			{			
				BYTE SlaveAddr = 0; 	
				WORD Value = 0;

				if(GetChipID_4K2K()==CHIPID_4K2K_FE)
				{
					SlaveAddr = (BYTE)GetParam(1);
					Value = (WORD)((GetParam(2)<<8)|GetParam(3));
					SOCIPC_VEGA_I2C_WriteWord_Via_FE(SlaveAddr, Value);
					gm_Print("\nVEGA I2C Wrire Slave Address 0x%x. Finished.\n",SlaveAddr);				
				}
			}
		 break;
		case SIAPPTESTCMD_VEGA_I2C_READ_VIA_FE:
			{
				BYTE SlaveAddr = 0;	 
				WORD Value = 0;
				if(GetChipID_4K2K()==CHIPID_4K2K_FE)
				{
					 SlaveAddr = (BYTE)GetParam(1);
					 Value = SOCIPC_VEGA_I2C_ReadWord_Via_FE(SlaveAddr);
					 gm_Print("\nVEGA I2C Read Value = 0x%x.\n",Value);				
				}
			}			
		 break;
		 case SIAPPTESTCMD_VEGA_HW_REG_WRITE_VIA_FE:
			{
				WORD HWAddr = 0;
				BYTE Value = 0;
						 
				HWAddr = (WORD)((GetParam(1)<<8)|GetParam(2));
				Value = (BYTE)GetParam(3);
				SOCIPC_VEGA_WriteHWReg_Via_FE(HWAddr,Value);
				gm_Print("\nVEGA HW Reg Wrire Address 0x%x. Finished.\n",HWAddr);	
			}
			break;
		case SIAPPTESTCMD_VEGA_HW_REG_READ_VIA_FE:
			{
		 		WORD HWAddr = 0;
				BYTE Value = 0;
			   
				HWAddr = (WORD)((GetParam(1)<<8)|GetParam(2));
				Value = SOCIPC_VEGA_ReadHWRegWord_Via_FE(HWAddr);
				gm_Print("\nVEGA HW Reg Read Value = 0x%x.\n",Value);
			}
			break;
         default:
         break;
      }	
   }

}

//******************************************************************************
// FUNCTION:   SocIPCGetCorrectSharpnessSetting
// USAGE:  Get current sharpness value base on color space and active channel
//
// INPUT:  None
// OUTPUT: RtSharpnessVal
// GLOBALS:
//******************************************************************************
BYTE SocIPCGetCorrectSharpnessSetting(void)
{
	BYTE RtSharpnessVal = 0;

#if (FEATURE_LOAD_RGB_COEFF == DISABLE)
	RtSharpnessVal = UserPrefSharpness;
#else
   if (IsSideBySidePipMode(UserPrefPipMode) == TRUE )
   {
      if ( B_FocusBorder == gmvb_MainChannel )
      {
         if ( gm_GetChannelInputAttr(gmvb_MainChannel, gmd_COLOR_SPACE) != gmd_RGB )
         {
			RtSharpnessVal = UserPrefSharpness;
         }
		 else if( gm_GetChannelInputAttr(gmvb_MainChannel, gmd_COLOR_SPACE) == gmd_RGB )
		 {
			RtSharpnessVal = UserPrefSharpnessRGB;			 	
		 }
		 else
		 {
			RtSharpnessVal = 0x00; //Error
		 }
      }
      else
      {
         if ( gm_GetChannelInputAttr(gmvb_PipChannel, gmd_COLOR_SPACE) != gmd_RGB )
         {
			RtSharpnessVal = UserPrefSharpnessPip;
         }
         else if ( gm_GetChannelInputAttr(gmvb_PipChannel, gmd_COLOR_SPACE) == gmd_RGB )
         {
			RtSharpnessVal = UserPrefSharpnessRGBpip;
         }
		 else
		 {
			RtSharpnessVal = 0x00; //Error
		 }
      }
   }
   else
   {
         if ( gm_GetChannelInputAttr(gmvb_MainChannel, gmd_COLOR_SPACE) != gmd_RGB )
         {
			RtSharpnessVal = UserPrefSharpness;
         }
		 else if( gm_GetChannelInputAttr(gmvb_MainChannel, gmd_COLOR_SPACE) == gmd_RGB )
		 {
			RtSharpnessVal = UserPrefSharpnessRGB;			 	
		 }
		 else
		 {
			 RtSharpnessVal = 0x00; //Error
		 } 
   }
#endif

	return RtSharpnessVal;
	
}

//******************************************************************************
// FUNCTION:   SocIPCSetCorrectSharpnessSetting
// USAGE:  Set current sharpness value base on color space and active channel
//
// INPUT:  SharpnessVal
// OUTPUT: TRUE or FALSE
// GLOBALS:
//******************************************************************************
BOOL SocIPCSetCorrectSharpnessSetting(BYTE SharpnessVal)
{
	BYTE IsCorrect = FALSE;

#if (FEATURE_LOAD_RGB_COEFF == DISABLE)
	UserPrefSharpness = SharpnessVal;
	IsCorrect = TRUE;		
#else
   if (IsSideBySidePipMode(UserPrefPipMode) == TRUE )
   {
      if ( B_FocusBorder == gmvb_MainChannel )
      {
         if ( gm_GetChannelInputAttr(gmvb_MainChannel, gmd_COLOR_SPACE) != gmd_RGB )
         {
			UserPrefSharpness = SharpnessVal;
			IsCorrect = TRUE;	
         }
		 else if( gm_GetChannelInputAttr(gmvb_MainChannel, gmd_COLOR_SPACE) == gmd_RGB )
		 {

			UserPrefSharpnessRGB = SharpnessVal;
			IsCorrect = TRUE;	 	 	
		 }
		 else
		 {
			IsCorrect = FALSE;	 
		 }
      }
      else
      {
         if ( gm_GetChannelInputAttr(gmvb_PipChannel, gmd_COLOR_SPACE) != gmd_RGB )
         {
			UserPrefSharpnessPip = SharpnessVal;
			IsCorrect = TRUE;	
         }
         else if ( gm_GetChannelInputAttr(gmvb_PipChannel, gmd_COLOR_SPACE) == gmd_RGB )
         {
			UserPrefSharpnessRGBpip = SharpnessVal;
			IsCorrect = TRUE;	
         }
		 else
		 {
			IsCorrect = FALSE;
		 }
      }
   }
   else
   {
         if ( gm_GetChannelInputAttr(gmvb_MainChannel, gmd_COLOR_SPACE) != gmd_RGB )
         {
			UserPrefSharpness = SharpnessVal;
			IsCorrect = TRUE; 					
         }
		 else if( gm_GetChannelInputAttr(gmvb_MainChannel, gmd_COLOR_SPACE) == gmd_RGB )
		 {
			UserPrefSharpnessRGB = SharpnessVal;
			IsCorrect = TRUE;	 			 
		 }
		 else
		 {
			IsCorrect = FALSE;
		 } 
}
#endif

	return IsCorrect;
	
}

#if (!ENABLE_GET_I2C_READ_BYTE_VIA_SOCIPC_RECV_PACKET)
//******************************************************************************
// FUNCTION:   SocIPCGetResponseDataLength
// USAGE:  Get response data length of SocIPC commands
//
// INPUT:  Cmd
// OUTPUT: RtPacketDataLen
// GLOBALS:
//******************************************************************************
BYTE SocIPCGetResponseDataLength(gmt_SocIPCCommandCode Cmd)
{
	BYTE RtPacketDataLen = 0;

	switch( Cmd )
	{
	
		case SICMD_RW_Power_CONTROL:
			RtPacketDataLen = 8;				
			break;
		case SICMD_RO_Power_Status:
			RtPacketDataLen = 9;						
			break;
	/*ChengLin 20130110: Color Temp - User Settings*/
		case SICMD_RW_CT_RedGain_Ctrl:
			RtPacketDataLen = 8;	
			break;
		case SICMD_RW_CT_GreenGain_Ctrl:
			RtPacketDataLen = 8;	
			break;
		case SICMD_RW_CT_BlueGain_Ctrl:
			RtPacketDataLen = 8;	
			break;
		case SICMD_RW_CT_RedOffset_Ctrl:
			RtPacketDataLen = 8;
			break;
		case SICMD_RW_CT_GreenOffset_Ctrl:
			RtPacketDataLen = 8;
			break;
		case SICMD_RW_CT_BlueOffset_Ctrl:
			RtPacketDataLen = 8;
			break;
	/*****************************************/				
		case SICMD_RW_Cables_Status:
			RtPacketDataLen = 8;									
			break;
		case SICMD_RW_Mode_Info_CONTRL:
			RtPacketDataLen = 8;								
			break;
		case SICMD_RW_Mode_Info_STATUS:
			RtPacketDataLen = 10;						
			break;
		case SICMD_RW_Timing_H_Width:
			RtPacketDataLen = 8;								
			break;
		case SICMD_RW_Timing_V_Length:
			RtPacketDataLen = 8;								
			break;
		case SICMD_RW_Timing_H_Total:
			RtPacketDataLen = 8;								
			break;
		case SICMD_RW_Timing_V_Total:
			RtPacketDataLen = 8;								
			break;
		case SICMD_RW_Timing_H_Start:
			RtPacketDataLen = 8;								
			break;
		case SICMD_RW_Timing_V_Start:
			RtPacketDataLen = 8;								
			break;
		case SICMD_RO_Timing_H_Width_Slave:
			RtPacketDataLen = 10;								
			break;
		case SICMD_RO_Timing_V_Length_Slave:
			RtPacketDataLen = 10;								
			break;
		case SICMD_RO_Timing_H_Total_Slave:
			RtPacketDataLen = 10;								
			break;
		case SICMD_RO_Timing_V_Total_Slave:
			RtPacketDataLen = 10;							
			break;
		case SICMD_RO_Timing_H_Start_Slave:
			RtPacketDataLen = 10;								
			break;
		case SICMD_RO_Timing_V_Start_Slave:
			RtPacketDataLen = 10;							
			break;
		case SICMD_RO_Timing_V_Freq_Slave:
			RtPacketDataLen = 10;							
			break;
		case SICMD_RW_Port_CONTRL:
			RtPacketDataLen = 8;								
			break;
		case SICMD_RO_Port_Status:
			RtPacketDataLen = 9;							
			break;
		case SICMD_RW_Signal_Sync_CONTRL:
			RtPacketDataLen = 0;							
			break;
		case SICMD_RO_Signal_Sync_Status:
			RtPacketDataLen = 0;							
			break;
		case SICMD_RW_Brightness_CONTRL:
			RtPacketDataLen = 8;								
			break;
		case SICMD_RO_Brightness_Status:
			RtPacketDataLen = 9;							
			break;
		case SICMD_RO_OSD_Sync_changes:
			RtPacketDataLen = 9;							
			break;
		case SICMD_RW_Contrast_CONTRL:
			RtPacketDataLen = 8;								
			break;
		case SICMD_RO_Contrast_Status:
			RtPacketDataLen = 10;							
			break;
		case SICMD_RW_Sharpness_CONTRL:
			RtPacketDataLen = 8;								
			break;
		case SICMD_RO_Sharpness_Status:
			RtPacketDataLen = 9;								
			break;
		case SICMD_RW_Color_CONTRL:
			RtPacketDataLen = 8;								
			break;
		case SICMD_RO_Color_Status:
			RtPacketDataLen = 10;								
			break;
		case SICMD_RW_Video_progress_CONTRL:
			RtPacketDataLen = 8;								
			break;
		case SICMD_RO_Video_progress_Status:
			RtPacketDataLen = 10;							
			break;
		case SICMD_RW_NR_CONTRL:
			RtPacketDataLen = 8;								
			break;
		case SICMD_RO_NR_Status:
			RtPacketDataLen = 10;							
			break;
		case SICMD_RW_Video_progress1_CONTRL:
			RtPacketDataLen = 8;								
			break;
		case SICMD_RO_Video_progress1_Status:
			RtPacketDataLen = 10;								
			break;
		case SICMD_RW_DisplaySetup_CONTRL:
			RtPacketDataLen = 8;							
			break;
		case SICMD_RO_DisplaySetup_Status:
			RtPacketDataLen = 10;							
			break;
		case SICMD_RW_Audio1_CONTRL:
			RtPacketDataLen = 8;				
			break;
		case SICMD_RO_Audio1_Status:
			RtPacketDataLen = 10;								
			break;
		case SICMD_RW_Audio2_CONTRL:
			RtPacketDataLen = 8;				
			break;
		case SICMD_RO_Audio2_Status:
			RtPacketDataLen = 10;							
			break;
		case SICMD_RW_Setup_CONTRL:
			RtPacketDataLen = 8;								
			break;
		case SICMD_RO_Setup_Status:
			RtPacketDataLen = 10;								
			break;
		case SICMD_RW_PBP_CONTRL:			
			RtPacketDataLen = 8;	
			break;
		case SICMD_RW_OSD_Sync_CONTRL:
			RtPacketDataLen = 8;								
			break;
		case SICMD_RO_OSD_Sync_Status:
			RtPacketDataLen = 9;							
			break;
		case SICMD_RW_OSD_DP_Switch:			
			RtPacketDataLen = 8;	
			break;
		case SICMD_RW_DDCI_CONTRL:
			RtPacketDataLen = 8;				
			break;
		case SICMD_RO_DDCI_Status:
			RtPacketDataLen = 14;				
			break;
	   case SICMD_RO_DCR_Result:
			RtPacketDataLen = 10;
	/*ChengLin 20130110: 6-axis Saturation, Hue, Brightness controls*/
		case SICMD_RW_6Axis_Sat_Red_Ctrl:
			RtPacketDataLen = 8;
			break;
		case SICMD_RW_6Axis_Sat_Green_Ctrl:
			RtPacketDataLen = 8;
			break;
		case SICMD_RW_6Axis_Sat_Blue_Ctrl:
			RtPacketDataLen = 8;
			break;
		case SICMD_RW_6Axis_Sat_Yellow_Ctrl:
			RtPacketDataLen = 8;
			break;
		case SICMD_RW_6Axis_Sat_Magenta_Ctrl:
			RtPacketDataLen = 8;
			break;
		case SICMD_RW_6Axis_Sat_Cyan_Ctrl:
			RtPacketDataLen = 8;
			break;
		case SICMD_RW_6Axis_Sat_Black_Ctrl:
			RtPacketDataLen = 8;
			break;
		case SICMD_RW_6Axis_Sat_White_Ctrl:
			RtPacketDataLen = 8;
			break;
		case SICMD_RW_6Axis_Hue_Red_Ctrl:
			RtPacketDataLen = 8;
			break;
		case SICMD_RW_6Axis_Hue_Green_Ctrl:
			RtPacketDataLen = 8;
			break;
		case SICMD_RW_6Axis_Hue_Blue_Ctrl:
			RtPacketDataLen = 8;
			break;
		case SICMD_RW_6Axis_Hue_Yellow_Ctrl:
			RtPacketDataLen = 8;
			break;
		case SICMD_RW_6Axis_Hue_Magenta_Ctrl:
			RtPacketDataLen = 8;
			break;
		case SICMD_RW_6Axis_Hue_Cyan_Ctrl:
			RtPacketDataLen = 8;
			break;
		case SICMD_RW_6Axis_Hue_Black_Ctrl:
			RtPacketDataLen = 8;
			break;
		case SICMD_RW_6Axis_Hue_White_Ctrl:
			RtPacketDataLen = 8;
			break;
		case SICMD_RW_6Axis_Bright_Red_Ctrl:
			RtPacketDataLen = 8;
			break;
		case SICMD_RW_6Axis_Bright_Green_Ctrl:
			RtPacketDataLen = 8;
			break;
		case SICMD_RW_6Axis_Bright_Blue_Ctrl:
			RtPacketDataLen = 8;
			break;
		case SICMD_RW_6Axis_Bright_Yellow_Ctrl:
			RtPacketDataLen = 8;
			break;
		case SICMD_RW_6Axis_Bright_Magenta_Ctrl:
			RtPacketDataLen = 8;
			break;
		case SICMD_RW_6Axis_Bright_Cyan_Ctrl:
			RtPacketDataLen = 8;
			break;
		case SICMD_RW_6Axis_Bright_Black_Ctrl:
			RtPacketDataLen = 8;
			break;
		case SICMD_RW_6Axis_Bright_White_Ctrl:
			RtPacketDataLen = 8;
			break;
	/*****************************************/			
        case SICMD_WO_SMT_COMMAND:
            RtPacketDataLen = 8;
            break;
#ifdef DP_HUB_2_LOG_PORTS            
		case SICMD_RW_DpRx0_MstInfo: 
			RtPacketDataLen = 8;	
			break;       
      case SICMD_RW_DPRx0_ReModeSetup:
			RtPacketDataLen = 8;	
         break;        
#endif         			
		case SICMD_RW_Keypad_CONTRL:
			RtPacketDataLen = 8;				
			break;
		case SICMD_RW_IR_CONTROL:
			RtPacketDataLen = 8;								
			break;
	/*ChengLin 20130128: New Sync Handler State Control*/	
		case SICMD_RW_IMAGE_SYNC_STATE_Ctrl:
			RtPacketDataLen = 8;
			break;
		case SICMD_RW_IMAGE_SYNC_STATE_Status:
			RtPacketDataLen = 9;
			break;
	/*********************************************/
/*ChengLin 20130128: New Sync Handler State Control*/	
#if ENABLE_ATHENA_R_L_IMAGE_SYNC_HANDLER
		case SICMD_RW_IMAGE_SYNC_BLACK_SCREEN_CTRL:
			RtPacketDataLen = 8;
			break;
#endif	
		case SICMD_RW_GAMMA_CORRECTION_CONTRL: //Add by ChengLin 20130226
			RtPacketDataLen = 8;
			break;
#if ENABLE_VEGA_ISP_CMD_VIA_UART_ISP_PORT          
/*ChengLin 20130411: Enable Athena FE or Left Debug Mode from Athena Right*/
      case SICMD_RW_ATHENA_FE_L_DEBUG_MODE_EN_CTRL:
			RtPacketDataLen = 8;
         break;
#endif         
#ifdef SMART_ISP
      case SICMD_RW_SMARTISP_CTRL:
         RtPacketDataLen = 8;
         break;
      case SICMD_RO_SMARTISP_STATUS:
         RtPacketDataLen = 10;
         break;
#endif

      case SICMD_RO_DpRx0_IsStable:
         RtPacketDataLen = 9;
         break;
         
      /*ChengLin 20130322: Load Default Settings or Original Setting Before/After Splash Screen*/
      case SICMD_RW_LOAD_SPLASH_SETTING_CTRL:
         RtPacketDataLen = 8;
         break;
		case SICMD_RW_Slave_Registers_addr_CONTRL:
			RtPacketDataLen = 8;								
			break;
		case SICMD_RW_Slave_Registers_data_CONTRL:
			RtPacketDataLen = 8;								
			break;
		case SICMD_RO_Slave_Registers_read_data:
			RtPacketDataLen = 8; 								
			break;
		case SICMD_RW_SaveModeIndepedent_Ctrl: //Added by ChengLin 20130124
			RtPacketDataLen = 8; 	
			break;
		default:
			break;		
	}

	return RtPacketDataLen;

}
#endif //(!ENABLE_GET_I2C_READ_BYTE_VIA_SOCIPC_RECV_PACKET)

//******************************************************************************
// FUNCTION:   SocIPCGetResponseDataType
// USAGE:  Get response data type of SocIPC commands
//
// INPUT:  Cmd
// OUTPUT: RtPacketDataType
// GLOBALS:
//******************************************************************************
BYTE SocIPCGetResponseDataType(gmt_SocIPCCommandCode Cmd)
{
	BYTE RtPacketDataType = RECV_NOTHING;
	
	switch( Cmd )
	{
		case SICMD_RW_Power_CONTROL:			
			RtPacketDataType = RECV_ACK;
			break;
		case SICMD_RO_Power_Status:			
			RtPacketDataType = RECV_DATA;			
			break;
	/*ChengLin 20130110: Color Temp - User Settings*/
		case SICMD_RW_CT_RedGain_Ctrl:
			RtPacketDataType = RECV_ACK;	
			break;
		case SICMD_RW_CT_GreenGain_Ctrl:
			RtPacketDataType = RECV_ACK;	
			break;
		case SICMD_RW_CT_BlueGain_Ctrl:
			RtPacketDataType = RECV_ACK;	
			break;
		case SICMD_RW_CT_RedOffset_Ctrl:
			RtPacketDataType = RECV_ACK;
			break;
		case SICMD_RW_CT_GreenOffset_Ctrl:
			RtPacketDataType = RECV_ACK;
			break;
		case SICMD_RW_CT_BlueOffset_Ctrl:
			RtPacketDataType = RECV_ACK;
			break;
	/*****************************************/				
		case SICMD_RW_Cables_Status:			
			RtPacketDataType = RECV_ACK;					
			break;
		case SICMD_RW_Mode_Info_CONTRL:			
			RtPacketDataType = RECV_ACK;					
			break;
		case SICMD_RW_Mode_Info_STATUS:			
			RtPacketDataType = RECV_DATA;			
			break;
		case SICMD_RW_Timing_H_Width:			
			RtPacketDataType = RECV_ACK;				
			break;
		case SICMD_RW_Timing_V_Length:			
			RtPacketDataType = RECV_ACK;				
			break;
		case SICMD_RW_Timing_H_Total:			
			RtPacketDataType = RECV_ACK;				
			break;
		case SICMD_RW_Timing_V_Total:			
			RtPacketDataType = RECV_ACK;				
			break;
		case SICMD_RW_Timing_H_Start:			
			RtPacketDataType = RECV_ACK;				
			break;
		case SICMD_RW_Timing_V_Start:			
			RtPacketDataType = RECV_ACK;				
			break;
		case SICMD_RO_Timing_H_Width_Slave:			
			RtPacketDataType = RECV_DATA;				
			break;
		case SICMD_RO_Timing_V_Length_Slave:			
			RtPacketDataType = RECV_DATA;				
			break;
		case SICMD_RO_Timing_H_Total_Slave:			
			RtPacketDataType = RECV_DATA;				
			break;
		case SICMD_RO_Timing_V_Total_Slave:			
			RtPacketDataType = RECV_DATA;				
			break;
		case SICMD_RO_Timing_H_Start_Slave:			
			RtPacketDataType = RECV_DATA;				
			break;
		case SICMD_RO_Timing_V_Start_Slave:			
			RtPacketDataType = RECV_DATA;				
			break;
		case SICMD_RO_Timing_V_Freq_Slave:			
			RtPacketDataType = RECV_DATA;				
			break;
		case SICMD_RW_Port_CONTRL:			
			RtPacketDataType = RECV_ACK;				
			break;
		case SICMD_RO_Port_Status:				
			RtPacketDataType = RECV_DATA;				
			break;
		case SICMD_RW_Signal_Sync_CONTRL:			
			RtPacketDataType = RECV_NOTHING;				
			break;
		case SICMD_RO_Signal_Sync_Status:			
			RtPacketDataType = RECV_NOTHING;				
			break;
		case SICMD_RW_Brightness_CONTRL:			
			RtPacketDataType = RECV_ACK;				
			break;
		case SICMD_RO_Brightness_Status:			
			RtPacketDataType = RECV_DATA;				
			break;
		case SICMD_RW_Contrast_CONTRL:			
			RtPacketDataType = RECV_ACK;				
			break;
		case SICMD_RO_Contrast_Status:			
			RtPacketDataType = RECV_DATA;				
			break;
		case SICMD_RW_Sharpness_CONTRL:				
			RtPacketDataType = RECV_ACK;				
			break;
		case SICMD_RO_Sharpness_Status:			
			RtPacketDataType = RECV_DATA;				
			break;
		case SICMD_RW_Color_CONTRL:			
			RtPacketDataType = RECV_ACK;				
			break;
		case SICMD_RO_Color_Status:			
			RtPacketDataType = RECV_DATA;				
			break;
		case SICMD_RW_Video_progress_CONTRL:				
			RtPacketDataType = RECV_ACK;				
			break;
		case SICMD_RO_Video_progress_Status:			
			RtPacketDataType = RECV_DATA;				
			break;
		case SICMD_RW_NR_CONTRL:			
			RtPacketDataType = RECV_ACK;				
			break;
		case SICMD_RO_NR_Status: 			
			RtPacketDataType = RECV_DATA;				
			break;
		case SICMD_RW_Video_progress1_CONTRL:				
			RtPacketDataType = RECV_ACK;				
			break;
		case SICMD_RO_Video_progress1_Status: 			
			RtPacketDataType = RECV_DATA;				
			break;
		case SICMD_RW_DisplaySetup_CONTRL:				
			RtPacketDataType = RECV_ACK;				
			break;
		case SICMD_RO_DisplaySetup_Status:			
			RtPacketDataType = RECV_DATA;				
			break;
		case SICMD_RW_Audio1_CONTRL:				
			RtPacketDataType = RECV_ACK;
			break;
		case SICMD_RO_Audio1_Status:			
			RtPacketDataType = RECV_DATA;				
			break;
		case SICMD_RW_Audio2_CONTRL:			
			RtPacketDataType = RECV_ACK;
			break;
		case SICMD_RO_Audio2_Status:			
			RtPacketDataType = RECV_DATA;				
			break;
		case SICMD_RW_Setup_CONTRL:				
			RtPacketDataType = RECV_ACK;				
			break;
		case SICMD_RO_Setup_Status: 			
			RtPacketDataType = RECV_DATA;				
			break;
		case SICMD_RW_PBP_CONTRL:			
			RtPacketDataType = RECV_ACK;				
			break;
		case SICMD_RW_OSD_Sync_CONTRL:			
			RtPacketDataType = RECV_ACK;				
			break;
		case SICMD_RO_OSD_Sync_Status:			
			RtPacketDataType = RECV_DATA;				
			break;
		case SICMD_RO_OSD_Sync_changes:			
			RtPacketDataType = RECV_DATA;				
			break;
		case SICMD_RW_OSD_DP_Switch:			
			RtPacketDataType = RECV_ACK;				
			break;
		case SICMD_RW_DDCI_CONTRL:				
			RtPacketDataType = RECV_ACK;
			break;
		case SICMD_RO_DDCI_Status:			
			RtPacketDataType = RECV_DATA;
			break;
		case SICMD_RO_DCR_Result:
			RtPacketDataType = RECV_DATA;
			break;
	/*ChengLin 20130110: 6-axis Saturation, Hue, Brightness controls*/
		case SICMD_RW_6Axis_Sat_Red_Ctrl:
			RtPacketDataType = RECV_ACK;
			break;
		case SICMD_RW_6Axis_Sat_Green_Ctrl:
			RtPacketDataType = RECV_ACK;
			break;
		case SICMD_RW_6Axis_Sat_Blue_Ctrl:
			RtPacketDataType = RECV_ACK;
			break;
		case SICMD_RW_6Axis_Sat_Yellow_Ctrl:
			RtPacketDataType = RECV_ACK;
			break;
		case SICMD_RW_6Axis_Sat_Magenta_Ctrl:
			RtPacketDataType = RECV_ACK;
			break;
		case SICMD_RW_6Axis_Sat_Cyan_Ctrl:
			RtPacketDataType = RECV_ACK;
			break;
		case SICMD_RW_6Axis_Sat_Black_Ctrl:
			RtPacketDataType = RECV_ACK;
			break;
		case SICMD_RW_6Axis_Sat_White_Ctrl:
			RtPacketDataType = RECV_ACK;
			break;
		case SICMD_RW_6Axis_Hue_Red_Ctrl:
			RtPacketDataType = RECV_ACK;
			break;
		case SICMD_RW_6Axis_Hue_Green_Ctrl:
			RtPacketDataType = RECV_ACK;
			break;
		case SICMD_RW_6Axis_Hue_Blue_Ctrl:
			RtPacketDataType = RECV_ACK;
			break;
		case SICMD_RW_6Axis_Hue_Yellow_Ctrl:
			RtPacketDataType = RECV_ACK;
			break;
		case SICMD_RW_6Axis_Hue_Magenta_Ctrl:
			RtPacketDataType = RECV_ACK;
			break;
		case SICMD_RW_6Axis_Hue_Cyan_Ctrl:
			RtPacketDataType = RECV_ACK;
			break;
		case SICMD_RW_6Axis_Hue_Black_Ctrl:
			RtPacketDataType = RECV_ACK;
			break;
		case SICMD_RW_6Axis_Hue_White_Ctrl:
			RtPacketDataType = RECV_ACK;
			break;
		case SICMD_RW_6Axis_Bright_Red_Ctrl:
			RtPacketDataType = RECV_ACK;
			break;
		case SICMD_RW_6Axis_Bright_Green_Ctrl:
			RtPacketDataType = RECV_ACK;
			break;
		case SICMD_RW_6Axis_Bright_Blue_Ctrl:
			RtPacketDataType = RECV_ACK;
			break;
		case SICMD_RW_6Axis_Bright_Yellow_Ctrl:
			RtPacketDataType = RECV_ACK;
			break;
		case SICMD_RW_6Axis_Bright_Magenta_Ctrl:
			RtPacketDataType = RECV_ACK;
			break;
		case SICMD_RW_6Axis_Bright_Cyan_Ctrl:
			RtPacketDataType = RECV_ACK;
			break;
		case SICMD_RW_6Axis_Bright_Black_Ctrl:
			RtPacketDataType = RECV_ACK;
			break;
		case SICMD_RW_6Axis_Bright_White_Ctrl:
			RtPacketDataType = RECV_ACK;	
			break;
	/*****************************************/				
        case SICMD_WO_SMT_COMMAND:
            RtPacketDataType = RECV_ACK;
            break;
#ifdef DP_HUB_2_LOG_PORTS            
		case SICMD_RW_DpRx0_MstInfo:	
			RtPacketDataType = RECV_ACK;
			break;    

      case SICMD_RO_DpRx0_IsStable:
         RtPacketDataType = RECV_DATA;       
         break; 
      case SICMD_RW_DPRx0_ReModeSetup:
			RtPacketDataType = RECV_ACK;
         break;        
#endif         			
		case SICMD_RW_Keypad_CONTRL:			
			RtPacketDataType = RECV_ACK;
			break;
		case SICMD_RW_IR_CONTROL:				
			RtPacketDataType = RECV_ACK;				
			break;
	/*ChengLin 20130128: New Sync Handler State Control*/	
		case SICMD_RW_IMAGE_SYNC_STATE_Ctrl:
			RtPacketDataType = RECV_ACK;
			break;
		case SICMD_RW_IMAGE_SYNC_STATE_Status:
			RtPacketDataType = RECV_DATA;
			break;
	/*********************************************/
/*ChengLin 20130128: New Sync Handler State Control*/	
#if ENABLE_ATHENA_R_L_IMAGE_SYNC_HANDLER
		case SICMD_RW_IMAGE_SYNC_BLACK_SCREEN_CTRL:
			RtPacketDataType = RECV_ACK;
			break;
#endif	
		case SICMD_RW_GAMMA_CORRECTION_CONTRL: //Add by ChengLin 20130226
			RtPacketDataType = RECV_ACK;
			break;
#if ENABLE_VEGA_ISP_CMD_VIA_UART_ISP_PORT          
/*ChengLin 20130411: Enable Athena FE or Left Debug Mode from Athena Right*/
      case SICMD_RW_ATHENA_FE_L_DEBUG_MODE_EN_CTRL:
			RtPacketDataType = RECV_ACK;
         break;
#endif         
#ifdef SMART_ISP
      case SICMD_RW_SMARTISP_CTRL:
         RtPacketDataType = RECV_ACK;
         break;
      case SICMD_RO_SMARTISP_STATUS:
         RtPacketDataType = RECV_DATA;       
         break;
#endif
      /*ChengLin 20130322: Load Default Settings or Original Setting Before/After Splash Screen*/
      case SICMD_RW_LOAD_SPLASH_SETTING_CTRL:
         RtPacketDataType = RECV_ACK;
         break;
		case SICMD_RW_Slave_Registers_addr_CONTRL:				
			RtPacketDataType = RECV_ACK;				
			break;
		case SICMD_RW_Slave_Registers_data_CONTRL:			
			RtPacketDataType = RECV_ACK;				
			break;
		case SICMD_RO_Slave_Registers_read_data:			
			RtPacketDataType = RECV_DATA;						
			break;
		case SICMD_RW_SaveModeIndepedent_Ctrl: //Added by ChengLin 20130124
			RtPacketDataType = RECV_ACK;	
			break;			
		default:			
			break;		
	}

	return RtPacketDataType;

}

//******************************************************************************
// FUNCTION:   SOCIPC_SyncOSDAdjustHandler
// USAGE:  Used for OSD Adjustment Function Common Interface
//
// INPUT:  SI_OSD_Cmd
// OUTPUT: N/A
// GLOBALS:
//******************************************************************************
void SOCIPC_SyncOSDAdjustHandler( gmt_SocIPC_OSDSyncID SI_OSD_Cmd )
{
   BYTE CmdBuf[1] = {0};

	switch(SI_OSD_Cmd)
	{
#ifdef ADJUSTER_BRIGHTNESS_DECLARED	
		case SI_OSD_SYNC_BRIGHTNESS:
				SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_Brightness_CONTRL, NULL);		
			break;
#endif

#ifdef ADJUSTER_CONTRAST_DECLARED
		case SI_OSD_SYNC_CONTRAST:
				SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_Contrast_CONTRL, NULL);
			break;
#endif

#if defined(ADJUSTER_SHARPNESS_DECLARED) || defined(ADJUSTER_SHARPNESSRGB_DECLARED)
		case SI_OSD_SYNC_SHARPNESS:
		case SI_OSD_SYNC_SHARPNESS_RGB:			
				SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_Sharpness_CONTRL, NULL);
			break;
#endif

#if defined(Action_AdjustImageScheme_WB_Used)|| defined(ADJUSTER_COLORTEMP_DECLARED)
		case SI_OSD_SYNC_COLOR_CTRL:
				SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_Color_CONTRL, NULL);				
			break;
#endif

#if defined(Action_AdjustDCDiMain_WB_Used) || defined(Action_AdjustMADI_WB_Used) || defined(ADJUSTER_LCDOVERDRIVE_DECLARED) || defined(VWD_HIGHLIGHT_DEMO)
		case SI_OSD_SYNC_VIDEO_PROCESSING:
			SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_Video_progress_CONTRL, NULL); 
			break;
#endif

#if defined(Action_AdjustTNR_WB_Used) || defined(Action_AdjustMPEGNRMode_WB_Used)
		case SI_OSD_SYNC_NOISE_REDUCTION:
			SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_NR_CONTRL, NULL); 
			break;
#endif

#if defined(ADJUSTER_SPLITSCREEN_DECLARED) || defined(Action_AdjustFilmMode_WB_Used)
		case SI_OSD_SYNC_VIDEO_PROCESSING_2:
			SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_Video_progress1_CONTRL, NULL); 
			break;
#endif

#if defined(Action_AdjustAutoAspectRatio_WB_Used) || defined(Action_AdjustAspectRatioMain_WB_Used)
		case SI_OSD_SYNC_DISPLAY_SETUP:
         /*ChengLin 20130315: Add for splash screen case*/
         CmdBuf[0] = TRUE; //Update Flag          
			SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_DisplaySetup_CONTRL, CmdBuf); 
			break;
#endif

#if (FEATURE_AUDIO == ENABLE)
		case SI_OSD_SYNC_AUDIO:
			SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_Audio1_CONTRL, NULL); 
			break;
			
		case SI_OSD_SYNC_AUDIO_2:
			SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_Audio2_CONTRL, NULL); 
			break;			
#endif

		case SI_OSD_SYNC_SETUP:
			SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_Setup_CONTRL, NULL); 
			break;

		case SI_OSD_SYNC_SAVE_PBP_Mode:
			SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_PBP_CONTRL, NULL);
			break;
			
		case SI_OSD_SYNC_DP_SWITCH:
			SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_OSD_DP_Switch, NULL);
			break;

		case SI_OSD_SYNC_SEL_INPUT_MAIN:
			SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_Port_CONTRL, NULL);
			break;

#ifdef ADJUSTER_USERREDGAIN_DECLARED
		case SI_OSD_SYNC_CT_USER_RGAIN:
			SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_CT_RedGain_Ctrl, NULL);
			break;
#endif

#ifdef ADJUSTER_USERGREENGAIN_DECLARED
		case SI_OSD_SYNC_CT_USER_GGAIN:
			SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_CT_GreenGain_Ctrl, NULL);
			break;
#endif

#ifdef ADJUSTER_USERBLUEGAIN_DECLARED
		case SI_OSD_SYNC_CT_USER_BGAIN:
			SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_CT_BlueGain_Ctrl, NULL);
			break;
#endif

#ifdef ADJUSTER_USERREDOFF_DECLARED
		case SI_OSD_SYNC_CT_USER_ROFFSET:
			SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_CT_RedOffset_Ctrl, NULL);
			break;
#endif

#ifdef ADJUSTER_USERGREENOFF_DECLARED
		case SI_OSD_SYNC_CT_USER_GOFFSET:
			SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_CT_GreenOffset_Ctrl, NULL);
			break;
#endif

#ifdef ADJUSTER_USERBLUEOFF_DECLARED
		case SI_OSD_SYNC_CT_USER_BOFFSET:
			SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_CT_BlueOffset_Ctrl, NULL);
			break;
#endif

#ifdef Action_AdjustAcmRedSat_WB_Used
		case SI_OSD_SYNC_SIX_AXIS_SAT_RED:
			SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_6Axis_Sat_Red_Ctrl, NULL);
			break;
#endif

#ifdef Action_AdjustAcmGreenSat_WB_Used
		case SI_OSD_SYNC_SIX_AXIS_SAT_GREEN:
			SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_6Axis_Sat_Green_Ctrl, NULL);			
			break;
#endif

#ifdef Action_AdjustAcmBlueSat_WB_Used
		case SI_OSD_SYNC_SIX_AXIS_SAT_BLUE:
			SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_6Axis_Sat_Blue_Ctrl, NULL);			
			break;
#endif

#ifdef Action_AdjustAcmYellowSat_WB_Used
		case SI_OSD_SYNC_SIX_AXIS_SAT_YELLOW:
			SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_6Axis_Sat_Yellow_Ctrl, NULL);			
			break;
#endif

#ifdef Action_AdjustAcmMagentaSat_WB_Used
		case SI_OSD_SYNC_SIX_AXIS_SAT_MAGENTA:
			SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_6Axis_Sat_Magenta_Ctrl, NULL);			
			break;
#endif

#ifdef Action_AdjustAcmCyanSat_WB_Used
		case SI_OSD_SYNC_SIX_AXIS_SAT_CYAN:
			SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_6Axis_Sat_Cyan_Ctrl, NULL);			
			break;
#endif

#ifdef Action_AdjustAcmBlackSat_WB_Used
		case SI_OSD_SYNC_SIX_AXIS_SAT_BLACK:
			SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_6Axis_Sat_Black_Ctrl, NULL);				
			break;
#endif

#ifdef Action_AdjustAcmWhiteSat_WB_Used
		case SI_OSD_SYNC_SIX_AXIS_SAT_WHITE:
			SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_6Axis_Sat_White_Ctrl, NULL);				
			break;
#endif

#ifdef Action_AdjustAcmRedHue_WB_Used
		case SI_OSD_SYNC_SIX_AXIS_HUE_RED:
			SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_6Axis_Hue_Red_Ctrl, NULL);			
			break;
#endif

#ifdef Action_AdjustAcmGreenHue_WB_Used
		case SI_OSD_SYNC_SIX_AXIS_HUE_GREEN:
			SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_6Axis_Hue_Green_Ctrl, NULL);			
			break;
#endif

#ifdef Action_AdjustAcmBlueHue_WB_Used
		case SI_OSD_SYNC_SIX_AXIS_HUE_BLUE:
			SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_6Axis_Hue_Blue_Ctrl, NULL);			
			break;
#endif

#ifdef Action_AdjustAcmYellowHue_WB_Used
		case SI_OSD_SYNC_SIX_AXIS_HUE_YELLOW:
			SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_6Axis_Hue_Yellow_Ctrl, NULL);				
			break;
#endif

#ifdef Action_AdjustAcmMagentaHue_WB_Used
		case SI_OSD_SYNC_SIX_AXIS_HUE_MAGENTA:
			SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_6Axis_Hue_Magenta_Ctrl, NULL);			
			break;
#endif

#ifdef Action_AdjustAcmCyanHue_WB_Used
		case SI_OSD_SYNC_SIX_AXIS_HUE_CYAN:
			SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_6Axis_Hue_Cyan_Ctrl, NULL);			
			break;
#endif

#ifdef Action_AdjustAcmBlackHue_WB_Used
		case SI_OSD_SYNC_SIX_AXIS_HUE_BLACK:
			SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_6Axis_Hue_Black_Ctrl, NULL);				
			break;
#endif

#ifdef Action_AdjustAcmWhiteHue_WB_Used
		case SI_OSD_SYNC_SIX_AXIS_HUE_WHITE:
			SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_6Axis_Hue_White_Ctrl, NULL);				
			break;
#endif

#ifdef Action_AdjustAcmRedLuma_WB_Used
		case SI_OSD_SYNC_SIX_AXIS_BRI_RED:
			SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_6Axis_Bright_Red_Ctrl, NULL);			
			break;
#endif

#ifdef Action_AdjustAcmGreenLuma_WB_Used
		case SI_OSD_SYNC_SIX_AXIS_BRI_GREEN:
			SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_6Axis_Bright_Green_Ctrl, NULL);		
			break;
#endif

#ifdef Action_AdjustAcmBlueLuma_WB_Used
		case SI_OSD_SYNC_SIX_AXIS_BRI_BLUE:
			SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_6Axis_Bright_Blue_Ctrl, NULL);			
			break;
#endif

#ifdef Action_AdjustAcmYellowLuma_WB_Used
		case SI_OSD_SYNC_SIX_AXIS_BRI_YELLOW:
			SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_6Axis_Bright_Yellow_Ctrl, NULL);		
			break;
#endif

#ifdef Action_AdjustAcmMagentaLuma_WB_Used
		case SI_OSD_SYNC_SIX_AXIS_BRI_MAGENTA:
			SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_6Axis_Bright_Magenta_Ctrl, NULL);		
			break;
#endif

#ifdef Action_AdjustAcmCyanLuma_WB_Used
		case SI_OSD_SYNC_SIX_AXIS_BRI_CYAN:
			SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_6Axis_Bright_Cyan_Ctrl, NULL);			
			break;
#endif

#ifdef Action_AdjustAcmBlackLuma_WB_Used
		case SI_OSD_SYNC_SIX_AXIS_BRI_BLACK:
			SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_6Axis_Bright_Black_Ctrl, NULL);			
			break;
#endif

#ifdef Action_AdjustAcmWhiteLuma_WB_Used
		case SI_OSD_SYNC_SIX_AXIS_BRI_WHITE:
			SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_6Axis_Bright_White_Ctrl, NULL);			
			break;
#endif

		case SI_OSD_SYNC_SAVE_MODE_INDEPEDENT:
			SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_SaveModeIndepedent_Ctrl, NULL);	
			break;
			
		default:
			gm_Print("\n<ERROR> Non-Supported SI OSD Sync Function.\n",0);
			break;
	}
}

/*ChengLin 20130131: Enable VEGA I2C Control Via Athena FE*/
#if (ENABLE_VEGA_CONTROL_VIA_ATHENA_FE)
//******************************************************************************
// FUNCTION:   SOCIPC_VEGA_I2C_WriteWord_Via_FE
// USAGE:  Used for Athena FE to write VEGA registers via I2C Master 1
//
// INPUT:  SlaveAddr, wValue 
// OUTPUT: N/A
// GLOBALS:
//******************************************************************************
void SOCIPC_VEGA_I2C_WriteWord_Via_FE(BYTE SlaveAddr, WORD wValue)
{
	BYTE Buffer[3];

#if ENABLE_VEGA_AVAIL_CHECK_VIA_FE_AC_ON
   SOCIPC_Check_VEGA_Available_Via_FE();
#endif

	if(GetChipID_4K2K() == CHIPID_4K2K_FE)
	{
		Buffer[0] = SlaveAddr;
		Buffer[1] = (BYTE)((wValue>>8)&0x00FF);	//High Byte First	
		Buffer[2] = (BYTE)(wValue&0x00FF);	//Low Byte	

		if (gm_Write2WireBlock(0, SOCIPC_VEGA_DEVICE_ADDRESS, (BYTE*)Buffer, 3, TRUE) != gmd_PASS)
	   		VEGA_ctrl_via_FE_msg("\nAthena FE Write Register 0x%x Fail.\n",SlaveAddr);
	}
}

//******************************************************************************
// FUNCTION:   SOCIPC_VEGA_I2C_ReadWord_Via_FE
// USAGE:  Used for Athena FE to read VEGA registers via I2C Master 1
//
// INPUT:  N/A
// OUTPUT:RetVal
// GLOBALS:
//******************************************************************************
WORD SOCIPC_VEGA_I2C_ReadWord_Via_FE(BYTE SlaveAddr)
{
	BYTE Buffer[2];
	WORD RetVal = 0;

#if ENABLE_VEGA_AVAIL_CHECK_VIA_FE_AC_ON
    SOCIPC_Check_VEGA_Available_Via_FE();
#endif

	if(GetChipID_4K2K() == CHIPID_4K2K_FE)
	{
		if (gm_Write2WireBlock(0, SOCIPC_VEGA_DEVICE_ADDRESS, (BYTE*) &SlaveAddr, 1, FALSE) != gmd_PASS)
		{
		   VEGA_ctrl_via_FE_msg("\nAthena FE Read Register 0x%x Fail.\n",SlaveAddr);
		   return 0;
		}
	   	if (gm_Read2WireBlock(0, SOCIPC_VEGA_DEVICE_ADDRESS, Buffer, 2, TRUE) != gmd_PASS)
	   	{
			VEGA_ctrl_via_FE_msg("\nAthena FE Read Register 0x%x Fail.\n",SlaveAddr);
			return 0;
	   	}		
		RetVal = (WORD)Buffer[0];
		RetVal = (WORD)(((RetVal<<8)&0xFF00)|Buffer[1]);
	}
 	return RetVal;
}

#if ENABLE_VEGA_AVAIL_CHECK_VIA_FE_AC_ON
//******************************************************************************
// FUNCTION:   SOCIPC_Check_VEGA_Available_Via_FE
// USAGE:  Athena FE check VEGA device is available or not
//
// INPUT:  N/A
// OUTPUT:void
// GLOBALS:
//******************************************************************************
void SOCIPC_Check_VEGA_Available_Via_FE(void)
{
   /*ChengLin 20130327: Athena FE must to wait VEGA to response "ACK" for ready program VEGA via Athena FE*/
   if(GetChipID_4K2K() == CHIPID_4K2K_FE)
   {
      if(VEGA_Initialed == FALSE)
      {   
         check_VEGA_available_msg("\nCheck VEGA Device is Ready First Time.......\n",0);   
         gm_TimerStart(VEGA_AVAIL_CHECK_VIA_FE_10ms_TMR, 50); //Wait 500ms
      
         while(gm_2WireCheck(0, SOCIPC_VEGA_DEVICE_ADDRESS)== gmd_FAIL)
         {
            if(gm_TimerCheck(VEGA_AVAIL_CHECK_VIA_FE_10ms_TMR) == TIMER_TMO)
            {
               gm_TimerStop(VEGA_AVAIL_CHECK_VIA_FE_10ms_TMR);          
               check_VEGA_available_msg("FAIL.\n",0); 
               break;
            }      
         }
         if(gm_2WireCheck(0, SOCIPC_VEGA_DEVICE_ADDRESS)== gmd_PASS)
            check_VEGA_available_msg("PASS.\n",0); 
         VEGA_Initialed = TRUE;
      }
   }
}
#endif

//******************************************************************************
// FUNCTION:   SOCIPC_VEGA_WriteHWReg_Via_FE
// USAGE:  Used for Write VEGA H/W Registers via Host Interface
//
// INPUT:  HWAddr,Value
// OUTPUT:N/A
// GLOBALS:
//******************************************************************************
void SOCIPC_VEGA_WriteHWReg_Via_FE(WORD HWAddr, BYTE Value)
{
	BYTE Buffer[4] = {0};

#if ENABLE_VEGA_CONTROL_VIA_ATHENA_FE //Added by ChengLin 20130220	  
   SOCIPC_VEGA_I2C_WriteWord_Via_FE(0x08,0x09F0); //Set I2C Address 0xF0 for H/W Register Access
#endif	  

	if(GetChipID_4K2K() == CHIPID_4K2K_FE)
	{
		Buffer[0] = (BYTE)((HWAddr>>8)&0x00FF);	//High Byte First
		Buffer[1] = (BYTE)(HWAddr&0x00FF);	//Low Byte
		Buffer[2] = Value;

		if (gm_Write2WireBlock(0, 0xF0, (BYTE*)Buffer, 3, TRUE) != gmd_PASS)
	   		VEGA_ctrl_via_FE_msg("\nAthena FE Write VEGA HW Register 0x%x Fail.\n",HWAddr);		
	}
}

//******************************************************************************
// FUNCTION:   SOCIPC_VEGA_ReadHWRegWord_Via_FE
// USAGE:  Used for Read VEGA H/W Registers via Host Interface
//
// INPUT:  HWAddr
// OUTPUT:RetVal
// GLOBALS:
//******************************************************************************
BYTE SOCIPC_VEGA_ReadHWRegWord_Via_FE(WORD HWAddr)
{
	BYTE AddrBuffer[2] = {0};
	BYTE RetVal = 0;

#if ENABLE_VEGA_CONTROL_VIA_ATHENA_FE //Added by ChengLin 20130220	  
   SOCIPC_VEGA_I2C_WriteWord_Via_FE(0x08,0x09F0); //Set I2C Address 0xF0 for H/W Register Access
#endif	  

	if(GetChipID_4K2K() == CHIPID_4K2K_FE)
	{
		AddrBuffer[0] = (BYTE)((HWAddr>>8)&0x00FF);	//High Byte First
		AddrBuffer[1] = (BYTE)(HWAddr&0x00FF);	//Low Byte
		
		if (gm_Write2WireBlock(0, 0xF0, AddrBuffer, 2, FALSE) != gmd_PASS)
		{
		   VEGA_ctrl_via_FE_msg("\nAthena FE Read VEGA HW Register 0x%x Fail.\n",HWAddr);
		   return 0;
		}
	   	if (gm_Read2WireBlock(0, 0xF0, &RetVal, 1, TRUE) != gmd_PASS)
	   	{
			VEGA_ctrl_via_FE_msg("\nAthena FE Read VEGA HWRegister 0x%x Fail.\n",HWAddr);
			return 0;
	   	}
	}
 	return RetVal;
}

//******************************************************************************
// FUNCTION:   SOCIPC_VEGA_LVDS_BUS_WIDTH_Via_HDMI_Input
// USAGE:  Used for Athena FE to check HDMI input color depth and program VEGA LVDS BUS WIDTH
//
// INPUT:  N/A
// OUTPUT:N/A
// GLOBALS:
//******************************************************************************
void SOCIPC_VEGA_LVDS_BUS_WIDTH_Via_HDMI_Input(void)
{
	WORD wCurrValue = 0;
	
	if(GetChipID_4K2K() == CHIPID_4K2K_FE)
	{
		wCurrValue = SOCIPC_VEGA_I2C_ReadWord_Via_FE(0x0B);
		
		switch(gm_HdmiGetColorDepth(gB_CurrentInputConnector[gmvb_MainChannel]))
		{
			case gm_COLOR_DEPTH_UNKNOWN:
			default:
            if((wCurrValue & 0x0030) != 0x0010) //Re-Setup once VEGA BUS not equal 8 bits mode
            {
				   wCurrValue &= 0xFFCF;
				   wCurrValue |= 0x0010; 
				   SOCIPC_VEGA_I2C_WriteWord_Via_FE(0x0B, wCurrValue);				
				   VEGA_ctrl_via_FE_msg("\n<Default> VEGA LVDS BUS WIDTH Set to 8 bits Mode. \n",0);
            }
				break;
			case gm_COLOR_DEPTH_18BPP:
			case gm_COLOR_DEPTH_24BPP:
            if((wCurrValue & 0x0030) != 0x0010) //Re-Setup once VEGA BUS not equal 8 bits mode
            {
				   wCurrValue &= 0xFFCF;
				   wCurrValue |= 0x0010; 
				   SOCIPC_VEGA_I2C_WriteWord_Via_FE(0x0B, wCurrValue);				
				   VEGA_ctrl_via_FE_msg("\n<HDMI Color Depth 8 bits> VEGA LVDS BUS WIDTH Set to 8 bits Mode. \n",0);
            }
				break;
				
			case gm_COLOR_DEPTH_30BPP:
			case gm_COLOR_DEPTH_36BPP:
			case gm_COLOR_DEPTH_48BPP:
            if((wCurrValue & 0x0030) != 0x0000) //Re-Setup once VEGA BUS not equal 10 bits mode 
            {
				   wCurrValue &= 0xFFCF;
				   SOCIPC_VEGA_I2C_WriteWord_Via_FE(0x0B, wCurrValue);				
				   VEGA_ctrl_via_FE_msg("\n<HDMI Color Depth 10 bits> VEGA LVDS BUS WIDTH Set to 10 bits Mode. \n",0);	
            }
				break;
		}
	}
}

#if ENABLE_VEGA_CSC_VIA_HDMI_INFO
//******************************************************************************
// FUNCTION:   SOCIPC_VEGA_CSC_Via_HDMI_Input
// USAGE:  Used for Athena FE to check HDMI input color format and program VEGA CSC
//
// INPUT:  bPassThrough
// OUTPUT:N/A
// GLOBALS:
//******************************************************************************
void SOCIPC_VEGA_CSC_Via_HDMI_Input(BOOL bPassThrough)
{
//VEGA DPTX will keep in RGB mode, this function will convert HDMI color space to RGB DPTX output for Pass-through mode
	BYTE ColorSpace = STHDMIRX_COLOR_FORMAT_RGB;
	BYTE B_HDMIColorimetry = STHDMIRX_COLORIMETRY_STD_DEFAULT;
	WORD wCurrValue = 0;	

	if(GetChipID_4K2K() == CHIPID_4K2K_FE)
	{
		wCurrValue = SOCIPC_VEGA_I2C_ReadWord_Via_FE(0x91);

		if(bPassThrough)
		{
         if((wCurrValue & 0x0008)== 0) //Re-Setup once VEGA cuurent mode is scaling mode
         {
            if(gm_IsHdmiTiming(gB_CurrentInputConnector[gmvb_MainChannel]))
            {
			      ColorSpace = gm_HdmiGetAviColorSpace(gB_CurrentInputConnector[gmvb_MainChannel]);
			      B_HDMIColorimetry = HdmiGetColorimetry(gB_CurrentInputConnector[gmvb_MainChannel]);		
            }
            else
            {
   			   ColorSpace = STHDMIRX_COLOR_FORMAT_RGB;
   			   B_HDMIColorimetry = STHDMIRX_COLORIMETRY_STD_DEFAULT;            
            }

            wCurrValue &= 0xFFF0;
			   wCurrValue|=0x0008; //Enable Pass Through Mode

			   switch(ColorSpace)
			   {
				   case STHDMIRX_COLOR_FORMAT_RGB:
				   default:
					   VEGA_ctrl_via_FE_msg("\n<VEGA COLOR FORMAT> RGB.\n",0);
					   break;
				   case STHDMIRX_COLOR_FORMAT_YUV_422:
					   wCurrValue|=0x0001;				
					   VEGA_ctrl_via_FE_msg("\n<VEGA COLOR FORMAT> YUV422.\n",0);
					   break;
				   case STHDMIRX_COLOR_FORMAT_YUV_444:
					   wCurrValue|=0x0002;				
					   VEGA_ctrl_via_FE_msg("\n<VEGA COLOR FORMAT> YUV444.\n",0);
					   break;
			   }

			   switch(B_HDMIColorimetry)
			   {
				   case STHDMIRX_COLORIMETRY_STD_DEFAULT:
				   case STHDMIRX_COLORIMETRY_STD_AdobeRGB:
				   default:
					   VEGA_ctrl_via_FE_msg("\n<VEGA COLORIMETRY> RGB.\n",0);
					   break;
				   case STHDMIRX_COLORIMETRY_STD_BT_601:
				   case STHDMIRX_COLORIMETRY_STD_XVYCC_601:
				   case STHDMIRX_COLORIMETRY_STD_sYCC_601:
				   case STHDMIRX_COLORIMETRY_STD_AdobeYCC_601:
					   VEGA_ctrl_via_FE_msg("\n<VEGA COLORIMETRY> 601.\n",0);
					   break;
				   case STHDMIRX_COLORIMETRY_STD_BT_709:
				   case STHDMIRX_COLORIMETRY_STD_XVYCC_709:
					   wCurrValue|=0x0004;					
					   VEGA_ctrl_via_FE_msg("\n<VEGA COLORIMETRY> 709.\n",0);
					   break;
			   }
            SOCIPC_VEGA_I2C_WriteWord_Via_FE(0x91, wCurrValue);	
         }
		}
		else
		{
         if((wCurrValue & 0x0008)!= 0) //Re-Setup once VEGA cuurent mode is passthrough mode
         {
            wCurrValue &= 0xFFF0;     
			   VEGA_ctrl_via_FE_msg("\n<VEGA DEFAULT> HDMI Scalaing Mode. \n",0);
            SOCIPC_VEGA_I2C_WriteWord_Via_FE(0x91, wCurrValue);
         }
		}

	}
}
#endif

#endif

#if ENABLE_ATHENA_R_L_IMAGE_SYNC_HANDLER
//******************************************************************************
// FUNCTION:   SOCIPC_SetAthena_L_ImageSyncState
// USAGE:  Used for set Athena Left current Image Sync State
//
// INPUT:  State
// OUTPUT:N/A
// GLOBALS:
//******************************************************************************
void SOCIPC_SetAthena_L_ImageSyncState(gmt_ImageSyncState State)
{
	BYTE SocIPCmdBuffer[1] = {0};

	SocIPCmdBuffer[0] = State;
	SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_IMAGE_SYNC_STATE_Ctrl, SocIPCmdBuffer);	
}

//******************************************************************************
// FUNCTION:   SOCIPC_GetAthena_L_ImageSyncState
// USAGE:  Used for get Athena Left current Image Sync State
//
// INPUT:  N/A
// OUTPUT:ImageSyncState
// GLOBALS:
//******************************************************************************
gmt_ImageSyncState SOCIPC_GetAthena_L_ImageSyncState(void)
{
	SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_IMAGE_SYNC_STATE_Status, NULL);	

	return (gAthena_LEFT_FE_Slave_Parameters.ImageSyncState);
}

//******************************************************************************
// FUNCTION:   SOCIPC_SetAthena_Right_Left_PB_Ctrl_Status
// USAGE:  Used for set Athena Right/Left PB_Ctrl Status
//
// INPUT:  Enable
// OUTPUT:N/A
// GLOBALS:
//******************************************************************************
void SOCIPC_SetAthena_Right_Left_PB_Ctrl_Status(BOOL Enable)
{
	BYTE SocIPCmdBuffer[1] = {0};

	//Added by ChengLin 20130227
#ifdef ST_4K2K_93xx_BOARD
	   if(GetChipID_4K2K()==CHIPID_4K2K_R)
	   {
#if ENABLE_SOC_IPC_BUSY_CHECK
		/*ChengLin 20130124: New 4k2k board BUSY status check*/
		if(!SocIPCIsSlaveLeftReady())
			return;
#endif                      
		SocIPCmdBuffer[0] = Enable; //Athena Left PB_Ctrl
		SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_IMAGE_SYNC_BLACK_SCREEN_CTRL, SocIPCmdBuffer);
	   }
#endif

	if(Enable) // On
	{
		gm_SetRegBitsWord(PB_CTRL, PB_MAIN_ENABLE);
		gm_ClearRegBitsWord(PB_CTRL, PB_FORCE_MAIN_BKGND);
		gm_ForceUpdate(CH_A , INPUT_OUTPUT_CLOCK);
	}
	else // Off
	{
		gm_ClearRegBitsWord(PB_CTRL, PB_MAIN_ENABLE);
		gm_SetRegBitsWord(PB_CTRL, PB_FORCE_MAIN_BKGND);
		gm_ForceUpdate(CH_A , INPUT_OUTPUT_CLOCK);
	}
}
#endif

//******************************************************************************
// FUNCTION:   SOCIPC_ResetVEGAViaAthenaRight
// USAGE:  Toggle VEGA Reset Pin via Athena Right
//
// INPUT:  N/A
// OUTPUT:N/A
// GLOBALS:
//******************************************************************************
void SOCIPC_ResetVEGAViaAthenaRight(void)
{
   if(GetChipID_4K2K() == CHIPID_4K2K_R)
   {
      gm_ClearRegBitsWord(GPOUTPUT0, GPIO12_OUT);
      gm_Delay1ms(5);
      gm_SetRegBitsWord(GPOUTPUT0, GPIO12_OUT);
   }
}

//******************************************************************************
// FUNCTION:   SOCIPC_LoadSplashScreenColorSettings
// USAGE:  Load or Roll Back Color Settings during Splash Screen
//
// INPUT:  Load
// OUTPUT:N/A
// GLOBALS:
//******************************************************************************
void SOCIPC_LoadSplashScreenColorSettings(BOOL Load)
{
   BYTE CmdBuf[1] = {0};

#ifdef ST_4K2K_93xx_BOARD
      if(GetChipID_4K2K()==CHIPID_4K2K_R)
      {
#if ENABLE_SOC_IPC_BUSY_CHECK
         /*ChengLin 20130124: New 4k2k board BUSY status check*/
         if(!SocIPCIsSlaveLeftReady())
            return;
#endif
           CmdBuf[0] = (BYTE)Load;        
           SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_LOAD_SPLASH_SETTING_CTRL, CmdBuf); 
      }
#endif		

   if(Load)
      SOCIPC_CheckColorSettingBeforeSplashScreen();
   else
      SOCIPC_RollBackColorSettingAfterSplashScreen();
}

//******************************************************************************
// FUNCTION:   SOCIPC_CheckColorSettingBeforeSplashScreen
// USAGE:  Load Default Settings for Splash Screen
//
// INPUT:  N/A
// OUTPUT:N/A
// GLOBALS:
//******************************************************************************
void SOCIPC_CheckColorSettingBeforeSplashScreen(void)
{

   check_default_setting_msg("\n<Splash Screen Info> Set Default Color Settings.\n",0);

   //Load Default Setting for Splash Screen   
   TempBrightness = UserPrefBrightness;
   TempContrast = UserPrefContrast;   
   TempSharpness = SocIPCGetCorrectSharpnessSetting();
   TempColorTemp = UserPrefColorTemp;
   TempImageScheme = UserPrefImageScheme;

   //Set Default Image Scheme Settings
   UserPrefImageScheme = USER;
   SOCIPC_SetClearIgnoreInterChipCmdFlag(TRUE);
   AdjustImageScheme();
   SOCIPC_SetClearIgnoreInterChipCmdFlag(FALSE);

   //Set Default Values for Color Settings
   UserPrefBrightness = SchemeFactoryDefaultInROM[USER].Brightness;
   UserPrefContrast = SchemeFactoryDefaultInROM[USER].Contrast;   
   UserPrefColorTemp = SchemeFactoryDefaultInROM[USER].ColorTemp;
   CscUpdateColors(CH_A);

   //Set Default Value for Sharpness
   SocIPCSetCorrectSharpnessSetting(12);
#if (FEATURE_LOAD_RGB_COEFF == DISABLE)
   if (gm_TimingIsStable(CH_A) == TRUE)
      gm_SetScalerSharpness(CH_A, UserPrefSharpness);   
#else
   if ( gm_GetChannelInputAttr(CH_A, gmd_COLOR_SPACE) != gmd_RGB )
   {
      if (gm_TimingIsStable(CH_A) == TRUE)
         gm_SetScalerSharpness(CH_A, UserPrefSharpness);

   }
   else if( gm_GetChannelInputAttr(CH_A, gmd_COLOR_SPACE) == gmd_RGB )
   {
      if (gm_TimingIsStable(CH_A) == TRUE)
         gm_SetScalerSharpness(CH_A, UserPrefSharpnessRGB); 
   }
#endif

}

//******************************************************************************
// FUNCTION:   SOCIPC_RollBackColorSettingAfterSplashScreen
// USAGE:  Roll Back to User Settings for Splash Screen
//
// INPUT:  N/A
// OUTPUT:N/A
// GLOBALS:
//******************************************************************************
void SOCIPC_RollBackColorSettingAfterSplashScreen(void)
{
   check_default_setting_msg("\n<Splash Screen Info> Roll Back Original Color Settings.\n",0);

   //Set Original Values for Color Settings
   UserPrefBrightness = TempBrightness;
   UserPrefContrast = TempContrast;   
   UserPrefColorTemp = TempColorTemp;
   CscUpdateColors(CH_A);

   //Set Original Value for Sharpness
   SocIPCSetCorrectSharpnessSetting(TempSharpness);
#if (FEATURE_LOAD_RGB_COEFF == DISABLE)
   if (gm_TimingIsStable(CH_A) == TRUE)
      gm_SetScalerSharpness(CH_A, UserPrefSharpness);   
#else
   if ( gm_GetChannelInputAttr(CH_A, gmd_COLOR_SPACE) != gmd_RGB )
   {
      if (gm_TimingIsStable(CH_A) == TRUE)
         gm_SetScalerSharpness(CH_A, UserPrefSharpness);

   }
   else if( gm_GetChannelInputAttr(CH_A, gmd_COLOR_SPACE) == gmd_RGB )
   {
      if (gm_TimingIsStable(CH_A) == TRUE)
         gm_SetScalerSharpness(CH_A, UserPrefSharpnessRGB); 
   }
#endif

   //Set Original Image Scheme Settings
   UserPrefImageScheme = TempImageScheme;
   SOCIPC_SetClearIgnoreInterChipCmdFlag(TRUE);
   AdjustImageScheme();
   SOCIPC_SetClearIgnoreInterChipCmdFlag(FALSE);

}

//******************************************************************************
// FUNCTION:   SOCIPC_SetClearIgnoreInterChipCmdFlag
// USAGE:  Flag for ignore Inter-Chip command process
//
// INPUT:  Status
// OUTPUT:N/A
// GLOBALS:
//******************************************************************************
void SOCIPC_SetClearIgnoreInterChipCmdFlag(BOOL Status)
{
   IgnoreInterChipCmd = Status;
}

#if ENABLE_VEGA_ISP_CMD_VIA_UART_ISP_PORT 
//******************************************************************************
// FUNCTION:   SOCIPC_SetAthenaFE_L_DebugFlag
// USAGE:  Enable Athena FE or Left Debug Mode Flag
//
// INPUT:  DevID
// OUTPUT:N/A
// GLOBALS:
//******************************************************************************
void SOCIPC_SetAthenaFE_L_DebugFlag(BYTE DevID)
{
	//Added by ChengLin 20130226
#ifdef ST_4K2K_93xx_BOARD
	if(GetChipID_4K2K()==CHIPID_4K2K_R)
	{
#if ENABLE_SOC_IPC_BUSY_CHECK
		/*ChengLin 20130124: New 4k2k board BUSY status check*/
		if(!SocIPCIsSlaveLeftReady())
			return;
#endif   
		SocIPCSendCmdPacketParser(DevID, SICMD_RW_ATHENA_FE_L_DEBUG_MODE_EN_CTRL, NULL);
	}
#endif
}

//******************************************************************************
// FUNCTION:   SOCIPC_VEGA_ISP_Enable
// USAGE:  Enable VEGA ISP Mode via UART ISP Port
//
// INPUT:  N/A
// OUTPUT:N/A
// GLOBALS:
//******************************************************************************
void SOCIPC_VEGA_ISP_Enable(void)
{
#ifdef ST_4K2K_93xx_BOARD
  if(GetChipID_4K2K()==CHIPID_4K2K_R)
  {
     //Step 1: Switch UART to Right First for VEGA Reset Pin Toggle
     gm_ClearRegBitsWord(GPOUTPUT0, (GPIO6_OUT | GPIO9_OUT));

     //Step 2: VEGA BS_4 to Low
     gm_SetRegBitsWord(GPOUTPUT4,GPIO66_OUT); //GPIO66 to High (BS_4 Low)
     gm_Delay1ms(5);

     //Step 3: Reset VEGA
     SOCIPC_ResetVEGAViaAthenaRight();  
     gm_Delay1ms(10);

     //Step 4: Athena FE go to Debug Mode
     SOCIPC_SetAthenaFE_L_DebugFlag(SOCIPC_DEVICEFE_DST_ADDRESS);
     gm_Delay1ms(10);

     //Step 5: Athena LEFT go to Debug Mode
     SOCIPC_SetAthenaFE_L_DebugFlag(SOCIPC_DEVICEL_DST_ADDRESS);
     gm_Delay1ms(10);

     //Step 6: Athena Right go to debug Mode
     gmvb_DebugOnly = TRUE; 

     gm_Print("\nVEGA ISP Mode Enabled - Please assign batch file on GProbe for VEGA ISP.......\n",0);
     gm_Delay1ms(10);   

     //Step 7: Switch UART to VEGA
     gm_SetRegBitsWord(GPOUTPUT0, (GPIO6_OUT | GPIO9_OUT));
  }
#endif

}
#endif

#ifdef SMART_ISP
//******************************************************************************
// FUNCTION:   SocIPCGetSlaveSmartIspStatus
// USAGE:  Get Athena LEFT or Athena FE SmartIsp Status
//
// INPUT:  DevID
// OUTPUT:	TRUE or FALSE
// GLOBALS:
//******************************************************************************
BOOL SocIPCGetSlaveSmartIspStatus(BYTE DevID)
{
	if((DevID != SOCIPC_DEVICEL_DST_ADDRESS) && (DevID != SOCIPC_DEVICEFE_DST_ADDRESS))
		return FALSE;
	
	//gAthena_LEFT_FE_Slave_Parameters.SmartIsp = 0;	

	if( SocIPCSendCmdPacketParser(DevID, SICMD_RO_SMARTISP_STATUS, NULL) == FALSE)
	{	
		//gm_Print("\n<Soc IPC Error> Read Slave SmartIsp Status Failure.\n",0);
		return FALSE;
	}

	return TRUE;
}


#endif

#endif

//*********************************  END  **************************************

