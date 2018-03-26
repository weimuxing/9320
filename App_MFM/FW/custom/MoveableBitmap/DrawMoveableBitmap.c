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
void DrawHorPosition1MoveableBitmap(void) {DrawMoveableBitmap(1, DEFAULT_COLOR_STATE);}
void DrawHorPosition2MoveableBitmap(void) {DrawMoveableBitmap(2, DEFAULT_COLOR_STATE);}
void DrawHorPosition3MoveableBitmap(void) {DrawMoveableBitmap(3, DEFAULT_COLOR_STATE);}
void DrawHorPosition4MoveableBitmap(void) {DrawMoveableBitmap(4, DEFAULT_COLOR_STATE);}
void DrawHorPosition5MoveableBitmap(void) {DrawMoveableBitmap(5, DEFAULT_COLOR_STATE);}
void DrawHorPosition6MoveableBitmap(void) {DrawMoveableBitmap(6, DEFAULT_COLOR_STATE);}
void DrawHorPosition7MoveableBitmap(void) {DrawMoveableBitmap(7, DEFAULT_COLOR_STATE);}
void DrawHorPosition8MoveableBitmap(void) {DrawMoveableBitmap(8, DEFAULT_COLOR_STATE);}
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
void DrawMoveableBitmap(BYTE HorPositionIndex, BYTE B_ColorState)
{
	msg("DrawBitmap:");
	DrawMoveableBitmapControl(HorPositionIndex, B_ColorState);
}

