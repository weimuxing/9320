/*
   $Workfile:   gm_ACC3.h  $
   $Revision: 1.11 $
   $Date: 2012/10/12 06:15:39 $
*/
#ifndef __ACC3_H__
#define __ACC3_H__
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
// MODULE:      gm_ACC3.h
//
// USAGE:
//
//******************************************************************************


//*************************************************************************
//
//   D E F I N I T I O N S
//
//*************************************************************************
#define NUMBER_OF_BINS 64 // 16
#define INTHIST_AVG_VAL 4096L // 4080L

//******************************************************************************
//  T Y P E    D E F I N I T I O N S
//******************************************************************************
typedef struct
{
   BYTE HistStretchEnable;			// 1 -  Histogram Stertch Enable:  0 - disable
   WORD Cumulative_Black_Pixels;	// Cumul. number of pixels before black poSWORD >> 12
   // Ex. SD 0.5% pixels = 1728, >>12 = 27
   WORD Cumulative_White_Pixels;
   WORD Max_Black_Bin;				// Max bin number for black poSWORD
   WORD Min_White_Bin;				// Min bin number for White poSWORD
   BYTE ACC_Strength;				// ACC strength value for Local ACC
   DWORD WF[NUMBER_OF_BINS];		// Weight factors for bins, SWORDernally /8 for range 0-1
   WORD BinLim[NUMBER_OF_BINS];	// limit for each bin value
} VIP_ACCSWParameters_t;         // software parameters

typedef struct
{
   BYTE bin[64];
}VIP_ACCStaticLut_t;                   // input static LUT

typedef struct
{
   BYTE IVPMeasureEn;
   BYTE IVPScCheckEn;
   BYTE IVPScSource;
   WORD IVPScThresh;
   WORD HpfAmpThreshY;
   WORD HpfAmpThreshUV;
   BYTE ClampBlack;
   BYTE LutTempFilterEnable;
   BYTE YclNlsatScale;
   BYTE YclNlsatEn;
   BYTE YclNlsatTable[8];
   BYTE YclUseKY;
   BYTE CompLutLpfEn;
   BYTE CheckScEnable;
   BYTE SceneChangeResetEnable;
   BYTE TfFrameNum;
   BYTE YCLinkEnable;
   BYTE SceneChangeSource;
   BYTE LeftSlope;      //ScaleMin
   BYTE RightSlope;    //ScaleMax
   BYTE ScaleThresh;
   BYTE YCLinkGain;    //ScaleFactor
   WORD SceneChangeThreshold;
   BYTE XvYccEnable;
   BYTE YLpf[5];
   BYTE CLpf[5];
   BYTE ACCYLpfFlt;
   BYTE ACCCLpfFlt;
   BYTE UseLpfY ;
   BYTE StaticLUTEn ;
   BYTE LACCEnable;
   WORD LACCTextureThresh;
   BYTE LACCDiffShift;
   BYTE LACCTextureOffset;
   BYTE LACCTextureGain;
   BYTE LACCSmoothThresh;
   BYTE LACCSmoothness;
} VIP_ACCHWParameters_t;          // hardware parameters

typedef struct
{
   LLD_TuningData_t descriptor; // descriptor
   VIP_ACCSWParameters_t ACCSWParams;
   VIP_ACCStaticLut_t ACCStatic;
   VIP_ACCHWParameters_t ACCHWParams;
} VQ_ACC_Parameters_t;

typedef enum gmt_ACC3_ControlPar_Enum
{
   AccOptionsTableSelect, //Attenuation Alghorithm selection: 0 - Select E-aghorithm; 1-Select A -alghorith
   AccParamTableSelect,//Select source of Scene Change detectetion: 0 - Select internal detection; 1-Select external detection
   AccModeControlsSelect
}gmt_ACC3CtrlParamSelector;

typedef struct AccModeCtrStruct
{
   WORD ProcessingEnable:1;
   WORD LutGenEnable:1; //gmt_AccRegID
}gmt_ACC3ModeCtr;


//******************************************************************************
//  G L O B A L    F U N C T I O N S
//******************************************************************************

extern void gm_ACC3SetParams(gmt_ACC3CtrlParamSelector SelParam,void far * ParamTable, WORD W_TabSize);
extern void gm_ACC3GetOptions( VIP_ACCHWParameters_t far * pOptions);
extern void gm_ACC3GetControlParam(gmt_ACC3ModeCtr far * ptr);
extern void gm_ACC3GetParams(VIP_ACCSWParameters_t far  * ParamTable);
extern void gm_ACC3UpdateModeDependants(void);
extern void gm_ACC3ProcessFrame(void);
extern void gm_ACC3Init(void);
extern BOOL gm_ACC3_IsProcessingEnabled(BOOL Bt_IsCalledFromInterrupt);
extern void gm_ACC3SwitchLUT(void);
extern DWORD gm_ACC3GetHistogram(BYTE B_Bin);
extern void gm_ACC3StopISR(void);
extern void gm_ACC3StartISR(void);
extern void gm_ACCDump(BYTE Mode, BYTE Count);
extern void gm_ACC3_AvoidForceUpdate(BOOL b_Action);
extern void LLD_ACCReadBinSlopes(WORD * BinSlopes_p, WORD W_offset, BYTE B_size);
extern void LLD_ACCReadInputHistogram(BYTE * InputHistogram_p, WORD W_offset, BYTE B_size);
extern void LLD_ACCSetMemoryParams(VIP_ACCStaticLut_t far * Params_p);

#endif


