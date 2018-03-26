/*
$Workfile:   gm_InputConfig.h  $
$Revision: 1.25 $
$Date: 2013/01/08 05:54:57 $
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
// MODULE :gm_InputConfig.h
//
// USAGE  :This module contains Tables and defines used for Configuration of
//         Front-End (AFE+DFE).
//
// NOTE   :This file is generated from the InputConfig tool ver.2.1 on,
//         Wed Apr 30 14:13:56 2008

//*******************************************************************************

//******************************************************************************
//  IMPORTANT : P L E A S E   D O   N O T   E D I T  T H I S  F I L E
//******************************************************************************

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************

#ifndef __GM_INPUTCONFIG_H__
#define __GM_INPUTCONFIG_H__
#include "System\all.h"

//Master list of all input ports spanning all projects
typedef enum
{
   ALL_BOARDS_CVBS1,       // 0
   ALL_BOARDS_SVIDEO1,     // 1
   ALL_BOARDS_COMP1,       // 2
   ALL_BOARDS_VGA1,        // 3
   ALL_BOARDS_DVI1,        // 4
   ALL_BOARDS_DVI2,        // 5
   ALL_BOARDS_DIP1,        // 6
   ALL_BOARDS_HDMI,        // 7
   ALL_BOARDS_HDMI2,       // 8
   ALL_BOARDS_HDMI3,       // 9
   ALL_BOARDS_DP1,         // 10
   ALL_BOARDS_DP2,         // 11
	ALL_BOARDS_DP3,         //12
	
	ALL_BOARDS_COMP2, 		// 13
	ALL_BOARDS_CVBS2, 		// 14
	
   NOT_USED,               // 13
   ALL_BOARDS_SLAVE1,      // 14
   NUM_MASTER_LIST,        // 15
} ALL_BOARDS_INPUT_PORT;

#ifdef Athena_C // 20110720, Athena-C no support DDVI
#define DUAL_DVI_CONNECTOR_ENABLE	0
#else // Athena_U
#ifdef DVI_SINGLE_LINK
#define DUAL_DVI_CONNECTOR_ENABLE	0
#else
#define DUAL_DVI_CONNECTOR_ENABLE	1
#endif
#endif
 
#define NUM_OF_CONNECTORS		NUM_MASTER_LIST
#define NUM_OF_AFE_REGISTERS		4
#define NUM_OF_ANALOG_INP		1
#define NUM_OF_DIGITAL_INP		0
#define NUM_OF_HDMIDVI_INP		2


// 20100519, for port scan
#ifdef AUTO_SCAN_INPUT
#define AUTOSCANINPUT_NUM_RINGS 1 // 4 //is equal to number of rings in ASInputRings array (not enums!)
//AUTOSCANINPUT_RINGS_SIZE define should be explicitly declared, because of compiler specifics
//it is equal to sizeof(ASInputRings).
#define AUTOSCANINPUT_RINGS_SIZE 5 // 31
extern /*ALL_BOARDS_INPUT_PORT*/BYTE ROM ASInputRings[AUTOSCANINPUT_RINGS_SIZE];
#endif


//User-Defined Connector Names
typedef enum USER_DEF_CONN_NAMES
{
	VGA1 = ALL_BOARDS_VGA1,
	DP1 = ALL_BOARDS_DP1,
	DVI1 = ALL_BOARDS_DVI1,
	HDMI1 = ALL_BOARDS_HDMI,
   	DIP1 = ALL_BOARDS_DIP1,
	#ifdef MUX_TEST_BOARD
	DP3 = ALL_BOARDS_DP3,
	#endif
	COMP1 = ALL_BOARDS_COMP1,
	#ifdef EXTERNAL_DECODER
	COMP2 = ALL_BOARDS_COMP2,
	CVBS1 = ALL_BOARDS_CVBS1,
	CVBS2 = ALL_BOARDS_CVBS2,
	#endif
}gmt_UserDefConnNames; //End of Use Defined Connector Names

//******************************************************************************
// E X T E R N A L     A F E     T A B L E S
//******************************************************************************

extern gmt_InputConfigTable ROM gmvs_InputConfigTbl[NUM_MASTER_LIST];

extern WORD ROM gmvw_AFERegAddress[NUM_OF_AFE_REGISTERS];
extern BYTE ROM gmvb_AFERegistersVals[NUM_MASTER_LIST*NUM_OF_AFE_REGISTERS];
extern BYTE ROM gmvb_AdcUsage[];

void GpioSetAnalogExtMux(WORD Connection);
#endif // __GM_INPUTCONFIG_H__
//*********************************  END  **************************************
