/*
	$Workfile:   RECS80.h  $
	$Revision: 1.1 $
	$Date: 2010/03/17 06:24:43 $
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

/* Characterization Data:

	(1) 35
	(0) 91
	(1) 46
	(0) 6
	(1) 5
	(0) 6
	(1) 6
	(0) 6
	(1) 5
	(0) 6
	(1) 6
	(0) 6
	(1) 6
	(0) 5
	(1) 6
	(0) 6
	(1) 17
	(0) 5
	(1) 6
	(0) 6
	(1) 17
	(1) 17
	(0) 6
	(1) 17
	(0) 6
	(1) 5
	(0) 7
	(1) 16
	(0) 6
	(1) 17
	(0) 6
	(1) 17
	(0) 5
	(1) 18
	(0) 5

*/

	#define	BIT_TOLERANCE					3		// +/- for Nominal value
	#define NOMINAL_TROUGH					6		// Nominal (avg) width of trough between bits
	#define NOMINAL_LOGIC0					5		// Nominal (avg) width of Logic 0
	#define NOMINAL_LOGIC1					17		// Nominal (avg) width of Logic 1

ROM gmt_IrProtocolExt IrProtocol = {    
    //
    // Protocol
    //
    IR_PROTOCOL_RECS80,								// Protocol
    (CLEAR_BUFFERS|IR_INV|IR_CFG_RAW),				// Config register (IR_CONFIG)
    0x2039,											// Timing register (IR_TIMEBASE_MEASURESTEP)
    120,											// Period (ms) - Time between leading edges of packets

    //
    // Packets
    //	
    2,												// Num Packets
    {
    	//
    	// Standard Key
    	//
    	{
    		IR_FLAG_NONE,							// No Special Processing
    		(92 - BIT_TOLERANCE),					// Start0 Min
    		(92 + BIT_TOLERANCE),					// Start0 Max
    		(45 - BIT_TOLERANCE),					// Start1 Min
    		(45 + BIT_TOLERANCE),					// Start1 Max
    		(NOMINAL_TROUGH - BIT_TOLERANCE),		// Through Min
    		(NOMINAL_TROUGH + BIT_TOLERANCE),		// Through Max
    		(NOMINAL_LOGIC0 - BIT_TOLERANCE),		// Logic0 Min
    		(NOMINAL_LOGIC0 + BIT_TOLERANCE), 		// Logic0 Max
    		(NOMINAL_LOGIC1 - BIT_TOLERANCE), 		// Logic1 Min
    		(NOMINAL_LOGIC1 + BIT_TOLERANCE),		// Logic1 Max
    		32,										// Payload Length
    		0xFFFF0000,								// Address Mask
    		16,										// Address Shift
    		0x0000FF00,								// Data Mask
    		8,										// Data Shift
    		0xffff,									// Cmd Code
    	}, // Standard Key
    	//
		// Repeat Key
		//
		{
			IR_FLAG_REPEAT,							// Repeat key (copy previsou payload on term)
			(92 - BIT_TOLERANCE),					// Start0 Min
			(92 + BIT_TOLERANCE),					// Start0 Max
			(22 - BIT_TOLERANCE),					// Start1 Min
			(22 + BIT_TOLERANCE),					// Start1 Max
			(NOMINAL_TROUGH - BIT_TOLERANCE),		// Through Min
			(NOMINAL_TROUGH + BIT_TOLERANCE),		// Through Max
			(NOMINAL_LOGIC0 - BIT_TOLERANCE),		// Logic0 Min
			(NOMINAL_LOGIC0 + BIT_TOLERANCE), 		// Logic0 Max
			(NOMINAL_LOGIC1 - BIT_TOLERANCE), 		// Logic1 Min
			(NOMINAL_LOGIC1 + BIT_TOLERANCE),		// Logic1 Max
			0,										// Payload Length
			0,										// Address Mask
			0,										// Address Shift
			0,										// Data Mask
			0,										// Data Shift
			0,										// Cmd Code
		}, // Repeat Key
	}, //packets
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
