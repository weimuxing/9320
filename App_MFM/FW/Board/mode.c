/*
	$Workfile:   mode.c  $
	$Revision: 1.29 $
	$Date: 2013/05/28 02:17:30 $
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
//
//  MODULE: mode.c
//
//  USAGE: 	This module contains the supported modes look up tables
//
//  NOTE : 	MUST update the following #define in mode.h if table is changed
//
//  		#define NumberOfModes
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "System\all.h"


#if FEATURE_AIP == ENABLE

//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************

//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************

//******************************************************************************
//
// USAGE    :  Component through HD Standard video mode lookup table
//
// NOTE     :  For "StdHStart, StdVStart"; The "Start" is based on the
//					sum of the Sync Time and Back Porch.
//
//
//******************************************************************************
ROM StandardModeStruct  CompHD_ModeTable[] =
{
   {  // 0 *** 480I/525I -  720 x  240 x 60I ***
      157, 20,    // StdHFreq, DeltaHFreq, (in 100Hz unit)
      599, 20,    // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      262, 35,    // StdVTotal, DeltaVTotal, (in line unit)
      858,        // StdHTotal,
      720, 240,   // StdWidth, StdHeight,
      63, 15,     // StdHStart, StdVStart
      0,          // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_SDVIDEO | gmd_MODE_SOG | gmd_MODE_INTERLACED,      // ModeType
      0,          // Overlap Type
   },

   {  // 1 *** 576I/625I  -  720 x  288 x 50I ***
      156, 20,    // StdHFreq, DeltaHFreq, (in 100Hz unit)
      500, 20,    // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      312, 35,    // StdVTotal, DeltaVTotal, (in line unit)
      864,        // StdHTotal,
      720, 288,   // StdWidth, StdHeight,
      81, 21,     // StdHStart, StdVStart,
      0,          // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_SDVIDEO | gmd_MODE_SOG | gmd_MODE_INTERLACED,      // ModeType
      0,          // Overlap Type
   },

   {  // 2 *** 720P (24Hz) - 1280 x  720 x 24P ***
      180, 50,              // StdHFreq, DeltaHFreq, (in 100Hz unit)
      240, 50,              // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      750, 10,              // StdVTotal, DeltaVTotal, (in line unit)
      3300,                 // StdHTotal,
      1280, 720,            // StdWidth, StdHeight,
      264, 17,              // StdHStart, StdVStart,
      0,					// StdHpulse (used only in VideoOverlapped)
      gmd_MODE_HDVIDEO | gmd_MODE_SOG, 		// ModeType
      0,				// Overlap Type
   },
   
   {  // 3 *** 480P (60Hz) -  720 x  480 x 60P ***
      314, 10,    // StdHFreq, DeltaHFreq, (in 100Hz unit)
      600, 10,    // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      525, 10,    // StdVTotal, DeltaVTotal, (in line unit)
      858,        // StdHTotal,
      720, 480,   // StdWidth, StdHeight,
      49, 39,     // StdHStart, StdVStart,
      62,         // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_EDVIDEO | gmd_MODE_SOG,    // ModeType
      0,      // Overlap Type
   },  
   
   {  // 4 *** 576P (50Hz) -  720 x  576 x 50P ***
      312, 10,    // StdHFreq, DeltaHFreq, (in 100Hz unit)
      500, 10,    // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      624, 10,    // StdVTotal, DeltaVTotal, (in line unit)
      864,        // StdHTotal,
      720, 576,   // StdWidth, StdHeight,
      90, 53,     // StdHStart, StdVStart,
      33+6,       // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_EDVIDEO | gmd_MODE_SOG,    // ModeType
      gmd_MODE_OVERLAPPED_1080_576,       // Overlap Type
   },

   {  // 5 *** 720P (50Hz) - 1280 x  720 x 50P ***
      375, 50,              // StdHFreq, DeltaHFreq, (in 100Hz unit)
      500, 50,              // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      750, 10,              // StdVTotal, DeltaVTotal, (in line unit)
      1980,                 // StdHTotal,
      1280, 720,            // StdWidth, StdHeight,
      262, 18,              // StdHStart, StdVStart,
      0,					// StdHpulse (used only in VideoOverlapped)
      gmd_MODE_HDVIDEO | gmd_MODE_SOG,		// ModeType
      0,				// Overlap Type
   },

   {  // 6 *** 720P (60Hz) - 1280 x  720 x 60P ***
      449, 50,              // StdHFreq, DeltaHFreq, (in 100Hz unit)
      600, 50,              // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      750, 10,              // StdVTotal, DeltaVTotal, (in line unit)
      1650,                 // StdHTotal,
      1280, 720,            // StdWidth, StdHeight,
      262+2, 18-1,           // StdHStart, StdVStart,
      0,					// StdHpulse (used only in VideoOverlapped)
      gmd_MODE_HDVIDEO | gmd_MODE_SOG, 		// ModeType
      0,				// Overlap Type
   },

   {  // 7 *** 1080I (50Hz) - 1920 x 1080 x 50I ***
      281, 50,              // StdHFreq, DeltaHFreq, (in 100Hz unit)
      500, 50,              // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      562, 20,              // StdVTotal, DeltaVTotal, (in line unit)
      2640,                 // StdHTotal,
      1920, 540,           	// StdWidth, StdHeight,
      200, 13,              // StdHStart, StdVStart,
      0,					// StdHpulse (used only in VideoOverlapped)
      gmd_MODE_HDVIDEO | gmd_MODE_SOG | gmd_MODE_INTERLACED,		// ModeType
      0,			// Overlap Type
   },

   {  // 8 *** 1080I (50Hz) - 1920 x 1080 x 50I ***  // smpte 295m
      312, 10,              // StdHFreq, DeltaHFreq, (in 100Hz unit)
      500, 10,              // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      624, 10,              // StdVTotal, DeltaVTotal, (in line unit)
      2376,                 // StdHTotal,
      1920, 540,           	// StdWidth, StdHeight,
      318, 77,              // StdHStart, StdVStart,
      33+6,					// StdHpulse (used only in VideoOverlapped)
      gmd_MODE_HDVIDEO | gmd_MODE_SOG | gmd_MODE_INTERLACED,		// ModeType
      gmd_MODE_OVERLAPPED_1080_576,			// Overlap Type
   },

   {  // 9 *** 1080i Australian    - 1280 x  1152 x 50P ***
      312, 50,			  // StdHFreq, DeltaHFreq, (in 100Hz unit)
      250, 50,			  // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      1250, 10,        // StdVTotal, DeltaVTotal, (in line unit)
      2304,				  // StdHTotal,
      1920, 540,       // StdWidth, StdHeight,
      235, 42,			  // StdHStart, StdVStart,
      0,					  // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_HDVIDEO | gmd_MODE_INTERLACED | gmd_MODE_SOG,   // ModeType OverlapSyncPolarity
      0,				// Overlap Type
   },

   {  // 10 *** 1080I (60Hz) - 1920 x 1080 x 60I ***
      337, 50,              // StdHFreq, DeltaHFreq, (in 100Hz unit)
      600, 50,              // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      562, 20,              // StdVTotal, DeltaVTotal, (in line unit)
      2200,                 // StdHTotal,
      1920, 540,            // StdWidth, StdHeight,
      194, 13,              // StdHStart, StdVStart,
      0,					// StdHpulse (used only in VideoOverlapped)
      gmd_MODE_HDVIDEO | gmd_MODE_SOG | gmd_MODE_INTERLACED,		// ModeType
      0,			// Overlap Type
   },

   {  // 11 *** 1080P (24Hz) - 1920 x 1080 x 25P ***
      270, 8,              // StdHFreq, DeltaHFreq, (in 100Hz unit)
      240, 5,              // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      1125, 20,            // StdVTotal, DeltaVTotal, (in line unit)
      2750,                // StdHTotal,
      1920, 1080,          // StdWidth, StdHeight,
      190, 18,             // StdHStart, StdVStart,
      0,     // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_HDVIDEO | gmd_MODE_SOG,   // ModeType
      0,
   },
   
   {  // 12 *** 1080P (25Hz) - 1920 x 1080 x 25P ***
      282, 50,              // StdHFreq, DeltaHFreq, (in 100Hz unit)
      250, 20,              // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      1125, 20,             // StdVTotal, DeltaVTotal, (in line unit)
      2640,                 // StdHTotal,
      1920, 1080,           // StdWidth, StdHeight,
      190, 18,              // StdHStart, StdVStart,
      0,     // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_HDVIDEO | gmd_MODE_SOG,   // ModeType
      0,
   },
   
   {  // 13 *** 1080P (30Hz) - 1920 x 1080 x 30P ***
      337, 50,              // StdHFreq, DeltaHFreq, (in 100Hz unit)
      300, 20,              // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      1125, 20,             // StdVTotal, DeltaVTotal, (in line unit)
      2200,                 // StdHTotal,
      1920, 1080,           // StdWidth, StdHeight,
      198, 18,              // StdHStart, StdVStart,
      0,     // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_HDVIDEO | gmd_MODE_SOG,   // ModeType
      0,				// Overlap Type
   },

   {  // 14 *** 1080P (60Hz) - 1920 x 1080 x 60P ***
      675, 50,              // StdHFreq, DeltaHFreq, (in 100Hz unit)
      600, 20,              // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      1125, 20,             // StdVTotal, DeltaVTotal, (in line unit)
      2200,                 // StdHTotal,
      1920, 1080,           // StdWidth, StdHeight,
      190,18,              // StdHStart, StdVStart,
      0,     // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_HDVIDEO | gmd_MODE_SOG,   // ModeType
      0,				// Overlap Type
   },

   {  // 15 *** 1080P (50Hz) - 1920 x 1080 x 50P ***
      560, 50,              // StdHFreq, DeltaHFreq, (in 100Hz unit)
      500, 20,              // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      1125, 20,             // StdVTotal, DeltaVTotal, (in line unit)
      2640,                 // StdHTotal,
      1920, 1080,            // StdWidth, StdHeight,
      198, 18,              // StdHStart, StdVStart,
      0,     // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_HDVIDEO | gmd_MODE_SOG,   // ModeType
      0,				// Overlap Type
   },

   {  // 16 *** 1152iLA    - 720 x  1152 x 50I ***
      312, 50,               // StdHFreq, DeltaHFreq, (in 100Hz unit)
      500, 50,               // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      624, 10,               // StdVTotal, DeltaVTotal, (in line unit)
      1188,                  // StdHTotal,
      720, 576,              // StdWidth, StdHeight,
      234, 49,               // StdHStart, StdVStart,
      0,                        // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_HDVIDEO | gmd_MODE_SOG | gmd_MODE_INTERLACED,      // ModeType OverlapSyncPolarity
      gmd_MODE_OVERLAPPED_1080_576,					// Overlap Type
   },

   {  // 17 *** 1152iSH    - 1280 x  1152 x 50I ***
      312, 50,               // StdHFreq, DeltaHFreq, (in 100Hz unit)
      500, 50,               // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      625, 10,               // StdVTotal, DeltaVTotal, (in line unit)
      1584,                   // StdHTotal,
      1280, 576,            // StdWidth, StdHeight,
      128, 49,               // StdHStart, StdVStart,
      0,                        // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_HDVIDEO | gmd_MODE_SOG | gmd_MODE_INTERLACED,      // ModeType OverlapSyncPolarity
      gmd_MODE_OVERLAPPED_1080_576,					// Overlap Type
   },

   {  // 18 *** 1152iLH    - 1920 x  1080 x 50I ***
      312, 50,       // StdHFreq, DeltaHFreq, (in 100Hz unit)
      500, 50,       // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      625, 10,       // StdVTotal, DeltaVTotal, (in line unit)
      2376,          // StdHTotal,
      1920, 540,     // StdWidth, StdHeight,
      128, 85,       // StdHStart, StdVStart,
      0,             // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_HDVIDEO | gmd_MODE_SOG | gmd_MODE_INTERLACED,      // ModeType OverlapSyncPolarity

      gmd_MODE_OVERLAPPED_1080_576,    // Overlap Type
   },
   
   
   {  // 19 *** 1080i (100Hz) - 1920 x  1080 x 100I ***
      561, 50,       // StdHFreq, DeltaHFreq, (in 100Hz unit)
      997, 50,       // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      563, 10,       // StdVTotal, DeltaVTotal, (in line unit)
      2640,          // StdHTotal,
      1920, 540,     // StdWidth, StdHeight,
      185, 12,       // StdHStart, StdVStart,
      44,            // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_HDVIDEO | gmd_MODE_SOG | gmd_MODE_INTERLACED,      // ModeType OverlapSyncPolarity
      0,    // Overlap Type
   },

   {  // 20 *** SMPTE295Mp , 1080p (50Hz) - 1920 x  1080 x 50P ***
      623, 50,       // StdHFreq, DeltaHFreq, (in 100Hz unit)
      500, 50,       // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      1250, 10,      // StdVTotal, DeltaVTotal, (in line unit)
      2376,          // StdHTotal,
      1920, 1080,    // StdWidth, StdHeight,
      313, 165,      // StdHStart, StdVStart,
      0,             // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_HDVIDEO | gmd_MODE_SOG,      // ModeType OverlapSyncPolarity
      0,    // Overlap Type
   },
   
   {  // 21 *** 480P (120Hz)  -  720 x  480 x 120P ***
      632, 10,    // StdHFreq, DeltaHFreq, (in 100Hz unit)
      1204, 10,   // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      525, 10,    // StdVTotal, DeltaVTotal, (in line unit)
      858,        // StdHTotal,
      720, 480,   // StdWidth, StdHeight,
      49, 39,     // StdHStart, StdVStart,
      62,         // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_EDVIDEO | gmd_MODE_SOG,    // ModeType
      0,          // Overlap Type
   },
   
   {  // 22 *** 1080i (120Hz) - 1920 x  1080 x 120I ***
      673, 50,       // StdHFreq, DeltaHFreq, (in 100Hz unit)
      1198, 50,      // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      562, 10,       // StdVTotal, DeltaVTotal, (in line unit)
      2200,          // StdHTotal,
      1920, 540,     // StdWidth, StdHeight,
      191, 12,       // StdHStart, StdVStart,
      44,            // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_HDVIDEO | gmd_MODE_SOG | gmd_MODE_INTERLACED,      // ModeType OverlapSyncPolarity
      0,             // Overlap Type
   },

   {  // 23 *** 480P (240Hz) -  720 x  480 x 240P ***
      1250, 10,   // StdHFreq, DeltaHFreq, (in 100Hz unit)
      2380, 10,   // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      525, 10,    // StdVTotal, DeltaVTotal, (in line unit)
      858,        // StdHTotal,
      720, 480,   // StdWidth, StdHeight,
      49, 39,     // StdHStart, StdVStart,
      62,         // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_EDVIDEO | gmd_MODE_SOG,    // ModeType
      0,          // Overlap Type
   },

   {  // 24 *** 576P (100Hz) -  720 x  576 x 100P ***
      623, 10,    // StdHFreq, DeltaHFreq, (in 100Hz unit)
      997, 10,    // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      624, 10,    // StdVTotal, DeltaVTotal, (in line unit)
      864,        // StdHTotal,
      720, 576,   // StdWidth, StdHeight,
      90, 53,     // StdHStart, StdVStart,
      39,         // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_EDVIDEO | gmd_MODE_SOG,    // ModeType
      0,          // Overlap Type
   },

   {  // 25 *** 720P (100Hz) - 1280 x  720 x 100P ***
      752, 50,              // StdHFreq, DeltaHFreq, (in 100Hz unit)
      1002, 50,             // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      750, 10,              // StdVTotal, DeltaVTotal, (in line unit)
      1980,                 // StdHTotal,
      1280, 720,            // StdWidth, StdHeight,
      264, 17,              // StdHStart, StdVStart,
      0,                    // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_HDVIDEO | gmd_MODE_SOG, 		// ModeType
      0,				// Overlap Type
   },

   {  // 26 *** 720P (120Hz) - 1280 x  720 x 120P ***
      903, 50,              // StdHFreq, DeltaHFreq, (in 100Hz unit)
      1204, 50,             // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      750, 10,              // StdVTotal, DeltaVTotal, (in line unit)
      1650,                 // StdHTotal,
      1280, 720,            // StdWidth, StdHeight,
      264, 17,              // StdHStart, StdVStart,
      0,					       // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_HDVIDEO | gmd_MODE_SOG, 		// ModeType
      0,				// Overlap Type
   },
   
   {  // *** Last Entry ***
      0, 0,
      0, 0,
      0, 0,
      0,
      0, 0,
      0, 0,
      0,
      0,
      0,
   }
};

//******************************************************************************
//
// USAGE        :   Standard Graphics mode lookup table
//
//******************************************************************************

//******************************************************************************
//  Stanard Mode table for timing above 60Hz
//******************************************************************************
ROM StandardModeStruct StandardModeTable_60[] =
{
	//Hyb130514+ for native mode input even it's 50Hz
	{  // 46 *** 2560 x 1440 x 50 Hz - PC2 ***
	   741,10,				 // StdHFreq, (in 100Hz unit)
	   500,10,
	   1481,4,				// StdVTotal, (in line unit)
	   2720,				// StdHTotal,
	   2560, 1440,			// StdWidth, StdHeight,
	   80,	1 + 3,	 // StdHBackPorch, StdVBackPorch,
	   0,					// StdHpulse (used only in VideoOverlapped)
	   gmd_MODE_GRAPHICS,	// ModeType,
	   0,		// ExtCheck
	},
   
   {  // 0 *** 1920 x 1440 x 60 Hz Reduce ***
      888, 10,          // StdHFreq, (in 100Hz unit)
      600, 10,
      1481, 4,          // StdVTotal, (in line unit)
      2080,             // StdHTotal,
      1920, 1440,       // StdWidth, StdHeight,
      80, 34,           // StdHBackPorch, StdVBackPorch,
      32,               // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_GRAPHICS,   // ModeType,
      0,                   // ExtCheck
   },

   {  // 1 *** 1792 x 1344 x 60 Hz ***
      836, 10,          // StdHFreq, (in 100Hz unit)
      600, 10,          // StdVFreq, (in 1/10Hz unit)
      1394, 4,          // StdVTotal, (in line unit)
      2448,             // StdHTotal,
      1792, 1344,          // StdWidth, StdHeight,
      328 + 200, 46 + 3,   // StdHBackPorch, StdVBackPorch,
      0,                   // StdHSyncWidth (for overlapped mode)
      gmd_MODE_GRAPHICS,   // ModeType,
      0,                   // ExtCheck
   },

   {  // 2 *** 1600 x 1280 x 60 Hz ***
      795,10,                    // StdHFreq, (in 100Hz unit)
      600,10,
      1327,4,                 // StdVTotal, (in line unit)
      2160,                // StdHTotal,
      1600, 1280,             // StdWidth, StdHeight,
      112 + 168, 3 + 7,                // StdHBackPorch, StdVBackPorch,
      0,                // StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS,            // ModeType,
      0,               // ExtCheck
   },

   {  // 3 *** 1600 x 1200 x 60 Hz ***        //sync is Pos  ,V Plus=3
      750,10,     // StdHFreq, (in 100Hz unit)
      600,10,     // StdVFreq, (in 1/10Hz unit)
      1250, 4,    // StdVTotal, (in line unit)
      2160,       // StdHTotal,
      1600, 1200,          // StdWidth, StdHeight,
      304 + 192,46 + 3,    // StdHBackPorch, StdVBackPorch,
      0,                   // StdHSyncWidth (for overlapped mode)
      gmd_MODE_GRAPHICS,      // ModeType,
      0,                      // ExtCheck
   },

   {  // 4 *** 1920 x 1200 x 60 Hz ***	        //sync is Pos  ,V Plus=6
      745,10,     // StdHFreq, (in 100Hz unit)
      600, 10,
      1245, 4,    // StdVTotal, (in line unit)
      2592,       // StdHTotal,
      1920, 1200,          // StdWidth, StdHeight,
      336 + 200, 36 + 6,   // StdHBackPorch, StdVBackPorch,
      0,                   // StdVSyncWidth (for overlapped mode)
      gmd_MODE_GRAPHICS,            // ModeType,
      0,   // ExtCheck
   },

   {  // 5 *** 1600 x 1200 x 60 Hz Reduce Blanking ***
      740, 10,       // StdHFreq, (in 100Hz unit)
      600, 10,       // StdVFreq, (in 1/10Hz unit)
      1235, 4,       // StdVTotal, (in line unit)
      1760,          // StdHTotal,
      1600, 1200,          // StdWidth, StdHeight,
      80 + 32, 26 + 3,     // StdHBackPorch, StdVBackPorch,
      4,             // StdVSyncWidth (for overlapped mode)
      gmd_MODE_GRAPHICS,            // ModeType,
      gmd_MODE_OVERLAPPED_VWIDTH,   // ExtCheck
   },
   
   {  // 6 *** 1920 x 1200 x 60 Hz Reduce Blanking ***
      740, 10,       // StdHFreq, (in 100Hz unit)
      600, 10,       // StdVFreq, (in 1/10Hz unit)
      1235, 4,       // StdVTotal, (in line unit)
      2080,          // StdHTotal,
      1920, 1200,          // StdWidth, StdHeight,
      80 + 32,26 + 6,      // StdHBackPorch, StdVBackPorch,
      6,          // StdVSyncWidth (for overlapped mode)
      gmd_MODE_GRAPHICS,   // ModeType,
      0,      // ExtCheck
   },

//20120625 add 2560x1080 timing table, Kordonm
   {  // 7 *** 2560 x 1080 x 60 Hz - 148.5MHz ***
	666, 10,                                                 // StdHFreq, DeltaHFreq, (in 100Hz unit)
	600, 10,                                  // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
	1111, 4,                                                 // StdVTotal, DeltaVTotal, (in line unit)
	2784,                                                     // StdHTotal,
	2560, 1080,                                          // StdWidth, StdHeight,
	96,18,                                                    // StdHStart, StdVStart,
	0,                                                            // StdHpulse (used only in VideoOverlapped)
	gmd_MODE_NEG_VSYNC,          // ModeType
	gmd_MODE_OVERLAPPED_VWIDTH,	//0,                                                             // Overlap Type
   },

   {  // 8 *** 1920 x 1080 x 50 Hz - 141.5MHz ***
      556, 10,             // StdHFreq, DeltaHFreq, (in 100Hz unit)
      499, 10,             // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      1114, 4,             // StdVTotal, DeltaVTotal, (in line unit)
      2544,                // StdHTotal,
      1920, 1080,          // StdWidth, StdHeight,
      312 + 200, 26 +5,              // StdHStart, StdVStart,
      0,                   // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_GRAPHICS,   // ModeType
      0,          // Overlap Type
   },

   {  // 9 *** 1920 x 1080 x 60 Hz - 148.5MHz ***
      675, 10,             // StdHFreq, DeltaHFreq, (in 100Hz unit)
      600, 10,             // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      1125, 4,             // StdVTotal, DeltaVTotal, (in line unit)
      2200,                // StdHTotal,
      1920, 1080,          // StdWidth, StdHeight,
      148 + 44, 36 + 5,	//190,18,              // StdHStart, StdVStart,
      0,                   // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_GRAPHICS,   // ModeType
      0,          // Overlap Type
   },

   {  // 10 *** 1920 x 1080 x 60 Hz - PC1 ***
      670,10,              // StdHFreq, (in 100Hz unit)
      600,10,
      1118,4,              // StdVTotal, (in line unit)
      2576,                // StdHTotal,
      1920, 1080,          // StdWidth, StdHeight,
      120 + 208,  1 + 3,   // StdHBackPorch, StdVBackPorch,
      0,                   // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_GRAPHICS,   // ModeType,
      0,       // ExtCheck
   },

   {  // 11 *** 1920 x 1080 x 60 Hz - Reduced ***
      666,10,           // StdHFreq, (in 100Hz unit)
      600, 10,
      1111,4,           // StdVTotal, (in line unit)
      2080,             // StdHTotal,
      1920, 1080,          // StdWidth, StdHeight,
      120 + 208, 1 + 3,    // StdHBackPorch, StdVBackPorch,
      0,                   // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_GRAPHICS,   // ModeType,
      0,                   // ExtCheck
   },
   
   {  // 12 *** 1680 x 1050 x 60 Hz ***
      653,10,     // StdHFreq, (in 100Hz unit)
      600, 10,    // StdVFreq, (in 1/10Hz unit)
      1089, 4,    // StdVTotal, (in line unit)
      2240,       // StdHTotal,
      1680, 1050,          // StdWidth, StdHeight,
      280 + 176 - (188), 21 + 6 + (9),   // StdHBackPorch, StdVBackPorch,
      0,
      gmd_MODE_GRAPHICS,            // ModeType,
      0,	//gmd_MODE_OVERLAPPED_VWIDTH,	//0,   // ExtCheck
   },
   
   {  // 13 *** 1400 x 1050 x 60 Hz ***
      653, 10,    // StdHFreq, (in 100Hz unit)
      600, 10,    // StdVFreq, (in 1/10Hz unit)
      1089,4,     // StdVTotal, (in line unit)
      1864,       // StdHTotal,
      1400, 1050,          // StdWidth, StdHeight,
      232 + 144, 32 + 4,   // StdHBackPorch, StdVBackPorch,
      144,
      gmd_MODE_GRAPHICS,            // ModeType,
      gmd_MODE_OVERLAPPED_HWIDTH,   // ExtCheck
   },

   {  // 11 *** 1680 x 1050 x 60 Hz ***
      653,10,     // StdHFreq, (in 100Hz unit)
      600, 10,    // StdVFreq, (in 1/10Hz unit)
      1089, 4,    // StdVTotal, (in line unit)
      2240,       // StdHTotal,
      1680, 1050,          // StdWidth, StdHeight,
      280 + 176, 21 + 6,   // StdHBackPorch, StdVBackPorch,
      0,
      gmd_MODE_GRAPHICS,            // ModeType,
      0,	// gmd_MODE_OVERLAPPED_HWIDTH,   // ExtCheck
   },

   {  // 14 *** 1680 x 1050 x 60 Hz Reduce Blanking ***
      647, 10,    // StdHFreq, (in 100Hz unit)
      600, 10,    // StdVFreq, (in 1/10Hz unit)
      1080,4,     // StdVTotal, (in line unit)
      1840,       // StdHTotal,
      1680, 1050,          // StdWidth, StdHeight,
      80 + 32, 21 + 6,     // StdHBackPorch, StdVBackPorch,
      6,          // StdVSyncWidth (for overlapped mode)
      gmd_MODE_GRAPHICS,               // ModeType,
      gmd_MODE_OVERLAPPED_VWIDTH,      // ExtCheck
   },

   {  // 15 *** 1400 x 1050 x 60 Hz Reduce Blanking ***
      647,10,     // StdHFreq, (in 100Hz unit)
      600, 10,    // StdVFreq, (in 1/10Hz unit)
      1080, 4,    // StdVTotal, (in line unit)
      1560,       // StdHTotal,
      1400, 1050,          // StdWidth, StdHeight,
      80 + 32,  23 + 4,    // StdHBackPorch, StdVBackPorch,
      0,
      gmd_MODE_GRAPHICS,      // ModeType,
      0,                      // ExtCheck
   },

   {  // 16 *** 1920 x 1080 x 60 Hz - PC2 ***
      641,10,              // StdHFreq, (in 100Hz unit)
      590,10,
      1086,4,              // StdVTotal, (in line unit)
      2256,                // StdHTotal,
      1920, 1080,          // StdWidth, StdHeight,
      120 + 208,  1 + 3,   // StdHBackPorch, StdVBackPorch,
      0,                   // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_GRAPHICS,   // ModeType,
      0,       // ExtCheck
   },

   {  // 17 *** 1280 x 1024 x 60 Hz ***
      639,10,     // StdHFreq, (in 100Hz unit)
      600, 10,    // StdVFreq, (in 1/10Hz unit)
      1066, 4,    // StdVTotal, (in line unit)
      1688,       // StdHTotal,
      1280, 1024,          // StdWidth, StdHeight,
      248 + 112, 38 + 3,   // StdHBackPorch, StdVBackPorch,
      0,
      gmd_MODE_GRAPHICS,   // ModeType,
      0,                   // ExtCheck
   },

   {  // 18  *** 1600 x 1000 x 60 Hz ***
      621, 10,    // StdHFreq, (in 100Hz unit)
      600, 10,    // StdVFreq, (in 1/10Hz unit)
      1038, 4,    // StdVTotal, (in line unit)
      2128,       // StdHTotal,
      1600, 1000,          // StdWidth, StdHeight,
      96 + 168, 3 + 6,     // StdHBackPorch, StdVBackPorch,
      0,
      gmd_MODE_GRAPHICS,   // ModeType,
      0,                   // ExtCheck
   },

   {  // 19 *** 1600 x 1000 x 60 Hz Reduce ***
      616, 10,    // StdHFreq, (in 100Hz unit)
      599, 10,    // StdVFreq, (in 1/10Hz unit)
      1029, 4,    // StdVTotal, (in line unit)
      1760,       // StdHTotal,
      1600, 1000,          // StdWidth, StdHeight,
      80 + 32, 20 + 6,	//96 + 168, 3 + 6,     // StdHBackPorch, StdVBackPorch,
      0,                   // StdHSyncWidth (for overlapped mode)
      gmd_MODE_NEG_HSYNC,              // ModeType,
      0,    // ExtCheck
   },

   {  // 20 *** 1920 x 1200 x 50 Hz Reduce ***
      614, 10,    // StdHFreq, (in 100Hz unit)
      500, 10,
      1229, 4,    // StdVTotal, (in line unit)
      2080,       // StdHTotal,
      1920, 1200,             // StdWidth, StdHeight,
      60 + 40, 3 + 6,         // StdHBackPorch, StdVBackPorch,
      0,                // StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS,            // ModeType,
      0,    // ExtCheck
   },

   {  // 21 *** 1600 x 900 x 60 Hz Reduce ***
      600, 10,    // StdHFreq, (in 100Hz unit)
      600, 10,    // StdVFreq, (in 1/10Hz unit)
      1000, 4,    // StdVTotal, (in line unit)
      1800,       // StdHTotal,
      1600, 900,           // StdWidth, StdHeight,
      96, 96,   // StdHBackPorch, StdVBackPorch,
      80,
      gmd_MODE_GRAPHICS,      // ModeType,
      gmd_MODE_OVERLAPPED_VWIDTH,	//gmd_MODE_OVERLAPPED_HWIDTH,     // ExtCheck
   },

   {  // 22 *** 1280 x 960 x 60 Hz ***
      600, 10,    // StdHFreq, (in 100Hz unit)
      600, 10,    // StdVFreq, (in 1/10Hz unit)
      1000, 4,    // StdVTotal, (in line unit)
      1800,       // StdHTotal,
      1280, 960,           // StdWidth, StdHeight,
      312 + 112, 36 + 3,   // StdHBackPorch, StdVBackPorch,
      0,
      gmd_MODE_GRAPHICS,      // ModeType,
      0,                      // ExtCheck
   },

   {  // 23 *** 1600 x 900 x 60 Hz Reduce ***
      559, 10,    // StdHFreq, (in 100Hz unit)
      600, 10,    // StdVFreq, (in 1/10Hz unit)
      934, 4,	//926,  4,    // StdVTotal, (in line unit)
      2112,	//2128,       // StdHTotal,
      1600, 900,           // StdWidth, StdHeight,
      256 + 168, 26 + 5,	//232 + 80, 25 + 6,    // StdHBackPorch, StdVBackPorch,
      5,                   // StdVSyncWidth (for overlapped mode)
      gmd_MODE_GRAPHICS,   // ModeType,
      gmd_MODE_OVERLAPPED_VWIDTH,    // ExtCheck
   },

   {  // 24 *** 1440 x 900 x 60 Hz ***  //DMT1459 (VG870:54)
      559,	10,   // StdHFreq, (in 100Hz unit)
      600, 10,    // StdVFreq, (in 1/10Hz unit)
      934,	4,    // StdVTotal, (in line unit)
      1904,       // StdHTotal,
      1440, 900,           // StdWidth, StdHeight,
      232 + 152 -(162), 25 + 6,   // StdHBackPorch, StdVBackPorch,
      0,
      gmd_MODE_GRAPHICS | gmd_MODE_NEG_HSYNC,            // ModeType,
      gmd_MODE_OVERLAPPED_VWIDTH,   // ExtCheck
   },

   {  // 25 *** 1600 x 900 x 60 Hz Reduce ***  //CVR1659H (VG870:56)
      555, 10,    // StdHFreq, (in 100Hz unit)
      600, 10,    // StdVFreq, (in 1/10Hz unit)
      926,  4,    // StdVTotal, (in line unit)
      1760,	//2128,       // StdHTotal,
      1600, 900,           // StdWidth, StdHeight,
      80 + 32, 18 + 5,	//232 + 80, 25 + 6,    // StdHBackPorch, StdVBackPorch,
      32,                   // StdVSyncWidth (for overlapped mode)
      gmd_MODE_GRAPHICS | gmd_MODE_NEG_VSYNC,   // ModeType,
      gmd_MODE_OVERLAPPED_VWIDTH,    // ExtCheck
   },

   {  // 26 *** 1440 x 900 x 60 Hz Reduce ***  //DMT1459R (VG870:53)
      555, 10,     // StdHFreq, (in 100Hz unit)
      600, 10,
      926,  4,     // StdVTotal, (in line unit)
      1600,        // StdHTotal,
      1440, 900,           // StdWidth, StdHeight,
      80 + 32 - (4),  17 + 6,    // StdHBackPorch, StdVBackPorch,
      6,                   // StdVSyncWidth (for overlapped mode)
      gmd_MODE_GRAPHICS | gmd_MODE_NEG_VSYNC,   // ModeType,
      0,	//gmd_MODE_OVERLAPPED_VWIDTH,       // ExtCheck
   },

   {  // 27 *** 1280 x 800 x 60 Hz ***
      497, 10,                    // StdHFreq, (in 100Hz unit)
      598, 10,
      831, 4,                     // StdVTotal, (in line unit)
      1680,                // StdHTotal,
      1280, 800,              // StdWidth, StdHeight,
      200, 22,                 // StdHBackPorch, StdVBackPorch,
      0,                // StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS,            // ModeType,
      0,    // ExtCheck
   },

   {  // 28 *** 1280 x 800 x 60 Hz Reduce ***
      493, 10,                    // StdHFreq, (in 100Hz unit)
      599, 10,
      823, 4,                     // StdVTotal, (in line unit)
      1440,                // StdHTotal,
      1280, 800,              // StdWidth, StdHeight,
      80, 14,                  // StdHBackPorch, StdVBackPorch,
      0,                // StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS,            // ModeType,
      0,             // ExtCheck
   },

   {  // 29 *** 1024 x 768 x 60 Hz ***
      483, 10,       // StdHFreq, (in 100Hz unit)
      600, 10,
      806,  4,       // StdVTotal, (in line unit)
      1344,          // StdHTotal,
      1024, 768,     // StdWidth, StdHeight,
      160 + 136 - (146), 29 + 6,   // StdHBackPorch, StdVBackPorch,
      0,                   // StdHSyncWidth (for overlapped mode)
      gmd_MODE_GRAPHICS,           // ModeType,
      0,   // ExtCheck
   },

   {  // 30 *** 1280 x 768 x 60 Hz ***
      477, 10,     // StdHFreq, (in 100Hz unit)
      600, 10,
      798,  2,     // StdVTotal, (in line unit)
      1664,        // StdHTotal,
      1280, 768,           // StdWidth, StdHeight,
      192 + 128,  20 + 7,  // StdHBackPorch, StdVBackPorch,
      0,                   // StdHSyncWidth (for overlapped mode)
      gmd_MODE_GRAPHICS | gmd_MODE_NEG_HSYNC,      // ModeType,
      gmd_MODE_OVERLAPPED_SYNCTYPE,                // ExtCheck
   },

   {  // 28 *** 1366 x 768 x 60 Hz ***
      477, 10,     // StdHFreq, (in 100Hz unit)
      598, 10,
      798,  2,     // StdVTotal, (in line unit)
      1792,        // StdHTotal,
      1366, 768,           // StdWidth, StdHeight,
      213 + 128,  20 + 7,  // StdHBackPorch, StdVBackPorch,
      0,                   // StdHSyncWidth (for overlapped mode)
      gmd_MODE_GRAPHICS,   // ModeType,
      0,                // ExtCheck
   },

   {  // 31 *** 1360 x 768 x 60 Hz ***
      477, 10,     // StdHFreq, (in 100Hz unit)
      600, 10,
      795,  3,      // StdVTotal, (in line unit)
      1792,       // StdHTotal,
      1360, 768,           // StdWidth, StdHeight,
      256 + 112,  18 + 6,  // StdHBackPorch, StdVBackPorch,
      112,                   // StdHSyncWidth (for overlapped mode)
      gmd_MODE_GRAPHICS,            // ModeType,
      gmd_MODE_OVERLAPPED_HWIDTH,   // ExtCheck
   },

   {  // 32 *** 1280 x 768 x 60 Hz - Reduced ***
      473, 10,     // StdHFreq, (in 100Hz unit)
      599, 10,
      790,  4,      // StdVTotal, (in line unit)
      1440,       // StdHTotal,
      1280, 768,           // StdWidth, StdHeight,
      110,  21,  // StdHBackPorch, StdVBackPorch,
      0,                   // StdHSyncWidth (for overlapped mode)
      gmd_MODE_GRAPHICS,      // ModeType,
      0,                      // ExtCheck
   },

   {  // 33 *** 1280 x 720 x 60 Hz ***
      447, 10,     // StdHFreq, (in 100Hz unit)
      600, 10,
      748,  4,     // StdVTotal, (in line unit)
      1664,       // StdHTotal,
      1280, 720,           // StdWidth, StdHeight,
      64 + 128,  3 + 5,    // StdHBackPorch, StdVBackPorch,
      128,                   // StdHSyncWidth (for overlapped mode)
      gmd_MODE_GRAPHICS,            // ModeType,
      gmd_MODE_OVERLAPPED_HWIDTH,   // ExtCheck
   },

   {  // 34 *** 1280 x 720 x 60 Hz ***
      447, 10,     // StdHFreq, (in 100Hz unit)
      600, 10,
      746,  4,      // StdVTotal, (in line unit)
      1664,       // StdHTotal,
      1280, 720,           // StdWidth, StdHeight,
      70 + 40, 5 + 5,      // StdHBackPorch, StdVBackPorch,
      0,                   // StdHSyncWidth (for overlapped mode)
      gmd_MODE_GRAPHICS,      // ModeType,
      0,                      // ExtCheck
   },

   {  // 35 *** 800 x 600 x 60 Hz ***
      378,10,							// StdHFreq, (in 100Hz unit)
      600,10,
      628,4,							// StdVTotal, (in line unit)
      1056,							// StdHTotal,
      800, 600,						// StdWidth, StdHeight,
      88 + 128, 23 + 4,						// StdHBackPorch, StdVBackPorch,
      0,						// StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS,						// ModeType,
      0,				// ExtCheck
   },

   {  // 36 *** 720 x 576 x 60 Hz ***
      358, 10,                    // StdHFreq, (in 100Hz unit)
      600, 10,
      599, 4,                     // StdVTotal, (in line unit)
      912,                    // StdHTotal,
      720, 576,                  // StdWidth, StdHeight,
      24 + 72, 1 + 3,                  // StdHBackPorch, StdVBackPorch,
      0,                // StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS,            // ModeType,
      0,             // ExtCheck
   },

   {  // 37 *** 800 x 600 x 56 Hz ***
      351,10,							// StdHFreq, (in 100Hz unit)
      600,10,
      625,4,							// StdVTotal, (in line unit)
      1024,							// StdHTotal,
      800, 600,						// StdWidth, StdHeight,
      128 + 72, 22 + 2,						// StdHBackPorch, StdVBackPorch,
      0,						// StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS,						// ModeType,
      0,						// ExtCheck
   },

   {  // 38 *** 640 x 480 x 60 Hz ***
      315,10,							// StdHFreq, (in 100Hz unit)
      600,10,
      525,4,							// StdVTotal, (in line unit)
      800,							// StdHTotal,
      640, 480,						// StdWidth, StdHeight,
      48 - (10),  33 + 2,						// StdHBackPorch, StdVBackPorch,
      0,						// StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS,	// ModeType,
      0,//gmd_MODE_OVERLAPPED_HWIDTH,				// ExtCheck
   },

   {  // 39 *** 848 x 480 x 60 Hz ***
      310,10,							// StdHFreq, (in 100Hz unit)
      600,10,
      517,4,							// StdVTotal, (in line unit)
      1088,							// StdHTotal,
      848, 480,						// StdWidth, StdHeight,
      112 + 112, 23 + 8,						// StdHBackPorch, StdVBackPorch,
      0,						// StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS,						// ModeType,
      0,				// ExtCheck
   },

   {  // 40 *** 640 x 500 x 60Hz ***
      312,10,							// StdHFreq, (in 100Hz unit)
      600,10,
      541,4,							// StdVTotal, (in line unit)
      808,							// StdHTotal,
      640, 500,						// StdWidth, StdHeight,
      24 + 64, 28 + 2,						// StdHBackPorch, StdVBackPorch,
      0,						// StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS,				// ModeType,
      0,					// ExtCheck
   },

   {  // 41 *** 848 x 480 x 60 Hz - Reduced ***
      298,10,              // StdHFreq, (in 100Hz unit)
      600,10,
      500,4,               // StdVTotal, (in line unit)
      1056,                // StdHTotal,
      848, 480,            // StdWidth, StdHeight,
      104 + 80, 12 + 5,    // StdHBackPorch, StdVBackPorch,
      0,                   // StdHSyncWidth (for overlapped mode)
      gmd_MODE_GRAPHICS,   // ModeType,
      0,                   // ExtCheck
   },

   {  // 42 *** 1366 x 768 x 60 Hz ***  //DMT1360H
      477, 10,     // StdHFreq, (in 100Hz unit)
      600, 10,
      798,  3,      // StdVTotal, (in line unit)
      1792,       // StdHTotal,
      1366, 768,           // StdWidth, StdHeight,
      213 + 143 - (4),  24 + 3,  // StdHBackPorch, StdVBackPorch,
      143,                   // StdHSyncWidth (for overlapped mode)
      gmd_MODE_GRAPHICS,            // ModeType,
      0,   // ExtCheck
   },

   {  // 43 *** 2048 x 1152R x 60 Hz Reduce *** DMR2060H
      720, 10,          // StdHFreq, (in 100Hz unit)
      600, 10,
      1200, 4,          // StdVTotal, (in line unit)
      2250,             // StdHTotal,
      2048, 1152,       // StdWidth, StdHeight,
      80 + 96, 3 + 44,           // StdHBackPorch, StdVBackPorch,
      80,               // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_GRAPHICS,   // ModeType,
      0,                   // ExtCheck
   },

   {  // 44 *** 2048 x 1152 x 60 Hz ***  CVT2060H
      716, 10,          // StdHFreq, (in 100Hz unit)
      599, 10,
      1195, 4,          // StdVTotal, (in line unit)
      2752,             // StdHTotal,
      2048, 1152,       // StdWidth, StdHeight,
      216 + 352, 3 + 35,           // StdHBackPorch, StdVBackPorch,
      216,               // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_GRAPHICS,   // ModeType,
      0,                   // ExtCheck
   },

   {  // 45 *** 1920 x 1440 x 60 Hz - PC2 ***
      1189,10,              // StdHFreq, (in 100Hz unit)
      598,10,
      1987,4,              // StdVTotal, (in line unit)
      2080,                // StdHTotal,
      1920, 1440,          // StdWidth, StdHeight,
      80,  1 + 3,   // StdHBackPorch, StdVBackPorch,
      0,                   // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_GRAPHICS,   // ModeType,
      0,       // ExtCheck
   },

  {  // 46 *** 2560 x 1440 x 60 Hz - PC2 ***
	 888,10,			   // StdHFreq, (in 100Hz unit)
	 600,10,
	 1481,4,			  // StdVTotal, (in line unit)
	 2720,				  // StdHTotal,
	 2560, 1440,		  // StdWidth, StdHeight,
	 80,  1 + 3,   // StdHBackPorch, StdVBackPorch,
	 0, 				  // StdHpulse (used only in VideoOverlapped)
	 gmd_MODE_GRAPHICS,   // ModeType,
	 0, 	  // ExtCheck
  },

   {  // *** Last Entry ***
      0, 0,    // StdHFreq, (in 100Hz unit)
      0, 0,    // StdVFreq, (in 1/10Hz unit)
      0, 0,    // StdVTotal, (in line unit)
      0,       // StdHTotal,
      0, 0,    // StdWidth, StdHeight,
      0, 0,    // StdHBackPorch, StdVBackPorch,
      0,       // StdHSyncWidth (for overlapped mode)
      0,       // ModeType,
      0,       // ExtCheck
   }
};

//******************************************************************************
//  Stanard Mode table for timing above 70Hz (including 66Hz)
//******************************************************************************
ROM StandardModeStruct StandardModeTable_70[] =
{
   {  // 0 *** 1600 x 1200 x 75 Hz ***
      937,10,     // StdHFreq, (in 100Hz unit)
      750,10,     // StdVFreq, (in 1/10Hz unit)
      1250,4,     // StdVTotal, (in line unit)
      2160,       // StdHTotal,
      1600, 1200,          // StdWidth, StdHeight,
      304 + 192, 46 + 3,   // StdHBackPorch, StdVBackPorch,
      0,                   // StdHSyncWidth (for overlapped mode)
      gmd_MODE_GRAPHICS,   // ModeType,
      0,                   // ExtCheck
   },

   {  // 1 *** 1600 x 1200 x 70 Hz ***
      875,10,     // StdHFreq, (in 100Hz unit)
      700,10,
      1250,4,     // StdVTotal, (in line unit)
      2160,       // StdHTotal,
      1600, 1200,             // StdWidth, StdHeight,
      304 + 192, 46 + 3,      // StdHBackPorch, StdVBackPorch,
      0,                      // StdHSyncWidth (for overlapped mode)
      gmd_MODE_GRAPHICS,      // ModeType,
      0,                      // ExtCheck
   },

   {  // 2 *** 1400 x 1050 x 75 Hz ***
       823,10,                         // StdHFreq, (in 100Hz unit)
       750,10,
       1099,4,                         // StdVTotal, (in line unit)
       1896,                           // StdHTotal,
       1400, 1050,                     // StdWidth, StdHeight,
       248 + 144, 42 + 4,              // StdHBackPorch, StdVBackPorch,
       4,                              // StdHSyncWidth, StdVSyncWidth,
       gmd_MODE_GRAPHICS,              // ModeType,
       gmd_MODE_OVERLAPPED_VWIDTH,     // ExtCheck
    },

   {  // 3 *** 1680 x 1050 x 75 Hz ***
      823,10,                             // StdHFreq, (in 100Hz unit)
      750,10,
      1105,4,                             // StdVTotal, (in line unit)
      2272,                            // StdHTotal,
      1680, 1050,                      // StdWidth, StdHeight,
      296 + 176, 46 + 6,                        // StdHBackPorch, StdVBackPorch,
      0,                         // StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS,                     // ModeType,
      0,                   // ExtCheck
   },

   {  // 4 *** 1680 x 1050 x 75 Hz ***
      823, 10,               					// StdHFreq, (in 100Hz unit)
      749, 10,
      1099, 4,              					// StdVTotal, (in line unit)
      2272,                 				// StdHTotal,
      1680, 1050,            				// StdWidth, StdHeight,
      296 + 176, 40 + 6,              				// StdHBackPorch, StdVBackPorch,
      0,              				// StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS,						// ModeType,
      gmd_MODE_OVERLAPPED_VWIDTH,	//0,       // ExtCheck
   },

   {  // 5 *** 1680 x 1050 x 75 Hz Reduce ***	//Dell Spec Vtotal:1080 line
      815, 10,                            // StdHFreq, (in 100Hz unit)
      749, 10,
      1080, 4,                            // StdVTotal, (in line unit)
      1840,                            // StdHTotal,
      1680, 1050,                      // StdWidth, StdHeight,
      80, 29,                        // StdHBackPorch, StdVBackPorch,
      0,                         // StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS,                  // ModeType,
      0,       // ExtCheck
   },

   {  // 6 *** 1680 x 1050 x 75 Hz Reduce ***		//Astro870 CVR1675D Vtotal:1088 line
      815, 10,                            // StdHFreq, (in 100Hz unit)
      749, 10,
      1088, 4,                            // StdVTotal, (in line unit)
      1840,                            // StdHTotal,
      1680, 1050,                      // StdWidth, StdHeight,
      80, 29,                        // StdHBackPorch, StdVBackPorch,
      0,                         // StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS,                  // ModeType,
      0,       // ExtCheck
   },

   {  // 7 *** 1600 x 1200 x 65 Hz ***
      812,10,               					// StdHFreq, (in 100Hz unit)
      650,10,
      1250,4,              					// StdVTotal, (in line unit)
      2160,                 				// StdHTotal,
      1600, 1200,            				// StdWidth, StdHeight,
      304 + 192, 46 + 3,              				// StdHBackPorch, StdVBackPorch,
      0,              				// StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS,							// ModeType,
      0,							// ExtCheck
   },

   {  // 8 *** 1280 x 1024 x 76 Hz ***
      811,  6,                // StdHFreq, (in 100Hz unit)
      761, 10,
      1066, 4,                // StdVTotal, (in line unit)
      1664,                   // StdHTotal,
      1280, 1024,             // StdWidth, StdHeight,
      32 + 64, 2 + 8,         // StdHBackPorch, StdVBackPorch,
      0,                      // StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS,      // ModeType,
      0,                      // ExtCheck
   },

   {  // 9 *** 1280 x 1024 x 75 Hz ***
      799, 10,                // StdHFreq, (in 100Hz unit)
      750, 10,
      1066, 4,                // StdVTotal, (in line unit)
      1688,                   // StdHTotal,
      1280, 1024,             // StdWidth, StdHeight,
      248 + 144, 38 + 3,      // StdHBackPorch, StdVBackPorch,
      0,                      // StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS,      // ModeType,
      0,                      // ExtCheck
   },

   {  // 10 *** 1600 x 1000 x 75 Hz ***
      783, 10,                    // StdHFreq, (in 100Hz unit)
      748, 10,
      1047, 4,                    // StdVTotal, (in line unit)
      2160,                   // StdHTotal,
      1600, 1000,                // StdWidth, StdHeight,
      280 + 168, 38 + 6,	//168 + 112, 6 + 3,                // StdHBackPorch, StdVBackPorch,
      0,                // StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS,                  // ModeType,
      0,                // ExtCheck
   },

   {  // 11 *** 1280 x 1024 x 72 Hz ***  //Add for Dell spec
      779, 10,                    // StdHFreq, (in 100Hz unit)
      720, 10,
      1082, 4,                    // StdVTotal, (in line unit)
      1728,                   // StdHTotal,
      1280, 1024,                // StdWidth, StdHeight,
      224 + 136, 52 + 5,                 // StdHBackPorch, StdVBackPorch,
      0,                   // StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS,                  // ModeType,
      0,          // ExtCheck
   },

   {  // 12 *** 1280 x 1024 x 72 Hz ***
      768, 10,                    // StdHFreq, (in 100Hz unit)
      720, 10,
      1067, 4,                    // StdVTotal, (in line unit)
      1728,                   // StdHTotal,
      1280, 1024,                // StdWidth, StdHeight,
      88 + 136, 1 + 5,                 // StdHBackPorch, StdVBackPorch,
      0,                   // StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS,                  // ModeType,
      0,          // ExtCheck
   },

   {  // 13 *** 1280 x 1024 x 70 Hz ***
      746, 10,                    // StdHFreq, (in 100Hz unit)
      700, 10,
      1066, 4,                    // StdVTotal, (in line unit)
      1728,                   // StdHTotal,
      1280, 1024,                // StdWidth, StdHeight,
      224 + 136, 7 + 36,                  // StdHBackPorch, StdVBackPorch,
      0,                   // StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS,                  // ModeType,
      0,          // ExtCheck
   },

   {  // 14 *** 1280 x 1024 x 70 Hz ***	//DMT1270G
      749, 10,                    // StdHFreq, (in 100Hz unit)
      699, 10,
      1072, 4,                    // StdVTotal, (in line unit)
      1696,                   // StdHTotal,
      1280, 1024,                // StdWidth, StdHeight,
      224 + 160, 42 + 4,                  // StdHBackPorch, StdVBackPorch,
      0,                   // StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS,                  // ModeType,
      0,          // ExtCheck
   },

   {  // 15 *** 1152 x 900 x 76 Hz ***
      715, 10,               					// StdHFreq, (in 100Hz unit)
      760, 10,
      940,  4,              					// StdVTotal, (in line unit)
      1552,                 				// StdHTotal,
      1152, 900,            				// StdWidth, StdHeight,
      208 + 96, 33 + 8,              				// StdHBackPorch, StdVBackPorch,
      0,              				// StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS,							// ModeType,
      gmd_MODE_OVERLAPPED_VWIDTH,							// ExtCheck
   },

   {  // 16 *** 1440 x 900 x 75 Hz ***
      706, 10,               					// StdHFreq, (in 100Hz unit)
      750, 10,
      942,  4,              					// StdVTotal, (in line unit)
      1936,                 				// StdHTotal,
      1440, 900,            				// StdWidth, StdHeight,
      248 + 152,  33 + 6,              				// StdHBackPorch, StdVBackPorch,
      0,              				// StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS,						// ModeType,
      gmd_MODE_OVERLAPPED_VWIDTH,					// ExtCheck
   },

   {  // 17 *** 1152 x 870 x 75 Hz MAC ***  //Add for Dell Spec
      687, 10,               					// StdHFreq, (in 100Hz unit)
      750, 10,
      915,  4,              					// StdVTotal, (in line unit)
      1456,                 				// StdHTotal,
      1152, 870,            				// StdWidth, StdHeight,
      144 + 128,  39 + 3,             				// StdHBackPorch, StdVBackPorch,
      0,             				// StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS | gmd_MODE_NEG_HSYNC | gmd_MODE_NEG_VSYNC,							// ModeType,
      gmd_MODE_OVERLAPPED_SYNCTYPE,					// ExtCheck
   },

   {  // 18 *** 1152 x 870 x 75 Hz MAC ***
      681, 10,               					// StdHFreq, (in 100Hz unit)
      750, 10,
      908,  4,              					// StdVTotal, (in line unit)
      1552,                 				// StdHTotal,
      1152, 870,            				// StdWidth, StdHeight,
      200 + 128, 34 + 3,	//144 + 128,  39 + 3,             				// StdHBackPorch, StdVBackPorch,
      0,             				// StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS | gmd_MODE_NEG_HSYNC | gmd_MODE_NEG_VSYNC,							// ModeType,
      gmd_MODE_OVERLAPPED_SYNCTYPE,					// ExtCheck
   },

   {  // 19 *** 1152 x 864 x 75 Hz ***
      675, 10,                             // StdHFreq, (in 100Hz unit)
      750, 10,
      900,  4,                           // StdVTotal, (in line unit)
      1600,                            // StdHTotal,
      1152, 864,                       // StdWidth, StdHeight,
      256 + 128, 32 + 3,                        // StdHBackPorch, StdVBackPorch,
      0,                         // StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS,                     // ModeType,
      0,                   // ExtCheck
   },

   {  // 20 *** 1152 x 864 x 70 Hz ***
      630, 10,                    // StdHFreq, (in 100Hz unit)
      700, 10,
      900,  4,                     // StdVTotal, (in line unit)
      1536,                   // StdHTotal,
      1152, 864,                 // StdWidth, StdHeight,
      32 + 96, 1 + 3,                  // StdHBackPorch, StdVBackPorch,
      0,                // StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS,                  // ModeType,
      0,                // ExtCheck
   },

   {  // 21 *** 1152 x 864 x 70 Hz ***	//DMT1170
      639, 10,                    // StdHFreq, (in 100Hz unit)
      700, 10,
      912,  4,                     // StdVTotal, (in line unit)
      1480,                   // StdHTotal,
      1152, 864,                 // StdWidth, StdHeight,
      200 + 96, 44 + 3,                  // StdHBackPorch, StdVBackPorch,
      0,                // StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS,                  // ModeType,
      0,                // ExtCheck
   },

   {  // 22 *** 1280 x 800 x 75 Hz ***
      628, 10,                    // StdHFreq, (in 100Hz unit)
      749, 10,
      838,  4,                     // StdVTotal, (in line unit)
      1696,                   // StdHTotal,
      1280, 800,                 // StdWidth, StdHeight,
      80 + 136, 1 + 3,                 // StdHBackPorch, StdVBackPorch,
      0,                // StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS,                  // ModeType,
      0,                // ExtCheck
   },

   {  // 23 *** 1152 x 900 x 66 Hz ***
      617, 10,               					// StdHFreq, (in 100Hz unit)
      660, 10,
      935,  4,              					// StdVTotal, (in line unit)
      1536,                 				// StdHTotal,
      1152, 900,            				// StdWidth, StdHeight,
      208 + 128, 31 + 4,              				// StdHBackPorch, StdVBackPorch,
      0,              				// StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS,							// ModeType,
      0,							// ExtCheck
   },

   {  // 24 *** 1024 x 768 x 75 Hz ***	//DMT1075A
      602, 10,               					// StdHFreq, (in 100Hz unit)
      749, 10,
      804,  1,              					// StdVTotal, (in line unit)
      1328,                 				// StdHTotal,
      1024, 768,            				// StdWidth, StdHeight,
      176 + 96, 30 + 3,              				// StdHBackPorch, StdVBackPorch,
      112,              				// StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS |gmd_MODE_NEG_HSYNC |gmd_MODE_NEG_VSYNC,							// ModeType,
      gmd_MODE_OVERLAPPED_SYNCTYPE,	//0,					// ExtCheck
   },
   
   {  // 25 *** 1280 x 768 x 75 Hz ***
      603, 10,               					// StdHFreq, (in 100Hz unit)
      749, 10,
      805,  2,              					// StdVTotal, (in line unit)
      1696,                 				// StdHTotal,
      1280, 768,            				// StdWidth, StdHeight,
      208 + 128, 27 + 7,              				// StdHBackPorch, StdVBackPorch,
      0,              				// StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS | gmd_MODE_NEG_HSYNC,				// ModeType,
      0,		// ExtCheck
   },

   {  // 26 *** 1024 x 768 x 75 Hz ***	//DMT1075
      600, 10,                            // StdHFreq, (in 100Hz unit)
      750, 10,
      800,  4,                            // StdVTotal, (in line unit)
      1312,                            // StdHTotal,
      1024, 768,                       // StdWidth, StdHeight,
      176 + 96, 28 + 3,                         // StdHBackPorch, StdVBackPorch,
      0,                         // StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS,                     // ModeType,
      0,             // ExtCheck
   },

   {  // 27 *** 1280 x 800 x 72 Hz ***
      600, 10,                    // StdHFreq, (in 100Hz unit)
      720, 10,
      834,  4,                     // StdVTotal, (in line unit)
      1712,                   // StdHTotal,
      1280, 800,                 // StdWidth, StdHeight,
      80 + 136, 1 + 3,                 // StdHBackPorch, StdVBackPorch,
      0,                // StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS,                  // ModeType,
      0,                // ExtCheck
   },

   {  // 28 *** 1280 x 800 x 70 Hz ***
      583, 10,                    // StdHFreq, (in 100Hz unit)
      700, 10,
      833,  4,                     // StdVTotal, (in line unit)
      1696,                   // StdHTotal,
      1280, 800,                 // StdWidth, StdHeight,
      80 + 136, 1 + 3,                 // StdHBackPorch, StdVBackPorch,
      0,                // StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS,                  // ModeType,
      0,                // ExtCheck
   },

   {  // 29 *** 1024 x 768 x 72 Hz ***
      576, 10,               					// StdHFreq, (in 100Hz unit)
      720, 10,
      801,  4,              					// StdVTotal, (in line unit)
      1360,                  				// StdHTotal,
      1024, 768,            				// StdWidth, StdHeight,
      150 + 100,  18 + 2,              				// StdHBackPorch, StdVBackPorch,
      0,              				// StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS,							// ModeType,
      0,							// ExtCheck
   },

   {  // 30 *** 1024 x 768 x 70 Hz ***
      564, 10,               					// StdHFreq, (in 100Hz unit)
      701, 10,
      806,  4,              					// StdVTotal, (in line unit)
      1328,                 				// StdHTotal,
      1024, 768,            				// StdWidth, StdHeight,
      114 + 136,  29 + 6,              				// StdHBackPorch, StdVBackPorch,
      0,              				// StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS,		// ModeType,
      0,							// ExtCheck
   },

   {  // 31 *** 1280 x 720 x 75 Hz ***
      562, 10,                    // StdHFreq, (in 100Hz unit)
      748, 10,
      752,  4,                     // StdVTotal, (in line unit)
      1696,                   // StdHTotal,
      1280, 720,                 // StdWidth, StdHeight,
      72 + 136, 1 + 3,                 // StdHBackPorch, StdVBackPorch,
      0,                // StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS,                  // ModeType,
      0,          // ExtCheck
   },

   {  // 32 *** 832 x 624 x 75 Hz *** //Add for Dell spec
	497, 10,                                                  // StdHFreq, (in 100Hz unit)
	745, 10,
	667, 4,                                     // StdVTotal, (in line unit)
	1152,                                                      // StdHTotal,
	832, 624,                                                                                                // StdWidth, StdHeight,
	224 + 64, 39 + 3,                 // StdHBackPorch, StdVBackPorch,
	0,
	gmd_MODE_GRAPHICS,                                                                                                // ModeType,
	0,                                                              // ExtCheck
   },

   {  // 33 *** 832 x 624 x 75 Hz ***
      489, 10,             // StdHFreq, (in 100Hz unit)
      749, 10,
      654, 4,     			// StdVTotal, (in line unit)
      1088,                // StdHTotal,
      832, 624,            				// StdWidth, StdHeight,
      224 + 64, 39 + 3,    // StdHBackPorch, StdVBackPorch,
      0,
      gmd_MODE_GRAPHICS,							// ModeType,
      0,					// ExtCheck
   },

   {  // 34 *** 800 x 600 x 72 Hz  ***
      481, 10,                             // StdHFreq, (in 100Hz unit)
      722, 10,
      666,  4,                           // StdVTotal, (in line unit)
      1040,                            // StdHTotal,
      800, 600,                        // StdWidth, StdHeight,
      64 + 120,  23 + 6,                        // StdHBackPorch, StdVBackPorch,
      0,                         // StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS,                     // ModeType,
      0,            // ExtCheck
   },

   {  // 35 *** 800 x 600 x 75 Hz ***
      468, 10,                             // StdHFreq, (in 100Hz unit)
      750, 10,
      625,  4,                           // StdVTotal, (in line unit)
      1056,                            // StdHTotal,
      800, 600,                        // StdWidth, StdHeight,
      160 + 80,  21 + 3,                        // StdHBackPorch, StdVBackPorch,
      0,                         // StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS,                     // ModeType,
      0,                   // ExtCheck
   },

   {  // 36 *** 640 x 480 x 72 Hz ***
      378, 10,                             // StdHFreq, (in 100Hz unit)
      720, 10,
      520,  4,                           // StdVTotal, (in line unit)
      832,                             // StdHTotal,
      640, 480,                        // StdWidth, StdHeight,
      128 + 40 - (50), 28 + 3,                         // StdHBackPorch, StdVBackPorch,
      0,                         // StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS | gmd_MODE_NEG_HSYNC | gmd_MODE_NEG_VSYNC,      // ModeType,
      0,                   // ExtCheck
   },

   {  // 37 *** 848 x 480 x 72 Hz ***
      360, 10,               // StdHFreq, (in 100Hz unit) // Sometimes 640x480@75 is detected as 374
      720, 10,
      501,  4,                           // StdVTotal, (in line unit)
      1088,                            // StdHTotal,
      848, 480,                        // StdWidth, StdHeight,
      32 + 88, 1 + 3,                        // StdHBackPorch, StdVBackPorch,
      0,                // StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS,                  // ModeType,
      0,            // ExtCheck
   },

   {  // 38 *** 848 x 480 x 75 Hz ***
      377, 10,                             // StdHFreq, (in 100Hz unit)
      748, 10,
      504,  1,                           // StdVTotal, (in line unit)
      1088,                            // StdHTotal,
      848, 480,                        // StdWidth, StdHeight,
      40 + 80, 3 + 5,                        // StdHBackPorch, StdVBackPorch,
      0,                   // StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS | gmd_MODE_NEG_HSYNC,                  // ModeType,
      gmd_MODE_OVERLAPPED_SYNCTYPE,                  // ExtCheck
   },

   {  // 39 *** 640 x 480 x 75 Hz ***
      375, 10,                             // StdHFreq, (in 100Hz unit)
      750, 10,
      500,  4,                           // StdVTotal, (in line unit)
      840,                             // StdHTotal,
      640, 480,                        // StdWidth, StdHeight,
      120 + 64 - (74), 16 + 3,                         // StdHBackPorch, StdVBackPorch,
      0,                         // StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS,      // ModeType,
      0,          // ExtCheck
   },

   {  // 40 *** 640 x 480 x 66 Hz ***
      350, 10,                             // StdHFreq, (in 100Hz unit)
      667, 10,
      525,  4,                           // StdVTotal, (in line unit)
      864,                             // StdHTotal,
      640, 480,                        // StdWidth, StdHeight,
      96 + 64 -(74), 39 + 3,                          // StdHBackPorch, StdVBackPorch,
      0,                         // StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS,                     // ModeType,
      0,                   // ExtCheck
   },

   {  // 41 *** 848 x 480 x 70 Hz ***
      350, 10,                             // StdHFreq, (in 100Hz unit)
      700, 10,
      500,  4,                           // StdVTotal, (in line unit)
      1072,                            // StdHTotal,
      848, 480,                        // StdWidth, StdHeight,
      24 + 88,  1 + 3,                          // StdHBackPorch, StdVBackPorch,
      0,
      gmd_MODE_GRAPHICS,                  // ModeType,
      0,          // ExtCheck
   },

   {  // 42 *** 720 x 400 x 70 Hz ***
      315, 10,                             // StdHFreq, (in 100Hz unit)
      700, 10,              // StdVFreq, (in 1/10Hz unit)
      449,  4,                           // StdVTotal, (in line unit)
      900,                 // StdHTotal,
      720, 400,            // StdWidth, StdHeight,
      54 + 108 - (126), 35 + 2 - (8),    // StdHBackPorch, StdVBackPorch,
      108,                   // StdHSyncWidth (for overlapped mode)
      gmd_MODE_GRAPHICS | gmd_MODE_NEG_HSYNC,   // ModeType,
      gmd_MODE_OVERLAPPED_SYNCTYPE,             // ExtCheck
   },

   {  // 43 *** 640 x 400 x 70 Hz ***
      315, 10,              // StdHFreq, (in 100Hz unit)
      701, 10,
      449, 4,               // StdVTotal, (in line unit)
      800,                             // StdHTotal,
      640, 400,                        // StdWidth, StdHeight,
      48 + 96 - (106), 35 + 2,                          // StdHBackPorch, StdVBackPorch,
      96,                         // StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS | gmd_MODE_NEG_HSYNC,   // ModeType,
      gmd_MODE_OVERLAPPED_SYNCTYPE,                   // ExtCheck
   },

   {  // 44 *** 640 x 350 x 70 Hz ***
      315, 10,          // StdHFreq, (in 100Hz unit)
      700, 10,
      449, 4,           // StdVTotal, (in line unit)
      800,              // StdHTotal,
      640, 350,         // StdWidth, StdHeight,
      96 + 64 - (20),  64 + 3 -(6), // StdHBackPorch, StdVBackPorch,
      96,
      gmd_MODE_GRAPHICS | gmd_MODE_NEG_VSYNC,   // ModeType,
      0,          // ExtCheck
   },

   {  // *** Last Entry ***
      0, 0,
      0, 0,
      0, 0,
      0,
      0, 0,
      0, 0,
      0,
      0,
      0,
   }
};

//******************************************************************************
//  Stanard Mode table for timing above 85Hz
//******************************************************************************
ROM StandardModeStruct StandardModeTable_85[] =
{
   {  // 0 *** 1400 x 1050 x 85 Hz ***
      939, 10,             // StdHFreq, (in 100Hz unit)
      850, 10,             // StdVFreq, (in 1/10Hz unit)
      1105, 4,             // StdVTotal, (in line unit)
      1912,                // StdHTotal,
      1400, 1050,          // StdWidth, StdHeight,
      256 + 152, 48 + 4,   // StdHBackPorch, StdVBackPorch,
      0,                   // StdHSyncWidth (for overlapped mode)
      gmd_MODE_GRAPHICS,   // ModeType,
      gmd_MODE_OVERLAPPED_VWIDTH,                   // ExtCheck
   },

   {  // 1 *** 1280 x 1024 x 85 Hz ***
      911,10,              // StdHFreq, (in 100Hz unit)
      850,10,
      1072,4,              // StdVTotal, (in line unit)
      1728,                // StdHTotal,
      1280, 1024,          // StdWidth, StdHeight,
      224 + 160, 44 + 3,   // StdHBackPorch, StdVBackPorch,
      0,                   // StdHSyncWidth (for overlapped mode)
      gmd_MODE_GRAPHICS,   // ModeType,
      0,							// ExtCheck
   },

   {  // 2 *** 1280 x 960 x 85 Hz ***
      859,10,               					// StdHFreq, (in 100Hz unit)
      850,10,
      1011,4,              					// StdVTotal, (in line unit)
      1728,                 				// StdHTotal,
      1280, 960,            				// StdWidth, StdHeight,
      224 + 160, 47 + 3,              				// StdHBackPorch, StdVBackPorch,
      0,              				// StdHSyncWidth (for overlapped mode)
      gmd_MODE_GRAPHICS,							// ModeType,
      0,							// ExtCheck
   },

   {  // 3 *** 1440 x 900 x 85 Hz ***
      804, 10,               					// StdHFreq, (in 100Hz unit)
      848, 10,
      948,  4,              					// StdVTotal, (in line unit)
      1952,                 				// StdHTotal,
      1440, 900,            				// StdWidth, StdHeight,
      256 + 152, 39 + 6,              				// StdHBackPorch, StdVBackPorch,
      0,              				// StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS,						// ModeType,
      0,							// ExtCheck
   },

   {  // 4 *** 1152 x 864 x 85 Hz ***
      771, 10,                             // StdHFreq, (in 100Hz unit)
      848, 10,
      910,  4,                           // StdVTotal, (in line unit)
      1552,                             // StdHTotal,
      1152, 864,                       // StdWidth, StdHeight,
      64 + 128, 1 + 3,                          // StdHBackPorch, StdVBackPorch,
      0,                         // StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS | gmd_MODE_NEG_HSYNC,                     // ModeType,
      0,             // ExtCheck
   },

   {  // 5 *** 1280 x 800 x 85 Hz ***
      716, 10,                    // StdHFreq, (in 100Hz unit)
      849, 10,
      843,  4,                     // StdVTotal, (in line unit)
      1712,                   // StdHTotal,
      1280, 800,                 // StdWidth, StdHeight,
      80 + 136, 3 + 6,                 // StdHBackPorch, StdVBackPorch,
      0,                // StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS,                  // ModeType,
      0,                // ExtCheck
   },

   {  // 6 *** 1280 x 768 x 85 Hz ***
      686, 10,                            // StdHFreq, (in 100Hz unit)
      848, 10,
      809,  4,                            // StdVTotal, (in line unit)
      1712,                            // StdHTotal,
      1280, 768,                       // StdWidth, StdHeight,
      208 + 96, 36 + 3,                         // StdHBackPorch, StdVBackPorch,
      0,                         // StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS | gmd_MODE_NEG_HSYNC,                     // ModeType,
      gmd_MODE_OVERLAPPED_SYNCTYPE,             // ExtCheck
   },

   {  // 7 *** 1024 x 768 x 85 Hz ***
      686, 10,               					// StdHFreq, (in 100Hz unit)
      850, 10,
      808,  4,              					// StdVTotal, (in line unit)
      1376,                 				// StdHTotal,
      1024, 768,            				// StdWidth, StdHeight,
      208 + 96, 36 + 3,              				// StdHBackPorch, StdVBackPorch,
      0,              				// StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS,							// ModeType,
      0,	//gmd_MODE_OVERLAPPED_SYNCTYPE,					// ExtCheck
   },

   {  // 8 *** 800 x 600 x 85 Hz  ***
      537, 10,             // StdHFreq, (in 100Hz unit)
      850, 10,
      631,  4,               // StdVTotal, (in line unit)
      1048,                // StdHTotal,
      800, 600,            // StdWidth, StdHeight,
      210,  27,            // StdHBackPorch, StdVBackPorch,
      0,                   // StdHSyncWidth (for overlapped mode)
      gmd_MODE_GRAPHICS,   // ModeType,
      0,                   // ExtCheck
   },

   {  // 9 *** 848 x 480 x 85 Hz ***
      429, 10,                             // StdHFreq, (in 100Hz unit)
      847, 10,
      507,  4,                           // StdVTotal, (in line unit)
      1088,                             // StdHTotal,
      848, 480,                        // StdWidth, StdHeight,
      120 + 80, 19+ 5,                          // StdHBackPorch, StdVBackPorch,
      0,                         // StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS ,      // ModeType,
      gmd_MODE_OVERLAPPED_VWIDTH,                   // ExtCheck
   },
   
   {  // 10 *** 640 x 480 x 85 Hz ***
      432, 10,                             // StdHFreq, (in 100Hz unit)
      850, 10,
      509,  4,                           // StdVTotal, (in line unit)
      832,                             // StdHTotal,
      640, 480,                        // StdWidth, StdHeight,
      80 + 56, 25 + 3,                          // StdHBackPorch, StdVBackPorch,
      0,                         // StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS ,      // ModeType,
      0,                   // ExtCheck
   },
   
   {  // 11 *** 720 x 400 x 85 Hz ***
      379, 10,                // StdHFreq, (in 100Hz unit)
      850, 10,
      446,  4,                // StdVTotal, (in line unit)
      936,                    // StdHTotal,
      720, 400,               // StdWidth, StdHeight,
      108 + 72, 42 + 3,       // StdHBackPorch, StdVBackPorch,
      72,                     // StdHSyncWidth (for overlapped mode)
      gmd_MODE_GRAPHICS | gmd_MODE_NEG_HSYNC,      // ModeType,
      gmd_MODE_OVERLAPPED_SYNCTYPE|gmd_MODE_OVERLAPPED_GR640_GR720,	//gmd_MODE_OVERLAPPED_SYNCTYPE,      // ExtCheck
   },

   {  // 12 *** 640 x 400 x 85 Hz ***
      378, 10,                // StdHFreq, (in 100Hz unit)
      850, 10,
      445,  4,                // StdVTotal, (in line unit)
      832,                    // StdHTotal,
      640, 400,               // StdWidth, StdHeight,
      96 + 64,  42 + 3,       // StdHBackPorch, StdVBackPorch,
      64,                     // StdHSyncWidth (for overlapped mode)
      gmd_MODE_GRAPHICS | gmd_MODE_NEG_HSYNC,      // ModeType,
      gmd_MODE_OVERLAPPED_SYNCTYPE,	//0,       // ExtCheck
   },

   {  // 13 *** 640 x 350 x 85 Hz ***
      378, 10,                // StdHFreq, (in 100Hz unit)
      850, 10,
      445,  4,                // StdVTotal, (in line unit)
      832,                    // StdHTotal,
      640, 350,               // StdWidth, StdHeight,
      96 + 64,  60 + 3,       // StdHBackPorch, StdVBackPorch,
      0,                      // StdHSyncWidth (for overlapped mode)
      gmd_MODE_GRAPHICS | gmd_MODE_NEG_VSYNC,   // ModeType,
      0,	//gmd_MODE_OVERLAPPED_SYNCTYPE,	//gmd_MODE_OVERLAPPED_GR640_GR720,	//gmd_MODE_OVERLAPPED_SYNCTYPE,             // ExtCheck
   },

   {  // 14 *** 1024 x 768 - 43 ***
      355, 10,          // StdHFreq, (in 100Hz unit)
      868, 10,
      408,  4,          // StdVTotal, (in line unit)
      1264,             // StdHTotal,
      1024, 384,        // StdWidth, StdHeight,
      56 + 176,  20 + 4,   // StdHBackPorch, StdVBackPorch,
      0,                   // StdHSyncWidth (for overlapped mode)
      gmd_MODE_GRAPHICS | gmd_MODE_INTERLACED,  // ModeType,
      0,                   // ExtCheck
   },

   {  // 15 *** 1680 x 1050 - 85 ***
      938, 10,          // StdHFreq, (in 100Hz unit)
      849, 10,
      1105,  4,          // StdVTotal, (in line unit)
      2288,             // StdHTotal,
      1680, 1050,        // StdWidth, StdHeight,
      304 + 176,  46 + 6,   // StdHBackPorch, StdVBackPorch,
      0,                   // StdHSyncWidth (for overlapped mode)
      gmd_MODE_GRAPHICS |gmd_MODE_NEG_HSYNC,  // ModeType,
      0,                   // ExtCheck
   },

   {  // 16 *** 1920 x 1080 x 85 Hz ***
      965, 10,                             // StdHFreq, (in 100Hz unit)
      848, 10,
      1137,  4,                           // StdVTotal, (in line unit)
      2624,                             // StdHTotal,
      1920, 1080,                       // StdWidth, StdHeight,
      352 + 208, 49 + 5,                          // StdHBackPorch, StdVBackPorch,
      0,                         // StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS | gmd_MODE_NEG_HSYNC,                     // ModeType,
      0,             // ExtCheck
   },

   {  // 17 *** 1920 x 1080 x 100 Hz ***		CR2D1910
      1132, 10,                             // StdHFreq, (in 100Hz unit)
      999, 10,
      1133,  4,                           // StdVTotal, (in line unit)
      2080,                             // StdHTotal,
      1920, 1080,                       // StdWidth, StdHeight,
      80 + 32, 45 + 5,                          // StdHBackPorch, StdVBackPorch,
      0,                         // StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS | gmd_MODE_NEG_VSYNC,                     // ModeType,
      0,             // ExtCheck
   },

   {  // 18 *** 1920 x 1080 x 110 Hz ***		CR2D1911
      1251, 10,                             // StdHFreq, (in 100Hz unit)
      1110, 10,
      1138,  4,                           // StdVTotal, (in line unit)
      2080,                             // StdHTotal,
      1920, 1080,                       // StdWidth, StdHeight,
      80 + 32, 50 + 5,                          // StdHBackPorch, StdVBackPorch,
      0,                         // StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS | gmd_MODE_NEG_VSYNC,                     // ModeType,
      0,             // ExtCheck
   },

   {  // 19 *** 1920 x 1080 x 120 Hz ***		CR2D1912
      1373, 10,                             // StdHFreq, (in 100Hz unit)
      1200, 10,
      1144,  4,                           // StdVTotal, (in line unit)
      2080,                             // StdHTotal,
      1920, 1080,                       // StdWidth, StdHeight,
      80 + 32, 56 + 5,                          // StdHBackPorch, StdVBackPorch,
      0,                         // StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS | gmd_MODE_NEG_VSYNC,                     // ModeType,
      0,             // ExtCheck
   },

   {  // 20 *** 1920 x 1080 x 100 Hz ***		CR3D1910
      1137, 10,                             // StdHFreq, (in 100Hz unit)
      999, 10,
      1138,  4,                           // StdVTotal, (in line unit)
      2080,                             // StdHTotal,
      1920, 1080,                       // StdWidth, StdHeight,
      80 + 32, 50 + 5,                          // StdHBackPorch, StdVBackPorch,
      0,                         // StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS | gmd_MODE_NEG_VSYNC,                     // ModeType,
      0,             // ExtCheck
   },

     {  // 21 *** 1920 x 1080 x 110 Hz ***		CR3D1911
      1257, 10,                             // StdHFreq, (in 100Hz unit)
      1110, 10,
      1143,  4,                           // StdVTotal, (in line unit)
      2080,                             // StdHTotal,
      1920, 1080,                       // StdWidth, StdHeight,
      80 + 32, 55 + 5,                          // StdHBackPorch, StdVBackPorch,
      0,                         // StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS | gmd_MODE_NEG_VSYNC,                     // ModeType,
      0,             // ExtCheck
   },

   {  // 22 *** 1920 x 1080 x 120 Hz ***		CR3D1912
      1379, 10,                             // StdHFreq, (in 100Hz unit)
      1200, 10,
      1149,  4,                           // StdVTotal, (in line unit)
      2080,                             // StdHTotal,
      1920, 1080,                       // StdWidth, StdHeight,
      80 + 32, 61 + 5,                          // StdHBackPorch, StdVBackPorch,
      0,                         // StdHSyncWidth, StdVSyncWidth,
      gmd_MODE_GRAPHICS | gmd_MODE_NEG_VSYNC,                     // ModeType,
      0,             // ExtCheck
   },

   {  // *** Last Entry ***
      0, 0,
      0, 0,
      0, 0,
      0,
      0, 0,
      0, 0,
      0,
      0,
      0,
   }
};

//******************************************************************************
//  Stanard Mode table for timing Out of Range
//******************************************************************************
ROM StandardModeStruct StandardModeTable_OOR[] =
{
   {
      // 0 *** 1856 x 1392 x 75 Hz *** 288MHz
      // 0 *** 1920 x 1440 x 75 Hz *** 297MHz
      1125, 10,            // StdHFreq, (in 100Hz unit)
      750,  10,            // StdVFreq, (in 1/10Hz unit)
      1500,  5,            // StdVTotal, (in line unit)
      2560,                // StdHTotal,
      1920, 1440,          // StdWidth, StdHeight,
      352,  104,           // StdHBackPorch, StdVBackPorch,
      0,                   // StdSyncWidth (for overlapped mode)
      gmd_MODE_GRAPHICS,   // ModeType,
      0,                   // ExtCheck
   },

   {  // 1 *** 1920 x 1200 x 85 Hz *** 281.25MHz
      1072, 10,            // StdHFreq, (in 100Hz unit)
      850,  10,            // StdVFreq, (in 1/10Hz unit)
      1262,  4,            // StdVTotal, (in line unit)
      2624,                // StdHTotal,
      1920, 1200,          // StdWidth, StdHeight,
      352,  53,            // StdHBackPorch, StdVBackPorch,
      0,                   // StdSyncWidth (for overlapped mode)
      gmd_MODE_GRAPHICS,   // ModeType,
      0,                   // ExtCheck
   },

   {  // 2 *** 1792 x 1344 x 75 Hz *** 261MHz
      1062, 10,            // StdHFreq, (in 100Hz unit)
      750,  10,            // StdVFreq, (in 1/10Hz unit)
      1417,  4,            // StdVTotal, (in line unit)
      2456,                // StdHTotal,
      1792, 1344,          // StdWidth, StdHeight,
      352,  69,            // StdHBackPorch, StdVBackPorch,
      0,                   // StdSyncWidth (for overlapped mode)
      gmd_MODE_GRAPHICS,   // ModeType,
      0,                   // ExtCheck
   },


   {  // 3 *** 1600 x 1200 x 85 Hz *** 229.5MHz
      1062, 10,            // StdHFreq, (in 100Hz unit)
      850,  10,            // StdVFreq, (in 1/10Hz unit)
      1250,  4,            // StdVTotal, (in line unit)
      2160,                // StdHTotal,
      1600, 1200,          // StdWidth, StdHeight,
      304,  46,            // StdHBackPorch, StdVBackPorch,
      0,                   // StdSyncWidth (for overlapped mode)
      gmd_MODE_GRAPHICS,   // ModeType,
      0,                   // ExtCheck
   },

   {  // 4 *** 2560 x 1600 x 60 Hz *** 348.5MHz
      995, 10,             // StdHFreq, (in 100Hz unit)
      600, 10,             // StdVFreq, (in 1/10Hz unit)
      1658, 4,             // StdVTotal, (in line unit)
      3504,                // StdHTotal,
      2560, 1600,          // StdWidth, StdHeight,
      472,  49,            // StdHBackPorch, StdVBackPorch,
      0,                   // StdSyncWidth (for overlapped mode)
      gmd_MODE_GRAPHICS,   // ModeType,
      0,                   // ExtCheck
   },

   {  // 5 *** 2560 x 1600 x 60 Hz Reduce *** 268.5MHz
      987, 10,             // StdHFreq, (in 100Hz unit)
      600, 10,             // StdVFreq, (in 1/10Hz unit)
      1646, 4,             // StdVTotal, (in line unit)
      2720,                // StdHTotal,
      2560, 1600,          // StdWidth, StdHeight,
      80,  37,             // StdHBackPorch, StdVBackPorch,
      0,                   // StdSyncWidth (for overlapped mode)
      gmd_MODE_GRAPHICS,   // ModeType,
      0,                   // ExtCheck
   },

   {  // 6 *** 2048 x 1536 x 60 Hz *** 267.25MHz
      954, 10,             // StdHFreq, (in 100Hz unit)
      600, 10,             // StdVFreq, (in 1/10Hz unit)
      1592, 4,             // StdVTotal, (in line unit)
      2800,                // StdHTotal,
      2048, 1536,          // StdWidth, StdHeight,
      376,  49,            // StdHBackPorch, StdVBackPorch,
      0,                   // StdSyncWidth (for overlapped mode)
      gmd_MODE_GRAPHICS,   // ModeType,
      0,                   // ExtCheck
   },

   {  // 7 *** 2048 x 1536 x 60 Hz Reduce *** 209.25MHz
      947, 10,             // StdHFreq, (in 100Hz unit)
      600, 10,             // StdVFreq, (in 1/10Hz unit)
      1580, 4,             // StdVTotal, (in line unit)
      2800,                // StdHTotal,
      2048, 1536,          // StdWidth, StdHeight,
      80,  37,             // StdHBackPorch, StdVBackPorch,
      0,                   // StdSyncWidth (for overlapped mode)
      gmd_MODE_GRAPHICS,   // ModeType,
      0,                   // ExtCheck
   },

   {  // 8 *** 1680 x 1050 x 85 Hz *** 214.75MHz
      939, 10,             // StdHFreq, (in 100Hz unit)
      850, 10,             // StdVFreq, (in 1/10Hz unit)
      1105, 4,             // StdVTotal, (in line unit)
      2288,                // StdHTotal,
      1680, 1050,          // StdWidth, StdHeight,
      304,  46,            // StdHBackPorch, StdVBackPorch,
      0,                   // StdSyncWidth (for overlapped mode)
      gmd_MODE_GRAPHICS,   // ModeType,
      0,                   // ExtCheck
   },

   {  // 9 *** 1920 x 1440 x 60 Hz *** 234MHz
      900, 10,             // StdHFreq, (in 100Hz unit)
      600, 10,             // StdVFreq, (in 1/10Hz unit)
      1500, 4,             // StdVTotal, (in line unit)
      2600,                // StdHTotal,
      1920, 1440,          // StdWidth, StdHeight,
      344,  56,            // StdHBackPorch, StdVBackPorch,
      0,                   // StdSyncWidth (for overlapped mode)
      gmd_MODE_GRAPHICS,   // ModeType,
      0,                   // ExtCheck
   },

   {  // 10 *** 1800 x 1440 x 60 Hz *** 218.25MHz
      894, 10,             // StdHFreq, (in 100Hz unit)
      600, 10,             // StdVFreq, (in 1/10Hz unit)
      1493, 4,             // StdVTotal, (in line unit)
      2440,                // StdHTotal,
      1800, 1440,          // StdWidth, StdHeight,
      320,  43,            // StdHBackPorch, StdVBackPorch,
      0,                   // StdSyncWidth (for overlapped mode)
      gmd_MODE_GRAPHICS,   // ModeType,
      0,                   // ExtCheck
   },

   {  // 11 *** 2560 x 1440 x 60 Hz *** 241.5MHz
      888, 10,             // StdHFreq, (in 100Hz unit)
      600, 10,             // StdVFreq, (in 1/10Hz unit)
      1481, 4,             // StdVTotal, (in line unit)
      2720,                // StdHTotal,
      2560, 1440,          // StdWidth, StdHeight,
      80,  33,             // StdHBackPorch, StdVBackPorch,
      0,                   // StdSyncWidth (for overlapped mode)
      gmd_MODE_GRAPHICS,   // ModeType,
      0,                   // ExtCheck
   },

   {  // 12 *** 1856 x 1392 x 60 Hz *** 218.25MHz
      863, 10,             // StdHFreq, (in 100Hz unit)
      600, 10,             // StdVFreq, (in 1/10Hz unit)
      1439, 4,             // StdVTotal, (in line unit)
      2528,                // StdHTotal,
      1856, 1392,          // StdWidth, StdHeight,
      352,  43,            // StdHBackPorch, StdVBackPorch,
      0,                   // StdSyncWidth (for overlapped mode)
      gmd_MODE_GRAPHICS,   // ModeType,
      0,                   // ExtCheck
   },

   {  // *** Last Entry ***
      0, 0,
      0, 0,
      0, 0,
      0,
      0, 0,
      0, 0,
      0,
      0,
      0,
   }
};

//******************************************************************************
// Mode Table number definition
//******************************************************************************
ROM BYTE NumOfModeTableHD = sizeof(CompHD_ModeTable) / sizeof(CompHD_ModeTable[0]) - 1;
ROM BYTE NumOfModeTable60 = sizeof(StandardModeTable_60) / sizeof(StandardModeTable_60[0]) - 1;
ROM BYTE NumOfModeTable70 = sizeof(StandardModeTable_70) / sizeof(StandardModeTable_70[0]) - 1;
ROM BYTE NumOfModeTable85 = sizeof(StandardModeTable_85) / sizeof(StandardModeTable_85[0]) - 1;

#endif // FEATURE_AIP == ENABLE

//******************************************************************************
//
// USAGE        :   HDMI Graphics mode lookup table
//
// NOTE 		: 	For "StdHStart, StdVStart"; The "Start" is based on the
//					sum of the Sync Time and Back Porch.
//
//                  ======================================================
//					ANY CHANGES IN THIS FILE MUST BE UPDATED WITH "mode.h"
//                  ======================================================
//
//
// DESCRIPTION  :	HDMI Graphic supported modes:
//
//	Due of the pixel clock constraint in FLI8541 and FLI8548, these modes are only available
//	for internal DVI/HDMI port, but not through the AIP.
//					1280 x 1024 x 85 Hz (VESA) ***
//					1600 x 1200 x 60 Hz
//					1920 x 1200 x 60 Hz
//******************************************************************************
#ifdef USE_HDMI_MODE_TABLE
ROM StandardModeStruct  HDMI_ModeTable[] =
{
   {                       // *** 480P/      -  1440 x  480 x 60P ***
      314, 10,              // StdHFreq, DeltaHFreq, (in 100Hz unit)
      600, 10,              // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      525, 10,              // StdVTotal, DeltaVTotal, (in line unit)
      1716,                 // StdHTotal,
      1440, 480,            // StdWidth, StdHeight,
      276, 45,              // StdHStart, StdVStart,
      0,                    // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_EDVIDEO | gmd_MODE_SOG,		// ModeType
      gmd_MODE_OVERLAPPED_480P,            // Overlap Type
   },

   {                      // *** (HDTV) 1080P(25Hz) - 1920 x 1080 x 25P ***
      282, 50,             // StdHFreq, DeltaHFreq, (in 100Hz unit)
      250, 20,             // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      1125, 20,            // StdVTotal, DeltaVTotal, (in line unit)
      2640,                // StdHTotal,
      1920, 1080,          // StdWidth, StdHeight,
      720, 45,             // StdHStart, StdVStart,
      0,                   // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_HDVIDEO | gmd_MODE_SOG,   // ModeType
      0,                   // Overlap Type
   },

   {                       // *** 1280 x 1024 x 85 Hz  (VESA)***
      911, 10,              // StdHFreq, DeltaHFreq, (in 100Hz unit)
      850, 10,              // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      1072, 4,              // StdVTotal, DeltaVTotal, (in line unit)
      1728,                 // StdHTotal,
      1280, 1024,           // StdWidth, StdHeight,
      160+224+2, 3+44-1,	// StdHStart (Sync+BPorch), StdVStart, (+H +V)
      0,					// StdHpulse (used only in VideoOverlapped)
      gmd_MODE_GRAPHICS,    // ModeType
      0,    // Overlap Type
   },

   {                       // *** 58: Std xx - 1600 x 1200 x 60 Hz ***
      750, 10,              // StdHFreq, DeltaHFreq, (in 100Hz unit)
      600, 10,              // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      1246, 6,              // StdVTotal, DeltaVTotal, (in line unit)
      2160,                 // StdHTotal,
      1600, 1200,           // StdWidth, StdHeight,
      496, 48,              // StdHStart, StdVStart,
      0,					// StdHpulse (used only in VideoOverlapped)
      gmd_MODE_GRAPHICS,    // ModeType
      0,    				// Overlap Type
   },

   {                        // *** 60: Std xx - 1920 x 1200 x 60 Hz ***
      740, 10,              // StdHFreq, DeltaHFreq, (in 100Hz unit)
      600, 10,              // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      1235, 4,              // StdVTotal, DeltaVTotal, (in line unit)
      2080,                 // StdHTotal,
      1920, 1200,           // StdWidth, StdHeight,
      112, 33,              // StdHStart, StdVStart,
      0,     // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_GRAPHICS,    // ModeType
      0,    				// Overlap Type
   },
/////////////////////////////////////////////////////////////////////////////////

// NB!!!
// code between //..// and //..// is added to fix SCR3028 and is subject to debug

// CEA-861-D code: 29,30
   {                      // *** (?DTV) 576P(50Hz) - 1440 x 576 x 50P ***
      312, 10,             // StdHFreq, DeltaHFreq, (in 100Hz unit)
      500, 10,             // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      625, 10,             // StdVTotal, DeltaVTotal, (in line unit)
      1728,                // StdHTotal,
      1440, 576,           // StdWidth, StdHeight,
      288, 49,             // StdHStart, StdVStart,
      0,                   // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_SDVIDEO,    // | gmd_MODE_SOG,   // ModeType
      0,                   // Overlap Type
   },

// CEA-861-D code: 35,36
   {                      // *** (?DTV) 480P(60Hz) - 2280 x 480 x 60P ***
      315,  10,            // StdHFreq, DeltaHFreq, (in 100Hz unit)
      600,  10,            // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      525,  10,            // StdVTotal, DeltaVTotal, (in line unit)
      3432,                // StdHTotal,
      2880, 480,           // StdWidth, StdHeight,
      552,  45,            // StdHStart, StdVStart,
      0,                   // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_SDVIDEO,    // | gmd_MODE_SOG,   // ModeType
      0,                   // Overlap Type
   },

// CEA-861-D code: 37,38
   {                      // *** (?DTV) 576P(50Hz) - 2280 x 576 x 50P ***
      312,  10,             // StdHFreq, DeltaHFreq, (in 100Hz unit)
      500,  10,             // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      525,  10,            // StdVTotal, DeltaVTotal, (in line unit)
      3456,                // StdHTotal,
      2880, 576,           // StdWidth, StdHeight,
      576,  49,            // StdHStart, StdVStart,
      0,                   // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_SDVIDEO,    // | gmd_MODE_SOG,   // ModeType
      0,                   // Overlap Type
   },

// CEA-861-D code: 40
   {                      // *** (?DTV) 1080P(100Hz) - 1920 x 1080 x 100I ***
      562,  10,             // StdHFreq, DeltaHFreq, (in 100Hz unit)
      1000, 10,            // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      1125/2, 10,          // StdVTotal, DeltaVTotal, (in line unit) /2 because of interlaced
      2640,                // StdHTotal,
      1920, 540,          // StdWidth, StdHeight,
      720,  45,            // StdHStart, StdVStart,
      0,                   // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_HDVIDEO |
      gmd_MODE_INTERLACED, // | gmd_MODE_SOG,   // ModeType
      0,                   // Overlap Type
   },

// CEA-861-D code: 41
   {                      // *** (?DTV) 720P(100Hz) - 1280 x 720 x 100P ***
      750,  10,             // StdHFreq, DeltaHFreq, (in 100Hz unit)
      1000, 10,            // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      750,  10,            // StdVTotal, DeltaVTotal, (in line unit)
      1980,                // StdHTotal,
      1280, 720,           // StdWidth, StdHeight,
      700,  30,            // StdHStart, StdVStart,
      0,                   // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_HDVIDEO,    // | gmd_MODE_SOG,   // ModeType
      0,                   // Overlap Type
   },

// CEA-861-D code: 42,43
   {                      // *** (?DTV) 576P(100Hz) - 720 x 576 x 100P ***
      625,  10,             // StdHFreq, DeltaHFreq, (in 100Hz unit)
      1000, 10,            // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      625,  10,            // StdVTotal, DeltaVTotal, (in line unit)
      864,                 // StdHTotal,
      720,  576,           // StdWidth, StdHeight,
      144,  49,            // StdHStart, StdVStart,
      0,                   // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_EDVIDEO,    // | gmd_MODE_SOG,   // ModeType
      0,                   // Overlap Type
   },

// CEA-861-D code: 44,45
   {                      // *** (?DTV) 576I(100Hz) - 1440 x 576 x 100I ***
      312,  10,             // StdHFreq, DeltaHFreq, (in 100Hz unit)
      1000, 10,            // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      625/2,  10,            // StdVTotal, DeltaVTotal, (in line unit)
      1728,                // StdHTotal,
      1440, 288,           // StdWidth, StdHeight,
      288,  49,            // StdHStart, StdVStart,
      0,                   // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_EDVIDEO |
      gmd_MODE_INTERLACED, // | gmd_MODE_SOG,   // ModeType
      0,                   // Overlap Type
   },

// CEA-861-D code: 46
   {                      // *** (?DTV) 1080I(120Hz) - 1920 x 1080 x 120I ***
      674,  10,             // StdHFreq, DeltaHFreq, (in 100Hz unit)
      1198, 10,            // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      1125/2, 10,            // StdVTotal, DeltaVTotal, (in line unit)
      2200,                // StdHTotal,
      1920, 540,          // StdWidth, StdHeight,
      280,  45,            // StdHStart, StdVStart,
      0,                   // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_HDVIDEO |
      gmd_MODE_INTERLACED, // | gmd_MODE_SOG,   // ModeType
      0,                   // Overlap Type
   },

// CEA-861-D code: 47
   {                      // *** (?DTV) 720P(120Hz) - 1280 x 720 x 120P ***
      899,  10,             // StdHFreq, DeltaHFreq, (in 100Hz unit)
      1198, 10,            // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      750,  10,             // StdVTotal, DeltaVTotal, (in line unit)
      1650,                // StdHTotal,
      1280, 720,           // StdWidth, StdHeight,
      370,  30,            // StdHStart, StdVStart,
      0,                   // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_EDVIDEO,    // | gmd_MODE_SOG,   // ModeType
      0,                   // Overlap Type
   },

// CEA-861-D code: 48,49
   {                      // *** (?DTV) 720P(120Hz) - 720 x 480 x 120P ***
      629,  10,             // StdHFreq, DeltaHFreq, (in 100Hz unit)
      1198, 10,            // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      525,  10,             // StdVTotal, DeltaVTotal, (in line unit)
      858,                 // StdHTotal,
      720, 480,            // StdWidth, StdHeight,
      138,  45,            // StdHStart, StdVStart,
      0,                   // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_SDVIDEO,    // | gmd_MODE_SOG,   // ModeType
      0,                   // Overlap Type
   },

// CEA-861-D code: 50,51
   {                      // *** (?DTV) 480I(120Hz) - 1440 x 480 x 120I ***
      315,  10,             // StdHFreq, DeltaHFreq, (in 100Hz unit)
      1198, 10,            // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      525/2,  10,             // StdVTotal, DeltaVTotal, (in line unit)
      1716,                // StdHTotal,
      1440, 240,           // StdWidth, StdHeight,
      276,  45,            // StdHStart, StdVStart,
      0,                   // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_SDVIDEO |
      gmd_MODE_INTERLACED, // | gmd_MODE_SOG,   // ModeType
      0,                   // Overlap Type
   },

// CEA-861-D code: 52,53
   {                      // *** (?DTV) 576P(200Hz) - 720 x 576 x 200P ***
      1250, 10,            // StdHFreq, DeltaHFreq, (in 100Hz unit)
      2000, 10,            // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      625,  10,             // StdVTotal, DeltaVTotal, (in line unit)
      864,                 // StdHTotal,
      720,  576,           // StdWidth, StdHeight,
      144,  49,            // StdHStart, StdVStart,
      0,                   // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_SDVIDEO,    // | gmd_MODE_SOG,   // ModeType
      0,                   // Overlap Type
   },

// CEA-861-D code: 52,53
   {                      // *** (?DTV) 576P(200Hz) - 720 x 576 x 200P ***
      1250, 10,            // StdHFreq, DeltaHFreq, (in 100Hz unit)
      2000, 10,            // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      625,  10,             // StdVTotal, DeltaVTotal, (in line unit)
      864,                 // StdHTotal,
      720,  576,           // StdWidth, StdHeight,
      144,  49,            // StdHStart, StdVStart,
      0,                   // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_SDVIDEO,    // | gmd_MODE_SOG,   // ModeType
      0,                   // Overlap Type
   },

// CEA-861-D code: 54,55
   {                      // *** (?DTV) 576I(200Hz) - 1440 x 576 x 200P ***
      625,  10,             // StdHFreq, DeltaHFreq, (in 100Hz unit)
      2000, 10,            // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      625/2,  10,             // StdVTotal, DeltaVTotal, (in line unit)
      1728,                // StdHTotal,
      1440, 288,           // StdWidth, StdHeight,
      388,  49,            // StdHStart, StdVStart,
      0,                   // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_SDVIDEO |
      gmd_MODE_INTERLACED, // | gmd_MODE_SOG,   // ModeType
      0,                   // Overlap Type
   },

// CEA-861-D code: 56,57
   {                      // *** (?DTV) 480P(240Hz) - 720 x 480 x 240P ***
      1258, 10,            // StdHFreq, DeltaHFreq, (in 100Hz unit)
      2397, 10,            // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      525,  10,             // StdVTotal, DeltaVTotal, (in line unit)
      858,                 // StdHTotal,
      720,  480,           // StdWidth, StdHeight,
      138,  45,            // StdHStart, StdVStart,
      0,                   // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_SDVIDEO,     // | gmd_MODE_SOG,   // ModeType
      0,                   // Overlap Type
   },

// CEA-861-D code: 58,59
   {                      // *** (?DTV) 480I(240Hz) - 1440 x 480 x 240I ***
      629,  10,            // StdHFreq, DeltaHFreq, (in 100Hz unit)
      2397, 10,            // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      525/2,  10,             // StdVTotal, DeltaVTotal, (in line unit)
      1716,                 // StdHTotal,
      1440, 240,           // StdWidth, StdHeight,
      276,  45,            // StdHStart, StdVStart,
      0,                   // StdHpulse (used only in VideoOverlapped)
      gmd_MODE_SDVIDEO |
      gmd_MODE_INTERLACED, // | gmd_MODE_SOG,   // ModeType
      0,                   // Overlap Type
   },

/////////////////////////////////////////////////////////////////////////////////

   // null element terminator
   {
      0, 0,
      0, 0,
      0, 0,
      0,
      0, 0,
      0, 0,
      0,
      0,
      0,
   }
};

#endif

//******************************************************************************
//
// USAGE        :   Internal decoder video mode lookup table
//
// NOTE 		: 	For "StdHStart, StdVStart"; The "Start" is based on the
//					sum of the Sync Time and Back Porch.
//
//                  ======================================================
//					ANY CHANGES IN THIS FILE MUST BE UPDATED WITH "mode.h"
//                  ======================================================
//
// DESCRIPTION  :	Standard Video supported modes for internal decoder:
//					 ***  1  - NTSC 	  	-  720 x  240 x 60I ***
//					 ***  2  - PAL  		-  720 x  288 x 50I ***
//
//******************************************************************************
#ifdef EXTERNAL_DECODER
ROM StandardModeStruct  IntDecoder_ModeTable[] =
{
   //************************************************************************
   //  V I D E O                                              (Decoder input)
   //************************************************************************

   {                       // *** NTSC -  720 x  240 x 60I ***
      157, 10,              // StdHFreq, DeltaHFreq, (in 100Hz unit)
      599, 10,              // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      262, 4,               // StdVTotal, DeltaVTotal, (in line unit)
      858,                  // StdHTotal,
      720, 240,   			// StdWidth, StdHeight,
      4, 20,                // StdHStart, StdVStart
      0,					// StdHpulse (used only in VideoOverlapped)
      gmd_MODE_SDVIDEO | gmd_MODE_INTERLACED,		// ModeType
      0,			 	// Overlap Type
   },
   {                       // *** PAL  -  720 x  288 x 50I ***
      156, 10,              // StdHFreq, DeltaHFreq, (in 100Hz unit)
      500, 10,              // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      312, 4,             	// StdVTotal, DeltaVTotal, (in line unit)
      864,                  // StdHTotal,
      720, 288,             // StdWidth, StdHeight,
      4, 24,                // StdHStart, StdVStart,
      0,					// StdHpulse (used only in VideoOverlapped)
      gmd_MODE_SDVIDEO | gmd_MODE_INTERLACED,		// ModeType
      0,				// Overlap Type
   },

   // null element terminator
   {
      0, 0,
      0, 0,
      0, 0,
      0,
      0, 0,
      0, 0,
      0,
      0,
      0,
   }
};

//******************************************************************************
//
// USAGE        :   External decoder video mode lookup table
//
// NOTE 		: 	For "StdHStart, StdVStart"; The "Start" is based on the
//					sum of the Sync Time and Back Porch.
//
//                  ======================================================
//					ANY CHANGES IN THIS FILE MUST BE UPDATED WITH "mode.h"
//                  ======================================================
//
// DESCRIPTION  :	Standard Video supported modes for internal decoder:
//					 ***  1  - NTSC 	  	-  720 x  240 x 60I ***
//					 ***  2  - PAL  		-  720 x  288 x 50I ***
//
//******************************************************************************
ROM StandardModeStruct  ExtDecoder_ModeTable[] =
{
   {                       // *** NTSC -  720 x  240 x 60I ***
      157, 10,              // StdHFreq, DeltaHFreq, (in 100Hz unit)
      599, 10,              // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      262, 4,               // StdVTotal, DeltaVTotal, (in line unit)
      858,                  // StdHTotal,
      720, 240,   			// StdWidth, StdHeight,
      6, 20,                // StdHStart, StdVStart
      0,					// StdHpulse (used only in VideoOverlapped)
      gmd_MODE_SDVIDEO | gmd_MODE_INTERLACED,		// ModeType
      0,				// Overlap Type
   },
   {                       // *** PAL  -  720 x  288 x 50I ***
      156, 10,              // StdHFreq, DeltaHFreq, (in 100Hz unit)
      500, 10,              // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      312, 4,             	// StdVTotal, DeltaVTotal, (in line unit)
      864,                  // StdHTotal,
      720, 288,             // StdWidth, StdHeight,
      4, 24,                // StdHStart, StdVStart,
      0,					// StdHpulse (used only in VideoOverlapped)
      gmd_MODE_SDVIDEO | gmd_MODE_INTERLACED,		// ModeType
      0,				// Overlap Type
   },

   {                       // *** PAL  -  720 x  288 x 50I ***
      316, 10,              // StdHFreq, DeltaHFreq, (in 100Hz unit)
      302, 10,              // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      1052, 4,             	// StdVTotal, DeltaVTotal, (in line unit)
      864,                  // StdHTotal,
      720, 576,             // StdWidth, StdHeight,
      4, 24,                // StdHStart, StdVStart,
      0,					// StdHpulse (used only in VideoOverlapped)
      gmd_MODE_SDVIDEO ,		// ModeType
      0,				// Overlap Type
   },

   {                       // *** PAL  -  720 x  288 x 50I ***
      633, 10,              // StdHFreq, DeltaHFreq, (in 100Hz unit)
      500, 10,              // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
      2102, 8,             	// StdVTotal, DeltaVTotal, (in line unit)
      1728,                  // StdHTotal,
      704, 576,             // StdWidth, StdHeight,
      4, 24,                // StdHStart, StdVStart,
      0,					// StdHpulse (used only in VideoOverlapped)
      gmd_MODE_SDVIDEO ,		// ModeType
      0,				// Overlap Type
   },

  //Hyb140310+ for component mode search.
  {  //  *** 480P (60Hz) -  720 x	480 x 60P ***
	 314, 10,	 // StdHFreq, DeltaHFreq, (in 100Hz unit)
	 600, 10,	 // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
	 525, 10,	 // StdVTotal, DeltaVTotal, (in line unit)
	 858,		 // StdHTotal,
	 720, 480,	 // StdWidth, StdHeight,
	 49, 39,	 // StdHStart, StdVStart,
	 62,		 // StdHpulse (used only in VideoOverlapped)
	 gmd_MODE_EDVIDEO | gmd_MODE_SOG,	 // ModeType
	 0, 	 // Overlap Type
  },  
  
  {  // *** 576P (50Hz) -  720 x	576 x 50P ***
	 312, 10,	 // StdHFreq, DeltaHFreq, (in 100Hz unit)
	 500, 10,	 // StdVFreq, DeltaVFreq, (in 1/10Hz unit)
	 624, 10,	 // StdVTotal, DeltaVTotal, (in line unit)
	 864,		 // StdHTotal,
	 720, 576,	 // StdWidth, StdHeight,
	 90, 53,	 // StdHStart, StdVStart,
	 33+6,		 // StdHpulse (used only in VideoOverlapped)
	 gmd_MODE_EDVIDEO | gmd_MODE_SOG,	 // ModeType
	 gmd_MODE_OVERLAPPED_1080_576,		 // Overlap Type
  },

	  
   // null element terminator
   {
      0, 0,
      0, 0,
      0, 0,
      0,
      0, 0,
      0, 0,
      0,
      0,
      0,
   }
};

#endif // EXTERNAL_DECODER

//******************************************************************************
//******************************************************************************

/*********************************  END  ************************************/
