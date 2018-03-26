/*
	$Workfile:   gm_CscUpdateColors.c  $
	$Revision: 1.34 $
	$Date: 2012/03/29 07:25:56 $
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
// MODULE:		gm_CscUpdateColors.c
//																			
// USAGE:		This module contains color space control functions for		
//				STDP93xx controller.
//																			
//******************************************************************************

//******************************************************************************
//	I N C L U D E    F I L E S												
//******************************************************************************

#include "StdLibInc\gm_Global.h"
#include "StdLibInc\gm_HostReg.h"
#include "StdLibInc\gm_Register.h"
#include "StdLibInc\gm_StdLib.h"
#include "StdLibInc\gm_GlobalVariables.h"
#include "StdLibInc\gm_ColorControl.h"
#include "StdLibInc\gm_ChipFeature.h"
#include "StdLibInc\gm_ModeSetup.h"
#include "System\all.h"


extern ROM BYTE gmc_EnableAccAcmOnGraphic;

//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************

#define x_DEBUG

#ifdef _DEBUG
			#define  msg(a)             gm_Print(a,0)
			#define  msg_i(a,b)         gm_Print(a,b)
#else
			#define  msg(a)
			#define  msg_i(a,b)
#endif

#define SIZE_OF_COLOR_STRUCT 		sizeof (gmt_ColorStruct)

//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************

extern SWORD ROM gmc_CSCCoeff [4][9];
extern BYTE Input_RGB_Range;
extern BYTE Input_YUV_Range;
#if(FEATURE_VWD == ENABLE)
extern BYTE ChangeByVWD;
#endif
#define Second_CSC_BLOCK_START		MAIN_S_MULT_COEF11

//******************************************************************************
//  S T A T I C    V A R I A B L E S
//******************************************************************************

static gmt_PHY_CH gmvb_CscChannel;
static gmt_ColorStruct * gStp_CscAdj;
static SWORD ROM *CSC_CoeffPtr[4];
static BOOL CscPtrInit=FALSE;

static ROM BYTE CSC_YUV_COEFF [9] =		//
{
   // Athena register coefficients
	COEF_11, COEF_12, COEF_13,
	COEF_21, COEF_22, COEF_23,			//
	COEF_31, COEF_32, COEF_33,					
};

static WORD ROM W_SF = 2004; // 0.489346902 in 4.12 notation
static WORD ROM W_OffsetIn = 5906; // OffsetIn for OVP 3x3 matrix output offset.
static WORD ROM W_OffsetIn_YUV = 0; //20120711 Add by KevinChen for YUV 32 gray color issue 
//******************************************************************************
//  S T A T I C    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

static void gmp_UpdateOffset (void);
static void gmp_UpdateOffset_YUV (void);//20120711 Add by KevinChen for YUV 32 gray color issue

static void gmp_UpdateRGBCoeff (void);
static void gmp_UpdateYUVCoeff (gmt_MODE_FLAGS gmvw_Input);

SWORD Sine (SWORD sW_Degree);
SWORD Cosine (SWORD sW_Degree);

//******************************************************************************
//  C O D E                                                                 
//******************************************************************************

//******************************************************************************
//
// FUNCTION       : gmt_GetCSC gm_GetCsc (BYTE Select)
//
// USAGE          : Returns pointer to the active array of Color Space Coefficieants
//
// INPUT          : Select = 0  Requesting SD CSC pointer
//                 Select = 1	Requesting HD CSC pointer
//
// OUTPUT         : Pointer to CSC;
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
gmt_GetCSC gm_GetCsc(BYTE B_Select)
{
	gmt_GetCSC CscData;

   CscData.Coeff = ((B_Select < 4) && (CscPtrInit == TRUE)) ? CSC_CoeffPtr[B_Select] : NULL_PTR;
	CscData.Shift = CSC_YUV_COEFF;
	return CscData;
}
// FUNCTION       : gmt_RET_STAT gm_CscUpdateMatrix (SWORD ROM *CSC_SD_Coeff, SWORD ROM *CSC_HD_Coeff)
//
// USAGE          : Updates the 3x3 matrix selection
//
// INPUT          : CSC_SD_Coeff - pointer to SD 3x3 matrix
//                  CSC_HD_Coeff - pointer to HD 3x3 matrix
//
// OUTPUT         : OK			-	Success
//                  ERR_POINTER	-	Invalid pointer
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
gmt_RET_STAT gm_CscUpdateMatrix (SWORD ROM *CSC_SD_Coeff, SWORD ROM *CSC_HD_Coeff, SWORD ROM *CSC_SD_Coeff_F, SWORD ROM *CSC_HD_Coeff_F)
{
   if (CSC_SD_Coeff == NULL_PTR || CSC_HD_Coeff == NULL_PTR || CSC_SD_Coeff_F == NULL_PTR || CSC_HD_Coeff_F == NULL_PTR)
		return (ERR_POINTER);

	CscPtrInit = TRUE;					// Set flag
	CSC_CoeffPtr[0] = CSC_SD_Coeff;		// Set SD matrix
	CSC_CoeffPtr[1] = CSC_HD_Coeff;		// Set HD matrix
   CSC_CoeffPtr[2] = CSC_SD_Coeff_F;		// Set SD matrix
   CSC_CoeffPtr[3] = CSC_HD_Coeff_F;		// Set HD matrix
	return (OK);
}

//******************************************************************************
//
// FUNCTION       : gmt_RET_STAT gm_CscUpdateColors (gmt_PHY_CH B_Channel, gmt_ColorStruct * Stp_Parameters)
//
// USAGE          : Updates the color settings on the selected channel. This 
//                  will be called directly by the application. To change a 
//                  color parameter, the application must change the appropriate 
//                  color parameter in the color structure, then call this 
//                  function to execute the change.					
//
// INPUT          : B_Channel	-	CH_A or CH_B
//                  Stp_Parameters	-	pointer to structure containing color 
//                     related parameters
//
// OUTPUT         : OK			-	Success
//                  ERR_PARAM	-	Unknown color space detected in specified 
//                  channel
//
// GLOBALS        : gmvb_CscChannel		(RW)
//                  gmvw_InputModeAttr	(RO)
//                  gStp_CscAdj			(RO)
//                  gW_HostUpdate		(W)
//
// USED_REGS      : None
//
// PRE_CONDITION  : 
//
// POST_CONDITION : 
//
//******************************************************************************
gmt_RET_STAT gm_CscUpdateColors (gmt_PHY_CH B_Channel, gmt_ColorStruct * Stp_Parameters)
{
   gmt_MODE_FLAGS	gmvw_Input; 										// CSC conversion type
   WORD W_InputRegist;
   gStp_CscAdj = Stp_Parameters;										// Store params in local global

#if (FEATURE_CROSS_POINT_SWITCH == ENABLE)
   // if TRUE, then MAIN channel uses PIP 3x3 and vice versa
	//if (gm_ReadRegWord(ODP_CTRL) & COLOR_RESOURCE_SEL)
	//if((gm_ReadRegWord(OVP_CTRL) & OVP_IN_MUX) && (gm_ReadRegWord(OVP_CTRL)&RGB2YUV_WIN_CTRL))
	if(gm_ReadRegWord(OVP_CTRL) & OVP_IN_MUX)
	{
      gmvb_CscChannel = gm_OtherChannel(B_Channel);					// Initialize local static;
	}
	else
	{
      gmvb_CscChannel = B_Channel;									// Initialize local static	
	}
#else
	gmvb_CscChannel = B_Channel;										// Initialize local static
#endif

   gmvw_Input = stInput[B_Channel].ModeFlags & gmd_MODE_SIGNAL_MASK;	// Obtain input type

   // 422 to 444 ...
   {
      BYTE B_OVPInterpolatorBit;
      
      if(B_Channel == CH_A)
      {
         W_InputRegist = IMP_CONTROL;
         B_OVPInterpolatorBit = MAIN_INTERPOLATOR_BYPASS;
      }
      else
      {
         W_InputRegist = IPP_CONTROL;
         B_OVPInterpolatorBit = PIP_INTERPOLATOR_BYPASS;
      }

      if(gm_ReadRegWord(W_InputRegist)&IMP_YUV_EN)
      {
         gm_ClearRegBitsWord(OVP_CTRL, B_OVPInterpolatorBit);
      }
      else
      {
         gm_SetRegBitsWord(OVP_CTRL, B_OVPInterpolatorBit);
      }
   }
   msg_i("B_Channel-%d",B_Channel);
   msg_i("gmvw_Input-%d",gmvw_Input);
   //
	// Determine what type of processing should be performed
	//
	switch (gmvw_Input)
	{
      case gmd_MODE_GRAPHICS:
         //
			// Obtain and load offsets and coefficients for graphics inputs
			//
         if (stInput[B_Channel].ColorDomain == Input_RGB)
         {
            if((gm_ReadRegWord(W_InputRegist)&IMP_YUV_EN) || 
               (((gm_ReadRegWord(ACM3_CTRL) & ACM3_EN) ||   // ACM or ACC enabled
                  (gm_ReadRegWord(ACC_CTRL) & ACC_ENABLE)) && 
                ((gm_ReadRegWord(OVP_CTRL) & OVP_IN_MUX) == B_Channel)))
            {
               if(gmvw_Input == gmd_MODE_GRAPHICS)
                  gmvw_Input = gmd_MODE_HDVIDEO;            

               msg_i(" 4 1 YUV ....... %d", Input_YUV_Range);
               #ifdef VWD_INSIDE_WINDOW_APPLY_ONLY
			    //121113 Edward for non ACM mode , outside window is RGB domain don't use YUV coefficient 
			   	if(IsVwdDetected()&&(ChangeByVWD==0)&&(!IsAcmEnablePresetMode()))
			   		gmp_UpdateRGBCoeff();
			    else
			   #endif
			   		gmp_UpdateYUVCoeff(gmvw_Input);
			   	
            }
            else
            {
               msg_i(" 4 RGB ....... %d", Input_RGB);
               gmp_UpdateRGBCoeff();
            }
         }			
         else //(stInput[B_Channel].ColorDomain == Input_YUV_Reduce)
         {
            if (gmvw_Input == gmd_MODE_GRAPHICS)
               gmvw_Input = gmd_MODE_HDVIDEO;

            msg_i(" 5 YUV ....... %d", Input_YUV_Range);
            gmp_UpdateYUVCoeff(gmvw_Input);
         }

			break;
	
		case gmd_MODE_SDVIDEO:
		case gmd_MODE_HDVIDEO:
		case gmd_MODE_EDVIDEO:
			//		
			// Obtain and load offsets and coefficients for YUV inputs
			//
			if(stInput[B_Channel].ColorDomain == Input_RGB)
         {         			
            if((gm_ReadRegWord(W_InputRegist)&IMP_YUV_EN) || 
               (((gm_ReadRegWord(ACM3_CTRL) & ACM3_EN) ||   // ACM or ACC enabled
                  (gm_ReadRegWord(ACC_CTRL) & ACC_ENABLE)) &&
                ((gm_ReadRegWord(OVP_CTRL) & OVP_IN_MUX) == B_Channel)))
            {
               gmp_UpdateYUVCoeff(gmvw_Input);
            }
            else
   			gmp_UpdateRGBCoeff();
         }
         else
         {
            gmp_UpdateYUVCoeff(gmvw_Input);            
         }
			break;
	
		default:
			return (ERR_PARAM);
	}
	if((B_Channel == CH_A)&&(stInput[B_Channel].ColorDomain != Input_RGB))//20120711 Add by KevinChen for YUV 32 gray color issue
	{
		gmp_UpdateOffset_YUV();
	}
	else
	{
		if((gm_ReadRegWord(W_InputRegist)&IMP_YUV_EN) || 
			(((gm_ReadRegWord(ACM3_CTRL) & ACM3_EN) ||	 // ACM or ACC enabled
			(gm_ReadRegWord(ACC_CTRL) & ACC_ENABLE)) &&
			((gm_ReadRegWord(OVP_CTRL) & OVP_IN_MUX) == B_Channel)))
		{
				gmp_UpdateOffset_YUV();
		}
		else
		{
				gmp_UpdateOffset();
		}
	}	

   
   // RGB2YUV
   if(B_Channel == gmvb_MainChannel) //KevinL 20120921 for main channel only, it will be effected by PIP if main condition matched!!!
   {
   if ((gm_ReadRegWord (IMP_CONTROL)  & IMP_YUV_EN)
         || (((gm_ReadRegWord(ACM3_CTRL) & ACM3_EN) == 0)   // No ACM and ACC enabled
            && ((gm_ReadRegWord(ACC_CTRL) & ACC_ENABLE) == 0)))
   {
      if (gm_IsValidSignal(gmvb_MainChannel))	
      gm_WaitDisplayBlanking_CH(B_Channel);		//Neil120724* enable it avoid garbage when change preset mode, update in blanking
      gm_ClearRegBitsWord(OVP_CTRL, RGB2YUV_WIN_CTRL);      
      //gm_Printf("~~~~ YUV input or ACC ACM are not enable ... %d", B_Channel);
   }
   else
   {
      gm_WaitDisplayBlanking_CH(B_Channel);		//Neil120724* enable it avoid garbage when change preset mode, update in blanking
      if(((UserPrefCurrentInputMain == ALL_BOARDS_DVI1)||(UserPrefCurrentInputMain == ALL_BOARDS_VGA1))&&(UserPrefColorFormat== InputColorFormat_YUV))
      		gm_ClearRegBitsWord(OVP_CTRL, RGB2YUV_WIN_CTRL);
	  else
	  	{
	  	#ifdef VWD_INSIDE_WINDOW_APPLY_ONLY
	  		//if(IsVwdDetected())
	  		//121113 Edward for VWD enable , RGB to YUV only apply inside window for non ACM mode. 
	  		if(IsVWDEnabled()&& !IsAcmEnablePresetMode())
				gm_SetRegBitsWord(OVP_CTRL, 0x0400);
			else
		#endif		
      			gm_SetRegBitsWord(OVP_CTRL, RGB2YUV_WIN_CTRL);
	  	}
      //gm_Printf("~~~~ RGB input and ACC ACM are enable ... %d", B_Channel);
   }
   }

   gm_SetRegBitsDWord(OVP_UPDATE_CTRL, OVP_SYNC_UPDATE);

#ifdef USE_SMT
   if(B_Channel == CH_A)
   {
      extern BYTE B_ColorSpaceConversion;
      
      if((gmvw_Input == gmd_MODE_SDVIDEO) || (gmvw_Input == gmd_MODE_EDVIDEO))
		{
			if(Input_YUV_Range == 1) // Full Range 
         B_ColorSpaceConversion = 4;
			else
				B_ColorSpaceConversion = 6;				
		}
      else if(gmvw_Input == gmd_MODE_HDVIDEO)
		{
			if(Input_YUV_Range == 1) // Full Range 
         B_ColorSpaceConversion = 5;
      else
				B_ColorSpaceConversion = 7;
   }
      else // Graphics
         B_ColorSpaceConversion = 10;
   }
#endif   
   
	return (OK);
}

//******************************************************************************
//
// FUNCTION       : static void gmp_UpdateOffset (void)
//
// USAGE          : Updates the YUV offsets by writing to the offset registers
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : gmvb_CscChannel			(RO)
//                  gStp_CscAdj				(RO)
//
// USED_REGS      : MAIN_OFFSET1_A			(W)
//                  MAIN_OFFSET2_A			(W)
//                  MAIN_OFFSET3_A			(W)
//                  PIP_OFFSET1				(W)
//                  PIP_OFFSET2				(W)
//                  PIP_OFFSET3				(W)
//                  MAIN_OFFSET1_B			(W)		
//                  MAIN_OFFSET2_B			(W)		
//                  MAIN_OFFSET3_B			(W)		
//
// PRE_CONDITION  : 
//
// POST_CONDITION : 
//
//******************************************************************************
static void gmp_UpdateOffset (void)
{
	SWORD sW_ROffset, sW_GOffset, sW_BOffset;	
	//
	// Get RGB offset values
	//
	#if(FEATURE_VWD == ENABLE)
	if(ChangeByVWD==1)
	{
		sW_ROffset = UserPrefOffset_Vwd << 6;
		sW_GOffset = UserPrefOffset_Vwd << 6;
		sW_BOffset = UserPrefOffset_Vwd << 6; 
	}
	else
	#endif
	{
		sW_ROffset = (gStp_CscAdj->sB_Brightness) << 6;
		sW_GOffset = (gStp_CscAdj->sB_Brightness) << 6;
		sW_BOffset = (gStp_CscAdj->sB_Brightness) << 6; 
	}

	//120608 JP to extend range//120718 Edward disable and extend range at CSCupdateColor
	sW_ROffset += (gStp_CscAdj->sB_RedOff);
    sW_GOffset += (gStp_CscAdj->sB_GreenOff);
    sW_BOffset += (gStp_CscAdj->sB_BlueOff);

	// xvYCC offset // assume CH_A is main channel ...
	if(((stInput[CH_A].Flags & gmd_INPUT_xvYCC_601) ||
		(stInput[CH_A].Flags & gmd_INPUT_xvYCC_709)) && 
		(gmvb_CscChannel == CH_A)) // PIP channel doesn't support xvYCC for now.
	{
      sW_ROffset += W_OffsetIn;
      sW_GOffset += W_OffsetIn;
      sW_BOffset += W_OffsetIn;      
	}

	#if(FEATURE_VWD == ENABLE)
	if(ChangeByVWD==1)
	{
		gm_WriteRegWord (MAIN_S_OUT_OFFSET1, sW_GOffset);
		gm_WriteRegWord (MAIN_S_OUT_OFFSET2, sW_BOffset);
		gm_WriteRegWord (MAIN_S_OUT_OFFSET3, sW_ROffset);

	}
	else
	#endif
	{
		if (gmvb_CscChannel == CH_A)
		{
			gm_WriteRegWord (MAIN_P_OUT_OFFSET1, sW_GOffset);
			gm_WriteRegWord (MAIN_P_OUT_OFFSET2, sW_BOffset);
			gm_WriteRegWord (MAIN_P_OUT_OFFSET3, sW_ROffset);
		}
		else
		{
			gm_WriteRegWord (PIP_OUT_OFFSET1, sW_GOffset);
			gm_WriteRegWord (PIP_OUT_OFFSET2, sW_BOffset);
			gm_WriteRegWord (PIP_OUT_OFFSET3, sW_ROffset);
		}
	}
}

static void gmp_UpdateOffset_YUV (void)//20120711 Add by KevinChen for YUV 32 gray color issue
{
	SWORD sB_ROffset, sB_GOffset, sB_BOffset;	
	//
	// Get RGB offset values
	//

	
	#if(FEATURE_VWD == ENABLE)
	if(ChangeByVWD==1)
	{
		sB_ROffset = UserPrefOffset_Vwd << 6;
		sB_GOffset = UserPrefOffset_Vwd << 6;
		sB_BOffset = UserPrefOffset_Vwd << 6; 
	}
	else
	#endif
	{
		sB_ROffset = (gStp_CscAdj->sB_Brightness) << 6;
		sB_GOffset = (gStp_CscAdj->sB_Brightness) << 6;
		sB_BOffset = (gStp_CscAdj->sB_Brightness) << 6; 
	}

	//120608 JP to extend range//120718 Edward disable and extend range at CSCcolorupdate
	sB_ROffset += (gStp_CscAdj->sB_RedOff);
    sB_GOffset += (gStp_CscAdj->sB_GreenOff);
    sB_BOffset += (gStp_CscAdj->sB_BlueOff);

	// xvYCC offset // assume CH_A is main channel ...
	//if((stInput[CH_A].Flags & gmd_INPUT_xvYCC_601) ||
		//(stInput[CH_A].Flags & gmd_INPUT_xvYCC_709))
	{
      sB_ROffset += W_OffsetIn_YUV;
      sB_GOffset += W_OffsetIn_YUV;
      sB_BOffset += W_OffsetIn_YUV;      
	}
	#if(FEATURE_VWD == ENABLE)
	if(ChangeByVWD==1)
	{
		gm_WriteRegWord (MAIN_S_OUT_OFFSET1, sB_GOffset);
		gm_WriteRegWord (MAIN_S_OUT_OFFSET2, sB_BOffset);
		gm_WriteRegWord (MAIN_S_OUT_OFFSET3, sB_ROffset);

	}
	else
	#endif
	{
		if (gmvb_CscChannel == CH_A)
		{
			gm_WriteRegWord (MAIN_P_OUT_OFFSET1, sB_GOffset);
			gm_WriteRegWord (MAIN_P_OUT_OFFSET2, sB_BOffset);
			gm_WriteRegWord (MAIN_P_OUT_OFFSET3, sB_ROffset);
		}
		else
		{
			gm_WriteRegWord (PIP_OUT_OFFSET1, sB_GOffset);
			gm_WriteRegWord (PIP_OUT_OFFSET2, sB_BOffset);
			gm_WriteRegWord (PIP_OUT_OFFSET3, sB_ROffset);
		}
	}
}

//******************************************************************************
//
// FUNCTION       : static void gmp_UpdateRGBCoeff (void)
//
// USAGE          : Updates the RGB coefficients by writing to the coefficient 
//                  registers
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : gmvb_CscChannel			(RO)
//                  gStp_CscAdj				(RO)
//
// USED_REGS      : MAIN_MULT_COEF11_A 		(W)
//                  MAIN_MULT_COEF12_A 		(W)
//                  MAIN_MULT_COEF13_A 		(W)
//                  MAIN_MULT_COEF21_A 		(W)
//                  MAIN_MULT_COEF22_A 		(W)
//                  MAIN_MULT_COEF23_A 		(W)
//                  MAIN_MULT_COEF31_A 		(W)
//                  MAIN_MULT_COEF32_A		(W)
//                  MAIN_MULT_COEF33_A 		(W)
//
//                  PIP_MULT_COEF11 		(W)
//                  PIP_MULT_COEF12 		(W)
//                  PIP_MULT_COEF13 		(W)
//                  PIP_MULT_COEF21 		(W)
//                  PIP_MULT_COEF22 		(W)
//                  PIP_MULT_COEF23 		(W)
//                  PIP_MULT_COEF31 		(W)
//                  PIP_MULT_COEF32 		(W)
//                  PIP_MULT_COEF33 		(W)
//
//                  MAIN_MULT_COEF11_B 		(W)
//                  MAIN_MULT_COEF12_B 		(W)
//                  MAIN_MULT_COEF13_B 		(W)
//                  MAIN_MULT_COEF21_B 		(W)
//                  MAIN_MULT_COEF22_B 		(W)
//                  MAIN_MULT_COEF23_B 		(W)
//                  MAIN_MULT_COEF31_B 		(W)
//                  MAIN_MULT_COEF32_B		(W)
//                  MAIN_MULT_COEF33_B 		(W)
//
// PRE_CONDITION  : 
//
// POST_CONDITION : 
//
//******************************************************************************
static void gmp_UpdateRGBCoeff (void)
{
	WORD W_Addr, W_Contrast;
	DWORD Dw_RGain, Dw_GGain, Dw_BGain;	
	//
	// Obtain contrast value and convert to (Signed 4.12) fixed point format (0x100 equal 0x1.000)
	// 1 sign bit, 3 integer bits and 12 fraction bits. 
	// By shifting left 4 bits.	
	W_Contrast = gStp_CscAdj->W_Contrast << 4;
	//
	// Multiply RGB gain values with same contrast value to program RGB gains.
	// This is done by multiplying 2 (4.12 notation) value to obtain a (Signed 8.24) value
	// then convert (Signed 8.24) to to (Signed 2.8) since this is what the HW expects. 
	// Fixed point format (0x1.000 equal 0x1.00). Achieve by shifting right 16 bits.	
	//	
	Dw_RGain = (DWORD) (gStp_CscAdj->W_RedGain << 4) * (DWORD)W_Contrast;
	RSHFT(Dw_RGain, 16);

	Dw_GGain = (DWORD) (gStp_CscAdj->W_GreenGain << 4) * (DWORD)W_Contrast;
	RSHFT(Dw_GGain, 16);

	Dw_BGain = (DWORD) (gStp_CscAdj->W_BlueGain << 4) * (DWORD)W_Contrast;
	RSHFT(Dw_BGain, 16);
	//
	// Choose to update Main or Pip coefficients
	// 

	msg_i("Dw_RGain-%d",Dw_RGain);
	msg_i("Dw_GGain-%d",Dw_GGain);
	msg_i("Dw_BGain-%d",Dw_BGain);

	
	if (gmvb_CscChannel == CH_A)					
		W_Addr = MAIN_CSC_BLOCK_START;			// use MAIN coefficients
	else
		W_Addr = PIP_CSC_BLOCK_START;			// use PIP coefficients

//	  TRUNC (W_CellVal, 0x80, 0x180);			// TODO: verify truncate macro (0.5 < Contr < 1.5) (discuss with CT)
	//
	// Write the coefficient values based on the contrast settings
	//
   // Dw_xGain is in 9-bit so shift left 1 to be 10-bit
   #if 1 // Order G/B/R
	gm_WriteRegWord (W_Addr + COEF_33, (WORD) Dw_RGain << 1);
	gm_WriteRegWord (W_Addr + COEF_11, (WORD) Dw_GGain << 1);
	gm_WriteRegWord (W_Addr + COEF_22, (WORD) Dw_BGain << 1);   
   #else
	gm_WriteRegWord (W_Addr + COEF_11, (WORD) Dw_RGain << 1);
	gm_WriteRegWord (W_Addr + COEF_22, (WORD) Dw_GGain << 1);
	gm_WriteRegWord (W_Addr + COEF_33, (WORD) Dw_BGain << 1);	
   #endif

	//
	// Zero out the remaining coefficients (not used in the main channel for RGB inputs)
	//
	gm_WriteRegWord (W_Addr + COEF_12, 0);
	gm_WriteRegWord (W_Addr + COEF_13, 0);
	gm_WriteRegWord (W_Addr + COEF_21, 0);
	gm_WriteRegWord (W_Addr + COEF_23, 0);
	gm_WriteRegWord (W_Addr + COEF_31, 0);
	gm_WriteRegWord (W_Addr + COEF_32, 0);

   // Need to program Input offset.
	gm_WriteRegWord (W_Addr - 12, 0x0);
	gm_WriteRegWord (W_Addr - 8, 0x0);
	gm_WriteRegWord (W_Addr - 4, 0x0);	
}

//******************************************************************************
//
// FUNCTION       : static void gmp_UpdateYUVCoeff (gmt_MODE_FLAGS gmvw_Input)
//
// USAGE          : Updates the RGB coefficients by writing to the coefficient 
//                  registers
//
// INPUT          : gmvw_Input -
//                     gmd_MODE_SDVIDEO	
//                     gmd_MODE_HDVIDEO
//                     gmd_MODE_EDVIDEO
//
// OUTPUT         : None
//
// GLOBALS        : gmvb_CscChannel	(RO)
//                  gStp_CscAdj		(RO)
//                  CSC_CoeffPtr	(RW)
//
// USED_REGS      : MAIN_MULT_COEF11_A	(W)
//                  MAIN_MULT_COEF12_A	(W)
//                  MAIN_MULT_COEF13_A	(W)
//                  MAIN_MULT_COEF21_A	(W)
//                  MAIN_MULT_COEF22_A	(W)
//                  MAIN_MULT_COEF23_A	(W)
//                  MAIN_MULT_COEF31_A	(W)
//                  MAIN_MULT_COEF32_A	(W)
//                  MAIN_MULT_COEF33_A	(W)
//
//                  PIP_MULT_COEF11	(W)
//                  PIP_MULT_COEF12	(W)
//                  PIP_MULT_COEF13	(W)
//                  PIP_MULT_COEF21	(W)
//                  PIP_MULT_COEF22	(W)
//                  PIP_MULT_COEF23	(W)
//                  PIP_MULT_COEF31	(W)
//                  PIP_MULT_COEF32	(W)
//                  PIP_MULT_COEF33	(W)
// 
//
//                  MAIN_MULT_COEF11_B	(W)
//                  MAIN_MULT_COEF12_B	(W)
//                  MAIN_MULT_COEF13_B	(W)
//                  MAIN_MULT_COEF21_B	(W)
//                  MAIN_MULT_COEF22_B	(W)
//                  MAIN_MULT_COEF23_B	(W)
//                  MAIN_MULT_COEF31_B	(W)
//                  MAIN_MULT_COEF32_B	(W)
//                  MAIN_MULT_COEF33_B	(W)
//
// PRE_CONDITION  : 
//
// POST_CONDITION : 
//
//******************************************************************************
static void gmp_UpdateYUVCoeff (gmt_MODE_FLAGS gmvw_Input)
{
	SDWORD	sDwa_Matrix[9];
	SWORD	sW_Saturation, sW_Contrast, sW_SatCosHue, sW_SatSinHue;
	WORD 	W_RedGain, W_GreenGain, W_BlueGain;	
	SDWORD	sDw_Temp;
	WORD 	W_i, W_Addr, W_CscIndex;

#ifdef YUV_GAIN_ADJ_IN_YUV_DOMAIN
	WORD W_YGain, W_UGain, W_VGain;
#endif // YUV_GAIN_ADJUSTMENT_IN_YUV

	if (CscPtrInit == FALSE)				// Not initialize
	{
		gm_CscUpdateMatrix((SWORD ROM *)&gmc_CSCCoeff[0], 
                         (SWORD ROM *)&gmc_CSCCoeff[1],
                         (SWORD ROM *)&gmc_CSCCoeff[2],
                         (SWORD ROM *)&gmc_CSCCoeff[3]);
	}
	if (gmvw_Input == gmd_MODE_SDVIDEO || gmvw_Input == gmd_MODE_EDVIDEO)
   {
      if (Input_YUV_Range == 1) // Full Range
         W_CscIndex = 2;
      else
		   W_CscIndex = 0;
   }
	else if (gmvw_Input == gmd_MODE_HDVIDEO)
   {
      if (Input_YUV_Range == 1) // Full Range
         W_CscIndex = 3;
      else
		   W_CscIndex = 1;
   }
	else
		return;								// Error input
	
	//
	// Get Contrast value
	//
	sW_Contrast = (SWORD)gStp_CscAdj->W_Contrast;
	//
	// Convert WORD to SWORD (Signed 4.12) fixed point format (0x100 equal 0x1.000)
	// 1 sign bit, 3 integer bits and 12 fraction bits. 
	// By shift left 4 bits.	
	// TODO: consider programmable precision (i.e. n-bit decimals as opposed to 12 bit precision)
	//
	LSHFT(sW_Contrast, 4);
	//
	// Get Saturation value
	//
	sW_Saturation = (SWORD)gStp_CscAdj->W_Saturation;
	//
	// Convert BYTE to SWORD (Signed 4.12) fixed point format (0x100 equal 0x1.000)
	// 1 sign bit, 3 integer bits and 12 fraction bits.
	// By shift left 4 bits.	
	//
	LSHFT(sW_Saturation, 4);                          										
/*	
	//
	// Counter "neon effect" by scaling with contrast.
	//
	TRUNC (sW_Saturation, 0, 0x1C80);	// TODO: This adjustment should be handled by application
*/	
	//
	// For hue values, the default library supports 1/4 degree increments for any degree [0..360]
	// By changing the OSD Workbench stepsize, min/max ranges the range can be limited.
	//
	sW_SatSinHue = (SWORD) Sine(gStp_CscAdj->sW_Hue);		// SinH
	sW_SatCosHue = (SWORD) Cosine(gStp_CscAdj->sW_Hue);		// CosH
	//
	// Multiply Saturation and Cosine(Hue) at position [1,1] and [2,2] in 3x3 matrix
	// Saturation * Cosine(Hue)
	// (Signed 8.24) = (Signed 4.12) * (Signed 4.12)
	//
	sDw_Temp = (SDWORD)sW_Saturation * (SDWORD)sW_SatCosHue;
	//
	// Convert SDWORD (Signed 8.24) to SWORD (Signed 4.12) fixed point format (0x01.000000 equal 0x1.000)
	// By shift right 12 bits.
	//
	RSHFT(sDw_Temp, 12);
	sW_SatCosHue = (SWORD)sDw_Temp;		// Store Sat*Cos(Hue)
	//
	// Multiply Saturation and Cosine(Hue) at position [1,2] and [2,1] in 3x3 matrix	
	// Saturation * Sine(Hue)
	// (Signed 8.24) = (Signed 4.12) * (Signed 4.12)
	//
	sDw_Temp = (SDWORD)sW_Saturation * (SDWORD)sW_SatSinHue;
	//
	// Convert SDWORD (Signed 8.24) to SWORD (Signed 4.12) fixed point format (0x01.000000 equal 0x1.000)
	// By shift right 12 bits.
	//	
	RSHFT(sDw_Temp, 12);	
	sW_SatSinHue = (SWORD)sDw_Temp;		// Store Sat*Sin(Hue)
	//
	// Perform the final matrix calculation now that all the necessary parameters are available
	//
	//	[a b c]		[ Contr	  0 			0				] 
	//	[d e f]	X	[ 0    	Sat*cos(Hue)  	Sat*sin(Hue)	]  where a-i are the conversion coefficients
	//	[g h i]  	[ 0   	-Sat*sin(Hue)   Sat*cos(Hue)	]
	//
	// Instead of performing matrix multiplication at run time the multiplication will result in the following equation:
	//
	// [	[ G * a * Cont] * (Y)	 [G * b * S * cos(hue) - G * c * S * sin(hue)] * (U)   [G * b * S * sin(hue) + B * c * S * cos(hue)] * (V)  ]
	// [	[ B * d * Cont] * (Y)	 [B * e * S * cos(hue) - G * f * S * sin(hue)] * (U)    [B * e * S * sin(hue) + B * f * S * cos(hue)] * (V)  ]
	// [	[ R * g * Cont] * (Y)	 [R * h * S * cos(hue) - G * i * S * sin(hue)] * (U)    [R * h * S * sin(hue) + B * i * S * cos(hue)] * (V)  ]
	sDwa_Matrix[0] = (SDWORD) sW_Contrast * (SDWORD) CSC_CoeffPtr[W_CscIndex][0];
	
	sDwa_Matrix[1] = ( (SDWORD) sW_SatCosHue * (SDWORD) CSC_CoeffPtr[W_CscIndex][1]) -
					( (SDWORD) sW_SatSinHue * (SDWORD) CSC_CoeffPtr[W_CscIndex][2]);
	
	sDwa_Matrix[2] = ( (SDWORD) sW_SatSinHue * (SDWORD) CSC_CoeffPtr[W_CscIndex][1]) + 
					( (SDWORD) sW_SatCosHue * (SDWORD) CSC_CoeffPtr[W_CscIndex][2]);
	
	sDwa_Matrix[3] = (SDWORD) sW_Contrast * (SDWORD) CSC_CoeffPtr[W_CscIndex][3];
	
	sDwa_Matrix[4] = ( (SDWORD) sW_SatCosHue * (SDWORD) CSC_CoeffPtr[W_CscIndex][4]) - 
					( (SDWORD) sW_SatSinHue * (SDWORD) CSC_CoeffPtr[W_CscIndex][5]);
	
	sDwa_Matrix[5] = ( (SDWORD) sW_SatSinHue * (SDWORD) CSC_CoeffPtr[W_CscIndex][4]) + 
					( (SDWORD) sW_SatCosHue * (SDWORD) CSC_CoeffPtr[W_CscIndex][5]);
	
	sDwa_Matrix[6] = (SDWORD) sW_Contrast * (SDWORD) CSC_CoeffPtr[W_CscIndex][6];
	
	sDwa_Matrix[7] = ( (SDWORD) sW_SatCosHue * (SDWORD) CSC_CoeffPtr[W_CscIndex][7]) - 
					( (SDWORD) sW_SatSinHue * (SDWORD) CSC_CoeffPtr[W_CscIndex][8]);
	
	sDwa_Matrix[8] = ( (SDWORD) sW_SatSinHue * (SDWORD) CSC_CoeffPtr[W_CscIndex][7]) + 
					( (SDWORD) sW_SatCosHue * (SDWORD) CSC_CoeffPtr[W_CscIndex][8]);
	//
	// RGB Gain adjusment (in YUV domain) will be used for color temperature adjustments
	//
	// Convert WORD to (Signed 4.12) fixed point format (0x100 equal 0x1.000)
	// 1 sign bit, 3 integer bits and 12 fraction bits. 
	// By shifting left 4 bits.	
	//
	W_GreenGain		= gStp_CscAdj->W_GreenGain << 4;
	W_RedGain 		= gStp_CscAdj->W_RedGain << 4;
	W_BlueGain		= gStp_CscAdj->W_BlueGain << 4;
	//
	// Convert SDWORD (Signed 8.24) to SWORD (Signed 4.12) fixed point format (0x01.000000 equal 0x1.000)
	// By shifting right 12 bits. Then multiply by RGB gains for RGB gain adjustment in YUV domain
	//
	sDwa_Matrix[0] = (SDWORD) W_GreenGain * ( (SDWORD) sDwa_Matrix[0]  >> 12);
	sDwa_Matrix[1] = (SDWORD) W_GreenGain * ( (SDWORD) sDwa_Matrix[1]  >> 12);
	sDwa_Matrix[2] = (SDWORD) W_GreenGain * ( (SDWORD) sDwa_Matrix[2]  >> 12);
	sDwa_Matrix[3] = (SDWORD) W_BlueGain * ( (SDWORD) sDwa_Matrix[3]  >> 12);
	sDwa_Matrix[4] = (SDWORD) W_BlueGain * ( (SDWORD) sDwa_Matrix[4]  >> 12);
	sDwa_Matrix[5] = (SDWORD) W_BlueGain * ( (SDWORD) sDwa_Matrix[5]  >> 12);
	sDwa_Matrix[6] = (SDWORD) W_RedGain * ( (SDWORD) sDwa_Matrix[6]  >> 12);
	sDwa_Matrix[7] = (SDWORD) W_RedGain * ( (SDWORD) sDwa_Matrix[7]  >> 12);
	sDwa_Matrix[8] = (SDWORD) W_RedGain * ( (SDWORD) sDwa_Matrix[8]  >> 12);

	//
	// xvYCC SF
	//
	//
	// Convert SDWORD (Signed 8.24) to SWORD (Signed 4.12) fixed point format (0x01.000000 equal 0x1.000)
	// By shifting right 12 bits. Then multiply by SF in YUV domain
	//	
	if((stInput[gmvb_CscChannel].Flags & gmd_INPUT_xvYCC_601) ||
		(stInput[gmvb_CscChannel].Flags & gmd_INPUT_xvYCC_709))
	{
		sDwa_Matrix[0] = (SDWORD) W_SF * ( (SDWORD) sDwa_Matrix[0]  >> 12);
		sDwa_Matrix[1] = (SDWORD) W_SF * ( (SDWORD) sDwa_Matrix[1]  >> 12);
		sDwa_Matrix[2] = (SDWORD) W_SF * ( (SDWORD) sDwa_Matrix[2]  >> 12);
		sDwa_Matrix[3] = (SDWORD) W_SF * ( (SDWORD) sDwa_Matrix[3]  >> 12);
		sDwa_Matrix[4] = (SDWORD) W_SF * ( (SDWORD) sDwa_Matrix[4]  >> 12);
		sDwa_Matrix[5] = (SDWORD) W_SF * ( (SDWORD) sDwa_Matrix[5]  >> 12);
		sDwa_Matrix[6] = (SDWORD) W_SF * ( (SDWORD) sDwa_Matrix[6]  >> 12);
		sDwa_Matrix[7] = (SDWORD) W_SF * ( (SDWORD) sDwa_Matrix[7]  >> 12);
		sDwa_Matrix[8] = (SDWORD) W_SF * ( (SDWORD) sDwa_Matrix[8]  >> 12);	
	}
	
	//
	// YUV Gain adjusment may be desired by some customers
	//	
#ifdef YUV_GAIN_ADJ_IN_YUV_DOMAIN
	//
	// Convert WORD to (Signed 4.12) fixed point format (0x100 equal 0x1.000)
	// 1 sign bit, 3 integer bits and 12 fraction bits.
	// By shifting left 4 bits.	
	//
	W_YGain 	= gStp_CscAdj->W_YGain << 4;
	W_UGain 	= gStp_CscAdj->W_UGain << 4;
	W_VGain		= gStp_CscAdj->W_VGain << 4;
	//
	// Convert SDWORD (Signed 8.24) to SWORD (Signed 4.12) fixed point format (0x01.000000 equal 0x1.000)
	// By shifting right 12 bits. Then multiply by YUV gains for YUV gain adjustment in YUV domain.
	//
	sDwa_Matrix[0] = (SDWORD) W_YGain* ( (SDWORD) sDwa_Matrix[0]  >> 12);
	sDwa_Matrix[1] = (SDWORD) W_UGain * ( (SDWORD) sDwa_Matrix[1]  >> 12);
	sDwa_Matrix[2] = (SDWORD) W_VGain * ( (SDWORD) sDwa_Matrix[2]  >> 12);
	sDwa_Matrix[3] = (SDWORD) W_YGain * ( (SDWORD) sDwa_Matrix[3]  >> 12);
	sDwa_Matrix[4] = (SDWORD) W_UGain * ( (SDWORD) sDwa_Matrix[4]  >> 12);
	sDwa_Matrix[5] = (SDWORD) W_VGain * ( (SDWORD) sDwa_Matrix[5]  >> 12);
	sDwa_Matrix[6] = (SDWORD) W_YGain * ( (SDWORD) sDwa_Matrix[6]  >> 12);
	sDwa_Matrix[7] = (SDWORD) W_UGain * ( (SDWORD) sDwa_Matrix[7]  >> 12);
	sDwa_Matrix[8] = (SDWORD) W_VGain * ( (SDWORD) sDwa_Matrix[8]  >> 12);
#endif
	//
	// Choose to update Main or Pip coefficients
	// 
	if (gmvb_CscChannel == CH_A)					
		W_Addr = MAIN_CSC_BLOCK_START;			// use MAIN coefficients
	else
		W_Addr = PIP_CSC_BLOCK_START;			// use PIP coefficients

	#if(FEATURE_VWD == ENABLE)
	if(ChangeByVWD==1)
		W_Addr = Second_CSC_BLOCK_START;
	#endif
	//
	// Write the coefficients
	// 
	for (W_i = 0; W_i < 9; W_i++)
	{
		//
		// Convert SDWORD (Signed 8.24) to to (Signed 2.8) since this is what the HW
		// expects. Fixed point format (0x1.000 equal 0x1.00). Achieve by shifting right 
		// 16 bits.
		//			
		// truncate to 11-bit
		RSHFT (sDwa_Matrix[W_i], 16); 
		TRUNC ( (SWORD) sDwa_Matrix[W_i], -0x800, 0x7FF ); 	// Truncate cell value to HW's expected value.		
		gm_WriteRegWord (W_Addr + CSC_YUV_COEFF[W_i], (SWORD) sDwa_Matrix[W_i]);
   }

   // Need to program Input offset.
   if (Input_YUV_Range == 1) // Full Range
   //if (Input_YUV_Range == 0) // Limited Range
   {
      gm_WriteRegWord (W_Addr - 12, 0x0);
      gm_WriteRegWord (W_Addr - 8, 0x6000);
      gm_WriteRegWord (W_Addr - 4, 0x6000);
   }
   else
   {
	gm_WriteRegWord (W_Addr - 12, 0x7C00);
	gm_WriteRegWord (W_Addr - 8, 0x6000);
	gm_WriteRegWord (W_Addr - 4, 0x6000);
}
}
//*************************************************  END  **********************
