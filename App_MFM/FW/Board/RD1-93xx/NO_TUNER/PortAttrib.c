/*
   $Workfile:   PortAttrib.c  $
   $Revision: 1.28 $
   $Date: 2013/02/26 09:43:36 $
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
// MODULE:    PortAttrib.c
//
// USAGE:      Table of the application port attributes for each port in the master port list.
//                  Note that not all attributes may be used in the firmware. Any attribute that
//                  isn't used is noted with (DUMMY).
//
//******************************************************************************


//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "System\All.h"
#include "Board\PortAttrib.h"

//*****************************************************************************
//  E X T E R N A L    F U N C T I O N    P R O T O T Y P E S
//*****************************************************************************
extern void SelectInputExternal(gmt_PHY_CH, gmt_UserDefConnNames);
extern void ConfigureExtDevices(gmt_PHY_CH B_Channel,
                                   gmt_UserDefConnNames B_InputConnector);


//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************
#if FEATURE_AIP == ENABLE
StandardModeStruct ROM * ROM LocalCompModeTable[] = {CompHD_ModeTable, StandardModeTable_60, StandardModeTable_OOR, 0};
StandardModeStruct ROM * ROM SlaveCompModeTable[] = {CompHD_ModeTable, 0};

StandardModeStruct ROM * ROM LocalVGAModeTable_SOG[] = {CompHD_ModeTable, StandardModeTable_60, 0};

StandardModeStruct ROM * ROM LocalVGAModeTable_60[] = {StandardModeTable_60, CompHD_ModeTable, StandardModeTable_OOR, 0};
StandardModeStruct ROM * ROM LocalVGAModeTable_75[] = {StandardModeTable_70, StandardModeTable_OOR, 0};
StandardModeStruct ROM * ROM LocalVGAModeTable_85[] = {StandardModeTable_85, StandardModeTable_OOR, 0};

#else // FEATURE_AIP == DISABLE
StandardModeStruct ROM * ROM LocalCompModeTable[] = {0};
StandardModeStruct ROM * ROM SlaveCompModeTable[] = {0};
#endif // FEATURE_AIP

#ifdef EXTERNAL_DECODER
StandardModeStruct ROM * ROM LocalIntDecoderModeTable[] = {IntDecoder_ModeTable, 0};
StandardModeStruct ROM * ROM SlaveIntDecoderModeTable[] = {IntDecoder_ModeTable, 0};

StandardModeStruct ROM * ROM LocalExtDecoderModeTable[] = {ExtDecoder_ModeTable, 0};
StandardModeStruct ROM * ROM SlaveExtDecoderModeTable[] = {ExtDecoder_ModeTable, 0};
#else
StandardModeStruct ROM * ROM LocalIntDecoderModeTable[] = {0};
StandardModeStruct ROM * ROM SlaveIntDecoderModeTable[] = {0};

StandardModeStruct ROM * ROM LocalExtDecoderModeTable[] = {0};
StandardModeStruct ROM * ROM SlaveExtDecoderModeTable[] = {0};
#endif


// Master list of port attributes:
PortAttrib ROM AppPortAttrib[NUM_MASTER_LIST] =
{
   //ALL_BOARDS_CVBS1 // 0		//TVP5160 CVBS 
   #ifdef EXTERNAL_DECODER
	{
			DEV_EXTERN,							// detection measurement type
			DET_RESTART,						// detection state configuration
		{
			NO_SYNC_STATE,
			NO_SYNC_STATE,
			NO_SYNC_STATE,
			NO_SYNC_STATE
		},								// list of sync types (DUMMY)
	
		#if (SPEEDUP_MODE_CHANGE == ENABLE)
		6,									// count until stable
		4,                         // count until unstable
		#else
		6,									// count until stable
		4,                         // count until unstable
		#endif
		gmd_DATA,							// IBD measurement type
		gmd_ALL_MODES_SELECT,				// video standards supported
		AUTO_MUTE_ENABLE,             // auto display muting
	#ifdef EXTERNAL_DECODER
		gmd_SYNC_EVEN_OFFSET,//|gmd_SYNC_ODD_TYPE_EXT,		// input capture sync alignment
	#else
		gmd_SYNC_ODD_TYPE_EXT,
	#endif
		ConfigureExtDevices,				// usage & extensions to configure external devices
		0,                         // input color space (DUMMY)
		gmt_ExternalStatus,									// type of input measurement needed (DUMMY)
		LocalExtDecoderModeTable,			// mode table for selection
		SlaveExtDecoderModeTable,			// alternative selection
		0,									// power handler policy regarding the input status (DUMMY)
		0,									// power handler policy regarding the actions (DUMMY)
		0,									// audio usage */
	},
   #else
   {
      DEV_DECODER,                  // detection measurement type
      DET_RESTART,                  // detection state configuration
      {0},                       // list of sync types (DUMMY)
      6,                         // count until stable
      4,                         // count until unstable
      gmd_DATA,                     // IBD measurement type
      gmd_ALL_MODES_SELECT,            // video standards supported
      AUTO_MUTE_DISABLE,               // auto display muting
      (gmd_SYNC_ODD_INV | gmd_SYNC_ODD_TYPE_EXT),              // input capture sync alignment
      SelectInputExternal,       // usage & extensions to configure external devices (DUMMY)
      0,                         // input color space (DUMMY)
      0,                         // type of input measurement needed (DUMMY)
      LocalIntDecoderModeTable,        // mode table for selection
      SlaveIntDecoderModeTable,        // alternative selection
      0,                         // power handler policy regarding the input status (DUMMY)
      0,                         // power handler policy regarding the actions (DUMMY)
      0,                         // audio usage
   },
   #endif

   //ALL_BOARDS_SVIDEO1 // 1
   {
      DEV_DECODER,                  // detection measurement type
      DET_RESTART,                  // detection state configuration
      {0},                       // list of sync types (DUMMY)
      2,                         // count until stable
      1,                         // count until unstable
      gmd_DATA,                     // IBD measurement type
      gmd_ALL_MODES_SELECT,            // video standards supported
      AUTO_MUTE_DISABLE,               // auto display muting
      (gmd_SYNC_ODD_INV | gmd_SYNC_ODD_TYPE_EXT),              // input capture sync alignment
      SelectInputExternal,       // usage & extensions to configure external devices (DUMMY)
      0,                         // input color space (DUMMY)
      0,                         // type of input measurement needed (DUMMY)
      LocalIntDecoderModeTable,        // mode table for selection
      SlaveIntDecoderModeTable,        // alternative selection
      0,                         // power handler policy regarding the input status (DUMMY)
      0,                         // power handler policy regarding the actions (DUMMY)
      0,                         // audio usage
   },
   
   //ALL_BOARDS_COMP1 // 2	// On board Component
   {
      DEV_IFM,                   // detection measurement type
      DET_RESTART,                  // detection state configuration
      {
         SOG_STATE,                 // SOG state
         NO_SYNC_STATE,
         NO_SYNC_STATE,
         NO_SYNC_STATE
      },                         // list of sync used
      #if (SPEEDUP_MODE_CHANGE == ENABLE)
      2,                         // count until stable
      2,                         // count until unstable
      #else
      3,                         // count until stable
      2,                         // count until unstable
      #endif
      gmd_DATA,                     // IBD measurement type
      gmd_ALL_MODES_SELECT,            // video standards supported
      AUTO_MUTE_ENABLE,             // auto display muting
      gmd_SYNC_EVEN_OFFSET,               // input capture sync alignment
      SelectInputExternal,       // usage & extensions to configure external devices (DUMMY)
      0,                         // input color space
      gmt_IFMmeasurement,              // type of input measurement needed
      LocalCompModeTable,              // mode table for selection
      SlaveCompModeTable,              // alternative selection
      gmd_PowerNoChannel,              // power handler policy regarding the input status
      gmd_TimeBased | gmd_PowerKey,    // power handler policy regarding the actions
      0,                         // audio usage
   },

   //ALL_BOARDS_VGA1 // 3
   {
      DEV_IFM,                   // detection measurement type
      DET_RESTART,                  // detection state configuration
      {
         DCS_STATE,                 // Composite Sync
         DSS_STATE,                 // Normal mode
         SOG_STATE,                 // SOG State
         NO_SYNC_STATE
      },                         // list of sync used
#if (SPEEDUP_MODE_CHANGE == ENABLE)
	  2,						 // count until stable
	  2,						 // count until unstable
#else
	  3,	// 3,						  // count until stable //20121029 Kordonm modify from 3 to 7 for PC Dos mode 720*400@70 to 1024*768@60 doesn't do mode setup again (modify from Unique)
	  2,						  // count until unstable
#endif

      gmd_DATA,                     // IBD measurement type
      gmd_ALL_MODES_SELECT,            // video standards supported
      AUTO_MUTE_ENABLE,             // auto display muting
      gmd_SYNC_ODD_INV,             // input capture sync alignment
      SelectInputExternal,       // usage & extensions to configure external devices (DUMMY)
      0,                         // input color space (DUMMY)
      gmt_IFMmeasurement,              // type of input measurement need
      NULL,                      // mode table for selection
      NULL,                      // alternative selection
      gmd_PowerNoChannel,              // power handler policy regarding the input status
      gmd_TimeBased | gmd_PowerKey,    // power handler policy regarding the actions
      0,                         // audio usage
   },

   //ALL_BOARDS_DVI1 // 4
   {
      DEV_HDMIDVI,                   // detection measurement type
      DET_RESTART,                  // detection state configuration
      {
         DSS_STATE,                 // Composite Sync
         NO_SYNC_STATE,             // Normal mode
         NO_SYNC_STATE,             // SOG State
         NO_SYNC_STATE
      },                         // list of sync used
#if (SPEEDUP_MODE_CHANGE == ENABLE)
	3,						   // count until stable
	2,						   // count until unstable
#else
	3,	  // 3, 						// count until stable 
	1,							// count until unstable
#endif
      gmd_DE,                       // IBD measurement type
      gmd_ALL_MODES_SELECT,            // video standards supported
      AUTO_MUTE_ENABLE,             // auto display muting
      //change specific to Silicon Image Sil1161 DVI module
      (gmd_SYNC_UV_ALIGN_TO_DE | gmd_SYNC_ODD_INV | gmd_SYNC_OFFSET_HSYNC),   // input capture sync alignment
      SelectInputExternal,       // usage & extensions to configure external devices (DUMMY)
      0,                         // input color space (DUMMY)
      gmt_HdmiDvimeasurement,              // type of input measurement need
      NULL,                      // mode table for selection
      NULL,                      // alternative selection
      gmd_PowerNoChannel,              // power handler policy regarding the input status
      gmd_TimeBased | gmd_PowerKey,    // power handler policy regarding the actions
      0,                         // audio usage
   },

   //ALL_BOARDS_DVI2 // 5
   {
      DEV_HDMIDVI,                   // detection measurement type
      DET_RESTART,                  // detection state configuration
      {
         DSS_STATE,                 // Composite Sync
         NO_SYNC_STATE,             // Normal mode
         NO_SYNC_STATE,             // SOG State
         NO_SYNC_STATE
      },                         // list of sync used
#if (SPEEDUP_MODE_CHANGE == ENABLE)
	  2,						 // count until stable
	  1,						 // count until unstable
#else
	  3,	// 3,						  // count until stable 
	  1,						  // count until unstable
#endif
      gmd_DE,                       // IBD measurement type
      gmd_ALL_MODES_SELECT,            // video standards supported
      AUTO_MUTE_ENABLE,             // auto display muting
      //change specific to Silicon Image Sil1161 DVI module
      (gmd_SYNC_UV_ALIGN_TO_DE | gmd_SYNC_ODD_INV | gmd_SYNC_OFFSET_HSYNC),   // input capture sync alignment
      SelectInputExternal,       // usage & extensions to configure external devices (DUMMY)
      0,                         // input color space (DUMMY)
      gmt_HdmiDvimeasurement,              // type of input measurement need
      NULL,                      // mode table for selection
      NULL,                      // alternative selection
      gmd_PowerNoChannel,              // power handler policy regarding the input status
      gmd_TimeBased | gmd_PowerKey,    // power handler policy regarding the actions
      0,                         // audio usage
   },


   //ALL_BOARDS_DIP1 //6
	{
		DEV_EXTERN,							// detection measurement type
		DET_RESTART,						// detection state configuration
		{
			NO_SYNC_STATE,
			NO_SYNC_STATE,
			NO_SYNC_STATE,
			NO_SYNC_STATE
		},								// list of sync types (DUMMY)
	#if (SPEEDUP_MODE_CHANGE == ENABLE)
		3,						   // count until stable
		2,						   // count until unstable
	#else
		8,	  // 3, 						// count until stable
		2,							// count until unstable
	#endif
		gmd_DATA,							// IBD measurement type
		gmd_ALL_MODES_SELECT,				// video standards supported
		AUTO_MUTE_ENABLE,             // auto display muting
	#ifdef EXTERNAL_DECODER
		#if (DEFAULT_DIP_DAUGHTER_BOARD_OUT == VPC3230_601)
		gmd_SYNC_EVEN_OFFSET|gmd_SYNC_ODD_TYPE_EXT,		// input capture sync alignment
		#else
		gmd_SYNC_EVEN_OFFSET,//|gmd_SYNC_ODD_TYPE_EXT,		// input capture sync alignment
		#endif
	#else
		gmd_SYNC_ODD_TYPE_EXT,
	#endif
		ConfigureExtDevices,				// usage & extensions to configure external devices
		0,                         // input color space (DUMMY)
		gmt_ExternalStatus,									// type of input measurement needed (DUMMY)
		LocalExtDecoderModeTable,			// mode table for selection
		SlaveExtDecoderModeTable,			// alternative selection
		0,									// power handler policy regarding the input status (DUMMY)
		0,									// power handler policy regarding the actions (DUMMY)
		0,									// audio usage */
	},

   //ALL_BOARDS_HDMI // 7
   {
      DEV_HDMIDVI,                   // type of detection measurement used: Internal Decoder
      DET_RESTART,                  // detection state configuration: as restarting, or stopping it
      {
         DSS_STATE,                 // Normal mode
         NO_SYNC_STATE,
         NO_SYNC_STATE, 
         NO_SYNC_STATE, 
      },                         // list of sync used
#if (SPEEDUP_MODE_CHANGE == ENABLE)
	  3,						 // count until stable
	  1,						 // count until unstable
#else
	  3, // 3,					 //Hyb130620* Org = 6	   // count until stable
	  3,						 //Hyb130620* Org = 1 // count until unstable
#endif

      gmd_DE,                       // type of IBD measurement
      gmd_ALL_MODES_SELECT,            // video standards supported, to configure decoder detection
      AUTO_MUTE_ENABLE,             // auto display muting
      (gmd_SYNC_UV_ALIGN_TO_DE |gmd_SYNC_EVEN_OFFSET | gmd_SYNC_OFFSET_HSYNC),   // input capture sync alignment, no UV alignment for HDMI
      SelectInputExternal,       // usage & extensions to configure external devices (DUMMY)
      0,                         // input color space (DUMMY)
      gmt_HdmiDvimeasurement,              // type of input measurement needed
      NULL,                      // mode table for selection
      NULL,                      // alternative selection
      gmd_PowerNoChannel,              // power handler policy regarding the input status
      gmd_TimeBased | gmd_PowerKey,    // power handler policy regarding the actions
      0,                         // audio usage
   },

   //ALL_BOARDS_HDMI2 // 8
   {
      DEV_HDMIDVI,                   // type of detection measurement used: Internal Decoder
      DET_RESTART,                  // detection state configuration: as restarting, or stopping it
      {
         DSS_STATE,                 // Normal mode
         NO_SYNC_STATE,
         NO_SYNC_STATE,
         NO_SYNC_STATE,
      },                         // list of sync used
#if (SPEEDUP_MODE_CHANGE == ENABLE)
	2,						   // count until stable
	1,						   // count until unstable
#else
	3,						 // count until stable
	1,						   // count until unstable
#endif

      gmd_DE,                       // type of IBD measurement
      gmd_ALL_MODES_SELECT,            // video standards supported, to configure decoder detection
      AUTO_MUTE_ENABLE,             // auto display muting
      (gmd_SYNC_UV_ALIGN_TO_DE |gmd_SYNC_EVEN_OFFSET | gmd_SYNC_OFFSET_HSYNC),   // input capture sync alignment, no UV alignment for HDMI
      SelectInputExternal,       // usage & extensions to configure external devices (DUMMY)
      0,                         // input color space (DUMMY)
      gmt_HdmiDvimeasurement,              // type of input measurement needed
      NULL,                      // mode table for selection
      NULL,                      // alternative selection
      gmd_PowerNoChannel,              // power handler policy regarding the input status
      gmd_TimeBased | gmd_PowerKey,    // power handler policy regarding the actions
      0,                         // audio usage
   },

   //ALL_BOARDS_HDMI3 // 9
   {
      DEV_HDMIDVI,                   // type of detection measurement used: Internal Decoder
      DET_RESTART,                  // detection state configuration: as restarting, or stopping it
      {
         DSS_STATE,                 // Normal mode
         NO_SYNC_STATE,
         NO_SYNC_STATE,
         NO_SYNC_STATE,
      },                         // list of sync used
#if (SPEEDUP_MODE_CHANGE == ENABLE)
	1,						 // count until stable
	1,						 // count until unstable
#else
	3,					   // count until stable
	1,						 // count until unstable
#endif

      gmd_DE,                       // type of IBD measurement
      gmd_ALL_MODES_SELECT,            // video standards supported, to configure decoder detection
      AUTO_MUTE_ENABLE,             // auto display muting
      (gmd_SYNC_UV_ALIGN_TO_DE |gmd_SYNC_EVEN_OFFSET | gmd_SYNC_OFFSET_HSYNC),   // input capture sync alignment, no UV alignment for HDMI
      SelectInputExternal,       // usage & extensions to configure external devices (DUMMY)
      0,                         // input color space (DUMMY)
      gmt_HdmiDvimeasurement,              // type of input measurement needed
      NULL,                      // mode table for selection
      NULL,                      // alternative selection
      gmd_PowerNoChannel,              // power handler policy regarding the input status
      gmd_TimeBased | gmd_PowerKey,    // power handler policy regarding the actions
      0,                         // audio usage
   },


   //ALL_BOARDS_DP1 //10
   {
      DEV_DP,  //DEV_DP,          // 20090319 #2, Recommand from Peui-Hsin, DP measure by IFM mode             // type of detection measurement used: Internal Decoder
      DET_RESTART,                  // detection state configuration: as restarting, or stopping it
      {
         DSS_STATE,                   // Normal mode
         NO_SYNC_STATE,
         NO_SYNC_STATE,
         NO_SYNC_STATE,
      },                              // list of sync used
#if (SPEEDUP_MODE_CHANGE == ENABLE)
	2,						 // count until stable
	1,						 // count until unstable
#else
	6, // 3, // count until stable
	1, // count until unstable
#endif
      gmd_DE,                         // type of IBD measurement
      gmd_ALL_MODES_SELECT,           // video standards supported, to configure decoder detection
      AUTO_MUTE_ENABLE,               // auto display muting
      (gmd_SYNC_UV_ALIGN_TO_DE |gmd_SYNC_EVEN_OFFSET | gmd_SYNC_OFFSET_HSYNC),   // input capture sync alignment, no UV alignment for HDMI
      SelectInputExternal,       // usage & extensions to configure external devices (DUMMY)
      0,                              // input color space (DUMMY)
      gmt_DPmeasurement,              // type of input measurement needed
      NULL,                           // mode table for selection
      NULL,                           // alternative selection
      gmd_PowerNoChannel,             // power handler policy regarding the input status
      gmd_TimeBased | gmd_PowerKey,   // power handler policy regarding the actions
      0,                              // audio usage
   },

   //ALL_BOARDS_DP2 // 11
   {
      DEV_DP,                       // type of detection measurement used: Internal Decoder
      DET_RESTART,                  // detection state configuration: as restarting, or stopping it
      {
         DSS_STATE,                   // Normal mode
         NO_SYNC_STATE,
         NO_SYNC_STATE,
         NO_SYNC_STATE,
      },                              // list of sync used
#if (SPEEDUP_MODE_CHANGE == ENABLE)
	2,					   // count until stable
	1,					   // count until unstable
#else
	6, // 3, // count until stable
	1, // count until unstable
#endif

      gmd_DE,                         // type of IBD measurement
      gmd_ALL_MODES_SELECT,           // video standards supported, to configure decoder detection
      AUTO_MUTE_ENABLE,               // auto display muting
      (gmd_SYNC_UV_ALIGN_TO_DE |gmd_SYNC_EVEN_OFFSET | gmd_SYNC_OFFSET_HSYNC),   // input capture sync alignment, no UV alignment for HDMI
      SelectInputExternal,       // usage & extensions to configure external devices (DUMMY)
      0,                              // input color space (DUMMY)
      gmt_DPmeasurement,              // type of input measurement needed
      NULL,                           // mode table for selection
      NULL,                           // alternative selection
      gmd_PowerNoChannel,             // power handler policy regarding the input status
      gmd_TimeBased | gmd_PowerKey,   // power handler policy regarding the actions
      0,                              // audio usage
   },

   //ALL_BOARDS_DP3 //12
   {
      DEV_DP,  //DEV_DP,          // 20090319 #2, Recommand from Peui-Hsin, DP measure by IFM mode             // type of detection measurement used: Internal Decoder
      DET_RESTART,                  // detection state configuration: as restarting, or stopping it
      {
         DSS_STATE,                   // Normal mode
         NO_SYNC_STATE,
         NO_SYNC_STATE,
         NO_SYNC_STATE,
      },                              // list of sync used
      6, // 3, // count until stable
      1, // count until unstable
      gmd_DE,                         // type of IBD measurement
      gmd_ALL_MODES_SELECT,           // video standards supported, to configure decoder detection
      AUTO_MUTE_ENABLE,               // auto display muting
      (gmd_SYNC_UV_ALIGN_TO_DE |gmd_SYNC_EVEN_OFFSET | gmd_SYNC_OFFSET_HSYNC),   // input capture sync alignment, no UV alignment for HDMI
      SelectInputExternal,       // usage & extensions to configure external devices (DUMMY)
      0,                              // input color space (DUMMY)
      gmt_DPmeasurement,              // type of input measurement needed
      NULL,                           // mode table for selection
      NULL,                           // alternative selection
      gmd_PowerNoChannel,             // power handler policy regarding the input status
      gmd_TimeBased | gmd_PowerKey,   // power handler policy regarding the actions
      0,                              // audio usage
   },

	#ifdef EXTERNAL_DECODER
	#ifdef TVP5160_DEV_ID
	//ALL_BOARDS_COMP2 //13	// TVP5160 Component
	{
			DEV_EXTERN,							// detection measurement type
			DET_RESTART,						// detection state configuration
		{
			NO_SYNC_STATE,
			NO_SYNC_STATE,
			NO_SYNC_STATE,
			NO_SYNC_STATE
		},								// list of sync types (DUMMY)
#if (SPEEDUP_MODE_CHANGE == ENABLE)
		6,					   // count until stable
		4,					   // count until unstable
#else
		6,									// count until stable
		4,                         // count until unstable
#endif
		gmd_DATA,							// IBD measurement type
		gmd_ALL_MODES_SELECT,				// video standards supported
		AUTO_MUTE_ENABLE,             // auto display muting
	#ifdef EXTERNAL_DECODER
		gmd_SYNC_EVEN_OFFSET,//|gmd_SYNC_ODD_TYPE_EXT,		// input capture sync alignment
	#else
		gmd_SYNC_ODD_TYPE_EXT,
	#endif
		ConfigureExtDevices,				// usage & extensions to configure external devices
		0,                         // input color space (DUMMY)
		gmt_ExternalStatus,									// type of input measurement needed (DUMMY)
		LocalExtDecoderModeTable,			// mode table for selection
		SlaveExtDecoderModeTable,			// alternative selection
		0,									// power handler policy regarding the input status (DUMMY)
		0,									// power handler policy regarding the actions (DUMMY)
		0,									// audio usage */
	},
	#endif
	
	//ALL_BOARDS_CVBS2 //14	//TVP5160 CVBS
	{
			DEV_EXTERN,							// detection measurement type
			DET_RESTART,						// detection state configuration
		{
			NO_SYNC_STATE,
			NO_SYNC_STATE,
			NO_SYNC_STATE,
			NO_SYNC_STATE
		},								// list of sync types (DUMMY)
#if (SPEEDUP_MODE_CHANGE == ENABLE)
		6,					   // count until stable
		4,					   // count until unstable
#else
		6,									// count until stable
		4,						   // count until unstable
#endif

		gmd_DATA,							// IBD measurement type
		gmd_ALL_MODES_SELECT,				// video standards supported
		AUTO_MUTE_ENABLE,             // auto display muting
	#ifdef EXTERNAL_DECODER
		gmd_SYNC_EVEN_OFFSET,//|gmd_SYNC_ODD_TYPE_EXT,		// input capture sync alignment
	#else
		gmd_SYNC_ODD_TYPE_EXT,
	#endif
		ConfigureExtDevices,				// usage & extensions to configure external devices
		0,                         // input color space (DUMMY)
		gmt_ExternalStatus,									// type of input measurement needed (DUMMY)
		LocalExtDecoderModeTable,			// mode table for selection
		SlaveExtDecoderModeTable,			// alternative selection
		0,									// power handler policy regarding the input status (DUMMY)
		0,									// power handler policy regarding the actions (DUMMY)
		0,									// audio usage */
	},

	#endif
		 
   //ALL_BOARDS_SLAVE1 (DUMMY) // 13
   {
      0,                         // detection measurement type
      0,                         // detection state configuration
      {0},                       // list of sync used
      0,                         // count until stable
      0,                         // count until unstable
      0,                         // IBD measurement type
      0,                         // video standards supported, to configure decoder detection
      0,                         // auto display muting
      0,                         // input capture sync alignment
      0,                         // usage & extensions to configure various external devices
      0,                         // input color space
      0,                         // type of input measurement need
      0,                         // mode table for selection
      0,                         // alternative selection
      0,                         // power handler policy regarding the input status
      0,                         // power handler policy regarding the actions
      0,                         // audio usage
   },
};

//*********************************  END  **************************************
