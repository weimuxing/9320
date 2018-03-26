/*
   $Workfile:   smt_Ports.h  $
   $Revision: 1.8 $
   $Date: 2011/12/15 06:13:06 $
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
// MODULE:      smt_Ports.h
//
// *****************************************************************

#ifndef __SMT_PORTS_H__
#define __SMT_PORTS_H__

/*******************************************************
*		 Port handlers header files					   *
*******************************************************/
#include "CommLink/smt_acm3.h"
#include "CommLink/smt_ColorControl.h"
#include "CommLink/smt_matrix3x3.h"
#include "CommLink/smt_Acc5bin.h"
#include "CommLink/smt_Filters.h"
#include "CommLink/smt_GammaCorrection.h"
#include "CommLink/smt_Acc3.h"
#include "CommLink/smt_MADi.h"
#include "CommLink/smt_AFM.h"
#include "CommLink/smt_TNR.h"
#include "CommLink/smt_MNR.h"
#include "CommLink/smt_CSC.h"
#include "CommLink/smt_ACC3.h"
#include "CommLink/smt_Sharpness.h"
#include "CommLink/smt_LBC_LCT.h"


/*******************************************************/

/**************************************************************************
 This definition  determine context buffersize for SMT transactions if it wasn't define in port header files
**************************************************************************/
#if !defined(SMT_MAX_BUF_SIZE)
#define SMT_MAX_BUF_SIZE 1
#endif
/**************************************************************************/



//*************************************************************************
// Important.
// This enum must be the same for all series of chips to keep consistency
//*************************************************************************
typedef enum SMT_Port
{
   SMTPORT_ACC16 = 0,	// ACC 16 dedicated SMT Port ID
   SMTPORT_ACM3 = 1,	// ACM3 dedicated SMT Port ID
   SMTPORT_ACC5 = 2,
   SMTPORT_MATRIX_3x3 = 6,
   SMTPORT_COLOR_CONTROL = 7,
   SMTPORT_MSG = 8,	//generic message port id
   SMTPORT_GAMMA = 9,
   SMTPORT_FILTERS = 10, // TODO: SMT protocol and Filter protocol recommend different port ID for this feature. Check and FIx
   SMTPORT_ACC2 = 12,
   SMTPORT_ACC3 = 15,
   SMTPORT_MADI = 18,
   SMTPORT_AFM = 19,
   SMTPORT_TNR = 20,
   SMTPORT_SHARPNESS = 21,
   SMTPORT_CSC = 24,
   SMTPORT_MNR = 25,
   SMTPORT_LBC = 27,
   SMTPORT_LCT = 28,
   SMTPORT_ACT = 29,
   MAX_SMTPORT
} SMT_Port_t;


typedef struct SMT_PortMapping
{
   SMT_Port_t	PortID;
   FPT_SMT_MsgHandler Fp_SMT_MsgHandler;
} SMT_PortMapping_t;

// PortID <-> Handler mapping
extern SMT_PortMapping_t ROM SMT_Port2HandlerTable[];
#endif

