/*
	$Workfile:   MultiDisplay.c  $
	$Revision: 1.4 $
	$Date: 2011/12/15 07:49:13 $
*/
#define __MULTIDISPLAY_C__
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
//=============================================================
//
// MODULE:		MultiDisplay.c
//
// USAGE:		This module contains the calculations of window attributes for the
//				multi-display feature.
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "System\all.h"

//******************************************************************************
//  D E F I N E S
//******************************************************************************

#define ABS_DIFF(a,b)			((a>b)?(a-b):(b-a))

//******************************************************************************
//  S T A T I C    F U N C T I O N S
//******************************************************************************

static void CalMultiDisplayWinAttrib(BYTE B_WinNum, WORD W_OutputImageOffset,
                                     DWORD DW_OutputImageActive, WORD W_PanelActive,	WORD W_PanelBezelActive,
                                     WORD* Wp_InputStart, WORD* Wp_InputActive, WORD* Wp_OutputWinStart,
                                     WORD* Wp_OutputWinActive);

static BYTE B_NumOfCol;
static WORD W_PanelBezelW;
static WORD W_PanelBezelH;
static WORD W_PanelW;
static WORD W_PanelH;

static DWORD DW_OutputImageW;
static DWORD DW_OutputImageH;
static WORD W_OutputLeftOffset;
static WORD W_OutputTopOffset;

//******************************************************************************
//  C O D E
//******************************************************************************

//******************************************************************************
//
// FUNCTION       : void MultiDisplaySetup(WORD W_OutputLeftBorderWidth,
//						WORD W_OutputRightBorderWidth, WORD W_OutputTopBorderHeight,
//						WORD W_OutputBottomBorderHeight, WORD W_PanelWidth,
//						WORD W_PanelHeight, WORD W_PanelBezelWidth,
//						WORD W_PanelBezelHeight, BYTE B_NumOfColumn,
//						BYTE B_NumOfRow)
//
// USAGE          : Set up static variables to be used by GetMultiDisplayWinAttrib(). This
//					function should be called every time the multi-display configuration
//					is changed.
//
// INPUT          : W_OutputLeftBorderWidth - offset the output image should be away
//												from the left panel bezel
//					W_OutputRightBorderWidth - offset the output image should be away
//												from the right panel bezel
//					W_OutputTopBorderHeight - offset the output image should be away
//												from the top panel bezel
//					W_OutputBottomBorderHeight - offset the output image should be away
//												from the bottom panel bezel
//					W_PanelWidth - panel width
//					W_PanelHeight - panel height
//					W_PanelBezelWidth - inter-panel bezel width
//					W_PanelBezelHeight - inter-panel bezel height
//					B_NumOfColumn - num of columns in this multi-display configuration
//					B_NumOfRow - num of rows in this multi-display configuration
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION : Should call GetMultiDisplayWinAttrib() to get the window attributes
//
//******************************************************************************
void MultiDisplaySetup(WORD W_OutputLeftBorderWidth, WORD W_OutputRightBorderWidth,
                       WORD W_OutputTopBorderHeight, WORD W_OutputBottomBorderHeight,
                       WORD W_PanelWidth, WORD W_PanelHeight, WORD W_PanelBezelWidth,
                       WORD W_PanelBezelHeight, BYTE B_NumOfColumn, BYTE B_NumOfRow)
{
   DWORD DW_TotalOutputW;
   DWORD DW_TotalOutputH;

   if ((W_PanelWidth==0) || (W_PanelHeight==0) || (B_NumOfColumn==0) || (B_NumOfRow==0))
      return;

   W_PanelH = W_PanelHeight;
   W_PanelW = W_PanelWidth;
   W_PanelBezelW = W_PanelBezelWidth;
   W_PanelBezelH = W_PanelBezelHeight;
   B_NumOfCol = B_NumOfColumn;
   W_OutputLeftOffset = W_OutputLeftBorderWidth;
   W_OutputTopOffset = W_OutputTopBorderHeight;

   DW_TotalOutputW = W_PanelW + (W_PanelW + W_PanelBezelW) * (B_NumOfColumn - 1);
   DW_TotalOutputH = W_PanelH + (W_PanelH + W_PanelBezelH) * (B_NumOfRow - 1);
   if (DW_TotalOutputW >= W_OutputLeftBorderWidth + W_OutputRightBorderWidth)
      DW_OutputImageW = DW_TotalOutputW - W_OutputLeftBorderWidth - W_OutputRightBorderWidth;
   else
      return;
   if (DW_TotalOutputH >= W_OutputTopBorderHeight + W_OutputBottomBorderHeight)
      DW_OutputImageH = DW_TotalOutputH - W_OutputTopBorderHeight - W_OutputBottomBorderHeight;
   else
      return;

   //if input aspect ratio is to be maintained for the output
   //if ((W_InputHActive/W_InputVActive) < (DW_OutputImageWidth/DW_OutputImageHeight))
   //alternatively...
   /*	if ((W_InputHActive * DW_OutputImageH) < (DW_OutputImageW * W_InputVActive))
   		DW_OutputImageW = DW_OutputImageH * W_InputHActive / W_InputVActive;
   	else
   		DW_OutputImageH = DW_OutputImageW * W_InputVActive / W_InputHActive;
   */
}

//******************************************************************************
//
// FUNCTION       : void GetMultiDisplayWinAttrib(BYTE B_WinNum,
//						gmt_WindowStruct* pInputWinDim, gmt_WindowStruct* pOutputWinDim)
//
// USAGE          : Wrapper function for CalMultiDisplayWinAttrib() to calculate the
//						input/output window attributes for the window B_WinNum
//
// INPUT          : B_WinNum - designated window number as referred to by OSD. The
//								range of this number is from zero to (max_num_of_win-1)
//								corresponding to a multi-display configuration.
//					pInputWinDim - pointer to a structure that stores the current input
//									attributes which may be updated by
//									CalMultiDisplayWinAttrib()
//					pOutputWinDim - pointer to a structure that stores the new output
//									attributes calculated by CalMultiDisplayWinAttrib()
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  : MultiDisplaySetup() is called to initialize all static variables
//
// POST_CONDITION :
//
//******************************************************************************
void GetMultiDisplayWinAttrib(BYTE B_WinNum,
                              gmt_WindowStruct* pInputWinDim, gmt_WindowStruct* pOutputWinDim)
{
   BYTE B_ColNum;
   BYTE B_RowNum;

   if (B_NumOfCol == 0)
      return;

   B_ColNum = B_WinNum % B_NumOfCol;
   B_RowNum = B_WinNum / B_NumOfCol;

   //Horizontal
   CalMultiDisplayWinAttrib(B_ColNum, W_OutputLeftOffset, DW_OutputImageW,
                            W_PanelW,	W_PanelBezelW, &pInputWinDim->W_Hstart, &pInputWinDim->W_Width,
                            &pOutputWinDim->W_Hstart, &pOutputWinDim->W_Width);

   //Vertical
   CalMultiDisplayWinAttrib(B_RowNum, W_OutputTopOffset, DW_OutputImageH,
                            W_PanelH, W_PanelBezelH, &pInputWinDim->W_Vstart, &pInputWinDim->W_Height,
                            &pOutputWinDim->W_Vstart, &pOutputWinDim->W_Height);
}

//******************************************************************************
//
// FUNCTION       : static void CalMultiDisplayWinAttrib(BYTE B_WinNum,
//								WORD W_OutputImageOffset, DWORD DW_OutputImageActive,
//								WORD W_PanelActive,	WORD W_PanelBezelActive,
//								WORD* Wp_InputStart, WORD* Wp_InputActive,
//								WORD* Wp_OutputWinStart, WORD* Wp_OutputWinActive)
//
// USAGE          :	Calculates the input and output window dimension of window
//					B_WinNum in a multi-display configuration. All references to
//					vertical/horizontal or row/column are removed since this logic
//					is generic enough to be used in both cases.
//
// INPUT          :	B_WinNum - designated window number. Range of this number is
//								from zero to (max_num_of_col-1 or max_num_of_row-1)
//					W_OutputImageOffset - offset of the output image from top/left edge of the
//									panel (in pixels). This value can be larger than the
//									height/width of a panel.
//					DW_OutputImageActive - total output image width/height. This value
//											includes: 1) the whole/partial panel size that
//											the output image spans across; and 2) the
//											inter-panel bezel widths. (in pixels)
//					W_PanelActive - panel width/height (in pixels)
//					W_PanelBezelActive - inter-panel bezel width/height (in pixels)
//					Wp_InputStart - input [H|V]Start to be modified
//					Wp_InputActive - input [H|V]Active to be modified
//					Wp_OutputWinStart - output [H|V]Start to be modified with respect to
//										window B_WinNum
//					Wp_OutputWinActive - output [H|V]Active to be modified with respect to
//										window B_WinNum
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
static void CalMultiDisplayWinAttrib(BYTE B_WinNum, WORD W_OutputImageOffset,
                                     DWORD DW_OutputImageActive, WORD W_PanelActive,	WORD W_PanelBezelActive, WORD* Wp_InputStart,
                                     WORD* Wp_InputActive, WORD* Wp_OutputWinStart, WORD* Wp_OutputWinActive)
{
   if ((W_OutputImageOffset > (B_WinNum * (W_PanelActive + W_PanelBezelActive) + W_PanelActive))
         || ((W_OutputImageOffset + DW_OutputImageActive) < (B_WinNum * (W_PanelActive + W_PanelBezelActive))))
   {
      *Wp_OutputWinActive = 0;
   }
   else
   {
      //the left/top edge
      if ((W_OutputImageOffset >= (B_WinNum * (W_PanelActive + W_PanelBezelActive)))
            && (W_OutputImageOffset <= (B_WinNum + 1) * (W_PanelActive + W_PanelBezelActive)))
      {
         //if output image size is within one panel size
         if (DW_OutputImageActive+(W_OutputImageOffset - B_WinNum * (W_PanelActive + W_PanelBezelActive)) <= W_PanelActive)
            *Wp_OutputWinActive = (WORD)DW_OutputImageActive;
         else
            *Wp_OutputWinActive = W_PanelActive - (W_OutputImageOffset - (B_WinNum * (W_PanelActive + W_PanelBezelActive)));
         *Wp_OutputWinStart = W_OutputImageOffset - (B_WinNum * (W_PanelActive + W_PanelBezelActive));
         gm_Printf("PanelW=%d,outputimageoffset=%d,winnum=%d,panelactive+bezel=%d",W_PanelActive , W_OutputImageOffset,B_WinNum,W_PanelActive + W_PanelBezelActive);
         *Wp_InputStart += 0;
      }
      //the right/bottom edge
      else if (((W_OutputImageOffset + DW_OutputImageActive) >= (B_WinNum * (W_PanelActive + W_PanelBezelActive)))
               && ((W_OutputImageOffset + DW_OutputImageActive) <= (B_WinNum + 1) * (W_PanelActive + W_PanelBezelActive)))
      {
         *Wp_OutputWinActive = (WORD)(W_OutputImageOffset + DW_OutputImageActive - B_WinNum * ((DWORD)W_PanelActive + W_PanelBezelActive));
         *Wp_OutputWinStart = 0;
         *Wp_InputStart += (WORD)((B_WinNum * ((DWORD)W_PanelActive + W_PanelBezelActive) - W_OutputImageOffset) * (*Wp_InputActive)
                                  / DW_OutputImageActive);
      }
      //somewhere in between
      else
      {
         *Wp_OutputWinActive = W_PanelActive;
         *Wp_OutputWinStart = 0;
         *Wp_InputStart += (WORD)((B_WinNum * ((DWORD)W_PanelActive + W_PanelBezelActive) - W_OutputImageOffset) * (*Wp_InputActive)
                                  / DW_OutputImageActive);
      }
   }

   *Wp_InputActive = (WORD)(*Wp_OutputWinActive * (DWORD)*Wp_InputActive
                            / DW_OutputImageActive);
}
