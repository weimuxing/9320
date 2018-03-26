//****************************************************************************
// USAGE:
// INPUT:  SDWORD min, max -- the minimum and maximum range, in fixed 1.7.24 format
//         BYTE inBits, outBits -- # of bits for LUT input and output
// implied FLUT:
//               WORD* FLUT -- offset and scaled 16-bit full LUT content
//               WORD numOfEntries
//               SDWORD fmin, SDWORD fmax  - in 1.7.24 format, 1 sign bit, 7 bit integer, 24-bit fractional part
// OUTPUT:
// GLOBALS:
// USED_REGS:
//*****************************************************************************
#if 1
#include <mem.h>
#include "System\All.h"
#else
#include <mem.h>
#include "ColorFidelity\GlobalDefine.h"
#include "ColorFidelity\DynamicGamutMapping.h"
#include "ColorFidelity\gm_Register_Stdp93xx.h"
#include "ColorFidelity\gm_HostReg.h"
#endif

#ifdef USE_COLOR_FIDELITY

#define DEBUG_GAMUT_MAPPING 1



#if DEBUG_GAMUT_MAPPING
#define msg(a,b)	gm_Print(a,b)
#else
#define msg(a,b)
#endif


#if USE_DYNAMIC_GAMUTMAPPING_SUPPORT

#define gamutBufSize		44
#define gamutDataUpdateFlagOffset  42

BYTE commonGamutDataBuf[gamutBufSize];
BYTE WGS_colorSpaceMode = 0;

static BYTE brightness_reg_value;
static BYTE postLUTLoadedFlag = 0;

WGSCapStruct ROM defaultDisplayWGS =
{
   0x00,
   0x01,   // only support source simplified color profile
   {
      200,    // typical white luminance
      80,     // minimum white luminance
      250     // maximum white luminance
   },
   0x00
};

#if 0
typedef enum
{
   RED_VLUT_COMPONENT      = 0,
   GREEN_VLUT_COMPONENT = 1,
   BLUE_VLUT_COMPONENT     = 2
} gmt_VLUT_COLOR_COMPONENT;

typedef enum
{
   CH_A = 0,                     // Physical Channel A
   CH_B,                      // Physical Channel B
   CH_PHY_NUM                    // Number of Physical Channels
} gmt_PHY_CH;

typedef enum
{
   INPUT_CLOCK,                  // Update with input clock
   OUTPUT_CLOCK,                       // Update with output/display clock
   INPUT_OUTPUT_CLOCK,               // Update both input/output clock
   GMT_HOST_UPDATE_SIZE
} gmt_HOST_UPDATE;

#endif

/* mul64 - multiply two 32 bit numbers giving a 64 bit product */
#define HALFBITS        16
#define HALFMASK        0xFFFF
void gmp_mul64(DWORD a, DWORD b, DWORD *pHigh, DWORD *pLow)
{
   DWORD a1,a0,b1,b0,t3,t2,t1,r3,r2,r1,r0;

   /* take apart the operands */
   a1 = (DWORD)a >> HALFBITS;
   a0 = a & HALFMASK;
   b1 = (DWORD)b >> HALFBITS;
   b0 = b & HALFMASK;

   /* do the multiplies */
   r0 = b0 * a0;
   t1 = b0 * a1;
   t2 = b1 * a0;
   t3 = b1 * a1;

   /* compute the result */
   r1 = (r0 >> HALFBITS) + (t1 & HALFMASK) + (t2 & HALFMASK);
   r2 = (r1 >> HALFBITS) + (t1 >> HALFBITS) + (t2 >> HALFBITS) + (t3 & HALFMASK);
   r3 = (r2 >> HALFBITS) + (t3 >> HALFBITS);

   /* assemble the result */
   r0 = (r1 << HALFBITS) | (r0 & HALFMASK);
   r1 = (r3 << HALFBITS) | (r2 & HALFMASK);

   /* return the two halves of the result */
   *pHigh = (DWORD)r1;
   *pLow = (DWORD)r0;
}

/*
   Copyright stuff

   Use of this program, for any purpose, is granted the author,
   Ian Kaplan, as long as this copyright notice is included in
   the source code or any source code derived from this program.
   The user assumes all responsibility for using this code.

   Ian Kaplan, October 1996

*/

/* ushift64 - unsigned shift a 64 bit value */
void gmp_ushift64(SDWORD n1,SDWORD n0, SWORD shift,SDWORD *pHigh,SDWORD *pLow)
{
   SDWORD r1,r0;

   /* do the shift */
   if (shift > 32)
   {
      r0 = (DWORD)n1 >> (shift - 32);
      r1 = n1 < 0 ? -1 : 0;
   }
   else if (shift > 0)
   {
      r0 = (n1 << (32 - shift)) | ((DWORD)n0 >> shift);
      r1 = (DWORD)n1 >> shift;
   }
   else if (shift > -32)
   {
      r0 = n0 << -shift;
      r1 = (n1 << -shift) | ((DWORD)n0 >> (32 + shift));
   }
   else
   {
      r0 = 0;
      r1 = n0 << (-shift - 32);
   }

   /* return the two halves of the result */
   *pHigh = r1;
   *pLow = r0;
}
// Converted Ian Kaplan's routine to 64 bit divide:
void gmp_unsigned_divide64(DWORD dividend_hi, DWORD dividend_lo,
                           DWORD divisor,DWORD *quotient_hi, DWORD *quotient_lo,
                           DWORD *remainder)
{
   DWORD t, num_bits;
   DWORD q, bit, d_hi, d_lo;
   WORD  i;

   *remainder = 0;
   *quotient_hi = 0;
   *quotient_lo = 0;

   if (divisor == 0)
      return;

   if ((dividend_hi == 0x00) && (divisor > dividend_lo))
   {
      *remainder = dividend_lo;
      return;
   }

   if ((dividend_hi == 0x00) && (divisor == dividend_lo))
   {
      *quotient_lo = 1;
      return;
   }

//  num_bits = 32;
   num_bits = 64;

   while (*remainder < divisor)
   {
      // bit = (dividend & 0x80000000UL) >> 31;
      bit = (dividend_hi & 0x80000000UL) >> 31;
      // *remainder = (*remainder << 1) | bit;
      *remainder = (*remainder << 1) | bit;
      // d = dividend;
      d_hi = dividend_hi;
      d_lo = dividend_lo;
      // dividend = dividend << 1;
      gmp_ushift64(dividend_hi,dividend_lo, -1, (SDWORD*)&dividend_hi,(SDWORD*)&dividend_lo);
      // num_bits--;
      num_bits--;
   }


   /* The loop, above, always goes one iteration too far.
      To avoid inserting an "if" statement inside the loop
      the last iteration is simply reversed. */

   // dividend = d;
   dividend_hi = d_hi;
   dividend_lo = d_lo;
   // *remainder = *remainder >> 1;
   *remainder = *remainder >> 1;
   // num_bits++;
   num_bits++;

   for (i = 0; i < num_bits; i++)
   {
//      bit = (dividend & 0x80000000UL) >> 31;
      bit = (dividend_hi & 0x80000000UL) >> 31;
//      *remainder = (*remainder << 1) | bit;
      *remainder = (*remainder << 1) | bit;
//      t = *remainder - divisor;
      t = *remainder - divisor;
//      q = !((t & 0x80000000UL) >> 31);
      q = !((t & 0x80000000UL) >> 31);
//      dividend = dividend << 1;
      gmp_ushift64(dividend_hi,dividend_lo, -1, (SDWORD*)&dividend_hi,(SDWORD*)&dividend_lo);
//      *quotient = (*quotient << 1) | q;
      gmp_ushift64(*quotient_hi, *quotient_lo, -1, (SDWORD*)quotient_hi,(SDWORD*)quotient_lo);
      *quotient_lo |= q;
      if (q)
      {
         *remainder = t;
      }
   }
}  /* unsigned_divide */

//*****************************************************************************
// FUNCTION     : void GetPanelGamutInfo
// USAGE        :
// INPUT        :
// OUTPUT       :
// GLOBALS      :
// USED_REGS    :
//*****************************************************************************
void GetPanelGamutInfo(GamutDataStruct* destGamut)
{
#if 1//PANEL == LM260WU2_SLA1
   destGamut->whiteLum = 340;
   destGamut->white_x = 0x5333;     //0.325
   destGamut->white_y = 0x5999;     //0.350
   destGamut->red_x = 0xA9BA;       //0.663
   destGamut->red_y = 0x52F1;       //0.324
   destGamut->green_x = 0x33F7;     //0.203
   destGamut->green_y = 0xB7CE;     //0.718
   destGamut->blue_x = 0x25E3;     //0.148
   destGamut->blue_y = 0x0E97;     //0.057
   destGamut->red_min = 0x0000;     //0.0
   destGamut->red_max = 0x2000;     //1.0
   destGamut->green_min = 0x0000;     //0.0
   destGamut->green_max = 0x2000;     //1.0
   destGamut->blue_min = 0x0000;     //0.0
   destGamut->blue_max = 0x2000;     //1.0
   destGamut->bias_0 = 0x0000;     //0.0
   destGamut->divisor_1 = 0x00FF;     //1.0
#endif
}

BYTE ROM defaultSrcGamutData[] =
{
   //sRGB2.4
   0x50, 0x00, 0x0D, 0x50, 0x39, 0x54, 0xD7, 0xA3, 0x7B, 0x54, 0xCD, 0x4C, 0x9A, 0x99, 0x66, 0x26, \
   0x5C, 0x0F, 0x29, 0x00, 0xAE, 0x33, 0x38, 0x00, 0x9A, 0x09, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, \
   0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0xFF, 0x00, 0x01,                               \

   //AdobeRGB
   0xA0, 0x00, 0x0D, 0x50, 0x39, 0x54, 0xD7, 0xA3, 0x0A, 0x57, 0xC3, 0x35, 0xC3, 0xB5, 0x66, 0x26, \
   0x5C, 0x0F, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0xCC, 0x08, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, \
   0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0xFF, 0x00, 0x01
};

//*****************************************************************************
// FUNCTION     :
// USAGE        :
// INPUT        :
// OUTPUT       :
// GLOBALS      :
// USED_REGS    :
//*****************************************************************************
WORD ROM PredefinedGamma_IndexTable[] =
{
   // Note: there is no actual gamma LUT data for gamma=1.0
   // the integer value is equal to gamma*2^10 for each entry.
   // gamma: 1.00, 1.10	1.20	1.30	1.40	1.50, 1.60, 1.70, 1.80, 1.90
   0x400, 0x466, 0x4CD, 0x533, 0x59A, 0x600, 0x666, 0x6CD, 0x733, 0x79A,
   // gamma: 0.05
   0x33,
};

#define NUM_GAMMA_LUT	11
#define LUT_ID_MASK		0x0F
#define LUT_SQUARED_MASK		0x10
#define LUT_INTERPOLATION_MASK	0x20
#define LUT_INTP_POWER_MASK		0xC0

//*****************************************************************************
// FUNCTION     : void FindGammaLutIndex
// USAGE        :
// INPUT        : WORD gamma , BYTE* lutId
// OUTPUT       :
// GLOBALS      :
// USED_REGS    :
//*****************************************************************************
void FindGammaLutIndex(WORD gamma, BYTE* lutId)
{
   // To find how gamma should be computed based on the based LUT and interpolation LUT
   // gamma range: 1.0 ~ 3.6
   // LUTID bit definition as below:
   // Bit3:0 -- index to 10 base LUTs (0-9), gamma=1.0 has index 0.
   // Bit7:4 -- indicates how interpolation will be done to compute gamma,
   //   Bit4   -- squared base LUT or not: 1 - squared x*x, 0 - no square
   //   Bit5   -- 0.05 gamma LUT used or not:, 1 - used, 0 - not used
   //   Bit7:6 -- 0.05 gamma LUT used how many times: 1~3

   SBYTE i;
   SBYTE index, k;
   WORD orig_gamma, base_gamma;
   SWORD delta, d2;


   if (gamma > 0x0FFF)	/// Limit the gamma less than 3.99
      gamma = 0x0FFF;
   else if (gamma < 0x0400)	/// Limit the gamma larger than 1.0
      gamma = 0x0400;

   msg("FindGammaLutIndex - 0x%x",gamma);

   (*lutId) = 0;
   //to make sure gamma is within base LUT range
   orig_gamma = gamma;

   if (gamma >= 0x800)
   {
      gamma = gamma/2;
      (*lutId) |= LUT_SQUARED_MASK;	//set bit 4 for squared (x)
      msg("Squared gamma",0);
   }
   //check the base gamma
   index = 0;
   for (i=NUM_GAMMA_LUT-2; i>=0; i--)
   {
      if (gamma >= PredefinedGamma_IndexTable[i])
      {
         index = i;
         break;
      }
   }

   msg("Found Gamma table index = %d",i);

   //check how interpolation gamma LUTs will be used
   base_gamma = PredefinedGamma_IndexTable[index];
   if (orig_gamma > gamma)   //gamma in range of (2.0, 3.6)
   {
      base_gamma = base_gamma*2;
      //msg("gamma 2.0 ~ 3.6",0);
   }
   delta = orig_gamma - base_gamma;
   k = (delta + PredefinedGamma_IndexTable[NUM_GAMMA_LUT-1] / 2) / PredefinedGamma_IndexTable[NUM_GAMMA_LUT-1];   //0.5 rounding

   //msg("base_gamma: 0x%x", base_gamma);
   //msg("delta1: 0x%x", delta);
   //msg("k1: 0x%x", k);

   //check if it is close to base gamma or next base gamma
   delta = orig_gamma - (base_gamma+((WORD)k)*PredefinedGamma_IndexTable[NUM_GAMMA_LUT-1]);

   //msg("delta2: 0x%x", delta);

   //gamma of next index
   if (index < NUM_GAMMA_LUT-2)
      base_gamma = PredefinedGamma_IndexTable[index+1];
   else
      base_gamma = PredefinedGamma_IndexTable[0]*2;

   if (orig_gamma > gamma)
      base_gamma = base_gamma*2;

   d2 = base_gamma - orig_gamma;

   //msg("d2: 0x%x", d2);

   if (d2 <= delta)
   {
      k = k+1;
      if (k > 3)  //for gamma > 2.0
      {
         index += 1;
         k = 0;
      }
   }

   msg("k2: 0x%x", k);

   if (index==(NUM_GAMMA_LUT-2) && (orig_gamma==gamma) && (k>=2))  //gamma value between (1.95 ~ 2.0)
   {
      index = 0;
      (*lutId) |= LUT_SQUARED_MASK;
      k = 0;
   }

   msg("LUT index = %d",index);

   (*lutId) |= index;
   if (k)
   {
      if (k>3) k = 3;

      (*lutId) |= ((k<<6) | LUT_INTERPOLATION_MASK);
   }

   msg("LutId = 0x%x",(*lutId));
   msg("==============================",0);
}

//*****************************************************************************
// FUNCTION     : void ComputeGammaValue
// USAGE        :
// INPUT        : WORD index, BYTE lutId, WORD* value
// OUTPUT       :
// GLOBALS      :
// USED_REGS    :
//*****************************************************************************
void ComputeGammaValue(WORD index, BYTE lutId, DWORD* value)
{
   //lutId: defined in the same format as in function FindGammaLutIndex()
   //index: the index of LUT input
#define GAMMALUT_BLOCK_SIZE		2048	//287
//#define GAMMALUT_MBLCK_SIZE		(32*2)
//#define LUTENTRY_STEP_SIZE		8
#define LUTVALUE_SCALE_FACTOR	65535	//8191     //2^13-1
   //for speed reason, this function decompress gamma segment by segment,
   //each segement has LUTENTRY_STEP_SIZE gamma entries.
   //The index is the main index, and the size has to be the same size as LUTENTRY_STEP_SIZE
   //except the last one which has only 7 entries from index 249 to 255.
   //If the index is not a main index, it only computes 1 entry.
   DWORD v[5]; //v[0]: for main entry; v[1]: for interpolation entry; v[2-3]: temp value
   WORD pos, i;

   if (index==0 || index>1023)
   {
      value[0] = 0;
      return;
   }

   if ((lutId & LUT_ID_MASK) == 0)  //no actual LUT, gamma=1.0
   {
      v[3] = (((DWORD)index + 1) << 6) - 1;
   }
   else
   {
      pos = GAMMALUT_BLOCK_SIZE * ((lutId & LUT_ID_MASK)-1) + index * 2;
      v[0] = PredefinedGammaLUTData[pos] + (PredefinedGammaLUTData[pos+1]<<8);
      v[3] = v[0] & 0xFFFF;
   }

   if (lutId & LUT_SQUARED_MASK)
   {
      // Square value
      gmp_mul64(v[0], v[0], &v[1], &v[2]);
      gmp_unsigned_divide64(v[1], v[2], LUTVALUE_SCALE_FACTOR, &v[1], &v[2], &v[3]);

      v[3] = v[2] & 0xFFFF;
   }

   if (lutId & LUT_INTERPOLATION_MASK)
   {
      BYTE k;

      pos = GAMMALUT_BLOCK_SIZE*(NUM_GAMMA_LUT-2) + index * 2;
      k = ((lutId & LUT_INTP_POWER_MASK) >> 6) - 1;

      //v[0] = 0;
      v[0] = ((PredefinedGammaLUTData[pos] + (PredefinedGammaLUTData[pos+1]<<8)) & 0xFFFF);
      v[1] = LUTVALUE_SCALE_FACTOR;

      while (k--)
      {
         //v[2] = (((v[2]*v[1]*2)/LUTVALUE_SCALE_FACTOR)+1)>>1;   //x^k, rounding
         gmp_mul64(v[0], v[1], &v[2], &v[1]);
         gmp_unsigned_divide64(v[2], v[1], LUTVALUE_SCALE_FACTOR, &v[2], &v[1], &v[4]);
      }

      //v[0] = (((v[0]*v[2]*2)/LUTVALUE_SCALE_FACTOR)+1)>>1;
      gmp_mul64(v[1], v[3], &v[1], &v[2]);
      gmp_unsigned_divide64(v[1], v[2], LUTVALUE_SCALE_FACTOR, &v[2], &v[3], &v[4]);
   }

   value[0] = v[3] & 0xFFFF;

#undef GAMMALUT_BLOCK_SIZE
//#undef GAMMALUT_MBLCK_SIZE
//#undef LUTENTRY_STEP_SIZE
#undef LUTVALUE_SCALE_FACTOR     //2^13-1
}
#undef NUM_GAMMA_LUT

//*****************************************************************************
// FUNCTION     : void LoadCustomGamma
// USAGE        :
//                NOTE: this function does not support extended RGB range.
//                It assumes that RGB are in 0.0 to 1.0 range.
//                Future work:
//                This function needs to be combined with function  WGS_CustomizingLUT()
// INPUT        : const GamutDataStruct* gamutData
// OUTPUT       : update LUT hardware
// GLOBALS      :
// USED_REGS    :
//*****************************************************************************
void far LoadCustomGamma(const GamutDataStruct* gamutData)
{
   BYTE lutId;
   WORD i, j, index;
   DWORD GammaTempVal;	//value[8];
   WORD a0, a1, a2, a3;
#if 1 // Compress
   BYTE B[8] = {0};
   SBYTE SB[8] = {0};
   WORD W_PreV = 0;
   BYTE B_Porality = 0;
#endif

   a0 =  gamutData->gamma_A0;
   a1 =  gamutData->gamma_A1;
   a2 =  gamutData->gamma_A2;
   a3 =  gamutData->gamma_A3;

   //if x<=a0, y=x/a1;
   //if x>a0, y = [(x+a2)/(1+a2)]^a3

#if 0 //temporary bug fix for wrong gamma passed from Portrait tool /// Disabled. It improved in PhotoTune v0.10
   if (a0==0x29 && a1==0x33ae) //sRGB
   {
      a2 = 0x38;
      a3 = 0x99A;
   }
   else if (a0==0x00 && a1==0x400) //AdobeRGB
   {
      a2 = 0x00;
      a3 = 0x8CC;
   }
   else
   {
      a2 = 0x00;
      a3 = 0x99A;
   }
#endif

   FindGammaLutIndex(a3, &lutId);

   PreLUTAccessEnable();

   i = 0;
   while (i<1024)
   {
      index = i;
      if (i <= a0)
      {
         //GammaTempVal = (WORD)(((((DWORD)(((DWORD)index)<<15))/(DWORD)a1)+1)>>3);     //+1 rounding, result is 12-bit value
         GammaTempVal = (WORD)(((((DWORD)(((DWORD)index)<<15))/(DWORD)a1)+1)>>1);     //+1 rounding, result is 14-bit value
      }
      else
      {
         if (a2 != 0)
         {
            index = (WORD)((((DWORD)(i+a2)<<11)/((DWORD)(0x400+a2))+1)>>1);	//+1 rounding, index: 10-bit value
         }

         ComputeGammaValue(index, lutId, &GammaTempVal);
         //GammaTempVal = GammaTempVal >> 4; // 12-bit
         GammaTempVal = GammaTempVal >> 2; // 14-bit
      }

      { // Compress
         if ((i%8) == 0)
         {
            // init buffer array
            memset(B, 0, 8);

            // First 14-bit
            B[0] = (BYTE)(GammaTempVal >> 6);
            B[1] = (BYTE)((GammaTempVal & 0x3F) << 2);

            W_PreV = (WORD)GammaTempVal;
         }
         else
         {
            SB[i%8] = (SBYTE)(GammaTempVal - W_PreV);

            W_PreV = (WORD)GammaTempVal;

            //if((SB[i%8] > 127) || (SB[i%8] < -64))
            if (SB[i%8] < -64)
               gm_Printf("Out of Range : index %d delta 0x%x", i, SB[i%8]);
         }

         if ((SB[i%8] < 0) && ((B_Porality & BIT0) == 0))
            B_Porality |= BIT0;

         if ((i%8) == 7)
         {

            if (B_Porality & BIT0)
            {
               SBYTE j = 7;

               for (; j >= 0; j--)
               {
                  if (SB[j] > 63)
                     gm_Printf("Out of Range 2 : index %d delta 0x%x", i-j, SB[7 - j]);
               }
            }



            B[1] = B[1] | ((SB[1] & 0x60) >> 5);
            B[2] = (BYTE)((SB[1] & 0x1F) << 3) | ((SB[2] & 0x70) >> 4);
            B[3] = (BYTE)((SB[2] & 0xF) << 4) | ((SB[3] & 0x78) >> 3);
            B[4] = (BYTE)((SB[3] & 0x7) << 5) | ((SB[4] & 0x7C) >> 2);
            B[5] = (BYTE)((SB[4] & 0x3) << 6) | ((SB[5] & 0x7E) >> 1);
            B[6] = (BYTE)((SB[5] & 0x1) << 7) | (SB[6] & 0x7F);
            B[7] = (BYTE)((SB[7] & 0x7F) << 1) | B_Porality;

            B_Porality = 0; // reset porality

            // Program into memory
            {
               SBYTE j = 7;

               for (;j >= 0; j--)
               {
                  WritePreLutRed(i - j, B[7 - j]);
                  WritePreLutGreen(i - j, B[7 - j]);
                  WritePreLutBlue(i - j, B[7 - j]);
               }
            }
         }
      }

      i++;

   }

   //Do not update postLUT every time, just load it once
   if (!postLUTLoadedFlag)
   {
      // Need to make sure the table is 14-bit compress table
      WritePostLut();

      postLUTLoadedFlag = 1;
   }

   ColorForceUpdate();
}

#define BITS1_SHIFT		2
#define BITS2_SHIFT		(16-BITS1_SHIFT)

//*****************************************************************************
// FUNCTION     : static void  Matrix_MxM
// USAGE        : Matrix multiply.  Multiplies 3x3 matrix A with
//                3x3 matrix B, storing the results back in A.
// INPUT        : SDWORD *A, SDWORD *B
// OUTPUT       :
// GLOBALS      :
// USED_REGS    :
//*****************************************************************************
static void  Matrix_MxM(SDWORD *A, const SDWORD *B)
{
   BYTE row,col;
   SDWORD M[9];
   SDWORD a,b,c;

   for (row=0;row<3;row++)
   {
      for (col=0;col<3;col++)
      {
         a = A[row * 3 + 0] * (B[0 * 3 + col]>>BITS1_SHIFT);
         b = A[row * 3 + 1] * (B[1 * 3 + col]>>BITS1_SHIFT);
         c = A[row * 3 + 2] * (B[2 * 3 + col]>>BITS1_SHIFT);
         M[row*3 + col] = (a + b + c) >> BITS2_SHIFT;
      }
   }
   memcpy(A,M,sizeof(M));
}

//*****************************************************************************
// FUNCTION     : static void  Matrix_MxV
// USAGE        : Matrix multiply.  Multiplies 3x3 matrix A with
//                vector V, storing the results back to V.
// INPUT        : SDWORD *A, SDWORD *V
// OUTPUT       :
// GLOBALS      :
// USED_REGS    :
//*****************************************************************************
static void  Matrix_MxV(const SDWORD *A, SDWORD *V)
{
   BYTE row;
   SDWORD M[3];
   SDWORD a,b,c;

   for (row=0;row<3;row++)
   {
      a = A[row * 3 + 0] * (V[0]>>BITS1_SHIFT);
      b = A[row * 3 + 1] * (V[1]>>BITS1_SHIFT);
      c = A[row * 3 + 2] * (V[2]>>BITS1_SHIFT);
      M[row] = (a + b + c)>>BITS2_SHIFT;
   }
   memcpy(V,M,sizeof(M));
}

//*****************************************************************************
// FUNCTION     : static void  Matrix_MxCV
// USAGE        : Matrix multiply.  Multiplies 3x3 matrix A with
//                column vector CV, i.e., Aij = Aij*CV[j],
//						storing the results back to matrix A.
// INPUT        : SDWORD *A, SDWORD *CV
// OUTPUT       :
// GLOBALS      :
// USED_REGS    :
//*****************************************************************************
static void  Matrix_MxCV(SDWORD *A, const SDWORD *CV)
{
   BYTE row;

   for (row=0;row<3;row++)
   {
      A[row * 3 + 0] = (A[row * 3 + 0] * (CV[0]>>BITS1_SHIFT))>>BITS2_SHIFT;
      A[row * 3 + 1] = (A[row * 3 + 1] * (CV[1]>>BITS1_SHIFT))>>BITS2_SHIFT;
      A[row * 3 + 2] = (A[row * 3 + 2] * (CV[2]>>BITS1_SHIFT))>>BITS2_SHIFT;
   }
}

//*****************************************************************************
// FUNCTION     : static BYTE  Matrix_MInverse
// USAGE        : Matrix inverse. To compute the inverse matrix of 3x3 matrix A,
//                storing the results in B.
// INPUT        : SDWORD *A, SDWORD *B
// OUTPUT       :
// GLOBALS      :
// USED_REGS    :
//*****************************************************************************
static BYTE  Matrix_MInverse(const SDWORD *A, SDWORD *B)
{
   //NOTE:
   //1) the actual element values of matrix A are right shifted 16-bit.
   //2) each element decimal value of matrix A should not be out of ragne [-4.0, +4.0]
   //3) be carefull of the overflow of  intermediate computation results due to 32-bit precision
   BYTE id;
   SDWORD M[9];
   SDWORD a,b,c, detA;

   //The inverse of matrix A = [adj(A)]' / |A|,  where
   //matrix A:
   //    		a11	a12	a13     	linear index:		0		1		2
   //    		a21	a22	a23                     	3     4     5
   //    		a31	a32	a33                     	6     7     8
   //|A|: the determinant of matrix A: det(A)
   //   det(A) = 	a11*|a22	a23|	-	a12*|a21	a23|	+ a13*|a21	a22|
   //						 |a32	a33|			 |a31	a33|			|a31	a32|
   //
   //[adj(A)]': the transpose of adjoint matrix of A, indicated as below
   //     |a22	a23|		-|a12		a13|			|a12		a13|
   //	  |a32	a33|		 |a32		a33|			|a22		a23|
   //
   //    -|a21	a23|		 |a11		a13|		  -|a11		a13|
   //	  |a31	a33|		 |a31		a33|			|a21		a23|
   //
   //     |a21	a22|		-|a11		a12|			|a11		a12|
   //	  |a31	a32|		 |a31		a32|			|a21		a22|
   //msg("Print A:", 0);
   //PrintMatrix(A);

   //make sure there will be no overflow by right shifting element value down 2-bit
   for (id=0; id<9; id++)
   {
      B[id] = (A[id]>>BITS1_SHIFT);
   }

   a = B[0]*((B[4]*B[8] - B[5]*B[7])>>BITS2_SHIFT);       //NOTE: over flow
   b = B[1]*((B[5]*B[6] - B[3]*B[8])>>BITS2_SHIFT);
   c = B[2]*((B[3]*B[7] - B[4]*B[6])>>BITS2_SHIFT);
   detA = (a+b+c)>>16;

   //msg("a_lo: 0x%x", (WORD)a);
   //msg("a_hi: 0x%x", (WORD)(a>>16));
   //msg("b_lo: 0x%x", (WORD)b);
   //msg("b_hi: 0x%x", (WORD)(b>>16));
   //msg("c_lo: 0x%x", (WORD)c);
   //msg("c_hi: 0x%x", (WORD)(c>>16));
   //msg("det(A) lo: 0x%x", (WORD)detA);
   //msg("det(A) hi: 0x%x", (WORD)(detA>>16));

   if (detA==0)return 0xff;   //inverse of matrix does not exist!

   M[0] = B[4]*B[8] - B[5]*B[7];
   M[1] = B[2]*B[7] - B[1]*B[8];
   M[2] = B[1]*B[5] - B[2]*B[4];
   M[3] = B[5]*B[6] - B[3]*B[8];
   M[4] = B[0]*B[8] - B[2]*B[6];
   M[5] = B[2]*B[3] - B[0]*B[5];
   M[6] = B[3]*B[7] - B[4]*B[6];
   M[7] = B[1]*B[6] - B[0]*B[7];
   M[8] = B[0]*B[4] - B[1]*B[3];

   //msg("Print M:", 0);
   //PrintMatrix((const SDWORD*)&M);

   for (id=0;id<9;id++)
   {
      M[id] = M[id]/detA;
   }

   memcpy(B,M,sizeof(M));

   //msg("Print Matrix_MInverse result matrix B:", 0);
   //PrintMatrix((const SDWORD*)B);

   return 0;
}

//*****************************************************************************
// FUNCTION     : BYTE  ComputeMatrixFromGamutData
// USAGE        : To compute rgb2xyz conversion matrix from gamut data,
//                storing the results in rgb2xyz.
// INPUT        : GamutDataStruct *gamutData, SDWORD *rgb2xyz
// OUTPUT       : return: 0-successful, 1-failed
// GLOBALS      :
// USED_REGS    :
//*****************************************************************************
BYTE ComputeMatrixFromGamutData(const GamutDataStruct* gamutData, SDWORD* rgb2xyz)
{
   BYTE ret;
   SDWORD M[9], tmpM[9], vc[3];

   //matrix elements:
   // red    green    blue    index:
   // r_x    g_x      b_x     0   1   2
   // r_y    g_y      b_y     3   4   5
   // r_z    g_z      b_z     6   7   8
   M[0] = gamutData->red_x;
   M[1] = gamutData->green_x;
   M[2] = gamutData->blue_x;
   M[3] = gamutData->red_y;
   M[4] = gamutData->green_y;
   M[5] = gamutData->blue_y;
   M[6] = 0x10000L - (M[0]+M[3]);
   M[7] = 0x10000L - (M[1]+M[4]);
   M[8] = 0x10000L - (M[2]+M[5]);

   //msg("ComputeMatrixFromGamutData:", 0);

   //to compute K vector from normalized white: K = Inverse(M)*white_vector_normal
   ret =  Matrix_MInverse((const SDWORD *)M, (SDWORD*)tmpM);
   if (ret) return ret;	//return if failed

   //msg("Print Matrix_MInverse:", 0);
   //PrintMatrix((const SDWORD*)&tmpM);

   vc[0] = vc[1] = vc[2] = 0x00;	//initialization
   if (gamutData->white_y)
   {
      vc[0] = (((SDWORD)gamutData->white_x)<<16) / gamutData->white_y;   // x/y
      vc[1] = 0x10000L;                                                   // y=1
      vc[2] = 0x10000L - (gamutData->white_x + gamutData->white_y);       // z = 1-(x+y)
      vc[2] = (((SDWORD)vc[2])<<16) / gamutData->white_y;                // z/y
   }
   Matrix_MxV((const SDWORD *)&tmpM, (SDWORD*)vc);

   //to compute the matrix of rgb2xyz
   Matrix_MxCV((SDWORD*)M, (const SDWORD *)vc);	//M is the matrix of rgb2xyz
   memcpy(rgb2xyz,M,sizeof(M));

   //msg("Print Matrix_MxCV:", 0);
   //PrintMatrix((const SDWORD*)rgb2xyz);

   return ret;
}

//*****************************************************************************
// FUNCTION     : BYTE ComputeGamutMappingMatrix
// USAGE        :
// INPUT        :
// OUTPUT       :
// GLOBALS      :
// USED_REGS    :
//*****************************************************************************
BYTE ComputeGamutMappingMatrix(const GamutDataStruct* srcGamut, const GamutDataStruct* destGamut, SDWORD* maxtrixBuf)
{
   BYTE ret;
   SDWORD src_rgb2xyz[9];
   SDWORD dst_rgb2xyz[9];
   SDWORD M[9];

   //to get source RGB2XYZ conversion from srcGamut data:
   ret = ComputeMatrixFromGamutData(srcGamut, src_rgb2xyz);
   if (ret) return ret;

   //to get XYZ2PanelRGB conversion from destGamut data:
   ret = ComputeMatrixFromGamutData(destGamut, dst_rgb2xyz);
   if (ret) return ret;

   //to compute gamut mapping matrix
   ret =  Matrix_MInverse(dst_rgb2xyz, M);
   Matrix_MxM(M, src_rgb2xyz);

   memcpy(maxtrixBuf,M,sizeof(M));

   return ret;
}

//*****************************************************************************
// FUNCTION     : void SetDefaultSourceGamutData
// USAGE        :
// INPUT        :
// OUTPUT       :
// GLOBALS      :
// USED_REGS    :
//*****************************************************************************
void SetDefaultSourceGamutData(BYTE* buf)
{
   static BYTE flag=0;

   if (flag==0)
   {
      _fmemcpy(buf, defaultSrcGamutData, sizeof(defaultSrcGamutData)/2);
   }
   else
   {
      _fmemcpy(buf, defaultSrcGamutData+(sizeof(defaultSrcGamutData)/2), sizeof(defaultSrcGamutData)/2);
   }

   flag = !flag;
}

//*****************************************************************************
// FUNCTION     : void LoadMappingMatrix
// USAGE        :
// INPUT        :
// OUTPUT       :
// GLOBALS      :
// USED_REGS    :
//*****************************************************************************
void  LoadMappingMatrix(const SDWORD* matrix)
{
   BYTE i;
   WORD value;

   for (i=0; i<9; i++)
   {
      value = (WORD)(( ((DWORD *)matrix )[i] )>>8);
#if 1
      //gm_WriteRegWord(MAIN_P_MULT_COEF11   + (i * 2),	value );
      //gm_WriteRegWord(MAIN_S_MULT_COEF11   + (i * 2),	value );
      gm_WriteRegWord(GAMMA_LUT_MATRIX_MULT_COEF11   + (i * 2),	value );
#else
      gm_WriteRegWord(MAIN_MULT_COEF11_A   + (i * 2),	value );
      gm_WriteRegWord(MAIN_MULT_COEF11_B   + (i * 2),	value );
#endif
   }

   //for gamut mapping matrix
#if 1 // No more ...	 
   //gm_WriteRegByte(COLOR_MATRIX_CTRL, 0x0303);	//  both inside&outside window are RGB format
#else
   gm_WriteRegByte(MAIN_MULT_CTRL, 0x0303);	//  both inside&outside window are RGB format
#endif

   //gm_ForceUpdate(CH_A, OUTPUT_CLOCK);
   //ColorForceUpdate();
   gm_SyncUpdate(CH_A, INPUT_OUTPUT_CLOCK);
   //gm_SetRegBitsByte(OVP_UPDATE_CTRL, OVP_SYNC_UPDATE);
}

//*****************************************************************************
//*****************************************************************************
BYTE ROM LumPWMRegMTb[12] =
{
   //luminance in cd/m^2
   80, 100, 120, 160, 200, 240,
#if 1//DEFAULT_PANEL == LG_WUXGA_LM260WU2_SLA1
   //register value
   0x42, 0x54, 0x60, 0x6A, 0x84, 0x97
#else
   //register value
   0xf8, 0xcc, 0xaa, 0x79, 0x58, 0x42
#endif
};

//*****************************************************************************
// FUNCTION     : void SetBrightness
// USAGE        :
// INPUT        :
// OUTPUT       :
// GLOBALS      :
// USED_REGS    :
//*****************************************************************************
//void  SetBrightness(const GamutDataStruct* gamutData)
#define PWM_MAX_BRIGHTNESS      0xFF
#define PWM_MIN_BRIGHTNESS      0x20
void  SetBrightness(WORD whiteLum)
{
   WORD value;
   BYTE i;

   //whiteLum = gamutData->whiteLum;
   i = whiteLum/20;
   if (i <= 3) //whitelum < 80
   {
      //value = 0xf8 + ((((80-whiteLum)*14)/80+1)>>1);
      value = PWM_MIN_BRIGHTNESS + ((((80 - whiteLum)*14)/80+1)>>1);
   }
   else if (i <= 11) //whiteLum between 80 and 240
   {
      if (i <= 5)
         i = i-4;
      else
         i = (i-6)/2 + 2;

      value = LumPWMRegMTb[6+i] + ((SWORD)(whiteLum - LumPWMRegMTb[i])*(SWORD)(LumPWMRegMTb[7+i]-LumPWMRegMTb[6+i]))/(SWORD)(LumPWMRegMTb[i+1]-LumPWMRegMTb[i]);
   }
   else //whiteLum>=240
   {
      i = 5;
      value = LumPWMRegMTb[6+i] + ((SWORD)(whiteLum - LumPWMRegMTb[i])*(SWORD)(-LumPWMRegMTb[6+i]))/(SWORD)(PWM_MAX_BRIGHTNESS-LumPWMRegMTb[i]);
   }

   msg("SetBrightness--value: 0x%x", (WORD)value);

#if 0
   if (gamutData->whiteLum < defaultDisplayWGS.whiteLum[1] ) //minimum white lum
      whiteLum = defaultDisplayWGS.whiteLum[1];
   if (gamutData->whiteLum > defaultDisplayWGS.whiteLum[2] ) //maximum white lum
      whiteLum = defaultDisplayWGS.whiteLum[2];

   value =  0x100 - whiteLum;       //it is not accurate, just for illustration purpose
#endif

   brightness_reg_value = value;

   // Need to check if still using PWM3 as backlight control
   gm_WriteRegWord(PWM3_CTRL, (value << 8) | 0x00FF);

   //gm_ForceUpdate(CH_A, OUTPUT_CLOCK);
   ColorForceUpdate();
}

//*****************************************************************************
// FUNCTION     : BYTE  far DoGamutMapping(BYTE* gamutDataBuf, BYTE mode)
// USAGE        : To do gamut remapping
// INPUT        : BYTE* gamutDataBuf, BYTE mode
//                mode: 1-set default source gamut; 0-use current source gamut in memory
// OUTPUT       : return: 0-successful, 1-failed
// GLOBALS      :
// USED_REGS    :
//*****************************************************************************
BYTE far DoGamutMapping(BYTE* gamutDataBuf, BYTE mode)
{
   BYTE ret;
   GamutDataStruct* buf;
   GamutDataStruct panelGamutInfo;
   SDWORD	MappingMatrix[9];

   //set default gamut data
   //this code should be removed when gamut data is set from i2c
   if (mode)
   {
      SetDefaultSourceGamutData(gamutDataBuf);
   }
   else
   {
      if (WGS_colorSpaceMode != WGSM_CS_MODE_PROFILE)
         return 0xff;
   }


   //msg("mode: 0x%x", (WORD)(mode));
   //msg("gamut mapping flag: 0x%x", (WORD)(gamutDataBuf[31]));
   if (gamutDataBuf[gamutDataUpdateFlagOffset])
   {
      gamutDataBuf[gamutDataUpdateFlagOffset] = 0x00;
   }
   else
   {
      return 0xff;
   }

   ret = 0;
   buf = (GamutDataStruct*)gamutDataBuf;


   //print gamut data
#if 1
   msg("white Lum = 0x%x", buf->whiteLum);
   msg("white x = 0x%x", buf->white_x);
   msg("white y = 0x%x", buf->white_y);
   msg("red x = 0x%x", buf->red_x);
   msg("red y = 0x%x", buf->red_y);
   msg("green x = 0x%x", buf->green_x);
   msg("green y = 0x%x", buf->green_y);
   msg("blue x = 0x%x", buf->blue_x);
   msg("blue y = 0x%x", buf->blue_y);
   msg("gamma A0 = 0x%x", buf->gamma_A0);
   msg("gamma A1 = 0x%x", buf->gamma_A1);
   msg("gamma A2 = 0x%x", buf->gamma_A2);
   msg("gamma A3 = 0x%x", buf->gamma_A3);
   msg("Flag = 0x%x", buf->Flag);
#endif

   //compute and load preLUT
   LoadCustomGamma((const GamutDataStruct*)buf);

#if 1
   //compute and load 3x3 matrix
   GetPanelGamutInfo(&panelGamutInfo);
   ret = ComputeGamutMappingMatrix((const GamutDataStruct*)buf, (const GamutDataStruct*)&panelGamutInfo, (SDWORD*)&MappingMatrix);

   if (ret == 0) //load mapping matrix only when mapping matrix is right
   {
      LoadMappingMatrix((const SDWORD*)&MappingMatrix);
   }
#endif

   SetBrightness(((const GamutDataStruct*)buf)->whiteLum);
   EnableColorProcessing();

   return ret;
}

//*****************************************************************************
// FUNCTION     : void GetWGSCapabilities
// USAGE        :
// INPUT        :
// OUTPUT       :
// GLOBALS      :
// USED_REGS    :
//*****************************************************************************
void GetWGSCapabilities(WGSCapStruct* cs)
{
   _fmemcpy(cs, (const void*)&defaultDisplayWGS, sizeof(WGSCapStruct));
}

//*****************************************************************************
// FUNCTION     : BYTE SetColorSpaceMode
// USAGE        :
// INPUT        :
// OUTPUT       : return: 0x00-successful; 0xFF-failed
// GLOBALS      :
// USED_REGS    :
//*****************************************************************************
BYTE SetColorSpaceMode(BYTE mode)
{
   BYTE ret;

   switch (mode)
   {
      case WGSM_CS_MODE_STANDARD:
      case WGSM_CS_MODE_CUSTOM:
         ret = 0xFF;
         break;

      case WGSM_CS_MODE_PROFILE:
         ret = 0x00;
         WGS_colorSpaceMode = mode;
         break;
      default:
         ret = 0xFF;
   }

   return ret;
}

//*****************************************************************************
// FUNCTION     : BYTE UpdateSimplifiedColorProfile
// USAGE        :
// INPUT        :
// OUTPUT       : return: 0x00-successful; 0xFF-failed
// GLOBALS      :
// USED_REGS    :
//*****************************************************************************
BYTE UpdateSimplifiedColorProfile(GamutDataStruct* srcGamutData, BYTE size, BYTE type)
{
   BYTE ret = 0xFF;

   if (type == WGSM_UPDATE_SRC_PROFILE)
   {
      if (size ==  26 || size == 42)
      {
         memcpy(commonGamutDataBuf, srcGamutData, size);

         commonGamutDataBuf[gamutDataUpdateFlagOffset] = 0x01;

         ret = 0x00;
      }
   }

   return ret;
}
//*****************************************************************************
// FUNCTION     : void EnableColorProcessing
// USAGE        : set display system to pevious color space
// INPUT        :
// OUTPUT       :
// GLOBALS      :
// USED_REGS    :
//*****************************************************************************
void EnableColorProcessing(void)
{
   msg("EnableColorProcessing:", 0);

   gm_SetRegBitsWord(GAMMA_LUT1_CTRL, GAMMA_LUT1_CTL); // Enable Pre_LUT
   gm_SetRegBitsWord(GAMMA_LUT2_CTRL, GAMMA_LUT2_CTL); // Enable POST_LUT
   //gm_SetRegBitsWord(DISP_LUT_CONTROL, DISP_LUT_WINDOW_TYPE); // enable all pixels
   gm_SetRegBitsWord(DISP_LUT_CONTROL, BIT0|BIT1); // enable main pixels only

   //for gamut mapping matrix
   gm_SetRegBitsWord(GAMMA_LUT2_CTRL, BIT12); // Enable ODP_3x3 and following DISP_LUT_CONTROL window

   //link this to typical white luminance of display if not set
   if (brightness_reg_value == 0)
   {
      SetBrightness(defaultDisplayWGS.whiteLum[0]);
   }

   gm_WriteRegWord(PWM3_CTRL, ((WORD)brightness_reg_value << 8) | 0x00FF);

   //gm_ForceUpdate(CH_A, OUTPUT_CLOCK);
   ColorForceUpdate();
}

//*****************************************************************************
// FUNCTION     : void DisableColorProcessing
// USAGE        :
// INPUT        :
// OUTPUT       :
// GLOBALS      :
// USED_REGS    :
//*****************************************************************************
void DisableColorProcessing(void)
{
   msg("DisableColorProcessing:", 0);
   brightness_reg_value = gm_ReadRegByte(PWM3_CTRL) >> 8;	// Get PWM pulse

   gm_ClearRegBitsWord(GAMMA_LUT1_CTRL, GAMMA_LUT1_CTL); // Disable Pre_LUT
   gm_ClearRegBitsWord(GAMMA_LUT2_CTRL, GAMMA_LUT2_CTL); // Disable POST_LUT

   SetBrightness(defaultDisplayWGS.whiteLum[0]);

   postLUTLoadedFlag = 0;
}


#endif //USE_DYNAMIC_GAMUTMAPPING_SUPPORT

#endif // USE_COLOR_FIDELITY

