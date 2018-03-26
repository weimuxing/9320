/*
   $Workfile:   DELLAccTbl.c  $
   $Revision: 1.6 $
   $Date: 2012/02/09 18:25:12 $
*/
#define __DELLACCTBL_C__
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
// MODULE:      DELLAccTbl.c
//
// USAGE:      This file contains table for Automatic Contrast Control (ACC)
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#include "System/all.h"

//120803 Edward add for seperate color table by panel
#if (DEFAULT_PANEL == LG_WUXGA_LM240WU9_SLA1)
#include "..\..\Panel\WUxga\LG\LM240WU9_SLA1\LM240WU9_ACC3Tbl.h"
#elif (DEFAULT_PANEL == LG_WQHD_LM270WQ1)
#include "..\..\Panel\WQHD\LG\LM270WQ1\LM270WQ1_ACC3Tbl.h"
#elif (DEFAULT_PANEL == LG_WHD_LM290WW1_SSA1)
	#include "..\..\Panel\WHD\LG\LM290WW1\LM290WW1_ACC3Tbl.h"
#elif ((DEFAULT_PANEL == LG_WQXGA_LM300WQ5_SLA1)||(DEFAULT_PANEL == LG_WQXGA_LM300WQ6_SLA1))
#include "..\..\Panel\WQXGA\LG\LM300WQ5_SLA1\LM300WQ5_ACC3Tbl.h"
#elif (DEFAULT_PANEL == ZDT_4K2K_QFHD)
#include "..\..\Panel\4K2K\ZDT_4K2K\ZDT_4K2K_QFHD_ACC3Tbl.h"

#elif (DEFAULT_PANEL == ZDT_4K2K_CMI)
#include "..\..\Panel\4K2K\ZDT_4K2K_CMI\ZDT_4K2K_CMI_ACC3Tbl.h"

#elif (DEFAULT_PANEL == SAMSUNG_WXGA_LTI260AP01)
#include "..\..\Panel\Wxga\Samsung\LTI260AP01\LTI260AP01_ACC3Tbl.h"
#elif (DEFAULT_PANEL == SAMSUNG_1080P_LTI460HM03)
#include "..\..\Panel\1080P\Samsung\LTI460HM03\LTI460HM03_ACC3Tbl.h"

#elif (DEFAULT_PANEL == CMO_WSXGA_M220Z1_L09)
#include "..\..\Panel\Wsxga\CMO\M220Z1_L09\M220Z1_L09_ACC3Tbl.h"
#elif (DEFAULT_PANEL == CMO_WSXGA_M220Z1_L01)
#include "..\..\Panel\Wsxga\CMO\M220Z1_L01\M220Z1_L01_ACC3Tbl.h"

#else //(DEFAULT_PANEL == LG_1080P_LC320EUN)
#include "..\..\Panel\WUxga\LG\LM240WU9_SLA1\LM240WU9_ACC3Tbl.h"

//#else
//#pragma message "!!!!No ACC table include"
#endif


