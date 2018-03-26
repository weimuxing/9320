/*
	$Workfile:   DrawBitmap.c  $
	$Revision:   1.0  $
	$Date:   Feb 11 2004 14:01:42  $
*/
#define __DRAWBITMAP_C__
//******************************************************************************
//
//              Copyright (C) 2002.  GENESIS MICROCHIP INC. CONFIDENTIAL
//      All rights reserved.  No part of this program may be reproduced.    
//
//     Genesis Microchip Inc., 165 Commerce Valley Dr. West                 
//     Thornhill, Ontario, Canada, L3T 7V8                                  
//
//==============================================================================
//
// MODULE:      DrawBitmap.c
//
// USAGE:       Draws a bitmap control in "Default" color state
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#include "system\all.h"

//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************
#ifdef BITMAP_DEBUG
#define msg(string)         gm_Print(string,0)
#define msg_i(string,value) gm_Print(string,value)
#else 
#define msg(string)         
#define msg_i(string,value) 
#endif


#define DEFAULT_COLOR_STATE		0		// DStaticText
#define SELECT_COLOR_STATE		1		// DStaticText
#define DISABLE_COLOR_STATE		2		// DStaticText  // Alex M 0430

//******************************************************************************
//  C O D E
//******************************************************************************
void DrawSelectedHorPosition1MoveableBitmap(void) {DrawSelectedMoveableBitmap(1, SELECT_COLOR_STATE);}
void DrawSelectedHorPosition2MoveableBitmap(void) {DrawSelectedMoveableBitmap(2, SELECT_COLOR_STATE);}
void DrawSelectedHorPosition3MoveableBitmap(void) {DrawSelectedMoveableBitmap(3, SELECT_COLOR_STATE);}
void DrawSelectedHorPosition4MoveableBitmap(void) {DrawSelectedMoveableBitmap(4, SELECT_COLOR_STATE);}
void DrawSelectedHorPosition5MoveableBitmap(void) {DrawSelectedMoveableBitmap(5, SELECT_COLOR_STATE);}
void DrawSelectedHorPosition6MoveableBitmap(void) {DrawSelectedMoveableBitmap(6, SELECT_COLOR_STATE);}
void DrawSelectedHorPosition7MoveableBitmap(void) {DrawSelectedMoveableBitmap(7, SELECT_COLOR_STATE);}
void DrawSelectedHorPosition8MoveableBitmap(void) {DrawSelectedMoveableBitmap(8, SELECT_COLOR_STATE);}
//******************************************************************************
// FUNCTION:    DrawBitmap(void)
//
// USAGE:       Draws a bitmap control in "Default" color state	
//
// DESCRIPTION:	Wrapper function for DrawBitmapControl, passes in unselected 
// 				color state.
//
// INPUT:		None
// OUTPUT:		None
// GLOBALS:		None
// USED_REGS:	None
//******************************************************************************
void DrawSelectedMoveableBitmap(BYTE HorPositionIndex, BYTE B_ColorState)
{
	msg("DrawBitmap:");
	DrawMoveableBitmapControl(HorPositionIndex, B_ColorState);
}

