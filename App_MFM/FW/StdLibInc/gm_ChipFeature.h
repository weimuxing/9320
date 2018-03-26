/*
   $Workfile:   gm_ChipFeature.h  $
   $Revision: 1.59 $
   $Date: 2012/11/28 03:10:24 $
*/
#ifndef __CHIP_FEATURE_H__
#define __CHIP_FEATURE_H__
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
// MODULE:      gm_ChipFeature.h
//
// USAGE:      This header file is used to define all the chip dependent
//            feature or revision that are used in the Cortez's family
//            code base.
//            This apply to both application and system library
//
//******************************************************************************

//******************************************************************************
//   G L O B A L      D E F I N I T I O N S
//******************************************************************************
#ifndef DISABLE
#define DISABLE 0
#endif
#ifndef ENABLE
#define ENABLE 1
#endif

//
// HW revision
//
#define REV_NONE        0	// HW not exist
#define REV1            1
#define REV2            2
#define REV3            3
#define REV4            4
#define REV5            5
#define REV6            6

//
// for FEATURE_VERTICAL_MOTION_DETECTION
//
#define SW_ENABLE       0	// Use FW method
#define HW_ENABLE       1	// Use HW method

//========================================================
// Special Product
//========================================================
// HUB 
#define FEATURE_DP_HUB DISABLE // Athena HUB board 

//========================================================
// Video Process
//========================================================
// CCS // Cross Color Supression
#define FEATURE_CCS     DISABLE // Athena does not support CCS

// Adaptive Contrast Ratio
#define FEATURE_ACR                          ENABLE

// TNR mode support
#define FEATURE_TNR               REV1 // REV3 // REV1 for TNR; REV3 for TNR3

// TNR progressive mode support
#define FEATURE_TNR_PROGRESSIVE              ENABLE

// Video Sharp Noise support
#define FEATURE_VIDEO_SHARPNOISE             REV2

// Main DCDI support Rev-2
#define FEATURE_MAIN_DCDI                    REV2

// For some chips there is not be enough memory bandwidth
// to keep motion detection while AFM (filmmode) is disable.
#define FEATURE_MOTION_DETECTION_OFF         ENABLE

// ACM3D support
#define FEATURE_ACM_3D                       ENABLE

// ACC feature
#define FEATURE_ACC3                         ENABLE
//#define FEATURE_ACC2                DISABLE
#define FEATURE_ACC                 DISABLE
#define FEATURE_ACC_RAM_SIZE        10

// MPEG_NR support version
#define FEATURE_MPEG_NR                      REV2

// Blue stretch support
// Fleshtone support
#define FEATURE_VIDEO_BLUESTRETCH            DISABLE
#define FEATURE_VIDEO_FLESHTONE              DISABLE

// Cross point switch support
#define FEATURE_CROSS_POINT_SWITCH           ENABLE

// Vertical motion detection support - SW
// Vertical motion detection support - HW
#define FEATURE_VERTICAL_MOTION_DETECTION    HW_ENABLE

// Luma feature
#define FEATURE_LUMA                REV2


// Overdrive
#define FEATURE_OVERDRIVE           ENABLE
#define FEATURE_NOISE_CORING        ENABLE


// Diplay Uniformity Module (DUM) support
#define FEATURE_DUM                 ENABLE

#define FEATURE_VLUT_SIZE           10

// Dither support
#define FEATURE_DITHER              ENABLE

#define FEATURE_LUMA_CLAMP          DISABLE // ENABLE // Athena has full range support

// Input capture feature - 1080P
#define FEATURE_INPUT_CAPTURE_1080P    ENABLE

#define FEATURE_FREEZE_VIDEO           ENABLE

//
// Sharpness feature
//
// Macro to define revisons of Scaler sharpness block in a Cortez Family.
// The settings for Enhancer block (HEM) are also under this macro.
//
// REV1 - FLI8532. Vertical video filters were ported from Malibu.
//        Horizontal video video filters are in HW ROM tables.
//        There are no sharpness for PIP and RGB.
//
// REV2 - Cortez Plus. Vertical video filters were ported from Malibu.
//        Horizontal video video filters were ported from Malibu.
//        There are no sharpness for PIP and RGB.
//
// REV3 - FLI8668. New 10-bit separate HPF and LPF filters.
//        Software loadable Vertical/Horizontal Video/RGB CH_A/CH_B filters.
//
// NOTE: REV4 de facto has no diffrenece to REV3. To be reviewed.
#define FEATURE_SHARPNESS           REV6

// Download RGB scaler coefficient support
#define FEATURE_LOAD_RGB_COEFF      ENABLE

// Panoramic scaling
#define FEATURE_PANORAMIC           ENABLE

// Note:  These revisions refer to a "prevent false 22 mode
//        by monitoring vertical motion" software patch.
//
// REV1 - It was the patch implemented for Fli8668 and Fli8538 in FLI8xDrv.lib.
//
// REV2 - This revision refers to the FLI30436 which has a
//        patch inside IROM. The difference between REV1 and REV2
//        is there is no need to redefine function pointer to the patch.
//
// REV3 - The revision is for Fli8532 chip which has no patch at all
#define FEATURE_AFP22DETECTVM_PATCH             REV2


//========================================================
// Memory
//========================================================
#define FEATURE_DDR_EXECUTION                   ENABLE

// MC Data Timing Configuration
#define FEATURE_MC_TIMING_CONFIGURATION         REV3


//========================================================
// Misc
//========================================================
#define FEATURE_SERIAL_FLASH              ENABLE

// ODP Pixel grab function for appstest
#define FEATURE_ODP_PIXEL_GRAB            ENABLE

// 2nd 2Wire hardware master
#define FEATURE_2WIRE_NUM                 2
#define FEATURE_BLENDER_FAILSAFE          REV2
#define FEATURE_IMP_LOCKUP_ISSUE_FIX      REV_NONE

// CLK HW version
#define FEATURE_CLK_HW                    REV2
#define FEATURE_IFMC                      ENABLE
#define FEATURE_IFMC_FIELD_SEL            ENABLE

// PIP special feature support
#define FEATURE_PIP_DCDI                     ENABLE
#define FEATURE_PIP_HF_BLOCK                 ENABLE
#define FEATURE_PIP_RGB_COLORSPACE           ENABLE
#define FEATURE_TWO_STAGE_PROCESSING_MODE    ENABLE

// Macro to define revisons of internal VBI Slicer in a Cortez Family
// for VBI driver implementation.
//
// REV1 - It was the first revision implemented in Fli8532
//
// REV2 - This revision refer to the slicers implemented in Fli8668.
//        There are two types of slicers in Fli8668: 'OLD' or 'NEW' type.
//        The OLD type was an minor improved version of Fli8532 slicers;
//        The NEW type was a totaly new design implementatation that was
//        only available in Fli8668.
//        In general, both type of slicers may be enabled simultaneously
//        enabled in the chip.
//
// REV3 - This was an enhanced version of Fli8532 type slicer implemented
//        in Fli8538 or in Fli30436
//
// Notes:
//    1.  By enabling REV2, it refer to BOTH 'Old' and 'New' slicers in
//        a Cortez chip, which dedicated to Fli8668 as it is the only
//        chip that has all four slicers
//    2.  In previous CHIP_ID implementation, the CHIP_ID was used to
//        determine if a chip has two slicers; in the FEATURE
//        implementation, the FEATURE_DECODER will be used to determine
//        how many slicer is available in the chip (assume slicer always
//        comes with the decoder)
//
#define FEATURE_VBI_SLICER                REV3

#define FEATURE_WSS                       DISABLE		//Neil120712-
#define FEATURE_WSS_BLOCK                 REV5

// Interrupt service interface revision.
// Interrupt Status and Mask registers are not hardware binded with apropriate IRQ anymore
// started from Sequoia. 
#define FEATURE_INTERRUPT_SERVICE         REV2

// GProbe UART communication support
// Identical and independent support of both UART and independent buffer for transmission
// in second revision
#define FEATURE_GPROBE_UART               REV2

// Low Power Monitor support
#define FEATURE_LOW_POWER_MONITOR         ENABLE


//========================================================
// ADC feature
//========================================================
// AIP
#define FEATURE_AIP                    ENABLE

// Glitch filter support
#define FEATURE_GLITCH_FILTER          ENABLE

#define FEATURE_ADC_AOC                ENABLE

// Instant Auto support
#define FEATURE_INSTANT_AUTO           ENABLE
#define FEATURE_AA_ENHANCED            ENABLE


//========================================================
// DIP HW Revision
//========================================================
#define FEATURE_DIP                    REV2


//========================================================
// HDMI
//========================================================
#define FEATURE_HDMI                   ENABLE
// CEC support
#define FEATURE_CEC                    DISABLE


//========================================================
// Display Port
//========================================================
#define FEATURE_DISPLAY_PORT           ENABLE


//========================================================
// Pixel Cruncher Application
//========================================================
// Pixel Cruncher
#define FEATURE_PIXEL_CRUNCH           ENABLE
// Video Window Detection
#define FEATURE_VWD                    ENABLE

#if (FEATURE_DISPLAY_PORT == ENABLE)
#define FEATURE_HDCP_REPEATER          ENABLE
#else
#define FEATURE_HDCP_REPEATER          DISABLE
#endif
//========================================================
// 
//========================================================
   #define FEATURE_3D		       DISABLE

#define FEATURE_AUDIO                  ENABLE


//******************************************************************************
//  C O M P I L E R     U S E     O N L Y
//******************************************************************************
#if (FEATURE_DP_HUB == ENABLE)
#undef FEATURE_ACR
#undef FEATURE_TNR_PROGRESSIVE
#undef FEATURE_MOTION_DETECTION_OFF
#undef FEATURE_ACM_3D
#undef FEATURE_ACC3
#undef FEATURE_CROSS_POINT_SWITCH
#undef FEATURE_OVERDRIVE
#undef FEATURE_NOISE_CORING
#undef FEATURE_DUM
#undef FEATURE_DITHER
#undef FEATURE_LUMA_CLAMP
#undef FEATURE_FREEZE_VIDEO
#undef FEATURE_LOAD_RGB_COEFF
#undef FEATURE_PANORAMIC
#undef FEATURE_DDR_EXECUTION
#undef FEATURE_ODP_PIXEL_GRAB
#undef FEATURE_PIP_DCDI
#undef FEATURE_PIP_HF_BLOCK
#undef FEATURE_PIP_RGB_COLORSPACE
#undef FEATURE_TWO_STAGE_PROCESSING_MODE
#undef FEATURE_WSS
#undef FEATURE_LOW_POWER_MONITOR
#undef FEATURE_AIP
#undef FEATURE_HDMI
#undef FEATURE_CEC
#undef FEATURE_PIXEL_CRUNCH
#undef FEATURE_VWD

#define FEATURE_ACR                       DISABLE
#define FEATURE_TNR_PROGRESSIVE           DISABLE
#define FEATURE_MOTION_DETECTION_OFF      DISABLE
#define FEATURE_ACM_3D                    DISABLE
#define FEATURE_ACC3                      DISABLE
#define FEATURE_CROSS_POINT_SWITCH        DISABLE
#define FEATURE_OVERDRIVE                 DISABLE
#define FEATURE_NOISE_CORING              DISABLE
#define FEATURE_DUM                       DISABLE
#define FEATURE_DITHER                    DISABLE
#define FEATURE_LUMA_CLAMP                DISABLE
#define FEATURE_FREEZE_VIDEO              DISABLE
#define FEATURE_LOAD_RGB_COEFF            DISABLE
#define FEATURE_PANORAMIC                 DISABLE
#define FEATURE_DDR_EXECUTION             DISABLE
#define FEATURE_ODP_PIXEL_GRAB            DISABLE
#define FEATURE_PIP_DCDI                  DISABLE
#define FEATURE_PIP_HF_BLOCK              DISABLE
#define FEATURE_PIP_RGB_COLORSPACE        DISABLE
#define FEATURE_TWO_STAGE_PROCESSING_MODE DISABLE
#define FEATURE_WSS                       DISABLE
#define FEATURE_LOW_POWER_MONITOR         DISABLE
#define FEATURE_AIP                       DISABLE
#define FEATURE_HDMI                      DISABLE
#define FEATURE_CEC                       DISABLE
#define FEATURE_PIXEL_CRUNCH              DISABLE
#define FEATURE_VWD                       DISABLE
#endif

#endif
//*********************************  END  **************************************

