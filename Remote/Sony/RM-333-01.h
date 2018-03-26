/*
	$Workfile:   RM-333-01.h  $
	$Revision: 1.1 $
	$Date: 2010/03/17 06:24:44 $
*/
#ifndef __IR_REMOTE_H__
#define __IR_REMOTE_H__

//******************************************************************************
//  R E M O T E
//******************************************************************************
//
//	Manufacter:		Generic
//
//	Model:			(Not Applicable)
//
//	Notes:			32 Bit RECS80 w/Repeat
//						Normal = 92:45 start
//						Repeat = 92:22 start
//
//******************************************************************************

//******************************************************************************
//  P A Y L O A D
//******************************************************************************

/* Characterization Data:		"3"



*/

	#define	BIT_TOLERANCE					10		// +/- for Nominal value
	#define NOMINAL_TROUGH					25		// Nominal (avg) width of trough between bits
	#define NOMINAL_LOGIC0					25		// Nominal (avg) width of Logic 0
	#define NOMINAL_LOGIC1					56		// Nominal (avg) width of Logic 1

ROM gmt_IrProtocolExt IrProtocol = {

	//
	// Protocol
	//
	IR_PROTOCOL_RECS80,								// Protocol
	(CLEAR_BUFFERS|IR_CFG_RAW),	//IR_INV			// Config register (IR_CONFIG)
	0x0F19,											// Timing register (IR_TIMEBASE_MEASURESTEP)
	44,												// Period (ms) - Time between leading edges of packets

	//
	// Packets
	//	
	1,												// Num Packets
	{
		//
		// Standard Key
		//
		{
			IR_FLAG_NONE,							// No Special Processing
			(10),									// Start0 Min
			(170),									// Start0 Max
			(113 - BIT_TOLERANCE),					// Start1 Min
			(113 + BIT_TOLERANCE),					// Start1 Max
			(NOMINAL_TROUGH - BIT_TOLERANCE),		// Through Min
			(NOMINAL_TROUGH + BIT_TOLERANCE),		// Through Max
			(NOMINAL_LOGIC0 - BIT_TOLERANCE),		// Logic0 Min
			(NOMINAL_LOGIC0 + BIT_TOLERANCE), 		// Logic0 Max
			(NOMINAL_LOGIC1 - BIT_TOLERANCE), 		// Logic1 Min
			(NOMINAL_LOGIC1 + BIT_TOLERANCE),		// Logic1 Max
			20,										// Payload Length
			0x00000fff,								// Address Mask
			0,										// Address Shift
			0x000ff000,								// Data Mask
			12,										// Data Shift
			0xffff,									// Cmd Code
		}, // Standard Key

	}, // Packets

}; // IrProtocol

#ifdef IR_DEBUG_USE_LOCAL_REMOTE_TABLE
ST_IR ROM IRDebugValues[] =
{
	{ 0x01, Key0044 }, //IR_RESET
 	{ 0x02, Key0001 }, //IR_EXIT
 	{ 0x03, Key0005 }, //IR_DOWN
 	{ 0x04, Key0004 }, //IR_ENTER
 	{ 0x07, Key0002 }, //IR_RIGHT
 	{ 0x08, Key0003 }, //IR_LEFT
 	{ 0x09, Key0006 }, //IR_UP
 	{ 0x0a, Key0013 }, //IR_MENU
 	{ 0x0d, Key0041 }, //IR_HIGHLIGHT
 	{ 0x0e, Key0024 }, //IR_CHANNEL_UP
 	{ 0x0f, Key0025 }, //IR_CHANNEL_DOWN
 	{ 0x10, Key0032 }, //IR_VOLUME_DOWN
 	{ 0x11, Key0031 }, //IR_VOLUME_UP
 	{ 0x12, Key0026 }, //IR_KEYSTONE
 	{ 0x13, Key0047 }, //IR_NUMERIC
 	{ 0x14, Key0014 }, //IR_ZERO
 	{ 0x15, Key0042 }, //IR_ASTERISK
 	{ 0x16, Key0021 }, //IR_SEVEN
 	{ 0x17, Key0022 }, //IR_EIGHT
 	{ 0x18, Key0023 }, //IR_NINE
 	{ 0x19, Key0020 }, //IR_SIX
 	{ 0x1a, Key0019 }, //IR_FIVE
 	{ 0x1b, Key0018 }, //IR_FOUR
 	{ 0x1c, Key0015 }, //IR_ONE
 	{ 0x1d, Key0016 }, //IR_TWO
 	{ 0x1e, Key0017 }, //IR_THREE
 	{ 0x1f, Key0045 }, //IR_SWAP
 	{ 0x20, Key0040 }, //IR_PIP
 	{ 0x21, Key0037 }, //IR_SOURCE
 	{ 0x22, Key0027 }, //IR_POWER
 	{ 0x23, Key0035 }, //IR_AUTO
 	{ 0x24, Key0038 } //IR_POINTER
 };
#endif
	
#endif
