/*
    $Workfile:   Init.c  $
    $Revision: 1.340 $
    $Date: 2013/06/14 02:38:02 $
*/

#define __INIT_C__
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
// MODULE:      init.c
//
// USAGE:       This module contains application initialization
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#include "System\All.h"
#include "system\Sdram.h"

#if (FEATURE_ACM_3D == ENABLE)
#include "StdLibInc\gm_ACM3D.h"
#endif
#if(FEATURE_ACC3 == ENABLE)
#include "StdLibInc\gm_ACC3.h"
#endif

#if (FEATURE_LOW_POWER_MONITOR == ENABLE)
#include "StdLibInc\gm_LPM_IPC.h"
#endif

#if (FEATURE_INTERRUPT_SERVICE == REV2)
#include "StdLibInc\gm_Mcu186.h"
#endif

#if (FEATURE_ACR == ENABLE)
#include "system\ACR_Handler.h"
#endif

//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************

//#define _DEBUG_MSG
#ifdef _DEBUG_MSG
#define msg(x)            gm_Print(x,0)
#define msg_i(x,y)        gm_Print(x,y)
#else
#define msg(x,y)
#define msg_i(x,y)
#endif

#define VERSION_TAG_ADDR               0x410004UL
#define DUAL_FLASH_EN		            BIT3
#define IS_DUAL_FLASH_ENABLE()        (gm_ReadRegWord(BOOTSTRAP) & DUAL_FLASH_EN)
#define IS_CURRENT_FLASH_BANK_B()      (gm_ReadRegWord(SERIAL_FLASH_CTRL2) & SPI_BANK_SEL)

//
// OSD
//
#ifdef MAX_NUMBER_OF_TILES_USED
#if (MAX_NUMBER_OF_TILES_USED > 15)    // 15 means 16 total tiles since start from 0
#error "Too many tiles are used by the OSD"
#endif
#endif //#ifdef MAX_NUMBER_OF_TILES_USED

#define TILE_COMMAND_SRAM_OFFSET \
        (sizeof(ST_SRAM_TILE) * MAX_NUMBER_OF_TILES_USED - sizeof(WORD))

#define S_THRESHOLD_LUT_SIZE     60

#if (FEATURE_LOW_POWER_MONITOR == ENABLE)
#define NUM_OF_RX_QUEUES         4
#define QUEUE_LENGTH             10 //in bytes
#define MESSAGE_SEND_TIMEOUT    500 //in ms
#define RCOSC_DELTA           6 // Tolerance
#define RCOSC_EQUAL(a,b) ((a) < (b)+RCOSC_DELTA) && ((a) > (b)-RCOSC_DELTA)

static gmt_QUEUE Queue[NUM_OF_RX_QUEUES];
static BYTE B_Buffer[NUM_OF_RX_QUEUES * (QUEUE_LENGTH+3)]; // add 3 bytes to avoid queue buffer overflow for gm_MessageInIsr() write data with DWORD size
static gmt_Power_Mode_Switch LPM_PwrStatus;
static BYTE IsWarmReset = 0;
static BYTE RcOscTrim = 0;
static WORD MeasRcOscVal = 0x3FF;

typedef struct
{
   WORD Value;
   BYTE Count;
} gmt_RcOscClkMeas;
#endif
//******************************************************************************
//  E X T E R N A L
//******************************************************************************
#if (FEATURE_HDMI==ENABLE)
	#define HDCP_DDC_PORT_ADDR 0x3A
#endif 

extern gmt_AcmColorStruct St_AcmColorAdjust;
extern DWORD gd_MemoryMainStart;

extern void SaveOriginalValue    (gmt_PHY_CH B_Channel);
/*ChengLin 20130221: Athena Right Image Sync INT1 Flag*/
#ifdef ST_4K2K_93xx_BOARD
#if ENABLE_ATHENA_R_L_IMAGE_SYNC_HANDLER
extern BOOL ImageSyncINT1_Flag;
#endif
#if ENABLE_PBP_ATHENA_L_DEEP_SLEEP_CHECK               
extern BOOL AthenaRIgnoreSleep_PBP;            
extern BOOL PBP_AthenaR_ReCheck_AthenaLStatus;
#endif
#endif

extern void BoardInit(void);
extern void InitializeNVRAM(void);
extern void TeletextBlenderOveride(BOOL B_Enable);
extern void far ServiceDDC2Bi(WORD dummy);
extern void SaveUserPref(gmt_OSD_Adjuster_Info * AdjusterInfo);
extern void RetrieveUserPref(gmt_OSD_Adjuster_Info * AdjusterInfo);
extern void SetOSDMenuState(BYTE MenuState);

#if (FEATURE_LOW_POWER_MONITOR == ENABLE)
typedef struct
{
   BYTE ROM * Ba_Array;
   WORD W_StartingOffset;
   WORD W_Length;
} TDumpDescriptor;

extern TDumpDescriptor ROM FLASHDriverDumps[];
static void LPM_StartUp(void);
static void LPM_SharedPBusRequest(gmt_BusOwn PBusOwner);
static void LPM_MuxingRegDisable(void);
BYTE LPM_ReceiveStatus(void);
#endif

#if (FEATURE_INTERRUPT_SERVICE == REV2)
static void far interrupt IrqZeroManager(void);
#endif

#if (FEATURE_CEC == ENABLE)
void CecInit(void);
#endif

#if FEATURE_DISPLAY_PORT == ENABLE
extern void DpRxLoadEdid2Buf(void);
extern void DpRxLoadEdid2LpmBuf(void);
#endif

void OsdPatch(BYTE value);

// ThEnter32
extern WORD gW_AlgVtToAfpThresh1;
extern WORD	gW_AlgVtToAfpMidlowThresh;

// ThLeave32
extern WORD gW_AlgAfpToVtThresh2;
extern WORD	gW_AlgAfpToVtThresh3;
extern WORD	gW_AlgAfpToVtThresh4;
extern WORD	gW_AlgAfpToVtThresh5;

// ThEnter22
extern WORD gW_AlgVtToGfxMatches;

// ThLeave22
extern WORD gW_AlgGfxToVtThresh1;
extern WORD	gW_AlgGfxToVtThresh2;

// Rej32LowMotion
extern WORD gW_AlgSwitch;

// AFM_32Persis
extern WORD gW_AlgAfpPersistThresh;
extern WORD gW_AlgAfpPersistThresh2;

#ifdef USE_SMT
extern VQ_Sharpness_Parameters_t SMT_RAM VQ_Sharpness_Para_TEMP;
#endif
//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************

#if FEATURE_AIP == ENABLE
//
// ADC default gain, offset values
//
ROM gmt_ADC_SETTING  AdcDefaultSetting[NumberOfAdcSetting]=
{
   #if 1
   {0x40,0x40,0x47,0x87,0x7E,0x98,0x006F,0x0069,0x0069},    // ADC_SETTING_GRAPHIC
   {0x22,0x22,0x22,0x40,0x40,0x40,0x0010,0x0010,0x0010},    // ADC_SETTING_HD
   {0x22,0x22,0x22,0x40,0x40,0x40,0x0010,0x0010,0x0010},    // ADC_SETTING_HD2
   #else
   {0x21,0x22,0x23,0x41,0x42,0x43,0x0021,0x0022,0x0023},    // ADC_SETTING_GRAPHIC
   {0x22,0x22,0x22,0x40,0x40,0x40,0x0010,0x0010,0x0010},    // ADC_SETTING_HD
   {0x22,0x22,0x22,0x40,0x40,0x40,0x0010,0x0010,0x0010},    // ADC_SETTING_HD2
   #endif
};
#endif

//
// global OSD buffer for pixel expansion 8bpp->8bpp, 8bpp->16bpp,
// and for text strings decompression
//
#ifdef OSD_MAX_TEXT_BUFFER_SIZE
#if (OSD_MAX_TEXT_BUFFER_SIZE != 0)
WORD PixelExpansionTable[(OSD_MAX_TEXT_BUFFER_SIZE + 1) / 2] = {0};
#endif
#endif

#ifdef OSD_WB_USED
//
// array to keep track of all osd thread states
//
gmt_OSD_SM_THREAD_CONTEXT gmv_OsdThreadContextArray[NUMBER_OF_THREADS];
#endif

// WB4 -----------------------
#if(WB_MAJOR == 0x0004)
BYTE AdjEngineVer = WB_ADJUSTER_ENGINE_VERSION;
#else
BYTE AdjEngineVer = 0x03;
#endif
// --------------------------

// Which 100Hz/120Hz improvement method is enabled.
#if (defined(ACC_LUT_METHOD))
BYTE Method_ACC_LUT_Enable = FALSE;
#endif
BYTE Method_Black_Frame_Insertion_Enable = FALSE;
BYTE Method_Grey_Frame_Insertion_Enable = FALSE;
// 20130305
#ifdef ST_4K2K_93xx_BOARD
BYTE TempMainPort = ALL_BOARDS_DP2 ; //11; // DP2 (HDMI)
BYTE SplashAspectRatio = OUTPUT_AR_FULL;
BOOL BackFlag = 0;
BOOL AllowKeypadAction = FALSE;
#endif

VQ_AFM_Parameters_t ROM AFMParams =
{
	{
		sizeof(VQ_AFM_Parameters_t),
		//STLAYER_VIDEO_AFM,
		0
	},
	3,		// Afm_Mode
	14,		// RTh
	14,		// STh
	8,		// VMTh1
	8,		// VMTh2
	1,		// VMTh3
	0,		// EnableHWThNoiseAdaptive
	2,		// ThEnter32
	3,		// ThLeave32
	2,		// ThEnter22
	2,		// ThLeave22
	1,		// Enable32Persis
	2,		// AFM_32Persis
	1,		// Enable22Persis
	4,		// AFM_22Persis
	12,		// LowSignalThreshold
	144,		// SceneChangeThreshold
	0,		// Rej32ShortSeq
	0,		// Rej32LowMotion
	5,		// WinLeft
	5,		// WinRight
	9,		// WinTop
	5,		// WinBottom
} ;

//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
//static void AssignPanelData(gmd_MasterPanelList PanelIndex);
void gm_SystemInitBlock(void);

#define FLASH_BPX_EN
#ifdef FLASH_BPX_EN
Bool SerialFlashInit(void);
Bool EnableWriteProtection(void);
#endif

void LPM_GPIO_Config(void);
void DUM_ClearLut(void);

//******************************************************************************
//  C O D E
//******************************************************************************

/*******************************************************************************
Name        : LLD_AFMSetParameters
Description : Programs AFM parameters
Parameters  : 
Assumptions :
Limitations :
Returns     : 
*******************************************************************************/
void LLD_AFMSetParameters(void)
{
   // This function must be excuted after 
   // gm_HardwareSystemInit();
   // gm_SystemInitBlock(); // AfpRegisterInit

   gm_WriteRegWord(AFM_THRESH_RS, ((WORD)AFMParams.RTh << 8) | AFMParams.STh);
   gm_WriteRegWord(AFM_22_CTRL1, ((WORD)AFMParams.VMTh2<<6) | AFMParams.VMTh1);
   gm_WriteRegWord(AFM_22_CTRL2, AFMParams.VMTh3);

   // EnableHWThNoiseAdaptive is not used

   // ThEnter32
   // const U8 row1[5] = { 1.1*20, 1.5*20, 2*20, 3*20, 4*20 };
   // const U8 row2[5] = { 1.1*20, 1.8*20, 2.5*20, 5*20, 7*20 };		
   if(AFMParams.ThEnter32 == 0)
   {    
   	gW_AlgVtToAfpThresh1 = 22;
      gW_AlgVtToAfpMidlowThresh = 22;
   }
   else if(AFMParams.ThEnter32 == 1)
   {    
   	gW_AlgVtToAfpThresh1 = 30;
      gW_AlgVtToAfpMidlowThresh = 36;
   }
   else if(AFMParams.ThEnter32 == 2)
   {    
   	gW_AlgVtToAfpThresh1 = 40;
      gW_AlgVtToAfpMidlowThresh = 50;
   }
   else if(AFMParams.ThEnter32 == 3)
   {    
   	gW_AlgVtToAfpThresh1 = 60;
      gW_AlgVtToAfpMidlowThresh = 100;
   }
   else
   {    
   	gW_AlgVtToAfpThresh1 = 80;
      gW_AlgVtToAfpMidlowThresh = 140;
   }   

	// ThLeave32	
   // const U8 row1[5] = { 4*20, 3*20, 2*20, 1.5*20, 1*20 };
   // const U8 row2[5] = { 3*20, 2*20, 1.5*20, 1.2*20, 1*20 };
   // const U8 row3[5] = { 1.2*20, 1.15*20, 1.1*20, 1.05*20, 1*20 };
   // const U8 row4[5] = { 3*20, 2*20, 1.5*20, 1.25*20, 1*20 };      
	if(AFMParams.ThLeave32 == 0)
   {    
		gW_AlgAfpToVtThresh2 = 80;
      gW_AlgAfpToVtThresh3 = 60;
      gW_AlgAfpToVtThresh4 = 24;
      gW_AlgAfpToVtThresh5 = 60;
   }
	else if(AFMParams.ThLeave32 == 1)
   {    
		gW_AlgAfpToVtThresh2 = 60;
      gW_AlgAfpToVtThresh3 = 40;
      gW_AlgAfpToVtThresh4 = 23;
      gW_AlgAfpToVtThresh5 = 40;
   }
   else if(AFMParams.ThLeave32 == 2)
   {
		gW_AlgAfpToVtThresh2 = 40;
      gW_AlgAfpToVtThresh3 = 30;
      gW_AlgAfpToVtThresh4 = 22;
      gW_AlgAfpToVtThresh5 = 30;
   }
	else if(AFMParams.ThLeave32 == 3)
   {    
		gW_AlgAfpToVtThresh2 = 30;
      gW_AlgAfpToVtThresh3 = 24;
      gW_AlgAfpToVtThresh4 = 21;
      gW_AlgAfpToVtThresh5 = 25;
   }
	else
   {    
		gW_AlgAfpToVtThresh2 = 20; 
      gW_AlgAfpToVtThresh3 = 20;
      gW_AlgAfpToVtThresh4 = 20;
      gW_AlgAfpToVtThresh5 = 20;
   }   

	// ThEnter22
	// const U16 row1[5] = { 3, 8, 15, 20, 30 };
	if(AFMParams.ThEnter22 == 0)
		gW_AlgVtToGfxMatches = 3;
	else if(AFMParams.ThEnter22 == 1)
		gW_AlgVtToGfxMatches = 8;
	else if(AFMParams.ThEnter22 == 2)
		gW_AlgVtToGfxMatches = 15;
	else if(AFMParams.ThEnter22 == 3)
		gW_AlgVtToGfxMatches = 20;
	else
		gW_AlgVtToGfxMatches = 30;

	// ThLeave22
   // const U8 row1[5] = { 140, 120, 100, 60, 20 };
   // const U8 row2[5] = { 60, 40, 30, 25, 20 };      
	if(AFMParams.ThLeave22 == 0)
   {    
		gW_AlgGfxToVtThresh1 = 140;
      gW_AlgGfxToVtThresh2 = 60;
   }
	else if(AFMParams.ThLeave22 == 1)
   {    
		gW_AlgGfxToVtThresh1 = 120;
      gW_AlgGfxToVtThresh2 = 40;
   }
	else if(AFMParams.ThLeave22 == 2)
   {    
		gW_AlgGfxToVtThresh1 = 100;
      gW_AlgGfxToVtThresh2 = 30;
   }
	else if(AFMParams.ThLeave22 == 3)
   {    
		gW_AlgGfxToVtThresh1 = 60;
      gW_AlgGfxToVtThresh2 = 25;
   }
	else
   {    
		gW_AlgGfxToVtThresh1 = 20;
      gW_AlgGfxToVtThresh2 = 20;
   }
	
	// Enable32Persis
	// switch BIT0
	// gW_AlgSwitch
	// Bp_out[25] = (BYTE)(gW_AlgSwitch & (~BIT0));
	
	// Enable22Persis
	// switch BIT1
	// gW_AlgSwitch
	// Bp_msg[27] = (BYTE)(gW_AlgSwitch & (~BIT1)) >> 1;		

	// Rej32ShortSeq
	// switch BIT2
	// gW_AlgSwitch
	//Bp_msg[31] = (BYTE)(gW_AlgSwitch & (~BIT2)) >> 2;

	// Rej32LowMotion
	// switch BIT4
	// gW_AlgSwitch
	// Bp_msg[32] = (BYTE)(gW_AlgSwitch & (~BIT4)) >> 4;

	gW_AlgSwitch = (AFMParams.Rej32LowMotion << 4) | (AFMParams.Rej32ShortSeq << 2) | 
	                  (AFMParams.Enable22Persis << 1) | AFMParams.Enable32Persis; 

	// AFM_32Persis
   // const U16 row1[5] = { 21, 22, 25, 30, 60 };
   // const U16 row2[5] = { 21, 22, 25, 26, 30 };
	if(AFMParams.AFM_32Persis == 0)
   {    
		gW_AlgAfpPersistThresh = 21;
      gW_AlgAfpPersistThresh2 = 21;
   }
	else if(AFMParams.AFM_32Persis == 1)
   {    
		gW_AlgAfpPersistThresh = 22;
      gW_AlgAfpPersistThresh2 = 22;
   }
	else if(AFMParams.AFM_32Persis == 2)
   {    
		gW_AlgAfpPersistThresh = 25;
      gW_AlgAfpPersistThresh2 = 25;
   }
	else if(AFMParams.AFM_32Persis == 3)
   {    
		gW_AlgAfpPersistThresh = 30;
      gW_AlgAfpPersistThresh2 = 26;
   }
	else
   {    
		gW_AlgAfpPersistThresh = 60;
      gW_AlgAfpPersistThresh2 = 30;
   }

   #if 0 // The follow wiil overwrite by gm_AfmSetup() in modesetup
	// AFM_22Persis
	// const U16 row1[5] = { 256, 2047, 4296, 32767, 65535 };
	// gDw_AlgGfxMaxLowS
	if(AFMParams.AFM_22Persis == 0)
		gDw_AlgGfxMaxLowS = 256;
	else if(AFMParams.AFM_22Persis == 1)
		gDw_AlgGfxMaxLowS = 2047;
	else if(AFMParams.AFM_22Persis == 2)
		gDw_AlgGfxMaxLowS = 4296;
	else if(AFMParams.AFM_22Persis == 3)
		gDw_AlgGfxMaxLowS = 32767;
	else
		gDw_AlgGfxMaxLowS = 0xFFFF;

	// LowSignalThreshold
	gDw_AlgAfpPhase0MaxS = AFMParams.LowSignalThreshold;

	// SceneChangeThreshold
	gDw_AlgMinSceneChange = AFMParams.SceneChangeThreshold;

   {
      WORD AfmHeight, AfmVStart, AfmWidth, AfmHStart;  

      AfmWidth = (stInput[CH_A].Hactive > gmvw_OutputHActive[CH_A]) ? gmvw_OutputHActive[CH_A]:stInput[CH_A].Hactive;
   
      AfmHeight = (stInput[CH_A].Vactive > gmvw_OutputVActive[CH_A]) ? gmvw_OutputVActive[CH_A] : stInput[CH_A].Vactive;
      
   	// WinLeft
   	// scale 100
   	// Bp_msg[33] = (BYTE)(gm_ReadRegWord(AFM_HSTART) * 100 / gmvw_OutputHActive[CH_A]);
   	gm_WriteRegWord(AFM_HSTART, (WORD)((DWORD)AFMParams.WinLeft*AfmWidth/1000));

   	// WinRight
   	// Bp_msg[34] = (BYTE)((gmvw_OutputHActive[CH_A] + gm_ReadRegByte(AFM_HSTART)) * 100 / gmvw_OutputHActive[CH_A]);
   	// AfmWidth -= AfmHStart + (U16)((U32)AfmWidth * AFMData_p->WinRight / 100);
   	AfmHStart = gm_ReadRegWord(AFM_HSTART);
   	
   	AfmWidth -= AfmHStart + (WORD)((DWORD)AfmWidth * AFMParams.WinRight / 1000);
   	gm_WriteRegWord(AFM_HCOUNT, AfmWidth);

   	// WinTop
   	// AfmVStart = (U16)((U32) AfmHeight * AFMData_p->WinTop / 100);		
   	gm_WriteRegWord(AFM_VSTART, (WORD)((DWORD)AFMParams.WinTop*AfmHeight/1000));

   	// WinBottom
   	//Bp_msg[36] = (BYTE)((gmvw_OutputVActive[CH_A] + gm_ReadRegByte(AFM_VSTART)) * 100 / gmvw_OutputVActive[CH_A]);
   	// AfmHeight -= AfmVStart + (U16)((U32) AfmHeight * AFMData_p->WinBottom / 100);
   	AfmVStart = gm_ReadRegWord(AFM_VSTART);
   	
   	AfmHeight -= AfmVStart + (WORD)((DWORD) AfmHeight * AFMParams.WinBottom / 1000);
   	
   	gm_WriteRegWord(AFM_VCOUNT, AfmHeight);   
   }
   #endif   
}

//******************************************************************************
//
// FUNCTION       : static void AssignPanelData(gmd_MasterPanelList PanelIndex)
//
// USAGE          :
//
// INPUT          :
//
// OUTPUT         :
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
//static void AssignPanelData(gmd_MasterPanelList PanelIndex)
extern gmt_PanelData  CurPanelData;
extern gmt_PanelData far * CurrentPanelData;
void AssignPanelData(gmd_MasterPanelList PanelIndex)
{
   BYTE i;
   gmt_PanelData ROM * DefaultPanelData = NULL_PTR;
   CurrentPanelData = NULL_PTR;

   //Assign the Panel pointer
   for (i=1; PanelArray[i] != NULL_PTR; i++)
   {
      // If the panel is not included in the project seach next element in array
      if (PanelArray[i] == PanelArray)
         continue;

      // For manually modified panel, search for panel ID based on userpreference
      if (PanelIndex)
      {
         if (PanelArray[i]->ID == PanelIndex)
         {
//#if (NON_SCALING_OUTPUT == 1)
            //CurrentPanelData =(gmt_PanelData far *) PanelArray[i];
//#else
            CurrentPanelData = (gmt_PanelData far *)PanelArray[i];
//#endif
            break;
         }
         else if (PanelArray[i]->ID == DEFAULT_PANEL)
            DefaultPanelData = PanelArray[i];
      }
      // Otherwise, use default panel
      else
      {
         BYTE b_PanelID = DEFAULT_PANEL;
      #ifdef ST_4K2K_93xx_BOARD
         if(GetChipID_4K2K()==CHIPID_4K2K_FE)
         {
            b_PanelID = DEFAULT_PANEL_FE;
         }
      #endif                  
         if (PanelArray[i]->ID == b_PanelID )
         {
	         //#if 0//(NON_SCALING_OUTPUT==1)
			#if 0//def OUTPUT_MODE_CHANGE
			if(UserPrefOutputMode != OPM_Normal)
			{
				CurrentPanelData = (gmt_PanelData far *)PanelArray[i];
				_fmemcpy((BYTE far *)&CurPanelData, (BYTE far *)CurrentPanelData, sizeof(gmt_PanelData));
				CurrentPanelData = (gmt_PanelData far *) &CurPanelData;
			}
			else
			#endif
	        // #else
	            CurrentPanelData = (gmt_PanelData far *)PanelArray[i];
				#if(defined(BOARD_93XX_OV))
			  OutputPanelSel = DEFAULT_PANEL;
			  #endif
	          //  #endif
	         break;
         }
      }
   }

   // Output warning if panel not found
   if (CurrentPanelData == NULL_PTR)
   {
      gm_Print ("*******************************", 0);
      gm_Print ("PANEL PARAMETERS NOT LOADED!!!!", 0);
      gm_Print ("*******************************", 0);
      //gmvb_DebugOnly = TRUE;    // Force Debug mode as this is a very serious problem
      if (DefaultPanelData != NULL_PTR)
      {
         gm_Print ("*******************************************", 0);
         gm_Print ("DEFAULT PANEL PARAMETERS ARE LOADED INSTEAD", 0);
         gm_Print ("*******************************************", 0);
//#if (NON_SCALING_OUTPUT == 1)
//         CurrentPanelData =(gmt_PanelData far *) DefaultPanelData;
//#else
         CurrentPanelData = (gmt_PanelData far *)DefaultPanelData;
//#endif

      }
     //else
       //  return;
   }
   //Initialize constants
   gmv_OUTPUT_HTOTAL = PanelTypHTotal; // Typical
   gmv_OUTPUT_VTOTAL = PanelTypVTotal; // Typical
   gmv_OUTPUT_HTOTAL_22 = Panel22HTotal; // HTotal for 2:2 (60Hz to 48Hz conversion)
   gmv_OUTPUT_VTOTAL_22 = Panel22VTotal; // VTotal for 2:2 (60Hz to 48Hz conversion)
   gmv_OUTPUT_HTOTAL_33 = Panel33HTotal; // HTotal for 3:3 (60Hz to 72Hz conversion)
   gmv_OUTPUT_VTOTAL_33 = Panel33VTotal; // VTotal for 3:3 (60Hz to 72Hz conversion)
   gmv_OUTPUT_HACTIVE = PanelWidth;
   gmv_OUTPUT_VACTIVE = PanelHeight;

   gmv_PanelSetupExeptions.DflErrLimit = (PanelID == SAMSUNG_WUXGA_LTM240W1_L01) ? 1 : 0 ;
}

//******************************************************************************
// FUNCTION       : void PanelBacklightInit(void)
// USAGE          : Initialize panel backlight PWM settings
// INPUT          :
// OUTPUT         : None
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  :
// POST_CONDITION :
//******************************************************************************
void PanelBacklightInit(void)
{
   gm_WriteRegWord(BACKLIGHT_PWM_CONFIG_REG, BACKLIGHT_PWM_CONFIG);

   gm_ClearAndSetBitsWord(BACKLIGHT_PWM_CTRL_REG, PWM3_PERIOD, (BYTE)(MAX_PWMPERIOD & 0x0FF));
   gm_ClearAndSetBitsWord(BACKLIGHT_PWM_CTRL_REG + 2, PWM3_PERIOD_MS, (BYTE)((MAX_PWMPERIOD & 0xF00) >> 8));
   
   AdjustPanelBacklight(DEFAULT_BACKLIGHT_INIT);
}

//******************************************************************************
// FUNCTION       : void ApplicationInit(void)
// USAGE          : Initialize all the system variables in order to model the system behaviour
// INPUT          :
// OUTPUT         : None
// GLOBALS        : a lot of
// USED_REGS      : None
// PRE_CONDITION  :
// POST_CONDITION :
//******************************************************************************
extern DWORD gd_MemoryMainStart;
//owen 121018 extern BYTE EnableDDC2BiAddress0x7C;//20120726 Add by KevinChen
BYTE WakeUpFromPowerSaving=0;//20120608 Add by KevinChen for LCD condition check.

void ApplicationInit(void)
{
#ifdef ST_4K2K_93xx_BOARD //Add by ChengLin 20130315
   BYTE CmdBuf[1] = {0}; 
#endif

   // Watchdog timer service: check were latest OCM reset caused by
   // WDT or external Reset. In the case it was called by WDT (severe hangup)
   // external reset will be called, because of WDT reset have reset only OCM
#ifdef WDT_SERVICE_ENABLE
   gm_WDT_Check_Reset((DWORD)WdtResetSystem);
#endif
	#ifdef DELL_U2410F		//Neil120721+ Mute it first anyway when init
	gm_ClearRegBitsWord(AUDIO_MUTE_PORT, AUDIO_MUTE_PIN);
	AUDIO_MUTE();	
	#if ((BOARD_USED == BOARD_93xx_CJ2))
	HEADPHONE_MUTE();
	#endif
	#ifdef USE_TIMER_CTRL_AUD_PWR
	AUDIO_POWER_DISABLE();		//Neil 120804+Power off first
	#endif
	#endif
#ifdef ST_4K2K_93xx_BOARD
   ChipIDInit_4K2K();
#endif


#ifdef ST_4K2K_93xx_BOARD
   #if (DEBUG_DISPLAY_TIME_MEASUREMENT==ENABLE)
      gm_SetRegBitsWord(GPIO_1_ENABLE, GPIO18_EN);
      gm_SetRegBitsWord(GPIO_1_DIRCTRL, GPIO18_IO);
      gm_SetRegBitsWord(GPOUTPUT1, GPIO18_OUT);
   #endif



	if(GetChipID_4K2K()==CHIPID_4K2K_R)
	{
   	// GPIO init
		gm_SetRegBitsWord(GPIO_1_ENABLE, (GPIO25_EN| GPIO29_EN)); // Panel power and backlight
		gm_SetRegBitsWord(GPIO_1_DIRCTRL, (GPIO25_EN| GPIO29_EN));	
		gm_ClearRegBitsWord(GPOUTPUT1, (GPIO25_EN| GPIO29_EN)); // Panel power and backlight
		//gm_SetRegBitsWord(GPIO_5_ENABLE, GPIO82_EN);		
   }
#endif

   //Initialization:
   // - IROM and Driver data,
   // - Serial flash frequency (optioanl see gmc_FLASH_CLK_DIV_N)
   // - Rclk and Oclk frequency
   // - System time service
   gm_HardwareSystemInit();

#ifdef WDT_SERVICE_ENABLE
   // init WDT service with maximum time interval (671 ms for oclk=100MHz)
   // after gm_HardwareSystemInit() because it will reset watchdog ISR
   gm_WDT_Init((DWORD)WdtResetSystem, WDT_TU_8);
   gm_WDT_Reset(WDT_EXP_TIME);
#endif

   UserTimerInit(); //Initialize Application user timer

   //------------------------------------------------------------
#if(FEATURE_GPROBE_UART == REV2)
   {
	UARTInit();
   }
#endif

   if( IS_DUAL_FLASH_ENABLE())
   {
      msg_i("Dual Flash Bank is enabled",0);
      msg_i("Bootup from Flash Bank(%d)",IS_CURRENT_FLASH_BANK_B()? 1:0);
      msg_i("Version Tag (%d)", (*LINEAR_TO_FPTR_B(VERSION_TAG_ADDR)));
   }
   else
   {
      msg_i("Dual Flash Bank is disabled.",0);
   }

   //Common CSM mux routing as default for DDC transactions
   if (!(gm_ReadRegWord(LPM_MISSION_STATUS) & LP_SRAM_LOADED))
      gm_CsmI2CMuxSelect(MUX_DDC2_LpmI2cSlv2_MisDviCp);


#if (FEATURE_SOCIPC == ENABLE && ENABLE_SOC_IPC_KEYPAD_VIA_QUEUE)
#ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()==CHIPID_4K2K_R)
   {
	   /*ChengLin 20120208: New Keypad Key Handle for SocIPC keypad command*/
   	SocIPCKeypadBufferInitial();
   	socipc_kbd_ir_queue_msg("\nSocIPC Keypad Queue Buffer Initialed.\n",0);
	}
#endif
#endif

#if (FEATURE_SOCIPC == ENABLE && ENABLE_SOC_IPC_IR_VIA_QUEUE)
#ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()==CHIPID_4K2K_R)
   {

	   /*ChengLin 20120209: New IR Key Handle for SocIPC IR command*/
	   SocIPCIRBufferInitial();
	   socipc_kbd_ir_queue_msg("\nSocIPC IR Key Queue Buffer Initialed.\n",0);
   }
#endif
#endif

   gm_SystemInitBlock();

   LLD_AFMSetParameters(); // Data from Gearbox

   gd_MemoryMainStart = gmc_MemoryOsdSize; //DDR allocation: OSD goes first in DDR

#if ((FEATURE_DDR_EXECUTION == ENABLE) && ((defined DRAM_CODE_EXECUTION) || (DATA_DRAM_SIZE > 0)))
   // Allocate DRAM memory for code (second block after OSD)
   gd_MemoryMainStart = gm_MemoryCodeAllocate(gd_MemoryMainStart);
#endif // (FEATURE_DDR_EXECUTION == ENABLE)

#if (FEATURE_3D == ENABLE)
   gm_3D_InitializeOnce();
   gm_3D_Initialize(FALSE);
#endif

   //------------------------------------------------------------
   // Channel Allocation
   gm_SetMainChannel(CH_A);
   gm_SetPipChannel(CH_B);

   // 1) internal decompression initialization
   // 2) osd library temp buffer init
   // 3) osd driver initialization
   gm_DecompressInit(sizeof(TempBuffer), (BYTE *)&TempBuffer);
   gmv_TempBuffer = (BYTE *)TempBuffer;
   gmvw_TempBufferSize = sizeof(TempBuffer);

   InitInput();
   AssignPanelData(0); // Initialize default panel data
	InitPanelSpectrum();
                                         
   if(PanelMaxVFreq >= 120)
      gmvb_OutputFrameRate = 120;
   else
      gmvb_OutputFrameRate = (BYTE)gmc_DEF_DISPLAY_RATE;

   if (gmvb_OutputFrameRate < PanelMinVFreq)
      gmvb_OutputFrameRate = PanelMinVFreq;
   if (gmvb_OutputFrameRate > PanelMaxVFreq)
      gmvb_OutputFrameRate = PanelMaxVFreq;

   //SystemInitBlock();VVVVVV
   // Enable Blender Failsafe
   gm_SetRegBitsWord(PB_UNDERFLOW_CTRL, BIT0);

   // The 'Host Shadow Access' feature seems to be broken in Torino and it
   // hasn't been proven to work in Cortez Advanced.
   // Therefore it has to be disabled from now on until it's verified
   // Waitinig for further HW investigation
   gm_ClearRegBitsWord(OCM_MISC_CTRL, BIT0); // Disable Host Shadow Access
 
#if FEATURE_AIP == ENABLE
   //reduce ASE slice level  from default 150mv to avoid vrong sync detection with some DVD sources.
   //gm_AseSetSyncSliceLevel(gmd_ADC_0, ASE_SS_ST, ASE_SLICE_LEVEL_125mV);
#endif
  
	#if GAMMA_IN_FLASH/*** hansPENG FLASH ***/
		LoadFunctionIntoRam();
	#endif

   // Board Specific, I2C and keyboard
   BoardInit();

   // Load UserPref value from NVRAM (MUST be after HW and board Init)
   NvramCheck();                            // Get all the UserPref values

	#ifdef UART_DEBUG_MODE
	#ifdef USE_SECOND_UART
	UserPrefUartSelect = UART1;
	#else
	UserPrefUartSelect = UART0;
	#endif
	#else
	UserPrefUartSelect = OSD_UART_DEF;
	#endif

    UserPrefAudioDownMix = 0;//Barbara Lin;20121210;Audio level became half of the original level when AudioDownMix was on.
    
#ifdef DELL_U2410F//120326 Edward merge
	UserPrefWakeUpByTouchPad = 0;		//Neil 20120525+ 
	
	if(UserPrefPipMode!=UserPrefStoredPipMode)	//Hyb130314* for Pip hotkey behaivor
		UserPrefStoredPipMode = UserPrefPipMode;
	#ifdef CHECK_PxP_STATES_AC_ON
	//120402 Edward add condition for wake up from DPMS do not change the PxP status only AC DC On/Off 
	//if wake up from PIP do not disable PIP channel , otherwise will check main status in modehander to En/Disable PIP
	if(gm_ReadRegWord(MISSION_LPM_DATA_IN_0) != PWR_SW_SIGDET_ON) 
	{	
		//120529 Edward update condition disable PxP if OSD is keep rotate
		if ((UserPrefPipMode != NO_PIP)&&(UserPrefOsdRotation != OSD_ROTATED_90)) //KevinL 20090223
		{			
			B_StoredPipMode = UserPrefPipMode;
			UserPrefPipMode = NO_PIP;	
			B_MainNoSyncCheckState = START_CHECK; //KevinL 20090325 for power on to determine PIP/PBP should be turned on or not.			
		}
		else
		{
			B_StoredPipMode = NO_PIP;
			UserPrefStoredPipMode = NO_PIP;
			/*if(IsDVI() || IsMainVGAInput())
				B_MainNoSyncCheckState = STOP_CHECK; 
			else*/
				B_MainNoSyncCheckState = STOP_CHECK_FOR_ICON; //KevinL 20090325 for power on to determine PIP/PBP should be turned on or not.		
		}
		b_EnablePowerOnPIPModeFlag = FALSE; 
	}
	else
	{	//120529 Edward add for power on monitor disable PxP if auto rotate function enable and monitor 90" 
		#ifdef PIVOT_SENSOR_ENABLE	
		if (Is_OSDAutoRotate_ON()&& Is_SENSOR_ROTATE90()&&(UserPrefPipMode != NO_PIP))
		{
	      		B_StoredPipMode = UserPrefStoredPipMode = UserPrefPipMode = NO_PIP;
			B_MainNoSyncCheckState = STOP_CHECK_FOR_ICON;
		}
		#endif
	}
	#endif
#endif	

//gm_Print("####UserPrefPowerState  %d",UserPrefPowerState);
//gm_Print("####UserPrefPowerOnFlag  %d",UserPrefPowerOnFlag);
//gm_Print("####MISSION_LPM_DATA_IN_0  0x%x",gm_ReadRegWord(MISSION_LPM_DATA_IN_0));

#if defined(BUZZER_USED) && defined(DELL_U2410F)
        gm_ClearRegBitsWord(MFP_MISC_ALT_PINS,MFP_OCM_INT1_EN);	// Raymond[(2011-08-04):02], temporary solution to enable pwm
        //if (UserPrefPowerKeyPressedFlag == TRUE)
        if(gm_ReadRegWord(MISSION_LPM_DATA_IN_0) != PWR_SW_SIGDET_ON)
        {
            if (!UserPrefButtonSoundOff) //KevinL 20081118 StartBuzzer immediately
            {                       
              if(gm_ReadRegWord(LPM_MISSION_STATUS)&LP_SRAM_LOADED)
                //||(!(gm_ReadRegWord(LPM_MISSION_STATUS)&LP_SRAM_LOADED)&&(1==UserPrefPowerState)))//add this condition by lion 20090327 
              {
		  			//#ifdef POWER_LED_PATCH
		  			//#ifdef TouchPadLed
    
                    //KeypadLightControl(KEYPAD_LED_GREEN, LED_NORMAL);
					//#endif
					//#endif

                if (UserPrefPowerState && (gm_ReadRegWord(MISSION_LPM_DATA_IN_0) != PWR_SW_OFF))					
		  			gm_WriteRegWord(BUZZER_PWM_CTRL, (BUZZER_PWM_PULSE_4K7Hz << BUZZER_PWM_PULSE_SHIFT) | BUZZER_PWM_PERIOD_4K7Hz);
		  	  	else
		  			gm_WriteRegWord(BUZZER_PWM_CTRL, (BUZZER_PWM_PULSE_3KHz << BUZZER_PWM_PULSE_SHIFT) | BUZZER_PWM_PERIOD_3KHz); 
		  
                	gm_Delay10ms(15);               
                	gm_WriteRegWord(BUZZER_PWM_CTRL, 0);  					
           	  }
            }
        }
		else
		{
			//gm_Print("UserPrefPowerState-%d",UserPrefPowerState);
			//gm_Print("(gm_ReadRegWord(MISSION_LPM_DATA_IN_0) != PWR_SW_OFF)-%d",(gm_ReadRegWord(MISSION_LPM_DATA_IN_0) != PWR_SW_OFF));
			WakeUpFromPowerSaving=1;//20120608 Add by KevinChen for LCD condition check.
		}
#endif

if(WakeUpFromPowerSaving&&UserPrefLCDCondition_Save)//20120608 Add by KevinChen for LCD condition check.
{
	UserPrefGlobalLCDOnOffFlag=UserPrefLCDCondition_Save;
}

#if FEATURE_AIP == ENABLE
   // Perform Bandgap calibration and save the calibrated value to NVRAM
 #ifdef ADC_BANDGAP_CALIBRATION
   {
      BYTE B_AdcBandGap;
      
      ReadBandgapSetting(&B_AdcBandGap);
      if (B_AdcBandGap > 0x1F)
         AdcBandgapCalibrate();
         
      AdcBandgapInvoke();   
   }
#endif
#endif

   // Initialize function pointer for saving single UserPref into NVRAM
   gFp_SaveUserPref = SaveUserPref;
   // Initialize function pointer for retrieving single UserPref from NVRAM
   gFp_RetrieveUserPref = RetrieveUserPref;

#ifdef MULTIPANEL_DATA_IN_ONE_HEX
   //This code is for supporting multipip panel data in single hex (Appstest 255 panel_id)
   //  The calling of AssignPanelData() above only sets up the default panel since I2C
   //  is not initialized. As a result, UserPrefPanelSelect is always zero at that moment.
   //  Now after NvramCheck() is called, I2C is running and UserPrefPanelSelect carries
   //  the saved panel ID.
   AssignPanelData(UserPrefPanelSelect);

   #if 0 // gm_SetDisplayFreeRun will set it ...
   // DDDS Initialization (Per Panel)
   gm_DdsSetInitFreq(CLK_DDDS, (DWORD)gmv_OUTPUT_HTOTAL *
                     gmv_OUTPUT_VTOTAL *   gmvb_OutputFrameRate);
   #endif

   // EDDS Initialization (108MHz)
   // gm_DdsSetInitFreq(CLK_EDDS, FREQ_108MHz);
#if 0//(FEATURE_DECODER == TWO_DECODER)
   gm_DdsSetInitFreq(CLK_EDDS2, FREQ_108MHz);
#endif

   #if 0 // Move out for all system init
   // Freerun DDS
   gmvb_DDDSSourceChannel = gmvb_MainChannel;
   gm_SetDisplayFreeRun();
	#endif
#endif
#if(PROJECT_SELECT == PROJ_4K2K)
//UserPrefPanelSelect = DEFAULT_PANEL;
UserPrefOutputMode = OPM_Normal;
OutputModeSel = OPM_Normal;
OutputPanelSel = UserPrefPanelSelect;
#endif
#if (defined(BOARD_93XX_4K2K))
	#if((DEFAULT_PANEL == ZDT_4K2K_CMI))
		UserPrefPanelSelect = ZDT_4K2K_CMI;
	#elif(DEFAULT_PANEL == ZDT_4K2K_CMI_39)
		UserPrefPanelSelect = ZDT_4K2K_CMI_39;
	#elif(DEFAULT_PANEL == ZDT_4K2K_CMI_65)
		UserPrefPanelSelect = ZDT_4K2K_CMI_65;	
	#elif(DEFAULT_PANEL == ZDT_4K2K_QFHD)
		UserPrefPanelSelect = ZDT_4K2K_QFHD;
	#elif(DEFAULT_PANEL == ZDT_4K2K_CMI_50)
		UserPrefPanelSelect = ZDT_4K2K_CMI_50;
	#endif
#endif
UserPrefPanelSelect = DEFAULT_PANEL;
UserPrefSelectPanel = UserPrefPanelSelect;
AssignPanelData(UserPrefPanelSelect);
#if 1 // Must init DDDS after panel data ...
   // Freerun DDS
   gmvb_DDDSSourceChannel = gmvb_MainChannel;
   gm_SetDisplayFreeRun();
#endif


   InitMemIF();

   // Since MemoryInit() performs memory allocation based on the actual panel size used
   // in the plafrom, it should be called after the AssignPanelData(UserPrefPanelSelect) is called
   // SCR 2884
   MemoryInit(); // Partation memory for system

   SchedulerInit();

   // Initialize Panel Parameters
   {   // Display Aspect
      //  4:3 = 1.33 = 133
      // 16:9 = 1.77 = 177
      // 15:9 = 1.66 = 166
      //  5:4 = 1.25 = 125
      WORD W_Ratio = (WORD)(((DWORD)PanelWidth * 100) / PanelHeight);
      if ((W_Ratio >= 176) && (W_Ratio <= 179))
         B_OutputAspect = gmd_ASPECT_16x9;
      // change the lower limit from 165 to 160 in order to catch WUXGA panels as well
      else if ((W_Ratio >= 160) && (W_Ratio <= 168))
         B_OutputAspect = gmd_ASPECT_15x9;
      else if ((W_Ratio >= 124) && (W_Ratio <= 126))
         B_OutputAspect = gmd_ASPECT_5x4;
      else
         B_OutputAspect = gmd_ASPECT_4x3;
   }

   // Display Locking Method
   #if (ENABLE_DFL == 1)         //Hyb130622* Add MST TX define
      B_DisplayLockMethod = DISP_LOCK_DFL; // Default selection
   #elif (SELECT_DPTX_TYPE==USE_DPTX_DYNAMIC_SWITCH) ||(SELECT_DPTX_TYPE==USE_MST_DPTX_OUTPUT)|| \
   		(SELECT_DPTX_TYPE==USE_IDP_DPTX_OUTPUT) ||(SELECT_DPTX_TYPE==USE_DPTX_MANUAL_SWITCH) ||\
    defined(RD2_93xx_BOARD) || (DEFAULT_PANEL == ST_1080P_VEGA_1080P) || (DEFAULT_PANEL == ST_WQXGA_VEGA_WQXGA) || \
    (DEFAULT_PANEL == ST_WQHD_VEGA_WQHD)|| (DEFAULT_PANEL == ST_2K2K_VEGA_2K2K) ||(DEFAULT_PANEL == LD_T120001A)
   B_DisplayLockMethod = DISP_FREERUN;
#else
   B_DisplayLockMethod = DISP_LOCK_DFL; // Default selection
#endif



   //-------------------------------------------------------------------------
#if (FEATURE_DISPLAY_PORT == ENABLE)
   #ifndef ST_4K2K_93xx_BOARD
   DpRxLoadEdid2Buf();  //Load EDID here for uploading EDID to LPM EDID virtualization buffer
   #endif
#endif
#ifdef LOW_POWER_MONITOR_SUPPORT
   //cut2: "LPM clock source from Mission TCLK" OR "Invert LPM clock(source from MCLK)" to avoid LPM upload failed!
   //gm_WriteRegWord(LPM_CLOCK_CTRL, LPM_TCLK_SEL);
   gm_SetRegBitsWord(CLOCK_INV1, LPM_CLK_INV);
	#if (LPM_RC_OSC_TRIM_SOURCE == 0)
   if (!(gm_ReadRegWord(LPM_MISSION_STATUS) & LP_SRAM_LOADED))
      LPM_CalibrateRingOSC();
   #endif
   LPM_MuxingRegDisable();
   LPM_StartUp();
   LPM_SharedPBusRequest(BUS_MISSION);
   gm_RegMsgInit();
   if (!IsWarmReset)
   {
      LPM_GPIO_Config();
      LPM_SendCalibratedTrim();
   }
   //Check if PBus owner is switched to Mission
   {
      DWORD t1=gm_GetSystemTime();
      while (gm_ReadRegByte(LPM_MISSION_STATUS) & LPM_PBUS_OWN_STATUS)
      {
         if (gm_GetSystemTime()-t1>500)
         {
            gm_Print("#### Break of waiting ShardPBus Control",0);
            break;
         }
      }
      //gm_Print("#### GetSysTime1=%ld",gm_GetSystemTime()-t1);
   }
   LPM_PwrStatus = LPM_ReceiveStatus();

   // Set LED display
   if (UserPrefPowerState != POWER_STATE_ON)
   {
      KeypadLightControl(KEYPAD_LED_OFF, LED_NORMAL);
   }
   else if (LPM_PwrStatus == PWR_SW_SIGDET_ON)
   {
      KeypadLightControl(KEYPAD_LED_RED, LED_NORMAL);
   }
   else
   {
      KeypadLightControl(KEYPAD_LED_GREEN, LED_NORMAL);
   }

#if KEEP_POWER_ON_FOR_AC_ON
      UserPrefPowerState = POWER_STATE_ON;
	  KeypadLightControl(KEYPAD_LED_GREEN, LED_NORMAL);
#endif
#else // LOW_POWER_MONITOR_SUPPORT
#if (FEATURE_HDMI == ENABLE) || (FEATURE_DISPLAY_PORT == ENABLE)
   // Init OTP shadow RAM
   gm_OtpSramInit();
#endif
#endif // LOW_POWER_MONITOR_SUPPORT

    // Download code into DRAM
   // MUST be after gm_SdramMemorySetup() and MemoryInit()
#if (FEATURE_DDR_EXECUTION == ENABLE)
#if (defined DRAM_CODE_EXECUTION)
   #if (DRAM_CODE_EXECUTION_MODE == 1)
   #if (FEATURE_LOW_POWER_MONITOR == ENABLE)
   if((LPM_PwrStatus != PWR_SW_SIGDET_ON) || (UserPrefCurrentInputMain == ALL_BOARDS_DP1) 
   	||(UserPrefCurrentInputMain == ALL_BOARDS_DP3))		//Neil120807* Add DP3, why only DP1?
   #endif
   {
      gm_MemoryCodeDownload();
      SystemFlags.sys.DramCodeExecution = TRUE;
   }
   #else
   gm_MemoryCodeDownload();
   #endif
   // Setup new Timeout for 2wire becasue effective CPU speed increase
   // Default is 1000 count base on 120MHz with parallel flash
   // Assuming DDR code execution is 4 times faster.
   gm_2WireSetTimeout(4000);
#endif

#if ((defined DRAM_CODE_EXECUTION) || (DATA_DRAM_SIZE > 0))
   #if (DRAM_CODE_EXECUTION_MODE == 1)
   if(SystemFlags.sys.DramCodeExecution != TRUE)
   {
   gm_MemoryCodeAdjust(DATA_DRAM_SIZE);
   }
   #endif
   gm_MemoryCodeStart();
   // 20100422 #1, Add write protection for flash.
   // Enable it for Flash is no problem, but not ROM emulate ... Disable it first for debug time...
#if 0 //def FLASH_BPX_EN
   {
      BYTE tmp_W_ClkDivider, tmp_CACHE_CTRL2;
      tmp_W_ClkDivider=gm_ReadRegByte(SERIAL_FLASH_CTRL2);
      tmp_CACHE_CTRL2=gm_ReadRegByte(CACHE_CTRL2); //CACHE_BURST_SIZE_SERIAL = 0;
      //gm_WriteRegByte(CACHE_UPDATE_EN,(BYTE)CACHE_UPDATE_SERIAL_EN);

      SerialFlashInit();
      EnableWriteProtection();

      //gm_WriteRegByte(SERIAL_FLASH_CTRL2, tmp_W_ClkDivider);  // set clock divider value (2.3 or 4 supported)
      gm_ClearAndSetBitsWord(SERIAL_FLASH_CTRL2,FLASH_CLK_DIV_N,tmp_W_ClkDivider);
      gm_WriteRegByte(CACHE_CTRL2,tmp_CACHE_CTRL2); //CACHE_BURST_SIZE_SERIAL = 0;
      //gm_WriteRegByte(CACHE_UPDATE_EN,(BYTE)CACHE_UPDATE_DRAM_EN);
      gm_WriteRegByte(CACHE_UPDATE_EN,(BYTE)(CACHE_UPDATE_DRAM_EN | CACHE_UPDATE_SERIAL_EN));
   }
#endif
#endif
#endif // (FEATURE_DDR_EXECUTION == ENABLE)

/*ChengLin 20130219: Restore Image Sync State to default value*/
#ifdef ST_4K2K_93xx_BOARD
#if ENABLE_ATHENA_R_L_IMAGE_SYNC_HANDLER
	if(GetChipID_4K2K() != CHIPID_4K2K_FE)
	{
	ImageSyncHandleSetState(IMAGE_LVDS_ON_STATE); //Athena R/L restore to LVDS ON state

	if(GetChipID_4K2K() == CHIPID_4K2K_R)
	{	
      /*ChengLin 20130314: 1. Wait Athena Left Change to LVDS_ON sate, if Timer is time-out, we will to check Athena Left Only.
                                             2. To implement a waiting time for wait Athena Left ready to response inter-chip commands
         */
      gm_TimerStart(IMAGE_SYNC_HANDLER_TIMEOUT_10ms_TMR, 50); //Wait 500ms for check Athena Left 
		while(SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_IMAGE_SYNC_STATE_Status, NULL)!= TRUE)
		{
         if(gm_TimerCheck(IMAGE_SYNC_HANDLER_TIMEOUT_10ms_TMR) == TIMER_TMO)
         {
	         gm_TimerStop(IMAGE_SYNC_HANDLER_TIMEOUT_10ms_TMR);	         
            ImageSyncHandler_msg("\n<TimeOut> Athena Left Initial Image Sync state not equal IMAGE_LVDS_ON_STATE.\n",0);
            break;
         }
			SOCIPC_SetAthena_L_ImageSyncState(IMAGE_LVDS_ON_STATE);
			gm_Delay10ms(1);    
		}

      /*ChengLin 20130314: Confirm Athena Left IMAGE SYNC State*/
      if(SOCIPC_GetAthena_L_ImageSyncState()==IMAGE_LVDS_ON_STATE)
      {         
         ImageSyncHandler_msg("\nSet Athena Left to LVDS_ON_SATE Successful.\n",0);
      }
      ImageSyncHandler_msg("\nInitial Athena Right Image Sync State to IMAGE_LVDS_ON Successful.\n",0);		
	}
   else
   {
	   ImageSyncHandler_msg("\nInitial Athena Left Image Sync State to IMAGE_LVDS_ON Successful.\n",0);
   }
	}
#endif

#endif

   OSDKeyInit(); //Initialize OSD key

   //Frequency of Timer 1 interrupt has been changed from 2ms to 8ms in version 2.4.
   //There is no point to call ADC handler every 2ms, 8ms is more than enough.
   //Every ADC channel (3 is maximum number supported by Workbench) should be called about every 20 ms (8*3=24 ms).
   //Approriate changes (related to ISR call freq) have been done in Timer.c, ADCKeypad.c and IR.c.  SCR#2088
   ApplicationTimer1Init(8); //timer resolution is 8ms

   // Initialize Debug Handler
   DebugHandlerInit();

   // 1) Display Timing
   // 2) Panel Power
   // 3) Enable Display timing for OSD (ODP timing)
   DisplaySetTiming();

	//Neil120707+ setup display size anyway, otherwise no display sometimes, and no OSD
	//gm_Printf(" PB_MAIN_HWIDTH = 0x%x ;PB_MAIN_VHEIGHT = 0x%x", gm_ReadRegWord(PB_MAIN_HWIDTH), gm_ReadRegWord(PB_MAIN_VHEIGHT));
	#ifdef DELL_U2410F
	gm_DisplaySetSize(CH_A);	
	gm_DisplaySetSize(CH_B);		
    	gm_DisplaySetPosition(CH_A);
    	gm_DisplaySetPosition(CH_B);
	#endif
	
   // Initalize ModeSetup
   ModeHandlerInit(); // Mode Handler Requests can be Scheduled from this point on
   ModeSetupInit(); // Mode Setup Requests can be Scheduled from this point on

   // Default OSD SM watchdog timeout
   gm_SetOsdSMWatchdogTimeout(DEFAULT_WATCHDOG_TMO);

   // ************************************************************
   // VERY IMPORTANT
   // ODP need to be initialized at the time we are doing this call
   // ************************************************************
    OsdSdramChkTimesMax = 500;//120615 Edward for OSD garbage , suggestion from AE
    gm_OsdInit(MAX_NUMBER_OF_TILES_USED, TILESET_0_ADDRESS, TILESET_1_ADDRESS);

   //pixel expansion table init
#ifdef OSD_MAX_TEXT_BUFFER_SIZE
#if (OSD_MAX_TEXT_BUFFER_SIZE != 0)
   gmv_OsdTextStringDecompBuffer = (BYTE *)PixelExpansionTable;

   gm_OsdPixelExpansionBufferInit((BYTE near *)PixelExpansionTable,
                                  sizeof(PixelExpansionTable) / 2);
#endif //#if (OSD_MAX_TEXT_BUFFER_SIZE != 0)
#endif //#ifdef OSD_MAX_TEXT_BUFFER_SIZE

   //
   // OSD SM/library initialization
   //
#ifdef OSD_WB_USED
#ifdef RENDER_ENABLED
   gm_OsdSetFontVector(gmv_GlyphTblSet, NUMBER_OF_GLYPH_TABLES);
#endif //#ifdef RENDER_ENABLED

   gm_OsdSmInitThreadsContext();

   gmvb_OsdRotation = UserPrefOsdRotation;
   gmvb_OsdLanguage = UserPrefOsdLanguage;
#endif //#ifdef OSD_WB_USED

   //
   // AVS control (bypass or delay)
   //
#ifdef Action_AdjustAVSDelayOnOff_WB_Used
   //AdjustAVSDelayOnOff();
#endif


#if (FEATURE_DISPLAY_PORT == ENABLE) || (FEATURE_HDMI == ENABLE)
   ComboPhyCableStateInit();
#endif


#if (FEATURE_HDMI == ENABLE) || (FEATURE_DISPLAY_PORT == ENABLE)
   // Enable DP TCLK before loading HDCP key to avoid system hangs!!
   gm_SetRegBitsWord(DP_TOP_CLK_CONFIG, DP_TCLK_EN);  //Set to have clock available to DP
   LoadHdcpKey(); 
#endif

#if (FEATURE_HDMI==ENABLE)
   #ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()==CHIPID_4K2K_FE)
   	{
      HdmiDviInit();
   	}
   else
   	{
		/*ChengLin 20130121: Patch for 4kx2k Platform, once disable HDMI for Athena R/L, Athena FE HDMI HDCP authentication will always failure*/   	
		gm_WriteRegByte(HDRX_HDCP_ADDR,0);
		gm_WriteRegByte(DVI0_HDCP_ADDR,0);
		gm_WriteRegByte(DVI1_HDCP_ADDR,0);
   	}
   #else
   HdmiDviInit();
   #endif
#endif

#if (FEATURE_DISPLAY_PORT == ENABLE)
   DpAppInit();
   #if (FEATURE_HDCP_REPEATER == ENABLE) && (!defined(ST_4K2K_93xx_BOARD))
      CreateRepeaterInstances();
   #endif

	//AdjustDPSwitch_Init();
#endif

   //
   // Adaptive MADI algorithm setup
   //
   gm_AdaptiveGlobalMotion(UserPrefMADI,UserPrefTNR,UserPrefCCS,
                           NOISE_MONITOR_OFF,MOTION_VALUE_OFF,MOTION_REGION_OFF);

#if (FEATURE_CCS == ENABLE)
   if (IsCCSforHDDisabled())
      gm_VideoProcessingAdjustment(VIDEO_CCS, CCS_OFF);
#endif

   //This change is added because the Clamping values were overwritten.
   gmvb_InputConnector[gmvb_MainChannel] = UserPrefCurrentInputMain;
   #ifdef CHANNEL_CLONE
   //Hyb131109+ for DP all init to DP somehow make DP no sync even it has, will looking into it later. temp solution
   if((UserPrefCurrentInputMain == UserPrefCurrentInputPip)&&(UserPrefCurrentInputPip == ALL_BOARDS_DP1))
   		UserPrefCurrentInputPip = WB_PORT_DEF_PXP;
   #endif
   gmvb_InputConnector[gmvb_PipChannel] = UserPrefCurrentInputPip;

#ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()==CHIPID_4K2K_R)
   {
      UserPrefCaptureM = CAPTURE_MODE_RIGHT; // Right
      if(!((UserPrefCurrentInputMain == ALL_BOARDS_DP1) || (UserPrefCurrentInputMain == ALL_BOARDS_DP2) || (UserPrefCurrentInputMain == ALL_BOARDS_DP3)))
         gmvb_InputConnector[gmvb_MainChannel] = UserPrefCurrentInputMain = ALL_BOARDS_DP1; //Default	

      gmvb_InputConnector[gmvb_PipChannel] = UserPrefCurrentInputPip = NOT_USED;

		// Develop state, set to default first
		UserPrefPBPMode = PBP_OFF;
		UserPrefUARTSwitch = UART_TO_ATHENA_R;
      //gm_SetRegBitsWord(DISPLAY_CONTROL, D2PIXWIDE);
   }
   else if(GetChipID_4K2K()==CHIPID_4K2K_L)
   {
      UserPrefCaptureM = CAPTURE_MODE_LEFT; // Left

      if(UserPrefDPSWitch == DP_Input_11)
         gm_SetRegBitsWord(DISPLAY_CONTROL, D2PIXWIDE);
      else
         gm_ClearRegBitsWord(DISPLAY_CONTROL, D2PIXWIDE);

      if(!((UserPrefCurrentInputMain == ALL_BOARDS_DP1) || (UserPrefCurrentInputMain == ALL_BOARDS_DP2)))
         gmvb_InputConnector[gmvb_MainChannel] = UserPrefCurrentInputMain = ALL_BOARDS_DP1;
         
      gmvb_InputConnector[gmvb_PipChannel] = UserPrefCurrentInputPip = ALL_BOARDS_DIP1;
      //gm_SetRegBitsWord(DISPLAY_CONTROL, D2PIXWIDE);
   }
   else if(GetChipID_4K2K()==CHIPID_4K2K_FE)
   {
      UserPrefCaptureM = CAPTURE_MODE_FULL; // FE

      gmvb_InputConnector[gmvb_MainChannel] = UserPrefCurrentInputMain = ALL_BOARDS_HDMI;
      gmvb_InputConnector[gmvb_PipChannel] = UserPrefCurrentInputPip = NOT_USED;
   }
	gm_Print("ST 4Kx2K board, FW date is 20130614 AM", 0);

	// 20130305
	if(GetChipID_4K2K()==CHIPID_4K2K_R)
	{      
		if((UserPrefCurrentInputMain != ALL_BOARDS_DP2) && (IsWarmReset == 0))
		{
			TempMainPort = UserPrefCurrentInputMain;
			check_default_setting_msg("TempMainPort ............... %d", TempMainPort);
			UserPrefCurrentInputMain = ALL_BOARDS_DP2;
			DpRxPortChange(TempMainPort);
		}
	}
#endif
  
   //Hyb140314+ for Asianda HDMI only project
   #ifdef HDMI_ONLY
	   #if ((LOGO_SELECT == LOGO_ASIANDA)&&(IS_PROJ_4K2K())&&(DEFAULT_PANEL == ZDT_4K2K_QFHD))
		if(UserPrefCurrentInputMain != ALL_BOARDS_HDMI)
			UserPrefCurrentInputMain = ALL_BOARDS_HDMI;
	   #endif
   #endif
   AdjustCurrentInputMain();

   // translation needed because NVRAM init has no knowledge of the
   // tuner mapping

#ifdef PIP_PORT_INDEPENDENCE
   // If there is port collision, disable PIP mode detection, otherwise do normal SelectInput()
   if (!IsMainSourceValid(UserPrefCurrentInputMain))
   {
      DisablePipModeDetection();
   }
   else
#endif
   {
      AdjustCurrentInputPip();
   }

#ifdef ST_4K2K_93xx_BOARD
   /*ChengLin 20130322: Check Color Settings before Splash Screen*/
	if((GetChipID_4K2K()==CHIPID_4K2K_R) && (IsWarmReset == 0))
   {  
      /*ChengLin 20130315: Avoid Splash Screen Size follow Aspect Ratio Setting to change*/
      if(UserPrefOutputAspectRatio != OUTPUT_AR_FULL)
      {
         check_default_setting_msg("\n<Info> Current Aspect Ratio Not FULL Mode.\n",0);
         SplashAspectRatio = UserPrefOutputAspectRatio;
         UserPrefOutputAspectRatio = OUTPUT_AR_FULL; //For Athena Right
         CmdBuf[0] = FALSE; //Update Flag          
         SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_DisplaySetup_CONTRL, CmdBuf); //For Athena Left  
      }

      SOCIPC_LoadSplashScreenColorSettings(TRUE);
   }
#endif

   // Setup for system startup splash screen
#ifdef DEMO_SPLASH_SCREEN
   #ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()!=CHIPID_4K2K_FE)
   {
      B_SplashComplete = TRUE;                 // Consider done
   }
   else
   #endif

	#if 0//	//Hyb130103-
            if ((UserPrefOSDSplashScreen == 0) || (UserPrefPowerState == POWER_STATE_OFF))    // No splash screen
        #endif
		
		UserPrefRunBurnInModeFlag = OFF;
		gm_Print("MTO_SplashScreen %d",MTO_SplashScreen);
		#if (CUSTOMER_INFO == ENABLE)	//show LOGO 	//Hyb130103*
		if(UserPrefLOGO_Ctrl == ON)
		{
			if ( (UserPrefRunBurnInModeFlag == FALSE) && (B_SplashComplete==FALSE) )	
				OSDRequestMsg(gmvb_MainChannel, MTO_SplashScreen, REQUEST_NOW);
			else
				B_SplashComplete = TRUE;
		}
		else
			B_SplashComplete = TRUE;
		
				if (UserPrefRunBurnInModeFlag == FALSE)
					UserPrefFactoryFlag = FALSE;
				else	//Neil120723+ if UserPrefRunBurnInModeFlag true, factory mode
					UserPrefFactoryFlag = gmd_TRUE;
					
				//UserPrefOsdHPosition = 500;//1000; //set demo osd to center
				//UserPrefOsdVPosition = 500;//1000;	
		#else
				B_SplashComplete = TRUE;           // Consider done
		#endif	

      if (B_SplashComplete == FALSE)
      {
         WORD W_ChipID = CHIP_ID;

         #if (defined(UserPrefChipIDdigit1) && defined(UserPrefChipIDdigit2) &&\
              defined(UserPrefChipIDdigit3) && defined(UserPrefChipIDdigit4))

            UserPrefChipIDdigit1 = (BYTE)(W_ChipID / 1000);
            UserPrefChipIDdigit2 = (BYTE)((W_ChipID % 1000) / 100);
            UserPrefChipIDdigit3 = (BYTE)((W_ChipID % 100) / 10);
            UserPrefChipIDdigit4 = (BYTE)(W_ChipID % 10);
         #endif
		
		#ifndef DELL_U2410F //120514 Edward disable , already send above 
        OSDRequestMsg(gmvb_MainChannel, MTO_SplashScreen, REQUEST_NOW);
		#endif
        }

#else                                        // No splash screen
   B_SplashComplete = TRUE;                 // Consider done
#endif
#if (OSD_HARD_LOCK == DISABLE)
	//UserPrefOSDLock = OSD_UNLOCK;
	UserPrefOSDHardLock = OSD_UNLOCK;
#endif

   //TODO: TEMPORARY.. get rid of this.  prevents multipip crashing from no initialzation call
#if 0 // 20081015 #2, for PIP wakeup
   UserPrefPipMode = NO_PIP;
#endif

   // Send MTO message to OSD handler for system startup
   OSDRequestMsg(gmvb_MainChannel, MTO_SystemStartUp, REQUEST_NOW);

   //--------------------------------------------------------------------
   // Update Enhancements Block values
   //--------------------------------------------------------------------
   gmvb_AdaptiveTNRMethod = CORTEZ27REL_METHOD;

   // OVP Init
#if (FEATURE_DP_HUB != ENABLE)
   {
      DWORD DW_TestCount = 0;

      while (1)
      {
         LLD_OVPInit();

         if (gm_ReadRegWord(OVP_FRAME_RESET_CTRL))
         {
            //gm_Printf("~~~~ OVP_INI done ~~~~ 0x%x", DW_TestCount);
            break;
         }


         if (DW_TestCount >= 0xFFFF)
         {
            gm_Printf("~~~~ OVP_INI failed ~~~~ 0x%x", DW_TestCount);
            break;
         }

         DW_TestCount++;
      }
   }
#endif

   // Default to use primary coef for in/out window
   // Need to be programmed after OVP Init
   gm_WriteRegWord(COLOR_MATRIX_CTRL, MAIN_MATRIX_WIN_CTRL);
   
#if (FEATURE_ACC3 == ENABLE)
   gm_ACC3Init();
#endif

#if (FEATURE_CROSS_POINT_SWITCH == ENABLE)
 #ifdef UserPrefColorResourceRouting
   gm_ColorResourceRouting(UserPrefColorResourceRouting == CH_A);// 0: MAIN, 1: PIP
 #endif
#endif

   // Setup Video Window if Split screen feature enabled
#if defined(UserPrefSplitScreen)
   if (UserPrefSplitScreen)
      gmvw_AccAcmMode |=   ACC_SPLIT | ACM_SPLIT;
   else
      gmvw_AccAcmMode &= ~(ACC_SPLIT | ACM_SPLIT);
#endif

   // For ACC feature, need to program based on panel height/width
   gm_WriteRegWord(ACC_TOTAL_PIXEL, AccTotalPixel);

#if(!defined(ACC_LUT_METHOD))
#if (FEATURE_ACC3 == ENABLE)
	// Initial ACC and process 1 time to avoid garbage
   gm_WriteRegWord(OVP_PBUS_CTRL, 0x4);   
   //gm_UpdateAcc(LINEAR);
			#ifdef DELL_U2410F//120208 Edward default disable ACC copy from 2410
                // Initial ACC and process 1 time to avoid garbage   
				{
					BYTE tempScheme;
					tempScheme = UserPrefGraphicsImageMode;//enable ACC mode first , so need to store original setting
					UserPrefGraphicsImageMode = Graphics_MOVIE;			
				
					gm_UpdateAcc_DELL(UserPrefGraphicsImageMode);
					gm_ACC3ProcessFrame();
					UserPrefGraphicsImageMode = tempScheme;
					gm_UpdateAcc_DELL(UserPrefGraphicsImageMode);			
				}	
   #else
   if(UserPrefImageScheme != USER)
   {
      gm_UpdateAcc (UserPrefImageScheme);
      gm_ACC3ProcessFrame();
   }
   else
   {
   gm_UpdateAcc (VIVID);
   gm_ACC3ProcessFrame();
   gm_UpdateAcc (UserPrefImageScheme);
   }
	#endif
   gm_WriteRegWord(OVP_PBUS_CTRL, 0);   
#endif
#endif

#if (FEATURE_ACM_3D == ENABLE)
   ACM3D_Init(); // setup default regions and correction params, but do not enable
   if (IsAcmUsed())
   {
        #ifdef DELL_U2410F//120208 Edward ACM depend on OSD setting   //Modified by ST_hansPENG 081007
			if (IsAcmEnablePresetMode())
		#else
            if ( UserPrefImageScheme )
		#endif		
      {
         SETBIT (gmvw_AccAcmMode, ACM_ON);
         gm_VideoProcessingEnable (VIDEO_ACM);
      }
      else
      {
         SETBIT (gmvw_AccAcmMode, ACM_OFF);
         gm_VideoProcessingDisable (VIDEO_ACM);
      }
   }
#endif

#if (FEATURE_VIDEO_FLESHTONE == ENABLE)
   AdjustFleshToneLevel();
#endif

#if 1 // Init ColorWarp. the parameter is read from NVRAM in SelectInput.
   {// ColorWarp
      BYTE i;

      #if (FEATURE_COLORWARP == REV1)
      for(i=0; i<6; i++)
      #else
      for(i=0; i<8; i++)
      #endif
      {
         #if (FEATURE_COLORWARP == REV1)
         AdjustSixAxisColor(i);
         #else // (FEATURE_COLORWARP == REV2)
         AdjustColorWarp(i);
         #endif // (FEATURE_COLORWARP == REV1)
      }
   }
#endif

#if (FEATURE_OVERDRIVE == ENABLE)
   AdjustLCDOverdrive();
   AdjustLCDOverdriveNoiseCoring();
#endif
   // Enhancer
   // ENH is removed.
   //AdjustOldEnhancerEnable();

#if (FEATURE_SHARPNESS == REV6)
   AdjustEnhancerEnable();
#endif

#if (FEATURE_CEC == ENABLE)
   CecInit();
#endif

	AdjustOsdRotation();

   ProgramPanelOutput();
   gm_SetRegBitsWord(LVDS_CLOCK_CONFIG, DCLK_SEL); // Force to ODP_CAPTURE_CLK

   //-------------------------------------------------------------------------
#ifdef LOW_POWER_MONITOR_SUPPORT

   if (UserPrefPowerState == POWER_STATE_OFF)
   {
      PowerRequestOff();
   }
   else if (LPM_PwrStatus == PWR_SW_SIGDET_ON)
   {
      PowerTransition();
		// 20130204, FE is in this state during wake up state, output LVDS for R side
		#ifdef ST_4K2K_93xx_BOARD
      /*ChengLin 20130402: Add for Fix DP can't wake up from LPM Sleep mode*/
      if(GetChipID_4K2K()==CHIPID_4K2K_R) // Out put sync for L side synchronaztion
      {
         gm_SetRegBitsWord(DISPLAY_CONTROL, DTG_RUN_EN | DCNTL_EN | DDATA_EN);
			PanelPowerOn4k2k();
      }
		else if(GetChipID_4K2K()==CHIPID_4K2K_FE)
		{
			gm_DisplayOnOff(On);// to minimaze screen flashing  
		}
		#endif
   }
   else
   {
      //if(IsCUT2_1() == FALSE) // Enable Pad CTRL for GPIO change LVDS issue.
      {
         // SW workaround for LVDS QUAL unable control C and D channel
         gm_ClearRegBitsDWord(LVDS_BUS_AB_PAD_CTRL, BUS_A_CH0_ENABLE|BUS_A_CH1_ENABLE|BUS_A_CH2_ENABLE|BUS_A_CH3_ENABLE|BUS_A_CH4_ENABLE|BUS_A_CH5_ENABLE|BUS_A_CLK_ENABLE|
                                                    BUS_B_CH0_ENABLE|BUS_B_CH1_ENABLE|BUS_B_CH2_ENABLE|BUS_B_CH3_ENABLE|BUS_B_CH4_ENABLE|BUS_B_CH5_ENABLE|BUS_B_CLK_ENABLE );
      
         gm_ClearRegBitsDWord(LVDS_BUS_CD_PAD_CTRL, BUS_C_CH0_ENABLE|BUS_C_CH1_ENABLE|BUS_C_CH2_ENABLE|BUS_C_CH3_ENABLE|BUS_C_CH4_ENABLE|BUS_C_CH5_ENABLE|BUS_C_CLK_ENABLE|
                              BUS_D_CH0_ENABLE|BUS_D_CH1_ENABLE|BUS_D_CH2_ENABLE|BUS_D_CH3_ENABLE|BUS_D_CH4_ENABLE|BUS_D_CH5_ENABLE|BUS_D_CLK_ENABLE );
      }

		#ifdef ST_4K2K_93xx_BOARD
			if(GetChipID_4K2K()==CHIPID_4K2K_R) // Out put sync for L side synchronaztion
			{
				gm_SetRegBitsWord(DISPLAY_CONTROL, DTG_RUN_EN | DCNTL_EN | DDATA_EN);
				PanelPowerOn4k2k();
			}

			else if(GetChipID_4K2K()==CHIPID_4K2K_FE)
			{
				gm_DisplayOnOff(On);// to minimaze screen flashing  
			}
		#else
			//gm_DisplayOnOff(On);// to minimaze screen flashing  	//Hyb131101- turn on backlight later
		#endif
   }
#else // LOW_POWER_MONITOR_SUPPORT
      UserPrefPowerState = POWER_STATE_ON; 

      //if(IsCUT2_1() == FALSE) // Enable Pad CTRL for GPIO change LVDS issue.
      {
         // SW workaround for LVDS QUAL unable control C and D channel
         gm_ClearRegBitsDWord(LVDS_BUS_AB_PAD_CTRL, BUS_A_CH0_ENABLE|BUS_A_CH1_ENABLE|BUS_A_CH2_ENABLE|BUS_A_CH3_ENABLE|BUS_A_CH4_ENABLE|BUS_A_CH5_ENABLE|BUS_A_CLK_ENABLE|
                                                    BUS_B_CH0_ENABLE|BUS_B_CH1_ENABLE|BUS_B_CH2_ENABLE|BUS_B_CH3_ENABLE|BUS_B_CH4_ENABLE|BUS_B_CH5_ENABLE|BUS_B_CLK_ENABLE );      
         gm_ClearRegBitsDWord(LVDS_BUS_CD_PAD_CTRL, BUS_C_CH0_ENABLE|BUS_C_CH1_ENABLE|BUS_C_CH2_ENABLE|BUS_C_CH3_ENABLE|BUS_C_CH4_ENABLE|BUS_C_CH5_ENABLE|BUS_C_CLK_ENABLE|
                              BUS_D_CH0_ENABLE|BUS_D_CH1_ENABLE|BUS_D_CH2_ENABLE|BUS_D_CH3_ENABLE|BUS_D_CH4_ENABLE|BUS_D_CH5_ENABLE|BUS_D_CLK_ENABLE );
      }
      gm_DisplayOnOff(On);// to minimaze screen flashing
#endif // LOW_POWER_MONITOR_SUPPORT


#if ((DPRX_PANEL == ENABLE)||(SELECT_DPTX_TYPE==USE_IDP_DPTX_OUTPUT))
		  SetupDPPanel();	  //Hyb130508+ for DP panel 
#endif

   //-------------------------------------------------------------------------

#ifdef USE_SMT
   /* initialize smt transactions */
   SMT_TransactionInit();
#endif

   // Disable all Madi and Spatial modes since inputs are not stable yet
   gm_VideoProcessingDisable(VIDEO_SPATIAL | VIDEO_MADI);

#if (FEATURE_CEC == ENABLE)
   CecAppPowerHandlerRequest(ReqPwrOn);
#endif
   
	BacklightInit();

	if(B_SplashComplete == TRUE) //Chunbao DC off/on black screen brightness change 
	{
		AdjustBrightness();
	}
#ifdef DELL_U2410F
		gm_TimerStop(DiagnosticTestPattern_1s_TMR);
		gm_TimerStart(FactoryModeAdmit_1s_TMR, 5); //KevinL 20090120 for Power on < 5S admit entering factory mode
		gm_TimerStart(SystemPowerOn_TMR, 35); //KevinL 20090212 for Power on 3.5S to display Pip Message,	
		gmvb_OsdReadyForPowerDown = FALSE; 
		//CableDetectionInit();		//Neil 20120526- not used for Athena//KevinL 20090105
		
		//UserPrefPowerKeyPressedFlag = FALSE;
		//UserPrefDCRTestOnOffFlag = Off;
		//gm_TimerStart(DEEP_SLEEP_100ms_TMR, DEEPSLEEP_TIME);	  //StKam090117  avoid DC-OnOff show nosync Message dialog before sync detect OK   
		StorePresetModes(); //KevinL 20090213	

		
		//120326 Edward merge
		#ifdef CHECK_PxP_STATES_AC_ON
		if(B_MainNoSyncCheckState == START_CHECK) //KevinL 20090325 for power on to determine PIP/PBP should be turned on or not.	
		{
	       if(B_SplashComplete == FALSE)
	       {
	       		if(UserPrefCurrentInputMain == ALL_BOARDS_HDMI)
					gm_TimerStart(MainNoSyncCheck_100ms_TMR, HDMI_NO_SYNC_CHECK_TIMEOUT+SPLASH_SCREEN_TIMEOUT);	 		   	
				else   	
					gm_TimerStart(MainNoSyncCheck_100ms_TMR, NO_SYNC_CHECK_TIMEOUT+SPLASH_SCREEN_TIMEOUT);	
			
			//if(IsDVI() || IsMainVGAInput()) //KevinL 20090401
				//UserPrefHideIconFlag = 1;			
	       }
		  else
		  {
	       	if((UserPrefCurrentInputMain == ALL_BOARDS_HDMI)||(UserPrefCurrentInputMain == ALL_BOARDS_DVI1))	//yvonne 20120922 solve DC off/on PIP disappear issue.
				gm_TimerStart(MainNoSyncCheck_100ms_TMR, HDMI_NO_SYNC_CHECK_TIMEOUT);	 		   	
			else if(GetPortConnType(UserPrefCurrentInputMain)==CT_DISPLAY_PORT)
				gm_TimerStart(MainNoSyncCheck_100ms_TMR, DP_NO_SYNC_CHECK_TIMEOUT);
			else	
				gm_TimerStart(MainNoSyncCheck_100ms_TMR, NO_SYNC_CHECK_TIMEOUT);	 		
		  }
			//DW_Time1 = gm_GetSystemTime();
		}
		#endif
#endif

#if (FEATURE_VWD == ENABLE)
   VWD_Initial();
#endif

#if (FEATURE_ACR == ENABLE)
   ACR_Init();
#endif

#if (FEATURE_AUDIO == ENABLE)
   UserPrefAudioMute = OFF;		//140729
   Audio_Initialize();
#endif

   RequestUpdateCableStatus();

#if (FEATURE_DUM == ENABLE)
   DUM_ClearLut();
#endif

#ifdef USE_SMT
   // sharpness init ...
   VQ_Sharpness_Para_TEMP.NumOfSteps = 0;
#endif

   #ifdef SAVING_DATA_INTO_FLASH
   #if (DRAM_CODE_EXECUTION_MODE == 1)
   if(SystemFlags.sys.DramCodeExecution == TRUE)
   {
      SerialFlashInit();      
      {
         DUM_FlashCheck();
      }   
   }
   #else
   SerialFlashInit();      
   {
      DUM_FlashCheck();
   }   
   #endif  
   #endif  

   
#ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()==CHIPID_4K2K_L)
   {
   	gm_WriteRegWord(OSD_H_ACTIVE,0);
	   gm_WriteRegWord(OSD_V_ACTIVE,0);
   }
   else if(GetChipID_4K2K()==CHIPID_4K2K_R)
   {
/*ChengLin 20130124: Athena R INT1/ Athena R/L INT2 handler enable*/   
#if (ENABLE_SOC_IPC_LEFT_RIGHT_SLAVE_INT2_FROM_FE_MASTER)
	#if(ENABLE_ATHENA_RIGHT_INT1_HANDLE)
	gm_WriteRegWord(EXT_IRQ_OCMMASK, OCM_INT1_MASK|OCM_INT2_MASK);
	#else
	gm_WriteRegWord(EXT_IRQ_OCMMASK, OCM_INT2_MASK);
	#endif
#else
	#if(ENABLE_ATHENA_RIGHT_INT1_HANDLE)
	gm_WriteRegWord(EXT_IRQ_OCMMASK, OCM_INT1_MASK);
	#endif
#endif
   }
#endif

   
   #ifdef SAVING_DATA_INTO_NVRAM
	{
	  if(UserPrefDUMSwitch != UNIFORMITY_OFF)
		 DUM_NVRAMCheck();
	}	
   #endif

   //120530 Edward save the Brightness / contrast / Sharpness by scheme mode 
	if(IsPresetMode_GRAPHICS_TEXT)
	{
		UserPrefBrightness = UserPrefBrightness_Text;
		UserPrefContrast = UserPrefContrast_Text;
		UserPrefPortSharpness = UserPrefPortSharpness_Text;		
	}	
	else if(IsColorSpace_GRAPHICS_CAL1)//120806 Edward add for CAL1/CAL2 save seperate
	{
		UserPrefBrightness = UserPrefBrightness_CAL1;
		UserPrefContrast = UserPrefContrast_CAL1;
		UserPrefPortSharpness = UserPrefPortSharpness_All;
	}
	else if(IsColorSpace_GRAPHICS_CAL2)
	{
		UserPrefBrightness = UserPrefBrightness_CAL2;
		UserPrefContrast = UserPrefContrast_CAL2;
		UserPrefPortSharpness = UserPrefPortSharpness_All;
	}
	else
	{
		UserPrefBrightness = UserPrefBrightness_All;
		UserPrefContrast = UserPrefContrast_All;
		UserPrefPortSharpness = UserPrefPortSharpness_All;		
	}	
	//120608 Edward save the Hue / Saturation 
	if(IsPresetMode_GRAPHICS_MOVIE)
	{
			UserPrefHue = UserPrefHue_Movie;
			UserPrefSaturation = UserPrefSaturation_Movie;
	}
	else if(IsPresetMode_GRAPHICS_GAME)
	{
			UserPrefHue = UserPrefHue_Game;
			UserPrefSaturation = UserPrefSaturation_Game;
	}
	else
	{
			UserPrefHue = 0;
			UserPrefSaturation = WB_SATURATION_DEFAULT;
	}	
	
	AdjustHStart();

#if 0 // (defined(ST_4K2K_93xx_BOARD)
#ifdef Athena_4K2K_Chip_A
	UserPrefCaptureM = 0; // Main
	if( (UserPrefCurrentInputMain != ALL_BOARDS_HDMI) && (UserPrefCurrentInputMain != ALL_BOARDS_DP1) && (UserPrefCurrentInputMain != ALL_BOARDS_DP2))
		UserPrefCurrentInputMain = ALL_BOARDS_HDMI; //Default	
#endif
#ifdef Athena_4K2K_Chip_B
	UserPrefCaptureM = 2; // Left
	UserPrefCurrentInputMain = ALL_BOARDS_DP2;
	gm_SetRegBitsWord(DISPLAY_CONTROL, D2PIXWIDE);
#endif
#ifdef Athena_4K2K_Chip_C
	UserPrefCaptureM = 1; // Right
	UserPrefCurrentInputMain = ALL_BOARDS_DP2;
	UserPrefCurrentInputPip = ALL_BOARDS_DIP1;
	//gm_SetupInputPip();
	gm_WriteRegWord(OSD_H_ACTIVE,0);
	gm_WriteRegWord(OSD_V_ACTIVE,0);
#endif
#endif

   // DDC channel----------------------------------------------------------------
   // Use Lpm I2C slave for handling GProbe DDC2Bi & DDCCI commands
   // Initialize I2C slave by while loop to avoid stretching automatically when DDC can't be handled
#if 1
   I2CSlaveInitAll();
#else
   {
      static TDdc2bi ddc2biData;
      gm_DDC2Bi_Init(DDC2BI_CHANNEL0, &ddc2biData); //FLI326xxx does not have DDC_PORT bootstrap bit
   }
#endif

#ifdef DDCCi_PROTOCOL_SUPPORT //Add by ChengLin 20130117
   #ifdef ST_4K2K_93xx_BOARD
      if(GetChipID_4K2K()!=CHIPID_4K2K_FE)
   #endif
	if (UserPrefCurrentInputMain == ALL_BOARDS_VGA1)
		SetCapabilitiesString(VGAvcpstr, GetCapabilitiesStringSize());
	else
		SetCapabilitiesString(strCapabilitiesRequest, GetCapabilitiesStringSize());
#endif

#ifdef TouchPadLed //Lvjie 081013
//Neil120813+ for factory mode set all LED on
if(UserPrefFactoryFlag == gmd_TRUE)
	SetOSDMenuState(FIVE_KEYSTATE);
#endif
	B_AudioTimerCtrl |= Aud_Tmr_ACOn;
	AudioUpdateNow = gmd_TRUE;
	SysChFlag[CH_A] |= SysCh_ACOn;
	SysChFlag[CH_B] |= SysCh_ACOn;
//#if(PROJECT_SELECT != PROJ_PD)
if(UserPrefMainMode > Main_SIDE_BY_SIDE)
{	
	UserPrefMainMode = Main_FULL_SCREEN;
	UserPrefMainPosition = TOP_LEFT;
	if(UserPrefMainMode != Main_FULL_SCREEN)
		AdjustMainMode();
}
//#endif

AdjustPxpBlendLevel();
AdjustOsdBlend();

#ifdef VWD_FULL_WINDOW_CHECK //KevinL 20120922 
gm_TimerStop(VWD_FULL_WIDNOW_1S_TMR);
#endif

#if (DEFAULT_PANEL == LG_WHD_LM290WW1_SSA1)
{
	UserPrefLCDOverdrive = ON;
	AdjustLCDOverdrive();
	gm_UpdateOverDrive(OD_OFF, 0,0);  //Michael for 29" 20121011
}
#else
	if(UserPrefLCDOverdrive != OFF)
	{
		UserPrefLCDOverdrive = OFF;
		AdjustLCDOverdrive();
	}
#endif
#if defined(ST_4K2K_93xx_BOARD) && (DEBUG_DISPLAY_TIME_MEASUREMENT==ENABLE)
    gm_ClearRegBitsWord(GPOUTPUT1, GPIO18_OUT);
#endif	

#ifdef EXTERNAL_DECODER
#ifdef TVP5160_DEV_ID
	//Avoid initial failed in any case; 
	//Hyb130417+ may be 3.3V I/O not stable, somehow decoder lost all initial settings,
	//maybe been reset again after last init in ExternalDeviceInit();
	#if DECODER_IN_LPM
	//if((Mission_TVP5160ReadReg(0x33) != 0x41)||(Mission_TVP5160ReadReg(0x34) != 0x11))
		Mission_TVP5160_InitDecoder();
		if(IsMainCVBSInput() ||IsMainCOMP2Input())
			Mission_TVP5160SetVideoInput(CH_A, UserPrefCurrentInputMain);
		if(IsPIPCVBSInput() ||IsPIPCOMP2Input())
			Mission_TVP5160SetVideoInput(CH_B, UserPrefCurrentInputPip);
	#else
	//if(!MIS_TVP5160_CheckOutput())
		MIS_TVP5160_InitDecoder();
		if(IsMainCVBSInput() ||IsMainCOMP2Input())
			MIS_TVP5160_ChangeToPort(CH_A, UserPrefCurrentInputMain);
		if(IsPIPCVBSInput() ||IsPIPCOMP2Input())
			MIS_TVP5160_ChangeToPort(CH_B, UserPrefCurrentInputPip);
	#endif
#endif
#endif
  // dtm_msg("DTM> Init_End_T=%Ld",gm_GetSystemTime()); 

#ifdef MODE_CHANGE_BACKLIGHT_OFF
#if((DEFAULT_PANEL == ZDT_4K2K_CMI)||(DEFAULT_PANEL == ZDT_4K2K_CMI_39)\
||(DEFAULT_PANEL == ZDT_4K2K_CMI_65)||(DEFAULT_PANEL == ZDT_4K2K_CMI_50))
if((gm_ReadRegByte(PANEL_POWER_STATUS) >= 1))//&&(B_PanelInit == gmd_FALSE))
{
	//gm_Delay10ms(20);  //20  wei 140814 inital delay Vcc to Backlight
	ZDT_4K2K_CMI_Panel_Init(UserPrefPanelSelect);
	//B_PanelInit = gmd_TRUE;
}
#endif
#endif
gm_DisplayOnOff(On);// to minimaze screen flashing  
#if ((BOARD_USED == BOARD_93xx_CJ2))
HEADPHONE_UNMUTE();
#endif
AdjustImageRotation();  //wei 140825 for image rotation error when no signal
#if (PROJECT_SELECT == PROJ_4K2K)
	//20090317+
	//UserPrefOutputMode = OPM_PassThrough;
	//StartChangeOutputMode();
	OutputModeAuto = gmd_TRUE;
#endif
UserPrefHideIconFlag = gmd_FALSE;
#if 0//((DEFAULT_PANEL == ZDT_4K2K_CMI_50))//||(DEFAULT_PANEL == ZDT_4K2K_CMI_39)||(DEFAULT_PANEL == ZDT_4K2K_CMI_65))
gm_Delay100ms(5);
ZDT_4K2K_CMI_Panel_Init(UserPrefPanelSelect);
#endif
QFHD_LVDSConfig();
#if(DEFAULT_PANEL == ZDT_4K2K_QFHD)
gm_TimerStart(ZDT_4K2K_Reset168_100ms_TMR, 20);
#endif
SetNoSyncOSDByPanelParameters();

}

#if (FEATURE_LOW_POWER_MONITOR == ENABLE)
//******************************************************************************
//
// FUNCTION       : static void LPM_StartUp(void)
//
// USAGE          : Load LPM hex code and kick start LPM
//
// INPUT          :  None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
static void LPM_StartUp(void)
{
   TDumpDescriptor ROM * HexIndexPtr = FLASHDriverDumps;
   WORD W_HexSize = 0;

ReLoadLPM:   	// 20090315 #1, Improved LPM check, avoid load error issue. (LPM lib also need update).

   // set up inter-processor communication (IPC)
   gm_MessageInit((gmt_QUEUE far *)Queue, (BYTE)NUM_OF_RX_QUEUES, (BYTE far *)B_Buffer,
                  (WORD)sizeof(B_Buffer), (WORD)MESSAGE_SEND_TIMEOUT, (WORD (far*)(void))gm_GetSystemTimeW);

   WRITE_PCB_REG(I0CON, 6|BIT4);//level trigger
   gm_InterruptSetFunc(IRQ0, (FPT_ISR)IrqZeroManager);
   gm_ClearAndSetBitsWord(MISC_OCMMASK_SEL, LPM_MAIL_OUT_IRQ_SEL, IRQ0);
   
#ifdef ST_4K2K_93xx_BOARD   
   if(GetChipID_4K2K()!=CHIPID_4K2K_R)
   {
   #if ( ENABLE_ATHENA_FE_LEFT_INT1_HANDLE )
		#if (ENABLE_SOC_IPC_LEFT_RIGHT_SLAVE_INT2_FROM_FE_MASTER)
         if(GetChipID_4K2K()==CHIPID_4K2K_L || GetChipID_4K2K()==CHIPID_4K2K_FE)
            gm_WriteRegWord(EXT_IRQ_OCMMASK, OCM_INT1_MASK|OCM_INT2_MASK);
         else   
            gm_WriteRegWord(EXT_IRQ_OCMMASK, OCM_INT1_MASK);   
		#else
   	   		gm_WriteRegWord(EXT_IRQ_OCMMASK, OCM_INT1_MASK);
      #endif
	#else
		/*ChengLin 20130123: New 4k2k board not use INT 1 for BYSY check*/	
		#if (ENABLE_SOC_IPC_LEFT_RIGHT_SLAVE_INT2_FROM_FE_MASTER)
         if(GetChipID_4K2K()==CHIPID_4K2K_L ||  GetChipID_4K2K()==CHIPID_4K2K_FE)
            gm_WriteRegWord(EXT_IRQ_OCMMASK, OCM_INT2_MASK);
		#endif
	#endif
	}

#elif defined(FEATURE_HOSTIF)
   //Enable External Interrupts after GPIO Initial (INT_1 for Busy Status Check)
   gm_WriteRegWord(EXT_IRQ_OCMMASK, OCM_INT1_MASK);
#endif
   {
      WORD volatile W_Temp;
      asm cli;
      W_Temp = gm_ReadRegWord(MISSION_MICRO_MAIL_IN);//to clear status bit
      WRITE_PCB_REG(EOI, INT0_VTYPE); // Clear mailbox interrupt
      gm_SetRegBitsWord(MISC_OCMMASK, LPM_MAIL_OUT_MASK);//enable interrupt
      asm sti;
   }
   {
      gmt_STimeEx timer;
      gm_TimerStartEx(&timer, 100, 0); //timeout = 100ms
      while (!(gm_ReadRegWord(LPM_MISSION_STATUS) & LP_POR_DONE))
      {	// 20090315 #1, Improved LPM check, avoid load error issue. (LPM lib also need update).
         if (gm_TimerCheckEx(&timer,0,0) == TIMER_TMO)
         {
            gm_Print("Error LPM: LPM logic reset timeout!");
            break;
         }
      }
   }
   if (!(gm_ReadRegWord(LPM_MISSION_STATUS) & LP_SRAM_LOADED))
   {
      //	gm_Print("~~~~~LPM Delay",0);
      gm_Delay100ms(1);
   }

   if (!(gm_ReadRegWord(LPM_MISSION_STATUS) & LP_SRAM_LOADED))
   {
      //HOST_LPM_SRAM_WE_SEL =1: sync write pulse from mission side to lpm ocm_clk
      //HOST_LPM_SRAM_BUS_SEL=1: switch lpm sram bus ownership to mission side
      gm_SetRegBitsWord(LPM_SPARE1, HOST_LPM_SRAM_WE_SEL|HOST_LPM_SRAM_BUS_SEL);

      while ((HexIndexPtr != 0) && (HexIndexPtr->Ba_Array != 0))
      {
         gm_LoadSRAMBlockEx(gmd_TAB_LPM, gmd_OCM_POLLING_MODE, gmd_SAFE_MODE,
                            (WORD far*)HexIndexPtr->Ba_Array, HexIndexPtr->W_StartingOffset, HexIndexPtr->W_Length);
         W_HexSize += HexIndexPtr->W_Length;
         HexIndexPtr++;
      }

      {
#define LPM_CUSTOM_CONFIG_ABSOLUTE_ADDR 0xBE00
         //customize(reload default) IrProtocol in accordance with Application configuration
         gm_LoadSRAMBlockEx(gmd_TAB_LPM, gmd_OCM_POLLING_MODE, gmd_SAFE_MODE,
                            (WORD far*)&IrProtocol, LPM_CUSTOM_CONFIG_ABSOLUTE_ADDR, sizeof(IrProtocol));

         {   //customize configuration
            struct
            {
               WORD PermanentPeripheralControl:1;
            } Configuration =
            {
#ifdef LPM_PERMANENTLY_CONTROLS_PERIPHERAL
               1
#else
               0
#endif
            };
            gm_LoadSRAMBlockEx(gmd_TAB_LPM, gmd_OCM_POLLING_MODE, gmd_SAFE_MODE,
                               (WORD far*)&Configuration,
                               LPM_CUSTOM_CONFIG_ABSOLUTE_ADDR + ((sizeof(gmt_IrProtocolLpm)+1) & ~1),
                               sizeof(Configuration));
         }
      }
      gm_Print("Uploaded LPM: %d bytes", W_HexSize);
#if 0 //FEATURE_DISPLAY_PORT == ENABLE
      DpRxLoadEdid2LpmBuf();   //Load DP EDIDs to LPM EDID virtualization buffers
#endif

      // Init OTP shadow RAM
      gm_OtpSramInit();

      //Switch lpm sram bus ownership to LPM side
      //Disabled sync write pulse from mission side to lpm ocm_clk to avoid affect DDR
      gm_ClearRegBitsWord(LPM_SPARE1, HOST_LPM_SRAM_WE_SEL|HOST_LPM_SRAM_BUS_SEL);

      //signal low power monitor that it may power down mission mode from now on
      gm_WriteRegWord(LPM_MISSION_CONTROL, GO_LOW_POWER);
      gm_WriteRegWord(LPM_MISSION_CONTROL, 0);
      gm_WriteRegWord(LPM_MISSION_CONTROL, GO_LOW_POWER);


      {   //wait for 0xA5A5 value appears on MISSION_MICRO_CONTROL_IN before proceeding
         gmt_STimeEx timer;
            gm_TimerStartEx(&timer, 1000, 0); //timeout = 500ms
         while ((gm_ReadRegWord(MISSION_MICRO_CONTROL_IN) != 0xA5A5))
         {	// 20090315 #1, Improved LPM check, avoid load error issue. (LPM lib also need update).
            if (gm_TimerCheckEx(&timer,0,0) == TIMER_TMO)
            {
               gm_Print("Error LPM: LPM is not started!");
               HexIndexPtr = FLASHDriverDumps;
               W_HexSize = 0;
               goto ReLoadLPM;
               //break;
            }
         }
      }
   }
   else
   {
      gm_Print("LPM restarted from SRAM");
      //Prevent splash screen from appearing when only restarting from SRAM
      B_SplashComplete = TRUE;
#ifdef ST_4K2K_93xx_BOARD      
      AllowKeypadAction = TRUE; //Add by ChengLin 20130401
#endif      
      OSDRequestCancelMsg(gmvb_MainChannel, MTO_SplashScreen);
      UserPrefPowerState = POWER_STATE_ON;
      SaveModeIndependentSettings();
      IsWarmReset = 1;
	  SysChFlag[CH_A] |= SysCh_DCOn;
	  SysChFlag[CH_B] |= SysCh_DCOn;
   }

}

static void LPM_SharedPBusRequest(gmt_BusOwn PBusOwner)
{
   #define PBUS_OWN_REQ_INV   1  // 1: To avoid PBus switch to LPM as executing reset 0
   if (PBusOwner == BUS_MISSION)	 //Request PBus ownership to Mission
   {
      #if PBUS_OWN_REQ_INV
      gm_ClearRegBitsWord(LPM_MISSION_CONTROL, MIS_LPM_PBUS_OWN_REQ);
      #else
      gm_SetRegBitsWord(LPM_MISSION_CONTROL, MIS_LPM_PBUS_OWN_REQ);
      #endif
   }
   else if (PBusOwner == BUS_LPM) //Request PBus ownership to LPM
   {
      #if PBUS_OWN_REQ_INV
      gm_SetRegBitsWord(LPM_MISSION_CONTROL, MIS_LPM_PBUS_OWN_REQ);
      #else
      gm_ClearRegBitsWord(LPM_MISSION_CONTROL, MIS_LPM_PBUS_OWN_REQ);
      #endif
   }
}

static void LPM_MuxingRegDisable(void)
{
   gm_ClearAndSetBitsWord(LPM_C0_AUX_CONTROL, DPRX_AUX_CLK_EN, DPRX_AUX_POWER_DOWN);
   gm_ClearAndSetBitsWord(LPM_C2_AUX_CONTROL, DPRX_AUX_CLK_EN, DPRX_AUX_POWER_DOWN);
   gm_WriteRegWord(LPM_COMBO0_CONTROL, 0);
   gm_WriteRegWord(LPM_COMBO1_CONTROL, 0);
   gm_WriteRegWord(LPM_COMBO2_CONTROL, 0);
   gm_WriteRegWord(LPM_ASYNC_DET_CTRL, 0); 
   gm_WriteRegWord(LPM_AFE_CTRL, 0);
   gm_WriteRegWord(CPHY_TMDSCK_DET_CTRL, 0);
   gm_WriteRegWord(LPM_SPARE1, 0);
   gm_WriteRegWord(LPM_PD_CONTROL, 0);
}
#endif

#if (FEATURE_INTERRUPT_SERVICE == REV2)
//******************************************************************************
//
// FUNCTION       : static void huge interrupt IrqZeroManager(void)
//
// USAGE          : IRQ0 manager
//
// INPUT          :  None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
static void far interrupt IrqZeroManager(void)
{
   asm sti;

#ifdef ST_4K2K_93xx_BOARD
#if ( ENABLE_ATHENA_FE_LEFT_INT1_HANDLE || ENABLE_SOC_IPC_LEFT_RIGHT_SLAVE_INT2_FROM_FE_MASTER || ENABLE_ATHENA_RIGHT_INT1_HANDLE || defined(FEATURE_HOSTIF))

      #ifdef FEATURE_HOSTIF
      if( gm_ReadRegWord(EXT_IRQ_OCM_STATUS) & OCM_INT1 )
      {
         //Raise specific GPIO or Flag if necessary!!
         gm_SetRegBitsWord(EXT_IRQ_OCM_STATUS, OCM_INT1);	//Clear External Interrupt 1
      }

      #else
   	if ( gm_ReadRegWord(EXT_IRQ_OCM_STATUS) & 0x07 )
   	{
		//Process INT1~INT3 External Interrupts
#if (ENABLE_ATHENA_RIGHT_INT1_HANDLE || ENABLE_ATHENA_FE_LEFT_INT1_HANDLE)		
			if( gm_ReadRegWord(EXT_IRQ_OCM_STATUS) & 0x01 )
 			{
	/*ChengLin 20130124: Athena RIGHT Interrupt(INT 1) Handle Route*/   			
	#if (ENABLE_ATHENA_RIGHT_INT1_HANDLE)  

	/*ChengLin 20130221: Athena Right Handle Image Sync Status*/
	#if ENABLE_ATHENA_R_L_IMAGE_SYNC_HANDLER
			if(GetChipID_4K2K()==CHIPID_4K2K_R)
			{
   				ImageSyncHandler_msg("Int1 coming ... %d", 121);
				if(ImageSyncHandleGetState() != IMAGE_LVDS_ON_STATE)
				{
      /*ChengLin 20130425: Enable PBP Deep Sleep Check*/
      #if ENABLE_PBP_ATHENA_L_DEEP_SLEEP_CHECK				
               if(UserPrefCaptureM == CAPTURE_MODE_HALF_HORIZONTAL)
               {
                  if((IsSyncOn() == FALSE) && (IsPBP_AthenaL_SyncValid()))
                  {
                     OSDRequestCancelMsg(gmvb_MainChannel, MTO_NoSignal);
                     AthenaRIgnoreSleep_PBP = TRUE;
                     PBP_AthenaR_ReCheck_AthenaLStatus = TRUE;
                     ImageSyncHandler_msg("\nPBP Mode- Athena R NoSignal and Athena L sync exist!!\n",0);
                  }
                  else
                  {
                     AthenaRIgnoreSleep_PBP = FALSE;
                     PBP_AthenaR_ReCheck_AthenaLStatus = TRUE;
                  }
               }
               else //Full Screen Mode
      #endif        
               {
					   if(IS_Image_Ready() == TRUE)
						   Image_Force_OnOff(TRUE); //Disable Black Screen
					   else
						   Image_Force_OnOff(FALSE); //Enable Black Screen
      #if ENABLE_PBP_ATHENA_L_DEEP_SLEEP_CHECK						   
                  AthenaRIgnoreSleep_PBP = FALSE;
                  PBP_AthenaR_ReCheck_AthenaLStatus = FALSE;      
      #endif
               }
					ImageSyncINT1_Flag = TRUE;
				}
			}
	#else //Original
   			if(GetChipID_4K2K()==CHIPID_4K2K_R)
   			{
   				socipc_recv_msg("Int1 coming ... %d", 121);
   				gm_SetRegBitsWord(PB_CTRL, PB_MAIN_ENABLE);
   				gm_ClearRegBitsWord(PB_CTRL, PB_FORCE_MAIN_BKGND);
   				gm_ForceUpdate(CH_A , INPUT_OUTPUT_CLOCK);
   		   }
	#endif		
	#endif

	/*ChengLin 20130124: Athena Left/FE Interrupt(INT 1) Handle Route*/
	#if (ENABLE_ATHENA_FE_LEFT_INT1_HANDLE)	
   			if(GetChipID_4K2K()==CHIPID_4K2K_L)
            {
				//Reserved for new 4k2k board implementation
 				socipc_recv_msg("\n<SocIPC INT1>  Athena Left INT1 Coming.....\n",0);					
            }
   			else if(GetChipID_4K2K()==CHIPID_4K2K_FE)
   			{
				//Reserved for new 4k2k board implementation
 				socipc_recv_msg("\n<SocIPC INT1>  Athena FE INT1 Coming.....\n",0);						
   			}
	#endif
  				gm_SetRegBitsWord(EXT_IRQ_OCM_STATUS,BIT0);	//Clear External Interrupt 1		
  			}
#endif			

/*ChengLin 20130124: Athena Left/Right Interrupt(INT 2) Handle Route*/
#if ENABLE_SOC_IPC_LEFT_RIGHT_SLAVE_INT2_FROM_FE_MASTER
   			if(GetChipID_4K2K()==CHIPID_4K2K_L || GetChipID_4K2K()==CHIPID_4K2K_R )
   			{
			if( gm_ReadRegWord(EXT_IRQ_OCM_STATUS) & 0x02 )
			{
				//TODO: What behaviors when Athena LEFT get interrupt from Athena FE
				socipc_recv_msg("\n<SocIPC INT2> Athena LEFT/RIGHT Interrupt by Athena FE\n",0);		
				gm_SetRegBitsWord(EXT_IRQ_OCM_STATUS,BIT1);	//Clear External Interrupt 2

				// 20130305
				#ifdef ST_4K2K_93xx_BOARD
				if(GetChipID_4K2K()==CHIPID_4K2K_R)
				{
					BackFlag = 1;
				}
				#endif
			}
   			}
#endif
   	}
      #endif
   	else
#endif		//( ENABLE_ATHENA_FE_LEFT_INT1_HANDLE || ENABLE_SOC_IPC_LEFT_RIGHT_SLAVE_INT2_FROM_FE_MASTER || ENABLE_ATHENA_RIGHT_INT1_HANDLE || defined(FEATURE_HOSTIF))
#endif   	//ST_4K2K_93xx_BOARD
   	{
#if (FEATURE_LOW_POWER_MONITOR == ENABLE)
		 if (gm_ReadRegWord(MISC_OCM_STATUS) & LPM_MAIL_OUT_READY)
		  {
			 gm_MessageInIsr();
		  }
#endif

   	}
   	asm cli;//to avoid interrupt between next command and iret
   	WRITE_PCB_REG(EOI, INT0_VTYPE); // Interrupt exit
}
#endif

//******************************************************************************
//
// FUNCTION       : gmt_RET_STAT gm_SdramMemorySetup(void)
//
// USAGE          : Setup the Memory Controller
//
// INPUT          : None
//
// OUTPUT         : OK    - Success
//                  ERR_TMO    - Failure
//
// GLOBALS        : None
//
// USED_REGS      : MC_DLL_CONTROL            (W)
//                  MC_DLL_LOCK_STATUS        (R/W)
//                  MC_SDRAM                (W)
//                  EXTENDED_MODE_REG        (W)
//                  MC_TINIT_TIME            (W)
//                  MC_TDLL                    (W)
//                  MC_RFSH_CYC                (W)
//                  MC_RFSH_CYC_INT            (W)
//                  MC_RFSH_REQ                (W)
//                  MC_READ_DV_CTRL            (W)
//                  MC_CONFIG                (R/W)
//                  MC_STATUS                (R)
//
// PRE_CONDITION  :    gm_HardwareSystemInit was called to setup system timer.
//
// POST_CONDITION :
//
//******************************************************************************
gmt_RET_STAT gm_SdramMemorySetup(void)
{
   InitMemIF();
   return OK;
}

#if defined(DRAM_CODE_EXECUTION) && (DRAM_CODE_EXECUTION_MODE == 1)
void DramExecute(void)
{
#if defined(DRAM_CODE_EXECUTION) && (DRAM_CODE_EXECUTION_MODE == 1)
   if(SystemFlags.sys.DramCodeExecution != TRUE)
   {
#if (defined DRAM_CODE_EXECUTION)
      gm_MemoryCodeDownload();
#endif
#if ((defined DRAM_CODE_EXECUTION) || (DATA_DRAM_SIZE > 0))
      gm_MemoryCodeAdjust(DATA_DRAM_SIZE+CODE_SIZE);
      gm_MemoryCodeStart();
#endif
      gm_Print("DRAM code execution");
      SystemFlags.sys.DramCodeExecution = TRUE;
   }

   if(SystemFlags.sys.SerialFlashInit != TRUE)
   {
      SerialFlashInit();      
      #ifdef SAVING_DATA_INTO_FLASH
      {
         DUM_FlashCheck();
      }   
      #endif
      SystemFlags.sys.SerialFlashInit = TRUE;
   }
#endif
}
#endif

void gm_GetPanelInfo(gmt_PanelInfo* Ptr_Info)
{
   Ptr_Info->B_PixPerClock = PanelPixPerClk;
   Ptr_Info->W_Width = PanelWidth;
   Ptr_Info->W_Height= PanelHeight;
   Ptr_Info->B_MaxVFreq=PanelMaxVFreq;
   Ptr_Info->B_MinVFreq=PanelMinVFreq;
   Ptr_Info->DW_MaxPClk=PanelMaxPClk;
}

#if (FEATURE_LOW_POWER_MONITOR == ENABLE)
#if (LPM_RC_OSC_TRIM_SOURCE == 0)
//#define DEBUG_RC_OSC_CAL
void LPM_CalibrateRingOSC(void)
{
   #define CLKMEAS_LEN        20
   #define CLKVAL_MIN_LIMIT   0x1FF
   #define CLKVAL_MIN         0x38F
   #define CLKVAL_27M         0x3FF
   #define CLKVAL_MAX         0x43C
   #define CLKVAL_MAX_LIMIT   0x5FF
   #define CLKVAL_DIFF_TOL    25
   #define RCOSC_STABLE_TIME  5  //ms
   #define LPM_RCV_WAIT_TIME  10  //ms, depends on LPM loop time
   
   BYTE i, j, Cnt;
   BYTE Trim, TrimSaved, TrimMin, TrimMax;
   BYTE IsLpmRunning = FALSE;    
   BYTE IsTrimFound = FALSE;
   WORD ClkVal, BestClkVal, TempClkVal, Diff, MinDiff = CLKVAL_MAX_LIMIT;  
   gmt_RcOscClkMeas ClkValTemp[CLKMEAS_LEN];  
   

   NVRam_ReadBuffer(RcOscTrimStart, &TrimSaved, NVRAM_RCOSC_TRIM_SIZE);
   if(TrimSaved > 7) 
      TrimSaved = 3;
   //gm_Print("==== Trim Saved = 0x%x ====", TrimSaved);   

   if (gm_ReadRegWord(LPM_MISSION_STATUS) & LP_SRAM_LOADED)
      IsLpmRunning = TRUE;

   if(IsLpmRunning)  //Re-Calibration
   {
      //LPM_C register can't RW by Mission directly after LPM starts up
      WORD msg[3] = {Set_bit, LPM_CLOCK_CTRL, LPM_OCLK2CORE_PD};//3: clear bits
      gm_MessageSend(gmd_MESSAGE_LPM_GPIO_CTRL, &msg, sizeof(msg));
      gm_Delay1ms(LPM_RCV_WAIT_TIME);
      TrimMax = TrimSaved+1;      
      if(TrimSaved == 0)
         TrimMin = 0;
      else
         TrimMin = TrimSaved-1; 
   }
   else 
   {
      gm_SetRegBitsWord(LPM_CLOCK_CTRL, LPM_OCLK2CORE_PD);
      TrimMax = 7;       
      TrimMin = 0;     
   }
   
   gm_WriteRegWord(P_CLKMEAS_CONTROL, 0x1FFF);//Select LPM_RCOSC_CLK & Width 0x3FF(27MHz)

   for(Trim = TrimMin; Trim <= TrimMax; Trim++)
   {
      BYTE MaxCount = 1;
      BYTE Index  = 0;      

      if(IsLpmRunning)
      {
         //LPM_C register can't RW by Mission directly after LPM starts up
         WORD msg[3];
         msg[0] = Write_Word;
         msg[1] = LPM_RCOSC;
         msg[2] = LPM_RC_OSC_EN|(Trim<<LPM_RC_OSC_TRIM_SHIFT);
         gm_MessageSend(gmd_MESSAGE_LPM_GPIO_CTRL, &msg, sizeof(msg));
         gm_Delay1ms(LPM_RCV_WAIT_TIME);
      }
      else       
      {
         gm_WriteRegByte(LPM_RCOSC, LPM_RC_OSC_EN|(Trim<<LPM_RC_OSC_TRIM_SHIFT));        
      }
      gm_Delay1ms(RCOSC_STABLE_TIME);
         
      #ifdef DEBUG_RC_OSC_CAL
      gm_Print("==== Trim Value = 0x%x ====", Trim);
      #endif

      //Store the Clock Measurement Result to Clock Values Temp Array
      for(i=0, Cnt=0; i<100; i++)
      {
         WORD volatile Val = gm_ReadRegWord(P_CLKMEAS_RESULT);         
         if((Val > CLKVAL_MIN_LIMIT) && (Val < CLKVAL_MAX_LIMIT)) //only record resonable values
         {
            ClkValTemp[Cnt].Value = Val;
            ClkValTemp[Cnt].Count = 1;
            Cnt++;
         }
         if(Cnt > CLKMEAS_LEN)
            break;         
      }
      #ifdef DEBUG_RC_OSC_CAL
      for(i=0; i<CLKMEAS_LEN; i++)
         gm_Print("ClkValRaw[%d].Value = 0x%x", i, ClkValTemp[i].Value);
      #endif
      

      //Sum up the counter of Clock Values, the difference within tolerance will be combined together!
      for(i=0; i<CLKMEAS_LEN; i++)
      {
         for(j=i+1; j<CLKMEAS_LEN; j++)
         {
            if((ClkValTemp[j].Value != 0) && RCOSC_EQUAL(ClkValTemp[i].Value, ClkValTemp[j].Value))
            {   
               ClkValTemp[j].Value = 0;
               ClkValTemp[j].Count = 0;
               ClkValTemp[i].Count++;
            }
         }
      }

      #if 0//def DEBUG_RC_OSC_CAL
      for(i=0; i<3; i++)
      {
         gm_Print("ClkValTemp[%d].Value = 0x%x", i, ClkValTemp[i].Value);
         gm_Print("ClkValTemp[%d].Count = %d", i, ClkValTemp[i].Count);      
      }
      #endif

      //Find out the clock value that with lageset count numbers
      for(i=0; i<CLKMEAS_LEN; i++)
      {
         if(ClkValTemp[i].Count > MaxCount)
         {
            MaxCount = ClkValTemp[i].Count;
            Index = i;
         }
      }
      #if 0//def DEBUG_RC_OSC_CAL
      gm_Print("Clock Value = 0x%x", ClkValTemp[Index].Value);
      #endif
      
      // Find the trim value that makes LPM clock within 27MHz-11% ~ 27MHz+6% 
      // 24.03MHz < LPM clock < 28.62MHz  (27MHz = 0x3FF)
      // 1 step trim value inclrease/decrease 2.x MHz
      if((ClkValTemp[Index].Value >= CLKVAL_MIN) && (ClkValTemp[Index].Value <= CLKVAL_MAX))
      {
         Diff = abs(CLKVAL_27M - ClkValTemp[Index].Value);
         //Update trim value only when new diff value is smaller enough than MinDiff (closer to 27MHz)
         //If both trim value are close to 27MHz, choose the small one!!
         if( (Diff <= MinDiff) && ((MinDiff - Diff) > CLKVAL_DIFF_TOL) )
         {
            MinDiff = Diff;
            BestClkVal = ClkValTemp[Index].Value;
            RcOscTrim = Trim;
            IsTrimFound = TRUE;
         }
      }
      else
      {
         if(Trim == TrimSaved)
            TempClkVal = ClkValTemp[Index].Value;
      }
   }
   
   gm_WriteRegWord(P_CLKMEAS_CONTROL, 0);

   if(IsLpmRunning)
   {
      //LPM_C register can't RW by Mission directly after LPM starts up
      WORD msg[3] = {Clear_bit, LPM_CLOCK_CTRL, LPM_OCLK2CORE_PD};//3: clear bits
      gm_MessageSend(gmd_MESSAGE_LPM_GPIO_CTRL, &msg, sizeof(msg));
      gm_Delay1ms(LPM_RCV_WAIT_TIME);
   }
   else
   {
      gm_ClearRegBitsWord(LPM_CLOCK_CTRL, LPM_OCLK2CORE_PD);
   }

   if(IsTrimFound == FALSE)
   {
      DWORD LpmRcOscClk;   
      RcOscTrim = TrimSaved;  //use saved trim value for RC-OSC
      //RcOscTrim = 2;  // For Cut1.0
      if((TempClkVal > CLKVAL_MIN-102) && (TempClkVal < CLKVAL_MAX+102)) //within 10% inaccurancy
         MeasRcOscVal = TempClkVal;
      else
         MeasRcOscVal = CLKVAL_27M;
      LpmRcOscClk = SYSTEM_TCLK_FREQ/CLKVAL_27M*MeasRcOscVal;      
      gm_Print("!!! Can't find a trim value, use saved trim = %d !!!", RcOscTrim);
      gm_Print("Used RC-OSC Clock Value = 0x%x", MeasRcOscVal);     
      gm_Print("Mapping RC-OSC Clock = %d000Hz", (WORD)(LpmRcOscClk/1000));      
   }
   else
   {
      DWORD LpmRcOscClk;    
      MeasRcOscVal = BestClkVal;
      LpmRcOscClk = SYSTEM_TCLK_FREQ/CLKVAL_27M*MeasRcOscVal;
      gm_Print("Calibrated RC-OSC Trim Value = %d", RcOscTrim);      
      gm_Print("IFM measured RC-OSC Clock Value = 0x%x", MeasRcOscVal);
      gm_Print("Measured RC-OSC Clock = %d000Hz", (WORD)(LpmRcOscClk/1000));
   }

   if(TrimSaved != RcOscTrim)
      NVRam_WriteBuffer(RcOscTrimStart, &RcOscTrim, NVRAM_RCOSC_TRIM_SIZE);
}
#endif

void LPM_SendCalibratedTrim(void)
{
   #define CLKMEAS_27MHz   0x3FF
   WORD CalibratedData[2];
   
#if (LPM_RC_OSC_TRIM_SOURCE == 1) //from OTP
   WORD TrimVal;
   BYTE Tc1, Tc2;

   TrimVal = GetRingOscTrimVal();

   Tc1 = (BYTE)(TrimVal&0xFF);
   Tc2 = (BYTE)(TrimVal>>8);

   if ((Tc1&Tc2) == 0)
   {
      Tc1 &= 0x0F;
      CalibratedData[0] = Tc1;
      CalibratedData[1] = CLKMEAS_27MHz;
      gm_MessageSend(gmd_MESSAGE_CALIBRATION, CalibratedData, sizeof(CalibratedData));
   }
   else
      gm_Print("Trim data check error", 0);
   
#else
   #if (LPM_RC_OSC_TRIM_SOURCE == 2)
   RcOscTrim = 3;  //use default trim value for RC-OSC
   //RcOscTrim = 2;  //use default trim value for RC-OSC
   MeasRcOscVal = CLKMEAS_27MHz;
   #endif
   CalibratedData[0] = RcOscTrim;
   CalibratedData[1] = MeasRcOscVal;      
   gm_MessageSend(gmd_MESSAGE_CALIBRATION, CalibratedData, sizeof(CalibratedData));
#endif
}
#endif

/*********************************  END  **************************************/
