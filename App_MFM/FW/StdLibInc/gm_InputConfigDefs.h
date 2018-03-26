/*
    $Workfile:   gm_InputConfigDefs.h  $
    $Revision: 1.19 $
    $Date: 2012/08/20 03:43:22 $
*/
#ifndef __GM_INPUTCONFIGDEFS_H__
#define __GM_INPUTCONFIGDEFS_H__
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
// MODULE :gm_InputConfigDefs.h
//
// USAGE  :This module contains Typedefs and defines used for Configuration of
//         Front-End (AFE+DFE).
//
// NOTE   :This file is generated from the spreadsheet Cortez_InputConfig.xls on,
//         9/30/2004 at 12:15:31 PM
//*******************************************************************************

//#include "gm_ChipFeature.h"				// Need to be after gm_global.h

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************

#define NUM_OF_VIDEO_REGISTERS 25
#define NONE 0xFF
#define VLUT_SIZE 2048

typedef enum CONNECTOR_TYPES
{
   //Connector Types
   COMPOSITE,
   TUNER,
   SVIDEO,
   COMPONENT,
   VGA,
   DVI,
   HDMI,
   SCART,
   SCARTT2,
   OVERLAY_SCART,
   SLAVE,
   CT_DISPLAY_PORT,
   DIPX,
   NUM_CONNECTOR_TYPES
}gmt_ConnTypes; //End of Connector Types

typedef enum SIGNAL_TYPE
{
   //SIGNAL_TYPE
   SIG_COMPOSITE,
   SIG_TUNER_COMPOSITE,
   SIG_TUNER_SVIDEO,
   SIG_SVIDEO,
   SIG_COMPONENT,
   SIG_VGA,
   SIG_DVI,
   SIG_OVL_TYPE1,
   SIG_OVL_TYPE2,
   SIG_OVL_TYPE3,
   SIG_DP,
   SIG_DIPX,
}gmt_AipSignalType; //End of Signal Types
// Input Processing Type
typedef enum INPUT_PROCESSING_TYPE
{
   //Input Processing
   INTERNAL,
   EXTERNAL,
}gmt_InputProcessing; //End of Input Processing Type

// Input Processing Port
typedef enum INPUT_PROCESSING_PORT
{
   AIP,
   DIP,
   DVI_PORT,
   DP_PORT,
}gmt_InputProcessingPort; //End of Input Processing Port

typedef enum
{
   ADC_A = BIT0,        //Bit =1 (ADC used); Bit=0 (ADC unused)
   ADC_B = BIT1,
   ADC_C = BIT2,
   ADC_SYNC = BIT3,
}gmt_ADC_USED;


//Enums for Pin List
typedef enum CORTEZ_INPUT_PINS
{
   //Analog Input Pins,Ports
   AIP_A1,
   AIP_A2,
   AIP_A3,
   AIP_A4,
   AIP_B1,
   AIP_B2,
   AIP_B3,
   AIP_B4,
   AIP_C1,
   AIP_C2,
   AIP_C3,
   AIP_C4,
   AIP_SV1,
   AIP_SV2,
   AIP_SV3,
   AIP_SV4,
   AIP_SIF1,
   AIP_SIF2,
   AIP_AHSP,
   AIP_BHSP,
   AIP_CHSP,
   AIP_NONE
}gmt_AipPins; //End of Analog Pins

typedef struct
{
   gmt_ADC_USED B_AdcUsed;
   gmt_AipPins  B_PinsUsed[5];
}gmt_AipProperties;

/*
typedef enum  //HDMI MUX inputs
{
 HDMI_NOT_USED = 0,	// for backward compatibilty
 HDMI_MUX_IN1,
 HDMI_MUX_IN2
}gmt_HdmiMuxInput;

typedef struct
{
  gmt_HdmiMuxInput HdmiInput;
}gmt_DviProperties;
*/

typedef enum HDMI_DVI_DID
{
   DID_DVI0=0,            // Digital DVI0 PHY
   DID_DVI1,              // Digital DVI1 PHY
   DID_DVI_DUAL,          // Digital DVI PHY 0 and 1 for DVI Dual
   DID_HDMI,              // Digital HDMI PHY
   DID_HDMI_DVI_END,
   DID_HDMI_DVI_NONE=DID_HDMI_DVI_END,
}gmt_HDMI_DVI_DID;

typedef enum DPRX_DID
{
   DID_DPRX0=0,				// Digital DPRX0
   DID_DPRX1,					// Digital DPRX1
   DID_DPRX_END,
   DID_DPRX_NONE=DID_DPRX_END,
}gmt_DPRX_DID;


typedef enum COMBOPHY_AID
{
   AID_PHY0=0,					// Analog Combo PHY0
   AID_PHY1,					// Analog Combo PHY1
   AID_PHY2,					// Analog Combo PHY2
   AID_PHY12,					// For Dual DVI
   AID_PHY_END,
   AID_PHY_NONE=AID_PHY_END,
}gmt_COMBOPHY_AID;


enum
{
   MUX_SELECTOR_ID0=0,         
   MUX_SELECTOR_ID1,           
   MUX_SELECTOR_ID2,           
   MUX_SELECTOR_ID3,           
   MUX_SELECTOR_ID4,           
   MUX_SELECTOR_ID5,           
   MUX_SELECTOR_ID6,           
   MUX_SELECTOR_ID7,
};


typedef struct
{
	gmt_COMBOPHY_AID 	B_AID;
	gmt_DPRX_DID		B_DID;
	BYTE              B_Mux_Flag;       //TRUE if ext Mux is available
	BYTE              B_Mux_SelectorID; //SelectorID of Mux Selection
	void (EXT_FUNC* ExtPortSetup)(void);
}gmt_DpProperties;




#define ST_MAX_DEVICE_NAME 16  /* 15 characters plus '\0' */
typedef char ST_DeviceName_t[ST_MAX_DEVICE_NAME];

typedef struct
{
   gmt_COMBOPHY_AID 	B_AID;
   gmt_HDMI_DVI_DID	B_DID;
   void (EXT_FUNC* ExtPortSetup)(void);
   ST_DeviceName_t		DeviceName;
   void (EXT_FUNC* CustomSetHPD)(void);
   void (EXT_FUNC* CustomClearHPD)(void);   
//	char DeviceName[ST_MAX_DEVICE_NAME];
}gmt_HdmiDviProperties;


//The Following table illustrates the use of the B_PinList array
//This is the definition of the Pin list when Analog Input Port(AIP) is used
//----------------------------------------------------------|
//S.NO  | CONNECTOR | PIN1   | PIN2  | PIN3 | PIN4  | PIN5  |
//------|-----------| -------|-------|------|-------| ------|
// 1.   |  CVBS     | Y/CVBS | -NA-  | -NA- | -NA-  | -NA-  |
// 2.   |  RF       | Y/CVBS | -NA-  | -NA- | -NA-  | -NA-  |
// 3.   |  SVIDEO   | Y/CVBS | CHROMA| -NA- | -NA-  | -NA-  |
// 4.   |  COMPONENT| Y/CVBS | Pr    | Pb   | -NA-  | -NA-  |
// 5.   |  VGA      | R      | G     | B    | -NA-  | -NA-  |
// 6.   |  SCART    | Y/CVBS | R     | G    | B     | FB    |
//----------------------------------------------------------|
typedef enum DIP_PORT
{
   DIP_PORTA,
   DIP_PORTB,
}gmt_DipPort;

typedef enum  //DIP bus flip
{
   DIP_LSB_MSB,  //(0-7)
   DIP_MSB_LSB,  //(7-0)
}gmt_DipBusFlip;

typedef enum  //DIP bus swap
{
   DIP_RGB_VYU,
   DIP_RBG_VUY,
   DIP_GRB_YVU,
   DIP_GBR_YUV,
   DIP_BRG_UVY,
   DIP_BGR_UYV,
}gmt_DipBusSwap;


typedef enum
{
   //Input Clock
   DIP_IPCLK0 = 6,
   DIP_IPCLK1,
   DIP_IPCLK2,
   DIP_IPCLK3,
}gmt_DipClock;

typedef enum
{
   DIP_EMBEDDED_SYNC,
   DIP_EXTERNAL_SYNC,
}gmt_DipSignalType;


//Enums for Pin List
typedef enum INPUT_DIGITAL_PINS
{
   //Analog Input Pins,Ports
   DIP_A1,
   DIP_A2,
   DIP_A3,
   DIP_B1,
   DIP_B2,
   DIP_B3,
   DIP_NONE
}gmt_DipPins; //End of digital Pins

//Enums for type of configuration
typedef  enum DIP_PIN_CONFIG
{
   PORT_CONF_0 = 0x0000,
   PORT_CONF_1 = 0x0100,
   PORT_CONF_2 = 0x0200 ,
   PORT_CONF_3 = 0x0300,
   PORT_CONF_4 = 0x0400,
   PORT_CONF_5 = 0x0500,
   //PORT_CONF_NONE
}gmt_DipPinConfig;


typedef enum DIP_BUS_WIDTH
{
   DIP_8_BIT,
   DIP_16_BIT,
   DIP_24_BIT,
   DIP_10_BIT,
   DIP_20_BIT,
   DIP_30_BIT
}gmt_DipBusWidth;

typedef  enum DIP_BUS_PROPS
{
   DIP_8_BIT_CONF0  = PORT_CONF_0 | DIP_8_BIT,
   DIP_16_BIT_CONF0 = PORT_CONF_0 | DIP_16_BIT,
   DIP_24_BIT_CONF0 = PORT_CONF_0 | DIP_24_BIT,
   DIP_30_BIT_CONF1 = PORT_CONF_1 | DIP_30_BIT,

   DIP_10_BIT_CONF2 = PORT_CONF_2 | DIP_10_BIT,
   DIP_10_BIT_CONF3 = PORT_CONF_3 | DIP_10_BIT,
   DIP_10_BIT_CONF4 = PORT_CONF_4 | DIP_10_BIT,
   DIP_10_BIT_CONF5 = PORT_CONF_5 | DIP_10_BIT,

   DIP_20_BIT_CONF2 = PORT_CONF_2 | DIP_20_BIT,
   DIP_20_BIT_CONF3 = PORT_CONF_3 | DIP_20_BIT,
   DIP_20_BIT_CONF4 = PORT_CONF_4 | DIP_20_BIT,
   DIP_20_BIT_CONF5 = PORT_CONF_5 | DIP_20_BIT,
}gmt_DipBusProps;


typedef struct
{
   gmt_DipPort B_Port;
   gmt_DipBusProps B_DipBusProps;
   gmt_DipBusFlip B_BusFlip;
   gmt_DipBusSwap B_RGBSwap;
   gmt_DipClock B_DipClock;
   gmt_DipSignalType  B_SynclType;
   gmt_DipPins B_DipPins[3];
   gmt_DipPinConfig B_DipPinConfig;
}gmt_DipProperties;

//This is the definition of the Pin list when DIGITA Input Port(DIP) is used
//------------------------------------------------------------------|
//S.NO  | CONNECTOR | PIN1   | PIN2     | PIN3      | PIN4  | PIN5  |
//------|-----------| -------|----------|-----------|-------| ------|
// 1.   |  CVBS     | DIP_Y  | -NA-     | -NA-      | -NA-  | -NA-  |
// 2.   |  RF       | DIP_Y  | -NA-     | -NA-      | -NA-  | -NA-  |
// 3.   |  SVIDEO   | DIP_Y  | DIP_C(*) | -NA-      | -NA-  | -NA-  |
// 4.   |  COMPONENT| DIP_Y  | DIP_PR(*)| DIP_PB(*) | -NA-  | -NA-  |
// 5.   |  VGA      | DIP_R  | DIP_G    | DIP_B     | -NA-  | -NA-  |
//------------------------------------------------------------------|
//Input Configuration Table Declaration
extern enum  gmt_UserDefConnNames;
typedef struct INPUT_CONFIG_TABLE
{
   enum gmt_UserDefConnNames B_ConnName;        //Connector Name
   BYTE B_ConnectorNumber;        //Connector number
   gmt_ConnTypes B_ConnType;        //Connector Type
   gmt_AipSignalType B_SignalType;
   gmt_InputProcessing B_InputProcessing; //Input Processing Type
   gmt_InputProcessingPort B_InputProcessingPort;
   ROM * PropertyTable;
}gmt_InputConfigTable;

// Audio inputs table descriptor
typedef enum
{
   RF_MODULATED_AUDIO,
   BASEBAND_AUDIO,
   SPDIF_AUDIO,
   I2S_AUDIO,
   HDMI_AUDIO,
   DP_AUDIO,
   AUDIO_NOT_USED
}gmt_AudioInputType;

typedef enum
{
   BASEBAND_MUX_IN1,
   BASEBAND_MUX_IN2,
   BASEBAND_MUX_IN3,
   BASEBAND_MUX_IN4,
   BASEBAND_MUX_IN5,
   BASEBAND_MUX_MONO,
   SIF1,
   SIF2,
   SPDIF_IN,
   I2S_IN1,
   I2S_IN2,
   HDMI_IN,
   DP_IN,
   INPUT_NOT_USED
}gmt_AudioInput;

typedef enum
{
   EXT_MUX_IN0,
   EXT_MUX_IN1,
   EXT_MUX_IN2,
   EXT_MUX_IN3,
   EXT_MUX_NOT_USED
}gmt_AudioInputExtMux;
#pragma option -b. //as a byte

typedef struct
{
   gmt_AudioInputType B_AudioInputType;
   gmt_AudioInput AudioInput;
}gmt_AudioInputsTable;

#pragma option -b- //as a byte
typedef enum
{
   IMP_VLUT_NOT_USED,
   IMP_VLUT_RGB,
   IMP_VLUT_YUV,
   IMP_VLUT_AUTO
}gmt_IMP_VLUT_Type;
#pragma option -b. //as a byte

#endif // __GM_INPUTCONFIGDEFS_H__
//*********************************  END  **************************************
