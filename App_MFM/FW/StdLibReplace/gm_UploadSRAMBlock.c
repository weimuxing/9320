/*
   $Workfile:   gm_UploadSRAMBlock.c  $
   $Revision: 1.5 $
   $Date: 2011/12/14 10:35:32 $
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
// MODULE:      gm_UploadSRAMBlock.c
//
//******************************************************************


#include <embedded.h>
#include "System/all.h"
#include "Stdlibinc/gm_Register.h"
#include "stdlibreplace/gm_UploadSRAMBlock.h"


extern WORD gWa_UserTmrBuf[];

#define TAB_READ_TIMOUT_VAL 0xff
#define WAIT_TABLE_ACCESS_BRIDGE(W_Value, status_var, label)			\
	if((gm_ReadRegWord(TABLE_ACCESS_BRIDGE_STATUS) & (W_Value)))		\
	{																	\
		gWa_UserTmrBuf[SRAM_TABLE_READ_TIMEOUT] = TAB_READ_TIMOUT_VAL;		\
		while((gm_ReadRegWord(TABLE_ACCESS_BRIDGE_STATUS) & (W_Value)))		\
		{																\
			if(gWa_UserTmrBuf[SRAM_TABLE_READ_TIMEOUT] == 0)			\
			{															\
				status_var = ERR_BUSY;									\
				goto label;												\
			}															\
		}																\
	}


//========================================================================

TTableUploadParams const far  TableUploadingParams[] =
{
#if (CHIP_ID == STDP93xx)
   // Louis ... To Do --> Modify it for Athena later
   // 		 ctrlAddr				ctrlMask	 dma poll	stall	 safe hi-prior status request  mAdLo  tabSize
   { TABLE_ACCESS_BRIDGE_CTRL1, BIT0 |BIT1 |BIT2,	{ 0, BIT0,  BIT1 },	{0, BIT2 },	BIT0, 0x0040, 0x18000L, 0x1800}, //VLUT	=0
   { TABLE_ACCESS_BRIDGE_CTRL1, BIT3 |BIT4 |BIT5,	{ 0, BIT3,  BIT4 },	{0, BIT5 },	BIT1, 0x00A0, 0x17000L, 0x1000}, //ACC	=1
   { TABLE_ACCESS_BRIDGE_CTRL1, BIT9 |BIT10|BIT11,	{ 0, BIT9,  BIT10},	{0, BIT11},	BIT3, 0x0050, 0x0F000L, 0x0480}, //OSD	=2
   { TABLE_ACCESS_BRIDGE_CTRL1, BIT12|BIT13|BIT14,	{ 0, BIT12, BIT13},	{0, BIT14},	BIT4, 0x00B0, 0x16400L, 0x0400}, //ACM	=3
   { TABLE_ACCESS_BRIDGE_CTRL2, BIT0 |BIT1 |BIT2,	{ 0, BIT0,  BIT1 },	{0, BIT2 },	BIT5, 0x0060, 0x12000L, 0x1C20}, //MVF	=4
   { TABLE_ACCESS_BRIDGE_CTRL2, BIT3 |BIT4 |BIT5,	{ 0, BIT3,  BIT4 },	{0, BIT5 },	BIT6, 0x0080, 0x14000L, 0x0E40}, //MHF	=5
   { TABLE_ACCESS_BRIDGE_CTRL2, BIT6 |BIT7 |BIT8,	{ 0, BIT6,  BIT7 },	{0, BIT8 },	BIT7, 0x0070, 0x16000L, 0x0388}, //PVF	=6
   { TABLE_ACCESS_BRIDGE_CTRL2, BIT9 |BIT10|BIT11,	{ 0, BIT9,  BIT10},	{0, BIT11},	BIT8, 0x0090, 0x15000L, 0x0E40}, //PHF 	=7
#else
#error Wrong CHIP_ID Definition.
#endif
};
//========================================================================


gmt_RET_STAT gm_UploadSRAMBlock(gmt_TAB_TABLE Table,  gmt_TAB_TRANSFER_MODE TransferMode,
                                gmt_TAB_ACCESS_MODE AccessMode, WORD far * const p_DestBuffer,  WORD TabOffset, WORD BlockSize)
{
   gmt_RET_STAT status = OK;

//Here is reading (via DMA or OCM Poll)

   if ((AccessMode != gmd_SAFE_MODE)&&(AccessMode != gmd_HIGH_PRIORITY_MODE))
   {
      return ERR_PARAM;
   }
   else
   {

      TTableUploadParams params = TableUploadingParams[Table];
#ifdef __ADDR24__
      WORD far *p_Table = (WORD far *)(((DWORD)((WORD)(params.memAddr >> 16) << 8) << 16) | (WORD)params.memAddr + TabOffset);
#else
      WORD  far *p_Table = (WORD far *)(((DWORD)((WORD)(params.memAddr >> 16) << 12) << 16) | (WORD)params.memAddr + TabOffset);
#endif

      gm_ClearAndSetBitsWord(params.ctrlAddr, params.ctrlMask, params.ctrlMode[TransferMode] | params.ctrlAccs[AccessMode] );

      switch (TransferMode)
      {
         case gmd_OCM_POLLING_MODE:
            {
               WORD W_Flag = params.status;
               register WORD W_Index;
               {
                  register WORD * Wp_TableNear = (WORD *)p_DestBuffer;
                  register WORD _es * Wp_Src = (WORD _es *)p_Table;
                  _ES = FP_SEG(p_Table);

                  for (W_Index = BlockSize / sizeof(WORD); W_Index ; W_Index--)
                  {
                     WAIT_TABLE_ACCESS_BRIDGE(W_Flag, status, _terminate_transfer);

                     *Wp_TableNear++ = *Wp_Src++ ;
                  }
               }
            }
            break;
         case gmd_OCM_STALLING_MODE:
            {
               register WORD W_Index;

               {
                  register WORD *Wp_TableNear = (WORD *)p_DestBuffer;
                  register WORD _es *Wp_Src = (WORD _es *)p_Table;

                  _ES = FP_SEG(p_Table);

                  for (W_Index = BlockSize / sizeof(WORD); W_Index ; W_Index--)
                     *Wp_TableNear++ = *Wp_Src++ ;

               }
            }
            break;
         default:
            return ERR_PARAM;
      }

   }
_terminate_transfer:

   return status;
}

