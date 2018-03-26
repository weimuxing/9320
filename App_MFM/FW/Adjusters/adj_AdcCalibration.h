/*
	$Workfile:   adj_AdcCalibration.h  $
	$Revision: 1.5 $
	$Date: 2012/01/06 09:20:01 $
*/
#ifndef __ADJ_ADC_CALIBRATION_H__
#define __ADJ_ADC_CALIBRATION_H__

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
// MODULE:      Adj_AdcCalibration.h
//
// USAGE:
//
//******************************************************************************

//******************************************************************************
//  G L O B A L    S T R U C T U R E
//******************************************************************************

typedef enum ADCCalibrationTypesEnum
{
   ADC_GRAPHICS,
   ADC_COMPOSITE,
   ADC_COMPONENT,
   ADC_SVIDEO,
   ADC_SCART
} ADCCalibrationTypes;

typedef enum ADCCalibrationMethod_CompositeEnum
{
   THREE_CVBS_VGA_CONNECTOR,
   THREE_CVBS_COMP_CONNECTOR,
   THREE_CVBS_COMPILE_PIN_SELECTION,
   ONE_CVBS_INPUT,
   INTERNAL_DAC_CVBS,
   NUM_ADC_METHODS_CVBS
} ADCCalibrationMethod_Composite;

typedef enum ADCCalibrationMethod_ComponentEnum
{
   EIGHT_COLOR_BAR_100,
   EIGHT_COLOR_BAR_75,
   INTERNAL_DAC_COMP,
   NUM_ADC_METHODS_COMP
} ADCCalibrationMethod_Component;

typedef enum ADCCalibrationMethod_VGAEnum
{
   PIXEL_GRAB,
   INTERNAL_DAC_VGA,
   MIN_MAX,
   INTERNAL_DAC_MIN_MAX,
   NUM_ADC_METHODS_VGA
} ADCCalibrationMethod_VGA;

typedef enum ADCCalibrationMethod_InternalDacEnum
{
   BAND_GAP,
   RESISTIVE_LADDER,
   NUM_ADC_METHODS_INTERNAL_DAC
} ADCCalibrationMethod_InternalDac;

typedef enum ADCCalibrationMethod_SCARTEnum
{
   INTERNAL_DAC_SCART,
   EIGHT_COLOR_BAR_100_SCART,
   NUM_ADC_METHODS_SCART
} ADCCalibrationMethod_SCARTEnum;

typedef enum ADCCalibrationMethod_SIFEnum
{
   INTERNAL_DAC_SIF_CALIB_OFF,
   INTERNAL_DAC_SIF_CALIB_ON,
   NUM_ADC_METHODS_SIF
} ADCCalibrationMethod_SIFEnum;

//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

void PerformAdcCalibration			(void);
void DecoderCalibration				(void);
BOOL IsPortValidForADCCalibration	(BYTE B_ADCCalibrationType);
#ifdef ADC_BANDGAP_CALIBRATION
gmt_RET_STAT AdcBandgapCalibrate(void);
gmt_RET_STAT AdcBandgapInvoke(void);
#endif
void PrintADCRegisterVal(gmt_ADC_ID B_AdcId);
#endif
//*********************************  END  **************************************
