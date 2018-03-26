#include <mem.h>
#include "System\All.h"
#include "CommLink\smt.h"
//#include "CommLink\SMT_MADi.h"

#ifdef USE_SMT


#define MADI_DEBUG_MSG
#if defined(MADI_DEBUG_MSG)
#define dbgmsg(a,b) gm_Printf(a,b)
#else
#define dbgmsg(a,b)
#endif

#define TS_MADI_VER 0

#define MADI_DYN_DATA_SIZE 256

typedef enum
{
   TS_MADI_START,
   TS_MADI_STOP,
   TS_MADI_READ,
   TS_MADI_WRITE,
   TS_MADI_EN,
   TS_MADI_DISABLE,
   TS_MADI_GET_NOISE //GetNoiseDataMADI
}TsMADiCmd_t;

#pragma pack(push)
#pragma pack(1)
typedef struct
{
   BYTE IntNoise;
   BYTE ExtNoise;
   WORD MVBar[4];
}MadiFrameData_t;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct
{
   DWORD FrameNum;
   DWORD NumOfFrames;
   MadiFrameData_t *FrameDat;
}MadiDynData_t;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct
{
   BOOL Paused;
   DWORD ReadIndex;
   DWORD WriteIndex;
   MadiFrameData_t Buf[MADI_DYN_DATA_SIZE];
} MadiDynDataLog_t;
#pragma pack(pop)

extern BYTE Ba_Motion_Level_Based_MADI_Dynamic_Thresholds[8][3];
extern WORD Global_Mot_Threshold_hi;
extern WORD Global_Mot_Threshold_lo;
extern BYTE B_VerticalMotionVectorSum;

static volatile MadiDynDataLog_t SMT_RAM DataLog;

/*******************************************************************************
Name        : LLD_MotHistogramRead
Description : Read Motion Value Histogram. Number of pixels in which the Motion
                Detection resulted in "00","01","10","11" motion value
Parameters  : Channel - Input channel information (CH_A)
                [out] mv_p - pointer to U16[4] array
Returns     : ST_ERROR_FEATURE_NOT_SUPPORTED/ST_NO_ERROR
Examples    :
                U16 MVBar[4];
                LLD_MotHistogramRead(CH_A, MVBar);
*******************************************************************************/
#pragma argsused
void MotHistogramRead(WORD* mv_p)
{
   if (mv_p)
   {
      *mv_p++ = gm_ReadRegWord(IMP_MV0_BIN_SUM);
      *mv_p++ = gm_ReadRegWord(IMP_MV1_BIN_SUM);
      *mv_p++ = gm_ReadRegWord(IMP_MV2_BIN_SUM);
      *mv_p = gm_ReadRegWord(IMP_MV3_BIN_SUM);
   }
}



void VCD_InitMadiDynData(void)
{
   DataLog.Paused = FALSE;
   DataLog.ReadIndex = DataLog.WriteIndex = 0;
}


/*******************************************************************************
Name        :
Description : Read and store new readings into data log, called by ISR
Parameters  : none
Assumptions : This routine assumes that the data log and ISR are already
              initialized.
Limitations :
Returns     : none
*******************************************************************************/
void VCD_UpdateMadiDynData(void)
{
    volatile MadiFrameData_t SMT_RAM * const wr_p = &DataLog.Buf[(WORD)DataLog.WriteIndex];
   DWORD Val;

   if (DataLog.Paused) // if paused, return here
      return;

   Val = gm_ReadRegWord(MADI_INT_NOISE); // LLD_MadiNoiseRead(CH_A);

   if (Val & ~0xFF)
      Val = 0xFF;

   wr_p->IntNoise = (BYTE)Val;
   wr_p->ExtNoise = 0;
   MotHistogramRead((WORD *)wr_p->MVBar);

   // increment write buf ptr, wrap around if necessary
   DataLog.WriteIndex = (DataLog.WriteIndex + 1) % MADI_DYN_DATA_SIZE;
   // if write ptr has caught up to start ptr increment start buf ptr, wrap around if necessary
   if (DataLog.WriteIndex == DataLog.ReadIndex)
      DataLog.ReadIndex = (DataLog.ReadIndex + 1) % MADI_DYN_DATA_SIZE;
}

DWORD VCD_ExtractMadiDynData(MadiFrameData_t* Data_p, DWORD MaxSize)
{
   if (Data_p)
   {
      DWORD Res = 0;

      DataLog.Paused = TRUE; /* pause any logging activity */

      while ((DataLog.ReadIndex > DataLog.WriteIndex) && (MaxSize > 0))
      {   /* if buffer wrapped-around */
         memcpy(Data_p++, (void *)&DataLog.Buf[(WORD)DataLog.ReadIndex], sizeof(MadiFrameData_t));
         DataLog.ReadIndex = (DataLog.ReadIndex + 1) % MADI_DYN_DATA_SIZE;
         Res++;
         MaxSize--;
      }

      while ((DataLog.ReadIndex < DataLog.WriteIndex) && (MaxSize > 0))
      {   /* remained part */
         memcpy(Data_p++, (void *)(&DataLog.Buf[(WORD)DataLog.ReadIndex]), sizeof(MadiFrameData_t));
         DataLog.ReadIndex = (DataLog.ReadIndex + 1) % MADI_DYN_DATA_SIZE;
         Res++;
         MaxSize--;
      }

      DataLog.Paused = FALSE; /* unpause logging activity */
      return Res;
   }
   return 0;
}



#pragma argsused
BYTE MADi_MsgHandler(MsgPacket_t *message, BYTE comm_use)
{
   BYTE* Bp_msg = (BYTE *)(message->msgpkt);
   //BYTE B_Len = message->len - 5;
   VQ_MADi_Parameters_t madidatabuf;

   //get pointer to output message length setting
   BYTE* Bp_len=GetOutMsgLenPtr();
   BYTE* Bp_out= GetOutMsgBufPtr();

   if (Bp_msg[0] == TS_MADI_START)
   {
      Bp_out[0] = TS_OK;
      Bp_out[1] = TS_MADI_VER;
      *Bp_len = 2;
      dbgmsg("TS_MADI_START %s\n","OK");
      gm_Print("TS_MADI_START", 0);
   }
   else if (Bp_msg[0] == TS_MADI_STOP)
   {
      Bp_out[0] = TS_OK;
      *Bp_len = 1;
      gm_Print("TS_MADI_STOP", 0);
   }
   else if (Bp_msg[0] == TS_MADI_READ)
   {
      Bp_out[0] = TS_OK;

      // Size
		Bp_out[1] = 46; // 56; // 46; // ?
      Bp_out[2] = 0; // ?
      Bp_out[3] = 0; // ?
      Bp_out[4] = 0; // ?

      // Signature
      Bp_out[5] = 21; // ?
      Bp_out[6] = 0; // ?
      Bp_out[7] = 0; // ?
      Bp_out[8] = 0; // ?

      // Version ID
      Bp_out[9] = 0; // 1; // ?
      Bp_out[10] = 0; // ?
      Bp_out[11] = 0; // ?
      Bp_out[12] = 0; // ?

      if (UserPrefMADI == MADI_ADAPTIVE)
         madidatabuf.EnableGlobalMotionAdaptive = 1;
      else
         madidatabuf.EnableGlobalMotionAdaptive = 0;

      madidatabuf.LowMotion.Threshold0 = Ba_Motion_Level_Based_MADI_Dynamic_Thresholds[0][0];
      madidatabuf.LowMotion.Threshold1 = Ba_Motion_Level_Based_MADI_Dynamic_Thresholds[0][1];
      madidatabuf.LowMotion.Threshold2 = Ba_Motion_Level_Based_MADI_Dynamic_Thresholds[0][2];

		madidatabuf.StandardMotion.Threshold0 = gm_ReadRegWord(MADI_QUANT_THRESH0); // Ba_Motion_Level_Based_MADI_Dynamic_Thresholds[1][0];
		madidatabuf.StandardMotion.Threshold1 = gm_ReadRegWord(MADI_QUANT_THRESH1); // Ba_Motion_Level_Based_MADI_Dynamic_Thresholds[1][1];
		madidatabuf.StandardMotion.Threshold2 = gm_ReadRegWord(MADI_QUANT_THRESH2); // Ba_Motion_Level_Based_MADI_Dynamic_Thresholds[1][2];

      madidatabuf.HighMotion.Threshold0 = Ba_Motion_Level_Based_MADI_Dynamic_Thresholds[2][0];
      madidatabuf.HighMotion.Threshold1 = Ba_Motion_Level_Based_MADI_Dynamic_Thresholds[2][1];
      madidatabuf.HighMotion.Threshold2 = Ba_Motion_Level_Based_MADI_Dynamic_Thresholds[2][2];

      madidatabuf.PanMotion.Threshold0 = Ba_Motion_Level_Based_MADI_Dynamic_Thresholds[3][0];
      madidatabuf.PanMotion.Threshold1 = Ba_Motion_Level_Based_MADI_Dynamic_Thresholds[3][1];
      madidatabuf.PanMotion.Threshold2 = Ba_Motion_Level_Based_MADI_Dynamic_Thresholds[3][2];

      madidatabuf.GlobalMotionStandardThreshold = Global_Mot_Threshold_lo;
      madidatabuf.GlobalMotionHighThreshold = Global_Mot_Threshold_hi;

      madidatabuf.GlobalMotionPanThreshold = B_VerticalMotionVectorSum;

      madidatabuf.GlobalMotionInterpolation = 0; // ?

      madidatabuf.NoiseGain = gm_ReadRegByte(MADI_EXT_NOISE);

      madidatabuf.ChromaGain = gm_ReadRegByte(MADI_GAIN)&MADI_CHROMA_GAIN;

      madidatabuf.VerticalGain = (gm_ReadRegByte(MADI_GAIN)&MADI_VERT_GAIN)>>2;

      madidatabuf.LumaAdaptiveEnable = 0; // ?

      madidatabuf.StaticHistGain = 0; // ?

      madidatabuf.MVLHL = gm_ReadRegByte(MADI_FILTER_CTRL4);
      madidatabuf.MVLHH = (BYTE)(gm_ReadRegWord(MADI_FILTER_CTRL4)>>8);
      madidatabuf.MVHHL = gm_ReadRegByte(MADI_FILTER_CTRL5);
      madidatabuf.MVHHH = (BYTE)(gm_ReadRegWord(MADI_FILTER_CTRL5)>>8);
      madidatabuf.MCVLHL = gm_ReadRegByte(MADI_FILTER_CTRL8);
      madidatabuf.MCVLHH = (BYTE)(gm_ReadRegWord(MADI_FILTER_CTRL8)>>8);
      madidatabuf.MCVHHL = gm_ReadRegByte(MADI_FILTER_CTRL9);
      madidatabuf.MCVHHH = (BYTE)(gm_ReadRegWord(MADI_FILTER_CTRL9)>>8);

      madidatabuf.DeinterlacerDCDiEnable = gm_ReadRegByte(MVF_DCDI_CTRL1)&MVF_DCDI_ENABLE;
      madidatabuf.DeinterlacerDCDiStrength = gm_ReadRegByte(MVF_DCDI_CTRL4)&MVF_DCDI_PELCORR_TH;

      memcpy((Bp_out+13),&madidatabuf,sizeof(madidatabuf));
      *Bp_len = sizeof(VQ_MADi_Parameters_t) + 13;
      dbgmsg("TS_MADI_READ: %d bytes transferred \n", *Bp_len);
      dbgmsg("\t DCDI %d \n", madidatabuf.DeinterlacerDCDiEnable);
      dbgmsg("\t DCDI Strength = %d\n",madidatabuf.DeinterlacerDCDiStrength);
   }
   else if (Bp_msg[0] == TS_MADI_WRITE)
   {
      memcpy(&madidatabuf,(Bp_msg+14),sizeof(madidatabuf));

      UserPrefMADI = madidatabuf.EnableGlobalMotionAdaptive;

      Ba_Motion_Level_Based_MADI_Dynamic_Thresholds[0][0] = madidatabuf.LowMotion.Threshold0;
      Ba_Motion_Level_Based_MADI_Dynamic_Thresholds[0][1] = madidatabuf.LowMotion.Threshold1;
      Ba_Motion_Level_Based_MADI_Dynamic_Thresholds[0][2] = madidatabuf.LowMotion.Threshold2;

      Ba_Motion_Level_Based_MADI_Dynamic_Thresholds[1][0] = madidatabuf.StandardMotion.Threshold0;
      Ba_Motion_Level_Based_MADI_Dynamic_Thresholds[1][1] = madidatabuf.StandardMotion.Threshold1;
      Ba_Motion_Level_Based_MADI_Dynamic_Thresholds[1][2] = madidatabuf.StandardMotion.Threshold2;

      gm_WriteRegWord(MADI_QUANT_THRESH0, madidatabuf.StandardMotion.Threshold0);
      gm_WriteRegWord(MADI_QUANT_THRESH1, madidatabuf.StandardMotion.Threshold1);
      gm_WriteRegWord(MADI_QUANT_THRESH2, madidatabuf.StandardMotion.Threshold2);

      Ba_Motion_Level_Based_MADI_Dynamic_Thresholds[2][0] = madidatabuf.HighMotion.Threshold0;
      Ba_Motion_Level_Based_MADI_Dynamic_Thresholds[2][1] = madidatabuf.HighMotion.Threshold1;
      Ba_Motion_Level_Based_MADI_Dynamic_Thresholds[2][2] = madidatabuf.HighMotion.Threshold2;

      Ba_Motion_Level_Based_MADI_Dynamic_Thresholds[3][0] = madidatabuf.PanMotion.Threshold0;
      Ba_Motion_Level_Based_MADI_Dynamic_Thresholds[3][1] = madidatabuf.PanMotion.Threshold1;
      Ba_Motion_Level_Based_MADI_Dynamic_Thresholds[3][2] = madidatabuf.PanMotion.Threshold2;

      Global_Mot_Threshold_lo = madidatabuf.GlobalMotionStandardThreshold;
      Global_Mot_Threshold_hi = madidatabuf.GlobalMotionHighThreshold;

      B_VerticalMotionVectorSum = madidatabuf.GlobalMotionPanThreshold;

      // madidatabuf.GlobalMotionInterpolation // ?

      gm_WriteRegByte(MADI_EXT_NOISE, madidatabuf.NoiseGain);

      gm_ClearAndSetBitsByte(MADI_GAIN, MADI_CHROMA_GAIN, (madidatabuf.ChromaGain & MADI_CHROMA_GAIN));
      gm_ClearAndSetBitsByte(MADI_GAIN, MADI_VERT_GAIN, ((madidatabuf.VerticalGain<<2) & MADI_VERT_GAIN));


      //madidatabuf.LumaAdaptiveEnable = 0; // ?

      //madidatabuf.StaticHistGain = 0; // ?

      gm_WriteRegWord(MADI_FILTER_CTRL4, ((madidatabuf.MVLHH << 8) & madidatabuf.MVLHL));
      gm_WriteRegWord(MADI_FILTER_CTRL5, ((madidatabuf.MVHHH << 8) & madidatabuf.MVHHL));
      gm_WriteRegWord(MADI_FILTER_CTRL8, ((madidatabuf.MCVLHH << 8) & madidatabuf.MCVLHL));
      gm_WriteRegWord(MADI_FILTER_CTRL9, ((madidatabuf.MCVHHH << 8) & madidatabuf.MCVHHL));

      if (madidatabuf.DeinterlacerDCDiEnable)
         gm_SetRegBitsByte(MVF_DCDI_CTRL1, MVF_DCDI_ENABLE);
      else
         gm_ClearRegBitsByte(MVF_DCDI_CTRL1, MVF_DCDI_ENABLE);

      gm_WriteRegByte(MVF_DCDI_CTRL4, (madidatabuf.DeinterlacerDCDiStrength & MVF_DCDI_PELCORR_TH));

      gm_SetRegBitsWord(HOST_CONTROL,ODP_FORCE_UPDATE);

      Bp_out[0] = TS_OK;
      *Bp_len = 1;
      gm_Print("TS_MADI_WRITE", 0);
   }
   else if (Bp_msg[0] == TS_MADI_EN)
   {
      gm_ClearAndSetBitsWord(MVF_CTRL, (MVF_SPATIAL_EN|AFM_VT_MADI_SEL), (BIT3|MVF_MADI_EN));
      gm_SetRegBitsWord(MOTION_PROC_CTRL, MADI_EN);
      gm_SetRegBitsWord(HOST_CONTROL,ODP_FORCE_UPDATE);

      dbgmsg("TS_MADI_EN \n", 0);

      Bp_out[0] = TS_OK;
      *Bp_len = 1;
   }
   else if (Bp_msg[0] == TS_MADI_DISABLE)
   {
      gm_ClearAndSetBitsWord(MVF_CTRL, MVF_MADI_EN, (MVF_SPATIAL_EN|AFM_VT_MADI_SEL));
      gm_ClearRegBitsWord(MOTION_PROC_CTRL, MADI_EN);
      gm_SetRegBitsWord(HOST_CONTROL,ODP_FORCE_UPDATE);

      dbgmsg("TS_MADI_DISABLE \n", 0);

      *Bp_len = 1;
      Bp_out[0] = TS_OK;
      gm_Print("TS_MADI_DISABLE", 0);
   }
   else if (Bp_msg[0] == TS_MADI_GET_NOISE)
   {
      DWORD Len;
      static DWORD debug_framenum;

      Bp_out[0] = TS_OK;
      ((MadiDynData_t *)(Bp_out+1))->NumOfFrames = Len = VCD_ExtractMadiDynData(((MadiDynData_t *)(Bp_out+1))->FrameDat, 512);
      debug_framenum += Len;
      ((MadiDynData_t *)(Bp_out+1))->FrameNum = debug_framenum;

      if (Len)
         *Bp_len = 1 + sizeof(MadiDynData_t) + (BYTE)Len * sizeof(MadiFrameData_t);
      else
         *Bp_len = 1 + 6; // ?

      gm_WriteRegWord(IMP_FLAGLINE, Len); /* debug only */

      gm_Printf("sizeof(MadiDynData_t) %d", sizeof(MadiDynData_t));
      gm_Printf("Len %d", Len);
      gm_Print("TS_MADI_GET_NOISE", 0);
   }
   else
   {
      dbgmsg("STTS_Handler_MADI: UNKNOWN COMMAND \n", 0);
      Bp_out[0] = TS_NOT_SUPPORTED;
      *Bp_len = 1;
   }



   return (BYTE)PORT_MSG_HNDLR_OK;
}

#endif

