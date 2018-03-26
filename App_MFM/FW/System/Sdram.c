/*
    $Workfile:   Sdram.c  $
    $Revision: 1.18 $
    $Date: 2013/06/11 06:32:46 $
*/
#define __SDRAM_C__

//******************************************************************************
// COPYRIGHT (C) STMicroelectronics 2010 All rights reserved.
// This document contains proprietary and confidential information of the
// STMicroelectronics Group. This document is not to be copied in whole or part.
// STMicroelectronics assumes no responsibility for the consequences of use of
// such information nor for any infringement of patents or other rights of third
// parties which may result from its use. No license is granted by implication
// or otherwise under any patent or patent rights of STMicroelectronics.
// STMicroelectronics products are not authorized for use as critical components
// in life support devices or systems without express written approval of
// STMicroelectronics.
//******************************************************************************

//******************************************************************************
//
// MODULE:      SDRAM.c
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "System\all.h"
#include "system\Sdram.h"
#include "embedded.h"

#if (FEATURE_DDR_EXECUTION == ENABLE)

//******************************************************************************
//  D E F I N E S
//******************************************************************************

//******************************************************************************
//  G L O B A L   V A R I A B L E S
//******************************************************************************
BYTE DDR_RW_Flag = 0x00;

#if 1
WORD ROM WBDRegLUT[4][12] =
{
   {PCTL32_PHY_DX0BDLR0,	// DQ0WBD
      PCTL32_PHY_DX0BDLR0,	// DQ1WBD
      PCTL32_PHY_DX0BDLR0,	// DQ2WBD
      PCTL32_PHY_DX0BDLR0,	// DQ3WBD
      PCTL32_PHY_DX0BDLR0,	// DQ4WBD
      PCTL32_PHY_DX0BDLR1,	// DQ5WBD
      PCTL32_PHY_DX0BDLR1,	// DQ6WBD
      PCTL32_PHY_DX0BDLR1,    // DQ7WBD
      PCTL32_PHY_DX0BDLR1,    // DMWBD
      PCTL32_PHY_DX0BDLR1,    // DSWBD
      PCTL32_PHY_DX0BDLR2,    // DSOEBD
      PCTL32_PHY_DX0BDLR2},	// DQOEBD

   {PCTL32_PHY_DX1BDLR0,
    PCTL32_PHY_DX1BDLR0,
    PCTL32_PHY_DX1BDLR0,
    PCTL32_PHY_DX1BDLR0,
    PCTL32_PHY_DX1BDLR0,
    PCTL32_PHY_DX1BDLR1,
    PCTL32_PHY_DX1BDLR1,
    PCTL32_PHY_DX1BDLR1,
    PCTL32_PHY_DX1BDLR1,
    PCTL32_PHY_DX1BDLR1,
    PCTL32_PHY_DX1BDLR2,
    PCTL32_PHY_DX1BDLR2},

   {PCTL32_PHY_DX2BDLR0,
    PCTL32_PHY_DX2BDLR0,
    PCTL32_PHY_DX2BDLR0,
    PCTL32_PHY_DX2BDLR0,
    PCTL32_PHY_DX2BDLR0,
    PCTL32_PHY_DX2BDLR1,
    PCTL32_PHY_DX2BDLR1,
    PCTL32_PHY_DX2BDLR1,
    PCTL32_PHY_DX2BDLR1,
    PCTL32_PHY_DX2BDLR1,
    PCTL32_PHY_DX2BDLR2,
    PCTL32_PHY_DX2BDLR2},

   {PCTL32_PHY_DX3BDLR0,
    PCTL32_PHY_DX3BDLR0,
    PCTL32_PHY_DX3BDLR0,
    PCTL32_PHY_DX3BDLR0,
    PCTL32_PHY_DX3BDLR0,
    PCTL32_PHY_DX3BDLR1,
    PCTL32_PHY_DX3BDLR1,
    PCTL32_PHY_DX3BDLR1,
    PCTL32_PHY_DX3BDLR1,
    PCTL32_PHY_DX3BDLR1,
    PCTL32_PHY_DX3BDLR2,
    PCTL32_PHY_DX3BDLR2}
};

WORD ROM RBDRegLUT[4][10] =
{
   {PCTL32_PHY_DX0BDLR3,	// DQ0RBD
      PCTL32_PHY_DX0BDLR3,	// DQ1RBD
      PCTL32_PHY_DX0BDLR3,	// DQ2RBD
      PCTL32_PHY_DX0BDLR3,	// DQ3RBD
      PCTL32_PHY_DX0BDLR3,	// DQ4RBD
      PCTL32_PHY_DX0BDLR4,	// DQ5RBD
      PCTL32_PHY_DX0BDLR4,	// DQ6RBD
      PCTL32_PHY_DX0BDLR4,    // DQ7RBD
      PCTL32_PHY_DX0BDLR4,    // DMRBD
      PCTL32_PHY_DX0BDLR4},	// DQSRBD

   {PCTL32_PHY_DX1BDLR3,
    PCTL32_PHY_DX1BDLR3,
    PCTL32_PHY_DX1BDLR3,
    PCTL32_PHY_DX1BDLR3,
    PCTL32_PHY_DX1BDLR3,
    PCTL32_PHY_DX1BDLR4,
    PCTL32_PHY_DX1BDLR4,
    PCTL32_PHY_DX1BDLR4,
    PCTL32_PHY_DX1BDLR4,
    PCTL32_PHY_DX1BDLR4},

   {PCTL32_PHY_DX2BDLR3,
    PCTL32_PHY_DX2BDLR3,
    PCTL32_PHY_DX2BDLR3,
    PCTL32_PHY_DX2BDLR3,
    PCTL32_PHY_DX2BDLR3,
    PCTL32_PHY_DX2BDLR4,
    PCTL32_PHY_DX2BDLR4,
    PCTL32_PHY_DX2BDLR4,
    PCTL32_PHY_DX2BDLR4,
    PCTL32_PHY_DX2BDLR4},

   {PCTL32_PHY_DX3BDLR3,
    PCTL32_PHY_DX3BDLR3,
    PCTL32_PHY_DX3BDLR3,
    PCTL32_PHY_DX3BDLR3,
    PCTL32_PHY_DX3BDLR3,
    PCTL32_PHY_DX3BDLR4,
    PCTL32_PHY_DX3BDLR4,
    PCTL32_PHY_DX3BDLR4,
    PCTL32_PHY_DX3BDLR4,
    PCTL32_PHY_DX3BDLR4}
};

DWORD ROM BDOfsLUT[12] =
{
   0x0000003F,
   0x00000FC0,
   0x0003F000,
   0x00FC0000,
   0x3F000000,
   0x0000003F,
   0x00000FC0,
   0x0003F000,
   0x00FC0000,
   0x3F000000,
   0x0000003F,
   0x00000FC0
};

BYTE ROM BDShiftLUT[12] =
{
   0,
   6,
   12,
   18,
   24,
   0,
   6,
   12,
   18,
   24,
   0,
   6
};

WORD ROM LCDLRegLUT[4][4] =
{
   {PCTL32_PHY_DX0LCDLR0,	// R0WLD
      PCTL32_PHY_DX0LCDLR1,	// WDQD
      PCTL32_PHY_DX0LCDLR1,	// RDQSD
      PCTL32_PHY_DX0LCDLR2},	// R0DQSGD

   {PCTL32_PHY_DX1LCDLR0,
    PCTL32_PHY_DX1LCDLR1,
    PCTL32_PHY_DX1LCDLR1,
    PCTL32_PHY_DX1LCDLR2},

   {PCTL32_PHY_DX2LCDLR0,
    PCTL32_PHY_DX2LCDLR1,
    PCTL32_PHY_DX2LCDLR1,
    PCTL32_PHY_DX2LCDLR2},

   {PCTL32_PHY_DX3LCDLR0,
    PCTL32_PHY_DX3LCDLR1,
    PCTL32_PHY_DX3LCDLR1,
    PCTL32_PHY_DX3LCDLR2}
};

DWORD ROM LCDLOfsLUT[4] =
{
   0x000000FF,
   0x000000FF,
   0x0000FF00,
   0x000000FF
};

BYTE ROM LCDLShiftLUT[4] =
{
   0,
   0,
   8,
   0
};
#endif

//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
void SetDDRClock(void);
void InitMixer(void);
void InitPCTL(void);
void FineTuneLCDLR1(void);
void InitMemCtrl(void);

//*****************************************************************************
//  C O D E
//*****************************************************************************

//******************************************************************************
//
// FUNCTION       : void InitMemIF(void)
//
// USAGE          :
//
// INPUT          :
//
// OUTPUT         : None
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
void InitMemIF(void)
{
   DWORD DW_Temp, DW_EnableVT;
   WORD W_Tmp;

   // Clear all the FIFO requests
   gm_WriteRegWord(MC_FIFO_EN_hi, 0x0000);
   gm_WriteRegWord(MC_FIFO_EN_lo, 0x0000);

#if (RCLK_FREQ_ARGS == RCLKSEL_270M)
   gm_FS432Setup(FS432_1, FSCLK_300M, RCLK_FREQ_ARGS);

#else
   // Memory Controller Clock
#if (ENABLE_DDR_X16 == ENABLE)
   gm_FS432Setup(FS432_1, FSCLK_250M, RCLK_FREQ_ARGS);
#else
   gm_FS432Setup(FS432_1, FSCLK_400M, RCLK_FREQ_ARGS);
#endif
#endif //(RCLK_FREQ_ARGS)

   gm_ClearAndSetBitsWord(CLOCK_CONFIG1, MC_CLK_SEL, (3 << 0));

   // Reset MemCtrl before initiating LMI
   gm_WriteRegWord(MC_CONFIG, VAL_MC_CONFIG);
   gm_SetRegBitsWord(SOFT_RESETS, MC_RESET);
   gm_Delay1ms(1);
   gm_ClearRegBitsWord(SOFT_RESETS, MC_RESET);

   SetDDRClock();

   InitMixer();

	InitPCTL();

	//HYB130113 disable key message
	//gm_Print(DdrDevice, 0);

   // Start memory power-up sequence
   PCTL_WriteReg(PCTL32_POWCTL, 0x00000001);
   W_Tmp = 0;
   while (((PCTL_ReadReg(PCTL32_POWCTL) & BIT0) == BIT0) && (W_Tmp < 0xFFF0))
      W_Tmp++;

   if (W_Tmp >= 0xFFF0)
      gm_Print("PCTL32_POWCTL check fail.");

   InitDdr();

   // Move PCTL to CFG state
   PCTL_WriteReg(PCTL32_SCTL, 0x00000001);

   // Enable Datax8 according to DDR bus size. x32 or x16.
   PCTL_WriteReg(PCTL32_PUB_PPMCFG, VAL_PCTL32_PUB_PPMCFG);
   //PCTL_WriteReg(PCTL32_PHY_DX2GCR, VAL_PCTL32_PHY_DX0GCR);
   //PCTL_WriteReg(PCTL32_PHY_DX3GCR, VAL_PCTL32_PHY_DX1GCR);
   PCTL_WriteReg(PCTL32_PHY_DX2GCR, VAL_PCTL32_PHY_DX2GCR);
   PCTL_WriteReg(PCTL32_PHY_DX3GCR, VAL_PCTL32_PHY_DX3GCR);

   PCTL_WriteReg(PCTL32_DQSECFG, VAL_PCTL32_DQSECFG);
   PCTL_WriteReg(PCTL32_PUB_SLCR, VAL_PCTL32_PUB_SLCR);

   // Program PUB SMCTL initial value
   PCTL_WriteReg(PCTL32_PUB_SMCTL, 0xA5580000);

   // Set INITPUBCTL to 1 to indicate ST1 is completely
   PCTL_WriteReg(PCTL32_INITPUBPCTL, 0x00000001);

   W_Tmp = 0;
   while ((PCTL_ReadReg(PCTL32_PUB_SMCTL) != 0xFFFF0000) && (W_Tmp < 0xFFF0))
      W_Tmp++;

   if (W_Tmp >= 0xFFF0)
   {
      gm_Print("PCTL32_PUB_SMCTL check fail.");

      DW_Temp = PCTL_ReadReg(PCTL32_PUB_SMCTL);
      gm_Print("PCTL32_PUB_SMCTL_lo = 0x%x", (WORD)DW_Temp);
      gm_Print("PCTL32_PUB_SMCTL_hi = 0x%x", (WORD)(DW_Temp >> 16));
   }

   FineTuneLCDLR1();

   // Enabling the VT
   DW_EnableVT = PCTL_ReadReg(PCTL32_PHY_PIR);
   DW_EnableVT = (DWORD)(DW_EnableVT & (0xEFFFFFFF));
   PCTL_WriteReg(PCTL32_PHY_PIR, DW_EnableVT);

   
	#if Enable_2T
	 DW_Temp = PCTL_ReadReg(PCTL32_MCFG);
	 DW_Temp |= 0x00000008;
	 PCTL_WriteReg(PCTL32_MCFG, DW_Temp);
	#endif

	PCTL_WriteReg(PCTL32_PHYPVTCFG, VAL_PCTL32_PHYPVTCFG);
   PCTL_WriteReg(PCTL32_PHYTUPDWAIT, 0x00000003);
   PCTL_WriteReg(PCTL32_PVTTUPDWAIT, 0x00000003);
   //PCTL_WriteReg(PCTL32_PVTUPDI, 0x00000000);

   PCTL_WriteReg(PCTL32_PHYIOCRV1, VAL_PCTL32_PHYIOCRV1);

   // Move PCTL to GO state
   PCTL_WriteReg(PCTL32_SCTL, 0x00000002);

   InitMemCtrl();
}


void SetDDRClock(void)
{
   gm_WriteRegWord(CTL_PLL_DDR_FREQ, VAL_CTL_PLL_DDR_FREQ_lo);
   gm_WriteRegWord(CTL_PLL_DDR_FREQ + 2, VAL_CTL_PLL_DDR_FREQ_hi);
   gm_Delay1ms(1);

   gm_WriteRegWord(CTL_EN, 0x0007);
   gm_WriteRegWord(CTL_EN + 2, 0x0000);
   gm_WriteRegWord(CTL_PLL_DDR_SSC, 0x0000);
   gm_WriteRegWord(CTL_PLL_DDR_SSC + 2, 0x0000);
   gm_WriteRegWord(CTL_OBS_MUX, 0x0000);
   gm_WriteRegWord(CTL_OBS_MUX + 2, 0x0000);
}

void InitMixer(void)
{
   WORD far *dst;
   BYTE i;

   gm_WriteRegWord(MIXER_ROW_ADDR_MASK, VAL_MIXER_ROW_ADDR_MASK_lo);
   gm_WriteRegWord(MIXER_ROW_ADDR_MASK + 2, VAL_MIXER_ROW_ADDR_MASK_hi);

   gm_WriteRegWord(MIXER_DDR_BASE_ADDR, VAL_MIXER_DDR_BASE_ADDR_lo);
   gm_WriteRegWord(MIXER_DDR_BASE_ADDR + 2, VAL_MIXER_DDR_BASE_ADDR_hi);

   gm_WriteRegWord(MIXER_GEN_PURPOSE_REG0, VAL_MIXER_GEN_PURPOSE_REG0_lo);
   gm_WriteRegWord(MIXER_GEN_PURPOSE_REG0 + 2, VAL_MIXER_GEN_PURPOSE_REG0_hi);

   gm_WriteRegWord(MIXER_DDR_PARAMETER, VAL_MIXER_DDR_PARAMETER_lo);
   gm_WriteRegWord(MIXER_DDR_PARAMETER + 2, VAL_MIXER_DDR_PARAMETER_hi);

   gm_WriteRegWord(MIXER_GEN_CTRL, VAL_MIXER_GEN_CTRL_lo);
   gm_WriteRegWord(MIXER_GEN_CTRL + 2, VAL_MIXER_GEN_CTRL_hi);

   // ---- BANK LUT Memory programmed to 10,11,12 START
   gm_WriteRegWord(DVI_DP_CONFIG, BIT12);
   // [12] MIXER_BANKLUT_RAM_MODE	RAM priority
   // [13] MIXER_LPSRC_RAM_MODE	RAM priority

   // Bank Reg LUT
   dst = MK_FP(0x17C,0x0000);

   for (i = 0; i < 64; i++)
   {
      *dst++ = VAL_BankRegLUT;
   }

}

void InitPCTL(void)
{
   DWORD DW_Temp;
	
   gm_Print("Init PCTL. April 27 2012", 0);

   PCTL_WriteReg(PCTL32_PHY_ACBDLR, VAL_PCTL32_PHY_ACBDLR);

   PCTL_WriteReg(PCTL32_PHY_PLLCR, VAL_PCTL32_PHY_PLLCR | BIT18);

   // Re-init the PHY. Addition Reinit of PHY Bit 0 should be set to 1  to retrigger PHY init.
   DW_Temp = PCTL_ReadReg(PCTL32_PHY_PIR);
   PCTL_WriteReg(PCTL32_PHY_PIR, (DW_Temp  | BIT0));

   PCTL_WriteReg(PCTL32_TOGCNT1U, VAL_PCTL32_TOGCNT1U);
   PCTL_WriteReg(PCTL32_TINIT, VAL_PCTL32_TINIT);
   PCTL_WriteReg(PCTL32_TRSTH, VAL_PCTL32_TRSTH);
   PCTL_WriteReg(PCTL32_TOGCNT100N, VAL_PCTL32_TOGCNT100N);

   // Static ODT to be enabled Before running the PUB ST M/c 
   PCTL_WriteReg(PCTL32_PHYIOCRV1, (VAL_PCTL32_PHYIOCRV1 & ~(BIT2)));

   PCTL_WriteReg(PCTL32_PHYPVTCFG, (VAL_PCTL32_PHYPVTCFG & ~(BIT15 | BIT7)));
   PCTL_WriteReg(PCTL32_PHY_ZQ0CR, VAL_PCTL32_PHY_ZQ0CR);

   PCTL_WriteReg(PCTL32_MCFG, VAL_PCTL32_MCFG);
   PCTL_WriteReg(PCTL32_PPCFG, VAL_PCTL32_PPCFG);

   PCTL_WriteReg(PCTL32_ODTCFG, VAL_PCTL32_ODTCFG);
   PCTL_WriteReg(PCTL32_ODTCFG1, VAL_PCTL32_ODTCFG1);

#if (Enable_DRR == 1)
   PCTL_WriteReg(PCTL32_TREFI, (VAL_PCTL32_TREFI >> 1));
#else
   PCTL_WriteReg(PCTL32_TREFI, VAL_PCTL32_TREFI);
#endif
   PCTL_WriteReg(PCTL32_TMRD, VAL_PCTL32_TMRD);
   PCTL_WriteReg(PCTL32_TRFC, VAL_PCTL32_TRFC);
   PCTL_WriteReg(PCTL32_TRP, VAL_PCTL32_TRP);
   PCTL_WriteReg(PCTL32_TRTW, VAL_PCTL32_TRTW);
   PCTL_WriteReg(PCTL32_TAL, VAL_PCTL32_TAL);
   PCTL_WriteReg(PCTL32_TCL, VAL_PCTL32_TCL);
   PCTL_WriteReg(PCTL32_TCWL, VAL_PCTL32_TCWL);
   PCTL_WriteReg(PCTL32_TRAS, VAL_PCTL32_TRAS);
   PCTL_WriteReg(PCTL32_TRC, VAL_PCTL32_TRC);

   PCTL_WriteReg(PCTL32_TRCD, VAL_PCTL32_TRCD);
   PCTL_WriteReg(PCTL32_TRRD, VAL_PCTL32_TRRD);
   PCTL_WriteReg(PCTL32_TRTP, VAL_PCTL32_TRTP);
   PCTL_WriteReg(PCTL32_TWR, VAL_PCTL32_TWR);
   PCTL_WriteReg(PCTL32_TWTR, VAL_PCTL32_TWTR);
   PCTL_WriteReg(PCTL32_TEXSR, VAL_PCTL32_TEXSR);
   PCTL_WriteReg(PCTL32_TXP, VAL_PCTL32_TXP);
   PCTL_WriteReg(PCTL32_TXPDLL, VAL_PCTL32_TXPDLL);

   PCTL_WriteReg(PCTL32_TCKE, VAL_PCTL32_TCKE);

   #if (DDRType == DDR3)
   PCTL_WriteReg(PCTL32_TZQCS, VAL_PCTL32_TZQSCS);
   PCTL_WriteReg(PCTL32_TZQCSI, VAL_PCTL32_TZQSCSI);
   //PCTL_WriteReg(PCTL32_TDQS, VAL_PCTL32_TDQS);	// Use POD value
   PCTL_WriteReg(PCTL32_TCKSRE, VAL_PCTL32_TCKSRE);
   PCTL_WriteReg(PCTL32_TCKSRX, VAL_PCTL32_TCKSRX);

   PCTL_WriteReg(PCTL32_TMOD, VAL_PCTL32_TMOD);
   PCTL_WriteReg(PCTL32_TRSTL, VAL_PCTL32_TRSTL);
   PCTL_WriteReg(PCTL32_TZQCL, VAL_PCTL32_TZQCL);
   #endif
}

void DdrCmd(DWORD DW_Val)
{
   if ((PCTL_ReadReg(PCTL32_MCMD) & BIT31) == BIT31)
   {
      gm_Print("One of commands is still running. ", 0);
      gm_Print("PCTL32_MCMD_lo = 0x%x", (WORD)DW_Val);
      gm_Print("PCTL32_MCMD_hi = 0x%x", (WORD)(DW_Val >> 16));
      gm_Delay1ms(1);
   }

#if 0	// Dbg purpose
   gm_Print("PCTL32_MCMD_lo = 0x%x", (WORD)DW_Val);
   gm_Print("PCTL32_MCMD_hi = 0x%x", (WORD)(DW_Val >> 16));
#endif

   PCTL_WriteReg(PCTL32_MCMD, (DW_Val | BIT31));
}

void InitDdr(void)
{
#if (DDRType == DDR2)
   // PREA
   DdrCmd(Val_PCTL32_MCMD | OPCODE_PREA);

   // MR2
   DdrCmd(Val_PCTL32_MCMD | (VAL_OPCODE_MRS_MR2 | OPCODE_MRS));
   // MR3
   DdrCmd(Val_PCTL32_MCMD | (VAL_OPCODE_MRS_MR3 | OPCODE_MRS));
   // MR1, Enable DLL
   DdrCmd(Val_PCTL32_MCMD | (VAL_OPCODE_MRS_MR1 | OPCODE_MRS));
   // MR0, DLL Reset
   DdrCmd(Val_PCTL32_MCMD | (VAL_OPCODE_MRS_MR0 | OPCODE_MRS));

   DdrCmd(Val_PCTL32_MCMD | OPCODE_PREA);
   DdrCmd(Val_PCTL32_MCMD | OPCODE_REF);
   DdrCmd(Val_PCTL32_MCMD | OPCODE_REF);

   // MR0, Initialize device operation
   DdrCmd(Val_PCTL32_MCMD | VAL_OPCODE_MRS_MR0_ActDll | OPCODE_MRS);

   // MR1, OCD Calibration
   DdrCmd(Val_PCTL32_MCMD | VAL_OPCODE_MRS_MR1_OcdCal | OPCODE_MRS);
   // MR1, Exit OCD Calibration
   DdrCmd(Val_PCTL32_MCMD | VAL_OPCODE_MRS_MR1 | OPCODE_MRS);

	#else

   // NOP
   //DdrCmd(OPCODE_NOP, 0); // bit[27:24] was set to 7
   DdrCmd(Val_PCTL32_MCMD | OPCODE_NOP);
   // MR2
#if (Enable_DRR == 1)
   DdrCmd(Val_PCTL32_MCMD | ((VAL_OPCODE_MRS_MR2 | 0x00000800) | OPCODE_MRS));
#else
   //DdrCmd(OPCODE_MRS, 0x4010);
   DdrCmd(Val_PCTL32_MCMD | (VAL_OPCODE_MRS_MR2 | OPCODE_MRS));
#endif
   // MR3
   //DdrCmd(OPCODE_MRS, 0x6000);
   DdrCmd(Val_PCTL32_MCMD | (VAL_OPCODE_MRS_MR3 | OPCODE_MRS));
   // MR1
   //DdrCmd(OPCODE_MRS, 0x2044);
   DdrCmd(Val_PCTL32_MCMD | (VAL_OPCODE_MRS_MR1 | OPCODE_MRS));
   // MR0
   //DdrCmd(OPCODE_MRS, 0x0940);
   DdrCmd(Val_PCTL32_MCMD | (VAL_OPCODE_MRS_MR0 | OPCODE_MRS));
   //ZQCL
   //DdrCmd(OPCODE_ZQCL, 0);
   DdrCmd(Val_PCTL32_MCMD | OPCODE_ZQCL);
#endif
}

void InitMemCtrl(void)
{
   gm_SetRegBitsWord(MC_REQUEST_CTRL, MC_FIFO_RST_WHEN_DIS);

   gm_WriteRegWord(MC_FIFO_EN_hi, 0x0FF8);
   gm_WriteRegWord(MC_FIFO_EN_lo, 0x7F8F);

   gm_WriteRegWord(MC_REQUEST_CTRL, 0x0006);
}

BYTE GetDdrPhyDly(BYTE B_DlyType, BYTE B_ByteID, BYTE B_BitID)
{
   BYTE B_Ret;

   B_Ret = AdjustBitDelayLine(GET_DLY, B_DlyType, B_ByteID, B_BitID, 0);

   return B_Ret;
}

void SetDdrPhyDly(BYTE B_DlyType, BYTE B_ByteID, BYTE B_BitID, BYTE B_DlyVal)
{
   BYTE B_Ret;

   B_Ret = AdjustBitDelayLine(SET_DLY, B_DlyType, B_ByteID, B_BitID, B_DlyVal);
   UNUSED_VAR(B_Ret);
}

BYTE AdjustBitDelayLine(BYTE B_RW, BYTE B_DlyType, BYTE B_ByteID, BYTE B_BitID, BYTE B_Delay)
{
   BYTE B_Shift, B_RetVal;
   WORD W_Reg;
   DWORD DW_RegVal, DW_Mask;

   switch (B_DlyType)
   {
      case DLY_WBD:
         W_Reg = WBDRegLUT[B_ByteID][B_BitID];
         DW_Mask = BDOfsLUT[B_BitID];
         B_Shift = BDShiftLUT[B_BitID];
         break;

      case DLY_DMWBD:
         W_Reg = WBDRegLUT[B_ByteID][8];
         DW_Mask = BDOfsLUT[8];
         B_Shift = BDShiftLUT[8];
         break;

      case DLY_DSWBD:
         W_Reg = WBDRegLUT[B_ByteID][9];
         DW_Mask = BDOfsLUT[9];
         B_Shift = BDShiftLUT[9];
         break;

      case DLY_DSOEBD:
         W_Reg = WBDRegLUT[B_ByteID][10];
         DW_Mask = BDOfsLUT[10];
         B_Shift = BDShiftLUT[10];
         break;

      case DLY_DQOEBD:
         W_Reg = WBDRegLUT[B_ByteID][11];
         DW_Mask = BDOfsLUT[11];
         B_Shift = BDShiftLUT[11];
         break;

      case DLY_RBD:
         W_Reg = RBDRegLUT[B_ByteID][B_BitID];
         DW_Mask = BDOfsLUT[B_BitID];
         B_Shift = BDShiftLUT[B_BitID];
         break;

      case DLY_DMRBD:
         W_Reg = RBDRegLUT[B_ByteID][8];
         DW_Mask = BDOfsLUT[8];
         B_Shift = BDShiftLUT[8];
         break;

      case DLY_DSRBD:
         W_Reg = RBDRegLUT[B_ByteID][9];
         DW_Mask = BDOfsLUT[9];
         B_Shift = BDShiftLUT[9];
         break;

      case DLY_WLD:
         W_Reg = LCDLRegLUT[B_ByteID][0];
         DW_Mask = LCDLOfsLUT[0];
         B_Shift = LCDLShiftLUT[0];
         break;

      case DLY_WDQD:
         W_Reg = LCDLRegLUT[B_ByteID][1];
         DW_Mask = LCDLOfsLUT[1];
         B_Shift = LCDLShiftLUT[1];
         break;

      case DLY_RDQSD:
         W_Reg = LCDLRegLUT[B_ByteID][2];
         DW_Mask = LCDLOfsLUT[2];
         B_Shift = LCDLShiftLUT[2];
         break;

      case DLY_DQSGD:
         W_Reg = LCDLRegLUT[B_ByteID][3];
         DW_Mask = LCDLOfsLUT[3];
         B_Shift = LCDLShiftLUT[3];
         break;
   }

   DW_RegVal = PCTL_ReadReg(W_Reg);
   B_RetVal = (BYTE)((DW_RegVal & DW_Mask) >> B_Shift);
   if (B_RW == SET_DLY)
   {
      DW_RegVal &= ~DW_Mask;
      DW_RegVal |= ((DWORD)B_Delay << B_Shift);
      PCTL_WriteReg(W_Reg, DW_RegVal);
   }

   return B_RetVal;
}

void FineTuneLCDLR1(void)
{
	BYTE i, j, B_Val;
	BYTE B_Plus, B_Dly;

	#if (FineTune_WDQD != 0x00)
	gm_Print("Fine tune WDQD: 0x%x", FineTune_WDQD);

	B_Plus = TRUE;
	B_Dly = FineTune_WDQD;
	if ((B_Dly & BIT7) == BIT7)
	{
		B_Plus = FALSE;
		B_Dly &= ~BIT7;
	}

	for (i = 0; i < NUM_DATAx8; i++)
	{
		B_Val = GetDdrPhyDly(DLY_WDQD, i, 0);

		if (B_Plus == TRUE)
			B_Val += B_Dly;
		else
			B_Val -= B_Dly;

		SetDdrPhyDly(DLY_WDQD, i, 0, B_Val);
	}
	#endif

	#if (FineTune_RDQSD != 0x00)
	gm_Print("Fine tune RDQSD: 0x%x", FineTune_RDQSD);

	B_Plus = TRUE;
	B_Dly = FineTune_RDQSD;
	if ((B_Dly & BIT7) == BIT7)
	{
		B_Plus = FALSE;
		B_Dly &= ~BIT7;
	}

	for (i = 0; i < NUM_DATAx8; i++)
	{
		B_Val = GetDdrPhyDly(DLY_RDQSD, i, 0);

		if (B_Plus == TRUE)
			B_Val += B_Dly;
		else
			B_Val -= B_Dly;

		SetDdrPhyDly(DLY_RDQSD, i, 0, B_Val);
	}
	#endif
}

void PctlRegProgramming(WORD W_Reg, DWORD DW_Val)
{
   PCTL_WriteReg(PCTL32_SCTL, 0x00000001);
   PCTL_WriteReg(W_Reg, DW_Val);
   PCTL_WriteReg(PCTL32_SCTL, 0x00000002);
}

void DDR_RW_HANDLER(void)
{
   #if (DDR_TEST_CODE == ENABLE)
   if ((DDR_RW_Flag & BIT0) == BIT0)
   {
      DdrTest_LongHours(1);
   }
   #endif
}
#else // (FEATURE_DDR_EXECUTION == ENABLE)
void InitMemIF(void)
{
}
void DDR_RW_HANDLER(void)
{
}
#endif // (FEATURE_DDR_EXECUTION == ENABLE)

/***********************************  END  ************************************/
