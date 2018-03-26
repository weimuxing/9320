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
// MODULE:      SMT_PortMapping.c
//
//******************************************************************

#include "CommLink/smt.h"

#include "CommLink\smt_msgfwd.h"

#ifdef USE_SMT
//*************************************************************************
// Streaming Message Transport Protocol PortID <-> Handler mapping
//*************************************************************************
// IMPORTANT!
// To add a new port handling 3 parts should be modified
// 1. Table SMT_Port2HandlerTable below
// 2. Type SMT_Port_t in SMT.h
// 3. New header file include is to be added in Smt_Port.h
SMT_PortMapping_t ROM SMT_Port2HandlerTable[  ] = // SMT_NUM_PORT_MAPPED
{
   { SMTPORT_ACC16, NULL_PTR },

#if ((FEATURE_ACM_3D == ENABLE)&& defined(SMT_ACM_3D_INCLUDED))
   { SMTPORT_ACM3, (FPT_SMT_MsgHandler)ACM3_SMT_MsgHandler },
#endif

#if 0 // 	(defined(SMT_MATRIX_3x3_INCLUDED))
   {SMTPORT_MATRIX_3x3, (FPT_SMT_MsgHandler)Matrix3x3_MsgHandler},
#endif

#if	(defined(SMT_COLOR_CONTROL_INCLUDED))
   { SMTPORT_COLOR_CONTROL, (FPT_SMT_MsgHandler)ColorControl_MsgHandler },
#endif

#if ((FEATURE_ACC_RAM_SIZE == 10) && defined(SMT_ACC_5_INCLUDED))
   { SMTPORT_ACC5, (FPT_SMT_MsgHandler)Acc5_MsgHandler },
#endif

#if	(defined(SMT_FILTERS_INCLUDED))
   { SMTPORT_FILTERS, (FPT_SMT_MsgHandler)Filters_MsgHandler },
#endif

#if 	(defined(GAMMA_INCLUDED))
   { SMTPORT_GAMMA, (FPT_SMT_MsgHandler)GammaCorr_MsgHandler },
#endif

#if 0 // 	(defined(SMT_ACC2_INCLUDED) && FEATURE_ACC2 == ENABLE)
   { SMTPORT_ACC2, (FPT_SMT_MsgHandler)Acc2_MsgHandler},
#endif
   { SMTPORT_MSG, (FPT_SMT_MsgHandler)SMT_MsgFwdHandler},

#if (FEATURE_ACC3 == ENABLE)
   { SMTPORT_ACC3,		(FPT_SMT_MsgHandler)ACC3_MsgHandler },
#endif
   {SMTPORT_MADI,		(FPT_SMT_MsgHandler)MADi_MsgHandler},
   {SMTPORT_AFM,		(FPT_SMT_MsgHandler)AFM_MsgHandler},
   #if (FEATURE_TNR == REV3)
   {SMTPORT_TNR,		(FPT_SMT_MsgHandler)TNR_MsgHandler},
	#endif
   {SMTPORT_SHARPNESS,		(FPT_SMT_MsgHandler)Sharpness_MsgHandler},
   {SMTPORT_CSC,		(FPT_SMT_MsgHandler)CSC_MsgHandler},
   {SMTPORT_MNR,		(FPT_SMT_MsgHandler)MNR_MsgHandler},
   {SMTPORT_LBC,		(FPT_SMT_MsgHandler)LBC_MsgHandler},
   {SMTPORT_LCT,		(FPT_SMT_MsgHandler)LCT_MsgHandler},
   {SMTPORT_ACT,		(FPT_SMT_MsgHandler)ACT_MsgHandler},

// end mark of the table, make all additions to table before this line
   { MAX_SMTPORT, NULL_PTR }
};

#endif
