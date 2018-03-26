/*
	$Workfile:   HdmiApp.h  $
	$Revision: 1.58 $
	$Date: 2013/04/01 03:51:07 $
*/
#ifndef __HDMI_APP_H__
#define __HDMI_APP_H__
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
// MODULE:      HdmiApp.h
//
// USAGE:       Header file for HDMI application functions
//
//******************************************************************************
#if (FEATURE_HDMI==ENABLE)
#define HPD_CONTROL_HANDLER_ENABLE           1        //Set 1 to enable HPD control
   #define HDMI_HPD_PULSE_IN_PORT_INIT       1        //Issue a HPD pulse when port init during the conditions such as port change, power on and wake-up. 
   #define DVI_HPD_PULSE_IN_PORT_INIT        1        //Issue a HPD pulse when port init during the conditions such as port change, power on and wake-up. 
   #ifdef ST_4K2K_93xx_BOARD
   #define HPD_PULSE_WIDTH_10mTime           1        //Default is 10ms  -ST4K2K board doesn't need to change port in Athena_FE
   #else
   #define HPD_PULSE_WIDTH_10mTime           15       //Default is 150ms
   #endif
   #define TURN_OFF_CPHY_IN_HPD_LOW          1        //Make PHY interface as HiZ to simulate calbe unplug 

   #define HDMI_1ST_MODESETUP_FB             0        //TEST ONLY. DONOT ENABLE. Force Background in 1st MdoeSetup to prevent Hdcp snow screen
   #define DVI_1ST_MODESETUP_FB              0        //TEST ONLY. DONOT ENABLE. Force Background in 1st MdoeSetup to prevent Hdcp snow screen
   #define MODESETUP_FB_10mTime              410      //Unit of 10ms, 2.1s

   #define ENABLE_MIN_PORT_OFF_LIMITATION    0        //Enalbe to limit Port_Off-->On duration bigger then MIN_PORT_OFF_Time 
   #define MIN_PORT_OFF_Time                 2100     //Unit of 1ms, defaul is 2.1 seconds.

   #define HDMI_PKT_ERROR_HPD                0        //Issue HPD if input HDMI Packet Noise is detected
   #define PKT_ERR_HPD_WAIT_TIME             250      //x10ms 


   #if (HPD_CONTROL_HANDLER_ENABLE==0)
   #undef  ENABLE_MIN_PORT_OFF_LIMITATION
   #undef  HDMI_PKT_ERROR_HPD

   #define ENABLE_MIN_PORT_OFF_LIMITATION    0
   #define HDMI_PKT_ERROR_HPD                0
   #endif


#define HDMI_VDDS_STABILITY_IMPROVEMENT   1        //Enalbe to improve VDDS stability
#define REINIT_VDDS_MULDIV_WR_IN_UNSTABLE 1        //To fix sometimes AVDDS_MULDVI_WR is not strobed when input signal is unstable in AC power on

#define FAST_AVDDS_LOCK                   1        //Set K MAIN/DIFF to small one for fast transition process

#define SCAN_TYPE_RECHECK                 1        //ScanType recheck from application layer, in case of HdmiRxlib reports incorrect info
                                                   //during temporary unstable input timing. 
#define PATCH_OF_DVI_FIELD_INVERSION      1        //Enalbe to fix filed invesion issue in DVI interlaced mode.

#define USE_HDMIDVI_HDCP_KSV_INTERRUPT    1        //Enable Isr to load KsvFifo for HDCP repeater function

#define USE_HDMIDVI_AFR_DETECT_INTERRUPT  0        //Enable Isr to catch AFR_DETECT for mode change garbage

#define USE_MINI_ISR_HANDLER              0        //Test Only: DONOT enabled it. Enable Timer base ISR for Hdmi mini handler

#define ENABLE_DDVI_EXCEPTION_TABLE       0        //Enable to identfy input is DVI or DualDVI by checking the provided exception table.

#define DUALDVI_HDCP_MODE_DET_DELAY       1        //Enable to prevent some dual dvi hdcp source may display temporay noise.
#define DUALDVI_HDCP_DELAY_COUNT          10       

#define BACKUP_INFODATA                   0        //All of the info data use the same structure (InfoUpdateEvtData) to get data from HdmirxLib
//If the retrieved data needs to be used somewhere later, enable "BACKUP_INFODATA" will copy
//the info data into individual structures to avoid the data is overrided while another info
//data.


#define HDMI_DVI_TIMING_LOSS_PATCH        0        //Enalbe to check Timing lost during ModeSetup. If detected, ModeSetup will be terminated immediately.

#define PATCH_OF_CLEAR_HDMI_ANA_LCLK_LOST 1        //Patch of MS_ABORT continuously happening deu to ANA_LCKL_LOST is not cleared in COBY DVD-288 player.


#define IBD_CONSISTENCE_CHECK             0        //Check IBD consistence. If previous IBD != current IBD value, gm_HdmiIsTmingMatched will return FALSE.


#define LINK_CLOCK_CHECK_SUPPORT          0        //Provide an API to check if the link clock is detected in the selected port.


#define DEBUG_HDMI_PRINT_TIMING_DISPLAY_TIME   0   // 0: Disable  //1: PortInit --> ModeSetupEnd  //2: Cable Plugged -->ModeSetupEnd
                                                   // Note: CANNOT enable DEBUG_HDMI_PRINT_TIMING_DISPLAY_TIME and DEBUG_DP_PRINT_TIMING_DISPLAY_TIME in the same time.
#if (DEBUG_HDMI_PRINT_TIMING_DISPLAY_TIME!=0)
#define SetDebugDisplayTime SetHdmiDebugDisplayTime
#define GetDebugDisplayTime GetHdmiDebugDisplayTime
#endif

#define DEBUG_HDMI_CPHY_CAPTURE           0        // Debug usage. 

#define DEBUG_HDMI_EQ_BATHTUB             0        // Debug usage.

typedef struct
{
   WORD        IsActivePort            : 1;
   WORD        SignalDataLoaded        : 1; 	
   WORD        VideoDataLoaded         : 1;
   WORD        AudioDataLoaded         : 1;	
   WORD        PreAVMuteStatus         : 1;
   WORD        ReDoModeSetup           : 1;
   WORD        ReDoModeSetup_3D        : 1;  
   WORD        EnableHpdInPortInit     : 1;
   WORD        FirstModeSetupFB        : 1;
   WORD        PerformInstReset        : 1;
#if (LINK_CLOCK_CHECK_SUPPORT ==1) 
   WORD        IsLinkClockPresented    : 1;
#endif   
}HdmiDviStatus;



typedef struct
{
   BYTE                             InputConnector;
   HdmiDviStatus                    AppStatus;
   ST_DeviceName_t                  DeviceName;
   STHDMIRX_Handle_t                DevHandle;
   STHDMIRX_InitParams_t            InitParams;
   STHDMIRX_OpenParams_t            OpenParams;
   STHDMIRX_TermParams_t            TermParams;
   STHDMIRX_InputConfiguration_t    InputConfig;
   STHDMIRX_InputHandle_t           InputHandle;
   STHDMIRX_Capability_t            Capability;
   STHDMIRX_SignalStatusEvtData_t   SignalStatusEvtData;
   STHDMIRX_VideoPropertyEvtData_t  VideoPropertyEvtData;
   STHDMIRX_AudioPropertyEvtData_t  AudioPropertyEvtData;
   STHDMIRX_InfoUpdateEvtData_t     InfoUpdateEvtData;
   STHDMIRX_Event_t                 Event ;
   ST_ErrorCode_t                   Error;
#if (LINK_CLOCK_CHECK_SUPPORT ==1)   
   WORD                             LinkClockCnt[3];
   BYTE                             LinkClockCntIndex;
#endif   
#if (BACKUP_INFODATA == 1)
   STHDMIRX_VSInfo_t                VSInfo;
   STHDMIRX_SPDInfo_t               SPDInfo;
   STHDMIRX_MPEGSourceInfo_t        MPEGInfo;
   STHDMIRX_ISRCInfo_t              ISRCInfo;
   STHDMIRX_ACPInfo_t               ACPInfo;
   STHDMIRX_GBDInfo_t               GBDInfo;
#endif
#if (ENABLE_MIN_PORT_OFF_LIMITATION == 1)
   DWORD                            PrevPortOffTime;
#endif
}gmt_HdmiDviInputPort;


typedef enum
{
   HDMIRX_VID_VDDS=0,
   HDMIRX_AUD_VDDS
} STHDMIRX_DdsSrcTypes_t;


#if (IBD_CONSISTENCE_CHECK!=0)
typedef struct
{
   WORD  Pre_HActive;
   WORD  Pre_VActive;
}gmt_IbdRecheck;
#endif
// #define HDMI_HOT_PLUG_DETECT_MODEL

#define HDMI_DAC_USED				4		// how many dac will be used on the board
#define HDMI_SPDIF_USED				1		// enable/disable SPDIF

#define HDMI_FR_FL_DAC				gmd_HDMI_AUD_DAC_0	// DAC number for front left and front right channel
#define HDMI_FC_FLE_DAC				gmd_HDMI_AUD_DAC_1	// DAC number for front center and low frequency effect channel
#define HDMI_RR_RL_DAC				gmd_HDMI_AUD_DAC_2	// DAC number for rear right and rear left/rear center channel
#define HDMI_RRC_RLC_DAC			gmd_HDMI_AUD_DAC_3	// DAC number for front/rear right center and front/rear left center channel

#define HDMI_SWAP_FR_FL_DAC		0		// swap channels
#define HDMI_SWAP_FC_FLE_DAC		0		// swap channels
#define HDMI_SWAP_RR_RL_DAC		0		// swap channels
#define HDMI_SWAP_RRC_RLC_DAC		0		// swap channels

#define HDMI_I2S_MSB_FIRST			0
#define HDMI_I2S_LEFT_ALIGNED		0
#define HDMI_I2S_WORD_CLK_MODE  	1

#define HDMI_DEFAULT_DAC_SIZE		24		// if dac size undefined in AVI info frame
#define HDMI_SPDIF_DAC_SIZE		32

#define FIFO_BUSY_WAIT_LIMIT        2000    // 44 mks for 45 MHz, 74 mks for 27 MHz
#define RX_HDCP_SHA_MIN_CHUNK_SIZE  64
#define BSTATUS_HDMI_MODE           BIT12

//******************************************************************************
//  E X T E R N S
//******************************************************************************


//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************
void HdmiDviInit(void);
void HdmiDviHandler(void);
void HdmiDviInputPortInit(gmt_PHY_CH B_Channel,BYTE InputConnector);
void HdmiDviCloseInactivePort(void);
void HdmiDviCloseInputPort(BYTE InputConnector);
BYTE GetInPortConfigArrayLocation(BYTE InputConnector);


gmt_CEA861_AVI_Y gm_HdmiGetAviColorSpace(BYTE InputConnector);
BOOL gm_HdmiIsStableState(BYTE InputConnector);
BOOL gm_HdmiIsTmingMatched(gmt_PHY_CH Channel, gmt_FrontEndSignalTiming_t* Ptr_Timing);
BOOL gm_IsHdmi3DTiming(BYTE InputConnector);
BOOL gm_IsHdmiTiming(BYTE InputConnector);
gmt_3D_INPUT_FORMAT_TYPE gm_HdmiGet3DFormat(BYTE InputConnector);
gmt_COLOR_DEPTH gm_HdmiGetColorDepth(BYTE InputConnector);
gmt_RGB_Quantization gm_HdmiGetRGBQuanRange(BYTE InputConnector);
gmt_YCC_Quantization gm_HdmiGetYCCQuanRange(BYTE InputConnector);
//BOOL IsHdmiDviLibReportStable(BYTE InputConnector);
BYTE gm_HdmiGetAviPixelRepetition(BYTE InputConnector);
gmt_CEA861_AVI_S gm_HdmiGetAviScanInfo(BYTE InputConnector);
STHDMIRX_ColorimetryStd_t HdmiGetColorimetry(BYTE InputConnector);
STHDMIRX_RGBQuantRange_t HdmiGetRGBQuantRange(BYTE InputConnector);
STHDMIRX_YCQuantRange_t HdmiGetYCQuantRange(BYTE InputConnector);
BOOL HdmiDviGetSignalTiming(BYTE InputConnector, gmt_FrontEndSignalTiming_t* Ptr_Timing);
BOOL HdmiDviGetSignalTimingExt(BYTE InputConnector, gmt_FrontEndSignalTiming_t* Ptr_Timing);

BOOL HdmiDviGetAudioProperty(BYTE InputConnector, STHDMIRX_AudioProperty_t* pAudioProp);
BOOL HdmiGetAudioChannelStatus(BYTE InputConnector,  STHDMIRX_AudioChannelStatus_t*	ChannelStatus);

#if (LINK_CLOCK_CHECK_SUPPORT ==1) 
BOOL HdmiDviIsLinkClockPresent(BYTE InputConnector);
#endif


#if DVI_1ST_MODESETUP_FB
BOOL IsDVIModeSetupFB(BYTE InputConnector);
#endif
void GeneralHdmiDviPowerUp(void);
void GeneralHdmiDviPowerDown(void);
void HdmiDviPowrUpInputPort(BYTE InputConnector);
void HdmiDviPowrDownInputPort(BYTE InputConnector);
#if (HDMI_DVI_TIMING_LOSS_PATCH==1)
BOOL IsHdmiDviTimingChangeFromStable2Unstable(BYTE InputConnector);
#endif

#if (ENABLE_MIN_PORT_OFF_LIMITATION==1)
void HdmiDvi_SendAC_ON_Notice(void);
#endif 

void HdmiDviHpdControl(BYTE InputConnector, BYTE b_Set);
//void HdmiSetHPDPinToLow(gmt_UserDefConnNames B_InputConnector);
// void HdmiSetHPDPinToHigh(gmt_UserDefConnNames B_InputConnector);
//BOOL HdmiGetHPDPinStatus(gmt_UserDefConnNames B_InputConnector);
void HdmiSetHpdPulseStart(gmt_PHY_CH Channel,gmt_UserDefConnNames B_InputConnector, WORD w_Timex10ms);

void HdmiDviPrintSignalProperty(BYTE InputConnector);
void HdmiDviPrintVideoProperty(BYTE InputConnector);
void HdmiDviPrintAudioProperty(BYTE InputConnector);

void HdmiDviSetCustomEQ(BYTE InputConnector, BYTE EqValue);
#if (FEATURE_HDCP_REPEATER == ENABLE)
void HdmiRxHdcpRptrRxEntry (U32 RxDeviceIdentifier_32,HdcpRepeaterRxRequest_t	RxRequest_e,U8 * RxRequestData_pv);
#endif
void far HdmiDvi_Interrupt(void);
void far HdmiDvi_MiniIsrHandler(void);

#if (USE_HDMIDVI_AFR_DETECT_INTERRUPT!=0)
void HdmiDviEnableAfrInterrupt(BYTE InputConnector, BYTE b_Flag);
#endif

BOOL HdmiGetITContentDoOverScan(BYTE InputConnector);//20120719 Add by KevinChen for ITC flage do overscan

#if (DEBUG_HDMI_PRINT_TIMING_DISPLAY_TIME!=0)
void SetHdmiDebugDisplayTime(void);
DWORD GetHdmiDebugDisplayTime(void);
#endif

#if DEBUG_HDMI_CPHY_CAPTURE !=0
void PrintCphyRegs(void);
void StopCaptureCphyRegs(void);
#endif

#if DEBUG_HDMI_EQ_BATHTUB !=0
void Hdmi_RunEQ_Bathtub(BYTE InputConnector);
#endif


#else   // (FEATURE_HDMI==ENABLE)
BOOL gm_IsHdmiTiming(BYTE InputConnector);



#endif // (FEATURE_HDMI==ENABLE)
#endif //__HDMI_APP_H__
