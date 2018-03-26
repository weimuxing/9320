#define __COLOR_FIDELITY_CONTROL_C__

//******************************************************************************
//
//
//==============================================================================
//
// MODULE:      ColorFidelityControl.c
//
// USAGE:
//
//
//******************************************************************************

#include "System\All.h"

#ifdef USE_COLOR_FIDELITY

#define PreAdrLUTGammaTable_R 0x1C000L // 0x000B000UL
#define PreAdrLUTGammaTable_G 0x1C800L // 0x000B800UL
#define PreAdrLUTGammaTable_B 0x1D000L // 0x1009800UL


#define PostAdrLUTGammaTable_R  0x1D800L // 0x1008000
#define PostAdrLUTGammaTable_G  0x1E000L // 0x1008800
#define PostAdrLUTGammaTable_B  0x1E800L // 0x1009000


//*****************************************************************************
// FUNCTION     : void PreLUTAccessEnable
// USAGE        : Enable PreLUT access
// INPUT        :
// OUTPUT       :
// GLOBALS      :
// USED_REGS    :
//*****************************************************************************
void far PreLUTAccessEnable(void)
{
   //gm_SetRegBitsByte(IMP_LUT_CONTROL, IMP_LUT_EN );
   gm_ClearAndSetBitsWord(GAMMA_LUT1_CTRL, GAMMA_LUT1_CTL, BIT5);
   // No more bridge control
   //gm_WriteRegWord(TABLE_ACCESS_BRIDGE_CTRL1, 0x0186);	/// Enable DMA bridge for writing Pre-LUT
}

//*****************************************************************************
// FUNCTION     : void WritePreLutRed
// USAGE        : Write PreLUT Red
// INPUT        :
// OUTPUT       :
// GLOBALS      :
// USED_REGS    :
//*****************************************************************************
//void far WritePreLutRed(WORD W_AddrOffset, WORD W_Val)
void far WritePreLutRed(WORD W_AddrOffset, BYTE B_Val)
{
   //gm_WriteRegWord(PreAdrLUTGammaTable_R + W_AddrOffset, W_Val);
   *LINEAR_TO_FPTR_B(PreAdrLUTGammaTable_R + W_AddrOffset) = B_Val;
}

//*****************************************************************************
// FUNCTION     : void WritePreLutGreen
// USAGE        : Write PreLUT Green
// INPUT        :
// OUTPUT       :
// GLOBALS      :
// USED_REGS    :
//*****************************************************************************
//void far WritePreLutGreen(WORD W_AddrOffset, WORD W_Val)
void far WritePreLutGreen(WORD W_AddrOffset, BYTE B_Val)
{
   //gm_WriteRegWord(PreAdrLUTGammaTable_G + W_AddrOffset, W_Val);
   *LINEAR_TO_FPTR_B(PreAdrLUTGammaTable_G + W_AddrOffset) = B_Val;
}

//*****************************************************************************
// FUNCTION     : void WritePreLutBlue
// USAGE        : Write PreLUT Blue
// INPUT        :
// OUTPUT       :
// GLOBALS      :
// USED_REGS    :
//*****************************************************************************
//void far WritePreLutBlue(WORD W_AddrOffset, WORD W_Val)
void far WritePreLutBlue(WORD W_AddrOffset, BYTE B_Val)
{
   //BYTE far * Pre_BPtr = (BYTE far *)(PreAdrLUTGammaTable_B);

   //*(Pre_BPtr + W_AddrOffset) = (BYTE)W_Val;
   //*(Pre_BPtr + W_AddrOffset +1) = (BYTE)(W_Val >> 8);
   *LINEAR_TO_FPTR_B(PreAdrLUTGammaTable_B + W_AddrOffset) = B_Val;
}

//*****************************************************************************
// FUNCTION     : void WritePostLut
// USAGE        : Write PostLUT
// INPUT        :
// OUTPUT       :
// GLOBALS      :
// USED_REGS    :
//*****************************************************************************
void far WritePostLut(void)
{
	gm_LoadVlutTable(RED_VLUT_COMPONENT, 0, 1024/2, (WORD far *)PANEL_COMP_GAMMA_RED, 0);
	gm_LoadVlutTable(GREEN_VLUT_COMPONENT, 0, 1024/2, (WORD far *)PANEL_COMP_GAMMA_GREEN, 0);
	gm_LoadVlutTable(BLUE_VLUT_COMPONENT, 0, 1024/2, (WORD far *)PANEL_COMP_GAMMA_BLUE, 0);
}

//*****************************************************************************
// FUNCTION     : void ColorForceUpdate
// USAGE        : Force Update for color change
// INPUT        :
// OUTPUT       :
// GLOBALS      :
// USED_REGS    :
//*****************************************************************************
void far ColorForceUpdate(void)
{
   gm_ForceUpdate(gmvb_MainChannel, INPUT_OUTPUT_CLOCK);
   //gm_ForceUpdate(gmvb_MainChannel, OUTPUT_CLOCK);
}

#endif

#undef __COLOR_FIDELITY_CONTROL_C__

