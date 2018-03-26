/*
	$Workfile:   ADCKeypad.c  $
	$Revision: 1.24 $
	$Date: 2013/05/06 03:21:14 $
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
//
//		---
//
//		MODULE:		ADCKeypad.c
//
//		USAGE:		ADC Keypad related functions
//
//******************************************************************************
//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#include "system\all.h"
#include <math.h>
#if (FEATURE_LOW_POWER_MONITOR == ENABLE)
#include "StdLibInc\gm_LPM_IPC.h"
#endif

#define DEBUG_MSG 	1   //0
#if DEBUG_MSG
#define msg(a,b) gm_Print(a,b)
#define msg_x(a,b,c) gm_Print(a,b,c)
#else
#define msg(a,b)
#define msg_x(a,b,c)
#endif

#define ADC_KBD_NOISE_FILTER	0x10  // 1//wei 140702  for keypad print value is 0    0x10	// Up to 0.2 volts is keypad noise.

enum
{
   ADC_KEY1,
   ADC_KEY2,
   ADC_KEY3,
   MAX_ADC_KEY
};

enum
{
#ifdef PORT_GPIO_USED
   GPIO_KEY,
#endif
#ifdef PORT_ADC_1_USED
   ADC1_KEY,
#endif
#ifdef PORT_ADC_2_USED
   ADC2_KEY,
#endif
#ifdef PORT_ADC_3_USED
   ADC3_KEY,
#endif
#ifdef PORT_IR_USED
   IR_KEY,
#endif
   MAX_KEY_TYPE
};

/*ChengLin 20130315: Avoid OSD Menu Showing  during Splash Screen*/
#ifdef ST_4K2K_93xx_BOARD
extern BOOL AllowKeypadAction;
#if ENABLE_PBP_ATHENA_L_DEEP_SLEEP_CHECK               
extern BOOL AthenaRIgnoreSleep_PBP;
#endif
#endif

extern void GpioLed1Keypad(BOOL B_Set);
extern void GpioLed2Keypad(BOOL B_Set);

#if (defined(PORT_ADC_1_USED) || defined(PORT_ADC_2_USED) || defined(PORT_ADC_3_USED) \
    ||(defined(WBK_FUNCTION_GETADCVALUE_USED) && !defined(LPM_PERMANENTLY_CONTROLS_PERIPHERAL)))
BYTE B_AdcKeyData[NUM_KEYPAD_CH];
BYTE B_AdcData[NUM_KEYPAD_CH];
#endif

#ifdef WBK_FUNCTION_GETADCVALUE_USED		// WB4
static BYTE B_AdcChanNum = 0;
static BYTE B_AdcDriversIndex[NUM_KEYPAD_CH];
#endif

static struct
{
   gmt_LED_COLOR	LED_Color;
   gmt_LED_ATTRIB	LED_Attrib;
   gmt_LED_STATUS	LED_Status;
} LEDStruct;
extern BOOL DDCPowerNapFlag;

#if (defined(PORT_ADC_1_USED) | defined(PORT_ADC_2_USED) | defined(PORT_ADC_3_USED))

//******************************************************************************
//
// FUNCTION       : static KEY_INDEX TranslateAdcToKey (BYTE B_KeyPort, BYTE B_Value)
//
// USAGE          : Compares ADC value with table code by the method binary tree
//
// INPUT          : B_KeyPort - ADC key port number
//                     (ADC_KEY1, ADC_KEY2, ADC_KEY3)
//                  B_Value
//
// OUTPUT         : KEY_INDEX - id pushed key
//                  If none of the key is pressed, return Key_None
//
// GLOBALS        : NUMBER_OF_ADC_1Values (RO) - number of members in array ADC_1Value
//                  ADC_1Values 			   (RO)- a struct to map ADC code to id key.
//                  NUMBER_OF_ADC_2Values (RO) - number of members in array ADC_2Value
//                  ADC_1Values 			   (RO)- a struct to map ADC code to id key.
//                  NUMBER_OF_ADC_3Values (RO) - number of members in array ADC_3Value
//                  ADC_1Values 			   (RO)- a struct to map ADC code to id key.
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
static KEY_INDEX TranslateAdcToKey (BYTE B_KeyPort, BYTE B_Value)
{
   SBYTE  SB_Result;
   SWORD  B_Offset, B_Max, B_Min = 0;
   ST_ADC ROM	*ADC_Values;

#ifdef PORT_ADC_1_USED
   if (B_KeyPort == ADC_KEY1)
   {
      B_Max = NUMBER_OF_ADC_1Values - 1;
      ADC_Values = ADC_1Values;
   }
#endif
#ifdef PORT_ADC_2_USED
   if (B_KeyPort == ADC_KEY2)
   {
      B_Max = NUMBER_OF_ADC_2Values - 1;
      ADC_Values = ADC_2Values;
   }
#endif
#ifdef PORT_ADC_3_USED
   if (B_KeyPort == ADC_KEY3)
   {
      B_Max = NUMBER_OF_ADC_3Values - 1;
      ADC_Values = ADC_3Values;
   }
#endif
   if (B_Value > ADC_KBD_NOISE_FILTER)		//is key press present?
   {
      // Do a binary tree search of all the ADC1 values until we find the
      // real key value where our read value falls into the range of and
      // return this real value
      // If B_Min goes past B_Max, then no corresponding code was found so
      // we return Key_None.
      while (B_Min <= B_Max)
      {
         B_Offset = ((WORD)B_Max + B_Min) / 2;
         // Check if the value falls within the range of the current ADC
         // value we are pointing at.
         if (abs(B_Value - ADC_Values[B_Offset].Code) <= ADC_DELTA)
            return(ADC_Values[B_Offset].KeyIndex);
         else if (ADC_Values[B_Offset].Code > B_Value)
            B_Max = B_Offset - 1;
         else
            B_Min = B_Offset + 1;
      }
   }
   return Key_None;
}

//******************************************************************************
//
// FUNCTION       : BYTE ScanNextAdcKey (void)
//
// USAGE          : Scans the next ADC channel.
//
// INPUT          : None
//
// OUTPUT         : TRUE - ADC key available
//                  FALSE - no ADC key available
//
// GLOBALS        : NUMBER_OF_ADC_1Values		(RO)
//                  NUMBER_OF_ADC_2Values		(RO)
//                  NUMBER_OF_ADC_3Values		(RO)
//                  B_AdcData					(R/W)
//                  B_AdcKeyData				(W)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BYTE ScanNextAdcKey (void)
{
   static BYTE B_AdcChannel = 2;
   BYTE B_Temp;

   // select next available ADC channel (one at a time - cycle through the defined ports)
   do
   {
      if (	++B_AdcChannel > 2)
         B_AdcChannel = 0;

      if (	B_AdcChannel == 0 && NUMBER_OF_ADC_1Values ||
            B_AdcChannel == 1 && NUMBER_OF_ADC_2Values ||
            B_AdcChannel == 2 && NUMBER_OF_ADC_3Values)
         break;

   } while (TRUE);

   // program ADC select bits, scan and debounce specified channel
   B_Temp = KeyFilter(B_AdcChannel,gm_ScanAdcKey ((BIT1 << B_AdcChannel) & (BIT2|BIT3)));

   // If difference from emitting value exceeds threshold, then update emitting value
   if (abs((SWORD) B_Temp-B_AdcData[B_AdcChannel]) > ADC_DELTA/2)
   {
      B_AdcData[B_AdcChannel] = B_Temp;
      B_AdcKeyData[0] = TranslateAdcToKey(0,B_AdcData[0]);
      B_AdcKeyData[1] = TranslateAdcToKey(1,B_AdcData[1]);
      B_AdcKeyData[2] = TranslateAdcToKey(2,B_AdcData[2]);
      return TRUE;
   }

   return FALSE;
}
#endif

extern gmt_PwrStateType PwrState;
BOOL IsLockKeypadLightControl(void)
{
   return (PwrState == Pwr_Transition ? TRUE : FALSE);
}
//******************************************************************************
//
// FUNCTION       : void KeypadLightControl(BYTE B_Control)
//
// USAGE          : Controls the keypad LED
//
// INPUT          : B_Control
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void KeypadLightControl(gmt_LED_COLOR B_Color, gmt_LED_ATTRIB B_Attrib)
{
   LEDStruct.LED_Attrib = B_Attrib;

   // Save LED status (active or inactive)
   if (B_Color != KEYPAD_LED_OFF)
      LEDStruct.LED_Status = LED_ON;
   else
      LEDStruct.LED_Status = LED_OFF;

   if(!IsLockKeypadLightControl())
   {
      switch (B_Color)
      {
         case KEYPAD_LED_OFF:
            GpioLed1Keypad(FALSE);
            GpioLed2Keypad(FALSE);
            break;

         case KEYPAD_LED_RED:
			#if 0//((BOARD_USED == BOARD_93xx_CJ2))
            GpioLed1Keypad(FALSE);
            GpioLed2Keypad(FALSE);
			#else
            GpioLed1Keypad(FALSE);
            GpioLed2Keypad(TRUE);
			#endif
            break;

         case KEYPAD_LED_GREEN:
            GpioLed1Keypad(TRUE);
			#ifdef BOARD_93xx_TK
			GpioLed2Keypad(TRUE);
			//#elif((BOARD_USED == BOARD_93xx_CJ2))
			//GpioLed2Keypad(TRUE);
			#else
			GpioLed2Keypad(FALSE);
			#endif
			break;

         case KEYPAD_LED_ORANGE:
			#ifdef BOARD_93xx_TK
			GpioLed1Keypad(TRUE);
			#else
			GpioLed1Keypad(FALSE);
			#endif
            GpioLed2Keypad(TRUE);
            break;

         default:
            break;
      }
   }

   if (B_Attrib == LED_BLINKING)
   {
      gm_TimerStart(BLINKING_LED_TMR, 3); // 300msec

      if (B_Color != KEYPAD_LED_OFF)
         LEDStruct.LED_Color = B_Color;
   }

}
//******************************************************************************
//
// FUNCTION       :
//
// USAGE          :
//
// INPUT          :
//
// OUTPUT         :
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void BlinkingHandler(void)
{
   if ((LEDStruct.LED_Attrib == LED_BLINKING) && (gm_TimerCheck(BLINKING_LED_TMR) == TIMER_TMO))
   {
      if (LEDStruct.LED_Status == LED_OFF)
         KeypadLightControl(LEDStruct.LED_Color, LED_BLINKING);
      else
         KeypadLightControl(KEYPAD_LED_OFF, LED_BLINKING);
   }
}
#ifdef WBK_FUNCTION_GETADCVALUE_USED		// WB4
//******************************************************************************
//
// FUNCTION       : BYTE InitADCDriver( BYTE B_Index )
//
// USAGE          : Initializes ADC driver
//
// INPUT          : B_Index - index of this keypad in WBK_Keypads[].
//
// OUTPUT         : TRUE
//
// GLOBALS        : WBK_Keypads[]
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
#ifdef LPM_PERMANENTLY_CONTROLS_PERIPHERAL
#if 0
#pragma argsused
BYTE InitADCDriver( BYTE B_Index )		// WB4
{
#pragma pack(push)
#pragma pack(1)
   struct
   {
      BYTE timeOut; // =5ms, 5*4channels = 20ms debounce time
      BYTE treshold; // =8
      BYTE numChannels; // =4 by default, 6 - maximum
      BYTE offChannel; //=0xFF - no selected power on button (any key)
      BYTE offValue; //=24 PowerOn key code
   } settings = {5, 8, 4, 0, 24}; // could be taken from WB project.
#pragma pack(pop)
   gm_MessageSend(gmd_MESSAGE_LBADC, &settings, sizeof(settings));
   return TRUE;
}
#else
BYTE InitADCDriver( BYTE B_Index )		// WB4
{
   UNUSED_VAR(B_Index);
   // Set power key definition in GpioControl.c by each board.
   // LPM read table and config GPADC and GPIO from OTP free memory.
   // Return OK to OSD.
   return TRUE;
}
#endif
#else
BYTE InitADCDriver( BYTE B_Index )		// WB4
{
//   B_AdcDriversIndex[B_AdcChanNum] = B_Index;
//   B_AdcChanNum++;
   gm_SetRegBitsWord(LPM_GPADC_GPIO_CONTROL,GPADC_ADC0IN_DIS|GPADC_ADC1IN_DIS|GPADC_ADC2IN_DIS|GPADC_ADC3IN_DIS);
   gm_WriteRegWord(LPM_GPADC_CTRL,0x5AB);
   UNUSED_VAR(B_Index);
   return TRUE;
}

#endif

//******************************************************************************
//
// FUNCTION       : BYTE GetADCValue( BYTE B_Index )
//
// USAGE          : ADC driver function. Scans the ADC channel and translates code
//                  to key value.
//
// INPUT          : B_Index - index of this keypad in WBK_Keypads[].
//
// OUTPUT         : TRUE - ADC key available
//                  FALSE - no ADC key available
//
// GLOBALS        : WBK_Keypads[], B_AdcData[]
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************

#if (FEATURE_SOCIPC == ENABLE)
   #ifdef ST_4K2K_93xx_BOARD 
      #if ENABLE_SOC_IPC_KEYPAD_VIA_QUEUE
      KeypadRecvFormat_t SocIPCKeypadBuf;
      #else //Original
      BOOL g_Keypad_GetValidKeys = FALSE;
      BYTE g_Keypad_Channel = 0x00;
      BYTE g_Keypad_Value = 0x00;
      #endif
   #endif
#endif

#ifdef LPM_PERMANENTLY_CONTROLS_PERIPHERAL

BOOL CheckMenuKey = FALSE;//----Chunbao 20110808 add for menu lock----//
void StartHardLockTimer(void);
typedef struct 
{
    BYTE ch; 
    BYTE val;
} t_LbwAdcKey;

#ifdef BUZZER_USED
void BuzzerProcess(WBK_Keys result)
{
	static BYTE PreviousKey = 0;		//Neil 20081112+ for Buzzer control
    //BOOL bIsMenuKey = FALSE;
    extern gmt_PwrStateType PwrState;
    extern BYTE MenuKeyState;

	if (!UserPrefButtonSoundOff)
	{
        if ( PreviousKey != result )
        {
            PreviousKey = result;
            //gm_Print("BuzzerProcess result = %d",result);

	     if ( Key_None != result && B_SplashComplete)
            {
            	  if (result == Key_KEY_PHYSICAL_POWER && (!MissionPWDown))
            	  {
            	  	//120420 Edward add MissionPWDown to avoid buzzer play twice 
            	  	//gm_Print("MissionPWDown %d",MissionPWDown);
		     		StartBuzzer_3KHz();		
            	  }	
		  		else	
		  		{
		  			//111013 Edward enable key / buzzer only at LED indication
		  			if ((result == Key_KEY_PHYSICAL_PIP) || (MenuKeyState == FIVE_KEYSTATE) || UserPrefFactoryFlag ||
			    	((result != Key_KEY_PHYSICAL_SOURCE)&&(MenuKeyState == FOUR_KEYSTATE)))
					StartBuzzer_4K7Hz();
		  		}			 
            }
        }
	//slove when have four led on.the top key have audio issue. 			 
	if(IsOSDMainMenu() && (result == Key_KEY_PHYSICAL_SOURCE))
		B_BuzzerControlFlag = BuzzerState_STOP;
    }
}
#endif

#define KeyPad_New	
#define Key_THOLD		8
#define WB_Key_Power		29
#if 0//(BOARD_USED == BOARD_93xx_KOIOS)
#define WB_Key_Menu		212
#define WB_Key_Source		146
#else
#define WB_Key_Menu		211
#define WB_Key_Source	147

#endif
#define WB_Key_Up		85
#define WB_Key_Down		193
#define WB_Key_Left		112
#define WB_Key_Right		57
#define WB_Key_Enter		159
//#define WB_Key_Exit		211
#define WB_Key_Auto		59
#define WB_Key_Pxp		175
//===========================================================
#ifdef BOARD_93xx_TK//Neil121101+ for KYQ

#define ADCKey_Power		38
#define ADCKey_Menu		223
#define ADCKey_Source		140
#define ADCKey_Up		86
#define ADCKey_Down		195
#define ADCKey_Left		113
#define ADCKey_Right		57

#define ADCKey_Enter		159
#define ADCKey_Auto		59
#define ADCKey_Pxp		175



#elif((BOARD_USED == BOARD_93xx_CJ2))
#define ADCKey_Power		36
#define ADCKey_Menu		2  //0        // wei 140626   245
#define ADCKey_Source	126    // wei 140626	 135
#define ADCKey_Up		0       // wei 140626    104
#define ADCKey_Down		87     // wei 140626    220
#define ADCKey_Left		176   // wei 140626   135
#define ADCKey_Right		136   // wei 140626   71


#define ADCKey_Enter		159
#define ADCKey_Auto		59
#define ADCKey_Pxp		175
#endif



//============================================================
BYTE GetADCValue( BYTE B_Index )		// WB4
   {
   
#if (FEATURE_SOCIPC == ENABLE)
#ifdef Athena_4K2K_Chip_A 
  #pragma pack(push)
  #pragma pack(1)
  typedef struct
  {
     BYTE ch;
     BYTE val;
  } t_LbwAdcKey;
  #pragma pack(pop)
  t_LbwAdcKey key;
         
  if (!gm_MessageReceived(gmd_MESSAGE_LBADC, &key, sizeof(key)))
  {
     return FALSE;
  }
  else
  {
      msg("GetADCValue, key.ch = %d", key.ch);
      msg("GetADCValue, key.val = %d", key.val);
   
#if ENABLE_SOC_IPC_KEYPAD_VIA_QUEUE
		SocIPCKeypadBuf.ValidKey = TRUE;
		SocIPCKeypadBuf.KeyChannel = key.ch;
		SocIPCKeypadBuf.KeyValue = key.val;
		SocIPCKeypadBufferInsertKey(SocIPCKeypadBuf);
#else //Original
      g_Keypad_Channel = key.ch;
      g_Keypad_Value = key.val;
      g_Keypad_GetValidKeys = TRUE;
#endif
      return TRUE;
  }
#else
      return FALSE;
#endif   
#else //Original
#pragma pack(push)
#pragma pack(1)
      typedef struct
      {
         BYTE ch;
         BYTE val;
      } t_LbwAdcKey;
#pragma pack(pop)
      t_LbwAdcKey key;
      WBK_KEYPAD_DEFINITION ROM* Sp_Keypad;
   
	 // BYTE B_F_Counter;
     // BYTE B_F_Buffer[9];
	//  BYTE B_Factory_Key_Map[] = {160,85,85,193,193,112,57,112,57};
	//  BYTE Enter_Factory_Flag = 0;
	  BYTE i, LB_ADCKey;
	//  BYTE B_F_Counter_Flag;
	#ifdef TOUCH_KEY
    BYTE buff_touchpad,length_adc,length_touchpad,translate_result;
	length_touchpad = gm_MessageReceived(gmd_MESSAGE_TOUCHKEY, &buff_touchpad, sizeof(buff_touchpad));
		//if(!length_touchpad)
		//	return FALSE;
	#endif
	  
      LB_ADCKey =  gm_MessageReceived(gmd_MESSAGE_LBADC, &key, sizeof(key));
         //return FALSE;
	  #ifdef TOUCH_KEY
	  if((!length_touchpad)&&(!LB_ADCKey))	//None of them, return , otherwise processin one of them
	  	return FALSE;
	  #else
	  if(!LB_ADCKey)	//None of them, return , otherwise processin one of them
	  	return FALSE;
	  #endif


	if(LB_ADCKey)
	{
	  
		msg("===Before: GetADCValue, key.ch = %d", key.ch);
		msg("===Before: GetADCValue, key.val = %d", key.val);
			
		#if((BOARD_USED == BOARD_93xx_CJ2))
				//Hyb130724* for new key value;
				//if(key.ch == 1)
				{
					if((key.ch == 0)&&((abs(key.val - ADCKey_Menu) < Key_THOLD)||(abs(key.val - 239) < Key_THOLD)))
				        {
					  key.ch = 1;  //wei 140708
					  key.val = WB_Key_Menu;//menu
					}
					else if((key.ch == 1)&&(abs(key.val - ADCKey_Up) < Key_THOLD))
					{
					  key.ch = 1;
					  key.val = WB_Key_Up;//up
					}
					else if((key.ch == 1)&&((abs(key.val - ADCKey_Down) < Key_THOLD)||(abs(key.val - 223) < Key_THOLD)))
					{
					  key.ch = 1;
					  key.val = WB_Key_Down;//down 	  
					}
					else if((key.ch == 1)&&(abs(key.val - ADCKey_Enter) < Key_THOLD))
					{
					  key.ch = 1;
					  key.val = WB_Key_Enter;//ok
					}
					else if((key.ch == 1)&&(abs(key.val - ADCKey_Left) < Key_THOLD))
					{
					  key.ch = 1;
					  key.val = WB_Key_Left;//left
					}
					else if((key.ch == 1)&&(abs(key.val - ADCKey_Right) < Key_THOLD))
					{
					  key.ch = 1;
					  key.val = WB_Key_Right;//rigt
					}
					else if((key.ch == 0)&&(abs(key.val - ADCKey_Source) < Key_THOLD))
					{
					  key.ch = 0;
					  key.val = WB_Key_Source;//source
					}
				}
			#endif
		msg("===after: GetADCValue, key.ch = %d", key.ch);
		msg("===after: GetADCValue, key.val = %d", key.val);
		#if 1  //wei 140702 test 
		if ((key.ch ==1) && (key.val==0)&&CheckMenuKey)
		{
			EnqueueOsdKey(Key_None);
			#ifdef BUZZER_USED
			BuzzerProcess(Key_None);
			#endif
			#if(OSD_HARD_LOCK == ENABLE)
			//----Chunbao 20110808 add for menu lock----//
			if((CheckMenuKey)&&(gm_TimerCheck(HardLock_1s_TMR) == TIMER_OK))
			{
			gm_Print(">>>>>>>>>>>>>>>>>>>>>>>>HardLock timer Stop ", 0);
			gm_TimerStop(HardLock_1s_TMR);
			SetOsdTimeout();
			CheckMenuKey = FALSE;
			}
			//----menu lock 20110808----//
			#endif
		}
                #endif
				
		#if (FEATURE_3D == ENABLE)
		#if (FEATURE_3D_KEY_HANDLER == ENABLE)
		if (gm_3D_KeyEventHandler((BYTE far *) &key))
		{
			EnqueueOsdKey(Key_None);
			return TRUE;
		}
		#endif
		#endif

		for (B_Index = 0;;)
		{
			Sp_Keypad = WBK_Keypads[B_Index];
			if (Sp_Keypad
			&& (Sp_Keypad->GetKeyValue == GetADCValue) //is it LBADC keypad? (but IR)
			&& (Sp_Keypad->B_ChanNumber == key.ch)) //chack channel number
			break;

			if (B_Index++>=WB_NUMBER_OF_INPUT_SOURCE)
			return FALSE;
		}

		//----Chunbao 20110808 add for menu lock----//	
		#if(OSD_HARD_LOCK == ENABLE)
		if(((key.val == 211)&&(key.ch==1))&&(!CheckMenuKey))
		{
			gm_Print(">>>>>>>>>>>>>>>>>>>>>>>>HardLock start key.ch= %d", key.ch);
			StartHardLockTimer();
			CheckMenuKey = TRUE;	
		}
		#endif
		/*
		//if(((key.val == 211)&&(key.ch==1))&&(CheckMenuKey))
		if(((key.val == 211)&&(key.ch==1))&&(CheckMenuKey))
		{
		//Neil 200801112+ for OSD lock function  Bug list 127
		if(gm_TimerCheck(HardLock_1s_TMR) != TIMER_OK)		//Not Timer out
		{
		gm_Print(">>>>>>>>>>>>>>>>>>>>>>>>HardLock Runing ", 0);
		//if ((UserPrefRunBurnInModeFlag == FALSE) || (IsLockOSD()))//calo add for the when burn in on ,can not enter hard lock status //090615
		if (UserPrefRunBurnInModeFlag == FALSE)//calo add for the when burn in on ,can not enter hard lock status //120913 by Kevin Chen
		OSDRequestMsg(gmvb_MainChannel, MTO_HardLock, REQUEST_NOW);
		SetOsdTimeout();
		CheckMenuKey =	FALSE;
		}
		}
		*/
		//----menu lock 20110808----//
		Keypad_EnterFactory(key.ch,key.val);


		if ( Sp_Keypad->B_KeysNumber == 0 )
		return FALSE;

		EnqueueOsdKey((key.val < ADC_KBD_NOISE_FILTER) ? Key_None :             //wei 140702  for keypad print value change <= to <    //if key released than Key_None
		TranslateCodeToKey(Sp_Keypad->Bp_KeyValues, Sp_Keypad->B_KeysNumber,
		Sp_Keypad->B_CodeLen, Sp_Keypad->B_Delta, key.val));

		
		return TRUE;
	}

	#ifdef TOUCH_KEY
    //only touch pad need to transition	
    if (length_touchpad)//&&(buff_touchpad!= 0 ))//&& (!length_adc))
	{
		//static BYTE B_PreKey = 0xFF, B_KeyCnt = 0;
		if(buff_touchpad!= 0 )
		{
			//key.ch = TouchPad_Value[buff_touchpad].ch;
			//key.val = TouchPad_Value[buff_touchpad].val;
			//msg("===Before: toukey, length_touchpad = %d", length_touchpad);
			msg("===Before: touchkey, buff_touchpad = 0x%x", buff_touchpad);
			/*
			if(B_PreKey != buff_touchpad)
			{
				B_KeyCnt = 1;
				B_PreKey = buff_touchpad;
				return FALSE;
			}
			else if(B_PreKey == buff_touchpad)
			{
				B_KeyCnt++;
				if(B_KeyCnt <= 3)
					return FALSE;

			}
			B_PreKey = buff_touchpad;
			*/
			
			if (buff_touchpad == TOUCHKEY_EXIT)//Exit
			{
				key.ch=0; 
				key.val=WB_Key_Power;
			}
			else if (buff_touchpad == TOUCHKEY_MENU)//Menu
			{
				key.ch=1; 
				key.val=WB_Key_Menu;
			}
			else if (buff_touchpad == TOUCHKEY_DOWN)//Down
			{
				key.ch=1; 
				if(UserPrefKeyChanged == 1)
					key.val=WB_Key_Left;
				else			
					key.val=WB_Key_Down;
			}
			else if (buff_touchpad == TOUCHKEY_UP)//up
			{
				key.ch=1; 
				if(UserPrefKeyChanged == 1)
					key.val=WB_Key_Right;
				else
					key.val=WB_Key_Up;
			}
			else if (buff_touchpad == TOUCHKEY_INPUT)//Source
			{
				key.ch=0; 
				key.val=WB_Key_Source;
			} 		  
			else
			{
				key.ch=0; 
				key.val=0;
			}  

			//common feature by hot key enter DiagnosticTestPattern
			if (buff_touchpad == TOUCHKEY_Diagnostic) //MENU+INPUT enter DiagnosticTestPattern
			{
				OSDRequestMsg(gmvb_MainChannel, MTO_DiagnosticTestPattern, REQUEST_NOW);
				UserPrefBID++;
				SaveFactorySettingsInNVR();//save BID in the factory block
				return FALSE;
			}
			else if(buff_touchpad == TOUCHKEY_Factory) 
			{
				UserPrefFactoryFlag = TRUE;
				gm_Print("#########Mission Factroy key get", 0);
				return FALSE;
			}
			//gm_Print("buff_length(%d)touchpad (%d)been pressed",length_touchpad, buff_touchpad);
			//gm_Print("key.ch = %d", key.ch );
			//gm_Print("key.val= %d", key.val );

			
			msg("===after: touchkey, key.ch = %d", key.ch);
			msg("===after: touchkey, key.val = %d", key.val);
			
		}
		else
		{
			key.ch=0; 
			key.val=0;
		}  

	if ((key.ch ==1) && (key.val==0)&&CheckMenuKey)
	{
		EnqueueOsdKey(Key_None);
	#ifdef BUZZER_USED
		BuzzerProcess(Key_None);
	#endif
	#if(OSD_HARD_LOCK == ENABLE)
		//----Chunbao 20110808 add for menu lock----//
		if((CheckMenuKey)&&(gm_TimerCheck(HardLock_1s_TMR) == TIMER_OK))
		{
		gm_Print(">>>>>>>>>>>>>>>>>>>>>>>>HardLock timer Stop ", 0);
		gm_TimerStop(HardLock_1s_TMR);
		SetOsdTimeout();
		CheckMenuKey = FALSE;
		}
		//----menu lock 20110808----//
	#endif
	}
	
#if (FEATURE_3D == ENABLE)
#if (FEATURE_3D_KEY_HANDLER == ENABLE)
	if (gm_3D_KeyEventHandler((BYTE far *) &key))
	{
		EnqueueOsdKey(Key_None);
		return TRUE;
	}
#endif
#endif
	
	for (B_Index = 0;;)
	{
		Sp_Keypad = WBK_Keypads[B_Index];
		if (Sp_Keypad
		&& (Sp_Keypad->GetKeyValue == GetADCValue) //is it LBADC keypad? (but IR)
		&& (Sp_Keypad->B_ChanNumber == key.ch)) //chack channel number
		break;
	
		if (B_Index++>=WB_NUMBER_OF_INPUT_SOURCE)
		return FALSE;
	}
	
	//----Chunbao 20110808 add for menu lock----//	
#if(OSD_HARD_LOCK == ENABLE)
	if(((key.val == 211)&&(key.ch==1))&&(!CheckMenuKey))
	{
		gm_Print(">>>>>>>>>>>>>>>>>>>>>>>>HardLock start key.ch= %d", key.ch);
		StartHardLockTimer();
		CheckMenuKey = TRUE;	
	}
#endif
	/*
	//if(((key.val == 211)&&(key.ch==1))&&(CheckMenuKey))
	if(((key.val == 211)&&(key.ch==1))&&(CheckMenuKey))
	{
	//Neil 200801112+ for OSD lock function  Bug list 127
	if(gm_TimerCheck(HardLock_1s_TMR) != TIMER_OK)		//Not Timer out
	{
	gm_Print(">>>>>>>>>>>>>>>>>>>>>>>>HardLock Runing ", 0);
	//if ((UserPrefRunBurnInModeFlag == FALSE) || (IsLockOSD()))//calo add for the when burn in on ,can not enter hard lock status //090615
	if (UserPrefRunBurnInModeFlag == FALSE)//calo add for the when burn in on ,can not enter hard lock status //120913 by Kevin Chen
	OSDRequestMsg(gmvb_MainChannel, MTO_HardLock, REQUEST_NOW);
	SetOsdTimeout();
	CheckMenuKey =	FALSE;
	}
	}
	*/
	//----menu lock 20110808----//
	Keypad_EnterFactory(key.ch,key.val);
	
	if ( Sp_Keypad->B_KeysNumber == 0 )
		return FALSE;
	
	EnqueueOsdKey((key.val <= ADC_KBD_NOISE_FILTER) ? Key_None : //if key released than Key_None
	TranslateCodeToKey(Sp_Keypad->Bp_KeyValues, Sp_Keypad->B_KeysNumber,
	Sp_Keypad->B_CodeLen, Sp_Keypad->B_Delta, key.val));

		
		return TRUE;
	}
	
#endif	//TOUCH_KEY

   return TRUE;
#endif   
   }

#else // Non-LPM_PERMANENTLY_CONTROLS_PERIPHERAL
WORD W_AdcPrevKey = 0xFFFF;
BYTE B_LastKey[NUM_KEYPAD_CH];
BYTE B_EmitKey[NUM_KEYPAD_CH];
BYTE B_Count[NUM_KEYPAD_CH];
BYTE GetADCValue( BYTE B_Index )		// WB4
{
   WBK_KEYPAD_DEFINITION ROM* Sp_Keypad;
   WORD W_PhKey = 0;
   BYTE KeyIndex;
   BYTE B_AdcChannel;

   Sp_Keypad = WBK_Keypads[B_Index];

   if ( Sp_Keypad->B_KeysNumber == 0 )
      return FALSE;

   if(Sp_Keypad->B_ChanNumber == 1)
      B_AdcChannel =1;
   else if(Sp_Keypad->B_ChanNumber == 0)
      B_AdcChannel =0;

   W_PhKey = gm_ScanAdcKey(B_AdcChannel);

   //Stability filter
   //See if the ADC value has changed over a threshhold or not
   if (abs(W_PhKey-B_LastKey[B_AdcChannel]) < ADC_DELTA/2)
   {
      if (~B_Count[B_AdcChannel])
         B_Count[B_AdcChannel] ++;
   }
   else
   {
      B_Count[B_AdcChannel] = 0;
   }
   B_LastKey[B_AdcChannel] = W_PhKey;

   if (B_Count[B_AdcChannel] >= 2)
   {
      B_EmitKey[B_AdcChannel] = W_PhKey;
   }
   W_PhKey = B_EmitKey[B_AdcChannel];

   // If difference from emitting value exceeds threshold, then update emitting value
   if (abs((SWORD) W_PhKey-B_AdcData[B_AdcChannel]) > Sp_Keypad->B_Delta/2)
   {
      B_AdcData[B_AdcChannel] = W_PhKey;
      if (W_PhKey > ADC_KBD_NOISE_FILTER)		//is key press present?
      {
      }
      else
      {
         W_PhKey = 0;
      }
   }

   if(W_PhKey < 30) // Ignore noise
      W_PhKey = 0; 

   if ( W_PhKey == 0 && W_AdcPrevKey == 0)
      return FALSE;

   KeyIndex = TranslateCodeToKey(Sp_Keypad->Bp_KeyValues, Sp_Keypad->B_KeysNumber,
                                 Sp_Keypad->B_CodeLen, Sp_Keypad->B_Delta, W_PhKey);
   EnqueueOsdKey(KeyIndex);
   if (W_AdcPrevKey != W_PhKey)
   {
      //msg_x("GetADCValue KeyIndex = %d, W_PhKey = 0x%X",KeyIndex,W_PhKey);
      W_AdcPrevKey = W_PhKey;
   }
   return TRUE;
}
#endif

#endif // #ifdef WBK_FUNCTION_GETADCVALUE_USED

#ifdef WBK_FUNCTION_GETGPIOVALUE_USED

#ifdef VIRTUAL_GPIO_KEY
WORD VirtualGPINPUT,VirtualGPIODIRCTRL;
#endif

WORD ROM W_ArrayOfGPIOInputAddress[] =
{
   GPINPUT0, // Channel 0
#ifdef VIRTUAL_GPIO_KEY
   &VirtualGPINPUT // Channel 1
#endif
};
WORD ROM W_ArrayOfGPIODirAddress[] =
{
   GPIO_0_DIRCTRL, // Channel 0
#ifdef VIRTUAL_GPIO_KEY
   &VirtualGPIODIRCTRL // Channel 1
#endif
};

#pragma pack(push)
#pragma pack(1)
typedef struct WBK_ST_KeyConvert
{
   WORD Key_Ph;
   BYTE Key_Log; // Key index
} WBK_ST_KeyConvertType;
#pragma pack(pop)

WORD GetMaskGPIO(WBK_KEYPAD_DEFINITION ROM* Bp_Data)
{
   BYTE B_Count = ((WBK_KEYPAD_DEFINITION ROM*)Bp_Data)->B_KeysNumber;
   WORD W_KeysMask = 0;
   WBK_ST_KeyConvertType ROM* Sp_Masks = (WBK_ST_KeyConvertType ROM*)(((WBK_KEYPAD_DEFINITION ROM*)Bp_Data)->Bp_KeyValues);

   for (; B_Count; B_Count--)
   {
      W_KeysMask |= Sp_Masks->Key_Ph;
      Sp_Masks++;
   }
   return W_KeysMask;
}

BYTE InitGPIODriver(BYTE B_Index)
{
   WBK_KEYPAD_DEFINITION ROM* Sp_Keypad;
   WORD W_KeysMask;
   WORD W_Address;
   Sp_Keypad = WBK_Keypads[B_Index];
   W_KeysMask = GetMaskGPIO(Sp_Keypad);
   W_Address = W_ArrayOfGPIODirAddress[Sp_Keypad->B_ChanNumber];
   msg("W_Address = 0x%x",W_Address);
   msg("W_KeysMask = 0x%x",W_KeysMask);
   gm_WriteRegWord(W_Address, gm_ReadRegWord(W_Address) & ~W_KeysMask);
   msg("InitGPIODriver(%d)",Sp_Keypad->B_ChanNumber);
   return TRUE;
}

//*****************************************************************************
// FUNCTION     : BYTE GetLGPIOValue (WBK_KEYPAD_DEFINITION ROM* Bp_Data )
// USAGE        : This function reads data from current GPIO
// INPUT        : Bp_Data - pointer to keypad structure
// OUTPUT       : physical GPIO key data
// GLOBALS      : none
// USED_REGS    : none
//*****************************************************************************
#define KEY_POLARITY 1    // polarity for GPIO keypad
WORD GetLGPIOValue(WBK_KEYPAD_DEFINITION ROM* Bp_Data )
{
   WORD W_KeysMask = GetMaskGPIO(Bp_Data);
   BYTE B_Channel = Bp_Data->B_ChanNumber;

   //msg("B_KeysMask 0x%x",W_KeysMask);
#if KEY_POLARITY
   return (gm_ReadRegWord(W_ArrayOfGPIOInputAddress[B_Channel]) & W_KeysMask);
#else
   return (~gm_ReadRegWord(W_ArrayOfGPIOInputAddress[B_Channel]) & W_KeysMask);
#endif    //  KeyPolarity 
}

//*****************************************************************************
// FUNCTION     : BYTE GetGPIOValue (BYTE B_Index)
// USAGE        : This function returns logical data from current GPIO
// INPUT        : B_Index - Keypad/IR channel
// OUTPUT       : logical GPIO key data
// GLOBALS      : none
// USED_REGS    : none
//*****************************************************************************
WORD W_PrevKey = 0xFFFF;
BYTE GetGPIOValue(BYTE B_Index)
{
   WBK_KEYPAD_DEFINITION ROM* Sp_Keypad;
   WORD W_PhKey = 0;
   BYTE KeyIndex;

   Sp_Keypad = WBK_Keypads[B_Index];

   if ( Sp_Keypad->B_KeysNumber == 0 )
      return FALSE;

#ifdef VIRTUAL_GPIO_KEY
   if (gm_TimerCheck(TV_BACKGROUNDAFT_100ms_TMR) == TIMER_TMO)
   {
      VirtualGPINPUT = 0;
      gm_TimerStop(TV_BACKGROUNDAFT_100ms_TMR);
   }
#endif

   W_PhKey = GetLGPIOValue(Sp_Keypad);
   if ( W_PhKey == 0 && W_PrevKey == 0)
      return FALSE;

   KeyIndex = TranslateCodeToKey(Sp_Keypad->Bp_KeyValues, Sp_Keypad->B_KeysNumber,
                                 Sp_Keypad->B_CodeLen, Sp_Keypad->B_Delta, W_PhKey);
   EnqueueOsdKey(KeyIndex);
   if (W_PrevKey != W_PhKey)
   {
      msg_x("GetGPIOValue KeyIndex = %d, W_PhKey = 0x%X",KeyIndex,W_PhKey);
      W_PrevKey = W_PhKey;
   }
   return TRUE;
}

#ifdef VIRTUAL_GPIO_KEY
void SetVirtualGPINPUT(WORD value)
{
   VirtualGPINPUT = value;
   gm_TimerStart(TV_BACKGROUNDAFT_100ms_TMR, 2); // Auto release virtual gpio key timeout 200ms
}
#endif
#endif
//*********************************  END  **************************************

