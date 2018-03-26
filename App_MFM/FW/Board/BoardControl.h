/*
	$Workfile:   BoardControl.h  $
	$Revision: 1.13 $
	$Date: 2013/05/23 09:41:34 $
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
// MODULE:      BoardControl.h
//
// USAGE:       Header file for BoardSpecific.c initialization.
//
//******************************************************************************

#ifndef __BOARDCONTROL_H__
#define __BOARDCONTROL_H__

   typedef enum
   {
      CHIPID_4K2K_NONE,
      CHIPID_4K2K_L,
      CHIPID_4K2K_FE,
      CHIPID_4K2K_R   
   } gmt_4k2k_ChipID;

   typedef enum
 {
   BOARD_EV1,
   BOARD_EV2,
   BOARD_RD1,
   BOARD_RD1_REV_B,
   BOARD_RD2,
   BOARD_EV3,
   BOARD_RD3,
   BOARD_4K2K,
   BOARD_TK,
   BOARD_TK_RD1,
   BOARD_CJ1_RD1,
   BOARD_CJ1_RD3,
   BOARD_CJ2,
   BOARD_CJ3,	//Hyb130317+	
   BOARD_NUM,
} gmt_BoardType;

   typedef enum
   {
      BEHAVIOR_ID_UNKNOWN,
      BEHAVIOR_ID_93xx_EV1,
      BEHAVIOR_ID_93xx_EV2,
      BEHAVIOR_ID_93xx_EV3,
      BEHAVIOR_ID_93xx_RD1,
      BEHAVIOR_ID_93xx_RD1_REV_B,
      BEHAVIOR_ID_93xx_RD2,
      BEHAVIOR_ID_93xx_RD3,
      BEHAVIOR_ID_93xx_ST_4K2K_FE,
      BEHAVIOR_ID_93xx_ST_4K2K_R,
      BEHAVIOR_ID_93xx_ST_4K2K_L,
   } gmt_BEHAVIOR_ID;

//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

void InitSystemGpio(void);
void I2C_Init(void);
void GpioSoftReset(WORD W_Delay);
void GpioLed1Keypad(BOOL B_State);
void GpioLed2Keypad(BOOL B_State);
void GpioLedFilmMode1Init(void);
void GpioLedFilmMode1(BYTE B_State);
void GpioLedFilmMode0(BYTE B_State);
void ForceDpHpd2Low(gmt_DPRX_DID b_DID, BOOL b_Set2Low);
BYTE GetCurrentBoardInfo(void);
BYTE GetIrPowerKeyVal(BYTE KeyItem);
gmt_BEHAVIOR_ID Get_Behavior_ID(void);

//void	GpioHdmiInitHotPlug(gmt_UserDefConnNames Connection);
//void	GpioHdmiSetHPDPinToLow(gmt_UserDefConnNames B_InputConnector);
//void far GpioHdmiSetHPDPinToHigh(gmt_UserDefConnNames B_InputConnector);
//BOOL 	GpioHdmiGetHPDPinStatus(gmt_UserDefConnNames B_InputConnector);
#ifdef ST_4K2K_93xx_BOARD
void ChipIDInit_4K2K(void);
BYTE GetChipID_4K2K(void);
void LPM_Set_DCFlag(BOOL B_State);	// 20130204
#elif defined(RD2_4K2K_DEMO)
void ChipIDInit_4K2K(void);
BYTE GetChipID_4K2K(void);
BYTE GetHdmiInputNumber(void);
#endif

//*********************************  END  **************************************
#endif //__BOARDCONTROL_H__
