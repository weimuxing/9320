/*
	$Workfile:   DebugHandler.h  $
	$Revision: 1.4 $
	$Date: 2011/12/13 10:32:45 $
*/

#ifndef __DEBUG_HANDLER_H__
#define __DEBUG_HANDLER_H__
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
// MODULE:      DebugHandler.h
//
// USAGE:       Header file for application debug handler definition
//
//******************************************************************************

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************

#define	DEBUG_MESSAGE_TMO		8		// debug handler timeout (100ms unit)
// print message every 800ms

#ifdef UART_DEBUG_MODE
#define	DEBUG_MODE_SETUP				// debug mode setup debug message
#endif


//#define DEBUG_VIDEO_PROCESSING		// debug video processing in realtime
//#define DEBUG_PROFILE					// print profile

typedef enum
{
   DBG_HDLR_HW_UPDATE,
   DBG_HDLR_SERIAL_CMD,
   DBG_HDLR_MODE_DETECT,
   DBG_HDLR_MODE_SETUP,
   DBG_HDLR_FILMMODE,
   DBG_HDLR_OSD_EVENT,
   DBG_HDLR_VBI,
   DBG_HDLR_TV,
   MAX_DBG_HDLR
}DBG_HDLR;

//******************************************************************************
//  E X T E R N     V A R I A B L E
//******************************************************************************

#ifdef DEBUG_MODE_SETUP
extern DWORD D_Elapsed[];
#endif

#ifdef DEBUG_VIDEO_PROCESSING
extern WORD  W_LastVideoProcessing;
#endif

//******************************************************************************
//  G L O B A L    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

void DebugHandlerInit	(void);
void DebugHandler		(void);
void MeasureHandlerStart(DBG_HDLR W_Handler);
void MeasureHandlerEnd	(DBG_HDLR W_Handler);

#ifdef DEBUG_MODE_SETUP
void DebugMode		(gmt_PHY_CH B_Channel);
void DebugChannel	(gmt_PHY_CH B_Channel);
#endif

#if defined(DEBUG_MODE_SETUP) || defined(DEBUG_VIDEO_PROCESSING)
void DebugProcessing(void);
#endif

#endif
//*********************************  END  **************************************
