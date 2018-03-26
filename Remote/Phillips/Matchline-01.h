/*
	$Workfile:   Matchline-01.h  $
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
//	Model:			Matchline
//
//	Notes:			RC5
//
//
//******************************************************************************

//******************************************************************************
//  P R O T O C O L
//******************************************************************************

	#define	BIT_TOLERANCE					3
	#define NOMINAL_SINGLE					6
	#define NOMINAL_DOUBLE					12

ROM gmt_IrProtocolDef IrProtocolDef = {

	//
	// Protocol
	//
	IR_PROTOCOL_RC5,								// Protocol
	(CLEAR_BUFFERS|IR_INV|IR_CFG_RAW),				// Config register
	0x3020,											// Timing register
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
			0xffff,									// Cmd Code
			30										// Packet Burst (ms) - Time from Leading edge to last bit in packet
		}, // Standard Key
		
	}, // Packets
	
}; // IrProtocolDef
	
#endif
