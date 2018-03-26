#include "system\all.h"
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
// MODULE:      DrawBitmapSlider.c
//
// USAGE:       A wrapper function that takes a color state parameter. It in turn calls the
//		  DrawBitmapSliderControl function
//
//******************************************************************************

#ifdef DELL_U2410F

#ifdef CONTROLTYPE_VEKBITMAPBASESLIDER_USED
extern void DrawVEKBitmapSliderControl(BYTE B_ColorState, BYTE B_DrawFlag);
//******************************************************************************
//
// FUNCTION: DrawBitmapSlider(void)
//
// USAGE: A wrapper function that takes a color state parameter. It in turn calls the 
//		  DrawBitmapSliderControl function
//
// INPUT:		None
// OUTPUT:		None
// GLOBALS:		None
//
// USED_REGS:	None
//
//******************************************************************************
void DrawVEKBitmapBaseSlider(void)
{
	DrawVEKBitmapSliderControl (BITMAP_COLOR_STATE, TRUE);
}

void DrawSelectedVEKBitmapBaseSlider(void)
{
	DrawVEKBitmapSliderControl (1, TRUE);
}
#endif
#endif // DELL_U2410F
