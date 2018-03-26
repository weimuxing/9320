
#ifndef __PROJ_CONFIG_H__
#define __PROJ_CONFIG_H__

//***************************************
//  P R O J E C T  S E L E C T I O N
//***************************************
//HYB130108+ for different project OSD
//Don't change the order, it will impact LPM settings
#define	PROJ_SUV       		0 		//surviliance This is base set to 0 avoid some mistake	
#define PROJ_4K2K			5		
#define PROJ_MNT			7		//For Monitor project

#define LOGO_ZDT 			0


//==================IMPORTANT!!!!=============================
//Project select for OSD and application, Please check BOARD_USED define!!!!!!!!!!!
//==========================================================

//===============FW version, date and Mode name, OSD default control=================
#define CustomApp_FW_Version 		"-140820V00"
#define CustomApp_FW_Date 			"ZJ8S20-A-CMI-V650DK1"    //"20140513"

//==============================================

//==============Application function select===============
#define PRINT_IMPORTANT_MSG				DISABLE		//ENABLE: Will print important message; DISABLE: Will not print important message;
#define FEATURE_MultiPxp				DISABLE		//Hyb130412 Don't enable it since we didn't fine tune it;
#define FEATURE_SOURCE_SCAN				DISABLE		//Enable source scan

//=============================================
#define IsLogo_ON()			(UserPrefLOGO_Ctrl == ON)
//ZDT
#define IsLogo_ZDT()		((CUSTOMER_INFO == ENABLE)&&(LOGO_SELECT == LOGO_ZDT))


//================OSD Function select================
#define IS_PROJ_PD()			(gmd_FALSE)
#define IS_PROJ_HAS_AV()		((PROJECT_SELECT == PROJ_SUV))
#define IS_PROJ_HAS_YPbPr()		((PROJECT_SELECT == PROJ_SUV))
#define IS_PROJ_4K2K()			((PROJECT_SELECT == PROJ_4K2K))

//DOWN set as Pxp Hotkey;
#define IS_HOTKEY_PXP()			((PROJECT_SELECT != PROJ_4K2K)&&(DEFAULT_PANEL == SHARP_1080P_LD900D3LE1))

//INX8921 doesn't have rotation function
#define IS_IMAGE_ROTATION_EN()	(DEFAULT_PANEL != ZDT_4K2K_CMI)
//===============Board Settings==============================

#if (BOARD_USED == BOARD_93xx_CJ2)
#define Board_ID 					"Board CJ2"
#endif

#define DP_EDID_FROM_NVRAM		0
#define DP_EDID_FROM_CODE		1


#if(BOARD_USED == BOARD_93xx_CJ2)
//#define DP_EDID_LOCATION		DP_EDID_FROM_NVRAM
#define DP_EDID_LOCATION		DP_EDID_FROM_CODE
#else
#define DP_EDID_LOCATION		DP_EDID_FROM_NVRAM
//#define DP_EDID_LOCATION		DP_EDID_FROM_CODE

#endif

#endif
