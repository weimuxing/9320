/*
	$Workfile:   LCD-TV-01.h  $
	$Revision: 1.1 $
	$Date: 2010/03/17 06:24:43 $
*/
#ifndef __IR_REMOTE_H__
#define __IR_REMOTE_H__

//******************************************************************************
//  R E M O T E
//******************************************************************************
//
//	Manufacter:		NEC
//	Model:			LCD-TV
//
//	Notes:			32 Bit RECS80 w/Repeat (Normal 92:45 start) (Repeat 92:22 start)
//
//
//******************************************************************************

//******************************************************************************
//  P A Y L O A D
//******************************************************************************

/* Characterization Data:


*/

	#define	BIT_TOLERANCE					4
	#define NOMINAL_TROUGH					6
	#define NOMINAL_LOGIC0					4
	#define NOMINAL_LOGIC1					16

ROM gmt_IrProtocolExt IrProtocol = {

	//
	// Protocol
	//
	IR_PROTOCOL_RECS80,								// Protocol
	(CLEAR_BUFFERS|IR_INV|IR_CFG_RAW),				// Config register
	0x2039,											// Timing register
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
			0x000000FF,								// Data Mask
			0,										// Data Shift
			0x60DF,									// Cmd Code
		}, // Standard Key

		//
		// Repeat Key
		//
		{
			IR_FLAG_REPEAT,							// Repeat key
			(92 - BIT_TOLERANCE),					// Start0 Min
			(92 + BIT_TOLERANCE),					// Start0 Max
			(24 - BIT_TOLERANCE),					// Start1 Min
			(24 + BIT_TOLERANCE),					// Start1 Max
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
	{ 0x02, Key0035 }, //IR_AUTO (PIP Source)
 	{ 0x08, Key0016 }, //IR_TWO
	{ 0x0A, Key0045 }, //IR_SWAP
 	{ 0x10, Key0022 }, //IR_EIGHT
 	{ 0x12, Key0002 }, //IR_RIGHT
 	{ 0x20, Key0040 }, //IR_PIP 	
 	{ 0x22, Key0006 }, //IR_UP
 	{ 0x2A, Key0031 }, //IR_VOLUME_UP
 	{ 0x30, Key0019 }, //IR_FIVE
 	{ 0x38, Key0003 }, //IR_LEFT
	{ 0x3A, Key0004 }, //IR_ENTER
	{ 0x40, Key0037 }, //IR_SOURCE (Main Source)
 	{ 0x42, Key0032 }, //IR_VOLUME_DOWN
 	{ 0x48, Key0038 }, //IR_POINTER (Freeze)
 	{ 0x50, Key0025 }, //IR_CHANNEL_DOWN 
 	{ 0x68, Key0024 }, //IR_CHANNEL_UP 
 	{ 0x70, Key0027 }, //IR_POWER
 	{ 0x88, Key0017 }, //IR_THREE 	
 	{ 0x8A, Key0001 }, //IR_EXIT 	
 	{ 0x90, Key0023 }, //IR_NINE 	  	
  	{ 0xA0, Key0041 }, //IR_HIGHLIGHT (Aspect Ratio)
 	{ 0xB0, Key0020 }, //IR_SIX 	
 	{ 0xB2, Key0014 }, //IR_ZERO 	
 	{ 0xB8, Key0005 }, //IR_DOWN 	 	
 	{ 0xc8, Key0015 }, //IR_ONE 	
 	{ 0xD0, Key0021 }, //IR_SEVEN
 	{ 0xE0, Key0013 }, //IR_MENU
 	{ 0xf0, Key0018 }, //IR_FOUR
 	
 	{ 0xff, Key0026 }, //IR_KEYSTONE (No Key)
 	{ 0xff, Key0047 }, //IR_NUMERIC (No Key)
 	{ 0xff, Key0042 }, //IR_ASTERISK (No Key) 	 	
	{ 0xff, Key0044 }, //IR_RESET (No Key) 	
 };
#endif
	
#endif
