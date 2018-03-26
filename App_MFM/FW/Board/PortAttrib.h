/*
	$Workfile:   PortAttrib.h  $
	$Revision: 1.8 $
	$Date: 2013/01/18 05:48:06 $
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
// MODULE:    PortAttrib.h
//
// USAGE:      Definition of the application port attributes
//
//******************************************************************************

#ifndef PORTATTRIB_H
#define	PORTATTRIB_H

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************

// Type of input measurement
typedef enum
{
   gmt_DecoderStatus,			// decoder status,
   gmt_IFMmeasurement,		// ifm measurement,
   gmt_IBDmeasurement,		// use also ibd measurement (along with ifm measurement)
   gmt_ExternalStatus,			// external status, // to be able to extend with future status
   gmt_DPmeasurement, //Display port measurement
   gmt_HdmiDvimeasurement,
}gmt_InputMeasurementType;

typedef enum
{
   gmd_RGBColorSpaceRangeFullExpand,			// 0-255
   gmd_YUVColorSpaceLiniarExpand,				// 0-219
   gmd_YUVColorSpaceWithBlackOffset,			// 16-235
   gmd_RGBColorSpaceWithPartialExpand,		// 0-235
}gmt_ColorSpaceRange;

typedef struct
{
   gmt_DATA_WIDTH B_DataWidth;				// input 444, 422
   gmt_COLOR_SPACE B_ColorSpace;				// color space
   BOOL B_ColorSpaceConversion;				// gmd_RGB_TO_YUV
   gmt_ColorSpaceRange B_ColorSpaceRange;
}InputColorSpace;

// Power handler policy regarding the input status
typedef enum
{
   gmd_PowerNoChannel,
   gmd_PowerMain,
   gmd_PowerPip,
   gmd_PowerGraphics,
   gmd_PowerVideo,
   gmd_PowerAllChannel,
   gmd_PowerAnyChannel,
   gmd_PowerExternal,
} gmt_PowerPolicy;

// Power handler policy regarding the actions
typedef enum
{
   gmd_GraphicsPowerSaving	= BIT0,
   gmd_VideoPowerSaving		= BIT1,
   gmd_TimeBased				= BIT2,
   gmd_PowerKey				= BIT3
}ActionPowerPolicy;


/*
//
typedef enum
{
	gmd_VideoMux,
	gmd_VideoDecoder,
	gmd_Slave					// more responsabilities required for Slave
}gmt_ExtDeviceType ;			// we need to know if there is anything outside that needs to be controlled, the control will be different

// application definition
typedef enum
{
	gmd_SIPC,
	gmd_DDCCI,
	gmd_GPROBE,					// more responsabilities required for Slave
}gmt_ProtocolUsed;

// application definition
typedef enum
{
	gmd_2Wire,
	gmd_3Wire,
	gmd_UART,
}gmt_PhyLinkType;

// external device definition
typedef struct
{
	gmt_ExtDeviceType B_ExtDeviceType;			// selection between video mux, slave, video decoder
	gmt_PhyLinkType B_PhyLinkType;
	gmt_ProtocolUsed B_ProtocolUsed;			//
	BYTE ROM * B_CommCharacteristics;			// example for i2c: device address, ring, speed,
}
ExtDeviceLink;
*/


// Structure for port attributes
typedef struct
{
   // Port selection usage detection attributes:
   gmt_DEVICE_ID B_DeviceID;						// detection measurement type
   gmt_DET_STATE B_DetectionConfig;				// detection state configuration
   gmt_SYNC_TYPE Ba_Sync[MAX_SYNC_TYPE];			// list of sync types
   BYTE B_StableCount;								// count until stable
   BYTE B_UnstableCount;							// count until unstable
   gmt_IBD_SOURCE_TYPE B_IbdSourceType;			// IBD measurement type
   gmt_VIDEO_DECODER_MODE_SELECT B_VideoModes;	// video standards supported
   gmt_AutoMode B_AutoMode;						// auto display muting
   gmt_SYNC_FLAGS B_SyncFlags;					// input capture sync alignment (ex: internal or external odd signal)

   // External device usage & other extensions to configure different external devices:
   void (far *const ConfigureExtDevices)(gmt_PHY_CH B_Channel, gmt_UserDefConnNames B_InputConnector);

   // Mode setup usage:
   // This configuration could depend also on 2 other parameters: the physical input channel and the mode determination
   // This option is the default one that could be changed later on by mode table
   InputColorSpace ROM * B_InputColorSpace;

   // Mode determination usage:
   // Note: if there is set the null pointer than there is no need for mode determination process
   gmt_InputMeasurementType B_InputMeasurementType;	// type of input measurement need (ifm, decoder status, ibd)
   StandardModeStruct ROM * ROM * LocalModeTable; 		// mode table entries
   StandardModeStruct ROM * ROM * RemoteModeTable; 		// alternative (ex. main selector for Cortez & pip channel for Hudson)

   // Power handler:
   gmt_PowerPolicy B_PowerPolicy;					// power handler policy for input status
   ActionPowerPolicy B_ActionPowerPolicy;				// power handler policy for actions

   // Audio usage:
   BYTE ROM * AudioEntry; 							// place holder till is defined
}PortAttrib;

//110802 Edward disable , the below setting already define in other file
//#define	IsGraphicInput(x)               ((x)==ALL_BOARDS_VGA1 || (x)==ALL_BOARDS_HDMI || (x)==ALL_BOARDS_HDMI2 || (x)==ALL_BOARDS_HDMI3 ||(x)==ALL_BOARDS_DVI1 ||(x)==ALL_BOARDS_DVI2 || (x)==ALL_BOARDS_DP1 ||(x)==ALL_BOARDS_DP2) // HDMI2 is DVI, and set DP as Graphic Input
//#define	IsVideoInput(x)                 ((x)==ALL_BOARDS_CVBS1 || (x)==ALL_BOARDS_SVIDEO1 || (x)==ALL_BOARDS_COMP1 || (x)==ALL_BOARDS_DIP1)

//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************

extern PortAttrib ROM AppPortAttrib[NUM_MASTER_LIST];

#endif
//*********************************  END  **************************************
