/*
   $Workfile:   System.h  $
   $Revision: 1.167 $
   $Date: 2013/05/23 09:41:34 $
*/
#ifndef __SYSTEM_H__
#define __SYSTEM_H__
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
// MODULE:      System.h
//
// USAGE:       1. Header file for board selection
//              2. Header file for system level definition/selection
//
// NOTE:      For board specific configuration, check in "board.h"
//
//******************************************************************************

#define DISABLE   0
#define ENABLE    1

#include ".\StdLibInc\gm_ChipFeature.h"

//******************************************************************************
//  S Y S T E M     B O A R D    S E L E C T I O N
//******************************************************************************
//***************************************
//  C H I P   S E L E C T I O N
//***************************************
//#define Athena_U
//#define Athena_C

//***************************************
//  B O A R D   S E L E C T I O N
//***************************************

#define BOARD_93xx_CJ2				1		//Hyb130526+ 	base on RD3 design,too
#define DELL_93xx_BOARD 			10


#define BOARD_USED 				BOARD_93xx_CJ2

#define UART_DEBUG_MODE			// Uart will switch to uart 0 at 115200UL baud rate if in debug mode;

#define CHECKSUM_DEFAULT			0x0426

//#define EV1_93xx_BOARD
//#define EV2_93xx_BOARD    
//#define EV3_93xx_BOARD
#if(BOARD_USED == BOARD_93xx_CJ2)
#define RD3_93xx_BOARD			//Difference with RD1: 1, HPD inverse; 2, Detect cable source power; 3, TMDS_EN;

#else
#define RD1_93xx_BOARD			//Base on this board
#endif
//#define ST_4K2K_93xx_BOARD

//***************************************
//  B O A R D   R E V I S I O N   S E L E C T I O N
//***************************************
#if defined(Athena_C) && defined(RD1_93xx_BOARD) // Athena-C special board revision
//#define RD1_93xx_BOARD_REV_B // Athena-C RD1 Rev.B, invert DVI and HDMI HPD signal and add VGA/DVI/HDMI power and cable detection.
#endif

 // Athena RD3 is based on RD1 board, change to use DDR3, invert DVI and HDMI HPD signal and add VGA/DVI/HDMI power and cable detection.
#ifdef RD3_93xx_BOARD
 #define RD1_93xx_BOARD
#endif

 // Athena RD2 is based on RD1 board, just bypass IMP. (Set Bypss PASS_THROUGH).
 #ifdef RD2_93xx_BOARD
  #define RD1_93xx_BOARD
//  #define RD2_4K2K_DEMO
 #endif

 // Athena EV3 is based on EV1 board, change HDMI/DP1 to DDVI
 #ifdef EV3_93xx_BOARD
  #define EV1_93xx_BOARD
 #endif

#ifdef ST_4K2K_93xx_BOARD
	#define FEATURE_SOCIPC		ENABLE
	#define DEBUG_DISPLAY_TIME_MEASUREMENT DISABLE         //For Debug Only
	//  R1211 - MISSION_POWER_ON
   //  R479  - DIN12/GPIO18  ATHENA_FE --> To indicate AppInit_Start, AppInit_End
   //  R449  - DIN12/GPIO18  ATHENA_L  --> To indicate AppInit_Start, AppInit_End
   //  R421  - DIN12/GPIO18  ATHENA_R  --> To indicate AppInit_Start, AppInit_End
#else
	#define FEATURE_SOCIPC		DISABLE
   #define DEBUG_DISPLAY_TIME_MEASUREMENT DISABLE	
#endif

#if DEBUG_DISPLAY_TIME_MEASUREMENT==ENABLE
   #define dtm_msg(x,y)        gm_Print(x,y)
#else
   #define dtm_msg(x,y) 
#endif

//#define DPTX_ONLY

#ifdef RD1_93xx_BOARD
//#define BUZZER_USED					//HYB121209	// Increased by ST_hansPENG 081015	// Raymond[(2011-08-04):02], add buzzer function
#define VWD_ACM_TEST

#define DELL_Docking_PR02X_DVI_Patch	//121010 Louis for DELL docking lose color issue on DVI
#endif//end if RD1_93xx_BOARD

#define DELL_U2410F

#define DPCD600_ASTRO_VG870    0//owen 121030

//#define CHECK_PxP_STATES_AC_ON
#define INPUTSCAN						// Bossen, Enable Auto Input scan at power up.	20081023
#define COLOR_UNIFORMITY_DEMO		1 //110831 Edward for demo
#define GAMMA_IN_FLASH				0
//#define DP_NOSYNC_LED_FADING			//120808 Kordonm add
//#define MUX_TEST_BOARD		//HYB121209-
#if (FEATURE_VWD == ENABLE)
#define VWD_FULL_WINDOW_CHECK 			//KevinL 20120922 Apply full window VWD video enhance after 5 Sec. check.
//#define VWD_INSIDE_WINDOW_APPLY_ONLY	//121113 Edward for VWD color apply do not effect outside window 
#endif
#define POWERNAP_SLEEP 	1	//20121120 for DDCCI command 

//#define PIVOT_SENSOR_ENABLE

#if (BOARD_USED == DELL_93xx_BOARD)
//#define USE_TIMER_CTRL_AUD_PWR		//Neil120804+ follow U2410 audio power control, but still can't ctrl sound bar power when DC on......
#endif

#define SAVING_DATA_INTO_NVRAM			//yvonne 20120604 save data to NVRAM.
#define USE_XRite						//yvonne 20120604 For XRite ddcci command to get gamma table.
#ifdef USE_XRite
#define Qubyx_Uniformity
#endif
#define Contrast_0_Black 				//yvonne 20120614
#define VALUE_MATCH_WITH_OSD			//yvonne 20120907 for DDCCI

// Louis 20120221, Patch control for HDMI 
#define HDMI_temp_Patch			1
#define Second_Patch_Time		50
#define COLOR_CALI_FORCE_6500 	1

//***************************************
//  S C A L I N G   T Y P E   S E L E C T I O N
//***************************************
// Select scaling mode for output
// Non scaling output: 
//     0=Output timing is based on panel definition 
//     1=Output timing is same as input
// PassThruMode: Only works when non scaling output is enabled.
//               No memory would be used for both 0/1.
//     0=Use frame buffer;
//     1=No frame buffer used, input would be pass thru to ODP
// Deinterlaced output: Only works for PassThruMode is disabled.
//                      Memory would be used for MADi
//     0=No de-interlaced for input
//     1=Output is de-interlaced by MADi
//       Display VS ISR would be used for DPTx interlaced output

#define NON_SCALING_OUTPUT          	0	//Hyb130505
#define PASS_THRU_MODE			 0
#define DEINTERLACED_OUTPUT 	0



//***************************************
//  D D R   T Y P E   S E L E C T I O N
//***************************************
#if (defined(Athena_C))
#define ENABLE_DDR_X16        ENABLE//DISABLE     // Enable: One DDR , Disable: Two DDR
#else
#define ENABLE_DDR_X16        DISABLE     // Enable: One DDR , Disable: Two DDR
#endif
#if(defined(BOARD_93XX_4K2K))
#define ENABLE_8BIT_MEMORY              // Enable for 8-bit data to memory for bandwidth
#endif


//***************************************
//  F L A S H   S I Z E
//***************************************
// Only extended 24bits mode are supported.
// If enable dual bank flash, please also change Define.Inc flash size.
#ifndef __ADDR24__ //real 20bits mode
 #error Not support real 20bits mode
#else //extended 24bits mode 

 //#define ROM_SIZE_512K
 //#define ROM_SIZE_1MB
 #define ROM_SIZE_2MB
 //#define ROM_SIZE_4MB
 //#define ROM_SIZE_8MB 
#endif

// Select SPI-flash clock as 30, 37, 50, 75Mhz for 150MHz OCM clock
// Please make sure  OCLK_FREQ_ARGS == FSCLK_150M
// ROM emulator may not work for high speed (ex. more than 50Mhz)
//#define FLASH_CLOCK_SELECT 75

//***************************************
//  E X T E R N A L   D E V I C E
//***************************************
#if (defined(BOARD_93XX_SUV))
#define EXTERNAL_DECODER		
#endif

#ifdef ST_4K2K_93xx_BOARD
   #ifndef EXTERNAL_DECODER
		#define EXTERNAL_DECODER
   #endif
#endif


// External Slicer defines
//#define EXTERNAL_SLICER

#ifdef EXTERNAL_DECODER
//#define ANX9021_DEV_ID        0x60
//#define VPC3230_DEV_ID      0x88
//#define SAA7119_DEV_ID      0x40
#define TVP5160_DEV_ID      	0xB8
//#define TVP5158_DEV_ID      	0xB0

//define input & output of DIP external daughter board
#define VPC3230_COMPONENT_IN     0
#define VPC3230_COMPOSITE_IN     1
#define ANX9021_INPORT_0         2
#define ANX9021_INPORT_1         3

#define DEFAULT_DIP_DAUGHTER_BOARD_IN 		VPC3230_COMPOSITE_IN

#define VPC3230_656              0
#define VPC3230_601              1
#define ANX9021_RGB444           2
#define ANX9021_YCbCr444         3
#define ANX9021_YCbCr422         4
#define TVP5160_YCbCr422		5

#ifdef TVP5160_DEV_ID
#define DEFAULT_DIP_DAUGHTER_BOARD_OUT 		TVP5160_YCbCr422
#else
#define DEFAULT_DIP_DAUGHTER_BOARD_OUT 		VPC3230_656
#endif

#ifdef ANX9021_DEV_ID
#define DEC_DEV_ID   ANX9021_DEV_ID
#endif

#ifdef VPC3230_DEV_ID
#define DEC_DEV_ID   VPC3230_DEV_ID
#endif

#ifdef SAA7119_DEV_ID
#define DEC_DEV_ID   SAA7119_DEV_ID
#endif

#ifdef TVP5160_DEV_ID
#define DEC_DEV_ID   TVP5160_DEV_ID
#endif

#endif // EXTERNAL_DECODER

//***************************************
//  O T H E R
//***************************************

//***************************************
// No tuner for MFM design
#define NO_TUNER

//******************************************************************************
// F E A T U R E     S E L E C T I O N
//******************************************************************************

//***************************************
//  D R A M   C O D E   E X E C U T I O N
//***************************************
// Code is being downloaded to DRAM and executed out of it
#define DRAM_CODE_EXECUTION
// Code download sequence mode
// 0: download and start in application init.
// 1: download and start after mode setup for fast see display.
#define DRAM_CODE_EXECUTION_MODE 	1

//***************************************
//  S M A R T   I S P
//***************************************
#define SMART_ISP
//#define SMART_ISP_DUAL_FLASH   
#define SMART_ISP_AUTO_DETECTION


//***************************************
// S M T   P R O T O C O L
//***************************************
#define USE_SMT      // SMT Protocol enable
#define MAX_TRANSACTION_NUMBER         1     // number of opened SMT transactions
#define MAX_SMT_MASTER_TRANSACTIONS    1


//***************************************
// D E M O   W I N D O W   C O N F I G
//***************************************
// Demo Window configuration option - select only one of the below options
//#define DEMO_WINDOW_SPLIT      VIDEO_WIN_LEFT
//#define DEMO_WINDOW_SPLIT      VIDEO_WIN_RIGHT
#define DEMO_WINDOW_SPLIT        VIDEO_WIN_UP
//#define DEMO_WINDOW_SPLIT      VIDEO_WIN_DOWN


//***************************************
// C O L O R   F E A T U R E
//***************************************
//#define USE_COLOR_FIDELITY
#define FEATURE_COLORWARP REV2 // REV1

//***************************************
// A U T O   S C A N   F E A T U R E
//***************************************
//#define AUTO_SCAN_INPUT
#ifdef AUTO_SCAN_INPUT
#define AUTO_SCAN_INPUT_NVRAM             // First input port in the ring is default port
#define MAIN_AUTOSCAN_CYCLE      300     // 2secs !!! WARNING this is the minimal accepted value do not decrease
#define PIP_AUTOSCAN_CYCLE       300
#endif
//#define CHECK_IS_CABLE_CONNECT


//***************************************
// A U T O   A D J U S T   F E A T U R E
//***************************************
#define AUTO_AUTOADJUST          // Automatically perform an AutoAdjust to graphics Modes after mode setup
#define MAX_AUTO_ADJUST    1     // 0 = NO LIMIT
//#define AUTOADJUST_ABORT_KEY   // Use remote key to abort AutoAdjust


//***************************************
// V G A   F E A T U R E
//***************************************
#define CVT_MODES_SUPPORT
#define ESTIMATED_MODES_SUPPORT
#define DDCCi_PROTOCOL_SUPPORT

#define ADC_BANDGAP_CALIBRATION

#define ALLOW_COMP_CALIB_ON_VGA     // Enable to have another ADC setting for video timing in VGA port

// Maximum number of cvt entries saved in NVRAM.
#ifdef CVT_MODES_SUPPORT
 #define MAX_CVT_MODES_SAVED   5
#endif

//***************************************
// Overlap mode preference between 480P and 640x480@60Hz.
//    640 = select 640x480@60Hz
//    720 = select 480P
#define OVERLAP_480P_VGA60    640

//***************************************
// Preference for ovelapped graphics modes 640x350 vs 720x350, and 640x400 vs 720x400
// Set to 640 or 720 to select the mode with the same width
#define OVERLAP_GR640_GR720   720	//640

//***************************************
// Preference for ovelapped modes 1152iLA, 1152iSH, 1152iLH
// 1188 = select 1152iLA
// 1584 = select 1152iSH
// 2376 = select 1152iLH
#define OVERLAP_1152I         1188

//***************************************
// Maximum number of mode dependent entries saved in NVRAM.
// 5 RL: changed from 5 to 10 after removing Preset modes from nvram
// Reduced back to 5 because on PAL NVRAM usage goes beyond 4k-byte limit
#define MAX_USER_MODES_SAVED        5


//***************************************
// P R O J E C T    O P T I O N S
//***************************************
#define DEMO_SPLASH_SCREEN          // Startup splash screen
#define BLANK_DISPLAY_MUTE_ON       // Display MUTE feature
//#define NO_PIP_SUPPORT
#define USER_EXTENDED_GPROBE_CMD    // Enable AppsTest command
#define USE_LETTERBOX_DETECTION     // Letter Box Detection
//#define SDRAM_BANDWIDTH_1TO1_VF      // For 1:1 Vertical scaling put VFilters before SDRAM
//#define DEBUG_HANDLER                // Debug handler for FW
//#define WB_ENABLE_DEBUG_INFO_STATES     // WB State Debugging
//#define MULTIPANEL_DATA_IN_ONE_HEX      // "Appstest 255 PANEL_ID" to choose a diff panel
//#define SQA_AUTOMATION_DEBUGMODE        // SQA automation testing purposes (SCR 2898) showing both Main and PIP sqa messages
//#define SQA_AUTOMATION_PIP_ONLY         // will only work if SQA_AUTOMATION_DEBUGMODE is defined - SQA automation testing purposes (SCR 2898) if defined, only pip messages gets displayed otherwise BOTH MAIN AND PIP messages are displayed
//#define SQA_AUTOMATION_MAIN_ONLY        // will only work if SQA_AUTOMATION_DEBUGMODE is defined - SQA automation testing purposes (SCR 2898) if defined, only MAIN messages gets displayed otherwise BOTH MAIN AND PIP messages are displayed
//#define OSD_PORT_CHANGE_DIALOG          // OSD port changing dialog

#define ENABLE_DFL 0 // 1:enable DFL, 0: free run

//#define CHANNEL_CLONE
#ifdef CHANNEL_CLONE
 #define PIP_PORT_INDEPENDENCE
#endif


//***************************************
// TCLK/TCLK_DIVISOR is used in low power mode (value range from 1 to 255; 0 means TCLK)
#define TCLK_DIVISOR    2

//***************************************
// default OSD watchdog timeout
#define DEFAULT_WATCHDOG_TMO     10    //10x100ms = 1sec timeout


//***************************************
// Low Power Monitor support
#if (FEATURE_LOW_POWER_MONITOR == ENABLE)
#define LOW_POWER_MONITOR_SUPPORT
 #ifdef LOW_POWER_MONITOR_SUPPORT
  //use if IR and CEC are ALWAYS connected to the low power monitor and low power
  //cpu needs to forward messages to mission cpu
  #define LPM_PERMANENTLY_CONTROLS_PERIPHERAL
 #endif
#endif

// To support LPM UART debug - UART_DEBUG_SUPPORT in Lpm_System.h should be defined too.
//#define LPM_UART_SUPPORT  //I2C SLV4 will be disabled due to pins sharing (LPM_GPIO1,2)

// To support LPM power key force power down - refer LPM_FORCE_POWER_DOWN define in Lpm_System.h
#define LPM_FORCE_POWER_DOWN 1


//******************************************************************************
#if 1//120102 enable for DELL OSD 
#ifdef DELL_U2410F
#ifdef VGA_EXT_ADC_CALIBRATION
#define VGA_ADC_CALIBRATION_METHOD 	PIXEL_GRAB
#else
#define VGA_ADC_CALIBRATION_METHOD 	MIN_MAX	// INTERNAL_DAC_VGA	//Torisa 
#endif

#ifdef COMP_EXT_ADC_CALIBRATION
#define COMP_ADC_CALIBRATION_METHOD 	EIGHT_COLOR_BAR_100
#else
#define COMP_ADC_CALIBRATION_METHOD 	INTERNAL_DAC_COMP
#endif
#endif

#endif
//***************************************
// Deep Sleep defines
#define DEEPSLEEP_TIME     200   //100ms-unit: defines the necessary time in Sleep mode with no sync to DeepSleep transition
//#define DEEP_SLEEP_MAIN        //Sync monitoring is done only for Main
#define DEEP_SLEEP_MAIN_AND_PIP  // Sync monitoring is done for both Main and Pip			// 20081015 #3, for PIP wake up from deep sleep. (main no sync).
//#define DEEP_SLEEP_DISABLED    // Power management is disabled
#define DEEP_SLEEP_CONDITION // If no signal condition with Host Power and Cable on Main/Pip Input, wait for at least 5min before DEEP SLEEP Switch.
#define DEEP_SLEEP_CONDITION_TIME  300	//Org = 600 //600*100ms = 1 min //3000 * 100ms = 5 min

//***************************************
// Normal mode power saving defines
#define INACTIVE_DEVICE_POWER_SAVING

//***************************************
// U S E R    P R E F E R E N C E    O P T I O N S
//#define MAIN_BACKGROUND_COLOR BLACK		//Move to CustoerAPP
#define PIP_BORDER_ENABLED             // Switch to enable border
#define PIP_BORDER_COLOR         LIGHTBLUE
#define PIP_WINDOW_HORIZONTAL_OFFSET   0  // change by redy 140828  //20
#define PIP_WINDOW_VERTICAL_OFFSET     0  // change by redy 140828 //20
#define PIP_BORDER_WIDTH         3
#define PIP_BORDER_HEIGHT        3
#define HIGHLIGHT_BORDER_ENABLED       // Switch to enable border
#define HIGHLIGHT_BORDER_COLOR   LIGHTBLUE

#define SCHEME_DEPENDENT         1
#define PORT_DEPENDENT           2
#define PORT_SCHEME_DEPENDENT    SCHEME_DEPENDENT

#if (PORT_SCHEME_DEPENDENT == SCHEME_DEPENDENT)
 #define NUM_OF_IMAGE_SCHEMES_ENTRIES   NUM_IMAGE_SCHEMES
#else
 #define NUM_OF_IMAGE_SCHEMES_ENTRIES   10 //NUM_OF_CONNECTORS
#endif

//***************************************
//  S E C O N D   U A R T   S E L E C T I O N
// Enabling the Second UART define, the application ensures the following services:
//   - ISR, (similar to the UART0) is placed in the interrupt vector table
//   - the second UART is initialized for rx/tx on the desired baud rate
//   - serial rx is gBa_CommBuf1 256 bytes long
//   - tx - call gm_Uart1TrxBuf (txdata_pointer, message_length)
//   - rx - messasge ready to be parsed when gm_Uart1RcvBuf (rxdata_pointer) returns
//            non zero (data ready for parsing in the pointed buffer)
//       - messsage ready when no serial activity for 100 ms after the last character
//            is received
//   - parser framwork included in /fw/comm/gProbe.c in case of a slave implementation
//   - master implementation is not available yet
//#define USE_SECOND_UART                  // Enable the MCU second UART


//***************************************
//#define WDT_SERVICE_ENABLE     // System MCU-186 watchdog

#ifdef WDT_SERVICE_ENABLE
#define WDT_INIT        WDT_TU_8    // 1 WDT unit = 671ms @ OCM 100MHz, 447ms @ OCM 150MHz
#define WDT_EXP_TIME    23          // 23 * 671ms ~ 15s, 23 * 447ms ~ 10s
#endif


//***************************************
// For 100/120 Hz panel
//#define PANEL_120_OR_100_HZ
#ifdef PANEL_120_OR_100_HZ
// The enhanced methods for reducing motion blur using 120/100 Hz panel are mutually exclusive.
// Select only ONE of them for the project.

//#define BLACK_FRAME_INSERTION_METHOD // BFI using forcing display color
//#define GREY_FRAME_INSERTION_METHOD // GFI using forcing display color
#endif


//***************************************
// support of backlight PWM control for adjust brightness
// that feature is supported for special panel (LG_WUXGA_LM240WU4_SLB1)
//#define BRIGHTNESS_PWM

//***************************************
// support of disable UART print message (for main loop)
//#define DISABLE_PRINT_MESSAGE


//******************************************************************************
//  F / W     P A T C H
//******************************************************************************
#define PATCH_OF_DVI_AFB_DIABLE     0        //Fixed in Cut2. Patch for AFB always be triggered if input from DVI front-end.

#define LPM_RC_OSC_TRIM_SOURCE      0       // 0: IFM Calibration  1: OTP  2: Use default value(might be difference between different silicon)

#define PATCH_OF_RD2_LVDS_LOCKUP    0

//#define SINGLE_FIELD_DEINT_FIXED

//******************************************************************************
//  T E S T
//******************************************************************************
// Test definition for bring up
#define CHECK_ITEM      0     // 1: Enable check items
#define IGNORE_SLEEP    			0    // 1: Ignore sleep function, 0: enable sleep function
#if (defined(BOARD_93XX_SUV)||defined(BOARD_93XX_4K2K))
#define KEEP_POWER_ON_FOR_AC_ON      1 // 1: do not store last power status for loss power, keep power status as on for initialisation
#else
#define KEEP_POWER_ON_FOR_AC_ON      0 // 1: do not store last power status for loss power, keep power status as on for initialisation
#endif

#define DDR_TEST_CODE       ENABLE
#define DDR_Timing_DBG      ENABLE

#define FSB_OSD_VERIFICATION      // Appstest for SDRAM testing

// To get data before send short pulse.
//#define ENABLE_RECORD_DATA


//******************************************************************************
//  O T H E R
//******************************************************************************
//#define COMPONENT_VBI_THROUGH_OAK_SLICER

//#define ENABLE_DCR    // DCR function by using "IMP_BRIGHTNESS". Reserved for future.


// External flash wait state
//#define SET_WAIT_STATE(num) (0x6060 | ((num-1)<<8) | (num-1))
//#define FLASH_WAIT_STATE SET_WAIT_STATE(11)   // 11 wait state (for 90ns flash)

//#define VIDEO_CAPTURE       // Video capture function (not implemented)

//******************************************************************************
//  C U S T O M     U S E
//******************************************************************************


//******************************************************************************
//  C O M P I L E R     U S E     O N L Y
//******************************************************************************
#if (FEATURE_DP_HUB == ENABLE)
#if 1 // Use Athena_C, RD2_93xx_BOARD
  #ifdef Athena_U
  #undef Athena_U
  #endif
  #ifndef Athena_C 
  #define Athena_C
  #endif
  #ifndef RD2_93xx_BOARD
  #undef EV1_93xx_BOARD
  #undef EV2_93xx_BOARD
  #undef EV3_93xx_BOARD
  #undef RD3_93xx_BOARD
  #define RD1_93xx_BOARD
  #define RD2_93xx_BOARD
  #endif
#endif
   #undef NON_SCALING_OUTPUT
   #undef PASS_THRU_MODE
   #undef CHECK_ITEM      1
   #undef IGNORE_SLEEP    1
   #define NON_SCALING_OUTPUT         1
   #define PASS_THRU_MODE            1
   #define CHECK_ITEM      1
   #define IGNORE_SLEEP    1

   #ifdef INACTIVE_DEVICE_POWER_SAVING
   #undef INACTIVE_DEVICE_POWER_SAVING
   #endif
#endif

#ifdef Athena_C
 #undef ENABLE_DDR_X16
 #define ENABLE_DDR_X16    ENABLE   // Athena-C only supports one DDR

 #undef EXTERNAL_DECODER      // Athena-C not support DIP interface (external device)
#endif

#if (FEATURE_DUM == ENABLE)
 //#define SAVING_DATA_INTO_FLASH
#endif

#if (FEATURE_DDR_EXECUTION == DISABLE) 
#undef DRAM_CODE_EXECUTION
#undef SMART_ISP
   
#ifdef SAVING_DATA_INTO_FLASH
#undef SAVING_DATA_INTO_FLASH
#endif
   
#undef USE_SMT
   
#define DISABLE_OSD_HANDLER
#undef DEMO_SPLASH_SCREEN
#elif !defined(DRAM_CODE_EXECUTION)
#undef SMART_ISP
      
#ifdef SAVING_DATA_INTO_FLASH
#undef SAVING_DATA_INTO_FLASH
#endif
#endif

//#ifdef FEATURE_HOSTIF
//#undef DEMO_SPLASH_SCREEN
//#endif

//***************************************
// SPI flash control driver.
#if defined(FLASH_CLOCK_SELECT)
#if FLASH_CLOCK_SELECT == 75
#define SPI_FLASH_CLK_DIVIDE 2
#elif FLASH_CLOCK_SELECT == 50
#define SPI_FLASH_CLK_DIVIDE 3
#elif FLASH_CLOCK_SELECT == 37 // 37.5
#define SPI_FLASH_CLK_DIVIDE 4
#elif FLASH_CLOCK_SELECT == 30
#define SPI_FLASH_CLK_DIVIDE 5
#else
#pragma message "Incorrect SPI-flash clock, use default setting."
#endif
#endif
#if defined(SMART_ISP) || defined(SAVING_DATA_INTO_FLASH)
 #define SPIFLASH_CTRL_DRV
#endif




//***************************************
// Include board specific setting
#include "board\Board.h"

//***************************************
// Include different RAM memory mapping (need to be include after Board.h and DRAM_CODE_EXECUTION)
#include   "system\MemMap.h"


#endif   // __SYSTEM_H__

//*********************************  END  **************************************
