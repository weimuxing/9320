/*
	$Workfile:   EraseBitmap.c  $
	$Revision:   1.0  $
	$Date:   Feb 11 2004 14:01:54  $
*/
#define __ERASEBITMAP_C__
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
// MODULE:      EraseBitmap.c
//
// USAGE:       Erases a bitmap control from the screen
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#include "system\all.h"
#include "mem.h"

//******************************************************************************
//  C O D E
//******************************************************************************
//******************************************************************************
// void EraseBitmap(void)
//
// USAGE: Erases a bitmap control from the screen
// 
// DESCRIPTION: Erases a bitmap from the OSD by calling gm_OsdFillRectangle() 
//              to fill in the rectangular aread that the control was taking up.
//				This differs from the generic EraseControl in that it requires
//				ST_OSD_UNI_BITMAP sturcture to find the actual length and width
//				that needs to be erased.
//
// INPUT:       None
// OUTPUT:      None
// GLOBALS:		gmv_ActionOpPtr - Uses TileId, XPos, YPos, XSize, YSize and
//				EraseColorIndex
// USED_REGS:	None
//
//******************************************************************************
void EraseMoveableBitmap(BYTE HorPositionIndex)
{
	gmt_OSD_MOVEABLEBITMAP_CTRL ROM *Sp_BitmapCtrl;
	gmt_OSD_UNI_BITMAP ROM *Sp_Bitmap;

   gmt_OSD_TILE_POSITION tile_pos;

   gmt_OSD_MOVEABLEBITMAP_CTRL	MoveableBitmap;
   gmt_OSD_MOVEABLEBITMAP_CTRL  *Sp_BitmapCtrl_1;

	gm_OsdGetActionOperand();
  	Sp_BitmapCtrl = (gmt_OSD_MOVEABLEBITMAP_CTRL ROM *)gmv_ActionOpPtr;
  	Sp_Bitmap = &(Sp_BitmapCtrl->Colors);

   Sp_BitmapCtrl_1 = &MoveableBitmap;

   _fmemcpy((ROM*) Sp_BitmapCtrl_1, Sp_BitmapCtrl, sizeof(MoveableBitmap));

   switch (HorPositionIndex)
	{
		case 1:
		//Sp_BitmapCtrl_1 ->XPos  = (WORD)Sp_BitmapCtrl_1-> HorPosition1 + (WORD)Sp_BitmapCtrl_1->EraseXPosition1;  // AlexM 062205
      //Sp_BitmapCtrl_1 ->XSize -= (WORD)Sp_BitmapCtrl_1->EraseXPosition1;   // AlexM 062205
     		Sp_BitmapCtrl_1 ->XPos  = (WORD)Sp_BitmapCtrl_1-> HorPosition1; 
		break;
		case 2:
		//Sp_BitmapCtrl_1 ->XPos  = (WORD)Sp_BitmapCtrl_1-> HorPosition2 + (WORD)Sp_BitmapCtrl_1->EraseXPosition2;   // AlexM 062205
      //Sp_BitmapCtrl_1 ->XSize -= (WORD)Sp_BitmapCtrl_1->EraseXPosition2;   // AlexM 062205
      		Sp_BitmapCtrl_1 ->XPos  = (WORD)Sp_BitmapCtrl_1-> HorPosition2;
		break;
		case 3:
//		Sp_BitmapCtrl_1 ->XPos  = (WORD)Sp_BitmapCtrl_1-> HorPosition3 + (WORD)Sp_BitmapCtrl_1->EraseXPosition3;    // AlexM 062205
//      Sp_BitmapCtrl_1 ->XSize -= (WORD)Sp_BitmapCtrl_1->EraseXPosition3;    // AlexM 062205
		Sp_BitmapCtrl_1 ->XPos  = (WORD)Sp_BitmapCtrl_1-> HorPosition3;
		break;
		case 4:
		Sp_BitmapCtrl_1 ->XPos  = (WORD)Sp_BitmapCtrl_1-> HorPosition4;
		break;
		case 5:
		Sp_BitmapCtrl_1 ->XPos  = (WORD)Sp_BitmapCtrl_1-> HorPosition5;
		break;
		case 6:
		Sp_BitmapCtrl_1 ->XPos  = (WORD)Sp_BitmapCtrl_1-> HorPosition6;
		break;
		case 7:
		Sp_BitmapCtrl_1 ->XPos  = (WORD)Sp_BitmapCtrl_1-> HorPosition7;
		break;
		case 8:
		Sp_BitmapCtrl_1 ->XPos  = (WORD)Sp_BitmapCtrl_1-> HorPosition8;
		break;
	}

   tile_pos = gmv_OsdTilePosition[Sp_BitmapCtrl_1->TileId];

   Sp_BitmapCtrl_1 ->XPos -= tile_pos.W_XPos;

	gm_OsdFillRectangle(Sp_BitmapCtrl_1->TileId,
							Sp_BitmapCtrl_1->XPos, Sp_BitmapCtrl_1->YPos,
							*((WORD ROM *)(Sp_Bitmap->Bp_PixelData + BITMAP_WIDTH_OFFSET)),
							*((WORD ROM *)(Sp_Bitmap->Bp_PixelData + BITMAP_HEIGHT_OFFSET)),
							Sp_BitmapCtrl_1->EraseColorIndex);
}

void EraseHorPosition1MoveableBitmap(void) {EraseMoveableBitmap(1);}
void EraseHorPosition2MoveableBitmap(void) {EraseMoveableBitmap(2);}
void EraseHorPosition3MoveableBitmap(void) {EraseMoveableBitmap(3);}
void EraseHorPosition4MoveableBitmap(void) {EraseMoveableBitmap(4);}
void EraseHorPosition5MoveableBitmap(void) {EraseMoveableBitmap(5);}
void EraseHorPosition6MoveableBitmap(void) {EraseMoveableBitmap(6);}
void EraseHorPosition7MoveableBitmap(void) {EraseMoveableBitmap(7);}
void EraseHorPosition8MoveableBitmap(void) {EraseMoveableBitmap(8);}