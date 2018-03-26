/*
	$Workfile:   ad9888.h  $
	$Revision: 1.3 $
	$Date: 2011/12/13 10:32:43 $
*/

#ifndef __AD9888_H__
#define __AD9888_H__

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
// MODULE:	ad9883.h
//
// USAGE:	Header file contains function prototypes and definitions
//			related to ADC AD9888 driver
//
//******************************************************************************


//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************

// Device address
#define AD9888_DEV_ID	0x9A

// Register address defs
#define AD9888_PLLDIVM             0x01   // PLL divider ratio MSBs
#define AD9888_PLLDIVL             0x02   // PLL divider ration LSBs
#define AD9888_VCOCTRL             0x03   // VCO range and Charge pump current
#define AD9888_PHASE               0x04   // Phase control
#define AD9888_CLPLACE             0x05   // Clamp Placement
#define AD9888_CLDUR               0x06   // Clamp Duration
#define AD9888_GAINR               0x08   // Red gain control
#define AD9888_GAING               0x09   // Green gain control
#define AD9888_GAINB               0x0A   // Blue gain control
#define AD9888_OFFSETR             0x0B   // Red offset control
#define AD9888_OFFSETG             0x0C   // Green offset control
#define AD9888_OFFSETB             0x0D   // Blue offset control
#define AD9888_SYNC_CONTROL        0x0E   // Sync Control 
#define AD9888_CLMPCOAST_CONTROL   0x0F   // Clamp+Coast control 
#define AD9888_SOG_THRESHOLD       0x10   // Sync On Green Threshold
#define AD9888_SYNC_SEP_THRSHOLD   0x11   // Sync Separator Threshold
#define AD9888_PRE_COAST           0x12   // Pre-Coast
#define AD9888_POST_COAST          0x13   // Post-Coast
#define AD9888_SYNC_DETECT         0x14   // Sync Detect
#define AD9888_OUTPUT_CONTROL      0x15   // Output control

//Limits
#define AD9888_MAX_GAIN   255
#define AD9888_MAX_OFFSET 63
#define AD9888_MAX_PHASE  31

#define RED_CHANNEL        0
#define GREEN_CHANNEL      1
#define BLUE_CHANNEL       2

//******************************************************************************
//	G L O B A L    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

extern void adc_Init(void);
extern void adc_SetPhase(BYTE B_phaseVal);
extern void adc_SetGain(BYTE B_channel, BYTE B_contrast);
extern void adc_SetOffset(BYTE B_channel, BYTE B_blackLevel);
extern void adc_SetClampLevel(BYTE B_level);
extern void adc_SetCoast(BYTE B_preCoastCount,BYTE B_postCoastCount);

extern void adc_SetClock(WORD W_hTotal,WORD W_vTotal,
                            BYTE B_hPolarity,BYTE B_refreshRate);

BYTE adc_GetSyncStatus(void);
extern void adc_Write08(BYTE B_regAddr,BYTE B_value);
extern BYTE adc_Read08(BYTE B_regAddr);

extern void adc_Setup (WORD InputVTotal, BYTE InterlacedMode);

extern void adc_PowerUp(void);
extern void adc_PowerDown(void);


#endif //__AD9888_H__
