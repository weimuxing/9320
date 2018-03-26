/*
   $Workfile:   gm_StdLib.h  $
   $Revision: 1.57 $
   $Date: 2012/09/26 07:39:27 $
*/
#ifndef __GM_STDLIB_H__
#define __GM_STDLIB_H__
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
// MODULE:        gm_StdLib.h
//
// USAGE:      Header file for all Standard Library functions to be
//             accessed from the Application.
//
//******************************************************************************

#if 1//def __ALL_H__							// Application
	#include "StdLibInc\gm_Global.h"		// Need to be in the beginning
	#include "StdLibInc\gm_ChipFeature.h"	// Need to be after gm_global.h
	#include "StdLibInc\gm_HostReg.h"
	#include "StdLibInc\gm_DrvLib.h"
	#include "StdLibInc\gm_InputConfigDefs.h"
   #include "StdLibInc\gm_System_Stdp93xx.h"
	//#include "StdLibApps\gm_InputConfig.h"
#else
	#include "gm_Global.h"              	// Need to be in the beginning
	#include "gm_ChipFeature.h"				// Need to be after gm_global.h
	#include "gm_HostReg.h"
	#include "gm_Mcu186.h"
	#include "gm_Register.h"
	#include "gm_DrvLib.h"
	#include "gm_InputConfigDefs.h"
	#include "gm_InputConfig.h"
   #include "gm_System.h"
#endif

//******************************************************************************
//  D E F I N E S
//******************************************************************************

#define NUM_CHANNELS       2           // number of channel in chip

#define  Q16                  (0x10000)      // 2^16
#define  Q24                  (0x1000000L)   // 2^24

#define gmd_DISP_MODE_MASK       (gmd_EXPANSION|gmd_V_SHRINK|gmd_H_SHRINK|gmd_VSYNC_DIV2)
#define gmd_FORCE_FREERUN     (gmd_AUTO_FREERUN|gmd_SRC_FREERUN)
#define gmd_FREERUN_MASK      (gmd_AUTO_FREERUN|gmd_SRC_FREERUN)

#define gmd_SYNC_POL_MASK     (gmd_NEG_HSYNC|gmd_NEG_VSYNC)

//#define YUV_GAIN_ADJ_IN_YUV_DOMAIN      // Allows for YUV gain adjustment

#define  I2C_MST_CLK_SCALE_20kHz  0x1D4
#define  I2C_MST_CLK_SCALE_50kHz  0x18e
#define  I2C_MST_CLK_SCALE_100kHz 0xEA
#define  I2C_MST_CLK_SCALE_400kHz 0x35
#define  I2C_MST_CLK_SCALE_1MHz   0x0F

#define I2C_MST_CLK_HIGH_FREQ I2C_MST_CLK_SCALE_1MHz

#define AFM_WINDOW_HEIGHT_REF 			188
#define AFM_WINDOW_WIDTH_REF			668
#define GLOBAL_MOTION_THRESHOLD_LOW	3		// 0~65536.  Default = 3
#define GLOBAL_MOTION_THRESHOLD_HIGH	4800	// 0~65536.  Default = 4300

//
//  size of TNR and sharpness tables
//
#if (FEATURE_TNR == REV1)
#define TNR_REGS			27
#else // (FEATURE_TNR == REV3)
#define TNR_REGS			12
#endif
#define TNR_OSD_STEPS		4

#define SHARPNOISE_REGS			5
#define SHARPNOISE_NOISE_STEPS	12
#define SHARPNOISE_OSD_STEPS		3

// Define a macro for unused variables - to prevent compiler warnings 
#define UNUSED_VAR(x)	((x) = (x))

//******************************************************************************
//  E N U M S
//******************************************************************************

//
// Additional (to IROM -defined) common error number for function return status
//
enum
{
   OSD_INVALID_TILESET = ERR_OSD_INVALID_TILESET,  // Invalid tileset
   OSD_INVALID_TILE = ERR_OSD_INVALID_TILE,  // Invalid tile number
   OSD_DISABLE_TILE = ERR_OSD_DISABLED_TILE, // tile is disabled, can't draw the control
   OSD_INVALID_SIZE = ERR_OSD_INVALID_SIZE,  // Invalid control size
   OSD_INVALID_BPP = ERR_OSD_INVALID_BPP, // Invalid color depth
   OSD_INVALID_FONT_ID = ERR_OSD_INVALID_FONT_ID,  // Invalid font id
   OSD_TOOBIG_STRING = ERR_OSD_TOOBIG_STRING,      // Temporary buffer size is too small for rendering
   ABORTED = LAST_IROM_DEFINED_RET_STAT+1,  // execution was aborted
   FAILED,                       // something failed
   TMR_ACTIVE,                   // Timer is active
   TMR_TIMED_OUT,                // Timer is timed out
   TMR_STOPPED,                  // Timer is stopped
   SYNC_HONVON,                  // Hsync on, Vsync on
   SYNC_HOFFVON,                 // Hsync off, Vsync on
   SYNC_HONVOFF,                 // etc
   SYNC_HOFFVOFF,                // etc
   AA_LOST_FEATURES,             // auto adjust lost features
   ERR_CHANNEL,                  // Channel is Invalid
   ERR_INCORRECT_PIN,            // Incorrect Pin - AIP
   ERR_DECODER_STAT,             // Decoder Status Error
   FEATURE_NO_AVAIL,           // Select feature not available
   ERR_DECODER_NOISY_INPUT,      // Input to Decoder is Noisy
   ERR_DEC_MODE_CHANGE           // Decoder has a Mode Change
};

//
// FAIL / PASS enumeration
//
typedef enum
{
   gmd_FAIL = 0,
   gmd_PASS = 1,
} gmt_RET_PASSFAIL;

//
// TIMER status enumeration
//
typedef enum
{
   gmd_TIMER_ACTIVE,             // Timer is active
   gmd_TIMER_TIMED_OUT,          // Timer is timed out
   gmd_TIMER_STOPPED,               // Timer is stopped
} gmt_TIMER_STAT;

//
// VALID / OUT_OF_RANGE enumeration
//
typedef enum
{
   gmd_VALID,
   gmd_OUT_OF_RANGE
} gmt_RET_VALID;

//
// OSD status enumeration
//
typedef enum
{
   gmd_OSD_OK,                   // OK
   gmd_OSD_INVALID_TILESET,         // Invalid tileset
   gmd_OSD_INVALID_TILE,            // Invalid tile number
   gmd_OSD_DISABLE_TILE,            // Tile is disabled, cannot draw control
   gmd_OSD_INVALID_SIZE,            // Invalid control size
   gmd_OSD_INVALID_BPP,          // Invalid color depth
   gmd_OSD_INVALID_FONT_ID,         // Invalid font id
   gmd_OSD_TOOBIG_STRING,           // Temp buffer size too small to render
} gmt_OSD_STAT;

//
// Physical Channels
//
typedef enum
{
   CH_A = 0,                     // Physical Channel A
   CH_B,                      // Physical Channel B
   CH_PHY_NUM                    // Number of Physical Channels
} gmt_PHY_CH;

typedef enum
{
	MIFM = 0,					// Main IFM (associated with CH_A)
	PIFM,						// Pip IFM  (associated with CH_B)
#if	(FEATURE_IFMC == ENABLE)
	IFMC,						// Floating IFM
#endif
	IFM_NUM,						// Number Of IFMs
} gmt_PHY_IFM;

//
// Physical ADC set
//
typedef enum
{
   ADC_SET_0 = 0,                   // Physical ADC set 0
   ADC_SET_1,                       // Physical ADC set 1
   NUM_ADC_SET                      // Number of Physical ADCs
} gmt_PHY_ADC;


//
// Host register update selection (IMP,IPP or ODP)
//
typedef enum
{
   INPUT_CLOCK,                  // Update with input clock
   OUTPUT_CLOCK,                       // Update with output/display clock
   INPUT_OUTPUT_CLOCK,               // Update both input/output clock
   GMT_HOST_UPDATE_SIZE
} gmt_HOST_UPDATE;

//
// Device ID connected to channel
//
typedef enum
{ // Note : external device don't have odd address
    DEV_NONE = 0,       // NO DEVICE
    DEV_DECODER = 1,    // internal decoder
    DEV_HDMIDVI = 2,
    DEV_IFM  = 3,       // internal or external device use IFM
    DEV_EXTERN  = 4,    // external device which use external status checking  (external knowldege to the library)
    DEV_DP = 5          // display port    
} gmt_DEVICE_ID;

//
// Detection mode setup
//
typedef enum
{
    // detection mode
    DET_STANDARD        ,           // Enable mode detection
    DET_NONSTANDARD        ,           // Enable mode detection
    DET_KEEP_MODE       ,
    DET_MODE_MASK
} gmt_DET_MODE;


typedef enum
{
    // state of detection
    DET_ENABLE          ,           // Enable mode detection, START DETECTION
    DET_DISABLE            ,           // Disable mode detection, STOP DETECTION
    DET_RESTART         ,           // Init and start mode detection (port change)
    DET_FORCED_STABLE,
} gmt_DET_STATE;

typedef enum
{
   AUTO_MUTE_DISABLE,
   AUTO_MUTE_ENABLE,
} gmt_AutoMode;

//
// Video Processing
//
typedef DWORD gmt_VIDEO_PROCESSING;

#define  VIDEO_MADI        		0x00000001L  	// Motion Adaptive DeInterlacing
#define  VIDEO_DCDI_MAIN   		0x00000002L     // DCDi for Main channel
#define  VIDEO_FILMMODE    		0x00000004L     // Film Mode
#if (FEATURE_CCS == ENABLE)
#define  VIDEO_CCS         		0x00000008L     // Cross Color Supression
#endif
#define  VIDEO_TNR         		0x00000010L     // Temporal Noise Reduction
#define  VIDEO_MNR         		0x00000020L     // MPEG Noise Reduction
#define  VIDEO_GNR         		0x00000040L     // Gaussian Noise Reduction
#define  VIDEO_ACC         		0x00000080L     // Auto Color Control
#define  VIDEO_ACM         		0x00000100L     // ACM
#define  VIDEO_HEM         		0x00000200L     // HEM
#define  VIDEO_3_3         		0x00000400L     // FilmMode 3:3 display (72Hz)
#define  VIDEO_2_2         		0x00000800L     // FilmMode 2:2 display (48Hz)
#define  VIDEO_33_AND_22   		0x00000C00L     // Both Video 3:3 and Video 2:2 Mask
#define  VIDEO_SPATIAL     		0x00001000L     // Single Field Spatial Processing
#define  VIDEO_DYN_SCAL    		0x00002000L     // Dynamic Scaling
#define  VIDEO_VCR    	   		0x00004000L     // VCR trick mode
#define  VIDEO_TNR_PROGRESSIVE	0x00008000L		// Temporal Noise Reduction for Progressive Inputs
#define  VIDEO_FLESHTONE   		0x00010000L  	// Flesh tone correction
#define  VIDEO_2STAGE      		0x00020000L  	// 2Stage/2Pass Processing
#define  VIDEO_DCDI_PIP    		0x00040000L  	// DCDi for PIP channel
#define  VIDEO_BLUESTRETCH 		0x00080000L  	// Blue stretch
#define  VIDEO_SHARPNOISE  		0x00100000L  	// Sharpness Noise Coring

#define  VIDEO_PROCESS_ALL 		0x001FFFFFL  	// All feature mask

typedef enum
{
   DISPLAY_LOCK_NORMAL,
   DISPLAY_LOCK_DFL
} gmt_DISPLAY_LOCKING;

//
// ACC / ACM modes
//
typedef enum
{
   ACC_OFF              = BIT0,
   ACC_ON               = BIT1,
   ACC_SPLIT            = BIT2,
   ACM_OFF              = BIT3,
   ACM_ON               = BIT4,
   ACM_SPLIT            = BIT5,
} gmt_ACC_ACM_MODE;

//
// For splitscreen mode
//
#define ACC_ACM_INSIDE_HIGHLIGHT_WIN	1
#define ACC_ACM_OUTSIDE_HIGHLIGHT_WIN	2
#define ACC_ACM_FULLSCREEN				3

#if (FEATURE_OVERDRIVE == ENABLE)
//
// OverDrive Control (OD) modes
//
typedef enum
{
   OD_OFF,
   OD_LOAD_TBL,
   OD_SPLIT,
   OD_FULL_SCREEN,
} gmt_OD_MODE;
#endif

//
// OverDrive Noise Coring modes
//
typedef enum   // These definitions are used in Workbench
{
   OD_NOISE_CORING_OFF,
   OD_NOISE_CORING_LOW,
   OD_NOISE_CORING_MEDIUM,
   OD_NOISE_CORING_HIGH,
   OD_NOISE_CORING_ADAPTIVE,
} gmt_OD_NOISE_CORING_MODE;

//
// Sharpness Control modes
//
typedef enum
{
   SHARPNESS_SAFE_MODE              = BIT0,
   SHARPNESS_FORCE_MODE          = BIT1,
   SHARPNESS_UV_CORRECTION          = BIT2,
   SHARPNESS_ADJ_FROM_OSD           = BIT3,
} gmt_SHARPNESS_MODE;
//
// Image Scheme modes
//
typedef enum
{
   IS_OFF,     // Disables ACC/ACM
   IS_VIVID,
   IS_MOVIE,
   IS_NATURE,
   IS_SPORTS,
   IS_ON,      // Enables ACC/ACM
   IS_MAX = IS_ON, // MAX ImageScheme
} gmt_IMAGE_SCHEME_MODE;
//
// Image Scheme Loading Control modes
//
typedef enum
{
   ACC_SAFE_MODE              = BIT0,
   ACC_FORCE_MODE             = BIT1,
   ACM_SAFE_MODE              = BIT2,
   ACM_FORCE_MODE             = BIT3,
   ACC_ACM_SPLIT              = BIT4, // For Demo purposes (split screen ACC/ACM)
} gmt_IMAGE_SCHEME_CTRL;

//
// Video Window configuration
//
typedef enum
{
   VIDEO_WIN_LEFT,
   VIDEO_WIN_RIGHT,
   VIDEO_WIN_UP,
   VIDEO_WIN_DOWN,
   VIDEO_WIN_DISABLED,
} gmt_VIDEO_WIN_CONFIG;

//
// Input Path Data Width            // FIXME - FK - Remove
//
typedef enum
{
   gmd_BUSWIDTH_16 = 0,          // 16 Bits
   gmd_BUSWIDTH_24,              // 24 Bits
} gmt_DATABUS_WIDTH;

//
// Memory Allocation
//
typedef enum
{
	gmd_MEMORY_ALLOCATE_STATIC,
	gmd_MEMORY_ALLOCATE_DYNAMIC,
} gmt_MEMORY_ALLOCATE;

typedef struct
{
	WORD	W_MainMaxGraphicsWidth;
	WORD	W_MainMaxGraphicsHeight;
	BYTE	B_MainMaxGraphicBpp;
	BYTE	B_MainMaxGraphicNumBuffers;

	WORD	W_MainMaxVideoProgresiveWidth;
	WORD	W_MainMaxVideoProgresiveHeight;
	BYTE	B_MainMaxVideoProgresiveBpp;
	BYTE	B_MainMaxVideoProgresiveNumBuffers;

	WORD	W_MainMaxVideoInterlacedWidth;
	WORD	W_MainMaxVideoInterlacedHeight;
	BYTE	B_MainMaxVideoInterlacedBpp;
	BYTE	B_MainMaxVideoInterlacedNumBuffers;

	WORD	W_PipMaxGraphicsWidth;
	WORD	W_PipMaxGraphicsHeight;
	BYTE	B_PipMaxGraphicBpp;
	BYTE	B_PipMaxGraphicNumBuffers;

	WORD	W_PipMaxVideoWidth;
	WORD	W_PipMaxVideoHeight;
	BYTE	B_PipMaxVideoBpp;
	BYTE	B_PipMaxVideoNumBuffers;
} gmt_MemoryAllocateParams;

typedef enum
{
	BUF_OSD					= BIT0,
	BUF_LCD_OVERDRIVE		= BIT1,
	BUF_AVS					= BIT2,
	BUF_VBI					= BIT3,
	BUF_TELETEXT			= BIT4,
	BUF_DECODER				= BIT5,
	BUF_DECODER2			= BIT6,
	BUF_MOTION_VECTORS		= BIT7,
	BUF_CCS					= BIT8,
	BUF_MAIN				= BIT9,
	BUF_PIP					= BIT10,
	BUF_CODE				= BIT11,
	BUF_TNR					= BIT12,
	BUF_ALL					= (BUF_OSD | BUF_LCD_OVERDRIVE | BUF_AVS | BUF_VBI |
							   BUF_TELETEXT | BUF_DECODER |BUF_DECODER2 |
							   BUF_MOTION_VECTORS | BUF_CCS | BUF_MAIN | BUF_PIP | 
							   BUF_CODE | BUF_TNR)
} gmt_BUFF_TYPE;

typedef enum
{
	BUF_PAL_AUTOINSTALL		= BIT0, // PAL autoinstall buffer
	BUF_TOTAL_CUSTOM			= BIT1 // Total number of custom buffers
} gmt_CUSTOM_BUFF_TYPE;

typedef enum
{
	BUF_START_ADDRESS,
	BUF_END_ADDRESS,
	BUF_SIZE,
	BUF_MAX_SIZE,
} gmt_BUFF_PROPERTY;


//
// Enumaration of FIFO types
//
//	(note: these bit definitions do NOT necessarily map directly to the MC_FIFO_EN register)
//
typedef DWORD gmt_FIFO_TYPE;

#define  FIFO_MAIN_WR			0x00000001L		// Main Write FIFO
#define  FIFO_PIP_WR			0x00000002L		// Pip Write FIFO
#define  FIFO_MV_WR				0x00000004L		// Motion Vector Write FIFO
#define  FIFO_OD_WR				0x00000008L		// Overdrive Write FIFO
#define  FIFO_VBI_WR			0x00000010L		// VBI Write FIFO
#define  FIFO_DEC1_WR			0x00000020L		// Decoder 1 Write FIFO
#define  FIFO_DEC2_WR			0x00000040L		// Decoder 2 Write FIFO
#define  FIFO_OD_RD				0x00000080L		// Overdrive Read FIFO
#define  FIFO_MAIN_RD			0x00000100L		// Main Current Field Read FIFO
#define  FIFO_MAIN_PREV_RD		0x00000200L		// Main Previous Field Read FIFO
#define  FIFO_PIP_RD			0x00000400L		// PIP Read FIFO
#define  FIFO_MADI_PREV_RD		0x00000800L		// MADI previous Read FIFO
#define  FIFO_MADI_PM1_RD		0x00001000L		// MADI previous-1 Read FIFO
#define  FIFO_MV_RD				0x00002000L		// Motion Vector Current Read FIFO
#define  FIFO_MV_PREV_RD		0x00004000L		// Motion Vector Previous Read FIFO
#define  FIFO_DEC1_RD1			0x00008000L		// Decoder 1 Read FIFO 1
#define  FIFO_DEC1_RD2			0x00010000L		// Decoder 1 Read FIFO 2
#define  FIFO_DEC2_RD1			0x00020000L		// Decoder 2 Read FIFO 1
#define  FIFO_DEC2_RD2			0x00040000L		// Decoder 2 Read FIFO 2
#define  FIFO_AVS_WR			0x00080000L		// AVS Write FIFO
#define  FIFO_AVS_RD			0x00100000L		// AVS Read FIFO
#define  FIFO_OSD_RD			0x00200000L		// OSD Read FIFO
#define  FIFO_FSB_WR			0x00400000L		// Frame Store Bridge Write FIFO
#define  FIFO_FSB_RD			0x00800000L		// Frame Store Bridge Read FIFO
#define  FIFO_CCS_WR			0x01000000L		// CCS Write FIFO
#define  FIFO_CCS_RD			0x02000000L		// CCS previous-1 Read FIFO
#define  FIFO_CACHE_WR			0x04000000L		// Cache Write FIFO
#define  FIFO_CACHE_RD			0x08000000L		// Cache Read FIFO
#define  FIFO_TNR_RD			0x10000000L		// TNR Read FIFO
#define  FIFO_TNR_WR			0x20000000L		// TNR Write FIFO

typedef enum
{
   gmd_RESET_READ_FIFO,
   gmd_RESET_WRITE_FIFO,
   gmd_REQUEST_READ_FIFO,
   gmd_REQUEST_WRITE_FIFO,
   gmd_RELEASE_READ_FIFO,
   gmd_RELEASE_WRITE_FIFO,
} gmt_FSB_FIFO_ACTION;

typedef enum
{
   gmd_READ,
   gmd_WRITE,
} gmt_SDRAM_ACTION;

//
// Scaler mode
//
typedef enum
{
    SCALER_ZOOM,
    SCALER_SHRINK,
   SCALER_BYPASS
} gmt_ScalerMode;

//
// Power down modules
//
typedef enum
{
   POWER_IMP,
   POWER_IPP,
   POWER_DISPLAY,
   POWER_AVS,
   POWER_MC,
   POWER_DDS,
   POWER_SDDS,
   POWER_DDDS,
   POWER_MEMORY_CONTROLLER,
   POWER_IFM,
   POWER_LVDS,
   POWER_ADC,
   POWER_CLOCK_CONFIG1,
   POWER_CLOCK_CONFIG2,
   POWER_MC_CLK,
   POWER_RCLK,
   POWER_OCM_CLK,
   POWER_BYPASS,
   POWER_LAST_BLOCK_INDEX
} gmt_POWER_BLOCKS;

//
// Source Types
typedef enum
{
   DIP_A = 0,
   DIP_B = 0,
   HDMI_SRC,
   AIP_HD_GFX,
   DVI1_SRC,
   DP2_SRC,	
   DP1_SRC,
   STG_TIM,
   NUM_OF_CH_SRC
} gmt_SOURCE_TYPE;

//
// Blender mode
//
typedef enum
{
   PIP_ON_MAIN,
   MAIN_ON_PIP
}gmt_BLENDER_MODE;

//
// PIP mode
//
typedef enum
{
   MODE_NO_PIP,
   MODE_SINGLE_PIP,
    MODE_MULTI_PIP
}gmt_PIP_MODE;

//
// PIP highlight origin
//
typedef enum
{
   ORIGIN_ON_BORDER,
   ORIGIN_INSIDE_BORDER
}gmt_PIP_HIGHLIGHT_ORIGIN;

//
// Sync Flags
//
typedef enum
{
   gmd_SYNC_UV_ALIGN    		= BIT0,			
   gmd_SYNC_EVEN_OFFSET    	= BIT1,
   gmd_SYNC_ODD_TYPE_EXT   	= BIT2,
   gmd_SYNC_ODD_INV     			= BIT3,
   gmd_SYNC_OFFSET_HSYNC 	= BIT4,
   gmd_SYNC_UV_ALIGN_TO_DE 	= BIT5,
} gmt_SYNC_FLAGS;

//
// Define sync type
//
typedef enum SYNC_TYPE
{
   NO_SYNC_STATE,          // No SYnc
   DSS_STATE,              // Normal mode
   DCS_STATE,              // Composite Sync
   SOG_STATE,              // SOG State
   MAX_SYNC_TYPE
} gmt_SYNC_TYPE;

//
// Aspect Ratios
//
typedef enum
{
   gmd_ASPECT_4x3 = 0,        // 4:3
   gmd_ASPECT_16x9,        // 16:9
   gmd_ASPECT_15x9,        // 15:9
   gmd_ASPECT_5x4,            // 5:4
} gmt_ASPECT_RATIO;

//
// Panoramic Configuration
//
typedef enum
{
   gmd_PANORAMIC_OFF,         // Turns panoramic control off
   gmd_PANORAMIC_ON,       // Configures panoramic registers and turns it on
} gmt_PANORAMIC;


//
// VCR Trick Modes
//
typedef enum
{
   gmd_INPUT_VCR_UNKNOWN,     // Unknown
   gmd_INPUT_VCR_STOP,        // VCR Detected
   gmd_INPUT_VCR_PLAY,     // Play mode
   gmd_INPUT_VCR_FF,       // Fast Forward
   gmd_INPUT_VCR_REW,         // Rewind
   gmd_INPUT_VCR_PAUSE        // Pause
} gmt_VCR_MODES;

//
// WSS Modes
//
typedef enum
{
   Wss_PAL,       		// PAL
   Wss_NTSC,         	// NTSC
   Wss_MULTI,        	// Multi tv system
   Wss_480i,         	// 480i
   Wss_480p,         	// 480p
   Wss_576i,			// 576i
   Wss_576p,			// 576p
   Wss_720p,         	// 720p
   Wss_1080i,        	// 1080i
   Wss_UNKOWN_MODE     	//unknown
}Wss_Mode;

//
// WSS Lines
//
typedef enum
{
   WSS_WORD0,
   WSS_WORD1,
   WSS_WORD2,
   WSS_CRCC,
   WSS_RAW_DATA,
   WSS_MAX
} WSS_LINE;

//
// WSS Aspect ratio
//
typedef enum
{
	WSS_4_3,
	WSS_4_3_LETTER,
	WSS_16_9,
	WSS_UNKNOWN_ASPECT_RATIO
} WSS_ASPECT_RATIO;


typedef enum
{
   STATE_IDLE,
   STATE_MONITOR,
}WSS_State;
// Mode Flags (Mode Determination)
//
// Set by Mode Determination (Application) and used by library
// These flags all contain confirmed and validated information
//
typedef enum
{
   gmd_MODE_GRAPHICS          = 0,     // Mode is Graphics
   gmd_MODE_SDVIDEO           = 1,     // Mode is SD Video
   gmd_MODE_HDVIDEO           = 2,     // Mode is HD Video
   gmd_MODE_EDVIDEO           = 3,     // Mode is Extended Definition Video (480p/576p/etc)
   gmd_MODE_INTERLACED           = BIT2,     // Mode is Interlaced
   gmd_MODE_NEG_HSYNC            = BIT3,     // Mode HSync is Negative
   gmd_MODE_NEG_VSYNC            = BIT4,     // Mode VSync is Negative
   gmd_MODE_SOG               = BIT5,     // Mode is SOG or Embedded sync
   gmd_MODE_CSYNC             = BIT6,     // Mode is Composite Sync
   gmd_MODE_CAPTURE_CROP				= BIT7,		// Capture Crop Mode
   gmd_MODE_CVT			= BIT8,		// CVT Mode
   gmd_MODE_ESTIMATED            = BIT9,     // Mode is Estimated
   gmd_MODE_OUT_RANGE            = BIT10, // Signal is Out of Range
   gmd_MODE_SUB_SAMPLE           = BIT11, // Sub-sample input mode
   gmd_MODE_UNKNOWN           = BIT12, // Mode not found in Rom table
   gmd_MODE_VALID             = BIT13, // Mode is Stable and Valid
   gmd_MODE_COMPLETE          = BIT14, // Mode has been configured
#pragma warn -sig // BIT15 Causes "Conversion may lose significant digits" warning because we are using the sign bit
   gmd_MODE_EMBEDDED_VIDEO       = BIT15, // Mode is a Video Image in a Graphic signal
#pragma warn +sig

   gmd_MODE_SIGNAL_MASK       = (BIT0|BIT1), // Signal Types (Graphics, SDVideo, HDVideo, etc)

} gmt_MODE_FLAGS;

//
// Input Flags (Mode Detection)
//
// Set by library or Mode Handler and used by Mode Detection
// These flags are unconfirmed and only used by Mode Determination
// during validation of the mode
//
typedef enum
{
   gmd_INPUT_MACROVISION   = BIT0,        // Input contains Macrovision
   gmd_INPUT_WSS        = BIT1,        // WSS Detected
   gmd_INPUT_INTERLACED    = BIT2,        // Input is Interlaced
   gmd_INPUT_NEG_HSYNC  = BIT3,        // HSync is Negative
   gmd_INPUT_NEG_VSYNC  = BIT4,        // VSync is Negative

   // for input color flag
   gmd_INPUT_xvYCC_601 = BIT5,
   gmd_INPUT_xvYCC_709 = BIT6,

} gmt_INPUT_FLAGS;

//
// Control flag for gm_HardwareUpdate handler
//
typedef enum
{
   DECODER_UPDATE_HDLR  = BIT0,
   DECODER_NOISE_FILETR = BIT1,
   INPUT_DETECT_HDLR       = BIT2,
   MONITOR_MAIN_CH      = BIT3
} gmt_HARDWARE_UPDATE_TASK;

//
// SplashScreen Policy
//
typedef enum
{
   gmd_SPALSHSCREEN_Always,
   gmd_SPLASHSCREEN_Never,
   gmd_SPLASHSCREEN_AtPowerUp,
} gmt_SPLASHSCREEN_POLICY;

// Video LUT table
typedef enum
{
   RED_VLUT_COMPONENT      = 0,
   GREEN_VLUT_COMPONENT = 1,
   BLUE_VLUT_COMPONENT     = 2,
   RED_STEEP_VLUT_COMPONENT      = 3,
   GREEN_STEEP_VLUT_COMPONENT = 4,
   BLUE_STEEP_VLUT_COMPONENT     = 5,   
} gmt_VLUT_COLOR_COMPONENT;

// Coefficient Table
typedef enum
{
   Y_VF_COEFF_COMPONENT = 0,
   UV_VF_COEFF_COMPONENT   = 1,
} gmt_VF_COEFF_COLOR_COMPONENT;

// RGB colors
typedef enum
{
   gmd_RED_COMPONENT    = 0,
   gmd_GREEN_COMPONENT     = 1,
   gmd_BLUE_COMPONENT      = 2,
   gmd_ALL_COMPONENT
}gmt_RGB_COLOR;

// IBD Options
typedef enum
{
   gmd_IBD_MEASURE_SOURCE,
   gmd_IBD_MEASURE_WINDOW,
   gmd_IBD_RGB_SELECT,
   gmd_IBD_THRESHOLD,
   gmd_IBD_ENABLE,
}gmt_IBD_OPTION;

// IBD Source Type
typedef enum
{
   gmd_DATA,
   gmd_DE,
}gmt_IBD_SOURCE_TYPE;

enum
{
   VF_SAFE_MODE         = 0,
   VF_HIGH_PRIORITY_MODE   = 1,
   VF_MAX_NUMBER_ACCESS_MODES
};

//
// VBI Service Definitions
//
typedef enum
{
   VBI_SERVICE_NONE,          // no VBI servie enabled
   VBI_SERVICE_TTX,           // Enable Teletext
   VBI_SERVICE_CC,            // Enable Closed Caption
   VBI_SERVICE_VPS,           // Enable VPS
   VBI_SERVICE_WSS,           // Enable WSS
   VBI_SERVICE_VPS_2,         // Enable different capture window for VPS
   VBI_SERVICE_PAL_N_CC    // Enable PAL N Closed Caption
} gmt_VBI_SERVICE;

//
// Register Blocks
//
typedef BYTE *gmt_REG_BLOCK;

//
// Defines the State Information (From IROM AFP.H)
//
enum
{
    VT_MODE,
    SM_MODE,
    AFP_TEMP_VT_MODE,
    SWITCHTO_AFP_MODE,
    AFP_MODE,
    GFX_MODE,
    GFX_TEMP_VT_MODE
};

//
// Enumerators for 2Wire function set
//
typedef enum
{
   RING_ALL = 0,  // all rings are enabled, for BACKWARD COMPATIBILITY
   RING_NOCHANGES,
   RING0,
   RING1,
   RING2,
} gmt_2WIRE_RING;

typedef enum
{
   MASTER_2WIRE_1,   			// HW Master 2Wire 1
#if (FEATURE_2WIRE_NUM > 1)
   MASTER_2WIRE_2,       		// HW Master 2Wire 2
#endif
#if (FEATURE_2WIRE_NUM > 2)
   MASTER_2WIRE_LPM_C,	
#endif
} gmt_2WIRE_MASTER_IDX;

typedef enum
{
   NO_STOP_BIT = 0,
   STOP_BIT,
} gmt_2WIRE_STOP;

typedef enum
{
	MASTER_2WIRE_DMA,
	MASTER_2WIRE_I2C,
} gmt_2WIRE_MODE;

#ifdef __PARADIGM__
   #pragma pack(push)
   #pragma pack(1)
#endif
typedef struct
{
	gmt_2WIRE_RING			B_Ring;			// Output Ring
	BYTE					B_DevAddress;	// device address
	gmt_2WIRE_MASTER_IDX	B_2WireMaster;	// I2C Master (always 0 for FLI8532, 0 or 1 for FLI8668)
	WORD					W_ClkScale;		// clock pre-scale for desired frequency
	gmt_2WIRE_MODE			B_Mode;			// DMA 0 / I2C 1
	WORD					W_Timeout;		// timeout in 10th of microseconds
} gmt_2WIRE_STRUCT;
#ifdef __PARADIGM__
   #pragma pack(pop)
#endif

//
// Window structure
//
#ifdef __PARADIGM__
   #pragma pack(push)
   #pragma pack(1)
#endif
typedef struct
{
   WORD W_Hstart;          // Horizontal start
   WORD W_Vstart;          // Vertical start
   WORD W_Width;           // Horizontal width
   WORD W_Height;          // Vertical height
} gmt_WindowStruct;
#ifdef __PARADIGM__
   #pragma pack(pop)
#endif

typedef struct
{
	WORD Htotal; //Pixels
	WORD Vtotal;
	WORD Hfrq;
	WORD Vfrq;
	WORD Hstart;
	WORD Vstart;
	WORD Hactive;
	WORD Vactive;
    gmt_INPUT_FLAGS Flags;
    gmt_MODE_FLAGS ModeFlags;
    BYTE ModeIndex;
    BYTE ColorDomain;   // 20110718
} gmt_InputStruct;
        
// 20110718
typedef enum
{
   Input_Auto = 0,
   Input_RGB,
   Input_YUV_Full,
   Input_YUV_Reduce,
} gmt_input_colordomain;

typedef enum
{
   Output_Auto = 0,
   Output_RGB,
   Output_YUV_Full,
   Output_YUV_Reduce,
} gmt_onput_colordomain;

//
// Color control
//
#ifdef __PARADIGM__
   #pragma pack(push)
   #pragma pack(1)
#endif

typedef struct
{
   SBYTE sB_Brightness;
   WORD W_Saturation;
   WORD W_Contrast;
   SWORD sW_Hue;
   WORD W_RedGain;
   WORD W_GreenGain;
   WORD W_BlueGain;
   SBYTE sB_RedOff;
   SBYTE sB_GreenOff;
   SBYTE sB_BlueOff; 	
#ifdef YUV_GAIN_ADJ_IN_YUV_DOMAIN
   WORD W_YGain;
   WORD W_UGain;
   WORD W_VGain;
#endif // #ifdef YUV_GAIN_ADJ_IN_YUV_DOMAIN
} gmt_ColorStruct;

#ifdef __PARADIGM__
   #pragma pack(pop)
#endif

#ifdef __PARADIGM__
   #pragma pack(push)
   #pragma pack(1)
#endif

typedef struct
{
   //
   // RGBYCM - Saturation
   //
   SBYTE sB_AcmRedSat;
   SBYTE sB_AcmGreenSat;
   SBYTE sB_AcmBlueSat;
   SBYTE sB_AcmYellowSat;
   SBYTE sB_AcmCyanSat;
   SBYTE sB_AcmMagentaSat;
   //
   // RGBYCM - Hue
   //
   SBYTE sB_AcmRedHue;
   SBYTE sB_AcmGreenHue;
   SBYTE sB_AcmBlueHue;
   SBYTE sB_AcmYellowHue;
   SBYTE sB_AcmCyanHue;
   SBYTE sB_AcmMagentaHue;
   //
   // FleshTone
   //
   BYTE B_AcmFleshTone;
}gmt_AcmColorStruct;

#ifdef __PARADIGM__
   #pragma pack(pop)
#endif

//
// Panoramic Settings
//
#ifdef __PARADIGM__
   #pragma pack(push)
   #pragma pack(1)
#endif

typedef struct
{
   WORD        W_PanelWidth;
   WORD        W_InputWidth;
   WORD        W_InputHeight;
   WORD        W_Zone1;
   WORD        W_Zone2;
   WORD        W_PB;
   WORD        W_NL_Hi;
   WORD        W_NL_Lo;
   WORD        W_Scale_Hi;
   WORD        W_Scale_Lo;
   BOOL        ExpandSides;
} gmt_StandardPanorStruct;

#ifdef __PARADIGM__
   #pragma pack(pop)
#endif

// passed as a parameter to gm_AutoGeometry()
typedef enum
{
   gmd_DEFAULT_AUTO = 0,            // whatever the default is

                                    // just one of these should on
   gmd_INSTANT_AUTO = BIT0,         // 1 - instant auto
   gmd_FORCE_WIDTH = BIT1,          // 1 - width & sum of diff

   gmd_STEALTH_MODE = BIT2,         // 1 - use stealth mode (only with instant auto)
   gmd_STEALTH_FREEZE = BIT3,       // 1 - stealth by freezing panel control signals
                                    // 0 - stealth by using panel background
   gmd_PHASE_SKEW = BIT4,           // 1 - skew RGB phases for best phase (only with instant auto)
   gmd_AUTO_FILTER = BIT5           // 1 - choose filtering for best phase (only with instant auto)
} gmt_AUTO_ADJ_MODE_MASK;


// passed as a parameter to gm_AutoGeometry() 
typedef enum
{
   DEFAULT_AUTO 	= 0,				  // Do IA first, if failed, run Force_Width and SumOfDiff
   IA_HTOTAL		= BIT0,
   IBD_WIDTH_BASE	= BIT1,
   IA_PHASE		= BIT2,
   SUM_OF_DIFF 	= BIT3,
} gmt_AUTO_ADJ_MODE;

//
// IA Return definitions
//
typedef enum
{
   IA_RET_OK = 0,
   IA_RET_FAIL = 1,
   IA_RET_MEMORY_ERR = 2,
   IA_RET_ABORT = 3,
   IA_RET_TMO = 4
} gmt_IA_RET_STAT;

typedef enum
{
   gmd_2WIRE_WR = 0, // Write to slave
   gmd_2WIRE_RD = 1, // Read from slave
} gmt_2WIRE_CONTROL;

typedef enum
{
   gmd_NACK_CMD = BIT0, // When a receiver, send ACK(0) or NAK(1)
   gmd_WR = BIT1, // Write to slave
   gmd_RD = BIT2, // Read from slave
   gmd_STO = BIT3, // Generate stop condition
   gmd_STA = BIT4, // Generate (repeated) start condition
   gmd_CTRL_MODE = BIT5, //
} gmt_2WIRE_COMMAND;

//
// Color space setting
//
#if 0 // (CHIP_ID == STDP93xx)
typedef enum
{
	MAIN_MATRIX_ENABLE = BIT0,
	PIP_MATRIX_ENABLE  = BIT1,

   SEC_COEFF_INSIDE_AND_OUTSIDE = 0,
   PRI_COEFF_INSIDE_AND_SEC_COEFF_OUTSIDE = BIT4,
   SEC_COEFF_INSIDE_AND_PRI_COEFF_OUTSIDE = BIT5,
   PRI_COEFF_INSIDE_AND_OUTSIDE = BIT4+BIT5,

} gmt_COLOR_SPACE_SETTING;

// #else // if (CHIP_ID == FLI326xxBx)
typedef enum
{
	INSIDE_WINDOW_MULT_ENABLE=BIT0,
	OUTSIDE_WINDOW_MULT_ENABLE=BIT1,
	INSIDE_WINDOW_COLOR_YUV=BIT2,
	OUTSIDE_WINDOW_COLOR_YUV=BIT3,
	INSIDE_WINDOW_MULT_DISABLE=0,
	OUTSIDE_WINDOW_MULT_DISABLE=0,
	INSIDE_WINDOW_COLOR_RGB=0,
	OUTSIDE_WINDOW_COLOR_RGB=0,
	OUTPUT_IS_YUV=BIT4,
	OUTPUT_IS_RGB=0,
} gmt_COLOR_SPACE_SETTING;
#endif

typedef struct
{
	SWORD ROM * Coeff;
	BYTE ROM * Shift;

}gmt_GetCSC;

//
// Adc Id
//
typedef enum
{
   gmd_ADC_0,
   gmd_ADC_1,
   gmd_ADC_NUM,
}gmt_ADC_ID;

//
// Input parameter for "gm_AdaptiveGlobalMotion"
//
enum ADAPTIVE_MOTION
{
   MADI_OFF    =0,
   MADI_ADAPTIVE  =1,
   MADI_STANDARD  =2,
   MADI_LOW_MOTION =3,
   MADI_HIGH_MOTION=4,
   TNR_OFF     =0,
   TNR_ADAPTIVE   =1,
   TNR_LOW   =2,
   TNR_MEDIUM    =3,
   TNR_HIGH   =4,
   TNR_SUPER_HIGH = 5,
   SHARPNOISE_OFF     =0,
   SHARPNOISE_ADAPTIVE   =1,
   SHARPNOISE_LOW   =2,
   SHARPNOISE_MEDIUM    =3,
   SHARPNOISE_HIGH   =4,
   CCS_OFF     =0,
   CCS_ADAPTIVE   =1,
   CCS_STANDARD   =2,
   NOISE_MONITOR_OFF =0,
   NOISE_MONITOR_RAW    =1,
   NOISE_MONITOR_mV  =2,
   NOISE_MONITOR_db  =3,
   MOTION_VALUE_OFF     =0,
   MOTION_VALUE_RS      =1,
   MOTION_VALUE_CORR_RS    =2,
   MOTION_VALUE_dS      =3,
   MOTION_REGION_OFF       =0,
   MOTION_REGION_ON     =1
};

//
// SCART
//
typedef enum
{
   SCART_FB_LOW = 0,
   SCART_FB_HIGH = 1,
   SCART_FB_TOGGLE = 2
}gmt_SCART_FB_STAT;

//
// Blue stretch region enum
//
typedef enum
{
   gmd_BLUE_STRETCH_FULL_SCREEN=0,
   gmd_BLUE_STRETCH_MAIN_WIN_ONLY,
   gmd_BLUE_STRETCH_PIP_WIN_ONLY,
   gmd_BLUE_STRETCH_VIDEO_WIN_ONLY
} gmt_BlueStretchRegion;

//
// Blue stretch structure
//
#ifdef __PARADIGM__
   #pragma pack(push)
   #pragma pack(1)
#endif
typedef struct
{
   BYTE B_Gain;
   BYTE B_Area;
   gmt_BlueStretchRegion Region;
   BOOL Y_OPT_EN;
   BOOL G_OPT_EN;
   BOOL SAT_LIM_EN;
   WORD G_OPTION;
   WORD LUMA_MIN;
   WORD SAT_LIM;
} gmt_BlueStretchStruct;
#ifdef __PARADIGM__
   #pragma pack(pop)
#endif

//
// LetterBox Window
//
typedef struct
{
	WORD W_TopHstart;
	WORD W_TopHend;
	WORD W_TopVstart;
	WORD W_TopVend;
	WORD W_BottomHstart;
	WORD W_BottomHend;
	WORD W_BottomVstart;
	WORD W_BottomVend;
} gmt_LB_WINDOW;

//
// LetterBox Modes
//
typedef enum
{
	LB_480i,			// 480i
	LB_480p,			// 480p
	LB_576i,			// 576i
	LB_576p,			// 576p
	LB_NO_SUPPORT 		// No support mode
} gmt_LB_MODE;

//
// Fleshtone region enum
//
typedef enum
{
	gmd_FLESHTONE_DISABLED,
	gmd_FLESHTONE_INSIDE_VIDEO_WIN,
	gmd_FLESHTONE_OUTSIDE_VIDEO_WIN,
	gmd_FLESHTONE_INSIDE_AND_OUTSIDE_VIDEO_WIN
} gmt_FleshToneRegion;

//
// Fleshtone structure
//
#ifdef __PARADIGM__
   #pragma pack(push)
   #pragma pack(1)
#endif
typedef struct
{
	gmt_FleshToneRegion B_Region;
	BOOL Bt_0_TO_75_EN;
	BOOL Bt_CurveAdj;
	BYTE B_Aperture;
	BYTE B_Gain;
} gmt_FleshToneStruct;
#ifdef __PARADIGM__
   #pragma pack(pop)
#endif

//
// Fleshtone coefficient structure
//
#ifdef __PARADIGM__
   #pragma pack(push)
   #pragma pack(1)
#endif
typedef struct
{
	BYTE B_Coef_C1;
	BYTE B_Coef_C2;
	BYTE B_Coef_C3;
	BYTE B_Coef_C4;
	BYTE B_Coef_C5;
	BYTE B_Coef_C6;
	BYTE B_Coef_C7;
	BYTE B_Coef_C8;
} gmt_FleshToneCoefStruct;
#ifdef __PARADIGM__
   #pragma pack(pop)
#endif

//
// ODP Pixel Grab
//
typedef struct
{
	WORD	W_PixelGrabRed;
	WORD	W_PixelGrabGreen;
	WORD	W_PixelGrabBlue;
} gmt_PixelGrab;

//
// IFM Measurement Struct
//
typedef struct
{
	WORD			W_VTotal;
	WORD			W_VsyncPulse;
	WORD			W_HsyncPulse; 
	WORD			W_HFreq;
	WORD			W_VFreq;
	gmt_INPUT_FLAGS	W_DetectFlags;
} gmt_InputMeasurementStruct;

//
// IFM Control Field enum
//
typedef enum
{
	gmd_IFM_ENABLE,
	gmd_IFM_NEW_MEASURE_FROM_NEXT_VS,
	gmd_IFM_OFFSET_HSYNC,
	gmd_IFM_SELECT_FIELD,
	gmd_IFM_MEASURE_SELECTED_FIELD_ONLY,
	gmd_IFM_INT_GEN_ODD_EVEN_FIELD_ID,
	gmd_IFM_CSYNC_MEASUREMENT,
	gmd_IFM_INVERT_ODD_EVEN_FIELD_ID,
	gmd_IFM_FIELD_DETECT_MODE,
	gmd_IFM_MCLK_PER_FRAME_MEASURE,
	gmd_IFM_COUNT_MCLK_BETWEEN_VS,
	gmd_IFM_TOGGLE_ODD_EVERY_FIELD,
	NUM_OF_IFM_BIT_FIELDS,
} gmt_IFMCtrlBitFieldType;

//
// AVS Source
//
typedef enum
{
	gmd_AVS_SOURCE_I2S_PINS,
	gmd_AVS_SOURCE_HDMI_I2S_OUTPUT,
	gmd_AVS_SOURCE_HDMI_SPDIF_OUTPUT,
	gmd_AVS_SOURCE_HDMI_I2S_SPDIF_OUTPUT
}gmt_AVS_SOURCE;

//A bit feeld to read into library panel exceptions;
//Do not use reserved bits as is. First specify necessary fields instead.
typedef struct
{
	WORD DflErrLimit :1;
	WORD Reserved:15;
}gmt_PanelSetupExeptions; // */

//
// Mode Type
//
typedef enum
{
	gmd_UNKNOWN_MODE = 0,     //unknown mode
	gmd_GRAPHICS_MODE,		//graphics mode
	gmd_NTSC_480I,				// NTSC/ 480i
	gmd_PAL_576I,					// PAL/ 576i
	gmd_480P,						// 480P
	gmd_576P,						// 576P
	gmd_720P,						// 720P
	gmd_1080I,						// 1080I
	gmd_1080P						// 1080P
} gmt_MODE_TYPE;

//
// Adaptive TNR methods
//
// enum to name Adaptive TNR and SharpNoise methods
typedef enum gmt_AdaptiveTnrSharpNoiseMethods_t
{
    VQLAB_MAY07_METHOD = 0,
    CORTEZ27REL_METHOD = 1
}gmt_AdaptiveTnrSharpNoiseMethods;


typedef enum
{
	gm_COLOR_DEPTH_UNKNOWN,
	gm_COLOR_DEPTH_18BPP,
    gm_COLOR_DEPTH_24BPP,
    gm_COLOR_DEPTH_30BPP,
    gm_COLOR_DEPTH_36BPP,
    gm_COLOR_DEPTH_48BPP
} gmt_COLOR_DEPTH;

typedef enum
{
   gmd_RGB_QUANTIZATION_DEFAULT,
   gmd_RGB_QUANTIZATION_LIMITED,
   gmd_RGB_QUANTIZATION_FULL,
   gmd_RGB_QUANTIZATION_UNKNOW
}gmt_RGB_Quantization;

typedef enum
{
   gmd_YCC_QUANTIZATION_LIMITED,
   gmd_YCC_QUANTIZATION_FULL,
   gmd_YCC_QUANTIZATION_UNKNOW
}gmt_YCC_Quantization;

typedef struct
{
	BYTE				B_PixPerClock;
	WORD				W_Width;
	WORD				W_Height;	
	BYTE				B_MaxVFreq;
	BYTE				B_MinVFreq;
	DWORD				DW_MaxPClk; // KHz
}gmt_PanelInfo;


typedef struct
{
	DWORD		PixelClockHz;
	WORD		HActiveStart;
	WORD		HActive ;
	WORD		HTotal;
	WORD		VActiveStart;
	WORD		VActive ;
	WORD		VTotal;
	WORD		Interlaced:1;
	WORD		HS_Positive:1;
	WORD		VS_Positive:1;	
} gmt_FrontEndSignalTiming_t;


typedef enum
{
    gm_SRC_AVDDS1=0,
    gm_SRC_AVDDS2,
	gm_SRC_AVDDS3,
	gm_SRC_AVDDS4,
	gm_SRC_AVDDS5,
	gm_SRC_AVDDS_INVALID
} gm_AVDdsSrcTypes_t;

//******************************************************************************
//  P R O T O T Y P E S
//******************************************************************************

//***************
// IROM
//***************

//
// Host register update
//
void           gm_ForceUpdate(gmt_PHY_CH B_Channel, gmt_HOST_UPDATE B_Clock);
void           gm_SyncUpdate(gmt_PHY_CH B_Channel, gmt_HOST_UPDATE B_Clock);
void           gm_SyncUpdateWait(gmt_PHY_CH B_Channel, gmt_HOST_UPDATE B_Clock);

//
// Channel
//
gmt_RET_STAT      gm_ValidatePhyCh(gmt_PHY_CH B_Channel);

//
// ADC Keypad
//
BYTE           gm_ScanAdcKey(BYTE B_Channel);

//
// 2Wire
//
gmt_RET_PASSFAIL  gm_2WireStart(BYTE B_DevAddr, gmt_2WIRE_CONTROL Ctrl);
gmt_RET_PASSFAIL  gm_2WireCheck(gmt_2WIRE_RING B_Port, BYTE B_DevAddr);
void          	  gm_2WireStop(void);
gmt_RET_PASSFAIL  gm_2WireGetByte(WORD W_Cmd, BYTE * Bp_Data);
gmt_RET_PASSFAIL  gm__2WireGetByte(gmt_2WIRE_COMMAND Cmd, BYTE * Bp_Data);
gmt_RET_PASSFAIL  gm_2WirePutByte(BYTE far * Bp_Data);
gmt_RET_STAT   	  gm_2WireWriteAddrBuffer(BYTE B_DevAddr, BYTE far *Bp_RegAddr, BYTE B_AddrLen, BYTE far *Bp_Data, WORD W_Len);
gmt_RET_STAT      gm_2WireReadAddrBuffer (BYTE B_DevAddr, BYTE far *Bp_RegAddr, BYTE B_AddrLen, BYTE     *Bp_Data, WORD W_Len);
gmt_RET_PASSFAIL  gm_Write2WireBlock(gmt_2WIRE_RING B_Port, BYTE B_DevAddress, BYTE far *Bp_Buffer, BYTE B_Length, gmt_2WIRE_STOP B_Stop);
gmt_RET_PASSFAIL  gm_Read2WireBlock (gmt_2WIRE_RING B_Port, BYTE B_DevAddress, BYTE     *Bp_Buffer, BYTE B_Length, gmt_2WIRE_STOP B_Stop);
gmt_RET_PASSFAIL  gm_2WireConfig(gmt_2WIRE_MASTER_IDX B_2WireMaster, WORD W_2WireFreq, gmt_2WIRE_RING B_Port);
// NOTE: Selector function for the I2C engine (gmt_2WIRE_MASTER_IDX) meaningful only to Cortez Advanced which has 2 engines.
// For all other chips, the call is not necessary. By default, the selection is set to MASTER_2WIRE_1.
void			  gm_2WireSelectIdx (gmt_2WIRE_MASTER_IDX B_Idx);
void 			  gm_2WireSetTimeout(WORD W_Timeout);

//
// Memory
//
gmt_RET_STAT		gm_MemoryAllocate(gmt_BUFF_TYPE B_StaticBuffers, ROM gmt_MemoryAllocateParams *ptrParam);
gmt_RET_STAT		gm_MemorySetup(gmt_PHY_CH B_Channel);
gmt_RET_STAT		gm_MemorySizeAdjust(gmt_PHY_CH B_Channel);
gmt_RET_STAT		gm_MemoryIsAvailable(gmt_PHY_CH B_Channel);
DWORD				gm_MemoryGetInfo(gmt_BUFF_TYPE B_BuffType, gmt_BUFF_PROPERTY B_BuffProperty);
DWORD				gm_MemoryComputeSize(gmt_PHY_CH B_Channel);
gmt_RET_STAT		gm_MemoryBypass(gmt_PHY_CH B_Channel, BOOL B_Enable);
void				gm_MemoryWriteFreeze(gmt_PHY_CH B_Channel, BOOL Bt_Enable);
gmt_RET_PASSFAIL	gm_MultiPipMemorySetup(void);
gmt_RET_STAT 		gm_SdramMemorySetup(void);
void				gm_FifoControl(WORD W_Enable, gmt_FIFO_TYPE D_Fifo);
#if (FEATURE_DDR_EXECUTION == ENABLE)
	DWORD gm_MemoryCodeStartAddress(void);
	DWORD gm_MemoryCodeAllocate(DWORD D_StartAddr);
	void gm_MemoryCodeDownload(void);
	void gm_MemoryCodeStart(void);
	void gm_MemoryCodeStop(void);
	BOOL gm_MemoryCodeVerify(void);
    DWORD gm_MemoryCodeStartAddressInBursts(void);
    DWORD gm_MemoryCodeEndAddressInBursts(void);
    DWORD gm_MemoryCodeSizeInBursts(void);
    void gm_MemoryDataWritePurge(void);
    void gm_MemoryCodeAdjust(DWORD size);
#endif

#if 0 //((FEATURE_DECODER == TWO_DECODER)||(FEATURE_DECODER_MOTION_BUFFER == ENABLE))
    DWORD gm_MemoryDecoder2StartAddressInBursts(void);
    DWORD gm_MemoryDecoder2EndAddressInBursts(void);
    DWORD gm_MemoryDecoder2SizeInBursts(void);
	DWORD gm_MemoryDecoderStartAddressInBursts(void);
	DWORD gm_MemoryDecoderEndAddressInBursts(void);
	DWORD gm_MemoryDecoderSizeInBursts(void);
#endif

//
// Channel Management
//
void           gm_SetMainChannel(gmt_PHY_CH B_Channel);
void           gm_SetPipChannel(gmt_PHY_CH B_Channel);

//
// Vertical Pulse Width settings for SOG detection
//
void gm_SetVPulseWidth(BYTE PulseWidth);
BYTE gm_GetVPulseWidth(void);

//
// IFM
//
void gm_ReadIFMTiming(gmt_PHY_IFM Ifm, gmt_InputMeasurementStruct* IfmStr);
void gm_SetupIFMCtrl 				(gmt_PHY_IFM Ifm, WORD W_IfmCtrl);
void gmp_IFMSetSyncSource			(gmt_PHY_CH B_Channel);
void gm_SetupIFMHLine 				(gmt_PHY_IFM Ifm, WORD W_IfmHLine);
void gm_SetupIFMWatchdog 			(gmt_PHY_IFM Ifm, WORD W_IfmWd);
void gm_SetupIFMChange 				(gmt_PHY_IFM Ifm, WORD W_IfmChange);
void gm_SetInvertIFMOddEven 		(gmt_PHY_IFM Ifm);
void gm_ClearInvertIFMOddEven 		(gmt_PHY_IFM Ifm);
void gm_SetIFMFieldSelect 			(gmt_PHY_IFM Ifm);
void gm_ClearIFMFieldSelect 		(gmt_PHY_IFM Ifm);
void gm_EnableIntGenOddEvenFiledId 	(gmt_PHY_IFM Ifm);
void gm_DisableIntGenOddEvenFiledId (gmt_PHY_IFM Ifm);
void gm_SetIFMCtrlBitField(gmt_PHY_IFM Ifm, gmt_IFMCtrlBitFieldType Type, BOOL Bt_State);

//
// Input
//
typedef struct
{
	WORD HStart;
	WORD VStart;
	WORD HTotal;
	WORD VTotal;
	WORD HActive;
	WORD VActive;
}gmt_IbdStruct;

void           gm_IbdConfig(gmt_PHY_CH B_Channel, gmt_IBD_OPTION B_Option, BYTE B_Value);
gmt_RET_PASSFAIL gm_IbdRead(gmt_PHY_CH B_Channel, gmt_IbdStruct* pIBD);
WORD gm_IbdReadHWidth(gmt_PHY_CH B_Channel);
// NOTE: gm_ReadInputTiming is a function still present becasue of legacy reasons.
// Use of gm_ReadIFMTiming is recommneded
gmt_RET_STAT      gm_InputSetHPosition(gmt_PHY_CH B_Channel);
gmt_RET_STAT      gm_InputSetVPosition(gmt_PHY_CH B_Channel);
void gm_AdjVCaptureForDecVLineVar(gmt_PHY_CH B_Channel,WORD W_CurrentDecoderLineCount);
void           gm_SwapSource(void);
gmt_RET_STAT      gm_InputWaitVsync(gmt_PHY_CH B_Channel, BYTE B_NumSync);
WORD gm_VsyncCheck(gmt_PHY_CH B_Channel);
gmt_RET_STAT      gm_InputSetCaptureWindow(gmt_PHY_CH B_Channel);
void           gm_SetAdcPhase(gmt_PHY_CH B_Channel, BYTE B_Phase);
void           gm_SddsSetHTotal(gmt_ADC_ID B_AdcId, WORD W_HTotal);
void           gm_SetAdcGain(gmt_PHY_CH B_Channel, WORD W_RGain, WORD W_GGain, WORD W_BGain);
void           gm_SetOffset2(gmt_PHY_CH B_Channel, BYTE B_ROff2, BYTE B_GOff2, BYTE B_BOff2);
gmt_RET_PASSFAIL  gm_WaitForInputBlanking (gmt_PHY_CH B_Channel);
gmt_RET_PASSFAIL  gm_InputWaitHLine (gmt_PHY_CH B_Channel, BYTE B_LineNumber);
void           gm_InputCaptureFreeze(gmt_PHY_CH B_Channel, BOOL Bt_State);

//
// Channel
//
gmt_RET_STAT      gm_SetChannelSource(gmt_PHY_CH B_Channel, gmt_SOURCE_TYPE B_Type);
gmt_SOURCE_TYPE      gm_GetChannelSource(gmt_PHY_CH B_Channel);

//
// Product ID
//
WORD			gm_ReadProductIdReg (void);

//
// Product Rev
//
WORD			gm_ReadProductRevReg (void);

//
// Decoder
//

//
// Display
//
gmt_RET_PASSFAIL  gm_WaitForDisplaySync (BYTE B_SyncCount);
gmt_RET_PASSFAIL  gm_WaitDisplayBlanking_CH (gmt_PHY_CH B_Chanel);
gmt_RET_PASSFAIL  gm_WaitDisplayBlanking (void);
void           gm_DisplayOnOff (BOOL Bt_OnOff);
void           gm_DisplaySetSize (gmt_PHY_CH B_Channel);
void           gm_DisplaySetPosition (gmt_PHY_CH B_Channel);
//void			gm_DisplaySetupColorSpace(gmt_PHY_CH B_Channel,
//												gmt_COLOR_SPACE_SETTING W_Setting);
void           gm_HighLightWindowSetup (WORD W_Width, WORD W_Height, WORD W_Color);
void           gm_HighLightWindowSetSize (WORD W_HActive, WORD W_VActive);
void           gm_HighLightWindowSetPosition (WORD W_HStart, WORD W_VStart);
void			gm_HighlightBorder(BOOL B_State, WORD W_BorderWidth, WORD W_BorderHeight,
							WORD W_HighlightColor, gmt_WindowStruct* WinDim);
gmt_RET_STAT      gm_DisplayForceBkgnd (gmt_PHY_CH B_Channel, BYTE B_Enable, WORD W_BkgndColor);
BOOL           gm_DisplayForceBkgndIsOn (gmt_PHY_CH B_Channel);
void           gm_DisplayColorWindowSetup(gmt_WindowStruct* p_WinDim, WORD W_Color);
void           gm_DisplayEnableVlut(BOOL B_State);
void           gm_DisplayBlueStretchCtrl(BOOL Bt_Enable);
void           gm_DisplayBlueStretchSetup(gmt_BlueStretchStruct* BlueStretchPtr);
void           gm_DisplayFleshToneCtrl(BOOL Bt_Enable);
void           gm_DisplayFleshToneSetup(gmt_FleshToneStruct * FleshTonePtr, gmt_FleshToneCoefStruct * FleshToneCoefPtr);

//
// ODP Pixel Grab
//
#if (FEATURE_ODP_PIXEL_GRAB == ENABLE)
void gm_OdpPixelGrab (WORD HPos, WORD VPos, gmt_PixelGrab *OdpPixelGrabValues);
#endif

//
// Scaler
//
gmt_RET_STAT      gm_ScalerSetup(gmt_PHY_CH B_Channel);
//gmt_RET_STAT      gm_ScalerSetHFilterCoeff(BYTE B_TableNum);
gmt_ScalerMode       gm_GetScalerHMode(gmt_PHY_CH B_Channel);
gmt_ScalerMode       gm_GetScalerVMode(gmt_PHY_CH B_Channel);
gmt_RET_STAT      gm_ScalerSetPanoramic (WORD W_PanelWidth, gmt_PANORAMIC W_Mode);
gmt_RET_STAT      gm_ScalerDynamic(BYTE B_ChanAB);
void           gm_ScalerDynamicSet(gmt_PHY_CH B_Channel);
void           gm_ScalerDynamicWait(BYTE W_Wait);
void           gm_ScalerDynamicEnable(gmt_PHY_CH B_Channel, BYTE B_Enable);
void           gm_ScalerSetUseMVFBeforeMemory_1to1(gmt_PHY_CH B_Channel, BOOL B_Flag);

#if (FEATURE_TWO_STAGE_PROCESSING_MODE == ENABLE)
gmt_RET_STAT	gm_ScalerSetupTwoStage(void);
#endif

//
// Power
//
void           gm_PowerUpAll(void);
void              gm_PowerDownAll(void);

//
// Blender
//
void		gm_BlenderOveride(BOOL Bt_OnOff);
void		gm_BlenderOnOff (gmt_PHY_CH B_Channel, BOOL Bt_OnOff);
void		gm_BlenderSetPriority(gmt_BLENDER_MODE B_Mode);
void		gm_BlenderSetLevel(BYTE B_BlendLevel);
void		gm_BlenderSetTables(BYTE B_Nibble);
BOOL		gm_BlenderIsOn (gmt_PHY_CH B_Channel);

// Blender Fail Safe
#if (FEATURE_BLENDER_FAILSAFE != REV_NONE)
void           gm_BlenderFailSafeOnOff (BOOL Bt_OnOff);
#endif

//
// Color matrix (3x3)
//
gmt_GetCSC 		gm_GetCsc(BYTE B_Select);
gmt_RET_STAT 	gm_CscUpdateMatrix (SWORD ROM *CSC_SD_Coeff, SWORD ROM *CSC_HD_Coeff, SWORD ROM *CSC_SD_Coeff_F, SWORD ROM *CSC_HD_Coeff_F);
gmt_RET_STAT    gm_CscUpdateColors(gmt_PHY_CH B_Channel, gmt_ColorStruct * Stp_Parameters);
void           	gm_CscSetVideoWindow (gmt_VIDEO_WIN_CONFIG W_Config);
void gm_CscSetVideoWindowSize (gmt_VIDEO_WIN_CONFIG W_Config);
void LLD_OVPInit(void);

#if (FEATURE_SHARPNESS == REV6)
//
// Enhancer
//
void			gm_EnhancerEnable(BOOL Bt_State);
#endif

//
// Old Enhancer
//
//void			gm_OldEnhancerEnable(BOOL Bt_State);
#if (FEATURE_CROSS_POINT_SWITCH == ENABLE)
// ENH is removed.
//void			gm_OldEnhancerRouting(BOOL Bt_RoutedToMain);
void			gm_ColorResourceRouting(BOOL Bt_RoutedToMain);
#endif

//
// Video Processing
//
gmt_RET_STAT    gm_VideoProcessingEnable(gmt_VIDEO_PROCESSING D_Mode);
gmt_RET_STAT    gm_VideoProcessingDisable(gmt_VIDEO_PROCESSING D_Mode);
gmt_RET_STAT    gm_VideoProcessingAdjustment(gmt_VIDEO_PROCESSING D_Mode, WORD W_Adjustment);
gmt_VIDEO_PROCESSING gm_VideoProcessingValidate(gmt_VIDEO_PROCESSING D_Mode);
void           	gm_TableAccessBridgeSetMode(gmt_TAB_TABLE W_Table, gmt_TAB_TRANSFER_MODE W_TransferMode, gmt_TAB_ACCESS_MODE W_AccessMode);
gmt_RET_STAT    gm_LoadVlutTable(gmt_VLUT_COLOR_COMPONENT W_Color,
                             WORD W_StartIndex, WORD W_Size, WORD far *Wp_Data, BOOL B_ShiftFlag);
gmt_RET_STAT    gm_LoadImpVlutTable(gmt_VLUT_COLOR_COMPONENT W_Color,
                				 WORD W_StartIndex, WORD W_Size, WORD far *Wp_Data, BOOL B_ShiftFlag);
//gmt_RET_STAT    gm_LoadVfCoeffTable(gmt_VF_COEFF_COLOR_COMPONENT W_Color, BYTE far *Bp_Src);
//gmt_RET_STAT    gm_LoadAccTable(WORD W_StartIndex, WORD W_Size, BYTE far *Bp_Data);
gmt_RET_STAT    gm_LoadAcmTable(WORD W_StartIndex, WORD W_Size, WORD far *Wp_Data);
void        	gm_SetScalerSharpness(gmt_PHY_CH B_Channel, WORD W_Sharpness);
DWORD 			gm_GetVScaleValue(gmt_PHY_CH B_Channel);
DWORD 			gm_GetHScaleValue(gmt_PHY_CH B_Channel);
gmt_RET_STAT	gm_ScalerBypass(gmt_PHY_CH B_Channel);
gmt_RET_STAT    gm_UpdateAcm (BYTE B_AcmTable, gmt_AcmColorStruct * Stp_Parameters);
//gmt_RET_STAT    gm_UpdateColorScheme (BYTE B_ColorScheme);
gmt_RET_STAT 	gm_UpdateACM3DColorScheme(BYTE B_ColorScheme);
//BYTE           	gm_GetNumAcmTable (void);

gmt_RET_STAT    gm_UpdateAcc (gmt_IMAGE_SCHEME_MODE ColorScheme);

#if (FEATURE_OVERDRIVE == ENABLE)
gmt_RET_STAT    gm_UpdateOverDrive (gmt_OD_MODE W_OverDriveMode, WORD ROM * Wp_OverDriveLUT1, WORD ROM * Wp_OverDriveLUT2);
void gm_SetOverdriveCompressRatio(BYTE B_OnOff, BYTE B_RatioSelect, WORD W_PanelHeight, WORD W_PanelWidth, BYTE B_EnableBanding);
gmt_RET_STAT    gm_UpdateOverDriveNoiseCoring (gmt_OD_NOISE_CORING_MODE W_OverDriveNoiseCoringMode);
void gm_OverdriveTemp(BYTE B_OD_Temp);
void gm_OverdriveFineGranularity(BYTE B_OD_Fine);
void gm_3D_Overdrive(BOOL B_3D_ODMode, WORD W_IP_VStart, WORD W_IP_VEnd, WORD W_TOPScreen_COEF, DWORD DW_CENScreen_COEF, WORD W_BOT_COEF);
void gm_SetODWindow(WORD W_OD_HStart, WORD W_OD_HEnd, WORD W_OD_VStart, WORD W_OD_VEnd);
#endif

void           	gm_DecompressGammaTable(BYTE far *Bp_Source, WORD *Wp_Dest);
#if (FEATURE_SHARPNESS == REV6)
gmt_RET_STAT	gm_LoadSharpCoeffTable(gmt_TAB_TABLE B_TableType, BYTE far *Bp_Src,WORD DestOffset, WORD BlockSize);
#endif

gmt_RET_STAT	gm_UpdateMpegNrThreshold(BYTE B_MpegThreshold0, BYTE B_MpegThreshold1, BYTE B_MpegThreshold2);

//
// PIP Configuration
//
void           gm_PipSetMode (gmt_PIP_MODE B_Mode);
void           gm_PipBorderSetup (WORD W_BorderWidth, WORD W_BorderHeight, WORD W_BorderColor);
void           gm_PipBorderEnable (BOOL Bt_OnOff);
gmt_RET_STAT      gm_MultiPipSetHighlightWindow (BYTE B_PipNum, gmt_PIP_HIGHLIGHT_ORIGIN OriginFlag);
gmt_RET_PASSFAIL  gm_MultiPipSetStyle (WORD W_PipMask);
gmt_RET_VALID     gm_MultiPipSetActiveWindow (BYTE B_PipNum);
void           gm_MultiPipSetBkgnd (DWORD Dw_BkgndColor);

//
// VBI Driver and Teletext and CC support functions
//
void              gm_VbiInit(void);
gmt_RET_STAT         gm_VBI_TTXReset(void);
gmt_RET_STAT         gm_VBI_ServiceConfig(gmt_VBI_SERVICE ServiceType);
gmt_RET_STAT         gm_VBI_ServiceEnable(void);
gmt_RET_STAT         gm_VBI_ServiceDisable(void);
gmt_RET_PASSFAIL     gm_GetFirstPacket(BYTE *Bp_Dest, WORD *Wp_Len, WORD maxLen);
gmt_RET_PASSFAIL     gm_GetNextPacket(BYTE *Bp_Dest, WORD *Wp_Len, WORD maxLen);
BOOL           gm_GetVbiBufferOverflowStatus(void);
void           gm_ResetVbiBufferOverflowStatus(void);
BOOL           gm_IsVbiBufferAboveThreshold( const WORD W_VbiThreshold );
WORD gm_VBI_SlicerCcOpen(void (far *ptVoidFn)(BYTE, BYTE, BYTE));

//******************************************************************************
//  M A C R O S
//******************************************************************************

#define IsCUT2_1() (gm_ReadProductRevReg() == REV_CODE_2_1)

#define gm_GetSignalType(CH)  (stInput[(CH)].ModeFlags & gmd_MODE_SIGNAL_MASK)
#define  gm_IsVideo(CH)       (gm_GetSignalType((CH)) > gmd_MODE_GRAPHICS)
#define  gm_IsSDVideo(CH)     (gm_GetSignalType((CH)) == gmd_MODE_SDVIDEO)
#define  gm_IsHDVideo(CH)     (gm_GetSignalType((CH)) == gmd_MODE_HDVIDEO)
#define  gm_IsEDVideo(CH)     (gm_GetSignalType((CH)) == gmd_MODE_EDVIDEO)
#define  gm_IsAnyHDVideo(CH)     (gm_GetSignalType((CH)) >= gmd_MODE_HDVIDEO)
#define  gm_IsGraphics(CH)    (gm_GetSignalType((CH)) == gmd_MODE_GRAPHICS)
#define  gm_IsInterlaced(CH)     ((stInput[CH].ModeFlags & gmd_MODE_INTERLACED)\
                          == gmd_MODE_INTERLACED)
#define  gm_IsValidSignal(CH) ((stInput[(CH)].ModeFlags & gmd_MODE_VALID)   \
                          == gmd_MODE_VALID)
#define  gm_IsModeComplete(CH)   ((stInput[(CH)].ModeFlags & gmd_MODE_COMPLETE)\
                          == gmd_MODE_COMPLETE)
#define  gm_IsEmbeddedVideo(CH)  ((stInput[(CH)].ModeFlags & gmd_MODE_EMBEDDED_VIDEO)\
                          == gmd_MODE_EMBEDDED_VIDEO)
#define  gm_IsCaptureCrop(CH)     ((stInput[CH].ModeFlags & gmd_MODE_CAPTURE_CROP)\
                          == gmd_MODE_CAPTURE_CROP)
#define  gm_OtherChannel(CH)     (1-(CH))

#define  gm_IsMADIEnabled()       ((gmvd_VideoProcessingEnabled & VIDEO_MADI) == VIDEO_MADI)
#define  gm_IsDCDIMainEnabled()       ((gmvd_VideoProcessingEnabled & VIDEO_DCDI_MAIN) == VIDEO_DCDI_MAIN)
#define  gm_IsFilmModeEnabled()   ((gmvd_VideoProcessingEnabled & VIDEO_FILMMODE) == VIDEO_FILMMODE)
#if (FEATURE_CCS == ENABLE)
#define  gm_IsCCSEnabled()     ((gmvd_VideoProcessingEnabled & VIDEO_CCS) == VIDEO_CCS)
#endif
#define  gm_IsTNREnabled()     ((gmvd_VideoProcessingEnabled & VIDEO_TNR) == VIDEO_TNR)
#define  gm_IsMNREnabled()     ((gmvd_VideoProcessingEnabled & VIDEO_MNR) == VIDEO_MNR)
#define  gm_IsGNREnabled()     ((gmvd_VideoProcessingEnabled & VIDEO_GNR) == VIDEO_GNR)
#define  gm_IsACCEnabled()     ((gmvd_VideoProcessingEnabled & VIDEO_ACC) == VIDEO_ACC)
#define  gm_IsACMEnabled()     ((gmvd_VideoProcessingEnabled & VIDEO_ACM) == VIDEO_ACM)
#define  gm_IsHEMEnabled()     ((gmvd_VideoProcessingEnabled & VIDEO_HEM) == VIDEO_HEM)
#define  gm_Is33Enabled()      ((gmvd_VideoProcessingEnabled & VIDEO_3_3) == VIDEO_3_3)
#define  gm_Is22Enabled()      ((gmvd_VideoProcessingEnabled & VIDEO_2_2) == VIDEO_2_2)
#define  gm_IsSpatialEnabled()    ((gmvd_VideoProcessingEnabled & VIDEO_SPATIAL) == VIDEO_SPATIAL)
#define  gm_IsDynScalingEnabled() ((gmvd_VideoProcessingEnabled & VIDEO_DYN_SCAL) == VIDEO_DYN_SCAL)
#define  gm_IsVCRTrickModeEnabled() ((gmvd_VideoProcessingEnabled & VIDEO_VCR) == VIDEO_VCR)
#define  gm_IsFilmDetected()      (gm_ReadRegWord(AFM_MODE_CTRL) & AFM_EN)

#if (FEATURE_TNR_PROGRESSIVE == ENABLE)
#define  gm_IsTNRProgressiveEnabled()     ((gmvd_VideoProcessingEnabled & VIDEO_TNR_PROGRESSIVE) == VIDEO_TNR_PROGRESSIVE)
#endif
#if (FEATURE_PIP_DCDI == ENABLE)
#define  gm_IsDCDIPipEnabled()     	((gmvd_VideoProcessingEnabled & VIDEO_DCDI_PIP) == VIDEO_DCDI_PIP)
#endif
#if (FEATURE_VIDEO_BLUESTRETCH == ENABLE)
#define  gm_IsBlueStretchEnabled()  ((gmvd_VideoProcessingEnabled & VIDEO_BLUESTRETCH) == VIDEO_BLUESTRETCH)
#endif
#if (FEATURE_VIDEO_FLESHTONE == ENABLE)
#define  gm_IsFleshToneEnabled()    ((gmvd_VideoProcessingEnabled & VIDEO_FLESHTONE) == VIDEO_FLESHTONE)
#endif
#if (FEATURE_TWO_STAGE_PROCESSING_MODE == ENABLE)
#define  gm_Is2StageEnabled()     	((gmvd_VideoProcessingEnabled & VIDEO_2STAGE) == VIDEO_2STAGE)
#endif
#if (FEATURE_VIDEO_SHARPNOISE == REV2)
#define  gm_IsSharpNoiseEnabled()   ((gmvd_VideoProcessingEnabled & VIDEO_SHARPNOISE) == VIDEO_SHARPNOISE)
#endif

//******************************************************************************
//  P R O T O T Y P E S
//******************************************************************************

//
// Mode Detection
//
BOOL           gm_TimingIsStable(gmt_PHY_CH Channel);
BOOL           gm_InputTimingTransitionToStable(gmt_PHY_CH Channel);
BOOL           gm_InputTimingTransitionFromStable(gmt_PHY_CH Channel);
void           gm_ModeDetectionConfig(gmt_PHY_CH Channel, gmt_DET_MODE Mode);
void           gm_StateDetectionConfig(gmt_PHY_CH Channel, gmt_DET_STATE State);
BYTE           gm_GetExternalDeviceStatus(gmt_PHY_CH Channel);
gmt_DEVICE_ID     gm_GetDeviceId(gmt_PHY_CH Channel);
void           gm_SetDeviceId(gmt_PHY_CH B_Channel, gmt_DEVICE_ID Device);

void           gm_SetSyncTypeOrder(gmt_PHY_CH Channel, gmt_SYNC_TYPE B_S1,
                              gmt_SYNC_TYPE B_S2, gmt_SYNC_TYPE B_S3,
                              gmt_SYNC_TYPE B_S4);
void           gm_EnableSyncType (gmt_PHY_CH B_Channel, gmt_SYNC_TYPE W_SyncType, WORD W_SampleShift);
void           gm_SetAutoMode(gmt_PHY_CH Channel, gmt_AutoMode Mode);
gmt_SYNC_TYPE gm_GetSyncState(gmt_PHY_CH B_Channel);

void gm_ForceModeChanged(gmt_PHY_CH B_Channel);

//
// Mode Setup
//
gmt_RET_PASSFAIL  gm_SourceClockSetup(gmt_PHY_CH B_Channel, WORD W_HTotal);

//
// Auto Adjust
//
gmt_RET_STAT   gm_AutoGeometry(gmt_PHY_CH B_Channel, BYTE B_AutoMode);
BOOL           gm_AutoPosition(gmt_PHY_CH B_InputChannel);
gmt_RET_STAT   gm_AutoPeriodicCall(void);
gmt_RET_STAT   gm_AutoADCInit(gmt_ADC_ID B_AdcId, gmt_PHY_CH B_Channel);

// private functions (TODO: Why Are private functions here?)
WORD           gmp_GrabPixel(gmt_PHY_CH B_Channel, WORD W_Xcord, WORD W_Ycord, gmt_RGB_COLOR B_Color);

//
// Hardware
//
void           gm_HardwareUpdate(void);
void           gm_HardwareSystemInit(void);
void gm_Delay1us(WORD W_Delay);
void gm_TimerInitStd(DWORD D_CpuClock);

//
// gProbe
//
void far gm_Printf(const far char *Bp_String, ...);

//
// Interface function for OCM Watch Dog Timer
// gm_WDT_Init(DWORD appCallBack), to initialize the WDT driver with applciation
//                                 callback function when timeout occured
// gm_WDT_Enable(void), to activiate the WDT driver
// gm_WDT_Reset(void), if WDT is enabled, this function must be call before
//                     the WDT duration exhaust to prevent WDT to kick in
//
// Usage:
//    step 1:  call gm_WDT_Init(&appCallBack) to initialize the WDT
//    setp 2:  call gm_WDT_Enable() to enable the WDT after all initialization
//    setp 3:  periodically call gm_WDT_Reset() to prevent WDT from timeout
//
// See also gmc_WDT_TIMEOUT_DURATION in gm_System.c
typedef enum
{
	WDT_TU_DISABLE		= 0x00, //disable WDT
	WDT_TU_1 			= 0x01, //WDT time = (1<<10)/fr (120MHz - 8.5us)
	WDT_TU_2 			= 0x02, //WDT time = (1<<20)/fr (120MHz - 8.74ms)
	WDT_TU_3 			= 0x04, //WDT time = (1<<21)/fr (120MHz - 17.5ms)
	WDT_TU_4 			= 0x08, //WDT time = (1<<22)/fr (120MHz - 35.0ms)
	WDT_TU_5 			= 0x10, //WDT time = (1<<23)/fr (120MHz - 69.9ms)
	WDT_TU_6 			= 0x20, //WDT time = (1<<24)/fr (120MHz - 140ms)
	WDT_TU_7 			= 0x40, //WDT time = (1<<25)/fr (120MHz - 280ms)
	WDT_TU_8 			= 0x80, //WDT time = (1<<26)/fr (120MHz - 559ms)
} gmt_WDT_TimeUnit;

void gm_WDT_Check_Reset(DWORD appCallBack);
void gm_WDT_Init(DWORD appCallBack, gmt_WDT_TimeUnit timeUnit);
void gm_WDT_Reset (WORD W_Duration);

//
// WSS Mode Detect/Setup
//
void gm_WssEnableMeasure(gmt_PHY_CH B_Channel);
void gm_WssSetup(gmt_PHY_CH B_Channel, Wss_Mode W_Mode);
BOOL gm_WssCheckStatus(gmt_PHY_CH B_Channel, BOOL Available);
WORD gm_WssGetStatus(gmt_PHY_CH B_Channel, WSS_LINE W_Mode);
void gm_MonitorStatus(gmt_PHY_CH B_Channel, Wss_Mode W_Mode, BOOL Valid);
BYTE gm_WssNTSCDecode(WORD W_Data);
BYTE gm_WssPALDecode(WORD W_Data);
BYTE gm_WssDecode(WORD W_Data);

#if 0
//
// VCR/VTD Mode
//
void gm_SetupVTDBlock(BYTE VTD_Mode);
gmt_VCR_MODES  gm_GetVcrMode(gmt_PHY_CH B_Channel);
#endif

//
// Monitor input static image
//
gmt_RET_STAT gm_StaticImageDetectConfig(gmt_PHY_CH B_Channel, WORD W_Timeout,
                              DWORD D_Threshold);
BOOL gm_StaticImageDetectStatus(gmt_PHY_CH B_Channel);
void gm_StaticImageDetectHandle(void);

//
// Enhancer
//
void gm_AdaptiveGlobalMotion(BYTE MADI_MODE,BYTE TNR_MODE,BYTE CCS_MODE,
                     BYTE Noise_Monitor,BYTE Motion_Value_Monitor,
                     BYTE Motion_Region_Monitor);
// wrappers of gm_AdaptiveGlobalMotion() for separate TNR, MADI, CCS setup
gmt_RET_PASSFAIL gm_MADISetup( BYTE B_MADIMode );
#if 0 // (FEATURE_CCS == ENABLE)
gmt_RET_PASSFAIL gm_CCSSetup( BYTE B_CCSMode );
#endif
#if (FEATURE_VIDEO_SHARPNOISE == REV2)
gmt_RET_PASSFAIL gm_SharpNoiseCoringSetup( BYTE B_SharpNoiseMode );
#endif
WORD gmp_AdjustThreshouldByAfmWindow(WORD W_Threshold);
void gm_SetAdjusterValue(WORD W_AdjusterId, WORD W_Value);
void gm_SetLumaMeasureWindow(void);

//
// Auto Free Run
//
void gm_AfrRestart(gmt_PHY_CH B_Channel);

//
// Letterbox detection
//
gmt_RET_STAT gm_LetterBoxDetectInit(gmt_PHY_CH B_Channel,gmt_LB_MODE W_Mode);
gmt_RET_STAT gm_LetterBoxDetectSetup (gmt_PHY_CH B_Channel, gmt_LB_WINDOW *St_Param);
gmt_RET_STAT gm_LetterBoxDetectTop (gmt_PHY_CH B_Channel,WORD *W_FirstActiveLine,
									WORD *W_LastActiveLine);
gmt_RET_STAT gm_LetterBoxDetectLine (gmt_PHY_CH B_Channel,
									 WORD *W_FirstActiveLine,
									 WORD *W_LastActiveLine);

//
// Input source info
//
BOOL gm_IsInputDecoder (gmt_PHY_CH B_Channel);
BOOL gm_IsInputTripleADC (gmt_PHY_CH B_Channel);
BOOL gm_IsInputDIP (gmt_PHY_CH B_Channel);
BOOL gm_IsInputHdmiDvi (gmt_PHY_CH B_Channel);
BOOL gm_IsInputDP (gmt_PHY_CH B_Channel);
BOOL gm_IsInputDigital (gmt_PHY_CH B_Channel);
gmt_MODE_TYPE gm_GetCurrentInputModeType (gmt_PHY_CH B_Channel);

//
// Flag Line Interrupt Enable/Disable
void far gm_SetFlagLineProcess(gmt_PHY_CH B_Channel, BYTE B_Enable);

// Gm_RegAccessMapper.c
BYTE gm_ReadRegByteDP12Rx(gmt_DPRX_DID b_DID, WORD w_RegAddr);
WORD gm_ReadRegWordDP12Rx(gmt_DPRX_DID b_DID, WORD w_RegAddr);
void gm_WriteRegByteDP12Rx(gmt_DPRX_DID b_DID, WORD w_RegAddr, BYTE b_RegValue);
void gm_WriteRegWordDP12Rx(gmt_DPRX_DID b_DID, WORD w_RegAddr, WORD w_RegValue);
void gm_ClearRegBitsByteDP12Rx(gmt_DPRX_DID b_DID, WORD w_RegAddr, BYTE b_Clear);
void gm_ClearRegBitsWordDP12Rx(gmt_DPRX_DID b_DID, WORD w_RegAddr, WORD w_Clear);
void gm_SetRegBitsByteDP12Rx(gmt_DPRX_DID b_DID, WORD w_RegAddr, BYTE b_Set);
void gm_SetRegBitsWordDP12Rx(gmt_DPRX_DID b_DID, WORD w_RegAddr, WORD w_Set);
void gm_ClearAndSetBitsByteDP12Rx(gmt_DPRX_DID b_DID, WORD w_RegAddr, BYTE b_Clear, BYTE b_Set);
void gm_ClearAndSetBitsWordDP12Rx(gmt_DPRX_DID b_DID, WORD w_RegAddr, WORD w_Clear, WORD w_Set);

void gm_InitialDdsSetup(gm_AVDdsSrcTypes_t AvDdsSrc);
void gm_DDSOpenLoopForce(gm_AVDdsSrcTypes_t AvDdsSrc, DWORD ulDdsInitFreq);
BOOL gm_DDSCloseLoopForce(gm_AVDdsSrcTypes_t AvDdsSrc, DWORD ulDdsInitFreq);
BOOL gm_DDSInitFreqSet(gm_AVDdsSrcTypes_t AvDdsSrc, DWORD ulDdsInitFreq);
DWORD gm_DDSCurrentFreqGet(gm_AVDdsSrcTypes_t AvDdsSrc);
WORD gm_DDSTrackingErrorGet(gm_AVDdsSrcTypes_t AvDdsSrc);
void gm_DDSFreqDeltaSet(gm_AVDdsSrcTypes_t AvDdsSrc, WORD w_FreqDleta);
BOOL gm_AVDDSWaitStable(gm_AVDdsSrcTypes_t AvDdsSrc);



#endif
//*********************************  END  **************************************

