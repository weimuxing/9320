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
// MODULE:      smt_ColorControl.h
//
//******************************************************************
#ifndef SMT_COLOR_CONTROL_INCLUDED
#define SMT_COLOR_CONTROL_INCLUDED


#ifdef USE_SMT
//*************************************************************************
//
//*************************************************************************

#define ColorControl_MAX_VALUE 0xFFFF
#define ColorControl_MIN_VALUE 0
typedef enum ColorControl_CommandEnum
{
   ColorControl_GET_VERSION = 0,
   ColorControl_GET_STATUS =1,
   ColorControl_COLOR_SPACE_READ =2,
   ColorControl_BRIGHTNESS_READ=3,
   ColorControl_BRIGTNESS_WRITE=4,
   ColorControl_CONTRAST_READ=5,
   ColorControl_CONTRAST_WRITE=6,
   ColorControl_HUE_READ=7,
   ColorControl_HUE_WRITE=8,
   ColorControl_SATURATION_READ=9,
   ColorControl_SATURATION_WRITE=10,
   ColorControl_RED_GAIN_READ=11,
   ColorControl_RED_GAIN_WRITE=12,
   ColorControl_GREEN_GAIN_READ=13,
   ColorControl_GREEN_GAIN_WRITE=14,
   ColorControl_BLUE_GAIN_READ=15,
   ColorControl_BLUE_GAIN_WRITE=16,
   ColorControl_NumberOfCommands=17
} ColorControl_CommandType;

typedef enum ColorControl_ErrorEnum
{
   ColorControl_OK = 0,
   ColorControl_ERROR = 1,
   ColorControl_INVALIDPARAM = 2,
} ColorControl_ErrorType;

typedef struct ScalingDataStruct
{
   SDWORD sDW_InVal;
   SDWORD sDW_InMax;
   SDWORD sDW_InMin;
   SDWORD sDW_OutVal;
   SDWORD sDW_OutMax;
   SDWORD sDW_OutMin;
}ScalingDataType;

typedef enum DirectionEnum
{
   ValRead,
   ValWrite
}ValTarget;

/**************************************************************************
 Definition for SMT transactions handler to determine buffersize enough for this prot handler
**************************************************************************/
#if  !defined(SMT_MAX_BUF_SIZE)
#define SMT_MAX_BUF_SIZE 1
#endif
/**************************************************************************/

//******************************************************************************
//  G L O B A L    F U N C T I O N S
//******************************************************************************
BYTE ColorControl_MsgHandler(MsgPacket_t *message, BYTE comm_use);
#endif
#endif

