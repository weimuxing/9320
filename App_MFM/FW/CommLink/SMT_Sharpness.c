#include <mem.h>
#include "System\All.h"
#include "CommLink\smt.h"
//#include "CommLink\SMT_Sharpness.h"

#ifdef USE_SMT


#define TS_Sharpness_DEBUG_MSG
#if defined(TS_Sharpness_DEBUG_MSG)
    #define dbgmsg(a,b) gm_Printf(a,b)
#else
    #define dbgmsg(a,b)
#endif

#define TS_Sharpness_VER 0

typedef enum
{
    TS_Sharpness_START,
    TS_Sharpness_STOP,
    TS_Sharpness_READ,
    TS_Sharpness_WRITE,
    TS_Sharpness_EN,
    TS_Sharpness_DISABLE
}TsSharpnessCmd_t;

const VQ_Sharpness_Parameters_t SharpnessDefaultVQTable =
{
    {
      //sizeof(STLAYER_TuningData_t) + sizeof(VIP_SharpnessDataRange_t) + sizeof(U8) + sizeof(SharpnessDefaultStep),
      sizeof(VQ_Sharpness_Parameters_t),
      0, // STLAYER_VIDEO_SHARPNESS,
      0
    },
    { // Data Range
        127,    // MHF_SHARPNESS_LUMA_MAX    127
        { // MainLumaHMax
            128,    // MHF_ENH_Y_LOSHOOT_TOL_MAX    128
            128,    // MHF_ENH_Y_LUSHOOT_TOL_MAX    128
            96,    // MHF_ENH_Y_SOSHOOT_TOL_MAX    96
			96,	// MHF_ENH_Y_SUSHOOT_TOL_MAX	96
			128,	// MHF_ENH_Y_LGAIN_MAX	128
			128,	// MHF_ENH_Y_SGAIN_MAX	128
			128,	// MHF_ENH_Y_FINALLGAIN_MAX	128
			96,	// MHF_ENH_Y_FINALSGAIN_MAX	96
			256,	// MHF_ENH_Y_FINALGAIN_MAX	256
			20,	// MHF_ENH_Y_DELTA_MAX	20
			0,	// MHF_ENH_Y_SLOPE_MAX	0
			64,	// MHF_ENH_Y_THRESH_MAX	64
			40,	// MHF_ENH_Y_HIGH_SLOPE_AGC_MAX	40
			40,	// MHF_ENH_Y_LOW_SLOPE_AGC_MAX	40
			200,	// MHF_ENH_Y_HIGH_THRESH_AGC_MAX	200
			50	// MHF_ENH_Y_LOW_THRESH_AGC_MAX	50
		},	// MainLumaHMax
		127,	// MVF_SHARPNESS_LUMA_MAX	127
		{ // MainLumaVMax
			128,	// MVF_ENH_Y_LOSHOOT_TOL_MAX	128
			128,	// MVF_ENH_Y_LUSHOOT_TOL_MAX	128
			96,	// MVF_ENH_Y_SOSHOOT_TOL_MAX	96
			96,	// MVF_ENH_Y_SUSHOOT_TOL_MAX	96
			128,	// MVF_ENH_Y_LGAIN_MAX	128
			128,	// MVF_ENH_Y_SGAIN_MAX	128
			128,	// MVF_ENH_Y_FINALLGAIN_MAX	128
			96,	// MVF_ENH_Y_FINALSGAIN_MAX	96
			256,	// MVF_ENH_Y_FINALGAIN_MAX	256
			20,	// MVF_ENH_Y_DELTA_MAX	20
			0,	// MVF_ENH_Y_SLOPE_MAX	0
			64,	// MVF_ENH_Y_THRESH_MAX	64
			40,	// MVF_ENH_Y_HIGH_SLOPE_AGC_MAX	40
			40,	// MVF_ENH_Y_LOW_SLOPE_AGC_MAX	40
			200,	// MVF_ENH_Y_HIGH_THRESH_AGC_MAX	200
			50	// MVF_ENH_Y_LOW_THRESH_AGC_MAX	50
		},	// MainLumaVMax
		127,	// MHF_SHARPNESS_CHROMA_MAX	127
		{ // MainChromaHMax
			16,	// MHF_ENH_UV_LOSHOOT_TOL_MAX	16
			16,	// MHF_ENH_UV_LUSHOOT_TOL_MAX	16
			16,	// MHF_ENH_UV_SOSHOOT_TOL_MAX	16
			16,	// MHF_ENH_UV_SUSHOOT_TOL_MAX	16
			64,	// MHF_ENH_UV_LGAIN_MAX	64
			128,	// MHF_ENH_UV_SGAIN_MAX	128
			96,	// MHF_ENH_UV_FINALLGAIN_MAX	96
			128,	// MHF_ENH_UV_FINALSGAIN_MAX	128
			256,	// MHF_ENH_UV_FINALGAIN_MAX	256
			20,	// MHF_ENH_UV_DELTA_MAX	20
			0,	// MHF_ENH_UV_SLOPE_MAX	0
			64,	// MHF_ENH_UV_THRESH_MAX	64
			40,	// MHF_ENH_UV_HIGH_SLOPE_AGC_MAX	40
			40,	// MHF_ENH_UV_LOW_SLOPE_AGC_MAX	40
			30,	// MHF_ENH_UV_HIGH_THRESH_AGC_MAX	30
			30	// MHF_ENH_UV_LOW_THRESH_AGC_MAX	30
		},	// MainChromaHMax
		127,	// MVF_SHARPNESS_CHROMA_MAX	127
		{ // MainChromaVMax
			16,	// MVF_ENH_UV_LOSHOOT_TOL_MAX	16
			16,	// MVF_ENH_UV_LUSHOOT_TOL_MAX	16
			16,	// MVF_ENH_UV_SOSHOOT_TOL_MAX	16
			16,	// MVF_ENH_UV_SUSHOOT_TOL_MAX	16
			96,	// MVF_ENH_UV_LGAIN_MAX	96
			128,	// MVF_ENH_UV_SGAIN_MAX	128
			128,	// MVF_ENH_UV_FINALLGAIN_MAX	128
			128,	// MVF_ENH_UV_FINALSGAIN_MAX	128
			256,	// MVF_ENH_UV_FINALGAIN_MAX	256
			20,	// MVF_ENH_UV_DELTA_MAX	20
			0,	// MVF_ENH_UV_SLOPE_MAX	0
			64,	// MVF_ENH_UV_THRESH_MAX	64
			40,	// MVF_ENH_UV_HIGH_SLOPE_AGC_MAX	40
			40,	// MVF_ENH_UV_LOW_SLOPE_AGC_MAX	40
			30,	// MVF_ENH_UV_HIGH_THRESH_AGC_MAX	30
			30	// MVF_ENH_UV_LOW_THRESH_AGC_MAX	30
        },    // MainChromaVMax
        { // MainNLHMax
            64,    // Threshold1    64
            15,    // Gain1    15
            200,    // Threshold2    200
            15    // Gain2    15
        },    // MainNLHMax
        { // MainNLVMax
            64,    // Threshold1    64
            15,    // Gain1    15
            200,    // Threshold2    200
            15    // Gain2    15
        },    // MainNLVMax
        { // MainNCHMax
            20    // MHF_NOISE_CORING_CTRL_THRESH_MAX    20
        },    // MainNCHMax
        { // MainNCVMax
            20    // MVF_NOISE_CORING_CTRL_THRESH_MAX    20
        },    // MainNCVMax
        0,    // MHF_SHARPNESS_LUMA_MIN    0
        { // MainLumaHMin
            0,    // MHF_ENH_Y_LOSHOOT_TOL_MIN    0
            0,    // MHF_ENH_Y_LUSHOOT_TOL_MIN    0
            0,    // MHF_ENH_Y_SOSHOOT_TOL_MIN    0
            0,    // MHF_ENH_Y_SUSHOOT_TOL_MIN    0
            0,    // MHF_ENH_Y_LGAIN_MIN    0
            0,    // MHF_ENH_Y_SGAIN_MIN    0
            0,    // MHF_ENH_Y_FINALLGAIN_MIN    0
            0,    // MHF_ENH_Y_FINALSGAIN_MIN    0
            0,    // MHF_ENH_Y_FINALGAIN_MIN    0
            20,    // MHF_ENH_Y_DELTA_MIN    20
            0,    // MHF_ENH_Y_SLOPE_MIN    0
            64,    // MHF_ENH_Y_THRESH_MIN    64
            1,    // MHF_ENH_Y_HIGH_SLOPE_AGC_MIN    1
            1,    // MHF_ENH_Y_LOW_SLOPE_AGC_MIN    1
            0,    // MHF_ENH_Y_HIGH_THRESH_AGC_MIN    0
            0    // MHF_ENH_Y_LOW_THRESH_AGC_MIN    0
        },    // MainLumaHMin
        0,    // MVF_SHARPNESS_LUMA_MIN    0
        { // MainLumaVMin
            0,    // MVF_ENH_Y_LOSHOOT_TOL_MIN    0
            0,    // MVF_ENH_Y_LUSHOOT_TOL_MIN    0
            0,    // MVF_ENH_Y_SOSHOOT_TOL_MIN    0
            0,    // MVF_ENH_Y_SUSHOOT_TOL_MIN    0
            0,    // MVF_ENH_Y_LGAIN_MIN    0
            0,    // MVF_ENH_Y_SGAIN_MIN    0
            0,    // MVF_ENH_Y_FINALLGAIN_MIN    0
            0,    // MVF_ENH_Y_FINALSGAIN_MIN    0
            0,    // MVF_ENH_Y_FINALGAIN_MIN    0
            20,    // MVF_ENH_Y_DELTA_MIN    20
            0,    // MVF_ENH_Y_SLOPE_MIN    0
            64,    // MVF_ENH_Y_THRESH_MIN    64
            1,    // MVF_ENH_Y_HIGH_SLOPE_AGC_MIN    1
            1,    // MVF_ENH_Y_LOW_SLOPE_AGC_MIN    1
            0,    // MVF_ENH_Y_HIGH_THRESH_AGC_MIN    0
            0    // MVF_ENH_Y_LOW_THRESH_AGC_MIN    0
        },    // MainLumaVMin
        0,    // MHF_SHARPNESS_CHROMA_MIN    0
        { // MainChromaHMin
            0,    // MHF_ENH_UV_LOSHOOT_TOL_MIN    0
            0,    // MHF_ENH_UV_LUSHOOT_TOL_MIN    0
            0,    // MHF_ENH_UV_SOSHOOT_TOL_MIN    0
            0,    // MHF_ENH_UV_SUSHOOT_TOL_MIN    0
            0,    // MHF_ENH_UV_LGAIN_MIN    0
            0,    // MHF_ENH_UV_SGAIN_MIN    0
            0,    // MHF_ENH_UV_FINALLGAIN_MIN    0
            0,    // MHF_ENH_UV_FINALSGAIN_MIN    0
            0,    // MHF_ENH_UV_FINALGAIN_MIN    0
            20,    // MHF_ENH_UV_DELTA_MIN    20
            0,    // MHF_ENH_UV_SLOPE_MIN    0
            64,    // MHF_ENH_UV_THRESH_MIN    64
            1,    // MHF_ENH_UV_HIGH_SLOPE_AGC_MIN    1
            1,    // MHF_ENH_UV_LOW_SLOPE_AGC_MIN    1
            0,    // MHF_ENH_UV_HIGH_THRESH_AGC_MIN    0
            0    // MHF_ENH_UV_LOW_THRESH_AGC_MIN    0
        },    // MainChromaHMin
        0,    // MVF_SHARPNESS_CHROMA_MIN    0
        { // MainChromaVMin
            0,    // MVF_ENH_UV_LOSHOOT_TOL_MIN    0
            0,    // MVF_ENH_UV_LUSHOOT_TOL_MIN    0
            0,    // MVF_ENH_UV_SOSHOOT_TOL_MIN    0
            0,    // MVF_ENH_UV_SUSHOOT_TOL_MIN    0
            0,    // MVF_ENH_UV_LGAIN_MIN    0
            0,    // MVF_ENH_UV_SGAIN_MIN    0
            0,    // MVF_ENH_UV_FINALLGAIN_MIN    0
            0,    // MVF_ENH_UV_FINALSGAIN_MIN    0
            0,    // MVF_ENH_UV_FINALGAIN_MIN    0
            20,    // MVF_ENH_UV_DELTA_MIN    20
            0,    // MVF_ENH_UV_SLOPE_MIN    0
            64,    // MVF_ENH_UV_THRESH_MIN    64
            1,    // MVF_ENH_UV_HIGH_SLOPE_AGC_MIN    1
            1,    // MVF_ENH_UV_LOW_SLOPE_AGC_MIN    1
            0,    // MVF_ENH_UV_HIGH_THRESH_AGC_MIN    0
            0    // MVF_ENH_UV_LOW_THRESH_AGC_MIN    0
        },    // MainChromaVMin
        { // MainNLHMin
            0,    // Threshold1    0
            0,    // Gain1    0
            100,    // Threshold2    100
            0    // Gain2    0
        },    // MainNLHMin
        { // MainNLVMin
            0,    // Threshold1    0
            0,    // Gain1    0
            100,    // Threshold2    100
            0    // Gain2    0
        },    // MainNLVMin
        { // MainNCHMin
            0    // MHF_NOISE_CORING_CTRL_THRESH_MIN    0
        },    // MainNCHMin
        { // MainNCVMin
            0    // MVF_NOISE_CORING_CTRL_THRESH_MIN    0
        }    // MainNCVMin
    }, // Data Range
	51, // Number of steps 51
	{ // Steps
		{ // Step_00
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			15,	// NoiseCoring	15
			{ // Luma
				50,	// OverShoot	50
				25,	// LargeGain	25
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				25,	// LargeGain	25
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			}	// Chroma
		}, // Step_00
		{ // Step_01
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			15,	// NoiseCoring	15
			{ // Luma
				50,	// OverShoot	50
				25,	// LargeGain	25
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				25,	// LargeGain	25
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			}	// Chroma
		}, // Step_01
		{ // Step_02
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			15,	// NoiseCoring	15
			{ // Luma
				50,	// OverShoot	50
				26,	// LargeGain	26
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				25,	// LargeGain	25
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			}	// Chroma
		}, // Step_02
		{ // Step_03
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			15,	// NoiseCoring	15
			{ // Luma
				50,	// OverShoot	50
				26,	// LargeGain	26
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				26,	// LargeGain	26
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			}	// Chroma
		}, // Step_03
		{ // Step_04
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			15,	// NoiseCoring	15
			{ // Luma
				50,	// OverShoot	50
				26,	// LargeGain	26
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				26,	// LargeGain	26
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			}	// Chroma
		}, // Step_04
		{ // Step_05
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			20,	// NoiseCoring	20
			{ // Luma
				50,	// OverShoot	50
				26,	// LargeGain	26
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				26,	// LargeGain	26
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			}	// Chroma
		}, // Step_05
		{ // Step_06
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			20,	// NoiseCoring	20
			{ // Luma
				50,	// OverShoot	50
				27,	// LargeGain	27
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				27,	// LargeGain	27
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			}	// Chroma
		}, // Step_06
		{ // Step_07
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			20,	// NoiseCoring	20
			{ // Luma
				50,	// OverShoot	50
				27,	// LargeGain	27
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				27,	// LargeGain	27
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			}	// Chroma
		}, // Step_07
		{ // Step_08
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			20,	// NoiseCoring	20
			{ // Luma
				50,	// OverShoot	50
				27,	// LargeGain	27
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				27,	// LargeGain	27
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			}	// Chroma
		}, // Step_08
		{ // Step_09
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			20,	// NoiseCoring	20
			{ // Luma
				50,	// OverShoot	50
				27,	// LargeGain	27
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				27,	// LargeGain	27
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			}	// Chroma
		}, // Step_09
		{ // Step_10
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			25,	// NoiseCoring	25
			{ // Luma
				50,	// OverShoot	50
				28,	// LargeGain	28
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				28,	// LargeGain	28
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			}	// Chroma
		}, // Step_10
		{ // Step_11
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			25,	// NoiseCoring	25
			{ // Luma
				50,	// OverShoot	50
				28,	// LargeGain	28
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				28,	// LargeGain	28
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			}	// Chroma
		}, // Step_11
		{ // Step_12
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			25,	// NoiseCoring	25
			{ // Luma
				50,	// OverShoot	50
				28,	// LargeGain	28
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				28,	// LargeGain	28
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			}	// Chroma
		}, // Step_12
		{ // Step_13
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			25,	// NoiseCoring	25
			{ // Luma
				50,	// OverShoot	50
				28,	// LargeGain	28
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				28,	// LargeGain	28
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			}	// Chroma
		}, // Step_13
		{ // Step_14
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			25,	// NoiseCoring	25
			{ // Luma
				50,	// OverShoot	50
				28,	// LargeGain	28
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				28,	// LargeGain	28
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			}	// Chroma
		}, // Step_14
		{ // Step_15
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			30,	// NoiseCoring	30
			{ // Luma
				50,	// OverShoot	50
				29,	// LargeGain	29
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				28,	// LargeGain	28
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			}	// Chroma
		}, // Step_15
		{ // Step_16
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			30,	// NoiseCoring	30
			{ // Luma
				50,	// OverShoot	50
				29,	// LargeGain	29
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				29,	// LargeGain	29
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			}	// Chroma
		}, // Step_16
		{ // Step_17
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			30,	// NoiseCoring	30
			{ // Luma
				50,	// OverShoot	50
				29,	// LargeGain	29
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				29,	// LargeGain	29
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			}	// Chroma
		}, // Step_17
		{ // Step_18
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			30,	// NoiseCoring	30
			{ // Luma
				50,	// OverShoot	50
				29,	// LargeGain	29
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				29,	// LargeGain	29
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			}	// Chroma
		}, // Step_18
		{ // Step_19
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			30,	// NoiseCoring	30
			{ // Luma
				50,	// OverShoot	50
				29,	// LargeGain	29
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				29,	// LargeGain	29
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			}	// Chroma
		}, // Step_19
		{ // Step_20
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			35,	// NoiseCoring	35
			{ // Luma
				50,	// OverShoot	50
				30,	// LargeGain	30
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				30,	// LargeGain	30
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			}	// Chroma
		}, // Step_20
		{ // Step_21
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			35,	// NoiseCoring	35
			{ // Luma
				50,	// OverShoot	50
				30,	// LargeGain	30
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				30,	// LargeGain	30
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			}	// Chroma
		}, // Step_21
		{ // Step_22
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			35,	// NoiseCoring	35
			{ // Luma
				50,	// OverShoot	50
				30,	// LargeGain	30
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				30,	// LargeGain	30
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			}	// Chroma
		}, // Step_22
		{ // Step_23
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			35,	// NoiseCoring	35
			{ // Luma
				50,	// OverShoot	50
				30,	// LargeGain	30
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				30,	// LargeGain	30
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			}	// Chroma
		}, // Step_23
		{ // Step_24
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			35,	// NoiseCoring	35
			{ // Luma
				50,	// OverShoot	50
				31,	// LargeGain	31
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				31,	// LargeGain	31
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			}	// Chroma
		}, // Step_24
		{ // Step_25
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			40,	// NoiseCoring	40
			{ // Luma
				50,	// OverShoot	50
				31,	// LargeGain	31
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				31,	// LargeGain	31
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			}	// Chroma
		}, // Step_25
		{ // Step_26
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			40,	// NoiseCoring	40
			{ // Luma
				50,	// OverShoot	50
				32,	// LargeGain	32
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				32,	// LargeGain	32
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			}	// Chroma
		}, // Step_26
		{ // Step_27
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			40,	// NoiseCoring	40
			{ // Luma
				50,	// OverShoot	50
				32,	// LargeGain	32
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				32,	// LargeGain	32
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			}	// Chroma
		}, // Step_27
		{ // Step_28
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			40,	// NoiseCoring	40
			{ // Luma
				50,	// OverShoot	50
				33,	// LargeGain	33
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				33,	// LargeGain	33
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			}	// Chroma
		}, // Step_28
		{ // Step_29
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			40,	// NoiseCoring	40
			{ // Luma
				50,	// OverShoot	50
				34,	// LargeGain	34
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				34,	// LargeGain	34
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			}	// Chroma
		}, // Step_29
		{ // Step_30
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			45,	// NoiseCoring	45
			{ // Luma
				50,	// OverShoot	50
				35,	// LargeGain	35
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				35,	// LargeGain	35
				50,	// SmallGain	50
				50,	// FinalGain	50
				50	// AGC	50
			}	// Chroma
		}, // Step_30
		{ // Step_31
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			45,	// NoiseCoring	45
			{ // Luma
				50,	// OverShoot	50
				35,	// LargeGain	35
				50,	// SmallGain	50
				51,	// FinalGain	51
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				35,	// LargeGain	35
				50,	// SmallGain	50
				51,	// FinalGain	51
				50	// AGC	50
			}	// Chroma
		}, // Step_31
		{ // Step_32
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			45,	// NoiseCoring	45
			{ // Luma
				50,	// OverShoot	50
				36,	// LargeGain	36
				50,	// SmallGain	50
				52,	// FinalGain	52
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				36,	// LargeGain	36
				50,	// SmallGain	50
				52,	// FinalGain	52
				50	// AGC	50
			}	// Chroma
		}, // Step_32
		{ // Step_33
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			45,	// NoiseCoring	45
			{ // Luma
				50,	// OverShoot	50
				37,	// LargeGain	37
				50,	// SmallGain	50
				53,	// FinalGain	53
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				37,	// LargeGain	37
				50,	// SmallGain	50
				53,	// FinalGain	53
				50	// AGC	50
			}	// Chroma
		}, // Step_33
		{ // Step_34
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			45,	// NoiseCoring	45
			{ // Luma
				50,	// OverShoot	50
				38,	// LargeGain	38
				50,	// SmallGain	50
				54,	// FinalGain	54
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				38,	// LargeGain	38
				50,	// SmallGain	50
				54,	// FinalGain	54
				50	// AGC	50
			}	// Chroma
		}, // Step_34
		{ // Step_35
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			50,	// NoiseCoring	50
			{ // Luma
				50,	// OverShoot	50
				39,	// LargeGain	39
				50,	// SmallGain	50
				55,	// FinalGain	55
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				39,	// LargeGain	39
				50,	// SmallGain	50
				55,	// FinalGain	55
				50	// AGC	50
			}	// Chroma
		}, // Step_35
		{ // Step_36
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			50,	// NoiseCoring	50
			{ // Luma
				50,	// OverShoot	50
				40,	// LargeGain	40
				50,	// SmallGain	50
				56,	// FinalGain	56
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				40,	// LargeGain	40
				50,	// SmallGain	50
				56,	// FinalGain	56
				50	// AGC	50
			}	// Chroma
		}, // Step_36
		{ // Step_37
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			50,	// NoiseCoring	50
			{ // Luma
				50,	// OverShoot	50
				41,	// LargeGain	41
				50,	// SmallGain	50
				57,	// FinalGain	57
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				41,	// LargeGain	41
				50,	// SmallGain	50
				57,	// FinalGain	57
				50	// AGC	50
			}	// Chroma
		}, // Step_37
		{ // Step_38
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			50,	// NoiseCoring	50
			{ // Luma
				50,	// OverShoot	50
				42,	// LargeGain	42
				50,	// SmallGain	50
				58,	// FinalGain	58
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				42,	// LargeGain	42
				50,	// SmallGain	50
				58,	// FinalGain	58
				50	// AGC	50
			}	// Chroma
		}, // Step_38
		{ // Step_39
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			50,	// NoiseCoring	50
			{ // Luma
				50,	// OverShoot	50
				43,	// LargeGain	43
				50,	// SmallGain	50
				59,	// FinalGain	59
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				43,	// LargeGain	43
				50,	// SmallGain	50
				59,	// FinalGain	59
				50	// AGC	50
			}	// Chroma
		}, // Step_39
		{ // Step_40
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			55,	// NoiseCoring	55
			{ // Luma
				50,	// OverShoot	50
				44,	// LargeGain	44
				50,	// SmallGain	50
				60,	// FinalGain	60
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				44,	// LargeGain	44
				50,	// SmallGain	50
				60,	// FinalGain	60
				50	// AGC	50
			}	// Chroma
		}, // Step_40
		{ // Step_41
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			55,	// NoiseCoring	55
			{ // Luma
				50,	// OverShoot	50
				44,	// LargeGain	44
				50,	// SmallGain	50
				61,	// FinalGain	61
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				44,	// LargeGain	44
				50,	// SmallGain	50
				61,	// FinalGain	61
				50	// AGC	50
			}	// Chroma
		}, // Step_41
		{ // Step_42
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			55,	// NoiseCoring	55
			{ // Luma
				50,	// OverShoot	50
				45,	// LargeGain	45
				50,	// SmallGain	50
				62,	// FinalGain	62
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				45,	// LargeGain	45
				50,	// SmallGain	50
				62,	// FinalGain	62
				50	// AGC	50
			}	// Chroma
		}, // Step_42
		{ // Step_43
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			55,	// NoiseCoring	55
			{ // Luma
				50,	// OverShoot	50
				46,	// LargeGain	46
				50,	// SmallGain	50
				63,	// FinalGain	63
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				46,	// LargeGain	46
				50,	// SmallGain	50
				63,	// FinalGain	63
				50	// AGC	50
			}	// Chroma
		}, // Step_43
		{ // Step_44
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			55,	// NoiseCoring	55
			{ // Luma
				50,	// OverShoot	50
				47,	// LargeGain	47
				50,	// SmallGain	50
				64,	// FinalGain	64
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				47,	// LargeGain	47
				50,	// SmallGain	50
				64,	// FinalGain	64
				50	// AGC	50
			}	// Chroma
		}, // Step_44
		{ // Step_45
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			60,	// NoiseCoring	60
			{ // Luma
				50,	// OverShoot	50
				47,	// LargeGain	47
				50,	// SmallGain	50
				65,	// FinalGain	65
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				47,	// LargeGain	47
				50,	// SmallGain	50
				65,	// FinalGain	65
				50	// AGC	50
			}	// Chroma
		}, // Step_45
		{ // Step_46
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			60,	// NoiseCoring	60
			{ // Luma
				50,	// OverShoot	50
				48,	// LargeGain	48
				50,	// SmallGain	50
				66,	// FinalGain	66
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				48,	// LargeGain	48
				50,	// SmallGain	50
				66,	// FinalGain	66
				50	// AGC	50
			}	// Chroma
		}, // Step_46
		{ // Step_47
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			60,	// NoiseCoring	60
			{ // Luma
				50,	// OverShoot	50
				48,	// LargeGain	48
				50,	// SmallGain	50
				67,	// FinalGain	67
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				48,	// LargeGain	48
				50,	// SmallGain	50
				67,	// FinalGain	67
				50	// AGC	50
			}	// Chroma
		}, // Step_47
		{ // Step_48
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			60,	// NoiseCoring	60
			{ // Luma
				50,	// OverShoot	50
				49,	// LargeGain	49
				50,	// SmallGain	50
				68,	// FinalGain	68
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				49,	// LargeGain	49
				50,	// SmallGain	50
				68,	// FinalGain	68
				50	// AGC	50
			}	// Chroma
		}, // Step_48
		{ // Step_49
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			60,	// NoiseCoring	60
			{ // Luma
				50,	// OverShoot	50
				50,	// LargeGain	50
				50,	// SmallGain	50
				69,	// FinalGain	69
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				50,	// LargeGain	50
				50,	// SmallGain	50
				69,	// FinalGain	69
				50	// AGC	50
			}	// Chroma
		}, // Step_49
		{ // Step_50
			50,	// NonLinear_Thresh1	50
			50,	// NonLinear_Gain1	50
			50,	// NonLinear_Thresh2	50
			50,	// NonLinear_Gain2	50
			60,	// NoiseCoring	60
			{ // Luma
				50,	// OverShoot	50
				50,	// LargeGain	50
				50,	// SmallGain	50
				70,	// FinalGain	70
				50	// AGC	50
			},	// Luma
			{ // Chroma
				50,	// OverShoot	50
				50,	// LargeGain	50
				50,	// SmallGain	50
				70,	// FinalGain	70
				50	// AGC	50
			}	// Chroma
		} // Step_50
	} // Steps
};

VQ_Sharpness_Parameters_t SMT_RAM VQ_Sharpness_Para_TEMP;

#pragma argsused
BYTE Sharpness_MsgHandler(MsgPacket_t *message, BYTE comm_use)
{
	BYTE* Bp_msg = (BYTE *)(message->msgpkt);

	//get pointer to output message length setting 
	BYTE* Bp_len=GetOutMsgLenPtr();
	BYTE* Bp_out= GetOutMsgBufPtr();

   BYTE B_TransId;
	BYTE B_ContextStatus;
	BYTE far * Bp_Context = NULL_PTR;
	WORD W_ContextCnt = 0;
	WORD W_CurrentCmd = 0xFFFF;   

   B_TransId = message->control;

	// get buffer pointer from SMT driver
	B_ContextStatus = SMT_GetBuffer( B_TransId, (BYTE far * far *)&Bp_Context, SMT_MAX_BUF_SIZE);

	// If B_ContextStatus is FIRST_REQUEST clear Bp_Context buffer
	if ( B_ContextStatus == FIRST_REQUEST )
	{
		// Clear Buffer
		for( W_ContextCnt = 0; W_ContextCnt < SMT_MAX_BUF_SIZE; W_ContextCnt++ )
			*(Bp_Context + W_ContextCnt) = 0;
		
		W_CurrentCmd = (Bp_msg[1] << 8) + Bp_msg[0];
		
		// Save command length
		*(Bp_Context + 0) = message->len - SMT_HEADER_SIZE - CHECKSUM_SIZE;
		//*(Bp_Context + 1) = 0;

		// Step for Read, Index for Write
		//*(Bp_Context + 2) = 0;
		//*(Bp_Context + 3) = 0;

		for(W_ContextCnt = 0; W_ContextCnt < *(Bp_Context + 0); W_ContextCnt++)
		{
			*(Bp_Context + 4 + W_ContextCnt) = Bp_msg[W_ContextCnt];
		}
      gm_Printf("Sharpness_MsgHandler:FIRST_REQUEST %d", W_CurrentCmd);
      gm_Printf("command length = %d", *(Bp_Context + 0));
	}
	else if ( B_ContextStatus == NEXT_REQUEST )
	{
		WORD W_Temp;
		
		W_CurrentCmd = ((*(Bp_Context + 5))<<8) + (*(Bp_Context + 4));

		// Previou data count
		W_Temp = (*(Bp_Context + 1) << 8) + *(Bp_Context + 0);

		// Save data into buffer
		for(W_ContextCnt = 0; W_ContextCnt < (message->len - SMT_HEADER_SIZE - CHECKSUM_SIZE); W_ContextCnt++)
		{
			*(Bp_Context + 4 + W_Temp + W_ContextCnt) = Bp_msg[W_ContextCnt];
		}

		// Update command length
		// W_Temp = (*(Bp_Context + 1) << 8) + *(Bp_Context + 0) + message->len - SMT_HEADER_SIZE - CHECKSUM_SIZE;
		W_Temp += message->len - SMT_HEADER_SIZE - CHECKSUM_SIZE;
		*(Bp_Context + 0) = (BYTE)(W_Temp&0xFF);
		*(Bp_Context + 1) = (BYTE)((W_Temp >> 8)&0xFF);
		gm_Printf("data size = %d", (((WORD)*(Bp_Context + 1))<<8)|(*(Bp_Context + 0)));	
	}
	else if (B_ContextStatus == NO_MEMORY) // do we have valid allocated memory ?
	{
		// send replay
		Bp_msg[0] = 6;	   // message length
		Bp_msg[1] = CMD_SMTProtocol;   // GProbe SMT comand
		//Bp_msg[2] = EvOdd;   // return even/odd flag
		Bp_msg[3] = B_TransId;
		Bp_msg[4] = TS_ERR;

      if((comm_use == I2C_SLAVE1_USE)||(comm_use == I2C_SLAVE2_USE)
         ||(comm_use == I2C_SLAVE3_USE)||(comm_use == I2C_SLAVE4_USE))
      {
         SLAVE_CH B_SlaveCh = comm_use - I2C_SLAVE1_USE;
         I2C_SLAVE_TYPE far* Sp_I2CSlavePtr = Sp_I2CSlavePtrGlobal[B_SlaveCh];
         _fmemcpy(&Sp_I2CSlavePtr->Buffer[Start_of_GprobeCmd_in_DDC2BiMsg], Bp_msg, (OutMsg.len - 1));
         gm_WriteCmdPkt((BYTE far*)&Sp_I2CSlavePtr->Buffer[Start_of_GprobeCmd_in_DDC2BiMsg], comm_use);
      }
      else
      {
			gm_WriteCmdPkt((BYTE far*)Bp_msg, comm_use);
      }

		gm_Printf("NO_MEMORYNO_MEMORY", 0);
		return PORT_MSG_HNDLR_SELFCARED;//ERR_PARAM;
	}
   
	if(W_CurrentCmd == TS_Sharpness_START)
	{
        Bp_out[0] = TS_OK;
        Bp_out[1] = TS_Sharpness_VER;
        *Bp_len = 2;
        dbgmsg("TS_Sharpness_START %s\n","OK");
	}
	else if(W_CurrentCmd == TS_Sharpness_STOP)
	{
        Bp_out[0] = TS_OK;
        *Bp_len = 1;	
		dbgmsg("TS_Sharpness_STOP", 0);	
	}
	else if(W_CurrentCmd == TS_Sharpness_READ)
	{
	   BYTE B_Step = *(Bp_Context + 2);
      VQ_Sharpness_Parameters_t VQ_Sharpness_Para_Buff;
      
      union
      {
         WORD  w;
         BYTE  b[2];
      } WordVal;      
      union
      {
         DWORD  dw;
         BYTE  b[4];
      } DWordVal;

      if(VQ_Sharpness_Para_TEMP.NumOfSteps != 0)
      {
         VQ_Sharpness_Para_Buff = VQ_Sharpness_Para_TEMP;
      }
      else
      {
         VQ_Sharpness_Para_Buff = SharpnessDefaultVQTable;
      }

      // Header + Max Range
      if ( B_ContextStatus == FIRST_REQUEST )
		{
		   BYTE B_Counter = 0;
      
   		Bp_out[B_Counter++] = TS_OK; // 1

   		// size
   		DWordVal.dw = 129 + 15*VQ_Sharpness_Para_Buff.NumOfSteps;
   		Bp_out[B_Counter++] = DWordVal.b[0]; // 2
   		Bp_out[B_Counter++] = DWordVal.b[1]; // 3
   		Bp_out[B_Counter++] = DWordVal.b[2]; // 4
   		Bp_out[B_Counter++] = DWordVal.b[3]; // 5         

   		// signature
   		Bp_out[B_Counter++] = 24; // 6
   		Bp_out[B_Counter++] = 0; // 7
   		Bp_out[B_Counter++] = 0; // 8
   		Bp_out[B_Counter++] = 0; // 9

   		// version
   		Bp_out[B_Counter++] = 0; // 10
   		Bp_out[B_Counter++] = 0; // 11
   		Bp_out[B_Counter++] = 0; // 12
   		Bp_out[B_Counter++] = 0; // 13

         // MHF_SHARPNESS_LUMA	2 X 1 (instance x size)         
         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainLumaHGainMax; // 14
         
         // MHF_ENH_Y_LOSHOOT_TOL	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaHMax.LOSHOOT_TOL_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 15
         Bp_out[B_Counter++] = WordVal.b[1]; // 16

         // MHF_ENH_Y_LUSHOOT_TOL	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaHMax.LUSHOOT_TOL_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 17
         Bp_out[B_Counter++] = WordVal.b[1]; // 18

         // MHF_ENH_Y_SOSHOOT_TOL	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaHMax.SOSHOOT_TOL_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 19
         Bp_out[B_Counter++] = WordVal.b[1]; // 20

         // MHF_ENH_Y_SUSHOOT_TOL	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaHMax.SUSHOOT_TOL_Value;         
         Bp_out[B_Counter++] = WordVal.b[0]; // 21
         Bp_out[B_Counter++] = WordVal.b[1]; // 22

         // MHF_ENH_Y_LGAIN	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaHMax.LGAIN_Value;         
         Bp_out[B_Counter++] = WordVal.b[0]; // 23
         Bp_out[B_Counter++] = WordVal.b[1]; // 24

         // MHF_ENH_Y_SGAIN	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaHMax.SGAIN_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 25
         Bp_out[B_Counter++] = WordVal.b[1]; // 26
         
         // MHF_ENH_Y_FINALLGAIN	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaHMax.FINALLGAIN_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 27
         Bp_out[B_Counter++] = WordVal.b[1]; // 28

         // MHF_ENH_Y_FINALSGAIN	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaHMax.FINALSGAIN_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 29
         Bp_out[B_Counter++] = WordVal.b[1]; // 30

         // MHF_ENH_Y_FINALGAIN	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaHMax.FINALGAIN_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 31
         Bp_out[B_Counter++] = WordVal.b[1]; // 32

         // MHF_ENH_Y_DELTA	2 X 1                  
         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainLumaHMax.DELTA_Value; // 33
         
         // MHF_ENH_Y_SLOPE	2 X 1
         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainLumaHMax.SLOPE_Value; // 34

         // MHF_ENH_Y_THRESH	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaHMax.THRESH_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 35
         Bp_out[B_Counter++] = WordVal.b[1]; // 36

         // MHF_ENH_Y_HIGH_SLOPE_AGC	2 X 1
         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainLumaHMax.HIGH_SLOPE_AGC_Value; // 37

         // MHF_ENH_Y_LOW_SLOPE_AGC	2 X 1
         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainLumaHMax.LOW_SLOPE_AGC_Value; // 38

         // MHF_ENH_Y_HIGH_THRESH_AGC	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaHMax.HIGH_THRESH_AGC_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 39
         Bp_out[B_Counter++] = WordVal.b[1]; // 40

         // MHF_ENH_Y_LOW_THRESH_AGC	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaHMax.LOW_THRESH_AGC_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 41
         Bp_out[B_Counter++] = WordVal.b[1]; // 42

         // MVF_SHARPNESS_LUMA	2 X 1 (instance x size)
         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainLumaVGainMax;

         // MVF_ENH_Y_LOSHOOT_TOL	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaVMax.LOSHOOT_TOL_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 43
         Bp_out[B_Counter++] = WordVal.b[1]; // 44
         
         // MVF_ENH_Y_LUSHOOT_TOL	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaVMax.LUSHOOT_TOL_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 45
         Bp_out[B_Counter++] = WordVal.b[1]; // 46

         // MVF_ENH_Y_SOSHOOT_TOL	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaVMax.SOSHOOT_TOL_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 47
         Bp_out[B_Counter++] = WordVal.b[1]; // 48

         // MVF_ENH_Y_SUSHOOT_TOL	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaVMax.SUSHOOT_TOL_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 49
         Bp_out[B_Counter++] = WordVal.b[1]; // 50

         // MVF_ENH_Y_LGAIN	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaVMax.LGAIN_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 51
         Bp_out[B_Counter++] = WordVal.b[1]; // 52

         // MVF_ENH_Y_SGAIN	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaVMax.SGAIN_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 53
         Bp_out[B_Counter++] = WordVal.b[1]; // 54

         // MVF_ENH_Y_FINALLGAIN	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaVMax.FINALLGAIN_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 55
         Bp_out[B_Counter++] = WordVal.b[1]; // 56

         // MVF_ENH_Y_FINALSGAIN	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaVMax.FINALSGAIN_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 57
         Bp_out[B_Counter++] = WordVal.b[1]; // 58

         // MVF_ENH_Y_FINALGAIN	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaVMax.FINALGAIN_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 59
         Bp_out[B_Counter++] = WordVal.b[1]; // 60

         // MVF_ENH_Y_DELTA	2 X 1                  
         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainLumaVMax.DELTA_Value; // 61
         
         // MVF_ENH_Y_SLOPE	2 X 1
         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainLumaVMax.SLOPE_Value; // 62

         // MVF_ENH_Y_THRESH	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaVMax.THRESH_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 63
         Bp_out[B_Counter++] = WordVal.b[1]; // 64

         // MVF_ENH_Y_HIGH_SLOPE_AGC	2 X 1
         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainLumaVMax.HIGH_SLOPE_AGC_Value; // 65

         // MVF_ENH_Y_LOW_SLOPE_AGC	2 X 1
         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainLumaVMax.LOW_SLOPE_AGC_Value; // 66

         // MVF_ENH_Y_HIGH_THRESH_AGC	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaVMax.HIGH_THRESH_AGC_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 67
         Bp_out[B_Counter++] = WordVal.b[1]; // 68

         // MVF_ENH_Y_LOW_THRESH_AGC	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaVMax.LOW_THRESH_AGC_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 69
         Bp_out[B_Counter++] = WordVal.b[1]; // 70

         // MHF_SHARPNESS_CHROMA	2 X 1 (instance x size)         
         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainChromaHGainMax; // 71
         
         // MHF_ENH_C_LOSHOOT_TOL	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaHMax.LOSHOOT_TOL_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 72
         Bp_out[B_Counter++] = WordVal.b[1]; // 73

         // MHF_ENH_C_LUSHOOT_TOL	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaHMax.LUSHOOT_TOL_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 74
         Bp_out[B_Counter++] = WordVal.b[1]; // 75

         // MHF_ENH_C_SOSHOOT_TOL	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaHMax.SOSHOOT_TOL_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 76
         Bp_out[B_Counter++] = WordVal.b[1]; // 77

         // MHF_ENH_C_SUSHOOT_TOL	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaHMax.SUSHOOT_TOL_Value;         
         Bp_out[B_Counter++] = WordVal.b[0]; // 78
         Bp_out[B_Counter++] = WordVal.b[1]; // 79

         // MHF_ENH_C_LGAIN	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaHMax.LGAIN_Value;         
         Bp_out[B_Counter++] = WordVal.b[0]; // 80
         Bp_out[B_Counter++] = WordVal.b[1]; // 81

         // MHF_ENH_C_SGAIN	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaHMax.SGAIN_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 82
         Bp_out[B_Counter++] = WordVal.b[1]; // 83
         
         // MHF_ENH_C_FINALLGAIN	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaHMax.FINALLGAIN_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 84
         Bp_out[B_Counter++] = WordVal.b[1]; // 85

         // MHF_ENH_C_FINALSGAIN	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaHMax.FINALSGAIN_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 86
         Bp_out[B_Counter++] = WordVal.b[1]; // 87

         // MHF_ENH_C_FINALGAIN	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaHMax.FINALGAIN_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 88
         Bp_out[B_Counter++] = WordVal.b[1]; // 89

         // MHF_ENH_C_DELTA	2 X 1                  
         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainChromaHMax.DELTA_Value; // 90
         
         // MHF_ENH_C_SLOPE	2 X 1
         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainChromaHMax.SLOPE_Value; // 91

         // MHF_ENH_C_THRESH	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaHMax.THRESH_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 92
         Bp_out[B_Counter++] = WordVal.b[1]; // 93

         // MHF_ENH_C_HIGH_SLOPE_AGC	2 X 1
         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainChromaHMax.HIGH_SLOPE_AGC_Value; // 94

         // MHF_ENH_C_LOW_SLOPE_AGC	2 X 1
         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainChromaHMax.LOW_SLOPE_AGC_Value; // 95

         // MHF_ENH_C_HIGH_THRESH_AGC	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaHMax.HIGH_THRESH_AGC_Value; 
         Bp_out[B_Counter++] = WordVal.b[0]; // 96
         Bp_out[B_Counter++] = WordVal.b[1]; // 97

         // MHF_ENH_C_LOW_THRESH_AGC	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaHMax.LOW_THRESH_AGC_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 98
         Bp_out[B_Counter++] = WordVal.b[1]; // 99

         // MVF_SHARPNESS_Chroma	2 X 1 (instance x size)
         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainChromaVGainMax; // 100

         // MVF_ENH_C_LOSHOOT_TOL	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaVMax.LOSHOOT_TOL_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 101
         Bp_out[B_Counter++] = WordVal.b[1]; // 102
         
         // MVF_ENH_C_LUSHOOT_TOL	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaVMax.LUSHOOT_TOL_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 103
         Bp_out[B_Counter++] = WordVal.b[1]; // 104

         // MVF_ENH_C_SOSHOOT_TOL	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaVMax.SOSHOOT_TOL_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 105
         Bp_out[B_Counter++] = WordVal.b[1]; // 106

         // MVF_ENH_C_SUSHOOT_TOL	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaVMax.SUSHOOT_TOL_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 107
         Bp_out[B_Counter++] = WordVal.b[1]; // 108

         // MVF_ENH_C_LGAIN	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaVMax.LGAIN_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 109
         Bp_out[B_Counter++] = WordVal.b[1]; // 110

         // MVF_ENH_C_SGAIN	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaVMax.SGAIN_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 111
         Bp_out[B_Counter++] = WordVal.b[1]; // 112

         // MVF_ENH_C_FINALLGAIN	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaVMax.FINALLGAIN_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 113
         Bp_out[B_Counter++] = WordVal.b[1]; // 114

         // MVF_ENH_C_FINALSGAIN	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaVMax.FINALSGAIN_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 115
         Bp_out[B_Counter++] = WordVal.b[1]; // 116

         // MVF_ENH_C_FINALGAIN	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaVMax.FINALGAIN_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 117
         Bp_out[B_Counter++] = WordVal.b[1]; // 118

			*Bp_len = B_Counter;

         gm_Printf("0 B_Counter = %d", B_Counter);

			// More Data ...
			MoreData(1);		
			
			dbgmsg("Sharpness_MsgHandler: FIRST_REQUEST \n", 0);         
      }
      // Min Range + Step Number
      else if(*(Bp_Context + 2) == 0)
      {
         BYTE B_Counter = 0;
         
         // MVF_ENH_C_DELTA	2 X 1                  
         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainChromaVMax.DELTA_Value; // 1
         
         // MVF_ENH_C_SLOPE	2 X 1
         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainChromaVMax.SLOPE_Value; // 2

         // MVF_ENH_C_THRESH	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaVMax.THRESH_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 3
         Bp_out[B_Counter++] = WordVal.b[1]; // 4

         // MVF_ENH_C_HIGH_SLOPE_AGC	2 X 1
         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainChromaVMax.HIGH_SLOPE_AGC_Value; // 5

         // MVF_ENH_C_LOW_SLOPE_AGC	2 X 1
         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainChromaVMax.LOW_SLOPE_AGC_Value; // 6

         // MVF_ENH_C_HIGH_THRESH_AGC	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaVMax.HIGH_THRESH_AGC_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 7
         Bp_out[B_Counter++] = WordVal.b[1]; // 8

         // MVF_ENH_C_LOW_THRESH_AGC	2 X 2
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaVMax.LOW_THRESH_AGC_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 9
         Bp_out[B_Counter++] = WordVal.b[1]; // 10

         // MainNLHMax
         // WORD Threshold1;
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainNLHMax.Threshold1;
         Bp_out[B_Counter++] = WordVal.b[0]; // 11
         Bp_out[B_Counter++] = WordVal.b[1]; // 12

         // BYTE Gain1;
         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainNLHMax.Gain1; // 13
         
         // WORD Threshold2;
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainNLHMax.Threshold2;
         Bp_out[B_Counter++] = WordVal.b[0]; // 14
         Bp_out[B_Counter++] = WordVal.b[1]; // 15

         // BYTE Gain2;
         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainNLHMax.Gain2; // 16

         // MainNLVMax
         // WORD Threshold1;
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainNLVMax.Threshold1;
         Bp_out[B_Counter++] = WordVal.b[0]; // 17
         Bp_out[B_Counter++] = WordVal.b[1]; // 18

         // BYTE Gain1;
         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainNLVMax.Gain1; // 19
         
         // WORD Threshold2;
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainNLVMax.Threshold2;
         Bp_out[B_Counter++] = WordVal.b[0]; // 20
         Bp_out[B_Counter++] = WordVal.b[1]; // 21

         // BYTE Gain2;
         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainNLVMax.Gain2; // 22

         // MainNCHMax
         // BYTE Threshold1;
         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainNCHMax.Threshold1; // 23

         // MainNCVMax
         // BYTE Threshold1;
         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainNCVMax.Threshold1; // 24

         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainLumaHGainMin; // 25

         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaHMin.LOSHOOT_TOL_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 26
         Bp_out[B_Counter++] = WordVal.b[1]; // 27


         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaHMin.LUSHOOT_TOL_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 28
         Bp_out[B_Counter++] = WordVal.b[1]; // 29        
         

         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaHMin.SOSHOOT_TOL_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 30
         Bp_out[B_Counter++] = WordVal.b[1]; // 31
         

         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaHMin.SUSHOOT_TOL_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 32
         Bp_out[B_Counter++] = WordVal.b[1]; // 33
         

         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaHMin.LGAIN_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 34
         Bp_out[B_Counter++] = WordVal.b[1]; // 35
         

         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaHMin.SGAIN_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 36
         Bp_out[B_Counter++] = WordVal.b[1]; // 37        
         

         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaHMin.FINALLGAIN_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 38
         Bp_out[B_Counter++] = WordVal.b[1]; // 39
         

         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaHMin.FINALSGAIN_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 40
         Bp_out[B_Counter++] = WordVal.b[1]; // 41
         

         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaHMin.FINALGAIN_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 42
         Bp_out[B_Counter++] = WordVal.b[1]; // 43
         

         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainLumaHMin.DELTA_Value; // 44
         

         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainLumaHMin.SLOPE_Value; // 45
         

         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaHMin.THRESH_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 46
         Bp_out[B_Counter++] = WordVal.b[1]; // 47
         

         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainLumaHMin.HIGH_SLOPE_AGC_Value; // 48
         

         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainLumaHMin.LOW_SLOPE_AGC_Value; // 49
         

         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaHMin.HIGH_THRESH_AGC_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 50
         Bp_out[B_Counter++] = WordVal.b[1]; // 51   
         

         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaHMin.LOW_THRESH_AGC_Value;         
         Bp_out[B_Counter++] = WordVal.b[0]; // 52
         Bp_out[B_Counter++] = WordVal.b[1]; // 53

         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainLumaVGainMin; // 54

         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaVMin.LOSHOOT_TOL_Value;         
         Bp_out[B_Counter++] = WordVal.b[0]; // 55
         Bp_out[B_Counter++] = WordVal.b[1]; // 56

         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaVMin.LUSHOOT_TOL_Value;         
         Bp_out[B_Counter++] = WordVal.b[0]; // 57
         Bp_out[B_Counter++] = WordVal.b[1]; // 58                 

         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaVMin.SOSHOOT_TOL_Value;         
         Bp_out[B_Counter++] = WordVal.b[0]; // 59
         Bp_out[B_Counter++] = WordVal.b[1]; // 60
         
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaVMin.SUSHOOT_TOL_Value;         
         Bp_out[B_Counter++] = WordVal.b[0]; // 61
         Bp_out[B_Counter++] = WordVal.b[1]; // 62
         
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaVMin.LGAIN_Value;         
         Bp_out[B_Counter++] = WordVal.b[0]; // 63
         Bp_out[B_Counter++] = WordVal.b[1]; // 64
         
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaVMin.SGAIN_Value;         
         Bp_out[B_Counter++] = WordVal.b[0]; // 65
         Bp_out[B_Counter++] = WordVal.b[1]; // 66        
         
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaVMin.FINALLGAIN_Value;         
         Bp_out[B_Counter++] = WordVal.b[0]; // 67
         Bp_out[B_Counter++] = WordVal.b[1]; // 68
         
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaVMin.FINALSGAIN_Value;         
         Bp_out[B_Counter++] = WordVal.b[0]; // 69
         Bp_out[B_Counter++] = WordVal.b[1]; // 70
         
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaVMin.FINALGAIN_Value;         
         Bp_out[B_Counter++] = WordVal.b[0]; // 71
         Bp_out[B_Counter++] = WordVal.b[1]; // 72
         
         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainLumaVMin.DELTA_Value; // 73
         
         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainLumaVMin.SLOPE_Value; // 74

         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaVMin.THRESH_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 75
         Bp_out[B_Counter++] = WordVal.b[1]; // 76
         
         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainLumaVMin.HIGH_SLOPE_AGC_Value; // 77
         
         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainLumaVMin.LOW_SLOPE_AGC_Value; // 78
         
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaVMin.HIGH_THRESH_AGC_Value;         
         Bp_out[B_Counter++] = WordVal.b[0]; // 79
         Bp_out[B_Counter++] = WordVal.b[1]; // 80   
         
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainLumaVMin.LOW_THRESH_AGC_Value;         
         Bp_out[B_Counter++] = WordVal.b[0]; // 81
         Bp_out[B_Counter++] = WordVal.b[1]; // 82

         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainChromaHGainMin; // 83

         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaHMin.LOSHOOT_TOL_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 84
         Bp_out[B_Counter++] = WordVal.b[1]; // 85

         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaHMin.LUSHOOT_TOL_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 86
         Bp_out[B_Counter++] = WordVal.b[1]; // 87        

         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaHMin.SOSHOOT_TOL_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 88
         Bp_out[B_Counter++] = WordVal.b[1]; // 89         

         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaHMin.SUSHOOT_TOL_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 90
         Bp_out[B_Counter++] = WordVal.b[1]; // 91         

         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaHMin.LGAIN_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 92
         Bp_out[B_Counter++] = WordVal.b[1]; // 93         

         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaHMin.SGAIN_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 94
         Bp_out[B_Counter++] = WordVal.b[1]; // 95         

         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaHMin.FINALLGAIN_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 96
         Bp_out[B_Counter++] = WordVal.b[1]; // 97         

         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaHMin.FINALSGAIN_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 98
         Bp_out[B_Counter++] = WordVal.b[1]; // 99         

         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaHMin.FINALGAIN_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 100
         Bp_out[B_Counter++] = WordVal.b[1]; // 101         

         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainChromaHMin.DELTA_Value; // 102         

         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainChromaHMin.SLOPE_Value; // 103         

         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaHMin.THRESH_Value; 
         Bp_out[B_Counter++] = WordVal.b[0]; // 104
         Bp_out[B_Counter++] = WordVal.b[1]; // 105
         
         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainChromaHMin.HIGH_SLOPE_AGC_Value; // 106         

         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainChromaHMin.LOW_SLOPE_AGC_Value; // 107         

         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaHMin.HIGH_THRESH_AGC_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 108
         Bp_out[B_Counter++] = WordVal.b[1]; // 109
         
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaHMin.LOW_THRESH_AGC_Value;         
         Bp_out[B_Counter++] = WordVal.b[0]; // 110
         Bp_out[B_Counter++] = WordVal.b[1]; // 111

         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainChromaVGainMin; // 112

         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaVMin.LOSHOOT_TOL_Value;         
         Bp_out[B_Counter++] = WordVal.b[0]; // 113
         Bp_out[B_Counter++] = WordVal.b[1]; // 114

         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaVMin.LUSHOOT_TOL_Value;         
         Bp_out[B_Counter++] = WordVal.b[0]; // 115
         Bp_out[B_Counter++] = WordVal.b[1]; // 116

         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaVMin.SOSHOOT_TOL_Value;         
         Bp_out[B_Counter++] = WordVal.b[0]; // 117
         Bp_out[B_Counter++] = WordVal.b[1]; // 118
         
			*Bp_len = B_Counter;

         gm_Printf("1 B_Counter = %d", B_Counter);
      
			// Save step number ...
			*(Bp_Context + 2) = *(Bp_Context + 2) + 1;
         
			// More Data ...
			MoreData(1);      
      }
      else if(*(Bp_Context + 2) == 1)
      {
         BYTE B_Counter = 0;      
         
			*Bp_len = B_Counter;

         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaVMin.SUSHOOT_TOL_Value;         
         Bp_out[B_Counter++] = WordVal.b[0]; // 1
         Bp_out[B_Counter++] = WordVal.b[1]; // 2
         
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaVMin.LGAIN_Value;         
         Bp_out[B_Counter++] = WordVal.b[0]; // 3
         Bp_out[B_Counter++] = WordVal.b[1]; // 4
         
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaVMin.SGAIN_Value;         
         Bp_out[B_Counter++] = WordVal.b[0]; // 5
         Bp_out[B_Counter++] = WordVal.b[1]; // 6         
         
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaVMin.FINALLGAIN_Value;         
         Bp_out[B_Counter++] = WordVal.b[0]; // 7
         Bp_out[B_Counter++] = WordVal.b[1]; // 8
         
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaVMin.FINALSGAIN_Value;         
         Bp_out[B_Counter++] = WordVal.b[0]; // 9
         Bp_out[B_Counter++] = WordVal.b[1]; // 10
         
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaVMin.FINALGAIN_Value;         
         Bp_out[B_Counter++] = WordVal.b[0]; // 11
         Bp_out[B_Counter++] = WordVal.b[1]; // 12
         
         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainChromaVMin.DELTA_Value; // 13
         
         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainChromaVMin.SLOPE_Value; // 14

         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaVMin.THRESH_Value;
         Bp_out[B_Counter++] = WordVal.b[0]; // 15
         Bp_out[B_Counter++] = WordVal.b[1]; // 16
         
         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainChromaVMin.HIGH_SLOPE_AGC_Value; // 17
         
         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainChromaVMin.LOW_SLOPE_AGC_Value; // 18
         
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaVMin.HIGH_THRESH_AGC_Value;         
         Bp_out[B_Counter++] = WordVal.b[0]; // 20
         Bp_out[B_Counter++] = WordVal.b[1]; // 21
         
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainChromaVMin.LOW_THRESH_AGC_Value;         
         Bp_out[B_Counter++] = WordVal.b[0]; // 22
         Bp_out[B_Counter++] = WordVal.b[1]; // 23

         // MainNLHMax
         // WORD Threshold1;
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainNLHMin.Threshold1;
         Bp_out[B_Counter++] = WordVal.b[0]; // 24
         Bp_out[B_Counter++] = WordVal.b[1]; // 25

         // BYTE Gain1;
         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainNLHMin.Gain1; // 26
         
         // WORD Threshold2;
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainNLHMin.Threshold2;
         Bp_out[B_Counter++] = WordVal.b[0]; // 27
         Bp_out[B_Counter++] = WordVal.b[1]; // 28

         // BYTE Gain2;
         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainNLHMin.Gain2; // 29

         // MainNLVMax
         // WORD Threshold1;
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainNLVMin.Threshold1;
         Bp_out[B_Counter++] = WordVal.b[0]; // 30
         Bp_out[B_Counter++] = WordVal.b[1]; // 31

         // BYTE Gain1;
         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainNLVMin.Gain1; // 32
         
         // WORD Threshold2;
         WordVal.w = VQ_Sharpness_Para_Buff.DataRange.MainNLVMin.Threshold2;
         Bp_out[B_Counter++] = WordVal.b[0]; // 33
         Bp_out[B_Counter++] = WordVal.b[1]; // 34   

         // BYTE Gain2;
         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainNLVMin.Gain2; // 35

         // MainNCHMax
         // BYTE Threshold1;
         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainNCHMin.Threshold1; // 36

         // MainNCVMax
         // BYTE Threshold1;
         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.DataRange.MainNCVMin.Threshold1; // 37

         // Sharpness Step
         Bp_out[B_Counter++] = VQ_Sharpness_Para_Buff.NumOfSteps; // 38

         gm_Printf("2 B_Counter = %d", B_Counter);
      
			// Save step number ...
			*(Bp_Context + 2) = *(Bp_Context + 2) + 1;
         
			// More Data ...
			MoreData(1);      
      }         
      // Array of Sharpness Steps
      else
      {
         // NON-LINEAR_THRESH1	1
         Bp_out[0] = VQ_Sharpness_Para_Buff.StepData_p[B_Step - 2].NonLinearThreshold1;
         
         // NON-LINEAR_GAIN1		1
         Bp_out[1] = VQ_Sharpness_Para_Buff.StepData_p[B_Step - 2].NonLinearGain1;
         
         // NON-LINEAR_THRESH2	1
         Bp_out[2] = VQ_Sharpness_Para_Buff.StepData_p[B_Step - 2].NonLinearThreshold2;
         
         // NON-LINEAR_GAIN2		1
         Bp_out[3] = VQ_Sharpness_Para_Buff.StepData_p[B_Step - 2].NonLinearGain2;
         
         // NOISE_CORING			1
         Bp_out[4] = VQ_Sharpness_Para_Buff.StepData_p[B_Step - 2].NoiseCoring;
         
         // LUMA_OVERSHOOT		1
         Bp_out[5] = VQ_Sharpness_Para_Buff.StepData_p[B_Step - 2].Luma.OverShoot;
         
         // LUMA_LARGE_GAIN		1
         Bp_out[6] = VQ_Sharpness_Para_Buff.StepData_p[B_Step - 2].Luma.LargeGain;
         
         // LUMA_SMALL_GAIN		1
         Bp_out[7] = VQ_Sharpness_Para_Buff.StepData_p[B_Step - 2].Luma.SmallGain;
         
         // LUMA_FINAL_GAIN		1
         Bp_out[8] = VQ_Sharpness_Para_Buff.StepData_p[B_Step - 2].Luma.FinalGain;
         
         // LUMA_AGC				1
         Bp_out[9] = VQ_Sharpness_Para_Buff.StepData_p[B_Step - 2].Luma.AGC;
         
         // CHROMA_OVERSHOOT		1
         Bp_out[10] = VQ_Sharpness_Para_Buff.StepData_p[B_Step - 2].Chroma.OverShoot;
         
         // CHROMA_LARGE_GAIN 	1
         Bp_out[11] = VQ_Sharpness_Para_Buff.StepData_p[B_Step - 2].Chroma.LargeGain;
         
         // CHROMA_SMALL_GAIN 	1
         Bp_out[12] = VQ_Sharpness_Para_Buff.StepData_p[B_Step - 2].Chroma.SmallGain;
         
         // CHROMA_FINAL_GAIN 	1
         Bp_out[13] = VQ_Sharpness_Para_Buff.StepData_p[B_Step - 2].Chroma.FinalGain;
         
         // CHROMA_AGC			1
         Bp_out[14] = VQ_Sharpness_Para_Buff.StepData_p[B_Step - 2].Chroma.AGC;
         
			*Bp_len = 15;

         dbgmsg("Sharpness_MsgHandler: Step %d\n", *(Bp_Context + 2) - 2);

			// Save step number ...
			*(Bp_Context + 2) = *(Bp_Context + 2) + 1;
			
			if((*(Bp_Context + 2)) == (VQ_Sharpness_Para_Buff.NumOfSteps + 2)) // +2 for MVF Data Range and Thresh
				MoreData(0); // No More Data ...
			else
				MoreData(1);
							
		}         

		dbgmsg("TS_Sharpness_READ", 0);
		
	}
	else if(W_CurrentCmd == TS_Sharpness_WRITE)	
	{	
      BYTE far * pTransferTable= Bp_msg;
      WORD W_Size = GetMesageLength();
      //WORD W_Index;
      BYTE far * bp_SharpnessData = (BYTE far *)&VQ_Sharpness_Para_TEMP;
      
      union
      {
         DWORD  dw;
         BYTE  b[4];
      } DWordVal;
      
      union
      {
         WORD  w;
         BYTE  b[2];
      } WordVal;

      if ( B_ContextStatus == FIRST_REQUEST )
      {
         W_Size -= 2; // 0, 1 for command
         pTransferTable += 2;
         
         DWordVal.b[0] = Bp_msg[2];
         DWordVal.b[1] = Bp_msg[3];
         DWordVal.b[2] = Bp_msg[4];
         DWordVal.b[3] = Bp_msg[5];
         //gm_Printf("size 0x%lX", DWordVal.dw);

         DWordVal.b[0] = Bp_msg[6];
         DWordVal.b[1] = Bp_msg[7];
         DWordVal.b[2] = Bp_msg[8];
         DWordVal.b[3] = Bp_msg[9];
         //gm_Printf("signature 0x%lX", DWordVal.dw);         

         DWordVal.b[0] = Bp_msg[10];
         DWordVal.b[1] = Bp_msg[11];
         DWordVal.b[2] = Bp_msg[12];
         DWordVal.b[3] = Bp_msg[13];
         //gm_Printf("version 0x%lX", DWordVal.dw);         

         W_Size -= 12;
         pTransferTable += 12;

         // MHF_SHARPNESS_LUMA	2 X 1 (instance x size)
         //gm_Printf("MHF LUMA Min = %d", Bp_msg[14]);
         //gm_Printf("MHF LUMA Max = %d", Bp_msg[15]);

         //gm_Printf("0 == 0x%x", *(Bp_Context + 0));
         //gm_Printf("1 == 0x%x", *(Bp_Context + 1));         

         WordVal.w = W_Size;
         *(Bp_Context + 2) = WordVal.b[0];
         *(Bp_Context + 3) = WordVal.b[1];
         _fmemcpy(bp_SharpnessData, pTransferTable, W_Size);
         
      }
      else
      {
         WordVal.b[0] = *(Bp_Context + 2);
         WordVal.b[1] = *(Bp_Context + 3);

         _fmemcpy((bp_SharpnessData + WordVal.w), pTransferTable, W_Size);
      
         WordVal.w += W_Size;
         
         *(Bp_Context + 2) = WordVal.b[0];
         *(Bp_Context + 3) = WordVal.b[1];               
      }

      gm_Printf("VQ_Sharpness_Para_TEMP NumOfSteps = %d", VQ_Sharpness_Para_TEMP.NumOfSteps);

      WordVal.b[0] = *(Bp_Context + 2);
      WordVal.b[1] = *(Bp_Context + 3);
      gm_Printf("Index = %d", WordVal.w);

		Bp_out[0] = TS_OK;
		*Bp_len = 1;

		dbgmsg("TS_Sharpness_WRITE", 0);
	}
	else if(W_CurrentCmd == TS_Sharpness_EN)
	{
		Bp_out[0] = TS_OK;
		*Bp_len = 1;	
		dbgmsg("TS_Shaprness_EN \n", 0);
	}
	else if(W_CurrentCmd == TS_Sharpness_DISABLE)
	{
		Bp_out[0] = TS_OK;
		*Bp_len = 1;	
		
		dbgmsg("TS_Sharpness_DISABLE \n", 0);
	}
	else
	{
		dbgmsg("STTS_Handler_Sharpness: UNKNOWN COMMAND \n", 0);
		Bp_out[0] = TS_NOT_SUPPORTED;
		*Bp_len = 1;	
	}


	return (BYTE)PORT_MSG_HNDLR_OK;
}

#endif
