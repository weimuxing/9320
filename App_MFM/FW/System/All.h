/*
	$Workfile:   All.h  $
	$Revision: 1.46 $
	$Date: 2013/01/18 05:48:14 $
*/
#ifndef __ALL_H__
#define __ALL_H__
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
// MODULE:      all.h
//
// USAGE:       Header file for all include files requires and
//				hardware specific definition.
//
//******************************************************************************

//******************************************************************************
//	C O M P I L E R    O P T I O N S
//******************************************************************************

//******************************************************************************
//  G L O B A L    I N C L U D E	F I L E
//******************************************************************************
//
// Header files specific to Standard Library
//
#include "StdLibInc\gm_Register.h"
#include "StdLibInc\gm_ChipFeature.h"
#include "StdLibInc\gm_StdLib.h"
#include "StdLibInc\gm_OsdLib.h"
#include "StdLibInc\gm_OsdControl.h"
#include "StdLibInc\gm_OsdGlobalVariables.h"	// OSD GLobals
#include "StdLibInc\gm_GlobalVariables.h"		// Driver Globals
#include "StdLibInc\gm_InputConfigDefs.h"
#include "StdLibInc\gm_Input.h"
#include "StdLibInc\gm_Timer.h"
#include "StdLibInc\gm_Clocks.h"
#include "StdLibInc\gm_ModeSetup.h"
//#include "StdLibInc\gm_Ir.h"
#include "StdLibInc\gm_Decoder.h"
#include "StdLibInc\gm_SetupHTotal.h"
#include "StdLibApps\gm_System.h"				// StdLib - extern for gm_system.c
//
// Header files specific to application project
//
#include "System\System.h"				// System specific - top of system files
#include "CustomAPP\ProjConfig.h"				// System specific - top of system files
#if (defined(BOARD_93XX_SUV))
#include "CustomAPP\ProjConfig_SUV.h"
#elif (defined(BOARD_93XX_4K2K))
#include "CustomAPP\ProjConfig_4K2K.h"
#elif (defined(BOARD_93XX_MNT))
#include "CustomAPP\ProjConfig_MNT.h"
#endif

#include "Board\panel.h"
#include "ZDTLibInc\ZDTLib.h"

#if (FEATURE_CEC == ENABLE)
#include "StdLibInc\CecLib.h"
#endif
#include "CEC\CECFeatures.h"

#include "stdlibinc\3D.h"
#include "stdlibinc\gm_AudioAPI.h"

#ifdef ST_4K2K_93xx_BOARD
#include "OsdFiles_4k2k\osd_uset.h"		// Need to be front for UserSetting
#include "OsdFiles_4k2k\osd_cdat.h"		// Need to be here for OSD COMMAND SRAM
#include "OsdFiles_4k2k\Osd_ctype.h"
#else
#include "OsdFiles\osd_uset.h"		// Need to be front for UserSetting
#include "OsdFiles\osd_cdat.h"		// Need to be here for OSD COMMAND SRAM
#include "OsdFiles\Osd_ctype.h"
#endif

#include "Main\init.h"					//
#include "Main\version.h"				//
#include "TestHarness\TestServices\DebugHandler.h"			// Debug Handler
#include "TestHarness\TestServices\SQA_DebugHandler.h"		// SQA Debug Handler
#include "Main\AppGlobalVariables.h"	// Application Globals
//#include "Board\RegBlock.h"				// Application register blocks
#include "CommLink\gProbe.h"			//
#if ((FEATURE_SOCIPC == ENABLE) || defined(FEATURE_HOSTIF))
#include "CommLink\SocIPC.h"			//Enable SOC IPC Communication functions	
#endif
#include "CommLink\ddc2bi.h"			// Ddc2bi protocol
#include "CommLink\smt.h"
#include "CommLink\Msg_Fwd.h"
//#include "Board\panel.h"		//hyb130503- move up
#include "Board\DisplayPortConfig.h"		//
#include "Board\gm_InputConfig.h"		//
#include "Board\BoardControl.h"			// board specific configurations
#include "Board\gm_GetPortAttr.h"		//
#include "System\Input.h"				//
#include "System\ModeHandler.h"			//
#include "System\ModeDeterm.h"			// Mode Determination
#include "System\ModePrefs.h"			// Mode Preferences
#include "System\Memory.h"				// Memory allocation
#include "System\Sdram.h"
#include "System\ModeSetup.h"			// Mode Setup
#include "System\PowerHandler.h"
#include "System\MultiPipHandler.h"
#include "System\MultiDisplay.h"		// Multi-display window size calculation routines
#include "System\FilmMode.h"			// Film Mode Handler and support routines
#include "System\Scheduler.h"			// Scheduler routines
#include "System\ir.h"
#include "System\AdcKeypad.h"
#include "System\timers.h"
#include "System\wdt.h"
#include "System\WssHandler.h"
#include "Board\Osd_AppDependent.h"
#include "Board\mode.h"
#include "Display\osd_adj.h"
#include "Display\osd_input.h"
#include "Display\osd_util.h"
#include "Display\osd_display.h"
#include "Display\osd_audio.h"
#include "Display\osd_pip.h"
#include "Display\osd_timer.h"
#include "Display\osd_ports.h"
#include "Display\osd_inputCapture.h"
#include "Display\osd_factory.h"
#include "Display\osd_draw.h"
#include "Board\AutoScanInput.h"

#include "Adjusters\adjustor.h"
#include "Adjusters\adj_inputcapture.h"
#include "Adjusters\adj_display.h"
#include "Adjusters\multi_pip.h"
#include "Adjusters\ScalerDynamic.h"
#include "Adjusters\adj_AdcCalibration.h"
//#include "Adjusters\adj_ACM2.h"
#include "Adjusters\adj_ACM3D.h"
#include "Adjusters\adj_ColorWrap.h"
#ifdef USE_COLOR_FIDELITY
// 20100209 #1, Added Color Fidelity
#include "ColorFidelity\dynamicgamutmapping.h"
#endif

#ifdef ST_4K2K_93xx_BOARD
    #include "OsdFiles_4k2k\dynfonts.h"
    #include "OsdFiles_4k2k\osd_act.h"
    #include "OsdFiles_4k2k\osd_clpt.h"
    #include "OsdFiles_4k2k\osd_ctype.h"			// For custom draw controls
    #include "OsdFiles_4k2k\osd_dat.h"
    #include "OsdFiles_4k2k\osd_glph.h"
    #include "OsdFiles_4k2k\osd_trtbl.h"
    #include "OsdFiles_4k2k\osd_uadj.h"
#else
//    #include "OsdFiles\osdSplash.h"
#include "OsdFiles\dynfonts.h"
#include "OsdFiles\osd_act.h"
#include "OsdFiles\osd_clpt.h"
#include "OsdFiles\osd_ctype.h"			// For custom draw controls
#include "OsdFiles\osd_dat.h"
#include "OsdFiles\osd_glph.h"
#include "OsdFiles\osd_trtbl.h"
#include "OsdFiles\osd_uadj.h"
#endif
#include "CustomAPP\CustomAPP.h"		//Application header

#if 0//(defined(BOARD_93XX_SUV)||(defined(BOARD_93XX_4K2K)))
#include "..\..\NHLib\CustomApp\Custom_App.h"
#endif

#if (defined(BOARD_93XX_4K2K))
#include "ZDTLibInc\Custom_4K2K.h"
#endif

#if(WB_STATE_MACHINE_VERSION >= 0x0004)	// WB4
	#include "StdLibInc\gm_SM4.h"
#endif

#include "Device\Vdecoder\viddec.h"
#ifdef VPC3230_DEV_ID
#include "Device\Vdecoder\vpc3230.h"
#endif
#ifdef SAA7119_DEV_ID
#include "Device\Vdecoder\saa7119.h"
#endif
#ifdef VPX322xF_DEV_ID
#include "Device\Vdecoder\vpx32xx.h"
#endif

#ifdef EXTERNAL_DECODER
//#include "Device\Vdecoder\tvp5160.h"
//#include "..\..\NHLib\Decoder\DEV_tvp5160.h"
#include "ZDTLibInc\DEV_tvp5160.h"
#endif
#include "Device\Audio\TDA7491.h"
#ifdef TVP5158_DEV_ID
	#include "CustomAPP\DEV_tvp5158.h"
#endif

#include "Device\Nvram\24LC32.h"

#ifdef DRIVE_INA3221//owen 121030 ina3221
#include "Device\driverboard\ina3221.h"
#endif

#ifdef ANX9021_DEV_ID
#include "Device\Hdmi\anx9021.h"
#endif
//
// Must be included after device specify
//
#include "System\Audproc.h"

//
#include "UserPref\UserPref.h"

#ifdef EXTERNAL_SLICER
#include "StdLibReplace\ExternalSlicer.h"
#endif 	//#ifdef EXTERNAL_SLICER

#if defined(CVT_MODES_SUPPORT)
#include "System\cvt.h"
#endif

#if (FEATURE_DISPLAY_PORT == ENABLE)
#include "DP\DP1_2LibDevice\DpLibInterface.h"
#include "DP\DpApp.h"
#include "HDCP\HdcpApp.h"
#endif

#if (FEATURE_DISPLAY_PORT == ENABLE || FEATURE_HDMI == ENABLE)
#include "ComboPhy\ComboPhy.h"
#endif


#if 1 //(FEATURE_HDMI == ENABLE)
#include "HdmiDvi\sthdmirx.h"
#include "HdmiDvi\gm_HdmiInfo.h"
#include "HdmiDvi\HdmiApp.h"
#endif

#if ((FEATURE_DISPLAY_PORT == ENABLE && defined(CFG_DPTX_HDCP_EN)) || (FEATURE_HDMI == ENABLE))
#include "System\sha1.h"
#endif

// 20090730 #1, Implement DP EDID wirte by SW I2C.
#if 1//def SOFT_I2C_FOR_EDID
#include "Device\NVram\soft_I2C.H"
#endif

#include "Device\SpiFlash\flash.h"

// 20090827 #4, Implement SMART_ISP
//#ifdef SMART_ISP
#include "SmartIsp\SmartIsp.h"
//#endif

#include "stdlibinc\gm_LPM_IPC.h"
#include "System\CableHandler.h"

#include "system\VWD.h"

#include "StdLibInc\gm_OsdAdjustEngine.h"

#if 1 // avoid compiler error
// for remove xds_rating.h but used definitions in OSD WB
#define US_TV_RATING_SUBCODE_D  BIT0
#define US_TV_RATING_SUBCODE_L  BIT1
#define US_TV_RATING_SUBCODE_S  BIT2
#define US_TV_RATING_SUBCODE_V  BIT3
#define US_TV_RATING_SUBCODE_FV BIT4
// for remove gm_TtxLib.h but used definitions in OSD WB
#define TTX_LANGUAGE_GROUP_WESTERN_EUROPE	 0
#define TTX_LANGUAGE_GROUP_EASTERN_EUROPE	 1
#define TTX_LANGUAGE_GROUP_RUSSIAN			 2
#define TTX_LANGUAGE_GROUP_ARABIC			 3
#define TTX_LANGUAGE_GROUP_FARSI			 4
#endif

#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif

#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif
#endif
//*********************************  END  **************************************
