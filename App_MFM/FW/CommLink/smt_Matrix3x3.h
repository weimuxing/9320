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
// MODULE:      smt_matrix3x3.h
//
//******************************************************************
#ifndef SMT_MATRIX_3x3_INCLUDED
#define SMT_MATRIX_3x3_INCLUDED

#ifdef USE_SMT
//*************************************************************************
//
//*************************************************************************

typedef enum Matrix3x3_CommandEnum
{
   MATRIX_GET_VERSION=0,
   MATRIX_READ=1,
   MATRIX_WRITE=2,
   MATRIX_SETWINDOW=3
} Matrix3x3_CommandType;

typedef enum Matrix3x3_ErrorEnum
{
   Matrix3x3_OK = 0,
   Matrix3x3_ERROR = 1,
   Matrix3x3_INVALIDPARAM = 2,
} Matrix3x3_ErrorType;

/**************************************************************************
 Definition for SMT transactions handler to determine buffersize enough for this prot handler
**************************************************************************/
#if (!defined(SMT_MAX_BUF_SIZE) )
#define SMT_MAX_BUF_SIZE 1
#endif
/**************************************************************************/
//******************************************************************************
//  G L O B A L    F U N C T I O N S
//******************************************************************************
BYTE Matrix3x3_MsgHandler(MsgPacket_t *message, BYTE comm_use);
#endif
#endif
