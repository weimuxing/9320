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
// MODULE:      anx9021.h
//
//******************************************************************************

#ifndef __ANX9021_H__
#define __ANX9021_H__

#ifndef __SYSTEM_H__
#include "System\System.h"
#endif


//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************

#define ANX9021_I2C_ADDR			0x60	// The ANX9021 device address.  Can only be 0x60 or 0x62.
#define ANX9021_I2C_AUDIO_ADDR		0x68	// audio module address. Can only be 0x68 or 0x6a.
#define ANX9021_I2C_PORT		0

//Register defines
#define VID 		0x00
#define DID			0x02
#define SWRST		0x05
#define SSTATUS		0x06
#define SCTRL1		0x08
#define PORTSEL		0x09
#define BCAP_SET	0x2E
#define BSTATUS		0x30
#define VID_AOF		0x5F


typedef enum
{
   gmd_HDMI_RGB,
   gmd_HDMI_YUV422_SD,
   gmd_HDMI_YUV422_HD,
   gmd_HDMI_YUV444_SD,
   gmd_HDMI_YUV444_HD,
} gmt_HDMI_SIG_FORMAT;

//Audio register defines

//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

BOOL ANX9021_IsSignalStable(void);
void ANX9021_PowerUp(BOOL Bt_State);
void ANX9021_EnablePorts(BYTE B_PortNum, BOOL Bt_State);
BOOL ANX9021_IsSignalHDMI(void);
void ANX9021_Init(void);
BOOL ANX9021_IsConnected(void);
gmt_HDMI_SIG_FORMAT ANX9021_GetSignalFormat(void);
gmt_HDMI_SIG_FORMAT ANX9021_GetBoardOutputFormat(void);
void ANX9021_SetBoardOutputFormat(BYTE B_mode);


//*********************************  END  **************************************
#endif
