#ifndef __SYSLIB_3D_H__
#define __SYSLIB_3D_H__

#define FEATURE_3D_APPSTEST                        ENABLE
#define FEATURE_3D_READ_FRONTEND_DIRECTLY          DISABLE
#define FEATURE_3D_KEY_HANDLER                     DISABLE
#define FEATURE_3D_OSD                             ENABLE

#define SGTC_SW                                    1
#define SGTC_HW                                    2
#define SGTC_CTRL_METHOD                           SGTC_HW

#define DEMO_3D_BD_PLAYER_KEY                      DISABLE

//
// 3D Input timing data
//
typedef struct
{
   WORD W_StdHFreq;           // Standard H. frequency in 100 Hz unit
   BYTE B_DeltaHFreq;         // H. frequency tolerance in 100 Hz unit

   WORD W_StdVFreq;           // Standard V. frequency in 1/10 Hz unit
   BYTE B_DeltaVFreq;         // V. frequency tolerance in 1/10 Hz unit

   WORD W_StdVTotal;          // Standard V. total in line unit
   BYTE B_DeltaVTotal;        // V. total tolerance in line unit

   WORD W_StdHTotal;          // Standard H. total in pixel unit

   WORD W_StdWidth;           // Standard width in pixel unit
   WORD W_StdHeight;          // Standard height in line unit

   WORD W_StdHBackPorch;      // Standard H. back porch in pixel unit 
   WORD W_StdVBackPorch;      // Standard V. back porch in line unit 

   BYTE B_StdHSyncWidth;      // Standard H. start in pixel unit 
   BYTE B_StdVSyncWidth;      // Standard V. start in line unit 

   WORD W_StdHFrontPorch;     // Standard H. front porch in pixel unit 
   BYTE W_StdVFrontPorch;     // Standard V. front porch in line unit 

   WORD W_ModeType;           // Mode type
   WORD W_OverlapType;        // Overlap type
} _3D_ModeStruct;

// define of 3D input format

typedef enum
{
   gmd_3D_IN_UNKNOWN = 0,

   gmd_3D_IN_ORIGINAL_2D = 1,

   gmd_3D_IN_FRAME_SEQUENTIAL = 2,

   gmd_3D_IN_FRAME_PACKING = 3,
   gmd_3D_IN_STACKED_TOP_BOTTOM = 4,

   gmd_3D_IN_SIDE_BY_SIDE_HALF = 5,
   gmd_3D_IN_SIDE_BY_SIDE_FULL = 6,

   gmd_3D_IN_TOP_AND_BOTTOM = 7,

   gmd_3D_IN_FIELD_ALTERNATIVE = 8,
   gmd_3D_IN_LINE_ALTERNATIVE = 9,

   gmd_3D_IN_FRAME_SEQUENTIAL_RIGHT = 10,
   gmd_3D_IN_FRAME_SEQUENTIAL_LEFT = 11,

   gmd_3D_IN_SIDE_BY_SIDE_HALF_RIGHT = 12,
   gmd_3D_IN_SIDE_BY_SIDE_HALF_LEFT = 13,

   gmd_3D_IN_INTERLACED_RIGHT_EVEN = 14,
   gmd_3D_IN_INTERLACED_RIGHT_ODD = 15,
   gmd_3D_IN_INTERLACED_RIGHT_CHECKERBOARD = 16,
   gmd_3D_IN_INTERLACED_RIGHT_V_LEFT = 17,
   gmd_3D_IN_INTERLACED_RIGHT_V_RIGHT = 18,

   gmd_3D_IN_SIDE_BY_SIDE_HALF_INTERLACED = 19,

   gmd_3D_IN_FRAME_PACKING_CASE_5 = 20,

   gmd_3D_IN_MAXIMUM = gmd_3D_IN_FRAME_PACKING_CASE_5,
}  gmt_3D_INPUT_FORMAT_TYPE;

typedef enum
{
   gmd_3D_OUT_FRAME_SEQUENTIAL,
   gmd_3D_OUT_LINE_ALTERNATIVE,
   gmd_3D_OUT_SIDE_BY_SIDE,
   gmd_3D_OUT_UNKNOWN,
}  gmt_3D_OUTPUT_FORMAT_TYPE;

typedef struct
{
   WORD H_Width;
   WORD V_Height;
}  gmt_3D_INPUT_TIMING;

typedef struct
{
   WORD H_Width;
   WORD V_Height;
}  gmt_3D_OUTPUT_TIMING;

typedef enum
{
   gmd_3D_STATE_READY,
   gmd_3D_STATE_FRONT_END_DETECTED,
   gmd_3D_STATE_FRONT_END_DETECTED_SUCCESS,
   gmd_3D_STATE_FRC_FINISHED,
   gmd_3D_STATE_SCALER_FINISHED,
   gmd_3D_STATE_FINISHED,
}  gmt_3D_PROGRAMMING_STATE;

typedef enum
{
   gmd_3D_MODESETUP_NOT_COMPLETED,
   gmd_3D_MODESETUP_NOT_COMPLETED_WAIT,
   gmd_3D_MODESETUP_COMPLETED_NOW,
   gmd_3D_MODESETUP_COMPLETED_WAIT,
   gmd_3D_MODESETUP_COMPLETED_WAIT_DONE,
   gmd_3D_MODESETUP_COMPLETED,
   gmd_3D_MODESETUP_COMPLETED_IDEL,
}  gmt_3D_MODESETUP_STATE;

typedef struct
{
   BOOL Is3DModeEnabled;

   BOOL IsFrontEndChanged;

#if (FEATURE_3D_APPSTEST == ENABLE)
   BOOL IsForceModeChanged;

   gmt_3D_INPUT_FORMAT_TYPE Appstest_3D_Format;
#endif

   BYTE DetectedFailedCounter;            // counter of mode detection failed

   gmt_3D_PROGRAMMING_STATE State;        // state of 3D programming sequence

   BYTE ModeTableIndex_FrontEndIBD;       // Mode table index of "Front End IBD" timing

   BOOL IsPatchAppliedInEndOfModeDetermine;
   BOOL IsAlwaysCompressedForFRC;

   BOOL Is_LeftRight_Swapped;             // identify for left right swapped

   BOOL IsODPin400MHz;

   BOOL IsInterlaced;

   BOOL IsTwoStageEnabled;

   BOOL IsVSyncInserted;

   BOOL IsSGTC_Enabled;                   // identify for shuttle glasses enabled/disabled
   BOOL LEFTorRIGHT;                      // identify for left or right

   gmt_3D_MODESETUP_STATE ModeState;      // identify for mode setup status

   gmt_3D_INPUT_FORMAT_TYPE InputFormat;
   gmt_3D_OUTPUT_FORMAT_TYPE OutputFormat;

   gmt_UserDefConnNames InputConnector;
   gmt_ConnTypes InputConnType;

   BYTE IN_Bits_Per_Pixel;    // Input bits per pixel.
   WORD IN_H_Period;          // Input H sync period in TCLK.
   WORD IN_V_Period;          // Input V sync period in line.
   WORD IN_H_Freq;            // Input H sync frequence in 100Hz.
   WORD IN_V_Freq;            // Input V sync frequence in 1/10Hz.
   WORD IN_H_Total;           // Input H total in pixel.
   WORD IN_V_Total;           // Input V total in line.
   WORD IN_H_Width;           // Input H width in pixel.
   WORD IN_V_Height;          // Input V height in line.
   BYTE IN_H_SyncWidth;       // Input H sync width in pixel.
   WORD IN_H_Back;            // Input H back porch in pixel.
   WORD IN_H_Front;           // Input H front porch in pixel.
   BYTE IN_V_SyncWidth;       // Input V sync width in line.
   WORD IN_V_Back;            // Input V back porch in line.
   BYTE IN_V_Front;           // Input V front porch in line.

   BYTE IN_V_Active_SP;       // Input V active SP for VSYNC insertion region in line. // for Frame.Packing only
   WORD IN_V_Active_Start;    // Input V active start in line.                         // for Frame.Packing only

   WORD OUT_H_Width;          // Output H width in pixel.
   WORD OUT_V_Height;         // Output V height in line.
   WORD OUT_V_MinFreq;        // Output V min sync frequence in Hz.
   WORD OUT_V_MaxFreq;        // Output V max sync frequence in Hz.

   WORD FRC_IN_Width;
   WORD FRC_IN_Height;
   WORD FRC_OUT_Width;
   WORD FRC_OUT_Height;
}  gmt_3D_INFO;

typedef struct
{
   BOOL Is_3D_OSD_Enabled;
   BOOL Is_3D_LR_Swapped;
} gmt_3D_ADJUSTER;

typedef struct
{
   BYTE index;                // index of pattern registers of left/right eyes control
                              //  0 ~ 16 for SGTC_A_PAT0 ~ SGTC_A_PAT16
                              // 17 ~ 32 for SGTC_B_PAT0 ~ SGTC_B_PAT16
                              // 33      for end of SGTC_A_PAT16 to next frame
                              // 34      for end of SGTC_B_PAT16 to next frame
                              // 35      for swap pattern of left/right eyes control
                              // 255     End of this data stream
   BYTE level;                // 0: low, 1: high
   WORD interval;             // unit in TCLK/32 = 27M/32 = 1.1852us
} gmt_3D_SG_PAT;

#if 0		//Hyb130503* move to Panel.h for elimite compiler warning
typedef enum PANEL_FORMAT
{
   FORMAT_ORG_2D,
   FORMAT_3D_FRAME_SEQUENTIAL,
   FORMAT_3D_LINE_ALTERNATIVE,
   FORMAT_3D_SIDE_BY_SIDE
} gmd_PanelDataFormat;
#endif

typedef struct
{
   BYTE ch;
   BYTE val;
	void (* RelatedFunction)(void);
}  gmt_3D_KEY_SUPPORT;

#define KEY_CH_0              0
#define KEY_CH_1              1

#define KEY_CH_DOWN           31
#define KEY_CH_UP             120
#define KEY_VOLUME_DOWN       170
#define KEY_VOLUME_UP         225

#define ADJ_3D_OSD_ENABLED    gm_3D_GetAdjuster()->Is_3D_OSD_Enabled
#define ADJ_3D_LR_SWAPPED     gm_3D_GetAdjuster()->Is_3D_LR_Swapped

// ISR
void gm_3D_Routine_In_IMP_V_Active(void);
void gm_3D_Routine_In_IMP_V_Blank(void);
void gm_3D_Routine_In_IMP_V_LineFlag(void);
void gm_3D_Routine_In_IMP_V_Sync(void);
void gm_3D_Routine_In_IPP_V_Active(void);
void gm_3D_Routine_In_IPP_V_Blank(void);
void gm_3D_Routine_In_IPP_V_LineFlag(void);
void gm_3D_Routine_In_IPP_V_Sync(void);
void gm_3D_Routine_In_DV_Active(void);
void gm_3D_Routine_In_DV_Blank(void);
void gm_3D_Routine_In_DV_LineFlag(void);
void gm_3D_Routine_In_DV_Sync(void);

// For debug and testing only
BOOL gm_3D_KeyEventHandler(BYTE far * key);
void gm_3D_Appstest(void);

// OSD adjuster function about 3D
gmt_3D_ADJUSTER * gm_3D_GetAdjuster(void);
void Adjust3D_LR_Swap_Enabled(void);
void Adjust3D_LR_Swap_Disabled(void);
void Adjust3D_OSD_Enabled(void);
void Adjust3D_OSD_Disabled(void);

// miscellanous
void gm_3D_Handler(void);

// Initialize
void gm_3D_Initialize(BOOL keep);
void gm_3D_InitializeOnce(void);
void gm_3D_SetStatusForFrontEndChanged(void);
void gm_3D_RecoveryTo2D(void);
void gm_3D_OutputTiming_2D_Mode(void);

// identify
BOOL gm_3D_Is3DModeEnabled(void);
BOOL gm_3D_IsODPin400MHz(void);
BOOL gm_3D_IsWaitToDisplay(void);

// code patch
void gm_3D_CodePatch_In_Start_Of_ModeDetermine(void);
void gm_3D_CodePatch_In_End_Of_ModeDetermine(gmt_PHY_CH B_Channel);

// mode determine and setup
BOOL gm_3D_ModeDetermineByFrontEnd(void);
WORD gm_3D_GetDesiredOutputVFreq(void);
BOOL gm_3D_SetCaptureWindow(void);
void gm_3D_SetMemoryByBPP(BYTE B_Bpp);
BOOL gm_3D_FrameBufferProgramming(DWORD *Start_Addr);
BOOL gm_3D_ScalerSetup(void);

#endif
