#include <mem.h>
#include "System\All.h"
#include "CommLink\smt.h"
#include "CommLink\SMT_TNR.h"
#include "StdLibInc\gm_TNR.h"

#ifdef USE_SMT
#if (FEATURE_TNR == REV3)

#define TNR_DEBUG_MSG
#if defined(TNR_DEBUG_MSG)
#define dbgmsg(a,b) gm_Printf(a,b)
#else
#define dbgmsg(a,b)
#endif


#define TS_TNR_VER 0

typedef enum
{
   TS_TNR_START,
   TS_TNR_STOP,
   TS_TNR_READ,
   TS_TNR_WRITE,
   TS_TNR_EN,
   TS_TNR_DISABLE,
   TS_TNR_READ_DYNAMIC
}TsTnrCmd_t;

typedef struct
{
   BYTE InternalNoiseMeter;
   BYTE ExternalNoiseMeter;
   BYTE Histogram[16];
} TnrDynDat_t;

#pragma pack(push)
#pragma pack(1)
typedef struct
{//
   DWORD CurrentFrame;//    frame counter
   WORD Len;//    number of TNR_FrameData table entries (next field)
   TnrDynDat_t TNR_FrameData[1];//16*1    Noise_Hist_bin0-15
}TsTnrDynamic_t;
#pragma pack(pop)

extern VQ_TNR_Parameters_t ROM TNRParams_High;
extern VQ_TNR_Parameters_t ROM TNRParams_Medium;
extern VQ_TNR_Parameters_t ROM TNRParams_Low;

// default VQData tables
VQ_TNR_Parameters_t SMT_RAM TNRDefaultVQTable;
VQ_TNR_Parameters_t ROM TNRParams_Default = 
{
   0,		// EnableGlobalNoiseAdaptive
   0,		// EnableGlobalMotionAdaptive
   3,		// GlobalMotionStdTh
   4800,//GlobalMotionHighTh
   17,//GlobalMotionPanTh
   -1,//GlobalMotionStd
   -2,//GlobalMotionHigh
   -3,//GlobalMotionPan
   // EnableSNR, SNRLumaSensitivity, SNRLumaStrength, SNRChromaSensitivity, SNRChromaStrength,
   // YKMin, UVKMin, YGrad, FleshtoneGrad, ChromaGrad

   {{// 0
         0, 63, 2, 63, 2, 4, 4, 48, 52, 48,
      },
      {// 1
         0, 1, 2, 63, 2, 31, 31, 255, 255, 255,
      },
      {// 2
         0, 2, 2, 63, 2, 31, 31, 255, 255, 255,
      },
      {// 3
         0, 3, 2, 63, 2, 31, 31, 255, 255, 255,
      },
      {// 4
         0, 4, 2, 63, 2, 31, 31, 255, 255, 255,
      },
      {// 5
         0, 5, 2, 63, 2, 31, 31, 255, 255, 255,
      },
      {// 6
         0, 6, 2, 63, 2, 31, 31, 255, 255, 255,
      },
      {// 7
         0, 7, 2, 63, 2, 31, 31, 255, 255, 255,
      },
      {// 8
         0, 8, 2, 63, 2, 31, 31, 255, 255, 255,
      },
      {// 9
         0, 9, 2, 63, 2, 31, 31, 255, 255, 255,
      },
      {// 10
         0, 10, 2, 63, 2, 31, 31, 255, 255, 255,
      },
      {// 11
         0, 11, 2, 63, 2, 31, 31, 255, 255, 255,
      }}
};

DWORD SMT_RAM FrameNumber;

/*******************************************************************************
Name        : LLD_TNRInit
Description : Initialize TNR registers
Parameters  : physical channel
Assumptions :
Limitations :
Returns     : ST_ErrorCode_t type value :
                - ST_NO_ERROR, if no problem during init.
                - ST_ERROR_FEATURE_NOT_SUPPORTED, if input source not supported.
                - ST_ERROR_BAD_PARAMETER, if bad Init parameters.
*******************************************************************************/
void LLD_TNRInit(void)
{
#if 1
   // wait for implement.
#else
   gm_WriteRegWord(TNR3_CTRL,   1<<TNR_NOISE_GAIN_OFFSET |
                   3<<TNR3_IIR_CONVERGENCE_THRESH_UV_OFFSET |
                   3<<TNR3_IIR_CONVERGENCE_Y_OFFSET |
                   TNR3_FLESHTONE_EN_MASK);
#endif
   gm_WriteRegWord(TNR3_SNR_K_MATH, 0xfff);
   gm_WriteRegWord(TNR3_Y_NOISE, 1);
   gm_WriteRegWord(TNR3_UV_NOISE, 1);
   gm_WriteRegWord(TNR3_K_LOGIC, 0x3451);
   gm_WriteRegWord(TNR3_LA_LUT1, 0x107);
   gm_WriteRegWord(TNR3_LA_LUT2, 0x18a);
   gm_WriteRegWord(TNR3_FLESHTONE_YMIN, 0xd8);
   gm_WriteRegWord(TNR3_FLESHTONE_YMAX, 0x320);
   gm_WriteRegWord(TNR3_FLESHTONE_U, 0x403);
   gm_WriteRegWord(TNR3_FLESHTONE_V, 0x4c3);
   gm_WriteRegWord(TNR3_UV_DIF_MAX, 0x16);
   gm_WriteRegWord(MADI_CTRL, MADI_EXT_NOISE_TNR_EN); // MADI_EXT_NOISE_TNR_EN_MASK); /*that bit should be always set to 1 */
   gm_SyncUpdate(CH_A, INPUT_OUTPUT_CLOCK);

}

/*******************************************************************************
Description : Set noise level to be used for TNR based on external measurements
              (video decoder or any other block)
Parameters  : Channel - Input channel information (CH_A, CH_B, CH_C)
              Noise - noise level from external measurements
Returns     : ST_NO_ERROR / ST_ERROR_FEATURE_NOT_SUPPORTED
Example     : //set noise level to 0x34
              LLD_TNRExtNoiseSet(Channel, 0x34);
*******************************************************************************/
void LLD_TNRExtNoiseSet(BYTE Noise)
{

   gm_ClearAndSetBitsWord(MADI_EXT_NOISE, MADI_EXT_NOISE_TNR, Noise);

   gm_SyncUpdate(CH_A, INPUT_OUTPUT_CLOCK);

}

/*******************************************************************************
Name        : LLD_TNRGetHistogram
Description : Get TNR histogram data
Parameters  : physical channel, pointer to histogram buffer
Assumptions :
Limitations :
Returns     : ST_ErrorCode_t type value :
                - ST_NO_ERROR, if no problem during init.
                - ST_ERROR_FEATURE_NOT_SUPPORTED, if input source not supported.
                - ST_ERROR_BAD_PARAMETER, if bad Init parameters.
*******************************************************************************/
void LLD_TNRGetHistogram(void* Histogram_p)
{
   {
      WORD* H_p = Histogram_p;
      DWORD regAddr;
      for (regAddr = NOISE_HIST_BIN0_BIN1; regAddr <= NOISE_HIST_BINE_BINF; regAddr += sizeof(WORD))
         *H_p++ = gm_ReadRegWord(regAddr);
   }
}

/*******************************************************************************
Name        : LLD_TNRExtNoiseRead
Description : Get noise level to be used for TNR based on external measurements
Parameters  : Channel - Input channel information (CH_A, CH_B, CH_C)
Returns     : U8 - Noise value
Example     : LLD_TNRExtNoiseSet(Channel, &NoiseValue);
*******************************************************************************/
BYTE LLD_TNRExtNoiseRead(void)
{
   /* read Pending or Active value? */
   return (BYTE) ((gm_ReadRegWord(MADI_EXT_NOISE) & MADI_EXT_NOISE_TNR) >>
                  8);
}

/*******************************************************************************
Name        : LLD_MadiNoiseRead
Description : Read MADI calculated noise measurement.
              This values is taken after the application of MADI_NOISE_GAINs
Parameters  : Channel - Input channel information (CH_A, CH_B, CH_C)
Returns     : Noise value or error when returned value > 255
Examples    : //to read and print out noise value
                U32 noise = LLD_MadiNoiseRead(CH_A);
                if (noise < 255)
                    print("MADI noise: %d", noise);
*******************************************************************************/
BYTE LLD_MadiNoiseRead(void)
{
   return (gm_ReadRegByte(MADI_INT_NOISE) & MADI_INT_NOISE_METER);
}


/*******************************************************************************
Description : Get TNR noise measurements
Parameters  : HAL layer manager handle, TNR measurement structure
Assumptions :
Limitations :
Returns     : ST_ErrorCode_t type value :
                - ST_NO_ERROR, if no problem during init.
                - ST_ERROR_FEATURE_NOT_SUPPORTED, if input source not supported.
                - ST_ERROR_BAD_PARAMETER, if bad Init parameters.
*******************************************************************************/
void VCD_GetTNRDynData(TnrDynDat_t* TnrDynData_p)
{
   if (TnrDynData_p)
   {
      TnrDynData_p->InternalNoiseMeter = LLD_MadiNoiseRead();
      TnrDynData_p->ExternalNoiseMeter = LLD_TNRExtNoiseRead();
      LLD_TNRGetHistogram(TnrDynData_p->Histogram);
   }
}



#pragma argsused
BYTE TNR_MsgHandler(MsgPacket_t *message, BYTE comm_use)
{
   BYTE* Bp_msg = (BYTE *)(message->msgpkt);

   //get pointer to output message length setting
   BYTE* Bp_len=GetOutMsgLenPtr();
   BYTE* Bp_out= GetOutMsgBufPtr();

   BYTE B_TransId;
   BYTE B_ContextStatus;
   BYTE far * Bp_Context = NULL_PTR;
   BYTE B_ContextCnt = 0;
   WORD W_CurrentCmd = 0xFFFF;

   B_TransId = message->control;

   // get buffer pointer from SMT driver
   B_ContextStatus = SMT_GetBuffer( B_TransId, (BYTE far * far *)&Bp_Context, 145 );

   // If B_ContextStatus is FIRST_REQUEST clear Bp_Context buffer
   if ( B_ContextStatus == FIRST_REQUEST )
   {
	   if(UserPrefTNR == TNR_LOW)
      {
         TNRDefaultVQTable = TNRParams_Low;
      }      
      else if(UserPrefTNR == TNR_MEDIUM)
      {
         TNRDefaultVQTable = TNRParams_Medium;
      }
      else if(UserPrefTNR == TNR_HIGH)
      {
         TNRDefaultVQTable = TNRParams_High;
      }
      else
      {
         TNRDefaultVQTable = TNRParams_Default;
      }
      // Clear Buffer
      for ( B_ContextCnt = 0; B_ContextCnt < 145; B_ContextCnt++ )
         *(Bp_Context + B_ContextCnt) = 0;

      W_CurrentCmd = (Bp_msg[1] << 8) + Bp_msg[0];
      *(Bp_Context + 0) = Bp_msg[0];
      *(Bp_Context + 1) = Bp_msg[1];

      // Save command length
      *(Bp_Context + 2) = message->len - SMT_HEADER_SIZE - CHECKSUM_SIZE;

      for (B_ContextCnt = 0; B_ContextCnt < *(Bp_Context + 2); B_ContextCnt++)
      {
         *(Bp_Context + 3 + B_ContextCnt) = Bp_msg[B_ContextCnt];
      }
   }
   else if ( B_ContextStatus == NEXT_REQUEST )
   {
      W_CurrentCmd = ((*(Bp_Context + 1))<<8) + (*(Bp_Context + 0));

      // Save data into buffer
      for (B_ContextCnt = 0; B_ContextCnt < (message->len - SMT_HEADER_SIZE - CHECKSUM_SIZE); B_ContextCnt++)
      {
         *(Bp_Context + 3 + *(Bp_Context + 2) + B_ContextCnt) = Bp_msg[B_ContextCnt];
      }

      // Update command length
      *(Bp_Context + 2) += message->len - SMT_HEADER_SIZE - CHECKSUM_SIZE;

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
      gm_Printf("NO_MEMORYNO_MEMORY", 0);
      return PORT_MSG_HNDLR_SELFCARED;//ERR_PARAM;
   }

   if (W_CurrentCmd == TS_TNR_START)
   {
      Bp_out[0] = TS_OK;
      Bp_out[1] = TS_TNR_VER;
      *Bp_len = 2;
      dbgmsg("TS_TNR_START %s\n","OK");
      gm_Print("TS_TNR_START", 0);
   }
   else if (W_CurrentCmd == TS_TNR_STOP)
   {
      Bp_out[0] = TS_OK;
      *Bp_len = 1;
      gm_Print("TS_TNR_STOP", 0);
   }
   else if (W_CurrentCmd == TS_TNR_READ)
   {
      Bp_out[0] = TS_OK;

      if ( B_ContextStatus == FIRST_REQUEST )
      {
         BYTE B_i;

         //Bp_out[2] = message->msgType | MORE_DATA_BIT;
         MoreData(1);

         gm_Printf("More Data \n", 0);

         Bp_out[1] = 142;
         Bp_out[2] = 0;
         Bp_out[3] = 0;
         Bp_out[4] = 0;

         Bp_out[5] = 25;
         Bp_out[6] = 0;
         Bp_out[7] = 0;
         Bp_out[8] = 0;

         Bp_out[9] = 0;
         Bp_out[10] = 0;
         Bp_out[11] = 0;
         Bp_out[12] = 0;

         Bp_out[13] = TNRDefaultVQTable.EnableGlobalNoiseAdaptive;
         Bp_out[14] = TNRDefaultVQTable.EnableGlobalMotionAdaptive;

         Bp_out[15] = (BYTE)TNRDefaultVQTable.GlobalMotionStdTh;
         Bp_out[16] = (BYTE)(TNRDefaultVQTable.GlobalMotionStdTh >> 8);

         Bp_out[17] = (BYTE)TNRDefaultVQTable.GlobalMotionHighTh;
         Bp_out[18] = (BYTE)(TNRDefaultVQTable.GlobalMotionHighTh >> 8);

         Bp_out[19] = TNRDefaultVQTable.GlobalMotionPanTh;

         Bp_out[20] = TNRDefaultVQTable.GlobalMotionStd;
         Bp_out[21] = TNRDefaultVQTable.GlobalMotionHigh;
         Bp_out[22] = TNRDefaultVQTable.GlobalMotionPan;

         *Bp_len = 23;
      }
      else
      {
         BYTE B_i;
         MoreData(0);

         for (B_i = 0; B_i < 12; )
         {
            Bp_out[10*B_i+0] = TNRDefaultVQTable.AdaptiveData[B_i].EnableSNR;
            Bp_out[10*B_i+1] = TNRDefaultVQTable.AdaptiveData[B_i].SNRLumaSensitivity;
            Bp_out[10*B_i+2] = TNRDefaultVQTable.AdaptiveData[B_i].SNRLumaStrength;
            Bp_out[10*B_i+3] = TNRDefaultVQTable.AdaptiveData[B_i].SNRChromaSensitivity;
            Bp_out[10*B_i+4] = TNRDefaultVQTable.AdaptiveData[B_i].SNRChromaStrength;
            Bp_out[10*B_i+5] = TNRDefaultVQTable.AdaptiveData[B_i].YKMin;
            Bp_out[10*B_i+6] = TNRDefaultVQTable.AdaptiveData[B_i].UVKMin;
            Bp_out[10*B_i+7] = TNRDefaultVQTable.AdaptiveData[B_i].YGrad;
            Bp_out[10*B_i+8] = TNRDefaultVQTable.AdaptiveData[B_i].FleshtoneGrad;
            Bp_out[10*B_i+9] = TNRDefaultVQTable.AdaptiveData[B_i].ChromaGrad;

            B_i++;
         }

         *Bp_len = 120;
         gm_Printf("No More Data \n", 0);
      }


      // send replay

      //Bp_msg[3] = B_TransId;
      //Bp_out[4] = TS_OK;
      //gm_WriteCmdPkt((BYTE far*)B_Reply, comm_use);


      dbgmsg("TS_TNR_READ \n", 0);
   }
   else if (W_CurrentCmd == TS_TNR_WRITE)
   {
      // if( B_ContextStatus == FIRST_REQUEST )
      if (MoreDataExpected())
      { // Save data into buffer do nothing ...
         gm_Printf("FIRST_REQUEST ... ", 0);
      }
      else
      { // Update all data
         BYTE B_i;

         TNRDefaultVQTable.EnableGlobalNoiseAdaptive = *(Bp_Context + 17);

         TNRDefaultVQTable.EnableGlobalMotionAdaptive = *(Bp_Context + 18);

         TNRDefaultVQTable.GlobalMotionStdTh = (((WORD) *(Bp_Context + 20)) << 8) | *(Bp_Context + 19);

         TNRDefaultVQTable.GlobalMotionHighTh = (((WORD) *(Bp_Context + 22)) << 8) | *(Bp_Context + 21);

         TNRDefaultVQTable.GlobalMotionPanTh = *(Bp_Context + 23);

         TNRDefaultVQTable.GlobalMotionStd = *(Bp_Context + 24);

         TNRDefaultVQTable.GlobalMotionHigh  = *(Bp_Context + 25);

         TNRDefaultVQTable.GlobalMotionPan  = *(Bp_Context + 26);

         for (B_i = 0;B_i < 12 ; B_i ++)
         {
            TNRDefaultVQTable.AdaptiveData[B_i].EnableSNR = *(Bp_Context + 27 + B_i*10);

            TNRDefaultVQTable.AdaptiveData[B_i].SNRLumaSensitivity = *(Bp_Context + 28 + B_i*10);

            TNRDefaultVQTable.AdaptiveData[B_i].SNRLumaStrength = *(Bp_Context + 29 + B_i*10);

            TNRDefaultVQTable.AdaptiveData[B_i].SNRChromaSensitivity = *(Bp_Context + 30 + B_i*10);

            TNRDefaultVQTable.AdaptiveData[B_i].SNRChromaStrength = *(Bp_Context + 31 + B_i*10);

            TNRDefaultVQTable.AdaptiveData[B_i].YKMin = *(Bp_Context + 32 + B_i*10);

            TNRDefaultVQTable.AdaptiveData[B_i].UVKMin = *(Bp_Context + 33 + B_i*10);

            TNRDefaultVQTable.AdaptiveData[B_i].YGrad = *(Bp_Context + 34 + B_i*10);

            TNRDefaultVQTable.AdaptiveData[B_i].FleshtoneGrad = *(Bp_Context + 35 + B_i*10);

            TNRDefaultVQTable.AdaptiveData[B_i].ChromaGrad = *(Bp_Context + 36 + B_i*10);
         }

         LLD_TNRSetAdaptivePar((VQ_TNR_Adaptive_Parameters_t far *)&TNRDefaultVQTable.AdaptiveData[0]); // Write TNR parameter into register
         
         gm_Printf("NEXT_REQUEST ... ", 0);
      }

      Bp_out[0] = TS_OK;
      *Bp_len = 1;
      gm_Print("TS_TNR_WRITE", 0);
   }
   else if (W_CurrentCmd == TS_TNR_EN)
   {
      FrameNumber = 0;

      gm_ClearRegBitsWord(TNR3_CTRL, TNR3_BYPASS);

      gm_SetRegBitsWord(MOTION_PROC_CTRL, TNR3_EN);

      gm_SyncUpdate(CH_A, INPUT_OUTPUT_CLOCK);

      dbgmsg("TS_TNR_EN \n", 0);

      Bp_out[0] = TS_OK;
      *Bp_len = 1;
   }
   else if (W_CurrentCmd == TS_TNR_DISABLE)
   {
      gm_SetRegBitsWord(TNR3_CTRL, TNR3_BYPASS);

      gm_ClearRegBitsWord(MOTION_PROC_CTRL, TNR3_EN);

      gm_SyncUpdate(CH_A, INPUT_OUTPUT_CLOCK);

      dbgmsg("TS_TNR_DISABLE \n", 0);

      *Bp_len = 1;
      Bp_out[0] = TS_OK;
   }
   else if (W_CurrentCmd == TS_TNR_READ_DYNAMIC)
   {
      TsTnrDynamic_t * TsTnrDynData = (TsTnrDynamic_t*)(Bp_out+1);
      TnrDynDat_t TnrDyn;

      VCD_GetTNRDynData(&TnrDyn);
		_fmemcpy(TsTnrDynData->TNR_FrameData,&TnrDyn,sizeof(TnrDynDat_t));
      TsTnrDynData->Len = 1;

      FrameNumber += TsTnrDynData->Len;
      TsTnrDynData->CurrentFrame = FrameNumber;

      *Bp_len = 25;

      Bp_out[0] = TS_OK;

      dbgmsg("TS_TNR_READ_DYNAMIC \n", 0);
   }
   else
   {
      dbgmsg("STTS_Handler_TNR: UNKNOWN COMMAND \n", 0);
      Bp_out[0] = TS_NOT_SUPPORTED;
      *Bp_len = 1;
   }

   return (BYTE)PORT_MSG_HNDLR_OK;
}

#endif // (FEATURE_TNR == REV3)

#endif // USE_SMT
