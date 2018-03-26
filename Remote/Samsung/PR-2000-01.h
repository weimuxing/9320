/*
	$Workfile:   PR-2000-01.h  $
	$Revision: 1.1 $
	$Date: 2010/03/17 06:24:44 $
*/
#ifndef __IR_REMOTE_H__
#define __IR_REMOTE_H__

//******************************************************************************
//  R E M O T E
//******************************************************************************
//
//	Manufacter:		Samsung
//	Model:			PR-2000
//
//	Notes:			
//
//
//******************************************************************************

//******************************************************************************
//  P R O T O C O L
//******************************************************************************

	#define	BIT_TOLERANCE					3
	#define NOMINAL_TROUGH					6
	#define NOMINAL_LOGIC0					4
	#define NOMINAL_LOGIC1					16

ROM gmt_IrProtocolExt IrProtocol= {

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
		// Standard Key (RECS80-32) - 32 Payload Bits
		//
		{
			IR_FLAG_NONE,							// Start Pulse embedded in Payload
			(45 - BIT_TOLERANCE),					// Start0 Min
			(45 + BIT_TOLERANCE),					// Start0 Max
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
			0xE0E0,									// Cmd Code
		}, // Standard Key

		//
		// Power Key (RECS80-16:20) - 16 Address Bits + Start Pulse + 20 Payload Bits
		//
		{
			IR_FLAG_MULT_START|IR_FLAG_POWER,		// Start Pulse embedded in Payload
			(45 - BIT_TOLERANCE),					// Start0 Min
			(45 + BIT_TOLERANCE),					// Start0 Max
			(45 - BIT_TOLERANCE),					// Start1 Min
			(45 + BIT_TOLERANCE),					// Start1 Max
			(NOMINAL_TROUGH - BIT_TOLERANCE),		// Through Min
			(NOMINAL_TROUGH + BIT_TOLERANCE),		// Through Max
			(NOMINAL_LOGIC0 - BIT_TOLERANCE),		// Logic0 Min
			(NOMINAL_LOGIC0 + BIT_TOLERANCE), 		// Logic0 Max
			(NOMINAL_LOGIC1 - BIT_TOLERANCE), 		// Logic1 Min
			(NOMINAL_LOGIC1 + BIT_TOLERANCE),		// Logic1 Max
			36,										// Payload Length (16 + 1 + 20)
			0x0000FFFF,								// Address Mask
			0,										// Address Shift
			0x000FFC00,								// Data Mask
			10,										// Data Shift
			0x0800,									// Cmd Code
		}, // Standard Key	

	}, // Packets
	
}; // IrProtocolDef

#ifdef IR_DEBUG_USE_LOCAL_REMOTE_TABLE
ST_IR ROM IRDebugValues[] =
{
 	{ 0x04, Key0045 }, //IR_SWAP
 	{ 0x06, Key0006 }, //IR_UP
 	{ 0x08, Key0025 }, //IR_CHANNEL_DOWN
 	{ 0x10, Key0018 }, //IR_FOUR 	
 	{ 0x16, Key0004 }, //IR_ENTER
 	{ 0x20, Key0015 }, //IR_ONE 	
 	{ 0x30, Key0021 }, //IR_SEVEN 	
 	{ 0x40, Key0027 }, //IR_POWER 	
 	{ 0x46, Key0002 }, //IR_RIGHT
 	{ 0x48, Key0024 }, //IR_CHANNEL_UP 	
 	{ 0x50, Key0020 }, //IR_SIX 	
  	{ 0x58, Key0013 }, //IR_MENU
 	{ 0x60, Key0017 }, //IR_THREE  	
 	{ 0x70, Key0023 }, //IR_NINE  	
	{ 0x76, Key0038 }, //IR_POINTER (Freeze) 	
 	{ 0x7C, Key0035 }, //IR_AUTO (PIP Source) 	 	
 	{ 0x80, Key0040 }, //IR_PIP 	
 	{ 0x86, Key0005 }, //IR_DOWN 	 	
 	{ 0x88, Key0014 }, //IR_ZERO 	 	
 	{ 0x90, Key0019 }, //IR_FIVE 	
 	{ 0xA0, Key0016 }, //IR_TWO 	
 	{ 0xA6, Key0003 }, //IR_LEFT
 	{ 0xB0, Key0022 }, //IR_EIGHT 	
 	{ 0xB4, Key0041 }, //IR_HIGHLIGHT (Aspect) 	
 	{ 0xC8, Key0037 }, //IR_SOURCE 	
 	{ 0xD0, Key0032 }, //IR_VOLUME_DOWN 	
 	{ 0xE0, Key0031 }, //IR_VOLUME_UP 	
 	{ 0xF8, Key0001 }, //IR_EXIT
 	
	{ 0xff, Key0042 }, //IR_ASTERISK (No Key)
	{ 0xff, Key0047 }, //IR_NUMERIC (No Key)
	{ 0xff, Key0026 }, //IR_KEYSTONE (No Key)
	{ 0xff, Key0044 }, //IR_RESET (No Key)
 };
#endif


#endif