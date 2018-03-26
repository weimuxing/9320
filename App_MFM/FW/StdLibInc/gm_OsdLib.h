/*
	$Workfile:   gm_OsdLib.h  $
	$Revision: 1.5 $
	$Date: 2012/04/19 08:40:40 $
*/
#ifndef __OSDLIB_H__
#define __OSDLIB_H__
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
// MODULE:     	OsdLib.h
//
// USAGE:		Header file for all Standard Library function to be
// 				access from application project.
//
//******************************************************************************

//******************************************************************************
//  A D J U S T I N G    E N G I N E
//******************************************************************************

//******************************************************************************
//  T Y P E    D E F I N I T I O N S
//******************************************************************************
#ifdef __PARADIGM__
#pragma pack(push)
#pragma pack(1)
#endif //__PARADIGM__

typedef struct ST_OSD_SET_ADJUSTER_CTRL_STRUCT
{
   WORD	W_AdjusterId;
   WORD	W_Value;
} gmt_OSD_SET_ADJUSTER_CTRL;

typedef struct ST_OSD_ADJUSTERS_GROUP_STRUCT
{
   void	*Vp_ptr;
} gmt_OSD_ADJUSTERS_GROUP;

typedef struct ST_OSD_ADJUSTERS_MODE_SWITCHERS_STRUCT
{
   BYTE	*Bp_Switcher;
   BYTE	B_NumberOfModes;
} gmt_OSD_ADJUSTERS_MODE_SWITCHERS;

#ifdef __PARADIGM__
#pragma pack(pop)
#endif //__PARADIGM__

typedef enum OSD_ADJUSTER_TYPE_ENUM
{
   VT_BYTE = 0,
   VT_SBYTE = 1,
   VT_WORD = 2,
   VT_SWORD = 3,
   VT_Cyclic = (1 << 4),
   VT_FuncMin = (1 << 5),
   VT_FuncMax = (1 << 6),
   VT_Calculate = (1 << 7)
} gmd_OSD_ADJUSTER_TYPE;

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************

//******************************************************************************
//  E X T E R N A L    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

void gm_IncAdjuster(void);
void gm_IncAdjusterSM4(WORD W_Offset);
void gm_DecAdjuster(void);
void gm_DecAdjusterSM4(WORD W_Offset);
void gm_SetAdjuster(void);
SDWORD gm_GetAdjuster(WORD W_AdjOffset);
SDWORD gm_GetAdjusterMin(WORD W_AdjOffset);
SDWORD gm_GetAdjusterMax(WORD W_AdjOffset);
void gm_SaveAdjuster(void);
void gm_RetrieveAdjuster(void);

//******************************************************************************
//  O S D    S T A T E    M A C H I N E
//******************************************************************************

//******************************************************************************
//  T Y P E    D E F I N I T I O N S
//******************************************************************************
typedef enum OSD_SM_STATE_CONTROL_BYTE_ENUM
{
   SCB_CtrlByte	=	0x00,
   SCB_NOCtrlByte	=	0x80,  // translation table
   SCB_TranslTbl	=	0x08,
   SCB_StateData	=	0x10,
   SCB_IncDec		=	0x20,
   SCB_Parent		=	0x40,
   SCB_Inherit		=	0x01
} gmt_OSD_SM_STATE_CONTROL_BYTE;

typedef enum OSD_SM_STATE_ACTION_ENTRY_CONTROL_BYTE_ENUM
{
   SAT_CB_00		=	0,				//Event - Action
   SAT_CB_01		=	(BIT6),			//Event
   SAT_CB_10		=	(BIT7),			//Event - Condition Ctrl
   SAT_CB_11		=	(BIT7 | BIT6),	//MultiAction Ctrl - Condition Ctrl
   SAT_EvtAct_1	=	(BIT6),
   SAT_EvtAct_2	=	(BIT7),
   SAT_Reference	=	(BIT7 | BIT6),
   SAT_Condition	=	(BIT4),
   SAT_MultiCond	=	(BIT5 | BIT4)
} gmt_OSD_SM_STATE_ACTION_ENTRY_CONTROL_BYTE;

// WB4 -------------------------------------------
typedef enum
{
   SM_PARAM_NONE 	= 	0,
   SM_PARAM_BYTE		=	(BIT6 | 1),
   SM_PARAM_SBYTE	=	(BIT6 | 1),
   SM_PARAM_WORD	=	(BIT6 | 2),
   SM_PARAM_SWORD	=	(BIT6 | 2),
   SM_PARAM_TRIPLE	=	(BIT6 | 3),
   SM_PARAM_POINTER	=	(BIT7 | 4)
} gmt_OSD_SM_PARAM_CONTROL_BYTE;

#define SM_PARAM_DATA_BLOCK(Size)		(BIT7 | BIT6 | Size)

#define SM_PARAM_ENTRY_BYTE(Value)	(BYTE) (Value)
#define SM_PARAM_ENTRY_SBYTE(Value)	(BYTE) (Value)
#define SM_PARAM_ENTRY_WORD(Value)	(BYTE)(((WORD)(Value)) >> 8), (BYTE) (Value)
#define SM_PARAM_ENTRY_SWORD(Value)	(BYTE)(((WORD)(Value)) >> 8), (BYTE) (Value)
#define SM_PARAM_ENTRY_TRIPLE(Value)	(BYTE)(((DWORD)(Value)) >> 16), (BYTE)(((WORD)(Value)) >> 8), (BYTE) (Value)

#define	WBK_EXTRACT_CODE_BYTE(Cnst)	(BYTE)(Cnst)						// WB4
#define	WBK_EXTRACT_CODE_WORD(Cnst)	(BYTE)(Cnst), (BYTE) ((Cnst) >> 8)	// WB4
// -------------------------------------------
// State machine thread context structure
typedef struct ST_OSD_SM_THREAD_CONTEXT_STRUCT
{
   BYTE ROM * Bp_CurrentStatePtr;
   BYTE ROM * Bp_NextStatePtr;
   BYTE ROM * Bp_SaveStatePtr;
   WORD XScalePercent;
   WORD YScalePercent;
} gmt_OSD_SM_THREAD_CONTEXT;

typedef struct ST_OSD_SM_THREAD_DESCRIPTION_STRUCT
{
   BYTE ROM * ROM *Bp_StatesSet;  // pointer to thread state set
   BYTE B_DefaultStateId;
} gmt_OSD_SM_THREAD_DESCRIPTOR;

// WB4 -------------------------------------------
typedef void far (*gmt_OSD_ACTION_FUNC)(void);
typedef BYTE far (*gmt_OSD_CONDITION_FUNC)(void);

typedef struct
{
   BYTE B_DataSize;
   gmt_OSD_ACTION_FUNC Fp_ActionFunc;
} gmt_OSD_ACTION_ENTRY;

typedef struct
{
   WORD	address;
   WORD	length;
}gmt_BLOCKINFO;
//  -------------------------------------------

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************
#define OE_None 0

//Reference Coding Macros
#define ShortIDRef(a)				(a)
#define ShortPtrRef(a)				BIT7 | (((WORD)(a)) >> 8), (BYTE)(a)
#define LongIDRef(a)				BIT7 | BIT6 | (BYTE)(((WORD)(a)) >> 8), (BYTE)(a)
#define LongBankRef(bank, a)		BIT7 | BIT6 | BIT5 | bank, ((WORD)(a)) >> 8, (BYTE)(a)
#define ShortEATRef(a)	(a)
#define LongEATRef(a)	BIT7 | (((WORD)(a)) >> 8), (BYTE)(a)

#define SAT_CB_00_EVT_SHIFT		0x0003
#define SAT_CB_10_EVT_SHIFT		0x0002
#define SAT_CB_11_MAC_SHIFT		0x0002
#define SAT_CB_11_ATREF			(BIT5)
#define SAT_CB_11_ATREF_MASK	(BIT4 | BIT3 | BIT2 | BIT1 | BIT0)

#define Fit_00(Event, Action)		(((Event) << SAT_CB_00_EVT_SHIFT) | (Action))
#define Fit_10(Event, CndCtrl)		(((Event) << SAT_CB_10_EVT_SHIFT) | (CndCtrl))
#define Fit_11(CndCtrl, MultiActn)	(((MultiActn) << SAT_CB_11_MAC_SHIFT) | (CndCtrl))

// Osd Timers
#define SetTimer(TimerId, TimerCount)	(BYTE)(BIT7 | BIT6 | BIT5 | ((BYTE)(TimerId) & 0x1f)), \
											(BYTE)(((WORD)(TimerCount)) >> 8), (BYTE)(TimerCount)
#define TIMER_ID_MASK					(~(BIT7 | BIT6 | BIT5))

// TranslationTableEntry flags
#define TTEF_NoAutorepeat		0
#define TTEF_Autorepeat			1
#define TTEF_TableContinues		0
#define TTEF_TableEnds			1


#define SM_PARAM_TYPE_ADJUSTER_ID		WORD
#define SM_PARAM_TYPE_BYTE		WORD
#define SM_PARAM_TYPE_SBYTE		WORD
#define SM_PARAM_TYPE_WORD		WORD
#define SM_PARAM_TYPE_SWORD		WORD

//******************************************************************************
//  E X T E R N A L    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

void gm_OsdEventHandler(BYTE B_OsdEvent);
void gm_OsdEventHandlerSM4(WORD W_OsdEvent);
void gm_OsdEventHandlerSM5(WORD W_OsdEvent);
BYTE gm_OsdGetCurrentStateTranslTable(void);
void gm_OsdGetActionOperand(void);

BYTE gm_OsdGetActionByteParameter(void);
WORD gm_OsdGetActionWordParameter(void);
DWORD gm_OsdGetActionTripleByteParameter(void);

//these defines are only for backward compatibility
//these lines should be remove later
BYTE gm_OsdGetCurrentSATOffsetByte(void);
WORD gm_GetCurrentSATOffsetWord(void);
DWORD gm_GetCurrentSATOffsetTripleByte(void);

void gm_OsdSmInitThreadsContext(void);
void gm_SwitchOsdThread(BYTE B_ThreadId);

void gm_SetOsdSMWatchdogTimeout(WORD W_Data);

void gm_DecState(void);
void gm_DecStateBy(void);
void gm_DecStateBySM4(WORD W_Offset);
void gm_IncState(void);
void gm_IncStateBy(void);
void gm_IncStateBySM4(WORD W_Offset);
void gm_GoToState(void);
void gm_GoToStateSM4(WORD W_Offset);
void gm_SaveState(void);
void gm_RestoreState(void);
void gm_PostEvent(void);
WORD gm_GetPostedEvent(void);

//******************************************************************************
//  O S D    C O N T R O L S
//******************************************************************************

//******************************************************************************
//  T Y P E    D E F I N I T I O N S
//******************************************************************************
#ifdef __PARADIGM__
#pragma pack(push)
#pragma pack(1)
#endif //__PARADIGM__

// current independent tile positoin (doesn't depend on osd position)
typedef struct ST_OSD_TILE_POSITION_STRUCT
{
   WORD W_XPos;
   WORD W_YPos;
} gmt_OSD_TILE_POSITION;

typedef struct ST_OSD_UNI_BITMAP_STRUCT
{
   BYTE ROM * ROM	Bp_PixelData;
   BYTE ROM * ROM	Bp_ColorMap;
} gmt_OSD_UNI_BITMAP;

typedef struct ST_OSD_CLUT_STRUCT
{
   BYTE			B_StartPosition;
   BYTE ROM * ROM	Bp_Clut;
} gmt_OSD_CLUT;

//structure to control tile highlight settings
typedef struct ST_OSD_HIGHLIGHT_CTRL_STRUCT
{
   BYTE B_HgltWndInfo;	// The highlight window information to use for highlight
   BYTE B_Transparent;	// Transparency setting for TileHighlight
   BYTE B_HgltEnable;	// Determines whether to create new tile window or remove tile window
} gmt_OSD_HIGHLIGHT_CTRL;

typedef struct ST_OSD_DIALOG_CTRL_STRUCT
{
   BYTE				DialogId;
   WORD				XSize;
   WORD				YSize;
   WORD				MaxXpos;
   WORD				MaxYpos;
   WORD				HPositionRef;
   WORD				VPositionRef;
   BYTE				FragmNum;
   gmt_OSD_CLUT ROM	*ColorRef;
   WORD	XScalePercent;
   WORD	YScalePercent;
} gmt_OSD_DIALOG_CTRL;

typedef struct ST_OSD_INVISIBLESTATE_CTRL_STRUCT
{
   BYTE				FragmNum;
   gmt_OSD_CLUT ROM	*ColorRef;
} gmt_OSD_INVISIBLESTATE_CTRL;

typedef struct ST_OSD_TILE_CTRL_STRUCT
{
   BYTE	TileId;
   BYTE	DialogId;
   WORD	XPos;
   WORD	YPos;
   WORD	XSize;
   WORD	YSize;
   BYTE	Attr;
   WORD	ColorAttr;
   DWORD	Address;
   BYTE	AllocScheme;
} gmt_OSD_TILE_CTRL;

typedef struct ST_OSD_BASE_CTRL_STRUCT
{
   BYTE	TileId;
   WORD	XPos;
   WORD	YPos;
   WORD	XSize;
   WORD	YSize;
   BYTE	EraseColorIndex;
} gmt_OSD_BASE_CTRL;

typedef struct ST_OSD_BITMAP_CTRL_STRUCT
{
   BYTE	TileId;
   WORD	XPos;
   WORD	YPos;
   WORD	XSize;
   WORD	YSize;
   BYTE	EraseColorIndex;
   gmt_OSD_UNI_BITMAP	Colors;
} gmt_OSD_BITMAP_CTRL;

typedef struct ST_OSD_BITMAPBUTTON_CTRL_STRUCT
{
   BYTE	TileId;
   WORD	XPos;
   WORD	YPos;
   WORD	XSize;
   WORD	YSize;
   BYTE	EraseColorIndex;
   gmt_OSD_UNI_BITMAP	Colors;
} gmt_OSD_BITMAPBUTTON_CTRL;

//120322 Edward merge 2410
typedef struct ST_OSD_MOVEABLEBITMAP_CTRL_STRUCT
{
	BYTE	TileId;
	WORD	XPos;
	WORD	YPos;
	WORD	XSize;
	WORD	YSize;
	BYTE	EraseColorIndex;
	gmt_OSD_UNI_BITMAP	Colors;
	DWORD	HorPosition1;
	SDWORD	HorPosition2;
	SDWORD	HorPosition3;
	SDWORD	HorPosition4;
	SDWORD	HorPosition5;
	SDWORD	HorPosition6;
	SDWORD	HorPosition7;
	SDWORD	HorPosition8;	
} gmt_OSD_MOVEABLEBITMAP_CTRL;

typedef struct ST_OSD_BITMAPSLIDER_CTRL_STRUCT
{
   BYTE	TileId;
   WORD	XPos;
   WORD	YPos;
   WORD	XSize;
   WORD	YSize;
   BYTE	EraseColorIndex;
   gmt_OSD_UNI_BITMAP	Colors;
   WORD	Associate;
   BYTE	Direction;
} gmt_OSD_BITMAPSLIDER_CTRL;

typedef struct ST_OSD_BITMAPSLIDERBUTTON_CTRL_STRUCT
{
   BYTE	TileId;
   WORD	XPos;
   WORD	YPos;
   WORD	XSize;
   WORD	YSize;
   BYTE	EraseColorIndex;
   gmt_OSD_UNI_BITMAP	Colors;
   WORD	Associate;
   BYTE	Direction;
} gmt_OSD_BITMAPSLIDERBUTTON_CTRL;

typedef struct ST_OSD_LONGVALUE_CTRL_STRUCT
{
   BYTE	TileId;
   WORD	XPos;
   WORD	YPos;
   WORD	XSize;
   WORD	YSize;
   BYTE	EraseColorIndex;
   WORD	Associate;
   BYTE ROM *	UsedGlyphs;
   WORD	MinValue;
   WORD	MaxValue;
   BYTE ROM *	Colors;
   BYTE	Horizontal;
   BYTE	Vertical;
} gmt_OSD_LONGVALUE_CTRL;

typedef struct ST_OSD_LONGVALUEBUTTON_CTRL_STRUCT
{
   BYTE	TileId;
   WORD	XPos;
   WORD	YPos;
   WORD	XSize;
   WORD	YSize;
   BYTE	EraseColorIndex;
   WORD	Associate;
   BYTE ROM *	UsedGlyphs;
   WORD	MinValue;
   WORD	MaxValue;
   BYTE ROM *	Colors;
   BYTE	Horizontal;
   BYTE	Vertical;
} gmt_OSD_LONGVALUEBUTTON_CTRL;

typedef struct ST_OSD_RECTANGLE_CTRL_STRUCT
{
   BYTE	TileId;
   WORD	XPos;
   WORD	YPos;
   WORD	XSize;
   WORD	YSize;
   BYTE	EraseColorIndex;
   BYTE ROM *	Colors;
   BYTE 	BorderWidth;
} gmt_OSD_RECTANGLE_CTRL;

typedef struct ST_OSD_RECTANGLEBUTTON_CTRL_STRUCT
{
   BYTE	TileId;
   WORD	XPos;
   WORD	YPos;
   WORD	XSize;
   WORD	YSize;
   BYTE	EraseColorIndex;
   BYTE ROM *	Colors;
   BYTE 	BorderWidth;
} gmt_OSD_RECTANGLEBUTTON_CTRL;

typedef struct ST_OSD_SLIDER_CTRL_STRUCT
{
   BYTE	TileId;
   WORD	XPos;
   WORD	YPos;
   WORD	XSize;
   WORD	YSize;
   BYTE	EraseColorIndex;
   WORD	Associate;
   BYTE	Direction;
   BYTE ROM *	Colors;
   BYTE	BorderWidth;
} gmt_OSD_SLIDER_CTRL;

typedef struct ST_OSD_VERTICALSLIDER_CTRL_STRUCT
{
   BYTE	TileId;
   WORD	XPos;
   WORD	YPos;
   WORD	XSize;
   WORD	YSize;
   BYTE	EraseColorIndex;
   WORD	Associate;
   BYTE	Direction;
   BYTE ROM *	Colors;
   BYTE	BorderWidth;
} gmt_OSD_VERTICALSLIDER_CTRL;

typedef struct ST_OSD_SLIDERBUTTON_CTRL_STRUCT
{
   BYTE	TileId;
   WORD	XPos;
   WORD	YPos;
   WORD	XSize;
   WORD	YSize;
   BYTE	EraseColorIndex;
   WORD	Associate;
   BYTE	Direction;
   BYTE ROM *	Colors;
   BYTE	BorderWidth;
} gmt_OSD_SLIDERBUTTON_CTRL;

typedef struct ST_OSD_VERTICALSLIDERBUTTON_CTRL_STRUCT
{
   BYTE	TileId;
   WORD	XPos;
   WORD	YPos;
   WORD	XSize;
   WORD	YSize;
   BYTE	EraseColorIndex;
   WORD	Associate;
   BYTE	Direction;
   BYTE ROM *	Colors;
   BYTE	BorderWidth;
} gmt_OSD_VERTICALSLIDERBUTTON_CTRL;

typedef struct ST_OSD_TEXT_CTRL_STRUCT
{
   BYTE	TileId;
   WORD	XPos;
   WORD	YPos;
   WORD	XSize;
   WORD	YSize;
   BYTE	EraseColorIndex;
   BYTE ROM *	Text;
   BYTE ROM *	Colors;
} gmt_OSD_TEXT_CTRL;

typedef struct gmt_OSD_VERTICALTEXT_CTRLStruct
{
   BYTE	TileId;
   WORD	XPos;
   WORD	YPos;
   WORD	XSize;
   WORD	YSize;
   BYTE	EraseColorIndex;
   BYTE ROM *	Text;
   BYTE ROM *	Colors;
} gmt_OSD_VERTICALTEXT_CTRL;


typedef struct ST_OSD_TEXTBUTTON_CTRL_STRUCT
{
   BYTE	TileId;
   WORD	XPos;
   WORD	YPos;
   WORD	XSize;
   WORD	YSize;
   BYTE	EraseColorIndex;
   BYTE ROM *	Text;
   BYTE ROM *	Colors;
} gmt_OSD_TEXTBUTTON_CTRL;

typedef struct gmt_OSD_VERTICALTEXTBUTTON_CTRLStruct
{
   BYTE	TileId;
   WORD	XPos;
   WORD	YPos;
   WORD	XSize;
   WORD	YSize;
   BYTE	EraseColorIndex;
   BYTE ROM *	Text;
   BYTE ROM *	Colors;
} gmt_OSD_VERTICALTEXTBUTTON_CTRL;

typedef struct gmt_OSD_DSTATICTEXTBUTTONEXT_CTRLStruct
{
   BYTE	TileId;
   WORD	XPos;
   WORD	YPos;
   WORD	XSize;
   WORD	YSize;
   BYTE	EraseColorIndex;
   BYTE ROM *	Text;
   BYTE ROM *	Colors;
   BYTE ROM *	Text_2;
   BYTE ROM *	Text_3;
   BYTE ROM *	Text_4;
   BYTE ROM *	Text_5;
   BYTE ROM *	Text_6;
   BYTE ROM *	Text_7;
   BYTE ROM *	Text_8;
   BYTE ROM *	Text_9;
   BYTE ROM *	Text_10;
   BYTE ROM *	Text_11;
   BYTE ROM *	Text_12;
   BYTE ROM *	Text_13;
   BYTE ROM *	Text_14;
   BYTE ROM *	Text_15;
   BYTE ROM *	Text_16;
   BYTE ROM *	Text_17;
   BYTE ROM *	Text_18;
   BYTE ROM *	Text_19;
   BYTE ROM *	Text_20;
   BYTE ROM *	Text_21;
   BYTE ROM *	Text_22;
   BYTE ROM *	Text_23;
   BYTE ROM *	Text_24;
   BYTE ROM *	Text_25;
   BYTE ROM *	Text_26;
   BYTE ROM *	Text_27;
   BYTE ROM *	Text_28;
   BYTE ROM *	Text_29;
   BYTE ROM *	Text_30;
   BYTE ROM *	Text_31;
   BYTE ROM *	Text_32;
} gmt_OSD_DSTATICTEXTBUTTONEXT_CTRL;

typedef struct gmt_OSD_DSTATICTEXTBUTTON_CTRLStruct
{
   BYTE	TileId;
   WORD	XPos;
   WORD	YPos;
   WORD	XSize;
   WORD	YSize;
   BYTE	EraseColorIndex;
   BYTE ROM *	Text;
   BYTE ROM *	Colors;
   BYTE ROM *	Text_2;
   BYTE ROM *	Text_3;
   BYTE ROM *	Text_4;
   BYTE ROM *	Text_5;
   BYTE ROM *	Text_6;
   BYTE ROM *	Text_7;
   BYTE ROM *	Text_8;
} gmt_OSD_DSTATICTEXTBUTTON_CTRL;

typedef struct gmt_OSD_DSTATICTEXT_CTRLStruct
{
   BYTE	TileId;
   WORD	XPos;
   WORD	YPos;
   WORD	XSize;
   WORD	YSize;
   BYTE	EraseColorIndex;
   BYTE ROM *	Text;
   BYTE ROM *	Colors;
   BYTE ROM *	Text_2;
   BYTE ROM *	Text_3;
   BYTE ROM *	Text_4;
   BYTE ROM *	Text_5;
   BYTE ROM *	Text_6;
   BYTE ROM *	Text_7;
   BYTE ROM *	Text_8;
} gmt_OSD_DSTATICTEXT_CTRL;

typedef struct gmt_OSD_DSTATICTEXTEXT_CTRLStruct
{
   BYTE	TileId;
   WORD	XPos;
   WORD	YPos;
   WORD	XSize;
   WORD	YSize;
   BYTE	EraseColorIndex;
   BYTE ROM *	Text;
   BYTE ROM *	Colors;
   BYTE ROM *	Text_2;
   BYTE ROM *	Text_3;
   BYTE ROM *	Text_4;
   BYTE ROM *	Text_5;
   BYTE ROM *	Text_6;
   BYTE ROM *	Text_7;
   BYTE ROM *	Text_8;
   BYTE ROM *	Text_9;
   BYTE ROM *	Text_10;
   BYTE ROM *	Text_11;
   BYTE ROM *	Text_12;
   BYTE ROM *	Text_13;
   BYTE ROM *	Text_14;
   BYTE ROM *	Text_15;
   BYTE ROM *	Text_16;
   BYTE ROM *	Text_17;
   BYTE ROM *	Text_18;
   BYTE ROM *	Text_19;
   BYTE ROM *	Text_20;
   BYTE ROM *	Text_21;
   BYTE ROM *	Text_22;
   BYTE ROM *	Text_23;
   BYTE ROM *	Text_24;
   BYTE ROM *	Text_25;
   BYTE ROM *	Text_26;
   BYTE ROM *	Text_27;
   BYTE ROM *	Text_28;
   BYTE ROM *	Text_29;
   BYTE ROM *	Text_30;
   BYTE ROM *	Text_31;
   BYTE ROM *	Text_32;
}	gmt_OSD_DSTATICTEXTEXT_CTRL;

typedef struct ST_OSD_MOVABLELONGVALUE_CTRL_STRUCT
{
   BYTE	TileId;
   WORD	XPos;
   WORD	YPos;
   WORD	XSize;
   WORD	YSize;
   BYTE	EraseColorIndex;
   WORD	Associate;
   BYTE ROM *	UsedGlyphs;
   WORD	MinValue;
   WORD	MaxValue;
   BYTE ROM *	Colors;
   BYTE	Horizontal;
   BYTE	Vertical;
   WORD XOffset;
   WORD YOffset;
} gmt_OSD_MOVABLELONGVALUE_CTRL;

typedef struct ST_OSD_DYNAMICTEXT_CTRL_STRUCT
{
   BYTE	TileId;
   WORD	XPos;
   WORD	YPos;
   WORD	XSize;
   WORD	YSize;
   BYTE	EraseColorIndex;
   BYTE ROM *	Colors;
   BYTE	Horizontal;
   BYTE	Vertical;
   BYTE   MaxString;
   BYTE ROM *	UsedGlyphs;
   WORD PtrSegment;
   WORD PtrOffset;
} gmt_OSD_DYNAMICTEXT_CTRL;

typedef struct ST_OSD_TIMINGINFO_CTRL_STRUCT
{
   BYTE	TileId;
   WORD	XPos;
   WORD	YPos;
   WORD	XSize;
   WORD	YSize;
   BYTE	EraseColorIndex;
   BYTE ROM *	UsedGlyphs;
   BYTE ROM *	Colors;
   BYTE	Channel;
   BYTE	Horizontal;
   BYTE	Vertical;
} gmt_OSD_TIMINGINFO_CTRL;

typedef struct gmt_OSD_TTX_FONTStruct
{
   BYTE ROM * ROM * Bp_FontPixelData;
   BYTE	B_CharWidth;
   BYTE	B_CharHeight;
   BYTE	B_CharSize;
} gmt_OSD_TTX_FONT;

typedef struct gmt_OSD_TTXTILE_CTRLStruct
{
   BYTE	TileId;
   BYTE	DialogId;
   WORD	XPos;
   WORD	YPos;
   WORD	XSize;
   WORD	YSize;
   BYTE	Attr;
   WORD	ColorAttr;
   DWORD	Address;
   BYTE	ColorNum;
   gmt_OSD_TTX_FONT	TtxFont;
   BYTE	AllocScheme;
} gmt_OSD_TTXTILE_CTRL;

typedef struct gmt_OSD_CCTILE_CTRLStruct
{
   BYTE	TileId;
   BYTE	DialogId;
   WORD	XPos;
   WORD	YPos;
   WORD	XSize;
   WORD	YSize;
   BYTE	Attr;
   WORD	ColorAttr;
   DWORD	Address;
   BYTE	ColorNum;
   gmt_OSD_TTX_FONT	CcFont;
   BYTE	AllocScheme;
} gmt_OSD_CCTILE_CTRL;

typedef struct gmt_OSD_Adjuster_InfoStruct
{
   BYTE AdjStructId;
   BYTE AdjOffset;
   BYTE *CurrentAdjValue;
   BYTE CurrentAdjSize;
} gmt_OSD_Adjuster_Info;

typedef struct gmt_OSD_UNI_FONTStruct
{
   BYTE NumberFonts;
   BYTE const far * const far *Wp_Fonts;	//pointer to an array containing pointers to font tables
   WORD const far * const far *Wp_Maps;	//pointer to an array containing pointers to map files
   WORD const far *Wp_Control;				//pointer to an array containing control information for each font
   BYTE W_CharWidth;	//character width in pixels
   BYTE W_CharHeight;	//character height in lines
   WORD W_CharSize;	//character size in bytes
} gmt_OSD_UNI_FONT;

typedef struct gmt_OSD_UNICODETEXT_CTRLStruct
{
   BYTE	TileId;
   WORD	XPos;
   WORD	YPos;
   WORD	XSize;
   WORD	YSize;
   BYTE	EraseColorIndex;
   BYTE ROM *	Colors;
   gmt_OSD_UNI_FONT	UnicodeFont;
} gmt_OSD_UNICODETEXT_CTRL;


typedef struct gmt_OSD_UNICODERANGETEXT_CTRLStruct
{
   BYTE	TileId;
   WORD	XPos;
   WORD	YPos;
   WORD	XSize;
   WORD	YSize;
   BYTE	EraseColorIndex;
   BYTE ROM *	UnicodeRange;
   BYTE ROM *	Colors;
   BYTE	Horizontal;
   BYTE	Vertical;
   WORD	PtrSegment;
   WORD	PtrOffset;
} gmt_OSD_UNICODERANGETEXT_CTRL;


#ifdef __PARADIGM__
#pragma pack(pop)
#endif //__PARADIGM__

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************
//unknown dialog position inside of tile (this value is set if tile isn't allocated or
//tile structure doesn't have information about its dialog
#define UNKNOWN_TILE_DIALOG_POSITION		0xFFFF

//used for OSD rotation
enum WB_Rotation
{
   OSD_NON_ROTATED	= 0,
   OSD_ROTATED_90	= 1,
   OSD_ROTATED_180	= 2,
   OSD_ROTATED_270	= 3
};

//used for text controls alignment
enum WB_Alignment
{
   ALIGN_Left_Top,
   ALIGN_Center,
   ALIGN_Right_Bottom
};

// 20120418, used for 4kx2k OSD port disolay
enum WB_OSDPort_4kx2k
{
   OSD_4kx2k_HDMI	= 0,
   OSD_4kx2k_HDMI2	= 1,
   OSD_4kx2k_HDMI3	= 2,
   OSD_4kx2k_DP1	= 3,
   OSD_4kx2k_DP2	= 4
};


//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************

//******************************************************************************
//  E X T E R N A L    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
void gm_OsdLoadClut(gmt_OSD_CLUT ROM *Sp_Clut);
void gm_OsdLoadClut2(WORD W_StartIndex, WORD W_Size, WORD ROM *Wp_ColorData);

void gm_SetOsdLanguage(BYTE B_Lang);
void gm_SetOsdRotation(void);
void gm_OsdHFlipControl(BOOL Bt_State);
void gm_OsdVFlipControl(BOOL Bt_State);
void gm_SetOsdRotationSM4(BYTE B_Rotation);

void LoadColorFragments(BYTE ROM *Bp_ColorFragment);

void gm_OsdDrawBitmapStripe(gmt_OSD_BASE_CTRL* Stp_CtrlParams, WORD W_ColourDepth,
                            BYTE ROM *Bp_Data, BYTE ROM *Bp_ColorMap);

BYTE ROM * ParseColorMapping(BYTE B_ColorState, BYTE ROM *Bp_Colors);

WORD far DecompressWrapper(BYTE *Bp_Src, WORD W_Size);

BYTE NumberToGlyphs(const DWORD DW_value, BYTE *Bp_buf, BYTE far *Bp_glyph_indices);

WORD GetTextWidth(BYTE B_FontId, WORD W_NumberOfGlyphs, BYTE ROM *Bp_Text);
WORD GetTextHeight(BYTE B_FontId);
WORD CalculateXAlignmentOffset(BYTE B_NumberOfGlyphs, BYTE B_Horizontal,
                               BYTE ROM *Bp_Text, BYTE B_FontId, WORD W_XSize);
WORD CalculateYAlignmentOffset(BYTE B_Vertical, BYTE B_FontId, WORD W_YSize);

void ReverseTileSet(void);

void EraseControl(void);
void FreeHighlightWindow(void);

void GetAdjusterStruct(WORD * W_AdjId, gmt_OSD_Adjuster_Info * AdjusterInfo);

void GetUnicodeStringInfo(WORD const far **Wp_String, WORD *W_StringLength, BYTE *B_InterGlyphSpace);

//******************************************************************************
// I N V I S I B L E   S T A T E    C O N T R O L    F U N C T I O N S
//******************************************************************************
void LoadColorsInvisibleState(void);

//******************************************************************************
// D I A L O G    C O N T R O L     F U N C T I O N S
//******************************************************************************
void LoadColorsDialog(void);
void SetActiveDialog(void);
void ScaleXParam(WORD *XParam);
void ScaleYParam(WORD *YParam);

//******************************************************************************
// T I L E    C O N T R O L     F U N C T I O N S
//******************************************************************************
void CreateTtxTile(void);
void DestroyTtxTile(void);
void HideTtxTile(void);
void ShowTtxTile(void);
void CreateCcTile(void);
void DestroyCcTile(void);
void HideCcTile(void);
void ShowCcTile(void);
void AllocateTile(void);
void DeallocateTile(void);
void EnableTile(void);
void DisableTile(void);
void EnableTransparencyTile(void);
void DisableTransparencyTile(void);
void EnableBlendingTile(void);
void DisableBlendingTile(void);
void EnableBlinkingTile(void);
void DisableBlinkingTile(void);

//******************************************************************************
// B I T M A P    C O N T R O L     F U N C T I O N S
//******************************************************************************
void DrawBitmap(void);
void DrawSelectedBitmap(void);
void DrawDisabledBitmap(void);
void DrawBitmapControl(BYTE B_ColorState);
void EraseBitmap(void);

//******************************************************************************
// B I T M A P    S L I D E R    C O N T R O L     F U N C T I O N S
//******************************************************************************
void DrawBitmapSlider(void);
void DrawSelectedBitmapSlider(void);
void DrawDisabledBitmapSlider(void);
void DrawBitmapSliderControl(BYTE B_ColorState, BYTE B_DrawFlag);
void UpdateBitmapSlider(void);
void UpdateDisabledBitmapSlider(void);
void UpdateSelectedBitmapSlider(void);

//******************************************************************************
// F R A M E    C O N T R O L     F U N C T I O N S
//******************************************************************************
void DrawRectangle(void);
void DrawSelectedRectangle(void);
void DrawDisabledRectangle(void);
void DrawRectangleControl(BYTE B_ColorState);

//******************************************************************************
// L O N G V A L U E    C O N T R O L     F U N C T I O N S
//******************************************************************************
void DrawLongValue(void);
void DrawSelectedLongValue(void);
void DrawDisabledLongValue(void);
void DrawLongValueControl(BYTE B_ColorState, BYTE B_DrawFlag);
void UpdateLongValue(void);
void UpdateSelectedLongValue(void);
void UpdateDisabledLongValue(void);

//******************************************************************************
// S L I D E R    C O N T R O L     F U N C T I O N S
//******************************************************************************
void DrawSlider(void);
void DrawSelectedSlider(void);
void DrawDisabledSlider(void);
void DrawSliderControl(WORD W_ColorState, BYTE Flag);
void UpdateSlider(void);
void UpdateSelectedSlider(void);
void UpdateDisabledSlider(void);
void UpdateSliderControl(WORD W_ColorState, BYTE Flag);
void DrawRotatedSlider(gmt_OSD_SLIDER_CTRL ROM *Sp_SliderCtrl,
                       BYTE ROM *Bp_ColorMap, WORD W_SliderValue,
                       WORD W_DrawFlag);

//******************************************************************************
// V E R T I C A L    S L I D E R    C O N T R O L     F U N C T I O N S
//******************************************************************************
void DrawVSlider(void);
void DrawSelectedVSlider(void);
void DrawDisabledVSlider(void);
void UpdateVSlider(void);
void UpdateSelectedVSlider(void);
void UpdateDisabledVSlider(void);

//******************************************************************************
// T E X T    C O N T R O L     F U N C T I O N S
//******************************************************************************
void DrawText(void);
void DrawSelectedText(void);
void DrawDisabledText(void);
void DrawTextControl(WORD W_ColorState, BYTE Flag);

//******************************************************************************
// V E R T I C A L    T E X T    C O N T R O L     F U N C T I O N S
//******************************************************************************
void DrawVText(void);
void DrawSelectedVText(void);
void DrawDisabledVText(void);
void DrawRotatedVText(BYTE ROM *Bp_CtrlPtr, BYTE ROM *Bp_ColorMap,
                      WORD W_XOffset, WORD W_YOffset, BYTE B_FontId,
                      BYTE ROM *Bp_Text, BYTE B_NumOfGlyphs);

//******************************************************************************
// U N I C O D E  T E X T    C O N T R O L     F U N C T I O N S
//******************************************************************************
void DrawUnicodeText(void);

//******************************************************************************
// U N I C O D E  R A N G E T E X T    C O N T R O L     F U N C T I O N S
//******************************************************************************
void DrawUnicodeRangeText(void);

//******************************************************************************
// T I M I N G I N F O    C O N T R O L     F U N C T I O N S
//******************************************************************************
void DrawTimingInfoBase(WORD W_InfoToDraw);
void DrawTimingInfo(void);
void DrawResolutionTimingInfo(void);
void DrawVFreqTimingInfo(void);
void DrawHFreqTimingInfo(void);

#endif //#ifndef __OSDLIB_H__
//*********************************  END  **************************************
