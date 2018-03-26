/*
   $Workfile:   smt_ACC5bin.H  $
   $Revision: 1.3 $
   $Date: 2011/12/15 03:52:11 $
*/
//******************************************************************
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
//================================================================
//
// MODULE:      smt_Acc5bin.h
//
//******************************************************************
#if(!defined( SMT_ACC_5_INCLUDED) && (FEATURE_ACC == ENABLE) )
#define SMT_ACC_5_INCLUDED

#ifdef USE_SMT
#if FEATURE_ACC_RAM_SIZE == 10


//*************************************************************************
//
//*************************************************************************

typedef struct
{
   WORD Shift:16;
   WORD  StoredDataByte:8;
   WORD  DbgCntr:8;
   WORD StoredFlag:1;
   WORD CurrentCommand:3;

}t_Acc5Context;

/**************************************************************************
 Definition for SMT transactions handler to determine buffersize enough for this prot handler
**************************************************************************/
#if !defined(SMT_MAX_BUF_SIZE)
#define SMT_MAX_BUF_SIZE sizeof(t_Acc5Context)
#elif SMT_MAX_BUF_SIZE < sizeof(t_Acc5Context)
#undef SMT_MAX_BUF_SIZE
#define SMT_MAX_BUF_SIZE sizeof(t_Acc5Context)
#endif
/**************************************************************************/


typedef enum Acc5_CommandEnum
{
   Acc5_GET_VERSION = 0,
   Acc5_WRITE =1,
   Acc5_READ =2,
   Acc5_READ_BIN=3,
   Acc5_UPDATE_BIN=4,
   Acc5_ENABLE=5,
   Acc5_DISABLE=6,
   Acc5_READ_OUTPUT_CURVE=7,
   Acc5_NumberOfCommands=8
} Acc5_CommandType;

typedef enum Acc5_ErrorEnum
{
   Acc5_OK = 0,
   Acc5_ERROR = 1,
   Acc5_INVALIDPARAM = 2,
} Acc5_ErrorType;

#pragma option -b-
// enum is represented as a byte
typedef enum YCLink_enum
{
   ACC_DISABLE=0,
   ACC_EN_INISDE=1,
   ACC_EN_OUTSIDE=2,
   ACC_EN_ANYWERE=3
}t_YCLink ;
#pragma option -b.

#pragma pack(push)
#pragma pack(1)
typedef struct ACC5_DATA_WR_STRUCT
{
   WORD id;				//< command  id (16bit unsigned) >,
   BYTE bins;				//< Number of Bins (byte) >,	// 3 Bins should enable ACC3, and 5 bins - enable ACC5
   WORD low_end;			// <ACC_BIN0_RLIMIT (word)>,	for ACC3 <Lower End (word)>
   WORD mid_start;			//< ACC_BIN1_LLIMIT (word)>,	for ACC3 <Mid Start (word)>,
   WORD mid_end;			//< ACC_BIN1_RLIMIT				for ACC3 <Mid End (word)>, (word)>
   WORD upper_start;		//< ACC_BIN2_LLIMIT (word)>,	for ACC3 <Upper Start (word)>
   WORD bin2_r;			//< ACC_BIN2_RLIMIT (word)>,	// ACC5 only (for Acc3 must be ignored)
   WORD bin3_l;			//< ACC_BIN3_LLIMIT (word)>,	// ACC5 only (for Acc3 must be ignored)
   WORD bin3_r	;			//< ACC_BIN3_RLIMIT (word)>,	// ACC5 only (for Acc3 must be ignored)
   WORD bin4_l;			//< ACC_BIN4_LLIMIT (word)>,	// ACC5 only (for Acc3 must be ignored)
   t_YCLink YCLink;		//YCLink <byte>,// 0 - disable; 1 - Enable inside; 2- Enable outside; 3-Enable inside and outside
   BYTE YCLmaxdelta;		//<YCLMaxDelta (byte)>,
   BYTE SCLtreshold;		//<SCLThreshold (byte)>,
   BYTE YCLscale;			//<YCLScale (byte)>,		// ScaleA | ScaleB << 4
   WORD Ymin;				//<YMin (word)>,
   WORD Ymax;				//<YMax (word)>,
   BYTE curves;			//<Number of Transfer Curves (byte)>,
   WORD curvsize;			//<Transfer Curves Data Size (word)>	// For Torino Acc3 = 3 * 514 bytes; Acc5 = 5 * 258 bytes
   BYTE data[1];			//<Transfer curves Data (variable)>
}ACC5_WRITE_TYPE;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct ACC5_DATA_RD_STRUCT
{
   BYTE id;				//< command  or error id (16bit unsigned) >,
   BYTE bins;				//< Number of Bins (byte) >,	// 3 Bins should enable ACC3, and 5 bins - enable ACC5
   WORD low_end;			// <ACC_BIN0_RLIMIT (word)>,	for ACC3 <Lower End (word)>
   WORD mid_start;			//< ACC_BIN1_LLIMIT (word)>,	for ACC3 <Mid Start (word)>,
   WORD mid_end;			//< ACC_BIN1_RLIMIT				for ACC3 <Mid End (word)>, (word)>
   WORD upper_start;		//< ACC_BIN2_LLIMIT (word)>,	for ACC3 <Upper Start (word)>
   WORD bin2_r;			//< ACC_BIN2_RLIMIT (word)>,	// ACC5 only (for Acc3 must be ignored)
   WORD bin3_l;			//< ACC_BIN3_LLIMIT (word)>,	// ACC5 only (for Acc3 must be ignored)
   WORD bin3_r	;			//< ACC_BIN3_RLIMIT (word)>,	// ACC5 only (for Acc3 must be ignored)
   WORD bin4_l;			//< ACC_BIN4_LLIMIT (word)>,	// ACC5 only (for Acc3 must be ignored)
   t_YCLink YCLink;		//YCLink <byte>,// 0 - disable; 1 - Enable inside; 2- Enable outside; 3-Enable inside and outside
   BYTE YCLmaxdelta;		//<YCLMaxDelta (byte)>,
   BYTE SCLtreshold;		//<SCLThreshold (byte)>,
   BYTE YCLscale;			//<YCLScale (byte)>,		// ScaleA | ScaleB << 4
   WORD Ymin;				//<YMin (word)>,
   WORD Ymax;				//<YMax (word)>,
   BYTE curves;			//<Number of Transfer Curves (byte)>,
   WORD curvsize;			//<Transfer Curves Data Size (word)>	// For Torino Acc3 = 3 * 514 bytes; Acc5 = 5 * 258 bytes
   BYTE data[1];			//<Transfer curves Data (variable)>
}ACC5_READ_TYPE;
#pragma pack(pop)


#pragma pack(push)
#pragma pack(1)
typedef struct ACC5_BIN_STRUCT
{
   WORD id;//< command id(16bit unsigned) >,
   BYTE bins;				//< Number of Bins (byte) >,	// 3 Bins should enable ACC3, and 5 bins - enable ACC5
   WORD low_end;			// <ACC_BIN0_RLIMIT (word)>,	for ACC3 <Lower End (word)>
   WORD mid_start;			//< ACC_BIN1_LLIMIT (word)>,	for ACC3 <Mid Start (word)>,
   WORD mid_end;			//< ACC_BIN1_RLIMIT				for ACC3 <Mid End (word)>, (word)>
   WORD upper_start;		//< ACC_BIN2_LLIMIT (word)>,	for ACC3 <Upper Start (word)>
   WORD bin2_r;			//< ACC_BIN2_RLIMIT (word)>,	// ACC5 only (for Acc3 must be ignored)
   WORD bin3_l;			//< ACC_BIN3_LLIMIT (word)>,	// ACC5 only (for Acc3 must be ignored)
   WORD bin3_r	;			//< ACC_BIN3_RLIMIT (word)>,	// ACC5 only (for Acc3 must be ignored)
   WORD bin4_l;			//< ACC_BIN4_LLIMIT (word)>,	// ACC5 only (for Acc3 must be ignored)
}ACC5_BIN_TYPE;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct ACC5_READ_OUTPUT_CURVE_STRUCT
{
   BYTE id;				//< command  or error id (16bit unsigned) >,
   WORD size;				//< Data Size (word)>,		// Number of Output curve bytes
   BYTE data[1];			//<Output Curve Data (byte)>
}tACC5_READ_OUTPUT_CURVE;
#pragma pack(pop)

//******************************************************************************
//  G L O B A L    F U N C T I O N S
//******************************************************************************
BYTE Acc5_MsgHandler(MsgPacket_t *message, BYTE comm_use);
#endif
#endif
#endif
