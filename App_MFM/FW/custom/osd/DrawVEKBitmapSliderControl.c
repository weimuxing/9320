#include "system\all.h"
#include <string.h>  //JasonWu 20060126 for close warrning problem
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
// USAGE:       Draws the Bitmap Control.
//
//******************************************************************************

#ifdef DELL_U2410F

//#undef BITMAP_DEBUG
//#define BITMAP_DEBUG
#ifdef BITMAP_DEBUG
	#define msg(string)         gm_Print(string,0)
	#define msg_i(string,value) gm_Print(string,value)
#else
	#define msg(string)
	#define msg_i(string,value)
#endif

WORD YPOSSTART,ystart,sliderwidth;
static void OsdDrawBitmapStripeCustom(BYTE B_TileId, WORD *Stp_CtrlParams, WORD W_ColourDepth, BYTE ROM *Bp_Pdata, BYTE ROM *Bp_ColorMap,WORD XLim,WORD YPOS_START);

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
//void DrawBitmapControl(BYTE B_ColorState)
#ifdef CONTROLTYPE_VEKBITMAPBASESLIDER_USED
void DrawVEKBitmapSliderControl(BYTE B_ColorState, BYTE B_DrawFlag)
{
#if 1
	gmt_OSD_VEKBITMAPBASESLIDER_CTRL ROM *Sp_Bitmap;
	BYTE ROM *Bp_Pdata;
	BYTE ROM *Bp_ColorMap;

	WORD W_BmpWidth;

	WORD W_ColourDepth;
	WORD Wa_CtrlProp[4];
	WORD W_BitmapXStart, W_BitmapYStart, W_BitmapWidth;
	WORD W_FragmentWidth, W_FragmentHeight;
	WORD W_LastFragment, W_Size, W_SpecialSizes;
	WORD W_CurrentXStart, W_CurrentYStart;
	gmt_OSD_BASE_CTRL St_CtrlParams, St_DestParams;
	DWORD W_Interval, W_SliderValue,W_SliderValue2;
	SDWORD SDW_AdjValue, SDW_AdjMin;

   	WORD X_Pos, Y_Pos, W_Xsize, W_Ysize, W_temp;
//==> JimmyH200801113 add   
   #if 0	
	WORD W_RedF,W_GreenF,WBlueF,W_RedV,W_GreenV,WBlueV;
   #endif
//<== JimmyH20080213 add
	// Set up all our needed pointers to the control
	gm_OsdGetActionOperand();
	Sp_Bitmap = (gmt_OSD_VEKBITMAPBASESLIDER_CTRL ROM *)gmv_ActionOpPtr;

//	W_BmpWidth=((ST_OSD_BITMAP_HDR_ROM *)(Sp_Bitmap->Bp_PixelData))->W_Width

	SDW_AdjMin = gm_GetAdjusterMin(Sp_Bitmap->Associate);
	W_Interval = (DWORD)(gm_GetAdjusterMax(Sp_Bitmap->Associate) - SDW_AdjMin);
	SDW_AdjValue = gm_GetAdjuster(Sp_Bitmap->Associate) - SDW_AdjMin;

	// Calculate the adjustor value if direction is set
	if(Sp_Bitmap->Direction)
	{
		SDW_AdjValue = (SDWORD)W_Interval - SDW_AdjValue;
	}

	W_SliderValue2 = W_SliderValue = (DWORD)((DWORD)((Sp_Bitmap->YSize +1 ) * SDW_AdjValue) / W_Interval);

   
	// Parse colors and highlight information
	// Don't redraw the image if we are only hiliting it.
	if(!ParseDrawProperties( B_ColorState, Sp_Bitmap->Colors.Bp_ColorMap) )
		return;

	Bp_ColorMap = gmv_ColorMap;

	Bp_Pdata = Sp_Bitmap->Colors.Bp_PixelData;



	St_CtrlParams.TileId = Sp_Bitmap->TileId;
	CONTROL_XPOS(Wa_CtrlProp) = Sp_Bitmap->XPos;
	St_CtrlParams.XPos = Sp_Bitmap->XPos;
	CONTROL_YPOS(Wa_CtrlProp) = Sp_Bitmap->YPos;
	St_CtrlParams.YPos = Sp_Bitmap->YPos;
	CONTROL_WIDTH(Wa_CtrlProp) = ((ST_OSD_BITMAP_HDR ROM *)Bp_Pdata)->W_Width;
	St_CtrlParams.XSize = ((ST_OSD_BITMAP_HDR ROM *)Bp_Pdata)->W_Width;
	CONTROL_HEIGHT(Wa_CtrlProp) = ((ST_OSD_BITMAP_HDR ROM *)Bp_Pdata)->W_Height;
	St_CtrlParams.YSize = ((ST_OSD_BITMAP_HDR ROM *)Bp_Pdata)->W_Height;
   YPOSSTART=0;
   YPOSSTART=Sp_Bitmap->YPos+St_CtrlParams.YSize;

	// Get new co-ordinates according to rotation values and scaling.
	ScaleXParam(&St_CtrlParams.XPos);
	ScaleYParam(&St_CtrlParams.YPos);

	W_ColourDepth = ((ST_OSD_BITMAP_HDR ROM *)Bp_Pdata)->B_Attr & BITMAP_CLR_DEPTH_BITS;

   ystart=0;
	sliderwidth=CONTROL_HEIGHT(Wa_CtrlProp);
         
	if(((ST_OSD_BITMAP_HDR ROM *)Bp_Pdata)->B_Attr & BITMAP_FRAGMENTS_BIT)
	{

  		msg("Many pixel data fragments");

		W_BitmapXStart = St_CtrlParams.XPos;
		W_BitmapYStart = St_CtrlParams.YPos;
		W_BitmapWidth = St_CtrlParams.XSize;
		W_FragmentWidth = Bp_Pdata [BITMAP_DATA_OFFSET + 1];
//      W_FragmentWidth = 20;
		W_FragmentHeight = Bp_Pdata [BITMAP_DATA_OFFSET];
//      W_FragmentHeight = 1;
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
//         W_LastFragment = 1;
			W_SpecialSizes = W_Size & BITMAP_SPEC_SIZE_BIT;
  //       W_SpecialSizes= 0;
			W_Size = W_Size & BITMAP_FRAGMENT_SIZE_BITS;
			Bp_Pdata += 2;
			
			if(W_SpecialSizes)
			{
				St_CtrlParams.XSize = Bp_Pdata [1];
  				CONTROL_WIDTH(Wa_CtrlProp) = Bp_Pdata[1];
				St_CtrlParams.YSize = Bp_Pdata[0];
  				CONTROL_HEIGHT(Wa_CtrlProp) = Bp_Pdata[0];
				Bp_Pdata += 2;
			}	
			else
			{
				St_CtrlParams.XSize = W_FragmentWidth;
  				CONTROL_WIDTH(Wa_CtrlProp) = W_FragmentWidth;
				St_CtrlParams.YSize = W_FragmentHeight;
 				CONTROL_HEIGHT(Wa_CtrlProp) = W_FragmentHeight;
			}

			if(W_CurrentXStart >= W_BitmapWidth)
			{
				W_CurrentYStart += W_FragmentHeight;
				W_CurrentXStart = 0;
            W_SliderValue=W_SliderValue2;
			}
			
			St_CtrlParams.XPos = W_BitmapXStart + W_CurrentXStart;
         CONTROL_XPOS(Wa_CtrlProp) = W_BitmapXStart + W_CurrentXStart;
			St_CtrlParams.YPos = W_BitmapYStart + W_CurrentYStart;
         CONTROL_YPOS(Wa_CtrlProp) = W_BitmapYStart + W_CurrentYStart;

			msg_i("Fragment: St_DestParams.XPos = %d", St_DestParams.XPos);
			msg_i("Fragment: St_DestParams.YPos = %d", St_DestParams.YPos);
			msg_i("Fragment: St_DestParams.XSize = %d", St_DestParams.XSize);
			msg_i("Fragment: St_DestParams.YSize = %d", St_DestParams.YSize);
			msg_i("Fragment: W_Size = %d", W_Size);
			msg_i("Fragment: W_LastFragment = %d", W_LastFragment);
			msg_i("Fragment: W_SpecialSizes = %d", W_SpecialSizes);
//==> JimmyH200801113 add   
   #if 0			
			W_RedF = W_RedV = UserPrefUserRed;
			W_GreenF = W_GreenV = UserPrefUserGreen;
			WBlueF = WBlueV = UserPrefUserBlue;

			if (W_SliderValue < 4)
			{
				UserPrefUserRed = 0;
				W_SliderValue = 0;
			}
#endif
//<== JimmyH20080113 add
         if(W_SliderValue)
			{
				if (W_SliderValue <= (YPOSSTART-Sp_Bitmap->YPos))
				{
	  				OsdDrawBitmapStripeCustom(Sp_Bitmap->TileId,Wa_CtrlProp, W_ColourDepth, Bp_Pdata, Bp_ColorMap, (WORD)(W_SliderValue&(~1)),YPOSSTART);
          			W_SliderValue = 0;
				}
				else            
	         		{
//	         			Debugmessage_i("+++++++++Slider is Full",0);
					#if 1 // 1 // 20070731 Mikhail
 		         		W_SliderValue=CONTROL_HEIGHT(Wa_CtrlProp);
					#else
		         		W_SliderValue-=CONTROL_HEIGHT(Wa_CtrlProp);
					#endif
					gm_OsdDrawBitmapStripe(&St_CtrlParams, W_ColourDepth, Bp_Pdata, Bp_ColorMap);
				}
			}
		
			W_CurrentXStart += W_FragmentWidth;
			Bp_Pdata += W_Size;
     
		}

	}
	else
	{
//		Debugmessage_i("------------- 1111111111111111111111111",0);
		OsdDrawBitmapStripeCustom(Sp_Bitmap->TileId,Wa_CtrlProp, W_ColourDepth, Bp_Pdata, Bp_ColorMap, (WORD)(W_SliderValue&(~1)),YPOSSTART);
	}   
 
 	if(TEST_OSD_PARAM_ROTATION)
		{		
//		Debugmessage_i("-------------------22222222222222222222222222222",0);
  		gm_OsdGetTileSize(Sp_Bitmap->TileId, &W_Xsize, &W_Ysize);

	  	//save X position
		W_temp  = Sp_Bitmap->XPos;
 		
      //set X position
		X_Pos = Sp_Bitmap->YPos;

		//set Y position
		Y_Pos = W_Ysize - (W_temp - 1) - CONTROL_WIDTH(Wa_CtrlProp) - 1;			

      gm_OsdFillRectangle((WORD)Sp_Bitmap->TileId, (WORD)(X_Pos-2), (WORD)Y_Pos,
		(WORD)((Sp_Bitmap->YSize-W_SliderValue2)+2), (WORD)CONTROL_WIDTH(Wa_CtrlProp), (WORD)Sp_Bitmap->EraseColorIndex);

      gm_OsdFillRectangle(Sp_Bitmap->TileId, X_Pos+sliderwidth-1, Y_Pos,
		2, CONTROL_WIDTH(Wa_CtrlProp), Sp_Bitmap->EraseColorIndex);	

	   }
   else
      {
//      		Debugmessage_i("-------------------33333333333333333333333333333333333",0);
	 	gm_OsdFillRectangle((WORD)Sp_Bitmap->TileId, (WORD)Sp_Bitmap->XPos, (WORD)Sp_Bitmap->YPos,(WORD)CONTROL_WIDTH(Wa_CtrlProp), (WORD)(Sp_Bitmap->YSize-W_SliderValue2), (WORD)Sp_Bitmap->EraseColorIndex);	
      }
//==> JimmyH200801113 add   
   #if 0
	UserPrefUserRed = W_RedF;
	UserPrefUserGreen = W_GreenF;
	UserPrefUserBlue = WBlueF;
#endif
//<==  JimmyH20080213 add
B_DrawFlag = B_DrawFlag;
#endif

}

//static void gm_OsdDrawBitmapStripe(gmt_OSD_BASE_CTRL *Stp_CtrlParams, WORD W_ColourDepth, BYTE ROM *Bp_Data, BYTE ROM *Bp_ColorMap, WORD XLim)
static void OsdDrawBitmapStripeCustom(BYTE B_TileId, WORD *Wa_CtrlProp1, WORD W_ColourDepth, BYTE ROM *Bp_Pdata, BYTE ROM *Bp_ColorMap,WORD YLim, WORD YPOSSTART)
{
  	WORD Wa_CtrlProp[4];
	WORD xsize;
	//WORD YPOS;
	WORD ypos,ys,xpos;

	WORD W_Size = gm_GmcaCycleDecompress((BYTE far *)Bp_Pdata,
										(BYTE *)gmv_TempBuffer + BITMAP_DATA_OFFSET,
										DecompressWrapper);
	msg_i("Decompressed: W_Size = %d", W_Size);

// Mikhail
	Wa_CtrlProp[0] = Wa_CtrlProp1[0];
	Wa_CtrlProp[1] = Wa_CtrlProp1[1];
	Wa_CtrlProp[2] = Wa_CtrlProp1[2];
	Wa_CtrlProp[3] = Wa_CtrlProp1[3];
	if (!YLim)
		{
//		return ystart;
		}
// END Mikhail

	memset((void *)((BYTE *)gmv_TempBuffer + BITMAP_DATA_OFFSET + W_Size), 0,
			gmvw_TempBufferSize - BITMAP_DATA_OFFSET - W_Size);


	Bp_Pdata = (BYTE far *)gmv_TempBuffer; 
	
	
	if(TEST_OSD_PARAM_ROTATION)
	{		
		switch(W_ColourDepth)
		{
/*			case 1:
				gm_OsdRotate1bppPixelData(
									(Stp_CtrlParams->XSize + 7) / 8,
									Stp_CtrlParams->YSize,
									(BYTE *)gmv_TempBuffer + BITMAP_DATA_OFFSET,
									(BYTE *)gmv_TempBuffer +  2 * BITMAP_DATA_OFFSET + W_Size,
									(Stp_CtrlParams->YSize + 7) / 8);
				break;
*/
			case 2:
				gm_OsdRotate2bppPixelData(
									(CONTROL_WIDTH(Wa_CtrlProp) + 3) / 4,
									CONTROL_HEIGHT(Wa_CtrlProp),
									(BYTE *)gmv_TempBuffer + BITMAP_DATA_OFFSET,
									(BYTE *)gmv_TempBuffer +  2 * BITMAP_DATA_OFFSET + W_Size,
									(CONTROL_HEIGHT(Wa_CtrlProp) + 3) / 4);
				break;

			case 3:
				gm_OsdRotate4bppPixelData(
									(CONTROL_WIDTH(Wa_CtrlProp) + 1) / 2,
									CONTROL_HEIGHT(Wa_CtrlProp),
									(BYTE *)gmv_TempBuffer + BITMAP_DATA_OFFSET,
									(BYTE *)gmv_TempBuffer +  2 * BITMAP_DATA_OFFSET + W_Size,
									(CONTROL_HEIGHT(Wa_CtrlProp) + 1) / 2);
				break;

			case 4:
				gm_OsdRotate8bppPixelData(CONTROL_WIDTH(Wa_CtrlProp),
									CONTROL_HEIGHT(Wa_CtrlProp),
									(BYTE *)gmv_TempBuffer + BITMAP_DATA_OFFSET,
									(BYTE *)gmv_TempBuffer +  2 * BITMAP_DATA_OFFSET + W_Size);
				break;
		}

		Bp_Pdata += BITMAP_DATA_OFFSET + W_Size;
	

//	GetControlParams(Stp_CtrlParams,  &St_DestCtrl);
	
	((ST_OSD_BITMAP_HDR far *)Bp_Pdata)->B_Attr =  (BYTE)W_ColourDepth;
	((ST_OSD_BITMAP_HDR far *)Bp_Pdata)->W_Width = CONTROL_HEIGHT(Wa_CtrlProp);
	((ST_OSD_BITMAP_HDR far *)Bp_Pdata)->W_Height = CONTROL_WIDTH(Wa_CtrlProp);

   ypos=CONTROL_WIDTH(Wa_CtrlProp);
   ypos=0;
   //	Stp_CtrlParams = &St_DestCtrl;
			#if 1
				{
					WORD W_Xsize, W_Ysize, W_temp;
					
				gm_OsdGetTileSize(B_TileId, &W_Xsize, &W_Ysize);

				//save X position
				W_temp  = CONTROL_XPOS(Wa_CtrlProp);
				
				//set X position
				CONTROL_XPOS(Wa_CtrlProp) = CONTROL_YPOS(Wa_CtrlProp);

				//set Y position
				CONTROL_YPOS(Wa_CtrlProp) = W_Ysize - (W_temp - 1) - CONTROL_WIDTH(Wa_CtrlProp) - 1;			
				}
			#endif
	 		ypos += CONTROL_YPOS(Wa_CtrlProp);

////////////////////////////////////////////////////////////
/*			xpos=CONTROL_XPOS(Wa_CtrlProp);         

		ys=BITMAP_HEIGHT(Bp_Pdata);

      	xsize=20;
	   for(;ys;ys--,xpos++)
		{
//	      if (ypos >= (YPOSSTART-YLim))
         {
          ((ST_OSD_BITMAP_HDR far *)Bp_Pdata)->B_Attr=(BYTE)W_ColourDepth;
         
         		  //	BITMAP_WIDTH(Bp_Pdata) = XLim;
//                 Debugmessage_i ("xpos: %d", xpos);
	((ST_OSD_BITMAP_HDR far *)Bp_Pdata)->W_Width = 1;
	((ST_OSD_BITMAP_HDR far *)Bp_Pdata)->W_Height = 20;
           			gm_OsdDrawUncomprImage(B_TileId, xpos,
        								 CONTROL_YPOS(Wa_CtrlProp),
										Bp_Pdata, Bp_ColorMap,1);

         
         }
         Bp_Pdata += xsize;
         ystart=xpos-CONTROL_XPOS(Wa_CtrlProp);
		}
*/
//	BITMAP_WIDTH(Bp_Pdata) = 10;

if (CONTROL_XPOS(Wa_CtrlProp)>= (YPOSSTART-YLim))
{
	ystart=ystart+BITMAP_WIDTH(Bp_Pdata);
	gm_OsdDrawUncomprImage(B_TileId, CONTROL_XPOS(Wa_CtrlProp),
							ypos,
							Bp_Pdata, Bp_ColorMap, 1);
}
else 
{
 if ((CONTROL_XPOS(Wa_CtrlProp)+CONTROL_HEIGHT(Wa_CtrlProp))<(YPOSSTART-YLim))
 {
 
	ystart=ystart+BITMAP_WIDTH(Bp_Pdata);
 }
 else
 {
       ys= BITMAP_HEIGHT(Bp_Pdata);
	ypos=CONTROL_YPOS(Wa_CtrlProp);
	xsize=CONTROL_HEIGHT(Wa_CtrlProp);

    switch(W_ColourDepth)
		{
				case 1: xsize >>= 3;// Veniamin:Move to next line
					break;

				case 2: xsize >>= 2;// Veniamin:Move to next line
					break;

				case 3: xsize >>= 1;// Veniamin:Move to next line
					break;
		}	

	   for(;ys;ys--,ypos++)
		{
          ((ST_OSD_BITMAP_HDR far *)Bp_Pdata)->B_Attr=(BYTE)W_ColourDepth;
         {
         			BITMAP_WIDTH(Bp_Pdata) = CONTROL_HEIGHT(Wa_CtrlProp)-(sliderwidth-YLim-ystart);
						BITMAP_HEIGHT(Bp_Pdata) = 1;

           			gm_OsdDrawUncomprImage(B_TileId, CONTROL_XPOS(Wa_CtrlProp)+(sliderwidth-YLim-ystart),
        								ypos,
										Bp_Pdata, Bp_ColorMap,1);

         }
         Bp_Pdata += xsize;
		}
 }
}

///////////////////////////////////////////////////////////////////////
	}
	else
		{
	BITMAP_WIDTH(Bp_Pdata) = CONTROL_WIDTH(Wa_CtrlProp);
	BITMAP_HEIGHT(Bp_Pdata)= CONTROL_HEIGHT(Wa_CtrlProp);

  	((ST_OSD_BITMAP_HDR far *)Bp_Pdata)->B_Attr =  (BYTE)W_ColourDepth;
	((ST_OSD_BITMAP_HDR far *)Bp_Pdata)->W_Width = CONTROL_WIDTH(Wa_CtrlProp);
	((ST_OSD_BITMAP_HDR far *)Bp_Pdata)->W_Height = CONTROL_HEIGHT(Wa_CtrlProp);

	ys=BITMAP_HEIGHT(Bp_Pdata);

ypos=CONTROL_YPOS(Wa_CtrlProp);
	xsize=CONTROL_WIDTH(Wa_CtrlProp);

    switch(W_ColourDepth)
		{
				case 1: xsize >>= 3;// Veniamin:Move to next line
					break;

				case 2: xsize >>= 2;// Veniamin:Move to next line
					break;

				case 3: xsize >>= 1;// Veniamin:Move to next line
					break;
			}	


                   ((ST_OSD_BITMAP_HDR far *)Bp_Pdata)->B_Attr=(BYTE)W_ColourDepth;


         
	   for(;ys;ys--,ypos++)
		{
	      if (ypos >= (YPOSSTART-YLim))
         {
          ((ST_OSD_BITMAP_HDR far *)Bp_Pdata)->B_Attr=(BYTE)W_ColourDepth;
         
         		  //	BITMAP_WIDTH(Bp_Pdata) = XLim;
                 BITMAP_WIDTH(Bp_Pdata)= CONTROL_WIDTH(Wa_CtrlProp);
						BITMAP_HEIGHT(Bp_Pdata) = 1;
           			gm_OsdDrawUncomprImage(B_TileId, CONTROL_XPOS(Wa_CtrlProp),
        								ypos,
										Bp_Pdata, Bp_ColorMap,1);

         
         }
         Bp_Pdata += xsize;
         ystart=ypos-CONTROL_YPOS(Wa_CtrlProp);
		}


	}

//	gm_OsdDrawUncomprImage(B_TileId, CONTROL_XPOS(Wa_CtrlProp),	YPOS,	Bp_Pdata, Bp_ColorMap, 1);
//
//	gm_OsdDrawUncomprImage(Stp_CtrlParams->TileId, Stp_CtrlParams->XPos,
//							Stp_CtrlParams->YPos,
//							Bp_Data, Bp_ColorMap, 1);	
}
#endif
#endif // DELL_U2410F