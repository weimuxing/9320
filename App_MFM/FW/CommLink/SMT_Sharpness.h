
#ifdef USE_SMT



#define SHARPNESS_STEPS_MAX_NUM 64

//#define PACKED  __attribute__((packed))
#pragma pack(push)
#pragma pack(1)

typedef struct VIP_EnhancerParameters_s
{
   WORD LOSHOOT_TOL_Value;
   WORD LUSHOOT_TOL_Value;
   WORD SOSHOOT_TOL_Value;
   WORD SUSHOOT_TOL_Value;
   WORD LGAIN_Value;
   WORD SGAIN_Value;
   WORD FINALLGAIN_Value;
   WORD FINALSGAIN_Value;
   WORD FINALGAIN_Value;
   BYTE  DELTA_Value;
   BYTE  SLOPE_Value;
   WORD THRESH_Value;
   BYTE  HIGH_SLOPE_AGC_Value;
   BYTE  LOW_SLOPE_AGC_Value;
   WORD HIGH_THRESH_AGC_Value;
   WORD LOW_THRESH_AGC_Value;
} VIP_EnhancerParameters_t;

typedef struct VIP_EnhancerNonLinearControl_s
{
   WORD Threshold1;
   BYTE Gain1;
   WORD Threshold2;
   BYTE Gain2;
} VIP_EnhancerNonLinearControl_t;

typedef struct VIP_EnhancerNoiseCoringControl_s
{
   BYTE Threshold1;
} VIP_EnhancerNoiseCoringControl_t;

typedef struct VIP_SharpnessDataRange_s
{
   // max data range
   BYTE MainLumaHGainMax;
   VIP_EnhancerParameters_t MainLumaHMax;
   BYTE MainLumaVGainMax;
   VIP_EnhancerParameters_t MainLumaVMax;
   BYTE MainChromaHGainMax;
   VIP_EnhancerParameters_t MainChromaHMax;
   BYTE MainChromaVGainMax;
   VIP_EnhancerParameters_t MainChromaVMax;
   VIP_EnhancerNonLinearControl_t MainNLHMax;
   VIP_EnhancerNonLinearControl_t MainNLVMax;
   VIP_EnhancerNoiseCoringControl_t MainNCHMax;
   VIP_EnhancerNoiseCoringControl_t MainNCVMax;

   // min data range
   BYTE MainLumaHGainMin;
   VIP_EnhancerParameters_t MainLumaHMin;
   BYTE MainLumaVGainMin;
   VIP_EnhancerParameters_t MainLumaVMin;
   BYTE MainChromaHGainMin;
   VIP_EnhancerParameters_t MainChromaHMin;
   BYTE MainChromaVGainMin;
   VIP_EnhancerParameters_t MainChromaVMin;
   VIP_EnhancerNonLinearControl_t MainNLHMin;
   VIP_EnhancerNonLinearControl_t MainNLVMin;
   VIP_EnhancerNoiseCoringControl_t MainNCHMin;
   VIP_EnhancerNoiseCoringControl_t MainNCVMin;
} VIP_SharpnessDataRange_t;

typedef struct VIP_SharpnessStepGroup_s
{
   BYTE OverShoot;
   BYTE LargeGain;
   BYTE SmallGain;
   BYTE FinalGain;
   BYTE AGC;
} VIP_SharpnessStepGroup_t;

typedef struct VQ_Sharpness_Step_Setting_s
{
   BYTE NonLinearThreshold1;
   BYTE NonLinearGain1;
   BYTE NonLinearThreshold2;
   BYTE NonLinearGain2;
   BYTE NoiseCoring;

   VIP_SharpnessStepGroup_t Luma;
   VIP_SharpnessStepGroup_t Chroma;
} VQ_Sharpness_Step_Setting_t;

typedef struct VQ_Sharpness_Parameters_s
{
   LLD_TuningData_t descriptor; // descriptor
   VIP_SharpnessDataRange_t DataRange;
   BYTE NumOfSteps;

   VQ_Sharpness_Step_Setting_t StepData_p[SHARPNESS_STEPS_MAX_NUM];
} VQ_Sharpness_Parameters_t;

#pragma pack(pop)

//******************************************************************************
//  G L O B A L    F U N C T I O N S
//******************************************************************************
BYTE Sharpness_MsgHandler(MsgPacket_t *message, BYTE comm_use);

#endif


