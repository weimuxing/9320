/*
	$Workfile:   CT-90179.h  $
	$Revision: 1.1 $
	$Date: 2010/03/17 06:24:44 $
*/
#ifndef __IR_REMOTE_H__
#define __IR_REMOTE_H__

//******************************************************************************
//  R E M O T E
//******************************************************************************
//
//	Manufacter:		Toshiba
//
//	Model:			CT-90179
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

	(1) 73
	(0) 93
	(1) 45
	(0) 6
	(1) 5
	(0) 6
	(1) 6
	(0) 6
	(1) 5
	(0) 6
	(1) 6
	(0) 6
	(1) 5
	(0) 6
	(1) 6
	(0) 6
	(1) 16
	(0) 6
	(1) 6
	(0) 6
	(1) 16
	(0) 7
	(1) 16
	(0) 6
	(1) 17
	(0) 6
	(1) 5
	(0) 7
	(1) 16
	(0) 6
	(1) 17
	(0) 6
	(1) 16
	(0) 7
	(1) 16
	(0) 6
	(1) 20
	(0) 93
	(1) 22
	(0) 6

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
			IR_FLAG_REVERSE,						// Bit order is reversed
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
			0x0000FFFF,								// Address Mask
			0,										// Address Shift
			0x00FF0000,								// Data Mask
			16,										// Data Shift
			0xFFFF,									// Cmd Code
		}, // Standard Key

		//
		// Repeat Key
		//
		{
			IR_FLAG_REPEAT|IR_FLAG_REVERSE,			// Repeat key (copy previsou payload on term)
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

	}, // Packets

}; // IrProtocol

#ifdef IR_DEBUG_USE_LOCAL_REMOTE_TABLE
ST_IR ROM IRDebugValues[] =
{
	{ 0x00, Key0014 }, //IR_ZERO
 	{ 0x01, Key0015 }, //IR_ONE
 	{ 0x02, Key0016 }, //IR_TWO
 	{ 0x03, Key0017 }, //IR_THREE
 	{ 0x04, Key0018 }, //IR_FOUR
 	{ 0x05, Key0019 }, //IR_FIVE
 	{ 0x06, Key0020 }, //IR_SIX
 	{ 0x07, Key0021 }, //IR_SEVEN
 	{ 0x08, Key0022 }, //IR_EIGHT
 	{ 0x09, Key0023 }, //IR_NINE
 	{ 0x0D, Key0042 }, //IR_PIP_POSITION
 	{ 0x11, Key0049 }, //IR_PIP_ASPECTRATIO
 	{ 0x12, Key0027 }, //IR_POWER
 	{ 0x15, Key0053 }, //IR_PIP_SIZE
 	{ 0x16, Key0047 }, //IR_INFO
 	{ 0x19, Key0006 }, //IR_UP 	
 	{ 0x1D, Key0005 }, //IR_DOWN 	
 	{ 0x21, Key0004 }, //IR_OK

 	{ 0x40, Key0002 }, //IR_RIGHT 	 	
 	{ 0x42, Key0003 }, //IR_LEFT
 	{ 0x43, Key0001 }, //IR_EXIT
 	{ 0x48, Key0037 }, //IR_SOURCE
 	{ 0x49, Key0035 }, //IR_PIP_SOURCE
 	{ 0x4A, Key0038 }, //IR_FREEZE
 	{ 0x52, Key0045 }, //IR_SWAP
 	{ 0x53, Key0040 }, //IR_PIP
 	{ 0x59, Key0041 }, //IR_ASPECT
 	{ 0x5B, Key0013 }, //IR_MENU

 	{ 0xff, Key0024 }, //IR_CHANNEL_UP
 	{ 0xff, Key0025 }, //IR_CHANNEL_DOWN
 	{ 0xff, Key0031 }, //IR_VOLUME_UP
 	{ 0xff, Key0032 }, //IR_VOLUME_DOWN
 	{ 0xff, Key0044 }, //IR_ENTER_JUMP	
 	{ 0xff, Key0050 }, //IR_SCAN
 	{ 0xff, Key0051 }, //IR_MODE
 	{ 0xff, Key0052 }, //IR_SPLIT
 	{ 0xff, Key0059 }, //IR_INDEX
 	{ 0xff, Key0054 }, //IR_TTX_RED
 	{ 0xff, Key0062 }, //IR_HOLD
 	{ 0xff, Key0055 }, //IR_TTX_GREEN
 	{ 0xff, Key0060 }, //IR_SIZE
 	{ 0xff, Key0061 }, //IR_REVEAL
 	{ 0xff, Key0026 }, //IR_TTX_CC
 	{ 0xff, Key0056 }, //IR_TTX_YELLOW
 	{ 0xff, Key0057 }, //IR_TTX_CYAN
 	{ 0xff, Key0058 }  //IR_SUB
 };
#endif

#endif
