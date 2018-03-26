/*
    $Workfile:   HdmiApp.c  $
    $Revision: 1.138 $
    $Date: 2013/05/24 08:04:37 $
*/
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
// MODULE:      HdmiApp.c
//
// USAGE:       This module contains application level support function for
//                internal HDMI block
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#include "System\All.h"
#include "mem.h"


#if (FEATURE_HDMI==ENABLE)
//******************************************************************************
//  D E B U G
//******************************************************************************
//#define _DEBUG_HDMI_MSG
#ifdef _DEBUG_HDMI_MSG
#define hdmi_msg(x,y)        gm_Print(x,y)
#define hdmi_msg_ii(x,y,z)        gm_Print(x,y,z)
#else
#define hdmi_msg(x,y)
#define hdmi_msg_ii(x,y,z)
#endif
//******************************************************************************
//  E X T E R N A L    F U N C T I O N S
//******************************************************************************
// For LOAD_HDCPKEY_FROM_CODE_FOR_TEST define
void LoadHdcpKeyArray(BYTE far *Bp_RxArrayAddr, BYTE far *Bp_TxArrayAddr);

//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************
#if (FEATURE_HDCP_REPEATER == ENABLE)
void  far STHDMIRX_HdcpRptrEntry(U32 InputHandle,
										HdcpRepeaterRxRequest_t	RxRequest_e,
										U8*							RxRequestData_pv);
#endif
//******************************************************************************
//  D E F I N E S
//******************************************************************************
#define CPHY_AUX_PHY_CTRL                              0x0000 //33536 (CPHY_PHYA)
#define CPHY_PHY_CTRL                                  0x000C //33548 (CPHY_PHYA)
#define CPHY_L0_CTRL_0                                 0x0010 //33552 (CPHY_PHYA)
#define CPHY_L0_CTRL_4                                 0x0014 //33556 (CPHY_PHYA)
#define CPHY_L0_CPTRIM                                 0x0018 //33560 (CPHY_PHYA)
#define CPHY_L1_CTRL_0                                 0x001C //33564 (CPHY_PHYA)
#define CPHY_L1_CTRL_4                                 0x0020 //33568 (CPHY_PHYA)
#define CPHY_L1_CPTRIM                                 0x0024 //33572 (CPHY_PHYA)
#define CPHY_L2_CTRL_0                                 0x0028 //33576 (CPHY_PHYA)
#define CPHY_L2_CTRL_4                                 0x002C //33580 (CPHY_PHYA)
#define CPHY_L2_CPTRIM                                 0x0030 //33584 (CPHY_PHYA)
#define CPHY_L3_CTRL_0                                 0x0034 //33588 (CPHY_PHYA)
#define CPHY_L3_CTRL_4                                 0x0038 //33592 (CPHY_PHYA)
#define CPHY_L3_CPTRIM                                 0x003C //33596 (CPHY_PHYA)
#define CPHY_RCAL_RESULT                               0x0040 //33600 (CPHY_PHYA)
#define CPHY_ALT_FREQ_MEAS_CTRL                        0x0044 //33604 (CPHY_PHYA)
#define CPHY_ALT_FREQ_MEAS_RESULT                      0x0048 //33608 (CPHY_PHYA)
#define CPHY_TEST_CTRL                                 0x004C //33612 (CPHY_PHYA)
#define CPHY_BIST_CONTROL                              0x0050 //33616 (CPHY_PHYA)
#define CPHY_BIST_PATTERN0                             0x0054 //33620 (CPHY_PHYA)
#define CPHY_BIST_PATTERN1                             0x0058 //33624 (CPHY_PHYA)
#define CPHY_BIST_PATTERN2                             0x005C //33628 (CPHY_PHYA)
#define CPHY_BIST_PATTERN3                             0x0060 //33632 (CPHY_PHYA)
#define CPHY_BIST_PATTERN4                             0x0064 //33636 (CPHY_PHYA)
#define CPHY_BIST_STATUS                               0x0068 //33640 (CPHY_PHYA)
#define CPHY_RSVRBITS                                  0x006C //33644 (CPHY_PHYA)
#define CPHY_BIST_TMDS_CRC_UPDATE                      0x0070 //33648 (CPHY_PHYA)
#define CPHY_JE_CTRL                                   0x0080 //33664 (CPHY_PHY_JE)
#define CPHY_JE_CONFIG                                 0x0084 //33668 (CPHY_PHY_JE)
#define CPHY_JE                                        0x0088 //33672 (CPHY_PHY_JE)
#define CPHY_JE_TST_THRESHOLD                          0x008C //33676 (CPHY_PHY_JE)
#define CPHY_JE_STATUS                                 0x0090 //33680 (CPHY_PHY_JE)

#define HDMIDVI_RX_PHY_CTRL                            0x0048	//36936 (C9HDRX_HDMI_RX_PHY_DG)
#define HDRX_IFM_INSTRUMENT_PU_ADDRS_OFFSET            0x0500   
#define HDMI_RX_INSTRUMENT_POLARITY_CONTROL            0x0028
#define HDMI_RX_INPUT_IRQ_STATUS                       0x0018
#define HDMI_RX_INPUT_IBD_HTOTAL                       0x00F8
#define HDMI_RX_INPUT_IBD_VTOTAL                       0x00FC
#define HDMI_RX_INPUT_IBD_HSTART                       0x0100 
#define HDMI_RX_INPUT_IBD_HWIDTH                       0x0104
#define HDMI_RX_INPUT_IBD_VSTART                       0x0108 
#define HDMI_RX_INPUT_IBD_VLENGTH                      0x010C
#define HDMI_RX_INPUT_IRQ_STATUS                       0x0018
#define HDMI_RX_MEAS_IRQ_STS                           0x01A0
#define HDMI_RX_SIGNL_DET_CTRL                         0x0184
#define HDMI_RX_MEAS_RESULT                            0x0198
#define DVI_MEASUREMENTS_PERIOD                        189    
#define DVI_CLOCK_THRESH                               22
#define DVIB_OFFSET                                    0x700
#define HDMI_RX_HDCP_ADDR                              0x0444
#define HDMI_RX_AFR_CONTROL 				                0x000C
#define HDMI_RX_AFB_CONTROL 				                0x0010
//#define CPHY_L0_IB_CNTL_SHIFT                          6
#define HDMI_RX_PHY_EQ_PEAK                            0x700UL
#define HDMI_RX_PHY_EQ_PEAK_SHIFT                      8
#define HDMI_RX_PHY_EQ_GAIN                            0x07
#define HDMI_RX_PHY_EQ_GAIN_SHIFT                      0
#define HDMIRX_PHY_CTRL                                0x00048
#define HDMIRX_PHY_STATUS                              0x0004C
#define HDMIRX_PHY_PHS_PICK                            0x00050
#define HDMIRX_PHY_SIGQUAL                             0x00054
#define HDMIRX_PHY_PHS_A_SCR                           0x00058
#define HDMIRX_PHY_PHS_B_SCR                           0x0005C
#define HDMIRX_PHY_PHS_C_SCR                           0x00060
#define HDMIRX_LINK_ERR_CTRL                           0x017C	
#define HDMIRX_PHY_DEC_ERR_STATUS                      0x0180	
#define HDMIRX_SDP_AV_MUTE_CTRL                        0x0264  
#define HDMIRX_PATGEN_CONTROL                          0x0228  
#define HDMIRX_HDCP_STATUS                             0x0454  
#define HDMIRX_INST_SOFT_RESETS        			       0x0000

#define IMP_SYNC_STATUS_MASK (IMP_NO_HS|IMP_NO_VS)
#define IPP_SYNC_STATUS_MASK (IPP_NO_HS|IPP_NO_VS)

#define IMP_TIMING_STATUS_MASK (IMP_HS_PERIOD_ERR|IMP_VS_PERIOD_ERR)
#define IPP_TIMING_STATUS_MASK (IPP_HS_PERIOD_ERR|IPP_VS_PERIOD_ERR)

#define IMP_STATUS_MASK (IMP_SYNC_STATUS_MASK | IMP_TIMING_STATUS_MASK)
#define IPP_STATUS_MASK (IPP_SYNC_STATUS_MASK | IPP_TIMING_STATUS_MASK)

//#define LOAD_HDCPKEY_FROM_CODE_FOR_TEST
#define Patch_Of_RCVRDATA_POLARITY_in_Cut2   0        //Temporary, Need to move into library.
#if defined(RD3_93xx_BOARD) || defined(RD1_93xx_BOARD_REV_B) || defined(ST_4K2K_93xx_BOARD)
#define Default_Hpd_Polarity_Reversed        1
#else
#define Default_Hpd_Polarity_Reversed        1
#endif

#define PC_PortConfigPtr					ptr_PortConfig

#define PC_InputConnector					PC_PortConfigPtr->InputConnector
#define PC_DeviceName   					PC_PortConfigPtr->DeviceName
#define PC_DevHandle						   PC_PortConfigPtr->DevHandle
#define PC_InitParams						PC_PortConfigPtr->InitParams
#define PC_OpenParams						PC_PortConfigPtr->OpenParams
#define PC_TermParams						PC_PortConfigPtr->TermParams
#define PC_InputConfig						PC_PortConfigPtr->InputConfig
#define PC_InputHandle						PC_PortConfigPtr->InputHandle
#define PC_Capability						PC_PortConfigPtr->Capability
#define PC_SignalStatusEvtData			PC_PortConfigPtr->SignalStatusEvtData
#define PC_VideoPropertyEvtData			PC_PortConfigPtr->VideoPropertyEvtData
#define PC_AudioPropertyEvtData			PC_PortConfigPtr->AudioPropertyEvtData
#define PC_InfoUpdateEvtData				PC_PortConfigPtr->InfoUpdateEvtData
#define PC_Event							   PC_PortConfigPtr->Event
#define PC_Error							   PC_PortConfigPtr->Error
#define PC_LinkClockCnt                PC_PortConfigPtr->LinkClockCnt
#define PC_LinkClockCntIndex           PC_PortConfigPtr->LinkClockCntIndex
#define PC_IsActivePort						PC_PortConfigPtr->AppStatus.IsActivePort
#define PC_SignalDataLoaded				PC_PortConfigPtr->AppStatus.SignalDataLoaded
#define PC_VideoDataLoaded					PC_PortConfigPtr->AppStatus.VideoDataLoaded
#define PC_AudioDataLoaded					PC_PortConfigPtr->AppStatus.AudioDataLoaded
#define PC_PreAVMuteStatus             PC_PortConfigPtr->AppStatus.PreAVMuteStatus
#define PC_ReDoModeSetup               PC_PortConfigPtr->AppStatus.ReDoModeSetup
#define PC_EnableHpdInPortInit         PC_PortConfigPtr->AppStatus.EnableHpdInPortInit
#define PC_FirstModeSetupFB            PC_PortConfigPtr->AppStatus.FirstModeSetupFB
#define PC_PerformInstReset            PC_PortConfigPtr->AppStatus.PerformInstReset
#if (LINK_CLOCK_CHECK_SUPPORT==1)
#define PC_IsLinkClockPresented        PC_PortConfigPtr->AppStatus.IsLinkClockPresented
#define PC_LinkClockCnt                PC_PortConfigPtr->LinkClockCnt
#define PC_LinkClockCntIndex           PC_PortConfigPtr->LinkClockCntIndex
#endif


#if (ENABLE_MIN_PORT_OFF_LIMITATION == 1)
#define PC_PrevPortOffTime             PC_PortConfigPtr->PrevPortOffTime
#endif

#if (BACKUP_INFODATA == 1)
#define PC_VSInfo							   PC_PortConfigPtr->VSInfo
#define PC_SPDInfo							PC_PortConfigPtr->SPDInfo
#define PC_MPEGInfo							PC_PortConfigPtr->MPEGInfo
#define PC_ISRCInfo							PC_PortConfigPtr->ISRCInfo
#define PC_ACPInfo							PC_PortConfigPtr->ACPInfo
#define PC_GBDInfo							PC_PortConfigPtr->GBDInfo
#endif


typedef struct
{
   BYTE 							InputConnector;
   BOOL							IsActivated;
}gmt_HdmiHpdCtrl;

//******************************************************************************
//  S T A T I C    V A R I A B L E S
//******************************************************************************
static gmt_HdmiDviInputPort				HdmiDviInPortConfig[NUM_OF_HDMIDVI_INP];

#if (HPD_CONTROL_HANDLER_ENABLE==1)
static gmt_HdmiHpdCtrl					HdmiHpdControl[NUM_CHANNELS]={{0xFF, FALSE},{0xFF, FALSE}};
#endif

#if (ENABLE_DDVI_EXCEPTION_TABLE==1)
static ROM STHDMIRX_DDVIExceptionModeTable_t DDVI_ExceptTable[]=
{
   {2560,1470},
   {2560,1080},	
   {0   ,   0}       // Next Element must be zero to indicate "end of mode table"
};
#endif

#if (USE_MINI_ISR_HANDLER==1)
static BOOL b_Initialed=FALSE;
#endif

#if (DEBUG_HDMI_PRINT_TIMING_DISPLAY_TIME!=0)
static DWORD dw_DebugDisplayTime=0;
#endif


#if (IBD_CONSISTENCE_CHECK !=0)
static gmt_IbdRecheck IbdReCheck[CH_PHY_NUM];
#endif   


#ifdef LOAD_HDCPKEY_FROM_CODE_FOR_TEST
//Test key for test
static BYTE ROM BA_Hdcp_Keys[] =
{
   0xcd, 0x1a, 0xf2, 0x1e, 0x51,  // firt 5 entries is BKSV values.
   0x00, 0x23,	0xb3, 0x00, 0x00, 0x00, 0x00, // Key 1
   0xb4, 0x65, 0xed, 0xad, 0xa9, 0x86, 0x16, // Key 2
   0xa8, 0x6e, 0x1a, 0xaa, 0xbf, 0x27, 0xfa, // Key 3
   0x47, 0x39, 0x7b, 0x2f, 0x06, 0xea, 0x8d, // Key 4
   0x61, 0x09, 0x66, 0xe9, 0x22, 0x16, 0xa6, // Key 5
   0x14, 0x35, 0x88, 0x6c, 0xf5, 0x78, 0x13, // Key 6
   0xc1, 0x74, 0xf1, 0x28, 0xeb, 0x0f, 0x16, // Key 7
   0xa3, 0xfb, 0x53, 0x8e, 0x47, 0xa5, 0xf1, // Key 8
   0x4a, 0x29, 0xd7, 0x0d, 0x26, 0xed, 0xdf, // Key 9
   0xcb, 0x36, 0x4a, 0xe1, 0x9f, 0xff, 0x05, // Key 10
   0xc1, 0x9d, 0x0f, 0x8e, 0xe1, 0x1e, 0x71, // Key 11
   0x92, 0xf6, 0x5a, 0x86, 0xa8, 0xe9, 0xf3, // Key 12
   0x8d, 0xb4, 0x8c, 0xbb, 0x2d, 0x58, 0x3b, // Key 13
   0x2e, 0x9e, 0xe9, 0xf7, 0x8c, 0xcb, 0xcf, // Key 14
   0xb6, 0x76, 0x20, 0x80, 0xd0, 0xed, 0x01, // Key 15
   0x40, 0x80, 0x57, 0x53, 0xfb, 0xe5, 0x89, // Key 16
   0x3f, 0x00, 0xa8, 0xb1, 0x75, 0x00, 0x49, // Key 17
   0x7f, 0x34, 0xe4, 0xb8, 0xf5, 0xe8, 0x09, // Key 18
   0x5f, 0x52, 0x4d, 0xda, 0xb1, 0x74, 0xb8, // Key 19
   0x2e, 0x55, 0x83, 0x50, 0x63, 0x37, 0x32, // Key 20
   0xef, 0x89, 0x39, 0x54, 0xef, 0x65, 0x58, // Key 21
   0x57, 0xe3, 0xd1, 0xaa, 0xb1, 0x5c, 0xe1, // Key 22
   0xdd, 0x61, 0xa2, 0xc7, 0x8b, 0x9e, 0x3e, // Key 23
   0xb9, 0xf0, 0x16, 0xe0, 0x68, 0xef, 0x8c, // Key 24
   0x4d, 0x08, 0x3b, 0x8d, 0x2c, 0xcb, 0xb5, // Key 25
   0x55, 0xcc, 0x55, 0x3c, 0x76, 0x4e, 0x73, // Key 26
   0x29, 0x55, 0x24, 0x16, 0x31, 0xf9, 0xe6, // Key 27
   0xea, 0xeb, 0x53, 0x96, 0x53, 0x0f, 0x0f, // Key 28
   0xfe, 0x8d, 0x48, 0xbe, 0xba, 0x2b, 0xdf, // Key 29
   0xf4, 0x01, 0x28, 0x9e, 0xf3, 0x57, 0x93, // Key 30
   0xd6, 0x26, 0x7e, 0xe4, 0xf5, 0x98, 0x4f, // Key 31
   0x66, 0xa5, 0xfa, 0x26, 0xe9, 0xcf, 0x45, // Key 32
   0x9f, 0xfb, 0xdf, 0xa8, 0xd2, 0xdd, 0xce, // Key 33
   0x92, 0xcd, 0x21, 0xff, 0xe9, 0x92, 0x85, // Key 34
   0x77, 0x40, 0x41, 0x28, 0x5b, 0x14, 0xe5, // Key 35
   0x92, 0xc5, 0x1b, 0x7f, 0x15, 0x7b, 0x42, // Key 36
   0x01, 0x46, 0x03, 0x87, 0x70, 0x54, 0x7e, // Key 37
   0xb4, 0x70, 0x2c, 0xdd, 0x1b, 0xec, 0x91, // Key 38
   0x6d, 0x80, 0xfc, 0x45, 0xc7, 0xd4, 0x1f, // Key 39
   0x39, 0x2d, 0xc1, 0x18, 0x13, 0x9f, 0x7f, // Key 40
   0x35, 0xe2, 0xdf, 0x7a, 0x40, 0xbb, 0x40  // Key 41
};
#endif

#if (ENABLE_MIN_PORT_OFF_LIMITATION ==1)
static BYTE b_AcOnNoticed=FALSE;
#endif

#if (DUALDVI_HDCP_MODE_DET_DELAY==1)
typedef struct
{
   U8 Count    :7;
   U8 FlagSet  :1;
}ModeDetDelayCont_t;
static ModeDetDelayCont_t b_DualDviDelayCont={0,0};
#endif


//*****************************************************************************
//  S T A T I C    F U N C T I O N    P R O T O T Y P E S
//*****************************************************************************

#if (HPD_CONTROL_HANDLER_ENABLE==1)
static void HdmiHpdhandler(gmt_PHY_CH Channel,gmt_UserDefConnNames B_InputConnector);
#endif


//*****************************************************************************
//  C O D E
//*****************************************************************************
#if DEBUG_HDMI_CPHY_CAPTURE !=0
typedef struct
{
   DWORD dwTime;
   DWORD dwBuf[50];
}RdCphyBuf;

#define CATCH_TIME      20
#define CATCH_DURATION  1000UL  // 1 sec
static DWORD CatchPreTime;

static RdCphyBuf RdCphy[CATCH_TIME];
static BYTE idx=0;
static BYTE CaptureFlag=FALSE;

void CaptureCphyRegs(BYTE InputConnector)
{
/*
CPHY2_AUX_PHY_CTRL
CPHY2_PHY_CTRL    
CPHY2_L0_CTRL_0   
CPHY2_L0_CTRL_4   
CPHY2_L0_CPTRIM   
CPHY2_L1_CTRL_0   
CPHY2_L1_CTRL_4   
CPHY2_L1_CPTRIM   
CPHY2_L2_CTRL_0   
CPHY2_L2_CTRL_4   
CPHY2_L2_CPTRIM   
CPHY2_L3_CTRL_0   
CPHY2_L3_CTRL_4   
CPHY2_L3_CPTRIM   
CPHY2_RCAL_RESULT        
CPHY2_ALT_FREQ_MEAS_CTRL 
CPHY2_ALT_FREQ_MEAS_RESULT 
CPHY2_TEST_CTRL            
CPHY2_BIST_CONTROL         
CPHY2_BIST_PATTERN0        
CPHY2_BIST_PATTERN1        
CPHY2_BIST_PATTERN2        
CPHY2_BIST_PATTERN3        
CPHY2_BIST_PATTERN4        
CPHY2_BIST_STATUS          
CPHY2_RSVRBITS             
CPHY2_BIST_TMDS_CRC_UPDATE 
CPHY2_JE_CTRL              
CPHY2_JE_CONFIG            
CPHY2_JE                   
CPHY2_JE_TST_THRESHOLD     
CPHY2_JE_STATUS            
HDMI_RX_PHY_STATUS
HDMI_RX_PHY_SIGQUAL
HDMI_RX_PHY_PHS_A_SCR
HDMI_RX_PHY_PHS_B_SCR
HDMI_RX_PHY_PHS_C_SCR
HDMI_RX_PHY_IRQ_STS
HDRX_CLK_MS_FST_STATUS
HDRX_IRQ_STATUS
HDRX_MUTE_TOP_IRQ_STS
HDRX_MAIN_LINK_STATUS
HDRX_PHY_DEC_ERR_STATUS
HDRX_MEAS_RESULT
HDRX_MEAS_IRQ_STS
HDRX_PHY_LNK_CRC_TEST_STS
*/
      gmt_HdmiDviInputPort* ptr_PortConfig=&HdmiDviInPortConfig[GetInPortConfigArrayLocation(InputConnector)];
      RdCphyBuf *pRdCphy;
      WORD BasAddr= (WORD)(PC_InitParams.PhyBaseAddress_p);

      if(PC_InitParams.DeviceType!=STHDMIRX_DEVICE_TYPE_ATHENA_HDMI)
         return;

         

      if(CaptureFlag==TRUE)
      {
         if(idx < CATCH_TIME)
         {
            DWORD NewTime=gm_GetSystemTime();
            if((NewTime-CatchPreTime) > CATCH_DURATION)
            {
               gm_Print("*********Cphy Capture(%d)",idx);
               
               pRdCphy=&(RdCphy[idx]);
               pRdCphy->dwTime=gm_GetSystemTime();
               pRdCphy->dwBuf[0]=gm_ReadRegDWord(BasAddr+CPHY_AUX_PHY_CTRL);
               pRdCphy->dwBuf[1]=gm_ReadRegDWord(BasAddr+CPHY_PHY_CTRL);
               pRdCphy->dwBuf[2]=gm_ReadRegDWord(BasAddr+CPHY_L0_CTRL_0);
               pRdCphy->dwBuf[3]=gm_ReadRegDWord(BasAddr+CPHY_L0_CTRL_4);
               pRdCphy->dwBuf[4]=gm_ReadRegDWord(BasAddr+CPHY_L0_CPTRIM);
               pRdCphy->dwBuf[5]=gm_ReadRegDWord(BasAddr+CPHY_L1_CTRL_0);
               pRdCphy->dwBuf[6]=gm_ReadRegDWord(BasAddr+CPHY_L1_CTRL_4);
               pRdCphy->dwBuf[7]=gm_ReadRegDWord(BasAddr+CPHY_L1_CPTRIM);
               pRdCphy->dwBuf[8]=gm_ReadRegDWord(BasAddr+CPHY_L2_CTRL_0);
               pRdCphy->dwBuf[9]=gm_ReadRegDWord(BasAddr+CPHY_L2_CTRL_4);
               
               pRdCphy->dwBuf[10]=gm_ReadRegDWord(BasAddr+CPHY_L2_CPTRIM);
               pRdCphy->dwBuf[11]=gm_ReadRegDWord(BasAddr+CPHY_L3_CTRL_0);
               pRdCphy->dwBuf[12]=gm_ReadRegDWord(BasAddr+CPHY_L3_CTRL_4);
               pRdCphy->dwBuf[13]=gm_ReadRegDWord(BasAddr+CPHY_L3_CPTRIM);
               pRdCphy->dwBuf[14]=gm_ReadRegDWord(BasAddr+CPHY_RCAL_RESULT);
               pRdCphy->dwBuf[15]=gm_ReadRegDWord(BasAddr+CPHY_ALT_FREQ_MEAS_CTRL);
               pRdCphy->dwBuf[16]=gm_ReadRegDWord(BasAddr+CPHY_ALT_FREQ_MEAS_RESULT);
               pRdCphy->dwBuf[17]=gm_ReadRegDWord(BasAddr+CPHY_TEST_CTRL);
               pRdCphy->dwBuf[18]=gm_ReadRegDWord(BasAddr+CPHY_BIST_CONTROL);
               pRdCphy->dwBuf[19]=gm_ReadRegDWord(BasAddr+CPHY_BIST_PATTERN0);
               
               pRdCphy->dwBuf[20]=gm_ReadRegDWord(BasAddr+CPHY_BIST_PATTERN1);
               pRdCphy->dwBuf[21]=gm_ReadRegDWord(BasAddr+CPHY_BIST_PATTERN2);
               pRdCphy->dwBuf[22]=gm_ReadRegDWord(BasAddr+CPHY_BIST_PATTERN3);
               pRdCphy->dwBuf[23]=gm_ReadRegDWord(BasAddr+CPHY_BIST_PATTERN4);
               pRdCphy->dwBuf[24]=gm_ReadRegDWord(BasAddr+CPHY_BIST_STATUS);
               pRdCphy->dwBuf[25]=gm_ReadRegDWord(BasAddr+CPHY_RSVRBITS);
               pRdCphy->dwBuf[26]=gm_ReadRegDWord(BasAddr+CPHY_BIST_TMDS_CRC_UPDATE);
               pRdCphy->dwBuf[27]=gm_ReadRegDWord(BasAddr+CPHY_JE_CTRL);
               pRdCphy->dwBuf[28]=gm_ReadRegDWord(BasAddr+CPHY_JE_CONFIG);
               pRdCphy->dwBuf[29]=gm_ReadRegDWord(BasAddr+CPHY_JE);
               
               pRdCphy->dwBuf[30]=gm_ReadRegDWord(BasAddr+CPHY_JE_TST_THRESHOLD);
               pRdCphy->dwBuf[31]=gm_ReadRegDWord(BasAddr+CPHY_JE_STATUS);
               pRdCphy->dwBuf[32]=gm_ReadRegDWord(HDMI_RX_PHY_STATUS);
               pRdCphy->dwBuf[33]=gm_ReadRegDWord(HDMI_RX_PHY_SIGQUAL);
               pRdCphy->dwBuf[34]=gm_ReadRegDWord(HDMI_RX_PHY_PHS_A_SCR);
               pRdCphy->dwBuf[35]=gm_ReadRegDWord(HDMI_RX_PHY_PHS_B_SCR);
               pRdCphy->dwBuf[36]=gm_ReadRegDWord(HDMI_RX_PHY_PHS_C_SCR);
               pRdCphy->dwBuf[37]=gm_ReadRegDWord(HDMI_RX_PHY_IRQ_STS);
               pRdCphy->dwBuf[38]=gm_ReadRegDWord(HDRX_CLK_MS_FST_STATUS);
               pRdCphy->dwBuf[39]=gm_ReadRegDWord(HDRX_IRQ_STATUS);
               
               pRdCphy->dwBuf[40]=gm_ReadRegDWord(HDRX_MUTE_TOP_IRQ_STS);   
               pRdCphy->dwBuf[41]=gm_ReadRegDWord(HDRX_MAIN_LINK_STATUS);
               pRdCphy->dwBuf[42]=gm_ReadRegDWord(HDRX_PHY_DEC_ERR_STATUS);
               pRdCphy->dwBuf[43]=gm_ReadRegDWord(HDRX_MEAS_RESULT);
               pRdCphy->dwBuf[44]=gm_ReadRegDWord(HDRX_MEAS_IRQ_STS);   
               pRdCphy->dwBuf[45]=gm_ReadRegDWord(HDRX_PHY_LNK_CRC_TEST_STS);      
               idx++;
               
               CatchPreTime=gm_GetSystemTime();
            }
         }
         else
         {
            CaptureFlag=FALSE;
         }
      }
}


void PrintCphyRegs(void)
{
   BYTE i;
   BYTE j;
   RdCphyBuf *pRdCphy;
   for (i=0;i<idx;i++)
   {
      pRdCphy=&(RdCphy[i]);
      gm_Print("",0);
      gm_Print("Index:%d",i);
      gm_Print("T(%Ld)",pRdCphy->dwTime);
      for(j=0;j<46;j++)
         gm_Print("%LX",pRdCphy->dwBuf[j]); 
   }
   gm_Delay10ms(1);
}

void StopCaptureCphyRegs(void)
{
   CaptureFlag=FALSE;
}
#endif 

gmt_SHA_STRUCT gS_HdmiRxSha;

void far HdmiRxExt_Sha1Reset(void)
{
   gm_SHA1Reset((gmt_SHA1_CONTEXT *) &gS_HdmiRxSha);
   hdmi_msg("Hdmi_SHA: Reset", 0);
}

void far HdmiRxExt_Sha1Input(const U8 * Bp_Message, U16 W_Length)
{

   gm_SHA1Input((gmt_SHA1_CONTEXT *) &gS_HdmiRxSha, (BYTE far *) Bp_Message, W_Length);
   hdmi_msg("Hdmi_SHA: Processed", 0);
}

U8 * far HdmiRxExt_Sha1Result(void)
{
   gm_SHA1Result((gmt_SHA1_CONTEXT *) &gS_HdmiRxSha, (BYTE *) gS_HdmiRxSha.Message_Digest);
   hdmi_msg("Hdmi_SHA: Result is obtained", 0);

   return ((U8 far*) gS_HdmiRxSha.Message_Digest);
}

#if (ENABLE_MIN_PORT_OFF_LIMITATION==1)
void HdmiDvi_SendAC_ON_Notice(void)
{
   b_AcOnNoticed=TRUE;
   //gm_Print("b_AcOnNoticed",0);
}
#endif

#if (IBD_CONSISTENCE_CHECK !=0)
BOOL HdmiDviConsistenceCheck(gmt_PHY_CH Channel, gmt_IbdStruct *ibd)
{
   BYTE ret=FALSE;
   if((IbdReCheck[Channel].Pre_HActive==ibd->HActive) && (IbdReCheck[Channel].Pre_VActive==ibd->VActive))
   {
      ret=TRUE;
   }
   #ifdef _DEBUG_HDMI_MSG
   else
   {
      gm_Print("PreHV (%d x %d)",IbdReCheck[Channel].Pre_HActive, IbdReCheck[Channel].Pre_VActive);
      gm_Print("NewHV (%d x %d)",ibd->HActive, ibd->VActive);
   }
   #endif
   
   IbdReCheck[Channel].Pre_HActive=ibd->HActive;
   IbdReCheck[Channel].Pre_VActive=ibd->VActive;
   hdmi_msg("HdmiDviConsistenceCheck(%d)",ret);
   return ret;
}


void ResetPreIbdValue(gmt_PHY_CH Channel)
{
   IbdReCheck[Channel].Pre_HActive=0;
   IbdReCheck[Channel].Pre_VActive=0;
}
#endif


void far DefaultHdmiSetHpd(void)
{
   hdmi_msg("Hdmi_Hpd_1",0);
#if (Default_Hpd_Polarity_Reversed==0)   
   gm_SetRegBitsWord(HDRX_HPD_CTRL, HDRX_HPD_CTL);
#else
   gm_ClearRegBitsWord(HDRX_HPD_CTRL, HDRX_HPD_CTL);
#endif
}

void far DefaultHdmiClearHpd(void)
{
   hdmi_msg("Hdmi_Hpd_0",0);
#if (Default_Hpd_Polarity_Reversed==0) 
   gm_ClearRegBitsWord(HDRX_HPD_CTRL, HDRX_HPD_CTL);
#else
   gm_SetRegBitsWord(HDRX_HPD_CTRL, HDRX_HPD_CTL);
#endif   
}

void far DefaultDviSetHpd(void)
{
   hdmi_msg("Dvi_Hpd_1",0);
#if (Default_Hpd_Polarity_Reversed==0)    
   gm_SetRegBitsWord(DVI0_HPD_CTRL, HDRX_HPD_CTL);
#else
   gm_ClearRegBitsWord(DVI0_HPD_CTRL, HDRX_HPD_CTL);
#endif   
}

void far DefaultDviClearHpd(void)
{
   hdmi_msg("Dvi_Hpd_0",0);
#if (Default_Hpd_Polarity_Reversed==0)    
   gm_ClearRegBitsWord(DVI0_HPD_CTRL, HDRX_HPD_CTL);
#else
   gm_SetRegBitsWord(DVI0_HPD_CTRL, HDRX_HPD_CTL);
#endif   
}


static void SelectCPhy2HdmiRxSel(gmt_COMBOPHY_AID b_AID)
{
   switch (b_AID)
   {
      case AID_PHY0:
         gm_ClearAndSetBitsWord(HOST_DVI_DP_SETUP,HDMI_PORT_SEL,0);
         break;
      case AID_PHY1:
         gm_ClearAndSetBitsWord(HOST_DVI_DP_SETUP,HDMI_PORT_SEL,BIT0);
         break;
      case AID_PHY2:
         gm_ClearAndSetBitsWord(HOST_DVI_DP_SETUP,HDMI_PORT_SEL,BIT1);
         break;
		case AID_PHY_NONE:
			gm_ClearAndSetBitsWord(HOST_DVI_DP_SETUP,HDMI_PORT_SEL,BIT0 | BIT1);
			break;
   }
}

void HdmiDviInit(void)
{
   BYTE b_HdmiDviInNo,b_InputConnector;
   gmt_HDMI_DVI_DID b_DID;
   gmt_COMBOPHY_AID b_AID;
   gmt_HdmiDviInputPort * ptr_PortConfig;

   gm_WriteRegByte(HDRX_HDCP_ADDR,0);
   gm_WriteRegByte(DVI0_HDCP_ADDR,0);
   gm_WriteRegByte(DVI1_HDCP_ADDR,0);

#ifdef LOAD_HDCPKEY_FROM_CODE_FOR_TEST
   hdmi_msg("Load Hdcp Keys from code for testing",0);
   LoadHdcpKeyArray((BYTE far *)BA_Hdcp_Keys, NULL_PTR);
#endif

   hdmi_msg("HdmiDviInit",0);
   gm_Print(STHDMIRX_GetRevision(),0);
   // Init Defualt as invalid input port. (0xFF)
   for (b_HdmiDviInNo=0;b_HdmiDviInNo< NUM_OF_HDMIDVI_INP ; b_HdmiDviInNo++)
   {
      ptr_PortConfig       = &HdmiDviInPortConfig[b_HdmiDviInNo];
      PC_InputConnector    = 0xFF;
      PC_IsActivePort      = FALSE;
      PC_SignalDataLoaded  = FALSE;
      PC_VideoDataLoaded   = FALSE;
      PC_AudioDataLoaded   = FALSE;
      PC_PreAVMuteStatus   = FALSE;
      #if (ENABLE_MIN_PORT_OFF_LIMITATION == 1)
      if(b_AcOnNoticed==TRUE)
         PC_PrevPortOffTime   = gm_GetSystemTime();
      else
         PC_PrevPortOffTime   = 0xFFFFFFFFUL ; //gm_GetSystemTime();
      #endif
   }


   b_HdmiDviInNo = 0;
   b_InputConnector = 0;

   while ( (b_HdmiDviInNo < NUM_OF_HDMIDVI_INP) && (b_InputConnector < NUM_MASTER_LIST))
   {
      b_DID=gm_GetHdmiDviDigitalID(b_InputConnector);
      if (b_DID != DID_HDMI_DVI_NONE)
      {

         b_AID=gm_GetHdmiDviAnaPhyID(b_InputConnector);
         if (b_AID != AID_PHY_NONE)
         {
            ptr_PortConfig = &HdmiDviInPortConfig[b_HdmiDviInNo];
            // Assign Input Port Configuration Parameters
            PC_InputConnector = b_InputConnector;
            _fmemcpy( PC_DeviceName,((gmt_HdmiDviProperties far*)(GetPortPropertyTable(b_InputConnector)))->DeviceName,ST_MAX_DEVICE_NAME);
            PC_InitParams.MeasureClockFreqHz = TCLK_FREQ;
            PC_InitParams.MaxOpen			 =	1;
            PC_InitParams.I2SOutClkScalingFactor = 384;
            PC_InitParams.DDVITablePtr = NULL_PTR;
            PC_InitParams.Sha1Reset_pf = HdmiRxExt_Sha1Reset;
            PC_InitParams.Sha1Input_pf = HdmiRxExt_Sha1Input;
            PC_InitParams.Sha1Result_pf= HdmiRxExt_Sha1Result; 
            hdmi_msg("===HdmiDviInPortConfig: %d ===",b_HdmiDviInNo);
            hdmi_msg("AppInputConnector No= %d", PC_InputConnector);
            //hdmi_msg("%s",PC_DeviceName);
            switch (b_DID)
            {
               case DID_DVI0:
                  {
                  #if DVI_HPD_PULSE_IN_PORT_INIT
                     PC_EnableHpdInPortInit=TRUE;
                  #else
                     PC_EnableHpdInPortInit=FALSE;
                  #endif
                     PC_InitParams.DeviceType=STHDMIRX_DEVICE_TYPE_ATHENA_DDVI;
                     PC_InitParams.DeviceBaseAddress_p =  DVI0_RX_PHY_RES_CTRL;
                     PC_InitParams.BaseAddress_p = DVI0_RX_PHY_CTRL;
                     PC_InitParams.ClockGenBaseAddress_p =	DVI0_TCLK_SEL;
                     PC_InitParams.SetHPD_pf = DefaultDviSetHpd;
                     PC_InitParams.ClearHPD_pf = DefaultDviClearHpd;
                     hdmi_msg("DEVICE_TYPE_ATHENA_DDVI (DVI_0)",0);
                  }
                  break;
               case DID_DVI1:
                  {
                  #if DVI_HPD_PULSE_IN_PORT_INIT
                     PC_EnableHpdInPortInit=TRUE;
                  #else
                     PC_EnableHpdInPortInit=FALSE;
                  #endif
                     PC_InitParams.DeviceType=STHDMIRX_DEVICE_TYPE_ATHENA_DDVI;
                     PC_InitParams.DeviceBaseAddress_p =  DVI1_RX_PHY_RES_CTRL;
                     PC_InitParams.BaseAddress_p = DVI1_RX_PHY_CTRL;
                     PC_InitParams.ClockGenBaseAddress_p =	DVI1_TCLK_SEL;
                     PC_InitParams.SetHPD_pf = DefaultDviSetHpd;
                     PC_InitParams.ClearHPD_pf = DefaultDviClearHpd;
                     hdmi_msg("DEVICE_TYPE_ATHENA_DDVI (DVI_1)",0);
                  }
                  break;
               case DID_DVI_DUAL:
                  {
                  #if DVI_HPD_PULSE_IN_PORT_INIT
                     PC_EnableHpdInPortInit=TRUE;
                  #else
                     PC_EnableHpdInPortInit=FALSE;
                  #endif
                     PC_InitParams.DeviceType=STHDMIRX_DEVICE_TYPE_ATHENA_DDVI;
                     PC_InitParams.DeviceBaseAddress_p =  DVI0_RX_PHY_RES_CTRL;
                     PC_InitParams.BaseAddress_p = DVI0_RX_PHY_CTRL;
                     PC_InitParams.ClockGenBaseAddress_p =	DVI0_TCLK_SEL;
                  #if (ENABLE_DDVI_EXCEPTION_TABLE==1)
                     PC_InitParams.DDVITablePtr= (STHDMIRX_DDVIExceptionModeTable_t far*)DDVI_ExceptTable; 
                  #endif
                     PC_InitParams.SetHPD_pf = DefaultDviSetHpd;
                     PC_InitParams.ClearHPD_pf = DefaultDviClearHpd;
                  
                     hdmi_msg("DEVICE_TYPE_ATHENA_DDVI (Dual DVI)",0);
                  }
                  break;
               case DID_HDMI:
                  {
                  #if HDMI_HPD_PULSE_IN_PORT_INIT
                     PC_EnableHpdInPortInit=TRUE;
                  #else
                     PC_EnableHpdInPortInit=FALSE;
                  #endif
                     PC_InitParams.DeviceType=STHDMIRX_DEVICE_TYPE_ATHENA_HDMI;
                     PC_InitParams.DeviceBaseAddress_p =  HDMI_RX_PHY_RES_CTRL;
                     PC_InitParams.BaseAddress_p = HDMI_RX_PHY_CTRL;
                     PC_InitParams.ClockGenBaseAddress_p =	HDRX_TCLK_SEL;
                     PC_InitParams.SetHPD_pf = DefaultHdmiSetHpd;
                     PC_InitParams.ClearHPD_pf = DefaultHdmiClearHpd;
                     hdmi_msg("DEVICE_TYPE_ATHENA_HDMI",0);
                  }
                  break;
            }

            switch (b_AID)
            {
               case AID_PHY0:
                  {
                     PC_InitParams.PhyBaseAddress_p =  CPHY0_AUX_PHY_CTRL ;
                     PC_InitParams.PhyBaseAddress_p2 =  NULL;
                     hdmi_msg("CPHY0_AUX_PHY",0);
                  }
                  break;

               case AID_PHY1:
                  {
                     PC_InitParams.PhyBaseAddress_p =  CPHY1_AUX_PHY_CTRL ;
                     PC_InitParams.PhyBaseAddress_p2 =  NULL;
                     hdmi_msg("CPHY1_AUX_PHY",0);
                  }
                  break;
               case AID_PHY2:
                  {
                     PC_InitParams.PhyBaseAddress_p =  CPHY2_AUX_PHY_CTRL ;
                     PC_InitParams.PhyBaseAddress_p2 =  NULL;
                     hdmi_msg("CPHY2_AUX_PHY",0);
                  }
                  break;

               case AID_PHY12:
                  {
                     PC_InitParams.PhyBaseAddress_p =  CPHY1_AUX_PHY_CTRL ;
                     PC_InitParams.PhyBaseAddress_p2 =  CPHY2_AUX_PHY_CTRL ;
                     hdmi_msg("CPHY1/2_AUX_PHY",0);
                  }
                  break;
            }
            STHDMIRX_Term(PC_DeviceName,&PC_TermParams);
            // Next HdmiDviInConfig
            b_HdmiDviInNo++;
         }
         if (((gmt_HdmiDviProperties far*)(GetPortPropertyTable(b_InputConnector)))->CustomSetHPD != NULL_PTR)
            PC_InitParams.SetHPD_pf = ((gmt_HdmiDviProperties far*)(GetPortPropertyTable(b_InputConnector)))->CustomSetHPD;
         
         if (((gmt_HdmiDviProperties far*)(GetPortPropertyTable(b_InputConnector)))->CustomClearHPD != NULL_PTR)
            PC_InitParams.ClearHPD_pf = ((gmt_HdmiDviProperties far*)(GetPortPropertyTable(b_InputConnector)))->CustomClearHPD;
         
      }
      // Next InputConnector
      b_InputConnector++;
   }
   SelectCPhy2HdmiRxSel(AID_PHY1);   //Set default to PHY1

   #if (USE_HDMIDVI_HDCP_KSV_INTERRUPT!=0) || (USE_HDMIDVI_AFR_DETECT_INTERRUPT != 0)
   gm_WriteRegWord(HDMI_DVI_IRQ_SELECT,0xCC);   // Enable HDMI_IRQ_SEL / DVI_IRQ_SEL to IRQ4
   #endif

   #if (USE_MINI_ISR_HANDLER==1)
   b_Initialed=TRUE;
   #endif
}


BYTE GetInPortConfigArrayLocation(BYTE InputConnector)
{
   BYTE b_HdmiDviInNo;
   for (b_HdmiDviInNo=0;b_HdmiDviInNo< NUM_OF_HDMIDVI_INP ; b_HdmiDviInNo++)
   {
      if (HdmiDviInPortConfig[b_HdmiDviInNo].InputConnector == InputConnector)
         return b_HdmiDviInNo;

   }
   return 0xFF;
}

static void PatchOfHdmiPortSel(void)
{
   gmt_HdmiDviInputPort* ptr_PortConfig;
   BYTE b_HdmiDviInNo;
   BOOL b_HdmiFlag=FALSE;

   for (b_HdmiDviInNo=0;b_HdmiDviInNo< NUM_OF_HDMIDVI_INP ; b_HdmiDviInNo++)
   {
      ptr_PortConfig= &HdmiDviInPortConfig[b_HdmiDviInNo];
      if (PC_IsActivePort == TRUE)
      {
         if (PC_InitParams.DeviceType==STHDMIRX_DEVICE_TYPE_ATHENA_HDMI)
         {
            SelectCPhy2HdmiRxSel(gm_GetHdmiDviAnaPhyID(PC_InputConnector));
            b_HdmiFlag=TRUE;
         }
      }
   }
   if (b_HdmiFlag==FALSE)
   {
      //gm_Print("@@@@No HdmiPort is active",0);
      SelectCPhy2HdmiRxSel(AID_PHY_NONE);
//      gm_ClearAndSetBitsWord(HDRX_INPUT_SEL,HDRX_INP_PHY_SEL,1<<HDRX_INP_PHY_SEL_SHIFT);
   }


}

#if HDMI_1ST_MODESETUP_FB || DVI_1ST_MODESETUP_FB
static void InstantSoftReset(BYTE InputConnector)
{
   gmt_HdmiDviInputPort* ptr_PortConfig=&HdmiDviInPortConfig[GetInPortConfigArrayLocation(InputConnector)];
   
   gm_SetRegBitsByte((WORD)(PC_InitParams.DeviceBaseAddress_p)+HDRX_IFM_INSTRUMENT_PU_ADDRS_OFFSET+HDMIRX_INST_SOFT_RESETS,DVI0_IFM_RESET);
  	if(PC_InitParams.PhyBaseAddress_p2 != NULL_PTR)
      gm_SetRegBitsByte((WORD)(PC_InitParams.DeviceBaseAddress_p)+DVIB_OFFSET+HDRX_IFM_INSTRUMENT_PU_ADDRS_OFFSET+HDMIRX_INST_SOFT_RESETS,DVI0_IFM_RESET);

   gm_Delay1ms(1);
   gm_Print(">>>InstRest(%d)",InputConnector);

   gm_ClearRegBitsByte((WORD)(PC_InitParams.DeviceBaseAddress_p)+HDRX_IFM_INSTRUMENT_PU_ADDRS_OFFSET+HDMIRX_INST_SOFT_RESETS,DVI0_IFM_RESET);
  	if(PC_InitParams.PhyBaseAddress_p2 != NULL_PTR)
      gm_ClearRegBitsByte((WORD)(PC_InitParams.DeviceBaseAddress_p)+DVIB_OFFSET+HDRX_IFM_INSTRUMENT_PU_ADDRS_OFFSET+HDMIRX_INST_SOFT_RESETS,DVI0_IFM_RESET);

}

static void HdmiDviForceBackground(BYTE InputConnector, BYTE b_Set)
{
   gmt_HdmiDviInputPort* ptr_PortConfig=&HdmiDviInPortConfig[GetInPortConfigArrayLocation(InputConnector)];   
   if(b_Set==TRUE)
   {
   	gm_SetRegBitsByte((WORD)(PC_InitParams.DeviceBaseAddress_p)+HDMIRX_SDP_AV_MUTE_CTRL ,HDRX_A_MUTE_SOFT);
   	gm_WriteRegWord((WORD)(PC_InitParams.DeviceBaseAddress_p)+HDMIRX_PATGEN_CONTROL,HDRX_VIDEO_TPG_EN);
   	if(PC_InitParams.PhyBaseAddress_p2 != NULL_PTR)
   	{
         gm_WriteRegWord((WORD)(PC_InitParams.DeviceBaseAddress_p)+DVIB_OFFSET+HDMIRX_PATGEN_CONTROL,HDRX_VIDEO_TPG_EN);
   	}
   }
   else
   {
      gm_ClearRegBitsByte((WORD)(PC_InitParams.DeviceBaseAddress_p)+HDMIRX_SDP_AV_MUTE_CTRL ,HDRX_A_MUTE_SOFT);
      gm_WriteRegWord((WORD)(PC_InitParams.DeviceBaseAddress_p)+HDMIRX_PATGEN_CONTROL,0);
   	if(PC_InitParams.PhyBaseAddress_p2 != NULL_PTR)
   	{
         gm_WriteRegWord((WORD)(PC_InitParams.DeviceBaseAddress_p)+DVIB_OFFSET+HDMIRX_PATGEN_CONTROL,0);
   	}
   }
}

static WORD GetHdcpStatus(BYTE InputConnector)
{
   gmt_HdmiDviInputPort* ptr_PortConfig=&HdmiDviInPortConfig[GetInPortConfigArrayLocation(InputConnector)];   
   return gm_ReadRegWord((WORD)(PC_InitParams.DeviceBaseAddress_p)+HDMIRX_HDCP_STATUS);
}


BOOL IsDVIModeSetupFB(BYTE InputConnector)
{
   BYTE b_HdmiDviInNo = GetInPortConfigArrayLocation(InputConnector);
   gmt_HdmiDviInputPort* ptr_PortConfig;
   ptr_PortConfig= &HdmiDviInPortConfig[b_HdmiDviInNo];

   if (b_HdmiDviInNo >= NUM_OF_HDMIDVI_INP)
      return FALSE;

   if(PC_InitParams.DeviceType==STHDMIRX_DEVICE_TYPE_ATHENA_HDMI)
      return FALSE;

   return PC_FirstModeSetupFB;
}
#endif

void HdmiDviInputPortInit(gmt_PHY_CH B_Channel,BYTE InputConnector)
{
   BYTE b_HdmiDviInNo = GetInPortConfigArrayLocation(InputConnector);
   gmt_HdmiDviInputPort* ptr_PortConfig;

   if (b_HdmiDviInNo >= NUM_OF_HDMIDVI_INP)
   {
      hdmi_msg("HdmiDviInputPortInit Failed ###",0);
      return;
   }

   #if (DEBUG_HDMI_PRINT_TIMING_DISPLAY_TIME==1)
   SetDebugDisplayTime();
   #endif
   
   HdmiDviCloseInactivePort();

   //Call ExtPortSetup
   if (((gmt_HdmiDviProperties far*)(GetPortPropertyTable(InputConnector)))->ExtPortSetup != NULL_PTR)
      ((gmt_HdmiDviProperties far*)(GetPortPropertyTable(InputConnector)))->ExtPortSetup();


   ptr_PortConfig= &HdmiDviInPortConfig[b_HdmiDviInNo];

   hdmi_msg_ii("HdmiDviInputPortInit- CH%d  Port(%d)",B_Channel,InputConnector);



   /*
   	if(PC_InitParams.DeviceType==STHDMIRX_DEVICE_TYPE_ATHENA_HDMI)
   		SelectCPhy2HdmiRxSel(gm_GetHdmiDviAnaPhyID(InputConnector));
   	else if(PC_InitParams.PhyBaseAddress_p2 != NULL)
   		SelectCPhy2HdmiRxSel(AID_PHY0);	//DDVI Port must use PHY1/2
   */

   if (PC_IsActivePort!=TRUE)
   {
      PC_SignalDataLoaded = FALSE;
      PC_VideoDataLoaded  = FALSE;
      PC_AudioDataLoaded  = FALSE;
      PC_ReDoModeSetup    = FALSE;
      #if (LINK_CLOCK_CHECK_SUPPORT ==1)
      PC_IsLinkClockPresented = FALSE;
      PC_LinkClockCntIndex =0;
      PC_LinkClockCnt[0]=PC_LinkClockCnt[1]=PC_LinkClockCnt[2]=0;
      #endif
      
      //gm_Print("HdmiDviInputPortInit- CH%d  Port(%d)  --- Full",B_Channel,InputConnector);
      //	STHDMIRX_Init(p_Config->,&InitParams);
      #ifdef INACTIVE_DEVICE_POWER_SAVING
      HdmiDviPowrUpInputPort(PC_InputConnector);
      #endif
      
 
      STHDMIRX_Init(PC_DeviceName,&PC_InitParams);
      // Open a device instance
      STHDMIRX_Open(PC_DeviceName,&PC_OpenParams,&PC_DevHandle);

      //!!!! Check the capabilities and access device accordingly!!!!!!
      STHDMIRX_GetCapability(PC_DeviceName, &PC_Capability);
      hdmi_msg(" **********Capability of Athena HDMI Device***********",0);
      hdmi_msg("NumberOfInputPortsSupported:%d",PC_Capability.NumberOfInputPortsSupported);
      hdmi_msg("NumberOfRoutesSupported    :%d",PC_Capability.NumberOfRoutesSupported);
      hdmi_msg("SPDSupport                 :%d",PC_Capability.SPDSupport);
      hdmi_msg("CECInterfaceSupport        :%d",PC_Capability.CECInterfaceSupport);
      hdmi_msg("DDCInterfaceSupport        :%d",PC_Capability.DDCInterfaceSupport);
      hdmi_msg("SupportedOutputPixelWidth  :%d",PC_Capability.SupportedOutputPixelWidth);
      hdmi_msg("SupportedAudioStreams      :%d",PC_Capability.SupportedAudioStreams);
      hdmi_msg("VideoSupport3D             :%d",PC_Capability.VideoSupport3D);
      hdmi_msg("DDVISupport                :%d",PC_Capability.DDVISupport);
   }
   else
   {
      //gm_Print("HdmiDviInputPortInit- CH%d  Port(%d) --- ReOpen",B_Channel,InputConnector);
      STHDMIRX_StopSignalMonitor(PC_InputHandle);
      STHDMIRX_CloseInput(PC_InputHandle);

   }
   // Open input port
   // Establish the input connection
   if (PC_InitParams.DeviceType == STHDMIRX_DEVICE_TYPE_ATHENA_HDMI)
   {  //Assign Port 1 / Route 1 to HDMI Digital Phy
      PC_InputConfig.Port = STHDMIRX_INPUT_PORT_1;
      PC_InputConfig.Route = STHDMIRX_ROUTE_1;
      PC_InputConfig.ModeOfOperation = STHDMIRX_MODE_AUTO;
   }
   else
   {  //Assign Port 2 / Route 1 to DVI Digital Phy

      PC_InputConfig.Port = STHDMIRX_INPUT_PORT_2;
      PC_InputConfig.Route = STHDMIRX_ROUTE_1;
      if (PC_InitParams.PhyBaseAddress_p2 == NULL)
         PC_InputConfig.ModeOfOperation = STHDMIRX_MODE_DVI;
      else
         PC_InputConfig.ModeOfOperation = STHDMIRX_MODE_DDVI;
   }


   PC_InputConfig.OutputPixelWidth = STHDMIRX_OUTPUT_PIXEL_WIDTH_8_BITS;
   PC_InputConfig.EqMode = STHDMIRX_EQ_MODE_AUTO; //STHDMIRX_EQ_MODE_CUSTOM; //STHDMIRX_EQ_MODE_HIGH_GAIN;
   PC_InputConfig.CustomRterm = 8; 



   if (B_Channel==gmvb_MainChannel)
   {
      //gm_Print("***AVDDS1",0);
      PC_InputConfig.DdsConfigInfo.estVidDds= gm_SRC_AVDDS1;
      PC_InputConfig.DdsConfigInfo.estAudDds= gm_SRC_AVDDS3;
   }
   else // if(InputConnector==gB_CurrentInputConnector[gmvb_PipChannel])
   {
      //gm_Print("***AVDDS2",0);
      PC_InputConfig.DdsConfigInfo.estVidDds= gm_SRC_AVDDS2;
      PC_InputConfig.DdsConfigInfo.estAudDds= gm_SRC_AVDDS4;
   }
   /*	else
   	{
   	   gm_Print("***AVDDS Invalid",0);
   		PC_InputConfig.DdsConfigInfo.estVidDds= gm_SRC_AVDDS_INVALID;
   		PC_InputConfig.DdsConfigInfo.estAudDds= gm_SRC_AVDDS_INVALID;
   	}*/

   STHDMIRX_OpenInput(PC_DevHandle,&PC_InputConfig, &PC_InputHandle);

#if (HPD_CONTROL_HANDLER_ENABLE==1)
   if ((PC_IsActivePort!=TRUE) && (PC_EnableHpdInPortInit==TRUE))
   {
      HdmiSetHpdPulseStart(B_Channel, PC_InputConnector, HPD_PULSE_WIDTH_10mTime /* x 10ms */);
   }
   else
#endif   
   {
      HdmiDviHpdControl(PC_InputConnector, 1);      //HPD High
   }

   /*

   	if(gm_GetHdmiDviDigitalID(InputConnector)==DID_DVI0)
   		gm_SetRegBitsWord(DUAL_DVI0_CTRL,DUAL_DVI0_OUTPUT_EN);
   	else if (gm_GetHdmiDviDigitalID(InputConnector)==DID_DVI1)
   		gm_SetRegBitsWord(DUAL_DVI1_CTRL,DUAL_DVI1_OUTPUT_EN);

      if(gm_GetHdmiDviDigitalID(InputConnector)==DID_HDMI)
         gm_WriteRegWord(DUAL_DVI0_CTRL,0);
   */


   // Start signal Monitor
   STHDMIRX_StartSignalMonitor(PC_InputHandle);


   PC_IsActivePort = TRUE; //Mark this port as an active port;
   //hdmi_msg("HdmiDviInputPortInit-END",0);

   PatchOfHdmiPortSel();

#if Patch_Of_RCVRDATA_POLARITY_in_Cut2   
   if(PC_InitParams.PhyBaseAddress_p ==CPHY0_AUX_PHY_CTRL)
      gm_SetRegBitsDWord(CPHY0_PHY_CTRL,CPHY1_RCVRDATA_POLARITY);
   else if(PC_InitParams.PhyBaseAddress_p ==CPHY1_AUX_PHY_CTRL)
      gm_SetRegBitsDWord(CPHY1_PHY_CTRL,CPHY1_RCVRDATA_POLARITY);
   else if(PC_InitParams.PhyBaseAddress_p ==CPHY2_AUX_PHY_CTRL)
      gm_SetRegBitsDWord(CPHY2_PHY_CTRL,CPHY1_RCVRDATA_POLARITY);

   if (PC_InitParams.PhyBaseAddress_p2 == CPHY2_AUX_PHY_CTRL)
      gm_SetRegBitsDWord(CPHY2_PHY_CTRL,CPHY1_RCVRDATA_POLARITY);
   
#endif

#if (PATCH_OF_DVI_FIELD_INVERSION == 1)
   if(PC_InitParams.DeviceType==STHDMIRX_DEVICE_TYPE_ATHENA_DDVI)
   {
      WORD BaseAddr= (WORD)(PC_InitParams.DeviceBaseAddress_p)+HDRX_IFM_INSTRUMENT_PU_ADDRS_OFFSET;
      gm_SetRegBitsWord(BaseAddr+HDMI_RX_INSTRUMENT_POLARITY_CONTROL,DVI0_VS_POL);

      if(PC_InitParams.PhyBaseAddress_p2 != NULL_PTR)
      {
         gm_SetRegBitsWord(BaseAddr+DVIB_OFFSET+HDMI_RX_INSTRUMENT_POLARITY_CONTROL,DVI0_VS_POL);
      }
   }

#endif 
#if 0
   if ((PC_InputConfig.ModeOfOperation == STHDMIRX_MODE_DVI) && (PC_InitParams.PhyBaseAddress_p ==  CPHY2_AUX_PHY_CTRL))
      gm_WriteRegByte(DUAL_DVI0_CTRL,0x17);
#endif

   if(PC_InitParams.DeviceType==STHDMIRX_DEVICE_TYPE_ATHENA_HDMI)
   {
   #if HDMI_1ST_MODESETUP_FB
      PC_FirstModeSetupFB=TRUE;
      PC_PerformInstReset=TRUE;
   #else
      PC_FirstModeSetupFB=FALSE;
      PC_PerformInstReset=FALSE;
   #endif
   }
   else  //DVI or DDVI
   {
   #if DVI_1ST_MODESETUP_FB
      PC_FirstModeSetupFB=TRUE;
      PC_PerformInstReset=TRUE;
   #else
      PC_FirstModeSetupFB=FALSE;
      PC_PerformInstReset=FALSE;
   #endif
   }

   #if 0 //HDMI_1ST_MODESETUP_FB || DVI_1ST_MODESETUP_FB
   if(PC_FirstModeSetupFB==TRUE)
   {
      BYTE bTimer=(B_Channel==gmvb_MainChannel)? MODESETUP_FB_Main_10ms_TMR:MODESETUP_FB_Pip_10ms_TMR;
      //InstantSoftReset(InputConnector);
      gm_TimerStart(bTimer,MODESETUP_FB_10mTime);
      HdmiDviForceBackground(PC_InputConnector, TRUE);
      gm_Print("---->MODESETUP_FB_Start (%ld)",gm_GetSystemTime());
   }
   #endif

#if (DEBUG_HDMI_PRINT_TIMING_DISPLAY_TIME==1)
    gm_Print("PortInit --> PortInitEnd: T(%d)",(WORD)(gm_GetSystemTime()-GetDebugDisplayTime()));
#endif  

#if (DEBUG_HDMI_CPHY_CAPTURE !=0)
   if(PC_InitParams.DeviceType==STHDMIRX_DEVICE_TYPE_ATHENA_HDMI)
   {
      CaptureFlag=TRUE;
      CatchPreTime=gm_GetSystemTime();
   }
#endif     
} // end main( )

void HdmiDviCloseInactivePort(void)
{
   gmt_HdmiDviInputPort* ptr_PortConfig;
   BYTE b_HdmiDviInNo;
   //hdmi_msg("HdmiDviCloseInactivePort",0);
   for (b_HdmiDviInNo=0;b_HdmiDviInNo< NUM_OF_HDMIDVI_INP ; b_HdmiDviInNo++)
   {
      ptr_PortConfig= &HdmiDviInPortConfig[b_HdmiDviInNo];
      if (PC_IsActivePort == TRUE)
      {
         if ((PC_InputConnector != UserPrefCurrentInputMain) &&
               (PC_InputConnector != UserPrefCurrentInputPip))
         {
            //gm_Print("HdmiDviCloseInactivePort",0);
            HdmiDviCloseInputPort(PC_InputConnector);

            #ifdef INACTIVE_DEVICE_POWER_SAVING
            HdmiDviPowrDownInputPort(PC_InputConnector);
            #endif

            #if (ENABLE_MIN_PORT_OFF_LIMITATION==1)
            PC_PrevPortOffTime = gm_GetSystemTime();
            #endif

         }
      }
   }
}

#if 0
void HdmiDviSetCustomEQ(BYTE InputConnector, BYTE EqValue)
{
   STHDMIRX_EqualizationGain_t EqGain;
   gmt_HdmiDviInputPort* ptr_PortConfig=&HdmiDviInPortConfig[GetInPortConfigArrayLocation(InputConnector)];

   EqGain.LowFreqGain = 0;

   EqGain.HighFreqGain = EqValue;

   STHDMIRX_SetEqualizationGain(PC_InputHandle, &EqGain);
}
#endif
//******************************************************************************
// FUNCTION       : void HdmiDviCloseInputPort(BYTE InputConnector)
// USAGE          : This function calls hdmi library to close the input
//                  hdmi connection.
// INPUT          : None
// OUTPUT         : None
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  :
// POST_CONDITION :
//******************************************************************************
void HdmiDviCloseInputPort(BYTE InputConnector)
{
   BYTE b_HdmiDviInNo = GetInPortConfigArrayLocation(InputConnector);
   gmt_HdmiDviInputPort* ptr_PortConfig;

   hdmi_msg("HdmiDviCloseInputPort(%d)",InputConnector);

   if (b_HdmiDviInNo >= NUM_OF_HDMIDVI_INP)
   {
      hdmi_msg("HdmiDviCloseInputPort Failed ###",0);
      return;
   }
   
   
   ptr_PortConfig= &HdmiDviInPortConfig[b_HdmiDviInNo];
   /*
   	ST_DeviceName_t*                            p_DeviceName= ptr_PortConfig->p_DeviceName;
   	STHDMIRX_Handle_t*							DevHandle= &(ptr_PortConfig->DevHandle);
   	STHDMIRX_TermParams_t*                      TermParams= &(ptr_PortConfig->TermParams);
   */
   // close the input connection.
 
   PC_IsActivePort = FALSE;

#if (USE_HDMIDVI_AFR_DETECT_INTERRUPT!=0)
   HdmiDviEnableAfrInterrupt(InputConnector, FALSE);
#endif
 
   STHDMIRX_CloseInput(PC_InputHandle);
   STHDMIRX_Close(PC_DevHandle);
   //	DevHandle);
   PC_TermParams.ForceTerminate=TRUE;
   //gm_Print("@@@@@@Terminate(%d)",InputConnector);
   STHDMIRX_Term(PC_DeviceName,&PC_TermParams);

}


//******************************************************************************
// FUNCTION       : HdmiRedoModeSetupCheck
// USAGE          : Use to check if is needed to redo modesetup when 
//                  video evt changed
// INPUT          : STHDMIRX_VideoProperty_t* PreVidData, STHDMIRX_VideoProperty_t* NewVidData
// OUTPUT         : TRUE/FALSE
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  :
// POST_CONDITION :
//******************************************************************************
static BYTE HdmiRedoModeSetupCheck(STHDMIRX_VideoProperty_t* PreVidData, STHDMIRX_VideoProperty_t* NewVidData)
{
   BYTE ret=FALSE;

   hdmi_msg("HdmiRedoModeSetupCheck",0);

   if(PreVidData->HDMIVideoFormat != NewVidData->HDMIVideoFormat)
   {
      hdmi_msg_ii("ChgEvt:HDMIVideoFormat(%d-->%d)", PreVidData->HDMIVideoFormat, NewVidData->HDMIVideoFormat);
      ret |=TRUE;
   }

   // Variable "HDMIVideoFormat" related with "Vendor-Specific Info Frame".
   // Most of HDMI sources don't access it.
   // It caused wrong image color when input color space changed.
   //if(NewVidData->HDMIVideoFormat==FALSE)
   //{
   //   return ret;
   //}
   
   if(PreVidData->Video3D != NewVidData->Video3D)
   {
      hdmi_msg_ii("ChgEvt:Video3D(%d-->%d)", PreVidData->Video3D, NewVidData->Video3D);
      ret |=TRUE;
   }

   if(NewVidData->Video3D==TRUE)
   {
      if(PreVidData->VideoProperty3D.Format!= NewVidData->VideoProperty3D.Format)
      {
         hdmi_msg_ii("ChgEvt:3DFormat(%d-->%d)", PreVidData->VideoProperty3D.Format, NewVidData->VideoProperty3D.Format);
         ret |=TRUE;
      }
   }

   if(PreVidData->ColorFormat != NewVidData->ColorFormat)
   {
      hdmi_msg_ii("ChgEvt:ColorFormat(%d-->%d)", PreVidData->ColorFormat, NewVidData->ColorFormat);
      ret |=TRUE;
   }

   if(PreVidData->Colorimetery != NewVidData->Colorimetery)
   {
      hdmi_msg_ii("ChgEvt:Colorimetery(%d-->%d)", PreVidData->Colorimetery, NewVidData->Colorimetery);
      ret |=TRUE;
   }

   if(PreVidData->RGBQuantRange != NewVidData->RGBQuantRange)
   {
      hdmi_msg_ii("ChgEvt:ScalingInfo(%d-->%d)", PreVidData->RGBQuantRange, NewVidData->RGBQuantRange);
      ret |=TRUE;
   }

   if(PreVidData->YCQuantRange != NewVidData->YCQuantRange)
   {
      hdmi_msg_ii("ChgEvt:YCQuantRange(%d-->%d)", PreVidData->YCQuantRange, NewVidData->YCQuantRange);
      ret |=TRUE;
   }

/* Note: If application support the following info changed in mode-setup, please enable it accordingly */
#if 0
   if(PreVidData->BarInfo != NewVidData->BarInfo)
   {
      hdmi_msg_ii("ChgEvt:BarInfo(%d-->%d)", PreVidData->BarInfo, NewVidData->BarInfo);
      ret |=TRUE;
   }

   if(PreVidData->ScanInfo != NewVidData->ScanInfo)
   {
      hdmi_msg_ii("ChgEvt:ScanInfo(%d-->%d)", PreVidData->ScanInfo, NewVidData->ScanInfo);
      ret |=TRUE;
   }

   if(PreVidData->ScalingInfo != NewVidData->ScalingInfo)
   {
      hdmi_msg_ii("ChgEvt:ScalingInfo(%d-->%d)", PreVidData->ScalingInfo, NewVidData->ScalingInfo);
      ret |=TRUE;
   }

   if(PreVidData->PictureAR != NewVidData->PictureAR)
   {
      hdmi_msg_ii("ChgEvt:PictureAR(%d-->%d)", PreVidData->PictureAR, NewVidData->PictureAR);
      ret |=TRUE;
   }

   if(PreVidData->ActiveFormatAR != NewVidData->ActiveFormatAR)
   {
      hdmi_msg_ii("ChgEvt:ActiveFormatAR(%d-->%d)", PreVidData->ActiveFormatAR, NewVidData->ActiveFormatAR);
      ret |=TRUE;
   }


   if(PreVidData->PixelRepeatFactor != NewVidData->PixelRepeatFactor)
   {
      hdmi_msg_ii("ChgEvt:PixelRepeatFactor(%d-->%d)", PreVidData->PixelRepeatFactor, NewVidData->PixelRepeatFactor);
      ret |=TRUE;
   }


   if(PreVidData->STHDMIRX_ContentType_t != NewVidData->STHDMIRX_ContentType_t)
   {
      hdmi_msg_ii("ChgEvt:STHDMIRX_ContentType_t(%d-->%d)", PreVidData->STHDMIRX_ContentType_t, NewVidData->STHDMIRX_ContentType_t);
      ret |=TRUE;
   }

#endif
   return ret;
}



//******************************************************************************
// FUNCTION       : void HdmiDviHandler(void)
// USAGE          : This function calls hdmi library function, and monitors
//                  hdmi connection.
// INPUT          : None
// OUTPUT         : None
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  :
// POST_CONDITION :
//******************************************************************************
void HdmiDviInputPortHandler(gmt_PHY_CH Channel, BYTE InputConnector)
{
   BYTE b_HdmiDviInNo = GetInPortConfigArrayLocation(InputConnector);
   gmt_HdmiDviInputPort* ptr_PortConfig;

   if (!((GetPortConnType(InputConnector)==HDMI)||(GetPortConnType(InputConnector)==DVI)))
      return ;

   if (b_HdmiDviInNo >= NUM_OF_HDMIDVI_INP)
   {
      hdmi_msg("HdmiDviCloseInputPort Failed ###",0);
      return;
   }

   //hdmi_msg("--- HdmiDviInputPortHandler(%d)",InputConnector);
   UpdateComboPhyCableState(gm_GetHdmiDviAnaPhyID(InputConnector)); 

#if (HPD_CONTROL_HANDLER_ENABLE==1)
   HdmiHpdhandler(Channel,InputConnector);
#endif

#if (LINK_CLOCK_CHECK_SUPPORT ==1) 
   HdmiDviIsLinkClockPresent(InputConnector);
#endif   

#if DEBUG_HDMI_CPHY_CAPTURE !=0
   CaptureCphyRegs(InputConnector);
#endif

   ptr_PortConfig= &HdmiDviInPortConfig[b_HdmiDviInNo];

   {
   #if (HDMI_PKT_ERROR_HPD !=0)
      BYTE bSigStableTimer=(Channel==gmvb_MainChannel)? HDMI_SIG_STABLE_Main_10ms_TMR:HDMI_SIG_STABLE_Pip_10ms_TMR;
   #endif   

      PC_Event = STHDMIRX_Handler (PC_DevHandle);

#if 1
      if (STHDMIRX_GetHwMuteStatus(PC_InputHandle)==TRUE)
      {
         if (PC_PreAVMuteStatus == FALSE)
         {
            PC_PreAVMuteStatus = TRUE;
            hdmi_msg("Port(%d) HdmiAVMute is ON",InputConnector);
            //ToDo: Place code here for AVmute status change

         #if (FEATURE_AUDIO == ENABLE)
            SYS_API_AUDIO_HDMI_Set_AVMUTE();
         #endif
         }
      }
      else
      {
         if (PC_PreAVMuteStatus == TRUE)
         {
            PC_PreAVMuteStatus = FALSE;
            hdmi_msg("Port(%d) HdmiAVMute is OFF",InputConnector);
            //ToDo: Place code here for AVmute status change

         #if (FEATURE_AUDIO == ENABLE)
            SYS_API_AUDIO_HDMI_Clear_AVMUTE();
         #endif
         }

      }
#endif

      //hdmi_msg("@@@@PC_Event =0x%x",PC_Event);
      if  (PC_Event & STHDMIRX_SIGNAL_STATUS_CHANGE_EVT)
      {
         STHDMIRX_GetEventData(PC_DevHandle, STHDMIRX_SIGNAL_STATUS_CHANGE_EVT, /*EvtData*/&PC_SignalStatusEvtData) ;
         if (PC_SignalStatusEvtData.Status == STHDMIRX_SIGNAL_STATUS_PRESENT)
         {
            // Clear input status after Hdmi Lib returns stable
            // Just avoid getting incorrect input status. No issue fixed.
            if (Channel == CH_A)
      			gm_WriteRegWord(MAIN_INPUT_STATUS, IMP_STATUS_MASK);
      		else
      			gm_WriteRegWord(PIP_INPUT_STATUS, IPP_STATUS_MASK);
            
            STHDMIRX_GetEventData(PC_DevHandle, STHDMIRX_VIDEO_PROPERTY_CHANGE_EVT,&PC_VideoPropertyEvtData);
            #if (DEBUG_HDMI_PRINT_TIMING_DISPLAY_TIME!=0)
                gm_Print(" -->SigP: T(%d)",(WORD)(gm_GetSystemTime()-GetDebugDisplayTime()));
            #endif 

            #if (DEBUG_DISPLAY_TIME_MEASUREMENT == ENABLE)
                dtm_msg("DTM> HDMI_SIG_Stable_T(%Ld)",gm_GetSystemTime());
            #endif
            
            hdmi_msg("############## STHDMIRX_SIGNAL_STATUS_PRESENT",0);
            //hdmi_msg("ScanType      :%d",PC_SignalStatusEvtData.Property.Timing.ScanType);
            hdmi_msg("PixelClock    :%LdHz",PC_SignalStatusEvtData.Property.Timing.PixelClockHz);
            //hdmi_msg("HSyncPolarity :%d",PC_SignalStatusEvtData.Property.Timing.HSyncPolarity);
            //hdmi_msg("HActiveStart  :%d",PC_SignalStatusEvtData.Property.Timing.HActiveStart);
            hdmi_msg("HActive       :%d",PC_SignalStatusEvtData.Property.Timing.HActive);
            //hdmi_msg("HTotal        :%d",PC_SignalStatusEvtData.Property.Timing.HTotal);
            //hdmi_msg("VSyncPolarity :%d",PC_SignalStatusEvtData.Property.Timing.VSyncPolarity);
            //hdmi_msg("VActiveStart  :%d",PC_SignalStatusEvtData.Property.Timing.VActiveStart);
            hdmi_msg("VActive       :%d",PC_SignalStatusEvtData.Property.Timing.VActive);
            //hdmi_msg("VTotal        :%d",PC_SignalStatusEvtData.Property.Timing.VTotal);
            #if 0 //(REINIT_VDDS_MULDIV_WR_IN_UNSTABLE !=0)
            if(PC_InitParams.DeviceType=STHDMIRX_DEVICE_TYPE_ATHENA_HDMI)
            {
               if((PC_SignalStatusEvtData.Property.Timing.HActive ==0) || (PC_SignalStatusEvtData.Property.Timing.VActive==0))
               {
                  gm_Print("***ReInit AVDDS_MULDIV_WR",0);
                  if(Channel==gmvb_MainChannel)
                     gm_WriteRegByte(AVDDS1_INIT,AVDDS1_MULDIV_WR);
                  else
                     gm_WriteRegByte(AVDDS2_INIT,AVDDS2_MULDIV_WR);
               }
            }
            #endif			

            STHDMIRX_StartAudio(PC_InputHandle);
            PC_SignalDataLoaded = TRUE;
            PC_VideoDataLoaded  = TRUE;
            PC_ReDoModeSetup    = TRUE;

         #if (FEATURE_3D==ENABLE)
            if (PC_SignalStatusEvtData.Property.SignalType == STHDMIRX_SINGAL_TYPE_HDMI)
               if((PC_VideoPropertyEvtData.Property.Video3D == TRUE) && (PC_InputConnector==gB_CurrentInputConnector[CH_A]))
                  gm_3D_ModeDetermineByFrontEnd();
         #endif

         #if (SCAN_TYPE_RECHECK !=0)
            {
               WORD BaseAddr= (WORD)(PC_InitParams.DeviceBaseAddress_p)+HDRX_IFM_INSTRUMENT_PU_ADDRS_OFFSET;
               gm_WriteRegWord(BaseAddr+HDMI_RX_INPUT_IRQ_STATUS, 0xFF);
            }
         #endif

         #if (HDMI_PKT_ERROR_HPD !=0)
               gm_TimerStart(bSigStableTimer,PKT_ERR_HPD_WAIT_TIME);
               hdmi_msg(">>>>>>>>>>>>>Sig Stable(T%Ld)",gm_GetSystemTime());
         #endif
         #if HDMI_1ST_MODESETUP_FB || DVI_1ST_MODESETUP_FB
            if((PC_FirstModeSetupFB==TRUE)&&(PC_PerformInstReset==TRUE))
            {
               BYTE bTimer=(Channel==gmvb_MainChannel)? MODESETUP_FB_Main_10ms_TMR:MODESETUP_FB_Pip_10ms_TMR;
               InstantSoftReset(InputConnector);
               gm_TimerStart(bTimer,MODESETUP_FB_10mTime);
               HdmiDviForceBackground(PC_InputConnector, TRUE);
               gm_Print("---->MODESETUP_FB_Start (%ld)",gm_GetSystemTime());
               PC_PerformInstReset=FALSE;
            }
         #endif   

         }

         if (PC_SignalStatusEvtData.Status == STHDMIRX_SINGAL_STATUS_NOT_PRESENT)
         {
            //hdmi_msg("^^^^^^^^^^^^^^ STHDMIRX_SINGAL_STATUS_NOT_PRESENT",0);
            // stop the video processing
            // shutdown audio gracefully
            //	STHDMIRX_StopAudio(PC_InputHandle);
         #if (FEATURE_3D==ENABLE)
            if (PC_InputConnector==gB_CurrentInputConnector[CH_A])
            {
               gm_3D_SetStatusForFrontEndChanged();
               gm_3D_RecoveryTo2D();
            }
         #endif
            PC_SignalDataLoaded = FALSE;
            PC_VideoDataLoaded  = FALSE;
            PC_AudioDataLoaded  = FALSE;
            PC_ReDoModeSetup    = FALSE;

         #if (IBD_CONSISTENCE_CHECK !=0)
            ResetPreIbdValue(Channel);
         #endif            
         }

      }

      if  (PC_Event & STHDMIRX_VIDEO_PROPERTY_CHANGE_EVT)
      {
         STHDMIRX_VideoProperty_t PrevVideoPropertyEvtData;

         hdmi_msg("@@@@@@@@-STHDMIRX_VIDEO_PROPERTY_CHANGE_EVT",0);

         if(gm_TimingIsStable(Channel) == TRUE)
         {  //Only need to perform HdmiRedoModeSetupCheck after modesetup is done.
            _fmemcpy(&PrevVideoPropertyEvtData, &(PC_VideoPropertyEvtData.Property), sizeof(STHDMIRX_VideoPropertyEvtData_t));
         }

         STHDMIRX_GetEventData(PC_DevHandle, STHDMIRX_VIDEO_PROPERTY_CHANGE_EVT,&PC_VideoPropertyEvtData);

         PC_VideoDataLoaded = TRUE;

		/*ChengLin 20130205: Program VEGA CSC Register via HDMI input status(Restore to default value everytime)*/
		#ifdef ST_4K2K_93xx_BOARD
   		if(GetChipID_4K2K()==CHIPID_4K2K_FE)
   		{
			#if (FEATURE_HDMI == ENABLE && ENABLE_VEGA_CONTROL_VIA_ATHENA_FE && ENABLE_VEGA_CSC_VIA_HDMI_INFO)
			SOCIPC_VEGA_CSC_Via_HDMI_Input(FALSE); //Default to Scaling Mode
			#endif
   		}
		#endif
		
         if(gm_TimingIsStable(Channel) == TRUE)
         {  //Only need to perform HdmiRedoModeSetupCheck after modesetup is done.
            PC_ReDoModeSetup=HdmiRedoModeSetupCheck(&PrevVideoPropertyEvtData, &(PC_VideoPropertyEvtData.Property));
         }
      }

      if (PC_Event & STHDMIRX_AUDIO_PROPERTY_CHANGE_EVT)
      {
         //hdmi_msg("@@@@-STHDMIRX_AUDIO_PROPERTY_CHANGE_EVT",0);
         STHDMIRX_GetEventData(PC_DevHandle, STHDMIRX_AUDIO_PROPERTY_CHANGE_EVT, &PC_AudioPropertyEvtData);

         PC_AudioDataLoaded = TRUE;

      #if (FEATURE_AUDIO == ENABLE)
	  #ifdef DELL_U2410F		//Neil120919+ 
		if(IsAudioOnCurrentPort(ALL_BOARDS_HDMI))
	  	{
	  		AUDIO_MUTE();
			//if(B_PropertyChangeCNT > 2)
        	{
			//	B_PropertyChangeCNT = 0;
				//gm_Print("B_PropertyChangeCNT  %d", B_PropertyChangeCNT);
         SYS_API_AUDIO_HDMI_Set_PropertyChange();
			}
		//	B_PropertyChangeCNT ++;
		}
	  #endif
      #endif

         if ((PC_SignalDataLoaded == TRUE) && (PC_SignalStatusEvtData.Status == STHDMIRX_SIGNAL_STATUS_PRESENT))
         {
            STHDMIRX_StartAudio(PC_InputHandle);
         }

         // Copy event data to AudioPropertyEvtData
         // Identify the change in property
         // Reconfigure the audio processing pipe
         // May have to stop and start audio based on the required reconfiguration
      }

      if (PC_Event&STHDMIRX_INFORMATION_VSINFO_EVT)
      {
         STHDMIRX_GetEventData(PC_DevHandle,STHDMIRX_INFORMATION_VSINFO_EVT, &PC_InfoUpdateEvtData);
         //gm_Print("STHDMIRX_INFORMATION_VSINFO_EVT",0);
         #if (BACKUP_INFODATA == 1)
         _fmemcpy(&PC_VSInfo, &PC_InfoUpdateEvtData.Type.VSInfo, sizeof(STHDMIRX_VSInfo_t));
         #endif

      }
      if (PC_Event&STHDMIRX_INFORMATION_SPDINFO_EVT)
      {
         STHDMIRX_GetEventData(PC_DevHandle,STHDMIRX_INFORMATION_SPDINFO_EVT,&PC_InfoUpdateEvtData);
         //gm_Print("STHDMIRX_INFORMATION_SPDINFO_EVT",0);
         #if (BACKUP_INFODATA == 1)
         _fmemcpy(&PC_SPDInfo, &PC_InfoUpdateEvtData.Type.SPDInfo, sizeof(STHDMIRX_SPDInfo_t));
         #endif

      }
      if (PC_Event&STHDMIRX_INFORMATION_MPEGSOURCEINFO_EVT)
      {
         STHDMIRX_GetEventData(PC_DevHandle,STHDMIRX_INFORMATION_MPEGSOURCEINFO_EVT,&PC_InfoUpdateEvtData);
         //gm_Print("STHDMIRX_INFORMATION_MPEGSOURCEINFO_EVT",0);
         #if (BACKUP_INFODATA == 1)
         _fmemcpy(&PC_MPEGInfo, &PC_InfoUpdateEvtData.Type.MPEGInfo, sizeof(STHDMIRX_MPEGSourceInfo_t));
         #endif

      }
      if (PC_Event&STHDMIRX_INFORMATION_ISRCINFO_EVT)
      {
         STHDMIRX_GetEventData(PC_DevHandle,STHDMIRX_INFORMATION_ISRCINFO_EVT,&PC_InfoUpdateEvtData);
         //gm_Print("STHDMIRX_INFORMATION_ISRCINFO_EVT",0);
         #if (BACKUP_INFODATA == 1)
         _fmemcpy(&PC_ISRCInfo, &PC_InfoUpdateEvtData.Type.ISRCInfo, sizeof(STHDMIRX_ISRCInfo_t));
         #endif

      }
      if (PC_Event&STHDMIRX_INFORMATION_ACPINFO_EVT)
      {
         STHDMIRX_GetEventData(PC_DevHandle,STHDMIRX_INFORMATION_ACPINFO_EVT,&PC_InfoUpdateEvtData);
         //gm_Print("STHDMIRX_INFORMATION_ACPINFO_EVT",0);
         #if (BACKUP_INFODATA == 1)
         _fmemcpy(&PC_ACPInfo, &PC_InfoUpdateEvtData.Type.ACPInfo, sizeof(STHDMIRX_ACPInfo_t));
         #endif
      }
      if (PC_Event&STHDMIRX_INFORMATION_METAINFO_EVT)
      {
         STHDMIRX_GetEventData(PC_DevHandle,STHDMIRX_INFORMATION_METAINFO_EVT,&PC_InfoUpdateEvtData);
         //gm_Print("STHDMIRX_INFORMATION_METAINFO_EVT",0);
         #if (BACKUP_INFODATA == 1)
         _fmemcpy(&PC_GBDInfo, &PC_InfoUpdateEvtData.Type.GBDInfo, sizeof(STHDMIRX_GBDInfo_t));
         #endif
      }
      /*    //Not supported by HdmirxLib
            if(PC_Event&STHDMIRX_INFORMATION_AUDIOINFO_EVT)
            {
               STHDMIRX_GetEventData(PC_DevHandle,STHDMIRX_INFORMATION_AUDIOINFO_EVT,EvtData);
            }
            if(PC_Event&STHDMIRX_INFORMATION_ACSINFO_EVT)
            {
               STHDMIRX_GetEventData(PC_DevHandle,STHDMIRX_INFORMATION_ACSINFO_EVT,EvtData);
            }
            if(PC_Event&STHDMIRX_INFORMATION_ACRINFO_EVT)
            {
               STHDMIRX_GetEventData(PC_DevHandle,STHDMIRX_INFORMATION_ACRINFO_EVT,EvtData);
            }
            if(PC_Event&STHDMIRX_INFORMATION_GCPINFO_EVT)
            {
               STHDMIRX_GetEventData(PC_DevHandle,STHDMIRX_INFORMATION_GCPINFO_EVT,EvtData);
            }
      */

      if((PC_VideoDataLoaded == TRUE) && (PC_SignalDataLoaded==TRUE))
      {
         if(PC_ReDoModeSetup == TRUE) 
         {
         #if (FEATURE_3D==ENABLE)
            if((PC_VideoPropertyEvtData.Property.Video3D == TRUE) && (PC_SignalStatusEvtData.Property.SignalType == STHDMIRX_SINGAL_TYPE_HDMI))
            {
               hdmi_msg("<<ForceModeChanged by 3D>>",0);
               gm_3D_ModeDetermineByFrontEnd();
               gm_ForceModeChanged(CH_A);
            }
            else
         #endif   
         {
			//Neil120614* don't send request if already modedetermine finish, need further check
            if((gm_TimingIsStable(Channel) == TRUE)&&!(stInput[Channel].ModeFlags &(gmd_MODE_VALID|gmd_MODE_OUT_RANGE)))
               {
                  ModeSetupRequestCancel(Channel, REQ_MS_SETUP, PARAM_NONE);
                  ModeSetupRequest(Channel, REQ_MS_RESTART, PARAM_NONE, 0);
                  hdmi_msg("<<REQ_MS_RESTART>>",0);
               }
            }
            PC_ReDoModeSetup=FALSE;
         }
         #if (HDMI_PKT_ERROR_HPD !=0)
            if(PC_InitParams.DeviceType==STHDMIRX_DEVICE_TYPE_ATHENA_HDMI)
            {
               if (gm_TimingIsStable(Channel) != TRUE)
               {
                  if (gm_ReadRegWord(HDRX_SDP_MUTE_SRC_STATUS) & HDRX_AV_MUTE_SRC_PACK_NOISE)
                  {
                     hdmi_msg("..........PKT_ErrDetcted(T%Ld)",gm_GetSystemTime());
                     if((gm_TimerCheck(bSigStableTimer) !=TIMER_OK) && (HdmiHpdControl[Channel].IsActivated==FALSE))
                     {
                        HdmiSetHpdPulseStart(Channel, PC_InputConnector, HPD_PULSE_WIDTH_10mTime /* x 10ms */);
                        hdmi_msg("@@@@@@@@PKT_HPD",0);

                     }
                  }
               }
            }
         #endif
      }
   }
#if HDMI_1ST_MODESETUP_FB || DVI_1ST_MODESETUP_FB
   if ((PC_FirstModeSetupFB==TRUE)&&(PC_PerformInstReset!=TRUE))
   {
      BYTE bTimer=(Channel==gmvb_MainChannel)? MODESETUP_FB_Main_10ms_TMR:MODESETUP_FB_Pip_10ms_TMR;
      //InstantSoftReset(InputConnector);
      //gm_Print("***1stMS_FB(%d)",PC_InputConnector);
      if(gm_TimerCheck(bTimer)!=TIMER_OK)
      {
         HdmiDviForceBackground(PC_InputConnector, FALSE);
         PC_FirstModeSetupFB=FALSE;
         gm_Print("###1stMS_FB_TOUT(%d)",PC_InputConnector);
         gm_Print("----< MODESETUP_FB_Stop(%ld)",gm_GetSystemTime());
         
      }
/*      else if(((GetHdcpStatus(PC_InputConnector) & 3)==3) && (PC_SignalStatusEvtData.Status == STHDMIRX_SIGNAL_STATUS_PRESENT))
      {
         HdmiDviForceBackground(PC_InputConnector, FALSE);
         PC_FirstModeSetupFB=FALSE;
         gm_Print("@@@1stMS_FB_AUTH(%d)",PC_InputConnector);
         gm_Print("----< MODESETUP_FB_Stop(%ld)",gm_GetSystemTime());
      } */
   }
#endif
//	gm_Print("HdmiDviInputPortHandler END",0);
}



//******************************************************************************
// FUNCTION       : void HdmiDviHandler(void)
// USAGE          : This function calls hdmi library function, and monitors
//                  hdmi connection.
// INPUT          : None
// OUTPUT         : None
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  :
// POST_CONDITION :
//******************************************************************************
void HdmiDviHandler(void)
{
   BYTE b_InputConnector;
   BYTE b_HdmiDviInNo;
   // gmt_HdmiDviInputPort* ptr_PortConfig;

   //Update Cable status

   //Main Channel
   HdmiDviInputPortHandler(gmvb_MainChannel, gB_CurrentInputConnector[gmvb_MainChannel]);

   //Pip Channel
   HdmiDviInputPortHandler(gmvb_PipChannel, gB_CurrentInputConnector[gmvb_PipChannel]);

}


gmt_CEA861_AVI_Y gm_HdmiGetAviColorSpace(BYTE InputConnector)
{
   gmt_HdmiDviInputPort* ptr_PortConfig=&HdmiDviInPortConfig[GetInPortConfigArrayLocation(InputConnector)];
   //STHDMIRX_GetEventData(PC_DevHandle, STHDMIRX_VIDEO_PROPERTY_CHANGE_EVT,&PC_VideoPropertyEvtData);
   if (PC_VideoDataLoaded==TRUE)
   {
      switch (PC_VideoPropertyEvtData.Property.ColorFormat)
      {
         case STHDMIRX_COLOR_FORMAT_YUV_422:
            return gmd_CEA861_AVI_Y_YUV422;

         case STHDMIRX_COLOR_FORMAT_YUV_444:
            return gmd_CEA861_AVI_Y_YUV444;
      }
   }
   else
   {
      hdmi_msg("### gm_HdmiGetAviColorSpace:Data not loaded!",0);
   }
   return gmd_CEA861_AVI_Y_RGB;
}



BOOL gm_HdmiIsStableState(BYTE InputConnector)
{
   gmt_HdmiDviInputPort* ptr_PortConfig=&HdmiDviInPortConfig[GetInPortConfigArrayLocation(InputConnector)];

      if (PC_SignalDataLoaded == TRUE)
      {
         if (PC_SignalStatusEvtData.Status == STHDMIRX_SIGNAL_STATUS_PRESENT)
         {
            
         #if (DUALDVI_HDCP_MODE_DET_DELAY==1)
            if(PC_InitParams.PhyBaseAddress_p2 != NULL)
            {  //Dual DVI port
              if((gm_ReadRegWord(DUAL_DVI0_CTRL)==9) && (gm_ReadRegWord(DVI0_HDCP_STATUS_EXT) & DVI0_HDCP_I2C_IRQ_STATUS))
              {
                  if(b_DualDviDelayCont.FlagSet==0)
                  {
                     b_DualDviDelayCont.FlagSet=1;
                     b_DualDviDelayCont.Count=DUALDVI_HDCP_DELAY_COUNT;
                  }
              }
            }
         #endif
            return TRUE;
         }
      }
      #if (DUALDVI_HDCP_MODE_DET_DELAY==1)
      b_DualDviDelayCont.Count=0;
      b_DualDviDelayCont.FlagSet=0;
      #endif
      return FALSE;
}
// ifdef HDMI_DVI_TIMING_LOSS_PATCH
BOOL gm_HdmiIsTmingMatched(gmt_PHY_CH Channel, gmt_FrontEndSignalTiming_t* Ptr_Timing)
{
   BOOL ret=FALSE;
   if (HdmiDviGetSignalTimingExt(gB_CurrentInputConnector[Channel], Ptr_Timing))
   {
      gmt_IbdStruct ibd;

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
      #if (IBD_CONSISTENCE_CHECK !=0)
      if(ret==TRUE)
         ret=HdmiDviConsistenceCheck(Channel, &ibd);
      #endif

      
      #if (DUALDVI_HDCP_MODE_DET_DELAY==1)
      if(b_DualDviDelayCont.Count)
      {
         b_DualDviDelayCont.Count--;
         //gm_Print("D%d",b_DualDviDelayCont.Count);
         ret=FALSE;
      }
      #endif

      #if (HDMI_PKT_ERROR_HPD != 0)
      {
         gmt_HdmiDviInputPort* ptr_PortConfig=&HdmiDviInPortConfig[GetInPortConfigArrayLocation(gB_CurrentInputConnector[Channel])];
         if(PC_InitParams.DeviceType==STHDMIRX_DEVICE_TYPE_ATHENA_HDMI)
         {
            if (gm_ReadRegWord(HDRX_SDP_MUTE_SRC_STATUS) & HDRX_AV_MUTE_SRC_PACK_NOISE)
            {
               ret=FALSE;
            }
         }
      }
      #endif            
   }
   //hdmi_msg_ii("P(%d)-TM(%d)",gB_CurrentInputConnector[Channel],ret);
   return ret;
}


BOOL gm_IsHdmi3DTiming(BYTE InputConnector)
{
   gmt_HdmiDviInputPort* ptr_PortConfig=&HdmiDviInPortConfig[GetInPortConfigArrayLocation(InputConnector)];

   //STHDMIRX_GetEventData(PC_DevHandle, STHDMIRX_SIGNAL_STATUS_CHANGE_EVT, &PC_SignalStatusEvtData) ;
   if ((PC_SignalDataLoaded == TRUE) && (PC_VideoDataLoaded == TRUE))
   {
      if ((PC_SignalStatusEvtData.Property.SignalType == STHDMIRX_SINGAL_TYPE_HDMI) &&
            (PC_VideoPropertyEvtData.Property.Video3D== TRUE))
         return TRUE;
   }
   else
   {
      hdmi_msg("### gm_IsHdmi3DTiming:Data not loaded!",0);
   }
   return FALSE;

}

BOOL gm_IsHdmiTiming(BYTE InputConnector)
{
   gmt_HdmiDviInputPort* ptr_PortConfig=&HdmiDviInPortConfig[GetInPortConfigArrayLocation(InputConnector)];

   //STHDMIRX_GetEventData(PC_DevHandle, STHDMIRX_SIGNAL_STATUS_CHANGE_EVT, &PC_SignalStatusEvtData) ;
   if (PC_SignalDataLoaded == TRUE)
   {

      if ((PC_SignalStatusEvtData.Property.SignalType == STHDMIRX_SINGAL_TYPE_HDMI)
	  	||(gm_ReadRegWord(HDRX_MAIN_LINK_STATUS)&HDRX_HDMI_SIGN_DETECTED) == HDRX_HDMI_SIGN_DETECTED)	//Neil120913+ Don't konw why PC_SignalStatusEvtData.Property.SignalType set to 0 (DVI) actually HDMI
         return TRUE;
   }
   else
   {
      hdmi_msg("### gm_IsHdmiTiming:Data not loaded!",0);
   }
   return FALSE;
}


gmt_3D_INPUT_FORMAT_TYPE gm_HdmiGet3DFormat(BYTE InputConnector)
{
   gmt_HdmiDviInputPort* ptr_PortConfig=&HdmiDviInPortConfig[GetInPortConfigArrayLocation(InputConnector)];
   //STHDMIRX_GetEventData(PC_DevHandle, STHDMIRX_VIDEO_PROPERTY_CHANGE_EVT,&PC_VideoPropertyEvtData);
   if (PC_VideoDataLoaded==TRUE)
   {
      switch (PC_VideoPropertyEvtData.Property.VideoProperty3D.Format)
      {
         case	STHDMIRX_3D_FORMAT_FRAME_PACK:
            return gmd_3D_IN_FRAME_PACKING;

         case	STHDMIRX_3D_FORMAT_FIELD_ALT:
            return gmd_3D_IN_FIELD_ALTERNATIVE;

         case	STHDMIRX_3D_FORMAT_LINE_ALT:
            return gmd_3D_IN_LINE_ALTERNATIVE;

         case	STHDMIRX_3D_FORMAT_SBYS_FULL:
            return gmd_3D_IN_SIDE_BY_SIDE_FULL;

         case	STHDMIRX_3D_FORMAT_SBYS_HALF:
            return gmd_3D_IN_SIDE_BY_SIDE_HALF;

         case	STHDMIRX_3D_FORMAT_TOP_AND_BOTTOM:
            return gmd_3D_IN_TOP_AND_BOTTOM;

//			case	STHDMIRX_3D_FORMAT_L_D:
//			case	STHDMIRX_3D_FORMAT_L_D_G_GMINUSD:
//				return gmd_3D_IN_STACKED_TOP_BOTTOM;

      }

   }
   else
   {
      hdmi_msg("### HDMI_Get3DFormat:Data not loaded!",0);
   }
   return gmd_3D_IN_UNKNOWN;
}


gmt_COLOR_DEPTH gm_HdmiGetColorDepth(BYTE InputConnector)
{
   gmt_HdmiDviInputPort* ptr_PortConfig=&HdmiDviInPortConfig[GetInPortConfigArrayLocation(InputConnector)];
   //STHDMIRX_GetEventData(PC_DevHandle, STHDMIRX_VIDEO_PROPERTY_CHANGE_EVT,&PC_VideoPropertyEvtData);
   if (PC_VideoDataLoaded==TRUE)
   {
      switch (PC_VideoPropertyEvtData.Property.ColorDepth)
      {
         case STHDMIRX_COLOR_DEPTH_24BPP:
            return gm_COLOR_DEPTH_24BPP;

         case STHDMIRX_COLOR_DEPTH_30BPP:
            return gm_COLOR_DEPTH_30BPP;

         case STHDMIRX_COLOR_DEPTH_36BPP:
            return gm_COLOR_DEPTH_36BPP;

         case STHDMIRX_COLOR_DEPTH_48BPP:
            return gm_COLOR_DEPTH_48BPP;

      }
   }
   else
   {
      hdmi_msg("### HDMI_GetColorDepth:Data not loaded!",0);
   }
   return gm_COLOR_DEPTH_UNKNOWN;
}

gmt_RGB_Quantization gm_HdmiGetRGBQuanRange(BYTE InputConnector)
{
   gmt_HdmiDviInputPort* ptr_PortConfig=&HdmiDviInPortConfig[GetInPortConfigArrayLocation(InputConnector)];

   if (PC_VideoDataLoaded == TRUE)
   {
      switch (PC_VideoPropertyEvtData.Property.RGBQuantRange)
      {
         case STHDMIRX_RGB_QUANT_RANGE_DEFAULT:
            return gmd_RGB_QUANTIZATION_DEFAULT;

         case STHDMIRX_RGB_QUANT_RANGE_LIMITED:
            return gmd_RGB_QUANTIZATION_LIMITED;

         case STHDMIRX_RGB_QUANT_RANGE_FULL:
            return gmd_RGB_QUANTIZATION_FULL;

         default:
            return gmd_RGB_QUANTIZATION_UNKNOW;
      }
   }
   else
   {
      hdmi_msg("### gm_HdmiGetRGBQuanRange:Data not loaded!",0);
   }
   return gmd_RGB_QUANTIZATION_UNKNOW;
}

gmt_YCC_Quantization gm_HdmiGetYCCQuanRange(BYTE InputConnector)
{
   gmt_HdmiDviInputPort* ptr_PortConfig=&HdmiDviInPortConfig[GetInPortConfigArrayLocation(InputConnector)];

   if (PC_VideoDataLoaded == TRUE)
   {
      switch (PC_VideoPropertyEvtData.Property.YCQuantRange)
      {
         case STHDMIRX_YC_QUANT_RANGE_LIMITED:
            return gmd_YCC_QUANTIZATION_LIMITED;

         case STHDMIRX_YC_QUANT_RANGE_FULL:
            return gmd_YCC_QUANTIZATION_FULL;

         default:
            return gmd_YCC_QUANTIZATION_UNKNOW;
      }
   }
   else
   {
      hdmi_msg("### gm_HdmiGetYCCQuanRange:Data not loaded!",0);
   }
   return gmd_YCC_QUANTIZATION_UNKNOW;
}

/*
BOOL IsHdmiDviLibReportStable(BYTE InputConnector)
{
	return gm_HdmiIsStableState(InputConnector);
}
*/

BYTE gm_HdmiGetAviPixelRepetition(BYTE InputConnector)
{
   gmt_HdmiDviInputPort* ptr_PortConfig=&HdmiDviInPortConfig[GetInPortConfigArrayLocation(InputConnector)];

   //STHDMIRX_GetEventData(PC_DevHandle, STHDMIRX_VIDEO_PROPERTY_CHANGE_EVT,&PC_VideoPropertyEvtData);
   if (PC_VideoDataLoaded == TRUE)
      return PC_VideoPropertyEvtData.Property.PixelRepeatFactor;

   hdmi_msg("### gm_HdmiGetAviPixelRepetition:Data not loaded!",0);
   return STHDMIRX_PIXEL_REPEAT_FACTOR_1;
}


gmt_CEA861_AVI_S gm_HdmiGetAviScanInfo(BYTE InputConnector)
{
   gmt_HdmiDviInputPort* ptr_PortConfig=&HdmiDviInPortConfig[GetInPortConfigArrayLocation(InputConnector)];

   //STHDMIRX_GetEventData(PC_DevHandle, STHDMIRX_VIDEO_PROPERTY_CHANGE_EVT,&PC_VideoPropertyEvtData);
   if (PC_VideoDataLoaded == TRUE)
      return PC_VideoPropertyEvtData.Property.ScanInfo;

   hdmi_msg("### gm_HdmiGetAviScanInfo:Data not loaded!",0);
   return STHDMIRX_SCAN_INFO_NO_DATA;

}


STHDMIRX_ColorimetryStd_t HdmiGetColorimetry(BYTE InputConnector)
{
   gmt_HdmiDviInputPort* ptr_PortConfig=&HdmiDviInPortConfig[GetInPortConfigArrayLocation(InputConnector)];

   //STHDMIRX_GetEventData(PC_DevHandle, STHDMIRX_VIDEO_PROPERTY_CHANGE_EVT,&PC_VideoPropertyEvtData);

   if (PC_VideoDataLoaded == TRUE)
      return PC_VideoPropertyEvtData.Property.Colorimetery;

   hdmi_msg("### HdmiGetColorimetry:Data not loaded!",0);

   return STHDMIRX_COLORIMETRY_STD_DEFAULT;

}

STHDMIRX_RGBQuantRange_t HdmiGetRGBQuantRange(BYTE InputConnector)
{
   gmt_HdmiDviInputPort* ptr_PortConfig=&HdmiDviInPortConfig[GetInPortConfigArrayLocation(InputConnector)];

   //STHDMIRX_GetEventData(PC_DevHandle, STHDMIRX_VIDEO_PROPERTY_CHANGE_EVT,&PC_VideoPropertyEvtData);
   if (PC_VideoDataLoaded == TRUE)
      return PC_VideoPropertyEvtData.Property.RGBQuantRange;

   hdmi_msg("### HdmiGetRGBQuantRange:Data not loaded!",0);
   return STHDMIRX_RGB_QUANT_RANGE_DEFAULT;


}

STHDMIRX_YCQuantRange_t HdmiGetYCQuantRange(BYTE InputConnector)
{
   gmt_HdmiDviInputPort* ptr_PortConfig=&HdmiDviInPortConfig[GetInPortConfigArrayLocation(InputConnector)];

   //STHDMIRX_GetEventData(PC_DevHandle, STHDMIRX_VIDEO_PROPERTY_CHANGE_EVT,&PC_VideoPropertyEvtData);
   if (PC_VideoDataLoaded == TRUE)
      return PC_VideoPropertyEvtData.Property.YCQuantRange;

   hdmi_msg("### HdmiGetYCQuantRange:Data not loaded!",0);
   return STHDMIRX_YC_QUANT_RANGE_LIMITED;


}


BOOL HdmiDviGetSignalTiming(BYTE InputConnector, gmt_FrontEndSignalTiming_t* Ptr_Timing)
{
   gmt_HdmiDviInputPort* ptr_PortConfig=&HdmiDviInPortConfig[GetInPortConfigArrayLocation(InputConnector)];

   //STHDMIRX_GetEventData(PC_DevHandle, STHDMIRX_SIGNAL_STATUS_CHANGE_EVT, &PC_SignalStatusEvtData) ;
   if (PC_SignalDataLoaded == TRUE)
   {
      Ptr_Timing->PixelClockHz = PC_SignalStatusEvtData.Property.Timing.PixelClockHz;

      Ptr_Timing->HActiveStart = PC_SignalStatusEvtData.Property.Timing.HActiveStart;
      Ptr_Timing->HActive      = PC_SignalStatusEvtData.Property.Timing.HActive;
      Ptr_Timing->HTotal       = PC_SignalStatusEvtData.Property.Timing.HTotal;

      Ptr_Timing->VActiveStart = PC_SignalStatusEvtData.Property.Timing.VActiveStart;
      Ptr_Timing->VActive      = PC_SignalStatusEvtData.Property.Timing.VActive;
      Ptr_Timing->VTotal		 = PC_SignalStatusEvtData.Property.Timing.VTotal;

      Ptr_Timing->Interlaced   = PC_SignalStatusEvtData.Property.Timing.ScanType ? FALSE : TRUE;
      Ptr_Timing->HS_Positive  = PC_SignalStatusEvtData.Property.Timing.HSyncPolarity;
      Ptr_Timing->VS_Positive  = PC_SignalStatusEvtData.Property.Timing.VSyncPolarity;
      return TRUE;
   }

   hdmi_msg("### HdmiGetYCQuantRange:Data not loaded!",0);
   return FALSE;


}





BOOL HdmiDviGetSignalTimingExt(BYTE InputConnector, gmt_FrontEndSignalTiming_t* Ptr_Timing)
{
   gmt_HdmiDviInputPort* ptr_PortConfig=&HdmiDviInPortConfig[GetInPortConfigArrayLocation(InputConnector)];
   WORD BaseAddr= (WORD)(PC_InitParams.DeviceBaseAddress_p)+HDRX_IFM_INSTRUMENT_PU_ADDRS_OFFSET;

   //STHDMIRX_GetEventData(PC_DevHandle, STHDMIRX_SIGNAL_STATUS_CHANGE_EVT, &PC_SignalStatusEvtData) ;
   if (PC_SignalDataLoaded == TRUE)
   {
      Ptr_Timing->PixelClockHz = PC_SignalStatusEvtData.Property.Timing.PixelClockHz;
      
   #if (SCAN_TYPE_RECHECK ==0)
      Ptr_Timing->Interlaced   = PC_SignalStatusEvtData.Property.Timing.ScanType ? FALSE : TRUE;
   #else
      Ptr_Timing->Interlaced   = (gm_ReadRegWord(BaseAddr+HDMI_RX_INPUT_IRQ_STATUS) & HDMI_INPUT_INTLC_ERR)? FALSE : TRUE;
      if(Ptr_Timing->Interlaced == PC_SignalStatusEvtData.Property.Timing.ScanType)
      {
         //gm_Print("*** Updated ScanType",0);
         PC_SignalStatusEvtData.Property.Timing.ScanType=PC_SignalStatusEvtData.Property.Timing.ScanType ? FALSE : TRUE;
      }

   #endif

      Ptr_Timing->HS_Positive  = PC_SignalStatusEvtData.Property.Timing.HSyncPolarity;
      Ptr_Timing->VS_Positive  = PC_SignalStatusEvtData.Property.Timing.VSyncPolarity;

      Ptr_Timing->HActiveStart = gm_ReadRegWord(BaseAddr+HDMI_RX_INPUT_IBD_HSTART); 
      Ptr_Timing->HActive      = gm_ReadRegWord(BaseAddr+HDMI_RX_INPUT_IBD_HWIDTH);
      Ptr_Timing->HTotal       = gm_ReadRegWord(BaseAddr+HDMI_RX_INPUT_IBD_HTOTAL);

      Ptr_Timing->VActiveStart = gm_ReadRegWord(BaseAddr+HDMI_RX_INPUT_IBD_VSTART);
      Ptr_Timing->VActive      = gm_ReadRegWord(BaseAddr+HDMI_RX_INPUT_IBD_VLENGTH);
      Ptr_Timing->VTotal		 = gm_ReadRegWord(BaseAddr+HDMI_RX_INPUT_IBD_VTOTAL);

      if(PC_InitParams.PhyBaseAddress_p2!=NULL)
      {  //Dual DVI
         if(gm_ReadRegWord(DUAL_DVI0_CTRL) & DUAL_DVI0_DUAL_TO_SINGL)
         {
             Ptr_Timing->HActive *=2;
             Ptr_Timing->HTotal  *=2;
             Ptr_Timing->HActiveStart  *=2;
         }
      }
		//Neil120719+ for VG870 1075 timing fake 1080i output, deal with 1080i as U2410
		#ifdef DELL_U2410F
		if(Ptr_Timing->Interlaced != 1)
		{	
			if ((abs(Ptr_Timing->VActive - 540)< 4)&(abs(Ptr_Timing->HActive - 1920)< 4)
				&&((abs(Ptr_Timing->HTotal - 0x900)< 4)||(abs(Ptr_Timing->HTotal - 0x898)< 4)
				||(abs(Ptr_Timing->HTotal - 0xA50)< 4)))
				Ptr_Timing->Interlaced = 1;
		}
	  	#endif
      return TRUE;
   }

   hdmi_msg("### HdmiGetYCQuantRange:Data not loaded!",0);
   return FALSE;
}


BOOL HdmiDviGetAudioProperty(BYTE InputConnector, STHDMIRX_AudioProperty_t* pAudioProp)
{
   gmt_HdmiDviInputPort* ptr_PortConfig=&HdmiDviInPortConfig[GetInPortConfigArrayLocation(InputConnector)];

   if (PC_AudioDataLoaded == TRUE)
   {
      _fmemcpy(pAudioProp, &(PC_AudioPropertyEvtData.Property), sizeof(STHDMIRX_AudioProperty_t));
      return TRUE;
   }
   return FALSE;
}


#if (LINK_CLOCK_CHECK_SUPPORT ==1) 
BOOL HdmiDviIsLinkClockPresent(BYTE InputConnector)
{
   U16 Difference[3];
   WORD BaseAddr;
   gmt_HdmiDviInputPort* ptr_PortConfig=&HdmiDviInPortConfig[GetInPortConfigArrayLocation(InputConnector)];

   BaseAddr=(WORD)PC_InitParams.DeviceBaseAddress_p;

   if(gm_ReadRegWord(BaseAddr+HDMI_RX_MEAS_IRQ_STS) & HDRX_MEAS_CLK_IRQ_STS)
   {
      gm_ClearRegBitsByte(BaseAddr+HDMI_RX_SIGNL_DET_CTRL, HDRX_MEAS_RD_MD);
      PC_LinkClockCnt[PC_LinkClockCntIndex] = gm_ReadRegWord(BaseAddr+HDMI_RX_MEAS_RESULT);
      PC_LinkClockCntIndex++;
      PC_LinkClockCntIndex %= 3;
   }
   Difference[0] = ABS(PC_LinkClockCnt[0], PC_LinkClockCnt[1]);
   Difference[1] = ABS(PC_LinkClockCnt[0], PC_LinkClockCnt[2]);
   Difference[2] = ABS(PC_LinkClockCnt[1], PC_LinkClockCnt[2]);
   if(PC_LinkClockCnt[0] >= (DVI_MEASUREMENTS_PERIOD*DVI_CLOCK_THRESH)) 
   {
      if (((PC_IsLinkClockPresented == FALSE) &&(Difference[0] <=5) && (Difference[1]<=5))
          ||((PC_IsLinkClockPresented == TRUE)&& ((Difference[0] <=5) || (Difference[1] <=5) || (Difference[2] <=5))))
      {
         PC_IsLinkClockPresented = TRUE;
         return TRUE;
      }
   }
   PC_IsLinkClockPresented = FALSE;
   return FALSE;
}
#endif


BOOL HdmiGetAudioChannelStatus(BYTE InputConnector,  STHDMIRX_AudioChannelStatus_t*	ChannelStatus)
{
   ST_ErrorCode_t ErrorCode;
   gmt_HdmiDviInputPort* ptr_PortConfig=&HdmiDviInPortConfig[GetInPortConfigArrayLocation(InputConnector)];
   
   ErrorCode=STHDMIRX_GetAudioChannelStatus(PC_InputHandle, ChannelStatus);
   if(ErrorCode != ST_NO_ERROR)
      return FALSE;
   return TRUE;   
}


void HdmiDviHpdControl(BYTE InputConnector, BYTE b_Set)
{
   BYTE b_HdmiDviInNo=GetInPortConfigArrayLocation(InputConnector);
   if(b_HdmiDviInNo >= NUM_OF_HDMIDVI_INP)
   {
      gm_Print("HdmiDviHpdControl: Incorrect Port Number!",0);
   }
   else
   {
      gmt_HdmiDviInputPort* ptr_PortConfig=&HdmiDviInPortConfig[b_HdmiDviInNo];
      if(b_Set==0)
      {
         if(PC_InitParams.ClearHPD_pf!=NULL_PTR)
            (PC_InitParams.ClearHPD_pf)();

         #if TURN_OFF_CPHY_IN_HPD_LOW
            gm_SetRegBitsWord((WORD)(PC_InitParams.BaseAddress_p),0x3);
            gm_SetRegBitsWord((WORD)(PC_InitParams.PhyBaseAddress_p) + CPHY_TEST_CTRL ,CPHY1_IMBGEN_PD);
            if(PC_InitParams.PhyBaseAddress_p2 != NULL) //  CPHY2_AUX_PHY_CTRL
               gm_SetRegBitsWord((WORD)(PC_InitParams.PhyBaseAddress_p2) + CPHY_TEST_CTRL ,CPHY1_IMBGEN_PD);
            #endif   
      }
      else
      {
         #if 1 
            gm_ClearRegBitsWord((WORD)(PC_InitParams.BaseAddress_p),0x3);
            gm_ClearRegBitsWord((WORD)(PC_InitParams.PhyBaseAddress_p) + CPHY_TEST_CTRL ,CPHY1_IMBGEN_PD);
            if(PC_InitParams.PhyBaseAddress_p2 != NULL) //  CPHY2_AUX_PHY_CTRL
               gm_ClearRegBitsWord((WORD)(PC_InitParams.PhyBaseAddress_p2) + CPHY_TEST_CTRL ,CPHY1_IMBGEN_PD);
         #endif
         if(PC_InitParams.SetHPD_pf!=NULL_PTR)
            (PC_InitParams.SetHPD_pf)();
      }
   }
}

void GeneralHdmiDviPowerUp(void)
{
   //ToDo: General Power up for HdmiDvi Block

}

void GeneralHdmiDviPowerDown(void)
{
   //ToDo: General Power down for HdmiDvi Block

}


#ifdef INACTIVE_DEVICE_POWER_SAVING
//******************************************************************************
// FUNCTION       : void HdmiDviPowrDownInputPort(BYTE InputConnector)
// USAGE          : Use this function to power down HdmiDvi block for power 
//                  saving.
// INPUT          : None
// OUTPUT         : None
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  :
// POST_CONDITION :
//******************************************************************************



void HdmiDviPowrDownInputPort(BYTE InputConnector)
{
   BYTE b_HdmiDviInNo = GetInPortConfigArrayLocation(InputConnector);
   gmt_HdmiDviInputPort* ptr_PortConfig;
   
   if (b_HdmiDviInNo >= NUM_OF_HDMIDVI_INP)
   {
      return;
   }
   ptr_PortConfig= &HdmiDviInPortConfig[b_HdmiDviInNo];
   //Power down digital block
   gm_SetRegBitsWord((WORD)(PC_InitParams.DeviceBaseAddress_p) + HDMIDVI_RX_PHY_CTRL,HDMI_RX_PHY_PWR_CTRL);
  
   //Power down PHY 
   gm_SetRegBitsDWord((WORD)(PC_InitParams.PhyBaseAddress_p) + CPHY_L0_CTRL_0 ,CPHY0_L0_IDDQEN);
   gm_SetRegBitsDWord((WORD)(PC_InitParams.PhyBaseAddress_p) + CPHY_L1_CTRL_0 ,CPHY0_L1_IDDQEN);
   gm_SetRegBitsDWord((WORD)(PC_InitParams.PhyBaseAddress_p) + CPHY_L2_CTRL_0 ,CPHY0_L2_IDDQEN);
   gm_SetRegBitsDWord((WORD)(PC_InitParams.PhyBaseAddress_p) + CPHY_L3_CTRL_0 ,CPHY0_L3_IDDQEN);

   if(PC_InitParams.PhyBaseAddress_p2 != NULL) //  CPHY2_AUX_PHY_CTRL
   {   //Dual DVI
      gm_SetRegBitsWord((WORD)(PC_InitParams.DeviceBaseAddress_p)+DVIB_OFFSET+HDMIDVI_RX_PHY_CTRL, HDMI_RX_PHY_PWR_CTRL);
      //Power down PHY 
      gm_SetRegBitsDWord((WORD)(PC_InitParams.PhyBaseAddress_p2) + CPHY_L0_CTRL_0 ,CPHY0_L0_IDDQEN);
      gm_SetRegBitsDWord((WORD)(PC_InitParams.PhyBaseAddress_p2) + CPHY_L1_CTRL_0 ,CPHY0_L1_IDDQEN);
      gm_SetRegBitsDWord((WORD)(PC_InitParams.PhyBaseAddress_p2) + CPHY_L2_CTRL_0 ,CPHY0_L2_IDDQEN);
      gm_SetRegBitsDWord((WORD)(PC_InitParams.PhyBaseAddress_p2) + CPHY_L3_CTRL_0 ,CPHY0_L3_IDDQEN);
   }

}


//******************************************************************************
// FUNCTION       : void HdmiDviPowrUpInputPort(BYTE InputConnector)
// USAGE          : Use this function to power up HdmiDvi block for power 
//                  saving.
// INPUT          : None
// OUTPUT         : None
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  :
// POST_CONDITION :
//******************************************************************************
void HdmiDviPowrUpInputPort(BYTE InputConnector)
{
   BYTE b_HdmiDviInNo = GetInPortConfigArrayLocation(InputConnector);
   gmt_HdmiDviInputPort* ptr_PortConfig;
   
   if (b_HdmiDviInNo >= NUM_OF_HDMIDVI_INP)
   {
      return;
   }
   ptr_PortConfig= &HdmiDviInPortConfig[b_HdmiDviInNo];
   //Power up digital block
   gm_ClearRegBitsWord((WORD)(PC_InitParams.DeviceBaseAddress_p)+ HDMIDVI_RX_PHY_CTRL,HDMI_RX_PHY_PWR_CTRL);
  
   //Power up PHY 
   gm_ClearRegBitsDWord((WORD)(PC_InitParams.PhyBaseAddress_p) + CPHY_L0_CTRL_0 ,CPHY0_L0_IDDQEN);
   gm_ClearRegBitsDWord((WORD)(PC_InitParams.PhyBaseAddress_p) + CPHY_L1_CTRL_0 ,CPHY0_L1_IDDQEN);
   gm_ClearRegBitsDWord((WORD)(PC_InitParams.PhyBaseAddress_p) + CPHY_L2_CTRL_0 ,CPHY0_L2_IDDQEN);
   gm_ClearRegBitsDWord((WORD)(PC_InitParams.PhyBaseAddress_p) + CPHY_L3_CTRL_0 ,CPHY0_L3_IDDQEN);

   if(PC_InitParams.PhyBaseAddress_p2 != NULL) //  CPHY2_AUX_PHY_CTRL
   {   //Dual DVI
      gm_ClearRegBitsWord((WORD)(PC_InitParams.DeviceBaseAddress_p)+DVIB_OFFSET+HDMIDVI_RX_PHY_CTRL, HDMI_RX_PHY_PWR_CTRL);
      //Power up PHY 
      gm_ClearRegBitsDWord((WORD)(PC_InitParams.PhyBaseAddress_p2) + CPHY_L0_CTRL_0 ,CPHY0_L0_IDDQEN);
      gm_ClearRegBitsDWord((WORD)(PC_InitParams.PhyBaseAddress_p2) + CPHY_L1_CTRL_0 ,CPHY0_L1_IDDQEN);
      gm_ClearRegBitsDWord((WORD)(PC_InitParams.PhyBaseAddress_p2) + CPHY_L2_CTRL_0 ,CPHY0_L2_IDDQEN);
      gm_ClearRegBitsDWord((WORD)(PC_InitParams.PhyBaseAddress_p2) + CPHY_L3_CTRL_0 ,CPHY0_L3_IDDQEN);
   }      
}


#endif // INACTIVE_DEVICE_POWER_SAVING

BOOL EXT_FUNC gm_SclHdmiMemCpy(U8  * Bp_Dest, U8  * Bp_Source, U16 B_Cnt)
{
   _fmemcpy(Bp_Dest, Bp_Source, B_Cnt);
   return TRUE;
}

#if (HPD_CONTROL_HANDLER_ENABLE==1)
/*
//******************************************************************************
//
// FUNCTION       : BOOL HdmiGetHPDPinStatus(gmt_UserDefConnNames B_InputConnector)
// USAGE          : Read status hdmi HPD pin, if is high or low
// INPUT          : B_InputConnector
// OUTPUT         : return 	TRUE if pin is high (connected)
//						FALSE if pin is low (disconnected)
// GLOBALS        : None
// POST_CONDITION :
//******************************************************************************
BOOL HdmiGetHPDPinStatus(gmt_UserDefConnNames B_InputConnector)
{
   if (gm_GetHdmiDviDigitalID(B_InputConnector)==DID_HDMI)
      return (gm_ReadRegByte(HDRX_HPD_STATUS) & BIT0)? TRUE: FALSE;
   return FALSE;
}
*/


void HdmiSetHpdPulseStart(gmt_PHY_CH Channel,gmt_UserDefConnNames B_InputConnector, WORD w_Timex10ms)
{
   if(Channel==gmvb_MainChannel)
      gm_TimerStart(HDMI_HPD_TIME_Main_10ms_TMR,w_Timex10ms);
   else
      gm_TimerStart(HDMI_HPD_TIME_Pip_10ms_TMR,w_Timex10ms);

   HdmiDviHpdControl(B_InputConnector, 0);         //HPD Low
   HdmiHpdControl[Channel].InputConnector=B_InputConnector;
   HdmiHpdControl[Channel].IsActivated=TRUE;
}


static void HdmiHpdhandler(gmt_PHY_CH Channel,gmt_UserDefConnNames B_InputConnector)
{
   BYTE b_10ms_Timer_TMR;
   if(Channel==gmvb_MainChannel)
      b_10ms_Timer_TMR=HDMI_HPD_TIME_Main_10ms_TMR;
   else
      b_10ms_Timer_TMR=HDMI_HPD_TIME_Pip_10ms_TMR;

   if (HdmiHpdControl[Channel].IsActivated == TRUE)
   {
      if (HdmiHpdControl[Channel].InputConnector != B_InputConnector)
      {	//Port is changed, force to stop HPD Pulse
         //HdmiDviHpdControl(B_InputConnector, 1);   //HPD High
         gm_TimerStop(b_10ms_Timer_TMR);
         HdmiHpdControl[Channel].IsActivated = FALSE;
      }
      else
      {
         if (gm_TimerCheck(b_10ms_Timer_TMR) != TIMER_OK)
         { //Time out or Time Stop
            #if (ENABLE_MIN_PORT_OFF_LIMITATION == 1)
            gmt_HdmiDviInputPort* ptr_PortConfig=&HdmiDviInPortConfig[GetInPortConfigArrayLocation(B_InputConnector)];
            if(PC_EnableHpdInPortInit==TRUE)
            {
               DWORD curTime=gm_GetSystemTime();
               BYTE b_HdmiDviInNo = GetInPortConfigArrayLocation(B_InputConnector);
               gmt_HdmiDviInputPort* ptr_PortConfig;

               ptr_PortConfig= &HdmiDviInPortConfig[b_HdmiDviInNo];

               if((curTime<PC_PrevPortOffTime) || ((curTime-PC_PrevPortOffTime)>MIN_PORT_OFF_Time))
               {
                  HdmiDviHpdControl(B_InputConnector, 1);   //HPD High
                  gm_TimerStop(b_10ms_Timer_TMR);
                  HdmiHpdControl[Channel].IsActivated = FALSE;
               }
               #if 0
               else
               {
                  gm_Print("H-",0);
               }
               #endif
            }
            else
            {
               HdmiDviHpdControl(B_InputConnector, 1);   //HPD High
               gm_TimerStop(b_10ms_Timer_TMR);
               HdmiHpdControl[Channel].IsActivated = FALSE;
            }
            #else
            HdmiDviHpdControl(B_InputConnector, 1);   //HPD High
            gm_TimerStop(b_10ms_Timer_TMR);
            HdmiHpdControl[Channel].IsActivated = FALSE;
            #endif
         }
      }
   }
}
#endif //(HPD_CONTROL_HANDLER_ENABLE==1)


static gm_AVDdsSrcTypes_t GetCorrespondedAVDDS(STHDMIRX_InputHandle_t InputHandle, STHDMIRX_DdsSrcTypes_t estAVdds)
{
   gmt_HdmiDviInputPort * ptr_PortConfig;
   gm_AVDdsSrcTypes_t AvDdsSrc=gm_SRC_AVDDS_INVALID;
   BYTE b_HdmiDviInNo,b_InputConnector=0xFF;

   for (b_HdmiDviInNo=0;b_HdmiDviInNo< NUM_OF_HDMIDVI_INP ; b_HdmiDviInNo++)
   {
      ptr_PortConfig       = &HdmiDviInPortConfig[b_HdmiDviInNo];
      if (InputHandle == PC_InputHandle)
         b_InputConnector =PC_InputConnector;
   }

   if (estAVdds==HDMIRX_VID_VDDS)
   {
      if (b_InputConnector==gB_CurrentInputConnector[gmvb_MainChannel])
         AvDdsSrc = gm_SRC_AVDDS1;
      else if (b_InputConnector==gB_CurrentInputConnector[gmvb_PipChannel])
         AvDdsSrc = gm_SRC_AVDDS2;
   }
   else if (estAVdds==HDMIRX_AUD_VDDS)
   {
      if (b_InputConnector==gB_CurrentInputConnector[gmvb_MainChannel])
         AvDdsSrc = gm_SRC_AVDDS3;
      else if (b_InputConnector==gB_CurrentInputConnector[gmvb_PipChannel])
         AvDdsSrc = gm_SRC_AVDDS4;
   }
   return AvDdsSrc;
}

void STHDMIRX_CLKGEN_InitialDdsSetupEx(gm_AVDdsSrcTypes_t estAVdds)
{
   //gm_Print("InitialDdsSetupEx(%d)",estAVdds);
   gm_InitialDdsSetup(estAVdds);
}


void STHDMIRX_CLKGEN_DDSOpenLoopForceEx(gm_AVDdsSrcTypes_t estAVdds, U32 ulDdsInitFreq)
{
   //gm_Print("DDSOpenLoopForceEx(%d)",estAVdds);

    /* set the open loop frequency*/
#if (HDMI_VDDS_STABILITY_IMPROVEMENT==1)       
   if(estAVdds <= gm_SRC_AVDDS2)
   {
      gm_DDSInitFreqSet(estAVdds, (ulDdsInitFreq/2));  
      gm_DDSFreqDeltaSet(estAVdds, 0);

      if (estAVdds == gm_SRC_AVDDS1)
      {
         gm_SetRegBitsWord(AVDDS1_CONTROL0,AVDDS1_FORCE_OPLOOP);
         gm_ClearRegBitsWord(AVDDS1_CONTROL0,(AVDDS1_CORR_CTRL|AVDDS1_BYPASS_MN_LOOP));    
         gm_ClearRegBitsWord(AVDDS1_CONTROL1,0xF0);    
      }
      else
      {  // (estAVdds == gm_SRC_AVDDS2)
         gm_SetRegBitsWord(AVDDS2_CONTROL0,AVDDS2_FORCE_OPLOOP);
         gm_ClearRegBitsWord(AVDDS2_CONTROL0,(AVDDS2_CORR_CTRL|AVDDS2_BYPASS_MN_LOOP));    
         gm_ClearRegBitsWord(AVDDS2_CONTROL1,0xF0);    
      }
   }
   else
#endif   
      gm_DDSOpenLoopForce(estAVdds,ulDdsInitFreq);
}

#define VDDSOFFSET (AVDDS2_CONTROL0 - AVDDS1_CONTROL0)
BOOL  STHDMIRX_CLKGEN_DDSCloseLoopForceEx(gm_AVDdsSrcTypes_t estAVdds, U32 ulDdsInitFreq)
{
   //gm_Print("DDSCloseLoopForceEx(%d)",estAVdds);
#if (HDMI_VDDS_STABILITY_IMPROVEMENT==1)     
   #if 0  
   //Done by STHDMIRX_VideoPipe_Init         
   gm_WriteRegWord(HDRX_VID_BUF_FILT_ZONE, 0x0002);
   gm_WriteRegWord(HDRX_VID_LNK_RSRV, 0x0003);
   gm_WriteRegWord(HDRX_VID_CONV_CTRL, 0x0000);  
   #endif   

   if(estAVdds <= gm_SRC_AVDDS2)
   {
      BOOL  uLockStatus = TRUE;
      BYTE B_Deepcolormode;
      WORD VDDS1_CONTROL0=AVDDS1_CONTROL0;
      WORD VDDS1_CONTROL1=AVDDS1_CONTROL1;
      WORD VDDS1_INIT=AVDDS1_INIT;
      WORD VDDS1_DIV=AVDDS1_DIV;
      WORD VDDS1_MUL=AVDDS1_MUL;
      WORD VDDS1_CUR_FREQ=AVDDS1_CUR_FREQ;
      WORD VDDS1_INI_FREQ=AVDDS1_INI_FREQ;
      WORD COLOR_DEPH_STATUS;
      gmt_HDMI_DVI_DID b_DID=gm_GetHdmiDviDigitalID(gB_CurrentInputConnector[gmvb_MainChannel]);
      //gmt_HdmiDviInputPort* ptr_PortConfig=&HdmiDviInPortConfig[GetInPortConfigArrayLocation(gB_CurrentInputConnector[gmvb_MainChannel])];
      
      if(estAVdds == gm_SRC_AVDDS2)
      {
         VDDS1_CONTROL0+=VDDSOFFSET;
         VDDS1_CONTROL1+=VDDSOFFSET;
         VDDS1_INIT    +=VDDSOFFSET;
         VDDS1_DIV     +=VDDSOFFSET;
         VDDS1_MUL     +=VDDSOFFSET;
         VDDS1_CUR_FREQ+=VDDSOFFSET;
         b_DID=gm_GetHdmiDviDigitalID(gB_CurrentInputConnector[gmvb_PipChannel]);
         //ptr_PortConfig=&HdmiDviInPortConfig[GetInPortConfigArrayLocation(gB_CurrentInputConnector[gmvb_PipChannel])];
      }
         
      gm_WriteRegWord(VDDS1_CONTROL0, 0x0048); //AVDDS1_K_DIFF=100  AVDDS1_K_MAIN=100
      gm_WriteRegWord(VDDS1_CONTROL1, 0x0002); //AVDDS1_CLKMUL=1 

      if(ulDdsInitFreq)
      {
      /* set the Init Frequency so DDS will start Tracking from this freq, try to program the desired DDS freq, it locks the DDS faster*/
      gm_DDSInitFreqSet(estAVdds, (ulDdsInitFreq/2));
      gm_WriteRegWord(VDDS1_INIT, BIT0); 
      }

      gm_WriteRegDWord_LE(VDDS1_DIV, 0x0000FFF0UL);
      // gm_WriteRegWord(AVDDS1_DIV+2 + VddsOffset, 0x0000); 

      // Check Deep Color mode 
      if(b_DID == DID_HDMI)
      {
         B_Deepcolormode = gm_ReadRegByte(HDRX_COLOR_DEPH_STATUS) & 0x0F;
         B_Deepcolormode = ((B_Deepcolormode==0x05)? 10 : \
         (B_Deepcolormode==0x06)? 12 : \
         (B_Deepcolormode==0x07)? 16 : 8);
      }
      else
         B_Deepcolormode = 8;      
      
      hdmi_msg("Color Depth =%d bpc",B_Deepcolormode);
      switch(B_Deepcolormode)
      { 
         case 10: // 10 bit
            gm_WriteRegDWord_LE(VDDS1_MUL, 0x0000CCC0UL);   // AVDDS1_MUL
            //gm_WriteRegWord(AVDDS1_MUL+2, 0x0000); // AVDDS1_MUL
            break;
         case 12: // 12 bit
            gm_WriteRegDWord_LE(VDDS1_MUL, 0x0000AAA0UL); // AVDDS1_MUL
            //gm_WriteRegWord(AVDDS1_MUL+2, 0x0000); // AVDDS1_MUL
            break;
         case 16: // 16 bit
            gm_WriteRegDWord_LE(VDDS1_MUL, 0x00007FF8UL); // AVDDS1_MUL
            //gm_WriteRegWord(AVDDS1_MUL+2, 0x0000); // AVDDS1_MUL
            break;
         default: // 8 bit
            gm_WriteRegDWord_LE(VDDS1_MUL, 0x0000FFF0UL); // AVDDS1_MUL
            //gm_WriteRegWord(VDDS1_MUL+2, 0x0000); // AVDDS1_MUL
            break;
      }
      gm_WriteRegWord(VDDS1_INIT, AVDDS1_MULDIV_WR); // AVDDS1_INIT
      gm_Delay1ms(1);
#if (FAST_AVDDS_LOCK!=0)
      gm_WriteRegWord(VDDS1_CONTROL0, (AVDDS1_FREQ_AVG | (1 << AVDDS1_K_MAIN_SHIFT) | (1 << AVDDS1_K_DIFF_SHIFT)));
#else      
      gm_WriteRegWord(VDDS1_CONTROL0, 0x8048); //AVDDS1_FREQ_AVG=1 AVDDS1_K_DIFF=100  AVDDS1_K_MAIN=100
#endif    
      gm_WriteRegWord(VDDS1_CONTROL1, 0x0083); //AVDDS1_COR_K_P=1000 AVDDS1_CLKMUL=1 AVDDS1_MULDIV_SEL=1   
      gm_Delay1ms(1);
#if (FAST_AVDDS_LOCK!=0)    
      gm_WriteRegWord(VDDS1_CONTROL0, (AVDDS1_FREQ_AVG | AVDDS1_BYPASS_MN_LOOP | (1 << AVDDS1_K_MAIN_SHIFT) | (1 << AVDDS1_K_DIFF_SHIFT)));
#else
      gm_WriteRegWord(VDDS1_CONTROL0, 0xB048); //AVDDS1_FREQ_AVG=1 AVDDS1_BYPASS_MN_LOOP=1 AVDDS1_CORR_CTRL=1 AVDDS1_K_DIFF=100  AVDDS1_K_MAIN=100 
#endif      
      gm_WriteRegWord(VDDS1_CONTROL1, 0x00C3); //AVDDS1_COR_K_P=1100 AVDDS1_CLKMUL=1 AVDDS1_MULDIV_SEL=1   

      /* DDS Tracking Error*/
      if (gm_AVDDSWaitStable(estAVdds))
      {
         DWORD DW_Freq, DW_FreqErr, DW_LFreq;
         //gm_Hdmi_WriteRegWord(0x81DC, 0x0023); // AVDDS1_CONTROL1    
#if (FAST_AVDDS_LOCK!=0)      
         gm_WriteRegWord(VDDS1_CONTROL0, (AVDDS1_CORR_CTRL | AVDDS1_BYPASS_MN_LOOP | (4 << AVDDS1_K_MAIN_SHIFT) | (4 << AVDDS1_K_DIFF_SHIFT)));
#else
         gm_WriteRegWord(VDDS1_CONTROL0, 0x3048); //AVDDS1_BYPASS_MN_LOOP=1 AVDDS1_CORR_CTRL=1 AVDDS1_K_DIFF=100  AVDDS1_K_MAIN=100 
#endif

         gm_Delay1ms(1); //gm_Delay1us(100);
         gm_WriteRegDWord_LE(VDDS1_CUR_FREQ,0xFFFFFFFFUL);
         DW_Freq = gm_ReadRegDWord_LE(VDDS1_CUR_FREQ);
         switch(B_Deepcolormode)
         { 
            case 10: // 10 bit LCLK = 1.25*VCLK
               DW_Freq = (((DW_Freq<<2)+DW_Freq)>>2);
               break;
            case 12: // 12 bit LCLK = 1.5*VCLK
               DW_Freq = (((DW_Freq<<1)+DW_Freq)>>1);
               break;
            case 16: // 16 bit LCLK = 2*VCLK
               DW_Freq = (DW_Freq<<1);
               break;
            default: // 8 bit LCLK = 1*VCLK
               break;
         }

         DW_LFreq = (gm_ReadRegDWord_LE(VDDS1_INI_FREQ))<<1;
         DW_FreqErr = ((DW_Freq > DW_LFreq) ? (DW_Freq - DW_LFreq) : (DW_LFreq - DW_Freq));
         //gm_Print("HDMIRx: Ex FreqL = %x",DW_LFreq);
         //gm_Print("HDMIRx: Ex FreqH = %x",DW_LFreq>>16);
         //gm_Print("HDMIRx: Curr FreqL = %x",DW_Freq);
         //gm_Print("HDMIRx: Curr FreqH = %x",DW_Freq>>16);
         //~250ppm error
         if (DW_FreqErr & 0xFFFFF000UL)
         {        
            hdmi_msg("HDMIRx: Freq Error greater than 250ppm = %x",DW_FreqErr);
            // Will need to re-initiate DDS
            uLockStatus = FALSE;
         }
      }
      else
      {
         #if (REINIT_VDDS_MULDIV_WR_IN_UNSTABLE !=0)
         gm_WriteRegWord(VDDS1_INIT, AVDDS1_MULDIV_WR); // AVDDS1_INIT
         #endif
         uLockStatus = FALSE;
      }

      return uLockStatus;
   }
   else
#endif   
      return gm_DDSCloseLoopForce(estAVdds,ulDdsInitFreq);
}

BOOL STHDMIRX_CLKGEN_DdsInitFreqSetEx(gm_AVDdsSrcTypes_t estAVdds, U32 ulDdsInitFreq)
{
   //gm_Print("DdsInitFreqSetEx(%d)",estAVdds);
   return gm_DDSInitFreqSet(estAVdds,ulDdsInitFreq);
}

U32 STHDMIRX_CLKGEN_DdsCurrentFreqGetEx(gm_AVDdsSrcTypes_t estAVdds)
{

   return gm_DDSCurrentFreqGet(estAVdds);
}

U16 STHDMIRX_CLKGEN_DdsTrackingErrorGetEx(gm_AVDdsSrcTypes_t estAVdds)
{

   return gm_DDSTrackingErrorGet(estAVdds);
}

#if (HDMI_DVI_TIMING_LOSS_PATCH==1)
BOOL IsHdmiDviTimingChangeFromStable2Unstable(BYTE InputConnector)
{
   #if (PATCH_OF_CLEAR_HDMI_ANA_LCLK_LOST==1)
   static BYTE b_AnaLostCount=0;
   #endif
   gmt_HdmiDviInputPort* ptr_PortConfig=&HdmiDviInPortConfig[GetInPortConfigArrayLocation(InputConnector)];
   WORD BaseAddr= (WORD)(PC_InitParams.DeviceBaseAddress_p)+HDRX_IFM_INSTRUMENT_PU_ADDRS_OFFSET;

   #if (PATCH_OF_CLEAR_HDMI_ANA_LCLK_LOST==1)
   if(gm_ReadRegWord(BaseAddr+HDMI_RX_INPUT_IRQ_STATUS) & (HDMI_INPUT_AFR_DETECT|HDMI_ANA_LCLK_LOST))
   {
         
         hdmi_msg("***(0x%x)",gm_ReadRegWord(BaseAddr+HDMI_RX_INPUT_IRQ_STATUS));
         if(gm_ReadRegWord(BaseAddr+HDMI_RX_INPUT_IRQ_STATUS)&HDMI_ANA_LCLK_LOST)
            b_AnaLostCount++;
         if(b_AnaLostCount>=3)
         {
            gm_WriteRegWord(BaseAddr+HDMI_RX_INPUT_IRQ_STATUS,HDMI_ANA_LCLK_LOST);
            b_AnaLostCount=0;
            //gm_Print("--->Clear ANA_LCLK_LOST",0);
         }
         return TRUE;
      }
      b_AnaLostCount=0;
   #else
      if(gm_ReadRegWord(BaseAddr+HDMI_RX_INPUT_IRQ_STATUS) & (HDMI_INPUT_AFR_DETECT|HDMI_ANA_LCLK_LOST))
      {
         hdmi_msg("***(0x%x)",gm_ReadRegWord(BaseAddr+HDMI_RX_INPUT_IRQ_STATUS));
         //gm_SetRegBitsWord(BaseAddr+HDMI_RX_INPUT_IRQ_STATUS, HDMI_INPUT_AFR_DETECT|HDMI_ANA_LCLK_LOST);
         return TRUE;
      }
   #endif

   return FALSE;
}
#endif

#if (FEATURE_HDCP_REPEATER == ENABLE)
void HdmiRxHdcpRptrRxEntry (U32 RxDeviceIdentifier_32,HdcpRepeaterRxRequest_t	RxRequest_e,U8 * RxRequestData_pv)
{
#if 1
   BYTE i;
   for(i=0; i<NUM_CHANNELS; i++)
   {
      if(((gB_CurrentInputConnector[i]==ALL_BOARDS_DVI1)  && (RxDeviceIdentifier_32==Dvi1RxPort ))||
         ((gB_CurrentInputConnector[i]==ALL_BOARDS_DVI2)  && (RxDeviceIdentifier_32==Dvi2RxPort ))||
         ((gB_CurrentInputConnector[i]==ALL_BOARDS_HDMI)  && (RxDeviceIdentifier_32==Hdmi1RxPort))||
         ((gB_CurrentInputConnector[i]==ALL_BOARDS_HDMI2) && (RxDeviceIdentifier_32==Hdmi2RxPort))||
         ((gB_CurrentInputConnector[i]==ALL_BOARDS_HDMI3) && (RxDeviceIdentifier_32==Hdmi3RxPort)))
      {
         BYTE b_HdmiDviInNo = GetInPortConfigArrayLocation(gB_CurrentInputConnector[i]);
         gmt_HdmiDviInputPort* ptr_PortConfig; //=&HdmiDviInPortConfig[GetInPortConfigArrayLocation(gB_CurrentInputConnector[i])];
         ptr_PortConfig= &HdmiDviInPortConfig[b_HdmiDviInNo];
         
         STHDMIRX_HdcpRptrEntry(PC_InputHandle, RxRequest_e, RxRequestData_pv);
         return;
      }
   }
#else
   gmt_HDMI_DVI_DID b_DID;
   BYTE i;
   for(i=0; i<NUM_CHANNELS; i++)
   {
      b_DID=gm_GetHdmiDviDigitalID(gB_CurrentInputConnector[i]);
      
      if (((b_DID==DID_HDMI) && (RxDeviceIdentifier_32==HdmiRxPort)) ||
         (((b_DID==DID_DVI0)||(b_DID==DID_DVI_DUAL)) && (RxDeviceIdentifier_32==DviRxPort)))
      {
         gmt_HdmiDviInputPort* ptr_PortConfig=&HdmiDviInPortConfig[GetInPortConfigArrayLocation(gB_CurrentInputConnector[i])];
         STHDMIRX_HdcpRptrEntry(PC_InputHandle, RxRequest_e, RxRequestData_pv);
         return;
      }
   }
#endif   
};
#endif

void HdmiDviPrintSignalProperty(BYTE InputConnector)
{
   BYTE b_HdmiDviInNo = GetInPortConfigArrayLocation(InputConnector);
   gmt_HdmiDviInputPort* ptr_PortConfig;

   if (b_HdmiDviInNo >= NUM_OF_HDMIDVI_INP)
   {
      gm_Print("Wrong InputConnector number",0);
      return;
   }
   ptr_PortConfig= &HdmiDviInPortConfig[b_HdmiDviInNo];

   gm_Print("===HdmiDviPrintSignalProperty:(%d)===",InputConnector);
   if (PC_SignalDataLoaded== FALSE)
   {
      gm_Print("Error! Signal Property Not loaded.",0);
      return;
   }

   switch (PC_SignalStatusEvtData.Property.SignalType)
   {
      case STHDMIRX_SIGNAL_TYPE_DVI:
         gm_Print("Signal Type   :DVI",0);
         break;
      case STHDMIRX_SINGAL_TYPE_HDMI:
         gm_Print("Signal Type   :HDMI",0);
         break;
      case STHDMIRX_SIGNAL_TYPE_DDVI:
         gm_Print("Signal Type   :Dual DVI",0);
         break;
   }

   switch (PC_SignalStatusEvtData.Property.Timing.ScanType)
   {
      case STHDMIRX_SINGAL_SCAN_TYPE_PROGRESSIVE:
         gm_Print("ScanType      :Progressive",0);
         break;
      case STHDMIRX_SIGNAL_SCAN_TYPE_INTERLACED:
         gm_Print("ScanType      :Interlaced",0);
         break;
   }
   gm_Print("PixelClock    :%LdHz",PC_SignalStatusEvtData.Property.Timing.PixelClockHz);
   gm_Print("HActiveStart  :%d",PC_SignalStatusEvtData.Property.Timing.HActiveStart);
   gm_Print("HActive       :%d",PC_SignalStatusEvtData.Property.Timing.HActive);
   gm_Print("HTotal        :%d",PC_SignalStatusEvtData.Property.Timing.HTotal);
   gm_Print("VActiveStart  :%d",PC_SignalStatusEvtData.Property.Timing.VActiveStart);
   gm_Print("VActive       :%d",PC_SignalStatusEvtData.Property.Timing.VActive);
   gm_Print("VTotal        :%d",PC_SignalStatusEvtData.Property.Timing.VTotal);

   if (PC_SignalStatusEvtData.Property.Timing.HSyncPolarity)
      gm_Print("HSyncPolarity :Positive",0);
   else
      gm_Print("HSyncPolarity :Negative",0);

   if (PC_SignalStatusEvtData.Property.Timing.VSyncPolarity)
      gm_Print("VSyncPolarity :Positive",0);
   else
      gm_Print("VSyncPolarity :Negative",0);


}


void HdmiDviPrintVideoProperty(BYTE InputConnector)
{
   BYTE b_HdmiDviInNo = GetInPortConfigArrayLocation(InputConnector);
   gmt_HdmiDviInputPort* ptr_PortConfig;

   if (b_HdmiDviInNo >= NUM_OF_HDMIDVI_INP)
   {
      gm_Print("Wrong InputConnector number",0);
      return;
   }
   ptr_PortConfig= &HdmiDviInPortConfig[b_HdmiDviInNo];

   gm_Print("===HdmiDviPrintVideoProperty:(%d)===",InputConnector);

   if (PC_VideoDataLoaded == FALSE)
   {
      gm_Print("Error! Video Property Not loaded.",0);
      return;
   }

   switch (PC_VideoPropertyEvtData.Property.ColorDepth)
   {
      case STHDMIRX_COLOR_DEPTH_24BPP:
         gm_Print("ColorDepth    :24 BPP",0);
         break;
      case STHDMIRX_COLOR_DEPTH_30BPP:
         gm_Print("ColorDepth	   :30 BPP",0);
         break;
      case STHDMIRX_COLOR_DEPTH_36BPP:
         gm_Print("ColorDepth	   :36 BPP",0);
         break;
      case STHDMIRX_COLOR_DEPTH_48BPP:
         gm_Print("ColorDepth	   :48 BPP",0);
         break;
   }

   switch (PC_VideoPropertyEvtData.Property.ColorFormat)
   {
      case STHDMIRX_COLOR_FORMAT_RGB:
         gm_Print("ColorFormat   :RGB",0);
         break;
      case STHDMIRX_COLOR_FORMAT_YUV_422:
         gm_Print("ColorFormat   :YUV422",0);
         break;
      case STHDMIRX_COLOR_FORMAT_YUV_444:
         gm_Print("ColorFormat   :YUV444",0);
         break;
   }

   if (PC_SignalStatusEvtData.Property.SignalType==STHDMIRX_SINGAL_TYPE_HDMI)
      gm_Print("PixRepeatFactr:%d",PC_VideoPropertyEvtData.Property.PixelRepeatFactor);

   if (PC_VideoPropertyEvtData.Property.Video3D ==TRUE)
   {
      gm_Print("Is3DFormat    :Yes",0);

      switch (PC_VideoPropertyEvtData.Property.VideoProperty3D.Format)
      {
         case	STHDMIRX_3D_FORMAT_FRAME_PACK:
            gm_Print("3dFormat      :Frame Packing",0);
            break;
         case	STHDMIRX_3D_FORMAT_FIELD_ALT:
            gm_Print("3dFormat      :Field Alternative",0);
            break;
         case	STHDMIRX_3D_FORMAT_LINE_ALT:
            gm_Print("3dFormat      :Line Alternative",0);
            break;
         case	STHDMIRX_3D_FORMAT_SBYS_FULL:
            gm_Print("3dFormat      :Side By Side Full",0);
            break;
         case	STHDMIRX_3D_FORMAT_SBYS_HALF:
            gm_Print("3dFormat      :Side By Side Half",0);
            break;
         case	STHDMIRX_3D_FORMAT_TOP_AND_BOTTOM:
            gm_Print("3dFormat      :Top and Bottom",0);
            break;

      }

   }
   else
   {
      gm_Print("Is3DFormat    :No",0);
   }

   if(PC_VideoPropertyEvtData.Property.ITContent)
   {
      gm_Print("ITContent     :1",0); 
   }
   else
   {
      gm_Print("ITContent     :0",0);
   }

   gm_Printf("VideoTimingCode   :%d", PC_VideoPropertyEvtData.Property.VideoTimingCode);

   switch (PC_VideoPropertyEvtData.Property.ScanInfo)
   {
      case STHDMIRX_SCAN_INFO_NO_DATA:
         gm_Print("ScanInfo      :No Data",0);
         break;
      case STHDMIRX_SCAN_INFO_FOR_OVERSCANNED_DISPLAY:
         gm_Print("ScanInfo      :OverScanned Display",0);
         break;
      case STHDMIRX_SCAN_INFO_FOR_UNDER_SCANNED_DISPLAY:
         gm_Print("ScanInfo      :UnderScanned Display",0);
         break;
      case STHDMIRX_SCAN_INFO_RESERVED:
         gm_Print("ScanInfo      :Reserved",0);
         break;
   }

   switch (PC_VideoPropertyEvtData.Property.Colorimetery)
   {
      case STHDMIRX_COLORIMETRY_STD_DEFAULT:
         gm_Print("Colorimetery  :Default",0);
         break;
      case STHDMIRX_COLORIMETRY_STD_BT_601:
         gm_Print("Colorimetery  :BT601",0);
         break;
      case STHDMIRX_COLORIMETRY_STD_BT_709:
         gm_Print("Colorimetery  :BT709",0);
         break;
      case STHDMIRX_COLORIMETRY_STD_XVYCC_601:
         gm_Print("Colorimetery  :XvYCC601",0);
         break;
      case STHDMIRX_COLORIMETRY_STD_XVYCC_709:
         gm_Print("Colorimetery  :XvYCC709",0);
         break;
      case STHDMIRX_COLORIMETRY_STD_sYCC_601:
         gm_Print("Colorimetery  :sYCC601",0);
         break;
      case STHDMIRX_COLORIMETRY_STD_AdobeYCC_601:
         gm_Print("Colorimetery  :Adobe601",0);
         break;
      case STHDMIRX_COLORIMETRY_STD_AdobeRGB:
         gm_Print("Colorimetery	:AdobeRGB",0);
         break;
   }

   switch (PC_VideoPropertyEvtData.Property.RGBQuantRange)
   {
      case STHDMIRX_RGB_QUANT_RANGE_DEFAULT:
         gm_Print("RGBQuantRange :Default",0);
         break;
      case STHDMIRX_RGB_QUANT_RANGE_LIMITED:
         gm_Print("RGBQuantRange :Limited",0);
         break;
      case STHDMIRX_RGB_QUANT_RANGE_FULL:
         gm_Print("RGBQuantRange :Full",0);
         break;
      case STHDMIRX_RGB_QUANT_RANGE_RESERVED:
         gm_Print("RGBQuantRange :Reserved",0);
         break;
   }

   switch (PC_VideoPropertyEvtData.Property.YCQuantRange)
   {
      case STHDMIRX_YC_QUANT_RANGE_LIMITED:
         gm_Print("YCQuantRange  :Limited",0);
         break;
      case STHDMIRX_YC_QUANT_RANGE_FULL:
         gm_Print("YCQuantRange  :Full",0);
         break;
      case STHDMIRX_YC_QUANT_RANGE_RESERVED:
         gm_Print("YCQuantRange  :Reserved",0);
         break;
   }

   if(PC_VideoPropertyEvtData.Property.ContentType == STHDMIRX_CONTENT_TYPE_GRAPHICS)
   {
      gm_Print("ContentType     :GRAPHICS",0);
   }
   else if(PC_VideoPropertyEvtData.Property.ContentType == STHDMIRX_CONTENT_TYPE_PHOTO)
   {
      gm_Print("ContentType     :PHOTO",0);
   }
   else if(PC_VideoPropertyEvtData.Property.ContentType == STHDMIRX_CONTENT_TYPE_CINEMA)
   {
      gm_Print("ContentType     :CINEMA",0);
   }
   else if(PC_VideoPropertyEvtData.Property.ContentType == STHDMIRX_CONTENT_TYPE_GAME)
   {
      gm_Print("ContentType     :GAME",0);
   }
   else
   {
      gm_Print("ContentType     :UNKNOWN",0); 
   }


#if 0
   typedef struct
   {
      STHDMIRX_ColorDepth_t			ColorDepth;
      STHDMIRX_ColorFormat_t			ColorFormat;
      STHDMIRX_ColorimetryStd_t		Colorimetery;
      STHDMIRX_BarInfo_t				BarInfo;
      STHDMIRX_ScanInfo_t			    ScanInfo;
      STHDMIRX_ScalingInfo_t			ScalingInfo;
      STHDMIRX_AspectRatio_t	 		PictureAR;
      STHDMIRX_AspectRatio_t			ActiveFormatAR;
      STHDMIRX_RGBQuantRange_t		RGBQuantRange;
      STHDMIRX_PixelRepeatFactor_t	PixelRepeatFactor;
      BOOL							ITContent;
      U32								VideoTimingCode;
      STHDMIRX_YCQuantRange_t		    YCQuantRange;
      STHDMIRX_ContentType_t			ContentType;
      BOOL							HDMIVideoFormat;
      BOOL							Video3D;
      STHDMIRX_3DVideoProperty_t		VideoProperty3D;
   } STHDMIRX_VideoProperty_t;
#endif
}

void HdmiDviPrintAudioProperty(BYTE InputConnector)
{
   BYTE b_HdmiDviInNo = GetInPortConfigArrayLocation(InputConnector);
   gmt_HdmiDviInputPort* ptr_PortConfig;

   //STHDMIRX_AudioPropertyEvtData_t audio;

   if (b_HdmiDviInNo >= NUM_OF_HDMIDVI_INP)
   {
      gm_Print("Wrong InputConnector number",0);
      return;
   }

   ptr_PortConfig = &HdmiDviInPortConfig[b_HdmiDviInNo];

   gm_Print("===HdmiDviPrintAudioProperty:(%d)===", InputConnector);

   if (PC_AudioDataLoaded == FALSE)
   {
      gm_Print("Audio Property Not loaded.");
      return;
   }

   switch (PC_AudioPropertyEvtData.Property.StreamType)
   {
      case STHDMIRX_AUDIO_STREAM_TYPE_UNKNOWN:
         gm_Print(" StreamType        = %d : UnKnown", PC_AudioPropertyEvtData.Property.StreamType);
         break;
      case STHDMIRX_AUDIO_STREAM_TYPE_IEC:
         gm_Print(" StreamType        = %d : IEC", PC_AudioPropertyEvtData.Property.StreamType);
         break;
      case STHDMIRX_AUDIO_STREAM_TYPE_DSD:
         gm_Print(" StreamType        = %d : DSD", PC_AudioPropertyEvtData.Property.StreamType);
         break;
      case STHDMIRX_AUDIO_STREAM_TYPE_DST:
         gm_Print(" StreamType        = %d : DST", PC_AudioPropertyEvtData.Property.StreamType);
         break;
      case STHDMIRX_AUDIO_STREAM_TYPE_HBR:
         ;
         gm_Print(" StreamType        = %d : HBR", PC_AudioPropertyEvtData.Property.StreamType);
         break;
   }

   switch (PC_AudioPropertyEvtData.Property.CodingType)
   {
      case STHDMIRX_AUDIO_CODING_TYPE_NONE:
         gm_Print(" CodingType        = %d : None",    PC_AudioPropertyEvtData.Property.CodingType);
         break;
      case STHDMIRX_AUDIO_CODING_TYPE_PCM:
         gm_Print(" CodingType        = %d : PCM",     PC_AudioPropertyEvtData.Property.CodingType);
         break;
      case STHDMIRX_AUDIO_CODING_TYPE_AC3:
         gm_Print(" CodingType        = %d : AC3",     PC_AudioPropertyEvtData.Property.CodingType);
         break;
      case STHDMIRX_AUDIO_CODING_TYPE_MPEG1:
         gm_Print(" CodingType        = %d : MPEG1",   PC_AudioPropertyEvtData.Property.CodingType);
         break;
      case STHDMIRX_AUDIO_CODING_TYPE_MP3:
         gm_Print(" CodingType        = %d : MP3",     PC_AudioPropertyEvtData.Property.CodingType);
         break;
      case STHDMIRX_AUDIO_CODING_TYPE_MPEG2:
         gm_Print(" CodingType        = %d : MPEG2",   PC_AudioPropertyEvtData.Property.CodingType);
         break;
      case STHDMIRX_AUDIO_CODING_TYPE_AAC:
         gm_Print(" CodingType        = %d : AAC",     PC_AudioPropertyEvtData.Property.CodingType);
         break;
      case STHDMIRX_AUDIO_CODING_TYPE_DTS:
         gm_Print(" CodingType        = %d : DTS",     PC_AudioPropertyEvtData.Property.CodingType);
         break;
      case STHDMIRX_AUDIO_CODING_TYPE_ATRAC:
         gm_Print(" CodingType        = %d : ATRAC",   PC_AudioPropertyEvtData.Property.CodingType);
         break;
      case STHDMIRX_AUDIO_CODING_TYPE_DSD:
         gm_Print(" CodingType        = %d : DSD",     PC_AudioPropertyEvtData.Property.CodingType);
         break;
      case STHDMIRX_AUDIO_CODING_TYPE_DDPLUS:
         gm_Print(" CodingType        = %d : DDPLUS",  PC_AudioPropertyEvtData.Property.CodingType);
         break;
      case STHDMIRX_AUDIO_CODING_TYPE_DTS_HD:
         gm_Print(" CodingType        = %d : DTS_HD",  PC_AudioPropertyEvtData.Property.CodingType);
         break;
      case STHDMIRX_AUDIO_CODING_TYPE_MAT:
         gm_Print(" CodingType        = %d : MAT",     PC_AudioPropertyEvtData.Property.CodingType);
         break;
      case STHDMIRX_AUDIO_CODING_TYPE_DST:
         gm_Print(" CodingType        = %d : DST",     PC_AudioPropertyEvtData.Property.CodingType);
         break;
      case STHDMIRX_AUDIO_CODING_TYPE_WMA_PRO:
         gm_Print(" CodingType        = %d : WMA_PRO", PC_AudioPropertyEvtData.Property.CodingType);
         break;
   }

   gm_Print(" ChannelCount      = %d",  PC_AudioPropertyEvtData.Property.ChannelCount);
   gm_Print(" ChannelAllocation = %d",  PC_AudioPropertyEvtData.Property.ChannelAllocation);
   gm_Print(" SamplingFrequency = %ld", PC_AudioPropertyEvtData.Property.SamplingFrequency);

   switch (PC_AudioPropertyEvtData.Property.SampleSize)
   {
      case STHDMIRX_AUDIO_SAMPLE_SIZE_NONE:
         gm_Print(" SampleSize        = %d : None", PC_AudioPropertyEvtData.Property.SampleSize);
         break;
      case STHDMIRX_AUDIO_SAMPLE_SIZE_16_BITS:
         gm_Print(" SampleSize        = %d : 16", PC_AudioPropertyEvtData.Property.SampleSize);
         break;
      case STHDMIRX_AUDIO_SAMPLE_SIZE_17_BITS:
         gm_Print(" SampleSize        = %d : 17", PC_AudioPropertyEvtData.Property.SampleSize);
         break;
      case STHDMIRX_AUDIO_SAMPLE_SIZE_18_BITS:
         gm_Print(" SampleSize        = %d : 18", PC_AudioPropertyEvtData.Property.SampleSize);
         break;
      case STHDMIRX_AUDIO_SAMPLE_SIZE_19_BITS:
         gm_Print(" SampleSize        = %d : 19", PC_AudioPropertyEvtData.Property.SampleSize);
         break;
      case STHDMIRX_AUDIO_SAMPLE_SIZE_20_BITS:
         gm_Print(" SampleSize        = %d : 20", PC_AudioPropertyEvtData.Property.SampleSize);
         break;
      case STHDMIRX_AUDIO_SAMPLE_SIZE_21_BITS:
         gm_Print(" SampleSize        = %d : 21", PC_AudioPropertyEvtData.Property.SampleSize);
         break;
      case STHDMIRX_AUDIO_SAMPLE_SIZE_22_BITS:
         gm_Print(" SampleSize        = %d : 22", PC_AudioPropertyEvtData.Property.SampleSize);
         break;
      case STHDMIRX_AUDIO_SAMPLE_SIZE_23_BITS:
         gm_Print(" SampleSize        = %d : 23", PC_AudioPropertyEvtData.Property.SampleSize);
         break;
      case STHDMIRX_AUDIO_SAMPLE_SIZE_24_BITS:
         gm_Print(" SampleSize        = %d : 24", PC_AudioPropertyEvtData.Property.SampleSize);
         break;
   }

   gm_Print(" DownMixInhibit    = %d", PC_AudioPropertyEvtData.Property.DownMixInhibit);
   gm_Print(" LevelShift        = %d", PC_AudioPropertyEvtData.Property.LevelShift);

   switch (PC_AudioPropertyEvtData.Property.LFEPlayBackLevel)
   {
      case STHDMIRX_AUDIO_LFE_PLAYBACK_LEVEL_UNKNOWN:
         gm_Print(" LFEPlayBackLevel  = %d : UnKnown", PC_AudioPropertyEvtData.Property.LFEPlayBackLevel);
         break;
      case STHDMIRX_AUDIO_LFE_PLAYBACK_LEVEL_0dB:
         gm_Print(" LFEPlayBackLevel  = %d : 0dB",     PC_AudioPropertyEvtData.Property.LFEPlayBackLevel);
         break;
      case STHDMIRX_AUDIO_LFE_PLAYBACK_LEVEL_10dBs:
         gm_Print(" LFEPlayBackLevel  = %d : 10dB",    PC_AudioPropertyEvtData.Property.LFEPlayBackLevel);
         break;
   }
}


static BYTE GetActiveInputPortByDigitalPhyId(gmt_HDMI_DVI_DID b_DID)
{
   BYTE i;
   for(i=0;i<NUM_CHANNELS;i++)
   {
      if(gm_GetHdmiDviDigitalID(gB_CurrentInputConnector[i])==b_DID)
         return gB_CurrentInputConnector[i];
   }
   return NOT_USED;
}

#if (USE_HDMIDVI_AFR_DETECT_INTERRUPT!=0)
void HdmiDviEnableAfrInterrupt(BYTE InputConnector, BYTE b_Flag)
{
   gmt_HDMI_DVI_DID b_DID=gm_GetHdmiDviDigitalID(InputConnector);
   if(b_DID==DID_HDMI)
   {
      if(b_Flag==TRUE)
         gm_SetRegBitsWord(HDMI_INPUT_IRQ_MASK,HDMI_INPUT_AFR_DETECT_MASK);
      else
         gm_ClearRegBitsWord(HDMI_INPUT_IRQ_MASK,HDMI_INPUT_AFR_DETECT_MASK);
   }
   else if((b_DID==DID_DVI0) ||(b_DID==DID_DVI_DUAL))
   {
      if(b_Flag==TRUE)
         gm_SetRegBitsWord(DVI0_INPUT_IRQ_MASK,DVI0_INPUT_AFR_DETECT_MASK);
      else
         gm_ClearRegBitsWord(DVI0_INPUT_IRQ_MASK,DVI0_INPUT_AFR_DETECT_MASK);
   }
}

static void HdmiDviAFR_Detected(BYTE InputConnector)
{
   if(InputConnector == gB_CurrentInputConnector[gmvb_MainChannel])
   {
      gm_SetRegBitsWord(PB_CTRL, PB_FORCE_MAIN_BKGND);
      gm_ForceUpdate(CH_A, INPUT_OUTPUT_CLOCK);
   }
   else if(InputConnector == gB_CurrentInputConnector[gmvb_PipChannel])
   {
      gm_SetRegBitsWord(PB_CTRL, PB_FORCE_PIP_BKGND);
      gm_ForceUpdate(CH_B, INPUT_OUTPUT_CLOCK);
   }
}
#endif

void far HdmiDvi_Interrupt(void)
{
#if (USE_HDMIDVI_HDCP_KSV_INTERRUPT!=0) || (USE_HDMIDVI_AFR_DETECT_INTERRUPT!=0)
   BYTE InputConnector;
#endif

#if (USE_HDMIDVI_AFR_DETECT_INTERRUPT!=0)
   WORD hdmiInputIrqStatus = gm_ReadRegWord(HDMI_INPUT_IRQ_STATUS);
   WORD hdmiInputIrqMask   = gm_ReadRegWord(HDMI_INPUT_IRQ_MASK);
   WORD DviInputIrqStatus = gm_ReadRegWord(DVI0_INPUT_IRQ_STATUS);
   WORD DviInputIrqMask   = gm_ReadRegWord(DVI0_INPUT_IRQ_MASK);
#endif

#if (USE_HDMIDVI_HDCP_KSV_INTERRUPT!=0)
   gmt_HdmiDviInputPort* ptr_PortConfig;
   WORD HdmiHdcpKsvFifoSts   = gm_ReadRegWord(HDRX_HDCP_KSV_FIFO_STS);
   WORD HdmiHdmiKsvFifoCtrl  = gm_ReadRegWord(HDRX_HDCP_KSV_FIFO_CTRL);
   WORD DviHdcpKsvFifoSts   = gm_ReadRegWord(DVI0_HDCP_KSV_FIFO_STS);
   WORD DviHdmiKsvFifoCtrl  = gm_ReadRegWord(DVI0_HDCP_KSV_FIFO_CTRL);
#endif   

#if (USE_HDMIDVI_AFR_DETECT_INTERRUPT!=0)

   if(hdmiInputIrqStatus & hdmiInputIrqMask)
   {
      if(hdmiInputIrqStatus & HDMI_INPUT_AFR_DETECT)
      {
         gm_ClearRegBitsWord(HDMI_INPUT_IRQ_MASK,HDMI_INPUT_AFR_DETECT_MASK);
         InputConnector=GetActiveInputPortByDigitalPhyId(DID_HDMI);
         if(InputConnector!=NOT_USED)
            HdmiDviAFR_Detected(InputConnector);
      }
   }


   if(DviInputIrqStatus & DviInputIrqMask)
   {
      if(DviInputIrqStatus & DVI0_INPUT_AFR_DETECT)
      {
         gm_ClearRegBitsWord(DVI0_INPUT_IRQ_MASK,DVI0_INPUT_AFR_DETECT_MASK);
         InputConnector=GetActiveInputPortByDigitalPhyId(DID_DVI0);
         if(InputConnector!=NOT_USED)
         {
            HdmiDviAFR_Detected(InputConnector);
         }
         else
         {
            InputConnector=GetActiveInputPortByDigitalPhyId(DID_DVI_DUAL);
            if(InputConnector!=NOT_USED)
               HdmiDviAFR_Detected(InputConnector);
         }
      }   

   }
#endif //#if (USE_HDMIDVI_AFR_DETECT_INTERRUPT!=0)

#if (USE_HDMIDVI_HDCP_KSV_INTERRUPT!=0)
       
   if(HdmiHdcpKsvFifoSts & HdmiHdmiKsvFifoCtrl)
   {
      InputConnector=GetActiveInputPortByDigitalPhyId(DID_HDMI);
      if(InputConnector!=NOT_USED)
      {
         ptr_PortConfig=&HdmiDviInPortConfig[GetInPortConfigArrayLocation(InputConnector)];
//         gm_Print("Hk-%x",HdmiHdcpKsvFifoSts);
         STHDMIRX_HdcpKsvServiceRoutine (PC_InputHandle);
      }
   }


   if(DviHdcpKsvFifoSts & DviHdmiKsvFifoCtrl)
   {
      InputConnector=GetActiveInputPortByDigitalPhyId(DID_DVI0);
      if(InputConnector!=NOT_USED)
      {
         ptr_PortConfig=&HdmiDviInPortConfig[GetInPortConfigArrayLocation(InputConnector)];
//         gm_Print("Dk-%x",DviHdcpKsvFifoSts);
         STHDMIRX_HdcpKsvServiceRoutine (PC_InputHandle);
      }
      else
      {
         InputConnector=GetActiveInputPortByDigitalPhyId(DID_DVI_DUAL);
         
         if(InputConnector!=NOT_USED)
         {
            ptr_PortConfig=&HdmiDviInPortConfig[GetInPortConfigArrayLocation(InputConnector)];
//            gm_Print("Dk-%x",DviHdcpKsvFifoSts);
            STHDMIRX_HdcpKsvServiceRoutine (PC_InputHandle);
         }
      }
         
   }
#endif  //#if (USE_HDMIDVI_HDCP_KSV_INTERRUPT!=0)
}

BOOL HdmiGetITContentDoOverScan(BYTE InputConnector)//20120719 Add by KevinChen for ITC flage do overscan
{
                BYTE b_HdmiDviInNo = GetInPortConfigArrayLocation(InputConnector);
                gmt_HdmiDviInputPort* ptr_PortConfig;
                BOOL DoOVerScan=TRUE;
                if (b_HdmiDviInNo >= NUM_OF_HDMIDVI_INP)
                {
                                gm_Print("Wrong InputConnector number",0);
                                //return 0;
                }
                ptr_PortConfig= &HdmiDviInPortConfig[b_HdmiDviInNo];
                #if 0
                gm_Print("IT Content - %d",PC_VideoPropertyEvtData.Property.ITContent);
                gm_Print("Content type - %d",PC_VideoPropertyEvtData.Property.ContentType);
                switch(PC_VideoPropertyEvtData.Property.ContentType)
                {
                                case STHDMIRX_CONTENT_TYPE_UNKNOWN:
                                                gm_Print("STHDMIRX_CONTENT_TYPE_UNKNOWN",0);
                                                break;
                                case STHDMIRX_CONTENT_TYPE_GRAPHICS:
                                                gm_Print("STHDMIRX_CONTENT_TYPE_GRAPHICS",0);
                                                break;
                                case STHDMIRX_CONTENT_TYPE_PHOTO:
                                                gm_Print("STHDMIRX_CONTENT_TYPE_PHOTO",0);
                                                break;
                                case STHDMIRX_CONTENT_TYPE_CINEMA:
                                                gm_Print("STHDMIRX_CONTENT_TYPE_CINEMA",0);
                                                break;
                                case STHDMIRX_CONTENT_TYPE_GAME:
                                                gm_Print("STHDMIRX_CONTENT_TYPE_GAME",0);
                                                break;
                }
                #endif
                if(PC_VideoPropertyEvtData.Property.ITContent!=0)// && PC_VideoPropertyEvtData.Property.ContentType==0)
                                DoOVerScan=FALSE;
                return DoOVerScan;
}


#if (USE_MINI_ISR_HANDLER==1)
void far HdmiDvi_MiniIsrHandler(void)
{
   BYTE b_InputConnector;
   BYTE b_HdmiDviInNo;
   gmt_HdmiDviInputPort* ptr_PortConfig;

   for (b_HdmiDviInNo =0; b_HdmiDviInNo < NUM_OF_HDMIDVI_INP; b_HdmiDviInNo++)
   {
      ptr_PortConfig= &HdmiDviInPortConfig[b_HdmiDviInNo];
      if((PC_IsActivePort==TRUE) && (b_Initialed==TRUE) /*&& (gm_ReadRegByte(IMP_PATGEN_BLUE)==1)*/)
      {
         STHDMIRX_InterruptHandler(PC_InputHandle);
         //gm_WriteRegByte(IMP_PATGEN_BLUE,gm_ReadRegByte(IMP_PATGEN_BLUE)+1);
      }
   }
}
#endif

#if (DEBUG_HDMI_PRINT_TIMING_DISPLAY_TIME!=0)
void SetHdmiDebugDisplayTime(void)
{
   gm_Print("*UdDisT",0);
   dw_DebugDisplayTime=gm_GetSystemTime();
}

DWORD GetHdmiDebugDisplayTime(void)
{
   return dw_DebugDisplayTime;
}
#endif

//******************************************************************************
// HDMI Bathtub
//******************************************************************************
#if (DEBUG_HDMI_EQ_BATHTUB !=0)
#define GAIN_NUM  8
#define PEAK_NUM  8
typedef struct
{
   WORD PhySigQual;
   WORD PhyDecErr;
}gmt_HdmiQualSts;

void Hdmi_GetSignalQualityStatus(BYTE InputConnector, gmt_HdmiQualSts *pResult)
{
   gmt_HdmiDviInputPort* ptr_PortConfig=&HdmiDviInPortConfig[GetInPortConfigArrayLocation(InputConnector)];

   WORD ulDecErr_0, ulDecErr_1, ulDecErr_2;

   pResult->PhySigQual= gm_ReadRegWord((WORD)(PC_InitParams.DeviceBaseAddress_p)+HDMIRX_PHY_SIGQUAL);
   if(gm_ReadRegWord((WORD)(PC_InitParams.DeviceBaseAddress_p)+HDMIRX_PHY_SIGQUAL+2))
   {
     pResult->PhySigQual = 0xffff;
   }

   gm_WriteRegByte((WORD)(PC_InitParams.DeviceBaseAddress_p)+HDMIRX_LINK_ERR_CTRL, 0x00);
   ulDecErr_0 = gm_ReadRegWord((WORD)(PC_InitParams.DeviceBaseAddress_p)+HDMIRX_PHY_DEC_ERR_STATUS) & 0x0fff;

   gm_WriteRegByte((WORD)(PC_InitParams.DeviceBaseAddress_p)+HDMIRX_LINK_ERR_CTRL, 0x01);
   ulDecErr_1= gm_ReadRegWord((WORD)(PC_InitParams.DeviceBaseAddress_p)+HDMIRX_PHY_DEC_ERR_STATUS) & 0x0fff;

   gm_WriteRegByte((WORD)(PC_InitParams.DeviceBaseAddress_p)+HDMIRX_LINK_ERR_CTRL, 0x02);
   ulDecErr_2 = gm_ReadRegWord((WORD)(PC_InitParams.DeviceBaseAddress_p)+HDMIRX_PHY_DEC_ERR_STATUS) & 0x0fff;

   pResult->PhyDecErr = ulDecErr_0+ulDecErr_1+ulDecErr_2;
   
}

void Hdmi_RunEQ_Bathtub(BYTE InputConnector)
{
   gmt_HdmiDviInputPort* ptr_PortConfig=&HdmiDviInPortConfig[GetInPortConfigArrayLocation(InputConnector)];
   WORD IbCntl;
   WORD Gain;
   WORD Peak;
   gmt_HdmiQualSts EqBathtub[PEAK_NUM][GAIN_NUM];
   gmt_HdmiQualSts *pSts;
   WORD bakHdmiAFR, bakHdmiAFB, bakAfrControl;
   DWORD bakCPHY_L1_CTRL_0,bakCPHY_L2_CTRL_0,bakCPHY_L3_CTRL_0;
   DWORD bakCPHY_L1_CTRL_4,bakCPHY_L2_CTRL_4,bakCPHY_L3_CTRL_4;
   DWORD bak2CPHY_L1_CTRL_0,bak2CPHY_L2_CTRL_0,bak2CPHY_L3_CTRL_0;
   DWORD bak2CPHY_L1_CTRL_4,bak2CPHY_L2_CTRL_4,bak2CPHY_L3_CTRL_4;


   //BackUp 
   bakHdmiAFR=gm_ReadRegWord((WORD)(PC_InitParams.DeviceBaseAddress_p)+HDRX_IFM_INSTRUMENT_PU_ADDRS_OFFSET+HDMI_RX_AFR_CONTROL);
   bakHdmiAFB=gm_ReadRegWord((WORD)(PC_InitParams.DeviceBaseAddress_p)+HDRX_IFM_INSTRUMENT_PU_ADDRS_OFFSET+HDMI_RX_AFB_CONTROL);
   bakAfrControl=gm_ReadRegWord(AFR_CONTROL);

   bakCPHY_L1_CTRL_0=gm_ReadRegDWord((WORD)(PC_InitParams.PhyBaseAddress_p)+CPHY_L1_CTRL_0);
   bakCPHY_L2_CTRL_0=gm_ReadRegDWord((WORD)(PC_InitParams.PhyBaseAddress_p)+CPHY_L2_CTRL_0);
   bakCPHY_L3_CTRL_0=gm_ReadRegDWord((WORD)(PC_InitParams.PhyBaseAddress_p)+CPHY_L3_CTRL_0);   
  
   bakCPHY_L1_CTRL_4=gm_ReadRegDWord((WORD)(PC_InitParams.PhyBaseAddress_p)+CPHY_L1_CTRL_4);
   bakCPHY_L2_CTRL_4=gm_ReadRegDWord((WORD)(PC_InitParams.PhyBaseAddress_p)+CPHY_L2_CTRL_4);
   bakCPHY_L3_CTRL_4=gm_ReadRegDWord((WORD)(PC_InitParams.PhyBaseAddress_p)+CPHY_L3_CTRL_4);  

   if(PC_InitParams.PhyBaseAddress_p2 != NULL)
   {
      bak2CPHY_L1_CTRL_0=gm_ReadRegDWord((WORD)(PC_InitParams.PhyBaseAddress_p2)+CPHY_L1_CTRL_0);
      bak2CPHY_L2_CTRL_0=gm_ReadRegDWord((WORD)(PC_InitParams.PhyBaseAddress_p2)+CPHY_L2_CTRL_0);
      bak2CPHY_L3_CTRL_0=gm_ReadRegDWord((WORD)(PC_InitParams.PhyBaseAddress_p2)+CPHY_L3_CTRL_0);   

      bak2CPHY_L1_CTRL_4=gm_ReadRegDWord((WORD)(PC_InitParams.PhyBaseAddress_p2)+CPHY_L1_CTRL_4);
      bak2CPHY_L2_CTRL_4=gm_ReadRegDWord((WORD)(PC_InitParams.PhyBaseAddress_p2)+CPHY_L2_CTRL_4);
      bak2CPHY_L3_CTRL_4=gm_ReadRegDWord((WORD)(PC_InitParams.PhyBaseAddress_p2)+CPHY_L3_CTRL_4);  
   }

   
    //Clear AFR
   gm_WriteRegWord((WORD)(PC_InitParams.DeviceBaseAddress_p)+HDRX_IFM_INSTRUMENT_PU_ADDRS_OFFSET+HDMI_RX_AFR_CONTROL,0);
   gm_WriteRegWord((WORD)(PC_InitParams.DeviceBaseAddress_p)+HDRX_IFM_INSTRUMENT_PU_ADDRS_OFFSET+HDMI_RX_AFB_CONTROL,0);
   gm_WriteRegWord(AFR_CONTROL,0);
   gm_ClearRegBitsWord(OVP_CTRL,MAIN_AFB_EN);

   //Set IbCntl
   for(IbCntl=0; IbCntl<4; IbCntl++)
   {
      WORD ibCtrl;
      ibCtrl = (((WORD)(IbCntl & 0x03)<<CPHY0_L0_IB_CNTL_SHIFT));
      gm_ClearAndSetBitsWord((WORD)(PC_InitParams.PhyBaseAddress_p)+CPHY_L1_CTRL_0,CPHY0_L0_IB_CNTL, ibCtrl);
      gm_ClearAndSetBitsWord((WORD)(PC_InitParams.PhyBaseAddress_p)+CPHY_L2_CTRL_0,CPHY0_L0_IB_CNTL, ibCtrl);
      gm_ClearAndSetBitsWord((WORD)(PC_InitParams.PhyBaseAddress_p)+CPHY_L3_CTRL_0,CPHY0_L0_IB_CNTL, ibCtrl);   

      if(PC_InitParams.PhyBaseAddress_p2 != NULL)
      {
         gm_ClearAndSetBitsWord((WORD)(PC_InitParams.PhyBaseAddress_p2)+CPHY_L1_CTRL_0,CPHY0_L0_IB_CNTL, ibCtrl);
         gm_ClearAndSetBitsWord((WORD)(PC_InitParams.PhyBaseAddress_p2)+CPHY_L2_CTRL_0,CPHY0_L0_IB_CNTL, ibCtrl);
         gm_ClearAndSetBitsWord((WORD)(PC_InitParams.PhyBaseAddress_p2)+CPHY_L3_CTRL_0,CPHY0_L0_IB_CNTL, ibCtrl);   
      }

      for(Peak=0; Peak<8; Peak++)
      {
         WORD FrqCtrl;
         //Set Peak
         FrqCtrl = (((WORD)(Peak & 0x07)<<HDMI_RX_PHY_EQ_PEAK_SHIFT));

         //PHY_SET_REGBITS_WORD((U32)(PhyControl_p->BaseAddress_p+CPHY_L0_CTRL_4+2),FrqCtrl);
         gm_ClearAndSetBitsWord((WORD)(PC_InitParams.PhyBaseAddress_p)+CPHY_L1_CTRL_4+2,HDMI_RX_PHY_EQ_PEAK, FrqCtrl);
         gm_ClearAndSetBitsWord((WORD)(PC_InitParams.PhyBaseAddress_p)+CPHY_L2_CTRL_4+2,HDMI_RX_PHY_EQ_PEAK, FrqCtrl);
         gm_ClearAndSetBitsWord((WORD)(PC_InitParams.PhyBaseAddress_p)+CPHY_L3_CTRL_4+2,HDMI_RX_PHY_EQ_PEAK, FrqCtrl);
         if(PC_InitParams.PhyBaseAddress_p2 != NULL)
         {
            gm_ClearAndSetBitsWord((WORD)(PC_InitParams.PhyBaseAddress_p2)+CPHY_L1_CTRL_4+2,HDMI_RX_PHY_EQ_PEAK, FrqCtrl);
            gm_ClearAndSetBitsWord((WORD)(PC_InitParams.PhyBaseAddress_p2)+CPHY_L2_CTRL_4+2,HDMI_RX_PHY_EQ_PEAK, FrqCtrl);
            gm_ClearAndSetBitsWord((WORD)(PC_InitParams.PhyBaseAddress_p2)+CPHY_L3_CTRL_4+2,HDMI_RX_PHY_EQ_PEAK, FrqCtrl);
         }

         for(Gain=0; Gain<8; Gain++)
         {
            WORD EqCtrl=0;
            EqCtrl = (((WORD)(Gain & 0x07)<<HDMI_RX_PHY_EQ_GAIN_SHIFT));
            //Set Gain         
            gm_ClearAndSetBitsWord((WORD)(PC_InitParams.PhyBaseAddress_p)+CPHY_L1_CTRL_0+2,HDMI_RX_PHY_EQ_GAIN, EqCtrl);
            gm_ClearAndSetBitsWord((WORD)(PC_InitParams.PhyBaseAddress_p)+CPHY_L2_CTRL_0+2,HDMI_RX_PHY_EQ_GAIN, EqCtrl);
            gm_ClearAndSetBitsWord((WORD)(PC_InitParams.PhyBaseAddress_p)+CPHY_L3_CTRL_0+2,HDMI_RX_PHY_EQ_GAIN, EqCtrl);
            if(PC_InitParams.PhyBaseAddress_p2 != NULL)
            {
               gm_ClearAndSetBitsWord((WORD)(PC_InitParams.PhyBaseAddress_p2)+CPHY_L1_CTRL_0+2,HDMI_RX_PHY_EQ_GAIN, EqCtrl);
               gm_ClearAndSetBitsWord((WORD)(PC_InitParams.PhyBaseAddress_p2)+CPHY_L2_CTRL_0+2,HDMI_RX_PHY_EQ_GAIN, EqCtrl);
               gm_ClearAndSetBitsWord((WORD)(PC_InitParams.PhyBaseAddress_p2)+CPHY_L3_CTRL_0+2,HDMI_RX_PHY_EQ_GAIN, EqCtrl);
            }

            gm_Delay1ms(2);   // Delay 2ms to get stable error cnt
            //Clear Decode Err
            gm_SetRegBitsByte((WORD)(PC_InitParams.DeviceBaseAddress_p)+HDMIRX_LINK_ERR_CTRL, HDRX_PHY_DEC_ERR_CLR);
            gm_ClearRegBitsByte((WORD)(PC_InitParams.DeviceBaseAddress_p)+HDMIRX_LINK_ERR_CTRL, HDRX_PHY_DEC_ERR_CLR);

            //Clear QualScore 
            gm_SetRegBitsDWord((WORD)(PC_InitParams.DeviceBaseAddress_p)+HDMIRX_PHY_CTRL ,HDMI_RX_PHY_CTRL_CLR_SCR);
            gm_ClearRegBitsDWord((WORD)(PC_InitParams.DeviceBaseAddress_p)+HDMIRX_PHY_CTRL ,HDMI_RX_PHY_CTRL_CLR_SCR);
            gm_Delay1ms(50);
            gm_SetRegBitsDWord((WORD)(PC_InitParams.DeviceBaseAddress_p)+HDMIRX_PHY_CTRL ,HDMI_RX_PHY_CTRL_CLR_SCR);
            gm_ClearRegBitsDWord((WORD)(PC_InitParams.DeviceBaseAddress_p)+HDMIRX_PHY_CTRL ,HDMI_RX_PHY_CTRL_CLR_SCR);

            gm_Delay1ms(50);

            pSts= &(EqBathtub[Peak][Gain]);
            Hdmi_GetSignalQualityStatus(InputConnector, pSts);
         }
      }


      //Print SigQual Result
      gm_Print("===IbCntl(%d)",IbCntl);
      gm_Print("SigQual",0);
      for(Peak=0; Peak<8; Peak++)
      {
         gm_Print("Peak%d",Peak);
         for(Gain=0; Gain<8; Gain++)
         {
            pSts= &(EqBathtub[Peak][Gain]);
            gm_Print("%d",pSts->PhySigQual);
            gm_Delay1ms(5);
         }
         gm_Delay1ms(5);
      }

      //Print SigDecErr Result
      gm_Print("SigDecErr",0);
      for(Peak=0; Peak<8; Peak++)
      {
         gm_Print("Peak%d",Peak);
         for(Gain=0; Gain<8; Gain++)
         {
            pSts= &(EqBathtub[Peak][Gain]);
            gm_Print("%d",pSts->PhyDecErr);
            gm_Delay1ms(5);
         }
         gm_Delay1ms(5);
      }

   }

   // Restore IbCntl/Peak/Gain
   gm_WriteRegDWord((WORD)(PC_InitParams.PhyBaseAddress_p)+CPHY_L1_CTRL_0,bakCPHY_L1_CTRL_0);
   gm_WriteRegDWord((WORD)(PC_InitParams.PhyBaseAddress_p)+CPHY_L2_CTRL_0,bakCPHY_L2_CTRL_0);
   gm_WriteRegDWord((WORD)(PC_InitParams.PhyBaseAddress_p)+CPHY_L3_CTRL_0,bakCPHY_L3_CTRL_0);   
  
   gm_WriteRegDWord((WORD)(PC_InitParams.PhyBaseAddress_p)+CPHY_L1_CTRL_4,bakCPHY_L1_CTRL_4);
   gm_WriteRegDWord((WORD)(PC_InitParams.PhyBaseAddress_p)+CPHY_L2_CTRL_4,bakCPHY_L2_CTRL_4);
   gm_WriteRegDWord((WORD)(PC_InitParams.PhyBaseAddress_p)+CPHY_L3_CTRL_4,bakCPHY_L3_CTRL_4);   

   if(PC_InitParams.PhyBaseAddress_p2 != NULL)
   {
      gm_WriteRegDWord((WORD)(PC_InitParams.PhyBaseAddress_p2)+CPHY_L1_CTRL_0,bak2CPHY_L1_CTRL_0);
      gm_WriteRegDWord((WORD)(PC_InitParams.PhyBaseAddress_p2)+CPHY_L2_CTRL_0,bak2CPHY_L2_CTRL_0);
      gm_WriteRegDWord((WORD)(PC_InitParams.PhyBaseAddress_p2)+CPHY_L3_CTRL_0,bak2CPHY_L3_CTRL_0);   
      
      gm_WriteRegDWord((WORD)(PC_InitParams.PhyBaseAddress_p2)+CPHY_L1_CTRL_4,bak2CPHY_L1_CTRL_4);
      gm_WriteRegDWord((WORD)(PC_InitParams.PhyBaseAddress_p2)+CPHY_L2_CTRL_4,bak2CPHY_L2_CTRL_4);
      gm_WriteRegDWord((WORD)(PC_InitParams.PhyBaseAddress_p2)+CPHY_L3_CTRL_4,bak2CPHY_L3_CTRL_4);   
   }

   gm_Delay1ms(50);
   
   // Reset AFR
   gm_WriteRegWord(HDMI_INPUT_IRQ_STATUS,0xFFFF);
   gm_WriteRegWord(MAIN_INPUT_STATUS,0xFFFF);
   gm_WriteRegWord(DISPLAY_DEC_STATUS,0xFFFF);

   
   gm_WriteRegWord((WORD)(PC_InitParams.DeviceBaseAddress_p)+HDRX_IFM_INSTRUMENT_PU_ADDRS_OFFSET+HDMI_RX_AFR_CONTROL,bakHdmiAFR);
   gm_WriteRegWord((WORD)(PC_InitParams.DeviceBaseAddress_p)+HDRX_IFM_INSTRUMENT_PU_ADDRS_OFFSET+HDMI_RX_AFB_CONTROL,bakHdmiAFB);
   gm_WriteRegWord(AFR_CONTROL,bakAfrControl);
   gm_SetRegBitsWord(OVP_CTRL,MAIN_AFB_EN);
}
#endif //DEBUG_HDMI_EQ_BATHTUB

#else //(FEATURE_HDMI==ENABLE)
BOOL gm_IsHdmiTiming(BYTE InputConnector)
{
   UNUSED_VAR(InputConnector);
   return FALSE;
}
#endif //(FEATURE_HDMI==ENABLE)


/*********************************  END  **************************************/
