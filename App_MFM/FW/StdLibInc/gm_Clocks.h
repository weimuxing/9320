/*
	$Workfile:   gm_Clocks.h  $
	$Revision: 1.26 $
	$Date: 2012/04/20 05:56:34 $
*/
#ifndef __GM_CLOCKS_H__
#define __GM_CLOCKS_H__
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
// MODULE:     	gm_Clocks.h
//
// USAGE:		Header file for all Standard Library functions to be
// 				accessed from the Application.
//
//******************************************************************************

//******************************************************************************
//  E X T E R N S
//******************************************************************************

extern DWORD 				D_OCMCLKFreq;						// Hz


//******************************************************************************
//  E N U M S
//******************************************************************************

//
// System Clocks
//
typedef enum
{
	CLK_MC,					// Memory Controller Clock
	CLK_OCM,				// On Chip Micro Clock
	CLK_IFM,				// Input Format Measurement Clock
	CLK_LPM,
	CLK_ODP,				// ODP Clock
	CLK_AVS,				// Audio Visual Clock
	CLK_SCLK,				// Sample Clock (Digital Logic after ADC)
	CLK_SD,					// Standard Def Clock
	CLK_HD,					// High Def CLock
	CLK_IMP,				// Input Main Clock
	CLK_IPP,				// Input PIP Clock
	CLK_ADC_A,				// ADC A Clock
	CLK_ADC_B,				// ADC B Clock
	CLK_ADC_C,				// ADC C Clock
	CLK_ACLK,				// AClk
	CLK_SDDS = CLK_ACLK,	// SDDS Clk (AClk)
	CLK_DCLK,				// DClk
	CLK_DDDS = CLK_DCLK,	// DDDS Clk (DClk)
	CLK_ECLK,				// EClk
	CLK_EDDS = CLK_ECLK,	// EDDS Clk (EClk)
	CLK_DVIA,
	CLK_DVIB,
	CLK_HDMI,
	CLK_VDDS1,
	CLK_VDDS2,
    CLK_DPRX_VCLK1,
    CLK_DPRX_VCLK2,
	CLK_DPTX_VCLK,
    CLK_DPRX_LTCLK,
    CLK_DPRX_AUXCLK,
	#if 0 //(CHIP_ID == STDP93xx)	//SWENG_PREWORK	        
	CLK_DPRX1_AUXCLK=CLK_DPRX_AUXCLK,
	CLK_DPRX2_AUXCLK,
	#endif
    CLK_DPTX_AUXCLK,
	CLK_LVTX_IN,
   CLK_ODP_CAPTURE,
} gmt_CLK;

	typedef enum
	{
		SEL_CLK_SDDS1, 	// 0000
		SEL_CLK_SDDS2,	// 0001
		SEL_CLK_DDDS,	// 0010
		SEL_CLK_AUDDS1,	// 0011
		SEL_CLK_AUDDS2,	// 0100
		SEL_CLK_AUDDS3,	// 0101
		SEL_CLK_AUDDS4,	// 0110
		SEL_CLK_VDDS1,	// 0111
		SEL_CLK_VDDS2,	// 1000
		MAX_CLK_READ_SEL,
	} gmt_CLK_READ_SEL;


//
// Register CLOCK_CONFIG1 bit definition
//
#define PD_CTRL_EDDS1_EN	BIT7
#define PD_CTRL_EDDS2_EN	BIT8
#define PD_CTRL_SDDS1_EN	BIT11
#define PD_CTRL_DDDS1_EN	BIT15
#define PD_CTRL_AVDDS1_EN	BIT3	//BIT19
#define PD_CTRL_AVDDS2_EN	BIT4	//BIT20
#define PD_CTRL_AVDDS3_EN	BIT5	//BIT21
#define PD_CTRL_AVDDS4_EN	BIT6	//BIT22
#define PD_CTRL_AVDDS5_EN	BIT7	//BIT23
#define PD_CTRL_AVDDS6_EN	BIT8	//BIT24


//
// MC clock source selection
//
typedef enum
{
	MC_SEL_TCLK,
   	MC_SEL_GND,
	MC_SEL_RESERVED,
	MC_SEL_FS432_SYNTH_CLK1,
} gmt_MC_SEL;

#define MC_CLK_SEL_TCLK					(0)					// "00"
#define MC_CLK_SEL_GND					(BIT0)				// "01"
#define MC_CLK_SEL_RESERVED			(BIT1)
#define MC_CLK_SEL_FS432_SYNTH_CLK1	(BIT0 | BIT1)
#define OCM_CLK_SEL_TCLK				(0)					// "00"
#define OCM_CLK_SEL_TCLK_DIV			(BIT2)				// "01"
#define OCM_CLK_SEL_FS432_SYNTH_CLK3	(BIT3)				// "10" Normal


//
// OCM clock source selection
//
// The following definitions define the register values to configure the clock generator
typedef enum
{
   OCM_SEL_TCLK,     // Use TCLK
   OCM_SEL_TCLK_DIV, // Use TCLK/DIV
   OCM_SEL_FS432_SYNTH_CLK3,     
} gmt_OCM_SEL;

//
// IFM clock source selection
//
typedef enum
{
	IFM_SEL_TCLK,						// Normal
   	IFM_SEL_GND,
	IFM_SEL_FS432_SYNTH_CLK3_DIV2,
} gmt_IFM_SEL;

#define IFM_CLK_SEL_TCLK				(0)					// "00" Normal
#define IFM_CLK_SEL_GND					(BIT4)				// "01"
#define IFM_CLK_SEL_FS432_SYNTH_CLK3_DIV2	(BIT5)


//
// LPM clock source selection
//
typedef enum
{
	LPM_SEL_TCLK,
	LPM_SEL_OCM_CLK,
	LPM_SEL_OCM_CLK_DIV2,
	LPM_SEL_OCM_CLK_DIV4,
} gmt_LPM_SEL;

#define LPM_CLK_SEL_TCLK	 			(0) 				// "00"
#define LPM_CLK_SEL_OCM_CLK				(BIT6)				// "01"
#define LPM_CLK_SEL_OCM_CLK_DIV2 		(BIT7)				// "10"
#define LPM_CLK_SEL_OCM_CLK_DIV4		(BIT6|BIT7)			// "11"


//
// ODP clock source selection
//
typedef enum
{
	ODP_SEL_TCLK,		
   	ODP_SEL_GND,
   	ODP_SEL_DCLK,						// Normal
	ODP_SEL_IMP_CLK,
	ODP_SEL_FS432_SYNTH_CLK2,
	ODP_SEL_ODP_PLL_CLK,
} gmt_ODP_SEL;

#define ODP_CLK_SEL_TCLK				(0)					// "000"
#define ODP_CLK_SEL_GND					(BIT8)				// "001"
#define ODP_CLK_SEL_DCLK				(BIT9)				// "010" Normal
#define ODP_CLK_SEL_IMP_CLK				(BIT8|BIT9)			// "011"
#define ODP_CLK_SEL_FS432_SYNTH_CLK2	(BIT10)				// "100"
#define ODP_CLK_SEL_ODP_PLL_CLK			(BIT10|BIT8)		// "101"


//
// LVTX_IN clock source selection
//
typedef enum
{
	LVTX_IN_SEL_TCLK,
	LVTX_IN_SEL_GND,
	LVTX_IN_SEL_DDDS,
	LVTX_IN_SEL_IMP_CLK,
	LVTX_IN_SEL_FS432_SYNTH_CLK2,
} gmt_LVTX_IN_SEL;

#define LVTX_IN_CLK_SEL_TCLK				(0)				// "000"
#define LVTX_IN_CLK_SEL_GND					(BIT11)			// "001"
#define LVTX_IN_CLK_SEL_DDDS				(BIT12)			// "010"
#define LVTX_IN_CLK_SEL_IMP_CLK				(BIT11|BIT12)	// "011"
#define LVTX_IN_CLK_SEL_FS432_SYNTH_CLK2	(BIT13)			// "100"

//
// ODP capture clock source selection
//
typedef enum
{
   ODP_CAPTURE_SEL_GND,
   ODP_CAPTURE_SEL_DCLK,         // Normal      
	ODP_CAPTURE_SEL_IMP_CLK,   
   ODP_CAPTURE_SEL_SYNTH2_CLK,		      
} gmt_ODP_CAPTURE_SEL;

#define ODP_CAPTURE_CLK_SEL_GND           (0)            // "00"
#define ODP_CAPTURE_CLK_SEL_DCLK          (BIT14)        // "01" Normal
#define ODP_CAPTURE_CLK_SEL_IMP_CLK       (BIT15)        // "10" 
#define ODP_CAPTURE_CLK_SEL_SYNTH2_CLK    (BIT14|BIT15)  // "11"

//
// AVS clock source selection
//
typedef enum
{
	AVS_SEL_TCLK,
	AVS_SEL_GND,
   	AVS_SEL_CLK_PIN,					// Normal
   	AVS_SEL_CLK_INVERTED,
   	AVS_SEL_CLK_HDMI,
} gmt_AVS_SEL;

#define AVS_CLK_SEL_TCLK				(0)					// "00"
#define AVS_CLK_SEL_GND					(BIT10)				// "01"
#define AVS_CLK_SEL_CLK_PIN				(BIT11)				// "10" Normal
#define AVS_CLK_SEL_CLK_INVERTED		(BIT10|BIT11)		// "10"
#define AVS_CLK_SEL_HDMI_CLK			(BIT12)				// "100"



//
// DVIA/DVIB clock source selection
//
	typedef enum
	{
		DVI_VCLK_SEL_TCLK,
		DVI_VCLK_SEL_GND,
		DVI_VCLK_SEL_VDDS1_CLK,
		DVI_VCLK_SEL_VDDS2_CLK,
		DVI_VCLK_SEL_EXT_CLK,
	} gmt_DVI_VCLK_SEL;
	#define DVIA_VCLK_SEL_TCLK			(0)
	#define DVIA_VCLK_SEL_GND			(BIT0)
	#define DVIA_VCLK_SEL_VDDS1_CLK		(BIT1)
	#define DVIA_VCLK_SEL_VDDS2_CLK		(BIT0|BIT1)
	#define DVIA_VCLK_SEL_EXT_CLK		(BIT2)
	#define DVIB_VCLK_SEL_TCLK			(0)
	#define DVIB_VCLK_SEL_GND			(BIT4)
	#define DVIB_VCLK_SEL_VDDS1_CLK 	(BIT5)
	#define DVIB_VCLK_SEL_VDDS2_CLK 	(BIT4|BIT5)
	#define DVIB_VCLK_SEL_EXT_CLK		(BIT6)


	typedef enum
	{
		HDMI_VCLK_SEL_TCLK,
		HDMI_VCLK_SEL_GND,
		HDMI_VCLK_SEL_VDDS1_CLK,
		HDMI_VCLK_SEL_VDDS2_CLK,
		HDMI_VCLK_SEL_EXT_CLK,
	} gmt_HDMI_VCLK_SEL;
#define HDMI_VCLK_SEL_TCLK			(0)
#define HDMI_VCLK_SEL_GND			(BIT0)
#define HDMI_VCLK_SEL_VDDS1_CLK 	(BIT1)
#define HDMI_VCLK_SEL_VDDS2_CLK 	(BIT0|BIT1)
#define HDMI_VCLK_SEL_EXT_CLK		(BIT2)


//
// Register CLOCK_CONFIG2 bit definition
//

//
// Sample clock source selection
//
typedef enum
{
	SCLK_SEL_TCLK,
	SCLK_SEL_GND,
   	SCLK_SEL_ACLK,						// Normal
   	SCLK_SEL_ACLK_DVI2,					
} gmt_SCLK_SEL;

#define SCLK_CLK_SEL_TCLK				(0)					// "00"
#define SCLK_CLK_SEL_GND				(BIT0)				// "01"
#define SCLK_CLK_SEL_ACLK				(BIT1)				// "10" Normal
#define SCLK_CLK_SEL_ACLK_DVI2			(BIT0|BIT1)			// "11"

//
// SD clock source selection
//
typedef enum
{
	SD_CLK_SEL_TCLK,
	SD_CLK_SEL_GND,
   	SD_CLK_SEL_ECLK_DIV8,				// Normal
} gmt_SD_CLK_SEL;

#define SDCLK_CLK_SEL_TCLK				(0)					// "00"
#define SDCLK_CLK_SEL_GND				(BIT2)				// "01"
#define SDCLK_CLK_SEL_ECLK_DIV8			(BIT3)				// "10" Normal

//
// HD clock source selection
//
typedef enum
{
	HD_CLK_SEL_TCLK,
	HD_CLK_SEL_GND,
   	HD_CLK_SEL_SCLK,					// Normal (Graphics)
   	HD_CLK_SEL_SCLK_DIV2,				// Normal (720p/1080i)
   	HD_CLK_SEL_SCLK_DIV4,				// Normal (480p)
   	HD_CLK_SEL_SCLK_DIV8,				// 
} gmt_HD_CLK_SEL;


#define HDCLK_CLK_SEL_TCLK				(0)					// "000"
#define HDCLK_CLK_SEL_GND				(BIT2)				// "001"
#define HDCLK_CLK_SEL_SCLK				(BIT3)				// "010" Normal
#define HDCLK_CLK_SEL_SCLK_DIV2			(BIT2|BIT3)			// "011"
#define HDCLK_CLK_SEL_SCLK_DIV4			(BIT4)				// "100"
#define HDCLK_CLK_SEL_SCLK_DIV8			(BIT4|BIT2)			// "101"
//
// IMP clock source selection
//
typedef enum
{
	IMP_CLK_SEL_TCLK,
	IMP_CLK_SEL_GND,
   	IMP_CLK_SEL_HD_G_CLK,
	IMP_CLK_SEL_ACLK,
	IMP_CLK_SEL_SCLK,
	IMP_CLK_SEL_ADC_OUT_CLK,
   	IMP_CLK_SEL_IPCLK0,
   	IMP_CLK_SEL_IPCLK1,
	IMP_CLK_SEL_VDDS1_CLK,
	IMP_CLK_SEL_VDDS2_CLK,
	IMP_CLK_SEL_HDMI_VCLK,
	IMP_CLK_SEL_HDMI_VCLK_OUT,
	IMP_CLK_SEL_DPRX1_CLK,
	IMP_CLK_SEL_DPRX2_CLK,
	IMP_CLK_SEL_DVI1_OUT_CLK,
	IMP_CLK_SEL_DVI2_OUT_CLK,
} gmt_IMP_CLK_SEL;

#define IMPCLK_CLK_SEL_TCLK				(0)						// "0000"
#define IMPCLK_CLK_SEL_GND				(BIT6)					// "0001"
#define IMPCLK_CLK_SEL_HD_CLK			(BIT7)					// "0010"
#define IMPCLK_CLK_SEL_ACLK				(BIT6|BIT7)				// "0011"
#define IMPCLK_CLK_SEL_SCLK				(BIT8)					// "0100"
#define IMPCLK_CLK_SEL_ADC_OUT_CLK		(BIT8|BIT6)				// "0101"
#define IMPCLK_CLK_SEL_IPCLK0			(BIT8|BIT7)				// "0110"
#define IMPCLK_CLK_SEL_IPCLK1			(BIT8|BIT7|BIT6)		// "0111"
#define IMPCLK_CLK_SEL_VDDS1_CLK		(BIT9)					// "1000"
#define IMPCLK_CLK_SEL_VDDS2_CLK		(BIT9|BIT6)				// "1001"
#define IMPCLK_CLK_SEL_HDMI_VCLK		(BIT9|BIT7)				// "1010"
#define IMPCLK_CLK_SEL_HDMI_OUT_VCLK	(BIT9|BIT7|BIT6)		// "1011"
#define IMPCLK_CLK_SEL_DPRX1_CLK		(BIT9|BIT8)				// "1100"
#define IMPCLK_CLK_SEL_DPRX2_CLK		(BIT9|BIT8|BIT6)		// "1101"
#define IMPCLK_CLK_SEL_DVI1_OUT_CLK		(BIT9|BIT8|BIT7)		// "1110"
#define IMPCLK_CLK_SEL_DVI2_OUT_CLK		(BIT9|BIT8|BIT7|BIT6)	// "1111"


//
// IPP clock source selection
//
typedef enum
{
	IPP_CLK_SEL_TCLK,
	IPP_CLK_SEL_GND,
   	IPP_CLK_SEL_HD_G_CLK,
	IPP_CLK_SEL_ACLK,
	IPP_CLK_SEL_SCLK,
	IPP_CLK_SEL_ADC_OUT_CLK,
   	IPP_CLK_SEL_IPCLK0,
   	IPP_CLK_SEL_IPCLK1,
	IPP_CLK_SEL_VDDS1_CLK,
	IPP_CLK_SEL_VDDS2_CLK,
	IPP_CLK_SEL_HDMI_VCLK,
	IPP_CLK_SEL_HDMI_VCLK_OUT,
	IPP_CLK_SEL_DPRX1_CLK,
	IPP_CLK_SEL_DPRX2_CLK,
	IPP_CLK_SEL_DVI1_OUT_CLK,
	IPP_CLK_SEL_DVI2_OUT_CLK,
} gmt_IPP_CLK_SEL;


#define IPPCLK_CLK_SEL_TCLK				(0)							// "0000"
#define IPPCLK_CLK_SEL_GND				(BIT11)						// "0001"
#define IPPCLK_CLK_SEL_HD_CLK			(BIT12)						// "0010"
#define IPPCLK_CLK_SEL_ACLK				(BIT11|BIT12)				// "0011"
#define IPPCLK_CLK_SEL_SCLK				(BIT13)						// "0100"
#define IPPCLK_CLK_SEL_ADC_OUT_CLK		(BIT13|BIT11)				// "0101"
#define IPPCLK_CLK_SEL_IPCLK0			(BIT13|BIT12)				// "0110"
#define IPPCLK_CLK_SEL_IPCLK1			(BIT13|BIT12|BIT11)			// "0111"
#define IPPCLK_CLK_SEL_VDDS1_CLK		(BIT14)						// "1000"
#define IPPCLK_CLK_SEL_VDDS2_CLK		(BIT14|BIT11)				// "1001"
#define IPPCLK_CLK_SEL_HDMI_VCLK		(BIT14|BIT12)				// "1010"
#define IPPCLK_CLK_SEL_HDMI_OUT_VCLK	(BIT14|BIT12|BIT11)			// "1011"
#define IPPCLK_CLK_SEL_DPRX1_CLK		(BIT14|BIT13)				// "1100"
#define IPPCLK_CLK_SEL_DPRX2_CLK		(BIT14|BIT13|BIT11)			// "1101"
#define IPPCLK_CLK_SEL_DVI1_OUT_CLK		(BIT14|BIT13|BIT12)			// "1110"
#define IPPCLK_CLK_SEL_DVI2_OUT_CLK		(BIT14|BIT13|BIT12|BIT11)	// "1111"

//
// Register CLOCK_CONFIG6 bit definition
//
//#if (FEATURE_DISPLAY_PORT == ENABLE)


typedef enum
{
    DPRX_VCLK1_SEL_TCLK,
    DPRX_VCLK1_SEL_GND,
    DPRX_VCLK1_SEL_VDDS1,
    DPRX_VCLK1_SEL_VDDS2,
} gmt_DPRX_VCLK1_SEL;


#define DPRXCLK_VCLK1_SEL_TCLK   (0)
#define DPRXCLK_VCLK1_SEL_GND    (BIT0)
#define DPRXCLK_VCLK1_SEL_VDDS1  (BIT1)
#define DPRXCLK_VCLK1_SEL_VDDS2  (BIT1|BIT0)


typedef enum
{
    DPRX_VCLK2_SEL_TCLK,
    DPRX_VCLK2_SEL_GND,
    DPRX_VCLK2_SEL_VDDS1,
    DPRX_VCLK2_SEL_VDDS2,
} gmt_DPRX_VCLK2_SEL;


#define DPRXCLK_VCLK2_SEL_TCLK   (0)
#define DPRXCLK_VCLK2_SEL_GND    (BIT4)
#define DPRXCLK_VCLK2_SEL_VDDS1  (BIT5)
#define DPRXCLK_VCLK2_SEL_VDDS2  (BIT5|BIT4)


typedef enum
{
    DPRX_LTCLK_SEL_TCLK,
    DPRX_LTCLK_SEL_GND,		
    DPRX_LTCLK_SEL_FS432_SYNTH4_CLK,
} gmt_DPRX_LTCLK_SEL;

#define DPRXCLK_LTCLK_SEL_TCLK   			(0)
#define DPRXCLK_LTCLK_SEL_GND    			(BIT12)
#define DPRXCLK_LTCLK_SEL_FS432_SYNTH4_CLK	(BIT13)


typedef enum
{
    DPRX_AUCLK1_SEL_TCLK,
    DPRX_AUCLK1_SEL_GND,
    DPRX_AUCLK1_SEL_AUDDS1_CLK,
    DPRX_AUCLK1_SEL_AUDDS2_CLK,
    DPRX_AUCLK1_SEL_AUDDS3_CLK,
    DPRX_AUCLK1_SEL_AUDDS4_CLK,
}gmt_DPRX_AUCLK1_SEL;

#define DPRXCLK_AUCLK1_SEL_TCLK        (0)
#define DPRXCLK_AUCLK1_SEL_GND         (BIT3)
#define DPRXCLK_AUCLK1_SEL_AUDDS1_CLK  (BIT4)
#define DPRXCLK_AUCLK1_SEL_AUDDS2_CLK  (BIT4|BIT3)
#define DPRXCLK_AUCLK1_SEL_AUDDS3_CLK  (BIT5)
#define DPRXCLK_AUCLK1_SEL_AUDDS4_CLK  (BIT5|BIT3)

typedef enum
{
    DPRX_AUCLK2_SEL_TCLK,
    DPRX_AUCLK2_SEL_GND,
    DPRX_AUCLK2_SEL_AUDDS1_CLK,
    DPRX_AUCLK2_SEL_AUDDS2_CLK,
    DPRX_AUCLK2_SEL_AUDDS3_CLK,
    DPRX_AUCLK2_SEL_AUDDS4_CLK,
} gmt_DPRX_AUCLK2_SEL;

#define DPRXCLK_AUCLK2_SEL_TCLK        (0)
#define DPRXCLK_AUCLK2_SEL_GND         (BIT6)
#define DPRXCLK_AUCLK2_SEL_AUDDS1_CLK  (BIT7)
#define DPRXCLK_AUCLK2_SEL_AUDDS2_CLK  (BIT7|BIT6)
#define DPRXCLK_AUCLK2_SEL_AUDDS3_CLK  (BIT8)
#define DPRXCLK_AUCLK2_SEL_AUDDS4_CLK  (BIT8|BIT6)

typedef enum
{
    DPRX_AUXCLK_SEL_TCLK_DIV9,
    DPRX_AUXCLK_SEL_TCLK_DIV10,
    DPRX_AUXCLK_SEL_TCLK_DIV19,
    DPRX_AUXCLK_SEL_TCLK_DIV20,
    DPRX_AUXCLK_SEL_TCLK_DIV12,
    DPRX_AUXCLK_SEL_TCLK_DIV13,
    DPRX_AUXCLK_SEL_TCLK_DIV25,
    DPRX_AUXCLK_SEL_TCLK_DIV26,
} gmt_DPRX_AUXCLK_SEL;

#define DPRXCLK_AUXCLK_SEL_TCLK_DIV9    (0)
#define DPRXCLK_AUXCLK_SEL_TCLK_DIV10   (BIT11)
#define DPRXCLK_AUXCLK_SEL_TCLK_DIV19   (BIT12)
#define DPRXCLK_AUXCLK_SEL_TCLK_DIV20   (BIT12|BIT11)
#define DPRXCLK_AUXCLK_SEL_TCLK_DIV12   (BIT9)
#define DPRXCLK_AUXCLK_SEL_TCLK_DIV13   (BIT11|BIT9)
#define DPRXCLK_AUXCLK_SEL_TCLK_DIV25   (BIT12|BIT9)
#define DPRXCLK_AUXCLK_SEL_TCLK_DIV26   (BIT12|BIT11|BIT9)

//#endif // FEATURE_DISPLAY_PORT == ENABLE



typedef enum
{
   DPTX_VCLK_SEL_GND,
   DPTX_VCLK_SEL_OVP_VOUT,
} gmt_DPTX_VCLK_SEL;

#define DPTXCLK_VCLK_SEL_GND    		   (0)
#define DPTXCLK_VCLK_SEL_OVP_VOUT		(BIT8)


typedef enum
{
    DPTX_AUCLK1_SEL_TCLK,
    DPTX_AUCLK1_SEL_GND,
    DPTX_AUCLK1_SEL_AUDDS1_CLK,
    DPTX_AUCLK1_SEL_AUDDS2_CLK,
    DPTX_AUCLK1_SEL_AUDDS3_CLK,
    DPTX_AUCLK1_SEL_AUDDS4_CLK,
} gmt_DPTX_AUCLK1_SEL;

#define DPTXCLK_AUCLK1_SEL_TCLK         (0)
#define DPTXCLK_AUCLK1_SEL_GND          (BIT9)
#define DPTXCLK_AUCLK1_SEL_AUDDS1_CLK   (BIT10)
#define DPTXCLK_AUCLK1_SEL_AUDDS2_CLK   (BIT10|BIT9)
#define DPTXCLK_AUCLK1_SEL_AUDDS3_CLK   (BIT11)
#define DPTXCLK_AUCLK1_SEL_AUDDS4_CLK   (BIT11|BIT9)

typedef enum
{
    DPTX_AUCLK2_SEL_TCLK,
    DPTX_AUCLK2_SEL_GND,
    DPTX_AUCLK2_SEL_AUDDS1_CLK,
    DPTX_AUCLK2_SEL_AUDDS2_CLK,
    DPTX_AUCLK2_SEL_AUDDS3_CLK,
    DPTX_AUCLK2_SEL_AUDDS4_CLK,
} gmt_DPTX_AUCLK2_SEL;

#define DPTXCLK_AUCLK2_SEL_TCLK         (0)
#define DPTXCLK_AUCLK2_SEL_GND          (BIT12)
#define DPTXCLK_AUCLK2_SEL_AUDDS1_CLK   (BIT13)
#define DPTXCLK_AUCLK2_SEL_AUDDS2_CLK   (BIT13|BIT12)
#define DPTXCLK_AUCLK2_SEL_AUDDS3_CLK   (BIT14)
#define DPTXCLK_AUCLK2_SEL_AUDDS4_CLK   (BIT14|BIT12)




typedef enum
{
	DPTX_AUXCLK_DISABLE,
    DPTX_AUXCLK_SEL_2_077MHZ,
    DPTX_AUXCLK_SEL_1_929MHZ,			
} gmt_DPTX_AUXCLK_SEL;
#define DPTXCLK_AUXCLK_SEL_2_077MHZ		(0x65)
#define DPTXCLK_AUXCLK_SEL_1_929MHZ		(0x66)

typedef enum
{
	FS432_1,
	FS432_2,
	FS432_3,
	FS432_4,
	FS432_END,
}gmt_FS432Channnel;


#define RPLL_CONFIG_432MHZ		0xBF1
#define RPLL_CONFIG_378MHZ		0xB71


typedef enum
{
  VDDS_CLKSRC_DPRX1,
  VDDS_CLKSRC_DPRX2,
  VDDS_CLKSRC_HDMI,
  VDDS_CLKSRC_REG,
}gmt_VDDS_CLKSRC_SEL;


//******************************************************************************
//  P R O T O T Y P E S 
//******************************************************************************

//******************
// All Clocks
//******************
gmt_RET_STAT 		gm_ClkSelect(gmt_CLK B_Clk, BYTE B_SrcClk);
gmt_RET_STAT		gm_ClkEnable(gmt_CLK B_Clk, BYTE B_Enable);
gmt_RET_STAT        gm_ClkInverse(gmt_CLK B_Clk, BOOL B_Inverse);

//******************
// DDS
//******************
gmt_RET_STAT 		gm_DdsForceCloseLoop(gmt_CLK B_Clk);
gmt_RET_STAT 		gm_DdsForceOpenLoop(gmt_CLK B_Clk);
gmt_RET_STAT 		gm_DdsOpenLoop(gmt_CLK B_Clk);
gmt_RET_STAT 		gm_DdsCloseLoop(gmt_CLK B_Clk);
gmt_RET_STAT 		gm_DdsWaitStable(gmt_CLK B_Clk);
WORD 				gm_DdsGetFreq(gmt_CLK B_Clk);
gmt_RET_STAT		gm_DdsSetInitFreq(gmt_CLK B_Clk, DWORD D_Frequency);

//******************
// SDDS
//******************
void				gm_SddsSetHTotal(gmt_ADC_ID B_AdcId, WORD W_HTotal);
void 				gm_SetSrcFreeRunMode(void);

//******************
// DDDS
//******************
gmt_RET_PASSFAIL	gm_DisplayClockSetup(void);
gmt_RET_STAT 		gm_DddsLock(void);
gmt_RET_STAT		gm_DFLLock(BYTE B_ModulusIn, BYTE B_ModulusOut);
void 				gm_SetDisplayFreeRun(void);

//******************
// VDDS
//******************
void gm_VddsInit(gmt_CLK B_Clk, gmt_VDDS_CLKSRC_SEL B_VddsClkSel);


//******************
// AFR
//******************
void gm_SetAfrControl(WORD W_AfrEvents, BOOL B_Flag);


//******************
// FS432
//******************
gmt_RET_PASSFAIL gm_FS432Setup(gmt_FS432Channnel b_Channel, gmt_FS432Clk FsClkSelect, gmt_RCLK_FREQ_SEL RclkSelect);

//******************
// RCLK
//******************
gmt_RET_PASSFAIL gm_SetAndEnableRCLK(gmt_RCLK_FREQ_SEL RclkSelect);
gmt_RET_PASSFAIL gm_ReadRCLK(gmt_RCLK_FREQ_SEL *RclkSelect);
gmt_RET_PASSFAIL gm_PowerDownRCLK(void);

#endif


