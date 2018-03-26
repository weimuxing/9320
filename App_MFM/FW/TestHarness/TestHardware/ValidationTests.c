/*
    $Workfile:   ValidationTests.c  $
    $Revision: 1.5 $
    $Date: 2011/12/15 03:52:12 $
*/
#define __FSB_OSD_VERIFICATION_C__
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
#include "StdLibInc\gm_Register.h"
#include "StdLibInc\gm_Mcu186.h"

#ifdef IROM_VALIDATION_TEST_ENABLE

//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************
#define VLUT_TEST_BLOCK_SIZE 256


typedef struct
{
   WORD ctrlAddr; //address of crl host register TABLE_ACCESS_BRIDGE_CTRL1 or TABLE_ACCESS_BRIDGE_CTRL2
   WORD ctrlMask; //bit mask of apropriate table inside ctrl register
   WORD ctrlMode[gmd_MAX_NUMBER_TABLE_TRANSFER_MODES];
   WORD ctrlAccs[gmd_MAX_NUMBER_TABLE_ACCESS_MODES];
   WORD status; //apropriate bit in status register
   WORD request; //mask in DMA_REQUEST_CTRL register
   DWORD memAddr; //address in memory space reserved for that table
   WORD tabSize; //size of table
} TTableLoadParams;

//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************
extern gmt_GammaDescr ROM * ROM Sta_TestTables[];
//******************************************************************************
//  S T A T I C    V A R I A B L E S
//******************************************************************************

//******************************************************************************
//  S T A T I C    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

//******************************************************************************
//  C O D E
//******************************************************************************

//******************************************************************************
//
// FUNCTION       :
//
// USAGE          :
//
// INPUT          :
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
static WORD const far TABSize[gmd_TAB_SIZE] =
{
   0x1800, //VLUT
   0x0200, //ACC3
   0x0480, //OSD
   0x2000, //LPM
   0x2000, //MVF
   0x1000, //MHF
   0x0400, //PVF
   0x1000  //PHF
}; 

static WORD const far TAB_data_mask[gmd_TAB_SIZE] =
{
   0xFFF, //VLUT
   0x1FFF, //ACC3
   0x0800, //OSD
   0x2000, //LPM
   0x2000, //MVF
   0x1000, //MHF
   0x0400, //PVF
   0x1000  //PHF
}; 

#define D1CON_SETTINGS (DMA_DEST_POINTER_IS_MEMORY|DMA_DEST_POINTER_INC \
	|DMA_SRC_POINTER_IS_MEMORY|DMA_SRC_POINTER_INC|DMA_TC_TERMINATE \
	|DMA_DEST_SYNC|DMA_LOW_PRIORITY|DMA_DISABLE_TIMER_REQ \
	|DMA_START_OPERATIONS|DMA_WORD_TRANSFERS)


static void TestPattern(void _ss* pbuf, gmt_TAB_TABLE TabTable, WORD offs, WORD rnd)
{
   if (!pbuf)
      return;

   switch (TabTable)
   {
      case gmd_TAB_VLUT:
         {
            WORD _ss* pw = (WORD _ss*)pbuf;
            WORD w1 = 16/sizeof(WORD);
            for (; w1--; pw++)
               *pw = (WORD)((0xFFF-(offs++)+rnd) & 0xFFF);
            break;
         }
      case gmd_TAB_ACC_OUT_LUT:
         {
            WORD _ss* pw = (WORD _ss*)pbuf;
            WORD w1 = 16/sizeof(WORD);
            for (; w1--; pw++)
               *pw = (WORD)((0x1FFF-(offs++)+rnd) & 0x1FFF);
            break;
         }
      case gmd_TAB_OSDLUT:
         {
            ST_OSD_RGB _ss* ps = (ST_OSD_RGB _ss*)pbuf;
            WORD w1 = 16/sizeof(ST_OSD_RGB);
            for (; w1--; ps++)
            {
               ps->B_Red = (BYTE)(rnd+(offs++));
               ps->B_Blue = (BYTE)(rnd+(offs++));
               ps->B_Green = (BYTE)(rnd+(offs++));
               ps->reserved = 0;
            }
            break;
         }
      case gmd_TAB_LPM:
         {
            WORD _ss* pw = (WORD _ss*)pbuf;
            WORD w1 = 16/sizeof(WORD);
            for (; w1--; pw++)
               *pw = (WORD)(rnd-(offs++));
            break;
         }
         /*case TAB_MVF: {
             WORD (*pwa)[4] = (WORD (*)[4])&buffer;
             WORD w2 = (132 < w1/8) ? 132 : (w1/8);
             w1 = w2 * 8;
             for (; w2; w2--, pwa++)
             {
                 (*pwa)[0] = (*pwa)[2] = (~w2<<8) + hole;
                 (*pwa)[1] = (w2<<8) + taccess;
                 (*pwa)[3] = ((w2<<1) + tmode) & 0xff;
             }
             offset = (WORD)hole * 0x800;
             break;}
         case TAB_MHF: case TAB_PHF: {
             WORD (*pwa)[8] = (WORD (*)[8])&buffer;
             WORD w2 = (36 < w1/16) ? 36 : (w1/16);
             w1 = w2 * 16;
             for (; w2; w2--, pwa++)
             {
                 (*pwa)[0] = (*pwa)[2] = (w2<<8) + taccess;
                 (*pwa)[1] = (*pwa)[3] = (~w2<<8) + hole;
                 (*pwa)[4] = (w2<<8) + tmode;
                 (*pwa)[5] = (*pwa)[6] = (*pwa)[7] = 0;
             }
             offset = (WORD)hole * 0x400;
             break;}
         case TAB_PVF: {
             WORD (*pwa)[2] = (WORD (*)[2])&buffer;
             WORD w2 = (34 < w1/4) ? 34 : (w1/4);
             w1 = w2 * 4;
             for (; w2; w2--, pwa++)
             {
                 (*pwa)[0] = w2<<8 + taccess<<4 + tmode;
                 (*pwa)[1] = (~w2<<8) + hole;
             }
             offset = (WORD)hole * 0x100;
             break;}*/
   }
}


gmt_RET_STAT LoadTabTest(void)
{
   gmt_TAB_TABLE TabTable;
   //set up and enable VLUT
   gm_WriteRegWord(DISP_LUT_CONTROL, 0);
   //gm_WriteRegWord(ACC_CTRL , ACC_BYPASS|ACC_PD_n);
   gm_WriteRegWord(ACC_CTRL , ACC_PD_n);
   gm_SetRegBitsWord(HOST_CONTROL, ODP_SYNC_UPDATE);
   for (TabTable = gmd_TAB_VLUT; TabTable <= gmd_TAB_LPM; TabTable++)
   {
      gmt_TAB_TRANSFER_MODE TransMode;
      for (TransMode= gmd_DMA_MODE; TransMode < gmd_MAX_NUMBER_TABLE_TRANSFER_MODES; TransMode++)
      {
         gmt_TAB_ACCESS_MODE AccMode;
         for (AccMode = gmd_SAFE_MODE; AccMode < gmd_MAX_NUMBER_TABLE_ACCESS_MODES; AccMode++)
         {
            WORD buf[0x1000];
            WORD w;
            WORD w_rand = (WORD)gm_GetSystemTime();
            _fmemset(buf, 0, sizeof(buf));

            gm_Print("TAB test %s %s %s",
                     (TabTable == gmd_TAB_VLUT) ? "VLUT" :
                     (TabTable == gmd_TAB_ACC_OUT_LUT) ? "ACC" :
                     (TabTable == gmd_TAB_OSDLUT) ? "OSDLUT" : "LPM",
                     (TransMode == gmd_DMA_MODE)?"DMA":((TransMode == gmd_OCM_POLLING_MODE)?"Poll":"Stall"),
                     (AccMode == gmd_SAFE_MODE) ? "Safe" : "High");

            w = gm_LoadSRAMBlockEx(TabTable, TransMode, AccMode, (WORD far*)&buf, 0, TABSize[TabTable]);
            if ( w < TABSize[TabTable])
            {
               gm_Print("  Clear table failed: %d", w);
               break;
            }

            // fill buffer
            for (w = 0; w< TABSize[TabTable]; w+=16)
               TestPattern(&buf[w/sizeof(WORD)], TabTable, w, w_rand);

            w = gm_LoadSRAMBlockEx(TabTable, TransMode, AccMode, (WORD far*)&buf, 0, TABSize[TabTable]);
            if ( w < TABSize[TabTable])
               gm_Print("  Fill table failed: %d", w);

            while ((TransMode == gmd_DMA_MODE) && !gm_SRAMBlockDmaReady()); //wait till DMA finish transfer

            // check writing result
            _fmemset(buf, 0, sizeof(buf));
            w = gm_ReadSRAMBlock(TabTable, gmd_HIGH_PRIORITY_MODE,
                                 (WORD far*)&buf, 0, TABSize[TabTable]);
            if ( w < TABSize[TabTable])
               gm_Print("  Read table failed: %d", w);

            {
               WORD buf_r[16/sizeof(WORD)];
               WORD w2 = 10;
               for (w = 0; w< TABSize[TabTable];w+=sizeof(WORD))
               {
                  WORD w3 = w/sizeof(WORD);
                  if (w%16 == 0)
                     TestPattern(buf_r, TabTable, w, w_rand);
                  if (buf[w3] != buf_r[w3%8])
                  {
                     gm_Print("  %d: 0x%x != 0x%x", w3, buf[w3], buf_r[w3%8]);
                     if (--w2 == 0)
                        break;
                  }
               }
               gm_Print("  %s", (w2 < 10) ? "FAIL" : "OK");
            }
         }
      }
   }
   return OK;
}
#endif
//*********************************  END  **************************************



