/*
	$Workfile:   ad9888.c  $
	$Revision: 1.4 $
	$Date: 2011/12/13 10:32:42 $
*/

#define __ad9888_c__

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
// MODULE:	AD9888.c
//
// USAGE:	This module contains ADC - AD9888A programming routines
//
//******************************************************************************


//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "System\all.h"
#ifdef ADC_AD9888

// Default Control register initializations
#define SYNC_CTRL_DEFAULT         0x4B
#define SYNC_CLMPCOAST_DEFAULT    0x66 //as per AD9888A datasheet


//******************************************************************************
//  C O D E
//******************************************************************************

//******************************************************************************
//
// FUNCTION       : void adc_Init(void)
//
// USAGE          : Initialization of AD9888
//                  Programs the control register
//                  Sync Control:0x5B
//                  Hsync input polarity determined auto by chip, output is set to high
//                  Active Hsync - SOG
//                  Active Vsync - SOG
//                  Clamp+Coast Control:0x0F
//                  Internal Clamp(Hsync),active high,Coast pol auto detected,Allow low
//                  power mode,Normal operation
//                  Coast Programming:
//                  Coast Select= Internal Vsync
//                  Coast polarity= User defined.
//                  polarity= Active High
//                  Start=End=10
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void adc_Init(void)
{
   adc_Write08(AD9888_SYNC_CONTROL, SYNC_CTRL_DEFAULT);

#ifdef AD9888_INTERNAL_COASTING
   adc_Write08(AD9888_CLMPCOAST_CONTROL, SYNC_CLMPCOAST_DEFAULT);
#else
   adc_Write08(AD9888_CLMPCOAST_CONTROL, 0x46);
#endif
   adc_Write08(AD9888_SYNC_SEP_THRSHOLD, 0x20);

   // DEBUG -  the coast position pgming may have to be programmed after mode detect
   adc_Write08(AD9888_PRE_COAST, 0x0A);
   adc_Write08(AD9888_POST_COAST, 0x0A);

   // Clamp start = 8 clocks from falling edge of regenerated Hsync
   adc_Write08(AD9888_CLPLACE, 0x08);
//	adc_Write08(AD9888_CLPLACE, 0x28);
   // Clamp window with = 10 clokcs
   adc_Write08(AD9888_CLDUR, 0x0A);

//	adc_Write08(AD9888_SOG_THRESHOLDAD9888_SOG_THRESHOLD, 0xBE);  //clamp = mid level for YUV input
//	adc_Write08(AD9888_SOG_THRESHOLD, 0x7e);  //Brian
   // brian use the 0x7e
   adc_Write08(AD9888_SOG_THRESHOLD, 0xAE);

#ifdef AD9888_INTERNAL_COASTING
   adc_Write08(AD9888_OUTPUT_CONTROL, 0x02);
#else
   adc_Write08(AD9888_OUTPUT_CONTROL, 0x02);
//   adc_Write08(AD9888_OUTPUT_CONTROL, 0x42); // brian
#endif

   adc_SetGain(RED_CHANNEL, 0x80);
   adc_SetGain(GREEN_CHANNEL, 0x80); //ff
   adc_SetGain(BLUE_CHANNEL, 0x80);  //b0
   adc_SetOffset(RED_CHANNEL, 0x9B);
   adc_SetOffset(GREEN_CHANNEL, 0x4C);
   adc_SetOffset(BLUE_CHANNEL, 0xA2);

   adc_Write08(0x16, 0xFE);
   adc_Write08(0x17, 0x00);
}



// PLL value computation parameters
#define PLL_STATBILITY_RATIO     20
#define LOOP_FILTER_CAPACITANCE  39

typedef struct CHARGE_PUMP_IP_TBL_
{
   WORD  range;
   BYTE  ipValue;
} CHARGE_PUMP_IP_TBL;

CHARGE_PUMP_IP_TBL ROM ChargePumpIpTable[8] =
{
   {50,   0x00},
   {100,  0x01},
   {150,  0x02},
   {250,  0x03},
   {350,  0x04},
   {500,  0x05},
   {750,  0x06},
   {1500, 0x07}
};
//******************************************************************************
//
// FUNCTION       : void adc_SetClock(WORD W_hTotal,WORD W_vTotal,BYTE B_hPolarity,BYTE B_refreshRate)
//
// USAGE          : Programs PLL divider - sets the clock
//
// INPUT          : W_hTotal
//                  W_vTotal
//                  B_hPolarity
//                  B_refreshRate
//
// OUTPUT         : None
//
// GLOBALS        : ChargePumpIpTable (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
// NOTE: This function may be optimized
void adc_SetClock(WORD W_hTotal,WORD W_vTotal,BYTE B_hPolarity,BYTE B_refreshRate)
{
   DWORD D_kVco;
   BYTE  B_vcoIpValue;

   // VCO range
   {
      DWORD D_clockFreq;

      D_clockFreq = (DWORD)W_hTotal * (DWORD)W_vTotal * (DWORD)B_refreshRate;
      if (D_clockFreq < 45000000L)
      {
         B_vcoIpValue = 0x00;
         D_kVco = 22500;
      }
      else if (D_clockFreq < 90000000L)
      {
         B_vcoIpValue = 0x40;
         D_kVco = 45000L;
      }
      else if (D_clockFreq < 150000000L)
      {
         B_vcoIpValue = 0x80;
         D_kVco = 90000L;
      }
      else
      {
         B_vcoIpValue = 0xC0;
         D_kVco = 180000L;
      }
   }

   // Ip range
   {
      DWORD D_hsyncFreq;
      DWORD D_chargePumpIp;
      BYTE B_count;

      //Ip <= [(Hsync Freq*2pi)/(19.5)]^2 * [(Ct*N)/Kvco]
      //Ip <= [(Hsync Freq*2pi)/(19.5)]^2 * [(Ct*clockFreq/Hsync Freq)/Kvco]
      //Ip <= [(Hsync Freq*2pi)/(19.5)]^2 * [(Ct*hTotal)/Kvco]
      D_hsyncFreq = (DWORD)W_vTotal * B_refreshRate;

      D_chargePumpIp = (D_hsyncFreq * 628L) / 1950L;
      D_chargePumpIp = D_chargePumpIp * D_chargePumpIp;
      D_chargePumpIp = (D_chargePumpIp / D_kVco) * LOOP_FILTER_CAPACITANCE;
      D_chargePumpIp = (D_chargePumpIp * W_hTotal) / 1000000L;

      for (B_count = 0;B_count < 8;B_count++)
      {
         if (D_chargePumpIp < ChargePumpIpTable[B_count].range)
         {
            B_vcoIpValue |= (ChargePumpIpTable[B_count].ipValue << 3);
            break;
         }
      }

   }

   // Control register setting
   {
      BYTE  B_regValue;

      B_regValue = B_vcoIpValue & (BYTE)0x3F;
      adc_Write08(AD9888_VCOCTRL,B_regValue); //Set VCO clear

      // sync control
      B_regValue = SYNC_CTRL_DEFAULT;
      if (B_hPolarity == 1)  // positive
         B_regValue |= 0x40; // Edge=1 for + Pol, 0 for - Pol
      adc_Write08(AD9888_SYNC_CONTROL, B_regValue);

      // PLL divider programming
      B_regValue = (BYTE) (((W_hTotal - 1) & 0x0FF0) >> 4);
      adc_Write08(AD9888_PLLDIVM, B_regValue);
      B_regValue = (BYTE) (((W_hTotal - 1) & 0x000F) << 4);
      adc_Write08(AD9888_PLLDIVL, B_regValue);

      //VCO programming
      adc_Write08(AD9888_VCOCTRL,B_vcoIpValue);
   }

}


//******************************************************************************
//
// FUNCTION       : void adc_SetPhase(BYTE B_phaseVal)
//
// USAGE          : Programs the Clock phase
//
// INPUT          : B_phaseVal - phase to be set
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
// NOTE: This function may be optimized
void adc_SetPhase(BYTE B_phaseVal)
{
   adc_Write08(AD9888_PHASE,(BYTE)((B_phaseVal & 0x1F) << 3));
}


//******************************************************************************
//
// FUNCTION       : void adc_SetGain(BYTE B_channel, BYTE B_contrast)
//
// USAGE          : Programs the Gain value for given channel
//
// INPUT          : B_channel - selected channel (0-R, 1-G, 2-B)
//                  B_contrast - gain to be set (0-255)
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void adc_SetGain(BYTE B_channel, BYTE B_contrast)
{
   adc_Write08((BYTE)(AD9888_GAINR + B_channel),(BYTE)~(B_contrast));
}

//******************************************************************************
//
// FUNCTION       : void adc_SetOffset(BYTE B_channel, BYTE B_blackLevel)
//
// USAGE          : Programs the Blacklevel offset value for given channel
//
// INPUT          : B_channel - selected channel (0-R, 1-G, 2-B)
//                  B_blackLevel - blacklevel to be set (0-63)
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void adc_SetOffset(BYTE B_channel, BYTE B_blackLevel)
{
   adc_Write08((BYTE)(AD9888_OFFSETR + B_channel),((BYTE)(B_blackLevel /*<< 2*/)));
}


//******************************************************************************
//
// FUNCTION       : void adc_SetClampLevel(BYTE B_level)
//
// USAGE          : Programs the mid level clamp setting
//
// INPUT          : B_level - clamp level selection
//                     0 = Ground level clamp for RGB inputs
//                     1 = midi level clamp for YUV inputs
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void adc_SetClampLevel(BYTE B_level)
{
   if (B_level)
      adc_Write08(AD9888_SOG_THRESHOLD, 0xBF);  //clamp = mid level for YUV input
   else
      adc_Write08(AD9888_SOG_THRESHOLD, 0xB8);  //clamp = ground level for RGB input

}


//******************************************************************************
//
// FUNCTION       : void adc_SetCoast(BYTE B_preCoastCount,BYTE B_postCoastCount)
//
// USAGE          : Programs the coast generation window wrt internally extracted VSYNC
//
// INPUT          : B_preCoastCount - number of clocks before extracted Vsync
//                  B_postCoastCount - number of clocks after extracted Vsync
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void adc_SetCoast(BYTE B_preCoastCount,BYTE B_postCoastCount)
{
   adc_Write08(AD9888_PRE_COAST, B_preCoastCount);
   adc_Write08(AD9888_POST_COAST, B_postCoastCount);

} // end - adc_SetCoast

//******************************************************************************
//
// FUNCTION       : void adc_Write08(BYTE B_regAddr,BYTE B_value)
//
// USAGE          : Programs the given value to given register of AD9888
//
// INPUT          : B_regAddr - register address
//                  B_value - value to be written
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void adc_Write08(BYTE B_regAddr,BYTE B_value)
{
   BYTE  temp[2];

   temp[0] = B_regAddr;
   temp[1] = B_value;

   gm_Write2WireBlock(0, AD9888_DEV_ID, (BYTE *)temp, 2, TRUE);

}

//******************************************************************************
//
// FUNCTION       : BYTE adc_Read08(BYTE B_regAddr)
//
// USAGE          : Reads the given register of AD9888
//
// INPUT          : B_regAddr - register address
//
// OUTPUT         : reg Value read
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BYTE adc_Read08(BYTE B_regAddr)
{
   BYTE rdata;
   BYTE address;

   address = B_regAddr;

   gm_Write2WireBlock(0, AD9888_DEV_ID, (BYTE *)&address, 1, FALSE);
   gm_Read2WireBlock(0, AD9888_DEV_ID, (BYTE *)&rdata, 1, TRUE);

   return rdata;

}


//******************************************************************************
//
// FUNCTION       : void adc_PowerUp(void)
//
// USAGE          : Power up AD9888
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void adc_PowerUp(void)
{
   adc_Write08(AD9888_CLMPCOAST_CONTROL,
               (adc_Read08(AD9888_CLMPCOAST_CONTROL)| 0x01));
}

//******************************************************************************
//
// FUNCTION       : void adc_PowerDown(void)
//
// USAGE          : Chip power down of AD9888
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void adc_PowerDown(void)
{
   adc_Write08(AD9888_CLMPCOAST_CONTROL,
               (adc_Read08(AD9888_CLMPCOAST_CONTROL)& 0xFE));
}

//******************************************************************************
//
// FUNCTION       : BYTE adc_GetSyncStatus(void)
//
// USAGE          : Get the sync state of AD9888
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BYTE adc_GetSyncStatus(void)
{
   return adc_Read08(AD9888_SYNC_DETECT);

}

/*
BYTE adc_GetStatus(void)
{
	return adc_Read08(AD9888_SYNC_DETECT);

}
*/

//******************************************************************************
//
// FUNCTION       : void adc_Setup (WORD InputVTotal, BYTE InterlacedMode)
//
// USAGE          : Set window coast in order to remove the Macrovision effects for the display
//
// INPUT          : InputVTotal
//                  InterlacedMode
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void adc_Setup (WORD InputVTotal, BYTE InterlacedMode)
{
   // TODO: magic numbers should be determined for proper functioning
   // to avoid compiler warning
   InterlacedMode = InterlacedMode;

   // numbers for for 480i, 480p, 625i
   if ((InputVTotal == 525/2)	||
         (InputVTotal == 525 )	||
         (InputVTotal == 625/2 ))
   {
      adc_Write08(AD9888_CLPLACE, 0x08);
   }

   // numbers for for 625p
   else if (InputVTotal == 625 )
   {
      adc_SetCoast(10, 40);
   }
   // for the rest of the standard video modes
   // THAT HAS NO MACROVISION
   else
   {
      adc_Write08(AD9888_CLPLACE, 0x28);
   }
}

#endif //ADC_AD9888
//**********************************  END  *************************************
