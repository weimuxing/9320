#include <mem.h>
#include "System\All.h"
#include "CommLink\smt.h"
#include "CommLink\SMT_CSC.h"

#ifdef USE_SMT


#define TS_CSC_VER 0

#define TS_CSC_DEBUG_MSG
#if defined(TS_CSC_DEBUG_MSG)
#define dbgmsg(a,b) gm_Printf(a,b)
#else
#define dbgmsg(a,b)
#endif

typedef enum
{
   TS_CSC_START,
   TS_CSC_STOP,
   TS_CSC_READ,
   TS_CSC_WRITE,
   TS_CSC_EN,
   TS_CSC_DISABLE
}TsCscCmd_t;

BYTE B_ColorSpaceConversion = 0;
SDWORD SMT_RAM CSCMatrix[9];
SDWORD SMT_RAM InOffset[3];
SDWORD SMT_RAM OutOffset[3];


#pragma argsused
BYTE CSC_MsgHandler(MsgPacket_t *message, BYTE comm_use)
{
   BYTE* Bp_msg = (BYTE *)(message->msgpkt);

   //get pointer to output message length setting
   BYTE* Bp_len=GetOutMsgLenPtr();
   BYTE* Bp_out= GetOutMsgBufPtr();

   if (Bp_msg[0] == TS_CSC_START)
   {
      Bp_out[0] = TS_OK;
      Bp_out[1] = TS_CSC_VER;
      *Bp_len = 2;

      dbgmsg("TS_CSC_START: OK (%d)\n",0);
   }
   else if (Bp_msg[0] == TS_CSC_STOP)
   {
      Bp_out[0] = TS_OK;
      *Bp_len = 1;
      dbgmsg("TS_CSC_STOP: OK (%d)\n",0);
   }
   else if (Bp_msg[0] == TS_CSC_READ)
   {
      Bp_out[0] = TS_OK;

      Bp_out[1] = 73;
      Bp_out[2] = 0;
      Bp_out[3] = 0;
      Bp_out[4] = 0;

      Bp_out[5] = 18;
      Bp_out[6] = 0;
      Bp_out[7] = 0;
      Bp_out[8] = 0;

      Bp_out[9] = 1;
      Bp_out[10] = 0;
      Bp_out[11] = 0;
      Bp_out[12] = 0;

      Bp_out[13] = B_ColorSpaceConversion;

      { // 3x3
         BYTE i;
         WORD W_BaseAddr = MAIN_P_MULT_COEF11;

         for (i = 0; i < 9; i++)
         {
            CSCMatrix[i] = ((SDWORD)gm_ReadRegWord(W_BaseAddr + i*4) >> 1) << 4;

            // check if the value is negative
            if (CSCMatrix[i]&BIT14)
               CSCMatrix[i] = CSCMatrix[i]|BIT15|0xFFFF0000;

            Bp_out[14+i*4] = (BYTE)CSCMatrix[i];
            Bp_out[15+i*4] = (BYTE)(CSCMatrix[i]>>8);
            Bp_out[16+i*4] = (BYTE)(CSCMatrix[i]>>16);
            Bp_out[17+i*4] = (BYTE)(CSCMatrix[i]>>24);
         }
      }

      { // In Offset
         BYTE i;
         WORD W_BaseAddr = MAIN_P_IN_OFFSET1;

         for (i = 0; i < 3; i++)
         {
            InOffset[i] = (SDWORD)gm_ReadRegWord(W_BaseAddr + i*4);

            // check if the value is negative
            if (InOffset[i]&BIT14)
               InOffset[i] = InOffset[i]|BIT15|0xFFFF0000;


            Bp_out[50+i*4] = (BYTE)InOffset[i];
            Bp_out[51+i*4] = (BYTE)(InOffset[i]>>8);
            Bp_out[52+i*4] = (BYTE)(InOffset[i]>>16);
            Bp_out[53+i*4] = (BYTE)(InOffset[i]>>24);

         }
      }

      { // Out Offset
         BYTE i;
         WORD W_BaseAddr = MAIN_P_OUT_OFFSET1;

         for (i = 0; i < 3; i++)
         {
            OutOffset[i] = (SDWORD)gm_ReadRegWord(W_BaseAddr + i*4);

            // check if the value is negative
            if (OutOffset[i]&BIT14)
               OutOffset[i] = OutOffset[i]|BIT15|0xFFFF0000;


            Bp_out[62+i*4] = (BYTE)OutOffset[i];
            Bp_out[63+i*4] = (BYTE)(OutOffset[i]>>8);
            Bp_out[64+i*4] = (BYTE)(OutOffset[i]>>16);
            Bp_out[65+i*4] = (BYTE)(OutOffset[i]>>24);

         }
      }

      *Bp_len = 74;
      dbgmsg("TS_CSC_READ \n", 0);
   }
   else if (Bp_msg[0] == TS_CSC_WRITE)
   {
      // GamutDefaultRGBToYUV601VQTable

      // ColorSpaceConversion
      B_ColorSpaceConversion = Bp_msg[14];

      // MultCoef11 // Signed value 1.3.12
      {
         BYTE i;
         WORD W_BaseAddr = MAIN_P_MULT_COEF11;

         for (i = 0; i < 9; i++)
         {
            CSCMatrix[i] = ((SDWORD)Bp_msg[18+i*4] << 24) | (Bp_msg[17+i*4] << 16) | (Bp_msg[16+i*4] << 8) | Bp_msg[15+i*4];
            gm_Printf("CSCMatrix[%d] = 0x%X", i, CSCMatrix[i]>>3);
            gm_WriteRegWord(W_BaseAddr+i*4, CSCMatrix[i]>>3);
         }
      }

      // InOffset Signed value 1.14.0
      {
         BYTE i;
         WORD W_BaseAddr = MAIN_P_IN_OFFSET1;

         for (i = 0; i < 3; i++)
         {
            InOffset[i] = ((SDWORD)Bp_msg[54+i*4] << 24) | (Bp_msg[53+i*4] << 16) | (Bp_msg[52+i*4] << 8) | Bp_msg[51+i*4];
            gm_Printf("InOffset[%d] = 0x%X", i, InOffset[i]&0x7FFF);
            gm_WriteRegWord(W_BaseAddr+i*4, InOffset[i]&0x7FFF);
         }
      }

      // OutOffset Signed value 1.14.0
      {
         BYTE i;
         WORD W_BaseAddr = MAIN_P_OUT_OFFSET1;

         for (i = 0; i < 3; i++)
         {
            OutOffset[i] = ((SDWORD)Bp_msg[66+i*4] << 24) | (Bp_msg[65+i*4] << 16) | (Bp_msg[64+i*4] << 8) | Bp_msg[63+i*4];
            gm_Printf("InOffset[%d] = 0x%X", i, OutOffset[i]&0x7FFF);
            gm_WriteRegWord(W_BaseAddr+i*4, OutOffset[i]&0x7FFF);
         }
      }

      Bp_out[0] = TS_OK;
      *Bp_len = 1;
      dbgmsg("TS_CSC_WRITE \n", 0);
   }
   else if (Bp_msg[0] == TS_CSC_EN)
   {
      // COLOR_MATRIX_CTRL:MAIN_MATRIX_EN
      // gm_SetRegBitsWord(COLOR_MATRIX_CTRL, MAIN_MATRIX_EN);
      Bp_out[0] = TS_OK;
      *Bp_len = 1;
      dbgmsg("TS_CSC_EN \n", 0);
   }
   else if (Bp_msg[0] == TS_CSC_DISABLE)
   {
      //gm_ClearRegBitsWord(COLOR_MATRIX_CTRL, MAIN_MATRIX_EN);
      Bp_out[0] = TS_OK;
      *Bp_len = 1;
      dbgmsg("TS_CSC_DISABLE \n", 0);
   }
   else
   {
      dbgmsg("STTS_Handler_CSC: UNKNOWN COMMAND \n", 0);
      Bp_out[0] = TS_NOT_SUPPORTED;
      *Bp_len = 1;
   }

   return (BYTE)PORT_MSG_HNDLR_OK;
}

#endif
