	/*
	   $Workfile:	LM240WU9_ACC3Tbl.h  $
	   $Revision: 1.0 $
	   $Date: 2012/08/03 11:35:12 $
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
	// MODULE:		LM240WU9_ACC3Tbl.h
	//
	// USAGE:	   This file contains table for Automatic Contrast Control (ACC)
	//
	//******************************************************************************
	
	//******************************************************************************
	//	I N C L U D E	 F I L E S
	//******************************************************************************
	
	#include "StdLibInc\gm_Global.h"
	#include "StdLibReplace\Acc.h"

	
	//***************************************************************************
	//	C O D E
	//***************************************************************************
#if (FEATURE_ACC3 == ENABLE)
	
	VQ_ACC_Parameters_t ROM AccMultimediaGraphic = 
	{

		// 120924 KimChye
		
		{
			sizeof(VQ_ACC_Parameters_t),
			0, //STLAYER_VIDEO_ACC,
			0
		},
		{ // ACCSWParameters_high
			// HistStretchEnable
			0,
			// Cumulative_Black_Pixels
			200,
			// Cumulative_White_Pixels
			50,
			// Max_Black_Bin
			28,
			// Min_White_Bin
			188,
			// ACC_Strength
			96,
			{ // WF
				// WF00
				0,
				// WF01
				0,
				// WF02
				0,
				// WF03
				0,
				// WF04
				0,
				// WF05
				0,
				// WF06
				0,
				// WF07
				0,
				// WF08
				0,
				// WF09
				0,
				// WF10
				0,
				// WF11
				0,
				// WF12
				0,
				// WF13
				0,
				// WF14
				0,
				// WF15
				0,
				// WF16
				0,
				// WF17
				0,
				// WF18
				0,
				// WF19
				0,
				// WF20
				0,
				// WF21
				0,
				// WF22
				0,
				// WF23
				0,
				// WF24
				0,
				// WF25
				0,
				// WF26
				0,
				// WF27
				0,
				// WF28
				0,
				// WF29
				0,
				// WF30
				0,
				// WF31
				0,
				// WF32
				0,
				// WF33
				0,
				// WF34
				0,
				// WF35
				0,
				// WF36
				0,
				// WF37
				0,
				// WF38
				0,
				// WF39
				0,
				// WF40
				0,
				// WF41
				0,
				// WF42
				0,
				// WF43
				0,
				// WF44
				0,
				// WF45
				0,
				// WF46
				0,
				// WF47
				0,
				// WF48
				0,
				// WF49
				0,
				// WF50
				0,
				// WF51
				0,
				// WF52
				0,
				// WF53
				0,
				// WF54
				0,
				// WF55
				0,
				// WF56
				0,
				// WF57
				0,
				// WF58
				0,
				// WF59
				0,
				// WF60
				0,
				// WF61
				0,
				// WF62
				0,
				// WF63
				0
			},	// WF
			{ // BinLim
				// BinLim00
				138,
				// BinLim01
				150,
				// BinLim02
				162,
				// BinLim03
				168,
				// BinLim04
				180,
				// BinLim05
				198,
				// BinLim06
				210,
				// BinLim07
				210,
				// BinLim08
				210,
				// BinLim09
				215,
				// BinLim10
				215,
				// BinLim11
				220,
				// BinLim12
				230,
				// BinLim13
				240,
				// BinLim14
				255,
				// BinLim15
				255,
				// BinLim16
				255,
				// BinLim17
				255,
				// BinLim18
				255,
				// BinLim19
				255,
				// BinLim20
				255,
				// BinLim21
				255,
				// BinLim22
				255,
				// BinLim23
				255,
				// BinLim24
				255,
				// BinLim25
				255,
				// BinLim26
				255,
				// BinLim27
				255,
				// BinLim28
				255,
				// BinLim29
				255,
				// BinLim30
				255,
				// BinLim31
				255,
				// BinLim32
				255,
				// BinLim33
				255,
				// BinLim34
				255,
				// BinLim35
				255,
				// BinLim36
				255,
				// BinLim37
				255,
				// BinLim38
				255,
				// BinLim39
				255,
				// BinLim40
				255,
				// BinLim41
				255,
				// BinLim42
				255,
				// BinLim43
				255,
				// BinLim44
				255,
				// BinLim45
				255,
				// BinLim46
				255,
				// BinLim47
				255,
				// BinLim48
				255,
				// BinLim49
				255,
				// BinLim50
				255,
				// BinLim51
				255,
				// BinLim52
				255,
				// BinLim53
				255,
				// BinLim54
				313,
				// BinLim55
				367,
				// BinLim56
				415,
				// BinLim57
				445,
				// BinLim58
				475,
				// BinLim59
				427,
				// BinLim60
				433,
				// BinLim61
				451,
				// BinLim62
				457,
				// BinLim63
				463
			}	// BinLim
		},	// ACCSWParameters_high
		{ // ACCStaticLut_high
			{ // ACCStaticLut
				// ACCStaticLut00
				85,
				// ACCStaticLut01
				85,
				// ACCStaticLut02
				85,
				// ACCStaticLut03
				85,
				// ACCStaticLut04
				85,
				// ACCStaticLut05
				85,
				// ACCStaticLut06
				85,
				// ACCStaticLut07
				85,
				// ACCStaticLut08
				85,
				// ACCStaticLut09
				85,
				// ACCStaticLut10
				85,
				// ACCStaticLut11
				85,
				// ACCStaticLut12
				85,
				// ACCStaticLut13
				85,
				// ACCStaticLut14
				85,
				// ACCStaticLut15
				85,
				// ACCStaticLut16
				85,
				// ACCStaticLut17
				85,
				// ACCStaticLut18
				85,
				// ACCStaticLut19
				85,
				// ACCStaticLut20
				85,
				// ACCStaticLut21
				85,
				// ACCStaticLut22
				85,
				// ACCStaticLut23
				85,
				// ACCStaticLut24
				85,
				// ACCStaticLut25
				85,
				// ACCStaticLut26
				85,
				// ACCStaticLut27
				85,
				// ACCStaticLut28
				85,
				// ACCStaticLut29
				85,
				// ACCStaticLut30
				85,
				// ACCStaticLut31
				85,
				// ACCStaticLut32
				85,
				// ACCStaticLut33
				85,
				// ACCStaticLut34
				85,
				// ACCStaticLut35
				85,
				// ACCStaticLut36
				85,
				// ACCStaticLut37
				85,
				// ACCStaticLut38
				85,
				// ACCStaticLut39
				85,
				// ACCStaticLut40
				85,
				// ACCStaticLut41
				85,
				// ACCStaticLut42
				85,
				// ACCStaticLut43
				85,
				// ACCStaticLut44
				85,
				// ACCStaticLut45
				85,
				// ACCStaticLut46
				85,
				// ACCStaticLut47
				85,
				// ACCStaticLut48
				85,
				// ACCStaticLut49
				85,
				// ACCStaticLut50
				85,
				// ACCStaticLut51
				85,
				// ACCStaticLut52
				85,
				// ACCStaticLut53
				85,
				// ACCStaticLut54
				85,
				// ACCStaticLut55
				85,
				// ACCStaticLut56
				85,
				// ACCStaticLut57
				85,
				// ACCStaticLut58
				85,
				// ACCStaticLut59
				85,
				// ACCStaticLut60
				85,
				// ACCStaticLut61
				85,
				// ACCStaticLut62
				85,
				// ACCStaticLut63
				85
			}	// ACCStaticLut
		},	// ACCStaticLut_high
		{ // ACCHWParameters_high
			// IVP_IP_MEASURE_EN
			0,
			// IVP_IP_MEAS_SC_CHK_EN
			0,
			// IVP_IP_MEAS_SC_SOURCE
			0, //External Detector,
			// IVP_IP_MEAS_SC_THRESH
			0,
			// HpfAmpThreshY
			8,
			// HpfAmpThreshUV
			20,
			// ClampBlack
			1,
			// LutTempFilterEnable
			1,
			// YclNlsatScale
			4,
			// YclNlsatEn
			0,
			{ // YclNlsatTable
				// YclNlsatTable00
				16,
				// YclNlsatTable01
				16,
				// YclNlsatTable02
				16,
				// YclNlsatTable03
				16,
				// YclNlsatTable04
				16,
				// YclNlsatTable05
				16,
				// YclNlsatTable06
				16,
				// YclNlsatTable07
				16
			},	// YclNlsatTable
			// YclUseKY
			1,
			// CompLutLpfEn
			1,
			// CheckScEnable
			1,
			// SceneChangeResetEnable
			1,
			// TfFrameNum
			1, // 001 = 1/2 current frame, 1/2 previous LUT values,
			// YCLinkEnable
			1,
			// SceneChangeSource
			1, // External Detector,
			// LeftSlope
			5,
			// RightSlope
			6,
			// ScaleThresh
			189,
			// YCLinkGain
			4,
			// SceneChangeThreshold
			200,
			// XvYccEnable
			0,
			{ // YLpf
				// YLpf00
				0.0000,
				// YLpf01
				0.0000,
				// YLpf02
				0.0000,
				// YLpf03
				0.0000,
				// YLpf04
				0.0000
			},	// YLpf
			{ // CLpf
				// CLpf00
				0.0000,
				// CLpf01
				0.0000,
				// CLpf02
				0.0000,
				// CLpf03
				0.0000,
				// CLpf04
				0.0000
			},	// CLpf
			// ACCYLpfFlt
			 3, //9 taps (fixed),
			// ACCCLpfFlt
			 3, //9 taps (fixed),
			// UseLpfY
			1,
			// StaticLUTEn
			1,
			// LACCEnable
			1,
			// LACCTextureThresh
			20,
			// LACCDiffShift
			4,
			// LACCTextureOffset
			4,
			// LACCTextureGain
			255,
			// LACCSmoothThresh
			100,
			// LACCSmoothness
			0
		}	// ACCHWParameters_high
	};
	
	
	VQ_ACC_Parameters_t ROM AccMovieGraphic = 
	{
		//120924 KimChye
		
		{
			sizeof(VQ_ACC_Parameters_t),
			0, //STLAYER_VIDEO_ACC,
			0
		},
		{ // ACCSWParameters_high
			// HistStretchEnable
			0,
			// Cumulative_Black_Pixels
			200,
			// Cumulative_White_Pixels
			50,
			// Max_Black_Bin
			28,
			// Min_White_Bin
			188,
			// ACC_Strength
			96,
			{ // WF
				// WF00
				1,
				// WF01
				1,
				// WF02
				1,
				// WF03
				1,
				// WF04
				1,
				// WF05
				1,
				// WF06
				1,
				// WF07
				1,
				// WF08
				1,
				// WF09
				1,
				// WF10
				1,
				// WF11
				1,
				// WF12
				1,
				// WF13
				1,
				// WF14
				1,
				// WF15
				1,
				// WF16
				1,
				// WF17
				1,
				// WF18
				1,
				// WF19
				1,
				// WF20
				1,
				// WF21
				1,
				// WF22
				1,
				// WF23
				1,
				// WF24
				1,
				// WF25
				1,
				// WF26
				1,
				// WF27
				1,
				// WF28
				1,
				// WF29
				1,
				// WF30
				1,
				// WF31
				1,
				// WF32
				1,
				// WF33
				1,
				// WF34
				1,
				// WF35
				1,
				// WF36
				1,
				// WF37
				1,
				// WF38
				1,
				// WF39
				1,
				// WF40
				1,
				// WF41
				1,
				// WF42
				1,
				// WF43
				1,
				// WF44
				1,
				// WF45
				1,
				// WF46
				1,
				// WF47
				1,
				// WF48
				1,
				// WF49
				1,
				// WF50
				1,
				// WF51
				1,
				// WF52
				1,
				// WF53
				1,
				// WF54
				1,
				// WF55
				1,
				// WF56
				1,
				// WF57
				1,
				// WF58
				1,
				// WF59
				1,
				// WF60
				1,
				// WF61
				1,
				// WF62
				1,
				// WF63
				1
			},	// WF
			{ // BinLim
				// BinLim00
				255,
				// BinLim01
				255,
				// BinLim02
				255,
				// BinLim03
				255,
				// BinLim04
				255,
				// BinLim05
				255,
				// BinLim06
				255,
				// BinLim07
				255,
				// BinLim08
				255,
				// BinLim09
				255,
				// BinLim10
				255,
				// BinLim11
				255,
				// BinLim12
				255,
				// BinLim13
				255,
				// BinLim14
				255,
				// BinLim15
				255,
				// BinLim16
				255,
				// BinLim17
				255,
				// BinLim18
				255,
				// BinLim19
				255,
				// BinLim20
				255,
				// BinLim21
				255,
				// BinLim22
				255,
				// BinLim23
				255,
				// BinLim24
				255,
				// BinLim25
				255,
				// BinLim26
				255,
				// BinLim27
				255,
				// BinLim28
				255,
				// BinLim29
				255,
				// BinLim30
				255,
				// BinLim31
				255,
				// BinLim32
				255,
				// BinLim33
				255,
				// BinLim34
				255,
				// BinLim35
				255,
				// BinLim36
				255,
				// BinLim37
				255,
				// BinLim38
				255,
				// BinLim39
				255,
				// BinLim40
				255,
				// BinLim41
				255,
				// BinLim42
				255,
				// BinLim43
				255,
				// BinLim44
				255,
				// BinLim45
				255,
				// BinLim46
				255,
				// BinLim47
				255,
				// BinLim48
				255,
				// BinLim49
				255,
				// BinLim50
				255,
				// BinLim51
				255,
				// BinLim52
				255,
				// BinLim53
				255,
				// BinLim54
				255,
				// BinLim55
				255,
				// BinLim56
				255,
				// BinLim57
				255,
				// BinLim58
				255,
				// BinLim59
				255,
				// BinLim60
				255,
				// BinLim61
				255,
				// BinLim62
				255,
				// BinLim63
				255
			}	// BinLim
		},	// ACCSWParameters_high
		{ // ACCStaticLut_high
			{ // ACCStaticLut
				// ACCStaticLut00
				85,
				// ACCStaticLut01
				85,
				// ACCStaticLut02
				85,
				// ACCStaticLut03
				85,
				// ACCStaticLut04
				85,
				// ACCStaticLut05
				85,
				// ACCStaticLut06
				85,
				// ACCStaticLut07
				85,
				// ACCStaticLut08
				85,
				// ACCStaticLut09
				85,
				// ACCStaticLut10
				85,
				// ACCStaticLut11
				85,
				// ACCStaticLut12
				85,
				// ACCStaticLut13
				85,
				// ACCStaticLut14
				85,
				// ACCStaticLut15
				85,
				// ACCStaticLut16
				85,
				// ACCStaticLut17
				85,
				// ACCStaticLut18
				85,
				// ACCStaticLut19
				85,
				// ACCStaticLut20
				85,
				// ACCStaticLut21
				85,
				// ACCStaticLut22
				85,
				// ACCStaticLut23
				85,
				// ACCStaticLut24
				85,
				// ACCStaticLut25
				85,
				// ACCStaticLut26
				85,
				// ACCStaticLut27
				85,
				// ACCStaticLut28
				85,
				// ACCStaticLut29
				85,
				// ACCStaticLut30
				85,
				// ACCStaticLut31
				85,
				// ACCStaticLut32
				85,
				// ACCStaticLut33
				85,
				// ACCStaticLut34
				85,
				// ACCStaticLut35
				85,
				// ACCStaticLut36
				85,
				// ACCStaticLut37
				85,
				// ACCStaticLut38
				85,
				// ACCStaticLut39
				85,
				// ACCStaticLut40
				85,
				// ACCStaticLut41
				85,
				// ACCStaticLut42
				85,
				// ACCStaticLut43
				85,
				// ACCStaticLut44
				85,
				// ACCStaticLut45
				85,
				// ACCStaticLut46
				85,
				// ACCStaticLut47
				85,
				// ACCStaticLut48
				85,
				// ACCStaticLut49
				85,
				// ACCStaticLut50
				85,
				// ACCStaticLut51
				85,
				// ACCStaticLut52
				85,
				// ACCStaticLut53
				85,
				// ACCStaticLut54
				85,
				// ACCStaticLut55
				85,
				// ACCStaticLut56
				85,
				// ACCStaticLut57
				85,
				// ACCStaticLut58
				85,
				// ACCStaticLut59
				85,
				// ACCStaticLut60
				85,
				// ACCStaticLut61
				85,
				// ACCStaticLut62
				85,
				// ACCStaticLut63
				85
			}	// ACCStaticLut
		},	// ACCStaticLut_high
		{ // ACCHWParameters_high
			// IVP_IP_MEASURE_EN
			0,
			// IVP_IP_MEAS_SC_CHK_EN
			0,
			// IVP_IP_MEAS_SC_SOURCE
			0,// External Detector,
			// IVP_IP_MEAS_SC_THRESH
			0,
			// HpfAmpThreshY
			8,
			// HpfAmpThreshUV
			20,
			// ClampBlack
			1,
			// LutTempFilterEnable
			1,
			// YclNlsatScale
			4,
			// YclNlsatEn
			0,
			{ // YclNlsatTable
				// YclNlsatTable00
				16,
				// YclNlsatTable01
				16,
				// YclNlsatTable02
				16,
				// YclNlsatTable03
				16,
				// YclNlsatTable04
				16,
				// YclNlsatTable05
				16,
				// YclNlsatTable06
				16,
				// YclNlsatTable07
				16
			},	// YclNlsatTable
			// YclUseKY
			1,
			// CompLutLpfEn
			1,
			// CheckScEnable
			1,
			// SceneChangeResetEnable
			1,
			// TfFrameNum
			1, // 001 = 1/2 current frame, 1/2 previous LUT values,
			// YCLinkEnable
			1,
			// SceneChangeSource
			1,// ACC measured scene change,
			// LeftSlope
			5,
			// RightSlope
			6,
			// ScaleThresh
			189,
			// YCLinkGain
			4,
			// SceneChangeThreshold
			200,
			// XvYccEnable
			0,
			{ // YLpf
				// YLpf00
				0.0000,
				// YLpf01
				0.0000,
				// YLpf02
				0.0000,
				// YLpf03
				0.0000,
				// YLpf04
				0.0000
			},	// YLpf
			{ // CLpf
				// CLpf00
				0.0000,
				// CLpf01
				0.0000,
				// CLpf02
				0.0000,
				// CLpf03
				0.0000,
				// CLpf04
				0.0000
			},	// CLpf
			// ACCYLpfFlt
			 3, //9 taps (fixed),
			// ACCCLpfFlt
			 3, //9 taps (fixed),
			// UseLpfY
			1,
			// StaticLUTEn
			1,
			// LACCEnable
			1,
			// LACCTextureThresh
			20,
			// LACCDiffShift
			4,
			// LACCTextureOffset
			4,
			// LACCTextureGain
			255,
			// LACCSmoothThresh
			100,
			// LACCSmoothness
			0
		}	// ACCHWParameters_high
	};
	
	
	VQ_ACC_Parameters_t ROM AccGameGraphic = 
	{

		// 120924 KimChye
	
		{
			sizeof(VQ_ACC_Parameters_t),
			0, //STLAYER_VIDEO_ACC,
			0
		},
		{ // ACCSWParameters_high
			// HistStretchEnable
			0,
			// Cumulative_Black_Pixels
			200,
			// Cumulative_White_Pixels
			50,
			// Max_Black_Bin
			28,
			// Min_White_Bin
			188,
			// ACC_Strength
			10,
			{ // WF
				// WF00
				0,
				// WF01
				0,
				// WF02
				0,
				// WF03
				0,
				// WF04
				0,
				// WF05
				0,
				// WF06
				0,
				// WF07
				0,
				// WF08
				0,
				// WF09
				0,
				// WF10
				0,
				// WF11
				0,
				// WF12
				0,
				// WF13
				0,
				// WF14
				0,
				// WF15
				0,
				// WF16
				0,
				// WF17
				0,
				// WF18
				0,
				// WF19
				0,
				// WF20
				0,
				// WF21
				0,
				// WF22
				0,
				// WF23
				0,
				// WF24
				0,
				// WF25
				0,
				// WF26
				0,
				// WF27
				0,
				// WF28
				0,
				// WF29
				0,
				// WF30
				0,
				// WF31
				0,
				// WF32
				0,
				// WF33
				0,
				// WF34
				0,
				// WF35
				0,
				// WF36
				0,
				// WF37
				0,
				// WF38
				0,
				// WF39
				0,
				// WF40
				0,
				// WF41
				0,
				// WF42
				0,
				// WF43
				0,
				// WF44
				0,
				// WF45
				0,
				// WF46
				0,
				// WF47
				0,
				// WF48
				0,
				// WF49
				0,
				// WF50
				0,
				// WF51
				0,
				// WF52
				0,
				// WF53
				0,
				// WF54
				0,
				// WF55
				0,
				// WF56
				0,
				// WF57
				0,
				// WF58
				0,
				// WF59
				0,
				// WF60
				0,
				// WF61
				0,
				// WF62
				0,
				// WF63
				0
			},	// WF
			{ // BinLim
				// BinLim00
				245,
				// BinLim01
				245,
				// BinLim02
				245,
				// BinLim03
				245,
				// BinLim04
				245,
				// BinLim05
				245,
				// BinLim06
				245,
				// BinLim07
				245,
				// BinLim08
				245,
				// BinLim09
				245,
				// BinLim10
				245,
				// BinLim11
				245,
				// BinLim12
				245,
				// BinLim13
				245,
				// BinLim14
				255,
				// BinLim15
				255,
				// BinLim16
				255,
				// BinLim17
				255,
				// BinLim18
				255,
				// BinLim19
				255,
				// BinLim20
				255,
				// BinLim21
				255,
				// BinLim22
				255,
				// BinLim23
				255,
				// BinLim24
				255,
				// BinLim25
				255,
				// BinLim26
				255,
				// BinLim27
				255,
				// BinLim28
				255,
				// BinLim29
				255,
				// BinLim30
				255,
				// BinLim31
				255,
				// BinLim32
				255,
				// BinLim33
				255,
				// BinLim34
				255,
				// BinLim35
				255,
				// BinLim36
				255,
				// BinLim37
				255,
				// BinLim38
				255,
				// BinLim39
				255,
				// BinLim40
				255,
				// BinLim41
				255,
				// BinLim42
				255,
				// BinLim43
				255,
				// BinLim44
				255,
				// BinLim45
				255,
				// BinLim46
				255,
				// BinLim47
				255,
				// BinLim48
				255,
				// BinLim49
				255,
				// BinLim50
				255,
				// BinLim51
				255,
				// BinLim52
				255,
				// BinLim53
				255,
				// BinLim54
				255,
				// BinLim55
				255,
				// BinLim56
				255,
				// BinLim57
				255,
				// BinLim58
				255,
				// BinLim59
				255,
				// BinLim60
				255,
				// BinLim61
				255,
				// BinLim62
				255,
				// BinLim63
				255
			}	// BinLim
		},	// ACCSWParameters_high
		{ // ACCStaticLut_high
			{ // ACCStaticLut
				// ACCStaticLut00
				85,
				// ACCStaticLut01
				85,
				// ACCStaticLut02
				85,
				// ACCStaticLut03
				85,
				// ACCStaticLut04
				85,
				// ACCStaticLut05
				85,
				// ACCStaticLut06
				85,
				// ACCStaticLut07
				85,
				// ACCStaticLut08
				85,
				// ACCStaticLut09
				85,
				// ACCStaticLut10
				85,
				// ACCStaticLut11
				85,
				// ACCStaticLut12
				85,
				// ACCStaticLut13
				85,
				// ACCStaticLut14
				85,
				// ACCStaticLut15
				85,
				// ACCStaticLut16
				85,
				// ACCStaticLut17
				85,
				// ACCStaticLut18
				85,
				// ACCStaticLut19
				85,
				// ACCStaticLut20
				85,
				// ACCStaticLut21
				85,
				// ACCStaticLut22
				85,
				// ACCStaticLut23
				85,
				// ACCStaticLut24
				85,
				// ACCStaticLut25
				85,
				// ACCStaticLut26
				85,
				// ACCStaticLut27
				85,
				// ACCStaticLut28
				85,
				// ACCStaticLut29
				85,
				// ACCStaticLut30
				85,
				// ACCStaticLut31
				85,
				// ACCStaticLut32
				85,
				// ACCStaticLut33
				85,
				// ACCStaticLut34
				85,
				// ACCStaticLut35
				85,
				// ACCStaticLut36
				85,
				// ACCStaticLut37
				85,
				// ACCStaticLut38
				85,
				// ACCStaticLut39
				85,
				// ACCStaticLut40
				85,
				// ACCStaticLut41
				85,
				// ACCStaticLut42
				85,
				// ACCStaticLut43
				85,
				// ACCStaticLut44
				85,
				// ACCStaticLut45
				85,
				// ACCStaticLut46
				85,
				// ACCStaticLut47
				85,
				// ACCStaticLut48
				85,
				// ACCStaticLut49
				85,
				// ACCStaticLut50
				85,
				// ACCStaticLut51
				85,
				// ACCStaticLut52
				85,
				// ACCStaticLut53
				85,
				// ACCStaticLut54
				85,
				// ACCStaticLut55
				85,
				// ACCStaticLut56
				85,
				// ACCStaticLut57
				85,
				// ACCStaticLut58
				85,
				// ACCStaticLut59
				85,
				// ACCStaticLut60
				85,
				// ACCStaticLut61
				85,
				// ACCStaticLut62
				85,
				// ACCStaticLut63
				85
			}	// ACCStaticLut
		},	// ACCStaticLut_high
		{ // ACCHWParameters_high
			// IVP_IP_MEASURE_EN
			0,
			// IVP_IP_MEAS_SC_CHK_EN
			0,
			// IVP_IP_MEAS_SC_SOURCE
			0, // External Detector,
			// IVP_IP_MEAS_SC_THRESH
			0,
			// HpfAmpThreshY
			8,
			// HpfAmpThreshUV
			20,
			// ClampBlack
			1,
			// LutTempFilterEnable
			1,
			// YclNlsatScale
			4,
			// YclNlsatEn
			0,
			{ // YclNlsatTable
				// YclNlsatTable00
				16,
				// YclNlsatTable01
				16,
				// YclNlsatTable02
				16,
				// YclNlsatTable03
				16,
				// YclNlsatTable04
				16,
				// YclNlsatTable05
				16,
				// YclNlsatTable06
				16,
				// YclNlsatTable07
				16
			},	// YclNlsatTable
			// YclUseKY
			1,
			// CompLutLpfEn
			1,
			// CheckScEnable
			1,
			// SceneChangeResetEnable
			1,
			// TfFrameNum
			0,// 000 = current frame only (no filtering),
			// YCLinkEnable
			1,
			// SceneChangeSource
			1,// External Detector,
			// LeftSlope
			5,
			// RightSlope
			6,
			// ScaleThresh
			166,
			// YCLinkGain
			2,
			// SceneChangeThreshold
			200,
			// XvYccEnable
			0,
			{ // YLpf
				// YLpf00
				0.0000,
				// YLpf01
				0.0000,
				// YLpf02
				0.0000,
				// YLpf03
				0.0000,
				// YLpf04
				0.0000
			},	// YLpf
			{ // CLpf
				// CLpf00
				0.0000,
				// CLpf01
				0.0000,
				// CLpf02
				0.0000,
				// CLpf03
				0.0000,
				// CLpf04
				0.0000
			},	// CLpf
			// ACCYLpfFlt
			 3, //9 taps (fixed),
			// ACCCLpfFlt
			 3, //9 taps (fixed),
			// UseLpfY
			1,
			// StaticLUTEn
			1,
			// LACCEnable
			1,
			// LACCTextureThresh
			20,
			// LACCDiffShift
			4,
			// LACCTextureOffset
			4,
			// LACCTextureGain
			255,
			// LACCSmoothThresh
			100,
			// LACCSmoothness
			0
		}	// ACCHWParameters_high
	};

	VQ_ACC_Parameters_t ROM AccMultimediaVideo = 
	{
	
		// 120924 KimChye
		
		{
			sizeof(VQ_ACC_Parameters_t),
			0, //STLAYER_VIDEO_ACC,
			0
		},
		{ // ACCSWParameters_high
			// HistStretchEnable
			0,
			// Cumulative_Black_Pixels
			200,
			// Cumulative_White_Pixels
			50,
			// Max_Black_Bin
			28,
			// Min_White_Bin
			188,
			// ACC_Strength
			96,
			{ // WF
				// WF00
				0,
				// WF01
				0,
				// WF02
				0,
				// WF03
				0,
				// WF04
				0,
				// WF05
				0,
				// WF06
				0,
				// WF07
				0,
				// WF08
				0,
				// WF09
				0,
				// WF10
				0,
				// WF11
				0,
				// WF12
				0,
				// WF13
				0,
				// WF14
				0,
				// WF15
				0,
				// WF16
				0,
				// WF17
				0,
				// WF18
				0,
				// WF19
				0,
				// WF20
				0,
				// WF21
				0,
				// WF22
				0,
				// WF23
				0,
				// WF24
				0,
				// WF25
				0,
				// WF26
				0,
				// WF27
				0,
				// WF28
				0,
				// WF29
				0,
				// WF30
				0,
				// WF31
				0,
				// WF32
				0,
				// WF33
				0,
				// WF34
				0,
				// WF35
				0,
				// WF36
				0,
				// WF37
				0,
				// WF38
				0,
				// WF39
				0,
				// WF40
				0,
				// WF41
				0,
				// WF42
				0,
				// WF43
				0,
				// WF44
				0,
				// WF45
				0,
				// WF46
				0,
				// WF47
				0,
				// WF48
				0,
				// WF49
				0,
				// WF50
				0,
				// WF51
				0,
				// WF52
				0,
				// WF53
				0,
				// WF54
				0,
				// WF55
				0,
				// WF56
				0,
				// WF57
				0,
				// WF58
				0,
				// WF59
				0,
				// WF60
				0,
				// WF61
				0,
				// WF62
				0,
				// WF63
				0
			},	// WF
			{ // BinLim
				// BinLim00
				138,
				// BinLim01
				150,
				// BinLim02
				162,
				// BinLim03
				168,
				// BinLim04
				180,
				// BinLim05
				198,
				// BinLim06
				210,
				// BinLim07
				210,
				// BinLim08
				210,
				// BinLim09
				215,
				// BinLim10
				215,
				// BinLim11
				220,
				// BinLim12
				230,
				// BinLim13
				240,
				// BinLim14
				255,
				// BinLim15
				255,
				// BinLim16
				255,
				// BinLim17
				255,
				// BinLim18
				255,
				// BinLim19
				255,
				// BinLim20
				255,
				// BinLim21
				255,
				// BinLim22
				255,
				// BinLim23
				255,
				// BinLim24
				255,
				// BinLim25
				255,
				// BinLim26
				255,
				// BinLim27
				255,
				// BinLim28
				255,
				// BinLim29
				255,
				// BinLim30
				255,
				// BinLim31
				255,
				// BinLim32
				255,
				// BinLim33
				255,
				// BinLim34
				255,
				// BinLim35
				255,
				// BinLim36
				255,
				// BinLim37
				255,
				// BinLim38
				255,
				// BinLim39
				255,
				// BinLim40
				255,
				// BinLim41
				255,
				// BinLim42
				255,
				// BinLim43
				255,
				// BinLim44
				255,
				// BinLim45
				255,
				// BinLim46
				255,
				// BinLim47
				255,
				// BinLim48
				255,
				// BinLim49
				255,
				// BinLim50
				255,
				// BinLim51
				255,
				// BinLim52
				255,
				// BinLim53
				255,
				// BinLim54
				313,
				// BinLim55
				367,
				// BinLim56
				415,
				// BinLim57
				445,
				// BinLim58
				475,
				// BinLim59
				427,
				// BinLim60
				433,
				// BinLim61
				451,
				// BinLim62
				457,
				// BinLim63
				463
			}	// BinLim
		},	// ACCSWParameters_high
		{ // ACCStaticLut_high
			{ // ACCStaticLut
				// ACCStaticLut00
				85,
				// ACCStaticLut01
				85,
				// ACCStaticLut02
				85,
				// ACCStaticLut03
				85,
				// ACCStaticLut04
				85,
				// ACCStaticLut05
				85,
				// ACCStaticLut06
				85,
				// ACCStaticLut07
				85,
				// ACCStaticLut08
				85,
				// ACCStaticLut09
				85,
				// ACCStaticLut10
				85,
				// ACCStaticLut11
				85,
				// ACCStaticLut12
				85,
				// ACCStaticLut13
				85,
				// ACCStaticLut14
				85,
				// ACCStaticLut15
				85,
				// ACCStaticLut16
				85,
				// ACCStaticLut17
				85,
				// ACCStaticLut18
				85,
				// ACCStaticLut19
				85,
				// ACCStaticLut20
				85,
				// ACCStaticLut21
				85,
				// ACCStaticLut22
				85,
				// ACCStaticLut23
				85,
				// ACCStaticLut24
				85,
				// ACCStaticLut25
				85,
				// ACCStaticLut26
				85,
				// ACCStaticLut27
				85,
				// ACCStaticLut28
				85,
				// ACCStaticLut29
				85,
				// ACCStaticLut30
				85,
				// ACCStaticLut31
				85,
				// ACCStaticLut32
				85,
				// ACCStaticLut33
				85,
				// ACCStaticLut34
				85,
				// ACCStaticLut35
				85,
				// ACCStaticLut36
				85,
				// ACCStaticLut37
				85,
				// ACCStaticLut38
				85,
				// ACCStaticLut39
				85,
				// ACCStaticLut40
				85,
				// ACCStaticLut41
				85,
				// ACCStaticLut42
				85,
				// ACCStaticLut43
				85,
				// ACCStaticLut44
				85,
				// ACCStaticLut45
				85,
				// ACCStaticLut46
				85,
				// ACCStaticLut47
				85,
				// ACCStaticLut48
				85,
				// ACCStaticLut49
				85,
				// ACCStaticLut50
				85,
				// ACCStaticLut51
				85,
				// ACCStaticLut52
				85,
				// ACCStaticLut53
				85,
				// ACCStaticLut54
				85,
				// ACCStaticLut55
				85,
				// ACCStaticLut56
				85,
				// ACCStaticLut57
				85,
				// ACCStaticLut58
				85,
				// ACCStaticLut59
				85,
				// ACCStaticLut60
				85,
				// ACCStaticLut61
				85,
				// ACCStaticLut62
				85,
				// ACCStaticLut63
				85
			}	// ACCStaticLut
		},	// ACCStaticLut_high
		{ // ACCHWParameters_high
			// IVP_IP_MEASURE_EN
			0,
			// IVP_IP_MEAS_SC_CHK_EN
			0,
			// IVP_IP_MEAS_SC_SOURCE
			0, //External Detector,
			// IVP_IP_MEAS_SC_THRESH
			0,
			// HpfAmpThreshY
			8,
			// HpfAmpThreshUV
			20,
			// ClampBlack
			1,
			// LutTempFilterEnable
			1,
			// YclNlsatScale
			4,
			// YclNlsatEn
			0,
			{ // YclNlsatTable
				// YclNlsatTable00
				16,
				// YclNlsatTable01
				16,
				// YclNlsatTable02
				16,
				// YclNlsatTable03
				16,
				// YclNlsatTable04
				16,
				// YclNlsatTable05
				16,
				// YclNlsatTable06
				16,
				// YclNlsatTable07
				16
			},	// YclNlsatTable
			// YclUseKY
			1,
			// CompLutLpfEn
			1,
			// CheckScEnable
			1,
			// SceneChangeResetEnable
			1,
			// TfFrameNum
			1, // 001 = 1/2 current frame, 1/2 previous LUT values,
			// YCLinkEnable
			1,
			// SceneChangeSource
			1, // External Detector,
			// LeftSlope
			5,
			// RightSlope
			6,
			// ScaleThresh
			189,
			// YCLinkGain
			4,
			// SceneChangeThreshold
			200,
			// XvYccEnable
			0,
			{ // YLpf
				// YLpf00
				0.0000,
				// YLpf01
				0.0000,
				// YLpf02
				0.0000,
				// YLpf03
				0.0000,
				// YLpf04
				0.0000
			},	// YLpf
			{ // CLpf
				// CLpf00
				0.0000,
				// CLpf01
				0.0000,
				// CLpf02
				0.0000,
				// CLpf03
				0.0000,
				// CLpf04
				0.0000
			},	// CLpf
			// ACCYLpfFlt
			 3, //9 taps (fixed),
			// ACCCLpfFlt
			 3, //9 taps (fixed),
			// UseLpfY
			1,
			// StaticLUTEn
			1,
			// LACCEnable
			1,
			// LACCTextureThresh
			20,
			// LACCDiffShift
			4,
			// LACCTextureOffset
			4,
			// LACCTextureGain
			255,
			// LACCSmoothThresh
			100,
			// LACCSmoothness
			0
		}	// ACCHWParameters_high
	};
	
	
	VQ_ACC_Parameters_t ROM AccMovieVideo =
	{
		//120925 KimChye
		
		{
			sizeof(VQ_ACC_Parameters_t),
			0, //STLAYER_VIDEO_ACC,
			0
		},
		{ // ACCSWParameters_high
			// HistStretchEnable
			0,
			// Cumulative_Black_Pixels
			200,
			// Cumulative_White_Pixels
			50,
			// Max_Black_Bin
			28,
			// Min_White_Bin
			188,
			// ACC_Strength
			96,
			{ // WF
				// WF00
				1,
				// WF01
				1,
				// WF02
				1,
				// WF03
				1,
				// WF04
				1,
				// WF05
				1,
				// WF06
				1,
				// WF07
				1,
				// WF08
				1,
				// WF09
				1,
				// WF10
				1,
				// WF11
				1,
				// WF12
				1,
				// WF13
				1,
				// WF14
				1,
				// WF15
				1,
				// WF16
				1,
				// WF17
				1,
				// WF18
				1,
				// WF19
				1,
				// WF20
				1,
				// WF21
				1,
				// WF22
				1,
				// WF23
				1,
				// WF24
				1,
				// WF25
				1,
				// WF26
				1,
				// WF27
				1,
				// WF28
				1,
				// WF29
				1,
				// WF30
				1,
				// WF31
				1,
				// WF32
				1,
				// WF33
				1,
				// WF34
				1,
				// WF35
				1,
				// WF36
				1,
				// WF37
				1,
				// WF38
				1,
				// WF39
				1,
				// WF40
				1,
				// WF41
				1,
				// WF42
				1,
				// WF43
				1,
				// WF44
				1,
				// WF45
				1,
				// WF46
				1,
				// WF47
				1,
				// WF48
				1,
				// WF49
				1,
				// WF50
				1,
				// WF51
				1,
				// WF52
				1,
				// WF53
				1,
				// WF54
				1,
				// WF55
				1,
				// WF56
				1,
				// WF57
				1,
				// WF58
				1,
				// WF59
				1,
				// WF60
				1,
				// WF61
				1,
				// WF62
				1,
				// WF63
				1
			},	// WF
			{ // BinLim
				// BinLim00
				255,
				// BinLim01
				255,
				// BinLim02
				255,
				// BinLim03
				255,
				// BinLim04
				255,
				// BinLim05
				255,
				// BinLim06
				255,
				// BinLim07
				255,
				// BinLim08
				255,
				// BinLim09
				255,
				// BinLim10
				255,
				// BinLim11
				255,
				// BinLim12
				255,
				// BinLim13
				255,
				// BinLim14
				255,
				// BinLim15
				255,
				// BinLim16
				255,
				// BinLim17
				255,
				// BinLim18
				255,
				// BinLim19
				255,
				// BinLim20
				255,
				// BinLim21
				255,
				// BinLim22
				255,
				// BinLim23
				255,
				// BinLim24
				255,
				// BinLim25
				255,
				// BinLim26
				255,
				// BinLim27
				255,
				// BinLim28
				255,
				// BinLim29
				255,
				// BinLim30
				255,
				// BinLim31
				255,
				// BinLim32
				255,
				// BinLim33
				255,
				// BinLim34
				255,
				// BinLim35
				255,
				// BinLim36
				255,
				// BinLim37
				255,
				// BinLim38
				255,
				// BinLim39
				255,
				// BinLim40
				255,
				// BinLim41
				255,
				// BinLim42
				255,
				// BinLim43
				255,
				// BinLim44
				255,
				// BinLim45
				255,
				// BinLim46
				255,
				// BinLim47
				255,
				// BinLim48
				255,
				// BinLim49
				255,
				// BinLim50
				255,
				// BinLim51
				255,
				// BinLim52
				255,
				// BinLim53
				255,
				// BinLim54
				255,
				// BinLim55
				255,
				// BinLim56
				255,
				// BinLim57
				255,
				// BinLim58
				255,
				// BinLim59
				255,
				// BinLim60
				255,
				// BinLim61
				255,
				// BinLim62
				255,
				// BinLim63
				255
			}	// BinLim
		},	// ACCSWParameters_high
		{ // ACCStaticLut_high
			{ // ACCStaticLut
				// ACCStaticLut00
				85,
				// ACCStaticLut01
				85,
				// ACCStaticLut02
				85,
				// ACCStaticLut03
				85,
				// ACCStaticLut04
				85,
				// ACCStaticLut05
				85,
				// ACCStaticLut06
				85,
				// ACCStaticLut07
				85,
				// ACCStaticLut08
				85,
				// ACCStaticLut09
				85,
				// ACCStaticLut10
				85,
				// ACCStaticLut11
				85,
				// ACCStaticLut12
				85,
				// ACCStaticLut13
				85,
				// ACCStaticLut14
				85,
				// ACCStaticLut15
				85,
				// ACCStaticLut16
				85,
				// ACCStaticLut17
				85,
				// ACCStaticLut18
				85,
				// ACCStaticLut19
				85,
				// ACCStaticLut20
				85,
				// ACCStaticLut21
				85,
				// ACCStaticLut22
				85,
				// ACCStaticLut23
				85,
				// ACCStaticLut24
				85,
				// ACCStaticLut25
				85,
				// ACCStaticLut26
				85,
				// ACCStaticLut27
				85,
				// ACCStaticLut28
				85,
				// ACCStaticLut29
				85,
				// ACCStaticLut30
				85,
				// ACCStaticLut31
				85,
				// ACCStaticLut32
				85,
				// ACCStaticLut33
				85,
				// ACCStaticLut34
				85,
				// ACCStaticLut35
				85,
				// ACCStaticLut36
				85,
				// ACCStaticLut37
				85,
				// ACCStaticLut38
				85,
				// ACCStaticLut39
				85,
				// ACCStaticLut40
				85,
				// ACCStaticLut41
				85,
				// ACCStaticLut42
				85,
				// ACCStaticLut43
				85,
				// ACCStaticLut44
				85,
				// ACCStaticLut45
				85,
				// ACCStaticLut46
				85,
				// ACCStaticLut47
				85,
				// ACCStaticLut48
				85,
				// ACCStaticLut49
				85,
				// ACCStaticLut50
				85,
				// ACCStaticLut51
				85,
				// ACCStaticLut52
				85,
				// ACCStaticLut53
				85,
				// ACCStaticLut54
				85,
				// ACCStaticLut55
				85,
				// ACCStaticLut56
				85,
				// ACCStaticLut57
				85,
				// ACCStaticLut58
				85,
				// ACCStaticLut59
				85,
				// ACCStaticLut60
				85,
				// ACCStaticLut61
				85,
				// ACCStaticLut62
				85,
				// ACCStaticLut63
				85
			}	// ACCStaticLut
		},	// ACCStaticLut_high
		{ // ACCHWParameters_high
			// IVP_IP_MEASURE_EN
			0,
			// IVP_IP_MEAS_SC_CHK_EN
			0,
			// IVP_IP_MEAS_SC_SOURCE
			0,// External Detector,
			// IVP_IP_MEAS_SC_THRESH
			0,
			// HpfAmpThreshY
			8,
			// HpfAmpThreshUV
			20,
			// ClampBlack
			1,
			// LutTempFilterEnable
			1,
			// YclNlsatScale
			4,
			// YclNlsatEn
			0,
			{ // YclNlsatTable
				// YclNlsatTable00
				16,
				// YclNlsatTable01
				16,
				// YclNlsatTable02
				16,
				// YclNlsatTable03
				16,
				// YclNlsatTable04
				16,
				// YclNlsatTable05
				16,
				// YclNlsatTable06
				16,
				// YclNlsatTable07
				16
			},	// YclNlsatTable
			// YclUseKY
			1,
			// CompLutLpfEn
			1,
			// CheckScEnable
			1,
			// SceneChangeResetEnable
			1,
			// TfFrameNum
			1, // 001 = 1/2 current frame, 1/2 previous LUT values,
			// YCLinkEnable
			1,
			// SceneChangeSource
			1,// ACC measured scene change,
			// LeftSlope
			5,
			// RightSlope
			6,
			// ScaleThresh
			189,
			// YCLinkGain
			4,
			// SceneChangeThreshold
			200,
			// XvYccEnable
			0,
			{ // YLpf
				// YLpf00
				0.0000,
				// YLpf01
				0.0000,
				// YLpf02
				0.0000,
				// YLpf03
				0.0000,
				// YLpf04
				0.0000
			},	// YLpf
			{ // CLpf
				// CLpf00
				0.0000,
				// CLpf01
				0.0000,
				// CLpf02
				0.0000,
				// CLpf03
				0.0000,
				// CLpf04
				0.0000
			},	// CLpf
			// ACCYLpfFlt
			 3, //9 taps (fixed),
			// ACCCLpfFlt
			 3, //9 taps (fixed),
			// UseLpfY
			1,
			// StaticLUTEn
			1,
			// LACCEnable
			1,
			// LACCTextureThresh
			20,
			// LACCDiffShift
			4,
			// LACCTextureOffset
			4,
			// LACCTextureGain
			255,
			// LACCSmoothThresh
			100,
			// LACCSmoothness
			0
		}	// ACCHWParameters_high
	};
	
	
	VQ_ACC_Parameters_t ROM AccGameVideo =
	{
	
		// 120924 KimChye
	
		{
			sizeof(VQ_ACC_Parameters_t),
			0, //STLAYER_VIDEO_ACC,
			0
		},
		{ // ACCSWParameters_high
			// HistStretchEnable
			0,
			// Cumulative_Black_Pixels
			200,
			// Cumulative_White_Pixels
			50,
			// Max_Black_Bin
			28,
			// Min_White_Bin
			188,
			// ACC_Strength
			10,
			{ // WF
				// WF00
				0,
				// WF01
				0,
				// WF02
				0,
				// WF03
				0,
				// WF04
				0,
				// WF05
				0,
				// WF06
				0,
				// WF07
				0,
				// WF08
				0,
				// WF09
				0,
				// WF10
				0,
				// WF11
				0,
				// WF12
				0,
				// WF13
				0,
				// WF14
				0,
				// WF15
				0,
				// WF16
				0,
				// WF17
				0,
				// WF18
				0,
				// WF19
				0,
				// WF20
				0,
				// WF21
				0,
				// WF22
				0,
				// WF23
				0,
				// WF24
				0,
				// WF25
				0,
				// WF26
				0,
				// WF27
				0,
				// WF28
				0,
				// WF29
				0,
				// WF30
				0,
				// WF31
				0,
				// WF32
				0,
				// WF33
				0,
				// WF34
				0,
				// WF35
				0,
				// WF36
				0,
				// WF37
				0,
				// WF38
				0,
				// WF39
				0,
				// WF40
				0,
				// WF41
				0,
				// WF42
				0,
				// WF43
				0,
				// WF44
				0,
				// WF45
				0,
				// WF46
				0,
				// WF47
				0,
				// WF48
				0,
				// WF49
				0,
				// WF50
				0,
				// WF51
				0,
				// WF52
				0,
				// WF53
				0,
				// WF54
				0,
				// WF55
				0,
				// WF56
				0,
				// WF57
				0,
				// WF58
				0,
				// WF59
				0,
				// WF60
				0,
				// WF61
				0,
				// WF62
				0,
				// WF63
				0
			},	// WF
			{ // BinLim
				// BinLim00
				245,
				// BinLim01
				245,
				// BinLim02
				245,
				// BinLim03
				245,
				// BinLim04
				245,
				// BinLim05
				245,
				// BinLim06
				245,
				// BinLim07
				245,
				// BinLim08
				245,
				// BinLim09
				245,
				// BinLim10
				245,
				// BinLim11
				245,
				// BinLim12
				245,
				// BinLim13
				245,
				// BinLim14
				255,
				// BinLim15
				255,
				// BinLim16
				255,
				// BinLim17
				255,
				// BinLim18
				255,
				// BinLim19
				255,
				// BinLim20
				255,
				// BinLim21
				255,
				// BinLim22
				255,
				// BinLim23
				255,
				// BinLim24
				255,
				// BinLim25
				255,
				// BinLim26
				255,
				// BinLim27
				255,
				// BinLim28
				255,
				// BinLim29
				255,
				// BinLim30
				255,
				// BinLim31
				255,
				// BinLim32
				255,
				// BinLim33
				255,
				// BinLim34
				255,
				// BinLim35
				255,
				// BinLim36
				255,
				// BinLim37
				255,
				// BinLim38
				255,
				// BinLim39
				255,
				// BinLim40
				255,
				// BinLim41
				255,
				// BinLim42
				255,
				// BinLim43
				255,
				// BinLim44
				255,
				// BinLim45
				255,
				// BinLim46
				255,
				// BinLim47
				255,
				// BinLim48
				255,
				// BinLim49
				255,
				// BinLim50
				255,
				// BinLim51
				255,
				// BinLim52
				255,
				// BinLim53
				255,
				// BinLim54
				255,
				// BinLim55
				255,
				// BinLim56
				255,
				// BinLim57
				255,
				// BinLim58
				255,
				// BinLim59
				255,
				// BinLim60
				255,
				// BinLim61
				255,
				// BinLim62
				255,
				// BinLim63
				255
			}	// BinLim
		},	// ACCSWParameters_high
		{ // ACCStaticLut_high
			{ // ACCStaticLut
				// ACCStaticLut00
				85,
				// ACCStaticLut01
				85,
				// ACCStaticLut02
				85,
				// ACCStaticLut03
				85,
				// ACCStaticLut04
				85,
				// ACCStaticLut05
				85,
				// ACCStaticLut06
				85,
				// ACCStaticLut07
				85,
				// ACCStaticLut08
				85,
				// ACCStaticLut09
				85,
				// ACCStaticLut10
				85,
				// ACCStaticLut11
				85,
				// ACCStaticLut12
				85,
				// ACCStaticLut13
				85,
				// ACCStaticLut14
				85,
				// ACCStaticLut15
				85,
				// ACCStaticLut16
				85,
				// ACCStaticLut17
				85,
				// ACCStaticLut18
				85,
				// ACCStaticLut19
				85,
				// ACCStaticLut20
				85,
				// ACCStaticLut21
				85,
				// ACCStaticLut22
				85,
				// ACCStaticLut23
				85,
				// ACCStaticLut24
				85,
				// ACCStaticLut25
				85,
				// ACCStaticLut26
				85,
				// ACCStaticLut27
				85,
				// ACCStaticLut28
				85,
				// ACCStaticLut29
				85,
				// ACCStaticLut30
				85,
				// ACCStaticLut31
				85,
				// ACCStaticLut32
				85,
				// ACCStaticLut33
				85,
				// ACCStaticLut34
				85,
				// ACCStaticLut35
				85,
				// ACCStaticLut36
				85,
				// ACCStaticLut37
				85,
				// ACCStaticLut38
				85,
				// ACCStaticLut39
				85,
				// ACCStaticLut40
				85,
				// ACCStaticLut41
				85,
				// ACCStaticLut42
				85,
				// ACCStaticLut43
				85,
				// ACCStaticLut44
				85,
				// ACCStaticLut45
				85,
				// ACCStaticLut46
				85,
				// ACCStaticLut47
				85,
				// ACCStaticLut48
				85,
				// ACCStaticLut49
				85,
				// ACCStaticLut50
				85,
				// ACCStaticLut51
				85,
				// ACCStaticLut52
				85,
				// ACCStaticLut53
				85,
				// ACCStaticLut54
				85,
				// ACCStaticLut55
				85,
				// ACCStaticLut56
				85,
				// ACCStaticLut57
				85,
				// ACCStaticLut58
				85,
				// ACCStaticLut59
				85,
				// ACCStaticLut60
				85,
				// ACCStaticLut61
				85,
				// ACCStaticLut62
				85,
				// ACCStaticLut63
				85
			}	// ACCStaticLut
		},	// ACCStaticLut_high
		{ // ACCHWParameters_high
			// IVP_IP_MEASURE_EN
			0,
			// IVP_IP_MEAS_SC_CHK_EN
			0,
			// IVP_IP_MEAS_SC_SOURCE
			0, // External Detector,
			// IVP_IP_MEAS_SC_THRESH
			0,
			// HpfAmpThreshY
			8,
			// HpfAmpThreshUV
			20,
			// ClampBlack
			1,
			// LutTempFilterEnable
			1,
			// YclNlsatScale
			4,
			// YclNlsatEn
			0,
			{ // YclNlsatTable
				// YclNlsatTable00
				16,
				// YclNlsatTable01
				16,
				// YclNlsatTable02
				16,
				// YclNlsatTable03
				16,
				// YclNlsatTable04
				16,
				// YclNlsatTable05
				16,
				// YclNlsatTable06
				16,
				// YclNlsatTable07
				16
			},	// YclNlsatTable
			// YclUseKY
			1,
			// CompLutLpfEn
			1,
			// CheckScEnable
			1,
			// SceneChangeResetEnable
			1,
			// TfFrameNum
			0,// 000 = current frame only (no filtering),
			// YCLinkEnable
			1,
			// SceneChangeSource
			1,// External Detector,
			// LeftSlope
			5,
			// RightSlope
			6,
			// ScaleThresh
			166,
			// YCLinkGain
			2,
			// SceneChangeThreshold
			200,
			// XvYccEnable
			0,
			{ // YLpf
				// YLpf00
				0.0000,
				// YLpf01
				0.0000,
				// YLpf02
				0.0000,
				// YLpf03
				0.0000,
				// YLpf04
				0.0000
			},	// YLpf
			{ // CLpf
				// CLpf00
				0.0000,
				// CLpf01
				0.0000,
				// CLpf02
				0.0000,
				// CLpf03
				0.0000,
				// CLpf04
				0.0000
			},	// CLpf
			// ACCYLpfFlt
			 3, //9 taps (fixed),
			// ACCCLpfFlt
			 3, //9 taps (fixed),
			// UseLpfY
			1,
			// StaticLUTEn
			1,
			// LACCEnable
			1,
			// LACCTextureThresh
			20,
			// LACCDiffShift
			4,
			// LACCTextureOffset
			4,
			// LACCTextureGain
			255,
			// LACCSmoothThresh
			100,
			// LACCSmoothness
			0
		}	// ACCHWParameters_high
	};
	
	VQ_ACC_Parameters_t ROM AccNatureVideo =
	{
	   {
		  sizeof(VQ_ACC_Parameters_t),
		  0, // STLAYER_VIDEO_ACC,
		  0
	   },
	   { // ACCSWParameters_high
		  0,	// HistStretchEnable	0
		  200,	// Cumulative_Black_Pixels	200
		  50, // Cumulative_White_Pixels	50
		  28, // Max_Black_Bin	28
		  188,	// Min_White_Bin	188
		  96, // ACC_Strength 96
		  { // WF
			 0, // WF00 0
			 0, // WF01 0
			 0, // WF02 0
			 0, // WF03 0
			 0, // WF04 0
			 0, // WF05 0
			 0, // WF06 0
			 0, // WF07 0
			 1, // WF08 1
			 2, // WF09 2
			 3, // WF10 3
			 3, // WF11 3
			 3, // WF12 3
			 3, // WF13 3
			 3, // WF14 3
			 3, // WF15 3
			 3, // WF16 3
			 3, // WF17 3
			 3, // WF18 3
			 3, // WF19 3
			 3, // WF20 3
			 3, // WF21 3
			 2, // WF22 2
			 1, // WF23 1
			 0, // WF24 0
			 0, // WF25 0
			 0, // WF26 0
			 0, // WF27 0
			 0, // WF28 0
			 0, // WF29 0
			 0, // WF30 0
			 0, // WF31 0
			 0, // WF32 0
			 0, // WF33 0
			 0, // WF34 0
			 0, // WF35 0
			 0, // WF36 0
			 0, // WF37 0
			 0, // WF38 0
			 0, // WF39 0
			 0, // WF40 0
			 0, // WF41 0
			 0, // WF42 0
			 0, // WF43 0
			 0, // WF44 0
			 0, // WF45 0
			 0, // WF46 0
			 0, // WF47 0
			 0, // WF48 0
			 0, // WF49 0
			 0, // WF50 0
			 0, // WF51 0
			 0, // WF52 0
			 0, // WF53 0
			 0, // WF54 0
			 0, // WF55 0
			 0, // WF56 0
			 0, // WF57 0
			 0, // WF58 0
			 0, // WF59 0
			 0, // WF60 0
			 0, // WF61 0
			 0, // WF62 0
			 0	// WF63 0
		  },	// WF
		  { // BinLim
			 324,	// BinLim00 324
			 324,	// BinLim01 324
			 324,	// BinLim02 324
			 333,	// BinLim03 333
			 342,	// BinLim04 342
			 351,	// BinLim05 351
			 358,	// BinLim06 358
			 364,	// BinLim07 364
			 370,	// BinLim08 370
			 376,	// BinLim09 376
			 381,	// BinLim10 381
			 385,	// BinLim11 385
			 390,	// BinLim12 390
			 394,	// BinLim13 394
			 397,	// BinLim14 397
			 398,	// BinLim15 398
			 399,	// BinLim16 399
			 400,	// BinLim17 400
			 401,	// BinLim18 401
			 402,	// BinLim19 402
			 403,	// BinLim20 403
			 404,	// BinLim21 404
			 400,	// BinLim22 400
			 392,	// BinLim23 392
			 383,	// BinLim24 383
			 375,	// BinLim25 375
			 372,	// BinLim26 372
			 374,	// BinLim27 374
			 376,	// BinLim28 376
			 378,	// BinLim29 378
			 378,	// BinLim30 378
			 376,	// BinLim31 376
			 374,	// BinLim32 374
			 372,	// BinLim33 372
			 370,	// BinLim34 370
			 368,	// BinLim35 368
			 366,	// BinLim36 366
			 364,	// BinLim37 364
			 364,	// BinLim38 364
			 365,	// BinLim39 365
			 366,	// BinLim40 366
			 367,	// BinLim41 367
			 366,	// BinLim42 366
			 362,	// BinLim43 362
			 358,	// BinLim44 358
			 354,	// BinLim45 354
			 350,	// BinLim46 350
			 346,	// BinLim47 346
			 342,	// BinLim48 342
			 338,	// BinLim49 338
			 333,	// BinLim50 333
			 326,	// BinLim51 326
			 320,	// BinLim52 320
			 313,	// BinLim53 313
			 312,	// BinLim54 312
			 315,	// BinLim55 315
			 318,	// BinLim56 318
			 321,	// BinLim57 321
			 321,	// BinLim58 321
			 318,	// BinLim59 318
			 315,	// BinLim60 315
			 312,	// BinLim61 312
			 312,	// BinLim62 312
			 312 // BinLim63 312
		  } // BinLim
	   },	// ACCSWParameters_high
	   { // ACCStaticLut_high
		  { // ACCStaticLut
			 1, // ACCStaticLut00	1
			 1, // ACCStaticLut01	1
			 1, // ACCStaticLut02	1
			 1, // ACCStaticLut03	1
			 1, // ACCStaticLut04	1
			 1, // ACCStaticLut05	1
			 1, // ACCStaticLut06	1
			 1, // ACCStaticLut07	1
			 1, // ACCStaticLut08	1
			 1, // ACCStaticLut09	1
			 1, // ACCStaticLut10	1
			 1, // ACCStaticLut11	1
			 1, // ACCStaticLut12	1
			 1, // ACCStaticLut13	1
			 1, // ACCStaticLut14	1
			 1, // ACCStaticLut15	1
			 1, // ACCStaticLut16	1
			 1, // ACCStaticLut17	1
			 1, // ACCStaticLut18	1
			 1, // ACCStaticLut19	1
			 1, // ACCStaticLut20	1
			 1, // ACCStaticLut21	1
			 1, // ACCStaticLut22	1
			 1, // ACCStaticLut23	1
			 1, // ACCStaticLut24	1
			 1, // ACCStaticLut25	1
			 1, // ACCStaticLut26	1
			 1, // ACCStaticLut27	1
			 1, // ACCStaticLut28	1
			 1, // ACCStaticLut29	1
			 1, // ACCStaticLut30	1
			 1, // ACCStaticLut31	1
			 1, // ACCStaticLut32	1
			 1, // ACCStaticLut33	1
			 1, // ACCStaticLut34	1
			 1, // ACCStaticLut35	1
			 1, // ACCStaticLut36	1
			 1, // ACCStaticLut37	1
			 1, // ACCStaticLut38	1
			 1, // ACCStaticLut39	1
			 1, // ACCStaticLut40	1
			 1, // ACCStaticLut41	1
			 1, // ACCStaticLut42	1
			 1, // ACCStaticLut43	1
			 1, // ACCStaticLut44	1
			 1, // ACCStaticLut45	1
			 1, // ACCStaticLut46	1
			 1, // ACCStaticLut47	1
			 1, // ACCStaticLut48	1
			 1, // ACCStaticLut49	1
			 1, // ACCStaticLut50	1
			 1, // ACCStaticLut51	1
			 1, // ACCStaticLut52	1
			 1, // ACCStaticLut53	1
			 1, // ACCStaticLut54	1
			 1, // ACCStaticLut55	1
			 1, // ACCStaticLut56	1
			 1, // ACCStaticLut57	1
			 1, // ACCStaticLut58	1
			 1, // ACCStaticLut59	1
			 1, // ACCStaticLut60	1
			 1, // ACCStaticLut61	1
			 1, // ACCStaticLut62	1
			 1	// ACCStaticLut63	1
		  } // ACCStaticLut
	   },	// ACCStaticLut_high
	   { // ACCHWParameters_high
		  0,	// IVP_IP_MEASURE_EN	0
		  0,	// IVP_IP_MEAS_SC_CHK_EN	0
		  0,	// IVP_IP_MEAS_SC_SOURCE	 External Detector
		  0,	// IVP_IP_MEAS_SC_THRESH	0
		  8,	// HpfAmpThreshY	8
		  20, // HpfAmpThreshUV 20
		  1,	// ClampBlack	1
		  1,	// LutTempFilterEnable	1
		  4,	// YclNlsatScale	4
		  0,	// YclNlsatEn	0
		  { // YclNlsatTable
			 16, // YclNlsatTable00 16
			 16, // YclNlsatTable01 16
			 16, // YclNlsatTable02 16
			 16, // YclNlsatTable03 16
			 16, // YclNlsatTable04 16
			 16, // YclNlsatTable05 16
			 16, // YclNlsatTable06 16
			 16 // YclNlsatTable07	16
		  },	// YclNlsatTable
		  1,	// YclUseKY 1
		  1,	// CompLutLpfEn 1
		  1,	// CheckScEnable	1
		  1,	// SceneChangeResetEnable	1
		  0,	// TfFrameNum	 000 = current frame only (no filtering)
		  1,	// YCLinkEnable 1
		  1,	// SceneChangeSource	 ACC measured scene change
		  5,	// LeftSlope	5
		  6,	// RightSlope	6
		  189,	// ScaleThresh	189
		  9,	// YCLinkGain	9
		  200,	// SceneChangeThreshold 200
		  0,	// XvYccEnable	0
		  { // YLpf
			 0, // YLpf00	0.0000
			 0, // YLpf01	0.0000
			 0, // YLpf02	0.0000
			 0, // YLpf03	0.0000
			 0	// YLpf04	0.0000
		  },	// YLpf
		  { // CLpf
			 0, // CLpf00	0.0000
			 0, // CLpf01	0.0000
			 0, // CLpf02	0.0000
			 0, // CLpf03	0.0000
			 0	// CLpf04	0.0000
		  },	// CLpf
		  3,	// ACCYLpfFlt	 9 taps (fixed)
		  3,	// ACCCLpfFlt	 9 taps (fixed)
		  1,	// UseLpfY	1
		  1,	// StaticLUTEn	1
		  1,	// LACCEnable	1
		  20, // LACCTextureThresh	20
		  4,	// LACCDiffShift	4
		  4,	// LACCTextureOffset	4
		  255,	// LACCTextureGain	255
		  100,	// LACCSmoothThresh 100
		  0 // LACCSmoothness	0
	   }	// ACCHWParameters_high
	};
	
	
	VQ_ACC_Parameters_t ROM AccMovieVideo_Comp =
	{
	   {
		  sizeof(VQ_ACC_Parameters_t),
		  0, // STLAYER_VIDEO_ACC,
		  0
	   },
	   { // ACCSWParameters_high
		  0,	// HistStretchEnable	0
		  200,	// Cumulative_Black_Pixels	200
		  50, // Cumulative_White_Pixels	50
		  28, // Max_Black_Bin	28
		  188,	// Min_White_Bin	188
		  96, // ACC_Strength 96
		  { // WF
			 0, // WF00 0
			 0, // WF01 0
			 0, // WF02 0
			 0, // WF03 0
			 0, // WF04 0
			 0, // WF05 0
			 0, // WF06 0
			 0, // WF07 0
			 1, // WF08 1
			 2, // WF09 2
			 3, // WF10 3
			 3, // WF11 3
			 3, // WF12 3
			 3, // WF13 3
			 3, // WF14 3
			 3, // WF15 3
			 3, // WF16 3
			 3, // WF17 3
			 3, // WF18 3
			 3, // WF19 3
			 3, // WF20 3
			 3, // WF21 3
			 2, // WF22 2
			 1, // WF23 1
			 0, // WF24 0
			 0, // WF25 0
			 0, // WF26 0
			 0, // WF27 0
			 0, // WF28 0
			 0, // WF29 0
			 0, // WF30 0
			 0, // WF31 0
			 0, // WF32 0
			 0, // WF33 0
			 0, // WF34 0
			 0, // WF35 0
			 0, // WF36 0
			 0, // WF37 0
			 0, // WF38 0
			 0, // WF39 0
			 0, // WF40 0
			 0, // WF41 0
			 0, // WF42 0
			 0, // WF43 0
			 0, // WF44 0
			 0, // WF45 0
			 0, // WF46 0
			 0, // WF47 0
			 0, // WF48 0
			 0, // WF49 0
			 0, // WF50 0
			 0, // WF51 0
			 0, // WF52 0
			 0, // WF53 0
			 0, // WF54 0
			 0, // WF55 0
			 0, // WF56 0
			 0, // WF57 0
			 0, // WF58 0
			 0, // WF59 0
			 0, // WF60 0
			 0, // WF61 0
			 0, // WF62 0
			 0	// WF63 0
		  },	// WF
		  { // BinLim
			 324,	// BinLim00 324
			 324,	// BinLim01 324
			 324,	// BinLim02 324
			 333,	// BinLim03 333
			 342,	// BinLim04 342
			 351,	// BinLim05 351
			 358,	// BinLim06 358
			 364,	// BinLim07 364
			 370,	// BinLim08 370
			 376,	// BinLim09 376
			 381,	// BinLim10 381
			 385,	// BinLim11 385
			 390,	// BinLim12 390
			 394,	// BinLim13 394
			 397,	// BinLim14 397
			 398,	// BinLim15 398
			 399,	// BinLim16 399
			 400,	// BinLim17 400
			 401,	// BinLim18 401
			 402,	// BinLim19 402
			 403,	// BinLim20 403
			 404,	// BinLim21 404
			 400,	// BinLim22 400
			 392,	// BinLim23 392
			 383,	// BinLim24 383
			 375,	// BinLim25 375
			 372,	// BinLim26 372
			 374,	// BinLim27 374
			 376,	// BinLim28 376
			 378,	// BinLim29 378
			 378,	// BinLim30 378
			 376,	// BinLim31 376
			 374,	// BinLim32 374
			 372,	// BinLim33 372
			 370,	// BinLim34 370
			 368,	// BinLim35 368
			 366,	// BinLim36 366
			 364,	// BinLim37 364
			 364,	// BinLim38 364
			 365,	// BinLim39 365
			 366,	// BinLim40 366
			 367,	// BinLim41 367
			 366,	// BinLim42 366
			 362,	// BinLim43 362
			 358,	// BinLim44 358
			 354,	// BinLim45 354
			 350,	// BinLim46 350
			 346,	// BinLim47 346
			 342,	// BinLim48 342
			 338,	// BinLim49 338
			 333,	// BinLim50 333
			 326,	// BinLim51 326
			 320,	// BinLim52 320
			 313,	// BinLim53 313
			 312,	// BinLim54 312
			 315,	// BinLim55 315
			 318,	// BinLim56 318
			 321,	// BinLim57 321
			 321,	// BinLim58 321
			 318,	// BinLim59 318
			 315,	// BinLim60 315
			 312,	// BinLim61 312
			 312,	// BinLim62 312
			 312 // BinLim63 312
		  } // BinLim
	   },	// ACCSWParameters_high
	   { // ACCStaticLut_high
		  { // ACCStaticLut
			 1, // ACCStaticLut00	1
			 1, // ACCStaticLut01	1
			 1, // ACCStaticLut02	1
			 1, // ACCStaticLut03	1
			 1, // ACCStaticLut04	1
			 1, // ACCStaticLut05	1
			 1, // ACCStaticLut06	1
			 1, // ACCStaticLut07	1
			 1, // ACCStaticLut08	1
			 1, // ACCStaticLut09	1
			 1, // ACCStaticLut10	1
			 1, // ACCStaticLut11	1
			 1, // ACCStaticLut12	1
			 1, // ACCStaticLut13	1
			 1, // ACCStaticLut14	1
			 1, // ACCStaticLut15	1
			 1, // ACCStaticLut16	1
			 1, // ACCStaticLut17	1
			 1, // ACCStaticLut18	1
			 1, // ACCStaticLut19	1
			 1, // ACCStaticLut20	1
			 1, // ACCStaticLut21	1
			 1, // ACCStaticLut22	1
			 1, // ACCStaticLut23	1
			 1, // ACCStaticLut24	1
			 1, // ACCStaticLut25	1
			 1, // ACCStaticLut26	1
			 1, // ACCStaticLut27	1
			 1, // ACCStaticLut28	1
			 1, // ACCStaticLut29	1
			 1, // ACCStaticLut30	1
			 1, // ACCStaticLut31	1
			 1, // ACCStaticLut32	1
			 1, // ACCStaticLut33	1
			 1, // ACCStaticLut34	1
			 1, // ACCStaticLut35	1
			 1, // ACCStaticLut36	1
			 1, // ACCStaticLut37	1
			 1, // ACCStaticLut38	1
			 1, // ACCStaticLut39	1
			 1, // ACCStaticLut40	1
			 1, // ACCStaticLut41	1
			 1, // ACCStaticLut42	1
			 1, // ACCStaticLut43	1
			 1, // ACCStaticLut44	1
			 1, // ACCStaticLut45	1
			 1, // ACCStaticLut46	1
			 1, // ACCStaticLut47	1
			 1, // ACCStaticLut48	1
			 1, // ACCStaticLut49	1
			 1, // ACCStaticLut50	1
			 1, // ACCStaticLut51	1
			 1, // ACCStaticLut52	1
			 1, // ACCStaticLut53	1
			 1, // ACCStaticLut54	1
			 1, // ACCStaticLut55	1
			 1, // ACCStaticLut56	1
			 1, // ACCStaticLut57	1
			 1, // ACCStaticLut58	1
			 1, // ACCStaticLut59	1
			 1, // ACCStaticLut60	1
			 1, // ACCStaticLut61	1
			 1, // ACCStaticLut62	1
			 1	// ACCStaticLut63	1
		  } // ACCStaticLut
	   },	// ACCStaticLut_high
	   { // ACCHWParameters_high
		  0,	// IVP_IP_MEASURE_EN	0
		  0,	// IVP_IP_MEAS_SC_CHK_EN	0
		  0,	// IVP_IP_MEAS_SC_SOURCE	 External Detector
		  0,	// IVP_IP_MEAS_SC_THRESH	0
		  8,	// HpfAmpThreshY	8
		  20, // HpfAmpThreshUV 20
		  1,	// ClampBlack	1
		  1,	// LutTempFilterEnable	1
		  4,	// YclNlsatScale	4
		  0,	// YclNlsatEn	0
		  { // YclNlsatTable
			 16, // YclNlsatTable00 16
			 16, // YclNlsatTable01 16
			 16, // YclNlsatTable02 16
			 16, // YclNlsatTable03 16
			 16, // YclNlsatTable04 16
			 16, // YclNlsatTable05 16
			 16, // YclNlsatTable06 16
			 16 // YclNlsatTable07	16
		  },	// YclNlsatTable
		  1,	// YclUseKY 1
		  1,	// CompLutLpfEn 1
		  1,	// CheckScEnable	1
		  1,	// SceneChangeResetEnable	1
		  0,	// TfFrameNum	 000 = current frame only (no filtering)
		  1,	// YCLinkEnable 1
		  1,	// SceneChangeSource	 ACC measured scene change
		  5,	// LeftSlope	5
		  6,	// RightSlope	6
		  189,	// ScaleThresh	189
		  9,	// YCLinkGain	9
		  200,	// SceneChangeThreshold 200
		  0,	// XvYccEnable	0
		  { // YLpf
			 0, // YLpf00	0.0000
			 0, // YLpf01	0.0000
			 0, // YLpf02	0.0000
			 0, // YLpf03	0.0000
			 0	// YLpf04	0.0000
		  },	// YLpf
		  { // CLpf
			 0, // CLpf00	0.0000
			 0, // CLpf01	0.0000
			 0, // CLpf02	0.0000
			 0, // CLpf03	0.0000
			 0	// CLpf04	0.0000
		  },	// CLpf
		  3,	// ACCYLpfFlt	 9 taps (fixed)
		  3,	// ACCCLpfFlt	 9 taps (fixed)
		  1,	// UseLpfY	1
		  1,	// StaticLUTEn	1
		  1,	// LACCEnable	1
		  20, // LACCTextureThresh	20
		  4,	// LACCDiffShift	4
		  4,	// LACCTextureOffset	4
		  255,	// LACCTextureGain	255
		  100,	// LACCSmoothThresh 100
		  0 // LACCSmoothness	0
	   }	// ACCHWParameters_high
	};
	
	
	
	VQ_ACC_Parameters_t ROM AccGameVideo_Comp =
	{
	   {
		  sizeof(VQ_ACC_Parameters_t),
		  0, // STLAYER_VIDEO_ACC,
		  0
	   },
	   { // ACCSWParameters_high
		  0,	// HistStretchEnable	0
		  200,	// Cumulative_Black_Pixels	200
		  50, // Cumulative_White_Pixels	50
		  28, // Max_Black_Bin	28
		  188,	// Min_White_Bin	188
		  96, // ACC_Strength 96
		  { // WF
			 0, // WF00 0
			 0, // WF01 0
			 0, // WF02 0
			 0, // WF03 0
			 0, // WF04 0
			 0, // WF05 0
			 0, // WF06 0
			 0, // WF07 0
			 1, // WF08 1
			 2, // WF09 2
			 3, // WF10 3
			 3, // WF11 3
			 3, // WF12 3
			 3, // WF13 3
			 3, // WF14 3
			 3, // WF15 3
			 3, // WF16 3
			 3, // WF17 3
			 3, // WF18 3
			 3, // WF19 3
			 3, // WF20 3
			 3, // WF21 3
			 2, // WF22 2
			 1, // WF23 1
			 0, // WF24 0
			 0, // WF25 0
			 0, // WF26 0
			 0, // WF27 0
			 0, // WF28 0
			 0, // WF29 0
			 0, // WF30 0
			 0, // WF31 0
			 0, // WF32 0
			 0, // WF33 0
			 0, // WF34 0
			 0, // WF35 0
			 0, // WF36 0
			 0, // WF37 0
			 0, // WF38 0
			 0, // WF39 0
			 0, // WF40 0
			 0, // WF41 0
			 0, // WF42 0
			 0, // WF43 0
			 0, // WF44 0
			 0, // WF45 0
			 0, // WF46 0
			 0, // WF47 0
			 0, // WF48 0
			 0, // WF49 0
			 0, // WF50 0
			 0, // WF51 0
			 0, // WF52 0
			 0, // WF53 0
			 0, // WF54 0
			 0, // WF55 0
			 0, // WF56 0
			 0, // WF57 0
			 0, // WF58 0
			 0, // WF59 0
			 0, // WF60 0
			 0, // WF61 0
			 0, // WF62 0
			 0	// WF63 0
		  },	// WF
		  { // BinLim
			 324,	// BinLim00 324
			 324,	// BinLim01 324
			 324,	// BinLim02 324
			 333,	// BinLim03 333
			 342,	// BinLim04 342
			 351,	// BinLim05 351
			 358,	// BinLim06 358
			 364,	// BinLim07 364
			 370,	// BinLim08 370
			 376,	// BinLim09 376
			 381,	// BinLim10 381
			 385,	// BinLim11 385
			 390,	// BinLim12 390
			 394,	// BinLim13 394
			 397,	// BinLim14 397
			 398,	// BinLim15 398
			 399,	// BinLim16 399
			 400,	// BinLim17 400
			 401,	// BinLim18 401
			 402,	// BinLim19 402
			 403,	// BinLim20 403
			 404,	// BinLim21 404
			 400,	// BinLim22 400
			 392,	// BinLim23 392
			 383,	// BinLim24 383
			 375,	// BinLim25 375
			 372,	// BinLim26 372
			 374,	// BinLim27 374
			 376,	// BinLim28 376
			 378,	// BinLim29 378
			 378,	// BinLim30 378
			 376,	// BinLim31 376
			 374,	// BinLim32 374
			 372,	// BinLim33 372
			 370,	// BinLim34 370
			 368,	// BinLim35 368
			 366,	// BinLim36 366
			 364,	// BinLim37 364
			 364,	// BinLim38 364
			 365,	// BinLim39 365
			 366,	// BinLim40 366
			 367,	// BinLim41 367
			 366,	// BinLim42 366
			 362,	// BinLim43 362
			 358,	// BinLim44 358
			 354,	// BinLim45 354
			 350,	// BinLim46 350
			 346,	// BinLim47 346
			 342,	// BinLim48 342
			 338,	// BinLim49 338
			 333,	// BinLim50 333
			 326,	// BinLim51 326
			 320,	// BinLim52 320
			 313,	// BinLim53 313
			 312,	// BinLim54 312
			 315,	// BinLim55 315
			 318,	// BinLim56 318
			 321,	// BinLim57 321
			 321,	// BinLim58 321
			 318,	// BinLim59 318
			 315,	// BinLim60 315
			 312,	// BinLim61 312
			 312,	// BinLim62 312
			 312 // BinLim63 312
		  } // BinLim
	   },	// ACCSWParameters_high
	   { // ACCStaticLut_high
		  { // ACCStaticLut
			 1, // ACCStaticLut00	1
			 1, // ACCStaticLut01	1
			 1, // ACCStaticLut02	1
			 1, // ACCStaticLut03	1
			 1, // ACCStaticLut04	1
			 1, // ACCStaticLut05	1
			 1, // ACCStaticLut06	1
			 1, // ACCStaticLut07	1
			 1, // ACCStaticLut08	1
			 1, // ACCStaticLut09	1
			 1, // ACCStaticLut10	1
			 1, // ACCStaticLut11	1
			 1, // ACCStaticLut12	1
			 1, // ACCStaticLut13	1
			 1, // ACCStaticLut14	1
			 1, // ACCStaticLut15	1
			 1, // ACCStaticLut16	1
			 1, // ACCStaticLut17	1
			 1, // ACCStaticLut18	1
			 1, // ACCStaticLut19	1
			 1, // ACCStaticLut20	1
			 1, // ACCStaticLut21	1
			 1, // ACCStaticLut22	1
			 1, // ACCStaticLut23	1
			 1, // ACCStaticLut24	1
			 1, // ACCStaticLut25	1
			 1, // ACCStaticLut26	1
			 1, // ACCStaticLut27	1
			 1, // ACCStaticLut28	1
			 1, // ACCStaticLut29	1
			 1, // ACCStaticLut30	1
			 1, // ACCStaticLut31	1
			 1, // ACCStaticLut32	1
			 1, // ACCStaticLut33	1
			 1, // ACCStaticLut34	1
			 1, // ACCStaticLut35	1
			 1, // ACCStaticLut36	1
			 1, // ACCStaticLut37	1
			 1, // ACCStaticLut38	1
			 1, // ACCStaticLut39	1
			 1, // ACCStaticLut40	1
			 1, // ACCStaticLut41	1
			 1, // ACCStaticLut42	1
			 1, // ACCStaticLut43	1
			 1, // ACCStaticLut44	1
			 1, // ACCStaticLut45	1
			 1, // ACCStaticLut46	1
			 1, // ACCStaticLut47	1
			 1, // ACCStaticLut48	1
			 1, // ACCStaticLut49	1
			 1, // ACCStaticLut50	1
			 1, // ACCStaticLut51	1
			 1, // ACCStaticLut52	1
			 1, // ACCStaticLut53	1
			 1, // ACCStaticLut54	1
			 1, // ACCStaticLut55	1
			 1, // ACCStaticLut56	1
			 1, // ACCStaticLut57	1
			 1, // ACCStaticLut58	1
			 1, // ACCStaticLut59	1
			 1, // ACCStaticLut60	1
			 1, // ACCStaticLut61	1
			 1, // ACCStaticLut62	1
			 1	// ACCStaticLut63	1
		  } // ACCStaticLut
	   },	// ACCStaticLut_high
	   { // ACCHWParameters_high
		  0,	// IVP_IP_MEASURE_EN	0
		  0,	// IVP_IP_MEAS_SC_CHK_EN	0
		  0,	// IVP_IP_MEAS_SC_SOURCE	 External Detector
		  0,	// IVP_IP_MEAS_SC_THRESH	0
		  8,	// HpfAmpThreshY	8
		  20, // HpfAmpThreshUV 20
		  1,	// ClampBlack	1
		  1,	// LutTempFilterEnable	1
		  4,	// YclNlsatScale	4
		  0,	// YclNlsatEn	0
		  { // YclNlsatTable
			 16, // YclNlsatTable00 16
			 16, // YclNlsatTable01 16
			 16, // YclNlsatTable02 16
			 16, // YclNlsatTable03 16
			 16, // YclNlsatTable04 16
			 16, // YclNlsatTable05 16
			 16, // YclNlsatTable06 16
			 16 // YclNlsatTable07	16
		  },	// YclNlsatTable
		  1,	// YclUseKY 1
		  1,	// CompLutLpfEn 1
		  1,	// CheckScEnable	1
		  1,	// SceneChangeResetEnable	1
		  0,	// TfFrameNum	 000 = current frame only (no filtering)
		  1,	// YCLinkEnable 1
		  1,	// SceneChangeSource	 ACC measured scene change
		  5,	// LeftSlope	5
		  6,	// RightSlope	6
		  189,	// ScaleThresh	189
		  9,	// YCLinkGain	9
		  200,	// SceneChangeThreshold 200
		  0,	// XvYccEnable	0
		  { // YLpf
			 0, // YLpf00	0.0000
			 0, // YLpf01	0.0000
			 0, // YLpf02	0.0000
			 0, // YLpf03	0.0000
			 0	// YLpf04	0.0000
		  },	// YLpf
		  { // CLpf
			 0, // CLpf00	0.0000
			 0, // CLpf01	0.0000
			 0, // CLpf02	0.0000
			 0, // CLpf03	0.0000
			 0	// CLpf04	0.0000
		  },	// CLpf
		  3,	// ACCYLpfFlt	 9 taps (fixed)
		  3,	// ACCCLpfFlt	 9 taps (fixed)
		  1,	// UseLpfY	1
		  1,	// StaticLUTEn	1
		  1,	// LACCEnable	1
		  20, // LACCTextureThresh	20
		  4,	// LACCDiffShift	4
		  4,	// LACCTextureOffset	4
		  255,	// LACCTextureGain	255
		  100,	// LACCSmoothThresh 100
		  0 // LACCSmoothness	0
	   }	// ACCHWParameters_high
	};
	
	
	VQ_ACC_Parameters_t ROM AccNatureVideo_Comp =
	{
	   {
		  sizeof(VQ_ACC_Parameters_t),
		  0, // STLAYER_VIDEO_ACC,
		  0
	   },
	   { // ACCSWParameters_high
		  0,	// HistStretchEnable	0
		  200,	// Cumulative_Black_Pixels	200
		  50, // Cumulative_White_Pixels	50
		  28, // Max_Black_Bin	28
		  188,	// Min_White_Bin	188
		  96, // ACC_Strength 96
		  { // WF
			 0, // WF00 0
			 0, // WF01 0
			 0, // WF02 0
			 0, // WF03 0
			 0, // WF04 0
			 0, // WF05 0
			 0, // WF06 0
			 0, // WF07 0
			 1, // WF08 1
			 2, // WF09 2
			 3, // WF10 3
			 3, // WF11 3
			 3, // WF12 3
			 3, // WF13 3
			 3, // WF14 3
			 3, // WF15 3
			 3, // WF16 3
			 3, // WF17 3
			 3, // WF18 3
			 3, // WF19 3
			 3, // WF20 3
			 3, // WF21 3
			 2, // WF22 2
			 1, // WF23 1
			 0, // WF24 0
			 0, // WF25 0
			 0, // WF26 0
			 0, // WF27 0
			 0, // WF28 0
			 0, // WF29 0
			 0, // WF30 0
			 0, // WF31 0
			 0, // WF32 0
			 0, // WF33 0
			 0, // WF34 0
			 0, // WF35 0
			 0, // WF36 0
			 0, // WF37 0
			 0, // WF38 0
			 0, // WF39 0
			 0, // WF40 0
			 0, // WF41 0
			 0, // WF42 0
			 0, // WF43 0
			 0, // WF44 0
			 0, // WF45 0
			 0, // WF46 0
			 0, // WF47 0
			 0, // WF48 0
			 0, // WF49 0
			 0, // WF50 0
			 0, // WF51 0
			 0, // WF52 0
			 0, // WF53 0
			 0, // WF54 0
			 0, // WF55 0
			 0, // WF56 0
			 0, // WF57 0
			 0, // WF58 0
			 0, // WF59 0
			 0, // WF60 0
			 0, // WF61 0
			 0, // WF62 0
			 0	// WF63 0
		  },	// WF
		  { // BinLim
			 324,	// BinLim00 324
			 324,	// BinLim01 324
			 324,	// BinLim02 324
			 333,	// BinLim03 333
			 342,	// BinLim04 342
			 351,	// BinLim05 351
			 358,	// BinLim06 358
			 364,	// BinLim07 364
			 370,	// BinLim08 370
			 376,	// BinLim09 376
			 381,	// BinLim10 381
			 385,	// BinLim11 385
			 390,	// BinLim12 390
			 394,	// BinLim13 394
			 397,	// BinLim14 397
			 398,	// BinLim15 398
			 399,	// BinLim16 399
			 400,	// BinLim17 400
			 401,	// BinLim18 401
			 402,	// BinLim19 402
			 403,	// BinLim20 403
			 404,	// BinLim21 404
			 400,	// BinLim22 400
			 392,	// BinLim23 392
			 383,	// BinLim24 383
			 375,	// BinLim25 375
			 372,	// BinLim26 372
			 374,	// BinLim27 374
			 376,	// BinLim28 376
			 378,	// BinLim29 378
			 378,	// BinLim30 378
			 376,	// BinLim31 376
			 374,	// BinLim32 374
			 372,	// BinLim33 372
			 370,	// BinLim34 370
			 368,	// BinLim35 368
			 366,	// BinLim36 366
			 364,	// BinLim37 364
			 364,	// BinLim38 364
			 365,	// BinLim39 365
			 366,	// BinLim40 366
			 367,	// BinLim41 367
			 366,	// BinLim42 366
			 362,	// BinLim43 362
			 358,	// BinLim44 358
			 354,	// BinLim45 354
			 350,	// BinLim46 350
			 346,	// BinLim47 346
			 342,	// BinLim48 342
			 338,	// BinLim49 338
			 333,	// BinLim50 333
			 326,	// BinLim51 326
			 320,	// BinLim52 320
			 313,	// BinLim53 313
			 312,	// BinLim54 312
			 315,	// BinLim55 315
			 318,	// BinLim56 318
			 321,	// BinLim57 321
			 321,	// BinLim58 321
			 318,	// BinLim59 318
			 315,	// BinLim60 315
			 312,	// BinLim61 312
			 312,	// BinLim62 312
			 312 // BinLim63 312
		  } // BinLim
	   },	// ACCSWParameters_high
	   { // ACCStaticLut_high
		  { // ACCStaticLut
			 1, // ACCStaticLut00	1
			 1, // ACCStaticLut01	1
			 1, // ACCStaticLut02	1
			 1, // ACCStaticLut03	1
			 1, // ACCStaticLut04	1
			 1, // ACCStaticLut05	1
			 1, // ACCStaticLut06	1
			 1, // ACCStaticLut07	1
			 1, // ACCStaticLut08	1
			 1, // ACCStaticLut09	1
			 1, // ACCStaticLut10	1
			 1, // ACCStaticLut11	1
			 1, // ACCStaticLut12	1
			 1, // ACCStaticLut13	1
			 1, // ACCStaticLut14	1
			 1, // ACCStaticLut15	1
			 1, // ACCStaticLut16	1
			 1, // ACCStaticLut17	1
			 1, // ACCStaticLut18	1
			 1, // ACCStaticLut19	1
			 1, // ACCStaticLut20	1
			 1, // ACCStaticLut21	1
			 1, // ACCStaticLut22	1
			 1, // ACCStaticLut23	1
			 1, // ACCStaticLut24	1
			 1, // ACCStaticLut25	1
			 1, // ACCStaticLut26	1
			 1, // ACCStaticLut27	1
			 1, // ACCStaticLut28	1
			 1, // ACCStaticLut29	1
			 1, // ACCStaticLut30	1
			 1, // ACCStaticLut31	1
			 1, // ACCStaticLut32	1
			 1, // ACCStaticLut33	1
			 1, // ACCStaticLut34	1
			 1, // ACCStaticLut35	1
			 1, // ACCStaticLut36	1
			 1, // ACCStaticLut37	1
			 1, // ACCStaticLut38	1
			 1, // ACCStaticLut39	1
			 1, // ACCStaticLut40	1
			 1, // ACCStaticLut41	1
			 1, // ACCStaticLut42	1
			 1, // ACCStaticLut43	1
			 1, // ACCStaticLut44	1
			 1, // ACCStaticLut45	1
			 1, // ACCStaticLut46	1
			 1, // ACCStaticLut47	1
			 1, // ACCStaticLut48	1
			 1, // ACCStaticLut49	1
			 1, // ACCStaticLut50	1
			 1, // ACCStaticLut51	1
			 1, // ACCStaticLut52	1
			 1, // ACCStaticLut53	1
			 1, // ACCStaticLut54	1
			 1, // ACCStaticLut55	1
			 1, // ACCStaticLut56	1
			 1, // ACCStaticLut57	1
			 1, // ACCStaticLut58	1
			 1, // ACCStaticLut59	1
			 1, // ACCStaticLut60	1
			 1, // ACCStaticLut61	1
			 1, // ACCStaticLut62	1
			 1	// ACCStaticLut63	1
		  } // ACCStaticLut
	   },	// ACCStaticLut_high
	   { // ACCHWParameters_high
		  0,	// IVP_IP_MEASURE_EN	0
		  0,	// IVP_IP_MEAS_SC_CHK_EN	0
		  0,	// IVP_IP_MEAS_SC_SOURCE	 External Detector
		  0,	// IVP_IP_MEAS_SC_THRESH	0
		  8,	// HpfAmpThreshY	8
		  20, // HpfAmpThreshUV 20
		  1,	// ClampBlack	1
		  1,	// LutTempFilterEnable	1
		  4,	// YclNlsatScale	4
		  0,	// YclNlsatEn	0
		  { // YclNlsatTable
			 16, // YclNlsatTable00 16
			 16, // YclNlsatTable01 16
			 16, // YclNlsatTable02 16
			 16, // YclNlsatTable03 16
			 16, // YclNlsatTable04 16
			 16, // YclNlsatTable05 16
			 16, // YclNlsatTable06 16
			 16 // YclNlsatTable07	16
		  },	// YclNlsatTable
		  1,	// YclUseKY 1
		  1,	// CompLutLpfEn 1
		  1,	// CheckScEnable	1
		  1,	// SceneChangeResetEnable	1
		  0,	// TfFrameNum	 000 = current frame only (no filtering)
		  1,	// YCLinkEnable 1
		  1,	// SceneChangeSource	 ACC measured scene change
		  5,	// LeftSlope	5
		  6,	// RightSlope	6
		  189,	// ScaleThresh	189
		  9,	// YCLinkGain	9
		  200,	// SceneChangeThreshold 200
		  0,	// XvYccEnable	0
		  { // YLpf
			 0, // YLpf00	0.0000
			 0, // YLpf01	0.0000
			 0, // YLpf02	0.0000
			 0, // YLpf03	0.0000
			 0	// YLpf04	0.0000
		  },	// YLpf
		  { // CLpf
			 0, // CLpf00	0.0000
			 0, // CLpf01	0.0000
			 0, // CLpf02	0.0000
			 0, // CLpf03	0.0000
			 0	// CLpf04	0.0000
		  },	// CLpf
		  3,	// ACCYLpfFlt	 9 taps (fixed)
		  3,	// ACCCLpfFlt	 9 taps (fixed)
		  1,	// UseLpfY	1
		  1,	// StaticLUTEn	1
		  1,	// LACCEnable	1
		  20, // LACCTextureThresh	20
		  4,	// LACCDiffShift	4
		  4,	// LACCTextureOffset	4
		  255,	// LACCTextureGain	255
		  100,	// LACCSmoothThresh 100
		  0 // LACCSmoothness	0
	   }	// ACCHWParameters_high
	};
	
	
	VQ_ACC_Parameters_t ROM AccxvYCCVideo_Comp =
	{
	   {
		  sizeof(VQ_ACC_Parameters_t),
		  0, // STLAYER_VIDEO_ACC,
		  0
	   },
	   { // ACCSWParameters_high
		  0,	// HistStretchEnable	0
		  200,	// Cumulative_Black_Pixels	200
		  50, // Cumulative_White_Pixels	50
		  28, // Max_Black_Bin	28
		  188,	// Min_White_Bin	188
		  96, // ACC_Strength 96
		  { // WF
			 0, // WF00 0
			 0, // WF01 0
			 0, // WF02 0
			 0, // WF03 0
			 0, // WF04 0
			 0, // WF05 0
			 0, // WF06 0
			 0, // WF07 0
			 1, // WF08 1
			 2, // WF09 2
			 3, // WF10 3
			 3, // WF11 3
			 3, // WF12 3
			 3, // WF13 3
			 3, // WF14 3
			 3, // WF15 3
			 3, // WF16 3
			 3, // WF17 3
			 3, // WF18 3
			 3, // WF19 3
			 3, // WF20 3
			 3, // WF21 3
			 2, // WF22 2
			 1, // WF23 1
			 0, // WF24 0
			 0, // WF25 0
			 0, // WF26 0
			 0, // WF27 0
			 0, // WF28 0
			 0, // WF29 0
			 0, // WF30 0
			 0, // WF31 0
			 0, // WF32 0
			 0, // WF33 0
			 0, // WF34 0
			 0, // WF35 0
			 0, // WF36 0
			 0, // WF37 0
			 0, // WF38 0
			 0, // WF39 0
			 0, // WF40 0
			 0, // WF41 0
			 0, // WF42 0
			 0, // WF43 0
			 0, // WF44 0
			 0, // WF45 0
			 0, // WF46 0
			 0, // WF47 0
			 0, // WF48 0
			 0, // WF49 0
			 0, // WF50 0
			 0, // WF51 0
			 0, // WF52 0
			 0, // WF53 0
			 0, // WF54 0
			 0, // WF55 0
			 0, // WF56 0
			 0, // WF57 0
			 0, // WF58 0
			 0, // WF59 0
			 0, // WF60 0
			 0, // WF61 0
			 0, // WF62 0
			 0	// WF63 0
		  },	// WF
		  { // BinLim
			 324,	// BinLim00 324
			 324,	// BinLim01 324
			 324,	// BinLim02 324
			 333,	// BinLim03 333
			 342,	// BinLim04 342
			 351,	// BinLim05 351
			 358,	// BinLim06 358
			 364,	// BinLim07 364
			 370,	// BinLim08 370
			 376,	// BinLim09 376
			 381,	// BinLim10 381
			 385,	// BinLim11 385
			 390,	// BinLim12 390
			 394,	// BinLim13 394
			 397,	// BinLim14 397
			 398,	// BinLim15 398
			 399,	// BinLim16 399
			 400,	// BinLim17 400
			 401,	// BinLim18 401
			 402,	// BinLim19 402
			 403,	// BinLim20 403
			 404,	// BinLim21 404
			 400,	// BinLim22 400
			 392,	// BinLim23 392
			 383,	// BinLim24 383
			 375,	// BinLim25 375
			 372,	// BinLim26 372
			 374,	// BinLim27 374
			 376,	// BinLim28 376
			 378,	// BinLim29 378
			 378,	// BinLim30 378
			 376,	// BinLim31 376
			 374,	// BinLim32 374
			 372,	// BinLim33 372
			 370,	// BinLim34 370
			 368,	// BinLim35 368
			 366,	// BinLim36 366
			 364,	// BinLim37 364
			 364,	// BinLim38 364
			 365,	// BinLim39 365
			 366,	// BinLim40 366
			 367,	// BinLim41 367
			 366,	// BinLim42 366
			 362,	// BinLim43 362
			 358,	// BinLim44 358
			 354,	// BinLim45 354
			 350,	// BinLim46 350
			 346,	// BinLim47 346
			 342,	// BinLim48 342
			 338,	// BinLim49 338
			 333,	// BinLim50 333
			 326,	// BinLim51 326
			 320,	// BinLim52 320
			 313,	// BinLim53 313
			 312,	// BinLim54 312
			 315,	// BinLim55 315
			 318,	// BinLim56 318
			 321,	// BinLim57 321
			 321,	// BinLim58 321
			 318,	// BinLim59 318
			 315,	// BinLim60 315
			 312,	// BinLim61 312
			 312,	// BinLim62 312
			 312 // BinLim63 312
		  } // BinLim
	   },	// ACCSWParameters_high
	   { // ACCStaticLut_high
		  { // ACCStaticLut
			 1, // ACCStaticLut00	1
			 1, // ACCStaticLut01	1
			 1, // ACCStaticLut02	1
			 1, // ACCStaticLut03	1
			 1, // ACCStaticLut04	1
			 1, // ACCStaticLut05	1
			 1, // ACCStaticLut06	1
			 1, // ACCStaticLut07	1
			 1, // ACCStaticLut08	1
			 1, // ACCStaticLut09	1
			 1, // ACCStaticLut10	1
			 1, // ACCStaticLut11	1
			 1, // ACCStaticLut12	1
			 1, // ACCStaticLut13	1
			 1, // ACCStaticLut14	1
			 1, // ACCStaticLut15	1
			 1, // ACCStaticLut16	1
			 1, // ACCStaticLut17	1
			 1, // ACCStaticLut18	1
			 1, // ACCStaticLut19	1
			 1, // ACCStaticLut20	1
			 1, // ACCStaticLut21	1
			 1, // ACCStaticLut22	1
			 1, // ACCStaticLut23	1
			 1, // ACCStaticLut24	1
			 1, // ACCStaticLut25	1
			 1, // ACCStaticLut26	1
			 1, // ACCStaticLut27	1
			 1, // ACCStaticLut28	1
			 1, // ACCStaticLut29	1
			 1, // ACCStaticLut30	1
			 1, // ACCStaticLut31	1
			 1, // ACCStaticLut32	1
			 1, // ACCStaticLut33	1
			 1, // ACCStaticLut34	1
			 1, // ACCStaticLut35	1
			 1, // ACCStaticLut36	1
			 1, // ACCStaticLut37	1
			 1, // ACCStaticLut38	1
			 1, // ACCStaticLut39	1
			 1, // ACCStaticLut40	1
			 1, // ACCStaticLut41	1
			 1, // ACCStaticLut42	1
			 1, // ACCStaticLut43	1
			 1, // ACCStaticLut44	1
			 1, // ACCStaticLut45	1
			 1, // ACCStaticLut46	1
			 1, // ACCStaticLut47	1
			 1, // ACCStaticLut48	1
			 1, // ACCStaticLut49	1
			 1, // ACCStaticLut50	1
			 1, // ACCStaticLut51	1
			 1, // ACCStaticLut52	1
			 1, // ACCStaticLut53	1
			 1, // ACCStaticLut54	1
			 1, // ACCStaticLut55	1
			 1, // ACCStaticLut56	1
			 1, // ACCStaticLut57	1
			 1, // ACCStaticLut58	1
			 1, // ACCStaticLut59	1
			 1, // ACCStaticLut60	1
			 1, // ACCStaticLut61	1
			 1, // ACCStaticLut62	1
			 1	// ACCStaticLut63	1
		  } // ACCStaticLut
	   },	// ACCStaticLut_high
	   { // ACCHWParameters_high
		  0,	// IVP_IP_MEASURE_EN	0
		  0,	// IVP_IP_MEAS_SC_CHK_EN	0
		  0,	// IVP_IP_MEAS_SC_SOURCE	 External Detector
		  0,	// IVP_IP_MEAS_SC_THRESH	0
		  8,	// HpfAmpThreshY	8
		  20, // HpfAmpThreshUV 20
		  1,	// ClampBlack	1
		  1,	// LutTempFilterEnable	1
		  4,	// YclNlsatScale	4
		  0,	// YclNlsatEn	0
		  { // YclNlsatTable
			 16, // YclNlsatTable00 16
			 16, // YclNlsatTable01 16
			 16, // YclNlsatTable02 16
			 16, // YclNlsatTable03 16
			 16, // YclNlsatTable04 16
			 16, // YclNlsatTable05 16
			 16, // YclNlsatTable06 16
			 16 // YclNlsatTable07	16
		  },	// YclNlsatTable
		  1,	// YclUseKY 1
		  1,	// CompLutLpfEn 1
		  1,	// CheckScEnable	1
		  1,	// SceneChangeResetEnable	1
		  0,	// TfFrameNum	 000 = current frame only (no filtering)
		  1,	// YCLinkEnable 1
		  1,	// SceneChangeSource	 ACC measured scene change
		  5,	// LeftSlope	5
		  6,	// RightSlope	6
		  189,	// ScaleThresh	189
		  9,	// YCLinkGain	9
		  200,	// SceneChangeThreshold 200
		  0,	// XvYccEnable	0
		  { // YLpf
			 0, // YLpf00	0.0000
			 0, // YLpf01	0.0000
			 0, // YLpf02	0.0000
			 0, // YLpf03	0.0000
			 0	// YLpf04	0.0000
		  },	// YLpf
		  { // CLpf
			 0, // CLpf00	0.0000
			 0, // CLpf01	0.0000
			 0, // CLpf02	0.0000
			 0, // CLpf03	0.0000
			 0	// CLpf04	0.0000
		  },	// CLpf
		  3,	// ACCYLpfFlt	 9 taps (fixed)
		  3,	// ACCCLpfFlt	 9 taps (fixed)
		  1,	// UseLpfY	1
		  1,	// StaticLUTEn	1
		  1,	// LACCEnable	1
		  20, // LACCTextureThresh	20
		  4,	// LACCDiffShift	4
		  4,	// LACCTextureOffset	4
		  255,	// LACCTextureGain	255
		  100,	// LACCSmoothThresh 100
		  0 // LACCSmoothness	0
	   }	// ACCHWParameters_high
	};
	
#endif
	
	//******************************** END *********************************//
	

