/*
	$Workfile:   24LC32.h  $
	$Revision: 1.3 $
	$Date: 2011/12/13 10:32:43 $
*/
#ifndef __24LC32_H__
#define __24LC32_H__
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
// MODULE:  	24LC32.h
//
// USAGE :  	Standard low level NVRAM read/write function prototypes
//
//******************************************************************************

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************


#define NVRAM_DEVICE		0xA2        // NVRAM device address
//120430 Neil for 24C512

#define NVRAM_PAGE_SIZE		128//32			// Page write size

//******************************************************************************
//	E X T E R N A L    V A R I A B L E S
//******************************************************************************

#ifndef __NVRAM_C__
extern BYTE gB_NVRamError;			// NVram error bit indicator
#endif

//******************************************************************************
//	G L O B A L    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

gmt_RET_STAT NVRam_CheckDevice		(WORD W_RegAddr);
BYTE 	 	 NVRam_ReadByte			(WORD W_RegAddr);
gmt_RET_STAT NVRam_WriteByte		(WORD W_RegAddr, BYTE B_Data);
gmt_RET_STAT NVRam_ReadBuffer		(WORD W_RegAddr, BYTE *Bp_Buf, WORD W_Length);
gmt_RET_STAT NVRam_WriteBuffer		(WORD W_RegAddr, BYTE *Bp_Buf, WORD W_Length);

#ifdef SAVING_DATA_INTO_NVRAM
WORD 		 NVRam_ReadWORD		(WORD W_RegAddr);
gmt_RET_STAT NVRam_WriteWORD(WORD W_RegAddr, WORD B_Data);
gmt_RET_STAT  NVRam_WriteBuffer_DUM_Gamma(WORD W_RegAddr, BYTE far *Bp_Buf, WORD W_Length);
void NVRam_WriteBuffer_Clear_NVRAM(WORD W_RegAddr,WORD W_Length);

#endif


#endif
//**********************************  END  *************************************

