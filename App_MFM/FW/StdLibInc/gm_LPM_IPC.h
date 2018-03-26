#ifndef __GM_LPM_IPC_H__
#define __GM_LPM_IPC_H__

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
// MODULE:      gm_LPM_IPC.h
//
// USAGE:
//
//******************************************************************************
//#include ".\LowPowerMonitor\Lpm_System.h"

//Uncomment if Sequoia AA is used
//#define FEATURE_CHIP_REV_AA

#ifdef LPM_SIDE
#define IRQ_STATUS 		LPM_IRQ_STATUS
#define PENDING_WRITE	MISSION_MAIL_IN_READY
#define CONTROL_IN		LPM_MICRO_CONTROL_IN
#define CONTROL_OUT		LPM_MICRO_CONTROL_OUT
#define MAIL_IN			LPM_MICRO_MAIL_IN
#define MAIL_OUT		LPM_MICRO_MAIL_OUT
#else
#define IRQ_STATUS 		MISC_OCM_STATUS
#define PENDING_WRITE	LPM_MAIL_IN_READY
#define CONTROL_IN		MISSION_MICRO_CONTROL_IN
#define CONTROL_OUT		MISSION_MICRO_CONTROL_OUT
#define MAIL_IN			MISSION_MICRO_MAIL_IN
#define MAIL_OUT		MISSION_MICRO_MAIL_OUT
#endif

// TODO: check this type fix with Sequia Ax
// difference between various lpm_ipc.h discovered
typedef enum
{
   gmd_MESSAGE_IR,
   gmd_MESSAGE_CEC,
   gmd_MESSAGE_LBADC,
   gmd_MESSAGE_POWER,
   gmd_MESSAGE_CableStatus,	// 20100802, for Cable status
   gmd_MESSAGE_DPStore,       // for pseudo-LT, DPCD usage
   gmd_MESSAGE_LPM_GPIO_PINOUT,
   gmd_MESSAGE_LPM_GPADC_PINOUT,
   gmd_MESSAGE_DEBUG,
   gmd_MESSAGE_TOUCHKEY,
   gmd_MESSAGE_HOSTBUS,
   gmd_MESSAGE_DDC,
   gmd_MESSAGE_CALIBRATION,
   gmd_MESSAGE_LPM_GPIO_CTRL,
       gmd_MESSAGE_USER,        		
   gmd_MESSAGE_CHECK_STATUS
} gmt_MESSAGE_SOURCE;

// 20100802 , for Cbale status
// 1: Connected; 0: Disconnect;
typedef struct
{
	WORD PHY1_CableStatus:1;
	WORD PHY1_SourcePowerStatus:1;
	WORD PHY2_CableStatus:1;
	WORD PHY2_SourcePowerStatus:1;
	WORD PHY3_CableStatus:1;
	WORD PHY3_SourcePowerStatus:1;
	WORD VGA_CableStatus:1;
	WORD VGA_SourcePowerStatus:1; // Should no use;
	#ifdef MUX_TEST_BOARD
	WORD MiniDP_CableStatus:1;
	WORD MiniDP_SourcePowerStatus:1;
	#endif
	WORD Cables_Reserver:6;
} CableInfo;

typedef enum
{
   PHY1,
   PHY2,
   PHY3,
   PHY12,
   PHY_VGA,
   Reserve
} Cables_PHY;


//typedef BYTE	gmt_MESSAGE_SOURCE;

typedef enum
{
   gmd_IPC_QUEUE_FREE,
   gmd_IPC_QUEUE_LOADING,
   gmd_IPC_QUEUE_COMPLETE
} gmt_QUEUE_STATE;

typedef struct
{
   WORD B_SourceId:6;
   WORD B_State:2;
   WORD date:8;
} gmt_QUEUE_INFO;

typedef struct
{
   gmt_QUEUE_INFO QueueInfo;
   BYTE B_EmptyIndex;
   BYTE far * Bp_Buffer;
} gmt_QUEUE;

typedef struct LPM_PINOUT_STRUCT
{
   WORD LPMGPIO_Addr;		// LPM_GPIO port 1 2
   WORD LPMGPIO_Data;		// Word or Bit
   BYTE	LPMGPIO_Action;		// Set / Clear bit or Read / Write word
   BYTE LPMGPIO_ID;			// Power Key or VGA cable detect or Amber LED etc.
}gmt_LPM_PINOUT_Config;

typedef enum
{
   Clear_bit,
   Set_bit,
   Read_Word,
   Write_Word
} LPM_REG_Action;

typedef enum
{
   PowerKey,
   Maser_I2C,
   Amber_LED,
   ST_TouchSensor,
   Keypad_in1,
   Keypad_in2,
   VGA1_Cable_Detection,
   DVI1_Cable_Detection,
   HDMI1_Cable_Detection,
   VGA2_Cable_Detection,
   DVI2_Cable_Detection,
   HDMI2_Cable_Detection,
   DP1_HPD,
   DP2_HPD,
   HDMI1_HPD,
   HDMI2_HPD,
   DVI1_HPD,
   DVI2_HPD,
   ADC
   // Others
} LPM_GPIO_ID;
void far gm_MessageInIsr(void);
#if (FEATURE_LOW_POWER_MONITOR == ENABLE)
BOOL far gm_MessageSend(gmt_MESSAGE_SOURCE B_SourceId, void far* B_Buffer, WORD W_Length);
WORD far gm_MessageReceived(gmt_MESSAGE_SOURCE B_SourceId, void far* B_Buffer, WORD W_BufferSize);
#else
#define gm_MessageSend(B_SourceId,  B_Buffer, W_Length) (FALSE)
#define gm_MessageReceived(B_SourceId, B_Buffer, W_BufferSize) (0)
#endif
void far gm_MessageInit(gmt_QUEUE far * QueuePtr, BYTE B_NumOfQueues, void far* B_Buffer,
                        WORD W_TotalBufferSize, WORD W_Timeout, WORD (far* GetTimePtr)(void));


void far gm_RegMsgInit(void);
BOOL far gm_RegMsgSd(gmt_MESSAGE_SOURCE B_SourceId, void far* B_Buffer, WORD W_Length);
WORD far gm_RegMsgRv(gmt_MESSAGE_SOURCE B_SourceId, void far * Bp_Buffer);
void far RegMsgHandler(void);


#define REG_MSG_NUM 24
#define REG_MSG_SIZE 20			//REG_MSG_NUM - 4;	//REG_MSG_IN_TX, REG_MSG_IN_RX, REG_MSG_IN_CMD, REG_MSG_IN_LEN;
#define REG_MSG_BUF 64
extern BYTE REG_MSG_CONTENT[REG_MSG_BUF];
extern BYTE REG_MSG_PTR;
extern BYTE REG_MSG_TOP;
extern WORD RX_MONITOR;
extern WORD BACK_TX;
extern WORD BACK_RX;



#ifdef LPM_SIDE
#define REG_MSG_IN 			LPM_MIS_DATA_IN_0
#define REG_MSG_OUT			LPM_MIS_DATA_OUT_0
#define REG_MSG_IN_TX			LPM_MIS_DATA_IN_20
#define REG_MSG_IN_RX			LPM_MIS_DATA_IN_21
#define REG_MSG_IN_CMD			LPM_MIS_DATA_IN_22
#define REG_MSG_IN_LEN			LPM_MIS_DATA_IN_23
#define REG_MSG_IN_START		REG_MSG_IN
#define REG_MSG_OUT_TX			LPM_MIS_DATA_OUT_20
#define REG_MSG_OUT_RX			LPM_MIS_DATA_OUT_21
#define REG_MSG_OUT_CMD			LPM_MIS_DATA_OUT_22
#define REG_MSG_OUT_LEN			LPM_MIS_DATA_OUT_23
#define REG_MSG_OUT_START		REG_MSG_OUT
#else
#define REG_MSG_IN 			MISSION_LPM_DATA_IN_0
#define REG_MSG_OUT			MISSION_LPM_DATA_OUT_0
#define REG_MSG_IN_TX			MISSION_LPM_DATA_IN_20
#define REG_MSG_IN_RX			MISSION_LPM_DATA_IN_21
#define REG_MSG_IN_CMD			MISSION_LPM_DATA_IN_22
#define REG_MSG_IN_LEN			MISSION_LPM_DATA_IN_23
#define REG_MSG_IN_START		REG_MSG_IN
#define REG_MSG_OUT_TX			MISSION_LPM_DATA_OUT_20
#define REG_MSG_OUT_RX			MISSION_LPM_DATA_OUT_21
#define REG_MSG_OUT_CMD			MISSION_LPM_DATA_OUT_22
#define REG_MSG_OUT_LEN			MISSION_LPM_DATA_OUT_23
#define REG_MSG_OUT_START		REG_MSG_OUT
#endif



#endif //__GM_LPM_IPC_H__
