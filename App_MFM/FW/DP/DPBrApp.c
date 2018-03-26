/*
	$Workfile:   DPBrApp.c  $
	$Revision: 1.217 $
	$Date: 2013/06/14 02:38:01 $
*/
#define __DPBRAPP_C__

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
// MODULE:      DPBrApp.c
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#include "System\all.h"
#if (FEATURE_DISPLAY_PORT == ENABLE)
//******************************************************************************
//  D E F I N E S
//******************************************************************************

//#define _DEBUG_MSG
#ifdef _DEBUG_MSG
#define msg(x,y)        gm_Print(x,y)
#define msg_ii(x,y,z)   gm_Print(x,y,z)
#else
#define msg(x,y)
#define msg_ii(x,y,z)
#endif

#define REV_CODE_2_0          0x01        // Cut 2.0
#define REV_CODE_2_1          0x02        // Cut 2.1

#define FIRMWARE_MAJOR_REVISION			0
#define FIRMWARE_MINOR_REVISION			1


#define SetDpRx0_As_DP1_1                             0
#define PatchForNoVideoFlagNotPresented               0    //Can be set to 0 after REL0.11
#define UseDpRxAfrControl                             1
#define Rx0Edid_From_Rx0_AUXI2CRequest                0
#define Patch_Of_DPRX_AUX2I2C_REQ_IRQ_EN              0
#define PATCH_OF_RX1_DPRX_ENHANCED_BS                 0
#define PATCH_OF_GET_INTERLACED_FLAG                  0    //Can be set to 0 after REL0.21
#define PATCH_OF_MS_N_ZERO_CHECK                      0    //Can be set to 0 after REL0.21
#define PATCH_OF_NOVIDFLAG_UNSTABLE_VB                1    //NoVidFlag is unstable if vertical blanking period is too long
#define PATCH_OF_ASTRO_T1075_INTERLACE_ERROR          0    //Astro #T1075 (1080i@50) Interlace flag mistake
#define PATCH_OF_AUX_GAIN_FOR_INTEL_SERIES4_DP        1    //Defualt Aux Gain is not enough for Intel Series 4 DP 
#define Patch_Of_VCLK_CORR_EN_In_DpReInit             1    //Fixed PIP video fifo overflow and cause main display crash when DpReInit. 
#define PATCH_DPTX_AUD_EN_WAIT_FOR_PORT_ACTIVE        1    //Fixed IdpTx Audio is not enabled due to IdpTx is not activated yet during transition state
#define PATCH_DPTX_NO_AUD_BY_ZERO_OF_SDP_AUD_M_MEAS   0    //Fixed IdpTx Audio is occasionally no Audio when DPTX_SDP_AUD_M_MEAS is 0
//******************************************************************************
//  E X T E R N S
//******************************************************************************
//BOOL testbypass=TRUE;
PortStructure_t * DP_EXT_FUNC DP12_SetCurrentTxByPortId(U8 PortId_8);
BOOL far GetLogicalPortBksv(BYTE far * Bksv_p8);
extern BYTE DpRxEDIDBuffer[EDID_BUFFER_SIZE];
#if PATCH_OF_UNIQUE_EDID  
extern void PatchOfUniqueEDID(void);
#endif
extern void DpRxLoadEdid2Buf(void); 

#if defined(ST_4K2K_93xx_BOARD) && defined(DP_HUB_2_LOG_PORTS)
extern BYTE code DpRxEDIDBuffer_1stLogPort[EDID_BUFFER_SIZE_Mst_R];
extern BYTE code DpRxEDIDBuffer_2ndLogPort[EDID_BUFFER_SIZE_Mst_L];
#endif
//******************************************************************************
//  G L O B AL
//******************************************************************************
#define	APP_DP12_STANDARD_MAJOR_REVISION    0
#define	APP_DP12_STANDARD_MINOR_REVISION    21
//******************************************************************************
//  L O C A L S
//******************************************************************************
#define        EDID_ROM_ADDR      0xA0
#define        E_DDC_ADDR         0x60

#ifdef DP_HUB_2_LOG_PORTS
static U16     I2cAddress[2]  = {0,0};
   #if (DISPLAY_ID_SUPPORT !=0)
   static U8      EdidSegment[2] = {0,0};  //    // 1 segment is 256-byte 
   #endif
#else
static U16     I2cAddress[1]  = {0};
   #if (DISPLAY_ID_SUPPORT !=0)
   static U8      EdidSegment[1] = {0};              //    // 1 segment is 256-byte
   #endif
#endif


#if defined(ST_4K2K_93xx_BOARD) && defined(DP_HUB_2_LOG_PORTS)
#define DEFAUL_NUM_VC_ALLOCATED        2       // 2 streams
#define AthenaL_WaitMstInfoTimex10ms   150    // 1500ms
#define AthenaR_WaitMstInfoTimex10ms   30    // 300ms
static U8 DpRx0_MstInfoUpdated=FALSE;
static U8 DpRx0_NumberVirtualChannelsAllocated=DEFAUL_NUM_VC_ALLOCATED;
static U8 DpRx0_WaitMstInfoTimerStarted=FALSE;
static U8 DpRx0_Ready2UpdateMstInfo=FALSE;  
#endif

#if (DP_TOTAL_NUM_UPSTRM_PORTS!=0)
static gmt_DpRxConfig  DpInp[DP_TOTAL_NUM_UPSTRM_PORTS];
#endif

#if (DEBUG_DP_PRINT_TIMING_DISPLAY_TIME!=0)
static DWORD dw_DebugDisplayTimeDp=0;
static BYTE b_StablePrnt=FALSE;
#endif

#if (DEBUG_DISPLAY_TIME_MEASUREMENT == ENABLE)
static BYTE b_StablePrintEn=TRUE;
#endif


#if (ENABLE_DPCD_SINK_TEST_SLEEP_DELAY != 0)
static void DpcdTestSinkMonitor(BYTE InputConnector);
#endif

#if (DP_NUM_SST_ONLY_TX != 0) //&& (NON_SCALING_OUTPUT ==0)
static BOOL SstOnlyTx_EnVdoFlag = FALSE;
#endif

static U8 Pre_PortIdToActivate_8=MstTxPort;


#if ENABLE_HPD_OVERRIDED
   #if DP_RL_NUM_UPSTRM_PORTS != 0
   static BOOL Rx0_ForceHpdLow_Flag=TRUE;
   #endif
   #if DP_NUM_SST_ONLY_RX != 0
   static BOOL Rx1_ForceHpdLow_Flag=TRUE;
   #endif
#endif

#if  (PATCH_DPTX_AUD_EN_WAIT_FOR_PORT_ACTIVE!=0) && (DP_NUM_SST_ONLY_TX != 0)
typedef enum
{
	DpTxAudioEnFlagNone=0,
   DpTxAudioIsWaitingToBeEnabled=BIT1,
   DpTxAudioIsWaitingToBeMuted=BIT2,
   DpTxAudioIsWaitingToBeUnMuted=BIT3,
}DptxAudioEnableFlag;
BYTE DptxAudioEnFlag=DpTxAudioEnFlagNone;
DP_AudioConfig_t DpTxAudioConfig;
#endif 


#ifdef ST_4K2K_93xx_BOARD
#ifdef DP_HUB_2_LOG_PORTS

//#define TWO_LOGICAL_PORT_DEBUG

#ifdef TWO_LOGICAL_PORT_DEBUG
#define Two_LogicalPort_msg(x,y)        gm_Print(x,y)
#else
#define Two_LogicalPort_msg(x,y)
#endif

#if (ENABLE_DPRX0_RE_MODE_SETUP)
BOOL ForceDP_ReModeSetup = FALSE;
BOOL EnableReModeSetupStart = FALSE;
#endif
#endif
#endif

BOOL FAR  DP_GetLogicalPortBksv (U8 *Bksv_p8);
void FAR DP_ExecuteLogicalPortI2cTransaction(U8 AppPortId, Aux_data_buffer_t * I2cTransaction, U8 stopbit);

BYTE Rx_GetSrcPowerStatus(BYTE ApplicationPort_b);
BYTE Rx_GetCableStatus(BYTE ApplicationPort_b);

BOOL far Rx0_AUXI2CRequest(U16 W_Addr, BOOL B_IsRead, U8 *B_Chunk, U16 W_Len, U8 B_Stop);
void Rx0_AudioDetectedNotify(void);

BOOL far Rx1_AUXI2CRequest(U16 W_Addr, BOOL B_IsRead, U8 *B_Chunk, U16 W_Len, U8 B_Stop);
void Rx1_AudioDetectedNotify(void);

//void far DpTxExt_LinkAutomationPatGen(U8 PortId_8,DP_MsaParameters_t DP_RAM * LinkAutomationDpcdMsaParams_ps);
void EXT_FUNC DpTxExt_Sha1Reset(void);
void EXT_FUNC DpTxExt_Sha1Input(const U8 far * Bp_Message, U16 W_Length);
U8 DP_RAM * EXT_FUNC DpTxExt_Sha1Result(void);
void far Rx0_ConfigChange(U16 ConfigParam_16);
void far Rx1_ConfigChange(U16 ConfigParam_16);
BOOL far SetVoltageLvl_cb(DPRX_VoltageLvl_t VoltLvl_t);

#define INIT_VOLTLVL	DP_VOLTAGELVL_3_3V

#if DP_NUM_SST_ONLY_RX != 0
BYTE SST_OUI[]={0x0, 0x80, 0xE1,'D','p', '1', '.', '1', '\0'}; // For ST
#endif
#if DP_RL_NUM_UPSTRM_PORTS != 0
BYTE MST_OUI[]={0x0, 0x80, 0xE1,'D','p', '1', '.', '2', '\0'}; // For ST
#endif


#if DP_NUM_SST_ONLY_RX != 0
DPRX_SstOnlyInitParams_t code DPRX_SstOnlyInitParams_s =
{
	(DP_BaseAddr_t)DP12RX1_CONTROL,							//FEMemBase_p
	(DP_BaseAddr_t)CPHY2_AUX_PHY_CTRL,						//PhyMemBase_p
	(DP_BaseAddr_t)LINEAR_TO_FPTR(DPRX2_DPCD_BASE_ADDR),	//DpcdMemBase_p
	(DP_BaseAddr_t)DP12RX_KEYHOST_CTRL,						//KeyHostReg_p
	(BOOL)TRUE,												//AutoCalibEnable_b
	(DP_Standard_t)DP_Standard_Dp,							//DpStandard_e
	(DP_Version_t)DP_Version1_1,							//DpVersion_e
	(DPRX_EqualizationLevel_t)DP_EqualizationLevel_1_5dB,	//EqualizationLevel_e
	(BOOL)FALSE,											//AuxActiveInPowerOff_b
	(U16)3000,												//HpdLongPulseWidth_16
	(DP_MainLinkRate_t)DP_MainLinkRate_Hbr,					//LinkRate_e
	(DP_MainLinkNumLanes_t)DP_MainLinkNumLanes_4Lanes,		//LaneCount_e

	 //callbacks										
	(DPRX_DpRxExtFuncClkPower_t)NULL,
	(DPRX_DpRxExtFuncGetCableStatus_t)Rx_GetCableStatus, //Rx_GetSrcPowerStatus,
	(DPRX_DpRxExtFuncPwrEventNotification_t)NULL,
	(DPRX_DpRxExtFuncAUXI2CHandler_t)Rx1_AUXI2CRequest,
	(DPRX_DpRxExtFuncPWStateUpdate_t)NULL,

	(DP_Ext_Sha1Reset_t)DpTxExt_Sha1Reset,					//Sha1Reset_pf
	(DP_Ext_Sha1Input_t)DpTxExt_Sha1Input,					//Sha1Input_pf
	(DP_Ext_Sha1Result_t)DpTxExt_Sha1Result,				//Sha1Result_pf	
	
	//Version
   APP_DP12_STANDARD_MAJOR_REVISION,						//DpcdMajorVersion_8
   APP_DP12_STANDARD_MINOR_REVISION,						//DpcdMinorVersion_8;
   (BOOL)TRUE,												//HdcpPordKeys
   SST_OUI,												//DpcdOui_8
   INIT_VOLTLVL,											//VoltageLevel
   (DP_Ext_RXSetVoltLvl_t)SetVoltageLvl_cb,				//DPSetVoltLvl_pf


	(DP_BaseAddr_t)DP12RX1_BE_CONTROL,						//BEMemBase_p
	(DP_BaseAddr_t)LINEAR_TO_FPTR(DPRX2_SDP_MEM_BASE),  	//SdpMemBase_p
	(BOOL)TRUE,												//IdpUseHpd_b
	(DPRX_AudioClockMode_t)DPRX_AudioClockMode_384_fs,  	//DPAudioClockMode_e
	(BOOL)FALSE,											//Aux2I2Cenable_b
	(DPRX_DpRxExtFuncAudioDetectedNotify_t)NULL,
	(DP_Ext_DpConfigChange_t)Rx1_ConfigChange,
};
#endif

#if DP_NUM_SST_ONLY_TX != 0
DPTX_SstOnlyInitParams_t code DPTX_SstOnlyInitParams_s =
{
	(DP_BaseAddr_t)DP12TX_FE_CONTROL,							//FeMemBase_p
	(DP_BaseAddr_t)DPTX_AUX_PHY_CTRL,						//PhyMemBase_p
	(DP_BaseAddr_t)DP12TX_KEYHOST_CTRL,						//KeyHostReg_p

	(DP_Standard_t)DP_Standard_Dp,							//Standard_e
	(DP_MainLinkRate_t)DP_MainLinkRate_Hbr,				//LinkRate_e
	(DP_MainLinkNumLanes_t)DP_MainLinkNumLanes_4Lanes,		//LaneCount_e
	(BOOL)TRUE,											//AutoCalibEnable_b
	(U8)0,													//Gain_8
	(U8)0,													//PreEmp_8
	// callbacks	
	(DPTX_SetExtClkPower_t)NULL,
	(DPTX_SetExtClkSpreadSpectrum_t)NULL,
	(DP_Ext_Sha1Reset_t)DpTxExt_Sha1Reset,
	(DP_Ext_Sha1Input_t)DpTxExt_Sha1Input,
	(DP_Ext_Sha1Result_t)DpTxExt_Sha1Result,
	(DPTX_Ext_GetHardwareVersion_t)NULL,
	(DPTX_Ext_GetFirmwareVersion_t)NULL,
	(BOOL)TRUE,												// HdcpProdKeys
	(DPTX_Ext_GetHpdStatus_t)NULL,

//SST Specific Init
   (DP_BaseAddr_t)DPTX_BE_CONTROL,				      //BeMemBase_p
	(DP_BaseAddr_t)NULL, 									   //PatGenMemBase_p
  	(DPTX_Ext_LinkAutomationPatGen_t)NULL,
   (DPTX_Ext_ExecuteLogicalPortI2c_t)NULL, 		//DP_ExecuteLogicalPortI2cTransaction,
  	(DP_BaseAddr_t)LINEAR_TO_FPTR(DPTX_SDP_MEM_BASE),	//SDPMemBase_p
	(BOOL)FALSE,											      //IdpHdpMode_b
};	
#endif

#if DP_RL_NUM_UPSTRM_PORTS != 0
DP12_RxMstCapableInitParams_t code DPRX_MstCapableInitParams_s =
{
	(DP_BaseAddr_t)DP12RX0_CONTROL,							//FEMemBase_p
   (DP_BaseAddr_t)CPHY0_AUX_PHY_CTRL,						//PhyMemBase_p	
	//(DP_BaseAddr_t)CPHY0_AUX_PHY_CTRL,						//PhyMemBase_p   // 20130103
	(DP_BaseAddr_t)LINEAR_TO_FPTR(DPRX_DPCD_BASE_ADDR),		//DpcdMemBase_p
	(DP_BaseAddr_t)DP12RX_KEYHOST_CTRL,						//KeyHostReg_p;
	(BOOL)TRUE,												//AutoCalibEnable_b
	(DP_Standard_t)DP_Standard_Dp,							//DpStandard_e
#if SetDpRx0_As_DP1_1	
	(DP_Version_t)DP_Version1_1,							//DpVersion_e
#else
    (DP_Version_t)DP_Version1_2,
#endif
	(DPRX_EqualizationLevel_t)DP_EqualizationLevel_1_5dB,	//EqualizationLevel_e
	(BOOL)FALSE,											//AuxActiveInPowerOff_b
	(U16)3000,												//HpdLongPulseWidth_16
#if SetDpRx0_As_DP1_1	
	(DP_MainLinkRate_t)DP_MainLinkRate_Hbr,					//LinkRate_e
#else
    (DP_MainLinkRate_t)DP_MainLinkRate_Hbr2,
#endif
	(DP_MainLinkNumLanes_t)DP_MainLinkNumLanes_4Lanes,		//LaneCount_e

     //callbacks										
	(DPRX_DpRxExtFuncClkPower_t)NULL,
	(DPRX_DpRxExtFuncGetCableStatus_t)Rx_GetCableStatus ,//Rx_GetSrcPowerStatus,
	(DPRX_DpRxExtFuncPwrEventNotification_t)NULL,
	(DPRX_DpRxExtFuncAUXI2CHandler_t)Rx0_AUXI2CRequest,	
	(DPRX_DpRxExtFuncPWStateUpdate_t)NULL,

	(DP_Ext_Sha1Reset_t)DpTxExt_Sha1Reset,					//Sha1Reset_pf
	(DP_Ext_Sha1Input_t)DpTxExt_Sha1Input,					//Sha1Input_pf
	(DP_Ext_Sha1Result_t)DpTxExt_Sha1Result,				//Sha1Result_pf
	//Version
   APP_DP12_STANDARD_MAJOR_REVISION,						//DpcdMajorVersion_8
   APP_DP12_STANDARD_MINOR_REVISION,						//DpcdMinorVersion_8
   (BOOL)TRUE,												//HdcpPordKeys
   MST_OUI,
   INIT_VOLTLVL,											//Voltage Level
   (DP_Ext_RXSetVoltLvl_t)SetVoltageLvl_cb,				//DPSetVoltLvl_pf

	(DP_BaseAddr_t)LINEAR_TO_FPTR(DPRX_DPCD_UPREQ_BASE),	//DpcdUpReqBase_p
	(DP_BaseAddr_t)LINEAR_TO_FPTR(DPRX_DPCD_UPREP_BASE),	//DpcdUpRepBase_p
	(DP_BaseAddr_t)LINEAR_TO_FPTR(DPRX_DPCD_DOWNREQ_BASE),	//DpcdDownReqBase_p
	(DP_BaseAddr_t)LINEAR_TO_FPTR(DPRX_DPCD_DOWNREP_BASE),	//DpcdDownRepBase_p
	(DP_BaseAddr_t)DP12RX_RL_CONFIG,						//BEMemBase_p
	(DP_BaseAddr_t)DP12_GTC_SOFT_RESET						//GTCMemBase_p
};

#if (DPRX0_DYNAMIC_INIT_BY_NVRAM!=0)
DP12_RxMstCapableInitParams_t code DPRX_MstDpCapableInitParams_s_DP11 =
{
	(DP_BaseAddr_t)DP12RX0_CONTROL,							//FEMemBase_p
   (DP_BaseAddr_t)CPHY0_AUX_PHY_CTRL,						//PhyMemBase_p	

	//(DP_BaseAddr_t)CPHY0_AUX_PHY_CTRL,						//PhyMemBase_p		// 20130103
	(DP_BaseAddr_t)LINEAR_TO_FPTR(DPRX_DPCD_BASE_ADDR),		//DpcdMemBase_p
	(DP_BaseAddr_t)DP12RX_KEYHOST_CTRL,						//KeyHostReg_p;
	(BOOL)TRUE,												//AutoCalibEnable_b
	(DP_Standard_t)DP_Standard_Dp,							//DpStandard_e
	(DP_Version_t)DP_Version1_1,							//DpVersion_e
	(DPRX_EqualizationLevel_t)DP_EqualizationLevel_1_5dB,	//EqualizationLevel_e
	(BOOL)FALSE,											//AuxActiveInPowerOff_b
	(U16)3000,												//HpdLongPulseWidth_16
	(DP_MainLinkRate_t)DP_MainLinkRate_Hbr,					//LinkRate_e
	(DP_MainLinkNumLanes_t)DP_MainLinkNumLanes_4Lanes,		//LaneCount_e

     //callbacks										
	(DPRX_DpRxExtFuncClkPower_t)NULL,
	(DPRX_DpRxExtFuncGetCableStatus_t)Rx_GetCableStatus ,//Rx_GetSrcPowerStatus,
	(DPRX_DpRxExtFuncPwrEventNotification_t)NULL,
	(DPRX_DpRxExtFuncAUXI2CHandler_t)Rx0_AUXI2CRequest,	
	(DPRX_DpRxExtFuncPWStateUpdate_t)NULL,

	(DP_Ext_Sha1Reset_t)DpTxExt_Sha1Reset,					//Sha1Reset_pf
	(DP_Ext_Sha1Input_t)DpTxExt_Sha1Input,					//Sha1Input_pf
	(DP_Ext_Sha1Result_t)DpTxExt_Sha1Result,				//Sha1Result_pf
	//Version
	APP_DP12_STANDARD_MAJOR_REVISION,						//DpcdMajorVersion_8
	APP_DP12_STANDARD_MINOR_REVISION,						//DpcdMinorVersion_8
   (BOOL)TRUE,												//HdcpPordKeys
   MST_OUI,
   INIT_VOLTLVL,											//Voltage Level
   (DP_Ext_RXSetVoltLvl_t)SetVoltageLvl_cb,				//DPSetVoltLvl_pf

	(DP_BaseAddr_t)LINEAR_TO_FPTR(DPRX_DPCD_UPREQ_BASE),	//DpcdUpReqBase_p
	(DP_BaseAddr_t)LINEAR_TO_FPTR(DPRX_DPCD_UPREP_BASE),	//DpcdUpRepBase_p
	(DP_BaseAddr_t)LINEAR_TO_FPTR(DPRX_DPCD_DOWNREQ_BASE),	//DpcdDownReqBase_p
	(DP_BaseAddr_t)LINEAR_TO_FPTR(DPRX_DPCD_DOWNREP_BASE),	//DpcdDownRepBase_p
	(DP_BaseAddr_t)DP12RX_RL_CONFIG,						//BEMemBase_p
	(DP_BaseAddr_t)DP12_GTC_SOFT_RESET						//GTCMemBase_p
};
#endif //(DPRX0_DYNAMIC_INIT_BY_NVRAM!=0)

#endif // DP_RL_NUM_UPSTRM_PORTS != 0

#if DP_RL_NUM_DOWNSTREAM_PHYSICAL_PORTS != 0
DP12_TxMstInitParams_t code DPTX_MstCapableInitParams_s =
{
	(DP_BaseAddr_t)DP12TX_FE_CONTROL,						//FeMemBase_p
	(DP_BaseAddr_t)DPTX_AUX_PHY_CTRL,					//PhyMemBase_p
	(DP_BaseAddr_t)DP12TX_KEYHOST_CTRL, 				//KeyHostReg_p
	(DP_Standard_t)DP_Standard_Dp,						//Standard_e
	(DP_MainLinkRate_t)DP_MainLinkRate_Hbr2,			//LinkRate_e
	(DP_MainLinkNumLanes_t)DP_MainLinkNumLanes_4Lanes,	//LaneCount_e
	(BOOL)TRUE,										//AutoCalibEnable_b
	(U8)0,												//Gain_8
	(U8)0,												//PreEmp_8
	// callbacks	
	(DPTX_SetExtClkPower_t)NULL,
	(DPTX_SetExtClkSpreadSpectrum_t)NULL,
	(DP_Ext_Sha1Reset_t)DpTxExt_Sha1Reset,
	(DP_Ext_Sha1Input_t)DpTxExt_Sha1Input,
	(DP_Ext_Sha1Result_t)DpTxExt_Sha1Result,
	(DPTX_Ext_GetHardwareVersion_t)NULL,
	(DPTX_Ext_GetFirmwareVersion_t)NULL,
	(BOOL)TRUE,											//HdcpPordKeys
	(DPTX_Ext_GetHpdStatus_t)NULL,

	(DP_BaseAddr_t)DP12TX2_RL_CONFIG,					//BeMemBase_p
	(DP_BaseAddr_t)NULL,   								//PatGenMemBase_p
  	(DPTX_Ext_LinkAutomationPatGen_t)NULL,
   (DPTX_Ext_ExecuteLogicalPortI2c_t)DP_ExecuteLogicalPortI2cTransaction,
};
#endif

#if DP_RL_NUM_DOWNSTREAM_LOGICAL_PORTS != 0
DP12_DigitalTxInitParams_t code DPTX_LogicalInitParams_s =
{
//DPTX MST BE Params
   (DP_BaseAddr_t)DP12TX1_RL_CONFIG,
   (DP_BaseAddr_t)NULL,
  	(DPTX_Ext_LinkAutomationPatGen_t)NULL,
   (DPTX_Ext_ExecuteLogicalPortI2c_t)DP_ExecuteLogicalPortI2cTransaction,  	
   {
      2,																	// SdpStreams_t							SdpStreamsInfo.Number_SDP_Stream_Sinks1;
      1,																	// SdpStreams_t							SdpStreamsInfo.Number_SDP_Streams1;
   },
   (DPTX_SetExtClkPower_t)NULL,
   (DPTX_SetExtClkSpreadSpectrum_t)NULL,
   (DPTX_Ext_LinkAutomationPatGen_t)NULL,
//   (DPTX_Ext_ExecuteLogicalPortI2c_t)DP_ExecuteLogicalPortI2cTransaction,
   (DP_Ext_GetLogicalPortBksv_t)DP_GetLogicalPortBksv,

//DPRX SST BE Params "Note:Do not add DPTX params below this line"
   (DP_BaseAddr_t)DP12RX0_BE_CONTROL,
   (DP_BaseAddr_t)LINEAR_TO_FPTR(DPRX_SDP_MEM_BASE),
   (DPRX_AudioClockMode_t)DPRX_AudioClockMode_384_fs,
   (DP_Ext_DpConfigChange_t)Rx0_ConfigChange,
};
#endif

#if (DP_RL_NUM_UPSTRM_PORTS != 0) && (DPRX0_DYNAMIC_INIT_BY_NVRAM!=0)
static void Patch_Of_DpReInit(void)
{
   *LINEAR_TO_FPTR_B(0x1F202) = 0x0;
	*LINEAR_TO_FPTR_B(0x1F203) = 0x0;
	gm_ClearRegBitsByte(DP12RX_RL_CONFIG,DPRX_TYPE);
}
#endif

#if (PATCH_OF_NOVIDFLAG_UNSTABLE_VB!=0)
   #define THRESH_NOVID_VBLANK_MIN  84

   #if DP_RL_NUM_UPSTRM_PORTS != 0
   static BYTE Rx0_ReCheckNoVidFlag=FALSE;
   #endif
   #if DP_NUM_SST_ONLY_RX != 0
   static BYTE Rx1_ReCheckNoVidFlag=FALSE;
   #endif

static void Set_ReCheckNoVidFlag(BYTE InputConnector, BYTE bSet)
{
#if (DP_TOTAL_NUM_UPSTRM_PORTS == 0)
   UNUSED_PARAMETER(InputConnector);
   UNUSED_PARAMETER(bSet);
#endif
   #if DP_RL_NUM_UPSTRM_PORTS != 0
   if(gm_GetDpDigitalID(InputConnector)==DID_DPRX0)
   {
      Rx0_ReCheckNoVidFlag=bSet;
      return;
   }
   #endif

   #if DP_NUM_SST_ONLY_RX != 0
   if(gm_GetDpDigitalID(InputConnector)==DID_DPRX1)
   {
      Rx1_ReCheckNoVidFlag=bSet;
      return;
   }
   #endif
}

static BYTE Get_ReCheckNoVidFlag(BYTE InputConnector)
{
#if (DP_TOTAL_NUM_UPSTRM_PORTS == 0)
      UNUSED_PARAMETER(InputConnector);
#endif

#if DP_RL_NUM_UPSTRM_PORTS != 0
   if(gm_GetDpDigitalID(InputConnector)==DID_DPRX0)
      return Rx0_ReCheckNoVidFlag;
#endif

#if DP_NUM_SST_ONLY_RX != 0
   if(gm_GetDpDigitalID(InputConnector)==DID_DPRX1)
      return Rx1_ReCheckNoVidFlag;
#endif

   return FALSE;
}
#endif //(PATCH_OF_NOVIDFLAG_UNSTABLE_VB!=0)

void AddToString (BYTE * FV, BYTE number)
{
   BYTE		powerOf10;
   BYTE *	startFV;
   while ((*FV != '?') && (*FV!='\0'))
   {
      FV++;
   }

   if (number > 99)
   {
      powerOf10 = 10 * 10;
   }
   else if (number > 9)
   {
      powerOf10 = 10;
   }
   else
   {
      powerOf10 = 1;
   }

   startFV = FV;
   while (*FV == '?')
   {
      if (number == 0 && startFV != FV)
      {
         *FV = ' ';
      }
      else
      {
         *FV = '0' + number / powerOf10;
         number %= powerOf10;
         powerOf10 /= 10;
      }
      FV++;
      if ((number==0)&&(powerOf10!=0))
         startFV=FV;
   }
}

#if (DISPLAY_ID_SUPPORT !=0)
void ResetEdidSegment(void)
{
   EdidSegment[0]=0;
   #ifdef DP_HUB_2_LOG_PORTS   
   EdidSegment[1]=0;
   #endif
}
#endif


void far SetDdsFreq (WORD Cdds2Freq_32)
{
	gm_WriteRegWord(CDDS2_FREQ, Cdds2Freq_32);

   if (gm_GetDpDigitalID(gB_CurrentInputConnector[gmvb_MainChannel])==DID_DPRX0)
   {
	gm_WriteRegDWord(AVDDS1_INI_FREQ, ((U32)(Cdds2Freq_32/2))<<8);
   	#if (FEATURE_AUDIO == ENABLE)
	gm_WriteRegDWord(AVDDS3_INI_FREQ, ((U32)(Cdds2Freq_32/2))<<8);
   	#endif
	}
	else if (gm_GetDpDigitalID(gB_CurrentInputConnector[gmvb_PipChannel])==DID_DPRX0)
	{
   	gm_WriteRegDWord(AVDDS2_INI_FREQ, ((U32)(Cdds2Freq_32/2))<<8);
      #if (FEATURE_AUDIO == ENABLE)
   	gm_WriteRegDWord(AVDDS4_INI_FREQ, ((U32)(Cdds2Freq_32/2))<<8);
      #endif
	}
}

WORD far GetCdds2Freq (void)
{
	return gm_ReadRegWord(CDDS2_FREQ);
}
/***********Audio Back Ground Noise Issue PATCH START**********************/
U8 far GetAVDDSTrackingError (void)
{
   if (gm_GetDpDigitalID(gB_CurrentInputConnector[gmvb_MainChannel])==DID_DPRX0)
   {
      return(gm_ReadRegByte(AVDDS3_TRACK_ERR));
}
   else if (gm_GetDpDigitalID(gB_CurrentInputConnector[gmvb_PipChannel])==DID_DPRX0)
   {
      return(gm_ReadRegByte(AVDDS4_TRACK_ERR));
   }

   return 0;
}

/***********Audio Back Ground Noise Issue PATCH END**********************/
WORD far GetRclkFreq (void)
{
   #if RCLK_FREQ_ARGS == RCLKSEL_432M
      return 432;
   #elif RCLK_FREQ_ARGS == RCLKSEL_378M
      return 378;
   #elif RCLK_FREQ_ARGS == RCLKSEL_270M
      return 270;
   #endif
}

BOOL FAR  DP_GetLogicalPortBksv (U8 *Bksv_p8)
{
	BYTE TempBuf[5];
	BYTE *BufPtr;
	if(Bksv_p8==NULL_PTR)
	   BufPtr=TempBuf;
	else
	   BufPtr=Bksv_p8;
	   
   if(GetLogicalPortBksv(BufPtr))
   {
   /*
      gm_Print("BKSV[0] = 0x%X",BufPtr[0]);
      gm_Print("BKSV[1] = 0x%X",BufPtr[1]);
      gm_Print("BKSV[2] = 0x%X",BufPtr[2]);
      gm_Print("BKSV[3] = 0x%X",BufPtr[3]);
      gm_Print("BKSV[4] = 0x%X",BufPtr[4]);
   */
      return TRUE;
   }
   else
   {
      msg("Default 2nd-BKSV!",0);
      return TRUE;
   }
}


void FAR DP_ExecuteLogicalPortI2cTransaction(U8 AppPortId, Aux_data_buffer_t * I2cTransaction, U8 stopbit)
{
#if Rx0Edid_From_Rx0_AUXI2CRequest
      UNUSED_PARAMETER(AppPortId);
      if (I2cTransaction->ReadRequest)
      {
         if (Rx0_AUXI2CRequest((U16)I2cTransaction->Addr_32, I2cTransaction->ReadRequest, (U8 *)I2cTransaction->RepBuff_p, I2cTransaction->RequestDataLen_16, !stopbit)==TRUE)
         {
            I2cTransaction->HandledDataLen_16=I2cTransaction->RequestDataLen_16;
            I2cTransaction->AuxTransactionStatus_t = AUX_REQ_SUCCESS;
         }
         else
         {
            I2cTransaction->HandledDataLen_16=0;
            I2cTransaction->AuxTransactionStatus_t = AUX_REQ_FAIL;
         }
      }
      else
      {
         if (Rx0_AUXI2CRequest((U16)I2cTransaction->Addr_32, I2cTransaction->ReadRequest, (U8 *)I2cTransaction->ReqBuff_p, I2cTransaction->RequestDataLen_16, !stopbit)==TRUE)
         {
            I2cTransaction->HandledDataLen_16=0;
            I2cTransaction->AuxTransactionStatus_t = AUX_REQ_SUCCESS;
         }
         else
         {
            I2cTransaction->HandledDataLen_16=0;
            I2cTransaction->AuxTransactionStatus_t = AUX_REQ_FAIL;
         }
      }
#else // Rx0Edid_From_Rx0_AUXI2CRequest == 0
      U8             ArrayIdx=0;
      U16            offsetAddr;
      #ifdef DP_HUB_2_LOG_PORTS
   		if(AppPortId == MstTxPort)
   		{
   			ArrayIdx = 1;
   		}
      #else
         UNUSED_PARAMETER(AppPortId);
      #endif

   #if (DISPLAY_ID_SUPPORT !=0) && (Base_CEA_DisplayID!=0)
      if ((I2cTransaction->Addr_32 == (U32)(EDID_ROM_ADDR>>1)) || (I2cTransaction->Addr_32 == (U32)(E_DDC_ADDR>>1)))
   #else
      if (I2cTransaction->Addr_32 == (U32)(EDID_ROM_ADDR>>1))
   #endif
      {
         if (I2cTransaction->ReadRequest == DP_FALSE)
         {
            I2cAddress[ArrayIdx] = (BYTE)(*I2cTransaction->ReqBuff_p);
         #if (DISPLAY_ID_SUPPORT !=0) && (Base_CEA_DisplayID!=0)   
            if(I2cTransaction->Addr_32 == (U32)(E_DDC_ADDR>>1))
            {
               EdidSegment[ArrayIdx] =(U8) ((I2cTransaction->ReqBuff_p)[0]); //for i2c cmd to access data above 256 address boundary page
            }
            offsetAddr = (((U16)EdidSegment[ArrayIdx])<<8) + ((U16)I2cAddress[ArrayIdx]%256);
         #else
            offsetAddr = ((U16)I2cAddress[ArrayIdx]%256);
         #endif
            I2cTransaction->HandledDataLen_16 = 1;
         }
         else
         {
            if(I2cTransaction->RequestDataLen_16 == 0)
            {
               if(stopbit)
               {
                  I2cAddress[ArrayIdx] = 0;
                  #if (DISPLAY_ID_SUPPORT !=0) && (Base_CEA_DisplayID!=0)      
                  EdidSegment[ArrayIdx]= 0;
                  #endif
               }
            }
            #if (DISPLAY_ID_SUPPORT !=0) && (Base_CEA_DisplayID!=0)   
               offsetAddr = (((U16)EdidSegment[ArrayIdx])<<8) + ((U16)(I2cAddress[ArrayIdx]&0x0FF));
            #else
               offsetAddr = ((U16)(I2cAddress[ArrayIdx]&0x0FF)); 
            #endif

         #ifdef DP_HUB_2_LOG_PORTS
            if(ArrayIdx==0)
               _fmemcpy(I2cTransaction->RepBuff_p, &DpRxEDIDBuffer_1stLogPort[offsetAddr], I2cTransaction->RequestDataLen_16);
            else
               _fmemcpy(I2cTransaction->RepBuff_p, &DpRxEDIDBuffer_2ndLogPort[offsetAddr], I2cTransaction->RequestDataLen_16);
         #else
               _fmemcpy(I2cTransaction->RepBuff_p, &DpRxEDIDBuffer[offsetAddr], I2cTransaction->RequestDataLen_16);
         #endif

            I2cAddress[ArrayIdx] += I2cTransaction->RequestDataLen_16;
            I2cTransaction->HandledDataLen_16 = I2cTransaction->RequestDataLen_16;

            #if (DISPLAY_ID_SUPPORT !=0) && (Base_CEA_DisplayID!=0)  
            //reset segment if read all data
            if((EdidSegment[ArrayIdx]==0x01) && (I2cAddress[ArrayIdx]==0x80))
            {
               EdidSegment[ArrayIdx] = 0;
            }
            #endif
         }

         I2cTransaction->AuxTransactionStatus_t = AUX_REQ_SUCCESS;
      }
      /*ChengLin 20130514: Enable DP1.2 DDC/CI Supporting*/
   #if (ENABLE_DP12_DDCCI_SUPPORT)
      else if (I2cTransaction->Addr_32 == (U32)(DDC2BI_ADDR_ID>>1))
      {
         //Place DDCCI process here
         DpRxDdcciMultiStreamTransaction(I2cTransaction, stopbit);
      } 
   #endif 
      else
      { 
         I2cTransaction->AuxTransactionStatus_t = AUX_REQ_FAIL;
      }
         
#endif //Rx0Edid_From_Rx0_AUXI2CRequest
}



BYTE DpRxGetPortId(gmt_UserDefConnNames b_InputConnector)
{
#if DP_TOTAL_NUM_UPSTRM_PORTS
   gmt_COMBOPHY_AID b_AID;
   if ( b_InputConnector < NUM_MASTER_LIST)
   {
      b_AID=gm_GetDpAnaPhyID(b_InputConnector);
      if (b_AID==AID_PHY0)
      {
         return LogicalTxPort;
      }
      else if (b_AID==AID_PHY2)
      {
         return SSTLegacyRxPort;
      }
   }
#else
   UNUSED_VAR(b_InputConnector);
#endif
   return 0;
}

BYTE DpRxGetPortID_Of_Phy0(void)
{
   return LogicalTxPort;
}

BYTE DpRxGetPortID_Of_Phy2(void)
{
   return SSTLegacyRxPort;
}

BYTE DpTxGetPortID(void)
{
   return iDpTxPort;
}

//******************************************************************************
// FUNCTION       : void DpInpControlInit(void)
// USAGE          : Init for DpInpControlInit
// INPUT          : None
// OUTPUT         : None
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  : 
// POST_CONDITION : 
//******************************************************************************
void DpInpControlInit(void)
{
#if (DP_TOTAL_NUM_UPSTRM_PORTS != 0)
   BYTE i=0,j;
   BYTE b_MatchedPort=0xFF;

 #if DP_RL_NUM_UPSTRM_PORTS!=0
   for(j=NUM_MASTER_LIST-1;j>0;j--)
   {
      if(gm_GetDpDigitalID(j)==DID_DPRX0)
      {
         b_MatchedPort=j;
         if(((gmt_DpProperties far *)(GetPortPropertyTable(j)))->B_Mux_Flag != TRUE)
            break;

         if((j==UserPrefCurrentInputMain)||(j==UserPrefCurrentInputPip))
            break;
      }
   }
   DpInp[i].B_CurInputCntr=b_MatchedPort;
   i++;
#endif
   
#if DP_NUM_SST_ONLY_RX!=0
   b_MatchedPort=0xFF;
   for(j=NUM_MASTER_LIST-1;j>0;j--)
   {
      if(gm_GetDpDigitalID(j)==DID_DPRX1)
      {
         b_MatchedPort=j;
         if(((gmt_DpProperties far *)(GetPortPropertyTable(j)))->B_Mux_Flag != TRUE)
            break;

         if((j==UserPrefCurrentInputMain)||(j==UserPrefCurrentInputPip))
            break;
      }
   }
   DpInp[i].B_CurInputCntr=b_MatchedPort;
#endif

#if (DP_TOTAL_NUM_UPSTRM_PORTS!=0)
   //Process PreSetup if Mux is available
   for(i=0;i<DP_TOTAL_NUM_UPSTRM_PORTS;i++)
   {
      j=DpInp[i].B_CurInputCntr;
      if(((gmt_DpProperties far *)(GetPortPropertyTable(j)))->B_Mux_Flag==TRUE)
      {
         if (((gmt_DpProperties far*)(GetPortPropertyTable(j)))->ExtPortSetup != NULL_PTR)
            ((gmt_DpProperties far*)(GetPortPropertyTable(j)))->ExtPortSetup();
      }
   }
#endif

#ifdef _DEBUG_MSG
   for(i=0;i<DP_TOTAL_NUM_UPSTRM_PORTS;i++)
      gm_Print("DpInp[%d].B_CurInputCntr=%d",i,DpInp[i].B_CurInputCntr);
#endif
#endif //DP_TOTAL_NUM_UPSTRM_PORTS
}

#if (DP_TOTAL_NUM_UPSTRM_PORTS!=0)
//******************************************************************************
// FUNCTION       : gmt_UserDefConnNames GetDpInputConnector(gmt_DPRX_DID b_DID)
// USAGE          : Called for Get Dp Input Connector
// INPUT          : gmt_DPRX_DID
// OUTPUT         : gmt_UserDefConnNames
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  : 
// POST_CONDITION : 
//******************************************************************************
gmt_UserDefConnNames GetDpInputConnector(gmt_DPRX_DID b_DID)
{
   BYTE i;
   for(i=0;i<DP_TOTAL_NUM_UPSTRM_PORTS;i++)
   {
      if(gm_GetDpDigitalID(DpInp[i].B_CurInputCntr)==b_DID)   
         return DpInp[i].B_CurInputCntr;
   }
	return 0xFF;
}



//******************************************************************************
// FUNCTION       : UpdateDpInpControl(gmt_UserDefConnNames b_InputConnector)
// USAGE          : Update InputConnector to DpInp CurInputCntr
// INPUT          : gmt_UserDefConnNames
// OUTPUT         : None
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  : 
// POST_CONDITION : 
//******************************************************************************
void UpdateDpInpControl(gmt_UserDefConnNames b_InputConnector)
{
   BYTE i;
   for(i=0;i<DP_TOTAL_NUM_UPSTRM_PORTS;i++)
   {
      if(gm_GetDpDigitalID(DpInp[i].B_CurInputCntr)==gm_GetDpDigitalID(b_InputConnector))
      {
         DpInp[i].B_CurInputCntr=b_InputConnector;
         msg_ii("DpInp[%d].B_CurInputCntr=%d",i,DpInp[i].B_CurInputCntr);
         return;
      }
   }
}

//******************************************************************************
// FUNCTION       : IsInputConnectorReady(gmt_UserDefConnNames b_InputConnector)
// USAGE          : Check the input connector is already ready or not.
// INPUT          : gmt_UserDefConnNames
// OUTPUT         : TRUE / FALSE
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  : 
// POST_CONDITION : 
//******************************************************************************
BOOL IsInputConnectorReady(gmt_UserDefConnNames b_InputConnector)
{
   BYTE i;
   for(i=0;i<DP_TOTAL_NUM_UPSTRM_PORTS;i++)
   {
      if(DpInp[i].B_CurInputCntr==b_InputConnector)   
         return TRUE;
   }
	return FALSE;
}
#else //DP_TOTAL_NUM_UPSTRM_PORTS
void UpdateDpInpControl(gmt_UserDefConnNames b_InputConnector)
{
   UNUSED_PARAMETER(b_InputConnector);
}

BOOL IsInputConnectorReady(gmt_UserDefConnNames b_InputConnector)
{
   UNUSED_PARAMETER(b_InputConnector);
   return FALSE;
}
#endif // DP_TOTAL_NUM_UPSTRM_PORTS
//******************************************************************************
// FUNCTION       : DWORD far DP_Ext_GetSystemTime (void)
// USAGE          : For DP1_2 Lib usage
// INPUT          : None
// OUTPUT         : DWORD time-ms
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  :
// POST_CONDITION :
//******************************************************************************
DWORD far DP_Ext_GetSystemTime (void)
{
   return gm_GetSystemTime();
}


//******************************************************************************
// FUNCTION       : void far DP_Ext_Delay1Us(DWORD count_u32)
// USAGE          : For DP1_2 Lib usage
// INPUT          : DWORD time-us
// OUTPUT         : None
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  :
// POST_CONDITION :
//******************************************************************************
void far DP_Ext_Delay1Us(DWORD count_u32)
{
   gm_Delay1us((WORD)count_u32);
}



#if (AUXLLI2C_TYPE==AUXLLI2C_FROM_LIB)
//******************************************************************************
// FUNCTION       : BOOL DpRxAux2I2CHandler(WORD W_Addr, BOOL B_IsRead, BYTE *B_Chunk,
//                :                         WORD W_Len, BYTE B_Stop, AUX_CH B_AuxCh
// USAGE          : Called from DP1_2LIB AuxCh_AuxLLI2CRequest with AUX_CH assigned
// INPUT          : W_Addr, B_IsRead, *B_Chunk, W_Len, B_Stop, B_AuxCh
// OUTPUT         : TRUE/FALSE
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  :
// POST_CONDITION :
//******************************************************************************
static BOOL DpRxAux2I2CHandler(WORD W_Addr, BOOL B_IsRead, BYTE *B_Chunk, WORD W_Len, BYTE B_Stop, AUX_CH B_AuxCh)
{
   BOOL B_IsSW;

      B_IsSW = gm_AuxRxI2C_SW_IsSWTransaction(W_Addr, B_AuxCh);

   if (B_IsSW)
   {
      gm_AuxRxI2C_SW_Handler(W_Addr,!B_IsRead,B_Chunk,W_Len,B_Stop,B_AuxCh);
   }
   else
   {
      return(DpRxEdidHandler(W_Addr, B_IsRead, B_Chunk, W_Len, B_Stop, B_AuxCh));
   }
   return TRUE;
}


//******************************************************************************
// FUNCTION       : BOOL Rx0_AUXI2CRequest(WORD W_Addr, BOOL B_IsRead,
//                :                         BYTE *B_Chunk, WORD W_Len, BYTE B_Stop)
// USAGE          : Called from DP1_2LIB AuxCh_AuxLLI2CRequest-RX1
// INPUT          : W_Addr, B_IsRead, *B_Chunk, W_Len, B_Stop
// OUTPUT         : TRUE/FALSE
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  :
// POST_CONDITION :
//******************************************************************************
//typedef BOOL DP_EXT_FUNC (* DPRX_DpRxExtFuncAUXI2CHandler_t)(U16 Address_16, BOOL Dir_b, U8 *Buffer_p8, U16 Length_16, U8 Stop_8);

BOOL far Rx0_AUXI2CRequest(U16 W_Addr, BOOL B_IsRead, U8 *B_Chunk, U16 W_Len, U8 B_Stop)
{
   return DpRxAux2I2CHandler(W_Addr, B_IsRead, B_Chunk, W_Len, B_Stop, AUX_CH_C0);
}


//******************************************************************************
// FUNCTION       : BOOL Rx1_AUXI2CRequest(WORD W_Addr, BOOL B_IsRead,
//                :                         BYTE *B_Chunk, WORD W_Len, BYTE B_Stop)
// USAGE          : Called from DP1_2LIB AuxCh_AuxLLI2CRequest-RX2
// INPUT          : W_Addr, B_IsRead, *B_Chunk, W_Len, B_Stop
// OUTPUT         : TRUE/FALSE
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  :
// POST_CONDITION :
//******************************************************************************
BOOL Rx1_AUXI2CRequest(U16 W_Addr, BOOL B_IsRead, U8 *B_Chunk, U16 W_Len, U8 B_Stop)
{
   return DpRxAux2I2CHandler(W_Addr, B_IsRead, B_Chunk, W_Len, B_Stop, AUX_CH_C2);
}



#endif //(AUXLLI2C_TYPE==AUXLLI2C_FROM_LIB)


//******************************************************************************
// FUNCTION       : BYTE Rx_GetSrcPowerStatus(void)
// USAGE          : Called from DP1_2LIB DpRxExtFuncGetCableStatus_pf
// INPUT          : None
// OUTPUT         : TRUE/FALSE
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  :
// POST_CONDITION :
//******************************************************************************
BYTE Rx_GetSrcPowerStatus(BYTE ApplicationPort_b)
{
   gmt_SRCPWR_STATE b_sts;

   if (ApplicationPort_b == SSTLegacyRxPort)
   {
      #if (ENABLE_HPD_OVERRIDED!=0) && (DP_NUM_SST_ONLY_RX != 0)
      if(Rx1_ForceHpdLow_Flag==TRUE)
         return FALSE;
      #endif
      b_sts =  GetComboPhySrcPwrState(AID_PHY2);

      #if (SRCPWR_OFF_DEBOUNCE_CHECK!=0)
      if(b_sts==SRCPWR_OFF)
      {
         if(gm_TimerCheck(DP2_SRCPWR_OFF_DEBOUNCE_10ms_TMR)==TIMER_OK) //Running
            b_sts=SRCPWR_ON;
      }
      #endif
   }
   else if (ApplicationPort_b == MstRxPort)
   {
      #if (ENABLE_HPD_OVERRIDED!=0) && (DP_RL_NUM_UPSTRM_PORTS != 0)
      if(Rx0_ForceHpdLow_Flag==TRUE)
         return FALSE;
      #endif
      b_sts =  GetComboPhySrcPwrState(AID_PHY0);

      #if (SRCPWR_OFF_DEBOUNCE_CHECK!=0)
      if(b_sts==SRCPWR_OFF)
      {
         if(gm_TimerCheck(DP1_SRCPWR_OFF_DEBOUNCE_10ms_TMR)==TIMER_OK) //Running
            b_sts=SRCPWR_ON;
      }
      #endif
   }
   else
      b_sts = SRCPWR_ON;

   if ((b_sts==SRCPWR_ON) ||(b_sts==SRCPWR_DETECTING))
      return TRUE;
   else
      return FALSE;
}
//******************************************************************************
// FUNCTION       : BYTE Rx_GetCableStatus(void)
// USAGE          : Called from DP1_2LIB DpRxExtFuncGetCableStatus_pf
// INPUT          : None
// OUTPUT         : TRUE/FALSE
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  :
// POST_CONDITION :
//******************************************************************************
#if (UNRELIABLE_DP_CAB_STS_WKAROUND!=0)  
#define GetComboPhyCableState GetDpRxComboPhyCableState
#endif

//extern BOOL SendLongPulse_b;
BYTE Rx_GetCableStatus(BYTE ApplicationPort_b)
{
   gmt_CABLE_STATE b_sts;
/*	if(SendLongPulse_b == DP_TRUE)
	{
		return CAB_DISCONNECTED;
	}*/
   
   if (ApplicationPort_b == SSTLegacyRxPort)
   {
   #if (ENABLE_HPD_OVERRIDED!=0) && (DP_NUM_SST_ONLY_RX != 0)
      if(Rx1_ForceHpdLow_Flag==TRUE)
         b_sts = CAB_UNPLUGGING;
      else
   #endif   
      b_sts =  GetComboPhyCableState(AID_PHY2);
   }
   else if (ApplicationPort_b == MstRxPort)
   {
   #if (ENABLE_HPD_OVERRIDED!=0) && (DP_RL_NUM_UPSTRM_PORTS != 0)
      if(Rx0_ForceHpdLow_Flag==TRUE)
         b_sts = CAB_DISCONNECTED;
      else
   #endif   
         b_sts =  GetComboPhyCableState(AID_PHY0);
   #if (DISPLAY_ID_SUPPORT !=0)
      if(b_sts==CAB_DISCONNECTED)
         ResetEdidSegment();
   #endif         
   }
   else
      b_sts = CAB_CONNECTED;

   if (b_sts==CAB_DISCONNECTED)
      return FALSE;
   else
      return TRUE;
}


//******************************************************************************
// FUNCTION       : void Rx0_AudioDetectedNotify(void)
// USAGE          : Called from DP1_2LIB DpRxExtFuncAudioDectedNotify_pf
// INPUT          : None
// OUTPUT         : None
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  :
// POST_CONDITION :
//******************************************************************************
void Rx0_AudioDetectedNotify(void)
{
   gm_Print("Rx0_AudioDetectedNotify!",0);
   //#pragma message "ToDo: Implement Rx1_AudioDetectedNotify"
}


//******************************************************************************
// FUNCTION       : void Rx1_AudioDetectedNotify(void)
// USAGE          : Called from DP1_2LIB DpRxExtFuncAudioDectedNotify_pf
// INPUT          : None
// OUTPUT         : None
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  :
// POST_CONDITION :
//******************************************************************************
void Rx1_AudioDetectedNotify(void)
{
   gm_Print("Rx1_AudioDetectedNotify!",0);
//	#pragma message "ToDo: Implement Rx2_AudioDetectedNotify"
}

#if (DP_RL_NUM_DOWNSTREAM_LOGICAL_PORTS != 0) || (DP_NUM_SST_ONLY_RX != 0)
void DPRX_ConfigChange(U8 InputConnector, U16 ConfigParam_16)
{

   #ifndef DELL_U2410F		//Neil120614- disable it, don't need send MS_RESTART when init;
   //ToDo: The following code is not verified yet!
   msg_ii("DPRX_ConfigChange(%d)=0x%x",InputConnector,ConfigParam_16);
   if (ConfigParam_16 & DPEventVideoColorSpace)
   {
      if (gB_CurrentInputConnector[gmvb_MainChannel] == InputConnector)
         ModeSetupRequest(gmvb_MainChannel, REQ_MS_RESTART, PARAM_NONE, 0);
      else if (gB_CurrentInputConnector[gmvb_PipChannel] == InputConnector)
         ModeSetupRequest(gmvb_PipChannel, REQ_MS_RESTART, PARAM_NONE, 0);
   }
	#endif

   if (gB_CurrentInputConnector[gmvb_MainChannel] == InputConnector)
   {
      if (ConfigParam_16 & DPEventAudioMute)
      {
		 BYTE AudioStatus_8 = DPRX_GetAudioMuteState(DpRxGetPortId(InputConnector));
         if (AudioStatus_8 != FALSE)
         {
            DPRX_MuteAudio(DpRxGetPortId(InputConnector), TRUE);

         #if (FEATURE_AUDIO == ENABLE)
            SYS_API_AUDIO_DPRx_Set_AVMUTE();
         #endif
         }
         else
         {
            DPRX_MuteAudio(DpRxGetPortId(InputConnector), FALSE);

         #if (FEATURE_AUDIO == ENABLE)
            SYS_API_AUDIO_DPRx_Clear_AVMUTE();
         #endif
         }
      }
      if (ConfigParam_16 & DPEventAudioConfig)
      {
      #if (FEATURE_AUDIO == ENABLE)
         //gm_AUDIO_Set_Event_DPRx(gmd_AUDIO_PROPERTY_CHANGE); //Marked for pass Audio CTS        
      #endif

      }
   }
}
#endif

#if DP_RL_NUM_DOWNSTREAM_LOGICAL_PORTS != 0
void far Rx0_ConfigChange(U16 ConfigParam_16)
{
   DPRX_ConfigChange( GetDpInputConnector(DID_DPRX0),ConfigParam_16);
}
#endif

#if DP_NUM_SST_ONLY_RX != 0
void far Rx1_ConfigChange(U16 ConfigParam_16)
{
   DPRX_ConfigChange( GetDpInputConnector(DID_DPRX1),ConfigParam_16);
}
#endif



#ifdef CFG_DPTX_HDCP_EN
gmt_SHA_STRUCT gS_DpTxSha;

void EXT_FUNC DpTxExt_Sha1Reset(void)
{
   gm_SHA1Reset((gmt_SHA1_CONTEXT *) &gS_DpTxSha);
   msg("SHA: Reset", 0);
}

void EXT_FUNC DpTxExt_Sha1Input(const U8 far * Bp_Message, U16 W_Length)
{

   gm_SHA1Input((gmt_SHA1_CONTEXT *) &gS_DpTxSha, (BYTE far *) Bp_Message, W_Length);
   msg("SHA: Processed", 0);
}

U8 DP_RAM * EXT_FUNC DpTxExt_Sha1Result(void)
{
   gm_SHA1Result((gmt_SHA1_CONTEXT *) &gS_DpTxSha, (BYTE *) gS_DpTxSha.Message_Digest);
   msg("SHA: Result is obtained", 0);

   return ((U8 far*) gS_DpTxSha.Message_Digest);
}
#endif

#if 0
void far DpTxExt_LinkAutomationPatGen(U8 PortId_8,DP_MsaParameters_t DP_RAM * LinkAutomationDpcdMsaParams_ps)
{
// TODO: Should be moved to register.h
#define DPTX_PATGEN_P0_BPP_SHIFT 			4
#define DPTX_PATGEN_P0_HTOTAL_SHIFT 		0
#define DPTX_PATGEN_P0_HSTART_SHIFT 		16
#define DPTX_PATGEN_P0_VTOTAL_SHIFT 		0
#define DPTX_PATGEN_P0_VSTART_SHIFT 		16
#define DPTX_PATGEN_P0_HWIDTH_SHIFT 		0
#define DPTX_PATGEN_P0_VHEIGHT_SHIFT 		16
#define DPTX_PATGEN_P0_HSYNC_WIDTH_SHIFT 	0
#define DPTX_PATGEN_P0_HPOLARITY_SHIFT 		15
#define DPTX_PATGEN_P0_VSYNC_WIDTH_SHIFT 	16
#define DPTX_PATGEN_P0_VPOLARITY_SHIFT		31

   void DP_EXT_FUNC Util_RlTxPatGenSelect(U8 PatGenId_8);

#define PTG_IN_USE_ID 1	// Athena only has one PTG in PORT 1
#define PTG_DEFAULT_PATTERN DP_TPG_PTRN_CHECKER_BOARD
#define PTG_REG_BLK_SIZE (DP12TX_PATGEN_P1_CTRL - DP12TX_PATGEN_P0_CTRL)
#define gm_DpTxPtgRegAddr(RegName) (DP12TX_PATGEN_P0_ ##RegName + PTG_IN_USE_ID*PTG_REG_BLK_SIZE)
#define gm_DpTxPtgFieldAddr(RegName, FieldName) (gm_DpTxPtgRegAddr(RegName) + DPTX_PATGEN_P0_ ##FieldName ##_SHIFT / 8)

   WORD W_PtgCtrl;

   Util_RlTxPatGenSelect(PTG_IN_USE_ID);

   gm_WriteRegWord(gm_DpTxPtgFieldAddr(HPARAM, HTOTAL), LinkAutomationDpcdMsaParams_ps->HTotal_16);
   gm_WriteRegWord(gm_DpTxPtgFieldAddr(HPARAM, HSTART), LinkAutomationDpcdMsaParams_ps->HActiveStart_16);

   gm_WriteRegWord(gm_DpTxPtgFieldAddr(VPARAM, VTOTAL), LinkAutomationDpcdMsaParams_ps->VTotal_16);
   gm_WriteRegWord(gm_DpTxPtgFieldAddr(VPARAM, VSTART), LinkAutomationDpcdMsaParams_ps->VActiveStart_16);

   gm_WriteRegWord(gm_DpTxPtgFieldAddr(ACTIVE, HWIDTH), LinkAutomationDpcdMsaParams_ps->HActiveWidth_16);
   gm_WriteRegWord(gm_DpTxPtgFieldAddr(ACTIVE, VHEIGHT), LinkAutomationDpcdMsaParams_ps->VActiveWidth_16);

   gm_WriteRegWord(gm_DpTxPtgFieldAddr(SYNC, HSYNC_WIDTH), LinkAutomationDpcdMsaParams_ps->HSyncWidth_16.Value_bf15 | (LinkAutomationDpcdMsaParams_ps->HSyncWidth_16.Polarity_bf1 << (DPTX_PATGEN_P0_HPOLARITY_SHIFT - DPTX_PATGEN_P0_HSYNC_WIDTH_SHIFT)));
   gm_WriteRegWord(gm_DpTxPtgFieldAddr(SYNC, VSYNC_WIDTH), LinkAutomationDpcdMsaParams_ps->VSyncWidth_16.Value_bf15 | (LinkAutomationDpcdMsaParams_ps->VSyncWidth_16.Polarity_bf1 << (DPTX_PATGEN_P0_VPOLARITY_SHIFT - DPTX_PATGEN_P0_VSYNC_WIDTH_SHIFT)));

   W_PtgCtrl = DPTX_PATGEN_P0_EN | gm_RegBitShiftValue(DPTX_PATGEN_P0_PATTERNS, PTG_DEFAULT_PATTERN);

   switch (LinkAutomationDpcdMsaParams_ps->MsaFlags_16.Msa_bfs.DpColorDepth_bf3)
   {
      case DP_MsaColorDepth_8Bits:
         W_PtgCtrl |= gm_RegBitShiftValue(DPTX_PATGEN_P0_BPP, 0); // 8 bpp
         break;

      case DP_MsaColorDepth_10Bits:
         W_PtgCtrl |= gm_RegBitShiftValue(DPTX_PATGEN_P0_BPP, 1); // 10 bpp
         break;


      default:
         W_PtgCtrl |= gm_RegBitShiftValue(DPTX_PATGEN_P0_BPP, 0); // 8 bpp

   }

   if (LinkAutomationDpcdMsaParams_ps->MsaFlags_16.Msa_bfs.DpMsaColorMode_bf1 == DP_MsaColorMode_YOnly)
   {
      W_PtgCtrl |= gm_RegBitShiftValue(DPTX_PATGEN_P0_TYPE, DP_TPG_CS_Y_ONLY);
   }
   else // DpMsaColorMode_bf1 == DP_MsaColorMode_All
   {
      switch (LinkAutomationDpcdMsaParams_ps->MsaFlags_16.Msa_bfs.DpColorFormat_bf4)
      {
         case DP_MsaColorFormat_RGB:
            W_PtgCtrl |= gm_RegBitShiftValue(DPTX_PATGEN_P0_TYPE, DP_TPG_CS_RGB);
            break;

         case DP_MsaColorFormat_YCbCr444Bt601:
         case DP_MsaColorFormat_YCbCr444Bt709:
            W_PtgCtrl |= gm_RegBitShiftValue(DPTX_PATGEN_P0_TYPE, DP_TPG_CS_YCBCR_444);
            break;

         case DP_MsaColorFormat_YCbCr422Bt601:
         case DP_MsaColorFormat_YCbCr422Bt709:
            W_PtgCtrl |= gm_RegBitShiftValue(DPTX_PATGEN_P0_TYPE, DP_TPG_CS_YCBCR_422);
            break;

         default:
            W_PtgCtrl |= gm_RegBitShiftValue(DPTX_PATGEN_P0_TYPE, DP_TPG_CS_RGB);
      }
   }

   gm_WriteRegWord(gm_DpTxPtgRegAddr(CTRL), W_PtgCtrl);

   UNUSED_VAR(PortId_8);
}

#endif


//******************************************************************************
// FUNCTION			: void DpTxFe_SetInputSrc(DPTX_FE_INPUT_SOURCE_TYPE src)
// USAGE			: Set Mux to select input source of DPTX front-end
// INPUT				: src - input source type
// OUTPUT			: None
// GLOBALS			: None
// USED_REGS      	: HOST_HDMI_DP_SETUP
// PRE_CONDITION 	: None
// POST_CONDITION 	: None
//******************************************************************************
// Doxygen Format
/**
 *	@fn void DpTxFe_SetInputSrc(DPTX_FE_INPUT_SOURCE_TYPE src)
 *	@brief Set Mux to select input source of DPTX front-end
 *	@param[in] src FE_SRC_DPTXBE = DPRX back-end, FE_SRC_DP12BRIDGE = DP1.2 bridge
 *	@return void
 */
 /*
void DpTxFe_SetInputSrc(DPTX_FE_INPUT_SOURCE_TYPE src)
{
   gm_ClearAndSetBitsByte(HOST_DVI_DP_SETUP, DPTX_PHY_SRC_SELECT, gm_RegBitShiftValue(DPTX_PHY_SRC_SELECT, src));
}
*/
/*
extern U8 DP_EXT_FUNC LT_Frequency_Lock_Mode(void);
extern void DP_EXT_FUNC LT_Set_PDEN(U8 NumLanes_8, BOOL B_Set);
extern void DP_EXT_FUNC HPD_HpdSetLow(void);
extern void DP_EXT_FUNC HPD_HpdSetHigh(void);
extern void DP_EXT_FUNC LT_Reset_CPT(U8 NumLanes_8, BOOL B_Reset);

BOOL SendLongPulse_b = DP_FALSE; 

void SendLongPulse (void)
{
	HPD_AppHpdSetLow();
	HPD_HpdSetLow();
	gm_Print("Hpd Low",0);
	SendLongPulse_b = DP_TRUE;
	DpAppInit();


	gm_ClearRegBitsDWord(DP12RX0_CLK_CTRL, DPRX_LSCLK_135_SRC);
	LT_Set_PDEN(4, DP_FALSE);
	LT_Reset_CPT(4, DP_TRUE);
	
	DP_Ext_Delay1Us(100000);

	HPD_AppHpdSetHigh();
	HPD_HpdSetHigh();

	SendLongPulse_b = DP_FALSE;
	gm_Print("Hpd High",0);
}
*/
//******************************************************************************
// FUNCTION       : void DpAppInit(void)
// USAGE          : Called from SystemInit for DP Initilization
// INPUT          : None
// OUTPUT         : None
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  :
// POST_CONDITION :
//******************************************************************************
void DpAppInit(void)
{

#if DP_TOTAL_NUM_PORTS
   BYTE						ApplicationPortId= 0;
   DP_PortInitParams_t			DP_PortInitParams_s[DP_TOTAL_NUM_PORTS];

   DP_GetLogicalPortBksv(NULL_PTR);  // 1st call will copy 2ndBKSV from OTP to OTP SRAM. 

#ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()!=CHIPID_4K2K_FE)
#endif
   {
   #if ENABLE_HPD_OVERRIDED
      #if DP_RL_NUM_UPSTRM_PORTS != 0
         #if (DPRX0_DYNAMIC_INIT_BY_NVRAM!=0)
         ForceDpHpd2Low(DID_DPRX0,TRUE);
         SetForceHpdLowFlag(DID_DPRX0, TRUE);
         #endif
      gm_TimerStart(RX0_HPD_OVERRIDED_10ms_TMR,HPD_OVERRIDED_TIMEx10ms);
      #endif

      #if DP_NUM_SST_ONLY_RX != 0
         #if (DPRX0_DYNAMIC_INIT_BY_NVRAM!=0)
         ForceDpHpd2Low(DID_DPRX1,TRUE);
         SetForceHpdLowFlag(DID_DPRX1, TRUE);
         #endif
      gm_TimerStart(RX1_HPD_OVERRIDED_10ms_TMR,HPD_OVERRIDED_TIMEx10ms);
      #endif
   #endif
   }


#if PATCH_OF_UNIQUE_EDID   
   PatchOfUniqueEDID();
#endif

   DpInpControlInit();
	
     
#if DP_NUM_SST_ONLY_RX != 0
   #ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()!=CHIPID_4K2K_FE)
   #endif
   {

   //SetDpRxHPDLevel(DID_DPRX1, FALSE);                          //Set HPD to low during Applicaiton init
   DP_PortInitParams_s[ApplicationPortId].PortType_e          = DP_PortType_SstOnlyRx;
   DP_PortInitParams_s[ApplicationPortId].ApplicationPortId_8 = SSTLegacyRxPort;
   DP_PortInitParams_s[ApplicationPortId].RoutingLogicPortNum_e   = DP_RoutingLogicPortNum_0;
   DP_PortInitParams_s[ApplicationPortId].PortInitParams_ps   = &DPRX_SstOnlyInitParams_s;

   ApplicationPortId++;

   msg("DP_PortType_SstOnlyRx Init",0);

   #if Patch_Of_DPRX_AUX2I2C_REQ_IRQ_EN
   gm_SetRegBitsWord(DP12RX1_AUX_IRQ_CTRL,DPRX_AUX2I2C_REQ_IRQ_EN);
   #endif
   }
#endif

#if DP_NUM_SST_ONLY_TX != 0
#ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()==CHIPID_4K2K_FE)
#endif
   {

   DP_PortInitParams_s[ApplicationPortId].PortType_e          = DP_PortType_SstOnlyTx;
   DP_PortInitParams_s[ApplicationPortId].ApplicationPortId_8 = iDpTxPort;
   DP_PortInitParams_s[ApplicationPortId].RoutingLogicPortNum_e   = DP_RoutingLogicPortNum_0;
   DP_PortInitParams_s[ApplicationPortId].PortInitParams_ps   = &DPTX_SstOnlyInitParams_s;

   ApplicationPortId++;
   gm_ClearRegBitsByte(HOST_DVI_DP_SETUP,DPTX_PHY_SRC_SELECT);

   msg("DP_PortType_SstOnlyTx Init",0);
   }
#endif

   gm_WriteRegWord(DP_MIXER_IRQ_SELECT, 0);  // Disable Interrupt

#if DP_RL_NUM_UPSTRM_PORTS != 0
   #ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()!=CHIPID_4K2K_FE)
   #endif
   {
      DP_PortInitParams_s[ApplicationPortId].PortType_e          = DP_PortType_MstCapableRx;
      DP_PortInitParams_s[ApplicationPortId].ApplicationPortId_8 = MstRxPort;
      DP_PortInitParams_s[ApplicationPortId].RoutingLogicPortNum_e   = DP_RoutingLogicPortNum_0;
   #if (DPRX0_DYNAMIC_INIT_BY_NVRAM!=0) && (defined UserPrefDPSWitch)
      Patch_Of_DpReInit();
      #ifdef ST_4K2K_93xx_BOARD
         if(GetChipID_4K2K()==CHIPID_4K2K_L)      
         {  //For Athena_L, just fixed in DP1.2
         #if 0 //def DP_HUB_2_LOG_PORTS
            DP_PortInitParams_s[ApplicationPortId].PortInitParams_ps   = &DPRX_MstDpCapableInitParams_s_DP11;
         #else
            DP_PortInitParams_s[ApplicationPortId].PortInitParams_ps   = &DPRX_MstCapableInitParams_s;
         #endif
         }
         else
      #endif
         {
            if (UserPrefDPSWitch == DP_Input_11)
               DP_PortInitParams_s[ApplicationPortId].PortInitParams_ps   = &DPRX_MstDpCapableInitParams_s_DP11;
            else
               DP_PortInitParams_s[ApplicationPortId].PortInitParams_ps   = &DPRX_MstCapableInitParams_s;
         }
   	#ifdef ST_4K2K_93xx_BOARD
      DpRxLoadEdid2Buf();
   	#endif
   #else
      DP_PortInitParams_s[ApplicationPortId].PortInitParams_ps   = &DPRX_MstCapableInitParams_s;
   #endif

      ApplicationPortId++;
      msg("DP_PortType_MstCapableRx Init",0);
   #if Patch_Of_DPRX_AUX2I2C_REQ_IRQ_EN
      gm_SetRegBitsWord(DP12RX0_AUX_IRQ_CTRL,DPRX_AUX2I2C_REQ_IRQ_EN);
   #endif

   #if Patch_Of_VCLK_CORR_EN_In_DpReInit
      gm_ClearRegBitsWord(DP12RX0_MS_CTRL,DPRX_DTG_VCLK_CORR_EN);
   #endif
   } 
#endif


#if DP_RL_NUM_DOWNSTREAM_LOGICAL_PORTS!= 0
   #ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()!=CHIPID_4K2K_FE)
   #endif
   {
      DP_PortInitParams_s[ApplicationPortId].PortType_e          	= DP_PortType_DigitalTx;
      DP_PortInitParams_s[ApplicationPortId].ApplicationPortId_8	= LogicalTxPort;
      DP_PortInitParams_s[ApplicationPortId].RoutingLogicPortNum_e	= DP_RoutingLogicPortNum_0;
      DP_PortInitParams_s[ApplicationPortId].PortInitParams_ps   	= &DPTX_LogicalInitParams_s;

      ApplicationPortId++;
      msg("DP_PortType_LogicalTx Init",0);
   }
#endif

#if (DP_RL_NUM_DOWNSTREAM_PHYSICAL_PORTS != 0)
   #ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()!=CHIPID_4K2K_FE)
   #endif
   {
      DP_PortInitParams_s[ApplicationPortId].PortType_e          = DP_PortType_MstCapableTx;
      DP_PortInitParams_s[ApplicationPortId].ApplicationPortId_8 = MstTxPort;
      DP_PortInitParams_s[ApplicationPortId].RoutingLogicPortNum_e   = DP_RoutingLogicPortNum_1;
      DP_PortInitParams_s[ApplicationPortId].PortInitParams_ps   = &DPTX_MstCapableInitParams_s;

      ApplicationPortId++;
      gm_ClearAndSetBitsByte(HOST_DVI_DP_SETUP, DPTX_PHY_SRC_SELECT, BIT3);
      msg("DP_PortType_MstCapableTx Init",0);
   }
#endif

   if((gm_ReadRegWord(HOST_DVI_DP_SETUP) & HDMI_PORT_SEL) == 0)      //Don't update HDMI_PORT_SEL when DpAppInit is called for DP1.1/1.2 changed.
      gm_SetRegBitsWord(HOST_DVI_DP_SETUP,0x1);                      //Otherwise, it may cause Hdmi no video. 

   gm_SetRegBitsByte(DP12TX_KEYHOST_CTRL, DP12TX_KEYHOST_EN);
   gm_SetRegBitsByte(DP12RX_KEYHOST_CTRL, DP12RX_KEYHOST_EN);

	if((gm_ReadRegWord(PRODUCT_REV) & PROD_REV) == REV_CODE_2_0)
	{
		gm_Print("**Athena Product Version 2.%d**", 0);
		DP_ConfigLib(DP_IpVersion_1);	  
	}
	else if((gm_ReadRegWord(PRODUCT_REV) & PROD_REV) == REV_CODE_2_1)
	{
		gm_Print("**Athena Product Version 2.%d**", 1);
		DP_ConfigLib(DP_IpVersion_2);	   
	}

   DP_Init((DP_PortInitParams_t *)&DP_PortInitParams_s);

#ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()!=CHIPID_4K2K_FE)
#endif
   {
      #if DP_RL_NUM_UPSTRM_PORTS != 0
      #if Patch_Of_VCLK_CORR_EN_In_DpReInit
      gm_SetRegBitsWord(DP12RX0_MS_CTRL,DPRX_DTG_VCLK_CORR_EN);
      #endif
      #endif


      #if (PATCH_OF_AUX_GAIN_FOR_INTEL_SERIES4_DP != 0)
      #if DP_RL_NUM_UPSTRM_PORTS != 0
      gm_ClearAndSetBitsWord(CPHY0_AUX_PHY_CTRL+2, CPHY0_AUX_G>>16, 0x60);
      #endif

      #if DP_NUM_SST_ONLY_RX != 0
      gm_ClearAndSetBitsWord(CPHY2_AUX_PHY_CTRL+2, CPHY0_AUX_G>>16, 0x60);
      #endif 
      #endif


      #if (DP_NUM_SST_ONLY_RX != 0) && (PATCH_OF_RX1_DPRX_ENHANCED_BS==1)
      gm_SetRegBitsByte(DP12RX1_BE_CONTROL,DPRX_ENHANCED_BS);
      #endif
   }

#if (DP_NUM_SST_ONLY_TX == 0) && (DP_RL_NUM_DOWNSTREAM_PHYSICAL_PORTS==0)
   //Fixed power on MstRx LT is done but no video issue.
   gm_ClearRegBitsWord(DPTX_PHY_CTRL,DPTX_TXL0_RESET|DPTX_TXL1_RESET|DPTX_TXL2_RESET|DPTX_TXL3_RESET);
#endif


   msg("DP App init Done", 0);

   //gm_Delay10ms(10);
#ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()!=CHIPID_4K2K_FE)
#endif
   {   
      gm_AuxRxI2C_SW_TransInit((BYTE *)(&(SW_Transaction_ModuleData[AUX_CH_C0])),AUX_CH_C0);
      gm_AuxRxI2C_SW_AddSWDevId(DDC2BI_ADDR_ID, AUX_CH_C0);
      gm_AuxRxI2C_SW_TransInit((BYTE *)(&(SW_Transaction_ModuleData[AUX_CH_C2])),AUX_CH_C2);
      gm_AuxRxI2C_SW_AddSWDevId(DDC2BI_ADDR_ID, AUX_CH_C2);
   }

#if (AUXLLI2C_TYPE==AUXLLI2C_FROM_IROM)
   //Add App-layer Extra Init here
   gm_DpRxAuxLLI2CInit((BYTE *)(&AuxRxI2CModuleData),
                       gm_AuxRxI2C_SWCallback,	//Hook IROM AuxRxI2C Handler to send/receive 0x6E packets
                       DpRxEdidHandler);		//Hook EDID Handler
#endif

#ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()!=CHIPID_4K2K_FE)
#endif
   {   
      DpRxEdidHandlerInit();

      #ifdef INACTIVE_DEVICE_POWER_SAVING   
      DpPwrCheckInit();
      #endif
   }

   //Init IRQ here
   //Note:Name mismatch on Athena register spec:
   //	   DPRX_INT_SEL = DP_TOP_IRQ for DPRX0
   //     DPRX_HDCP_INT_SEL = DP_TOP_IRQ for DPRX1
   gm_WriteRegWord(DP_MIXER_IRQ_SELECT, 0x330 | DPTOP_MASK |DPRX_HDCP_MASK);  //interrupts on IRQ3


#if (DP_RL_NUM_DOWNSTREAM_PHYSICAL_PORTS==0) && (DP_NUM_SST_ONLY_TX==0)
   gm_SetRegBitsWord(DPTX_PHY_CTRL,DPTX_TXL0_PD|DPTX_TXL1_PD|DPTX_TXL2_PD|DPTX_TXL3_PD);
   gm_ClearRegBitsWord(DPTX_CLK_CTRL, DPTX_LSCLK_135_SRC);
#endif
#endif //DP_TOTAL_NUM_PORTS
}

#if 1	//Hyb130508* Enable for LG 27" DP Panel//(SELECT_DPTX_TYPE==USE_DPTX_DYNAMIC_SWITCH) || (SELECT_DPTX_TYPE==USE_DPTX_MANUAL_SWITCH)
extern void DP_EXT_FUNC DP12_SetTxActivePort(U8 PortIdToDeactivate_8, U8 PortIdToActivate_8);

void Reset_Pre_PortIdToActivate(void)
{
   Pre_PortIdToActivate_8 = 0xFF;
}

void DPTxPortSwitch (U8 PortIdToDeactivate_8, U8 PortIdToActivate_8)
{
   if(Pre_PortIdToActivate_8 == PortIdToActivate_8)
      return ;

   Pre_PortIdToActivate_8 = PortIdToActivate_8;

	if(PortIdToActivate_8 == MstTxPort)
	{
	   gm_ClearAndSetBitsByte(HOST_DVI_DP_SETUP, DPTX_PHY_SRC_SELECT, BIT3);
	}
	else if(PortIdToActivate_8 == iDpTxPort)
	{
	   gm_ClearRegBitsByte(HOST_DVI_DP_SETUP,DPTX_PHY_SRC_SELECT);
	}
    DP12_SetTxActivePort(PortIdToDeactivate_8,PortIdToActivate_8);
}

#if 1 //(DPTX_PING_SINK_DEVICE_WHEN_VIDEO_ON!=0)
BYTE GetCurrentDPTXPort(void)
{
   return Pre_PortIdToActivate_8;
}
#endif 
#endif //(SELECT_DPTX_TYPE==USE_DPTX_DYNAMIC_SWITCH) || (SELECT_DPTX_TYPE==USE_DPTX_MANUAL_SWITCH)

#if ENABLE_HPD_OVERRIDED
void SetForceHpdLowFlag(gmt_DPRX_DID b_DID, BYTE SetLow)
{
#if (DP_TOTAL_NUM_UPSTRM_PORTS !=0)
   if(b_DID == DID_DPRX1)
   {
      #if DP_NUM_SST_ONLY_RX != 0
      Rx1_ForceHpdLow_Flag = SetLow;
      #endif
   }
   else if(b_DID == DID_DPRX0)
   {
      #if DP_RL_NUM_UPSTRM_PORTS != 0
      Rx0_ForceHpdLow_Flag = SetLow;
      #endif
   }
#else
   UNUSED_PARAMETER(b_DID);
   UNUSED_PARAMETER(SetLow);
#endif
}

void DpHpdHandler(void)
{
   #if DP_NUM_SST_ONLY_RX != 0
      if (Rx1_ForceHpdLow_Flag==TRUE)
      {
         if (gm_TimerCheck(RX1_HPD_OVERRIDED_10ms_TMR)== TIMER_TMO)
         {
            ForceDpHpd2Low(DID_DPRX1, FALSE);
            DP_SetHpdHigh(SSTLegacyRxPort);
            Rx1_ForceHpdLow_Flag=FALSE;
         }
      }
   #endif
   
   #if DP_RL_NUM_UPSTRM_PORTS != 0
      if (Rx0_ForceHpdLow_Flag==TRUE)
      {
         if (gm_TimerCheck(RX0_HPD_OVERRIDED_10ms_TMR)== TIMER_TMO)
         {
            ForceDpHpd2Low(DID_DPRX0, FALSE);
            DP_SetHpdHigh(MstRxPort);
            Rx0_ForceHpdLow_Flag=FALSE;
         }
      }
   #endif
}
#endif




void DpRxPortChange(BYTE InputConnector)
{
   BYTE b_MuxSwitched=FALSE;
   gmt_COMBOPHY_AID b_AID=gm_GetDpAnaPhyID(InputConnector);

   //gm_Print("***DpRxPortChange(%d)",InputConnector);
   if((IsInputConnectorReady(InputConnector)==FALSE) && 
     (((gmt_DpProperties far *)(GetPortPropertyTable(InputConnector)))->B_Mux_Flag == TRUE))
   {
      UpdateDpInpControl(InputConnector);
      ResetDPCD600(InputConnector);
      b_MuxSwitched=TRUE;
      if (((gmt_DpProperties far*)(GetPortPropertyTable(InputConnector)))->ExtPortSetup != NULL_PTR)
      {  //Before Switch Mux, need to pull low HPD first
         if (b_AID==AID_PHY0)
         {
            #if (ENABLE_HPD_OVERRIDED!=0) && (DP_RL_NUM_UPSTRM_PORTS != 0)
            if (Rx0_ForceHpdLow_Flag==FALSE)
            {
               
               ForceDpHpd2Low(DID_DPRX0,1);
               Rx0_ForceHpdLow_Flag=TRUE;
               gm_TimerStart(RX0_HPD_OVERRIDED_10ms_TMR,HPD_OVERRIDED_TIMEx10ms);
               //gm_Print("Force HPD(%d)",InputConnector);
            }
            #endif
         }
         else
         {
            #if (ENABLE_HPD_OVERRIDED!=0) && (DP_NUM_SST_ONLY_RX != 0)
                  if (Rx1_ForceHpdLow_Flag==FALSE)
                  {
                     ForceDpHpd2Low(DID_DPRX1,1);
                     Rx1_ForceHpdLow_Flag=TRUE;
                     gm_TimerStart(RX1_HPD_OVERRIDED_10ms_TMR,HPD_OVERRIDED_TIMEx10ms);
                  }
            #endif
         }
         // Call ExtPortSetup to switch Extern Mux
         ((gmt_DpProperties far*)(GetPortPropertyTable(InputConnector)))->ExtPortSetup();
      }
   }
   
   if (b_AID==AID_PHY0)
   {
//      DP_ResetRxLink(MstRxPort);  //Not necessary for cut2

      SetDdsFreq(gm_ReadRegWord(CDDS2_FREQ));
      
      if((DpRxIsLT_Success(InputConnector)==FALSE) || (b_MuxSwitched==TRUE))
      {
         #if (ENABLE_HPD_OVERRIDED!=0) && (DP_RL_NUM_UPSTRM_PORTS != 0)
         if (Rx0_ForceHpdLow_Flag==FALSE)
         {
            
            ForceDpHpd2Low(DID_DPRX0,1);
            Rx0_ForceHpdLow_Flag=TRUE;
            gm_TimerStart(RX0_HPD_OVERRIDED_10ms_TMR,HPD_OVERRIDED_TIMEx10ms);
            //gm_Print("Force HPD(%d)",InputConnector);
         }
         #endif
      }
      else
      {
         gm_SetRegBitsDWord(DP12TX1_RL_CONFIG,DP12TX1_SOFT_RESET_ALL);
         gm_Delay1ms(1);
         gm_ClearRegBitsDWord(DP12TX1_RL_CONFIG,DP12TX1_SOFT_RESET_ALL);
      }
   }
   else if (b_AID==AID_PHY2) 
   {
//      DP_ResetRxLink(SSTLegacyRxPort);  //Not necessary for cut2
      if((DpRxIsLT_Success(InputConnector)==FALSE) || (b_MuxSwitched==TRUE))
      {
   #if (ENABLE_HPD_OVERRIDED!=0) && (DP_NUM_SST_ONLY_RX != 0)
         if (Rx1_ForceHpdLow_Flag==FALSE)
         {
            ForceDpHpd2Low(DID_DPRX1,1);
            Rx1_ForceHpdLow_Flag=TRUE;
            gm_TimerStart(RX1_HPD_OVERRIDED_10ms_TMR,HPD_OVERRIDED_TIMEx10ms);
         }
   #endif
      }
/*      else
      {
         gm_SetRegBitsDWord(DP12TX1_RL_CONFIG,DP12TX1_SOFT_RESET_ALL);
         gm_Delay1ms(1);
         gm_ClearRegBitsDWord(DP12TX1_RL_CONFIG,DP12TX1_SOFT_RESET_ALL);
      }
*/      
   }
}

//******************************************************************************
// FUNCTION       : void DpAppHandler(void)
// USAGE          : Called from Main Loop to Handler DP
// INPUT          : None
// OUTPUT         : None
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  :
// POST_CONDITION :
//******************************************************************************
void DpAppHandler(void)
{
#if DP_TOTAL_NUM_PORTS
   gmt_DPRX_DID b_DID;
   BYTE InputConnector;
#if (DP_NUM_SST_ONLY_TX != 0) && (EnableDpTxDelayCount==1)
   static BYTE ExtraDpTxEnDelay=ExtraDpTxEnableDelayCount;
#endif

#if (DP_TOTAL_NUM_UPSTRM_PORTS!=0)
   for (b_DID=0;b_DID<DID_DPRX_END;b_DID++)
   {
      InputConnector=GetDpInputConnector(b_DID);

      if (InputConnector < NUM_MASTER_LIST)
      {  //Valid InputConnector Port

         //**************************************************************************
         // Dynamically turn on/off DP PHY based on DP Cable/SrdPwr/DPCD600... state
         //**************************************************************************
         #ifdef INACTIVE_DEVICE_POWER_SAVING
         if (DpRxPwrStateCheck(InputConnector)==TRUE)
            AdjustDpPwrLevel(InputConnector);
         #else
            DpRxPwrStateCheck(InputConnector);
         #endif

#ifdef ST_4K2K_93xx_BOARD
#ifdef DP_HUB_2_LOG_PORTS
#if (ENABLE_DPRX0_RE_MODE_SETUP)
         if(GetChipID_4K2K()==CHIPID_4K2K_R)
            DP_Two_LogicalPortTimingChangeHandler();
#endif         
#endif
#endif

         //**************************************************************************
         // DPRx to  Main Channel Processing
         //**************************************************************************
         if (InputConnector == gB_CurrentInputConnector[gmvb_MainChannel])
         {

            if (DpRxIsStableState(InputConnector))
            {
               
               #if (DEBUG_DP_PRINT_TIMING_DISPLAY_TIME!=0)
               if(b_StablePrnt)
               {
                   gm_Print("CablePlugged -->SigP: T(%d)",(WORD)(gm_GetSystemTime()-GetDebugDisplayTime()));
                   b_StablePrnt=FALSE;
               }
               #endif    

               #if (DEBUG_DISPLAY_TIME_MEASUREMENT == ENABLE)
               if(b_StablePrintEn)
               {
                  dtm_msg("DTM> DP_SIG_Stable_LT(%d)",gm_GetSystemTime());
                  b_StablePrintEn=FALSE;
               }
               #endif
               
               DPRX_EnableAudio(DpRxGetPortId(InputConnector));

               #if (FEATURE_3D==ENABLE)
               if (gm_DpRxGet3DFormat(InputConnector)!=gmd_3D_IN_UNKNOWN)
               {
                  gm_3D_ModeDetermineByFrontEnd();
               }
               #endif
            }
            else
            {
               #if (DEBUG_DISPLAY_TIME_MEASUREMENT == ENABLE)
               b_StablePrintEn=TRUE;
               #endif
            
               //DPRX_DisableAudio(DpRxGetPortId(InputConnector));
               #if (FEATURE_3D==ENABLE)
               gm_3D_SetStatusForFrontEndChanged();
               gm_3D_RecoveryTo2D();
               #endif
            }


            #if (ENABLE_DPCD_SINK_TEST_SLEEP_DELAY != 0)
            DpcdTestSinkMonitor(InputConnector);
            #endif            

         }
         //**************************************************************************
         // DPRx to  Pip Channel Processing
         //**************************************************************************


      }
   }
#endif //#if (DP_TOTAL_NUM_UPSTRM_PORTS!=0)
   //**************************************************************************
   //Call DP Library Handler
   //**************************************************************************
   DP_Handler();

   #if ENABLE_HPD_OVERRIDED
   DpHpdHandler();
   #endif

   DpRxUpdaateVddsInitFreq();  //Now, only for SstOnlyRx useage. 

#if (DP_NUM_SST_ONLY_TX != 0) 

   #ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()==CHIPID_4K2K_FE)
   #endif
   {
      if (DPTX_IsLinkReady(iDpTxPort)==TRUE)
      {
         if (SstOnlyTx_EnVdoFlag==FALSE)
         {
      #if (EnableDpTxDelayCount==1)
            if (ExtraDpTxEnDelay==0)
            {
               SstOnlyTx_EnVdoFlag=TRUE;
               msg("SstOnlyTx Link is ready",0);
          
            //#if (NON_SCALING_OUTPUT ==0)           
			#ifdef OUTPUT_MODE_CHANGE
	   		if(UserPrefOutputMode == OPM_Normal)
               DpTxEnableVideo(DPTX_TIMING_BY_OUTPUT);
            //#else
            else
				DpTxEnableVideo(DPTX_TIMING_BY_INPUT);
			#else
			DpTxEnableVideo(DPTX_TIMING_BY_OUTPUT);
			#endif				
            //#endif
            }
            else
            {
               ExtraDpTxEnDelay--;
               msg("ExD-",0);
            }
      #else
            SstOnlyTx_EnVdoFlag=TRUE;
            msg("SstOnlyTx Link is ready",0);
         //#if (NON_SCALING_OUTPUT ==0)           
	#ifdef OUTPUT_MODE_CHANGE
	 if(UserPrefOutputMode == OPM_Normal)
	{  
          	#ifdef ST_4K2K_93xx_BOARD
               if((gm_TimingIsStable(gmvb_MainChannel) == TRUE) && (gm_ReadRegByte(BYPASS) & PASS_THROUGH))
                  DpTxEnableVideo(DPTX_TIMING_BY_INPUT);
               else
                  DpTxEnableVideo(DPTX_TIMING_BY_OUTPUT);
            #else
               DpTxEnableVideo(DPTX_TIMING_BY_OUTPUT);
            #endif
	 }
	 else
     {
	         //#else
            DpTxEnableVideo(DPTX_TIMING_BY_INPUT);
	         //#endif
 	}
	 #else
	 	{	
          	#ifdef ST_4K2K_93xx_BOARD
				if((gm_TimingIsStable(gmvb_MainChannel) == TRUE) && (gm_ReadRegByte(BYPASS) & PASS_THROUGH))
				   DpTxEnableVideo(DPTX_TIMING_BY_INPUT);
				else
				   DpTxEnableVideo(DPTX_TIMING_BY_OUTPUT);
            #else
				DpTxEnableVideo(DPTX_TIMING_BY_OUTPUT);
            #endif
		  }
	 #endif
      #endif
         }


         #ifdef ST_4K2K_93xx_BOARD
         if(GetChipID_4K2K()==CHIPID_4K2K_FE)
         #endif
         {
            #if  (PATCH_DPTX_AUD_EN_WAIT_FOR_PORT_ACTIVE!=0)
            //gm_Print(" -1",0);
            if(DP_IsTxActive(iDpTxPort)==TRUE)
            {
               if(DptxAudioEnFlag!=DpTxAudioEnFlagNone)
               {
               //gm_Print(" -2 = 0x%x",DptxAudioEnFlag);
               
                  if(DptxAudioEnFlag & DpTxAudioIsWaitingToBeEnabled)
                  {
                     DPTX_SetAudioConfig(iDpTxPort, &DpTxAudioConfig);
                     DPTX_EnableAudio(iDpTxPort);
                     DPTX_MuteAudio(iDpTxPort, FALSE);
                     #if (PATCH_DPTX_NO_AUD_BY_ZERO_OF_SDP_AUD_M_MEAS !=0)
                     gm_TimerStart(DPTX_AUD_MONITOR_10ms_TMR, 100);
                     #endif
                     DptxAudioEnFlag&=(~DpTxAudioIsWaitingToBeEnabled);  
                     msg(" ^1^ DpTxAudioIsEnabled(%Ld)",gm_GetSystemTime());
                     
                  }

                  if(DptxAudioEnFlag & DpTxAudioIsWaitingToBeMuted)
                  {
                     DPTX_MuteAudio(iDpTxPort, TRUE); 
                     #if (PATCH_DPTX_NO_AUD_BY_ZERO_OF_SDP_AUD_M_MEAS !=0)
                     gm_TimerStart(DPTX_AUD_MONITOR_10ms_TMR, 100);
                     #endif
                     DptxAudioEnFlag&=(~(DpTxAudioIsWaitingToBeUnMuted|DpTxAudioIsWaitingToBeMuted));  
                     msg(" ^2^ DpTxAudioIsMuted(%Ld)",gm_GetSystemTime());
                  }
                  else if(DptxAudioEnFlag & DpTxAudioIsWaitingToBeUnMuted)
                  {
                     DPTX_MuteAudio(iDpTxPort, FALSE); 
                     DptxAudioEnFlag&=(~(DpTxAudioIsWaitingToBeUnMuted|DpTxAudioIsWaitingToBeMuted));   
                     msg(" ^4^ DpTxAudioIsUnMuted(%Ld)",gm_GetSystemTime());
                  }
               }
            #if (PATCH_DPTX_NO_AUD_BY_ZERO_OF_SDP_AUD_M_MEAS !=0)
               if(SYS_API_AUDIO_Is_SyncDetected(AAPI_Audio_Get_SourceChannel())==TRUE)
               {
                  if((gm_ReadRegDWord(DPTX_SDP_AUD_M_MEAS)==0) && 
                     (gm_ReadRegWord(DPTX_SDP_CTRL) & DPTX_SDP_STRM_EN) && 
                     (!(gm_ReadRegWord(DPTX_SDP_AUD_CTRL) & DPTX_SDP_AUD_MUTE))&&
                     (gm_TimerCheck(DPTX_AUD_MONITOR_10ms_TMR)!=TIMER_OK))
                  {
                     msg("#######PATCH_DPTX_NO_AUD_BY_ZERO_OF_SDP_AUD_M_MEAS######",0);
                     gm_WriteRegWord(DPTX_RESET_CTRL, DPTX_RESET_LINK_CLK);
                     gm_Delay1ms(1);
                     gm_SetRegBitsWord(DP12TX_TEST_CTRL, DPTX_HPD_LOOPBACK_EN);
                     gm_Delay1ms(5);
                     gm_ClearRegBitsWord(DP12TX_TEST_CTRL, DPTX_HPD_LOOPBACK_EN);
                     gm_TimerStart(DPTX_AUD_MONITOR_10ms_TMR, 400);
                     gm_WriteRegWord(DPTX_SDP_UPDATE, DPTX_SDP_UPDATE_CTRL);
                  }
               }
            #endif      
            }
            #endif 
         }
      }
      else
      {
         //gm_Print("L",0);
         if (SstOnlyTx_EnVdoFlag==TRUE)
         {
            SstOnlyTx_EnVdoFlag=FALSE;
      #if (EnableDpTxDelayCount==1)
            ExtraDpTxEnDelay=ExtraDpTxEnableDelayCount;
      #endif
            msg("SstOnlyTx Link is lost",0);
         }
      }
   }
#endif

#endif	//DP_TOTAL_NUM_PORTS 

/*
   {
      static WORD preCnt=0x8000;
      WORD NewCnt=gm_ReadRegWord(DP12RX0_ERROR_CNT);
      if(NewCnt!=preCnt)
      {
         preCnt=NewCnt;
         gm_Print("ErrCnt=0x%x",NewCnt);
      }
   }
*/   
}


gmt_CEA861_AVI_Y DpRxGetAviColorSpace(BYTE InputConnector)
{
   DP_MsaParameters_t SysMsParam_ps;
   DP_GetRxVideoParameters(DpRxGetPortId(InputConnector), &SysMsParam_ps);
   return (SysMsParam_ps.MsaFlags_16.Msa_bfs.DpColorFormat_bf4 & (BIT0|BIT1));
}


BOOL DpRxGetAudioConfig(BYTE InputConnector,DP_AudioConfig_t * AudioConfig_ps)
{
#if defined(ST_4K2K_93xx_BOARD) && defined(DP_HUB_2_LOG_PORTS)  
   if (DP_IsRxVideoPresent(DpRxGetPortId(InputConnector))==TRUE)
#else
   if (DpRxIsStableState(InputConnector)==TRUE)
#endif      
   {
      DPRX_GetAudioConfig(DpRxGetPortId(InputConnector),AudioConfig_ps);
      return TRUE;
   }
   return FALSE;
}


BOOL DpRxIsMST_Stream(BYTE InputConnector)
{
   gmt_DPRX_DID b_DID=gm_GetDpDigitalID(InputConnector);

   if(b_DID==DID_DPRX0)
   {
   #if defined(ST_4K2K_93xx_BOARD) && defined(DP_HUB_2_LOG_PORTS)  
      if(DpRx0_NumberVirtualChannelsAllocated == 2)
         return TRUE;
   #else
      if(gm_ReadRegWord(DP12RX_RL_CONFIG) & DPRX_TYPE)
      {
         return TRUE;
      }
   #endif   
   }
   return FALSE;
}

/*
BOOL DpRxIsMstInfoUpdated(BYTE InputConnector)
{
   gmt_DPRX_DID b_DID=gm_GetDpDigitalID(InputConnector);

   if(b_DID==DID_DPRX0)
      return DpRx0_MstInfoUpdated;

   return TRUE;
}
*/
#if defined(ST_4K2K_93xx_BOARD) && defined(DP_HUB_2_LOG_PORTS)
/*ChengLin 20130530: This function for DP 2 Logical Ports Allocate payload number check, the result will sync to Athena L side*/
void DP_TwoLogical_CheckAndSyncAllocatedPayload(void)
{
   if(gm_ReadRegWord(DP12RX_RL_CONFIG) & DPRX_TYPE)
      {
      DpRx0_NumberVirtualChannelsAllocated=DP12_GetNumberVirtualChannelsAllocated(MstRxPort);
      Two_LogicalPort_msg("MST- DpRx0_NumberVirtualChannelsAllocated = %d.",DpRx0_NumberVirtualChannelsAllocated);
      }
   else
      {
      DpRx0_NumberVirtualChannelsAllocated=1;
      Two_LogicalPort_msg("SST- DpRx0_NumberVirtualChannelsAllocated = %d.",DpRx0_NumberVirtualChannelsAllocated);                           
      }
   
   SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_DpRx0_MstInfo, &DpRx0_NumberVirtualChannelsAllocated);

}

void ResetDpRx0VirtualChannelsAllocated(void)
{
   DpRx0_NumberVirtualChannelsAllocated=DEFAUL_NUM_VC_ALLOCATED;
}


void ResetDpRx0MstInfoUpdatFlag(void)
{
   DpRx0_WaitMstInfoTimerStarted=FALSE;
   DpRx0_MstInfoUpdated=FALSE;
   DpRx0_Ready2UpdateMstInfo=FALSE;
}


void UpdateDpRx0MstInfo(BYTE MstInfo)
{
   DpRx0_NumberVirtualChannelsAllocated=MstInfo;
   DpRx0_MstInfoUpdated=TRUE;
   //gm_Print("M=%d",DpRx0_NumberVirtualChannelsAllocated);
}

BOOL IsDpRx0_Ready2UpdateMstInfo(void)
{
   return DpRx0_Ready2UpdateMstInfo;
}
#endif

BOOL DpRxIsLT_Success(BYTE InputConnector)
{
   BYTE PortId_8;
   DPRXLSMStatus_t sts;
   if (GetPortConnType(InputConnector)!=CT_DISPLAY_PORT)
      return FALSE;  //Invalid InputConnector

   PortId_8= DpRxGetPortId(InputConnector);
   if(PortId_8==LogicalTxPort)
      PortId_8=MstRxPort;

   sts=DP_GetLinkStatus(PortId_8);
   //msg("DP_GetLinkStatus=%d", sts);
   if (sts==DPRXLSMStatus_SL_DONE)
      return TRUE;
   else
      return FALSE;
}


BOOL DpRxIsStableState(BYTE InputConnector)
{

   if (GetPortConnType(InputConnector)!=CT_DISPLAY_PORT)
      return FALSE;  //Invalid InputConnector

/*ChengLin 20130304: Don't to do mode setup when image sync stste is "IMAGE_LVDS_ON"*/
#ifdef ST_4K2K_93xx_BOARD
#if ENABLE_ATHENA_R_L_IMAGE_SYNC_HANDLER
	if(ImageSyncHandleGetState() == IMAGE_LVDS_ON_STATE)
		return FALSE;	   
#endif

#ifdef DP_HUB_2_LOG_PORTS
#if (ENABLE_DPRX0_RE_MODE_SETUP)
if(ForceDP_ReModeSetup)
{
   if(EnableReModeSetupStart == FALSE)
   {
      gm_TimerStart(DpRx0_EnableReModeSetupCheck_10ms_TMR, 15); //150ms
      EnableReModeSetupStart = TRUE;      
      Two_LogicalPort_msg("\n<Timer> DpRx0_EnableReModeSetupCheck_10ms_TMR Start!!\n",0);
   }

   if(gm_TimerCheck(DpRx0_EnableReModeSetupCheck_10ms_TMR)==TIMER_TMO && EnableReModeSetupStart)
   {
      gm_TimerStop(DpRx0_EnableReModeSetupCheck_10ms_TMR); 
      EnableReModeSetupStart = FALSE;
      ForceDP_ReModeSetup = FALSE;
      Two_LogicalPort_msg("\n<Time out> DpRx0_EnableReModeSetupCheck_10ms_TMR Timeout!!\n",0);       
   }            
   return FALSE;
}

#endif
#endif

#endif

#if (PATCH_OF_NOVIDFLAG_UNSTABLE_VB !=0)
   {
      BYTE loop=1;
      
#if (SPEEDUP_MODE_CHANGE == ENABLE)
      if(Get_ReCheckNoVidFlag(InputConnector)==TRUE)
         loop=1;
#else
if(Get_ReCheckNoVidFlag(InputConnector)==TRUE)
   loop=3;
#endif

      while(loop--)
      {
#endif      
         if (DP_IsRxVideoPresent(DpRxGetPortId(InputConnector))==TRUE)
         {
            #if (PatchForNoVideoFlagNotPresented==1)
            {
               gmt_SRCPWR_STATE b_sts=GetComboPhySrcPwrState(gm_GetDpAnaPhyID(InputConnector));
               if ((b_sts==SRCPWR_OFF)||(b_sts==SRCPWR_LOSTING))
                  return FALSE;
            }
            if (gm_ReadRegByteDP12Rx(gm_GetDpDigitalID(InputConnector),DP12RX0_BE_IRQ_STATUS) & DPRX_NO_BS_STS)// &&
      //   	  (gm_ReadRegByteDP12Rx(gm_GetDpDigitalID(InputConnector),DP12RX0_ATTR_STATUS) & DPRX_VFIFO_ERR)//)
               return FALSE;

            #endif

            #if (PATCH_OF_MS_N_ZERO_CHECK==1)
            {
               DWORD dw_N=0;
               dw_N= gm_ReadRegWordDP12Rx(gm_GetDpDigitalID(InputConnector),DP12RX0_MS_N+2) & 0xF ;
               dw_N=(dw_N<<16) | gm_ReadRegWordDP12Rx(gm_GetDpDigitalID(InputConnector),DP12RX0_MS_N);
               if (dw_N==0)
                  return FALSE;
            }
            #endif
      #if (FORCE_NO_SIGNAL_IF_DPCD600_2==1)
      		if(IsDpcdSetPowerReallyDown(InputConnector)==TRUE)
      			return FALSE;
      #endif			

      #if defined(ST_4K2K_93xx_BOARD) && defined(DP_HUB_2_LOG_PORTS)  
            if(gm_GetDpDigitalID(InputConnector)==DID_DPRX0)
            {               
               if(GetChipID_4K2K()==CHIPID_4K2K_R)
               {
                  if(DpRx0_MstInfoUpdated==FALSE)
                  {
                     if(DpRx0_WaitMstInfoTimerStarted==FALSE)
                     {
                        gm_TimerStart(DpRx0_WaitMstInfoTimeOut_10ms_TMR, AthenaR_WaitMstInfoTimex10ms);
                        DpRx0_WaitMstInfoTimerStarted=TRUE;
                        Two_LogicalPort_msg("*T-Srt",0);
                     }
                     else if(gm_TimerCheck(DpRx0_WaitMstInfoTimeOut_10ms_TMR)==TIMER_TMO) 
                     {

                        if(UserPrefCaptureM != CAPTURE_MODE_HALF_HORIZONTAL)
                        {
                           if(!SocIPCIsSlaveLeftReady())
                           {
                              Two_LogicalPort_msg("L Not Ready!",0);
                              return FALSE;  
                           }

                           if( SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RO_DpRx0_IsStable, NULL) == TRUE)
                           {
                              if(gAthena_LEFT_FE_Slave_Parameters.DPRX0_IsStable==FALSE)
                              {
                                 Two_LogicalPort_msg("F1",0);
                                 return FALSE;
                              }
                           } 
                           else
                           {
                              Two_LogicalPort_msg("F2",0);
                              return FALSE;
                           }
                        }
                        else
                        {
                           Two_LogicalPort_msg("PBP Mode - Do not check Athena Left Signal Status!!",0);
                        }
                        #if defined(ST_4K2K_93xx_BOARD) && defined(DP_HUB_2_LOG_PORTS)
                        DP_TwoLogical_CheckAndSyncAllocatedPayload();
                        #endif
                        DpRx0_MstInfoUpdated=TRUE; 
                        DpRx0_WaitMstInfoTimerStarted=FALSE;
                     }
                   return FALSE;
                  }
               }
               else if(GetChipID_4K2K()==CHIPID_4K2K_L)
               {
                  if(DpRx0_MstInfoUpdated==FALSE)
                  {
                     if(DpRx0_WaitMstInfoTimerStarted==FALSE)
                     {
                        gm_TimerStart(DpRx0_WaitMstInfoTimeOut_10ms_TMR, AthenaL_WaitMstInfoTimex10ms);
                        DpRx0_WaitMstInfoTimerStarted=TRUE;
                        DpRx0_Ready2UpdateMstInfo=TRUE;
                        Two_LogicalPort_msg("*T-Srt",0);
                     }
                     else if(gm_TimerCheck(DpRx0_WaitMstInfoTimeOut_10ms_TMR)==TIMER_TMO) 
                     {
                        DpRx0_MstInfoUpdated=TRUE;
                        DpRx0_WaitMstInfoTimerStarted=FALSE;
                        Two_LogicalPort_msg("*TMO",0);
                     }
                     Two_LogicalPort_msg("Nd-",0);
                     return FALSE;
                  }
               }
            }
      #endif
            return TRUE;
         }

#if (PATCH_OF_NOVIDFLAG_UNSTABLE_VB !=0)
         if(loop)
            gm_Delay1ms(1);  

      }
      Set_ReCheckNoVidFlag(InputConnector,FALSE);
   }
#endif
   //   gm_Print("DP12RX0_IFM_IRQ_STATUS=0x%x",gm_ReadRegWord(DP12RX0_IFM_IRQ_STATUS));
   //   gm_Print("DP12RX0_SDP_TATUS=0x%x",gm_ReadRegWord(DP12RX0_SDP_STATUS));
   #if defined(ST_4K2K_93xx_BOARD) && defined(DP_HUB_2_LOG_PORTS)  
   if(gm_GetDpDigitalID(InputConnector)==DID_DPRX0)
   {
      if(DpRx0_MstInfoUpdated==TRUE)
      {
         Two_LogicalPort_msg("@F",0);
         ResetDpRx0MstInfoUpdatFlag();
      }
   }
   #endif
   
   return FALSE;
}

BOOL gm_DpRxIsTmingMatched(gmt_PHY_CH Channel, gmt_FrontEndSignalTiming_t* Ptr_Timing)
{
   BOOL ret=FALSE;
   
   if (DpRxGetSignalTiming(gB_CurrentInputConnector[Channel], Ptr_Timing))
   {
      gmt_IbdStruct ibd;

      {
         gmt_DPRX_DID b_DID = gm_GetDpDigitalID(gmvb_InputConnector[Channel]);
         WORD W_IP_Src = gm_ReadRegWord(ip_source[Channel]) & ~(IMP_VS_INV | IMP_HS_INV );

         if ((gm_ReadRegWordDP12Rx(b_DID,DP12RX0_MS_HS_WIDTH) & DPRX_MS_HS_POLARITY))
         {
            W_IP_Src |= IMP_HS_INV;
         }

         if ((gm_ReadRegWordDP12Rx(b_DID,DP12RX0_MS_VS_WIDTH) & DPRX_MS_VS_POLARITY))
            W_IP_Src |= IMP_VS_INV;

         gm_WriteRegWord(ip_source[Channel], W_IP_Src);
      }      

      gm_IbdRead(Channel, &ibd);

      if ((ibd.HActive==Ptr_Timing->HActive) && (ibd.HTotal==Ptr_Timing->HTotal))
      {
         if (Ptr_Timing->Interlaced)
         {
            if ((abs(ibd.VActive-Ptr_Timing->VActive)<=2) && (abs(ibd.VTotal-Ptr_Timing->VTotal)<=2))
               ret= TRUE;
         }
         else
         {
            if ((ibd.VActive==Ptr_Timing->VActive) && (ibd.VTotal==Ptr_Timing->VTotal))
               ret= TRUE;
         }
      }
   }
   msg_ii("P(%d)-TM(%d)",gB_CurrentInputConnector[Channel],ret);
#if (PATCH_OF_NOVIDFLAG_UNSTABLE_VB !=0)
   if(ret==TRUE)
   {
      if((Ptr_Timing->VTotal - Ptr_Timing->VActive) > THRESH_NOVID_VBLANK_MIN)
      {
         Set_ReCheckNoVidFlag(gB_CurrentInputConnector[Channel], TRUE);
         #if (PATCH_OF_ASTRO_T1075_INTERLACE_ERROR !=0)
          if((Ptr_Timing->HActive==1920)&&(Ptr_Timing->VActive==540))
            Ptr_Timing->Interlaced=TRUE;
         #endif
      }
   }
#endif

   return ret;
}


gmt_3D_INPUT_FORMAT_TYPE gm_DpRxGet3DFormat(BYTE InputConnector)
{

   DP_MsaParameters_t SysMsParam_ps;
   DP_GetRxVideoParameters(DpRxGetPortId(InputConnector), &SysMsParam_ps);

   if (SysMsParam_ps.MsaFlags_16.Msa_bfs.DpStereoMode_bf2)
   #if 1
   {
      if((gm_ReadRegWord(PRODUCT_REV) & PROD_REV) >= REV_CODE_2_1)
      {
         STDP_3DFormat_t Format=DP_GetRx3DFormat(DpRxGetPortId(InputConnector));
         switch (Format)
         {
            case STDP_3D_FORMAT_NO_3D_VIDEO:
            case STDP_3D_FORMAT_UNKNOWN:
            //return gmd_3D_IN_UNKNOWN;
            return gmd_3D_IN_STACKED_TOP_BOTTOM;      //Current Graphic card not support VSC packet. So, default set to STACKED_TOP_BOTTOM;

            case STDP_3D_FORMAT_FRAME_SEQ_MISC:
            return gmd_3D_IN_FRAME_SEQUENTIAL;

            case STDP_3D_FORMAT_FRAME_SEQ_RIGHT:
            return gmd_3D_IN_FRAME_SEQUENTIAL_RIGHT;

            case STDP_3D_FORMAT_FRAME_SEQ_LEFT:
            return gmd_3D_IN_FRAME_SEQUENTIAL_LEFT;

            case STDP_3D_FORMAT_STACKED_FRAME:
            return gmd_3D_IN_STACKED_TOP_BOTTOM;

            case STDP_3D_FORMAT_INTERLEAVED_RIGHT_EVEN:
            return gmd_3D_IN_INTERLACED_RIGHT_EVEN;

            case STDP_3D_FORMAT_INTERLEAVED_RIGHT_ODD:
            return gmd_3D_IN_INTERLACED_RIGHT_ODD;

            case STDP_3D_FORMAT_INTERLEAVED_CHECKERBOARD:
            return gmd_3D_IN_INTERLACED_RIGHT_CHECKERBOARD;

            case STDP_3D_FORMAT_INTERLEAVED_VERTICAL_LEFT:
            return gmd_3D_IN_INTERLACED_RIGHT_V_LEFT;

            case STDP_3D_FORMAT_INTERLEAVED_VERTICAL_RIGHT: 
            return gmd_3D_IN_INTERLACED_RIGHT_V_RIGHT;

            case STDP_3D_FORMAT_SBS_RHALF_RIGHT:
            return gmd_3D_IN_SIDE_BY_SIDE_HALF_RIGHT;

            case STDP_3D_FORMAT_SBS_RHALF_LEFT:
            return gmd_3D_IN_SIDE_BY_SIDE_HALF_LEFT;
         }
      }
      else
         return gmd_3D_IN_STACKED_TOP_BOTTOM;	
   }
   #else
      return gmd_3D_IN_STACKED_TOP_BOTTOM;				//Temporary fixed to Stacked top_bottom.
   #endif

   return gmd_3D_IN_UNKNOWN;
}





gmt_COLOR_DEPTH gm_DpRxGetColorDepth(BYTE InputConnector)
{
   DP_MsaParameters_t SysMsParam_ps;
   DP_GetRxVideoParameters(DpRxGetPortId(InputConnector), &SysMsParam_ps);

   switch (SysMsParam_ps.MsaFlags_16.Msa_bfs.DpColorDepth_bf3)
   {
      case DP_MsaColorDepth_6Bits:
         return gm_COLOR_DEPTH_18BPP;
      case DP_MsaColorDepth_8Bits:
         return gm_COLOR_DEPTH_24BPP;
      case DP_MsaColorDepth_10Bits:
         return gm_COLOR_DEPTH_30BPP;
      case DP_MsaColorDepth_12Bits:
         return gm_COLOR_DEPTH_36BPP;
      case DP_MsaColorDepth_16Bits:
         return gm_COLOR_DEPTH_48BPP;
   }
   return gm_COLOR_DEPTH_UNKNOWN;
}



STDPRX_ColorimetryStd_t DpRxGetColorimetry(BYTE InputConnector)
{
   DP_MsaParameters_t SysMsParam_ps;
   DP_GetRxVideoParameters(DpRxGetPortId(InputConnector), &SysMsParam_ps);

   if (SysMsParam_ps.MsaFlags_16.Msa_bfs.DpMsaColorMode_bf1)
      return STDPRX_COLORIMETRY_Yonly;

   return SysMsParam_ps.MsaFlags_16.Msa_bfs.DpColorFormat_bf4;
}


BOOL DpRxGetSignalTiming(BYTE InputConnector, gmt_FrontEndSignalTiming_t* Ptr_Timing)
{
   DP_MsaParameters_t SysMsParam_ps;
   DP_GetRxVideoParameters(DpRxGetPortId(InputConnector), &SysMsParam_ps);

   Ptr_Timing->PixelClockHz = SysMsParam_ps.PixelClockHz_32;

   Ptr_Timing->HActiveStart = SysMsParam_ps.HActiveStart_16;
   Ptr_Timing->HActive 	 = SysMsParam_ps.HActiveWidth_16;
   Ptr_Timing->HTotal		 = SysMsParam_ps.HTotal_16;
   //Ptr_Timing->HSyncPulse	 = SysMsParam_ps.HSyncWidth_16.Value_bf15;

   Ptr_Timing->VActiveStart = SysMsParam_ps.VActiveStart_16;
   Ptr_Timing->VActive 	 = SysMsParam_ps.VActiveWidth_16;
   Ptr_Timing->VTotal		 = SysMsParam_ps.VTotal_16;
   //Ptr_Timing->VSyncPulse	 = SysMsParam_ps.VSyncWidth_16.Value_bf15;
#if PATCH_OF_GET_INTERLACED_FLAG //Patch of Interlaced detection.
   if (gm_ReadRegByteDP12Rx(gm_GetDpDigitalID(InputConnector),DP12RX0_IFM_STATUS) & IFM_INTERLACED_MODE)
      Ptr_Timing->Interlaced	 = TRUE;
   else
      Ptr_Timing->Interlaced	 = FALSE;
#else
   Ptr_Timing->Interlaced	 = SysMsParam_ps.MsaVideoMode_e;
#endif
   Ptr_Timing->HS_Positive  = SysMsParam_ps.HSyncWidth_16.Polarity_bf1 ? 0:1;
   Ptr_Timing->VS_Positive  = SysMsParam_ps.VSyncWidth_16.Polarity_bf1 ? 0:1;



   return TRUE;

}

BYTE DpRxReadIfmClkDivRatio(BYTE InputConnector)
{
   BYTE DivRatio=(gm_ReadRegWordDP12Rx(gm_GetDpDigitalID(InputConnector),DP12RX1_IFM_CTRL) & 0x0C)>>2;
   if (DivRatio)
      return DivRatio*2;
   return 1;
}


void DpRxAfrEnable(BYTE InputConnector, BOOL b_En)
{
#if UseDpRxAfrControl
   if (b_En==TRUE)
      gm_WriteRegWordDP12Rx( gm_GetDpDigitalID(InputConnector),DP12RX0_DTG_CONF,DPRX_DTG_AFR_MASK_CLOCK_RECOVERY_LOST |
                             DPRX_DTG_AFR_MASK_SYM_LOCK_LOST| DPRX_DTG_AFR_MASK_ALIGN_ERROR); 
//                             DPRX_DTG_AFR_MASK_NO_VIDEO);    // Caused unstable to 480i in cut2
   else
      gm_WriteRegWordDP12Rx( gm_GetDpDigitalID(InputConnector),DP12RX0_DTG_CONF,0);

#else
   UNUSED_VAR(InputConnector);
   UNUSED_VAR(b_En);
#endif
}


void DpRxPrintSignalProperty(BYTE InputConnector)
{
   DP_MsaParameters_t SysMsParam_ps;

 #if defined(ST_4K2K_93xx_BOARD) && defined(DP_HUB_2_LOG_PORTS)  
   if (DpRxIsStableState(InputConnector)!=TRUE)
   {
       if (DP_IsRxVideoPresent(DpRxGetPortId(InputConnector))!=TRUE)
       {
         gm_Print("===DpRxInput(%d) Unstable===",InputConnector);
         return;
       }
   }
#else
   if (DpRxIsStableState(InputConnector)!=TRUE)
   {
      gm_Print("===DpRxInput(%d) Unstable===",InputConnector);
      return;
   }
#endif   

   gm_Print("===DpRxPrintSignalProperty:(%d)===",InputConnector);
   DP_GetRxVideoParameters(DpRxGetPortId(InputConnector), &SysMsParam_ps);


   gm_Print("PixelClock      :%LdHz",SysMsParam_ps.PixelClockHz_32);
   gm_Print("HActiveStart    :%d",SysMsParam_ps.HActiveStart_16);
   gm_Print("HActive         :%d",SysMsParam_ps.HActiveWidth_16);
   gm_Print("HTotal          :%d",SysMsParam_ps.HTotal_16);
   gm_Print("VActiveStart    :%d",SysMsParam_ps.VActiveStart_16);
   gm_Print("VActive         :%d",SysMsParam_ps.VActiveWidth_16);
   gm_Print("VTotal          :%d",SysMsParam_ps.VTotal_16);
   if (SysMsParam_ps.HSyncWidth_16.Polarity_bf1==TRUE)
      gm_Print("HSyncPolarity   :Negative",0);
   else
      gm_Print("HSyncPolarity   :Positive",0);  

   if (SysMsParam_ps.VSyncWidth_16.Polarity_bf1==TRUE)
      gm_Print("VSyncPolarity   :Negative",0);
   else
      gm_Print("VSyncPolarity   :Positive",0);      
#if PATCH_OF_GET_INTERLACED_FLAG
   if (gm_ReadRegByteDP12Rx(gm_GetDpDigitalID(InputConnector),DP12RX0_IFM_STATUS) & IFM_INTERLACED_MODE)
#else
   if (SysMsParam_ps.MsaVideoMode_e)
#endif
      gm_Print("ScanType        :Interlaced",0);
   else
      gm_Print("ScanType        :Progressive",0);

   if (SysMsParam_ps.MsaFlags_16.Msa_bfs.DpClockType_bf1==DP_MsaClockType_Async)
      gm_Print("ClockType       :Asynchronous",0);
   else
      gm_Print("ClockType       :Synchronous",0);

   if (SysMsParam_ps.MsaFlags_16.Msa_bfs.DpMsaColorMode_bf1)
      gm_Print("ColorFormat     :Y-Only",0);
   else
   {
      switch (SysMsParam_ps.MsaFlags_16.Msa_bfs.DpColorFormat_bf4)
      {
         case STDPRX_COLORIMETRY_RGB_DEFAULT:
            gm_Print ("ColorFormat     :RGB",0);
            break;
         case STDPRX_COLORIMETRY_xvYCC601_422:
            gm_Print ("ColorFormat     :xvYCC601_422",0);
            break;
         case STDPRX_COLORIMETRY_xvYCC601_444:
            gm_Print ("ColorFormat     :xvYCC601_444",0);
            break;
         case STDPRX_COLORIMETRY_XRGB:
            gm_Print ("ColorFormat     :XRGB",0);
            break;
         case STDPRX_COLORIMETRY_sRGB:
            gm_Print ("ColorFormat     :sRGB",0);
            break;
         case STDPRX_COLORIMETRY_ITU601_422:
            gm_Print ("ColorFormat     :ITU601_422",0);
            break;
         case STDPRX_COLORIMETRY_ITU601_444:
            gm_Print ("ColorFormat     :ITU601_444",0);
            break;
         case STDPRX_COLORIMETRY_DCI_P3:
            gm_Print ("ColorFormat     :DCI_P3",0);
            break;
         case STDPRX_COLORIMETRY_xvYCC709_422:
            gm_Print ("ColorFormat     :xvYCC709_422",0);
            break;
         case STDPRX_COLORIMETRY_xvYCC709_444:
            gm_Print ("ColorFormat     :xvYCC709_444",0);
            break;
         case STDPRX_COLORIMETRY_scRGB:
            gm_Print ("ColorFormat     :scRGB",0);
            break;
         case STDPRX_COLORIMETRY_AdobeRGB:
            gm_Print ("ColorFormat     :AdobeRGB",0);
            break;
         case STDPRX_COLORIMETRY_ITU709_422:
            gm_Print ("ColorFormat     :ITU709_422",0);
            break;
         case STDPRX_COLORIMETRY_ITU709_444:
            gm_Print ("ColorFormat     :ITU709_444",0);
            break;
         case STDPRX_COLORIMETRY_Color_Profile:
            gm_Print ("ColorFormat     :Color_Profile",0);
            break;
      }
   }

   switch (SysMsParam_ps.MsaFlags_16.Msa_bfs.DpColorDepth_bf3)
   {
      case DP_MsaColorDepth_6Bits:
         gm_Print ("ColorDepth      :6Bits",0);
         break;
      case DP_MsaColorDepth_8Bits:
         gm_Print ("ColorDepth      :8Bits",0);
         break;
      case DP_MsaColorDepth_10Bits:
         gm_Print ("ColorDepth      :10Bits",0);
         break;
      case DP_MsaColorDepth_12Bits:
         gm_Print ("ColorDepth      :12Bits",0);
         break;
      case DP_MsaColorDepth_16Bits:
         gm_Print ("ColorDepth      :16Bits",0);
         break;
   }

   switch (SysMsParam_ps.MsaFlags_16.Msa_bfs.DpStereoMode_bf2)
   {
      case DP_MsaStereoModeType_No3d:
         gm_Print ("StereoMode      :No3D",0);
         break;
      case DP_MsaStereoModeType_Right:
         gm_Print ("StereoMode      :Right",0);
         break;
      case DP_MsaStereoModeType_Reserved:
         gm_Print ("StereoMode      :Reserved",0);
         break;
      case DP_MsaStereoModeType_Left:
         gm_Print ("StereoMode      :Left",0);
         break;
   }

   if(SysMsParam_ps.MsaFlags_16.Msa_bfs.DpStereoMode_bf2 != DP_MsaStereoModeType_No3d)
   {
      STDP_3DFormat_t Format=DP_GetRx3DFormat(DpRxGetPortId(InputConnector));
      switch (Format)
      {
         case STDP_3D_FORMAT_NO_3D_VIDEO:
            gm_Print("3D_Format       :NO_3D_VIDEO",0);
            break;

         case STDP_3D_FORMAT_UNKNOWN:
            gm_Print("3D_Format       :3D_UNKNOWN",0);
            break;

         case STDP_3D_FORMAT_FRAME_SEQ_MISC:
            gm_Print("3D_Format       :FRAME_SEQ_MISC",0);
            break;

         case STDP_3D_FORMAT_FRAME_SEQ_RIGHT:
            gm_Print("3D_Format       :FRAME_SEQ_RIGHT",0);
            break;

         case STDP_3D_FORMAT_FRAME_SEQ_LEFT:
            gm_Print("3D_Format       :FRAME_SEQ_LEFT",0);
            break;

         case STDP_3D_FORMAT_STACKED_FRAME:
            gm_Print("3D_Format       :STACKED_FRAME",0);
            break;

         case STDP_3D_FORMAT_INTERLEAVED_RIGHT_EVEN:
            gm_Print("3D_Format       :INTERLEAVED_RIGHT_EVEN",0);
            break;

         case STDP_3D_FORMAT_INTERLEAVED_RIGHT_ODD:
            gm_Print("3D_Format       :INTERLEAVED_RIGHT_ODD",0);
            break;

         case STDP_3D_FORMAT_INTERLEAVED_CHECKERBOARD:
            gm_Print("3D_Format       :INTERLEAVED_CHECKERBOARD",0);
            break;

         case STDP_3D_FORMAT_INTERLEAVED_VERTICAL_LEFT:
            gm_Print("3D_Format       :INTERLEAVED_VERTICAL_LEFT",0);
            break;

         case STDP_3D_FORMAT_INTERLEAVED_VERTICAL_RIGHT: 
            gm_Print("3D_Format       :INTERLEAVED_VERTICAL_RIGHT",0);
            break;

         case STDP_3D_FORMAT_SBS_RHALF_RIGHT:
            gm_Print("3D_Format       :SBS_RHALF_RIGHT",0);
            break;

         case STDP_3D_FORMAT_SBS_RHALF_LEFT:
            gm_Print("3D_Format       :SBS_RHALF_LEFT",0);
            break;
      }
   }

}


void DpRxPrintAudioProperty(BYTE InputConnector)
{
   DP_AudioConfig_t AudioConfig;
   if (DpRxGetAudioConfig(InputConnector, &AudioConfig)==FALSE)
   {
      gm_Print("===DpRxInput(%d) Unstable===",InputConnector);
      return;
   }

   gm_Print("===DpRxPrintAudioProperty:(%d)===",InputConnector);
   gm_Print("ChannelCount    :%d",AudioConfig.ChannelCount_8);
   gm_Print("ChannelAllocate :%d",AudioConfig.ChannelAllocation_8);

   switch (AudioConfig.CodingType_e)
   {
      case DP_AudioCodingType_NONE:
         gm_Print("CodingType      :NONE",0);
         break;
      case DP_AudioCodingType_PCM:
         gm_Print("CodingType      :PCM",0);
         break;
      case DP_AudioCodingType_AC3:
         gm_Print("CodingType      :AC3",0);
         break;
      case DP_AudioCodingType_MPEG1:
         gm_Print("CodingType      :MPEG1",0);
         break;
      case DP_AudioCodingType_MP3:
         gm_Print("CodingType      :MP3",0);
         break;
      case DP_AudioCodingType_MPEG2:
         gm_Print("CodingType      :MPEG2",0);
         break;
      case DP_AudioCodingType_AAC:
         gm_Print("CodingType      :AAC",0);
         break;
      case DP_AudioCodingType_DTS:
         gm_Print("CodingType      :PCM",0);
         break;
      case DP_AudioCodingType_ATRAC:
         gm_Print("CodingType      :ATRAC",0);
         break;
      case DP_AudioCodingType_DSD:
         gm_Print("CodingType      :DSD",0);
         break;
      case DP_AudioCodingType_DDPlus:
         gm_Print("CodingType      :DDplus",0);
         break;
      case DP_AudioCodingType_DTS_HD:
         gm_Print("CodingType      :DTS_HD",0);
         break;
      case DP_AudioCodingType_MAT:
         gm_Print("CodingType      :MAT",0);
         break;
      case DP_AudioCodingType_DST:
         gm_Print("CodingType      :DST",0);
         break;
      case DP_AudioCodingType_WMA_PRO:
         gm_Print("CodingType      :WMA_PRO",0);
         break;
      case DP_AudioCodingType_RESERVED:
         gm_Print("CodingType      :RESERVED",0);
         break;
   }


   switch (AudioConfig.SamplingFrequency_e)
   {
      case 	DP_AudioSamplingFrequency_None:
         gm_Print("SamplingFreq    :None",0);
         break;
      case 	DP_AudioSamplingFrequency_32_Khz:
         gm_Print("SamplingFreq    :32Khz",0);
         break;
      case 	DP_AudioSamplingFrequency_44_1_Khz:
         gm_Print("SamplingFreq    :44.1Khz",0);
         break;
      case 	DP_AudioSamplingFrequency_48_Khz:
         gm_Print("SamplingFreq    :48Khz",0);
         break;
      case 	DP_AudioSamplingFrequency_88_2_Khz:
         gm_Print("SamplingFreq    :88.2Khz",0);
         break;
      case 	DP_AudioSamplingFrequency_96_Khz:
         gm_Print("SamplingFreq    :96Khz",0);
         break;
      case 	DP_AudioSamplingFrequency_176_4_Khz:
         gm_Print("SamplingFreq    :176.4Khz",0);
         break;
      case 	DP_AudioSamplingFrequency_192_Khz:
         gm_Print("SamplingFreq    :192Khz",0);
         break;
      case 	DP_AudioSamplingFrequency_22_05_Khz:
         gm_Print("SamplingFreq    :22.05Khz",0);
         break;
      case 	DP_AudioSamplingFrequency_24_Khz:
         gm_Print("SamplingFreq    :24Khz",0);
         break;
      case 	DP_AudioSamplingFrequency_768_Khz:
         gm_Print("SamplingFreq    :768Khz",0);
         break;
   }

   switch (AudioConfig.SampleSize_e)
   {
      case 	DP_AudioSampleSize_None:
         gm_Print("SampleSize      :None",0);
         break;
      case 	DP_AudioSampleSize_16Bits:
         gm_Print("SampleSize      :16Bits",0);
         break;
      case 	DP_AudioSampleSize_20Bits:
         gm_Print("SampleSize      :20Bits",0);
         break;
      case 	DP_AudioSampleSize_24Bits:
         gm_Print("SampleSize      :24Bits",0);
         break;
      case 	DP_AudioSampleSize_17Bits:
         gm_Print("SampleSize      :17Bits",0);
         break;
      case 	DP_AudioSampleSize_18Bits:
         gm_Print("SampleSize      :18Bits",0);
         break;
      case 	DP_AudioSampleSize_19Bits:
         gm_Print("SampleSize      :19Bits",0);
         break;
      case 	DP_AudioSampleSize_21Bits:
         gm_Print("SampleSize      :21Bits",0);
         break;
      case 	DP_AudioSampleSize_22Bits:
         gm_Print("SampleSize      :22Bits",0);
         break;
      case 	DP_AudioSampleSize_23Bits:
         gm_Print("SampleSize      :23Bits",0);
         break;
      case 	DP_AudioSampleSize_32Bits:
         gm_Print("SampleSize      :32Bits",0);
         break;
   }

   switch (AudioConfig.AudioPort_e)
   {
      case 	DP_AudioPort_I2s:
         gm_Print("Port            :I2s",0);
         break;
      case	DP_AudioPort_Spdif:
         gm_Print("Port            :Spdif",0);
         break;
   }

   switch (AudioConfig.AudioStreamType_e)
   {
      case DP_AudioStreamType_Unknown:
         gm_Print("StreamType      :Unknown",0);
         break;
      case DP_AudioStreamType_Iec:
         gm_Print("StreamType      :Iec",0);
         break;
      case DP_AudioStreamType_Dsd:
         gm_Print("StreamType      :Dsd",0);
         break;
      case DP_AudioStreamType_Dst:
         gm_Print("StreamType      :Dst",0);
         break;
      case DP_AudioStreamType_Hbr:
         gm_Print("StreamType      :Hbr",0);
         break;
   }

   switch (AudioConfig.AudioType_e)
   {
      case 	DP_AudioType_Compressed:
         gm_Print("AudioType       :Compressed",0);
         break;
      case	DP_AudioType_LPCM:
         gm_Print("AudioType       :LPCM",0);
         break;
   }

   gm_Print("DacsUsed        :%d",AudioConfig.AudioDacsUsed_8);

}


void Set_SstOnlyTx_EnVdoFlag(BOOL EnFlag)
{
#if (DP_NUM_SST_ONLY_TX != 0) && (NON_SCALING_OUTPUT ==0)
   SstOnlyTx_EnVdoFlag = EnFlag;
#else
   UNUSED_VAR(EnFlag);
#endif
}

#define DPTX_LL_TRAINING_PATTERN_SET          0x0300
void DpTxEnableVideo(BYTE TimingMethod)
{
#if DP_NUM_SST_ONLY_TX != 0
   DP_MsaParameters_t MsaParams_s;
   STDP_3DFormat_t ThreeDFormat;
   STDP_3DView_t ThreeDView;

#ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()!=CHIPID_4K2K_FE)
      return;
#endif
   
   #if (DPTX_PING_SINK_DEVICE_WHEN_VIDEO_ON !=0)
   if(GetCurrentDPTXPort()==iDpTxPort)
   {
      DPTX_LinkTrainingStatus_t LtStatus;

      DP12_SetCurrentTxByPortId(iDpTxPort);   
      LtStatus=Tx_Fe_LinkGetLastLtStatus();

      if((LtStatus!=DPTX_LinkTrainingStatus_None) && (LtStatus!=DPTX_LinkTrainingStatus_InProgress))
      {
         Tx_Fe_IsLinkTrainingRequired();
      }
   }
   #endif
   if (DPTX_IsLinkReady(iDpTxPort)==FALSE)
      return;
      
   if (TimingMethod == DPTX_TIMING_BY_INPUT)
   {
      msg("DpTxEnableVideo- Set Timing by Input",0);      
      if (GetChannelConnType(gmvb_MainChannel) == CT_DISPLAY_PORT)
      {
         DP_GetRxVideoParameters(DpRxGetPortId(gB_CurrentInputConnector[gmvb_MainChannel]), &MsaParams_s);
         #if (PASS_THRU_MODE == 0)
         if (DpRxGetAviColorSpace(gB_CurrentInputConnector[gmvb_MainChannel]) == gmd_CEA861_AVI_Y_YUV422)
            MsaParams_s.MsaFlags_16.Msa_bfs.DpColorFormat_bf4 = gmd_CEA861_AVI_Y_YUV444;
         #endif

         if(MsaParams_s.MsaFlags_16.Msa_bfs.DpStereoMode_bf2!=DP_MsaStereoModeType_No3d)
         {
            ThreeDFormat=DP_GetRx3DFormat(DpRxGetPortId(gB_CurrentInputConnector[gmvb_MainChannel]));
            if(MsaParams_s.MsaFlags_16.Msa_bfs.DpStereoMode_bf2 == DP_MsaStereoModeType_Right)
               ThreeDView= STDP_3D_RIGHT_VIEW;
            else
               ThreeDView= STDP_3D_LEFT_VIEW;
         }
      }
      else  // Non-DpRx inputs
      {  
         WORD W_Hperiod;
         WORD W_Vperiod;
         WORD W_Hpulse;
         WORD W_Vpulse;
         /*********************************Setup PixelClock***************************/
      //#if (NON_SCALING_OUTPUT == 1) && (PASS_THRU_MODE == 0) && (DEINTERLACED_OUTPUT == 1)
		#ifdef OUTPUT_MODE_CHANGE
	 	if(UserPrefOutputMode == OPM_DInterlaced)
     	{
	         if (stInput[gmvb_MainChannel].ModeFlags & gmd_MODE_INTERLACED)
	            MsaParams_s.PixelClockHz_32 = ((DWORD)stInput[CH_A].Htotal * (stInput[CH_A].Vtotal * 2 + 1)/10) *  stInput[CH_A].Vfrq;
			else
				MsaParams_s.PixelClockHz_32 = ((DWORD)stInput[CH_A].Htotal * stInput[CH_A].Vtotal/10) *  stInput[CH_A].Vfrq;
		}
         else
		 #endif
      //#endif
         	MsaParams_s.PixelClockHz_32 = ((DWORD)stInput[CH_A].Htotal * stInput[CH_A].Vtotal/10) *  stInput[CH_A].Vfrq;
               
         /*********************************Setup HTotal*******************************/
         MsaParams_s.HTotal_16       = stInput[CH_A].Htotal;

         /*********************************Setup HStart*******************************/
         MsaParams_s.HActiveStart_16 = stInput[CH_A].Hstart;

         /*********************************Setup HAcitve******************************/
         MsaParams_s.HActiveWidth_16 = stInput[CH_A].Hactive;
         W_Hperiod = gm_ReadRegWord(MIFM_HS_PERIOD);
         W_Hpulse  = gm_ReadRegWord(MIFM_HS_PULSE);

         /*********************************Setup HSyncWidth***************************/      
         /*********************************Setup HSyncPol*****************************/
      #if 1
         if (W_Hpulse > (W_Hperiod / 2))
            W_Hpulse = W_Hperiod - W_Hpulse;

         MsaParams_s.HSyncWidth_16.Value_bf15 = (WORD)((DWORD)(MsaParams_s.HTotal_16*W_Hpulse) / W_Hperiod);
         MsaParams_s.HSyncWidth_16.Value_bf15 &= 0x7FF8;
         if (MsaParams_s.HSyncWidth_16.Value_bf15 < 8)
            MsaParams_s.HSyncWidth_16.Value_bf15 = 8;

         MsaParams_s.HSyncWidth_16.Polarity_bf1 = (stInput[gmvb_MainChannel].ModeFlags & gmd_MODE_NEG_HSYNC)? 1:0;
      #else
         if (W_Hpulse > (W_Hperiod / 2))
            MsaParams_s.HSyncWidth_16.Value_bf15   = W_Hperiod - W_Hpulse;
         else
            MsaParams_s.HSyncWidth_16.Value_bf15   = W_Hpulse;
      #endif


         /*********************************Setup VTotal*******************************/
         /*********************************Setup VActive******************************/
         /*********************************Setup VideoMode****************************/
      //#if (NON_SCALING_OUTPUT == 1) && (PASS_THRU_MODE == 0) && (DEINTERLACED_OUTPUT == 1)
	#ifdef OUTPUT_MODE_CHANGE
	  if(UserPrefOutputMode == OPM_DInterlaced)
  	{
	         if (stInput[gmvb_MainChannel].ModeFlags & gmd_MODE_INTERLACED)
	         {
	            MsaParams_s.VTotal_16 = stInput[gmvb_MainChannel].Vtotal * 2 + 1;
	            MsaParams_s.VActiveWidth_16 = stInput[gmvb_MainChannel].Vactive * 2;
	         }
	         else
	         {
	            MsaParams_s.VTotal_16 = stInput[gmvb_MainChannel].Vtotal;
	            MsaParams_s.VActiveWidth_16 = stInput[gmvb_MainChannel].Vactive;                 
	         }

	         MsaParams_s.MsaVideoMode_e = DP_MsaVideoMode_Progressivel; 
  	}
	  else
	  #endif
  	{
      //#else
         MsaParams_s.MsaVideoMode_e = (stInput[gmvb_MainChannel].ModeFlags & gmd_MODE_INTERLACED)? DP_MsaVideoMode_Interlaced : DP_MsaVideoMode_Progressivel;
         
         if (MsaParams_s.MsaVideoMode_e == DP_MsaVideoMode_Interlaced)
            MsaParams_s.VTotal_16 = stInput[gmvb_MainChannel].Vtotal & 0x7FFE;
         else
            MsaParams_s.VTotal_16 = stInput[gmvb_MainChannel].Vtotal;
            
         MsaParams_s.VActiveWidth_16 = stInput[gmvb_MainChannel].Vactive;
	  }
      //#endif // (NON_SCALING_OUTPUT == 1)...(DEINTERLACED_OUTPUT == 1)


         /*********************************Setup VStart*******************************/
         /*********************************Setup VSyncWidth***************************/
         /*********************************Setup VSyncPol*****************************/
         MsaParams_s.VActiveStart_16 = stInput[gmvb_MainChannel].Vstart;
         W_Vperiod = gm_ReadRegWord(MIFM_VS_PERIOD);
         W_Vpulse = gm_ReadRegWord(MIFM_VS_PULSE);
         if (W_Vpulse > (W_Vperiod / 2))
            MsaParams_s.VSyncWidth_16.Value_bf15   =  W_Vperiod - W_Vpulse;
         else
            MsaParams_s.VSyncWidth_16.Value_bf15   = W_Vpulse;   
            
         MsaParams_s.VSyncWidth_16.Polarity_bf1 = (stInput[gmvb_MainChannel].ModeFlags & gmd_MODE_NEG_VSYNC)? 1:0;         


         /*********************************Setup MsaFlags for Hdmi Input**************/
#if (FEATURE_HDMI == ENABLE)      
         if ((GetChannelConnType(gmvb_MainChannel)==HDMI) && (gm_IsHdmiTiming(gB_CurrentInputConnector[gmvb_MainChannel])==TRUE))
         {
            
            MsaParams_s.MsaFlags_16.Msa_bfs.DpColorFormat_bf4 = gm_HdmiGetAviColorSpace(gB_CurrentInputConnector[gmvb_MainChannel]);

            #if (PASS_THRU_MODE == 0)
            if (MsaParams_s.MsaFlags_16.Msa_bfs.DpColorFormat_bf4 == gmd_CEA861_AVI_Y_YUV422)
            {
               #ifdef ST_4K2K_93xx_BOARD // 4k2k Athena_FE pass through mode not to change the color format
               if(GetChipID_4K2K() != CHIPID_4K2K_FE)
               #endif
               	MsaParams_s.MsaFlags_16.Msa_bfs.DpColorFormat_bf4 = gmd_CEA861_AVI_Y_YUV444;
            }
            #endif

            switch (gm_HdmiGetRGBQuanRange(gB_CurrentInputConnector[gmvb_MainChannel]))
            {
               case gmd_YCC_QUANTIZATION_LIMITED:
                  MsaParams_s.MsaFlags_16.Msa_bfs.DpColorFormat_bf4 |= BIT2;
                  break;
               case gmd_YCC_QUANTIZATION_UNKNOW:
                  if (stInput[gmvb_MainChannel].ColorDomain == Input_YUV_Reduce)
                     MsaParams_s.MsaFlags_16.Msa_bfs.DpColorFormat_bf4 |= BIT2;
                  break;
            }

            if (stInput[gmvb_MainChannel].ModeFlags & gmd_MODE_HDVIDEO)
            {
               MsaParams_s.MsaFlags_16.Msa_bfs.DpColorFormat_bf4 |= BIT3;
            }
            MsaParams_s.MsaFlags_16.Msa_bfs.DpClockType_bf1 = DP_MsaClockType_Async;

            MsaParams_s.MsaFlags_16.Msa_bfs.DpColorDepth_bf3 = gm_HdmiGetColorDepth(gB_CurrentInputConnector[gmvb_MainChannel]);
            if (MsaParams_s.MsaFlags_16.Msa_bfs.DpColorDepth_bf3!=0)
               MsaParams_s.MsaFlags_16.Msa_bfs.DpColorDepth_bf3--;

            MsaParams_s.MsaFlags_16.Msa_bfs.DpVTotalEvenOdd_bf1 = DP_MsaVTotalOddEven_Odd;
            MsaParams_s.MsaFlags_16.Msa_bfs.DpStereoMode_bf2 = DP_MsaStereoModeType_No3d;
            MsaParams_s.MsaFlags_16.Msa_bfs.DpMsaColorMode_bf1 = DP_MsaColorMode_All;

            if(gm_IsHdmi3DTiming(gB_CurrentInputConnector[gmvb_MainChannel])==TRUE)
            {
               gmt_3D_INPUT_FORMAT_TYPE Format3D=gm_HdmiGet3DFormat(gB_CurrentInputConnector[gmvb_MainChannel]);

               MsaParams_s.MsaFlags_16.Msa_bfs.DpStereoMode_bf2 = DP_MsaStereoModeType_Right;
               ThreeDView= STDP_3D_RIGHT_VIEW;

               switch(Format3D)
               {
                  case gmd_3D_IN_FRAME_PACKING:
                     ThreeDFormat=STDP_3D_FORMAT_STACKED_FRAME;
                     break;

                  case  gmd_3D_IN_LINE_ALTERNATIVE:
                     ThreeDFormat=STDP_3D_FORMAT_INTERLEAVED_RIGHT_EVEN;
                     break;

                  case  gmd_3D_IN_SIDE_BY_SIDE_FULL:
                  case  gmd_3D_IN_SIDE_BY_SIDE_HALF:                  
                     ThreeDFormat=STDP_3D_FORMAT_SBS_RHALF_RIGHT;
                     break;

                  case  gmd_3D_IN_FIELD_ALTERNATIVE:
                  case  gmd_3D_IN_TOP_AND_BOTTOM:
                  default:
                     //  DP not supported
                     MsaParams_s.MsaFlags_16.Msa_bfs.DpStereoMode_bf2 = DP_MsaStereoModeType_No3d;
                     break;                  
               }
            }
         }
         else
#endif //(FEATURE_HDMI == ENABLE) 
         /*********************************Setup MsaFlags for Non-Hdmi/Dp Inputs*********/
         {

            MsaParams_s.MsaFlags_16.Msa_bfs.DpColorFormat_bf4 = 0;
            if (stInput[gmvb_MainChannel].ColorDomain == Input_YUV_Reduce)
            {
               MsaParams_s.MsaFlags_16.Msa_bfs.DpColorFormat_bf4 = BIT2|BIT1;
               if (stInput[gmvb_MainChannel].ModeFlags & gmd_MODE_HDVIDEO)
                  MsaParams_s.MsaFlags_16.Msa_bfs.DpColorFormat_bf4 |= BIT3;
            }
            //MsaParams_s.MsaFlags_16.Msa_bfs.DpColorFormat_bf4 = DP_MsaColorFormat_RGB;
            MsaParams_s.MsaFlags_16.Msa_bfs.DpClockType_bf1 = DP_MsaClockType_Async;
            MsaParams_s.MsaFlags_16.Msa_bfs.DpColorDepth_bf3 = DP_MsaColorDepth_8Bits;
            MsaParams_s.MsaFlags_16.Msa_bfs.DpVTotalEvenOdd_bf1 = DP_MsaVTotalOddEven_Odd;
            MsaParams_s.MsaFlags_16.Msa_bfs.DpStereoMode_bf2 = DP_MsaStereoModeType_No3d;
            MsaParams_s.MsaFlags_16.Msa_bfs.DpMsaColorMode_bf1 = DP_MsaColorMode_All;
         }
      }
      
      // Force color depth=10bits if input color depth > 10bits.  (ODP can only output upto 10bits)
      if (MsaParams_s.MsaFlags_16.Msa_bfs.DpColorDepth_bf3 > DP_MsaColorDepth_10Bits)
         MsaParams_s.MsaFlags_16.Msa_bfs.DpColorDepth_bf3 = DP_MsaColorDepth_10Bits;
         
   }// (TimingMethod != DPTX_TIMING_BY_INPUT)
   else
   {
      msg("DpTxEnableVideo- Set Timing by Output",0);
      MsaParams_s.PixelClockHz_32                     = PanelMaxPClk*1000;
      MsaParams_s.HTotal_16                           = PanelTypHTotal;
      MsaParams_s.HActiveStart_16                     = PanelMinHSyncWidth+PanelMinHSyncBackPorch;
      #ifdef ST_4K2K_93xx_BOARD
      if((gm_IsModeComplete(gmvb_MainChannel)) && (GetChipID_4K2K() == CHIPID_4K2K_FE))
      {         
         MsaParams_s.HActiveWidth_16                     = gmvw_InputHActiveOrg[gmvb_MainChannel];
      }
      else
      #endif
      MsaParams_s.HActiveWidth_16                     = PanelWidth;
      MsaParams_s.HSyncWidth_16.Value_bf15            = PanelMinHSyncWidth;
      MsaParams_s.VTotal_16                           = PanelTypVTotal;
      MsaParams_s.VActiveStart_16                     = PanelMinVSyncWidth+PanelMinVSyncBackPorch;
      #ifdef ST_4K2K_93xx_BOARD
      if((gm_IsModeComplete(gmvb_MainChannel)) && (GetChipID_4K2K() == CHIPID_4K2K_FE))
      {
         if(gm_IsInterlaced(gmvb_MainChannel))
            MsaParams_s.VActiveWidth_16                     = gmvw_InputVActiveOrg[gmvb_MainChannel]*2;
         else
         	MsaParams_s.VActiveWidth_16                     = gmvw_InputVActiveOrg[gmvb_MainChannel];         
      }
      else
      #endif
      MsaParams_s.VActiveWidth_16                     = PanelHeight;
      MsaParams_s.VSyncWidth_16.Value_bf15            = PanelMinVSyncWidth;

      MsaParams_s.MsaVideoMode_e                      = DP_MsaVideoMode_Progressivel;
      MsaParams_s.MsaFlags_16.Msa_bfs.DpColorFormat_bf4 = DP_MsaColorFormat_RGB;
      MsaParams_s.MsaFlags_16.Msa_bfs.DpClockType_bf1 = DP_MsaClockType_Async;
      #ifdef ST_4K2K_93xx_BOARD
      if(GetChipID_4K2K() == CHIPID_4K2K_FE)
      {
         MsaParams_s.MsaFlags_16.Msa_bfs.DpColorDepth_bf3 = gm_HdmiGetColorDepth(gB_CurrentInputConnector[gmvb_MainChannel]);
         
         if (MsaParams_s.MsaFlags_16.Msa_bfs.DpColorDepth_bf3!=0)
            MsaParams_s.MsaFlags_16.Msa_bfs.DpColorDepth_bf3--;      
         
         // Force color depth=10bits if input color depth > 10bits.  (ODP can only output upto 10bits)
         if (MsaParams_s.MsaFlags_16.Msa_bfs.DpColorDepth_bf3 > DP_MsaColorDepth_10Bits)
            MsaParams_s.MsaFlags_16.Msa_bfs.DpColorDepth_bf3 = DP_MsaColorDepth_10Bits;         
      }
      else
      #endif
      MsaParams_s.MsaFlags_16.Msa_bfs.DpColorDepth_bf3 = DP_MsaColorDepth_8Bits;
      MsaParams_s.MsaFlags_16.Msa_bfs.DpVTotalEvenOdd_bf1 = DP_MsaVTotalOddEven_Odd;
      MsaParams_s.MsaFlags_16.Msa_bfs.DpStereoMode_bf2 = DP_MsaStereoModeType_No3d;
      MsaParams_s.MsaFlags_16.Msa_bfs.DpMsaColorMode_bf1 = DP_MsaColorMode_All;
   }


   msg("PixelClockHz_32 = %Ld", MsaParams_s.PixelClockHz_32);
   msg("HTotal_16       = %d" , MsaParams_s.HTotal_16);
   msg("HActiveStart_16 = %d" , MsaParams_s.HActiveStart_16);
   msg("HActiveWidth_16 = %d" , MsaParams_s.HActiveWidth_16);
   msg("HSyncWidth_16   = %d" , MsaParams_s.HSyncWidth_16.Value_bf15);
   msg("VTotal_16       = %d" , MsaParams_s.VTotal_16);
   msg("VActiveStart_16 = %d" , MsaParams_s.VActiveStart_16);
   msg("VActiveWidth_16 = %d" , MsaParams_s.VActiveWidth_16);
   msg("VSyncWidth_16   = %d" , MsaParams_s.VSyncWidth_16.Value_bf15);
   msg("MsaFlags_16     = 0x%x" , MsaParams_s.MsaFlags_16.Msa_16);

   DPTX_SetVideoParameters(iDpTxPort, &MsaParams_s);
   if(MsaParams_s.MsaFlags_16.Msa_bfs.DpStereoMode_bf2 != DP_MsaStereoModeType_No3d)
   {
      DP_SendVSCPacket(iDpTxPort, ThreeDFormat);
      DP_SetLeftRight(iDpTxPort, ThreeDView);
      msg("ThreeDFormat=%d",ThreeDFormat);
      msg("ThreeDView=%d",ThreeDView);
   }
   DPTX_EnableVideo(iDpTxPort);
#else
   UNUSED_VAR(TimingMethod);
#endif
}

void DpTxDisableVideo(void)
{
#if DP_NUM_SST_ONLY_TX != 0
#ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()==CHIPID_4K2K_FE)
#endif
      DPTX_DisableVideo(iDpTxPort);
#endif
}




void DpTxEnableAudio(DP_AudioConfig_t *AudioConfig_ps)
{
#if DP_NUM_SST_ONLY_TX != 0
#ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()==CHIPID_4K2K_FE)
#endif
   {

      #if 0
      AudioConfig_ps->ChannelCount_8 = 2;
      AudioConfig_ps->ChannelAllocation_8 = 0;
      AudioConfig_ps->CodingType_e = DP_AudioCodingType_PCM;
      AudioConfig_ps->SamplingFrequency_e = DP_AudioSamplingFrequency_48_Khz;
      AudioConfig_ps->SampleSize_e = DP_AudioSampleSize_16Bits;
      AudioConfig_ps->AudioPort_e = DP_AudioPort_I2s;
      AudioConfig_ps->AudioStreamType_e = DP_AudioStreamType_Iec;
      AudioConfig_ps->AudioType_e = DP_AudioType_LPCM;
      AudioConfig_ps->AudioDacsUsed_8 = 0;
      #endif
      
      #if  (PATCH_DPTX_AUD_EN_WAIT_FOR_PORT_ACTIVE!=0)
      if(GetCurrentDPTXPort()==iDpTxPort)
      {
         if(DP_IsTxActive(iDpTxPort)==TRUE)
         {
            DPTX_SetAudioConfig(iDpTxPort, AudioConfig_ps);
            DPTX_EnableAudio(iDpTxPort);
            DPTX_MuteAudio(iDpTxPort, FALSE);

            #if (PATCH_DPTX_NO_AUD_BY_ZERO_OF_SDP_AUD_M_MEAS !=0)
            gm_TimerStart(DPTX_AUD_MONITOR_10ms_TMR, 100);
            #endif
            //DptxAudioEnFlag&=(~(DpTxAudioIsWaitingToBeEnabled); 
            DptxAudioEnFlag=DpTxAudioEnFlagNone;
            msg(" ^^DpTxEnableAudio",0);
         }
         else
         {
            DptxAudioEnFlag=DpTxAudioIsWaitingToBeEnabled;
            _fmemcpy((BYTE *)(&DpTxAudioConfig), (BYTE *)AudioConfig_ps, sizeof(DpTxAudioConfig));         
            msg(" @1@ DpTxAudioIsWaitingToBeEnabled(%Ld)",gm_GetSystemTime());
         }
      }
      #else
      DPTX_SetAudioConfig(iDpTxPort, AudioConfig_ps);
      DPTX_EnableAudio(iDpTxPort);
      DPTX_MuteAudio(iDpTxPort, FALSE);
      #endif

   
   }
#else
   UNUSED_PARAMETER(AudioConfig_ps);
#endif
}



void DpTxDisableAudio(void)
{
#if DP_NUM_SST_ONLY_TX != 0
#ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()==CHIPID_4K2K_FE)
#endif
   {
      if(GetCurrentDPTXPort()==iDpTxPort)
      {   
         DPTX_DisableAudio(iDpTxPort);
         #if PATCH_DPTX_AUD_EN_WAIT_FOR_PORT_ACTIVE!=0 
         DptxAudioEnFlag=DpTxAudioEnFlagNone;
         //gm_Print(" -4",0);
         #endif 
      }
   }
#endif
}

void DpTxMuteAudio(BYTE b_MuteFlag)
{
#if DP_NUM_SST_ONLY_TX != 0
   #ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()==CHIPID_4K2K_FE)
   #endif
   {
      #if  (PATCH_DPTX_AUD_EN_WAIT_FOR_PORT_ACTIVE!=0)
      if(GetCurrentDPTXPort()==iDpTxPort)
      {
         if(DP_IsTxActive(iDpTxPort)==TRUE)
         {
            DPTX_MuteAudio(iDpTxPort, b_MuteFlag); 
            #if (PATCH_DPTX_NO_AUD_BY_ZERO_OF_SDP_AUD_M_MEAS !=0)
            if(b_MuteFlag==TRUE)
               gm_TimerStart(DPTX_AUD_MONITOR_10ms_TMR, 100);
            #endif
            
            DptxAudioEnFlag&=(~(DpTxAudioIsWaitingToBeUnMuted|DpTxAudioIsWaitingToBeMuted));  
            msg(" ^^DpTxMuteAudio=%d",b_MuteFlag);
         }
         else
         {
            if(b_MuteFlag==TRUE)
            {
               DptxAudioEnFlag&=(~DpTxAudioIsWaitingToBeUnMuted);         
               DptxAudioEnFlag|=DpTxAudioIsWaitingToBeMuted;
               msg(" @2@ DpTxAudioIsWaitingToBeMuted(%Ld)",gm_GetSystemTime());
            }
            else
            {
               DptxAudioEnFlag&=(~DpTxAudioIsWaitingToBeMuted);
               DptxAudioEnFlag|=DpTxAudioIsWaitingToBeUnMuted;
               msg(" @4@ DpTxAudioIsWaitingToBeUnMuted(%Ld)",gm_GetSystemTime());
            }
         }
      }
      #else
      DPTX_MuteAudio(iDpTxPort, b_MuteFlag);    
      #endif
   }
#endif
b_MuteFlag = b_MuteFlag;

}



#if 1
void DpRxUpdaateVddsInitFreq(void)
{
#if DP_NUM_SST_ONLY_RX != 0
   if (gm_GetDpDigitalID(UserPrefCurrentInputMain)==DID_DPRX1)
   {
      DWORD DW_Freq=0;
      BYTE far *W_Ptr;
      W_Ptr=LINEAR_TO_FPTR_B(0x1B100);
      // 1. HBR2 - B6DB6D
      // 2. HBR   - 5B6DB6
      // 3. RBR   - 36DB6D
      if (W_Ptr[0] == 0x14)
         DW_Freq = 0xB6DB6D;
      else if (W_Ptr[0] == 0x0A)
         DW_Freq = 0x5B6DB6;
      else if (W_Ptr[0] == 0x06)
         DW_Freq = 0x36DB6D;
      if (DW_Freq != 0 && gm_ReadRegDWord_LE(AVDDS1_INI_FREQ) != DW_Freq)
      {
         //gm_Print("Org AVDDS1_INI_FREQ as 0x%X%X",(WORD)(gm_ReadRegDWord_LE(AVDDS1_INI_FREQ)>>16),(WORD)(gm_ReadRegDWord_LE(AVDDS1_INI_FREQ)&0xFFFFUL));
         #if (FEATURE_AUDIO == ENABLE)
         gm_WriteRegDWord_LE(AVDDS3_INI_FREQ, DW_Freq);
         #endif
         gm_WriteRegDWord_LE(AVDDS1_INI_FREQ, DW_Freq);
         gm_WriteRegDWord_LE(AVDDS1_INIT, 0x11);
         //gm_Print("Update AVDDS1_INI_FREQ as 0x%X%X",(WORD)(DW_Freq>>16),(WORD)(DW_Freq&0xFFFFUL));
      }
   }

   if (gm_GetDpDigitalID(UserPrefCurrentInputPip)==DID_DPRX1)
   {
      DWORD DW_Freq=0;
      BYTE far *W_Ptr;
      W_Ptr=LINEAR_TO_FPTR_B(0x1B100);
      // 1. HBR2 - B6DB6D
      // 2. HBR   - 5B6DB6
      // 3. RBR   - 36DB6D
      if (W_Ptr[0] == 0x14)
         DW_Freq = 0xB6DB6D;
      else if (W_Ptr[0] == 0x0A)
         DW_Freq = 0x5B6DB6;
      else if (W_Ptr[0] == 0x06)
         DW_Freq = 0x36DB6D;
      if (DW_Freq != 0 && gm_ReadRegDWord_LE(AVDDS2_INI_FREQ) != DW_Freq)
      {
         #if (FEATURE_AUDIO == ENABLE)
         gm_WriteRegDWord_LE(AVDDS4_INI_FREQ, DW_Freq);
         #endif
         gm_WriteRegDWord_LE(AVDDS2_INI_FREQ, DW_Freq);
         gm_WriteRegDWord_LE(AVDDS2_INIT, 0x11);
         msg_ii("Update AVDDS2_INI_FREQ as 0x%X%X",(WORD)(DW_Freq>>16),(WORD)(DW_Freq&0xFFFFUL));
      }
   }
#endif //#if DP_NUM_SST_ONLY_RX != 0
}

#else

void DpRxUpdaateVddsInitFreq(void)
{
   if (GetPortProc(UserPrefCurrentInputMain) == DP_PORT)
   {
      DWORD DW_Freq=0;
      BYTE far *W_Ptr;
      if (gm_GetDpDigitalID(UserPrefCurrentInputMain)==DID_DPRX0)
         W_Ptr=LINEAR_TO_FPTR_B(0x1F100);
      else
         W_Ptr=LINEAR_TO_FPTR_B(0x1B100);
      // 1. HBR2 - B6DB6D
      // 2. HBR   - 5B6DB6
      // 3. RBR   - 36DB6D
      if (W_Ptr[0] == 0x14)
         DW_Freq = 0xB6DB6D;
      else if (W_Ptr[0] == 0x0A)
         DW_Freq = 0x5B6DB6;
      else if (W_Ptr[0] == 0x06)
         DW_Freq = 0x36DB6D;
      if (DW_Freq != 0 && gm_ReadRegDWord_LE(AVDDS1_INI_FREQ) != DW_Freq)
      {
         //gm_Print("Org AVDDS1_INI_FREQ as 0x%X%X",(WORD)(gm_ReadRegDWord_LE(AVDDS1_INI_FREQ)>>16),(WORD)(gm_ReadRegDWord_LE(AVDDS1_INI_FREQ)&0xFFFFUL));
#if (FEATURE_AUDIO == ENABLE)
         gm_WriteRegDWord_LE(AVDDS3_INI_FREQ, DW_Freq);
#endif
         gm_WriteRegDWord_LE(AVDDS1_INI_FREQ, DW_Freq);
         gm_WriteRegDWord_LE(AVDDS1_INIT, 0x11);
         //gm_Print("Update AVDDS1_INI_FREQ as 0x%X%X",(WORD)(DW_Freq>>16),(WORD)(DW_Freq&0xFFFFUL));
      }
   }

   if (GetPortProc(UserPrefCurrentInputPip) == DP_PORT)
   {
      DWORD DW_Freq=0;
      BYTE far *W_Ptr;
      if (gm_GetDpDigitalID(UserPrefCurrentInputPip)==DID_DPRX0)
         W_Ptr=LINEAR_TO_FPTR_B(0x1F100);
      else
         W_Ptr=LINEAR_TO_FPTR_B(0x1B100);
      // 1. HBR2 - B6DB6D
      // 2. HBR   - 5B6DB6
      // 3. RBR   - 36DB6D
      if (W_Ptr[0] == 0x14)
         DW_Freq = 0xB6DB6D;
      else if (W_Ptr[0] == 0x0A)
         DW_Freq = 0x5B6DB6;
      else if (W_Ptr[0] == 0x06)
         DW_Freq = 0x36DB6D;
      if (DW_Freq != 0 && gm_ReadRegDWord_LE(AVDDS2_INI_FREQ) != DW_Freq)
      {
#if (FEATURE_AUDIO == ENABLE)
         gm_WriteRegDWord_LE(AVDDS4_INI_FREQ, DW_Freq);
#endif
         gm_WriteRegDWord_LE(AVDDS2_INI_FREQ, DW_Freq);
         gm_WriteRegDWord_LE(AVDDS2_INIT, 0x11);
         msg_ii("Update AVDDS2_INI_FREQ as 0x%X%X",(WORD)(DW_Freq>>16),(WORD)(DW_Freq&0xFFFFUL));
      }
   }

}

#endif

#if (ENABLE_DPCD_SINK_TEST_SLEEP_DELAY != 0)
#define DPRX0_DPCD_TEST_SINK           0x1F270
#define DPRX1_DPCD_TEST_SINK           0x1B270
#define DPRX_DPCD_TEST_SINK_START     BIT0
static void DpcdTestSinkMonitor(BYTE InputConnector)
{
   BYTE far *W_Ptr;
   gmt_DPRX_DID b_DID=gm_GetDpDigitalID(InputConnector);
   if(b_DID==DID_DPRX0)
      W_Ptr=LINEAR_TO_FPTR_B(DPRX0_DPCD_TEST_SINK);
   else if (b_DID==DID_DPRX1)
      W_Ptr=LINEAR_TO_FPTR_B(DPRX1_DPCD_TEST_SINK);
   else
      return;
      
   if (W_Ptr[0] & DPRX_DPCD_TEST_SINK_START)
      gm_TimerStart(DPCD_SINK_TEST_1s_TMR,SINK_TEST_SLEEP_DELAY_x1s);
   return;
}
#endif

void DpMstRxPowerManagementBeforeLT(void)
{
#if (PATCH_OF_DPCD600_2_RE_LT == 1)
	U8	 DpStatus_8  = 0;
	U16 LinkStatus_16 = 0;
  	BYTE far *W_Ptr;	

	DpStatus_8 = gm_ReadRegByte(DP12RX0_IRQ_STATUS);

	if (DpStatus_8 & DPRX_PHY_IRQ_STS)
	{
	 LinkStatus_16 = gm_ReadRegWord(DP12RX0_LINK_IRQ_STS);			 
	 LinkStatus_16 &= gm_ReadRegWord(DP12RX0_LINK_IRQ_CTRL);
	 
	 if (LinkStatus_16 & DPRX_LT_CYCLE_END_IRQ_STS)
	 {
		 W_Ptr = LINEAR_TO_FPTR_B(0x1F600);
		 
   		 if (W_Ptr[0] == 0x02)
		 	*LINEAR_TO_FPTR_B(0x1F600) = 0x01;
	 }
	}
#endif
}

#if (PCLK_VALUE_FROM_AVDDS!=0)
DWORD DpRx_UpdatePixelClkExt(BYTE PortId)
{
   WORD reg_AVDDS_CUR_FREQ;
   DWORD dw_Avdds;
#if RCLK_FREQ_ARGS == RCLKSEL_432M
    WORD Factor=1648;                  //Factor= (RCLK/2^24)<<6
#elif RCLK_FREQ_ARGS == RCLKSEL_378M
    WORD Factor=1442;                  
#elif RCLK_FREQ_ARGS == RCLKSEL_270M
    WORD Factor=1030; 
#endif

   if(DpRxGetPortId(gB_CurrentInputConnector[gmvb_MainChannel])==PortId)
      reg_AVDDS_CUR_FREQ=AVDDS1_CUR_FREQ;
   else if(DpRxGetPortId(gB_CurrentInputConnector[gmvb_PipChannel])==PortId)
      reg_AVDDS_CUR_FREQ=AVDDS2_CUR_FREQ;      
   else
      return 0;
      
   gm_WriteRegDWord_LE(reg_AVDDS_CUR_FREQ,0xFFFFFFFFUL);
   dw_Avdds=gm_ReadRegDWord_LE(reg_AVDDS_CUR_FREQ)>>6;
   return (dw_Avdds*Factor);
}
#endif

#if (DEBUG_DP_PRINT_TIMING_DISPLAY_TIME!=0)
void SetDpDebugDisplayTime(void)
{
   gm_Print("*UdDisT",0);
   b_StablePrnt=TRUE;
   dw_DebugDisplayTimeDp=gm_GetSystemTime();
}

DWORD GetDpDebugDisplayTime(void)
{
   return dw_DebugDisplayTimeDp;
}
#endif

/*------------------------------------------------------------
FUNCTION: 
SetVoltageLvl_cb

DESCRIPTION:
Sets the voltage level for powering the upstream device

INPUT PARAMETERS:
DPRX_VoltageLvl_t	: Voltage level to set

RETURN VALUE:
Boolean
TRUE	:	Voltage level was set
FALSE	: 	Voltage level was not set
------------------------------------------------------------*/
BOOL far SetVoltageLvl_cb(DPRX_VoltageLvl_t VoltLvl_t)
{
	//This function is to be filled in by Application Developers
	gm_Print("Voltage Level to set=%x",VoltLvl_t);

	if ( (VoltLvl_t != DP_VOLTAGELVL_NONE) && (VoltLvl_t > INIT_VOLTLVL) )
	{
		//Expects more than what we can provide. Return
		gm_Print("ERROR! More V than capacity",0);
		return FALSE;
	}
	
	return TRUE;
}

#ifdef ST_4K2K_93xx_BOARD
#ifdef DP_HUB_2_LOG_PORTS
#if (ENABLE_DPRX0_RE_MODE_SETUP)
void AthenaR_L_Redo_ModeSetup(void)
{
#if (FEATURE_DUM == ENABLE)

#ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()==CHIPID_4K2K_R)
   {
#if ENABLE_SOC_IPC_BUSY_CHECK
	/*ChengLin 20130124: New 4k2k board BUSY status check*/
		if(!SocIPCIsSlaveLeftReady())
			return; 
#endif
      SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_DPRx0_ReModeSetup,NULL);   
   }
#endif
   ForceDP_ReModeSetup = TRUE;
#endif
}

void DP_Two_LogicalPortTimingChangeHandler(void)
{
   static BYTE PreStatus = DEFAUL_NUM_VC_ALLOCATED;
   
   if((gm_TimingIsStable(gmvb_MainChannel) == TRUE) && (gm_GetDpDigitalID(gB_CurrentInputConnector[gmvb_MainChannel])==DID_DPRX0))
   {
      if(gm_ReadRegWord(DP12RX_RL_CONFIG) & DPRX_TYPE)
      {
         if(PreStatus!= DP12_GetNumberVirtualChannelsAllocated(MstRxPort))
         {
            if(gm_IsModeComplete(gmvb_MainChannel))
            {
               Two_LogicalPort_msg("\nAthena R Detected Virtual Channel Number Change...\n",0); 
               AthenaR_L_Redo_ModeSetup();
            }
         PreStatus = DP12_GetNumberVirtualChannelsAllocated(MstRxPort);
         }

      }
   }
}
#endif
#endif
#endif

#endif //(FEATURE_DISPLAY_PORT == ENABLE)



