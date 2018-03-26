/*
	$Workfile:   SocIPC.h  $
	$Revision: 1.68 $
	$Date: 2013/05/23 06:31:39 $
*/
#define __SOCPIC_H__
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
// MODULE:      SocIPC.h
//
// USAGE:       Header file for Soc IPC communication definition
//
//******************************************************************************

#if ((FEATURE_SOCIPC == ENABLE) || defined(FEATURE_HOSTIF))

#ifdef FEATURE_HOSTIF
#define ENABLE_SOC_IPC_BUSY_CHECK 0
#else
#define ENABLE_SOC_IPC_BUSY_CHECK 1
#endif

/*ChengLin 20130129: Enable Bidirectional BUSY Status Check*/
#if(ENABLE_SOC_IPC_BUSY_CHECK)
#define ENABLE_BIDIRECTIONAL_BUSY_CHECK 1
#else
#define ENABLE_BIDIRECTIONAL_BUSY_CHECK 0
#endif

/*ChengLin 20130123: Enable the communication between Athena Right/Left(Slave) and Athena FE(Master)*/
#define ENABLE_SOC_IPC_LEFT_RIGHT_SLAVE_INT2_FROM_FE_MASTER 1 //0

/*ChengLin 20120208: Enable SocIPC Keypad Process via Queue Buffer*/
#define ENABLE_SOC_IPC_KEYPAD_VIA_QUEUE 0

/*ChengLin 20120209: Enable SocIPC IR Process via Queue Buffer*/
#define ENABLE_SOC_IPC_IR_VIA_QUEUE 0

/*ChengLin 20120301: Enable OSD Settings Sync for 3 Athena Platform*/
#define ENABLE_SOC_IPC_OSD_SYNC 0

/*ChengLin 20120301: Enable SocIPC Get I2C Read Byte Length via SocIPC Recv Packet Length*/
#define ENABLE_GET_I2C_READ_BYTE_VIA_SOCIPC_RECV_PACKET	1

/*ChengLin 20130124: Enable Athena Rtight Master Interrupt Handle*/
#define ENABLE_ATHENA_RIGHT_INT1_HANDLE 1

/*ChengLin 20130124: Enable Athena FE/LEFT Slave Interrupt Handle*/
#define ENABLE_ATHENA_FE_LEFT_INT1_HANDLE 0

/*ChengLin 20130131: Enable VEGA I2C Control Via Athena FE*/
#define ENABLE_VEGA_CONTROL_VIA_ATHENA_FE	1

/*ChengLin 20130205: Enable VEGA CSC program by HDMI input*/
#define ENABLE_VEGA_CSC_VIA_HDMI_INFO 1

/*ChengLin 20130219: Enable Image Sync Handler*/
#define ENABLE_ATHENA_R_L_IMAGE_SYNC_HANDLER	1

/*ChengLin 20130327: Enable VEGA Device Check During Athena FE AC ON*/
#define ENABLE_VEGA_AVAIL_CHECK_VIA_FE_AC_ON 1

/*ChengLin 20130412: Enable VEGA ISP Command via ISP UART connector*/
#define ENABLE_VEGA_ISP_CMD_VIA_UART_ISP_PORT 1

/*ChengLin 20130416: Enable DP MST Timeout Check and control own ready bit*/
#define ENABLE_DP_MST_TIMEOUT_CHECK_FOR_OWN_READY_BIT 1

/*ChengLin 20130425: Enable PBP Deep Sleep Check*/
#define ENABLE_PBP_ATHENA_L_DEEP_SLEEP_CHECK  1

/*ChengLin 20130507: Enable 2 Logical Port Deep Sleep Check*/
#define ENABLE_TWO_LOGICAL_PORTS_SLEEP_CHECK 1

//******************************************************************************
//  G L O B A L    VARIABLES
//******************************************************************************
//

typedef struct SoCIPCParametersStruct
{
	BYTE PowerStatus;
	/*ChengLin 20130110: Color Temp - User Settings*/
	WORD CTUserRGain;
	WORD CTUserGGain;
	WORD CTUserBGain;
	SBYTE CTUserROffset;
	SBYTE CTUserGOffset;
	SBYTE CTUserBOffset;		
	/*****************************************/		
	BYTE CableStatus;
	WORD ModeFlags;
	WORD HWidth;
	WORD VLength;
	WORD HTotal;
	WORD VTotal;
	WORD HStart;
	WORD VStart;
	WORD VFreq; // 20130312
	BYTE PortStatus;
	SBYTE Brightness;
	WORD Contrast;
	BYTE Sharpness;
	WORD ColorSettings;
	WORD VideoProgress;
	WORD NRSettings;
	WORD VideoProgress_1;
	WORD DisplaySetup;
	WORD Audio1;
	WORD Audio2;
	WORD Setup;
	BYTE OSDSync;
	BYTE DDC_VCPCode;
	WORD DDC_Value;
	BYTE DDC_VCP_Type;
	WORD DDC_VCP_Max;
	WORD SlaveAddress;
	WORD SlaveWriteData;
	WORD SlaveReadData;
	BYTE DP_Switch;
	WORD DcrResult;
/*ChengLin 20130110: 6-axis Saturation, Hue, Brightness controls*/
	SBYTE SixAxisSatRed;
	SBYTE SixAxisSatGreen;
	SBYTE SixAxisSatBlue;
	SBYTE SixAxisSatYellow;
	SBYTE SixAxisSatMagenta;
	SBYTE SixAxisSatCyan;
	SBYTE SixAxisSatBlack;
	SBYTE SixAxisSatWhite;
	SBYTE SixAxisHueRed;
	SBYTE SixAxisHueGreen;
	SBYTE SixAxisHueBlue;
	SBYTE SixAxisHueYellow;
	SBYTE SixAxisHueMagenta;
	SBYTE SixAxisHueCyan;
	SBYTE SixAxisHueBlack;
	SBYTE SixAxisHueWhite;
	SBYTE SixAxisBrightRed;
	SBYTE SixAxisBrightGreen;
	SBYTE SixAxisBrightBlue;
	SBYTE SixAxisBrightYellow;
	SBYTE SixAxisBrightMagenta;
	SBYTE SixAxisBrightCyan;
	SBYTE SixAxisBrightBlack;
	SBYTE SixAxisBrightWhite;
	BYTE CaptureMain; //20130117
	BYTE DPRX0_MstInfo;
   BYTE DPRX0_IsStable;    
	BYTE DPRX0_ReModeSetup;
/*****************************************/
/*ChengLin 20130128: New Sync Handler State Control*/	
	BYTE ImageSyncState;
/*********************************************/
/*ChengLin 20130222: Add for sync Left/Right PB Control Status*/
#if ENABLE_ATHENA_R_L_IMAGE_SYNC_HANDLER
	BYTE ImageSyncBlackScreen;
#endif
#ifdef SMART_ISP
   BYTE SmartIspCtrl;
   BYTE SmartIspStep;
   BYTE SmartIspStatus;
#endif
   BYTE SplashLoadDefault; //Add by ChengLin 20130322
	BYTE GammaCorrection;
   BYTE AthenaFE_L_DebugMode; //Add by ChengLin 20130411
} SoCIPCPara_t;

extern SoCIPCPara_t gAthena_RIGHT_Master_Parameters;


extern SoCIPCPara_t gAthena_LEFT_FE_Slave_Parameters;


#if ENABLE_SOC_IPC_BUSY_CHECK
extern BOOL gAthena_LEFT_FE_BUSY;
#endif


extern BOOL gGetAckFromSlave;



//******************************************************************************
//  SOCIPC RELATED DEBUG INFO DEFINITIONS
//******************************************************************************
//

#define ENABLE_SOC_IPC_DEBUG 	0
#define ENABLE_SOC_IPC_RECV_DEBUG 	0
#define ENABLE_SOC_IPC_BUSY_CHECK_DEBUG	0
#define ENABLE_SOC_IPC_KEYPAD_IR_QUEUE_DEBUG	0
#define ENABLE_SOC_IPC_KEYPAD_IR_DEBUG	0
#define ENABLE_SOC_IPC_OSD_SYNC_DEBUG	0
#define ENABLE_SOC_IPC_DDC_PROC_DEBUG	0
#define ENABLE_IMAGE_SYNC_HANDLER_DEBUG   0
#define ENABLE_DEFAULT_SETTING_CHECK_DEBUG   0
#define ENABLE_VEGA_AVAIL_CHECK_DEBUG   0
#define ENABLE_VEGA_CONTROL_VIA_FE_DEBUG   0


#if ENABLE_SOC_IPC_DEBUG
#define socipc_msg(a,b) gm_Print(a,b)
#define socipc_msg_ex(a,b,c) gm_Print(a,b,c)
#else
#define socipc_msg(a,b)
#endif

#if ENABLE_SOC_IPC_RECV_DEBUG
#define socipc_recv_msg(a,b) gm_Print(a,b)
#else
#define socipc_recv_msg(a,b)
#endif

#if ENABLE_SOC_IPC_BUSY_CHECK_DEBUG
#define socipc_busychk_msg(a,b) gm_Print(a,b)
#else
#define socipc_busychk_msg(a,b)
#endif

#if ENABLE_SOC_IPC_KEYPAD_IR_QUEUE_DEBUG
#define socipc_kbd_ir_queue_msg(a,b) gm_Print(a,b)
#else
#define socipc_kbd_ir_queue_msg(a,b)
#endif

#if ENABLE_SOC_IPC_KEYPAD_IR_DEBUG
#define socipc_kbd_ir_msg(a,b) gm_Print(a,b)
#else
#define socipc_kbd_ir_msg(a,b)
#endif

#if ENABLE_SOC_IPC_OSD_SYNC_DEBUG
#define socipc_osd_sync_msg(a,b) gm_Print(a,b)
#else
#define socipc_osd_sync_msg(a,b)
#endif

#if ENABLE_SOC_IPC_DDC_PROC_DEBUG
#define socipc_ddc_proc_msg(a,b) gm_Print(a,b)
#else
#define socipc_ddc_proc_msg(a,b)
#endif

#if ENABLE_IMAGE_SYNC_HANDLER_DEBUG
#define ImageSyncHandler_msg(a,b) gm_Print(a,b)
#else
#define ImageSyncHandler_msg(a,b)
#endif

#if ENABLE_DEFAULT_SETTING_CHECK_DEBUG
#define check_default_setting_msg(a,b) gm_Print(a,b)
#else
#define check_default_setting_msg(a,b)
#endif

#if ENABLE_VEGA_AVAIL_CHECK_DEBUG
#define check_VEGA_available_msg(a,b) gm_Print(a,b)
#else
#define check_VEGA_available_msg(a,b)
#endif

#if ENABLE_VEGA_CONTROL_VIA_FE_DEBUG
#define VEGA_ctrl_via_FE_msg(a,b) gm_Print(a,b)
#else
#define VEGA_ctrl_via_FE_msg(a,b)
#endif

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************
//

#define SOC_IPC_RECV_RETRY_TIME 30

#define SOC_IPC_TRX_BUF_SIZE 	131
#define SOCIPC_TRANS_MSG_MAX_SIZE	(SOC_IPC_TRX_BUF_SIZE-9) //123
#define SOCIPC_STD_MSG_LEN	0x03 //Length+1
#define SOCIPC_TRANS_MSG_LEN(LEN)	(LEN+SOCIPC_STD_MSG_LEN)

#define SOCIPC_DEVICEL_DST_ADDRESS 	0x7C
#define SOCIPC_DEVICEFE_DST_ADDRESS 	0x7E
#define HOSTIF_DEVICE_DST_ADDRESS 	0xC6
#define SOCIPC_VEGA_DEVICE_ADDRESS 0xE6 //Added by ChengLin 20130131

#define SOCIPC_TX_SRC_ADDRESS 		0x51
#define SOC_IPC_TX_VCP_PREFIX_CMD 		0xC0
#define SOC_IPC_TX_VCP_PREFIX_CMD_PAGE 	0x0000


#define I2C_SLAVE_REG_OFFSET	(I2C_SLAVE2_CTRL1 - I2C_SLAVE1_CTRL1)
#define I2C_SLAVE_CTRL2			(I2C_SLAVE1_CTRL2 		+ gB_I2cSlaveSel * I2C_SLAVE_REG_OFFSET)


#define I2C_SOC_IPC_CMD(CH)				Sp_I2CSlavePtrGlobal[CH]->Buffer[2]
#define I2C_SOC_ManufacturerID(CH)		((((*(WORD*)(&(Sp_I2CSlavePtrGlobal[CH]->Buffer[3])))<<8)&0xFF00)|(((*(WORD*)(&(Sp_I2CSlavePtrGlobal[CH]->Buffer[3])))>>8)&0x00FF))

#define Start_of_SoC_IPC_Cmd_in_DDC2BiMsg   5   // 3 (device id + length + cmd) + 2 (manufacturer id) 

#if ENABLE_SOC_IPC_KEYPAD_VIA_QUEUE
#define KEYPAD_BUFFER_MAX 40
#endif

#if ENABLE_SOC_IPC_IR_VIA_QUEUE
#define IR_BUFFER_MAX 40
#endif

typedef enum
{
   I2C_SLAVE_STATE_FREE = 0,
   I2C_SLAVE_STATE_STOP_RD,
   I2C_SLAVE_STATE_STOP_WR,
   I2C_SLAVE_STATE_WR_STRETCH,
   I2C_SLAVE_STATE_RD_STRETCH,
   I2C_SLAVE_STATE_RD,
   I2C_SLAVE_STATE_WR,
   I2C_SLAVE_STATE_ERR,
   I2C_SLAVE_STATE_END
}I2C_SLAVE_STATE_ENUM;

typedef enum RecvAckNackDataStruct
{
   RECV_ACK = 0,
   RECV_NACK,   	
   RECV_DATA,
   RECV_NOTHING
}RecvAckNackData_t;


// SOC IPC COMMAND CODES
//
#define SICMD_NAK		0x0B
#define SICMD_ACK		0x0C
#define SICMD_INVALID	0xFF

//******************************************************************************
//         SOC IPC  C O M M A N D   S T R U C T U R E S
//******************************************************************************
typedef enum
{
//TODO: Add new commands in here!!
	SICMD_RW_Power_CONTROL			= 0x01,
	SICMD_RO_Power_Status			= 0x02,
/*ChengLin 20130110: Color Temp - User Settings*/
	SICMD_RW_CT_RedGain_Ctrl		= 0x03,
	SICMD_RW_CT_GreenGain_Ctrl		= 0x04,
	SICMD_RW_CT_BlueGain_Ctrl		= 0x05,
	SICMD_RW_CT_RedOffset_Ctrl		= 0x06,
	SICMD_RW_CT_GreenOffset_Ctrl	= 0x07,
	SICMD_RW_CT_BlueOffset_Ctrl		= 0x08,		
/*****************************************/
	SICMD_RW_Cables_Status			= 0x11,	
	SICMD_RW_Mode_Info_CONTRL 		= 0x21,
	SICMD_RW_Mode_Info_STATUS 		= 0x22,
	SICMD_RW_Timing_H_Width 		= 0x23,
	SICMD_RW_Timing_V_Length 		= 0x24,
	SICMD_RW_Timing_H_Total 		= 0x25,
	SICMD_RW_Timing_V_Total 		= 0x26,
	SICMD_RW_Timing_H_Start 		= 0x27,
	SICMD_RW_Timing_V_Start 		= 0x28,
	SICMD_RO_Timing_H_Width_Slave 	= 0x29,
	SICMD_RO_Timing_V_Length_Slave 	= 0x2A,
	SICMD_RO_Timing_H_Total_Slave 	= 0x2B,
	SICMD_RO_Timing_V_Total_Slave 	= 0x2C,
	SICMD_RO_Timing_H_Start_Slave 	= 0x2D,
	SICMD_RO_Timing_V_Start_Slave 	= 0x2E,
	SICMD_RO_Timing_V_Freq_Slave 	= 0x2F, // 20130312
	SICMD_RW_Port_CONTRL 			= 0x41,
	SICMD_RO_Port_Status 			= 0x42,
	SICMD_RW_Signal_Sync_CONTRL 	= 0x51,
	SICMD_RO_Signal_Sync_Status 	= 0x52,
	SICMD_RW_Brightness_CONTRL 		= 0x61,
	SICMD_RO_Brightness_Status 		= 0x62,
	SICMD_RW_Contrast_CONTRL 		= 0x63,
	SICMD_RO_Contrast_Status 		= 0x64,
	SICMD_RW_Sharpness_CONTRL 		= 0x65,
	SICMD_RO_Sharpness_Status 		= 0x66,
	SICMD_RW_Color_CONTRL 			= 0x67,
	SICMD_RO_Color_Status 			= 0x68,
	SICMD_RW_Video_progress_CONTRL 	= 0x69,
	SICMD_RO_Video_progress_Status 	= 0x6A,
	SICMD_RW_NR_CONTRL 				= 0x6B,
	SICMD_RO_NR_Status 				= 0x6C,
	SICMD_RW_Video_progress1_CONTRL = 0x6D,
	SICMD_RO_Video_progress1_Status = 0x6E,
	SICMD_RW_DisplaySetup_CONTRL 	= 0x6F,
	SICMD_RO_DisplaySetup_Status 	= 0x70,
	SICMD_RW_Audio1_CONTRL 			= 0x71,
	SICMD_RO_Audio1_Status 			= 0x72,
	SICMD_RW_Audio2_CONTRL 			= 0x73,
	SICMD_RO_Audio2_Status 			= 0x74,
	SICMD_RW_Setup_CONTRL 			= 0x75,
	SICMD_RO_Setup_Status 			= 0x76,
	SICMD_RW_PBP_CONTRL = 0x77,	// 20130117
	SICMD_RO_DCR_Result           = 0x78,
	SICMD_RW_OSD_Sync_CONTRL 		= 0x91,
	SICMD_RO_OSD_Sync_Status 		= 0x92,
	SICMD_RO_OSD_Sync_changes 		= 0x93,
	SICMD_RW_OSD_DP_Switch			= 0x94,
	SICMD_RW_DDCI_CONTRL 			= 0xA1,
	SICMD_RO_DDCI_Status 			= 0xA2,
/*ChengLin 20130110: 6-axis Saturation, Hue, Brightness controls*/
	SICMD_RW_6Axis_Sat_Red_Ctrl		= 0xA3,
	SICMD_RW_6Axis_Sat_Green_Ctrl	= 0xA4,	
	SICMD_RW_6Axis_Sat_Blue_Ctrl	= 0xA5,
	SICMD_RW_6Axis_Sat_Yellow_Ctrl	= 0xA6,
	SICMD_RW_6Axis_Sat_Magenta_Ctrl	= 0xA7,	
	SICMD_RW_6Axis_Sat_Cyan_Ctrl	= 0xA8,
	SICMD_RW_6Axis_Sat_Black_Ctrl	= 0xA9,	
	SICMD_RW_6Axis_Sat_White_Ctrl	= 0xAA,
	SICMD_RW_6Axis_Hue_Red_Ctrl 	= 0xAB,
	SICMD_RW_6Axis_Hue_Green_Ctrl	= 0xAC, 
	SICMD_RW_6Axis_Hue_Blue_Ctrl	= 0xAD,
	SICMD_RW_6Axis_Hue_Yellow_Ctrl	= 0xAE,
	SICMD_RW_6Axis_Hue_Magenta_Ctrl = 0xAF, 
	SICMD_RW_6Axis_Hue_Cyan_Ctrl	= 0xB0,
	SICMD_RW_6Axis_Hue_Black_Ctrl	= 0xB1, 
	SICMD_RW_6Axis_Hue_White_Ctrl	= 0xB2,
	SICMD_RW_6Axis_Bright_Red_Ctrl 		= 0xB3,
	SICMD_RW_6Axis_Bright_Green_Ctrl	= 0xB4, 
	SICMD_RW_6Axis_Bright_Blue_Ctrl		= 0xB5,
	SICMD_RW_6Axis_Bright_Yellow_Ctrl	= 0xB6,
	SICMD_RW_6Axis_Bright_Magenta_Ctrl	= 0xB7, 
	SICMD_RW_6Axis_Bright_Cyan_Ctrl		= 0xB8,
	SICMD_RW_6Axis_Bright_Black_Ctrl	= 0xB9, 
	SICMD_RW_6Axis_Bright_White_Ctrl	= 0xBA,
/*****************************************/
//inter-chip SMT communication
//refer to doc "4Kx2K System FW SMT Design Gamma Calibration and Uniformity Compensation"
	SICMD_WO_SMT_COMMAND                = 0xBB,
//
	SICMD_RW_DpRx0_MstInfo           = 0xBC,
	SICMD_RO_DpRx0_IsStable          = 0xBD, 	
	SICMD_RW_DPRx0_ReModeSetup       = 0xBE,	
	SICMD_RW_Keypad_CONTRL 			= 0xD1,
	SICMD_RW_IR_CONTROL 			= 0xD2,
/*ChengLin 20130128: New Sync Handler State Control*/	
	SICMD_RW_IMAGE_SYNC_STATE_Ctrl 			= 0xD3,
	SICMD_RW_IMAGE_SYNC_STATE_Status 		= 0xD4,
/*********************************************/
/*ChengLin 20130222: Add for sync Left/Right PB Control Status*/
#if ENABLE_ATHENA_R_L_IMAGE_SYNC_HANDLER
	SICMD_RW_IMAGE_SYNC_BLACK_SCREEN_CTRL	= 0xD5,
#endif
	SICMD_RW_GAMMA_CORRECTION_CONTRL 		= 0xD6, //Add by ChengLin 20130226
#if ENABLE_VEGA_ISP_CMD_VIA_UART_ISP_PORT 	
   SICMD_RW_ATHENA_FE_L_DEBUG_MODE_EN_CTRL	= 0xD7, //Add by ChengLin  20130411	
#endif   
#ifdef SMART_ISP
   SICMD_RW_SMARTISP_CTRL	               = 0xD8,
   SICMD_RO_SMARTISP_STATUS               = 0xD9,
#endif
   SICMD_RW_LOAD_SPLASH_SETTING_CTRL      = 0xDA, //Add by ChengLin 20130322
	SICMD_RW_Slave_Registers_addr_CONTRL 	= 0xE1,
	SICMD_RW_Slave_Registers_data_CONTRL 	= 0xE2,
	SICMD_RO_Slave_Registers_read_data 		= 0xE3,
	SICMD_RW_Reserved_Registers_CONTRL 		= 0xF1,
	SICMD_RO_Reserved_Registers_Status 		= 0xF2,	
	SICMD_RW_SaveModeIndepedent_Ctrl 		= 0xF3,	
} gmt_SocIPCCommandCode;

typedef enum
{
//TODO: Add new AppsTest commands in here!!
// For Athena_4K2K_Chip_R
	SIAPPTESTCMD_KEYPAD_TEST					= 0,
	SIAPPTESTCMD_IR_TEST						= 1,
#if ENABLE_SOC_IPC_OSD_SYNC
	SIAPPTESTCMD_OSD_SYNC_TEST					= 2,
#endif
	SIAPPTESTCMD_SEND_CMD_TEST					= 3,
	SIAPPTESTCMD_DDC_TEST						= 4,
	SIAPPTESTCMD_DEV_CHK_TEST					= 5,
	SIAPPTESTCMD_KEYPAD_BURST_SEND_CMD_TEST		= 6,
	SIAPPTESTCMD_REG_WRITE_TEST					= 7,
	SIAPPTESTCMD_REG_READ_TEST					= 8,
	SIAPPTESTCMD_SLAVE_CURRENT_INFO_TEST		= 9,	
	SIAPPTESTCMD_MASTER_CURRENT_INFO_TEST		= 10,	
	SIAPPTESTCMD_SET_AND_GET_SHARPNESS_TEST		= 11,		
	
   //Athena LEFT or Athena FE Test commands

	SIAPPTESTCMD_INT2_TRIGGER_TEST				= 12,
	SIAPPTESTCMD_VEGA_I2C_WRITE_VIA_FE			= 13,
	SIAPPTESTCMD_VEGA_I2C_READ_VIA_FE			= 14,		
	SIAPPTESTCMD_VEGA_HW_REG_WRITE_VIA_FE		= 15,
	SIAPPTESTCMD_VEGA_HW_REG_READ_VIA_FE		= 16,
	SIAPPTESTCMD_VEGA_HW_RESET_VIA_RIGHT		= 17,		
#if ENABLE_VEGA_ISP_CMD_VIA_UART_ISP_PORT 
   SIAPPTESTCMD_EN_VEGA_ISP_MODE          = 18, 
#endif
} gmt_SocIPCAppsTestCommands;

/*ChengLin 20130108: OSD Sync Function for 4kx2k Platform*/


typedef enum
{
	SI_OSD_SYNC_BRIGHTNESS			= 0,
	SI_OSD_SYNC_CONTRAST			= 1,		
	SI_OSD_SYNC_SHARPNESS			= 2,
	SI_OSD_SYNC_SHARPNESS_RGB		= 3,		
	SI_OSD_SYNC_COLOR_CTRL			= 4,
	SI_OSD_SYNC_VIDEO_PROCESSING	= 5,
	SI_OSD_SYNC_VIDEO_PROCESSING_2	= 6,
	SI_OSD_SYNC_NOISE_REDUCTION		= 7,	
	SI_OSD_SYNC_DISPLAY_SETUP		= 8,	
	SI_OSD_SYNC_AUDIO				= 9,
	SI_OSD_SYNC_AUDIO_2				= 10,
	SI_OSD_SYNC_SETUP 				= 11,
	SI_OSD_SYNC_DP_SWITCH 			= 12,
	SI_OSD_SYNC_SEL_INPUT_MAIN 		= 13,
	SI_OSD_SYNC_CT_USER_RGAIN 		= 14,
	SI_OSD_SYNC_CT_USER_GGAIN 		= 15,
	SI_OSD_SYNC_CT_USER_BGAIN 		= 16,
	SI_OSD_SYNC_CT_USER_ROFFSET 	= 17,
	SI_OSD_SYNC_CT_USER_GOFFSET 	= 18,
	SI_OSD_SYNC_CT_USER_BOFFSET 	= 19,	
	SI_OSD_SYNC_SIX_AXIS_SAT_RED 		= 20,
	SI_OSD_SYNC_SIX_AXIS_SAT_GREEN 		= 21,
	SI_OSD_SYNC_SIX_AXIS_SAT_BLUE 		= 22,
	SI_OSD_SYNC_SIX_AXIS_SAT_YELLOW 	= 23,	
	SI_OSD_SYNC_SIX_AXIS_SAT_MAGENTA	= 24,
	SI_OSD_SYNC_SIX_AXIS_SAT_CYAN		= 25,
	SI_OSD_SYNC_SIX_AXIS_SAT_BLACK		= 26,
	SI_OSD_SYNC_SIX_AXIS_SAT_WHITE		= 27,
	SI_OSD_SYNC_SIX_AXIS_HUE_RED 		= 28,
	SI_OSD_SYNC_SIX_AXIS_HUE_GREEN 		= 29,
	SI_OSD_SYNC_SIX_AXIS_HUE_BLUE 		= 30,
	SI_OSD_SYNC_SIX_AXIS_HUE_YELLOW 	= 31,	
	SI_OSD_SYNC_SIX_AXIS_HUE_MAGENTA	= 32,
	SI_OSD_SYNC_SIX_AXIS_HUE_CYAN		= 33,
	SI_OSD_SYNC_SIX_AXIS_HUE_BLACK		= 34,
	SI_OSD_SYNC_SIX_AXIS_HUE_WHITE		= 35,
	SI_OSD_SYNC_SIX_AXIS_BRI_RED 		= 36,
	SI_OSD_SYNC_SIX_AXIS_BRI_GREEN 		= 37,
	SI_OSD_SYNC_SIX_AXIS_BRI_BLUE 		= 38,
	SI_OSD_SYNC_SIX_AXIS_BRI_YELLOW 	= 39,	
	SI_OSD_SYNC_SIX_AXIS_BRI_MAGENTA	= 40,
	SI_OSD_SYNC_SIX_AXIS_BRI_CYAN		= 41,
	SI_OSD_SYNC_SIX_AXIS_BRI_BLACK		= 42,
	SI_OSD_SYNC_SIX_AXIS_BRI_WHITE		= 43,
	SI_OSD_SYNC_SAVE_MODE_INDEPEDENT	= 44,	
	SI_OSD_SYNC_SAVE_PBP_Mode	= 45,	
} gmt_SocIPC_OSDSyncID;

/*ChengLin 20130128: New Sync Handler states*/
typedef enum
{
   IMAGE_LVDS_ON_STATE = 0,
   IMAGE_LVDS_OFF_STATE,
   IMAGE_OFF_STATE,
   IMAGE_RDY_WAIT_STATE,
   IMAGE_ON_STATE,
   IMAGE_LOST_WAIT_STATE,
   IMAGE_FAIL_STATE
}gmt_ImageSyncState;

typedef enum
{
   CT_USER_NONE 		= 0,
   CT_USER_RED_GAIN 	= 1,
   CT_USER_GREEN_GAIN 	= 2,
   CT_USER_BLUE_GAIN 	= 3,
   CT_USER_RED_OFFSET 	= 4,
   CT_USER_GREEN_OFFSET = 5,
   CT_USER_BLUE_OFFSET 	= 6   
}gmt_CTUserModeIndex;

typedef enum
{
	//Saturation
	SIX_AXIS_COLOR_NONE 		= 0,
	SIX_AXIS_COLOR_RED_SAT		= 1,
	SIX_AXIS_COLOR_GREEN_SAT	= 2,
	SIX_AXIS_COLOR_BLUE_SAT		= 3,
	SIX_AXIS_COLOR_YELLOW_SAT	= 4,
	SIX_AXIS_COLOR_MAGENTA_SAT	= 5,
	SIX_AXIS_COLOR_CYAN_SAT 	= 6,
	SIX_AXIS_COLOR_BLACK_SAT	= 7,
	SIX_AXIS_COLOR_WHITE_SAT 	= 8,

	//Hue
	SIX_AXIS_COLOR_RED_HUE		= 9,
	SIX_AXIS_COLOR_GREEN_HUE	= 10,
	SIX_AXIS_COLOR_BLUE_HUE		= 11,
	SIX_AXIS_COLOR_YELLOW_HUE	= 12,
	SIX_AXIS_COLOR_MAGENTA_HUE	= 13,
	SIX_AXIS_COLOR_CYAN_HUE 	= 14,
	SIX_AXIS_COLOR_BLACK_HUE	= 15,
	SIX_AXIS_COLOR_WHITE_HUE 	= 16,

	//Luma
	SIX_AXIS_COLOR_RED_LUMA		= 17,
	SIX_AXIS_COLOR_GREEN_LUMA	= 18,
	SIX_AXIS_COLOR_BLUE_LUMA	= 19,
	SIX_AXIS_COLOR_YELLOW_LUMA	= 20,
	SIX_AXIS_COLOR_MAGENTA_LUMA	= 21,
	SIX_AXIS_COLOR_CYAN_LUMA 	= 22,
	SIX_AXIS_COLOR_BLACK_LUMA	= 23,
	SIX_AXIS_COLOR_WHITE_LUMA 	= 24	
}gmt_SixAxisColorIndex;

typedef struct SocIPCTransPacketStruct
{
	BYTE DestDevSlaveAddr;
	BYTE SrcDevMasterAddr;
	BYTE TransPacketLength;
	BYTE VCPPrefixCmd;	
	BYTE VCPPrefixPage1;
	BYTE VCPPrefixPage2;
	BYTE MessageLength;
	BYTE MessageCmd;
	BYTE MessageBuf[SOCIPC_TRANS_MSG_MAX_SIZE];
	BYTE PacketChkSum;
} SocIPCTransPkt_t;

/*ChengLin 20130130: Update Flags for SOCIPC Slave LEFT/FE*/
typedef struct SOCIPC_SlaveUpdateStatus
{
	BYTE Update_CT_User_Index;
	WORD Update_PortControl			:1;
	WORD Update_Color				:1;	
	WORD Update_Sharpness			:1;	
	WORD Update_ColorTemperature	:1;	
	WORD Update_ImageScheme			:1;
	WORD Update_CSC					:1;		
	WORD Update_DUM					:1;
	WORD Update_DCDI				:1;
	WORD Update_MADI				:1;
	WORD Update_OD					:1;
	WORD Update_TNR					:1;
	WORD Update_MEPG_NR				:1;
	WORD Update_FilmMode			:1;
	WORD Update_AspectRatio			:1;
	WORD Update_DCR					:1;
	WORD Update_FactoryReset		:1;
	WORD Update_DPSwitch			:1;
	WORD Update_UserData			:1;
	WORD Update_DDCCI				:1;
	BYTE Update_SixAxisColor_Index;
/*ChengLin 20130222: Add for sync Left/Right PB Control Status*/
#if ENABLE_ATHENA_R_L_IMAGE_SYNC_HANDLER	
	WORD Update_PB_Ctrl				:1;	
#endif
	WORD Update_GammaCorrection		:1;
   WORD Update_SplashLoadDef      :1;    
   WORD Update_Redo_ModeSetup      :1;
//Add more update flag below
}SOCIPC_SlaveUpdateStatus_t;

typedef struct PowerControlFuncCmdStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	Power_Ctrl;
} PowerCtrlFuncCmd_t;

typedef struct PowerStatusCmdResponseStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	Power_Status;
} PowerStatusResPkt_t;

/*ChengLin 20130110: Color Temp - User Settings*/
typedef struct CTUserRGainCtrlFuncCmdStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	CT_User_RGain_HighByte;
   BYTE CT_User_RGain_LowByte;
} CTUserRGainCtrlFuncCmd_t;

typedef struct CTUserGGainCtrlFuncCmdStruct
{
	BYTE Length;
	BYTE Cmd;
	BYTE CT_User_GGain_HighByte;
	BYTE CT_User_GGain_LowByte;
} CTUserGGainCtrlFuncCmd_t;

typedef struct CTUserBGainCtrlFuncCmdStruct
{
	BYTE Length;
	BYTE Cmd;
	BYTE CT_User_BGain_HighByte;
	BYTE CT_User_BGain_LowByte;
} CTUserBGainCtrlFuncCmd_t;

typedef struct CTUserROffsetCtrlFuncCmdStruct
{
	BYTE Length;
	BYTE Cmd;
	BYTE CT_User_ROffset;
} CTUserROffsetCtrlFuncCmd_t;

typedef struct CTUserGOffsetCtrlFuncCmdStruct
{
	BYTE Length;
	BYTE Cmd;
	BYTE CT_User_GOffset;
} CTUserGOffsetCtrlFuncCmd_t;

typedef struct CTUserBOffsetCtrlFuncCmdStruct
{
	BYTE Length;
	BYTE Cmd;
	BYTE CT_User_BOffset;
} CTUserBOffsetCtrlFuncCmd_t;
/*****************************************/

typedef struct CableStatusFuncCmdStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	Reserved_Byte;
   BYTE CableStatus;
} CableStatusFuncCmd_t;

typedef struct ModeInfoCtrlFuncCmdStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	ModeInfo_HighByte;
   BYTE ModeInfo_LowByte;
} ModeInfoCtrlFuncCmd_t;

typedef struct ModeInfoStatusCmdResponseStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	ModeInfo_HighByte;
   BYTE	ModeInfo_LowByte;   
} ModeInfoStatusResPkt_t;

typedef struct TimingHWidthFuncCmdStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	HWidth_HighByte;
   BYTE HWidth_LowByte;
} TimingHWidthCtrlFuncCmd_t;

typedef struct TimingVLengthFuncCmdStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	VLength_HighByte;
   BYTE Vength_LowByte;
} TimingVLengthCtrlFuncCmd_t;

typedef struct TimingHTotalFuncCmdStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	HTotal_HighByte;
   BYTE HTotal_LowByte;
} TimingHTotalCtrlFuncCmd_t;

typedef struct TimingVTotalFuncCmdStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	VTotal_HighByte;
   BYTE VTotal_LowByte;
} TimingVTotalCtrlFuncCmd_t;

typedef struct TimingHStartFuncCmdStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	HStart_HighByte;
   BYTE HStart_LowByte;
} TimingHStartCtrlFuncCmd_t;

typedef struct TimingVStartFuncCmdStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	VStart_HighByte;
   BYTE VStart_LowByte;
} TimingVStartCtrlFuncCmd_t;

typedef struct TimingHWidthSlaveCmdResponseStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	HWidth_HighByte;
   BYTE	HWidth_LowByte;   
} TimingHWidthSlaveResPkt_t;

typedef struct TimingVLengthSlaveCmdResponseStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	VLength_HighByte;
   BYTE	VLength_LowByte;   
} TimingVLengthSlaveResPkt_t;

typedef struct TimingHTotalSlaveCmdResponseStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	HTotal_HighByte;
   BYTE	HTotal_LowByte;   
} TimingHTotalSlaveResPkt_t;

typedef struct TimingVTotalSlaveCmdResponseStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	VTotal_HighByte;
   BYTE	VTotal_LowByte;   
} TimingVTotalSlaveResPkt_t;

typedef struct TimingHStartSlaveCmdResponseStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	HStart_HighByte;
   BYTE	HStart_LowByte;   
} TimingHStartSlaveResPkt_t;

typedef struct TimingVStartSlaveCmdResponseStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	VStart_HighByte;
   BYTE	VStart_LowByte;   
} TimingVStartSlaveResPkt_t;

typedef struct TimingVFreqSlaveCmdResponseStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	VFreq_HighByte;
   BYTE	VFreq_LowByte; 
} TimingVFreqSlaveResPkt_t;


typedef struct PortCtrlFuncCmdStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	PortCtrlBits;
} PortCtrlFuncCmd_t;

typedef struct PortStatusCmdResponseStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	PortStatus;   
} PortStatusResPkt_t;

typedef struct BrightnessCtrlFuncCmdStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE Brightness;
} BrightnessCtrlFuncCmd_t;

typedef struct BrightnessStatusCmdResponseStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	Brightness;   
} BrightnessStatusResPkt_t;

typedef struct ChangeFlagStatusCmdResponseStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	ChangeFlag;   
} ChangeFlagStatusResPkt_t;


typedef struct ContrastCtrlFuncCmdStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	Contrast_HighByte;
   BYTE Contrast_LowByte;
} ContrastCtrlFuncCmd_t;

typedef struct ContrastStatusCmdResponseStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	Contrast_HighByte;
   BYTE	Contrast_LowByte;   
} ContrastStatusResPkt_t;

typedef struct SharpnessCtrlFuncCmdStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	Sharpness;
} SharpnessCtrlFuncCmd_t;

typedef struct SharpnessStatusCmdResponseStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	Sharpness;   
} SharpnessStatusResPkt_t;

typedef struct ColorCtrlFuncCmdStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	ColorSettings_HighByte;
   BYTE ColorSettings_LowByte;
} ColorCtrlFuncCmd_t;

typedef struct ColorStatusCmdResponseStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	ColorSettings_HighByte;
   BYTE	ColorSettings_LowByte;   
} ColorStatusResPkt_t;

typedef struct VideoProgressCtrlFuncCmdStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	VideoProgress_HighByte;
   BYTE VideoProgress_LowByte;
} VideoProgressCtrlFuncCmd_t;

typedef struct VideoProgressStatusCmdResponseStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	VideoProgress_HighByte;
   BYTE	VideoProgress_LowByte;   
} VideoProgressStatusResPkt_t;

typedef struct NRCtrlFuncCmdStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	NR_HighByte;
   BYTE NR_LowByte;
} NRCtrlFuncCmd_t;

typedef struct NRStatusCmdResponseStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	NR_HighByte;
   BYTE	NR_LowByte;   
} NRStatusResPkt_t;

typedef struct VideoProgress_1CtrlFuncCmdStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	VideoProgress_1_HighByte;
   BYTE VideoProgress_1_LowByte;
} VideoProgress_1CtrlFuncCmd_t;

typedef struct VideoProgress_1StatusCmdResponseStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	VideoProgress_1_HighByte;
   BYTE	VideoProgress_1_LowByte;   
} VideoProgress_1StatusResPkt_t;

typedef struct DisplaySetupCtrlFuncCmdStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	DisplaySetup_HighByte;
   BYTE  DisplaySetup_LowByte;
   BYTE  UpdateFlag; //ChengLin 20130315: Special for Splash screen to update userpref data only
} DisplaySetupCtrlFuncCmd_t;

typedef struct DisplaySetupStatusCmdResponseStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	DisplaySetup_HighByte;
   BYTE	DisplaySetup_LowByte;   
} DisplaySetupStatusResPkt_t;

typedef struct Audio1CtrlFuncCmdStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	Audio1_HighByte;
   BYTE Audio1_LowByte;
} Audio1CtrlFuncCmd_t;

typedef struct Audio1StatusCmdResponseStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	Audio1_HighByte;
   BYTE	Audio1_LowByte;   
} Audio1StatusResPkt_t;

typedef struct Audio2CtrlFuncCmdStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	Audio2_HighByte;
   BYTE Audio2_LowByte;
} Audio2CtrlFuncCmd_t;

typedef struct Audio2StatusCmdResponseStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	Audio2_HighByte;
   BYTE	Audio2_LowByte;   
} Audio2StatusResPkt_t;

typedef struct SetupCtrlFuncCmdStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	Setup_HighByte;
   BYTE Setup_LowByte;
} SetupCtrlFuncCmd_t;

typedef struct SetupStatusCmdResponseStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	Setup_HighByte;
   BYTE	Setup_LowByte;   
} SetupStatusResPkt_t;

typedef struct MainCapCtrlFuncCmdStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE CaptureMain;
} MainCapCtrlFuncCmd_t;

typedef struct OSDSyncCtrlFuncCmdStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	OSDSync;
} OSDSyncCtrlFuncCmd_t;

typedef struct OSDSyncStatusCmdResponseStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	OSDSync;   
} OSDSyncStatusResPkt_t;

typedef struct DcrResultCmdResponseStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	DcrDat_HighByte;
   BYTE	DcrDat_LowByte;
} DcrResultResPkt_t;

typedef struct DPSwitchCtrlFuncCmdStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE DP_Switch;
} DPSwitchCtrlFuncCmd_t;

/*ChengLin 20130110: 6-axis Saturation, Hue, Brightness controls*/
typedef struct SixAxisSatRedCtrlFuncCmdStruct
{
	BYTE Length;
	BYTE Cmd;
	BYTE SixAxisSatRed;
} SixAxisSatRedCtrlFuncCmd_t;

typedef struct SixAxisSatGreenCtrlFuncCmdStruct
{
	BYTE Length;
	BYTE Cmd;
	BYTE SixAxisSatGreen;
} SixAxisSatGreenCtrlFuncCmd_t;

typedef struct SixAxisSatBlueCtrlFuncCmdStruct
{
	BYTE Length;
	BYTE Cmd;
	BYTE SixAxisSatBlue;
} SixAxisSatBlueCtrlFuncCmd_t;

typedef struct SixAxisSatYellowCtrlFuncCmdStruct
{
	BYTE Length;
	BYTE Cmd;
	BYTE SixAxisSatYellow;
} SixAxisSatYellowCtrlFuncCmd_t;

typedef struct SixAxisSatMagentaCtrlFuncCmdStruct
{
	BYTE Length;
	BYTE Cmd;
	BYTE SixAxisSatMagenta;
} SixAxisSatMagentaCtrlFuncCmd_t;

typedef struct SixAxisSatCyanCtrlFuncCmdStruct
{
	BYTE Length;
	BYTE Cmd;
	BYTE SixAxisSatCyan;
} SixAxisSatCyanCtrlFuncCmd_t;

typedef struct SixAxisSatBlackCtrlFuncCmdStruct
{
	BYTE Length;
	BYTE Cmd;
	BYTE SixAxisSatBlack;
} SixAxisSatBlackCtrlFuncCmd_t;

typedef struct SixAxisSatWhiteCtrlFuncCmdStruct
{
	BYTE Length;
	BYTE Cmd;
	BYTE SixAxisSatWhite;
} SixAxisSatWhiteCtrlFuncCmd_t;

typedef struct SixAxisHueRedCtrlFuncCmdStruct
{
	BYTE Length;
	BYTE Cmd;
	BYTE SixAxisHueRed;
} SixAxisHueRedCtrlFuncCmd_t;
	
typedef struct SixAxisHueGreenCtrlFuncCmdStruct
{
	BYTE Length;
	BYTE Cmd;
	BYTE SixAxisHueGreen;
} SixAxisHueGreenCtrlFuncCmd_t;
	
typedef struct SixAxisHueBlueCtrlFuncCmdStruct
{
	BYTE Length;
	BYTE Cmd;
	BYTE SixAxisHueBlue;
} SixAxisHueBlueCtrlFuncCmd_t;
	
typedef struct SixAxisHueYellowCtrlFuncCmdStruct
{
	BYTE Length;
	BYTE Cmd;
	BYTE SixAxisHueYellow;
} SixAxisHueYellowCtrlFuncCmd_t;
	
typedef struct SixAxisHueMagentaCtrlFuncCmdStruct
{
	BYTE Length;
	BYTE Cmd;
	BYTE SixAxisHueMagenta;
} SixAxisHueMagentaCtrlFuncCmd_t;
	
typedef struct SixAxisHueCyanCtrlFuncCmdStruct
{
	BYTE Length;
	BYTE Cmd;
	BYTE SixAxisHueCyan;
} SixAxisHueCyanCtrlFuncCmd_t;
	
typedef struct SixAxisHueBlackCtrlFuncCmdStruct
{
	BYTE Length;
	BYTE Cmd;
	BYTE SixAxisHueBlack;
} SixAxisHueBlackCtrlFuncCmd_t;
	
typedef struct SixAxisHueWhiteCtrlFuncCmdStruct
{
	BYTE Length;
	BYTE Cmd;
	BYTE SixAxisHueWhite;
} SixAxisHueWhiteCtrlFuncCmd_t;

typedef struct SixAxisBrightRedCtrlFuncCmdStruct
{
	BYTE Length;
	BYTE Cmd;
	BYTE SixAxisBrightRed;
} SixAxisBrightRedCtrlFuncCmd_t;
		
typedef struct SixAxisBrightGreenCtrlFuncCmdStruct
{
	BYTE Length;
	BYTE Cmd;
	BYTE SixAxisBrightGreen;
} SixAxisBrightGreenCtrlFuncCmd_t;
		
typedef struct SixAxisBrightBlueCtrlFuncCmdStruct
{
	BYTE Length;
	BYTE Cmd;
	BYTE SixAxisBrightBlue;
} SixAxisBrightBlueCtrlFuncCmd_t;
		
typedef struct SixAxisBrightYellowCtrlFuncCmdStruct
{
	BYTE Length;
	BYTE Cmd;
	BYTE SixAxisBrightYellow;
} SixAxisBrightYellowCtrlFuncCmd_t;
		
typedef struct SixAxisBrightMagentaCtrlFuncCmdStruct
{
	BYTE Length;
	BYTE Cmd;
	BYTE SixAxisBrightMagenta;
} SixAxisBrightMagentaCtrlFuncCmd_t;
		
typedef struct SixAxisBrightCyanCtrlFuncCmdStruct
{
	BYTE Length;
	BYTE Cmd;
	BYTE SixAxisBrightCyan;
} SixAxisBrightCyanCtrlFuncCmd_t;
		
typedef struct SixAxisBrightBlackCtrlFuncCmdStruct
{
	BYTE Length;
	BYTE Cmd;
	BYTE SixAxisBrightBlack;
} SixAxisBrightBlackCtrlFuncCmd_t;
		
typedef struct SixAxisBrightWhiteCtrlFuncCmdStruct
{
	BYTE Length;
	BYTE Cmd;
	BYTE SixAxisBrightWhite;
} SixAxisBrightWhiteCtrlFuncCmd_t;
/*****************************************/

typedef struct DpRx0MstInfoFuncCmdStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE DpRx0MstInfo;
} DpRx0MstInfoFuncCmd_t;

typedef struct DpRx0IsStableResPktStruct
{
	BYTE Length;
	BYTE Cmd;
	BYTE DpRx0_IsStable;
} DpRx0IsStableResPkt_t;

typedef struct DpRx0ReModeSetupFuncCmdStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE DpRx0ReModeSetup;
} DpRx0ReModeSetupFuncCmd_t;

typedef struct ADCKeypadFuncCmdStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	Key_Value;   //ChengLin: Base on Louis's specification. Vaue is High Byte [8:15]
   BYTE	Key_Channel; //ChengLin: Base on Louis's specification. Channel is Low Byte[0:7] 
} ADCKeypadFuncCmd_t;

typedef struct KeypadCmdResponseStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	Key_Value;	//ChengLin: Base on Louis's specification. Vaue is High Byte [8:15]
   BYTE	Key_Channel;//ChengLin: Base on Louis's specification. Channel is Low Byte[0:7]   
} KeypadResPkt_t;

typedef struct IRFuncCmdStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	IR_Value;
} IRFuncCmd_t;

/*ChengLin 20130128: New Sync Handler states*/
typedef struct ImageSyncStateCtrlFuncCmdStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE ImageSyncState;
} ImageSyncStateCtrlFuncCmd_t;

typedef struct ImageSyncStateStatusCmdResponseStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	ImageSyncState;   
} ImageSyncStateStatusResPkt_t;
/*****************************************/

/*ChengLin 20130222: Add for sync Left/Right PB Control Status*/
#if ENABLE_ATHENA_R_L_IMAGE_SYNC_HANDLER
typedef struct ImageSyncBlackScreenCtrlFuncCmdStruct
{
	BYTE Length;
	BYTE Cmd;
	BYTE ImageSyncBlackScreen;
} ImageSyncBlackScreenCtrlFuncCmd_t;
#endif	

typedef struct GammaCorrectionCtrlFuncCmdStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE GammaCorrection;
} GammaCorrectionCtrlFuncCmd_t;

/*ChengLin 20130411: Enable/Disable Athena FE or Left Debug Mode from Athena Right*/
typedef struct AthenaFE_L_DebugModeCtrlFuncCmdStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE DebugMode;
} AthenaFE_L_DebugModeCtrlFuncCmd_t;

#ifdef SMART_ISP
typedef struct SmartIspCtrlFuncCmdStruct
{
	BYTE Length;
	BYTE Cmd;
	BYTE SmartIspControl;
} SmartIspCtrlFuncCmd_t;

typedef struct SmartIspStatusResPktStruct
{
	BYTE Length;
	BYTE Cmd;
	BYTE SmartIspStep;
	BYTE SmartispStatus;
} SmartIspStatusResPkt_t;

#endif

/*ChengLin 20130322: Load Default Settings or Original Setting Before/After Splash Screen*/
typedef struct SplashScreenLoadDefCtrlFuncCmdStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE SplashLoadDefault;
} SplashScreenLoadDefCtrlFuncCmd_t;

typedef struct SlaveRegistersAddrFuncCmdStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	Address_HighByte;
   BYTE Address_LowByte;
} SlaveRegistersAddrFuncCmd_t;

typedef struct SlaveRegistersDataFuncCmdStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	Data_HighByte;
   BYTE Data_LowByte;
} SlaveRegistersDataFuncCmd_t;

typedef struct SlaveRegistersDataCmdResponseStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	Data_HighByte;
   BYTE Data_LowByte;
} SlaveRegistersDataResPkt_t;

typedef struct DDCCICtrlFuncCmdStruct
{
   BYTE	Length;
   BYTE	Cmd;
   BYTE	DDC_VCP_Code;
   BYTE	DDC_Value_HighByte;
   BYTE	DDC_Value_LowByte;   
} DDCCICtrlFuncCmd_t;

typedef struct DDCCIStatusCmdResponseStruct
{
	BYTE Length;
	BYTE Cmd;
	BYTE DDC_VCP_Code;
	BYTE DDC_VCP_Type;
	BYTE DDC_VCP_Max_HighByte;
	BYTE DDC_VCP_Max_LowByte;
	BYTE DDC_Value_HighByte;
	BYTE DDC_Value_LowByte; 
} DDCCIStatusResPkt_t;

#if ENABLE_SOC_IPC_KEYPAD_VIA_QUEUE
typedef struct ADCKeypadKeyStruct
{
   BOOL	ValidKey;
   BYTE	KeyChannel;
   BYTE	KeyValue;  
} KeypadRecvFormat_t;

extern KeypadRecvFormat_t gKeypadQueueBuffer[KEYPAD_BUFFER_MAX];
#endif

#if ENABLE_SOC_IPC_IR_VIA_QUEUE
typedef struct IRKeyStruct
{
   BOOL	ValidKey;
   BYTE	IRKey;  
} IRRecvFormat_t;

extern IRRecvFormat_t gIRKeyQueueBuffer[IR_BUFFER_MAX];
#endif


//******************************************************************************
//  G L O B A L    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************



BOOL SocIPCSrcPacketTransmitter(gmt_SocIPCCommandCode Cmd);
BOOL SocIPCSendCmdPacketParser(BYTE DeviceID, gmt_SocIPCCommandCode Cmd, BYTE* MsgParaBuf);
BOOL SocIPCTxReceivePacketCheck( BYTE DeviceID, gmt_SocIPCCommandCode Cmd );
void SoCIPC_UpdateRecvDataFromDevice_LEFT_FE(BYTE far* gParabufferPtr);
BYTE SocIPCTxGetCheckSum(BYTE *Bp_Buff,BYTE B_Len);
BOOL SocIPCTxRecvBufCheckCS(BYTE *Bp_Buff,BYTE B_Len);
void SocIPCSendKeypadEvent(void);
void SocIPCSendIREvent(void);
/*ChengLin 20130115: New 4k2k board not to use those functions*/

#if ENABLE_SOC_IPC_BUSY_CHECK
void SocIPCSetClearSlaveLeftBusyStatus(BOOL Busy);
void SocIPCSetClearSlaveFEBusyStatus(BOOL Busy);
BOOL SocIPCIsSlaveLeftReady(void);
BOOL SocIPCIsSlaveFEReady(void);
#endif
/*ChengLin 20130129: Enable Bidirectional BUSY Status Check*/
#if(ENABLE_BIDIRECTIONAL_BUSY_CHECK)
BOOL SocIPCWaitAthenaMasterReady(void);
#endif
#if ENABLE_SOC_IPC_OSD_SYNC
BOOL SocIPCMasterSlaveOSDSyncSettings(void);
#endif
BOOL SocIPCCheckDevice(BYTE DevID);
#if ENABLE_SOC_IPC_KEYPAD_VIA_QUEUE
void SocIPCKeypadBufferInitial(void);
void SocIPCKeypadBufferInsertKey(KeypadRecvFormat_t KeypadValidKey);
BOOL SocIPCIsKeypadBufferExistValidKey(void);
KeypadRecvFormat_t SocIPCKeypadBufferGetValidKey(void);
#endif
#if ENABLE_SOC_IPC_IR_VIA_QUEUE
void SocIPCIRBufferInitial(void);
void SocIPCIRBufferInsertKey(IRRecvFormat_t IRValidKey);
BOOL SocIPCIsIRBufferExistValidKey(void);
IRRecvFormat_t SocIPCIRBufferGetValidKey(void);
#endif
void SocIPCWriteRegWord(BYTE DevID, WORD W_RegAddr, WORD W_RegVal);
WORD SocIPCReadRegWord(BYTE DevID, WORD W_RegAddr);
BOOL SocIPCGetSlaveTimingInfo(BYTE DevID);
BOOL SocIPCGetSlaveModeInfo(BYTE DevID);
BOOL SocIPCGetSlavePortStatus(BYTE DevID);
BOOL SocIPCGetDcrResult(BYTE DevID);
BOOL SocIPCSendMasterCableStatus(BYTE DevID);
BOOL SocIPCSendMasterTimingInfo(BYTE DevID);
BOOL SocIPCSendMasterModeInfo(BYTE DevID);
BOOL SocIPCSendMasterPortStatus(BYTE DevID);
#if ENABLE_GET_I2C_READ_BYTE_VIA_SOCIPC_RECV_PACKET
gmt_RET_PASSFAIL SocIPCAutoLengthRead2WireBlock( BYTE B_DevAddr, BYTE *Bp_Buffer, gmt_2WIRE_STOP B_Stop );
#endif



BOOL IsCompleted_SOC_IPC_I2C_Msg(SLAVE_CH B_SlaveCh);
BOOL huge SocIPCI2CSlaveWriteCmdPkt(void far* pBuf, SLAVE_CH B_SlaveCh);
BOOL SocIPC_SlaveSetDDCCIActionHandler(BYTE DDC_Cmd , WORD W_VCPValue );
BOOL SocIPC_SlaveGetDDCCIStatusHandler(BYTE DDC_Cmd , DDCCIStatusResPkt_t *GetVcpReply );
BYTE SoCIPC_SlavDevHandler(SLAVE_CH B_SlaveCh);
PARSER_STATUS_t SoCIPC_SlavDevCmdParser(BYTE far* gExtDebugCmdPtr, HANDLER_USE_t B_whichHandler);
void SOCIPC_EndOfSlaveCmdParser(SOCIPC_SlaveUpdateStatus_t* Update);


#if ENABLE_SOC_IPC_LEFT_RIGHT_SLAVE_INT2_FROM_FE_MASTER
void SocIPCSyncInterruptViaSlaveFE(void);
#endif


void SocIPCAppsTestHandler(void);
BYTE SocIPCGetCorrectSharpnessSetting(void);
BOOL SocIPCSetCorrectSharpnessSetting(BYTE SharpnessVal);
#if (!ENABLE_GET_I2C_READ_BYTE_VIA_SOCIPC_RECV_PACKET)
BYTE SocIPCGetResponseDataLength(gmt_SocIPCCommandCode Cmd);
#endif
BYTE SocIPCGetResponseDataType(gmt_SocIPCCommandCode Cmd);

/*ChengLin 20130108: OSD Sync Function for 4kx2k Platform*/
void SOCIPC_SyncOSDAdjustHandler( gmt_SocIPC_OSDSyncID SI_OSD_Cmd );

/*ChengLin 20130131: Enable VEGA I2C Control Via Athena FE*/
#if (ENABLE_VEGA_CONTROL_VIA_ATHENA_FE)
void SOCIPC_VEGA_I2C_WriteWord_Via_FE(BYTE SlaveAddr, WORD wValue);
WORD SOCIPC_VEGA_I2C_ReadWord_Via_FE(BYTE SlaveAddr);
#if ENABLE_VEGA_AVAIL_CHECK_VIA_FE_AC_ON
void SOCIPC_Check_VEGA_Available_Via_FE(void);
#endif
void SOCIPC_VEGA_WriteHWReg_Via_FE(WORD HWAddr, BYTE Value);
BYTE SOCIPC_VEGA_ReadHWRegWord_Via_FE(WORD HWAddr);
void SOCIPC_VEGA_LVDS_BUS_WIDTH_Via_HDMI_Input(void);
#endif

#if ENABLE_VEGA_CSC_VIA_HDMI_INFO
void SOCIPC_VEGA_CSC_Via_HDMI_Input(BOOL bPassThrough);
#endif

#if ENABLE_ATHENA_R_L_IMAGE_SYNC_HANDLER
void SOCIPC_SetAthena_L_ImageSyncState(gmt_ImageSyncState State);
gmt_ImageSyncState SOCIPC_GetAthena_L_ImageSyncState(void);
void SOCIPC_SetAthena_Right_Left_PB_Ctrl_Status(BOOL Enable);
#endif
void SOCIPC_ResetVEGAViaAthenaRight(void);
void SOCIPC_LoadSplashScreenColorSettings(BOOL Load);
void SOCIPC_CheckColorSettingBeforeSplashScreen(void);
void SOCIPC_RollBackColorSettingAfterSplashScreen(void);
void SOCIPC_SetClearIgnoreInterChipCmdFlag(BOOL Status);
#if ENABLE_VEGA_ISP_CMD_VIA_UART_ISP_PORT 
void SOCIPC_SetAthenaFE_L_DebugFlag(BYTE DevID);//Added by ChengLin 20130411
void SOCIPC_VEGA_ISP_Enable(void);//Added by ChengLin 20130411
#endif
#ifdef SMART_ISP
BOOL SocIPCGetSlaveSmartIspStatus(BYTE DevID);
#endif

#endif
//*********************************  END  **************************************
