/*
	$Workfile:   debug_measure.h  $
	$Revision: 1.3 $
	$Date: 2011/12/13 10:32:45 $
*/
#ifndef __DEBUG_MEASURE_H__

#define __DEBUG_MEASURE_H__
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
// MODULE:      debug_measure.h
//
// USAGE:       This file contains declaration for time measurement functions.
//
//				MEASURE_BEGIN(marker) - start a measurement with 'marker'
//				MEASURE_END(marker) - end the measurement with 'marker'
//				The measurement is placed in debug_TickBuf[] in the following
//				format:
//					marker, startTime, endTime
//
//				ElaseTime = (endTime - startTime) x SystemTick Resolution
//
//				SystemTick Resolution is 1 mses by default, or 100usec
//				if __MEASURE_IN_USEC__ is turned on
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#define __DEBUG_MEASURE__		// enable time measurement macro
//
// The following macro defines the max number of measurements allowed.
// After the maximum number of measurement have been taken, no more measurement
// is allowed until the buffer is clear.
// Note: each measurement need 10 bytes of RAM
//
#define SZ_TICK_BUF			2 //20

//
// *** do not use the following switch unless you are
// *** sure there is no conflit on hardware
// The macro enable the measurement by toggle the CS0 pin o that a more prcise
// measurement may be taken  with an oscilloscpe.  The CS0 is configure as
// output to drive the pin.
//
// If enabled, two pulses will be generated on reset so that overhead of the
// macro may be takede for reference.
// Use MEASURE_PIN_HI() to set pin high (initial state after reset)
// Use MEASURE_PIN_LO() to set pin lo
//
#define __MEASURE_BY_PROBE__

//******************************************************************************
//  G L O B A L    F U N C T I O N S
//******************************************************************************

//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************

//******************************************************************************
//  M A C R O   D E F I N I T I O N
//******************************************************************************
//
// The default measurement resolution is 1ms per tick.  To measure with
// higher resolution, enable the following define to change the resolution
// to 100 usec per tick.
// Notes:
// (1) increasing resolution cause timer interrupt occures more frequently
//	which may increase the measured time.
// (2) timer0 is used for high resolution (100usec/tick) measurement.
// (3) To enable the high resolution measurement,
//		(a) enable the __MEASURE_IN_USEC and
//		(b)	execute MEASURE_INIT() once before any measurement start.
//
//#define __MEASURE_IN_USEC__

//
// Structure holds each measurement information
//
typedef struct
{
   WORD  id;				// identity (marker) of a measurement
   DWORD begin;			// tick count from which measurement begins
   DWORD end;				// tick count from which measurement ends
} ST_MEASURE_STAMP;

//******************************************************************************
//  I N T E R C A C E
//******************************************************************************
#ifndef __DEBUG_MEASURE_C__

#ifndef __DEBUG_MEASURE__
#define MEASURE_INIT()
#define MEASURE_CLEAR()
#define MEASURE_BEGIN(id)
#define MEASURE_END(id)
#define MEASURE_PIN_HI()
#define MEASURE_PIN_LO()
#else
//
// MEARURE_INIT()
//
// The macro initialize the variable for measure setup.
// In the case of measurement in 100 us resolution, the timer0 interupt
// handler is installed.
//
#define MEASURE_INIT()													\
{																		\
	debug_Measure_Init();												\
};

//
// MEASURE_CLEAR()
//
// The macro clear the measurement buffer.
// Once the maximum number of measurement is reached, no more measurement
// is allowed until the buffer is clear.
//
#define MEASURE_CLEAR()													\
{																		\
	for (TickBufIdx=SZ_TICK_BUF; TickBufIdx; )							\
	{																	\
		debug_TickBuf[--TickBufIdx].id = 0;								\
		debug_TickBuf[TickBufIdx].begin = 0;							\
		debug_TickBuf[TickBufIdx].end = 0;								\
	}																	\
};

//
// MEASURE_BEGIN(marker)
//
// The macro places the current system tick value to the next available
// slot of the measurement buffer.  The slot is designated with the
// specified 'marker', which will be used to match for 'end' measurement.
//
// marker - any integer number to identify the measurement ponit
//
#define MEASURE_BEGIN(marker)											\
{																		\
	if (TickBufIdx < SZ_TICK_BUF)										\
	{																	\
		debug_TickBuf[TickBufIdx].id = (WORD)marker;					\
		debug_TickBuf[TickBufIdx++].begin = debug_GetTickCount();		\
	}																	\
   else  \
     TickBufIdx = 0; \
};

//
// MEASURE_END(marker)
//
// The macro places the current system tick value to mark end
// measurement of a entry in the buffer where the same 'marker'
// value is found.
//
// marker - use the same marker to which the measurement was begin
//
#define MEASURE_END(marker)												\
{																		\
	register WORD thisIdx;												\
	DWORD thisTick = debug_GetTickCount();								\
	for (thisIdx=TickBufIdx; thisIdx; )									\
	{																	\
		if (debug_TickBuf[--thisIdx].id == (WORD)marker)				\
		{																\
			debug_TickBuf[thisIdx].end = thisTick;						\
			break;														\
		}																\
	}																	\
};


DWORD debug_Measure_GetAverage( WORD Id );
DWORD debug_Measure_GetMaximum( WORD Id );

extern WORD TickBufIdx;
extern ST_MEASURE_STAMP debug_TickBuf[SZ_TICK_BUF];

extern DWORD debug_GetTickCount(void);
extern void debug_Measure_Init(void);

#ifndef __MEASURE_BY_PROBE__
#define MEASURE_PIN_HI()
#define MEASURE_PIN_LO()
#else
#define MEASURE_PIN_HI()											\
{																	\
	SetGPIO_OpHi( FLT_IND_LED_GPIO );								\
};

#define MEASURE_PIN_LO()											\
{																	\
	SetGPIO_OpLo( FLT_IND_LED_GPIO );								\
};
#endif	// __MEASURE_BY_PROBE__
#endif	// __DEBUG_MEASURE__
#endif	// __DEBUG_MEASURE_C__
#endif	// #ifndef __DEBUG_MEASURE_H__

//
// Measurement with Oscillscope on CS0
//
#ifdef __MEASURE_BY_PROBE__
//**********************************
//  Macros for programming GPIO
//**********************************
#define SetGPIO_Enable( gpio )                                                               \
   gm_WriteRegWord(                                                                         \
                     ( GPIO_0_ENABLE+(gpio/16)*10 ),                                         \
                     ( gm_ReadRegWord( GPIO_0_ENABLE+(gpio/16)*10 ) | (1 << (gpio % 16) ) ) \
                   )

#define SetGPIO_Disable( gpio )                                                               \
   gm_WriteRegWord(                                                                         \
                     ( GPIO_0_ENABLE+(gpio/16)*10 ),                                         \
                     ( gm_ReadRegWord( GPIO_0_ENABLE+(gpio/16)*10 ) & ~(1 << (gpio % 16) ) ) \
                   )

#define SetGPIO_DirToOut( gpio )                                                              \
   gm_WriteRegWord(                                                                           \
                     ( GPIO_0_DIRCTRL+(gpio/16)*10 ),                                         \
                     ( gm_ReadRegWord( GPIO_0_DIRCTRL+(gpio/16)*10 ) | (1 << (gpio % 16) ) )  \
                   )

#define SetGPIO_OpToPullUp( gpio )                                                            \
   gm_WriteRegWord(                                                                           \
                     ( GPIO_0_OPENDRAIN_EN+(gpio/16)*10 ),                                     \
                     ( gm_ReadRegWord( GPIO_0_OPENDRAIN_EN+(gpio/16)*10 ) & ~(1 << (gpio % 16) ) ) \
                   )

#define SetGPIO_OpLo( gpio )                                                                  \
   gm_WriteRegWord(                                                                           \
                     ( GPOUTPUT0+(gpio/16)*10 ),                                              \
                     ( gm_ReadRegWord( GPOUTPUT0+(gpio/16)*10 ) & ~(1 << (gpio % 16) ) )      \
                   )

#define SetGPIO_OpHi( gpio )                                                                  \
   gm_WriteRegWord(                                                                           \
                     ( GPOUTPUT0+(gpio/16)*10 ),                                              \
                     ( gm_ReadRegWord( GPOUTPUT0+(gpio/16)*10 ) | (1 << (gpio % 16) ) )       \
                   )

#define FLT_IND_LED_GPIO			2		// GPIO_2 (same as CS0n)
#endif

