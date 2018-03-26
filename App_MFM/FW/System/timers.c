/*
	$Workfile:   timers.c  $
	$Revision: 1.11 $
	$Date: 2013/04/30 10:00:38 $
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
// MODULE:      timers.c
//
// USAGE:       Initialize the application timers
//
//******************************************************************************
//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "system\all.h"
#include "StdLibInc\gm_Mcu186.h"

//******************************************************************************
//  STATIC   V A R I A B L E S
//******************************************************************************

static BOOL Bt_MonitorCounter1Minute = FALSE;  // This flag is used by Update_Monitor_Power_On_Counter() to increase the
//Monitor Power On time with 1 every 1 minute.
//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************

BYTE B_IRTimeCounter;
//Flag for showing Ir Key had been detected recently
BYTE B_IrKeyDetected;		//BIT0 = "Data available for clean-up"

//
//User timer and Timer array intialization.
//
WORD gWa_UserTmrBuf [USER_END_1s-USER_START_1ms];


// This is the Monitor Power On Time in minutes. Used to indicate the age(working hours) of the Monitor.
extern DWORD DW_MonitorPowerOnTime;

extern gmt_PwrStateType PwrState;
extern WORD	gm_NestedIntCounter;

extern DWORD DW_MonitorCounter;

#if (FEATURE_CEC == ENABLE)
WORD UserExecutionTimeoutMain = 0;
WORD UserExecutionTimeoutPip = 0;
BYTE B_UserCommandExecution = 0;
#endif
//******************************************************************************
//  F U N C T I O N   P R O T O T Y P E S
//******************************************************************************

void interrupt ApplicationTimerIsr1(void);
void UpdateMonitorPowerOnCounter(void);

//******************************************************************************
//  C O D E
//******************************************************************************
//******************************************************************************
//
// FUNCTION       : void UpdateMonitorPowerOnCounter(void)
//
// USAGE          : Updates the Monitor Power On Counter in NVRAM.
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : Bt_MonitorCounter1Minute; DW_MonitorPowerOnTime
//
// USED_REGS      : None
//
// PRE_CONDITION  : None
//
// POST_CONDITION : None
//
//******************************************************************************
void UpdateMonitorPowerOnCounter(void)
{
   if (Bt_MonitorCounter1Minute == TRUE)
   {
      DW_MonitorPowerOnTime += 1;
      WriteMonitorPowerOnTimeintoNVRAM(&DW_MonitorPowerOnTime);
      Bt_MonitorCounter1Minute = FALSE;
   }
}

//******************************************************************************
//
// FUNCTION       : void UserTimerInit(void)
//
// USAGE          : Calls the internal rom function to append User timers to system
//                  timers
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : gWa_UserTmrBuf
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void UserTimerInit(void)
{
   gm_UserTimerInit(gWa_UserTmrBuf,
                    USER_END_1ms - USER_START_1ms, USER_END_10ms-USER_START_10ms,
                    USER_END_100ms-USER_START_100ms, USER_END_1s - USER_START_1s);
}

//******************************************************************************
//
// FUNCTION       : void ApplicationTimer1Init(BYTE B_AppsTimeout)
//
// USAGE          : Initializes MCU timer_1 for application usage (OSD + customer)
//				As a prescaled clock we use timer2 that generate one clock per one ms.
//                  The timer 1 resolution is defined by parameter B_AppsTimeout ms
//
// INPUT          : B_AppsTimeout - number of
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : None
//
//******************************************************************************
void ApplicationTimer1Init(WORD timeout)
{
   WRITE_PCB_REG(T1CMPA, timeout);
   WRITE_PCB_REG(T1CNT, 0); //Clear counter

   // Enable Timer1 with interrupt in continue mode
   WRITE_PCB_REG(T1CON, (EN|INH|INT|P|CONT));
   // Setup Timer1 Interrupt vector
   SET_VECTOR(TIMER1_VECTOR, ApplicationTimerIsr1);
}

#if(WB_MAJOR != 0x0004)
//******************************************************************************
//
// FUNCTION       : void interrupt ApplicationTimerIsr1(void)
//
// USAGE          : Prints system something out.
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : B_IRTimeCounter		(R/W)
//                  B_IrKeyDetected		(R/W)
//                  D_Key				(W)
//                  B_NumBits			(W)
//                  B_ValidIrData		(W)
//
// USED_REGS      : IR_CONFIG			(W)
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void interrupt ApplicationTimerIsr1(void)
{
   asm sti; //allow other high or equal priority ISR
#if ((defined(PORT_ADC_1_USED) || defined(PORT_ADC_2_USED) || defined(PORT_ADC_3_USED)))
   // ADC
   if (ScanNextAdcKey())		// if key state changed
      EnqueueOsdKey(); 			// Enqueue Adc key press for application layer
#endif

#ifdef USER_IR_SUPPORT
#if defined(PORT_IR_USED)
   {	// IR (24ms) SCR#2088
      static BYTE count=0;
      if ( (count = (count+1) % 3) == 0) //call handler every 24ms (= 8ms*3)
         IrHandler();
   }
#endif
#endif

   // Update the Monitor Power On Timer only if the Power Handler is not in Pwr_Off state.
   if (PwrState != Pwr_Off)
   {
      static WORD isr_count = 0;
      // Monitor Power On Time (60000 ms = 1 minute)
      if (++isr_count >= 7500) //call Update_Monitor_Power_On_Counter() every 60000 ms (= 8ms*7500)
      {
         Bt_MonitorCounter1Minute = TRUE;
         isr_count = 0;
      }
   }
   // Clear MaxCount flag and interrupt exit (End of interrupt - EOI)
   CLEAR_PCB_REG_BITS(T1CON, MC);
   asm cli; //to avoid recurrent interrupt between following line and iret
   WRITE_PCB_REG(EOI, TIMER1_VTYPE);
}

#else //#if(WB_MAJOR != 0x0004)

//******************************************************************************
//
// FUNCTION       : void interrupt ApplicationTimerIsr1(void)
//
// USAGE          : Prints system something out.
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : B_IRTimeCounter		(R/W)
//                  B_IrKeyDetected		(R/W)
//                  D_Key				(W)
//                  B_NumBits			(W)
//                  B_ValidIrData		(W)
//
// USED_REGS      : IR_CONFIG			(W)
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void interrupt ApplicationTimerIsr1(void)
{
   asm sti; //allow other high or equal priority ISR


#if (FEATURE_HDMI==ENABLE) && (USE_MINI_ISR_HANDLER==1)
   HdmiDvi_MiniIsrHandler();
#endif

#ifndef LPM_PERMANENTLY_CONTROLS_PERIPHERAL
   // scan channels every 8ms SCR#2088
   {
      BYTE i;
      for (i = 0; i < WB_NUMBER_OF_INPUT_SOURCE; i++)
         ((WBK_KEYPAD_DEFINITION ROM*)WBK_Keypads[i]) -> GetKeyValue(i);
   }
#else
   //when handler is in LPM 24ms is good enough
   {
      static BYTE isr_count;
      if (++isr_count>=WB_NUMBER_OF_INPUT_SOURCE)
         isr_count = 0;
      ((WBK_KEYPAD_DEFINITION ROM*)WBK_Keypads[isr_count]) -> GetKeyValue(isr_count);
   }
#endif





#ifdef BUZZER_USED// Raymond[(2011-08-04):02], add buzzer function
{
        static BYTE BuzzerTimer = 0;	
        //StKam090311   ISR to base buzzer timer, don't use main loop to control buzzer time 
	if (B_BuzzerControlFlag==BuzzerState_3KHz_START)
	{
		gm_WriteRegWord(BUZZER_PWM_CTRL, (BUZZER_PWM_PULSE_3KHz << BUZZER_PWM_PULSE_SHIFT) | BUZZER_PWM_PERIOD_3KHz);	
		BuzzerTimer=25;	
		B_BuzzerControlFlag=BuzzerState_SINGING;	
	}
	if (B_BuzzerControlFlag==BuzzerState_4K7Hz_START)
	{
		gm_WriteRegWord(BUZZER_PWM_CTRL, (BUZZER_PWM_PULSE_4K7Hz << BUZZER_PWM_PULSE_SHIFT) | BUZZER_PWM_PERIOD_4K7Hz);
		//gm_WriteRegWord(MC_SPARE1, gm_ReadRegWord(MC_SPARE1)+1);
		BuzzerTimer=25;	
		B_BuzzerControlFlag=BuzzerState_SINGING;	
	}	

	if (BuzzerTimer)
	{
		BuzzerTimer--;
	}
	else
	{
        //if(B_BuzzerControlFlag != BuzzerState_STOP)//120921 Edward no need this condition to avoid buttom sound play continue issue
		{
            B_BuzzerControlFlag=BuzzerState_STOP;
            gm_WriteRegWord(BUZZER_PWM_CTRL, 0);
        }		
	}
}
#endif










    
   // Update the Monitor Power On Timer only if the Power Handler is not in Pwr_Off state.
   if (PwrState != Pwr_Off)
   {
      // Monitor Power On Time (60000 ms = 1 minute)
      static WORD isr_count = 0;
      if (++isr_count >= 7500) //call Update_Monitor_Power_On_Counter() every 60000 ms (= 8ms*7500)
      {
         Bt_MonitorCounter1Minute = TRUE;
         isr_count = 0;
      }
#if (FEATURE_CEC == ENABLE)
      if (B_UserCommandExecution & BIT0)
      {
         if (UserExecutionTimeoutMain >= 350)
         {
            B_UserCommandExecution &=(BYTE)(~BIT0);
            UserExecutionTimeoutMain = 0;
         }
         else
            UserExecutionTimeoutMain++;
      }
      if (B_UserCommandExecution & BIT1)
      {
         if (UserExecutionTimeoutPip >= 350)
         {
            B_UserCommandExecution &=(BYTE)(~BIT1);
            UserExecutionTimeoutPip = 0;
         }
         else
            UserExecutionTimeoutPip++;
      }
#endif
   }

/*ChengLin 20130221: Image Sync Handle for Athena Left Only*/
#ifdef ST_4K2K_93xx_BOARD 
#if ENABLE_ATHENA_R_L_IMAGE_SYNC_HANDLER
   if (!gmvb_DebugOnly && (PwrState != Pwr_DeepSleep))
   {
	   if(GetChipID_4K2K() == CHIPID_4K2K_L)
	   {
		   if( (ImageSyncHandleGetState() > IMAGE_LVDS_OFF_STATE) && (IS_Image_Ready() == FALSE))
		   {
			   if(gm_ReadRegWord(PB_CTRL) & PB_MAIN_ENABLE)
            { 
				   Image_Force_OnOff(FALSE); //Black Screen
               #if ENABLE_PBP_ATHENA_L_DEEP_SLEEP_CHECK   
               if(UserPrefCaptureM == CAPTURE_MODE_HALF_HORIZONTAL)
                  gm_TimerStart(PBP_SLEEP_CHECK_TIMEOUT_1ms_TMR, PBP_SLEEP_WAIT_TIME_OUT);
               #endif
            }
		   }
	   }
   }
#endif
#endif

   // Clear MaxCount flag and interrupt exit (End of interrupt - EOI)
   CLEAR_PCB_REG_BITS(T1CON, MC);

   asm cli; //to avoid recurrent interrupt between following line and iret
   WRITE_PCB_REG(EOI, TIMER1_VTYPE);
}
#endif	//#if(WB_MAJOR != 0x0004)

/***********************************  END  ************************************/


