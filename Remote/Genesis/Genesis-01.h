/*
	$Workfile:   Genesis-01.h  $
	$Revision: 1.1 $
	$Date: 2010/03/17 06:24:43 $
*/
#ifndef __IR_REMOTE_H__
#define __IR_REMOTE_H__

//******************************************************************************
//  R E M O T E
//******************************************************************************
//
//	Manufacter:		Genesis Microchip
//	Model:			Unknown
//
//	Notes:			Standard Genesis Remote
//
//
//******************************************************************************

//******************************************************************************
//  P A Y L O A D
//******************************************************************************

/* Characterization Data:

	(1) 109			Idle (High)
	(0) 27			Start Lo
	(1) 26			Start Hi
	(0) 12			Trough
	(1) 11			Addr(7) '0'
	(0) 12
	(1) 29			Addr(6) '1'
	(0) 12
	(1) 11			Addr(5) '0'
	(0) 12
	(1) 29			Addr(4) '1'
	(0) 12
	(1) 11			Addr(3) '0'
	(0) 12
	(1) 29			Addr(2) '1'
	(0) 12
	(1) 29			Addr(1) '1'
	(0) 12
	(1) 11			Addr(0) '0'		Address = 01010110b = 56h
	(0) 13
	(1) 11			Data(7) '0'
	(0) 12
	(1) 11			Data(6) '0'
	(0) 13
	(1) 28			Data(5) '1'
	(0) 12
	(1) 11			Data(4) '0'
	(0) 13
	(1) 28			Data(3) '1'
	(0) 12
	(1) 11			Data(2) '0'
	(0) 13
	(1) 11			Data(1) '0'
	(0) 12
	(1) 11			Data(0) '0'		Data = 00101000b = 28h >> 2 = 10
	(0) 12

*/

	#define	BIT_TOLERANCE					3
	#define	NOMINAL_START_WIDTH				26
	#define NOMINAL_TROUGH					11
	#define NOMINAL_LOGIC0					11
	#define NOMINAL_LOGIC1					28

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
	1,												// Num Packets
	{
		//
		// Standard Key
		//
		{
			IR_FLAG_NONE,							// No Special Processing
			(NOMINAL_START_WIDTH - BIT_TOLERANCE),	// Start0 Min
			(NOMINAL_START_WIDTH + BIT_TOLERANCE),	// Start0 Max
			(NOMINAL_START_WIDTH - BIT_TOLERANCE),	// Start1 Min
			(NOMINAL_START_WIDTH + BIT_TOLERANCE),	// Start1 Max
			(NOMINAL_TROUGH - BIT_TOLERANCE),		// Through Min
			(NOMINAL_TROUGH + BIT_TOLERANCE),		// Through Max
			(NOMINAL_LOGIC0 - BIT_TOLERANCE),		// Logic0 Min
			(NOMINAL_LOGIC0 + BIT_TOLERANCE), 		// Logic0 Max
			(NOMINAL_LOGIC1 - BIT_TOLERANCE), 		// Logic1 Min
			(NOMINAL_LOGIC1 + BIT_TOLERANCE),		// Logic1 Max
			16,										// Payload Length
			0x0000FF00,								// Address Mask
			8,										// Address Shift
			0x000000FF,								// Data Mask
			2,										// Data Shift
			0x0056,									// Cmd Code
		}, // Standard Key
		
	}, // Packets
	
}; // IrProtocol

#endif
