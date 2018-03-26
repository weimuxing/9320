/*
   $Workfile:   smt_GammaCorrection.c  $
   $Revision: 1.55 $
   $Date: 2013/04/29 03:17:00 $
*/

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
// MODULE:      smt_GammaCorrection.c
//
//******************************************************************************
//
// USAGE:   This file contains a SMT port handler GammaCorr_MsgHandler to provide
//         support of Gamma calibration tools
//
//******************************************************************************
// DOCUMENTS:
// http://torpvcs/TorontoSoftwareDocs/Tools_FW_Interface/SMT/Gamma/ Objectives_and_Requirements.doc Rev_1.4
// Document describes high level protocol between external tool and this message handler
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#include <mem.h>
#include "CommLink\smt.h"
#include "CommLink\smt_GammaCorrection.h"
#include "stdlibreplace\gm_UploadSRAMBlock.h"


BYTE B_SelectedProfile = 0;
#ifdef DELL_U2410F
#if COLOR_CALI_FORCE_6500
WORD B_tempRGain = 0;
WORD B_tempGGain = 0;
WORD B_tempBGain = 0;
#endif
#endif   
//extern BYTE B_ReturnBusy;
//extern BYTE B_ODD_EVEN_OverRide;
extern BYTE B_ReplyEvenOdd; 

//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************
#ifdef USE_SMT

#define   DEBUG_SMT   OFF // ON

#if (DEBUG_SMT == ON)
#define msg(a,b) gm_Print((const char far *)a,b)
#define msgd(a,b,c) gm_Printf((const far char  *)a,b,c)
#else
#define msg(a,b)
#define msgd(a,b,c)
#endif

#if  (FEATURE_VLUT_SIZE == 10)
#define TABLE_ACCESS_CTRL    TABLE_ACCESS_BRIDGE_CTRL1
#else
#define TABLE_ACCESS_CTRL    TABLE_ACCESS_BRIDGE_CTRL
#endif

#define ALL_VLUT_COMPONENT 7
//gamma v.4

#define SMT_3X3_WIDTH 16
#define ATHENA_3X3_WIDTH 10 // 14 // not total bits but the fraction
#define SHIFT_3X3 (SMT_3X3_WIDTH - ATHENA_3X3_WIDTH)

#if (GAMMA_SMT_PROTOCOL_VER >= VERSION_6)
#define ATHENA_3X3_WIDTH_VERSION_6          14 //version 6 3x3 matrix notation 1.3.10, sign bit + fraction bits + integer bits
#define ATHENA_3X3_FRACTIONWIDTH_VERSION_6 10 //version 6 3x3 matrix notation 1.3.10, fraction 10 bits
#define ATHENA_3X3_INTEGERWIDTH_VERSION_6   3 //version 6 3x3 matrix notation 1.3.10, integer 3 bits
#define ATHENA_3x3_COEF_MASK_BITS           0x3FFF  //total 14 bits
#define ATHENA_3x3_OFFSET_MASK_BITS         0x7FFF  //0-14 bits are used, total 15 bits
#endif

#define INPUT_GAMMA_R_MEM_START 0x1C000L
#define INPUT_GAMMA_R_STEEP_MEM_START 0x1C400L
#define INPUT_GAMMA_G_MEM_START 0x1C800L
#define INPUT_GAMMA_G_STEEP_MEM_START 0x1CC00L
#define INPUT_GAMMA_B_MEM_START 0x1D000L
#define INPUT_GAMMA_B_STEEP_MEM_START 0x1D400L
#define OUTPUT_GAMMA_R_MEM_START 0x1D800L
#define OUTPUT_GAMMA_R_STEEP_MEM_START 0x1DC00L
#define OUTPUT_GAMMA_G_MEM_START 0x1E000L
#define OUTPUT_GAMMA_G_STEEP_MEM_START 0x1E400L
#define OUTPUT_GAMMA_B_MEM_START 0x1E800L
#define OUTPUT_GAMMA_B_STEEP_MEM_START 0x1EC00L

#define GAMMA_COMPONENT_SAVED_START_OFFSET 0
#define GAMMA_COMPONENT_SAVED_SIZE 4
#define GAMMA_REGISTER_START_OFFSET (GAMMA_COMPONENT_SAVED_START_OFFSET+GAMMA_COMPONENT_SAVED_SIZE)
#define GAMMA_REGISTER_SIZE 30 // 82 // 15 registers
#define POST_LUT_CONFIG_REG_START_OFFSET (GAMMA_REGISTER_START_OFFSET+GAMMA_REGISTER_SIZE)
#define POST_LUT_CONFIG_REG_SIZE 26 // 1 + 12 registers
#define POST_LUT_START_OFFSET (POST_LUT_CONFIG_REG_START_OFFSET+POST_LUT_CONFIG_REG_SIZE) // (GAMMA_REGISTER_START_OFFSET+GAMMA_REGISTER_SIZE)
#define POST_LUT_SIZE 1024*3
#define PRE_LUT_CONFIG_REG_START_OFFSET (POST_LUT_START_OFFSET+POST_LUT_SIZE) // (GAMMA_REGISTER_START_OFFSET+GAMMA_REGISTER_SIZE)
#define PRE_LUT_CONFIG_REG_SIZE 26 // 1 + 12 registers
#define PRE_LUT_START_OFFSET (PRE_LUT_CONFIG_REG_START_OFFSET+PRE_LUT_CONFIG_REG_SIZE)
#define PRE_LUT_SIZE 1024*3
#define POST_STEEP_START_OFFSET (PRE_LUT_START_OFFSET+PRE_LUT_SIZE)
#define POST_STEEP_SIZE 256*3
#define PRE_STEEP_START_OFFSET (POST_STEEP_START_OFFSET+POST_STEEP_SIZE)

// version 5
#define Gamma_PostLUT_Component   BIT0
#define Gamma_PreLUT_Component    BIT1
#define Gamma_Matrix_Component    BIT2
#define Gamma_PostSteep_Component BIT3
#define Gamma_PreSteep_Component  BIT4
#define Gamma_Athena_Default_Component       0x1F
#define Gamma_MaxProfile 2

#ifdef ST_4K2K_93xx_BOARD
extern void SMT_SendTimeoutTimetoHost(MsgPacket_t *message, BYTE comm_use, WORD Timeout);
#endif

#if (GAMMA_SMT_PROTOCOL_VER >= VERSION_6)
static BYTE B_SMTICVersion = VERSION_6; //IC version number from SMT command Gamma_StartCalibration
static BOOL B_IsSMTICStarted = FALSE;

//array to save ovp 3x3 matrix coeff and output offset register values
//initialize 3x3 to [1 0 0]
//              [0 1 0]
//              [0 0 1]
//all output offset to 0
static WORD W_OVP3x3MatrixSaved[12] = {
    //11    12      13
    0x200,  0,      0,
    //21    22      23
    0,      0x200,  0,
    //31    32      33
    0,      0,      0x200,
    //output
    //Offset1   Offset2     Offset3
    0,          0,          0
};


static void SetSMTICToolVersion(BYTE VersionNumber);
static BYTE GetSMTICToolVersion(void);
static void SetSMTICStarted(BOOL Status);
static BOOL GetSMTICStarted(void);
static PortMsgHndlrReplyType BackupOVP3x3Matrix(WORD * OVP3x3MatrixData);
static PortMsgHndlrReplyType ProgramOVP3x3MatrixReg(WORD * OVP3x3MatrixData);

#define IC2_DEBUG_PRINT

#endif
//******************************************************************************
//  C O D E
//******************************************************************************
/*
void ShiftRight4bits(WORD W_Size, WORD far *Wp_Data)
{
   WORD W_Index;

   //gm_Printf("Wp_Data[32] = 0x%X", Wp_Data[32]);
   //gm_Printf("Wp_Data[2] = 0x%X", Wp_Data[2]);

   for (W_Index = 0; W_Index < W_Size; W_Index++)
   {
      Wp_Data[W_Index] = Wp_Data[W_Index] >> 4;
      if (W_Index == 32)
         gm_Printf("Wp_Data[W_Index] = 0x%X", Wp_Data[W_Index]);
   }

   //gm_Printf("Wp_Data[32] = 0x%X", Wp_Data[32]);
   //gm_Printf("Wp_Data[2] = 0x%X", Wp_Data[2]);


}
*/
//******************************************************************************
//
// FUNCTION     :   BYTE GammaCorr_MsgHandler(MsgPacket_t *message, BYTE comm_use)
// USAGE        :   This function parse the SMT messages
//
// INPUT        :   MsgPacket_t *message = SMT message
//                  BYTE comm_use  = SERIAL_USE or DDC2BI_USE
// OUTPUT       :   OK = if no error,
//
//******************************************************************************
BYTE Gamma_Apply_Profile(BYTE far * Bp_Component, BYTE B_Selected_Profile)
{
   DWORD DW_ComponentToApply = 0;
   BYTE B_Return = 0;

   #ifdef SAVING_DATA_INTO_FLASH
   	DWORD DW_FlashAddr = 0x5FA000; // 0x1FB000          
   #elif defined(SAVING_DATA_INTO_NVRAM)          
   	WORD	DW_NvramAddr;
   #endif
   
   // Component
   msg(" Component = 0x%x ",Bp_Component[1]);

   // reserved byte should be 0
   if((Bp_Component[1] & 0xE0) || Bp_Component[2] || Bp_Component[3] || Bp_Component[4])
   {
      //gm_Printf("Invalid Component", 0);
      B_Return = 1; // Not Support
      DW_ComponentToApply = 0;
      return B_Return;
   }
   
   DW_ComponentToApply = Bp_Component[1] & Gamma_Athena_Default_Component;
   
   #ifdef SAVING_DATA_INTO_FLASH            
   while(DW_ComponentToApply)
   {
      DWORD DW_LUTAddr;
      
      // reset the base offset for each component
      DW_FlashAddr = 0x5FA000 - (0x2000*B_Selected_Profile); // 2 sector for 1 color space
      
      if((FlashReadWord(DW_FlashAddr) == 0xFFFF) || 
         ((FlashReadWord(DW_FlashAddr) & DW_ComponentToApply) != DW_ComponentToApply))
      {
         B_Return = 2; // Component not available
         //*Bp_Len = 1;
         DW_ComponentToApply = 0;
         return B_Return;
         //break;
      }               

      if(DW_ComponentToApply & 0x1B) // Other than Matrix
      {
         WORD i, W_size;
         BYTE j;

         for(j = 0; j < 3; j++)
         {
            if(j == 0) // R
            {
               if(DW_ComponentToApply & Gamma_PostLUT_Component)
               {
                  DW_LUTAddr = OUTPUT_GAMMA_R_MEM_START;
                  DW_FlashAddr += POST_LUT_START_OFFSET;
                  W_size = 1024;
               }
               else if(DW_ComponentToApply & Gamma_PreLUT_Component)
               {
                  DW_LUTAddr = INPUT_GAMMA_R_MEM_START;
                  DW_FlashAddr += PRE_LUT_START_OFFSET;
                  W_size = 1024;
               }                        
               else if(DW_ComponentToApply & Gamma_PostSteep_Component)
               {
                  DW_LUTAddr = OUTPUT_GAMMA_R_STEEP_MEM_START;
                  DW_FlashAddr += POST_STEEP_START_OFFSET;
                  W_size = 128;
               }                        
               else if(DW_ComponentToApply & Gamma_PreSteep_Component)
               {
                  DW_LUTAddr = INPUT_GAMMA_R_STEEP_MEM_START;
                  DW_FlashAddr += PRE_STEEP_START_OFFSET;
                  W_size = 128;
               }                        
               else
               {
                  //gm_Printf("Invalid Component R", 0);
                  B_Return = 3; // Component not available
                  DW_ComponentToApply = 0;
                  //break; // Break the while loop
                  return B_Return;
               }
            }
            else if(j == 1) // G
            {
               if(DW_ComponentToApply & Gamma_PostLUT_Component)
               {
                  DW_LUTAddr = OUTPUT_GAMMA_G_MEM_START;
                  W_size = 1024;
               }
               else if(DW_ComponentToApply & Gamma_PreLUT_Component)
               {
                  DW_LUTAddr = INPUT_GAMMA_G_MEM_START;
                  W_size = 1024;
               }                        
               else if(DW_ComponentToApply & Gamma_PostSteep_Component)
               {
                  DW_LUTAddr = OUTPUT_GAMMA_G_STEEP_MEM_START;
                  W_size = 128;
               }                        
               else if(DW_ComponentToApply & Gamma_PreSteep_Component)
               {
                  DW_LUTAddr = INPUT_GAMMA_G_STEEP_MEM_START;
                  W_size = 128;
               }                        
               else
               {
                  //gm_Printf("Invalid Component G", 0);
                  B_Return = 3; // Component not available
                  DW_ComponentToApply = 0;
                  //break;// break the while loop
                  return B_Return;
               }                     
            }
            else // B
            {
               if(DW_ComponentToApply & Gamma_PostLUT_Component)
               {
                  DW_LUTAddr = OUTPUT_GAMMA_B_MEM_START;
                  W_size = 1024;
                  //CLRBIT(DW_ComponentToApply, Gamma_PostLUT_Component);
               }
               else if(DW_ComponentToApply & Gamma_PreLUT_Component)
               {
                  DW_LUTAddr = INPUT_GAMMA_B_MEM_START;
                  W_size = 1024;
                  //CLRBIT(DW_ComponentToApply, Gamma_PreLUT_Component);
               }                        
               else if(DW_ComponentToApply & Gamma_PostSteep_Component)
               {
                  DW_LUTAddr = OUTPUT_GAMMA_B_STEEP_MEM_START;
                  W_size = 128;
                  CLRBIT(DW_ComponentToApply, Gamma_PostSteep_Component);
               }                        
               else if(DW_ComponentToApply & Gamma_PreSteep_Component)
               {
                  DW_LUTAddr = INPUT_GAMMA_B_STEEP_MEM_START;
                  W_size = 128;
                  CLRBIT(DW_ComponentToApply, Gamma_PreSteep_Component);
               }                        
               else
               {
                  //gm_Printf("Invalid Component B", 0);
                  B_Return = 3; // Component not available
                  DW_ComponentToApply = 0;
                  //break; // break the while loop
                  return B_Return;
               }                     
            }
                              
            for(i = 0; i < W_size; i++)
            {                     
               if(W_size == 128)
               {                           
                  *LINEAR_TO_FPTR_W(DW_LUTAddr) = FlashReadWord(DW_FlashAddr);
                  //FlashWriteWord(DW_FlashAddr, *LINEAR_TO_FPTR_W(DW_LUTAddr));
                  DW_LUTAddr += 4;
                  DW_FlashAddr += 2;
               }
               else
               {                           
                  *LINEAR_TO_FPTR_B(DW_LUTAddr) = FlashReadByte(DW_FlashAddr);
                  //FlashWriteByte(DW_FlashAddr, *LINEAR_TO_FPTR_B(DW_LUTAddr));
                  DW_LUTAddr += 1;
                  DW_FlashAddr += 1;
               }                                                                       
            }
         }

         if(DW_ComponentToApply & Gamma_PostLUT_Component)
         {
            //gm_Printf(" Apply Post LUT Config Registers", 0);
            DW_FlashAddr = 0x5FA000 - (0x2000*B_Selected_Profile) + POST_LUT_CONFIG_REG_START_OFFSET; // 2 sector for 1 color space
            gm_WriteRegWord(GAMMA_LUT2_CTRL, FlashReadWord(DW_FlashAddr));
               
            DW_FlashAddr += 2;
            DW_LUTAddr = GAMMA_LUT2_STEEP_STRT_R;            

            for(i = 0; i < 12; i++)
            {
               gm_WriteRegWord(DW_LUTAddr, FlashReadWord(DW_FlashAddr));
               DW_LUTAddr += 2;
               DW_FlashAddr += 2;            
            }
            CLRBIT(DW_ComponentToApply, Gamma_PostLUT_Component);
         }
         else if(DW_ComponentToApply & Gamma_PreLUT_Component)
         {

            //gm_Printf(" Apply Pre LUT Config Registers", 0);
            DW_FlashAddr = 0x5FA000 - (0x2000*B_Selected_Profile) + PRE_LUT_CONFIG_REG_START_OFFSET; // 2 sector for 1 color space
            gm_WriteRegWord(GAMMA_LUT1_CTRL, FlashReadWord(DW_FlashAddr));
               
            DW_FlashAddr += 2;
            DW_LUTAddr = GAMMA_LUT1_STEEP_STRT_R;            

            for(i = 0; i < 12; i++)
            {
               gm_WriteRegWord(DW_LUTAddr, FlashReadWord(DW_FlashAddr));
               DW_LUTAddr += 2;
               DW_FlashAddr += 2;            
            }         
            CLRBIT(DW_ComponentToApply, Gamma_PreLUT_Component);
         }            
      }
      else if(DW_ComponentToApply & Gamma_Matrix_Component)
      {
         BYTE i;
         DW_LUTAddr = GAMMA_LUT_MATRIX_MULT_COEF11; // GAMMA_LUT1_CTRL;
         DW_FlashAddr += GAMMA_REGISTER_START_OFFSET;

         //for(i = 0; i < 41; i++) // 41 registers
         for(i = 0; i < 15; i++) // 15 registers
         {                     
            //FlashWriteWord(DW_FlashAddr, gm_ReadRegWord(DW_LUTAddr));
            gm_WriteRegWord(DW_LUTAddr, FlashReadWord(DW_FlashAddr));
            DW_LUTAddr += 2;
            DW_FlashAddr += 2;
            
         }
         CLRBIT(DW_ComponentToApply, Gamma_Matrix_Component);
         if(gm_ReadRegWord(DISP_LUT_CONTROL) != 0x03)
         {
            gm_WriteRegWord(DISP_LUT_CONTROL, 0x3);
            gm_SyncUpdate(gmvb_MainChannel, OUTPUT_CLOCK);
            //gm_Printf("~~~ DISP_LUT_CONTROL ~~~", 0);
         }
      }
      else
      {
         //gm_Printf(" Invalid Input Parameter ", 0);
         B_Return = 3; // Component not available
         DW_ComponentToApply = 0;
         //break; // break the while loop
         return B_Return;
      }
   }
#elif defined(SAVING_DATA_INTO_NVRAM)

   while(DW_ComponentToApply)
   {
      DWORD DW_LUTAddr;
	  BYTE Lut_Buf[1024];
      
      // reset the base offset for each component
//      DW_NvramAddr = C_GAMMA_COMPONENT_SAVED_START_OFFSET+ (0x2000*B_Selected_Profile); // 
      
      if(DW_ComponentToApply & 0x1B) // Other than Matrix
      {
         WORD i, W_size;
         BYTE j;

         for(j = 0; j < 3; j++)
         {
            if(j == 0) // R
            {
               if(DW_ComponentToApply & Gamma_PostLUT_Component)
               {
                  DW_LUTAddr = OUTPUT_GAMMA_R_MEM_START;
				  DW_NvramAddr = C_POST_LUT_START_OFFSET+ (0x2000*B_Selected_Profile); // 
                  W_size = 1024;
               }
               else if(DW_ComponentToApply & Gamma_PreLUT_Component)
               {
                  DW_LUTAddr = INPUT_GAMMA_R_MEM_START;
				  DW_NvramAddr = C_PRE_LUT_START_OFFSET+ (0x2000*B_Selected_Profile); // 
                  W_size = 1024;
               }                        
               else if(DW_ComponentToApply & Gamma_PostSteep_Component)
               {
                  DW_LUTAddr = OUTPUT_GAMMA_R_STEEP_MEM_START;
				  DW_NvramAddr = C_POST_STEEP_START_OFFSET+ (0x2000*B_Selected_Profile); // 
                  W_size = 128;
               }                        
               else if(DW_ComponentToApply & Gamma_PreSteep_Component)
               {
                  DW_LUTAddr = INPUT_GAMMA_R_STEEP_MEM_START;
				  DW_NvramAddr = C_PRE_STEEP_START_OFFSET+ (0x2000*B_Selected_Profile); // 
                  W_size = 128;
               }                        
               else
               {
                  //gm_Printf("Invalid Component R", 0);
                  B_Return = 3; // Component not available
                  DW_ComponentToApply = 0;
                  //break; // Break the while loop
return B_Return;
               }
            }
            else if(j == 1) // G
            {
               if(DW_ComponentToApply & Gamma_PostLUT_Component)
               {
                  DW_LUTAddr = OUTPUT_GAMMA_G_MEM_START;
				  DW_NvramAddr = C_POST_LUT_START_OFFSET + (0x2000*B_SelectedProfile)+ C_ONE_CHANNEL_GAMMA_SIZE; 
                  W_size = 1024;
               }
               else if(DW_ComponentToApply & Gamma_PreLUT_Component)
               {
                  DW_LUTAddr = INPUT_GAMMA_G_MEM_START;
				  DW_NvramAddr = C_PRE_LUT_START_OFFSET + (0x2000*B_SelectedProfile)+ C_ONE_CHANNEL_GAMMA_SIZE; 
                  W_size = 1024;
               }                        
               else if(DW_ComponentToApply & Gamma_PostSteep_Component)
               {
                  DW_LUTAddr = OUTPUT_GAMMA_G_STEEP_MEM_START;
				  DW_NvramAddr = C_POST_STEEP_START_OFFSET + (0x2000*B_SelectedProfile)+ C_ONE_CHANNEL_GAMMA_STEEP_SIZE; 
                  W_size = 128;
               }                        
               else if(DW_ComponentToApply & Gamma_PreSteep_Component)
               {
                  DW_LUTAddr = INPUT_GAMMA_G_STEEP_MEM_START;
				  DW_NvramAddr = C_PRE_STEEP_START_OFFSET + (0x2000*B_SelectedProfile)+ C_ONE_CHANNEL_GAMMA_STEEP_SIZE; 
                  W_size = 128;
               }                        
               else
               {
                  //gm_Printf("Invalid Component G", 0);
                  B_Return = 3; // Component not available
                  DW_ComponentToApply = 0;
                  //break;// break the while loop
return B_Return;
               }                     
            }
            else // B
            {
               if(DW_ComponentToApply & Gamma_PostLUT_Component)
               {
                  DW_LUTAddr = OUTPUT_GAMMA_B_MEM_START;
				  DW_NvramAddr = C_POST_LUT_START_OFFSET + (0x2000*B_SelectedProfile)+ C_ONE_CHANNEL_GAMMA_SIZE*2; 
                  W_size = 1024;
                  //CLRBIT(DW_ComponentToApply, Gamma_PostLUT_Component);
               }
               else if(DW_ComponentToApply & Gamma_PreLUT_Component)
               {
                  DW_LUTAddr = INPUT_GAMMA_B_MEM_START;
				  DW_NvramAddr = C_PRE_LUT_START_OFFSET + (0x2000*B_SelectedProfile)+ C_ONE_CHANNEL_GAMMA_SIZE*2; 
                  W_size = 1024;
                  //CLRBIT(DW_ComponentToApply, Gamma_PreLUT_Component);
               }                        
               else if(DW_ComponentToApply & Gamma_PostSteep_Component)
               {
                  DW_LUTAddr = OUTPUT_GAMMA_B_STEEP_MEM_START;
				  DW_NvramAddr = C_POST_STEEP_START_OFFSET + (0x2000*B_SelectedProfile)+ C_ONE_CHANNEL_GAMMA_STEEP_SIZE*2; 
                  W_size = 128;
                  CLRBIT(DW_ComponentToApply, Gamma_PostSteep_Component);
               }                        
               else if(DW_ComponentToApply & Gamma_PreSteep_Component)
               {
                  DW_LUTAddr = INPUT_GAMMA_B_STEEP_MEM_START;
				  DW_NvramAddr = C_PRE_STEEP_START_OFFSET + (0x2000*B_SelectedProfile)+ C_ONE_CHANNEL_GAMMA_STEEP_SIZE*2; 
                  W_size = 128;
                  CLRBIT(DW_ComponentToApply, Gamma_PreSteep_Component);
               }                        
               else
               {
                  //gm_Printf("Invalid Component B", 0);
                  B_Return = 3; // Component not available
                  DW_ComponentToApply = 0;
                  //break; // break the while loop
return B_Return;
               }                     
            }

			 if(W_size == 128)
				 NVRam_ReadBuffer(DW_NvramAddr, Lut_Buf, 256);
			 else
				 NVRam_ReadBuffer(DW_NvramAddr, Lut_Buf, 1024);
			 
			 for(i = 0; i < W_size; i++)
			 {					   
				 if(W_size == 128)
				 {							 
					 *LINEAR_TO_FPTR_W(DW_LUTAddr) = Lut_Buf[i*2]|Lut_Buf[i*2+1]<<8 ;
					 DW_LUTAddr += 4;
					 //DW_NvramAddr += 2;
				 }
				 else
				 {							 
					 *LINEAR_TO_FPTR_B(DW_LUTAddr) = Lut_Buf[i];
					 DW_LUTAddr += 1;
					 //DW_NvramAddr += 1;
				 }																		 
			 }
         }

         if(DW_ComponentToApply & Gamma_PostLUT_Component)
         {
            //gm_Printf(" Apply Post LUT Config Registers", 0);
            DW_NvramAddr = C_POST_LUT_CONFIG_REG_START_OFFSET+ (0x2000*B_Selected_Profile) ; // 
            gm_WriteRegWord(GAMMA_LUT2_CTRL, NVRam_ReadWORD(DW_NvramAddr));
               
            DW_NvramAddr += 2;
            DW_LUTAddr = GAMMA_LUT2_STEEP_STRT_R;            

            for(i = 0; i < 12; i++)
            {
               gm_WriteRegWord(DW_LUTAddr, NVRam_ReadWORD(DW_NvramAddr));
               DW_LUTAddr += 2;
               DW_NvramAddr += 2;            
            }
            CLRBIT(DW_ComponentToApply, Gamma_PostLUT_Component);
         }
         else if(DW_ComponentToApply & Gamma_PreLUT_Component)
         {

            //gm_Printf(" Apply Pre LUT Config Registers", 0);
            DW_NvramAddr = C_PRE_LUT_CONFIG_REG_START_OFFSET + (0x2000*B_Selected_Profile) ; // 2 sector for 1 color space
            gm_WriteRegWord(GAMMA_LUT1_CTRL, NVRam_ReadWORD(DW_NvramAddr));
               
            DW_NvramAddr += 2;
            DW_LUTAddr = GAMMA_LUT1_STEEP_STRT_R;            

            for(i = 0; i < 12; i++)
            {
               gm_WriteRegWord(DW_LUTAddr, NVRam_ReadWORD(DW_NvramAddr));
               DW_LUTAddr += 2;
               DW_NvramAddr += 2;            
            }         
            CLRBIT(DW_ComponentToApply, Gamma_PreLUT_Component);
         }            
      }
      else if(DW_ComponentToApply & Gamma_Matrix_Component)
      {
         BYTE i;
         DW_LUTAddr = GAMMA_LUT_MATRIX_MULT_COEF11; // GAMMA_LUT1_CTRL;
         DW_NvramAddr = C_GAMMA_REGISTER_START_OFFSET+ (0x2000*B_Selected_Profile);

         //for(i = 0; i < 41; i++) // 41 registers
         for(i = 0; i < 15; i++) // 15 registers
         {                     
            gm_WriteRegWord(DW_LUTAddr, NVRam_ReadWORD(DW_NvramAddr));
            DW_LUTAddr += 2;
            DW_NvramAddr += 2;
            
         }
         CLRBIT(DW_ComponentToApply, Gamma_Matrix_Component);
         if(gm_ReadRegWord(DISP_LUT_CONTROL) != 0x03)
         {
            gm_WriteRegWord(DISP_LUT_CONTROL, 0x3);
            //gm_SyncUpdate(gmvb_MainChannel, OUTPUT_CLOCK);
            gm_SetRegBitsWord(HOST_CONTROL, ODP_SYNC_UPDATE); // debugon can't use gm_SyncUpdate
            //gm_Printf("~~~ DISP_LUT_CONTROL ~~~", 0);
         }
      }
      else
      {
         //gm_Printf(" Invalid Input Parameter ", 0);
         B_Return = 3; // Component not available
         DW_ComponentToApply = 0;
         //break; // break the while loop
return B_Return;
      }
   }
#else
	UNUSED_VAR(DW_ComponentToApply);
	UNUSED_VAR(B_Selected_Profile);
    B_Return = 3; // Component not available
#endif//end if SAVING_DATA_INTO_NVRAM  , SAVING_DATA_INTO_FLASH

   return B_Return;
}

BYTE Gamma_Erase_Profile(BYTE B_ProfileIDtoErase)
{            
   BYTE B_Return = 0;
   WORD DW_NvramAddr;
   
   // Profile ID
   //msg(" Profile ID = %d",Bp_msg[1]);
   //B_ProfileIDtoErase = Bp_msg[1] - 1; // Tool index range is 1 ~ 255
   msg(" Profile ID = %d", B_ProfileIDtoErase);

#ifdef SAVING_DATA_INTO_FLASH
   if(B_ProfileIDtoErase < Gamma_MaxProfile)
   { // Clear Index All components ...
      DWORD DW_FlashAddr = 0x5FA000; // 0x1FB000

      DW_FlashAddr = DW_FlashAddr - (0x2000*B_ProfileIDtoErase); // 2 sector for 1 color space

      Flash_EraseSector(DW_FlashAddr, 2, SECT_ER_DEST_INCLUDED);
   }
   else
   {
      //gm_Printf("Invalid Profile ID to erase", 0);
      B_Return = 1; // Invalid Profile ID
   }         
   
#elif defined(SAVING_DATA_INTO_NVRAM)   
	if(B_ProfileIDtoErase < Gamma_MaxProfile)
	{ // Clear Index All components ...

		DW_NvramAddr=  C_GAMMA_COMPONENT_SAVED_START_OFFSET+(0x2000*B_ProfileIDtoErase) ;
			
	   	NVRam_WriteByte(DW_NvramAddr,0xFF);
	   	NVRam_WriteByte(DW_NvramAddr+1,0xFF);
	   	NVRam_WriteByte(DW_NvramAddr+2,0xFF);
   		NVRam_WriteByte(DW_NvramAddr+3,0xFF);
	}
	else
	{
	   //gm_Printf("Invalid Profile ID to erase", 0);
	   B_Return = 1; // Invalid Profile ID
	}			 

#else
   	B_Return = 3; // Component not available
#endif

   return B_Return;
}
   
#pragma argsused
BYTE GammaCorr_MsgHandler(MsgPacket_t *message, BYTE comm_use)
{
   BYTE far * Bp_msg = (BYTE far *)message->msgpkt;
   GammaCorr_CommandType B_Command;
   GammaContextType far * p_Context;

   BYTE far * Bp_Len = GetOutMsgLenPtr();
   BYTE far * Bp_Reply =  GetOutMsgBufPtr();
   union
   {
      WORD  w;
      BYTE  b[2];
   } WordVal;

   // get buffer to store data beetwen different messages
   // responds 0 if new buffer is allocated,  responds 1 if buffer is already in use or 0xff if error
   BYTE B_MemAllocResult = GetBuffer((BYTE far * far *)&p_Context, sizeof(GammaContextType));

	PortMsgHndlrReplyType ErrorCode = PORT_MSG_HNDLR_OK;

   if (B_MemAllocResult == 0 )
   {// buffer is just allocated; initialize the context

      if (p_Context == NULL_PTR)
         return PORT_MSG_HNDLR_ERROR;

      // buffer initialisation
      _fmemset (p_Context,0,sizeof(GammaContextType));
      p_Context->FirstMessage = 1;

      // get command id from input buffer
      B_Command = (GammaCorr_CommandType)Bp_msg[0];

      p_Context->CurrentCommand = B_Command;
   }
   else if (B_MemAllocResult == 1 )
   {// buffer is already in use. Data from previous message is stored in the buffer
      //use command ID from context if is not the first message in multy message transport transaction
      msg("",0);
      B_Command = (GammaCorr_CommandType)p_Context->CurrentCommand;
      p_Context->FirstMessage = 0;
   }
   else // memory is not allocated(B_MemAllocResult == 0xff)
   {
      // handle memory allocation error
      msg("GAMMA: ERROR memalloc 1",0);
      return PORT_MSG_HNDLR_ERROR;
   }

   if ((Bp_Len == NULL_PTR) || (Bp_msg == NULL_PTR))
   {
      msg("GAMMA: ERROR memalloc 2",0);
      return (BYTE)PORT_MSG_HNDLR_ERROR;
   }

   Bp_Reply[0] = Gamma_OK;
   *Bp_Len = 1; //output message length

   //parse command
   switch (B_Command)
   {
      case Gamma_StartCalibration:
         #ifdef ST_4K2K_93xx_BOARD
         if(GetChipID_4K2K()==CHIPID_4K2K_R)
    		{
           	SMT_SendTimeoutTimetoHost(message, comm_use, SMT_TRANSACTION_TIMEOUT);
        	   ErrorCode = SMT_ForwardMessageToSlave(message, SOCIPC_DEVICEL_DST_ADDRESS);

            // SMT_SendTimeoutTimetoHost will change the Outmsg buffer so need to set again ...
            *Bp_Len = 1; //output message length
        	}
			if(ErrorCode == PORT_MSG_HNDLR_OK)
    		#endif
			{
         // set DebugOn mode
         gmvb_DebugOnly = TRUE;

         // set handler version number to responce
         Bp_Reply[0] = GAMMA_SMT_PROTOCOL_VER;//gamma v.4

         // Bypass ODP LUT and Matrix
         gm_WriteRegWord(DISP_LUT_CONTROL, 0); // Bypass VLUT
         gm_WriteRegWord(GAMMA_LUT1_CTRL, 0); // Bypass LUT1
         gm_WriteRegWord(GAMMA_LUT2_CTRL, 0); // Bypass LUT2 and Matrix

         // Disable DUM
         gm_ClearRegBitsWord(C9VODP_DUM_CONTROL_0, DUM_ENABLE);

         // Disable ACC/ACM and make color temp as 6500 which should be panel native.
	#ifdef DELL_U2410F
         #if COLOR_CALI_FORCE_6500
		   //if(!IsPresetMode_GRAPHICS_STANDARD)
         {
            UserPrefGraphicsImageMode = Graphics_STANDARD;
            UserPrefPresetColorTemp = CTEMP_6500K;
            B_tempRGain = UserPrefColorT6500_R;
            B_tempGGain = UserPrefColorT6500_G;
            B_tempBGain = UserPrefColorT6500_B;
            UserPrefColorT6500_R = 255;//0x100;
            UserPrefColorT6500_G = 255;//0x100;
            UserPrefColorT6500_B = 255;//0x100;
         #else
		 if(!IsPresetMode_GRAPHICS_STANDARD)
         {
            UserPrefGraphicsImageMode = Graphics_STANDARD;
         #endif
            AdjustPresetModes();
         }
	#else
		 if((UserPrefImageScheme != USER) || (UserPrefColorTemp != CTEMP_6500K))
         {
            UserPrefColorTemp = CTEMP_6500K;
            UserPrefImageScheme = USER;
            AdjustImageScheme();
         }
	#endif	 

         // Bypass ODP LUT and Matrix
         gm_WriteRegWord(DISP_LUT_CONTROL, 0); // Bypass VLUT
         gm_WriteRegWord(GAMMA_LUT1_CTRL, 0); // Bypass LUT1
         gm_WriteRegWord(GAMMA_LUT2_CTRL, 0); // Bypass LUT2 and Matrix

         // Disable DUM
         gm_ClearRegBitsWord(C9VODP_DUM_CONTROL_0, DUM_ENABLE);

         gm_SetRegBitsWord(HOST_CONTROL,ODP_SYNC_UPDATE);

         msg("Gamma_StartCalibration",0);

    			#if (GAMMA_SMT_PROTOCOL_VER >= VERSION_6)
            SetSMTICToolVersion(Bp_msg[1]);
            if(GetSMTICToolVersion() >= VERSION_6)
            {
					SetSMTICStarted(TRUE);
					//backup ovp 3x3 matrix if B_SMTICVersion >= VERSION_6
					BackupOVP3x3Matrix(W_OVP3x3MatrixSaved);
				   #ifdef IC2_DEBUG_PRINT
               gm_Print("saved 3x3",0);
               gm_Print("0x%x 0x%x 0x%x", W_OVP3x3MatrixSaved[0],W_OVP3x3MatrixSaved[1],W_OVP3x3MatrixSaved[2]);
               gm_Print("0x%x 0x%x 0x%x", W_OVP3x3MatrixSaved[3],W_OVP3x3MatrixSaved[4],W_OVP3x3MatrixSaved[5]); 
               gm_Print("0x%x 0x%x 0x%x", W_OVP3x3MatrixSaved[6],W_OVP3x3MatrixSaved[7],W_OVP3x3MatrixSaved[8]); 
               gm_Print("0x%x 0x%x 0x%x", W_OVP3x3MatrixSaved[9],W_OVP3x3MatrixSaved[10],W_OVP3x3MatrixSaved[11]);        
				   #endif
            }
    			#endif
			}
         break;

      case Gamma_EndCalibration:

         #ifdef ST_4K2K_93xx_BOARD
         if(GetChipID_4K2K()==CHIPID_4K2K_R)
    		{
           	SMT_SendTimeoutTimetoHost(message, comm_use, SMT_TRANSACTION_TIMEOUT);
        	   ErrorCode = SMT_ForwardMessageToSlave(message, SOCIPC_DEVICEL_DST_ADDRESS);
        	}
			if(ErrorCode == PORT_MSG_HNDLR_OK)
    		#endif
			{
         //restore default gamma table and color settings
    			#if (GAMMA_SMT_PROTOCOL_VER >= VERSION_6)
         	if(GetSMTICToolVersion() < VERSION_6)
    			#endif
         gm_WriteRegWord(IMP_PATGEN_CONTROL, 0); // disable pattern generator ..
         
#ifdef DELL_U2410F		//yvonne 20120920
         #if COLOR_CALI_FORCE_6500
         UserPrefColorT6500_R = B_tempRGain;
         UserPrefColorT6500_G = B_tempGGain;
         UserPrefColorT6500_B = B_tempBGain;
         #endif

			AdjustPresetModes();
			ApplyDUMTable();
#else
         AdjustGammaCorrection();
#endif

         // set DebugOff mode
         gmvb_DebugOnly = FALSE;

         // no data to responce
         *Bp_Len = 0;

    			#if (GAMMA_SMT_PROTOCOL_VER >= VERSION_6)
            if((GetSMTICToolVersion() >= VERSION_6) && (GetSMTICStarted() == FALSE))
					ErrorCode = PORT_MSG_HNDLR_ERROR;
            else
					if(GetSMTICToolVersion() >= VERSION_6)
               {
						SetSMTICStarted(FALSE);
						//restore ovp 3x3 matrix if B_SMTICVersion >= VERSION_6
						ProgramOVP3x3MatrixReg(W_OVP3x3MatrixSaved);
						gm_SetRegBitsWord(OVP_UPDATE_CTRL, OVP_SYNC_UPDATE); // debugon can't use gm_SyncUpdate
					}
    			#endif

         msg("Gamma_EndCalibration",0)    ;
			}
         break;

      case Gamma_GetCapabilities:
         {
            GammaCapabilitiesType * GammaCapabilities = (GammaCapabilitiesType *)Bp_Reply;

            //Panel color depth
            GammaCapabilities->PanelRedWidth = CurrentPanelData->B_Depth;//r;
            GammaCapabilities->PanelGreenWidth = CurrentPanelData->B_Depth;//g;
            GammaCapabilities->PanelBlueWidth = CurrentPanelData->B_Depth;//b;

            //Panel shades number;
            GammaCapabilities->PanelRedShades = NUMBER_OF_SHADES;
            GammaCapabilities->PanelGreenShades = NUMBER_OF_SHADES;
            GammaCapabilities->PanelBlueShades = NUMBER_OF_SHADES;
            GammaCapabilities->PanelGreyShades = NUMBER_OF_SHADES;

            //color space, supported by chip: bit0 - RGB; bit1 - YUV; bit2 YPbPr ...
            GammaCapabilities->ColorSpace = BIT0;

            //Width of the data path
            GammaCapabilities->GDataPathWidth = FEATURE_VLUT_SIZE;
            GammaCapabilities->BDataPathWidth = FEATURE_VLUT_SIZE;
            GammaCapabilities->RDataPathWidth = FEATURE_VLUT_SIZE;

            //actual length of data buffer
            *Bp_Len = sizeof(GammaCapabilitiesType);
            msg("Gamma_GetCapabilities",0);

            break;
         }
      case Gamma_GetColorSpaceCapabilities:
         {
            ColorSpaceCapabilitiesType * ColorSpaceCapabilities = (ColorSpaceCapabilitiesType *)Bp_Reply;

            if ((Bp_msg[1]!=Gamma_Space_RGB)     //color space ruquest;
                  ||(Bp_msg[2]==0))               //Monochrome color component request.
            {
               ColorSpaceCapabilities->SupportLevel = Gamma_CompensationNotSupported;//ERROR: Color Space not supported
               *Bp_Len = 1;
            }
            else
            {
               //color space capabilities of gamma table are the same for all supported cases.
               ColorSpaceCapabilities->SupportLevel = Gamma_SeparateTables;//
               ColorSpaceCapabilities->GammaTableEntries = NUMBER_OF_ELEMENTS_IN_GAMMA_LUT;
               ColorSpaceCapabilities->GammaTableWidth = GAMMA_TABLE_WIDTH;
               ColorSpaceCapabilities->EntryOffset = 0;
               *Bp_Len = sizeof(ColorSpaceCapabilitiesType);
            }
            msg("Gamma_GetColorSpaceCapabilities",0);
            break;
         }

         //show a colored rectangle with received color at the center of the screen
         //How to do: Enable window at the center of the screen. Program 0 to all coefficiients of 3x3 matrix #B to get
         //black color (0,0,0) output; Program necessery color to #0 element of gama table.
      case Gamma_DisplayColor:
         #ifdef ST_4K2K_93xx_BOARD
         if(GetChipID_4K2K()==CHIPID_4K2K_R)
    		{
        		ErrorCode = SMT_ForwardMessageToSlave(message, SOCIPC_DEVICEL_DST_ADDRESS);
        	}

			if(ErrorCode == PORT_MSG_HNDLR_OK)
    		#endif
         {
            WORD W_Ad;

            // check color space is supported
            if (Bp_msg[1] != Gamma_Space_RGB)
            {
               Bp_Reply[0] = Gamma_ColorSpaceNotSupported;
               break;
            }

				// use imp pattern generator
    			#if (GAMMA_SMT_PROTOCOL_VER >= VERSION_6)
            if(GetSMTICToolVersion() < VERSION_6)
    			#endif
            {
            	gm_WriteRegByte(IMP_PATGEN_CONTROL, 0xE0);	// Enable IMP_PAT with 8-bit
            }

            for (W_Ad = RED_VLUT_COMPONENT; W_Ad<3;W_Ad++)
            {
               WORD W_Convert ;
               WordVal.b[0] = Bp_msg[2+W_Ad*2];
               WordVal.b[1] = Bp_msg[3+W_Ad*2];

					#if (GAMMA_SMT_PROTOCOL_VER >= VERSION_6)
					if((GetSMTICToolVersion() >= VERSION_6) && (GetSMTICStarted() == FALSE))
						ErrorCode = PORT_MSG_HNDLR_ERROR;
					else
						if(GetSMTICToolVersion() >= VERSION_6)
						{
							gm_WriteRegWord(MAIN_P_MULT_COEF11, 0);
							gm_WriteRegWord(MAIN_P_MULT_COEF12, 0);
							gm_WriteRegWord(MAIN_P_MULT_COEF13, 0);
							gm_WriteRegWord(MAIN_P_MULT_COEF21, 0);
							gm_WriteRegWord(MAIN_P_MULT_COEF22, 0);
							gm_WriteRegWord(MAIN_P_MULT_COEF23, 0);
							gm_WriteRegWord(MAIN_P_MULT_COEF31, 0);
							gm_WriteRegWord(MAIN_P_MULT_COEF32, 0);
							gm_WriteRegWord(MAIN_P_MULT_COEF33, 0);
							//display 14-bit color, color value from IC is 16-bit
							//offset1 - green   offset2 - blue  offset3 - red
							W_Convert  = ((WordVal.w) >> 2) & 0x3FFF;

							#ifdef IC2_DEBUG_PRINT
							if (W_Ad == RED_VLUT_COMPONENT)
							{
								gm_Print("DisplayColor : Red 0x%x", W_Convert);                         
							}
							else if (W_Ad == GREEN_VLUT_COMPONENT)
							{
								gm_Print("DisplayColor : Green 0x%x", W_Convert);                         
							}
							else if (W_Ad == BLUE_VLUT_COMPONENT)
							{
								gm_Print("DisplayColor : Blue 0x%x", W_Convert);                         
							}
							#endif

							if (W_Ad == RED_VLUT_COMPONENT)
							{
								gm_WriteRegWord(MAIN_P_OUT_OFFSET3, W_Convert & 0x7FFF);   //offset3
							}
							else if (W_Ad == GREEN_VLUT_COMPONENT)
							{
								gm_WriteRegWord(MAIN_P_OUT_OFFSET1, W_Convert & 0x7FFF);   //offset1
							}
							else if (W_Ad == BLUE_VLUT_COMPONENT)
							{
								gm_WriteRegWord(MAIN_P_OUT_OFFSET2, W_Convert & 0x7FFF);   //offset2
							}

							gm_SetRegBitsWord(OVP_UPDATE_CTRL, OVP_SYNC_UPDATE); // debugon can't use gm_SyncUpdate
						} // if(GetSMTICToolVersion() >= VERSION_6)
					else
					#endif
					{
                  W_Convert  = (WordVal.w) >> 8;
   
                  if (W_Ad == RED_VLUT_COMPONENT)
                     gm_WriteRegByte(IMP_PATGEN_RED, W_Convert);
                  else if (W_Ad == GREEN_VLUT_COMPONENT)
                     gm_WriteRegByte(IMP_PATGEN_GRN, W_Convert);
                  else if (W_Ad == BLUE_VLUT_COMPONENT)
                     gm_WriteRegByte(IMP_PATGEN_BLUE, W_Convert);
            	}
            } // for(W_Ad = RED_VLUT_COMPONENT; W_Ad<3;W_Ad++)

            //5 Activate rectangle
            //gm_SetRegBitsWord(MAIN_MULT_CTRL, BIT0|BIT1);      //enable 3x3 matrix;
            //gm_SetRegBitsWord(DISP_LUT_CONTROL, BIT0 );         //enable gamma for video window only;
            //gm_SetRegBitsWord(VIDEO_WIN_CTRL, VIDEO_WIN_EN );   //enable video window
            //gm_WriteRegWord(HOST_CONTROL,0xff);

            // 6  generate responce
            Bp_Reply[0] = Gamma_OK;
            *Bp_Len = 1;

            msg("Gamma_DisplayColor",0);
         } // if(ErrorCode == PORT_MSG_HNDLR_OK)
			break;

      #if 0 // // not supported for Athena
      case Gamma_UploadPreLUT://gamma v.2
      case Gamma_UploadGammaTable:
         {
            BYTE far * pTransferTable= Bp_msg;
            WORD W_Size = GetMesageLength();
            WORD W_Index;
            BYTE * Bp_TempTable = TempBuffer;

            msg("Gamma_UploadGammaTable",0);

            Bp_Reply [0] = Gamma_OK;
            *Bp_Len = 1;

            if (p_Context->FirstMessage)
            {//Handle the first message in the sequense

               //Communication buffer size is not enough for this handler.
               if (W_Size < 4)
               {
                  msg("buffer size is not enough = %d",W_Size);
                  return PORT_MSG_HNDLR_ERROR;
               }
               //Check if supported RGB color space is used;
               if (Bp_msg[1] != Gamma_Space_RGB)
               {
                  Bp_Reply[0] = Gamma_ColorSpaceNotSupported;
                  return (BYTE)PORT_MSG_HNDLR_OK;
               }

               p_Context->AllTables = 0;
               //Check if supported color components are received and convert them to gmt_VLUT_COLOR_COMPONENT type;
               switch ((GamaColorComponentType)Bp_msg[2])
               {
                  case   Gamma_G_Component:
                     p_Context->ColorCmpnt = GREEN_VLUT_COMPONENT;
                     break;
                  case   Gamma_B_Component:
                     p_Context->ColorCmpnt = BLUE_VLUT_COMPONENT;
                     break;
                  case   Gamma_R_Component:
                     p_Context->ColorCmpnt = RED_VLUT_COMPONENT;
                     break;
                  case   Gamma_RGB_Component://all three tables are coming one by one. The order is R-G-B
                     p_Context->AllTables = 1;
                     p_Context->ColorCmpnt = RED_VLUT_COMPONENT;
                     break;
                  default:
                     Bp_Reply[0] = Gamma_FormatNotSupported;
                     return PORT_MSG_HNDLR_OK;
               }
               pTransferTable += 3;
               W_Size -= 3;
            }

            //cut off data if it is too long for gamma table
            //Cutting off value depends on color component
            {
               WORD W_CutOffValue =sizeof(WORD)*((p_Context->AllTables) ? NUMBER_OF_ELEMENTS_IN_GAMMA_LUT*3 : NUMBER_OF_ELEMENTS_IN_GAMMA_LUT);
               if ((W_Size + sizeof(WORD)*p_Context->Shift+p_Context->StoredFlag) > W_CutOffValue)
               {
                  W_Size = W_CutOffValue -p_Context->Shift;
                  Bp_Reply[0] = Gamma_FormatNotSupported;
               }
            }

            //copy input data bufer with taken in mind stored previous data - part of uncompleted gamma table component (low byte of a WORD)
            if (p_Context->StoredFlag)
            {
               Bp_TempTable[0] = p_Context->DataStored;
            }

            //take data from input buffer
            memcpy(&Bp_TempTable[p_Context->StoredFlag], (BYTE*)pTransferTable, W_Size);

            //modify Size value if data byte was stored
            W_Size += p_Context->StoredFlag;

            for (W_Index = 0; W_Index < W_Size; W_Index += sizeof(WORD))
            {
               //truncate less significant bits
               WordVal.b[0] = Bp_TempTable[W_Index];
               WordVal.b[1] = Bp_TempTable[W_Index+1];

               WordVal.w = WordVal.w >> (16-GAMMA_TABLE_WIDTH);

               Bp_TempTable[W_Index] = WordVal.b[0];
               Bp_TempTable[W_Index + 1] = WordVal.b[1];

            }


            //Load gamatable data.
            if (p_Context->AllTables)
            {
               // if RGB_Component calculate correct shift and color component ID
               if ((W_Size + sizeof(WORD)*p_Context->Shift) >= sizeof(WORD)*NUMBER_OF_ELEMENTS_IN_GAMMA_LUT)
               {
                  //Write the last part of current component data to the VLUT;
                  //Change Color Component ID
                  //Set shift and size for the rest part of recevied table.
                  WORD W_LastPart = sizeof(WORD)*NUMBER_OF_ELEMENTS_IN_GAMMA_LUT -sizeof(WORD)*p_Context->Shift;
                  msg("W_Size = %d (last part)",W_LastPart);
                  msg("p_Context->Shift = %d",p_Context->Shift);
                  msg("p_Context->ColorCmpnt = %d",p_Context->ColorCmpnt);

                  if (B_Command == Gamma_UploadPreLUT)//gamma v.2
                     gm_LoadImpVlutTable(p_Context->ColorCmpnt, p_Context->Shift, W_LastPart/2, (WORD far*)Bp_TempTable, 1);
                  else
                     gm_LoadVlutTable(p_Context->ColorCmpnt, p_Context->Shift, W_LastPart/2, (WORD far*)Bp_TempTable, 1);

                  if (p_Context->ColorCmpnt != BLUE_VLUT_COMPONENT)
                     p_Context->ColorCmpnt++;
                  p_Context->Shift = 0;
                  W_Size -= W_LastPart;
                  Bp_TempTable += W_LastPart;
               }
            }

            msg("W_Size = %d",W_Size);
            msg("p_Context->Shift = %d",p_Context->Shift);
            msg("p_Context->ColorCmpnt = %d",p_Context->ColorCmpnt);
            if (W_Size)
            {
               if (B_Command ==Gamma_UploadPreLUT)//gamma v.2
                  gm_LoadImpVlutTable(p_Context->ColorCmpnt, p_Context->Shift, W_Size/2, (WORD far*)Bp_TempTable, 1);
               else
                  gm_LoadVlutTable(p_Context->ColorCmpnt, p_Context->Shift, W_Size/2, (WORD far*)Bp_TempTable, 1);
            }

            p_Context->Shift += W_Size/sizeof(WORD);

            //store the last byte for next message if data size is odd
            if (W_Size%sizeof(WORD))
            {
               p_Context->StoredFlag= 1;
            p_Context->DataStored = Bp_TempTable[W_Size-1];
            }
            else
               p_Context->StoredFlag= 0;

            // turn on gamma correction if it is the last message
            if (!MoreDataExpected())
            {
               if (B_Command ==Gamma_UploadPreLUT)//gamma v.2
               {
                  //gm_ClearAndSetBitsWord(GAMMA_LUT1_CTL, GAMMA_LUT1_CTL, GAMMA_LUT1_CTL);
                  gm_SetRegBitsWord(GAMMA_LUT1_CTRL, GAMMA_LUT1_CTL);
               }
               else
               {
                  gm_SetRegBitsWord(GAMMA_LUT2_CTRL, GAMMA_LUT2_CTL);
               }
               gm_SetRegBitsWord(HOST_CONTROL,ODP_SYNC_UPDATE);
            }
            break;
         }
      #endif

      #if 0 // not supported for Athena
      case Gamma_DownloadPreLUT://gamma v.2
      case Gamma_DownloadGammaTable:

         *Bp_Len = 1;
         
         msg("Gamma_DownloadGammaTable %d",p_Context->Shift);

         //check if gamma table is used
         if ((((gm_ReadRegWord(GAMMA_LUT2_CTRL) & GAMMA_LUT2_CTL) == 0) && (B_Command == Gamma_DownloadGammaTable))
               || ((((gm_ReadRegWord(GAMMA_LUT1_CTRL) & GAMMA_LUT1_CTL) >> 4) == 0) && (B_Command == Gamma_DownloadPreLUT))
            )
         {
            *Bp_Reply = Gamma_TableIsEmpty;
         }

         else
         {
            WORD W_Index;
            WORD far * Wp_VLUT = (WORD *)TempBuffer;

            //get size for output buffer and make it even
            WORD W_Size = 0xfffe & GetMaxDataLength();
            *Bp_Reply = Gamma_OK;

            //Check if it the first entry
            if (p_Context->FirstMessage)
            {


               //Check if supported RGB color space is requested;
               if (Bp_msg[1] != Gamma_Space_RGB)
               {
                  *Bp_Reply = Gamma_ColorSpaceNotSupported;
                  break;
               }

               //Check if supported color components are received and convert them to gmt_VLUT_COLOR_COMPONENT type;
               p_Context->AllTables = 0;
               switch ((GamaColorComponentType)Bp_msg[2])
               {
                  case   Gamma_G_Component:
                     p_Context->ColorCmpnt = GREEN_VLUT_COMPONENT;
                     break;
                  case   Gamma_B_Component:
                     p_Context->ColorCmpnt = BLUE_VLUT_COMPONENT;
                     break;
                  case   Gamma_R_Component:
                     p_Context->ColorCmpnt = RED_VLUT_COMPONENT;
                     break;
                  case   Gamma_RGB_Component:
                     p_Context->ColorCmpnt = RED_VLUT_COMPONENT;
                     p_Context->AllTables = 1;
                     break;
                  default:
                     Bp_Reply[0] = Gamma_FormatNotSupported;
                     return PORT_MSG_HNDLR_OK;
               }

               Bp_Reply++;
            }

            //calculate number of gamma table elements to be send during this transfer
            if ((NUMBER_OF_ELEMENTS_IN_GAMMA_LUT*sizeof(WORD) -p_Context->Shift) <= (W_Size-2))
            {
               //prepare for the last data transfer message
               W_Size = NUMBER_OF_ELEMENTS_IN_GAMMA_LUT*sizeof(WORD) -p_Context->Shift;
            }
            else
            {
               msg("Gamma: More Data",0);
               MoreData(TRUE);
               W_Size -= 2;
            }

            //read Gamma table block
            {
               WORD W_Shift = p_Context->Shift;

               //if(B_Command ==Gamma_DownloadGammaTable)
               //    W_Shift += NUMBER_OF_ELEMENTS_IN_GAMMA_LUT*sizeof(WORD)*p_Context->ColorCmpnt;

               msgd("Gamma: W_Shift = %d (0x%x)",W_Shift,W_Shift);
               msgd("Gamma: W_Size = %d (0x%x)", W_Size, W_Size);
               {
                  DWORD DW_Addr;

                  //Choose proper LUT addr
                  if (B_Command ==Gamma_DownloadPreLUT)//gamma v.2
                  {
                     DW_Addr =
                        (p_Context->ColorCmpnt == GREEN_VLUT_COMPONENT) ? INPUT_GAMMA_G_MEM_START :
                        (p_Context->ColorCmpnt == BLUE_VLUT_COMPONENT) ? INPUT_GAMMA_B_MEM_START:
                        INPUT_GAMMA_R_MEM_START;
                  }
                  else // Post LUT
                  {
                     DW_Addr =
                        (p_Context->ColorCmpnt == GREEN_VLUT_COMPONENT) ? OUTPUT_GAMMA_G_MEM_START :
                        (p_Context->ColorCmpnt == BLUE_VLUT_COMPONENT) ? OUTPUT_GAMMA_B_MEM_START:
                        OUTPUT_GAMMA_R_MEM_START;
                  }

                  //Read the block of table date to the buffer
                  {
                     WORD W_Index = 0;

                     for (; W_Index < W_Size; W_Index++)
                     {
                        DW_Addr = DW_Addr + W_Shift + W_Index;
                        Wp_VLUT[W_Index] = *LINEAR_TO_FPTR_W(DW_Addr);
                     }
                  }
               }
            }


            for (W_Index = 0 ; W_Index< W_Size ; W_Index+=sizeof(WORD))
            {
               //shift value left to get 16 bits scale.
               WordVal.w = (*Wp_VLUT++)<<(16-GAMMA_TABLE_WIDTH);
               //fill less significant bits by "1"
               WordVal.w |= (0x1 << (16-GAMMA_TABLE_WIDTH))-1;

               *Bp_Reply++ = WordVal.b[0];
               *Bp_Reply++ = WordVal.b[1];
            }

            //store counter of transmitted gamma table elements or clear it if gamma table transmitting is completed;
            //prepare context for next message or inform SMT handler that transmitting is completed;
            if ((NUMBER_OF_ELEMENTS_IN_GAMMA_LUT*sizeof(WORD) -p_Context->Shift) == W_Size)
            {
               p_Context->Shift = 0;
               MoreData(FALSE);
               if ((p_Context->AllTables) && (p_Context->ColorCmpnt != BLUE_VLUT_COMPONENT))
               {
                  p_Context->ColorCmpnt++;
                  MoreData(TRUE);
                  msg("Gamma: More Data",0);
               }
            }
            else
               p_Context->Shift += W_Size;

            *Bp_Len = W_Size + p_Context->FirstMessage;

            msg(" .....done",0);
         }
         // */
         break;
         #endif

      case Gamma_GetSupportedTables:
         {
            GammaGetSupportTablesType * GammaSupportTables = (GammaGetSupportTablesType *)Bp_Reply;
            GammaSupportTables->SupportTablesCapabilities = SMTCapabilities_PostLut_Support | SMTCapabilities_PreLut_Support;
            GammaSupportTables->SupportTablesCapabilities |= SMTCapabilities_3x3Matrix;
            GammaSupportTables->SupportTablesCapabilities |= SMTCapabilities_Compressed_PostLUT|SMTCapabilities_Compressed_PreLUT|SMTCapabilities_PostLUT_Steep|SMTCapabilities_PreLUT_Steep;
            GammaSupportTables->SupportTablesCapabilities |= SMTCapabilities_Load_Clear_Save; // version 5

				#if (GAMMA_SMT_PROTOCOL_VER >= VERSION_6)
				if((GetSMTICToolVersion() >= VERSION_6) && (GetSMTICStarted() == FALSE))
					ErrorCode = PORT_MSG_HNDLR_ERROR;
				else
					if(GetSMTICToolVersion() >= VERSION_6)
						GammaSupportTables->SupportTablesCapabilities |= SMTCapabilities_NativeMatrixDataFormat; //version 6
				#endif

            *Bp_Len = sizeof(GammaGetSupportTablesType);
         }
         break;

      case Gamma_GetPreLUTColorSpaceCapabilities:
         {
            gmt_SMTGetCapabilities_Req * pReq = (gmt_SMTGetCapabilities_Req *)Bp_msg;
            ColorSpaceCapabilitiesType * ColorSpaceCapabilities = (ColorSpaceCapabilitiesType *)Bp_Reply;

            if (pReq->Color_Space != SMTColor_Space_RGB)
            {
               ColorSpaceCapabilities->SupportLevel = Gamma_CompensationNotSupported;
               *Bp_Len  = 1;
            }
            else if (pReq->Color_component == SMTColor_component_Monochrome)
            {
               ColorSpaceCapabilities->SupportLevel = Gamma_CompensationNotSupported;
               *Bp_Len  = 1;
            }
            else
            {
               ColorSpaceCapabilities->SupportLevel = Gamma_SeparateTables;//
               ColorSpaceCapabilities->GammaTableEntries = NUMBER_OF_ELEMENTS_IN_GAMMA_LUT;
               ColorSpaceCapabilities->GammaTableWidth = GAMMA_TABLE_WIDTH;
               ColorSpaceCapabilities->EntryOffset = 0;
               *Bp_Len = sizeof(ColorSpaceCapabilitiesType);
            }

         }

         break;

      case Gamma_GetColorMappingGamutMatrixColorSpaceCapabilities:
         {
            gmt_SMTGetColorGamutMatrixCSC_Res * pRes =      (gmt_SMTGetColorGamutMatrixCSC_Res *)Bp_Reply;
            pRes->Support_level = 1;//Color Mapping Gamut Matrix is supported;

				#if (GAMMA_SMT_PROTOCOL_VER >= VERSION_6)
				if((GetSMTICToolVersion() >= VERSION_6) && (GetSMTICStarted() == FALSE))
					ErrorCode = PORT_MSG_HNDLR_ERROR;
				else if(GetSMTICToolVersion() >= VERSION_6)
				{
					pRes->Table_entry_size     = ATHENA_3X3_WIDTH_VERSION_6;
					pRes->Table_FractionalSize  = ATHENA_3X3_FRACTIONWIDTH_VERSION_6;
					pRes->Table_IntegralSize   = ATHENA_3X3_INTEGERWIDTH_VERSION_6;
				}
				else
				#endif
				{
            	pRes->Table_entry_size = ATHENA_3X3_WIDTH;
				}
            *Bp_Len = sizeof(gmt_SMTGetColorGamutMatrixCSC_Res);
         }
         break;

		#if (GAMMA_SMT_PROTOCOL_VER >= VERSION_6)
		case Gamma_DownloadNativeColorMappingGamutMatrix:
			{
				GammaDownloadColorMappingGamutMatrix_t * Responce_p = (GammaDownloadColorMappingGamutMatrix_t *)Bp_Reply;

            Responce_p->Status = 0;//OK
            // DE 3x3 RGB
            // Athena 3x3 GBR

            #ifdef IC2_DEBUG_PRINT
            gm_Print("download 3x3",0);
            #endif

            Responce_p->Coeff22 = (gm_ReadRegWord(GAMMA_LUT_MATRIX_MULT_COEF11) & ATHENA_3x3_COEF_MASK_BITS);
            Responce_p->Coeff23 = (gm_ReadRegWord(GAMMA_LUT_MATRIX_MULT_COEF12) & ATHENA_3x3_COEF_MASK_BITS); 
            Responce_p->Coeff21 = (gm_ReadRegWord(GAMMA_LUT_MATRIX_MULT_COEF13) & ATHENA_3x3_COEF_MASK_BITS); 
            Responce_p->Coeff32 = (gm_ReadRegWord(GAMMA_LUT_MATRIX_MULT_COEF21) & ATHENA_3x3_COEF_MASK_BITS); 
            Responce_p->Coeff33 = (gm_ReadRegWord(GAMMA_LUT_MATRIX_MULT_COEF22) & ATHENA_3x3_COEF_MASK_BITS); 
            Responce_p->Coeff31 = (gm_ReadRegWord(GAMMA_LUT_MATRIX_MULT_COEF23) & ATHENA_3x3_COEF_MASK_BITS); 
            Responce_p->Coeff12 = (gm_ReadRegWord(GAMMA_LUT_MATRIX_MULT_COEF31) & ATHENA_3x3_COEF_MASK_BITS);
            Responce_p->Coeff13 = (gm_ReadRegWord(GAMMA_LUT_MATRIX_MULT_COEF32) & ATHENA_3x3_COEF_MASK_BITS);
            Responce_p->Coeff11 = (gm_ReadRegWord(GAMMA_LUT_MATRIX_MULT_COEF33) & ATHENA_3x3_COEF_MASK_BITS);
            // In_Offset ??
            Responce_p->Offset2= (gm_ReadRegWord(GAMMA_LUT_MATRIX_OUT_OFFSET1) & ATHENA_3x3_OFFSET_MASK_BITS); 
            Responce_p->Offset3= (gm_ReadRegWord(GAMMA_LUT_MATRIX_OUT_OFFSET2) & ATHENA_3x3_OFFSET_MASK_BITS);
            Responce_p->Offset1= (gm_ReadRegWord(GAMMA_LUT_MATRIX_OUT_OFFSET3) & ATHENA_3x3_OFFSET_MASK_BITS);
            
            *Bp_Len  = sizeof(GammaDownloadColorMappingGamutMatrix_t);
        }
        break;
		#endif

      case Gamma_DownloadColorMappingGamutMatrix:
         {
            GammaDownloadColorMappingGamutMatrix_t * Responce_p = (GammaDownloadColorMappingGamutMatrix_t *)Bp_Reply;

            Responce_p->Status = 0;//OK

            // DE 3x3 RGB
            // Athena 3x3 GBR
            Responce_p->Coeff22 = (gm_ReadRegWord(GAMMA_LUT_MATRIX_MULT_COEF11)<<SHIFT_3X3); //  + ((1<<SHIFT_3X3)-1);
            Responce_p->Coeff23 = (gm_ReadRegWord(GAMMA_LUT_MATRIX_MULT_COEF12)<<SHIFT_3X3); //  + ((1<<SHIFT_3X3)-1);
            Responce_p->Coeff21 = (gm_ReadRegWord(GAMMA_LUT_MATRIX_MULT_COEF13)<<SHIFT_3X3); //  + ((1<<SHIFT_3X3)-1);
            Responce_p->Coeff32 = (gm_ReadRegWord(GAMMA_LUT_MATRIX_MULT_COEF21)<<SHIFT_3X3); //  + ((1<<SHIFT_3X3)-1);
            Responce_p->Coeff33 = (gm_ReadRegWord(GAMMA_LUT_MATRIX_MULT_COEF22)<<SHIFT_3X3); //  + ((1<<SHIFT_3X3)-1);
            Responce_p->Coeff31 = (gm_ReadRegWord(GAMMA_LUT_MATRIX_MULT_COEF23)<<SHIFT_3X3); //  + ((1<<SHIFT_3X3)-1);
            Responce_p->Coeff12 = (gm_ReadRegWord(GAMMA_LUT_MATRIX_MULT_COEF31)<<SHIFT_3X3); //  + ((1<<SHIFT_3X3)-1);
            Responce_p->Coeff13 = (gm_ReadRegWord(GAMMA_LUT_MATRIX_MULT_COEF32)<<SHIFT_3X3); //  + ((1<<SHIFT_3X3)-1);
            Responce_p->Coeff11 = (gm_ReadRegWord(GAMMA_LUT_MATRIX_MULT_COEF33)<<SHIFT_3X3); //  + ((1<<SHIFT_3X3)-1);
            // In_Offset ??
            Responce_p->Offset2= (gm_ReadRegWord(GAMMA_LUT_MATRIX_OUT_OFFSET1)<<SHIFT_3X3); //  + ((1<<SHIFT_3X3)-1);
            Responce_p->Offset3= (gm_ReadRegWord(GAMMA_LUT_MATRIX_OUT_OFFSET2)<<SHIFT_3X3); //  + ((1<<SHIFT_3X3)-1);
            Responce_p->Offset1= (gm_ReadRegWord(GAMMA_LUT_MATRIX_OUT_OFFSET3)<<SHIFT_3X3); //  + ((1<<SHIFT_3X3)-1);

            *Bp_Len  = sizeof(GammaDownloadColorMappingGamutMatrix_t);
         }
         break;

		#if (GAMMA_SMT_PROTOCOL_VER >= VERSION_6)
		case Gamma_UploadNativeColorMappingGamutMatrix:
			{
				#ifdef ST_4K2K_93xx_BOARD
				if(GetChipID_4K2K()==CHIPID_4K2K_R)
				{
					SMT_SendTimeoutTimetoHost(message, comm_use, SMT_TRANSACTION_TIMEOUT);
					ErrorCode = SMT_ForwardMessageToSlave(message, SOCIPC_DEVICEL_DST_ADDRESS);
				}

				if(ErrorCode == PORT_MSG_HNDLR_OK)
				#endif
				{
					GammaUploadColorMappingGamutMatrix_t * MatrixData = (GammaUploadColorMappingGamutMatrix_t *)Bp_msg;
					*Bp_Len  = 1;

					if (GetMesageLength() != sizeof(GammaUploadColorMappingGamutMatrix_t))
					{
						//gm_Printf("SMT GAMMA:: Wrong 3x3 matrix size recevied");
						Bp_Reply[0] = 2; // format not supported
					}
					else
					{
						DWORD DW_TempCoeff;

						// gm_Printf("Gamma_UploadColorMappingGamutMatrix", 0);

						// DE 3x3 RGB
						// Athena 3x3 GBR
						DW_TempCoeff = (DWORD)(MatrixData->Coeff22 & ATHENA_3x3_COEF_MASK_BITS); //total 14 bits
						gm_WriteRegWord(GAMMA_LUT_MATRIX_MULT_COEF11, (WORD)DW_TempCoeff);
						DW_TempCoeff = (DWORD)(MatrixData->Coeff23 & ATHENA_3x3_COEF_MASK_BITS);
						gm_WriteRegWord(GAMMA_LUT_MATRIX_MULT_COEF12, (WORD)DW_TempCoeff);
                  DW_TempCoeff = (DWORD)(MatrixData->Coeff21 & ATHENA_3x3_COEF_MASK_BITS);
                  gm_WriteRegWord(GAMMA_LUT_MATRIX_MULT_COEF13, (WORD)DW_TempCoeff);
                  DW_TempCoeff = (DWORD)(MatrixData->Coeff32 & ATHENA_3x3_COEF_MASK_BITS);
                  gm_WriteRegWord(GAMMA_LUT_MATRIX_MULT_COEF21, (WORD)DW_TempCoeff);
                  DW_TempCoeff = (DWORD)(MatrixData->Coeff33 & ATHENA_3x3_COEF_MASK_BITS);
                  gm_WriteRegWord(GAMMA_LUT_MATRIX_MULT_COEF22, (WORD)DW_TempCoeff);
                  DW_TempCoeff = (DWORD)(MatrixData->Coeff31 & ATHENA_3x3_COEF_MASK_BITS);
                  gm_WriteRegWord(GAMMA_LUT_MATRIX_MULT_COEF23, (WORD)DW_TempCoeff);
                  DW_TempCoeff = (DWORD)(MatrixData->Coeff12 & ATHENA_3x3_COEF_MASK_BITS);
                  gm_WriteRegWord(GAMMA_LUT_MATRIX_MULT_COEF31, (WORD)DW_TempCoeff);
                  DW_TempCoeff = (DWORD)(MatrixData->Coeff13 & ATHENA_3x3_COEF_MASK_BITS);
                  gm_WriteRegWord(GAMMA_LUT_MATRIX_MULT_COEF32, (WORD)DW_TempCoeff);
                  DW_TempCoeff = (DWORD)(MatrixData->Coeff11 & ATHENA_3x3_COEF_MASK_BITS);
                  gm_WriteRegWord(GAMMA_LUT_MATRIX_MULT_COEF33, (WORD)DW_TempCoeff);

                  // In_Offset // ?
                  gm_WriteRegWord(GAMMA_LUT_MATRIX_OUT_OFFSET1,   MatrixData->Offset2 & ATHENA_3x3_OFFSET_MASK_BITS);//total 15 bits
                  gm_WriteRegWord(GAMMA_LUT_MATRIX_OUT_OFFSET2,   MatrixData->Offset3 & ATHENA_3x3_OFFSET_MASK_BITS);
                  gm_WriteRegWord(GAMMA_LUT_MATRIX_OUT_OFFSET3,   MatrixData->Offset1 & ATHENA_3x3_OFFSET_MASK_BITS);

                  Bp_Reply[0] = 0;    // OK
               
						if(!MoreDataExpected())
						{
							if(gm_ReadRegWord(DISP_LUT_CONTROL) != 0x3)
								gm_WriteRegWord(DISP_LUT_CONTROL, 0x3);
                  
							gm_SetRegBitsWord(GAMMA_LUT2_CTRL, BIT12);

							//gm_SyncUpdate(gmvb_MainChannel, OUTPUT_CLOCK);
							gm_SetRegBitsWord(HOST_CONTROL, ODP_SYNC_UPDATE); // debugon can't use gm_SyncUpdate
						} // if(!MoreDataExpected())               
					} // if (GetMesageLength() != sizeof(GammaUploadColorMappingGamutMatrix_t))
          	} // if(ErrorCode == PORT_MSG_HNDLR_OK)
			}
			break;
		#endif

      case Gamma_UploadColorMappingGamutMatrix:
         #ifdef ST_4K2K_93xx_BOARD
         if(GetChipID_4K2K()==CHIPID_4K2K_R)
    		{
           	SMT_SendTimeoutTimetoHost(message, comm_use, SMT_TRANSACTION_TIMEOUT);
        	   ErrorCode = SMT_ForwardMessageToSlave(message, SOCIPC_DEVICEL_DST_ADDRESS);
        	}

			if(ErrorCode == PORT_MSG_HNDLR_OK)
    		#endif
         {
            GammaUploadColorMappingGamutMatrix_t * MatrixData = (GammaUploadColorMappingGamutMatrix_t *)Bp_msg;
            *Bp_Len  = 1;
            if (GetMesageLength() != sizeof(GammaUploadColorMappingGamutMatrix_t))
            {
               //gm_Printf("SMT GAMMA:: Wrong 3x3 matrix size recevied");
               Bp_Reply[0] = 2; // format not supported
            }
            else
            {
               DWORD DW_TempCoeff;

               // gm_Printf("Gamma_UploadColorMappingGamutMatrix", 0);

               // DE 3x3 RGB
               // Athena 3x3 GBR
               DW_TempCoeff = (DWORD)(MatrixData->Coeff22 >> SHIFT_3X3); //  * 0x100/2047;
               gm_WriteRegWord(GAMMA_LUT_MATRIX_MULT_COEF11, (WORD)DW_TempCoeff);// + ((MatrixData->Coeff11 & (1<<(SHIFT_3X3-1))) ? 1 : 0));
               DW_TempCoeff = (DWORD)(MatrixData->Coeff23 >> SHIFT_3X3); //  * 0x100/2047;
               gm_WriteRegWord(GAMMA_LUT_MATRIX_MULT_COEF12, (WORD)DW_TempCoeff);// + ((MatrixData->Coeff12 & (1<<(SHIFT_3X3-1))) ? 1 : 0));// MatrixData->Coeff12 >> SHIFT_3X3 + (MatrixData->Coeff12 & (1<<(SHIFT_3X3-1))) ? 1 : 0);
               DW_TempCoeff = (DWORD)(MatrixData->Coeff21 >> SHIFT_3X3); //  * 0x100/2047;
               gm_WriteRegWord(GAMMA_LUT_MATRIX_MULT_COEF13, (WORD)DW_TempCoeff);// + ((MatrixData->Coeff13 & (1<<(SHIFT_3X3-1))) ? 1 : 0));
               DW_TempCoeff = (DWORD)(MatrixData->Coeff32 >> SHIFT_3X3); //  * 0x100/2047;
               gm_WriteRegWord(GAMMA_LUT_MATRIX_MULT_COEF21, (WORD)DW_TempCoeff);// + ((MatrixData->Coeff21 & (1<<(SHIFT_3X3-1))) ? 1 : 0));
               DW_TempCoeff = (DWORD)(MatrixData->Coeff33 >> SHIFT_3X3); //  * 0x100/2047;
               gm_WriteRegWord(GAMMA_LUT_MATRIX_MULT_COEF22, (WORD)DW_TempCoeff);// + ((MatrixData->Coeff22 & (1<<(SHIFT_3X3-1))) ? 1 : 0));
               DW_TempCoeff = (DWORD)(MatrixData->Coeff31 >> SHIFT_3X3); //  * 0x100/2047;
               gm_WriteRegWord(GAMMA_LUT_MATRIX_MULT_COEF23, (WORD)DW_TempCoeff);// + ((MatrixData->Coeff23 & (1<<(SHIFT_3X3-1))) ? 1 : 0));
               DW_TempCoeff = (DWORD)(MatrixData->Coeff12 >> SHIFT_3X3); //  * 0x100/2047;
               gm_WriteRegWord(GAMMA_LUT_MATRIX_MULT_COEF31, (WORD)DW_TempCoeff);// + ((MatrixData->Coeff31 & (1<<(SHIFT_3X3-1))) ? 1 : 0));
               DW_TempCoeff = (DWORD)(MatrixData->Coeff13 >> SHIFT_3X3); //  * 0x100/2047;
               gm_WriteRegWord(GAMMA_LUT_MATRIX_MULT_COEF32, (WORD)DW_TempCoeff);// + ((MatrixData->Coeff32 & (1<<(SHIFT_3X3-1))) ? 1 : 0));
               DW_TempCoeff = (DWORD)(MatrixData->Coeff11 >> SHIFT_3X3); //  * 0x100/2047;
               gm_WriteRegWord(GAMMA_LUT_MATRIX_MULT_COEF33, (WORD)DW_TempCoeff);// + ((MatrixData->Coeff33 & (1<<(SHIFT_3X3-1))) ? 1 : 0));

               // In_Offset // ?
               gm_WriteRegWord(GAMMA_LUT_MATRIX_OUT_OFFSET1,       MatrixData->Offset2 >> SHIFT_3X3);// + ((MatrixData->Offset1 & (1<<(SHIFT_3X3-1))) ? 1 : 0));
               gm_WriteRegWord(GAMMA_LUT_MATRIX_OUT_OFFSET2,       MatrixData->Offset3 >> SHIFT_3X3);// + ((MatrixData->Offset2 & (1<<(SHIFT_3X3-1))) ? 1 : 0));
               gm_WriteRegWord(GAMMA_LUT_MATRIX_OUT_OFFSET3,       MatrixData->Offset1 >> SHIFT_3X3);// + ((MatrixData->Offset3 & (1<<(SHIFT_3X3-1))) ? 1 : 0));

               Bp_Reply[0] = 0;    // OK
               
               if(!MoreDataExpected())
               {
                  if(gm_ReadRegWord(DISP_LUT_CONTROL) != 0x3)
                     gm_WriteRegWord(DISP_LUT_CONTROL, 0x3);
                  
                  gm_SetRegBitsWord(GAMMA_LUT2_CTRL, BIT12);

                  //gm_SyncUpdate(gmvb_MainChannel, OUTPUT_CLOCK);
                  gm_SetRegBitsWord(HOST_CONTROL, ODP_SYNC_UPDATE); // debugon can't use gm_SyncUpdate
               }               
            }
         }
         break;

#if 1 // v3 and v4
      case Gamma_DownloadCompressedTable:
         {
            gmt_GammaUploadCoompressedTable_Req * pReq = (gmt_GammaUploadCoompressedTable_Req *)Bp_msg;
            WORD W_Index;
            WORD far * Wp_VLUT = (WORD *)TempBuffer;

            //get size for output buffer and make it even
            WORD W_Size = 0xfffe & GetMaxDataLength();
            //gm_Printf("W_Size %d", W_Size);

            *Bp_Reply = Gamma_OK;
            *Bp_Len = 1;
            msg("Gamma_DownloadCompressedTable %d",p_Context->Shift);

            //Check if it the first entry
            if (p_Context->FirstMessage)
            {
               //Check if supported RGB color space is requested;
               if (pReq->ColorSpace != Gamma_Space_RGB)
               {
                  *Bp_Reply = Gamma_ColorSpaceNotSupported;
                  break;
               }

               if (pReq->GammaTable < 2)
                  p_Context->ExtraData = pReq->GammaTable;
               else
               {
                  *Bp_Reply = Gamma_FormatNotSupported;
                  break;
               }

               // Increase address for status byte.
               Bp_Reply++;

               //Check if supported color components are received and convert them to gmt_VLUT_COLOR_COMPONENT type;
               p_Context->AllTables = 0;
               /* Data Size for RGB component request */
               WordVal.w = NUMBER_OF_ELEMENTS_IN_GAMMA_LUT / sizeof(WORD); // / sizeof(WORD) for compress data

               // RED or Specified channel
               *(Bp_Reply++) = WordVal.b[0]; /* Index 1 */
               *(Bp_Reply++) = WordVal.b[1]; /* Index 2 */
               switch (pReq->ColorComponent)
               {
                  case   Gamma_G_Component:
                     p_Context->ColorCmpnt = GREEN_VLUT_COMPONENT;

                     break;

                  case   Gamma_B_Component:
                     p_Context->ColorCmpnt = BLUE_VLUT_COMPONENT;
                     break;

                  case   Gamma_R_Component:
                     p_Context->ColorCmpnt = RED_VLUT_COMPONENT;
                     break;

                  case   Gamma_RGB_Component:
                     p_Context->ColorCmpnt = RED_VLUT_COMPONENT;
                     p_Context->AllTables = 1;
                     /* Red */
                     //*(Bp_Reply++) = WordVal.b[0]; /* Index 1 */
                     //*(Bp_Reply++) = WordVal.b[1]; /* Index 2 */
                     /* Green */
                     *(Bp_Reply++) = WordVal.b[0]; /* Index 3 */
                     *(Bp_Reply++) = WordVal.b[1]; /* Index 4 */
                     /* Blue */
                     *(Bp_Reply++) = WordVal.b[0]; /* Index 5 */
                     *(Bp_Reply++) = WordVal.b[1]; /* Index 6 */
                     break;

                  default:
                     Bp_Reply[0] = Gamma_FormatNotSupported;
                     return PORT_MSG_HNDLR_OK;
               }
               //Bp_Reply++;
            }

            //calculate number of gamma table elements to be send during this transfer
            if ((NUMBER_OF_ELEMENTS_IN_GAMMA_LUT -p_Context->Shift) <= (W_Size-2))
            {
               //prepare for the last data transfer message
               W_Size = NUMBER_OF_ELEMENTS_IN_GAMMA_LUT -p_Context->Shift;
            }
            else
            {
               msg("Gamma: More Data",0);
               MoreData(TRUE);
               if (p_Context->FirstMessage)
               {
                  if (pReq->ColorComponent == Gamma_RGB_Component)
                     W_Size -= 8; // 7 byte and make it even
                  else
                     W_Size -= 4; // 3 byte and make it even
               }
               else
                  W_Size -= 2;

            }

            //read Gamma table block
            {
               WORD W_Shift = p_Context->Shift;

               msgd("Gamma: W_Shift = %d (0x%x)",W_Shift,W_Shift);
               msgd("Gamma: W_Size = %d (0x%x)", W_Size, W_Size);

               {
                  DWORD DW_Addr;

                  //Choose proper LUT addr
                  if (p_Context->ExtraData == Gamma_PreLUT)
                  {
                     DW_Addr =
                        (p_Context->ColorCmpnt == GREEN_VLUT_COMPONENT) ? INPUT_GAMMA_G_MEM_START :
                        (p_Context->ColorCmpnt == BLUE_VLUT_COMPONENT) ? INPUT_GAMMA_B_MEM_START:
                        INPUT_GAMMA_R_MEM_START;
                  }
                  else if (p_Context->ExtraData == Gamma_PostLUT)
                  {
                     DW_Addr =
                        (p_Context->ColorCmpnt == GREEN_VLUT_COMPONENT) ? OUTPUT_GAMMA_G_MEM_START :
                        (p_Context->ColorCmpnt == BLUE_VLUT_COMPONENT) ? OUTPUT_GAMMA_B_MEM_START:
                        OUTPUT_GAMMA_R_MEM_START;
                  }
                  else
                  {
                     Bp_Reply[0] = Gamma_FormatNotSupported;
                     msg("HAL_TsHandler_Gamma: Gamma_DownloadCompressedTable - not supported table ID request\n", 0);
                     break;
                  }

                  //Read the block of table date to the buffer
                  Wp_VLUT = LINEAR_TO_FPTR_W(DW_Addr + W_Shift);
               }
            }//read Gamma table block

            // Move buffer data to output buffer
            for (W_Index = 0 ; W_Index< W_Size ; W_Index+=sizeof(WORD))
            {
               WordVal.w = *(Wp_VLUT++);

               *Bp_Reply++ = WordVal.b[0];
               *Bp_Reply++ = WordVal.b[1];
            }

            //store counter of transmitted gamma table elements or clear it if gamma table transmitting is completed;
            //prepare context for next message or inform SMT handler that transmitting is completed;
            if ((NUMBER_OF_ELEMENTS_IN_GAMMA_LUT -p_Context->Shift) == W_Size)
            {
               p_Context->Shift = 0;
               MoreData(FALSE);

               if ((p_Context->AllTables) && (p_Context->ColorCmpnt != BLUE_VLUT_COMPONENT))
               {
                  p_Context->ColorCmpnt++;
                  MoreData(TRUE);
                  msg("Gamma: More Data",0);
               }
            }
            else
               p_Context->Shift += W_Size;

            if (p_Context->FirstMessage)
            {
               if (pReq->ColorComponent == Gamma_RGB_Component)
                  *Bp_Len = W_Size + 7; // 3 channel + 7 bytes
               else
                  *Bp_Len = W_Size + 3; // single channel +3 bytes
            }
            else
               *Bp_Len = W_Size;

            msg(" .....done",0);
         }
         break;

      case Gamma_UploadCompressedTable:
         #ifdef ST_4K2K_93xx_BOARD
         if(GetChipID_4K2K()==CHIPID_4K2K_R)
    		{
           	SMT_SendTimeoutTimetoHost(message, comm_use, SMT_TRANSACTION_TIMEOUT);
        	   ErrorCode = SMT_ForwardMessageToSlave(message, SOCIPC_DEVICEL_DST_ADDRESS);
        	}

			if(ErrorCode == PORT_MSG_HNDLR_OK)
    		#endif
         {
            BYTE far * pTransferTable= Bp_msg;
            WORD W_Size = GetMesageLength();
            WORD W_Index;
            BYTE * Bp_TempTable = TempBuffer;

            msg("Gamma_UploadCompressedTable",0);

            Bp_Reply [0] = Gamma_OK;
            *Bp_Len = 1;

            if (p_Context->FirstMessage)
            {//Handle the first message in the sequense

               //Communication buffer size is not enough for this handler.
               if (W_Size < 4)
               {
                  msg("buffer size is not enough = %d",W_Size);
                  return PORT_MSG_HNDLR_ERROR;
               }
               //Check if supported RGB color space is used;
               if (Bp_msg[1] != Gamma_Space_RGB)
               {
                  Bp_Reply[0] = Gamma_ColorSpaceNotSupported;
                  return (BYTE)PORT_MSG_HNDLR_OK;
               }

               p_Context->AllTables = 0;
               //Check if supported color components are received and convert them to gmt_VLUT_COLOR_COMPONENT type;
               switch ((GamaColorComponentType)Bp_msg[2])
               {
                  case   Gamma_G_Component:
                     p_Context->ColorCmpnt = GREEN_VLUT_COMPONENT;
                     break;

                  case   Gamma_B_Component:
                     p_Context->ColorCmpnt = BLUE_VLUT_COMPONENT;
                     break;

                  case   Gamma_R_Component:
                     p_Context->ColorCmpnt = RED_VLUT_COMPONENT;
                     break;

                  case   Gamma_RGB_Component://all three tables are coming one by one. The order is R-G-B
                     p_Context->AllTables = 1;
                     p_Context->ColorCmpnt = RED_VLUT_COMPONENT;
                     break;

                  default:
                     Bp_Reply[0] = Gamma_FormatNotSupported;
                     return PORT_MSG_HNDLR_OK;
               }

               if (Bp_msg[3] == 1) // PreLUT
               {
                  p_Context->ExtraData = Gamma_PreLUT;
               }
               else if (Bp_msg[3] == 0) // PostLUT
               {
                  p_Context->ExtraData = Gamma_PostLUT;
               }
               else
               {
                  Bp_Reply[0] = Gamma_FormatNotSupported;
                  return PORT_MSG_HNDLR_OK;
               }

               // Bp_msg[4] // channel, not used for now.

               // Data Length
               if ((GamaColorComponentType)Bp_msg[2] == Gamma_RGB_Component)
               {
                  // Bp_msg[5] ~ Bp_msg[10]
                  pTransferTable += 11;
                  W_Size -= 11;
               }
               else
               {
                  // Bp_msg[5]
                  pTransferTable += 7;
                  W_Size -= 7;
               }
            }

            //cut off data if it is too long for gamma table
            //Cutting off value depends on color component
            {
               WORD W_CutOffValue =sizeof(WORD)*((p_Context->AllTables) ? (NUMBER_OF_ELEMENTS_IN_GAMMA_LUT/sizeof(WORD))*3 : (NUMBER_OF_ELEMENTS_IN_GAMMA_LUT / sizeof(WORD))); // /sizeof(WORD) for compress data

               if ((W_Size + sizeof(WORD)*p_Context->Shift+p_Context->StoredFlag) > W_CutOffValue)
               {
                  W_Size = W_CutOffValue -p_Context->Shift;
                  Bp_Reply[0] = Gamma_FormatNotSupported;
               }
            }

            //copy input data bufer with taken in mind stored previous data - part of uncompleted gamma table component (low byte of a WORD)
            if (p_Context->StoredFlag)
            {
               Bp_TempTable[0] = p_Context->DataStored;
            }

            //take data from input buffer
            memcpy(&Bp_TempTable[p_Context->StoredFlag], (BYTE*)pTransferTable, W_Size);

            //modify Size value if data byte was stored
            W_Size += p_Context->StoredFlag;

#if 0 // Compress data no need to do this ...					 
            for (W_Index = 0; W_Index < W_Size; W_Index += sizeof(WORD))
            {
               //truncate less significant bits
               WordVal.b[0] = Bp_TempTable[W_Index];
               WordVal.b[1] = Bp_TempTable[W_Index+1];

               WordVal.w = WordVal.w >> (16-GAMMA_TABLE_WIDTH);

               Bp_TempTable[W_Index] = WordVal.b[0];
               Bp_TempTable[W_Index + 1] = WordVal.b[1];

            }
#endif


            //Load gamatable data.
            if (p_Context->AllTables)
            {
               // if RGB_Component calculate correct shift and color component ID
               //if((W_Size + sizeof(WORD)*p_Context->Shift) >= sizeof(WORD)*NUMBER_OF_ELEMENTS_IN_GAMMA_LUT)
               if ((W_Size + sizeof(WORD)*p_Context->Shift) >= NUMBER_OF_ELEMENTS_IN_GAMMA_LUT) // For compress data no need to *2
               {
                  //Write the last part of current component data to the VLUT;
                  //Change Color Component ID
                  //Set shift and size for the rest part of recevied table.
                  //WORD W_LastPart = sizeof(WORD)*NUMBER_OF_ELEMENTS_IN_GAMMA_LUT -sizeof(WORD)*p_Context->Shift;
                  WORD W_LastPart = NUMBER_OF_ELEMENTS_IN_GAMMA_LUT -sizeof(WORD)*p_Context->Shift; // For compress data no need to *2

                  msg("W_Size = %d (last part)",W_LastPart);
                  msg("p_Context->Shift = %d",p_Context->Shift);
                  msg("p_Context->ColorCmpnt = %d",p_Context->ColorCmpnt);

                  // W_Size is in BYTE resolution, shift is in WORD resolution, function start index is in BYTE resolution, size is in WORD resolution
                  if (p_Context->ExtraData == Gamma_PreLUT)
                     gm_LoadImpVlutTable(p_Context->ColorCmpnt, p_Context->Shift*2, W_LastPart/2, (WORD far*)Bp_TempTable, 1);
                  else
                     gm_LoadVlutTable(p_Context->ColorCmpnt, p_Context->Shift*2, W_LastPart/2, (WORD far*)Bp_TempTable, 1);

                  if (p_Context->ColorCmpnt != BLUE_VLUT_COMPONENT)
                     p_Context->ColorCmpnt++;

                  p_Context->Shift = 0;
                  W_Size -= W_LastPart;
                  Bp_TempTable += W_LastPart;
               }
            }

            msg("W_Size = %d",W_Size);
            msg("p_Context->Shift = %d",p_Context->Shift);
            msg("p_Context->ColorCmpnt = %d",p_Context->ColorCmpnt);

            if (W_Size)
            {
               // W_Size is in BYTE resolution, shift is in WORD resolution, function start index is in BYTE resolution, size is in WORD resolution
               if (p_Context->ExtraData == Gamma_PreLUT)
                  gm_LoadImpVlutTable(p_Context->ColorCmpnt, p_Context->Shift*2, W_Size/2, (WORD far*)Bp_TempTable, 1);
               else
                  gm_LoadVlutTable(p_Context->ColorCmpnt, p_Context->Shift*2, W_Size/2, (WORD far*)Bp_TempTable, 1);
            }

            p_Context->Shift += W_Size/sizeof(WORD);

            //store the last byte for next message if data size is odd
            if (W_Size%sizeof(WORD))
            {
               p_Context->StoredFlag= 1;
               p_Context->DataStored = Bp_TempTable[W_Size-1];
            }
            else
               p_Context->StoredFlag= 0;

            // turn on gamma correction if it is the last message
            if (!MoreDataExpected())
            {
               if(gm_ReadRegWord(DISP_LUT_CONTROL) != 0x3)
               {
                  gm_WriteRegWord(DISP_LUT_CONTROL, 0x3); // In and Out side Main Window
               }

               if (p_Context->ExtraData == Gamma_PreLUT)
                  gm_SetRegBitsWord(GAMMA_LUT1_CTRL, GAMMA_LUT1_CTL);
               else if(p_Context->ExtraData == Gamma_PostLUT)
                  gm_SetRegBitsWord(GAMMA_LUT2_CTRL, GAMMA_LUT2_CTL);

               //gm_SyncUpdate(gmvb_MainChannel, OUTPUT_CLOCK);
               gm_SetRegBitsWord(HOST_CONTROL, ODP_SYNC_UPDATE); // debugon can't use gm_SyncUpdate
            }
         }
         break;

      case Gamma_GetSteepTableColorspaceCapabilities:
         {
            //gmt_GammaUploadCoompressedTable_Req * pReq = (gmt_GammaUploadCoompressedTable_Req *)Bp_msg;
            gmt_SMTGetSteepTableColorspaceCapabilities_Res * pRes =      (gmt_SMTGetSteepTableColorspaceCapabilities_Res *)Bp_Reply;

            pRes->SupportLevel= SMTSupport_level_Supported_as_Separate_table; //

            pRes->NumberTableEntries = NUMBER_OF_ELEMENTS_IN_GAMMA_STEEP_LUT - 1; // For Capabilities return 128 ...

            pRes->TableEntrySize = GAMMA_STEEP_TABLE_WIDTH; //

            pRes->EntryOffset = 0; //

            pRes->SteepFinalEntrySize = 14; // Width of the steep table final entry in bits

            pRes->SteepTableSizeFactor = 1;

            pRes->SteepTabelAddressFactor = 1;

            //gm_Printf("Wait for detail ...", 0);

            *Bp_Len = sizeof(gmt_SMTGetSteepTableColorspaceCapabilities_Res);
         }
         break;

      case Gamma_DownloadLUTAndSteepTable:
         {
            gmt_GammaDownloadLUTAndSteepTable_Req * pReq = (gmt_GammaDownloadLUTAndSteepTable_Req *)Bp_msg;
            WORD W_Index;
            WORD far * Wp_VLUT = (WORD *)TempBuffer;

            //get size for output buffer and make it even
            WORD W_Size = 0xfffe & GetMaxDataLength();

            // Status
            *Bp_Reply = Gamma_OK;
            *Bp_Len = 1;
            msg("Gamma_DownloadLUTAndSteepTable %d",p_Context->Shift);

            //Check if it the first entry
            if (p_Context->FirstMessage)
            {
               //Check if supported RGB color space is requested;
               if (pReq->ColorSpace != Gamma_Space_RGB)
               {
                  *Bp_Reply = Gamma_ColorSpaceNotSupported;
                  break;
               }

               if (pReq->GammaTable < 2)
                  p_Context->ExtraData = pReq->GammaTable;
               else
               {
                  *Bp_Reply = Gamma_FormatNotSupported;
                  break;
               }
               // Increase address for status byte.
               Bp_Reply++;

               //Check if supported color components are received and convert them to gmt_VLUT_COLOR_COMPONENT type;
               p_Context->AllTables = 0;
               /* LUT Data Length for RGB component request */
               WordVal.w = NUMBER_OF_ELEMENTS_IN_GAMMA_LUT / sizeof(WORD); // Compress data transfer in WORD for 1024 byte

               // RED or Specified channel
               *(Bp_Reply++) = WordVal.b[0]; /* Index 1 */
               *(Bp_Reply++) = WordVal.b[1]; /* Index 2 */
               
               switch (pReq->ColorComponent)
               {
                  case   Gamma_G_Component:
                     p_Context->ColorCmpnt = GREEN_VLUT_COMPONENT;
                     break;

                  case   Gamma_B_Component:
                     p_Context->ColorCmpnt = BLUE_VLUT_COMPONENT;
                     break;

                  case   Gamma_R_Component:
                     p_Context->ColorCmpnt = RED_VLUT_COMPONENT;
                     break;

                  case   Gamma_RGB_Component:
                     p_Context->ColorCmpnt = RED_VLUT_COMPONENT;
                     p_Context->AllTables = 1;
                     /* Red */
                     //*(Bp_Reply++) = WordVal.b[0]; /* Index 1 */
                     //*(Bp_Reply++) = WordVal.b[1]; /* Index 2 */
                     /* Green */
                     *(Bp_Reply++) = WordVal.b[0]; /* Index 3 */
                     *(Bp_Reply++) = WordVal.b[1]; /* Index 4 */
                     /* Blue */
                     *(Bp_Reply++) = WordVal.b[0]; /* Index 5 */
                     *(Bp_Reply++) = WordVal.b[1]; /* Index 6 */
                     break;

                  default:
                     Bp_Reply[0] = Gamma_FormatNotSupported;
                     return PORT_MSG_HNDLR_OK;
               }

               {
                  DWORD DW_TempAddr;

                  DW_TempAddr = pReq->GammaTable ? GAMMA_LUT1_STEEP_STRT_R : GAMMA_LUT2_STEEP_STRT_R;

                  if (pReq->ColorComponent == Gamma_G_Component)
                  {
                     DW_TempAddr += 6; // Offset value from R to G
                  }
                  else if (pReq->ColorComponent == Gamma_B_Component)
                  {
                     DW_TempAddr += 12; // Offset value from R to B
                  }


                  // Steep Start R/G/B
                  WordVal.w = gm_ReadRegWord(DW_TempAddr);
                  //gm_Printf("~~~~ Steep Start = 0x%x", WordVal.w);
                  // Value in Register is 14-bit but return to DW is 10-bit
                  if (WordVal.w) // Must make sure the value is not 0
                     WordVal.w = (WordVal.w + 1) >> 4;

                  *(Bp_Reply++) = WordVal.b[0];
                  *(Bp_Reply++) = WordVal.b[1];

                  if (pReq->ColorComponent == Gamma_RGB_Component)
                  {
                     // Steep Start G
                     WordVal.w = gm_ReadRegWord(DW_TempAddr+6);
                     // Value in Register is 14-bit but return to DW is 10-bit
                     if (WordVal.w) // Must make sure the value is not 0
                        WordVal.w = (WordVal.w + 1) >> 4;
                     *(Bp_Reply++) = WordVal.b[0];
                     *(Bp_Reply++) = WordVal.b[1];

                     // Steep Start B
                     WordVal.w = gm_ReadRegWord(DW_TempAddr+12);
                     // Value in Register is 14-bit but return to DW is 10-bit
                     if (WordVal.w) // Must make sure the value is not 0
                        WordVal.w = (WordVal.w + 1) >> 4;
                     *(Bp_Reply++) = WordVal.b[0];
                     *(Bp_Reply++) = WordVal.b[1];
                  }

                  // Steep End
                  DW_TempAddr += 2; // offset value from Start to End
                  WordVal.w = gm_ReadRegWord(DW_TempAddr);
                  WordVal.w = (WordVal.w + 1) >> 4; // Set to 10-bit for Display Editor
                  //gm_Printf("~~~~ Steep End = 0x%x", WordVal.w);
                  *(Bp_Reply++) = WordVal.b[0];
                  *(Bp_Reply++) = WordVal.b[1];

                  if (pReq->ColorComponent == Gamma_RGB_Component)
                  {
                     // Steep Start G
                     WordVal.w = gm_ReadRegWord(DW_TempAddr+6);
                     WordVal.w = (WordVal.w + 1) >> 4; // Set to 10-bit for Display Editor
                     *(Bp_Reply++) = WordVal.b[0];
                     *(Bp_Reply++) = WordVal.b[1];

                     // Steep Start B
                     WordVal.w = gm_ReadRegWord(DW_TempAddr+12);
                     WordVal.w = (WordVal.w + 1) >> 4; // Set to 10-bit for Display Editor
                     *(Bp_Reply++) = WordVal.b[0];
                     *(Bp_Reply++) = WordVal.b[1];
                  }

                  // Steep Step
                  DW_TempAddr = pReq->GammaTable ? GAMMA_LUT1_CTRL: GAMMA_LUT2_CTRL;
                  {
                     //BYTE B_ShiftBit = pReq->GammaTable ? 8 : 12;
                     BYTE B_ShiftBit = pReq->GammaTable ? 12 : 8;
                     WORD B_MaskBit = pReq->GammaTable ?  GAMMA_LUT1_STEEP_ST: GAMMA_LUT2_STEEP_ST;
                     BYTE B_SteepStep = 0;

                     B_SteepStep = (gm_ReadRegWord(DW_TempAddr)&B_MaskBit) >> B_ShiftBit;

                     *(Bp_Reply++) = B_SteepStep;

                     if (pReq->ColorComponent == Gamma_RGB_Component)
                     {
                        *(Bp_Reply++) = B_SteepStep;
                        *(Bp_Reply++) = B_SteepStep;
                     }
                  }

                  // Steep Data Length
                  {
                     WORD B_MaskBit = pReq->GammaTable ?  GAMMA_LUT1_STEEP_CT: GAMMA_LUT2_STEEP_CT;
                     
                     if(gm_ReadRegWord(DW_TempAddr)&B_MaskBit)
                  		WordVal.w = NUMBER_OF_ELEMENTS_IN_GAMMA_STEEP_LUT; // return 129 ...
                     else
                        WordVal.w = 0; // return 0 when steep is not enable

                  	// RED or Specified channel
                  	*(Bp_Reply++) = WordVal.b[0]; /* Index 1 */
                  	*(Bp_Reply++) = WordVal.b[1]; /* Index 2 */

                  	if (pReq->ColorComponent == Gamma_RGB_Component)
                  	{
                     	/* Green */
                     	*(Bp_Reply++) = WordVal.b[0]; /* Index 3 */
                     	*(Bp_Reply++) = WordVal.b[1]; /* Index 4 */
                     	/* Blue */
                     	*(Bp_Reply++) = WordVal.b[0]; /* Index 5 */
                     	*(Bp_Reply++) = WordVal.b[1]; /* Index 6 */
                  	}
               	}
               }
               // Bp_Reply++;
            }



            //if(p_Context->UnCompletedElement == 0) // Check Steep Flag, gamma first ...
            {
               WORD W_NumberOfElement = 0;

               //calculate number of gamma table elements to be send during this transfer
               if (p_Context->UnCompletedElement == 0) // Check Steep Flag, gamma first ...
                  W_NumberOfElement = NUMBER_OF_ELEMENTS_IN_GAMMA_LUT * sizeof(BYTE); // Entry is 1024 but compress to 1024 byte which is 512 WORD
               else
                  W_NumberOfElement = NUMBER_OF_ELEMENTS_IN_GAMMA_STEEP_LUT * sizeof(WORD); // Entry is 129 in WORD

               if ((W_NumberOfElement -p_Context->Shift) <= (W_Size-2))
               {
                  //prepare for the last data transfer message
                  W_Size = W_NumberOfElement - p_Context->Shift;
               }
               else
               {
                  msg("Gamma: More Data",0);
                  MoreData(TRUE);
                  if (p_Context->FirstMessage)
                  {
                     if (pReq->ColorComponent == Gamma_RGB_Component)
                        W_Size -= 28; // 28 byte and make it even
                     else
                        W_Size -= 10; // 10 byte and make it even
                  }
                  else
                     W_Size -= 2;

               }

               //read Gamma table block
               {
                  WORD W_Shift = p_Context->Shift;

                  msgd("Gamma: W_Shift = %d (0x%x)",W_Shift,W_Shift);
                  msgd("Gamma: W_Size = %d (0x%x)", W_Size, W_Size);

                  {
                     DWORD DW_Addr;

                     //Choose proper LUT addr
                     if (p_Context->ExtraData == Gamma_PreLUT)
                     {
                        DW_Addr =
                           (p_Context->ColorCmpnt == RED_VLUT_COMPONENT) ? INPUT_GAMMA_R_MEM_START :
                           (p_Context->ColorCmpnt == GREEN_VLUT_COMPONENT) ? INPUT_GAMMA_G_MEM_START:
                           INPUT_GAMMA_B_MEM_START;
                     }
                     else if (p_Context->ExtraData == Gamma_PostLUT)
                     {
                        DW_Addr =
                           (p_Context->ColorCmpnt == RED_VLUT_COMPONENT) ? OUTPUT_GAMMA_R_MEM_START :
                           (p_Context->ColorCmpnt == GREEN_VLUT_COMPONENT) ? OUTPUT_GAMMA_G_MEM_START:
                           OUTPUT_GAMMA_B_MEM_START;
                     }
                     else
                     {
                        Bp_Reply[0] = Gamma_FormatNotSupported;
                        msg("HAL_TsHandler_Gamma: Gamma_DownloadCompressedTable - not supported table ID request\n", 0);
                        break;
                     }

                     if (p_Context->UnCompletedElement == 1) // if Steep add address offset
                     {
                        DW_Addr += 0x400;
                     }

                     //Read the block of table date to the buffer
                     if (p_Context->UnCompletedElement == 0) // Check Steep Flag, gamma first ...
                        Wp_VLUT = LINEAR_TO_FPTR_W(DW_Addr + W_Shift);
                     else
                     {
                        DW_Addr = DW_Addr + W_Shift * 2;

                        for (W_Index = 0; W_Index < W_Size/2; W_Index++)
                        {
                           // Steep in 12-bit shift to 16-bit for DisplayEditor
                           Wp_VLUT[W_Index] = (*LINEAR_TO_FPTR_W(DW_Addr)) << 4;

                           if(DW_Addr == 0x1DE00L)
                           {                              
                              Wp_VLUT[W_Index] = gm_ReadRegWord(GAMMA_LUT2_STEEP_FINAL_R) << 2;
                              //gm_Printf("POST Steep Fianl R 0x%X", Wp_VLUT[W_Index]);
                           }
                           else if(DW_Addr == 0x1E600L)
                           {
                              Wp_VLUT[W_Index] = gm_ReadRegWord(GAMMA_LUT2_STEEP_FINAL_G) << 2;
                              //gm_Printf("POST Steep Fianl G 0x%X", Wp_VLUT[W_Index]);                           
                           }
                           else if(DW_Addr == 0x1EE00L)
                           {
                              Wp_VLUT[W_Index] = gm_ReadRegWord(GAMMA_LUT2_STEEP_FINAL_B) << 2;
                              //gm_Printf("POST Steep Fianl B 0x%X", Wp_VLUT[W_Index]);                           
                           }
                           else if(DW_Addr == 0x1C600L)
                           {                              
                              Wp_VLUT[W_Index] = gm_ReadRegWord(GAMMA_LUT1_STEEP_FINAL_R) << 2;
                              //gm_Printf("Pre Steep Fianl R 0x%X", Wp_VLUT[W_Index]);
                           }
                           else if(DW_Addr == 0x1CE00L)
                           {
                              Wp_VLUT[W_Index] = gm_ReadRegWord(GAMMA_LUT1_STEEP_FINAL_G) << 2;
                              //gm_Printf("Pre Steep Fianl G 0x%X", Wp_VLUT[W_Index]);                           
                           }
                           else if(DW_Addr == 0x1D600L)
                           {
                              Wp_VLUT[W_Index] = gm_ReadRegWord(GAMMA_LUT1_STEEP_FINAL_B) << 2;
                              //gm_Printf("Pre Steep Fianl B 0x%X", Wp_VLUT[W_Index]);                           
                           }

                                                         
                           DW_Addr += 4; // steep LUT in DW format
                        }
                     }
                  }
               }//read Gamma table block

               // Move data from temp to reply buffer
               for (W_Index = 0 ; W_Index< W_Size ; W_Index+=sizeof(WORD))
               {

                  WordVal.w = *(Wp_VLUT++);

                  *Bp_Reply++ = WordVal.b[0];
                  *Bp_Reply++ = WordVal.b[1];
               }
            }

            {
               WORD W_NumberOfElement = 0;

               if (p_Context->UnCompletedElement == 0)
                  W_NumberOfElement = NUMBER_OF_ELEMENTS_IN_GAMMA_LUT * sizeof(BYTE);
               else
                  W_NumberOfElement = NUMBER_OF_ELEMENTS_IN_GAMMA_STEEP_LUT * sizeof(WORD);

               //store counter of transmitted gamma table elements or clear it if gamma table transmitting is completed;
               //prepare context for next message or inform SMT handler that transmitting is completed;
               //if((NUMBER_OF_ELEMENTS_IN_GAMMA_LUT*sizeof(BYTE) -p_Context->Shift) == W_Size)
               if ((W_NumberOfElement - p_Context->Shift) <= W_Size)
               {
                  p_Context->Shift = 0;
                  MoreData(FALSE);


                  if (p_Context->AllTables)
                  {
                     if (p_Context->ColorCmpnt != BLUE_VLUT_COMPONENT)
                     {
                        p_Context->ColorCmpnt++;
                        MoreData(TRUE);
                        msg("Gamma: More Data",0);
                     }
                     else if (p_Context->UnCompletedElement == 0) // Check Steep Flag
                     {
                        DWORD DW_TempAddr = pReq->GammaTable ? GAMMA_LUT1_CTRL: GAMMA_LUT2_CTRL;
                        WORD B_MaskBit = pReq->GammaTable ?  GAMMA_LUT1_STEEP_CT: GAMMA_LUT2_STEEP_CT;

                        if(gm_ReadRegWord(DW_TempAddr)&B_MaskBit)
                        {
                        	p_Context->UnCompletedElement = 1;
                        	p_Context->ColorCmpnt = 0;
                        	MoreData(TRUE);
                        	msg("Gamma: More Data for steep",0);
                     	}
                     	else
                        	msg("Gamma: Finish ...",0);
                  	}
                     else
                        msg("Gamma: Finish ...",0);
                  }
                  else if (p_Context->UnCompletedElement == 0) // Check Steep Flag
                  {
                     p_Context->UnCompletedElement = 1;
                     MoreData(TRUE);
                     msg("Gamma: More Data for steep",0);
                  }
                  else
                     msg("Gamma: Finish ... ",0);
               }
               else
                  p_Context->Shift += W_Size;
            }

            if (p_Context->FirstMessage)
            {
               if (pReq->ColorComponent == Gamma_RGB_Component)
                  *Bp_Len = W_Size + 28; // 3 channel + 28 bytes
               else
                  *Bp_Len = W_Size + 10; // single channel + 10 bytes
            }
            else
               *Bp_Len = W_Size;

            msg(" .....done, *Bp_Len %d", *Bp_Len);
         }
         break;

      case Gamma_UploadLUTAndSteepTable:
			#ifdef ST_4K2K_93xx_BOARD
			if(GetChipID_4K2K()==CHIPID_4K2K_R)
			{
				SMT_SendTimeoutTimetoHost(message, comm_use, SMT_TRANSACTION_TIMEOUT);
				ErrorCode = SMT_ForwardMessageToSlave(message, SOCIPC_DEVICEL_DST_ADDRESS);
			}
			
			if(ErrorCode == PORT_MSG_HNDLR_OK)
			#endif
         {
            BYTE far * pTransferTable= Bp_msg;
            WORD W_Size = GetMesageLength();
            WORD W_Index;
            BYTE * Bp_TempTable = TempBuffer;

            msg("Gamma_UploadLUTAndSteepTable",0);

            Bp_Reply [0] = Gamma_OK;
            *Bp_Len = 1;

            if (p_Context->FirstMessage)
            {//Handle the first message in the sequense

               //Communication buffer size is not enough for this handler.
               if (W_Size < 15)
               {
                  msg("buffer size is not enough = %d",W_Size);
                  return PORT_MSG_HNDLR_ERROR;
               }

               //Check if supported RGB color space is used;
               if (Bp_msg[1] != Gamma_Space_RGB)
               {
                  Bp_Reply[0] = Gamma_ColorSpaceNotSupported;
                  return (BYTE)PORT_MSG_HNDLR_OK;
               }

               p_Context->AllTables = 0;
               //Check if supported color components are received and convert them to gmt_VLUT_COLOR_COMPONENT type;
               switch ((GamaColorComponentType)Bp_msg[2])
               {
                  case   Gamma_G_Component:
                     p_Context->ColorCmpnt = GREEN_VLUT_COMPONENT;
                     break;

                  case   Gamma_B_Component:
                     p_Context->ColorCmpnt = BLUE_VLUT_COMPONENT;
                     break;

                  case   Gamma_R_Component:
                     p_Context->ColorCmpnt = RED_VLUT_COMPONENT;
                     break;

                  case   Gamma_RGB_Component://all three tables are coming one by one. The order is R-G-B
                     p_Context->AllTables = 1;
                     p_Context->ColorCmpnt = RED_VLUT_COMPONENT;
                     break;

                  default:
                     Bp_Reply[0] = Gamma_FormatNotSupported;
                     return PORT_MSG_HNDLR_OK;
               }

               if (Bp_msg[3] == 1) // PreLUT
               {
                  p_Context->ExtraData = Gamma_PreLUT;
               }
               else if (Bp_msg[3] == 0) // PostLUT
               {
                  p_Context->ExtraData = Gamma_PostLUT;
               }
               else
               {
                  Bp_Reply[0] = Gamma_FormatNotSupported;
                  return PORT_MSG_HNDLR_OK;
               }

               // Bp_msg[4] // channel, not used for now.

               if (Bp_msg[5] == 0) // LUT Compression
               {
                  // Athena only support compressed LUT data
                  Bp_Reply[0] = Gamma_FormatNotSupported;
                  return PORT_MSG_HNDLR_OK;
               }

               // LUT Data Length
               if ((GamaColorComponentType)Bp_msg[2] == Gamma_RGB_Component)
               {
                  // Bp_msg[6] ~ Bp_msg[11]
                  //pTransferTable += 12;
                  //W_Size -= 12;
                  //gm_Printf("W_Size = %d", W_Size);
                  //msg("R LUT Data Length [6] = %d", Bp_msg[6]);
                  //msg("R LUT Data Length [7] = %d", Bp_msg[7]);
                  //msg("G LUT Data Length [8] = %d", Bp_msg[8]);
                  //msg("G LUT Data Length [9] = %d", Bp_msg[9]);
                  //msg("B LUT Data Length [10] = %d", Bp_msg[10]);
                  //msg("B LUT Data Length [11] = %d", Bp_msg[11]);
               }
               else
               {
                  // Bp_msg[6]
                  //pTransferTable += 8;
                  //W_Size -= 8;
                  //gm_Printf("W_Size = %d", W_Size);
                  //msg("LUT Data Length [6] = %d", Bp_msg[6]);
                  //msg("LUT Data Length [7] = %d", Bp_msg[7]);
               }

               {
                  BYTE B_Shift = p_Context->AllTables ? 4 : 0;
                  DWORD DW_BaseAddr;

                  // Steep Compress
                  // Bp_msg[8+B_Shift] // Not used for Athena

                  DW_BaseAddr = p_Context->ExtraData ? GAMMA_LUT1_STEEP_STRT_R : GAMMA_LUT2_STEEP_STRT_R; // Check LUT1 or LUT2

                  if (p_Context->ColorCmpnt == GREEN_VLUT_COMPONENT)
                  {
                     DW_BaseAddr += 6; // Offset value from R to G
                  }
                  else if (p_Context->ColorCmpnt == BLUE_VLUT_COMPONENT)
                  {
                     DW_BaseAddr += 12; // Offset value from R to B
                  }

                  // Steep Start
                  // R or specified channel
                  WordVal.b[0] = Bp_msg[9 + B_Shift];
                  WordVal.b[1] = Bp_msg[10 + B_Shift];

                  // The input value from DE is 10-bit need to config to 14-bit
                  if (WordVal.w) // Must make sure START value not 0
                     WordVal.w = (WordVal.w << 4) - 1;

                  gm_WriteRegWord(DW_BaseAddr, WordVal.w);

                  if (p_Context->AllTables)
                  {
                     // G
                     WordVal.b[0] = Bp_msg[11 + B_Shift];
                     WordVal.b[1] = Bp_msg[12 + B_Shift];

                     // The input value from DE is 10-bit need to config to 14-bit
                     if (WordVal.w) // Must make sure START value not 0
                        WordVal.w = (WordVal.w << 4) - 1;

                     gm_WriteRegWord(DW_BaseAddr+6, WordVal.w);

                     // B
                     WordVal.b[0] = Bp_msg[13 + B_Shift];
                     WordVal.b[1] = Bp_msg[14 + B_Shift];

                     // The input value from DE is 10-bit need to config to 14-bit
                     if (WordVal.w) // Must make sure START value not 0
                        WordVal.w = (WordVal.w << 4) - 1;

                     gm_WriteRegWord(DW_BaseAddr+12, WordVal.w);
                  }

                  B_Shift = p_Context->AllTables ? 8 : 0;
                  DW_BaseAddr += 2; // Address offset from start to end
                  // Steep End
                  // R or specified channel
                  WordVal.b[0] = Bp_msg[11 + B_Shift];
                  WordVal.b[1] = Bp_msg[12 + B_Shift];

                  // The input value from DE is 10-bit need to config to 14-bit
                  WordVal.w = (WordVal.w << 4) - 1;

                  gm_WriteRegWord(DW_BaseAddr, WordVal.w);

                  if (p_Context->AllTables)
                  {
                     // G
                     WordVal.b[0] = Bp_msg[13 + B_Shift];
                     WordVal.b[1] = Bp_msg[14 + B_Shift];

                     // The input value from DE is 10-bit need to config to 14-bit
                     WordVal.w = (WordVal.w << 4) - 1;

                     gm_WriteRegWord(DW_BaseAddr+6, WordVal.w);

                     // B
                     WordVal.b[0] = Bp_msg[15 + B_Shift];
                     WordVal.b[1] = Bp_msg[16 + B_Shift];

                     // The input value from DE is 10-bit need to config to 14-bit
                     WordVal.w = (WordVal.w << 4) - 1;

                     gm_WriteRegWord(DW_BaseAddr+12, WordVal.w);
                  }

                  B_Shift = p_Context->AllTables ? 12 : 0;
                  // Steep Step
                  {
                     BYTE B_SteepStep;
                     BYTE B_SteepStepOffset = p_Context->ExtraData ? 12 : 8;

                     //gm_Printf("~~~~ Steep Mode = %d", Bp_msg[13 + B_Shift]);

                     B_SteepStep = Bp_msg[13 + B_Shift];

                     // 0 - 15 steps, 1 - 7 steps, 2 - 3 steps, 3 - 1 steps
                     if (B_SteepStep > 3)
                     {
                        // Not Support Steep Step
                        Bp_Reply[0] = Gamma_FormatNotSupported;
                        msg("Not Support Steep Step", 0);
                        return PORT_MSG_HNDLR_OK;
                     }

                     DW_BaseAddr = p_Context->ExtraData ? GAMMA_LUT1_CTRL : GAMMA_LUT2_CTRL; // Check LUT2 or LUT1
                     gm_ClearAndSetBitsWord(DW_BaseAddr, 3 << B_SteepStepOffset,((WORD)B_SteepStep) << B_SteepStepOffset);
                     msg("DW_BaseAddr = 0x%X", gm_ReadRegWord(DW_BaseAddr));

                     if (p_Context->AllTables)
                     {
                        if ((Bp_msg[13 + B_Shift] != Bp_msg[14 + B_Shift]) || (Bp_msg[14 + B_Shift] != Bp_msg[15 + B_Shift]))
                        {
                           Bp_Reply[0] = Gamma_InconsistentSteepSteps;
                           msg("Inconsistent Steep Steps", 0);
                           return PORT_MSG_HNDLR_OK;
                        }
                     }


                  }

                  B_Shift = p_Context->AllTables ? 14 : 0;
                  // Steep Data Length
                  // Bp_msg[14] ~ Bp_msg[15]
                  msg("Steep Data Length [14] = %d", Bp_msg[14+B_Shift]);
                  msg("Steep Data Length [15] = %d", Bp_msg[15+B_Shift]);

                  if((Bp_msg[14+B_Shift] == 0) && (Bp_msg[15+B_Shift] == 0)) // upload size 0
                  {
                     msg("Clear Steep CTRL", 0);
                     if(p_Context->ExtraData == Gamma_PreLUT)
							{
                        gm_ClearRegBitsWord(GAMMA_LUT1_CTRL, GAMMA_LUT1_STEEP_CT);
							}
                     else
							{
                        gm_ClearRegBitsWord(GAMMA_LUT2_CTRL, GAMMA_LUT2_STEEP_CT);
                  	}
                  }
                  else 
                  {
                     msg("Set Steep CTRL", 0);
                     if(p_Context->ExtraData == Gamma_PreLUT)
							{
                        gm_SetRegBitsWord(GAMMA_LUT1_CTRL, GAMMA_LUT1_STEEP_CT);
							}
                     else
							{
                        gm_SetRegBitsWord(GAMMA_LUT2_CTRL, GAMMA_LUT2_STEEP_CT);                  
                  	}                  
                  }                  

                  B_Shift = p_Context->AllTables ? 18 : 0;
                  pTransferTable += (16+B_Shift);
                  W_Size -= (16+B_Shift);
               }
            }

            //cut off data if it is too long for gamma table
            //Cutting off value depends on color component
            {
               WORD W_CutOffValue =sizeof(WORD)*((p_Context->AllTables) ? (NUMBER_OF_ELEMENTS_IN_GAMMA_LUT/sizeof(WORD)+NUMBER_OF_ELEMENTS_IN_GAMMA_STEEP_LUT)*3 : (NUMBER_OF_ELEMENTS_IN_GAMMA_LUT/sizeof(WORD)+NUMBER_OF_ELEMENTS_IN_GAMMA_STEEP_LUT));
               if ((W_Size + sizeof(WORD)*p_Context->Shift+p_Context->StoredFlag) > W_CutOffValue)
               {
                  W_Size = W_CutOffValue -p_Context->Shift;
                  Bp_Reply[0] = Gamma_FormatNotSupported;
               }
            }

            //copy input data bufer with taken in mind stored previous data - part of uncompleted gamma table component (low byte of a WORD)
            if (p_Context->StoredFlag)
            {
               Bp_TempTable[0] = p_Context->DataStored;
            }

            //take data from input buffer
            memcpy(&Bp_TempTable[p_Context->StoredFlag], (BYTE*)pTransferTable, W_Size);

            //modify Size value if data byte was stored
            W_Size += p_Context->StoredFlag;

#if 0 // No need to truncate because this is compress data
            for (W_Index = 0; W_Index < W_Size; W_Index += sizeof(WORD))
            {
               //truncate less significant bits
               WordVal.b[0] = Bp_TempTable[W_Index];
               WordVal.b[1] = Bp_TempTable[W_Index+1];

               WordVal.w = WordVal.w >> (16-GAMMA_TABLE_WIDTH);

               Bp_TempTable[W_Index] = WordVal.b[0];
               Bp_TempTable[W_Index + 1] = WordVal.b[1];

            }
#endif


            //Load gamma table data.
            if (p_Context->AllTables)
            {
               WORD W_NumberOfElements;

               if (p_Context->UnCompletedElement)
               {
                  msg(" Switch to Steep ", 0);
                  W_NumberOfElements = NUMBER_OF_ELEMENTS_IN_GAMMA_STEEP_LUT * sizeof(WORD); // Steep 128 x 2 bytes
               }
               else
               {
                  msg(" Switch to LUT ", 0);
                  W_NumberOfElements = NUMBER_OF_ELEMENTS_IN_GAMMA_LUT; // 1024 bytes
               }


               // if RGB_Component calculate correct shift and color component ID
               if ((W_Size + sizeof(WORD)*p_Context->Shift) >= W_NumberOfElements)
               {
                  //Write the last part of current component data to the VLUT;
                  //Change Color Component ID
                  //Set shift and size for the rest part of recevied table.
                  WORD W_LastPart = W_NumberOfElements -sizeof(WORD)*p_Context->Shift;

                  msg("W_Size = %d (last part)",W_LastPart);
                  msg("p_Context->Shift = %d",p_Context->Shift);
                  msg("p_Context->ColorCmpnt = %d",p_Context->ColorCmpnt);

                  //if (p_Context->ColorCmpnt >= RED_STEEP_VLUT_COMPONENT)
                  //   ShiftRight4bits(W_LastPart/2, (WORD far*)Bp_TempTable);

                  // Steep table shift in DWORD resolution so * 2 in advance
                  if ((p_Context->ColorCmpnt >= RED_STEEP_VLUT_COMPONENT) && W_Size)
                     p_Context->Shift = p_Context->Shift * 2;

                  // W_Size is in BYTE resolution, shift is in WORD resolution, function start index is in BYTE resolution, size is in WORD resolution
                  if (p_Context->ExtraData == Gamma_PreLUT)
                  {
                     gm_LoadImpVlutTable(p_Context->ColorCmpnt, p_Context->Shift*2, W_LastPart/2, (WORD far*)Bp_TempTable, 1);
                  }
                  else
                  {
                     gm_LoadVlutTable(p_Context->ColorCmpnt, p_Context->Shift*2, W_LastPart/2, (WORD far*)Bp_TempTable, 1);
                  }

                  //if(p_Context->ColorCmpnt != BLUE_VLUT_COMPONENT)
                  if (p_Context->ColorCmpnt != BLUE_STEEP_VLUT_COMPONENT)
                     p_Context->ColorCmpnt++;

                  if (p_Context->ColorCmpnt == RED_STEEP_VLUT_COMPONENT)
                     p_Context->UnCompletedElement = 1;

                  p_Context->Shift = 0;
                  W_Size -= W_LastPart;
                  Bp_TempTable += W_LastPart;
               }
            }
            else
            {
               WORD W_NumberOfElements;

               if (p_Context->UnCompletedElement)
               {
                  msg(" Switch to Steep ", 0);
                  W_NumberOfElements = NUMBER_OF_ELEMENTS_IN_GAMMA_STEEP_LUT * sizeof(WORD); // Steep 128 x 2 bytes
               }
               else
               {
                  msg(" Switch to LUT ", 0);
                  W_NumberOfElements = NUMBER_OF_ELEMENTS_IN_GAMMA_LUT; // 1024 bytes
               }


               // if RGB_Component calculate correct shift and color component ID
               if ((W_Size + sizeof(WORD)*p_Context->Shift) >= W_NumberOfElements)
               {
                  //Write the last part of current component data to the VLUT;
                  //Change Color Component ID
                  //Set shift and size for the rest part of recevied table.
                  WORD W_LastPart = W_NumberOfElements -sizeof(WORD)*p_Context->Shift;

                  msg("W_Size = %d (last part)",W_LastPart);
                  msg("p_Context->Shift = %d",p_Context->Shift);
                  msg("p_Context->ColorCmpnt = %d",p_Context->ColorCmpnt);

                  //if (p_Context->ColorCmpnt >= RED_STEEP_VLUT_COMPONENT)
                  //   ShiftRight4bits(W_LastPart/2, (WORD far*)Bp_TempTable);

                  // Steep table shift in DWORD resolution so * 2 in advance
                  if ((p_Context->ColorCmpnt >= RED_STEEP_VLUT_COMPONENT) && W_Size)
                     p_Context->Shift = p_Context->Shift * 2;

                  // W_Size is in BYTE resolution, shift is in WORD resolution, function start index is in BYTE resolution, size is in WORD resolution
                  if (p_Context->ExtraData == Gamma_PreLUT)
                  {
                     gm_LoadImpVlutTable(p_Context->ColorCmpnt, p_Context->Shift*2, W_LastPart/2, (WORD far*)Bp_TempTable, 1);
                  }
                  else
                  {
                     gm_LoadVlutTable(p_Context->ColorCmpnt, p_Context->Shift*2, W_LastPart/2, (WORD far*)Bp_TempTable, 1);
                  }

                  if(p_Context->ColorCmpnt == RED_VLUT_COMPONENT)
                  {
                     p_Context->ColorCmpnt = RED_STEEP_VLUT_COMPONENT;
                     //gm_Printf("R Steep", 0);
                  }
                  else if(p_Context->ColorCmpnt == GREEN_VLUT_COMPONENT)
                  {
                     p_Context->ColorCmpnt = GREEN_STEEP_VLUT_COMPONENT;
                     //gm_Printf("G Steep", 0);
                  }
                  else if(p_Context->ColorCmpnt == BLUE_VLUT_COMPONENT)
                  {
                     p_Context->ColorCmpnt = BLUE_STEEP_VLUT_COMPONENT;
                     //gm_Printf("B Steep", 0);
                  }

                  p_Context->UnCompletedElement = 1;

                  p_Context->Shift = 0;
                  W_Size -= W_LastPart;
                  Bp_TempTable += W_LastPart;
               }
            
            }

            msg("1 W_Size = %d",W_Size);
            msg("1 p_Context->Shift = %d",p_Context->Shift);
            msg("1 p_Context->ColorCmpnt = %d",p_Context->ColorCmpnt);

            // The data from DE is 16-bit which need to convert to 12-bit
            //if ((p_Context->ColorCmpnt >= RED_STEEP_VLUT_COMPONENT) && W_Size)
            //   ShiftRight4bits(W_Size/2, (WORD far*)Bp_TempTable);

            // Steep table shift in DWORD resolution so * 2 in advance
            if ((p_Context->ColorCmpnt >= RED_STEEP_VLUT_COMPONENT) && W_Size)
               p_Context->Shift = p_Context->Shift * 2;

            if (W_Size)
            {
               // W_Size is in BYTE resolution, shift is in WORD resolution, function start index is in BYTE resolution, size is in WORD resolution
               if (p_Context->ExtraData == Gamma_PreLUT)
               {
                  gm_LoadImpVlutTable(p_Context->ColorCmpnt, p_Context->Shift*2, W_Size/2, (WORD far*)Bp_TempTable, 1);
               }
               else
               {
                  gm_LoadVlutTable(p_Context->ColorCmpnt, p_Context->Shift*2, W_Size/2, (WORD far*)Bp_TempTable, 1);
               }
            }
            else
            {
               msg("W_Size==0", 0);
            }

            p_Context->Shift += W_Size/sizeof(WORD);

            //store the last byte for next message if data size is odd
            if (W_Size%sizeof(WORD))
            {
               p_Context->StoredFlag= 1;
					p_Context->DataStored = Bp_TempTable[W_Size-1];
            }
            else
               p_Context->StoredFlag= 0;

            // turn on gamma correction if it is the last message
            if (!MoreDataExpected())
            {
               if(gm_ReadRegWord(DISP_LUT_CONTROL) != 0x3)
               {
                  gm_WriteRegWord(DISP_LUT_CONTROL, 0x3); // In and Out side Main Window
               }

               // set steep ctrl in length check to cover tool ...
               if (p_Context->ExtraData == Gamma_PreLUT)
					{
                  //gm_SetRegBitsWord(GAMMA_LUT1_CTRL, GAMMA_LUT1_CTL|GAMMA_LUT1_STEEP_CT);
                  gm_SetRegBitsWord(GAMMA_LUT1_CTRL, GAMMA_LUT1_CTL);
					}
               else if(p_Context->ExtraData == Gamma_PostLUT)
					{
                  //gm_SetRegBitsWord(GAMMA_LUT2_CTRL, GAMMA_LUT2_CTL|GAMMA_LUT2_STEEP_CT);
                  gm_SetRegBitsWord(GAMMA_LUT2_CTRL, GAMMA_LUT2_CTL);
					}

               //gm_SyncUpdate(gmvb_MainChannel, OUTPUT_CLOCK);
               gm_SetRegBitsWord(HOST_CONTROL, ODP_SYNC_UPDATE); // debugon can't use gm_SyncUpdate

               msg("Finish transfer data ...", 0);

            }
            else
            {
               msg("~~~~ More Data ...", 0);
            }
         }
         break;

      case Gamma_EnableTable:
         #ifdef ST_4K2K_93xx_BOARD
         if(GetChipID_4K2K()==CHIPID_4K2K_R)
    		{
        		ErrorCode = SMT_ForwardMessageToSlave(message, SOCIPC_DEVICEL_DST_ADDRESS);
        	}

			if(ErrorCode == PORT_MSG_HNDLR_OK)
    		#endif
         {
            // Color Space
            msg(" Color Space = %d",Bp_msg[1]);

            // Component
            msg(" Component = %d",Bp_msg[2]);

            // Channel
            msg(" Channel = %d",Bp_msg[3]);

            // Enable
            msg(" Enable = %d",Bp_msg[4]);

            Bp_Reply [0] = Gamma_OK;
            *Bp_Len = 1;

         }
         break;

      case Gamma_GetNumOfProfiles:
         {
            //Bp_Reply [0] = Gamma_OK; // no status ...
            
            // Max Profiles: default 2
            Bp_Reply [0] = Gamma_MaxProfile;
            
            // Components-4 byte: default Gamma_Athena_Default_Component
            Bp_Reply [1] = Gamma_Athena_Default_Component;
            Bp_Reply [2] = 0; // reserved
            Bp_Reply [3] = 0; // reserved
            Bp_Reply [4] = 0; // reserved
            
            *Bp_Len = 6;            
         }
         break;

      case Gamma_SelectProfile:
         #ifdef ST_4K2K_93xx_BOARD
         if(GetChipID_4K2K()==CHIPID_4K2K_R)
    		{
        		ErrorCode = SMT_ForwardMessageToSlave(message, SOCIPC_DEVICEL_DST_ADDRESS);
        	}

			if(ErrorCode == PORT_MSG_HNDLR_OK)
    		#endif
         {
            // Profile ID
            msg(" Profile ID = %d", Bp_msg[1]);
            
            if(Bp_msg[1] > Gamma_MaxProfile)
            {
               Bp_Reply [0] = 1; // Invalid Profile ID
               *Bp_Len = 1;
            }
            else
            {
               #ifdef SAVING_DATA_INTO_FLASH
               DWORD DW_FlashAddr = 0x5FA000; // Real Flash address :0x1FB000, 0x5FB000 is Athena mapping address            
               #endif            
			   #ifdef SAVING_DATA_INTO_NVRAM
			   WORD DW_NvramAddr = C_GAMMA_COMPONENT_SAVED_START_OFFSET;
			   #endif
               
               B_SelectedProfile = Bp_msg[1] - 1; // Tool range is 1 ~ 254, FW from 0 ~ 253
               
               Bp_Reply [0] = Gamma_OK;

               // Components: Gamma_Athena_Default_Component
               #ifdef SAVING_DATA_INTO_FLASH
               DW_FlashAddr = 0x5FA000 - (0x2000*B_SelectedProfile);
               
               if(FlashReadByte(DW_FlashAddr) == 0xFF) // 0xFF means flash is empty
               {
                  msg("Empty", 0);
                  Bp_Reply [1] = 0;
                  Bp_Reply [2] = 0;
                  Bp_Reply [3] = 0;
                  Bp_Reply [4] = 0;
               }
               else
               {
                  Bp_Reply [1] = FlashReadByte(DW_FlashAddr);
                  Bp_Reply [2] = FlashReadByte(DW_FlashAddr+1);
                  Bp_Reply [3] = FlashReadByte(DW_FlashAddr+2);
                  Bp_Reply [4] = FlashReadByte(DW_FlashAddr+3);    
                  msg("Saved Component 0x%X", Bp_Reply [1]);
               }
               #else
#ifdef SAVING_DATA_INTO_NVRAM
				DW_NvramAddr = C_GAMMA_COMPONENT_SAVED_START_OFFSET + (0x2000*B_SelectedProfile);

				if(NVRam_ReadByte(DW_NvramAddr) == 0xFF) // 0xFF means flash is empty
				{
				   Bp_Reply [1] = 0;
				   Bp_Reply [2] = 0;
				   Bp_Reply [3] = 0;
				   Bp_Reply [4] = 0;
				}
				else
				{
				   Bp_Reply [1] = NVRam_ReadByte(DW_NvramAddr);
				   Bp_Reply [2] = NVRam_ReadByte(DW_NvramAddr+1);
				   Bp_Reply [3] = 0;//NVRam_ReadByte(DW_NvramAddr+2);
				   Bp_Reply [4] = 0;//NVRam_ReadByte(DW_NvramAddr+3);
				}
#else
               Bp_Reply [1] = Gamma_Athena_Default_Component;
               Bp_Reply [2] = 0;
               Bp_Reply [3] = 0;
               Bp_Reply [4] = 0;
               #endif
               #endif
               
               *Bp_Len = 5;            
            }
         }
         break;

      case Gamma_SaveProfile:
         #ifdef ST_4K2K_93xx_BOARD
         if(GetChipID_4K2K()==CHIPID_4K2K_R)
    		{
           	SMT_SendTimeoutTimetoHost(message, comm_use, 2*SMT_TRANSACTION_TIMEOUT);
        	   ErrorCode = SMT_ForwardMessageToSlave(message, SOCIPC_DEVICEL_DST_ADDRESS);
        	}

			if(ErrorCode == PORT_MSG_HNDLR_OK)
    		#endif
         {
            DWORD DW_ComponentToSave = 0;
            
            #ifdef SAVING_DATA_INTO_FLASH
            DWORD DW_FlashAddr = 0x5FA000; // Real Flash address :0x1FB000, 0x5FB000 is Athena mapping address            
            #elif defined(SAVING_DATA_INTO_NVRAM)
			WORD DW_NvramAddr = C_GAMMA_COMPONENT_SAVED_START_OFFSET;
#ifdef Gamma_DUM_Initial_Check
			BYTE IntsaColor_CheckSum=0;
#endif
			#endif

           #if 0
            if(B_ReturnBusy)
            {
               msg("BUSY", 0);
               return PORT_MSG_HNDLR_BUSY;
            }
            #endif

            //if(B_ReturnBusy == 0)
            {
               //extern BYTE SMT_GetTransID(MsgPacket_t * message);
               //extern SMT_TransactionStruct_t SMT_RAM * IsTransIDValid(BYTE TransID);
               //extern void SMT_SendBusy(BYTE TransID, BYTE comm_use, BYTE EvenOdd, WORD W_Delay_ms);
               
               BYTE ExpectedEvenOdd;
               BYTE B_TransactionID;
               SMT_TransactionStruct_t SMT_RAM * pCurrentTransaction;

               B_TransactionID = SMT_GetTransID(message);
               pCurrentTransaction = IsTransIDValid(B_TransactionID);
                              
               //gm_Printf("BUSY", 0);
               {
                  ExpectedEvenOdd = message->msgType & EVEN_ODD_BIT;

                  #if 0 // 1
                  if(B_ODD_EVEN_OverRide)
                     if(ExpectedEvenOdd)
                        ExpectedEvenOdd = 0;
                     else
                        ExpectedEvenOdd = EVEN_ODD_BIT;
                  #endif
               }
			   #ifdef SAVING_DATA_INTO_NVRAM
               SMT_SendBusy(pCurrentTransaction->ID,comm_use,ExpectedEvenOdd,2000);
			   #else
               SMT_SendBusy(pCurrentTransaction->ID,comm_use,ExpectedEvenOdd,2000); // add for re-load flash data
			   #endif

               //B_ReturnBusy = 1;               
            }
            
            Bp_Reply [0] = Gamma_OK; // Sucess            
            
            //gm_Printf("S %d", gm_GetSystemTime());
            //gm_Printf("S %d", B_SelectedProfile);
            // Component
            msg(" Component = 0x%x", Bp_msg[1]);
            //gm_Printf(" C 0x%X", Bp_msg[1]);

            if((Bp_msg[1] & 0xE0) || Bp_msg[2] || Bp_msg[3] || Bp_msg[4])
            {
               //gm_Printf("Invalid Component to Save", 0);
               Bp_Reply [0] = 1; // Invalid Component
               *Bp_Len = 1;             
               break;
            }
            DW_ComponentToSave = (DWORD)Bp_msg[1] & Gamma_Athena_Default_Component;
            
            // reserved
            //Bp_msg[2]
            //Bp_msg[3]
            //Bp_msg[4]
            
            #ifdef SAVING_DATA_INTO_FLASH
            DW_FlashAddr = 0x5FA000 - (0x2000*B_SelectedProfile); // 2 sector for 1 color space

            if(FlashReadByte(DW_FlashAddr) != 0xFF) // check empty
            {
               BYTE B_Component[5]={0};
                                        
               B_Component[1] = FlashReadByte(DW_FlashAddr);
               CLRBIT(B_Component[1], (BYTE)DW_ComponentToSave);
               //gm_Printf("Component to apply first 0x%X", B_Component[1]);
               B_Component[2] = FlashReadByte(DW_FlashAddr+1);
               B_Component[3] = FlashReadByte(DW_FlashAddr+2);
               B_Component[4] = FlashReadByte(DW_FlashAddr+3);

               Gamma_Apply_Profile((BYTE far *)&B_Component, B_SelectedProfile); // save profile not re-load data into memory first

               SETBIT((BYTE)DW_ComponentToSave, B_Component[1]); // set the original component
               Bp_msg[1] = (BYTE)DW_ComponentToSave; // over write the component to original profile's component
               Bp_msg[2] = FlashReadByte(DW_FlashAddr+1);
               Bp_msg[3] = FlashReadByte(DW_FlashAddr+2);
               Bp_msg[4] = FlashReadByte(DW_FlashAddr+3);                                         
               Gamma_Erase_Profile(B_SelectedProfile); // clear current profile for re-load data
            }

            
            while(DW_ComponentToSave)
            {
               DWORD DW_LUTAddr;
               
               // reset the base flash address for each component
               DW_FlashAddr = 0x5FA000 - (0x2000*B_SelectedProfile); // 2 sector for 1 color space
                  
               if(DW_ComponentToSave & 0x1B) // other than Matrix
               {
                  WORD i, W_size;
                  BYTE j;

                  for(j = 0; j < 3; j++)
                  {
                     if(j == 0) // R
                     {
                        if(DW_ComponentToSave & Gamma_PostLUT_Component)
                        {
                           if(gm_ReadRegWord(GAMMA_LUT2_CTRL)&GAMMA_LUT2_CTL)
                           {
                              DW_LUTAddr = OUTPUT_GAMMA_R_MEM_START;
                              DW_FlashAddr += POST_LUT_START_OFFSET;
                              W_size = 1024;
                           }
                           else
                           {
                              //gm_Printf("Data is not uploaded: Post LUT", 0);
                              Bp_Reply [0] = 2; // Component not uploaded
                              DW_ComponentToSave = 0; // clear the component to save flag
                              break; // stop saving                           
                           }
                           
                        }
                        else if(DW_ComponentToSave & Gamma_PreLUT_Component)
                        {
                           if(gm_ReadRegWord(GAMMA_LUT1_CTRL)&GAMMA_LUT1_CTL)
                           {
                              DW_LUTAddr = INPUT_GAMMA_R_MEM_START;
                              DW_FlashAddr += PRE_LUT_START_OFFSET;
                              W_size = 1024;
                           }
                           else
                           {
                              //gm_Printf("Data is not uploaded: Pre LUT", 0);
                              Bp_Reply [0] = 2; // Component not uploaded
                              DW_ComponentToSave = 0; // clear the component to save flag
                              break; // stop saving                           
                           }
                        }                        
                        else if(DW_ComponentToSave & Gamma_PostSteep_Component)
                        {
                           if(gm_ReadRegWord(GAMMA_LUT2_CTRL)&GAMMA_LUT2_STEEP_CT)
                           {
                              DW_LUTAddr = OUTPUT_GAMMA_R_STEEP_MEM_START;
                              DW_FlashAddr += POST_STEEP_START_OFFSET;
                              W_size = 128;
                           }
                           else
                           {
                              //gm_Printf("Data is not uploaded: Post Steep", 0);
                              Bp_Reply [0] = 2; // Component not uploaded
                              DW_ComponentToSave = 0; // clear the component to save flag
                              break; // stop saving                           
                           }
                        }                        
                        else if(DW_ComponentToSave & Gamma_PreSteep_Component)
                        {
                           if(gm_ReadRegWord(GAMMA_LUT1_CTRL)&GAMMA_LUT1_STEEP_CT)
                           {
                              DW_LUTAddr = INPUT_GAMMA_R_STEEP_MEM_START;
                              DW_FlashAddr += PRE_STEEP_START_OFFSET;
                              W_size = 128;
                           }
                           else
                           {
                              //gm_Printf("Data is not uploaded: Pre Steep", 0);
                              Bp_Reply [0] = 2; // Component not uploaded
                              DW_ComponentToSave = 0; // clear the component to save flag
                              break; // stop saving                           
                           }
                        }                        
                        else
                        {                           
                           //gm_Printf("Unclear compponent R", 0);
                           Bp_Reply [0] = 3; // Component not available
                           DW_ComponentToSave = 0; // clear the component to save flag
                           break; // stop saving                           
                        }
                     }
                     else if(j == 1) // G
                     {
                        if(DW_ComponentToSave & Gamma_PostLUT_Component)
                        {
                           DW_LUTAddr = OUTPUT_GAMMA_G_MEM_START;
                           W_size = 1024;
                        }
                        else if(DW_ComponentToSave & Gamma_PreLUT_Component)
                        {
                           DW_LUTAddr = INPUT_GAMMA_G_MEM_START;
                           W_size = 1024;
                        }                        
                        else if(DW_ComponentToSave & Gamma_PostSteep_Component)
                        {
                           DW_LUTAddr = OUTPUT_GAMMA_G_STEEP_MEM_START;
                           W_size = 128;
                        }                        
                        else if(DW_ComponentToSave & Gamma_PreSteep_Component)
                        {
                           DW_LUTAddr = INPUT_GAMMA_G_STEEP_MEM_START;
                           W_size = 128;
                        }                        
                        else
                        {
                           //gm_Printf("Unclear Component G", 0);
                           Bp_Reply [0] = 3; // Component not available
                           DW_ComponentToSave = 0; // clear the component to save flag
                           break; // stop saving                           
                        }                     
                     }
                     else // B
                     {
                        if(DW_ComponentToSave & Gamma_PostLUT_Component)
                        {
                           DW_LUTAddr = OUTPUT_GAMMA_B_MEM_START;
                           W_size = 1024;
                           //CLRBIT(DW_ComponentToSave, Gamma_PostLUT_Component);
                        }
                        else if(DW_ComponentToSave & Gamma_PreLUT_Component)
                        {
                           DW_LUTAddr = INPUT_GAMMA_B_MEM_START;
                           W_size = 1024;
                           //CLRBIT(DW_ComponentToSave, Gamma_PreLUT_Component);
                        }                        
                        else if(DW_ComponentToSave & Gamma_PostSteep_Component)
                        {
                           DW_LUTAddr = OUTPUT_GAMMA_B_STEEP_MEM_START;
                           W_size = 128;
                           CLRBIT(DW_ComponentToSave, Gamma_PostSteep_Component);
                        }                        
                        else if(DW_ComponentToSave & Gamma_PreSteep_Component)
                        {
                           DW_LUTAddr = INPUT_GAMMA_B_STEEP_MEM_START;
                           W_size = 128;
                           CLRBIT(DW_ComponentToSave, Gamma_PreSteep_Component);
                        }                        
                        else
                        {
                           //gm_Printf("Unclear Component B", 0);
                           Bp_Reply [0] = 3; // Component not available
                           DW_ComponentToSave = 0; // clear the component to save flag
                           break; // stop saving                           
                        }                     
                     }
                                       
                     for(i = 0; i < W_size; i++)
                     {                     
                        if(W_size == 128)
                        {
                           //gm_Printf("%lx %x", DW_LUTAddr, *LINEAR_TO_FPTR_W(DW_LUTAddr));                           
                           FlashWriteWord(DW_FlashAddr, *LINEAR_TO_FPTR_W(DW_LUTAddr));
                           DW_LUTAddr += 4;
                           DW_FlashAddr += 2;
                        }
                        else
                        {                           
                           FlashWriteByte(DW_FlashAddr, *LINEAR_TO_FPTR_B(DW_LUTAddr));
                           DW_LUTAddr += 1;
                           DW_FlashAddr += 1;
                        }                                                                       
                     }
                  }

                  if(DW_ComponentToSave & Gamma_PostLUT_Component)
                  {
                     //gm_Printf(" Save Post LUT Config Registers", 0);
                     DW_FlashAddr = 0x5FA000 - (0x2000*B_SelectedProfile) + POST_LUT_CONFIG_REG_START_OFFSET; // 2 sector for 1 color space
                     //gm_WriteRegWord(GAMMA_LUT2_CTRL, FlashReadWord(DW_FlashAddr));
                     FlashWriteWord(DW_FlashAddr, gm_ReadRegWord(GAMMA_LUT2_CTRL));   
                     
                     DW_FlashAddr += 2;
                     DW_LUTAddr = GAMMA_LUT2_STEEP_STRT_R;            

                     for(i = 0; i < 12; i++)
                     {
                        //gm_WriteRegWord(DW_LUTAddr, FlashReadWord(DW_FlashAddr));
                        FlashWriteWord(DW_FlashAddr, gm_ReadRegWord(DW_LUTAddr));
                        DW_LUTAddr += 2;
                        DW_FlashAddr += 2;            
                     }
                  
                     CLRBIT(DW_ComponentToSave, Gamma_PostLUT_Component);
                  }
                  else if(DW_ComponentToSave & Gamma_PreLUT_Component)
                  {
                     //gm_Printf(" Save Pre LUT Config Registers", 0);
                     DW_FlashAddr = 0x5FA000 - (0x2000*B_SelectedProfile) + PRE_LUT_CONFIG_REG_START_OFFSET; // 2 sector for 1 color space
                     //gm_WriteRegWord(GAMMA_LUT2_CTRL, FlashReadWord(DW_FlashAddr));
                     FlashWriteWord(DW_FlashAddr, gm_ReadRegWord(GAMMA_LUT1_CTRL));   
                     
                     DW_FlashAddr += 2;
                     DW_LUTAddr = GAMMA_LUT1_STEEP_STRT_R;            

                     for(i = 0; i < 12; i++)
                     {
                        //gm_WriteRegWord(DW_LUTAddr, FlashReadWord(DW_FlashAddr));
                        FlashWriteWord(DW_FlashAddr, gm_ReadRegWord(DW_LUTAddr));
                        DW_LUTAddr += 2;
                        DW_FlashAddr += 2;            
                     }
                  
                     CLRBIT(DW_ComponentToSave, Gamma_PreLUT_Component);
                  }                  
               }
               else if(DW_ComponentToSave & Gamma_Matrix_Component)
               {
                  BYTE i;
                  DW_LUTAddr = GAMMA_LUT_MATRIX_MULT_COEF11; // GAMMA_LUT1_CTRL;
                  DW_FlashAddr += GAMMA_REGISTER_START_OFFSET;

                  //for(i = 0; i < 41; i++) // 41 registers
                  for(i = 0; i < 15; i++) // 15 registers
                  {                     
                     FlashWriteWord(DW_FlashAddr, gm_ReadRegWord(DW_LUTAddr));
                     DW_LUTAddr += 2;
                     DW_FlashAddr += 2;
                     
                  }

                  CLRBIT(DW_ComponentToSave, Gamma_Matrix_Component);
               }
               else
               {
                  //gm_Printf(" Unclear Component ", 0);
                  Bp_Reply [0] = 3; // Component not available
                  DW_ComponentToSave = 0;
               }
            }            

            // save the components saved flag once done.
            if(Bp_Reply [0] == Gamma_OK)
            {
               DW_FlashAddr = 0x5FA000 - (0x2000*B_SelectedProfile); // 2 sector for 1 color space
               
               FlashWriteByte(DW_FlashAddr, Bp_msg[1]);               
               FlashWriteByte(DW_FlashAddr+1, Bp_msg[2]);
               FlashWriteByte(DW_FlashAddr+2, Bp_msg[3]);
               FlashWriteByte(DW_FlashAddr+3, Bp_msg[4]);
               //gm_Printf("Saved Components 0x%X", Bp_msg[1]);

               #if 1 // reload the flash data ... in test it taks around 10xxms
               {                  
                  //gm_MemoryCodeStop();
                  gm_MemoryCodeAdjust(DATA_DRAM_SIZE); // Change boundary
                  gm_MemoryCodeStart(); // Must start here or will not upate the data.
               //gm_Printf("1 gm_MemoryCodeStop %d", gm_GetSystemTime());
               gm_MemoryCodeDownload();
                  gm_MemoryCodeAdjust(DATA_DRAM_SIZE+CODE_SIZE);
               //gm_Printf("2 gm_MemoryCodeDownload %d", gm_GetSystemTime());
               gm_MemoryCodeStart();               
               }              
               #endif
            }
			
      #elif defined(SAVING_DATA_INTO_NVRAM) 
	  
            DW_NvramAddr = C_GAMMA_COMPONENT_SAVED_START_OFFSET + (0x2000*B_SelectedProfile); // 2 sector for 1 color space

            if(NVRam_ReadByte(DW_NvramAddr) != 0xFF) // check empty
            {
               BYTE B_Component[5]={0};
                                        
               B_Component[1] = NVRam_ReadByte(DW_NvramAddr);
               CLRBIT(B_Component[1], (BYTE)DW_ComponentToSave);
               //gm_Printf("Component to apply first 0x%X", B_Component[1]);
               B_Component[2] = NVRam_ReadByte(DW_NvramAddr+1);
               B_Component[3] = NVRam_ReadByte(DW_NvramAddr+2);
               B_Component[4] = NVRam_ReadByte(DW_NvramAddr+3);
            
               Gamma_Apply_Profile((BYTE far *)&B_Component, B_SelectedProfile); // save profile not re-load data into memory first

               SETBIT((BYTE)DW_ComponentToSave, B_Component[1]); // set the original component
               Bp_msg[1] = (BYTE)DW_ComponentToSave; // over write the component to original profile's component
               Bp_msg[2] = NVRam_ReadByte(DW_NvramAddr+1);
               Bp_msg[3] = NVRam_ReadByte(DW_NvramAddr+2);
               Bp_msg[4] = NVRam_ReadByte(DW_NvramAddr+3);                                         
               Gamma_Erase_Profile(B_SelectedProfile); // clear current profile for re-load data
            }

            
            while(DW_ComponentToSave)
            {
               DWORD DW_LUTAddr;
			   BYTE far * Wp_Dest;
               BYTE B_temp[1024];
               // reset the base flash address for each component
			   
//			   DW_NvramAddr = C_GAMMA_COMPONENT_SAVED_START_OFFSET + (0x2000*B_SelectedProfile); 
			   
               if(DW_ComponentToSave & 0x1B) // other than Matrix
               {
                  WORD i, W_size;
                  BYTE j;

                  for(j = 0; j < 3; j++)
                  {
                     if(j == 0) // R
                     {
                        if(DW_ComponentToSave & Gamma_PostLUT_Component)
                        {
                           if(gm_ReadRegWord(GAMMA_LUT2_CTRL)&GAMMA_LUT2_CTL)
                           {
                              DW_LUTAddr = OUTPUT_GAMMA_R_MEM_START;
							  DW_NvramAddr = C_POST_LUT_START_OFFSET+ (0x2000*B_SelectedProfile); // 
                              W_size = 1024;
                           }
                           else
                           {
                              //gm_Printf("Data is not uploaded: Post LUT", 0);
                              Bp_Reply [0] = 2; // Component not uploaded
                              DW_ComponentToSave = 0; // clear the component to save flag
                              break; // stop saving                           
                           }
                           
                        }
                        else if(DW_ComponentToSave & Gamma_PreLUT_Component)
                        {
                           if(gm_ReadRegWord(GAMMA_LUT1_CTRL)&GAMMA_LUT1_CTL)
                           {
                              DW_LUTAddr = INPUT_GAMMA_R_MEM_START;
							  DW_NvramAddr = C_PRE_LUT_START_OFFSET+ (0x2000*B_SelectedProfile); // 
                              W_size = 1024;
                           }
                           else
                           {
                              //gm_Printf("Data is not uploaded: Pre LUT", 0);
                              Bp_Reply [0] = 2; // Component not uploaded
                              DW_ComponentToSave = 0; // clear the component to save flag
                              break; // stop saving                           
                           }
                        }                        
                        else if(DW_ComponentToSave & Gamma_PostSteep_Component)
                        {
                           if(gm_ReadRegWord(GAMMA_LUT2_CTRL)&GAMMA_LUT2_STEEP_CT)
                           {
                              DW_LUTAddr = OUTPUT_GAMMA_R_STEEP_MEM_START;
							  DW_NvramAddr = C_POST_STEEP_START_OFFSET+ (0x2000*B_SelectedProfile); // 
                              W_size = 128;
                           }
                           else
                           {
                              //gm_Printf("Data is not uploaded: Post Steep", 0);
                              Bp_Reply [0] = 2; // Component not uploaded
                              DW_ComponentToSave = 0; // clear the component to save flag
                              break; // stop saving                           
                           }
                        }                        
                        else if(DW_ComponentToSave & Gamma_PreSteep_Component)
                        {
                           if(gm_ReadRegWord(GAMMA_LUT1_CTRL)&GAMMA_LUT1_STEEP_CT)
                           {
                              DW_LUTAddr = INPUT_GAMMA_R_STEEP_MEM_START;
							  DW_NvramAddr = C_PRE_STEEP_START_OFFSET+ (0x2000*B_SelectedProfile); // 
                              W_size = 128;
                           }
                           else
                           {
                              //gm_Printf("Data is not uploaded: Pre Steep", 0);
                              Bp_Reply [0] = 2; // Component not uploaded
                              DW_ComponentToSave = 0; // clear the component to save flag
                              break; // stop saving                           
                           }
                        }                        
                        else
                        {                           
                           //gm_Printf("Unclear compponent R", 0);
                           Bp_Reply [0] = 3; // Component not available
                           DW_ComponentToSave = 0; // clear the component to save flag
                           break; // stop saving                           
                        }
                     }
                     else if(j == 1) // G
                     {
                        if(DW_ComponentToSave & Gamma_PostLUT_Component)
                        {
                           DW_LUTAddr = OUTPUT_GAMMA_G_MEM_START;
						   DW_NvramAddr = C_POST_LUT_START_OFFSET + (0x2000*B_SelectedProfile)+ C_ONE_CHANNEL_GAMMA_SIZE; 
                           W_size = 1024;
                        }
                        else if(DW_ComponentToSave & Gamma_PreLUT_Component)
                        {
                           DW_LUTAddr = INPUT_GAMMA_G_MEM_START;
						   DW_NvramAddr = C_PRE_LUT_START_OFFSET + (0x2000*B_SelectedProfile)+ C_ONE_CHANNEL_GAMMA_SIZE; 
                           W_size = 1024;
                        }                        
                        else if(DW_ComponentToSave & Gamma_PostSteep_Component)
                        {
                           DW_LUTAddr = OUTPUT_GAMMA_G_STEEP_MEM_START;
						   DW_NvramAddr = C_POST_STEEP_START_OFFSET + (0x2000*B_SelectedProfile)+ C_ONE_CHANNEL_GAMMA_STEEP_SIZE; 
                           W_size = 128;
                        }                        
                        else if(DW_ComponentToSave & Gamma_PreSteep_Component)
                        {
                           DW_LUTAddr = INPUT_GAMMA_G_STEEP_MEM_START;
						   DW_NvramAddr = C_PRE_STEEP_START_OFFSET + (0x2000*B_SelectedProfile)+ C_ONE_CHANNEL_GAMMA_STEEP_SIZE; 
                           W_size = 128;
                        }                        
                        else
                        {
                           //gm_Printf("Unclear Component G", 0);
                           Bp_Reply [0] = 3; // Component not available
                           DW_ComponentToSave = 0; // clear the component to save flag
                           break; // stop saving                           
                        }                     
                     }
                     else // B
                     {
                        if(DW_ComponentToSave & Gamma_PostLUT_Component)
                        {
                           DW_LUTAddr = OUTPUT_GAMMA_B_MEM_START;
						   DW_NvramAddr = C_POST_LUT_START_OFFSET + (0x2000*B_SelectedProfile)+ C_ONE_CHANNEL_GAMMA_SIZE*2; 
                           W_size = 1024;
                           //CLRBIT(DW_ComponentToSave, Gamma_PostLUT_Component);
                        }
                        else if(DW_ComponentToSave & Gamma_PreLUT_Component)
                        {
                           DW_LUTAddr = INPUT_GAMMA_B_MEM_START;
						   DW_NvramAddr = C_PRE_LUT_START_OFFSET + (0x2000*B_SelectedProfile)+ C_ONE_CHANNEL_GAMMA_SIZE*2; 
                           W_size = 1024;
                           //CLRBIT(DW_ComponentToSave, Gamma_PreLUT_Component);
                        }                        
                        else if(DW_ComponentToSave & Gamma_PostSteep_Component)
                        {
                           DW_LUTAddr = OUTPUT_GAMMA_B_STEEP_MEM_START;
						   DW_NvramAddr = C_POST_STEEP_START_OFFSET + (0x2000*B_SelectedProfile)+ C_ONE_CHANNEL_GAMMA_STEEP_SIZE*2; 
                           W_size = 128;
                           CLRBIT(DW_ComponentToSave, Gamma_PostSteep_Component);
                        }                        
                        else if(DW_ComponentToSave & Gamma_PreSteep_Component)
                        {
                           DW_LUTAddr = INPUT_GAMMA_B_STEEP_MEM_START;
						   DW_NvramAddr = C_PRE_STEEP_START_OFFSET + (0x2000*B_SelectedProfile)+ C_ONE_CHANNEL_GAMMA_STEEP_SIZE*2; 
                           W_size = 128;
                           CLRBIT(DW_ComponentToSave, Gamma_PreSteep_Component);
                        }                        
                        else
                        {
                           //gm_Printf("Unclear Component B", 0);
            			   Bp_Reply [0] = 3; // Component not available
                           DW_ComponentToSave = 0; // clear the component to save flag
                           break; // stop saving                           
                        }                     
                     }
					 
                     {                     
                        if(W_size == 128)
                        {				   
                        	for(i=0; i<128;)
                        	{
						   		B_temp[i*2] = *LINEAR_TO_FPTR_B(DW_LUTAddr);   
								B_temp[i*2+1] = *LINEAR_TO_FPTR_B(DW_LUTAddr+1);
								i+=1;
								DW_LUTAddr+=4;
                        	}
#ifdef Gamma_DUM_Initial_Check
							IntsaColor_CheckSum+=CalculateChecksum(B_temp,256);
							//gm_Print("128 B_CheckSum=%x",IntsaColor_CheckSum);
#endif
						    NVRam_WriteBuffer(DW_NvramAddr, B_temp, 256);
                        }
                        else
                        {                           
							Wp_Dest = LINEAR_TO_FPTR_B(DW_LUTAddr);    
							NVRam_WriteBuffer_DUM_Gamma(DW_NvramAddr, Wp_Dest, 1024);
#ifdef Gamma_DUM_Initial_Check
							for(i=0;i<1024;i++)
							{
								B_temp[i] = *LINEAR_TO_FPTR_B(DW_LUTAddr);
								DW_LUTAddr+=1;
							}
							IntsaColor_CheckSum+=CalculateChecksum(B_temp,1024);
							//gm_Print("1024 B_CheckSum=%x",IntsaColor_CheckSum);
#endif
                        }                                                                       
                     }
                  }

				  if(DW_ComponentToSave & Gamma_PostLUT_Component)
                  {
                     //gm_Printf(" Save Post LUT Config Registers", 0);
                     DW_NvramAddr = C_POST_LUT_CONFIG_REG_START_OFFSET+ (0x2000*B_SelectedProfile); //
                     NVRam_WriteWORD(DW_NvramAddr, gm_ReadRegWord(GAMMA_LUT2_CTRL));   
#ifdef Gamma_DUM_Initial_Check
					 B_temp[0]=gm_ReadRegWord(GAMMA_LUT2_CTRL)&0xFF;
					 B_temp[1]=gm_ReadRegWord(GAMMA_LUT2_CTRL)>>8;
#endif					 
                     DW_NvramAddr += 2;
                     DW_LUTAddr = GAMMA_LUT2_STEEP_STRT_R;            

                     for(i = 0; i < 12; i++)
                     {
#ifdef Gamma_DUM_Initial_Check
                     	B_temp[i*2+2]=gm_ReadRegWord(DW_LUTAddr)&0xFF ;
						B_temp[i*2+3]=gm_ReadRegWord(DW_LUTAddr)>>8 ;							
#endif
                        NVRam_WriteWORD(DW_NvramAddr, gm_ReadRegWord(DW_LUTAddr));
                        DW_LUTAddr += 2;
                        DW_NvramAddr += 2;            
                     }
                  
#ifdef Gamma_DUM_Initial_Check
					 IntsaColor_CheckSum+=CalculateChecksum(B_temp,26);
						//gm_Print("LUT2 B_CheckSum=%x",IntsaColor_CheckSum);
#endif
                     CLRBIT(DW_ComponentToSave, Gamma_PostLUT_Component);
                  }
                  else if(DW_ComponentToSave & Gamma_PreLUT_Component)
                  {
                     //gm_Printf(" Save Pre LUT Config Registers", 0);
                     DW_NvramAddr = C_PRE_LUT_CONFIG_REG_START_OFFSET + (0x2000*B_SelectedProfile); // 2 sector for 1 color space
                     NVRam_WriteWORD(DW_NvramAddr, gm_ReadRegWord(GAMMA_LUT1_CTRL));   
                   
#ifdef Gamma_DUM_Initial_Check
					 B_temp[0]=gm_ReadRegWord(GAMMA_LUT1_CTRL)&0xFF;
					 B_temp[1]=gm_ReadRegWord(GAMMA_LUT1_CTRL)>>8;
#endif					 
                     DW_NvramAddr += 2;
                     DW_LUTAddr = GAMMA_LUT1_STEEP_STRT_R;            

                     for(i = 0; i < 12; i++)
                     {
#ifdef Gamma_DUM_Initial_Check
						B_temp[i*2+2]=gm_ReadRegWord(DW_LUTAddr)&0xFF ;
						B_temp[i*2+3]=gm_ReadRegWord(DW_LUTAddr)>>8 ;							 
#endif
                        NVRam_WriteWORD(DW_NvramAddr, gm_ReadRegWord(DW_LUTAddr));
                        DW_LUTAddr += 2;
                        DW_NvramAddr += 2;            
                     }
                  
#ifdef Gamma_DUM_Initial_Check
					 IntsaColor_CheckSum+=CalculateChecksum(B_temp,26);
					//gm_Print("LUT1 B_CheckSum=%x",IntsaColor_CheckSum);
#endif
                     CLRBIT(DW_ComponentToSave, Gamma_PreLUT_Component);
                  }                  
               }
               else if(DW_ComponentToSave & Gamma_Matrix_Component)
               {
                  BYTE i;
                  DW_LUTAddr = GAMMA_LUT_MATRIX_MULT_COEF11; // GAMMA_LUT1_CTRL;
                  DW_NvramAddr = C_GAMMA_REGISTER_START_OFFSET+ (0x2000*B_SelectedProfile);

                  for(i = 0; i < 15; i++) // 15 registers
                  {                     
#ifdef Gamma_DUM_Initial_Check
					 B_temp[i*2]=gm_ReadRegWord(DW_LUTAddr)&0xFF ;
					 B_temp[i*2+1]=gm_ReadRegWord(DW_LUTAddr)>>8 ; 						   
#endif
                     NVRam_WriteWORD(DW_NvramAddr, gm_ReadRegWord(DW_LUTAddr));
                     DW_LUTAddr += 2;
                     DW_NvramAddr += 2;
                     
                  }
#ifdef Gamma_DUM_Initial_Check
				  IntsaColor_CheckSum+=CalculateChecksum(B_temp,30);
					//gm_Print("Matrix B_CheckSum=%x",IntsaColor_CheckSum);
#endif

                  CLRBIT(DW_ComponentToSave, Gamma_Matrix_Component);
               }
               else
               {
                  //gm_Printf(" Unclear Component ", 0);
            Bp_Reply [0] = 3; // Component not available
                  DW_ComponentToSave = 0;
               }
            }            

            // save the components saved flag once done.
            if(Bp_Reply [0] == Gamma_OK)
            {
               DW_NvramAddr = C_GAMMA_COMPONENT_SAVED_START_OFFSET + (0x2000*B_SelectedProfile); // 2 sector for 1 color space
               
               NVRam_WriteByte(DW_NvramAddr, Bp_msg[1]);               
               NVRam_WriteByte(DW_NvramAddr+1, Bp_msg[2]);
               NVRam_WriteByte(DW_NvramAddr+2, Bp_msg[3]);
               NVRam_WriteByte(DW_NvramAddr+3, Bp_msg[4]);
			   
#ifdef Gamma_DUM_Initial_Check
               NVRam_WriteByte(DW_NvramAddr+3, IntsaColor_CheckSum);
#endif
               //gm_Printf("Saved Components 0x%X", Bp_msg[1]);
            }

	#else 
			UNUSED_VAR(DW_ComponentToSave);
            Bp_Reply [0] = 3; // Component not available
	#endif //endif  SAVING_DATA_INTO_FLASH , SAVING_DATA_INTO_NVRAM

            
            *Bp_Len = 1;

            //gm_Printf("E", 0);

         }
         break;

      case Gamma_ApplyProfile:
         {
         #ifdef ST_4K2K_93xx_BOARD
         if(GetChipID_4K2K()==CHIPID_4K2K_R)
    		{
           	SMT_SendTimeoutTimetoHost(message, comm_use, SMT_TRANSACTION_TIMEOUT);
        	   ErrorCode = SMT_ForwardMessageToSlave(message, SOCIPC_DEVICEL_DST_ADDRESS);
        	}
			
			if(ErrorCode == PORT_MSG_HNDLR_OK)
			
    		#endif
		 {	
#ifdef SAVING_DATA_INTO_NVRAM
            //if(B_ReturnBusy == 0)
            {
              // extern BYTE SMT_GetTransID(MsgPacket_t * message);
               //extern SMT_TransactionStruct_t SMT_RAM * IsTransIDValid(BYTE TransID);
               //extern void SMT_SendBusy(BYTE TransID, BYTE comm_use, BYTE EvenOdd, WORD W_Delay_ms);
               
               BYTE ExpectedEvenOdd;
               BYTE B_TransactionID;
               SMT_TransactionStruct_t SMT_RAM * pCurrentTransaction;

               B_TransactionID = SMT_GetTransID(message);
               pCurrentTransaction = IsTransIDValid(B_TransactionID);
                              
               //gm_Printf("BUSY", 0);
               {
                  ExpectedEvenOdd = message->msgType & EVEN_ODD_BIT;

				#if 0 // 1
                  if(B_ODD_EVEN_OverRide)
                     if(ExpectedEvenOdd)
                        ExpectedEvenOdd = 0;
                     else
                        ExpectedEvenOdd = EVEN_ODD_BIT;
				#endif
               }
               SMT_SendBusy(pCurrentTransaction->ID,comm_use,ExpectedEvenOdd,1000);

               //B_ReturnBusy = 1;               
            }
#endif
}
            #if 1
            Bp_Reply [0] = Gamma_Apply_Profile(Bp_msg, B_SelectedProfile);
            *Bp_Len = 1;
            #else
            DWORD DW_ComponentToApply = 0;

            #ifdef SAVING_DATA_INTO_FLASH
            DWORD DW_FlashAddr = 0x5FA000; // 0x1FB000          
            #endif // SAVING_DATA_INTO_FLASH
            
            Bp_Reply [0] = Gamma_OK; // Sucess
            
            
            // Component
            msg(" Component = 0x%x ",Bp_msg[1]);
            DW_ComponentToApply = Bp_msg[1] & Gamma_Athena_Default_Component;

            // reserved byte should be 0
            if(Bp_msg[2] || Bp_msg[3] || Bp_msg[4])
            {
               gm_Printf("Invalid Component", 0);
               Bp_Reply [0] = 1; // Not Support
               DW_ComponentToApply = 0;
               break;
            }


            
            #ifdef SAVING_DATA_INTO_FLASH            
            while(DW_ComponentToApply)
            {
               DWORD DW_LUTAddr;
               
               // reset the base offset for each component
               DW_FlashAddr = 0x5FA000 - (0x2000*B_SelectedProfile); // 2 sector for 1 color space
               
               if((FlashReadWord(DW_FlashAddr) == 0xFFFF) || 
                  ((FlashReadWord(DW_FlashAddr) & DW_ComponentToApply) != DW_ComponentToApply))
               {
                  Bp_Reply [0] = 2; // Component not available
                  *Bp_Len = 1;
                  DW_ComponentToApply = 0;
                  break;
               }               

               if(DW_ComponentToApply & 0x1B) // Other than Matrix
               {
                  WORD i, W_size;
                  BYTE j;

                  for(j = 0; j < 3; j++)
                  {
                     if(j == 0) // R
                     {
                        if(DW_ComponentToApply & Gamma_PostLUT_Component)
                        {
                           DW_LUTAddr = OUTPUT_GAMMA_R_MEM_START;
                           DW_FlashAddr += POST_LUT_START_OFFSET;
                           W_size = 1024;
                        }
                        else if(DW_ComponentToApply & Gamma_PreLUT_Component)
                        {
                           DW_LUTAddr = INPUT_GAMMA_R_MEM_START;
                           DW_FlashAddr += PRE_LUT_START_OFFSET;
                           W_size = 1024;
                        }                        
                        else if(DW_ComponentToApply & Gamma_PostSteep_Component)
                        {
                           DW_LUTAddr = OUTPUT_GAMMA_R_STEEP_MEM_START;
                           DW_FlashAddr += POST_STEEP_START_OFFSET;
                           W_size = 128;
                        }                        
                        else if(DW_ComponentToApply & Gamma_PreSteep_Component)
                        {
                           DW_LUTAddr = INPUT_GAMMA_R_STEEP_MEM_START;
                           DW_FlashAddr += PRE_STEEP_START_OFFSET;
                           W_size = 128;
                        }                        
                        else
                        {
                           gm_Printf("Invalid Component R", 0);
                           Bp_Reply [0] = 3; // Component not available
                           DW_ComponentToApply = 0;
                           break; // Break the while loop
                        }
                     }
                     else if(j == 1) // G
                     {
                        if(DW_ComponentToApply & Gamma_PostLUT_Component)
                        {
                           DW_LUTAddr = OUTPUT_GAMMA_G_MEM_START;
                           W_size = 1024;
                        }
                        else if(DW_ComponentToApply & Gamma_PreLUT_Component)
                        {
                           DW_LUTAddr = INPUT_GAMMA_G_MEM_START;
                           W_size = 1024;
                        }                        
                        else if(DW_ComponentToApply & Gamma_PostSteep_Component)
                        {
                           DW_LUTAddr = OUTPUT_GAMMA_G_STEEP_MEM_START;
                           W_size = 128;
                        }                        
                        else if(DW_ComponentToApply & Gamma_PreSteep_Component)
                        {
                           DW_LUTAddr = INPUT_GAMMA_G_STEEP_MEM_START;
                           W_size = 128;
                        }                        
                        else
                        {
                           gm_Printf("Invalid Component G", 0);
                           Bp_Reply [0] = 3; // Component not available
                           DW_ComponentToApply = 0;
                           break;// break the while loop
                        }                     
                     }
                     else // B
                     {
                        if(DW_ComponentToApply & Gamma_PostLUT_Component)
                        {
                           DW_LUTAddr = OUTPUT_GAMMA_B_MEM_START;
                           W_size = 1024;
                           CLRBIT(DW_ComponentToApply, Gamma_PostLUT_Component);
                        }
                        else if(DW_ComponentToApply & Gamma_PreLUT_Component)
                        {
                           DW_LUTAddr = INPUT_GAMMA_B_MEM_START;
                           W_size = 1024;
                           CLRBIT(DW_ComponentToApply, Gamma_PreLUT_Component);
                        }                        
                        else if(DW_ComponentToApply & Gamma_PostSteep_Component)
                        {
                           DW_LUTAddr = OUTPUT_GAMMA_B_STEEP_MEM_START;
                           W_size = 128;
                           CLRBIT(DW_ComponentToApply, Gamma_PostSteep_Component);
                        }                        
                        else if(DW_ComponentToApply & Gamma_PreSteep_Component)
                        {
                           DW_LUTAddr = INPUT_GAMMA_B_STEEP_MEM_START;
                           W_size = 128;
                           CLRBIT(DW_ComponentToApply, Gamma_PreSteep_Component);
                        }                        
                        else
                        {
                           gm_Printf("Invalid Component B", 0);
                           Bp_Reply [0] = 3; // Component not available
                           DW_ComponentToApply = 0;
                           break; // break the while loop
                        }                     
                     }
                                       
                     for(i = 0; i < W_size; i++)
                     {                     
                        if(W_size == 128)
                        {                           
                           *LINEAR_TO_FPTR_W(DW_LUTAddr) = FlashReadWord(DW_FlashAddr);
                           //FlashWriteWord(DW_FlashAddr, *LINEAR_TO_FPTR_W(DW_LUTAddr));
                           DW_LUTAddr += 4;
                           DW_FlashAddr += 2;
                        }
                        else
                        {                           
                           *LINEAR_TO_FPTR_B(DW_LUTAddr) = FlashReadByte(DW_FlashAddr);
                           //FlashWriteByte(DW_FlashAddr, *LINEAR_TO_FPTR_B(DW_LUTAddr));
                           DW_LUTAddr += 1;
                           DW_FlashAddr += 1;
                        }                                                                       
                     }
                  }
               }
               else if(DW_ComponentToApply & Gamma_Matrix_Component)
               {
                  BYTE i;
                  DW_LUTAddr = GAMMA_LUT1_CTRL;
                  DW_FlashAddr += GAMMA_REGISTER_START_OFFSET;

                  for(i = 0; i < 41; i++) // 41 registers
                  {                     
                     //FlashWriteWord(DW_FlashAddr, gm_ReadRegWord(DW_LUTAddr));
                     gm_WriteRegWord(DW_LUTAddr, FlashReadWord(DW_FlashAddr));
                     DW_LUTAddr += 2;
                     DW_FlashAddr += 2;
                     
                  }
                  CLRBIT(DW_ComponentToApply, Gamma_Matrix_Component);
               }
               else
               {
                  gm_Printf(" Invalid Input Parameter ", 0);
                  Bp_Reply [0] = 3; // Component not available
                  DW_ComponentToApply = 0;
                  break; // break the while loop
               }
            }            
            #else // SAVING_DATA_INTO_FLASH
            Bp_Reply [0] = 3; // Component not available
            #endif // SAVING_DATA_INTO_FLASH


            
            *Bp_Len = 1;
#endif

         }
         break;

      case Gamma_EraseProfile:
         #ifdef ST_4K2K_93xx_BOARD
         if(GetChipID_4K2K()==CHIPID_4K2K_R)
    		{
        	   SMT_SendTimeoutTimetoHost(message, comm_use, 2*SMT_TRANSACTION_TIMEOUT);
        	   ErrorCode = SMT_ForwardMessageToSlave(message, SOCIPC_DEVICEL_DST_ADDRESS);
       	}

			if(ErrorCode == PORT_MSG_HNDLR_OK)
    		#endif
         {            
            #if 1
            #ifdef SAVING_DATA_INTO_FLASH
            #if 0
            if(B_ReturnBusy)
            {
               msg("BUSY", 0);
               return PORT_MSG_HNDLR_BUSY;
            }
            #endif

            //if(B_ReturnBusy == 0)
            {
               //extern BYTE SMT_GetTransID(MsgPacket_t * message);
               //extern SMT_TransactionStruct_t SMT_RAM * IsTransIDValid(BYTE TransID);
               //extern void SMT_SendBusy(BYTE TransID, BYTE comm_use, BYTE EvenOdd, WORD W_Delay_ms);
               
               BYTE ExpectedEvenOdd;
               BYTE B_TransactionID;
               SMT_TransactionStruct_t SMT_RAM * pCurrentTransaction;

               B_TransactionID = SMT_GetTransID(message);
               pCurrentTransaction = IsTransIDValid(B_TransactionID);
                              
               //gm_Printf("BUSY", 0);
               {
                  ExpectedEvenOdd = message->msgType & EVEN_ODD_BIT;

                  #if 0 // 1
                  if(B_ODD_EVEN_OverRide)
                     if(ExpectedEvenOdd)
                        ExpectedEvenOdd = 0;
                     else
                        ExpectedEvenOdd = EVEN_ODD_BIT;
                  #endif
               }
               //SMT_SendBusy(pCurrentTransaction->ID,comm_use,ExpectedEvenOdd,500);
               SMT_SendBusy(pCurrentTransaction->ID,comm_use,ExpectedEvenOdd,1500); // add for re-load flash data

               //B_ReturnBusy = 1;               
            }            
            #endif
            if(Bp_msg[1])
            Bp_Reply [0] = Gamma_Erase_Profile(Bp_msg[1] - 1);
            else
               Bp_Reply [0] = 1; // Invalid Profile ID
            #else
            BYTE B_ProfileIDtoErase = 0;
            // Profile ID
            msg(" Profile ID = %d",Bp_msg[1]);
            B_ProfileIDtoErase = Bp_msg[1] - 1; // Tool index range is 1 ~ 255
            

            #ifdef SAVING_DATA_INTO_FLASH
            if(B_ProfileIDtoErase < Gamma_MaxProfile)
            { // Clear Index All components ...
               DWORD DW_FlashAddr = 0x5FA000; // 0x1FB000

               DW_FlashAddr = DW_FlashAddr - (0x2000*B_ProfileIDtoErase); // 2 sector for 1 color space

               Flash_EraseSector(DW_FlashAddr, 2, SECT_ER_DEST_INCLUDED);
               Bp_Reply [0] = Gamma_OK; // Sucess
            }
            else
            {
               gm_Printf("Invalid Profile ID to erase", 0);
               Bp_Reply [0] = 1; // Invalid Profile ID
            }            
            #else // SAVING_DATA_INTO_FLASH
            Bp_Reply [0] = 3; // Component not available
            #endif // SAVING_DATA_INTO_FLASH
            #endif
            
            *Bp_Len = 1;

            #ifdef SAVING_DATA_INTO_FLASH
            #if 1 // reload the flash data ... in test it taks around 10xxms
            //gm_Printf("Erase", 0);
            //gm_MemoryCodeStop();
            gm_MemoryCodeAdjust(DATA_DRAM_SIZE);
            gm_MemoryCodeStart();
            //gm_Printf("1 gm_MemoryCodeStop %d", gm_GetSystemTime());
            gm_MemoryCodeDownload();
            //gm_Printf("2 gm_MemoryCodeDownload %d", gm_GetSystemTime());
            gm_MemoryCodeAdjust(DATA_DRAM_SIZE+CODE_SIZE);
            gm_MemoryCodeStart();               
            #endif
            #endif          
         }
         break;         
#endif

      default:

         // wrong command code received
         Bp_Reply[0] = Gamma_FormatNotSupported;
   }

   //store current command ID in the context.
   p_Context->CurrentCommand = B_Command;

   gm_WriteRegWord(HOST_CONTROL,ODP_SYNC_UPDATE);

   return ErrorCode;
}

#endif //USE_SMT
//*********************************  END  **************************************
