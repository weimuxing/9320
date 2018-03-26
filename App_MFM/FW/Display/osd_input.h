/*
	$Workfile:   osd_input.h  $
	$Revision: 1.4 $
	$Date: 2011/12/15 03:12:03 $
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
// MODULE:      osd_input.h
//
// USAGE:	Header file for OSD input event.
//
//******************************************************************************


//******************************************************************************
//  T Y P E    D E F I N I T I O N S
//******************************************************************************

#define NUMBER_OF_INPUT_SOURCE		6		// MAX number of input key queues

// Input Source.
typedef enum InputSourceEnum
{
   ADC_KEYPAD,
   IR_REMOTE,
   GPIO_KEYPAD,
   NUM_OF_INPUT_SOURCE,
} INPUT_SOURCE;

// State of the keypad and IR buttons.
typedef enum KeyStateEnum
{
   WAITING_COMMAND,               		// waiting for key press.
   RELEASING,							// Waiting for key release.
   AUTO_REPEAT_START,    				// Starting autorepeat.
   AUTO_REPEAT          				// Autorepeat in progress.
} KEY_STATE;

typedef BYTE KEY_INDEX;
typedef WORD GPIO_KEY_VALUE;
typedef WORD KEY_VALUE;

#ifdef __PARADIGM__
#pragma pack(push)
#pragma pack(1)
#endif //__PARADIGM__

typedef struct ST_KEYPAD_STATEStruct
{
   KEY_INDEX	B_iButton;
   BYTE	B_iTimer;
} ST_KEYPAD_STATE;

#ifdef __PARADIGM__
#pragma warn -bbf
#endif //__PARADIGM__


typedef struct ST_TRANSLATION_TABLE_ENTRYStruct
{
   ST_KEYPAD_STATE	St_PreviousKS;
   ST_KEYPAD_STATE	St_CurrentKS;
   WORD	W_iEvent;
   BYTE	B_iAutorepeatStartDelay;
   BYTE	W_iAutorepeatDelay : 6;
   BYTE	W_iAutorepeatEnable : 1;
   BYTE	W_iTableEnd : 1;
} ST_TRANSLATION_TABLE_ENTRY;

#ifdef __PARADIGM__
#pragma warn -bbf
#endif //__PARADIGM__

typedef struct ST_GPIOStruct
{
   BYTE	Port;
   BYTE	Bit;
} ST_GPIO;

#ifdef __PARADIGM__
#pragma warn -bbf
#endif //__PARADIGM__


typedef struct ST_GPIO_TRANSLATIONStruct
{
   GPIO_KEY_VALUE	KeyValue;
   KEY_INDEX	KeyIndex;
} ST_GPIO_TRANSLATION;

#ifdef __PARADIGM__
#pragma warn -bbf
#endif //__PARADIGM__


typedef struct ST_ADCStruct
{
   BYTE	Code;
   KEY_INDEX	KeyIndex;
} ST_ADC;

#ifdef __PARADIGM__
#pragma warn -bbf
#endif //__PARADIGM__


typedef struct ST_IRStruct
{
   BYTE	Code;
   KEY_INDEX	KeyIndex;
} ST_IR;

#ifdef __PARADIGM__
#pragma warn -bbf
#endif //__PARADIGM__

// WB4
typedef struct ST_KEYPAD_DIFINITIONStruct
{
   BYTE		B_ChanNumber;
   BYTE ROM*	Bp_KeyValues;
   BYTE		B_KeysNumber;
   BYTE		(* GetKeyValue)(BYTE);
   BYTE		(* InitKeypadDriver)(BYTE);
   BYTE		B_CodeLen;
   BYTE		B_Delta;
} WBK_KEYPAD_DEFINITION;

#ifdef __PARADIGM__
#pragma warn -bbf
#endif //__PARADIGM__

#ifdef __PARADIGM__
#pragma pack(pop)
#endif //__PARADIGM__

typedef struct ST_PORTS_MIXED_TRANSLATIONStruct
{
   BYTE	Codes[NUMBER_OF_INPUT_SOURCE];
   KEY_INDEX	KeyIndex;
} ST_PORTS_MIXED_TRANSLATION;


//End block defined in libs

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************
#define RELEASE_IR_TMO			28		// 28 * 10ms = 280ms
#define RELEASE_ADC_TMO			5		// 5 * 10ms = 50ms
#define	ADC_DELTA 				8		// ADC margin +/- 4
#define OSD_TIMER_STOP			0		// OSD Timer stop
#define OSD_TIMER_START			1		// OSD Timer start


//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************


//******************************************************************************
//  M A C R O   D E F I N I T I O N
//******************************************************************************

//******************************************************************************
//  G L O B A L    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

void OsdEventHandler			(void);
void OSDKeyInit(void);
#if (WB_MAJOR == 0x0004)
BYTE EnqueueOsdKey	(KEY_INDEX B_DataKey);
BYTE DequeueOsdKey	(KEY_INDEX *Bp_DataKey);
#else
BYTE EnqueueOsdKey	(void);
BYTE DequeueOsdKey	(void);
#endif
void FlushOsdKeyQueue(void);
KEY_INDEX TranslateCodeToKey (BYTE  ROM *Bp_KeyValue, BYTE B_KeysNumber,
                              BYTE B_CodeLen, BYTE B_Delta, WORD W_Value);
void ClearOsdKeyQueue(void);

//******************************************************************************
// D R I V E R     F U N C T I O N S
//******************************************************************************
BYTE GetADCValue( BYTE B_Index );
BYTE InitADCDriver( BYTE B_Index );
BYTE GetIRValue( BYTE B_Index );
BYTE InitIRDriver( BYTE B_Index );
BYTE GetGPIOValue( BYTE B_Index );
BYTE InitGPIODriver( BYTE B_Index );
BYTE GetOSDQueueLen(void); //KevinL 20101124

