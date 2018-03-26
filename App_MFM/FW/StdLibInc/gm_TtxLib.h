/*
	$Workfile:   gm_TtxLib.h  $
	$Revision: 1.3 $
	$Date: 2011/12/14 10:35:32 $
*/

#ifndef __GM_TTXLIB_H__
	#define __GM_TTXLIB_H__
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
// MODULE:      gm_TtxLib.h
//
// USAGE:		Header file for teletext driver support
//
// Note:    This is a temporary header file for user interface to the 
//          TTX decoder
//******************************************************************************

//******************************************************************************
//  T Y P E    D E F I N I T I O N S                                      
//******************************************************************************
#ifdef __PARADIGM__
#pragma pack(push)
#pragma pack(1)
#endif //__PARADIGM__

typedef struct ST_OSD_TTXCONTROL_CTRL_STRUCT
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
} gmt_OSD_TTXCONTROL_CTRL;

#ifdef __PARADIGM__
#pragma pack(pop)
#endif //__PARADIGM__

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************
typedef enum
{
	TTX_KEY_ON,
	TTX_KEY_OFF,
	TTX_KEY_OK,
	TTX_KEY_LEFT,
	TTX_KEY_RIGHT,
	TTX_KEY_UP,
	TTX_KEY_DOWN,
	TTX_KEY_RED,
	TTX_KEY_GREEN,
	TTX_KEY_YELLOW,
	TTX_KEY_CYAN,
	TTX_KEY_ONE,
	TTX_KEY_TWO,
	TTX_KEY_THREE,
	TTX_KEY_FOUR,
	TTX_KEY_FIVE,
	TTX_KEY_SIX,
	TTX_KEY_SEVEN,
	TTX_KEY_EIGHT,
	TTX_KEY_NINE,
	TTX_KEY_ZERO,
	TTX_KEY_INDEX_PAGE,
	TTX_KEY_SUBTITLE,
	TTX_KEY_PREVIOUS_PAGE,
	TTX_KEY_SUBPAGE_BAR_ON,
	TTX_KEY_SUBPAGE_BAR_OFF,
	TTX_KEY_CLOCK,
	TTX_KEY_ZOOM_MODE,
	TTX_KEY_ZOOM_NORMAL_MODE,
	TTX_KEY_ZOOM_UPPER_MODE,
	TTX_KEY_ZOOM_LOWER_MODE,	
	TTX_KEY_MIX_MODE,
	TTX_KEY_REVEAL,
	TTX_KEY_HOLD,
	TTX_KEY_CANCEL_MODE,
	TTX_KEY_STATUS_DISPLAY,
	TTX_KEY_ACI_ON,
	TTX_KEY_ACI_OFF,
	TTX_KEY_ON_GO_TO_SUBPAGE_MODE
} gmt_TtxKey;

// ttx decoder language groups
typedef enum
{
	TTX_LANGUAGE_GROUP_WESTERN_EUROPE	= 0,
	TTX_LANGUAGE_GROUP_EASTERN_EUROPE	= 1,
	TTX_LANGUAGE_GROUP_RUSSIAN			= 2,
	TTX_LANGUAGE_GROUP_ARABIC			= 3,
	TTX_LANGUAGE_GROUP_FARSI			= 4
} gmt_TtxLanguageGroup;

typedef enum
{
	TTX_ACI_LANGUAGE_ENGLISH		= 0,
	TTX_ACI_LANGUAGE_FRENCH			= 1,
	TTX_ACI_LANGUAGE_GERMAN			= 2,
	TTX_ACI_LANGUAGE_ITALIAN			= 3,
	TTX_ACI_LANGUAGE_SPANISH		= 4,
	TTX_ACI_LANGUAGE_DUTCH			= 5,
	TTX_ACI_LANGUAGE_DANISH			= 6,
	TTX_ACI_LANGUAGE_NORWEGIAN		= 7,
	TTX_ACI_LANGUAGE_SWEDISH		= 8,
	TTX_ACI_LANGUAGE_SUOMI			= 9,
	TTX_ACI_LANGUAGE_POLISH			= 10,
	TTX_ACI_LANGUAGE_CZECH			= 11,
	TTX_ACI_LANGUAGE_HUNGARIAN		= 12,
	TTX_ACI_LANGUAGE_ROMANIAN		= 13,
	TTX_ACI_LANGUAGE_BULGARIAN		= 14,
	TTX_ACI_LANGUAGE_RUSHIAN		= 15,
	TTX_ACI_LANGUAGE_GREEK			= 16,
	TTX_ACI_LANGUAGE_PORTUGUESE	= 17,
	TTX_ACI_LANGUAGE_TURKISH		= 18,
	TTX_ACI_LANGUAGE_DEFAULT		= 19
	
} gmt_TtxAciLanguage;

typedef enum 
{
	PACKET_DATA_VPS,
	PACKET_DATA_SUBTITLE_C6,
	PACKET_DATA_8_30_1,
	PACKET_DATA_8_30_2		
} gmt_PACKET_DATA_TYPE;

typedef enum
{
	TTX_INTERLACED_DISPLAY_MODE		= 0,
	TTX_PROGRESSIVE_DISPLAY_MODE	= 1
} gmt_TtxDisplayMode;

typedef enum
{
	TTX_RESET_DECODER_MODE		= 0,
	TTX_NO_RESET_DECODER_MODE	= 1
} gmt_TtxOnOffResetDecoderMode;

typedef enum
{
	TTX_NAVIGATION_NONE		= 0x0000,
	TTX_NAVIGATION_TOP		= 0x0001,
	TTX_NAVIGATION_FLOF		= 0x0002,
	TTX_NAVIGATION_TOP_FLOF	= 0x0003,
	TTX_NAVIGATION_FLOF_TOP	= 0x0007
} gmt_TtxNavigationMode;

typedef enum
{
	TTX_FONT_G0_LATIN,
	TTX_FONT_G1_BLOCK_MOSAIC,
	TTX_FONT_G1_BLOCK_MOSAIC_SEPARATED,
	TTX_FONT_G2_LATIN,
	TTX_FONT_G3_SMOOTH_MOSAIC,
	TTX_FONT_INTERNATIONAL1,
	TTX_FONT_INTERNATIONAL2,
	TTX_FONT_INTERNATIONAL3,
	TTX_FONT_SYMBOL,
	TTX_FONT_G0_ARABIC,
	TTX_FONT_G0_CYRILLIC1,
	TTX_FONT_G0_CYRILLIC2,
	TTX_FONT_G0_CYRILLIC3,
	TTX_FONT_G0_GREEK,
	TTX_FONT_G0_HEBREW,
	TTX_FONT_G2_ARABIC,
	TTX_FONT_G2_CYRILLIC,
	TTX_FONT_G2_GREEK,
	TTX_FONT_G3_SMOOTH_MOSAIC_SEPARATED
} gmt_TtxFont;

typedef enum
{
   MODE_PDC_ONLY,          // Always use PDC from TTX, VPS is off
   MODE_VPS_PDC,           // Check VPS, then PDC
   MODE_VPS_ONLY,          // Always enable for VPS, TTX is off
   MODE_VPS2_PDC,          // Check VPS, if not found, try 2nd windows, then PDC
} gmt_VPS_MODE;

typedef enum
{
   TTX_STATUS_DISPLAY_830_NONE,                 // Do not process packet 8/30 for status display
   TTX_STATUS_DISPLAY_830_FORMAT_1,         // Process packet 8/30 Format 1 only for status display
   TTX_STATUS_DISPLAY_830_FORMAT_2,         // Process packet 8/30 Format 2 only for status display
   TTX_STATUS_DISPLAY_ALL                             // Process both Format 1 and 2 for status display
} gmt_TtxPacket830ProcType;

typedef struct 
{
  BYTE    Mix;
  BYTE    Cancel;
  BYTE    Reveal;
  BYTE    Hold;
  BYTE    User;
  BYTE    Display;
  BYTE    Clock;
  BYTE    StatusDisplay;
  BYTE    List;
  BYTE    Size;
  BYTE    PageCatch;
  BYTE    TopMenu;
  BYTE    Subtitle;
  BYTE    SubtitlePage;
  BYTE    NewsflashPage;
  BYTE    C8UpdateAvailable;
  BYTE    PageFound;
  BYTE    PageProcessed;
  BYTE    PageAvailable;
  BYTE    PageUpdateAvailable;
  BYTE    RequestedPageAvailable;
  BYTE    AciFound;
  BYTE    SubpageAvailable;
  BYTE    SubpageBarVisible;
  BYTE    TopFlofEnable;
  BYTE    NumCount;
  WORD  NumKey;
  BYTE    ListModeSelection;
  BYTE    SubpageInput;
  BYTE    SubpageInputNumCount;
  WORD  SubpageInputNumKey;
} gmt_TtxDecStatus;

typedef enum
{
  TTX_DECODER_LEVEL_15            = 0x0000,
  TTX_DECODER_LEVEL_25            = 0x0001,
  TTX_DECODER_LEVEL_25_SIDEPANELS = 0x0002,
} gmt_TtxDecoderLevel;

//******************************************************************************
//  M A C R O   D E F I N I T I O N
//****************************************************************************** 
#define TTX_DISP_NULL_CHAR	0xFF

#define TT_CLOCK_FORMAT_HHMMSS	1
#define TT_CLOCK_FORMAT_HHMM	2

#define TT_COLOR_BLACK		0x00
#define TT_COLOR_RED		0x01
#define TT_COLOR_GREEN		0x02
#define TT_COLOR_YELLOW		0x03
#define TT_COLOR_BLUE		0x04
#define TT_COLOR_MAGENTA	0x05
#define TT_COLOR_CYAN		0x06
#define TT_COLOR_WHITE		0x07

#define TTX_SUBPAGE_INPUT_HEADER   TRUE
#define TTX_SUBPAGE_INPUT_NAVBAR   FALSE

// ttx decoder level
#define TELETEXT_LEVEL_15 15
#define TELETEXT_LEVEL_25 25

//******************************************************************************
//  E X T E R N A L    V A R I A B L E S                                    
//******************************************************************************
extern BYTE gmv_TtxDisplayMode;

extern BYTE gmv_TtxOnOffResetDecoderMode;

//******************************************************************************
//  G L O B A L    F U N C T I O N    P R O T O T Y P E S                   
//******************************************************************************

//******************************************************************************
//  E X T E R N A L    F U N C T I O N    P R O T O T Y P E S                   
//******************************************************************************
gmt_RET_STAT gm_TtxGetDecStatus(gmt_TtxDecStatus* DecStatus);
void TtxUserEventHandler(void);
void gm_TtxUserEventHandler(gmt_TtxKey B_ttxKey);

BOOL gm_TtxHamming84CorrectionCtrl(void);
void gm_TtxSetDrawProp(BYTE ROM * ROM *Bp_FontTablePtrPar,
						WORD W_TtxTileIdPar,
						WORD W_TtxCharacterWidthPar,
						WORD W_TtxCharacterHeightPar,
						WORD W_TtxCharacterSizePar,
						WORD W_TtxXposTileOffsetPar,
						WORD W_TtxYposTileOffsetPar,
						BYTE ROM *Bp_TtxColourMapPtrPar,
						WORD W_TtxColourStartIndex);
void gm_TtxSetRow25ScaleCtrl(BYTE B_Ctrl);
gmt_RET_STAT gm_TtxSetLanguageGroup(gmt_TtxLanguageGroup LanguageGroup);
gmt_RET_STAT gm_TtxToggleListMode(void);
gmt_RET_STAT gm_TtxSetListModeData(WORD W_RedPage, WORD W_GreenPage,
							WORD W_YellowPage, WORD W_BluePage,
							WORD W_CustomPage1, WORD W_CustomPage2);
gmt_RET_STAT gm_TtxGetListModeData(WORD *Wp_RedPage, WORD *Wp_GreenPage,
							WORD *Wp_YellowPage, WORD *Wp_BluePage,
							WORD *Wp_CustomPage1, WORD *Wp_CustomPage2);
gmt_RET_STAT gm_TtxFlashListModeBar(BOOL bFlash);
BOOL gm_TtxGetDispPageNumber(WORD *Wp_PageNum);
void gm_TtxGetRecentHeaderPacket(BYTE *Bp_PacketBuffer);
BOOL gm_TtxIsAnyData(void);
gmt_RET_STAT gm_TtxGetHeaderTime(BYTE *Bp_Hour, BYTE *Bp_Min, BYTE *Bp_Sec );
BOOL gm_TtxGetPacketData(BYTE *Bp_Buf, gmt_PACKET_DATA_TYPE DataType, BYTE B_MaxLen);
gmt_RET_STAT gm_TtxToggleSubpageMode(void);
gmt_RET_STAT gm_TtxTopFlofCtrl(gmt_TtxNavigationMode B_Ctrl);
gmt_RET_STAT gm_TtxListModeCtrl(BYTE B_Ctrl);
gmt_RET_STAT gm_TtxSubpageModeCtrl(BYTE B_Ctrl);
BOOL gm_TtxToggleVerticalSubpageBar(void);
gmt_RET_STAT gm_TtxSubpageBarVisible(BOOL B_Ctrl);

//RET_STAT gm_VpsSetMode(gmt_VPS_MODE B_VpsMode);
void gm_VpsAcquistionSetWindow(WORD W_VpsAcqWindow);

void gm_TtxSetTileOffset(WORD W_LeftOffset, WORD W_RightOffset,
						WORD W_TopOffset, WORD W_ButtomOffset);
gmt_RET_STAT gm_TtxSetDecoderLevel(gmt_TtxDecoderLevel W_DecoderLevel);
gmt_RET_STAT gm_TtxSetAciLanguage(gmt_TtxAciLanguage AciLanguage);

void gm_TtxSetDrawCallback(void (*DrawCallBack)(BOOL));
BYTE gm_TtxGetNumOfSubtitlePages(void);

#ifdef _TT_MAIN_PROCESS_H_
TT_BOOL TT_MainProcessOpen(char *);
void TT_MainProcessClose(void);
TT_BOOL TT_MainProcessCreate(char *);
TT_BOOL TT_MainProcessDestroy(void);
TT_MainProcess_Event TT_MainProcess(void);
#else
#define TT_Main_Process_Open    TT_MainProcessOpen
#define TT_Main_Process_Close   TT_MainProcessClose
#define TT_Main_Process_Create  TT_MainProcessCreate
#define TT_Main_Process_Destroy TT_MainProcessDestroy
#define TT_Main_Process         TT_MainProcess
#define TT_CMDHandler           TT_CmdHandler

BYTE TT_MainProcessOpen(char *);
void TT_MainProcessClose(void);
BYTE TT_MainProcessCreate(char *);
BYTE TT_MainProcessDestroy(void);
WORD TT_MainProcess(void);
SWORD TT_CmdHandler(unsigned char *aCommandBuffer);
#endif

#endif //#ifndef __GM_TTXLIB_H__
//*********************************  END  ************************************** 
