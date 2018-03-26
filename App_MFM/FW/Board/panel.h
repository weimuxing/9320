/*
	$Workfile:   panel.h  $
	$Revision: 1.50 $
	$Date: 2013/05/20 07:01:13 $
*/

#ifndef __PANEL_H__
#define __PANEL_H__
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
//================================================================
//
// MODULE:  panel.h
//
// USAGE :  Panel parameters to setup display output registers.
//
// NOTE  :  Select panel using the panel number
//          defined in the enum shown below.
//
//			-- IMPORTANT --
//
//			The panel selection consists of 2 portions. A macro define prefixed with
//			"INCLUDE_" indicates which panels can be supported with the same HEX file. Due to code space
//			limitation, select only the panel(s) you want to support with the same flash. The macro define
//			"DEFAULT_PANEL" is the default panel selected when NVRAM is first initialized. It can be changed later.
//			This macro define can only be assigned to 1 panel.  For more detailes read below.
//
//******************************************************************************

//******************************************************************************
// G L O B A L    D E F I N I T I O N
//******************************************************************************

//
// New panels can be added by appending to the end of the list.
// The corresponding panel header files also need to include the panel ID
// (based on the enum below)
//
typedef enum PANEL_ID
{
	LG_XGA_150X06 = 1,
	LG_XGA_150X06_VGA,
	LG_WXGA_LM171W01,
	LG_WXGA_LC171W03,
	LG_WXGA_LC230W01,
	LG_WXGA_LC300_W01,
	LG_WXGA_LC320W01,
	SAMSUNG_WXGA_LTM170W01,
	SAMSUNG_SXGA_LTM170E4_L01,
	SAMSUNG_UXGA_LTM213U4_L01,		// 10
	LG_SXGA_LM190E01,
	CUSTOMER_WXGA_PANEL_1,
	CUSTOMER_WXGA_1366,
	CUSTOMER_WXGA_DLP,
	CUSTOMER_WXGA_720P,
	LG_WXGA_LC171W03_VGA,
	Sanyo_TM290WX_71N31,
	AUO_WXGA_T315XW01,
	LG_WXGA_LC200WX1,
	AUO_1080P_T370HW01,				// 20
	AUO_1080P_T370HW02,
	CMO_1080P_V370H1_L01,
	SAMSUNG_WUXGA_LTM240W1_L01,
	LG_WUXGA_LM260WU2_SLA1,
	LPL_WUXGA_LM240WU5,
	LG_UXGA_LM201U03,
	CUSTOMER_1080P_DVIOUT,
	LG_1080P_LC370WU1,
	IPS_1080P_AX094F002E,
	SAMSUNG_1080P_LTA400HH_LH1,		// 30
	LG_WSXGA_LM220WE1_TLD1,
	LG_WUXGA_LM240WU4_SLB1,
	LG_WUXGA_LM240WU9_SLA1, 		// 33 //DELL 2413
	CRT_33K,
	CRT_38K,
	INT_1080I,
	LG_WXGA_LC370WX2, // 100/120 Hz panel
	CUSTOMER_720P_DVIOUT,
	CUSTOMER_1080I_DVIOUT,
	CUSTOMER_480P_DVIOUT,			// 40
	LG_WQXGA_LM300WQ5_SLA1,			// 41
	LG_1080P_LM230WF8_TLA1,
	CMO_1080P_M236H5_L02_60,
	CMO_1080P_M236H5_L02_120,
	LG_1080P_LM270WF3_120,
	ST_WQXGA_VEGA_WQXGA,
	ST_1080P_VEGA_1080P,
	LG_WQHD_LM270WQ1,				// 48
	ST_WQHD_VEGA_WQHD,
	SONY_2XGA_S_2XGA_SG,				// 50
	ST_2K2K_VEGA_2K2K,
	LG_1080P_LC470EUF_120,
	CMO_V562D1_L03,   
	LG_WHD_LM290WW1_SSA1,			// 54
	CMO_WSXGA_M220Z1_L09,
	LD_T120001A,
	LG_1080P_LC320EUN,
	ZDT_4K2K_QFHD,					// 58
	SAMSUNG_WXGA_LTI260AP01,
	LG_WXGA_LC320EXN,				// 60
	SAMSUNG_1080P_LTI460HN05,		//61
	SAMSUNG_WXGA_LTI460AA05,		//62
	CMO_WSXGA_M220Z1_L01,		//63
	ZDT_2K2K,					//64
	ZDT_4K1K,					//65
	LG_WQXGA_LM300WQ6_SLA1, // 66
	LG_1080P_LD420WUB_4K2K, 		// 67
	LG_1080P_LD420WUB_10BIT,			//68
	LG_1080P_LD470WUB_8BIT, 		//69
	LG_1080P_LD420WUB_8BIT, 		//70
	LG_1080P_LD470WUB_10BIT,			//71
	
	SAMSUNG_1080P_LTI460HM03,		// 72
	SAMSUNG_1080P_LTI460HN09,		// 73
	
	AUO_1080P_T550HVN01V9_8BIT, 	// 74
	AUO_1080P_T650HVN02_10BIT,		// 75
	SAMSUNG_1080P_LTI400HA02,	//76
	ZDT_4K2K_STD,					// 77
	ZDT_4K2K_CMI,					// 78		//IN8921
	ZDT_4K2K_CMI_39,					// 79	//CM3971 39
	SHARP_1080P_LD900D3LE1, 			//80
		
	NLT_NL204153AM21,				// 81
	SAMSUNG_WQHD_LTM270DL02,		// 82
	LG_WUXGA_LM240WU4_SLA1,			// 83
	ZDT_4K2K_CMI_65,				// 84		//CM3971 65
	LG_1080P_LD550DUS,				//85		LG 55" 10Bits
	
	ZDT_4K2K_CMI_50 = 91,				// 91
   // Add new panels below
}gmd_MasterPanelList;


//
// The list of uncommented defines indicates which panels can be supported
// with the same compiled HEX file (only include the ones required to save code space)
//

#if(defined(BOARD_93XX_4K2K))
#define INCLUDE_ZDT_4K2K_STD
#define INCLUDE_ZDT_4K2K_QFHD
#define INCLUDE_ZDT_4K2K_CMI
#define INCLUDE_ZDT_4K2K_CMI_39
#define INCLUDE_ZDT_4K2K_CMI_65
#define INCLUDE_ZDT_4K2K_CMI_50
#else

//#define INCLUDE_LG_XGA_150X06
//#define INCLUDE_LG_XGA_150X06_VGA
//#define INCLUDE_LG_WXGA_LM171W01
//#define INCLUDE_LG_WXGA_LC171W03
//#define INCLUDE_LG_WXGA_LC171W03_VGA
//#define INCLUDE_LG_WXGA_LC230W01
//#define INCLUDE_LG_WXGA_LC300_W01
//#define INCLUDE_LG_WXGA_LC320W01
//#define	INCLUDE_LG_WXGA_LC370WX2       // 100/120 Hz panel
//#define INCLUDE_SAMSUNG_WXGA_LTM170W01
//#define INCLUDE_SAMSUNG_SXGA_LTM170E4_L01
//#define INCLUDE_SAMSUNG_UXGA_LTM213U4_L01
//#define INCLUDE_LG_SXGA_LM190E01
//#define INCLUDE_Sanyo_TM290WX_71N31
//#define INCLUDE_AUO_WXGA_T315XW01
//#define INCLUDE_CUSTOMER_WXGA_PANEL_1
//#define INCLUDE_CUSTOMER_WXGA_DLP
//#define INCLUDE_CUSTOMER_WXGA_720P
//#define INCLUDE_CUSTOMER_WXGA_1366
//#define INCLUDE_LG_WXGA_LC200WX1
//	#define INCLUDE_AUO_1080P_T370HW01
//	#define INCLUDE_AUO_1080P_T370HW02
//	#define INCLUDE_CMO_1080P_V370H1_L01
//	#define INCLUDE_LG_WUXGA_LM260WU2_SLA1
//	#define INCLUDE_LG_UXGA_LM201U03
//	#define INCLUDE_CUSTOMER_1080P_DVIOUT
//	#define INCLUDE_LG_1080P_LC370WU1
//   #define INCLUDE_IPS_1080P_AX094F002E
//	#define INCLUDE_SAMSUNG_1080P_LTA400HH_LH1
	#define INCLUDE_LPL_WUXGA_LM240WU5 // Obiwan
//#define INCLUDE_CRT_33K
//#define INCLUDE_CRT_38K
//#define INCLUDE_1080I
//#define INCLUDE_CUSTOMER_720P_DVIOUT
//#define INCLUDE_CUSTOMER_1080I_DVIOUT
//#define INCLUDE_CUSTOMER_480P_DVIOUT
#define INCLUDE_LG_WQXGA_LM300WQ5_SLA1
//#define INCLUDE_LG_1080P_LM230WF8_TLA1
//#define INCLUDE_CMO_1080P_M236H5_L02_60
//#define INCLUDE_CMO_1080P_M236H5_L02_120
//#define INCLUDE_LG_1080P_LM270WF3_120
//#define INCLUDE_ST_WQXGA_VEGA_WQXGA
//#define INCLUDE_ST_1080P_VEGA_1080P
#define INCLUDE_LG_WQHD_LM270WQ1
//#define INCLUDE_ST_WQHD_VEGA_WQHD
//#define INCLUDE_SONY_2XGA_S_2XGA_SG
#define INCLUDE_LG_WUXGA_LM240WU4_SLB1 // 2410
#define INCLUDE_LG_WUXGA_LM240WU9_SLA1 // 2413
//#define INCLUDE_LG_WSXGA_LM220WE1_TLD1
//#define INCLUDE_SAMSUNG_WUXGA_LTM240W1_L01
//#define INCLUDE_ST_2K2K_VEGA_2K2K
//#define INCLUDE_LG_1080P_LC470EUF_120
//#define INCLUDE_CMO_2K2K_V562D1_L03
#define INCLUDE_LG_WHD_LM290WW1_SSA1
//#define INCLUDE_LG_1080P_LC320EUN_SDV1_60
//#define INCLUDE_CMO_2K2K_LD_T120001A
//#define INCLUDE_SAMSUNG_WXGA_LTI260AP01
//#define INCLUDE_SAMSUNG_1080P_LTA460HM03
#define INCLUDE_LG_WXGA_LC320EXN
//#define INCLUDE_CMO_WSXGA_M220Z1_L09
#define INCLUDE_LG_1080P_LC320EUN
//#define INCLUDE_SAMSUNG_1080P_LTI460HN05

//#define INCLUDE_SAMSUNG_WXGA_LTI460AA05
#define INCLUDE_CMO_WSXGA_M220Z1_L01
#define INCLUDE_ZDT_2K2K
#define INCLUDE_ZDT_4K1K
#define INCLUDE_LG_WQXGA_LM300WQ6_SLA1
//#define INCLUDE_LG_1080P_LD420WUB_4K2K  	
//#define INCLUDE_LG_1080P_LD420WUB_10BIT
//#define INCLUDE_LG_1080P_LD470WUB_8BIT
//#define INCLUDE_LG_1080P_LD420WUB_8BIT
//#define INCLUDE_LG_1080P_LD470WUB_10BIT
//#define INCLUDE_CMO_1080P_M236H5_4K2K
#define INCLUDE_SAMSUNG_1080P_LTI460HM03
#define INCLUDE_AUO_1080P_T550HVN01V9_8BIT
#define INCLUDE_AUO_1080P_T650HVN02_10BIT
//#define INCLUDE_SHARP_1080P_LD900D3LE1
//#define INCLUDE_NLT_NL204153AM21
//#define INCLUDE_SAMSUNG_WQHD_LTM270DL02
#endif


#if 0//(USE_NEW_GAMMA != 1)
#define INCLUDE_SAMSUNG_1080P_LTI400HA02
#define INCLUDE_SAMSUNG_1080P_LTI460HM01
#define INCLUDE_SAMSUNG_1080P_LTI460HM02
#define INCLUDE_SAMSUNG_1080P_LTI550HN01
#define INCLUDE_SAMSUNG_1080P_LTI550HJ04
#define INCLUDE_LG_1080P_LC320WUN	
#define INCLUDE_AUO_1080P_P420HW01
#define INCLUDE_AUO_1080P_P460HW03
#endif

//******************************************************************************
//              S E L E C T E D   P A N E L
//******************************************************************************

//
// Only 1 uncommented DEFAULT_PANEL macro should be defined. This configures the panel to be the one after
// NVRAM is reset and the system is powered on for the first time. For multiple panel support solutions the default
// panel selection can be changed by altering the user preference (UserPrefPanelSelect). Refer to appstest #255
// which calls the function "SelectPanel" for an example of how to changes panels "on the fly".
//

//	#define DEFAULT_PANEL	LG_XGA_150X06
//	#define DEFAULT_PANEL	LG_XGA_150X06_VGA
//	#define DEFAULT_PANEL	LG_WXGA_LM171W01
//	#define DEFAULT_PANEL	LG_WXGA_LC171W03
//	#define DEFAULT_PANEL	LG_WXGA_LC171W03_VGA
//	#define DEFAULT_PANEL	LG_WXGA_LC230W01
//	#define DEFAULT_PANEL	LG_WXGA_LC300_W01
//	#define DEFAULT_PANEL	LG_WXGA_LC320W01
//	#define DEFAULT_PANEL	LG_WXGA_LC370WX2        // 100/120 Hz panel
//	#define DEFAULT_PANEL	SAMSUNG_WXGA_LTM170W01
//	#define DEFAULT_PANEL	SAMSUNG_SXGA_LTM170E4_L01
//	#define DEFAULT_PANEL	SAMSUNG_UXGA_LTM213U4_L01
//	#define DEFAULT_PANEL	LG_SXGA_LM190E01
//	#define DEFAULT_PANEL	Sanyo_TM290WX_71N31
//	#define DEFAULT_PANEL	AUO_WXGA_T315XW01
//	#define DEFAULT_PANEL	CUSTOMER_WXGA_PANEL_1
//	#define DEFAULT_PANEL	CUSTOMER_WXGA_1366
//	#define DEFAULT_PANEL	CUSTOMER_WXGA_DLP
//	#define DEFAULT_PANEL	CUSTOMER_WXGA_720P
//	#define DEFAULT_PANEL	LG_WXGA_LC200WX1
//	#define DEFAULT_PANEL	AUO_1080P_T370HW01
//	#define DEFAULT_PANEL	AUO_1080P_T370HW02
//	#define DEFAULT_PANEL	CMO_1080P_V370H1_L01
//	#define DEFAULT_PANEL	LG_WUXGA_LM260WU2_SLA1
//	#define DEFAULT_PANEL	LG_UXGA_LM201U03
//	#define DEFAULT_PANEL	CUSTOMER_1080P_DVIOUT
//	#define DEFAULT_PANEL	LG_1080P_LC370WU1
//    #define DEFAULT_PANEL	IPS_1080P_AX094F002E
//	#define	DEFAULT_PANEL	SAMSUNG_1080P_LTA400HH_LH1
//	#define DEFAULT_PANEL	CRT_33K
//	#define DEFAULT_PANEL	CRT_38K
//	#define DEFAULT_PANEL	INT_1080I
//	#define DEFAULT_PANEL	CUSTOMER_720P_DVIOUT
//	#define DEFAULT_PANEL	CUSTOMER_1080I_DVIOUT
//	#define DEFAULT_PANEL	CUSTOMER_480P_DVIOUT

// The under is tested
//	#define DEFAULT_PANEL	LG_1080P_LM230WF8_TLA1
//	#define DEFAULT_PANEL	CMO_1080P_M236H5_L02_60 
//	#define DEFAULT_PANEL	CMO_1080P_M236H5_L02_120
//	#define DEFAULT_PANEL	LG_1080P_LM270WF3_120
// 	#define DEFAULT_PANEL ST_WQXGA_VEGA_WQXGA
// 	#define DEFAULT_PANEL   ST_1080P_VEGA_1080P
// 	#define DEFAULT_PANEL   SAMSUNG_WUXGA_LTM240W1_L01
// 	#define DEFAULT_PANEL	LG_WUXGA_LM240WU4_SLB1 // 2410
// 	#define DEFAULT_PANEL	LG_WQHD_LM270WQ1
// 	#define DEFAULT_PANEL   ST_WQHD_VEGA_WQHD
//	#define DEFAULT_PANEL	SONY_2XGA_S_2XGA_SG
// 	#define DEFAULT_PANEL	LG_WSXGA_LM220WE1_TLD1
// 	#define DEFAULT_PANEL   ST_2K2K_VEGA_2K2K
//	#define DEFAULT_PANEL	LG_1080P_LC470EUF_120
//	#define DEFAULT_PANEL CMO_V562D1_L03
#if (defined(Athena_U))
	//#define DEFAULT_PANEL LG_1080P_LC320EUN//ST_1080P_VEGA_1080P//LG_1080P_LC320EUN//
	#if (defined(BOARD_93XX_SUV))
		#define DEFAULT_PANEL LG_WQHD_LM270WQ1
		//#define DEFAULT_PANEL SAMSUNG_1080P_LTI460HM03
		//#define DEFAULT_PANEL LG_WQXGA_LM300WQ6_SLA1
	#elif(defined(BOARD_93XX_4K2K))
		//#define DEFAULT_PANEL SAMSUNG_1080P_LTI460HM03	//LG_1080P_LD420WUB_4K2K//PD 1080p
		//#define DEFAULT_PANEL ZDT_4K2K_QFHD// 4K2K
		//#define DEFAULT_PANEL ZDT_4K2K_CMI		// 4K2K	INX8921s
		//#define DEFAULT_PANEL ZDT_4K2K_CMI_39// 4K2K CM3971
		#define DEFAULT_PANEL ZDT_4K2K_CMI_65   //ZDT_4K2K_CMI_65
		//#define DEFAULT_PANEL  ZDT_4K2K_CMI_50
		//#define DEFAULT_PANEL LG_1080P_LD420WUB_4K2K
		//#define DEFAULT_PANEL CMO_1080P_M236H5_L02_60// 4K2K
	#else	//MNT
		//#define DEFAULT_PANEL LG_WQHD_LM270WQ1
		//#define DEFAULT_PANEL LG_WQXGA_LM300WQ6_SLA1
		//#define DEFAULT_PANEL AUO_1080P_T550HVN01V9_8BIT
		//#define DEFAULT_PANEL AUO_1080P_T650HVN02_10BIT
		#define DEFAULT_PANEL LG_1080P_LC320EUN
		//#define DEFAULT_PANEL LG_WXGA_LC320EXN	//PD 1366x768
		//#define DEFAULT_PANEL SAMSUNG_WXGA_LTI460AA05 	//PD 1366x768
		//#define DEFAULT_PANEL NLT_NL204153AM21//1680 x 1050@120Hz
		//#define DEFAULT_PANEL	LPL_WUXGA_LM240WU5 // Obiwan
		//#define DEFAULT_PANEL	LG_XGA_150X06			// 3.3V  VCC
		//#define DEFAULT_PANEL 	LG_1080P_LM230WF8_TLA1	//12V VCC
		//#define DEFAULT_PANEL SHARP_1080P_LD900D3LE1
		#endif
#else
	#define DEFAULT_PANEL	LG_WUXGA_LM240WU9_SLA1 
#endif	

//For latest add panel, demo code use new gamma adjust;

#define USE_NEW_GAMMA 		((DEFAULT_PANEL == LG_WQHD_LM270WQ1) || (DEFAULT_PANEL == LG_WQXGA_LM300WQ5_SLA1)||(DEFAULT_PANEL == LG_WQXGA_LM300WQ6_SLA1)\
							||(DEFAULT_PANEL == LG_WHD_LM290WW1_SSA1)||(DEFAULT_PANEL == LG_WUXGA_LM240WU4_SLB1)\
							||(DEFAULT_PANEL == LG_WUXGA_LM240WU9_SLA1)||(DEFAULT_PANEL == LPL_WUXGA_LM240WU5))		
							
#if (DEFAULT_PANEL == LG_WQHD_LM270WQ1)
	#define DPRX_PANEL		ENABLE
#else
#define DPRX_PANEL		DISABLE
#endif
//******************************************************************************
//******************************************************************************

/*ChengLin 20120217: Add for QFHD Panel*/
#if (DEFAULT_PANEL == CMO_V562D1_L03) || (DEFAULT_PANEL == LD_T120001A)
#define PANEL_DATA_LEFT_RIGHT	 ENABLE // ENABLE for left/right data panel
#else

//#if (DEFAULT_PANEL == CMO_1080P_M236H5_L02_60) || (DEFAULT_PANEL == CMO_1080P_M236H5_L02_120)
#define PANEL_DATA_LEFT_RIGHT 	DISABLE // ENABLE for left/right data panel
//#endif
#endif


#ifdef ST_4K2K_93xx_BOARD
#define DEFAULT_PANEL_FE   ST_1080P_VEGA_1080P   
#endif

//
// panel type definitions
//
typedef enum PANEL_CONTROL
{
   PANEL_CTRL_LVDS_ON_REQUEST          =BIT0,
   PANEL_CTRL_BKL_ON_REQUEST           =BIT1,
   PANEL_CTRL_LVDS_OFF_REQUEST         =BIT2,   
   PANEL_CTRL_PWR_OFF_REQUEST          =BIT3,   
   PANEL_CTRL_REQ_2_IMAGE_SYNC_STATE   =BIT4
};

typedef enum PANEL_TYPE
{
   PANEL_XGA,
   PANEL_WXGA,
   PANEL_SXGA,
   PANEL_UXGA,
   PANEL_1080P,
   PANEL_WUXGA,
   PANEL_480I,
   PANEL_576I,
   PANEL_1080I,
   PANEL_720P,
   PANEL_480P,
   PANEL_WSXGA,
   PANEL_WQXGA,
   PANEL_WQHD,
   PANEL_2XGA,
   PANEL_2K2K,
   PANEL_4K1K,
   PANEL_4K2K,
   MAX_PANEL_TYPE,
}gmd_PanelType;

//
// number of bits per pixel definitions
//
enum PANEL_PIXPERCLK
{
   ONE,
   TWO,
   THREE,
   FOUR,
   MAX_PIXPERCLK,
};
//
// number of bits for panel output definitions
//
enum PANEL_BIT_OUTPUT
{
   OUT_24_48_BIT,
   OUT_30_BIT,
   MAX_BIT_OUTPUT,
};
//
// LVDS parameters
//
enum LVDS_PARAM
{
   SINGLE_WIDE_LVDS,
   DOUBLE_WIDE_LVDS_A_EVEN,
   DOUBLE_WIDE_LVDS_B_EVEN,
};

enum LVDS_PANEL_MODE
{
   NO_LVDS,
   LVDS_SIX_BIT_MODE,
   LVDS_EIGHT_BIT_MODE,
};

enum LVDS_POS_NEG_SWAP
{
   SWAP,
   NO_SWAP,
};

enum LVDS_PIX_CLK
{
   SINGLE_BUS,
   DUAL_BUS,
   RESERVED,
   QUAD_BUS
};

typedef enum
{
   STPANEL_LVDS_MAP_STANDARD1,
   STPANEL_LVDS_MAP_STANDARD2,
   STPANEL_LVDS_MAP_CUSTOM,
} STPANEL_LVDSMap_t;

typedef enum
{
   STPLF_DISPLAY_PAD_DRIVE_1_25 = 0,
   STPLF_DISPLAY_PAD_DRIVE_1_1 = 1,
   STPLF_DISPLAY_PAD_DRIVE_1_35 = 2,
} STPLF_DisplayPadDrive_t;


enum LVDS_CURRENT_CTRL
{
   THREE_PT_SIX_MA,
   FOUR_PT_SIX_MA,
   FIVE_PT_SIX_MA,
   SEVEN_PT_ONE_MA
};


typedef enum PANEL_FORMAT
{
   FORMAT_ORG_2D,
   FORMAT_3D_FRAME_SEQUENTIAL,
   FORMAT_3D_LINE_ALTERNATIVE,
   FORMAT_3D_SIDE_BY_SIDE
} gmd_PanelDataFormat;


typedef enum
{
   STPLF_DISPLAY_LVDS_CURRENT_0_5 = 0,
   STPLF_DISPLAY_LVDS_CURRENT_1_0 = 1,
   STPLF_DISPLAY_LVDS_CURRENT_1_5 = 2,
   STPLF_DISPLAY_LVDS_CURRENT_2_0 = 3,
   STPLF_DISPLAY_LVDS_CURRENT_2_5 = 4,
   STPLF_DISPLAY_LVDS_CURRENT_3_0 = 5,
   STPLF_DISPLAY_LVDS_CURRENT_3_5 = 6,
   STPLF_DISPLAY_LVDS_CURRENT_4_0 = 7,
} STPLF_DisplayLvdsCurrent_t;

#if 0
typedef struct						// Structure for WORD
{
   WORD DisplayHtotal;				// Display H total
   WORD  DisplayVtotal;			// Display V total
} gmt_DisplayOutputTotals;


typedef struct
{
   gmt_DisplayOutputTotals	Totals60hz;
   gmt_DisplayOutputTotals 	Totals50hz;
   gmt_DisplayOutputTotals 	TotalsFilm;
} gmt_DisplayOutputFreq;
#endif

#ifdef __PARADIGM__
#pragma pack(push)
#pragma pack(1)
#endif

typedef struct
{
   // GAMMA_TABLE_22, GAMMA_TABLE_24, GAMMA_TABLE_26, GAMMA_TABLE_28, GAMMA_TABLE_CUSTOM
   BYTE 	B_ColorSpace;

   // CTEMP_4000K, CTEMP_5000K, CTEMP_6500K, CTEMP_7500K, CTEMP_8200K, CTEMP_9300K, CTEMP_10000K, CTEMP_11500K, CTEMP_CUSTOM
   BYTE 	B_ColorTemp;

   //an array of three pointers to gamma lut for each components: red, green, blue
   BYTE ROM *GammaLut1[3];

   BYTE ROM *GammaLut2[3];

   //pointer to 3x3 matrix values: read gain, green gain, blue gain, red offset, green offset, blue offset
   WORD ROM *Wp_3x3Values;

   BYTE B_GammaLUT1_SteepEnable;

   BYTE B_GammaLUT1_SteepStep;

   WORD W_GammaLUT1_SteepStart;

   WORD W_GammaLUT1_SteepEnd;

   WORD ROM *Wp_GammaLUT1_SteepFinal;

   WORD ROM *GammaLUT1Steep[3];

   BYTE B_GammaLUT2_SteepEnable;

   BYTE B_GammaLUT2_SteepStep;

   WORD W_GammaLUT2_SteepStart;

   WORD W_GammaLUT2_SteepEnd;

   WORD ROM *Wp_GammaLUT2_SteepFinal;

   WORD ROM *GammaLUT2Steep[3];
} gmt_GammaDescr;


typedef struct
{
   char ROM *			C_Name;
   gmd_MasterPanelList	ID;
   gmd_PanelType		Type;
   gmd_PanelDataFormat    Format;
   BYTE				B_PixPerClock;
   BYTE				B_Depth;
   WORD				W_Width;
   WORD				W_Height;
   BYTE				B_MaxVFreq;
   BYTE				B_MinVFreq;
   DWORD				DW_MaxPClk; // KHz
   WORD				W_MinHTotal;
   WORD				W_TypHTotal; // = MinHTotal + Offset
   WORD				W_22HTotal;
   WORD				W_33HTotal;
   BYTE				B_MinHSyncWidth;
   WORD				B_MinHSyncBackPorch;
   WORD				W_22VTotal;
   WORD				W_33VTotal;
   WORD				W_MaxVtotal;
   WORD				W_MinVtotal;
   WORD				W_TypVTotal;
   BYTE				B_MinVSyncWidth;
   BYTE				B_MinVSyncBackPorch;
   WORD				W_PowerUpTiming;
   WORD				W_PowerDownTiming;
   WORD				W_AccToPixelVal;
   WORD				W_SpreadSpectrumCtrl; // Value for Spread_Spectrum_Control register
   BYTE				B_DClkDelay; // DCLK display timing delay adjustment in ns unit.
   SWORD ROM * ROM *	Wa_OverDrvLut;
   WORD ROM *			Wp_SRGBTable; // Pointer to the sRGB table.
   gmt_GammaDescr ROM * ROM *Stp_GammaLut;

   WORD				W_Invert_DVS			: 1;
   WORD				W_Invert_DHS			: 1;
   WORD				W_Invert_DCLK			: 1;
   WORD				W_Invert_DEN			: 1;

   WORD				W_DportSwap				: 4;
   WORD				W_LVDSInterface			: 1;
   WORD				W_LVDS_BusType			: 2; // 0x00: single bus; 0x01: dual bus; 0x11: quad bus
   WORD				W_LvdsPanelMode			: 2;
   //WORD				LVDSBus_EvenOddSwap		: 1; // 0x01 - Swap 0x00 NoSwap
   WORD				W_Spread_Spect_En		: 1; // 0xFF : Enable spectrum, 0x00 : Disable 
   WORD				W_LvdsPosNegSwap		: 1; // 0x01 - No Swap, 0x00 NoSwap (default)
   WORD				W_PadDrive				: 2; // 0 - 1.25v, 1 - 1.10v, 2 - 1.35v, 3 - 1.25v
   WORD				W_LvdsCurrentCtrl		: 3; // 0 - 0.5ma, 1 - 1.0ma, 2 - 1.5ma, 3 - 2.0ma, 4 - 2.5ma, 5 - 3.0ma, 6 - 3.5ma, 7 - 4.0ma
   WORD				W_BitOutput				: 1; // 0 - 24/48, 1 - 30bit output
   WORD				W_LVDSSignalMap			: 2; // 0 - Standard 1, 1 - Standard 2, 3 - Custom
// WORD				W_PanelPixPerClk		: 1; // 0 - Single, 1 - Dual .
// WORD				PANEL_RF_Support_type	: 2; // 0x00 - 50 Hz only, 0x01- 60 Hz Only,
   // 0x10 - 50 or 60 HZ By DCLK variation
   // 0x11 - 50 or 60 HZ By TOTAL variation
   WORD				W_LvdsClkData;				 // register LVDS_CLK_DATA programming (0x0063 default)
}gmt_PanelData;
#ifdef __PARADIGM__
	#pragma pack(pop)
#endif


#if 1//(NON_SCALING_OUTPUT==1) || defined(ST_4K2K_93xx_BOARD)
extern gmt_PanelData  CurPanelData;
extern gmt_PanelData far * CurrentPanelData;
#else
extern gmt_PanelData  CurPanelData;
extern gmt_PanelData ROM * CurrentPanelData;
#endif

extern ROM gmt_PanelData * ROM PanelArray[];


#define PanelName				CurrentPanelData->C_Name
#define PanelID					CurrentPanelData->ID
#define PanelType				CurrentPanelData->Type
#define PanelDataFormat 		CurrentPanelData->Format
#define PanelPixPerClk			CurrentPanelData->B_PixPerClock
#define PanelDepth				CurrentPanelData->B_Depth
#define PanelWidth				CurrentPanelData->W_Width
#define PanelHeight				CurrentPanelData->W_Height
#define PanelMaxVFreq			CurrentPanelData->B_MaxVFreq
#define PanelMinVFreq			CurrentPanelData->B_MinVFreq
#define PanelMaxPClk			CurrentPanelData->DW_MaxPClk
#define PanelMinHTotal			CurrentPanelData->W_MinHTotal
#define PanelTypHTotal			CurrentPanelData->W_TypHTotal
#define Panel22HTotal			CurrentPanelData->W_22HTotal
#define Panel33HTotal			CurrentPanelData->W_33HTotal
#define PanelMinHSyncWidth		CurrentPanelData->B_MinHSyncWidth
#define PanelMinHSyncBackPorch	CurrentPanelData->B_MinHSyncBackPorch
//#define PanelHActiveStart		(PanelMinHSyncWidth + PanelMinHSyncBackPorch)
#define Panel22VTotal			CurrentPanelData->W_22VTotal
#define Panel33VTotal			CurrentPanelData->W_33VTotal
#define PanelMaxVTotal			CurrentPanelData->W_MaxVtotal
#define PanelTypVTotal			CurrentPanelData->W_TypVTotal
//#define PanelMinVTotal			CurrentPanelData->W_MinVtotal
#define PanelMinVSyncWidth		CurrentPanelData->B_MinVSyncWidth
#define PanelMinVSyncBackPorch	CurrentPanelData->B_MinVSyncBackPorch
//#define PanelVActiveStart		(PanelMinVSyncWidth + PanelMinVSyncBackPorch)
#define PanelPwrUpTiming		CurrentPanelData->W_PowerUpTiming
#define PanelPwrDnTiming		CurrentPanelData->W_PowerDownTiming
#define AccTotalPixel			CurrentPanelData->W_AccToPixelVal
#define Panel_Spread_Spect_En	CurrentPanelData->W_Spread_Spect_En
#define PanelSpreadSpectrumCtrl	CurrentPanelData->W_SpreadSpectrumCtrl
#define PanelOverDrvLut			CurrentPanelData->Wa_OverDrvLut
#define PanelGammaLUT			CurrentPanelData->Stp_GammaLut
//#define PanelDClkDelay			CurrentPanelData->B_DClkDelay
#define Panel_Invert_DVS		CurrentPanelData->W_Invert_DVS
#define Panel_Invert_DHS		CurrentPanelData->W_Invert_DHS
#define Panel_Invert_DCLK		CurrentPanelData->W_Invert_DCLK
#define Panel_Invert_DEN		CurrentPanelData->W_Invert_DEN
#define PanelBitOutput			CurrentPanelData->W_BitOutput
//#define PanelSRGB2Table			CurrentPanelData->Wp_SRGBTable

// LVDS related
#define DportSwap				CurrentPanelData->W_DportSwap
#define LvdsInterface			CurrentPanelData->W_LVDSInterface
#define LvdsBusType				CurrentPanelData->W_LVDS_BusType
//#define LvdsPanelMode			CurrentPanelData->W_LvdsPanelMode
#define LvdsPosNegSwap			CurrentPanelData->W_LvdsPosNegSwap
#define PanelPadDrive			CurrentPanelData->W_PadDrive
#define LvdsCurrentCtrl			CurrentPanelData->W_LvdsCurrentCtrl
#define LVDSSignalMap			CurrentPanelData->W_LVDSSignalMap
#define LvdsClkData				CurrentPanelData->W_LvdsClkData



//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************

//******************************************************************************
//  E X T E R N A L    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
void DisplaySetTiming (void);
void ProgramPanelOutput (void);
void PanelPwrOn(void);
void PanelPwrOff(void);
void UpdateCurrentPanelData(gmt_PHY_CH B_Channel);
void ClearOdpTiming(void);
#ifdef ST_4K2K_93xx_BOARD

void PanelControl_MasterHandler(void);
void PanelControl_SlaveHandler(void);
void PanelPowerOn4k2k(void);
void PanelPowerOff4k2k(void);
void SetPanelControlStatus(BYTE b_Status);
void gm_DisplayOnOff_4Kx2K(BOOL Bt_OnOff);
void ChangeModeHandler(void);
void ImageSync_RL(void);
void ImageSync_HStart(void);
void ImageSync_FE(void);
void Image_Hstart_Patch(void);
#endif
void gm_4Kx2k_LVDS_Control(void);
#endif

//*********************************  END  **************************************
