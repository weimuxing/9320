/*
            $Workfile:   gm_OsdGlobalVariables.h  $
            $Revision: 1.2 $
            $Date    :   $
*/

#ifndef __GM_OSDGLOBALVARIABLE_H__
#define __GM_OSDGLOBALVARIABLE_H__
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
// MODULE:      gmglobalvariables.h
//
// USAGE:       This module contains all the Global variables used across all
//		the modules in the library. This is the header file containing
//		the externs declarations
//
//******************************************************************************

//******************************************************************************
//  E X T E R N S
//******************************************************************************

extern ROM char gmc_OsdLibVersion[];			// OSD Library version number
extern ROM char gmc_OsdLibDate[];				// OSD Library Date

// *** DRIVER ******************************************************************

//
// Have to be initialized by application
//
extern gmt_OSD_ADJUSTERS_MODE_SWITCHERS ROM gmv_OsdAdjusterModeSwitchers[];
extern gmt_OSD_ADJUSTERS_GROUP ROM gmv_OsdAdjustersGroupList[];
extern BYTE ROM gmv_UserDefAdjusterProperty[];
extern BYTE ROM * ROM gmv_OsdAdjusterFuncSet[];
extern BYTE ROM * ROM gmv_OsdMinMaxFuncSet[];
extern BYTE ROM * ROM gmv_OsdStepFuncSet[];


extern BYTE ROM *gmv_ActionOpPtr;			// action operand pointer

//
// Have to be initialized by application
//
extern BYTE ROM gmc_NumberOfOsdThreads;

extern gmt_OSD_SM_THREAD_DESCRIPTOR ROM gmv_GlobalThreadSet[];
extern gmt_OSD_SM_THREAD_CONTEXT gmv_OsdThreadContextArray[];

extern BYTE ROM * ROM gmv_ActionOpSet[];
extern BYTE ROM * ROM gmv_ActTableSet[];


extern BYTE gmvb_OsdActiveDialog;
extern BYTE gmvb_OsdRotation;
extern gmt_OSD_TILE_POSITION gmv_OsdTilePosition[];

//
// Have to be initialized by application
//
extern BYTE ROM gmc_OsdComprTxt;
extern BYTE ROM * ROM gmv_OsdDialogs[];
extern BYTE ROM * ROM gmv_EmbeddedPointersArray[];
extern BYTE ROM * ROM gmv_GlyphTblSet[];

extern BYTE ROM *gmv_ColorMap;

extern BYTE *gmv_TempBuffer;
extern WORD gmvw_TempBufferSize;

extern BYTE gmvb_OsdLanguage;

extern BYTE *gmv_OsdTextStringDecompBuffer;

extern void (*gFp_SaveUserPref)(gmt_OSD_Adjuster_Info * AdjusterInfo);
extern void (*gFp_RetrieveUserPref)(gmt_OSD_Adjuster_Info * AdjusterInfo);

#endif
//*********************************  END  **************************************
