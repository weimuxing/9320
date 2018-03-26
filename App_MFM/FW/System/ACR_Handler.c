/*
	$Workfile:   ACR_Handler.c  $
	$Revision: 1.22 $
	$Date: 2013/04/23 07:44:00 $
*/

//*****************************************************************************************
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
//
//		---
// 		ACR functioms 20100526
// 		Danymic Contrast ratio
//		 Default: Get Luminance data from Pixel grab.
// 		Second solution: Use ACC, if pixel grab used for VWD.
// 		Limitation: If system can not use ACC and pixel grab already used for VWD, it will no ACR anymore.
//
//*****************************************************************************************

//
//******************************************************************************
//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#include "System\All.h"
#include "StdLibInc\gm_Register.h"
#include "System\ACR_Handler.h"
#if (FEATURE_ACC3 == ENABLE)
   #include "StdLibInc\gm_ACC3.h"
#endif
#if (FEATURE_PIXEL_CRUNCH == ENABLE)
//#include "stdlibinc\PIXCRMain.h"
#endif
//******************************************************************************
//  D E F I N E S
//******************************************************************************
// Full White:   25923
// Middle White: 13188
// Full Black:   0
#define ACR_LEVEL_1_THRESHOLD       5000
#define ACR_LEVEL_2_THRESHOLD       10300
#define ACR_LEVEL_3_THRESHOLD       16000
#define ACR_LEVEL_4_THRESHOLD       20000

#define _DEBUG
//#define _DEBUG_EXT

#ifdef _DEBUG
#define  msg(a,b)       gm_Print(a,b)
#else
#define  msg(a,b)
#endif

#ifdef _DEBUG_EXT
#define  msg_i(a,b)     gm_Print(a,b)
#else
#define  msg_i(a,b)
#endif

// Full White:   25923
// Middle White: 13188
// Full Black:   0
#define ACR_PWM_TARGET_0      0
#define ACR_LEVEL_1_THRESHOLD    5000
#define ACR_PWM_TARGET_1      30
#define ACR_LEVEL_2_THRESHOLD    10300
#define ACR_PWM_TARGET_2      70
#define ACR_LEVEL_3_THRESHOLD    16000
#define ACR_PWM_TARGET_3      90
#define ACR_LEVEL_4_THRESHOLD    20000
#define ACR_PWM_TARGET_4      100

#define ACR_EXECUTE_INTERVAL     100   // Unit: ms
#define ACR_PWM_ADJ_INTERVAL     20    // Unit: ms

#define ACR_LUM_DIFFERENCE       5
#define ACR_BACKLIGHT_STEP       1     // Backlight transition step in percentage
#define ACR_ACTION_LAG           5     // Unit: 100ms


#if 0
//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************
BYTE CurrentBrightness = 0;

BOOL IsEnterHiDCRStatus = gmd_FALSE;

//******************************************************************************
//  C O D E
//******************************************************************************

//#ifdef DCR_READ_BRIGHTNESS_USE_ACC3
static HighDcrActivationProcedureType HighDcrActivationProcedure = HighDCR_STRANDBY;

static WORD gm_ACC3GetBrightness(void)
{
	WORD Hist[NUMBER_OF_BINS];
	BYTE i;
	DWORD SumHist = 0, HistBrightness = 0;
 
	for (i = 0; i < NUMBER_OF_BINS; i++) 
	{
		Hist[i] = gm_ACC3GetHistogram(i) >> 9; // ACC2 12 bit,  ACC3 21 bit
		SumHist += Hist[i];
		HistBrightness = HistBrightness + Hist[i] * i;
	}
	SumHist /= NUMBER_OF_BINS;

	if (SumHist > 0)
		HistBrightness /= SumHist;

	return (WORD)HistBrightness;
}
//#endif

static WORD ReadInputAverageBrigttness(void)
{
#ifdef DCR_READ_BRIGHTNESS_USE_ACC3
	return (gm_ACC3GetBrightness());
#else
	return (gm_ReadRegWord(IMP_BRIGHTNESS));
#endif
}

//
// U2410 High DCR - Activation Criteria
//
// DCR for Gaming and Movie Modes only
// DCR On/Off from OSD menu (default is on)
//	When DCR is On:
//	2 step DCR Activation Procedure
//		Step 1: Normal DCR turn on immediately 
//			1. DCR level(CR=tbd) at panel specified min Lamp Current
//		Step 2: High DCR - Activation Criteria
//			1. Activated when gray level < L4 for 10 seconds
//			2. High DCR continuous activation limit to 1 minute
//			3. High DCR activation only after the panel warm up for 10 minutes
//

static void HighDcrActivationCheck(void)
{
	if (ReadInputAverageBrigttness() < HIGH_DCR_GRAY_LEVEL_UNDER)
	{
		switch (HighDcrActivationProcedure)
		{
			case HighDCR_STRANDBY:
				gm_TimerStart(HIGH_DCR_ACTIVATION_1s_TMR, HIGH_DCR_ACTIVATION_TIME);
				HighDcrActivationProcedure = HighDCR_ACTIVATION;

			case HighDCR_ACTIVATION:	// 1. Activated when gray level < L4 for 10 seconds
				if (IS_DCR_ON() && (gm_TimerCheck(HIGH_DCR_PANEL_WARM_UP_1s_TMR) == TIMER_TMO) && 
					(gm_TimerCheck(HIGH_DCR_ACTIVATION_1s_TMR) == TIMER_TMO))
				{
						gm_TimerStart(HIGH_DCR_ACTIVATION_1s_TMR, HIGH_DCR_CONTINUOUS_LIMIT_TIME);
						HighDcrActivationProcedure = HighDCR_CONTINUOUS;
						gm_Print("Enter HIGH DCR state", 0);
				}
				break;

			case HighDCR_CONTINUOUS:	// 2. High DCR continuous activation limit to 1 minute
				if (gm_TimerCheck(HIGH_DCR_ACTIVATION_1s_TMR) == TIMER_TMO)
				{
					gm_TimerStop(HIGH_DCR_ACTIVATION_1s_TMR);
					HighDcrActivationProcedure = HighDCR_LIMIT;
					gm_Print("Exit HIGH DCR state for continuous 1 minut limit", 0);
				}
				break;
		}
	}
	else
	{
		gm_TimerStop(HIGH_DCR_ACTIVATION_1s_TMR);
		HighDcrActivationProcedure = HighDCR_STRANDBY;
	}
}


static void HighDcrProcedureForPanelOnOff(BOOL On_Off)
{
	if (On_Off == On)
		gm_TimerStart(HIGH_DCR_PANEL_WARM_UP_1s_TMR, HIGH_DCR_PANEL_WARM_UP_TIME);
	else
	{
		gm_TimerStop(HIGH_DCR_ACTIVATION_1s_TMR);
		gm_TimerStop(HIGH_DCR_PANEL_WARM_UP_1s_TMR);
	}
	HighDcrActivationProcedure = HighDCR_STRANDBY;
}


#define DCR_CHECK_TIME	18	// 180ms / 10ms
void ACR_Handler(void)
{
   if (IsEnterHiDCRStatus)//calo add for the factory command to test high DCR status
   {
      if ((gm_TimerCheck(HIGH_DCR_TEST_1s_TMR) == TIMER_TMO) || (gm_TimerCheck(HIGH_DCR_TEST_1s_TMR) == TIMER_STOP))
      { 
         IsEnterHiDCRStatus = FALSE;
         gm_TimerStop(HIGH_DCR_TEST_1s_TMR);
         SetBacklightPwnPulse(CurrentBrightness);
      }
      else
         SetBacklightPwnPulse(DCR_HIGH_PWM_PULSE_MIN);
   }
   else
   {
      HighDcrActivationCheck();

      //if (IS_DCR_ON() && (gm_TimerCheck(DCR_CHECK_10ms_TMR) == TIMER_TMO))
      if ((gm_TimerCheck(DCR_CHECK_10ms_TMR) == TIMER_TMO))
      {
         WORD W_Imp_Brightness;
         BYTE B_PwmPulse;

         gm_TimerStart(DCR_CHECK_10ms_TMR, DCR_CHECK_TIME);

         // Solution 1: Pixel Grab
         // TBD
         // Solution 2: ACC Histogram
         W_Imp_Brightness = ReadInputAverageBrigttness();

         B_PwmPulse = ReadBacklightPwnPulse();
         if (B_PwmPulse > DCR_PWM_PULSE_MAX)
         {
            SetBacklightPwnPulse(DCR_PWM_PULSE_MAX);
            B_PwmPulse = DCR_PWM_PULSE_MAX;
         }
         else if (B_PwmPulse < DCR_PWM_PULSE_MIN)
         {
            SetBacklightPwnPulse(DCR_PWM_PULSE_MIN);
            B_PwmPulse = DCR_PWM_PULSE_MIN;
         }

         if ((abs(ConvertImpBrightnessToPwmPulse(W_Imp_Brightness) - B_PwmPulse) > DCR_NOISE_THRESHOLD) || 
             ((ConvertImpBrightnessToPwmPulse(W_Imp_Brightness) != B_PwmPulse) && 
             ((ConvertImpBrightnessToPwmPulse(W_Imp_Brightness) > (DCR_PWM_PULSE_MAX - DCR_NOISE_THRESHOLD)) || 
             (ConvertImpBrightnessToPwmPulse(W_Imp_Brightness) < (DCR_PWM_PULSE_MIN + DCR_NOISE_THRESHOLD)))))
         {
            gm_TimerStart(DELAY_1ms_TMR, 100);
            // clear VBLANK edge from previous frame
            gm_WriteRegWord(MAIN_INPUT_STATUS, IMP_VACTIVE);
            while ((gm_ReadRegWord(MAIN_INPUT_STATUS) & IMP_VACTIVE) == 0)
            {
               if (gm_TimerCheck(DELAY_1ms_TMR) == TIMER_TMO)
                  break;	// timeout - return with error
            }

            gm_Print("ACR PwmPulse ......... %d", B_PwmPulse);

            if (B_PwmPulse < ConvertImpBrightnessToPwmPulse(W_Imp_Brightness))
               SetBacklightPwnPulse(B_PwmPulse + 1);
            else if (B_PwmPulse > ConvertImpBrightnessToPwmPulse(W_Imp_Brightness))
               SetBacklightPwnPulse(B_PwmPulse - 1);
         }
      }
   }
}

#endif

#if (FEATURE_ACR == ENABLE)

extern BOOL gm_IsAfrSet(gmt_PHY_CH Channel);

//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************
static BOOL ACR_ENABLE = FALSE;
static WORD PrevAvgINTXY = INVALID_RESULT;
static BYTE PrevACRPwmTarget = 0xFF;
static BYTE CurACRPwmTarget = 0xFF;
static BYTE AdjustBacklightVal = DEFAULT_BACKLIGHT_INIT;

//******************************************************************************
//  C O D E
//******************************************************************************
WORD GetCurrentAcrResult(void)
{
   return PrevAvgINTXY;
}

//******************************************************************************
// USAGE:   IsACREnabled
// INPUT:   None
// OUTPUT:	BOOL status of ACR
// GLOBALS:
//******************************************************************************
BOOL IsACREnabled(void)
{
   return ACR_ENABLE;
}

//******************************************************************************
// USAGE:   IsPixCrAnalyzeACR
// INPUT:   None
// OUTPUT:	BOOL ACR using pixel cruncher
// GLOBALS:
//******************************************************************************
BOOL IsPixCrAnalyzeACR(void)
{
   if ((UserPrefVwdFlag == OFF) && (gm_ReadRegWord(IMP_CONTROL) & IMP_YUV_EN) == 0)
      return TRUE;
   else
      return FALSE;
}

//******************************************************************************
// USAGE:   ACR_Init
// INPUT:   None
// OUTPUT:	None
// GLOBALS:
//******************************************************************************
void ACR_Init(void)
{
   AdjustBacklightVal = DEFAULT_BACKLIGHT_INIT;
	gm_TimerStop(ACR_ADJUST_100ms_TMR);//20110815 Add by KevinChen.
	gm_TimerStop(DCR_CHECK_10ms_TMR);//20110815 Add by KevinChen.
}

//******************************************************************************
// USAGE:   ACR_Enable
// INPUT:   None
// OUTPUT:	None
// GLOBALS:
//******************************************************************************
void ACR_Enable(void)
{
   msg_i("ACR Enable",0);
   ACR_ENABLE = TRUE;
   if ((gm_IsACCEnabled() == FALSE) && IsPixCrAnalyzeACR() == FALSE)
   {
      // Enable ACC for average brightness
    #ifndef DELL_U2410F
      UserPrefImageScheme = VIVID;  // Test only. Default scheme should be used.
    #endif
      AdjustImageScheme();
   }
}

//******************************************************************************
// USAGE:   ACR_Disable
// INPUT:   None
// OUTPUT:	None
// GLOBALS:
//******************************************************************************
void ACR_Disable(void)
{
   msg_i("ACR Disable",0);
#if (FEATURE_PIXEL_CRUNCH == ENABLE)
   PixCrAnalyzeACRStop();
#endif
   ACR_ENABLE = FALSE;
   PrevAvgINTXY = INVALID_RESULT;
   PrevACRPwmTarget = 0xFF;
   AdjustBacklightVal = DEFAULT_BACKLIGHT_INIT;
   AdjustPanelBacklight(AdjustBacklightVal);
}

//******************************************************************************
// USAGE:   ACR_Running
// INPUT:   None
// OUTPUT:	None
// GLOBALS:
//******************************************************************************
void ACR_Running(void)
{
#if (FEATURE_PIXEL_CRUNCH == ENABLE)
   // ACR only can be worked once the VWD in OSD is disabled.
   if (IsPixCrAnalyzeACR())
   {
      PixCrAnalyzeImageForACR();
   }
#endif   
}

//******************************************************************************
// USAGE:   ACR_GetLumData
// INPUT:   None
// OUTPUT:	WORD Luminance data
// GLOBALS:
//******************************************************************************
#ifdef ST_4K2K_93xx_BOARD
WORD W_PrevLeftDcrResult = INVALID_RESULT;
#endif
WORD ACR_GetLumData(void)
{
#if (FEATURE_PIXEL_CRUNCH == ENABLE)
   // ***** Return value *****
   // Full White:   23428 ~ 25923
   // Middle White: 13102 ~ 13188
   // Full Black:   0
   if (IsPixCrAnalyzeACR())
   {
    #ifdef ST_4K2K_93xx_BOARD
      if (GetChipID_4K2K() == CHIPID_4K2K_R)
      {
         if (SocIPCGetDcrResult(SOCIPC_DEVICEL_DST_ADDRESS))
         {
            if (gAthena_LEFT_FE_Slave_Parameters.DcrResult != INVALID_RESULT)
               W_PrevLeftDcrResult = gAthena_LEFT_FE_Slave_Parameters.DcrResult;

            return ((W_PrevLeftDcrResult + (WORD)(GetAvgINTXY() >> 3)) / 2);
         }
         else
         {
            msg(" Error: Read Left DCR data!",0);
            return INVALID_RESULT;
         }
      }
    #endif
      return (WORD)(GetAvgINTXY() >> 3);
   }
   else
#endif 
   {	 
   	 #ifndef DELL_U2410F //original standard code 
#if 1 // workaround for ACC_AVG_BRIGHTNESS overflow    
   	   #define ACC3_ACCMEAS_HISTOGRAM_0_MEM_START 0x32E00L // from 0x32E00 ~ 32EFF,  256 bytes, 64x22, 
      DWORD W_AvgBrightness = 0;
      DWORD DW_TotalPixel = 0;
      union
      {
         DWORD  dw;
         BYTE  b[4];
      } DWordVal;
      BYTE i;

      for(i = 0; i < 64; i++)
      {
         DWORD DW_TempAddr = (DWORD)ACC3_ACCMEAS_HISTOGRAM_0_MEM_START + i * 4;         
         
         DWordVal.b[0] = *(LINEAR_TO_FPTR_B(DW_TempAddr));
         DW_TempAddr++;
         DWordVal.b[1] = *(LINEAR_TO_FPTR_B(DW_TempAddr));
         DW_TempAddr++;
         DWordVal.b[2] = *(LINEAR_TO_FPTR_B(DW_TempAddr));
         DW_TempAddr++;
         DWordVal.b[3] = *(LINEAR_TO_FPTR_B(DW_TempAddr));

         DW_TotalPixel += DWordVal.dw;
         W_AvgBrightness += (DWordVal.dw * i); // total brightness
      }
      
      W_AvgBrightness = W_AvgBrightness/(DW_TotalPixel/512); // avg brightness, Max : 512 * 58 = 29696
      #else
      WORD W_AvgBrightness = (gm_ReadRegWord(ACC_AVG_BRIGHTNESS) & 0xFFFF) / 2;

      if (W_AvgBrightness > 2000)
         W_AvgBrightness -= 2000;
      else
         W_AvgBrightness = 0;
      #endif

      return (WORD)W_AvgBrightness;
	  
	 #else //120711 Edward from AE suggest to double check the ACC high bin data first
	 
          WORD W_AvgBrightness;
	  DWORD DWa_HistBuff;	 
      
      DWa_HistBuff =  (DWORD)(*(LINEAR_TO_FPTR_B(ACCHighBinAddr)));  
      DWa_HistBuff |= ((DWORD)(*(LINEAR_TO_FPTR_B(ACCHighBinAddr+1)))) << 8;  
      DWa_HistBuff |= ((DWORD)(*(LINEAR_TO_FPTR_B(ACCHighBinAddr+2)))) << 16;	

	  if(DWa_HistBuff > ACCHistogramLimit)
	  {
	  	//gm_Print("ACC over limit",0);
	  	return (WORD)(0x8000);	
	  }	
	  else
	  	{
	  		W_AvgBrightness = (gm_ReadRegWord(ACC_AVG_BRIGHTNESS) & 0xFFFF) / 2;
			
			if (W_AvgBrightness > 2000)
         		W_AvgBrightness -= 2000;
      		else
         		W_AvgBrightness = 0;

      		return W_AvgBrightness;
	  	}
	#endif  
   }
}

//******************************************************************************
// USAGE:   ACR_Handler
// INPUT:   None
// OUTPUT:	None
// GLOBALS:
//******************************************************************************
void ACR_Handler(void)
{
   static AcrHandlerState_t AcrHandlerState = ACR_NO_ACTION;
   static DWORD LastAcrExecTime = 0;
   WORD W_AvgLumData;

#ifdef ST_4K2K_93xx_BOARD
   if (GetChipID_4K2K() == CHIPID_4K2K_FE)
      return;
#endif
   if ((UserPrefDCR == OFF) || (!IsACREnabled()) || (gm_IsAfrSet(gmvb_MainChannel)))
      return;   

   if ((gm_GetSystemTime() - LastAcrExecTime) > ACR_EXECUTE_INTERVAL)
   {
      ACR_Running();
      W_AvgLumData = ACR_GetLumData();

      if ((abs(W_AvgLumData - PrevAvgINTXY) > ACR_LUM_DIFFERENCE) && (W_AvgLumData != INVALID_RESULT))
      {
         PrevAvgINTXY = W_AvgLumData;
         msg_i("Current Lum = %d",PrevAvgINTXY);
        #ifdef ST_4K2K_93xx_BOARD
         if (GetChipID_4K2K() == CHIPID_4K2K_L)
         {
            return;
         }
         else
        #endif
            AcrHandlerState = ACR_CALCULATE_PWM;
      }
   
      LastAcrExecTime = gm_GetSystemTime();
   }
   
   switch (AcrHandlerState)
   {
      case ACR_CALCULATE_PWM:
         if (W_AvgLumData <= ACR_LEVEL_1_THRESHOLD)
            CurACRPwmTarget = ACR_PWM_TARGET_0;
         else if (W_AvgLumData <= ACR_LEVEL_2_THRESHOLD)
            CurACRPwmTarget = ACR_PWM_TARGET_1;
         else if (W_AvgLumData <= ACR_LEVEL_3_THRESHOLD)
            CurACRPwmTarget = ACR_PWM_TARGET_2;
         else if (W_AvgLumData <= ACR_LEVEL_4_THRESHOLD)
            CurACRPwmTarget = ACR_PWM_TARGET_3;
         else
            CurACRPwmTarget = ACR_PWM_TARGET_4;

         if ((CurACRPwmTarget != PrevACRPwmTarget) || (AdjustBacklightVal != CurACRPwmTarget))
         {
            PrevACRPwmTarget = CurACRPwmTarget;
            msg(" ACR: Current Lum = %d",PrevAvgINTXY);
            msg("  ACR: New BL Target = %d",CurACRPwmTarget);
            msg_i("    AdjustBacklightVal = %d",AdjustBacklightVal);
            gm_TimerStart(ACR_ADJUST_100ms_TMR, ACR_ACTION_LAG);
            AcrHandlerState = ACR_ADJUST_PWM;
         }
         else
            AcrHandlerState = ACR_NO_ACTION;
         break;
         
      case ACR_ADJUST_PWM:
         if (gm_TimerCheck(ACR_ADJUST_100ms_TMR) != TIMER_OK)
         {
            static DWORD LastPwmAdjustTime = 0;
            if ((gm_GetSystemTime() - LastPwmAdjustTime) < ACR_PWM_ADJ_INTERVAL)
               break;
            
            if (AdjustBacklightVal != CurACRPwmTarget)
            {
               if (CurACRPwmTarget > AdjustBacklightVal)
                  AdjustBacklightVal = ((AdjustBacklightVal + ACR_BACKLIGHT_STEP) < CurACRPwmTarget)?(AdjustBacklightVal + ACR_BACKLIGHT_STEP):CurACRPwmTarget;
               else
               {
                  if ((AdjustBacklightVal - CurACRPwmTarget) <= ACR_BACKLIGHT_STEP)
                     AdjustBacklightVal = CurACRPwmTarget;
                  else
                     AdjustBacklightVal = ((AdjustBacklightVal - ACR_BACKLIGHT_STEP) > CurACRPwmTarget)?(AdjustBacklightVal - ACR_BACKLIGHT_STEP):CurACRPwmTarget;
               }
               AdjustPanelBacklight(AdjustBacklightVal);
            }
            else
            {
               msg_i("ACR: Backlight Adj Done!",0);
               AcrHandlerState = ACR_NO_ACTION;
            }
            LastPwmAdjustTime = gm_GetSystemTime();
         }
         break;
   }
}

#endif // FEATURE_ACR
