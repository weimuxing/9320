/*
	$Workfile:   gm_PanoramicTbl.c  $
	$Revision: 1.4 $
	$Date: 2012/01/16 06:25:45 $
*/
#define __GM_PANORAMIC_TBL_C__
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
// MODULE:     	gm_PanoramicTbl.c
//
// USAGE:	This file contains table for video panoramic scaling control
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "System\all.h"

#if (FEATURE_PANORAMIC == ENABLE)
//******************************************************************************
//  C O D E
//******************************************************************************

//
// Panoramic Settings
//
ROM gmt_StandardPanorStruct StandardVideoPanoramicTableNonHDMI [] =
{
   //
   // WXGA Panels - 1280 wide
   //
   { 1280, 858,  	262,    0x016B, 0x0395, 0x0011, 0x0000 ,    0x0C20, 0x0005, 0x08BE, 1 },    // NTSC_1
   { 1280, 864,  	312,    0x016A, 0x0396, 0x000F, 0x0000 ,    0x0AB6, 0x0005, 0x5295, 1 },    // PAL_1
   { 1280, 858,  	525,    0x016F, 0x0391, 0x000E, 0x0000 ,    0x0A1E, 0x0005, 0x634B, 1 },    // 480p_1
   { 1280, 864,  	625,    0x016C, 0x0394, 0x001D, 0x0000 ,    0x14C2, 0x0002, 0x8313, 1 },    // 576p_1
   //
   // WXGA Panels - 1366 wide
   //
   { 1366, 858,	262,	0x019A, 0x03BC, 0x0015, 0x0000 ,	0x10DE, 0x0002, 0x9414, 1 },	// NTSC_1
   { 1366, 864,	312,	0x0195, 0x03C1, 0x0014, 0x0000 ,	0x0FED, 0x0002, 0xCF81, 1 },	// PAL_1
   { 1366, 858,	525,	0x0199, 0x03BD, 0x0013, 0x0000 ,	0x0F4B, 0x0002, 0xE7AB, 1 },	// 480p_1
   { 1366, 864,	625,	0x0192, 0x03C4, 0x0014, 0x0000 ,	0x0FC4, 0x0002, 0xFB92, 1 }, 	// 576p_1
   //
   // 1080p panels
   //
   { 1920, 858,	262,	0x0194, 0x05EC, 0x0008, 0x0000 ,	0x065C, 0x0003, 0x8617, 1 },	// NTSC_1
   { 1920, 864,	312,	0x0194, 0x05EC, 0x000D, 0x0000 ,	0x0A41, 0x0002, 0x1E36, 1 },	// PAL_1
   { 1920, 858,	525,	0x01A0, 0x05E0, 0x0012, 0x0000 ,	0x0EBB, 0x0000, 0x6D05, 1 },	// 480p_1
   { 1920, 864,	625,	0x0190, 0x05F0, 0x0013, 0x0000 ,	0x0EE6, 0x0000, 0xA666, 1 },	// 576p_1

   // Add new entries below sorted based on panel width

   //****************************************************************************************************
   //NOTE: Panoramic entries for panels with different resolutions, but the same width cannot co-exist in current
   //     implementation. For example, 1080p, 1080i and WUXGA panels alll have panel widths of 1920, but the
   //     library driver cannot load different tables for each individual panels unless the interface is modified.
   //****************************************************************************************************
};

ROM BYTE NUM_OF_MODES_IN_PANORAMIC_TABLE_NON_HDMI =
   (sizeof(StandardVideoPanoramicTableNonHDMI)/ sizeof(StandardVideoPanoramicTableNonHDMI[0]));

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// HDMI tables (separate table needed since search algorithm depends on H/V size which is not unique between HDMI
// and non-HDMI modes)
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////

ROM gmt_StandardPanorStruct StandardVideoPanoramicTableHDMI [] =
{
   //
   // WXGA Panels - 1280 wide
   //
   { 1280, 858,	263,	0x016B, 0x0385, 0x0014, 0x0000 ,	0x0E40, 0x0004, 0x7286, 1 },	// 480i and 480i - 2x
   { 1280, 3432,	262,	0x0172, 0x038E, 0x013E, 0x0000 ,	0xE708, 0x0067, 0x7A6F, 0 },	// 480i - 4x
   { 1280, 864,	312,	0x0169, 0x0397, 0x0019, 0x0000 ,	0x11BD, 0x0003, 0xEA32, 1 },	// 576i and 576i - 2x
   { 1280, 3456,	312,	0x0172, 0x038E, 0x00EE, 0x0000 ,	0xAD46, 0x0056, 0x98D3, 0 },	// 576i - 4x
   { 1280, 858,	525,	0x016F, 0x0391, 0x0016, 0x0000 ,	0x0FA8, 0x0004, 0x0E43, 1 },	// 480p
   { 1280, 1716,	525,	0x0184, 0x037C, 0x001C, 0x0000 ,	0x1546, 0x0018, 0x70D6, 0 },	// 480p - 2x
   { 1280, 864,	625,	0x0172, 0x038E, 0x0018, 0x0000 ,	0x1162, 0x0003, 0x85C9, 1 },	// 576p_1
   //
   // WXGA Panels - 1366 wide
   //
   { 1366, 858,	263,	0x019D, 0x03B9, 0x0012, 0x0000 ,	0x0E9C, 0x0003, 0xB8FE, 1 },	// 480i and 480i - 2x
   { 1366, 3432,	262,	0x0198, 0x03BE, 0x0034, 0x0000 ,	0x29D8, 0x0014, 0x5D69, 1 },	// 480i - 4x
   { 1366, 864,	312,	0x0196, 0x03C0, 0x0014, 0x0000 ,	0x0FE9, 0x0003, 0x5E30, 1 },	// 576i and 576i - 2x
   { 1366, 3456,	312,	0x0198, 0x03BE, 0x009A, 0x0000 ,	0x7B4A, 0x0049, 0x1F86, 0  },	// 576i - 4x
   { 1366, 858,	525,	0x0193, 0x03C3, 0x0015, 0x0000 ,	0x108F, 0x0003, 0x3128, 1  },	// 480p
   { 1366, 1716,	525,	0x0194, 0x03C2, 0x000C, 0x0000 ,	0x0987, 0x0013, 0xE3A6, 0 },	// 480p - 2x
   { 1366, 864,	625,	0x0198, 0x038E, 0x0010, 0x0000 ,	0x0CDC, 0x0004, 0x52F9, 1 },	// 576p_1
   //
   // 1080p panels
   //
   { 1920, 858,	263,	0x01A6, 0x05DA, 0x0008, 0x0000 ,	0x0652, 0x0003, 0x796A, 1 },	// 480i and 480i - 2x
   { 1920, 3432,	262,	0x01A6, 0x05DA, 0x007D, 0x0000 ,	0x6788, 0x003C, 0x71C7, 0 },	// 480i - 4x
   { 1920, 864,	312,	0x01A0, 0x05E0, 0x000E, 0x0000 ,	0x0B6A, 0x0002, 0x076B, 1 },	// 576i and 576i - 2x
   { 1920, 3456,	312,	0x01A6, 0x05DA, 0x008B, 0x0000 ,	0x735D, 0x0040, 0x9C09, 0 },	// 576i - 4x
   { 1920, 858,	525,	0x01A0, 0x05E0, 0x000B, 0x0000 ,	0x0880, 0x0002, 0xE109, 1 },	// 480p
   { 1920, 1716,	525,	0x01A3, 0x05DD, 0x0018, 0x0000 ,	0x13BA, 0x0005, 0x18D2, 1 },	// 480p - 2x
   { 1920, 864,	625,	0x019F, 0x05E1, 0x000D, 0x0000 ,	0x0A94, 0x0002, 0x5392, 1 },	// 576p_1

   // Add new entries below sorted based on panel width

   //****************************************************************************************************
   //NOTE: Panoramic entries for panels with different resolutions, but the same width cannot co-exist in current
   //     implementation. For example, 1080p, 1080i and WUXGA panels alll have panel widths of 1920, but the
   //     library driver cannot load different tables for each individual panels unless the interface is modified.
   //****************************************************************************************************
};

ROM BYTE NUM_OF_MODES_IN_PANORAMIC_TABLE_HDMI =
   (sizeof(StandardVideoPanoramicTableHDMI)/ sizeof(StandardVideoPanoramicTableHDMI[0]));

ROM gmt_StandardPanorStruct StandardVideoPanoramicTableNoCropHDMI [] =
{
   //
   // WXGA Panels - 1280 wide
   //
   { 1280, 858,	525,	0x016F, 0x0391, 0x0014, 0x0000 ,	0x0E90, 0x0004, 0x0BD5F, 1 },	// 480p
};

ROM BYTE NUM_OF_MODES_IN_PANORAMIC_TABLE_NO_CROP_HDMI =
   (sizeof(StandardVideoPanoramicTableNoCropHDMI)/ sizeof(StandardVideoPanoramicTableNoCropHDMI[0]));

#endif // FEATURE_PANORAMIC
//************************************* END ************************************

