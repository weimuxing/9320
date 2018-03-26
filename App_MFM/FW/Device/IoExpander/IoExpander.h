/*
	$Workfile:   IoExpander.h  $
	$Revision: 1.3 $
	$Date: 2011/12/14 10:35:31 $
*/

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
// MODULE:      IoExpander.h
//
// USAGE:
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************



//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************


//******************************************************************************
//  E X T E R N A L     F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

//******************************************************************************
//  E X T E R N A L     V A R I A L L E
//******************************************************************************

//******************************************************************************
//  S T A T I C    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

//******************************************************************************
//  C O D E
//******************************************************************************

#ifndef IOEXPANDER_H
#define IOEXPANDER_H

#define IO_EXP_PCF8575		0x48
#define IO_EXP_PCF8575_4C	0x4C

#define 	IO_EXP_P00  BIT0
#define 	IO_EXP_P01  BIT1
#define 	IO_EXP_P02  BIT2
#define 	IO_EXP_P03  BIT3
#define 	IO_EXP_P04  BIT4
#define 	IO_EXP_P05  BIT5
#define 	IO_EXP_P06  BIT6
#define 	IO_EXP_P07  BIT7

#define 	IO_EXP_P10  BIT8
#define 	IO_EXP_P11  BIT9
#define 	IO_EXP_P12  BIT10
#define 	IO_EXP_P13  BIT11
#define 	IO_EXP_P14  BIT12
#define 	IO_EXP_P15  BIT13
#define 	IO_EXP_P16  BIT14
#define 	IO_EXP_P17  BIT15



void IoExpanderClearBitWord(BYTE B_Addr, WORD W_Bit);
void IoExpanderSetBitWord(BYTE B_Addr, WORD W_Bit);
WORD IoExpanderGetBitWord(BYTE B_Addr, WORD W_Bit);

#endif