
#ifdef USE_SMT


//*************************************************************************
// DEFINES
//*************************************************************************
typedef struct
{
   BYTE Threshold0;
   BYTE Threshold1;
   BYTE Threshold2;
} VIP_MADiThreshold_t;

#pragma pack(push)
#pragma pack(1)
typedef struct VQ_MADi_Parameters_s
{
   BYTE EnableGlobalMotionAdaptive;
   VIP_MADiThreshold_t LowMotion;
   VIP_MADiThreshold_t StandardMotion;
   VIP_MADiThreshold_t HighMotion;
   VIP_MADiThreshold_t PanMotion;
   WORD GlobalMotionStandardThreshold;
   WORD GlobalMotionHighThreshold;
   BYTE GlobalMotionPanThreshold;
   BYTE GlobalMotionInterpolation;
   BYTE NoiseGain;
   BYTE ChromaGain;
   BYTE VerticalGain;
   BYTE LumaAdaptiveEnable;
   BYTE StaticHistGain;
   BYTE MVLHL;
   BYTE MVLHH;
   BYTE MVHHL;
   BYTE MVHHH;
   BYTE MCVLHL;
   BYTE MCVLHH;
   BYTE MCVHHL;
   BYTE MCVHHH;
   BYTE DeinterlacerDCDiEnable;
   BYTE DeinterlacerDCDiStrength;
} VQ_MADi_Parameters_t;
#pragma pack(pop)




//******************************************************************************
//  G L O B A L    F U N C T I O N S
//******************************************************************************
BYTE MADi_MsgHandler(MsgPacket_t *message, BYTE comm_use);

#endif

