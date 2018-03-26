/*
	$Workfile:   RC25106-01.h  $
	$Revision: 1.1 $
	$Date: 2010/03/17 06:24:43 $
*/
#ifndef __IR_REMOTE_H__
#define __IR_REMOTE_H__

//******************************************************************************
//  R E M O T E
//******************************************************************************
//
//	Manufacter:		Phillips
//	Model:			RC25106 (Battery Bay)
//
//	Notes:			RC5
//
//
//******************************************************************************

//******************************************************************************
//  P R O T O C O L
//******************************************************************************

	#define	BIT_TOLERANCE					2
	#define NOMINAL_SINGLE					9
	#define NOMINAL_DOUBLE					18

ROM gmt_IrProtocolExt IrProtocol = {

	//
	// Protocol
	//
	IR_PROTOCOL_RC5,								// Protocol
	(CLEAR_BUFFERS|IR_INV|IR_CFG_RAW),				// Config register
	0x2039,											// Timing register
	120,											// Period (ms) - Time between leading edges of packets

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
			0,										// Start0 Min
			0,										// Start0 Max
			0,										// Start1 Min
			0,										// Start1 Max
			0,										// Through Min
			0,										// Through Max
			(NOMINAL_SINGLE - BIT_TOLERANCE),		// Single Min
			(NOMINAL_SINGLE + BIT_TOLERANCE), 		// Single Max
			(NOMINAL_DOUBLE - BIT_TOLERANCE), 		// Double Min
			(NOMINAL_DOUBLE + BIT_TOLERANCE),		// Double Max
			24,										// Payload Length
			0x000007C0,								// Address Mask
			6,										// Address Shift
			0x0000003F,								// Data Mask
			0,										// Data Shift
			0,										// Cmd Code
		}, // Standard Key
		
	}, // Packets
	
}; // IrProtocolDef

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
 	{ 0x0C, Key0027 }, //IR_POWER 	
 	{ 0x0D, Key0001 }, //IR_EXIT 	 	
 	{ 0x10, Key0031 }, //IR_VOLUME_UP 	
 	{ 0x11, Key0032 }, //IR_VOLUME_DOWN 	
 	{ 0x20, Key0024 }, //IR_CHANNEL_UP  	
 	{ 0x21, Key0025 }, //IR_CHANNEL_DOWN  	 	
 	{ 0x22, Key0040 }, //IR_PIP 	 	
	{ 0x23, Key0037 }, //IR_SOURCE (Main Source) 	
 	{ 0x26, Key0038 }, //IR_POINTER (Freeze)	
	{ 0x3A, Key0035 }, //IR_AUTO (PIP Source)
	{ 0x4C, Key0004 }, //IR_ENTER
 	{ 0x50, Key0006 }, //IR_UP
 	{ 0x51, Key0005 }, //IR_DOWN 	 	 	
 	{ 0x52, Key0013 }, //IR_MENU 	
 	{ 0x55, Key0003 }, //IR_LEFT	
 	{ 0x56, Key0002 }, //IR_RIGHT	
  	{ 0x59, Key0041 }, //IR_HIGHLIGHT (Aspect Ratio) 	
	{ 0x68, Key0045 }, //IR_SWAP

 	{ 0xff, Key0026 }, //IR_KEYSTONE (No Key)
 	{ 0xff, Key0047 }, //IR_NUMERIC (No Key)
 	{ 0xff, Key0042 }, //IR_ASTERISK (No Key) 	 	
	{ 0xff, Key0044 }, //IR_RESET (No Key) 	
 };
#endif
	
#endif
