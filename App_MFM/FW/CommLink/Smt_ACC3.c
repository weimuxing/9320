#include <mem.h>
#include "System\All.h"
#include "CommLink\smt.h"
#include "CommLink\SMT_ACC3.h"
#include "StdLibInc\gm_ACC3.h"

#ifdef USE_SMT

#if (FEATURE_ACC3 == ENABLE)

#define TS_ACC3_DEBUG_MSG
#if defined(TS_ACC3_DEBUG_MSG)
#define dbgmsg(a,b) gm_Printf(a,b)
#else
#define dbgmsg(a,b)
#endif

#define TS_ACC3_VER 0


#define ACC3_STATIC_0_MEM_START 		   0x32400L // from 0x32400 to 0x327FF, 1k bytes, 256x2
//#define ACC3_STATIC_1_MEM_START			 0x32800L // from 0x32800 to 0x32BFF, 1k bytes, 256x2, total 2k bytes

typedef enum
{
   TS_ACC_START,
   TS_ACC_STOP,
   TS_ACC_READ,
   TS_ACC_WRITE,
   TS_ACC_EN,
   TS_ACC_DISABLE,
   TS_ACC_READ_CRD,
   TS_ACC_READ_FLD,
   TS_ACC_WRITE_FLD,
   TS_ACC_WRITE_CWD
}TsACC3Cmd_t;

/*******************************************************************************
Name        : LLD_ACCFinalLUT
Description : Read/write ACC final LUT
Parameters  :   IsWrite - FALSE:read, TRUE:write
                *FinalLUT_p - pointer to ACC final LUT (memory must be allocated by higher level SW)
Assumptions :
Limitations :
Returns     : ST_ErrorCode_t type value :
                - ST_NO_ERROR if no error
                - ST_ERROR_TIMEOUT when reading LUT is hanged
*******************************************************************************/
gmt_RET_STAT LLD_ACCFinalLUT( BOOL IsWrite, SWORD * FinalLUT_p, WORD DestOffset, WORD BlockSize)
{
#define ACC3_OUTPUT_LUT_MEM_START            0x32000L
#define ACC3_LUT_MEM_SIZE 0x400

   // need to ensure ACC histogram ready interrupt
   // is paused before calling this function

   // need to ensure ACC FW is disabled so LUT flipping from VCPU is stopped before calling this function
   DWORD far* LUT_p = LINEAR_TO_FPTR_DW(ACC3_OUTPUT_LUT_MEM_START+(DestOffset/2));
   DWORD Ix = 0;


   if (IsWrite)
   {   //WRITING LUT

      //skip writing if ACC is enabled.
      if ((gm_ReadRegByte(ACC_CTRL)&ACC_WIN_CTRL) == 0)
      {
         DWORD AccCtrl;

         AccCtrl = gm_ReadRegDWord_LE(ACC_CTRL);

         // disable lut autogeneration to free FLUT for writing.
         gm_ClearRegBitsDWord(ACC_CTRL, ACC_LUT_AUTOGEN_EN);
         gm_SetRegBitsByte(OVP_UPDATE_CTRL, ACC_SYNC_UPDATE|OVP_SYNC_UPDATE);

         for (; Ix < ACC3_LUT_MEM_SIZE/sizeof(DWORD); Ix++)
         {
            *LUT_p++ = 0xffff&(DWORD)(*FinalLUT_p++);
         }

         // restore ACC_CTRL value
         gm_WriteRegDWord_LE(ACC_CTRL, AccCtrl);
         gm_SetRegBitsByte(OVP_UPDATE_CTRL, ACC_SYNC_UPDATE|OVP_SYNC_UPDATE);
      }
   }
   else
   {   //READING LUT

      //STOS_Clock_t Timeout = STOS_TimePlus(STOS_TimeNow(), ST_GetClocksPerSecond() / 10000); /* 100 microsec */
		#define WATCHDOG_TMO 100
      WORD W_TimeOut = gm_GetSystemTimeW() + WATCHDOG_TMO;
      WORD W_Count = 0;

      Ix = DestOffset/2;

      //LUT_p += Ix;

      //while(Ix < ACC3_LUT_MEM_SIZE/sizeof(DWORD))
      while (Ix < (DestOffset + BlockSize)/2)
      {
         if (gm_ReadRegByte(ACC_LUT_COMPUTE)& ACC_TRIGGER_LUT_COMPUTE)
         {//clear all downloaded LUT and start again if LUT is being generated
            if ((((W_TimeOut - gm_GetSystemTimeW()) & 0x8000) == 0))
            {// exit from the cycle by time out
               gm_Printf("~ ~ ~ ~ ~ERR_TMO", 0);
               return ERR_TMO;
            }
            Ix = 0;
            continue;
         }
         //memcpy(FinalLUT_p++,(SWORD *)LUT_p++,sizeof(SWORD));
         //_fmemcpy(FinalLUT_p++,(SWORD *)LUT_p++,sizeof(SWORD));
         FinalLUT_p[W_Count] = (SWORD)LUT_p[W_Count];
         Ix++;
         W_Count++;
      }
   }

   return (OK);
}



#pragma argsused
BYTE ACC3_MsgHandler(MsgPacket_t *message, BYTE comm_use)
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
   B_ContextStatus = SMT_GetBuffer( B_TransId, (BYTE far * far *)&Bp_Context, ACC3_FULL_SIZE);

   // If B_ContextStatus is FIRST_REQUEST clear Bp_Context buffer
   if ( B_ContextStatus == FIRST_REQUEST )
   {
      // Clear Buffer
      for ( W_ContextCnt = 0; W_ContextCnt < ACC3_FULL_SIZE; W_ContextCnt++ )
         *(Bp_Context + W_ContextCnt) = 0;

      W_CurrentCmd = (Bp_msg[1] << 8) + Bp_msg[0];

      // Save command length
      *(Bp_Context + 0) = message->len - SMT_HEADER_SIZE - CHECKSUM_SIZE;
      //*(Bp_Context + 1) = 0;

      // Only write command need to be saved
      if ((W_CurrentCmd == TS_ACC_WRITE) || (W_CurrentCmd == TS_ACC_WRITE_FLD) || (W_CurrentCmd == TS_ACC_WRITE_CWD))
      {
         // Save data into buffer
         for (W_ContextCnt = 0; W_ContextCnt < *(Bp_Context + 0); W_ContextCnt++)
         {
            *(Bp_Context + 2 + W_ContextCnt) = Bp_msg[W_ContextCnt];
         }
      }
      else
      {
         *(Bp_Context + 2) = Bp_msg[0];
         *(Bp_Context + 3) = Bp_msg[1];
      }
      gm_Printf("FIRST_REQUEST data size = %d", (((WORD)*(Bp_Context + 1))<<8)|(*(Bp_Context + 0)));
   }
   else if ( B_ContextStatus == NEXT_REQUEST )
   {
      WORD W_Temp;

      W_CurrentCmd = ((*(Bp_Context + 3))<<8) + (*(Bp_Context + 2));

      // Previou data count
      W_Temp = (*(Bp_Context + 1) << 8) + *(Bp_Context + 0);

      // Only write command need to be saved
      if ((W_CurrentCmd == TS_ACC_WRITE) || (W_CurrentCmd == TS_ACC_WRITE_FLD) || (W_CurrentCmd == TS_ACC_WRITE_CWD))
      {
         // Save data into buffer
         for (W_ContextCnt = 0; W_ContextCnt < (message->len - SMT_HEADER_SIZE - CHECKSUM_SIZE); W_ContextCnt++)
         {
            *(Bp_Context + 2 + W_Temp + W_ContextCnt) = Bp_msg[W_ContextCnt];
         }
      }

      // Update command length
      //W_Temp = (*(Bp_Context + 1) << 8) + *(Bp_Context + 0) + message->len - SMT_HEADER_SIZE - CHECKSUM_SIZE;
      W_Temp += message->len - SMT_HEADER_SIZE - CHECKSUM_SIZE;
      *(Bp_Context + 0) = (BYTE)(W_Temp&0xFF);
      *(Bp_Context + 1) = (BYTE)((W_Temp >> 8)&0xFF);
      gm_Printf("NEXT_REQUEST data size = %d", (((WORD)*(Bp_Context + 1))<<8)|(*(Bp_Context + 0)));
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
         _fmemcpy(&Sp_I2CSlavePtr->Buffer[Start_of_GprobeCmd_in_DDC2BiMsg], Bp_msg, Bp_msg[0]-1);
         gm_WriteCmdPkt((BYTE far*)&Sp_I2CSlavePtr->Buffer[Start_of_GprobeCmd_in_DDC2BiMsg], comm_use);
      }
      else
      {
         gm_WriteCmdPkt((BYTE far*)Bp_msg, comm_use);
      }
      //gm_Printf("NO_MEMORYNO_MEMORY", 0);
      return PORT_MSG_HNDLR_SELFCARED;//ERR_PARAM;
   }



   if (W_CurrentCmd == TS_ACC_START)
   {
      Bp_out[0] = TS_OK;
      Bp_out[1] = TS_ACC3_VER;
      *Bp_len = 2;
      dbgmsg("TS_ACC_START %s\n","OK");
   }
   else if (W_CurrentCmd == TS_ACC_STOP)
   {
      Bp_out[0] = TS_OK;
      *Bp_len = 1;
      dbgmsg("TS_ACC_STOP", 0);
   }
   else if (W_CurrentCmd == TS_ACC_READ)
   {
      WORD W_DataSent;
      VIP_ACCSWParameters_t ACC3Params;
      VIP_ACCHWParameters_t ACC3Options;

      gm_ACC3GetParams((VIP_ACCSWParameters_t far *)&ACC3Params);
      gm_ACC3GetOptions((VIP_ACCHWParameters_t far *)&ACC3Options);

      W_DataSent = ((*(Bp_Context + 5))<<8) + (*(Bp_Context + 4));

      if (W_DataSent == 0)
      {
         Bp_out[0] = TS_OK;

         // size
         Bp_out[1] = 0xF; // 13;
         Bp_out[2] = 2; // 0;
         Bp_out[3] = 0;
         Bp_out[4] = 0;

         // signature
         Bp_out[5] = 14; // 26; // 25;
         Bp_out[6] = 0;
         Bp_out[7] = 0;
         Bp_out[8] = 0;

         // version
         Bp_out[9] = 0;
         Bp_out[10] = 0;
         Bp_out[11] = 0;
         Bp_out[12] = 0;

         // HistStretchEnable
         Bp_out[13] = ACC3Params.HistStretchEnable;

         // Cumulative_Black_Pixels
         Bp_out[14] = (BYTE)ACC3Params.Cumulative_Black_Pixels;
         Bp_out[15] = (BYTE)(ACC3Params.Cumulative_Black_Pixels >> 8);

         // Cumulative_White_Pixels
         Bp_out[16] = (BYTE)ACC3Params.Cumulative_White_Pixels;
         Bp_out[17] = (BYTE)(ACC3Params.Cumulative_White_Pixels >> 8);

         // Max_Black_Bin
         Bp_out[18] = (BYTE)ACC3Params.Max_Black_Bin;
         Bp_out[19] = (BYTE)(ACC3Params.Max_Black_Bin >> 8);

         // Min_White_Bin
         Bp_out[20] = (BYTE)ACC3Params.Min_White_Bin;
         Bp_out[21] = (BYTE)(ACC3Params.Min_White_Bin >> 8);

         // ACC_Strength
         Bp_out[22] = ACC3Params.ACC_Strength;

         *Bp_len = 23;
         MoreData(1);
      }
      else if (W_DataSent < 5)
      { // WF[64]
         BYTE i, j;

         j = (W_DataSent - 1)*(NUMBER_OF_BINS/4);

         for (i = 0; i < NUMBER_OF_BINS/4; i++)
         {

            Bp_out[i*4] = (BYTE)ACC3Params.WF[i + j];
            Bp_out[i*4+1] = (BYTE)(ACC3Params.WF[i + j] >> 8);
            Bp_out[i*4+2] = (BYTE)(ACC3Params.WF[i + j] >> 16);
            Bp_out[i*4+3] = (BYTE)(ACC3Params.WF[i + j] >> 24);
         }

         *Bp_len = 64;
         MoreData(1);
      }
      else if (W_DataSent < 7)
      { // BinLim[64]
         BYTE i, j;

         j = (W_DataSent - 5)*(NUMBER_OF_BINS/2);

         for (i = 0; i < NUMBER_OF_BINS/2; i++)
         {
            Bp_out[i*2] = (BYTE)ACC3Params.BinLim[i + j];
            Bp_out[i*2+1] = (BYTE)(ACC3Params.BinLim[i + j] >> 8);
         }

         *Bp_len = 64;
         MoreData(1);
      }
      else if (W_DataSent == 7)
      { // ACCStaticLut[64]
         BYTE i;
         DWORD far * DWp_StaticLUT = LINEAR_TO_FPTR_DW(ACC3_STATIC_0_MEM_START);

         for (i = 0; i < NUMBER_OF_BINS; i++)
         {
				Bp_out[i] = (BYTE)(DWp_StaticLUT[i*4]|(DWp_StaticLUT[i*4+1]<<2)|(DWp_StaticLUT[i*4+2]<<4)|(DWp_StaticLUT[i*4+3]<<6));
         }
         *Bp_len = 64;
         MoreData(1);
      }
      else if (W_DataSent == 8)
      {
         // IVP_IP_MEASURE_EN
         Bp_out[0] = gm_ReadRegWord(IMP_IP_MEASURE_CTRL)& IMP_IP_MEASURE_EN;

         // IVP_IP_MEAS_SC_CHK_EN
         Bp_out[1] = gm_ReadRegWord(IMP_IP_MEAS_SC_CTRL) & IMP_IP_MEAS_SC_CHK_EN;

         // IVP_IP_MEAS_SC_SOURCE
         Bp_out[2] = gm_ReadRegWord(IMP_IP_MEAS_SC_CTRL) & IMP_IP_MEAS_SC_SOURCE;

         // IVP_IP_MEAS_SC_THRESH
         Bp_out[3] = (BYTE)(gm_ReadRegWord(IMP_IP_MEAS_SC_CTRL) >> 4);
         Bp_out[4] = (BYTE)(gm_ReadRegWord(IMP_IP_MEAS_SC_CTRL) >> 11);


         // HpfAmpThreshY
         Bp_out[5] = (BYTE)gm_ReadRegDWord_LE(ACC_HPF_AMP_THRESH);
         Bp_out[6] = (BYTE)(gm_ReadRegDWord_LE(ACC_HPF_AMP_THRESH) >> 8);

         // HpfAmpThreshUV
         Bp_out[7] = (BYTE)(gm_ReadRegDWord_LE(ACC_HPF_AMP_THRESH) >> 16);
         Bp_out[8] = (BYTE)(gm_ReadRegDWord_LE(ACC_HPF_AMP_THRESH) >> 24);

         // ClampBlack
         Bp_out[9] = ACC3Options.ClampBlack;

         // LutTempFilterEnable
         Bp_out[10] = ACC3Options.LutTempFilterEnable;

         // YclNlsatScale
         Bp_out[11] = (BYTE)((gm_ReadRegDWord_LE(YCL_CTRL) & YCL_NLSAT_SCALE) >> 3);

         // YclNlsatEn
         Bp_out[12] = (gm_ReadRegByte(YCL_CTRL) & YCL_NLSAT_EN) >> 6;

         // YclNlsatTable[8]
         Bp_out[13] = (BYTE)gm_ReadRegDWord_LE(YCL_NLSAT_LUT0_LUT3);
         Bp_out[14] = (BYTE)(gm_ReadRegDWord_LE(YCL_NLSAT_LUT0_LUT3) >> 8);
         Bp_out[15] = (BYTE)(gm_ReadRegDWord_LE(YCL_NLSAT_LUT0_LUT3) >> 16);
         Bp_out[16] = (BYTE)(gm_ReadRegDWord_LE(YCL_NLSAT_LUT0_LUT3) >> 24);
         Bp_out[17] = (BYTE)gm_ReadRegDWord_LE(YCL_NLSAT_LUT4_LUT7);
         Bp_out[18] = (BYTE)(gm_ReadRegDWord_LE(YCL_NLSAT_LUT4_LUT7) >> 8);
         Bp_out[19] = (BYTE)(gm_ReadRegDWord_LE(YCL_NLSAT_LUT4_LUT7) >> 16);
         Bp_out[20] = (BYTE)(gm_ReadRegDWord_LE(YCL_NLSAT_LUT4_LUT7) >> 24);

         // YclUseKY
         Bp_out[21] = (gm_ReadRegByte(YCL_CTRL) & YCL_USE_KY) >> 1;

         // CompLutLpfEn
         Bp_out[22] = (BYTE)((gm_ReadRegWord(ACC_LUT_CTRL) & ACC_COMP_LUT_LPF_EN) >> 8);

         // CheckScEnable
         Bp_out[23] = gm_ReadRegByte(ACC_SC_CTRL) & ACC_SC_CHK_EN;

         // SceneChangeResetEnable
         Bp_out[24] = (gm_ReadRegByte(ACC_LUT_CTRL) & ACC_TF_SC_RESET) >> 1;

         // TfFrameNum
         Bp_out[25] = (gm_ReadRegByte(ACC_LUT_CTRL) & ACC_TF_FRAME_NUM) >> 4;

         // YCLinkEnable
         Bp_out[26] = gm_ReadRegByte(YCL_CTRL) & YCL_ENABLE;

         // SceneChangeSource

         Bp_out[27] = (gm_ReadRegByte(ACC_SC_CTRL) & ACC_SC_SOURCE) >> 1;

         // LeftSlope
         Bp_out[28] = (BYTE)((gm_ReadRegDWord_LE(YCL_CTRL) & YCL_SCALE_MIN) >> 16);

         // RightSlope
         Bp_out[29] = (BYTE)((gm_ReadRegWord(YCL_CTRL) & YCL_SCALE_MAX) >> 8);

         // ScaleThresh
         Bp_out[30] = (BYTE)((gm_ReadRegDWord_LE(YCL_CTRL) & YCL_SCALE_THRESH) >> 24);

         // YCLinkGain
         Bp_out[31] = (BYTE)((gm_ReadRegDWord_LE(YCL_CTRL) & YCL_SCALE_FACTOR) >> 20);

         // SceneChangeThreshold
         {
            WORD W_Temp = (gm_ReadRegWord(ACC_SC_CTRL) & ACC_SC_THRESH) >> 3;
            Bp_out[32] = (BYTE)W_Temp;
            Bp_out[33] = (BYTE)(W_Temp >> 8);
         }

         // XvYccEnable
         Bp_out[34] = (BYTE)((gm_ReadRegDWord_LE(ACC_CTRL) & ACC_XVYCC_CLAMP_EN) >> 27);

         // YLpf[5]
         Bp_out[35] = gm_ReadRegByte(ACC_Y_LPF_COEF_A) & 0x7F;
         Bp_out[36] = (BYTE)((gm_ReadRegWord(ACC_Y_LPF_COEF_A) >> 8) & 0x7F);
         Bp_out[37] = (BYTE)((gm_ReadRegDWord_LE(ACC_Y_LPF_COEF_A) >> 16) & 0x7F);
         Bp_out[38] = (BYTE)((gm_ReadRegDWord_LE(ACC_Y_LPF_COEF_A) >> 24) & 0x7F);
         Bp_out[39] = gm_ReadRegByte(ACC_Y_LPF_COEF_B) & 0x7F;

         // CLpf[5]
         Bp_out[40] = gm_ReadRegByte(ACC_C_LPF_COEF_A) & 0x7F;
         Bp_out[41] = (BYTE)((gm_ReadRegWord(ACC_C_LPF_COEF_A) >> 8) & 0x7F);
         Bp_out[42] = (BYTE)((gm_ReadRegDWord_LE(ACC_C_LPF_COEF_A) >> 16) & 0x7F);
         Bp_out[43] = (BYTE)((gm_ReadRegDWord_LE(ACC_C_LPF_COEF_A) >> 24) & 0x7F);
         Bp_out[44] = gm_ReadRegByte(ACC_C_LPF_COEF_B) & 0x7F;

         // ACCYLpfFlt
         Bp_out[45] = gm_ReadRegByte(ACC_LPF_CTRL) & ACC_Y_LPF_FLT;

         // ACCCLpfFlt
         Bp_out[46] = (gm_ReadRegByte(ACC_LPF_CTRL) & ACC_C_LPF_FLT) >> 4;


         // UseLpfY
         Bp_out[47] = (BYTE)((gm_ReadRegDWord_LE(ACC_CTRL) & ACC_USE_LPF_Y) >> 15);

         // StaticLUTEn
         Bp_out[48] = (gm_ReadRegByte(ACC_LUT_CTRL) & ACC_STATIC_LUT_EN) >> 3;

         // LACCEnable
         Bp_out[49] = gm_ReadRegByte(LACC_CTRL1) & LACC_EN;

         // LACCTextureThresh
         {
            WORD W_Temp = (gm_ReadRegWord(LACC_CTRL1) & LACC_TEXTURE_THRES) >> 1;
            Bp_out[50] = (BYTE)W_Temp;
            Bp_out[51] = (BYTE)(W_Temp >> 8);
         }

         // LACCDiffShift
         Bp_out[52] = (BYTE)((gm_ReadRegWord(LACC_CTRL1) & LACC_DIFF_SHIFT) >> 13);

         // LACCTextureOffset
         Bp_out[53] = (BYTE)((gm_ReadRegDWord_LE(LACC_CTRL1) & LACC_TEXTURE_OFFSET) >> 16);

         // LACCTextureGain
         Bp_out[54] = (BYTE)((gm_ReadRegDWord_LE(LACC_CTRL1) & LACC_TEXTURE_GAIN) >> 24);

         // LACCSmoothThresh
         Bp_out[55] = gm_ReadRegByte(LACC_CTRL2) & LACC_SMOOTH_THRES;

         // LACCSmoothness
         Bp_out[56] = (BYTE)((gm_ReadRegWord(LACC_CTRL2) & LACC_SMOOTHNESS) >> 8);

         *Bp_len = 57;
         MoreData(0);
      }
      else
      {
         gm_Printf("Data Sending Error ...", W_DataSent);
         Bp_out[0] = TS_NOT_SUPPORTED;
         *Bp_len = 1;
      }




      W_DataSent++;

      *(Bp_Context + 4) = (BYTE)(W_DataSent&0xFF);
      *(Bp_Context + 5) = (BYTE)((W_DataSent >> 8)&0xFF);

      dbgmsg("TS_ACC_READ", 0);

   }

   else if (W_CurrentCmd == TS_ACC_WRITE)
   {
      if (!MoreDataExpected())
      {
         VIP_ACCSWParameters_t ACC3Params;
         VIP_ACCHWParameters_t ACC3Options;
         WORD W_Temp;

         gm_ACC3GetParams((VIP_ACCSWParameters_t far *)&ACC3Params);
         gm_ACC3GetOptions((VIP_ACCHWParameters_t far *)&ACC3Options);


         // size 4, 5, 6, 7
         // Signature 8, 9, 10, 11
         // Version ID 12, 13, 14, 15

         // CSD
         // HistStretchEnable
         ACC3Params.HistStretchEnable = ((WORD) *(Bp_Context + 16))&0xFF;
         //gm_Printf("16 = 0x%x", *(Bp_Context + 16));


         // Cumulative_Black_Pixels
         ACC3Params.Cumulative_Black_Pixels = (((WORD) *(Bp_Context + 18)) << 8) | *(Bp_Context + 17);

         // Cumulative_White_Pixels
         ACC3Params.Cumulative_White_Pixels = (((WORD) *(Bp_Context + 20)) << 8) | *(Bp_Context + 19);

         // Max_Black_Bin
         ACC3Params.Max_Black_Bin = (((WORD) *(Bp_Context + 22)) << 8) | *(Bp_Context + 21);

         // Min_White_Bin
         ACC3Params.Min_White_Bin = (((WORD) *(Bp_Context + 24)) << 8) | *(Bp_Context + 23);

         // ACC_Strength
         ACC3Params.ACC_Strength = *(Bp_Context + 25);

         {
            BYTE i;

            // WF[64]
            for (i = 0; i < NUMBER_OF_BINS; i++)
            {
               ACC3Params.WF[i] = (DWORD) ((DWORD)*(Bp_Context + 29 + i*4)<24)| ((DWORD)*(Bp_Context + 28 + i*4)<< 16) | ((DWORD)*(Bp_Context + 27 + i*4)<<8) | *(Bp_Context + 26 + i*4);
            }

            // the latest point is 281; 63*4+29

            // BinLim
            for (i = 0; i < NUMBER_OF_BINS; i++)
            {
               ACC3Params.BinLim[i] = (((WORD) *(Bp_Context + 283 + i * 2)) << 8) | *(Bp_Context + 282 + i * 2);
            }

            // the latest point is 409; 63*2+283
            // ACCStaticLut 1*64
            {
               DWORD far * DWp_StaticLUT = LINEAR_TO_FPTR_DW(ACC3_STATIC_0_MEM_START);
               BYTE far * Bp_Source = Bp_Context + 410;
               BYTE i;

               // must pass through to update
               gm_SetRegBitsDWord(OVP_CTRL, OVP_OUT_MUX);

               for (i = 0; i < NUMBER_OF_BINS; i++)
               {
						//DWp_StaticLUT[i*4] = (DWORD)Bp_Source[i];
						DWp_StaticLUT[i*4] = (DWORD)(Bp_Source[i]&(BIT0|BIT1));
                  DWp_StaticLUT[i*4+1] = (DWORD)((Bp_Source[i]&(BIT2|BIT3))>>2);
                  DWp_StaticLUT[i*4+2] = (DWORD)((Bp_Source[i]&(BIT4|BIT5))>>4);
                  DWp_StaticLUT[i*4+3] = (DWORD)((Bp_Source[i]&(BIT6|BIT7))>>6);
               }

               gm_ClearRegBitsDWord(OVP_CTRL, OVP_OUT_MUX);
            }

            // the latest point is 473; 63*1+410
         }

         // IVP_IP_MEASURE_EN
         ACC3Options.IVPMeasureEn = *(Bp_Context + 474);

         // IVP_IP_MEAS_SC_CHK_EN
         ACC3Options.IVPScCheckEn = *(Bp_Context + 475);

         // IVP_IP_MEAS_SC_SOURCE
         ACC3Options.IVPScSource = *(Bp_Context + 476);

         // IVP_IP_MEAS_SC_THRESH
         ACC3Options.IVPScThresh = ((WORD)(*(Bp_Context + 478))<<8) | (*(Bp_Context + 477));

         // HpfAmpThreshY
         // HpfAmpThreshUV
         ACC3Options.HpfAmpThreshY = ((WORD)(*(Bp_Context + 480)) << 8) | (*(Bp_Context + 479));
         ACC3Options.HpfAmpThreshUV = ((WORD)(*(Bp_Context + 482)) << 8) | (*(Bp_Context + 481));

         // ClampBlack
         ACC3Options.ClampBlack = *(Bp_Context + 483);

         // LutTempFilterEnable
         ACC3Options.LutTempFilterEnable= *(Bp_Context + 484);

         // YclNlsatScale
         ACC3Options.YclNlsatScale = *(Bp_Context + 485);

         // YclNlsatEn
         ACC3Options.YclNlsatEn = *(Bp_Context + 486);

         // YclNlsatTable[8]
         ACC3Options.YclNlsatTable[0] = *(Bp_Context + 487);
         ACC3Options.YclNlsatTable[1] = *(Bp_Context + 488);
         ACC3Options.YclNlsatTable[2] = *(Bp_Context + 489);
         ACC3Options.YclNlsatTable[3] = *(Bp_Context + 490);
         ACC3Options.YclNlsatTable[4] = *(Bp_Context + 491);
         ACC3Options.YclNlsatTable[5] = *(Bp_Context + 492);
         ACC3Options.YclNlsatTable[6] = *(Bp_Context + 493);
         ACC3Options.YclNlsatTable[7] = *(Bp_Context + 494);

         // YclUseKY
         ACC3Options.YclUseKY = *(Bp_Context + 495);

         // CompLutLpfEn
         ACC3Options.CompLutLpfEn = *(Bp_Context + 496);

         // CheckScEnable
         ACC3Options.CheckScEnable = *(Bp_Context + 497);

         // SceneChangeResetEnable
         ACC3Options.SceneChangeResetEnable = *(Bp_Context + 498);

         // TfFrameNum
         ACC3Options.TfFrameNum = *(Bp_Context + 499);

         // YCLinkEnable
         ACC3Options.YCLinkEnable = *(Bp_Context + 500);

         // SceneChangeSource
         ACC3Options.SceneChangeSource = *(Bp_Context + 501);

         // LeftSlope
         ACC3Options.LeftSlope = *(Bp_Context + 502);

         // RightSlope
         ACC3Options.RightSlope = *(Bp_Context + 503);

         // ScaleThresh
         ACC3Options.ScaleThresh = *(Bp_Context + 504);

         // YCLinkGain
         ACC3Options.YCLinkGain = *(Bp_Context + 505);

         // SceneChangeThreshold
         ACC3Options.SceneChangeThreshold = ((WORD)(*(Bp_Context + 507)) << 8) | *(Bp_Context + 506);

         // XvYccEnable
         ACC3Options.XvYccEnable = *(Bp_Context + 508);

         // YLpf[5]
         ACC3Options.YLpf[0] = *(Bp_Context + 509);
         ACC3Options.YLpf[1] = *(Bp_Context + 510);
         ACC3Options.YLpf[2] = *(Bp_Context + 511);
         ACC3Options.YLpf[3] = *(Bp_Context + 512);
         ACC3Options.YLpf[4] = *(Bp_Context + 513);

         // CLpf[5]
         ACC3Options.CLpf[0] = *(Bp_Context + 514);
         ACC3Options.CLpf[1] = *(Bp_Context + 515);
         ACC3Options.CLpf[2] = *(Bp_Context + 516);
         ACC3Options.CLpf[3] = *(Bp_Context + 517);
         ACC3Options.CLpf[4] = *(Bp_Context + 518);

         // ACCYLpfFlt
         ACC3Options.ACCYLpfFlt = *(Bp_Context + 519);

         // ACCCLpfFlt
         ACC3Options.ACCCLpfFlt = *(Bp_Context + 520);

         // UseLpfY
         ACC3Options.UseLpfY = *(Bp_Context + 521);

         // StaticLUTEn
         ACC3Options.StaticLUTEn = *(Bp_Context + 522);

         // LACCEnable
         ACC3Options.LACCEnable = *(Bp_Context + 523);

         // LACCTextureThresh
         ACC3Options.LACCTextureThresh = ((WORD)(*(Bp_Context + 525)) << 8)| (*(Bp_Context + 524));

         // LACCDiffShift
         ACC3Options.LACCDiffShift = *(Bp_Context + 526);

         // LACCTextureOffset
         ACC3Options.LACCTextureOffset = *(Bp_Context + 527);

         // LACCTextureGain
         ACC3Options.LACCTextureGain = *(Bp_Context + 528);

         // LACCSmoothThresh
         ACC3Options.LACCSmoothThresh = *(Bp_Context + 529);

         // LACCSmoothness
         ACC3Options.LACCSmoothness = *(Bp_Context + 530);

         //gm_Printf("530 = 0x%x", *(Bp_Context + 530));

         gm_ACC3SetParams(AccParamTableSelect, &ACC3Params, sizeof(VIP_ACCSWParameters_t));

         gm_ACC3SetParams(AccOptionsTableSelect, &ACC3Options, sizeof(VIP_ACCHWParameters_t));
      }


      Bp_out[0] = TS_OK;
      *Bp_len = 1;

      dbgmsg("TS_ACC_WRITE", 0);
   }
   else if (W_CurrentCmd == TS_ACC_EN)
   {
      gm_SetRegBitsDWord(ACC_CTRL , (ACC_PD_n | ACC_ENABLE));// enable ACC3

      gm_WriteRegWord(HOST_CONTROL,ODP_SYNC_UPDATE);

      Bp_out[0] = TS_OK;
      *Bp_len = 1;
      dbgmsg("TS_ACC_EN \n", 0);
   }
   else if (W_CurrentCmd == TS_ACC_DISABLE)
   {
      gm_ClearRegBitsDWord(ACC_CTRL , (ACC_PD_n | ACC_ENABLE));// disable ACC3

      gm_WriteRegWord(HOST_CONTROL,ODP_SYNC_UPDATE);


      Bp_out[0] = TS_OK;
      *Bp_len = 1;
      dbgmsg("TS_ACC_DISABLE \n", 0);
   }
   else if (W_CurrentCmd == TS_ACC_READ_CRD)
   {
      WORD W_DataSent;

      W_DataSent = ((*(Bp_Context + 5))<<8) + (*(Bp_Context + 4));

      if (W_DataSent == 0)
      {
         Bp_out[0] = TS_OK;

         // ACC_SC_STATUS
         Bp_out[1] = gm_ReadRegByte(ACC_STATUS)&ACC_SC_STATUS;

         *Bp_len = 2;
         MoreData(1);
      }
      else if (W_DataSent < 9)
      {
         BYTE B_Offset = W_DataSent - 1;

         // 4 * 64 = 256
         // Stop interrupt ...
         // LLD_ACCSetInterruptMask(ch,FALSE);
         LLD_ACCReadInputHistogram(Bp_out, B_Offset*32, 32);
         // Restart interrupt ...
         // LLD_ACCSetInterruptMask(ch,TRUE);

         *Bp_len = 32;
         MoreData(1);

      }
      else if (W_DataSent == 9)
      {
         // ACC_AVG_IN_BRIGHTNESS 15:0
         Bp_out[0] = (BYTE) (gm_ReadRegDWord_LE(ACC_AVG_BRIGHTNESS)&ACC_AVG_IN_BRIGHTNESS);
         Bp_out[1] = (BYTE) ((gm_ReadRegDWord_LE(ACC_AVG_BRIGHTNESS)&ACC_AVG_IN_BRIGHTNESS) >> 8);
         // ACC_AVG_OUT_BRIGHTNESS 31:16
         Bp_out[2] = (BYTE) ((gm_ReadRegDWord_LE(ACC_AVG_BRIGHTNESS)&ACC_AVG_OUT_BRIGHTNESS) >> 16);
         Bp_out[3] = (BYTE) ((gm_ReadRegDWord_LE(ACC_AVG_BRIGHTNESS)&ACC_AVG_OUT_BRIGHTNESS) >> 24);

         *Bp_len = 4;
         MoreData(1);
      }
      else if ((W_DataSent > 9) && (W_DataSent < 14))
      {
         // 2 * 64 = 128
         // Bin slope

         BYTE B_Offset = W_DataSent - 10;

         //LLD_ACCSetInterruptMask(ch,FALSE);
         //err = LLD_ACCReadBinSlopes(AccCrd->BinSlope);
         LLD_ACCReadBinSlopes((WORD *)Bp_out, B_Offset*32/sizeof(WORD), 32/sizeof(WORD));
         //LLD_ACCSetInterruptMask(ch,TRUE);

         *Bp_len = 32;
         MoreData(1);

      }
      else if (W_DataSent == 14)
      {
         // ACC_WIN_CTRL
         Bp_out[0] = gm_ReadRegWord(ACC_CTRL)&ACC_WIN_CTRL;

         // DynamicLUTEnable
         Bp_out[1] = (BYTE)((gm_ReadRegWord(ACC_LUT_CTRL)&ACC_DYN_LUT_EN) >> 2);

         *Bp_len = 2;
         MoreData(0);
      }
      else
      {
         gm_Printf("Data Sending Error ...", W_DataSent);
      }

      W_DataSent++;

      *(Bp_Context + 4) = (BYTE)(W_DataSent&0xFF);
      *(Bp_Context + 5) = (BYTE)((W_DataSent >> 8)&0xFF);



      dbgmsg("TS_ACC_READ_CRD \n", 0);
   }
   else if (W_CurrentCmd == TS_ACC_READ_FLD)
   {
      WORD W_DataSent;
      BYTE B_Remain = 0;

      W_DataSent = ((*(Bp_Context + 5))<<8) + (*(Bp_Context + 4));

      //gm_Printf("W_DataSent %d", W_DataSent);

      if ((512 - W_DataSent) < 100)
         B_Remain = 512 - W_DataSent;
      else
         B_Remain = 100;

      if (W_DataSent)
      {
         //gm_ReadSRAMBlock(gmd_TAB_ACC_OUT_LUT, gmd_HIGH_PRIORITY_MODE,
         //	(WORD far*)(Bp_out), W_DataSent, B_Remain);
         LLD_ACCFinalLUT(0, (SWORD *)(Bp_out), W_DataSent, B_Remain);

         *Bp_len = B_Remain ;
      }
      else
      { // First read need to add response ...
         Bp_out[0] = TS_OK;
         //gm_ReadSRAMBlock(gmd_TAB_ACC_OUT_LUT, gmd_HIGH_PRIORITY_MODE,
         //	(WORD far*)(Bp_out+1), W_DataSent, B_Remain);
         LLD_ACCFinalLUT(0, (SWORD *)(Bp_out+1), W_DataSent, B_Remain);

         *Bp_len = B_Remain + 1;
      }

      if (B_Remain == 100)
         MoreData(1);
      else
      {
         MoreData(0);
         Bp_out[B_Remain] = (BYTE)((gm_ReadRegWord(ACC_CTRL)&ACC_LUT_INDEX) >> 5);
         Bp_out[B_Remain + 1] = (BYTE)((gm_ReadRegWord(ACC_CTRL)&ACC_LUT_AUTOGEN_EN) >> 12);
         *Bp_len += 2;
      }


      W_DataSent += B_Remain;

      *(Bp_Context + 4) = (BYTE)(W_DataSent&0xFF);
      *(Bp_Context + 5) = (BYTE)((W_DataSent >> 8)&0xFF);

      dbgmsg("TS_ACC_READ_FLD W_DataSent %d\n", W_DataSent);
   }
   else if (W_CurrentCmd == TS_ACC_WRITE_FLD)
   {
      if (!MoreDataExpected())
      {
         gm_ClearRegBitsWord(ACC_CTRL, ACC_LUT_AUTOGEN_EN);
         gm_WriteRegWord(HOST_CONTROL, ODP_FORCE_UPDATE);

         //Load prepared data to ACC table
         if (LLD_ACCFinalLUT(1,(SWORD *) (Bp_Context + 4), 0, 0) != OK)
            return PORT_MSG_HNDLR_ERROR;

         if (*(Bp_Context + 516))
            gm_SetRegBitsWord( ACC_CTRL, ACC_LUT_INDEX);
         else
            gm_ClearRegBitsWord( ACC_CTRL, ACC_LUT_INDEX);

         if (*(Bp_Context + 517))
            gm_SetRegBitsWord( ACC_CTRL, ACC_LUT_AUTOGEN_EN);
         else
            gm_ClearRegBitsWord( ACC_CTRL, ACC_LUT_AUTOGEN_EN);

         gm_WriteRegWord(HOST_CONTROL, ODP_FORCE_UPDATE);
      }

      Bp_out[0] = TS_OK;
      *Bp_len = 1;
      dbgmsg("TS_ACC_WRITE_FLD \n", 0);
   }
   else if (W_CurrentCmd == TS_ACC_WRITE_CWD)
   {
      // WinCtrl
      gm_ClearAndSetBitsWord(ACC_CTRL, ACC_WIN_CTRL, *(Bp_Context + 4));

      // DynLutEn
      if (*(Bp_Context + 5))
         gm_SetRegBitsWord(ACC_LUT_CTRL, ACC_DYN_LUT_EN);
      else
         gm_ClearRegBitsWord(ACC_LUT_CTRL, ACC_DYN_LUT_EN);

      gm_WriteRegWord(HOST_CONTROL,ODP_SYNC_UPDATE);

      Bp_out[0] = TS_OK;
      *Bp_len = 1;
      dbgmsg("TS_ACC_WRITE_CWD \n", 0);
   }
   else
   {
      dbgmsg("STTS_Handler_ACC3: UNKNOWN COMMAND \n", 0);
      Bp_out[0] = TS_NOT_SUPPORTED;
      *Bp_len = 1;
   }


   return (BYTE)PORT_MSG_HNDLR_OK;
}

#endif // (FEATURE_ACC3 == ENABLE)

#endif
