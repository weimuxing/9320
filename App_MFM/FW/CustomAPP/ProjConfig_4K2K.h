
#ifndef __PROJ_CONFIG_MNT_H__
#define __PROJ_CONFIG_MNT_H__

//==================IMPORTANT!!!!=============================
//Project select for OSD and application, Please check BOARD_USED define!!!!!!!!!!!
//==========================================================
#define PROJECT_SELECT			PROJ_4K2K		// DP EDID
//===============FW version, date and Mode name, OSD default control=================
//#define DVI_SINGLE_LINK					//Set DVI to Single link for Pxp convernience

//==============Application function select============================
#define ON_BOARD_COMP				DISABLE 		//Athena component enable
#define OSD_HARD_LOCK				DISABLE
#define USER_IR_SUPPORT
#define FEATURE_TDA7491 			ENABLE		//ENABLE : Has Speaker on board; DISABLE: Hasn't Speaker on board
		
#define MAIN_BACKGROUND_COLOR		BLUE
#define PIP_BACKGROUND_COLOR		BLUE //BLACK
#define FEATURE_LAMP				        DISABLE
#define FLY_WIN_RANDOM				DISABLE
#define SPEEDUP_MODE_CHANGE 		DISABLE
//#define OUTPUT_MODE_CHANGE
#define NO_CABLE_COUNT_DOWN 		DISABLE
		
//====================OSD default value setting======================
#define LOGO_SELECT 				LOGO_ZDT

#define ModeName					"4K2K"

//#define TOUCH_KEY
#ifdef TOUCH_KEY
	#define TOUCHKEY_INVERSE
#endif

#define CUSTOMER_INFO				DISABLE
#define SPLASHSCREEN_BKG_COLOR		BLUE
#define SHOW_LOGO_WITHOUT_SYNC		DISABLE
#define FLYING_WINDOW				ENABLE  //DISABLE 
#define WB_LOGO_DEF 				OFF
#define LANG_DEF						LANG_Simply_Chinese

#define VWD_DEF 						VWD_DEMO_OFF
#define DP_SWITCH_DEF				DP_SWITCH_1v1a					//Athena component enable
#define WB_BRIGHTNESS_DEFAULT		((WB_BRIGHTNESS_MAX - WB_BRIGHTNESS_MIN)/2)  //256		//Move here
#ifdef UART_DEBUG_MODE
#define OSD_UART_DEF					UART0	//PD use Uart1 as background control
#define BAUDRATE_SELECT 				115200UL//38400UL//9600UL
#else
#define OSD_UART_DEF					UART1	//PD use Uart1 as background control
#define BAUDRATE_SELECT 				19200UL//38400UL//9600UL
#endif
#define WB_BAUDRATE_DEF					BR_115200//38400UL//9600UL
	
//#define MODE_CHANGE_BACKLIGHT_OFF		// Neil 140816 For panel Vcc direct connect to power supply 12V. PPWR only control backlight

#ifdef HDMI_ONLY
#define WB_PORT_DEF_MAIN				ALL_BOARDS_HDMI
#define WB_PORT_DEF_PXP 				ALL_BOARDS_VGA1
#else
#define WB_PORT_DEF_MAIN				ALL_BOARDS_VGA1
#define WB_PORT_DEF_PXP 				ALL_BOARDS_HDMI
#endif

#define WB_BOARD_ID_DEF 					1

#if(FEATURE_TDA7491 == ENABLE)
#define WB_AUDIO_OUT_DEF			gmd_AUDIO_TAR_LINE_OUT
#else
#define WB_AUDIO_OUT_DEF			gmd_AUDIO_TAR_HeadPhone_OUT
#endif

#define NoCableCNT 		297 	// 5Mins		

//==============================================

//==============Application function select===============
	//BOARD_93XX_MNT

#ifdef UART_DEBUG_MODE
	#define NVRAM_CHECKSUM    			(CHECKSUM_DEFAULT  + 0x49A) 
	#define FACTORY_NVRAM_VERSION		(CHECKSUM_DEFAULT  + 0x4AB) //120803 Edward add factory block check		
#else	//UART_DEBUG_MODE
	#define NVRAM_CHECKSUM				(CHECKSUM_DEFAULT  + 0x491) 
	#define FACTORY_NVRAM_VERSION		(CHECKSUM_DEFAULT  + 0x4A2) //120803 Edward add factory block check 
#endif
//=============================================

//================OSD Function select================
#define OSD_NoSync_Time				150		//OSD timer resolution change to 0.1s from 1s
#define OSD_NoSyncByWakeUp_Time		200
//#define OSD_NoCable_Time				300


#define WB_Max_XNumber					1
#define WB_Max_YNumber					1
#if(SPEEDUP_MODE_CHANGE == ENABLE)
#define SPEEDUP_FACT					10
#endif
#endif
