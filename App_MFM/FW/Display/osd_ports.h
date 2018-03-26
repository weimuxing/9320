/*
	$Workfile:   osd_ports.h  $
	$Revision: 1.7 $
	$Date: 2012/07/17 09:18:30 $
*/
#ifndef __OSD_PORTS_H__
#define __OSD_PORTS_H__
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
// MODULE:      osd_ports.h
//
// USAGE:
//
//******************************************************************************

//******************************************************************************
//  T Y P E    D E F I N I T I O N S
//******************************************************************************
typedef enum
{
   SCART_AUTOMATIC,
   SCART_COMPOSITE,
   SCART_SVIDEO,
   SCART_RGB_OVERLAY,
   SCART_RGB,
   MAX_SCART_CONFIGS
}Scart_Configs;

#if 1//defined(Qisda_93XX_BOARD) //KevinL 20120430 DP Switch Control 0 : MINI-DP, 1 : NORMAL-DP
typedef enum
{
  DP_MINI,
  DP_NORMAL,  
}DP_Port_Types;
#endif //defined(Qisda_93XX_BOARD)

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************

//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************

//******************************************************************************
//  E X T E R N A L    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

void AdjustCurrentInputMain(void);
void AdjustCurrentInputPip(void);
BOOL IsVgaOnScreen(void);
BOOL IsInputVGA(gmt_PHY_CH B_Channel);
BOOL IsInputDigital(BYTE B_Channel);
gmt_UserDefConnNames GetNextMainInputPort(gmt_UserDefConnNames B_CurrentPort);
gmt_UserDefConnNames GetPrevMainInputPort(gmt_UserDefConnNames B_CurrentPort);
void InputPortSelectionChannelDeterm (BYTE InputPortSelection);
void InputPortListScroll(void);
gmt_UserDefConnNames GetNextPipInputPort(gmt_UserDefConnNames B_CurrentPort);
gmt_UserDefConnNames GetPrevPipInputPort(gmt_UserDefConnNames B_CurrentPort);
gmt_UserDefConnNames GetPrevSCARTPort(gmt_UserDefConnNames B_CurrentPort);
BOOL IsSystemPal(void);
BOOL IsMultiSystem(void);

#ifdef PIP_PORT_INDEPENDENCE
void DisablePipModeDetection(void);
void ChoosePipSetupMethod(void);
#endif
#if 1//defined(Qisda_93XX_BOARD) //KevinL 20120430 DP Switch Control
void Input_DP_Switch_Select(DP_Port_Types B_DP_Type);
#endif //defined(Qisda_93XX_BOARD)
#endif __OSD_PORTS_H__
//*********************************  END  **************************************

