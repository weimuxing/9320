/*
	$Workfile:   gm_UpdateSharpness.c  $
	$Revision: 1.13 $
	$Date: 2013/06/10 10:03:59 $
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
// MODULE:     	gm_UpdateSharpness.c
//
// USAGE:		Update sharpness values based on scaling and user selection
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "System\all.h"
#include "StdLibInc\gm_Register.h"
#include "StdLibInc\gm_UpdateSharpness.h"

//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************

#if	(FEATURE_SHARPNESS == REV6)
#include "StdLibReplace\SharpnessTable_8668.c"
#endif

//#define _DEBUG_MSG
#ifdef _DEBUG_MSG
#define msg(x)			gm_Print(x,0)
#define msg_i(x,y)		gm_Print(x,y)
#else
#define msg(x)
#define msg_i(x,y)
#endif

#if (FEATURE_SHARPNESS == REV6)
BOOL gmp_EnhancerEnable = FALSE;//disabled is the default
#endif

//******************************************************************************
//  E X T E R N A L     V A R I A B L E S
//******************************************************************************

extern BYTE ROM * ROM Ba_VfTablePtr[];

//******************************************************************************
//  S T A T I C    F U N C T I O N S
//******************************************************************************

#if (FEATURE_SHARPNESS == REV6)
static BYTE gm_GetHorShrnkIndex_Video( DWORD HScale );
static BYTE gm_GetHorShrnkIndex_RGB( DWORD HScale );
static BYTE gm_GetVertShrnkIndex_Video( DWORD VScale );
static BYTE gm_GetVertShrnkIndex_RGB( DWORD VScale );
static void SetSharpness_8668(gmt_PHY_CH B_Channel, WORD W_Sharpness);
#endif

//***************************************************************************
//  C O D E
//***************************************************************************

//******************************************************************************
//
// FUNCTION       : void gm_SetScalerSharpness(gmt_PHY_CH B_Channel, WORD W_Sharpness)
//
// USAGE          : Updates the sharpness by using the video vertical
//                  coefficient tables and HEM (for the horizontal).
//
// INPUT          : B_Channel -Channel to apply sharpness to.
//                  W_Sharpness - Given by the application (userpref)
//
// OUTPUT         : None
//
// GLOBALS        : Ba_VfTablePtr			(RO)
//
// USED_REGS      : HOST_CONTROL			(W)
//                  MVF_CTRL				(W)
//                  ENH_OVR_GAIN			(W)
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************

void gm_SetScalerSharpness(gmt_PHY_CH B_Channel, WORD W_Sharpness)
{
#if (FEATURE_SHARPNESS == REV6)
   //********************************************************************************************************
   //
   // FLI8668 specific Sharpness control
   //
   //********************************************************************************************************
   SetSharpness_8668( B_Channel, W_Sharpness);

   gm_SyncUpdate(B_Channel, INPUT_OUTPUT_CLOCK);
#else
   UNUSED_VAR(B_Channel);
   UNUSED_VAR(W_Sharpness);
#endif
}


#if (FEATURE_SHARPNESS == REV6)
//
// Cortez Advanced set of sharpness functions
//

//******************************************************************************
//
// FUNCTION       : static BYTE gm_GetHorShrnkIndex( DWORD HScale )
//
// USAGE          : Return table row index based on HScale value
//
// INPUT          : DWORD HScale - HScale value returned by gm_GetHScaleValue()
//
// OUTPUT         : BYTE HorShrnkIndex - row index in Ba_HShrpGainY Sharpness Gain table
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  : For FLI8668 ONLY
//
// POST_CONDITION :
//
//******************************************************************************
static BYTE gm_GetHorShrnkIndex_Video( DWORD HScale )
{
   if (HScale <= 0x00060000L) 		//0x00060000->0.375 - 1080p panel
      return 0;
   else if (HScale <= 0x00100000L) //0x00100000->1.0
      return 1;
   else if (HScale <= 0x00107896L) //0x00107896->1.02944
      return 2;
   else if (HScale <= 0x00117896L) //0x00117896->1.09194
      return 3;
   else if (HScale <= 0x0013999AL) //00x0013999A->1.1625
      return 4;
   else if (HScale <= 0x0014E2B7L) //0x0014E2B7->1.24285
      return 5;
   else if (HScale <= 0x00155CC6L) //0x00155CC6->1.33515
      return 6;
   else if (HScale <= 0x001713A9L) //0x001713A9->1.4423
      return 7;
   else if (HScale <= 0x00191724L) //0x00191724->1.56815
      return 8;
   else if (HScale <= 0x001B7D8BL) //0x001B7D8B->1.71815
      return 9;
   else if (HScale <= 0x001E6666L) //0x001E6666->1.9
      return 10;
   else if (HScale <= 0x00220000L) //0x00220000->2.125
      return 11;
   else if (HScale <= 0x0027926FL) //0x0027926F->2.41075
      return 12;
   else if (HScale <= 0x002D926FL) //0x002D926F->2.78575
      return 13;
   else if (HScale <= 0x0035CC63L) //0x0035CC63->3.29989
      return 14;
   else if (HScale <= 0x0041CB19L) //0x0041CB19->4.04958
      return 15;
   else
      return 16;
}


//******************************************************************************
//
// FUNCTION       : static BYTE gm_GetHorShrnkIndex_RGB( DWORD HScale )
//
// USAGE          : Return table row index based on HScale value
//
// INPUT          : DWORD HScale - HScale value returned by gm_GetHScaleValue()
//
// OUTPUT         : BYTE HorShrnkIndex - row index in Ba_HShrpGainY Sharpness Gain table
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  : For FLI8668 ONLY
//
// POST_CONDITION :
//
//******************************************************************************
static BYTE gm_GetHorShrnkIndex_RGB( DWORD HScale )
{
   #ifdef ST_4K2K_93xx_BOARD
   if((GetChipID_4K2K() != CHIPID_4K2K_FE) && (UserPrefPBPMode == PBP_OFF))
      return 0; // For adjust sharpness center line issue   
   #endif

   if (HScale <= 0x080000L)			// 0x080000 -> 0.5
      return 0;
   else if (HScale <= 0x100000L)	// 0x100000 -> 1.0
      return 1;
   else if (HScale <= 0x10CCCCL)	// 0x10CCCC -> 1.05
      return 2;
   else if (HScale <= 0x126666L)	// 0x126666 -> 1.15
      return 3;
   else if (HScale <= 0x140000L)	// 0x140000 -> 1.25
      return 4;
   else if (HScale <= 0x159999L)	// 0x159999 -> 1.35
      return 5;
   else if (HScale <= 0x173333L)	// 0x173333 -> 1.45
      return 6;
   else if (HScale <= 0x18CCCCL)	// 0x18CCCC -> 1.55
      return 7;
   else if (HScale <= 0x1A6666L)	// 0x1A6666 -> 1.65
      return 8;
   else if (HScale <= 0x1C0000L)	// 0x1C0000 -> 1.75
      return 9;
   else if (HScale <= 0x1D9999L)	// 0x1D9999 -> 1.85
      return 10;
   else if (HScale <= 0x1F3333L)	// 0x1F3333 -> 1.95
      return 11;
   else if (HScale <= 0x20CCCCL)	// 0x20CCCC -> 2.05
      return 12;
   else if (HScale <= 0x226666L)	// 0x226666 -> 2.15
      return 13;
   else if (HScale <= 0x240000L)	// 0x240000 -> 2.25
      return 14;
   else if (HScale <= 0x259999L)	// 0x259999 -> 2.35
      return 15;
   else if (HScale <= 0x273333L)	// 0x273333 -> 2.45
      return 16;
   else if (HScale <= 0x28CCCCL)	// 0x28CCCC -> 2.55
      return 17;
   else if (HScale <= 0x2A6666L)	// 0x2A6666 -> 2.65
      return 18;
   else if (HScale <= 0x2C0000L)	// 0x2C0000 -> 2.75
      return 19;
   else
      return 20;
}


//******************************************************************************
//
// FUNCTION       : static BYTE gm_GetVertShrnkIndex_Video( DWORD VScale )
//
// USAGE          : Return table row index based on VScale value
//
// INPUT          : DWORD VScale - VScale value returned by gm_GetVScaleValue()
//
// OUTPUT         : BYTE VertShrnkIndex - row index in Ba_VShrpGainY Sharpness Gain table
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  : For FLI8668 ONLY
//
// POST_CONDITION :
//
//******************************************************************************

static BYTE gm_GetVertShrnkIndex_Video( DWORD VScale )
{
   //-----------------------------------------------------------------------------------
   // Out vertical size greater than one fields:
   //-----------------------------------------------------------------------------------
   if (VScale <= 0x20000L) //0x20000  -> 0.25 - Zoom
      return 0;
   else if (VScale <= 0x40000L) //0x40000  -> 1.0 - Zoom
      return 1;

   //-----------------------------------------------------------------------------------
   // Out vertical size less than one field:
   //-----------------------------------------------------------------------------------
   else if (VScale <= 0x41E9AL) //0x41E9A  -> 1.035676023
      return 2;
   else if (VScale <= 0x46F86L) //0x46F86 -> 1.112643482
      return 3;
   else if (VScale <= 0x4CDE6L) //0x4CDE6 -> 1.201967459
      return 4;
   else if (VScale <= 0x531B8L) //0x531B8 -> 1.306756204
      return 5;
   else if (VScale <= 0x5B9AAL) //0x5B9AA -> 1.431831211
      return 6;
   else if (VScale <= 0x64F52L) //0x64F52 -> 1.583408341
      return 7;
   else if (VScale <= 0x71540L) //0x71540 -> 1.77071616
      return 8;
   else if (VScale <= 0x80000L) //0x80000 -> 2.0
      return 9;
   else if (VScale <= 0x948F4L) //0x948F4 -> 2.321581655
      return 10;
   else if (VScale <= 0xAFE6EL) //0xAFE6E -> 2.74970006
      return 11;
   else
      return 12;
}


//******************************************************************************
//
// FUNCTION       : static BYTE gm_GetVertShrnkIndex_RGB( DWORD VScale )
//
// USAGE          : Return table row index based on VScale value
//
// INPUT          : DWORD VScale - VScale value returned by gm_GetVScaleValue()
//
// OUTPUT         : BYTE VertShrnkIndex - row index in Ba_VShrpGainY Sharpness Gain table
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  : For FLI8668 ONLY
//
// POST_CONDITION :
//
//******************************************************************************
static BYTE gm_GetVertShrnkIndex_RGB( DWORD VScale )
{
   if (VScale <= 0x30000L)				// 0x30000 -> 0.75 Zoom for 1080p panel
      return 0;
   if (VScale <= 0x40000L)				// 0x40000 -> 1.0 Zoom
      return 1;
   else if (VScale <= 0x43333L)		// 0x43333 -> 1.05
      return 2;
   else if (VScale <= 0x49999L)		// 0x49999 -> 1.15
      return 3;
   else if (VScale <= 0x50000L)		// 0x50000 -> 1.25
      return 4;
   else if (VScale <= 0x56666L)		// 0x56666 -> 1.35
      return 5;
   else if (VScale <= 0x5CCCCL)		// 0x5CCCC -> 1.45
      return 6;
   else if (VScale <= 0x63333L)		// 0x63333 -> 1.55
      return 7;
   else if (VScale <= 0x69999L)		// 0x69999 -> 1.65
      return 8;
   else if (VScale <= 0x70000L)		// 0x70000 -> 1.75
      return 9;
   else if (VScale <= 0x76666L)		// 0x76666 -> 1.85
      return 10;
   else if (VScale <= 0x7CCCCL)		// 0x7CCCC -> 1.95
      return 11;
   else if (VScale <= 0x83333L)		// 0x83333 -> 2.05
      return 12;
   else if (VScale <= 0x89999L)		// 0x89999 -> 2.15
      return 13;
   else if (VScale <= 0x90000L)		// 0x90000 -> 2.25
      return 14;
   else if (VScale <= 0x96666L)		// 0x96666 -> 2.35
      return 15;
   else if (VScale <= 0x9CCCCL)		// 0x9CCCC -> 2.45
      return 16;
   else
      return 17;
}


//******************************************************************************
//
// FUNCTION       : static void SetSharpness_8668(gmt_PHY_CH B_Channel, WORD W_Sharpness)
//
// USAGE          : Updates the sharpness by using the video vertical
//                  coefficient tables and HEM (for the horizontal).
//
// INPUT          : B_Channel -Channel to apply sharpness to.
//                  W_Sharpness - Given by the application (userpref)
//
// OUTPUT         : None
//
// GLOBALS        : Ba_VfTablePtr			(RO)
//
// USED_REGS      : HOST_CONTROL			(W)
//                  MVF_CTRL				(W)
//                  ENH_OVR_GAIN			(W)
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
static void SetSharpness_8668(gmt_PHY_CH B_Channel, WORD W_Sharpness)
{
   // set of static variables to keep the last scaling params
   static BYTE VertShrnkIndexVideoPrev=0xFF;
   static BYTE HorShrnkIndexVideoPrev=0xFF;
   static BYTE VertShrnkPipIndexVideoPrev=0xFF;
   static BYTE HorShrnkPipIndexVideoPrev=0xFF;
   static BYTE VertShrnkIndexRGBPrev=0xFF;
   static BYTE HorShrnkIndexRGBPrev=0xFF;
   static BYTE VertShrnkPipIndexRGBPrev=0xFF;
   static BYTE HorShrnkPipIndexRGBPrev=0xFF;
   static WORD W_SharpnessPrev=0xFF;
   static WORD W_SharpnessPipPrev=0xFF;
   static WORD W_ColorSpacePrev[NUM_CHANNELS]={0xFFFF, 0xFFFF};

   BYTE VertShrnkIndex=0;
   BYTE HorShrnkIndex=0;
   WORD W_ColorSpace[NUM_CHANNELS]={0, 0};

   // set of pointers to switch between tables to fill out LUT
   BYTE far * Bp_MVF_UV;
   BYTE far * Bp_MVF_Y;
   BYTE far * Bp_MVF_UVpk;
   BYTE far * Bp_MVF_Ypk;
   BYTE far * Bp_PVF_UV;
   BYTE far * Bp_PVF_Y;
   BYTE far * Bp_PVF_UVpk;
   BYTE far * Bp_PVF_Ypk;

   WORD far * Wp_MHF_UV;
   WORD far * Wp_MHF_Y;
   WORD far * Wp_MHF_UVpk;
   WORD far * Wp_MHF_Ypk;
   WORD far * Wp_PHF_UV;
   WORD far * Wp_PHF_Y;
   WORD far * Wp_PHF_UVpk;
   WORD far * Wp_PHF_Ypk;

   DWORD D_VertScaleValue;
   DWORD D_HorScaleValue;

   //WORD W_VTableNum, W_HFilterNum, W_EnhOvrGain;
   WORD W_VTableNum, W_HFilterNum;

   //
   // five tables are used to update five HW registers
   // BYTE Ba_HEMGain[7][25]		- ENH_OVR_GAIN
   // BYTE Ba_HShrpGainY[7][25]	- MHF_SHARPNESS_LUMA
   // BYTE Ba_HShrpGainUV[7][25]	- MHF_SHARPNESS_CHROMA
   // BYTE Ba_VShrpGainY[11][25]	- MVF_SHARPNESS_LUMA
   // BYTE Ba_VShrpGainUV[11][25]	- MVF_SHARPNESS_CHROMA
   //

   W_ColorSpace[CH_A] = gm_GetChannelInputAttr(CH_A, gmd_COLOR_SPACE);
   W_ColorSpace[CH_B] = gm_GetChannelInputAttr(CH_B, gmd_COLOR_SPACE);

   if (( W_Sharpness > MAX_SHARPNESS_SLIDER ) ||
         ( VertShrnkIndex > NUM_VERT_SCALE_RANGES ) ||
         ( HorShrnkIndex > NUM_HOR_SCALE_RANGES ))
   {
   #ifdef SINGLE_FIELD_DEINT_FIXED
      if (W_Sharpness < 29)
         VertShrnkIndex = W_Sharpness - 12;
      else
         VertShrnkIndex =0;
         
      Bp_MVF_UV	= (BYTE far *)SharpnessTables_MVF_YUV[VertShrnkIndex].Bp_UV;
      Bp_MVF_Y	= (BYTE far *)SharpnessTables_MVF_YUV[VertShrnkIndex].Bp_Y;
      Bp_MVF_UVpk	= (BYTE far *)SharpnessTables_MVF_YUV[VertShrnkIndex].Bp_UVpk;
      Bp_MVF_Ypk	= (BYTE far *)SharpnessTables_MVF_YUV[VertShrnkIndex].Bp_Ypk;
      //
      // Load Vertical MVF LUT (Main)
      // Load Horizontal MHF LUT (Main)
      //
      msg("---->> Sharpness LUT loading for Main YUV");
      gm_LoadSharpCoeffTable(gmd_TAB_MVF, (BYTE far *)Bp_MVF_UV, UV_MVF_START_ADDR, MVF_LUT_SIZE);
      gm_LoadSharpCoeffTable(gmd_TAB_MVF, (BYTE far *)Bp_MVF_Y, Y_MVF_START_ADDR, MVF_LUT_SIZE);
      gm_LoadSharpCoeffTable(gmd_TAB_MVF, (BYTE far *)Bp_MVF_UVpk, UVPK_MVF_START_ADDR, MVF_LUT_SIZE);
      gm_LoadSharpCoeffTable(gmd_TAB_MVF, (BYTE far *)Bp_MVF_Ypk, YPK_MVF_START_ADDR, MVF_LUT_SIZE);
   #endif
      return;	// Table Index is out of range
   }

   if (B_Channel == CH_A)
   {
      //******************************************************************************
      // MAIN channel
      //******************************************************************************

      //
      // get Main Hor and Vert scale value
      // find appropriate sharpness gain table indexes
      // check if indexes are not as previous
      // Update HW registers ENH_OVR_GAIN, MHF_SHARPNESS, MVF_SHARPNESS by Table values
      //
      D_VertScaleValue = gm_GetVScaleValue(CH_A);
      D_HorScaleValue = gm_GetHScaleValue(CH_A);

      if ( W_ColorSpace[CH_A] == gmd_RGB )
      {
       	 //121010 Edward for 2913 only RGB native mode remapping by DELL
		#if(DEFAULT_PANEL == LG_WHD_LM290WW1_SSA1)
         		if((B_Channel == gmvb_MainChannel)&& IsNativeModeInput(CH_A))
		#endif
           		W_Sharpness = RGBSharpnessRemapping();
		 //gm_Print("##RGB sharpness %d",W_Sharpness);
         //
         //       R G B    color space
         //
         VertShrnkIndex = gm_GetVertShrnkIndex_RGB( D_VertScaleValue );
         HorShrnkIndex = gm_GetHorShrnkIndex_RGB( D_HorScaleValue );
         msg (" ");
         msg_i ("R G B__ Main VertShrnkIndex = %d", VertShrnkIndex);
         msg_i ("R G B__ Main HorShrnkIndex = %d", HorShrnkIndex);

        #if defined(Qisda_93XX_BOARD) //KevinL 20120906 (P3_TestDrive bug#1) DVI 2560x1440 PBP issue
         if ((W_Sharpness != W_SharpnessPrev) ||
               (VertShrnkIndexRGBPrev != VertShrnkIndex) ||
               (HorShrnkIndexRGBPrev != HorShrnkIndex) ||
               (W_ColorSpace[CH_A] != W_ColorSpacePrev[CH_A]) ||
               (b_ForceSharpnessFilterReload[CH_A] == TRUE))        
        #else
         if ((W_Sharpness != W_SharpnessPrev) ||
               (VertShrnkIndexRGBPrev != VertShrnkIndex) ||
               (HorShrnkIndexRGBPrev != HorShrnkIndex) ||
               (W_ColorSpace[CH_A] != W_ColorSpacePrev[CH_A]))
        #endif               
         {
            W_HFilterNum = ( MHF_SHARPNESS_LUMA & Ba_HShrpGainY_RGB[HorShrnkIndex][W_Sharpness] );
            W_VTableNum = ( MVF_SHARPNESS_LUMA & Ba_VShrpGainY_RGB[VertShrnkIndex][W_Sharpness] );
            msg("Update Sharpness for RGB");

#if (FEATURE_SHARPNESS == REV6)
            gm_ClearAndSetBitsWord(MHF_CTRL, MHF_ENH_EN, MHF_ENH_BYPASS);
            gm_ClearAndSetBitsWord(MVF_CTRL, MVF_ENH_EN, MVF_ENH_BYPASS);
#endif
            gm_WriteRegWord( MHF_SHARPNESS, W_HFilterNum );
            gm_WriteRegWord( MVF_SHARPNESS, W_VTableNum	);

            msg_i ("SHARPNESS slider  Main RGB = %d", W_Sharpness);
            msg_i ("** Main Vertical sharpness gain from Table %d **", W_VTableNum);
            msg_i ("** Main Horizontal sharpness gain from Table %d **", W_HFilterNum);

            W_SharpnessPrev = W_Sharpness;
         }

         //
         // check if Scaling indexes are not as previous
         // Update pointers to appropriate MVF and MHF LUT
         //
      #if defined(Qisda_93XX_BOARD) //KevinL 20120906 (P3_TestDrive bug#1) DVI 2560x1440 PBP issue
         if ((W_Sharpness != W_SharpnessPrev) ||
               (VertShrnkIndexRGBPrev != VertShrnkIndex) ||
               (HorShrnkIndexRGBPrev != HorShrnkIndex) ||
               (W_ColorSpace[CH_A] != W_ColorSpacePrev[CH_A]) ||
               (b_ForceSharpnessFilterReload[CH_A] == TRUE))        
      #else 
         //120915 merge standard code always loaded table for DVI shrink mode data loose issue...
         #if 0 // 1
         if ((VertShrnkIndexRGBPrev != VertShrnkIndex) ||
               (HorShrnkIndexRGBPrev != HorShrnkIndex) ||
               (W_ColorSpace[CH_A] != W_ColorSpacePrev[CH_A]))
         #endif
      #endif         
         {
            //
            //    H O R I Z O N T A L   M A I N    filters for  R G B
            //
            Wp_MHF_UV	= (WORD far *)SharpnessTables_MHF_RGB[HorShrnkIndex].Wp_UV;
            Wp_MHF_Y	= (WORD far *)SharpnessTables_MHF_RGB[HorShrnkIndex].Wp_Y;
            Wp_MHF_UVpk	= (WORD far *)SharpnessTables_MHF_RGB[HorShrnkIndex].Wp_UVpk;
            Wp_MHF_Ypk	= (WORD far *)SharpnessTables_MHF_RGB[HorShrnkIndex].Wp_Ypk;
            //
            //    V E R T I C A L   M A I N    filters for  R G B
            //
            Bp_MVF_UV	= (BYTE far *)SharpnessTables_MVF_RGB[VertShrnkIndex].Bp_UV;
            Bp_MVF_Y	= (BYTE far *)SharpnessTables_MVF_RGB[VertShrnkIndex].Bp_Y;
            Bp_MVF_UVpk	= (BYTE far *)SharpnessTables_MVF_RGB[VertShrnkIndex].Bp_UVpk;
            Bp_MVF_Ypk	= (BYTE far *)SharpnessTables_MVF_RGB[VertShrnkIndex].Bp_Ypk;
            //
            // Load Vertical MVF LUT (Main)
            // Load Horizontal MHF LUT (Main)
            //
            msg("---->> Sharpness LUT loading for Main  RGB");
            gm_LoadSharpCoeffTable(gmd_TAB_MVF, (BYTE far *)Bp_MVF_UV, UV_MVF_START_ADDR, MVF_LUT_SIZE);
            gm_LoadSharpCoeffTable(gmd_TAB_MVF, (BYTE far *)Bp_MVF_Y, Y_MVF_START_ADDR, MVF_LUT_SIZE);
            gm_LoadSharpCoeffTable(gmd_TAB_MVF, (BYTE far *)Bp_MVF_UVpk, UVPK_MVF_START_ADDR, MVF_LUT_SIZE);
            gm_LoadSharpCoeffTable(gmd_TAB_MVF, (BYTE far *)Bp_MVF_Ypk, YPK_MVF_START_ADDR, MVF_LUT_SIZE);

            gm_LoadSharpCoeffTable(gmd_TAB_MHF, (BYTE far *)Wp_MHF_UV, UV_MHF_START_ADDR, MHF_LUT_SIZE);
            gm_LoadSharpCoeffTable(gmd_TAB_MHF, (BYTE far *)Wp_MHF_Y, Y_MHF_START_ADDR, MHF_LUT_SIZE);
            gm_LoadSharpCoeffTable(gmd_TAB_MHF, (BYTE far *)Wp_MHF_UVpk, UVPK_MHF_START_ADDR, MHF_LUT_SIZE);
            gm_LoadSharpCoeffTable(gmd_TAB_MHF, (BYTE far *)Wp_MHF_Ypk, YPK_MHF_START_ADDR, MHF_LUT_SIZE);

            VertShrnkIndexRGBPrev = VertShrnkIndex;
            HorShrnkIndexRGBPrev = HorShrnkIndex;
            W_ColorSpacePrev[CH_A] = W_ColorSpace[CH_A];
      #if defined(Qisda_93XX_BOARD) //KevinL 20120906 (P3_TestDrive bug#1) DVI 2560x1440 PBP issue            
            b_ForceSharpnessFilterReload[CH_A] = FALSE;            
      #endif      
         }
      }
      else
      {
		 //Neil120818* copy from U2410
         #if 1
         if((IsTMDSInput(UserPrefCurrentInputMain)||IsDPInput(gmvb_MainChannel)||IsVideoInput(UserPrefCurrentInputMain))&& (B_Channel == gmvb_MainChannel))
           	W_Sharpness = YUVSharpnessRemapping();
	#else	 
         if((IsTMDSInput(UserPrefCurrentInputMain)||IsDPInput(gmvb_MainChannel))&& (B_Channel == gmvb_MainChannel))
           	W_Sharpness = YUVSharpnessRemapping();
         #endif
		 
         //
         //       Y U V    color space
         //
         VertShrnkIndex = gm_GetVertShrnkIndex_Video( D_VertScaleValue );
         HorShrnkIndex = gm_GetHorShrnkIndex_Video( D_HorScaleValue );
         msg_i ("Y U V__ Main VertShrnkIndex = %d", VertShrnkIndex);
         msg_i ("Y U V__ Main HorShrnkIndex = %d", HorShrnkIndex);

         if ((W_Sharpness != W_SharpnessPrev) ||
               (VertShrnkIndexVideoPrev != VertShrnkIndex) ||
               (HorShrnkIndexVideoPrev != HorShrnkIndex) ||
               (W_ColorSpace[CH_A] != W_ColorSpacePrev[CH_A]))
         {
#if (FEATURE_SHARPNESS == REV6)
            WORD W_MhfEnhYFinalGain, W_MhfEnhUVFinalGain, W_MvfEnhYFinalGain, W_MvfEnhUVFinalGain;
#endif

		#ifdef DELL_U2410F//120922 Edward fix MVF_SHARPNESS_CHROMA to 0x0800 by Vincent request for HD 		
			if(gm_IsHDVideo(gmvb_MainChannel))
				W_HFilterNum = ( MHF_SHARPNESS_LUMA & Ba_HShrpGainY[HorShrnkIndex][W_Sharpness] ) +
							   ( MHF_SHARPNESS_CHROMA & (0x0800));
			else
		#endif
            W_HFilterNum = ( MHF_SHARPNESS_LUMA & Ba_HShrpGainY[HorShrnkIndex][W_Sharpness] ) +
                           ( MHF_SHARPNESS_CHROMA & ((Ba_HShrpGainUV[HorShrnkIndex][W_Sharpness]) << 8));
		
            W_VTableNum = ( MVF_SHARPNESS_LUMA & Ba_VShrpGainY[VertShrnkIndex][W_Sharpness] ) +
                          ( MVF_SHARPNESS_CHROMA & ((Ba_VShrpGainUV[VertShrnkIndex][W_Sharpness]) << 8));
            msg("Update Sharpness for YUV");

#if (FEATURE_SHARPNESS == REV6)
            gm_ClearAndSetBitsWord(MHF_CTRL, MHF_ENH_BYPASS, MHF_ENH_EN);
            gm_ClearAndSetBitsWord(MVF_CTRL, MVF_ENH_BYPASS, MVF_ENH_EN);

            W_MhfEnhYFinalGain = Ba_MHF_ENH_Y_FINALGAIN[HorShrnkIndex][W_Sharpness];
            W_MhfEnhUVFinalGain = Ba_MHF_ENH_UV_FINALGAIN[HorShrnkIndex][W_Sharpness];
            W_MvfEnhYFinalGain = Ba_MVF_ENH_Y_FINALGAIN[VertShrnkIndex][W_Sharpness];
            W_MvfEnhUVFinalGain = Ba_MVF_ENH_UV_FINALGAIN[VertShrnkIndex][W_Sharpness];
            gm_WriteRegWord(MHF_ENH_Y_FINALGAIN, W_MhfEnhYFinalGain);
            gm_WriteRegWord(MHF_ENH_UV_FINALGAIN, W_MhfEnhUVFinalGain);
            gm_WriteRegWord(MVF_ENH_Y_FINALGAIN, W_MvfEnhYFinalGain);
            gm_WriteRegWord(MVF_ENH_UV_FINALGAIN, W_MvfEnhUVFinalGain);

            msg_i ("MHF_ENH_Y_FINALGAIN = 0x%x", W_MhfEnhYFinalGain);
            msg_i ("MHF_ENH_UV_FINALGAIN = 0x%x", W_MhfEnhUVFinalGain);
            msg_i ("MVF_ENH_Y_FINALGAIN = 0x%x", W_MvfEnhYFinalGain);
            msg_i ("MVF_ENH_UV_FINALGAIN = 0x%x", W_MvfEnhUVFinalGain);
#endif

            gm_WriteRegWord( MHF_SHARPNESS, W_HFilterNum );
            gm_WriteRegWord( MVF_SHARPNESS, W_VTableNum	);

            msg_i ("SHARPNESS slider Main Video = %d", W_Sharpness);
            msg_i ("** Main Vertical sharpness gain from Table %d **", W_VTableNum);
            msg_i ("** Main Horizontal sharpness gain from Table %d **", W_HFilterNum);


            W_SharpnessPrev = W_Sharpness;
         }

         //
         // check if Scaling indexes are not as previous
         // Update pointers to appropriate MVF and MHF LUT
         //
         if ((VertShrnkIndexVideoPrev != VertShrnkIndex) ||
               (HorShrnkIndexVideoPrev != HorShrnkIndex) ||
               (W_ColorSpace[CH_A] != W_ColorSpacePrev[CH_A]))
         {
            //
            //    H O R I Z O N T A L   M A I N    filters  YUV
            //
            Wp_MHF_UV	= (WORD far *)SharpnessTables_MHF_YUV[HorShrnkIndex].Wp_UV;
            Wp_MHF_Y	= (WORD far *)SharpnessTables_MHF_YUV[HorShrnkIndex].Wp_Y;
            Wp_MHF_UVpk	= (WORD far *)SharpnessTables_MHF_YUV[HorShrnkIndex].Wp_UVpk;
            Wp_MHF_Ypk	= (WORD far *)SharpnessTables_MHF_YUV[HorShrnkIndex].Wp_Ypk;
            //
            //    V E R T I C A L   M A I N    filters  YUV
            //
            Bp_MVF_UV	= (BYTE far *)SharpnessTables_MVF_YUV[VertShrnkIndex].Bp_UV;
            Bp_MVF_Y	= (BYTE far *)SharpnessTables_MVF_YUV[VertShrnkIndex].Bp_Y;
            Bp_MVF_UVpk	= (BYTE far *)SharpnessTables_MVF_YUV[VertShrnkIndex].Bp_UVpk;
            Bp_MVF_Ypk	= (BYTE far *)SharpnessTables_MVF_YUV[VertShrnkIndex].Bp_Ypk;
            //
            // Load Vertical MVF LUT (Main)
            // Load Horizontal MHF LUT (Main)
            //
            msg("---->> Sharpness LUT loading for Main YUV");
            gm_LoadSharpCoeffTable(gmd_TAB_MVF, (BYTE far *)Bp_MVF_UV, UV_MVF_START_ADDR, MVF_LUT_SIZE);
            gm_LoadSharpCoeffTable(gmd_TAB_MVF, (BYTE far *)Bp_MVF_Y, Y_MVF_START_ADDR, MVF_LUT_SIZE);
            gm_LoadSharpCoeffTable(gmd_TAB_MVF, (BYTE far *)Bp_MVF_UVpk, UVPK_MVF_START_ADDR, MVF_LUT_SIZE);
            gm_LoadSharpCoeffTable(gmd_TAB_MVF, (BYTE far *)Bp_MVF_Ypk, YPK_MVF_START_ADDR, MVF_LUT_SIZE);

            gm_LoadSharpCoeffTable(gmd_TAB_MHF, (BYTE far *)Wp_MHF_UV, UV_MHF_START_ADDR, MHF_LUT_SIZE);
            gm_LoadSharpCoeffTable(gmd_TAB_MHF, (BYTE far *)Wp_MHF_Y, Y_MHF_START_ADDR, MHF_LUT_SIZE);
            gm_LoadSharpCoeffTable(gmd_TAB_MHF, (BYTE far *)Wp_MHF_UVpk, UVPK_MHF_START_ADDR, MHF_LUT_SIZE);
            gm_LoadSharpCoeffTable(gmd_TAB_MHF, (BYTE far *)Wp_MHF_Ypk, YPK_MHF_START_ADDR, MHF_LUT_SIZE);

            VertShrnkIndexVideoPrev = VertShrnkIndex;
            HorShrnkIndexVideoPrev = HorShrnkIndex;
            W_ColorSpacePrev[CH_A] = W_ColorSpace[CH_A];
         }
      }
   }

   else // CH_B

   {
      //******************************************************************************
      // P I P  channel
      //******************************************************************************

      //
      // four tables are used to update four HW registers
      // BYTE Ba_HShrpGainY_Pip[7][25]	- PHF_SHARPNESS_LUMA
      // BYTE Ba_HShrpGainUV_Pip[7][25]	- PHF_SHARPNESS_CHROMA
      // BYTE Ba_VShrpGainY_Pip[11][25]	- PVF_SHARPNESS_LUMA
      // BYTE Ba_VShrpGainUV_Pip[11][25]	- PVF_SHARPNESS_CHROMA
      //
      //
      // get Pip Hor and Vert scale value
      // find appropriate sharpness gain table indexes
      // check if indexes are not as previous
      // Update HW registers PHF_SHARPNESS, PVF_SHARPNESS by Table values (not ENH_OVR_GAIN)
      //

      D_VertScaleValue = gm_GetVScaleValue(CH_B);
      D_HorScaleValue = gm_GetHScaleValue(CH_B);

      if ( W_ColorSpace[CH_B] == gmd_RGB )
      {
         //
         //       R G B    color space
         //
         VertShrnkIndex = gm_GetVertShrnkIndex_RGB( D_VertScaleValue );
         HorShrnkIndex = gm_GetHorShrnkIndex_RGB( D_HorScaleValue );
         msg_i ("R G B__ Pip VertShrnkIndex = %d", VertShrnkIndex);
         msg_i ("R G B__ Pip HorShrnkIndex = %d", HorShrnkIndex);

#if defined(Qisda_93XX_BOARD) //KevinL 20120906 (P3_TestDrive bug#1) DVI 2560x1440 PBP issue
         if ((W_Sharpness != W_SharpnessPrev) ||
              (VertShrnkPipIndexRGBPrev != VertShrnkIndex) ||
              (HorShrnkPipIndexRGBPrev != HorShrnkIndex) ||
              (W_ColorSpace[CH_B] != W_ColorSpacePrev[CH_B]) ||
                  (b_ForceSharpnessFilterReload[CH_B] == TRUE))        
#else 

         if ((W_Sharpness != W_SharpnessPipPrev) ||
               (VertShrnkPipIndexRGBPrev != VertShrnkIndex) ||
               (HorShrnkPipIndexRGBPrev != HorShrnkIndex) ||
               (W_ColorSpace[CH_B] != W_ColorSpacePrev[CH_B]))
#endif               
         {
            W_HFilterNum = ( PHF_SHARPNESS_LUMA & Ba_HShrpGainY_Pip_RGB[HorShrnkIndex][W_Sharpness] );
            W_VTableNum = ( PVF_SHARPNESS_LUMA & Ba_VShrpGainY_Pip_RGB[VertShrnkIndex][W_Sharpness] );
            gm_WriteRegWord( PHF_SHARPNESS, W_HFilterNum );
            gm_WriteRegWord( PVF_SHARPNESS, W_VTableNum	);

            W_SharpnessPipPrev = W_Sharpness;

            msg("Update Pip Sharpness for RGB");
            msg_i ("SHARPNESS slider  Pip RGB = %d", W_Sharpness);
            msg_i ("** Pip Vertical sharpness gain from Table %d **", W_VTableNum);
            msg_i ("** Pip Horizontal sharpness gain from Table %d **", W_HFilterNum);
         }


         //
         // check if Scaling indexes are not as previous
         // Update pointers to appropriate PVF and PHF LUT
         //
#if defined(Qisda_93XX_BOARD) //KevinL 20120906 (P3_TestDrive bug#1) DVI 2560x1440 PBP issue
         if ((VertShrnkPipIndexRGBPrev != VertShrnkIndex) ||
              (HorShrnkPipIndexRGBPrev != HorShrnkIndex) ||
              (W_ColorSpace[CH_B] != W_ColorSpacePrev[CH_B]) ||
                  (b_ForceSharpnessFilterReload[CH_B] == TRUE))        
#else          
         if ((VertShrnkPipIndexRGBPrev != VertShrnkIndex) ||
               (HorShrnkPipIndexRGBPrev != HorShrnkIndex) ||
               (W_ColorSpace[CH_B] != W_ColorSpacePrev[CH_B]))
#endif               
         {
            //
            //    H O R I Z O N T A L   P I P    filters  RGB
            //
            Wp_PHF_UV	= (WORD far *)SharpnessTables_PHF_RGB[HorShrnkIndex].Wp_UV;
            Wp_PHF_Y	= (WORD far *)SharpnessTables_PHF_RGB[HorShrnkIndex].Wp_Y;
            Wp_PHF_UVpk	= (WORD far *)SharpnessTables_PHF_RGB[HorShrnkIndex].Wp_UVpk;
            Wp_PHF_Ypk	= (WORD far *)SharpnessTables_PHF_RGB[HorShrnkIndex].Wp_Ypk;
            //
            //    V E R T I C A L    P I P    filters  RGB
            //
            Bp_PVF_UV	= (BYTE far *)SharpnessTables_PVF_RGB[VertShrnkIndex].Bp_UV;
            Bp_PVF_Y	= (BYTE far *)SharpnessTables_PVF_RGB[VertShrnkIndex].Bp_Y;
            Bp_PVF_UVpk	= (BYTE far *)SharpnessTables_PVF_RGB[VertShrnkIndex].Bp_UVpk;
            Bp_PVF_Ypk	= (BYTE far *)SharpnessTables_PVF_RGB[VertShrnkIndex].Bp_Ypk;
            //
            // Load Vertical PVF LUT (PIP)
            // Load Horizontal PHF LUT (PIP)
            //
            msg("---->> Sharpness LUT loading for Pip RGB");
            gm_LoadSharpCoeffTable(gmd_TAB_PVF, (BYTE far *)Bp_PVF_UV, UV_PVF_START_ADDR, PVF_LUT_SIZE);
            gm_LoadSharpCoeffTable(gmd_TAB_PVF, (BYTE far *)Bp_PVF_Y, Y_PVF_START_ADDR, PVF_LUT_SIZE);
            gm_LoadSharpCoeffTable(gmd_TAB_PVF, (BYTE far *)Bp_PVF_UVpk, UVPK_PVF_START_ADDR, PVF_LUT_SIZE);
            gm_LoadSharpCoeffTable(gmd_TAB_PVF, (BYTE far *)Bp_PVF_Ypk, YPK_PVF_START_ADDR, PVF_LUT_SIZE);

            gm_LoadSharpCoeffTable(gmd_TAB_PHF, (BYTE far *)Wp_PHF_UV, UV_PHF_START_ADDR, PHF_LUT_SIZE);
            gm_LoadSharpCoeffTable(gmd_TAB_PHF, (BYTE far *)Wp_PHF_Y, Y_PHF_START_ADDR, PHF_LUT_SIZE);
            gm_LoadSharpCoeffTable(gmd_TAB_PHF, (BYTE far *)Wp_PHF_UVpk, UVPK_PHF_START_ADDR, PHF_LUT_SIZE);
            gm_LoadSharpCoeffTable(gmd_TAB_PHF, (BYTE far *)Wp_PHF_Ypk, YPK_PHF_START_ADDR, PHF_LUT_SIZE);

            VertShrnkPipIndexRGBPrev = VertShrnkIndex;
            HorShrnkPipIndexRGBPrev = HorShrnkIndex;
            W_ColorSpacePrev[CH_B] = W_ColorSpace[CH_B];
      #if defined(Qisda_93XX_BOARD) //KevinL 20120906 (P3_TestDrive bug#1) DVI 2560x1440 PBP issue            
            b_ForceSharpnessFilterReload[CH_B] = FALSE;            
      #endif               
         }
      }
      else
      {
         //
         //       Y U V    color space
         //
         VertShrnkIndex = gm_GetVertShrnkIndex_Video( D_VertScaleValue );
         HorShrnkIndex = gm_GetHorShrnkIndex_Video( D_HorScaleValue );
         msg_i ("Y U V__ Pip  VertShrnkIndex = %d", VertShrnkIndex);
         msg_i ("Y U V__ Pip  HorShrnkIndex = %d", HorShrnkIndex);

         if ((W_Sharpness != W_SharpnessPipPrev) ||
               (VertShrnkPipIndexVideoPrev != VertShrnkIndex) ||
               (HorShrnkPipIndexVideoPrev != HorShrnkIndex) ||
               (W_ColorSpace[CH_B] != W_ColorSpacePrev[CH_B]))
         {
            W_HFilterNum = ( PHF_SHARPNESS_LUMA & Ba_HShrpGainY_Pip[HorShrnkIndex][W_Sharpness] ) +
                           ( PHF_SHARPNESS_CHROMA & ((Ba_HShrpGainUV_Pip[HorShrnkIndex][W_Sharpness]) << 8));
            W_VTableNum = ( PVF_SHARPNESS_LUMA & Ba_VShrpGainY_Pip[VertShrnkIndex][W_Sharpness] ) +
                          ( PVF_SHARPNESS_CHROMA & ((Ba_VShrpGainUV_Pip[VertShrnkIndex][W_Sharpness]) << 8));

            gm_WriteRegWord( PHF_SHARPNESS, W_HFilterNum );
            gm_WriteRegWord( PVF_SHARPNESS, W_VTableNum	);

            W_SharpnessPipPrev = W_Sharpness;

            msg_i ("SHARPNESS slider Pip Video = %d", W_Sharpness);
            msg_i ("** Pip Vertical sharpness gain from Table %d **", W_VTableNum);
            msg_i ("** Pip Horizontal sharpness gain from Table %d **", W_HFilterNum);
         }

         //
         // check if Scaling indexes are not as previous
         // Update pointers to appropriate PVF and PHF LUT
         //
         if ((VertShrnkPipIndexVideoPrev != VertShrnkIndex) ||
               (HorShrnkPipIndexVideoPrev != HorShrnkIndex) ||
               (W_ColorSpace[CH_B] != W_ColorSpacePrev[CH_B]))
         {
            //
            //    H O R I Z O N T A L   P I P    filters  YUV
            //
            Wp_PHF_UV	= (WORD far *)SharpnessTables_PHF_YUV[HorShrnkIndex].Wp_UV;
            Wp_PHF_Y	= (WORD far *)SharpnessTables_PHF_YUV[HorShrnkIndex].Wp_Y;
            Wp_PHF_UVpk	= (WORD far *)SharpnessTables_PHF_YUV[HorShrnkIndex].Wp_UVpk;
            Wp_PHF_Ypk	= (WORD far *)SharpnessTables_PHF_YUV[HorShrnkIndex].Wp_Ypk;
            //
            //    V E R T I C A L    P I P    filters  YUV
            //
            Bp_PVF_UV	= (BYTE far *)SharpnessTables_PVF_YUV[VertShrnkIndex].Bp_UV;
            Bp_PVF_Y	= (BYTE far *)SharpnessTables_PVF_YUV[VertShrnkIndex].Bp_Y;
            Bp_PVF_UVpk	= (BYTE far *)SharpnessTables_PVF_YUV[VertShrnkIndex].Bp_UVpk;
            Bp_PVF_Ypk	= (BYTE far *)SharpnessTables_PVF_YUV[VertShrnkIndex].Bp_Ypk;
            //
            // Load Vertical PVF LUT (PIP)
            // Load Horizontal PHF LUT (PIP)
            //
            msg("---->> Sharpness LUT loading for Pip Video YUV ");

            gm_LoadSharpCoeffTable(gmd_TAB_PHF, (BYTE far *)Wp_PHF_UV, UV_PHF_START_ADDR, PHF_LUT_SIZE);
            gm_LoadSharpCoeffTable(gmd_TAB_PHF, (BYTE far *)Wp_PHF_Y, Y_PHF_START_ADDR, PHF_LUT_SIZE);
            gm_LoadSharpCoeffTable(gmd_TAB_PHF, (BYTE far *)Wp_PHF_UVpk, UVPK_PHF_START_ADDR, PHF_LUT_SIZE);
            gm_LoadSharpCoeffTable(gmd_TAB_PHF, (BYTE far *)Wp_PHF_Ypk, YPK_PHF_START_ADDR, PHF_LUT_SIZE);

            gm_LoadSharpCoeffTable(gmd_TAB_PVF, (BYTE far *)Bp_PVF_UV, UV_PVF_START_ADDR, PVF_LUT_SIZE);
            gm_LoadSharpCoeffTable(gmd_TAB_PVF, (BYTE far *)Bp_PVF_Y, Y_PVF_START_ADDR, PVF_LUT_SIZE);
            gm_LoadSharpCoeffTable(gmd_TAB_PVF, (BYTE far *)Bp_PVF_UVpk, UVPK_PVF_START_ADDR, PVF_LUT_SIZE);
            gm_LoadSharpCoeffTable(gmd_TAB_PVF, (BYTE far *)Bp_PVF_Ypk, YPK_PVF_START_ADDR, PVF_LUT_SIZE);

            VertShrnkPipIndexVideoPrev = VertShrnkIndex;
            HorShrnkPipIndexVideoPrev = HorShrnkIndex;
            W_ColorSpacePrev[CH_B] = W_ColorSpace[CH_B];
         }
      }
   }

}
#endif

//*********************** end of file *******************************************

