
#ifndef __PROJ_CONFIG_SUV_H__
#define __PROJ_CONFIG_SUV_H__

//==================IMPORTANT!!!!=============================
//Project select for OSD and application, Please check BOARD_USED define!!!!!!!!!!!
//==========================================================
#define PROJECT_SELECT 		PROJ_SUV

//#define DVI_SINGLE_LINK					//Set DVI to Single link for Pxp convernience

	//==============Application function select============================
	#define VWD_DEF						VWD_DEMO_OFF
	#define OSD_HARD_LOCK				DISABLE
	#define USER_IR_SUPPORT
	#define ON_BOARD_COMP				DISABLE 			//Athena component enable
	#define FEATURE_TDA7491 			ENABLE		//ENABLE : Has Speaker on board; DISABLE: Hasn't Speaker on board
	#define NO_CABLE_COUNT_DOWN 		DISABLE
	
	#define SPLASHSCREEN_BKG_COLOR		WHITE
	#define MAIN_BACKGROUND_COLOR		BLUE
	#define PIP_BACKGROUND_COLOR		BLUE
	#define FEATURE_LAMP				DISABLE
	#define FLY_WIN_RANDOM				ENABLE
	#define SPEEDUP_MODE_CHANGE			DISABLE
	//#define PORT_CHANGE_SEAMLESS		

	//====================OSD default value setting======================
	#if(PROJECT_SELECT == PROJ_SUV)
		#define LOGO_SELECT 				LOGO_ZDT
		#define ModeName					"ZDT SUV"
		#define FLYING_WINDOW				DISABLE 
		#define CUSTOMER_INFO				DISABLE
		#define LANG_DEF					LANG_Simply_Chinese
		#define DP_SWITCH_DEF				DP_SWITCH_1v2		//Athena component enable
		#define WB_BRIGHTNESS_DEFAULT		((WB_BRIGHTNESS_MAX - WB_BRIGHTNESS_MIN)/2)//256		//Move here
		
		#define SHOW_LOGO_WITHOUT_SYNC			ENABLE
		#define WB_BOARD_ID_DEF 				1
		
		#define DECODER_IN_LPM					0
		
		#if (CUSTOMER_INFO == DISABLE)
			#define WB_LOGO_DEF 					OFF
		#else
			#define WB_LOGO_DEF 					ON
		#endif
		
	#endif
	
	#define WB_PORT_DEF_MAIN				ALL_BOARDS_VGA1
	#define WB_PORT_DEF_PXP 				ALL_BOARDS_HDMI
	
	#define OSD_NoSync_Time				150		//OSD timer resolution change to 0.1s from 1s
	#define OSD_NoSyncByWakeUp_Time		200
	//#define OSD_NoCable_Time				300


#if(FEATURE_TDA7491 == ENABLE)
	#define WB_AUDIO_OUT_DEF			gmd_AUDIO_TAR_LINE_OUT
#else
	#define WB_AUDIO_OUT_DEF			gmd_AUDIO_TAR_HeadPhone_OUT
#endif

#ifdef UART_DEBUG_MODE
	#define OSD_UART_DEF					UART0	//PD use Uart1 as system control
	#define BAUDRATE_SELECT 				115200UL//38400UL//9600UL
#else
	#define OSD_UART_DEF					UART1	//PD use Uart1 as system control
	#define BAUDRATE_SELECT 				19200UL//38400UL//9600UL
#endif
#define WB_BAUDRATE_DEF 				BR_19200//BR_19200//38400UL//9600UL

#define NoCableCNT 		297 	// 5Mins		

//============================================
#ifdef UART_DEBUG_MODE
	#define NVRAM_CHECKSUM				(CHECKSUM_DEFAULT  + 0x59A) 
	#define FACTORY_NVRAM_VERSION		(CHECKSUM_DEFAULT  + 0x5AB) //120803 Edward add factory block check
#else	//UART_DEBUG_MODE
	#define NVRAM_CHECKSUM				(CHECKSUM_DEFAULT  + 0x591) 
	#define FACTORY_NVRAM_VERSION		(CHECKSUM_DEFAULT  + 0x5A2) //120803 Edward add factory block check
#endif


//================OSD Function select================
#define WB_Max_XNumber					1
#define WB_Max_YNumber					1

#if(SPEEDUP_MODE_CHANGE == ENABLE)
#define SPEEDUP_FACT					10
#endif

#endif	//End of backlight settings
