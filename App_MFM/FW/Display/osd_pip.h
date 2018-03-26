/*
	$Workfile:   osd_pip.h  $
	$Revision: 1.5 $
	$Date: 2012/04/23 07:26:48 $
*/
#ifndef __OSD_PIP_H__
#define __OSD_PIP_H__
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
// MODULE:      osd_pip.h
//
// USAGE:
//
//******************************************************************************

typedef enum PipModeEnum
{
   NO_PIP,
   LARGE_PIP,
   SMALL_PIP,
   SIDE_BY_SIDE,
   SIDE_BY_SIDE_TALL,
   MULTI_PIP,				//5 
	MEDIUM_PIP,
	HUGE_PIP,			
	SIDE_BY_SIDE_PUP,		//8 P upon p
	QUARTER_PXP,			//9
	FULL_SCREEN_PIP,		// 10
	FLEXIBLE_PIP,			// 11
	NUM_PIP_MODES
} PipModes;

typedef enum PipPositionEnum
{
   TOP_LEFT,//BOTTOM_RIGHT,
   TOP_RIGHT,
   BOTTOM_LEFT,//TOP_LEFT,
   BOTTOM_RIGHT,//BOTTOM_LEFT,
	POS_SIDE_BY_SIDE,		//P By P
	POS_SIDE_BY_SIDE_TALL,	// 5	//P By P
	POS_SIDE_BY_SIDE_PUP,	//P upon P
	POS_QUARTER_PXP_TL,	//Top Left
	POS_QUARTER_PXP_TR,	//Top Right
	POS_QUARTER_PXP_BL,	//Bottom Left
	POS_QUARTER_PXP_BR,		//10//Bottom Right
	POS_FULL_SCREEN,		
	POS_PXP_FLEXIBLE,
	NUM_PIP_POSITIONS
} PipPositions;

typedef enum PipWindEnum
{
   PIP_FIRST,
   PIP_NO1 = PIP_FIRST,
   PIP_NO2,
   PIP_NO3,
   PIP_NO4,
   PIP_NO5,
   PIP_NO6,
   PIP_NO7,
   PIP_LAST = PIP_NO7,
}PipWindNumber;


//******************************************************************************
//  M A C R O S
//******************************************************************************


#define	IsSideBySidePipMode(PipMode)	\
				((PipMode == SIDE_BY_SIDE) || (PipMode == SIDE_BY_SIDE_TALL))

#define IsSinglePipMode(PipMode)		\
				((PipMode == LARGE_PIP) || (PipMode == SMALL_PIP) || (PipMode == FLEXIBLE_PIP)||(PipMode == MEDIUM_PIP)||(PipMode == HUGE_PIP))

//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************

//extern BYTE B_PreviousPipMode;

//******************************************************************************
//  E X T E R N A L    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
void AdjustPipMode(void);
void AdjustPipPosition(void);
 
BOOL IsPipModeAllowed(PipModes PendingPipMode);

void SwapMainPip(void);
#ifdef SMART_SWAP
void SwapMainPip_InputSwap(void);
#endif

#endif

