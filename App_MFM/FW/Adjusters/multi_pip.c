/*
    $Workfile:   multi_pip.c  $
    $Revision:   1.61  $
    $Date:   Aug 01 2008 12:00:10  $
*/

//******************************************************************************
//
//      Copyright (C) 2004.  GENESIS MICROCHIP INC. CONFIDENTIAL
//      All rights reserved.  No part of this program may be reproduced.
//
//     Genesis Microchip Inc., 165 Commerce Valley Dr. West
//     Thornhill, Ontario, Canada, L3T 7V8
//
//==============================================================================
//
// MODULE:      multi_pip.c
//
// USAGE:       This module contains application level code for multi pip
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "System\all.h"

#if (FEATURE_MultiPxp == ENABLE)

//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************

enum 
{
	Unlocked,
	Locked,
};




//******************************************************************************
//  G L O B A L   V A R I A B L E S
//******************************************************************************
static MultiPipStyleInfo ROM MultiPipStyle_WXGA_1280x768[] =
{
	/*
	MPIP Style 1 (POP 7)
	*/
	{
		4,					//Number of rows the panel is divided for multi pip
		4,					//Number of columns the panel is divided for multi pip
		7,					//Number of pip windows that are enabled
		MPIP_MASK_POP7,		//1000_1000_1000_1111
		320,				//The width of each pip window
		192,				//The height of each pip window
		0,					//The horizontal start position of the multi pip configuration
		0,					//The vertical start position of the multi pip configuration
		4,					//The width of the border around each pip window
		4,					//The height of the border around each pip window
		RGB_565(LIGHTBLUE), //The color of the border around each window (RGB565 format)
		4,					//The width of the border for the highlight window
		4,					//The height of the border for the highlight window
		RGB_565(TEAL),		//The color of the border for the highlight window (RGB565 format)
		ORIGIN_INSIDE_BORDER,
		960,			   	//The width of the main window
		576,			   	//The height of the main window
		0,					//The horizontal start position of the main window in pixels
		192, 				//The vertical start position of the main window in pixels
		{	1,2,3,4,       	//Scan order of Pip windows
			0,0,0,5,
			0,0,0,6,
			0,0,0,7
		},
		TOP_RIGHT,			//The Position of MPip Channel Number (TOP_LEFT, BOTTOM_LEFT, TOP_RIGHT, BOTTOM_RIGHT)
		25,					//XOffset Pixel padding of channel number from respective edge
		10,					//YOffset Pixel padding of channel number from respective edge
		NON_MOVING_HIGHLIGHT,	//If Highlight window follows the active scanning window
		80,					//Timeout using 10ms resolution timer value of each scan window
		USE_ANCHOR,			//If Scanning will stop at the anchor after one scan cycle
		300					//Timeout using 10ms resolution timer value during navigation
	},
	/*
	MPIP Style 2 (POP 12)
	*/
	{
		4,					//Number of rows the panel is divided for multi pip
		4,					//Number of columns the panel is divided for multi pip
		12,					//Number of pip windows that are enabled
		MPIP_MASK_POP12,	//1111_1001_1001_1111
		320,					//The width of each pip window
		192,					//The height of each pip window
		0,					//The horizontal start position of the multi pip configuration
		0,					//The vertical start position of the multi pip configuration
		2,					//The width of the border around each pip window
		2,					//The height of the border around each pip window
		RGB_565(DARKGREY), 	//The color of the border around each window (RGB565 format)
		2,					//The width of the border for the highlight window
		2,					//The height of the border for the highlight window
		RGB_565(WHITE),		//The color of the border for the highlight window (RGB565 format)
		ORIGIN_ON_BORDER,
		640,					//The width of the main window
		384,					//The height of the main window
		320,					//The horizontal start position of the main window in pixels
		192,					//The vertical start position of the main window in pixels
		{	1,2,3,4,       		//Scan order of Pip windows
			12,0,0,5,
			11,0,0,6,
			10,9,8,7
		},
		TOP_LEFT,
		25,
		10,
		MOVING_HIGHLIGHT,
		200,
		USE_ANCHOR,
		600
	},
	/*
	MPIP Style 3 (POP 3)
	*/
	{
		3,					//Number of rows the panel is divided for multi pip
		3,					//Number of columns the panel is divided for multi pip
		3,					//Number of pip windows that are enabled
		MPIP_MASK_POP3,		//0x0444,				//0000_0000_0100_1001
		426,					//The width of each pip window
		256,					//The height of each pip window
		852,					//The horizontal start position of the multi pip configuration
		0,					//The vertical start position of the multi pip configuration
		4,					//The width of the border around each pip window
		4,					//The height of the border around each pip window
		RGB_565(DARKGREY), 	//The color of the border around each window (RGB565 format)
		4,					//The width of the border for the highlight window
		4,					//The height of the border for the highlight window
		RGB_565(WHITE),		//The color of the border for the highlight window (RGB565 format)
		ORIGIN_ON_BORDER,
		852,					//The width of the main window
		768,					//The height of the main window
		0,					//The horizontal start position of the main window in pixels
		0,					//The vertical start position of the main window in pixels
		{	1,0,0,0,      		//Scan order of Pip windows
			2,0,0,0,
			3,0,0,0,
			0,0,0,0
		},
		TOP_RIGHT,
		25,
		10,
		MOVING_HIGHLIGHT,
		100,
		NO_ANCHOR,
		500
	},
	/*
	MPIP Style 4 (3 x 3 GRID)
	*/
	{
		3,					//Number of rows the panel is divided for multi pip
		3,					//Number of columns the panel is divided for multi pip
		3,					//Number of pip windows that are enabled
		MPIP_MASK_THREE_BY_THREE,//0000_0000_0100_1001
		426,					//The width of each pip window
		256,					//The height of each pip window
		0,					//The horizontal start position of the multi pip configuration
		0,					//The vertical start position of the multi pip configuration
		0,					//The width of the border around each pip window
		0,					//The height of the border around each pip window
		RGB_565(LIGHTBLUE), 	//The color of the border around each window (RGB565 format)
		0,					//The width of the border for the highlight window
		0,					//The height of the border for the highlight window
		RGB_565(TEAL),		//The color of the border for the highlight window (RGB565 format)
		ORIGIN_ON_BORDER,
		1278,					//The width of the main window
		768,					//The height of the main window
		0,					//The horizontal start position of the main window in pixels
		0,					//The vertical start position of the main window in pixels
		{	1,2,3,0,       		//Scan order of Pip windows
			4,5,6,0,
			7,8,9,0,
			0,0,0,0
		},
		TOP_RIGHT,
		25,
		10,
		MOVING_HIGHLIGHT,
		100,
		NO_ANCHOR,
		500
	}

};

static MultiPipStyleInfo ROM MultiPipStyle_1080P_1920x1080[] =
{
	/*
	MPIP Style 1 (POP 7)
	*/
	{
		4,					//Number of rows the panel is divided for multi pip
		4,					//Number of columns the panel is divided for multi pip
		7,					//Number of pip windows that are enabled
		MPIP_MASK_POP7,		//1000_1000_1000_1111
		480,					//The width of each pip window
		270,					//The height of each pip window
		0,					//The horizontal start position of the multi pip configuration
		0,					//The vertical start position of the multi pip configuration
		4,					//The width of the border around each pip window
		4,					//The height of the border around each pip window
		RGB_565(LIGHTBLUE), 	//The color of the border around each window (RGB565 format)
		4,					//The width of the border for the highlight window
		4,					//The height of the border for the highlight window
		RGB_565(TEAL),		//The color of the border for the highlight window (RGB565 format)
		ORIGIN_INSIDE_BORDER,
		1440,					//The width of the main window
		810,					//The height of the main window
		0,					//The horizontal start position of the main window in pixels
		270, 				//The vertical start position of the main window in pixels
		{	1,2,3,4,    		//Scan order of Pip windows
			0,0,0,5,
			0,0,0,6,
			0,0,0,7
		},
		TOP_RIGHT,
		25,
		10,
		NON_MOVING_HIGHLIGHT,
		80,
		USE_ANCHOR,
		300
	},
	/*
	MPIP Style 2 (POP 12)
	*/
	{
		4,					//Number of rows the panel is divided for multi pip
		4,					//Number of columns the panel is divided for multi pip
		12,					//Number of pip windows that are enabled
		MPIP_MASK_POP12,	//1111_1001_1001_1111
		480,					//The width of each pip window
		270,					//The height of each pip window
		0,					//The horizontal start position of the multi pip configuration
		0,					//The vertical start position of the multi pip configuration
		2,					//The width of the border around each pip window
		2,					//The height of the border around each pip window
		RGB_565(DARKGREY), 	//The color of the border around each window (RGB565 format)
		2,					//The width of the border for the highlight window
		2,					//The height of the border for the highlight window
		RGB_565(WHITE),		//The color of the border for the highlight window (RGB565 format)
		ORIGIN_ON_BORDER,
		960,					//The width of the main window
		540,					//The height of the main window
		480,					//The horizontal start position of the main window in pixels
		270,					//The vertical start position of the main window in pixels
		{	1,2,3,4,       		//Scan order of Pip windows
			12,0,0,5,
			11,0,0,6,
			10,9,8,7
		},
		TOP_LEFT,
		25,
		10,
		MOVING_HIGHLIGHT,
		200,
		USE_ANCHOR,
		600
	},
	/*
	MPIP Style 3 (POP 3)
	*/
	{
		3,					//Number of rows the panel is divided for multi pip
		3,					//Number of columns the panel is divided for multi pip
		3,					//Number of pip windows that are enabled
		MPIP_MASK_POP3,		//0000_0000_0100_1001
		640,					//The width of each pip window
		360,					//The height of each pip window
		1280,					//The horizontal start position of the multi pip configuration
		0,					//The vertical start position of the multi pip configuration
		4,					//The width of the border around each pip window
		4,					//The height of the border around each pip window
		RGB_565(DARKGREY), 	//The color of the border around each window (RGB565 format)
		4,					//The width of the border for the highlight window
		4,					//The height of the border for the highlight window
		RGB_565(WHITE),		//The color of the border for the highlight window (RGB565 format)
		ORIGIN_ON_BORDER,
		1280,					//The width of the main window
		1080,					//The height of the main window
		0,					//The horizontal start position of the main window in pixels
		0,					//The vertical start position of the main window in pixels
		{	1,0,0,0,      		//Scan order of Pip windows
			2,0,0,0,
			3,0,0,0,
			0,0,0,0
		},
		TOP_RIGHT,
		25,
		10,
		MOVING_HIGHLIGHT,
		100,
		NO_ANCHOR,
		500
	},
	/*
	MPIP Style 4 (3 x 3 GRID)
	*/
	{
		3,					//Number of rows the panel is divided for multi pip
		3,					//Number of columns the panel is divided for multi pip
		3,					//Number of pip windows that are enabled
		MPIP_MASK_THREE_BY_THREE,//0000_0000_0100_1001
		640,					//The width of each pip window
		360,					//The height of each pip window
		0,					//The horizontal start position of the multi pip configuration
		0,					//The vertical start position of the multi pip configuration
		0,					//The width of the border around each pip window
		0,					//The height of the border around each pip window
		RGB_565(LIGHTBLUE), 	//The color of the border around each window (RGB565 format)
		0,					//The width of the border for the highlight window
		0,					//The height of the border for the highlight window
		RGB_565(TEAL),		//The color of the border for the highlight window (RGB565 format)
		ORIGIN_ON_BORDER,
		1920,					//The width of the main window
		1080,					//The height of the main window
		0,					//The horizontal start position of the main window in pixels
		0,					//The vertical start position of the main window in pixels
		{	1,2,3,0,       		//Scan order of Pip windows
			4,5,6,0,
			7,8,9,0,
			0,0,0,0
		},
		TOP_RIGHT,
		25,
		10,
		MOVING_HIGHLIGHT,
		100,
		NO_ANCHOR,
		500
	}

};

static MultiPipStyleInfo ROM MultiPipStyle_WUXGA_1920x1200[] =
{
	/*
	MPIP Style 1 (POP 7)
	*/
	{
		4,					//Number of rows the panel is divided for multi pip
		4,					//Number of columns the panel is divided for multi pip
		7,					//Number of pip windows that are enabled
		MPIP_MASK_POP7,		//1000_1000_1000_1111
		480,					//The width of each pip window
		300,					//The height of each pip window
		0,					//The horizontal start position of the multi pip configuration
		0,					//The vertical start position of the multi pip configuration
		4,					//The width of the border around each pip window
		4,					//The height of the border around each pip window
		RGB_565(LIGHTBLUE), 	//The color of the border around each window (RGB565 format)
		4,					//The width of the border for the highlight window
		4,					//The height of the border for the highlight window
		RGB_565(TEAL),		//The color of the border for the highlight window (RGB565 format)
		ORIGIN_INSIDE_BORDER,
		1440,					//The width of the main window
		900,					//The height of the main window
		0,					//The horizontal start position of the main window in pixels
		300, 				//The vertical start position of the main window in pixels
		{	1,2,3,4,    		//Scan order of Pip windows
			0,0,0,5,
			0,0,0,6,
			0,0,0,7
		},
		TOP_RIGHT,
		25,
		10,
		NON_MOVING_HIGHLIGHT,
		80,
		USE_ANCHOR,
		300
	},
	/*
	MPIP Style 2 (POP 12)
	*/
	{
		4,					//Number of rows the panel is divided for multi pip
		4,					//Number of columns the panel is divided for multi pip
		12,					//Number of pip windows that are enabled
		MPIP_MASK_POP12,	//1111_1001_1001_1111
		480,					//The width of each pip window
		300,					//The height of each pip window
		0,					//The horizontal start position of the multi pip configuration
		0,					//The vertical start position of the multi pip configuration
		2,					//The width of the border around each pip window
		2,					//The height of the border around each pip window
		RGB_565(DARKGREY), 	//The color of the border around each window (RGB565 format)
		2,					//The width of the border for the highlight window
		2,					//The height of the border for the highlight window
		RGB_565(WHITE),		//The color of the border for the highlight window (RGB565 format)
		ORIGIN_ON_BORDER,
		960,					//The width of the main window
		600,					//The height of the main window
		480,					//The horizontal start position of the main window in pixels
		300,					//The vertical start position of the main window in pixels
		{	1,2,3,4,       		//Scan order of Pip windows
			12,0,0,5,
			11,0,0,6,
			10,9,8,7
		},
		TOP_LEFT,
		25,
		10,
		MOVING_HIGHLIGHT,
		200,
		USE_ANCHOR,
		600
	},
	/*
	MPIP Style 3 (POP 3)
	*/
	{
		3,					//Number of rows the panel is divided for multi pip
		3,					//Number of columns the panel is divided for multi pip
		3,					//Number of pip windows that are enabled
		MPIP_MASK_POP3,		//0000_0000_0100_1001
		640,					//The width of each pip window
		400,					//The height of each pip window
		1280,					//The horizontal start position of the multi pip configuration
		0,					//The vertical start position of the multi pip configuration
		4,					//The width of the border around each pip window
		4,					//The height of the border around each pip window
		RGB_565(DARKGREY), 	//The color of the border around each window (RGB565 format)
		4,					//The width of the border for the highlight window
		4,					//The height of the border for the highlight window
		RGB_565(WHITE),		//The color of the border for the highlight window (RGB565 format)
		ORIGIN_ON_BORDER,
		1280,					//The width of the main window
		1200,					//The height of the main window
		0,					//The horizontal start position of the main window in pixels
		0,					//The vertical start position of the main window in pixels
		{	1,0,0,0,      		//Scan order of Pip windows
			2,0,0,0,
			3,0,0,0,
			0,0,0,0
		},
		TOP_RIGHT,
		25,
		10,
		MOVING_HIGHLIGHT,
		100,
		NO_ANCHOR,
		500
	},
	/*
	MPIP Style 4 (3 x 3 GRID)
	*/
	{
		3,					//Number of rows the panel is divided for multi pip
		3,					//Number of columns the panel is divided for multi pip
		3,					//Number of pip windows that are enabled
		MPIP_MASK_THREE_BY_THREE,//0000_0000_0100_1001
		640,					//The width of each pip window
		400,					//The height of each pip window
		0,					//The horizontal start position of the multi pip configuration
		0,					//The vertical start position of the multi pip configuration
		0,					//The width of the border around each pip window
		0,					//The height of the border around each pip window
		RGB_565(LIGHTBLUE), 	//The color of the border around each window (RGB565 format)
		0,					//The width of the border for the highlight window
		0,					//The height of the border for the highlight window
		RGB_565(TEAL),		//The color of the border for the highlight window (RGB565 format)
		ORIGIN_ON_BORDER,
		1920,					//The width of the main window
		1200,					//The height of the main window
		0,					//The horizontal start position of the main window in pixels
		0,					//The vertical start position of the main window in pixels
		{	1,2,3,0,       		//Scan order of Pip windows
			4,5,6,0,
			7,8,9,0,
			0,0,0,0
		},
		TOP_RIGHT,
		25,
		10,
		MOVING_HIGHLIGHT,
		100,
		NO_ANCHOR,
		500
	}

};

static MultiPipStyleInfo ROM MultiPipStyle_UXGA_1600x1200[] =
{
	/*
	MPIP Style 1 (POP 7)
	*/
	{
		4,					//Number of rows the panel is divided for multi pip
		4,					//Number of columns the panel is divided for multi pip
		7,					//Number of pip windows that are enabled
		MPIP_MASK_POP7,		//1000_1000_1000_1111
		400,				//The width of each pip window
		300,				//The height of each pip window
		0,					//The horizontal start position of the multi pip configuration
		0,					//The vertical start position of the multi pip configuration
		4,					//The width of the border around each pip window
		4,					//The height of the border around each pip window
		RGB_565(LIGHTBLUE), 	//The color of the border around each window (RGB565 format)
		4,					//The width of the border for the highlight window
		4,					//The height of the border for the highlight window
		RGB_565(TEAL),		//The color of the border for the highlight window (RGB565 format)
		ORIGIN_INSIDE_BORDER,
		1200,				//The width of the main window
		900,				//The height of the main window
		0,					//The horizontal start position of the main window in pixels
		300, 				//The vertical start position of the main window in pixels
		{	1,2,3,4,    		//Scan order of Pip windows
			0,0,0,5,
			0,0,0,6,
			0,0,0,7
		},
		TOP_RIGHT,
		25,
		10,
		NON_MOVING_HIGHLIGHT,
		80,
		USE_ANCHOR,
		300
	},
	/*
	MPIP Style 2 (POP 12)
	*/
	{
		4,					//Number of rows the panel is divided for multi pip
		4,					//Number of columns the panel is divided for multi pip
		12,					//Number of pip windows that are enabled
		MPIP_MASK_POP12,	//1111_1001_1001_1111
		400,				//The width of each pip window
		300,				//The height of each pip window
		0,					//The horizontal start position of the multi pip configuration
		0,					//The vertical start position of the multi pip configuration
		2,					//The width of the border around each pip window
		2,					//The height of the border around each pip window
		RGB_565(DARKGREY), 	//The color of the border around each window (RGB565 format)
		2,					//The width of the border for the highlight window
		2,					//The height of the border for the highlight window
		RGB_565(WHITE),		//The color of the border for the highlight window (RGB565 format)
		ORIGIN_ON_BORDER,
		800,				//The width of the main window
		600,				//The height of the main window
		400,				//The horizontal start position of the main window in pixels
		300,				//The vertical start position of the main window in pixels
		{	1,2,3,4,       		//Scan order of Pip windows
			12,0,0,5,
			11,0,0,6,
			10,9,8,7
		},
		TOP_LEFT,
		25,
		10,
		MOVING_HIGHLIGHT,
		200,
		USE_ANCHOR,
		600
	},
	/*
	MPIP Style 3 (POP 3)
	*/
	{
		3,					//Number of rows the panel is divided for multi pip
		3,					//Number of columns the panel is divided for multi pip
		3,					//Number of pip windows that are enabled
		MPIP_MASK_POP3,		//0000_0000_0100_1001
		534,				//The width of each pip window
		400,				//The height of each pip window
		1066,				//The horizontal start position of the multi pip configuration
		0,					//The vertical start position of the multi pip configuration
		4,					//The width of the border around each pip window
		4,					//The height of the border around each pip window
		RGB_565(DARKGREY), 	//The color of the border around each window (RGB565 format)
		4,					//The width of the border for the highlight window
		4,					//The height of the border for the highlight window
		RGB_565(WHITE),		//The color of the border for the highlight window (RGB565 format)
		ORIGIN_ON_BORDER,
		1066,				//The width of the main window
		1200,				//The height of the main window
		0,					//The horizontal start position of the main window in pixels
		0,					//The vertical start position of the main window in pixels
		{	1,0,0,0,      		//Scan order of Pip windows
			2,0,0,0,
			3,0,0,0,
			0,0,0,0
		},
		TOP_RIGHT,
		25,
		10,
		MOVING_HIGHLIGHT,
		100,
		NO_ANCHOR,
		500
	},
	/*
	MPIP Style 4 (3 x 3 GRID)
	*/
	{
		3,					//Number of rows the panel is divided for multi pip
		3,					//Number of columns the panel is divided for multi pip
		3,					//Number of pip windows that are enabled
		MPIP_MASK_THREE_BY_THREE,//0000_0000_0100_1001
		533,				//The width of each pip window
		400,				//The height of each pip window
		0,					//The horizontal start position of the multi pip configuration
		0,					//The vertical start position of the multi pip configuration
		0,					//The width of the border around each pip window
		0,					//The height of the border around each pip window
		RGB_565(LIGHTBLUE), 	//The color of the border around each window (RGB565 format)
		0,					//The width of the border for the highlight window
		0,					//The height of the border for the highlight window
		RGB_565(TEAL),		//The color of the border for the highlight window (RGB565 format)
		ORIGIN_ON_BORDER,
		1596,				//The width of the main window
		1200,				//The height of the main window
		0,					//The horizontal start position of the main window in pixels
		0,					//The vertical start position of the main window in pixels
		{	1,2,3,0,       		//Scan order of Pip windows
			4,5,6,0,
			7,8,9,0,
			0,0,0,0
		},
		TOP_RIGHT,
		25,
		10,
		MOVING_HIGHLIGHT,
		100,
		NO_ANCHOR,
		500
	}

};

static MultiPipStyleInfo ROM MultiPipStyle_WXGA_1366x768[] =
{
	/*
	MPIP Style 1 (POP 7)
	*/
	{
		4,					//Number of rows the panel is divided for multi pip
		4,					//Number of columns the panel is divided for multi pip
		7,					//Number of pip windows that are enabled
		MPIP_MASK_POP7,		//1000_1000_1000_1111
		341,					//The width of each pip window
		192,					//The height of each pip window
		0,					//The horizontal start position of the multi pip configuration
		0,					//The vertical start position of the multi pip configuration
		4,					//The width of the border around each pip window
		4,					//The height of the border around each pip window
		RGB_565(LIGHTBLUE), 	//The color of the border around each window (RGB565 format)
		4,					//The width of the border for the highlight window
		4,					//The height of the border for the highlight window
		RGB_565(TEAL),		//The color of the border for the highlight window (RGB565 format)
		ORIGIN_INSIDE_BORDER,
		1024,					//The width of the main window
		576,					//The height of the main window
		0,					//The horizontal start position of the main window in pixels
		192, 				//The vertical start position of the main window in pixels
		{	1,2,3,4,    		//Scan order of Pip windows
			0,0,0,5,
			0,0,0,6,
			0,0,0,7
		},
		TOP_RIGHT,
		25,
		10,
		NON_MOVING_HIGHLIGHT,
		80,
		USE_ANCHOR,
		300
	},
	/*
	MPIP Style 2 (POP 12)
	*/
	{
		4,					//Number of rows the panel is divided for multi pip
		4,					//Number of columns the panel is divided for multi pip
		12,					//Number of pip windows that are enabled
		MPIP_MASK_POP12,	//1111_1001_1001_1111
		341,					//The width of each pip window
		192,					//The height of each pip window
		0,					//The horizontal start position of the multi pip configuration
		0,					//The vertical start position of the multi pip configuration
		2,					//The width of the border around each pip window
		2,					//The height of the border around each pip window
		RGB_565(DARKGREY), 	//The color of the border around each window (RGB565 format)
		2,					//The width of the border for the highlight window
		2,					//The height of the border for the highlight window
		RGB_565(WHITE),		//The color of the border for the highlight window (RGB565 format)
		ORIGIN_ON_BORDER,
		683,					//The width of the main window
		384,					//The height of the main window
		341,					//The horizontal start position of the main window in pixels
		192,					//The vertical start position of the main window in pixels
		{	1,2,3,4,       		//Scan order of Pip windows
			12,0,0,5,
			11,0,0,6,
			10,9,8,7
		},
		TOP_LEFT,
		25,
		10,
		MOVING_HIGHLIGHT,
		200,
		USE_ANCHOR,
		600
	},
	/*
	MPIP Style 3 (POP 3)
	*/
	{
		3,					//Number of rows the panel is divided for multi pip
		3,					//Number of columns the panel is divided for multi pip
		3,					//Number of pip windows that are enabled
		MPIP_MASK_POP3,		//0000_0000_0100_1001
		455,					//The width of each pip window
		256,					//The height of each pip window
		910,					//The horizontal start position of the multi pip configuration
		0,					//The vertical start position of the multi pip configuration
		4,					//The width of the border around each pip window
		4,					//The height of the border around each pip window
		RGB_565(DARKGREY), 	//The color of the border around each window (RGB565 format)
		4,					//The width of the border for the highlight window
		4,					//The height of the border for the highlight window
		RGB_565(WHITE),		//The color of the border for the highlight window (RGB565 format)
		ORIGIN_ON_BORDER,
		910,					//The width of the main window
		768,					//The height of the main window
		0,					//The horizontal start position of the main window in pixels
		0,					//The vertical start position of the main window in pixels
		{	1,0,0,0,      		//Scan order of Pip windows
			2,0,0,0,
			3,0,0,0,
			0,0,0,0
		},
		TOP_RIGHT,
		25,
		10,
		MOVING_HIGHLIGHT,
		100,
		NO_ANCHOR,
		500
	},
	/*
	MPIP Style 4 (3 x 3 GRID)
	*/
	{
		3,					//Number of rows the panel is divided for multi pip
		3,					//Number of columns the panel is divided for multi pip
		3,					//Number of pip windows that are enabled
		MPIP_MASK_THREE_BY_THREE,//0000_0000_0100_1001
		455,					//The width of each pip window
		256,					//The height of each pip window
		0,					//The horizontal start position of the multi pip configuration
		0,					//The vertical start position of the multi pip configuration
		0,					//The width of the border around each pip window
		0,					//The height of the border around each pip window
		RGB_565(LIGHTBLUE), 	//The color of the border around each window (RGB565 format)
		0,					//The width of the border for the highlight window
		0,					//The height of the border for the highlight window
		RGB_565(TEAL),		//The color of the border for the highlight window (RGB565 format)
		ORIGIN_ON_BORDER,
		1362,					//The width of the main window
		768,					//The height of the main window
		0,					//The horizontal start position of the main window in pixels
		0,					//The vertical start position of the main window in pixels
		{	1,2,3,0,       		//Scan order of Pip windows
			4,5,6,0,
			7,8,9,0,
			0,0,0,0
		},
		TOP_RIGHT,
		25,
		10,
		MOVING_HIGHLIGHT,
		100,
		NO_ANCHOR,
		500
	}

};

static MultiPipStyleInfo ROM MultiPipStyle_WXGA_1280x720[] =
{
	/*
	MPIP Style 1 (POP 7)
	*/
	{
		4,					//Number of rows the panel is divided for multi pip
		4,					//Number of columns the panel is divided for multi pip
		7,					//Number of pip windows that are enabled
		MPIP_MASK_POP7,		//1000_1000_1000_1111
		320,					//The width of each pip window
		180,					//The height of each pip window
		0,					//The horizontal start position of the multi pip configuration
		0,					//The vertical start position of the multi pip configuration
		4,					//The width of the border around each pip window
		4,					//The height of the border around each pip window
		RGB_565(LIGHTBLUE), 	//The color of the border around each window (RGB565 format)
		4,					//The width of the border for the highlight window
		4,					//The height of the border for the highlight window
		RGB_565(TEAL),		//The color of the border for the highlight window (RGB565 format)
		ORIGIN_INSIDE_BORDER,
		960,					//The width of the main window
		540,					//The height of the main window
		0,					//The horizontal start position of the main window in pixels
		180, 				//The vertical start position of the main window in pixels
		{	1,2,3,4,    		//Scan order of Pip windows
			0,0,0,5,
			0,0,0,6,
			0,0,0,7
		},
		TOP_RIGHT,
		25,
		10,
		NON_MOVING_HIGHLIGHT,
		80,
		USE_ANCHOR,
		300
	},
	/*
	MPIP Style 2 (POP 12)
	*/
	{
		4,					//Number of rows the panel is divided for multi pip
		4,					//Number of columns the panel is divided for multi pip
		12,					//Number of pip windows that are enabled
		MPIP_MASK_POP12,	//1111_1001_1001_1111
		320,					//The width of each pip window
		180,					//The height of each pip window
		0,					//The horizontal start position of the multi pip configuration
		0,					//The vertical start position of the multi pip configuration
		2,					//The width of the border around each pip window
		2,					//The height of the border around each pip window
		RGB_565(DARKGREY), 	//The color of the border around each window (RGB565 format)
		2,					//The width of the border for the highlight window
		2,					//The height of the border for the highlight window
		RGB_565(WHITE),		//The color of the border for the highlight window (RGB565 format)
		ORIGIN_ON_BORDER,
		640,					//The width of the main window
		360,					//The height of the main window
		320,					//The horizontal start position of the main window in pixels
		180,					//The vertical start position of the main window in pixels
		{	1,2,3,4,       		//Scan order of Pip windows
			12,0,0,5,
			11,0,0,6,
			10,9,8,7
		},
		TOP_LEFT,
		25,
		10,
		MOVING_HIGHLIGHT,
		200,
		USE_ANCHOR,
		600
	},
	/*
	MPIP Style 3 (POP 3)
	*/
	{
		3,					//Number of rows the panel is divided for multi pip
		3,					//Number of columns the panel is divided for multi pip
		3,					//Number of pip windows that are enabled
		MPIP_MASK_POP3,		//0x0444,				//0000_0000_0100_1001
		426,					//The width of each pip window
		240,					//The height of each pip window
		852,					//The horizontal start position of the multi pip configuration
		0,					//The vertical start position of the multi pip configuration
		4,					//The width of the border around each pip window
		4,					//The height of the border around each pip window
		RGB_565(DARKGREY), 	//The color of the border around each window (RGB565 format)
		4,					//The width of the border for the highlight window
		4,					//The height of the border for the highlight window
		RGB_565(WHITE),		//The color of the border for the highlight window (RGB565 format)
		ORIGIN_ON_BORDER,
		852,					//The width of the main window
		720,					//The height of the main window
		0,					//The horizontal start position of the main window in pixels
		0,					//The vertical start position of the main window in pixels
		{	1,0,0,0,      		//Scan order of Pip windows
			2,0,0,0,
			3,0,0,0,
			0,0,0,0
		},
		TOP_RIGHT,
		25,
		10,
		MOVING_HIGHLIGHT,
		100,
		NO_ANCHOR,
		500
	},
	/*
	MPIP Style 4 (3 x 3 GRID)
	*/
	{
		3,					//Number of rows the panel is divided for multi pip
		3,					//Number of columns the panel is divided for multi pip
		3,					//Number of pip windows that are enabled
		MPIP_MASK_THREE_BY_THREE,//0000_0000_0100_1001
		426,					//The width of each pip window
		240,					//The height of each pip window
		0,					//The horizontal start position of the multi pip configuration
		0,					//The vertical start position of the multi pip configuration
		0,					//The width of the border around each pip window
		0,					//The height of the border around each pip window
		RGB_565(LIGHTBLUE), 	//The color of the border around each window (RGB565 format)
		0,					//The width of the border for the highlight window
		0,					//The height of the border for the highlight window
		RGB_565(TEAL),		//The color of the border for the highlight window (RGB565 format)
		ORIGIN_ON_BORDER,
		1278,					//The width of the main window
		720,					//The height of the main window
		0,					//The horizontal start position of the main window in pixels
		0,					//The vertical start position of the main window in pixels
		{	1,2,3,0,       		//Scan order of Pip windows
			4,5,6,0,
			7,8,9,0,
			0,0,0,0
		},
		TOP_RIGHT,
		25,
		10,
		MOVING_HIGHLIGHT,
		100,
		NO_ANCHOR,
		500
	}

};

static MultiPipStyleInfo ROM MultiPipStyle_SXGA_1280x1024[] =
{
	/*
	MPIP Style 1 (POP 7)
	*/
	{
		4,					//Number of rows the panel is divided for multi pip
		4,					//Number of columns the panel is divided for multi pip
		7,					//Number of pip windows that are enabled
		MPIP_MASK_POP7,		//1000_1000_1000_1111
		320,					//The width of each pip window
		256,					//The height of each pip window
		0,					//The horizontal start position of the multi pip configuration
		0,					//The vertical start position of the multi pip configuration
		4,					//The width of the border around each pip window
		4,					//The height of the border around each pip window
		RGB_565(LIGHTBLUE), 	//The color of the border around each window (RGB565 format)
		4,					//The width of the border for the highlight window
		4,					//The height of the border for the highlight window
		RGB_565(TEAL),		//The color of the border for the highlight window (RGB565 format)
		ORIGIN_INSIDE_BORDER,
		960,					//The width of the main window
		768,					//The height of the main window
		0,					//The horizontal start position of the main window in pixels
		256, 				//The vertical start position of the main window in pixels
		{	1,2,3,4,    		//Scan order of Pip windows
			0,0,0,5,
			0,0,0,6,
			0,0,0,7
		},
		TOP_RIGHT,
		25,
		10,
		NON_MOVING_HIGHLIGHT,
		80,
		USE_ANCHOR,
		300
	},
	/*
	MPIP Style 2 (POP 12)
	*/
	{
		4,					//Number of rows the panel is divided for multi pip
		4,					//Number of columns the panel is divided for multi pip
		12,					//Number of pip windows that are enabled
		MPIP_MASK_POP12,	//1111_1001_1001_1111
		320,					//The width of each pip window
		256,					//The height of each pip window
		0,					//The horizontal start position of the multi pip configuration
		0,					//The vertical start position of the multi pip configuration
		2,					//The width of the border around each pip window
		2,					//The height of the border around each pip window
		RGB_565(DARKGREY), 	//The color of the border around each window (RGB565 format)
		2,					//The width of the border for the highlight window
		2,					//The height of the border for the highlight window
		RGB_565(WHITE),		//The color of the border for the highlight window (RGB565 format)
		ORIGIN_ON_BORDER,
		640,					//The width of the main window
		512,					//The height of the main window
		320,					//The horizontal start position of the main window in pixels
		256,					//The vertical start position of the main window in pixels
		{	1,2,3,4,       		//Scan order of Pip windows
			12,0,0,5,
			11,0,0,6,
			10,9,8,7
		},
		TOP_LEFT,
		25,
		10,
		MOVING_HIGHLIGHT,
		200,
		USE_ANCHOR,
		600
	},
	/*
	MPIP Style 3 (POP 3)
	*/
	{
		3,					//Number of rows the panel is divided for multi pip
		3,					//Number of columns the panel is divided for multi pip
		3,					//Number of pip windows that are enabled
		MPIP_MASK_POP3,		//0x0444,				//0000_0000_0100_1001
		426,					//The width of each pip window
		341,					//The height of each pip window
		852,					//The horizontal start position of the multi pip configuration
		0,					//The vertical start position of the multi pip configuration
		4,					//The width of the border around each pip window
		4,					//The height of the border around each pip window
		RGB_565(DARKGREY), 	//The color of the border around each window (RGB565 format)
		4,					//The width of the border for the highlight window
		4,					//The height of the border for the highlight window
		RGB_565(WHITE),		//The color of the border for the highlight window (RGB565 format)
		ORIGIN_ON_BORDER,
		852,					//The width of the main window
		768,					//The height of the main window
		0,					//The horizontal start position of the main window in pixels
		0,					//The vertical start position of the main window in pixels
		{	1,0,0,0,      		//Scan order of Pip windows
			2,0,0,0,
			3,0,0,0,
			0,0,0,0
		},
		TOP_RIGHT,
		25,
		10,
		MOVING_HIGHLIGHT,
		100,
		NO_ANCHOR,
		500
	},
	/*
	MPIP Style 4 (3 x 3 GRID)
	*/
	{
		3,					//Number of rows the panel is divided for multi pip
		3,					//Number of columns the panel is divided for multi pip
		3,					//Number of pip windows that are enabled
		MPIP_MASK_THREE_BY_THREE,//0000_0000_0100_1001
		426,					//The width of each pip window
		341,					//The height of each pip window
		0,					//The horizontal start position of the multi pip configuration
		0,					//The vertical start position of the multi pip configuration
		0,					//The width of the border around each pip window
		0,					//The height of the border around each pip window
		RGB_565(LIGHTBLUE), 	//The color of the border around each window (RGB565 format)
		0,					//The width of the border for the highlight window
		0,					//The height of the border for the highlight window
		RGB_565(TEAL),		//The color of the border for the highlight window (RGB565 format)
		ORIGIN_ON_BORDER,
		1278,					//The width of the main window
		768,					//The height of the main window
		0,					//The horizontal start position of the main window in pixels
		0,					//The vertical start position of the main window in pixels
		{	1,2,3,0,       		//Scan order of Pip windows
			4,5,6,0,
			7,8,9,0,
			0,0,0,0
		},
		TOP_RIGHT,
		25,
		10,
		MOVING_HIGHLIGHT,
		100,
		NO_ANCHOR,
		500
	}

};

static MultiPipStyleInfo ROM MultiPipStyle_XGA_1024x768[] =
{
	/*
	MPIP Style 1 (POP 7)
	*/
	{
		4,					//Number of rows the panel is divided for multi pip
		4,					//Number of columns the panel is divided for multi pip
		7,					//Number of pip windows that are enabled
		MPIP_MASK_POP7,		//1000_1000_1000_1111
		256,					//The width of each pip window
		192,					//The height of each pip window
		0,					//The horizontal start position of the multi pip configuration
		0,					//The vertical start position of the multi pip configuration
		4,					//The width of the border around each pip window
		4,					//The height of the border around each pip window
		RGB_565(LIGHTBLUE), 	//The color of the border around each window (RGB565 format)
		4,					//The width of the border for the highlight window
		4,					//The height of the border for the highlight window
		RGB_565(TEAL),		//The color of the border for the highlight window (RGB565 format)
		ORIGIN_INSIDE_BORDER,
		768,					//The width of the main window
		576,					//The height of the main window
		0,					//The horizontal start position of the main window in pixels
		192, 				//The vertical start position of the main window in pixels
		{	1,2,3,4,    		//Scan order of Pip windows
			0,0,0,5,
			0,0,0,6,
			0,0,0,7
		},
		TOP_RIGHT,
		25,
		10,
		NON_MOVING_HIGHLIGHT,
		80,
		USE_ANCHOR,
		300
	},
	/*
	MPIP Style 2 (POP 12)
	*/
	{
		4,					//Number of rows the panel is divided for multi pip
		4,					//Number of columns the panel is divided for multi pip
		12,					//Number of pip windows that are enabled
		MPIP_MASK_POP12,	//1111_1001_1001_1111
		256,					//The width of each pip window
		192,					//The height of each pip window
		0,					//The horizontal start position of the multi pip configuration
		0,					//The vertical start position of the multi pip configuration
		2,					//The width of the border around each pip window
		2,					//The height of the border around each pip window
		RGB_565(DARKGREY), 	//The color of the border around each window (RGB565 format)
		2,					//The width of the border for the highlight window
		2,					//The height of the border for the highlight window
		RGB_565(WHITE),		//The color of the border for the highlight window (RGB565 format)
		ORIGIN_ON_BORDER,
		512,					//The width of the main window
		384,					//The height of the main window
		256,					//The horizontal start position of the main window in pixels
		192,					//The vertical start position of the main window in pixels
		{	1,2,3,4,       		//Scan order of Pip windows
			12,0,0,5,
			11,0,0,6,
			10,9,8,7
		},
		TOP_LEFT,
		25,
		10,
		MOVING_HIGHLIGHT,
		200,
		USE_ANCHOR,
		600
	},
	/*
	MPIP Style 3 (POP 3)
	*/
	{
		3,					//Number of rows the panel is divided for multi pip
		3,					//Number of columns the panel is divided for multi pip
		3,					//Number of pip windows that are enabled
		MPIP_MASK_POP3,		//0x0444,				//0000_0000_0100_1001
		341,					//The width of each pip window
		256,					//The height of each pip window
		852,					//The horizontal start position of the multi pip configuration
		0,					//The vertical start position of the multi pip configuration
		4,					//The width of the border around each pip window
		4,					//The height of the border around each pip window
		RGB_565(DARKGREY), 	//The color of the border around each window (RGB565 format)
		4,					//The width of the border for the highlight window
		4,					//The height of the border for the highlight window
		RGB_565(WHITE),		//The color of the border for the highlight window (RGB565 format)
		ORIGIN_ON_BORDER,
		682,					//The width of the main window
		768,					//The height of the main window
		0,					//The horizontal start position of the main window in pixels
		0,					//The vertical start position of the main window in pixels
		{	1,0,0,0,      		//Scan order of Pip windows
			2,0,0,0,
			3,0,0,0,
			0,0,0,0
		},
		TOP_RIGHT,
		25,
		10,
		MOVING_HIGHLIGHT,
		100,
		NO_ANCHOR,
		500
	},
	/*
	MPIP Style 4 (3 x 3 GRID)
	*/
	{
		3,					//Number of rows the panel is divided for multi pip
		3,					//Number of columns the panel is divided for multi pip
		3,					//Number of pip windows that are enabled
		MPIP_MASK_THREE_BY_THREE,//0000_0000_0100_1001
		341,					//The width of each pip window
		256,					//The height of each pip window
		0,					//The horizontal start position of the multi pip configuration
		0,					//The vertical start position of the multi pip configuration
		0,					//The width of the border around each pip window
		0,					//The height of the border around each pip window
		RGB_565(LIGHTBLUE), 	//The color of the border around each window (RGB565 format)
		0,					//The width of the border for the highlight window
		0,					//The height of the border for the highlight window
		RGB_565(TEAL),		//The color of the border for the highlight window (RGB565 format)
		ORIGIN_ON_BORDER,
		1024,					//The width of the main window
		768,					//The height of the main window
		0,					//The horizontal start position of the main window in pixels
		0,					//The vertical start position of the main window in pixels
		{	1,2,3,0,       		//Scan order of Pip windows
			4,5,6,0,
			7,8,9,0,
			0,0,0,0
		},
		TOP_RIGHT,
		25,
		10,
		MOVING_HIGHLIGHT,
		100,
		NO_ANCHOR,
		500
	}

};

// Variable to indicated if multipip controls are available (locked or unlocked)
static BYTE b_MultiPIPLockControl = Unlocked;
#endif

//******************************************************************************
//  C O D E
//******************************************************************************

//******************************************************************************
//
// FUNCTION       : BYTE CanMultiPipBeOn(void)
//
// USAGE          : returns whether MultiPip can be turned on
//
// INPUT          : 
//
// OUTPUT         : True or False
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
BYTE CanMultiPipBeOn(void)
{
#ifndef NO_PIP_SUPPORT
#if (FEATURE_MultiPxp == ENABLE)
	//
	// Mulitpip is disabled if parental control is enabled, panoramic aspect ratio, or dynamic
	// scaling is being used.
	//
	#if (MAX_TUNER_INPUTS == 1)
		if((GetPortProcType(UserPrefCurrentInputMain)==EXTERNAL || IsInternalHdmiPort(gmvb_MainChannel))
		 	&& (UserPrefTVRating == PROG_RATING_7_NO_BLOCK)		// Can not turn on if TV rating is enabled
			&& (UserPrefMPRating == PROG_RATING_7_NO_BLOCK) 		// Can not turn on if Movie rating is enabled
			&& (UserPrefTVUnrated == ENABLE_UNRATED)				// Can not turn on if channel rating blocking is enabled			
			&& (UserPrefAspectRatioMain != AR_PANORAMIC)			// Can not turn on if Main AR is panoramic
			&& (B_DynScalingDirection == DYN_IDLE)				// Can not turn on if dynamic scaling is taking place
			&& (UserPrefTVAutoScanRequired ==0)					// Can not turn on if autoscan is required
			&& (UserPrefPipMode!=SIDE_BY_SIDE)					// Can not turn on if in Side by Side mode
			&& (UserPrefPipMode!=SIDE_BY_SIDE_TALL)				// Can not turn on if in Side by Side mode
         && (UserPrefInfoDisplayOn != 1)                    //Can not turn on if Info Display is on (SCR 3641)
			&& (UserPrefFreeze != 1)                           //Can not turn on if Freeze is on (SCR 3641)
         && (UserPrefSpeakers != D_MUTE_ON)                         //Can not turn on if Mute is on (SCR 3641)
			&& (UserPrefChannelBlockStateFlag==0 || !IsInputTuner()) // Can not turn on if current channel is channel blocked
			&& !IsMultiPipControlLocked())					// Can not turn on if MPIP control is locked
			return TRUE;
		else
			return FALSE;
    #else
		if ( (UserPrefTVRating == PROG_RATING_7_NO_BLOCK)			// Can not turn on if TV rating is enabled
			&& (UserPrefMPRating == PROG_RATING_7_NO_BLOCK)		// Can not turn on if Movie rating is enabled
			&& (UserPrefTVUnrated == ENABLE_UNRATED)				// Can not turn on if channel rating blocking is enabled
			&& (UserPrefAspectRatioMain != AR_PANORAMIC)			// Can not turn on if Main AR is panoramic
			&& (B_DynScalingDirection == DYN_IDLE)				// Can not turn on if dynamic scaling is taking place
			&& (UserPrefTVAutoScanRequired ==0) 					// Can not turn on if autoscan is required
			&& (UserPrefPipMode!=SIDE_BY_SIDE)					// Can not turn on if in Side by Side mode
			&& (UserPrefPipMode!=SIDE_BY_SIDE_TALL)				// Can not turn on if in Side by Side mode
			&& (UserPrefChannelBlockStateFlag==0 || !IsInputTuner()) // Can not turn on if current channel is channel blocked
			&& !IsMultiPipControlLocked())						// Can not turn on if MPIP control is locked
			return TRUE;
		else
			return FALSE;
    #endif
#else
	return FALSE;
#endif	
#else
	return FALSE;
#endif	// ifndef NO_PIP_SUPPORT
}

//******************************************************************************
//
// FUNCTION       : void TurnOnMultiPipStyle(void)
//
// USAGE          : Turns on multi-pip with the style set in the userpref
//
// INPUT          : 
//
// OUTPUT         : None
//
// GLOBALS        : UserPrefMPipConfig	(R)
//
// USED_REGS      : None
//
// PRE_CONDITION  : 
//
// POST_CONDITION : 
//
//******************************************************************************
void TurnOnMultiPipStyle(void)
{
#if (FEATURE_MultiPxp == ENABLE)
	/*
	Entry point from Osd to turn on MultiPip. Pass handle to correct MPip style to start
	*/
	MultiPipStyleInfo ROM *MultiPipStyle;

	{
		//Function that called thought State Maishine (action function is OSD) wrapped in WATCHDOG timer. 
		//When that timer elapse it requested DMA reset. SCR1909

		WORD w_readWatchDogTimer = gm_TimerRead(SYSTEM_WATCHDOG_TMR);
		if((w_readWatchDogTimer !=0 ) && (w_readWatchDogTimer < TIME_FOR_MPIP_SETUP))
		{
			gm_TimerStart(SYSTEM_WATCHDOG_TMR, TIME_FOR_MPIP_SETUP);
		}
	}

	#ifdef SMART_SWAP
	//disable multipip when gmvb_PipChannel != CH_B because multipip is not possible
	//  on CH_A
	if (gmvb_PipChannel != CH_B)
	{
		//perform output swap...
		gmt_PHY_CH B_Channel = gmvb_MainChannel;
		gmvb_MainChannel = gmvb_PipChannel;
		gmvb_PipChannel = B_Channel;

		SetDisplayOff(CH_A, BACKGROUND_ONLY);
		SetDisplayOff(CH_B, BACKGROUND_ONLY);

		//followed by input swap
		SwapMainPip_InputSwap();
		ModeSetupSwap();
		ModeSetupRequestCancel(CH_A, REQ_MS_BLENDER, PARAM_NONE);
		ModeSetupRequestCancel(CH_B, REQ_MS_BLENDER, PARAM_NONE);
	}
	#else
		//disable multipip when gmvb_PipChannel != CH_B because multipip is not possible
		//  on CH_A
		if (gmvb_PipChannel != CH_B)
		{
			//notify OSD that we are exiting multipip
			OSDRequestMsg(gmvb_MainChannel, MTO_MultiPipOff, REQUEST_NOW);
			return;
		}
	#endif //SMART_SWAP

	switch (PanelType)
	{
		case PANEL_XGA:
			MultiPipStyle = MultiPipStyle_XGA_1024x768;
			break;
		case PANEL_SXGA:
			MultiPipStyle = MultiPipStyle_SXGA_1280x1024;
			break;
		case PANEL_WXGA:
			if (PanelWidth == 1280 && PanelHeight == 768)
			{
				MultiPipStyle = MultiPipStyle_WXGA_1280x768;
			}
			else if (PanelWidth == 1280 && PanelHeight == 720)
			{
				MultiPipStyle = MultiPipStyle_WXGA_1280x720;
			}
			else if (PanelWidth == 1366 && PanelHeight == 768)
			{
				MultiPipStyle = MultiPipStyle_WXGA_1366x768;
			}
			break;
		case PANEL_WUXGA:
			MultiPipStyle = MultiPipStyle_WUXGA_1920x1200;
			break;
		case PANEL_1080P:
			MultiPipStyle = MultiPipStyle_1080P_1920x1080;
			break;
		case PANEL_UXGA:
			MultiPipStyle = MultiPipStyle_UXGA_1600x1200;
			break;
		default:
			MultiPipStyle = MultiPipStyle_WXGA_1280x768;
	}		

	UserPrefCurrentInputPip = ALL_BOARDS_RF1;
	AdjustCurrentInputPip();

	MultiPipTurnOn(&MultiPipStyle[UserPrefMPipConfig]);
#endif	
}

//******************************************************************************
//
// FUNCTION       : void MPipConfigIncDec(void)
//
// USAGE          : Increment and decrements MPIP config userpref with wrapping taken care of
//
// INPUT          : 
//
// OUTPUT         : None
//
// GLOBALS        : UserPrefMPipConfig	(R)
//
// USED_REGS      : None
//
// PRE_CONDITION  : 
//
// POST_CONDITION : 
//
//******************************************************************************
void MPipConfigIncDec(void)
{
#if (FEATURE_MultiPxp == ENABLE)
	BYTE Direction = gm_OsdGetCurrentSATOffsetByte();
	switch (Direction)
	{
		case UP:
			UserPrefMPipConfig++;
			UserPrefMPipConfig %= NUM_MPIP_CONFIGS;
			break;
		case DOWN:
			if (UserPrefMPipConfig == 0)
				UserPrefMPipConfig = NUM_MPIP_CONFIGS - 1;
			else
				UserPrefMPipConfig--;
			break;
	}
#endif	
}

//******************************************************************************
//
// FUNCTION       : BOOL IsMultiPipControlLocked(void)
//
// USAGE          : Returns true if MPIP controls are locked
//
// INPUT          : 
//
// OUTPUT         : b_MultiPIPLockControl or False
//
// GLOBALS        : b_MultiPIPLockControl	(R)
//
// USED_REGS      : None
//
// PRE_CONDITION  : 
//
// POST_CONDITION : 
//
//******************************************************************************
BOOL IsMultiPipControlLocked(void)
{
#if (FEATURE_MultiPxp == ENABLE)
	return b_MultiPIPLockControl;
#else
	return FALSE;
#endif	
}

//******************************************************************************
//
// FUNCTION       : void LockMultiPipControl(void)
//
// USAGE          : Locks MPIP controls
//
// INPUT          : 
//
// OUTPUT         : None
//
// GLOBALS        : b_MultiPIPLockControl	(R)
//
// USED_REGS      : None
//
// PRE_CONDITION  : 
//
// POST_CONDITION : 
//
//******************************************************************************
void LockMultiPipControl(void)
{
#if (FEATURE_MultiPxp == ENABLE)
	b_MultiPIPLockControl = Locked;
#endif
}

//******************************************************************************
//
// FUNCTION       : void UnlockMultiPipControl(void)
//
// USAGE          : Unlocks MPIP controls
//
// INPUT          : 
//
// OUTPUT         : None
//
// GLOBALS        : b_MultiPIPLockControl	(R)
//
// USED_REGS      : None
//
// PRE_CONDITION  : 
//
// POST_CONDITION : 
//
//******************************************************************************
void UnlockMultiPipControl(void)
{
#if (FEATURE_MultiPxp == ENABLE)
	b_MultiPIPLockControl = Unlocked;
#endif	
}

	

