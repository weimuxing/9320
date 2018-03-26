/*
$Workfile:   gm_InputConfig.c  $
$Revision: 1.36 $
$Date: 2013/01/09 03:52:20 $
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
// MODULE :gm_InputConfig.c
//
// USAGE  :This module contains Tables and defines used for Configuration of
//     front-End (AFE+DFE).
//
// NOTE   :This file is generated from the InputConfig tool ver.2.1 on,
//         Wed Apr 30 14:13:56 2008

//*******************************************************************************

//******************************************************************************
//  IMPORTANT : P L E A S E   D O   N O T   E D I T  T H I S  F I L E
//******************************************************************************

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************
extern void EXT_FUNC HdmiDviExtPortSetup_RD1_DDVI(void);
extern void EXT_FUNC HdmiDviExtPortSetup_RD1_HDMI1(void);
#ifdef DELL_U2410F//120816 Steven for all DELL model 2 DP input within MUX
extern void EXT_FUNC Dp1ExtPortSetup_ID0(void);
extern void EXT_FUNC Dp1ExtPortSetup_ID1(void);
#endif

BYTE ROM gmc_NumOfReg = NUM_OF_AFE_REGISTERS;

#if (FEATURE_HDMI == ENABLE)
gmt_HdmiDviProperties ROM gmc_HDMI_DVIProps[NUM_OF_HDMIDVI_INP] =
{
#if (DUAL_DVI_CONNECTOR_ENABLE == 1	)
   {AID_PHY12, DID_DVI_DUAL, HdmiDviExtPortSetup_RD1_DDVI,  "STDDVIRX_1", NULL_PTR, NULL_PTR},	//Dual DVI
#else
   {AID_PHY1,  DID_DVI0,     NULL_PTR,                      "STDVIRX_1",  NULL_PTR, NULL_PTR},  //DVI
#endif
#if defined(CMI_73XX_BOARD)//120510 ChunBao no external switch
   {AID_PHY2,	DID_HDMI,	  NULL_PTR, "STHDMIRX_1", NULL_PTR, NULL_PTR}, //HDMI1
#else
   {AID_PHY2,  DID_HDMI,     HdmiDviExtPortSetup_RD1_HDMI1, "STHDMIRX_1", NULL_PTR, NULL_PTR}, //HDMI1
#endif   
}; //end gmc_DVIProps 
#endif

#if (FEATURE_DISPLAY_PORT == ENABLE) && (NUM_OF_DP_INP != 0)
gmt_DpProperties ROM gmc_DPProps[NUM_OF_DP_INP] =
{
#ifdef MUX_TEST_BOARD
   {AID_PHY0, DID_DPRX0, TRUE, MUX_SELECTOR_ID0, Dp1ExtPortSetup_ID0},	   // DP1 - DpNormal
   {AID_PHY0, DID_DPRX0, TRUE, MUX_SELECTOR_ID1, Dp1ExtPortSetup_ID1},     // DP3 - DpMini
#else
   {AID_PHY0, DID_DPRX0, FALSE, 0, NULL_PTR},	// DP1
#endif
}; //end gmc_DPProps
#endif

#ifdef EXTERNAL_DECODER
gmt_DipProperties ROM gmc_DIPProps[] =
{
   {DIP_PORTA,DIP_8_BIT,DIP_LSB_MSB,DIP_BGR_UYV,DIP_IPCLK0,DIP_EMBEDDED_SYNC,{DIP_A1, NONE, NONE}},  //vpc3230--656
   {DIP_PORTA,DIP_16_BIT,DIP_LSB_MSB,DIP_RBG_VUY,DIP_IPCLK1,DIP_EMBEDDED_SYNC,{DIP_A1, NONE, NONE}}, //vpc3230--601
   {DIP_PORTA,DIP_24_BIT,DIP_LSB_MSB,DIP_RGB_VYU,DIP_IPCLK0,DIP_EMBEDDED_SYNC,{DIP_A1, NONE, NONE}}, //anx9021--444
   #ifdef TVP5160_DEV_ID
   {DIP_PORTA,DIP_20_BIT,DIP_LSB_MSB,DIP_RBG_VUY,DIP_IPCLK0,DIP_EXTERNAL_SYNC,{DIP_A1, NONE, NONE}}, //TVP5160 422	CVBS1  	V1
   {DIP_PORTA,DIP_20_BIT,DIP_LSB_MSB,DIP_RBG_VUY,DIP_IPCLK0,DIP_EXTERNAL_SYNC,{DIP_B2, NONE, NONE}}, //TVP5160 422	Comp2	V5
   {DIP_PORTA,DIP_20_BIT,DIP_LSB_MSB,DIP_RBG_VUY,DIP_IPCLK0,DIP_EXTERNAL_SYNC,{DIP_A3, NONE, NONE}}, //TVP5160 422	CVBS2 	V3
   #else
   {DIP_PORTA,DIP_24_BIT,DIP_LSB_MSB,DIP_BGR_UYV,DIP_IPCLK0,DIP_EMBEDDED_SYNC,{DIP_A1, NONE, NONE}}, //anx9021--422  
   #endif
}; //end gmc_DIPProps
#endif
 
//Input Configuration Table Initialization

gmt_InputConfigTable ROM gmvs_InputConfigTbl[NUM_MASTER_LIST] =
{
#ifdef EXTERNAL_DECODER
/* P0  */   {CVBS1, 0,DIPX,SIG_DIPX,EXTERNAL,DIP,(void*)&gmc_DIPProps[3]},
#else
/* P0  */   {NOT_USED, 0, NONE,            NONE,          NONE,     NONE,     NONE}, 
#endif
/* P1  */   {NOT_USED, 0, NONE,            NONE,          NONE,     NONE,     NONE},

#if(ON_BOARD_COMP == ENABLE)
/* P2  */   {COMP1,    3, COMPONENT,       SIG_COMPONENT, INTERNAL, AIP,      NONE},
#else
/* P2  */   {NOT_USED,    0, COMPONENT,       SIG_COMPONENT, INTERNAL, AIP,      NONE},
#endif
/* P3  */   {VGA1,     4, VGA,             SIG_VGA,       INTERNAL, AIP,      NONE},
#if (FEATURE_HDMI == ENABLE)
/* P4  */   {DVI1,     6, DVI,             SIG_DVI,       INTERNAL, DVI_PORT, (void*)&gmc_HDMI_DVIProps[0]},
#else
/* P4  */   {NOT_USED, 0, NONE,            NONE,          NONE,     NONE,     NONE},
#endif
/* P5  */   {NOT_USED, 0, NONE,            NONE,          NONE,     NONE,     NONE},
#ifdef EXTERNAL_DECODER
   	#if (DEFAULT_DIP_DAUGHTER_BOARD_OUT == VPC3230_656)
	/* P6  */   {DIP1,     6, DIPX,            SIG_DIPX,      EXTERNAL, DIP,      (void*)&gmc_DIPProps[0]},
   	#elif (DEFAULT_DIP_DAUGHTER_BOARD_OUT == VPC3230_601)
      /* P6  */   {DIP1,     6, DIPX,            SIG_DIPX,      EXTERNAL, DIP,      (void*)&gmc_DIPProps[1]},
	#elif ((DEFAULT_DIP_DAUGHTER_BOARD_OUT == ANX9021_RGB444) || (DEFAULT_DIP_DAUGHTER_BOARD_OUT == ANX9021_YCbCr444))
	/* P6  */   {DIP1,     6, DIPX,            SIG_DIPX,      EXTERNAL, DIP,      (void*)&gmc_DIPProps[2]},
   	#elif (DEFAULT_DIP_DAUGHTER_BOARD_OUT == ANX9021_YCbCr422)
      /* P6  */   {DIP1,     6, DIPX,            SIG_DIPX,      EXTERNAL, DIP,      (void*)&gmc_DIPProps[3]},
   	#elif (DEFAULT_DIP_DAUGHTER_BOARD_OUT == TVP5160_YCbCr422)
      /* P6  */   {DIP1,     6, DIPX,            SIG_DIPX,      EXTERNAL, DIP,      (void*)&gmc_DIPProps[3]},
   	#else
      /* P6  */   {NOT_USED, 0, NONE,            NONE,          NONE,     NONE,     NONE},
   	#endif
#else
/* P6  */   {NOT_USED, 0, NONE,            NONE,          NONE,     NONE,     NONE},
#endif
#if (FEATURE_HDMI == ENABLE)
/* P7  */   {HDMI1,    7, HDMI,            SIG_DVI,       INTERNAL, DVI_PORT, (void*) &gmc_HDMI_DVIProps[1]},
#else
/* P7  */   {NOT_USED, 0, NONE,            NONE,          NONE,     NONE,     NONE},
#endif
/* P8  */   {NOT_USED, 0, NONE,            NONE,          NONE,     NONE,     NONE},
/* P9  */   {NOT_USED, 0, NONE,            NONE,          NONE,     NONE,     NONE},
#if (FEATURE_DISPLAY_PORT == ENABLE)
/* P10 */   {DP1,      5, CT_DISPLAY_PORT, SIG_DP,        INTERNAL, DP_PORT,  (void*)&gmc_DPProps[0]},
#else
/* P10 */   {NOT_USED, 0, NONE,            NONE,          NONE,     NONE,     NONE},
#endif
/* P11 */   {NOT_USED, 0, NONE,            NONE,          NONE,     NONE,     NONE},
#if (FEATURE_DISPLAY_PORT == ENABLE)
#ifdef MUX_TEST_BOARD
/* P12 */   {DP3,      5, CT_DISPLAY_PORT, SIG_DP,        INTERNAL, DP_PORT,  (void*)&gmc_DPProps[1]},
#else
/* P12 */   {NOT_USED, 0, NONE,            NONE,          NONE,     NONE,     NONE},
#endif
#endif
#ifdef TVP5160_DEV_ID
/* P13  */   {COMP2,    13, DIPX,       SIG_DIPX, EXTERNAL, DIP,	(void*)&gmc_DIPProps[4]},
/* P14  */   {CVBS2, 	14, DIPX,		SIG_DIPX, EXTERNAL, DIP,	(void*)&gmc_DIPProps[5]},
#endif


/* P13 */   {NOT_USED, 0, NONE,            NONE,          NONE,     NONE,     NONE},
/* P14 */   {NOT_USED, 0, NONE,            NONE,          NONE,     NONE,     NONE},
};//End of gmvs_InputConfigTbl


//==============================================================================

//AFE Register Dump, Created based on the connections provided in the Spreadsheet
//The Following is the list of Registers whose values are calculated
//AFE Register List

WORD ROM gmvw_AFERegAddress[NUM_OF_AFE_REGISTERS] =
{
   AFEDIG_CTRL0,
// Check ADC A, B, C clamping later
   VIDEO_CLAMP_CONTROL_1,
   ADC_CONTROL1,
   AIP_HD_GFX_BUS_SWAP //16-Bit Register,Upper 8bits are Reserved
}; //End of gmvw_AFERegAddress Init


//The following are the Register Values for each connector

BYTE ROM gmvb_AFERegistersVals[NUM_MASTER_LIST * NUM_OF_AFE_REGISTERS] =
{
//			CTL0  CLMP1 ADC1  BUSSWP
/*NOT_USED*/     0x00, 0x00, 0x00, 0x00, /*16Bit Value, Upper 8Bits are Rsvd*/
/*NOT_USED*/     0x00, 0x00, 0x00, 0x00, /*16Bit Value, Upper 8Bits are Rsvd*/
#ifdef BOARD_93xx_TK
/*COMP1*/        0xF6, 0xEA, 0x80, 0x02, /*16Bit Value, Upper 8Bits are Rsvd*/
/*VGA1*/         0x11, 0x00, 0x80, 0x02, /*16Bit Value, Upper 8Bits are Rsvd*/
#else
/*COMP1*/        0xF6, 0xEA, 0x80, 0x02, /*16Bit Value, Upper 8Bits are Rsvd*/
/*VGA1*/         0x11, 0x00, 0x80, 0x04, /*16Bit Value, Upper 8Bits are Rsvd*/
#endif
/*NOT_USED*/ 		0x00, 0x00, 0x00, 0x00, /*16Bit Value, Upper 8Bits are Rsvd*/
/*NOT_USED*/ 		0x00, 0x00, 0x00, 0x00, /*16Bit Value, Upper 8Bits are Rsvd*/
/*NOT_USED*/ 		0x00, 0x00, 0x00, 0x00, /*16Bit Value, Upper 8Bits are Rsvd*/
/*NOT_USED*/ 		0x00, 0x00, 0x00, 0x00, /*16Bit Value, Upper 8Bits are Rsvd*/
/*NOT_USED*/ 		0x00, 0x00, 0x00, 0x00, /*16Bit Value, Upper 8Bits are Rsvd*/
/*NOT_USED*/ 		0x00, 0x00, 0x00, 0x00, /*16Bit Value, Upper 8Bits are Rsvd*/
/*NOT_USED*/ 		0x00, 0x00, 0x00, 0x00, /*16Bit Value, Upper 8Bits are Rsvd*/
 }; //End of gmvb_AFERegisters Init

BYTE ROM gmvb_AdcUsage[NUM_MASTER_LIST*3] =
{
   /*			 ADC_CONTROL0  AFE_DAC_CTRL0  AFE_DAC_CTRL1       */
   /*NOT_USED*/      0x00, 0x00,	0x00,
   /*NOT_USED*/      0x00, 0x00, 0x00,
   /*COMP1*/         0x00, 0x00,	0x07,
   /*VGA1*/          0x01, 0x00,	0x00,
   /*NOT_USED*/      0x00, 0x00,	0x00,
   /*NOT_USED*/      0x00, 0x00, 0x00,
   /*NOT_USED*/      0x00, 0x00, 0x00,
   /*NOT_USED*/      0x00, 0x00, 0x00,
   /*NOT_USED*/      0x00, 0x00, 0x00,
   /*NOT_USED*/      0x00, 0x00, 0x00,
   /*NOT_USED*/      0x00, 0x00, 0x00,
   /*NOT_USED*/      0x00, 0x00, 0x00,
}; //End of gmv_AdcUsage Init


//*** Auto Scan Input configuration*********************************************
#ifdef AUTO_SCAN_INPUT
//Autoscan is a configuration ability to group several video input ports in one "ring".
//For example, project contains 3 rings with names "Video 1", Video 2", "DVD" and user can
//		switch between them by pressing "input select" key on remote control
//		"Video 1" ring combines "SVideo 1" and "CVBS 1" input ports and firmware "automatically"
//		select inputs which has Sync sygnal.
//
//Following ROM structure is responsible for Autoscan rings configuration.
//It is one-dimensional array, but actually it represents several "rings", which are sets of IDs
//	(ALL_BOARDS_INPUT_PORT type enumerators) and every ring must contain
//		- at least one port ID
//		- one "NOT_USED" enumerator in the end as a terminator of current ring set.
//
//For example:
//		"...ALL_BOARDS_CVBS1, ALL_BOARDS_SVIDEO1, NOT_USED,..." //defines ring with 2 inputs CVBS1 and SVIDEO1
//		"...ALL_BOARDS_COMP1, NOT_USED,..." //defines ring with 1 input: COMPONENT1.
//
//First input port in the ring is default port, i.e. it will be first checked on Sync presence after
//NVRAM reset (or after user change input ring if AUTO_SCAN_INPUT_NVRAM is not defined).
//
//AUTOSCANINPUT_RINGS_SIZE define should be explicitly declared, because of compiler specifics
//it is equal to sizeof(ASInputRings).
//
//AUTOSCANINPUT_NUM_RINGS is equal to number of rings in ASInputRings array (not enums!)
BYTE /*ALL_BOARDS_INPUT_PORT*/ ROM ASInputRings[AUTOSCANINPUT_RINGS_SIZE] = //BYTE is used for code space saving
{
   ALL_BOARDS_VGA1,
#if (FEATURE_HDMI == ENABLE)
   ALL_BOARDS_DVI1,
   ALL_BOARDS_HDMI,
#endif
#if (FEATURE_DISPLAY_PORT == ENABLE)
   ALL_BOARDS_DP1,
   ALL_BOARDS_DP3,
#endif   
   NOT_USED
	};
		

#endif 
//*********************************  END  **************************************
