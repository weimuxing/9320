/*
	$Workfile:	 fsb_osd_verification.c  $
	$Revision: 1.21 $
	$Date: 2013/06/11 06:33:39 $
*/
//#define __FSB_OSD_VERIFICATION_C__
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
// MODULE:      fsb_osd_verification.c
//
// USAGE:       FSB and OSD verification in Cortez.
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include <mem.h>
#include <embedded.h>
#include <stdlib.h>
#include "System\all.h"
#include "StdLibInc\gm_Mcu186.h"
#include "system\Sdram.h"
#include "TestHarness\AppsTest.h"

#if (FEATURE_DDR_EXECUTION == ENABLE)

//#ifdef __FSB_OSD_VERIFICATION__C__
#ifdef FSB_OSD_VERIFICATION

//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************

//#define STRESS_TEST_WRITE_THAN_READ
#define STRESS_TEST_WRITE_READ


#define WaitFSB_WAIT_DMA() while(READ_PCB_REG(D0TC)) {}

#define WaitFSB_RD_XFER() while((gm_ReadRegWord(RD_SDRAM_CONTROL) & RD_XFER_IN_PROGRESS)) {}

#define WaitFSB_WR_XFER() while((gm_ReadRegWord(WR_SDRAM_CONTROL) & WR_XFER_IN_PROGRESS)) {}

#define WaitFSB_RD_EMPTY() while((gm_ReadRegWord(RD_SDRAM_CONTROL) & RD_SDRAM_FIFO_EMPTY)) {}

#define WaitFSB_WR_FULL() while((gm_ReadRegWord(WR_SDRAM_CONTROL) & WR_SDRAM_FIFO_FULL)) {}

WORD const far *gWp_PatID;	// DDR_TEST_CODE

//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************


//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************

//extern far FPT_OsdCalculateSdramParams gFp_OsdCalculateSdramParams;
extern WORD const far Wa_RandomPattern[];
extern WORD const far Wa_RandomPattern_STLogo[];
extern WORD const far Wa_RandomPattern1[];
extern WORD const far Wa_FixedValue[];


//******************************************************************************
//  S T A T I C    V A R I A B L E S
//******************************************************************************

//******************************************************************************
//  S T A T I C    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
void MemIFTest_BD(BYTE B_DlyType, BYTE B_NumRetry, BYTE B_Opt, BYTE B_TestedBit);
void MemIFTest_ByteDly(BYTE B_DlyType, BYTE B_NumRetry, BYTE B_Opt);
gmt_RET_STAT MemAccess(BYTE B_Type, BYTE B_NumRetry, BYTE B_Opt, BYTE B_DataBit);


//******************************************************************************
//  C O D E
//******************************************************************************

#define START_TILE_POSITION        0x00000000L

#define RANGE		768
#define WRITE_HEIGHT	656
#define WRITE_WIDTH             ((RANGE + 7) / 8)
#define STRIDE                     (RANGE / 8)
#define NUMBER_OF_ADDRESS_INC     64
#define READ_ATTEMPTS             3
#define WRITE_ATTEMPTS             3
#define ERROR_LIMIT             10

// tiles background colours
BYTE const far TilesBgndColorLut[] =
{
   0, 0, 0, 0,
   255, 0, 255, 255,
};

ST_OSD_TILE const far Tile2 =
{
   0,
   0,
   RANGE,
   WRITE_HEIGHT,	// RANGE,
   OSD_COLOR2,		// OSD_COLOR256,
   0x001,			// 0x000,
   START_TILE_POSITION
};


//******************************************************************************
//
// FUNCTION       : gmt_RET_STAT TestFsbReadWrite(BYTE B_Mode, BYTE B_Interface,
//                                                  BYTE B_AddrNumberInc, BYTE B_AddrNmberIncStep,
//                                                  BYTE B_WriteWidth, BYTE B_WriteWidthStep,
//                                                  WORD W_WriteHeight, WORD W_WriteHeightStep)
//
// USAGE          : To verify the FS Bridge interface OCM < - > DMA < - > DDR SDRAM
//                  If B_Interface == 1 to 5 the following tests are performed.
//                     if (B_Interface == 1) do "AppsTest 105 x 0 64 32 82 40 768 128" (execution time 1s)
//                     if (B_Interface == 2) do "AppsTest 105 x 0 64  4 82 20 768  64" (execution time 10s)
//                     if (B_Interface == 3) do "AppsTest 105 x 0 64 16 82  8 768  64" (execution time 25s)
//                     if (B_Interface == 4) do "AppsTest 105 x 0 64  4 82  4 768  64" (execution time 4mins)
//                     if (B_Interface == 5) do "AppsTest 105 x 0 64  2 82  2 768   1" (execution time 4hrs)
//                  B_Interface == 6 to 8 are used to perform AppsTest 112
//
// INPUT          : B_Mode - 0 - OCM, 1- DMA
//                  B_Interface - 0 - set of parameters, 1 ... n - predefined tests
//                  B_AddrNumberInc - default 64, (FSB burst size)
//                  B_AddrNmberIncStep - set ratio how data devided between bursts, 2,4,...,32
//                  B_WriteWidth - default 82
//                  B_WriteWidthStep - 2..41
//                  W_WriteHeight - default 768
//                  W_WriteHeightStep - 1, 2, 4, ..., 380
//
//
// OUTPUT         :
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************

void TurnOnFsbTesOSD(void)
{
   //DWORD D_SdramAddr = Tile2.D_BmpAddr << 6;

   gm_OsdLoadClutByDma(0, 2, (WORD far *)TilesBgndColorLut);
   gm_SetSelectedTileset((gm_GetSelectedTileset()) ? 0 : 1);
   gm_OsdDisableTile(0);
   gm_OsdSetTile(0, (ST_OSD_TILE const far *)&Tile2, 1);
   gm_OsdShow();
}

void TurnOffFsbTesOSD(void)
{
   gm_OsdHide();
}

gmt_RET_STAT TestFsbReadWrite(BYTE B_Mode, DWORD DW_TestAddr, WORD B_WriteWidth, WORD W_WriteHeight, BYTE B_Option, BYTE B_TestedBit)
{
   WORD W_Status;
   WORD W_regheight, W_ReadHeight;
   WORD W_tmp1, W_tmp2, W_LoopCnt;
   WORD k, k2;
   WORD *Wp_read;
   WORD const far *Wp_pattern;
   WORD read_counter, W_DataID;
   WORD W_ErrorCounter = 0;
   DWORD D_WriteCounter = 0;
   WORD const far *Wp_PatID;
   BYTE B_MaskType, B_MaskShift, B_DoCmp;
   WORD W_Mask;
	DWORD DW_Addr;

   // B_Option:
   //	Bit[5]: Write data into DDR
   //	Bit[3]: Read data from DDR
   //	Bit[1]: Print message if there is error
   //	Bit[0]: Print message anyway

   Wp_PatID = gWp_PatID;

	DW_Addr = DW_TestAddr;
   //-------------------------------------------------------------------------
   // In that test after each write-function read-back function is called
   //-------------------------------------------------------------------------
	if ((B_Option & BIT5) != BIT5)
	{
	   W_Status = gm_SdramWrite_HA(DW_Addr, STRIDE, B_WriteWidth, W_WriteHeight, 0, (WORD far *)Wp_PatID, NULL_PTR, 0, B_WriteWidth);

		D_WriteCounter++;

		if (W_Status != OK)
			gm_Print("gm_SdramWrite_HA failed. W_Status = 0x%x", W_Status);

		if ((W_Status = gm_ReadRegWord(WR_SDRAM_CONTROL)) & (WR_FIFO_SM_ERROR|WR_FIFO_OF))
		{
			gm_WriteRegWord(WR_SDRAM_CONTROL, WR_FIFO_SM_ERROR|WR_FIFO_OF);
			W_ErrorCounter++;
		}
	}

	if ((B_Option & BIT3) != BIT3)
	{
		W_DataID = 0;
		W_regheight = sizeof(TempBuffer) / B_WriteWidth;
		W_tmp1 = W_WriteHeight / W_regheight;
		W_tmp2 = W_WriteHeight % W_regheight;
		W_LoopCnt = W_tmp1;
		if (W_tmp2 > 0)
			W_LoopCnt++;

		Wp_pattern = Wp_PatID;

		for (k = 0 ; k < W_LoopCnt ; k++)
		{
			W_ReadHeight = W_regheight;
			if (k == W_tmp1)
				W_ReadHeight = W_tmp2;

			memset(TempBuffer, 0xff, sizeof(TempBuffer));
		   W_Status = gm_SdramRead_HA(DW_Addr + (DWORD)k * W_regheight * STRIDE, STRIDE, B_WriteWidth, W_ReadHeight, 0, (WORD *)TempBuffer);

		   if (W_Status != OK)
			{
				gm_Print("gm_SdramRead_HA failed. W_Status = 0x%x", W_Status);
				W_ErrorCounter++;
			}
			else
			{
				if ((W_Status = gm_ReadRegWord(RD_SDRAM_CONTROL)) & (RD_FIFO_UF|RD_FIFO_SM_ERROR))
				{
					gm_WriteRegWord(RD_SDRAM_CONTROL, RD_FIFO_UF|RD_FIFO_SM_ERROR);
					gm_Print("RD_FIFO_UF or RD_FIFO_SM_ERROR", 0);
					W_ErrorCounter++;
				}
				else
				{
	            Wp_read = (WORD *)TempBuffer;
	            read_counter = 0;
					for (k2 = (B_WriteWidth * W_ReadHeight) / 2 ; k2 ; k2--)
		         {
		         	#if 1	// 20130603
						// Not show anymore
						#else
		            if ((B_Option & BIT0) == BIT0)
		            {
		               gm_Print("k = %d", k);
		               gm_Print("k2 = %d", k2);
		               gm_Print("read_counter = %d", read_counter);
		               gm_Print("Write = 0x%x", *Wp_pattern);
		               gm_Print("Read = 0x%x", *Wp_read);
		            }
						#endif

		            B_MaskType = B_TestedBit / 16;
		            B_MaskShift = B_TestedBit % 16;
		            B_DoCmp = FALSE;
		            if (B_TestedBit < 32)
		            {
		               if (B_MaskType == 0)
		               {
		                  if (((read_counter % 4) == 2) || ((read_counter % 4) == 3))
		                  {
		                     B_DoCmp = TRUE;
		                  }
		               }
		               else
		               {
		                  if (((read_counter % 4) == 0) || ((read_counter % 4) == 1))
		                  {
		                     B_DoCmp = TRUE;
		                  }
		               }
							#if 0 // DBG_20130417		20130523
							// Check whole Byte data. Need to think about how to differentiate with checking one bit only later.
		               W_Mask = (0xFF << B_MaskShift);
							#else
		               W_Mask = (1 << B_MaskShift);
							#endif
						}
	               else
	               {
	                  B_DoCmp = TRUE;
	                  W_Mask = 0xFFFF;
	               }

	               if (B_DoCmp == TRUE)
	               {
	                  if ((*Wp_read & W_Mask) != (*Wp_pattern & W_Mask))
	                  {
	                     W_ErrorCounter++;
	                     if ((B_Option & BIT1) == BIT1)
	                     {
									if (B_TestedBit < 32)
		                        gm_Print("Checked bit is Bit[%d]", (WORD)B_TestedBit);

									gm_Print("W_DataID = %d", W_DataID);
									gm_Print("read_counter = %d", read_counter);
	                        gm_Print("Write = 0x%x", *Wp_pattern);
	                        gm_Print("Read = 0x%x", *Wp_read);
	                     }
	                  }
	               }

	               W_DataID++;
	               read_counter++;
	               Wp_read++;
	               Wp_pattern++;
	            }
	         }
			}
   	}
   }

   if ((B_Option & BIT0) == BIT0)
   {
      if (W_ErrorCounter != 0)
      {
         gm_Print("Number of Words = %d", read_counter);
         gm_Print("Number of error Words are detected = %d", W_ErrorCounter);
      }
   }

   UNUSED_VAR(B_Mode);
   
   if (W_ErrorCounter == 0)
      return OK;
   else
      return ERR_WRITE;
}

void SetDdrPhyDly_Handler(BYTE B_DlyType, BYTE B_ByteID, BYTE B_BitID, BYTE B_DlyVal)
{
   DisableVTUpdate();
   SetDdrPhyDly(B_DlyType, B_ByteID, B_BitID, B_DlyVal);
   EnableVTUpdate();
}

void SetDataPattern(BYTE B_PatLut)
{
   BYTE B_Tmp;

   B_Tmp = ((B_PatLut & (BIT7 | BIT6)) >> 6);

   switch (B_Tmp)
   {
      case 0:
         gWp_PatID = Wa_RandomPattern;
         break;

      case 1:
         gWp_PatID = Wa_RandomPattern1;
         break;

      case 2:
         gWp_PatID = Wa_RandomPattern_STLogo;
         break;

      case 3:
         gWp_PatID = Wa_FixedValue;
         break;
   }
}

void DisableVTUpdate(void)
{
   DWORD DW_RegVal;

   DW_RegVal = PCTL_ReadReg(PCTL32_PHY_PIR);
   PCTL_WriteReg(PCTL32_PHY_PIR, (DW_RegVal | BIT28));
   gm_Delay1ms(1);
}

void EnableVTUpdate(void)
{
   DWORD DW_RegVal;

   DW_RegVal = PCTL_ReadReg(PCTL32_PHY_PIR);
   PCTL_WriteReg(PCTL32_PHY_PIR, (DW_RegVal & ~BIT28));
}

void MemIFTest(BYTE B_DlyType, BYTE B_NumRetry, BYTE B_Opt, BYTE B_TestedBit)
{
   BYTE B_FwStayInFlash, B_Tmp;
	WORD W_Tmp;
   DWORD DW_Tmp;

   DisableVTUpdate();

   switch (B_DlyType)
   {
      case DLY_QUALIFY:
         TurnOnFsbTesOSD();

         gm_Print("Start MemIFQualify (x%d)", (WORD)B_NumRetry);

			W_Tmp = gm_ReadRegWord(CTL_PLL_DDR_FREQ);

         gm_Printf("(0x%x) (0x%x) (0x%x) (2T: %d) (Double: %d) (0x%x)", 
							(WORD)((BYTE)(VAL_PCTL32_PHY_ZQ0CR >> 20)), 
							FineTune_WDQD, FineTune_RDQSD, Enable_2T, Enable_DRR, W_Tmp);
			
			W_Tmp = (stInput[0].Vfrq + 5) / 10;
			gm_Printf("Main: %dx%d %dHz", stInput[0].Hactive, stInput[0].Vactive, W_Tmp);

			#if 0
         DW_Tmp = PCTL_ReadReg(PCTL32_PHY_ZQ0CR);
         gm_Print("ZQ0CR_hi = 0x%x", (WORD)(DW_Tmp >> 16));
         gm_Print("ZQ0CR_lo = 0x%x", (WORD)DW_Tmp);
			#endif

         MemIFTest_ByteDly(DLY_WDQD, B_NumRetry, B_Opt);
         gm_MemoryCodeStop();
         MemIFTest_ByteDly(DLY_RDQSD, B_NumRetry, B_Opt);

         gm_Print("End of MemIFQualify", 0);
         gm_Print(" ", 0);

         DdrTest_LongHours(10);

         TurnOffFsbTesOSD();
         break;
			
		case DLY_DataBits:
			TurnOnFsbTesOSD();

			gm_Printf("Start MemIFQualify - Data Bits", 0);

			MemIFTest_BD(DLY_WBD, B_NumRetry, B_Opt, 32);
			gm_MemoryCodeStop();
			MemIFTest_BD(DLY_RBD, B_NumRetry, B_Opt, 32);

			gm_Print("End of MemIFQualify ......", 0);
			gm_Print(" ", 0);

			DdrTest_LongHours(1);

			TurnOffFsbTesOSD();
			break;
   }

   EnableVTUpdate();

#ifdef DRAM_CODE_EXECUTION
   if ((B_Opt & BIT6) != BIT6)
      gm_MemoryCodeStart();
#endif

   UNUSED_VAR(B_TestedBit);
}

#define NUM_DataBytes	4
#define NUM_DataBits	32

#define MAX_NumDlySteps		64
#define NUM_CharA			(MAX_NumDlySteps + 25)
#define NUM_CharB			(MAX_NumDlySteps + 25 - 3)
#define NUM_CharC			(MAX_NumDlySteps + 24)

#if 1	// Validation time saving
#define NumByte_Width	64	// ST Logo
#define NumByte_Height	480	// ST Logo
#else
#define NumByte_Width	(RANGE / 8)
#define NumByte_Height	WRITE_HEIGHT
#endif

BYTE dec2hex(BYTE B_Decimal)
{
   if (B_Decimal < 10) return (B_Decimal + '0');
   else return (B_Decimal + 'A' - 10);
}

void MemIFTest_BD(BYTE B_DlyType, BYTE B_NumRetry, BYTE B_Opt, BYTE B_TestedBit)
{
   BYTE B_DdrRW, B_Byte, B_Bit, B_Delay, B_DataBit, B_Idx;
   BYTE B_Min, B_Max, B_OriVal, B_Best[2][NUM_DataBits], B_NoErr;
   BYTE B_ByteStart, B_ByteEnd, B_BitStart, B_BitEnd;
   BYTE B_NumSteps = 32;
   char B_Result[NUM_CharA + 2];

   gm_Printf("B_DlyType = %d", (WORD)B_DlyType);

   B_DdrRW = 0;
   if (B_DlyType == DLY_RBD)
      B_DdrRW = 1;

   if ((B_Opt & BIT2) == BIT2)
      B_NumSteps = 64;

   if (B_TestedBit < 32)
   {
      B_ByteStart = B_TestedBit >> 3;
      B_ByteEnd = B_ByteStart + 1;
      B_BitStart = B_TestedBit % 8;
      B_BitEnd = B_BitStart + 1;
   }
   else
   {
      B_ByteStart = 0;
      B_ByteEnd = NUM_DATAx8;
      B_BitStart = 0;
      B_BitEnd = 8;
   }

   for (B_Byte = B_ByteStart; B_Byte < B_ByteEnd; B_Byte++)
   {
      gm_Printf("Datax8_%d", (WORD)B_Byte);
      for (B_Bit = B_BitStart; B_Bit < B_BitEnd; B_Bit++)
      {
         B_OriVal = GetDdrPhyDly(B_DlyType, B_Byte, B_Bit);

         B_DataBit = (B_Byte << 3) + B_Bit;
         B_Min = B_NumSteps - 1;
         B_Max = 0;

         B_Idx = 0;
         B_Result[B_Idx++] = '[';
         B_Result[B_Idx++] = B_Byte + 0x30;
         B_Result[B_Idx++] = ']';
         B_Result[B_Idx++] = '[';
         B_Result[B_Idx++] = B_Bit + 0x30;
         B_Result[B_Idx++] = ']';
         B_Result[B_Idx++] = ' ';
         for (B_Delay = 0; B_Delay < B_NumSteps; B_Delay++)
         {
            SetDdrPhyDly(B_DlyType, B_Byte, B_Bit, B_Delay);

            B_NoErr = MemAccess(B_DdrRW, B_NumRetry, B_Opt, B_DataBit);

            if (B_NoErr == TRUE)
            {
               if (B_Delay == B_OriVal)
                  B_Result[B_Idx++] = 'x';
               else
                  B_Result[B_Idx++] = '0';

               if (B_TestedBit < 32)
                  gm_Printf("0", 0);

               if (B_Delay < B_Min)
                  B_Min = B_Delay;

               if (B_Delay > B_Max)
                  B_Max = B_Delay;
            }
            else
            {
               if (B_Delay == B_OriVal)
                  B_Result[B_Idx++] = '!';
               else
                  B_Result[B_Idx++] = '1';

               if (B_TestedBit < 32)
                  gm_Printf("1", 0);
            }
         }

         B_Result[B_Idx++] = ' ';
         B_Result[B_Idx++] = '(';
         B_Result[B_Idx++] = 'x';
         B_Result[B_Idx++] = dec2hex(B_Min >> 4);
         B_Result[B_Idx++] = dec2hex(B_Min & 0x0F);
         B_Result[B_Idx++] = ' ';
         B_Result[B_Idx++] = '-';
         B_Result[B_Idx++] = ' ';
         B_Result[B_Idx++] = 'x';
         B_Result[B_Idx++] = dec2hex(B_OriVal >> 4);
         B_Result[B_Idx++] = dec2hex(B_OriVal & 0x0F);
         B_Result[B_Idx++] = ' ';
         B_Result[B_Idx++] = '-';
         B_Result[B_Idx++] = ' ';
         B_Result[B_Idx++] = 'x';
         B_Result[B_Idx++] = dec2hex(B_Max >> 4);
         B_Result[B_Idx++] = dec2hex(B_Max & 0x0F);
         B_Result[B_Idx++] = ')';

         B_Result[B_Idx++] = 0; // EOS
         B_Result[B_Idx++] = 0; // EOS
         //gm_Printf("[%s] %d",(char*)B_Result, (WORD)B_TestedBit);
         gm_Printf((char*)B_Result, 0);

         B_Best[0][B_DataBit] = ((B_Max + B_Min) >> 1);
         SetDdrPhyDly(B_DlyType, B_Byte, B_Bit, B_OriVal);
      }
   }
}

void MemIFTest_ByteDly(BYTE B_DlyType, BYTE B_NumRetry, BYTE B_Opt)
{
   BYTE B_Delay, B_Byte, B_TestedBit, B_DdrRW, B_Idx;
   BYTE B_Min, B_Max, B_OriVal, B_NoErr;
   BYTE B_NumSteps;
   char B_Result[NUM_CharB + 2];
	BYTE B_TurnOffFIFOs, B_Loops;
	WORD W_Tmp1, W_Tmp2;
	BYTE B_State, B_StableCnt, B_Skip;

   if (B_DlyType == DLY_WDQD)
	   gm_Print("Write DQS", 0);

   if (B_DlyType == DLY_RDQSD)
	   gm_Print("Read DQS", 0);

	#if 0 // (DDR_TEST_CODE == ENABLE)	// 20130430 Optimization
	B_NumSteps = gB_DdrDbg;
	if (B_NumSteps > 64)
		B_NumSteps = 64;
	#else
	B_NumSteps = 64;
	#endif

   B_DdrRW = 0;	// Delay for DDR Write
   if (B_DlyType == DLY_RDQSD)
   {
      B_DdrRW = 1;	// Delay for DDR Read

		B_TurnOffFIFOs = 1;

		W_Tmp1 = gm_ReadRegWord(MC_FIFO_EN_hi);
		W_Tmp2 = gm_ReadRegWord(MC_FIFO_EN_lo);
   }

   for (B_Byte = 0; B_Byte < NUM_DATAx8; B_Byte++)
   {
   	B_State = 0;
		B_StableCnt = 0;
		B_Skip = 0;
		B_Max = 0xFF;
		
      B_OriVal = GetDdrPhyDly(B_DlyType, B_Byte, 0);

      B_TestedBit = (B_Byte << 3);	// Needs to check whole byte instead of one bit.
      #if 0
      B_Min = B_NumSteps - 1;
      B_Max = 0;
		#endif

      B_Idx = 0;
      B_Result[B_Idx++] = '[';
      B_Result[B_Idx++] = B_Byte + '0';
      B_Result[B_Idx++] = ']';
      B_Result[B_Idx++] = ' ';
      for (B_Delay = 0; B_Delay < B_NumSteps; B_Delay++)
      {
      	if (B_Skip == 0)
   		{
				if (B_TurnOffFIFOs == 1)
				{
					gm_WriteRegWord(MC_FIFO_EN_hi, 0x0000);
					gm_WriteRegWord(MC_FIFO_EN_lo, 0x0000);

					if (B_Delay == 0)
					{
						gm_Delay1us(1000);
					}
				}

	         SetDdrPhyDly(B_DlyType, B_Byte, 0, B_Delay);

				if (B_TurnOffFIFOs == 1)
				{
					gm_WriteRegWord(MC_FIFO_EN_hi, W_Tmp1);
					gm_WriteRegWord(MC_FIFO_EN_lo, W_Tmp2);
				}

				for (B_Loops = 0; B_Loops < B_NumRetry; B_Loops++)
				{
					B_NoErr = MemAccess(B_DdrRW, 1, B_Opt, B_TestedBit);
					if (B_NoErr != TRUE)
						break;
				}

				switch (B_State)
				{
					case 0:
						if (B_NoErr != TRUE)	// Err is detected
						{
							B_StableCnt = 0;
						}
						else
						{
							B_StableCnt++;
							if (B_StableCnt == 1)
							{
								B_Min = B_Delay;
							}
						}
						break;

					case 1:
						if (B_NoErr != TRUE)	// Err is detected
						{
							B_StableCnt++;
							if (B_Max == 0xFF)
							{
								B_Max = B_Delay;
							}
						}
						else
						{
							B_StableCnt = 0;
						}
						break;
				}

				if (B_StableCnt > 3)
				{
					B_State++;
					B_StableCnt = 0;
				}

				if (B_State >= 2)
					B_Skip = 1;

	         if (B_NoErr == TRUE)
	         {
	            if (B_Delay == B_OriVal)
	               B_Result[B_Idx++] = 'x';
	            else
	               B_Result[B_Idx++] = '0';

					#if 0
	            if (B_Delay < B_Min)
	               B_Min = B_Delay;

	            if (B_Delay > B_Max)
	               B_Max = B_Delay;
					#endif
	         }
	         else
	         {
	            if (B_Delay == B_OriVal)
	               B_Result[B_Idx++] = '!';
	            else
	               B_Result[B_Idx++] = '1';
	         }
      	}
			else
			{
				B_Result[B_Idx++] = 'S';
			}

      }

      B_Result[B_Idx++] = ' ';
      B_Result[B_Idx++] = '(';
      B_Result[B_Idx++] = 'x';
      B_Result[B_Idx++] = dec2hex(B_Min >> 4);
      B_Result[B_Idx++] = dec2hex(B_Min & 0x0F);
      B_Result[B_Idx++] = ' ';
      B_Result[B_Idx++] = '-';
      B_Result[B_Idx++] = ' ';
      B_Result[B_Idx++] = 'x';
      B_Result[B_Idx++] = dec2hex(B_OriVal >> 4);
      B_Result[B_Idx++] = dec2hex(B_OriVal & 0x0F);
      B_Result[B_Idx++] = ' ';
      B_Result[B_Idx++] = '-';
      B_Result[B_Idx++] = ' ';
      B_Result[B_Idx++] = 'x';
      B_Result[B_Idx++] = dec2hex(B_Max >> 4);
      B_Result[B_Idx++] = dec2hex(B_Max & 0x0F);
      B_Result[B_Idx++] = ')';

      B_Result[B_Idx++] = 0; // EOS
      B_Result[B_Idx++] = 0; // EOS
      //gm_Printf("[%s] %d",(char*)B_Result, (WORD)B_TestedBit);
      gm_Printf((char*)B_Result, 0);

		if (B_TurnOffFIFOs == 1)
		{
			gm_WriteRegWord(MC_FIFO_EN_hi, 0x0000);
			gm_WriteRegWord(MC_FIFO_EN_lo, 0x0000);
		
			gm_Delay1us(1000);
		}

		SetDdrPhyDly(B_DlyType, B_Byte, 0, B_OriVal);

		if (B_TurnOffFIFOs == 1)
		{
			gm_WriteRegWord(MC_FIFO_EN_hi, W_Tmp1);
			gm_WriteRegWord(MC_FIFO_EN_lo, W_Tmp2);
		}

   }
}

gmt_RET_STAT MemAccess(BYTE B_Type, BYTE B_NumRetry, BYTE B_Opt, BYTE B_DataBit)
{
   BYTE i, j;
   BYTE B_NoErr;
   WORD W_Width, W_Height;

   B_NoErr = TRUE;

   W_Width = NumByte_Width;
   W_Height = NumByte_Height;
   if ((B_Opt & BIT4) == BIT4)
   {
      W_Width = 16;
      W_Height = 16;
   }

   if (B_Type == 0) // Writing
   {
      for (i = 0; i < B_NumRetry; i++)
      {
         for (j = 0; j < 4; j++)
         {
            SetDataPattern(j << 6);
            if (TestFsbReadWrite(0, 0, W_Width, W_Height, B_Opt, B_DataBit) != OK)
               B_NoErr = FALSE;
         }
      }
   }
   else if (B_Type == 1) // Reading
   {
      for (j = 0; j < 4; j++)
      {
         SetDataPattern(j << 6);
         if (TestFsbReadWrite(0, 0, W_Width, W_Height, B_Opt, 32) != OK)
         {
            // Err was observed during writing
         }

         for (i = 0; i < B_NumRetry; i++)
         {
            if (TestFsbReadWrite(0, 0, W_Width, W_Height, (B_Opt | BIT5), B_DataBit) != OK)
               B_NoErr = FALSE;
         }
      }
   }

   return B_NoErr;
}

void DdrTest_DataBit(BYTE B_TestedBit, BYTE B_Opt, BYTE B_NumAccess)
{
   BYTE i, B_NoErr;
   BYTE B_Start, B_End;

   if (B_TestedBit < 32)
   {
      B_Start = B_TestedBit;
      B_End = B_Start + 1;
   }
   else
   {
      B_Start = 0;
      B_End = 32;
   }

   gm_Print("DDR per bit checking ...", 0);

   for (i = B_Start; i < B_End; i++)
   {
      B_NoErr = MemAccess(1, B_NumAccess, B_Opt, i);

      if (B_NoErr == FALSE)
         gm_Print("Bit[%d]: Errors observed.", i);
      else
         gm_Print("Bit[%d]: OK", i);
   }

   gm_Print("End of DDR per bit check", 0);
}

void DdrTest_LongHours(DWORD DW_NumAccess)
{
   BYTE B_Flag, B_Opt, j, B_NoErr;
   WORD W_Width, W_Height;
   DWORD i;

   gm_Print("DdrTest_LongHours ......", 0);

   for (i = 0; i < DW_NumAccess; i++)
      B_NoErr = MemAccess(1, 1, BIT1, 32);

   if (B_NoErr == FALSE)
   {
      B_Flag = NVRam_ReadBuffer(NvramAddr_LongHrErr , &j, 1);
      if (B_Flag == OK)
      {
         gm_Print("NvramAddr_LongHrErr = %d", j);

         if (j < 255)
            j++;

         if (NVRam_WriteByte(NvramAddr_LongHrErr, j) != OK)
            gm_Print("Storing number of error failed.", 0);
      }
   }
   else
      gm_Print("DdrTest_LongHours Pass!", 0);

   gm_Print("End of DdrTest_LongHours", 0);
}

#endif

#endif // (FEATURE_DDR_EXECUTION == ENABLE)

//*********************************  END  **************************************

