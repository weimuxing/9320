/*
	$Workfile:   osd_draw.h  $
	$Revision: 1.3 $
	$Date: 2011/12/15 03:12:03 $
*/
#ifndef __OSD_DRAW_H__
#define __OSD_DRAW_H__
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
// MODULE:      osd_draw.h
//
// USAGE:
//
//******************************************************************************

//******************************************************************************
//  T Y P E    D E F I N I T I O N S
//******************************************************************************

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************

//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************

//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************

//******************************************************************************
//  E X T E R N A L    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

BOOL AlanTestingOSD(void);
BOOL AlanPrinter(BOOL alanbool);

#if defined(CONTROLTYPE_DSTATICTEXTEXT_USED) || defined(CONTROLTYPE_DSTATICTEXTBUTTONEXT_USED)
void DrawOption1DStaticTextExt(void);
void DrawOption2DStaticTextExt(void);
void DrawOption3DStaticTextExt(void);
void DrawOption4DStaticTextExt(void);
void DrawOption5DStaticTextExt(void);
void DrawOption6DStaticTextExt(void);
void DrawOption7DStaticTextExt(void);
void DrawOption8DStaticTextExt(void);
void DrawOption9DStaticTextExt(void);
void DrawOption10DStaticTextExt(void);
void DrawOption11DStaticTextExt(void);
void DrawOption12DStaticTextExt(void);
void DrawOption13DStaticTextExt(void);
void DrawOption14DStaticTextExt(void);
void DrawOption15DStaticTextExt(void);
void DrawOption16DStaticTextExt(void);
void DrawOption17DStaticTextExt(void);
void DrawOption18DStaticTextExt(void);
void DrawOption19DStaticTextExt(void);
void DrawOption20DStaticTextExt(void);
void DrawOption21DStaticTextExt(void);
void DrawOption22DStaticTextExt(void);
void DrawOption23DStaticTextExt(void);
void DrawOption24DStaticTextExt(void);
void DrawOption25DStaticTextExt(void);
void DrawOption26DStaticTextExt(void);
void DrawOption27DStaticTextExt(void);
void DrawOption28DStaticTextExt(void);
void DrawOption29DStaticTextExt(void);
void DrawOption30DStaticTextExt(void);
void DrawOption31DStaticTextExt(void);
void DrawOption32DStaticTextExt(void);
#endif

void DrawOption1DStaticText(void);
void DrawOption2DStaticText(void);
void DrawOption3DStaticText(void);
void DrawOption4DStaticText(void);
void DrawOption5DStaticText(void);
void DrawOption6DStaticText(void);
void DrawOption7DStaticText(void);
void DrawOption8DStaticText(void);

#ifdef CONTROLTYPE_DSTATICTEXTBUTTON_USED
void DrawOption1DStaticTextButton(void);
void DrawOption2DStaticTextButton(void);
void DrawOption3DStaticTextButton(void);
void DrawOption4DStaticTextButton(void);
void DrawOption5DStaticTextButton(void);
void DrawOption6DStaticTextButton(void);
void DrawOption7DStaticTextButton(void);
void DrawOption8DStaticTextButton(void);

void DrawSelectedOption1DStaticTextButton(void);
void DrawSelectedOption2DStaticTextButton(void);
void DrawSelectedOption3DStaticTextButton(void);
void DrawSelectedOption4DStaticTextButton(void);
void DrawSelectedOption5DStaticTextButton(void);
void DrawSelectedOption6DStaticTextButton(void);
void DrawSelectedOption7DStaticTextButton(void);
void DrawSelectedOption8DStaticTextButton(void);
void DrawSelectedOption9DStaticTextButton(void);
void DrawSelectedOption10DStaticTextButton(void);
void DrawSelectedOption11DStaticTextButton(void);
void DrawSelectedOption12DStaticTextButton(void);
void DrawSelectedOption13DStaticTextButton(void);
void DrawSelectedOption14DStaticTextButton(void);
void DrawSelectedOption15DStaticTextButton(void);
void DrawSelectedOption16DStaticTextButton(void);
void DrawSelectedOption17DStaticTextButton(void);
void DrawSelectedOption18DStaticTextButton(void);
void DrawSelectedOption19DStaticTextButton(void);
void DrawSelectedOption20DStaticTextButton(void);
void DrawSelectedOption21DStaticTextButton(void);
void DrawSelectedOption22DStaticTextButton(void);
void DrawSelectedOption23DStaticTextButton(void);
void DrawSelectedOption24DStaticTextButton(void);
void DrawSelectedOption25DStaticTextButton(void);
void DrawSelectedOption26DStaticTextButton(void);
void DrawSelectedOption27DStaticTextButton(void);
void DrawSelectedOption28DStaticTextButton(void);
void DrawSelectedOption29DStaticTextButton(void);
void DrawSelectedOption30DStaticTextButton(void);
void DrawSelectedOption31DStaticTextButton(void);
void DrawSelectedOption32DStaticTextButton(void);
#endif


void DrawTextControl3(BYTE B_TextIndex, BYTE B_ColorState);
void DrawTextControl2(BYTE B_TextIndex, BYTE B_ColorState);

void DrawTimeLongValue(void);
void DrawTimeLongValueControl(BYTE B_ColorState, BYTE B_DrawFlag);

void DrawVolumeSlider(void);
void DrawVolumeSliderControl(BYTE B_ColorState, BYTE B_DrawFlag);

void DrawDynamicText(void);
void DrawSelectedDynamicText(void);
void DrawDynamicTextControl(WORD W_ColorState);
BYTE Chars2Glyphs(char far* ch_string, BYTE *gl_string, BYTE ROM*glyph_indices, BYTE B_MaxString);
void DrawMovableLongValue(void);



#endif __OSD_DRAW_H__
//*********************************  END  **************************************

