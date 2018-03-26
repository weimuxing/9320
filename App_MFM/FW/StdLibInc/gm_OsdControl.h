/*
	$Workfile:   gm_OsdControl.h  $
	$Revision: 1.3 $
	$Date: 2011/12/14 03:44:47 $
*/
#ifndef __OSD_CNTRL_H__
#define __OSD_CNTRL_H__
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
// MODULE:      osd_cntrl.h
//
// USAGE:
//
//******************************************************************************

//******************************************************************************
//  T Y P E    D E F I N I T I O N S
//******************************************************************************

//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************
//color fragments control byte
enum
{
   COLOR_FRAG_STOP_BIT		= BIT7, //Bit 7: Stop bit (1 indicates this is the last)
   COLOR_FRAG_BIT			= BIT6, //Bit 6: 1 indicates a pointer to color fragment
   //Bit 6: 0 indicates that the color data is embedded in this structure.
   COLORS_NUM_BITS			= 0x1F	//Bits [5:0]: number of colors in the color fragment
};

// control bits in Sp_TileCtrl->DialogId
enum
{
   TILE_DIALOG_INFO_BIT = BIT7,	// dialog information
   TILE_DIALOG_ID_BITS = 0x7F
};

// flag of unadjustable tile
#define UNADJUSTABLE_BIT BIT15

// returns nonzero if we need to change control parameters in the case or rotation on 90 or 270 degrees
#define TEST_OSD_PARAM_ROTATION ((gmvb_OsdRotation == OSD_ROTATED_90) | (gmvb_OsdRotation == OSD_ROTATED_270))

#define BITMAP_CLR_DEPTH_BITS		0x07
#define BITMAP_FRAGMENTS_BIT		BIT7

#define BITMAP_LAST_FRAGMENT_BIT	BIT15
#define BITMAP_SPEC_SIZE_BIT		BIT13
#define BITMAP_FRAGMENT_SIZE_BITS	0x1FFF

// enumeration for control byte of color map
enum
{
   COLOR_MAP_STOP_BIT		= BIT7,	// Stop bit (1 indicates this is the last color state in the sequence)
   COLOR_REF_BIT			= BIT6, // 1 indicates a reference to another color state; the remaining 6 bits contain the index of the color state we are referencing.
   COLOR_FRAGMENT_BIT		= BIT5,	// 1 indicates that the color fragment flag is present
   FORCE_REDRAW_BIT		= BIT4,	// force redraw if 1
   OVWR_TILE_TRANSP_BIT	= BIT3,	// 1 indicates to overwtite tile transparency settings
   HGLT_TRANSP_BIT			= BIT2,	// 1 indicates to force transparency in the highlight window
   HGLT_STATE_BIT			= BIT1,	// 1 indicates that the state is shown with a highlight window
   COLOR_TRANSL_BIT		= BIT0,	// 1 indicates that color translation table array is present
   COLOR_STATE_INDEX		= 0x3F	// color state index in the case COLOR_REF_BIT = 1
};

// enumeration for highlight window control byte of color map
enum
{
   HGLT_BLINK_BIT				= BIT7, // blinking setting in a hl. window if BIT6 is 0
   USE_TILE_BLINK_BIT			= BIT6, // use the same blinking setting as used in a tile
   HGLT_TRANSP_BEHAV_BIT		= BIT5, // transperency behaviour for a hl. window if BIT4 is 0
   USE_TILE_TRANSP_BEHAV_BIT	= BIT4, // use the same transparency behaviour as used in a tile
   HGLT_CLR_INDEX_MASK			= 0x0F
};

// Define color states
#define BITMAP_COLOR_STATE              0
#define SELECT_BITMAP_COLOR_STATE       1
#define DISABLE_BITMAP_COLOR_STATE      2

#define LONGVALUE_COLOR_STATE           0
#define SELECT_LONGVALUE_COLOR_STATE    1
#define DISABLE_LONGVALUE_COLOR_STATE   2

#define RECTANGLE_COLOR_STATE           0
#define SELECT_RECTANGLE_COLOR_STATE    1
#define DISABLE_RECTANGLE_COLOR_STATE   2

#define SLIDER_COLOR_STATE     			0
#define SELECT_SLIDER_COLOR_STATE       		1
#define DISABLE_SLIDER_COLOR_STATE      		2

#define TEXT_COLOR_STATE       			0
#define SELECT_TEXT_COLOR_STATE         		1
#define DISABLE_TEXT_COLOR_STATE        		2

//get the total string height in pixels
#define TOTAL_FONT_HEIGHT(Bp_FontHeader) \
					((*((const WORD far *)(Bp_FontHeader))) & 0x7F)
//get inter character spacing size in pixels
#define FONT_INTER_GLYPH_SPACE(Bp_FontHeader) \
					(((*((const WORD far *)(Bp_FontHeader))) & 0x780) >> 7)

// get the glyph width (Bp_glyphHeader - a byte pointer to the glyph header)
#define GLYPH_WIDTH(Bp_GlyphHeader) (Bp_GlyphHeader[0] & 0x7f)
// get the glyph left offset (Bp_GlyphHeader - a byte pointer to the glyph header)
#define GLYPH_LEFT_OFFSET(Bp_GlyphHeader) (Bp_GlyphHeader[2] & 0x0f)
// get the glyph right offset (Bp_GlyphHeader - a byte pointer to the glyph header)
#define GLYPH_RIGHT_OFFSET(Bp_GlyphHeader) ((Bp_GlyphHeader[2] >> 4) & 0x0f)
// get the glyph top offset (Bp_GlyphHeader - a byte pointer to the glyph header)
#define GLYPH_TOP_OFFSET(Bp_GlyphHeader) (Bp_GlyphHeader[1] & 0x0f)
// get the glyph bottom offset (Bp_GlyphHeader - a byte pointer to the glyph header)
#define GLYPH_BOTTOM_OFFSET(Bp_GlyphHeader) ((Bp_GlyphHeader[1] >> 4) & 0x0f);
// get the total glyph width (Bp_GlyphHeader - a byte pointer to the glyph header)
#define TOTAL_GLYPH_WIDTH(Bp_GlyphHeader) \
					(GLYPH_WIDTH(Bp_GlyphHeader) + \
					GLYPH_LEFT_OFFSET(Bp_GlyphHeader) + \
					GLYPH_RIGHT_OFFSET(Bp_GlyphHeader))

#define ALL_TIMINGINFO				0
#define RESOLUTION_TIMINGINFO		1
#define VFREQ_TIMINGINFO			2
#define HFREQ_TIMINGINFO			3

//used for TimingInfo control
#define glyph_sp	10
#define glyph_dot	11
#define glyph_x 	12
#define glyph_K		13
#define glyph_H		14
#define glyph_z		15
#define glyph_at	16

//used for LongValue controls
#define glyph_space 	10
#define glyph_positive 	11
#define glyph_negative 	12
#define glyph_percent 	13

// DrawFlag bits for DrawRotatedSlider
enum
{
   DRAW_SLIDER_BORDER_BIT = BIT0,	// draws border
   DRAW_SLIDER_BACKGROUND_BIT = BIT1, // draws background
   DRAW_SLIDER_FOREGROUND_BIT = BIT2, // draws foreground
   DRAW_WHOLE_SLIDER_BIT = BIT3, // draws whole slider bars
   UPDATE_SLIDER_VALUE_BIT = BIT4 // update slider from previous value
};

//debug output
//#define SLIDER_DEBUG
//#define TEXT_DEBUG
//#define TILECONTROL_DEBUG
//#define TIMINGINFO_DEBUG

//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************
extern BYTE ROM *gBp_ColorFrag;
extern BYTE gBa_TileHgltWnd[MAX_NUMBER_OF_TILES];

extern SDWORD gSDW_PreviousLongValue;
extern BYTE ROM *gBp_PreviousLongValueActionOpPtr;

extern WORD gW_PreviousSliderValue;
extern BYTE ROM *gBp_PreviousSliderActionOpPtr;

extern WORD gW_PreviousBitmapSliderValue;
extern BYTE ROM *gBp_PreviousBitmapSliderActionOpPtr;

//******************************************************************************
//  E X T E R N A L    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
void DrawRotatedText(BYTE ROM *Bp_CtrlPtr, BYTE ROM *Bp_ColorMap,
                     WORD W_XOffset, WORD W_YOffset, BYTE B_FontId,
                     BYTE ROM *Bp_Text, BYTE B_NumOfGlyphs);

BOOL ParseDrawProperties(BYTE B_ColorState, BYTE ROM *Bp_ColorMap);
void GetControlParams(gmt_OSD_BASE_CTRL ROM *Sp_SrcCtrl,
                      gmt_OSD_BASE_CTRL *Sp_DestCtrl);
void TileHighlight(gmt_OSD_BASE_CTRL *Stp_ParamCtrl,
                   gmt_OSD_HIGHLIGHT_CTRL *Stp_HgltCtrl);

#endif // __OSD_CNTRL__
//*********************************  END  **************************************
