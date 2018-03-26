
#ifndef __CUSTOM_APP_H__
#define __CUSTOM_APP_H__

//******************************************************************************
// B A C K L I G H T    P W M    S E T T I N G S
//******************************************************************************

#if (DEFAULT_PANEL == SAMSUNG_WXGA_LTI260AP01)
	#define BACKLIGHT_CONTROL_GPIO		ENABLE//DISABLE -->PWM cotrol; ENABLE -->GPIO, set to max brightness;
#else
	#define BACKLIGHT_CONTROL_GPIO		DISABLE//DISABLE -->PWM cotrol; ENABLE -->GPIO, set to max brightness;
#endif

//===========Backlight PWM setting=================
#define DEFAULT_VIDEO_BACKLIGHT_PWM	MAX_PWMPERIOD	// Modified by ST_hansPENG 090225
#define BACKLIGHT_B_PWM_CONFIG			PWM0_CONFIG
#define BACKLIGHT_B_PWM_CTRL			PWM0_CTRL
#define BACKLIGHT_B_PWM_CTRL_MSB		PWM0_CTRL_MSB

#define BACKLIGHT_G_PWM_CONFIG			PWM3_CONFIG
#define BACKLIGHT_G_PWM_CTRL			PWM3_CTRL
#define BACKLIGHT_G_PWM_CTRL_MSB		PWM3_CTRL_MSB

#define BACKLIGHT_DCR_PWM_CONFIG		PWM2_CONFIG
#define BACKLIGHT_DCR_PWM_CTRL			PWM2_CTRL
#define BACKLIGHT_DCR_PWM_CTRL_MSB		PWM2_CTRL_MSB


/*
#if defined(CMI_73XX_BOARD)//120503 Edward for different board
#define BACKLIGHT_PWM_CONFIG		PWM2_CONFIG
#define BACKLIGHT_PWM_CTRL			PWM2_CTRL
#else
#define BACKLIGHT_PWM_CONFIG		PWM3_CONFIG
#define BACKLIGHT_PWM_CTRL			PWM3_CTRL
#endif

#define ReadBacklightPWMPulse()				((BYTE)(gm_ReadRegWord(BACKLIGHT_B_PWM_CTRL) >> 8))
#define SetBacklightPWMPulse(pulse)			(gm_ClearAndSetBitsWord(BACKLIGHT_B_PWM_CTRL, PWM3_PULSE, (WORD)pulse << 8))
*/

//120523 Edward use below replace all DCR function
#define ReadBlueBacklightPWMPulse()         ((WORD)(gm_ReadRegWord(BACKLIGHT_B_PWM_CTRL) >> 8) + (gm_ReadRegWord(BACKLIGHT_B_PWM_CTRL_MSB) & 0x0F00))
#define SetBlueBacklightPWMPulse(pulse)		(gm_ClearAndSetBitsWord(BACKLIGHT_B_PWM_CTRL, PWM0_PULSE, (WORD)pulse << 8))
#define ReadGreenBacklightPWMPulse()		((BYTE)(gm_ReadRegWord(BACKLIGHT_G_PWM_CTRL) >> 8))
#define SetGreenBacklightPWMPulse(pulse)	(gm_ClearAndSetBitsWord(BACKLIGHT_G_PWM_CTRL, PWM1_PULSE, (WORD)pulse << 8))
#if defined(CMI_73XX_BOARD)
#define ReadDCRlightPWMPulse()				((BYTE)(gm_ReadRegWord(BACKLIGHT_DCR_PWM_CTRL) >> 8))
#define SetDCRBacklightPWMPulse(pulse)		(gm_ClearAndSetBitsWord(BACKLIGHT_DCR_PWM_CTRL, PWM2_PULSE, (WORD)pulse << 8))
#endif
//===============================================


#define BACKLIGHT_PWM_CONFIG_REG       PWM3_CONFIG
#define BACKLIGHT_PWM_CTRL_REG         PWM3_CTRL

#define PWM_MAX_VAL     0x0FFF


#define MAX_ENERGY_Y				200
#define MIN_ENERGY_Y				100
#define ENERGY_SAVE_15			33//85 
#define ENERGY_SAVE_40			3//35


#if ((DEFAULT_PANEL == LD_T120001A) || (DEFAULT_PANEL == CMO_V562D1_L03))
	#if(BACKLIGHT_CONTROL_GPIO == ENABLE)
	#define BACKLIGHT_PWM_CONFIG      (PWM3_PRESCALE|PWM3_CLKSEL)
	#else
	#define BACKLIGHT_PWM_CONFIG	  (PWM3_PRESCALE|PWM3_CLKSEL|PWM3_GPIOn_EN)
	#endif
	#define MAX_PWMPERIOD      		0x0D7F
	#define MAX_BACKLIGHT_PWM         0x0D7F
	#define DEF_BACKLIGHT_PWM			0x767		// Default	value 
	#define MIN_BACKLIGHT_PWM         0x0150
	#define DEFAULT_BACKLIGHT_INIT     90
#elif ((DEFAULT_PANEL == LG_WUXGA_LM240WU4_SLB1))
	#if(BACKLIGHT_CONTROL_GPIO == ENABLE)
	#define BACKLIGHT_PWM_CONFIG      (PWM3_VSRESET|PWM3_TCLK_DIV|PWM3_CLKSEL)
	#else
	#define BACKLIGHT_PWM_CONFIG	  (PWM3_VSRESET|PWM3_TCLK_DIV|PWM3_CLKSEL|PWM3_GPIOn_EN)
	#endif
	#define MAX_PWMPERIOD		      0x0090
	#define MAX_BACKLIGHT_PWM         0x90
	#define DEF_BACKLIGHT_PWM		  0x5D		// Default	value 
	#define MIN_BACKLIGHT_PWM         0x2B
	#define DEFAULT_BACKLIGHT_INIT     100

#elif (DEFAULT_PANEL == LG_WQHD_LM270WQ1)

#if(BACKLIGHT_CONTROL_GPIO == ENABLE)
#define BACKLIGHT_PWM_CONFIG	  (PWM3_CLKSEL)
#else

#if(PROJECT_SELECT == PROJ_MNT)
	#define BACKLIGHT_PWM_INVERT			1
	#define BACKLIGHT_PWM_CONFIG	  (PWM3_GPIOn_EN|PWM3_CLKSEL|0x0C) //For Sky inverter
#else
	#define BACKLIGHT_PWM_INVERT			1
	#define BACKLIGHT_PWM_CONFIG	  (PWM3_GPIOn_EN|PWM3_CLKSEL|PWM3_TCLK_DIV) 	//For Guo Bin Inverter
#endif

#endif
#define DEFAULT_BACKLIGHT_INIT	   100

#if(PROJECT_SELECT == PROJ_MNT)

	//For Power panel new 27" Panel
	//#define MAX_PWMPERIOD				0xFF		//0x35
	//#define MAX_BACKLIGHT_PWM			0xBF		// 0x35
	//#define DEF_BACKLIGHT_PWM			0x69	// Default	value 
	//#define MIN_BACKLIGHT_PWM			0x13		//0x00
	
	#define MAX_PWMPERIOD				0x7DB
	#define MAX_BACKLIGHT_PWM			0x4FF//0x54D		//6d9
	#define DEF_BACKLIGHT_PWM			0x3B3//0x1AA		// Default	value 
	#define MIN_BACKLIGHT_PWM			0xAE//0x325 	//AE

#else
	#define MAX_PWMPERIOD				0xFF
	#define MAX_BACKLIGHT_PWM			0xBF
	#define DEF_BACKLIGHT_PWM			0x69	// Default	value 
	#define MIN_BACKLIGHT_PWM			0x13	
#endif

//#define DEF_BACKLIGHT_PWM			225

#define MAX_DCR_BACKLIGHT_PWM       0x5FF //0x6C //20120808 Modifiy to 8.5kHz by TigerSong for DCR_PWM. brightness < 30.
#define MIN_DCR_BACKLIGHT_PWM       0x1CC //0x12 //20120808 Modifiy for min Brightness <50cd/m2 by TigerSong.
#define FineStep_DCR_PWM  			2 //120913 Edward extend step from 20 to 20*8  //hgh modify 20120919
#define HIGHT_DCR_BACKLIGHT_PWM     0x00 //120703 Edward for high DCR test
#define MIN_B_BACKLIGHT_PWM         0x181
#define MAX_B_BACKLIGHT_PWM         0x4FF
#define MIN_G_BACKLIGHT_PWM         0x13F
#define MAX_G_BACKLIGHT_PWM         0x4FF

#define MAX_ENERGY_USE_PWM          MAX_BACKLIGHT_PWM
#define MIN_ENERGY_USE_PWM          MIN_BACKLIGHT_PWM


#elif (DEFAULT_PANEL == LG_WHD_LM290WW1_SSA1)	//20120625 modify for PWM1,PWM3 enable

#define BACKLIGHT_PWM_INVERT			0
#define DEFAULT_BACKLIGHT_INIT	   100

#if(BACKLIGHT_CONTROL_GPIO == ENABLE)
#define BACKLIGHT_PWM_CONFIG	  (PWM3_CLKSEL)
#else
#define BACKLIGHT_PWM_CONFIG	  (PWM3_GPIOn_EN|PWM3_CLKSEL|PWM3_TCLK_DIV)
#endif

#define MAX_PWMPERIOD					0xFF
#define MAX_BACKLIGHT_PWM				0xFF	//0xDF//0xFF //121008 Edward for 310 nits		//20121030 Chihlung modify to 0xFF
#define MIN_BACKLIGHT_PWM				0x13	//Hyb130507* Org = 0x02 //ORG =  0x3E	//0x30	//0x0B	//20120925 Chihlung request
#define DEF_BACKLIGHT_PWM				0x89

#define MAX_DCR_BACKLIGHT_PWM       0x5FF //0x6C //20120808 Modifiy to 8.5kHz by TigerSong for DCR_PWM. brightness < 30.
#define MIN_DCR_BACKLIGHT_PWM       0x1CC //0x12 //20120808 Modifiy for min Brightness <50cd/m2 by TigerSong.
#define FineStep_DCR_PWM  			2 //120913 Edward extend step from 20 to 20*8  //hgh modify 20120919
#define HIGHT_DCR_BACKLIGHT_PWM     0x00 //120703 Edward for high DCR test
#define MIN_B_BACKLIGHT_PWM         0x181
#define MAX_B_BACKLIGHT_PWM         0x4FF
#define MIN_G_BACKLIGHT_PWM         0x13F
#define MAX_G_BACKLIGHT_PWM         0x4FF

#define MAX_ENERGY_USE_PWM          WB_BRIGHTNESS_MAX//KevinL 20101117 for ENERGY USE OSD
#define MIN_ENERGY_USE_PWM          WB_BRIGHTNESS_MIN

//#define MAX_ENERGY_USE_PWM           	MAX_BACKLIGHT_PWM
//#define MIN_ENERGY_USE_PWM           	MIN_BACKLIGHT_PWM

//GPIO 29
#define DCR_BACKLIGHT_ENABLE		GPIO_1_ENABLE 	//20121030 Kordonm+ for DCR backlight gpio control
#define DCR_BACKLIGHT_DIRCTRL		GPIO_1_DIRCTRL
#define DCR_BACKLIGHT_OUTPUT		GPOUTPUT1

#define DCR_BACKLIGHT_EN          	 	GPIO29_EN
#define DCR_BACKLIGHT_IO           		GPIO29_IO
#define DCR_BACKLIGHT_OUT        	 	GPIO29_OUT

#elif(DEFAULT_PANEL == LG_WQXGA_LM300WQ5_SLA1)


#define BACKLIGHT_PWM_INVERT			1
#define DEFAULT_BACKLIGHT_INIT	   100

#if(BACKLIGHT_CONTROL_GPIO == ENABLE)
#define BACKLIGHT_PWM_CONFIG	  (PWM3_CLKSEL)
#else
#define BACKLIGHT_PWM_CONFIG	  (PWM3_GPIOn_EN|PWM3_CLKSEL|0x0)//(PWM0_GPIOn_EN|PWM0_CLKSEL|0x08)
#endif

#define MAX_PWMPERIOD				0xFF
#define MAX_BACKLIGHT_PWM			0xA6

#define MIN_BACKLIGHT_PWM			0x00		//Hyb130727* Org = 0x0
#define DEF_BACKLIGHT_PWM			0x53

#define MAX_DCR_BACKLIGHT_PWM       0x5FF //0x6C //20120808 Modifiy to 8.5kHz by TigerSong for DCR_PWM. brightness < 30.
#define MIN_DCR_BACKLIGHT_PWM       0x1CC //0x12 //20120808 Modifiy for min Brightness <50cd/m2 by TigerSong.
#define FineStep_DCR_PWM  			2 //120913 Edward extend step from 20 to 20*8  //hgh modify 20120919
#define HIGHT_DCR_BACKLIGHT_PWM     0x00 //120703 Edward for high DCR test
#define MIN_B_BACKLIGHT_PWM         0x181
#define MAX_B_BACKLIGHT_PWM         0x4FF
#define MIN_G_BACKLIGHT_PWM         0x13F
#define MAX_G_BACKLIGHT_PWM         0x4FF

#define MAX_ENERGY_USE_PWM          MAX_BACKLIGHT_PWM
#define MIN_ENERGY_USE_PWM          MIN_BACKLIGHT_PWM

#elif(DEFAULT_PANEL == LG_WQXGA_LM300WQ6_SLA1)


#define BACKLIGHT_PWM_INVERT			0
#define DEFAULT_BACKLIGHT_INIT	   100

#if(BACKLIGHT_CONTROL_GPIO == ENABLE)
#define BACKLIGHT_PWM_CONFIG	  (PWM3_CLKSEL)
#else
#define BACKLIGHT_PWM_CONFIG	  (PWM3_GPIOn_EN|PWM3_CLKSEL|0x0C) //For Sky inverterPWM3_VSRESET
#endif

#define MAX_PWMPERIOD				0x6A0
#define MAX_BACKLIGHT_PWM			0x4A6//0x54D		//6d9
#define DEF_BACKLIGHT_PWM			0x3B0//0x1AA		// Default	value 
#define MIN_BACKLIGHT_PWM			0x1A0//0x325 	//AE


#define MAX_DCR_BACKLIGHT_PWM       0x5FF //0x6C //20120808 Modifiy to 8.5kHz by TigerSong for DCR_PWM. brightness < 30.
#define MIN_DCR_BACKLIGHT_PWM       0x1CC //0x12 //20120808 Modifiy for min Brightness <50cd/m2 by TigerSong.
#define FineStep_DCR_PWM  			2 //120913 Edward extend step from 20 to 20*8  //hgh modify 20120919
#define HIGHT_DCR_BACKLIGHT_PWM     0x00 //120703 Edward for high DCR test
#define MIN_B_BACKLIGHT_PWM         0x181
#define MAX_B_BACKLIGHT_PWM         0x4FF
#define MIN_G_BACKLIGHT_PWM         0x13F
#define MAX_G_BACKLIGHT_PWM         0x4FF

#define MAX_ENERGY_USE_PWM          MAX_BACKLIGHT_PWM
#define MIN_ENERGY_USE_PWM          MIN_BACKLIGHT_PWM

#elif(DEFAULT_PANEL == LPL_WUXGA_LM240WU5)		//This panel use I2C for backlight adjust ,still use this define for brightness range limit;

#define BACKLIGHT_PWM_INVERT            1
#define DEFAULT_BACKLIGHT_INIT	   100

#if(BACKLIGHT_CONTROL_GPIO == ENABLE)
#define BACKLIGHT_PWM_CONFIG	  (PWM3_CLKSEL)
#else
#define BACKLIGHT_PWM_CONFIG	  (PWM3_GPIOn_EN|PWM3_CLKSEL|PWM3_TCLK_DIV)
#endif

#define MAX_DCR_BACKLIGHT_PWM       0x5FF //0x6C //20120808 Modifiy to 8.5kHz by TigerSong for DCR_PWM. brightness < 30.
#define MIN_DCR_BACKLIGHT_PWM       0x1CC //0x12 //20120808 Modifiy for min Brightness <50cd/m2 by TigerSong.
#define FineStep_DCR_PWM  			2 //120913 Edward extend step from 20 to 20*8  //hgh modify 20120919
#define HIGHT_DCR_BACKLIGHT_PWM     0x00 //120703 Edward for high DCR test
#define MIN_B_BACKLIGHT_PWM         0x181
#define MAX_B_BACKLIGHT_PWM         0x4FF
#define MIN_G_BACKLIGHT_PWM         0x13F
#define MAX_G_BACKLIGHT_PWM         0x4FF

#define MAX_ENERGY_USE_PWM          WB_BRIGHTNESS_MAX//KevinL 20101117 for ENERGY USE OSD
#define MIN_ENERGY_USE_PWM          WB_BRIGHTNESS_MIN

#define MAX_PWMPERIOD				0xFF
#define MAX_BACKLIGHT_PWM			0xFF
#define MIN_BACKLIGHT_PWM			0x14
#define DEF_BACKLIGHT_PWM			0x89

#elif(DEFAULT_PANEL == LG_WXGA_LC320EXN)	//Other Panel 

#define BACKLIGHT_PWM_INVERT            1
#define DEFAULT_BACKLIGHT_INIT	   100

#if(BACKLIGHT_CONTROL_GPIO == ENABLE)
#define BACKLIGHT_PWM_CONFIG	  (PWM3_CLKSEL)
#else
#define BACKLIGHT_PWM_CONFIG	  (PWM3_GPIOn_EN|PWM3_CLKSEL|PWM3_TCLK_DIV)
#endif

#define MAX_DCR_BACKLIGHT_PWM       0x5FF //0x6C //20120808 Modifiy to 8.5kHz by TigerSong for DCR_PWM. brightness < 30.
#define MIN_DCR_BACKLIGHT_PWM       0x1CC //0x12 //20120808 Modifiy for min Brightness <50cd/m2 by TigerSong.
#define FineStep_DCR_PWM  			2 //120913 Edward extend step from 20 to 20*8  //hgh modify 20120919
#define HIGHT_DCR_BACKLIGHT_PWM     0x00 //120703 Edward for high DCR test
#define MIN_B_BACKLIGHT_PWM         0x181
#define MAX_B_BACKLIGHT_PWM         0x4FF
#define MIN_G_BACKLIGHT_PWM         0x13F
#define MAX_G_BACKLIGHT_PWM         0x4FF

#define MAX_ENERGY_USE_PWM          WB_BRIGHTNESS_MAX//KevinL 20101117 for ENERGY USE OSD
#define MIN_ENERGY_USE_PWM          WB_BRIGHTNESS_MIN

#define MAX_PWMPERIOD				0xFF
#define MAX_BACKLIGHT_PWM			0xFF
#define MIN_BACKLIGHT_PWM			0x20
#define DEF_BACKLIGHT_PWM			0x8F

#elif(DEFAULT_PANEL == LG_1080P_LC320EUN)	//Other Panel 

#define BACKLIGHT_PWM_INVERT            1
#define DEFAULT_BACKLIGHT_INIT	   100

//#if(BACKLIGHT_CONTROL_GPIO == ENABLE)
#define BACKLIGHT_PWM_CONFIG	  (PWM3_CLKSEL)
//#else
//#define BACKLIGHT_PWM_CONFIG	  (PWM3_GPIOn_EN)		//Hyb130526* this panel don't have backlight adjust function
//#endif

#define MAX_DCR_BACKLIGHT_PWM       0x5FF //0x6C //20120808 Modifiy to 8.5kHz by TigerSong for DCR_PWM. brightness < 30.
#define MIN_DCR_BACKLIGHT_PWM       0x1CC //0x12 //20120808 Modifiy for min Brightness <50cd/m2 by TigerSong.
#define FineStep_DCR_PWM  			2 //120913 Edward extend step from 20 to 20*8  //hgh modify 20120919
#define HIGHT_DCR_BACKLIGHT_PWM     0x00 //120703 Edward for high DCR test
#define MIN_B_BACKLIGHT_PWM         0x181
#define MAX_B_BACKLIGHT_PWM         0x4FF
#define MIN_G_BACKLIGHT_PWM         0x13F
#define MAX_G_BACKLIGHT_PWM         0x4FF

#define MAX_ENERGY_USE_PWM          WB_BRIGHTNESS_MAX//KevinL 20101117 for ENERGY USE OSD
#define MIN_ENERGY_USE_PWM          WB_BRIGHTNESS_MIN

#define MAX_PWMPERIOD				0xFF
#define MAX_BACKLIGHT_PWM			0xFF
#define MIN_BACKLIGHT_PWM			0x00
#define DEF_BACKLIGHT_PWM			0x80

#elif(DEFAULT_PANEL == SAMSUNG_WXGA_LTI460AA05)	//Other Panel 

#define BACKLIGHT_PWM_INVERT            1
#define DEFAULT_BACKLIGHT_INIT	   100

#if(BACKLIGHT_CONTROL_GPIO == ENABLE)
#define BACKLIGHT_PWM_CONFIG	  (PWM3_CLKSEL)
#else
#define BACKLIGHT_PWM_CONFIG	  (PWM3_GPIOn_EN|PWM3_CLKSEL)///|PWM3_TCLK_DIV)	// PWM3_TCLK_DIV
#endif

#define MAX_DCR_BACKLIGHT_PWM       0x5FF //0x6C //20120808 Modifiy to 8.5kHz by TigerSong for DCR_PWM. brightness < 30.
#define MIN_DCR_BACKLIGHT_PWM       0x1CC //0x12 //20120808 Modifiy for min Brightness <50cd/m2 by TigerSong.
#define FineStep_DCR_PWM  			2 //120913 Edward extend step from 20 to 20*8  //hgh modify 20120919
#define HIGHT_DCR_BACKLIGHT_PWM     0x00 //120703 Edward for high DCR test
#define MIN_B_BACKLIGHT_PWM         0xCD
#define MAX_B_BACKLIGHT_PWM         0xFF
#define MIN_G_BACKLIGHT_PWM         0xCD
#define MAX_G_BACKLIGHT_PWM         0xFF

#define MAX_ENERGY_USE_PWM          WB_BRIGHTNESS_MAX//KevinL 20101117 for ENERGY USE OSD
#define MIN_ENERGY_USE_PWM          WB_BRIGHTNESS_MIN

#define MAX_PWMPERIOD				0x490
#define MAX_BACKLIGHT_PWM			0x48C//0xFF
#define MIN_BACKLIGHT_PWM			0x450//0xE7		
#define DEF_BACKLIGHT_PWM			0x48C	//0xF0		//Hyb131105* org   = 0x80

#elif(DEFAULT_PANEL == SAMSUNG_1080P_LTI460HN05)	//Other Panel 

#define BACKLIGHT_PWM_INVERT            1
#define DEFAULT_BACKLIGHT_INIT	   100

#if	(BACKLIGHT_CONTROL_GPIO == ENABLE)
#define BACKLIGHT_PWM_CONFIG	  (PWM3_CLKSEL)
#else
#define BACKLIGHT_PWM_CONFIG	  (PWM3_GPIOn_EN|PWM3_CLKSEL|PWM3_TCLK_DIV)
#endif

#define MAX_DCR_BACKLIGHT_PWM       0x5FF //0x6C //20120808 Modifiy to 8.5kHz by TigerSong for DCR_PWM. brightness < 30.
#define MIN_DCR_BACKLIGHT_PWM       0x1CC //0x12 //20120808 Modifiy for min Brightness <50cd/m2 by TigerSong.
#define FineStep_DCR_PWM  			2 //120913 Edward extend step from 20 to 20*8  //hgh modify 20120919
#define HIGHT_DCR_BACKLIGHT_PWM     0x00 //120703 Edward for high DCR test
#define MIN_B_BACKLIGHT_PWM         0x181
#define MAX_B_BACKLIGHT_PWM         0x4FF
#define MIN_G_BACKLIGHT_PWM         0x13F
#define MAX_G_BACKLIGHT_PWM         0x4FF

#define MAX_ENERGY_USE_PWM          WB_BRIGHTNESS_MAX//KevinL 20101117 for ENERGY USE OSD
#define MIN_ENERGY_USE_PWM          WB_BRIGHTNESS_MIN

#define MAX_PWMPERIOD				0xA0
#define MAX_BACKLIGHT_PWM			0x82
#define MIN_BACKLIGHT_PWM			0x20
#define DEF_BACKLIGHT_PWM			MAX_BACKLIGHT_PWM

#elif(DEFAULT_PANEL == SAMSUNG_1080P_LTI400HA02)	//Other Panel 

#define BACKLIGHT_PWM_INVERT            1
#define DEFAULT_BACKLIGHT_INIT	   100

#if	(BACKLIGHT_CONTROL_GPIO == ENABLE)
#define BACKLIGHT_PWM_CONFIG	  (PWM3_CLKSEL)
#else
#define BACKLIGHT_PWM_CONFIG	  (PWM3_GPIOn_EN|PWM3_CLKSEL|0x04)
#endif

#define MAX_DCR_BACKLIGHT_PWM       0x5FF //0x6C //20120808 Modifiy to 8.5kHz by TigerSong for DCR_PWM. brightness < 30.
#define MIN_DCR_BACKLIGHT_PWM       0x1CC //0x12 //20120808 Modifiy for min Brightness <50cd/m2 by TigerSong.
#define FineStep_DCR_PWM  			2 //120913 Edward extend step from 20 to 20*8  //hgh modify 20120919
#define HIGHT_DCR_BACKLIGHT_PWM     0x00 //120703 Edward for high DCR test
#define MIN_B_BACKLIGHT_PWM         0x181
#define MAX_B_BACKLIGHT_PWM         0x4FF
#define MIN_G_BACKLIGHT_PWM         0x13F
#define MAX_G_BACKLIGHT_PWM         0x4FF

#define MAX_ENERGY_USE_PWM          WB_BRIGHTNESS_MAX//KevinL 20101117 for ENERGY USE OSD
#define MIN_ENERGY_USE_PWM          WB_BRIGHTNESS_MIN

#define MAX_PWMPERIOD				0x98
#define MAX_BACKLIGHT_PWM			0x98
#define MIN_BACKLIGHT_PWM			0x80
#define DEF_BACKLIGHT_PWM			MAX_BACKLIGHT_PWM


#elif(DEFAULT_PANEL == SAMSUNG_1080P_LTI460HN09)	//Other Panel 

#define BACKLIGHT_PWM_INVERT            1
#define DEFAULT_BACKLIGHT_INIT	   100

#if	1//(BACKLIGHT_CONTROL_GPIO == ENABLE)
#define BACKLIGHT_PWM_CONFIG	  (PWM3_CLKSEL)
#else
#define BACKLIGHT_PWM_CONFIG	  (PWM3_GPIOn_EN|PWM3_CLKSEL|PWM3_TCLK_DIV)
#endif

#define MAX_DCR_BACKLIGHT_PWM       0x5FF //0x6C //20120808 Modifiy to 8.5kHz by TigerSong for DCR_PWM. brightness < 30.
#define MIN_DCR_BACKLIGHT_PWM       0x1CC //0x12 //20120808 Modifiy for min Brightness <50cd/m2 by TigerSong.
#define FineStep_DCR_PWM  			2 //120913 Edward extend step from 20 to 20*8  //hgh modify 20120919
#define HIGHT_DCR_BACKLIGHT_PWM     0x00 //120703 Edward for high DCR test
#define MIN_B_BACKLIGHT_PWM         0x181
#define MAX_B_BACKLIGHT_PWM         0x4FF
#define MIN_G_BACKLIGHT_PWM         0x13F
#define MAX_G_BACKLIGHT_PWM         0x4FF

#define MAX_ENERGY_USE_PWM          WB_BRIGHTNESS_MAX//KevinL 20101117 for ENERGY USE OSD
#define MIN_ENERGY_USE_PWM          WB_BRIGHTNESS_MIN

#define MAX_PWMPERIOD				0xA0
#define MAX_BACKLIGHT_PWM			0x82
#define MIN_BACKLIGHT_PWM			0x20
#define DEF_BACKLIGHT_PWM			MAX_BACKLIGHT_PWM

#elif(DEFAULT_PANEL == SAMSUNG_1080P_LTI460HM03)	//Other Panel 

#define BACKLIGHT_PWM_INVERT            1
#define DEFAULT_BACKLIGHT_INIT	   100

#if (BACKLIGHT_CONTROL_GPIO == ENABLE)
#define BACKLIGHT_PWM_CONFIG	  (PWM3_CLKSEL)
#else
#define BACKLIGHT_PWM_CONFIG	  (PWM3_GPIOn_EN|PWM3_CLKSEL|PWM3_TCLK_DIV)
#endif

#define MAX_DCR_BACKLIGHT_PWM       0x5FF //0x6C //20120808 Modifiy to 8.5kHz by TigerSong for DCR_PWM. brightness < 30.
#define MIN_DCR_BACKLIGHT_PWM       0x1CC //0x12 //20120808 Modifiy for min Brightness <50cd/m2 by TigerSong.
#define FineStep_DCR_PWM  			2 //120913 Edward extend step from 20 to 20*8  //hgh modify 20120919
#define HIGHT_DCR_BACKLIGHT_PWM     0x00 //120703 Edward for high DCR test
#define MIN_B_BACKLIGHT_PWM         0x181
#define MAX_B_BACKLIGHT_PWM         0x4FF
#define MIN_G_BACKLIGHT_PWM         0x13F
#define MAX_G_BACKLIGHT_PWM         0x4FF

#define MAX_ENERGY_USE_PWM          WB_BRIGHTNESS_MAX//KevinL 20101117 for ENERGY USE OSD
#define MIN_ENERGY_USE_PWM          WB_BRIGHTNESS_MIN

#define MAX_PWMPERIOD				0xA0//0xFF
#define MAX_BACKLIGHT_PWM			0x82//0xBE
#define MIN_BACKLIGHT_PWM			0x20//0x20
#define DEF_BACKLIGHT_PWM			MAX_BACKLIGHT_PWM

#elif((DEFAULT_PANEL == LG_1080P_LD420WUB_8BIT)||(DEFAULT_PANEL == LG_1080P_LD420WUB_8BIT))	//Other Panel 

#define BACKLIGHT_PWM_INVERT            1
#define DEFAULT_BACKLIGHT_INIT	   100

#if (BACKLIGHT_CONTROL_GPIO == ENABLE)
#define BACKLIGHT_PWM_CONFIG	  (PWM3_CLKSEL)
#else
#define BACKLIGHT_PWM_CONFIG	  (PWM3_GPIOn_EN|PWM3_CLKSEL|PWM3_TCLK_DIV)
#endif

#define MAX_DCR_BACKLIGHT_PWM       0x5FF //0x6C //20120808 Modifiy to 8.5kHz by TigerSong for DCR_PWM. brightness < 30.
#define MIN_DCR_BACKLIGHT_PWM       0x1CC //0x12 //20120808 Modifiy for min Brightness <50cd/m2 by TigerSong.
#define FineStep_DCR_PWM  			2 //120913 Edward extend step from 20 to 20*8  //hgh modify 20120919
#define HIGHT_DCR_BACKLIGHT_PWM     0x00 //120703 Edward for high DCR test
#define MIN_B_BACKLIGHT_PWM         0x181
#define MAX_B_BACKLIGHT_PWM         0x4FF
#define MIN_G_BACKLIGHT_PWM         0x13F
#define MAX_G_BACKLIGHT_PWM         0x4FF

#define MAX_ENERGY_USE_PWM          WB_BRIGHTNESS_MAX//KevinL 20101117 for ENERGY USE OSD
#define MIN_ENERGY_USE_PWM          WB_BRIGHTNESS_MIN

#define MAX_PWMPERIOD				0xFF
#define MAX_BACKLIGHT_PWM			0xDB
#define MIN_BACKLIGHT_PWM			0x10
#define DEF_BACKLIGHT_PWM			MAX_BACKLIGHT_PWM

#elif((DEFAULT_PANEL == LG_1080P_LD470WUB_8BIT)||(DEFAULT_PANEL == LG_1080P_LD470WUB_10BIT))	//Other Panel )

#define BACKLIGHT_PWM_INVERT            1
#define DEFAULT_BACKLIGHT_INIT	   100

#if (BACKLIGHT_CONTROL_GPIO == ENABLE)
#define BACKLIGHT_PWM_CONFIG	  (PWM3_CLKSEL)
#else
#define BACKLIGHT_PWM_CONFIG	  (PWM3_GPIOn_EN|PWM3_CLKSEL|0x08)
#endif

#define MAX_DCR_BACKLIGHT_PWM       0x5FF //0x6C //20120808 Modifiy to 8.5kHz by TigerSong for DCR_PWM. brightness < 30.
#define MIN_DCR_BACKLIGHT_PWM       0x1CC //0x12 //20120808 Modifiy for min Brightness <50cd/m2 by TigerSong.
#define FineStep_DCR_PWM  			2 //120913 Edward extend step from 20 to 20*8  //hgh modify 20120919
#define HIGHT_DCR_BACKLIGHT_PWM     0x00 //120703 Edward for high DCR test
#define MIN_B_BACKLIGHT_PWM         0x181
#define MAX_B_BACKLIGHT_PWM         0x4FF
#define MIN_G_BACKLIGHT_PWM         0x13F
#define MAX_G_BACKLIGHT_PWM         0x4FF

#define MAX_ENERGY_USE_PWM          WB_BRIGHTNESS_MAX//KevinL 20101117 for ENERGY USE OSD
#define MIN_ENERGY_USE_PWM          WB_BRIGHTNESS_MIN

#define MAX_PWMPERIOD				0x6FF
#define MAX_BACKLIGHT_PWM			0x6F2
#define MIN_BACKLIGHT_PWM			0x6BA
#define DEF_BACKLIGHT_PWM			MAX_BACKLIGHT_PWM

//#if(defined(BOARD_93XX_4K2K))
#elif(DEFAULT_PANEL == ZDT_4K2K_QFHD)	//4K2K Panel

#define BACKLIGHT_PWM_INVERT            1
#define DEFAULT_BACKLIGHT_INIT	   100

#if (BACKLIGHT_CONTROL_GPIO == ENABLE)
#define BACKLIGHT_PWM_CONFIG	  (PWM3_CLKSEL)
#else
#define BACKLIGHT_PWM_CONFIG	  (PWM3_GPIOn_EN|PWM3_CLKSEL|PWM3_TCLK_DIV)
#endif

#define MAX_DCR_BACKLIGHT_PWM       0x5FF //0x6C //20120808 Modifiy to 8.5kHz by TigerSong for DCR_PWM. brightness < 30.
#define MIN_DCR_BACKLIGHT_PWM       0x1CC //0x12 //20120808 Modifiy for min Brightness <50cd/m2 by TigerSong.
#define FineStep_DCR_PWM  			2 //120913 Edward extend step from 20 to 20*8  //hgh modify 20120919
#define HIGHT_DCR_BACKLIGHT_PWM     0x00 //120703 Edward for high DCR test
#define MIN_B_BACKLIGHT_PWM         0x181
#define MAX_B_BACKLIGHT_PWM         0x4FF
#define MIN_G_BACKLIGHT_PWM         0x13F
#define MAX_G_BACKLIGHT_PWM         0x4FF

#define MAX_ENERGY_USE_PWM          WB_BRIGHTNESS_MAX//KevinL 20101117 for ENERGY USE OSD
#define MIN_ENERGY_USE_PWM          WB_BRIGHTNESS_MIN

//For user
/*
#define MAX_PWMPERIOD				0xFF
#define MAX_BACKLIGHT_PWM			0xFA
#define MIN_BACKLIGHT_PWM			0x20
#define DEF_BACKLIGHT_PWM			0xF0
*/
//For user
#define MAX_PWMPERIOD				0xFF
#define MAX_BACKLIGHT_PWM			0xFF
#define MIN_BACKLIGHT_PWM			0xFA
#define DEF_BACKLIGHT_PWM			0xFE

#elif((DEFAULT_PANEL == ZDT_4K2K_CMI))	//4K2K Panel

#define BACKLIGHT_PWM_INVERT            1
#define DEFAULT_BACKLIGHT_INIT	   100

#if (BACKLIGHT_CONTROL_GPIO == ENABLE)
#define BACKLIGHT_PWM_CONFIG	  (PWM3_CLKSEL)
#else
#define BACKLIGHT_PWM_CONFIG	  (PWM3_GPIOn_EN|PWM3_CLKSEL|PWM3_TCLK_DIV)//|PWM3_VSRESET|PWM3_ROLL_RESET)
#endif

#define MAX_DCR_BACKLIGHT_PWM       0x5FF //0x6C //20120808 Modifiy to 8.5kHz by TigerSong for DCR_PWM. brightness < 30.
#define MIN_DCR_BACKLIGHT_PWM       0x1CC //0x12 //20120808 Modifiy for min Brightness <50cd/m2 by TigerSong.
#define FineStep_DCR_PWM  			2 //120913 Edward extend step from 20 to 20*8  //hgh modify 20120919
#define HIGHT_DCR_BACKLIGHT_PWM     0x00 //120703 Edward for high DCR test
#define MIN_B_BACKLIGHT_PWM         0x181
#define MAX_B_BACKLIGHT_PWM         0x4FF
#define MIN_G_BACKLIGHT_PWM         0x13F
#define MAX_G_BACKLIGHT_PWM         0x4FF

#define MAX_ENERGY_USE_PWM          WB_BRIGHTNESS_MAX//KevinL 20101117 for ENERGY USE OSD
#define MIN_ENERGY_USE_PWM          WB_BRIGHTNESS_MIN

#define MAX_PWMPERIOD				0xA4
#define MAX_BACKLIGHT_PWM			0xA4
#define MIN_BACKLIGHT_PWM			0x74
#define DEF_BACKLIGHT_PWM			0x98

#elif((DEFAULT_PANEL == ZDT_4K2K_CMI_39)||(DEFAULT_PANEL == ZDT_4K2K_CMI_65))	//4K2K Panel

#define BACKLIGHT_PWM_INVERT            0   
#define DEFAULT_BACKLIGHT_INIT	      50 //100

#if (BACKLIGHT_CONTROL_GPIO == ENABLE)
#define BACKLIGHT_PWM_CONFIG	  (PWM3_CLKSEL)
#else
#define BACKLIGHT_PWM_CONFIG	  (PWM3_GPIOn_EN|PWM3_CLKSEL|PWM3_TCLK_DIV)//|PWM3_VSRESET|PWM3_ROLL_RESET)
#endif

#define MAX_DCR_BACKLIGHT_PWM       0x5FF //0x6C //20120808 Modifiy to 8.5kHz by TigerSong for DCR_PWM. brightness < 30.
#define MIN_DCR_BACKLIGHT_PWM       0x1CC //0x12 //20120808 Modifiy for min Brightness <50cd/m2 by TigerSong.
#define FineStep_DCR_PWM  			2 //120913 Edward extend step from 20 to 20*8  //hgh modify 20120919
#define HIGHT_DCR_BACKLIGHT_PWM     0x00 //120703 Edward for high DCR test
#define MIN_B_BACKLIGHT_PWM         0x181
#define MAX_B_BACKLIGHT_PWM         0x4FF
#define MIN_G_BACKLIGHT_PWM         0x13F
#define MAX_G_BACKLIGHT_PWM         0x4FF

#define MAX_ENERGY_USE_PWM          WB_BRIGHTNESS_MAX//KevinL 20101117 for ENERGY USE OSD
#define MIN_ENERGY_USE_PWM          WB_BRIGHTNESS_MIN

#define MAX_PWMPERIOD				0xA4
#define MAX_BACKLIGHT_PWM			0xA4
#define MIN_BACKLIGHT_PWM			0x40
#define DEF_BACKLIGHT_PWM			0x98
#elif(DEFAULT_PANEL == ZDT_4K2K_CMI_50)	//4K2K Panel

#define BACKLIGHT_PWM_INVERT            1
#define DEFAULT_BACKLIGHT_INIT	      50 //100

#if (BACKLIGHT_CONTROL_GPIO == ENABLE)
#define BACKLIGHT_PWM_CONFIG	  (PWM3_CLKSEL)
#else
#define BACKLIGHT_PWM_CONFIG	  (PWM3_GPIOn_EN|PWM3_CLKSEL|PWM3_TCLK_DIV)//|PWM3_VSRESET|PWM3_ROLL_RESET)
#endif

#define MAX_DCR_BACKLIGHT_PWM       0x5FF //0x6C //20120808 Modifiy to 8.5kHz by TigerSong for DCR_PWM. brightness < 30.
#define MIN_DCR_BACKLIGHT_PWM       0x1CC //0x12 //20120808 Modifiy for min Brightness <50cd/m2 by TigerSong.
#define FineStep_DCR_PWM  			2 //120913 Edward extend step from 20 to 20*8  //hgh modify 20120919
#define HIGHT_DCR_BACKLIGHT_PWM     0x00 //120703 Edward for high DCR test
#define MIN_B_BACKLIGHT_PWM         0x181
#define MAX_B_BACKLIGHT_PWM         0x4FF
#define MIN_G_BACKLIGHT_PWM         0x13F
#define MAX_G_BACKLIGHT_PWM         0x4FF

#define MAX_ENERGY_USE_PWM          WB_BRIGHTNESS_MAX//KevinL 20101117 for ENERGY USE OSD
#define MIN_ENERGY_USE_PWM          WB_BRIGHTNESS_MIN

#define MAX_PWMPERIOD				0x91
#define MAX_BACKLIGHT_PWM			0x91
#define MIN_BACKLIGHT_PWM			0x40
#define DEF_BACKLIGHT_PWM			0x88


#elif(DEFAULT_PANEL == AUO_1080P_T550HVN01V9_8BIT)	//Other Panel 

#define BACKLIGHT_PWM_INVERT            1
#define DEFAULT_BACKLIGHT_INIT	   100

#if (BACKLIGHT_CONTROL_GPIO == ENABLE)
#define BACKLIGHT_PWM_CONFIG	  (PWM3_CLKSEL)
#else
#define BACKLIGHT_PWM_CONFIG	  (PWM3_GPIOn_EN|PWM3_CLKSEL|PWM3_TCLK_DIV)
#endif

#define MAX_DCR_BACKLIGHT_PWM       0x5FF //0x6C //20120808 Modifiy to 8.5kHz by TigerSong for DCR_PWM. brightness < 30.
#define MIN_DCR_BACKLIGHT_PWM       0x1CC //0x12 //20120808 Modifiy for min Brightness <50cd/m2 by TigerSong.
#define FineStep_DCR_PWM  			2 //120913 Edward extend step from 20 to 20*8  //hgh modify 20120919
#define HIGHT_DCR_BACKLIGHT_PWM     0x00 //120703 Edward for high DCR test
#define MIN_B_BACKLIGHT_PWM         0x181
#define MAX_B_BACKLIGHT_PWM         0x4FF
#define MIN_G_BACKLIGHT_PWM         0x13F
#define MAX_G_BACKLIGHT_PWM         0x4FF

#define MAX_ENERGY_USE_PWM          WB_BRIGHTNESS_MAX//KevinL 20101117 for ENERGY USE OSD
#define MIN_ENERGY_USE_PWM          WB_BRIGHTNESS_MIN

#define MAX_PWMPERIOD				0xA0
#define MAX_BACKLIGHT_PWM			0x82
#define MIN_BACKLIGHT_PWM			0x20
#define DEF_BACKLIGHT_PWM			MAX_BACKLIGHT_PWM


#elif(DEFAULT_PANEL == AUO_1080P_T650HVN02_10BIT)//Other Panel 

#define BACKLIGHT_PWM_INVERT            1
#define DEFAULT_BACKLIGHT_INIT	   100

#if (BACKLIGHT_CONTROL_GPIO == ENABLE)
#define BACKLIGHT_PWM_CONFIG	  (PWM3_CLKSEL)
#else
#define BACKLIGHT_PWM_CONFIG	  (PWM3_GPIOn_EN|PWM3_CLKSEL|PWM3_TCLK_DIV)
#endif

#define MAX_DCR_BACKLIGHT_PWM       0x5FF //0x6C //20120808 Modifiy to 8.5kHz by TigerSong for DCR_PWM. brightness < 30.
#define MIN_DCR_BACKLIGHT_PWM       0x1CC //0x12 //20120808 Modifiy for min Brightness <50cd/m2 by TigerSong.
#define FineStep_DCR_PWM  			2 //120913 Edward extend step from 20 to 20*8  //hgh modify 20120919
#define HIGHT_DCR_BACKLIGHT_PWM     0x00 //120703 Edward for high DCR test
#define MIN_B_BACKLIGHT_PWM         0x181
#define MAX_B_BACKLIGHT_PWM         0x4FF
#define MIN_G_BACKLIGHT_PWM         0x13F
#define MAX_G_BACKLIGHT_PWM         0x4FF

#define MAX_ENERGY_USE_PWM          WB_BRIGHTNESS_MAX//KevinL 20101117 for ENERGY USE OSD
#define MIN_ENERGY_USE_PWM          WB_BRIGHTNESS_MIN

#define MAX_PWMPERIOD				0xA0
#define MAX_BACKLIGHT_PWM			0x82
#define MIN_BACKLIGHT_PWM			0x20
#define DEF_BACKLIGHT_PWM			MAX_BACKLIGHT_PWM

#elif(DEFAULT_PANEL == SHARP_1080P_LD900D3LE1)//Other Panel 

#define BACKLIGHT_PWM_INVERT        1
#define DEFAULT_BACKLIGHT_INIT	   	100

#if (BACKLIGHT_CONTROL_GPIO == ENABLE)
#define BACKLIGHT_PWM_CONFIG	  	(PWM3_CLKSEL)
#else
#define BACKLIGHT_PWM_CONFIG	  	(PWM3_GPIOn_EN|PWM3_CLKSEL|0x0C)
#endif
#define DIM_PWM_CONFIG	  			(PWM3_GPIOn_EN|PWM3_CLKSEL|0x0C)

#define MAX_DCR_BACKLIGHT_PWM       0x5FF //0x6C //20120808 Modifiy to 8.5kHz by TigerSong for DCR_PWM. brightness < 30.
#define MIN_DCR_BACKLIGHT_PWM       0x1CC //0x12 //20120808 Modifiy for min Brightness <50cd/m2 by TigerSong.
#define FineStep_DCR_PWM  			2 //120913 Edward extend step from 20 to 20*8  //hgh modify 20120919
#define HIGHT_DCR_BACKLIGHT_PWM     0x00 //120703 Edward for high DCR test

#define MAX_B_PWMPERIOD				0xE60
#define MAX_B_BACKLIGHT_PWM         0xD38
#define DEF_B_BACKLIGHT_PWM			0x450
#define MIN_B_BACKLIGHT_PWM         0x00

#define MIN_G_BACKLIGHT_PWM         0x13F
#define MAX_G_BACKLIGHT_PWM         0x4FF

#define MAX_ENERGY_USE_PWM          WB_BRIGHTNESS_MAX//KevinL 20101117 for ENERGY USE OSD
#define MIN_ENERGY_USE_PWM          WB_BRIGHTNESS_MIN

#define MAX_PWMPERIOD				0xE60
#define MAX_BACKLIGHT_PWM			0xE60
#define DEF_BACKLIGHT_PWM			0xB50
#define MIN_BACKLIGHT_PWM			0x328
//#define MAX_PWMPERIOD				0xA0
//#define MAX_BACKLIGHT_PWM			0x82
//#define DEF_BACKLIGHT_PWM			0x60
//#define MIN_BACKLIGHT_PWM			0x20


#else	//Other Panel 

#define BACKLIGHT_PWM_INVERT            1
#define DEFAULT_BACKLIGHT_INIT	   100


#if(BACKLIGHT_CONTROL_GPIO == ENABLE)
#define BACKLIGHT_PWM_CONFIG	  (PWM3_CLKSEL)
#else
#define BACKLIGHT_PWM_CONFIG	  (PWM3_GPIOn_EN|PWM3_CLKSEL|PWM3_TCLK_DIV)
#endif

#define MAX_DCR_BACKLIGHT_PWM       0x5FF //0x6C //20120808 Modifiy to 8.5kHz by TigerSong for DCR_PWM. brightness < 30.
#define MIN_DCR_BACKLIGHT_PWM       0x1CC //0x12 //20120808 Modifiy for min Brightness <50cd/m2 by TigerSong.
#define FineStep_DCR_PWM  			2 //120913 Edward extend step from 20 to 20*8  //hgh modify 20120919
#define HIGHT_DCR_BACKLIGHT_PWM     0x00 //120703 Edward for high DCR test
#define MIN_B_BACKLIGHT_PWM         0x181
#define MAX_B_BACKLIGHT_PWM         0x4FF
#define MIN_G_BACKLIGHT_PWM         0x13F
#define MAX_G_BACKLIGHT_PWM         0x4FF

#define MAX_ENERGY_USE_PWM          WB_BRIGHTNESS_MAX//KevinL 20101117 for ENERGY USE OSD
#define MIN_ENERGY_USE_PWM          WB_BRIGHTNESS_MIN

#define MAX_PWMPERIOD				0xFF
#define MAX_BACKLIGHT_PWM			0xFF
#define MIN_BACKLIGHT_PWM			0x20
#define DEF_BACKLIGHT_PWM			0x8F

#endif	//End of backlight settings

//=============================================
//Buzzer control
#ifdef BUZZER_USED
#define BUZZER_PWM_CONFIG			PWM2_CONFIG
#define BUZZER_PWM_CTRL   			PWM2_CTRL
#endif

#if USE_NEW_GAMMA
#ifdef SAVING_DATA_INTO_NVRAM
#define USE_GAMMMA_TABLE_FROM_NVRAM		//yvonne 20120604 get gamma table from NVRAM.
#define Gamma_DUM_Initial_Check
//#define NonColorSpacePostLUT_FROM_NVRAM //120817 Edward add for color team request

#define GammaData_To_DDR				//121030 Edward for speed up color apply
#endif
#endif

#define D_STRING_LEN 		20

#define IsI2SInput(x)		((x == ALL_BOARDS_HDMI) ||(x == ALL_BOARDS_DVI1) || (x == ALL_BOARDS_DVI2) || (x == ALL_BOARDS_DP1) || (x == ALL_BOARDS_DP3))
#define IsTMDSInput(x)	 	((x == ALL_BOARDS_HDMI) ||(x == ALL_BOARDS_DVI1) || (x == ALL_BOARDS_DVI2))

#define VGA_CAB_DIR			GPIO_GROUP_AB_DIR
#define VGA_CAB_INOUT		GPIO_GROUP_AB_GPINPUT
#define VGA_CAB_CTRL		GPIO_GROUP_B7_IN

#define DVI_D1_CAB_EN		GPIO_1_ENABLE
#define DVI_D1_CAB_DIR		GPIO_1_DIRCTRL
#define DVI_D1_CAB_INOUT	GPINPUT1
#define DVI_D1_CAB_CTRL		GPIO27_IN

#define HDMI_CAB_EN			GPIO_1_ENABLE
#define HDMI_CAB_DIR		GPIO_1_DIRCTRL
#define HDMI_CAB_INOUT		GPINPUT1
#define HDMI_CAB_CTRL		GPIO20_IN

#define DP_CAB_EN			GPIO_1_ENABLE
#define DP_CAB_DIR			GPIO_1_DIRCTRL
#define DP_CAB_INOUT		GPINPUT1
#define DP_CAB_CTRL			GPIO21_IN

//120328 Edward add for OSD init in Workbench
#define DEFAULT_MAIN_INPUT		INPUT_DVI1
#define DEFAULT_PIP_INPUT		INPUT_DP

#define ALL_BOARDS_DPF			ALL_BOARDS_DP1
#define BOARD_HDTV				ALL_BOARDS_HDMI

#define CONTROL_XPOS(ptr) ((ptr)[0])
#define CONTROL_YPOS(ptr) ((ptr)[1])
#define CONTROL_WIDTH(ptr) ((ptr)[2])
#define CONTROL_HEIGHT(ptr) ((ptr)[3])


//States
enum
{	//Lvjie 081013 moved from 2709
	ZERO_KEYSTATE,	
	ONE_KEYSTATE,
	TWO_KEYSTATE,
	THREE_KEYSTATE, //VK 20080204
	FOUR_KEYSTATE,
	FIVE_KEYSTATE,
	CLOSE_OSD,
	BURNIN_KEYSTATE,
	DIAGNOSTIC_KEYSTATE, 
};	


enum
{
	DP_SWITCH_1v1a,
	DP_SWITCH_1v2,
	DP_SWITCH_Autos,
};

#define NO_SYNC_CHECK_TIMEOUT	 		25 //KevinL 20090325 Time (100ms) for power on to determine PIP/PBP should be turned on or not.	
#define HDMI_NO_SYNC_CHECK_TIMEOUT		35
#define DP_NO_SYNC_CHECK_TIMEOUT		30 //120908 Edward ad for PxP detect timer when DC On , for some graphic card need to > 27	
#define SPLASH_SCREEN_TIMEOUT			28 //120705 Edward calculate 4313-1480 =2833 from OsdBlendSplashScreen change for PxP mode detect at AC / DC on
//#define SPLASH_SCREEN_TIMEOUT			40	

#define AUTO_CONFIG_TIMEOUT 3

#define STEP_SPALSH_DELAY 5


#define OnForceWakeUpBit		BIT0
#define OnForceWakeUpFlag		(UserPrefBitFlags & OnForceWakeUpBit)//flag for avoid nosync event remove OSD


#define VGABit 			0x01
#define DVIBit 			0x02
#define DVI_2Bit 		0x04
#define HDMIBit 		0x08
#define DPBit 			0x10
//&*&*&*S 20120315:SWRD4001: Kordonm(0)  Modify for OSD Workbench
//#define DP3Bit 			0x20
//#define SVideoBit 0x20
//&*&*&*E 20120315 : Kordonm(1)  Modify for OSD Workbench
#define CompositeBit 	0x40
#define ComponentBit 	0x80

#define OVP_MAIN_AFB_RED_SHIFT              0
#define OVP_MAIN_AFB_GRN_SHIFT              8
#define OVP_MAIN_AFB_BLU_SHIFT              16
#define OVP_PIP_AFB_RED_SHIFT               0
#define OVP_PIP_AFB_GRN_SHIFT               8
#define OVP_PIP_AFB_BLU_SHIFT               16

/*
#define FORCE_BACKGROUND_GRAY		0
#define FORCE_BACKGROUND_RED		1
#define FORCE_BACKGROUND_GREEN		2	 
#define FORCE_BACKGROUND_BLUE		3
#define FORCE_BACKGROUND_BLACK		4 //change lcd condition sequence. //djyang 20071011
#define FORCE_BACKGROUND_WHITE		5
#define FORCE_BACKGROUND_YELLOW		6
#define FORCE_BACKGROUND_MAGENTA	7
#define FORCE_BACKGROUND_CYAN		8
*/
	//Neil120706*
	typedef enum
	{
		FORCE_BACKGROUND_GRAY = 0,	//		0
		FORCE_BACKGROUND_RED,	//		1
		FORCE_BACKGROUND_GREEN,	//		2	 
		FORCE_BACKGROUND_BLUE,	//		3
		FORCE_BACKGROUND_BLACK	,	//	4 //change lcd condition sequence. //djyang 20071011
		FORCE_BACKGROUND_WHITE	,	//	5
		FORCE_BACKGROUND_YELLOW	,	//	6
		FORCE_BACKGROUND_MAGENTA,	//	7
		FORCE_BACKGROUND_CYAN	,	//	8
		FORCE_BACKGROUND_MAX = FORCE_BACKGROUND_WHITE,		//Neil120714* follow U2410//set this can adjust the number you want to display
	}FORCE_BACKGROUND;

#define Dell2609 0
#define Dell2709 1

#define OSD_IsPipSync gm_IsValidSignal(gmvb_PipChannel) 
#define OSD_IsMainSync gm_IsValidSignal(gmvb_MainChannel)

#define OSD_IsPipOnScreen ((UserPrefPipMode != NO_PIP)&&(UserPrefOsdRotation == OSD_NON_ROTATED))
#define OSD_IsMainGrInput (IsGraphicInput(UserPrefCurrentInputMain))
#define OSD_IsPipGrInput  (IsGraphicInput(UserPrefCurrentInputPip))
#define OSD_IsCableConnectedMain IsCableConnected_Main()
#define OSD_IsMainOutOfRange (( stInput[gmvb_MainChannel].ModeFlags & gmd_MODE_OUT_RANGE) != 0) 
#define OSD_IsPipNosignal (OSD_IsPipOnScreen && !OSD_IsPipSync)
#define OSD_IsPipOutOfRange ((stInput[gmvb_PipChannel].ModeFlags & gmd_MODE_OUT_RANGE) != 0) 
#define OSD_IsPipNoCable (OSD_IsPipOnScreen && !IsCableConnected_Pip())
#define OSD_IsPipDisplayed (OSD_IsPipOnScreen && OSD_IsPipSync&&!OSD_IsPipOutOfRange)
#define OSD_IsMainVdInput (!OSD_IsMainGrInput)
#define OSD_IsPipNosignal_AddOutOfRange  (OSD_IsPipNosignal && !OSD_IsPipOutOfRange)//owen 20120808
#define OSD_IsPip_Pip		(Is_PIP_Pip())
#define OSD_IsPBP			(IS_PBP())


#define OSD_IsDPHighBitRate2 (ReadDPBandWidth() == 0x14)
#define OSD_IsDPHighBitRate (ReadDPBandWidth() == 0x0A)
#define OSD_IsDPLowBitRate  (ReadDPBandWidth() == 0x06)
#define OSD_IsDPLaneCount1 (ReadDPLaneCount() == 0x01)
#define OSD_IsDPLaneCount2 (ReadDPLaneCount() == 0x02)
#define OSD_IsDPLaneCount4 (ReadDPLaneCount() == 0x04)

#define OSD_IsPIPMsgAvailable  (bPipMessageAdmitFlag==TRUE) //KevinL 20090212
//#define OSD_EnterPowerSaving_NO_PIP  ((UserPrefPipMode == NO_PIP) && (!OSD_IsMainSync)&& OSD_IsMainGrInput && (UserPrefRunBurnInModeFlag == 0) && (OSD_IsCableConnectedMain)) //KevinL 20090223
#define OSD_EnterPowerSaving_NO_PIP  ((UserPrefPipMode == NO_PIP) && (!OSD_IsMainSync)&& OSD_IsMainGrInput && (OSD_IsCableConnectedMain)&&(!OSD_IsMainOutOfRange)) //Neil120720* Add Main OOR condition //KevinL 20090223 //calo update 090307
//120807 Edward remove PxP cable detect check for into DPMS whatever PxP cable is connectedd for DELL spec
#define OSD_EnterPowerSaving_PIP (((OSD_IsPip_Pip && OSD_IsPipGrInput && /*(!OSD_IsPipNoCable) &&*/ (!OSD_IsPipSync) && (!OSD_IsPipOutOfRange))) \
									&& (!OSD_IsMainSync) && OSD_IsMainGrInput && (UserPrefRunBurnInModeFlag == 0) && (OSD_IsCableConnectedMain)&&(!OSD_IsMainOutOfRange))//Neil120720* Add Main OOR condition 
#define OSD_EnterPowerSaving_PBP ((((UserPrefPipMode == SIDE_BY_SIDE) && OSD_IsPipGrInput && /*(!OSD_IsPipNoCable) &&*/ (!OSD_IsPipSync) && (!OSD_IsPipOutOfRange))) \
									&& (!OSD_IsMainSync) && OSD_IsMainGrInput && (UserPrefRunBurnInModeFlag == 0) && (OSD_IsCableConnectedMain)&&(!OSD_IsMainOutOfRange))//Neil120720* Add Main OOR condition 
#define OSD_EnterPIPOnState_PIP (((OSD_IsPip_Pip && !OSD_IsPipGrInput && (!OSD_IsPipNoCable) && (!OSD_IsPipSync) && (!OSD_IsPipOutOfRange))) && (!OSD_IsMainSync) && (UserPrefRunBurnInModeFlag == 0) && (OSD_IsCableConnectedMain))
#define OSD_EnterPIPOnState_PBP ((((UserPrefPipMode == SIDE_BY_SIDE) && !OSD_IsPipGrInput && (!OSD_IsPipNoCable) && (!OSD_IsPipSync) && (!OSD_IsPipOutOfRange))) && (!OSD_IsMainSync) && (UserPrefRunBurnInModeFlag == 0) && (OSD_IsCableConnectedMain))
#define OSD_EnterVideoNoSignalState_PBP ((OSD_IsMainVdInput) && (UserPrefPipMode == SIDE_BY_SIDE) && ((OSD_IsPipSync) || (OSD_IsPipOutOfRange)||(OSD_IsPipNoCable) ||(OSD_IsPipNosignal)))
#define OSD_EnterVideoNoSignalState_PIP ((OSD_IsMainVdInput) && OSD_IsPip_Pip && ((OSD_IsPipSync) || (OSD_IsPipOutOfRange)||(OSD_IsPipNoCable) ||(OSD_IsPipNosignal)))
#define OSD_EnterVideoNoSignalState_NO_PIP ((OSD_IsMainVdInput) && (UserPrefPipMode == NO_PIP))

#define  gm_IsModeOOR(CH)   ((stInput[(CH)].ModeFlags & gmd_MODE_OUT_RANGE) == gmd_MODE_OUT_RANGE)

#define Rotation_90 (UserPrefGlobalRotationEnabled==1)//OwenH 20120528
#define Rotation_0 (UserPrefGlobalRotationEnabled==0)

#define Rotation_OSD_0 (UserPrefOsdRotation == OSD_NON_ROTATED)//owen 20120627
#define Rotation_OSD_90 (UserPrefOsdRotation == OSD_ROTATED_90)
#define Rotation_OSD_180 (UserPrefOsdRotation == OSD_ROTATED_180)

#define PanelH_1200 (PanelHeight == 1200)
#define PanelH_1440 (PanelHeight == 1440)
#define PanelH_1600 (PanelHeight == 1600)//owen 120830
#define PanelH_1080 (PanelHeight == 1080)

#define Key1_Source_All (UserPrefPersonalKey1==ShortCut_InputSource_ALL) 
#define Key1_VGA (UserPrefPersonalKey1==ShortCut_InputSource_VGA)
#define Key1_DVI (UserPrefPersonalKey1==ShortCut_InputSource_DVI1)
#define Key1_HDMI (UserPrefPersonalKey1==ShortCut_InputSource_HDMI)
#define Key1_DP (UserPrefPersonalKey1==ShortCut_InputSource_DP)
#define Key1_DP3 (UserPrefPersonalKey1==ShortCut_InputSource_DP3)
#define Key1_AutoAdjust (UserPrefPersonalKey1 == ShortCut_AutoAdjust)

#define Key2_Source_All (UserPrefPersonalKey2==ShortCut_InputSource_ALL) 
#define Key2_VGA (UserPrefPersonalKey2==ShortCut_InputSource_VGA)
#define Key2_DVI (UserPrefPersonalKey2==ShortCut_InputSource_DVI1)
#define Key2_HDMI (UserPrefPersonalKey2==ShortCut_InputSource_HDMI)
#define Key2_DP (UserPrefPersonalKey2==ShortCut_InputSource_DP)
#define Key2_DP3 (UserPrefPersonalKey2==ShortCut_InputSource_DP3)

#define Key3_Source_All (UserPrefPersonalKey3==ShortCut_InputSource_ALL) 
#define Key3_VGA (UserPrefPersonalKey3==ShortCut_InputSource_VGA)
#define Key3_DVI (UserPrefPersonalKey3==ShortCut_InputSource_DVI1)
#define Key3_HDMI (UserPrefPersonalKey3==ShortCut_InputSource_HDMI)
#define Key3_DP (UserPrefPersonalKey3==ShortCut_InputSource_DP)
#define Key3_DP3 (UserPrefPersonalKey3==ShortCut_InputSource_DP3)


#define Main_VGA (UserPrefCurrentInputMain == ALL_BOARDS_VGA1)

#define Pip_VGA (UserPrefCurrentInputPip == ALL_BOARDS_VGA1)
#define Pip_DP1 (UserPrefCurrentInputPip == ALL_BOARDS_DP1)
#define Pip_DP3  (UserPrefCurrentInputPip == ALL_BOARDS_DP3)

#define Key1_All_Rotoation_0 (Key1_Source_All && Rotation_0)
#define Key1_VGA_Rotoation_0 (Key1_VGA && Rotation_0)
#define Key1_DVI_Rotoation_0 (Key1_DVI && Rotation_0) 
#define Key1_HDMI_Rotoation_0 (Key1_HDMI && Rotation_0) 
#define Key1_DP_Rotoation_0 (Key1_DP && Rotation_0) 
#define Key1_DP3_Rotoation_0 (Key1_DP3 && Rotation_0) 

#define Key1_All_Rotoation_90 (Key1_Source_All && Rotation_90)
#define Key1_VGA_Rotoation_90 (Key1_VGA && Rotation_90)
#define Key1_DVI_Rotoation_90 (Key1_DVI && Rotation_90) 
#define Key1_HDMI_Rotoation_90 (Key1_HDMI && Rotation_90) 
#define Key1_DP_Rotoation_90 (Key1_DP && Rotation_90) 
#define Key1_DP3_Rotoation_90 (Key1_DP3 && Rotation_90) 

//20120917 Add by KevinChen for Dell confirm spec(there should not have any effect/response/flashing when the shortcut key (ShortCutKey)is pressed).
#define Key1_ChangePortConflict (((Key1_DVI||Key1_HDMI)&&PxPSourceIsDVIHDMI())||((Key1_DP||Key1_DP3)&&PxPSourceIsDPMiniDP()))
#define Key2_ChangePortConflict (((Key2_DVI||Key2_HDMI)&&PxPSourceIsDVIHDMI())||((Key2_DP||Key2_DP3)&&PxPSourceIsDPMiniDP()))
#define Key3_ChangePortConflict (((Key3_DVI||Key3_HDMI)&&PxPSourceIsDVIHDMI())||((Key3_DP||Key3_DP3)&&PxPSourceIsDPMiniDP()))


#ifdef DELL_U2410F //KevinL 20090210 
typedef enum OSDMainMessageTypeEnum
{
	OSD_NULL = 0,
	OSD_MAIN_NO_CABLE,
	OSD_MAIN_NO_SIGNAL_GAPHIC,
	OSD_MAIN_NO_SIGNAL_VIDEO,
	OSD_MAIN_OUT_OF_RANGE,
	OSD_MAIN_ENTER_POWER_SAVING_GRAPHIC,
	OSD_MAIN_ENTER_POWER_SAVING_VIDEO,
	OSD_PBP_MAIN_NO_CABLE,
	OSD_PBP_MAIN_NO_SIGNAL_GRAPHIC,
	OSD_PBP_MAIN_NO_SIGNAL_VIDEO,
	OSD_PBP_MAIN_OUT_OF_RANGE,
	OSD_PBP_MAIN_ENTER_POWER_SAVING,
	OSD_PIP_ON_SCREEN,
}OSDMainMessageType;

#define OSD_IsPBP_Main_OOR_Required ((UserPrefStateTempFlag == OUT_OF_RANGE_STATE)) 
#define OSD_IsPBP_Main_NoCable_Required ((UserPrefStateTempFlag == NO_SIGNAL_STATE)) 
#define OSD_IsPBP_Main_NoInputSignal_Required (UserPrefStateTempFlag == PC_MESSAGE_STATE) && (UserPrefPipMode == SIDE_BY_SIDE)&&(IsGraphicInput(UserPrefCurrentInputMain)) && (!OSD_IsMainSync)  
#define IsUserPrefOutOfRange(UserPref, ADJV)	((UserPref > gm_GetAdjusterMax(ADJV)) || (UserPref < gm_GetAdjusterMin(ADJV)))
#endif

//&*&*&*S 20120315:SWRD4001: Kordonm(0)  Modify for OSD Workbench
#define IsGraphicInput(x)		((x == ALL_BOARDS_VGA1) || (x == ALL_BOARDS_DVI1) || (x == ALL_BOARDS_DVI2) || (x == ALL_BOARDS_DP1) || (x == ALL_BOARDS_DP3) || (x == ALL_BOARDS_HDMI))
//&*&*&*E 20120315 : Kordonm(1)  Modify for OSD Workbench
#define IsVideoInput(x)		((x == ALL_BOARDS_COMP1) || (x == ALL_BOARDS_CVBS1) || (x==ALL_BOARDS_DIP1) || (x==ALL_BOARDS_CVBS2) ||(x==ALL_BOARDS_COMP2))
#define IsDecoderInput(x)	((x == ALL_BOARDS_CVBS1) || (x==ALL_BOARDS_CVBS2) ||(x==ALL_BOARDS_COMP2))
enum  //owen 20120801
{
	OSD_Smart_Advace_Close = 0,
	OSD_Smart_Advace_Open
};


enum
{
	OSD_UNLOCK = 0,
	OSD_LOCK
};

enum 
{
       OSD_LANDSCAPE = 0,
	OSD_PORTRAIT 
};

enum
{
	Main,
	Pxp,
};

enum
{
	ENGLISH_LANG = LANG_English,
	ESPANOL_LANG = LANG_Spanish,
	FRANCE_LANG = LANG_French,
	GERMAN_LANG = LANG_German,
	JAPAN_LANG = LANG_Japanese,
    	RUSSIA_LANG = LANG_Russian,
    	BRAZIL_LANG = LANG_Brazilian_Portuguese,
    	CHINESE_LANG = LANG_Simply_Chinese
};

//	(UserPrefStateTempFlag == CONTRAST_HOTKEY)
enum //KevinL 20090115 updated for OSD StateTempFlag meaningful definition
{
	DEFAULT = 0,
	CUSTOM_COLOR_HOTKEY,
	CUSTOM_COLOR_MENU,
	DCR_BRIGHTNESS_HOTKEY,
	DCR_BRIGHTNESS_MENU,
	OSD_SCAN_SOURCE, //5
	ENTER_POWER_SAVING,//6
	AUTO_ADJUST_STATE, //7
	UNKNOW_STATE8,
	UNKNOW_STATE9,
	NO_SIGNAL_STATE, //10
	OUT_OF_RANGE_STATE, // 11
	PIP_ON_SCREEN_STATE, // 12
	INPUT_COLOR_FORMAT_STATE,// 13
	NO_CABLE_STATE,// 14	
	CONTRAST_HOTKEY,
	CONTRAST_MENU,
	VOLUME_HOTKEY,
	VOLUME_MENU,
	HOTKEY_DIM,
	FACTORY_RESET,
	PC_MESSAGE_STATE =99,
};


//	(UserPrefStateTempFlag == CONTRAST_HOTKEY)
enum //KevinL 20090115 updated for OSD StateTempFlag meaningful definition
{
	OSTATE_DEFAULT = 0,
	OSTATE_VALIDMODE,
	OSTATE_DEEPSLEEP,
	OSTATE_POWERSAVING,
	OSTATE_FORCEPOWERSAVING,
	OSTATE_BURNIN, //5
	OSTATE_POWEROFF,//6
	OSTATE_SPLASHSCREEN, //7
	
	OSTATE_MAIN = 10,
	OSTATE_MAIN_IMAGE = 15,
	OSTATE_MAIN_INPUT= 25, //10
	OSTATE_MAIN_DISPLAY = 35, // 11
	OSTATE_MAIN_PXP =45, // 12
	OSTATE_MAIN_AUDIO = 55,// 13
	OSTATE_MAIN_SETUP =65,// 14	
	OSTATE_MAIN_PD= 75,
	
	OSTATE_MAIN_MSG = 85,
	OSTATE_MAIN_MSG_NOSYNC,
	OSTATE_MAIN_MSG_NOCABLE,
	OSTATE_MAIN_MSG_OOR,
	OSTATE_MAIN_MSG_AUTO,
	OSTATE_MAIN_MSG_ENTERPS,		//enter power saving,
	OSTATE_MAIN_MSG_PS_PCM,
	OSTATE_MAIN_MSG_PS_VIDEO,
	OSTATE_MAIN_MSG_AUTOCOLOR,
	OSTATE_MAIN_MSG_INPUT,
	OSTATE_MAIN_MSG_AUDPCM,
	
	OSTATE_MAIN_MSG_OSDLOCK,
	OSTATE_MAIN_MSG_FLYING,
	OSTATE_MAIN_MSG_NOFLYING,
	
	
	OSTATE_PBP_MSG 	= 110,
	OSTATE_PBP_MSG_OOR,
	OSTATE_PBP_MSG_NOSYNC,
	OSTATE_PBP_MSG_NOCABLE,
	OSTATE_PBP_MSG_PSPC,
	OSTATE_PBP_MSG_PSVIDEO,
	OSTATE_PBP_MSG_AUTO,
	OSTATE_PBP_MSG_ENTERPS,
	OSTATE_PBP_MSG_PCM,
	
	OSTATE_CONTROL, // 119
	OSTATE_PORTCHANGE,
	OSTATE_LCDCONDITION,
	OSTATE_DIAGNOSTIC,
	OSTATE_FACTORYMODE, 
	OSTATE_INFO_SHOW,
	OSTATE_INFO_BOARDID,
	OSTATE_INFO_PRESETMODE,
	OSTATE_INFO_SMALL_SHOW,
	OSTATE_INFO_SMALL_BOARDID,
	OSTATE_INFO_SMALL_PRESETMODE,
	OSTATE_PIPMODE,
	OSTATE_YESNO,
	OSTATE_LAMPSTATE, 
	
	OSTATE_THREAD1_MAINICON_INPUT  , // 133

	OSTATE_THREAD2_PIPICON_INPUT,

	OSTATE_THREAD3_PIP_MSG,
	
	OSTATE_MAX =255,
};

//***************************************
//  S C A L I N G   T Y P E   S E L E C T I O N
//***************************************
// Select scaling mode for output
// Non scaling output: 
//     0=Output timing is based on panel definition 
//     1=Output timing is same as input
// PassThruMode: Only works when non scaling output is enabled.
//               No memory would be used for both 0/1.
//     0=Use frame buffer;
//     1=No frame buffer used, input would be pass thru to ODP
// Deinterlaced output: Only works for PassThruMode is disabled.
//                      Memory would be used for MADi
//     0=No de-interlaced for input
//     1=Output is de-interlaced by MADi
//       Display VS ISR would be used for DPTx interlaced output
typedef enum OutputModeEnum
{
	OPM_Normal,
	OPM_NoScaling,
	OPM_PassThrough,
	OPM_DInterlaced,
	OPM_Max,
}OutputModeType;

typedef enum OutputModeState
{
	OPS_Idle,
	OPS_Init,
	OPS_Start,
	OPS_Processing,
	OPS_Done,
}OutputModeStateType;

#ifndef DELL_U2410F
enum
{
	INPUT_VGA = 0,
	INPUT_DVI1,
	INPUT_DVI2,
	INPUT_HDMI,
	INPUT_DP,
	INPUT_SVIDEO,
	INPUT_CVBS,
	INPUT_COMP,
};
#endif

#define MAX_TNR_LEVEL 1

#ifdef DELL_U2410F 
//==================Brightness=======================
#define WB_BRIGHTNESS_MAX					100//50  //107 
#define WB_BRIGHTNESS_MIN					0//-50 //-108
//#define WB_BRIGHTNESS_DEFAULT 				50 //256		//Move Up

//For Text mode 
#define WB_BRIGHTNESS_TEXT_MAX				WB_BRIGHTNESS_MAX 
#define WB_BRIGHTNESS_TEXT_MIN				WB_BRIGHTNESS_MIN
#define WB_BRIGHTNESS_CAL1_MAX				WB_BRIGHTNESS_MAX 
#define WB_BRIGHTNESS_CAL1_MIN				WB_BRIGHTNESS_MIN
#define WB_BRIGHTNESS_CAL2_MAX				WB_BRIGHTNESS_MAX 
#define WB_BRIGHTNESS_CAL2_MIN				WB_BRIGHTNESS_MIN

#define WB_BRIGHTNESS_TEXT_DEFAULT 			WB_BRIGHTNESS_DEFAULT //256
#define WB_BRIGHTNESS_CAL1_DEFAULT 			WB_BRIGHTNESS_DEFAULT //256
#define WB_BRIGHTNESS_CAL2_DEFAULT 			WB_BRIGHTNESS_DEFAULT //256
#define WB_CSPACE_DEFAULT					CSPACE_sRGB	//20121023 Kordonm+ for VGA port image scheme data save

#define PWM_LowLevel_Smooth			1// 1120913 Edward for BL low level more smooth		


#if ((DEFAULT_PANEL == LG_WUXGA_LM240WU9_SLA1) ||(DEFAULT_PANEL == LG_WUXGA_LM240WU4_SLB1))   //Tiger change for P2 color tune
#define WB_PWM_Factory_Max                  255
#define WB_Brigntness_OSD_DCR_Start         20
#define WB_Brigntness_OSD_Default           50
#define WB_Brigntness_OSD_Max               100
#define WB_Brightness_TAB_LEN               76
#define WB_BriBase6500K_DEF                 120
#define WB_BriBasesRGB_DEF                  120
#define WB_BriBaseAdobeRGB_DEFT             120
#define WB_BriBaseTable1_DEFT               205  //chunbao request by tiger
#define WB_BriBaseTable2_DEFT               205
#define WB_BriBaseText_DEFT                 0  //chunbao Dell requir paper mode brightness 110+/-15nits
#elif(DEFAULT_PANEL == LG_WHD_LM290WW1_SSA1)
#define WB_BriBase6500K_DEF                 160//20120726 Modify by KevinChen form Qisda mail request.
#define WB_BriBasesRGB_DEF					160
#define WB_BriBaseAdobeRGB_DEFT 			160
#define WB_BriBaseTable1_DEFT             	210	//189//50		//20121030 Chihlung modify to 210
#define WB_BriBaseTable2_DEFT             	210	//189//50		//20121030 Chihlung modify to 210
#define WB_BriBaseText_DEFT 				110
#elif(DEFAULT_PANEL == LG_WQXGA_LM300WQ5_SLA1)
#define WB_BriBase6500K_DEF                 160//20120726 Modify by KevinChen form Qisda mail request.
#define WB_BriBasesRGB_DEF					160
#define WB_BriBaseAdobeRGB_DEFT 			160
#define WB_BriBaseTable1_DEFT             	50
#define WB_BriBaseTable2_DEFT             	50
#define WB_BriBaseText_DEFT 				110
#define WB_Brightness_TAB_LEN               101
#define WB_Brigntness_OSD_Max               100
#define WB_Brigntness_OSD_Default           50
#define WB_PWM_Factory_Max                  255

//#define MAX_DCR_BACKLIGHT_PWM      0x0 //owen 121017
//#define MIN_DCR_BACKLIGHT_PWM       0x0D8 
//#define HIGHT_DCR_BACKLIGHT_PWM     0x0D8 

//#define MIN_B_BACKLIGHT_PWM         0x0D8	//20121002
//#define MAX_B_BACKLIGHT_PWM         0xB
//#define MIN_G_BACKLIGHT_PWM         0x0D8
//#define MAX_G_BACKLIGHT_PWM         0x0
#else	//(DEFAULT_PANEL == LG_WQHD_LM270WQ1)
/*
#define WB_Brightness_TAB_LEN               101
#define WB_Brigntness_OSD_Max               100
#define WB_Brigntness_OSD_Default           50
#define WB_PWM_Factory_Max                  255
#define WB_Brigntness_OSD_DCR_Start         20
#define WB_Brigntness_OSD_Default           50
*/


#define WB_BriBase6500K_DEF                 50//20120919;modified for P3 panel
#define WB_BriBasesRGB_DEF					90//50
#define WB_BriBaseAdobeRGB_DEFT 			90//50
#define WB_BriBaseTable1_DEFT             	50
#define WB_BriBaseTable2_DEFT             	50
#define WB_BriBaseText_DEFT 				50
#endif

//==================Contrast=======================
#define WB_CONTRAST_MAX				100//306 //384 
#define WB_CONTRAST_MIN				0//206 //128 
#define WB_CONTRAST_MAX_YUV         	100//324 //20120711 Add by KevinChen for YUV 32 gray color issue
#define WB_CONTRAST_TEXT_MAX		WB_CONTRAST_MAX
#define WB_CONTRAST_TEXT_MIN		WB_CONTRAST_MIN
#define WB_CONTRAST_CAL1_MAX		WB_CONTRAST_MAX
#define WB_CONTRAST_CAL1_MIN		WB_CONTRAST_MIN
#define WB_CONTRAST_CAL2_MAX		WB_CONTRAST_MAX
#define WB_CONTRAST_CAL2_MIN		WB_CONTRAST_MIN


#define WB_CONTRAST_DEFAULT			50//256 
#define WB_CONTRAST_TEXT_DEFAULT		50//256 
#define WB_CONTRAST_CAL1_DEFAULT		50//256
#define WB_CONTRAST_CAL2_DEFAULT		50//256

#define ADJ_MAX_CONTRAST            		296
#define ADJ_MAX_CONTRAST_YUV        		332
#define ADJ_DEFAULT_CONTRAST        		258	//Hyb130429* Org = 258
#define ADJ_TEXT_DEFAULT_CONTRAST   		258 	//Hyb130429* Org = 258
#define ADJ_CUSTOM_DEFAULT_CONTRAST 	258  //Hyb130429* Org = 258
#define ADJ_MIN_CONTRAST            		0//206

#define WB_SATURATION_MAX					100  //512 
#define WB_SATURATION_MIN					0 
#define WB_SATURATION_DEFAULT 				50 //256
#define WB_SATURATION_YPbPr_DEFAULT 		50	//Hyb130414- Org = 46
#if(defined(BOARD_93XX_MNT))
#define SATURATION_MAX					512  //512 
#define SATURATION_MIN					0 
#define SATURATION_DEFAULT 				272 //256
#define SATURATION_YPbPr_DEFAULT 		272	//Hyb130414- Org = 46
#else
#define SATURATION_MAX					512  //512 
#define SATURATION_MIN					0 
#define SATURATION_DEFAULT 				256 //256
#define SATURATION_YPbPr_DEFAULT 		256	//Hyb130414- Org = 46
#endif



#define WB_SHARPNESS_MAX				22 
#define WB_SHARPNESS_MIN				2 
#define WB_SHARPNESS_DEFAULT			12 


#define HUE_MAX							50  //180 
#define HUE_MIN							-50 //-180

#define MAX_VGA_PHASE       				63
#define WB_MAX_VGA_PHASE  				100

#define ACM3_6AXIS_MAX					127  //KevinL 20090331  
#define ACM3_6AXIS_MIN					-127 
#define WB_6AXIS_MAX					50  //KevinL 20090331  
#define WB_6AXIS_MIN					-50 
#define WB_6AXIS_MAX_Sat				100  //110919 Edward  
#define WB_6AXIS_MIN_Sat				0 

#define WB_6AXIS_HUE_DEFAULT  			0//120523 Edward for reset AdjustColorWrap to default
#define WB_6AXIS_LUMA_DEFAULT  		0
#define WB_6AXIS_SAT_DEFAULT  			50

#define RGB_OFFSET_DEFAULT       		20
#define RGB_OFFSET_MAX					50 //100 
#define RGB_OFFSET_MIN					-14 //-100 
#define USER_RGB_OFFSET_MAX			50  
#define USER_RGB_OFFSET_MIN			-50 
#define USER_RGB_OFFSET_SHFIT			20 //120718 Edward for 

#else
#define MAX_CONTRAST					346 
#define MID_CONTRAST					100 
#define MIN_CONTRAST					0
#define DEFAULT_CONTRAST				256 
#endif

// Corelation to Custom(RGB) of OSD
#ifdef DELL_U2410F //KevinL 20090310 for DELL's Request  
#define USER_GAIN_MIN	0 //128
#define USER_GAIN_DEF	100 //255
#define USER_GAIN_MAX	100 //255
#define MAX_RGB_GAIN    255//230 //255 //120522 Edward change 0~255
#define MIN_RGB_GAIN    0

//for factory color temp default
#define WB_CTEMP_DEF		CTEMP_6500K
#define MAX_F_CT_GAIN     511
#define MIN_F_CT_GAIN 		0

#if defined(Athena_C) //20120910 KimChye
#define DEF_F_CT10000K_R 		201 //217
#define DEF_F_CT10000K_G  		213 //210
#define DEF_F_CT10000K_B  		252

#define DEF_F_CT9300K_R  		212 //213
#define DEF_F_CT9300K_G  		223 //218
#define DEF_F_CT9300K_B 		255

#define DEF_F_CT7500K_R  		233 //241
#define DEF_F_CT7500K_G  		238 //243
#define DEF_F_CT7500K_B  		255

#define DEF_F_CT7200K_R  		239 //255
#define DEF_F_CT7200K_G  		236 //232
#define DEF_F_CT7200K_B  		255 //250

#define DEF_F_CT6500K_R  		255 //249 //255 chunbao set to 255 for panel nactive
#define DEF_F_CT6500K_G  		255 //246 //255
#define DEF_F_CT6500K_B  		255 //253 //255

#define DEF_F_CT5700K_R  		247 //255
#define DEF_F_CT5700K_G  		237 //243
#define DEF_F_CT5700K_B  		229 //222

#define DEF_F_CT5000K_R  		251 //255
#define DEF_F_CT5000K_G  		234 //228
#define DEF_F_CT5000K_B  		211 //194

#define DEF_F_sRGB_R  			255
#define DEF_F_sRGB_G  			255
#define DEF_F_sRGB_B 	 		255

#define DEF_F_AdobeRGB_R  		255
#define DEF_F_AdobeRGB_G  		255
#define DEF_F_AdobeRGB_B  		255

#else		//Athena_U
#define DEF_F_CT10000K_R        212
#define DEF_F_CT10000K_G        214
#define DEF_F_CT10000K_B      	255

#define DEF_F_CT9300K_R         220
#define DEF_F_CT9300K_G         219
#define DEF_F_CT9300K_B         255

#define DEF_F_CT7500K_R         243
#define DEF_F_CT7500K_G         232
#define DEF_F_CT7500K_B         255

#define DEF_F_CT7200K_R         249
#define DEF_F_CT7200K_G         230
#define DEF_F_CT7200K_B         255

#define DEF_F_CT6500K_R         255
#define DEF_F_CT6500K_G         255
#define DEF_F_CT6500K_B         255

#define DEF_F_CT5700K_R         253
#define DEF_F_CT5700K_G         230
#define DEF_F_CT5700K_B         230

#define DEF_F_CT5000K_R         253
#define DEF_F_CT5000K_G         222
#define DEF_F_CT5000K_B         207

#define DEF_F_sRGB_R  			255
#define DEF_F_sRGB_G  			255
#define DEF_F_sRGB_B 	 		255

#define DEF_F_AdobeRGB_R  		255
#define DEF_F_AdobeRGB_G  		255
#define DEF_F_AdobeRGB_B  		255

#endif
#endif//DELL_U2410F

#define MAIN_DISPLAY 0
#define PIP_DISPLAY 0

//120321 Edward disable no use
//#define Default_AudioPreset 1
//#define Default_AudioInput 1
#define VGA_DAP  BIT0
#define DVI1_DAP  BIT1
#define DVI2_DAP  BIT2
#define HDMI_DAP BIT3
#define DP_DAP1   BIT4
#define DP_DAP2   BIT5
#define DAP_DELTA 8

typedef enum
{
  	CHANNEL_A,				// input graphic channel
  	CHANNEL_B,				// input video channel
  	CHANNELS_NUM			// max input channel
} gmt_INPUT_CHANNEL, gmt_DISPLAY_CHANNEL;


typedef enum
{
	StopScanSource,
  	BeginScanSource,		
  	CheckScanSource,
	FinishScanSource  	
} gmt_SCAN_SOURCE;
#define ScanningBit BIT0
#define AUTOSCAN_DELAY 		1800//Neil120802* Org = 1100  //Edward copy from 2410

enum ShortCutKeyEnum
{
	ShortCut_PresetMode_All = 0,			// 0
	ShortCut_PresetMode_Standard,		// 1
	ShortCut_PresetMode_Multimedia,		// 2
	ShortCut_PresetMode_Game,			// 3	
	ShortCut_PresetMode_Movie,			// 4
	ShortCut_PresetMode_Text,			// 5
	ShortCut_PresetMode_ColorTemp,       // 6	
	ShortCut_PresetMode_Warm,			// 7
	ShortCut_PresetMode_Cool,			// 8	
	ShortCut_PresetMode_ColorSpace,      // 9		
	ShortCut_PresetMode_AdobeRGB,		// 10
	ShortCut_PresetMode_sRGB,			// 11		
	ShortCut_PresetMode_xvMode,		// 12		
//&*&*&*S 20120417:SWRD4003: Kordonm(0)  Modify for OSD Workbench
//&*&*&*S 20120315:SWRD4001: Kordonm(0)  Modify for OSD Workbench
	ShortCut_PresetMode_CAL,			// 13
	ShortCut_PresetMode_PrinterPrevue,	// 14
	ShortCut_PresetMode_Printer1,		// 15
	ShortCut_PresetMode_Printer2,		// 16
	ShortCut_PresetMode_Printer3,		// 17
	ShortCut_PresetMode_CustomColor,	// 18	// 13		
	ShortCut_BrightnessContrast,			// 19	// 14	
	ShortCut_AutoAdjust,				// 20	// 15	
	ShortCut_PIPModePIP,				// 21	// 16
	ShortCut_PIPModePBP,				// 22	// 16
	ShortCut_PIPMode,					// 23	// 16
	ShortCut_InputSource_ALL,			// 24	// 17	
	ShortCut_InputSource_VGA,			// 25	// 18	
	ShortCut_InputSource_DVI1,			// 26	// 19	
	ShortCut_InputSource_DVI2,			// 27	// 20	
	ShortCut_InputSource_DP,			// 28	// 21	
	ShortCut_InputSource_DP3,			// 29	
	ShortCut_InputSource_HDMI,			// 30	// 22	
	ShortCut_InputSource_COMP,			// 31	// 23		
	ShortCut_InputSource_CVBS,			// 32	// 24	
	ShortCut_AspectRatio,				// 33	// 25
	ShortCut_MenuRotation,				// 34	// 26	
//&*&*&*E 20120315 : Kordonm(1)  Modify for OSD Workbench
//&*&*&*E 20120417 : Kordonm(1)  Modify for OSD Workbench
	MAX_SHORTCUTS
};

enum ColorModeEnum
{
	ColorMode_VIDEO,	// 0
	ColorMode_GRAPHIC	// 1
};

#if 1//120329 Edward for DELL new spec
enum GraphicsPresetModeEnum
{
	Graphics_STANDARD = 0,			// 0
	Graphics_MULTIMEDIA,			// 1
	Graphics_GAME,					// 2
	Graphics_MOVIE,					// 3	
	Graphics_TEXT,					// 4		
	Graphics_COLOR_TEMP,			//5
	Graphics_COLOR_SPACE,			//6
	//Graphics_PRINTER,				//7
	Graphics_CUSTOM_COLOR,			//8	
	MAX_GRAPHICS_MODES
};
#else
enum GraphicsPresetModeEnum
{
	Graphics_STANDARD = 0,			// 0
	Graphics_MULTIMEDIA,			// 1
	Graphics_GAME,					// 2
	Graphics_MOVIE,					// 3	
	Graphics_TEXT,					// 4		
	Graphics_WARM,					// 5
	Graphics_COLOR_TEMP,// = Graphics_WARM, //5
	Graphics_COOL,					// 6
	Graphics_COLOR_SPACE,
	Graphics_ADOBE_RGB,			// 7
	Graphics_SRGB,					// 8
	Graphics_xvMode,				// 9		
	Graphics_CUSTOM_COLOR,		// 10	
//&*&*&*S 20120315:SWRD4001: Kordonm(0)  Modify for OSD Workbench
	Graphics_CAL,   		  // 11
	Graphics_Printer1,      // 12
	Graphics_Printer2,      // 13
	Graphics_Printer3,      // 14	
//&*&*&*E 20120315 : Kordonm(1)  Modify for OSD Workbench
	MAX_GRAPHICS_MODES
};
#endif

#if 1 //DELL_U2410F //120209 Edward add from DELL2410
typedef enum
{
   	IS_OFF_GRAPHIC,     // Disables ACC/ACM
	IS_GAME_GRAPHIC,
   	IS_MOVIE_GRAPHIC,
   	IS_ON_GRAPHIC,      // Enables ACC/ACM
   	IS_MAX_GRAPHIC = IS_ON_GRAPHIC, // MAX ImageScheme
} gmt_IMAGE_SCHEME_MODE_GRAPHIC;

typedef enum
{
	IS_OFF_VIDEO,     // Disables ACC/ACM
	IS_MOVIE_VIDEO,
	IS_GAME_VIDEO,
	IS_NATURE_VIDEO,
	IS_XVMODE_VIDEO,  //zuguang 20090320
	IS_ON_VIDEO,      // Enables ACC/ACM
	IS_MAX_VIDEO = IS_ON_VIDEO, // MAX ImageScheme
} gmt_IMAGE_SCHEME_MODE_VIDEO;
#endif

#define APERTURE_6AXIS      2
#define HUE_FADE_6AXIS      52
#define SAT_R1_LOW         8 //30
#define SAT_R2_HIGH         180
#define SAT_R_FADE         10
#define LUM_L1_LOW         6
#define LUM_L2_HIGH        230 //255 //KevinL 20090428 updated from Vincent
#define LUM_L_FADE         5
#define HUE_GAIN         0x00   //no gain adjustment
#define HUE_OFFSET         0x00   //0
#define SAT_GAIN         0x40   //64/64
#define SAT_OFFSET         0x00   //0
#define LUM_GAIN         0x40   //64/64
#define LUM_OFFSET         0x00   //0

#define HUE_CENTER_MAGENTA		50		
#define HUE_CENTER_RED			     109		     
#define HUE_CENTER_YELLOW		     171		
#define HUE_CENTER_GREEN		     230		
#define HUE_CENTER_CYAN		     290		
#define HUE_CENTER_BLUE		     350		


enum VideoPresetModeEnum
{
	Video_MOVIE = 0,	// 0
	Video_GAME,		// 1
	Video_NATURE,		// 2
	Video_xvMode,          // 3
	MAX_VIDEO_MODES
};

enum CustomColorEnum //KevinL 20090116
{
	CustomColor_Gain = 0,	// 0
	CustomColor_Offset,		// 1
	CustomColor_Hue,		// 2
	CustomColor_Saturation, // 3
};

enum MainNoSyncCheckEnum  //KevinL 20090325 for power on to determine PIP/PBP should be turned on or not.	
{
	STOP_CHECK = 0,			// 0
	STOP_CHECK_FOR_ICON,	// 1
	START_CHECK,			// 2
};

#define IsShortCut1_PresetItems	(UserPrefPersonalKey1 <= ShortCut_PresetMode_CustomColor)
#define IsShortCut2_PresetItems	(UserPrefPersonalKey2 <= ShortCut_PresetMode_CustomColor)
#define IsShortCut3_PresetItems	(UserPrefPersonalKey3 <= ShortCut_PresetMode_CustomColor)
#define IsShortCut1_InputItems	((UserPrefPersonalKey1 >= ShortCut_InputSource_ALL) && (UserPrefPersonalKey1 <= ShortCut_InputSource_CVBS))
#define IsShortCut2_InputItems	((UserPrefPersonalKey2 >= ShortCut_InputSource_ALL) && (UserPrefPersonalKey2 <= ShortCut_InputSource_CVBS))
#define IsShortCut3_InputItems	((UserPrefPersonalKey3 >= ShortCut_InputSource_ALL) && (UserPrefPersonalKey3 <= ShortCut_InputSource_CVBS))

#define IsColorMode_VODEO		(UserPrefColorMode == ColorMode_VIDEO)
#define IsColorMode_GRAPHIC		(UserPrefColorMode == ColorMode_GRAPHIC)
#define IsPipColorMode_VODEO	(UserPrefPipColorMode == ColorMode_VIDEO)
#define IsPipColorMode_GRAPHIC	(UserPrefPipColorMode == ColorMode_GRAPHIC)
#define OSD_IsGraphics			(IsColorMode_GRAPHIC)

#define IsPresetMode_GRAPHICS_STANDARD			(IsColorMode_GRAPHIC && (UserPrefGraphicsImageMode == Graphics_STANDARD))
#define IsPresetMode_GRAPHICS_MULTIMEDIA		(IsColorMode_GRAPHIC && (UserPrefGraphicsImageMode == Graphics_MULTIMEDIA))
#define IsPresetMode_GRAPHICS_GAME				(IsColorMode_GRAPHIC && (UserPrefGraphicsImageMode == Graphics_GAME))
#define IsPresetMode_GRAPHICS_MOVIE				(IsColorMode_GRAPHIC && (UserPrefGraphicsImageMode == Graphics_MOVIE))
#define IsPresetMode_GRAPHICS_TEXT			    (IsColorMode_GRAPHIC && (UserPrefGraphicsImageMode == Graphics_TEXT))
//&*&*&*S 20120315:SWRD4001: Kordonm(0)  Modify for OSD Workbench
#define IsPresetMode_GRAPHICS_COLOR_TEMP		(IsColorMode_GRAPHIC && (UserPrefGraphicsImageMode == Graphics_COLOR_TEMP))
//&*&*&*E 20120315 : Kordonm(1)  Modify for OSD Workbench
//#define IsPresetMode_GRAPHICS_WARM				(IsColorMode_GRAPHIC && (UserPrefGraphicsImageMode == Graphics_WARM))
//#define IsPresetMode_GRAPHICS_COOL				(IsColorMode_GRAPHIC && (UserPrefGraphicsImageMode == Graphics_COOL))
#define IsPresetMode_GRAPHICS_COLOR_SPACE		(IsColorMode_GRAPHIC && (UserPrefGraphicsImageMode == Graphics_COLOR_SPACE))
#define IsColorSpace_GRAPHICS_ADOBE_RGB			(IsColorMode_GRAPHIC && (UserPrefGraphicsImageMode == Graphics_COLOR_SPACE)&&(UserPrefPresetColorSpace == CSPACE_AdobeRGB))
#define IsColorSpace_GRAPHICS_SRGB				(IsColorMode_GRAPHIC && (UserPrefGraphicsImageMode == Graphics_COLOR_SPACE)&&(UserPrefPresetColorSpace == CSPACE_sRGB))
#define IsColorSpace_GRAPHICS_CAL1				(IsColorMode_GRAPHIC && (UserPrefGraphicsImageMode == Graphics_COLOR_SPACE)&&(UserPrefPresetColorSpace == CSPACE_CAL1))
#define IsColorSpace_GRAPHICS_CAL2				(IsColorMode_GRAPHIC && (UserPrefGraphicsImageMode == Graphics_COLOR_SPACE)&&(UserPrefPresetColorSpace == CSPACE_CAL2))
#define IsPresetMode_GRAPHICS_CUSTOM_COLOR		(IsColorMode_GRAPHIC && (UserPrefGraphicsImageMode == Graphics_CUSTOM_COLOR))

#define IsPipPresetMode_GRAPHICS_STANDARD			(IsPipColorMode_GRAPHIC && (UserPrefPipGraphicsImageMode == Graphics_STANDARD))
#define IsPipPresetMode_GRAPHICS_MULTIMEDIA			(IsPipColorMode_GRAPHIC && (UserPrefPipGraphicsImageMode == Graphics_MULTIMEDIA))
#define IsPipPresetMode_GRAPHICS_GAME				(IsPipColorMode_GRAPHIC && (UserPrefPipGraphicsImageMode == Graphics_GAME))
#define IsPipPresetMode_GRAPHICS_COLOR_TEMP			(IsPipColorMode_GRAPHIC && (UserPrefPipGraphicsImageMode == Graphics_COLOR_TEMP))
//#define IsPipPresetMode_GRAPHICS_WARM				(IsPipColorMode_GRAPHIC && (UserPrefPipGraphicsImageMode == Graphics_WARM))
//#define IsPipPresetMode_GRAPHICS_COOL				(IsPipColorMode_GRAPHIC && (UserPrefPipGraphicsImageMode == Graphics_COOL))
#define IsPipPresetMode_GRAPHICS_COLOR_SPACE		(IsPipColorMode_GRAPHIC && (UserPrefPipGraphicsImageMode == Graphics_COLOR_SPACE))
//#define IsPipPresetMode_GRAPHICS_ADOBE_RGB			(IsPipColorMode_GRAPHIC && (UserPrefPipGraphicsImageMode == Graphics_ADOBE_RGB))
//#define IsPipPresetMode_GRAPHICS_SRGB				(IsPipColorMode_GRAPHIC && (UserPrefPipGraphicsImageMode == Graphics_SRGB))
#define IsPipPresetMode_GRAPHICS_CUSTOM_COLOR		(IsPipColorMode_GRAPHIC && (UserPrefPipGraphicsImageMode == Graphics_CUSTOM_COLOR))

#define IsPresetMode_VIDEO_MOVIE		(IsColorMode_VODEO && (UserPrefVideoImageMode == Video_MOVIE))
#define IsPresetMode_VIDEO_GAME			(IsColorMode_VODEO && (UserPrefVideoImageMode == Video_GAME))
#define IsPresetMode_VIDEO_NATURE		(IsColorMode_VODEO && (UserPrefVideoImageMode == Video_NATURE))
#define IsPresetMode_VIDEO_xvMode		(IsColorMode_VODEO && (UserPrefVideoImageMode == Video_xvMode))

#define IsPipPresetMode_VIDEO_MOVIE		(IsPipColorMode_VODEO && (UserPrefPipVideoImageMode == Video_MOVIE))
#define IsPipPresetMode_VIDEO_GAME		(IsPipColorMode_VODEO && (UserPrefPipVideoImageMode == Video_GAME))
#define IsPipPresetMode_VIDEO_NATURE	(IsPipColorMode_VODEO && (UserPrefPipVideoImageMode == Video_NATURE))
#define IsPipPresetMode_VIDEO_xvMode	(IsPipColorMode_VODEO && (UserPrefPipVideoImageMode == Video_xvMode))


#define  Warning_Message_BriCon (IsPresetMode_GRAPHICS_STANDARD || IsPresetMode_GRAPHICS_COLOR_TEMP || IsColorSpace_GRAPHICS_SRGB || IsColorSpace_GRAPHICS_ADOBE_RGB )//owen 20120731



//#define IsPresetMode_ColorTemp 			((UserPrefGraphicsImageMode == Graphics_WARM) || (UserPrefGraphicsImageMode == Graphics_COOL))
//&*&*&*S 20120315:SWRD4001: Kordonm(0)  Modify for OSD Workbench
//#define IsPresetMode_ColorSpace 			((UserPrefGraphicsImageMode == Graphics_ADOBE_RGB) || (UserPrefGraphicsImageMode == Graphics_SRGB) || (UserPrefGraphicsImageMode == Graphics_xvMode) || (UserPrefGraphicsImageMode == Graphics_CAL))
//#define IsPresetMode_ColorSpace 			((UserPrefGraphicsImageMode == Graphics_ADOBE_RGB) || (UserPrefGraphicsImageMode == Graphics_SRGB) || (UserPrefGraphicsImageMode == Graphics_xvMode))
//#define IsPresetMode_GRAPHICS_PRINTER 	(UserPrefGraphicsImageMode == Graphics_PRINTER)
//&*&*&*E 20120315 : Kordonm(1)  Modify for OSD Workbench


#ifdef DELL_U2410F
//zuguang 20090220 >
//zuguang 20090320 added xvMode
#if 1//120713 Edward update
#define IsAccEnablePresetMode()		(IsPresetMode_GRAPHICS_MULTIMEDIA || IsPresetMode_GRAPHICS_MOVIE || IsPresetMode_GRAPHICS_GAME)
#else
#define IsAccEnablePresetMode()		(((UserPrefColorMode == ColorMode_VIDEO) && \
									((UserPrefVideoImageMode == Video_MOVIE) || (UserPrefVideoImageMode == Video_GAME) || (UserPrefVideoImageMode == Video_NATURE) || (IsInputColorFormat_YUV() && (UserPrefVideoImageMode == Video_xvMode)))) || \
									((UserPrefColorMode == ColorMode_GRAPHIC) && \
									((UserPrefGraphicsImageMode == Graphics_MOVIE) || (UserPrefGraphicsImageMode == Graphics_GAME))))//120608 Edward ACC / ACM for Movie / Game
#endif
#if 1////120713 Edward update
#define IsAcmEnablePresetMode()		(IsPresetMode_GRAPHICS_MULTIMEDIA || IsPresetMode_GRAPHICS_MOVIE || IsPresetMode_GRAPHICS_GAME)

/*#define IsAcmEnablePresetMode()		(((UserPrefColorMode == ColorMode_VIDEO) && \
									((UserPrefVideoImageMode == Video_MOVIE) || (UserPrefVideoImageMode == Video_GAME) || (UserPrefVideoImageMode == Video_NATURE) || (UserPrefVideoImageMode == Video_xvMode))) || \
									((UserPrefColorMode == ColorMode_GRAPHIC) && \
									((UserPrefGraphicsImageMode == Graphics_MOVIE) || (UserPrefGraphicsImageMode == Graphics_GAME) || (UserPrefGraphicsImageMode == Graphics_CUSTOM_COLOR))) || \
									(IsInputColorFormat_YUV() && IsPresetMode_GRAPHICS_COLOR_SPACE))
*/
#else									
//KevinL 20090508 updated for RGB xvMode
#define IsAcmEnablePresetMode()		(((UserPrefColorMode == ColorMode_VIDEO) && \
									((UserPrefVideoImageMode == Video_MOVIE) || (UserPrefVideoImageMode == Video_GAME) || (UserPrefVideoImageMode == Video_NATURE) || (IsInputColorFormat_YUV() && (UserPrefVideoImageMode == Video_xvMode)))) || \
									((UserPrefColorMode == ColorMode_GRAPHIC) && \
									((UserPrefGraphicsImageMode == Graphics_MULTIMEDIA) || (UserPrefGraphicsImageMode == Graphics_GAME) || (UserPrefGraphicsImageMode == Graphics_CUSTOM_COLOR))) || \
									(IsInputColorFormat_YUV() && (IsPresetMode_GRAPHICS_SRGB || IsPresetMode_GRAPHICS_ADOBE_RGB)))

#endif									
//zuguang 20090220 <
#else
#define IsAccEnablePresetMode()		((UserPrefColorFormat == InputColorFormat_YUV) || \
									(UserPrefGraphicsImageMode == Graphics_MULTIMEDIA) || (UserPrefGraphicsImageMode == Graphics_GAME))

#define IsAcmEnablePresetMode()		((UserPrefColorFormat == InputColorFormat_YUV) || \
									(UserPrefGraphicsImageMode == Graphics_MULTIMEDIA) || (UserPrefGraphicsImageMode == Graphics_GAME) || \
									(UserPrefGraphicsImageMode == Graphics_CUSTOM_COLOR))
#endif

#define IsPipAccEnablePresetMode()	((UserPrefPipColorFormat == InputColorFormat_YUV) || \
									(UserPrefPipGraphicsImageMode == Graphics_MOVIE) || (UserPrefPipGraphicsImageMode == Graphics_GAME))//120608 Edward ACC / ACM for Movie / Game

#define IsPipAcmEnablePresetMode()	((UserPrefPipColorFormat == InputColorFormat_YUV) || \
									(UserPrefPipGraphicsImageMode == Graphics_MOVIE) || (UserPrefPipGraphicsImageMode == Graphics_GAME) || \
									(UserPrefPipGraphicsImageMode == Graphics_CUSTOM_COLOR))

//#define IsPresetMode_DCR_ON()	((IsPresetMode_VIDEO_MOVIE || IsPresetMode_VIDEO_GAME || IsPresetMode_VIDEO_NATURE || IsPresetMode_GRAPHICS_GAME))
#define IsPresetMode_DCR_ON()	((IsPresetMode_GRAPHICS_MOVIE || IsPresetMode_GRAPHICS_GAME))

#define IsBrightnessAdjustBacklight()	(IsGraphicInput(UserPrefCurrentInputMain))

#define IS_DCR_ON()					((UserPrefDCR == On) && IsPresetMode_DCR_ON() && (UserPrefPipMode == NO_PIP)) 
//#define IS_OSD_DCR_ENABLE()		(IsPresetMode_DCR_ON() && (UserPrefPipMode == NO_PIP)) //KevinL 20090423
#define IS_OSD_DCR_ENABLE()			(IsPresetMode_DCR_ON() && (UserPrefEnergySmartOnOffFlag == Off) && (UserPrefPipMode == NO_PIP) && (UserPrefDUMSwitch==UNIFORMITY_OFF)) //owen 20120712 add DUMSwitch
#define IsBrightnessItemGreyout		FALSE //KevinL 20090114 For DELL new Spec. No Gary-out  
#define IS_ENERGY_SMART_ON()		((UserPrefEnergySmartOnOffFlag == On) && (UserPrefPipMode == NO_PIP)) 
#define IS_ENERGY_SMART_Enable()  	(IS_UNIFORMITY_Off()  && (UserPrefPipMode == NO_PIP))	//owen 20120624

 //#define IS_UNIFORMITY_ON()	        (((UserPrefDUMSwitch==UNIFORMITY_Table1) ||(UserPrefDUMSwitch==UNIFORMITY_Table2)) && (UserPrefDCR == Off) && !IS_ENERGY_SMART_ON())  //owen 20120712
#define IS_UNIFORMITY_ON()	        ((UserPrefDUMSwitch==UNIFORMITY_Table1) ||(UserPrefDUMSwitch==UNIFORMITY_Table2))  //owen 20120712
#define IS_UNIFORMITY_Off()       	(UserPrefDUMSwitch==UNIFORMITY_OFF)
#define IS_UNIFORMITY_GrayOut   	(IsPresetMode_GRAPHICS_MULTIMEDIA||IsPresetMode_GRAPHICS_MOVIE||IsPresetMode_GRAPHICS_GAME||IsPresetMode_GRAPHICS_TEXT||IsPresetMode_GRAPHICS_COLOR_SPACE)
#define IS_UNIFORMITY_Enable()   	((UserPrefDCR != On) && !IS_ENERGY_SMART_ON() && (!IS_UNIFORMITY_GrayOut))

#define Is_OSD_NR_ON()				(IsPresetMode_GRAPHICS_MOVIE)
#define Is_SIDE_BY_SIDE()           		(UserPrefPipMode == SIDE_BY_SIDE)
#define Is_PIP_ON()            			(UserPrefPipMode != NO_PIP)
#define Is_Pxp_Off()					(UserPrefPipMode == NO_PIP)
#define Is_PIP_Pip()            			((UserPrefPipMode == SMALL_PIP)||(UserPrefPipMode == LARGE_PIP)||(UserPrefPipMode == MEDIUM_PIP)||(UserPrefPipMode == HUGE_PIP))
#if(defined(BOARD_93XX_MNT))
#define IS_PBP()			(UserPrefPipMode == SIDE_BY_SIDE)
#elif((defined(BOARD_93XX_SUV))||(defined(BOARD_93XX_4K2K)))
#define IS_PBP()			((UserPrefPipMode == SIDE_BY_SIDE)||(UserPrefPipMode == SIDE_BY_SIDE_TALL))
#else		//BOARD_93XX_PD
#define IS_PBP()			((UserPrefPipMode == SIDE_BY_SIDE)||(UserPrefPipMode == SIDE_BY_SIDE_PUP)||(UserPrefPipMode == SIDE_BY_SIDE_TALL))
#endif



enum UniformityEnum
{

	Uniformity_Brightness,			// 0
	Uniformity_Contrast	// 1	
};

enum ColorSpaceEnum
{
	ColorSpace_RGB,			// 0
	ColorSpace_YCbCr422 ,	// 1
	ColorSpace_YCbCr444	// 2	
};

enum InputColorFormatEnum
{
#ifdef DELL_U2410F//120523 Edward remapping for standard code
	InputColorFormat_RGB=Input_RGB,		// 1
	InputColorFormat_YUV=Input_YUV_Full	// 2
#else
	InputColorFormat_RGB,	// 0
	InputColorFormat_YUV	// 1
#endif	
};

#define IsInputColorFormat_RGB_CH(x)      	((x == gmvb_MainChannel)? (IsInputColorFormat_RGB()): (IsPipInputColorFormat_RGB())) //KevinL 20090527 
#define IsInputColorFormat_RGB()			(UserPrefColorFormat == InputColorFormat_RGB)
#define IsInputColorFormat_YUV()			(UserPrefColorFormat == InputColorFormat_YUV)
#define IsPipInputColorFormat_RGB()			(UserPrefPipColorFormat == InputColorFormat_RGB)
#define IsPipInputColorFormat_YUV()			(UserPrefPipColorFormat == InputColorFormat_YUV)
#define IsInputColor_YPbPr()				(gm_ReadRegWord(IMP_CONTROL) & IMP_YUV_EN)//120717 Edward add for ACC / ACM

enum GammaEnum
{
	Gamma_PC,
	Gamma_MAC
};

typedef struct VideoTypeStruct
{
	SWORD Hue;
	WORD Saturation;
} VideoType;

typedef struct VideoPresetModesTypeStruct
{
	VideoType Movie;
	VideoType Game;
	VideoType Nature;
	VideoType xvMode;	
} VideoPresetModesType;

typedef struct PresetModesTypeStruct
{
	//GraphicPresetModesType GraphicPresetMode;	// Removed by ST_hansPENG 090225
	VideoPresetModesType VideoPresetMode;
} PresetModesType;

#define PresetModesUserPrefSize	sizeof(PresetModesType)


#define DIGITAL_SW 					GPIO31_EN
#define LPM_POWER   					LPM_GPIO0_EN
#define LPM_POWER_OFF 				LPM_GPIO3_EN

#if 0//def Athena_C	//For 73xx

#if(BOARD_USED == DELL_93xx_BOARD)
#define USB_EN_EN					LPM_GPIO4_EN
#define USB_EN_DIR					LPM_GPIO4_IO
#define USB_EN_OD					LPM_GPIO4_OD
#define USB_EN_PIN		       		LPM_GPIO4_OUT
#endif
//GPIO 71 : NVRAM WP					//GPOUTPUT4
#define NVRAM_WP_PORT				GPOUTPUT4
#define NVRAM_WP_EN					GPIO71_EN
#define NVRAM_WP_DIR				GPIO71_IO
#define NVRAM_WP_OD					GPIO71_OD
#define NVRAM_WP_PIN		       	GPIO71_OUT

//GPIO 72 : EDID WP					//GPOUTPUT4
#define EDID_WP_PORT				LPM_GPOUTPUT_1
#define EDID_WP_EN					LPM_GPIO19_EN
#define EDID_WP_DIR					LPM_GPIO19_IO
#define EDID_WP_OD					LPM_GPIO19_OD
#define EDID_WP_PIN		       		LPM_GPIO19_OUT

//GPIO84: Buzzer adjust (PWM3)			//GPOUTPUT5
#define BUZZER_ADJ_EN        		GPIO84_EN 
//#define BUZZER_ADJ_DIR				GPIO84_IO
//#define BUZZER_ADJ_OD				GPIO84_OD
//#define BUZZER_ADJ_PIN				GPIO84_OUT

#if(BOARD_USED == DELL_93xx_BOARD)
//GPIO 85 : Sensor					//GPINPUT5
#define ROTATION_SENSOR_PORT		GPINPUT5
#define ROTATION_SENSOR_EN			GPIO85_EN
#define ROTATION_SENSOR_DIR			GPIO85_IO
#define ROTATION_SENSOR_OD			GPIO85_OD
#define ROTATION_SENSOR_PIN		    GPIO85_IN
#endif

#define AUDIO_MUTE_PORT				GPOUTPUT5
#define AUDIO_MUTE_EN        		GPIO86_EN  
#define AUDIO_MUTE_DIR				GPIO86_IO
#define AUDIO_MUTE_OD				GPIO86_OD
#define AUDIO_MUTE_PIN				GPIO86_OUT


#define FLASH_WP_PORT				GPOUTPUT5
#define FLASH_WP_EN					GPIO89_EN
#define FLASH_WP_DIR				GPIO89_IO
#define FLASH_WP_OD					GPIO89_OD
#define FLASH_WP_PIN		       	GPIO89_OUT


//GPIO 81 : Backlight diming control		//GPOUTPUT5
#define BKL_DIM_B_EN				GPIO81_EN
#define BKL_DIM_B_DIR				GPIO81_IO
#define BKL_DIM_B_OD				GPIO81_OD
#define BKL_DIM_B_PIN		       	GPIO81_OUT

//GPIO 82 : Backlight diming control		//GPOUTPUT5
#define BKL_DIM_G_EN				GPIO82_EN
#define BKL_DIM_G_DIR				GPIO82_IO
#define BKL_DIM_G_OD				GPIO82_OD
#define BKL_DIM_G_PIN		       	GPIO82_OUT

//GPIO 83 : DCR control(PWM2)			//GPOUTPUT5
#define PWM_DCR_EN					GPIO83_EN
#define PWM_DCR_DIR					GPIO83_IO
#define PWM_DCR_OD					GPIO83_OD
#define PWM_DCR_PIN		       		GPIO83_OUT

#if(BOARD_USED == DELL_93xx_BOARD)
//GPIO90: Audio Detect 				//GPOUTPUT5
#define AUDIO_DET_PORT            	GPINPUT5 
#define AUDIO_DET_EN            	GPIO90_EN 
#define AUDIO_DET_DIR				GPIO90_IO
#define AUDIO_DET_OD				GPIO90_OD
#define AUDIO_DET_PIN				GPIO90_IN
#endif

//GPIO 91 : Display output detect		//GPOUTPUT5
#define DP_OUTPUT_DET_EN			GPIO91_EN
#define DP_OUTPUT_DET_DIR			GPIO91_IO
#define DP_OUTPUT_DET_OD			GPIO91_OD
#define DP_OUTPUT_DET_PIN		   	GPIO91_OUT

#if(BOARD_USED == DELL_93xx_BOARD)
//GPIO 5 : EDID WP					//GPOUTPUT4
#define HEADPHONE_MUTE_PORT				GPOUTPUT5
#define HEADPHONE_MUTE_EN					GPIO92_EN
#define HEADPHONE_MUTE_DIR				GPIO92_IO
#define HEADPHONE_MUTE_OD					GPIO92_OD
#define HEADPHONE_MUTE_PIN		       	GPIO92_OUT

//GPIO92: Audio 12V power Control
#define AUDIO_POWER_PORT	       	GPOUTPUT5 
#define AUDIO_POWER_EN      	  	GPIO92_EN 
#define AUDIO_POWER_DIR				GPIO92_IO
#define AUDIO_POWER_OD				GPIO92_OD
#define AUDIO_POWER_PIN				GPIO92_OUT
//LPM_GPIO 0 : DP switch GPU Select(select DP or Mini DP input)
#define DP_SWITCH_EN				LPM_GPIO0_EN
#define DP_SWITCH_DIR				LPM_GPIO0_IO
#define DP_SWITCH_OD				LPM_GPIO0_OD
#define DP_SWITCH_PIN		   		LPM_GPIO0_OUT

//LPM_GPIO 9 : Power saving voltage :LPM_GPIO_ENABLE1
#define PSV_EN						LPM_GPIO9_EN
#define PSV_DIR						LPM_GPIO9_IO
#define PSV_OD						LPM_GPIO9_OD
#define PSV_PIN		   				LPM_GPIO9_OUT

#define CHARGE_EN                       GPADC_ADC3IN_DIS
#define CHARGE_DIR                      GPADC_GPIO3_IO
#define CHARGE_OD                     	BIT11	// GPADC_GPIO3_OD
#define CHARGE_PIN                      GPADC_GPIO3_OUT

//LPM GPADC_AN0 : USB Detect
#define USB_DET_EN						GPADC_ADC0IN_DIS
#define USB_DET_DIR						GPADC_GPIO0_IO
//#define USB_DET_OD					BIT8
#define USB_DET_PIN		   				GPADC_GPIO0_OUT
#endif

#define TMDS_EN                       		GPADC_ADC3IN_DIS
#define TMDS_DIR                      		GPADC_GPIO3_IO
#define TMDS_OD                     			BIT11	// GPADC_GPIO3_OD
#define TMDS_PIN                      		GPADC_GPIO3_OUT

#ifdef BOARD_93xx_TK
#define LED_GREEN_PORT	       		GPOUTPUT4 
#define LED_GREEN_EN      	  		GPIO79_EN 
#define LED_GREEN_DIR				GPIO79_IO
#define LED_GREEN_OD				GPIO79_OD
#define LED_GREEN_PIN				GPIO79_OUT
#else
//GPIO85: LED Green control
#define LED_GREEN_PORT	       		GPOUTPUT5
#define LED_GREEN_EN      	  		GPIO85_EN 
#define LED_GREEN_DIR				GPIO85_IO
#define LED_GREEN_OD				GPIO85_OD
#define LED_GREEN_PIN				GPIO85_OUT
#endif


#else		//#elif (defined (Athena_U))			

#if 0//(BOARD_USED == DELL_93xx_BOARD)
//GPIO 11 : Sensor					//GPINPUT0
#define ROTATION_SENSOR_PORT			GPINPUT0
#define ROTATION_SENSOR_EN			GPIO11_EN
#define ROTATION_SENSOR_DIR			GPIO11_IO
#define ROTATION_SENSOR_OD			GPIO11_OD
#define ROTATION_SENSOR_PIN		    	GPIO11_IN

//GPIO21: Audio 12V power Control
#define AUDIO_POWER_PORT	       	GPOUTPUT1 
#define AUDIO_POWER_EN      	  		GPIO21_EN 
#define AUDIO_POWER_DIR			GPIO21_IO
#define AUDIO_POWER_OD			GPIO21_OD
#define AUDIO_POWER_PIN			GPIO21_OUT

//GPIO22: Audio Detect 				//GPINPUT1
#define AUDIO_DET_PORT            		GPINPUT1 
#define AUDIO_DET_EN            		GPIO22_EN 
#define AUDIO_DET_DIR				GPIO22_IO
#define AUDIO_DET_OD				GPIO22_OD
#define AUDIO_DET_PIN				GPIO22_IN
//GPIO 81 : Backlight diming control		//GPOUTPUT5
#define BKL_DIM_B_EN				GPIO81_EN
#define BKL_DIM_B_DIR				GPIO81_IO
#define BKL_DIM_B_OD				GPIO81_OD
#define BKL_DIM_B_PIN		       	GPIO81_OUT

//GPIO 83 : Backlight diming control		//GPOUTPUT5
#define BKL_DIM_G_EN				GPIO82_EN
#define BKL_DIM_G_DIR				GPIO82_IO
#define BKL_DIM_G_OD				GPIO82_OD
#define BKL_DIM_G_PIN		       	GPIO82_OUT
#endif


#if(BOARD_USED == BOARD_93xx_CJ2)
//GPIO86: Audio Mute	Hyb130417: GPIO81 ==>GPIO86;
#define AUDIO_MUTE_PORT				GPOUTPUT5
#define AUDIO_MUTE_EN        		GPIO95_EN  
#define AUDIO_MUTE_DIR				GPIO95_IO
#define AUDIO_MUTE_OD				GPIO95_OD
#define AUDIO_MUTE_PIN				GPIO95_OUT
#else
//GPIO86: Audio Mute	Hyb130417: GPIO81 ==>GPIO86;
#define AUDIO_MUTE_PORT				GPOUTPUT5
#define AUDIO_MUTE_EN        		GPIO86_EN  
#define AUDIO_MUTE_DIR				GPIO86_IO
#define AUDIO_MUTE_OD				GPIO86_OD
#define AUDIO_MUTE_PIN				GPIO86_OUT
#endif

//GPIO 90 : NVRAM WP					//GPOUTPUT5
#define NVRAM_WP_PORT				GPOUTPUT5
#define NVRAM_WP_EN					GPIO90_EN
#define NVRAM_WP_DIR				GPIO90_IO
#define NVRAM_WP_OD					GPIO90_OD
#define NVRAM_WP_PIN		       	GPIO90_OUT

//GPIO91 : EDID WP					//GPOUTPUT5
#define EDID_WP_PORT				GPOUTPUT5
#define EDID_WP_EN					GPIO91_EN
#define EDID_WP_DIR					GPIO91_IO
#define EDID_WP_OD					GPIO91_OD
#define EDID_WP_PIN		       		GPIO91_OUT

#if((BOARD_USED == BOARD_93xx_CJ2))
//GPIO 92 : HeadPhone Mute					//GPOUTPUT5
#define HEADPHONE_MUTE_PORT			GPOUTPUT5
#define HEADPHONE_MUTE_EN			GPIO92_EN
#define HEADPHONE_MUTE_DIR			GPIO92_IO
#define HEADPHONE_MUTE_OD			GPIO92_OD
#define HEADPHONE_MUTE_PIN		    GPIO92_OUT
#endif

//GPIO 93 : flash WP					//GPOUTPUT5
#define FLASH_WP_PORT				GPOUTPUT5
#define FLASH_WP_EN					GPIO93_EN
#define FLASH_WP_DIR				GPIO93_IO
#define FLASH_WP_OD					GPIO93_OD
#define FLASH_WP_PIN		       	GPIO93_OUT


#if((BOARD_USED == BOARD_93xx_CJ2))
#define TMDS_EN_PORT				LPM_GPOUTPUT_0
#define TMDS_EN                     LPM_GPIO5_EN
#define TMDS_DIR                    LPM_GPIO5_IO
#define TMDS_OD                     LPM_GPIO5_OD	
#define TMDS_PIN                    LPM_GPIO5_OUT

#else
#define TMDS_EN                     GPADC_ADC3IN_DIS
#define TMDS_DIR                    GPADC_GPIO3_IO
#define TMDS_OD                     BIT11	// GPADC_GPIO3_OD
#define TMDS_PIN                    GPADC_GPIO3_OUT
#endif

#ifdef BOARD_93xx_TK
#define LED_GREEN_PORT	       		GPOUTPUT4 
#define LED_GREEN_EN      	  		GPIO79_EN 
#define LED_GREEN_DIR				GPIO79_IO
#define LED_GREEN_OD				GPIO79_OD
#define LED_GREEN_PIN				GPIO79_OUT
#elif((BOARD_USED == BOARD_93xx_CJ2))
//GPIO94: LED Green control
#define LED_GREEN_PORT	       		GPOUTPUT5
#define LED_GREEN_EN      	  		GPIO94_EN 
#define LED_GREEN_DIR				GPIO94_IO
#define LED_GREEN_OD				GPIO94_OD
#define LED_GREEN_PIN				GPIO94_OUT
#else
//GPIO85: LED Green control
#define LED_GREEN_PORT	       		GPOUTPUT5
#define LED_GREEN_EN      	  		GPIO85_EN 
#define LED_GREEN_DIR				GPIO85_IO
#define LED_GREEN_OD				GPIO85_OD
#define LED_GREEN_PIN				GPIO85_OUT
#endif

//GPIO88: LED Green control
#define LED_BLINK_PORT	       		GPOUTPUT5
#define LED_BLINK_EN      	  		GPIO88_EN 
#define LED_BLINK_DIR				GPIO88_IO
#define LED_BLINK_OD				GPIO88_OD
#define LED_BLINK_PIN				GPIO88_OUT


#if(DEFAULT_PANEL == SHARP_1080P_LD900D3LE1)
#if((BOARD_USED == BOARD_93xx_CJ2))
//GPIO88: use TVP5160 detect to control	Sharp 90" Panel backlight on/off
#define SHARP_BL_EN_PORT			GPOUTPUT5
#define SHARP_BL_EN_EN        		GPIO88_EN  
#define SHARP_BL_EN_DIR				GPIO88_IO
#define SHARP_BL_EN_OD				GPIO88_OD
#define SHARP_BL_EN_PIN				GPIO88_OUT
#else
//CJ1 use Audio Mute pin to control backlight on/off
#define SHARP_BL_EN_PORT			GPOUTPUT5
#define SHARP_BL_EN_EN        		GPIO86_EN  
#define SHARP_BL_EN_DIR				GPIO86_IO
#define SHARP_BL_EN_OD				GPIO86_OD
#define SHARP_BL_EN_PIN				GPIO86_OUT
#endif
#endif	//(DEFAULT_PANEL == SHARP_1080P_LD900D3LE1)

#endif

#if(BOARD_USED == DELL_93xx_BOARD)
#define IsAUDIO_Detected()			(gm_ReadRegWord(AUDIO_DET_PORT)&AUDIO_DET_PIN)
#define AUDIO_POWER_ENABLE()	(gm_SetRegBitsWord(AUDIO_POWER_PORT, AUDIO_POWER_PIN))
#define AUDIO_POWER_DISABLE()	(gm_ClearRegBitsWord(AUDIO_POWER_PORT, AUDIO_POWER_PIN))
#endif

#if((BOARD_USED == BOARD_93xx_CJ2))
#define HEADPHONE_MUTE()			(gm_ClearRegBitsWord(HEADPHONE_MUTE_PORT, HEADPHONE_MUTE_PIN))
#define HEADPHONE_UNMUTE()		(gm_SetRegBitsWord(HEADPHONE_MUTE_PORT, HEADPHONE_MUTE_PIN))
#endif

#define AUDIO_MUTE()				(SetAudioDACOutput(OFF))//(gm_SetRegBitsWord(AUDIO_MUTE_PORT, AUDIO_MUTE_PIN))	//20120701 Kordonm test, for always auido un-mute test
#define AUDIO_UNMUTE()			(SetAudioDACOutput(ON))//(gm_ClearRegBitsWord(AUDIO_MUTE_PORT, AUDIO_MUTE_PIN))
#define IS_AUDIO_MUTE()			((gm_ReadRegWord(AUDIO_MUTE_PORT)&AUDIO_MUTE_PIN)?FALSE:TRUE)

#define NVRAM_WP_ENABLE()		(gm_SetRegBitsWord(NVRAM_WP_PORT, NVRAM_WP_PIN))
#define NVRAM_WP_DISABLE()		(gm_ClearRegBitsWord(NVRAM_WP_PORT, NVRAM_WP_PIN))

#define FLASH_WP_ENABLE()		(gm_ClearRegBitsWord(FLASH_WP_PORT, FLASH_WP_PIN))
#define FLASH_WP_DISABLE()		(gm_SetRegBitsWord(FLASH_WP_PORT, FLASH_WP_PIN))


#define IS_EDID_WP()				(gm_ReadRegWord(EDID_WP_PORT)&EDID_WP_PIN)

#ifdef PIVOT_SENSOR_ENABLE//120528 Edward update
#if (defined(CMI_73XX_BOARD))
#define Is_SENSOR_ROTATE90()	(gm_ReadRegWord(ROTATION_SENSOR_PORT)&ROTATION_SENSOR_PIN)
#else
#define Is_SENSOR_ROTATE90()	(!(gm_ReadRegWord(ROTATION_SENSOR_PORT)&ROTATION_SENSOR_PIN))
#endif
#define Is_OSDAutoRotate_ON()	(UserPrefOsdRotation_Auto == ON)
#define Is_OSDAutoRotate_OFF()	(UserPrefOsdRotation_Auto != ON)
#define Is_OSDRotated()			(UserPrefOsdRotation != OSD_NON_ROTATED)		//Neil120615+
#else
//120613 Edward for Wistron no rotation compile error
#define Is_OSDAutoRotate_ON()	FALSE
#define Is_OSDAutoRotate_OFF()	TRUE
#define Is_OSDRotated()			(UserPrefOsdRotation != OSD_NON_ROTATED)		//20120625 Modify for OSD Rotation180
#define Is_SENSOR_ROTATE90()    FALSE

#ifdef PIVOT_SENSOR_POWER_CONTROL
// Pivot Sensor Power Control
#define PIVOT_SENSOR_POWER_ENABLE	GPIO_5_ENABLE
#define PIVOT_SENSOR_POWER_DIRCTRL	GPIO_5_DIRCTRL
#define PIVOT_SENSOR_POWER_OUTPUT	GPOUTPUT5
#define PIVOT_SENSOR_POWER_CTRL		GPIO82_OUT
#define SetPivotSensorPower_ON()		(gm_SetRegBitsWord(PIVOT_SENSOR_POWER_OUTPUT, PIVOT_SENSOR_POWER_CTRL))
#define SetPivotSensorPower_OFF()		(gm_ClearRegBitsWord(PIVOT_SENSOR_POWER_OUTPUT, PIVOT_SENSOR_POWER_CTRL))
#endif

#endif	//PIVOT_SENSOR_ENABLE

#define Is_OSDRotated90()			((UserPrefOsdRotation == OSD_ROTATED_90)||(UserPrefOsdRotation == OSD_ROTATED_180))		//20120626 Modify for OSD Rotation180, Kordonm


#ifdef USB_POWER_CONTROL
// USB Port Connect Detect
#define USB_CONNECT_DETECT_ENABLE	GPIO_4_ENABLE
#define USB_CONNECT_DETECT_DIRCTRL	GPIO_4_DIRCTRL
#define USB_CONNECT_DETECT_INPUT		GPINPUT4
#define USB_CONNECT_DETECT_CTRL		GPIO66_IN
#define IsUsbConnect()				((gm_ReadRegWord(USB_CONNECT_DETECT_INPUT) & USB_CONNECT_DETECT_CTRL) == USB_CONNECT_DETECT_CTRL)

// USB Power Control
#define USB_POWER_ENABLE		GPIO_0_ENABLE
#define USB_POWER_DIRCTRL		GPIO_0_DIRCTRL
#define USB_POWER_OUTPUT		GPOUTPUT0
#define USB_POWER_CTRL			GPIO6_OUT
#define SetUsbPower_ON()		(gm_SetRegBitsWord(USB_POWER_OUTPUT, USB_POWER_CTRL))
#define SetUsbPower_OFF()		(gm_ClearRegBitsWord(USB_POWER_OUTPUT, USB_POWER_CTRL))
#endif



#ifdef BUZZER_USED
// Buzzer define
extern BYTE B_BuzzerControlFlag;

enum BuzzerStateEnum
{
	BuzzerState_IDLE,			// 0
	BuzzerState_3KHz_START,	// 1
	BuzzerState_4K7Hz_START,	// 2
	BuzzerState_SINGING,		// 3
	BuzzerState_STOP			// 4
};

enum BuzzerFrequencyEnum
{
	BuzzerFrequency_OFF,		// 0
	BuzzerFrequency_3KHz,		// 1	// 3.2KHz
	BuzzerFrequency_4K7Hz		// 2	// 4.8KHz
};

#define BUZZER_PWM_PULSE_SHIFT		8
#define BUZZER_PWM_PERIOD_3KHz		0x0005
#define BUZZER_PWM_PULSE_3KHz			0x0002
#define BUZZER_PWM_PERIOD_4K7Hz		0x0003
#define BUZZER_PWM_PULSE_4K7Hz		0x0001
#define BUZZER_DURATION_TIME			20	// 200ms
#define BUZZER_ACTIVATE_AFTER_TIME	75	// 750ms

#define StartBuzzer_3KHz()	(B_BuzzerControlFlag = BuzzerState_3KHz_START)
#define StartBuzzer_4K7Hz()	(B_BuzzerControlFlag = BuzzerState_4K7Hz_START)
#endif	//BUZZER_USED

#ifdef DELL_U2410F
#define ZOOM_FACTOR	50		//Neil 20090303+ for zoom control
#define AUDIO_VOLUME	110		//Neil 20090504+ for volume control,reduce background noise after PC mute on , ORG = 120
//#define VERSION_PLUS			//Neil 20090505+ for show more information on version string
#define DDCCI_VALUE_CHANGE	//Neil 20090514+ for DDCCI detect value changed function
#define CROP_PERCENTAGE 	4 
#if(defined(BOARD_93XX_4K2K))
#define MainDisplayEnableDelay			100// 420	 Neil  140816 for PC DP Port AC ON issue	// 65	//it was 5 before
#define PxpDisplayEnableDelay			50	//it was 5 before
#else
#define MainDisplayEnableDelay			10	//it was 5 before
#define PxpDisplayEnableDelay			10	//it was 5 before
#endif

//#define AUDIO_NOISE_REDUCE			
//#define AUDIO_DIF_CHECK				1///1
//#define AUDIO_DP_M_CHECK			
//#define AUDIO_DP_USE_REGEN			//Neil120923+ for DP audio use regen function from DP12TX1_AUD_REGEN_CTRL
#endif

//120416 Edward disable due to ACR_Handler.h
#ifdef DELL_U2410F
#define DELL_HIGH_DCR //KevinL 20120606 enable DELL DCR feature
//#define DCR_READ_BRIGHTNESS_USE_ACC2		// Increased by ST_hansPENG 090424

#define DCR_NOISE_THRESHOLD					1	// 1: 0x37, 2: 0x6E, 3: 0xA6....

#define HIGH_DCR_ACTIVATION_TIME			10	// 10 seconds / 1s
#define HIGH_DCR_CONTINUOUS_LIMIT_TIME		60	// 1 minute / 1s
#define HIGH_DCR_PANEL_WARM_UP_TIME			600	// 10 minutes / 1s
#define HIGH_DCR_BlackScreen_TIME			1	//120924 check if real black screen to avoid turn off / on panel to often

typedef	enum HighDcrActivationProcedureEnum
{
	HighDCR_STRANDBY,		// 0
	HighDCR_ACTIVATION,	// 1
	HighDCR_CONTINUOUS,	// 2
	HighDCR_LIMIT,			// 3
} HighDcrActivationProcedureType;

#if (DEFAULT_PANEL == LG_WUXGA_LM240WU9_SLA1)
#define DCR_NORMAL_PWM_PULSE_MAX	WB_BRIGHTNESS_MAX
#define DCR_NORMAL_PWM_PULSE_MIN	21//12 // 120925 KimChye //1//WB_BRIGHTNESS_MIN //120703 Edward assume normal DCR range

#define DCR_HIGH_PWM_PULSE_MAX		DCR_NORMAL_PWM_PULSE_MAX
#define DCR_HIGH_PWM_PULSE_MIN		0//DCR_NORMAL_PWM_PULSE_MIN //120703 Edward assume 0 as High DCR min

#elif (DEFAULT_PANEL == LG_WQHD_LM270WQ1)
#define DCR_NORMAL_PWM_PULSE_MAX	MAX_BACKLIGHT_PWM
#if 0 //LGD_HBar
#define DCR_NORMAL_PWM_PULSE_MIN	MIN_BACKLIGHT_PWM + 5 // 120925 KimChye
#else
#define DCR_NORMAL_PWM_PULSE_MIN	MIN_BACKLIGHT_PWM + 30 // 120910 KimChye
#endif
#define DCR_HIGH_PWM_PULSE_MAX		DCR_NORMAL_PWM_PULSE_MAX
#if 0 //LGD_HBar
#define DCR_HIGH_PWM_PULSE_MIN		0x0B//arsenio 20120822 for LGD H Bar 0x05//Calo 20090312* Org = 0x05//0x0C
#else
#define DCR_HIGH_PWM_PULSE_MIN		0x0B//0x05//Calo 20090312* Org = 0x05//0x0C
#endif

#elif(DEFAULT_PANEL == LG_WHD_LM290WW1_SSA1)
#define DCR_NORMAL_PWM_PULSE_MAX	MAX_BACKLIGHT_PWM
#define DCR_NORMAL_PWM_PULSE_MIN	MIN_BACKLIGHT_PWM + 30// 121008 KimChye

#define DCR_HIGH_PWM_PULSE_MAX		DCR_NORMAL_PWM_PULSE_MAX
#define DCR_HIGH_PWM_PULSE_MIN		0x00	//0x05	//20121012 Kordonm modify for High DCR

#elif(DEFAULT_PANEL == LG_WQXGA_LM300WQ5_SLA1)
#define DCR_NORMAL_PWM_PULSE_MAX    MAX_BACKLIGHT_PWM
#define DCR_NORMAL_PWM_PULSE_MIN    MIN_BACKLIGHT_PWM

#define DCR_HIGH_PWM_PULSE_MAX      DCR_NORMAL_PWM_PULSE_MAX
#define DCR_HIGH_PWM_PULSE_MIN      DCR_NORMAL_PWM_PULSE_MIN // 0x00

#else  
#define DCR_NORMAL_PWM_PULSE_MAX	WB_BRIGHTNESS_MAX
#define DCR_NORMAL_PWM_PULSE_MIN	16 //121101 KimChye //0

#define DCR_HIGH_PWM_PULSE_MAX		DCR_NORMAL_PWM_PULSE_MAX
#define DCR_HIGH_PWM_PULSE_MIN		0
#endif

#define DCR_PWM_PULSE_MAX		((HighDcrActivationProcedure == HighDCR_CONTINUOUS)? (DCR_HIGH_PWM_PULSE_MAX): (DCR_NORMAL_PWM_PULSE_MAX))
#define DCR_PWM_PULSE_MIN		((HighDcrActivationProcedure == HighDCR_CONTINUOUS)? (DCR_HIGH_PWM_PULSE_MIN): (DCR_NORMAL_PWM_PULSE_MIN))
#define DCR_PWM_PULSE_RANGE		(DCR_PWM_PULSE_MAX - DCR_PWM_PULSE_MIN)

#if 0//def DCR_READ_BRIGHTNESS_USE_ACC2
#define IMP_BRIGHTNESS_MAX		241
#define IMP_BRIGHTNESS_MIN		32
#define IMP_BRIGHTNESS_RANGE	(IMP_BRIGHTNESS_MAX - IMP_BRIGHTNESS_MIN)
#else
#define IMP_BRIGHTNESS_MAX		255 //KevinL 20120606 updated brightness level to 0 ~ 255
#define IMP_BRIGHTNESS_MIN		0
#define IMP_BRIGHTNESS_RANGE	(IMP_BRIGHTNESS_MAX - IMP_BRIGHTNESS_MIN)
#endif
#define HIGH_DCR_GRAY_LEVEL_UNDER	(IMP_BRIGHTNESS_RANGE * 0x4 / 0xFF + IMP_BRIGHTNESS_MIN)	// Under 4/255

#define ConvertImpBrightnessToPwmPulse(W_Imp_Brightness) \
		((BYTE)((((DWORD)(W_Imp_Brightness - IMP_BRIGHTNESS_MIN) * DCR_PWM_PULSE_RANGE) / IMP_BRIGHTNESS_RANGE) + DCR_PWM_PULSE_MIN))
#endif

typedef enum		//Neil 20090110+ for keypad control
{
	Adj_None,
	Adj_Zoom,
	Adj_ImageScheme,
}CheckFunctionState;

//
// Mission flag to LPM
//
typedef enum		//Neil120618+
{
   Flag_AudioDetect           = BIT0,

} MissionFlag;


typedef enum
{
	VBID_VerticalBlank_Flag  = BIT0,
	VBID_FieldID_Flag  = BIT1,
	VBID_Interlaced_Flag  = BIT2,
	VBID_NoVideo_Flag  = BIT3,
	VBID_AudioMute_Flag  = BIT4,
	
	VBID_Reserved_Flag  = BIT5,
	//VBID_Reserved_Flag  = BIT6,
	//VBID_Reserved_Flag  = BIT7,
}VBIDStatus;

// used for SpeakerAudioSource and HeadhponeAudioSource
enum
{
	AUDIO_MAIN,
	AUDIO_PIP,
	NUM_AUDIO_SOURCES
};


//Neil120814+ for audio wait timer setting
#define Aud_ACDC_Time						50
#define Aud_ModeChange_Time					155		// ORG = 20;
#define Aud_Portchange_Time					40
#define Aud_ChannelChange_Time				30
#define Aud_SampleRateChange_Time			10// 15
#define Aud_default_Time					10
typedef enum
{
	Aud_Tmr_None = 0,
	Aud_Tmr_ACOn = BIT0,
	Aud_Tmr_DCOn = BIT1,
	Aud_Tmr_ModeChange = BIT2,
	Aud_Tmr_ChannelChange = BIT3,
	Aud_Tmr_InputChange = BIT4,
	Aud_Tmr_AudSampleRateChange = BIT5,
	Aud_Tmr_All = 0xff
}AudioTimerCtrl;

typedef enum
{
	SysCh_None = 0,
	SysCh_ACOn = BIT0,
	SysCh_DCOn = BIT1,
	SysCh_InputChange = BIT2,
	SysCh_ChannelChange = BIT3, //Channel swap
	SysCh_ModeChange_Normal = BIT4,
	SysCh_ModeChange_Quick = BIT5,
	SysCh_ModeChange_PD = BIT6,
	SysCh_All = 0xff
}SysChFlagCtrl;

#ifdef AUDIO_DIF_CHECK
enum
{
	AUD_DIF_INIT,
	AUD_DIF_ReCheck,		
	AUD_DIF_UP,
	AUD_DIF_DOWN,
	AUD_DIF_FINISH,
	AUD_DIF_Stop,
};//AUD_DIF_STATE;
#endif

typedef enum Video1ChannelSelectEnum
{
	DEC_1Ch_Sel_D1 ,
	DEC_1Ch_Sel_D2,
	DEC_1Ch_Sel_D3,
	DEC_1Ch_Sel_D4 ,
	DEC_1Ch_Sel_All,
}Video1ChannelSelectType;


typedef enum DisplayPosition
{
	Disp_Full,
	Disp_Left,
	Disp_Right
}DisplayPositionType;

typedef enum MainModeEnum
{
	Main_FULL_SCREEN,		// 0
	Main_LARGE,				// 1
	Main_SMALL,				// 2
	Main_SIDE_BY_SIDE,		// 3
	Main_SIDE_BY_SIDE_TALL,	// 4
	Main_Dummy1,			//0x5
	Main_MEDIUM 		= 6,
	Main_HUGE,			
	#if((PROJECT_SELECT == PROJ_SUV)||(IS_PROJ_PD()))
	Main_SIDE_BY_SIDE_PUP,		//8 P upon p
	Main_QUARTER_PXP,			//9
    #endif
	Main_Dummy2,			//0x0A
    Main_FLEXIBLE = 11,				//B
	NUM_Main_MODES
} MainModes;

typedef enum PD_BlannkEnum
{
 	Blank_None,			
 	Blank_Input,			
 	//Blank_Output,			
}PD_BlankType;


#ifdef USER_IR_SUPPORT
typedef struct USER_IRStruct
{
	BYTE WBKey;
	BYTE IR_CMD;
	BYTE IR_Value;
	BYTE IR_Index;
}USER_IRType;
#endif

#ifdef DELL_U2410F        //calo update for the information of color space in factory menu
enum
{
	RGBColorSpace = 0,
	YUV_422,
	YUV_444,
	UnKnown
};
#endif

//120425 Edward add for energy smart detect change to state machine
typedef enum
{
   PixelGrap_START=0,
   PixelGrap_CONFIG,
   PixelGrap_WAIT, 
   PixelGrap_CALCULATE,
   PixelGrap_FINISHED,
}PixelGrapState;

//100210 Edward add for the OSD display
typedef enum
{
	Ptr_Date,
	Ptr_FWVersion,	
    Ptr_PanelName,
	Ptr_ModelName,
	Ptr_ModelName2,
	Ptr_SerialNo,
	Ptr_BoardID,
} OSD_DynamicDisplayType;



typedef enum
{
	gmd_DPRX_HPD_PULSE_SHORT ,//= 1000,
	gmd_DPRX_HPD_PULSE_LONG // = 3000
} gmt_DPRX_HPD_PULSE_DURATION;

typedef enum
{
	BR_9600,
	BR_19200,
	BR_38400,
	BR_57600,
	BR_115200,
	BR_Max
}BaudRateType;

enum
{
	BID_None,
	BID_Verify,
	BID_Located,
	BID_End,
};

enum
{
	LED_IDLE,
	LED_BUSY,
};


#define X_Block 			4//8// 4 blocks sampling 2 pixels
#define Y_Block 			4//8
#define Total_Block		(X_Block*Y_Block)
#define Block_OFFSET_X	(stInput[CH_A].Hactive/X_Block)
#define Block_OFFSET_Y	(stInput[CH_A].Vactive/Y_Block)

static WORD X_Position=0, Y_Position=0;//= X_STEP/2, Y_Position = Y_STEP/2;
static DWORD DW_StoreR, DW_StoreG, DW_StoreB;
static BYTE SAMPLE_Index;
static BYTE B_YLevel=0;
static PixelGrapState PixelGrap_State=PixelGrap_START;


#ifdef DELL_U2410F
extern BOOL gmvb_OsdReadyForPowerDown;
extern BYTE B_BackupPresetMode;
extern BYTE OriginalInput; //KevinL 20090413
#ifdef VGA_GAME_MODE_PATCH //KevinL 20090418
extern BOOL B_DisplayFrameLock;//Michael
#endif
#endif
#define DEFAULT_COLOR_STATE		0		// DStaticText
#define SELECT_COLOR_STATE		1		// DStaticText

BOOL PxPSourceIsDVIHDMI (void);
BOOL PxPSourceIsDPMiniDP(void);

BYTE PxPSourceIsSVideoComposite (void);

BYTE IsNormalMode(void);
BYTE IsBlueMode(void);
BYTE IsRedMode(void);
BYTE IsMacMode(void);
BYTE IsPCMode(void);
BYTE IsCustomMode(void);
BYTE IsMainVideoInput(void);
BYTE IsMainGraphicsMode(void);
BYTE IsMainTMDSMode(void);
BOOL IsMain1920x1200(void);
BOOL IsMain1680x1050(void);
BOOL IsMain2560x1600(void);
BYTE IsLockOSD(void);
BYTE IsOSDHardLock(void);
BYTE IsPipGraphicsMode(void);
BYTE IsPipTMDSMode(void);
BYTE IsPipVideoMode(void);
BYTE IsLcdOn(void);

BOOL IsCableConnected_Pip (void);
BOOL IsCableConnected_Main (void);
BOOL IsCableConnected_Audio (void);
BOOL IsOSDFlying(void);
BOOL IsSplashScreenCompleted(void);

BOOL IsOSD_DCR_Or_NR_On(void);
BOOL IsMenuLandscape(void);
BYTE IsVGA(void);
BYTE IsDVI(void);
BOOL IsAnalogInput(gmt_PHY_CH B_Channel);
BOOL IsDigitalInput(gmt_PHY_CH B_Channel);
BOOL IsDPInput(gmt_PHY_CH B_Channel);

BOOL IsDIPInput(gmt_PHY_CH B_Channel);

BOOL IsMainVGAInput(void);
BOOL IsPIPVGAInput(void);
BOOL IsMainDVID1Input(void);
BOOL IsPIPDVID1Input(void);
BOOL IsMainDVID2Input(void);
BOOL IsPIPDVID2Input(void);
BOOL IsMainHDMIInput(void);
BOOL IsPIPHDMIInput(void);
BOOL IsMainDP1Input(void);
BOOL IsPIPDP1Input(void);
BOOL IsMainDP3Input(void);
BOOL IsPIPDP3Input(void);
BOOL IsMainCOMPInput(void);
BOOL IsPIPCOMPInput(void);
BOOL IsMainCOMP1Input(void);
BOOL IsPIPCOMP1Input(void);
BOOL IsMainCOMP2Input(void);
BOOL IsPIPCOMP2Input(void);
BOOL IsMainCVBSInput(void);
BOOL IsPIPCVBSInput(void);
BOOL IsMainCVBS1Input(void);
BOOL IsPIPCVBS1Input(void);
BOOL IsMainCVBS2Input(void);
BOOL IsPIPCVBS2Input(void);
BOOL IsMainDIPInput(void);
BOOL IsPIPDIPInput(void);
BOOL IsMainDIPInput_D1(void);
BOOL IsMainDIPInput_D2(void);
BOOL IsMainDIPInput_D3(void);
BOOL IsMainDIPInput_D4(void);
BOOL IsPxpDIPInput_D1(void);
BOOL IsPxpDIPInput_D2(void);
BOOL IsPxpDIPInput_D3(void);
BOOL IsPxpDIPInput_D4(void);
BOOL IsMainDigitalInput(void);

BOOL IsDPVideoMode(gmt_PHY_CH B_Channel);
BOOL IsDPColorSpaceRGB(void);
BOOL IsHDMIColorSpaceRGB(gmt_PHY_CH B_Channel);
BYTE InputConvertScheme(BYTE);
BYTE ConvertPresetMode(BYTE ImageScheme);//120417 Edward
void Slave_SetMiscControl (void);

void DrawVEKBitmapBaseSlider();
void PIPSendMessage(BYTE MessageId);
void OSDSendMessage(BYTE MessageId);
void SaveFactorySettingsInNVR (void);
extern void StorePresetModes(void);
extern void RestorePresetModes(void);
void SetOSDMenuState ();
void HideMain ();
void SavePortDependentSettings ();
void ClrForceBackground ();
void TurnLcdOn ();
void RestorePipOnscreenState ();
void ClrBitFlag ();
void SetWbMainChannel ();
void Osd_Hide ();
void SetForceBackground();
void DrawSelectedLongValueHexWB ();
void SavePipOnScreenState ();
void AdjustColorMode(void);
void AdjustOsdBlend ();
void AutoConfig();
void DrawLongValueHexWB();
void SaveModeDep_gm57xx();
void UpdateCustomRGB();
void AdjustOsdBlend(void);
void PipBorderControl(void);
void SetSplashScreenTimer ();
void UpdateSourceLeds() ;
void AdjustCustomSplashScreen ();
void LockOSD();
void OSDHardLock();
void OSDHardUnLock();
void RestoreLedState();
void TurnLcdOff ();
void TurnSoundOff();
void TurnSoundOn ();
void UnLockOSD();
void AdjustAudioPreset ();
void ChangeOSDRotation ();
void ClearUserArea ();
void DrawDynamicText1();
void FAdjustWriteReg();
void FUpdateGm5766AdcSettings ();
void ResetAudioSetting();
void SaveUserTimeInNVR();
void SetMessageHVPos();
void StartFlyingWindow();
void StartSourceScan();
void StopFlyingWindow (void);
void UpdateColorTemp ();
void FlyingWindowHandler ();
#if(FEATURE_SOURCE_SCAN == ENABLE)
void SourceScanHandler ();
#endif
void LED_Handler ();
void HardLockTimerHandler();
void SplashScreenTimerHandler();
void DiagnosticTestPatternTimerHandler();
void AudioRequestHandler();
void MonitorOnTimeHandler();
void NoneKeyHandler();
void CustomHandler();
void AdjustBackLightBase(void);
#if ((DEFAULT_PANEL == LG_WHD_LM290WW1_SSA1)||(DEFAULT_PANEL == LG_WQXGA_LM300WQ5_SLA1))
void UsbDetectControl(void);
void AdjustBackLightBase(void);
#endif

void AdjustAngleInterpolProp ();
void MaxBlackLevel(); 
void AdjustAudioConfig();
void AdjustAudioInput();
void AdjustBackLight();
void AdjustBlackLevel();
void AdjustBlue();
void MinBlackLevel();
void BallisticAccelerationForUserBlue();
void BallisticAccelerationForUserGreen();
void AdjustColorFormat(void);
void BallisticAccelerationForUserRed();

void AdjustFilmModeProp ();
void AdjustForceBackGroundColor();
void AdjustGrBlacklevel ();
void AdjustGreen();
void AdjustMadiProp();
void AdjustOsdZoom();
void AdjustPIPMessageOnOff();
void AdjustPanelDynamicContrast();
void AdjustPanelResponseTime ();
void AdjustPipBlendProp ();
void AdjustPipHPosProp ();
void AdjustPipSizeProp();
void AdjustPipVPosProp();
void AdjustPipValue();
void AdjustRed () ;
void AdjustTnrProp();
void AdjustUSBLink();
void AdjustZoom(void);
void FAdjustAmplitudeValue ();
void FAdjustAmplitudeValue2();
#if 1//120406 Edward replace all in the factory adjust
void FAdjustColorTempCommon();
#else
void FAdjustNormalPresetCommon ();
void FAdjustBluePresetCommon();
void FAdjustRedPresetCommon();
#endif
void FAdjustGain_B();
void FAdjustGain_G();
void FAdjustGain_R();
void FAdjustIspMode();
void FAdjustNormalPresetGamma18Common();
void FAdjustOffset1_B();
void FAdjustOffset1_G();
void FAdjustOffset1_R();
void FAdjustOffset2_B();
void FAdjustOffset2_G();
void FAdjustOffset2_R();
void FAdjustPeriodValue ();
void FAdjustPeriodValue2 ();
void FAdjustReadReg ();
void FAdjustRegisterNumber();
void FAdjustSpreadEnable();
void FAdjustTimeUsedHour();
void FAdjustTimeUsedMin();
void FAdjustUserPresetCommon();
void PanelPowerOffSequence(void);
void PanelPowerOnSequence (void);
//--- delete it //void LVDSChannelCBus0Patch(BOOL LVDSOnOff);	//20120921 Kordonm+

BYTE IsMacMode(void);
BYTE IsPCMode(void);
BYTE IsMainVideoInput(void);
BOOL IsMain1920x1200(void);
BOOL IsMain1680x1050(void);
BOOL IsMain2560x1440(void);//owen 20120614
BOOL IsMain2560x1080(void);//owen 20120614
BYTE IsVGA(void);
BYTE IsDVI(void);

BOOL IsInputColorFormatDisable (void);
BOOL IsGammaDisable (void);
BOOL IsDisplayPortBoard(void);
BOOL IsModeSelectDisable (void);
BOOL IsWideMode_1_1_Disable(void); //KevinL 20090115
BOOL IsHardLockTimerRunning(void);

void DrawDynamicText(void);
void DrawDynamicText1(void);

BYTE PxPSourceIsGraphic (void);
BYTE PxPSourceIsVideo (void);
BYTE PxPSourceIsDVI (void);
BYTE PxPSourceIsHDMI (void);
BYTE PxPSourceIsSVideo (void);
BYTE PxPSourceIsComposite (void);
BOOL AudioDetect (void);

void SourceLedControl (void);
 BOOL IsKeyPress(void);

 void BurnInHandler(void);

void	DebugOSD(BYTE state);

SBYTE ConvertACM3RGBCMY(SBYTE B_ACM3_6Axis); //KevinL 20090331 OSD range 50~-50 to range 127 ~-127
SBYTE ConvertBrightness(SBYTE B_Brightness);  
WORD ConvertContrast(WORD W_Contrast, gmt_PHY_CH Channel);
BYTE ConvertSharpnessVWD(BYTE B_SharpnessVWD); //20120618 Add by KevinChen OSD range 100~0 to range 255 ~0
WORD ConvertSaturation(WORD W_Saturation); //KevinL 20081211 remapping OSD range 100~0 to range 512 ~0;
SWORD ConvertHue(SWORD W_Hue); //KevinL 20081211 remapping OSD range 100~0 to range 180 ~ -180
WORD ConvertRGBGain(WORD W_RGBGain); //KevinL 20081212 remapping OSD range 100~0 to range 256 ~0
SBYTE ConvertRGBOffset(SBYTE B_RGBOffset); //KevinL 20081216 remapping OSD range 100~0 to range 100 ~ -100
BYTE ConvertVGAPhase(BYTE HSyncPhase); //KevinL 20090116 convert VGA phase from OSD 0~100 to setting 0~63
BYTE InvertVGAPhase(BYTE HSyncPhase); //KevinL 20090116 convert VGA phase from setting 0~63 to 0~100 OSD
BOOL IsSDVideo(gmt_PHY_CH B_Channel);//KevinL 20100309 for M1F192A
void AdjustBackLight(void); 
void ResetDisplaySettings (void);
void ResetColorSettings(void);
void ResetACM6AxisSettings(void); //KevinL 20081203

extern void InitPresetModesDescr(void);
extern void InitPresetModesDependent(void);
extern BOOL ReadPresetModesDependentEntry(BYTE B_Index);
extern BOOL CheckPresetModesDependentEntry(BYTE B_Index);
extern BOOL CheckPresetModesDependentDescriptor(void);
extern void SavePresetModesDependentSettings(void);
void ReadPipPortSchemeDependent(void);
void SavePipPortSchemeDependent(void)	;
void PipImageSchemeReset(void);

#ifdef DELL_U2410F
void AllocateTile(void);  
void SetRotatedTileSizePosition(ST_OSD_TILE *Stp_Tile);
void SetTileOffset(ST_OSD_TILE *Sp_Tile, gmt_OSD_DIALOG_CTRL ROM *Sp_DialogCtrl);
static SDWORD CalculateTileLocation(WORD W_PosRef, SDWORD SDW_Min, SDWORD SDW_Max);
void CustomRotateAdjust(void);
extern void ReadPresetModesSettings(void);
extern void SavePresetModesSettings(void);
void StartNOFlyingWindow(void);
void StopNOFlyingWindow(void);
void StoreACC2Mask(void); //KevinL 20090417 for PBP OSD noise
void RetoreACC2Mask(void);
void SetRotatedIconXOffset(void); //KevinL 20090417 Save rotated Main Menu X Offset
void RestoreRotatedXOffsetAfterIconDraw(void); //KevinL 20090417 Update reoated X offset for Main Menu
void SetPIPIconXYOffset(void); //KevinL 20090417 Save Main Menu XY Offset
void RestoreXYOffsetAfterPIPIconDraw(void); //KevinL 20090417 Update XY offset for Main Menu
void InitPIPIconPosition(void); //KevinL 20090120
void InitPIPMessagePosition(void); 
void BackupPresetMode(void);
void SetColorFormat(void); 
void ResetBurninTime(void); 
void ResetBID(void);
BOOL IsPIPSetupCompleted(void); //KevinL 20090414
void EnableGameMode(void); //KevinL 20090415
void DisableGameMode(void);//KevinL 20090415
#ifdef VGA_GAME_MODE_PATCH //KevinL 20090420
BOOL IsVGADisplayFrameLock(void);
BOOL IFMVtotalCheck(void); //KevinL 20090508
#endif
BOOL IsGameModeEnable(void); 
//BOOL IsSubCarrier443(void); //KevinL 20090413 for NTSC443
//BOOL IsCVBS_SECAM(void); //KevinL 20090418 for CVBS SECAM 
void InputOverScanCrop(gmt_PHY_CH B_Channel);
BOOL DPOverScan(gmt_PHY_CH B_Channel);
BOOL HDMIOverScan(gmt_PHY_CH B_Channel);
BOOL DVIOverScan(gmt_PHY_CH B_Channel);//calo update for overscan of PIP 090309	
BOOL VGAOverScan(gmt_PHY_CH B_Channel)	;

BOOL OSDDrawVideoTimingInfo(gmt_PHY_CH B_Channel);
BOOL IsDVI_HDCP_ON(gmt_PHY_CH B_Channel); 
BOOL IsDVIInput(gmt_PHY_CH B_Channel); //KevinL 20090422
BYTE ReadDPBandWidth(void); 
BYTE ReadDPLaneCount(void); 
void DrawDTimingInfo (void);
OSDMainMessageType MainMessageTypeReturn(void);
void AdjustINLFactoryInfo(void);
#endif

#ifdef USB_POWER_CONTROL
extern void UsbPowerHandler(void);
#endif
extern void CustomPanelOnOffControl(BOOL BL_PanelOnOff);

extern void NullFunction(void);



#ifdef DELL_U2410F//For pass compiler  on RD1
BOOL IsVGACableConnected(void);
void TurnLEDOff(void); 
BOOL IsDVI1CableConnected(void);
BOOL IsDVI2CableConnected(void);
#endif

#ifdef DELL_U2410F//#ifdef INNOLUX_MARS_BOARD 
void FactoryModeAdmitHandler(void); 
void PipMessageAdmitHandler(void);
#ifdef CHECK_PxP_STATES_AC_ON
void PowerOnPIPModeHandler(void);
#endif
#ifdef PIP_POS_OSD_ACC_PATCH //KevinL 20090513		
void AccUpdatedCheckHandler(void); 
#endif
void PIPMessageHandler(void);
void CableDetectionHandler(void);
void CableDetectionInit(void); 
BOOL IsVGACableConnected(void);
BOOL IsHDMICableConnected(void);
BOOL IsDVI1CableConnected(void);
BOOL IsDVI2CableConnected(void);
BOOL IsDPCableConnected(void);
BOOL IsDP_PWRTX_Connected(void);
void RotationSensorHandler(void);	
void SwitchDviInput(BYTE input);
//Lvjie 081013{moved from 2709 code
BOOL IsOSDMainMenu(void);
BOOL IsOSDSideMenu(void);
BOOL IsThreeKeyState(void);
BOOL IsBurnInState(void);
BOOL IsDiagnosticState(void);
BOOL IsFactoryKeyState(void);

//Lvjie 081013}
#ifdef INPUTSCAN
BOOL IsInputScanCompleted(void); //KevinL 20090407
BOOL IsInputOriInputVGA(void);
BOOL IsInputOriInputDVI1(void);
BOOL IsInputOriInputDVI2(void);
void InputScan(void);		
void StartInputScan(void);
#endif
#endif
#ifdef USE_HPD_Toggle
void HotPlug_Toggle(gmt_UserDefConnNames Connection);	//Neil 20081117+ for HPD 
#endif
#ifdef DCR_READ_BRIGHTNESS_USE_ACC2
static WORD gm_Acc2GetBrightness(void);
#endif
void DCR_Test_Handler(void); //KevinL 20090115 for DELL DCR test		
void HighDCR_QuitCheck(void);//120704 Edward test
static WORD ReadInputAverageBrigttness(void);
void gm_ForceModeChange(gmt_PHY_CH Channel);
BYTE IsNativeModeInput(gmt_PHY_CH Channel);
//void SetDecoderFreeRun(gmt_PHY_CH Channel);
gmt_MODE_TYPE gm_GetCurrentInputModeType_New(gmt_PHY_CH B_Channel);

void ResetIFMWatchDog(gmt_PHY_CH B_Channel);	//Neil 20090107+ for QD882 show OOR after change mode or DC off/on issue;
//void SetFunctionRunningFlag(CheckFunctionState Adj_Flag,CheckKeyState KeyPressed);

void SetOSDPosition(void);		//Neil 20090206+
void AudioControl(gmt_PHY_CH B_Channel);		//Neil 20090212+

//&*&*&*S 20120315:SWRD4001: Kordonm(0)  Modify for OSD Workbench
void AdjustPrinterPrevue(void);
//&*&*&*E 20120315 : Kordonm(1)  Modify for OSD Workbench

void AdjustPresetModes(void);
void PowerDownAudioSpeaker(void);
void AudioSpeakerPowerCheck(void);		//for wake up from power saving audio pop noise
#ifdef AUDIO_NOISE_REDUCE
void AudioSourceSelect(void);
void AudioSourceAutoSelect(void);
#endif
WORD GetDPHSartFromMS(void);
WORD GetDPVSartFromMS(void);
BOOL IsPowerInDeepSleep(void); //Kevinl 20090505
BOOL IsPreLutEnable(void); //KevinL 20090304
BOOL IsPostLutEnable(void); //KevinL 20090304

BOOL IsHDMIInfoFrameExist(void);  //calo updat for the factory menu 090404
BYTE ColorSpaceJudgement(void);  //calo updat for the factory menu 090404
BOOL IsHDMIAudioPacketExist(void); //calo add for factory request
BOOL IsDPAudioExist(void);
void SetAudioDACOutput(BYTE OnOrOff);	//Neil 20090312+
void CheckACCUpdateStatusWhenOSDShow(void);		//Neil 20090316+ another way to avoid osd broken when press up or down key
void AdjustEnhancer(gmt_PHY_CH B_Channel);		//Neil 20090317+
BOOL CheckUserPrefSettingRange(void);
void CVBSForceModeSetupHandler(void); //KevinL 20090415 for CVBS NTSC-M <->NTSC443 need modesetup for correct setting
BYTE IsInputSignalPresent(gmt_PHY_CH B_Channel,ALL_BOARDS_INPUT_PORT B_CurrentPort);
BYTE IsAudioPacketPresent(ALL_BOARDS_INPUT_PORT B_CurrentPort);
BYTE IsAudioOnCurrentPort(ALL_BOARDS_INPUT_PORT B_CurrentPort);		//Neil 20090507+
BYTE CheckIBDThreshold(gmt_PHY_CH B_Channel);
void FactoryModeDisableRotation(void);  //calo add for enter into factory menu disable rotation function 090618
void UpdateOSDMenuRotationPosition(void); //Use in MenuRotationSInvState of WB
//&*&*&*S 20120315:SWRD4001: Kordonm(0)  Modify for OSD Workbench
void AdjustColorUniformity(void);
//&*&*&*E 20120315 : Kordonm(1)  Modify for OSD Workbench
void ToggleMenuRotation(void);
#ifdef RESET_AUDIO_MPE
void NoSounddetect(void);
#endif
#ifdef ADJUST_FIFO_ENABLE
void AdjustFIFO(void);
#endif
BYTE RGBSharpnessRemapping(void); //121010 Edward for DELL 2913
BYTE YUVSharpnessRemapping(void);

void DrawHorPosition1MoveableBitmap(void);
void DrawHorPosition2MoveableBitmap(void);
void DrawHorPosition3MoveableBitmap(void);
void DrawHorPosition4MoveableBitmap(void);
void DrawHorPosition5MoveableBitmap(void);
void DrawHorPosition6MoveableBitmap(void);
void DrawHorPosition7MoveableBitmap(void);
void DrawHorPosition8MoveableBitmap(void);
void DrawMoveableBitmap(BYTE HorPositionIndex, BYTE B_ColorState);
void DrawMoveableBitmapControl(BYTE HorPositionIndex, BYTE B_ColorState);

void EraseHorPosition1MoveableBitmap(void);
void EraseHorPosition2MoveableBitmap(void);
void EraseHorPosition3MoveableBitmap(void);
void EraseHorPosition4MoveableBitmap(void);
void EraseHorPosition5MoveableBitmap(void);
void EraseHorPosition6MoveableBitmap(void);
void EraseHorPosition7MoveableBitmap(void);
void EraseHorPosition8MoveableBitmap(void);
void EraseMoveableBitmap(BYTE HorPositionIndex);

void DrawSelectedHorPosition1MoveableBitmap(void);
void DrawSelectedHorPosition2MoveableBitmap(void);
void DrawSelectedHorPosition3MoveableBitmap(void);
void DrawSelectedHorPosition4MoveableBitmap(void);
void DrawSelectedHorPosition5MoveableBitmap(void);
void DrawSelectedHorPosition6MoveableBitmap(void);
void DrawSelectedHorPosition7MoveableBitmap(void);
void DrawSelectedHorPosition8MoveableBitmap(void);
void DrawSelectedMoveableBitmap(BYTE HorPositionIndex, BYTE B_ColorState);

void DrawDisabledHorPosition1MoveableBitmap(void);
void DrawDisabledHorPosition2MoveableBitmap(void); 
void DrawDisabledHorPosition3MoveableBitmap(void);
void DrawDisabledHorPosition4MoveableBitmap(void);
void DrawDisabledHorPosition5MoveableBitmap(void);
void DrawDisabledHorPosition6MoveableBitmap(void);
void DrawDisabledHorPosition7MoveableBitmap(void);
void DrawDisabledHorPosition8MoveableBitmap(void);
void DrawDisabledMoveableBitmap(BYTE HorPositionIndex, BYTE B_ColorState);

void NoCablePowerDownHandler(void);
void StartNoCablePowerDownCounter(void);
void StopNoCablePowerDownCounter(void);
void EnergyUsedLevelHandler(void);
void EnergySmartHandler(void);
void AdjustEnergySmart(void);
void ReadFactorySettings(void);

#if 1 //DELL_U2410F //120209 Edward add from DELL2410
//zuguang 20090220
gmt_RET_STAT    gm_UpdateAcc_DELL(BYTE ColorScheme);//120717 Edward
//gmt_RET_STAT    gm_UpdateAcc_Graphic(gmt_IMAGE_SCHEME_MODE_GRAPHIC ColorScheme);
gmt_RET_STAT    gm_UpdateAcc_Video(BYTE ColorScheme);
#endif


void Color_Uniformity_Test(BOOL On_Off);//110831 Edward test code

#if (defined( CMI_73XX_BOARD))
BOOL IsUSBConnected(void);	
void USB_Enable(void);	
void PSV_CTRL(void);
#endif

#if(BOARD_USED == DELL_93xx_BOARD)
void AudioPowerCtrlHandler(void);
#endif
void ConvertDynamicString(char *c_DString);
void SetDynamicStringPtr(OSD_DynamicDisplayType Ptr_Type);//120517 Edward
BOOL IsTimingOutOfRange(gmt_PHY_CH B_Channel);//Neil 20120522+ Copy from U2410
void ColorSpaceHandler(gmt_PHY_CH B_Channel);		//Neil 20120531+ for color space change 
BOOL HdmiCropInput(gmt_PHY_CH B_Channel);
void ResetDPRXInit(void);
void DPRX_LinkCheck(gmt_PHY_CH B_Channel);
void SendMissionFlagToLPM(void);
void OSDRequestMsg_Pxp(gmt_PHY_CH B_Channel,WORD W_Param, DWORD DW_Time);
BYTE GetDPVBIDStatus(void);
void AdjustBurnInMode(void);
void BurninOffDisablrAddress0x7C(void);//20120726 Add by KevinChen
BOOL far CheckDP_Mvid_ForSyncChange(BYTE InputConnector);
	void DisplayEnable(void);
#if (FEATURE_CEC == DISABLE)
BOOL IsHDMIInput(gmt_UserDefConnNames Port);		//Neil120803+ 
#endif

 void far DVI_Reset(BYTE InputConnector);		//for 480i, 576i issue
void far SetDpRxHpdPulse(gmt_UserDefConnNames B_InputConnector,gmt_DPRX_HPD_PULSE_DURATION W_DurationUs);
void far InitPanelSpectrum(void);		//Neil120808+ for enable and adjust spectrun from panel array

BOOL far CheckDVIInputMode(gmt_PHY_CH B_Channel);
WORD CheckVideoTimingResolution(gmt_PHY_CH B_Channel, WORD W_VActive);


#ifdef Gamma_DUM_Initial_Check
void GAMMA_DUM_NVRAM_CHECK(BYTE Check);
void Load_GAMMA_From_Table_To_NVRAM(BOOL B_Selection);
void Load_DUM_From_Table_To_NVRAM(void);
#endif
BOOL CheckSumResultAtFactoryMenu(BYTE B_Selection);
BYTE GetAudioTimerValue(void);
#ifdef AUDIO_DIF_CHECK
void SetAUD_BUF_DELAY(BYTE B_Delay);
BYTE ReadAUD_BUF_DELAY(gmt_UserDefConnNames B_InputConnector);
WORD ReadAUD_BUFPTR_DIF(gmt_UserDefConnNames B_InputConnector);
void AudioDIFCheck(void);
BYTE AUD_Delay_Min(gmt_UserDefConnNames B_InputConnector);
BYTE AUD_Delay_Max(gmt_UserDefConnNames B_InputConnector);
BYTE AUD_DIF_THOLD(gmt_UserDefConnNames B_InputConnector);
#endif
#ifdef AUDIO_DP_M_CHECK
BOOL far CheckDP_Maud_ForAudioStable(void);
#endif
BOOL Is_HDMIInput(gmt_PHY_CH B_Channel);
gmt_AUDIO_INPORT_TYPE GetAudioSource(gmt_PHY_CH B_Channel);
void RestartAudioRoutine(void);
ALL_BOARDS_INPUT_PORT GetCurrentAudioPort(void);
BOOL IsCurChannelDisplayOFF(gmt_PHY_CH B_Channel);
void ResetDP_SDP_Data_Path(void);
void Reset_DP_Audio(void); //JC_SZ_0924
BOOL VWD_IsAudioReady(gmt_PHY_CH B_Channel);
BOOL  GetHDMIAudioPCMWarningMessage(void);//owen 120919 for test
void ToggleAudioDACAttn(void);
void Audio_RedoDACGain(void);//Neil120924+
#ifndef AUDIO_DP_USE_REGEN
void AUDIO_DP_ClearRegen(void);
#endif
#ifdef AUDIO_DP_USE_REGEN
void AUDIO_DP_SetRegen(void);
void Audio_ResetRegen(void);
#endif
void Audio_ResetAudioOutput(void);
void Audio_AdjustVolume(BYTE B_AdjustValue);
void OSD_AdjustBrigntnessContrast(void);//owen 120928
void DDC_CI_CapabilitiesString(void);
void RestorePresetModes_OSD(void);//owen 20121002
#if (!defined(BOARD_93XX_PD)&&(!defined(BOARD_93XX_OV)))
WORD MaxMainDisplayNumber(void);
WORD MaxPxpDisplayNumber(void);
void PublicDisplaySetup(gmt_PHY_CH B_Channel, BOOL PD_Enable);
void AdjustPD_AutoBlank(void);
void AdjustPublicDisplayCtrl(void);
void AdjustPD_XNumber(void);
BYTE GetXNumber(void);
void AdjustPD_YNumber(void);
BYTE GetYNumber(void);
BYTE GetMaxBoardID(void);
BYTE GetBoardID(void);
void AdjustPD_DisplayNumber(void);
BYTE GetPDNumber(void);
BYTE GetPDChannel(void);
BYTE GetAutoBlankCtrl(void);
void Adj_Free_Zoom(BYTE part,WORD W_Percent);
void Adj_Pixcel_Zoom(BYTE X_Number,BYTE Y_Number,BYTE DisplayNumber,BYTE Pixcel_NUMBER);
#endif	//defined(BOARD_93XX_PD)


void AdjustOSDBlending(void);
void AdjustOSDBlending1(void);
void AdjustOSDBlending2(void);


#ifdef CONTROLTYPE_D2STATICTEXT_USED
void DrawOption1D2StaticText(void);
void DrawOption2D2StaticText(void);

#endif



#ifdef CONTROLTYPE_D2STATICTEXTBUTTON_USED
void DrawOption1D2StaticTextButton(void);
void DrawOption2D2StaticTextButton(void);
void DrawSelectedOption1D2StaticTextButton(void);
void DrawSelectedOption2D2StaticTextButton(void);

#endif
#ifdef CONTROLTYPE_D3STATICTEXTBUTTON_USED
void DrawOption1D3StaticTextButton(void);
void DrawOption2D3StaticTextButton(void);
void DrawOption3D3StaticTextButton(void);
void DrawSelectedOption1D3StaticTextButton(void);
void DrawSelectedOption2D3StaticTextButton(void);
void DrawSelectedOption3D3StaticTextButton(void);
#endif
#ifdef CONTROLTYPE_D4STATICTEXTBUTTON_USED
void DrawOption1D4StaticTextButton(void);
void DrawOption2D4StaticTextButton(void);
void DrawOption3D4StaticTextButton(void);
void DrawOption4D4StaticTextButton(void);
void DrawSelectedOption1D4StaticTextButton(void);
void DrawSelectedOption2D4StaticTextButton(void);
void DrawSelectedOption3D4StaticTextButton(void);
void DrawSelectedOption4D4StaticTextButton(void);
#endif

void AdjustCurrentMainInputDIP(void);
void AdjustCurrentPxpInputDIP(void);

void ReInitHDMIVDDS(gmt_PHY_CH B_Channel);
void CheckSystemMessageState(gmt_PHY_CH B_Channel);
#ifdef USER_IR_SUPPORT
BYTE IR_SearchKey( WORD W_Cmd, WORD W_Data);
#endif
void IR_EnterFactory(BYTE IR_Value);
BOOL Keypad_SpecialKey(BYTE KeyCh, BYTE KeyVal, BYTE far * SpecialKey, BYTE SK_Len);
void Keypad_EnterFactory(BYTE KeyCh, BYTE KeyVal);
void Keypad_RestorePanel(BYTE KeyCh, BYTE KeyVal);
void AdjustMainPosition(void);
void Adj_Free_Zoom(BYTE part,WORD W_Percent);
void AdjustPxpBlendLevel(void);

void Adj_Pixcel_Zoom(BYTE X_Number,BYTE Y_Number,BYTE DisplayNumber,BYTE Pixcel_NUMBER);

BYTE GetProjIDInfo(void);
BYTE Random_BYTE(BYTE B_Max);
WORD Random_WORD(WORD B_Max);
void AdjustPublicDisplayCtrl(void);
BOOL IsFactoryMode(void);
void AdjustOsdZoom(void) ;
void EraseUnicodeRangeText(void);
void SetBackGroundColor(FORCE_BACKGROUND Color);
void ClearBackGroundColor(void);

BYTE GetNextPxpMode(void);
BYTE GetPrevPxpMode(void);
BYTE GetNextPxpPostion(void);
BYTE GetPrevPxpPosition(void);
void SetPxpModePosition(void);
BYTE GetNextPresetMode(void);
BYTE GetPrevPresetMode(void);
BYTE GetNextGammaTable(void);
BYTE GetPrevGammaTable(void);
WORD GetPanelHeight(void);

void ModeSetupDisplayScaling(gmt_PHY_CH B_Channel, BYTE B_Step);
void AdjustBKG_Lamp(void);
void EnterLampState(void);
void ExitLampState(void);
gmt_DipPins GetDIPPins(gmt_UserDefConnNames B_InputConnector);
void CheckVideoParaForActive(gmt_PHY_CH B_Channel);

#if 1//def Action_AdjustSpeakers_WB_Used
void AdjustSpeakers(void);
#endif

#if 1//def Action_AdjustSpeakerAudioSource_WB_Used 
void AdjustSpeakerAudioSource(void);
#endif
#if 1//def Action_AdjustHeadphones_WB_Used
void AdjustHeadphones(void);
#endif
#if 1//def Action_AdjustHeadphoneAudioSource_WB_Used
void AdjustHeadphoneAudioSource(void);
#endif
void ToggleFreeze(void);
void AdjustUartSelect(void);
void UARTInit(void);

void EDID_WP_ENABLE(void);	
void EDID_WP_DISABLE(void);
BOOL CheckADCCaliData(gmt_ADC_SETTING * CalibrationTable);

BYTE GetPrevMainAudInput(void);
BYTE GetNextMainAudInput(void);

BYTE GetPrevMainAudOutput(void);
BYTE GetNextMainAudOutput(void);

BYTE GetPrevPxpAudInput(void);
BYTE GetNextPxpAudInput(void);

BYTE GetPrevPxpAudOutput(void);
BYTE GetNextPxpAudOutput(void);

gmt_UserDefConnNames GetNextPxpPort(void);
gmt_UserDefConnNames GetPrevPxpPort(void);
gmt_UserDefConnNames GetNextMainPort_4K2K(void);
gmt_UserDefConnNames GetPrevMainPort_4K2K(void);


void SetupDPPanel(void);
void DCR_BACKLIGHT_ON(void);
void DCR_BACKLIGHT_OFF(void) ;
void BacklightInit(void);
void AdjustBacklightForLogo(void);
BOOL ReCheckInput(gmt_PHY_CH B_Channel);
void ModeSetupDisplayScaling(gmt_PHY_CH B_Channel, BYTE B_Step);
void AdjustPxpMode(void);
void AdjustMainPosition(void);
void AdjustOutputMode(void);
void AdjustMainMode(void);
void AdjustMainBlendLevel(void);

#if ((BOARD_USED == BOARD_93xx_CJ2))
BOOL Board_External_Detected(void);
#endif
void AdjustUartBaudRate(void);
DWORD GetUartBaudRate(void);
#ifdef CHANNEL_CLONE
BOOL IsCloneAvariable(gmt_PHY_CH B_Channel);
#endif
void SetOSDByPanelParameters(void);
void CheckInputAlignment(gmt_PHY_CH B_Channel);
void AdjustAudioMute(void);
void AdjustAudioInputSource(void);
void StartNoCableSleep(void);
void CheckScanlingFilterCtrl(gmt_PHY_CH B_Channel);

WORD GetPanelWidth(void);


BYTE GetPrevImageRotation(void);
BYTE GetNextImageRotation(void);

BYTE GetPrevOSDRotation(void);
BYTE GetNextOSDRotation(void);

void CheckNoSyncBKGColor(gmt_PHY_CH B_Channel);
BOOL CheckDPLinkError(gmt_PHY_CH B_Channel);
void AdjustDimEffect(void);
void ReInitOSD(void);
void PanelSetAsInputTiming (void);
void AdjustPanelHStart(gmt_PHY_CH B_Channel);    //wei 140530
void AdjustPanelProtectCount();  // 140530
BOOL IsDecoderInputStable(gmt_PHY_CH B_Channel);

void QFHD_LVDSConfig(void);
void AdjustOSDAudioVolume(void);
void SetOsdTilePosition_Flying(WORD W_TileId, WORD W_TileHpos, WORD W_TileVpos);
void AdjustFlyingOsdHPosition(void);
void AdjustFlyingOsdVPosition(void);
void ZDT_4K2K_OutputConfig(void);
void TurnBacklightOn(void);  //Neil 140816

#endif // #ifndef __CUSTOM_APP_H__

