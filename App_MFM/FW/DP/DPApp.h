/*
   $Workfile:   DPApp.h  $
   $Revision: 1.72 $
   $Date: 2013/06/14 02:38:01 $
*/
//******************************************************************************
// COPYRIGHT (C) STMicroelectronics 2010 All rights reserved.
// This document contains proprietary and confidential information of the
// STMicroelectronics Group. This document is not to be copied in whole or part.
// STMicroelectronics assumes no responsibility for the consequences of use of
// such information nor for any infringement of patents or other rights of third
// parties which may result from its use. No license is granted by implication
// or otherwise under any patent or patent rights of STMicroelectronics.
// STMicroelectronics products are not authorized for use as critical components
// in life support devices or systems without express written approval of
// STMicroelectronics.
//******************************************************************************

//==============================================================================
//
// MODULE:		DPApp.h
//
//******************************************************************************

#ifndef __DPAPP_H__
#define __DPAPP_H__
//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************

//******************************************************************************
//  G L O B A L  D E F I N I T I O N S
//******************************************************************************
#define FORCE_NO_SIGNAL_IF_DPCD600_2         0     // Force DP input to no signal state if DPCD600=2

#define INGORE_SLEEP_PHY_AUTOMATION          1     // For DPTX Phy Automation CTS

#define UNRELIABLE_DP_CAB_STS_WKAROUND       1     // To workaround unreliable cable status of DP source

#define DPRX0_DYNAMIC_INIT_BY_NVRAM          1     // Allow System can dynamically re-init Dplib in order to
                                                   // swith between DP1.1a and DP1.2

#define IGNORE_SLEEP_IF_SRCPWR_IS_ON         1     // Avoid system entering LPM sleep mode 
                                                   // If Cable is connected and SrcPwr=1
                                                   // Even if DPCD600=2. 

#define IGNORE_SLEEP_IF_DPCD600_NOT_2        1     // Avoid system entering LPM sleep mode 
                                                   // if DPCD600 is not 2 when cable is connectted
                                                   // And SrcPwr is on.
                                                
#define IGNORE_SLEEP_DPRX_IS_STABLE          1     // Avoid system entering LPM sleep mode 
                                                   // if Dp is still in stable state.
                                                   // (For Not-Wake-Up in Astro VG870 Sync off/on issue)
                                                
#define SRCPWR_OFF_DEBOUNCE_CHECK            1     // Debounce of SrcPwr checking in off condition
#define SRCPWR_OFF_DEBOUNCE_TIMEx10ms        25    // 250ms


#define ENABLE_HPD_OVERRIDED                 1     // If 1, DP_HPD will force to low 
                                                   // before Timer RX0/1_HPD_OVERRIDED_10ms_TMR
                                                   // is time-out, this is to form a HPD long pulse when power on.
#define HPD_OVERRIDED_TIMEx10ms             40// JC_SZ_0924  28    // 280ms 

#define EnableDpTxDelayCount                 0     // Test only. Not use now.
#define ExtraDpTxEnableDelayCount            3

#define AUXLLI2C_FROM_IROM	                  0     // Define to 0
#define AUXLLI2C_FROM_LIB	                  1     // Define to 1
#define AUXLLI2C_TYPE		AUXLLI2C_FROM_LIB       // Aux2I2c low-level driver. Athena only support "AUXLLI2C_FROM_LIB" now.

#define ENABLE_DPCD_SINK_TEST_SLEEP_DELAY    1     // Don't enter LPM mode when performing DP CTS test.
#define SINK_TEST_SLEEP_DELAY_x1s            60    // 60s

#define AUX_TRIGGER_NOTIFICATION_Timex10ms   200   // To indicate Source device sending commands over AUX during the past period. 

#define HPD_ON_PHYON_NOTIFICATION             1    // To issue a HPD long pulse after the time of "PHYON_NOTIFY_Time_x10ms" when LT is not established if DP_PHY is from off to on.
#define HPD_ON_PHYON_NOTIFICATION_ONLY_WAKEUP 1    // 1: Only issue the HPD when wake up. 0:Issue HPD when PHY status change from off to on.
#define PHYON_NOTIFY_Time_x10ms              100   // 100ms 
#define HPD_OVERRIDED_PHY_ACT_TIMEx10ms      10    // 


#define DPTX_PING_SINK_DEVICE_WHEN_VIDEO_ON  0     // When the connected sink device is in sleep mode, issue an Aux command to wake up the sink device when video on.

#define DEBUG_DP_PRINT_TIMING_DISPLAY_TIME   0     // 0: Disable  //1:Cable Plugged -->ModeSetupEnd
                                                   // Note: CANNOT enable DEBUG_HDMI_PRINT_TIMING_DISPLAY_TIME and DEBUG_DP_PRINT_TIMING_DISPLAY_TIME in the same time.
#define PATCH_OF_UNIQUE_EDID				 0	   //Patch for load BKSV first 4 bytes as EDID serial number and re-calculate checksum

#define PATCH_OF_DPCD600_2_RE_LT			 0	   //Patch for check DPCD 600h status before Re-Link Training

/*ChengLin 20130514: Enable DP1.2 DDC/CI Supporting*/
#define ENABLE_DP12_DDCCI_SUPPORT   1

#if (DEBUG_DP_PRINT_TIMING_DISPLAY_TIME!=0)
#define SetDebugDisplayTime SetDpDebugDisplayTime
#define GetDebugDisplayTime GetDpDebugDisplayTime
#endif
                                                   
#ifdef ST_4K2K_93xx_BOARD
#define DISPLAY_ID_SUPPORT                   1     // Only valid for Athena4K2K
#define Base_CEA_DisplayID                   1     // 1: Base+CEA+DisplayID    0: Base+DisplayID

#ifdef DP_HUB_2_LOG_PORTS
/*ChengLin 20130521: Enable Re-Mode Setup once Athena R get the same timing but MST payload be changed*/
#define ENABLE_DPRX0_RE_MODE_SETUP     1
#endif

#else
#define DISPLAY_ID_SUPPORT                   0     // Only valid for Athena4K2K
#define Base_CEA_DisplayID                   0     // 1: Base+CEA+DisplayID    0: Base+DisplayID

/*ChengLin 20130521: Enable Re-Mode Setup once Athena R get the same timing but MST payload be changed*/
#define ENABLE_DPRX0_RE_MODE_SETUP     0

#endif



//#define DP12_AUXCH_REG_OFFSET		(DP12RX1_AUX_CONTROL - DP12RX0_AUX_CONTROL)
//#define DP12AUXREG(AuxReg,AuxCh)	(AuxReg + (AuxCh * DP12_AUXCH_REG_OFFSET))
//#define DPRX1_AUX_GAIN				(DPRX1_AUX_PHY_CTRL + 2)
//#define DPRX1_AUX_PREEMPH 			(DPRX1_AUX_PHY_CTRL + 3)
//#define DP12RX1_AUX_OPT_CTRL		(DP12RX1_AUX_CONTROL + 1)
//#define DP12RX1_AUX_CLK				(DP12RX1_AUX_CONTROL + 2)
//#define DP12RX1_AUX_TBUS_SEL		(DP12RX1_AUX_CONTROL + 3)
//#define DP12RX1_AUX2OCM_REQ_LEN		(DP12RX1_AUX2OCM_REQ + 1)
//#define DP12RX1_AUX2OCM_RPLY_LEN	(DP12RX1_AUX2OCM_RPLY_COMM + 1)

#define AUX_MAX_BYTES_IN_CHUNK   16

//Reply
#define AUX_REPLY_CMD_MASK        0x0F

#define AUX_REPLY_CMD_AUX_ACK     0x00
#define AUX_REPLY_CMD_AUX_NACK    0x01
#define AUX_REPLY_CMD_AUX_DEFER   0x02
#define AUX_REPLY_CMD_I2C_NACK    0x04
#define AUX_REPLY_CMD_I2C_DEFER   0x08

#define AUX_REPLY_LENGTH_MASK     0x1F

//Request
#define AUX_REQUEST_I2C_WRITE     0
#define AUX_REQUEST_I2C_READ      1
#define AUX_REQUEST_I2C_STATUS    2
#define AUX_REQUEST_MOT           4

#define AUX_REQUEST_NATIVE_WRITE  0
#define AUX_REQUEST_NATIVE_READ   1
#define AUX_REQUEST_NATIVE_BIT    8

#define AUX_REQUEST_DIRECTION_MASK  1
#define AUX_REQUEST_LENGTH_MASK     0x0F
#define AUX_REQUEST_COMMAND_MASK    3


#define	AUX_TRANSACTION_SIZE			16		// One AUX Transation Data Lenght
//#define EDID_BUFFER_START				0xBD00
//#define EDID_BUFFER2_START			0xBC00
#ifdef ST_4K2K_93xx_BOARD
   #if (Base_CEA_DisplayID!=0) 
   #define EDID_BUFFER_SIZE_Mst_R		256   //BASE+DISPLAY_ID
   #define EDID_BUFFER_SIZE_Mst_L		384   //BASE+CEA861+DISPLAY_ID
   #define EDID_BUFFER_SIZE				256   //BASE+CEA861
   #else
   #define EDID_BUFFER_SIZE_Mst_R		256   //BASE+DISPLAY_ID
   #define EDID_BUFFER_SIZE_Mst_L		256   //BASE+DISPLAY_ID
   #define EDID_BUFFER_SIZE				256   //BASE+CEA861
   #endif
#else
   #define EDID_BUFFER_SIZE				256   //BASE+CEA861
#endif
#define AUX_EDID_WRITE_UNLOCK			0x5A
#define	EDID_EEPROM_DEV_ADDR			0xA0

#define CFG_DPTX_HDCP_EN



#define DPRX_DTG_PARAM_SEL_HTOTAL             DP_BIT0
#define DPRX_DTG_PARAM_SEL_HSTART             DP_BIT1
#define DPRX_DTG_PARAM_SEL_HWIDTH             DP_BIT2
#define DPRX_DTG_PARAM_SEL_HS_WIDTH           DP_BIT3
#define DPRX_DTG_PARAM_SEL_VTOTAL             DP_BIT4
#define DPRX_DTG_PARAM_SEL_VSTART             DP_BIT5
#define DPRX_DTG_PARAM_SEL_VHEIGHT            DP_BIT6
#define DPRX_DTG_PARAM_SEL_VS_WIDTH           DP_BIT7
#define DPRX_DTG_AFR_MASK_CLOCK_RECOVERY_LOST DP_BIT8
#define DPRX_DTG_AFR_MASK_SYM_LOCK_LOST       DP_BIT9
#define DPRX_DTG_AFR_MASK_CODE_ERROR          DP_BIT10
#define DPRX_DTG_AFR_MASK_ALIGN_ERROR         DP_BIT11
#define DPRX_DTG_AFR_MASK_VFIFO_ERROR         DP_BIT12
#define DPRX_DTG_AFR_MASK_VBID_FLAG_CHANGED   DP_BIT13
#define DPRX_DTG_AFR_MASK_COLOR_SPACE_CHANGED DP_BIT14
#define DPRX_DTG_AFR_MASK_NO_VIDEO            DP_BIT15


//******************************************************************************
//  G L O B A L  T Y P E S  D E F I N I T I O N S
//******************************************************************************
typedef enum
{
   DP_PWR_PHYON=0,
   DP_PWR_PHYOFF,
   DP_PWR_UNKNOWN
}	DP_PWRSTATE;

typedef struct
{
   DWORD dw_SetPowerExCheckTime;
   BYTE b_SetPowerExCheck    :1;
   BYTE b_SrcPwrOverrideFlag :1;
   BYTE PreDpPwrState        :2;
   BYTE DpPwrState           :2;
#if (HPD_ON_PHYON_NOTIFICATION !=0)
   BYTE b_PhyOnNotify        :1;   
   #if (HPD_ON_PHYON_NOTIFICATION_ONLY_WAKEUP!=0)
   BYTE b_LpmWkUpNotify      :1;
   #endif
#endif   
}DpPwrCheck_t;

typedef struct
{
   BYTE *Ba_EDIDReadBuffer;
   BYTE B_WrState;
   WORD W_Offset;
   WORD W_ReadPointer;
   WORD W_WritePointer;
} DprxVirtualEdid_t;

typedef enum
{
   WR_FREE,
   WR_START,
   WR_ADDR1,
   WR_ADDR2,
   WR_DATA,
   WR_STOP
} AUX_EDID_WRITE_TYPE;

typedef enum
{
   FE_SRC_DPTXBE,
   FE_SRC_DP12BRIDGE
} DPTX_FE_INPUT_SOURCE_TYPE;

typedef enum
{
   DP_TPG_PTRN_SPEC_BY_COMP,
   DP_TPG_PTRN_R_RAMP,
   DP_TPG_PTRN_G_RAMP,
   DP_TPG_PTRN_B_RAMP,
   DP_TPG_PTRN_WHITE_RAMP,
   DP_TPG_PTRN_COLOR_BAR,
   DP_TPG_PTRN_CHECKER_BOARD,
   DP_TPG_PTRN_RANDOM
} DP12RL_TPG_PATTERN_TYPE;

typedef enum
{
   DP_TPG_CS_RGB,
   DP_TPG_CS_Y_ONLY,
   DP_TPG_CS_YCBCR_444,
   DP_TPG_CS_YCBCR_422
} DP12RL_TPG_COLOR_SPACE_TYPE;


typedef enum
{
   STDPRX_COLORIMETRY_RGB_DEFAULT,
   STDPRX_COLORIMETRY_xvYCC601_422,
   STDPRX_COLORIMETRY_xvYCC601_444,
   STDPRX_COLORIMETRY_XRGB,
   STDPRX_COLORIMETRY_sRGB,
   STDPRX_COLORIMETRY_ITU601_422,
   STDPRX_COLORIMETRY_ITU601_444,
   STDPRX_COLORIMETRY_DCI_P3,
   STDPRX_COLORIMETRY_RESERVED_1000,
   STDPRX_COLORIMETRY_xvYCC709_422,
   STDPRX_COLORIMETRY_xvYCC709_444,
   STDPRX_COLORIMETRY_scRGB,
   STDPRX_COLORIMETRY_AdobeRGB,
   STDPRX_COLORIMETRY_ITU709_422,
   STDPRX_COLORIMETRY_ITU709_444,
   STDPRX_COLORIMETRY_Color_Profile,
   STDPRX_COLORIMETRY_Yonly
} STDPRX_ColorimetryStd_t;


typedef enum
{
   DPTX_TIMING_BY_INPUT,
   DPTX_TIMING_BY_OUTPUT
} DPTX_TIMING_CONFIG_METHOD;


#ifndef __CEA_861__
#define __CEA_861__
//Color space values of AVI InfoFrame (see CEA-861-D ,table  8)
typedef enum
{
   gmd_CEA861_AVI_Y_RGB = 0,
   gmd_CEA861_AVI_Y_YUV422,
   gmd_CEA861_AVI_Y_YUV444,
   gmd_CEA861_AVI_Y_RSVD
} gmt_CEA861_AVI_Y;

//Active information present field of AVI InfoFrame (CEA-861-D, table 8)
typedef enum
{
   gmd_CEA861_AVI_A_NODATA = 0,
   gmd_CEA861_AVI_A_DATA_VALID
} gmt_CEA861_AVI_A;

//Bar info data valid field of AVI InfoFrame (CEA-861-D, table 8)
typedef enum
{
   gmd_CEA861_AVI_B_NO_BAR = 0,
   gmd_CEA861_AVI_B_V_BAR,
   gmd_CEA861_AVI_B_H_BAR,
   gmd_CEA861_AVI_B_HV_BAR
} gmt_CEA861_AVI_B;

//Scan information field of AVI InfoFrame (CEA-861-D, table 8)
typedef enum
{
   gmd_CEA861_AVI_S_NO_DATA = 0,
   gmd_CEA861_AVI_S_OVERSCAN_TV,
   gmd_CEA861_AVI_S_UNDERSCAN,
   gmd_CEA861_AVI_S_RSVD
} gmt_CEA861_AVI_S;

//Colorimetry field of AVI InfoFrame (CEA-861-D, table 9)
typedef enum
{
   gmd_CEA861_AVI_C_NO_DATA = 0,
   gmd_CEA861_AVI_C_ITU_BT_601,
   gmd_CEA861_AVI_C_BT_709,
   gmd_CEA861_AVI_C_EXTENDED
} gmt_CEA861_AVI_C;

//Picture aspect ratio field (4:3, 16:9) of AVI InfoFrame (CEA-861-D, table 9)
typedef enum
{
   gmd_CEA861_AVI_M_NO_DATA = 0,
   gmd_CEA861_AVI_M_4_3,
   gmd_CEA861_AVI_M_16_9,
   gmd_CEA861_AVI_M_RSVD
} gmt_CEA861_AVI_M;

//Non-uniform picture scaling field (4:3, 16:9) of AVI InfoFrame (CEA-861-D, table 11)
typedef enum
{
   gmd_CEA861_AVI_SC_NONE = 0,
   gmd_CEA861_AVI_SC_H,
   gmd_CEA861_AVI_SC_V,
   gmd_CEA861_AVI_SC_HV,
} gmt_CEA861_AVI_SC;

//Audio coding type (CEA-861-D, table 17)
typedef enum
{
   gmd_CEA861_AUDIO_CT_NONE = 0,
   gmd_CEA861_AUDIO_CT_PCM,
   gmd_CEA861_AUDIO_CT_AC3,
   gmd_CEA861_AUDIO_CT_MPEG1,
   gmd_CEA861_AUDIO_CT_MP3,
   gmd_CEA861_AUDIO_CT_MPEG2,
   gmd_CEA861_AUDIO_CT_AAC,
   gmd_CEA861_AUDIO_CT_DTS,
   gmd_CEA861_AUDIO_CT_ATRAC,
   gmd_CEA861_AUDIO_CT_DSD,    // One Bit Audio
   gmd_CEA861_AUDIO_CT_DOLBY_DIGITAL_PLUS,
   gmd_CEA861_AUDIO_CT_DTS_HD,
   gmd_CEA861_AUDIO_CT_MAT,
   gmd_CEA861_AUDIO_CT_DST,
   gmd_CEA861_AUDIO_CT_WMA_PRO,
   gmd_CEA861_AUDIO_CT_RSVD
} gmt_CEA861_AUDIO_CT;
#endif //#ifndef __CEA_861__


typedef struct
{
   gmd_PanelType  Type;
   WORD           HTotal_16;
   WORD           HActiveStart_16;
   WORD           HActiveWidth_16;
   WORD           HSyncWidth_16;
   WORD           VTotal_16;
   WORD           VActiveStart_16;
   WORD           VActiveLength_16;
   WORD           VSyncWidth_16;
   DWORD          PixelFrequencyKhz_32;
} DpTxMsParam_t;

typedef enum
{
   DP_Input_11 = 0,
   DP_Input_12,
   DP_Input_Auto
} gmt_DP_Input_Type;

typedef struct
{
   BYTE B_CurInputCntr;   //Current input connector on Cphy[x]
//   BYTE B_Mux_Flag;       //TRUE if ext Mux is available
//   BYTE B_CurSeltrID;     //Current Mux Selection
//   void far * ExtPortSetup_ps;
}gmt_DpRxConfig;

#define DPRX_DPCD_BASE_ADDR   	 0x1F000UL
#define DPRX2_DPCD_BASE_ADDR   	 0x1B000UL
#define AUX_REQUEST_START_DPCD   0x7E0
#define AUX_REPLY_START_DPCD     0x7F0

#define DPRX_DPCD_BASE_OFFSET	(DPRX_DPCD_BASE_ADDR-DPRX2_DPCD_BASE_ADDR)

#define DPRX_SDP_MEM_BASE		0x10800L
#define DPRX2_SDP_MEM_BASE		0x1A000L
#define DPTX_SDP_MEM_BASE		0x10000L

#define DP12BRIDGE_TOP_BASE		0xFFFF

#define DPRX_DPCD_DOWNREP_BASE	0x26000L
#define DPRX_DPCD_UPREQ_BASE	0x26200L
#define DPRX_DPCD_DOWNREQ_BASE	0x26400L
#define DPRX_DPCD_UPREP_BASE	0x26600L

#define DPRX2_DPCD_DOWNREP_BASE	0x26800L
#define DPRX2_DPCD_UPREQ_BASE	0x26A00L
#define DPRX2_DPCD_DOWNREQ_BASE	0x26C00L
#define DPRX2_DPCD_UPREP_BASE	0x26E00L


#define DPTX_HDCP_BASE_ADDR     0x16C00L

//#define AUX_START_DPCD_OFFSET	(AUX_REQUEST_START_DPCD-AUX2_REQUEST_START_DPCD)


//******************************************************************************
//  F U N C T I O N S    P R O T O T Y P E S
//******************************************************************************

//************************  DpBrApp.c ******************************************
gmt_UserDefConnNames    GetDpInputConnector(gmt_DPRX_DID b_DID);
void DpAppInit(void);
void DpAppHandler(void);
//BYTE Dprx_DID_To_LibPortId(gmt_DPRX_DID b_DID);

#if (AUXLLI2C_TYPE==AUXLLI2C_FROM_LIB)
//BOOL Rx0_AUXI2CRequest(WORD W_Addr, BOOL B_IsRead, BYTE *B_Chunk, WORD W_Len, BYTE B_Stop);
//BOOL Rx1_AUXI2CRequest(WORD W_Addr, BOOL B_IsRead, BYTE *B_Chunk, WORD W_Len, BYTE B_Stop);
#endif

//void DpTxFe_SetInputSrc(DPTX_FE_INPUT_SOURCE_TYPE src);

gmt_CEA861_AVI_Y DpRxGetAviColorSpace(BYTE InputConnector);
BOOL DpRxIsStableState(BYTE InputConnector);
STDPRX_ColorimetryStd_t DpRxGetColorimetry(BYTE InputConnector);
gmt_3D_INPUT_FORMAT_TYPE gm_DpRxGet3DFormat(BYTE InputConnector);
gmt_COLOR_DEPTH gm_DpRxGetColorDepth(BYTE InputConnector);
BOOL DpRxGetSignalTiming(BYTE InputConnector, gmt_FrontEndSignalTiming_t* Ptr_Timing);
BYTE DpRxReadIfmClkDivRatio(BYTE InputConnector);
void DpRxPrintSignalProperty(BYTE InputConnector);
void DpRxAfrEnable(BYTE InputConnector, BOOL b_En);
void DpTxEnableVideo(BYTE TimingMethod);
void DpTxDisableVideo(void);
void DpRxPortChange(BYTE InputConnector);
void DpRxUpdaateVddsInitFreq(void);
BOOL DpRxGetAudioConfig(BYTE InputConnector,DP_AudioConfig_t * AudioConfig_ps);
void DpRxPrintAudioProperty(BYTE InputConnector);
void Set_SstOnlyTx_EnVdoFlag(BOOL EnFlag);


void DpTxEnableAudio(DP_AudioConfig_t *AudioConfig_ps);
void DpTxDisableAudio(void);
void DPTxPortSwitch (U8 PortIdToDeactivate_8, U8 PortIdToActivate_8);
void SetForceHpdLowFlag(gmt_DPRX_DID b_DID, BYTE SetLow);
void Reset_Pre_PortIdToActivate(void);
BOOL DpRxIsLT_Success(BYTE InputConnector);
BOOL DpRxIsMST_Stream(BYTE InputConnector);
#if defined(ST_4K2K_93xx_BOARD) && defined(DP_HUB_2_LOG_PORTS)
void ResetDpRx0MstInfoUpdatFlag(void);
BOOL IsDpRx0_Ready2UpdateMstInfo(void);
void DP_TwoLogical_CheckAndSyncAllocatedPayload(void); //Add by ChengLin 20130530
void ResetDpRx0VirtualChannelsAllocated(void);
#endif

DWORD DpRx_UpdatePixelClkExt(BYTE PortId);
BOOL gm_DpRxIsTmingMatched(gmt_PHY_CH Channel, gmt_FrontEndSignalTiming_t* Ptr_Timing);
#if (DEBUG_DP_PRINT_TIMING_DISPLAY_TIME!=0)
void SetDpDebugDisplayTime(void);
DWORD GetDpDebugDisplayTime(void);
#endif


//********************  DpPwrStateCheck.c **************************************
void DpPwrCheckInit(void);
BOOL IsDpcdSetPowerReallyDown(BYTE InputConnector);
void ResetDPCD600(BYTE InputConnector);
BOOL IsDpAuxTriggered(BYTE InputConnector);
BOOL IsDpSinkInTesting(BYTE InputConnector);
BOOL IsDpSrcPwrOverWritten(BYTE InputConnector);
BOOL DpRxPwrStateCheck(BYTE InputConnector);
DP_PWRSTATE GetDpRxPwrState(BYTE InputConnector);
void SetDpRxPwrState(BYTE InputConnector, DP_PWRSTATE Sts);
void AdjustDpPwrLevel(BYTE InputConnector);
void SetDp1AuxTrigger(void);
void SetDp2AuxTrigger(void);
BOOL DpRxSleepModeBlocker(BYTE InputConnector);
void PrintDpPwrCheck(gmt_DPRX_DID b_DID);
#if (HPD_ON_PHYON_NOTIFICATION!=0) && (HPD_ON_PHYON_NOTIFICATION_ONLY_WAKEUP!=0)
void DpRx_SendLpmWakeUp_Notice(void);
#endif

//***************************  DpRxEdid.c **************************************
#if (AUXLLI2C_TYPE==AUXLLI2C_FROM_IROM)
BOOL DpRxEdidHandler(DWORD DW_Addr, BYTE B_Req, BYTE B_Len, AUX_CH B_AuxCh);
#elif (AUXLLI2C_TYPE==AUXLLI2C_FROM_LIB)
BOOL DpRxEdidHandler(WORD W_Addr, BOOL B_IsRead, BYTE *BufPtr, WORD W_Len, BYTE B_Stop, AUX_CH B_AuxCh);
#endif
void DpRxEdidHandlerInit(void);

//***************************  DpRxDdcci.c **************************************
void DpRxAuxHandler(void);
void DpRxDdcCmdHandler(AUX_CH B_AuxCh);
void DpRxDdcciAuxSetReply(BYTE *B_BuffPtr, AUX_CH B_AuxCh);
#if (ENABLE_DP12_DDCCI_SUPPORT)
BOOL DDC_I2CSlaveCheckCS(BYTE B_DevID, BYTE B_Len, BYTE far*Bp_Buff);
void DpRxDdcciMultiStreamTransaction(Aux_data_buffer_t * I2cTransaction, U8 stopbit);
#endif

#ifdef ST_4K2K_93xx_BOARD
#ifdef DP_HUB_2_LOG_PORTS
#if (ENABLE_DPRX0_RE_MODE_SETUP)
void AthenaR_L_Redo_ModeSetup(void);
void DP_Two_LogicalPortTimingChangeHandler(void);
#endif
#endif
#endif

#endif
