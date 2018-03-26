/*
	$Workfile:   AccTbl.c  $
	$Revision: 1.6 $
	$Date: 2012/10/16 10:00:30 $
*/
#define __ACC_TBL_C__
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
// MODULE:	AccTbl.c
//
// USAGE:	This file contains table for Active Contrast Control (ACC)
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "StdLibInc\gm_ChipFeature.h"
#include "stdLibInc\gm_Global.h"
#include "StdLibReplace\Acc.h"

#if FEATURE_ACC3
//******************************************************************************
//  E X T E R N S
//******************************************************************************

extern VQ_ACC_Parameters_t ROM AccVivid;
extern VQ_ACC_Parameters_t ROM AccCinema;
extern VQ_ACC_Parameters_t ROM AccNature;
extern VQ_ACC_Parameters_t ROM AccSport;
extern VQ_ACC_Parameters_t ROM AccLinear;
#if 1//def DELL_U2410F//120209 Edward copy from 2410
extern VQ_ACC_Parameters_t ROM AccMultimediaGraphic;
extern VQ_ACC_Parameters_t ROM AccMovieGraphic;
extern VQ_ACC_Parameters_t ROM AccGameGraphic;

extern VQ_ACC_Parameters_t ROM AccMultimediaVideo;
extern VQ_ACC_Parameters_t ROM AccMovieVideo;
extern VQ_ACC_Parameters_t ROM AccGameVideo;
extern VQ_ACC_Parameters_t ROM AccNatureVideo;

extern VQ_ACC_Parameters_t ROM AccMovieVideo_Comp;
extern VQ_ACC_Parameters_t ROM AccGameVideo_Comp;
extern VQ_ACC_Parameters_t ROM AccNatureVideo_Comp;
extern VQ_ACC_Parameters_t ROM AccxvYCCVideo_Comp;  //zuguang 20090319
#endif


//***************************************************************************
//  C O D E
//***************************************************************************

VQ_ACC_Parameters_t ROM * ROM Sta_AccSchemes[] =
{
   &AccVivid,
   &AccCinema,
   &AccNature,
   &AccSport,
};

#if 1//def DELL_U2410F//120209 Edward copy from 2410
//120620 Edward update
VQ_ACC_Parameters_t ROM * ROM Sta_AccSchemes_Graphic[] =
{
	NULL_PTR,				//Graphics_STANDARD
	&AccMultimediaGraphic,	//Graphics_MULTIMEDIA
	&AccGameGraphic,		//Graphics_GAME
	&AccMovieGraphic,		//Graphics_MOVIE
	NULL_PTR,				//Graphics_TEXT
	NULL_PTR,				//Graphics_COLOR_TEMP
	NULL_PTR,				//Graphics_COLOR_SPACE
	NULL_PTR,				//Graphics_CUSTOM_COLOR		
};

VQ_ACC_Parameters_t ROM * ROM Sta_AccSchemes_Video[] =
{
	NULL_PTR,				//Graphics_STANDARD
	&AccMultimediaVideo,	//Graphics_MULTIMEDIA
	&AccGameVideo,			//Graphics_GAME
	&AccMovieVideo,			//Graphics_MOVIE
	NULL_PTR,				//Graphics_TEXT
	NULL_PTR,				//Graphics_COLOR_TEMP
	NULL_PTR,				//Graphics_COLOR_SPACE
	NULL_PTR,				//Graphics_CUSTOM_COLOR	
};
VQ_ACC_Parameters_t ROM * ROM Sta_AccSchemes_Video_Comp[] =
{
	&AccMovieVideo_Comp,
	&AccGameVideo_Comp,
	&AccNatureVideo_Comp,
	&AccxvYCCVideo_Comp,  //zuguang 20090319
};
#endif//end if DELL_U2410F

#endif



