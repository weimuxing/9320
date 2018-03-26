/*
   $Workfile:   osd_display.c  $
   $Revision: 1.73 $
   $Date: 2013/05/29 04:33:00 $
*/
#define __OSD_DISPLAY_C__
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
// MODULE:        osd_display.c
//
// USAGE:      This module contains standard osd adjusters for
//            display control (e.g. brightness, color temperature, video)
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "system\all.h"
#include "StdLibInc\gm_Register.h"
#include "mem.h"

#if FEATURE_CEC == ENABLE
#include "CEC\CecHandler.h"
#endif

#if (FEATURE_ACR == ENABLE)
#include "system\ACR_Handler.h"
#endif


#if (FEATURE_PIXEL_CRUNCH == ENABLE)
//#include "stdlibinc\PIXCRMain.h"
#endif

#ifdef DELL_U2410F
ValueScaleConvertType vConverT;//Lvjie 20081028 moved from 2709
#endif
//******************************************************************************
//  E X T E R N S
//******************************************************************************

extern ROM SWORD  gmc_CSCCoeff [2][9];

extern PortSchemeDependentUserPrefType ROM SchemeFactoryDefaultInROM[NUM_IMAGE_SCHEMES]; // PSD

#ifdef ST_4K2K_93xx_BOARD //Add by ChengLin 20130322
extern BOOL IgnoreInterChipCmd;
#endif

//******************************************************************************
//  D E F I N E S
//******************************************************************************

//
// Debug print message
//
//#define _DEBUG
#ifdef _DEBUG
#define  msg(a)                  gm_Print(a,0)
#define  msg_i(a,b)               gm_Print(a,b)
#else
#define  msg(a)
#define  msg_i(a,b)
#endif

//
// Color Temperature adjustment parameters
//
typedef struct
{
   BYTE   B_Red;                  // red         (RGB output)
   BYTE   B_Green;               // green      (RGB output)
   BYTE   B_Blue;                  // blue         (RGB output)
} ST_RGB_ADJUST;

#define ODP_CSC_BLOCK_START GAMMA_LUT_MATRIX_MULT_COEF11

#define COEF_11		0x0000
#define COEF_12	 	0x0002
#define COEF_13	 	0x0004
#define COEF_21	 	0x0006
#define COEF_22 		0x0008
#define COEF_23 		0x000A
#define COEF_31	 	0x000C
#define COEF_32 		0x000E
#define COEF_33 		0x0010
#define OFFSET1	 	0x0012
#define OFFSET2 		0x0014
#define OFFSET3 		0x0016


//******************************************************************************
//  C O N S T A N T
//******************************************************************************
/*
ST_RGB_ADJUST St_ColorTempConst[MAX_COLOR_TEMP] =
{
   { MAX_ADJUST_VALUE, MAX_ADJUST_VALUE, MAX_ADJUST_VALUE },   // RGB at No color temperature
   {253, 201, 157},         // RGB at 4200K
   {244, 217, 172},         // RGB at 5000K
   {248, 237, 210},         // RGB at 5400K
   {251, 237, 227},         // RGB at 6500K
   {234, 255, 241},         // RGB at 7500K
   {224, 228, 251},         // RGB at 9300K
   {MAX_ADJUST_VALUE, MAX_ADJUST_VALUE, MAX_ADJUST_VALUE}      // User defined
};
*/

//
// 3x3 YUV to RGB adjustment coefficients
//
ROM SWORD  LinearCSCCoeff [2][9] =      // Linear expansion
{
   {                           // applies to: improper SD to RGB output
      0x1000, -0x0550, -0x0B1F,      // {1.000, -0.332, -0.695}   [Row] x [Coloum]
      0x1000,  0x1BC0,  0x0000,      // {1.000,   1.734,   0.000}   [G, B, R] x [Y, Cb, Cr]
      0x1000,  0x0000,  0x15F0       // {1.000,   0.000,   1.371}
   },
   {                           // applies to: Improper HD to RGB output
      0x12A0, -0x0370, -0x0890,      // {1.164, -0.215 -0.535}     [Row] x [Coloum]
      0x12A0,  0x21D0,  0x0000,      // {1.164,   2.113,   0.000}    [G, B, R] x [Y, Pb, Pr]
      0x12A0,  0x0000,  0x1CB0      // {1.164,   0.000,   1.793}
   }
};
ROM SWORD  PartialCSCCoeff [2][9] =      // Partial expansion
{
   {                           // applies to: improper SD to RGB output
      0x111F, -0x05BE, -0x0BF0,      // {1.070, -0.359, -0.746}   [Row] x [Coloum]
      0x111F,  0x1DAE,  0x0000,      // {1.070,   1.855,   0.000}   [G, B, R] x [Y, Cb, Cr]
      0x111F,  0x0000,  0x1781       // {1.070,   0.000,   1.469}
   },
   {                           // applies to: Improper HD to RGB output
      0x12A0, -0x0370, -0x0890,      // {1.164, -0.215 -0.535}     [Row] x [Coloum]
      0x12A0,  0x21D0,  0x0000,      // {1.164,   2.113,   0.000}    [G, B, R] x [Y, Pb, Pr]
      0x12A0,  0x0000,  0x1CB0      // {1.164,   0.000,   1.793}
   }
};
ROM SWORD  UnityCSCCoeff[9] =         // Unity matrix
{
   0x1000,   0x0000,   0x0000,            //{1.000, 0.000, 0.000}
   0x0000,   0x1000,   0x0000,            //{0.000, 1.000, 0.000}
   0x0000,   0x0000,   0x1000,            //{0.000, 0.000, 1.000}
};

//******************************************************************************
//  E X T E R N S
//******************************************************************************

#if 0 // (FEATURE_ACM2 == ENABLE)
extern gmt_AcmColorStruct St_AcmColorAdjust;
#endif


//******************************************************************************
// S T A T I C   F U N C T I O N S
//******************************************************************************

//******************************************************************************
//  C O D E
//******************************************************************************

//******************************************************************************
//
// FUNCTION       : void SelectCscMatrix(CSC_MATRIX Mode)
//
// USAGE          : Called by OSD WB project to select the customer choice
//                  of 3x3 matrix
//
// INPUT          : Mode
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void SelectCscMatrix(CSC_MATRIX Mode)
{
   if (Mode == CSC_FULL_EXPANSION)
   {
      gm_CscUpdateMatrix((SWORD ROM *)&gmc_CSCCoeff[0],
                         (SWORD ROM *)&gmc_CSCCoeff[1],
                         (SWORD ROM *)&gmc_CSCCoeff[2],
                         (SWORD ROM *)&gmc_CSCCoeff[3]);
   }
   else if (Mode == CSC_LINEAR_EXPANSION)
   {
      gm_CscUpdateMatrix((SWORD ROM *)&LinearCSCCoeff[0],
                         (SWORD ROM *)&LinearCSCCoeff[1],
                         (SWORD ROM *)&LinearCSCCoeff[2],
                         (SWORD ROM *)&LinearCSCCoeff[3]);
   }
   else if (Mode == CSC_PARTIAL_EXPANSION)
   {
      gm_CscUpdateMatrix((SWORD ROM *)&PartialCSCCoeff[0],
                         (SWORD ROM *)&PartialCSCCoeff[1],
                         (SWORD ROM *)&PartialCSCCoeff[2],
                         (SWORD ROM *)&PartialCSCCoeff[3]);
   }
   else if (Mode == CSC_UNITY_MATRIX)
   {
      gm_CscUpdateMatrix((SWORD ROM *)UnityCSCCoeff,
                         (SWORD ROM *)UnityCSCCoeff,
                         (SWORD ROM *)UnityCSCCoeff,
                         (SWORD ROM *)UnityCSCCoeff);
   }
}

//******************************************************************************
// FUNCTION       : void AdjustPanelBacklight(void)
// USAGE          : Adjust panel backlight in percentage
// INPUT          : B_Percent (0% ~ 100%)
// OUTPUT         : None
// GLOBALS        : None
// USED_REGS      : None
//******************************************************************************
extern BOOL SetBrightnessPWM(SBYTE Brightness);
void AdjustPanelBacklight(BYTE B_Percent)
{
   WORD W_PwmVal;

   //gm_Print("   Backlight = %d",B_Percent);
   W_PwmVal = (WORD)((DWORD)(MAX_BACKLIGHT_PWM - MIN_BACKLIGHT_PWM) * B_Percent / 100 + MIN_BACKLIGHT_PWM);

   SetBrightnessPWM(W_PwmVal);
}

#ifdef ADJUSTER_BRIGHTNESS_DECLARED
//******************************************************************************
//
// FUNCTION       : void AdjustBrightness(void)
//
// USAGE          : The user preference variable UserPrefBrightness is copied to system library
//                  variable St_Graphic/videoCscAdjust.B_Brightness, then the system library
//                  function gm_UpdateInputCSC is invoked for adjusting brightness using
//                  color matrix in g_Channel. Adjust brightness of Main and Pipchannel
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : gmvb_MainChannel      (R)
//                  gmvb_PipChannel      (R)
//                  St_ColorAdjust      (R/W)
//                  UserPrefBrightness      (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AdjustBrightness(void)
{
   //
   // Condition is only called once when user is adjusting a preset image scheme. Values are
   // copied over from the table to the custom userpref
   //
#ifdef ST_4K2K_93xx_BOARD
#if ENABLE_SOC_IPC_BUSY_CHECK
	if(GetChipID_4K2K()==CHIPID_4K2K_R)
	{
	   /*ChengLin 20130124: New 4k2k board BUSY status check*/
	   if(!SocIPCIsSlaveLeftReady())
		   return;	
	}	
#endif	
#endif      

   
#if (PORT_SCHEME_DEPENDENT == PORT_DEPENDENT)
   if (UserPrefImageScheme != USER)
   {
      UserPrefImageScheme = USER;
      UserPrefMFMImageScheme = USER;
   }
#endif

   #if (defined(CMI_73XX_BOARD) || (defined(WISTRON_93XX_BOARD_30) && WST_P3_BAORD_30 == 0))
   if(UserPrefDCR == OFF)
   	    DCR_EnergyUse_Brightness = UserPrefBrightness;
   #endif

      #ifdef ST_4K2K_93xx_BOARD
      if(GetChipID_4K2K()==CHIPID_4K2K_R)
      {
		   SOCIPC_SyncOSDAdjustHandler( SI_OSD_SYNC_BRIGHTNESS );
		}
   	#endif

   CscUpdateColors(gmvb_MainChannel);
   CscUpdateColors(gmvb_PipChannel);
}
#endif

#ifdef ADJUSTER_CONTRAST_DECLARED
//******************************************************************************
//
// FUNCTION       : void AdjustContrast(void)
//
// USAGE          : User preference variable UserPrefContrast is copied to
//                  system library variable St_Graphic/videoCscAdjust.W_Contrast,
//                  then the system library function gm_UpdateInputCSC is
//                  invoked for adjusting contrast level using color matrix in g_Channel.
//                  Adjust contrast of Graphics and video channel
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : St_ColorAdjust      (R/W)
//                  gmvb_MainChannel   (R)
//                  gmvb_PipChannel      (R)
//                  UserPrefContrast      (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AdjustContrast(void)
{
   //
   // Condition is only called once when user is adjusting a preset image scheme. Values are
   // copied over from the table to the custom userpref
   //
#ifdef ST_4K2K_93xx_BOARD
#if ENABLE_SOC_IPC_BUSY_CHECK
	if(GetChipID_4K2K()==CHIPID_4K2K_R)
	{
		/*ChengLin 20130124: New 4k2k board BUSY status check*/
		if(!SocIPCIsSlaveLeftReady())
			return;
	}
#endif	
#endif      
   
#if (PORT_SCHEME_DEPENDENT == PORT_DEPENDENT)
   if (UserPrefImageScheme != USER)
   {
      UserPrefImageScheme = USER;
      UserPrefMFMImageScheme = USER;
   }
#endif

#ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()==CHIPID_4K2K_R)
   {
      SOCIPC_SyncOSDAdjustHandler( SI_OSD_SYNC_CONTRAST );
   }
#endif
	
   CscUpdateColors(gmvb_MainChannel);
   CscUpdateColors(gmvb_PipChannel);
}
#endif

#ifdef ADJUSTER_SATURATION_DECLARED
//******************************************************************************
//
// FUNCTION       : void AdjustSaturation(void)
//
// USAGE          : User preference variable UserPrefContrast is copied to
//                  system library variable St_Graphic/videoCscAdjust.W_Saturation,
//                  then the system library function gm_UpdateInputCSC is
//                  invoked for adjusting saturation level using color matrix in g_Channel.
//                  Adjust saturation of Graphics and video channel
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : St_ColorAdjust         (R/W)
//                  gmvb_MainChannel   (R)
//                  gmvb_PipChannel      (R)
//                  UserPrefSaturation      (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AdjustSaturation(void)
{
   //
   // Condition is only called once when user is adjusting a preset image scheme. Values are
   // copied over from the table to the custom userpref
   //
#if (PORT_SCHEME_DEPENDENT == PORT_DEPENDENT)
   if (UserPrefImageScheme != USER)
   {
      UserPrefImageScheme = USER;
      UserPrefMFMImageScheme = USER;
   }
#endif

   #if 0 //def ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()==CHIPID_4K2K_R)
   {
		BYTE TempControl[1] = {0};
		
		TempControl[1] = UserPrefSaturation;
		SocIPCSendCmdPacketParser(SOCIPC_DEVICEL_DST_ADDRESS, SICMD_RW_, TempControl);
	}
	#endif


   CscUpdateColors(gmvb_MainChannel);
   CscUpdateColors(gmvb_PipChannel);
}
#endif

#ifdef ADJUSTER_HUE_DECLARED
//******************************************************************************
//
// FUNCTION       : void AdjustHue(void)
//
// USAGE          : User preference variable UserPrefContrast is copied to
//                  system library variable St_Graphic/videoCscAdjust.sW_Hue,
//                  then the system library function gm_UpdateInputCSC is
//                  invoked for adjusting hue level using color matrix in g_Channel.
//                  Adjust hue of Graphics and video channel
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : St_ColorAdjust         (R/W)
//                  gmvb_MainChannel   (R)
//                  gmvb_PipChannel      (R)
//                  UserPrefHue         (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AdjustHue(void)
{
   //
   // Condition is only called once when user is adjusting a preset image scheme. Values are
   // copied over from the table to the custom userpref
   //
#if (PORT_SCHEME_DEPENDENT == PORT_DEPENDENT)
   if (UserPrefImageScheme != USER)
   {
      UserPrefImageScheme = USER;
      UserPrefMFMImageScheme = USER;
   }
#endif

   CscUpdateColors(gmvb_MainChannel);
   CscUpdateColors(gmvb_PipChannel);
}
#endif

#ifdef ADJUSTER_SHARPNESS_DECLARED
//******************************************************************************
//
// FUNCTION       : void AdjustSharpness(void)
//
// USAGE          : Adjust sharpness of video channel
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : UserPrefSharpness                           (R)
//                  :   UserPrefPipMode, UserPrefSharpnessPip (FLI8668)    (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AdjustSharpness(void)
{

#ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()==CHIPID_4K2K_R)
   {
#if ENABLE_SOC_IPC_BUSY_CHECK
	/*ChengLin 20130124: New 4k2k board BUSY status check*/
	if(!SocIPCIsSlaveLeftReady())
		return;
#endif      
      SOCIPC_SyncOSDAdjustHandler( SI_OSD_SYNC_SHARPNESS );
   }
#endif
#if 1		//Port dependent
		//120613 Kevin Chen for avoid PxP On /Off change main sharpness
		UserPrefSharpnessRGB = UserPrefSharpness = UserPrefPortSharpness;
	
	//Neil 20120526* according to FT itme 87: sharpness only adjust main channel
	if (gm_TimingIsStable(gmvb_MainChannel) == TRUE)
		gm_SetScalerSharpness(gmvb_MainChannel, UserPrefSharpness);

#else
#if   (FEATURE_LOAD_RGB_COEFF == DISABLE)
   if (gm_TimingIsStable(CH_A) == TRUE)
      	gm_SetScalerSharpness(CH_A, UserPrefSharpness);

#elif (FEATURE_LOAD_RGB_COEFF == ENABLE)
   if (IsSideBySidePipMode(UserPrefPipMode) == TRUE )
   {
      if ( B_FocusBorder == gmvb_MainChannel )
      {
		#ifndef DELL_U2410F	// Removed by ST_hansPENG 081121
         if( gm_GetChannelInputAttr(gmvb_MainChannel, gmd_COLOR_SPACE) != gmd_RGB )
		#endif
         {
            if (gm_TimingIsStable(gmvb_MainChannel) == TRUE)
               gm_SetScalerSharpness(gmvb_MainChannel, UserPrefSharpness);
         }
      }
      else
      {
		#ifndef DELL_U2410F	// Removed by ST_hansPENG 081121
         if( gm_GetChannelInputAttr(gmvb_PipChannel, gmd_COLOR_SPACE) != gmd_RGB )
		#endif
         {
            if (gm_TimingIsStable(gmvb_PipChannel) == TRUE)
               gm_SetScalerSharpness(gmvb_PipChannel, UserPrefSharpnessPip);
         }
      }
   }
   else
   {
	#ifndef DELL_U2410F	// Removed by ST_hansPENG 081121
      if( gm_GetChannelInputAttr(gmvb_MainChannel, gmd_COLOR_SPACE) != gmd_RGB )
	#endif
      {
         if (gm_TimingIsStable(gmvb_MainChannel) == TRUE)
            gm_SetScalerSharpness(gmvb_MainChannel, UserPrefSharpness);
      }
   }
#endif
#endif//#ifdef DELL_U2410F
}
#endif

#ifdef ADJUSTER_SHARPNESSRGB_DECLARED
//******************************************************************************
//
// FUNCTION       : void AdjustSharpnessRGB(void)
//
// USAGE          : Adjust sharpness of Graphics channel
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : UserPrefSharpnessRGB,    (R)
//                  :   UserPrefPipMode, UserPrefSharpnessRGBpip (FLI8668)    (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AdjustSharpnessRGB(void)
{
#ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()==CHIPID_4K2K_R)
   {
#if ENABLE_SOC_IPC_BUSY_CHECK
	/*ChengLin 20130124: New 4k2k board BUSY status check*/
	if(!SocIPCIsSlaveLeftReady())
		return;
#endif         
   	SOCIPC_SyncOSDAdjustHandler( SI_OSD_SYNC_SHARPNESS_RGB );
  	}
#endif

#if   (FEATURE_LOAD_RGB_COEFF == ENABLE)
   if (IsSideBySidePipMode(UserPrefPipMode) == TRUE )
   {
      if ( B_FocusBorder == gmvb_MainChannel )
      {
         if ( gm_GetChannelInputAttr(gmvb_MainChannel, gmd_COLOR_SPACE) == gmd_RGB )
         {
            if (gm_TimingIsStable(gmvb_MainChannel) == TRUE)
               gm_SetScalerSharpness(gmvb_MainChannel, UserPrefSharpnessRGB);
         }
      }
      else
      {
         if ( gm_GetChannelInputAttr(gmvb_PipChannel, gmd_COLOR_SPACE) == gmd_RGB )
         {
            if (gm_TimingIsStable(gmvb_PipChannel) == TRUE)
               gm_SetScalerSharpness(gmvb_PipChannel, UserPrefSharpnessRGBpip);
         }
      }
   }
   else
   {
      if ( gm_GetChannelInputAttr(gmvb_MainChannel, gmd_COLOR_SPACE) == gmd_RGB )
      {
         if (gm_TimingIsStable(gmvb_MainChannel) == TRUE)
            gm_SetScalerSharpness(gmvb_MainChannel, UserPrefSharpnessRGB);
      }
   }
#endif
}
#endif


#ifdef Action_Adjust3DNR_WB_Used
//******************************************************************************
//
// FUNCTION       : void Adjust3DNR(void)
//
// USAGE          : Adjust 3D NR
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : UserPrefThreeDNR      (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void Adjust3DNR(void)
{
   if (UserPrefThreeDNR != DISABLE)
   {
      gm_VideoProcessingEnable(VIDEO_TNR);
      gm_VideoProcessingAdjustment(VIDEO_TNR, UserPrefThreeDNR);
   }
   else
   {
      gm_VideoProcessingDisable(VIDEO_TNR);
   }
}
#endif

#ifdef Action_AdjustMPEGNR_WB_Used
//******************************************************************************
//
// FUNCTION       : void AdjustMPEGNR(void)
//
// USAGE          : Adjust MPEG NR
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : B_ModeSetupReq      (R/W)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AdjustMPEGNR(void)
{
   if (UserPrefMPEGNR != DISABLE)
   {
      gm_VideoProcessingEnable(VIDEO_MNR);
      gm_VideoProcessingAdjustment(VIDEO_MNR, UserPrefMPEGNR);
   }
   else
   {
      gm_VideoProcessingDisable(VIDEO_MNR);
   }
}
#endif

#ifdef ADJUSTER_USERREDGAIN_DECLARED
//******************************************************************************
//
// FUNCTION       : void AdjustRedGain(void)
//
// USAGE          : User preference variable UserPrefRedGain is copied to
//                  system library variable St_Graphic/videoCscAdjust.B_RedGain,
//                  then the system library function gm_CscUpdateColors is
//                  invoked for adjusting red level using color matrix in g_Channel.
//                  Adjust hue of Graphics and video channel
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : St_ColorAdjust      (R/W)
//                  UserPrefUserRedGain   (R)
//                  gmvb_MainChannel   (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AdjustRedGain(void)
{
   //
   // Condition is only called once when user is adjusting a preset image scheme. Values are
   // copied over from the table to the custom userpref
   //
#ifdef ST_4K2K_93xx_BOARD
#if ENABLE_SOC_IPC_BUSY_CHECK
	if(GetChipID_4K2K()==CHIPID_4K2K_R)
	{
	   /*ChengLin 20130124: New 4k2k board BUSY status check*/
	   if(!SocIPCIsSlaveLeftReady())
		   return;
	}
#endif   
#endif

//Added by ChengLin 20130110
#ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()==CHIPID_4K2K_R)
   {
   	SOCIPC_SyncOSDAdjustHandler( SI_OSD_SYNC_CT_USER_RGAIN );
  	}
#endif

   
#if (PORT_SCHEME_DEPENDENT == PORT_DEPENDENT)
   if (UserPrefImageScheme != USER)
   {
      UserPrefImageScheme = USER;
      UserPrefMFMImageScheme = USER;
   }
#endif

/*ChengLin 20130129: Don't to call 2 times SOCIPC Commands for OSD Sync*/
#ifdef ST_4K2K_93xx_BOARD
	CscUpdateColors(gmvb_MainChannel);	 
#else
	AdjustColorTemp();
#endif

}
#endif

#ifdef ADJUSTER_USERGREENGAIN_DECLARED
//******************************************************************************
//
// FUNCTION       : void AdjustGreenGain(void)
//
// USAGE          : User preference variable UserPrefGreenGain is copied to
//                  system library variable St_Graphic/videoCscAdjust.B_GreenGain,
//                  then the system library function gm_CscUpdateColors is
//                  invoked for adjusting green level using color matrix in g_Channel.
//                  Adjust hue of Graphics and video channel
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : St_ColorAdjust         (R/W)
//                  UserPrefUserGreenGain      (R)
//                  gmvb_MainChannel   (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AdjustGreenGain(void)
{
   //
   // Condition is only called once when user is adjusting a preset image scheme. Values are
   // copied over from the table to the custom userpref
   //
#ifdef ST_4K2K_93xx_BOARD
#if ENABLE_SOC_IPC_BUSY_CHECK
	if(GetChipID_4K2K()==CHIPID_4K2K_R)
	{
		/*ChengLin 20130124: New 4k2k board BUSY status check*/
		if(!SocIPCIsSlaveLeftReady())
			return;
	}
#endif   
#endif

//Added by ChengLin 20130110
#ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()==CHIPID_4K2K_R)
   {
   	SOCIPC_SyncOSDAdjustHandler( SI_OSD_SYNC_CT_USER_GGAIN );
  	}
#endif

   
#if (PORT_SCHEME_DEPENDENT == PORT_DEPENDENT)
   if (UserPrefImageScheme != USER)
   {
      UserPrefImageScheme = USER;
      UserPrefMFMImageScheme = USER;
   }
#endif

/*ChengLin 20130129: Don't to call 2 times SOCIPC Commands for OSD Sync*/
#ifdef ST_4K2K_93xx_BOARD
	CscUpdateColors(gmvb_MainChannel);	
#else
	AdjustColorTemp();
#endif
}
#endif

#ifdef ADJUSTER_USERBLUEGAIN_DECLARED
//******************************************************************************
//
// FUNCTION       : void AdjustBlueGain(void)
//
// USAGE          : User preference variable UserPrefBlueGain is copied to
//                  system library variable St_Graphic/videoCscAdjust.B_BlueGain,
//                  then the system library function gm_CscUpdateColors is
//                  invoked for adjusting blue level using color matrix in g_Channel.
//                  Adjust hue of Graphics and video channel
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : St_ColorAdjust         (R/W)
//                  UserPrefUserBlueGain      (R)
//                  gmvb_MainChannel   (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AdjustBlueGain(void)
{
   //
   // Condition is only called once when user is adjusting a preset image scheme. Values are
   // copied over from the table to the custom userpref
   //
#ifdef ST_4K2K_93xx_BOARD
#if ENABLE_SOC_IPC_BUSY_CHECK
	 if(GetChipID_4K2K()==CHIPID_4K2K_R)
	 {
		/*ChengLin 20130124: New 4k2k board BUSY status check*/
		if(!SocIPCIsSlaveLeftReady())
			return;
	 }
#endif
#endif         

   //Added by ChengLin 20130110
#ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()==CHIPID_4K2K_R)
   {
   	SOCIPC_SyncOSDAdjustHandler( SI_OSD_SYNC_CT_USER_BGAIN );
  	}
#endif	

#if (PORT_SCHEME_DEPENDENT == PORT_DEPENDENT)
   if (UserPrefImageScheme != USER)
   {
      UserPrefImageScheme = USER;
      UserPrefMFMImageScheme = USER;
   }
#endif

/*ChengLin 20130129: Don't to call 2 times SOCIPC Commands for OSD Sync*/
#ifdef ST_4K2K_93xx_BOARD
	CscUpdateColors(gmvb_MainChannel);  
#else
	AdjustColorTemp();
#endif
}
#endif

#ifdef ADJUSTER_USERREDOFF_DECLARED
//******************************************************************************
//
// FUNCTION       : void AdjustRedOff(void)
//
// USAGE          : User preference variable UserPrefRefOff is copied to
//                  system library variable St_Graphic/videoCscAdjust.sB_RedOff,
//                  then the system library function gm_CscUpdateColors is
//                  invoked for adjusting red offset value of the 3x3 matrix.
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : St_ColorAdjust      (R/W)
//                  UserPrefUserRedOff   (R)
//                  gmvb_MainChannel   (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AdjustRedOff(void)
{
   //
   // Condition is only called once when user is adjusting a preset image scheme. Values are
   // copied over from the table to the custom userpref
   //
#ifdef ST_4K2K_93xx_BOARD
#if ENABLE_SOC_IPC_BUSY_CHECK
	   if(GetChipID_4K2K()==CHIPID_4K2K_R)
	   {
		  /*ChengLin 20130124: New 4k2k board BUSY status check*/
		  if(!SocIPCIsSlaveLeftReady())
			  return;
	   }
#endif      
#endif         

//Added by ChengLin 20130110
#ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()==CHIPID_4K2K_R)
   {
   	SOCIPC_SyncOSDAdjustHandler( SI_OSD_SYNC_CT_USER_ROFFSET );
  	}
#endif

   
#if (PORT_SCHEME_DEPENDENT == PORT_DEPENDENT)
   if (UserPrefImageScheme != USER)
   {
      UserPrefImageScheme = USER;
      UserPrefMFMImageScheme = USER;
   }
#endif

/*ChengLin 20130129: Don't to call 2 times SOCIPC Commands for OSD Sync*/
#ifdef ST_4K2K_93xx_BOARD
	CscUpdateColors(gmvb_MainChannel);  
#else
	AdjustColorTemp();
#endif
}
#endif

#ifdef ADJUSTER_USERGREENOFF_DECLARED
//******************************************************************************
//
// FUNCTION       : void AdjustGreenOff(void)
//
// USAGE          : User preference variable UserPrefContrast is copied to
//                  system library variable St_Graphic/videoCscAdjust.B_GreenOff,
//                  then the system library function gm_CscUpdateColors is
//                  invoked for adjusting green offset value of the 3x3 matrix.
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : St_ColorAdjust         (R/W)
//                  UserPrefUserGreenOff      (R)
//                  gmvb_MainChannel   (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AdjustGreenOff(void)
{
   //
   // Condition is only called once when user is adjusting a preset image scheme. Values are
   // copied over from the table to the custom userpref
   //
#ifdef ST_4K2K_93xx_BOARD
#if ENABLE_SOC_IPC_BUSY_CHECK
	   if(GetChipID_4K2K()==CHIPID_4K2K_R)
	   {
		  /*ChengLin 20130124: New 4k2k board BUSY status check*/
		  if(!SocIPCIsSlaveLeftReady())
			  return;
	   }
#endif
#endif         

//Added by ChengLin 20130110
#ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()==CHIPID_4K2K_R)
   {
   	SOCIPC_SyncOSDAdjustHandler( SI_OSD_SYNC_CT_USER_GOFFSET );
  	}
#endif

#if (PORT_SCHEME_DEPENDENT == PORT_DEPENDENT)
   if (UserPrefImageScheme != USER)
   {
      UserPrefImageScheme = USER;
      UserPrefMFMImageScheme = USER;
   }
#endif

/*ChengLin 20130129: Don't to call 2 times SOCIPC Commands for OSD Sync*/
#ifdef ST_4K2K_93xx_BOARD
	CscUpdateColors(gmvb_MainChannel);  
#else
	AdjustColorTemp();
#endif
}
#endif

#ifdef ADJUSTER_USERBLUEOFF_DECLARED
//******************************************************************************
//
// FUNCTION       : void AdjustBlueOff(void)
//
// USAGE          : User preference variable UserPrefContrast is copied to
//                  system library variable St_Graphic/videoCscAdjust.B_BlueOff,
//                  then the system library function gm_CscUpdateColors is
//                  invoked for adjusting blue offset value of the 3x3 matrix.
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : St_ColorAdjust         (R/W)
//                  UserPrefUserBlueOff      (R)
//                  gmvb_MainChannel   (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AdjustBlueOff(void)
{
   //
   // Condition is only called once when user is adjusting a preset image scheme. Values are
   // copied over from the table to the custom userpref
   //
#ifdef ST_4K2K_93xx_BOARD
#if ENABLE_SOC_IPC_BUSY_CHECK
	   if(GetChipID_4K2K()==CHIPID_4K2K_R)
	   {
		  /*ChengLin 20130124: New 4k2k board BUSY status check*/
		  if(!SocIPCIsSlaveLeftReady())
			  return;
	   }
#endif      
#endif         

//Added by ChengLin 20130110
#ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()==CHIPID_4K2K_R)
   {
   	SOCIPC_SyncOSDAdjustHandler( SI_OSD_SYNC_CT_USER_BOFFSET );
  	}
#endif

#if (PORT_SCHEME_DEPENDENT == PORT_DEPENDENT)
   if (UserPrefImageScheme != USER)
   {
      UserPrefImageScheme = USER;
      UserPrefMFMImageScheme = USER;
   }
#endif

/*ChengLin 20130129: Don't to call 2 times SOCIPC Commands for OSD Sync*/
#ifdef ST_4K2K_93xx_BOARD
	CscUpdateColors(gmvb_MainChannel);  
#else
	AdjustColorTemp();
#endif
}
#endif

//******************************************************************************
//
// FUNCTION       : BOOL IsColorTempAvailable(BYTE ColorTempRequest)
//
// USAGE          : Used by the OSD to determine which Color Temperatures are available for the
//                        current gamma table applied.
//
// INPUT          : BYTE ColorTempRequest - OSD asking if this color temp is available
//
// OUTPUT         : TRUE/FALSE
//
// GLOBALS        :
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BOOL IsColorTempAvailable(BYTE ColorTempRequest)
{
   BOOL found = FALSE;

   // 20081016 #1, for xvYcc remapping Gamma table and 3x3 table.
#if 0 //ndef RD1_8028_BOARD
   gmt_GammaDescr ROM * ROM *ptr_GammaDescr = PanelGammaLUT;

   //Look to see if this certain color temp is found for the current gamma table until a terminator is reached
   while ((*ptr_GammaDescr) != NULL_PTR)
   {
      if (((*ptr_GammaDescr)->B_ColorSpace == UserPrefColorSpace) &&
            ((*ptr_GammaDescr)->B_ColorTemp== ColorTempRequest))
      {
         found = TRUE;
         break;
      }

      ptr_GammaDescr++;
   }
#else
   ColorTempRequest = ColorTempRequest;
   found = TRUE;
#endif

   return found;

}

static void CheckSignNum(SDWORD *InMatx)
{
   BYTE i;

   for (i=0;i<9;i++)
   {
      if (InMatx[i] & BIT13)	/// nagtive number
      {
         InMatx[i] |= 0xFFFFE000;
      }
   }
}

// 20081020 #1, for XvYcc remapping
static void __near matrix_MxM(SDWORD *A, SDWORD *B)
{
   BYTE row,col;
   SDWORD M[3][3];
   SDWORD a,b,c;

   for (row=0;row<3;row++)
   {
      for (col=0;col<3;col++)
      {
         a = A[row * 3 + 0] * B[0 * 3 + col];
         b = A[row * 3 + 1] * B[1 * 3 + col];
         c = A[row * 3 + 2] * B[2 * 3 + col];
         M[row][col] = (a + b + c) >> 10;
         //M[row][col] = (a + b + c) >> 8;
      }
   }
   memcpy(A,M,sizeof(M));
}

#ifdef DELL_U2410F

#ifdef USE_GAMMMA_TABLE_FROM_NVRAM
#define C_Gamma_PostLUT_Component   BIT0
#define C_Gamma_PreLUT_Component    BIT1
#define C_Gamma_Matrix_Component    BIT2
#define C_Gamma_PostSteep_Component BIT3
#define C_Gamma_PreSteep_Component  BIT4

void AdjustColorSpace_NVRAM(void)//For DELL firmware
{
   //static BYTE B_PreColorSpace = 0xFF;
   //static BYTE B_PreColorTemp = 0xFF;

	BYTE i;
	BYTE C_Selected_Profile, C_Component;
	DWORD DW_LUTAddr;
	WORD DW_NvramAddr;
	BYTE Buf[30];

	msg_i("1### %ld", gm_GetSystemTime());

	C_Selected_Profile = Get_NVRAMTableIndex();
	/*if(UserPrefPresetColorSpace == CSPACE_AdobeRGB)
		C_Selected_Profile=1;
	else if(UserPrefPresetColorSpace == CSPACE_sRGB)
		C_Selected_Profile=0;
	else if(UserPrefPresetColorSpace == CSPACE_CAL1)
		C_Selected_Profile=2;
	else
		C_Selected_Profile=3;	//CAL2*/

	DW_LUTAddr = GAMMA_LUT_MATRIX_MULT_COEF11; // GAMMA_LUT1_CTRL;
	DW_NvramAddr = C_GAMMA_COMPONENT_SAVED_START_OFFSET + (0x2000*C_Selected_Profile);
	msg_i("C_GAMMA_COMPONENT_SAVED_START_OFFSET=%d",NVRam_ReadByte(DW_NvramAddr));
	//if((NVRam_ReadByte(DW_NvramAddr)&BIT0) && (NVRam_ReadByte(DW_NvramAddr) != 0xFF))
	if(NVRAM_TableCheck(NVRAM_Table_Gamma ,C_Selected_Profile))
	{
		C_Component =  NVRam_ReadByte(DW_NvramAddr);
		
		if(C_Component & C_Gamma_Matrix_Component)
		{
		DW_NvramAddr = C_GAMMA_REGISTER_START_OFFSET + (0x2000*C_Selected_Profile);

		NVRam_ReadBuffer(DW_NvramAddr, Buf, 30);

		//for(i = 0; i < 41; i++) // 41 registers
		for(i = 0; i < 15; i++) // 15 registers
		{					  
			gm_WriteRegWord(DW_LUTAddr,(Buf[i*2]|Buf[i*2+1]<<8));
	   		DW_LUTAddr += 2;
		}
	
		// GAMMA_MATRIX_CTRL settings:
		// 00: Disable
		// 01: Inside DISP_LUT_CONTROL
		// 10: Outside DISP_LUT_CONTROL
		// 11: All pixels
		gm_SetRegBitsDWord(GAMMA_LUT2_CTRL, BIT12);
		}
		   

		   
		if((C_Component & C_Gamma_PostLUT_Component) ||(C_Component & C_Gamma_PreLUT_Component))
			AdjustGammaCorrection_NVRAM();

		#if 1//120801 Edward move gamma enable here to avoid image gamma apply delay from read NVRAM
			DW_NvramAddr = C_PRE_LUT_CONFIG_REG_START_OFFSET + (0x2000*C_Selected_Profile);
			gm_WriteRegWord(GAMMA_LUT1_CTRL, NVRam_ReadWORD(DW_NvramAddr));
			DW_NvramAddr = C_POST_LUT_CONFIG_REG_START_OFFSET+ (0x2000*C_Selected_Profile);
	   		gm_WriteRegWord(GAMMA_LUT2_CTRL, NVRam_ReadWORD(DW_NvramAddr));

			#if 1//120928 Edward to avoid garbage
			if(gm_WaitDisplayBlanking_CH(CH_A)) //KevinL 20120923 force update on blanking period immediatelly 
				gm_ForceUpdate(gmvb_MainChannel, OUTPUT_CLOCK);	
			else
				gm_SyncUpdateWait(gmvb_MainChannel, OUTPUT_CLOCK);
			#else
			gm_WaitDisplayBlanking_CH(CH_A);		//Neil120801+ for update in blanking to reduce garbage
			gm_SyncUpdateWait(gmvb_MainChannel, OUTPUT_CLOCK);
			#endif
	   	#endif
		
	}
	else
	{
			msg_i("~~NO Data in NVRAM~~",0);
			gm_WriteRegWord(GAMMA_LUT1_CTRL, 0);
			gm_WriteRegWord(GAMMA_LUT2_CTRL, 0);	
			
		#if 0//def Qisda_93XX_BOARD	yvonne 20120828 follow dell spec, no defaut in CAL1/CAL2
		msg_i("11  QQQ gm_GetSystemTime=%d",gm_GetSystemTime());
		if((UserPrefPresetColorSpace == CSPACE_CAL1)||(UserPrefPresetColorSpace == CSPACE_CAL2))
		{
			AdjustGammaCorrection_ColorSpace();
			gm_WaitDisplayBlanking(CH_A);		//Neil120801+ for update in blanking to reduce garbage
			gm_SyncUpdateWait(gmvb_MainChannel, OUTPUT_CLOCK);
		}
		msg_i("22  QQQ gm_GetSystemTime=%d",gm_GetSystemTime());
		#endif
	}


	msg_i("2### %ld", gm_GetSystemTime());

	//120928 Edward to avoid garbage
	#if 1
	if(gm_WaitDisplayBlanking_CH(CH_A)) //KevinL 20120923 force update on blanking period immediatelly 
		gm_ForceUpdate(gmvb_MainChannel, OUTPUT_CLOCK);	
	else
		gm_SyncUpdateWait(gmvb_MainChannel, OUTPUT_CLOCK);
	#else
		gm_WaitDisplayBlanking(CH_A);		//Neil120801+ for update in blanking to reduce garbage
		gm_SyncUpdateWait(gmvb_MainChannel, OUTPUT_CLOCK);
	#endif
         
}
#endif // USE_GAMMA_TABLE_FROM_NVRAM


void AdjustColorSpace(void)//For DELL firmware
{
   //#if (NON_SCALING_OUTPUT == 0)
   #ifdef OUTPUT_MODE_CHANGE
   if(UserPrefOutputMode == OPM_Normal)
   #endif
{
   static BYTE B_PrePresetMode = 0xFF;
   //static BYTE B_PreColorTemp = 0xFF;

   gmt_GammaDescr ROM * ROM *ptr_GammaDescr = GetGammaTable();
   gm_Printf("AdjustColorSpace: UserPrefColorSpace = %d", UserPrefColorSpace);

   // ODP 3x3 matrix update for color space
   {
      WORD W_Addr;
      SDWORD rgbGain[3][3] = {{0x400,0,0}, {0,0x400,0}, {0,0,0x400}};
      SDWORD *ccMatrix;
      BOOL UpdateFalag;
      SDWORD sRGB[3][3] = {{1024,0,0}, {0,1024,0}, {0,0,1024}};
	
      CheckSignNum(&rgbGain[0][0]);
	  #if 1         
      if(ptr_GammaDescr)
      {
         sRGB[0][0] = (*ptr_GammaDescr)->Wp_3x3Values[4]; // G2
         sRGB[0][1] = (*ptr_GammaDescr)->Wp_3x3Values[7]; // G3
         sRGB[0][2] = (*ptr_GammaDescr)->Wp_3x3Values[1]; // G1
         sRGB[1][0] = (*ptr_GammaDescr)->Wp_3x3Values[5]; // B2
         sRGB[1][1] = (*ptr_GammaDescr)->Wp_3x3Values[8]; // B3
         sRGB[1][2] = (*ptr_GammaDescr)->Wp_3x3Values[2]; // B1
         sRGB[2][0] = (*ptr_GammaDescr)->Wp_3x3Values[3]; // R2
         sRGB[2][1] = (*ptr_GammaDescr)->Wp_3x3Values[6]; // R3
         sRGB[2][2] = (*ptr_GammaDescr)->Wp_3x3Values[0]; // R1                                
      }
      else
      {
         sRGB[0][0] = 0x400;
         sRGB[1][1] = 0x400;
         sRGB[2][2] = 0x400;
      }      
      #else
      switch (UserPrefPresetColorTemp)
      {
      case CTEMP_USER:
      {
            sRGB[0][0] = UserPrefUserGreenGain * 4;
            sRGB[1][1] = UserPrefUserBlueGain * 4;
            sRGB[2][2] = UserPrefUserRedGain * 4;
      }
         break;

      case CTEMP_5000K:
      case CTEMP_5700K:
      case CTEMP_6500K:
      case CTEMP_7500K:
      case CTEMP_9300K:
      case CTEMP_10000K:
      {
            if(ptr_GammaDescr)
         {
               sRGB[0][0] = (*ptr_GammaDescr)->Wp_3x3Values[4]; // G2
               sRGB[0][1] = (*ptr_GammaDescr)->Wp_3x3Values[7]; // G3
               sRGB[0][2] = (*ptr_GammaDescr)->Wp_3x3Values[1]; // G1
               sRGB[1][0] = (*ptr_GammaDescr)->Wp_3x3Values[5]; // B2
               sRGB[1][1] = (*ptr_GammaDescr)->Wp_3x3Values[8]; // B3
               sRGB[1][2] = (*ptr_GammaDescr)->Wp_3x3Values[2]; // B1
               sRGB[2][0] = (*ptr_GammaDescr)->Wp_3x3Values[3]; // R2
               sRGB[2][1] = (*ptr_GammaDescr)->Wp_3x3Values[6]; // R3
               sRGB[2][2] = (*ptr_GammaDescr)->Wp_3x3Values[0]; // R1                                
         }
         else
         {
            sRGB[0][0] = 0x400;
            sRGB[1][1] = 0x400;
            sRGB[2][2] = 0x400;
         }
      }
         break;
      }
	  #endif

      ccMatrix = &sRGB[0][0];

   #ifdef _DEBUG
      gm_Print("sYUVRGB 0 .................. 0x%x", ccMatrix[0]);
      gm_Print("sYUVRGB 1 .................. 0x%x", ccMatrix[1]);
      gm_Print("sYUVRGB 2 .................. 0x%x", ccMatrix[2]);
      gm_Print("sYUVRGB 3 .................. 0x%x", ccMatrix[3]);
      gm_Print("sYUVRGB 4 .................. 0x%x", ccMatrix[4]);
      gm_Print("sYUVRGB 5 .................. 0x%x", ccMatrix[5]);
      gm_Print("sYUVRGB 6 .................. 0x%x", ccMatrix[6]);
      gm_Print("sYUVRGB 7 .................. 0x%x", ccMatrix[7]);
      gm_Print("sYUVRGB 8 .................. 0x%x", ccMatrix[8]);
	#endif

      // B = YUV coef
      // A = xvYCC tbl
      //matrix_MxM(&rgbGain[0][0],ccMatrix);
      matrix_MxM(ccMatrix, &rgbGain[0][0]);

   #ifdef _DEBUG
      gm_Print("ccMatrix 0 .................. 0x%x", ccMatrix[0]);
      gm_Print("ccMatrix 1 .................. 0x%x", ccMatrix[1]);
      gm_Print("ccMatrix 2 .................. 0x%x", ccMatrix[2]);
      gm_Print("ccMatrix 3 .................. 0x%x", ccMatrix[3]);
      gm_Print("ccMatrix 4 .................. 0x%x", ccMatrix[4]);
      gm_Print("ccMatrix 5 .................. 0x%x", ccMatrix[5]);
      gm_Print("ccMatrix 6 .................. 0x%x", ccMatrix[6]);
      gm_Print("ccMatrix 7 .................. 0x%x", ccMatrix[7]);
      gm_Print("ccMatrix 8 .................. 0x%x", ccMatrix[8]);
	#endif

      W_Addr = ODP_CSC_BLOCK_START;

      // GAMMA_MATRIX_CTRL settings:
      // 00: Disable
         // 01: Inside DISP_LUT_CONTROL
         // 10: Outside DISP_LUT_CONTROL
      // 11: All pixels
      gm_SetRegBitsDWord(GAMMA_LUT2_CTRL, BIT12);

      gm_WriteRegWord (W_Addr + COEF_11, ccMatrix[0]);
      gm_WriteRegWord (W_Addr + COEF_12, ccMatrix[1]);
      gm_WriteRegWord (W_Addr + COEF_13, ccMatrix[2]);
      gm_WriteRegWord (W_Addr + COEF_21, ccMatrix[3]);
      gm_WriteRegWord (W_Addr + COEF_22, ccMatrix[4]);
      gm_WriteRegWord (W_Addr + COEF_23, ccMatrix[5]);
      gm_WriteRegWord (W_Addr + COEF_31, ccMatrix[6]);
      gm_WriteRegWord (W_Addr + COEF_32, ccMatrix[7]);
      gm_WriteRegWord (W_Addr + COEF_33, ccMatrix[8]);

         // ODP Input offset for xvYCC
         if((stInput[gmvb_MainChannel].Flags & gmd_INPUT_xvYCC_601) ||
            (stInput[gmvb_MainChannel].Flags & gmd_INPUT_xvYCC_709))    
         {
            // input offset = -Offset_OUT, -3923 = 0x70AD
            gm_WriteRegWord(GAMMA_LUT_MATRIX_IN_OFFSET1, 0x70AD);
            gm_WriteRegWord(GAMMA_LUT_MATRIX_IN_OFFSET2, 0x70AD);
            gm_WriteRegWord(GAMMA_LUT_MATRIX_IN_OFFSET3, 0x70AD);

         //gm_Printf(" ~~~~ xvYCC In Offset ...", 0);
         }
         else
         {
            gm_WriteRegWord(GAMMA_LUT_MATRIX_IN_OFFSET1, 0);
            gm_WriteRegWord(GAMMA_LUT_MATRIX_IN_OFFSET2, 0);
            gm_WriteRegWord(GAMMA_LUT_MATRIX_IN_OFFSET3, 0);            

         //gm_Printf(" ~~~~ No Offset ...", 0);
         }         

      // Offset
      #if 0 // Move to OVP block
      if(UserPrefColorTemp == CTEMP_USER)
      {
         gm_WriteRegWord(GAMMA_LUT_MATRIX_OUT_OFFSET1, UserPrefUserGreenOff*128);
         gm_WriteRegWord(GAMMA_LUT_MATRIX_OUT_OFFSET2, UserPrefUserBlueOff*128);
         gm_WriteRegWord(GAMMA_LUT_MATRIX_OUT_OFFSET3, UserPrefUserRedOff*128);      
      }
      else
      #endif
      {
         gm_WriteRegWord(GAMMA_LUT_MATRIX_OUT_OFFSET1, 0);
         gm_WriteRegWord(GAMMA_LUT_MATRIX_OUT_OFFSET2, 0);
         gm_WriteRegWord(GAMMA_LUT_MATRIX_OUT_OFFSET3, 0);      
      }
   }
    
	/*if(((B_PrePresetMode != Graphics_COLOR_SPACE) &&
		(UserPrefGraphicsImageMode != Graphics_COLOR_SPACE))) // Bypass USER_TEMP to change the gamma as request from VQ team.
    
   {
      gm_SyncUpdateWait(gmvb_MainChannel, OUTPUT_CLOCK);
	  gm_Print("####UserPrefGraphicsImageMode  %d",UserPrefGraphicsImageMode);
      return;
   }*/

   B_PrePresetMode = UserPrefGraphicsImageMode; 
   
     	
#ifdef ADJUSTER_GAMMATABLE_DECLARED
   	  AdjustGammaCorrection();
#endif

	msg_i("2### %ld", gm_GetSystemTime());
#if 1
	if(gm_WaitDisplayBlanking_CH(CH_A)) //KevinL 20120923 force update on blanking period immediatelly 
		gm_ForceUpdate(gmvb_MainChannel, OUTPUT_CLOCK);	
	else
		gm_SyncUpdateWait(gmvb_MainChannel, OUTPUT_CLOCK);
#else	
	gm_WaitDisplayBlanking(CH_A);		//Neil120801+ for update in blanking to reduce garbage
	gm_SyncUpdateWait(gmvb_MainChannel, OUTPUT_CLOCK);
#endif		

}
   //#endif // (NON_SCALING_OUTPUT == 0)
         
}


#else

void AdjustColorSpace(void)//standard code
{
#ifdef OUTPUT_MODE_CHANGE
if(UserPrefOutputMode == OPM_Normal)
#endif	
{
   //#if (NON_SCALING_OUTPUT == 0)
#ifdef SAVING_DATA_INTO_FLASH
   extern BYTE Gamma_Apply_Profile(BYTE far * Bp_Component, BYTE B_Selected_Profile);
   DWORD DW_FlashAddr = 0;
   BYTE B_ApplyComponent[5];

#ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()==CHIPID_4K2K_R)
   {
#if ENABLE_SOC_IPC_BUSY_CHECK
	/*ChengLin 20130124: New 4k2k board BUSY status check*/
	if(!SocIPCIsSlaveLeftReady())
		return;
#endif
   	SOCIPC_SyncOSDAdjustHandler( SI_OSD_SYNC_COLOR_CTRL );
	}
#endif

   if(UserPrefColorSpace < ColorSpace_XvYcc)
   {
      DW_FlashAddr = 0x5FA000 - (0x2000*UserPrefColorSpace);
   }

   if((FlashReadByte(DW_FlashAddr) != 0xFF) && (FlashReadByte(DW_FlashAddr+1) != 0xFF) && 
      (UserPrefColorSpace < ColorSpace_XvYcc) && DW_FlashAddr)
   {
      BYTE B_Return = 0;

      // first byte is dummy byte ...
      B_ApplyComponent[1] = FlashReadByte(DW_FlashAddr);
      //gm_Printf("AC 0x%x", B_ApplyComponent[1]);
      B_ApplyComponent[2] = FlashReadByte(DW_FlashAddr+1);
      B_ApplyComponent[3] = FlashReadByte(DW_FlashAddr+2);
      B_ApplyComponent[4] = FlashReadByte(DW_FlashAddr+3); 
      B_Return = Gamma_Apply_Profile((BYTE far *)&B_ApplyComponent, UserPrefColorSpace);
      gm_Printf("B_Return = %d", B_Return);      
      // Check 6-axis
      IsColorWarpApply();
   }
   else
   #endif
   {
      static BYTE B_PreColorSpace = 0xFF;
      static BYTE B_PreColorTemp = 0xFF;
      gmt_GammaDescr ROM * ROM *ptr_GammaDescr = GetGammaTable();

      // ODP 3x3 matrix update for color space
      {
         WORD W_Addr;
         SDWORD rgbGain[3][3] = {{0x400,0,0}, {0,0x400,0}, {0,0,0x400}};         
         SDWORD *ccMatrix;
         BOOL UpdateFalag;
         SDWORD sRGB[3][3] = {{1024,0,0}, {0,1024,0}, {0,0,1024}};
         
         CheckSignNum(&rgbGain[0][0]);

         #if 1         
         if(ptr_GammaDescr)
         {
            sRGB[0][0] = (*ptr_GammaDescr)->Wp_3x3Values[4]; // G2
            sRGB[0][1] = (*ptr_GammaDescr)->Wp_3x3Values[7]; // G3
            sRGB[0][2] = (*ptr_GammaDescr)->Wp_3x3Values[1]; // G1
            sRGB[1][0] = (*ptr_GammaDescr)->Wp_3x3Values[5]; // B2
            sRGB[1][1] = (*ptr_GammaDescr)->Wp_3x3Values[8]; // B3
            sRGB[1][2] = (*ptr_GammaDescr)->Wp_3x3Values[2]; // B1
            sRGB[2][0] = (*ptr_GammaDescr)->Wp_3x3Values[3]; // R2
            sRGB[2][1] = (*ptr_GammaDescr)->Wp_3x3Values[6]; // R3
            sRGB[2][2] = (*ptr_GammaDescr)->Wp_3x3Values[0]; // R1                                
         }
         else
         {
            sRGB[0][0] = 0x400;
            sRGB[1][1] = 0x400;
            sRGB[2][2] = 0x400;
         }      
         #else
         switch(UserPrefColorTemp)
         {
            case USER_TEMP:
            #if 0 // Move to OVP block
            {
               sRGB[0][0] = UserPrefUserGreenGain * 4;
               sRGB[1][1] = UserPrefUserBlueGain * 4;
               sRGB[2][2] = UserPrefUserRedGain * 4;
            }
            break;
            #endif
            case CTEMP_4000K:
            case CTEMP_5000K:         
            case CTEMP_6500K:
            case CTEMP_7500K:
            case CTEMP_8200K:
            case CTEMP_9300K:
            case CTEMP_10000K:   
            case CTEMP_11500K:
            {            
               if(ptr_GammaDescr)
               {
                  sRGB[0][0] = (*ptr_GammaDescr)->Wp_3x3Values[4]; // G2
                  sRGB[0][1] = (*ptr_GammaDescr)->Wp_3x3Values[7]; // G3
                  sRGB[0][2] = (*ptr_GammaDescr)->Wp_3x3Values[1]; // G1
                  sRGB[1][0] = (*ptr_GammaDescr)->Wp_3x3Values[5]; // B2
                  sRGB[1][1] = (*ptr_GammaDescr)->Wp_3x3Values[8]; // B3
                  sRGB[1][2] = (*ptr_GammaDescr)->Wp_3x3Values[2]; // B1
                  sRGB[2][0] = (*ptr_GammaDescr)->Wp_3x3Values[3]; // R2
                  sRGB[2][1] = (*ptr_GammaDescr)->Wp_3x3Values[6]; // R3
                  sRGB[2][2] = (*ptr_GammaDescr)->Wp_3x3Values[0]; // R1                                
               }
               else
               {
                  sRGB[0][0] = 0x400;
                  sRGB[1][1] = 0x400;
                  sRGB[2][2] = 0x400;
               }
            }
            break;
         }
         #endif


         ccMatrix = &sRGB[0][0];

         #ifdef _DEBUG
         gm_Print("sYUVRGB 0 .................. 0x%x", ccMatrix[0]);
         gm_Print("sYUVRGB 1 .................. 0x%x", ccMatrix[1]);
         gm_Print("sYUVRGB 2 .................. 0x%x", ccMatrix[2]);
         gm_Print("sYUVRGB 3 .................. 0x%x", ccMatrix[3]);
         gm_Print("sYUVRGB 4 .................. 0x%x", ccMatrix[4]);
         gm_Print("sYUVRGB 5 .................. 0x%x", ccMatrix[5]);
         gm_Print("sYUVRGB 6 .................. 0x%x", ccMatrix[6]);
         gm_Print("sYUVRGB 7 .................. 0x%x", ccMatrix[7]);
         gm_Print("sYUVRGB 8 .................. 0x%x", ccMatrix[8]);
         #endif

         // B = YUV coef
         // A = xvYCC tbl
         //matrix_MxM(&rgbGain[0][0],ccMatrix);
         matrix_MxM(ccMatrix, &rgbGain[0][0]);

         #ifdef _DEBUG
         gm_Print("ccMatrix 0 .................. 0x%x", ccMatrix[0]);
         gm_Print("ccMatrix 1 .................. 0x%x", ccMatrix[1]);
         gm_Print("ccMatrix 2 .................. 0x%x", ccMatrix[2]);
         gm_Print("ccMatrix 3 .................. 0x%x", ccMatrix[3]);
         gm_Print("ccMatrix 4 .................. 0x%x", ccMatrix[4]);
         gm_Print("ccMatrix 5 .................. 0x%x", ccMatrix[5]);
         gm_Print("ccMatrix 6 .................. 0x%x", ccMatrix[6]);
         gm_Print("ccMatrix 7 .................. 0x%x", ccMatrix[7]);
         gm_Print("ccMatrix 8 .................. 0x%x", ccMatrix[8]);
         #endif

         // GAMMA_MATRIX_CTRL settings:
         // 00: Disable
         // 01: Inside DISP_LUT_CONTROL
         // 10: Outside DISP_LUT_CONTROL
         // 11: All pixels
         gm_SetRegBitsDWord(GAMMA_LUT2_CTRL, BIT12);

         if(IsColorWarpApply() == FALSE)
         {
            W_Addr = ODP_CSC_BLOCK_START;

            gm_WriteRegWord (W_Addr + COEF_11, ccMatrix[0]);
            gm_WriteRegWord (W_Addr + COEF_12, ccMatrix[1]);
            gm_WriteRegWord (W_Addr + COEF_13, ccMatrix[2]);
            gm_WriteRegWord (W_Addr + COEF_21, ccMatrix[3]);
            gm_WriteRegWord (W_Addr + COEF_22, ccMatrix[4]);
            gm_WriteRegWord (W_Addr + COEF_23, ccMatrix[5]);
            gm_WriteRegWord (W_Addr + COEF_31, ccMatrix[6]);
            gm_WriteRegWord (W_Addr + COEF_32, ccMatrix[7]);
            gm_WriteRegWord (W_Addr + COEF_33, ccMatrix[8]);            
   
            // ODP Input offset for xvYCC
            if((stInput[gmvb_MainChannel].Flags & gmd_INPUT_xvYCC_601) ||
               (stInput[gmvb_MainChannel].Flags & gmd_INPUT_xvYCC_709))    
            {
               // input offset = -Offset_OUT, -3923 = 0x70AD
               gm_WriteRegWord(GAMMA_LUT_MATRIX_IN_OFFSET1, 0x70AD);
               gm_WriteRegWord(GAMMA_LUT_MATRIX_IN_OFFSET2, 0x70AD);
               gm_WriteRegWord(GAMMA_LUT_MATRIX_IN_OFFSET3, 0x70AD);
   
               //gm_Printf(" ~~~~ xvYCC In Offset ...", 0);
            }
            else
            {
               gm_WriteRegWord(GAMMA_LUT_MATRIX_IN_OFFSET1, 0);
               gm_WriteRegWord(GAMMA_LUT_MATRIX_IN_OFFSET2, 0);
               gm_WriteRegWord(GAMMA_LUT_MATRIX_IN_OFFSET3, 0);            
   
               //gm_Printf(" ~~~~ No Offset ...", 0);
            }                  
   
            // Offset
            #if 0 // Move to OVP block
            if(UserPrefColorTemp == USER_TEMP)
            {
               gm_WriteRegWord(GAMMA_LUT_MATRIX_OUT_OFFSET1, UserPrefUserGreenOff*128);
               gm_WriteRegWord(GAMMA_LUT_MATRIX_OUT_OFFSET2, UserPrefUserBlueOff*128);
               gm_WriteRegWord(GAMMA_LUT_MATRIX_OUT_OFFSET3, UserPrefUserRedOff*128);      
            }
            else
            #endif
            {
               gm_WriteRegWord(GAMMA_LUT_MATRIX_OUT_OFFSET1, 0);
               gm_WriteRegWord(GAMMA_LUT_MATRIX_OUT_OFFSET2, 0);
               gm_WriteRegWord(GAMMA_LUT_MATRIX_OUT_OFFSET3, 0);      
            }
         }
      }

   if(((B_PreColorSpace == UserPrefColorSpace) && (B_PreColorTemp == UserPrefColorTemp)) || 
      (UserPrefColorTemp == CTEMP_USER)) // Bypass USER_TEMP to change the gamma as request from VQ team.
   {
         if(UserPrefColorTemp == CTEMP_USER)
         {
            gm_WriteRegWord (DISP_LUT_CONTROL, BIT0|BIT1); // sRGB, Dither, OD need to set the window for main only ...
         }
         gm_SyncUpdateWait(gmvb_MainChannel, OUTPUT_CLOCK);
         return;
      }

      B_PreColorSpace = UserPrefColorSpace;
      B_PreColorTemp = UserPrefColorTemp;
         	
      #ifdef ADJUSTER_GAMMATABLE_DECLARED
      AdjustGammaCorrection();
      #endif      
   }
   gm_SyncUpdateWait(gmvb_MainChannel, OUTPUT_CLOCK);
  //#endif // (NON_SCALING_OUTPUT == 0)
}   
}

#endif //endif DELL_U2410F

#ifdef ADJUSTER_COLORTEMP_DECLARED
//******************************************************************************
//
// FUNCTION       : void AdjustColorTemp(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AdjustColorTemp(void)
{
#ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()==CHIPID_4K2K_R)
   {
#if ENABLE_SOC_IPC_BUSY_CHECK
	/*ChengLin 20130124: New 4k2k board BUSY status check*/
	if(!SocIPCIsSlaveLeftReady())
		return;
#endif               
      SOCIPC_SyncOSDAdjustHandler( SI_OSD_SYNC_COLOR_CTRL );
   }
#endif
	
#if 1
//   gm_Printf(" ~~~~ CSC Update Colors in AdjustColorTemp ", 0);
   CscUpdateColors(gmvb_MainChannel);   
#else
#if (NON_SCALING_OUTPUT == 0)
   static BYTE B_PreColorSpace = 0xFF;
   static BYTE B_PreColorTemp = 0xFF;
   gmt_GammaDescr ROM * ROM *ptr_GammaDescr = GetGammaTable();

   // ODP 3x3 matrix update for colortemp
   {
      WORD W_Addr;
      SDWORD rgbGain[3][3] = {{0x400,0,0}, {0,0x400,0}, {0,0,0x400}};
      SDWORD *ccMatrix;
      BOOL UpdateFalag;
      SDWORD sRGB[3][3] = {{1024,0,0}, {0,1024,0}, {0,0,1024}};

      CheckSignNum(&rgbGain[0][0]);

      switch (UserPrefColorTemp)
      {
      	case CTEMP_USER:
            {
               sRGB[0][0] = UserPrefUserGreenGain * 4;
               sRGB[1][1] = UserPrefUserBlueGain * 4;
               sRGB[2][2] = UserPrefUserRedGain * 4;
            }
            break;

         case CTEMP_4000K:
         case CTEMP_5000K:
         case CTEMP_6500K:
         case CTEMP_7500K:
         case CTEMP_8200K:   
         case CTEMP_9300K:
         case CTEMP_10000K:
         case CTEMP_11500K:
            {
               if (ptr_GammaDescr)
               {
               	sRGB[0][0] = (*ptr_GammaDescr)->Wp_3x3Values[4]; // G2
               	sRGB[0][1] = (*ptr_GammaDescr)->Wp_3x3Values[7]; // G3
               	sRGB[0][2] = (*ptr_GammaDescr)->Wp_3x3Values[1]; // G1
               	sRGB[1][0] = (*ptr_GammaDescr)->Wp_3x3Values[5]; // B2
               	sRGB[1][1] = (*ptr_GammaDescr)->Wp_3x3Values[8]; // B3
               	sRGB[1][2] = (*ptr_GammaDescr)->Wp_3x3Values[2]; // B1
               	sRGB[2][0] = (*ptr_GammaDescr)->Wp_3x3Values[3]; // R2
               	sRGB[2][1] = (*ptr_GammaDescr)->Wp_3x3Values[6]; // R3
               	sRGB[2][2] = (*ptr_GammaDescr)->Wp_3x3Values[0]; // R1                                
               }
               else
               {
                  sRGB[0][0] = 0x400;
                  sRGB[1][1] = 0x400;
                  sRGB[2][2] = 0x400;
               }
            }
            break;
      }


      ccMatrix = &sRGB[0][0];

#ifdef _DEBUG
      gm_Print("sYUVRGB 0 .................. 0x%x", ccMatrix[0]);
      gm_Print("sYUVRGB 1 .................. 0x%x", ccMatrix[1]);
      gm_Print("sYUVRGB 2 .................. 0x%x", ccMatrix[2]);
      gm_Print("sYUVRGB 3 .................. 0x%x", ccMatrix[3]);
      gm_Print("sYUVRGB 4 .................. 0x%x", ccMatrix[4]);
      gm_Print("sYUVRGB 5 .................. 0x%x", ccMatrix[5]);
      gm_Print("sYUVRGB 6 .................. 0x%x", ccMatrix[6]);
      gm_Print("sYUVRGB 7 .................. 0x%x", ccMatrix[7]);
      gm_Print("sYUVRGB 8 .................. 0x%x", ccMatrix[8]);
#endif

      // B = YUV coef
      // A = xvYCC tbl
      //matrix_MxM(&rgbGain[0][0],ccMatrix);
      matrix_MxM(ccMatrix, &rgbGain[0][0]);

#ifdef _DEBUG
      gm_Print("ccMatrix 0 .................. 0x%x", ccMatrix[0]);
      gm_Print("ccMatrix 1 .................. 0x%x", ccMatrix[1]);
      gm_Print("ccMatrix 2 .................. 0x%x", ccMatrix[2]);
      gm_Print("ccMatrix 3 .................. 0x%x", ccMatrix[3]);
      gm_Print("ccMatrix 4 .................. 0x%x", ccMatrix[4]);
      gm_Print("ccMatrix 5 .................. 0x%x", ccMatrix[5]);
      gm_Print("ccMatrix 6 .................. 0x%x", ccMatrix[6]);
      gm_Print("ccMatrix 7 .................. 0x%x", ccMatrix[7]);
      gm_Print("ccMatrix 8 .................. 0x%x", ccMatrix[8]);
#endif

      W_Addr = ODP_CSC_BLOCK_START;

      // GAMMA_MATRIX_CTRL settings:
      // 00: Disable
      // 01: Inside DISP_LUT_CONTROL
      // 10: Outside DISP_LUT_CONTROL
      // 11: All pixels
      gm_SetRegBitsDWord(GAMMA_LUT2_CTRL, BIT12);

      gm_WriteRegWord (W_Addr + COEF_11, ccMatrix[0]);
      gm_WriteRegWord (W_Addr + COEF_12, ccMatrix[1]);
      gm_WriteRegWord (W_Addr + COEF_13, ccMatrix[2]);
      gm_WriteRegWord (W_Addr + COEF_21, ccMatrix[3]);
      gm_WriteRegWord (W_Addr + COEF_22, ccMatrix[4]);
      gm_WriteRegWord (W_Addr + COEF_23, ccMatrix[5]);
      gm_WriteRegWord (W_Addr + COEF_31, ccMatrix[6]);
      gm_WriteRegWord (W_Addr + COEF_32, ccMatrix[7]);
      gm_WriteRegWord (W_Addr + COEF_33, ccMatrix[8]);

      // ODP Input offset for xvYCC
      if ((stInput[gmvb_MainChannel].Flags & gmd_INPUT_xvYCC_601) ||
            (stInput[gmvb_MainChannel].Flags & gmd_INPUT_xvYCC_709))
      {
         // input offset = -Offset_OUT, -3923 = 0x70AD
         gm_WriteRegWord(GAMMA_LUT_MATRIX_IN_OFFSET1, 0x70AD);
         gm_WriteRegWord(GAMMA_LUT_MATRIX_IN_OFFSET2, 0x70AD);
         gm_WriteRegWord(GAMMA_LUT_MATRIX_IN_OFFSET3, 0x70AD);

         //gm_Printf(" ~~~~ xvYCC In Offset ...", 0);
      }
      else
      {
         gm_WriteRegWord(GAMMA_LUT_MATRIX_IN_OFFSET1, 0);
         gm_WriteRegWord(GAMMA_LUT_MATRIX_IN_OFFSET2, 0);
         gm_WriteRegWord(GAMMA_LUT_MATRIX_IN_OFFSET3, 0);

         //gm_Printf(" ~~~~ No Offset ...", 0);
      }

      // Offset
      if(UserPrefColorTemp == CTEMP_USER)
      {
         gm_WriteRegWord(GAMMA_LUT_MATRIX_OUT_OFFSET1, UserPrefUserGreenOff*128);
         gm_WriteRegWord(GAMMA_LUT_MATRIX_OUT_OFFSET2, UserPrefUserBlueOff*128);
         gm_WriteRegWord(GAMMA_LUT_MATRIX_OUT_OFFSET3, UserPrefUserRedOff*128);
      }
      else
      {
         gm_WriteRegWord(GAMMA_LUT_MATRIX_OUT_OFFSET1, 0);
         gm_WriteRegWord(GAMMA_LUT_MATRIX_OUT_OFFSET2, 0);
         gm_WriteRegWord(GAMMA_LUT_MATRIX_OUT_OFFSET3, 0);
      }
   }

   if(((B_PreColorSpace == UserPrefColorSpace) && (B_PreColorTemp == UserPrefColorTemp)) || 
      (UserPrefColorTemp == CTEMP_USER)) // Bypass USER_TEMP to change the gamma as request from VQ team.
   {
      gm_SyncUpdateWait(gmvb_MainChannel, OUTPUT_CLOCK);
      return;
   }

   B_PreColorSpace = UserPrefColorSpace;
   B_PreColorTemp = UserPrefColorTemp;

#ifdef ADJUSTER_GAMMATABLE_DECLARED
   AdjustGammaCorrection();
#endif

#endif // (NON_SCALING_OUTPUT == 0)

   gm_SyncUpdateWait(gmvb_MainChannel, OUTPUT_CLOCK);
#endif   
}
#endif


#ifdef ADJUSTER_SCARTRGBVIAOVERLAY_DECLARED
//******************************************************************************
//
// FUNCTION       : void AdjustScartRGBViaOverlay(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        :
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AdjustScartRGBViaOverlay(void)
{
   //Stub function: Replace!
   gm_Printf("AdjustScartRGBViaOverlay: stub function");
   if (UserPrefScartRgbViaOverlay== TRUE)
      gm_Printf("On");
   else
      gm_Printf("Off");
}
#endif

#ifdef Action_AdjustImageScheme_WB_Used
//******************************************************************************
//
// FUNCTION       : void AdjustImageScheme(void)
//
// USAGE          : Adjust image scheme
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : UserPrefImageScheme      (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AdjustImageScheme(void)
{
#ifdef ST_4K2K_93xx_BOARD
	if(GetChipID_4K2K()==CHIPID_4K2K_R && (IgnoreInterChipCmd == FALSE))
	{
#if ENABLE_SOC_IPC_BUSY_CHECK
		/*ChengLin 20130124: New 4k2k board BUSY status check*/
		if(!SocIPCIsSlaveLeftReady())
			return;
#endif
		  SOCIPC_SyncOSDAdjustHandler( SI_OSD_SYNC_COLOR_CTRL );
	}
#endif		

   #if (PORT_SCHEME_DEPENDENT == SCHEME_DEPENDENT)
		//Neil 20120528+
		BYTE B_ColorFormat = UserPrefColorFormat;
		UserPrefImageScheme = InputConvertScheme(UserPrefCurrentInputMain);
		UserPrefPipImageScheme = InputConvertScheme(UserPrefCurrentInputPip);

	// Chnage Image Scheme at blanking to avoid garbage
	if (gm_IsValidSignal(gmvb_MainChannel))
         gm_WaitDisplayBlanking_CH(CH_A);

   if (CheckPortSchemeDependentEntry(UserPrefImageScheme)) // PSD
   {
      msg_i("PSD: Scheme change: User preferences retreived.", 0);
      ReadPortSchemeDependentEntry(UserPrefImageScheme); // PSD

	#ifdef DELL_U2410F	// Increased by ST_hansPENG 090216
		ReadPipPortSchemeDependent();
		ReadPresetModesSettings();		
	#endif
   }
   else
   {
      PortSchemeDependentUserPreferences = SchemeFactoryDefaultInROM[UserPrefImageScheme];
   }

   		UserPrefColorFormat = B_ColorFormat;		//Neil 20120528+ somehow this variable been changed after read scheme,restore here
   #else
   {
   	  
	  // Chnage Image Scheme at blanking to avoid garbage
	  if (gm_IsValidSignal(gmvb_MainChannel))
		   gm_WaitDisplayBlanking_CH(CH_A);

      if (UserPrefImageScheme == USER)
      {
         if (CheckPortSchemeDependentEntry(gmvb_InputConnector[gmvb_MainChannel])) // PSD
         {
            msg_i("PSD: Scheme change: User preferences retreived.", 0);
            ReadPortSchemeDependentEntry(gmvb_InputConnector[gmvb_MainChannel]); // PSD
         }
         else
         {
            PortSchemeDependentUserPreferences = SchemeFactoryDefaultInROM[UserPrefImageScheme];
         }
      }
      else
      {
         PortSchemeDependentUserPreferences = SchemeFactoryDefaultInROM[UserPrefImageScheme];
      }

      UserPrefMFMImageScheme = UserPrefImageScheme; // PSD
   }
#endif

	#ifdef DELL_U2410F//120209 Edward ACC depend on OSD setting 
	   AdjustDELLACCScheme();
	#endif

   // AdjustACCACMScheme         // PSD
   if (UserPrefImageScheme < NUM_IMAGE_SCHEMES)//CUSTOM is not available
   {
		//120209 Edward disable follow DELL 2410
		/*#if (FEATURE_ACC3 == ENABLE)
 			#if(!defined(ACC_LUT_METHOD))
      			gm_UpdateAcc(UserPrefImageScheme);
 			#endif
		#endif
		*/

#if (FEATURE_ACM_3D == ENABLE)
      AdjustACM3DColorScheme();
//#elif (FEATURE_ACM2 == ENABLE)
//      gm_UpdateColorScheme(UserPrefImageScheme);
#endif
   }

   SetupColorSpace(gmvb_MainChannel);

   //AdjustColorTemp();

}
#endif

#ifdef Action_AdjustDynamicContrast_WB_Used
//******************************************************************************
//
// FUNCTION       : void AdjustDynamicContrast(void)
//
// USAGE          : Adjust ACC
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : UserPrefDynamicContrast      (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AdjustDynamicContrast(void)
{
#if (FEATURE_ACC3 == ENABLE)
#if(!defined(ACC_LUT_METHOD))
   if (UserPrefDynamicContrast <= NUM_OF_ACC_TABLES)
      gm_UpdateAcc (UserPrefDynamicContrast);
#endif
#endif
}
#endif

//******************************************************************************
//
// FUNCTION       : BYTE GetMaxAccTable (void)
//
// USAGE          : Retreive Max ACC table
//
// INPUT          : None
//
// OUTPUT         : Max ACC table
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
BYTE GetMaxAccTable (void)
{
   BYTE B_MaxTable = NUM_OF_ACC_TABLES;
   msg_i ("Max ACC table is = %d", B_MaxTable);
   return (B_MaxTable);
}


#ifdef Action_FreezeDisplay_WB_Used
//******************************************************************************
//
// FUNCTION       : void FreezeDisplay(void)
//
// USAGE          : Toggle between Freeze main channel display and Unfreeze
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void FreezeDisplay(void)
{

   BYTE B_Freeze = gm_OsdGetActionByteParameter();

   if (BlenderIsPending(gmvb_MainChannel) == FALSE)
   {
      Freeze(gmvb_MainChannel, B_Freeze);
   }
}
#endif

#ifdef Action_ToggleFreezeVar_WB_Used
//******************************************************************************
//
// FUNCTION       : void ToggleFreezeVar(void)
//
// USAGE          : Toggle GlobalUserPreferences.Freeze for Display state use
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void ToggleFreezeVar(void)
{
   UserPrefFreeze ^= 1;
   if (UserPrefFreeze)
   {
      if (IsSideBySidePipMode(UserPrefPipMode))
      {
         if (BlenderIsPending(B_FocusBorder) == FALSE)
            Freeze(B_FocusBorder, TRUE);
      }
      else
      {
         if (BlenderIsPending(gmvb_MainChannel) == FALSE)
            Freeze(gmvb_MainChannel, TRUE);
      }
   }
   else
   {
      if (BlenderIsPending(gmvb_MainChannel) == FALSE)
         Freeze(gmvb_MainChannel, FALSE);
      if (BlenderIsPending(gmvb_PipChannel) == FALSE)
         Freeze(gmvb_PipChannel, FALSE);
   }
}
#endif

#ifdef Action_AdjustFleshToneLevel_WB_Used
//******************************************************************************
//
// FUNCTION       : void AdjustFleshToneLevel(void)
//
// USAGE          : Adjusts the flesh tone level based on UserPrefFleshToneComp
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : UserPrefFleshToneComp (R)
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AdjustFleshToneLevel(void)
{
#if 0 // (FEATURE_VIDEO_FLESHTONE == ENABLE)
   gmt_FleshToneStruct FleshToneStruct;

//   gmt_FleshToneCoefStruct FleshToneCoefStruct;
//   FleshToneCoefStruct->B_Coef_C1 = 1; //not needed since default curve in use
//   FleshToneCoefStruct->B_Coef_C2 = 2;
//   FleshToneCoefStruct->B_Coef_C3 = 4;
//   FleshToneCoefStruct->B_Coef_C4 = 8;
//   FleshToneCoefStruct->B_Coef_C5 = 16;
//   FleshToneCoefStruct->B_Coef_C6 = 32;
//   FleshToneCoefStruct->B_Coef_C7 = 64;
//   FleshToneCoefStruct->B_Coef_C8 = 128;

   // The engineering menu takes precedent over the user menu
#ifdef Action_AdjustNonACMFleshtoneCorrection_WB_Used
   if (UserPrefNonACMFleshtoneCorrection == ENABLE)
   {
      AdjustNonACMFleshtoneBalance();
      AdjustNonACMFleshtoneCorrectionLevel();
      return;
   }
#endif

   switch (UserPrefFleshToneComp)
   {
      case FT_LOW:
         FleshToneStruct.B_Gain = 0x2;
         break;

      case FT_MODERATE:
         FleshToneStruct.B_Gain = 0x4;
         break;

      case FT_HIGH:
         FleshToneStruct.B_Gain = 0xF;
         break;

      case FT_OFF:
      default:
         FleshToneStruct.B_Gain = 0x0;
   }

   FleshToneStruct.B_Region = gmd_FLESHTONE_INSIDE_AND_OUTSIDE_VIDEO_WIN;
   FleshToneStruct.Bt_0_TO_75_EN = TRUE;//default value
   FleshToneStruct.Bt_CurveAdj = FALSE;
   FleshToneStruct.B_Aperture = 0x4; //default value
   gm_DisplayFleshToneSetup(&FleshToneStruct, NULL_PTR);

   if (UserPrefFleshToneComp == FT_OFF)
      gm_VideoProcessingDisable(VIDEO_FLESHTONE);
   else
      gm_VideoProcessingEnable(VIDEO_FLESHTONE);

//#elif (FEATURE_ACM2 == ENABLE)
   St_AcmColorAdjust.B_AcmFleshTone = UserPrefFleshToneComp;
   gm_UpdateAcm (0, &St_AcmColorAdjust);

   if (!IsAcmUsed())
      gm_VideoProcessingDisable(VIDEO_ACM);

#elif (FEATURE_ACM_3D == ENABLE)
   AdjustACM3DColorScheme();

#endif
}
#endif


//******************************************************************************
//
// FUNCTION       : BOOL IsCCDefined(void)
//
// USAGE          : Returns true if CC is define, else false
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BOOL IsCCDefined(void)
{
   return(FALSE);
}

//******************************************************************************
//
// FUNCTION       : BOOL IsTTXDefined(void)
//
// USAGE          : Returns true if CC is define, else false
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BOOL IsTTXDefined(void)
{
   return(FALSE);
}

//******************************************************************************
//
// FUNCTION       : BOOL IsNoTunerDefined(void)
//
// USAGE          : Returns true if No Tuner is defined, else false
//
// INPUT          : None
//
// OUTPUT         : True or False
//
// GLOBALS        : None
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BOOL IsNoTunerDefined(void)
{
   return(FALSE);
}

#ifdef Action_ToggleInfoDisplay_WB_Used
//******************************************************************************
//
// FUNCTION       : void ToggleInfoDisplay(void)
//
// USAGE          : Toggles on/off speakers
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : UserPrefSpeakers      (RW)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void ToggleInfoDisplay(void)
{
   UserPrefInfoDisplayOn ^= 1;      // Toggles userpref and calls the adjustor function;
   //UserPrefDisplayPortName = 0;
}
#endif


#ifdef Action_SetOsdBitFlag_WB_Used
//******************************************************************************
//
// FUNCTION       : void SetOsdBitFlag(void)
//
// USAGE          : OSD flags for different actions
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
void SetOsdBitFlag(void)
{
   BYTE temp=gm_OsdGetCurrentSATOffsetByte();
   UserPrefOsdBitFlags |= temp;
}
#endif

#ifdef Action_ClrOsdBitFlag_WB_Used
//******************************************************************************
//
// FUNCTION       : void ClrOsdBitFlag(void)
//
// USAGE          : OSD flags for different actions
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
void ClrOsdBitFlag(void)
{
   BYTE temp=gm_OsdGetCurrentSATOffsetByte();
   UserPrefOsdBitFlags &= ~temp;
}
#endif

//******************************************************************************
//
// FUNCTION       : BOOL IsAutoScanDefined(void)
//
// USAGE          : Returns true if CC is define, else false
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BOOL IsAutoScanDefined(void)
{
#ifdef AUTO_SCAN_INPUT
   return(TRUE);
#else
   return(FALSE);
#endif
}

#ifdef Action_SimultaneousActionInDifferentThread_WB_Used
//******************************************************************************
//
// FUNCTION       : void SimultaneousActionInDifferentThread(void)
//
// USAGE          : to send a MTO message in order to make an action in different thread sumultaneously
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void SimultaneousActionInDifferentThread(void)
{
   BYTE temp=gm_OsdGetCurrentSATOffsetByte();
   switch (temp)
   {
      case ERASE_BLOCKED_CHANNEL:
         OSDRequestMsg(gmvb_MainChannel, MTO_EraseBlockedChannelNum, REQUEST_NOW);
         break;
      case DISPLAY_BLOCKED_CHANNEL:
         OSDRequestMsg(gmvb_MainChannel, MTO_DisplayBlockedChannelNum, REQUEST_NOW);
         break;
      default:
         return;
   }
}
#endif

#if 1 //def Action_SimultaneousActionInDifferentThread_WB_Used
//******************************************************************************
//
// FUNCTION       : void SetOsdReadyForPowerDown(void)
//
// USAGE          : to set global variable gmvb_OsdReadyForPowerDown
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : gmvb_OsdReadyForPowerDown
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void SetOsdReadyForPowerDown(void)
{
   OSDRequestCancelMsg(gmvb_MainChannel, REQ_ANY);
   OSDRequestCancelMsg(gmvb_PipChannel, REQ_ANY);   
	#ifdef USE_TIMER_CTRL_AUD_PWR//Neil120804+ follow U2410
	AUDIO_MUTE();		
	if ((UserPrefPwrAudio == ON) && AudioDetect()) //KevinL 20090225 for Power Save Audio function	
	{
		//gm_Print("~~~~ SetOsdReadyForPowerDown ~~AudioDetect() = %d~~", AudioDetect());
		PowerDownAudioSpeaker();		//Neil 20090217+
		//gm_Delay100ms(6);			//if enter power saving, need more delay for audio pop noise
	}
   #endif
   PowerDeepSleepInRequest();
}
#endif

#if 1 //def Action_SimultaneousActionInDifferentThread_WB_Used
//******************************************************************************
//
// FUNCTION       : void SetOsdRequestForPowerUp(void)
//
// USAGE          : to set global variable gmvb_OsdRequestForPowerUp
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : gmvb_OsdRequestForPowerUp
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void SetOsdRequestForPowerUp(void)
{
#if(IS_PROJ_PD())
   CMD_PowerState = ON;
#endif
   PowerDeepSleepOutRequest();
}
#endif

//******************************************************************************
//
// FUNCTION       : BYTE IsNoSync(void)
//
// USAGE          : to get current status of signal is NoSignal
//
// INPUT          : None
//
// OUTPUT         : current status of signal
//
// GLOBALS        : B_SignalStatus
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BOOL IsNoSync(void)
{
   return (B_SignalStatus == STATUS_NO_SYNC);
}

//******************************************************************************
//
// FUNCTION       : BYTE IsNoCable(void)
//
// USAGE          : to get current status of signal is NoCable
//
// INPUT          : None
//
// OUTPUT         : current status of signal
//
// GLOBALS        : B_SignalStatus
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BOOL IsNoCable(void)
{
#ifdef CHECK_IS_CABLE_CONNECT
   return (B_SignalStatus == STATUS_NO_CABLE);
#else
   return FALSE;
#endif
}

//******************************************************************************
//
// FUNCTION       : BYTE IsInvalidMode(void)
//
// USAGE          : to get current status of signal is InvalidMode
//
// INPUT          : None
//
// OUTPUT         : current status of signal
//
// GLOBALS        : B_SignalStatus
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BOOL IsInvalidMode(void)
{
   return (B_SignalStatus == STATUS_INVALID_MODE);
}


//******************************************************************************
//
// FUNCTION       : BYTE ToggleMenuStyle()
//
// USAGE          : to user to define whether to use the new or old menu style
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

BOOL ToggleMenuStyle(void)
{
   if (UserPrefOldOrNewPortChange)
      UserPrefOldOrNewPortChange = 0;
   else
      UserPrefOldOrNewPortChange = 1;

   return (UserPrefOldOrNewPortChange);
}

//******************************************************************************
//
// FUNCTION       : BOOL IsPipFocusRight(void)
//
// USAGE          : determines if focus of side by side pip mode
//
// INPUT          : None
//
// OUTPUT         : returns true if PiP mode is side by side and focus is on right
//
// GLOBALS        : B_FocusBorder
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BOOL IsPipFocusRight(void)
{
   if ((IsSideBySidePipMode(UserPrefPipMode) == TRUE) &&
         (B_FocusBorder == gmvb_PipChannel))
   {
      return TRUE;
   }
   else
   {
      return FALSE;
   }
}

//******************************************************************************
//
// FUNCTION       : BOOL IsPipNoSync(void)
//
// USAGE          : determines if the pip signal is not present
//
// INPUT          : None
//
// OUTPUT         : returns true PIP has no signal
//
// GLOBALS        : gm_TimingIsStable, gmvb_PipChannel
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BOOL IsPipNoSync(void)
{
   if ((gm_TimingIsStable(gmvb_PipChannel) == FALSE) || (UserPrefPipMode == 0))
   {
      return TRUE;
   }
   else
   {
      return FALSE;
   }
}

//******************************************************************************
//
// FUNCTION       : void ScheduleNoSignalMsg(void)
//
// USAGE          : Schedules a request for OSD to display No Signal message if There is No Sync in the signal
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : gm_MainChannel, gmvb_PipChannel
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void ScheduleNoSignalMsg(void)
{
   if ((IsNoSync())&&(!IsPipFocusRight())) //&&(UserPrefSpeakers == D_MUTE_ON))
      {
	#ifdef DELL_U2410F
		OSDRequestMsg(gmvb_MainChannel, MTO_NoSync, NO_SIGNAL_DELAY);		 
	#else
         OSDRequestMsg(gmvb_MainChannel, MTO_NoSignal, NO_SIGNAL_DELAY);
	#endif
      }
   else if ((IsPipNoSync())&&(IsPipFocusRight())) //&&(UserPrefSpeakers == D_MUTE_ON))
      {
	#ifdef DELL_U2410F  
		OSDRequestMsg(gmvb_MainChannel, MTO_NoSync, NO_SIGNAL_DELAY);			 
	#else	 
         OSDRequestMsg(gmvb_PipChannel, MTO_NoSignal, NO_SIGNAL_DELAY);         
	#endif
      }
}   

//******************************************************************************
//
// FUNCTION       : void CancelNoSignalMsg(void)
//
// USAGE          : Cancels any previous request for OSD to display No Signal message
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : gm_MainChannel, gmvb_PipChannel
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void CancelNoSignalMsg(void)
{
    #ifdef DELL_U2410F  
	OSDRequestCancelMsg(gmvb_MainChannel, MTO_NoSync);
	OSDRequestCancelMsg(gmvb_PipChannel, MTO_NoSync);
    #else	
	OSDRequestCancelMsg(gmvb_MainChannel, MTO_NoSignal);
	OSDRequestCancelMsg(gmvb_PipChannel, MTO_NoSignal);
    #endif	
}

//******************************************************************************
//
// FUNCTION       : BOOL IsHDMIDefined(void)
//
// USAGE          : Returns true if HDMI is define, else false
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BOOL IsHDMIDefined(void)
{
#if (FEATURE_HDMI == ENABLE)
   return(TRUE);
#else
   return(FALSE);
#endif
}

//******************************************************************************
//
// FUNCTION       : BOOL IsAUDIO_LS_HP_INDEPENDENTDefined(void)
//
// USAGE          : Returns true if AUDIO_LS_HP_INDEPENDENT is define, else false
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
BOOL IsAUDIO_LS_HP_INDEPENDENTDefined(void)
{
#ifdef  AUDIO_LS_HP_INDEPENDENT
   return(TRUE);
#else
   return(FALSE);
#endif
}

BOOL IsGameModeApplicable(void)
{
   BOOL B_Result = TRUE;

#ifdef DYNAMIC_OUTPUT
	  if((abs(stInput[gmvb_MainChannel].Vfrq - 600) > 10) && (stInput[gmvb_MainChannel].Hactive <= 1920))
		  B_Result = FALSE;
		  
	  if((abs(stInput[gmvb_MainChannel].Vfrq - 300) > 10) && (stInput[gmvb_MainChannel].Hactive > 1920))
		 B_Result = FALSE;
#else
	  if(abs(stInput[gmvb_MainChannel].Vfrq - 600) > 10)
		 B_Result = FALSE;
#endif

   if(gm_ReadRegWord(IMP_CONTROL)&IMP_YUV_EN)
      B_Result = FALSE;

   if(UserPrefPipMode != NO_PIP)
      B_Result = FALSE;

   return B_Result;
}

void AdjustGameMode(void)
{
   if (UserPrefGameMode == 0)
   {
		UserPrefGameMode = 1;

      //if(abs(stInput[gmvb_MainChannel].Vfrq - 600) < 10)
      if(IsGameModeApplicable())
      {         
         B_DisplayLockMethod = DISP_LOCK_NORMAL;			
         #if (ENABLE_DFL == 0)
         gm_SetRegBitsWord(DDDS1_CONTROL, DDDS1_FORCE_OPLOOP);
         #endif
      }
      else
      {
         //UserPrefGameMode = 0;
         #if (ENABLE_DFL == 1)
         B_DisplayLockMethod = DISP_LOCK_DFL;
         #else
         B_DisplayLockMethod = DISP_FREERUN;
         #endif
      }
   }
   else
   {
      UserPrefGameMode = 0;

      #if (ENABLE_DFL == 1)
      B_DisplayLockMethod = DISP_LOCK_DFL;
      #else
      B_DisplayLockMethod = DISP_FREERUN;
      gm_SetRegBitsWord(DDDS1_CONTROL, DDDS1_FORCE_OPLOOP);
      #endif
   }

   if (gm_IsGraphics(gmvb_MainChannel))
   {
      ModeSetupRequest(gmvb_MainChannel, REQ_MS_RESTART, PARAM_NONE, 300);
   }

   if (gm_IsGraphics(gmvb_PipChannel))
   {
      ModeSetupRequest(gmvb_PipChannel, REQ_MS_RESTART, PARAM_NONE, 300);
   }
}



void AdjustXvYcc(void)
{
   if (UserPrefXvYcc== 0)
      UserPrefXvYcc = 1;
   else if (UserPrefXvYcc== 1)
      UserPrefXvYcc = 2;
   else
      UserPrefXvYcc = 0;
}

void AdjustDCR(void)
{
#if (FEATURE_ACR == ENABLE)
#ifdef ST_4K2K_93xx_BOARD
if(GetChipID_4K2K()==CHIPID_4K2K_R)
{
#if ENABLE_SOC_IPC_BUSY_CHECK
	/*ChengLin 20130124: New 4k2k board BUSY status check*/
	if(!SocIPCIsSlaveLeftReady())
		return;
#endif            
}
#endif

   if (UserPrefDCR == OFF)
   {
      UserPrefDCR = ON;
#ifdef ST_4K2K_93xx_BOARD
	  if(GetChipID_4K2K()==CHIPID_4K2K_R)
	  {
		  SOCIPC_SyncOSDAdjustHandler( SI_OSD_SYNC_SETUP);
	  }
#endif
      ACR_Enable();
   }
   else
   {
      UserPrefDCR = OFF;
#ifdef ST_4K2K_93xx_BOARD
	if(GetChipID_4K2K()==CHIPID_4K2K_R)
	{
		SOCIPC_SyncOSDAdjustHandler( SI_OSD_SYNC_SETUP);
	}
#endif	  
      ACR_Disable();
   }
#endif
}

#ifdef ENABLE_DCR
#if 0//110830 Kevin disable temp
void DCR_Handler( void)
{
	#ifndef DELL_OSD
   WORD TestBright = 0;
   //BYTE CurrentState = 0;
   WORD /*RanBrightness = 0,*/ TempBright = 0, RGBBright = 0;
   SWORD Total_Bright =0;
   //BYTE i = 0;

   if (UserPrefDCR)
   {

      TestBright = gm_ReadRegWord(IMP_BRIGHTNESS);


      if (TestBright > HighBrightness)
      {
         HighBrightness = TestBright;

         //gm_Print("HighBrightness  ======= %d", HighBrightness);
      }

      if ((TestBright < LowBrightness) && (TestBright != 0))
      {
         LowBrightness = TestBright;

         //gm_Print("LowBrightness  ======= %d", LowBrightness);
      }


      if (abs(TestBright - AVGBrightness) < 0x30)
      {
         TempBright = gm_ReadRegWord(PWM3_CTRL) >> 8 & 0x00FF;

         if (TempBright != TargetBright)
         {
            gm_TimerStart (DELAY_1ms_TMR, 100);
            // clear VBLANK edge from previous frame
            gm_WriteRegWord (MAIN_INPUT_STATUS, IMP_VACTIVE);
            while ((gm_ReadRegWord (MAIN_INPUT_STATUS) & IMP_VACTIVE) == 0)
            {
               if (gm_TimerCheck (DELAY_1ms_TMR) == TIMER_TMO)
                  break;	// timeout - return with error
            }

            if (TempBright > TargetBright)
               gm_WriteRegWord(PWM3_CTRL, ((TempBright - 1) << 8 | 0xFF));
            else if (TempBright < TargetBright)
               gm_WriteRegWord(PWM3_CTRL, ((TempBright + 1) << 8 | 0xFF));
         }
         return;
      }

      if ((gm_ReadRegWord(IMP_CONTROL)&IMP_YUV_EN))
      {
         Total_Bright = ((TestBright - LowBrightness) >> 4) * 0xFF;
         Total_Bright = Total_Bright / ((HighBrightness - LowBrightness) >>4);
         TargetBright = Total_Bright & 0x00FF;
      }
      else
      {
         gm_WriteRegWord(IMP_BIN_CONFIG,0xFFFF);
         RGBBright = gm_ReadRegWord(IMP_BIN_SUM);
         if (RGBBright > 0xF00)
         {
            TargetBright = 0xFF;
         }
         else if ((RGBBright < 0x200) && (TestBright == 0xFFF))
         {
            TargetBright = 0x0;
         }
         else
         {
            if (RGBBright > TestBright)
               TargetBright = (RGBBright >> 4) & 0x00FF;
            else
               TargetBright = (TestBright >> 4)  & 0x00FF;
         }
      }

      //gm_Print("Current Bright = ............. %d", TestBright);
      //gm_Print("TargetBright Bright = ............. %d", TargetBright);
      gm_Print("HighBrightness Bright = ............. %d", HighBrightness);
      //gm_Print("LowBrightness Bright = ............. %d", LowBrightness);


      //gm_Print("DCR State change ++++++++++++++++++ %d",  CurrentState);
      AVGBrightness = TestBright;
      //gm_Print("AVG Bright ================ %d", AVGBrightness);

   }
   else
   {
      if (gm_ReadRegWord(PWM3_CTRL) !=  0xFFFF)
         gm_WriteRegWord(PWM3_CTRL, 0xFFFF);
   }
   #endif
}
#endif
#endif // ENABLE_DCR


ValueScaleConvertType vConverT;

void ValueScaleConvert(ValueScaleConvertType* ptr)//Lvjie 20081028 moved from 2709
{
	WORD W_Interval;
	if(ptr->SW_DstMaxValue | ptr->SW_DstMinValue)
	{
		// We want to map the associate value to a percentage of the way through
		// our current range.
		W_Interval = (WORD)(ptr->SDW_SrcMax - ptr->SDW_SrcMin);

		if( W_Interval != 0 )
		{
			ptr->SDW_AdjValue =  (( ptr->SDW_AdjValue - ptr->SDW_SrcMin ) *
							 ( ptr->SW_DstMaxValue - ptr->SW_DstMinValue )) / W_Interval
							 + ptr->SW_DstMinValue;
		}
		else
		{
			ptr->SDW_AdjValue = ptr->SW_DstMinValue;
		}
	}
}

//************************************* END ************************************

