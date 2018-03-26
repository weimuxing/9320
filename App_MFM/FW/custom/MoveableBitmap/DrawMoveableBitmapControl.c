/*
	$Workfile:   DrawBitmapControl.c  $
	$Revision:   1.1  $
	$Date:   Jan 06 2005 15:38:58  $
*/
#define __DRAWBITMAPCONTROL_C__
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
// MODULE:      DrawBitmapControl.c
//
// USAGE:       Draws the Bitmap Control
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
#define msg(string)         gm_Print(string, 0)
#define msg_i(string,value) gm_Print(string, value)
#else 
#define msg(string)         
#define msg_i(string,value) 
#endif

//******************************************************************************
//  C O D E
//******************************************************************************
//******************************************************************************
// FUNCTION: DrawBitmapControl(BYTE B_ColorState)
// USAGE: Draws the Bitmap Control
//		  
// Description: The class information can be extracted by first casting the 
//				gmv_ActionOpPtr to BITMAP Control structure. The member variables of 
//				Rectangle structure used here include TileId, Xpos, YPos, XSize, 
//				YSize, Colors (a pointer that points to the color map data). 
//				The color map pointer of the control is passed in. This pointer
//				and the above parameters are required to call the system function
//				gm_OsdFillRectangle to draw the actual bitmap. 
//				If the rotation flag is enabled, then then width, height and 
//				position of the control is recalculated. The pointer to the pixel
//				data is also changed.
//
// INPUT:   B_ColorState - The color state that the bitmap is to be drawn in.
// OUTPUT:	None
// GLOBAL:	gmv_ActionOpPtr, Gb_GselOsdRotation, Gb_GselOsdRotationEnabled	 
//******************************************************************************
void DrawMoveableBitmapControl(BYTE HorPositionIndex, BYTE B_ColorState)
{
	gmt_OSD_MOVEABLEBITMAP_CTRL ROM *Sp_Bitmap;
	BYTE ROM *Bp_Pdata;
	BYTE ROM *Bp_ColorMap;
	WORD W_ColourDepth;
	WORD W_BitmapXStart, W_BitmapYStart, W_BitmapWidth;
	WORD W_FragmentWidth, W_FragmentHeight;
	WORD W_LastFragment, W_Size, W_SpecialSizes;
	WORD W_CurrentXStart, W_CurrentYStart;
	gmt_OSD_BASE_CTRL St_CtrlParams;

   gmt_OSD_TILE_POSITION tile_pos;

	// Set up all our needed pointers to the control
	gm_OsdGetActionOperand();
	Sp_Bitmap = (gmt_OSD_MOVEABLEBITMAP_CTRL ROM *)gmv_ActionOpPtr;

	// Parse colors and highlight information
	// Don't redraw the image if we are only hiliting it.
	if(!ParseDrawProperties( B_ColorState, Sp_Bitmap->Colors.Bp_ColorMap) )
		return;

	Bp_ColorMap = gmv_ColorMap;

	Bp_Pdata = Sp_Bitmap->Colors.Bp_PixelData;
	St_CtrlParams.TileId = Sp_Bitmap->TileId;

   switch (HorPositionIndex)
	{
		case 1:
		St_CtrlParams.XPos = (WORD)Sp_Bitmap-> HorPosition1;
		break;
		case 2:
		St_CtrlParams.XPos = (WORD)Sp_Bitmap-> HorPosition2;
		break;
		case 3:
		St_CtrlParams.XPos = (WORD)Sp_Bitmap-> HorPosition3;
		break;
		case 4:
		St_CtrlParams.XPos = (WORD)Sp_Bitmap-> HorPosition4;
		break;
		case 5:
		St_CtrlParams.XPos = (WORD)Sp_Bitmap-> HorPosition5;
		break;
		case 6:
		St_CtrlParams.XPos = (WORD)Sp_Bitmap-> HorPosition6;
		break;
		case 7:
		St_CtrlParams.XPos = (WORD)Sp_Bitmap-> HorPosition7;
		break;
		case 8:
		St_CtrlParams.XPos = (WORD)Sp_Bitmap-> HorPosition8;
		break;
	}

	tile_pos = gmv_OsdTilePosition[Sp_Bitmap->TileId];
   St_CtrlParams.XPos -= tile_pos.W_XPos;

   St_CtrlParams.YPos = Sp_Bitmap->YPos;
	St_CtrlParams.XSize = ((ST_OSD_BITMAP_HDR ROM *)Bp_Pdata)->W_Width;
	St_CtrlParams.YSize = ((ST_OSD_BITMAP_HDR ROM *)Bp_Pdata)->W_Height;

	// Get new co-ordinates according to rotation values and scaling.	
	ScaleXParam(&St_CtrlParams.XPos);
	ScaleYParam(&St_CtrlParams.YPos);
	
	W_ColourDepth = ((ST_OSD_BITMAP_HDR ROM *)Bp_Pdata)->B_Attr & BITMAP_CLR_DEPTH_BITS;

	if(((ST_OSD_BITMAP_HDR ROM *)Bp_Pdata)->B_Attr & BITMAP_FRAGMENTS_BIT)
	{
		msg("Many pixel data fragments");

		W_BitmapXStart = St_CtrlParams.XPos;
		W_BitmapYStart = St_CtrlParams.YPos;
		W_BitmapWidth = St_CtrlParams.XSize;
		W_FragmentWidth = Bp_Pdata [BITMAP_DATA_OFFSET + 1];
		W_FragmentHeight = Bp_Pdata [BITMAP_DATA_OFFSET];
		Bp_Pdata += BITMAP_DATA_OFFSET + 2;
		W_LastFragment = 0;

		msg_i("Bitmap: W_BitmapXStart = %d", W_BitmapXStart);
		msg_i("Bitmap: W_BitmapYStart = %d", W_BitmapYStart);
		msg_i("Bitmap: W_BitmapWidth = %d", W_BitmapWidth);
		msg_i("Bitmap: W_FragmentWidth = %d", W_FragmentWidth);
		msg_i("Bitmap: W_FragmentHeight = %d", W_FragmentHeight);

		W_CurrentXStart = 0;
		W_CurrentYStart = 0;
		
		while(!W_LastFragment)
		{
			W_Size = *((WORD far *)Bp_Pdata);
			W_LastFragment = W_Size & BITMAP_LAST_FRAGMENT_BIT;
			W_SpecialSizes = W_Size & BITMAP_SPEC_SIZE_BIT;
			W_Size = W_Size & BITMAP_FRAGMENT_SIZE_BITS;
			Bp_Pdata += 2;
			if(W_SpecialSizes)
			{
				St_CtrlParams.XSize = Bp_Pdata [1];
				St_CtrlParams.YSize = Bp_Pdata[0];
				Bp_Pdata += 2;
			}
			else
			{
				St_CtrlParams.XSize = W_FragmentWidth;
				St_CtrlParams.YSize = W_FragmentHeight;
			}

			if(W_CurrentXStart >= W_BitmapWidth)
			{
				W_CurrentYStart += W_FragmentHeight;
				W_CurrentXStart = 0;
			}

			St_CtrlParams.XPos = W_BitmapXStart + W_CurrentXStart;
			St_CtrlParams.YPos = W_BitmapYStart + W_CurrentYStart;

			msg_i("Fragment: St_CtrlParams.XPos = %d", St_CtrlParams.XPos);
			msg_i("Fragment: St_CtrlParams.YPos = %d", St_CtrlParams.YPos);
			msg_i("Fragment: St_CtrlParams.XSize = %d", St_CtrlParams.XSize);
			msg_i("Fragment: St_CtrlParams.YSize = %d", St_CtrlParams.YSize);
			msg_i("Fragment: W_Size = %d", W_Size);
			msg_i("Fragment: W_LastFragment = %d", W_LastFragment);
			msg_i("Fragment: W_SpecialSizes = %d", W_SpecialSizes);

			gm_OsdDrawBitmapStripe(&St_CtrlParams, W_ColourDepth, Bp_Pdata, Bp_ColorMap);
			
			W_CurrentXStart += W_FragmentWidth;
			Bp_Pdata += W_Size;
		}

	}
	else
	{
		gm_OsdDrawBitmapStripe(&St_CtrlParams, W_ColourDepth, Bp_Pdata + BITMAP_DATA_OFFSET, Bp_ColorMap);
	}
}


