/*
    $Workfile:   soft_I2C.H  $
    $Revision: 1.3 $
    $Date: 2011/12/15 03:52:11 $
*/

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
// MODULE:      soft_i2c.h
//
// USAGE:       This module contains api's for software i2c
//
//******************************************************************************

#if 1//ndef __DLPI2C_H__
#define __DLPI2C_H__

#include "system\all.h"


//******************************************************************************
//  G L O B A L    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
#define I2C_DONT_CARE			(WORD)0xFFFF
/*
typedef enum
{
	I2C_BUS1	=1,
	I2C_BUS2,
	I2C_BUS3,
};
*/
extern BYTE	g_is_I2C_error;



#ifdef SOFT_I2C_FOR_EDID
//-jhbae 030121
//void I2C_Write_Byte(BYTE bus, BYTE chip_id, WORD address, BYTE cData);
//void I2C_Write_Word(BYTE bus, BYTE chip_id, WORD address, WORD wData);
//end
extern BYTE far I2C_Burst_Read_Char(BYTE bus, BYTE chip_id, WORD address, BYTE far* pData, WORD wNumOfBytes);
extern void far I2C_Burst_Write_Char(BYTE bus, BYTE chip_id, WORD address, BYTE far* pData, WORD wNumOfBytes );
BYTE I2C_Burst_Read_Chartest(void);
//gmt_RET_STAT gm_utlWriteI2C(BYTE i2cRing, BYTE deviceID, BYTE *subAddr, BYTE subAddrLen, BYTE *devTxData, BYTE txDataLen);
//gmt_RET_STAT gm_utlReadI2C(BYTE i2cRing, BYTE deviceID, BYTE *subAddr, BYTE subAddrLen, BYTE *devRxData, BYTE rxDataLen);
extern void  I2C_BUS1_START(void);
extern void  I2C_BUS1_SEND_ACK(void)	;
extern void  I2C_BUS1_STOP(void);
extern void Clock_Low(void);
extern void Clock_High(void);
extern void Data_Low(void);
extern void Data_High(void);
extern void I2C_Send_Data (void);
#endif
#endif //#ifndef __DLPI2C_H__
//*********************************  END  **************************************
