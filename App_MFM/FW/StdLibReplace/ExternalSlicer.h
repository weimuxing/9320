/*
	$Workfile:   ExternalSlicer.h  $
	$Revision: 1.3 $
	$Date: 2011/12/13 10:32:44 $
*/

#ifndef __EXTERNAL_SLICER_H__
#define __EXTERNAL_SLICER_H__

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
//=============================================================
//
// MODULE	:	ExternalSlicer.h
//
// USAGE	:	This module contains the External Slicer global defines (replace gm_Vbi.h) defaults
//
//******************************************************************************

#define EXT_SZ_VBI_CACHE_BUF		128
#define EXT_NBR_VBI_XFER_BLOCK		(EXT_SZ_VBI_CACHE_BUF/64)		// where 64 is the size of 1 burst
#define EXT_XFER_BLOCK_ADJUST       ((128/EXT_SZ_VBI_CACHE_BUF)-1)	//((SZ_SLICER_FIFO/SZ_VBI_CACHE_BUF)-1)

extern WORD W_NumOfPackets;

void EnableExternalSlicer(void);
void DisableExternalSlicer(void);

#endif __EXTERNAL_SLICER_H__
//*********************************  END  **************************************

