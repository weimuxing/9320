/*
	$Workfile:   GpioControl.c  $
	$Revision: 1.26 $
	$Date: 2013/04/22 06:06:33 $
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
#define I2C_800K_FREQ		(800000L)
#define I2C_800K_SCALE		(((gDW_SystemOcmClk / I2C_800K_FREQ) / 5) - 2)

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
static BYTE b_ChipID_4K2K=CHIPID_4K2K_NONE;
void ChipIDInit_4K2K(void)
{
   //GPIO79
   gm_SetRegBitsWord(GPIO_4_ENABLE,GPIO79_EN);
   gm_ClearRegBitsWord(GPIO_4_DIRCTRL,GPIO79_IO);

   //GPIO80
   gm_SetRegBitsWord(GPIO_5_ENABLE,GPIO80_EN);
   gm_ClearRegBitsWord(GPIO_5_DIRCTRL,GPIO80_IO);
   if(gm_ReadRegWord(GPINPUT4) & GPIO79_IN)
   {
      if(gm_ReadRegWord(GPINPUT5) & GPIO80_IN)
         b_ChipID_4K2K=CHIPID_4K2K_R;
      else
         b_ChipID_4K2K=CHIPID_4K2K_L;
   }
   else
   {
      if(gm_ReadRegWord(GPINPUT5) & GPIO80_IN)
         b_ChipID_4K2K=CHIPID_4K2K_FE;
      else
         b_ChipID_4K2K=CHIPID_4K2K_NONE;
   }
}


BYTE GetChipID_4K2K(void)
{
   return b_ChipID_4K2K;
}



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
#if (ENABLE_ATHENA_FE_LEFT_INT1_HANDLE || ENABLE_ATHENA_RIGHT_INT1_HANDLE) // Modified by ChengLin 20130124 
	//Don't enable SCBL for INT1 pin
	//gm_SetRegBitsWord(MFP_SPARE2,BIT1);
	gm_WriteRegWord(MFP_SPARE2, 0x0);
#else //Original
   gm_SetRegBitsWord(MFP_SPARE2,(BIT1 | BIT4));
#endif

   // GPIO77(13): UDO0	GPIO78(14): UDO0		GPIO79(15): LED_Green
   //gm_SetRegBitsWord(GPIO_4_DIRCTRL, GPIO79_IO);
   //gm_SetRegBitsWord(GPIO_4_ENABLE, GPIO79_EN);
   

   // GPIO80(Bit0): DIN connect detect	GPIO81(Bit1): AUD_Main_Mute_Ctrl
   // GPIO82(Bit2): Audio value ctrl		GPIO83(Bit3): audio board INT in
   // GPIO84(Bit4): Panel backlight ctrl	GPIO85(Bit5): HDMI board INT in
   // GPIO86(Bit6): ext amp. standby ctrl	GPIO87(Bit7): Not use
   // GPIO88(Bit8): /SRESET			GPIO89(Bit9): Not use 

   if(GetChipID_4K2K()==CHIPID_4K2K_R)
   {
#if ENABLE_VEGA_ISP_CMD_VIA_UART_ISP_PORT    
/*ChengLin 20130412: Enable GPIO66 for VEGA ISP "BS_4" pin control*/
   gm_SetRegBitsWord(GPIO_4_ENABLE, GPIO66_EN);
   gm_SetRegBitsWord(GPIO_4_DIRCTRL, GPIO66_IO); //GPIO66 is output mode
   gm_ClearRegBitsWord(GPOUTPUT4,GPIO66_OUT); //GPIO66 to Low
#endif
   
/*ChengLin 20130124: New 4k2k board will use GPIO95/94 as GPIO Input for Busy status check*/   
#if (ENABLE_SOC_IPC_BUSY_CHECK)   
   	// Busy pins
	gm_SetRegBitsWord(GPIO_5_ENABLE, GPIO95_EN);
	gm_SetRegBitsWord(GPIO_5_ENABLE, GPIO94_EN);

/*ChengLin 20130129: Enable Bidirectional BUSY Status Check*/	
#if(ENABLE_BIDIRECTIONAL_BUSY_CHECK)
	gm_SetRegBitsWord(GPIO_5_OPENDRAIN_EN, GPIO95_OD); // For Athena L Open Drain Output
	gm_SetRegBitsWord(GPIO_5_OPENDRAIN_EN, GPIO94_OD); // For Athena FE Open Drain Output
	gm_SetRegBitsWord(GPIO_5_DIRCTRL, GPIO95_IO); // For Athena L Output Mode
	gm_SetRegBitsWord(GPIO_5_DIRCTRL, GPIO94_IO); // For Athena FE Output Mdoe
	gm_SetRegBitsWord(GPOUTPUT5,GPIO95_OUT); //Set Athena L to Ready status	
	gm_SetRegBitsWord(GPOUTPUT5,GPIO94_OUT); //Set Athena FE to Ready status	
#else //Original
   	gm_ClearRegBitsWord(GPIO_5_DIRCTRL, GPIO95_IO);	// For Athena L Input Mode
   	gm_ClearRegBitsWord(GPIO_5_DIRCTRL, GPIO94_IO); // For Athena FE Input Mode
#endif   	
#endif

/*ChengLin 20130321: Initial GPIO12 as open-drain output, default is "High" for VEGA reset pin*/
      gm_SetRegBitsWord(GPIO_0_ENABLE, (GPIO6_EN | GPIO9_EN | GPIO12_EN));
      gm_SetRegBitsWord(GPIO_0_OPENDRAIN_EN, GPIO12_OD);
      gm_SetRegBitsWord(GPIO_0_DIRCTRL, (GPIO6_OUT| GPIO9_OUT |GPIO12_OUT));
      gm_SetRegBitsWord(GPOUTPUT0, GPIO12_OUT); //VEGA Reset pin     
      
      //GPIO80 As Output for Control Athena C INT1
     // gm_SetRegBitsWord(GPIO_5_DIRCTRL,GPIO80_IO);
      
/*ChengLin 20130219: Configure GPIO 82 to output mode and open drain for image sync handler*/	  
#if ENABLE_ATHENA_R_L_IMAGE_SYNC_HANDLER
      gm_SetRegBitsWord(GPIO_5_ENABLE, GPIO82_EN);
	  gm_SetRegBitsWord(GPIO_5_OPENDRAIN_EN, GPIO82_OD); //GPIO82 open drain
	  gm_SetRegBitsWord(GPIO_5_DIRCTRL, GPIO82_IO); //GPIO82 as output mode
          gm_ClearRegBitsWord(GPOUTPUT5,GPIO82_OUT); //Set GPIO82 to low level for LVDS_SYNC_NOT_READY

#else //Orignal
      gm_SetRegBitsWord(GPIO_5_ENABLE, GPIO82_EN);
      gm_ClearRegBitsWord(GPIO_5_DIRCTRL, GPIO82_IO);
#endif
      
      // Led Green
      gm_SetRegBitsWord(GPIO_1_ENABLE, GPIO19_EN);
      gm_SetRegBitsWord(GPIO_1_DIRCTRL,GPIO19_IO); 
	  
#if (ENABLE_ATHENA_RIGHT_INT1_HANDLE)      
      gm_ClearRegBitsWord(GPIO_5_ENABLE, GPIO85_EN); // 85: Int
#endif

	  gm_SetRegBitsWord(LPM_GPIO_ENABLE2,LPM_GPIO19_EN);
      gm_SetRegBitsWord(LPM_GPIO_DIRCTRL2,LPM_GPIO19_IO);
   	
	}
	else if(GetChipID_4K2K()==CHIPID_4K2K_FE)
	{
/*ChengLin 20130124: New 4k2k board Athena FE will use GPIO94 as GPIO output for Busy status check*/   
#if (ENABLE_SOC_IPC_BUSY_CHECK)   
   	gm_SetRegBitsWord(GPIO_5_ENABLE,GPIO94_EN);
	/*ChengLin 20130129: Enable Bidirectional BUSY Status Check*/	
#if(ENABLE_BIDIRECTIONAL_BUSY_CHECK)
	gm_SetRegBitsWord(GPIO_5_OPENDRAIN_EN, GPIO94_OD); // For Athena FE Open Drain Output
#endif
   	gm_SetRegBitsWord(GPIO_5_DIRCTRL, GPIO94_IO);
	gm_SetRegBitsWord(GPOUTPUT5,GPIO94_OUT); //Set Athena FE to Ready status
#endif
	
/*ChengLin 20130123: Set GPIO83 to output mode for control Athena R INT2 interrupt*/
#if ENABLE_SOC_IPC_LEFT_RIGHT_SLAVE_INT2_FROM_FE_MASTER
	gm_SetRegBitsWord(GPIO_5_DIRCTRL, GPIO83_IO);
#endif
   }
   else if(GetChipID_4K2K()==CHIPID_4K2K_L)
   {
	   /*ChengLin 20130124: New 4k2k board Athena LEFT will use GPIO95 as GPIO output for Busy status check*/   
#if (ENABLE_SOC_IPC_BUSY_CHECK)  
	gm_SetRegBitsWord(GPIO_5_ENABLE, GPIO95_EN);
		/*ChengLin 20130129: Enable Bidirectional BUSY Status Check*/	
#if(ENABLE_BIDIRECTIONAL_BUSY_CHECK)
	gm_SetRegBitsWord(GPIO_5_OPENDRAIN_EN, GPIO95_OD); // For Athena L Open Drain Output
#endif
	gm_SetRegBitsWord(GPIO_5_DIRCTRL, GPIO95_IO);
	gm_SetRegBitsWord(GPOUTPUT5,GPIO95_OUT); //Set Athena L to Ready status
#endif

	gm_SetRegBitsWord(GPIO_5_ENABLE, (GPIO84_EN | GPIO85_EN)); // 84: Panel sync; 85 Int to R
/*ChengLin 20130219: Configure GPIO 84 to open drain for image sync handler*/	  
#if ENABLE_ATHENA_R_L_IMAGE_SYNC_HANDLER
	gm_SetRegBitsWord(GPIO_5_OPENDRAIN_EN, GPIO84_OD); //GPIO84 open drain
#endif
	gm_SetRegBitsWord(GPIO_5_DIRCTRL, (GPIO84_EN | GPIO85_EN));
	gm_ClearRegBitsWord(GPOUTPUT5, (GPIO84_OUT | GPIO85_OUT));
	
    gm_SetRegBitsWord(LPM_GPIO_ENABLE2,LPM_GPIO19_EN);
    gm_ClearRegBitsWord(LPM_GPIO_DIRCTRL2,LPM_GPIO19_IO);
   } 

//ChengLin 20130123: Disable GPIO83 for enable INT 2 on Athena R and Left
#if ENABLE_SOC_IPC_LEFT_RIGHT_SLAVE_INT2_FROM_FE_MASTER
	if(GetChipID_4K2K() == CHIPID_4K2K_R)
   {  
		gm_SetRegBitsWord(GPIO_5_ENABLE, (GPIO80_EN | GPIO81_EN | GPIO86_EN | GPIO88_EN));
   }
   else if(GetChipID_4K2K() == CHIPID_4K2K_L)
   {
		gm_SetRegBitsWord(GPIO_5_ENABLE, (GPIO80_EN | GPIO81_EN | GPIO86_EN));
   }
	else
   {  
      //ChengLin: Athena FE don't to enable GPIO86 & GPIO88
      gm_SetRegBitsWord(GPIO_5_ENABLE, (GPIO80_EN | GPIO81_EN | GPIO83_EN));
   }
#else
   gm_SetRegBitsWord(GPIO_5_ENABLE, (GPIO80_EN | GPIO81_EN | GPIO83_EN | GPIO86_EN | GPIO88_EN));
#endif
	gm_SetRegBitsWord(GPIO_5_DIRCTRL, (GPIO81_IO | GPIO86_IO | GPIO88_IO));
 
   
#ifdef USE_SECOND_UART
   // UART1 share pins (GPIO 79 & 80) with green LED and DIP connect detect
   gm_ClearRegBitsWord(GPIO_4_ENABLE, GPIO79_EN);
   gm_ClearRegBitsWord(GPIO_5_ENABLE, GPIO80_EN);
#endif

#ifdef EXTERNAL_DECODER
//   gm_SetRegBitsWord(GPOUTPUT5,GPIO88_OUT);
#endif

   // GPIO92 SPI-flash Hold. Enable GPIO function to override ENABLE_30BIT_OUTPUT 
   gm_SetRegBitsWord(GPIO_5_ENABLE,GPIO92_EN);
   gm_ClearRegBitsWord(GPIO_5_DIRCTRL,GPIO92_IO);

#if 1 // Disable write protect // ToDo: need implement write protect control function then enable it.
   // GPIO93 SPI-flash WP (0:Enable WP/1:Disable WP)
   gm_SetRegBitsWord(GPIO_5_ENABLE,GPIO93_EN);
   gm_SetRegBitsWord(GPIO_5_DIRCTRL,GPIO93_IO);
   gm_SetRegBitsWord(GPOUTPUT5,GPIO93_OUT);

   // GPIO90 NVRAM WP (0:Disable WP/1:Enable WP)
   gm_SetRegBitsWord(GPIO_5_ENABLE,GPIO90_EN);
   gm_SetRegBitsWord(GPIO_5_DIRCTRL,GPIO90_IO);
   gm_ClearRegBitsWord(GPOUTPUT5,GPIO90_OUT);
#if 1 // Disable EDID write protect
   // GPIO91 EDID WP (0:Disable WP/1:Enable WP)
   if(GetChipID_4K2K() == CHIPID_4K2K_R)
   {
      gm_SetRegBitsWord(GPIO_5_ENABLE, GPIO91_EN);
      gm_SetRegBitsWord(GPIO_5_DIRCTRL, GPIO91_IO);
      gm_ClearRegBitsWord(GPOUTPUT5, GPIO91_OUT);
   }
#endif
#endif

/*
   // LPM GPIO5 HDMI switch TMDS_EN (1:Power on/0:Power off)
   gm_SetRegBitsWord(LPM_GPIO_ENABLE1,LPM_GPIO5_EN);
   gm_SetRegBitsWord(LPM_GPIO_DIRCTRL1,LPM_GPIO5_IO);
   gm_SetRegBitsWord(LPM_GPOUTPUT_0,LPM_GPIO5_OUT);
*/

   // LPM GPIO19 HDMI switch TMDS_SEL (0:HDMI/1:DVI)
   //gm_SetRegBitsWord(LPM_GPIO_ENABLE2,LPM_GPIO19_EN);
   //gm_SetRegBitsWord(LPM_GPIO_DIRCTRL2,LPM_GPIO19_IO);
   //gm_ClearRegBitsWord(LPM_GPOUTPUT_1,LPM_GPIO19_OUT);

   //gm_Print("LPM GPIO 7 set",0);
   //gm_SetRegBitsWord(LPM_GPIO_ENABLE1, (BIT0 | BIT3/* | LPM_GPIO7_EN*/));
   //gm_SetRegBitsWord(LPM_GPIO_DIRCTRL1, BIT3);

   gm_SetRegBitsWord(LPM_GPOUTPUT_1,LPM_GPIO20_OUT);
   gm_SetRegBitsWord(LPM_GPIO_DIRCTRL2, LPM_GPIO20_IO);
   gm_SetRegBitsWord(LPM_GPIO_ENABLE2, LPM_GPIO20_EN);

   if(GetChipID_4K2K()==CHIPID_4K2K_R)
   {
      //DP_MUX_EN Control and DP_MUX_SEL
      gm_SetRegBitsWord(LPM_GPIO_ENABLE1, LPM_GPIO0_EN | LPM_GPIO13_EN | LPM_GPIO14_EN);	// LPM GPIO0 as output for FE comfirm DC or Sleep mode
      gm_SetRegBitsWord(LPM_GPIO_DIRCTRL1,LPM_GPIO0_IO | LPM_GPIO13_IO | LPM_GPIO14_IO);
      gm_SetRegBitsWord(LPM_GPOUTPUT_0,LPM_GPIO13_OUT|LPM_GPIO14_OUT);

		gm_SetRegBitsWord(LPM_GPIO_ENABLE2, LPM_GPIO19_EN);
      gm_ClearRegBitsWord(LPM_GPIO_DIRCTRL2,LPM_GPIO19_IO);	// LPM GPIO19 as input for FE wake up signal
   }
	else if(GetChipID_4K2K()==CHIPID_4K2K_FE)
   {
      gm_SetRegBitsWord(LPM_GPIO_ENABLE1, LPM_GPIO0_EN);
		gm_ClearRegBitsWord(LPM_GPIO_DIRCTRL1,LPM_GPIO0_IO);

		gm_SetRegBitsWord(LPM_GPIO_ENABLE2, LPM_GPIO19_EN);
      gm_SetRegBitsWord(LPM_GPIO_DIRCTRL2,LPM_GPIO19_IO);
   }

   // Backlight control
   gm_SetRegBitsWord(PWM3_CONFIG, PWM3_GPIOn_EN);

   // OCM_INT1 init
   //ChengLin 20130123: Only Athena Rright and Left will use PWM2 as INT2
#if (ENABLE_SOC_IPC_LEFT_RIGHT_SLAVE_INT2_FROM_FE_MASTER)   
   if(GetChipID_4K2K() != CHIPID_4K2K_FE)
#endif   	
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
   if(GetChipID_4K2K()!=CHIPID_4K2K_FE)
		gm_WriteRegWord(LPM_CSM_HPD_OUT_CTRL,0x0117);   
	else		
	  	gm_WriteRegWord(LPM_CSM_HPD_OUT_CTRL,0x0217);   // 0000 0010 0001 0111  SWENG0622: Force DP0 HPD to low during system initial
#else
	if(GetChipID_4K2K()!=CHIPID_4K2K_FE)
		gm_WriteRegWord(LPM_CSM_HPD_OUT_CTRL,0x0111);   
	else
   	gm_WriteRegWord(LPM_CSM_HPD_OUT_CTRL,0x0211);	// 0000 0010 0001 0001
#endif
   gm_WriteRegWord(LPM_CSM_HPD_PD_IN_CTRL,0x00FF);

   if (gm_ReadRegWord(LPM_MISSION_STATUS) & LP_SRAM_LOADED)
   {
      //LPM_C register can't RW by Mission directly after LPM starts up
      WORD msg[3] = {Clear_bit, LPM_GPIO_ENABLE1, LPM_GPIO8_EN|LPM_GPIO9_EN|LPM_GPIO10_EN};//3: clear bits
      gm_MessageSend(gmd_MESSAGE_LPM_GPIO_CTRL, &msg, sizeof(msg));
   }
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
	/*ChengLin 20130129: I2C Mater 0 for NVRAM, I2C Speed keep at 100Kbps
					      I2C Mater 1 for SPCIPC, I2C Speed change to  800Kbps
	*/
	if(GetChipID_4K2K() == CHIPID_4K2K_R)
	{
		//MSTR0 NVRAM 100Kbps
		gm_WriteRegWord(I2C_MST_CTR, I2C_EN | I2C_SDA_SCL0_EN);
		gm_WriteRegWord(I2C_MST_CLK_SCALE, I2C_100K_SCALE);

		//MSTR1 SOCIPC 800Kbps
		gm_WriteRegWord(I2C2_MST_CTR, I2C_EN | I2C_SDA_SCL1_EN);
		gm_WriteRegWord(I2C2_MST_CLK_SCALE, I2C_800K_SCALE);		
	}
	else
	{
		gm_WriteRegWord(I2C_MST_CTR, I2C_EN | I2C_SDA_SCL0_EN | I2C_SDA_SCL1_EN);
		gm_WriteRegWord(I2C_MST_CLK_SCALE, I2C_100K_SCALE);
	}
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
	if(GetChipID_4K2K()==CHIPID_4K2K_R)
	{
      if (B_State == TRUE)
      {
         	gm_SetRegBitsWord(GPOUTPUT1, GPIO19_OUT);
      }
      else
      {
   			gm_ClearRegBitsWord(GPOUTPUT1, GPIO19_OUT);
      }
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
      msg_data[0] = Clear_bit;
      gm_MessageSend(gmd_MESSAGE_LPM_GPIO_CTRL, msg_data, sizeof(msg_data));
   }
   else
   {
      msg_data[0] = Set_bit;
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

#define LBADC_DELTA                  8 // Tolerance
#define LBADC_DVI_CABLE            238 // ADC value for DVI cable plugged only
#define LBADC_VGA_CABLE            205 // ADC value for VGA cable plugged only
#define LBADC_DVI_5V_CABLE         187 // ADC value for DVI cable plugged+DVI_5V valid only
#define LBADC_VGA_5V_CABLE         126 // ADC value for DVI cable plugged & VGA cable plugged+VGA_5V valid
#define LBADC_DVI_VGA_CABLE        170 // ADC value for DVI cable plugged & VGA cable plugged
#define LBADC_DVI_VGA_5V_CABLE     112 // ADC value for DVI cable plugged & VGA cable plugged+VGA_5V valid
#define LBADC_DVI_5V_VGA_CABLE     142 // ADC value for DVI cable plugged+DVI_5V valid & VGA cable plugged
#define LBADC_DVI_5V_VGA_5V_CABLE   99 // ADC value for DVI cable plugged+DVI_5V valid & VGA cable plugged+VGA_5V valid
#define LBADC_HDMI_CABLE           210 // ADC value for HDMI cable plugged only
#define LBADC_HDMI_5V_CABLE        158 // ADC value for HDMI cable plugged+HDMI_5V valid

#define LBADC_EQUAL(a,b) ((a) < (b)+LBADC_DELTA) && ((a) >  (b)-LBADC_DELTA)
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

   if(GetChipID_4K2K()==CHIPID_4K2K_R)
   {
      //For Athena_R, PHY3 (for DpRx1) has not cable/Source detected, always return TRUE
      CableUpdate->PHY3_CableStatus = TRUE;
      CableUpdate->PHY3_SourcePowerStatus = TRUE;

      //For Athena_R, Original PHY3 cable/SrcPwr detection is connected for MiniDp
      if (!(gm_ReadRegByte(LPM_CSM_HPD_PD_IN_STATUS) & PD3_IN_STATUS))
      {
         CableUpdate->MiniDP_CableStatus= TRUE;
      }
      else
      {
         CableUpdate->MiniDP_CableStatus = FALSE;
      }
      
      if (gm_ReadRegByte(LPM_CSM_HPD_PD_IN_STATUS) & PD4_IN_STATUS)
      {
         CableUpdate->MiniDP_SourcePowerStatus = TRUE;
      }
      else
      {
         CableUpdate->MiniDP_SourcePowerStatus = FALSE;
      }   

   }
   else if(GetChipID_4K2K()==CHIPID_4K2K_L)
   {
      if (!(gm_ReadRegByte(LPM_CSM_HPD_PD_IN_STATUS) & PD3_IN_STATUS))
      {
         CableUpdate->PHY3_CableStatus = TRUE;
      }
      else
      {
         CableUpdate->PHY3_CableStatus = FALSE;
      }

      if (gm_ReadRegByte(LPM_CSM_HPD_PD_IN_STATUS) & PD4_IN_STATUS)
      {
         CableUpdate->PHY3_SourcePowerStatus = TRUE;
      }
      else
      {
         CableUpdate->PHY3_SourcePowerStatus = FALSE;
      }   
   }

/*
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
*/
   // HDMI
#if 0    
   if( LBADC_EQUAL(LPM_CableStatus[6],LBADC_HDMI_CABLE) 
      || LBADC_EQUAL(LPM_CableStatus[6],LBADC_HDMI_5V_CABLE) )
      CableUpdate->PHY3_CableStatus = TRUE;
   else
      CableUpdate->PHY3_CableStatus = FALSE;

   if( LBADC_EQUAL(LPM_CableStatus[6],LBADC_HDMI_5V_CABLE) )
      CableUpdate->PHY3_SourcePowerStatus = TRUE;
   else
      CableUpdate->PHY3_SourcePowerStatus = FALSE;
#endif      
/*
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
*/
}

void LPM_GPIO_Config(void)
{
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
   gm_WriteRegWord(MISSION_LPM_DATA_OUT_1, 0x00);
   //GPIO4 - GPIO7   
   gm_WriteRegWord(MISSION_LPM_DATA_OUT_2, 0x00);
   //GPIO8 - GPIO11 
   gm_WriteRegWord(MISSION_LPM_DATA_OUT_3, 0x00);

   //GPIO12 - GPIO15 
   if(GetChipID_4K2K()==CHIPID_4K2K_R)    
      gm_WriteRegWord(MISSION_LPM_DATA_OUT_4, 0x28); //00 10 10 00 //For DP_MUX_EN and SEL
   else
      gm_WriteRegWord(MISSION_LPM_DATA_OUT_4, 0x00);

   //GPIO16 - GPIO19 
   gm_WriteRegWord(MISSION_LPM_DATA_OUT_5, 0x80);
   //GPIO20    
   gm_WriteRegWord(MISSION_LPM_DATA_OUT_6, 0x02);
   //GPADC0 - GPADC3 
   gm_WriteRegWord(MISSION_LPM_DATA_OUT_7, 0x04);


   //LPM_GPADC control
   // ADC_AIN0: Keypad_in1 ADC_AIN2: Keypad2 ADC_AIN3: VGA / DVI cable det
   // ADC_AIN4: Used for HDMI cable det on RD3

   //Power Key deifinition ADC channel (0 - 3)
   gm_WriteRegWord(MISSION_LPM_DATA_OUT_8, 0x00|(LPM_FORCE_POWER_DOWN<<8));
   //Power Key deifinition ADC value (0-255)
   gm_WriteRegWord(MISSION_LPM_DATA_OUT_9, 0x26);
   //ST touch sensor deifinition SLV channel (1 - 4) 0: No Touch sensor
   gm_WriteRegWord(MISSION_LPM_DATA_OUT_10, 0x01); //SLV1
   //ST touch sensor deifinition INT GPIO (0 - 20)
   gm_WriteRegWord(MISSION_LPM_DATA_OUT_11, 0x00); //GPIO0

   //Send board info to LPM directly
   gm_WriteRegWord(MISSION_LPM_DATA_OUT_12, (GetChipID_4K2K()<<4)|GetCurrentBoardInfo());   //4 

   gm_WriteRegWord(MISSION_LPM_DATA_OUT_13, GetIrPowerKeyVal(Key_KEY_PHYSICAL_POWER));

   // Send Product ID and Revision 
   gm_WriteRegWord(MISSION_LPM_DATA_OUT_14, gm_ReadRegWord(PRODUCT_ID));
   gm_WriteRegWord(MISSION_LPM_DATA_OUT_15, gm_ReadRegWord(PRODUCT_REV));

   //Finally, fill IPC message ID to the first WORD to be recognized by LPM!!
   gm_WriteRegWord(MISSION_LPM_DATA_OUT_0, gmd_MESSAGE_LPM_GPIO_PINOUT); 
}

// 20130204, Config LPM GPIO 0 for confirm power down event, 1: DC off 0: no sync
#ifdef ST_4K2K_93xx_BOARD
void LPM_Set_DCFlag(BOOL B_State)
{
   WORD msg_data[3];

   msg_data[1] = LPM_GPOUTPUT_0;
   msg_data[2] = LPM_GPIO0_OUT;

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

	//gm_Print("Set DC Flag state %d", B_State);
}
#endif

#if (FEATURE_DISPLAY_PORT == ENABLE) && ENABLE_HPD_OVERRIDED
void ForceDpHpd2Low(gmt_DPRX_DID b_DID, BOOL b_Set2Low)
{
   if (b_DID==DID_DPRX0)
   {
      //gm_Print("ForceDpHpd2Low(%d)",b_Set2Low);
      if (b_Set2Low==TRUE)
         gm_SetRegBitsByte(LPM_CSM_HPD_OUT_CTRL,HPD1_SOURCE_SEL);
      else
         gm_ClearAndSetBitsByte(LPM_CSM_HPD_OUT_CTRL,HPD1_SOURCE_SEL,0x1);
   }
}
#endif

#ifdef MUX_TEST_BOARD
static void Input_DP_Switch_Select(BYTE B_DP_ID)
{
   if(GetChipID_4K2K()==CHIPID_4K2K_R)
   {
      WORD msg_data[3];
      msg_data[1] = LPM_GPOUTPUT_0;
      msg_data[2] = LPM_GPIO14_OUT;

      if (B_DP_ID == MUX_SELECTOR_ID0)
      {
         //gm_Print("LPM GPIO14 : 1 => DP_NORMAL",0);

         msg_data[0] = Set_bit;
         
        if(gm_MessageSend(gmd_MESSAGE_LPM_GPIO_CTRL, msg_data, sizeof(msg_data)) == gmd_FALSE)
         {
            static DWORD DW_BreakTime ;
            DW_BreakTime = gm_GetSystemTime();
            while(1)
            {
               if (abs((WORD)(gm_GetSystemTime() - DW_BreakTime)) > 500)
                  break;
               if(gm_MessageSend(gmd_MESSAGE_LPM_GPIO_CTRL, msg_data, sizeof(msg_data)))
                  break;
            }
        }
      }
      else // MUX_SELECTOR_ID1; Mini DP
      {
        //gm_Print("LPM GPIO14 : 0 => DP_MINI",0);
         
        msg_data[0] = Clear_bit;
            
        if(gm_MessageSend(gmd_MESSAGE_LPM_GPIO_CTRL, msg_data, sizeof(msg_data)) == gmd_FALSE)
        {
            static DWORD DW_BreakTime ;
            DW_BreakTime = gm_GetSystemTime();
            while(1)
            {
               if (abs((WORD)(gm_GetSystemTime() - DW_BreakTime)) > 500)
                  break;
               if(gm_MessageSend(gmd_MESSAGE_LPM_GPIO_CTRL, msg_data, sizeof(msg_data)))
                  break;
            }
        }
      }
   }
//   gm_Delay10ms(1);
}


void EXT_FUNC Dp1ExtPortSetup_ID0(void)
{
//   Place Mux Control code here
//   gm_Print("Dp1ExtPortSetup_ID0",0);
   Input_DP_Switch_Select(MUX_SELECTOR_ID0);

}
void EXT_FUNC Dp1ExtPortSetup_ID1(void)
{
//   Place Mux Control code here
//   gm_Print("Dp1ExtPortSetup_ID1",0);
   Input_DP_Switch_Select(MUX_SELECTOR_ID1);
}
#endif

/*********************************  END  **************************************/
