/*
   $Workfile:   gm_UploadSRAMBlock.h  $
   $Revision: 1.3 $
   $Date: 2011/12/14 10:35:32 $
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
// MODULE:      gm_UploadSRAMBlock.h
//
//******************************************************************


typedef struct UploadTableStruct
{
   WORD ctrlAddr; //address of crl host register TABLE_ACCESS_BRIDGE_CTRL1 or TABLE_ACCESS_BRIDGE_CTRL2
   WORD ctrlMask; //bit mask of apropriate table inside ctrl register
   WORD ctrlMode[gmd_MAX_NUMBER_TABLE_TRANSFER_MODES];
   WORD ctrlAccs[gmd_MAX_NUMBER_TABLE_ACCESS_MODES];
   WORD status; //apropriate bit in status register
   WORD request; //mask in DMA_REQUEST_CTRL register
   DWORD memAddr; //address in memory space reserved for that table
   WORD tabSize; //size of table
} TTableUploadParams;


gmt_RET_STAT gm_UploadSRAMBlock(gmt_TAB_TABLE Table,  gmt_TAB_TRANSFER_MODE TransferMode,
                                gmt_TAB_ACCESS_MODE AccessMode, WORD far * const p_DestBuffer,  WORD TabOffset, WORD BlockSize);


