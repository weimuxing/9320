/*
	$Workfile:   GpioControl.c  $
	$Revision: 1.75 $
	$Date: 2013/01/18 08:54:20 $
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
// MODULE:      GpioControl.c
//
// USAGE:       This module contains RD2_32626H_BOARD specific GPIO handling functions
//
//******************************************************************************
#include <embedded.h>
//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************

//
// I2C SCL FREQ (100KHz)
//
#define I2C_400K_FREQ		(400000L)
#define I2C_400K_SCALE		(((gDW_SystemOcmClk / I2C_400K_FREQ) / 5) - 2)

#define I2C_100K_FREQ		(100000L)
#define I2C_100K_SCALE		(((gDW_SystemOcmClk / I2C_100K_FREQ) / 5) - 2)

#define I2C_50K_FREQ		   (50000L)
#define I2C_50K_SCALE      (((gDW_SystemOcmClk / I2C_50K_FREQ) / 5) - 2)

typedef enum
{
   CPhyType_None,
   CPhyType_DVI,
   CPhyType_HDMI,
   CPhyType_DP
}gmt_CPhyType;

//******************************************************************************
//  E X T E R N S
//******************************************************************************


//******************************************************************************
//
// FUNCTION       : void InitSystemGpio(void)
//
// USAGE          : Call individual functions to initialize the system board control GPIO.
//
// INPUT          : 
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void InitSystemGpio(void)
{
   // Mission GPIO control
   //GPIO1: SCBL	GPIO4: Dual Flash switch GPIO2(bit2): Audio amp. gain1 ctrl
   //GPIO3(bit3): SPI wr protect	GPIO5(bit5): Ext. EDID wr protect
   //GPIO6(bit6): Ext. EDID wr protect
   //gm_SetRegBitsWord(GPIO_0_ENABLE,(BIT1 | BIT3 | BIT4 | BIT5 | BIT6));
   //gm_SetRegBitsWord(GPIO_0_DIRCTRL,(BIT1 | BIT3 | BIT4 | BIT5 | BIT6));

   // Enable SCBL on GPIO1_PWM14

#ifdef Athena_C
	gm_ClearRegBitsWord(MFP_SPARE2,(BIT0 | BIT1 | BIT2));  //enable PWM0, PWM1 , PWM2

#else
	gm_SetRegBitsWord(MFP_SPARE2,(BIT1 | BIT4)); 
#endif

	
	// GPIO77(13): UDO0	GPIO78(14): UDO0		GPIO79(15): LED_Green
	gm_SetRegBitsWord(GPIO_5_DIRCTRL, LED_GREEN_DIR);
	gm_SetRegBitsWord(GPIO_5_ENABLE, LED_GREEN_EN);   
	
	#if((BOARD_USED == BOARD_93xx_CJ2))
		gm_SetRegBitsWord(GPIO_5_ENABLE,  (GPIO82_EN|GPIO83_EN | GPIO86_EN | GPIO88_EN|GPIO95_EN));
		#if(DEFAULT_PANEL == SHARP_1080P_LD900D3LE1)
			gm_SetRegBitsWord(GPIO_5_DIRCTRL, (GPIO85_IO|GPIO86_IO|GPIO88_IO|GPIO95_IO));//GPIO88_IO Decoder Detect
		#else
			gm_SetRegBitsWord(GPIO_5_DIRCTRL, (GPIO85_IO|GPIO86_IO|GPIO88_IO|GPIO95_IO));//GPIO88_IO LED blinking
		#endif
			gm_SetRegBitsWord(GPIO_5_DIRCTRL, (GPIO82_IO|GPIO83_IO));//Aud Gain Control
			gm_ClearRegBitsWord(GPOUTPUT5, (GPIO82_OUT|GPIO83_OUT));//Aud Gain Control
			//GPIO95 for Main mute
			gm_SetRegBitsWord(GPOUTPUT5, GPIO95_OUT);
			HEADPHONE_MUTE();
	#endif

//#ifdef USE_SECOND_UART
   // UART1 share pins (GPIO 79 & 80) with green LED and DIP connect detect
   #if(OSD_UART_DEF == UART1)
   {
	   gm_ClearRegBitsWord(GPIO_4_ENABLE, GPIO79_EN);
	   gm_ClearRegBitsWord(GPIO_5_ENABLE, GPIO80_EN);
   }
   #endif
//#endif

#if (FEATURE_DP_HUB == ENABLE)
   // Disabel/Enable SPDIF/I2S for VegaTX audio, default: SPDIF 
   //                 HeadPhone    SPDIF    I2S
   // ------------------------------------------
   // GPIO67_EN       1            0           1
   // GPIO67_IO       0            x           0
   // GPIO87_EN       1            1           0
   // GPIO87_IO       0            0           x
   gm_ClearRegBitsWord(GPIO_4_ENABLE, GPIO67_EN);
   gm_ClearRegBitsWord(GPIO_5_DIRCTRL, GPIO87_IO);
   gm_SetRegBitsWord(GPIO_5_ENABLE, GPIO87_EN);
#endif

#ifdef EXTERNAL_DECODER
   gm_SetRegBitsWord(GPOUTPUT5,GPIO88_OUT);
#endif

   #if((BOARD_USED == BOARD_93xx_CJ2))
   // GPIO92 HeadPhone mute
   gm_SetRegBitsWord(GPIO_5_ENABLE,HEADPHONE_MUTE_EN);
   gm_SetRegBitsWord(GPIO_5_DIRCTRL,HEADPHONE_MUTE_DIR);
   #else
   // GPIO92 SPI-flash Hold. Enable GPIO function to override ENABLE_30BIT_OUTPUT 
   gm_SetRegBitsWord(GPIO_5_ENABLE,GPIO92_EN);
   gm_ClearRegBitsWord(GPIO_5_DIRCTRL,GPIO92_IO);
   #endif

#if 1 // Disable write protect // ToDo: need implement write protect control function then enable it.
   // GPIO93 SPI-flash WP (0:Enable WP/1:Disable WP)
	FLASH_WP_ENABLE();
	gm_SetRegBitsWord(GPIO_5_ENABLE,FLASH_WP_EN);
	gm_SetRegBitsWord(GPIO_5_DIRCTRL,FLASH_WP_DIR);
	gm_ClearRegBitsWord(GPIO_5_OPENDRAIN_EN, FLASH_WP_OD);

	// GPIO90 NVRAM WP (0:Disable WP/1:Enable WP)
	NVRAM_WP_ENABLE();
	gm_SetRegBitsWord(GPIO_5_ENABLE,NVRAM_WP_EN);
	gm_SetRegBitsWord(GPIO_5_DIRCTRL,NVRAM_WP_DIR);
	gm_ClearRegBitsWord(GPIO_5_OPENDRAIN_EN,NVRAM_WP_OD);	//Enable OD: 2.8V ,Disable OD: 3.11V 

	// GPIO91 EDID WP (0:Disable WP/1:Enable WP)
	EDID_WP_ENABLE();
	gm_SetRegBitsWord(GPIO_5_ENABLE,EDID_WP_EN);
	gm_SetRegBitsWord(GPIO_5_DIRCTRL,EDID_WP_DIR);
	gm_SetRegBitsWord(GPIO_5_OPENDRAIN_EN,EDID_WP_OD);
#endif

#if (DEFAULT_PANEL == SHARP_1080P_LD900D3LE1)
	gm_SetRegBitsWord(GPIO_5_ENABLE,SHARP_BL_EN_EN);
	gm_SetRegBitsWord(GPIO_5_DIRCTRL,SHARP_BL_EN_DIR);
	gm_SetRegBitsWord(SHARP_BL_EN_PORT,SHARP_BL_EN_PIN);
#endif

	//Hyb130804+
	#if(BOARD_USED == BOARD_93xx_CJ2)
   	// LPM GPIO0 LPM_NVRAM_WP	
	gm_SetRegBitsWord(LPM_GPIO_ENABLE1,LPM_GPIO0_EN);
   	gm_SetRegBitsWord(LPM_GPIO_DIRCTRL1,LPM_GPIO0_IO);
   	gm_SetRegBitsWord(LPM_GPOUTPUT_0,LPM_GPIO0_OUT);
	#endif
	
	#ifdef RD3_93xx_BOARD		//BOARD_93xx_CJ2&BOARD_93xx_CJ3
   	//RD3 boards
   	// LPM GPIO5 HDMI switch TMDS_EN (1:Power on/0:Power off)		
	gm_SetRegBitsWord(LPM_GPIO_ENABLE1,LPM_GPIO5_EN);
   	gm_SetRegBitsWord(LPM_GPIO_DIRCTRL1,LPM_GPIO5_IO);
   	gm_SetRegBitsWord(LPM_GPOUTPUT_0,LPM_GPIO5_OUT);
	#else
   	//LPM GPADC3 HDMI switch TMDS_EN (1:Power on/0:Power off) // Config in LPM 
	gm_ClearRegBitsWord(LPM_GPADC_GPIO_CONTROL,TMDS_EN);
	gm_SetRegBitsWord(LPM_GPADC_GPIO_CONTROL,TMDS_DIR);
	gm_SetRegBitsWord(LPM_GPADC_GPIO_CONTROL,TMDS_OD);
	gm_SetRegBitsWord(LPM_GPADC_GPOUTPUT,TMDS_PIN);   

	//HYB130112+ TK board LPM GPIO 5 as HDMI cable Detect
	gm_SetRegBitsWord(LPM_GPIO_ENABLE1,LPM_GPIO5_EN);
   	gm_ClearRegBitsWord(LPM_GPIO_DIRCTRL1,LPM_GPIO5_IO);
   	gm_SetRegBitsWord(LPM_GPIO_OD_EN_0,LPM_GPIO5_OD);	
	#endif
	
   // LPM GPIO19 HDMI switch TMDS_SEL (0:HDMI/1:DVI)
	gm_SetRegBitsWord(LPM_GPIO_ENABLE2,LPM_GPIO19_EN);
	gm_SetRegBitsWord(LPM_GPIO_DIRCTRL2,LPM_GPIO19_IO);
	gm_ClearRegBitsWord(LPM_GPOUTPUT_1,LPM_GPIO19_OUT);	// set to HDMI

	//gm_Print("LPM GPIO 7 set",0);	
	gm_SetRegBitsWord(LPM_GPIO_ENABLE1, (BIT0 | BIT3/* | LPM_GPIO7_EN*/));//20120719 Add BIT4 by KevinChen for enable GPIO4. //20120507 Add by KevinChen


	#if (defined(RD3_93xx_BOARD))		//BOARD_93xx_CJ1_RD3 and CJ2 for RS485 control	//
	gm_SetRegBitsWord(LPM_GPOUTPUT_0, LPM_GPIO9_OUT);
	gm_SetRegBitsWord(LPM_GPIO_DIRCTRL1, LPM_GPIO9_IO);
	gm_SetRegBitsWord(LPM_GPIO_ENABLE1, LPM_GPIO9_EN);	
	#endif
	
	gm_ClearRegBitsWord(LPM_GPOUTPUT_1,LPM_GPIO20_OUT);
	gm_SetRegBitsWord(LPM_GPIO_DIRCTRL2, LPM_GPIO20_IO);
	gm_SetRegBitsWord(LPM_GPIO_ENABLE2, LPM_GPIO20_EN);

	#ifdef BUZZER_USED
	// Buzzer Init
	gm_SetRegBitsWord(BUZZER_PWM_CONFIG, (PWM2_GPIOn_EN | PWM2_CLKSEL | PWM2_TCLK_DIV));
	#endif

	#ifdef TOUCHPAD_SUPPORT
    	gm_SetRegBitsWord(LPM_CSM_I2C_MUX_CTRL, LPM_I2C_M_SEL);//110726 Edward for LPM touch pad control
	#endif
   // OCM_INT1 init
   gm_SetRegBitsWord(MFP_MISC_ALT_PINS, MFP_OCM_INT1_EN);

   // Cable detect / Source (Host) power detect / HPD (Hot plug) configuration
   // HPD1 (Bit2-0)
   //	000: 0(TRI)	001: DP0_HPD	010: HDMI_HPD	011: DP0_HPD(if PD1=1)	100: HDMI_HPD(if PD1=1)
   //	101: DP0_HPD(TRI, if DP1_HPD=1)	110: HDMI_HPD(TRI, if HDMI_HPD=1)
   // HPD2 (Bit6-4)
   //	000: 0(TRI)	001: DVI_HPD		010: HDMI_HPD	011: DP0_HPD(if PD1=1)	100: HDMI_HPD(if PD1=1)
   //	101: DVI_HPD(TRI, if DVI_HPD=1)	110: HDMI_HPD(TRI, if HDMI_HPD=1)
   // HPD3 (Bit10-8)
   //	000: 0(TRI)	001: DP1_HPD	010: HDMI_HPD	011: DP0_HPD(if PD1=1)	100: HDMI_HPD(if PD1=1)
   //	101: DP1_HPD(TRI, if DP1_HPD=1)	110: HDMI_HPD(TRI, if HDMI_HPD=1)
#if (FEATURE_DISPLAY_PORT == ENABLE) && ENABLE_HPD_OVERRIDED
   gm_WriteRegWord(LPM_CSM_HPD_OUT_CTRL,0x0217);   // 0000 0010 0001 0111  SWENG0622: Force DP0 HPD to low during system initial
#else
   gm_WriteRegWord(LPM_CSM_HPD_OUT_CTRL,0x0211);	// 0000 0010 0001 0001
#endif

	#ifdef DELL_U2410F
	gm_WriteRegWord(LPM_CSM_HPD_PD_IN_CTRL,0x03F7);//20120905 Modify suggest by JP 0x3F7
	#else
   gm_WriteRegWord(LPM_CSM_HPD_PD_IN_CTRL,0x00BF);
	#endif

#if (defined(BOARD_93XX_4K2K))
	#if ((DEFAULT_PANEL == ZDT_4K2K_CMI))
		#if((BOARD_USED == BOARD_93xx_CJ2))
			//Hyb130802+ for PS_ON control;
			gm_SetRegBitsWord(LPM_GPIO_ENABLE1, LPM_GPIO3_EN);
			gm_SetRegBitsWord(LPM_GPIO_DIRCTRL1, LPM_GPIO3_IO);
			gm_SetRegBitsWord(LPM_GPOUTPUT_0, LPM_GPIO3_OUT);		//default set as high, power saving set low by condition
		#endif	//(BOARD_USED == BOARD_93xx_CJ2)
	#endif	//(DEFAULT_PANEL == ZDT_4K2K_CMI)
#endif	//(defined(BOARD_93XX_4K2K))


   if (gm_ReadRegWord(LPM_MISSION_STATUS) & LP_SRAM_LOADED)
   {
      //LPM_C register can't RW by Mission directly after LPM starts up
      WORD msg[3] = {Clear_bit, LPM_GPIO_ENABLE1, LPM_GPIO8_EN|LPM_GPIO9_EN|LPM_GPIO10_EN};//3: clear bits
      gm_MessageSend(gmd_MESSAGE_LPM_GPIO_CTRL, &msg, sizeof(msg));
   }
   //Only work for 39"????
#if(defined(BOARD_93XX_4K2K))
	#if ((DEFAULT_PANEL == ZDT_4K2K_CMI))
		#if((BOARD_USED == BOARD_93xx_CJ2))   
		   //gm_Delay10ms(15);	// Don't Delay here, othwise system may hangup!!!
		   gm_WriteRegWord(PANEL_PWR_UP,0);
		   gm_SetRegBitsWord(DISPLAY_CONTROL, DTG_RUN_EN);
		   //gm_DisplayOnOff(ON);
		   //gm_Delay10ms(15);	// Don't Delay here, othwise system may hangup!!!
		#endif	//(BOARD_USED == BOARD_93xx_CJ2)
	#endif	//(DEFAULT_PANEL == ZDT_4K2K_CMI)
#endif	//(defined(BOARD_93XX_4K2K))

   
}

//******************************************************************************
//
// FUNCTION       : void GpioSoftReset(WORD W_Delay)
//
// USAGE          : Initializes the soft reset (SRESET) GPIO
//
// INPUT          : W_Delay - number of milliseconds to be delayed in the function
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : IO_EXP_PCF8575 (R/W)
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void GpioSoftReset(WORD W_Delay)
{
   // Mars is not using external IO
   UNUSED_VAR(W_Delay);
}

//******************************************************************************
//
// FUNCTION       : void I2C_Init(void)
//
// USAGE          : Initializes the I2C port
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : I2C_MST_CTR			(W)
//                  I2C_MST_CLK_SCALE	(W)
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void I2C_Init(void)
{
   // Master I2C on MSTR0
#ifdef RD2_93xx_BOARD
   gm_WriteRegWord(I2C_MST_CTR, I2C_EN | I2C_SDA_SCL0_EN /*| I2C_SDA_SCL1_EN*/);  //Workaround for RD2's NVRAM issue
   gm_WriteRegWord(I2C_MST_CLK_SCALE, I2C_50K_SCALE);
#else
   gm_WriteRegWord(I2C_MST_CTR, I2C_EN | I2C_SDA_SCL0_EN | I2C_SDA_SCL1_EN);
   gm_WriteRegWord(I2C_MST_CLK_SCALE, I2C_100K_SCALE);
#endif
}

//******************************************************************************
//
// FUNCTION       : void GpioLed1Keypad(BOOL B_State)
//
// USAGE          : Writes to the registers to turn on or off LED1
//
// INPUT          : B_State - TRUE or FALSE
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : IO_EXP_PCF8575 (R/W)
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
//#define RD1_93xx_BOARD_REV_A  // Athena RD1 Rev.A has LED1 (green) can't turn off when mission power off issue.
void GpioLed1Keypad(BOOL B_State)
{
   if (B_State == TRUE)
   {
#ifdef RD1_93xx_BOARD_REV_A
      gm_ClearRegBitsWord(GPOUTPUT4, GPIO79_OUT);
#else
	  gm_SetRegBitsWord(LED_GREEN_PORT, LED_GREEN_PIN); 
#endif
   }
   else
   {
	#ifdef RD1_93xx_BOARD_REV_A
      gm_SetRegBitsWord(GPOUTPUT4, GPIO79_OUT);
	#else
	  gm_ClearRegBitsWord(LED_GREEN_PORT, LED_GREEN_PIN); 
	#endif
   }
}

//******************************************************************************
//
// FUNCTION       : void GpioLed2Keypad(BOOL B_State)
//
// USAGE          : Writes to the registers to turn on or off LED2
//
// INPUT          : B_State - TRUE or FALSE
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : IO_EXP_PCF8575 (R/W)
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void GpioLed2Keypad(BOOL B_State)
{
   WORD msg_data[3];

   msg_data[1] = LPM_GPOUTPUT_1;
   msg_data[2] = LPM_GPIO20_OUT;

   if (B_State == TRUE)
   {
      msg_data[0] = Set_bit;
      gm_MessageSend(gmd_MESSAGE_LPM_GPIO_CTRL, msg_data, sizeof(msg_data));
   }
   else
   {
		msg_data[0] = Clear_bit;
      gm_MessageSend(gmd_MESSAGE_LPM_GPIO_CTRL, msg_data, sizeof(msg_data));
   }
}

//******************************************************************************
//
// FUNCTION       : void GpioLedFilmMode1(BYTE B_State)
//
// USAGE          : Turns on or off LED_MADI
//
// INPUT          : B_State - TRUE or FALSE
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : IO_EXP_PCF8575 (R/W)
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void GpioLedFilmMode1(BYTE B_State)
{
   UNUSED_VAR(B_State);
}

//******************************************************************************
//
// FUNCTION       : void GpioLedFilmMode0(BYTE B_State)
//
// USAGE          : Turns on or off AFP_LED
//
// INPUT          : B_State - TRUE OR FALSE
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : IO_EXP_PCF8575 (R/W)
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void GpioLedFilmMode0(BYTE B_State)
{
   UNUSED_VAR(B_State);
}

//******************************************************************************
//
// USAGE          : This is function which is called out of "SelectInput"
//                  procedure to configure board specific input parameters .
//                  This is function is being called through a pointer in the ports
//                  attributes table and provides flexibiliy with different functions
//                  associated with different ports.
//
// INPUT          : B_Channel - Current input channel
//                  B_InputConnector - Input connector to be connected to the channel
//
//******************************************************************************
#pragma argsused
void SelectInputExternal(gmt_PHY_CH B_Channel, gmt_UserDefConnNames B_InputConnector)
{
   //--------- HDCP port switch -----------
   if ((UserPrefCurrentInputMain == ALL_BOARDS_VGA1) || (UserPrefCurrentInputPip == ALL_BOARDS_VGA1))
   {
      gm_WriteRegWord(HW_SERIAL_SLAVE, 0x100); // VGA
   }
   else
   {
      gm_WriteRegWord(HW_SERIAL_SLAVE, 0x200); // DVI or HDMI
   }

#if 0
   if ((UserPrefCurrentInputPip != ALL_BOARDS_HDMI) && (UserPrefCurrentInputMain != ALL_BOARDS_HDMI))
   {
      gm_WriteRegWord(MFP_SPARE1, 0x1); // DVI SLV2
   }
   else
   {
      gm_WriteRegWord(MFP_SPARE1, 0x2); // HDMI SLV3
   }
#endif
   //------ DDC2Bi port switch ---------
   {
      // Use CSM I2C Mux to route I2C slave pins to DDC
      static TDdc2bi ddc2biData;
      if (UserPrefCurrentInputMain == ALL_BOARDS_VGA1)
      {
         gm_DDC2Bi_Init(DDC2BI_CHANNEL0, &ddc2biData);
         gm_CsmI2CMuxSelect(MUX_DDC4_LpmI2cSlv4);
      }
      else
      {
         gm_DDC2Bi_Init(DDC2BI_CHANNEL1, &ddc2biData);
#if 1
         if ((B_InputConnector==ALL_BOARDS_HDMI) || (B_InputConnector==ALL_BOARDS_DVI1))
            gm_CsmI2CMuxSelect(MUX_DDC2_LpmI2cSlv2_MisDviCp);
#else
         if (UserPrefCurrentInputMain == ALL_BOARDS_HDMI)
            gm_CsmI2CMuxSelect(MUX_DDC3_Lpmi2cSlv3_MisHdmiCp);
         else if (UserPrefCurrentInputMain == ALL_BOARDS_DVI1)
            gm_CsmI2CMuxSelect(MUX_DDC2_LpmI2cSlv2_MisDviCp);
#endif
      }
   }
}




void EXT_FUNC HdmiDviExtPortSetup_RD1_DDVI(void)
{
   //#pragma message "ToDo: Implement External Mux control for Dual DVI1 "

   //send command to LPM , to assign MUX to select channel
   WORD command[3]={Set_bit, LPM_GPOUTPUT_1, LPM_GPIO19_OUT};

   //gm_Print(" HdmiDviExtPortSetup_RD1_DDVI() ",0);

   if (gm_MessageSend(gmd_MESSAGE_LPM_GPIO_CTRL, &command, sizeof(command))==FALSE)
      gm_Print("---fail  to send MUX command",0);

}

void EXT_FUNC HdmiDviExtPortSetup_RD1_HDMI1(void)
{
   //#pragma message "ToDo: Implement External Mux control for HDMI1"
   // DDVI use combo 2 3, HDMI use combo 1
   WORD command[3]={Clear_bit, LPM_GPOUTPUT_1, LPM_GPIO19_OUT};

   //gm_Print(" HdmiDviExtPortSetup_RD1_HDMI1() ",0);

   if (gm_MessageSend(gmd_MESSAGE_LPM_GPIO_CTRL, &command, sizeof(command)) == FALSE)
      gm_Print("---fail  to send MUX command",0);

   //gm_ClearAndSetBitsWord(HOST_DVI_DP_SETUP , HDMI_PORT_SEL,0x00);
}

#if defined(RD3_93xx_BOARD) || defined(RD1_93xx_BOARD_REV_B)
#if((BOARD_USED == BOARD_93xx_CJ2))
//RD3 board
#define LBADC_DELTA                  8 // Tolerance
#define LBADC_DVI_CABLE            230 // ADC value for DVI cable plugged only
#define LBADC_VGA_CABLE            200 // ADC value for VGA cable plugged only
#define LBADC_DVI_5V_CABLE         183 // ADC value for DVI cable plugged+DVI_5V valid only
#define LBADC_VGA_5V_CABLE         125 // ADC value for DVI cable plugged & VGA cable plugged+VGA_5V valid
#define LBADC_DVI_VGA_CABLE        111 // ADC value for DVI cable plugged & VGA cable plugged
#define LBADC_DVI_VGA_5V_CABLE     114 // ADC value for DVI cable plugged & VGA cable plugged+VGA_5V valid
#define LBADC_DVI_5V_VGA_CABLE     138 // ADC value for DVI cable plugged+DVI_5V valid & VGA cable plugged
#define LBADC_DVI_5V_VGA_5V_CABLE   100 // ADC value for DVI cable plugged+DVI_5V valid & VGA cable plugged+VGA_5V valid
#define LBADC_HDMI_CABLE           210 	//Hyb130706* Org = 210// ADC value for HDMI cable plugged only
#define LBADC_HDMI_5V_CABLE        153 //Hyb130706* Org = 159 // ADC value for HDMI cable plugged+HDMI_5V valid
#else
#define LBADC_DELTA                  8 // Tolerance
#define LBADC_DVI_CABLE            238 // ADC value for DVI cable plugged only
#define LBADC_VGA_CABLE            205 // ADC value for VGA cable plugged only
#define LBADC_DVI_5V_CABLE         187 // ADC value for DVI cable plugged+DVI_5V valid only
#define LBADC_VGA_5V_CABLE         126 // ADC value for DVI cable plugged & VGA cable plugged+VGA_5V valid
#define LBADC_DVI_VGA_CABLE        170 // ADC value for DVI cable plugged & VGA cable plugged
#define LBADC_DVI_VGA_5V_CABLE     112 // ADC value for DVI cable plugged & VGA cable plugged+VGA_5V valid
#define LBADC_DVI_5V_VGA_CABLE     142 // ADC value for DVI cable plugged+DVI_5V valid & VGA cable plugged
#define LBADC_DVI_5V_VGA_5V_CABLE   99 // ADC value for DVI cable plugged+DVI_5V valid & VGA cable plugged+VGA_5V valid
#define LBADC_HDMI_CABLE           149 	//Hyb130706* Org = 210// ADC value for HDMI cable plugged only
#define LBADC_HDMI_5V_CABLE        97 //Hyb130706* Org = 159 // ADC value for HDMI cable plugged+HDMI_5V valid
#endif
#else

#if((BOARD_USED == BOARD_93xx_CJ2))
#define LBADC_DELTA                  8 // Tolerance
#define LBADC_DVI_CABLE            238 // ADC value for DVI cable plugged only
#define LBADC_VGA_CABLE            205 // ADC value for VGA cable plugged only
#define LBADC_DVI_5V_CABLE         187 // ADC value for DVI cable plugged+DVI_5V valid only
#define LBADC_VGA_5V_CABLE         126 // ADC value for DVI cable plugged & VGA cable plugged+VGA_5V valid
#define LBADC_DVI_VGA_CABLE        170 // ADC value for DVI cable plugged & VGA cable plugged
#define LBADC_DVI_VGA_5V_CABLE     112 // ADC value for DVI cable plugged & VGA cable plugged+VGA_5V valid
#define LBADC_DVI_5V_VGA_CABLE     142 // ADC value for DVI cable plugged+DVI_5V valid & VGA cable plugged
#define LBADC_DVI_5V_VGA_5V_CABLE   99 // ADC value for DVI cable plugged+DVI_5V valid & VGA cable plugged+VGA_5V valid
#define LBADC_HDMI_CABLE           149 	//Hyb130706* Org = 210// ADC value for HDMI cable plugged only
#define LBADC_HDMI_5V_CABLE        97 //Hyb130706* Org = 159 // ADC value for HDMI cable plugged+HDMI_5V valid
#else//RD 1 Orginal
#define LBADC_DVI_CABLE     210 // ADC value for only DVI cable plugged
#define LBADC_VGA_CABLE     158 // ADC value for only VGA cable plugged
#define LBADC_DVI_VGA_CABLE 126 // ADC value for both DVI and VGA cable plugged
#endif
#endif

#define LBADC_DELTA           8 // Tolerance

#define LBADC_EQUAL(a,b) ((a) < (b)+LBADC_DELTA) && ((a) >  (b)-LBADC_DELTA)
#if defined(Qisda_93XX_BOARD)//20121204;modified tolerance +10/-15 for HDMI cable detect issue from QCS with MP HW 24K impedance;Barbara Lin
#define LBADC_EQUAL_HDMI(a,b) ((a) < ((b)+(LBADC_DELTA - 5))) && ((a) > (b)-LBADC_DELTA)
#endif

extern BYTE LPM_CableStatus[8];
void UpdateCableStatus(CableInfo * CableUpdate)
{

   // DP1.2 DDVI HDMI1.4
   // LPM_CableStatus[4]: LPM LB ADC 2
   // LPM_CableStatus[6]: LPM LB ADC 3

   // DP get cable status by mission
   if (!(gm_ReadRegByte(LPM_CSM_HPD_PD_IN_STATUS) & PD1_IN_STATUS))
   {
      CableUpdate->PHY1_CableStatus = TRUE;
   }
   else
   {
      CableUpdate->PHY1_CableStatus = FALSE;
   }

   if (gm_ReadRegByte(LPM_CSM_HPD_PD_IN_STATUS) & PD2_IN_STATUS)
   {
      CableUpdate->PHY1_SourcePowerStatus = TRUE;
   }
   else
   {
      CableUpdate->PHY1_SourcePowerStatus = FALSE;
   }

#if defined(RD3_93xx_BOARD) || defined(RD1_93xx_BOARD_REV_B)
   // DDVI/DVI
   if( LBADC_EQUAL(LPM_CableStatus[4],LBADC_DVI_CABLE) || LBADC_EQUAL(LPM_CableStatus[4],LBADC_DVI_5V_CABLE)
    || LBADC_EQUAL(LPM_CableStatus[4],LBADC_DVI_VGA_CABLE) || LBADC_EQUAL(LPM_CableStatus[4],LBADC_DVI_VGA_5V_CABLE)
    || LBADC_EQUAL(LPM_CableStatus[4],LBADC_DVI_5V_VGA_CABLE) || LBADC_EQUAL(LPM_CableStatus[4],LBADC_DVI_5V_VGA_5V_CABLE) )
      CableUpdate->PHY2_CableStatus = TRUE;
   else
      CableUpdate->PHY2_CableStatus = FALSE;

   if( LBADC_EQUAL(LPM_CableStatus[4],LBADC_DVI_5V_CABLE) 
    || LBADC_EQUAL(LPM_CableStatus[4],LBADC_DVI_5V_VGA_CABLE)
    || LBADC_EQUAL(LPM_CableStatus[4],LBADC_DVI_5V_VGA_5V_CABLE) )
      CableUpdate->PHY2_SourcePowerStatus = TRUE;
   else
      CableUpdate->PHY2_SourcePowerStatus = FALSE;

   // HDMI
   if( LBADC_EQUAL(LPM_CableStatus[6],LBADC_HDMI_CABLE) 
      || LBADC_EQUAL(LPM_CableStatus[6],LBADC_HDMI_5V_CABLE) )
      CableUpdate->PHY3_CableStatus = TRUE;
   else
      CableUpdate->PHY3_CableStatus = FALSE;

   if( LBADC_EQUAL(LPM_CableStatus[6],LBADC_HDMI_5V_CABLE) )
      CableUpdate->PHY3_SourcePowerStatus = TRUE;
   else
      CableUpdate->PHY3_SourcePowerStatus = FALSE;

   // VGA
   if( LBADC_EQUAL(LPM_CableStatus[4],LBADC_VGA_CABLE) || LBADC_EQUAL(LPM_CableStatus[4],LBADC_VGA_5V_CABLE)
    || LBADC_EQUAL(LPM_CableStatus[4],LBADC_DVI_VGA_CABLE) || LBADC_EQUAL(LPM_CableStatus[4],LBADC_DVI_VGA_5V_CABLE)
    || LBADC_EQUAL(LPM_CableStatus[4],LBADC_DVI_5V_VGA_CABLE) || LBADC_EQUAL(LPM_CableStatus[4],LBADC_DVI_5V_VGA_5V_CABLE) )
      CableUpdate->VGA_CableStatus = TRUE;
   else
      CableUpdate->VGA_CableStatus = FALSE;

   if( LBADC_EQUAL(LPM_CableStatus[4],LBADC_VGA_5V_CABLE) || LBADC_EQUAL(LPM_CableStatus[4],LBADC_DVI_VGA_5V_CABLE)
    || LBADC_EQUAL(LPM_CableStatus[4],LBADC_DVI_5V_VGA_5V_CABLE) )
      CableUpdate->VGA_SourcePowerStatus = TRUE;
   else
      CableUpdate->VGA_SourcePowerStatus = FALSE;
#else
   // DDVI/DVI
   if (LBADC_EQUAL(LPM_CableStatus[4],LBADC_DVI_CABLE) || LBADC_EQUAL(LPM_CableStatus[4],LBADC_DVI_VGA_CABLE))
      CableUpdate->PHY2_CableStatus = TRUE;
   else
      CableUpdate->PHY2_CableStatus = FALSE;

   CableUpdate->PHY2_SourcePowerStatus = 0;

   // HDMI
   if (!(gm_ReadRegByte(LPM_CSM_HPD_PD_IN_STATUS) & PD3_IN_STATUS))
      CableUpdate->PHY3_CableStatus = TRUE;
   else
      CableUpdate->PHY3_CableStatus = FALSE;

   CableUpdate->PHY3_SourcePowerStatus = 0;

   // VGA
   if (LBADC_EQUAL(LPM_CableStatus[4],LBADC_VGA_CABLE) || LBADC_EQUAL(LPM_CableStatus[4],LBADC_DVI_VGA_CABLE))
      CableUpdate->VGA_CableStatus = TRUE;
   else
      CableUpdate->VGA_CableStatus = FALSE;

   CableUpdate->VGA_SourcePowerStatus = 0;
#endif

}

void LPM_GPIO_Config(void)
{
#if 1
   //=============================================================
   // RD1 Board LPM GPIO Config
   // TYPE :
   //    00: Special function
   //    01: GPIO_IN
   //    10: GPIO_OUT
   //    11: Not Use pin
   //=============================================================
   //LPM GPIO control, shared PBus
   // GPIO0(Bit0): ST sensor INT 1  GPIO1: VGA DDC SDA   GPIO2: VGA DDC SCL GPIO3: ST power module ctrl
   // GPIO4: CEC  GPIO5: CP2 DP Cable det GPIO6: CP0 DP Power det
   // GPIO7: CP0 cable det GPIO8: CP2 DP HPD GPIO9: CP1 HDMI HPD
   // GPIO10: CP0 DP HPD   GPIO11: Not use   GPIO12: Not use
   // GPIO13: CP1 HDMI DDC SDA   GPIO14: CP1 HDMI DDC SCL
   // GPIO15(bit0): LPM master I2C SCL // GPIO16: LPM master I2C SDA
   // GPIO17: DP TX HPD in GPIO18: Mission power ctr
   // GPIO19: Not use      GPIO20: LED Amber
   //=============================================================

   // 00: Special function 01: GPIO_IN 10:GPIO_OUT 11: Not Use pin
   //GPIO0 - GPIO3   
   #if (defined(ST_4K2K_93xx_BOARD)&& defined(Athena_4K2K_Chip_C) && ENABLE_SOC_IPC_SLAVE_DEV_COMM)
   gm_WriteRegWord(MISSION_LPM_DATA_OUT_1, 0x82); //LPM_GPIO0 as Output pin for Athena C control Athena B INT_2 pin
   #else //Original
   gm_WriteRegWord(MISSION_LPM_DATA_OUT_1, 0x80);
   #endif
   //GPIO4 - GPIO7   
   gm_WriteRegWord(MISSION_LPM_DATA_OUT_2, 0x00);
   //GPIO8 - GPIO11 
   gm_WriteRegWord(MISSION_LPM_DATA_OUT_3, 0x00);
   //GPIO12 - GPIO15 
   gm_WriteRegWord(MISSION_LPM_DATA_OUT_4, 0x00);
   //GPIO16 - GPIO19 
   gm_WriteRegWord(MISSION_LPM_DATA_OUT_5, 0x80);
   //GPIO20    
   gm_WriteRegWord(MISSION_LPM_DATA_OUT_6, 0x02);
   //GPADC0 - GPADC3 
   #if defined(RD3_93xx_BOARD) || defined(RD1_93xx_BOARD_REV_B)
   gm_WriteRegWord(MISSION_LPM_DATA_OUT_7, 0x04);
   #else
   gm_WriteRegWord(MISSION_LPM_DATA_OUT_7, 0x03);
   #endif

   //LPM_GPADC control
   // ADC_AIN0: Keypad_in1 ADC_AIN1: Keypad2 ADC_AIN2: VGA / DVI cable det
   // ADC_AIN3: Used for HDMI cable det on RD3

   //Power Key deifinition ADC channel (0 - 3)
   gm_WriteRegWord(MISSION_LPM_DATA_OUT_8, 0x00|(LPM_FORCE_POWER_DOWN<<8));
   //Power Key deifinition ADC value (0-255)
   gm_WriteRegWord(MISSION_LPM_DATA_OUT_9, 0x26);
   //ST touch sensor deifinition SLV channel (1 - 4) 0: No Touch sensor
   gm_WriteRegWord(MISSION_LPM_DATA_OUT_10, 0x01); //SLV1
   //ST touch sensor deifinition INT GPIO (0 - 20)
   //gm_WriteRegWord(MISSION_LPM_DATA_OUT_11, 0x00); //GPIO0
   gm_WriteRegWord(MISSION_LPM_DATA_OUT_11, UserPrefBoardID); //GPIO0	//Hyb131104+ send boardID to LPM

   //Send board info to LPM directly
   gm_WriteRegWord(MISSION_LPM_DATA_OUT_12, GetCurrentBoardInfo());

   gm_WriteRegWord(MISSION_LPM_DATA_OUT_13, GetIrPowerKeyVal(Key_KEY_PHYSICAL_POWER));

   // Send Product ID and Revision 
   gm_WriteRegWord(MISSION_LPM_DATA_OUT_14, gm_ReadRegWord(PRODUCT_ID));
   gm_WriteRegWord(MISSION_LPM_DATA_OUT_15, gm_ReadRegWord(PRODUCT_REV));
   
   //Send Project info to LPM
   gm_WriteRegWord(MISSION_LPM_DATA_OUT_19, GetProjIDInfo());

   //Finally, fill IPC message ID to the first WORD to be recognized by LPM!!
   gm_WriteRegWord(MISSION_LPM_DATA_OUT_0, gmd_MESSAGE_LPM_GPIO_PINOUT);
      
#else
   BYTE LPMGPIO_Config[13];

   //=============================================================
   //LPM GPIO control, shared PBus
   // GPIO0(Bit0): ST sensor INT 1	GPIO1: VGA DDC SDA	GPIO2: VGA DDC SCL GPIO3: ST power module ctrl
   // GPIO4: CEC	GPIO5: CP2 DP Cable det	GPIO6: CP0 DP Power det
   // GPIO7: CP0 cable det	GPIO8: CP2 DP HPD	GPIO9: CP1 HDMI HPD
   // GPIO10: CP0 DP HPD	GPIO11: Not use	GPIO12: Not use
   // GPIO13: CP1 HDMI DDC SDA	GPIO14: CP1 HDMI DDC SCL
   // GPIO15(bit0): LPM master I2C SCL	// GPIO16: LPM master I2C SDA
   // GPIO17: DP TX HPD in	GPIO18: Mission power ctr
   // GPIO19: Not use		GPIO20: LED Amber

   //LPM_GPADC control
   // ADC_AIN0: Keypad_in1	ADC_AIN2: Keypad2	ADC_AIN3: VGA / DVI cable det
   // ADC_AIN4: Not use

   //=============================================================
   // RD1 Board LPM GPIO Config
   // TYPE :
   //		00: Special function
   //		01: GPIO_IN
   //		10: GPIO_OUT
   //		11: Not Use pin
   //=============================================================

   //GPIO0 - GPIO3	10 00 00 00
   //GPIO0:ST touch sensor device INT_IN
   //GPIO1:LPM UART UDO
   //GPIO2:LPM UART UDI
   //GPIO3:ST Power module status control
#if (Athena_4Kx2K_Function == 1 && defined(Athena_4K2K_Chip_C) && ENABLE_SOC_IPC_SLAVE_DEV_COMM)
	LPMGPIO_Config[0] = 0x82; //LPM_GPIO0 as Output pin for Athena C control Athena B INT_2 pin
#else //Original
   LPMGPIO_Config[0] = 0x80;
#endif
   //GPIO4 - GPIO7	00 00 00 00
   //GPIO4:CEC
   //GPIO5:CP2 AUX_P CABLE DETECT
   //GPIO6:CP0 AUX_N HOST POWER DETECT
   //GPIO7:CP0 AUX_P CABLE DETECT
   LPMGPIO_Config[1] = 0x00;

   //GPIO8 - GPIO11		00 00 00 00
   //GPIO08:HPD3 set to 00 due to PBus own is Mission
   //GPIO09:HPD2 set to 00 due to PBus own is Mission
   //GPIO10:HPD1 set to 00 due to PBus own is Mission
   //GPIO11:SLV3_SCL
   LPMGPIO_Config[2] = 0x00;

   //GPIO12 - GPIO15	00 00 00 00
   //GPIO12:SLV3_SDA
   //GPIO13:SLV2_SCL
   //GPIO14:SLV2_SDA
   //GPIO15:LPM Master I2C - SCL
   LPMGPIO_Config[3] = 0x00;

   //GPIO16 - GPIO19	10 00 00 00
   //GPIO16:LPM Master I2C- SDA
   //GPIO17:DPTX HPD
   //GPIO18:MAIN POWER ON
   //GPIO19:External TMDS switch IC control
   LPMGPIO_Config[4] = 0x80;

   //GPIO20 	00 00 00 10
   //GPIO20:LED Amber
   LPMGPIO_Config[5] = 0x02;

   //How many gpADC channel use  ==> 00 00 00 11 ==>0x03
   //GPADC0 : Keypad
   //GPADC1 : Keypad
   //GPADC2 : HDMI & VGA Cable detect
   //GPADC3 : GPIO: On/off TMDS switch power spply, need set in LPM
   LPMGPIO_Config[6] = 0x03;

   //Power Key deifinition ADC channel (0 - 3)
   LPMGPIO_Config[7] = 0x00;

   //Power Key deifinition ADC value (0-255)
   LPMGPIO_Config[8] = 0x26;

   //ST touch sensor deifinition SLV channel (1 - 4) 0: No Touch sensor
   LPMGPIO_Config[9] = 0x01;	// SLV1

   //ST touch sensor deifinition INT GPIO (0 - 20)
   LPMGPIO_Config[10] = 0x00;	// GPIO0

   //Send board info to LPM directly
   LPMGPIO_Config[11] = GetCurrentBoardInfo();

   LPMGPIO_Config[12] = GetIrPowerKeyVal(Key_KEY_PHYSICAL_POWER);

   gm_MessageSend(gmd_MESSAGE_LPM_GPIO_PINOUT, &LPMGPIO_Config[0], 13);
#endif   
}


#if (FEATURE_DISPLAY_PORT == ENABLE) && ENABLE_HPD_OVERRIDED
void ForceDpHpd2Low(gmt_DPRX_DID b_DID, BOOL b_Set2Low)
{
   if (b_DID==DID_DPRX0)
   {
      if (b_Set2Low==TRUE)
         gm_SetRegBitsByte(LPM_CSM_HPD_OUT_CTRL,HPD1_SOURCE_SEL);
      else
         gm_ClearAndSetBitsByte(LPM_CSM_HPD_OUT_CTRL,HPD1_SOURCE_SEL,0x1);
   }
}
#endif

#ifdef MUX_TEST_BOARD
void EXT_FUNC Dp1ExtPortSetup_ID0(void)
{
//   Place Mux Control code here
//   gm_Print("Dp1ExtPortSetup_ID0",0);
   Input_DP_Switch_Select(DP_NORMAL);
}
void EXT_FUNC Dp1ExtPortSetup_ID1(void)
{
//   Place Mux Control code here
//   gm_Print("Dp1ExtPortSetup_ID1",0);
   Input_DP_Switch_Select(DP_MINI);
}


#endif

/*********************************  END  **************************************/
