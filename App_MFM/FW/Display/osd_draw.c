/*
	$Workfile:   osd_draw.c  $
	$Revision: 1.5 $
	$Date: 2011/12/15 03:12:03 $
*/
#define __OSD_DRAW_C__
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
// MODULE:     	osd_draw.c
//
// USAGE:		This module contains custom drawing functions
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "system\all.h"
#include <embedded.h>

//******************************************************************************
//  E X T E R N S
//******************************************************************************


//******************************************************************************
//  D E F I N E S
//******************************************************************************

//
// Debug print message
//
//#define _DEBUG
#ifdef _DEBUG
#define  msg(a)            		gm_Print(a,0)
#define  msg_i(a,b)        	 	gm_Print(a,b)
#else
#define  msg(a)
#define  msg_i(a,b)
#endif

//
// FOR CUSTOM DRAW CONTROLS
//
#define glyph_zero 				0
#define glyph_volume 			12

#define DEFAULT_COLOR_STATE		0		// DStaticText
#define SELECT_COLOR_STATE		1		// DStaticText

//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************

//******************************************************************************
// S T A T I C   F U N C T I O N S
//******************************************************************************

//******************************************************************************
//  C O D E
//******************************************************************************
//******************************************************************************
// CUSTOM DRAW CONTROL FUNCTIONS
//******************************************************************************

#if defined(CONTROLTYPE_DSTATICTEXT_USED) || defined(CONTROLTYPE_DSTATICTEXTBUTTON_USED)
//******************************************************************************
//
// FUNCTION       : void DrawResultDStaticText(BYTE B_TextIndex, BYTE B_ColorState)
//
// USAGE          :
//
// INPUT          : B_TextIndex
//                  B_ColorState
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawResultDStaticText(BYTE B_TextIndex, BYTE B_ColorState)
{

   DrawTextControl2(B_TextIndex, B_ColorState);
}
#endif



#if defined(CONTROLTYPE_DSTATICTEXTEXT_USED) || defined(CONTROLTYPE_DSTATICTEXTBUTTONEXT_USED)
//******************************************************************************
//
// FUNCTION       : void DrawResultDStaticTextExt(BYTE B_TextIndex, BYTE B_ColorState)
//
// USAGE          :
//
// INPUT          : B_TextIndex
//                  B_ColorState
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawResultDStaticTextExt(BYTE B_TextIndex, BYTE B_ColorState)
{

   DrawTextControl3(B_TextIndex, B_ColorState);
}
#endif

#if defined(CONTROLTYPE_DSTATICTEXTEXT_USED) || defined(CONTROLTYPE_DSTATICTEXTBUTTONEXT_USED)
//******************************************************************************
//
// FUNCTION       : void DrawOption1DStaticTextExt(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption1DStaticTextExt(void)
{
   DrawResultDStaticTextExt(0,DEFAULT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawOption2DStaticTextExt(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption2DStaticTextExt(void)
{
   DrawResultDStaticTextExt(1,DEFAULT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawOption3DStaticTextExt(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption3DStaticTextExt(void)
{
   DrawResultDStaticTextExt(2,DEFAULT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawOption4DStaticTextExt(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption4DStaticTextExt(void)
{
   DrawResultDStaticTextExt(3,DEFAULT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawOption5DStaticTextExt(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption5DStaticTextExt(void)
{
   DrawResultDStaticTextExt(4,DEFAULT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawOption6DStaticTextExt(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption6DStaticTextExt(void)
{
   DrawResultDStaticTextExt(5,DEFAULT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawOption7DStaticTextExt(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption7DStaticTextExt(void)
{
   DrawResultDStaticTextExt(6,DEFAULT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawOption8DStaticTextExt(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption8DStaticTextExt(void)
{
   DrawResultDStaticTextExt(7,DEFAULT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawOption9DStaticTextExt(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption9DStaticTextExt(void)
{
   DrawResultDStaticTextExt(8,DEFAULT_COLOR_STATE);
}
//******************************************************************************
//
// FUNCTION       : void DrawOption10DStaticTextExt(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption10DStaticTextExt(void)
{
   DrawResultDStaticTextExt(9,DEFAULT_COLOR_STATE);
}
//******************************************************************************
//
// FUNCTION       : void DrawOption11DStaticTextExt(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption11DStaticTextExt(void)
{
   DrawResultDStaticTextExt(10,DEFAULT_COLOR_STATE);
}
//******************************************************************************
//
// FUNCTION       : void DrawOption12DStaticTextExt(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption12DStaticTextExt(void)
{
   DrawResultDStaticTextExt(11,DEFAULT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawOption13DStaticTextExt(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption13DStaticTextExt(void)
{
   DrawResultDStaticTextExt(12,DEFAULT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawOption14DStaticTextExt(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption14DStaticTextExt(void)
{
   DrawResultDStaticTextExt(13,DEFAULT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawOption15DStaticTextExt(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption15DStaticTextExt(void)
{
   DrawResultDStaticTextExt(14,DEFAULT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawOption16DStaticTextExt(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption16DStaticTextExt(void)
{
   DrawResultDStaticTextExt(15,DEFAULT_COLOR_STATE);
}


//******************************************************************************
//
// FUNCTION       : void DrawOption17DStaticTextExt(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption17DStaticTextExt(void)
{
   DrawResultDStaticTextExt(16,DEFAULT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawOption18DStaticTextExt(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption18DStaticTextExt(void)
{
   DrawResultDStaticTextExt(17,DEFAULT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawOption19DStaticTextExt(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption19DStaticTextExt(void)
{
   DrawResultDStaticTextExt(18,DEFAULT_COLOR_STATE);
}
//******************************************************************************
//
// FUNCTION       : void DrawOption20DStaticTextExt(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption20DStaticTextExt(void)
{
   DrawResultDStaticTextExt(19,DEFAULT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawOption21DStaticTextExt(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption21DStaticTextExt(void)
{
   DrawResultDStaticTextExt(20,DEFAULT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawOption22DStaticTextExt(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption22DStaticTextExt(void)
{
   DrawResultDStaticTextExt(21,DEFAULT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawOption23DStaticTextExt(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption23DStaticTextExt(void)
{
   DrawResultDStaticTextExt(22,DEFAULT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawOption24DStaticTextExt(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption24DStaticTextExt(void)
{
   DrawResultDStaticTextExt(23,DEFAULT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawOption25DStaticTextExt(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption25DStaticTextExt(void)
{
   DrawResultDStaticTextExt(24,DEFAULT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawOption26DStaticTextExt(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption26DStaticTextExt(void)
{
   DrawResultDStaticTextExt(25,DEFAULT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawOption27DStaticTextExt(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption27DStaticTextExt(void)
{
   DrawResultDStaticTextExt(26,DEFAULT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawOption28DStaticTextExt(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption28DStaticTextExt(void)
{
   DrawResultDStaticTextExt(27,DEFAULT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawOption29DStaticTextExt(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption29DStaticTextExt(void)
{
   DrawResultDStaticTextExt(28,DEFAULT_COLOR_STATE);
}
//******************************************************************************
//
// FUNCTION       : void DrawOption30DStaticTextExt(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption30DStaticTextExt(void)
{
   DrawResultDStaticTextExt(29,DEFAULT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawOption31DStaticTextExt(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption31DStaticTextExt(void)
{
   DrawResultDStaticTextExt(30,DEFAULT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawOption32DStaticTextExt(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption32DStaticTextExt(void)
{
   DrawResultDStaticTextExt(31,DEFAULT_COLOR_STATE);
}


#endif



#ifdef CONTROLTYPE_DSTATICTEXT_USED
//******************************************************************************
//
// FUNCTION       : void DrawOption1DStaticText(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption1DStaticText(void)
{
   DrawResultDStaticText(0,DEFAULT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawOption2DStaticText(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption2DStaticText(void)
{
   DrawResultDStaticText(1,DEFAULT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawOption3DStaticText(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption3DStaticText(void)
{
   DrawResultDStaticText(2,DEFAULT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawOption4DStaticText(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption4DStaticText(void)
{
   DrawResultDStaticText(3,DEFAULT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawOption5DStaticText(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption5DStaticText(void)
{
   DrawResultDStaticText(4,DEFAULT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawOption6DStaticText(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption6DStaticText(void)
{
   DrawResultDStaticText(5,DEFAULT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawOption7DStaticText(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption7DStaticText(void)
{
   DrawResultDStaticText(6,DEFAULT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawOption8DStaticText(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption8DStaticText(void)
{
   DrawResultDStaticText(7,DEFAULT_COLOR_STATE);
}
#endif

#ifdef CONTROLTYPE_DSTATICTEXTBUTTON_USED
//******************************************************************************
//
// FUNCTION       : void DrawOption1DStaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption1DStaticTextButton(void)
{
   DrawResultDStaticText(0,DEFAULT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawOption2DStaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption2DStaticTextButton(void)
{
   DrawResultDStaticText(1,DEFAULT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawOption3DStaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption3DStaticTextButton(void)
{
   DrawResultDStaticText(2,DEFAULT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawOption4DStaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption4DStaticTextButton(void)
{
   DrawResultDStaticText(3,DEFAULT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawOption5DStaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption5DStaticTextButton(void)
{
   DrawResultDStaticText(4,DEFAULT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawOption6DStaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption6DStaticTextButton(void)
{
   DrawResultDStaticText(5,DEFAULT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawOption7DStaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption7DStaticTextButton(void)
{
   DrawResultDStaticText(6,DEFAULT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawOption8DStaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawOption8DStaticTextButton(void)
{
   DrawResultDStaticText(7,DEFAULT_COLOR_STATE);
}


//******************************************************************************
//
// FUNCTION       : void DrawSelectedOption1DStaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawSelectedOption1DStaticTextButton(void)
{
   DrawResultDStaticText(0,SELECT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawSelectedOption2DStaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawSelectedOption2DStaticTextButton(void)
{
   DrawResultDStaticText(1,SELECT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawSelectedOption3DStaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawSelectedOption3DStaticTextButton(void)
{
   DrawResultDStaticText(2,SELECT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawSelectedOption4DStaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawSelectedOption4DStaticTextButton(void)
{
   DrawResultDStaticText(3,SELECT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawSelectedOption5DStaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawSelectedOption5DStaticTextButton(void)
{
   DrawResultDStaticText(4,SELECT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawSelectedOption6DStaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawSelectedOption6DStaticTextButton(void)
{
   DrawResultDStaticText(5,SELECT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawSelectedOption7DStaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawSelectedOption7DStaticTextButton(void)
{
   DrawResultDStaticText(6,SELECT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawSelectedOption8DStaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawSelectedOption8DStaticTextButton(void)
{
   DrawResultDStaticText(7,SELECT_COLOR_STATE);
}

#if defined(CONTROLTYPE_DSTATICTEXTEXT_USED) || defined(CONTROLTYPE_DSTATICTEXTBUTTONEXT_USED)
//******************************************************************************
//
// FUNCTION       : void DrawSelectedOption9DStaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawSelectedOption9DStaticTextButton(void)
{
   DrawResultDStaticTextExt(8,SELECT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawSelectedOption10DStaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawSelectedOption10DStaticTextButton(void)
{
   DrawResultDStaticTextExt(9,SELECT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawSelectedOption11DStaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawSelectedOption11DStaticTextButton(void)
{
   DrawResultDStaticTextExt(10,SELECT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawSelectedOption12DStaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawSelectedOption12DStaticTextButton(void)
{
   DrawResultDStaticTextExt(11,SELECT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawSelectedOption13DStaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawSelectedOption13DStaticTextButton(void)
{
   DrawResultDStaticTextExt(12,SELECT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawSelectedOption14DStaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawSelectedOption14DStaticTextButton(void)
{
   DrawResultDStaticTextExt(13,SELECT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawSelectedOption15DStaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawSelectedOption15DStaticTextButton(void)
{
   DrawResultDStaticTextExt(14,SELECT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawSelectedOption16DStaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawSelectedOption16DStaticTextButton(void)
{
   DrawResultDStaticTextExt(15,SELECT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawSelectedOption17DStaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawSelectedOption17DStaticTextButton(void)
{
   DrawResultDStaticTextExt(16,SELECT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawSelectedOption18DStaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawSelectedOption18DStaticTextButton(void)
{
   DrawResultDStaticTextExt(17,SELECT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawSelectedOption19DStaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawSelectedOption19DStaticTextButton(void)
{
   DrawResultDStaticTextExt(18,SELECT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawSelectedOption20DStaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawSelectedOption20DStaticTextButton(void)
{
   DrawResultDStaticTextExt(19,SELECT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawSelectedOption21DStaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawSelectedOption21DStaticTextButton(void)
{
   DrawResultDStaticTextExt(20,SELECT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawSelectedOption22DStaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawSelectedOption22DStaticTextButton(void)
{
   DrawResultDStaticText(21,SELECT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawSelectedOption23DStaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawSelectedOption23DStaticTextButton(void)
{
   DrawResultDStaticTextExt(22,SELECT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawSelectedOption24DStaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawSelectedOption24DStaticTextButton(void)
{
   DrawResultDStaticTextExt(23,SELECT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawSelectedOption25DStaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawSelectedOption25DStaticTextButton(void)
{
   DrawResultDStaticTextExt(24,SELECT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawSelectedOption26DStaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawSelectedOption26DStaticTextButton(void)
{
   DrawResultDStaticTextExt(25,SELECT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawSelectedOption27DStaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawSelectedOption27DStaticTextButton(void)
{
   DrawResultDStaticTextExt(26,SELECT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawSelectedOption28DStaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawSelectedOption28DStaticTextButton(void)
{
   DrawResultDStaticTextExt(27,SELECT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawSelectedOption29DStaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawSelectedOption29DStaticTextButton(void)
{
   DrawResultDStaticTextExt(28,SELECT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawSelectedOption30DStaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawSelectedOption30DStaticTextButton(void)
{
   DrawResultDStaticTextExt(29,SELECT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawSelectedOption31DStaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawSelectedOption31DStaticTextButton(void)
{
   DrawResultDStaticTextExt(30,SELECT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawSelectedOption32DStaticTextButton(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawSelectedOption32DStaticTextButton(void)
{
   DrawResultDStaticTextExt(31,SELECT_COLOR_STATE);
}
#endif
#endif



BOOL AlanPrinter(BOOL alanbool)
{
   if (alanbool)
   {
      gm_Printf("------------------------------TRUE it is good!",0);
   }
   else
   {
      gm_Printf("------------------------------FALSE it is good!",0);
   }
   return TRUE;
}

#if 1 //defined(CONTROLTYPE_DSTATICTEXTEXT_USED) || defined(CONTROLTYPE_DSTATICTEXTBUTTONEXT_USED) 
//******************************************************************************
//
// FUNCTION       : void DrawTextControl3(BYTE B_TextIndex, BYTE B_ColorState)
//
// USAGE          :
//
// INPUT          : B_TextIndex
//                  B_ColorState
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawTextControl3(BYTE B_TextIndex, BYTE B_ColorState)
{
   BYTE B_LangIndex = gmvb_OsdLanguage;
   BYTE B_NumOfGlyphs = 0;
   BYTE B_NumOfOffset = 0;
   BYTE B_BytesToSkip  = 0;
   BYTE B_NumOfLang;
   BYTE B_Temp;
   WORD W_YOffset = 0;
   WORD W_XOffset = 0;
   WORD W_Offset1, W_Offset2;
   gmt_OSD_DSTATICTEXTEXT_CTRL ROM *Sp_TextCtrl;

   BYTE ROM *Bp_Text;
   BYTE ROM *Bp_ptr;
   BYTE ROM *Bp_ColorMap;
//	BYTE TestChar[500];

   // Set up pointers to the control.
   gm_OsdGetActionOperand();
   //Sp_TextCtrl = DText;
   //Sp_TextCtrl = (gmt_OSD_TEXT_CTRL ROM *)gmv_ActionOpPtr;
   Sp_TextCtrl = (gmt_OSD_DSTATICTEXTEXT_CTRL ROM *)gmv_ActionOpPtr;


   switch (B_TextIndex)
   {
      case 0:
         Bp_Text = Sp_TextCtrl->Text;
         break;
      case 1:
         Bp_Text = Sp_TextCtrl->Text_2;
         break;
      case 2:
         Bp_Text = Sp_TextCtrl->Text_3;
         break;
      case 3:
         Bp_Text = Sp_TextCtrl->Text_4;
         break;
      case 4:
         Bp_Text = Sp_TextCtrl->Text_5;
         break;
      case 5:
         Bp_Text = Sp_TextCtrl->Text_6;
         break;
      case 6:
         Bp_Text = Sp_TextCtrl->Text_7;
         break;
      case 7:
         Bp_Text = Sp_TextCtrl->Text_8;
         break;
      case 8:
         Bp_Text = Sp_TextCtrl->Text_9;
         break;
      case 9:
         Bp_Text = Sp_TextCtrl->Text_10;
         break;
      case 10:
         Bp_Text = Sp_TextCtrl->Text_11;
         break;
      case 11:
         Bp_Text = Sp_TextCtrl->Text_12;
         break;
      case 12:
         Bp_Text = Sp_TextCtrl->Text_13;
         break;
      case 13:
         Bp_Text = Sp_TextCtrl->Text_14;
         break;
      case 14:
         Bp_Text = Sp_TextCtrl->Text_15;
         break;
      case 15:
         Bp_Text = Sp_TextCtrl->Text_16;
         break;
      case 16:
         Bp_Text = Sp_TextCtrl->Text_17;
         break;
      case 17:
         Bp_Text = Sp_TextCtrl->Text_18;
         break;
      case 18:
         Bp_Text = Sp_TextCtrl->Text_19;
         break;
      case 19:
         Bp_Text = Sp_TextCtrl->Text_20;
         break;
      case 20:
         Bp_Text = Sp_TextCtrl->Text_21;
         break;
      case 21:
         Bp_Text = Sp_TextCtrl->Text_22;
         break;
      case 22:
         Bp_Text = Sp_TextCtrl->Text_23;
         break;
      case 23:
         Bp_Text = Sp_TextCtrl->Text_24;
         break;
      case 24:
         Bp_Text = Sp_TextCtrl->Text_25;
         break;
      case 25:
         Bp_Text = Sp_TextCtrl->Text_26;
         break;
      case 26:
         Bp_Text = Sp_TextCtrl->Text_27;
         break;
      case 27:
         Bp_Text = Sp_TextCtrl->Text_28;
         break;
      case 28:
         Bp_Text = Sp_TextCtrl->Text_29;
         break;
      case 29:
         Bp_Text = Sp_TextCtrl->Text_30;
         break;
      case 30:
         Bp_Text = Sp_TextCtrl->Text_31;
         break;
      case 31:
         Bp_Text = Sp_TextCtrl->Text_32;
         break;
   }

   // We only continue to redraw everything if we are not using a hilite
   // window and everything is valid.
   if ( !ParseDrawProperties( B_ColorState, Sp_TextCtrl->Colors ) )
      return;

   Bp_ColorMap = gmv_ColorMap;

   // We uncompress the text string if Gb_Osd_Compr_Txt is set.
   if (gmc_OsdComprTxt)
   {
//		gm_GmcaCycleDecompress((BYTE far *)Bp_Text, TestChar, DecompressWrapper );
//		Bp_Text = (BYTE ROM *)TestChar;
      gm_GmcaCycleDecompress((BYTE far *)Bp_Text, gmv_OsdTextStringDecompBuffer, DecompressWrapper );
      Bp_Text = (BYTE ROM *)gmv_OsdTextStringDecompBuffer;



   }

   for (B_NumOfLang = 0 ; B_NumOfLang <= B_LangIndex ; B_NumOfLang++)
   {
      if (B_NumOfLang)
      {
         // compute the bytes need to skip to get the reference for the
         // next language text string
         B_BytesToSkip += B_NumOfGlyphs + (B_NumOfOffset << 1) + 2;

         if (*(Bp_Text + B_BytesToSkip) & BIT7)
         {
            if ( B_NumOfLang == B_LangIndex )
            {
               // reference to a previous language
               B_LangIndex = *(Bp_Text  + B_BytesToSkip) & (~BIT7);
               B_NumOfLang = 0;
               B_BytesToSkip = 0;
            }
            else
            {
               // If this is not the reference that we want, we only want
               // to skip the one byte reference.
               B_NumOfGlyphs = 0;
               B_BytesToSkip--;
               B_NumOfOffset = 0;
               continue;
            }
         }
      }

      B_NumOfOffset = 0;

      // Get the Number of Glyph Byte
      B_Temp = *(Bp_Text + B_BytesToSkip + 1);

      // if bit 7 is set
      if (B_Temp & BIT7)
      {
         // the next word is the first offset
         Bp_ptr = (BYTE ROM *)Bp_Text + B_BytesToSkip + 2;

         W_Offset1 = *((WORD far *)Bp_ptr);

         // if bit 14 of first offset word is set
         if (W_Offset1 & BIT14)
         {
            // it contains  y position offset
            W_YOffset = W_Offset1 &(~(BIT14 | BIT15));
         }
         else
         {
            // it contains x position offset
            W_XOffset = W_Offset1 &(~(BIT14 | BIT15));
         }

         B_NumOfOffset = 1;

         // if bit 15 of first offset word is set
         if (W_Offset1 & BIT15)
         {
            // the next word is the second offset
            Bp_ptr = (BYTE far *)Bp_Text + B_BytesToSkip + 4;

            W_Offset2 = *((WORD far *)Bp_ptr);

            //it is y offset
            W_YOffset = W_Offset2 &(~(BIT14 | BIT15));

            B_NumOfOffset = 2;
         }
      }

      B_NumOfGlyphs = B_Temp & (~ BIT7);

   }
   // Now that we've collected all the information, draw the text with the
   // appropriate rotation, or do nothing if no text exists.
   if ( B_NumOfGlyphs != 0 )
   {
      DrawRotatedText( (BYTE ROM *)Sp_TextCtrl, Bp_ColorMap, W_XOffset,
                       W_YOffset,  *(Bp_Text + B_BytesToSkip),
                       (Bp_Text + B_BytesToSkip + (B_NumOfOffset << 1) + 2),
                       B_NumOfGlyphs );
   }
}

#endif //CONTROLTYPE_DSTATICTEXTEXT_USED || CONTROLTYPE_DSTATICTEXTBUTTONEXT_USED

#if defined(CONTROLTYPE_DSTATICTEXT_USED) || defined(CONTROLTYPE_DSTATICTEXTBUTTON_USED)
//******************************************************************************
//
// FUNCTION       : void DrawTextControl2(BYTE B_TextIndex, BYTE B_ColorState)
//
// USAGE          :
//
// INPUT          : B_TextIndex
//                  B_ColorState
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawTextControl2(BYTE B_TextIndex, BYTE B_ColorState)
{
   BYTE B_LangIndex = gmvb_OsdLanguage;
   BYTE B_NumOfGlyphs = 0;
   BYTE B_NumOfOffset = 0;
   BYTE B_BytesToSkip  = 0;
   BYTE B_NumOfLang;
   BYTE B_Temp;
   WORD W_YOffset = 0;
   WORD W_XOffset = 0;
   WORD W_Offset1, W_Offset2;
#if ((defined CONTROLTYPE_DSTATICTEXTBUTTON_USED) && !(defined CONTROLTYPE_DSTATICTEXT_USED))
   gmt_OSD_DSTATICTEXTBUTTON_CTRL ROM *Sp_TextCtrl;	//Note: Must be careful, since this function is called by DStaticTextButton, it should be ST_OSD_DSTATICTEXTBUTTON_CTRL
#endif
#ifdef CONTROLTYPE_DSTATICTEXT_USED
   gmt_OSD_DSTATICTEXT_CTRL ROM *Sp_TextCtrl;	//Note: Must be careful, since this function is called by DStaticTextButton, it should be ST_OSD_DSTATICTEXTBUTTON_CTRL
#endif

   BYTE ROM *Bp_Text;
   BYTE ROM *Bp_ptr;
   BYTE ROM *Bp_ColorMap;
//	BYTE TestChar[500];

   // Set up pointers to the control.
   gm_OsdGetActionOperand();
   //Sp_TextCtrl = DText;
   //Sp_TextCtrl = (gmt_OSD_TEXT_CTRL ROM *)gmv_ActionOpPtr;
#if ((defined CONTROLTYPE_DSTATICTEXTBUTTON_USED) && !(defined CONTROLTYPE_DSTATICTEXT_USED))
   Sp_TextCtrl = (gmt_OSD_DSTATICTEXTBUTTON_CTRL ROM *)gmv_ActionOpPtr;
#endif
#ifdef CONTROLTYPE_DSTATICTEXT_USED
   Sp_TextCtrl = (gmt_OSD_DSTATICTEXT_CTRL ROM *)gmv_ActionOpPtr;
#endif

   switch (B_TextIndex)
   {
      case 0:
         Bp_Text = Sp_TextCtrl->Text;
         break;
      case 1:
         Bp_Text = Sp_TextCtrl->Text_2;
         break;
      case 2:
         Bp_Text = Sp_TextCtrl->Text_3;
         break;
      case 3:
         Bp_Text = Sp_TextCtrl->Text_4;
         break;
      case 4:
         Bp_Text = Sp_TextCtrl->Text_5;
         break;
      case 5:
         Bp_Text = Sp_TextCtrl->Text_6;
         break;
      case 6:
         Bp_Text = Sp_TextCtrl->Text_7;
         break;
      case 7:
         Bp_Text = Sp_TextCtrl->Text_8;
         break;
   }

   // We only continue to redraw everything if we are not using a hilite
   // window and everything is valid.
   if ( !ParseDrawProperties( B_ColorState, Sp_TextCtrl->Colors ) )
      return;

   Bp_ColorMap = gmv_ColorMap;

   // We uncompress the text string if Gb_Osd_Compr_Txt is set.
   if (gmc_OsdComprTxt)
   {
//		gm_GmcaCycleDecompress((BYTE far *)Bp_Text, TestChar, DecompressWrapper );
//		Bp_Text = (BYTE ROM *)TestChar;
      gm_GmcaCycleDecompress((BYTE far *)Bp_Text, gmv_OsdTextStringDecompBuffer, DecompressWrapper );
      Bp_Text = (BYTE ROM *)gmv_OsdTextStringDecompBuffer;



   }

   for (B_NumOfLang = 0 ; B_NumOfLang <= B_LangIndex ; B_NumOfLang++)
   {
      if (B_NumOfLang)
      {
         // compute the bytes need to skip to get the reference for the
         // next language text string
         B_BytesToSkip += B_NumOfGlyphs + (B_NumOfOffset << 1) + 2;

         if (*(Bp_Text + B_BytesToSkip) & BIT7)
         {
            if ( B_NumOfLang == B_LangIndex )
            {
               // reference to a previous language
               B_LangIndex = *(Bp_Text  + B_BytesToSkip) & (~BIT7);
               B_NumOfLang = 0;
               B_BytesToSkip = 0;
            }
            else
            {
               // If this is not the reference that we want, we only want
               // to skip the one byte reference.
               B_NumOfGlyphs = 0;
               B_BytesToSkip--;
               B_NumOfOffset = 0;
               continue;
            }
         }
      }

      B_NumOfOffset = 0;

      // Get the Number of Glyph Byte
      B_Temp = *(Bp_Text + B_BytesToSkip + 1);

      // if bit 7 is set
      if (B_Temp & BIT7)
      {
         // the next word is the first offset
         Bp_ptr = (BYTE ROM *)Bp_Text + B_BytesToSkip + 2;

         W_Offset1 = *((WORD far *)Bp_ptr);

         // if bit 14 of first offset word is set
         if (W_Offset1 & BIT14)
         {
            // it contains  y position offset
            W_YOffset = W_Offset1 &(~(BIT14 | BIT15));
         }
         else
         {
            // it contains x position offset
            W_XOffset = W_Offset1 &(~(BIT14 | BIT15));
         }

         B_NumOfOffset = 1;

         // if bit 15 of first offset word is set
         if (W_Offset1 & BIT15)
         {
            // the next word is the second offset
            Bp_ptr = (BYTE far *)Bp_Text + B_BytesToSkip + 4;

            W_Offset2 = *((WORD far *)Bp_ptr);

            //it is y offset
            W_YOffset = W_Offset2 &(~(BIT14 | BIT15));

            B_NumOfOffset = 2;
         }
      }

      B_NumOfGlyphs = B_Temp & (~ BIT7);

   }
   // Now that we've collected all the information, draw the text with the
   // appropriate rotation, or do nothing if no text exists.
   if ( B_NumOfGlyphs != 0 )
   {
      DrawRotatedText( (BYTE ROM *)Sp_TextCtrl, Bp_ColorMap, W_XOffset,
                       W_YOffset,  *(Bp_Text + B_BytesToSkip),
                       (Bp_Text + B_BytesToSkip + (B_NumOfOffset << 1) + 2),
                       B_NumOfGlyphs );
   }
}

#endif //#ifdef CONTROLTYPE_DSTATICTEXTBUTTON_USED

#ifdef CONTROLTYPE_TIMELONGVALUE_USED
//******************************************************************************
//
// FUNCTION       : void far DrawTimeLongValue(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void far DrawTimeLongValue(void)
{
   //gm_Print("Drawing TimeLongValue", 0);
   DrawTimeLongValueControl(LONGVALUE_COLOR_STATE, TRUE);
}

#pragma argsused
//******************************************************************************
//
// FUNCTION       : static WORD far WrapperLongValueUncompress(BYTE *Bp_Src, WORD W_Size)
//
// USAGE          :
//
// INPUT          : Bp_Src
//                  W_Size
//
// OUTPUT         :
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
static WORD far WrapperLongValueUncompress(BYTE *Bp_Src, WORD W_Size)
{
   return  W_Size;
}

#pragma argsused
//******************************************************************************
//
// FUNCTION       : void far DrawTimeLongValueControl(BYTE B_ColorState, BYTE B_DrawFlag)
//
// USAGE          :
//
// INPUT          : B_ColorState
//                  B_DrawFlag
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void far DrawTimeLongValueControl(BYTE B_ColorState, BYTE B_DrawFlag)
{
   gmt_OSD_LONGVALUE_CTRL ROM *Sp_LongValueCtrl;
   BYTE ROM *Bp_ColorMap;
   SDWORD SDW_AdjValue, SDW_AdjMin, SDW_AdjMax;
   SWORD SW_LVMaxValue, SW_LVMinValue;
   BYTE B_NumOfGlyphs = 0;
   BYTE Ba_TextString[20] = {0};  //that's displaying 20 digits
   WORD W_Interval;
   WORD W_XOffset, W_YOffset;
   BYTE ROM *Bp_Text;
   BYTE TestChar[100];

   // Variable Initialization
   gm_OsdGetActionOperand();
   Sp_LongValueCtrl = (gmt_OSD_LONGVALUE_CTRL ROM *)gmv_ActionOpPtr;
   Bp_Text = Sp_LongValueCtrl->UsedGlyphs;

   // Returns true if we need to draw the image and false if it's already
   // been highlighted.
   if ( !ParseDrawProperties( B_ColorState, Sp_LongValueCtrl->Colors ) )
      return;

   SW_LVMaxValue = Sp_LongValueCtrl->MaxValue;
   SW_LVMinValue = Sp_LongValueCtrl->MinValue;

   //calls adujster functions from adjustingEngine.c
   SDW_AdjMax = gm_GetAdjusterMax(Sp_LongValueCtrl->Associate);
   SDW_AdjMin = gm_GetAdjusterMin(Sp_LongValueCtrl->Associate);

   // get current value
   SDW_AdjValue = gm_GetAdjuster(Sp_LongValueCtrl->Associate);

   if (SW_LVMaxValue | SW_LVMinValue)
   {
      // We want to map the associate value to a percentage of the way through
      // our current range.
      W_Interval = (WORD)(SDW_AdjMax - SDW_AdjMin);

      if ( W_Interval != 0 )
      {
         SDW_AdjValue =  (( SDW_AdjValue - SDW_AdjMin ) *
                          ( SW_LVMaxValue - SW_LVMinValue )) / W_Interval
                         + SW_LVMinValue;
      }
      else
      {
         SDW_AdjValue = SW_LVMinValue;
      }
   }

   // if value has been changed in case of updating, do nothing
   if ((!B_DrawFlag) && (SDW_AdjValue == gSDW_PreviousLongValue) &&
         (gmv_ActionOpPtr == gBp_PreviousLongValueActionOpPtr))
   {
      return;
   }

   //	save new values
   gSDW_PreviousLongValue = SDW_AdjValue;
   gBp_PreviousLongValueActionOpPtr = gmv_ActionOpPtr;

   Bp_ColorMap = gmv_ColorMap;

   // We uncompress the text string if Gb_Osd_Compr_Txt is set.
   if (gmc_OsdComprTxt)
   {
      gm_GmcaCycleDecompress((BYTE far *)Bp_Text, TestChar, DecompressWrapper);
      Bp_Text = (BYTE ROM *)TestChar;
   }

   //init font id with the specified font
   gm_OsdSetGlobalFontId(*Bp_Text);

   // If the value we want to display is negative, add the negative glyph.
   if (SDW_AdjValue < 0)
   {
      Ba_TextString[B_NumOfGlyphs++] = Bp_Text[glyph_negative + 2];
      Ba_TextString[B_NumOfGlyphs++] = Bp_Text[glyph_space + 2];
      SDW_AdjValue = -SDW_AdjValue;
   }

   if (SDW_AdjValue < 10)
   {
//		gm_Print("time less than 10", 0);
      Ba_TextString[B_NumOfGlyphs++] = *(Bp_Text + 2 + glyph_zero);
   }

   // convert to glyphs
   B_NumOfGlyphs += NumberToGlyphs((DWORD) SDW_AdjValue,
                                   Ba_TextString + B_NumOfGlyphs,
                                   (BYTE far*)Bp_Text + 2);

   W_XOffset = CalculateXAlignmentOffset(B_NumOfGlyphs,
                                         Sp_LongValueCtrl->Horizontal, Ba_TextString,
                                         *(Bp_Text), Sp_LongValueCtrl->XSize );

   W_YOffset = CalculateYAlignmentOffset(Sp_LongValueCtrl->Vertical,
                                         *(Bp_Text), Sp_LongValueCtrl->YSize);

   // Now that we've collected all the information, draw the text with the
   // appropriate rotation.
   DrawRotatedText((BYTE ROM *)Sp_LongValueCtrl, Bp_ColorMap, W_XOffset,
                   W_YOffset,	*(Bp_Text), Ba_TextString, B_NumOfGlyphs );
}
#endif //#ifdef CONTROLTYPE_TIMELONGVALUE_USED

/*****************************************VOLUME SLIDER*/
#ifdef CONTROLTYPE_VOLUMESLIDER_USED
//******************************************************************************
//
// FUNCTION       : void DrawVolumeSlider(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawVolumeSlider(void)
{
   DrawVolumeSliderControl(LONGVALUE_COLOR_STATE, TRUE);
}

//******************************************************************************
//
// FUNCTION       : void DrawVolumeSliderControl(BYTE B_ColorState, BYTE B_DrawFlag)
//
// USAGE          :
//
// INPUT          : B_ColorState
//                  B_DrawFlag
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawVolumeSliderControl(BYTE B_ColorState, BYTE B_DrawFlag)
{
   gmt_OSD_LONGVALUE_CTRL ROM *Sp_LongValueCtrl;
   BYTE ROM *Bp_ColorMap;
   SDWORD SDW_AdjValue, SDW_AdjMin, SDW_AdjMax;
   SWORD SW_LVMaxValue, SW_LVMinValue;
   BYTE B_NumOfGlyphs = 0;
   BYTE Ba_TextString[100] = {0};  //that's displaying 20 digits
   WORD W_Interval;
   WORD W_XOffset, W_YOffset;
   BYTE ROM *Bp_Text;
   BYTE TestChar[100];
   BYTE i;

   // Variable Initialization
   gm_OsdGetActionOperand();
   Sp_LongValueCtrl = (gmt_OSD_LONGVALUE_CTRL ROM *)gmv_ActionOpPtr;
   Bp_Text = Sp_LongValueCtrl->UsedGlyphs;

   // Returns true if we need to draw the image and false if it's already
   // been highlighted.
   if (!ParseDrawProperties( B_ColorState, Sp_LongValueCtrl->Colors))
   {
      return;
   }

   SW_LVMaxValue = Sp_LongValueCtrl->MaxValue;
   SW_LVMinValue = Sp_LongValueCtrl->MinValue;

   //calls adujster functions from adjustingEngine.c
   SDW_AdjMax = gm_GetAdjusterMax(Sp_LongValueCtrl->Associate);
   SDW_AdjMin = gm_GetAdjusterMin(Sp_LongValueCtrl->Associate);

   // get current value
   SDW_AdjValue = gm_GetAdjuster(Sp_LongValueCtrl->Associate);

   if (SW_LVMaxValue | SW_LVMinValue)
   {
      // We want to map the associate value to a percentage of the way through
      // our current range.
      W_Interval = (WORD)(SDW_AdjMax - SDW_AdjMin);

      if ( W_Interval != 0 )
      {
         SDW_AdjValue =  (( SDW_AdjValue - SDW_AdjMin ) *
                          ( SW_LVMaxValue - SW_LVMinValue )) / W_Interval
                         + SW_LVMinValue;
      }
      else
      {
         SDW_AdjValue = SW_LVMinValue;
      }
   }

   // if value has been changed in case of updating, do nothing
   if ((!B_DrawFlag) && (SDW_AdjValue == gSDW_PreviousLongValue) &&
         (gmv_ActionOpPtr == gBp_PreviousLongValueActionOpPtr))
   {
      return;
   }

   //	save new values
   gSDW_PreviousLongValue = SDW_AdjValue;
   gBp_PreviousLongValueActionOpPtr = gmv_ActionOpPtr;

   Bp_ColorMap = gmv_ColorMap;

   // We uncompress the text string if Gb_Osd_Compr_Txt is set.
   if (gmc_OsdComprTxt)
   {
      gm_GmcaCycleDecompress((BYTE far *)Bp_Text, TestChar, DecompressWrapper);
      Bp_Text = (BYTE ROM *)TestChar;
   }

   //init font id with the specified font
   gm_OsdSetGlobalFontId(*Bp_Text);

   // If the value we want to display is negative, add the negative glyph.
   if (SDW_AdjValue < 0)
   {
      Ba_TextString[B_NumOfGlyphs++] = Bp_Text[glyph_negative + 2];
      Ba_TextString[B_NumOfGlyphs++] = Bp_Text[glyph_space + 2];
      SDW_AdjValue = -SDW_AdjValue;
   }

   for (i=0; i<SDW_AdjValue; i++)
   {
      Ba_TextString[B_NumOfGlyphs++] = *(Bp_Text + 2 + glyph_volume);
   }




   /*	if (SDW_AdjValue < 10)
   	{
   		gm_Print("time less than 10", 0);
   		Ba_TextString[B_NumOfGlyphs++] = *(Bp_Text + 2 + glyph_zero);
   	}
   */


   // convert to glyphs
   //B_NumOfGlyphs += NumberToGlyphs((DWORD) SDW_AdjValue,
   //						Ba_TextString + B_NumOfGlyphs,
   //					(BYTE far*)Bp_Text + 2);

   W_XOffset = CalculateXAlignmentOffset(B_NumOfGlyphs,
                                         Sp_LongValueCtrl->Horizontal, Ba_TextString,
                                         *(Bp_Text), Sp_LongValueCtrl->XSize );

   W_YOffset = CalculateYAlignmentOffset(Sp_LongValueCtrl->Vertical,
                                         *(Bp_Text), Sp_LongValueCtrl->YSize);

   // Now that we've collected all the information, draw the text with the
   // appropriate rotation.
   DrawRotatedText((BYTE ROM *)Sp_LongValueCtrl, Bp_ColorMap, W_XOffset,
                   W_YOffset,	*(Bp_Text), Ba_TextString, B_NumOfGlyphs );
}
#endif //#ifdef CONTROLTYPE_TIMELONGVALUE_USED

//&*&*&*S 20120328:SWRD4002: Kordonm(0)  DDC2Bi Dynamic String Test
#ifdef CONTROLTYPE_DYNAMIC2TEXT_USED	//CONTROLTYPE_DYNAMICTEXT_USED
//******************************************************************************
//
// FUNCTION       : void DrawDynamicText(void)
//
// USAGE          : Text control function wrapper which draws a dynamic text control
//                  in "Default" color state
//
// INPUT          : None
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
void DrawDynamicText(void)
{
   msg("DrawDynamicText");
   DrawDynamicTextControl(TEXT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawSelectedDynamicText(void)
//
// USAGE          : Text control function wrapper which draws a dynamic text control
//                  in "Selected" color state
//
// INPUT          : None
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
void DrawSelectedDynamicText(void)
{
   msg("DrawSelectedDynamicText");
   DrawDynamicTextControl(SELECT_TEXT_COLOR_STATE);
}

//******************************************************************************
//
// FUNCTION       : void DrawDynamicTextControl(WORD W_ColorState)
//
// USAGE          : Draw the dynamic text on the screen
//
// INPUT          : W_ColorState
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
void DrawDynamicTextControl(WORD W_ColorState)
{
   gmt_OSD_DYNAMICTEXT_CTRL ROM * Bp_CtrlPtr;
   BYTE ROM *Bp_ColorMap;
   BYTE B_NumOfGlyphs = 0;
   BYTE Ba_TextString[40] = {0};  //that's displaying 20 digits
   WORD W_XOffset, W_YOffset;
   BYTE ROM *Bp_Text;
   DWORD userText;


   // Set up pointers to the control.
   gm_OsdGetActionOperand();
   Bp_CtrlPtr = (gmt_OSD_DYNAMICTEXT_CTRL ROM *)gmv_ActionOpPtr;
   Bp_Text = Bp_CtrlPtr->UsedGlyphs;

   userText = (DWORD)MK_FP(gm_GetAdjuster(Bp_CtrlPtr->PtrSegment), gm_GetAdjuster(Bp_CtrlPtr->PtrOffset));

   // We only continue to redraw everything if we are not using a hilite
   // window and everything is valid.
   if (!ParseDrawProperties(W_ColorState, Bp_CtrlPtr->Colors))
   {
      return;
   }

   Bp_ColorMap = gmv_ColorMap;

   // We uncompress the text string if Gb_Osd_Compr_Txt is set.
   if (gmc_OsdComprTxt)
   {
      gm_GmcaCycleDecompress((BYTE far *)Bp_Text, gmv_OsdTextStringDecompBuffer, DecompressWrapper);
      Bp_Text = (BYTE ROM *)gmv_OsdTextStringDecompBuffer;
   }

   //init font id with the specified font
   gm_OsdSetGlobalFontId(*Bp_Text);

   B_NumOfGlyphs = Chars2Glyphs ((char far *)userText, Ba_TextString, (Bp_Text+2),Bp_CtrlPtr->MaxString);


   // Calculate offsets for alignment
   W_XOffset = CalculateXAlignmentOffset( B_NumOfGlyphs,
                                          Bp_CtrlPtr->Horizontal,
                                          Ba_TextString,
                                          *Bp_Text,
                                          Bp_CtrlPtr->XSize );

   W_YOffset = CalculateYAlignmentOffset( Bp_CtrlPtr->Vertical,
                                          *Bp_Text,
                                          Bp_CtrlPtr->YSize );


   // Now that we've collected all the information, draw the text with the
   // appropriate rotation.
   DrawRotatedText((BYTE ROM*)Bp_CtrlPtr, Bp_ColorMap,
                   W_XOffset, W_YOffset, *Bp_Text,
                   Ba_TextString, B_NumOfGlyphs );
}

//******************************************************************************
//
// FUNCTION       : BYTE Chars2Glyphs(char far* ch_string, BYTE *gl_string, BYTE ROM*glyph_indices, BYTE B_MaxString)
//
// USAGE          : Converts characters to glyphs
//
// INPUT          : ch_string
//                  gl_string
//                  ROM
//                  B_MaxString
//
// OUTPUT         : BYTE
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
#if 1
BYTE Chars2Glyphs(char far* ch_string, BYTE *gl_string, BYTE ROM*glyph_indices, BYTE B_MaxString)
{
	#define BLANK_OFFSET	0
	#define QUEST_OFFSET	31

	char symbol;
	BYTE i;
	BYTE B_endPosition;

	B_endPosition = B_MaxString-1;

	// Start from end position and keep moving back until non-space char is found.
	// This is to prevent trailing spaces from being included when drawing control.

	while ((ch_string[B_endPosition] == ' ') && (B_endPosition != 0))
	{
		B_endPosition--;
	}

	for (i = 0; i <= B_endPosition; i++)
	{
		symbol = ch_string[i];
		
		if (symbol >= ' ' && symbol <= '~') 
		{
			gl_string[i] = glyph_indices[symbol+BLANK_OFFSET-' ']; //glyph_indices?e?g???A?g?|?e?i¡XT?a?¡M?e?I?A?g?i?E?¢F
		}
		else //unknown symbol then question-mark
		{
			gl_string[i] = glyph_indices[QUEST_OFFSET];
		}
	}

	return i;

	#undef BLANK_OFFSET
	#undef QUEST_OFFSET 
}
#else
BYTE Chars2Glyphs(char far* ch_string, BYTE *gl_string, BYTE ROM*glyph_indices, BYTE B_MaxString)
{
#define BLANK_OFFSET		0
#define AMPERSAND_OFFSET	2
#define QUEST_OFFSET		23
#define CAPITAL_OFFSET		24
#define SMALL_OFFSET		50
#define ACCENT_E_OFFSET	76
#define ACCENT_I_OFFSET	78
#define ACCENT_U_OFFSET	79

   char symbol;
   BYTE i;
   BYTE B_endPosition;

   B_endPosition = B_MaxString-1;

   // Start from end position and keep moving back until non-space char is found.
   // This is to prevent trailing spaces from being included when drawing control.
   while ((ch_string[B_endPosition] == ' ') && (B_endPosition != 0))
   {
      B_endPosition--;
   }

   for (i = 0; i <= B_endPosition; i++)
   {
      symbol = ch_string[i];

      if (symbol >= ' ' && symbol <= '!')
      {
         gl_string[i] = glyph_indices[symbol+BLANK_OFFSET-' '];
      }
      else if (symbol >= '&' && symbol <= ':')
      {
         gl_string[i] = glyph_indices[symbol+AMPERSAND_OFFSET-'&'];
      }
      else if (symbol >= 'A' && symbol <= 'Z')
      {
         gl_string[i] = glyph_indices[symbol+CAPITAL_OFFSET-'A'];
      }
      else if (symbol >= 'a' && symbol <= 'z')
      {
         gl_string[i] = glyph_indices[symbol+SMALL_OFFSET-'a'];
      }
      else if (symbol >= 'è' && symbol <= 'é')
      {
         gl_string[i] = glyph_indices[symbol+ACCENT_E_OFFSET-'è'];
      }
      else if (symbol == 'î')
      {
         gl_string[i] = glyph_indices[ACCENT_I_OFFSET];
      }
      else if (symbol == 'ü')
      {
         gl_string[i] = glyph_indices[ACCENT_U_OFFSET];
      }
      else //unknown symbol then question-mark
      {
         gl_string[i] = glyph_indices[QUEST_OFFSET];
      }
   } // for

   return i;

#undef BLANK_OFFSET
#undef AMPERSAND_OFFSET
#undef QUEST_OFFSET
#undef CAPITAL_OFFSET
#undef SMALL_OFFSET
#undef ACCENT_E_OFFSET
#undef ACCENT_I_OFFSET
#undef ACCENT_U_OFFSET
}
#endif
#endif //#ifdef CONTROLTYPE_DYNAMICTEXT_USED
//&*&*&*E 20120328 : Kordonm(1)  DDC2Bi Dynamic String Test

#ifdef CONTROLTYPE_MOVABLELONGVALUE_USED
//******************************************************************************
//
// FUNCTION       : void DrawMovableLongValueControl(BYTE B_ColorState)
//
// USAGE          :
//
// INPUT          : B_ColorState
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawMovableLongValueControl(BYTE B_ColorState)
{
   gmt_OSD_MOVABLELONGVALUE_CTRL ROM *Sp_LongValueCtrl;
   BYTE ROM *Bp_ColorMap;
   SDWORD SDW_AdjValue, SDW_AdjMin, SDW_AdjMax;
   SWORD SW_LVMaxValue, SW_LVMinValue;
   BYTE B_NumOfGlyphs = 0;
   BYTE Ba_TextString[20] = {0};  //that's displaying 20 digits
   WORD W_Interval;
   WORD W_XOffset, W_YOffset;
   BYTE ROM *Bp_Text;

   // Variable Initialization
   gm_OsdGetActionOperand();
   Sp_LongValueCtrl = (gmt_OSD_MOVABLELONGVALUE_CTRL ROM *)gmv_ActionOpPtr;
   Bp_Text = Sp_LongValueCtrl->UsedGlyphs;

   // Returns true if we need to draw the image and false if it's already
   // been highlighted.
   if (!ParseDrawProperties( B_ColorState, Sp_LongValueCtrl->Colors))
   {
      return;
   }

   SW_LVMaxValue = Sp_LongValueCtrl->MaxValue;
   SW_LVMinValue = Sp_LongValueCtrl->MinValue;

   //calls adujster functions from adjustingEngine.c
   SDW_AdjMax = gm_GetAdjusterMax(Sp_LongValueCtrl->Associate);
   SDW_AdjMin = gm_GetAdjusterMin(Sp_LongValueCtrl->Associate);

   // get current value
   SDW_AdjValue = gm_GetAdjuster(Sp_LongValueCtrl->Associate);

   if (SW_LVMaxValue | SW_LVMinValue)
   {
      // We want to map the associate value to a percentage of the way through
      // our current range.
      W_Interval = (WORD)(SDW_AdjMax - SDW_AdjMin);

      if ( W_Interval != 0 )
      {
         SDW_AdjValue =  (( SDW_AdjValue - SDW_AdjMin ) *
                          ( SW_LVMaxValue - SW_LVMinValue )) / W_Interval
                         + SW_LVMinValue;
      }
      else
      {
         SDW_AdjValue = SW_LVMinValue;
      }
   }

   //	save new values
   gSDW_PreviousLongValue = SDW_AdjValue;
   gBp_PreviousLongValueActionOpPtr = gmv_ActionOpPtr;

   Bp_ColorMap = gmv_ColorMap;

   // We uncompress the text string if Gb_Osd_Compr_Txt is set.
   if (gmc_OsdComprTxt)
   {
      gm_GmcaCycleDecompress((BYTE far *)Bp_Text, gmv_OsdTextStringDecompBuffer, DecompressWrapper);
      Bp_Text = (BYTE ROM *)gmv_OsdTextStringDecompBuffer;
   }

   //init font id with the specified font
   gm_OsdSetGlobalFontId(*Bp_Text);

   // If the value we want to display is negative, add the negative glyph.
   if (SDW_AdjValue < 0)
   {
      Ba_TextString[B_NumOfGlyphs++] = Bp_Text[glyph_negative + 2];
      Ba_TextString[B_NumOfGlyphs++] = Bp_Text[glyph_space + 2];
      SDW_AdjValue = -SDW_AdjValue;
   }

   // convert to glyphs
   B_NumOfGlyphs += NumberToGlyphs((DWORD) SDW_AdjValue,
                                   Ba_TextString + B_NumOfGlyphs,
                                   (BYTE far*)Bp_Text + 2);

   W_XOffset = (WORD)gm_GetAdjuster(Sp_LongValueCtrl->XOffset);
   W_YOffset = (WORD)gm_GetAdjuster(Sp_LongValueCtrl->YOffset);

   // Now that we've collected all the information, draw the text with the
   // appropriate rotation.
   {
      gmt_OSD_MOVABLELONGVALUE_CTRL RAMLongValue;
      //memcpy (&RAMLongValue, Sp_LongValueCtrl, sizeof(RAMLongValue));
      RAMLongValue.TileId = Sp_LongValueCtrl->TileId;
      RAMLongValue.XSize = Sp_LongValueCtrl->XSize;
      RAMLongValue.YSize = Sp_LongValueCtrl->YSize;
      RAMLongValue.XPos = W_XOffset;
      RAMLongValue.YPos = W_YOffset;

      W_XOffset = CalculateXAlignmentOffset(B_NumOfGlyphs,
                                            Sp_LongValueCtrl->Horizontal, Ba_TextString,
                                            *(Bp_Text), Sp_LongValueCtrl->XSize );

      W_YOffset = CalculateYAlignmentOffset(Sp_LongValueCtrl->Vertical,
                                            *(Bp_Text), Sp_LongValueCtrl->YSize);

      DrawRotatedText((BYTE far *)&RAMLongValue, Bp_ColorMap, W_XOffset,
                      W_YOffset,	*(Bp_Text), Ba_TextString, B_NumOfGlyphs );


   }
}

//******************************************************************************
//
// FUNCTION       : void DrawMovableLongValue(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DrawMovableLongValue(void)
{
   DrawMovableLongValueControl(0);
}
#endif

#ifdef CONTROLTYPE_UNICODETEXT_USED
// THIS IS JUST AN EXAMPLE
// IT SHOULD HAVE A REAL BODY
WORD const far KoreanStringExample[] = {0xe020, 0xe021, 0xe022, 0xe023, 0xe024, 0xe025};
void GetUnicodeStringInfo(WORD const far **Wp_String, WORD *W_StringLength, BYTE *B_InterGlyphSpace)
{
   *Wp_String = (WORD const far *)KoreanStringExample;
   *W_StringLength = sizeof(KoreanStringExample)/sizeof(WORD);
   *B_InterGlyphSpace = 4;
}
#endif //#ifdef CONTROLTYPE_UNICODETEXT_USED



//*********************************  END  **************************************
