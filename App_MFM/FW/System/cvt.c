#define __CVT_C__
//*****************************************************************************
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
//=============================================================================
//
// MODULE:		cvt.c
//
// USAGE:		This module contains the 'determine mode' algorithms.
//
//*****************************************************************************

//*****************************************************************************
//	I N C L U D E	 F I L E S
//*****************************************************************************
#include "System\all.h"
#include <string.h>
#include <math.h>
#include "StdLibInc\gm_Register.h"

#ifdef CVT_MODES_SUPPORT

#define DEBUG_MSG				1
#define	DEBUG_CVT    			0
#define DEBUG_CVT_EXTENDED		0

#define USE_VBP_CHECK			0
#define USE_FUNCTION_MACROS		0

//#define DISPLAY_STANDARD_MODE_STRUCT

#ifdef DISPLAY_STANDARD_MODE_STRUCT
#define msgi(a, b)			gm_Print((const char far *)a, b)
#else
#define msgi(a, b)
#endif

#if DEBUG_CVT && DEBUG_MSG
#define	msg(a,b)				gm_Print((const char far *)a, b)
#else
#define msg(a,b)
#endif

#if DEBUG_CVT_EXTENDED && DEBUG_MSG
#define	msgx(a, b)				gm_Print((const char far *)a, b)
#else
#define msgx(a,b)
#endif



//**************************************************************
//	         G L O B A L   V A R I A B L E S
//**************************************************************

//**************************************************************
//	         L O C A L   V A R I A B L E S
//**************************************************************
typedef struct cvt_aspect_ratio_struct
{
   BYTE	Width;
   BYTE	Height;
} cvt_aspect_ratio_type;

cvt_aspect_ratio_type ROM cvt_aspect[] =
{
   {  4,  3 },		// vsync == 4,  4:3
   { 16,  9 },		// vsync == 5, 16:9
   { 16, 10 },		// vsync == 6, 16:10
   {  5,  4 },		// vsync == 7,  5:4 (VTotal > 1000)
   { 15,  9 }		// vsync == 7, 15:9 (VTotal < 1000)
};


//**************************************************************
//	         L O C A L   D E F I N I T I O N S
//**************************************************************

#define MIN_VERT_BP					7
#define VERT_FP						3

#define RB_MIN_V_BLANK_PERIOD 		460	// 460 us.
#define NORM_MIN_V_BLANK_PERIOD 	550	// 500 us.
#define RB_MIN_V_BLANK_LINES		13

#define RB_H_BLANK					160

#define C_PRIME						30
#define M_PRIME						300

#define PIXEL_CLK_MAX_CHANNEL_A		165000000L
#define PIXEL_CLK_MAX_CHANNEL_B		165000000L

#define MAKE_DIV8(a)				((a) & 0xfff8)

#if USE_FUNCTION_MACROS
#define isReducedBlanking(a)       \
				((((a) & gmd_INPUT_NEG_VSYNC) == gmd_INPUT_NEG_VSYNC) ? TRUE : FALSE)
#endif

//**************************************************************
//	         LOCAL  FUNCTION   PROTOTYPES
//**************************************************************

static WORD getVBI(gmt_InputMeasurementStruct *Stp_Input);

#if USE_VBP_CHECK
static WORD cvt_getVBP(gmt_InputMeasurementStruct *Stp_Input);
#endif

#if USE_FUNCTION_MACROS == 0
static BOOL isReducedBlanking(gmt_INPUT_FLAGS B_TimingFlags);
#endif


//******************************************************************************
//
// FUNCTION       : BOOL cvt_IsCVT(gmt_InputMeasurementStruct *Stp_Input,gmt_PHY_CH B_Channel)
//
// USAGE          : Determines if mode is a CVT mode.
//
// INPUT          : Stp_Input -
//                     A structure containing the modes measured
//                     values (HFreq, VFreq, VTotal, etc.)
//                  B_Channel
//
// OUTPUT         : FALSE if mode is not a CVT mode; TRUE otherwise
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
BOOL cvt_IsCVT(gmt_InputMeasurementStruct *Stp_Input,gmt_PHY_CH B_Channel)
{
   if (gmvb_CurrentSyncState[B_Channel] != DSS_STATE)
   {
      msgx("cvt: Not DSS_SYNC", 0);
      return FALSE;
   }
   {   // check sync polarity, if Hsync and Vsync the same polarity, it's not a CVT mode.
      gmt_INPUT_FLAGS B_TimingFlags = Stp_Input->W_DetectFlags & (gmd_INPUT_NEG_VSYNC | gmd_INPUT_NEG_HSYNC);
      if (B_TimingFlags == 0 || B_TimingFlags == (gmd_INPUT_NEG_VSYNC | gmd_INPUT_NEG_HSYNC))
      {
         msgx("cvt: both POS or NEG sync - %d", B_TimingFlags);
         return FALSE;
      }
      msgx("cvt: Sync Polarities (VH) is %d", B_TimingFlags);
   }
   {   // CVT is defined to have a Vsync pulse width of 4 through 7.
      // Anything outside this range is not currently defined to be CVT.
      WORD VsyncWidth = Stp_Input->W_VsyncPulse;
      if (VsyncWidth < 4 || VsyncWidth > 7)
      {
         msgx("cvt: VsyncWidth <= 4 or VsyncWidth >=7; width = %d", VsyncWidth);
         return FALSE;
      }
   }
#if USE_VBP_CHECK
   // if Vsync_BackPorch > 6, then CVT mode.
   if (cvt_getVBP(Stp_Input) <= 6)
   {
      msgx("cvt: VsyncBP <= 6 %d", cvt_getVBP(Stp_Input));
      return FALSE;
   }
#endif

   return TRUE;
}


//******************************************************************************
//
// FUNCTION       : void cvt_CalcTiming(gmt_InputMeasurementStruct 	*	Stp_Input,
//                  					OutputStruct 				*	Stp_CVT)
//
// USAGE          : Calculates CVT parameters
//                  (HTotal, HActive, HBlanking, VActive, VBlanking, etc.)
//
// INPUT          : Stp_Input -
//                     A structure containing the modes measured
//                     values (HFreq, VFreq, VTotal, etc.)
//                  Stp_CVT -
//                     A mode table entry structure to contain the
//                     calculated mode values for the mode measurements
//                     in Stp_Input structure.
//
// OUTPUT         : void
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
void cvt_CalcTiming(gmt_InputMeasurementStruct 	*	Stp_Input,
                    gmt_IbdStruct 				*	Stp_CVT)
{
   BYTE		i;
   WORD		HTotal;
   WORD		HActive;
   WORD		HBlanking;
   WORD		idealDC;
   WORD		roundup;
   WORD		VActive;
   WORD		VsyncWidth;


   // Calculate Vertical Active Region knowing the Total Number of Vertical Lines and the Number
   // of Vertical Lines in the Vertical Blanking Interval.
   VActive = (Stp_Input->W_VTotal - getVBI(Stp_Input)) & 0xfffe;

   VsyncWidth = Stp_Input->W_VsyncPulse;

   msg("cvt: VActive %d", VActive);
   msg("cvt: VsyncWidth %d", VsyncWidth);

   i = VsyncWidth - 4;

   if (VsyncWidth == 7 && Stp_Input->W_VTotal < 1000)
   {
      msg("vsync == 7 and Vtotal < 1000", 0);
      i++;
   }

   msg("cvt: Width %d", cvt_aspect[i].Width);
   msg("cvt: Height %d", cvt_aspect[i].Height);

   // Calculate the Horizontal Active Region knowing
   // the Vertical Active Region and the Aspect Ratio.
   // Horizontal Active Region / Vertical Active Region = Aspect Ration

   roundup = cvt_aspect[i].Height / 2;

   HActive = (WORD)((((DWORD)VActive * cvt_aspect[i].Width) + roundup) /
                    cvt_aspect[i].Height);

   HActive = MAKE_DIV8(HActive); // divisable by 8.

   msg("cvt: HActive %d",HActive);

   // Calculate the Horizontal Total Pixel Count
   // Horizontal Total Pixel Count is calculate
   // by adding the Horizontal Blanking Interval
   // to the Horizontal Active Region.

   if (isReducedBlanking(Stp_Input->W_DetectFlags) == TRUE)
   {
      // Reduced Horizontal Blanking Interval is
      // a constant amount.

      HBlanking = RB_H_BLANK;
      HTotal    = RB_H_BLANK + HActive;
   }
   else
   {
      // Standard CVT timing.

      // Calculate Horizontal Blanking Interval
      // based upon the Ideal Duty Cycle.
      // Duty Cycle = Horizontal Blanking
      // Interval Pixel Count / Horizontal Total
      // Pixel Count

      roundup = Stp_Input->W_HFreq / 2;
      idealDC = C_PRIME * 10 - ((((M_PRIME * 10) + roundup)*10) / Stp_Input->W_HFreq);

      msg("cvt: idealDC %d", idealDC);

      // Minimum value of Ideal Duty Cycle is 20%.

      if (idealDC < 200)
      {
         idealDC = 200;
      }

      // Calculate Horizontal Blanking Interval
      // Pixel Count knowing the Horizontal
      // Active Region Pixel Count and the
      // Horizontal Blanking Interval Pixel
      // Count Ideal Duty Cycle.

      // IdealDutyCycle = HBI / HTotal
      // IdealDutyCycle = HBI / (HBI + HActive)
      // HBI = HActive * IdealDutyCycle / (1 - IdealDutyCycle)

      roundup = (1000 - idealDC) / 2;
      HBlanking = MAKE_DIV8((WORD)(((DWORD)HActive * idealDC) / (1000 - idealDC)));
      msg("HBlanking %d", HBlanking);

      HTotal = MAKE_DIV8(HBlanking + HActive);
      msg("cvt: HTotal %d", HTotal);
   }

   // Copy values to the structure for
   // the system to access.

   Stp_CVT->VTotal = Stp_Input->W_VTotal;
   Stp_CVT->HTotal = HTotal;
   Stp_CVT->HActive = HActive;
   Stp_CVT->VActive = VActive;
   Stp_CVT->HStart = HBlanking;
   Stp_CVT->VStart = Stp_Input->W_VTotal - VActive;

   /*
   	{
   		// Set video mode and interlace flag if
   		// appropriate.  Video mode flag controls
   		// color space conversion.  If VideoMode
   		// is set, YCbCr/YPbPr is converted to
   		// RGB before display.

   		typedef struct CVT_VIDEO_TABLE_ENTRY
   		{
   			WORD	W_StdVFreq;
   			BYTE	B_DeltaVFreq;
   			WORD	W_StdVTotal;
   			BYTE	B_DeltaVTotal;
   			WORD	B_ModeType;
   		} CVT_VIDEO_TABLE_ENTRY;

   		// What other video modes should be included?
   		// If these are the only ones, the code is over
   		// kill.

   		static CVT_VIDEO_TABLE_ENTRY CVT_VideoTable[] =
   		{
   //			{ 600, 50, 750, 10, gmd_MODE_SDVIDEO | gmd_MODE_INTERLACED }, // 1280 x  720 x 60p
   			{ 600, 50, 562, 20, gmd_MODE_SDVIDEO | gmd_MODE_INTERLACED }, // 1920 x 1080 x 60i
   			{   0,  0,   0,  0, 0 }
   		};

   		CVT_VIDEO_TABLE_ENTRY *		Stp_VideoTable;

   #define VideoTable(a)	Stp_VideoTable->##a
   #define CVT(a)			Stp_CVT->##a
   #define VideoAbsDiff(a)	abs((CVT(W_Std##a)) - (VideoTable(W_Std##a)))

   		for (Stp_VideoTable = CVT_VideoTable;
   			 Stp_VideoTable->W_StdVFreq != 0;
   			 Stp_VideoTable++)
   		{
   			if (VideoAbsDiff(VFreq)  <= VideoTable(B_DeltaVFreq) &&
   				VideoAbsDiff(VTotal) <=	VideoTable(B_DeltaVTotal))
   			{
   				CVT(B_ModeType) |= VideoTable(B_ModeType);
   				return;
   			}
   		}

   		// Otherwise, mode is normal and progressive.

   		CVT(B_ModeType) |= (Normal | Progressive);
   	}
   	return;*/
}


#if USE_VBP_CHECK
//******************************************************************************
//
// FUNCTION       : static WORD cvt_getVBP(gmt_InputMeasurementStruct *Stp_Input)
//
// USAGE          : Calculates Vsync back porch
//
// INPUT          : Stp_Input -
//                     A structure containing the modes measured
//                     values (HFreq, VFreq, VTotal, etc.)
//
// OUTPUT         : Vsync back porch
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
static WORD cvt_getVBP(gmt_InputMeasurementStruct *Stp_Input)
{
   WORD	verticalBlank;
   WORD	VsyncBP;
   WORD	VsyncWidth;


   // Vertical Back Porch is Vertical Blanking minus
   // Vertical Front Porch minus VSync Pulse Width in
   // lines.

   VsyncWidth    = Stp_Input->W_VsyncPulse;
   verticalBlank = getVBI(Stp_Input);

   VsyncBP = verticalBlank - VERT_FP - VsyncWidth;

   msg("cvt_getVBP: verticalBlank %d", verticalBlank);
   msg("cvt_getVBP: VERT_FP %d", (WORD)VERT_FP);
   msg("cvt_getVBP: VsyncWidth %d", VsyncWidth);

   return VsyncBP;
}
#endif


//******************************************************************************
//
// FUNCTION       : static WORD getVBI(gmt_InputMeasurementStruct *Stp_Input)
//
// USAGE          : Calculates Vertical blanking interval
//
// INPUT          : Stp_Input
//
// OUTPUT         : Vertical blanking interval
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
static WORD getVBI(gmt_InputMeasurementStruct *Stp_Input)
{
   WORD	verticalBlank;
   // VBI   = Vertical Blanking Interval (lines)
   // MVBI  = Minimum Vertical Blanking Interval (seconds)
   // VFP   = Vertical Blanking Interval Front Porch (lines)
   // HFreq = Horizontal Frequency (lines/second)
   // VBI = MVBI * HFREQ + VFP + 1

   // For Reduced Blanking Inverval,
   // Minimum Vertical Blanking Interval = 460us
   // and
   // Vertical Front Porch = 0 lines;
   // otherwise,
   // Minimum Vertical Blanking Interval = 550us
   // and
   // Vertical Front Porch = 3 lines.
   // VBI = MVBI (1/10^6 sec) * HFreq (10^2 lines/sec) + 1
   // VBI = MVBI * HFreq * 1/10^4 + 1

   if (isReducedBlanking(Stp_Input->W_DetectFlags) == TRUE)
   {
      verticalBlank = (WORD)
                      (((DWORD)RB_MIN_V_BLANK_PERIOD * Stp_Input->W_HFreq) / 10000) + 1;

      if (verticalBlank < RB_MIN_V_BLANK_LINES)
      {
         verticalBlank = RB_MIN_V_BLANK_LINES;
      }
      msg("cvt_rb: verticalBlank %d", verticalBlank);
   }
   else
   {
      // Standard CVT timing.

      verticalBlank = (WORD)(((DWORD)NORM_MIN_V_BLANK_PERIOD *
                              Stp_Input->W_HFreq) / 10000) + 1 + VERT_FP;

      msgx("cvt_std: W_InputHFreq %d", Stp_Input->W_HFreq);
      msg("cvt_std: verticalBlank %d", verticalBlank);
   }

   return verticalBlank;
}


#if USE_FUNCTION_MACROS == 0
//******************************************************************************
//
// FUNCTION       : static BOOL isReducedBlanking(gmt_INPUT_FLAGS B_TimingFlags)
//
// USAGE          : Determines if the mode has standard blanking or reduced blanking
//
// INPUT          : B_TimingFlags - flag byte from the gmt_InputFormatStruct
//                     structure containing the polarity of the HSync
//                     and VSync signals.
//
// OUTPUT         : TRUE - if mode has reduced blanking;
//                  FALSE - if mode has normal blanking
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
static BOOL isReducedBlanking(gmt_INPUT_FLAGS B_TimingFlags)
{
   // If H+ and V- the mode is reduced blanking
   // Since this is a CVT mode we already know
   // HSync and VSync have opposite polarities
   // (H+ and V- or H- and V+).

   return (B_TimingFlags & gmd_INPUT_NEG_VSYNC) == gmd_INPUT_NEG_VSYNC ? TRUE : FALSE;
}
#endif


//******************************************************************************
//
// FUNCTION       : BOOL IsCVTPixelClockRange(gmt_InputMeasurementStruct *	Stp_CVT,
//                  						  OutputStruct  			 *  Out_CVT,
//                  						  gmt_PHY_CH					B_Channel)
//
// USAGE          : Checks if the Pixel Clock Frequency is within range
//
// INPUT          : Stp_CVT -
//                     Structure containing the horizontal line
//                     frequency and total horizontal pixel count
//                     used to calculate the pixel clock frequency.
//                  Out_CVT
//                  B_Channel -
//                     The current input channel to perform check on.
//
// OUTPUT         : TRUE if mode has reduced blanking;
//                  FALSE if mode has normal blanking
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
BOOL IsCVTPixelClockRange(gmt_InputMeasurementStruct *	Stp_CVT,
                          gmt_IbdStruct  			 *  Out_CVT,
                          gmt_PHY_CH					B_Channel)
{
   DWORD			PixelClkFreq;

   if (GetChannelConnType (B_Channel) == VGA)
   {
      // Calculate the Pixel Clock Frequency (Pixels/sec) as
      // Horizontal Lines Per Second (lines/sec) * Total
      // Number of Pixels Per Line (Pixels/Line).  LinesPerSec
      // is in 100 lines/sec units.
      // PixelClockFrequency = TotalPixelsPerLine * LinesPerSec * 100

      PixelClkFreq  = (DWORD)Stp_CVT->W_HFreq * 100L;
      PixelClkFreq *= (DWORD)Out_CVT->HTotal;
   }
   else if (GetChannelConnType (B_Channel) == DVI)
   {
      gm_Delay1ms(50);

      // Use the Video Clock Measurement Unit to measure
      // the DVI clock.  The Video Clock Measurement Unit
      // counts the number of clocks detected within 'x'
      // number of TClk's.  In this case 'x' is 256 TClk's
      // PixelClockFrequency = DVICount * TClkFreq (1K TClk/sec) / 256 TClk's
      // PixelClockFrequency = DVICount * TClkFreq * 1000 / 256

      if (B_Channel == CH_A)
         PixelClkFreq = (DWORD)gm_ReadRegWord(M_CLKMEAS_RESULT);
      else
         PixelClkFreq = (DWORD)gm_ReadRegWord(P_CLKMEAS_RESULT);
      PixelClkFreq = ((PixelClkFreq * (DWORD)gmc_TCLK_FREQ) / 256L) * 1000L;
   }
   else
   {
      // Ignore video inputs -- they should be within range

      PixelClkFreq = 0L;
   }

   if (( (B_Channel == CH_A) &&  (PixelClkFreq > PIXEL_CLK_MAX_CHANNEL_A)) ||
         ( (B_Channel == CH_B) &&  (PixelClkFreq > PIXEL_CLK_MAX_CHANNEL_B)))
      return FALSE;

   return TRUE;
}
#endif
