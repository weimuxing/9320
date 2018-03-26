/*
   $Workfile:   smt_acm3.c  $
   $Revision: 1.8 $
   $Date: 2012/04/13 08:36:35 $
*/

//******************************************************************
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
//================================================================
//
// MODULE:      smt_acm3.c
//
//******************************************************************

#include "CommLink\smt.h"
#include "CommLink\smt_acm3.h"
#include "StdLibInc\gm_ACM3D.h"
#include "StdLibInc\gm_Register.h"
#include "StdLibInc\gm_ChipFeature.h"

//#define _DEBUG_MSG
#ifdef _DEBUG_MSG
#define msg(x,y)      gm_Print(x,y)
#else
#define msg(x,y)
#endif

#ifdef USE_SMT
#if (FEATURE_ACM_3D == ENABLE)

#if 1
#define TS_ACM_VER 2

typedef enum
{
   TS_ACM_START,
   TS_ACM_STOP,
   TS_ACM_READ,
   TS_ACM_WRITE,
   TS_ACM_EN,
   TS_ACM_DISABLE
}TsACMCmd_t;

//******************************************************************************
//
// FUNCTION     :   BYTE ACM3_SMT_MsgHandler(MsgPacket_t *message, BYTE comm_use)
// USAGE        :   This function parse the SMT messages for ACM3 feature
//
// INPUT        :   MsgPacket_t *message = SMT message
//                  BYTE comm_use  = SERIAL_USE or DDC2BI_USE
// OUTPUT       :   OK = if no error, ERR_SEND = if error is found
//
//******************************************************************************
BYTE ACM3_SMT_MsgHandler(MsgPacket_t *message, BYTE comm_use)
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
   B_ContextStatus = SMT_GetBuffer( B_TransId, (BYTE far * far *)&Bp_Context, ACM3_FULL_SIZE);

   // If B_ContextStatus is FIRST_REQUEST clear Bp_Context buffer
   if ( B_ContextStatus == FIRST_REQUEST )
   {
      // Clear Buffer
      for ( W_ContextCnt = 0; W_ContextCnt < ACM3_FULL_SIZE; W_ContextCnt++ )
         *(Bp_Context + W_ContextCnt) = 0;

      W_CurrentCmd = (Bp_msg[1] << 8) + Bp_msg[0];

      // Save command length
      *(Bp_Context + 0) = message->len - SMT_HEADER_SIZE - CHECKSUM_SIZE;
      //*(Bp_Context + 1) = 0;

      // Region
      //*(Bp_Context + 2) = 0;

      for (W_ContextCnt = 0; W_ContextCnt < *(Bp_Context + 0); W_ContextCnt++)
      {
         *(Bp_Context + 3 + W_ContextCnt) = Bp_msg[W_ContextCnt];
      }
   }
   else if ( B_ContextStatus == NEXT_REQUEST )
   {
      WORD W_Temp;

      W_CurrentCmd = ((*(Bp_Context + 4))<<8) + (*(Bp_Context + 3));

      // Previou data count
      W_Temp = (*(Bp_Context + 1) << 8) + *(Bp_Context + 0);

      // Save data into buffer
      for (W_ContextCnt = 0; W_ContextCnt < (message->len - SMT_HEADER_SIZE - CHECKSUM_SIZE); W_ContextCnt++)
      {
         *(Bp_Context + 3 + W_Temp + W_ContextCnt) = Bp_msg[W_ContextCnt];
      }

      // Update command length
      // W_Temp = (*(Bp_Context + 1) << 8) + *(Bp_Context + 0) + message->len - SMT_HEADER_SIZE - CHECKSUM_SIZE;
      W_Temp += message->len - SMT_HEADER_SIZE - CHECKSUM_SIZE;
      *(Bp_Context + 0) = (BYTE)(W_Temp&0xFF);
      *(Bp_Context + 1) = (BYTE)((W_Temp >> 8)&0xFF);
      //gm_Printf("data size = %d", (((WORD)*(Bp_Context + 1))<<8)|(*(Bp_Context + 0)));
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

   if (W_CurrentCmd == TS_ACM_START)
   {
      Bp_out[0] = TS_OK;
      Bp_out[1] = TS_ACM_VER;
      *Bp_len = 2;
      msg("TS_ACM3D_START \n", 0);
   }
   else if (W_CurrentCmd == TS_ACM_STOP)
   {
      Bp_out[0] = TS_OK;
      *Bp_len = 1;
      msg("TS_ACM3D_STOP", 0);
   }
   else if (W_CurrentCmd == TS_ACM_READ)
   {
      BYTE B_Region = *(Bp_Context + 2)%ACM3_REGION_NUM;

      VIP_ACMRegionDefinitions_t gma_RegionDefs[ACM3D_REGION_NUM] =
      {
         //	 W_HueCenter, W_HueAperture,   B_R1, B_R2, B_Y1, B_Y2, W_HueFade, B_SatFade, B_LumiFade
         {0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0},
      };
      VIP_ACMRegionCorrectionParameters_t gma_CorrectionParamDefs[ACM3D_REGION_NUM] =
      {
         //	  SW_HueOffs, SW_HueGain, SW_SatOffs, W_SatGain, SW_LumiOffs, W_LumiGain, SW_U_Vect, SW_V_Vect, W_Alpha
         { 0, 0, 0, 0, 0, 0, 0, 0, 0},
         { 0, 0, 0, 0, 0, 0, 0, 0, 0},
         { 0, 0, 0, 0, 0, 0, 0, 0, 0},
         { 0, 0, 0, 0, 0, 0, 0, 0, 0},
         { 0, 0, 0, 0, 0, 0, 0, 0, 0},
         { 0, 0, 0, 0, 0, 0, 0, 0, 0},
         { 0, 0, 0, 0, 0, 0, 0, 0, 0},
         { 0, 0, 0, 0, 0, 0, 0, 0, 0},
      };

      // Set pointers to temporary arrays
      gm_ACM3DSetRegionDefinitionTablePtr((VIP_ACMRegionDefinitions_t far *)&gma_RegionDefs);
      gm_ACM3DSetCorrectionParamsTablePtr((VIP_ACMRegionCorrectionParameters_t far *)&gma_CorrectionParamDefs);

      // read ACM3D data to these arrays (all 8 regiones)
      gm_ACM3DReadAll();

      if ( B_ContextStatus == FIRST_REQUEST )
      {
         Bp_out[0] = TS_OK;

         // Size
         Bp_out[1] = 48;
         Bp_out[2] = 1;
         Bp_out[3] = 0;
         Bp_out[4] = 0;

         // Signature
         Bp_out[5] = 15;
         Bp_out[6] = 0;
         Bp_out[7] = 0;
         Bp_out[8] = 0;

         // Version
         Bp_out[9] = 2;
         Bp_out[10] = 0;
         Bp_out[11] = 0;
         Bp_out[12] = 0;

         {
            WORD W_TempRegVal;
            W_TempRegVal = gm_ReadRegWord(ACM3_CTRL);

            // ACM3_THFADE_LORGAIN_EN
            Bp_out[13] = (W_TempRegVal & ACM3_THFADE_LORGAIN_) >> 2;

            // ACM3_THFADE_LOR_VAL
            Bp_out[14] = (W_TempRegVal & ACM3_THFADE_LOR_VAL) >> 4;

            // ACM3_THFADE_R_CUTOFF
            Bp_out[15] = (W_TempRegVal & ACM3_THFADE_R_CUTOFF) >> 8;
         }

         // RegionsNum
         Bp_out[16] = ACM3_REGION_NUM;

         *Bp_len = 1 + 16;

         // More Data ...
         MoreData(1);

         msg("STTS_Handler_ACM3Dv2: FIRST_REQUEST \n", 0);
      }
      else if (*(Bp_Context + 2) < ACM3_REGION_NUM)
      {
         Bp_out[0] = (BYTE)(0xFF & gma_RegionDefs[B_Region].W_HueCenter);
         Bp_out[1] = (BYTE)(0xFF & (gma_RegionDefs[B_Region].W_HueCenter>>8));
         Bp_out[2] = (BYTE)(0xFF & gma_RegionDefs[B_Region].W_HueAperture);
         Bp_out[3] = 0;

         Bp_out[4] = (BYTE)(0xFF & gma_RegionDefs[B_Region].W_R1);
         Bp_out[5] = (BYTE)(gma_RegionDefs[B_Region].W_R1 >> 8);

         Bp_out[6] = (BYTE)(0xFF & gma_RegionDefs[B_Region].W_R2);
         Bp_out[7] = (BYTE)(gma_RegionDefs[B_Region].W_R2 >> 8);

         Bp_out[8] = (BYTE)(0xFF & gma_RegionDefs[B_Region].W_Y1);
         Bp_out[9] = (BYTE)(gma_RegionDefs[B_Region].W_Y1 >> 8);

         Bp_out[10] = (BYTE)(0xFF & gma_RegionDefs[B_Region].W_Y2);
         Bp_out[11] = (BYTE)(gma_RegionDefs[B_Region].W_Y2 >> 8);

         Bp_out[12] = (BYTE)(0xFF & gma_RegionDefs[B_Region].W_HueFade);
         Bp_out[13] = 0;

         Bp_out[14] = (BYTE)(0xFF & gma_RegionDefs[B_Region].W_SatFade);
         Bp_out[15] = (BYTE)(gma_RegionDefs[B_Region].W_SatFade >> 8);

         Bp_out[16] = (BYTE)(0xFF & gma_RegionDefs[B_Region].W_LumiFade);
         Bp_out[17] = (BYTE)(gma_RegionDefs[B_Region].W_LumiFade >> 8);

         // Save region number ...
         *(Bp_Context + 2) = *(Bp_Context + 2) + 1;

         *Bp_len = 18;

         MoreData(1);

         msg("STTS_Handler_ACM3Dv2: NEXT_REQUEST RegionDefs \n", 0);
      }
      else
      {
         Bp_out[0] = (BYTE)(0xFF & gma_CorrectionParamDefs[B_Region].SW_HueOffs);
         Bp_out[1] = ((gma_CorrectionParamDefs[B_Region].SW_HueOffs & BIT7)==BIT7)?0xFF:0x00;

         Bp_out[2] = (BYTE)(0xFF & gma_CorrectionParamDefs[B_Region].SW_HueGain);
         Bp_out[3] = ((gma_CorrectionParamDefs[B_Region].SW_HueGain & BIT3)==BIT3)?0xFF:0x00;

         Bp_out[4] = (BYTE)(0xFF & gma_CorrectionParamDefs[B_Region].SW_SatOffs);
         Bp_out[5] = ((gma_CorrectionParamDefs[B_Region].SW_SatOffs & BIT11)==BIT11)?
                     (0xFF&(gma_CorrectionParamDefs[B_Region].SW_SatOffs >> 8)):(gma_CorrectionParamDefs[B_Region].SW_SatOffs >> 8);

         Bp_out[6] = (BYTE)(0xFF & gma_CorrectionParamDefs[B_Region].W_SatGain);
         Bp_out[7] = 0x00;

         Bp_out[8] = (BYTE)(0xFF & gma_CorrectionParamDefs[B_Region].SW_LumiOffs);
         Bp_out[9] = ((gma_CorrectionParamDefs[B_Region].SW_LumiOffs & BIT11)==BIT11)?
                     (0xFF&(gma_CorrectionParamDefs[B_Region].SW_LumiOffs >> 8)):(gma_CorrectionParamDefs[B_Region].SW_LumiOffs >> 8);

         Bp_out[10] = (BYTE)(0xFF & gma_CorrectionParamDefs[B_Region].W_LumiGain);
         Bp_out[11] = 0x00;

         Bp_out[12] = (BYTE)(0xFF & gma_CorrectionParamDefs[B_Region].SW_U_Vect);
         Bp_out[13] = ((gma_CorrectionParamDefs[B_Region].SW_U_Vect & BIT11)==BIT11)?
                      (0xFF&(gma_CorrectionParamDefs[B_Region].SW_U_Vect >> 8)):(gma_CorrectionParamDefs[B_Region].SW_U_Vect >> 8);

         Bp_out[14] = (BYTE)(0xFF & gma_CorrectionParamDefs[B_Region].SW_V_Vect);
         Bp_out[15] = ((gma_CorrectionParamDefs[B_Region].SW_V_Vect & BIT11)==BIT11)?
                      (0xFF&(gma_CorrectionParamDefs[B_Region].SW_V_Vect >> 8)):(gma_CorrectionParamDefs[B_Region].SW_V_Vect >> 8);

         Bp_out[16] = (BYTE)(0xFF & gma_CorrectionParamDefs[B_Region].W_Alpha);
         Bp_out[17] = 0;

         *Bp_len = 18;

         // Save region number ...
         *(Bp_Context + 2) = *(Bp_Context + 2) + 1;

         if ((*(Bp_Context + 2)%ACM3_REGION_NUM) == 0 )
            MoreData(0); // No More Data ...
         else
            MoreData(1);


         msg("STTS_Handler_ACM3Dv2: NEXT_REQUEST CorrectionParamDefs \n", 0);
      }
      msg("STTS_Handler_ACM3Dv2: TS_ACM_READ \n", 0);

   }
   else if (W_CurrentCmd == TS_ACM_WRITE)
   {
      if (!MoreDataExpected())
      {
         VIP_ACMRegionDefinitions_t gma_RegionDefs[ACM3D_REGION_NUM] =
         {
            //	 W_HueCenter, W_HueAperture,   B_R1, B_R2, B_Y1, B_Y2, W_HueFade, B_SatFade, B_LumiFade
            {0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0},
         };
         VIP_ACMRegionCorrectionParameters_t gma_CorrectionParamDefs[ACM3D_REGION_NUM] =
         {
            //	  SW_HueOffs, SW_HueGain, SW_SatOffs, W_SatGain, SW_LumiOffs, W_LumiGain, SW_U_Vect, SW_V_Vect, W_Alpha
            { 0, 0, 0, 0, 0, 0, 0, 0, 0},
            { 0, 0, 0, 0, 0, 0, 0, 0, 0},
            { 0, 0, 0, 0, 0, 0, 0, 0, 0},
            { 0, 0, 0, 0, 0, 0, 0, 0, 0},
            { 0, 0, 0, 0, 0, 0, 0, 0, 0},
            { 0, 0, 0, 0, 0, 0, 0, 0, 0},
            { 0, 0, 0, 0, 0, 0, 0, 0, 0},
            { 0, 0, 0, 0, 0, 0, 0, 0, 0},
         };
         BYTE B_Region;
         WORD W_temp_offset;

         // ACM3_THFADE_LORGAIN_EN
         if (*(Bp_Context + 15))
            gm_SetRegBitsWord(ACM3_CTRL, ACM3_THFADE_LORGAIN_);
         else
            gm_ClearRegBitsWord(ACM3_CTRL, ACM3_THFADE_LORGAIN_);

         // ACM3_THFADE_LOR_VAL
         gm_ClearAndSetBitsWord(ACM3_CTRL, ACM3_THFADE_LOR_VAL, (*(Bp_Context + 16) << 4));

         // ACM3_THFADE_R_CUTOFF
         gm_ClearAndSetBitsWord(ACM3_CTRL, ACM3_THFADE_R_CUTOFF, ((WORD)*(Bp_Context + 17) << 8));

         // RegionsNum
         // gm_Printf("Region %d", *(Bp_Context + 20));

         // ACMRegionDefs[8]

         // ACMRegionCorrectionParams[8]

         // read all data, update recieved region data and write them back
         // Set pointers to temporary arrays
         gm_ACM3DSetRegionDefinitionTablePtr((VIP_ACMRegionDefinitions_t far *)&gma_RegionDefs);
         gm_ACM3DSetCorrectionParamsTablePtr((VIP_ACMRegionCorrectionParameters_t far *)&gma_CorrectionParamDefs);
         gm_ACM3DReadAll(); // read ACM3D data to these arrays (all 8 regiones)

         W_temp_offset = 21;

         // copy values one-by-one
         //for(B_Region = 0; B_Region < *(Bp_Context + 20); B_Region++)
         for (B_Region = 0; B_Region < ACM3_REGION_NUM; B_Region++)
         {
            gma_RegionDefs[B_Region].W_HueCenter = (Bp_Context[W_temp_offset + 1 + B_Region * 18]<<8) + Bp_Context[W_temp_offset + B_Region * 18];

            gma_RegionDefs[B_Region].W_HueAperture = Bp_Context[W_temp_offset + 2 + B_Region * 18];

            gma_RegionDefs[B_Region].W_R1 = (Bp_Context[W_temp_offset + 5 + B_Region * 18]<<8) + Bp_Context[W_temp_offset + 4 + B_Region * 18];

            gma_RegionDefs[B_Region].W_R2 = (Bp_Context[W_temp_offset + 7 + B_Region * 18]<<8) + Bp_Context[W_temp_offset + 6 + B_Region * 18];

            gma_RegionDefs[B_Region].W_Y1 = (Bp_Context[W_temp_offset + 9 + B_Region * 18]<<8) + Bp_Context[W_temp_offset + 8 + B_Region * 18];

            gma_RegionDefs[B_Region].W_Y2 = (Bp_Context[W_temp_offset + 11 + B_Region * 18]<<8) + Bp_Context[W_temp_offset + 10 + B_Region * 18];

            gma_RegionDefs[B_Region].W_HueFade = Bp_Context[W_temp_offset + 12 + B_Region * 18];

            gma_RegionDefs[B_Region].W_SatFade = (Bp_Context[W_temp_offset + 15 + B_Region * 18]<<8) + Bp_Context[W_temp_offset + 14 + B_Region * 18];

            gma_RegionDefs[B_Region].W_LumiFade = (Bp_Context[W_temp_offset + 17 + B_Region * 18]<<8) + Bp_Context[W_temp_offset + 16 + B_Region * 18];
         }

         W_temp_offset += B_Region * 18;

         //for(B_Region = 0; B_Region < *(Bp_Context + 20); B_Region++)
         for (B_Region = 0; B_Region < ACM3_REGION_NUM; B_Region++)
         {
            gma_CorrectionParamDefs[B_Region].SW_HueOffs = Bp_Context[W_temp_offset + B_Region * 18];

            gma_CorrectionParamDefs[B_Region].SW_HueGain = Bp_Context[W_temp_offset + 2 + B_Region * 18];

            gma_CorrectionParamDefs[B_Region].SW_SatOffs = (Bp_Context[W_temp_offset + 5 + B_Region * 18]<<8) + Bp_Context[W_temp_offset + 4 + B_Region * 18];

            gma_CorrectionParamDefs[B_Region].W_SatGain = Bp_Context[W_temp_offset + 6 + B_Region * 18];

            gma_CorrectionParamDefs[B_Region].SW_LumiOffs = (Bp_Context[W_temp_offset + 9 + B_Region * 18]<<8) + Bp_Context[W_temp_offset + 8 + B_Region * 18];

            gma_CorrectionParamDefs[B_Region].W_LumiGain = Bp_Context[W_temp_offset + 10 + B_Region * 18];

            gma_CorrectionParamDefs[B_Region].SW_U_Vect = (Bp_Context[W_temp_offset + 13 + B_Region * 18]<<8) + Bp_Context[W_temp_offset + 12 + B_Region * 18];
            gma_CorrectionParamDefs[B_Region].SW_V_Vect = (Bp_Context[W_temp_offset + 15 + B_Region * 18]<<8) + Bp_Context[W_temp_offset + 14 + B_Region * 18];
            gma_CorrectionParamDefs[B_Region].W_Alpha = Bp_Context[W_temp_offset + 16 + B_Region * 18];
         }
         // when all data are ready, initialize ACM3
         gm_ACM3DInit();

         // sync update called from gm_ACM3DInit() dosn't work in debug mode.
         // execute sync update directly.
         gm_WriteRegWord(HOST_CONTROL,ODP_SYNC_UPDATE);
      }
      Bp_out[0] = TS_OK;
      *Bp_len = 1;
      msg("STTS_Handler_ACM3Dv2: TS_ACM_WRITE \n", 0);
   }
   else if (W_CurrentCmd == TS_ACM_EN)
   {
      gm_SetRegBitsWord(ACM3_CTRL, ACM3_EN );
      //gm_SyncUpdate(CH_A, INPUT_OUTPUT_CLOCK);
      gm_WriteRegWord(HOST_CONTROL,ODP_SYNC_UPDATE);
      Bp_out[0] = ACM3_OK;
      *Bp_len = 1;
      msg("STTS_Handler_ACM3Dv2: TS_ACM_EN \n", 0);
   }
   else if (W_CurrentCmd == TS_ACM_DISABLE)
   {
      gm_ClearRegBitsWord(ACM3_CTRL, ACM3_EN);
      //gm_SyncUpdate(CH_A, INPUT_OUTPUT_CLOCK);
      gm_WriteRegWord(HOST_CONTROL,ODP_SYNC_UPDATE);
      Bp_out[0] = ACM3_OK;
      *Bp_len = 1;
      msg("STTS_Handler_ACM3Dv2: TS_ACM_DISABLE \n", 0);
   }
   else
   {
      msg("STTS_Handler_ACM3Dv2: UNKNOWN COMMAND \n", 0);
      Bp_out[0] = TS_NOT_SUPPORTED;
      *Bp_len = 1;
   }

   return (BYTE)PORT_MSG_HNDLR_OK;
}
#else
static BYTE ACM3_SMT_Write(BYTE B_TransId, BYTE * Bp_msg, BYTE comm_use,
                           BYTE far * Bp_Context, BYTE B_NextTMsg);
static BYTE ACM3_SMT_Read(BYTE B_TransId, BYTE * Bp_msg, BYTE comm_use,
                          BYTE far * Bp_Context, BYTE B_NextTMsg);

//******************************************************************************
//
// FUNCTION     :   BYTE ACM3_SMT_MsgHandler(MsgPacket_t *message, BYTE comm_use)
// USAGE        :   This function parse the SMT messages for ACM3 feature
//
// INPUT        :   MsgPacket_t *message = SMT message
//                  BYTE comm_use  = SERIAL_USE or DDC2BI_USE
// OUTPUT       :   OK = if no error, ERR_SEND = if error is found
//
//******************************************************************************
BYTE ACM3_SMT_MsgHandler(MsgPacket_t *message, BYTE comm_use)
{
   BYTE * Bp_msg = (BYTE *)message;
//   BYTE B_Success = ERR_SEND;
   gmt_MsgStruct  OutMsg;
   BYTE B_TransId;
   BYTE B_ContextStatus;
   BYTE far * Bp_Context = NULL_PTR;
   BYTE B_ContextCnt = 0;
   WORD W_CurrentCmd = ACM3_UNUSEDCMD;

   B_TransId = message->control;

   // get buffer pointer from SMT driver
   B_ContextStatus = SMT_GetBuffer( B_TransId, (BYTE far * far *)&Bp_Context, ACM3_FULL_CONTEXT_SIZE );

   // If B_ContextStatus is FIRST_REQUEST clear Bp_Context buffer
   if ( B_ContextStatus == FIRST_REQUEST )
   {
      for ( B_ContextCnt = 0; B_ContextCnt < ACM3_FULL_CONTEXT_SIZE; B_ContextCnt++ )
         *(Bp_Context + B_ContextCnt) = 0;
      W_CurrentCmd = (Bp_msg[5] << 8) + Bp_msg[4];
      *(Bp_Context + 0) = Bp_msg[4];
      *(Bp_Context + 1) = Bp_msg[5];
   }
   else if ( B_ContextStatus == NEXT_REQUEST )
   {
      W_CurrentCmd = ((*(Bp_Context + 1))<<8) + (*(Bp_Context + 0));
   }
   else if (B_ContextStatus == NO_MEMORY) // do we have valid allocated memory ?
   {
      // send replay
      Bp_msg[0] = 6;      // message length
      Bp_msg[1] = CMD_SMTProtocol;   // GProbe SMT comand
      //Bp_msg[2] = EvOdd;   // return even/odd flag
      Bp_msg[3] = B_TransId;
      Bp_msg[4] = ACM3_ERROR;
      gm_WriteCmdPkt((BYTE far*)Bp_msg, comm_use);
      return PORT_MSG_HNDLR_SELFCARED;//ERR_PARAM;
   }

   switch ( W_CurrentCmd )
   {
#if 1 // Douglas
      case ACM3_ENABLE:    // enable ACM3 HW block
         gm_SetRegBitsWord(ACM3_CTRL, ACM3_EN );
         // send replay
         Bp_msg[0] = 6;      // message length
         Bp_msg[3] = B_TransId;
         Bp_msg[4] = ACM3_OK;
         gm_WriteCmdPkt((BYTE far*)Bp_msg, comm_use);
         break;

      case ACM3_DISABLE:    // disable ACM3 HW block
         gm_ClearRegBitsWord(ACM3_CTRL, ACM3_EN);
         // send replay
         Bp_msg[0] = 6;      // message length
         Bp_msg[3] = B_TransId;
         Bp_msg[4] = ACM3_OK;
         gm_WriteCmdPkt((BYTE far*)Bp_msg, comm_use);
         break;
#endif
      case ACM3_GETMAXREGION:
         // send replay
         Bp_msg[0] = 7;      // message length
         Bp_msg[3] = B_TransId;
         Bp_msg[4] = ACM3_OK;
         Bp_msg[5] = ACM3_REGION_NUM; // the supported by HW number of regions
         gm_WriteCmdPkt((BYTE far*)Bp_msg, comm_use);
         break;
      case ACM3_GETMAXREGIONSPERTRANS:
         // send replay
         Bp_msg[0] = 7;      // message length
         Bp_msg[3] = B_TransId;
         Bp_msg[4] = ACM3_OK;
         Bp_msg[5] = 1; // the supported number of region's data per transaction
         gm_WriteCmdPkt((BYTE far*)Bp_msg, comm_use);
         break;
      case ACM3_WRITEANDRUN:
         /*B_Success =*/
         ACM3_SMT_Write( B_TransId, Bp_msg, comm_use, Bp_Context,
                         B_ContextStatus);
         break;
      case ACM3_READALL:
         /*B_Success =*/
         ACM3_SMT_Read( B_TransId, Bp_msg, comm_use, Bp_Context,
                        B_ContextStatus);
         break;
      case ACM3_SETWINDOW:   // set window to show ACM3
         {
            BYTE B_WindowCtrl;

            B_WindowCtrl = Bp_msg[ 6 ];

#if 1 // ACC2 
            gm_ACM3DUpdateCtrl( (B_WindowCtrl?ACM3_EN:0), (B_WindowCtrl & ACC_ACM_FULLSCREEN));
#else
            gm_ACM3DUpdateCtrl( (B_WindowCtrl?ACM3_EN:0), (B_WindowCtrl | ACM3_WIN_CTRL));
#endif
            //B_Success = OK;
            // send replay
            Bp_msg[0] = 6;      // message length
            Bp_msg[3] = B_TransId;
            Bp_msg[4] = ACM3_OK;
            gm_WriteCmdPkt((BYTE far*)Bp_msg, comm_use);
         }
         break;
      default:
         // send replay
         Bp_msg[0] = 6;      // message length
         Bp_msg[3] = B_TransId;
         Bp_msg[4] = ACM3_ERROR;
         gm_WriteCmdPkt((BYTE far*)Bp_msg, comm_use);
         break;
   };
   return PORT_MSG_HNDLR_SELFCARED;//;
}


//******************************************************************************
//
// FUNCTION     :   static BYTE ACM3_SMT_Write(BYTE B_TransId, BYTE * Bp_msg, BYTE comm_use)
// USAGE        :   This function parses the SMT ACM3_WRITEANDRUN message
//
// INPUT        :   BYTE * Bp_msg points to SMT message
// OUTPUT       :   OK = if no error, ERR_ = if error is found
//
//******************************************************************************
static BYTE ACM3_SMT_Write(BYTE B_TransId, BYTE * Bp_msg,
                           BYTE comm_use, BYTE far * Bp_Context, BYTE B_NextTMsg)
{
   BYTE B_Region;
   BYTE B_ContextCnt = 0;
   BYTE B_SrcCnt = 0;
   BYTE * Bp_Src = Bp_msg;
   BYTE B_SrcSize; // sub GProbe header and footer
   gmt_ACM3DRegionDef gma_RegionDefs[ACM3D_REGION_NUM] =
   {
      //   W_HueCenter, B_HueAperture,   B_R1, B_R2, B_Y1, B_Y2, B_HueFade, B_SatFade, B_LumiFade
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
   };
   gmt_ACM3DCorrectionParams gma_CorrectionParamDefs[ACM3D_REGION_NUM] =
   {
      //    sB_HueOffs, sB_HueGain, sB_SatOffs, sB_SatGain, sB_LumiOffs, sB_LumiGain, sB_U_Vect, sB_V_Vect, B_Alpha
      { 0, 0, 0, 0, 0, 0, 0, 0, 0},
      { 0, 0, 0, 0, 0, 0, 0, 0, 0},
      { 0, 0, 0, 0, 0, 0, 0, 0, 0},
      { 0, 0, 0, 0, 0, 0, 0, 0, 0},
      { 0, 0, 0, 0, 0, 0, 0, 0, 0},
      { 0, 0, 0, 0, 0, 0, 0, 0, 0},
      { 0, 0, 0, 0, 0, 0, 0, 0, 0},
      { 0, 0, 0, 0, 0, 0, 0, 0, 0},
   };

   B_ContextCnt = *(Bp_Context + 2);
   if (B_NextTMsg)
   {
      Bp_Src = Bp_Src + 4; // to skip Gprobe header
      B_SrcSize = Bp_msg[0] - 5;
   }
   else
   {
      Bp_Src = Bp_Src + 6; // to skip Gprobe header
      B_SrcSize = Bp_msg[0] - 7;
   }

   while (B_SrcCnt < B_SrcSize)
   {
      *(Bp_Context + 3 + B_ContextCnt) = *(Bp_Src + B_SrcCnt);
      B_ContextCnt++;
      (*(Bp_Context + 2))++; // data counter
      B_SrcCnt++;

      if ( B_ContextCnt >= (ACM3_CONTEXT_SIZE - 1))
      {
         //
         // region is filled out, ready to write to HW
         //
         B_Region = Bp_Context[ REGION_B ];
         if (( B_Region > ACM3_REGION_NUM ) || (B_Region == 0))
         {
            // send replay
            Bp_msg[0] = 6;      // message length
            Bp_msg[3] = B_TransId;
            Bp_msg[4] = ACM3_INVALIDPARAM;
            gm_WriteCmdPkt((BYTE far*)Bp_msg, comm_use);
            return ERR_PARAM;
         }
         else
            B_Region--;

         // read all data, update recieved region data and write them back
         // Set pointers to temporary arrays
         gm_ACM3DSetRegionDefinitionTablePtr((gmt_ACM3DRegionDef far *)&gma_RegionDefs);
         gm_ACM3DSetCorrectionParamsTablePtr((gmt_ACM3DCorrectionParams far *)&gma_CorrectionParamDefs);
         gm_ACM3DReadAll(); // read ACM3D data to these arrays (all 8 regiones)
         // copy values one-by-one
         gma_RegionDefs[B_Region].W_HueCenter =
            (Bp_Context[HUE_CENT_HB]<<8)+Bp_Context[HUE_CENT_LB];
         gma_RegionDefs[B_Region].B_HueAperture = Bp_Context[HUE_APERT_LB];
         gma_RegionDefs[B_Region].B_HueFade = Bp_Context[HUE_FADE_LB];
         gma_RegionDefs[B_Region].B_Y1 = Bp_Context[LUMI_Y1_LB];
         gma_RegionDefs[B_Region].B_Y2 = Bp_Context[LUMI_Y2_LB];
         gma_RegionDefs[B_Region].B_LumiFade = Bp_Context[LUMI_FADE_LB];
         gma_RegionDefs[B_Region].B_R1 = Bp_Context[SAT_R1_LB];
         gma_RegionDefs[B_Region].B_R2 = Bp_Context[SAT_R2_LB];
         gma_RegionDefs[B_Region].B_SatFade = Bp_Context[SAT_FADE_LB];

         gma_CorrectionParamDefs[B_Region].sB_HueGain = Bp_Context[HUE_GAIN_LB];
         gma_CorrectionParamDefs[B_Region].sB_HueOffs = Bp_Context[HUE_OFFS_LB];
         gma_CorrectionParamDefs[B_Region].sB_LumiGain = Bp_Context[LUMI_GAIN_LB];
         gma_CorrectionParamDefs[B_Region].sB_LumiOffs = Bp_Context[LUMI_OFFS_LB];
         gma_CorrectionParamDefs[B_Region].sB_SatGain = Bp_Context[SAT_GAIN_LB];
         gma_CorrectionParamDefs[B_Region].sB_SatOffs = Bp_Context[SAT_OFFS_LB];
         gma_CorrectionParamDefs[B_Region].sB_U_Vect = Bp_Context[U_VECT_LB];
         gma_CorrectionParamDefs[B_Region].sB_V_Vect = Bp_Context[V_VECT_LB];
         gma_CorrectionParamDefs[B_Region].B_Alpha = Bp_Context[ALPHA_LB];

         // when all data are ready, initialize ACM3
         gm_ACM3DInit();

         // sync update called from gm_ACM3DInit() dosn't work in debug mode.
         // execute sync update directly.
         gm_WriteRegWord(HOST_CONTROL,ODP_SYNC_UPDATE);

         B_ContextCnt = 0; // clear local counter
         *(Bp_Context + 2) = 0; // clear counter in context
      }
   }

   // send replay
   Bp_msg[0] = 6;      // message length
   Bp_msg[3] = B_TransId;
   Bp_msg[4] = ACM3_OK;
   gm_WriteCmdPkt((BYTE far*)Bp_msg, comm_use);
   return OK;
}

//******************************************************************************
//
// FUNCTION     :   static BYTE ACM3_SMT_Read(BYTE B_TransId, BYTE * Bp_msg, BYTE comm_use)
// USAGE        :   This function executes the SMT ACM3_READ message
//
// INPUT        :   BYTE * Bp_msg points to SMT message
// OUTPUT       :   OK = if no error, ERR_SEND = if error is found
//
//******************************************************************************
static BYTE ACM3_SMT_Read(BYTE B_TransId, BYTE * Bp_msg, BYTE comm_use,
                          BYTE far * Bp_Context, BYTE B_NextTMsg)
{
   BYTE B_Region;
   BYTE B_Offs;
   gmt_ACM3DRegionDef gma_RegionDefs[ACM3D_REGION_NUM] =
   {
      //   W_HueCenter, B_HueAperture,   B_R1, B_R2, B_Y1, B_Y2, B_HueFade, B_SatFade, B_LumiFade
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
   };
   gmt_ACM3DCorrectionParams gma_CorrectionParamDefs[ACM3D_REGION_NUM] =
   {
      //    sB_HueOffs, sB_HueGain, sB_SatOffs, sB_SatGain, sB_LumiOffs, sB_LumiGain, sB_U_Vect, sB_V_Vect, B_Alpha
      { 0, 0, 0, 0, 0, 0, 0, 0, 0},
      { 0, 0, 0, 0, 0, 0, 0, 0, 0},
      { 0, 0, 0, 0, 0, 0, 0, 0, 0},
      { 0, 0, 0, 0, 0, 0, 0, 0, 0},
      { 0, 0, 0, 0, 0, 0, 0, 0, 0},
      { 0, 0, 0, 0, 0, 0, 0, 0, 0},
      { 0, 0, 0, 0, 0, 0, 0, 0, 0},
      { 0, 0, 0, 0, 0, 0, 0, 0, 0},
   };

   if (B_NextTMsg)
   {
      B_Region = *(Bp_Context + 2);
   }
   else
   {
      B_Region = 0;
      *(Bp_Context + 2) = B_Region;
   }

   // to differentiate between first and other messages
   B_Offs = ( B_Region == 0 ) ? 1 : 0;

   // Set pointers to temporary arrays
   gm_ACM3DSetRegionDefinitionTablePtr((gmt_ACM3DRegionDef far *)&gma_RegionDefs);
   gm_ACM3DSetCorrectionParamsTablePtr((gmt_ACM3DCorrectionParams far *)&gma_CorrectionParamDefs);

   // read ACM3D data to these arrays (all 8 regiones)
   gm_ACM3DReadAll();

   // copy data from arrays to buffer to replay to SMT master
   // copy values one-by-one
   Bp_msg[B_Offs + 4] = B_Region + 1;
   Bp_msg[B_Offs+1+HUE_CENT_LB] = (BYTE)(0xFF & gma_RegionDefs[B_Region].W_HueCenter);
   Bp_msg[B_Offs+1+HUE_CENT_HB] = (BYTE)(0xFF & (gma_RegionDefs[B_Region].W_HueCenter>>8));
   Bp_msg[B_Offs+1+HUE_APERT_LB] = (BYTE)(0xFF & gma_RegionDefs[B_Region].B_HueAperture);
   Bp_msg[B_Offs+1+HUE_APERT_HB] = 0;
   Bp_msg[B_Offs+1+HUE_FADE_LB] = (BYTE)(0xFF & gma_RegionDefs[B_Region].B_HueFade);
   Bp_msg[B_Offs+1+HUE_FADE_HB] = 0;
   Bp_msg[B_Offs+1+LUMI_Y1_LB] = (BYTE)(0xFF & gma_RegionDefs[B_Region].B_Y1);
   Bp_msg[B_Offs+1+LUMI_Y1_HB] = 0;
   Bp_msg[B_Offs+1+LUMI_Y2_LB] = (BYTE)(0xFF & gma_RegionDefs[B_Region].B_Y2);
   Bp_msg[B_Offs+1+LUMI_Y2_HB] = 0;
   Bp_msg[B_Offs+1+LUMI_FADE_LB] = (BYTE)(0xFF & gma_RegionDefs[B_Region].B_LumiFade);
   Bp_msg[B_Offs+1+LUMI_FADE_HB] = 0;
   Bp_msg[B_Offs+1+SAT_R1_LB] = (BYTE)(0xFF & gma_RegionDefs[B_Region].B_R1);
   Bp_msg[B_Offs+1+SAT_R1_HB] = 0;
   Bp_msg[B_Offs+1+SAT_R2_LB] = (BYTE)(0xFF & gma_RegionDefs[B_Region].B_R2);
   Bp_msg[B_Offs+1+SAT_R2_HB] = 0;
   Bp_msg[B_Offs+1+SAT_FADE_LB] = (BYTE)(0xFF & gma_RegionDefs[B_Region].B_SatFade);
   Bp_msg[B_Offs+1+SAT_FADE_HB] = 0;

   Bp_msg[B_Offs+1+HUE_GAIN_LB] = (BYTE)(0xFF & gma_CorrectionParamDefs[B_Region].sB_HueGain);
   Bp_msg[B_Offs+1+HUE_GAIN_HB] = ((gma_CorrectionParamDefs[B_Region].sB_HueGain & BIT3)==BIT3)?0xFF:0x00;
   Bp_msg[B_Offs+1+HUE_OFFS_LB] = (BYTE)(0xFF & gma_CorrectionParamDefs[B_Region].sB_HueOffs);
   Bp_msg[B_Offs+1+HUE_OFFS_HB] = ((gma_CorrectionParamDefs[B_Region].sB_HueOffs & BIT7)==BIT7)?0xFF:0x00;
   Bp_msg[B_Offs+1+LUMI_GAIN_LB] = (BYTE)(0xFF & gma_CorrectionParamDefs[B_Region].sB_LumiGain);
   Bp_msg[B_Offs+1+LUMI_GAIN_HB] = 0x00;
   Bp_msg[B_Offs+1+LUMI_OFFS_LB] = (BYTE)(0xFF & gma_CorrectionParamDefs[B_Region].sB_LumiOffs);
   Bp_msg[B_Offs+1+LUMI_OFFS_HB] = ((gma_CorrectionParamDefs[B_Region].sB_LumiOffs & BIT7)==BIT7)?0xFF:0x00;
   Bp_msg[B_Offs+1+SAT_GAIN_LB] = (BYTE)(0xFF & gma_CorrectionParamDefs[B_Region].sB_SatGain);
   Bp_msg[B_Offs+1+SAT_GAIN_HB] = 0x00;
   Bp_msg[B_Offs+1+SAT_OFFS_LB] = (BYTE)(0xFF & gma_CorrectionParamDefs[B_Region].sB_SatOffs);
   Bp_msg[B_Offs+1+SAT_OFFS_HB] = ((gma_CorrectionParamDefs[B_Region].sB_SatOffs & BIT7)==BIT7)?0xFF:0x00;

   Bp_msg[B_Offs+1+U_VECT_LB] = (BYTE)(0xFF & gma_CorrectionParamDefs[B_Region].sB_U_Vect);
   Bp_msg[B_Offs+1+U_VECT_HB] = ((gma_CorrectionParamDefs[B_Region].sB_U_Vect & BIT7)==BIT7)?0xFF:0x00;
   Bp_msg[B_Offs+1+V_VECT_LB] = (BYTE)(0xFF & gma_CorrectionParamDefs[B_Region].sB_V_Vect);
   Bp_msg[B_Offs+1+V_VECT_HB] = ((gma_CorrectionParamDefs[B_Region].sB_V_Vect & BIT7)==BIT7)?0xFF:0x00;
   Bp_msg[B_Offs+1+ALPHA_LB] = (BYTE)(0xFF & gma_CorrectionParamDefs[B_Region].B_Alpha);
   Bp_msg[B_Offs+1+ALPHA_HB] = 0;

   // to indicate the last transport message
   if ( B_Region == ACM3_REGION_NUM-1 )
   {
      Bp_msg[2] = Bp_msg[2] & (~MORE_DATA_BIT);
   }
   else
   {
      Bp_msg[2] = Bp_msg[2] | MORE_DATA_BIT;
   }

   // send replay
   Bp_msg[0] = SMT_REGION_DATA_SIZE + B_Offs + 1 + 5;      // message length
   Bp_msg[3] = B_TransId;
   Bp_msg[4] = ACM3_OK;
   gm_WriteCmdPkt((BYTE far*)Bp_msg, comm_use);

   B_Region++;
   *(Bp_Context + 2) = B_Region;
   return OK;
}
#endif

#endif   // #if (FEATURE_ACM_3D == ENABLE)
#endif   // #ifdef USE_SMT


